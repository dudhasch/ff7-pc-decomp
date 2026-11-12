// src/ff7/src/main/main.cpp
// Original: C:\FF7\Src\main\main.cpp
// Contents: Holds the application's main loop glue: startup and shutdown, the game-mode dispatcher,
//   window message handling and input-state blocks, and the default view setup. It also saves and
//   restores the savemap field entry point, opens and closes the archives around a disc swap, and
//   draws the debug text.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/api/coaster.h"
#include "ff7/api/field.h"
#include "ff7/api/highway.h"
#include "ff7/api/main.h"
#include "lib/api/graphics.h"
#include "lib/api/movie.h"
#include "lib/api/token.h"
#include "ff7/enums/main.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "ff7/api/chocobo.h"
#include "ff7/api/submarine.h"
#include "ff7/api/credits.h"
#include "ff7/api/condor.h"
#include "ff7/api/menu.h"
#include "ff7/api/snobo.h"
#include "ff7/api/movie.h"
#include "lib/api/file.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "ff7/api/wm.h"
#include "ff7/types/main.h"
#include "lib/types/graphics.h"
#include "lib/types/file.h"
#include "ff7/savemap.h"
#include <mmsystem.h>
#include "lib/enums/file.h"
#include "ff7/types/field.h"
#include "ff7/field.h"
extern "C" {

#pragma intrinsic(strcpy, strcat, memset)
struct GfxEngineContext;
// clang-format off
// GLOBAL: FF7 0x007bab24
int g_Main_BlankScreenEnabled = 1;
char *g_Main_VersionString[1] = {
    // STRING: FF7 0x007bab2c
    "Version #: 052198"
};
// GLOBAL: FF7 0x009a06c8
int g_Main_EndingMovieStarted = 0;
// GLOBAL: FF7 0x009a06cc
int g_Main_FieldEntryPointActive = 0;
// GLOBAL: FF7 0x009a06d0
int g_Main_HeapTrackingEnabled = 0;
unsigned char g_bssPad_009a06d4[4] = {0};
// GLOBAL: FF7 0x009a06d8
MainAppContext g_Main_AppContext = {0};
// clang-format on
void __cdecl App_DrawDebugTextGdi(int x, int y, const char *message, HDC dc);

// GLOBAL: FF7 0x007b6658
static const char REG_GRAPHICS_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Graphics";
// GLOBAL: FF7 0x007b6698
static const char REG_SOUND_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Sound";
// GLOBAL: FF7 0x007b66d0
static const char REG_MIDI_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII\\1.00\\Midi";
// GLOBAL: FF7 0x007b6708
static const char REG_DD_GUID[] = "DD_GUID";
// GLOBAL: FF7 0x007b6710
static const char REG_MIDI_DEVICEID[] = "MIDI_DeviceID";
// GLOBAL: FF7 0x007b6720
static const char REG_SOUND_GUID[] = "Sound_GUID";

/*
 * Clears g_FieldPreloadState and the 16-bit value at g_FieldPreloadMapTarget.
 */
// FUNCTION: FF7 0x00408010
void __cdecl Field_ClearMapTransitionRequest(void)
{
    g_FieldPreloadState = 0;
    g_FieldPreloadMapTarget = 0;
}

/*
 * Plays a sound effect unless bits 0x30 of the flag byte gSavemap.soundMuteFlags are set: issues
 * SoundDispatchCommand(0xF1, 0, ...) and then SfxPlayCachedBuffer with the low 16 bits of sfxId as
 * the id, 0x40 as the pan and 0x7F as the volume.
 */
// FUNCTION: FF7 0x00408027
void __cdecl App_PlayCachedSfxUnlessMuted(int sfxId)
{
    if ((gSavemap.soundMuteFlags & 0x30) == 0) {
        SoundDispatchCommand(SOUND_SFX_STOP_ALL, 0, 0, 0, 0, 0, 0, 0, 0);
        SfxPlayCachedBuffer(sfxId & 0xFFFF, 0x40, 0x7F, 0);
    }
}

/*
 * Empty hook the dispatcher calls for GAME_MODE_REENTER_FIELD (0x10); performs no work.
 */
// FUNCTION: FF7 0x0040806f
void __cdecl GameModeDispatcher_ReenterFieldNoOp(void)
{
}

/*
 * Sets g_Main_FieldEntryPointActive to 1 and restores the field entry point from the savemap: the
 * field return mode, the game mode (GAME_MODE_NONE becomes GAME_MODE_FIELD) into g_EngineGameMode,
 * the location id into g_FieldEntryMapId and the field actor state, the player's coordinates
 * and direction, and the two field random indices.
 */
// FUNCTION: FF7 0x00408074
void __cdecl Savemap_RestoreFieldEntryPoint(void)
{
    g_Main_FieldEntryPointActive = 1;
    g_FieldReturnMode = gSavemap.fieldReturnMode;
    g_EngineGameMode = gSavemap.gameMode;
    if (g_EngineGameMode == GAME_MODE_NONE) {
        g_EngineGameMode = GAME_MODE_FIELD;
    }
    g_FieldEntryMapId = gSavemap.locationId;
    g_FieldActorState.state.mapId = gSavemap.locationId;
    g_FieldActorState.state.pcTileX = gSavemap.coordX;
    g_FieldActorState.state.pcTileY = gSavemap.coordY;
    g_FieldActorState.state.pcWalkMeshId = gSavemap.coordT;
    *(short *)&g_FieldActorState.state.pcDirection = *(unsigned char *)&gSavemap.coordD;
    g_FieldRandomIndex1 = gSavemap.fieldRandomIndex1;
    g_FieldRandomIndex2 = gSavemap.fieldRandomIndex2;
}

/*
 * Stores the field return mode and the game mode to come back to: (2, GAME_MODE_WORLD_MAP) on the
 * world map, otherwise (0, GAME_MODE_FIELD); captures the field location, the player entity's
 * coordinates shifted right by 12, its walkmesh triangle and direction, and
 * g_FieldRandomIndex1/g_FieldRandomIndex2.
 */
// FUNCTION: FF7 0x00408116
void __cdecl Savemap_CaptureFieldEntryPoint(void)
{
    if (g_EngineGameMode == GAME_MODE_WORLD_MAP) {
        gSavemap.fieldReturnMode = 2;
        gSavemap.gameMode = GAME_MODE_WORLD_MAP;
    } else {
        gSavemap.fieldReturnMode = 0;
        gSavemap.gameMode = GAME_MODE_FIELD;
    }
    gSavemap.locationId = g_FieldEntryMapId;
    gSavemap.coordX = g_FieldEntityState[g_FieldPlayerEntityIndex].posX >> 0xC;
    gSavemap.coordY = g_FieldEntityState[g_FieldPlayerEntityIndex].posY >> 0xC;
    gSavemap.coordT = (short)g_FieldEntityState[g_FieldPlayerEntityIndex].posI;
    gSavemap.coordD = (char)g_FieldEntityState[g_FieldPlayerEntityIndex].moveDir;
    gSavemap.fieldRandomIndex1 = g_FieldRandomIndex1;
    gSavemap.fieldRandomIndex2 = g_FieldRandomIndex2;
}

// Resets the savemap's field entry point. Clears g_Main_FieldEntryPointActive, sets
// gSavemap.discChangeRequest to 1, and writes the entry record in gSavemap
// (gSavemap.fieldReturnMode): fieldReturnMode 0, gameMode GAME_MODE_FIELD, locationId 0x74, and
// zero for coordX, coordY, coordT, coordD, fieldRandomIndex1 and fieldRandomIndex2. It is plain
// field-by-field initialisation.
// FUNCTION: FF7 0x004081e1
void __cdecl Savemap_ResetFieldEntryPoint(void)
{
    g_Main_FieldEntryPointActive = 0;
    gSavemap.discChangeRequest = 1;
    gSavemap.fieldReturnMode = 0;
    gSavemap.gameMode = GAME_MODE_FIELD;
    gSavemap.locationId = 0x74;
    gSavemap.coordX = 0;
    gSavemap.coordY = 0;
    gSavemap.coordT = 0;
    gSavemap.coordD = 0;
    gSavemap.fieldRandomIndex1 = 0;
    gSavemap.fieldRandomIndex2 = 0;
}

/*
 * Dispatches sound opcode 0xF4, calls SfxSaveChannelLoopRecords, copies the field state's
 * eventCmdParam (the battle id) into g_FieldRequestedBattleId and its battleMode2 word into
 * g_BattleResultWord, ORs in the signed byte g_FieldEncounterState and clears that byte, then
 * dispatches opcode 0xF5.
 */
// FUNCTION: FF7 0x00408243
void __cdecl Battle_SetupEntryFromField(void)
{
    SoundDispatchCommand(SOUND_PSX_NOP_F4, 0, 0, 0, 0, 0, 0, 0, 0);
    SfxSaveChannelLoopRecords();
    g_FieldRequestedBattleId = g_FieldActorState.state.eventCmdParam;
    *(short *)&g_BattleResultWord = (short)g_FieldActorState.state.battleMode2;
    *(short *)&g_BattleResultWord |= (short)(char)g_FieldEncounterState;
    g_FieldEncounterState = 0;
    SoundDispatchCommand(SOUND_PSX_NOP_F5, 0, 0, 0, 0, 0, 0, 0, 0);
}

/*
 * Opens char, flevel, chocobo, condor, world and moviecam archives in that order with IDs zero,
 * one, seven, six, five and fourteen. On the first failure logs the constructed path, sets
 * g_EngineGameMode to nineteen and returns zero, leaving earlier opens intact. Returns one only after
 * all six succeed; path construction has no length checks.
 */
// FUNCTION: FF7 0x004082bf
int __cdecl App_OpenCoreArchives(void)
{
    char path[256];
    char message[256];
    int success;
    success = 0;
    strcpy(path, g_InitPath_CdDataPath);
    strcat(path,
           // STRING: FF7 0x007bab40
           "field/char.lgp");
    if (!OpenLgpArchive(path, LGP_ARCHIVE_FIELD_CHAR)) {
        sprintf(message,
                // STRING: FF7 0x007bab50
                "Failed to load: %s\n", path);
        Gfx_DebugPrint(message);
        g_EngineGameMode = GAME_MODE_QUIT;
        return success;
    }
    strcpy(path, g_InitPath_CdDataPath);
    strcat(path,
           // STRING: FF7 0x007bab64
           "field/flevel.lgp");
    if (!OpenLgpArchive(path, LGP_ARCHIVE_FIELD_LEVEL)) {
        sprintf(message,
                // STRING: FF7 0x007bab78
                "Failed to load: %s\n", path);
        Gfx_DebugPrint(message);
        g_EngineGameMode = GAME_MODE_QUIT;
        return success;
    }
    strcpy(path, GetMinigameDirForChocobo());
    strcat(path,
           // STRING: FF7 0x007bab8c
           "chocobo.lgp");
    if (!OpenLgpArchive(path, LGP_ARCHIVE_CHOCOBO)) {
        sprintf(message,
                // STRING: FF7 0x007bab98
                "Failed to load: %s\n", path);
        Gfx_DebugPrint(message);
        g_EngineGameMode = GAME_MODE_QUIT;
        return success;
    }
    strcpy(path, GetMinigameDirForCondor());
    strcat(path,
           // STRING: FF7 0x007babac
           "condor.lgp");
    if (!OpenLgpArchive(path, LGP_ARCHIVE_CONDOR)) {
        sprintf(message,
                // STRING: FF7 0x007babb8
                "Failed to load: %s\n", path);
        Gfx_DebugPrint(message);
        g_EngineGameMode = GAME_MODE_QUIT;
        return success;
    }
    strcpy(path, g_InitPath_CdDataPath);
    strcat(path,
           // STRING: FF7 0x007babcc
           "wm/world_us.lgp");
    if (!OpenLgpArchive(path, LGP_ARCHIVE_WORLD)) {
        sprintf(message,
                // STRING: FF7 0x007babdc
                "Failed to load: %s\n", path);
        Gfx_DebugPrint(message);
        g_EngineGameMode = GAME_MODE_QUIT;
        return success;
    }
    strcpy(path, g_InitPath_DataPath);
    strcat(path,
           // STRING: FF7 0x007babf0
           "cd/moviecam.lgp");
    if (!OpenLgpArchive(path, LGP_ARCHIVE_MOVIE_CAMERA)) {
        sprintf(message,
                // STRING: FF7 0x007bac00
                "Failed to load: %s\n", path);
        Gfx_DebugPrint(message);
        g_EngineGameMode = GAME_MODE_QUIT;
        return 0;
    }
    success = 1;
    return success;
}

/*
 * Closes the six LGP archive slots 0, 1, 7, 6, 5 and 0xE through CloseLgpArchive, in that order,
 * ahead of a disc swap. Nothing else.
 */
// FUNCTION: FF7 0x00408706
void __cdecl Lgp_CloseArchivesForDiscSwap(void)
{
    CloseLgpArchive(LGP_ARCHIVE_FIELD_CHAR);
    CloseLgpArchive(LGP_ARCHIVE_FIELD_LEVEL);
    CloseLgpArchive(LGP_ARCHIVE_CHOCOBO);
    CloseLgpArchive(LGP_ARCHIVE_CONDOR);
    CloseLgpArchive(LGP_ARCHIVE_WORLD);
    CloseLgpArchive(LGP_ARCHIVE_MOVIE_CAMERA);
}

/*
 * Sets the supplied DC background to 0xff0000 and text color to 0xffff, then draws the complete
 * message at (x, y) with TextOutA.
 */
// FUNCTION: FF7 0x00408747
void __cdecl App_DrawDebugTextGdi(int x, int y, const char *message, HDC dc)
{
    SetBkColor(dc, 0xff0000);
    SetTextColor(dc, 0xffff);
    TextOutA(dc, x, y, message, lstrlenA(message));
}

/*
 * Free the input-state block (MainInputState) and the RSD model it owns: the model handle
 * rsdModel goes back through the polygon library, then the object itself goes back to the tracked heap, with
 * main.cpp's __FILE__ and line 558.
 * The literal is the first of the file's two copies of its own path.
 */
// FUNCTION: FF7 0x0040878b
void __cdecl App_FreeInputStateAndRsdModel(MainInputState *obj, GfxEngineContext *ctx)
{
    if (obj != 0) {
        Pfile_FreeRsdModel((void *)obj->rsdModel, (void *)ctx);
        // STRING: FF7 0x007bac14
        GameHeapFree(obj, "C:\\FF7\\Src\\main\\main.cpp", 0x22E);
    }
}

// Clears the twenty-two per-key words of an input-state block, starting at f1DebugToggle,
// then raises its enable flags resetFlagA, resetFlagB, resetFlagC, resetFlagD and resetFlagE. A null block is left alone; the
// null test wraps the whole body.
// FUNCTION: FF7 0x004087c2
void __cdecl App_ResetInputStateFlags(MainInputState *state)
{
    int i;

    if (state != 0) {
        for (i = 0; i < 0x16; i++) {
            (&state->f1DebugToggle)[i] = 0;
        }
        state->resetFlagA = 1;
        state->resetFlagB = 1;
        state->resetFlagC = 1;
        state->resetFlagD = 1;
        state->resetFlagE = 1;
    }
}

/*
 * Allocate the 0x90-byte input-state block from the tracked heap (main.cpp
 * line 593, the file's second copy of its path), set its four header words
 * and reset its flags. Returns the block, or 0 when the heap refused.
 */
// FUNCTION: FF7 0x0040882a
MainInputState *__cdecl App_AllocInputState(void)
{
    MainInputState *state;

    // STRING: FF7 0x007bac30
    state = (MainInputState *)GameHeapAlloc(1, 0x90, "C:\\FF7\\Src\\main\\main.cpp", 0x251);
    if (state != 0) {
        state->initHeaderA = 1;
        state->initHeaderB = 1;
        state->initHeaderC = 4;
        state->initHeaderD = 1;
        App_ResetInputStateFlags(state);
    }
    return state;
}

#pragma intrinsic(strcpy, strcat, strlen)


/*
 * Builds a default view on the stack and installs it into the frame context frameContext. Zeroes the first
 * slot of the stack record and sets two further slots to the float bit patterns 1250.0
 * (0x449C4000) and 2500.0 (0x451C4000), passes the record and a second stack buffer to
 * BuildTranslationMatrixRowVec, then hands that 4x4 matrix to Gfx_SetContextMatrix with frameContext.
 */
// FUNCTION: FF7 0x0040888a
void __cdecl App_InstallDefaultViewMatrix(GfxEngineContext *frameContext)
{
    GfxDirectionVec3 translation;
    GfxMatrix4x4 matrixBuffer;

    translation.x = 0;
    translation.y = FLOAT_1250_BITS; /* 1250.0f */
    translation.z = FLOAT_2500_BITS; /* 2500.0f */
    BuildTranslationMatrixRowVec(&translation, &matrixBuffer);
    Gfx_SetContextMatrix(&matrixBuffer, frameContext);
}

/*
 * Blanks the screen around a display mode switch, on the frame context frameContext. Sets
 * frameContext->presentEnable to 1 for the duration, and when g_Main_BlankScreenEnabled is non-zero picks the viewport from
 * GetUiScalingMode (mode 2 gives 640x480 at the origin, mode 1 gives 320x240 at (160, 120), mode 0
 * gives 320x240 at the origin), sets the background colour to black with alpha 1.0, and clears and
 * presents the frame twice. presentEnable is cleared again on the way out.
 */
// FUNCTION: FF7 0x004088c9
void __cdecl App_BlankScreenForModeSwitch(GfxEngineContext *frameContext)
{
    int uiMode;
    GfxClearColorBgra bgColor;

    uiMode = GetUiScalingMode();
    frameContext->presentEnable = 1;
    if (g_Main_BlankScreenEnabled != 0) {
        bgColor.r = 0;
        bgColor.g = 0;
        bgColor.b = 0;
        bgColor.a = FLOAT_ONE_BITS;
        if (uiMode == 2) {
            GfxSetViewportRect(0, 0, 0x280, 0x1E0, frameContext);
        } else if (uiMode == 1) {
            GfxSetViewportRect(0xA0, 0x78, 0x140, 0xF0, frameContext);
        } else if (uiMode == 0) {
            GfxSetViewportRect(0, 0, 0x140, 0xF0, frameContext);
        }
        GfxSetBackgroundColorCall(&bgColor, frameContext);
        GfxClearAllCall(frameContext);
        PresentFrame(frameContext);
        GfxClearAllCall(frameContext);
        PresentFrame(frameContext);
    }
    frameContext->presentEnable = 0;
}

/*
 * Clears startup controls and returns zero immediately for GAME_MODE_QUIT (19). Optionally reports heap
 * usage, allocates input state and attempts graphics initialization. On graphics success
 * initializes input/repeat rates and field entry, optional sound/MIDI, menu/battle/magic plus core
 * archives, display modes/minigame and field resources, default model flags and COM. Sound/archive
 * failures return early with GAME_MODE_QUIT (19) and no rollback; COM failure is logged but continues.
 * Hides the cursor until its count is negative, installs view/kernel/save defaults and sound
 * command, then marks success. Even graphics failure proceeds to GAME_MODE_CREDITS (27), volume
 * settings and one cursor hide before returning zero.
 */
// FUNCTION: FF7 0x004089c5
int __cdecl App_InitSubsystems(struct GfxEngineContext *ctx)
{
    // Names preserve the original local slots: two paths, display flags, result and COM status.
    char path[256];
    int softwareMode;
    char message[256];
    int success;
    int comStatus;
    int scalingMode;
    softwareMode = 0;
    g_BattleDebugKeyState = 0;
    g_Main_EndingMovieStarted = 0;
    if (g_EngineGameMode == GAME_MODE_QUIT) {
        return 0;
    }
    if (g_Main_HeapTrackingEnabled != 0) {
        PrintGameHeapUsage();
    }
    g_Main_AppContext.inputState = App_AllocInputState();
    success = 0;
    if (InitGraphicsDevice(ctx) != 0) {
        comStatus = 0;
        Menu_ClearUiTextureHandles();
        Input_Init(ctx);
        InputConfig_LoadFromFile();
        Input_SetRepeatRates(200, 50);
        Savemap_ResetFieldEntryPoint();
        if (g_InitPath_SoundEnabled != 0) {
            if (SoundInit(g_InitPath_SoundPath, g_InitPath_MusicPath, ctx->hwnd) == 0) {
                g_EngineGameMode = GAME_MODE_QUIT;
                return 0;
            }
            SfxLoadIntoBuffer(SFX_ID_PRELUDE_CHIME, 0);
        }
        if (g_InitPath_MidiEnabled != 0) {
            MidiInit(g_InitPath_MidiPath, ctx->hwnd);
        }
        strcpy(path, g_InitPath_DataPath);
        strcat(path,
               // STRING: FF7 0x007bac4c
               "menu/menu_us.lgp");
        if (OpenLgpArchive(path, LGP_ARCHIVE_MENU) == 0) {
            sprintf(message,
                    // STRING: FF7 0x007bac60
                    "Failed to load: %s\n", path);
            Gfx_DebugPrint(message);
            g_EngineGameMode = GAME_MODE_QUIT;
            return success;
        }
        strcpy(path, g_InitPath_DataPath);
        strcat(path,
               // STRING: FF7 0x007bac74
               "battle/battle.lgp");
        if (OpenLgpArchive(path, LGP_ARCHIVE_BATTLE) == 0) {
            sprintf(message,
                    // STRING: FF7 0x007bac88
                    "Failed to load: %s\n", path);
            Gfx_DebugPrint(message);
            g_EngineGameMode = GAME_MODE_QUIT;
            return 0;
        }
        strcpy(path, g_InitPath_DataPath);
        strcat(path,
               // STRING: FF7 0x007bac9c
               "battle/magic.lgp");
        if (OpenLgpArchive(path, LGP_ARCHIVE_MAGIC) == 0) {
            sprintf(message,
                    // STRING: FF7 0x007bacb0
                    "Failed to load: %s\n", path);
            Gfx_DebugPrint(message);
            g_EngineGameMode = GAME_MODE_QUIT;
            return 0;
        }
        if (App_OpenCoreArchives() == 0) {
            return success;
        }
        scalingMode = GetUiScalingMode();
        if (ctx->driverType == GFX_DRIVER_SOFTWARE) {
            softwareMode = 1;
        }
        Pfile_SetAssetSourceMode(0);
        Field_SetDisplayMode(softwareMode, scalingMode);
        Mini_ChocoboApplyUiScalingModeConfig(softwareMode, scalingMode);
        Mini_CondorApplyViewportPreset(softwareMode, scalingMode);
        Mini_ChocoboRaceSetAssetDirPath(GetMinigameDirForChocobo());
        Mini_CondorSetAssetDirPath(GetMinigameDirForCondor());
        strcpy(path, g_InitPath_CdDataPath);
        strcat(path,
               // STRING: FF7 0x007bacc4
               "field/");
        Field_InitModuleResources(ctx, path, path);
        Pfile_SetDefaultModelFlags(0x100000);
        App_InitHookNoOp();
        comStatus = CoInitialize(0);
        if (comStatus < 0) {
            Trace_Printf(
                // STRING: FF7 0x007baccc
                "Failed to initialize COM (0x%8.8X)\n", comStatus);
        }
        while (ShowCursor(0) >= 0) {
        }
        if (g_Main_HeapTrackingEnabled != 0) {
            PrintGameHeapUsage();
        }
        App_InstallDefaultViewMatrix(ctx);
        success = 1;
        g_MenuEntryState = 0;
        Kernel_LoadKernelBinFiles();
        Savemap_ResetConfigDefaults();
        Field_ClearMapTransitionRequest();
        g_FieldActorState.state.battleMode2 = 0;
        g_FieldActorState.state.encountersDisabled = 0;
        Kernel_ResetLoadState();
        SoundDispatchCommand(SOUND_MUSIC_VOLUME, 0x7f, 0, 0, 0, 0, 0, 0, 0);
    }
    g_EngineGameMode = GAME_MODE_CREDITS;
    g_EnginePreviousMode = GAME_MODE_NONE;
    ApplyRegistryVolumeSettings();
    ShowCursor(0);
    return success;
}

/*
 * Sets GAME_MODE_QUIT (19) and tears down clock, field, paths, archives, input, graphics, movie, COM and
 * helper state. Conditionally shuts down sound/MIDI, skips Menu_ReleaseAllRenderObjects in GAME_MODE_GAME_OVER (26), and prints
 * heap usage/leaks when the debug flag is set.
 */
// FUNCTION: FF7 0x00408edc
void __cdecl App_Shutdown(struct GfxEngineContext *ctx)
{
    int result;
    g_EngineGameMode = GAME_MODE_QUIT;
    Movie_FreeCameraDataBuffer();
    SystemParametersInfoA(SPI_SCREENSAVERRUNNING, FALSE, &result, 0);
    Savemap_StopClockTimer();
    Field_ShutdownModule(ctx);
    App_FreeInputStateAndRsdModel(g_Main_AppContext.inputState, ctx);
    Engine_FreePathGlobals();
    CloseAllLgpArchives();
    if (g_InitPath_SoundEnabled != 0) {
        Sfx_ReleaseCachedBuffer(0x2B, 15);
        SoundShutdown();
    }
    if (g_InitPath_MidiEnabled != 0) {
        MidiShutdown();
    }
    Input_Shutdown();
    if (g_EnginePreviousMode != GAME_MODE_GAME_OVER) {
        Menu_ReleaseAllRenderObjects();
    }
    Field_FreeModuleResources();
    World_ReleaseResourcesIfSuspended();
    ShutdownGraphicsDevice(ctx);
    Movie_ReleaseGraph();
    CoUninitialize();
    App_ShutdownHookNoOp();
    if (g_Main_HeapTrackingEnabled != 0) {
        PrintGameHeapUsage();
        DumpGameHeapLeaks();
    }
}

/*
 * Logs entry, redirects disc-sensitive modes through GAME_MODE_DISC_CHECK (12) when the requested disc differs,
 * and clears the context flag. The rest reads the field request byte g_FieldActorState.state.requestCode, not
 * the previous mode: for an ending movie, a game over or the world-map ending movie it clears the
 * encounter state; a game over selects GAME_MODE_GAME_OVER (26) after resetting the music volume, the field entry
 * point, the kernel data and the config; the world-map ending movie selects GAME_MODE_ENDING_MOVIE (25).
 */
// FUNCTION: FF7 0x00408fa6
void __cdecl GameModeDispatcher_Enter(GfxEngineContext *ctx)
{
    Gfx_DebugPrint(
        // STRING: FF7 0x007bacf0
        "Entering MAIN\n");
    if ((g_EnginePreviousMode == GAME_MODE_FIELD || g_EnginePreviousMode == GAME_MODE_CONTINUE_GAME || g_EnginePreviousMode == GAME_MODE_NEW_GAME || g_EnginePreviousMode == GAME_MODE_TITLE_MENU) && gSavemap.discChangeRequest != CdCheck_GetGameDiscNumber()) {
        g_EnginePreviousMode = g_EngineGameMode;
        g_EngineGameMode = GAME_MODE_DISC_CHECK;
    }
    ctx->presentEnable = 0;
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_ENDING_MOVIE || g_FieldActorState.state.requestCode == FIELD_REQUEST_GAME_OVER || g_FieldActorState.state.requestCode == FIELD_REQUEST_WORLD_ENDING_MOVIE) {
        g_FieldEncounterTriggered = 0;
        g_FieldEncounterStepCounter = 0;
        g_FieldEncounterRateAccum = 0;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_GAME_OVER) {
        g_FieldActorState.state.requestCode = FIELD_REQUEST_NONE;
        g_EngineGameMode = GAME_MODE_GAME_OVER;
        SoundDispatchCommand(SOUND_MUSIC_VOLUME, 0x7F, 0, 0, 0, 0, 0, 0, 0);
        Savemap_ResetFieldEntryPoint();
        Kernel_LoadKernelBinFiles();
        Savemap_ResetConfigDefaults();
        Field_ClearMapTransitionRequest();
    } else if (g_FieldActorState.state.requestCode == FIELD_REQUEST_WORLD_ENDING_MOVIE) {
        g_FieldActorState.state.requestCode = FIELD_REQUEST_NONE;
        g_EngineGameMode = GAME_MODE_ENDING_MOVIE;
    }
}

/*
 * Leaves the game-mode dispatcher, the module that runs between two game modes and starts the
 * next one. It logs "Exiting MAIN" and turns presenting back on: it sets the engine context's
 * `presentEnable`, which GameModeDispatcher_Enter cleared so that PresentFrame shows
 * nothing while the dispatcher runs. Every module that ends (field, battle, world map, menu,
 * credits, the minigames) hands control back by installing the dispatcher's callbacks, this one
 * among them.
 */
// FUNCTION: FF7 0x004090c7
void __cdecl GameModeDispatcher_Exit(GfxEngineContext *ctx)
{
    // STRING: FF7 0x007bad00
    Gfx_DebugPrint("Exiting MAIN\n");
    ctx->presentEnable = 1;
}

/*
 * Gets active module state and, only when the window is not minimized, blits an active movie, handles its
 * stop request and dispatches the global mode. Routes field, battle, world, menu, swirl, disc
 * check, credits and minigame callback sets; submarine selects asset source one and null input
 * handlers, field nulls exit. Ending mode opens/runs ending2.avi once then releases it and selects
 * ending credits. Other branches capture/restore field entry, initialize battle flags, increment a
 * two-byte encounter counter, refresh party tables, handle seven script services, reset new-game
 * state/start timer, or send window close. Credits archive/timer failures set mode nineteen.
 * Unknown modes do nothing after movie handling.
 */
// FUNCTION: FF7 0x004090e6
void __cdecl GameModeDispatcher_Frame(GfxEngineContext *ctx)
{
    // Codegen: the locals are named for their frame slots; VC5's bucket order of these names gives
    // the original 0x62C-byte frame, and the switches add eight bytes of temporaries.
    int keepGoing;
    GameModule module;
    char messageBuf[256];
    char moviePathBuffer[256];
    int sceneArg;
    int ddrawObjB;
    char endingFailMsg[256];
    char endingName[256];
    char failMsgA[256];
    char creditsName[256];
    int ddrawObjA;

    GetActiveModule(&module, ctx);
    keepGoing = 1;
    if (ctx->minimized == 0) {
        if (Movie_IsNotPlaying() == 0) {
            sceneArg = 0;
            Gfx_BeginSceneCall(sceneArg, ctx);
            ctx->presentEnable = 1;
            MovieBltFrameToBackBuffer(GfxGetSurfaceByIndex(1));
            Gfx_EndSceneCall(ctx);
        }
        if (Movie_IsStopRequested() != 0) {
            Movie_Stop();
        }
        switch (g_EngineGameMode) {
        case GAME_MODE_ENDING_MOVIE:
            ddrawObjA = 0;
            ddrawObjB = 0;
            if (g_Main_EndingMovieStarted == 0) {
                App_BlankScreenForModeSwitch(ctx);
                g_Main_EndingMovieStarted = 1;
                SfxStopAll();
                g_EngineGameMode = GAME_MODE_ENDING_MOVIE;
                strcpy(moviePathBuffer, g_InitPath_MoviePath);
                strcat(moviePathBuffer,
                       // STRING: FF7 0x007bad10
                       "ending2.avi");
                sprintf(messageBuf,
                        // STRING: FF7 0x007bad1c
                        "Opening movie: %s\n", moviePathBuffer);
                Gfx_DebugPrint(messageBuf);
                ddrawObjA = ctx->ddObject;
                ddrawObjB = ctx->ddObject2;
                Movie_Open((int)moviePathBuffer, 0, ddrawObjA, ddrawObjB);
                Movie_Run();
            } else {
                if (Movie_IsNotPlaying() != 0) {
                    Movie_ReleaseGraph();
                    g_EngineGameMode = GAME_MODE_ENDING_CREDITS;
                }
            }
            g_EnginePreviousMode = GAME_MODE_ENDING_MOVIE;
            break;
        case GAME_MODE_GAME_OVER:
            App_BlankScreenForModeSwitch(ctx);
            Savemap_StopClockTimer();
            module.enter = (GameModuleCallback)GameOver_ModuleEnter;
            module.frame = GameOver_ModuleFrame;
            module.onMouse = (GameModuleMessageCallback)GameOver_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)GameOver_ModuleKeyboard;
            module.exit = (GameModuleCallback)GameOver_ModuleExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_REENTER_FIELD:
            GameModeDispatcher_ReenterFieldNoOp();
            (g_FieldActorState.state.requestArg) = 2;
            g_EnginePreviousMode = GAME_MODE_REENTER_FIELD;
            g_EngineGameMode = GAME_MODE_FIELD;
            break;
        case GAME_MODE_BATTLE_SWIRL:
            module.enter = Swirl_ModuleEnter;
            module.frame = (GameModuleCallback)Swirl_ModuleFrame;
            module.exit = (GameModuleCallback)Swirl_ModuleExit;
            module.onMouse = (GameModuleMessageCallback)Swirl_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)Swirl_ModuleKeyboard;
            RequestModuleSwitch(&module, ctx);
            (gSavemap.battlesLo)++;
            if ((gSavemap.battlesLo) == 0) {
                (gSavemap.battlesHi)++;
            }
            if (g_PartyStatsDirty == 1) {
                Savemap_ReloadActivePartyStats();
                Menu_RefreshPartyLimitTables();
                g_PartyStatsDirty = 0;
            }
            if (g_EnginePreviousMode == GAME_MODE_FIELD) {
                if (g_FieldMoviePending == 0) {
                    if ((g_FieldActorState.state.nextBattleMusic) != 0) {
                        SoundDispatchCommand(SOUND_MUSIC_PLAY_ALT, (g_FieldActorState.state.nextBattleMusic), 0, 0, 0, 0, 0, 0, 0);
                    }
                    Battle_SetupEntryFromField();
                }
            } else {
                if (g_WorldBattleRequest & 0x80000000) {
                    g_FieldEncounterState = FIELD_BATTLE_FLAG_PREEMPTIVE;
                } else {
                    g_FieldEncounterState = 0;
                }
                if (g_WorldBattleRequest & WORLD_BATTLE_REQ_COUNTDOWN) {
                    g_FieldEncounterState |= FIELD_BATTLE_FLAG_COUNTDOWN;
                }
                (g_FieldActorState.state.eventCmdParam) = g_WorldBattleRequest & WORLD_BATTLE_REQ_ID_MASK;
                Battle_SetupEntryFromField();
            }
            break;
        case GAME_MODE_DISC_CHECK:
            App_BlankScreenForModeSwitch(ctx);
            if (gSavemap.discChangeRequest != CdCheck_GetGameDiscNumber()) {
                Lgp_CloseArchivesForDiscSwap();
                module.enter = (GameModuleCallback)CdCheck_ModuleEnter;
                module.frame = CdCheck_ModuleFrame;
                module.onMouse = (GameModuleMessageCallback)CdCheck_ModuleMouse;
                module.onKey = (GameModuleMessageCallback)CdCheck_ModuleKeyboard;
                module.exit = (GameModuleCallback)CdCheck_ModuleExit;
                RequestModuleSwitch(&module, ctx);
            } else {
                (g_FieldActorState.state.requestArg) = 2;
                g_EnginePreviousMode = GAME_MODE_DISC_CHECK;
                g_EngineGameMode = GAME_MODE_FIELD;
            }
            break;
        case GAME_MODE_CREDITS:
            Pfile_SetAssetSourceMode(0);
            strcpy(creditsName, g_InitPath_DataPath);
            strcat(creditsName,
                   // STRING: FF7 0x007bad30
                   "cd/cr_us.lgp");
            if (OpenLgpArchive(creditsName, LGP_ARCHIVE_CREDITS) == 0) {
                sprintf(failMsgA,
                        // STRING: FF7 0x007bad40
                        "Failed to load: %s\n", creditsName);
                Gfx_DebugPrint(failMsgA);
                g_EngineGameMode = GAME_MODE_QUIT;
                break;
            }
            if (g_EnginePreviousMode == GAME_MODE_GAME_OVER) {
                Credits_SetStartSection(0);
            } else {
                Credits_SetStartSection(7);
            }
            Credits_SetVariant(0);
            module.enter = (GameModuleCallback)Credits_ModuleEnter;
            module.frame = Credits_ModuleFrame;
            module.onMouse = (GameModuleMessageCallback)Credits_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)Credits_ModuleKeyboard;
            module.exit = (GameModuleCallback)Credits_ModuleExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_ENDING_CREDITS:
            Pfile_SetAssetSourceMode(0);
            strcpy(endingName, g_InitPath_DataPath);
            strcat(endingName,
                   // STRING: FF7 0x007bad54
                   "cd/cr_us.lgp");
            if (OpenLgpArchive(endingName, LGP_ARCHIVE_CREDITS) == 0) {
                sprintf(endingFailMsg,
                        // STRING: FF7 0x007bad64
                        "Failed to load: %s\n", endingName);
                Gfx_DebugPrint(endingFailMsg);
                g_EngineGameMode = GAME_MODE_QUIT;
                break;
            }
            App_BlankScreenForModeSwitch(ctx);
            Credits_SetVariant(1);
            module.enter = (GameModuleCallback)Credits_ModuleEnter;
            module.frame = Credits_ModuleFrame;
            module.onMouse = (GameModuleMessageCallback)Credits_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)Credits_ModuleKeyboard;
            module.exit = (GameModuleCallback)Credits_ModuleExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_TITLE_MENU:
            Pfile_SetAssetSourceMode(0);
            SoundDispatchCommand(SOUND_MUSIC_VOLUME, 0x7f, 0, 0, 0, 0, 0, 0, 0);
            Menu_CreateLoadScreenObjects();
            module.enter = Menu_ModuleEnter;
            module.frame = Menu_ModuleFrame;
            module.onMouse = (GameModuleMessageCallback)Menu_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)Menu_ModuleKeyboard;
            module.exit = (GameModuleCallback)Menu_ModuleExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_NEW_GAME:
            App_BlankScreenForModeSwitch(ctx);
            Pfile_SetAssetSourceMode(0);
            Savemap_ResetConfigDefaults();
            Savemap_ResetForNewGame();
            Savemap_ResetFieldEntryPoint();
            Savemap_RestoreFieldEntryPoint();
            Menu_RefreshPartyLimitTables();
            g_EnginePreviousMode = GAME_MODE_NONE;
            if (Savemap_StartClockTimer() == 0) {
                g_EngineGameMode = GAME_MODE_QUIT;
            }
            break;
        case GAME_MODE_CONTINUE_GAME:
            App_BlankScreenForModeSwitch(ctx);
            Pfile_SetAssetSourceMode(0);
            if (Movie_IsGraphOpen() != 0) {
                Movie_ReleaseGraph();
            }
            Savemap_RestoreFieldEntryPoint();
            Menu_RefreshPartyLimitTables();
            g_EnginePreviousMode = GAME_MODE_NONE;
            if (Savemap_StartClockTimer() == 0) {
                g_EngineGameMode = GAME_MODE_QUIT;
            }
            break;
        case GAME_MODE_QUIT:
            SendMessageA((void *)ctx->hwnd, WM_CLOSE, ctx->result, 0);
            break;
        case GAME_MODE_FIELD_SERVICE:
            Savemap_CaptureFieldEntryPoint();
            switch (g_FieldActorState.state.requestCode) {
            case FIELD_REQUEST_HARVEST_MATERIA:
                Script_SvcHarvestMateria();
                break;
            case FIELD_REQUEST_RETURN_MATERIA:
                Script_SvcReturnMateria();
                break;
            case FIELD_REQUEST_STRIP_CHARACTER:
                Script_SvcStripCharacter((int)(g_FieldActorState.state.eventCmdParam));
                break;
            case FIELD_REQUEST_SCALE_FIRST_FIVE_HP:
                Script_SvcScaleFirstFiveHp();
                break;
            case FIELD_REQUEST_CHECK_MATERIA_SET:
                Script_SvcCheckMateriaSet((int)(g_FieldActorState.state.eventCmdParam));
                break;
            case FIELD_REQUEST_REPLACE_MATERIA_GROUP:
                Script_SvcReplaceMateriaGroup((int)(g_FieldActorState.state.eventCmdParam));
                break;
            case FIELD_REQUEST_SNAPSHOT_LEVELS:
                Savemap_SnapshotLevelsToBank();
                break;
            }
            if (g_FieldActorState.state.requestCode != FIELD_REQUEST_REENTER_FIELD) {
                Savemap_ReconcileFieldParty();
            }
            (g_FieldActorState.state.requestArg) = 2;
            g_EnginePreviousMode = GAME_MODE_FIELD_SERVICE;
            g_EngineGameMode = GAME_MODE_FIELD;
            break;
        case GAME_MODE_MENU:
            App_BlankScreenForModeSwitch(ctx);
            Pfile_SetAssetSourceMode(0);
            Savemap_CaptureFieldEntryPoint();
            if (g_PartyStatsDirty == 1) {
                Savemap_ReloadActivePartyStats();
                Menu_RefreshPartyLimitTables();
                g_PartyStatsDirty = 0;
            }
            module.enter = Menu_ModuleEnter;
            module.frame = Menu_ModuleFrame;
            module.onMouse = (GameModuleMessageCallback)Menu_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)Menu_ModuleKeyboard;
            module.exit = (GameModuleCallback)Menu_ModuleExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_MENU_AFTER_BATTLE:
            Pfile_SetAssetSourceMode(0);
            module.enter = Menu_ModuleEnter;
            module.frame = Menu_ModuleFrame;
            module.onMouse = (GameModuleMessageCallback)Menu_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)Menu_ModuleKeyboard;
            module.exit = (GameModuleCallback)Menu_ModuleExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_BATTLE:
            App_BlankScreenForModeSwitch(ctx);
            Pfile_SetAssetSourceMode(0);
            module.frame = Battle_ModuleFrame;
            module.enter = Battle_ModuleEnter;
            module.exit = Battle_ModuleExit;
            module.onKey = 0;
            module.onMouse = 0;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_FIELD:
            App_BlankScreenForModeSwitch(ctx);
            Pfile_SetAssetSourceMode(0);
            Gfx_SetRenderStateCall(RENDER_STATE_TEXTURE_WRAP, 1, ctx);
            module.frame = Field_ModuleFrame;
            module.onMouse = (GameModuleMessageCallback)Field_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)Field_ModuleKeyboard;
            module.enter = Field_ModuleEnter;
            module.exit = 0;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_WORLD_MAP:
            Pfile_SetAssetSourceMode(0);
            Savemap_CaptureFieldEntryPoint();
            if (g_FieldReturnMode != 2) {
                if (g_EnginePreviousMode == GAME_MODE_BATTLE) {
                    g_FieldReturnMode = 1;
                } else {
                    g_FieldReturnMode = 0;
                }
                g_FieldNextModuleArg = g_FieldEntryMapId;
            }
            module.frame = WorldMap_ModuleFrame;
            module.onMouse = (GameModuleMessageCallback)WorldMap_ModuleMouse;
            module.onKey = (GameModuleMessageCallback)WorldMap_ModuleKeyboard;
            module.enter = WorldMap_ModuleEnter;
            module.exit = (GameModuleCallback)WorldMap_ModuleExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_CHOCOBO_RACE:
            Pfile_SetAssetSourceMode(0);
            App_BlankScreenForModeSwitch(ctx);
            module.frame = Mini_ChocoboFrame;
            module.onMouse = (GameModuleMessageCallback)Mini_ChocoboMouse;
            module.onKey = (GameModuleMessageCallback)Mini_ChocoboKeyboard;
            module.enter = Mini_ChocoboEnter;
            module.exit = (GameModuleCallback)Mini_ChocoboExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_FORT_CONDOR:
            Pfile_SetAssetSourceMode(0);
            App_BlankScreenForModeSwitch(ctx);
            module.frame = Mini_CondorFrame;
            module.onMouse = (GameModuleMessageCallback)Mini_CondorMouse;
            module.onKey = (GameModuleMessageCallback)Mini_CondorKeyboard;
            module.enter = Mini_CondorEnter;
            module.exit = (GameModuleCallback)Mini_CondorExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_HIGHWAY:
            Pfile_SetAssetSourceMode(0);
            App_BlankScreenForModeSwitch(ctx);
            module.frame = Mini_HighwayFrame;
            module.onMouse = (GameModuleMessageCallback)Mini_HighwayMouse;
            module.onKey = (GameModuleMessageCallback)Mini_HighwayKeyboard;
            module.enter = (GameModuleCallback)Mini_HighwayEnter;
            module.exit = Mini_HighwayExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_JET_COASTER:
            Pfile_SetAssetSourceMode(0);
            App_BlankScreenForModeSwitch(ctx);
            module.frame = Mini_CoasterFrame;
            module.onMouse = (GameModuleMessageCallback)Mini_CoasterMouse;
            module.onKey = (GameModuleMessageCallback)Mini_CoasterKeyboard;
            module.enter = Mini_CoasterEnter;
            module.exit = Mini_CoasterExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_SUBMARINE:
            Pfile_SetAssetSourceMode(1);
            App_BlankScreenForModeSwitch(ctx);
            module.frame = Submarine_ModuleFrame;
            module.onMouse = 0;
            module.onKey = 0;
            module.enter = Submarine_ModuleEnter;
            module.exit = (GameModuleCallback)Submarine_ModuleExit;
            RequestModuleSwitch(&module, ctx);
            break;
        case GAME_MODE_SNOWBOARD:
        case GAME_MODE_SNOWBOARD_2:
            App_BlankScreenForModeSwitch(ctx);
            module.frame = Mini_SnowboardFrame;
            module.onMouse = (GameModuleMessageCallback)Mini_SnowboardMouse;
            module.onKey = (GameModuleMessageCallback)Mini_SnowboardKeyboard;
            module.enter = Mini_SnowboardEnter;
            module.exit = Mini_SnowboardExit;
            RequestModuleSwitch(&module, ctx);
            break;
        }
    }
}

/*
 * Logs WM_ACTIVATEAPP; on activation calls Trace_Printf and tries to restore the movie surface,
 * logging restoration failure, while deactivation calls Trace_Printf and stops the movie only when
 * g_Movie_isPlaying is nonzero.
 */
// FUNCTION: FF7 0x00409cf2
void __cdecl App_OnActivateApp(int active)
{
    int activeCopy;
    int stopped;
    activeCopy = 0;
    stopped = 0;
    OutputDebugStringA(
        // STRING: FF7 0x007bad78
        "WM_ACTIVATEAPP \n");
    activeCopy = active;
    if (activeCopy != 0) {
        Trace_Printf(
            // STRING: FF7 0x007bad8c
            "Activating...\n");
        if (Movie_RestoreSurface() == 0) {
            OutputDebugStringA(
                // STRING: FF7 0x007bad9c
                "Could not restore movie surface \n");
        }
    } else {
        Trace_Printf(
            // STRING: FF7 0x007badc0
            "Deactivating...\n");
        if (g_Movie_isPlaying != 0) {
            stopped = 1;
            Movie_Stop();
        }
    }
}

/*
 * The engine context's device-change callback (onDeviceChange), which GameWindowProc calls on
 * WM_DEVICECHANGE, for example when the CD is taken out of the drive. It passes the event and its
 * device data to CdCheck_HandleDeviceChange, which on removal (0x8004) asks for the disc the game
 * needs and runs cdcheck.exe; the middle argument is not used.
 */
// FUNCTION: FF7 0x00409d66
void __cdecl App_OnDeviceChangeMessage(int event, int unused, int deviceData)
{
    CdCheck_HandleDeviceChange(event, deviceData);
}

/*
 * Returns zero for modes 23, 24, 25, 27, 28, or 10; for mode 1 returns whether Movie_IsNotPlaying
 * is nonzero; returns one for all other modes.
 */
// FUNCTION: FF7 0x00409d7b
int __cdecl App_AllowCtrlQQuit(void)
{
    if (g_EngineGameMode == GAME_MODE_BATTLE_SWIRL || g_EngineGameMode == GAME_MODE_UNUSED_18 || g_EngineGameMode == GAME_MODE_ENDING_MOVIE ||
        g_EngineGameMode == GAME_MODE_CREDITS || g_EngineGameMode == GAME_MODE_ENDING_CREDITS || g_EngineGameMode == GAME_MODE_SUBMARINE) {
        return 0;
    }
    if (g_EngineGameMode == GAME_MODE_FIELD) {
        if (Movie_IsNotPlaying() == 0) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 1;
    }
}

/*
 * Stores the unsigned low and high 16-bit halves of lParam in the engine context's cursorX and
 * cursorY; the `unused` parameter is ignored and msgCode only copied to a local.
 */
// FUNCTION: FF7 0x00409df1
void __cdecl App_OnMouseMessage(int msgCode, int unused, int lParam, GfxEngineContext *ctx)
{
    int mouseX;
    int mouseY;
    int savedArg;

    mouseX = lParam & 0xFFFF;
    mouseY = (unsigned int)lParam >> 0x10 & 0xFFFF & 0xFFFF;
    ctx->cursorX = mouseX;
    ctx->cursorY = mouseY;
    savedArg = msgCode;
}

/*
 * Handles key-down/up and character messages using the input-state block (g_Main_AppContext). Key-down
 * sets ctrlDown/shiftDown; F1 toggles f1DebugToggle when Shift was already down, otherwise reports the
 * F1 KEYPRESS event.
 * Enter does nothing. Key-up clears Control or Shift. Character q, after narrowing key to char,
 * sets mode nineteen only while Control is set; other messages/keys do nothing. unused is not
 * read.
 */
// FUNCTION: FF7 0x00409e39
void __cdecl App_OnKeyMessage(int message, int key, int unused, GfxEngineContext *ctx)
{
    // Three named locals followed by five compiler-created switch slots.
    int downKey;
    int upKey;
    char charCode;
    switch (message) {
    case WM_KEYDOWN:
        downKey = key;
        if (g_Main_AppContext.inputState->shiftDown != 0) {
            switch (downKey) {
            case VK_CONTROL:
                g_Main_AppContext.inputState->ctrlDown = 1;
                break;
            case VK_SHIFT:
                g_Main_AppContext.inputState->shiftDown = 1;
                break;
            case VK_RETURN:
                break;
            case VK_F1:
                g_Main_AppContext.inputState->f1DebugToggle = g_Main_AppContext.inputState->f1DebugToggle ^ 1;
                break;
            }
        } else {
            switch (downKey) {
            case VK_CONTROL:
                g_Main_AppContext.inputState->ctrlDown = 1;
                break;
            case VK_SHIFT:
                g_Main_AppContext.inputState->shiftDown = 1;
                break;
            case VK_RETURN:
                break;
            case VK_F1:
                ReportEngineError(ctx,
                                  // STRING: FF7 0x007bade0
                                  "F1",
                                  // STRING: FF7 0x007badd4
                                  "KEYPRESS", 0);
                break;
            }
        }
        break;
    case WM_KEYUP:
        upKey = key;
        switch (upKey) {
        case VK_CONTROL:
            g_Main_AppContext.inputState->ctrlDown = 0;
            break;
        case VK_SHIFT:
            g_Main_AppContext.inputState->shiftDown = 0;
            break;
        }
        break;
    case WM_CHAR:
        charCode = key;
        switch (charCode) {
        case 'q':
            if (g_Main_AppContext.inputState->ctrlDown != 0) {
                g_EngineGameMode = GAME_MODE_QUIT;
            }
            break;
        }
        break;
    }
}

/*
 * Enables heap tracking and initializes paths, checks available page-file bytes and marks mode
 * nineteen after displaying insufficient-swap error and freeing paths, but continues. Checks three
 * registry keys, requesting configuration for open failures, zero-size successful GUID queries or
 * MIDI ID minus one; query failures alone do not clear readiness. If needed launches
 * ff7config.exe, waits and checks exit code without checking API success or closing handles.
 * Missing game disc again frees paths and marks failure but continues. Creates an engine context,
 * applies configuration and installs lifecycle/input/device callbacks when nonnull; both arguments
 * are unused.
 */
// FUNCTION: FF7 0x0040a091
void __cdecl App_Bootstrap(int first, int second)
{
    // The names and declaration scopes preserve VC5's 0x1a0-byte frame.
    int ready;
    GfxEngineContext *engineCtxPtr;
    unsigned long dataBytes;
    char valBuf[16];
    MEMORYSTATUS memoryState;
    unsigned long regType;
    int deviceId;
    HKEY regKey;
    char configExe[256];
    ready = 1;
    SetGameHeapTrackingEnabled(g_Main_HeapTrackingEnabled);
    InitPathsFromRegistry();
    GlobalMemoryStatus(&memoryState);
    if (memoryState.dwAvailPageFile < 50000000) {
        MessageBoxA(0,
                    // STRING: FF7 0x007badf4
                    "There is insufficient swap space to play FF7.",
                    // STRING: FF7 0x007bade4
                    "No Swap Space", MB_ICONERROR);
        Engine_FreePathGlobals();
        g_EngineGameMode = GAME_MODE_QUIT;
    }
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_GRAPHICS_KEY, 0, KEY_QUERY_VALUE, &regKey) == 0) {
        dataBytes = 16;
        if (RegQueryValueExA(regKey,
                             REG_DD_GUID, 0, &regType, (unsigned char *)valBuf, &dataBytes) == 0 &&
            dataBytes == 0) {
            ready = 0;
        }
        RegCloseKey(regKey);
    } else {
        ready = 0;
    }
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_MIDI_KEY, 0, KEY_QUERY_VALUE, &regKey) == 0) {
        dataBytes = 4;
        if (RegQueryValueExA(regKey,
                             REG_MIDI_DEVICEID, 0, &regType, (unsigned char *)&deviceId, &dataBytes) == 0 &&
            deviceId == -1) {
            ready = 0;
        }
        RegCloseKey(regKey);
    } else {
        ready = 0;
    }
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      REG_SOUND_KEY, 0, KEY_QUERY_VALUE, &regKey) == 0) {
        dataBytes = 16;
        if (RegQueryValueExA(regKey,
                             REG_SOUND_GUID, 0, &regType, (unsigned char *)valBuf, &dataBytes) == 0 &&
            dataBytes == 0) {
            ready = 0;
        }
        RegCloseKey(regKey);
    } else {
        ready = 0;
    }
    if (ready == 0) {
        STARTUPINFOA procStartup = {0};
        PROCESS_INFORMATION procInfo = {0};
        unsigned long exitCode;
        procStartup.cb = 68;
        procStartup.lpReserved = 0;
        procStartup.lpReserved2 = 0;
        procStartup.cbReserved2 = 0;
        procStartup.lpDesktop = 0;
        procStartup.dwFlags = 0;
        strcpy(configExe, g_InitPath_AppPath);
        strcat(configExe,
               // STRING: FF7 0x007bae24
               "ff7config.exe");
        CreateProcessA(0, configExe, 0, 0, 1, 0x20, 0, 0, &procStartup, &procInfo);
        WaitForSingleObject(procInfo.hProcess, -1);
        GetExitCodeProcess(procInfo.hProcess, &exitCode);
        if (exitCode != 0) {
            g_EngineGameMode = GAME_MODE_QUIT;
        }
    }
    if (CdCheck_GetGameDiscNumber() == 0) {
        MessageBoxA(0,
                    // STRING: FF7 0x007bae48
                    "Please insert FF7 Game Disc 1, 2, or 3 and try again.",
                    // STRING: FF7 0x007bae34
                    "Insert Game Disc", MB_ICONERROR);
        Engine_FreePathGlobals();
        g_EngineGameMode = GAME_MODE_QUIT;
    }
    engineCtxPtr = (GfxEngineContext *)CreateEngineContext();
    if (engineCtxPtr != 0) {
        engineCtxPtr->fov = 90.0f;
        engineCtxPtr->nearPlane = 125.0f;
        engineCtxPtr->farPlane = 50000.0f;
        ApplyConfigToContext(engineCtxPtr);
        engineCtxPtr->activeModule.init = App_InitSubsystems;
        engineCtxPtr->activeModule.frame = GameModeDispatcher_Frame;
        engineCtxPtr->activeModule.shutdown = App_Shutdown;
        engineCtxPtr->activeModule.enter = GameModeDispatcher_Enter;
        engineCtxPtr->activeModule.exit = GameModeDispatcher_Exit;
        /* App_OnActivateApp and App_AllowCtrlQQuit ignore the trailing engine-context argument */
        engineCtxPtr->onActivateApp = (void (__cdecl *)(int, int))App_OnActivateApp;
        engineCtxPtr->onDeviceChange = App_OnDeviceChangeMessage;
        engineCtxPtr->quitConfirmRun = (GfxModeCallback)Menu_QuitConfirmDialogRun;
        engineCtxPtr->allowCtrlQQuit = (int (__cdecl *)(int))App_AllowCtrlQQuit;
        engineCtxPtr->activeModule.onMouse = App_OnMouseMessage;
        engineCtxPtr->activeModule.onKey = App_OnKeyMessage;
        SetEngineContext((int)engineCtxPtr);
    }
}
} /* extern "C" */

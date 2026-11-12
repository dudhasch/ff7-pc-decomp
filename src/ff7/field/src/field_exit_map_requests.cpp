// src/ff7/field/src/field_exit_map_requests.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Holds the field exit request and map preload state, starts map scripts, and runs field
//   logic that dispatches mode transitions. It also has a debug helper that looks up the on_line
//   save file.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/codegen.h"
#include "ff7/enums/main.h"
#include "ff7/enums/field.h"
#include "lib/types/graphics.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/movie.h"
#include "ff7/api/field.h"
#include "lib/api/sound.h"
#include <string.h>
#include "ff7/field.h"
#include "ff7/api/menu.h"
#include "ff7/api/main.h"
#include "ff7/api/movie.h"
#include "ff7/savemap.h"
#include "lib/enums/sound.h"
#include "lib/enums/file.h"
extern "C" {

// GLOBAL: FF7 0x00cffb8c
int g_FieldExitRequest = 0;
// clang-format on
#pragma intrinsic(strcpy)

/*
 * Stores exitRequest in g_FieldExitRequest.
 */
// FUNCTION: FF7 0x0063bc90
void __cdecl Field_RequestExit(int exitRequest)
{
    g_FieldExitRequest = exitRequest;
}

/*
 * Returns g_FieldExitRequest.
 */
// FUNCTION: FF7 0x0063bc9d
int __cdecl Field_GetExitRequest(void)
{
    return g_FieldExitRequest;
}

/*
 * Calls Script_BeginMap with g_FieldEntityState.
 */
// FUNCTION: FF7 0x0063bca7
void __cdecl Field_BeginMapScripts(void)
{
    Script_BeginMap(g_FieldEntityState);
}

/*
 * Clears g_FieldEntryMapId and g_FieldPreloadState.
 */
// FUNCTION: FF7 0x0063bcb9
void __cdecl Field_ClearCurrentMapIdAndPreloadState(void)
{
    g_FieldEntryMapId = 0;
    g_FieldPreloadState = 0;
}

/*
 * Copies g_FieldEntryMapId to g_FieldLayerScrollParam7.
 */
// FUNCTION: FF7 0x0063bcd0
void __cdecl Field_CopyEntryMapIdWord(void)
{
    g_FieldLayerScrollParam7 = g_FieldEntryMapId;
}

/*
 * Only for field identifier 0x1F6 clears nine dword flags and maps each nonempty party character
 * through character-entity and entity-model byte tables. For a nonempty model at most eight sets
 * its corresponding flag one.
 */
// FUNCTION: FF7 0x0063bce1
void __cdecl Field_SetPartyEntityDrawFlagsOnMap1F6(void)
{
    short character;
    short modelIdx;
    short entityIndex;
    int partySlot;

    if (g_FieldCurrentMapId == 0x1F6) {
        for (partySlot = 0; partySlot <= 8; partySlot++) {
            g_FieldModelVisible[partySlot] = 0;
        }
        for (partySlot = 0; partySlot < 3; partySlot++) {
            character = gSavemap.fieldParty[partySlot];
            if (character != PARTY_CHAR_NONE) {
                entityIndex = g_FieldCharEntity.charEntity[character];
                if (entityIndex != 0xFF) {
                    modelIdx = g_FieldEntityModel[entityIndex];
                    if (modelIdx != 0xFF && modelIdx <= 8) {
                        g_FieldModelVisible[modelIdx] = 1;
                    }
                }
            }
        }
    }
}

/*
 * Resets field map state. Clears common globals, and transition mode 2 posts request 0x18 with the
 * stored or alternate music identifier, performs transition cleanup, and dispatches the sound
 * command. Modes other than 13 initialize a 16-step fade-out state. Listed transition modes
 * 0/1/3/6/7/8/9/10/11 or missing field/entity state take the full reset path: restore camera/scroll
 * defaults, initialize rendering/input/movie state, copy trigger settings, place the player, clear
 * 16 mappings to 0xFF, and initialize door triggers. Other modes retain more state, reset windows,
 * and refresh walkmesh pointers. All paths reset line talk flags and final globals.
 */
// FUNCTION: FF7 0x0063bda8
void __cdecl Field_ResetMapState(void)
{
    int triangleCount;
    int idx;

    Field_CopyEntryMapIdWord();
    g_FieldLogicStateFlag = 0;
    g_FieldMovieCameraActive = 0;
    g_FieldLogicActive = 0;
    g_FieldPlayerCrossedLine = 0;
    g_FieldTransitionPendingFlag = 0;
    if (g_EnginePreviousMode == GAME_MODE_BATTLE) {
        g_FieldRequestPosted = 1;
        if (g_FieldEncounterTriggered == 1) {
            g_FieldEncounterTriggered = 0;
            g_FieldEncounterStepCounter = 0;
            g_FieldEncounterRateAccum = 0;
            g_FieldActorState.state.requestCode = FIELD_REQUEST_NONE;
        }
        SfxRestoreChannelLoopRecords();
        g_pFieldRequest->code = SOUND_MUSIC_CROSSFADE;
        if (g_FieldActorState.state.nextFieldMusic != 0x157C0) {
            g_pFieldRequest->args[0] = g_FieldActorState.state.nextFieldMusic;
        } else {
            g_pFieldRequest->args[0] = g_FieldMusicIdAlt;
        }
        g_pFieldRequest->args[1] = 4;
        SoundDispatchCommand(g_pFieldRequest->code, g_pFieldRequest->args[0], g_pFieldRequest->args[1], 0, 0, 0, 0, 0, 0);
    }
    if (g_EnginePreviousMode != GAME_MODE_FIELD_SERVICE) {
        g_FieldTransitionPendingFlag = 1;
        g_FieldFxStepCounter = 0;
        g_FieldActorState.state.fadeType = FIELD_FADE_SCALED_COMPLEMENT_OUT;
        g_FieldActorState.state.fadeSpeed = 0x10;
        g_FieldActorState.state.fadeAdjust = 0x100;
        g_FieldActorState.state.red = 0;
        g_FieldActorState.state.green = 0;
        g_FieldActorState.state.blue = 0;
    }
    if (g_EnginePreviousMode == GAME_MODE_NONE || (g_EnginePreviousMode == GAME_MODE_FIELD || (g_EnginePreviousMode == GAME_MODE_WORLD_MAP || (g_EnginePreviousMode == GAME_MODE_HIGHWAY || (g_EnginePreviousMode == GAME_MODE_SNOWBOARD || (g_EnginePreviousMode == GAME_MODE_CHOCOBO_RACE || (g_EnginePreviousMode == GAME_MODE_FORT_CONDOR || (g_EnginePreviousMode == GAME_MODE_JET_COASTER || (g_EnginePreviousMode == GAME_MODE_SUBMARINE || (g_pFieldState == 0 || g_pFieldEntities == 0)))))))))) {
        g_FieldBgFlag = 0;
        g_FieldLayerScrollX = 0;
        g_FieldLayerScrollY = 0;
        g_FieldBgLayer1ScrollX = 0xA0;
        g_FieldBgLayer1ScrollY = 0x78;
        SfxClearChannelLoopRecords();
        g_FieldActorState.state.layer2ScrollXSpeed = 0;
        g_FieldActorState.state.layer2ScrollYSpeed = 0;
        g_FieldActorState.state.layer3ScrollXSpeed = 0;
        g_FieldActorState.state.layer3ScrollYSpeed = 0;
        g_FieldActorState.state.oldActiveKeys32 = 0;
        g_FieldActorState.state.activeKeys32 = 0;
        g_FieldActorState.state.newActiveKeys32 = 0;
        g_FieldActorState.state.newInactiveKeys32 = 0;
        g_FieldActorState.state.oldActiveKeys2_32 = 0;
        g_FieldActorState.state.activeKeys2_32 = 0;
        g_FieldActorState.state.newActiveKeys2_32 = 0;
        g_FieldActorState.state.newInactiveKeys2_32 = 0;
        g_FieldActorState.state.layer3Depth = 1;
        g_FieldActorState.state.layer2Depth = 0xFFF;
        g_FieldActorState.state.viewOffset = g_pFieldScrollBounds->viewOffset;
        g_EngineGameMode = GAME_MODE_FIELD;
        Field_BeginMapScripts();
        Field_ResetScrollAccelerationCounters();
        Field_ClearPalettePendingFlag();
        Field_MapResetHookNoOp();
        g_FieldMoviePending = 0;
        g_FieldAudioDisabled = 0;
        g_FieldMoviePlaying = 0;
        Input_ClearButtonLatches();
        g_FieldPlayerEntityIndex = g_FieldActorState.state.playerModel;
        g_FieldEntityState[g_FieldPlayerEntityIndex].dir = g_FieldActorState.state.pcDirection;
        if ((gSavemap.fieldLineDrawFlags & 0x80) == 0) {
            g_FieldLineDrawIntensity = 0;
        } else {
            g_FieldLineDrawIntensity = 0xFF;
        }
        g_FieldEncounterStepCounter = 0;
        Field_PlacePlayerOnWalkmesh();
        for (idx = 0; idx < 0x10; idx++) {
            g_FieldScriptExecFlags[idx] = 0xFF;
        }
        Field_InitDoorTriggers((int)g_pFieldScrollBounds + 0x158);
    } else {
        Field_SetMessageTableFromScriptHeader();
        Input_ClearButtonLatches();
        g_EngineGameMode = GAME_MODE_FIELD;
        triangleCount = *(unsigned short *)g_pFieldWalkmesh;
        g_pFieldWalkmeshVertices = (FieldWalkmeshVertex *)((int)g_pFieldWalkmesh + 4);
        g_pFieldWalkmeshAccessTable = (short *)((int)g_pFieldWalkmesh + triangleCount * 0x18 + 4);
        g_FieldActorState.state.requestArg = 2;
    }
    Field_SetPartyEntityDrawFlagsOnMap1F6();
    Field_ResetScriptLineTalkFlags((int)&g_FieldLines[0].pos);
    g_FieldEncounterState = 0;
    g_FieldEventCompleteMessage[0] = 0;
    g_FieldLogicExitState = FIELD_EXIT_IDLE;
}

/*
 * Runs field logic and dispatches requested mode transitions. On return from service mode 0xD it
 * captures the entry point, performs the selected materia/party service and restores field state;
 * then services sound fades, movie requests, input, scripts and camera helpers. Request codes
 * choose menu, minigame, world or exit states, with early returns for codes 0x1A and 5; code 2
 * captures player position/triangle. Entity movement runs only in normal field state, followed by
 * line triggers, arrows and effects. Menu and field-change requests coordinate the fade state: a
 * destination field in 1 through 64 clears entities and graphics, performs transition cleanup and
 * returns 0; an out-of-range destination returns 1. Other paths return 0. Player indices are
 * unchecked, unsupported minigame selectors leave the mode unchanged, and graphics-color
 * submission relies on scalar-local frame storage.
 */
// FUNCTION: FF7 0x0063c17f
int __cdecl Field_UpdateLogic(void)
{
    GfxEngineContext *ctx;
    GfxClearColorBgra color;
    int inputFlags;

    ctx = GetEngineContext();
    Menu_SetMessageWindowBlackQuadEnabled(0);
    if (g_EngineGameMode == GAME_MODE_FIELD_SERVICE) {
        Savemap_CaptureFieldEntryPoint();
        switch (g_FieldActorState.state.requestCode) {
        case FIELD_REQUEST_HARVEST_MATERIA:
            Script_SvcHarvestMateria();
            break;
        case FIELD_REQUEST_RETURN_MATERIA:
            Script_SvcReturnMateria();
            break;
        case FIELD_REQUEST_STRIP_CHARACTER:
            Script_SvcStripCharacter(g_FieldActorState.state.eventCmdParam);
            break;
        case FIELD_REQUEST_SCALE_FIRST_FIVE_HP:
            Script_SvcScaleFirstFiveHp();
            break;
        case FIELD_REQUEST_CHECK_MATERIA_SET:
            Script_SvcCheckMateriaSet(g_FieldActorState.state.eventCmdParam);
            break;
        case FIELD_REQUEST_REPLACE_MATERIA_GROUP:
            Script_SvcReplaceMateriaGroup(g_FieldActorState.state.eventCmdParam);
            break;
        case FIELD_REQUEST_SNAPSHOT_LEVELS:
            Savemap_SnapshotLevelsToBank();
        }
        if (g_FieldActorState.state.requestCode != FIELD_REQUEST_REENTER_FIELD) {
            Savemap_ReconcileFieldParty();
        }
        g_FieldActorState.state.requestArg = 2;
        g_EnginePreviousMode = GAME_MODE_FIELD_SERVICE;
        g_EngineGameMode = GAME_MODE_FIELD;
    }
    SoundUpdateFades();
    Field_RequestExit(0);
    g_FieldWorldMapResult = (int)g_FieldWorldMapResult & 1;
    Field_ServiceMovieRequest();
    g_FieldActorState.state.windowPhase = *(char *)&g_FieldWorldMapResult;
    Field_UpdateMovieCamera();
    inputFlags = Field_ReadInput(&g_FieldLayerScrollX, &g_FieldLayerScrollY);
    g_FieldActorState.state.currentMovieFrame = Movie_GetCurrentFrame();
    if (g_FieldCurrentMapId == 0x74 && g_FieldActorState.state.currentMovieFrame == 0x6E0) {
        MidiPlaySong(2, 0, 0);
        g_pFieldState->nextFieldMusic = 2;
        g_FieldMusicIdAlt = 2;
    }
    if (g_EngineGameMode != GAME_MODE_MENU) {
        Script_UpdateFrame();
    }
    g_FieldPlayerEntityIndex = g_FieldActorState.state.playerModel;
    Field_BeginCameraTransitionFromRequest();
    Field_StepCameraScrollTransition();
    Field_UpdateScreenShakeAxis((FieldScreenShake *)&g_FieldActorState.state.shakeX);
    Field_UpdateScreenShakeAxis((FieldScreenShake *)&g_FieldActorState.state.shakeY);
    Field_UpdateCameraScroll();
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_DISC_CHANGE) {
        g_EngineGameMode = GAME_MODE_DISC_CHECK;
        Field_RequestExit(2);
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_REENTER_FIELD) {
        g_EnginePreviousMode = GAME_MODE_FIELD;
        g_EngineGameMode = GAME_MODE_REENTER_FIELD;
        Field_RequestExit(2);
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_HARVEST_MATERIA || (g_FieldActorState.state.requestCode == FIELD_REQUEST_RETURN_MATERIA || (g_FieldActorState.state.requestCode == FIELD_REQUEST_STRIP_CHARACTER || (g_FieldActorState.state.requestCode == FIELD_REQUEST_SCALE_FIRST_FIVE_HP || (g_FieldActorState.state.requestCode == FIELD_REQUEST_CHECK_MATERIA_SET || (g_FieldActorState.state.requestCode == FIELD_REQUEST_REPLACE_MATERIA_GROUP || g_FieldActorState.state.requestCode == FIELD_REQUEST_SNAPSHOT_LEVELS)))))) {
        g_EnginePreviousMode = GAME_MODE_FIELD;
        g_EngineGameMode = GAME_MODE_FIELD_SERVICE;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_NAME_ENTRY || (g_FieldActorState.state.requestCode == FIELD_REQUEST_PARTY_SELECT || (g_FieldActorState.state.requestCode == FIELD_REQUEST_MAIN_MENU || (g_FieldActorState.state.requestCode == FIELD_REQUEST_SAVE_MENU || (g_FieldActorState.state.requestCode == FIELD_REQUEST_SHOP || (g_FieldActorState.state.requestCode == FIELD_REQUEST_BACKUP_PARTY_EQUIP || g_FieldActorState.state.requestCode == FIELD_REQUEST_RESTORE_PARTY_EQUIP)))))) {
        g_EnginePreviousMode = GAME_MODE_FIELD;
        g_EngineGameMode = GAME_MODE_MENU;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_GAME_OVER) {
        g_EnginePreviousMode = GAME_MODE_FIELD;
        g_EngineGameMode = GAME_MODE_QUIT;
        Field_RequestExit(2);
        return 0;
    } else if ((inputFlags & INPUT_TRIANGLE) != 0 && *(char *)&g_FieldActorState.state.menuLocked == 0 && g_FieldLogicActive == 0) {
        g_EnginePreviousMode = GAME_MODE_FIELD;
        g_EngineGameMode = GAME_MODE_MENU;
        g_FieldActorState.state.requestCode = FIELD_REQUEST_MAIN_MENU;
        g_FieldActorState.state.eventCmdParam = 0;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_ENDING_MOVIE) {
        g_EnginePreviousMode = GAME_MODE_FIELD;
        g_EngineGameMode = GAME_MODE_ENDING_MOVIE;
        Field_RequestExit(2);
        return 0;
    } else if (g_FieldActorState.state.requestCode == FIELD_REQUEST_BATTLE) {
        g_FieldActorState.state.pcTileX = g_FieldEntityState[g_FieldPlayerEntityIndex].posX / 4096;
        g_FieldActorState.state.pcTileY = g_FieldEntityState[g_FieldPlayerEntityIndex].posY / 4096;
        g_FieldActorState.state.pcWalkMeshId = g_FieldEntityState[g_FieldPlayerEntityIndex].posI;
        g_EnginePreviousMode = GAME_MODE_FIELD;
        g_EngineGameMode = GAME_MODE_BATTLE_SWIRL;
        Field_RequestExit(2);
        ctx->presentEnable = 0;
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_MINIGAME) {
        g_FieldActorState.state.mapId = g_FieldEntryMapId;
        g_FieldEntryMapId = g_FieldActorState.state.eventCmdParam;
        g_EnginePreviousMode = GAME_MODE_FIELD;
        switch (g_FieldActorState.state.minigameKind) {
        case FIELD_MINIGAME_HIGHWAY:
            g_EngineGameMode = GAME_MODE_HIGHWAY;
            // STRING: FF7 0x00906b88
            strcpy((char *)g_FieldEventCompleteMessage, UNFOLDED("Event completes -- Highway"));
            break;
        case FIELD_MINIGAME_CHOCOBO_RACE:
            g_EngineGameMode = GAME_MODE_CHOCOBO_RACE;
            // STRING: FF7 0x00906ba4
            strcpy((char *)g_FieldEventCompleteMessage, UNFOLDED("Event completes -- Chocobo"));
            break;
        case FIELD_MINIGAME_SNOWBOARD:
            g_EngineGameMode = GAME_MODE_SNOWBOARD;
            // STRING: FF7 0x00906bc0
            strcpy((char *)g_FieldEventCompleteMessage, UNFOLDED("Event completes -- Snow Board"));
            break;
        case FIELD_MINIGAME_FORT_CONDOR:
            g_EngineGameMode = GAME_MODE_FORT_CONDOR;
            // STRING: FF7 0x00906be0
            strcpy((char *)g_FieldEventCompleteMessage, UNFOLDED("Event completes -- Condor War"));
            break;
        case FIELD_MINIGAME_SUBMARINE:
            g_EngineGameMode = GAME_MODE_SUBMARINE;
            // STRING: FF7 0x00906c00
            strcpy((char *)g_FieldEventCompleteMessage, UNFOLDED("Event completes -- Sub Marine"));
            break;
        case FIELD_MINIGAME_JET_COASTER:
            g_EngineGameMode = GAME_MODE_JET_COASTER;
            // STRING: FF7 0x00906c20
            strcpy((char *)g_FieldEventCompleteMessage, UNFOLDED("Event completes -- Jet"));
            break;
        case FIELD_MINIGAME_SNOWBOARD_2:
            g_EngineGameMode = GAME_MODE_SNOWBOARD_2;
            // STRING: FF7 0x00906c38
            strcpy((char *)g_FieldEventCompleteMessage, UNFOLDED("Event completes -- Snow Board 2"));
        }
        Field_RequestExit(2);
    }
    if (g_EngineGameMode == GAME_MODE_FIELD && g_FieldActorState.state.requestCode != FIELD_REQUEST_MAP_JUMP) {
        Field_UpdateEntities(inputFlags);
    }
    Field_TestScriptLineTriggers((FieldScriptLineTrigger *)&g_FieldEntityState[g_FieldPlayerEntityIndex].kawaiOp1, (int)&g_FieldLines[0].pos);
    Field_FlagNearestFacedEntityOnInputEdge();
    Field_StepLineDrawRecords();
    Field_DrawScriptLines(&g_FieldCameraTransform);
    Field_DrawGatewayArrows(&g_FieldCameraTransform, (struct FieldGatewayBox *)((int)g_pFieldScrollBounds + 0x38));
    Field_UpdateFieldFx();
    if (g_EngineGameMode == GAME_MODE_MENU) {
        if (g_FieldActorState.state.fadeType != FIELD_FADE_NONE && g_FieldLogicExitState == FIELD_EXIT_IDLE) {
            Field_RequestExit(1);
        } else {
            Menu_SetMessageWindowBlackQuadEnabled(1);
            if (g_FieldActorState.state.fadeType == FIELD_FADE_NONE && g_FieldLogicExitState == FIELD_EXIT_IDLE) {
                g_FieldActorState.state.fadeType = FIELD_FADE_BLACK_RAMP_SHORT;
                g_FieldModelIndex = 0xD;
                g_FieldActorState.state.fadeAdjust = 0;
                g_MenuEntryState = 1;
                g_FieldLogicExitState = FIELD_EXIT_FADING;
            } else if (g_FieldActorState.state.fadeType == FIELD_FADE_NONE) {
                Field_RequestExit(1);
            }
        }
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_MAP_JUMP) {
        Menu_SetMessageWindowBlackQuadEnabled(1);
        if (g_FieldLogicExitState == FIELD_EXIT_FADE_DONE) {
            g_FieldLogicExitState = FIELD_EXIT_IDLE;
            g_EnginePreviousMode = GAME_MODE_FIELD;
            g_FieldActorState.state.mapId = g_FieldEntryMapId;
            g_FieldEntryMapId = g_FieldActorState.state.eventCmdParam;
            if (g_FieldEntryMapId > 0 && g_FieldEntryMapId < 0x41) {
                g_EngineGameMode = GAME_MODE_WORLD_MAP;
                Field_RequestExit(2);
                g_pFieldEntities = 0;
                color.r = 0;
                color.g = 0;
                color.b = 0;
                color.a = FLOAT_ONE_BITS;
                GfxSetBackgroundColorCall(&color, ctx);
                GfxClearAllCall(ctx);
                Field_PresentEngineFrame();
                ctx->presentEnable = 0;
                GfxClearAllCall(ctx);
                return 0;
            }
            return 1;
        }
        if (g_EnginePreviousMode == GAME_MODE_FIELD_SERVICE) {
            g_EnginePreviousMode = GAME_MODE_FIELD;
            g_FieldActorState.state.mapId = g_FieldEntryMapId;
            g_FieldEntryMapId = g_FieldActorState.state.eventCmdParam;
            if (g_FieldEntryMapId > 0 && g_FieldEntryMapId < 0x41) {
                g_EngineGameMode = GAME_MODE_WORLD_MAP;
                Field_RequestExit(2);
                g_pFieldEntities = 0;
                color.r = 0;
                color.g = 0;
                color.b = 0;
                color.a = FLOAT_ONE_BITS;
                GfxSetBackgroundColorCall(&color, ctx);
                GfxClearAllCall(ctx);
                Field_PresentEngineFrame();
                ctx->presentEnable = 0;
                GfxClearAllCall(ctx);
                return 0;
            }
            return 1;
        }
        if (g_FieldActorState.state.fadeType != FIELD_FADE_BLACK_RAMP_LONG && g_FieldActorState.state.fadeType != FIELD_FADE_NONE) {
            g_EnginePreviousMode = GAME_MODE_FIELD;
            g_FieldActorState.state.mapId = g_FieldEntryMapId;
            g_FieldEntryMapId = g_FieldActorState.state.eventCmdParam;
            if (g_FieldEntryMapId > 0 && g_FieldEntryMapId < 0x41) {
                g_EngineGameMode = GAME_MODE_WORLD_MAP;
                Field_RequestExit(2);
                g_pFieldEntities = 0;
                color.r = 0;
                color.g = 0;
                color.b = 0;
                color.a = FLOAT_ONE_BITS;
                GfxSetBackgroundColorCall(&color, ctx);
                GfxClearAllCall(ctx);
                Field_PresentEngineFrame();
                ctx->presentEnable = 0;
                GfxClearAllCall(ctx);
                return 0;
            }
            return 1;
        }
        if (g_FieldActorState.state.fadeType == FIELD_FADE_NONE && g_FieldLogicExitState == FIELD_EXIT_IDLE) {
            g_FieldActorState.state.fadeType = FIELD_FADE_BLACK_RAMP_LONG;
            g_FieldModelIndex = 3;
            g_FieldActorState.state.fadeAdjust = 0;
            g_MenuEntryState = 1;
            g_FieldLogicExitState = FIELD_EXIT_FADING;
        } else {
            if (g_FieldActorState.state.fadeType == FIELD_FADE_NONE) {
                g_EnginePreviousMode = GAME_MODE_FIELD;
                g_FieldActorState.state.mapId = g_FieldEntryMapId;
                g_FieldEntryMapId = g_FieldActorState.state.eventCmdParam;
                if (g_FieldEntryMapId > 0 && g_FieldEntryMapId < 0x41) {
                    g_EngineGameMode = GAME_MODE_WORLD_MAP;
                    Field_RequestExit(2);
                    g_pFieldEntities = 0;
                    color.r = 0;
                    color.g = 0;
                    color.b = 0;
                    color.a = FLOAT_ONE_BITS;
                    GfxSetBackgroundColorCall(&color, ctx);
                    GfxClearAllCall(ctx);
                    Field_PresentEngineFrame();
                    ctx->presentEnable = 0;
                    GfxClearAllCall(ctx);
                    return 0;
                }
                return 1;
            }
        }
    }
    return 0;
}

/*
 * Looks up on_line.sav in LGP archive one and returns if absent. Reads the reported entry size
 * into a local SaveMapCopy, copies it to gSavemap, then restores selected field state
 * bytes/shorts, forces g_FieldEntryMapId to 502 and publishes the saved encounter random index/offset.
 */
// FUNCTION: FF7 0x0063cc4d
void __cdecl Field_DebugLoadOnLineSav(void)
{
    int size;
    int r;
    void *buf;
    SaveMapCopy saved;
    void *lgpFile;
    // STRING: FF7 0x00906c58
    lgpFile = FindLgpFile("on_line.sav", LGP_ARCHIVE_FIELD_LEVEL);
    if (lgpFile != 0) {
        size = GetLgpEntrySize((int)lgpFile, LGP_ARCHIVE_FIELD_LEVEL);
        buf = &saved;
        ReadLgpEntry(lgpFile, LGP_ARCHIVE_FIELD_LEVEL, buf, size);
        (*(SaveMapCopy *)&gSavemap) = saved;
    } else {
        return;
    }
    g_FieldReturnMode = gSavemap.fieldReturnMode;
    g_EngineGameMode = GAME_MODE_FIELD;
    g_FieldEntryMapId = 0x1F6;
    g_FieldActorState.state.mapId = gSavemap.locationId;
    g_FieldActorState.state.pcTileX = gSavemap.coordX;
    g_FieldActorState.state.pcTileY = gSavemap.coordY;
    g_FieldActorState.state.pcWalkMeshId = gSavemap.coordT;
    g_FieldActorState.state.entryId = (unsigned char)gSavemap.coordD;
    g_FieldRandomIndex1 = gSavemap.fieldRandomIndex1;
    g_FieldRandomIndex2 = gSavemap.fieldRandomIndex2;
}
} /* extern "C" */

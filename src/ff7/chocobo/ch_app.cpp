// src/ff7/chocobo/ch_app.cpp
// Original: C:\FF7\chocobo\ch_app.cpp
// Contents: Chocobo minigame application module: its engine context, module entry and exit,
//   per-frame step and tick, viewport and camera setup, and the model records it owns. It also
//   holds the frame draw hooks and the module's own mouse and keyboard callbacks, which are
//   empty.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/main.h"
#include "lib/codegen.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "ff7/api/chocobo.h"
#include "lib/api/graphics.h"
#include "ff7/types/chocobo.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "lib/pfile.h"
#include "ff7/gte.h"
extern "C" {

// clang-format off
/* The frame's start time read by ReadTimestampCounter. */
/* The 40-byte sound command block pointed to by g_pChocoboSoundCommand. */
// GLOBAL: FF7 0x00e3ba40
ChocoboRacerEntity g_ChocoboCurrentEntity = {0};
// GLOBAL: FF7 0x00e3ba68
int g_ChocoboUiContext = 0;
// GLOBAL: FF7 0x00e3ba6c
int g_ChocoboUiScalingMode = 0;
// GLOBAL: FF7 0x00e3ba70
int g_ChocoboViewportX = 0;
// GLOBAL: FF7 0x00e3ba74
int g_ChocoboViewportY = 0;
// GLOBAL: FF7 0x00e3ba78
int g_ChocoboViewportWidth = 0;
// GLOBAL: FF7 0x00e3ba7c
int g_ChocoboViewportHeight = 0;
// GLOBAL: FF7 0x00e3ba80
int g_ChocoboUiScaleFactor = 0;
// GLOBAL: FF7 0x00e3ba84
int g_ChocoboViewportOffsetX = 0;
// GLOBAL: FF7 0x00e3ba88
int g_ChocoboViewportOffsetY = 0;
// GLOBAL: FF7 0x00e3ba8c
int g_ChocoboViewportOffsetBase = 0;
// GLOBAL: FF7 0x00e3ba90
int g_ChocoboBetScreenLabelCount = 0;
// GLOBAL: FF7 0x00e3ba94
int g_ChocoboCourseSpriteSize = 0;
/* float bits: 232.0f, the GTE screen distance */
// GLOBAL: FF7 0x00e3ba98
int g_ChocoboScreenReference = 0;
// GLOBAL: FF7 0x00e3ba9c
ChocoboSpriteRecord *g_ChocoboBetScreenLabels = 0;
// GLOBAL: FF7 0x00e3baa0
ChocoboSpriteRecord *g_ChocoboResultScreenLabels = 0;
// GLOBAL: FF7 0x00e3baa4
ChocoboSpriteRecord *g_ChocoboBetScreenPanels = 0;
// GLOBAL: FF7 0x00e3baa8
ChocoboSpriteRecord *g_ChocoboResultScreenPanels = 0;
// GLOBAL: FF7 0x00e3baac
ChocoboSpriteRecord *g_ChocoboBetRacerSprites = 0;
// GLOBAL: FF7 0x00e3bab0
struct ChocoboResultUiEntry *g_ChocoboResultLayout = 0;
// GLOBAL: FF7 0x00e3bab4
ChocoboSpriteRecord *g_ChocoboHudSpriteA = 0;
// GLOBAL: FF7 0x00e3bab8
ChocoboSpriteRecord *g_ChocoboHudSpriteB = 0;
/* Mini_ChocoboDrawNumber2x or ..1x, for the UI scale */
// GLOBAL: FF7 0x00e3babc
void (__cdecl *g_ChocoboDrawNumberFn)(unsigned int number, int x, int y) = 0;
// GLOBAL: FF7 0x00e3bac0
double g_ChocoboFrameDuration = 0.0;
// GLOBAL: FF7 0x00e3bac8
int g_ChocoboFrameLate = 0;
unsigned char g_bssPad_00e3bacc[4] = {0};
/* Set by Mini_ChocoboExit; the frame then switches back to the field. */
// GLOBAL: FF7 0x00e3bad0
int g_ChocoboExitFlag = 0;
/* Set on entry; chocobo_engine_frame_sprites.cpp tests it. */
// GLOBAL: FF7 0x00e3bad4
int g_ChocoboFramePacingOn = 0;
unsigned char g_bssPad_00e3bad8[28] = {0};
// GLOBAL: FF7 0x00e3baf4
int g_ChocoboCameraSmoothedNode = 0;
unsigned char g_bssPad_00e3baf8[218352] = {0};
// GLOBAL: FF7 0x00e70fe8
int g_ChocoboCameraTargetSegment = 0;
// GLOBAL: FF7 0x00e70fec
int g_ChocoboCameraClearedWord = 0;
unsigned char g_bssPad_00e70ff0[16] = {0};
// GLOBAL: FF7 0x00e71000
int g_ChocoboRacerScreenDepth[6] = {0};
// GLOBAL: FF7 0x00e71018
int g_ChocoboFrameCounter = 0;
// GLOBAL: FF7 0x00e7101c
int g_ChocoboRacerStepDivisor = 0;
// GLOBAL: FF7 0x00e71020
int g_ChocoboRacerOnScreen[6] = {0};
// GLOBAL: FF7 0x00e71038
int g_ChocoboSkipPromptActive = 0;
// GLOBAL: FF7 0x00e7103c
int g_ChocoboRouteEventRacer = 0;
// GLOBAL: FF7 0x00e71040
struct ChocoboRaceAssetState g_ChocoboFlags = {0};
unsigned char g_bssPad_00e71048[4] = {0};
// GLOBAL: FF7 0x00e7104c
int g_ChocoboCameraInitIndex = 0;
unsigned char g_bssPad_00e71050[4] = {0};
// GLOBAL: FF7 0x00e71054
int g_ChocoboCameraSnapFlag = 0;
unsigned char g_bssPad_00e71058[16] = {0};
// GLOBAL: FF7 0x00e71068
unsigned char g_ChocoboBetGrid[120] = {0};
// GLOBAL: FF7 0x00e710e0
int g_ChocoboRaceTimer = 0;
// GLOBAL: FF7 0x00e710e4
int g_ChocoboCourseSegmentCount = 0;
// GLOBAL: FF7 0x00e710e8
int g_ChocoboDashActiveFlag = 0;
// GLOBAL: FF7 0x00e710ec
int g_ChocoboFadeLevel = 0;
// Base of the table Mini_ChocoboSubmitRaceHud reads the minimap markers' y from (+0x186B8, 0x18 a
// racer). No instruction in ff7.exe writes it, so the race reads near address 0x186B8 (original bug).
// GLOBAL: FF7 0x00e710f0
char *g_ChocoboMarkerYTable = 0;
unsigned char g_bssPad_00e710f4[4] = {0};
// GLOBAL: FF7 0x00e710f8
int g_ChocoboRaceAdvanceEnabled = 0;
// GLOBAL: FF7 0x00e710fc
int g_ChocoboRaceResetSentinel = 0;
// GLOBAL: FF7 0x00e71100
int g_ChocoboPrevInputLatch = 0;
// GLOBAL: FF7 0x00e71104
int g_ChocoboSpriteClearedWord = 0;
// GLOBAL: FF7 0x00e71108
int g_ChocoboSubmittedRecordCount = 0;
unsigned char g_bssPad_00e7110c[4] = {0};
// GLOBAL: FF7 0x00e71110
struct ChocoboRaceState g_ChocoboCurrentRouteEvent = {0};
// GLOBAL: FF7 0x00e71124
int g_ChocoboCameraBaseSegment = 0;
// GLOBAL: FF7 0x00e71128
int g_ChocoboPlayerRides = 0;
// GLOBAL: FF7 0x00e7112c
ChocoboTrackData *g_ChocoboTrackData = 0;
// clang-format on
/*
 * Stores context/mode and selects mode-specific viewport, geometry offsets, scale fields,
 * descriptor/text tables and number-render callback. Mode two uses the large resource set and
 * 640-wide viewport; other modes use the small set and 320-wide viewport at origin for mode zero
 * or offset for other modes. Retains differing Y viewport and geometry offsets.
 */
// FUNCTION: FF7 0x0076d320
void __cdecl Mini_ChocoboApplyUiScalingModeConfig(int context, int mode)
{
    g_ChocoboUiContext = context;
    g_ChocoboUiScalingMode = mode;
    if (mode == 2) {
        Mini_ChocoboSetViewportRect(0, 8, 640, 464);
        g_ChocoboUiScaleFactor = 2;
        g_ChocoboScreenReference = FLOAT_232_BITS;
        g_ChocoboViewportOffsetX = 0;
        g_ChocoboViewportOffsetY = 0;
        g_ChocoboViewportOffsetBase = 8;
        g_ChocoboBetScreenLabels = (ChocoboSpriteRecord *)&g_ChocoboBetScreenLabels2x;
        g_ChocoboResultScreenLabels = (ChocoboSpriteRecord *)&g_ChocoboResultScreenLabels2x;
        g_ChocoboBetScreenPanels = (ChocoboSpriteRecord *)&g_ChocoboBetScreenPanels2x;
        g_ChocoboResultScreenPanels = (ChocoboSpriteRecord *)&g_ChocoboResultScreenPanels2x;
        g_ChocoboBetRacerSprites = (ChocoboSpriteRecord *)&g_ChocoboBetRacerSprites2x;
        g_ChocoboHudSpriteA = (ChocoboSpriteRecord *)&g_ChocoboHudSpriteRecord0;
        g_ChocoboHudSpriteB = (ChocoboSpriteRecord *)&g_ChocoboHudSpriteRecord1;
        g_ChocoboBetScreenLabelCount = 11;
        g_ChocoboDrawNumberFn = Mini_ChocoboDrawNumber2x;
        g_ChocoboResultLayout = (struct ChocoboResultUiEntry *)&g_ChocoboResultRacerSprites2x;
        g_ChocoboCourseSpriteSize = 32;
    } else {
        g_ChocoboBetScreenLabels = (ChocoboSpriteRecord *)&g_ChocoboBetScreenLabels1x;
        g_ChocoboResultScreenLabels = (ChocoboSpriteRecord *)&g_ChocoboResultScreenLabels1x;
        g_ChocoboBetScreenPanels = (ChocoboSpriteRecord *)&g_ChocoboBetScreenPanels1x;
        g_ChocoboResultScreenPanels = (ChocoboSpriteRecord *)&g_ChocoboResultScreenPanels1x;
        g_ChocoboBetRacerSprites = (ChocoboSpriteRecord *)&g_ChocoboBetRacerSprites1x;
        g_ChocoboHudSpriteA = (ChocoboSpriteRecord *)&g_ChocoboHudSpriteRecord2;
        g_ChocoboHudSpriteB = (ChocoboSpriteRecord *)&g_ChocoboHudSpriteRecord3;
        g_ChocoboBetScreenLabelCount = 10;
        g_ChocoboDrawNumberFn = Mini_ChocoboDrawNumber1x;
        g_ChocoboResultLayout = (struct ChocoboResultUiEntry *)&g_ChocoboResultRacerSprites1x;
        g_ChocoboCourseSpriteSize = 16;
        g_ChocoboUiScaleFactor = 1;
        g_ChocoboScreenReference = FLOAT_232_BITS;
        g_ChocoboViewportOffsetBase = 4;
        if (mode == 0) {
            Mini_ChocoboSetViewportRect(0, 4, 320, 232);
            g_ChocoboViewportOffsetX = 0;
            g_ChocoboViewportOffsetY = 0;
        } else {
            Mini_ChocoboSetViewportRect(160, 124, 320, 232);
            g_ChocoboViewportOffsetX = 0xA0;
            g_ChocoboViewportOffsetY = 0x78;
        }
    }
}
/*
 * Mechanical setter for four adjacent globals: stores viewX in viewport x, viewY in viewport y,
 * viewWidth in viewport width and viewHeight in viewport height.
 */
// FUNCTION: FF7 0x0076d4e6
void __cdecl Mini_ChocoboSetViewportRect(int viewX, int viewY, int viewWidth, int viewHeight)
{
    g_ChocoboViewportX = viewX;
    g_ChocoboViewportWidth = viewWidth;
    g_ChocoboViewportY = viewY;
    g_ChocoboViewportHeight = viewHeight;
}

/*
 * Frees a chocobo model record and associated RSD model if present.
 */
// FUNCTION: FF7 0x0076d50d
void __cdecl Mini_ChocoboFreeModelRecord(ChocoboModelRecord *record, void *context)
{
    if (record != 0) {
        Pfile_FreeRsdModel(record->rsdModel, context);
        // STRING: FF7 0x0096e34c
        GameHeapFree(record, "C:\\FF7\\chocobo\\ch_app.cpp", 171);
    }
}
// Allocates the chocobo model record (0x10 bytes) from the game heap and sets the three fields
// that the zero fill would leave wrong: count and count2 to 1, and kind to 4. Returns 0 if the heap
// refuses the allocation.
// FUNCTION: FF7 0x0076d541
int __cdecl Mini_ChocoboAllocModelRecord(void)
{
    ChocoboModelRecord *rec;

    // STRING: FF7 0x0096e368
    rec = (ChocoboModelRecord *)GameHeapAlloc(1, 16, "C:\\FF7\\chocobo\\ch_app.cpp", 179);
    if (rec != 0) {
        rec->count = 1;
        rec->count2 = 1;
        rec->kind = 4;
    }
    return (int)rec;
}
/*
 * Points the chocobo module's current-entity pointer at the current entity global.
 */
// FUNCTION: FF7 0x0076d588
void __cdecl Mini_ChocoboSetSoundCommandBlockPointer(void)
{
    g_pChocoboSoundCommand = &g_ChocoboCurrentEntity;
}
/*
 * Enters the chocobo race (game mode GAME_MODE_CHOCOBO_RACE), the module's installed enter
 * callback. Sets the retail flag (so files come from chocobo.lgp, LGP slot 7) and clears the
 * late-frame flag g_ChocoboFrameLate, then, under a constant `if (1)`:
 * - sets the context's near and far planes to 100.0 and 80000.0 and selects Direct3D 5;
 * - applies the GTE screen distance g_ChocoboScreenReference (232.0, set by the UI scaling
 *   config), sets the animation transform flags to 0x8051 and matrix layout mode 0;
 * - clears g_ChocoboExitFlag, sets the default model load flags to 0x100000 and scene pass 0;
 * - caches the DirectDraw object and surfaces, unpacks the palette tables, points the sound
 *   command block at g_ChocoboCurrentEntity and sets all SFX channels to volume 127;
 * - sets the frame duration to the timestamp frequency / 30 (a 30 fps race), caches the pixel
 *   format, applies the viewport rectangle the UI scaling mode chose, and sets
 *   g_ChocoboFramePacingOn, which despite its name turns on frame pacing in
 *   `Mini_ChocoboTick` (nothing clears it);
 * - last, `Mini_ChocoboSetupCamera` loads the race and places the camera.
 */
// FUNCTION: FF7 0x0076d597
void __cdecl Mini_ChocoboEnter(GfxEngineContext *ctx)
{
    char p[128];
    char r[128];
    int setupState;
    int transformFlags;
    setupState = 0; /* setupState is stored, not read */
    g_ChocoboRetailFlag = 1;
    g_ChocoboFrameLate = 0;
    /* `if (1)`: the original tests a constant here, which VC5 does not fold */
    if (1) {
        (*(int *)&ctx->nearPlane) = FLOAT_100_BITS; /* 100.0f */
        (*(int *)&ctx->farPlane) = FLOAT_80000_BITS; /* 80000.0f */
        ctx->useDirect3D5 = 1;
        SetGeomScreenReference(*(float *)&g_ChocoboScreenReference);
        Anim_GetDefaultTransformFlags(&transformFlags);
        transformFlags = 0x8051;
        Anim_SetTransformFlags(&transformFlags);
        SetMatrixLayoutMode(0);
        setupState = 1;
        g_ChocoboExitFlag = 0;
        Pfile_SetDefaultModelFlags(0x100000);
        Gfx_SetScenePassCall(0, ctx);
        Mini_ChocoboCacheDDrawIfaces(ctx);
        Mini_ChocoboLoadPaletteTable();
        Mini_ChocoboSetSoundCommandBlockPointer();
        SfxSetAllChannelsVolume(127);
        g_ChocoboFrameDuration = ctx->tscFrequency / 30.0;
        Mini_ChocoboCachePixelFormat(ctx);
        GfxSetViewportRect(g_ChocoboViewportX, g_ChocoboViewportY, g_ChocoboViewportWidth, g_ChocoboViewportHeight, ctx);
        g_ChocoboFramePacingOn = 1;
        Mini_ChocoboSetupCamera();
    }
}
/*
 * Loads the race and sets up its fixed camera; called once from `Mini_ChocoboEnter`. Builds model
 * load options from the defaults with flag 0x100000 and no blending, and passes them with the
 * engine context to `Mini_ChocoboStartRaceSequence`, which reads the savemap, loads the course
 * and builds the six racers. Then rebuilds the projection with offset (0, 0), builds the GTE
 * camera matrix g_ChocoboGteMatrix as a 180-degree turn about Z (angle 0x800 of 0x1000) with
 * translation (320, 240, 900), loads it as the GTE rotation and translation, converts it into the
 * float view matrix g_ChocoboViewMatrix and hands a copy of that to the renderer.
 */
// FUNCTION: FF7 0x0076d6df
void __cdecl Mini_ChocoboSetupCamera(void)
{
    PfileLoadOptions loadParams;
    GfxEngineContext *engineCtxPtr;
    PfileLoadOptions *pOptions;
    short rotation[4];
    VECTOR translation;
    engineCtxPtr = GetEngineContext();
    pOptions = &loadParams;
    Pfile_InitLoadOptions(pOptions);
    pOptions->flags |= 0x100000;
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, pOptions);
    Mini_ChocoboStartRaceSequence(engineCtxPtr, pOptions);
    SetProjectionOffsetAndRebuild(0, 0);
    rotation[0] = 0;
    rotation[1] = 0;
    rotation[2] = 0x800 /* 180 deg */;
    RotMatrixXYZ(rotation, &g_ChocoboGteMatrix);
    translation.vx = 320;
    translation.vy = 240;
    translation.vz = 900;
    SetMatrixTranslation(&g_ChocoboGteMatrix, &translation);
    SetRotMatrix(&g_ChocoboGteMatrix);
    SetTransMatrix(&g_ChocoboGteMatrix);
    GtePsxMatrixToFloat4x4((PsxMatrix *)&g_ChocoboGteMatrix, &g_ChocoboViewMatrix);
    Mini_ChocoboSnapshotViewMatrix(engineCtxPtr);
}
/*
 * Frees the track data: with the retail flag clear, the eight blocks the loader allocated one by
 * one; with it set, the chocobo.wat buffer they point into instead. Either way it then frees three
 * more blocks and the 85-byte header. Each block is null-tested and cleared after the free. Last,
 * releases the course sounds.
 */
// FUNCTION: FF7 0x0076d7d8
void __cdecl Mini_ChocoboFreeTrackData(void)
{
    if (g_ChocoboTrackData != 0) {
        if (g_ChocoboRetailFlag == 0) {
            if (g_ChocoboTrackData->routeEventTable != 0) {
                // STRING: FF7 0x0096e384
                GameHeapFree((void *)g_ChocoboTrackData->routeEventTable, "C:\\FF7\\chocobo\\ch_app.cpp", 377);
                g_ChocoboTrackData->routeEventTable = 0;
            }
            if (g_ChocoboTrackData->block != 0) {
                // STRING: FF7 0x0096e3a0
                GameHeapFree((void *)g_ChocoboTrackData->block, "C:\\FF7\\chocobo\\ch_app.cpp", 382);
                g_ChocoboTrackData->block = 0;
            }
            if (g_ChocoboTrackData->floatVertexTable != 0) {
                // STRING: FF7 0x0096e3bc
                GameHeapFree((void *)g_ChocoboTrackData->floatVertexTable, "C:\\FF7\\chocobo\\ch_app.cpp", 387);
                g_ChocoboTrackData->floatVertexTable = 0;
            }
            if (g_ChocoboTrackData->triangleTable != 0) {
                // STRING: FF7 0x0096e3d8
                GameHeapFree((void *)g_ChocoboTrackData->triangleTable, "C:\\FF7\\chocobo\\ch_app.cpp", 392);
                g_ChocoboTrackData->triangleTable = 0;
            }
            if (g_ChocoboTrackData->quadTable != 0) {
                // STRING: FF7 0x0096e3f4
                GameHeapFree((void *)g_ChocoboTrackData->quadTable, "C:\\FF7\\chocobo\\ch_app.cpp", 397);
                g_ChocoboTrackData->quadTable = 0;
            }
            if (g_ChocoboTrackData->trackVertexTable != 0) {
                // STRING: FF7 0x0096e410
                GameHeapFree((void *)g_ChocoboTrackData->trackVertexTable, "C:\\FF7\\chocobo\\ch_app.cpp", 402);
                g_ChocoboTrackData->trackVertexTable = 0;
            }
            if (g_ChocoboTrackData->courseSpriteTable != 0) {
                // STRING: FF7 0x0096e42c
                GameHeapFree((void *)g_ChocoboTrackData->courseSpriteTable, "C:\\FF7\\chocobo\\ch_app.cpp", 407);
                g_ChocoboTrackData->courseSpriteTable = 0;
            }
            if (g_ChocoboTrackData->racingLineTable != 0) {
                // STRING: FF7 0x0096e448
                GameHeapFree((void *)g_ChocoboTrackData->racingLineTable, "C:\\FF7\\chocobo\\ch_app.cpp", 412);
                g_ChocoboTrackData->racingLineTable = 0;
            }
        } else if (g_pChocoboWatBuffer != 0) {
            // STRING: FF7 0x0096e464
            GameHeapFree(g_pChocoboWatBuffer, "C:\\FF7\\chocobo\\ch_app.cpp", 419);
            g_pChocoboWatBuffer = 0;
        }
        if (g_ChocoboTrackData->builtQuads != 0) {
            // STRING: FF7 0x0096e480
            GameHeapFree((void *)g_ChocoboTrackData->builtQuads, "C:\\FF7\\chocobo\\ch_app.cpp", 425);
            g_ChocoboTrackData->builtQuads = 0;
        }
        if (g_ChocoboTrackData->builtTriangles != 0) {
            // STRING: FF7 0x0096e49c
            GameHeapFree((void *)g_ChocoboTrackData->builtTriangles, "C:\\FF7\\chocobo\\ch_app.cpp", 430);
            g_ChocoboTrackData->builtTriangles = 0;
        }
        if (g_ChocoboTrackData->builtFloatVertices != 0) {
            // STRING: FF7 0x0096e4b8
            GameHeapFree((void *)g_ChocoboTrackData->builtFloatVertices, "C:\\FF7\\chocobo\\ch_app.cpp", 435);
            g_ChocoboTrackData->builtFloatVertices = 0;
        }
        // STRING: FF7 0x0096e4d4
        GameHeapFree(g_ChocoboTrackData, "C:\\FF7\\chocobo\\ch_app.cpp", 439);
        g_ChocoboTrackData = 0;
    }
    Mini_ChocoboReleaseCourseSounds();
}
/*
 * Leaves the chocobo race: frees the racers, the HUD render objects, the track data and the block at
 * g_ChocoboRaceHeapBlock, destroys the course sprite and fixed render objects, clears the DirectDraw
 * pointers and sets the exit flag, which makes the next frame switch back to the field.
 */
// FUNCTION: FF7 0x0076da88
void __cdecl Mini_ChocoboExit(void)
{
    Mini_ChocoboFreeRacers();
    Mini_ChocoboDestroyHudRenderObjects();
    Mini_ChocoboFreeTrackData();
    if (g_ChocoboRaceHeapBlock != 0) {
        // STRING: FF7 0x0096e4f0
        GameHeapFree((void *)g_ChocoboRaceHeapBlock, "C:\\FF7\\chocobo\\ch_app.cpp", 458);
        g_ChocoboRaceHeapBlock = 0;
    }
    Mini_ChocoboRaceDestroyCourseSpriteObjects();
    Mini_ChocoboDestroyFixedRenderObjects();
    g_ChocoboDirectDraw = 0;
    g_ChocoboPrimarySurface = 0;
    g_ChocoboExitFlag = 1;
}
//
// Chocobo module dispatch callbacks and input-state helpers.

__declspec(dllimport) unsigned long __stdcall SetBkColor(void *, unsigned long);
__declspec(dllimport) unsigned long __stdcall SetTextColor(void *, unsigned long);
__declspec(dllimport) int __stdcall TextOutA(void *, int, int, const char *, int);
__declspec(dllimport) int __stdcall lstrlenA(const char *);
// g_ChocoboKeyboardBindings is defined and initialized in chocobo_mini_chocobo_clear.c.

/*
 * Sets the supplied GDI device context background COLORREF to 0x00ff0000 (blue) and text COLORREF
 * to 0x0000ffff (yellow), then draws the NUL-terminated string with TextOutA using its lstrlenA length.
 */
// FUNCTION: FF7 0x0076daef
void __cdecl Mini_ChocoboDrawDebugText(int x, int y, const char *text, void *dc)
{
    SetBkColor(dc, 0xff0000);
    SetTextColor(dc, 0xffff);
    TextOutA(dc, x, y, text, lstrlenA(text));
}
/*
 * When the exit flag is set, clears the screen to black, switches the game mode to the field and
 * requests a switch to the GameModeDispatcher module (enter, exit, frame and the app's input
 * callbacks), then clears the exit flag. The module record's first 8 bytes are left unset.
 */
// FUNCTION: FF7 0x0076db33
void __cdecl Mini_ChocoboReturnToField(struct GfxEngineContext *ctx)
{
    GfxClearColorBgra color;
    GameModule module;
    if (g_ChocoboExitFlag != 0) {
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = FLOAT_ONE_BITS;
        GfxSetBackgroundColorCall(&color, ctx);
        GfxClearAllCall(ctx);
        g_EnginePreviousMode = GAME_MODE_CHOCOBO_RACE;
        g_EngineGameMode = GAME_MODE_FIELD;
        module.frame = GameModeDispatcher_Frame;
        module.onMouse = App_OnMouseMessage;
        module.onKey = App_OnKeyMessage;
        module.enter = GameModeDispatcher_Enter;
        module.exit = GameModeDispatcher_Exit;
        RequestModuleSwitch(&module, ctx);
        g_ChocoboExitFlag = 0;
    }
}
/*
 * Records the frame's start time, clears the screen to black, polls the input latches and runs the
 * race-result loop, then ticks the minigame while the exit flag is clear and returns to the field
 * once it is set. Reserved locals are unused.
 */
// FUNCTION: FF7 0x0076dbd1
void __cdecl Mini_ChocoboFrame(struct GfxEngineContext *ctx)
{
    GfxClearColorBgra color;
    int zero;
    int oneFlag;
    zero = 0;
    oneFlag = 1;
    ReadTimestampCounter(&g_ChocoboFrameStartTimestamp);
    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = FLOAT_ONE_BITS;
    GfxSetBackgroundColorCall(&color, ctx);
    GfxClearAllCall(ctx);
    Mini_ChocoboPollInputLatches();
    Mini_ChocoboRunRaceResultLoop();
    if (g_ChocoboExitFlag == 0) {
        Mini_ChocoboTick(ctx);
    } else {
        Mini_ChocoboReturnToField(ctx);
    }
}
/*
 * Reads the timestamp counter, processes input/state helpers Mini_ChocoboPollInputLatches and Mini_ChocoboRunRaceResultLoop, runs
 * Mini_ChocoboTick only when exit flag is zero, then always calls Mini_ChocoboReturnToField.
 */
// FUNCTION: FF7 0x0076dc5b
void __cdecl Mini_ChocoboStepFrame(struct GfxEngineContext *ctx)
{
    int p;
    int q;
    int r;
    int s;
    int zero;
    int oneFlag;
    zero = 0;
    oneFlag = 1;
    ReadTimestampCounter(&g_ChocoboFrameStartTimestamp);
    Mini_ChocoboPollInputLatches();
    Mini_ChocoboRunRaceResultLoop();
    if (g_ChocoboExitFlag == 0) {
        Mini_ChocoboTick(ctx);
    }
    Mini_ChocoboReturnToField(ctx);
}
/*
 * The race's installed mouse callback. It does nothing: the race reads its input from the latches
 * `Mini_ChocoboFrame` polls each frame, not from window messages.
 *
 * Codegen: the original reserves three unused local slots and makes no calls.
 */
// FUNCTION: FF7 0x0076dcab
void __cdecl Mini_ChocoboMouse(void)
{
    int p;
    int q;
    int r;
    /* The original reserves three unused local slots. */
}
/*
 * The race's installed keyboard callback. It does nothing: the race reads its input from the
 * latches `Mini_ChocoboFrame` polls each frame, not from window messages.
 *
 * Codegen: the original reserves three unused local slots and makes no calls.
 */
// FUNCTION: FF7 0x0076dcb5
void __cdecl Mini_ChocoboKeyboard(void)
{
    int p;
    int q;
    int r;
    /* The original reserves three unused local slots. */
}
/*
 * Disables game heap tracking and creates an engine context. On success sets bootstrap
 * graphics/configuration fields, applies the geometry screen reference, overwrites the retrieved
 * animation flags with 0x8051, loads APP.CFG and installs that engine context.
 */
// FUNCTION: FF7 0x0076dcbf
void __cdecl Mini_ChocoboInitEngineContextAndLoadAppCfg(int value, int param)
{
    const char *path;
    int flags;
    GfxEngineContext *ctx;

    SetGameHeapTrackingEnabled(0);
    ctx = (GfxEngineContext *)CreateEngineContext();
    if (ctx != 0) {
        ctx->colorDepth = 0x10;
        ctx->fullScreen = 1;
        ctx->modeActive = 1;
        ctx->modeTransition = 0;
        ctx->modeSetupDone = 1;
        /* tests the value it has just stored, as the original does */
        if (ctx->modeSetupDone != 0) {
            ctx->modeLoaded = 1;
        }
        ctx->deviceIndex = 0;
        ctx->showCursor = 1;
        (*(int *)&ctx->nearPlane) = FLOAT_100_BITS; /* 100.0f */
        (*(int *)&ctx->farPlane) = FLOAT_80000_BITS; /* 80000.0f */
        ctx->useDirect3D5 = 1;
        SetGeomScreenReference(*(float *)&g_ChocoboScreenReference);
        Anim_GetDefaultTransformFlags(&flags);
        flags = 0x8051;
        Anim_SetTransformFlags(&flags);
        // STRING: FF7 0x0096e50c
        path = "APP.CFG";
        Gfx_LoadAppConfigFile((char *)path, (GfxEngineContext *)ctx);
        SetEngineContext((int)ctx);
    }
}
/*
 * The retail debug callback reserves a device-context slot, two integers and a 200-byte buffer,
 * but performs no work.
 */
// FUNCTION: FF7 0x0076ddd6
void __cdecl Mini_ChocoboDebugOverlayNoOp(struct GfxEngineContext *ctx)
{
    void *dc;
    int p;
    int q;
    char text[200];
}
} /* extern "C" */

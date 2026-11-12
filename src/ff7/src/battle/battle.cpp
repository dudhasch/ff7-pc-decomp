// src/ff7/src/battle/battle.cpp
// Original: C:\FF7\src\Battle\Battle.cpp
// Contents: The battle module lifecycle: entering and leaving the battle module, per-session setup
//   and shutdown, the frame limiter and frame loop, the phase state machine, clearing battle work
//   RAM, and a debug key handler.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdio.h>
#include "lib/codegen.h"
#include "lib/com_abi.h"
#include "ff7/api/battle.h"
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/movie.h"
#include "lib/api/token.h"
#include "ff7/api/main.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/enums/battle.h"
#include <string.h>
#include "ff7/savemap.h"
#include <windows.h>
#include "lib/types/mem.h"
#include "ff7/enums/menu.h"
#include "ff7/types/menu.h"
#include "ff7/field.h"

struct GfxEngineContext;
extern "C" {
// clang-format off
// Battle timing constants; only this object's code reads them.
// GLOBAL: FF7 0x007c0af0
double g_BattleTimingFrameRate = 60.0;
// GLOBAL: FF7 0x007c0af8
int g_BattleDebugToggle = 1;
// GLOBAL: FF7 0x007c0b00
double g_BattleTimingStepDivisor = 15.0;
// GLOBAL: FF7 0x007c0b08
double g_BattleFrameMinFps = 14.0;
// GLOBAL: FF7 0x007c0b10
double g_BattleFrameMaxFps = 15.0;
// GLOBAL: FF7 0x007c0b18
double g_BattleTimingScaleFactor = 0.8;
unsigned char g_rawData_007c0b20[4] = {0x01, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007c0b24
int g_BattleDarkenScreenGlobalAlphaEnabled = 1;
// GLOBAL: FF7 0x009a8748
BattleSceneState g_BattleScene = {0};
// GLOBAL: FF7 0x009a88c0
BattleMenuGraphicsPathBuffer g_BattleMenuGraphicsPathBuffer = {0};
// GLOBAL: FF7 0x009a89d0
int g_BattleSceneRecordLoaded = 0;
unsigned char g_bssPad_009a89d4[4] = {0};
// GLOBAL: FF7 0x009a89d8
Timestamp64 g_BattleTimestampCounterSample = {0};
// GLOBAL: FF7 0x009a89e0
int g_BattleProjectionOffsetX = 0;
// GLOBAL: FF7 0x009a89e4
int g_BattleProjectionOffsetY = 0;
// GLOBAL: FF7 0x009a89e8
Timestamp64 g_BattleFrameStepTicks = {0};
unsigned char g_bssPad_009a89f0[8] = {0};
// GLOBAL: FF7 0x009a89f8
char g_BattleDataDirectoryPathBuffer[256] = {0};
// GLOBAL: FF7 0x009a8af8
int g_BattleSessionEntryCount = 0;
// GLOBAL: FF7 0x009a8afc
int g_BattleSequenceWaitPlayed = 0;
// GLOBAL: FF7 0x009a8b00
int g_BattleSequenceEndReached = 0;
// GLOBAL: FF7 0x009a8b04
short g_BattleDefeatMusicPending = 0;
// GLOBAL: FF7 0x009a8b08
short g_BattleAtbFrozen = 0;
unsigned char g_bssPad_009a8b0c[4] = {0};
// Aliased as g_BattleState in include/ff7/api/battle.h.
// GLOBAL: FF7 0x009a8b10
struct BattleState DAT_009a8b10 = {0};
// GLOBAL: FF7 0x009aad3c
int g_BattleSceneNumber = 0;
// GLOBAL: FF7 0x009aad40
int g_BattleActionQueueMode = 0;
// GLOBAL: FF7 0x009aad44
int g_BattleEndSequenceDone = 0;
// Battle_InitSession clears it; no code reads it.
// GLOBAL: FF7 0x009aad48
int g_BattleUnreadSessionWord = 0;
// GLOBAL: FF7 0x009aad4c
int g_BattleViewportOriginX = 0;
// GLOBAL: FF7 0x009aad50
int g_BattleViewportOriginY = 0;
unsigned char g_bssPad_009aad54[4] = {0};
// GLOBAL: FF7 0x009aad58
int g_BattleArenaChooserPending = 0;
// GLOBAL: FF7 0x009aad5c
int g_BattleViewportWidth = 0;
// GLOBAL: FF7 0x009aad60
int g_BattleArenaChooserDone = 0;
// GLOBAL: FF7 0x009aad64
int g_BattlePhase = 0;
// GLOBAL: FF7 0x009aad68
int g_BattleSceneViewportHeight = 0;
// GLOBAL: FF7 0x009aad6c
unsigned short g_BattleResultMenuNextMode = 0;
// GLOBAL: FF7 0x009aad70
BattleSequenceBuffer g_BattleSequenceBuffer = {0};
unsigned char g_bssPad_009ab07c[4] = {0};
// GLOBAL: FF7 0x009ab080
struct Timestamp64 g_BattleFrameLimiterTimestamp = {0};
// GLOBAL: FF7 0x009ab088
Timestamp64 g_BattleSessionStartTimestamp = {0};
// GLOBAL: FF7 0x009ab090
double g_BattleFrameLimiterTargetTime = 0.0;
// GLOBAL: FF7 0x009ab098
int g_BattleArenaSlotPenaltyDone = 0;
unsigned char g_bssPad_009ab09c[4] = {0};
// GLOBAL: FF7 0x009ab0a0
struct BattleAiGlobals gBattleAiGlobals = {0};
// GLOBAL: FF7 0x009ab9ec
int g_BattleArenaRoundBreakDone = 0;
// Battle_InitSession clears it; no code reads it.
// GLOBAL: FF7 0x009ab9f0
int g_BattleUnreadPhaseWord = 0;
// GLOBAL: FF7 0x009ab9f4
unsigned char g_BattleMessageWindowTextId[0x4] = {0};
// GLOBAL: FF7 0x009ab9f8
int g_BattleSequenceWaitDone = 0;
unsigned char g_bssPad_009ab9fc[4] = {0};
// Battle_ModuleFrame clears these two when it runs the phase machine once; no code reads them.
// GLOBAL: FF7 0x009aba00
int g_BattleUnreadFrameWordA = 0;
// GLOBAL: FF7 0x009aba04
int g_BattleUnreadFrameWordB = 0;
// GLOBAL: FF7 0x009aba08
BattleDamageDisplayRecord g_BattleDamageDisplayRecords[128] = {0};
// GLOBAL: FF7 0x009ac108
int g_BattleViewportMaxX = 0;
unsigned char g_bssPad_009ac10c[4] = {0};
// Aliased as g_BattleItemListBlock in include/ff7/api/battle.h.
// GLOBAL: FF7 0x009ac110
BattleItemListBlock DAT_009ac110 = {0};
// GLOBAL: FF7 0x009acb58
int g_BattleRoundSequenceDone = 0;
// GLOBAL: FF7 0x009acb5c
int g_BattleUiScalingMode = 0;
unsigned char g_bssPad_009acb60[4] = {0};
// GLOBAL: FF7 0x009acb64
int g_BattleSceneLoadDone = 0;
// GLOBAL: FF7 0x009acb68
int g_BattleSequencePlayDone = 0;
unsigned char g_bssPad_009acb6c[4] = {0};
// GLOBAL: FF7 0x009acb70
struct Timestamp64 g_BattleFrameLimiterLastTimestamp = {0};
// GLOBAL: FF7 0x009acb78
Timestamp64 g_BattleTimestampScaledAccumulator = {0};
// GLOBAL: FF7 0x009acb80
int g_BattleArenaSlotSequenceActive = 0;
// GLOBAL: FF7 0x009acb84
int g_BattleDigitGlyphBase = 0;
// GLOBAL: FF7 0x009acb88
int g_BattleViewportHeight = 0;
// GLOBAL: FF7 0x009acb8c
int g_BattleActionQueueResult = 0;
// GLOBAL: FF7 0x009acb90
int g_BattleModeTransition = 0;
unsigned char g_bssPad_009acb94[4] = {0};
// The battle's hit records: 128 records of 12 bytes (BattleHitRecord). Battle_ClearWorkRam clears the
// whole table; battle_sequence_step_records.cpp walks it for q < 0x80; Battle3d_PushHitDisplayEntry
// (battle_opening_actor_setup.cpp) copies a record's status into the effect-order table. It is one
// object so that every record keeps its 12-byte stride.
// Aliased as g_BattleEffectOrderTable in include/ff7/api/battle.h.
// GLOBAL: FF7 0x009acb98
struct BattleHitRecord DAT_009acb98[128] = {0};
// GLOBAL: FF7 0x009ad198
unsigned char g_BattleViewportMaxY[8] = {0};
// GLOBAL: FF7 0x009ad1a0
Timestamp64 g_BattleTimestampAccumulator = {0};
// GLOBAL: FF7 0x009ad1a8
int g_BattleUiScale = 0;
// GLOBAL: FF7 0x009ad1ac
int g_BattleIsRunning = 0;
// GLOBAL: FF7 0x009ad1b0
unsigned char g_BattleSavedPartyBytes[4] = {0};
// GLOBAL: FF7 0x009ad1b4
int g_BattleReplayActionCount = 0;
// GLOBAL: FF7 0x009ad1b8
int g_BattleDebugKeyState = 0;
// GLOBAL: FF7 0x009ad1bc
void (__cdecl *g_BattleLoadSceneHandler)(int) = 0;
// GLOBAL: FF7 0x009ad1c0
void (__cdecl *g_BattleRoundSequenceHandler)(void) = 0;
// GLOBAL: FF7 0x009ad1c4
void (__cdecl *g_BattlePlaySequenceHandler)(void) = 0;
// GLOBAL: FF7 0x009ad1c8
int g_BattleArenaRoundStep = 0;
// GLOBAL: FF7 0x009ad1cc
int g_BattleNextBattleStep = 0;
// GLOBAL: FF7 0x009ad1d0
int g_BattleExitStep = 0;
// GLOBAL: FF7 0x009ad1d4
int g_BattleFrameLimiterFlag = 0;
// GLOBAL: FF7 0x009ad1d8
double g_BattleFrameLimiterCarryTime = 0.0;
// clang-format on

#pragma intrinsic(memset, strcpy, strcat)


static __inline void Battle_ShutdownSession(void);
static __inline void __cdecl Battle_InitSession(struct GfxEngineContext *context);

// The retail call arguments round directly onto the outgoing stack. /Op adds
// extra local spills here, so this function compiles without it.
#pragma optimize("p", off)
// Logs battle initialization, initializes the session, selects asset source from g_BattleAssetSourceModeSelector and
// configures matrix layout, screen reference 240 and transform flags 0x8051. Sets context
// perspective values and calls SetPerspective. For UI modes zero/one configures 320 by 240 with
// origin zero or 160/120; mode two uses 640 by 480 and multiplier two. Unsupported modes retain
// prior layout globals. Computes right/bottom limits, copies the context's modeTransition, loads
// battle UI textures and logs exit. The projection's near plane is 250.0f and its far plane 64000.0f.
// FUNCTION: FF7 0x0041b300
void __cdecl Battle_ModuleEnter(GfxEngineContext *ctx)
{
    int val;
    Gfx_DebugPrint(
                           // STRING: FF7 0x007c0c3c
                           "[BATTLE] Entering FRAME_INITIALIZE\n");
    Battle_InitSession(ctx);
    if (g_BattleAssetSourceModeSelector != 0) {
        Pfile_SetAssetSourceMode(0);
    } else {
        Pfile_SetAssetSourceMode(1);
    }
    SetMatrixLayoutMode(1);
    SetGeomScreenReference(240.0);
    val = 0x8051;
    Anim_SetTransformFlags(&val);
    ctx->offsetX = 0.0f;
    ctx->offsetY = 0.0f;
    ctx->fov = 90.0f;
    ctx->nearPlane = 250.0f;
    ctx->farPlane = 64000.0f;
    SetPerspective(ctx->fov, ctx->nearPlane, ctx->farPlane, ctx->offsetX, ctx->offsetY, (float)ctx->viewSizeX, (float)ctx->viewSizeY, ctx);
    g_BattleUiScalingMode = GetUiScalingMode();
    switch (g_BattleUiScalingMode) {
    case MAIN_GRAPHICS_MODE_320X240:
        g_BattleProjectionOffsetX = 0;
        g_BattleProjectionOffsetY = 37;
        g_BattleViewportOriginX = 0;
        g_BattleViewportOriginY = 0;
        g_BattleViewportWidth = 320;
        g_BattleViewportHeight = 240;
        g_BattleSceneViewportHeight = 166;
        g_BattleUiScale = 1;
        break;
    case MAIN_GRAPHICS_MODE_320X240_CENTERED:
        g_BattleProjectionOffsetX = 0;
        g_BattleProjectionOffsetY = 37;
        g_BattleViewportOriginX = 160;
        g_BattleViewportOriginY = 120;
        g_BattleViewportWidth = 320;
        g_BattleViewportHeight = 240;
        g_BattleSceneViewportHeight = 166;
        g_BattleUiScale = 1;
        break;
    case MAIN_GRAPHICS_MODE_640X480:
        g_BattleProjectionOffsetX = 0;
        g_BattleProjectionOffsetY = 74;
        g_BattleViewportOriginX = 0;
        g_BattleViewportOriginY = 0;
        g_BattleViewportWidth = 640;
        g_BattleViewportHeight = 480;
        g_BattleSceneViewportHeight = 332;
        g_BattleUiScale = 2;
        break;
    }
    g_BattleViewportMaxX = g_BattleViewportOriginX + (g_BattleViewportWidth - 1);
    (*(int *)&g_BattleViewportMaxY) = g_BattleViewportOriginY + (g_BattleSceneViewportHeight - 1);
    g_BattleModeTransition = ctx->modeTransition;
    Battle_LoadUiTextures();
    Gfx_DebugPrint(
                           // STRING: FF7 0x007c0c60
                           "[BATTLE] Exitting FRAME_INITIALIZE\n");
}

#pragma optimize("p", on)

// Initializes battle state, split counters, and resource directory roots.
// FUNCTION: FF7 0x0041b577
static __inline void __cdecl Battle_InitSession(struct GfxEngineContext *context)
{
    struct {
        char path[256];
        int index;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    Battle_ClearWorkRam();
    for (frame.index = 0; frame.index < 4; frame.index++)
        g_BattleSavedPartyBytes[frame.index] = gSavemap.party[frame.index];
    g_BattleSessionEntryCount = 0;
    g_BattleSceneNumber = (unsigned short)g_FieldRequestedBattleId;
    g_BattleScene.flags = g_BattleResultWord;
    g_BattleReplayActionCount = 0;
    // STRING: FF7 0x007c0b28
    sprintf(frame.path, "[BATTLE] Scene# %d\n", g_BattleSceneNumber);
    Gfx_DebugPrint(frame.path);
    g_BattleRoundSequenceDone = 0;
    g_BattleSequenceEndReached = 0;
    g_BattleSceneLoadDone = 0;
    g_BattleSequencePlayDone = 0;
    g_BattleSequenceWaitDone = 0;
    g_BattleSequenceWaitPlayed = 0;
    g_BattleSceneRecordLoaded = 0;
    g_BattleArenaRoundBreakDone = 0;
    g_BattleArenaChooserDone = 0;
    g_BattleArenaChooserPending = 0;
    g_BattleArenaSlotPenaltyDone = 0;
    g_BattleArenaSlotSequenceActive = 0;
    g_BattleEndSequenceDone = 0;
    g_BattleSequenceBuffer.phasePendingFlag = 0;
    g_BattlePhase = BATTLE_PHASE_INIT;
    g_BattleUnreadPhaseWord = 0;
    SplitFloatToSplitValue(context->tscFrequency / g_BattleTimingStepDivisor, &g_BattleFrameStepTicks);
    g_BattleFrameLimiterTargetTime = context->tscFrequency / g_BattleTimingStepDivisor;
    ReadTimestampCounter(&g_BattleTimestampCounterSample);
    Add64(&g_BattleFrameStepTicks, &g_BattleTimestampCounterSample);
    SplitFloatToSplitValue(CombineSplitValueToFloat(&g_BattleFrameStepTicks) * g_BattleTimingScaleFactor, &g_BattleTimestampScaledAccumulator);
    g_BattleUnreadSessionWord = 0;
    ReadTimestampCounter(&g_BattleSessionStartTimestamp);
    SplitFloatToSplitValue(context->tscFrequency / g_BattleTimingFrameRate, &g_BattleTimestampAccumulator);
    ReadTimestampCounter(&g_BattleMenuGraphicsPathBuffer.timestampSample);
    Add64(&g_BattleTimestampAccumulator, &g_BattleMenuGraphicsPathBuffer.timestampSample);
    if (g_BattleDebugToggle == 0)
        g_BattleIsRunning = 1;
    strcpy(g_BattleDataDirectoryPathBuffer, g_InitPath_DataPath);
    // STRING: FF7 0x007c0b3c
    strcat(g_BattleDataDirectoryPathBuffer, "BATTLE");
    Battle_SetDataDirectory(g_BattleDataDirectoryPathBuffer);
    if (g_BattleAssetSourceModeSelector != 0)
        // STRING: FF7 0x007c0b44
        BattleFx_SetResourceRoot("/FF7/DATA/BATTLE");
    else
        BattleFx_SetResourceRoot(g_BattleDataDirectoryPathBuffer);
    strcpy(g_BattleMenuGraphicsPathBuffer.path, g_InitPath_DataPath);
    // STRING: FF7 0x007c0b58
    strcat(g_BattleMenuGraphicsPathBuffer.path, "GRAPHICS/MENU");
    BattleFx_SetResourceRootAlt(g_BattleMenuGraphicsPathBuffer.path);
}

// Battle module exit: logs FRAME_QUIT, waits for async loads and sound group 15, shuts the session
// down, frees the UI textures, destroys the scene, resets the projection offset and clears the
// context's viewportHeightOverride.
// FUNCTION: FF7 0x0041b87d
void __cdecl Battle_ModuleExit(struct GfxEngineContext *ctx)
{
    // STRING: FF7 0x007c0c84
    Gfx_DebugPrint("[BATTLE] Entering FRAME_QUIT\n");
    Battle_WaitAsyncLoadsIdle();
    SfxWaitForGroupLoaded(0xf);
    Battle_ShutdownSession();
    Battle_FreeUiTextures();
    Battle_DestroyScene();
    SetProjectionOffset(0, 0);
    ctx->viewportHeightOverride = 0;
    // STRING: FF7 0x007c0ca4
    Gfx_DebugPrint("[BATTLE] Exitting FRAME_QUIT\n");
}

// Ends the battle session. It clears g_BattlePhase and the session entry count, frees
// g_BattleCameraGlobalScripts and clears that pointer, and copies the four saved party bytes in
// g_BattleSavedPartyBytes back into gSavemap.party. It then sends the message '[BATTLE] endof
// battle.' to the active target through Gfx_DebugPrint.
// FUNCTION: FF7 0x0041b8d3
static __inline void Battle_ShutdownSession(void)
{
    int partyIndex;
    char message[256];

    g_BattlePhase = BATTLE_PHASE_INIT;
    g_BattleSessionEntryCount = 0;
    // STRING: FF7 0x007c0b68
    GameHeapFree((void *)g_BattleCameraGlobalScripts, "C:\\FF7\\src\\Battle\\Battle.cpp", 0x143);
    g_BattleCameraGlobalScripts = 0;
    for (partyIndex = 0; partyIndex < 4; partyIndex++) {
        gSavemap.party[partyIndex] = g_BattleSavedPartyBytes[partyIndex];
    }
    // STRING: FF7 0x007c0b88
    sprintf(message, "[BATTLE] endof battle.\n");
    Gfx_DebugPrint(message);
}
 // extern "C"



/* C++ callback signature takes HDC rather than int. */
extern void __cdecl Gfx_StatsDrawTextRows(int context, int baseY, HDC callbackArg, void(__cdecl *)(int, int, const char *, HDC), struct GfxStatsRecord *statsRecord);
static __inline void __cdecl Battle_RunPhaseMachine(struct GfxEngineContext *ctx);

// Polls the timestamp-counter delta against g_BattleFrameLimiterTargetTime. Without saved overrun waits to a full
// duration or saves the excess; with an overrun waits to duration minus excess, then clears the
// overrun flag and amount. ctx is unused.
// FUNCTION: FF7 0x0041b965
void __cdecl Battle_FrameLimiterWait(struct GfxEngineContext *ctx)
{
    double elapsed;
    struct Timestamp64 tickDelta;
    elapsed = 0;
    ReadTimestampCounter(&g_BattleFrameLimiterTimestamp);
    Sub64(&g_BattleFrameLimiterTimestamp, &g_BattleFrameLimiterLastTimestamp, &tickDelta);
    elapsed = CombineSplitValueToFloat(&tickDelta);
    if (g_BattleFrameLimiterFlag == 0) {
        if (elapsed <= g_BattleFrameLimiterTargetTime) {
            while (elapsed < g_BattleFrameLimiterTargetTime) {
                ReadTimestampCounter(&g_BattleFrameLimiterTimestamp);
                Sub64(&g_BattleFrameLimiterTimestamp, &g_BattleFrameLimiterLastTimestamp, &tickDelta);
                elapsed = CombineSplitValueToFloat(&tickDelta);
            }
            g_BattleFrameLimiterFlag = 0;
            g_BattleFrameLimiterCarryTime = 0;
        } else {
            g_BattleFrameLimiterCarryTime = elapsed - g_BattleFrameLimiterTargetTime;
            g_BattleFrameLimiterFlag = 1;
        }
    } else {
        while (g_BattleFrameLimiterTargetTime - g_BattleFrameLimiterCarryTime > elapsed) {
            ReadTimestampCounter(&g_BattleFrameLimiterTimestamp);
            Sub64(&g_BattleFrameLimiterTimestamp, &g_BattleFrameLimiterLastTimestamp, &tickDelta);
            elapsed = CombineSplitValueToFloat(&tickDelta);
        }
        g_BattleFrameLimiterFlag = 0;
        g_BattleFrameLimiterCarryTime = 0;
    }
}

// Samples frame timing, services offscreen slots and resets draw pools, the scene's frame pools only
// when unpaused. Clears the full viewport, then on successful scene sets projection and battle
// viewport, runs phase logic once or twice according to timing/limiter controls, draws layers/HUD
// and conditionally updates camera. Runs four UI logic updates for state two, drawing lists only
// on the last; optionally waits then ends the scene. Debug drawing independently acquires/releases
// a surface DC and prints scene/FPS/limiter/skip information and optional object diagnostics. The
// skip local is initialized only in the timing-outlier branch yet read by debug output on other
// paths. Sound fades update even when scene start fails.
// FUNCTION: FF7 0x0041bab3
void __cdecl Battle_ModuleFrame(struct GfxEngineContext *ctx)
{
    struct {
        HDC debugDc;
        char debugText[200];
        int uiPass, padK, padZ;
        union {
            int bits[4];
            float rgba[4];
        } color;
        int uiPassCount, scenePass, fpsOutOfRange, padR, padQ, didCatchUpStep;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    ReadTimestampCounter(&g_BattleFrameLimiterLastTimestamp);
    if (g_BattleDebugToggle != 0) {
        if (ctx->fps < g_BattleFrameMinFps || ctx->fps > g_BattleFrameMaxFps) {
            frame.fpsOutOfRange = 1;
        } else {
            frame.fpsOutOfRange = 0;
        }
    } else {
        frame.fpsOutOfRange = 0;
    }
    Battle3d_ServiceOffscreenSlots();
    ResetFrameDrawPool(ctx);
    ResetRenderPools(ctx);
    Gfx_BeginSortedDrawFrame(ctx);
    Gfx_ResetNodeList(ctx);
    if (g_BattleIsPaused == 0) {
        Battle_ResetSceneFramePools();
    }
    frame.color.bits[2] = 0;
    frame.color.bits[1] = 0;
    frame.color.bits[0] = 0;
    frame.color.bits[3] = FLOAT_ONE_BITS;
    GfxSetViewportRect(g_BattleViewportOriginX, g_BattleViewportOriginY, g_BattleViewportWidth, g_BattleViewportHeight, ctx);
    GfxSetBackgroundColorCall(frame.color.rgba, ctx);
    GfxClearCall(1, 1, 1, ctx);
    frame.scenePass = 0;
    if (((int (__cdecl *)(int scene, void *context))Gfx_BeginSceneCall)(frame.scenePass, (void *)ctx)) { /* reads eax after a void call, as the original does */
        SetProjectionOffset(g_BattleProjectionOffsetX, g_BattleProjectionOffsetY);
        ctx->viewportHeightOverride = g_BattleViewportHeight;
        GfxSetViewportRect(g_BattleViewportOriginX, g_BattleViewportOriginY, g_BattleViewportWidth, g_BattleSceneViewportHeight, ctx);
        if (frame.fpsOutOfRange != 0) {
            frame.didCatchUpStep = 0;
            if (g_BattleIsPaused != 0) {
                g_BattleIsRunning = 0;
            } else {
                g_BattleIsRunning = 1;
            }
            if (g_BattleFrameLimiterFlag != 0) {
                Gfx_SetDrawEnabled(0);
            } else {
                Gfx_SetDrawEnabled(1);
            }
            Battle_RunPhaseMachine(ctx);
            if (g_BattleFrameLimiterFlag != 0) {
                Battle_FrameLimiterWait(ctx);
                g_BattleFrameLimiterFlag = 0;
                Gfx_SetDrawEnabled(1);
                ctx->framesThisSecond += 1.0;
                ReadTimestampCounter(&g_BattleFrameLimiterLastTimestamp);
                Battle_RunPhaseMachine(ctx);
                frame.didCatchUpStep = 1;
            }
        } else {
            if (g_BattleIsPaused != 0) {
                g_BattleIsRunning = 0;
            } else {
                g_BattleIsRunning = 1;
            }
            g_BattleUnreadFrameWordA = 0;
            g_BattleUnreadFrameWordB = 0;
            Gfx_SetDrawEnabled(1);
            Battle_RunPhaseMachine(ctx);
        }
        Battle_DrawLayer(0, 3);
        Battle_DrawLayer(0, 0);
        Gfx_SetScenePassCall(1, (struct GfxEngineContext *)ctx);
        GfxFlushFrameDrawList(ctx);
        Battle3d_SweepActorDrawTable(1);
        Battle_DrawHudSprites(1);
        Gfx_EndSortedDrawFrame(ctx);
        Battle_DrawLayer(0, 1);
        Battle_DrawLayer(g_BattleIsRunning, 2);
        if (g_BattleIsRunning != 0) {
            Battle3d_UpdateCameraNode();
        }
        GfxSetViewportRect(g_BattleViewportOriginX, g_BattleViewportOriginY, g_BattleViewportWidth, g_BattleViewportHeight, ctx);
        if (g_MenuEntryState == MENU_ENTRY_BATTLE_UI) {
            Menu_SetTextSubsystemReady(0);
            frame.uiPassCount = 4;
            for (frame.uiPass = 0; frame.uiPass < frame.uiPassCount; frame.uiPass++) {
                if (frame.uiPass == frame.uiPassCount - 1) {
                    Menu_SetTextSubsystemReady(1);
                }
                Battle_UpdateUiAndLogic();
                if (frame.uiPass == frame.uiPassCount - 1) {
                    Battle_DrawUiLists();
                }
            }
        }
        if (g_BattleDebugToggle != 0) {
            Battle_FrameLimiterWait(ctx);
        }
        Gfx_EndSceneCall(ctx);
    }
    if (g_BattleDebugKeyState != 0) {
        if ((*(ComDirectDrawSurface **)&ctx->backSurface)->GetDC(&frame.debugDc) == 0) {
            SetBkColor(frame.debugDc, 0xff0000);
            SetTextColor(frame.debugDc, 0xffff);
            sprintf(frame.debugText,
                    // STRING: FF7 0x007c0cc4
                    "Scene: %d", g_BattleSceneNumber);
            TextOutA(frame.debugDc, 100, 110, frame.debugText, lstrlenA(frame.debugText));
            sprintf(frame.debugText,
                    // STRING: FF7 0x007c0cd0
                    "FPS: %.2f", ctx->fps);
            TextOutA(frame.debugDc, 0, 110, frame.debugText, lstrlenA(frame.debugText));
            if (g_BattleDebugToggle != 0) {
                sprintf(frame.debugText,
                        // STRING: FF7 0x007c0cdc
                        "LOCKED");
                TextOutA(frame.debugDc, 0, 130, frame.debugText, lstrlenA(frame.debugText));
                sprintf(frame.debugText,
                        // STRING: FF7 0x007c0ce4
                        "Min: %.2f", g_BattleFrameMinFps);
                TextOutA(frame.debugDc, 0, 150, frame.debugText, lstrlenA(frame.debugText));
                sprintf(frame.debugText,
                        // STRING: FF7 0x007c0cf0
                        "Max: %.2f", g_BattleFrameMaxFps);
                TextOutA(frame.debugDc, 0, 170, frame.debugText, lstrlenA(frame.debugText));
            }
            if (frame.didCatchUpStep != 0) {
                sprintf(frame.debugText,
                        // STRING: FF7 0x007c0cfc
                        "Skip");
                TextOutA(frame.debugDc, 100, 130, frame.debugText, lstrlenA(frame.debugText));
            }
            if (g_BattleDebugKeyState & 2) {
                Gfx_StatsDrawTextRows(0, 0, frame.debugDc, App_DrawDebugTextGdi, ctx->statsRecord);
            }
            Gfx_StatsClearRecord(ctx->statsRecord);
            (*(ComDirectDrawSurface **)&ctx->backSurface)->ReleaseDC(frame.debugDc);
        }
    }
    SoundUpdateFades();
}

// Runs the battle phase switch (g_BattlePhase, enum BattlePhase) repeatedly until a phase yields, with deliberate fallthrough across
// initialization, scene loading, opening/round sequences, action execution and result processing.
// Selects first/next-round callbacks, clears requested party limits, snapshots actor masks and
// posts initial events. Main phases run action queues, sequences, status-transition loops,
// event/drop cleanup, HP/MP snapshots and party updates, setting end flags from surviving
// masks/countdown. Arena subphases process round breaks and choose a next scene or final exit. End
// subphases wait for sequence/result controls, update two-byte outcome counts and field/world
// return flags, then request dispatcher callbacks. Most callback/completion phases yield;
// unsupported phase or subphase values can leave the outer loop running without progress.
// FUNCTION: FF7 0x0041c0bb
static __inline void __cdecl Battle_RunPhaseMachine(struct GfxEngineContext *ctx)
{
    struct {
        GameModule module;
        int mask;
        int result;
        int partyIndex;
        int actorIndex;
        int done;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.done = 0;
    while (frame.done == 0) {
        switch (g_BattlePhase) {
        case BATTLE_PHASE_INIT:
            g_BattleResultWord = 0;
            if (g_BattleSessionEntryCount++) {
                g_BattleRoundSequenceHandler = Battle_RunNextRoundSequence;
                g_BattlePlaySequenceHandler = Battle_PlaySequence;
                g_BattleLoadSceneHandler = Battle_LoadSceneNextRound;
            } else {
                g_BattleRoundSequenceHandler = Battle_RunOpeningSequence;
                g_BattlePlaySequenceHandler = 0;
                g_BattleLoadSceneHandler = Battle_LoadSceneFirstRound;
            }
            g_BattlePhase = BATTLE_PHASE_FINISH_SEQUENCE;
        case BATTLE_PHASE_FINISH_SEQUENCE:
            if (g_BattlePlaySequenceHandler != 0) {
                g_BattlePlaySequenceHandler();
                if (g_BattleSequencePlayDone != 0) {
                    g_BattleSequencePlayDone = 0;
                    g_BattlePhase = BATTLE_PHASE_LOAD_SCENE;
                }
                frame.done = 1;
                break;
            }
            g_BattlePhase = BATTLE_PHASE_LOAD_SCENE;
        case BATTLE_PHASE_LOAD_SCENE:
            g_BattleLoadSceneHandler(g_BattleSceneNumber);
            if (g_BattleSceneLoadDone != 0) {
                g_BattleSceneLoadDone = 0;
                g_BattlePhase = BATTLE_PHASE_INIT_STATUS;
                if ((gBattleAiGlobals.arenaFlags & BATTLE_ARENA_FIXED_ATB) != 0) {
                    for (frame.partyIndex = 0; frame.partyIndex < 3; frame.partyIndex++) {
                        g_BattleState.partyStats[frame.partyIndex].loadLimitGauge = 0;
                        g_BattleState.partyStats[frame.partyIndex].limitGauge = 0;
                        Battle_ClearLimitReady(frame.partyIndex);
                        g_MenuPartyStats[frame.partyIndex].limitGauge = 0;
                        g_BattleScene.readyActorMask &= ~(1 << frame.partyIndex);
                    }
                }
            } else {
                frame.done = 1;
                break;
            }
        case BATTLE_PHASE_INIT_STATUS:
            gBattleAiGlobals.arenaFlags &= 0xFFFD;
            for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
                g_BattleScene.actors[frame.actorIndex].savedStatus = gBattleAiGlobals.combatants[frame.actorIndex].status & 0x0FFFFFFF;
                g_BattleScene.actors[frame.actorIndex].statusFlags = g_BattleScene.actors[frame.actorIndex].savedStatus;
                g_BattleState.actors[frame.actorIndex].dirtyFlags |= 15;
            }
            g_BattlePhase = BATTLE_PHASE_OPENING;
        case BATTLE_PHASE_OPENING:
            g_BattleRoundSequenceHandler();
            if (g_BattleRoundSequenceDone != 0) {
                g_BattleRoundSequenceDone = 0;
                g_BattlePhase = BATTLE_PHASE_START;
            }
            frame.done = 1;
            break;
        case BATTLE_PHASE_START:
            for (frame.actorIndex = 4; frame.actorIndex < 10; frame.actorIndex++) {
                if (gBattleAiGlobals.combatants[frame.actorIndex].idlePose != 0xFF) {
                    Battle_GetSetActorIdlePose(frame.actorIndex, gBattleAiGlobals.combatants[frame.actorIndex].idlePose);
                }
                g_BattleState.actors[frame.actorIndex].sceneState = *(char *)&g_BattleScene.actors[frame.actorIndex].state;
            }
            for (frame.actorIndex = 0; frame.actorIndex < 3; frame.actorIndex++) {
                if (g_BattleState.actors[frame.actorIndex].atbTick != 0) {
                    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, frame.actorIndex, BATTLE_ATB_EVENT_REOPEN_COMMAND_MENU, 0);
                }
            }
            g_BattleActionQueueResult = 0;
            gBattleAiGlobals.flags = 0;
            g_BattleSequenceBuffer.phaseSavedTargetMask = g_BattleScene.targetMask;
            g_BattlePhase = BATTLE_PHASE_WAIT_OPENING;
        case BATTLE_PHASE_WAIT_OPENING:
            Battle_WaitSequenceDone();
            if (g_BattleSequenceWaitDone != 0) {
                g_BattleSequenceWaitDone = 0;
                g_BattlePhase = BATTLE_PHASE_ACTION_QUEUE;
                // STRING: FF7 0x007c0ba0
                Gfx_DebugPrint("[BATTLE] Begin main battle loop\n");
                if (g_BattleSequenceWaitPlayed != 0) {
                    g_BattleSequenceWaitPlayed = 0;
                    frame.done = 1;
                    break;
                }
            } else {
                frame.done = 1;
                break;
            }
        case BATTLE_PHASE_ACTION_QUEUE:
            if (gBattleAiGlobals.flags == 0 || g_BattleActionQueueResult == 0) {
                g_BattleActionQueueMode = (gBattleAiGlobals.flags & (BATTLE_OUTCOME_OVER | BATTLE_OUTCOME_FLED | 0x01)) ? 1 : 7;
                g_BattleActionQueueResult = Battle_RunActionQueue(g_BattleActionQueueMode);
                if (g_BattleSweepComplete != 0) {
                    g_BattleSweepComplete = 0;
                    g_BattleState.selectedTargetIndex = -1;
                    for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
                        g_BattleScene.actors[frame.actorIndex].savedStatus = gBattleAiGlobals.combatants[frame.actorIndex].status & 0x0FFFFFFF;
                    }
                    g_BattlePhase = BATTLE_PHASE_PLAY_ACTION;
                } else {
                    Battle_RunSequenceStep();
                    frame.done = 1;
                    break;
                }
            } else {
                g_BattlePhase = BATTLE_PHASE_END;
                break;
            }
        case BATTLE_PHASE_PLAY_ACTION:
            Battle_PlaySequence();
            if (g_BattleSequencePlayDone != 0) {
                g_BattleSequencePlayDone = 0;
                g_BattlePhase = BATTLE_PHASE_APPLY_ACTION;
            }
            frame.done = 1;
            break;
        case BATTLE_PHASE_APPLY_ACTION:
            Battle_ApplyTimeSlices();
            Battle_FlushEventQueue(BATTLE_EVENT_QUEUE_ACTION);
            while (Battle_DispatchStatusTransitions()) {
                // STRING: FF7 0x007c0bc4
                Gfx_DebugPrint("[BATTLE] in while-loop!\n");
            }
            Battle_ReleasePendingDrops(10);
            for (frame.actorIndex = 0; frame.actorIndex < 3; frame.actorIndex++) {
                g_BattleState.partyStats[frame.actorIndex].loadLimitGauge = g_BattleState.partyStats[frame.actorIndex].limitGauge;
            }
            for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
                g_BattleScene.actors[frame.actorIndex].savedStatus = gBattleAiGlobals.combatants[frame.actorIndex].status & 0x0FFFFFFF;
                g_BattleScene.actors[frame.actorIndex].statusFlags = g_BattleScene.actors[frame.actorIndex].savedStatus;
            }
            g_BattlePhase = BATTLE_PHASE_WAIT_ACTION;
        case BATTLE_PHASE_WAIT_ACTION:
            Battle_WaitSequenceDone();
            if (g_BattleSequenceWaitDone != 0) {
                g_BattleSequenceWaitDone = 0;
                g_BattlePhase = BATTLE_PHASE_CHECK_END;
                if (g_BattleSequenceWaitPlayed != 0) {
                    g_BattleSequenceWaitPlayed = 0;
                    frame.done = 1;
                    break;
                }
            } else {
                frame.done = 1;
                break;
            }
        case BATTLE_PHASE_CHECK_END:
            Battle_FlushEventQueue(BATTLE_EVENT_QUEUE_ACTION);
            Battle_RecomputeActorMasks();
            Battle_SnapshotHpMp();
            if (g_BattleState.selectedTargetIndex != -1) {
                g_BattleActionQueueResult = 0;
            }
            for (frame.actorIndex = 0; frame.actorIndex < 3; frame.actorIndex++) {
                if (*(unsigned short *)&g_BattleState.partyStats[frame.actorIndex].mp != gBattleAiGlobals.combatants[frame.actorIndex].curMp) {
                    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, frame.actorIndex, BATTLE_ATB_EVENT_SET_DIRTY_FLAGS, 15);
                }
                g_BattleState.partyStats[frame.actorIndex].mp = gBattleAiGlobals.combatants[frame.actorIndex].curMp;
            }
            gBattleAiGlobals.flags &= (unsigned short)~(BATTLE_OUTCOME_OVER | BATTLE_OUTCOME_DEFEAT);
            if ((g_BattleScene.targetMask & ~g_BattleState.absentActorMask & BATTLE_ACTOR_MASK_ALLIES) == 0) {
                gBattleAiGlobals.flags |= BATTLE_OUTCOME_OVER | BATTLE_OUTCOME_DEFEAT;
            } else if ((g_BattleScene.targetMask & ~g_BattleState.absentActorMask & BATTLE_ACTOR_MASK_ENEMIES) == 0) {
                gBattleAiGlobals.flags |= BATTLE_OUTCOME_OVER;
            }
            if ((g_BattleScene.flags & FIELD_BATTLE_FLAG_COUNTDOWN) != 0 && gSavemap.countdownSeconds == 0) {
                gBattleAiGlobals.flags |= BATTLE_OUTCOME_OVER | BATTLE_OUTCOME_TIMED_OUT;
                g_BattleResultWord |= FIELD_BATTLE_FLAG_COUNTDOWN;
                g_BattleScene.setupFlags &= 0xFFF7;
            }
            g_BattlePhase = BATTLE_PHASE_ARENA_ROUND;
        case BATTLE_PHASE_ARENA_ROUND:
            switch (g_BattleArenaRoundStep) {
            case BATTLE_ARENA_ROUND_STEP_CHECK:
                if ((gBattleAiGlobals.arenaFlags & BATTLE_ARENA_KEEP_ROUND_HP) != 0 && g_BattleState.actionEnqueueSequence[0] == g_BattleState.actionRunSequence[0]) {
                    frame.mask = g_BattleScene.targetMask & g_BattleSequenceBuffer.phaseSavedTargetMask & BATTLE_ACTOR_MASK_ENEMIES;
                    if ((g_BattleSequenceBuffer.phaseSavedTargetMask & BATTLE_ACTOR_MASK_ENEMIES) != frame.mask && frame.mask != 0) {
                        g_BattleArenaRoundStep = BATTLE_ARENA_ROUND_STEP_BREAK;
                        break;
                    }
                    g_BattleSequenceBuffer.phaseSavedTargetMask = g_BattleScene.targetMask;
                }
                g_BattlePhase = BATTLE_PHASE_NEXT_TURN;
                break;
            case BATTLE_ARENA_ROUND_STEP_BREAK:
                frame.result = Battle_RunArenaRoundBreak();
                if (g_BattleArenaRoundBreakDone != 0) {
                    g_BattleArenaRoundBreakDone = 0;
                    g_BattleArenaRoundStep = BATTLE_ARENA_ROUND_STEP_CHECK;
                    if (frame.result != 0) {
                        g_BattlePhase = BATTLE_PHASE_END;
                        break;
                    } else {
                        g_BattleSequenceBuffer.phaseSavedTargetMask = g_BattleScene.targetMask;
                        g_BattlePhase = BATTLE_PHASE_NEXT_TURN;
                    }
                }
                frame.done = 1;
            }
            break;
        case BATTLE_PHASE_NEXT_TURN:
            if ((gBattleAiGlobals.flags & BATTLE_OUTCOME_ESCAPED) != 0) {
                g_BattlePhase = BATTLE_PHASE_END;
            } else {
                g_BattlePhase = BATTLE_PHASE_ACTION_QUEUE;
                break;
            }
        case BATTLE_PHASE_END:
            // STRING: FF7 0x007c0be0
            Gfx_DebugPrint("[BATTLE] End of current battle\n");
            if ((g_BattleScene.flags & FIELD_BATTLE_FLAG_BATTLE_SQUARE) != 0 && (gBattleAiGlobals.flags & BATTLE_OUTCOME_FLED) != 0) {
                gBattleAiGlobals.flags &= (unsigned short)~BATTLE_OUTCOME_FLED;
                gBattleAiGlobals.flags |= BATTLE_OUTCOME_DEFEAT;
            }
            g_BattlePhase = BATTLE_PHASE_NEXT_BATTLE;
        case BATTLE_PHASE_NEXT_BATTLE:
            switch (g_BattleNextBattleStep) {
            case BATTLE_NEXT_BATTLE_STEP_CHECK_OUTCOME:
                g_BattleSceneNumber = 0xFFFF;
                if ((gBattleAiGlobals.flags & (BATTLE_OUTCOME_DEFEAT | BATTLE_OUTCOME_FLED | BATTLE_OUTCOME_ESCAPED | BATTLE_OUTCOME_TIMED_OUT)) != 0) {
                    g_BattlePhase = BATTLE_PHASE_EXIT;
                    break;
                }
                g_BattleNextBattleStep = BATTLE_NEXT_BATTLE_STEP_NEXT_SCENE;
            case BATTLE_NEXT_BATTLE_STEP_NEXT_SCENE:
                g_BattleSceneNumber = g_BattleScene.nextScene;
                if ((g_BattleScene.flags & FIELD_BATTLE_FLAG_BATTLE_SQUARE) != 0) {
                    g_BattleNextBattleStep = BATTLE_NEXT_BATTLE_STEP_ARENA_CHOOSER;
                } else {
                    g_BattleNextBattleStep = BATTLE_NEXT_BATTLE_STEP_START;
                    break;
                }
            case BATTLE_NEXT_BATTLE_STEP_ARENA_CHOOSER:
                g_BattleSceneNumber = Battle_RunArenaNextBattleChooser();
                if (g_BattleArenaChooserDone != 0) {
                    g_BattleArenaChooserDone = 0;
                    g_BattleNextBattleStep = BATTLE_NEXT_BATTLE_STEP_START;
                }
                if (g_BattleArenaChooserPending != 0) {
                    g_BattleArenaChooserPending = 0;
                    frame.done = 1;
                }
                break;
            case BATTLE_NEXT_BATTLE_STEP_START:
                if (g_BattleSceneNumber == 0xFFFF) {
                    g_BattlePhase = BATTLE_PHASE_EXIT;
                } else {
                    g_BattlePhase = BATTLE_PHASE_INIT;
                    // STRING: FF7 0x007c0c00
                    Gfx_DebugPrint("[BATTLE] next battle\n");
                }
                g_BattleNextBattleStep = BATTLE_NEXT_BATTLE_STEP_CHECK_OUTCOME;
            }
            break;
        case BATTLE_PHASE_EXIT:
            switch (g_BattleExitStep) {
            case BATTLE_EXIT_STEP_PLAY_SEQUENCE:
                // STRING: FF7 0x007c0c18
                Gfx_DebugPrint("[BATTLE] Begin end battle stuff\n");
                Battle_PlaySequence();
                if (g_BattleSequencePlayDone != 0) {
                    g_BattleSequencePlayDone = 0;
                    g_BattleExitStep = BATTLE_EXIT_STEP_END_SEQUENCE;
                }
                frame.done = 1;
                break;
            case BATTLE_EXIT_STEP_END_SEQUENCE:
                Battle_RunEndSequence();
                if (g_BattleEndSequenceDone != 0) {
                    g_BattleEndSequenceDone = 0;
                    g_BattleExitStep = BATTLE_EXIT_STEP_PLAY_SEQUENCE;
                    frame.done = 1;
                    if (g_EnginePreviousMode == GAME_MODE_FIELD) {
                        if ((g_BattleResultWord & FIELD_BATTLE_FLAG_ESCAPED) != 0) {
                            gSavemap.escapesLo++;
                            if (gSavemap.escapesLo == 0) {
                                gSavemap.escapesHi++;
                            }
                        }
                        if ((g_BattleResultWord & FIELD_BATTLE_FLAG_LOST) == FIELD_BATTLE_FLAG_LOST && *(char *)&g_FieldActorState.state.battleMode1 == 0) {
                            g_FieldActorState.state.requestCode = FIELD_REQUEST_GAME_OVER;
                            g_BattleResultWord = 0;
                        }
                        if (g_MenuEntryState == MENU_ENTRY_BATTLE_RESULT) {
                            g_EngineGameMode = GAME_MODE_MENU_AFTER_BATTLE;
                            g_BattleResultMenuNextMode = GAME_MODE_FIELD;
                        } else {
                            g_EngineGameMode = GAME_MODE_FIELD;
                        }
                        g_EnginePreviousMode = GAME_MODE_BATTLE;
                    } else {
                        if ((g_BattleResultWord & FIELD_BATTLE_FLAG_ESCAPED) != 0) {
                            gSavemap.escapesLo++;
                            if (gSavemap.escapesLo == 0) {
                                gSavemap.escapesHi++;
                            }
                            g_EngineGameMode = GAME_MODE_WORLD_MAP;
                        } else if ((g_BattleResultWord & FIELD_BATTLE_FLAG_LOST) == FIELD_BATTLE_FLAG_LOST || (gSavemap.countdownSeconds == 0 && (g_WorldBattleRequest & WORLD_BATTLE_REQ_COUNTDOWN) != 0)) {
                            g_BattleResultWord = 0;
                            g_FieldActorState.state.requestCode = FIELD_REQUEST_GAME_OVER;
                            g_EngineGameMode = GAME_MODE_FIELD;
                        } else {
                            if ((g_WorldBattleRequest & WORLD_BATTLE_REQ_ENTER_FIELD) != 0) {
                                g_BattleResultMenuNextMode = GAME_MODE_FIELD;
                            } else {
                                g_BattleResultMenuNextMode = GAME_MODE_WORLD_MAP;
                            }
                            if (g_MenuEntryState == MENU_ENTRY_BATTLE_RESULT) {
                                g_EngineGameMode = GAME_MODE_MENU_AFTER_BATTLE;
                            } else {
                                g_EngineGameMode = GAME_MODE_FIELD;
                            }
                        }
                        g_EnginePreviousMode = GAME_MODE_BATTLE;
                    }
                    frame.module.enter = GameModeDispatcher_Enter;
                    frame.module.exit = GameModeDispatcher_Exit;
                    frame.module.frame = GameModeDispatcher_Frame;
                    frame.module.onMouse = App_OnMouseMessage;
                    frame.module.onKey = App_OnKeyMessage;
                    RequestModuleSwitch(&frame.module, ctx);
                }
                if (g_BattleSequenceBuffer.phasePendingFlag != 0) {
                    g_BattleSequenceBuffer.phasePendingFlag = 0;
                    frame.done = 1;
                }
            }
        }
    }
}
#pragma intrinsic(memset)

// Zeroes the six battle work blocks (the scene's first 0x178 bytes, the sequence buffer, the hit
// records, the damage display records, gBattleAiGlobals and the battle state's actors) and three
// shorts, then resets the command ring.
// FUNCTION: FF7 0x0041ccb2
void __cdecl Battle_ClearWorkRam(void)
{
    memset(&g_BattleScene, 0, sizeof(g_BattleScene));
    memset(&g_BattleSequenceBuffer.steps[0], 0, 0x300);
    memset(g_BattleEffectOrderTable, 0, 0x600);
    memset(g_BattleDamageDisplayRecords, 0, 0x700);
    g_BattleAtbFrozen = 0;
    g_BattleMenuGraphicsPathBuffer.victoryMusicPending = 0;
    g_BattleDefeatMusicPending = 0;
    memset(&gBattleAiGlobals, 0, 0x94C);
    memset(&g_BattleState.actors[0], 0, 0x222C);
    Battle_ResetCommandRing();
}

// On key-down F11 cycles the battle debug mode through zero, one and two. Character brackets
// decrement/increment minimum FPS within zero and maximum; braces decrement/increment maximum
// within minimum and sixty. Character l toggles the frame-lock flag. Character key is narrowed to
// unsigned byte; other messages/keys do nothing.
// FUNCTION: FF7 0x0041cd2d
void __cdecl Battle_HandleDebugKeyMessage(int message, int key)
{
    int keyCode;
    unsigned char charCode;
    switch (message) {
    case WM_KEYDOWN:
        keyCode = key;
        switch (keyCode) {
        case VK_F11:
            g_BattleDebugKeyState++;
            if (g_BattleDebugKeyState >= 3)
                g_BattleDebugKeyState = 0;
        }
        break;
    case WM_CHAR:
        charCode = (unsigned char)key;
        switch (charCode) {
        case '[':
            g_BattleFrameMinFps -= 1.0;
            if (g_BattleFrameMinFps <= 0.0)
                g_BattleFrameMinFps = 0.0;
            break;
        case ']':
            g_BattleFrameMinFps += 1.0;
            if (g_BattleFrameMinFps >= g_BattleFrameMaxFps)
                g_BattleFrameMinFps = g_BattleFrameMaxFps;
            break;
        case '{':
            g_BattleFrameMaxFps -= 1.0;
            if (g_BattleFrameMaxFps <= g_BattleFrameMinFps)
                g_BattleFrameMaxFps = g_BattleFrameMinFps;
            break;
        case '}':
            g_BattleFrameMaxFps += 1.0;
            if (g_BattleFrameMaxFps >= 60.0)
                g_BattleFrameMaxFps = 60.0;
            break;
        case 'l':
            g_BattleDebugToggle ^= 1;
            break;
        }
        break;
    }
}

// Empty. A battle hook the shipped build left with no body.
// FUNCTION: FF7 0x0041cf04
void __cdecl Battle_EmptyHook(void)
{
}
} /* extern "C" */

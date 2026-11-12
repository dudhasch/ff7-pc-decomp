// Contents: The highway minigame's main module: starting a new run, storing the scores on exit, the
//   per-frame update, drawing and event timeline, and the camera and scene updates. It also holds
//   the sound-cue helpers, a small file loader and the status message formatter.
// Original contribution 0x006504c0..0x006520c0 of the highway minigame.
//
// VC5 emits .text with 16-byte alignment, so every input contribution starts on a 16-byte boundary.
// $CRT_CPP_Initializer (0x006504c0) is the only aligned member start between the preceding CC run
// and the four CC after Mini_HighwayFormatAndDispatchMessage (0x00652073), so these 29 functions
// are one object. Its native unwind code (Mini_HighwayInit) and member calls make it C++ with /GX
// /Ob1. The section below reproduces the contribution, so LINK supplies the trailing CC.
#include "lib/types/mem.h"
#include "ff7/types/highway.h"
#include "ff7/enums/highway.h"
#include <stdarg.h>
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/highway_psxdata.h"
#include "ff7/api/highway.h"
#include "ff7/api/snobo.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "ff7/enums/main.h"
#include "ff7/savemap.h"
#include <stdio.h>
#include "ff7/api/main.h"
#include "lib/enums/file.h"

// clang-format off
// clang-format on
extern "C" {
// clang-format off
// GLOBAL: FF7 0x009092b0
int g_HighwaySfxIdTable[10] = {0x12d, 0x5, 0x12, 0x91, 0x138, 0xb3, 0x12b, 0x126, 0x241, 0x12e};
// The byte the highway TUs test for the LGP archive, the frame-time float, the sound command block pointer and the music flag.
// GLOBAL: FF7 0x009092d8
unsigned char g_HighwayUseLgpArchive = 1;
// GLOBAL: FF7 0x009092dc
float g_HighwayFrameDeltaTime = 1.0f;
// GLOBAL: FF7 0x009092e0
struct HighwaySoundCommand *g_pHighwaySoundCommandBuffer = (struct HighwaySoundCommand *)&g_HighwaySoundCommandBuffer;
// GLOBAL: FF7 0x009092e4
int g_HighwayMusicActive = 1;
// GLOBAL: FF7 0x00d858fc
int g_HighwayFalloffDistanceMax = 0;
// GLOBAL: FF7 0x00d85900
short g_HighwayDefaultVolumeStep = 0;
// GLOBAL: FF7 0x00d85904
int g_HighwaySlot0Volume = 0;
// GLOBAL: FF7 0x00d85908
int g_HighwayDefaultNearParam = 0;
// GLOBAL: FF7 0x00d8590c
short g_HighwayDefaultPitchParam = 0;
// GLOBAL: FF7 0x00d85910
int g_HighwaySoundParam = 0;
unsigned char g_bssPad_00d85914[4] = {0};
// GLOBAL: FF7 0x00d85918
struct HighwaySoundCommand g_HighwaySoundCommandBuffer = {0};
unsigned char g_bssPad_00d85924[4] = {0};
unsigned char g_bssPad_00d85928[20] = {0};
// GLOBAL: FF7 0x00d8593c
int g_HighwaySlotCountdown = 0;
// GLOBAL: FF7 0x00d85940
int g_HighwayNearestRivalVolume = 0;
unsigned char g_bssPad_00d85944[4] = {0};
// GLOBAL: FF7 0x00d85948
double g_HighwayTimeFrequencyReciprocal = 0.0;
// GLOBAL: FF7 0x00d85950
int g_HighwayDefaultOffsetParam = 0;
// GLOBAL: FF7 0x00d85954
int g_HighwayDefaultFarParam = 0;
// GLOBAL: FF7 0x00d85958
int g_HighwayFalloffDistanceMin = 0;
unsigned char g_bssPad_00d8595c[4] = {0};
// GLOBAL: FF7 0x00d85960
unsigned char g_HighwayTimestamp[4] = {0};
// GLOBAL: FF7 0x00d85964
int g_HighwayTimestampHigh = 0;
// GLOBAL: FF7 0x00d85968
unsigned char g_HighwayAssetSourceMode = 0;
// GLOBAL: FF7 0x00d8596c
int g_HighwayGameMode = 0;
// GLOBAL: FF7 0x00d85970
int g_HighwayControlFlags = 0;
// GLOBAL: FF7 0x00d85974
unsigned char g_HighwayRunningFlag = 0;
// GLOBAL: FF7 0x00d85978
int g_HighwayActorRenderOffset = 0;
// GLOBAL: FF7 0x00d8597c
int g_HighwayScrollSubpixel = 0;
// GLOBAL: FF7 0x00d85980
int g_HighwayScrollSpeed = 0;
// GLOBAL: FF7 0x00d85984
int g_HighwayCameraZPosition = 0;
// GLOBAL: FF7 0x00d85988
int g_HighwayTrackProgress = 0;
unsigned char g_bssPad_00d8598c[4] = {0};
// GLOBAL: FF7 0x00d85990
int g_HighwayCurrentScore = 0;
// GLOBAL: FF7 0x00d85994
int g_HighwayHighScore = 0;
// GLOBAL: FF7 0x00d85998
struct HighwayTrackEvent *g_pHighwayTrackEventTable = 0;
// GLOBAL: FF7 0x00d8599c
int g_HighwayTrackEventIndex = 0;
// GLOBAL: FF7 0x00d859a0
unsigned char g_HighwayExitRequested = 0;
// GLOBAL: FF7 0x00d859a4
char g_HighwayRunClearedFlag = 0;
// GLOBAL: FF7 0x00d859a8
unsigned char g_HighwaySpawningStopped = 0;
// GLOBAL: FF7 0x00d859ac
int g_HighwayEndingAStarted = 0;
// GLOBAL: FF7 0x00d859b0
int g_HighwayEndingBStarted = 0;
// GLOBAL: FF7 0x00d859b4
int g_HighwaySceneStep = 0;
// GLOBAL: FF7 0x00d859b8
int g_HighwayRunPhase = 0;
// clang-format on

void __cdecl Mini_ResetPrimBuffer(int arg1);
}

#pragma code_seg(".text")
extern "C" {

// The object's file-scope data set starts its code: .CRT$XCU slot 0x007ba030 ->
// $CRT_CPP_Initializer (0x006504c0), which runs the inline constructor $E1 (0x006504cf) and
// registers at-exit thunk 0x006504fa through 0x006504e8; the thunk calls ~HighwayPsxDataSet20
// (HighwayPsxDataSet20::~HighwayPsxDataSet20, 0x00652605). The compiler-generated $E functions
// carry no FUNCTION markers.
// GLOBAL: FF7 0x00d858e8
HighwayPsxDataSet20 g_HighwayPsxDataSet;
/*
 * Resets run-time state, scoring, distances, and object pointers for a new highway minigame run.
 */
// FUNCTION: FF7 0x00650509
void __cdecl Mini_HighwayResetRun(void)
{
    int idx;
    int eventAddress;
    int tableOffset;
    int altOffset;
    int altEventTable;

    g_HighwayGameMode = gSavemap.highwayGameMode;
    g_HighwayCurrentScore = 0;
    g_HighwayHighScore = 0;
    g_HighwayHighScore += gSavemap.highScoreHigh << 8;
    g_HighwayHighScore += gSavemap.highScoreLow;
    g_HighwayFalloffDistanceMin = 0xBB8;
    g_HighwayFalloffDistanceMax = 0x1770;
    g_HighwayDefaultNearParam = 0x41A0;
    g_HighwayDefaultFarParam = 0x4F4C;
    g_HighwayScrollSpeed = 0x7A;
    g_HighwayExitRequested = 0;
    g_HighwayRunClearedFlag = 0;
    g_HighwaySpawningStopped = 0;
    g_HighwayCameraZPosition = 0;
    g_HighwayTrackProgress = 0;
    g_HighwayScrollSubpixel = 0;
    g_HighwayDefaultPitchParam = 0;
    g_HighwayDefaultVolumeStep = 0x80;
    g_HighwaySoundParam = 0;
    g_HighwayDefaultOffsetParam = 0;
    g_HighwayNearestRivalVolume = 0;
    g_HighwaySlot0Volume = 0;
    g_HighwayCountdown = 0;
    g_HighwaySlotCountdown = 0;
    g_HighwayHpBarTable.bars[0].total = 0xFFFF;
    g_HighwayHpBarTable.bars[2].total = 0xFFFF;
    g_HighwayHpBarTable.bars[4].total = 0xFFFF;
    g_HighwayHpBarTable.bars[1].total = 0xFFFF;
    g_HighwayHpBarTable.bars[3].total = 0xFFFF;
    for (idx = 0; idx < 5; idx++) {
        g_HighwayHpBarTable.bars[idx].damageTaken = 0;
        g_HighwayHpBarTable.bars[idx].clearedState = 0;
        g_HighwayHpBarTable.bars[idx].flashTimer = 0;
        g_HighwayHpBarTable.bars[idx].remaining = *(short *)&g_HighwayHpBarTable.bars[idx].total;
        g_HighwayHpBarTable.bars[idx].iconState = 1;
    }
    g_HighwayEndingAStarted = 0;
    g_HighwayEndingBStarted = 0;
    g_HighwayRunPhase = 1;
    if ((g_HighwayControlFlags & HIGHWAY_CTRL_DEBUG_CAM_MODE6_FREEZE_SCROLL) != 0) {
        g_HighwayScrollSpeed = 0;
    }
    if (g_HighwayGameMode == 0) {
        tableOffset = *(int *)((char *)g_HighwayPsxDataSet.adrOffsets + (UNFOLDED(0x11) << 2));
        if (tableOffset >= g_HighwayPsxDataSet.binSize) {
            eventAddress = 0;
        } else {
            eventAddress = g_HighwayPsxDataSet.binData + tableOffset;
        }
        g_pHighwayTrackEventTable = (struct HighwayTrackEvent *)(eventAddress);
    } else {
        altOffset = *(int *)((char *)g_HighwayPsxDataSet.adrOffsets + (UNFOLDED(0x1F) << 2));
        if (altOffset >= g_HighwayPsxDataSet.binSize) {
            altEventTable = 0;
        } else {
            altEventTable = g_HighwayPsxDataSet.binData + altOffset;
        }
        g_pHighwayTrackEventTable = (struct HighwayTrackEvent *)(altEventTable);
    }
    g_HighwayTrackEventIndex = 0;
}
}
/*
 * Loads the motorcycle chase; called by `Mini_HighwayEnter`. Returns 1 when everything loaded and
 * 0 on the first failure, which leaves g_HighwayRunningFlag at 0 so that the first frame returns
 * straight to the field; the caller ignores the value. Steps:
 * - sets matrix layout mode 1 and the asset source mode, and records the minigame directory;
 * - when g_HighwayUseLgpArchive is set, opens high-us.lgp as LGP slot 8 (fails if it cannot);
 * - loads aaaa.adr and baaa.bin into g_HighwayPsxDataSet (fails if it cannot);
 * - creates the file block, resets the run (`Mini_HighwayResetRun`: variant and high score from
 *   the savemap, speed, counters, the variant's event table), then the scene block (fails when
 *   null), the model roster and its RSD models (fails when null), the actor index, the layer
 *   block and its layers, the HUD block and the track block (fails when null);
 * - sets up the six actor slots and their skeletons, the effect pool, a text buffer of
 *   640 * 240 + 1 bytes (`Mini_InitPrimBuffer(0x18, 0xF0, 0x280, 0xF0, 0, 0)`) and the input
 *   state;
 * - loads the ten sounds of g_HighwaySfxIdTable, starts the music and dispatches sound 0xB3 of
 *   kind 4 and 0x12B of kind 3;
 * - sets g_HighwayRunningFlag.
 * The `new` results of the file, index, layer and HUD blocks are not checked.
 */
// FUNCTION: FF7 0x0065076d
extern "C" int __cdecl Mini_HighwayInit()
{
    int slotResult, sfxIdx;
    GfxEngineContext *pCtx;
    g_HighwayRunningFlag = 0;
    pCtx = GetEngineContext();
    SetMatrixLayoutMode(1);
    Pfile_SetAssetSourceMode(g_HighwayAssetSourceMode);
    g_HighwayDataDirectory = GetMinigameDirForHighway();
    if (g_HighwayUseLgpArchive != 0 && OpenLgpArchive(
                                 // STRING: FF7 0x009092e8
                                 "high-us.lgp", LGP_ARCHIVE_HIGHWAY) == 0) {
        return 0;
    }
    if (((HighwayPsxDataSet20 *)&g_HighwayPsxDataSet)->Mini_HighwayLoadPsxDataSet(
            // STRING: FF7 0x00909300
            "aaaa.adr",
            // STRING: FF7 0x009092f4
            "baaa.bin") == 0) {
        return 0;
    }
    g_pHighwayFileBlock = (struct HighwayFileBlock3 *)new HighwayFileLoader;
    Mini_HighwayResetRun();
    g_pHighwaySceneBlock = (struct HighwaySceneBlock2 *)new HighwayScene;
    if (g_pHighwaySceneBlock == 0)
        return 0;
    g_HighwayRosterBlock = (struct HighwayArray *)new HighwayRoster;
    if (g_HighwayRosterBlock == 0)
        return 0;
    ((HighwayRoster *)g_HighwayRosterBlock)->loadModels();
    g_pHighwayIndexBlock = new HighwayIndex;
    g_pHighwayModeBlock = new HighwayModeController;
    g_pHighwayModeBlock->createLayers();
    g_pHighwayRenderBlock = (int)new HighwayRenderOwner;
    g_pHighwayFieldManager = (struct HighwayFieldManager *)new HighwayFieldContext;
    if (g_pHighwayFieldManager == 0)
        return 0;
    slotResult = Mini_HighwayInitSlotArray((int)GetEngineContext());
    Mini_HighwayInitEffectPool();
    Mini_InitPrimBuffer(0x18, 0xf0, 0x280, 0xf0, 0, 0);
    Mini_HighwayResetInputState();
    for (sfxIdx = 9; sfxIdx >= 0; sfxIdx--) {
        SfxLoadIntoBuffer(g_HighwaySfxIdTable[sfxIdx], 0);
    }
    Mini_HighwayStartMusic();
    Mini_HighwayDispatchSoundCategory(0xb3, 4, 0);
    Mini_HighwayDispatchSoundCategory(0x12b, 3, 0);
    g_HighwayRunningFlag = 1;
    return 1;
}
extern "C" {
/*
 * Writes minigame scores back to save memory, releases allocations, and closes archive.
 */
// FUNCTION: FF7 0x00650ad5
void __cdecl Mini_Mode6ExitStoreScores(void)
{
    int bankIdx;
    int sfxIdx;

    MidiStop();
    g_pHighwaySoundCommandBuffer->command = 0xB8;
    g_pHighwaySoundCommandBuffer->param = 0;
    SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, 0, 0, 0, 0, 0, 0, 0);
    for (sfxIdx = 9; sfxIdx >= 0; sfxIdx--) {
        Sfx_ReleaseCachedBuffer(g_HighwaySfxIdTable[sfxIdx], 0xF);
    }
    Mini_ReleasePrimBuffer();
    if (g_HighwayGameMode == 1) {
        if (g_HighwayCurrentScore > g_HighwayHighScore) {
            g_HighwayHighScore = g_HighwayCurrentScore;
        }
        gSavemap.highScoreHigh = (g_HighwayHighScore & 0xFF00) >> 8;
        gSavemap.highScoreLow = g_HighwayHighScore & 0xFF;
        gSavemap.currentScoreHigh = (g_HighwayCurrentScore & 0xFF00) >> 8;
        gSavemap.currentScoreLow = g_HighwayCurrentScore & 0xFF;
    } else {
        bankIdx = 0x174;
        gSavemap.scriptBanks[bankIdx] = g_HighwayHpBarTable.bars[0].remaining & 0xFF;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = (g_HighwayHpBarTable.bars[0].remaining & 0xFF00) >> 8;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = g_HighwayHpBarTable.bars[1].remaining & 0xFF;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = (g_HighwayHpBarTable.bars[1].remaining & 0xFF00) >> 8;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = g_HighwayHpBarTable.bars[3].remaining & 0xFF;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = (g_HighwayHpBarTable.bars[3].remaining & 0xFF00) >> 8;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = g_HighwayHpBarTable.bars[2].remaining & 0xFF;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = (g_HighwayHpBarTable.bars[2].remaining & 0xFF00) >> 8;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = g_HighwayHpBarTable.bars[4].remaining & 0xFF;
        bankIdx++;
        gSavemap.scriptBanks[bankIdx] = (g_HighwayHpBarTable.bars[4].remaining & 0xFF00) >> 8;
        bankIdx++;
    }
    Mini_HighwayReleaseSkeletonList();
    delete (HighwayScene *)g_pHighwaySceneBlock;
    delete (HighwayFieldContext *)g_pHighwayFieldManager;
    delete (HighwayRenderOwner *)g_pHighwayRenderBlock;
    delete g_pHighwayModeBlock;
    delete g_pHighwayIndexBlock;
    delete (HighwayRoster *)g_HighwayRosterBlock;
    delete (HighwayFileLoader *)g_pHighwayFileBlock;
    ((HighwayPsxDataSet20 *)&g_HighwayPsxDataSet)->Mini_HighwayFreePsxDataSet();
    if (g_HighwayUseLgpArchive != 0) {
        CloseLgpArchive(LGP_ARCHIVE_HIGHWAY);
    }
}
/*
 * Measures elapsed timestamp counter and updates frame delta time factor.
 */
// FUNCTION: FF7 0x00650e88
void __cdecl Mini_HighwayUpdateFrameDelta(struct GfxEngineContext *context)
{
    Timestamp64 elapsed;
    Timestamp64 now;
    double elapsedSeconds;
    if (g_HighwayMusicActive != 0) {
        ReadTimestampCounter((struct Timestamp64 *)&g_HighwayTimestamp);
        g_HighwayTimeFrequencyReciprocal = 1.0 / context->tscFrequency;
        g_HighwayMusicActive = 0;
    } else {
        ReadTimestampCounter(&now);
        Sub64(&now, (struct Timestamp64 *)&g_HighwayTimestamp, &elapsed);
        g_HighwayFrameDeltaTime = (elapsedSeconds = CombineSplitValueToFloat(&elapsed) * g_HighwayTimeFrequencyReciprocal) * 25.0;
        if (g_HighwayFrameDeltaTime >= 16.0f)
            g_HighwayFrameDeltaTime = 16.0f;
        (*(struct Timestamp64 *)&g_HighwayTimestamp).lo = now.lo;
        g_HighwayTimestampHigh = now.hi;
    }
}
/*
 * Runs and draws one frame of the chase; called by `Mini_HighwayFrame`. Does nothing when the
 * scene cannot begin. Otherwise resets the frame's draw pools, turns all 22 render states
 * (0..0x15) off and depth test, depth write, perspective correction, linear filtering and
 * dithering on, then in order: runs the track event timeline, measures the frame time (25 units
 * per second, at most 16), scrolls the track by the speed, updates and draws the actor slots,
 * draws the layers and the track, updates the effect records, runs the HUD (`dispatch`:
 * `drawHud` in variant 1, `tick` in variant 0), updates the scene and camera, resets the
 * text buffer, switches to scene pass 1, flushes the frame draw list, draws the fade rectangle
 * and ends the scene.
 */
// FUNCTION: FF7 0x00650f36
void __cdecl Mini_HighwayRunFrame(struct GfxEngineContext *context)
{
    int i;

    if (((int (__cdecl *)(int scene, struct GfxEngineContext *context))Gfx_BeginSceneCall)(0, context) != 0) { /* reads EAX left by driver beginScene */
        ResetFrameDrawPool(context);
        ResetRenderPools(context);
        Gfx_ResetNodeList(context);
        for (i = 0; i < 0x16; i++) {
            Gfx_SetRenderStateCall(i, 0, (struct GfxEngineContext *)context);
        }
        Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_TEST, 1, (struct GfxEngineContext *)context);
        Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 1, (struct GfxEngineContext *)context);
        Gfx_SetRenderStateCall(RENDER_STATE_PERSPECTIVE, 1, (struct GfxEngineContext *)context);
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, (struct GfxEngineContext *)context);
        Gfx_SetRenderStateCall(RENDER_STATE_DITHER, 1, (struct GfxEngineContext *)context);
        Mini_HighwayRunEventTimeline();
        Mini_HighwayUpdateFrameDelta(context);
        Mini_HighwayAdvanceTimeline();
        Mini_HighwayUpdateAndRenderActors(g_HighwayActorRenderOffset, context);
        Mini_HighwayDrawLayers();
        Mini_HighwayDrawFieldContext();
        Mini_HighwayUpdateEffectRecords();
        Mini_HighwayDispatchModeController();
        Mini_HighwayUpdateScene();
        Mini_ResetPrimBuffer(-1);
        Gfx_SetScenePassCall(1, (struct GfxEngineContext *)context);
        GfxFlushFrameDrawList(context);
        ((HighwayRenderOwner *)g_pHighwayRenderBlock)->drawFade();
        Gfx_EndSceneCall(context);
    }
}
/*
 * Advances highway progress timeline accumulators and updates camera position.
 */
// FUNCTION: FF7 0x00651071
void __cdecl Mini_HighwayAdvanceTimeline(void)
{
    int speed;
    int stepCount;
    int i;

    speed = g_HighwayScrollSpeed;
    g_HighwayCameraZPosition += speed;
    g_HighwayScrollSubpixel += speed;
    stepCount = g_HighwayScrollSubpixel >> 8;
    g_HighwayScrollSubpixel &= 0xFF;
    g_HighwayTrackProgress += stepCount;
    g_HighwayActorRenderOffset = g_HighwayCameraZPosition + 0x2300;
    for (i = 0; i < stepCount; i++) {
        ((HighwaySlotList *)g_pHighwayFieldManager)->clearActive();
        ((HighwayFieldContext *)g_pHighwayFieldManager)->update();
    }
}
/*
 * Updates highway scene object state.
 */
// FUNCTION: FF7 0x0065110e
void __cdecl Mini_HighwayUpdateScene(void)
{
    ((HighwayScene *)g_pHighwaySceneBlock)->update();
}
/*
 * Renders the highway field context.
 */
// FUNCTION: FF7 0x0065111e
void __cdecl Mini_HighwayDrawFieldContext(void)
{
    ((HighwayFieldContext *)g_pHighwayFieldManager)->draw();
}
/*
 * Renders highway layer geometry.
 */
// FUNCTION: FF7 0x0065112e
void __cdecl Mini_HighwayDrawLayers(void)
{
    g_pHighwayModeBlock->drawLayers();
}
/*
 * Dispatches the highway mode controller update.
 */
// FUNCTION: FF7 0x0065113e
void __cdecl Mini_HighwayDispatchModeController(void)
{
    ((HighwayRenderOwner *)g_pHighwayRenderBlock)->dispatch();
}
/*
 * Runs the chase's scripted track events and its two ending sequences; called first in each
 * frame by `Mini_HighwayRunFrame`. Counts g_HighwayCountdown and g_HighwaySlotCountdown down to
 * 0. When the track progress minus 0x2D has passed the trigger of the next entry of
 * g_pHighwayTrackEventTable it runs that entry and moves to the next (at most one per frame):
 * - 0x12, 5, 0x13, 6: scene effects, skipped when bit 0 or 1 of g_HighwayControlFlags is set;
 * - 0xA/0xB, 0xC/0xD, 0x10/0x11, 0x19/0x1A: turn the layer block's four visibility flags on/off;
 * - 8: stops spawning (three actor groups flagged, g_HighwaySpawningStopped set);
 * - 0xE: dispatches sounds 0 and 0x126 of kind 3 and shows an actor part;
 * - 0x14: places three actors with fixed velocities and positions, activates their scene
 *   effects, locks the input and starts ending A;
 * - 0x15 (once): adjusts the lead actor group, starts scene effect 9, locks the input, starts
 *   ending B and shows banner 2;
 * - 0x16, 0x17, 0x18: banner 1, 3, 0; 0x1C: sets an actor group's slot0State to 6.
 * Each running ending counts g_HighwaySceneStep up once per frame and slows the scroll speed
 * g_HighwayScrollSpeed (despite its name the speed) to 0: ending B by 10 per frame after step
 * 5, starts the fade at step 0x109, sets the run state to 2 at 0x14A and requests the exit at
 * 0x190; ending A by 3 per frame after step 0xB4, fades at 0x13B while turning the music volume
 * down by one per step, sets the run state to 2 at 0x17C and requests the exit at 0x1C2. Both
 * play and then silence channel 3 and 4 sounds along the way. The event index has no bound
 * check: the table must end in a trigger the track never reaches.
 */
// FUNCTION: FF7 0x0065114e
void __cdecl Mini_HighwayRunEventTimeline(void)
{
    HighwayEventSlot *eventSlot;
    HighwayModeController *layer1Off;
    int banner1Block;
    int hud2;
    HighwayModeController *layer4On;
    HighwayModeController *layer4Off3;
    HighwayModeController *workModeOnTarget;
    int hud3;
    int renderBanner0;
    int endAFadeBlock;
    int fadeBlockB;
    int volume;
    HighwaySceneBlock2 *sceneObj;
    HighwayModeController *calcOffBlock;
    HighwayModeController *layer2On;
    HighwayModeController *layer2Off3;
    HighwayModeController *blockOn1;

    if (g_HighwayCountdown > 0) {
        g_HighwayCountdown--;
    }
    if (g_HighwaySlotCountdown > 0) {
        g_HighwaySlotCountdown--;
    }
    if (g_HighwayTrackProgress - 0x2D > g_pHighwayTrackEventTable[g_HighwayTrackEventIndex].triggerProgress) {
        switch (g_pHighwayTrackEventTable[g_HighwayTrackEventIndex].eventType) {
        case HIGHWAY_EVENT_SCENE_INIT:
            if ((g_HighwayControlFlags & (HIGHWAY_CTRL_DEBUG_CAM_MODE7 | HIGHWAY_CTRL_DEBUG_CAM_MODE6_FREEZE_SCROLL)) == 0) {
                ((HighwayEffect *)g_pHighwaySceneBlock)->init(5, 4, 0x27100);
            }
            break;
        case HIGHWAY_EVENT_SCENE_PRESET_05:
            if ((g_HighwayControlFlags & (HIGHWAY_CTRL_DEBUG_CAM_MODE7 | HIGHWAY_CTRL_DEBUG_CAM_MODE6_FREEZE_SCROLL)) == 0) {
                Mini_HighwaySetSceneBlockPresetEvent05((struct HighwayEffectSlotInitA *)g_pHighwaySceneBlock);
            }
            break;
        case HIGHWAY_EVENT_SCENE_PRESET_13:
            if ((g_HighwayControlFlags & (HIGHWAY_CTRL_DEBUG_CAM_MODE7 | HIGHWAY_CTRL_DEBUG_CAM_MODE6_FREEZE_SCROLL)) == 0) {
                Mini_HighwaySetSceneBlockPresetEvent13((struct HighwayEffectSlotInitA *)g_pHighwaySceneBlock);
            }
            break;
        case HIGHWAY_EVENT_SCENE_ON:
            if ((g_HighwayControlFlags & (HIGHWAY_CTRL_DEBUG_CAM_MODE7 | HIGHWAY_CTRL_DEBUG_CAM_MODE6_FREEZE_SCROLL)) == 0) {
                sceneObj = g_pHighwaySceneBlock;
                sceneObj->sceneKind = 1;
            }
            break;
        case HIGHWAY_EVENT_LAYER1_OFF:
            layer1Off = g_pHighwayModeBlock;
            layer1Off->visible[0] = 0;
            break;
        case HIGHWAY_EVENT_LAYER1_ON:
            blockOn1 = g_pHighwayModeBlock;
            blockOn1->visible[0] = 1;
            break;
        case HIGHWAY_EVENT_LAYER2_OFF:
            layer2Off3 = g_pHighwayModeBlock;
            layer2Off3->visible[1] = 0;
            break;
        case HIGHWAY_EVENT_LAYER2_ON:
            layer2On = g_pHighwayModeBlock;
            layer2On->visible[1] = 1;
            break;
        case HIGHWAY_EVENT_LAYER3_OFF:
            calcOffBlock = g_pHighwayModeBlock;
            calcOffBlock->visible[2] = 0;
            break;
        case HIGHWAY_EVENT_LAYER3_ON:
            workModeOnTarget = g_pHighwayModeBlock;
            workModeOnTarget->visible[2] = 1;
            break;
        case HIGHWAY_EVENT_LAYER4_OFF:
            layer4Off3 = g_pHighwayModeBlock;
            layer4Off3->visible[3] = 0;
            break;
        case HIGHWAY_EVENT_LAYER4_ON:
            layer4On = g_pHighwayModeBlock;
            layer4On->visible[3] = 1;
            break;
        case HIGHWAY_EVENT_STOP_SPAWNING:
            ((HighwayActorGroup *)&g_HighwaySlotActors)->slot2IsFlyingAway = 1;
            ((HighwayActorGroup *)&g_HighwaySlotActors)->slot3IsFlyingAway = 1;
            ((HighwayActorGroup *)&g_HighwaySlotActors)->slot4IsFlyingAway = 1;
            g_HighwaySpawningStopped = 1;
            break;
        case HIGHWAY_EVENT_SHOW_PART:
            Mini_HighwayDispatchSoundCategory(0, 3, 0);
            Mini_HighwayDispatchSoundCategory(0x126, 3, 0);
            ((HighwayActorGroup *)&g_HighwaySlotActors)->slot5State = 0;
            ((HighwayPart *)((HighwayActorGroup *)&g_HighwaySlotActors)->part)->visible(1);
            break;
        case HIGHWAY_EVENT_ENDING_A:
            eventSlot = (HighwayEventSlot *)(((char *)&g_HighwaySlotActors + 0x68));
            eventSlot->counter = 0;
            eventSlot->velocityX = 0xFFFF3800;
            eventSlot->posY = 0x38400;
            ((HighwayEffect *)g_pHighwaySceneBlock)->activate(0, 7);
            eventSlot = (HighwayEventSlot *)(((char *)&g_HighwaySlotActors + 0x8B0));
            eventSlot->velocityX = 0xAA00;
            eventSlot->posY = 0x1F400;
            eventSlot->stage = 6;
            ((HighwayEffect *)g_pHighwaySceneBlock)->activate(5, 6);
            eventSlot = (HighwayEventSlot *)(((char *)&g_HighwaySlotActors + 0x210));
            eventSlot->velocityX = 0xFFFF4C00;
            eventSlot->posY = 0x5DC00;
            eventSlot->stage = 6;
            ((HighwayEffect *)g_pHighwaySceneBlock)->activate(1, 8);
            ((HighwayEffect *)g_pHighwaySceneBlock)->init(2, 4, 0x10000);
            g_HighwayInputLock = 1;
            g_HighwayEndingAStarted = 1;
            g_HighwaySceneStep = 0;
            break;
        case HIGHWAY_EVENT_ENDING_B:
            if (g_HighwayEndingBStarted == 0) {
                (((HighwayActorGroup *)&g_HighwaySlotActors)->timer)--;
                ((HighwayActorGroup *)&g_HighwaySlotActors)->slot0AnimCountdown = 0x4A;
                ((HighwayActorGroup *)&g_HighwaySlotActors)->slot1AnimCountdown = 0x31;
                ((HighwayEffect *)g_pHighwaySceneBlock)->init(9, 4, 0x10000);
                g_HighwayInputLock = 1;
                g_HighwayEndingBStarted = 1;
                g_HighwaySceneStep = 0;
                hud2 = g_pHighwayRenderBlock;
                ((HighwayRenderOwner *)hud2)->banner = 2;
            }
            break;
        case HIGHWAY_EVENT_BANNER_1:
            banner1Block = g_pHighwayRenderBlock;
            ((HighwayRenderOwner *)banner1Block)->banner = 1;
            break;
        case HIGHWAY_EVENT_BANNER_3:
            hud3 = g_pHighwayRenderBlock;
            ((HighwayRenderOwner *)hud3)->banner = 3;
            break;
        case HIGHWAY_EVENT_BANNER_NONE:
            renderBanner0 = g_pHighwayRenderBlock;
            ((HighwayRenderOwner *)renderBanner0)->banner = 0;
            break;
        case HIGHWAY_EVENT_SET_GROUP_STATE:
            ((HighwayActorGroup *)&g_HighwaySlotActors)->slot0State = 6;
        }
        g_HighwayTrackEventIndex++;
    }
    if (g_HighwayEndingBStarted != 0) {
        if (g_HighwaySceneStep > 5) {
            if (g_HighwayScrollSpeed > 0) {
                g_HighwayScrollSpeed -= 0xA;
            }
            if (g_HighwayScrollSpeed < 0) {
                g_HighwayScrollSpeed = 0;
            }
        }
        if (g_HighwaySceneStep == 0x109) {
            Mini_HighwaySendSoundCommandB9();
            fadeBlockB = g_pHighwayRenderBlock;
            ((HighwayRenderOwner *)fadeBlockB)->fadeActive = 1;
        }
        if (g_HighwaySceneStep == 5) {
            Mini_HighwaySendChannelCommandA0(0x7F, 3);
            Mini_HighwaySendChannelCommandA0(0x7F, 4);
            Mini_HighwaySendChannelCommandB0(1, 3);
            Mini_HighwaySendChannelCommandB0(1, 4);
            Mini_HighwayDispatchSoundCategory(0x241, 3, 0);
            Mini_HighwayDispatchSoundCategory(0x241, 4, 0);
            ((HighwayActorGroup *)&g_HighwaySlotActors)->scrollPos -= 0x2800;
            ((HighwayActorGroup *)&g_HighwaySlotActors)->slot1ScrollPos += 0x6400;
        }
        if (g_HighwaySceneStep == 0x19) {
            Mini_HighwayDispatchSoundCategory(0, 3, 0);
            Mini_HighwayDispatchSoundCategory(0, 4, 0);
            Mini_HighwaySendChannelCommandA0(0, 3);
            Mini_HighwaySendChannelCommandA0(0, 4);
        }
        if (g_HighwaySceneStep == 0x14A) {
            g_HighwayRunPhase = 2;
        }
        if (g_HighwaySceneStep == 0x190) {
            g_HighwayExitRequested = 1;
        }
        g_HighwaySceneStep++;
    }
    if (g_HighwayEndingAStarted != 0) {
        if (g_HighwaySceneStep > 0xDC) {
            (((HighwayActorGroup *)&g_HighwaySlotActors)->frameCount)++;
        }
        if (g_HighwaySceneStep > 0xB4) {
            if (g_HighwayScrollSpeed > 0) {
                g_HighwayScrollSpeed -= 3;
            }
            if (g_HighwayScrollSpeed < 0) {
                g_HighwayScrollSpeed = 0;
            }
        }
        if (g_HighwaySceneStep == 0x13B) {
            Mini_HighwaySendSoundCommandB9();
            endAFadeBlock = g_pHighwayRenderBlock;
            ((HighwayRenderOwner *)endAFadeBlock)->fadeActive = 1;
        }
        if (g_HighwaySceneStep > 0x13B) {
            volume = 0x7F - (g_HighwaySceneStep - 0x13B);
            if (volume < 0) {
                volume = 0;
            }
            MidiSetVolume(volume);
            SoundUpdateFades();
        }
        if (g_HighwaySceneStep == 0xB8) {
            Mini_HighwaySendChannelCommandA0(0x7F, 3);
            Mini_HighwaySendChannelCommandA0(0x7F, 4);
            Mini_HighwaySendChannelCommandB0(1, 3);
            Mini_HighwaySendChannelCommandB0(1, 4);
            Mini_HighwayDispatchSoundCategory(0x241, 3, 0);
            Mini_HighwayDispatchSoundCategory(0x241, 4, 0);
        }
        if (g_HighwaySceneStep == 0xCB) {
            Mini_HighwayDispatchSoundCategory(0, 3, 0);
            Mini_HighwayDispatchSoundCategory(0, 4, 0);
            Mini_HighwaySendChannelCommandA0(0, 3);
            Mini_HighwaySendChannelCommandA0(0, 4);
        }
        if (g_HighwaySceneStep == 0x17C) {
            g_HighwayRunPhase = 2;
        }
        if (g_HighwaySceneStep == 0x1C2) {
            g_HighwayExitRequested = 1;
        }
        g_HighwaySceneStep++;
    }
}
/*
 * Takes `amount` of damage off character `category`'s HP fraction (g_HighwayHpBarTable;
 * character 2 takes none while its fraction is below 0x1E): flashes its bar (flashTimer = 0x70), adds
 * the amount to its damage taken and lowers the fraction, not below 0. When the fraction is 0
 * already, the damage goes to one of characters 1..4 that has some left (the loop's last match,
 * with `category` moving as it goes).
 */
// FUNCTION: FF7 0x00651844
void __cdecl Mini_HighwayDamageCharacter(char category, short amount)
{
    char i;
    char found;
    char candidate;

    if (g_HighwayHpBarTable.bars[2].remaining < 0x1E && category == 2) {
        amount = 0;
    }
    g_HighwayHpBarTable.bars[category].flashTimer = 0x70;
    g_HighwayHpBarTable.bars[category].damageTaken += amount;
    if (g_HighwayHpBarTable.bars[category].remaining > amount) {
        *(short *)&g_HighwayHpBarTable.bars[category].remaining -= amount;
    } else {
        g_HighwayHpBarTable.bars[category].remaining = 0;
    }
    if (g_HighwayHpBarTable.bars[category].remaining < 1) {
        found = 0;
        for (i = 0; i < 4; i++) {
            candidate = ((category + i) % 4) + 1;
            if (g_HighwayHpBarTable.bars[candidate].remaining != 0) {
                category = candidate;
                found = 1;
            }
        }
        if (found == 1) {
            g_HighwayHpBarTable.bars[category].flashTimer = 0x70;
            g_HighwayHpBarTable.bars[category].damageTaken += amount;
            if (g_HighwayHpBarTable.bars[category].remaining > amount) {
                *(short *)&g_HighwayHpBarTable.bars[category].remaining -= amount;
            } else {
                g_HighwayHpBarTable.bars[category].remaining = 0;
            }
        }
    }
}
/*
 * Computes linear falloff factor based on distance between min and max bounds.
 */
// FUNCTION: FF7 0x006519d3
float __cdecl Mini_HighwayLinearFalloff(float distance)
{
    if ((float)g_HighwayFalloffDistanceMin > distance) {
        return 1.0f;
    } else if ((float)g_HighwayFalloffDistanceMax < distance) {
        return 0.0f;
    } else {
        return 1.0f - (distance - g_HighwayFalloffDistanceMin) / (g_HighwayFalloffDistanceMax - g_HighwayFalloffDistanceMin);
    }
}
/*
 * Computes depth (Z component) of a 3D point transformed by a 4x4 matrix.
 */
// FUNCTION: FF7 0x00651a2b
float __cdecl Mini_HighwayTransformDepth(HighwayDepthTransform *transform, HighwayDepthPoint *point)
{
    return transform->depthWeightX * point->posX + transform->depthWeightY * point->posY + transform->depthWeightZ * point->posZ + transform->depthOffset;
}
/*
 * Sets record matrix pointer in the active highway file block.
 */
// FUNCTION: FF7 0x00651a5d
void __cdecl Mini_HighwaySetRecordMatrix(int recordMatrix)
{
    g_pHighwayFileBlock->recordMatrix = recordMatrix;
}
/*
 * Scales an RGB color triplet by the distance falloff factor.
 */
// FUNCTION: FF7 0x00651a6d
void __cdecl Mini_HighwayScaleColorTriplet(HighwayColorSourceTriplet *srcColor, struct HighwayDepthPoint *point, HighwayColorTriplet *dstColor)
{
    float scale, viewDist, r;
    float scaledLow;
    char s;
    float channel0Value;
    char clamped1;
    char clamped0;
    viewDist = Mini_HighwayTransformDepth((struct HighwayDepthTransform *)(g_pHighwayFileBlock->recordMatrix), point);
    scale = Mini_HighwayLinearFalloff(viewDist);
    if ((r = srcColor->channel2 * scale) >= 255.0f)
        s = 255;
    else
        s = (unsigned char)r;
    dstColor->channel2 = s;
    if ((scaledLow = srcColor->channel1 * scale) >= 255.0f)
        clamped1 = 255;
    else
        clamped1 = (unsigned char)scaledLow;
    dstColor->channel1 = clamped1;
    if ((channel0Value = srcColor->channel0 * scale) >= 255.0f)
        clamped0 = 255;
    else
        clamped0 = (unsigned char)channel0Value;
    dstColor->channel0 = clamped0;
}
/*
 * Dispatches sound command opcode 0xB9 to the sound system.
 */
// FUNCTION: FF7 0x00651b4e
void __cdecl Mini_HighwaySendSoundCommandB9(void)
{
    g_pHighwaySoundCommandBuffer->command = 0xB9;
    g_pHighwaySoundCommandBuffer->param = 0xF0;
    g_pHighwaySoundCommandBuffer->category = 0;
    SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, g_pHighwaySoundCommandBuffer->category, 0, 0, 0, 0, 0, 0);
}
/*
 * Sends sound command 0xB8 and initiates playback of highway background music.
 */
// FUNCTION: FF7 0x00651ba8
void __cdecl Mini_HighwayStartMusic(void)
{
    g_pHighwaySoundCommandBuffer->command = 0xB8;
    g_pHighwaySoundCommandBuffer->param = 0x7F;
    SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, 0, 0, 0, 0, 0, 0, 0);
    MidiPlaySong(0x26, 0, 0x7F);
}
/*
 * Dispatches sound effects for specified category and updates duration timers.
 */
// FUNCTION: FF7 0x00651bfc
void __cdecl Mini_HighwayDispatchSoundCategory(int category, int kind, int duration)
{
    int busy;

    busy = 0;
    if (kind == 1 && g_HighwayCountdown > 0) {
        busy = 1;
    }
    if (kind == 2 && g_HighwaySlotCountdown > 0) {
        busy = 1;
    }
    if (kind == 1 && duration < 0) {
        busy = 0;
        g_pHighwaySoundCommandBuffer->command = 0x28;
        g_pHighwaySoundCommandBuffer->param = 0;
        g_pHighwaySoundCommandBuffer->category = category;
        SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, g_pHighwaySoundCommandBuffer->category, 0, 0, 0, 0, 0, 0);
    }
    if (kind == 2 && duration < 0) {
        busy = 0;
        g_pHighwaySoundCommandBuffer->command = 0x29;
        g_pHighwaySoundCommandBuffer->param = 0;
        g_pHighwaySoundCommandBuffer->category = category;
        SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, g_pHighwaySoundCommandBuffer->category, 0, 0, 0, 0, 0, 0);
    }
    if (busy != 1) {
        if (kind == 1) {
            g_pHighwaySoundCommandBuffer->command = 0x28;
        }
        if (kind == 2) {
            g_pHighwaySoundCommandBuffer->command = 0x29;
        }
        if (kind == 3) {
            g_pHighwaySoundCommandBuffer->command = 0x2A;
        }
        if (kind == 4) {
            g_pHighwaySoundCommandBuffer->command = 0x2B;
        }
        g_pHighwaySoundCommandBuffer->param = 0x40;
        g_pHighwaySoundCommandBuffer->category = category;
        SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, g_pHighwaySoundCommandBuffer->category, 0, 0, 0, 0, 0, 0);
    }
    if (busy == 0 && kind == 1) {
        g_HighwayCountdown = duration;
    }
    if (busy == 0 && kind == 2) {
        g_HighwaySlotCountdown = duration;
    }
    if (kind == 1 && duration < 0) {
        g_HighwayCountdown = -duration;
    }
    if (kind == 2 && duration < 0) {
        g_HighwaySlotCountdown = -duration;
    }
}
/*
 * Sends channel command with opcode 0xB0-0xB3 based on channel number.
 */
// FUNCTION: FF7 0x00651df1
void __cdecl Mini_HighwaySendChannelCommandB0(int param, int channel)
{
    if (channel == 1) {
        g_pHighwaySoundCommandBuffer->command = 0xB0;
    }
    if (channel == 2) {
        g_pHighwaySoundCommandBuffer->command = 0xB1;
    }
    if (channel == 3) {
        g_pHighwaySoundCommandBuffer->command = 0xB2;
    }
    if (channel == 4) {
        g_pHighwaySoundCommandBuffer->command = 0xB3;
    }
    g_pHighwaySoundCommandBuffer->param = param;
    SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, 0, 0, 0, 0, 0, 0, 0);
}
/*
 * Sends channel command with opcode 0xA0-0xA3 based on channel number.
 */
// FUNCTION: FF7 0x00651e6d
void __cdecl Mini_HighwaySendChannelCommandA0(int param, char channel)
{
    if (channel == 1) {
        g_pHighwaySoundCommandBuffer->command = 0xA0;
    }
    if (channel == 2) {
        g_pHighwaySoundCommandBuffer->command = 0xA1;
    }
    if (channel == 3) {
        g_pHighwaySoundCommandBuffer->command = 0xA2;
    }
    if (channel == 4) {
        g_pHighwaySoundCommandBuffer->command = 0xA3;
    }
    g_pHighwaySoundCommandBuffer->param = param;
    SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, 0, 0, 0, 0, 0, 0, 0);
}
/*
 * Sends channel command with opcode 0xA8-0xAB based on channel number.
 */
// FUNCTION: FF7 0x00651ef6
void __cdecl Mini_HighwaySendChannelCommandA8(short param, char channel)
{
    if (channel == 1) {
        g_pHighwaySoundCommandBuffer->command = 0xA8;
    }
    if (channel == 2) {
        g_pHighwaySoundCommandBuffer->command = 0xA9;
    }
    if (channel == 3) {
        g_pHighwaySoundCommandBuffer->command = 0xAA;
    }
    if (channel == 4) {
        g_pHighwaySoundCommandBuffer->command = 0xAB;
    }
    g_pHighwaySoundCommandBuffer->param = param;
    SoundDispatchCommand((short)g_pHighwaySoundCommandBuffer->command, g_pHighwaySoundCommandBuffer->param, 0, 0, 0, 0, 0, 0, 0);
}
}
/*
 * Constructor for HighwayFileLoader, allocating matrix storage blocks.
 */
// FUNCTION: FF7 0x00651f7f
HighwayFileLoader::HighwayFileLoader()
{
    GfxEngineContext *ctx;
    ctx = GetEngineContext();
    this->receiver_minimum[0] = ctx->cameraMatrix;
    this->receiver_minimum[1] = (int)operator new(64);
    this->receiver_minimum[2] = (int)operator new(64);
    this->receiver_minimum[3] = (int)operator new(64);
    this->receiver_minimum[4] = (int)operator new(64);
    SetIdentityMatrix4x4((float(*)[4]) this->receiver_minimum[2]);
    SetIdentityMatrix4x4((float(*)[4]) this->receiver_minimum[3]);
    SetIdentityMatrix4x4((float(*)[4]) this->receiver_minimum[4]);
}
/*
 * Destructor for HighwayFileLoader, releasing matrix storage blocks.
 */
// FUNCTION: FF7 0x00652012
HighwayFileLoader::~HighwayFileLoader()
{
    int block1;
    int block2;
    int block3;
    int block4;
    block1 = this->receiver_minimum[1];
    operator delete((void *)block1);
    block2 = this->receiver_minimum[2];
    operator delete((void *)block2);
    block3 = this->receiver_minimum[3];
    operator delete((void *)block3);
    block4 = this->receiver_minimum[4];
    operator delete((void *)block4);
}
/*
 * Formats a debug/status message and dispatches it to the active target.
 */
// FUNCTION: FF7 0x00652073
extern "C" void __cdecl Mini_HighwayFormatAndDispatchMessage(char *format, ...)
{
    va_list args;
    char buf[200];

    va_start(args, format);
    vsprintf(buf, format, args);
    Gfx_DebugPrint(buf);
    va_end(args);
}

#pragma code_seg()

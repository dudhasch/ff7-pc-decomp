// src/ff7/src/battle/battle_fx_limit_cosmo_memory.cpp
// Limit Break [41] (Red XIII Lv 4): Cosmo Memory (Japanese: コスモメモリー, dev tag: "LIMIT3/C_M" / CM)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: CM limit break effect: a timeline schedules many growing, shrinking, falling and
//   expanding model tasks, rotating sprite rings, a screen flash, converging projectiles, and an
//   animated billboard sprite.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/enums/sound.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x0082c3b0
unsigned int g_BattleLimitCosmoMemoryResourceDef[160] = {
    0x001900b1,
    // STRING: FF7 0x0082c740
    (unsigned int)"STER.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c74c
    (unsigned int)"P_N1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c758
    (unsigned int)"P_N2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c764
    (unsigned int)"KOU1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c770
    (unsigned int)"SENKOU1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c77c
    (unsigned int)"HADO.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c788
    (unsigned int)"HADO1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c794
    (unsigned int)"HADO2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c7a0
    (unsigned int)"HADO3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c7ac
    (unsigned int)"HADO4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c7b8
    (unsigned int)"HADO5.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c7c4
    (unsigned int)"HADO6.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c7d0
    (unsigned int)"HADO7.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c7dc
    (unsigned int)"HADO8.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x006400b1,
    // STRING: FF7 0x0082c7e8
    (unsigned int)"WA.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c7f0
    (unsigned int)"IWA1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c7fc
    (unsigned int)"IWA2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c808
    (unsigned int)"ISI1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c814
    (unsigned int)"ISI4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c820
    (unsigned int)"BAKU1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c82c
    (unsigned int)"BAKU2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c838
    (unsigned int)"BAKU3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c844
    (unsigned int)"BAKU4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c850
    (unsigned int)"BAKU5.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c85c
    (unsigned int)"BAKU6.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c868
    (unsigned int)"BAKU7.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c874
    (unsigned int)"BAKU8.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082c880
    (unsigned int)"HIKARI.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0082c88c
    (unsigned int)"COSMO02.TIM",
    // STRING: FF7 0x0082c898
    (unsigned int)"TAMA.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x0082c8a4
    (unsigned int)"JO_A.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0082c8b0
    (unsigned int)"COSMO02.TIM",
    // STRING: FF7 0x0082c8bc
    (unsigned int)"KOU.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0082c8c4
    (unsigned int)"COSMO02.TIM",
    // STRING: FF7 0x0082c8d0
    (unsigned int)"KEMU.SPT", 0x00000000, 0x00000000};
// clang-format off
// GLOBAL: FF7 0x0082c630
struct BattleLimitCosmoMemoryAnimatedSpriteRequest g_BattleLimitCosmoMemoryAnimatedSpriteRequest = {0, 0, {0x40, 0x40, 0x40, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x0082c640
struct BattleLimitCosmoMemoryFadingSpriteRequest g_BattleLimitCosmoMemoryFadingSpriteRequest = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
// clang-format on
// GLOBAL: FF7 0x0082c650
unsigned char g_BattleLimitCosmoMemoryFallingModelRequest[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082c660
struct GfxRotationAngles2 g_BattleLimitCosmoMemoryGrowingModelRotation = {-1024, 0, 0};
// clang-format off
// GLOBAL: FF7 0x0082c668
struct BattleLimitCosmoMemoryGrowingModelRequest g_BattleLimitCosmoMemoryGrowingModelRequest = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0082c678
struct BattleLimitCosmoMemoryCentroidModelRequest g_BattleLimitCosmoMemoryCentroidModelRequest = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0082c688
struct BattleLimitCosmoMemoryShrinkingModelRequest g_BattleLimitCosmoMemoryShrinkingModelRequest = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x0082c698
struct GfxRotationAngles2 g_BattleLimitCosmoMemoryShrinkingModelRotation = {-1024, 0, 0};
// clang-format off
// GLOBAL: FF7 0x0082c6a0
struct BattleLimitCosmoMemoryExpandingModelRequest g_BattleLimitCosmoMemoryExpandingModelRequest = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x0082c6b0
unsigned char g_BattleLimitCosmoMemoryProjectileModelRequest[16] = {0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082c6c0
unsigned char g_BattleLimitCosmoMemoryProjectileScale[6] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30};
// clang-format off
unsigned char g_rawData_0082c6c8[8] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x0082c6d0
struct GfxRotationAngles2 g_BattleLimitCosmoMemoryProjectileRotation = {-1024, 0, 0};
// GLOBAL: FF7 0x0082c6d8
unsigned char g_BattleLimitCosmoMemoryConvergingModelRequest[16] = {0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// clang-format off
// GLOBAL: FF7 0x0082c6e8
struct BattleLimitCosmoMemoryConvergingSpriteRequest g_BattleLimitCosmoMemoryConvergingSpriteRequest = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x0082c6f8
struct BattleLimitCosmoMemoryRingSpriteRequest g_BattleLimitCosmoMemoryRingSpriteRequest = {0, 0, {0x80, 0x80, 0x80, 0x2e, 0, 0, 0, 0}};
// GLOBAL: FF7 0x0082c708
struct BattleLimitCosmoMemoryTriSpinModelRequest g_BattleLimitCosmoMemoryTriSpinModelRequest = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0082c718
struct BattleLimitCosmoMemoryTriSpinSpriteRequest g_BattleLimitCosmoMemoryTriSpinSpriteRequest = {0, 0, {0x80, 0x80, 0x80, 0x2e, 0, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x0082c728
unsigned char g_BattleLimitCosmoMemoryTriSpinWheelRotation[6] = {0x00, 0x04, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082c730
unsigned char g_BattleLimitCosmoMemoryTriSpinOuterScale[6] = {0x00, 0x0a, 0x00, 0x00, 0x00, 0x0a};
// clang-format off
unsigned char g_rawData_0082c738[8] = {0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x00b039d0
PsxMatrix g_BattleLimitCosmoMemoryLoadedResTable = {0};
// GLOBAL: FF7 0x00b039f0
short g_BattleLimitCosmoMemoryRingScale = 0;
unsigned char g_bssPad_00b039f4[4] = {0};
// GLOBAL: FF7 0x00b039f8
struct BattleSpecMagmaDrawListHead g_BattleLimitCosmoMemoryDrawListHead = {0};
// GLOBAL: FF7 0x00b039fc
short g_BattleLimitCosmoMemoryCentroidZ = 0;
// GLOBAL: FF7 0x00b03a00
short g_BattleLimitCosmoMemoryTriSpinIntensity = 0;
// GLOBAL: FF7 0x00b03a04
int g_BattleLimitCosmoMemoryTargetMask = 0;
// GLOBAL: FF7 0x00b03a08
short g_BattleLimitCosmoMemorySourceActor = 0;
// GLOBAL: FF7 0x00b03a0c
short g_BattleLimitCosmoMemoryConvergeScale = 0;
// GLOBAL: FF7 0x00b03a10
short g_BattleLimitCosmoMemoryFlashBlue = 0;
// GLOBAL: FF7 0x00b03a14
short g_BattleLimitCosmoMemoryShrinkIntensity = 0;
// GLOBAL: FF7 0x00b03a18
short g_BattleLimitCosmoMemoryFlashRed = 0;
// GLOBAL: FF7 0x00b03a1c
short g_BattleLimitCosmoMemoryFlashGreen = 0;
// GLOBAL: FF7 0x00b03a20
struct GfxTableCursor *g_pBattleLimitCosmoMemoryTableCursor = 0;
unsigned char g_bssPad_00b03a24[4] = {0};
// GLOBAL: FF7 0x00b03a28
int g_BattleLimitCosmoMemoryCentroidModelRotation = 0;
unsigned char g_bssPad_00b03a2c[4] = {0};
// GLOBAL: FF7 0x00b03a30
int g_BattleLimitCosmoMemoryExpandingModelRotation = 0;
unsigned char g_bssPad_00b03a34[4] = {0};
// GLOBAL: FF7 0x00b03a38
int g_BattleLimitCosmoMemoryConvergingRotation = 0;
unsigned char g_bssPad_00b03a3c[4] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * If the shared renderer exists and the cached resource is absent, initializes duration/options,
 * builds LIMIT3/C_M and creates selector thirty-two using the retained descriptor and renderer
 * field. Existing resources are untouched. Options are rooted at separate scalar requestSlot and
 * the path helper receives scalar loadPath.
 */
// FUNCTION: FF7 0x004c4be0
void __cdecl BattleFx_LimitLoadResCosmoMemory(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest requestSlot;
    int kindId;
    char loadPath[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_pBattleLimitCosmoMemoryTableCursor == 0) {
        kindId = 0x20;
        stepResourceDef = (int)&g_BattleLimitCosmoMemoryResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0082c8dc
        pRequest->resourcePath = BattleFx_BuildResourcePath("LIMIT3/C_M", loadPath);
        Gfx_LoadModelTableIfAbsent(kindId, stepResourceDef, pRequest, &g_pBattleLimitCosmoMemoryTableCursor, drawDev);
    }
}

/*
 * Loads CM resources, initializes eight global control values including three 0xF0 values and two
 * 0x400 values, calls BattleFx_LimitBeginCosmoMemory with both arguments, and returns zero.
 */
// FUNCTION: FF7 0x004c4c70
int __cdecl BattleFx_LimitEnterCosmoMemory(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResCosmoMemory();
    g_BattleLimitCosmoMemoryShrinkIntensity = 8;
    g_BattleLimitCosmoMemoryTriSpinIntensity = 0;
    g_BattleLimitCosmoMemoryFlashRed = 0xF0;
    g_BattleLimitCosmoMemoryFlashGreen = 0xF0;
    g_BattleLimitCosmoMemoryFlashBlue = 0xF0;
    g_BattleLimitCosmoMemoryRingScale = 0x400 /* 0.25 */;
    g_BattleLimitCosmoMemoryConvergeScale = 0x400 /* 0.25 */;
    BattleFx_LimitBeginCosmoMemory(targetMask, actorIdx);
    return 0;
}

/*
 * Stores the target mask and its bone centroid, schedules BattleFx_LimitCosmoMemoryTimelineTask at the source actor
 * selected bone lowered by one thousand and builds a horizontal look basis toward the centroid.
 * Records the source actor, loads LIMIT3/C_M, clears flash RGB, starts a sixteen-step ramp,
 * dispatches two panned sounds and sets the shared active flag.
 */
// FUNCTION: FF7 0x004c4ccb
void __cdecl BattleFx_LimitBeginCosmoMemory(int targetMask, int actorIdx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        SVECTOR dirX;
        struct { unsigned char pad_to_8[0x8]; BattleFxCosmoMemorySpawnRecord *spawnRecord; };
    } frame;
#pragma pack(pop)

    g_BattleLimitCosmoMemorySourceActor = targetMask;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleLimitCosmoMemoryDrawListHead);
    frame.spawnRecord = (BattleFxCosmoMemorySpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].altBone, (short *)((char *)frame.spawnRecord + 4));
    frame.spawnRecord->posY -= 1000;
    frame.dirX.vx = (g_BattleLimitCosmoMemoryDrawListHead.originX) - frame.spawnRecord->posX;
    frame.dirX.vy = 0;
    frame.dirX.vz = g_BattleLimitCosmoMemoryCentroidZ - frame.spawnRecord->posZ;
    Gte_BuildLookBasisFromDirection(&frame.dirX.vx, (short *)&g_BattleLimitCosmoMemoryLoadedResTable);
    frame.spawnRecord->targetMask = actorIdx;
    BattleFx_LimitLoadResCosmoMemory();
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 16);
    SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition((char *)frame.spawnRecord + 4), 0x128, 0, 0, 0, 0, 0, 0);
    SoundDispatchCommand(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition((char *)frame.spawnRecord + 4), 0x1AA, 0, 0, 0, 0, 0, 0);
    g_BattleScriptEffectFlag = 1;
}

/*
 * When unpaused, schedules the two source-position model effects at twenty, sprite rings at fifty,
 * expanding model at 135 and shrinking model/sound at 140. At 170 schedules three more effects
 * including centroid-position models and sounds; emits a centroid callback every five counts from
 * 170 through 195 and selected typed records at 230. Advances and at exactly 250 starts a
 * twelve-step fade, clears the shared active flag and retires.
 */
// FUNCTION: FF7 0x004c4e1d
void __cdecl BattleFx_LimitCosmoMemoryTimelineTask(void)
{
    BattleFxLimitCosmoMemoryTimeline *localTaskSlot;
    short actorIdx;
    BattleFxCosmoMemorySlot *newTaskPtr;
    int s;
    int t;
    int u;
    int v;
    int w;
    int x;
    int y;
    int z;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;

    localTaskSlot = (BattleFxLimitCosmoMemoryTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (localTaskSlot->timer == 20) {
        newTaskPtr = (BattleFxCosmoMemorySlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryTriModelSpinTask);
        newTaskPtr->originX = localTaskSlot->originX;
        newTaskPtr->originY = localTaskSlot->originY;
        newTaskPtr->state = localTaskSlot->state;
    }
    if (localTaskSlot->timer == 20) {
        newTaskPtr = (BattleFxCosmoMemorySlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryEmitConvergingProjectilesTask);
        newTaskPtr->originX = localTaskSlot->originX;
        newTaskPtr->originY = localTaskSlot->originY;
        newTaskPtr->state = localTaskSlot->state;
    }
    if (localTaskSlot->timer == 50) {
        newTaskPtr = (BattleFxCosmoMemorySlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryRotatingSpriteRingsTask);
        newTaskPtr->originX = localTaskSlot->originX;
        newTaskPtr->originY = localTaskSlot->originY;
        newTaskPtr->state = localTaskSlot->state;
    }
    if (localTaskSlot->timer == 135) {
        newTaskPtr = (BattleFxCosmoMemorySlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryGrowingModelTask);
        newTaskPtr->originX = localTaskSlot->originX;
        newTaskPtr->originY = localTaskSlot->originY;
        newTaskPtr->state = localTaskSlot->state;
    }
    if (localTaskSlot->timer >= 170 && localTaskSlot->timer < 200 && localTaskSlot->timer % 5 == 0) {
        newTaskPtr = (BattleFxCosmoMemorySlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryCentroidGrowingModelTask);
        newTaskPtr->originX = (g_BattleLimitCosmoMemoryDrawListHead.originX);
        newTaskPtr->originY = g_BattleLimitCosmoMemoryDrawListHead.originY;
        newTaskPtr->state = g_BattleLimitCosmoMemoryCentroidZ;
    }
    if (localTaskSlot->timer == 140) {
        newTaskPtr = (BattleFxCosmoMemorySlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryShrinkingCyclingModelsTask);
        newTaskPtr->originX = localTaskSlot->originX;
        newTaskPtr->originY = localTaskSlot->originY;
        newTaskPtr->state = localTaskSlot->state;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition((char *)newTaskPtr + 4), 0x182, 0, 0, 0, 0, 0, 0);
    }
    if (localTaskSlot->timer == 170) {
        Battle_AllocateResourceSlot(BattleFx_LimitCosmoMemoryScreenFlashTask);
    }
    if (localTaskSlot->timer == 170) {
        newTaskPtr = (BattleFxCosmoMemorySlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryExpandingModelTask);
        newTaskPtr->originX = (g_BattleLimitCosmoMemoryDrawListHead.originX);
        newTaskPtr->originY = g_BattleLimitCosmoMemoryDrawListHead.originY;
        newTaskPtr->state = g_BattleLimitCosmoMemoryCentroidZ;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition((char *)newTaskPtr + 4), 0x20D, 0, 0, 0, 0, 0, 0);
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition((char *)newTaskPtr + 4), 0x25A, 0, 0, 0, 0, 0, 0);
    }
    if (localTaskSlot->timer == 230) {
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if ((g_BattleLimitCosmoMemorySourceActor & 1 << actorIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIdx);
            }
        }
    }
    (localTaskSlot->timer)++;
    if (localTaskSlot->timer == 250) {
        Battle_StartFixedPointRamp(0, 12);
        g_BattleScriptEffectFlag = 0;
        localTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a screen flash from three shared channel values before checking pause. When unpaused
 * reduces green/blue by ten below counter twenty-four and red by four on every call without
 * clamping, then advances the counter and retires exactly at sixty.
 */
// FUNCTION: FF7 0x004c51e1
void __cdecl BattleFx_LimitCosmoMemoryScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleLimitCosmoMemoryFlashRed, *(char *)&g_BattleLimitCosmoMemoryFlashGreen, *(char *)&g_BattleLimitCosmoMemoryFlashBlue);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 24) {
        g_BattleLimitCosmoMemoryFlashGreen -= 0xA;
        g_BattleLimitCosmoMemoryFlashBlue -= 0xA;
    }
    g_BattleLimitCosmoMemoryFlashRed -= 4;
    (task->frameCounter)++;
    if (task->frameCounter == 60) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry four with fixed rotation concatenated with the shared look basis, task
 * position, age-times-1500 uniform scale and an age-shifted descriptor, composed with the camera
 * in black. Drawing continues while paused; unpaused calls advance and retire on equality with
 * thirty.
 */
// FUNCTION: FF7 0x004c5290
void __cdecl BattleFx_LimitCosmoMemoryGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitCosmoMemoryGrowState *growState; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.growState = (BattleFxLimitCosmoMemoryGrowState *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitCosmoMemoryGrowingModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(4, g_pBattleLimitCosmoMemoryTableCursor);
    RotMatrix((SVECTOR *)&g_BattleLimitCosmoMemoryGrowingModelRotation, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleLimitCosmoMemoryLoadedResTable, &frame.modelMatrix, &frame.modelMatrix);
    frame.scaleX.vz = frame.growState->frameIndex * 0x5DC;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    frame.modelMatrix.t[0] = frame.growState->posX;
    frame.modelMatrix.t[1] = frame.growState->posY;
    frame.modelMatrix.t[2] = frame.growState->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitCosmoMemoryGrowingModelRequest.recordFrameAngleA = frame.growState->frameIndex << 8;
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitCosmoMemoryGrowingModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growState->frameIndex)++;
    if (frame.growState->frameIndex == 30) {
        frame.growState->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry fourteen using shared rotation, task position, age-times-two-thousand uniform
 * scale and an age-shifted descriptor, composed with the camera in black. Drawing continues while
 * paused; unpaused calls advance and retire on equality with thirty.
 */
// FUNCTION: FF7 0x004c53d6
void __cdecl BattleFx_LimitCosmoMemoryCentroidGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitCosmoMemoryGrowState *centroidState; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.centroidState = (BattleFxLimitCosmoMemoryGrowState *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitCosmoMemoryCentroidModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(14, g_pBattleLimitCosmoMemoryTableCursor);
    RotMatrix((SVECTOR *)&g_BattleLimitCosmoMemoryCentroidModelRotation, &frame.modelMatrix);
    frame.scaleX.vz = frame.centroidState->frameIndex * 0x7D0;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    frame.modelMatrix.t[0] = frame.centroidState->posX;
    frame.modelMatrix.t[1] = frame.centroidState->posY;
    frame.modelMatrix.t[2] = frame.centroidState->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitCosmoMemoryCentroidModelRequest.recordFrameAngleB = frame.centroidState->frameIndex << 8;
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitCosmoMemoryCentroidModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.centroidState->frameIndex)++;
    if (frame.centroidState->frameIndex == 30) {
        frame.centroidState->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry five and a cycling entry six through thirteen with fixed/shared look
 * orientation, shrinking X/Z scale and fixed Y scale, composed with the camera in black.
 * Descriptor intensity derives from the shared counter. Drawing continues while paused; unpaused
 * calls lower that counter from twenty-two, advance and retire on equality with thirty.
 */
// FUNCTION: FF7 0x004c5507
void __cdecl BattleFx_LimitCosmoMemoryShrinkingCyclingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_24[0x24]; int cycleEntry0[8]; };
        struct { unsigned char pad_to_44[0x44]; BattleFxCosmoMemoryRingSlot *shrinkSlot; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.shrinkSlot = (BattleFxCosmoMemoryRingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.cycleEntry0[0] = 6;
    frame.cycleEntry0[1] = 7;
    frame.cycleEntry0[2] = 8;
    frame.cycleEntry0[3] = 9;
    frame.cycleEntry0[4] = 10;
    frame.cycleEntry0[5] = 11;
    frame.cycleEntry0[6] = 12;
    frame.cycleEntry0[7] = 13;
    RotMatrix((SVECTOR *)&g_BattleLimitCosmoMemoryShrinkingModelRotation, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleLimitCosmoMemoryLoadedResTable, &frame.modelMatrix, &frame.modelMatrix);
    frame.scaleX.vz = 0x5000 - frame.shrinkSlot->timer * 0x258;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = 0x4000 /* 4.0 */;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    frame.modelMatrix.t[0] = frame.shrinkSlot->posX;
    frame.modelMatrix.t[1] = frame.shrinkSlot->posY;
    frame.modelMatrix.t[2] = frame.shrinkSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitCosmoMemoryShrinkingModelRequest.recordStateAngleA = 0x1000 - (g_BattleLimitCosmoMemoryShrinkIntensity << 8);
    (g_BattleLimitCosmoMemoryShrinkingModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(5, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitCosmoMemoryShrinkingModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    (g_BattleLimitCosmoMemoryShrinkingModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(frame.cycleEntry0[((frame.shrinkSlot->timer) % 8)], g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitCosmoMemoryShrinkingModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.shrinkSlot->timer >= 22) {
        g_BattleLimitCosmoMemoryShrinkIntensity--;
    }
    (frame.shrinkSlot->timer)++;
    if (frame.shrinkSlot->timer == 30) {
        frame.shrinkSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry twenty-seven and a cycling entry nineteen through twenty-six with shared
 * rotation, growing X/Z scale and fixed Y scale, composed with the camera in black. Unpaused calls
 * emit a radial model before fifty and four secondary ground callbacks every call, alternate the
 * shared camera Y short by 64, advance and retire on equality with sixty. Drawing continues while
 * paused.
 */
// FUNCTION: FF7 0x004c56ff
void __cdecl BattleFx_LimitCosmoMemoryExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitCosmoMemorySpawn *spawn; };
        struct { unsigned char pad_to_24[0x24]; short billboardIndex; };
        struct { unsigned char pad_to_28[0x28]; int cycleEntry0[8]; };
        struct { unsigned char pad_to_48[0x48]; BattleFxCosmoMemoryExpandingModel *expandState; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; short radius; };
        struct { unsigned char pad_to_70[0x70]; short angle; };
        struct { unsigned char pad_to_74[0x74]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.expandState = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.cycleEntry0[0] = 19;
    frame.cycleEntry0[1] = 20;
    frame.cycleEntry0[2] = 21;
    frame.cycleEntry0[3] = 22;
    frame.cycleEntry0[4] = 23;
    frame.cycleEntry0[5] = 24;
    frame.cycleEntry0[6] = 25;
    frame.cycleEntry0[7] = 26;
    RotMatrix((SVECTOR *)&g_BattleLimitCosmoMemoryExpandingModelRotation, &frame.modelMatrix);
    frame.scaleX.vz = frame.expandState->frame * 0x190 + 0x1000;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = 0x7F00 /* 7.9375 */;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    frame.modelMatrix.t[0] = frame.expandState->posX;
    frame.modelMatrix.t[1] = frame.expandState->posY;
    frame.modelMatrix.t[2] = frame.expandState->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitCosmoMemoryExpandingModelRequest.recordTableOffset = frame.expandState->frame * 68;
    (g_BattleLimitCosmoMemoryExpandingModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(27, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitCosmoMemoryExpandingModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    (g_BattleLimitCosmoMemoryExpandingModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(frame.cycleEntry0[((frame.expandState->frame) % 8)], g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitCosmoMemoryExpandingModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.radius = frame.expandState->frame * 70 + 400;
    if (frame.expandState->frame < 50) {
        frame.spawn = (BattleFxLimitCosmoMemorySpawn *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryFallingSpinningModelTask);
        frame.angle = ((rand()) % 4096);
        frame.spawn->posX = frame.expandState->posX + (SinFixed(frame.angle) * frame.radius >> 12);
        frame.spawn->posZ = frame.expandState->posZ + (CosFixed(frame.angle) * frame.radius >> 12);
        frame.spawn->posY = 0;
        frame.spawn->modelIndex = ((rand()) % 4);
        frame.spawn->spin = ((rand()) % 4096);
    }
    for (frame.billboardIndex = 0; frame.billboardIndex < 4; frame.billboardIndex++) {
        frame.spawn = (BattleFxLimitCosmoMemorySpawn *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitCosmoMemoryAnimatedBillboardSpriteTask)];
        frame.angle = ((rand()) % 4096);
        frame.spawn->posX = frame.expandState->posX + (SinFixed(frame.angle) * frame.radius >> 12);
        frame.spawn->posZ = frame.expandState->posZ + (CosFixed(frame.angle) * frame.radius >> 12);
        frame.spawn->posY = 0;
    }
    g_BattleCameraFocalPoint.focalPointY += (short)(64 - (((frame.expandState->frame) % 2) << 7));
    (frame.expandState->frame)++;
    if (frame.expandState->frame == 60) {
        frame.expandState->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * One frame of a battle sprite effect driven from the second effect array, the 0x20-byte effect
 * slot at g_BattleTaskSlotBuffer.secondarySlots + g_BattleSecondaryTaskActiveSlotIdx * 0x20, rather
 * than the g_BattleHitLivewireEmitterRecords one the neighbouring handlers use. Texture and clut come
 * from entry 0x1F of table g_pBattleLimitCosmoMemoryTableCursor into
 * g_BattleLimitCosmoMemoryAnimatedSpriteRequest/g_BattleLimitCosmoMemoryAnimatedSpriteRequest.animatedBillboardRes,
 * the billboard from slot+4 at scale 0x4000, the frame counter into
 * g_BattleLimitCosmoMemoryAnimatedSpriteRequest.animatedBillboardShort12, and the draw goes through
 * BattleFx_SubmitSpriteDraw into g_BattleCurrentOrderingTable+0x70 at shift 0xC threading
 * g_BattleOverlayDrawListHead. With g_BattleIsPaused clear and g_BattleLimitCosmoMemoryAnimatedSpriteRequest
 * nonnull the frame advances and the slot retires with 0xFFFF at the animation length
 * the length word of the first table entry.
 */
// FUNCTION: FF7 0x004c5a8a
void __cdecl BattleFx_LimitCosmoMemoryAnimatedBillboardSpriteTask(void)
{
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    (g_BattleLimitCosmoMemoryAnimatedSpriteRequest.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(31, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryAnimatedSpriteRequest.animatedBillboardRes = Gfx_ModelTableGetSpriteObject(31, g_pBattleLimitCosmoMemoryTableCursor);
    BattleFx_InstallBillboardMatrix(task->data, 0x4000 /* 4.0 */, 0);
    g_BattleLimitCosmoMemoryAnimatedSpriteRequest.animatedBillboardShort12 = task->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&(g_BattleLimitCosmoMemoryAnimatedSpriteRequest.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleLimitCosmoMemoryAnimatedSpriteRequest.spriteGroup) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleLimitCosmoMemoryAnimatedSpriteRequest.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the task-selector-indexed model from entries fifteen through eighteen using stored Z
 * rotation, task position and uniform scale one half for selectors below two or doubled otherwise,
 * composed with the camera. Drawing continues while paused; unpaused calls lower Y by four
 * hundred, rotate by 0x100, advance and retire on equality with ten.
 */
// FUNCTION: FF7 0x004c5b5b
void __cdecl BattleFx_LimitCosmoMemoryFallingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[108];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_28[0x28]; int modelEntry0[4]; };
        struct { unsigned char pad_to_38[0x38]; BattleFxLimitCosmoMemorySpinRecord *spinRecord; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinRecord = (BattleFxLimitCosmoMemorySpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelEntry0[0] = 0xF;
    frame.modelEntry0[1] = 0x10;
    frame.modelEntry0[2] = 0x11;
    frame.modelEntry0[3] = 0x12;
    frame.scaleX.vz = frame.spinRecord->modelIndex < 2 ? 0x800 : 0x2000;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.spinRecord->spin;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    frame.modelMatrix.t[0] = frame.spinRecord->posX;
    frame.modelMatrix.t[1] = frame.spinRecord->posY;
    frame.modelMatrix.t[2] = frame.spinRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    (*(int *)&g_BattleLimitCosmoMemoryFallingModelRequest) = Gfx_ModelTableGetRenderObject(frame.modelEntry0[frame.spinRecord->modelIndex], g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitCosmoMemoryFallingModelRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinRecord->posY -= 0x190;
    frame.spinRecord->spin += 0x100 /* 22.5 deg */;
    (frame.spinRecord->frame)++;
    if (frame.spinRecord->frame == 10) {
        frame.spinRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entries one and two with shared rotation/scale and a two-frame sprite entry thirty
 * at task position, including while paused. Unpaused counts twenty through 109 emit a randomly
 * rotated four-thousand-unit offset callback with ten-step inward velocity. Grows shared scale
 * before 120, shrinks it afterward, advances and retires on equality with 150.
 */
// FUNCTION: FF7 0x004c5cde
void __cdecl BattleFx_LimitCosmoMemoryEmitConvergingProjectilesTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[108];
        int depth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR offsetX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxSpecialEnergyOrbSlot *projectile; };
        struct { unsigned char pad_to_30[0x30]; SVECTOR rotX; };
        struct { unsigned char pad_to_38[0x38]; BattleFxCosmoMemoryConvergeSlot *convergeSlot; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.convergeSlot = (BattleFxCosmoMemoryConvergeSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitCosmoMemoryConvergingSpriteRequest.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(30, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryConvergingSpriteRequest.spriteResIndex30 = Gfx_ModelTableGetSpriteObject(30, g_pBattleLimitCosmoMemoryTableCursor);
    frame.scaleVec.vz = g_BattleLimitCosmoMemoryConvergeScale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    RotMatrix((SVECTOR *)&g_BattleLimitCosmoMemoryConvergingRotation, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.convergeSlot->posX;
    frame.rotMatrix.t[1] = frame.convergeSlot->posY;
    frame.rotMatrix.t[2] = frame.convergeSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    (*(int *)&g_BattleLimitCosmoMemoryConvergingModelRequest) = Gfx_ModelTableGetRenderObject(1, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitCosmoMemoryConvergingModelRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    (*(int *)&g_BattleLimitCosmoMemoryConvergingModelRequest) = Gfx_ModelTableGetRenderObject(2, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitCosmoMemoryConvergingModelRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    BattleFx_InstallBillboardMatrix((char *)frame.convergeSlot + 4, g_BattleLimitCosmoMemoryConvergeScale, 0);
    g_BattleLimitCosmoMemoryConvergingSpriteRequest.spriteStepParity = (frame.convergeSlot->step % 2);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitSpriteDraw(&(g_BattleLimitCosmoMemoryConvergingSpriteRequest.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.convergeSlot->step >= 20 && frame.convergeSlot->step < 110) {
        frame.projectile = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCosmoMemoryProjectileTask);
        frame.offsetX.vx = 0;
        frame.offsetX.vy = 0;
        frame.offsetX.vz = 4000;
        frame.rotX.vx = (rand() % 4096);
        frame.rotX.vy = (rand() % 4096);
        frame.rotX.vz = 0;
        RotMatrix(&frame.rotX, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.offsetX, (short *)&frame.offsetX, &frame.depth);
        frame.projectile->posX = frame.convergeSlot->posX + frame.offsetX.vx;
        frame.projectile->posY = frame.convergeSlot->posY + frame.offsetX.vy;
        frame.projectile->posZ = frame.convergeSlot->posZ + frame.offsetX.vz;
        frame.projectile->velocityX = -frame.offsetX.vx / 10;
        frame.projectile->velocityY = -frame.offsetX.vy / 10;
        frame.projectile->velocityZ = -frame.offsetX.vz / 10;
    }
    if (frame.convergeSlot->step < 120) {
        g_BattleLimitCosmoMemoryConvergeScale += 0x40;
    } else {
        g_BattleLimitCosmoMemoryConvergeScale -= 0x100 /* 0.0625 */;
    }
    (frame.convergeSlot->step)++;
    if (frame.convergeSlot->step == 150) {
        frame.convergeSlot->state = 0xFFFF;
    }
}

/*
 * Builds a packed-frame look basis from task velocity, concatenates fixed rotation and scale,
 * composes task position with the camera and draws model entry three in black. Rendering continues
 * while paused. Unpaused calls move XYZ, advance and at exactly nine attempt a secondary callback
 * at the new position and retire.
 */
// FUNCTION: FF7 0x004c6036
void __cdecl BattleFx_LimitCosmoMemoryProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[108];
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxPositionedSpriteSlot *secondarySlot; };
        struct { unsigned char pad_to_28[0x28]; BattleLimitCosmoMemoryProjectile *projectile; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleLimitCosmoMemoryProjectile *)BATTLE_CURRENT_TASK_SLOT;
    (*(int *)&g_BattleLimitCosmoMemoryProjectileModelRequest) = Gfx_ModelTableGetRenderObject(3, g_pBattleLimitCosmoMemoryTableCursor);
    Gte_BuildLookBasisFromDirection((short *)((char *)frame.projectile + 0xC), frame.lookBasis.m[0]);
    RotMatrix((SVECTOR *)&g_BattleLimitCosmoMemoryProjectileRotation, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleLimitCosmoMemoryProjectileScale);
    frame.rotMatrix.t[0] = frame.projectile->posX;
    frame.rotMatrix.t[1] = frame.projectile->posY;
    frame.rotMatrix.t[2] = frame.projectile->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitCosmoMemoryProjectileModelRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.projectile->posX += frame.projectile->velX;
    frame.projectile->posY += frame.projectile->velY;
    frame.projectile->posZ += frame.projectile->velZ;
    (frame.projectile->frame)++;
    if (frame.projectile->frame == 9) {
        frame.secondarySlot = (BattleFxPositionedSpriteSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitCosmoMemoryFadingBillboardSpriteTask)]);
        frame.secondarySlot->posX = frame.projectile->posX;
        frame.secondarySlot->posY = frame.projectile->posY;
        frame.secondarySlot->posZ = frame.projectile->posZ;
        frame.projectile->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * One frame of a battle sprite effect on the 0x20-byte effect slot at
 * g_BattleTaskSlotBuffer.secondarySlots + g_BattleSecondaryTaskActiveSlotIdx * 0x20, the second
 * effect array: texture from entry 0x1D of table g_pBattleLimitCosmoMemoryTableCursor into
 * g_BattleLimitCosmoMemoryFadingSpriteRequest and the clut from Battle3d_GetSceneRenderObject(3, g_pBattle3dScene) into
 * g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardRes, billboard from slot+4 at scale 0x2000. The
 * sprite frame at g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardFlag is the frame counter clamped to
 * 0 or 1 (it is the boolean frame != 0), and the three colour bytes
 * g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardAlphaOld2/49/4a all get 0x80 - (frame << 4), fading
 * out across the eight frames it lives. Submitted by BattleFx_SubmitSpriteDraw into
 * g_BattleCurrentOrderingTable+0x70 at shift 0xC threading g_BattleOverlayDrawListHead; with
 * g_BattleIsPaused clear the frame advances and the slot retires with 0xFFFF at frame 8.
 */
// FUNCTION: FF7 0x004c61e9
void __cdecl BattleFx_LimitCosmoMemoryFadingBillboardSpriteTask(void)
{
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    (g_BattleLimitCosmoMemoryFadingSpriteRequest.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(29, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardRes = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(task->data, 0x2000 /* 2.0 */, 0);
    g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardFlag = task->frameCounter != 0;
    g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardAlpha = 0x80 - (task->frameCounter << 4);
    g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardAlphaOld1 = g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardAlpha;
    g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardAlphaOld2 = g_BattleLimitCosmoMemoryFadingSpriteRequest.fadingBillboardAlphaOld1;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&(g_BattleLimitCosmoMemoryFadingSpriteRequest.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter == 8) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws three sixteen-point rotating rings of sprite entry twenty-eight, using a six-hundred-unit
 * radius transformed by per-ring rotations and shared scale into task-relative billboard
 * positions. Drawing continues while paused. Unpaused calls grow shared scale before forty, shrink
 * it afterward, advance and retire on equality with 120.
 */
// FUNCTION: FF7 0x004c62dc
void __cdecl BattleFx_LimitCosmoMemoryRotatingSpriteRingsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        int depth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; short pointIndex; };
        struct { unsigned char pad_to_38[0x38]; short ringIndex; };
        struct { unsigned char pad_to_3C[0x3C]; short baseAngle; };
        struct { unsigned char pad_to_40[0x40]; BattleFxCosmoMemoryRingSlot *ringSlot; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.ringSlot = (BattleFxCosmoMemoryRingSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitCosmoMemoryRingSpriteRequest.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(28, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryRingSpriteRequest.recordResIndex28 = Gfx_ModelTableGetSpriteObject(28, g_pBattleLimitCosmoMemoryTableCursor);
    frame.scaleVec.vz = g_BattleLimitCosmoMemoryRingScale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    frame.baseAngle = frame.ringSlot->timer << 7;
    for (frame.ringIndex = 0; frame.ringIndex < 3; frame.ringIndex++) {
        for (frame.pointIndex = 0; frame.pointIndex < 16; frame.pointIndex++) {
            frame.pointX.vx = CosFixed((frame.pointIndex << 8) + frame.baseAngle) * 0x258 >> 12;
            frame.pointX.vy = SinFixed((frame.pointIndex << 8) + frame.baseAngle) * 0x258 >> 12;
            frame.pointX.vz = 0;
            frame.rotX.vx = frame.ringSlot->timer << 6;
            frame.rotX.vy = frame.ringIndex * 0x555;
            frame.rotX.vz = 0;
            RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
            ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
            frame.rotMatrix.t[0] = frame.ringSlot->posX;
            frame.rotMatrix.t[1] = frame.ringSlot->posY;
            frame.rotMatrix.t[2] = frame.ringSlot->posZ;
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.pointX, (short *)&frame.pointX, &frame.depth);
            BattleFx_InstallBillboardMatrix(&frame.pointX, 0x1000 /* 1.0 */, 0);
            g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitSpriteDraw(&(g_BattleLimitCosmoMemoryRingSpriteRequest.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.ringSlot->timer < 40) {
        g_BattleLimitCosmoMemoryRingScale += 0x80;
    } else {
        g_BattleLimitCosmoMemoryRingScale -= 0x48;
    }
    (frame.ringSlot->timer)++;
    if (frame.ringSlot->timer == 120) {
        frame.ringSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws three copies of model entry zero at transformed offsets -800, zero and 800 with count-
 * driven rotations and shrinking scale; the outer two also receive the fixed secondary scale.
 * Writes a shared descriptor from the shared intensity counter and loads sprite metadata without
 * submitting that sprite. Rendering continues while paused. Unpaused calls raise shared intensity
 * before sixteen, advance and retire on equality with 120.
 */
// FUNCTION: FF7 0x004c650a
void __cdecl BattleFx_LimitCosmoMemoryTriModelSpinTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[152];
        int depth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; short copyIndex; };
        struct { unsigned char pad_to_44[0x44]; BattleFxCosmoMemoryTriSpin *spinSlot; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix wheelMatrix; };
        struct { unsigned char pad_to_68[0x68]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_88[0x88]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitCosmoMemoryTriSpinModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(0, g_pBattleLimitCosmoMemoryTableCursor);
    (g_BattleLimitCosmoMemoryTriSpinSpriteRequest.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(28, g_pBattleLimitCosmoMemoryTableCursor);
    g_BattleLimitCosmoMemoryTriSpinSpriteRequest.recordResIndex28B = Gfx_ModelTableGetSpriteObject(28, g_pBattleLimitCosmoMemoryTableCursor);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitCosmoMemoryTriSpinModelRequest.recordStateAngleB = 0x1000 - (g_BattleLimitCosmoMemoryTriSpinIntensity << 8);
    for (frame.copyIndex = 0; frame.copyIndex < 3; frame.copyIndex++) {
        frame.scaleVec.vz = 0x1000 - frame.spinSlot->step * 0x28;
        frame.scaleVec.vy = frame.scaleVec.vz;
        frame.scaleVec.vx = frame.scaleVec.vy;
        frame.offsetX.vx = 0;
        frame.offsetX.vy = 0;
        frame.offsetX.vz = (frame.copyIndex - 1) * 800;
        frame.rotX.vz = 0;
        frame.rotX.vy = (frame.spinSlot->step << 4) + 0x400;
        frame.rotX.vx = frame.spinSlot->step << 4;
        RotMatrix(&frame.rotX, &frame.rotMatrix);
        ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
        frame.rotMatrix.t[0] = frame.spinSlot->posX;
        frame.rotMatrix.t[1] = frame.spinSlot->posY;
        frame.rotMatrix.t[2] = frame.spinSlot->posZ;
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.offsetX, (short *)&frame.offsetX, &frame.depth);
        (((BattleFxTimelineState *)&g_BattleLimitCosmoMemoryTriSpinWheelRotation)->wheelAngle) = frame.spinSlot->step * ((frame.copyIndex + 1 << 4) + 0x20);
        RotMatrixXYZ((int *)&g_BattleLimitCosmoMemoryTriSpinWheelRotation, &frame.wheelMatrix);
        frame.rotX.vz = 0;
        frame.rotX.vy = (frame.spinSlot->step << 4) + 0x400;
        frame.rotX.vx = frame.spinSlot->step << 4;
        RotMatrix(&frame.rotX, &frame.rotMatrix);
        ConcatMatrixPreservingTranslation(&frame.rotMatrix, &frame.wheelMatrix, &frame.rotMatrix);
        frame.scaleVec.vz = 0x7F00 - frame.spinSlot->step * 0x10E;
        frame.scaleVec.vy = frame.scaleVec.vz;
        frame.scaleVec.vx = frame.scaleVec.vy;
        ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
        if (frame.copyIndex != 1) {
            ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleLimitCosmoMemoryTriSpinOuterScale);
        }
        frame.rotMatrix.t[0] = frame.offsetX.vx;
        frame.rotMatrix.t[1] = frame.offsetX.vy;
        frame.rotMatrix.t[2] = frame.offsetX.vz;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
        SetRotMatrix(&frame.combinedMatrix);
        SetTransMatrix(&frame.combinedMatrix);
        g_BattleLimitCosmoMemoryTargetMask = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitCosmoMemoryTriSpinModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCosmoMemoryTargetMask);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->step < 16) {
        g_BattleLimitCosmoMemoryTriSpinIntensity++;
    }
    (frame.spinSlot->step)++;
    if (frame.spinSlot->step == 120) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */

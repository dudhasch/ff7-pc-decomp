// src/ff7/src/battle/battle_fx_limit_sled_fang.cpp
// Limit Break [35, 37] (Red XIII Lv 1-1 & 2-1): Sled Fang & Blood Fang (Japanese: スレッドファング / ブラッドファング, dev tag: "limit/sld_fang" / SldFang)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: SldFang limit break effect: slash trails that follow bone positions, impact particle
//   bursts, flat moving sprites and an alternate timeline, with a draw-list selection task.

// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "lib/api/sound.h"
#include "ff7/types/battle_limit.h"
extern "C" {

// clang-format on

// clang-format off
// GLOBAL: FF7 0x0083b398
unsigned int g_BattleLimitSledFangResourceDefs[8] = {0x00c82a39, (unsigned int)&g_BattleLimitSledFangResFilenameSlmo, (unsigned int)&g_BattleLimitSledFangResFilenameSmok, 0x00000000, 0x00000000, 0x00c82a39, (unsigned int)&g_BattleLimitSledFangResFilenameSmok[0xC], (unsigned int)&g_BattleLimitSledFangResFilenameSmok[0x1C]};
unsigned char g_rawData_0083b3b8[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0083b3d0
unsigned char g_BattleLimitSledFangVector1[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0083b3e0
unsigned char g_BattleLimitSledFangVector2[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x0083b3f8
unsigned char g_BattleLimitSledFangOffsetTable1[192] = {
    0x80, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00,
    0xa8, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00, 0x22, 0x01, 0x00, 0x00,
    0xc6, 0x00, 0x00, 0x00, 0x4a, 0x01, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x72, 0x01, 0x00, 0x00,
    0xd8, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x9a, 0x01, 0x00, 0x00,
    0xe0, 0x00, 0x00, 0x00, 0xa4, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xa9, 0x01, 0x00, 0x00,
    0xe0, 0x00, 0x00, 0x00, 0xa4, 0x01, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, 0x9f, 0x01, 0x00, 0x00,
    0xd2, 0x00, 0x00, 0x00, 0x9a, 0x01, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x95, 0x01, 0x00, 0x00,
    0xbe, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0xaf, 0x00, 0x00, 0x00, 0x8b, 0x01, 0x00, 0x00,
    0x9b, 0x00, 0x00, 0x00, 0x86, 0x01, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x81, 0x01, 0x00, 0x00,
    0x6e, 0x00, 0x00, 0x00, 0x7c, 0x01, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00,
    0x41, 0x00, 0x00, 0x00, 0x72, 0x01, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x6d, 0x01, 0x00, 0x00,
    0x2d, 0x00, 0x00, 0x00, 0x6d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// GLOBAL: FF7 0x0083b4b8
unsigned char g_BattleLimitSledFangOffsetTable2[120] = {0x40, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x0e, 0x01, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x54, 0x01, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00, 0xc2, 0x01, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00, 0xea, 0x01, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x1c, 0x02, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x35, 0x02, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x44, 0x02, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x5d, 0x02, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x71, 0x02, 0x00, 0x00, 0x0e, 0x01, 0x00, 0x00, 0x85, 0x02, 0x00, 0x00, 0x1d, 0x01, 0x00, 0x00, 0x99, 0x02, 0x00, 0x00};
// GLOBAL: FF7 0x0083b530
struct BattleLimitSledFangSpriteRequest1 g_BattleLimitSledFangSpriteRequest1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x0083b540
struct BattleLimitSledFangSpriteRequest2 g_BattleLimitSledFangSpriteRequest2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x0083b550
unsigned char g_BattleLimitSledFangResFilenameSlmo[13] = "slmoke_1.tim";
// GLOBAL: FF7 0x0083b560
unsigned char g_BattleLimitSledFangResFilenameSmok[41] =
    "smoke_1.spt\0"
    "sld_fang.tim\0\0\0\0"
    "sld_fang.spt";
// GLOBAL: FF7 0x00b47518
struct BattleLimitSledFangQuad g_BattleLimitSledFangQuadPrimitive1 = {0};
// GLOBAL: FF7 0x00b47528
struct BattleLimitSledFangQuad g_BattleLimitSledFangTrail2 = {0};
// GLOBAL: FF7 0x00b47538
int g_BattleLimitSledFangDrawListHandle = 0;
// GLOBAL: FF7 0x00b4753c
int g_BattleLimitSledFangTrailInitFlag = 0;
// g_BattleLimitSledFangParticles starts the pool of 222 impact particles (BattleFxSledFangParticleRecord,
// 0x18 bytes each) that the code indexes past this definition; g_BattleLimitSledFangUnusedFlag and the
// padding below are pieces of the same pool, and each record's scale is addressed through
// g_BattleLimitSledFangUnusedFlag (BattleFxSledFangParticleScale).
// GLOBAL: FF7 0x00b47540
struct BattleLimitSledFangTrailRingActiveFlags g_BattleLimitSledFangParticles = {0};
// GLOBAL: FF7 0x00b47554
int g_BattleLimitSledFangUnusedFlag = 0;
unsigned char g_bssPad_00b47558[4] = {0};
unsigned char g_bssPad_00b4755c[4] = {0};
unsigned char g_bssPad_00b47560[5296] = {0};
// The six trail rings, one per bone position record of g_BattleLimitSledFangBoneWorldPositions.
// GLOBAL: FF7 0x00b48a10
BattleLimitSledFangTrailRing g_BattleLimitSledFangTrailRingRecords[6] = {0};
// The six trails' bone positions: 24 points of 8 bytes each per record.
// GLOBAL: FF7 0x00b48c98
struct BattleFxSledFangBoneRecord g_BattleLimitSledFangBoneWorldPositions[6] = {0};
// GLOBAL: FF7 0x00b49118
int g_BattleLimitSledFangTargetMask = 0;
unsigned char g_bssPad_00b4911c[4] = {0};
// GLOBAL: FF7 0x00b49120
int g_BattleLimitSledFangActorIdx = 0;
// GLOBAL: FF7 0x00b49124
struct GfxTableCursor *g_BattleLimitSledFangLoadedResTable = 0;
// clang-format on

#pragma function(memcpy)
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(memset)
#pragma function(memset)

// When resource context is present and the cached handle is zero, prepares a default-duration
// resource record with first field zero and path limit3/sld_fang and requests resource type 2 using
// the scene's model table bank. Helpers access a record starting at a scalar local. Failed guards
// produce no operation.
// FUNCTION: FF7 0x004ea970
void __cdecl BattleFx_LimitLoadResSledFang(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestBuffer;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleLimitSledFangLoadedResTable == 0) {
        kind = 2;
        defPtr = (int)&g_BattleLimitSledFangResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0083b58c
        request->resourcePath = BattleFx_BuildResourcePath("limit3/sld_fang", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, defPtr, request, &g_BattleLimitSledFangLoadedResTable, context);
    }
}

/*
 * Calls BattleFx_LimitBeginSledFang with both arguments and mode 0, then returns zero.
 */
// FUNCTION: FF7 0x004eaa00
int __cdecl BattleFx_LimitEnterSledFang(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginSledFang(targetMask, actorIdx, 0);
    return 0;
}

/*
 * Calls BattleFx_LimitBeginSledFang with both arguments and mode 1, then returns zero.
 */
// FUNCTION: FF7 0x004eaa19
int __cdecl BattleFx_LimitEnterBloodFang(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginSledFang(targetMask, actorIdx, 1);
    return 0;
}

/*
 * Allocates and saves the draw-list selection task, loads SldFang resources and saves a render-
 * state block. Stores arg1 and its first selected low-ten-bit actor index when one exists, stores
 * arg2, allocates BattleFx_LimitBloodFangSpawnTimeline for nonzero arg3 or BattleFx_LimitSledFangSpawnTimeline for zero, and clears g_BattleLimitSledFangTrailInitFlag.
 */
// FUNCTION: FF7 0x004eaa32
void __cdecl BattleFx_LimitBeginSledFang(int targetMask, int actorIdx, int isAlt)
{
    int i;
    int found;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    BattleFx_LimitLoadResSledFang();
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    g_BattleLimitSledFangTargetMask = targetMask;
    i = 0;
    for (found = 0; i < 0xA; i++) {
        if ((targetMask & 1 << i) != 0) {
            found++;
            if (found == 1) {
                g_BattleLimitSledFangDrawListHandle = i;
            }
        }
    }
    g_BattleLimitSledFangActorIdx = actorIdx;
    if (isAlt != 0) {
        Battle_AllocateResourceSlot(BattleFx_LimitBloodFangSpawnTimeline);
    } else {
        Battle_AllocateResourceSlot(BattleFx_LimitSledFangSpawnTimeline);
    }
    g_BattleLimitSledFangTrailInitFlag = 0;
}

/*
 * Retires the current battle-effect slot (ownerSlot 0xFFFF), then allocates a new slot for
 * BattleFx_LimitSledFangTimelineTask via Battle_AllocateResourceSlot, fills its boneWorldPos with
 * Battle_GetBoneWorldPos(g_BattleLimitSledFangDrawListHandle, the actor's targetBone) and zeroes its
 * nextRingIndex. Then it fills the two quad records g_BattleLimitSledFangQuadPrimitive1 and
 * g_BattleLimitSledFangTrail2 with constants (colours 0x20/0x20/0x88 and 0x28/0x28/0xA0, tip colours
 * of blue 8, frameDelta/baseValue -0x30/0x40 and -0x90/0x52) and calls
 * BattleFx_LimitSledFangClearTrailRings.
 */
// FUNCTION: FF7 0x004eaaf1
void __cdecl BattleFx_LimitSledFangSpawnTimeline(void)
{
    BattleLimitSledFangSlot *task;

    task = (BattleLimitSledFangSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleLimitSledFangSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSledFangTimelineTask);
    Battle_GetBoneWorldPos(g_BattleLimitSledFangDrawListHandle, g_BattleActorRecords[g_BattleLimitSledFangDrawListHandle].targetBone, &task->boneWorldPos);
    task->nextRingIndex = 0;
    g_BattleLimitSledFangQuadPrimitive1.colorR = 0x20;
    g_BattleLimitSledFangQuadPrimitive1.colorG = 0x20;
    g_BattleLimitSledFangQuadPrimitive1.colorB = 0x88;
    g_BattleLimitSledFangQuadPrimitive1.colorARed = 0;
    g_BattleLimitSledFangQuadPrimitive1.colorAGreen = 0;
    g_BattleLimitSledFangQuadPrimitive1.colorABlue = 8;
    g_BattleLimitSledFangQuadPrimitive1.frameDelta = -0x30;
    g_BattleLimitSledFangQuadPrimitive1.baseValue = 0x40;
    g_BattleLimitSledFangTrail2.colorR = 0x28;
    g_BattleLimitSledFangTrail2.colorG = 0x28;
    g_BattleLimitSledFangTrail2.colorB = 0xA0;
    g_BattleLimitSledFangTrail2.colorARed = 0;
    g_BattleLimitSledFangTrail2.colorAGreen = 0;
    g_BattleLimitSledFangTrail2.colorABlue = 8;
    g_BattleLimitSledFangTrail2.frameDelta = -0x90;
    g_BattleLimitSledFangTrail2.baseValue = 0x52;
    BattleFx_LimitSledFangClearTrailRings();
}

/*
 * Clears the six rings of g_BattleLimitSledFangTrailRingRecords: for each it zeroes the head, the
 * tail and the write index and the 24 point addresses.
 */
// FUNCTION: FF7 0x004eabe2
void __cdecl BattleFx_LimitSledFangClearTrailRings(void)
{
    int ring;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int slot;

    for (ring = 0; ring < 6; ring++) {
        g_BattleLimitSledFangTrailRingRecords[ring].headIndex = 0;
        g_BattleLimitSledFangTrailRingRecords[ring].tailIndex = 0;
        g_BattleLimitSledFangTrailRingRecords[ring].battleLimitSldFangTrailPointIndex = 0;
        for (slot = 0; slot < 0x18; slot++) {
            g_BattleLimitSledFangTrailRingRecords[ring].points[slot] = 0;
        }
    }
}

/*
 * Returns when paused. At age 12 allocates three trail callbacks with staggered angles, common
 * parameters and consecutive ring indices, initializes each from the selected actor bone at Y
 * -380, updates the next ring index and plays a centered sound. At age 27 reads another bone,
 * allocates a follow-up effect using controller X/Z and that bone Y, retires the controller and
 * plays a position-panned sound. Increments age after either event.
 */
// FUNCTION: FF7 0x004eac64
void __cdecl BattleFx_LimitSledFangTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        BattleFxLimitSledFangState *sldFangState;
        struct { unsigned char pad_to_4[0x4]; BattleFxLimitSledFangTrailSlot *trailSlot; };
        struct { unsigned char pad_to_C[0xC]; int ringIndex; };
        struct { unsigned char pad_to_10[0x10]; int trailIndex; };
        struct { unsigned char pad_to_14[0x14]; int boneX; };
        struct { unsigned char pad_to_16[0x16]; short boneY; };
    } frame;
#pragma pack(pop)

    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sldFangState = (BattleFxLimitSledFangState *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.sldFangState->frameIndex == 0xC) {
        frame.ringIndex = frame.sldFangState->nextRingIndex;
        for (frame.trailIndex = 0; frame.trailIndex < 3; frame.trailIndex++) {
            frame.trailSlot = (BattleFxLimitSledFangTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSledFangSlashTrailTask);
            frame.trailSlot->baseAngle = frame.trailIndex * 0x550;
            frame.trailSlot->offsetY = 0x78;
            frame.trailSlot->offsetZ = 0xFDA8;
            frame.trailSlot->ringIndex = *(short *)&frame.ringIndex;
            Battle_GetBoneWorldPos(g_BattleLimitSledFangActorIdx, g_BattleActorRecords[g_BattleLimitSledFangActorIdx].altBone, (short *)&g_BattleLimitSledFangBoneWorldPositions[frame.ringIndex]);
            g_BattleLimitSledFangBoneWorldPositions[frame.ringIndex].posY = 0xFE84;
            BattleFx_LimitSledFangPushTrailPoint(&g_BattleLimitSledFangTrailRingRecords[frame.ringIndex], &g_BattleLimitSledFangBoneWorldPositions[frame.ringIndex]);
            frame.ringIndex++;
        }
        frame.sldFangState->nextRingIndex = *(short *)&frame.ringIndex;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x186);
    }
    if (frame.sldFangState->frameIndex == 0x1B) {
        Battle_GetBoneWorldPos(g_BattleLimitSledFangActorIdx, g_BattleActorRecords[g_BattleLimitSledFangActorIdx].targetBone, (short *)&frame.boneX);
        frame.trailSlot = (BattleFxLimitSledFangTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSledFangSpawnImpactParticlesTask);
        frame.trailSlot->posX = frame.sldFangState->posX;
        frame.trailSlot->posY = frame.boneY;
        frame.trailSlot->posZ = frame.sldFangState->posZ;
        frame.sldFangState->ownerSlot = BATTLE_TASK_FREE;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.trailSlot->posX), 0x1A3);
    }
    (frame.sldFangState->frameIndex)++;
}

/*
 * Pushes pointValue into the fixed 24-entry ring buffer: the write index, the tail and the count
 * precede the point addresses. Stores at the write slot, wraps the index and the tail to 0 at 0x18,
 * saturates the count at 0x18, and when the index catches the tail advances the tail and drops the
 * count by one (clamped at zero), discarding the oldest entry. Same shape as
 * Battle_EffectProgramPushTrailRingEntry but with the capacity baked in rather than read from the
 * header, and no head history.
 */
// FUNCTION: FF7 0x004eae54
void __cdecl BattleFx_LimitSledFangPushTrailPoint(BattleLimitSledFangTrailRing *ringBuffer, void *pointValue)
{
    int num;
    int tailIdx;
    int writeIdx;

    writeIdx = ringBuffer->battleLimitSldFangTrailPointIndex;
    tailIdx = ringBuffer->tailIndex;
    num = ringBuffer->headIndex;
    ringBuffer->points[writeIdx] = pointValue;
    writeIdx++;
    num++;
    if (num >= 0x18) {
        num = 0x18;
    }
    if (writeIdx >= 0x18) {
        writeIdx = 0;
    }
    if (writeIdx == tailIdx) {
        tailIdx++;
        num--;
        if (num < 0) {
            num = 0;
        }
        if (tailIdx >= 0x18) {
            tailIdx = 0;
        }
    }
    ringBuffer->battleLimitSldFangTrailPointIndex = writeIdx;
    ringBuffer->tailIndex = tailIdx;
    ringBuffer->headIndex = num;
}

/*
 * When unpaused appends smoothed bone positions at Y -380 to a 24-entry trail ring: before age 10
 * adds midpoint and endpoint, afterward adds three intermediate points and endpoint. Independently
 * draws the current ring using actor heading, age-dependent angle, configured color and per-
 * section table offsets; carries prior transformed points and shading state between sections.
 * Rendering and retirement at age at least 24 occur before pause. When unpaused after age 15
 * reduces ring count by two and increments age.
 */
// FUNCTION: FF7 0x004eaeff
void __cdecl BattleFx_LimitSledFangSlashTrailTask(void)
{
    int framePhase;
    int ofsZ;
    int yOff;
    int segment;
    BattleLimitSledFangTrailPoint *pPoints;
    int prevIndex;
    int phaseDelta;
    BattleFxLimitSledFangTrail *pContext;
    BattleLimitSledFangTrailRing *ring;
    int count;
    BattleFxFangSlashTrail *task;
    int heading;
    int readPos;
    SVECTOR *prevPointPtr;

    task = (BattleFxFangSlashTrail *)BATTLE_CURRENT_TASK_SLOT;
    pContext = (BattleFxLimitSledFangTrail *)Gfx_GetPsxScratchpadPtr(0);
    ring = &g_BattleLimitSledFangTrailRingRecords[task->trailIndex];
    pPoints = (BattleLimitSledFangTrailPoint *)&g_BattleLimitSledFangBoneWorldPositions[task->trailIndex];
    if (g_BattleIsPaused == 0) {
        Battle_GetBoneWorldPos(g_BattleLimitSledFangActorIdx, g_BattleActorRecords[g_BattleLimitSledFangActorIdx].altBone, &pContext->edgeAX);
        pContext->originX = 0xFE84;
        readPos = ring->battleLimitSldFangTrailPointIndex - 1;
        if (readPos < 0) {
            readPos = 0x17;
        }
        *(Int64Pair *)&pContext->edgeBX = *(Int64Pair *)&pPoints[readPos];
        if (task->frame < 0xA) {
            pPoints[ring->battleLimitSldFangTrailPointIndex].posX = pContext->edgeAX + pContext->edgeBX >> 1;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posZ = 0xFE84;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posY = pContext->edgeAY + pContext->edgeBY >> 1;
            BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
        } else {
            pContext->midX = pContext->edgeAX + pContext->edgeBX >> 1;
            pContext->originY = 0xFE84;
            pContext->midY = pContext->edgeAY + pContext->edgeBY >> 1;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posX = pContext->midX + pContext->edgeBX >> 1;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posZ = 0xFE84;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posY = pContext->midY + pContext->edgeBY >> 1;
            BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
            *(Int64Pair *)&pPoints[ring->battleLimitSldFangTrailPointIndex] = *(Int64Pair *)&pContext->midX;
            BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
            pPoints[ring->battleLimitSldFangTrailPointIndex].posX = pContext->midX + pContext->edgeAX >> 1;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posZ = 0xFE84;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posY = pContext->midY + pContext->edgeAY >> 1;
            BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
        }
        *(Int64Pair *)&pPoints[ring->battleLimitSldFangTrailPointIndex] = *(Int64Pair *)&pContext->edgeAX;
        BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
    }
    framePhase = task->baseValue + task->frame * g_BattleLimitSledFangQuadPrimitive1.frameDelta;
    heading = g_BattleActorRecords[g_BattleLimitSledFangActorIdx].yaw;
    phaseDelta = g_BattleLimitSledFangQuadPrimitive1.baseValue;
    pContext->rotX = 0;
    pContext->trailIndex = heading;
    pContext->tableOffset = 0;
    RotMatrix((SVECTOR *)&pContext->rotX, (int *)&pContext->rotMatrix);
    TransformVectorToInt32(&pContext->rotMatrix, g_BattleLimitSledFangVector1, &pContext->dirX);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    SetDrawColorRGB(g_BattleLimitSledFangQuadPrimitive1.colorR, g_BattleLimitSledFangQuadPrimitive1.colorG, g_BattleLimitSledFangQuadPrimitive1.colorB);
    pContext->quadA = g_BattleLimitSledFangQuadPrimitive1.colorA;
    pContext->colorA = g_BattleLimitSledFangQuadPrimitive1.colorA;
    yOff = task->offsetY;
    ofsZ = task->offsetZ;
    count = ring->headIndex - 1;
    readPos = ring->battleLimitSldFangTrailPointIndex - 1;
    if (readPos < 0) {
        readPos = 0x17;
    }
    *(Int64Pair *)&pContext->posX = *(Int64Pair *)ring->points[readPos];
    pContext->posX += (short)(ofsZ * pContext->dirX >> 0xC);
    pContext->posY += (short)(ofsZ * pContext->dirY >> 0xC);
    pContext->posZ += (short)(ofsZ * pContext->dirZ >> 0xC);
    BattleFx_LimitSledFangProjectTrailEdgePoints(pContext, &pContext->posX, (GfxPsxShortVertexSlot *)&pContext->posX);
    for (segment = 1; segment <= count; readPos--, segment++) {
        if (readPos < 0) {
            readPos = 0x17;
        }
        prevIndex = readPos - 1;
        if (prevIndex < 0) {
            prevIndex = 0x17;
        }
        framePhase += phaseDelta;
        pContext->posX = 0;
        pContext->posY = -(*(int *)(g_BattleLimitSledFangOffsetTable1 + (segment * 8 - 4)) + yOff);
        pContext->posZ = ofsZ;
        pContext->tableOffset = framePhase - *(int *)(g_BattleLimitSledFangOffsetTable1 + (segment * 8 - 8));
        RotMatrix((SVECTOR *)&pContext->rotX, (int *)&pContext->rotMatrix);
        TransformVectorToInt16(&pContext->rotMatrix, &pContext->posX, &pContext->tipX);
        pContext->tableOffset = framePhase + *(int *)(g_BattleLimitSledFangOffsetTable1 + (segment * 8 - 8));
        RotMatrix((SVECTOR *)&pContext->rotX, (int *)&pContext->rotMatrix);
        TransformVectorToInt16(&pContext->rotMatrix, &pContext->posX, &pContext->posX);
        prevPointPtr = (struct SVECTOR *)ring->points[prevIndex];
        pContext->posX += prevPointPtr->vx;
        pContext->posY += prevPointPtr->vy;
        pContext->posZ += prevPointPtr->vz;
        pContext->tipX += prevPointPtr->vx;
        pContext->tipY += prevPointPtr->vy;
        pContext->tipZ += prevPointPtr->vz;
        pContext->prevValueA = pContext->valueA;
        pContext->prevValueB = pContext->valueB;
        pContext->prevVertexA = pContext->vertexA;
        pContext->prevVertexB = pContext->vertexB;
        pContext->prevDepth = pContext->depth;
        pContext->quadB = pContext->quadA;
        SetRotMatrix(&g_BattleCameraTransform);
        SetTransMatrix(&g_BattleCameraTransform);
        RefreshGlobalTransformState();
        BattleFx_LimitSledFangProjectTrailEdgePoints(pContext, &pContext->posX, (GfxPsxShortVertexSlot *)&pContext->tipX);
        BattleFx_LimitSledFangDrawTrailSegmentQuad(pContext, count - segment, count);
    }
    if (task->frame >= 0x18) {
        task->slotTag = 0xFFFF;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame > 0xF) {
        ring->headIndex -= 2;
    }
    (task->frame)++;
}

// Draws one shaded quad segment of a Sld-fang trail. The colour colorA is blended by the 12-bit
// fraction 0x1000 / totalSegments * segmentIdx with Gfx_ShadePsxVertexColor into quadA. A primitive
// from Gfx_GetPsxPacketScratchPtr takes quadA into color0 and color1 and quadB into color2 and color3, its
// top tag byte is set to 8 and its code byte to 0x3A, and the four vertex slots are set to vertexA,
// vertexB, prevVertexA and prevVertexB with Gfx_SetVertexSlotClipRect before Gfx_DrawPsxPrimitive.
// The local (prevDepth + depth) >> 1 >> 2 is computed but never read.
// FUNCTION: FF7 0x004eb63c
void __cdecl BattleFx_LimitSledFangDrawTrailSegmentQuad(BattleFxLimitSledFangTrail *context, int segmentIdx, int totalSegments)
{
    int colA;
    int colorB;
    int depthAvg;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int blend;
    BattleFxTrailSegmentQuadPrim *packet;

    blend = 0x1000 / totalSegments * segmentIdx;
    Gfx_ShadePsxVertexColor((CVECTOR *)&context->colorA, blend, (CVECTOR *)&context->quadA);
    colA = context->quadA;
    colorB = context->quadB;
    packet = (BattleFxTrailSegmentQuadPrim *)Gfx_GetPsxPacketScratchPtr();
    depthAvg = context->prevDepth + context->depth >> 1 >> 2;
    packet->color0 = colA;
    packet->color1 = colA;
    packet->color2 = colorB;
    packet->color3 = colorB;
    packet->colorCommand = packet->colorCommand & 0xFFFFFF | 0x8000000;
    packet->code = 0x3A;
    Gfx_SetVertexSlotClipRect(0, (struct GfxHomogeneousVec4 *)&context->vertexA);
    Gfx_SetVertexSlotClipRect(1, (struct GfxHomogeneousVec4 *)&context->vertexB);
    Gfx_SetVertexSlotClipRect(2, (struct GfxHomogeneousVec4 *)&context->prevVertexA);
    Gfx_SetVertexSlotClipRect(3, (struct GfxHomogeneousVec4 *)&context->prevVertexB);
    Gfx_DrawPsxPrimitive(packet);
}

// Projects two points for the trail at context. depth gets Gfx_TransformVertexSlot0ReturnDepth(pt0,
// &projUnused, &projFlags), and vertex slot 0 is copied into vertexA with
// Gfx_ReadVertexSlotVector. Then pt1 is projected with Gfx_TransformVertexSlot1ReturnDepth using
// the same two pointers, and vertex slot 1 is copied into vertexB.
// FUNCTION: FF7 0x004eb748
void __cdecl BattleFx_LimitSledFangProjectTrailEdgePoints(BattleFxLimitSledFangTrail *context, void *pt0, GfxPsxShortVertexSlot *pt1)
{
    context->depth = Gfx_TransformVertexSlot0ReturnDepth(pt0, &context->projUnused, &context->projFlags);
    Gfx_ReadVertexSlotVector(0, (struct GfxHomogeneousVec4 *)&context->vertexA);
    Gfx_TransformVertexSlot1ReturnDepth(pt1, &context->projUnused, &context->projFlags);
    Gfx_ReadVertexSlotVector(1, (struct GfxHomogeneousVec4 *)&context->vertexB);
}

/*
 * On unpaused ages 0 through 2, allocates an eight-particle sprite callback and fills eight
 * records in the wrapping 222-entry particle array with actor-heading-rotated random directions,
 * random positions, velocities and scales. Ages at least 6 retire the controller before
 * incrementing its age.
 */
// FUNCTION: FF7 0x004eb7ae
void __cdecl BattleFx_LimitSledFangSpawnImpactParticlesTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[100];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitSledFangImpact *impact; };
        struct { unsigned char pad_to_24[0x24]; BattleFxSledFangSprites *particleTask; };
        struct { unsigned char pad_to_28[0x28]; int tmpRand; };
        struct { unsigned char pad_to_30[0x30]; VECTOR dirX; };
        struct { unsigned char pad_to_40[0x40]; int particleIndex; };
        struct { unsigned char pad_to_44[0x44]; int scaleBase; };
        struct { unsigned char pad_to_48[0x48]; int i; };
        struct { unsigned char pad_to_4C[0x4C]; SVECTOR angleX; };
    } frame;
#pragma pack(pop)

    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.impact = (BattleFxLimitSledFangImpact *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.impact->frameIndex >= 0 && frame.impact->frameIndex <= 2) {
        frame.scaleBase = 0xDDD;
        frame.particleIndex = g_BattleLimitSledFangTrailInitFlag;
        frame.particleTask = (BattleFxSledFangSprites *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSledFangDrawParticlesTask);
        frame.particleTask->particleCount = 8;
        frame.particleTask->firstParticleIndex = *(short *)&frame.particleIndex;
        frame.angleX.vx = 0;
        frame.angleX.vy = g_BattleActorRecords[g_BattleLimitSledFangActorIdx].yaw;
        frame.angleX.vz = 0;
        RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
        for (frame.i = 0; frame.i < 8; frame.i++) {
            frame.dirX.vx = ((rand()) % 4096) - 0x800;
            frame.dirX.vy = ((rand()) % 4096) - 0x800;
            frame.dirX.vz = -(((rand()) % 2048) + 0x800);
            NormalizeVectorFixed(&frame.dirX, &frame.dirX);
            TransformVectorToInt32(&frame.rotMatrix, &frame.dirX, &frame.dirX);
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].active = 0;
            frame.tmpRand = rand() % 0x104;
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].posX = frame.impact->posX + (frame.tmpRand * frame.dirX.vx >> 0xC);
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].posY = frame.impact->posY + (frame.tmpRand * frame.dirX.vy >> 0xC);
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].posZ = frame.impact->posZ + (frame.tmpRand * frame.dirX.vz >> 0xC);
            frame.tmpRand = rand() % 0x78 + 0x122;
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].velocityX = frame.tmpRand * frame.dirX.vx >> 0xC;
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].velocityY = frame.tmpRand * frame.dirX.vy >> 0xC;
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].velocityZ = frame.tmpRand * frame.dirX.vz >> 0xC;
            frame.tmpRand = rand() % 0xC00 + 0xC00;
            ((BattleFxSledFangParticleScale *)&g_BattleLimitSledFangUnusedFlag)[frame.particleIndex].scale = frame.scaleBase * frame.tmpRand >> 0xC;
            frame.particleIndex++;
            if (frame.particleIndex >= 0xDE) {
                frame.particleIndex = 0;
            }
        }
        g_BattleLimitSledFangTrailInitFlag = frame.particleIndex;
    }
    if (frame.impact->frameIndex >= 6) {
        frame.impact->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.impact->frameIndex)++;
}

/*
 * Draws the configured particle count using the age divided by 4 as sprite frame, then advances
 * positions and damps each velocity by its sixteenth when unpaused. The particle index advances
 * only in the unpaused branch, so a paused call repeatedly draws the same first particle. Ages at
 * least 11 retire the slot even while paused; age increments only when unpaused.
 */
// FUNCTION: FF7 0x004eba53
void __cdecl BattleFx_LimitSledFangDrawParticlesTask(void)
{
    int padSlot[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int particle;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int stackWords[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int stackGap[6]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int iter;
    BattleFxSledFangParticle *task;

    task = (BattleFxSledFangParticle *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitSledFangSpriteRequest2.spriteEntry) = Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitSledFangLoadedResTable);
    g_BattleLimitSledFangSpriteRequest2.resource = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitSledFangLoadedResTable);
    g_BattleLimitSledFangSpriteRequest2.frameIndex = task->frameIndex / 4;
    particle = task->firstParticleIndex;
    for (iter = 0; iter < task->particleCount; iter++) {
        BattleFx_InstallBillboardMatrix(&((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].posX, (short)((BattleFxSledFangParticleScale *)&g_BattleLimitSledFangUnusedFlag)[particle].scale, -(((BattleFxSledFangParticleScale *)&g_BattleLimitSledFangUnusedFlag)[particle].scale >> 3));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitSledFangSpriteRequest2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        if (g_BattleIsPaused != 0) {
            continue;
        }
        ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].posX += ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityX;
        ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].posY += ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityY;
        ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].posZ += ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityZ;
        ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityX -= (short)(((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityX >> 4);
        ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityY -= (short)(((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityY >> 4);
        ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityZ -= (short)(((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[particle].velocityZ >> 4);
        particle++;
        if (particle >= 0xDE) {
            particle = 0;
        }
    }
    if (task->frameIndex >= 0xB) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameIndex)++;
}

/*
 * Retires the current effect slot (ownerSlot -1), allocates a slot running
 * BattleFx_LimitBloodFangTimelineTask, stores the bone world position of actor
 * g_BattleLimitSledFangDrawListHandle (bone targetBone) in its worldPos and zeroes its
 * nextTrailIndex, then fills the two quad records g_BattleLimitSledFangQuadPrimitive1
 * (colours 0x88,0x20,0x20, tip colour red 8, frameDelta -0x30, baseValue 0x40) and
 * g_BattleLimitSledFangTrail2 (colours 0xA0,0x28,0x28, tip colour red 8, frameDelta -0x90,
 * baseValue 0x52) and calls BattleFx_LimitSledFangClearTrailRings.
 */
// FUNCTION: FF7 0x004ebc95
void __cdecl BattleFx_LimitBloodFangSpawnTimeline(void)
{
    BattleFxBloodFangTask *task;

    task = (BattleFxBloodFangTask *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = -1;
    task = (BattleFxBloodFangTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBloodFangTimelineTask);
    Battle_GetBoneWorldPos(g_BattleLimitSledFangDrawListHandle, g_BattleActorRecords[g_BattleLimitSledFangDrawListHandle].targetBone,
                           &task->worldPos);
    task->nextTrailIndex = 0;
    g_BattleLimitSledFangQuadPrimitive1.colorR = 0x88;
    g_BattleLimitSledFangQuadPrimitive1.colorG = 0x20;
    g_BattleLimitSledFangQuadPrimitive1.colorB = 0x20;
    g_BattleLimitSledFangQuadPrimitive1.colorARed = 8;
    g_BattleLimitSledFangQuadPrimitive1.colorAGreen = 0;
    g_BattleLimitSledFangQuadPrimitive1.colorABlue = 0;
    g_BattleLimitSledFangQuadPrimitive1.frameDelta = -0x30;
    g_BattleLimitSledFangQuadPrimitive1.baseValue = 0x40;
    g_BattleLimitSledFangTrail2.colorR = 0xA0;
    g_BattleLimitSledFangTrail2.colorG = 0x28;
    g_BattleLimitSledFangTrail2.colorB = 0x28;
    g_BattleLimitSledFangTrail2.colorARed = 8;
    g_BattleLimitSledFangTrail2.colorAGreen = 0;
    g_BattleLimitSledFangTrail2.colorABlue = 0;
    g_BattleLimitSledFangTrail2.frameDelta = -0x90;
    g_BattleLimitSledFangTrail2.baseValue = 0x52;
    BattleFx_LimitSledFangClearTrailRings();
}

/*
 * On unpaused age 12 creates three phase-offset trail callbacks at the actor attachment with Y
 * forced to -380 and initializes their trail records; age 21 creates three more callbacks with
 * different phase and motion parameters. At age 27 samples another attachment, creates a burst at
 * the controller X/Z and sampled Y, caps its size at 1900, retires the controller and dispatches
 * positional sound. The age still increments after retirement.
 */
// FUNCTION: FF7 0x004ebd86
void __cdecl BattleFx_LimitBloodFangTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        BattleFxBloodFangTimeline *timeline;
        struct { unsigned char pad_to_4[0x4]; BattleFxBloodFangRecord *altRecord; };
        struct { unsigned char pad_to_C[0xC]; int trailIndex; };
        struct { unsigned char pad_to_10[0x10]; int phaseIndex; };
        struct { unsigned char pad_to_14[0x14]; int targetBonePos; };
        struct { unsigned char pad_to_16[0x16]; short sampledY; };
    } frame;
#pragma pack(pop)

    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.timeline = (BattleFxBloodFangTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.timeline->step == 0xC) {
        frame.trailIndex = frame.timeline->nextTrailIndex;
        for (frame.phaseIndex = 0; frame.phaseIndex < 3; frame.phaseIndex++) {
            frame.altRecord = (BattleFxBloodFangRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSledFangSlashTrailTask);
            frame.altRecord->angle = frame.phaseIndex * 0x550;
            frame.altRecord->offsetY = 0x78;
            frame.altRecord->offsetZ = 0xFDA8;
            frame.altRecord->trailIndex = *(short *)&frame.trailIndex;
            Battle_GetBoneWorldPos(g_BattleLimitSledFangActorIdx, g_BattleActorRecords[g_BattleLimitSledFangActorIdx].altBone, (short *)&g_BattleLimitSledFangBoneWorldPositions[frame.trailIndex]);
            g_BattleLimitSledFangBoneWorldPositions[frame.trailIndex].posY = 0xFE84;
            BattleFx_LimitSledFangPushTrailPoint(&g_BattleLimitSledFangTrailRingRecords[frame.trailIndex], &g_BattleLimitSledFangBoneWorldPositions[frame.trailIndex]);
            frame.trailIndex++;
        }
        frame.timeline->nextTrailIndex = *(short *)&frame.trailIndex;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x186);
    }
    if (frame.timeline->step == 0x15) {
        frame.trailIndex = frame.timeline->nextTrailIndex;
        for (frame.phaseIndex = 0; frame.phaseIndex < 3; frame.phaseIndex++) {
            frame.altRecord = (BattleFxBloodFangRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBloodFangSlashTrailTask);
            frame.altRecord->angle = frame.phaseIndex * 0x550 + 0x3E8;
            frame.altRecord->offsetY = 0x12C;
            frame.altRecord->offsetZ = 0xFCE0;
            frame.altRecord->trailIndex = *(short *)&frame.trailIndex;
            Battle_GetBoneWorldPos(g_BattleLimitSledFangActorIdx, g_BattleActorRecords[g_BattleLimitSledFangActorIdx].altBone, (short *)&g_BattleLimitSledFangBoneWorldPositions[frame.trailIndex]);
            g_BattleLimitSledFangBoneWorldPositions[frame.trailIndex].posY = 0xFE84;
            BattleFx_LimitSledFangPushTrailPoint(&g_BattleLimitSledFangTrailRingRecords[frame.trailIndex], &g_BattleLimitSledFangBoneWorldPositions[frame.trailIndex]);
            frame.trailIndex++;
        }
        frame.timeline->nextTrailIndex = *(short *)&frame.trailIndex;
    }
    if (frame.timeline->step == 0x1B) {
        Battle_GetBoneWorldPos(g_BattleLimitSledFangActorIdx, g_BattleActorRecords[g_BattleLimitSledFangActorIdx].targetBone, (short *)&frame.targetBonePos);
        frame.altRecord = (BattleFxBloodFangRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSledFangSpawnSpritesAndParticlesTask);
        frame.altRecord->posX = frame.timeline->posX;
        frame.altRecord->posY = frame.sampledY;
        frame.altRecord->posZ = frame.timeline->posZ;
        frame.phaseIndex = g_BattleActorRecords[g_BattleLimitSledFangActorIdx].radius + 0x100;
        if (frame.phaseIndex > 0x76C) {
            frame.phaseIndex = 0x76C;
        }
        frame.altRecord->angle = *(short *)&frame.phaseIndex;
        frame.timeline->slotTag = 0xFFFF;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.altRecord->posX), 0x1A3);
    }
    (frame.timeline->step)++;
}

/*
 * When unpaused, samples the actor attachment at Y -380 and appends two interpolated trail points
 * on the first two ages or four thereafter to a wrapping 24-point trail. Draws up to 15 segments
 * using actor-heading rotation, table offsets, configured width and phase progression even while
 * paused. Ages at least 16 retire before the pause check; after age 6 unpaused calls subtract two
 * from the trail count and advance age.
 */
// FUNCTION: FF7 0x004ec0b2
void __cdecl BattleFx_LimitBloodFangSlashTrailTask(void)
{
    int framePhase;
    int ofsZ;
    int yOff;
    int segment;
    BattleLimitSledFangTrailPoint *pPoints;
    int prevIndex;
    int phaseDelta;
    BattleFxLimitSledFangTrail *pContext;
    BattleLimitSledFangTrailRing *ring;
    int count;
    BattleFxFangSlashTrail *task;
    int heading;
    int readPos;
    SVECTOR *prevPointPtr;

    task = (BattleFxFangSlashTrail *)BATTLE_CURRENT_TASK_SLOT;
    pContext = (BattleFxLimitSledFangTrail *)Gfx_GetPsxScratchpadPtr(0);
    ring = &g_BattleLimitSledFangTrailRingRecords[task->trailIndex];
    pPoints = (BattleLimitSledFangTrailPoint *)&g_BattleLimitSledFangBoneWorldPositions[task->trailIndex];
    if (g_BattleIsPaused == 0) {
        Battle_GetBoneWorldPos(g_BattleLimitSledFangActorIdx, g_BattleActorRecords[g_BattleLimitSledFangActorIdx].altBone, &pContext->edgeAX);
        pContext->originX = 0xFE84;
        readPos = ring->battleLimitSldFangTrailPointIndex - 1;
        if (readPos < 0) {
            readPos = 0x17;
        }
        *(Int64Pair *)&pContext->edgeBX = *(Int64Pair *)&pPoints[readPos];
        if (task->frame < 2) {
            pPoints[ring->battleLimitSldFangTrailPointIndex].posX = pContext->edgeAX + pContext->edgeBX >> 1;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posZ = 0xFE84;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posY = pContext->edgeAY + pContext->edgeBY >> 1;
            BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
        } else {
            pContext->midX = pContext->edgeAX + pContext->edgeBX >> 1;
            pContext->originY = 0xFE84;
            pContext->midY = pContext->edgeAY + pContext->edgeBY >> 1;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posX = pContext->midX + pContext->edgeBX >> 1;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posZ = 0xFE84;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posY = pContext->midY + pContext->edgeBY >> 1;
            BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
            *(Int64Pair *)&pPoints[ring->battleLimitSldFangTrailPointIndex] = *(Int64Pair *)&pContext->midX;
            BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
            pPoints[ring->battleLimitSldFangTrailPointIndex].posX = pContext->midX + pContext->edgeAX >> 1;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posZ = 0xFE84;
            pPoints[ring->battleLimitSldFangTrailPointIndex].posY = pContext->midY + pContext->edgeAY >> 1;
            BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
        }
        *(Int64Pair *)&pPoints[ring->battleLimitSldFangTrailPointIndex] = *(Int64Pair *)&pContext->edgeAX;
        BattleFx_LimitSledFangPushTrailPoint(ring, &pPoints[ring->battleLimitSldFangTrailPointIndex]);
    }
    framePhase = task->baseValue + task->frame * g_BattleLimitSledFangTrail2.frameDelta;
    heading = g_BattleActorRecords[g_BattleLimitSledFangActorIdx].yaw;
    phaseDelta = g_BattleLimitSledFangTrail2.baseValue;
    pContext->rotX = 0;
    pContext->trailIndex = heading;
    pContext->tableOffset = 0;
    RotMatrix((SVECTOR *)&pContext->rotX, (int *)&pContext->rotMatrix);
    TransformVectorToInt32(&pContext->rotMatrix, g_BattleLimitSledFangVector1, &pContext->dirX);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    SetDrawColorRGB(g_BattleLimitSledFangTrail2.colorR, g_BattleLimitSledFangTrail2.colorG, g_BattleLimitSledFangTrail2.colorB);
    pContext->quadA = g_BattleLimitSledFangTrail2.colorA;
    pContext->colorA = g_BattleLimitSledFangTrail2.colorA;
    yOff = task->offsetY;
    ofsZ = task->offsetZ;
    count = ring->headIndex - 1;
    if (count > 0xF) {
        count = 0xF;
    }
    readPos = ring->battleLimitSldFangTrailPointIndex - 1;
    if (readPos < 0) {
        readPos = 0x17;
    }
    *(Int64Pair *)&pContext->posX = *(Int64Pair *)ring->points[readPos];
    pContext->posX += (short)(ofsZ * pContext->dirX >> 0xC);
    pContext->posY += (short)(ofsZ * pContext->dirY >> 0xC);
    pContext->posZ += (short)(ofsZ * pContext->dirZ >> 0xC);
    BattleFx_LimitSledFangProjectTrailEdgePoints(pContext, &pContext->posX, (GfxPsxShortVertexSlot *)&pContext->posX);
    for (segment = 1; segment <= count; readPos--, segment++) {
        if (readPos < 0) {
            readPos = 0x17;
        }
        prevIndex = readPos - 1;
        if (prevIndex < 0) {
            prevIndex = 0x17;
        }
        framePhase += phaseDelta;
        pContext->posX = 0;
        pContext->posY = -(*(int *)(g_BattleLimitSledFangOffsetTable2 + (segment * 8 - 4)) + yOff);
        pContext->posZ = ofsZ;
        pContext->tableOffset = framePhase - *(int *)(g_BattleLimitSledFangOffsetTable2 + (segment * 8 - 8));
        RotMatrix((SVECTOR *)&pContext->rotX, (int *)&pContext->rotMatrix);
        TransformVectorToInt16(&pContext->rotMatrix, &pContext->posX, &pContext->tipX);
        pContext->tableOffset = framePhase + *(int *)(g_BattleLimitSledFangOffsetTable2 + (segment * 8 - 8));
        RotMatrix((SVECTOR *)&pContext->rotX, (int *)&pContext->rotMatrix);
        TransformVectorToInt16(&pContext->rotMatrix, &pContext->posX, &pContext->posX);
        prevPointPtr = (struct SVECTOR *)ring->points[prevIndex];
        pContext->posX += prevPointPtr->vx;
        pContext->posY += prevPointPtr->vy;
        pContext->posZ += prevPointPtr->vz;
        pContext->tipX += prevPointPtr->vx;
        pContext->tipY += prevPointPtr->vy;
        pContext->tipZ += prevPointPtr->vz;
        pContext->prevValueA = pContext->valueA;
        pContext->prevValueB = pContext->valueB;
        pContext->prevVertexA = pContext->vertexA;
        pContext->prevVertexB = pContext->vertexB;
        pContext->prevDepth = pContext->depth;
        pContext->quadB = pContext->quadA;
        SetRotMatrix(&g_BattleCameraTransform);
        SetTransMatrix(&g_BattleCameraTransform);
        RefreshGlobalTransformState();
        BattleFx_LimitSledFangProjectTrailEdgePoints(pContext, &pContext->posX, (GfxPsxShortVertexSlot *)&pContext->tipX);
        BattleFx_LimitSledFangDrawTrailSegmentQuad(pContext, count - segment, count);
    }
    if (task->frame >= 0x10) {
        task->slotTag = 0xFFFF;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame > 6) {
        ring->headIndex -= 2;
    }
    (task->frame)++;
}

/*
 * On unpaused ages 1 through 4 allocates four flattened moving sprite callbacks with random
 * actor-heading-rotated positions and scales; ages 0 through 2 independently allocate an
 * eleven-particle callback and populate eleven records in the wrapping 222-entry particle array.
 * Ages at least 14 retire the controller before its age increment.
 */
// FUNCTION: FF7 0x004ec7fc
void __cdecl BattleFx_LimitSledFangSpawnSpritesAndParticlesTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxSledFangSpawnRecord *spawnRecord; };
        struct { unsigned char pad_to_24[0x24]; BattleFxSledFangSprites *particleTask; };
        struct { unsigned char pad_to_28[0x28]; BattleFxLimitSledFangSpriteSlot *spriteTask; };
        struct { unsigned char pad_to_2C[0x2C]; int tmpRand; };
        struct { unsigned char pad_to_30[0x30]; int spawnRadius; };
        struct { unsigned char pad_to_34[0x34]; VECTOR dirX; };
        struct { unsigned char pad_to_44[0x44]; int particleIndex; };
        struct { unsigned char pad_to_48[0x48]; int scaleBase; };
        struct { unsigned char pad_to_4C[0x4C]; int i; };
        struct { unsigned char pad_to_50[0x50]; SVECTOR angleX; };
        struct { unsigned char pad_to_58[0x58]; VECTOR spriteDirX; };
    } frame;
#pragma pack(pop)

    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spawnRecord = (BattleFxSledFangSpawnRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.spawnRecord->stage >= 1 && frame.spawnRecord->stage <= 4) {
        frame.scaleBase = 0x2333;
        frame.angleX.vx = 0;
        frame.angleX.vy = g_BattleActorRecords[g_BattleLimitSledFangActorIdx].yaw;
        frame.angleX.vz = 0;
        RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
        TransformVectorToInt32(&frame.rotMatrix, (int *)&g_BattleLimitSledFangVector2, &frame.dirX);
        frame.spawnRadius = 0x600;
        for (frame.i = 0; frame.i < 4; frame.i++) {
            frame.spriteTask = (BattleFxLimitSledFangSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSledFangFlatMovingSpriteTask);
            frame.spriteDirX.vx = ((rand()) % 4096) - 0x800;
            frame.spriteDirX.vy = 0;
            frame.spriteDirX.vz = ((rand()) % 2048);
            NormalizeVectorFixed(&frame.spriteDirX, &frame.spriteDirX);
            TransformVectorToInt32(&frame.rotMatrix, &frame.spriteDirX, &frame.spriteDirX);
            frame.tmpRand = (frame.spawnRadius >> 1) + rand() % frame.spawnRadius;
            frame.spriteTask->posX = frame.spawnRecord->posX + (frame.tmpRand * frame.spriteDirX.vx >> 0xC);
            frame.spriteTask->posY = 0xFFB0;
            frame.spriteTask->posZ = frame.spawnRecord->posZ + (frame.tmpRand * frame.spriteDirX.vz >> 0xC);
            frame.tmpRand = rand() % 0xA0 + 0x64;
            frame.spriteTask->velocityX = frame.tmpRand * frame.dirX.vx >> 0xC;
            frame.spriteTask->velocityY = 0;
            frame.spriteTask->velocityZ = frame.tmpRand * frame.dirX.vz >> 0xC;
            frame.tmpRand = rand() % 0xC00 + 0xA00;
            frame.spriteTask->scale = frame.scaleBase * frame.tmpRand >> 0xC;
        }
    }
    if (frame.spawnRecord->stage >= 0 && frame.spawnRecord->stage <= 2) {
        frame.scaleBase = 0xDDD;
        frame.particleIndex = g_BattleLimitSledFangTrailInitFlag;
        frame.particleTask = (BattleFxSledFangSprites *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitSledFangDrawParticlesTask);
        frame.particleTask->particleCount = 0xB;
        frame.particleTask->firstParticleIndex = *(short *)&frame.particleIndex;
        frame.angleX.vx = 0;
        frame.angleX.vy = g_BattleActorRecords[g_BattleLimitSledFangActorIdx].yaw;
        frame.angleX.vz = 0;
        RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
        for (frame.i = 0; frame.i < 0xB; frame.i++) {
            frame.dirX.vx = ((rand()) % 4096) - 0x800;
            frame.dirX.vy = ((rand()) % 4096) - 0x800;
            frame.dirX.vz = -(((rand()) % 2048) + 0x800);
            NormalizeVectorFixed(&frame.dirX, &frame.dirX);
            TransformVectorToInt32(&frame.rotMatrix, &frame.dirX, &frame.dirX);
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].active = 0;
            frame.tmpRand = rand() % 0x104;
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].posX = frame.spawnRecord->posX + (frame.tmpRand * frame.dirX.vx >> 0xC);
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].posY = frame.spawnRecord->posY + (frame.tmpRand * frame.dirX.vy >> 0xC);
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].posZ = frame.spawnRecord->posZ + (frame.tmpRand * frame.dirX.vz >> 0xC);
            frame.tmpRand = rand() % 0x78 + 0x122;
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].velocityX = frame.tmpRand * frame.dirX.vx >> 0xC;
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].velocityY = frame.tmpRand * frame.dirX.vy >> 0xC;
            ((BattleFxSledFangParticleRecord *)&g_BattleLimitSledFangParticles)[frame.particleIndex].velocityZ = frame.tmpRand * frame.dirX.vz >> 0xC;
            frame.tmpRand = rand() % 0xC00 + 0xC00;
            ((BattleFxSledFangParticleScale *)&g_BattleLimitSledFangUnusedFlag)[frame.particleIndex].scale = frame.scaleBase * frame.tmpRand >> 0xC;
            frame.particleIndex++;
            if (frame.particleIndex >= 0xDE) {
                frame.particleIndex = 0;
            }
        }
        g_BattleLimitSledFangTrailInitFlag = frame.particleIndex;
    }
    if (frame.spawnRecord->stage >= 0xE) {
        frame.spawnRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.spawnRecord->stage)++;
}

/*
 * Draws an age-selected sprite using a quarter-turn X rotation followed by actor heading and scale
 * with Y doubled. Retires ages at least 7 before checking pause. Unpaused calls advance X/Z, damp
 * their velocities by a sixteenth and increment age; paused calls still draw and can retire.
 */
// FUNCTION: FF7 0x004ecc66
void __cdecl BattleFx_LimitSledFangFlatMovingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[136];
        PsxMatrix spriteMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxSledFangSprite *sprite; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR angleX; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix headingMatrix; };
        struct { unsigned char pad_to_78[0x78]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxSledFangSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitSledFangSpriteRequest1.spriteEntry) = Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitSledFangLoadedResTable);
    g_BattleLimitSledFangSpriteRequest1.resource = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitSledFangLoadedResTable);
    g_BattleLimitSledFangSpriteRequest1.step = frame.sprite->step;
    frame.angleX.vx = 0x400;
    frame.angleX.vy = 0;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.spriteMatrix);
    frame.spriteMatrix.t[0] = frame.sprite->posX;
    frame.spriteMatrix.t[1] = frame.sprite->heightY;
    frame.spriteMatrix.t[2] = frame.sprite->posZ;
    frame.angleX.vx = 0;
    frame.angleX.vy = g_BattleActorRecords[g_BattleLimitSledFangActorIdx].yaw;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.headingMatrix);
    ConcatMatrixInPlace(&frame.headingMatrix, &frame.spriteMatrix);
    frame.scaleX.vz = frame.sprite->scale;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = frame.scaleX.vx << 1;
    ScaleMatrix(&frame.spriteMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.spriteMatrix, &frame.spriteMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitSledFangSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.sprite->step >= 7) {
        frame.sprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sprite->posX += frame.sprite->velocityX;
    frame.sprite->posZ += frame.sprite->velocityZ;
    frame.sprite->velocityX -= (short)(frame.sprite->velocityX >> 4);
    frame.sprite->velocityZ -= (short)(frame.sprite->velocityZ >> 4);
    (frame.sprite->step)++;
}
} /* extern "C" */

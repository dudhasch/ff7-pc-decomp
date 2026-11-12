// src/ff7/src/battle/battle_fx_limit_toy_soldier.cpp
// Limit Break [58] (Cait Sith Slots): Toy Soldier (Japanese: トイソルジャー, dev tag: "LIMIT/TOYDAT.DAT" / Toy)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Toy limit-break effect: begins on the targets through the shared Mog begin path, then
//   runs a timeline that moves global effect states and an animated sprite task over them.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008ff170
struct BattleFxLimitToySoldierSpriteQuad g_BattleFxLimitToySoldierSpriteQuad = {{0xf0, 0xff, 0xf0, 0xff}, 0, {0x20, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3c, 0, 0x87, 0x7a}, 0};
// GLOBAL: FF7 0x008ff188
unsigned char g_BattleFxLimitToySoldierAnimStateTable[24] = {0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008ff1a0
unsigned char g_BattleFxLimitToySoldierOffsetTable[24] = {0x38, 0xff, 0xff, 0xff, 0x92, 0xff, 0xff, 0xff, 0xec, 0xff, 0xff, 0xff, 0x46, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0xfa, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008ff1b8
struct BattleFxLimitToySoldierTransformMatrix g_BattleFxLimitToySoldierTransformMatrix = {{0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0x10}, 0, 0, 0, {0}};
// GLOBAL: FF7 0x00c3f2d8
struct Int64Pair g_BattleFxLimitToySoldierSourcePos = {0};
// GLOBAL: FF7 0x00c3f2e0
short g_BattleFxLimitToySoldierTargetPos[3] = {0};
// GLOBAL: FF7 0x00c3f2e8
int g_BattleFxLimitToySoldierTargetMask = 0;
// GLOBAL: FF7 0x00c3f2ec
int g_BattleFxLimitToySoldierActorIdx = 0;
// GLOBAL: FF7 0x00c3f2f0
struct BattleFxLimitToySoldierLocalMatrix g_BattleFxLimitToySoldierLocalMatrix = {0};
// GLOBAL: FF7 0x00c3f310
int g_BattleFxLimitToySoldierWorldMatrix = 0;
unsigned char g_bssPad_00c3f314[4] = {0};
unsigned char g_bssPad_00c3f318[24] = {0};
// clang-format on


#define g_BattleHitLivewireEmitterRecords ((unsigned char *)g_BattleTaskSlotBuffer.mainSlots)

/*
 * Runs BattleFx_LimitBeginToySoldier on the targets and returns 0.
 */
// FUNCTION: FF7 0x005d7720
int BattleFx_LimitEnterToySoldier(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginToySoldier(targetMask, actorIdx);
    return 0;
}

/*
 * Like BattleFx_LimitBeginMogDance, but the second task slot runs BattleFx_LimitToySoldierSpawnTimeline. Stores
 * the target mask and actor index in g_BattleFxLimitToySoldierTargetMask and g_BattleFxLimitToySoldierActorIdx.
 */
// FUNCTION: FF7 0x005d7737
void BattleFx_LimitBeginToySoldier(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr =
        &g_BattleHitLivewireEmitterRecords[Battle_AllocateResourceSlot(BattleFx_SelectDrawListBuffer) * 0x20];
    Battle_AllocateResourceSlot(BattleFx_LimitToySoldierSpawnTimeline);
    g_BattleFxLimitToySoldierTargetMask = targetMask;
    g_BattleFxLimitToySoldierActorIdx = actorIdx;
}

/*
 * Immediately retires the current slot, obtains the selected actor bone position and masked actor
 * bounding midpoint, and derives a fixed-point heading. Allocates a BattleFx_LimitToySoldierTimelineTask
 * child, copies the source position, offsets X/Z by sine/cosine times 330, sets Y zero, installs a
 * heading and X/Z velocity from sine/cosine times 45, and builds a globally stored XYZ rotation
 * matrix scaled equally by 16384. The written Z operands in the heading expression dereference
 * the Z components of the source and target positions.
 */
// FUNCTION: FF7 0x005d7778
void __cdecl BattleFx_LimitToySoldierSpawnTimeline(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[32];
        BattleFxLimitToySoldierSpawnSlot *toySlot;
        struct { unsigned char pad_to_4[0x4]; int heading; };
        struct { unsigned char pad_to_8[0x8]; SVECTOR rotX; };
        struct { unsigned char pad_to_10[0x10]; VECTOR scaleVector; };
    } frame;
#pragma pack(pop)

    frame.toySlot = (BattleFxLimitToySoldierSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.toySlot->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleFxLimitToySoldierActorIdx, g_BattleActorRecords[g_BattleFxLimitToySoldierActorIdx].targetBone, (short *)&g_BattleFxLimitToySoldierSourcePos);
    Battle_GetActorMaskCentroid(g_BattleFxLimitToySoldierTargetMask, (short *)&g_BattleFxLimitToySoldierTargetPos);
    frame.heading = ArcTangentFixed(*(short *)&g_BattleFxLimitToySoldierSourcePos - *(short *)&g_BattleFxLimitToySoldierTargetPos, *(short *)(((unsigned char *)&g_BattleFxLimitToySoldierSourcePos) + 4) - *(short *)((char *)g_BattleFxLimitToySoldierTargetPos + 4));
    frame.heading = 0x1000 - (frame.heading + 0x800);
    frame.toySlot = (BattleFxLimitToySoldierSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToySoldierTimelineTask);
    *(Int64Pair *)((char *)frame.toySlot + 4) = *&g_BattleFxLimitToySoldierSourcePos;
    frame.toySlot->posX += (short)-(SinFixed(frame.heading) * 0x14A >> 0xC);
    frame.toySlot->posY = 0;
    frame.toySlot->posZ += (short)(CosFixed(frame.heading) * 0x14A >> 0xC);
    frame.toySlot->rotationY = 0x800 - frame.heading;
    frame.toySlot->stepX = -(SinFixed(frame.heading) * 0x2D >> 0xC);
    frame.toySlot->stepZ = CosFixed(frame.heading) * 0x2D >> 0xC;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0x800 - frame.heading;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, (int *)&g_BattleFxLimitToySoldierLocalMatrix);
    frame.scaleVector.vz = 0x4000;
    frame.scaleVector.vy = frame.scaleVector.vz;
    frame.scaleVector.vx = frame.scaleVector.vy;
    ScaleMatrix((int *)&g_BattleFxLimitToySoldierLocalMatrix, &frame.scaleVector);
}

/*
 * Unpaused timeline emits kind-one record at four, moves globals during 26..38 and plays sound at
 * 28/33/38. At 58 allocates task/sound, 60 processes selected ten actors with typed
 * records/randomized helper, 70 allocates kind two; counters 75 or more retire, then increment.
 */
// FUNCTION: FF7 0x005d792e
void __cdecl BattleFx_LimitToySoldierTimelineTask(void)
{
    int record;
    int actorIndex;
    BattleLimitToySoldierTimeline *selfPtr;
    int child;

    if (g_BattleIsPaused != 0) {
        return;
    }
    selfPtr = (BattleLimitToySoldierTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (selfPtr->frame == 4) {
        record = Battle_AllocSceneMessage(1);
        *(Int64Pair *)((char *)record + 4) = *(Int64Pair *)((char *)selfPtr + 4);
        *(short *)((char *)record + 0x10) = selfPtr->rotationY;
    }
    if (selfPtr->frame >= 0x1A && selfPtr->frame < 0x27) {
        g_BattleActorRecords[3].posX += selfPtr->velocityX;
        g_BattleActorRecords[3].posZ += selfPtr->velocityZ;
        g_BattleFxLimitToySoldierLocalMatrix.matrixPhase = g_BattleActorRecords[3].posX;
        g_BattleFxLimitToySoldierLocalMatrix.matrixZeroWord = 0;
        g_BattleFxLimitToySoldierLocalMatrix.matrixEmitCounter = g_BattleActorRecords[3].posZ;
    }
    if (selfPtr->frame == 0x1C || (selfPtr->frame == 0x21 || selfPtr->frame == 0x26)) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[3].posX), 0xCC);
    }
    if (selfPtr->frame == 0x3A) {
        child = (int)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToySoldierAnimatedSpritesTask);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[3].posX), 0x1C6);
    }
    if (selfPtr->frame == 0x46) {
        Battle_AllocSceneMessage(2);
    }
    if (selfPtr->frame == 0x3C) {
        for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
            if ((g_BattleFxLimitToySoldierTargetMask & 1 << actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIndex);
                BattleFx_HitMarkSpawnAtActorBone(actorIndex, ((rand() % 2) << 8) | 0x29);
            }
        }
    }
    if (selfPtr->frame >= 0x4B) {
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (selfPtr->frame)++;
}

/*
 * Visits six global effect states after updating the object selector and combined matrix. A state
 * of one randomly becomes a frame-group value from 32 through 192, forced once the slot counter
 * reaches three; this transition also runs while paused. Other nonzero states select a frame from
 * their high bits, set a per-entry translation with fixed -50 and -110 components, and submit a
 * sprite quad with zero input draw chain. Increments those states only while unpaused and clears
 * them once the low nibble reaches two. Retires the slot at counter four or later, including while
 * paused, and advances its counter only while unpaused. The temporary matrix output passed as &x
 * spans separately declared locals and needs native layout evidence.
 */
// FUNCTION: FF7 0x005d7b24
void __cdecl BattleFx_LimitToySoldierAnimatedSpritesTask(void)
{
    int entry;
    BattleTaskSlot *task;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int tmpMatrix[9];
    int coin;

    g_BattleFxLimitToySoldierSpriteQuad.sceneObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    task = BATTLE_CURRENT_TASK_SLOT;
    MultiplyMatrix(&g_BattleCameraTransform, (int *)&g_BattleFxLimitToySoldierLocalMatrix, &g_BattleFxLimitToySoldierWorldMatrix);
    for (entry = 0; entry < 6; entry++) {
        if (((int *)&g_BattleFxLimitToySoldierAnimStateTable)[entry] != 0) {
            if (((int *)&g_BattleFxLimitToySoldierAnimStateTable)[entry] == 1) {
                coin = (rand() % 2);
                if (coin != 0 || task->frameCounter >= 3) {
                    ((int *)&g_BattleFxLimitToySoldierAnimStateTable)[entry] = rand() % 6 + 1 << 5;
                }
            } else {
                g_BattleFxLimitToySoldierSpriteQuad.animState = ((int *)&g_BattleFxLimitToySoldierAnimStateTable)[entry] & 0xFFFFFFF0;
                g_BattleFxLimitToySoldierTransformMatrix.matrixOffsetX = ((int *)&g_BattleFxLimitToySoldierOffsetTable)[entry];
                g_BattleFxLimitToySoldierTransformMatrix.matrixOffsetY = 0xFFFFFFCE;
                g_BattleFxLimitToySoldierTransformMatrix.matrixOffsetZ = 0xFFFFFF92;
                MultiplyMatrix(&g_BattleFxLimitToySoldierWorldMatrix, (int *)&g_BattleFxLimitToySoldierTransformMatrix, tmpMatrix);
                SetRotMatrix((int *)&g_BattleFxLimitToySoldierTransformMatrix);
                SetTransMatrix(tmpMatrix);
                BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleFxLimitToySoldierSpriteQuad, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
                if (g_BattleIsPaused == 0) {
                    (((int *)&g_BattleFxLimitToySoldierAnimStateTable)[entry])++;
                }
                if ((((int *)&g_BattleFxLimitToySoldierAnimStateTable)[entry] & 0xF) >= 2) {
                    ((int *)&g_BattleFxLimitToySoldierAnimStateTable)[entry] = 0;
                }
            }
        }
    }
    if (task->frameCounter >= 4) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameCounter)++;
}
} /* extern "C" */

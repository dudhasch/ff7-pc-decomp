// src/ff7/src/battle/battle_fx_special_gust.cpp
/* Enemy Special Attack [92]: Gust (built-in procedural wind swirl) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the built-in gust: the entry stores the render state block,
//   and the per-target task starts the gust timeline. The timeline draws gust segments, a swirl
//   task moves their endpoints around the target, and a record claim hands out free particle
//   segments.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/battle_tasks.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008ff098
short g_BattleFxGustColorScale = 16;
// The colour table: seven RGB rows, and a zero row after them.
// GLOBAL: FF7 0x008ff0a0
struct BattleFxGustColor g_BattleFxGustColorTable[8] = {{31, 3, 3}, {31, 16, 6}, {31, 31, 11}, {0, 27, 9}, {8, 31, 30}, {4, 15, 29}, {19, 0, 29}, {0}};
// GLOBAL: FF7 0x008ff0d0
short g_BattleFxGustAngleSteps[12] = {0, 8, 4, 2, 6, 1, 3, 5, 7, 0, 0, 0};
// clang-format on

void BattleFx_SpecialBuiltinGustDrawSegment(BattleFxGustSegment *segment);
BattleFxGustSegment *BattleFx_SpecialBuiltinGustClaimSegmentRecord(void);

// clang-format off
// GLOBAL: FF7 0x00c1ea70
BattleFxGustSegment g_BattleFxGustSegmentRecords[200] = {0};
// GLOBAL: FF7 0x00c1fa10
struct BattleFxGustFourColorPacket *g_BattleFxGustResourcePtr = 0;
// GLOBAL: FF7 0x00c1fa14
short g_BattleFxGustActiveFlag = 0;
// clang-format on

/*
 * Points g_BattleFxGustResourcePtr at g_GfxPsxPacketScratch, resets the colour scale to 16 and
 * the active flag to 0, then calls BattleFx_SpecialBeginBuiltinGust with the target mask and actor.
 */
// FUNCTION: FF7 0x005d4f90
void __cdecl BattleFx_SpecialEnterBuiltinGust(unsigned int targetMask, int actingActor)
{
    /* The first store plants an ADDRESS, not a value: taking `&g_GfxPsxPacketScratch`
       is what makes our operand relocated the way the target's is. */
    g_BattleFxGustResourcePtr = (struct BattleFxGustFourColorPacket *)&g_GfxPsxPacketScratch;
    g_BattleFxGustColorScale = 0x10;
    g_BattleFxGustActiveFlag = 0;
    BattleFx_SpecialBeginBuiltinGust(targetMask, actingActor);
}

/*
 * Starts the gust effect's per-target task BattleFx_SpecialSpawnBuiltinGustPerTarget with a 4-frame delay and clears the
 * in-use flag of all 200 records of g_BattleFxGustSegmentRecords.
 */
// FUNCTION: FF7 0x005d4fc1
void BattleFx_SpecialBeginBuiltinGust(unsigned int targetMask, int actingActor)
{
    int i;

    Battle_CreateBitPatternCallbackTask(targetMask, actingActor, 4,
                                 BattleFx_SpecialSpawnBuiltinGustPerTarget);
    for (i = 0; i < 200; i++) {
        g_BattleFxGustSegmentRecords[i].active = 0;
    }
}

/*
 * Allocates a BattleFx_SpecialBuiltinGustTimelineTask record, stores the target bone's world
 * position (posA, posB, posC) and the actor index in it, then plays sound 0xA3 panned to the actor.
 */
// FUNCTION: FF7 0x005d500c
void __cdecl BattleFx_SpecialSpawnBuiltinGustPerTarget(int actor)
{
    BattleFxBuiltinGustRecord *taskRecord;

    taskRecord = (BattleFxBuiltinGustRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBuiltinGustTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskRecord->posA);
    taskRecord->actorIndex = actor;
    Battle_PlaySfxPannedToActor(0xA3, actor, 0x40);
}

/*
 * Invokes draw/helper before pause guard. Unpaused even counters zero through fourteen emit
 * randomized child using table-dependent height and sine parameters. Counter 50 allocates typed
 * actor, counters above 44 decrement global, then advances and retires at resulting 60 while
 * setting completion global one.
 */
// FUNCTION: FF7 0x005d5072
void BattleFx_SpecialBuiltinGustTimelineTask(void)
{
    BattleFxBuiltinGustRecord *task;
    short phase;
    BattleFxGustSwirlSlot *swirl;

    task = (BattleFxBuiltinGustRecord *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialBuiltinGustDrawActiveSegments();
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step >= 0 && task->step < 16 &&
        task->step % 2 == 0) {
        swirl = (BattleFxGustSwirlSlot *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_SpecialBuiltinGustSwirlTask)];
        phase = g_BattleFxGustAngleSteps[task->step >> 1] << 7;
        swirl->posX = task->posA;
        swirl->posY = task->posB + phase + rand() % 100 - 562;
        swirl->posZ = task->posC;
        swirl->radius = (SinFixed(phase << 1) * 0x258 >> 12) + 200;
        swirl->radiusVelocity = (SinFixed(phase << 1) * 5 >> 12) + 5;
        swirl->angle = rand() % 0x1000;
    }
    if (task->step == 50) {
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
    }
    if (task->step > 44) {
        g_BattleFxGustColorScale--;
    }
    (task->step)++;
    if (task->step == 60) {
        g_BattleFxGustActiveFlag = 1;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * g_BattleFxGustSegmentRecords holds 200 segments whose first member is the in-use flag:
 * BattleFx_SpecialBeginBuiltinGust clears every flag,
 * BattleFx_SpecialBuiltinGustClaimSegmentRecord claims the first clear one, and this walks the
 * claimed ones.
 */
// FUNCTION: FF7 0x005d521c
void BattleFx_SpecialBuiltinGustDrawActiveSegments(void)
{
    int i;

    for (i = 0; i < 200; i++) {
        if (g_BattleFxGustSegmentRecords[i].active != 0) {
            BattleFx_SpecialBuiltinGustDrawSegment(&g_BattleFxGustSegmentRecords[i]);
        }
    }
}

/*
 * Builds four segment endpoints with vertical plus/minus 50, projects and draws only positive
 * helper result using two cyclic palette rows scaled by global intensity. Unpaused increases
 * counter two and clears active at sixteen. Paused still projects/draws.
 */
// FUNCTION: FF7 0x005d5266
void BattleFx_SpecialBuiltinGustDrawSegment(BattleFxGustSegment *segment)
{
    short bottomDisp;
    int outPosX;
    int colorCode;
    short topOfs;
    short vertA[3];
    BattleFxGustFourColorPacket *pPacket;
    int outBits;
    short lowerEnd[3];
    int res;
    int padSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short upperEnd[3];
    short pointHighA[3];
    unsigned char rgbA[3];
    unsigned char rgbB[3];

    colorCode = 0x3A000000;
    topOfs = 0x32;
    bottomDisp = 50;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    vertA[0] = segment->x1;
    vertA[1] = segment->y1 - topOfs;
    vertA[2] = segment->z1;
    lowerEnd[0] = segment->x2;
    lowerEnd[1] = segment->y2 - bottomDisp;
    lowerEnd[2] = segment->z2;
    pointHighA[0] = segment->x1;
    pointHighA[1] = segment->y1 + topOfs;
    pointHighA[2] = segment->z1;
    upperEnd[0] = segment->x2;
    upperEnd[1] = segment->y2 + bottomDisp;
    upperEnd[2] = segment->z2;
    pPacket = g_BattleFxGustResourcePtr;
    res = Gfx_TransformQuadReturnDepth(&vertA, (struct SVECTOR *)&lowerEnd, (struct SVECTOR *)&pointHighA, (struct SVECTOR *)&upperEnd, &outPosX, &outBits);
    if (res > 0) {
        rgbA[0] = (unsigned char)(g_BattleFxGustColorTable[segment->colorPhase % 7].colorR * g_BattleFxGustColorScale >> 1);
        rgbA[1] = (unsigned char)(g_BattleFxGustColorTable[segment->colorPhase % 7].colorG * g_BattleFxGustColorScale >> 1);
        rgbA[2] = (unsigned char)(g_BattleFxGustColorTable[segment->colorPhase % 7].colorB * g_BattleFxGustColorScale >> 1);
        rgbB[0] = (unsigned char)(g_BattleFxGustColorTable[(segment->colorPhase + 1) % 7].colorR * g_BattleFxGustColorScale >> 1);
        rgbB[1] = (unsigned char)(g_BattleFxGustColorTable[(segment->colorPhase + 1) % 7].colorG * g_BattleFxGustColorScale >> 1);
        rgbB[2] = (unsigned char)(g_BattleFxGustColorTable[(segment->colorPhase + 1) % 7].colorB * g_BattleFxGustColorScale >> 1);
        pPacket->packetTag = 0x8000000;
        pPacket->color2 = colorCode | rgbA[0] | (rgbA[1] << 8) | (rgbA[2] << 16);
        pPacket->color0 = pPacket->color2;
        pPacket->color3 = colorCode | rgbB[0] | (rgbB[1] << 8) | (rgbB[2] << 16);
        pPacket->color1 = pPacket->color3;
        Gfx_DrawPsxPrimitive(pPacket);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    segment->colorPhase += 2;
    if (segment->colorPhase >= 0x10) {
        segment->active = 0;
    }
}

/*
 * Unpaused performs two radial steps, computes old/new XZ endpoints while advancing angle 256 and
 * radius by velocity, and allocates optional segment records tagged one then zero. Allocation
 * failure still advances trajectory. Completion global retires parent after both steps; no local
 * lifetime counter.
 */
// FUNCTION: FF7 0x005d552d
void __cdecl BattleFx_SpecialBuiltinGustSwirlTask(void)
{
    BattleFxGustSegment *segment;
    BattleFxGustSwirl *taskPtr;
    short i;
    short nextZ;
    short startX;
    short previousZ;
    short xNew;

    taskPtr = (BattleFxGustSwirl *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        startX = CosFixed(taskPtr->angle) * taskPtr->radius >> 12;
        previousZ = SinFixed(taskPtr->angle) * taskPtr->radius >> 12;
        taskPtr->angle += 0x100 /* 22.5 deg */;
        taskPtr->radius += taskPtr->radiusVelocity;
        xNew = CosFixed(taskPtr->angle) * taskPtr->radius >> 12;
        nextZ = SinFixed(taskPtr->angle) * taskPtr->radius >> 12;
        segment = BattleFx_SpecialBuiltinGustClaimSegmentRecord();
        if (segment != 0) {
            segment->x1 = xNew + taskPtr->centerX;
            segment->y1 = taskPtr->centerY;
            segment->z1 = nextZ + taskPtr->centerZ;
            segment->x2 = startX + taskPtr->centerX;
            segment->y2 = taskPtr->centerY;
            segment->z2 = previousZ + taskPtr->centerZ;
            segment->colorPhase = 1 - i;
        }
    }
    if (g_BattleFxGustActiveFlag != 0) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Takes the first free segment of g_BattleFxGustSegmentRecords (active flag clear), marks it used
 * and returns it; 0 when all 200 are in use.
 */
// FUNCTION: FF7 0x005d56ca
BattleFxGustSegment *BattleFx_SpecialBuiltinGustClaimSegmentRecord(void)
{
    int i;

    for (i = 0; i < 200; i++) {
        if (g_BattleFxGustSegmentRecords[i].active == 0) {
            g_BattleFxGustSegmentRecords[i].active = 1;
            return &g_BattleFxGustSegmentRecords[i];
        }
    }
    return 0;
}

} /* extern "C" */

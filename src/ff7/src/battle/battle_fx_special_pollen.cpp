// src/ff7/src/battle/battle_fx_special_pollen.cpp
/* Enemy Special Effect [idx 62, 123]: Pollen (JP: kahun / 花粉 (Pollen), asset: "SPECIAL/KAHUN") — Pollen (0x204, Gagighandi [idx 62]), Big Pollen (0x290 [idx 62]), Sleep Scales (0x1cb [idx 62]), Confu-scales (0x268 [idx 62]); idx 123 KahunAlt */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Kahun attack: it loads resources and spawns a per-target
//   timeline that drives animated and fading sprites, a particle emitter that claims shared
//   records, PSX particle dots and a fading spinning model, plus an alternate entry point.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdlib.h>
extern "C" {

struct BattleFxPollenParticleRecord;
struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a4338
int g_BattleSpecPollenResourceDefs = 0x001900B1;
int g_BattleSpecPollenResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialPollenResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a4340
int g_BattleSpecPollenResourceModelRows[16] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialPollenResourceNames + 0xc), (int)((char *)&g_BattleFxSpecialPollenResourceNames + 0x18),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialPollenResourceNames + 0x24), (int)((char *)&g_BattleFxSpecialPollenResourceNames + 0x30), 0x0,
    0x0, 0x8, 0x0, 0x0
};
// GLOBAL: FF7 0x008a4380
struct BattleSpecPollenSpriteReq1 g_BattleSpecPollenSpriteReq1 = {{0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a4390
struct BattleSpecHeartBreathSpriteReq1 g_BattleSpecPollenSpriteReq2 = {-128, -128, -128, {0x2e, 0, 0, 0, 0}};
// GLOBAL: FF7 0x008a4398
int g_BattleSpecPollenModelScale = 6656;
// GLOBAL: FF7 0x008a439c
int g_BattleSpecPollenModelStep = 4096;
// GLOBAL: FF7 0x008a43a0
struct BattleSpecHellBubblesSpriteReq3 g_BattleSpecPollenSpriteReq3 = {0x1a00, {0}, 0, {8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a43b4[4] = {0x20, 0x00, 0x00, 0x00};
unsigned char g_BattleFxSpecialPollenResourceNames[60] =
    "SUNA2.RSD\0\0\0"
    "KAFUN03.TIM\0"
    "BAKU1.SPT\0\0\0"
    "KAFUN01.TIM\0"
    "KEMU.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6cc50
int g_BattleSpecPollenDrawListHead = 0;
// GLOBAL: FF7 0x00b6cc54
short g_BattleSpecPollenParticleLoopIndex = 0;
// GLOBAL: FF7 0x00b6cc58
struct BattleSpecPollenParticleRecord g_BattleSpecPollenParticleRecords[2] = {0};
unsigned char g_bssPad_00b6cc68[1584] = {0};
// GLOBAL: FF7 0x00b6d298
struct GfxTableCursor *g_BattleSpecPollenLoadedResTable = 0;
unsigned char g_bssPad_00b6d29c[4] = {0};
unsigned char g_bssPad_00b6d2a0[16] = {0};
// clang-format on

/*
 * Loads the SPECIAL/KAHUN resource table (type 3, g_BattleSpecPollenResourceDefs) when the battle scene
 * exists and no table is loaded yet.
 */
// FUNCTION: FF7 0x005426e0
void __cdecl BattleFx_SpecialLoadResPollen(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecPollenLoadedResTable == 0) {
        kind = 3;
        resourceDefsWord = (int)&g_BattleSpecPollenResourceDefs;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a43f4
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KAHUN", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecPollenLoadedResTable, newSceneField);
    }
}

/*
 * Entry point of the Kahun special: selects bone offset 8 (the caster bone the effect starts at), points the
 * draw list at g_GfxPsxPacketScratch, loads the resources, spawns a timeline per target and plays the sound.
 */
// FUNCTION: FF7 0x00542770
void __cdecl BattleFx_SpecialEnterPollen(int targetMask, int actorIdx)
{
    (*(short *)&g_BattleSpecPollenResourceModelRows[13]) = 8;
    g_BattleSpecPollenDrawListHead = (int)&g_GfxPsxPacketScratch;
    g_BattleSpecPollenParticleLoopIndex = 0;
    BattleFx_SpecialLoadResPollen();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnPollenPerTarget);
    Battle_PlaySfxPannedToActor(0x69, (short)actorIdx, 0x40);
}

/*
 * Spawns the timeline task of one target: its start is the world position of the caster's bone picked by
 * the offset in g_BattleSpecPollenResourceModelRows[13], its end the target's targetBone position. It also
 * marks all 200 particle records inactive.
 */
// FUNCTION: FF7 0x005427be
void __cdecl BattleFx_SpecialSpawnPollenPerTarget(int actor, int sourceIndex)
{
    short idx;
    BattleFxSpecialPollenTask *slotPtr;

    slotPtr = (BattleFxSpecialPollenTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPollenTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, (&g_BattleActorRecords[sourceIndex].targetBone)[*(short *)&g_BattleSpecPollenResourceModelRows[13]], &slotPtr->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slotPtr->endX);
    slotPtr->actorIndex = actor;
    for (idx = 0; idx < 200; idx++) {
        g_BattleSpecPollenParticleRecords[idx].active = 0;
    }
}

/*
 * Timeline of one Kahun hit: starts the particle emitter and the spinning model at the caster, a fade-in/out
 * sprite, and animated sprites around the target, and ends after 50 frames.
 */
// FUNCTION: FF7 0x0054286e
void __cdecl BattleFx_SpecialPollenTimelineTask(void)
{
    short heading;
    short diffZ;
    short xDelta;
    short dist;
    BattleFxSpecialPollenTask *pTimeline;
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short i;
    BattleFxPollenTaskRecord *spawned;

    pTimeline = (BattleFxSpecialPollenTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pTimeline->timer == 0) {
        heading = ArcTangentFixed(pTimeline->endX - pTimeline->startX, pTimeline->endZ - pTimeline->startZ);
        spawned = (BattleFxPollenTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPollenParticleEmitterTask);
        spawned->posX = pTimeline->startX;
        spawned->posY = pTimeline->posY;
        spawned->posZ = pTimeline->startZ;
        spawned->stepX = (pTimeline->endX - pTimeline->startX) / 20;
        spawned->angle = 0;
        spawned->stepZ = (pTimeline->endZ - pTimeline->startZ) / 20;
        spawned->stage = heading;
        spawned->growth = 0;
        spawned = (BattleFxPollenTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPollenFadingSpinModelTask);
        spawned->posX = pTimeline->startX;
        spawned->posY = pTimeline->posY;
        spawned->posZ = pTimeline->startZ;
        spawned->stepZ = 1024;
        spawned->angle = heading - 0x400;
        xDelta = pTimeline->endX - pTimeline->startX;
        diffZ = pTimeline->endZ - pTimeline->startZ;
        dist = SqrtToInt(xDelta * xDelta + diffZ * diffZ);
        spawned->scaleValue = dist * 0x1500 / 2800;
        spawned->growth = 0;
    }
    if (pTimeline->timer == 0) {
        spawned = (BattleFxPollenTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPollenFadeInOutSpriteTask);
        spawned->posX = 0;
        spawned->posY = 0xFF9C;
        spawned->posZ = 2000;
        spawned->scaleValue = 0xF00 /* 0.9375 */;
        spawned->stage = 6;
        spawned->fadeTimer = 0;
    }
    if (pTimeline->timer == 40) {
        Battle_PostActorHitOrHurtMessage(pTimeline->actorIndex);
    }
    if (pTimeline->timer > 32 && pTimeline->timer < 44) {
        for (i = 0; i < 2; i++) {
            spawned = (BattleFxPollenTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPollenAnimatedSpriteTask);
            spawned->posX = pTimeline->endX + rand() % 1400 + -700;
            spawned->posY = pTimeline->startY + rand() % 1400 + -700;
            spawned->posZ = pTimeline->endZ + rand() % 1400 + -700;
        }
    }
    (pTimeline->timer)++;
    if (pTimeline->timer == 50) {
        pTimeline->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one animated sprite (entry 1 of g_BattleSpecPollenLoadedResTable) as a billboard at the task's
 * position at scale 0x1800 and steps its frame; the task ends at the animation's frame count.
 */
// FUNCTION: FF7 0x00542bae
void __cdecl BattleFx_SpecialPollenAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecPollenResourceModelRows[14] = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecPollenLoadedResTable);
    g_BattleSpecPollenResourceModelRows[15] = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecPollenLoadedResTable);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800 /* 1.5 */, 0);
    g_BattleSpecPollenSpriteReq1.frame = task->frameCounter;
    g_BattleSpecPollenDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecPollenResourceModelRows[14], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecPollenDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecPollenResourceModelRows[14] != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= ((BattleFxSpriteGroup *)g_BattleSpecPollenResourceModelRows[14])->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the fading sprite (entry 2 of g_BattleSpecPollenLoadedResTable) scaled about the task's position,
 * fading it in over 8 frames and out from frame 0x25; the task ends at frame 0x2D.
 */
// FUNCTION: FF7 0x00542c7f
void __cdecl BattleFx_SpecialPollenFadeInOutSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        short m00;
        struct { unsigned char pad_to_2[0x2]; short m01; };
        struct { unsigned char pad_to_4[0x4]; short m02; };
        struct { unsigned char pad_to_6[0x6]; short m10; };
        struct { unsigned char pad_to_8[0x8]; short m11; };
        struct { unsigned char pad_to_A[0xA]; short m12; };
        struct { unsigned char pad_to_C[0xC]; short m20; };
        struct { unsigned char pad_to_E[0xE]; short m21; };
        struct { unsigned char pad_to_10[0x10]; short m22; };
        struct { unsigned char pad_to_12[0x12]; int transX; };
        struct { unsigned char pad_to_16[0x16]; int transY; };
        struct { unsigned char pad_to_1A[0x1A]; int transZ; };
        struct { unsigned char pad_to_20[0x20]; BattleFxPollenFadeInOutSpriteSlot *kahunSlot; };
        struct { unsigned char pad_to_24[0x24]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.kahunSlot = (BattleFxPollenFadeInOutSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.m00 = 0x1000;
    frame.m01 = 0;
    frame.m02 = 0;
    frame.m10 = 0;
    frame.m11 = 0x1000;
    frame.m12 = 0;
    frame.m20 = 0;
    frame.m21 = 0;
    frame.m22 = 0x1000;
    memset(&frame.transX, 0, 12);
    g_BattleSpecPollenSpriteReq1.spriteRecord = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecPollenLoadedResTable);
    g_BattleSpecPollenSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecPollenLoadedResTable);
    frame.scaleX.vz = frame.kahunSlot->uniformScale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.m00, &frame.scaleX);
    frame.transX = frame.kahunSlot->posX;
    frame.transY = frame.kahunSlot->posY;
    frame.transZ = frame.kahunSlot->posZ;
    SetRotMatrix(&frame.m00);
    SetTransMatrix(&frame.m00);
    g_BattleSpecPollenSpriteReq2.blue = frame.kahunSlot->fadeTimer * 12;
    g_BattleSpecPollenSpriteReq2.green = g_BattleSpecPollenSpriteReq2.blue;
    (g_BattleSpecPollenSpriteReq2.red) = g_BattleSpecPollenSpriteReq2.green;
    g_BattleSpecPollenDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecPollenSpriteReq1.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecPollenDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.kahunSlot->posX += frame.kahunSlot->fadeStep;
    frame.kahunSlot->posY += frame.kahunSlot->fadeStep;
    if (frame.kahunSlot->frameCounter < 8) {
        (frame.kahunSlot->fadeTimer)++;
    }
    if (frame.kahunSlot->frameCounter > 37) {
        (frame.kahunSlot->fadeTimer)--;
    }
    (frame.kahunSlot->frameCounter)++;
    if (frame.kahunSlot->frameCounter == 45) {
        frame.kahunSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the particle dots, claims 8 new particle records per frame for the first 24 frames and ages the
 * records; the task ends once no record is active.
 */
// FUNCTION: FF7 0x00542e4a
void __cdecl BattleFx_SpecialPollenParticleEmitterTask(void)
{
    BattleFxPollenParticleRecord *taskPtr;
    short i, alive;
    BattleFxClaimedParticleRecord *claimed;

    taskPtr = (BattleFxPollenParticleRecord *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialPollenDrawPsxParticleDots(taskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskPtr->angle += 8;
    if (taskPtr->age > 0 && taskPtr->age < 25) {
        for (i = 0; i < 8; i++) {
            claimed = (BattleFxClaimedParticleRecord *)BattleFx_SpecialPollenClaimParticleRecord();
            if (claimed != 0) {
                claimed->radius = (rand() % 512);
                claimed->angle = rand() & 0xFFF;
            }
        }
    }
    i = 0;
    for (alive = 0; i < 200; i++) {
        if (g_BattleSpecPollenParticleRecords[i].active != 0) {
            alive++;
            g_BattleSpecPollenParticleRecords[i].angle -= 0x80;
            (g_BattleSpecPollenParticleRecords[i].age)++;
            if (g_BattleSpecPollenParticleRecords[i].age == 25) {
                g_BattleSpecPollenParticleRecords[i].active = 0;
            }
        }
    }
    (taskPtr->age)++;
    if (taskPtr->age > 25 && alive == 0) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Claims the first inactive record of g_BattleSpecPollenParticleRecords: marks it active, resets its age and
 * returns its address; returns 0 when all 200 are in use.
 */
// FUNCTION: FF7 0x00542fd0
int __cdecl BattleFx_SpecialPollenClaimParticleRecord(void)
{
    short i;

    for (i = 0; i < 200; i++) {
        if (g_BattleSpecPollenParticleRecords[i].active == 0) {
            g_BattleSpecPollenParticleRecords[i].active = 1;
            g_BattleSpecPollenParticleRecords[i].age = 0;
            return (int)&g_BattleSpecPollenParticleRecords[i];
        }
    }
    return 0;
}

/*
 * Draws every active particle record as a PSX dot, placed from its polar radius and angle and its age.
 */
// FUNCTION: FF7 0x00543035
void __cdecl BattleFx_SpecialPollenDrawPsxParticleDots(BattleFxPollenParticleRecord *particleRecord)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        int dotPhase;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotA; };
        struct { unsigned char pad_to_C[0xC]; int dotDepth; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR dotX; };
        struct { unsigned char pad_to_18[0x18]; short activeCount; };
        struct { unsigned char pad_to_1C[0x1C]; BattleFxPollenDotBatchHead *dotPacket; };
        struct { unsigned char pad_to_20[0x20]; int vertexDepth; };
        struct { unsigned char pad_to_24[0x24]; short dotLevel; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_48[0x48]; short particleIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int screenPoint; };
    } frame;
#pragma pack(pop)

    frame.rotA.vz = 0;
    frame.rotA.vx = frame.rotA.vz;
    frame.rotA.vy = particleRecord->particleAngle;
    RotMatrix(&frame.rotA, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    frame.particleIndex = 0;
    for (frame.activeCount = 0; frame.particleIndex < 200; frame.particleIndex++) {
        if (g_BattleSpecPollenParticleRecords[frame.particleIndex].active != 0) {
            frame.activeCount++;
            frame.dotX.vx = CosFixed(g_BattleSpecPollenParticleRecords[frame.particleIndex].angle) * g_BattleSpecPollenParticleRecords[frame.particleIndex].radius * g_BattleSpecPollenParticleRecords[frame.particleIndex].age / 20 * (particleRecord->angle + 256) >> 20;
            frame.dotX.vy = SinFixed(g_BattleSpecPollenParticleRecords[frame.particleIndex].angle) * g_BattleSpecPollenParticleRecords[frame.particleIndex].radius * g_BattleSpecPollenParticleRecords[frame.particleIndex].age / 20 * (particleRecord->angle + 256) >> 20;
            frame.dotX.vz = 0;
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.dotX, (short *)&frame.dotX, &frame.dotDepth);
            frame.dotX.vx += (short)(particleRecord->baseA + particleRecord->ampA * g_BattleSpecPollenParticleRecords[frame.particleIndex].age);
            frame.dotX.vy += (short)(particleRecord->baseB + particleRecord->ampB * g_BattleSpecPollenParticleRecords[frame.particleIndex].age);
            frame.dotX.vz += (short)(particleRecord->baseC + particleRecord->ampC * g_BattleSpecPollenParticleRecords[frame.particleIndex].age);
            SetRotMatrix(&g_BattleCameraTransform);
            SetTransMatrix(&g_BattleCameraTransform);
            RefreshGlobalTransformState();
            frame.dotPacket = (BattleFxPollenDotBatchHead *)g_BattleSpecPollenDrawListHead;
            frame.dotPacket->tag = frame.dotPacket->tag & 0xFFFFFF | 0x2000000;
            frame.dotPacket->command = 0x68;
            frame.dotPacket->command = 0x6A;
            frame.vertexDepth = Gfx_TransformVertexSlot0ReturnDepth(&frame.dotX, &frame.screenPoint, &frame.dotDepth);
            if (g_BattleSpecPollenParticleRecords[frame.particleIndex].age < 17) {
                frame.dotPhase = 8;
            } else {
                frame.dotPhase = 0x19 - g_BattleSpecPollenParticleRecords[frame.particleIndex].age;
            }
            frame.dotLevel = *(short *)&frame.dotPhase;
            frame.dotPacket->colorR = frame.dotLevel * 0x68 >> 3;
            frame.dotPacket->colorG = frame.dotLevel * 0x68 >> 3;
            frame.dotPacket->colorB = UNFOLDED(0) >> 3;
            Gfx_DrawPsxPrimitive(frame.dotPacket);
        }
    }
}

/*
 * Draws the spinning, growing Kahun model, fading it in over 8 frames and out from frame 0x25; the task ends
 * at frame 0x2D.
 */
// FUNCTION: FF7 0x005432ee
void __cdecl BattleFx_SpecialPollenFadingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxPollenFadingSpinModelSlot *taskSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxPollenFadingSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecPollenSpriteReq3.modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleSpecPollenLoadedResTable);
    frame.taskSlot->rotX = -frame.taskSlot->frameCounter << 7;
    RotMatrix((SVECTOR *)&frame.taskSlot->rotX, &frame.rotMatrix);
    g_BattleSpecPollenModelStep = frame.taskSlot->modelStep;
    (g_BattleSpecPollenSpriteReq3.modelScale) = (frame.taskSlot->angle + 0x100) * 0x1A00 >> 8;
    g_BattleSpecPollenModelScale = (g_BattleSpecPollenSpriteReq3.modelScale);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecPollenModelScale);
    frame.rotMatrix.t[0] = frame.taskSlot->posX;
    frame.rotMatrix.t[1] = frame.taskSlot->posY;
    frame.rotMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecPollenSpriteReq3.alpha = g_BattleSpecPollenParticleLoopIndex << 4;
    g_BattleSpecPollenDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecPollenSpriteReq3.modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecPollenDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.taskSlot->angle += 8;
    if (frame.taskSlot->frameCounter < 8) {
        g_BattleSpecPollenParticleLoopIndex++;
    }
    if (frame.taskSlot->frameCounter > 37) {
        g_BattleSpecPollenParticleLoopIndex--;
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 45) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Alternate entry point of the Kahun special: like BattleFx_SpecialEnterPollen, but the caster bone offset is
 * set to 9 after the per-target tasks are created.
 */
// FUNCTION: FF7 0x0054348e
void __cdecl BattleFx_SpecialEnterPollenAlt(int targetMask, int actorIdx)
{
    g_BattleSpecPollenDrawListHead = (int)&g_GfxPsxPacketScratch;
    g_BattleSpecPollenParticleLoopIndex = 0;
    BattleFx_SpecialLoadResPollen();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnPollenPerTarget);
    Battle_PlaySfxPannedToActor(0x69, (short)actorIdx, 0x40);
    (*(short *)&g_BattleSpecPollenResourceModelRows[13]) = 9;
}
} /* extern "C" */

// src/ff7/src/battle/battle_fx_special_spider_web.cpp
/* Enemy Special Effect [idx 2]: SpiderWeb (JP: ito / 糸 (Thread/Web), asset: "SPECIAL/ITO") — Spider Web (0x35e, Palmer), Silk (0x122, Grashtrike), Spider Attack (0x20a, Gi Nattak), Thread (0x269) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Ito attack: loads resources, spawns per-target timelines,
//   and drives string trail line records, looping and variant web sprites, and orbiting particle
//   trails.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {


// clang-format off
// GLOBAL: FF7 0x008a80c8
int g_BattleSpecialSpiderWebResourceDefs = 0x00192A39;
char *g_BattleFxSpecialSpiderWebResourceNames[1] = {
    // STRING: FF7 0x008a814c
    "HIT1.TIM"
};
unsigned int g_BattleFxSpecialSpiderWebResourceNames2[18] = {
    // STRING: FF7 0x008a8158
    (unsigned int)"HIT1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8164
    (unsigned int)"HIT1.TIM",
    // STRING: FF7 0x008a8170
    (unsigned int)"HIT2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a817c
    (unsigned int)"JO_C03.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8188
    (unsigned int)"ITO1.TIM",
    // STRING: FF7 0x008a8194
    (unsigned int)"ITO1.SPT",
    0x00000000,
    0x00000000
};
#undef g_BattleSpecialSpiderWebSpriteData
// GLOBAL: FF7 0x008a8118
struct BattleSpecialSpiderWebSpriteData g_BattleSpecialSpiderWebSpriteData = {1, {0}, 0, 0, {0x60, 0x60, 0x60, 0x2c}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0};
unsigned char g_rawData_008a8144[4] = {0x01, 0x00, 0x00, 0x00};
unsigned char g_rawData_008a8148[4] = {0x02, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b71898
unsigned char g_BattleSpecialSpiderWebLineRecords[8] = {0};
unsigned char g_bssPad_00b718a0[4] = {0};
// GLOBAL: FF7 0x00b718a4
int g_BattleSpecialSpiderWebLineVertices = 0;
unsigned char g_bssPad_00b718a8[3984] = {0};
// GLOBAL: FF7 0x00b72838
int g_BattleSpecialSpiderWebDrawListHandle = 0;
// GLOBAL: FF7 0x00b7283c
struct GfxTableCursor *g_BattleSpecialSpiderWebLoadedRes = 0;
// clang-format on

/*
 * Loads resources for the Ito special effect when not already loaded.
 */
// FUNCTION: FF7 0x00559ac0
void __cdecl BattleFx_SpecialLoadResSpiderWeb(void)
{
    GfxModelTableBank *value;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSpiderWebLoadedRes == 0) {
        kind = 4;
        resourceDefsWord = (int)&g_BattleSpecialSpiderWebResourceDefs;
        value = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a81a0
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/ITO", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialSpiderWebLoadedRes, value);
    }
}

/*
 * Main entry point for the Ito special effect.
 */
// FUNCTION: FF7 0x00559b50
void __cdecl BattleFx_SpecialEnterSpiderWeb(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSpiderWeb();
    g_BattleSpecialSpiderWebDrawListHandle = (int)&g_GfxPsxPacketScratch;
    BattleFx_SpecialBeginSpiderWeb(targetMask, actorIdx);
}

/*
 * Initializes tasks and spawns per-target effects for Ito.
 */
// FUNCTION: FF7 0x00559b74
void __cdecl BattleFx_SpecialBeginSpiderWeb(int targetMask, int actorIdx)
{
    short i;

    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnSpiderWebPerTarget);
    Battle_PlaySfxPannedToActor(0x6A, (short)actorIdx, 0x40);
    for (i = 0; i < 0xC8; i++) {
        ((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[i].active = 0;
    }
}

/*
 * Spawns timeline task for a single Ito target.
 */
// FUNCTION: FF7 0x00559bd6
void __cdecl BattleFx_SpecialSpawnSpiderWebPerTarget(int actor, int actorIndex)
{
    BattleFxSpiderWebTaskSlot *taskPtr;

    taskPtr = (BattleFxSpiderWebTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpiderWebTimelineTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].bone33, (short *)((char *)taskPtr + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((char *)taskPtr + 0xC));
    taskPtr->targetIndex = actor;
    if (taskPtr->frameCount > taskPtr->frameLimit) {
        (g_BattleSpecialSpiderWebSpriteData.spriteIndex) = 0xFFFF;
    }
    taskPtr = (BattleFxSpiderWebTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpiderWebLineRecordsTask);
}

/*
 * Manages and updates active line and string trail records for Ito.
 */
// FUNCTION: FF7 0x00559c82
void __cdecl BattleFx_SpecialSpiderWebLineRecordsTask(void)
{
    BattleTaskSlot *taskPtr;
    short recIndex, brightness, numActive;
    int raw;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (taskPtr->frameCounter < 0x20) {
        raw = 0x80;
    } else {
        raw = 0x28 - taskPtr->frameCounter << 4;
    }
    brightness = raw;
    BattleFx_SpecialSpiderWebDrawLineRecords((short)brightness);
    if (g_BattleIsPaused != 0) {
        return;
    }
    recIndex = 0;
    for (numActive = 0; recIndex < 0xC8; recIndex++) {
        if (((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[recIndex].active != 0) {
            numActive++;
            (((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[recIndex].timer)++;
            if (((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[recIndex].timer == 0xA) {
                ((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[recIndex].active = 0;
            }
        }
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter == 0x28) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders 3D transformed line segments for Ito string records.
 */
// FUNCTION: FF7 0x00559d9c
void __cdecl BattleFx_SpecialSpiderWebDrawLineRecords(short trailWidth)
{
    short idx;
    int scratch;
    BattleFxSpiderWebLinePrim *primPtr;
    int visibleCnt;
    int localDepthOut[2];

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (idx = 0; idx < 0xC8; idx++) {
        if (((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[idx].active != 0) {
            primPtr = (BattleFxSpiderWebLinePrim *)g_BattleSpecialSpiderWebDrawListHandle;
            primPtr->tag = primPtr->tag & 0xFFFFFF | 0x3000000;
            primPtr->primCode = 0x40;
            primPtr->primCode = 0x42;
            Gfx_TransformVertexSlot0ReturnDepth(&((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[idx].startVertex, &scratch, localDepthOut);
            visibleCnt = Gfx_TransformVertexSlot1ReturnDepth((GfxPsxShortVertexSlot *)(idx * 0x14 + (char *)&g_BattleSpecialSpiderWebLineVertices), &scratch, localDepthOut);
            if (visibleCnt > 0) {
                primPtr->trailNew = trailWidth >> 1;
                primPtr->trailMid = primPtr->trailNew;
                primPtr->trailOld = primPtr->trailMid;
                Gfx_DrawPsxPrimitive(primPtr);
            }
        }
    }
}

/*
 * Drives the main timeline and sprite generation for Ito.
 */
// FUNCTION: FF7 0x00559ea2
void __cdecl BattleFx_SpecialSpiderWebTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        struct { unsigned char pad_to_4[0x4]; SVECTOR offsetX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxSpiderWebTimelineSpawnSlot *spriteSlot; };
        struct { unsigned char pad_to_34[0x34]; short spriteIndex; };
        struct { unsigned char pad_to_38[0x38]; short heading; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxSpiderWebTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_4C[0x4C]; int bonePos; };
        struct { unsigned char pad_to_4E[0x4E]; short targetY; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxSpiderWebTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spriteSlot = (BattleFxSpiderWebTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpiderWebLoopingSpriteTask);
        frame.spriteSlot->velB = 0;
        frame.spriteSlot->velA = 0;
        frame.heading = ArcTangentFixed(frame.timelineSlot->targetX - frame.timelineSlot->posA, frame.timelineSlot->targetZ - frame.timelineSlot->posC);
        frame.spriteSlot->velB = frame.heading;
        frame.offsetX.vy = 0;
        frame.offsetX.vx = frame.offsetX.vy;
        frame.offsetX.vz = 0x190;
        RotMatrix((SVECTOR *)((int)frame.spriteSlot + 0xC), &frame.rotMatrix);
        TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.offsetX.vx);
        frame.timelineSlot->posA += frame.offsetX.vx;
        frame.spriteSlot->posA = frame.timelineSlot->posA;
        frame.timelineSlot->posB += frame.offsetX.vy;
        frame.spriteSlot->posB = frame.timelineSlot->posB;
        frame.timelineSlot->posC += frame.offsetX.vz;
        frame.spriteSlot->posC = frame.timelineSlot->posC;
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].altBone, (short *)&frame.bonePos);
        frame.heading = frame.targetY - 0xC8;
        for (frame.spriteIndex = 0; frame.spriteIndex < 0x14; frame.spriteIndex++) {
            frame.spriteSlot = (BattleFxSpiderWebTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpiderWebOrbitTrailTask);
            frame.spriteSlot->posA = frame.timelineSlot->posA;
            frame.spriteSlot->posB = frame.timelineSlot->posB;
            frame.spriteSlot->posC = frame.timelineSlot->posC;
            frame.spriteSlot->velA = (frame.timelineSlot->targetX - frame.spriteSlot->posA) / 0xA;
            frame.spriteSlot->velB = (frame.heading / 0x14 * frame.spriteIndex - frame.spriteSlot->posB) / 0xA;
            frame.spriteSlot->velC = (frame.timelineSlot->targetZ - frame.spriteSlot->posC) / 0xA;
            if ((frame.spriteIndex % 2) == 0) {
                frame.spriteSlot->angle = (rand() % 512) - 0x100;
                frame.spriteSlot->spin = (g_BattleSpecialSpiderWebSpriteData.spriteIndex) * 0x180;
            } else {
                frame.spriteSlot->angle = (rand() % 512) + 0x700;
                frame.spriteSlot->spin = (g_BattleSpecialSpiderWebSpriteData.spriteIndex) * 0xFFFFFE80;
            }
            frame.spriteSlot->radius = 0;
            frame.spriteSlot->radiusScale = (SinFixed(frame.spriteIndex * 0x66) * 0x258 >> 0xC) + (SinFixed(frame.spriteIndex * 0x320) * 0xC8 >> 0xC) + 0x258;
        }
    }
    if (frame.timelineSlot->frameCounter == 0x24) {
        BattleFx_HitMarkSpawnAtActorBone(frame.timelineSlot->actorIndex, 0x33);
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    if (frame.timelineSlot->frameCounter > 0x10 && frame.timelineSlot->frameCounter < 0x1F) {
        frame.spriteSlot = (BattleFxSpiderWebTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSpiderWebVariantSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, (short *)((char *)frame.spriteSlot + 4));
        frame.spriteSlot->posA += (short)(rand() % 0x3E8 - 0x1F4);
        frame.spriteSlot->posB += (short)(rand() % 0x3E8 - 0x1F4);
        frame.spriteSlot->posC += (short)(rand() % 0x3E8 - 0x1F4);
        frame.spriteSlot->spin = (rand() % 2);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0x32) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders looping web sprites for Ito.
 */
// FUNCTION: FF7 0x0055a2f8
void __cdecl BattleFx_SpecialSpiderWebLoopingSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSpiderWebSpriteData.itoSpriteDataTable8 = Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecialSpiderWebLoadedRes);
    g_BattleSpecialSpiderWebSpriteData.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecialSpiderWebLoadedRes);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x3000, 0);
    g_BattleSpecialSpiderWebSpriteData.itoSpriteDataFrameIndex20 = task->frameCounter % *(int *)((char *)*(int *)g_BattleSpecialSpiderWebSpriteData.itoSpriteDataTable8 + 4);
    g_BattleSpecialSpiderWebDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSpiderWebSpriteData.itoSpriteDataTable8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSpiderWebDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 0xA) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders variant string sprites for Ito.
 */
// FUNCTION: FF7 0x0055a3c6
void __cdecl BattleFx_SpecialSpiderWebVariantSpriteTask(void)
{
    BattleFxSpiderWebVariantSprite *task;

    task = (BattleFxSpiderWebVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x2000, 0);
    g_BattleSpecialSpiderWebSpriteData.itoVariantFrameWord36 = task->frameIndex;
    g_BattleSpecialSpiderWebSpriteData.itoVariantSpriteTable24 = Gfx_ModelTableGetLoadedRecord((&g_BattleSpecialSpiderWebSpriteData.itoVariantRequestTable40)[task->requestIndex], g_BattleSpecialSpiderWebLoadedRes);
    if (task->requestIndex != 2) {
        g_BattleSpecialSpiderWebSpriteData.variantDrawObj = Gfx_ModelTableGetSpriteObject((&g_BattleSpecialSpiderWebSpriteData.itoVariantRequestTable40)[task->requestIndex], g_BattleSpecialSpiderWebLoadedRes);
    } else {
        g_BattleSpecialSpiderWebSpriteData.variantDrawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecialSpiderWebDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSpiderWebSpriteData.itoVariantSpriteTable24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSpiderWebDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialSpiderWebSpriteData.itoVariantSpriteTable24 != 0) {
        (task->frameIndex)++;
        if (task->frameIndex == *(int *)((char *)*(int *)g_BattleSpecialSpiderWebSpriteData.itoVariantSpriteTable24 + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates orbiting particle trails and emits string vertices for Ito.
 */
// FUNCTION: FF7 0x0055a4d4
void __cdecl BattleFx_SpecialSpiderWebOrbitTrailTask(void)
{
    int quad;
    BattleFxSpiderWebOrbit *orbit;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    orbit = (BattleFxSpiderWebOrbit *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    quad = BattleFx_SpecialSpiderWebClaimQuadRecord();
    if (quad != 0) {
        BattleFx_SpecialSpiderWebGetOrbitOffsetPoint(orbit, (Vec3sPacked *)(quad + 4));
    }
    if (orbit->step < 0xA) {
        orbit->centerX += orbit->velocityX;
        orbit->centerY += orbit->velocityY;
        orbit->centerZ += orbit->velocityZ;
        orbit->radius = SinFixed(orbit->step * 0xC8) * orbit->radiusScale >> 0xC;
    } else {
        orbit->angle += orbit->angularVelocity;
        orbit->centerY -= 5;
    }
    if (orbit->step > 0x14) {
        orbit->radius += (short)(orbit->radius / 64);
    }
    if (quad != 0) {
        BattleFx_SpecialSpiderWebGetOrbitOffsetPoint(orbit, (Vec3sPacked *)(quad + 0xC));
    }
    (orbit->step)++;
    if (orbit->step == 0x28) {
        orbit->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Allocates an available string record slot for Ito.
 */
// FUNCTION: FF7 0x0055a62c
int __cdecl BattleFx_SpecialSpiderWebClaimQuadRecord(void)
{
    short i;

    for (i = 0; i < 0xC8; i++) {
        if (((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[i].active == 0) {
            ((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[i].active = 1;
            ((BattleSpecialSpiderWebLineRecord *)(g_BattleSpecialSpiderWebLineRecords))[i].timer = 0;
            return i * 0x14 + (int)&(((BattleSpecialSpiderWebLineRecords *)&g_BattleSpecialSpiderWebLineRecords)->recordBase);
        }
    }
    return 0;
}

/*
 * Computes 3D position offset for orbiting Ito particles.
 */
// FUNCTION: FF7 0x0055a698
void __cdecl BattleFx_SpecialSpiderWebGetOrbitOffsetPoint(BattleFxSpiderWebOrbit *orbit, Vec3sPacked *outPoint)
{
    outPoint->vx = orbit->centerX + (CosFixed(orbit->angle) * orbit->radius >> 0xC);
    outPoint->vy = orbit->centerY;
    outPoint->vz = orbit->centerZ + (SinFixed(orbit->angle) * orbit->radius >> 0xC);
}
} /* extern "C" */

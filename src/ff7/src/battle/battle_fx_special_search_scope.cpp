// src/ff7/src/battle/battle_fx_special_search_scope.cpp
/* Enemy Special Effect [idx 75]: SearchScope (JP: scorp / スコープ / サーチアイ (Guard Scorpion Search Scope, SARCHI2.RSD), asset: "SPECIAL/SCORP") — Search Scope (0x11c, Guard Scorpion) */
// Contents: Battle special effect for the Scorp attack: it loads resources, begins the effect for a
//   target and actor, and runs a timeline that drives a weaving model draw and a two-stage animated
//   sprite task.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

struct Vec3sPacked;
struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x008a44c8
int g_BattleSpecialSearchScopeResourceDefs = 0x001900B1;
int g_BattleSpecialSearchScopeResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialSearchScopeResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a44d0
int g_BattleSpecialSearchScopeResourceTable[20] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialSearchScopeResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x19aa39, (int)((char *)&g_BattleFxSpecialSearchScopeResourceNames + 0x1c), (int)((char *)&g_BattleFxSpecialSearchScopeResourceNames + 0x28), 0x0,
    0x0, 0x19aa39, (int)((char *)&g_BattleFxSpecialSearchScopeResourceNames + 0x34), (int)((char *)&g_BattleFxSpecialSearchScopeResourceNames + 0x40), 0x0, 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x008a4520
unsigned char g_BattleSpecialSearchScopeColor[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a4528
struct BattleSpecialSearchScopeModelReq g_BattleSpecialSearchScopeModelReq = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
unsigned char g_BattleFxSpecialSearchScopeResourceNames[76] =
    "SARCHI2.RSD\0"
    "SARCHI33.RSD\0\0\0\0"
    "HIT1.TIM\0\0\0\0"
    "HIT11.SPT\0\0\0"
    "BAKU1.TIM\0\0\0"
    "BAKU11.SPT";
// GLOBAL: FF7 0x00b6d2b8
PsxMatrix g_BattleSpecialSearchScopeLookBasis = {0};
// GLOBAL: FF7 0x00b6d2d8
struct BattleSpecPetrifyCoordData g_BattleSpecialSearchScopeDeltaVec = {0};
// GLOBAL: FF7 0x00b6d2dc
short g_BattleSpecialSearchScopeDeltaZ = 0;
// GLOBAL: FF7 0x00b6d2e0
int g_BattleSpecialSearchScopeDrawListHandle = 0;
// GLOBAL: FF7 0x00b6d2e4
short g_BattleSpecialSearchScopeTargetMask = 0;
unsigned char g_bssPad_00b6d2e8[8] = {0};
// GLOBAL: FF7 0x00b6d2f0
struct GfxTableCursor *g_BattleSpecialSearchScopeLoadedRes = 0;
// GLOBAL: FF7 0x00b6d2f4
short g_BattleSpecialSearchScopeScaleStep = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)



/*
 * Loads resources for the Scorp special effect when not already loaded.
 */
// FUNCTION: FF7 0x00543d40
void __cdecl BattleFx_SpecialLoadResSearchScope(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSearchScopeLoadedRes == 0) {
        kind = 4;
        resourceTable = (int)&g_BattleSpecialSearchScopeResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008a4584
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/SCORP", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleSpecialSearchScopeLoadedRes, stage);
    }
}

/*
 * Initializes and begins the Scorp special effect for target and actor.
 */
// FUNCTION: FF7 0x00543dd0
void __cdecl BattleFx_SpecialEnterSearchScope(short targetMask, int actorId)
{
    short direction[3];
    BattleFxSearchScopeLine *line;

    g_BattleSpecialSearchScopeTargetMask = targetMask;
    BattleFx_SpecialLoadResSearchScope();
    line = (BattleFxSearchScopeLine *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSearchScopeTimelineTask);
    Battle_GetBoneWorldPos(actorId, g_BattleActorRecords[actorId].bone34, &line->startX);
    Battle_GetTargetMaskBoneCentroid(targetMask, (Vec3sPacked *)&line->endX);
    direction[0] = line->endX - line->startX;
    direction[1] = line->endY - line->startY;
    direction[2] = line->endZ - line->startZ;
    Gte_BuildLookBasisFromDirection(direction, (short *)&g_BattleSpecialSearchScopeLookBasis);
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2 = 0;
    Battle_StartFixedPointRamp(0x400, 0xA);
    Battle_PlaySfxPannedToActor(0x62, (short)actorId, 0x40);
}

/*
 * Timeline task for the Scorp special effect, updating animations and states.
 */
// FUNCTION: FF7 0x00543ec7
void __cdecl BattleFx_SpecialSearchScopeTimelineTask(void)
{
    BattleFxSearchScopePath *taskPtr;
    short pathLen;
    short i;
    BattleFxSearchScopeTimeline *weave;

    taskPtr = (BattleFxSearchScopePath *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step == 0) {
        (g_BattleSpecialSearchScopeDeltaVec.deltaX) = taskPtr->endX - taskPtr->startX;
        g_BattleSpecialSearchScopeDeltaVec.deltaY = taskPtr->endY - taskPtr->startY;
        g_BattleSpecialSearchScopeDeltaZ = taskPtr->endZ - taskPtr->startZ;
        pathLen = SqrtToInt((g_BattleSpecialSearchScopeDeltaVec.deltaX) * (g_BattleSpecialSearchScopeDeltaVec.deltaX) + g_BattleSpecialSearchScopeDeltaVec.deltaY * g_BattleSpecialSearchScopeDeltaVec.deltaY + g_BattleSpecialSearchScopeDeltaZ * g_BattleSpecialSearchScopeDeltaZ);
        for (i = 0; i < 4; i++) {
            weave = (BattleFxSearchScopeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSearchScopeDrawWeavingModelTask);
            weave->startX = taskPtr->startX;
            weave->startY = taskPtr->startY;
            weave->startZ = taskPtr->startZ;
            weave->yaw = ArcTangentFixed(taskPtr->endX - taskPtr->startX, taskPtr->endZ - taskPtr->startZ);
            weave->scaledLength = pathLen * 0x1500 / 0xAF0;
            weave->variant = i;
            weave->pathLength = pathLen;
        }
    }
    if (taskPtr->step == 0x20) {
        for (i = 0; i < 0xA; i++) {
            if ((g_BattleEffectScratch.targetMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
    }
    (taskPtr->step)++;
    if (taskPtr->step == 0x28) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        Battle_StartFixedPointRamp(0, 0xA);
    }
}

/*
 * Task to render the weaving model for the Scorp special effect.
 */
// FUNCTION: FF7 0x005440d9
void __cdecl BattleFx_SpecialSearchScopeDrawWeavingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int scaleValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxFrogTwoPhaseAnimatedSpriteSlot *spawnSlot; };
        struct { unsigned char pad_to_38[0x38]; int rotTransOut; };
        struct { unsigned char pad_to_3C[0x3C]; SVECTOR baseAngleX; };
        struct { unsigned char pad_to_44[0x44]; BattleFxSearchScopeDrawWeavingModelSlot *taskSlot; };
        struct { unsigned char pad_to_48[0x48]; short sign; };
        struct { unsigned char pad_to_4C[0x4C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxSearchScopeDrawWeavingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.sign = (frame.taskSlot->variant >> 1) * 2 - 1;
    frame.angleX.vx = SinFixed(frame.taskSlot->frameCounter << 8) / 0x28 * frame.sign;
    frame.baseAngleX.vx = frame.angleX.vx;
    frame.angleX.vy = (SinFixed(frame.taskSlot->frameCounter << 7) >> 4) * frame.sign;
    frame.baseAngleX.vy = frame.angleX.vy;
    frame.angleX.vz = 0;
    frame.baseAngleX.vz = frame.angleX.vz;
    frame.angleX.vx -= 0x400;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSearchScopeLookBasis, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleX.vy = frame.taskSlot->scaledLength;
    frame.scaleX.vz = ((frame.taskSlot->variant % 2) != 0 ? 0x1000 : 0) + 0x3000;
    frame.scaleX.vx = frame.scaleX.vz;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.taskSlot->posX;
    frame.rotMatrix.t[1] = frame.taskSlot->posY;
    frame.rotMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    if ((frame.taskSlot->variant % 2) == 0) {
        (g_BattleSpecialSearchScopeModelReq.modelHandle) = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialSearchScopeLoadedRes);
    } else {
        (g_BattleSpecialSearchScopeModelReq.modelHandle) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSearchScopeLoadedRes);
    }
    if ((frame.taskSlot->variant % 2) == 0) {
        frame.scaleValue = 0x1000 - (g_BattleSpecialSearchScopeScaleStep << 5);
    } else {
        frame.scaleValue = 0x1000 - g_BattleSpecialSearchScopeScaleStep * 0x18;
    }
    g_BattleSpecialSearchScopeModelReq.alpha = *(short *)&frame.scaleValue;
    g_BattleSpecialSearchScopeDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSearchScopeModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSearchScopeDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((frame.taskSlot->variant % 2) == 0) {
        frame.spawnSlot = (BattleFxFrogTwoPhaseAnimatedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSearchScopeTwoStageAnimatedSpriteTask);
        frame.spawnSlot->posX = frame.taskSlot->posX;
        frame.spawnSlot->posY = frame.taskSlot->posY;
        frame.spawnSlot->posZ = frame.taskSlot->posZ;
        frame.pointX.vx = 0;
        frame.pointX.vy = 0;
        frame.pointX.vz = frame.taskSlot->pathLength;
        RotMatrix(&frame.baseAngleX, &frame.rotMatrix);
        ConcatMatrixPreservingTranslation(&g_BattleSpecialSearchScopeLookBasis, &frame.rotMatrix, &frame.rotMatrix);
        frame.rotMatrix.t[0] = frame.spawnSlot->posX;
        frame.rotMatrix.t[1] = frame.spawnSlot->posY;
        frame.rotMatrix.t[2] = frame.spawnSlot->posZ;
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.pointX, &frame.spawnSlot->posX, &frame.rotTransOut);
        frame.spawnSlot->rotX = 0;
        frame.spawnSlot->yaw = frame.taskSlot->yaw;
        frame.spawnSlot->rotZ = 0;
        frame.spawnSlot->phase = 0;
    }
    if (frame.taskSlot->frameCounter < 8) {
        g_BattleSpecialSearchScopeScaleStep++;
    }
    if (frame.taskSlot->frameCounter > 0x18) {
        g_BattleSpecialSearchScopeScaleStep--;
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 0x20) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to update and render two-stage animated sprites for the Scorp effect.
 */
// FUNCTION: FF7 0x00544473
void __cdecl BattleFx_SpecialSearchScopeTwoStageAnimatedSpriteTask(void)
{
    int tbl[2];
    BattleFxSearchScopeSpriteTask *rec;

    rec = (BattleFxSearchScopeSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    tbl[0] = 2;
    tbl[1] = 3;
    BattleFx_InstallBillboardMatrix(&rec->pointX, 0x2400, 0);
    ((BattleFxSearchScopeTwoStageState *)&g_BattleSpecialSearchScopeResourceTable)->resourceField = Gfx_ModelTableGetLoadedRecord(tbl[rec->resourceIndex], g_BattleSpecialSearchScopeLoadedRes);
    ((BattleFxSearchScopeTwoStageState *)&g_BattleSpecialSearchScopeResourceTable)->resourceAlt = Gfx_ModelTableGetSpriteObject(tbl[rec->resourceIndex], g_BattleSpecialSearchScopeLoadedRes);
    *(short *)((char *)&g_BattleSpecialSearchScopeColor + 0x4) = rec->frame;
    g_BattleSpecialSearchScopeDrawListHandle = BattleFx_SubmitSpriteDraw((char *)&g_BattleSpecialSearchScopeResourceTable + 0x48, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSearchScopeDrawListHandle);
    if (g_BattleIsPaused == 0 && ((BattleFxSearchScopeTwoStageState *)&g_BattleSpecialSearchScopeResourceTable)->resourceField != 0) {
        (rec->frame)++;
        if (rec->frame >= *(int *)(**(int **)(int)((char *)&g_BattleSpecialSearchScopeResourceTable + 0x48) + 4)) {
            if (rec->resourceIndex == 0) {
                rec->resourceIndex = 1;
                rec->frame = 0;
            } else {
                rec->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}
} /* extern "C" */

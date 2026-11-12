// src/ff7/src/battle/battle_fx_limit_dynamite.cpp
// Limit Break [31] (Cid Lv 2-2): Dynamite (Japanese: ダイナマイト, dev tag: "LIMIT3/DYNAMITE" / Dynamite)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Dynamite limit break effect: a master timeline drives a ballistic model projectile,
//   spark lines, bouncing sparks, a bone-attached model and a growing explosion model. A helper
//   computes actor bone rotation from animation data.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
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
/* The X and Z rotation of an actor's bone node 0; bone nodes are 0x34 bytes apart. */
#define g_BattleActorRecordsBone0RotX (*(int *)((char *)&g_BattleActorRecords + 0x192))
#define g_BattleActorRecordsBone0RotZ (*(int *)((char *)&g_BattleActorRecords + 0x196))
// clang-format off
// GLOBAL: FF7 0x00809be0
int g_BattleLimitDynamiteResourceDef = 0x001900B4;
int g_BattleLimitDynamiteResourceDefHeadPtr[1] = {
    (int)g_BattleLimitDynamiteParamTable.kayakuRsd
};
int g_BattleLimitDynamiteResourceDefDataTail[14] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)g_BattleLimitDynamiteParamTable.dynamiteRsd, 0x0,
    0x0, 0x0, 0x192a39, (int)g_BattleLimitDynamiteParamTable.pieceTim, (int)g_BattleLimitDynamiteParamTable.pieceSpt, 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x00809c20
int g_BattleLimitDynamiteScale = 3072;
unsigned char g_rawData_00809c24[4] = {0x00, 0x0c, 0x00, 0x00};
unsigned char g_rawData_00809c28[8] = {0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
/* The thrown stick's draw request (KAYAKU.RSD), then the scale vector of the stick in hand. */
typedef struct {
    BattleFxModelDrawHead draw;
    VECTOR handScale;
} BattleLimitDynamiteStickDraw;
// GLOBAL: FF7 0x00809c30
BattleLimitDynamiteStickDraw g_BattleLimitDynamiteModelReq1 = {{0, 0x80, 0, 0, 0x20, 0}, {3072, 3072, 3072, 0}};
// GLOBAL: FF7 0x00809c50
BattleFxModelDrawHead g_BattleLimitDynamiteModelReq2 = {0, 0x80, 0, 0, 0x20, 0};
/* The bouncing pieces' sprite request (B_PIECE), then two words the effect does not read. */
typedef struct {
    BattleFxSpriteRequest sprite;
    int framePad[2];
} BattleLimitDynamitePieceSprite;
// GLOBAL: FF7 0x00809c60
BattleLimitDynamitePieceSprite g_BattleLimitDynamiteSpriteReq = {{0, 0, -128, -128, -128, 44, 0, 0}, {8, 0}};
// GLOBAL: FF7 0x00809c78
struct BattleLimitDynamiteParamTable g_BattleLimitDynamiteParamTable = {{0, 0xa8, 0, 128, 0x20, 0}, "KAYAKU.RSD", "DYNAMITE.RSD", "B_PIECE.TIM", "B_PIECE.SPT"};
/* The fuse tip in the stick's frame: X and Y here, Z in g_BattleLimitDynamiteFuseOffsetZ. */
// GLOBAL: FF7 0x00af1c18
short g_BattleLimitDynamiteState18[2] = {0};
// GLOBAL: FF7 0x00af1c1c
short g_BattleLimitDynamiteFuseOffsetZ = 0;
// GLOBAL: FF7 0x00af1c20
int g_BattleLimitDynamiteDrawListHead = 0;
// GLOBAL: FF7 0x00af1c24
short g_BattleLimitDynamiteTargetActorMask = 0;
/* A spark's vector before its random rotation: X and Y here (zero), Z in g_BattleLimitDynamiteLivewireLength. */
// GLOBAL: FF7 0x00af1c28
short g_BattleLimitDynamiteState28[2] = {0};
// GLOBAL: FF7 0x00af1c2c
short g_BattleLimitDynamiteLivewireLength = 0;
// GLOBAL: FF7 0x00af1c30
struct GfxTableCursor *g_BattleLimitDynamiteLoadedResTable = 0;
unsigned char g_bssPad_00af1c34[4] = {0};
// GLOBAL: FF7 0x00af1c38
short g_BattleLimitDynamiteStickAngles[3] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/* Loads the LIMIT3/DYNAMITE model table once, when the 3D scene exists and the table is not loaded yet. */
// FUNCTION: FF7 0x004b5120
void __cdecl BattleFx_LimitLoadResDynamite(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int tempResDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitDynamiteLoadedResTable == 0) {
        kind = 3;
        tempResDef = (int)&g_BattleLimitDynamiteResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x00809cbc
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("LIMIT3/DYNAMITE", pathText);
        Gfx_LoadModelTableIfAbsent(kind, tempResDef, pRequest, &g_BattleLimitDynamiteLoadedResTable, drawDev);
    }
}

/*
 * Entry point: loads the resources, puts the fuse tip 180 units up the stick, sets the first spark
 * length, points the packet cursor at the PSX packet scratch and starts the effect. Returns zero.
 */
// FUNCTION: FF7 0x004b51b0
int __cdecl BattleFx_LimitEnterDynamite(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResDynamite();
    g_BattleLimitDynamiteState18[0] = 0;
    g_BattleLimitDynamiteState18[1] = 0xFF4C;
    g_BattleLimitDynamiteFuseOffsetZ = 0;
    g_BattleLimitDynamiteState28[0] = 0;
    g_BattleLimitDynamiteState28[1] = 0;
    g_BattleLimitDynamiteLivewireLength = 80;
    g_BattleLimitDynamiteDrawListHead = (int)&g_GfxPsxPacketScratch;
    BattleFx_LimitBeginDynamite(targetMask, actorIdx);
    return 0;
}

/*
 * Reads the rotation of a bone (boneIdx counts from 1) in the actor's current animation frame, from
 * the secondary animation table when the actor uses it, converting degrees to 4096ths of a turn.
 * Leaves zeros when the actor has no model or no animation for the frame.
 */
// FUNCTION: FF7 0x004b520c
void __cdecl BattleFx_LimitDynamiteGetActorBoneRotation(int actorIdx, int boneIdx, short *rotAngles)
{
    BattleModelBoneTable *pBones;
    BattleModelAnimation *anim;
    int s;
    int animFrame;
    int animBank;
    float angle;
    boneIdx--;
    rotAngles[0] = 0;
    rotAngles[1] = 0;
    rotAngles[2] = 0;
    animBank = g_BattleActorRecords[actorIdx].animBank;
    pBones = (BattleModelBoneTable *)(g_BattleActorRecords[actorIdx].model);
    if (pBones != 0) {
        anim = 0;
        animFrame = g_BattleActorRecords[actorIdx].animFrame;
        if (g_BattleActorRecords[actorIdx].useSecondAngleTable != 0) {
            if (animFrame >= 0 && animFrame < pBones->secondaryCount) {
                anim = pBones->secondaryTable[animFrame];
            }
        } else {
            if (animFrame >= 0 && animFrame < pBones->rotationCount) {
                anim = pBones->rotationTable[animFrame];
            }
        }
        if (anim != 0) {
            angle = anim->banks[g_BattleActorRecords[actorIdx].animBank].boneRotations[boneIdx].angleX;
            rotAngles[0] = angle * 11.377778053283691f;
            angle = anim->banks[g_BattleActorRecords[actorIdx].animBank].boneRotations[boneIdx].angleY;
            rotAngles[1] = angle * 11.377778053283691f;
            angle = anim->banks[g_BattleActorRecords[actorIdx].animBank].boneRotations[boneIdx].angleZ;
            rotAngles[2] = angle * 11.377778053283691f;
        }
    }
}

/* Records the target mask and starts the timeline aimed at the targets' centre, for the source actor. */
// FUNCTION: FF7 0x004b538d
void __cdecl BattleFx_LimitBeginDynamite(int targetMask, int actorIdx)
{
    BattleFxDynamiteSlot *slot;

    g_BattleLimitDynamiteTargetActorMask = targetMask;
    slot = (BattleFxDynamiteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDynamiteTimelineTask);
    Battle_GetActorMaskCentroid(targetMask, &slot->targetX);
    slot->actorIndex = actorIdx;
}

/*
 * The Dynamite timeline, unpaused only. Stage 0 loads two sounds; 17 puts the lit stick in the
 * actor's hand; 42 plays the fuse sound; 70 throws the stick from the hand bone to land on the
 * targets' centre in 20 frames; 90 starts the explosion model, 20 bouncing pieces and sound 0xFA;
 * 110 queues a typed record for each target. At 120 the task retires and releases its sounds.
 */
// FUNCTION: FF7 0x004b53d9
void __cdecl BattleFx_LimitDynamiteTimelineTask(void)
{
    int r;
    short i8;
    BattleFxDynamiteSlot *taskPtr;
    BattleFxDynamiteParticle *particle;

    taskPtr = (BattleFxDynamiteSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->stage == 0) {
        SfxLoadIntoBuffer(0x116, 0);
        SfxLoadIntoBuffer(0xFA, 0);
    }
    if (taskPtr->stage == 17) {
        particle = (BattleFxDynamiteParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDynamiteActorBoneModelTask);
        particle->level = taskPtr->actorIndex;
    }
    if (taskPtr->stage == 42) {
        Battle_PlaySfxPannedToActor(0x116, (short)taskPtr->actorIndex, 0x40);
    }
    if (taskPtr->stage == 70) {
        particle = (BattleFxDynamiteParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDynamiteBallisticModelTask);
        Battle_GetBoneWorldPos(taskPtr->actorIndex, g_BattleActorRecords[taskPtr->actorIndex].bone34, &particle->posX);
        particle->angle = ArcTangentFixed(taskPtr->targetX - particle->posX, taskPtr->targetZ - particle->posZ);
        particle->velocityX = (taskPtr->targetX - particle->posX) / 20;
        particle->velocityY = 0xFF38;
        particle->velocityZ = (taskPtr->targetZ - particle->posZ) / 20;
    }
    if (taskPtr->stage == 110) {
        for (i8 = 0; i8 < 10; i8++) {
            if ((g_BattleLimitDynamiteTargetActorMask & 1 << i8) != 0) {
                Battle_PostActorHitOrHurtMessage(i8);
            }
        }
    }
    if (taskPtr->stage == 90) {
        particle = (BattleFxDynamiteParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDynamiteGrowingModelTask);
        particle->posX = taskPtr->targetX;
        particle->posY = 0;
        particle->posZ = taskPtr->targetZ;
        for (i8 = 0; i8 < 20; i8++) {
            particle = (BattleFxDynamiteParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDynamiteBouncingSpriteTask);
            particle->posX = taskPtr->targetX;
            particle->posZ = taskPtr->targetZ;
            particle->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFED4, 50, &particle->velocityX);
            particle->variant = rand() % 3;
        }
        Battle_PlaySfxPannedToActor(0xFA, -1, 0x40);
    }
    (taskPtr->stage)++;
    if (taskPtr->stage == 120) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x116, 0xF);
        Sfx_ReleaseCachedBuffer(0xFA, 0xF);
    }
}

/*
 * Draws the thrown stick (KAYAKU.RSD, table entry 0) black and tumbling, with its fuse sparks.
 * Unpaused, it moves by its velocity, falls (the Y velocity grows by 20 a frame) and retires at
 * frame 20.
 */
// FUNCTION: FF7 0x004b56da
void __cdecl BattleFx_LimitDynamiteBallisticModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_24[0x24]; BattleLimitDynamiteBallistic *ballistic; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.ballistic = (BattleLimitDynamiteBallistic *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitDynamiteModelReq1.draw.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitDynamiteLoadedResTable);
    g_BattleLimitDynamiteStickAngles[2] = -frame.ballistic->frame << 9;
    g_BattleLimitDynamiteStickAngles[1] = frame.ballistic->yawAngle + 0x400;
    RotMatrixXYZ(&g_BattleLimitDynamiteStickAngles, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleLimitDynamiteScale);
    frame.rotMatrix.t[0] = frame.ballistic->posX;
    frame.rotMatrix.t[1] = frame.ballistic->posY;
    frame.rotMatrix.t[2] = frame.ballistic->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitDynamiteDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitDynamiteModelReq1.draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDynamiteDrawListHead);
    BattleFx_LimitDynamiteDrawLivewireLines((Vec3sPacked *)&frame.ballistic->posX, &g_BattleLimitDynamiteStickAngles, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.ballistic->posX += frame.ballistic->velX;
    frame.ballistic->posY += frame.ballistic->velY;
    frame.ballistic->posZ += frame.ballistic->velZ;
    frame.ballistic->velY += 20;
    (frame.ballistic->frame)++;
    if (frame.ballistic->frame == 20) {
        frame.ballistic->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws ten fuse sparks at the fuse tip of a stick at position, turned by rotAngle and then by
 * extraAngle about Y. Each spark is a line from black at the tip to orange at a random direction and
 * length, and an orange dot at the end of another random direction; both are semi-transparent.
 */
// FUNCTION: FF7 0x004b586b
void __cdecl BattleFx_LimitDynamiteDrawLivewireLines(Vec3sPacked *position, void *rotAngle, short extraAngle)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        struct { unsigned char pad_to_2[0x2]; char lineFlagsB; };
        struct { unsigned char pad_to_3[0x3]; char lineFlagsA; };
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; short pointX; };
        struct { unsigned char pad_to_E[0xE]; short pointY; };
        struct { unsigned char pad_to_10[0x10]; short pointZ; };
        struct { unsigned char pad_to_14[0x14]; int projDepth; };
        struct { unsigned char pad_to_18[0x18]; BattleFxLivewireLine *sparkLineB; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; int vertexDepth; };
        struct { unsigned char pad_to_40[0x40]; short sparkIndex; };
        struct { unsigned char pad_to_44[0x44]; int projVertex; };
        struct { unsigned char pad_to_50[0x50]; int shortVertex; };
        struct { unsigned char pad_to_58[0x58]; BattleFxLivewireLine *sparkLineA; };
    } frame;
#pragma pack(pop)

    RotMatrixXYZ(rotAngle, &frame.rotMatrix);
    RotateMatrixY(extraAngle, &frame.rotMatrix);
    frame.rotMatrix.t[0] = position->vx;
    frame.rotMatrix.t[1] = position->vy;
    frame.rotMatrix.t[2] = position->vz;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RefreshGlobalTransformState();
    RotTransPointToInt16(g_BattleLimitDynamiteState18, &frame.pointX, &frame.projDepth);
    for (frame.sparkIndex = 0; frame.sparkIndex < 10; frame.sparkIndex++) {
        frame.angleX.vx = (rand() % 4096);
        frame.angleX.vy = (rand() % 4096);
        frame.angleX.vz = 0;
        RotMatrix(&frame.angleX, &frame.rotMatrix);
        frame.rotMatrix.t[0] = frame.pointX;
        frame.rotMatrix.t[1] = frame.pointY;
        frame.rotMatrix.t[2] = frame.pointZ;
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RefreshGlobalTransformState();
        g_BattleLimitDynamiteLivewireLength = rand() % 100 + 100;
        RotTransPointToInt16(g_BattleLimitDynamiteState28, (short *)&frame.shortVertex, &frame.projDepth);
        SetRotMatrix(&g_BattleCameraTransform);
        SetTransMatrix(&g_BattleCameraTransform);
        RefreshGlobalTransformState();
        frame.sparkLineA = (BattleFxLivewireLine *)g_BattleLimitDynamiteDrawListHead;
        frame.sparkLineA->tag = frame.sparkLineA->tag & 0xFFFFFF | 0x4000000;
        frame.sparkLineA->commandCode = 0x50;
        if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
            frame.sparkLineA->commandCode = frame.sparkLineA->commandCode | 2;
            frame.lineFlagsA = frame.sparkLineA->commandCode;
        } else {
            frame.sparkLineA->commandCode = frame.sparkLineA->commandCode & 0xFFFFFFFD;
            frame.lineFlagsA = frame.sparkLineA->commandCode;
        }
        frame.vertexDepth = Gfx_TransformVertexSlot0ReturnDepth(&frame.pointX, &frame.projVertex, &frame.projDepth);
        if (frame.vertexDepth > 0) {
            frame.sparkLineA->endColorB = 0;
            frame.sparkLineA->colorB = 0;
            frame.sparkLineA->colorG = 0;
            frame.sparkLineA->colorR = 0;
            frame.sparkLineA->endColorG = 0x40;
            frame.sparkLineA->endColorR = 0x80;
            Gfx_TransformVertexSlot1ReturnDepth((GfxPsxShortVertexSlot *)&frame.shortVertex, &frame.projVertex, &frame.projDepth);
            Gfx_DrawPsxPrimitive(frame.sparkLineA);
        }
        frame.angleX.vx = (rand() % 4096);
        frame.angleX.vy = (rand() % 4096);
        frame.angleX.vz = 0;
        RotMatrix(&frame.angleX, &frame.rotMatrix);
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RefreshGlobalTransformState();
        g_BattleLimitDynamiteLivewireLength = rand() % 100 + 100;
        RotTransPointToInt16(g_BattleLimitDynamiteState28, (short *)&frame.shortVertex, &frame.projDepth);
        SetRotMatrix(&g_BattleCameraTransform);
        SetTransMatrix(&g_BattleCameraTransform);
        RefreshGlobalTransformState();
        frame.sparkLineB = (BattleFxLivewireLine *)g_BattleLimitDynamiteDrawListHead;
        frame.sparkLineB->tag = frame.sparkLineB->tag & 0xFFFFFF | 0x2000000;
        frame.sparkLineB->commandCode = 0x68;
        if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
            frame.sparkLineB->commandCode = frame.sparkLineB->commandCode | 2;
            frame.lineFlagsB = frame.sparkLineB->commandCode;
        } else {
            frame.sparkLineB->commandCode = frame.sparkLineB->commandCode & 0xFFFFFFFD;
            frame.lineFlagsB = frame.sparkLineB->commandCode;
        }
        frame.vertexDepth = Gfx_TransformVertexSlot0ReturnDepth(&frame.shortVertex, &frame.projVertex, &frame.projDepth);
        if (frame.vertexDepth > 0) {
            frame.sparkLineB->colorR = 0x80;
            frame.sparkLineB->colorG = 0x40;
            frame.sparkLineB->colorB = 0;
            Gfx_DrawPsxPrimitive(frame.sparkLineB);
        }
    }
}

/*
 * Draws the stick (table entry 0) black in the actor's hand: at the primary bone, turned by the
 * bone's rotation in the current animation frame and the actor's yaw, offset (20, -50, -30) in the
 * bone's frame and scaled by the hand scale. From frame 25 it draws the fuse sparks too. Retires at
 * frame 53.
 */
// FUNCTION: FF7 0x004b5be3
void __cdecl BattleFx_LimitDynamiteActorBoneModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[152];
        int projDepth;
        struct { unsigned char pad_to_4[0x4]; short boneAngleX; };
        struct { unsigned char pad_to_6[0x6]; short boneAngleY; };
        struct { unsigned char pad_to_8[0x8]; short boneAngleZ; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; short bonePosX; };
        struct { unsigned char pad_to_16[0x16]; short bonePosY; };
        struct { unsigned char pad_to_18[0x18]; short bonePosZ; };
        struct { unsigned char pad_to_20[0x20]; PsxMatrix boneMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; BattleLimitBigBrawlTimeline *actorBone; };
        struct { unsigned char pad_to_74[0x74]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_94[0x94]; short yaw; };
    } frame;
#pragma pack(pop)

    frame.actorBone = (BattleLimitBigBrawlTimeline *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitDynamiteModelReq2.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitDynamiteLoadedResTable);
    Battle_GetBoneWorldPos(frame.actorBone->actorIndex, g_BattleActorRecords[frame.actorBone->actorIndex].primaryBone, &frame.bonePosX);
    frame.boneAngleX = *(short *)((char *)&g_BattleActorRecordsBone0RotX + (frame.actorBone->actorIndex * 0x1AEC + g_BattleActorRecords[frame.actorBone->actorIndex].primaryBone * 0x34));
    frame.boneAngleY = *(short *)((char *)&(g_BattleActorRecords[0].bone0Yaw) + (frame.actorBone->actorIndex * 0x1AEC + g_BattleActorRecords[frame.actorBone->actorIndex].primaryBone * 0x34));
    frame.boneAngleZ = *(short *)((char *)&g_BattleActorRecordsBone0RotZ + (frame.actorBone->actorIndex * 0x1AEC + g_BattleActorRecords[frame.actorBone->actorIndex].primaryBone * 0x34));
    frame.yaw = g_BattleActorRecords[frame.actorBone->actorIndex].yaw;
    BattleFx_LimitDynamiteGetActorBoneRotation(frame.actorBone->actorIndex, g_BattleActorRecords[frame.actorBone->actorIndex].primaryBone, &frame.boneAngleX);
    RotMatrixXYZ(&frame.boneAngleX, &frame.boneMatrix);
    RotateMatrixY(frame.yaw, &frame.boneMatrix);
    frame.offsetX.vx = 20;
    frame.offsetX.vy = 0xFFCE;
    frame.offsetX.vz = 0xFFE2;
    frame.boneMatrix.t[0] = frame.bonePosX;
    frame.boneMatrix.t[1] = frame.bonePosY;
    frame.boneMatrix.t[2] = frame.bonePosZ;
    SetRotMatrix(&frame.boneMatrix);
    SetTransMatrix(&frame.boneMatrix);
    RotTransPointToInt16(&frame.offsetX, &frame.bonePosX, &frame.projDepth);
    frame.boneAngleX -= 0x258;
    frame.boneAngleZ -= 0x190;
    RotMatrixXYZ(&frame.boneAngleX, &frame.boneMatrix);
    RotateMatrixY(frame.yaw, &frame.boneMatrix);
    ScaleMatrix(&frame.boneMatrix, &g_BattleLimitDynamiteModelReq1.handScale);
    frame.boneMatrix.t[0] = frame.bonePosX;
    frame.boneMatrix.t[1] = frame.bonePosY;
    frame.boneMatrix.t[2] = frame.bonePosZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.boneMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitDynamiteDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitDynamiteModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDynamiteDrawListHead);
    if (frame.actorBone->frame >= 25) {
        BattleFx_LimitDynamiteDrawLivewireLines((Vec3sPacked *)&frame.bonePosX, &frame.boneAngleX, (short)frame.yaw);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.actorBone->frame)++;
    if (frame.actorBone->frame == 53) {
        frame.actorBone->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a bouncing piece (the B_PIECE sprite, the task variant as its frame) at half scale, turning
 * about Z. Its grey level is 0x80 until frame 22, then fades, semi-transparent. Unpaused, it moves,
 * bounces off the ground (Y = 0) with its Y velocity reversed, falls, and retires at frame 30.
 */
// FUNCTION: FF7 0x004b5ee0
void __cdecl BattleFx_LimitDynamiteBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int frameValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short frameIndex; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix spriteMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int projDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxDynamiteBounceSprite *bounceSprite; };
    } frame;
#pragma pack(pop)

    frame.bounceSprite = (BattleFxDynamiteBounceSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitDynamiteSpriteReq.sprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitDynamiteLoadedResTable);
    g_BattleLimitDynamiteSpriteReq.sprite.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitDynamiteLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.spriteMatrix.m[2][frame.axisIndex] = 0;
        frame.spriteMatrix.m[1][frame.axisIndex] = 0;
        frame.spriteMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.bounceSprite->frameCount < 22) {
        frame.frameValue = 8;
    } else {
        frame.frameValue = 30 - frame.bounceSprite->frameCount;
    }
    frame.frameIndex = *(short *)&frame.frameValue;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.bounceSprite->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.bounceSprite->posX, frame.spriteMatrix.t, &frame.projDepth);
    frame.spriteMatrix.m[2][2] = 0x800;
    frame.spriteMatrix.m[1][1] = frame.spriteMatrix.m[2][2];
    frame.spriteMatrix.m[0][0] = frame.spriteMatrix.m[1][1];
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.spriteMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleLimitDynamiteSpriteReq.sprite.frame = frame.bounceSprite->variant;
    g_BattleLimitDynamiteSpriteReq.sprite.code = (frame.frameIndex - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleLimitDynamiteSpriteReq.sprite.colorB = frame.frameIndex << 4;
    g_BattleLimitDynamiteSpriteReq.sprite.colorG = g_BattleLimitDynamiteSpriteReq.sprite.colorB;
    g_BattleLimitDynamiteSpriteReq.sprite.colorR = g_BattleLimitDynamiteSpriteReq.sprite.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitDynamiteSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceSprite->posX += frame.bounceSprite->velocityX;
    frame.bounceSprite->posY += frame.bounceSprite->velocityY;
    frame.bounceSprite->posZ += frame.bounceSprite->velocityZ;
    if (frame.bounceSprite->posY > 0) {
        frame.bounceSprite->posY = 0;
        frame.bounceSprite->velocityY *= -1;
    }
    frame.bounceSprite->velocityY += 20;
    frame.bounceSprite->angle += 0x200 /* 45 deg */;
    (frame.bounceSprite->frameCount)++;
    if (frame.bounceSprite->frameCount == 30) {
        frame.bounceSprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the explosion (DYNAMITE.RSD, table entry 1) at the task position, turning about Y and growing
 * each frame, with its fade level rising by 0x78 a frame. Retires at frame 30.
 */
// FUNCTION: FF7 0x004b615a
void __cdecl BattleFx_LimitDynamiteGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[144];
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_48[0x48]; BattleFxCosmoMemoryRingSlot *growSlot; };
        struct { unsigned char pad_to_50[0x50]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_80[0x80]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.growSlot = (BattleFxCosmoMemoryRingSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitDynamiteParamTable.explosionDraw.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitDynamiteLoadedResTable);
    frame.angleX.vy = frame.growSlot->timer << 5;
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.scaleX.vz = frame.growSlot->timer * 0xC8 + 0x400;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.growSlot->posX;
    frame.rotMatrix.t[1] = frame.growSlot->posY;
    frame.rotMatrix.t[2] = frame.growSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleLimitDynamiteParamTable.explosionDraw.fadeLevel = frame.growSlot->timer * 120;
    g_BattleLimitDynamiteDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitDynamiteParamTable.explosionDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDynamiteDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growSlot->timer)++;
    if (frame.growSlot->timer == 30) {
        frame.growSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */

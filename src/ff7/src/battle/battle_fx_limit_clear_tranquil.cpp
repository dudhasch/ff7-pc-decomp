// src/ff7/src/battle/battle_fx_limit_clear_tranquil.cpp
// Limit Break [50] (Yuffie Lv 1-2): Clear Tranquil (Japanese: 明鏡止水, dev tag: "LIMIT2/MEIKYO" / Meikyo)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Clear Tranquil (Meikyo) limit break effect: a master timeline updates sprite and
//   gradient line records and runs growing model pairs. A helper claims a free particle slot from
//   the pool.
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

/* The draw cursor and the 0x12C effect records: the pool starts at g_BattleLimitClearTranquilParticlePool
 * and runs on over the two .bss globals after it, so it is reached through this view. */
#define CLEAR_TRANQUIL_POOL ((BattleLimitClearTranquilParticlePool *)&g_BattleLimitClearTranquilParticlePool)
/* The fading-sprite request: its sprite group and draw object are the last two words of
 * g_BattleLimitClearTranquilModelReq, its colour bytes begin g_BattleLimitClearTranquilSpriteReq. */
#define CLEAR_TRANQUIL_SPRITE_REQ ((BattleFxSpriteRequest *)&g_BattleLimitClearTranquilModelReq[14])

/* The fading-sprite request's colour, primitive code and frame, then the first twelve bytes of the
 * BattleFxModelDraw request the growing-pair task submits (g_rawData_00809b9c holds its state flags). */
typedef struct {
    char colorR;
    char colorG;
    char colorB;
    unsigned char code;
    unsigned short frame;
    unsigned short uvPacked;
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
} BattleLimitMeikyoSpriteReq;

// clang-format off
// GLOBAL: FF7 0x00809b40
int g_BattleLimitClearTranquilResourceDef = 0x001900B1;
int g_BattleLimitClearTranquilResourceDefHeadPtr[1] = {
    (int)g_BattleFxLimitClearTranquilResourceNames
};
// GLOBAL: FF7 0x00809b48
int g_BattleLimitClearTranquilModelReq[16] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)(g_BattleFxLimitClearTranquilResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x192a39, (int)(g_BattleFxLimitClearTranquilResourceNames + 0x18), (int)(g_BattleFxLimitClearTranquilResourceNames + 0x24), 0x0,
    0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x00809b88
BattleLimitMeikyoSpriteReq g_BattleLimitClearTranquilSpriteReq = {(char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0, 0, 0xa8, 0, 0};
unsigned char g_rawData_00809b9c[4] = {0x20, 0x00, 0x00, 0x00};
unsigned char g_BattleFxLimitClearTranquilResourceNames[48] =
    "SUIKYU.RSD\0\0"
    "JIMEN.RSD\0\0\0"
    "SHISUI.TIM\0\0"
    "SHISUI.SPT";
// GLOBAL: FF7 0x00af0490
BattleLimitClearTranquilParticlePool g_BattleLimitClearTranquilParticlePool = {0};
// GLOBAL: FF7 0x00af1c08
short g_BattleLimitClearTranquilTargetMask = 0;
// GLOBAL: FF7 0x00af1c0c
struct GfxTableCursor *g_BattleLimitClearTranquilLoadedResTable = 0;
// clang-format on
/* The rotation angles of the growing model pair, never written: RotMatrixXYZ reads the x and y
 * angles here and the z angle from the pad after it, all zero. */
// clang-format off
// GLOBAL: FF7 0x00af1c10
int g_BattleLimitClearTranquilPairAngles = 0;
unsigned char g_bssPad_00af1c14[4] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Loads Clear Tranquil / Meikyo limit break effect resources (LIMIT2/MEIKYO).
 */
// FUNCTION: FF7 0x004b46b0
void __cdecl BattleFx_LimitLoadResClearTranquil(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int tempResDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitClearTranquilLoadedResTable == 0) {
        kind = 3;
        tempResDef = (int)&g_BattleLimitClearTranquilResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x00809bd0
        pRequest->resourcePath = BattleFx_BuildResourcePath("LIMIT2/MEIKYO", pathText);
        Gfx_LoadModelTableIfAbsent(kind, tempResDef, pRequest, &g_BattleLimitClearTranquilLoadedResTable, drawDev);
    }
}

/*
 * Entry point of the Meikyo limit: loads its resources, points the pool's draw cursor at the
 * packet scratch and starts the effect on the targets.
 */
// FUNCTION: FF7 0x004b4740
int __cdecl BattleFx_LimitEnterClearTranquil(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResClearTranquil();
    CLEAR_TRANQUIL_POOL->drawPtr = (int)&g_GfxPsxPacketScratch;
    BattleFx_LimitBeginClearTranquil(targetMask, actorIdx);
    return 0;
}

/*
 * Records the target mask, starts the timeline task and frees every record of the effect pool.
 */
// FUNCTION: FF7 0x004b4766
void __cdecl BattleFx_LimitBeginClearTranquil(int targetMask, int actorIdx)
{
    short i;

    g_BattleLimitClearTranquilTargetMask = targetMask;
    Battle_AllocateResourceSlot(BattleFx_LimitClearTranquilTimelineTask);
    for (i = 0; i < 0x12C; i++) {
        CLEAR_TRANQUIL_POOL->records[i].tag = BATTLE_MEIKYO_RECORD_FREE;
    }
}

/*
 * Timeline task: draws the effect pool every frame; at frame 60 spawns a growing model pair on each
 * target's bone, at frame 100 calls Battle_PostActorHitOrHurtMessage for each target, and frees its slot at frame 110.
 */
// FUNCTION: FF7 0x004b47b7
void __cdecl BattleFx_LimitClearTranquilTimelineTask(void)
{
    BattleTaskSlot *localTaskSlot;
    short actorIdx;
    BattleFxGrowingPairSlot *newTaskPtr;
    int s;
    int t;
    int u;
    int v;
    int w;
    int x;
    int y;
    int z;
    int a;
    int r;
    int b;
    int d;
    int e;
    int f;
    int g;
    int h;

    localTaskSlot = BATTLE_CURRENT_TASK_SLOT;
    BattleFx_LimitClearTranquilUpdateSpriteRecords();
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (localTaskSlot->frameCounter == 0x3C) {
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if ((g_BattleLimitClearTranquilTargetMask & 1 << actorIdx) != 0) {
                newTaskPtr = (BattleFxGrowingPairSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitClearTranquilGrowingModelPairTask);
                Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, &newTaskPtr->posX);
                newTaskPtr->scaleStep = g_BattleActorRecords[actorIdx].radius / 2;
                if (newTaskPtr->scaleStep > 0x320) {
                    newTaskPtr->scaleStep = 0x320;
                }
            }
        }
        Battle_PlaySfxPannedToActor(0x130, -1, 0x40);
    }
    if (localTaskSlot->frameCounter == 0x64) {
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if ((g_BattleLimitClearTranquilTargetMask & 1 << actorIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIdx);
            }
        }
        Battle_PlaySfxPannedToActor(0x6C, -1, 0x40);
    }
    (localTaskSlot->frameCounter)++;
    if (localTaskSlot->frameCounter == 0x6E) {
        localTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws and advances every claimed record of the effect pool, by its tag.
 */
// FUNCTION: FF7 0x004b4941
void __cdecl BattleFx_LimitClearTranquilUpdateSpriteRecords(void)
{
    short i;
    BattleFxClearTranquilPoolRecord *recordAddr;

    i = 0;
    recordAddr = CLEAR_TRANQUIL_POOL->records;
    for (; i < 0x12C; i++, recordAddr++) {
        switch (recordAddr->tag) {
        case BATTLE_MEIKYO_RECORD_GRADIENT_LINE:
            BattleFx_LimitClearTranquilUpdateGradientLineRecord((BattleFxClearTranquilGradientLine *)recordAddr);
            break;
        case BATTLE_MEIKYO_RECORD_FADE_SPRITE:
            BattleFx_LimitClearTranquilUpdateFadingSpriteRecord((BattleFxClearTranquilFadeSprite *)recordAddr);
            break;
        }
    }
}

/*
 * Draws a fading sprite record as a billboard that darkens over 16 frames while it moves by its
 * velocity, then frees the record.
 */
// FUNCTION: FF7 0x004b49ae
void __cdecl BattleFx_LimitClearTranquilUpdateFadingSpriteRecord(BattleFxClearTranquilFadeSprite *fadeSprite)
{
    CLEAR_TRANQUIL_SPRITE_REQ->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitClearTranquilLoadedResTable);
    CLEAR_TRANQUIL_SPRITE_REQ->drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitClearTranquilLoadedResTable);
    BattleFx_InstallBillboardMatrix(&fadeSprite->posX, 0x800, 0);
    g_BattleLimitClearTranquilSpriteReq.colorB = 0x80 - (fadeSprite->frame << 3);
    g_BattleLimitClearTranquilSpriteReq.colorG = g_BattleLimitClearTranquilSpriteReq.colorB;
    g_BattleLimitClearTranquilSpriteReq.colorR = g_BattleLimitClearTranquilSpriteReq.colorG;
    CLEAR_TRANQUIL_POOL->drawPtr = BattleFx_SubmitSpriteDraw(&CLEAR_TRANQUIL_SPRITE_REQ->spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, CLEAR_TRANQUIL_POOL->drawPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    fadeSprite->posX += fadeSprite->velocityX;
    fadeSprite->posY += fadeSprite->velocityY;
    fadeSprite->posZ += fadeSprite->velocityZ;
    (fadeSprite->frame)++;
    if (fadeSprite->frame == 0x10) {
        fadeSprite->recordTag = BATTLE_MEIKYO_RECORD_FREE;
    }
}

/*
 * Draws a gradient line record: a Gouraud line from the record's origin along its rotation, black at
 * the far end and cyan at the near end, shortening over 10 steps before the record is freed.
 */
// FUNCTION: FF7 0x004b4ab8
void __cdecl BattleFx_LimitClearTranquilUpdateGradientLineRecord(BattleFxClearTranquilGradientLine *gradientLine)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        struct { unsigned char pad_to_3[0x3]; char commandByte; };
        struct { unsigned char pad_to_4[0x4]; int depthOut; };
        struct { unsigned char pad_to_8[0x8]; SVECTOR vertex1X; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR vertex0X; };
        struct { unsigned char pad_to_18[0x18]; int depth; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; int screenPoint; };
        struct { unsigned char pad_to_40[0x40]; BattleGpuGouraudLine *gradientPrim; };
    } frame;
#pragma pack(pop)

    RotMatrix(&gradientLine->rotation, &frame.rotMatrix);
    frame.rotMatrix.t[0] = gradientLine->lineX;
    frame.rotMatrix.t[1] = gradientLine->lineY;
    frame.rotMatrix.t[2] = gradientLine->lineZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RefreshGlobalTransformState();
    frame.vertex0X.vz = (0xA - gradientLine->step) * 0x258;
    frame.vertex0X.vy = 0;
    frame.vertex0X.vx = frame.vertex0X.vy;
    RotTransPointToInt16(&frame.vertex0X, (short *)&frame.vertex0X, &frame.depthOut);
    frame.vertex1X.vz = (0xA - gradientLine->step) * 0xC8;
    frame.vertex1X.vy = 0;
    frame.vertex1X.vx = frame.vertex1X.vy;
    RotTransPointToInt16(&frame.vertex1X, (short *)&frame.vertex1X, &frame.depthOut);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    frame.gradientPrim = (BattleGpuGouraudLine *)CLEAR_TRANQUIL_POOL->drawPtr;
    frame.gradientPrim->tag = frame.gradientPrim->tag & 0xFFFFFF | 0x4000000;
    frame.gradientPrim->command = 0x50;
    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        frame.gradientPrim->command = *(unsigned char *)&frame.gradientPrim->command | 2;
        frame.commandByte = frame.gradientPrim->command;
    } else {
        frame.gradientPrim->command = *(unsigned char *)&frame.gradientPrim->command & ~2;
        frame.commandByte = frame.gradientPrim->command;
    }
    frame.depth = Gfx_TransformVertexSlot0ReturnDepth(&frame.vertex0X, &frame.screenPoint, &frame.depthOut);
    if (frame.depth > 0) {
        frame.gradientPrim->colorB0 = 0;
        frame.gradientPrim->colorG0 = 0;
        frame.gradientPrim->colorR0 = 0;
        frame.gradientPrim->colorR1 = 0;
        frame.gradientPrim->colorB1 = 0x80;
        frame.gradientPrim->colorG1 = 0x80;
        Gfx_TransformVertexSlot1ReturnDepth((GfxPsxShortVertexSlot *)&frame.vertex1X, &frame.screenPoint, &frame.depthOut);
        Gfx_DrawPsxPrimitive(frame.gradientPrim);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (gradientLine->step)++;
    if (gradientLine->step == 0xA) {
        gradientLine->recordTag = BATTLE_MEIKYO_RECORD_FREE;
    }
}

/*
 * Draws the two models at a target, scaled up with the task's timer; spawns gradient lines while
 * the timer is below 20, two rings of 16 fading sprites at 35, and frees its slot at 40.
 */
// FUNCTION: FF7 0x004b4ca1
void __cdecl BattleFx_LimitClearTranquilGrowingModelPairTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        int depthOut;
        struct { unsigned char pad_to_4[0x4]; SVECTOR orbitX; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; short angleIndex; };
        struct { unsigned char pad_to_34[0x34]; short pairIndex; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_40[0x40]; BattleFxGrowingPairSlot *pairRecord; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; BattleFxClearTranquilPoolRecord *modelPair; };
        struct { unsigned char pad_to_70[0x70]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.pairRecord = (BattleFxGrowingPairSlot *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrixXYZ(&g_BattleLimitClearTranquilPairAngles, &frame.rotMatrix);
    frame.scaleX.vz = frame.pairRecord->scaleStep * frame.pairRecord->timer;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.pairRecord->posX;
    frame.rotMatrix.t[1] = 0xFFFFFFFD;
    frame.rotMatrix.t[2] = frame.pairRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitClearTranquilSpriteReq.fadeLevel = frame.pairRecord->timer * 0x64;
    g_BattleLimitClearTranquilSpriteReq.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitClearTranquilLoadedResTable);
    CLEAR_TRANQUIL_POOL->drawPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitClearTranquilSpriteReq.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, CLEAR_TRANQUIL_POOL->drawPtr);
    RotMatrixXYZ(&g_BattleLimitClearTranquilPairAngles, &frame.rotMatrix);
    frame.scaleX.vz = frame.pairRecord->scaleStep * frame.pairRecord->timer;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.pairRecord->posX;
    frame.rotMatrix.t[1] = 0xFFFFFFFD;
    frame.rotMatrix.t[2] = frame.pairRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleLimitClearTranquilSpriteReq.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitClearTranquilLoadedResTable);
    CLEAR_TRANQUIL_POOL->drawPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitClearTranquilSpriteReq.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, CLEAR_TRANQUIL_POOL->drawPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.pairRecord->timer < 0x14) {
        for (frame.pairIndex = 0; frame.pairIndex < ((0x14 - frame.pairRecord->timer) / 4) + 1; frame.pairIndex++) {
            frame.modelPair = BattleFx_LimitClearTranquilClaimEffectRecord(BATTLE_MEIKYO_RECORD_GRADIENT_LINE);
            frame.modelPair->posX = frame.pairRecord->posX;
            frame.modelPair->posY = frame.pairRecord->posY;
            frame.modelPair->posZ = frame.pairRecord->posZ;
            frame.modelPair->rotation.vx = (rand() % 4096);
            frame.modelPair->rotation.vy = (rand() % 4096);
            frame.modelPair->rotation.vz = 0;
        }
    }
    if (frame.pairRecord->timer == 0x23) {
        for (frame.pairIndex = 0; frame.pairIndex < 2; frame.pairIndex++) {
            for (frame.angleIndex = 0; frame.angleIndex < 0x10; frame.angleIndex++) {
                frame.modelPair = BattleFx_LimitClearTranquilClaimEffectRecord(BATTLE_MEIKYO_RECORD_FADE_SPRITE);
                if (frame.modelPair != 0) {
                    frame.modelPair->posX = frame.pairRecord->posX;
                    frame.modelPair->posY = frame.pairRecord->posY;
                    frame.modelPair->posZ = frame.pairRecord->posZ;
                    frame.orbitX.vx = SinFixed(frame.angleIndex << 8) * 0xC8 >> 0xC;
                    frame.orbitX.vy = 0;
                    frame.orbitX.vz = CosFixed(frame.angleIndex << 8) * 0xC8 >> 0xC;
                    frame.rotAngleX.vy = 0;
                    frame.rotAngleX.vx = frame.rotAngleX.vy;
                    frame.rotAngleX.vz = (frame.pairIndex << 0xA) - 0x200;
                    RotMatrixZYX(&frame.rotAngleX, &frame.rotMatrix);
                    frame.rotMatrix.t[2] = 0;
                    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                    SetRotMatrix(&frame.rotMatrix);
                    SetTransMatrix(&frame.rotMatrix);
                    RotTransPointToInt16(&frame.orbitX, &frame.modelPair->velocityX, &frame.depthOut);
                }
            }
        }
    }
    (frame.pairRecord->timer)++;
    if (frame.pairRecord->timer == 0x28) {
        frame.pairRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Claims the first free record of the effect pool for `tag`; returns it, or 0 when none is free.
 */
// FUNCTION: FF7 0x004b50ab
BattleFxClearTranquilPoolRecord *__cdecl BattleFx_LimitClearTranquilClaimEffectRecord(short tag)
{
    short i;

    for (i = 0; i < 0x12C; i++) {
        if (CLEAR_TRANQUIL_POOL->records[i].tag == BATTLE_MEIKYO_RECORD_FREE) {
            CLEAR_TRANQUIL_POOL->records[i].tag = tag;
            CLEAR_TRANQUIL_POOL->records[i].age = 0;
            return &CLEAR_TRANQUIL_POOL->records[i];
        }
    }
    return 0;
}
} /* extern "C" */

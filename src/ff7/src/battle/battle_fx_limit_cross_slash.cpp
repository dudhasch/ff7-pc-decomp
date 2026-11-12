// src/ff7/src/battle/battle_fx_limit_cross_slash.cpp
// Limit Break [1] (Cloud Lv 1-2): Cross-slash (Japanese: 凶斬り, dev tag: "LIMIT/KYOGIRI" / Kyogiri / Kyou)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Kyogiri limit-break effect: loads its resources, spawns a timeline per target, and
//   draws the slash sequence with contracting kanji sprites and trailing sparks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/codegen.h"
extern "C" {


#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008f3428
int g_BattleLimitCrossSlash_ResourceDefs = 0x00192A39;
char *g_BattleLimitCrossSlash_ResourceDefsTim[1] = {
    // STRING: FF7 0x008f3464
    "KYOGIRI.TIM"
};
unsigned int g_BattleLimitCrossSlash_ResourceDefsSpt[13] = {
    // STRING: FF7 0x008f3470
    (unsigned int)"ONE.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f3478
    (unsigned int)"KYOGIRI.TIM",
    // STRING: FF7 0x008f3484
    (unsigned int)"TWO.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f348c
    (unsigned int)"KYOGIRI.TIM",
    // STRING: FF7 0x008f3498
    (unsigned int)"THREE.SPT",
    0x00000000,
    0x00000000
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bda460
int g_BattleLimitCrossSlash_DrawListHandle = 0;
// GLOBAL: FF7 0x00bda464
struct GfxTableCursor *g_BattleLimitCrossSlash_LoadedRes = 0;
// clang-format on

/*
 * Loads resources for the Kyogiri limit effect when not already loaded.
 */
// FUNCTION: FF7 0x005ae4c0
void __cdecl BattleFx_LimitLoadResCrossSlash(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int resultType;
    char pathState[200];
    GfxResourceRequest *request;
    int descriptor;

    if (g_pBattle3dScene != 0 && g_BattleLimitCrossSlash_LoadedRes == 0) {
        resultType = 3;
        descriptor = (int)&g_BattleLimitCrossSlash_ResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008f34a4
        request->resourcePath = (char *)BattleFx_BuildResourcePath("LIMIT/KYOGIRI", pathState);
        Gfx_LoadModelTableIfAbsent(resultType, descriptor, request, &g_BattleLimitCrossSlash_LoadedRes, context);
    }
}

/*
 * Entry point registering per-target callback for Kyogiri.
 */
// FUNCTION: FF7 0x005ae550
int __cdecl Battle_LimitFx01_CrossSlash(int targetMask, int actorIndex)
{
    BattleFx_LimitLoadResCrossSlash();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIndex, 0, BattleFx_LimitSpawnCrossSlashPerTarget);
    return 0;
}

/*
 * Spawns timeline task for a single Kyogiri target.
 */
// FUNCTION: FF7 0x005ae573
void __cdecl BattleFx_LimitSpawnCrossSlashPerTarget(int actor)
{
    BattleFxCrossSlashTask *task;

    task = (BattleFxCrossSlashTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCrossSlashTimelineTask);
    task->targetIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &task->pos.xyz.x);
    task->negRadius = -g_BattleActorRecords[actor].radius;
}

/*
 * Drives the main timeline and slash sequence for Kyogiri.
 */
// FUNCTION: FF7 0x005ae5de
void __cdecl BattleFx_LimitCrossSlashTimelineTask(void)
{
    BattleFxCrossSlashSlot *task;
    BattleFxCrossSlashTimeline *child;

    task = (BattleFxCrossSlashSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        SfxLoadIntoBuffer(0x12, 0);
        SfxLoadIntoBuffer(0x12, 0);
        SfxLoadIntoBuffer(0x1B, 0);
        SfxLoadIntoBuffer(0x21, 0);
    }
    if (task->step == 5) {
        child = (BattleFxCrossSlashTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCrossSlashShrinkingSpriteTask);
        child->tableValue8 = Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitCrossSlash_LoadedRes);
        child->tableValue4 = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitCrossSlash_LoadedRes);
        child->step = task->step;
        child->pos.pair = task->pos.pair;
        child->negRadius = task->negRadius;
    }
    if (task->step == 9) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->pos), 0x12);
    }
    if (task->step == 0xF) {
        child = (BattleFxCrossSlashTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCrossSlashShrinkingSpriteTask);
        child->tableValue8 = Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitCrossSlash_LoadedRes);
        child->tableValue4 = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitCrossSlash_LoadedRes);
        child->step = task->step;
        child->pos.pair = task->pos.pair;
        child->negRadius = task->negRadius;
    }
    if (task->step == 0x13) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->pos), 0x12);
    }
    if (task->step == 0x1A) {
        child = (BattleFxCrossSlashTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCrossSlashShrinkingSpriteTask);
        child->tableValue8 = Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitCrossSlash_LoadedRes);
        child->tableValue4 = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitCrossSlash_LoadedRes);
        child->step = task->step;
        child->pos.pair = task->pos.pair;
        child->negRadius = task->negRadius;
    }
    if (task->step == 0x1E) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->pos), 0x1B);
    }
    if (task->step == 0x25) {
        Battle_PostActorHitOrHurtMessage(task->recordType);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->pos), 0x21);
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x12, 0xF);
        Sfx_ReleaseCachedBuffer(0x12, 0xF);
        Sfx_ReleaseCachedBuffer(0x1B, 0xF);
        Sfx_ReleaseCachedBuffer(0x21, 0xF);
    }
    (task->step)++;
}

/*
 * Updates and renders contracting slash kanji sprites for Kyogiri.
 */
// FUNCTION: FF7 0x005ae8c5
void __cdecl BattleFx_LimitCrossSlashShrinkingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[32];
        int drawWord0;
        struct { unsigned char pad_to_4[0x4]; int drawWord1; };
        struct { unsigned char pad_to_8[0x8]; char colorB; };
        struct { unsigned char pad_to_9[0x9]; char colorG; };
        struct { unsigned char pad_to_A[0xA]; char colorR; };
        struct { unsigned char pad_to_B[0xB]; char colorType; };
        struct { unsigned char pad_to_C[0xC]; short drawFlags; };
        struct { unsigned char pad_to_10[0x10]; BattleCrossSlashShrinkingSpriteRecord *trailRecord; };
        struct { unsigned char pad_to_14[0x14]; BattleFxCrossSlashShrinkSprite *shrinkSprite; };
        struct { unsigned char pad_to_18[0x18]; int phaseStep; };
        struct { unsigned char pad_to_1C[0x1C]; int billboardScale; };
    } frame;
#pragma pack(pop)

    frame.shrinkSprite = (BattleFxCrossSlashShrinkSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.drawWord0 = frame.shrinkSprite->spriteResource;
    frame.drawWord1 = frame.shrinkSprite->spriteResourceAlt;
    Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitCrossSlash_LoadedRes);
    frame.drawFlags = 0;
    if (frame.shrinkSprite->step < 4) {
        frame.colorR = (frame.shrinkSprite->step << 7) / 4;
        frame.colorG = frame.colorR;
        frame.colorB = frame.colorG;
        frame.colorType = 0x2E;
        frame.billboardScale = ((frame.shrinkSprite->step * -0x3000) / 4) + 0x4000;
        BattleFx_InstallBillboardMatrix(&frame.shrinkSprite->pos.xyz.x, *(short *)&frame.billboardScale, frame.shrinkSprite->negRadius);
        g_BattleLimitCrossSlash_DrawListHandle = BattleFx_SubmitSpriteDraw(&frame.drawWord0, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCrossSlash_DrawListHandle);
    } else {
        frame.phaseStep = frame.shrinkSprite->step + frame.shrinkSprite->ownerSlot;
        if (frame.phaseStep < 0x25) {
            frame.colorR = 0x80;
            frame.colorG = frame.colorR;
            frame.colorB = frame.colorG;
            frame.colorType = 0x2E;
            frame.billboardScale = 0x1000;
            BattleFx_InstallBillboardMatrix(&frame.shrinkSprite->pos.xyz.x, *(short *)&frame.billboardScale, frame.shrinkSprite->negRadius);
            g_BattleLimitCrossSlash_DrawListHandle = BattleFx_SubmitSpriteDraw(&frame.drawWord0, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCrossSlash_DrawListHandle);
        } else {
            frame.phaseStep -= 0x25;
            if (frame.phaseStep < 8) {
                if (g_BattleIsPaused == 0) {
                    frame.trailRecord = (BattleCrossSlashShrinkingSpriteRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitCrossSlashTrailSpriteTask);
                    frame.trailRecord->size = (frame.phaseStep * 3) / 4;
                    frame.trailRecord->pos.pair = frame.shrinkSprite->pos.pair;
                    frame.trailRecord->spriteGroup = (BattleFxSpriteGroup *)frame.shrinkSprite->spriteResource;
                    frame.trailRecord->drawObj = frame.shrinkSprite->spriteResourceAlt;
                    frame.trailRecord->ownerSlot = *(short *)&frame.phaseStep;
                    frame.trailRecord->negRadius = frame.shrinkSprite->negRadius;
                }
            } else {
                frame.shrinkSprite->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    if (g_BattleIsPaused == 0) {
        (frame.shrinkSprite->step)++;
    }
}

/*
 * Updates and renders trailing spark sprites along slash paths for Kyogiri.
 */
// FUNCTION: FF7 0x005aead6
void __cdecl BattleFx_LimitCrossSlashTrailSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; int drawWord0; };
        struct { unsigned char pad_to_C[0xC]; int drawWord1; };
        struct { unsigned char pad_to_10[0x10]; char colorB; };
        struct { unsigned char pad_to_11[0x11]; char colorG; };
        struct { unsigned char pad_to_12[0x12]; char colorR; };
        struct { unsigned char pad_to_13[0x13]; char colorType; };
        struct { unsigned char pad_to_14[0x14]; short drawFlags; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix trailMatrix; };
        struct { unsigned char pad_to_38[0x38]; BattleFxCrossSlashTrailSprite *trailSprite; };
        struct { unsigned char pad_to_3C[0x3C]; VECTOR scaleVector; };
    } frame;
#pragma pack(pop)

    frame.trailSprite = (BattleFxCrossSlashTrailSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.drawWord0 = frame.trailSprite->spriteResource;
    frame.drawWord1 = frame.trailSprite->spriteResourceAlt;
    Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitCrossSlash_LoadedRes);
    frame.drawFlags = 0;
    frame.colorR = ((frame.trailSprite->frameCount * -0x80) / 8) + 0x80;
    frame.colorG = frame.colorR;
    frame.colorB = frame.colorG;
    frame.colorType = 0x2E;
    frame.scaleVector.vz = ((frame.trailSprite->progress * 0x6FFF) / 8) + 0x1000;
    frame.scaleVector.vy = frame.scaleVector.vz;
    frame.scaleVector.vx = frame.scaleVector.vy;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.trailSprite->progress << 8;
    RotMatrix(&frame.angleX, &frame.trailMatrix);
    ScaleMatrix(&frame.trailMatrix, &frame.scaleVector);
    BattleFx_MakeViewSpacePointPushed(&frame.trailSprite->pos, frame.trailSprite->negRadius, (Vec3sPacked *)&frame.trailMatrix);
    SetRotMatrix(&frame.trailMatrix);
    SetTransMatrix(&frame.trailMatrix);
    g_BattleLimitCrossSlash_DrawListHandle = BattleFx_SubmitSpriteDraw(&frame.drawWord0, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitCrossSlash_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        (frame.trailSprite->frameCount)++;
        if (frame.trailSprite->frameCount >= 8) {
            frame.trailSprite->progress = 0xFFFF;
        }
    }
}
} /* extern "C" */

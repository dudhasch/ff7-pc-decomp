// src/ff7/src/battle/battle_fx_blue_goblin_punch_effect.cpp
// Enemy Skill [17]: Goblin Punch (Japanese: ゴブリンパンチ, dev tag: "blue/goblinp" / Goblinp)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Goblin Punch blue magic effect: a punching glove sprite, an impact flash and speed
//   streak lines, with punch sounds on the timeline.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x008abd48
int g_BattleBlueGoblinPunchResourceDefs = 0x00192A39;
char *g_BattleFxBlueGoblinPunchEffectResourceNames[1] = {
    // STRING: FF7 0x008abd90
    "GOBLIN_1.TIM"
};
unsigned int g_BattleFxBlueGoblinPunchEffectResourceNames2[8] = {
    // STRING: FF7 0x008abda0
    (unsigned int)"goblinp1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008abdb0
    (unsigned int)"GOBLIN_2.TIM",
    // STRING: FF7 0x008abdc0
    (unsigned int)"goblinp2.spt",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008abd70
struct BattleFxSpriteRequest g_BattleBlueGoblinPunchSpriteReqs[2] = {{0, 0, -128, -128, -128, 0x2e, 0, 0}, {0, 0, -128, -128, -128, 0x2e, 0, 0}};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00ba5f78
int g_BattleBlueGoblinPunchTargetIndex = 0;
// GLOBAL: FF7 0x00ba5f7c
int g_BattleBlueGoblinPunchStreakCount = 0;
// GLOBAL: FF7 0x00ba5f80
unsigned char g_BattleBlueGoblinPunchStreakVertices[5326] = {0};
// GLOBAL: FF7 0x00ba7450
struct GfxTableCursor *g_BattleBlueGoblinPunchLoadedResTable = 0;
unsigned char g_bssPad_00ba7454[4] = {0};
// clang-format on

/* Loads Goblin Punch blue magic resources. */
// FUNCTION: FF7 0x00572b90
void __cdecl BattleFx_BlueLoadResGoblinPunch(void)
{
    GfxModelTableBank *sceneAddr;
    GfxResourceRequest srcRequestBuf;
    int kind;
    char resPathBuf[200];
    GfxResourceRequest *pRequest;
    int defsDeck;

    if (g_pBattle3dScene != 0 && g_BattleBlueGoblinPunchLoadedResTable == 0) {
        kind = 2;
        defsDeck = (int)&g_BattleBlueGoblinPunchResourceDefs;
        sceneAddr = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &srcRequestBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008abdd0
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/goblinp", resPathBuf);
        Gfx_LoadModelTableIfAbsent(kind, defsDeck, pRequest, &g_BattleBlueGoblinPunchLoadedResTable, sceneAddr);
    }
}

/* Entry point for Goblin Punch blue magic effect. */
// FUNCTION: FF7 0x00572c20
void __cdecl BattleFx_BlueEnterGoblinPunch(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResGoblinPunch();
    BattleFx_BlueBeginGoblinPunch(targetMask, actorIdx);
}

/* Initializes setup task for Goblin Punch. */
// FUNCTION: FF7 0x00572c3a
void __cdecl BattleFx_BlueBeginGoblinPunch(int targetMask, int actorIdx)
{
    struct {
        int count;
        int targetSlot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr =
        (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    for (frame.targetSlot = 0, frame.count = 0; frame.targetSlot < 10; frame.targetSlot++) {
        if ((targetMask & 1 << frame.targetSlot) != 0) {
            frame.count++;
            if (frame.count == 1) {
                g_BattleBlueGoblinPunchTargetIndex = frame.targetSlot;
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_BlueGoblinPunchSetupTask);
    g_BattleBlueGoblinPunchStreakCount = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x9e);
}

/* Sets up target position and spawns timeline task for Goblin Punch. */
// FUNCTION: FF7 0x00572ce0
void __cdecl BattleFx_BlueGoblinPunchSetupTask(void)
{
    int rad;
    BattleFxGoblinPunchTaskSlot *task;

    task = (BattleFxGoblinPunchTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxGoblinPunchTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueGoblinPunchTimelineTask);
    Battle_GetBoneWorldPos(g_BattleBlueGoblinPunchTargetIndex, g_BattleActorRecords[g_BattleBlueGoblinPunchTargetIndex].targetBone,
                           &task->pos.vx);
    rad = g_BattleActorRecords[g_BattleBlueGoblinPunchTargetIndex].radius + 0xE0;
    if (rad > 1266) {
        rad = 0x4F2;
    }
    task->baseValue = rad;
    task->jitterRange = task->baseValue << 1;
    task->unitStep = -(task->pos.vy >> 1);
    task->rangeY = task->unitStep * 2 + task->unitStep;
    task->streakLength = rad / 10 + 140;
    task->seed = rad / 8 + 0x37;
}

/* Drives Goblin Punch timeline, punch sounds, and streak line generation. */
// FUNCTION: FF7 0x00572de7
void __cdecl BattleFx_BlueGoblinPunchTimelineTask(void)
{
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    VECTOR nx;
    int vertexIdx;
    int baseJitX;
    int jitterY;
    int currBaseJitZ;
    BattleFxBlueGoblinTimelineSlot *task;
    BattleFxGoblinPunchParticleSlot *pNewTask;
    int bitMix;
    int nextSpeed;
    int padEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int lineIdx;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxBlueGoblinTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step >= 1 && task->step <= 7) {
        pNewTask = (BattleFxGoblinPunchParticleSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueGoblinPunchFlashSpriteTask);
        pNewTask->pos.vx = task->pos.vx + rand() % task->jitterRange - task->jitterOffset;
        pNewTask->pos.vy = task->pos.vy - rand() % task->rangeY + task->offsetY;
        pNewTask->pos.vz = task->pos.vz + rand() % task->jitterRange - task->jitterOffset;
        lineIdx = (task->jitterOffset << 12) / 400;
        vertexIdx = rand() % 4096 + 2048;
        pNewTask->scaledOffset = lineIdx * vertexIdx >> 12;
        pNewTask->step = task->step;
    }
    if (task->step >= 8 && task->step <= 10) {
        if (task->step == 9) {
            BattleFx_StartScreenFlashRamp(1);
        }
        if (task->step == 10) {
            BattleFx_FadeOutScreenFlashRamp(4);
        }
        vertexIdx = g_BattleBlueGoblinPunchStreakCount;
        pNewTask = (BattleFxGoblinPunchParticleSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueGoblinPunchStreakLinesTask);
        pNewTask->step = vertexIdx;
        baseJitX = rand() % task->jitterRange - task->jitterOffset;
        jitterY = rand() % task->jitterRange - task->jitterOffset;
        currBaseJitZ = rand() % task->jitterRange - task->jitterOffset;
        for (lineIdx = 0; lineIdx < 18; lineIdx++) {
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos = task->pos;
            nx.vx = rand() % task->jitterRange - task->jitterOffset + baseJitX;
            nx.vy = rand() % task->jitterRange - task->jitterOffset + jitterY;
            nx.vz = rand() % task->jitterRange - task->jitterOffset + currBaseJitZ;
            NormalizeVectorFixed(&nx, &nx);
            bitMix = nx.vx + nx.vy + nx.vz;
            nextSpeed = task->streakOffsetBase + (bitMix & 0x1F);
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].offsetX = nx.vx * nextSpeed >> 12;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].offsetY = nx.vy * nextSpeed >> 12;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].offsetZ = nx.vz * nextSpeed >> 12;
            if (bitMix < 0) {
                nextSpeed = task->seed >> 1;
            } else {
                nextSpeed = 0;
            }
            nextSpeed = (nextSpeed + ((task->seed + (bitMix & 0xF)) + (nx.vx & 0xF)));
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].velocityX = nx.vx * nextSpeed >> 12;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].velocityY = nx.vy * nextSpeed >> 12;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].velocityZ = nx.vz * nextSpeed >> 12;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vx += ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].offsetY;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vy += ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].velocityZ;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vz += ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].offsetX;
            vertexIdx++;
            if (vertexIdx >= 222) {
                vertexIdx = 0;
            }
        }
        g_BattleBlueGoblinPunchStreakCount = vertexIdx;
    }
    if (task->step == 9) {
        pNewTask = (BattleFxGoblinPunchParticleSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueGoblinPunchAnimatedSpriteTask);
        pNewTask->pos = task->pos;
        pNewTask->scaledOffset = (task->jitterOffset << 12) / 350;
    }
    if (task->step == 9) {
        Battle_PostActorHitOrHurtMessage(g_BattleBlueGoblinPunchTargetIndex);
    }
    if (task->step >= 24) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

/* Animates and submits impact flash sprite for Goblin Punch. */
// FUNCTION: FF7 0x00573291
void __cdecl BattleFx_BlueGoblinPunchFlashSpriteTask(void)
{
    unsigned short flashSize;
    BattleFxGoblinPunchFlashSlot *task;

    task = (BattleFxGoblinPunchFlashSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (task->timer != 0) {
        flashSize = task->tableOffset >> 1;
    } else {
        flashSize = task->tableOffset;
    }
    BattleFx_InstallBillboardMatrix(&task->pos.vx, (short)flashSize, 0);
    g_BattleBlueGoblinPunchSpriteReqs[0].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleBlueGoblinPunchLoadedResTable);
    g_BattleBlueGoblinPunchSpriteReqs[0].drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleBlueGoblinPunchLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueGoblinPunchSpriteReqs[0], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->timer >= 1) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->state == 2 || task->state == 5) {
        if (task->timer != 0) {
            BattleFx_FadeOutScreenFlashRamp(1);
        } else {
            BattleFx_StartScreenFlashRamp(1);
        }
    }
    (task->timer)++;
}

/* Transforms and renders speed streak line geometry. */
// FUNCTION: FF7 0x005733a4
void __cdecl BattleFx_BlueGoblinPunchStreakLinesTask(void)
{
    int newBlend;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int streakIdx8;
    int bufWords[5];
    int vertexIdx;
    int y;
    BattleFxBerserkFadeSprite *task;
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxStreakPacket *pPacket;
    BattleFxBlueGoblinStreakState *drawState8;

    task = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    pPacket = (BattleFxStreakPacket *)bufWords;
    drawState8 = (BattleFxBlueGoblinStreakState *)Gfx_GetPsxScratchpadPtr(0);
    newBlend = task->frame << 9;
    SetDrawColorRGB(0, 0, 0);
    drawState8->baseColor0.word = 0x1C1450;
    drawState8->baseColor1.word = 0x6050F0;
    Gfx_ShadePsxVertexColor(&drawState8->baseColor0.rgb, newBlend, &drawState8->shadedColor0.rgb);
    Gfx_ShadePsxVertexColor(&drawState8->baseColor1.rgb, newBlend, &drawState8->shadedColor1.rgb);
    drawState8->shadedColor0.rgb.cd = 0x52;
    vertexIdx = task->ringIndex;
    for (streakIdx8 = 0; streakIdx8 < 18; streakIdx8++) {
        drawState8->pos.x = ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vx + ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].offsetX;
        drawState8->pos.y = ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vy + ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].offsetY;
        drawState8->pos.z = ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vz + ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].offsetZ;
        drawState8->depth = Gfx_TransformVertexSlot0ReturnDepth(&((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx], &drawState8->scratch, &drawState8->flags);
        Gfx_TransformVertexSlot1ReturnDepth(&drawState8->pos, &drawState8->scratch, &drawState8->flags);
        pPacket->tag = pPacket->tag & 0xFFFFFF | 0x4000000;
        pPacket->colorWord0 = drawState8->shadedColor0.word;
        pPacket->colorWord1 = drawState8->shadedColor1.word;
        Gfx_DrawPsxPrimitive(pPacket);
        if (g_BattleIsPaused == 0) {
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vx += ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].velocityX;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vy += ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].velocityY;
            ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].pos.vz += ((BattleFxBlueGoblinStreakVertex *)&g_BattleBlueGoblinPunchStreakVertices)[vertexIdx].velocityZ;
        }
        vertexIdx++;
        if (vertexIdx >= 222) {
            vertexIdx = 0;
        }
    }
    g_BattleFxActiveDrawListPtr = (int)pPacket;
    if (task->frame >= 8) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}

/* Animates and submits punching glove sprite. */
// FUNCTION: FF7 0x00573642
void __cdecl BattleFx_BlueGoblinPunchAnimatedSpriteTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxMatraMagicAnimSprite *q;

    q = (BattleFxMatraMagicAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueGoblinPunchSpriteReqs[1].frame = q->frame;
    BattleFx_InstallBillboardMatrix(&q->posX, (short)q->scale, -(q->scale >> 2));
    g_BattleBlueGoblinPunchSpriteReqs[1].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueGoblinPunchLoadedResTable);
    g_BattleBlueGoblinPunchSpriteReqs[1].drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueGoblinPunchLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueGoblinPunchSpriteReqs[1], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (q->frame >= 11) {
        q->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (q->frame)++;
}
} /* extern "C" */

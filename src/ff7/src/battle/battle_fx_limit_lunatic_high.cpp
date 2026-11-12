// src/ff7/src/battle/battle_fx_limit_lunatic_high.cpp
// Limit Break [39] (Red XIII Lv 1-2): Lunatic High (Japanese: ルナティックハイ, dev tag: "limit2/lunatic" / Lunatic)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Lunatic limit break effect: a timeline schedules ground-position callbacks per selected
//   actor, scaled model callbacks, and a particle pool, with two model draw tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_limit.h"
extern "C" {

#pragma intrinsic(memset)
#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x0082cb68
int g_BattleLimitLunaticHighResourceDef = 0x00192A39;
char *g_BattleFxLimitLunaticHighResourceNames[1] = {
    // STRING: FF7 0x0082cbc8
    "SMOKE_1.TIM"
};
unsigned int g_BattleFxLimitLunaticHighResourceNames2[14] = {
    // STRING: FF7 0x0082cbd4
    (unsigned int)"SMOKE_1.SPT",
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x0082cbe0
    (unsigned int)"POWER_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082cbec
    (unsigned int)"POWER_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x0082cba8
BattleFxSpriteRequest g_BattleLimitLunaticHighSpriteReq = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0};
unsigned char g_rawData_0082cbb8[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
// clang-format on

#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

// clang-format off
// GLOBAL: FF7 0x00b06960
int g_BattleLimitLunaticHighFirstSelected = 0;
unsigned char g_bssPad_00b06964[4] = {0};
// The 10 actor particle pools (100 24-byte BattleLunaticHighParticle records per actor, 0x960 bytes each).
// Actors 1..9 sit in g_bssPad_00b072c8 so the annotated extent stops before 0x00b0b0b0 (the RGB
// literal in BattleFx_RevengeStampGrayGradientStripTask).
// GLOBAL: FF7 0x00b06968
BattleLunaticHighParticle g_BattleLimitLunaticHighParticlePool[1][100] = {0};
unsigned char g_bssPad_00b072c8[21600] = {0};
// GLOBAL: FF7 0x00b0c728
int g_BattleLimitLunaticHighSelectMask = 0;
// GLOBAL: FF7 0x00b0c72c
int g_BattleLimitLunaticHighTargetMask = 0;
// GLOBAL: FF7 0x00b0c730
struct GfxTableCursor *g_BattleLimitLunaticHighLoadedResTable = 0;
unsigned char g_bssPad_00b0c734[4] = {0};
// clang-format on

/* Loads the limit2/lunatic model table once, when the 3D scene exists and the table is not loaded yet. */
// FUNCTION: FF7 0x004c8200
void __cdecl BattleFx_LimitLoadResLunaticHigh(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest requestSlot;
    int kindId;
    char loadPath[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitLunaticHighLoadedResTable == 0) {
        kindId = 3;
        stepResourceDef = (int)&g_BattleLimitLunaticHighResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0082cbf8
        pRequest->resourcePath = BattleFx_BuildResourcePath("limit2/lunatic", loadPath);
        Gfx_LoadModelTableIfAbsent(kindId, stepResourceDef, pRequest, &g_BattleLimitLunaticHighLoadedResTable, drawDev);
    }
}

/* Entry point: loads the Lunatic resources and starts the effect. Returns zero. */
// FUNCTION: FF7 0x004c8290
int __cdecl BattleFx_LimitEnterLunaticHigh(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResLunaticHigh();
    BattleFx_LimitBeginLunaticHigh(targetMask, actorIdx);
    return 0;
}

/*
 * Starts the draw-list buffer task, records the select mask, the first selected actor and the target
 * mask, and starts the timeline task.
 */
// FUNCTION: FF7 0x004c82ac
void __cdecl BattleFx_LimitBeginLunaticHigh(int targetMask, int actorIdx)
{
    int bitIdx;
    int count;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleLimitLunaticHighSelectMask = targetMask;
    bitIdx = 0;
    for (count = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            count++;
            if (count == 1) {
                g_BattleLimitLunaticHighFirstSelected = bitIdx;
            }
        }
    }
    g_BattleLimitLunaticHighTargetMask = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_LimitLunaticHighTimelineTask);
}

/*
 * The Lunatic timeline, unpaused only. Frame 0 starts a spawn task on the ground under each selected
 * actor, with the actor's radius plus 300, and plays sound 0x182 panned to them. The screen tint ramps
 * up over 8 frames, holds, and fades from frame 32. Frame 35 queues a typed record for each selected
 * actor; from frame 40 the tint is cleared, the task retires and sound 0 is played.
 */
// FUNCTION: FF7 0x004c833d
void __cdecl BattleFx_LimitLunaticHighTimelineTask(void)
{
    int i;
    unsigned short intensity;
    BattleTaskSlot *task;
    BattleLimitLunaticHighTimelineSlot *pNewTask;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCounter == 0) {
        for (i = 0; i < 0xA; i++) {
            if ((g_BattleLimitLunaticHighSelectMask & 1 << i) != 0) {
                pNewTask = (BattleLimitLunaticHighTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLunaticHighSpawnEffectsTask);
                Battle_GetBoneWorldPos(i, g_BattleActorRecords[i].targetBone, &pNewTask->bonePosX);
                pNewTask->bonePosY = 0;
                pNewTask->effectRadius = g_BattleActorRecords[i].radius + 0x12C;
                pNewTask->actorIndex = i;
            }
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleLimitLunaticHighSelectMask), 0x182);
    }
    if (task->frameCounter < 8) {
        g_BattleFxTimelineState.red = 0x48;
        g_BattleFxTimelineState.channel1 = 0x40;
        g_BattleFxTimelineState.channel2 = 0;
        intensity = task->frameCounter << 8;
    } else if (task->frameCounter >= 0x20) {
        intensity = 0x800 - (task->frameCounter - 0x20 << 8);
    } else {
        intensity = 0x800;
    }
    if (task->frameCounter == 0x23) {
        for (i = 0; i < 0xA; i++) {
            if ((g_BattleLimitLunaticHighSelectMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
    }
    if (task->frameCounter >= 0x28) {
        intensity = 0;
        g_BattleFxTimelineState.channel1 = intensity;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        task->ownerSlot = BATTLE_TASK_FREE;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleLimitLunaticHighSelectMask), 0);
    }
    g_BattleFxTimelineState.startValue = intensity;
    (task->frameCounter)++;
}

/*
 * One selected actor's effects, unpaused only. At step 1 it starts the POWER_1 and POWER_2 model tasks
 * at the task position, scaled from the effect radius, and the particle task with the radius and the
 * actor's particle pool, whose 100 particles it clears. The task retires at step 40.
 */
// FUNCTION: FF7 0x004c854a
void __cdecl BattleFx_LimitLunaticHighSpawnEffectsTask(void)
{
    int idx;
    int p;
    int r;
    int s;
    int t;
    int u;
    BattleFxLunaticHighSpawnRecord *childTaskPtr;
    int v;
    BattleFxLunaticHighSpawnSlot *pTaskSlot;
    int y;

    if (g_BattleIsPaused != 0) {
        return;
    }
    pTaskSlot = (BattleFxLunaticHighSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (pTaskSlot->step == 1) {
        childTaskPtr = (BattleFxLunaticHighSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLunaticHighDrawModelEntry1Task);
        childTaskPtr->pos = pTaskSlot->pos;
        childTaskPtr->scale = (pTaskSlot->scale << 0xC << 1) / 0x2EE;
        childTaskPtr = (BattleFxLunaticHighSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLunaticHighDrawModelEntry2Task);
        childTaskPtr->pos = pTaskSlot->pos;
        childTaskPtr->scale = (pTaskSlot->scale << 0xC << 1) / 0x15E;
    }
    if (pTaskSlot->step == 1) {
        childTaskPtr = (BattleFxLunaticHighSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLunaticHighSpriteParticlePool100Task);
        childTaskPtr->pos = pTaskSlot->pos;
        childTaskPtr->scale = pTaskSlot->scale;
        childTaskPtr->poolIndex = pTaskSlot->poolIndex;
        for (idx = 0; idx < 0x64; idx++) {
            g_BattleLimitLunaticHighParticlePool[pTaskSlot->poolIndex][idx].active = 0;
        }
    }
    if (pTaskSlot->step >= 0x28) {
        pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (pTaskSlot->step)++;
}

/*
 * Draws the live particles of the actor's 100-particle pool with the SMOKE_1 sprite (table entry 0),
 * each with its age as the frame; drawing also happens while paused. Unpaused, particles move, their
 * X/Z velocity loses an eighth, and they die after age 15. Before task frame 19, up to five free
 * particles a frame start on a random circle around the task position, sized from the radius, moving
 * outward and up with a random size. The task retires at frame 35.
 */
// FUNCTION: FF7 0x004c86cf
void __cdecl BattleFx_LimitLunaticHighSpriteParticlePool100Task(void)
{
    int idx;
    int theta;
    int sizeBase;
    int freeSlot;
    int r;
    BattleLunaticHighParticle *pParticlePool;
    int dist;
    BattleFxLunaticHighSpriteParticle *task;

    task = (BattleFxLunaticHighSpriteParticle *)BATTLE_CURRENT_TASK_SLOT;
    pParticlePool = g_BattleLimitLunaticHighParticlePool[task->poolIndex];
    g_BattleLimitLunaticHighSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitLunaticHighLoadedResTable);
    g_BattleLimitLunaticHighSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitLunaticHighLoadedResTable);
    for (idx = 0; idx < 0x64; idx++) {
        if (pParticlePool[idx].active != 0) {
            g_BattleLimitLunaticHighSpriteReq.frame = pParticlePool[idx].age;
            BattleFx_InstallBillboardMatrix(&pParticlePool[idx].posX, (short)pParticlePool[idx].scale, -(pParticlePool[idx].scale >> 3));
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitLunaticHighSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
            if (g_BattleIsPaused == 0) {
                pParticlePool[idx].posX += pParticlePool[idx].velocityX;
                pParticlePool[idx].posY += pParticlePool[idx].velocityY;
                pParticlePool[idx].posZ += pParticlePool[idx].velocityZ;
                pParticlePool[idx].velocityX -= (short)(pParticlePool[idx].velocityX >> 3);
                pParticlePool[idx].velocityZ -= (short)(pParticlePool[idx].velocityZ >> 3);
                if (pParticlePool[idx].age >= 0xF) {
                    pParticlePool[idx].active = 0;
                }
                (pParticlePool[idx].age)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCount >= 0x23) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (task->frameCount < 0x13) {
        sizeBase = 0xDC0;
        for (idx = 0; idx < 5; idx++) {
            for (freeSlot = 0; freeSlot < 0x64 && pParticlePool[freeSlot].active != 0; freeSlot++) {
            }
            if (freeSlot >= 0x64) {
                break;
            }
            pParticlePool[freeSlot].active = 1;
            pParticlePool[freeSlot].age = 0;
            theta = ((rand()) % 4096);
            dist = task->speed / 3 * 2 + rand() % 0x64;
            pParticlePool[freeSlot].posX = task->posX + (CosFixed(theta) * dist >> 0xC);
            pParticlePool[freeSlot].posY = task->posY;
            pParticlePool[freeSlot].posZ = task->posZ + (SinFixed(theta) * dist >> 0xC);
            dist = rand() % 0x5A + 0x5A;
            pParticlePool[freeSlot].velocityX = CosFixed(theta) * dist >> 0xC;
            pParticlePool[freeSlot].velocityY = -(rand() % 0xA + 0x14);
            pParticlePool[freeSlot].velocityZ = SinFixed(theta) * dist >> 0xC;
            theta = rand() % 0xB00 + 0xE00;
            pParticlePool[freeSlot].scale = sizeBase * theta >> 0xC;
        }
    }
    (task->frameCount)++;
}

/*
 * Draws POWER_1 (table entry 1) at the task position with the task's uniform scale; the low two bits of
 * the step pick the UV scroll and the texture page bit. The fade level rises over 8 steps, holds, and
 * falls from step 22. Drawing and the retirement at step 30 also happen while paused.
 */
// FUNCTION: FF7 0x004c8b10
void __cdecl BattleFx_LimitLunaticHighDrawModelEntry1Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitLunaticHighEntryOne *entry; };
        struct { unsigned char pad_to_24[0x24]; int uvSelect; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR rotX; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.entry = (BattleFxLimitLunaticHighEntryOne *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.entry->posX;
    frame.modelMatrix.t[1] = frame.entry->posY;
    frame.modelMatrix.t[2] = frame.entry->posZ;
    frame.scale.vz = frame.entry->uniformScale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.uvSelect = frame.entry->step & 3;
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitLunaticHighLoadedResTable);
    frame.modelDraw->flags = 0x28;
    frame.modelDraw->uvScroll = (frame.uvSelect & 1) << 7;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = ((frame.uvSelect & 2) >> 1) | 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.entry->step < 8) {
        frame.modelDraw->fadeLevel = frame.entry->step << 4;
    } else if (frame.entry->step >= 0x16) {
        frame.modelDraw->fadeLevel = 0x80 - (frame.entry->step - 0x16 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.entry->step >= 0x1E) {
        frame.entry->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.entry->step)++;
}

/*
 * Draws POWER_2 (table entry 2) like BattleFx_LimitLunaticHighDrawModelEntry1Task, with its UV frame two
 * steps ahead.
 */
// FUNCTION: FF7 0x004c8cc8
void __cdecl BattleFx_LimitLunaticHighDrawModelEntry2Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitLunaticHighEntryOne *entry; };
        struct { unsigned char pad_to_24[0x24]; int uvSelect; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR rotX; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.entry = (BattleFxLimitLunaticHighEntryOne *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.entry->posX;
    frame.modelMatrix.t[1] = frame.entry->posY;
    frame.modelMatrix.t[2] = frame.entry->posZ;
    frame.scale.vz = frame.entry->uniformScale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.uvSelect = frame.entry->step + 2 & 3;
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleLimitLunaticHighLoadedResTable);
    frame.modelDraw->flags = 0x28;
    frame.modelDraw->uvScroll = (frame.uvSelect & 1) << 7;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = ((frame.uvSelect & 2) >> 1) | 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.entry->step < 8) {
        frame.modelDraw->fadeLevel = frame.entry->step << 4;
    } else if (frame.entry->step >= 0x16) {
        frame.modelDraw->fadeLevel = 0x80 - (frame.entry->step - 0x16 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.entry->step >= 0x1E) {
        frame.entry->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.entry->step)++;
}
} /* extern "C" */

// src/ff7/src/battle/battle_fx_blue_flame_thrower_attach_effect.cpp
// Enemy Skill [8] / Special [70]: Flame Thrower Attached Variant (Japanese: 火炎放射, dev tag: "BLUE/KAENN" / KaennAttach)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The blue magic Flamethrower (Kaen Housha) effect: loads its resources, then spawns and
//   draws flame particles that damage the target, with a trajectory step helper.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memset)

#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x0082ee78
int g_BattleBlueFlameThrowerMagicId = 0x00192A39;
int g_BattleBlueFlameThrowerMagicIdHeadPtr[1] = {
    (int)((char *)&g_BattleFxBlueFlameThrowerAttachEffectResourceNames + 0x0)
};
int g_BattleBlueFlameThrowerMagicIdDataTail[8] = {
    (int)((char *)&g_BattleFxBlueFlameThrowerAttachEffectResourceNames + 0xc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxBlueFlameThrowerAttachEffectResourceNames + 0x18), 0x0,
    0x0, 0x0
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x0082eea0
BattleFxSpriteRequest g_BattleBlueFlameThrowerSpriteRequest = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0};
// The four corner points of the flame spray's source quad, rotated by the attach matrix.
// GLOBAL: FF7 0x0082eeb0
SVECTOR g_BattleBlueFlameThrowerVertexOffsets[4] = {{-300, 150, 0, 0}, {-50, 300, 0, 0}, {50, 300, 0, 0}, {300, 150, 0, 0}};
unsigned char g_BattleFxBlueFlameThrowerAttachEffectResourceNames[36] =
    "FLAME00.TIM\0"
    "KAENN.SPT\0\0\0"
    "FLAME01.TIM";
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
// GLOBAL: FF7 0x00b377b0
char g_BattleBlueFlameThrowerInitializedFlag = 0;
// GLOBAL: FF7 0x00b377b4
int g_BattleBlueFlameThrowerUnused = 0;
// GLOBAL: FF7 0x00b377b8
int g_BattleBlueFlameThrowerTargetMask = 0;
// GLOBAL: FF7 0x00b377bc
char g_BattleBlueFlameThrowerParticleActiveFlag = 0;
// GLOBAL: FF7 0x00b377c0
int g_BattleBlueFlameThrowerActorIndex = 0;
// GLOBAL: FF7 0x00b377c4
struct Vec3sPacked *g_pBattleBlueFlameThrowerScratchPos = 0;
// GLOBAL: FF7 0x00b377c8
BattleBlueFlameThrowerDataBlockPtr g_BattleBlueFlameThrowerDataBlockPtr = {0};
unsigned char g_bssPad_00b377d4[4] = {0};
unsigned char g_bssPad_00b377d8[6364] = {0};
unsigned char g_bssPad_00b390b4[4] = {0};
unsigned char g_bssPad_00b390b8[24] = {0};
// GLOBAL: FF7 0x00b390d0
struct GfxTableCursor *g_BattleBlueFlameThrowerLoadedResTable = 0;
unsigned char g_bssPad_00b390d4[4] = {0};
// clang-format on

/* Loads Flamethrower (Kaen Housha) blue magic resources. */
// FUNCTION: FF7 0x004d3f10
void __cdecl BattleFx_BlueLoadResFlameThrowerAttach(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest requestRecord;
    int resultId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int recordDef;

    if (g_pBattle3dScene != 0 && g_BattleBlueFlameThrowerLoadedResTable == 0) {
        resultId = 2;
        recordDef = (int)&g_BattleBlueFlameThrowerMagicId;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082eef4
        request->resourcePath = BattleFx_BuildResourcePath("BLUE/KAENN", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultId, recordDef, request, &g_BattleBlueFlameThrowerLoadedResTable, sceneContext);
    }
}

/* Entry point for Flamethrower enemy skill / blue magic effect. */
// FUNCTION: FF7 0x004d3fa0
void __cdecl BattleFx_BlueEnterFlameThrowerAttach(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFlameThrowerAttach();
    BattleFx_BlueBeginFlameThrowerAttach(targetMask, actorIdx);
}

/* Initializes target parameters and schedules flame timeline task. */
// FUNCTION: FF7 0x004d3fba
void __cdecl BattleFx_BlueBeginFlameThrowerAttach(int targetMask, int actorIdx)
{
    g_BattleBlueFlameThrowerTargetMask = targetMask;
    g_BattleBlueFlameThrowerActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_BlueFlameThrowerAttachTimelineTask);
}

/* Timeline task generating flame particles and applying damage. */
// FUNCTION: FF7 0x004d3fdd
void __cdecl BattleFx_BlueFlameThrowerAttachTimelineTask(void)
{
    BattleTaskSlot *taskPtr;
    int attachMtx[8];
    int vertexIdx;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_BattleBlueFlameThrowerInitializedFlag & 1) == 0) {
        g_BattleBlueFlameThrowerInitializedFlag |= 1;
        g_BattleBlueFlameThrowerDataBlockPtr.dataBlock = (BattleBlueFlameThrowerScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 0) {
        BattleFx_BlueFlameThrowerAttachClearParticleRecords();
        Battle_AllocateResourceSlot(BattleFx_BlueFlameThrowerAttachDrawParticlesTask);
    }
    if (taskPtr->frameCounter == 0) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x88);
    }
    if (taskPtr->frameCounter >= 0 && taskPtr->frameCounter < 0x3A) {
        Battle3d_GetActorAttachMatrix(g_BattleBlueFlameThrowerActorIndex, 2, (PsxMatrix *)attachMtx);
        Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, attachMtx, &g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->matrix);
        SetRotMatrix(&g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->matrix);
        SetTransMatrix(&g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->matrix);
        for (vertexIdx = 0; vertexIdx < 4; vertexIdx++) {
            RotTransPoint(&g_BattleBlueFlameThrowerVertexOffsets[vertexIdx], (int *)&g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.sourceX, &g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->rotateFlag);
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.centerX = g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.sourceX;
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.centerY = g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.sourceY;
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.centerZ = g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.sourceZ;
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.posX = g_BattleBlueFlameThrowerVertexOffsets[vertexIdx].vx << 3;
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.posY = g_BattleBlueFlameThrowerVertexOffsets[vertexIdx].vy << 3;
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.posZ = 0x960;
            RotateSVectorByInstalledMatrix((CameraScriptVector *)&g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.posX, (int *)&g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.sourceX);
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.posX = g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.sourceX;
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.posY = g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.sourceY;
            g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.posZ = g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.sourceZ;
            BattleFx_BlueFlameThrowerAttachSpawnParticles(&g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn, (Vec3sPacked *)&g_BattleBlueFlameThrowerDataBlockPtr.dataBlock->spawn.posX, taskPtr->frameCounter, 2);
        }
    }
    if (taskPtr->frameCounter == 0x32) {
        for (vertexIdx = 0; vertexIdx < 0xA; vertexIdx++) {
            if ((g_BattleBlueFlameThrowerTargetMask & 1 << vertexIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(vertexIdx);
            }
        }
    }
    if (taskPtr->frameCounter == 0x4E) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->frameCounter)++;
}

/* Walks active particle array and triggers particle rendering. */
// FUNCTION: FF7 0x004d4291
void __cdecl BattleFx_BlueFlameThrowerAttachDrawParticlesTask(void)
{
    int activeCount;
    BattleTaskSlot *task;
    int particleIdx;

    task = BATTLE_CURRENT_TASK_SLOT;
    activeCount = 0;
    for (particleIdx = 0; particleIdx < 0xC8; particleIdx++) {
        if (((BattleFxFlameThrowerParticle *)&g_BattleBlueFlameThrowerDataBlockPtr.particlePool)[particleIdx].state != -1) {
            BattleFx_BlueFlameThrowerAttachDrawParticle((BattleBlueFlameThrowerParticle *)&((BattleFxFlameThrowerParticle *)&g_BattleBlueFlameThrowerDataBlockPtr.particlePool)[particleIdx]);
            activeCount++;
        }
    }
    if (activeCount == 0 && g_BattleSecondaryTaskActiveCount <= 2) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Transforms and draws individual flame billboard particle. */
// FUNCTION: FF7 0x004d431b
void __cdecl BattleFx_BlueFlameThrowerAttachDrawParticle(BattleBlueFlameThrowerParticle *particleRecord)
{
    int sine;
    int dampedOfs;
    int unusedLift;

    if ((*(unsigned char *)&g_BattleBlueFlameThrowerParticleActiveFlag & 1) == 0) {
        g_BattleBlueFlameThrowerParticleActiveFlag |= 1;
        g_pBattleBlueFlameThrowerScratchPos = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleBlueFlameThrowerSpriteRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueFlameThrowerLoadedResTable);
    g_BattleBlueFlameThrowerSpriteRequest.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueFlameThrowerLoadedResTable);
    if (g_BattleBlueFlameThrowerSpriteRequest.spriteGroup != 0) {
        ((BattleFxBlueFlameThrowerDrawReq *)g_BattleBlueFlameThrowerSpriteRequest.spriteGroup)->drawObj0 = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueFlameThrowerLoadedResTable);
        ((BattleFxBlueFlameThrowerDrawReq *)g_BattleBlueFlameThrowerSpriteRequest.spriteGroup)->drawObj1 = Gfx_ModelTableGetSpriteObject(1, g_BattleBlueFlameThrowerLoadedResTable);
    }
    sine = SinFixed(particleRecord->angleStep * particleRecord->frameCounter / 0x14);
    if (particleRecord->offsetY < 0) {
        dampedOfs = particleRecord->offsetY;
    } else {
        dampedOfs = particleRecord->offsetY / 4;
    }
    unusedLift = dampedOfs;
    g_pBattleBlueFlameThrowerScratchPos->vy = particleRecord->baseY + (particleRecord->driftY * sine >> 0xC) + particleRecord->offsetY;
    if (g_pBattleBlueFlameThrowerScratchPos->vy > 0) {
        g_pBattleBlueFlameThrowerScratchPos->vy = -rand() % 0x64;
    }
    g_pBattleBlueFlameThrowerScratchPos->vx = particleRecord->baseX + (particleRecord->driftX * sine >> 0xC);
    g_pBattleBlueFlameThrowerScratchPos->vz = particleRecord->baseZ + (particleRecord->driftZ * sine >> 0xC);
    g_BattleBlueFlameThrowerSpriteRequest.frame = particleRecord->frameCounter * 0xA / particleRecord->period;
    BattleFx_InstallBillboardMatrix(g_pBattleBlueFlameThrowerScratchPos, 0x1800, 0xFFFFFE80);
    g_BattleBlueFlameThrowerUnused = BattleFx_SubmitSpriteDraw(&g_BattleBlueFlameThrowerSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleBlueFlameThrowerUnused);
    if (g_BattleIsPaused == 0) {
        (particleRecord->frameCounter)++;
        if (particleRecord->frameCounter >= particleRecord->period) {
            particleRecord->ownerSlot = BATTLE_TASK_FREE;
        } else {
            particleRecord->offsetY += particleRecord->velocityY;
            particleRecord->velocityY += particleRecord->accelerationY;
        }
    }
}

/* Resets all flame particle records to inactive state. */
// FUNCTION: FF7 0x004d4557
void __cdecl BattleFx_BlueFlameThrowerAttachClearParticleRecords(void)
{
    int i;

    for (i = 0; i < 0xC8; i++) {
        ((BattleFxFlameThrowerParticle *)&g_BattleBlueFlameThrowerDataBlockPtr.particlePool)[i].state = 0xFFFF;
    }
}

/* Spawns flame particles with random velocities from origin. */
// FUNCTION: FF7 0x004d458b
void __cdecl BattleFx_BlueFlameThrowerAttachSpawnParticles(BattleFxSpawnCenter *count, Vec3sPacked *posX, int posY, int posZ)
{
    int amount;
    BattleFxFlameThrowerParticle *particle;
    int i;
    int accelDraw;
    int speedValue;

    for (i = 0; i < posZ; i++) {
        particle = (BattleFxFlameThrowerParticle *)BattleFx_BlueFlameThrowerAttachClaimParticleRecord();
        particle->lifeFrames = rand() % 0xF + 0xA;
        particle->posX = count->centerX + rand() % 0x64 + -0x32;
        particle->posY = count->centerY + rand() % 0x64 + -0x32;
        particle->posZ = count->centerZ + rand() % 0x64 + -0x32;
        amount = (SinFixed((posY << 0xC) / 6) << 0xA >> 0xC) + rand() % 0x400 + 0xE00;
        particle->ampX = posX->vx * amount >> 0xC;
        particle->ampY = posX->vy * amount >> 0xC;
        particle->ampZ = posX->vz * amount >> 0xC;
        particle->offsetY = 0;
        particle->velocityY = 0x4B;
        if (DEAD_IN_RETAIL != 0) {
            accelDraw = 0xA;
        } else {
            accelDraw = rand() % 5 + 5;
        }
        particle->accelY = -accelDraw;
        if (DEAD_IN_RETAIL != 0) {
            speedValue = 0x480;
        } else {
            speedValue = rand() % 0x80 + 0x400;
        }
        particle->angleSpeed = speedValue;
    }
}

/* Finds and claims the next available flame particle record slot. */
// FUNCTION: FF7 0x004d4722
int __cdecl BattleFx_BlueFlameThrowerAttachClaimParticleRecord(void)
{
    int i;

    for (i = 0; i < 0xC8; i++) {
        if (((BattleFxFlameThrowerParticle *)&g_BattleBlueFlameThrowerDataBlockPtr.particlePool)[i].state == -1) {
            ((BattleFxFlameThrowerParticle *)&g_BattleBlueFlameThrowerDataBlockPtr.particlePool)[i].state = 0;
            ((BattleFxFlameThrowerParticle *)&g_BattleBlueFlameThrowerDataBlockPtr.particlePool)[i].frameCounter = 0;
            return (int)&((BattleFxFlameThrowerParticle *)&g_BattleBlueFlameThrowerDataBlockPtr.particlePool)[i];
        }
    }
    return 0;
}
} /* extern "C" */

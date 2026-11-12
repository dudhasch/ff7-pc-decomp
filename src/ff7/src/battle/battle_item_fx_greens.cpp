// src/ff7/src/battle/battle_item_fx_greens.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Item effect for Greens: identical entry points that start a per-target projectile task,
//   which runs the item effect's flight and impact on each target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x0089a338
unsigned int g_BattleItemGreensResourceDef[80] = {0x000a00b6, (unsigned int)g_BattleItemGreensModelNames[0], 0x00000000, 0x00000000, 0x00000000, 0x000a00b6, (unsigned int)g_BattleItemGreensModelNames[1], 0x00000000, 0x00000000, 0x00000000, 0x000600b6, (unsigned int)g_BattleItemGreensModelNames[2], 0x00000000, 0x00000000, 0x00000000, 0x000500b6, (unsigned int)g_BattleItemGreensModelNames[3], 0x00000000, 0x00000000, 0x00000000, 0x000500b6, (unsigned int)g_BattleItemGreensModelNames[4], 0x00000000, 0x00000000, 0x00000000, 0x000500b6, (unsigned int)g_BattleItemGreensModelNames[5], 0x00000000, 0x00000000, 0x00000000, 0x000a00b6, (unsigned int)g_BattleItemGreensModelNames[6], 0x00000000, 0x00000000, 0x00000000, 0x000f00b6, (unsigned int)g_BattleItemGreensModelNames[7], 0x00000000, 0x00000000, 0x00000000, 0x000a00b1, (unsigned int)g_BattleItemGreensModelNames[8], 0x00000000, 0x00000000, 0x00000000, 0x000a00b1, (unsigned int)g_BattleItemGreensModelNames[9], 0x00000000, 0x00000000, 0x00000000, 0x000600b1, (unsigned int)g_BattleItemGreensModelNames[10], 0x00000000, 0x00000000, 0x00000000, 0x000500b1, (unsigned int)g_BattleItemGreensModelNames[11], 0x00000000, 0x00000000, 0x00000000, 0x000500b1, (unsigned int)g_BattleItemGreensModelNames[12], 0x00000000, 0x00000000, 0x00000000, 0x000500b1, (unsigned int)g_BattleItemGreensModelNames[13], 0x00000000, 0x00000000, 0x00000000, 0x000a00b1, (unsigned int)g_BattleItemGreensModelNames[14], 0x00000000, 0x00000000, 0x00000000, 0x000f00b1, (unsigned int)g_BattleItemGreensModelNames[15], 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0089a478
BattleFxModelDrawHead g_BattleItemGreensSpriteRequest1 = {0, 0x180, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x0089a488
BattleFxModelDrawHead g_BattleItemGreensSpriteRequest2 = {0, 0xa8, 0, 0, 0x40, 0};
// GLOBAL: FF7 0x0089a498
PsxMatrix g_BattleItemGreensTransformMatrix = {{{0x400, 0, 0}, {0, 0x400, 0}, {0, 0, 0x400}}, {0, 0, 0}};
// GLOBAL: FF7 0x0089a4b8
char g_BattleItemGreensModelNames[16][12] = {
    "YASAI_1.RSD",
    "YASAI_2.RSD",
    "YASAI_3.RSD",
    "YASAI_4.RSD",
    "YASAI_5.RSD",
    "YASAI_6.RSD",
    "YASAI_7.RSD",
    "YASAI_8.RSD",
    "YASAI_1.RSD",
    "YASAI_2.RSD",
    "YASAI_3.RSD",
    "YASAI_4.RSD",
    "YASAI_5.RSD",
    "YASAI_6.RSD",
    "YASAI_7.RSD",
    "YASAI_8.RSD"
};
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b51cf4
int g_BattleItemGreensVariantIndex = 0;
// GLOBAL: FF7 0x00b51cf8
BattleItemGreensScratch *g_pBattleItemGreensScratch = 0;
// GLOBAL: FF7 0x00b51cfc
int g_BattleItemGreensFading = 0;
// GLOBAL: FF7 0x00b51d00
unsigned char g_BattleItemGreensScratchInit = 0;
// GLOBAL: FF7 0x00b51d04
unsigned char g_BattleItemGreensSourcePosInit = 0;
// GLOBAL: FF7 0x00b51d08
int g_BattleItemGreensPacketCursor = 0;
// GLOBAL: FF7 0x00b51d0c
struct SVECTOR *g_BattleItemGreensSourcePos = 0;
// GLOBAL: FF7 0x00b51d10
int g_BattleItemGreensModelResourceIndex = 0;
// GLOBAL: FF7 0x00b51d14
struct GfxTableCursor *g_BattleItemGreensLoadedResTable = 0;
// clang-format on

/*
 * When the battle scene exists and g_BattleItemGreensLoadedResTable is empty, loads the sixteen
 * YASAI_n.RSD models of g_BattleItemGreensResourceDef from ITEM/VEGET.
 */
// FUNCTION: FF7 0x00514710
void __cdecl Battle_ItemFxGreensInit(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleItemGreensLoadedResTable == 0) {
        kind = 0x10;
        stepResourceDef = (int)&g_BattleItemGreensResourceDef;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089a578
        pRequest->resourcePath = BattleFx_BuildResourcePath("ITEM/VEGET", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, pRequest, &g_BattleItemGreensLoadedResTable, stageSceneHandle);
    }
}

/*
 * One of eight identical entry points for the item effect: calls Battle_ItemFxGreensInit, sets the
 * variant selector g_BattleItemGreensVariantIndex to 0 and g_BattleItemGreensModelResourceIndex to 8, then hands target and param to
 * Battle_ItemFxGreensSpawn. Only those two constants distinguish it from Battle_ItemFxGreens1
 * through 7.
 */
// FUNCTION: FF7 0x005147a0
void __cdecl Battle_ItemFxGreens0(int target, int param)
{
    Battle_ItemFxGreensInit();
    g_BattleItemGreensVariantIndex = 0;
    g_BattleItemGreensModelResourceIndex = 8;
    Battle_ItemFxGreensSpawn(target, param);
}

/*
 * One of eight identical entry points for the item effect: calls Battle_ItemFxGreensInit, sets
 * g_BattleItemGreensVariantIndex to 1 and g_BattleItemGreensModelResourceIndex to 9, then hands target and param to Battle_ItemFxGreensSpawn.
 */
// FUNCTION: FF7 0x005147ce
void __cdecl Battle_ItemFxGreens1(int target, int param)
{
    Battle_ItemFxGreensInit();
    g_BattleItemGreensVariantIndex = 1;
    g_BattleItemGreensModelResourceIndex = 9;
    Battle_ItemFxGreensSpawn(target, param);
}

/*
 * One of eight identical entry points for the item effect: calls Battle_ItemFxGreensInit, sets
 * g_BattleItemGreensVariantIndex to 2 and g_BattleItemGreensModelResourceIndex to 0xA, then hands target and param to Battle_ItemFxGreensSpawn.
 */
// FUNCTION: FF7 0x005147fc
void __cdecl Battle_ItemFxGreens2(int target, int param)
{
    Battle_ItemFxGreensInit();
    g_BattleItemGreensVariantIndex = 2;
    g_BattleItemGreensModelResourceIndex = 0xA;
    Battle_ItemFxGreensSpawn(target, param);
}

/*
 * One of eight identical entry points for the item effect: calls Battle_ItemFxGreensInit, sets
 * g_BattleItemGreensVariantIndex to 3 and g_BattleItemGreensModelResourceIndex to 0xB, then hands target and param to Battle_ItemFxGreensSpawn.
 */
// FUNCTION: FF7 0x0051482a
void __cdecl Battle_ItemFxGreens3(int target, int param)
{
    Battle_ItemFxGreensInit();
    g_BattleItemGreensVariantIndex = 3;
    g_BattleItemGreensModelResourceIndex = 0xB;
    Battle_ItemFxGreensSpawn(target, param);
}

/*
 * One of eight identical entry points for the item effect: calls Battle_ItemFxGreensInit, sets
 * g_BattleItemGreensVariantIndex to 4 and g_BattleItemGreensModelResourceIndex to 0xC, then hands targetIndex and sourceIndex to Battle_ItemFxGreensSpawn.
 */
// FUNCTION: FF7 0x00514858
void __cdecl Battle_ItemFxGreens4(int targetIndex, int sourceIndex)
{
    Battle_ItemFxGreensInit();
    g_BattleItemGreensVariantIndex = 4;
    g_BattleItemGreensModelResourceIndex = 0xC;
    Battle_ItemFxGreensSpawn(targetIndex, sourceIndex);
}

/*
 * One of eight identical entry points for the item effect: calls Battle_ItemFxGreensInit, sets
 * g_BattleItemGreensVariantIndex to 5 and g_BattleItemGreensModelResourceIndex to 0xD, then hands targetIndex and sourceIndex to Battle_ItemFxGreensSpawn.
 */
// FUNCTION: FF7 0x00514886
void __cdecl Battle_ItemFxGreens5(int targetIndex, int sourceIndex)
{
    Battle_ItemFxGreensInit();
    g_BattleItemGreensVariantIndex = 5;
    g_BattleItemGreensModelResourceIndex = 0xD;
    Battle_ItemFxGreensSpawn(targetIndex, sourceIndex);
}

/*
 * One of eight identical entry points for the item effect: calls Battle_ItemFxGreensInit, sets
 * g_BattleItemGreensVariantIndex to 6 and g_BattleItemGreensModelResourceIndex to 0xE, then hands targetIndex and sourceIndex to Battle_ItemFxGreensSpawn.
 */
// FUNCTION: FF7 0x005148b4
void __cdecl Battle_ItemFxGreens6(int targetIndex, int sourceIndex)
{
    Battle_ItemFxGreensInit();
    g_BattleItemGreensVariantIndex = 6;
    g_BattleItemGreensModelResourceIndex = 0xE;
    Battle_ItemFxGreensSpawn(targetIndex, sourceIndex);
}

/*
 * One of eight identical entry points for the item effect: calls Battle_ItemFxGreensInit, sets
 * g_BattleItemGreensVariantIndex to 7 and g_BattleItemGreensModelResourceIndex to 0xF, then hands targetIndex and sourceIndex to Battle_ItemFxGreensSpawn.
 */
// FUNCTION: FF7 0x005148e2
void __cdecl Battle_ItemFxGreens7(int targetIndex, int sourceIndex)
{
    Battle_ItemFxGreensInit();
    g_BattleItemGreensVariantIndex = 7;
    g_BattleItemGreensModelResourceIndex = 0xF;
    Battle_ItemFxGreensSpawn(targetIndex, sourceIndex);
}

/*
 * Clears g_BattleItemGreensFading and creates a task for Battle_ItemFxGreensSpawnPerTarget with both arguments and mode zero.
 */
// FUNCTION: FF7 0x00514910
void __cdecl Battle_ItemFxGreensSpawn(int targetIndex, int sourceIndex)
{
    g_BattleItemGreensFading = 0;
    Battle_CreateBitPatternCallbackTask(targetIndex, sourceIndex, 0, Battle_ItemFxGreensSpawnPerTarget);
}

/*
 * Lazily acquires data block zero and allocates Battle_ItemFxGreensProjectileTask with actorIndex set to actor and
 * pos copied from the source actor's bone position. Computes a point behind the target actor's facing at
 * radius plus 300, derives sixteenth-step X/Z velocities with Y velocity -200, computes the gravity
 * accelerationY from the source Y and clears the three rotation angles.
 */
// FUNCTION: FF7 0x00514936
void __cdecl Battle_ItemFxGreensSpawnPerTarget(int actor, int sourceIndex)
{
    BattleItemFxGreensProjectile *projectile;

    if ((g_BattleItemGreensSourcePosInit & 1) == 0) {
        g_BattleItemGreensSourcePosInit |= 1;
        g_BattleItemGreensSourcePos = (SVECTOR *)Gfx_GetPsxScratchpadPtr(0);
    }
    projectile = (BattleItemFxGreensProjectile *)BATTLE_NEW_TASK_SLOT(Battle_ItemFxGreensProjectileTask);
    projectile->actorIndex = actor;
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].targetBone, (short *)g_BattleItemGreensSourcePos);
    projectile->pos = *g_BattleItemGreensSourcePos;
    g_BattleItemGreensSourcePos[1].vx = g_BattleActorRecords[actor].posX - (SinFixed(g_BattleActorRecords[actor].yaw) * (g_BattleActorRecords[actor].radius + 0x12C) >> 0xC);
    g_BattleItemGreensSourcePos[1].vy = 0;
    g_BattleItemGreensSourcePos[1].vz = g_BattleActorRecords[actor].posZ - (CosFixed(g_BattleActorRecords[actor].yaw) * (g_BattleActorRecords[actor].radius + 0x12C) >> 0xC);
    projectile->velocityX = (g_BattleItemGreensSourcePos[1].vx - g_BattleItemGreensSourcePos->vx) / 16;
    projectile->velocityY = 0xFF38;
    projectile->velocityZ = (g_BattleItemGreensSourcePos[1].vz - g_BattleItemGreensSourcePos->vz) / 16;
    projectile->accelerationY = (0xAF0 - g_BattleItemGreensSourcePos->vy << 1) / 0xC4;
    projectile->angles.vz = 0;
    projectile->angles.vy = 0;
    projectile->angles.vx = 0;
}

/*
 * Battle_ItemFxGreensProjectileTask: flies the Greens projectile, bursts it into sprites on landing, then frees it.
 */
// FUNCTION: FF7 0x00514b07
void __cdecl Battle_ItemFxGreensProjectileTask(void)
{
    int angle, phaseFrame;
    int i;
    BattleItemFxGreensProjectile *pProjectile;

    pProjectile = (BattleItemFxGreensProjectile *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleItemGreensScratchInit & 1) == 0) {
        g_BattleItemGreensScratchInit |= 1;
        g_pBattleItemGreensScratch = (BattleItemGreensScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_pBattle3dScene != 0) {
        g_BattleItemGreensSpriteRequest2.modelGroup = g_pBattle3dScene->modelRequest;
    } else {
        g_BattleItemGreensSpriteRequest2.modelGroup = 0;
    }
    g_BattleItemGreensSpriteRequest1.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleItemGreensVariantIndex, g_BattleItemGreensLoadedResTable);
    g_BattleItemGreensSpriteRequest1.flags = 0x80;
    g_BattleItemGreensSpriteRequest1.fadeLevel = 0;
    g_BattleItemGreensSpriteRequest2.fadeLevel = 0;
    if (g_BattleItemGreensFading != 0) {
        g_BattleItemGreensSpriteRequest1.flags = 0x88;
        g_BattleItemGreensSpriteRequest1.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleItemGreensModelResourceIndex, g_BattleItemGreensLoadedResTable);
    }
    RotMatrixXYZ(&pProjectile->angles, &g_pBattleItemGreensScratch->matrix);
    g_BattleItemGreensTransformMatrix.t[0] = pProjectile->pos.vx;
    g_pBattleItemGreensScratch->matrix.t[0] = g_BattleItemGreensTransformMatrix.t[0];
    g_pBattleItemGreensScratch->matrix.t[1] = pProjectile->pos.vy;
    g_BattleItemGreensTransformMatrix.t[2] = pProjectile->pos.vz;
    g_pBattleItemGreensScratch->matrix.t[2] = g_BattleItemGreensTransformMatrix.t[2];
    MultiplyMatrix(&g_BattleCameraTransform, &g_pBattleItemGreensScratch->matrix, &g_pBattleItemGreensScratch->matrix);
    SetRotMatrix(&g_pBattleItemGreensScratch->matrix);
    SetTransMatrix(&g_pBattleItemGreensScratch->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleItemGreensPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleItemGreensSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleItemGreensPacketCursor);
    g_BattleItemGreensTransformMatrix.t[1] = 0;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleItemGreensTransformMatrix, &g_pBattleItemGreensScratch->matrix);
    SetRotMatrix(&g_pBattleItemGreensScratch->matrix);
    SetTransMatrix(&g_pBattleItemGreensScratch->matrix);
    g_BattleItemGreensPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleItemGreensSpriteRequest2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 0, g_BattleItemGreensPacketCursor);
    if (g_BattleIsPaused == 0) {
        phaseFrame = pProjectile->frame;
        if (phaseFrame < 0xF) {
            pProjectile->pos.vx += pProjectile->velocityX;
            pProjectile->pos.vy += pProjectile->velocityY;
            pProjectile->pos.vz += pProjectile->velocityZ;
            pProjectile->velocityY += pProjectile->accelerationY;
        } else {
            phaseFrame -= 0xF;
            if (phaseFrame < 4) {
                if (phaseFrame == 0) {
                    pProjectile->velocityX = pProjectile->velocityX / 4;
                    pProjectile->velocityY = 0xFFC4;
                    pProjectile->velocityZ = pProjectile->velocityZ / 4;
                    for (i = 0; i < 5; i++) {
                        angle = (i << 0xC) / 5 + rand() % 0x333;
                        g_pBattleItemGreensScratch->pos.vx = pProjectile->pos.vx + (SinFixed(angle) * 0x64 >> 0xC);
                        g_pBattleItemGreensScratch->pos.vy = 0;
                        g_pBattleItemGreensScratch->pos.vz = pProjectile->pos.vz + (CosFixed(angle) * 0x64 >> 0xC);
                        Battle_SpawnWorldSpriteTask(&g_pBattleItemGreensScratch->pos, 0x1000, 0x1000);
                    }
                    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&pProjectile->pos), 0xCC);
                }
                pProjectile->pos.vx += pProjectile->velocityX;
                pProjectile->pos.vy += pProjectile->velocityY;
                pProjectile->pos.vz += pProjectile->velocityZ;
                pProjectile->velocityY += 0x1E;
            } else {
                phaseFrame -= 4;
                if (phaseFrame < 0xA) {
                    if (phaseFrame == 0) {
                        for (i = 0; i < 5; i++) {
                            angle = (i << 0xC) / 5 + rand() % 0x333;
                            g_pBattleItemGreensScratch->pos.vx = pProjectile->pos.vx + (SinFixed(angle) * 0x64 >> 0xC);
                            g_pBattleItemGreensScratch->pos.vy = 0;
                            g_pBattleItemGreensScratch->pos.vz = pProjectile->pos.vz + (CosFixed(angle) * 0x64 >> 0xC);
                            Battle_SpawnWorldSpriteTask(&g_pBattleItemGreensScratch->pos, 0x1000, 0x1000);
                        }
                        pProjectile->pos.vy = 0;
                    }
                } else {
                    phaseFrame -= 0xA;
                    if (phaseFrame < 0xA) {
                        if (phaseFrame == 0) {
                            g_BattleItemGreensFading = 1;
                        }
                        g_BattleItemGreensSpriteRequest2.fadeLevel = (phaseFrame << 0xC) / 0xA;
                        g_BattleItemGreensSpriteRequest1.fadeLevel = g_BattleItemGreensSpriteRequest2.fadeLevel;
                    } else {
                        Battle_PostActorHitOrHurtMessage(pProjectile->actorIndex);
                        pProjectile->actorIndex = 0xFFFF;
                        return;
                    }
                }
            }
        }
        (pProjectile->frame)++;
    }
}
} /* extern "C" */

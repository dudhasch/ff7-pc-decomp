// src/ff7/src/battle/battle_fx_throw_barret.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Throw effects for Barret's arm weapons: per-weapon entry points that dispatch a
//   straight or tumbling flight, with timeline and projectile tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "lib/api/sound.h"
#include "ff7/enums/battle.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x0082e5c8
unsigned int g_BattleThrowBarretResourceDef[150] = {0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon01, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon02, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon03, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon04, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon05, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon06, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon07, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon08, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon09, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon10, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon11, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon12, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon13, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon14, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowBarretResNameWeapon15_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon01_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon02_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon03_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon04_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon05_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon06_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon07_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon08_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon09_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon10_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon11_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon12_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon13_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon14_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowBarretResNameWeapon15, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0082e820
unsigned char g_BattleThrowBarretVectorOffset[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff};
// GLOBAL: FF7 0x0082e830
unsigned char g_BattleThrowBarretResNameWeapon01[13] = "brhand01.rsd";
// GLOBAL: FF7 0x0082e840
unsigned char g_BattleThrowBarretResNameWeapon02[13] = "brhand02.rsd";
// GLOBAL: FF7 0x0082e850
unsigned char g_BattleThrowBarretResNameWeapon03[13] = "brhand03.rsd";
// GLOBAL: FF7 0x0082e860
unsigned char g_BattleThrowBarretResNameWeapon04[13] = "brhand04.rsd";
// GLOBAL: FF7 0x0082e870
unsigned char g_BattleThrowBarretResNameWeapon05[13] = "brhand05.rsd";
// GLOBAL: FF7 0x0082e880
unsigned char g_BattleThrowBarretResNameWeapon06[13] = "brhand06.rsd";
// GLOBAL: FF7 0x0082e890
unsigned char g_BattleThrowBarretResNameWeapon07[13] = "brhand07.rsd";
// GLOBAL: FF7 0x0082e8a0
unsigned char g_BattleThrowBarretResNameWeapon08[13] = "brhand08.rsd";
// GLOBAL: FF7 0x0082e8b0
unsigned char g_BattleThrowBarretResNameWeapon09[13] = "brhand09.rsd";
// GLOBAL: FF7 0x0082e8c0
unsigned char g_BattleThrowBarretResNameWeapon10[13] = "brhand10.rsd";
// GLOBAL: FF7 0x0082e8d0
unsigned char g_BattleThrowBarretResNameWeapon11[13] = "brhand11.rsd";
// GLOBAL: FF7 0x0082e8e0
unsigned char g_BattleThrowBarretResNameWeapon12[13] = "brhand12.rsd";
// GLOBAL: FF7 0x0082e8f0
unsigned char g_BattleThrowBarretResNameWeapon13[13] = "brhand13.rsd";
// GLOBAL: FF7 0x0082e900
unsigned char g_BattleThrowBarretResNameWeapon14[13] = "brhand14.rsd";
// GLOBAL: FF7 0x0082e910
unsigned char g_BattleThrowBarretResNameWeapon15_B[13] = "brhand15.rsd";
// GLOBAL: FF7 0x0082e920
unsigned char g_BattleThrowBarretResNameWeapon01_B[13] = "brhand01.rsd";
// GLOBAL: FF7 0x0082e930
unsigned char g_BattleThrowBarretResNameWeapon02_B[13] = "brhand02.rsd";
// GLOBAL: FF7 0x0082e940
unsigned char g_BattleThrowBarretResNameWeapon03_B[13] = "brhand03.rsd";
// GLOBAL: FF7 0x0082e950
unsigned char g_BattleThrowBarretResNameWeapon04_B[13] = "brhand04.rsd";
// GLOBAL: FF7 0x0082e960
unsigned char g_BattleThrowBarretResNameWeapon05_B[13] = "brhand05.rsd";
// GLOBAL: FF7 0x0082e970
unsigned char g_BattleThrowBarretResNameWeapon06_B[13] = "brhand06.rsd";
// GLOBAL: FF7 0x0082e980
unsigned char g_BattleThrowBarretResNameWeapon07_B[13] = "brhand07.rsd";
// GLOBAL: FF7 0x0082e990
unsigned char g_BattleThrowBarretResNameWeapon08_B[13] = "brhand08.rsd";
// GLOBAL: FF7 0x0082e9a0
unsigned char g_BattleThrowBarretResNameWeapon09_B[13] = "brhand09.rsd";
// GLOBAL: FF7 0x0082e9b0
unsigned char g_BattleThrowBarretResNameWeapon10_B[13] = "brhand10.rsd";
// GLOBAL: FF7 0x0082e9c0
unsigned char g_BattleThrowBarretResNameWeapon11_B[13] = "brhand11.rsd";
// GLOBAL: FF7 0x0082e9d0
unsigned char g_BattleThrowBarretResNameWeapon12_B[13] = "brhand12.rsd";
// GLOBAL: FF7 0x0082e9e0
unsigned char g_BattleThrowBarretResNameWeapon13_B[13] = "brhand13.rsd";
// GLOBAL: FF7 0x0082e9f0
unsigned char g_BattleThrowBarretResNameWeapon14_B[13] = "brhand14.rsd";
// GLOBAL: FF7 0x0082ea00
unsigned char g_BattleThrowBarretResNameWeapon15[13] = "brhand15.rsd";
// GLOBAL: FF7 0x00b37700
int g_BattleThrowBarretTargetIndex = 0;
unsigned char g_bssPad_00b37704[4] = {0};
// GLOBAL: FF7 0x00b37708
int g_BattleThrowBarretDeltaVectorX = 0;
// GLOBAL: FF7 0x00b3770c
int g_BattleThrowBarretDeltaVectorY = 0;
// GLOBAL: FF7 0x00b37710
int g_BattleThrowBarretDeltaVectorZ = 0;
unsigned char g_bssPad_00b37714[4] = {0};
// GLOBAL: FF7 0x00b37718
BattleFxThrowSourcePos g_BattleThrowBarretSourcePos = {0};
// GLOBAL: FF7 0x00b37720
int g_BattleThrowBarretWeaponFlag = 0;
// GLOBAL: FF7 0x00b37724
int g_BattleThrowBarretWeaponIndex = 0;
// GLOBAL: FF7 0x00b37728
Vec3sPacked g_BattleThrowBarretTargetPos = {0};
// GLOBAL: FF7 0x00b37730
int g_BattleThrowBarretTargetMask = 0;
unsigned char g_bssPad_00b37734[4] = {0};
// GLOBAL: FF7 0x00b37738
Vec3sPacked g_BattleThrowBarretTargetAngles = {0};
// GLOBAL: FF7 0x00b37740
int g_BattleThrowBarretActorIndex = 0;
unsigned char g_bssPad_00b37744[4] = {0};
// GLOBAL: FF7 0x00b37748
VECTOR g_BattleThrowBarretTransformedVector = {0};
// GLOBAL: FF7 0x00b37758
struct GfxTableCursor *g_BattleThrowBarretLoadedResTable = 0;
unsigned char g_bssPad_00b3775c[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma function(memcpy)
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

/* Loads Barret arm weapon models and throw effect resources. */
// FUNCTION: FF7 0x004d2830
void __cdecl BattleFx_ThrowLoadResBarretWeapons(void)
{
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest requestBuffer;
    GfxModelTableBank *loadParam;
    GfxResourceRequest *request;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleThrowBarretLoadedResTable == 0) {
        typeId = 0x1E;
        def = (int)&g_BattleThrowBarretResourceDef;
        loadParam = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082ea10
        request->resourcePath = BattleFx_BuildResourcePath("item/wp_ba", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, def, request, &g_BattleThrowBarretLoadedResTable, loadParam);
    }
}

/* Throws Barret weapon 1 (Gatling Gun). */
// FUNCTION: FF7 0x004d28c0
void __cdecl Battle_ThrowFxWeaponBlock32(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 0;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 2 (Assault Gun). */
// FUNCTION: FF7 0x004d28e9
void __cdecl Battle_ThrowFxWeaponBlock32Index1(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 1;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 3 (Cannon Ball). */
// FUNCTION: FF7 0x004d2912
void __cdecl Battle_ThrowFxWeaponBlock32Index2(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 2;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 4 (Atomic Scissors). */
// FUNCTION: FF7 0x004d293b
void __cdecl Battle_ThrowFxWeaponBlock32Index3(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 3;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 5 (Heavy Vulcan). */
// FUNCTION: FF7 0x004d2964
void __cdecl Battle_ThrowFxWeaponBlock32Index4(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 4;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 6 (Chainsaw). */
// FUNCTION: FF7 0x004d298d
void __cdecl Battle_ThrowFxWeaponBlock32Index5(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 5;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 7 (Microlaser). */
// FUNCTION: FF7 0x004d29b6
void __cdecl Battle_ThrowFxWeaponBlock32Index6(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 6;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 8 (A-M Cannon). */
// FUNCTION: FF7 0x004d29df
void __cdecl Battle_ThrowFxWeaponBlock32Index7(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 7;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 9 (W-MachineGun). */
// FUNCTION: FF7 0x004d2a08
void __cdecl Battle_ThrowFxWeaponBlock32Index8(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 8;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 10 (Drill Arm). */
// FUNCTION: FF7 0x004d2a31
void __cdecl Battle_ThrowFxWeaponBlock32Index9(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 9;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 11 (Solid Bazooka). */
// FUNCTION: FF7 0x004d2a5a
void __cdecl Battle_ThrowFxWeaponBlock32Index10(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 0xA;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 12 (Rocket Punch). */
// FUNCTION: FF7 0x004d2a83
void __cdecl Battle_ThrowFxWeaponBlock32Index11(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 0xB;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 13 (Enemy Launcher). */
// FUNCTION: FF7 0x004d2aac
void __cdecl Battle_ThrowFxWeaponBlock32Index12(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 0xC;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 14 (Pile Bunker). */
// FUNCTION: FF7 0x004d2ad5
void __cdecl Battle_ThrowFxWeaponBlock32Index13(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 0xD;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 15 (Max Ray). */
// FUNCTION: FF7 0x004d2afe
void __cdecl Battle_ThrowFxWeaponBlock32Index14(int targetMask, int actorIndex)
{
    g_BattleThrowBarretWeaponIndex = 0xE;
    g_BattleThrowBarretWeaponFlag = BATTLE_BARRET_THROW_TUMBLING;
    Battle_ThrowFxSpawnWeapons32to47(targetMask, actorIndex);
}

/* Throws Barret weapon 16 (Missing Score). */
// FUNCTION: FF7 0x004d2b27
void __cdecl Battle_ThrowFxSpawnWeapons32to47(int targetMask, int actorIndex)
{
    int bitIdx;
    int count;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_ThrowLoadResBarretWeapons();
    g_BattleThrowBarretTargetMask = targetMask;
    bitIdx = 0;
    for (count = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            count++;
            if (count == 1) {
                g_BattleThrowBarretTargetIndex = bitIdx;
            }
        }
    }
    g_BattleThrowBarretActorIndex = actorIndex;
    Battle_AllocateResourceSlot(BattleFx_ThrowBarretWeaponsDispatchKindTask);
}

/*
 * Resolves target bone coordinates and selects the straight or tumbling projectile timeline for Barret's thrown weapon.
 */
// FUNCTION: FF7 0x004d2bc7
void __cdecl BattleFx_ThrowBarretWeaponsDispatchKindTask(void)
{
    struct {
        PsxMatrix matrix;
        int slot;
        short rotation[4];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.slot = (int)BATTLE_CURRENT_TASK_SLOT;
    *(short *)local.slot = 0xFFFF;
    Battle_GetBoneWorldPos(
        g_BattleThrowBarretTargetIndex,
        g_BattleActorRecords[g_BattleThrowBarretTargetIndex].targetBone,
        &g_BattleThrowBarretTargetPos.vx);
    Battle_GetBoneWorldPos(
        g_BattleThrowBarretActorIndex,
        g_BattleActorRecords[g_BattleThrowBarretActorIndex].targetBone,
        &g_BattleThrowBarretSourcePos.x);
    g_BattleThrowBarretSourcePos.yaw =
        g_BattleActorRecords[g_BattleThrowBarretActorIndex].yaw;
    local.rotation[0] = 0;
    local.rotation[1] = g_BattleThrowBarretSourcePos.yaw;
    local.rotation[2] = 0;
    RotMatrixXYZ((void *)local.rotation, &local.matrix);
    TransformVectorToInt32((PsxMatrix *)&local.matrix, (int *)&g_BattleThrowBarretVectorOffset, &g_BattleThrowBarretTransformedVector);
    switch (g_BattleThrowBarretWeaponFlag) {
    case BATTLE_BARRET_THROW_STRAIGHT:
        local.slot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowBarretWeaponsStraightTimelineTask);
        break;
    case BATTLE_BARRET_THROW_TUMBLING:
        local.slot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowBarretWeaponsTumblingTimelineTask);
        break;
    }
    *(short *)((char *)local.slot + 0x1C) = 0;
}

/*
 * Timeline task that launches and tracks a straight-line thrown Barret weapon.
 */
// FUNCTION: FF7 0x004d2cea
void __cdecl BattleFx_ThrowBarretWeaponsStraightTimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBarretThrowStraight *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowBarretDeltaVectorX = g_BattleThrowBarretTargetPos.vx - g_BattleThrowBarretSourcePos.x;
        g_BattleThrowBarretDeltaVectorY = g_BattleThrowBarretTargetPos.vy - g_BattleThrowBarretSourcePos.y;
        g_BattleThrowBarretDeltaVectorZ = g_BattleThrowBarretTargetPos.vz - g_BattleThrowBarretSourcePos.z;
        norm = NormalizeVectorFixed(&g_BattleThrowBarretDeltaVectorX, &g_BattleThrowBarretDeltaVectorX);
        speed = SqrtToInt(norm) / 0xC;
        pProjectile = (BattleFxBarretThrowStraight *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowBarretWeaponsStraightProjectileTask);
        *(Int64Pair *)((char *)pProjectile + 4) = *(Int64Pair *)&g_BattleThrowBarretSourcePos;
        pProjectile->offsetX = speed * g_BattleThrowBarretDeltaVectorX >> 0xC;
        pProjectile->offsetY = speed * g_BattleThrowBarretDeltaVectorY >> 0xC;
        pProjectile->offsetZ = speed * g_BattleThrowBarretDeltaVectorZ >> 0xC;
        pProjectile->angleVelocity = 0xAA;
        pProjectile->angle = 0xAA;
        g_BattleThrowBarretTargetAngles.vx = pProjectile->baseX - pProjectile->offsetX;
        g_BattleThrowBarretTargetAngles.vy = pProjectile->baseY - pProjectile->offsetY;
        g_BattleThrowBarretTargetAngles.vz = pProjectile->baseZ - pProjectile->offsetZ;
    }
    if (task->frameCounter == 0xD) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowBarretTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowBarretTargetPos.vx), 0x1E4);
    }
    if (task->frameCounter >= 0x1E) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Orients and draws a straight-flying thrown weapon model along its flight vector.
 */
// FUNCTION: FF7 0x004d2eb4
void __cdecl BattleFx_ThrowBarretWeaponsStraightProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        int rotMatrix;
        struct { unsigned char pad_to_12[0x12]; int drawX; };
        struct { unsigned char pad_to_16[0x16]; int drawY; };
        struct { unsigned char pad_to_1A[0x1A]; int drawZ; };
        struct { unsigned char pad_to_20[0x20]; BattleFxBarretStraightProjectile *projectile; };
        struct { unsigned char pad_to_24[0x24]; VECTOR tmpX; };
        struct { unsigned char pad_to_34[0x34]; int heightOffset; };
        struct { unsigned char pad_to_38[0x38]; int stepCount; };
        struct { unsigned char pad_to_3C[0x3C]; int angle; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_68[0x68]; VECTOR axis; };
        struct { unsigned char pad_to_78[0x78]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxBarretStraightProjectile *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.projectile->frameCount < 0xC) {
        frame.angle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleThrowBarretTransformedVector, (VECTOR *)&g_BattleThrowBarretDeltaVectorX, &frame.axis);
        frame.stepCount = frame.angle / 0xBE + 1;
        frame.heightOffset = (SinFixed(frame.projectile->angle) * -0x190 >> 0xC) * frame.stepCount;
    } else {
        frame.heightOffset = frame.projectile->angle;
    }
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.drawX = frame.projectile->posX;
    frame.drawY = frame.projectile->posY + frame.heightOffset;
    frame.drawZ = frame.projectile->posZ;
    frame.tmpX.vx = frame.drawX - g_BattleThrowBarretTargetAngles.vx;
    frame.tmpX.vy = frame.drawY - g_BattleThrowBarretTargetAngles.vy;
    frame.tmpX.vz = frame.drawZ - g_BattleThrowBarretTargetAngles.vz;
    NormalizeVectorFixed(&frame.tmpX, &frame.tmpX);
    frame.angle = ComputeAngleBetweenVectors((VECTOR *)g_BattleThrowBarretVectorOffset, &frame.tmpX, &frame.axis);
    Gte_BuildAxisAngleMatrix(frame.angle, &frame.axisMatrix, &frame.axis);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
    if (g_BattleIsPaused == 0) {
        g_BattleThrowBarretTargetAngles.vx = *(short *)&frame.drawX;
        g_BattleThrowBarretTargetAngles.vy = *(short *)&frame.drawY;
        g_BattleThrowBarretTargetAngles.vz = *(short *)&frame.drawZ;
    }
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.tmpX.vx = 0;
    frame.tmpX.vy = 0;
    frame.tmpX.vz = 0x50;
    TransformVectorToInt32((PsxMatrix *)&frame.rotMatrix, &frame.tmpX, &frame.tmpX);
    frame.drawX += frame.tmpX.vx;
    frame.drawY += frame.tmpX.vy;
    frame.drawZ += frame.tmpX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowBarretWeaponIndex, g_BattleThrowBarretLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.projectile->frameCount >= 0x18) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->fadeLevel = (frame.projectile->frameCount - 0x18) * 0x2AA;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.projectile->frameCount >= 0x1E) {
        frame.projectile->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.projectile->posX += frame.projectile->velocityX;
    frame.projectile->posY += frame.projectile->velocityY;
    frame.projectile->posZ += frame.projectile->velocityZ;
    frame.projectile->angle += frame.projectile->angleVelocity;
    if (frame.projectile->frameCount == 0xC) {
        frame.projectile->angle = 0;
        frame.projectile->angleVelocity = -(rand() % 0x1E + 0x3C);
    }
    (frame.projectile->frameCount)++;
}

/*
 * Timeline task that launches and tracks a tumbling thrown Barret weapon.
 */
// FUNCTION: FF7 0x004d31fc
void __cdecl BattleFx_ThrowBarretWeaponsTumblingTimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBarretThrowTumble *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowBarretDeltaVectorX = g_BattleThrowBarretTargetPos.vx - g_BattleThrowBarretSourcePos.x;
        g_BattleThrowBarretDeltaVectorY = g_BattleThrowBarretTargetPos.vy - g_BattleThrowBarretSourcePos.y;
        g_BattleThrowBarretDeltaVectorZ = g_BattleThrowBarretTargetPos.vz - g_BattleThrowBarretSourcePos.z;
        norm = NormalizeVectorFixed(&g_BattleThrowBarretDeltaVectorX, &g_BattleThrowBarretDeltaVectorX);
        speed = SqrtToInt(norm) / 0x12;
        pProjectile = (BattleFxBarretThrowTumble *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowBarretWeaponsTumblingProjectileTask);
        *(Int64Pair *)((char *)pProjectile + 4) = *(Int64Pair *)&g_BattleThrowBarretSourcePos;
        pProjectile->offsetX = speed * g_BattleThrowBarretDeltaVectorX >> 0xC;
        pProjectile->offsetY = speed * g_BattleThrowBarretDeltaVectorY >> 0xC;
        pProjectile->offsetZ = speed * g_BattleThrowBarretDeltaVectorZ >> 0xC;
        pProjectile->angleRate = 0x71;
        pProjectile->angle = 0x71;
        pProjectile->lifetime = rand() % 0xC8 + 0xB4;
        pProjectile->spinAngleX = pProjectile->lifetime;
        pProjectile->tumbleRate = rand() % 0x96 + 0xA0;
        pProjectile->spinAngleZ = pProjectile->tumbleRate;
    }
    if (task->frameCounter == 0x12) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowBarretTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowBarretTargetPos.vx), 0x1E4);
    }
    if (task->frameCounter >= 0x1E) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Spins, translates, and draws a tumbling thrown weapon model between caster and target.
 */
// FUNCTION: FF7 0x004d33d1
void __cdecl BattleFx_ThrowBarretWeaponsTumblingProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxBarretWeaponTumble *projectile; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int heightOffset; };
        struct { unsigned char pad_to_38[0x38]; int stepCount; };
        struct { unsigned char pad_to_3C[0x3C]; int angle; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_68[0x68]; VECTOR axis; };
        struct { unsigned char pad_to_78[0x78]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxBarretWeaponTumble *)BATTLE_CURRENT_TASK_SLOT;
    frame.angle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleThrowBarretTransformedVector, (VECTOR *)&g_BattleThrowBarretDeltaVectorX, &frame.axis);
    frame.stepCount = frame.angle / 0xBE + 1;
    frame.heightOffset = (SinFixed(frame.projectile->angle) * -0x258 >> 0xC) * frame.stepCount;
    frame.rotX.vx = frame.projectile->angleX;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.projectile->angleZ;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.projectile->posX;
    frame.rotMatrix.t[1] = frame.projectile->posY + frame.heightOffset;
    frame.rotMatrix.t[2] = frame.projectile->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = 0x50;
    TransformVectorToInt32(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
    frame.rotMatrix.t[0] += frame.offsetX.vx;
    frame.rotMatrix.t[1] += frame.offsetX.vy;
    frame.rotMatrix.t[2] += frame.offsetX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowBarretWeaponIndex, g_BattleThrowBarretLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.projectile->timer >= 0x18) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowBarretWeaponIndex + 0xF, g_BattleThrowBarretLoadedResTable);
        frame.modelDraw->fadeLevel = (frame.projectile->timer - 0x18) * 0x2AA;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.projectile->timer >= 0x1E) {
        frame.projectile->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.projectile->posX += frame.projectile->velocityX;
    frame.projectile->posY += frame.projectile->velocityY;
    frame.projectile->posZ += frame.projectile->velocityZ;
    frame.projectile->angle += frame.projectile->angleRate;
    if (frame.projectile->timer == 0x12) {
        frame.projectile->angle = 0;
    }
    frame.projectile->angleX += frame.projectile->angleXRate;
    frame.projectile->angleZ += frame.projectile->angleZRate;
    (frame.projectile->timer)++;
}
} /* extern "C" */

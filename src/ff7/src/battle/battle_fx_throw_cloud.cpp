// src/ff7/src/battle/battle_fx_throw_cloud.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Throw effects for Cloud's sword weapons: per-weapon entry points, a dispatch step, and
//   the flight timeline and spinning sword model tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "lib/api/sound.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x0082ea20
unsigned int g_BattleThrowCloudResourceDef[150] = {0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon01, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon02, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon03, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon04, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon05, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon06, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon07, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon08, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon09, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon10, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon11, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon12, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon13, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon14, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCloudResNameWeapon15_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon01_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon02_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon03_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon04_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon05_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon06_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon07_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon08_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon09_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon10_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon11_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon12_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon13_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon14_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCloudResNameWeapon15, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0082ec78
unsigned char g_BattleThrowCloudVectorOffset[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff};
// GLOBAL: FF7 0x0082ec88
unsigned char g_BattleThrowCloudResNameWeapon01[14] = "h_sword01.rsd";
// GLOBAL: FF7 0x0082ec98
unsigned char g_BattleThrowCloudResNameWeapon02[14] = "h_sword02.rsd";
// GLOBAL: FF7 0x0082eca8
unsigned char g_BattleThrowCloudResNameWeapon03[14] = "h_sword03.rsd";
// GLOBAL: FF7 0x0082ecb8
unsigned char g_BattleThrowCloudResNameWeapon04[14] = "h_sword04.rsd";
// GLOBAL: FF7 0x0082ecc8
unsigned char g_BattleThrowCloudResNameWeapon05[14] = "h_sword05.rsd";
// GLOBAL: FF7 0x0082ecd8
unsigned char g_BattleThrowCloudResNameWeapon06[14] = "h_sword06.rsd";
// GLOBAL: FF7 0x0082ece8
unsigned char g_BattleThrowCloudResNameWeapon07[14] = "h_sword07.rsd";
// GLOBAL: FF7 0x0082ecf8
unsigned char g_BattleThrowCloudResNameWeapon08[14] = "h_sword08.rsd";
// GLOBAL: FF7 0x0082ed08
unsigned char g_BattleThrowCloudResNameWeapon09[14] = "h_sword09.rsd";
// GLOBAL: FF7 0x0082ed18
unsigned char g_BattleThrowCloudResNameWeapon10[14] = "h_sword10.rsd";
// GLOBAL: FF7 0x0082ed28
unsigned char g_BattleThrowCloudResNameWeapon11[14] = "h_sword11.rsd";
// GLOBAL: FF7 0x0082ed38
unsigned char g_BattleThrowCloudResNameWeapon12[14] = "h_sword12.rsd";
// GLOBAL: FF7 0x0082ed48
unsigned char g_BattleThrowCloudResNameWeapon13[14] = "h_sword13.rsd";
// GLOBAL: FF7 0x0082ed58
unsigned char g_BattleThrowCloudResNameWeapon14[14] = "h_sword14.rsd";
// GLOBAL: FF7 0x0082ed68
unsigned char g_BattleThrowCloudResNameWeapon15_B[14] = "h_sword15.rsd";
// GLOBAL: FF7 0x0082ed78
unsigned char g_BattleThrowCloudResNameWeapon01_B[14] = "h_sword01.rsd";
// GLOBAL: FF7 0x0082ed88
unsigned char g_BattleThrowCloudResNameWeapon02_B[14] = "h_sword02.rsd";
// GLOBAL: FF7 0x0082ed98
unsigned char g_BattleThrowCloudResNameWeapon03_B[14] = "h_sword03.rsd";
// GLOBAL: FF7 0x0082eda8
unsigned char g_BattleThrowCloudResNameWeapon04_B[14] = "h_sword04.rsd";
// GLOBAL: FF7 0x0082edb8
unsigned char g_BattleThrowCloudResNameWeapon05_B[14] = "h_sword05.rsd";
// GLOBAL: FF7 0x0082edc8
unsigned char g_BattleThrowCloudResNameWeapon06_B[14] = "h_sword06.rsd";
// GLOBAL: FF7 0x0082edd8
unsigned char g_BattleThrowCloudResNameWeapon07_B[14] = "h_sword07.rsd";
// GLOBAL: FF7 0x0082ede8
unsigned char g_BattleThrowCloudResNameWeapon08_B[14] = "h_sword08.rsd";
// GLOBAL: FF7 0x0082edf8
unsigned char g_BattleThrowCloudResNameWeapon09_B[14] = "h_sword09.rsd";
// GLOBAL: FF7 0x0082ee08
unsigned char g_BattleThrowCloudResNameWeapon10_B[14] = "h_sword10.rsd";
// GLOBAL: FF7 0x0082ee18
unsigned char g_BattleThrowCloudResNameWeapon11_B[14] = "h_sword11.rsd";
// GLOBAL: FF7 0x0082ee28
unsigned char g_BattleThrowCloudResNameWeapon12_B[14] = "h_sword12.rsd";
// GLOBAL: FF7 0x0082ee38
unsigned char g_BattleThrowCloudResNameWeapon13_B[14] = "h_sword13.rsd";
// GLOBAL: FF7 0x0082ee48
unsigned char g_BattleThrowCloudResNameWeapon14_B[14] = "h_sword14.rsd";
// GLOBAL: FF7 0x0082ee58
unsigned char g_BattleThrowCloudResNameWeapon15[14] = "h_sword15.rsd";
// clang-format on
typedef struct {
    short x;
    short y;
    short z;
    short yaw;
} BattleThrowCloudSourcePos;
typedef struct {
    short x;
    short y;
    short z;
} BattleThrowCloudTargetPos;
// clang-format off
// GLOBAL: FF7 0x00b37760
int g_BattleThrowCloudTargetIndex = 0;
unsigned char g_bssPad_00b37764[4] = {0};
// GLOBAL: FF7 0x00b37768
int g_BattleThrowCloudDeltaVectorX = 0;
// GLOBAL: FF7 0x00b3776c
int g_BattleThrowCloudDeltaVectorY = 0;
// GLOBAL: FF7 0x00b37770
int g_BattleThrowCloudDeltaVectorZ = 0;
unsigned char g_bssPad_00b37774[4] = {0};
// GLOBAL: FF7 0x00b37778
BattleThrowCloudSourcePos g_BattleThrowCloudSourcePos = {0};
// GLOBAL: FF7 0x00b37780
int g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
// GLOBAL: FF7 0x00b37784
int g_BattleThrowCloudWeaponIndex = 0;
// GLOBAL: FF7 0x00b37788
BattleThrowCloudTargetPos g_BattleThrowCloudTargetPos = {0};
// GLOBAL: FF7 0x00b37790
int g_BattleThrowCloudTargetMask = 0;
// GLOBAL: FF7 0x00b37794
int g_BattleThrowCloudActorIndex = 0;
// GLOBAL: FF7 0x00b37798
VECTOR g_BattleThrowCloudTransformedVector = {0};
// GLOBAL: FF7 0x00b377a8
struct GfxTableCursor *g_BattleThrowCloudLoadedResTable = 0;
unsigned char g_bssPad_00b377ac[4] = {0};
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

/* Loads Cloud sword models and throw effect resources. */
// FUNCTION: FF7 0x004d36a0
void __cdecl BattleFx_ThrowLoadResCloudWeapons(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest requestBuffer;
    int resultId;
    char pathSize[200];
    GfxResourceRequest *pReq;
    int defsPtr;

    if (g_pBattle3dScene != 0 && g_BattleThrowCloudLoadedResTable == 0) {
        resultId = 0x1E;
        defsPtr = (int)&g_BattleThrowCloudResourceDef;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestBuffer;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x0082ee68
        pReq->resourcePath = BattleFx_BuildResourcePath("item/wp_cu", pathSize);
        Gfx_LoadModelTableIfAbsent(resultId, defsPtr, pReq, &g_BattleThrowCloudLoadedResTable, sceneContext);
    }
}

/* Throws Cloud weapon 0 (Buster Sword). */
// FUNCTION: FF7 0x004d3730
void __cdecl Battle_ThrowFxWeaponBlock00(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 0;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 1 (Mythril Saber). */
// FUNCTION: FF7 0x004d3759
void __cdecl Battle_ThrowFxWeaponBlock00Index1(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 1;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 2 (Hardedge). */
// FUNCTION: FF7 0x004d3782
void __cdecl Battle_ThrowFxWeaponBlock00Index2(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 2;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 3 (Butterfly Edge). */
// FUNCTION: FF7 0x004d37ab
void __cdecl Battle_ThrowFxWeaponBlock00Index3(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 3;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 4 (Enhance Sword). */
// FUNCTION: FF7 0x004d37d4
void __cdecl Battle_ThrowFxWeaponBlock00Index4(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 4;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 5 (Organics). */
// FUNCTION: FF7 0x004d37fd
void __cdecl Battle_ThrowFxWeaponBlock00Index5(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 5;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 6 (Crystal Sword). */
// FUNCTION: FF7 0x004d3826
void __cdecl Battle_ThrowFxWeaponBlock00Index6(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 6;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 7 (Force Stealer). */
// FUNCTION: FF7 0x004d384f
void __cdecl Battle_ThrowFxWeaponBlock00Index7(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 7;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 8 (Rune Blade). */
// FUNCTION: FF7 0x004d3878
void __cdecl Battle_ThrowFxWeaponBlock00Index8(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 8;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 9 (Murasame). */
// FUNCTION: FF7 0x004d38a1
void __cdecl Battle_ThrowFxWeaponBlock00Index9(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 9;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 10 (Nail Bat). */
// FUNCTION: FF7 0x004d38ca
void __cdecl Battle_ThrowFxWeaponBlock00Index10(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 10;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 11 (Yoshiyuki). */
// FUNCTION: FF7 0x004d38f3
void __cdecl Battle_ThrowFxWeaponBlock00Index11(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 11;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 12 (Apocalypse). */
// FUNCTION: FF7 0x004d391c
void __cdecl Battle_ThrowFxWeaponBlock00Index12(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 12;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 13 (Heaven's Cloud). */
// FUNCTION: FF7 0x004d3945
void __cdecl Battle_ThrowFxWeaponBlock00Index13(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 13;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Throws Cloud weapon 14 (Ragnarok) or 15 (Ultima Weapon). */
// FUNCTION: FF7 0x004d396e
void __cdecl Battle_ThrowFxWeaponBlock16(int targetMask, int actorIdx)
{
    g_BattleThrowCloudWeaponIndex = 14;
    g_BattleThrowCloudWeaponFlag = BATTLE_CLOUD_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons00to61(targetMask, actorIdx);
}

/* Schedules draw setup and timeline dispatch task for thrown Cloud sword. */
// FUNCTION: FF7 0x004d3997
void __cdecl Battle_ThrowFxSpawnWeapons00to61(int targetMask, int actorIdx)
{
    int bitIdx;
    int targetCount;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_ThrowLoadResCloudWeapons();
    g_BattleThrowCloudTargetMask = targetMask;
    bitIdx = 0;
    for (targetCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                g_BattleThrowCloudTargetIndex = bitIdx;
            }
        }
    }
    g_BattleThrowCloudActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_ThrowCloudWeaponsDispatchKindTask);
}

/* Sets up initial direction vectors and schedules sword timeline task. */
// FUNCTION: FF7 0x004d3a37
void __cdecl BattleFx_ThrowCloudWeaponsDispatchKindTask(void)
{
    int rotMatrix[8];
    short yawAngles[3];
    BattleFxYuffieWeaponTimeline2 *taskPtr;

    taskPtr = (BattleFxYuffieWeaponTimeline2 *)BATTLE_CURRENT_TASK_SLOT;
    taskPtr->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleThrowCloudTargetIndex, g_BattleActorRecords[g_BattleThrowCloudTargetIndex].targetBone, &g_BattleThrowCloudTargetPos.x);
    Battle_GetBoneWorldPos(g_BattleThrowCloudActorIndex, g_BattleActorRecords[g_BattleThrowCloudActorIndex].targetBone, &g_BattleThrowCloudSourcePos.x);
    g_BattleThrowCloudSourcePos.yaw = g_BattleActorRecords[g_BattleThrowCloudActorIndex].yaw;
    yawAngles[0] = 0;
    yawAngles[1] = g_BattleThrowCloudSourcePos.yaw;
    yawAngles[2] = 0;
    RotMatrixXYZ((void *)yawAngles, (void *)rotMatrix);
    TransformVectorToInt32((PsxMatrix *)rotMatrix, (int *)&g_BattleThrowCloudVectorOffset, &g_BattleThrowCloudTransformedVector);
    switch (g_BattleThrowCloudWeaponFlag) {
    case BATTLE_CLOUD_THROW_STRAIGHT:
        taskPtr = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCloudWeaponsStraightTimelineTask);
        break;
    }
    taskPtr->subCounter = 0;
}

/* Drives flight velocity and triggers impact sfx on sword arrival. */
// FUNCTION: FF7 0x004d3b36
void __cdecl BattleFx_ThrowCloudWeaponsStraightTimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxAerisThrowDelta *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowCloudDeltaVectorX = g_BattleThrowCloudTargetPos.x - g_BattleThrowCloudSourcePos.x;
        g_BattleThrowCloudDeltaVectorY = g_BattleThrowCloudTargetPos.y - g_BattleThrowCloudSourcePos.y;
        g_BattleThrowCloudDeltaVectorZ = g_BattleThrowCloudTargetPos.z - g_BattleThrowCloudSourcePos.z;
        norm = NormalizeVectorFixed(&g_BattleThrowCloudDeltaVectorX, &g_BattleThrowCloudDeltaVectorX);
        speed = SqrtToInt(norm) / 4;
        pProjectile = (BattleFxAerisThrowDelta *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCloudWeaponsStraightProjectileTask);
        *(Int64Pair *)((char *)pProjectile + 4) = *(Int64Pair *)&g_BattleThrowCloudSourcePos;
        pProjectile->deltaX = speed * g_BattleThrowCloudDeltaVectorX >> 12;
        pProjectile->deltaY = speed * g_BattleThrowCloudDeltaVectorY >> 12;
        pProjectile->deltaZ = speed * g_BattleThrowCloudDeltaVectorZ >> 12;
    }
    if (task->frameCounter == 4) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowCloudTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowCloudTargetPos.x), 0x12);
    }
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates rotation and renders 3D mesh for thrown Cloud sword. */
// FUNCTION: FF7 0x004d3ca8
void __cdecl BattleFx_ThrowCloudWeaponsStraightProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxSlowParticle *projectile; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int angle; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR rotX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axis; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxSlowParticle *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.projectile->posX;
    frame.rotMatrix.t[1] = frame.projectile->posY;
    frame.rotMatrix.t[2] = frame.projectile->posZ;
    frame.angle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleThrowCloudVectorOffset, (VECTOR *)&g_BattleThrowCloudDeltaVectorX, &frame.axis);
    Gte_BuildAxisAngleMatrix(frame.angle, &frame.axisMatrix, &frame.axis);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = 380;
    TransformVectorToInt32(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
    frame.rotMatrix.t[0] += frame.offsetX.vx;
    frame.rotMatrix.t[1] += frame.offsetX.vy;
    frame.rotMatrix.t[2] += frame.offsetX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->flags = BATTLE_FX_DRAW_FADE_TRANSPARENCY;
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCloudWeaponIndex, g_BattleThrowCloudLoadedResTable);
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.projectile->timer >= 22) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCloudWeaponIndex + 0xF, g_BattleThrowCloudLoadedResTable);
        frame.modelDraw->fadeLevel = frame.projectile->timer - 22 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.projectile->timer >= 30) {
        frame.projectile->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.projectile->timer < 4) {
        frame.projectile->posX += frame.projectile->velocityX;
        frame.projectile->posY += frame.projectile->velocityY;
        frame.projectile->posZ += frame.projectile->velocityZ;
    }
    (frame.projectile->timer)++;
}
} /* extern "C" */

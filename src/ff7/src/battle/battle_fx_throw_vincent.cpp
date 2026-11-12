// src/ff7/src/battle/battle_fx_throw_vincent.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Throw effects for Vincent's gun weapons: loads the models, then per-weapon entry points
//   start the flight timeline and thrown-model tasks.

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
#include "ff7/types/battle_fx.h"
#include "ff7/enums/battle.h"
extern "C" {

#pragma intrinsic(memset)
// clang-format off
// GLOBAL: FF7 0x0082d360
unsigned int g_BattleThrowVincentWeaponModelRows[130] = {0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun01, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun02, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun03, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun04, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun05, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun06, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun07, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun08, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun09, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun10, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun11, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun12, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowVincentResNameGun13_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun01_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun02_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun03_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun04_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun05_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun06_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun07_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun08_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun09_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun10_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun11_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun12_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowVincentResNameGun13, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0082d568
unsigned char g_BattleThrowVincentVectorOffset[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff};
// GLOBAL: FF7 0x0082d578
unsigned char g_BattleThrowVincentResNameGun01[14] = "vin_gun01.rsd";
// GLOBAL: FF7 0x0082d588
unsigned char g_BattleThrowVincentResNameGun02[14] = "vin_gun02.rsd";
// GLOBAL: FF7 0x0082d598
unsigned char g_BattleThrowVincentResNameGun03[14] = "vin_gun03.rsd";
// GLOBAL: FF7 0x0082d5a8
unsigned char g_BattleThrowVincentResNameGun04[14] = "vin_gun04.rsd";
// GLOBAL: FF7 0x0082d5b8
unsigned char g_BattleThrowVincentResNameGun05[14] = "vin_gun05.rsd";
// GLOBAL: FF7 0x0082d5c8
unsigned char g_BattleThrowVincentResNameGun06[14] = "vin_gun06.rsd";
// GLOBAL: FF7 0x0082d5d8
unsigned char g_BattleThrowVincentResNameGun07[14] = "vin_gun07.rsd";
// GLOBAL: FF7 0x0082d5e8
unsigned char g_BattleThrowVincentResNameGun08[14] = "vin_gun08.rsd";
// GLOBAL: FF7 0x0082d5f8
unsigned char g_BattleThrowVincentResNameGun09[14] = "vin_gun09.rsd";
// GLOBAL: FF7 0x0082d608
unsigned char g_BattleThrowVincentResNameGun10[14] = "vin_gun10.rsd";
// GLOBAL: FF7 0x0082d618
unsigned char g_BattleThrowVincentResNameGun11[14] = "vin_gun11.rsd";
// GLOBAL: FF7 0x0082d628
unsigned char g_BattleThrowVincentResNameGun12[14] = "vin_gun12.rsd";
// GLOBAL: FF7 0x0082d638
unsigned char g_BattleThrowVincentResNameGun13_B[14] = "vin_gun13.rsd";
// GLOBAL: FF7 0x0082d648
unsigned char g_BattleThrowVincentResNameGun01_B[14] = "vin_gun01.rsd";
// GLOBAL: FF7 0x0082d658
unsigned char g_BattleThrowVincentResNameGun02_B[14] = "vin_gun02.rsd";
// GLOBAL: FF7 0x0082d668
unsigned char g_BattleThrowVincentResNameGun03_B[14] = "vin_gun03.rsd";
// GLOBAL: FF7 0x0082d678
unsigned char g_BattleThrowVincentResNameGun04_B[14] = "vin_gun04.rsd";
// GLOBAL: FF7 0x0082d688
unsigned char g_BattleThrowVincentResNameGun05_B[14] = "vin_gun05.rsd";
// GLOBAL: FF7 0x0082d698
unsigned char g_BattleThrowVincentResNameGun06_B[14] = "vin_gun06.rsd";
// GLOBAL: FF7 0x0082d6a8
unsigned char g_BattleThrowVincentResNameGun07_B[14] = "vin_gun07.rsd";
// GLOBAL: FF7 0x0082d6b8
unsigned char g_BattleThrowVincentResNameGun08_B[14] = "vin_gun08.rsd";
// GLOBAL: FF7 0x0082d6c8
unsigned char g_BattleThrowVincentResNameGun09_B[14] = "vin_gun09.rsd";
// GLOBAL: FF7 0x0082d6d8
unsigned char g_BattleThrowVincentResNameGun10_B[14] = "vin_gun10.rsd";
// GLOBAL: FF7 0x0082d6e8
unsigned char g_BattleThrowVincentResNameGun11_B[14] = "vin_gun11.rsd";
// GLOBAL: FF7 0x0082d6f8
unsigned char g_BattleThrowVincentResNameGun12_B[14] = "vin_gun12.rsd";
// GLOBAL: FF7 0x0082d708
unsigned char g_BattleThrowVincentResNameGun13[14] = "vin_gun13.rsd";
// GLOBAL: FF7 0x00b371f0
int g_BattleThrowVincentTargetIndex = 0;
unsigned char g_bssPad_00b371f4[4] = {0};
// GLOBAL: FF7 0x00b371f8
int g_BattleThrowVincentDeltaVectorX = 0;
// GLOBAL: FF7 0x00b371fc
int g_BattleThrowVincentDeltaVectorY = 0;
// GLOBAL: FF7 0x00b37200
int g_BattleThrowVincentDeltaVectorZ = 0;
unsigned char g_bssPad_00b37204[4] = {0};
typedef union {
    Int64Pair raw;
    struct {
        short x;
        short y;
        short z;
        short yaw;
    };
} BattleThrowVincentPose;
// GLOBAL: FF7 0x00b37208
BattleThrowVincentPose g_BattleThrowVincentSourcePos = {0};
// GLOBAL: FF7 0x00b37210
int g_BattleThrowVincentWeaponFlag = 0;
// GLOBAL: FF7 0x00b37214
int g_BattleThrowVincentWeaponIndex = 0;
typedef struct {
    short x;
    short y;
    short z;
} BattleThrowVincentPoint;
// GLOBAL: FF7 0x00b37218
BattleThrowVincentPoint g_BattleThrowVincentTargetPos = {0};
// GLOBAL: FF7 0x00b37220
int g_BattleThrowVincentTargetMask = 0;
// GLOBAL: FF7 0x00b37224
int g_BattleThrowVincentActorIndex = 0;
// GLOBAL: FF7 0x00b37228
VECTOR g_BattleThrowVincentTransformedVector = {0};
// GLOBAL: FF7 0x00b37238
struct GfxTableCursor *g_BattleThrowVincentLoadedResTable = 0;
unsigned char g_bssPad_00b3723c[4] = {0};
// clang-format on
#pragma function(memcpy)
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

/* Loads Vincent gun models and throw effect resources. */
// FUNCTION: FF7 0x004ccf30
void __cdecl BattleFx_ThrowLoadResVincentWeapons(void)
{
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest requestBuffer;
    GfxModelTableBank *loadParam;
    GfxResourceRequest *request;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleThrowVincentLoadedResTable == 0) {
        typeId = 0x1A;
        def = (int)&g_BattleThrowVincentWeaponModelRows;
        loadParam = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082d718
        request->resourcePath = BattleFx_BuildResourcePath("item/wp_bi", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, def, request, &g_BattleThrowVincentLoadedResTable, loadParam);
    }
}

/* Throws Vincent weapon 114 (Quicksilver). */
// FUNCTION: FF7 0x004ccfc0
void __cdecl Battle_ThrowFxWeaponBlock114(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 0;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 115 (Shotgun). */
// FUNCTION: FF7 0x004ccfe9
void __cdecl Battle_ThrowFxWeaponBlock114Index1(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 1;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 116 (Shortbarrel). */
// FUNCTION: FF7 0x004cd012
void __cdecl Battle_ThrowFxWeaponBlock114Index2(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 2;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 117 (Lariat). */
// FUNCTION: FF7 0x004cd03b
void __cdecl Battle_ThrowFxWeaponBlock114Index3(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 3;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 118 (Winchester). */
// FUNCTION: FF7 0x004cd064
void __cdecl Battle_ThrowFxWeaponBlock114Index4(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 4;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 119 (Peacemaker). */
// FUNCTION: FF7 0x004cd08d
void __cdecl Battle_ThrowFxWeaponBlock114Index5(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 5;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 120 (Buntline). */
// FUNCTION: FF7 0x004cd0b6
void __cdecl Battle_ThrowFxWeaponBlock114Index6(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 6;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 121 (Long Barrel R). */
// FUNCTION: FF7 0x004cd0df
void __cdecl Battle_ThrowFxWeaponBlock114Index7(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 7;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 122 (Silver Rifle). */
// FUNCTION: FF7 0x004cd108
void __cdecl Battle_ThrowFxWeaponBlock114Index8(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 8;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 123 (Sniper CR). */
// FUNCTION: FF7 0x004cd131
void __cdecl Battle_ThrowFxWeaponBlock114Index9(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 9;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 124 (Supershot ST). */
// FUNCTION: FF7 0x004cd15a
void __cdecl Battle_ThrowFxWeaponBlock114Index10(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 10;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 125 (Outsider). */
// FUNCTION: FF7 0x004cd183
void __cdecl Battle_ThrowFxWeaponBlock114Index11(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 11;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Throws Vincent weapon 126 (Death Penalty). */
// FUNCTION: FF7 0x004cd1ac
void __cdecl Battle_ThrowFxWeaponBlock114Index12(int targetMask, int actorIdx)
{
    g_BattleThrowVincentWeaponIndex = 12;
    g_BattleThrowVincentWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons114to126(targetMask, actorIdx);
}

/* Schedules draw setup and timeline task for thrown Vincent gun weapon. */
// FUNCTION: FF7 0x004cd1d5
void __cdecl Battle_ThrowFxSpawnWeapons114to126(int targetMask, int actorIdx)
{
    int bitIdx;
    int targetCount;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_ThrowLoadResVincentWeapons();
    g_BattleThrowVincentTargetMask = targetMask;
    bitIdx = 0;
    for (targetCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                g_BattleThrowVincentTargetIndex = bitIdx;
            }
        }
    }
    g_BattleThrowVincentActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_ThrowVincentWeaponsSpawnTimeline);
}

/* Computes source and target bone positions and launches weapon trajectory. */
// FUNCTION: FF7 0x004cd275
void __cdecl BattleFx_ThrowVincentWeaponsSpawnTimeline(void)
{
    int rotMatrix[8];
    short yawAngles[3];
    BattleFxYuffieWeaponTimeline2 *taskPtr;

    taskPtr = (BattleFxYuffieWeaponTimeline2 *)BATTLE_CURRENT_TASK_SLOT;
    taskPtr->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleThrowVincentTargetIndex, g_BattleActorRecords[g_BattleThrowVincentTargetIndex].targetBone, &g_BattleThrowVincentTargetPos.x);
    Battle_GetBoneWorldPos(g_BattleThrowVincentActorIndex, g_BattleActorRecords[g_BattleThrowVincentActorIndex].targetBone, &g_BattleThrowVincentSourcePos.x);
    g_BattleThrowVincentSourcePos.yaw = g_BattleActorRecords[g_BattleThrowVincentActorIndex].yaw;
    yawAngles[0] = 0;
    yawAngles[1] = g_BattleThrowVincentSourcePos.yaw;
    yawAngles[2] = 0;
    RotMatrixXYZ(yawAngles, rotMatrix);
    TransformVectorToInt32((PsxMatrix *)rotMatrix, (int *)&g_BattleThrowVincentVectorOffset, &g_BattleThrowVincentTransformedVector);
    switch (g_BattleThrowVincentWeaponFlag) {
    case BATTLE_VINCENT_THROW_STRAIGHT:
        taskPtr = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowVincentWeaponsTimelineTask);
        break;
    }
    taskPtr->subCounter = 0;
}

/* Drives flight velocity and triggers impact sfx on weapon arrival. */
// FUNCTION: FF7 0x004cd374
void __cdecl BattleFx_ThrowVincentWeaponsTimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxVincentThrownWeapon *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowVincentDeltaVectorX = g_BattleThrowVincentTargetPos.x - g_BattleThrowVincentSourcePos.x;
        g_BattleThrowVincentDeltaVectorY = g_BattleThrowVincentTargetPos.y - g_BattleThrowVincentSourcePos.y;
        g_BattleThrowVincentDeltaVectorZ = g_BattleThrowVincentTargetPos.z - g_BattleThrowVincentSourcePos.z;
        norm = NormalizeVectorFixed(&g_BattleThrowVincentDeltaVectorX, &g_BattleThrowVincentDeltaVectorX);
        speed = SqrtToInt(norm) / 16;
        pProjectile = (BattleFxVincentThrownWeapon *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowVincentWeaponsThrownModelTask);
        pProjectile->position = g_BattleThrowVincentSourcePos.raw;
        pProjectile->offsetX = speed * g_BattleThrowVincentDeltaVectorX >> 12;
        pProjectile->offsetY = speed * g_BattleThrowVincentDeltaVectorY >> 12;
        pProjectile->offsetZ = speed * g_BattleThrowVincentDeltaVectorZ >> 12;
        pProjectile->spinRate = 0x80;
        pProjectile->angle = 0x80;
        pProjectile->angleXStep = rand() % 200 + 180;
        pProjectile->angleX = pProjectile->angleXStep;
        pProjectile->angleZStep = rand() % 150 + 160;
        pProjectile->angleZ = pProjectile->angleZStep;
    }
    if (task->frameCounter == 17) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowVincentTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowVincentTargetPos.x), 0x112);
    }
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates rotation and renders 3D mesh for thrown Vincent gun. */
// FUNCTION: FF7 0x004cd54a
void __cdecl BattleFx_ThrowVincentWeaponsThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxVincentThrowRecord *throwRecord; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int bobOffset; };
        struct { unsigned char pad_to_38[0x38]; int stepCount; };
        struct { unsigned char pad_to_3C[0x3C]; int angleBetween; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_68[0x68]; VECTOR axisVector; };
        struct { unsigned char pad_to_78[0x78]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.throwRecord = (BattleFxVincentThrowRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleBetween = ComputeAngleBetweenVectors((VECTOR *)&g_BattleThrowVincentTransformedVector, (VECTOR *)&g_BattleThrowVincentDeltaVectorX, &frame.axisVector);
    frame.stepCount = frame.angleBetween / 190 + 1;
    frame.bobOffset = (SinFixed(frame.throwRecord->angle) * -0x1F4 >> 12) * frame.stepCount;
    frame.rotX.vx = frame.throwRecord->angleX;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.throwRecord->angleZ;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.throwRecord->posX;
    frame.rotMatrix.t[1] = frame.throwRecord->posY + frame.bobOffset;
    frame.rotMatrix.t[2] = frame.throwRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = 0;
    TransformVectorToInt32(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
    frame.rotMatrix.t[0] += frame.offsetX.vx;
    frame.rotMatrix.t[1] += frame.offsetX.vy;
    frame.rotMatrix.t[2] += frame.offsetX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowVincentWeaponIndex, g_BattleThrowVincentLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.throwRecord->timer >= 22) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowVincentWeaponIndex + 0xD, g_BattleThrowVincentLoadedResTable);
        frame.modelDraw->fadeLevel = frame.throwRecord->timer - 22 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.throwRecord->timer >= 30) {
        frame.throwRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.throwRecord->posX += frame.throwRecord->velocityX;
    frame.throwRecord->posY += frame.throwRecord->velocityY;
    frame.throwRecord->posZ += frame.throwRecord->velocityZ;
    frame.throwRecord->angle += frame.throwRecord->spinRate;
    if (frame.throwRecord->timer == 16) {
        frame.throwRecord->angle = 0;
    }
    frame.throwRecord->angleX += frame.throwRecord->angleXStep;
    frame.throwRecord->angleZ += frame.throwRecord->angleZStep;
    (frame.throwRecord->timer)++;
}
} /* extern "C" */

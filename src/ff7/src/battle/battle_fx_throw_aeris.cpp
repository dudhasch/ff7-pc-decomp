// src/ff7/src/battle/battle_fx_throw_aeris.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Throw effects for Aeris's rods and staves: per-weapon entry points that choose a
//   straight or high-arc flight, with timeline and projectile tasks.

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
#pragma function(memcpy)
typedef struct {
    short x;
    short y;
    short z;
    short yaw;
} ThrowAerisPosition;
typedef struct {
    short x;
    short y;
    short z;
} ThrowAerisTarget;

// clang-format off
// GLOBAL: FF7 0x0082e2e8
unsigned int g_BattleThrowAerisResourceDef[110] = {0x001900b6, (unsigned int)&g_BattleThrowAerisResNameRod_01_02, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)(g_BattleThrowAerisResNameRod_01_02 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowAerisResNameRod_03_04, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)(g_BattleThrowAerisResNameRod_03_04 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowAerisResNameRod_05_06, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)(g_BattleThrowAerisResNameRod_05_06 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowAerisResNameRod_07_08, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)(g_BattleThrowAerisResNameRod_07_08 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowAerisResNameRod_09_10, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)(g_BattleThrowAerisResNameRod_09_10 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowAerisResNameRod_11_01, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleThrowAerisResNameRod_11_01 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowAerisResNameRod_02_03, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleThrowAerisResNameRod_02_03 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowAerisResNameRod_04_05, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleThrowAerisResNameRod_04_05 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowAerisResNameRod_06_07, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleThrowAerisResNameRod_06_07 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowAerisResNameRod_08_09, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleThrowAerisResNameRod_08_09 + 12), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowAerisResNameRod_10_11, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)(g_BattleThrowAerisResNameRod_10_11 + 12), 0x00000000, 0x00000000, 0x00000000};
// The vector (0, 0, -4096) the TU transforms and measures angles against, plus 4 bytes of padding.
// Its zero head cannot be a definition of its own (an all-zero definition goes to .bss).
// GLOBAL: FF7 0x0082e4a0
struct VECTOR g_BattleThrowAerisVectorOffset = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0082e4b0
unsigned char g_BattleThrowAerisResNameRod_01_02[22] =
    "rod01.rsd\0\0\0"
    "rod02.rsd";
// GLOBAL: FF7 0x0082e4c8
unsigned char g_BattleThrowAerisResNameRod_03_04[22] =
    "rod03.rsd\0\0\0"
    "rod04.rsd";
// GLOBAL: FF7 0x0082e4e0
unsigned char g_BattleThrowAerisResNameRod_05_06[22] =
    "rod05.rsd\0\0\0"
    "rod06.rsd";
// GLOBAL: FF7 0x0082e4f8
unsigned char g_BattleThrowAerisResNameRod_07_08[22] =
    "rod07.rsd\0\0\0"
    "rod08.rsd";
// GLOBAL: FF7 0x0082e510
unsigned char g_BattleThrowAerisResNameRod_09_10[22] =
    "rod09.rsd\0\0\0"
    "rod10.rsd";
// GLOBAL: FF7 0x0082e528
unsigned char g_BattleThrowAerisResNameRod_11_01[22] =
    "rod11.rsd\0\0\0"
    "rod01.rsd";
// GLOBAL: FF7 0x0082e540
unsigned char g_BattleThrowAerisResNameRod_02_03[22] =
    "rod02.rsd\0\0\0"
    "rod03.rsd";
// GLOBAL: FF7 0x0082e558
unsigned char g_BattleThrowAerisResNameRod_04_05[22] =
    "rod04.rsd\0\0\0"
    "rod05.rsd";
// GLOBAL: FF7 0x0082e570
unsigned char g_BattleThrowAerisResNameRod_06_07[22] =
    "rod06.rsd\0\0\0"
    "rod07.rsd";
// GLOBAL: FF7 0x0082e588
unsigned char g_BattleThrowAerisResNameRod_08_09[22] =
    "rod08.rsd\0\0\0"
    "rod09.rsd";
// GLOBAL: FF7 0x0082e5a0
unsigned char g_BattleThrowAerisResNameRod_10_11[22] =
    "rod10.rsd\0\0\0"
    "rod11.rsd";
// GLOBAL: FF7 0x00b376b0
int g_BattleThrowAerisTargetIndex = 0;
unsigned char g_bssPad_00b376b4[4] = {0};
// GLOBAL: FF7 0x00b376b8
int g_BattleThrowAerisDeltaVectorX = 0;
// GLOBAL: FF7 0x00b376bc
int g_BattleThrowAerisDeltaVectorY = 0;
// GLOBAL: FF7 0x00b376c0
int g_BattleThrowAerisDeltaVectorZ = 0;
unsigned char g_bssPad_00b376c4[4] = {0};
// GLOBAL: FF7 0x00b376c8
ThrowAerisPosition g_BattleThrowAerisSourcePos = {0};
// GLOBAL: FF7 0x00b376d0
int g_BattleThrowAerisWeaponFlag = 0;
// GLOBAL: FF7 0x00b376d4
int g_BattleThrowAerisWeaponIndex = 0;
// GLOBAL: FF7 0x00b376d8
ThrowAerisTarget g_BattleThrowAerisTargetPos = {0};
// GLOBAL: FF7 0x00b376e0
int g_BattleThrowAerisTargetMask = 0;
// GLOBAL: FF7 0x00b376e4
int g_BattleThrowAerisActorIndex = 0;
// GLOBAL: FF7 0x00b376e8
VECTOR g_BattleThrowAerisTransformedVector = {0};
// GLOBAL: FF7 0x00b376f8
struct GfxTableCursor *g_BattleThrowAerisLoadedResTable = 0;
unsigned char g_bssPad_00b376fc[4] = {0};
// clang-format on
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

/* Loads Aeris rod/staff models and throw effect resources. */
// FUNCTION: FF7 0x004d1b90
void __cdecl BattleFx_ThrowLoadResAerisWeapons(void)
{
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest requestBuffer;
    GfxModelTableBank *loadParam;
    GfxResourceRequest *request;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleThrowAerisLoadedResTable == 0) {
        typeId = 0x16;
        def = (int)&g_BattleThrowAerisResourceDef;
        loadParam = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082e5b8
        request->resourcePath = BattleFx_BuildResourcePath("item/wp_ea", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, def, request, &g_BattleThrowAerisLoadedResTable, loadParam);
    }
}

/* Throws Aeris weapon 62 (Guard Stick). */
// FUNCTION: FF7 0x004d1c20
void __cdecl Battle_ThrowFxWeaponBlock62(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 0;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 63 (Mythril Rod). */
// FUNCTION: FF7 0x004d1c55
void __cdecl Battle_ThrowFxWeaponBlock62Index1(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 1;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 64 (Full Metal Staff). */
// FUNCTION: FF7 0x004d1c8a
void __cdecl Battle_ThrowFxWeaponBlock62Index2(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 2;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 65 (Striking Staff). */
// FUNCTION: FF7 0x004d1cbf
void __cdecl Battle_ThrowFxWeaponBlock62Index3(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 3;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 66 (Prism Staff). */
// FUNCTION: FF7 0x004d1cf4
void __cdecl Battle_ThrowFxWeaponBlock62Index4(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 4;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 67 (Aurora Rod). */
// FUNCTION: FF7 0x004d1d29
void __cdecl Battle_ThrowFxWeaponBlock62Index5(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 5;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 68 (Wizard Staff). */
// FUNCTION: FF7 0x004d1d5e
void __cdecl Battle_ThrowFxWeaponBlock62Index6(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 6;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 69 (Wizer Staff). */
// FUNCTION: FF7 0x004d1d93
void __cdecl Battle_ThrowFxWeaponBlock62Index7(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 7;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 70 (Fairy Tale). */
// FUNCTION: FF7 0x004d1dc8
void __cdecl Battle_ThrowFxWeaponBlock62Index8(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 8;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 71 (Umbrella). */
// FUNCTION: FF7 0x004d1dfd
void __cdecl Battle_ThrowFxWeaponBlock62Index9(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 9;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Throws Aeris weapon 72 (Princess Guard). */
// FUNCTION: FF7 0x004d1e32
void __cdecl Battle_ThrowFxWeaponBlock62Index10(int targetMask, int actorIdx)
{
    g_BattleThrowAerisWeaponIndex = 10;
    g_BattleThrowAerisWeaponFlag = rand() % 2;
    Battle_ThrowFxSpawnWeapons62to72(targetMask, actorIdx);
}

/* Schedules draw setup and timeline dispatch task for thrown Aeris staff. */
// FUNCTION: FF7 0x004d1e67
void __cdecl Battle_ThrowFxSpawnWeapons62to72(int targetMask, int actorIdx)
{
    int bitIdx;
    int targetCount;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_ThrowLoadResAerisWeapons();
    g_BattleThrowAerisTargetMask = targetMask;
    bitIdx = 0;
    for (targetCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                g_BattleThrowAerisTargetIndex = bitIdx;
            }
        }
    }
    g_BattleThrowAerisActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_ThrowAerisWeaponsDispatchKindTask);
}

/* Determines trajectory kind (straight vs arc) and schedules timeline task. */
// FUNCTION: FF7 0x004d1f07
void __cdecl BattleFx_ThrowAerisWeaponsDispatchKindTask(void)
{
    struct {
        PsxMatrix matrix;
        int slot;
        short rotation[4];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.slot = (int)BATTLE_CURRENT_TASK_SLOT;
    *(short *)local.slot = 0xFFFF;
    Battle_GetBoneWorldPos(
        g_BattleThrowAerisTargetIndex,
        g_BattleActorRecords[g_BattleThrowAerisTargetIndex].targetBone,
        &g_BattleThrowAerisTargetPos.x);
    Battle_GetBoneWorldPos(
        g_BattleThrowAerisActorIndex,
        g_BattleActorRecords[g_BattleThrowAerisActorIndex].targetBone,
        &g_BattleThrowAerisSourcePos.x);
    g_BattleThrowAerisSourcePos.yaw =
        g_BattleActorRecords[g_BattleThrowAerisActorIndex].yaw;
    local.rotation[0] = 0;
    local.rotation[1] = g_BattleThrowAerisSourcePos.yaw;
    local.rotation[2] = 0;
    RotMatrixXYZ((void *)local.rotation, &local.matrix);
    TransformVectorToInt32((PsxMatrix *)&local.matrix, &g_BattleThrowAerisVectorOffset, &g_BattleThrowAerisTransformedVector);
    switch (g_BattleThrowAerisWeaponFlag) {
    case BATTLE_THROW_AERIS_FLIGHT_STRAIGHT:
        local.slot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowAerisWeaponsStraightTimelineTask);
        break;
    case BATTLE_THROW_AERIS_FLIGHT_ARC:
        local.slot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowAerisWeaponsArcTimelineTask);
        break;
    }
    *(short *)((char *)local.slot + 0x1C) = 0;
}

/* Timeline task for straight flight trajectory. */
// FUNCTION: FF7 0x004d202a
void __cdecl BattleFx_ThrowAerisWeaponsStraightTimelineTask(void)
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
        g_BattleThrowAerisDeltaVectorX = g_BattleThrowAerisTargetPos.x - g_BattleThrowAerisSourcePos.x;
        g_BattleThrowAerisDeltaVectorY = g_BattleThrowAerisTargetPos.y - g_BattleThrowAerisSourcePos.y;
        g_BattleThrowAerisDeltaVectorZ = g_BattleThrowAerisTargetPos.z - g_BattleThrowAerisSourcePos.z;
        norm = NormalizeVectorFixed(&g_BattleThrowAerisDeltaVectorX, &g_BattleThrowAerisDeltaVectorX);
        speed = SqrtToInt(norm) / 6;
        pProjectile = (BattleFxAerisThrowDelta *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowAerisWeaponsStraightProjectileTask);
        *(Int64Pair *)((char *)pProjectile + 4) = *(Int64Pair *)&g_BattleThrowAerisSourcePos;
        pProjectile->deltaX = speed * g_BattleThrowAerisDeltaVectorX >> 0xC;
        pProjectile->deltaY = speed * g_BattleThrowAerisDeltaVectorY >> 0xC;
        pProjectile->deltaZ = speed * g_BattleThrowAerisDeltaVectorZ >> 0xC;
    }
    if (task->frameCounter == 6) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowAerisTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowAerisTargetPos.x), 0x32);
    }
    if (task->frameCounter >= 0x1E) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates and renders 3D mesh for straight thrown staff. */
// FUNCTION: FF7 0x004d219b
void __cdecl BattleFx_ThrowAerisWeaponsStraightProjectileTask(void)
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
    frame.angle = ComputeAngleBetweenVectors(&g_BattleThrowAerisVectorOffset, (VECTOR *)&g_BattleThrowAerisDeltaVectorX, &frame.axis);
    Gte_BuildAxisAngleMatrix(frame.angle, &frame.axisMatrix, &frame.axis);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = 0x17C;
    TransformVectorToInt32(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
    frame.rotMatrix.t[0] += frame.offsetX.vx;
    frame.rotMatrix.t[1] += frame.offsetX.vy;
    frame.rotMatrix.t[2] += frame.offsetX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowAerisWeaponIndex, g_BattleThrowAerisLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.projectile->timer >= 0x16) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowAerisWeaponIndex + 0xB, g_BattleThrowAerisLoadedResTable);
        frame.modelDraw->fadeLevel = frame.projectile->timer - 0x16 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.projectile->timer >= 0x1E) {
        frame.projectile->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.projectile->timer < 6) {
        frame.projectile->posX += frame.projectile->velocityX;
        frame.projectile->posY += frame.projectile->velocityY;
        frame.projectile->posZ += frame.projectile->velocityZ;
    }
    (frame.projectile->timer)++;
}

/* Timeline task for high-arc flight trajectory. */
// FUNCTION: FF7 0x004d23f6
void __cdecl BattleFx_ThrowAerisWeaponsArcTimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxAerisArcSlot *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowAerisDeltaVectorX = g_BattleThrowAerisTargetPos.x - g_BattleThrowAerisSourcePos.x;
        g_BattleThrowAerisDeltaVectorY = g_BattleThrowAerisTargetPos.y - g_BattleThrowAerisSourcePos.y;
        g_BattleThrowAerisDeltaVectorZ = g_BattleThrowAerisTargetPos.z - g_BattleThrowAerisSourcePos.z;
        norm = NormalizeVectorFixed(&g_BattleThrowAerisDeltaVectorX, &g_BattleThrowAerisDeltaVectorX);
        speed = (SqrtToInt(norm) << 1) / 0xA;
        pProjectile = (BattleFxAerisArcSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowAerisWeaponsArcProjectileTask);
        *(Int64Pair *)((char *)pProjectile + 4) = *(Int64Pair *)&g_BattleThrowAerisSourcePos;
        pProjectile->arcVelocity = 0xFFF4;
        pProjectile->velocityX = speed * g_BattleThrowAerisDeltaVectorX >> 0xC;
        pProjectile->velocityY = speed * g_BattleThrowAerisDeltaVectorY >> 0xC;
        pProjectile->velocityZ = speed * g_BattleThrowAerisDeltaVectorZ >> 0xC;
        pProjectile->angle = 0;
        pProjectile->angleVelocity = 0x320;
    }
    if (task->frameCounter == 5) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowAerisTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowAerisTargetPos.x), 0x32);
    }
    if (task->frameCounter >= 0x1E) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates and renders 3D mesh for high-arc thrown staff. */
// FUNCTION: FF7 0x004d2584
void __cdecl BattleFx_ThrowAerisWeaponsArcProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxAerisArcProjectile *projectile; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int angle; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR rotX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axis; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxAerisArcProjectile *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.projectile->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.projectile->posX;
    frame.rotMatrix.t[1] = frame.projectile->posY;
    frame.rotMatrix.t[2] = frame.projectile->posZ;
    frame.angle = ComputeAngleBetweenVectors(&g_BattleThrowAerisVectorOffset, (VECTOR *)&g_BattleThrowAerisDeltaVectorX, &frame.axis);
    Gte_BuildAxisAngleMatrix(frame.angle, &frame.axisMatrix, &frame.axis);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowAerisWeaponIndex, g_BattleThrowAerisLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.projectile->frameCount >= 6) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowAerisWeaponIndex + 0xB, g_BattleThrowAerisLoadedResTable);
        frame.modelDraw->fadeLevel = (frame.projectile->frameCount - 6) * 0xAA;
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
    if (frame.projectile->frameCount >= 5) {
        frame.projectile->posY += frame.projectile->arcVelocity;
        frame.projectile->arcVelocity += (short)(frame.projectile->arcVelocity >> 1);
    }
    frame.projectile->angle += frame.projectile->angleVelocity;
    (frame.projectile->frameCount)++;
}
} /* extern "C" */

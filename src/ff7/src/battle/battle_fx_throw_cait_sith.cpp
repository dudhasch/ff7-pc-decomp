// src/ff7/src/battle/battle_fx_throw_cait_sith.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Throw effects for Cait Sith's megaphone weapons: loads the models, then per-weapon
//   entry points start the flight timeline and spinning-model tasks.

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
extern "C" {

#pragma intrinsic(memset)
#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x0082d728
unsigned int g_BattleThrowCaitSithResourceDef[130] = {
    0x001900b6,
    // STRING: FF7 0x0082d940
    (unsigned int)"mega01.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d94c
    (unsigned int)"mega02.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d958
    (unsigned int)"mega03.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d964
    (unsigned int)"mega04.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d970
    (unsigned int)"mega05.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d97c
    (unsigned int)"mega06.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d988
    (unsigned int)"mega07.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d994
    (unsigned int)"mega08.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d9a0
    (unsigned int)"mega09.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d9ac
    (unsigned int)"mega10.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d9b8
    (unsigned int)"mega11.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d9c4
    (unsigned int)"mega12.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082d9d0
    (unsigned int)"mega13.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082d9dc
    (unsigned int)"mega01.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082d9e8
    (unsigned int)"mega02.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082d9f4
    (unsigned int)"mega03.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da00
    (unsigned int)"mega04.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da0c
    (unsigned int)"mega05.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da18
    (unsigned int)"mega06.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da24
    (unsigned int)"mega07.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da30
    (unsigned int)"mega08.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da3c
    (unsigned int)"mega09.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da48
    (unsigned int)"mega10.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da54
    (unsigned int)"mega11.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da60
    (unsigned int)"mega12.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082da6c
    (unsigned int)"mega13.rsd",
    0x00000000,
    0x00000000,
    0x00000000
};
// The vector BattleFx_ThrowCaitSithWeaponsSpawnTimeline transforms (0x4CDB55 pushes its address): (0, 0, -4096)
// and its pad, 16 bytes. Its zero head cannot be a definition of its own (an all-zero definition goes to .bss).
// GLOBAL: FF7 0x0082d930
int g_BattleThrowCaitSithVectorOffset[4] = {0, 0, -4096, 0};
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
// GLOBAL: FF7 0x00b37240
int g_BattleThrowCaitSithTargetIndex = 0;
unsigned char g_bssPad_00b37244[4] = {0};
// GLOBAL: FF7 0x00b37248
int g_BattleThrowCaitSithDeltaVectorX = 0;
// GLOBAL: FF7 0x00b3724c
int g_BattleThrowCaitSithDeltaVectorY = 0;
// GLOBAL: FF7 0x00b37250
int g_BattleThrowCaitSithDeltaVectorZ = 0;
unsigned char g_bssPad_00b37254[4] = {0};
// GLOBAL: FF7 0x00b37258
BattleFxCaitSithBonePos g_BattleThrowCaitSithSourcePos = {0};
// GLOBAL: FF7 0x00b37260
int g_BattleThrowCaitSithWeaponFlag = 0;
// GLOBAL: FF7 0x00b37264
int g_BattleThrowCaitSithWeaponIndex = 0;
// GLOBAL: FF7 0x00b37268
short g_BattleThrowCaitSithTargetPos[3] = {0};
// GLOBAL: FF7 0x00b37270
int g_BattleThrowCaitSithTargetMask = 0;
// GLOBAL: FF7 0x00b37274
int g_BattleThrowCaitSithActorIndex = 0;
// GLOBAL: FF7 0x00b37278
VECTOR g_BattleThrowCaitSithTransformedVector = {0};
// GLOBAL: FF7 0x00b37288
struct GfxTableCursor *g_BattleThrowCaitSithLoadedResTable = 0;
unsigned char g_bssPad_00b3728c[4] = {0};
// clang-format on

/* Loads Cait Sith megaphone models and throw effect resources. */
// FUNCTION: FF7 0x004cd810
void __cdecl BattleFx_ThrowLoadResCaitSithWeapons(void)
{
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest requestBuffer;
    GfxModelTableBank *loadParam;
    GfxResourceRequest *request;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleThrowCaitSithLoadedResTable == 0) {
        typeId = 0x1A;
        def = (int)&g_BattleThrowCaitSithResourceDef;
        loadParam = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082da78
        request->resourcePath = (char *)BattleFx_BuildResourcePath("item/wp_ke", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, def, request, &g_BattleThrowCaitSithLoadedResTable, loadParam);
    }
}

/* Throws Cait Sith weapon 101 (Yellow Megaphone). */
// FUNCTION: FF7 0x004cd8a0
void __cdecl Battle_ThrowFxWeaponBlock101(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 0;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 102 (Green Megaphone). */
// FUNCTION: FF7 0x004cd8c9
void __cdecl Battle_ThrowFxWeaponBlock101Index1(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 1;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 103 (Blue Megaphone). */
// FUNCTION: FF7 0x004cd8f2
void __cdecl Battle_ThrowFxWeaponBlock101Index2(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 2;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 104 (Red Megaphone). */
// FUNCTION: FF7 0x004cd91b
void __cdecl Battle_ThrowFxWeaponBlock101Index3(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 3;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 105 (Crystal Megaphone). */
// FUNCTION: FF7 0x004cd944
void __cdecl Battle_ThrowFxWeaponBlock101Index4(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 4;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 106 (White Megaphone). */
// FUNCTION: FF7 0x004cd96d
void __cdecl Battle_ThrowFxWeaponBlock101Index5(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 5;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 107 (Black Megaphone). */
// FUNCTION: FF7 0x004cd996
void __cdecl Battle_ThrowFxWeaponBlock101Index6(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 6;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 108 (Silver Megaphone). */
// FUNCTION: FF7 0x004cd9bf
void __cdecl Battle_ThrowFxWeaponBlock101Index7(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 7;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 109 (Trumpet Shell). */
// FUNCTION: FF7 0x004cd9e8
void __cdecl Battle_ThrowFxWeaponBlock101Index8(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 8;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 110 (Gold Megaphone). */
// FUNCTION: FF7 0x004cda11
void __cdecl Battle_ThrowFxWeaponBlock101Index9(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 9;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 111 (Battle Trumpet). */
// FUNCTION: FF7 0x004cda3a
void __cdecl Battle_ThrowFxWeaponBlock101Index10(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 0xA;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 112 (Starlight Phone). */
// FUNCTION: FF7 0x004cda63
void __cdecl Battle_ThrowFxWeaponBlock101Index11(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 0xB;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Throws Cait Sith weapon 113 (HP Shout). */
// FUNCTION: FF7 0x004cda8c
void __cdecl Battle_ThrowFxWeaponBlock101Index12(int targetMask, int actorIdx)
{
    g_BattleThrowCaitSithWeaponIndex = 0xC;
    g_BattleThrowCaitSithWeaponFlag = 0;
    Battle_ThrowFxSpawnWeapons101to113(targetMask, actorIdx);
}

/* Schedules draw setup and timeline task for thrown Cait Sith megaphone. */
// FUNCTION: FF7 0x004cdab5
void __cdecl Battle_ThrowFxSpawnWeapons101to113(int targetMask, int actorIdx)
{
    int bitIdx;
    int targetCount;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_ThrowLoadResCaitSithWeapons();
    g_BattleThrowCaitSithTargetMask = targetMask;
    bitIdx = 0;
    for (targetCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                g_BattleThrowCaitSithTargetIndex = bitIdx;
            }
        }
    }
    g_BattleThrowCaitSithActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_ThrowCaitSithWeaponsSpawnTimeline);
}

/* Computes source and target bone positions and launches megaphone trajectory. */
// FUNCTION: FF7 0x004cdb55
void __cdecl BattleFx_ThrowCaitSithWeaponsSpawnTimeline(void)
{
    short yawAngles[3];
    BattleFxCaitSithSpawnSlot *task;
    int matrix[8];
    int weaponFlag;

    task = (BattleFxCaitSithSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleThrowCaitSithTargetIndex, g_BattleActorRecords[g_BattleThrowCaitSithTargetIndex].targetBone, g_BattleThrowCaitSithTargetPos);
    Battle_GetBoneWorldPos(g_BattleThrowCaitSithActorIndex, g_BattleActorRecords[g_BattleThrowCaitSithActorIndex].targetBone, (short *)&g_BattleThrowCaitSithSourcePos);
    g_BattleThrowCaitSithSourcePos.yaw = g_BattleActorRecords[g_BattleThrowCaitSithActorIndex].yaw;
    yawAngles[0] = 0;
    yawAngles[1] = g_BattleThrowCaitSithSourcePos.yaw;
    yawAngles[2] = 0;
    RotMatrixXYZ((void *)yawAngles, (void *)matrix);
    TransformVectorToInt32((PsxMatrix *)matrix, &g_BattleThrowCaitSithVectorOffset, &g_BattleThrowCaitSithTransformedVector);
    weaponFlag = g_BattleThrowCaitSithWeaponFlag;
    if (weaponFlag != 0) { /* empty first arm: lib/codegen.h */
    } else {
        task = (BattleFxCaitSithSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCaitSithWeaponsTimelineTask);
    }
    task->unusedWord = 0;
}

/* Drives flight velocity and triggers impact sfx on megaphone arrival. */
// FUNCTION: FF7 0x004cdc54
void __cdecl BattleFx_ThrowCaitSithWeaponsTimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxCaitSithThrownWeapon *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowCaitSithDeltaVectorX = g_BattleThrowCaitSithTargetPos[0] - g_BattleThrowCaitSithSourcePos.x;
        g_BattleThrowCaitSithDeltaVectorY = g_BattleThrowCaitSithTargetPos[1] - g_BattleThrowCaitSithSourcePos.y;
        g_BattleThrowCaitSithDeltaVectorZ = g_BattleThrowCaitSithTargetPos[2] - g_BattleThrowCaitSithSourcePos.z;
        norm = NormalizeVectorFixed(&g_BattleThrowCaitSithDeltaVectorX, &g_BattleThrowCaitSithDeltaVectorX);
        speed = SqrtToInt(norm) / 0x14;
        pProjectile = (BattleFxCaitSithThrownWeapon *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCaitSithWeaponsThrownModelTask);
        *(Int64Pair *)((char *)pProjectile + 4) = *(Int64Pair *)&g_BattleThrowCaitSithSourcePos;
        pProjectile->offsetX = speed * g_BattleThrowCaitSithDeltaVectorX >> 0xC;
        pProjectile->offsetY = speed * g_BattleThrowCaitSithDeltaVectorY >> 0xC;
        pProjectile->offsetZ = speed * g_BattleThrowCaitSithDeltaVectorZ >> 0xC;
        pProjectile->angleRate = 0x66;
        pProjectile->angle = 0x66;
        pProjectile->countdownStart = rand() % 0xC8 + 0xB4;
        pProjectile->countdown = pProjectile->countdownStart;
        pProjectile->lifetime = rand() % 0x96 + 0xA0;
        pProjectile->rotationZ = pProjectile->lifetime;
    }
    if (task->frameCounter == 0x15) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowCaitSithTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(g_BattleThrowCaitSithTargetPos), 0xE1);
    }
    if (task->frameCounter >= 0x1E) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates rotation and renders 3D mesh for thrown Cait Sith megaphone. */
// FUNCTION: FF7 0x004cde29
void __cdecl BattleFx_ThrowCaitSithWeaponsThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxBarretWeaponTumble *throwRecord; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int bobOffset; };
        struct { unsigned char pad_to_38[0x38]; int stepCount; };
        struct { unsigned char pad_to_3C[0x3C]; int angleBetween; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_68[0x68]; VECTOR axisVector; };
        struct { unsigned char pad_to_78[0x78]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.throwRecord = (BattleFxBarretWeaponTumble *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleBetween = ComputeAngleBetweenVectors((VECTOR *)&g_BattleThrowCaitSithTransformedVector, (VECTOR *)&g_BattleThrowCaitSithDeltaVectorX, &frame.axisVector);
    frame.stepCount = frame.angleBetween / 0xBE + 1;
    frame.bobOffset = (SinFixed(frame.throwRecord->angle) * -0x2BC >> 0xC) * frame.stepCount;
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCaitSithWeaponIndex, g_BattleThrowCaitSithLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.throwRecord->timer >= 0x1A) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCaitSithWeaponIndex + 0xD, g_BattleThrowCaitSithLoadedResTable);
        frame.modelDraw->fadeLevel = frame.throwRecord->timer - 0x1A << 0xA;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.throwRecord->timer >= 0x1E) {
        frame.throwRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.throwRecord->posX += frame.throwRecord->velocityX;
    frame.throwRecord->posY += frame.throwRecord->velocityY;
    frame.throwRecord->posZ += frame.throwRecord->velocityZ;
    frame.throwRecord->angle += frame.throwRecord->angleRate;
    if (frame.throwRecord->timer == 0x14) {
        frame.throwRecord->angle = 0;
    }
    frame.throwRecord->angleX += frame.throwRecord->angleXRate;
    frame.throwRecord->angleZ += frame.throwRecord->angleZRate;
    (frame.throwRecord->timer)++;
}
} /* extern "C" */

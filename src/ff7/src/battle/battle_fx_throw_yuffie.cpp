// src/ff7/src/battle/battle_fx_throw_yuffie.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Throw effects for Yuffie's shuriken and the Gil coin throw, with per-weapon entry
//   points and four flight kinds, each with its own timeline and model tasks.

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
// GLOBAL: FF7 0x0082da88
unsigned int g_BattleThrowYuffieResourceDef[150] = {
    0x001900b6,
    // STRING: FF7 0x0082dcf0
    (unsigned int)"shuriken01.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd00
    (unsigned int)"shuriken02.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd10
    (unsigned int)"shuriken03.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd20
    (unsigned int)"shuriken04.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd30
    (unsigned int)"shuriken05.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd40
    (unsigned int)"shuriken06.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd50
    (unsigned int)"shuriken07.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd60
    (unsigned int)"shuriken08.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd70
    (unsigned int)"shuriken09.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd80
    (unsigned int)"shuriken10.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dd90
    (unsigned int)"shuriken11.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082dda0
    (unsigned int)"shuriken12.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082ddb0
    (unsigned int)"shuriken13.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082ddc0
    (unsigned int)"shuriken14.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b6,
    // STRING: FF7 0x0082ddd0
    (unsigned int)"zeni.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082dddc
    (unsigned int)"shuriken01.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082ddec
    (unsigned int)"shuriken02.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082ddfc
    (unsigned int)"shuriken03.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de0c
    (unsigned int)"shuriken04.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de1c
    (unsigned int)"shuriken05.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de2c
    (unsigned int)"shuriken06.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de3c
    (unsigned int)"shuriken07.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de4c
    (unsigned int)"shuriken08.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de5c
    (unsigned int)"shuriken09.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de6c
    (unsigned int)"shuriken10.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de7c
    (unsigned int)"shuriken11.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de8c
    (unsigned int)"shuriken12.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082de9c
    (unsigned int)"shuriken13.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082deac
    (unsigned int)"shuriken14.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0082debc
    (unsigned int)"zeni.rsd",
    0x00000000, 0x00000000, 0x00000000
};
// The unit vector along -Z, the weapon model's flight axis. A full VECTOR with its pad word:
// VC5 pads neither $SG nor the literal pool start, so the four zero bytes before the pool are its tail.
// GLOBAL: FF7 0x0082dce0
VECTOR g_BattleThrowYuffieVectorOffset = {0, 0, -0x1000, 0};
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
// GLOBAL: FF7 0x00b37290
int g_BattleThrowYuffieTargetIndex = 0;
unsigned char g_bssPad_00b37294[4] = {0};
// GLOBAL: FF7 0x00b37298
int g_BattleThrowYuffieDeltaVector = 0;
// GLOBAL: FF7 0x00b3729c
int g_BattleThrowYuffieDeltaVectorY = 0;
// GLOBAL: FF7 0x00b372a0
int g_BattleThrowYuffieDeltaVectorZ = 0;
unsigned char g_bssPad_00b372a4[4] = {0};
// GLOBAL: FF7 0x00b372a8
struct BattleFxThrowYuffieSourcePos g_BattleThrowYuffieSourcePos = {0};
// GLOBAL: FF7 0x00b372b0
int g_BattleThrowYuffieKind3Cursor = 0;
// GLOBAL: FF7 0x00b372b4
int g_BattleThrowYuffieKind3Phase = 0;
// GLOBAL: FF7 0x00b372b8
BattleFxYuffieThrowState g_BattleThrowYuffieThrowState = {0};
// GLOBAL: FF7 0x00b372e0
struct BattleFxYuffieThrowTarget g_BattleThrowYuffieTargetBonePos = {0};
// GLOBAL: FF7 0x00b372e8
int g_BattleThrowYuffieTargetMask = 0;
// GLOBAL: FF7 0x00b372ec
int g_BattleThrowYuffieStepRatio = 0;
// GLOBAL: FF7 0x00b372f0
BattleFxYuffieKind3Target g_BattleThrowYuffieKind3Targets[6] = {0};
// GLOBAL: FF7 0x00b37368
int g_BattleThrowYuffieActorIndex = 0;
unsigned char g_bssPad_00b3736c[4] = {0};
// GLOBAL: FF7 0x00b37370
VECTOR g_BattleThrowYuffieTransformedOffset = {0};
// GLOBAL: FF7 0x00b37380
struct GfxTableCursor *g_BattleThrowYuffieLoadedResTable = 0;
// clang-format on

/* Loads the item/wp_yu model table (the weapon models, their fading copies and the coin) once, when the 3D scene exists. */
// FUNCTION: FF7 0x004ce0f0
void __cdecl BattleFx_ThrowLoadResYuffieWeapons(void)
{
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest requestBuffer;
    GfxModelTableBank *loadParam;
    GfxResourceRequest *request;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleThrowYuffieLoadedResTable == 0) {
        typeId = 0x1E;
        def = (int)&g_BattleThrowYuffieResourceDef;
        loadParam = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082dec8
        request->resourcePath = BattleFx_BuildResourcePath("item/wp_yu", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, def, request, &g_BattleThrowYuffieLoadedResTable, loadParam);
    }
}

/* Throws Yuffie weapon 87 (4-point Shuriken). */
// FUNCTION: FF7 0x004ce180
void __cdecl Battle_ThrowFxWeaponBlock87(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 0;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 88 (Boomerang). */
// FUNCTION: FF7 0x004ce1a9
void __cdecl Battle_ThrowFxWeaponBlock87Index1(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 1;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_CURVED;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 89 (Pinwheel). */
// FUNCTION: FF7 0x004ce1d2
void __cdecl Battle_ThrowFxWeaponBlock87Index2(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 2;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 90 (Razor Ring). */
// FUNCTION: FF7 0x004ce1fb
void __cdecl Battle_ThrowFxWeaponBlock87Index3(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 3;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_CURVED;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 91 (Hawkeye). */
// FUNCTION: FF7 0x004ce224
void __cdecl Battle_ThrowFxWeaponBlock87Index4(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 4;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 92 (Crystal Cross). */
// FUNCTION: FF7 0x004ce24d
void __cdecl Battle_ThrowFxWeaponBlock87Index5(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 5;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 93 (Wind Slash). */
// FUNCTION: FF7 0x004ce276
void __cdecl Battle_ThrowFxWeaponBlock87Index6(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 6;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 94 (Twin Viper). */
// FUNCTION: FF7 0x004ce29f
void __cdecl Battle_ThrowFxWeaponBlock87Index7(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 7;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 95 (Spiral Shuriken). */
// FUNCTION: FF7 0x004ce2c8
void __cdecl Battle_ThrowFxWeaponBlock87Index8(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 8;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_LOOPING;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 96 (Super Ball). */
// FUNCTION: FF7 0x004ce2f1
void __cdecl Battle_ThrowFxWeaponBlock87Index9(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 9;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_CURVED;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 97 (Magic Shuriken). */
// FUNCTION: FF7 0x004ce31a
void __cdecl Battle_ThrowFxWeaponBlock87Index10(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 0xA;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_LOOPING;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 98 (Rising Sun). */
// FUNCTION: FF7 0x004ce343
void __cdecl Battle_ThrowFxWeaponBlock87Index11(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 0xB;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_CURVED;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 99 (Oritsuru). */
// FUNCTION: FF7 0x004ce36c
void __cdecl Battle_ThrowFxWeaponBlock87Index12(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 0xC;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Yuffie weapon 100 (Conformer). */
// FUNCTION: FF7 0x004ce395
void __cdecl Battle_ThrowFxWeaponBlock87Index13(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 0xD;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_STRAIGHT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/* Throws Gil coins (Coin Throw command). */
// FUNCTION: FF7 0x004ce3be
void __cdecl Battle_CoinThrowEffect(int targetMask, int actorIdx)
{
    g_BattleThrowYuffieThrowState.weaponModel = 0xE;
    g_BattleThrowYuffieThrowState.flightKind = BATTLE_YUFFIE_THROW_MULTI_ORBIT;
    Battle_ThrowFxSpawnWeapons87to100(targetMask, actorIdx);
}

/*
 * Starts a throw: the draw-list buffer task, the resources, the target mask and its first target,
 * the thrower, and the task that starts the flight kind's timeline.
 */
// FUNCTION: FF7 0x004ce3e7
void __cdecl Battle_ThrowFxSpawnWeapons87to100(int targetMask, int actorIdx)
{
    int bitIdx;
    int targetCount;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_ThrowLoadResYuffieWeapons();
    g_BattleThrowYuffieTargetMask = targetMask;
    bitIdx = 0;
    for (targetCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                g_BattleThrowYuffieTargetIndex = bitIdx;
            }
        }
    }
    g_BattleThrowYuffieActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_ThrowYuffieWeaponsDispatchKindTask);
}

/*
 * Runs once: reads the first target's and the thrower's bone positions, turns the flight axis by the
 * thrower's yaw, and starts the timeline of the weapon's flight kind.
 */
// FUNCTION: FF7 0x004ce487
void __cdecl BattleFx_ThrowYuffieWeaponsDispatchKindTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxYuffieWeaponTimeline2 *timelineRecord; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
    } frame;
#pragma pack(pop)

    frame.timelineRecord = (BattleFxYuffieWeaponTimeline2 *)BATTLE_CURRENT_TASK_SLOT;
    frame.timelineRecord->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleThrowYuffieTargetIndex, g_BattleActorRecords[g_BattleThrowYuffieTargetIndex].targetBone, &g_BattleThrowYuffieTargetBonePos.targetX);
    Battle_GetBoneWorldPos(g_BattleThrowYuffieActorIndex, g_BattleActorRecords[g_BattleThrowYuffieActorIndex].targetBone, &g_BattleThrowYuffieSourcePos.sourceX);
    g_BattleThrowYuffieSourcePos.yaw = g_BattleActorRecords[g_BattleThrowYuffieActorIndex].yaw;
    frame.rotX.vx = 0;
    frame.rotX.vy = g_BattleThrowYuffieSourcePos.yaw;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    TransformVectorToInt32(&frame.rotMatrix, &g_BattleThrowYuffieVectorOffset, &g_BattleThrowYuffieTransformedOffset);
    switch (g_BattleThrowYuffieThrowState.flightKind) {
    case BATTLE_YUFFIE_THROW_STRAIGHT:
        frame.timelineRecord = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind0TimelineTask);
        break;
    case BATTLE_YUFFIE_THROW_CURVED:
        frame.timelineRecord = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind1TimelineTask);
        break;
    case BATTLE_YUFFIE_THROW_LOOPING:
        frame.timelineRecord = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind2TimelineTask);
        break;
    case BATTLE_YUFFIE_THROW_MULTI_ORBIT:
        frame.timelineRecord = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind3TimelineTask);
    }
    frame.timelineRecord->subCounter = 0;
}

/*
 * Straight throw timeline, unpaused only: frame 0 throws the weapon from the thrower at the target's
 * bone; frame 7 queues the target's typed record and plays the hit sound 0x159; retires at frame 30.
 */
// FUNCTION: FF7 0x004ce5f8
void __cdecl BattleFx_ThrowYuffieWeaponsKind0TimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxYuffieKind0ThrownModel *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowYuffieDeltaVector = g_BattleThrowYuffieTargetBonePos.targetX - g_BattleThrowYuffieSourcePos.sourceX;
        g_BattleThrowYuffieDeltaVectorY = g_BattleThrowYuffieTargetBonePos.targetY - g_BattleThrowYuffieSourcePos.sourceY;
        g_BattleThrowYuffieDeltaVectorZ = g_BattleThrowYuffieTargetBonePos.targetZ - g_BattleThrowYuffieSourcePos.sourceZ;
        norm = NormalizeVectorFixed(&g_BattleThrowYuffieDeltaVector, &g_BattleThrowYuffieDeltaVector);
        speed = (SqrtToInt(norm) << 1) / 12;
        pProjectile = (BattleFxYuffieKind0ThrownModel *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind0ThrownModelTask);
        pProjectile->pos = *(Int64Pair *)&g_BattleThrowYuffieSourcePos;
        pProjectile->velocityX = speed * g_BattleThrowYuffieDeltaVector >> 12;
        pProjectile->velocityY = speed * g_BattleThrowYuffieDeltaVectorY >> 12;
        pProjectile->velocityZ = speed * g_BattleThrowYuffieDeltaVectorZ >> 12;
        pProjectile->angle = 0;
        pProjectile->spin = 0x2BC;
    }
    if (task->frameCounter == 7) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowYuffieTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowYuffieTargetBonePos), 0x159);
    }
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates and renders 3D mesh for Kind 0 straight thrown weapon. */
// FUNCTION: FF7 0x004ce780
void __cdecl BattleFx_ThrowYuffieWeaponsKind0ThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxYuffieKind0ThrownRecord *thrownRecord; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int angleBetween; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR rotX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axisVector; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.thrownRecord = (BattleFxYuffieKind0ThrownRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.thrownRecord->accumA;
    frame.rotX.vz = 0xFC00 /* 5670 deg */;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.thrownRecord->posX;
    frame.rotMatrix.t[1] = frame.thrownRecord->posY;
    frame.rotMatrix.t[2] = frame.thrownRecord->posZ;
    frame.angleBetween = ComputeAngleBetweenVectors(&g_BattleThrowYuffieVectorOffset, (VECTOR *)&g_BattleThrowYuffieDeltaVector, &frame.axisVector);
    Gte_BuildAxisAngleMatrix(frame.angleBetween, &frame.axisMatrix, &frame.axisVector);
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel, g_BattleThrowYuffieLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.thrownRecord->timer >= 22) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel + 0xF, g_BattleThrowYuffieLoadedResTable);
        frame.modelDraw->fadeLevel = frame.thrownRecord->timer - 22 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.thrownRecord->timer >= 30) {
        frame.thrownRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.thrownRecord->posX += frame.thrownRecord->velX;
    frame.thrownRecord->posY += frame.thrownRecord->velY;
    frame.thrownRecord->posZ += frame.thrownRecord->velZ;
    frame.thrownRecord->accumA += frame.thrownRecord->stepA;
    (frame.thrownRecord->timer)++;
}

/*
 * Curved throw timeline, unpaused only: frame 0 throws the weapon at one and a half times the straight
 * speed; frame 20 queues the target's typed record and plays the hit sound 0x159; retires at frame 30.
 */
// FUNCTION: FF7 0x004ce9ea
void __cdecl BattleFx_ThrowYuffieWeaponsKind1TimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxYuffieKind0ThrownModel *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowYuffieDeltaVector = g_BattleThrowYuffieTargetBonePos.targetX - g_BattleThrowYuffieSourcePos.sourceX;
        g_BattleThrowYuffieDeltaVectorY = g_BattleThrowYuffieTargetBonePos.targetY - g_BattleThrowYuffieSourcePos.sourceY;
        g_BattleThrowYuffieDeltaVectorZ = g_BattleThrowYuffieTargetBonePos.targetZ - g_BattleThrowYuffieSourcePos.sourceZ;
        norm = NormalizeVectorFixed(&g_BattleThrowYuffieDeltaVector, &g_BattleThrowYuffieDeltaVector);
        speed = SqrtToInt(norm) * 3 / 12;
        pProjectile = (BattleFxYuffieKind0ThrownModel *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind1ThrownModelTask);
        pProjectile->pos = *(Int64Pair *)&g_BattleThrowYuffieSourcePos;
        pProjectile->velocityX = speed * g_BattleThrowYuffieDeltaVector >> 12;
        pProjectile->velocityY = speed * g_BattleThrowYuffieDeltaVectorY >> 12;
        pProjectile->velocityZ = speed * g_BattleThrowYuffieDeltaVectorZ >> 12;
        pProjectile->angle = 0;
        pProjectile->spin = 0x2BC;
    }
    if (task->frameCounter == 20) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowYuffieTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowYuffieTargetBonePos), 0x159);
    }
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates and renders 3D mesh for Kind 1 curved thrown weapon. */
// FUNCTION: FF7 0x004ceb73
void __cdecl BattleFx_ThrowYuffieWeaponsKind1ThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxYuffieThrownWeapon *thrownRecord; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int angleBetween; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR rotX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axisVector; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.thrownRecord = (BattleFxYuffieThrownWeapon *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.thrownRecord->angle;
    frame.rotX.vz = 0xFC00 /* 5670 deg */;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.thrownRecord->posX;
    frame.rotMatrix.t[1] = frame.thrownRecord->posY;
    frame.rotMatrix.t[2] = frame.thrownRecord->posZ;
    frame.angleBetween = ComputeAngleBetweenVectors(&g_BattleThrowYuffieVectorOffset, (VECTOR *)&g_BattleThrowYuffieDeltaVector, &frame.axisVector);
    Gte_BuildAxisAngleMatrix(frame.angleBetween, &frame.axisMatrix, &frame.axisVector);
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel, g_BattleThrowYuffieLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.thrownRecord->timer >= 24) {
        frame.thrownRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.thrownRecord->timer < 12) {
        frame.thrownRecord->posX += frame.thrownRecord->velX;
        frame.thrownRecord->posY += frame.thrownRecord->velY;
        frame.thrownRecord->posZ += frame.thrownRecord->velZ;
        frame.thrownRecord->angle += frame.thrownRecord->spin;
    } else if (frame.thrownRecord->timer < 24) {
        frame.thrownRecord->posX -= frame.thrownRecord->velX;
        frame.thrownRecord->posY -= frame.thrownRecord->velY;
        frame.thrownRecord->posZ -= frame.thrownRecord->velZ;
        frame.thrownRecord->angle += frame.thrownRecord->spin;
    }
    if (frame.thrownRecord->timer == 6) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowYuffieTargetBonePos), 0x159);
    }
    (frame.thrownRecord->timer)++;
}

/*
 * Looping throw timeline, unpaused only: frame 0 throws the weapon; frames 7 to 13 each start a fading
 * trail copy at a random point around the target, flying in along the throw; frame 26 queues the
 * target's typed record and plays sound 0x155; retires at frame 30.
 */
// FUNCTION: FF7 0x004cee2a
void __cdecl BattleFx_ThrowYuffieWeaponsKind2TimelineTask(void)
{
    BattleFxYuffieKind2TimelineRecord *pProjectile;
    int unusedWord[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int trailSpeed;
    int range;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowYuffieDeltaVector = g_BattleThrowYuffieTargetBonePos.targetX - g_BattleThrowYuffieSourcePos.sourceX;
        g_BattleThrowYuffieDeltaVectorY = g_BattleThrowYuffieTargetBonePos.targetY - g_BattleThrowYuffieSourcePos.sourceY;
        g_BattleThrowYuffieDeltaVectorZ = g_BattleThrowYuffieTargetBonePos.targetZ - g_BattleThrowYuffieSourcePos.sourceZ;
        NormalizeVectorFixed(&g_BattleThrowYuffieDeltaVector, &g_BattleThrowYuffieDeltaVector);
        pProjectile = (BattleFxYuffieKind2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind2ThrownModelTask);
        pProjectile->pos = *(Int64Pair *)&g_BattleThrowYuffieSourcePos;
        pProjectile->velX = g_BattleThrowYuffieDeltaVector * 0x12C >> 12;
        pProjectile->velY = g_BattleThrowYuffieDeltaVectorY * 0x12C >> 12;
        pProjectile->velZ = g_BattleThrowYuffieDeltaVectorZ * 0x12C >> 12;
        pProjectile->accumA = 0;
        pProjectile->stepA = 700;
    }
    if (task->frameCounter >= 7 && task->frameCounter <= 13) {
        pProjectile = (BattleFxYuffieKind2TimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind2TrailModelTask);
        pProjectile->pos = *(Int64Pair *)&g_BattleThrowYuffieTargetBonePos;
        range = g_BattleActorRecords[g_BattleThrowYuffieTargetIndex].radius;
        pProjectile->posX += (short)(rand() % (range << 1) - range);
        pProjectile->posY += (short)(rand() % (range << 1) - range);
        pProjectile->posZ += (short)(rand() % (range << 1) - range);
        trailSpeed = rand() % 150 + 300;
        range = trailSpeed * g_BattleThrowYuffieDeltaVector >> 12;
        pProjectile->posX -= (short)(range << 2);
        pProjectile->velX = range;
        range = trailSpeed * g_BattleThrowYuffieDeltaVectorY >> 12;
        pProjectile->posY -= (short)(range << 2);
        pProjectile->velY = range;
        range = trailSpeed * g_BattleThrowYuffieDeltaVectorZ >> 12;
        pProjectile->posZ -= (short)(range << 2);
        pProjectile->velZ = range;
        pProjectile->accumA = rand() % 200 + 300;
        pProjectile->stepA = rand() % 300 + 400;
        pProjectile->fadeLevel = 16 - (task->frameCounter - 13);
    }
    if (task->frameCounter == 26) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowYuffieTargetIndex);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowYuffieTargetBonePos), 0x155);
    }
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates and renders 3D mesh for Kind 2 looping thrown weapon. */
// FUNCTION: FF7 0x004cf15b
void __cdecl BattleFx_ThrowYuffieWeaponsKind2ThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxYuffieKind0ThrownRecord *thrownRecord; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int angleBetween; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR rotX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axisVector; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.thrownRecord = (BattleFxYuffieKind0ThrownRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.thrownRecord->accumA;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.thrownRecord->posX;
    frame.rotMatrix.t[1] = frame.thrownRecord->posY;
    frame.rotMatrix.t[2] = frame.thrownRecord->posZ;
    frame.angleBetween = ComputeAngleBetweenVectors(&g_BattleThrowYuffieVectorOffset, (VECTOR *)&g_BattleThrowYuffieDeltaVector, &frame.axisVector);
    Gte_BuildAxisAngleMatrix(frame.angleBetween, &frame.axisMatrix, &frame.axisVector);
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel, g_BattleThrowYuffieLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.thrownRecord->timer >= 2) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel + 0xF, g_BattleThrowYuffieLoadedResTable);
        frame.modelDraw->fadeLevel = frame.thrownRecord->timer - 2 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.thrownRecord->timer >= 4) {
        frame.thrownRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.thrownRecord->posX += frame.thrownRecord->velX;
    frame.thrownRecord->posY += frame.thrownRecord->velY;
    frame.thrownRecord->posZ += frame.thrownRecord->velZ;
    frame.thrownRecord->accumA += frame.thrownRecord->stepA;
    (frame.thrownRecord->timer)++;
}

/* Renders secondary trail meshes for Kind 2 thrown weapon. */
// FUNCTION: FF7 0x004cf3c5
void __cdecl BattleFx_ThrowYuffieWeaponsKind2TrailModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxYuffieWeaponTrail *trail; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int axisAngle; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR angleX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axisVector; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.trail = (BattleFxYuffieWeaponTrail *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vx = frame.trail->angle;
    frame.angleX.vy = 0;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.trail->posX;
    frame.rotMatrix.t[1] = frame.trail->posY;
    frame.rotMatrix.t[2] = frame.trail->posZ;
    frame.axisAngle = ComputeAngleBetweenVectors(&g_BattleThrowYuffieVectorOffset, (VECTOR *)&g_BattleThrowYuffieDeltaVector, &frame.axisVector);
    Gte_BuildAxisAngleMatrix(frame.axisAngle, &frame.axisMatrix, &frame.axisVector);
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
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel, g_BattleThrowYuffieLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.trail->frameCount < 4) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel + 0xF, g_BattleThrowYuffieLoadedResTable);
        frame.modelDraw->fadeLevel = 4096 - (frame.trail->frameCount << 10);
    } else if (frame.trail->frameCount >= frame.trail->lifetime - 4) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel + 0xF, g_BattleThrowYuffieLoadedResTable);
        frame.modelDraw->fadeLevel = frame.trail->frameCount - (frame.trail->lifetime - 4) << 10;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.trail->frameCount >= frame.trail->lifetime) {
        frame.trail->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.trail->frameCount < 4) {
        frame.trail->posX += frame.trail->velocityX;
        frame.trail->posY += frame.trail->velocityY;
        frame.trail->posZ += frame.trail->velocityZ;
        frame.trail->angle += frame.trail->angularVelocity;
    }
    if (frame.trail->frameCount == 4) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowYuffieTargetBonePos), 0x159);
    }
    (frame.trail->frameCount)++;
}

/*
 * Coin throw timeline, unpaused only: frame 0 records every target's bone position and spread (its
 * radius, at most 1200) and plays sound 0x2A0. Each frame it throws two coins at random points around
 * the next target of a shuffled order, until every target has had its share of the 24 coins. Frame 25
 * queues each target's typed record; retires at frame 30.
 */
// FUNCTION: FF7 0x004cf6d2
void __cdecl BattleFx_ThrowYuffieWeaponsKind3TimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        BattleTaskSlot *weaponSlot;
        struct { unsigned char pad_to_4[0x4]; BattleFxYuffieKind3Projectile *projectile; };
        struct { unsigned char pad_to_8[0x8]; int spread; };
        struct { unsigned char pad_to_C[0xC]; int tmpSwap; };
        struct { unsigned char pad_to_10[0x10]; int tmpRadius; };
        struct { unsigned char pad_to_14[0x14]; int targetCount; };
        struct { unsigned char pad_to_18[0x18]; int boneX; };
        struct { unsigned char pad_to_1A[0x1A]; short boneY; };
        struct { unsigned char pad_to_1C[0x1C]; int boneZ; };
        struct { unsigned char pad_to_20[0x20]; int actorIndex; };
    } frame;
#pragma pack(pop)

    frame.weaponSlot = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.weaponSlot->frameCounter == 0) {
        frame.actorIndex = 0;
        for (frame.targetCount = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleThrowYuffieTargetMask & 1 << frame.actorIndex) != 0) {
                g_BattleThrowYuffieKind3Targets[frame.targetCount].actorIndex = frame.actorIndex;
                Battle_GetBoneWorldPos(frame.actorIndex, g_BattleActorRecords[frame.actorIndex].targetBone, &g_BattleThrowYuffieKind3Targets[frame.targetCount].boneX);
                frame.tmpRadius = g_BattleActorRecords[frame.actorIndex].radius;
                if (frame.tmpRadius > 1200) {
                    frame.tmpRadius = 1200;
                }
                g_BattleThrowYuffieKind3Targets[frame.targetCount].spread = *(short *)&frame.tmpRadius;
                g_BattleThrowYuffieKind3Targets[frame.targetCount].throwCount = 0;
                frame.targetCount++;
            }
        }
        g_BattleThrowYuffieThrowState.targetCount = frame.targetCount;
        g_BattleThrowYuffieKind3Phase = BATTLE_YUFFIE_MULTI_THROW_SHUFFLE;
        g_BattleThrowYuffieStepRatio = 24 / frame.targetCount;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x2A0);
    }
    if (g_BattleThrowYuffieKind3Phase == BATTLE_YUFFIE_MULTI_THROW_SHUFFLE) {
        /* Numeric active target count (0..2). */
        switch (g_BattleThrowYuffieThrowState.targetCount) {
        case 0:
            break;
        case 1:
            g_BattleThrowYuffieThrowState.order[0] = 0;
            break;
        case 2:
            frame.actorIndex = rand() % 2;
            g_BattleThrowYuffieThrowState.order[0] = frame.actorIndex;
            g_BattleThrowYuffieThrowState.order[1] = frame.actorIndex + 1 & 1;
            break;
        default:
            for (frame.actorIndex = 0; frame.actorIndex < g_BattleThrowYuffieThrowState.targetCount; frame.actorIndex++) {
                g_BattleThrowYuffieThrowState.order[frame.actorIndex] = frame.actorIndex;
            }
            for (frame.actorIndex = 0; frame.actorIndex < g_BattleThrowYuffieThrowState.targetCount; frame.actorIndex++) {
                frame.targetCount = g_BattleThrowYuffieThrowState.targetCount - frame.actorIndex;
                if (frame.targetCount <= 1) {
                    break;
                }
                frame.tmpRadius = rand() % frame.targetCount + frame.actorIndex;
                frame.tmpSwap = g_BattleThrowYuffieThrowState.order[frame.actorIndex];
                g_BattleThrowYuffieThrowState.order[frame.actorIndex] = g_BattleThrowYuffieThrowState.order[frame.tmpRadius];
                g_BattleThrowYuffieThrowState.order[frame.tmpRadius] = frame.tmpSwap;
            }
        }
        g_BattleThrowYuffieKind3Phase = BATTLE_YUFFIE_MULTI_THROW_THROW;
        g_BattleThrowYuffieKind3Cursor = g_BattleThrowYuffieKind3Phase;
    }
    if (g_BattleThrowYuffieKind3Phase == BATTLE_YUFFIE_MULTI_THROW_THROW) {
        frame.targetCount = g_BattleThrowYuffieThrowState.order[g_BattleThrowYuffieKind3Cursor];
        frame.spread = g_BattleThrowYuffieKind3Targets[frame.targetCount].spread;
        for (frame.actorIndex = 0; frame.actorIndex < 2; frame.actorIndex++) {
            (g_BattleThrowYuffieKind3Targets[frame.targetCount].throwCount)++;
            if (g_BattleThrowYuffieKind3Targets[frame.targetCount].throwCount > g_BattleThrowYuffieStepRatio) {
                break;
            }
            *(Int64Pair *)&frame.boneX = *(Int64Pair *)&g_BattleThrowYuffieKind3Targets[frame.targetCount].boneX;
            *(short *)&frame.boneX += (short)(rand() % (frame.spread << 1) - frame.spread);
            frame.boneY += (short)(rand() % (frame.spread << 1) - frame.spread);
            *(short *)&frame.boneZ += (short)(rand() % (frame.spread << 1) - frame.spread);
            g_BattleThrowYuffieDeltaVector = *(short *)&frame.boneX - g_BattleThrowYuffieSourcePos.sourceX;
            g_BattleThrowYuffieDeltaVectorY = frame.boneY - g_BattleThrowYuffieSourcePos.sourceY;
            g_BattleThrowYuffieDeltaVectorZ = *(short *)&frame.boneZ - g_BattleThrowYuffieSourcePos.sourceZ;
            frame.tmpRadius = NormalizeVectorFixed(&g_BattleThrowYuffieDeltaVector, &g_BattleThrowYuffieDeltaVector);
            frame.tmpSwap = SqrtToInt(frame.tmpRadius) / 4;
            frame.projectile = (BattleFxYuffieKind3Projectile *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowYuffieWeaponsKind3ThrownModelTask);
            frame.projectile->pos = *(Int64Pair *)&g_BattleThrowYuffieSourcePos;
            frame.projectile->deltaX = frame.tmpSwap * g_BattleThrowYuffieDeltaVector >> 12;
            frame.projectile->deltaY = frame.tmpSwap * g_BattleThrowYuffieDeltaVectorY >> 12;
            frame.projectile->deltaZ = frame.tmpSwap * g_BattleThrowYuffieDeltaVectorZ >> 12;
            frame.projectile->lifetime = 30 - frame.weaponSlot->frameCounter;
        }
        g_BattleThrowYuffieKind3Cursor++;
        if (g_BattleThrowYuffieKind3Cursor >= g_BattleThrowYuffieThrowState.targetCount) {
            frame.actorIndex = 0;
            for (frame.targetCount = 0; frame.actorIndex < g_BattleThrowYuffieThrowState.targetCount; frame.actorIndex++) {
                if (g_BattleThrowYuffieKind3Targets[frame.actorIndex].throwCount >= g_BattleThrowYuffieStepRatio) {
                    frame.targetCount++;
                }
            }
            if (frame.targetCount >= g_BattleThrowYuffieThrowState.targetCount) {
                g_BattleThrowYuffieKind3Phase = BATTLE_YUFFIE_MULTI_THROW_DONE;
            } else {
                g_BattleThrowYuffieKind3Phase = BATTLE_YUFFIE_MULTI_THROW_SHUFFLE;
            }
        }
    }
    if (frame.weaponSlot->frameCounter == 25) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleThrowYuffieTargetMask & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
    }
    if (frame.weaponSlot->frameCounter >= 30) {
        frame.weaponSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.weaponSlot->frameCounter)++;
}

/* Animates and renders 3D mesh for Kind 3 thrown weapon. */
// FUNCTION: FF7 0x004cfbe5
void __cdecl BattleFx_ThrowYuffieWeaponsKind3ThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxYuffieThrownWeapon *thrownWeapon; };
        struct { unsigned char pad_to_24[0x24]; VECTOR velX; };
        struct { unsigned char pad_to_34[0x34]; int axisAngle; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR angleX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axisVector; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_74[0x74]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.thrownWeapon = (BattleFxYuffieThrownWeapon *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vx = 0;
    frame.angleX.vy = 0;
    frame.angleX.vz = 0x400 /* 90 deg */;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.thrownWeapon->posX;
    frame.rotMatrix.t[1] = frame.thrownWeapon->posY;
    frame.rotMatrix.t[2] = frame.thrownWeapon->posZ;
    frame.velX.vx = frame.thrownWeapon->velX;
    frame.velX.vy = frame.thrownWeapon->velY;
    frame.velX.vz = frame.thrownWeapon->velZ;
    NormalizeVectorFixed(&frame.velX, &frame.velX);
    frame.axisAngle = ComputeAngleBetweenVectors(&g_BattleThrowYuffieVectorOffset, &frame.velX, &frame.axisVector);
    Gte_BuildAxisAngleMatrix(frame.axisAngle, &frame.axisMatrix, &frame.axisVector);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
    frame.scale.vz = 0xCCC;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.velX.vx = 0;
    frame.velX.vy = 0;
    frame.velX.vz = 0;
    TransformVectorToInt32(&frame.rotMatrix, &frame.velX, &frame.velX);
    frame.rotMatrix.t[0] += frame.velX.vx;
    frame.rotMatrix.t[1] += frame.velX.vy;
    frame.rotMatrix.t[2] += frame.velX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel, g_BattleThrowYuffieLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.thrownWeapon->timer >= 6) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowYuffieThrowState.weaponModel + 0xF, g_BattleThrowYuffieLoadedResTable);
        frame.modelDraw->fadeLevel = frame.thrownWeapon->timer - 6 << 10;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.thrownWeapon->timer >= 10) {
        frame.thrownWeapon->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.thrownWeapon->timer < 4) {
        frame.thrownWeapon->posX += frame.thrownWeapon->velX;
        frame.thrownWeapon->posY += frame.thrownWeapon->velY;
        frame.thrownWeapon->posZ += frame.thrownWeapon->velZ;
    }
    (frame.thrownWeapon->timer)++;
}
} /* extern "C" */

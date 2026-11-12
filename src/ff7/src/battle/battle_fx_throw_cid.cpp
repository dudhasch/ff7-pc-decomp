// src/ff7/src/battle/battle_fx_throw_cid.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Throw effects for Cid's spear weapons: per-weapon entry points, four flight kinds, and
//   a ribbon trail drawn as shaded quads.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_fx.h"
#include "lib/enums/sound.h"
#include "ff7/enums/battle.h"
extern "C" {

#pragma intrinsic(memset)
#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x0082ded8
unsigned int g_BattleThrowCidResourceDef[140] = {0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon01, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon02, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon03, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon04, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon05, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon06, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon07, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon08, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon09, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon10, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon11, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon12, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon13, 0x00000000, 0x00000000, 0x00000000, 0x001900b6, (unsigned int)&g_BattleThrowCidResNameWeapon14, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon01_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon02_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon03_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon04_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon05_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon06_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon07_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon08_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon09_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon10_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon11_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon12_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon13_B, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleThrowCidResNameWeapon14_B, 0x00000000, 0x00000000, 0x00000000};
// The unit vector (0, 0, -0x1000) that the throw effects rotate to the flight direction.
// GLOBAL: FF7 0x0082e108
int g_BattleThrowCidVectorOffset[3] = {0, 0, -0x1000};
// GLOBAL: FF7 0x0082e118
unsigned char g_BattleThrowCidResNameWeapon01[13] = "weapon01.rsd";
// GLOBAL: FF7 0x0082e128
unsigned char g_BattleThrowCidResNameWeapon02[13] = "weapon02.rsd";
// GLOBAL: FF7 0x0082e138
unsigned char g_BattleThrowCidResNameWeapon03[13] = "weapon03.rsd";
// GLOBAL: FF7 0x0082e148
unsigned char g_BattleThrowCidResNameWeapon04[13] = "weapon04.rsd";
// GLOBAL: FF7 0x0082e158
unsigned char g_BattleThrowCidResNameWeapon05[13] = "weapon05.rsd";
// GLOBAL: FF7 0x0082e168
unsigned char g_BattleThrowCidResNameWeapon06[13] = "weapon06.rsd";
// GLOBAL: FF7 0x0082e178
unsigned char g_BattleThrowCidResNameWeapon07[13] = "weapon07.rsd";
// GLOBAL: FF7 0x0082e188
unsigned char g_BattleThrowCidResNameWeapon08[13] = "weapon08.rsd";
// GLOBAL: FF7 0x0082e198
unsigned char g_BattleThrowCidResNameWeapon09[13] = "weapon09.rsd";
// GLOBAL: FF7 0x0082e1a8
unsigned char g_BattleThrowCidResNameWeapon10[13] = "weapon10.rsd";
// GLOBAL: FF7 0x0082e1b8
unsigned char g_BattleThrowCidResNameWeapon11[13] = "weapon11.rsd";
// GLOBAL: FF7 0x0082e1c8
unsigned char g_BattleThrowCidResNameWeapon12[13] = "weapon12.rsd";
// GLOBAL: FF7 0x0082e1d8
unsigned char g_BattleThrowCidResNameWeapon13[13] = "weapon13.rsd";
// GLOBAL: FF7 0x0082e1e8
unsigned char g_BattleThrowCidResNameWeapon14[13] = "weapon14.rsd";
// GLOBAL: FF7 0x0082e1f8
unsigned char g_BattleThrowCidResNameWeapon01_B[13] = "weapon01.rsd";
// GLOBAL: FF7 0x0082e208
unsigned char g_BattleThrowCidResNameWeapon02_B[13] = "weapon02.rsd";
// GLOBAL: FF7 0x0082e218
unsigned char g_BattleThrowCidResNameWeapon03_B[13] = "weapon03.rsd";
// GLOBAL: FF7 0x0082e228
unsigned char g_BattleThrowCidResNameWeapon04_B[13] = "weapon04.rsd";
// GLOBAL: FF7 0x0082e238
unsigned char g_BattleThrowCidResNameWeapon05_B[13] = "weapon05.rsd";
// GLOBAL: FF7 0x0082e248
unsigned char g_BattleThrowCidResNameWeapon06_B[13] = "weapon06.rsd";
// GLOBAL: FF7 0x0082e258
unsigned char g_BattleThrowCidResNameWeapon07_B[13] = "weapon07.rsd";
// GLOBAL: FF7 0x0082e268
unsigned char g_BattleThrowCidResNameWeapon08_B[13] = "weapon08.rsd";
// GLOBAL: FF7 0x0082e278
unsigned char g_BattleThrowCidResNameWeapon09_B[13] = "weapon09.rsd";
// GLOBAL: FF7 0x0082e288
unsigned char g_BattleThrowCidResNameWeapon10_B[13] = "weapon10.rsd";
// GLOBAL: FF7 0x0082e298
unsigned char g_BattleThrowCidResNameWeapon11_B[13] = "weapon11.rsd";
// GLOBAL: FF7 0x0082e2a8
unsigned char g_BattleThrowCidResNameWeapon12_B[13] = "weapon12.rsd";
// GLOBAL: FF7 0x0082e2b8
unsigned char g_BattleThrowCidResNameWeapon13_B[13] = "weapon13.rsd";
// GLOBAL: FF7 0x0082e2c8
unsigned char g_BattleThrowCidResNameWeapon14_B[13] = "weapon14.rsd";
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
// GLOBAL: FF7 0x00b37388
int g_BattleThrowCidFirstTargetIdx[2] = {0}; /* the second word is unused */
// GLOBAL: FF7 0x00b37390
BattleThrowCidRibbonParams g_BattleThrowCidTrailRibbonParams[2] = {0};
// GLOBAL: FF7 0x00b373f8
int g_BattleThrowCidDirX = 0;
// GLOBAL: FF7 0x00b373fc
int g_BattleThrowCidDirY = 0;
// GLOBAL: FF7 0x00b37400
int g_BattleThrowCidDirZ = 0;
unsigned char g_bssPad_00b37404[4] = {0};
// One ring of trail points for each of the two thrown weapons.
// GLOBAL: FF7 0x00b37408
BattleFxCidTrailSource g_BattleThrowCidTrailRings[2] = {0};
// GLOBAL: FF7 0x00b374e0
short g_BattleThrowCidActorPos[4] = {0};
// Storage of the trail points: two rings of 24 BattleFxThrowTrailPoint, continued by the two pads below.
// GLOBAL: FF7 0x00b374e8
BattleFxThrowTrailPoint g_BattleThrowCidTrailPoints[2][24] = {0};
// GLOBAL: FF7 0x00b37668
int g_BattleThrowCidFlightKind = 0;
// GLOBAL: FF7 0x00b3766c
int g_BattleThrowCidModelIndex = 0;
// GLOBAL: FF7 0x00b37670
short g_BattleThrowCidTargetBonePos[3] = {0};
// GLOBAL: FF7 0x00b37678
int g_BattleThrowCidTargetMask = 0;
unsigned char g_bssPad_00b3767c[4] = {0};
// GLOBAL: FF7 0x00b37680
int g_BattleThrowCidPrevPosXY = 0;
// GLOBAL: FF7 0x00b37684
int g_BattleThrowCidPrevPosZ = 0;
// GLOBAL: FF7 0x00b37688
int g_BattleThrowCidActorIndex = 0;
unsigned char g_bssPad_00b3768c[4] = {0};
// GLOBAL: FF7 0x00b37690
struct Int64Pair g_BattleThrowCidHeadPos = {0};
// GLOBAL: FF7 0x00b37698
VECTOR g_BattleThrowCidRotatedAxis = {0};
// GLOBAL: FF7 0x00b376a8
struct GfxTableCursor *g_BattleThrowCidLoadedResTable = 0;
unsigned char g_bssPad_00b376ac[4] = {0};
// clang-format on

/* Loads Cid spear models and throw effect resources. */
// FUNCTION: FF7 0x004cfeb0
void __cdecl BattleFx_ThrowLoadResCidWeapons(void)
{
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest requestBuffer;
    GfxModelTableBank *loadParam;
    GfxResourceRequest *request;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleThrowCidLoadedResTable == 0) {
        typeId = 0x1C;
        def = (int)&g_BattleThrowCidResourceDef;
        loadParam = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082e2d8
        request->resourcePath = BattleFx_BuildResourcePath("item/wp_si", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, def, request, &g_BattleThrowCidLoadedResTable, loadParam);
    }
}

/* Throws Cid weapon 73 (Spear). */
// FUNCTION: FF7 0x004cff40
void __cdecl Battle_ThrowFxWeaponBlock73(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 0;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 74 (Slash Lance). */
// FUNCTION: FF7 0x004cff69
void __cdecl Battle_ThrowFxWeaponBlock73Index1(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 1;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 75 (Trident). */
// FUNCTION: FF7 0x004cff92
void __cdecl Battle_ThrowFxWeaponBlock73Index2(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 2;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 76 (Mast Ax). */
// FUNCTION: FF7 0x004cffbb
void __cdecl Battle_ThrowFxWeaponBlock73Index3(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 3;
    g_BattleThrowCidFlightKind = 1;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 77 (Partisan). */
// FUNCTION: FF7 0x004cffe4
void __cdecl Battle_ThrowFxWeaponBlock73Index4(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 4;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 78 (Viper Halberd). */
// FUNCTION: FF7 0x004d000d
void __cdecl Battle_ThrowFxWeaponBlock73Index5(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 5;
    g_BattleThrowCidFlightKind = 1;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 79 (Javelin). */
// FUNCTION: FF7 0x004d0036
void __cdecl Battle_ThrowFxWeaponBlock73Index6(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 6;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 80 (Grow Lance). */
// FUNCTION: FF7 0x004d005f
void __cdecl Battle_ThrowFxWeaponBlock73Index7(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 7;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 81 (Mop). */
// FUNCTION: FF7 0x004d0088
void __cdecl Battle_ThrowFxWeaponBlock73Index8(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 8;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 82 (Dragoon Lance). */
// FUNCTION: FF7 0x004d00b1
void __cdecl Battle_ThrowFxWeaponBlock73Index9(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 9;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 83 (Scimitar). */
// FUNCTION: FF7 0x004d00da
void __cdecl Battle_ThrowFxWeaponBlock73Index10(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 0xA;
    g_BattleThrowCidFlightKind = 1;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 84 (Flayer). */
// FUNCTION: FF7 0x004d0103
void __cdecl Battle_ThrowFxWeaponBlock73Index11(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 0xB;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 85 (Spirit Lance). */
// FUNCTION: FF7 0x004d012c
void __cdecl Battle_ThrowFxWeaponBlock73Index12(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 0xC;
    g_BattleThrowCidFlightKind = 2;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Throws Cid weapon 86 (Venus Gospel). */
// FUNCTION: FF7 0x004d0155
void __cdecl Battle_ThrowFxWeaponBlock73Index13(int targetMask, int actorIdx)
{
    g_BattleThrowCidModelIndex = 0xD;
    g_BattleThrowCidFlightKind = 0;
    Battle_ThrowFxSpawnWeapons73to86(targetMask, actorIdx);
}

/* Schedules draw setup and timeline dispatch task for thrown Cid weapon. */
// FUNCTION: FF7 0x004d017e
void __cdecl Battle_ThrowFxSpawnWeapons73to86(int targetMask, int actorIdx)
{
    int bitIdx;
    int targetCount;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_ThrowLoadResCidWeapons();
    g_BattleThrowCidTargetMask = targetMask;
    bitIdx = 0;
    for (targetCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            targetCount++;
            if (targetCount == 1) {
                g_BattleThrowCidFirstTargetIdx[0] = bitIdx;
            }
        }
    }
    g_BattleThrowCidActorIndex = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_ThrowCidWeaponsDispatchKindTask);
}

/* Determines weapon throw flight category (Kind 0..3) and schedules timeline task. */
// FUNCTION: FF7 0x004d021e
void __cdecl BattleFx_ThrowCidWeaponsDispatchKindTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxYuffieWeaponTimeline2 *timeline; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR angleX; };
    } frame;
#pragma pack(pop)

    frame.timeline = (BattleFxYuffieWeaponTimeline2 *)BATTLE_CURRENT_TASK_SLOT;
    frame.timeline->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_BattleThrowCidFirstTargetIdx[0], g_BattleActorRecords[g_BattleThrowCidFirstTargetIdx[0]].targetBone, &g_BattleThrowCidTargetBonePos[0]);
    Battle_GetBoneWorldPos(g_BattleThrowCidActorIndex, g_BattleActorRecords[g_BattleThrowCidActorIndex].targetBone, &g_BattleThrowCidActorPos[0]);
    g_BattleThrowCidActorPos[3] = g_BattleActorRecords[g_BattleThrowCidActorIndex].yaw;
    frame.angleX.vx = 0;
    frame.angleX.vy = g_BattleThrowCidActorPos[3];
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    TransformVectorToInt32(&frame.rotMatrix, g_BattleThrowCidVectorOffset, &g_BattleThrowCidRotatedAxis);
    switch (g_BattleThrowCidFlightKind) {
    case BATTLE_CID_THROW_STRAIGHT:
        frame.timeline = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCidWeaponsKind0TimelineTask);
        break;
    case BATTLE_CID_THROW_HIGH_JUMP:
        frame.timeline = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCidWeaponsKind1TimelineTask);
        break;
    case BATTLE_CID_THROW_SPINNING:
        frame.timeline = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCidWeaponsKind2TimelineTask);
    }
    frame.timeline->subCounter = 0;
    BattleFx_ThrowCidWeaponsClearTrailRings();
}

/* Clears trail ring vertex buffers before launching trail tasks. */
// FUNCTION: FF7 0x004d036a
void __cdecl BattleFx_ThrowCidWeaponsClearTrailRings(void)
{
    int ringIndex;
    int padLocal; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int pointIndex;

    for (ringIndex = 0; ringIndex < 2; ringIndex++) {
        g_BattleThrowCidTrailRings[ringIndex].count = 0;
        g_BattleThrowCidTrailRings[ringIndex].tail = 0;
        g_BattleThrowCidTrailRings[ringIndex].head = 0;
        for (pointIndex = 0; pointIndex < 24; pointIndex++) {
            g_BattleThrowCidTrailRings[ringIndex].points[pointIndex] = 0;
        }
    }
}

/* Timeline task for Kind 0 straight flight trajectory. */
// FUNCTION: FF7 0x004d03ec
void __cdecl BattleFx_ThrowCidWeaponsKind0TimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxCidKind0ThrownWeapon *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowCidDirX = g_BattleThrowCidTargetBonePos[0] - g_BattleThrowCidActorPos[0];
        g_BattleThrowCidDirY = g_BattleThrowCidTargetBonePos[1] - g_BattleThrowCidActorPos[1];
        g_BattleThrowCidDirZ = g_BattleThrowCidTargetBonePos[2] - g_BattleThrowCidActorPos[2];
        norm = NormalizeVectorFixed(&g_BattleThrowCidDirX, &g_BattleThrowCidDirX);
        speed = SqrtToInt(norm) / 10;
        pProjectile = (BattleFxCidKind0ThrownWeapon *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCidWeaponsKind0ThrownModelTask);
        *(Int64Pair *)&pProjectile->posX = *(Int64Pair *)g_BattleThrowCidActorPos;
        pProjectile->offsetX = speed * g_BattleThrowCidDirX >> 12;
        pProjectile->offsetY = speed * g_BattleThrowCidDirY >> 12;
        pProjectile->offsetZ = speed * g_BattleThrowCidDirZ >> 12;
        pProjectile->angleVel = 0xCC;
        pProjectile->angle = 0xCC;
        ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->x = pProjectile->posX - pProjectile->offsetX;
        ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->y = pProjectile->posY - pProjectile->offsetY;
        ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->z = pProjectile->posZ - pProjectile->offsetZ;
    }
    if (task->frameCounter == 11) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowCidFirstTargetIdx[0]);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowCidTargetBonePos[0]), 0x156);
    }
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates and renders 3D mesh for Kind 0 straight thrown weapon. */
// FUNCTION: FF7 0x004d05b6
void __cdecl BattleFx_ThrowCidWeaponsKind0ThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        int rotMatrix;
        struct { unsigned char pad_to_12[0x12]; int posX; };
        struct { unsigned char pad_to_16[0x16]; int posY; };
        struct { unsigned char pad_to_1A[0x1A]; int posZ; };
        struct { unsigned char pad_to_20[0x20]; BattleFxThrownModel *thrownModel; };
        struct { unsigned char pad_to_24[0x24]; VECTOR dirX; };
        struct { unsigned char pad_to_34[0x34]; int sinOffset; };
        struct { unsigned char pad_to_38[0x38]; int stepFactor; };
        struct { unsigned char pad_to_3C[0x3C]; int axisAngle; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR angleX; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_68[0x68]; VECTOR axisVector; };
        struct { unsigned char pad_to_78[0x78]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.thrownModel = (BattleFxThrownModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.axisAngle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleThrowCidRotatedAxis, (VECTOR *)&g_BattleThrowCidDirX, &frame.axisVector);
    frame.stepFactor = frame.axisAngle / 190 + 1;
    frame.sinOffset = (SinFixed(frame.thrownModel->angle) * -0x12C >> 12) * frame.stepFactor;
    frame.angleX.vx = 0;
    frame.angleX.vy = 0;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.posX = frame.thrownModel->posA;
    frame.posY = frame.thrownModel->posB + frame.sinOffset;
    frame.posZ = frame.thrownModel->posC;
    frame.dirX.vx = frame.posX - ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->x;
    frame.dirX.vy = frame.posY - ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->y;
    frame.dirX.vz = frame.posZ - *(short *)&g_BattleThrowCidPrevPosZ;
    NormalizeVectorFixed(&frame.dirX, &frame.dirX);
    frame.axisAngle = ComputeAngleBetweenVectors((VECTOR *)g_BattleThrowCidVectorOffset, &frame.dirX, &frame.axisVector);
    Gte_BuildAxisAngleMatrix(frame.axisAngle, &frame.axisMatrix, &frame.axisVector);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
    if (g_BattleIsPaused == 0 && frame.thrownModel->frame < 10) {
        ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->x = *(short *)&frame.posX;
        ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->y = *(short *)&frame.posY;
        *(short *)&g_BattleThrowCidPrevPosZ = *(short *)&frame.posZ;
    }
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.dirX.vx = 0;
    frame.dirX.vy = 0;
    frame.dirX.vz = 420;
    TransformVectorToInt32((PsxMatrix *)&frame.rotMatrix, &frame.dirX, &frame.dirX);
    frame.posX += frame.dirX.vx;
    frame.posY += frame.dirX.vy;
    frame.posZ += frame.dirX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCidModelIndex, g_BattleThrowCidLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.thrownModel->frame >= 22) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCidModelIndex + 0xE, g_BattleThrowCidLoadedResTable);
        frame.modelDraw->fadeLevel = frame.thrownModel->frame - 22 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.thrownModel->frame >= 30) {
        frame.thrownModel->endFlag = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.thrownModel->frame < 10) {
        frame.thrownModel->posA += frame.thrownModel->velA;
        frame.thrownModel->posB += frame.thrownModel->velB;
        frame.thrownModel->posC += frame.thrownModel->velC;
        frame.thrownModel->angle += frame.thrownModel->angleVel;
    }
    (frame.thrownModel->frame)++;
}

/* Timeline task for Kind 1 curved flight trajectory. */
// FUNCTION: FF7 0x004d08e6
void __cdecl BattleFx_ThrowCidWeaponsKind1TimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxCidThrowState *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowCidDirX = g_BattleThrowCidTargetBonePos[0] - g_BattleThrowCidActorPos[0];
        g_BattleThrowCidDirY = g_BattleThrowCidTargetBonePos[1] - g_BattleThrowCidActorPos[1];
        g_BattleThrowCidDirZ = g_BattleThrowCidTargetBonePos[2] - g_BattleThrowCidActorPos[2];
        norm = NormalizeVectorFixed(&g_BattleThrowCidDirX, &g_BattleThrowCidDirX);
        speed = SqrtToInt(norm) / 5;
        pProjectile = (BattleFxCidThrowState *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCidWeaponsKind1ThrownModelTask);
        *(Int64Pair *)&pProjectile->posX = *(Int64Pair *)g_BattleThrowCidActorPos;
        pProjectile->targetScaled = speed * g_BattleThrowCidDirX >> 12;
        pProjectile->deltaX = speed * g_BattleThrowCidDirY >> 12;
        pProjectile->deltaY = speed * g_BattleThrowCidDirZ >> 12;
    }
    if (task->frameCounter == 5) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowCidFirstTargetIdx[0]);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowCidTargetBonePos[0]), 0x156);
    }
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Animates and renders 3D mesh for Kind 1 curved thrown weapon. */
// FUNCTION: FF7 0x004d0a5a
void __cdecl BattleFx_ThrowCidWeaponsKind1ThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxSlowParticle *cidModel; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int axisAngle; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR angleX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axisVector; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.cidModel = (BattleFxSlowParticle *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vx = 0;
    frame.angleX.vy = 0;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.cidModel->posX;
    frame.rotMatrix.t[1] = frame.cidModel->posY;
    frame.rotMatrix.t[2] = frame.cidModel->posZ;
    frame.axisAngle = ComputeAngleBetweenVectors((VECTOR *)g_BattleThrowCidVectorOffset, (VECTOR *)&g_BattleThrowCidDirX, &frame.axisVector);
    Gte_BuildAxisAngleMatrix(frame.axisAngle, &frame.axisMatrix, &frame.axisVector);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = 420;
    TransformVectorToInt32(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
    frame.rotMatrix.t[0] += frame.offsetX.vx;
    frame.rotMatrix.t[1] += frame.offsetX.vy;
    frame.rotMatrix.t[2] += frame.offsetX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCidModelIndex, g_BattleThrowCidLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.cidModel->timer >= 22) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCidModelIndex + 0xE, g_BattleThrowCidLoadedResTable);
        frame.modelDraw->fadeLevel = frame.cidModel->timer - 22 << 9;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.cidModel->timer >= 30) {
        frame.cidModel->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.cidModel->timer < 5) {
        frame.cidModel->posX += frame.cidModel->velocityX;
        frame.cidModel->posY += frame.cidModel->velocityY;
        frame.cidModel->posZ += frame.cidModel->velocityZ;
    }
    (frame.cidModel->timer)++;
}

/* Timeline task for Kind 2 looping flight trajectory with ribbon trail. */
// FUNCTION: FF7 0x004d0cb5
void __cdecl BattleFx_ThrowCidWeaponsKind2TimelineTask(void)
{
    int speed;
    int norm;
    int unusedSpace[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxCidKind2ThrownWeapon *pProjectile;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        g_BattleThrowCidDirX = g_BattleThrowCidTargetBonePos[0] - g_BattleThrowCidActorPos[0];
        g_BattleThrowCidDirY = g_BattleThrowCidTargetBonePos[1] - g_BattleThrowCidActorPos[1];
        g_BattleThrowCidDirZ = g_BattleThrowCidTargetBonePos[2] - g_BattleThrowCidActorPos[2];
        norm = NormalizeVectorFixed(&g_BattleThrowCidDirX, &g_BattleThrowCidDirX);
        speed = SqrtToInt(norm) * 3 / 7;
        pProjectile = (BattleFxCidKind2ThrownWeapon *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCidWeaponsKind2ThrownModelTask);
        *(Int64Pair *)&pProjectile->posX = *(Int64Pair *)g_BattleThrowCidActorPos;
        pProjectile->offsetX = speed * g_BattleThrowCidDirX >> 12;
        pProjectile->offsetY = speed * g_BattleThrowCidDirY >> 12;
        pProjectile->offsetZ = speed * g_BattleThrowCidDirZ >> 12;
        *(Int64Pair *)&g_BattleThrowCidHeadPos = *(Int64Pair *)&pProjectile->posX;
        ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->x = pProjectile->posX - pProjectile->offsetX;
        ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->y = pProjectile->posY - pProjectile->offsetY;
        ((BattleFxCidThrowStep *)&g_BattleThrowCidPrevPosXY)->z = pProjectile->posZ - pProjectile->offsetZ;
        speed = 0;
        pProjectile = (BattleFxCidKind2ThrownWeapon *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCidWeaponsKind2TrailRibbonTask);
        pProjectile->trailIndex = speed;
        g_BattleThrowCidTrailRibbonParams[speed].colorR = 0x80;
        g_BattleThrowCidTrailRibbonParams[speed].colorG = 0x80;
        g_BattleThrowCidTrailRibbonParams[speed].colorB = 0x80;
        g_BattleThrowCidTrailRibbonParams[speed].baseColorR = 4;
        g_BattleThrowCidTrailRibbonParams[speed].baseColorG = 4;
        g_BattleThrowCidTrailRibbonParams[speed].baseColorB = 4;
        g_BattleThrowCidTrailRibbonParams[speed].startAngle = 0;
        g_BattleThrowCidTrailRibbonParams[speed].anglePerFrame = 0xFFFFFED4;
        g_BattleThrowCidTrailRibbonParams[speed].anglePerSegment = 0x60;
        g_BattleThrowCidTrailRibbonParams[speed].localYStart = 0;
        g_BattleThrowCidTrailRibbonParams[speed].localYStep = 0xFFFFFFE0;
        g_BattleThrowCidTrailRibbonParams[speed].halfSpreadStart = 8;
        g_BattleThrowCidTrailRibbonParams[speed].halfSpreadStep = 8;
        g_BattleThrowCidTrailRibbonParams[speed].localZ = 0xFFFFFE70;
        speed = 1;
        pProjectile = (BattleFxCidKind2ThrownWeapon *)BATTLE_NEW_TASK_SLOT(BattleFx_ThrowCidWeaponsKind2TrailRibbonTask);
        pProjectile->trailIndex = speed;
        g_BattleThrowCidTrailRibbonParams[speed].colorR = 0x80;
        g_BattleThrowCidTrailRibbonParams[speed].colorG = 0x80;
        g_BattleThrowCidTrailRibbonParams[speed].colorB = 0x80;
        g_BattleThrowCidTrailRibbonParams[speed].baseColorR = 4;
        g_BattleThrowCidTrailRibbonParams[speed].baseColorG = 4;
        g_BattleThrowCidTrailRibbonParams[speed].baseColorB = 4;
        g_BattleThrowCidTrailRibbonParams[speed].startAngle = 0x800 /* 180 deg */;
        g_BattleThrowCidTrailRibbonParams[speed].anglePerFrame = 0xFFFFFED4;
        g_BattleThrowCidTrailRibbonParams[speed].anglePerSegment = 0x60;
        g_BattleThrowCidTrailRibbonParams[speed].localYStart = 0;
        g_BattleThrowCidTrailRibbonParams[speed].localYStep = 0xFFFFFFE0;
        g_BattleThrowCidTrailRibbonParams[speed].halfSpreadStart = 8;
        g_BattleThrowCidTrailRibbonParams[speed].halfSpreadStep = 8;
        g_BattleThrowCidTrailRibbonParams[speed].localZ = 0xFFFFFE70;
    }
    if (task->frameCounter == 6) {
        Battle_PostActorHitOrHurtMessage(g_BattleThrowCidFirstTargetIdx[0]);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleThrowCidTargetBonePos[0]), 0x156);
    }
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/* Updates ribbon trail quad buffers for Kind 2 thrown weapon. */
// FUNCTION: FF7 0x004d107b
void __cdecl BattleFx_ThrowCidWeaponsKind2TrailRibbonTask(void)
{
    BattleThrowCidRibbonParams *paramsPtr;
    BattleFxThrowCidTrailRibbon *pWork;
    int headIndex;
    int prevIdx;
    int i;
    BattleFxThrowTrailPoint *pointsPtr;
    int offsetZ;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxCidTrailRecord *selfPtr;
    int numberSegs;
    BattleFxCidTrailSource *src;
    SVECTOR *prevPoint;
    int angle;

    selfPtr = (BattleFxCidTrailRecord *)BATTLE_CURRENT_TASK_SLOT;
    pWork = (BattleFxThrowCidTrailRibbon *)Gfx_GetPsxScratchpadPtr(0);
    src = &g_BattleThrowCidTrailRings[selfPtr->weaponIndex];
    paramsPtr = &g_BattleThrowCidTrailRibbonParams[selfPtr->weaponIndex];
    pointsPtr = (BattleFxThrowTrailPoint *)(selfPtr->weaponIndex * 0xC0 + (char *)&g_BattleThrowCidTrailPoints);
    if (g_BattleIsPaused == 0) {
        *(Int64Pair *)&pWork->aX = *(Int64Pair *)&g_BattleThrowCidHeadPos;
        *(Int64Pair *)&pWork->bX = *(Int64Pair *)&g_BattleThrowCidPrevPosXY;
        pWork->midX = pWork->aX + pWork->bX >> 1;
        pWork->midY = pWork->aY + pWork->bY >> 1;
        pWork->midZ = pWork->aZ + pWork->bZ >> 1;
        pointsPtr[src->head].posX = pWork->midX + pWork->bX >> 1;
        pointsPtr[src->head].posY = pWork->midY + pWork->bY >> 1;
        pointsPtr[src->head].posZ = pWork->midZ + pWork->bZ >> 1;
        BattleFx_ThrowCidWeaponsPushTrailPoint(src, &pointsPtr[src->head]);
        *(Int64Pair *)&pointsPtr[src->head] = *(Int64Pair *)&pWork->midX;
        BattleFx_ThrowCidWeaponsPushTrailPoint(src, &pointsPtr[src->head]);
        pointsPtr[src->head].posX = pWork->midX + pWork->aX >> 1;
        pointsPtr[src->head].posY = pWork->midY + pWork->aY >> 1;
        pointsPtr[src->head].posZ = pWork->midZ + pWork->aZ >> 1;
        BattleFx_ThrowCidWeaponsPushTrailPoint(src, &pointsPtr[src->head]);
        *(Int64Pair *)&pointsPtr[src->head] = *(Int64Pair *)&pWork->aX;
        BattleFx_ThrowCidWeaponsPushTrailPoint(src, &pointsPtr[src->head]);
    }
    headIndex = ComputeAngleBetweenVectors((VECTOR *)g_BattleThrowCidVectorOffset, (VECTOR *)&g_BattleThrowCidDirX, &pWork->axisVector);
    Gte_BuildAxisAngleMatrix(headIndex, &pWork->axisMatrix, &pWork->axisVector);
    TransformVectorToInt32(&pWork->axisMatrix, g_BattleThrowCidVectorOffset, &pWork->driftX);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    SetDrawColorRGB(paramsPtr->colorR, paramsPtr->colorG, paramsPtr->colorB);
    pWork->baseValue = paramsPtr->baseColor;
    pWork->unshadedColor = pWork->baseValue;
    angle = paramsPtr->startAngle + selfPtr->frameCount * paramsPtr->anglePerFrame;
    pWork->packedPos = paramsPtr->localYStart;
    pWork->offset = paramsPtr->halfSpreadStart;
    offsetZ = paramsPtr->localZ;
    numberSegs = src->count - 1;
    headIndex = src->head - 1;
    if (headIndex < 0) {
        headIndex = 23;
    }
    *(Int64Pair *)&pWork->posX = *(Int64Pair *)src->points[headIndex];
    pWork->posX += (short)(-offsetZ * pWork->driftX >> 12);
    pWork->posY += (short)(-offsetZ * pWork->driftY >> 12);
    pWork->posZ += (short)(-offsetZ * pWork->driftZ >> 12);
    BattleFx_ThrowCidWeaponsProjectSegmentPoints(pWork, (SVECTOR *)&pWork->posX, (SVECTOR *)&pWork->posX);
    for (i = 1; i <= numberSegs; headIndex--, i++) {
        if (headIndex < 0) {
            headIndex = 23;
        }
        prevIdx = headIndex - 1;
        if (prevIdx < 0) {
            prevIdx = 23;
        }
        angle += paramsPtr->anglePerSegment;
        pWork->posX = 0;
        pWork->posY = *(short *)&pWork->packedPos;
        pWork->posZ = offsetZ;
        pWork->edgeCoord = angle - pWork->offset;
        RotMatrix((SVECTOR *)&pWork->rotX, (int *)pWork->edgeMatrix);
        TransformVectorToInt16(pWork->edgeMatrix, &pWork->posX, &pWork->accX);
        TransformVectorToInt16(&pWork->axisMatrix, &pWork->accX, &pWork->accX);
        pWork->edgeCoord = angle + pWork->offset;
        RotMatrix((SVECTOR *)&pWork->rotX, (int *)pWork->edgeMatrix);
        TransformVectorToInt16(pWork->edgeMatrix, &pWork->posX, &pWork->posX);
        TransformVectorToInt16(&pWork->axisMatrix, &pWork->posX, &pWork->posX);
        pWork->packedPos += paramsPtr->localYStep;
        pWork->offset += paramsPtr->halfSpreadStep;
        prevPoint = (SVECTOR *)src->points[prevIdx];
        pWork->posX += prevPoint->vx;
        pWork->posY += prevPoint->vy;
        pWork->posZ += prevPoint->vz;
        pWork->accX += prevPoint->vx;
        pWork->accY += prevPoint->vy;
        pWork->accZ += prevPoint->vz;
        pWork->vertex2 = pWork->vertex0;
        pWork->vertex3 = pWork->vertex1;
        pWork->prevDepth = pWork->depth;
        pWork->prevColor = pWork->baseValue;
        SetRotMatrix(&g_BattleCameraTransform);
        SetTransMatrix(&g_BattleCameraTransform);
        RefreshGlobalTransformState();
        BattleFx_ThrowCidWeaponsProjectSegmentPoints(pWork, (SVECTOR *)&pWork->posX, (SVECTOR *)&pWork->accX);
        BattleFx_ThrowCidWeaponsDrawGouraudQuad(pWork, numberSegs - i, numberSegs);
    }
    if (selfPtr->frameCount >= 30) {
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (selfPtr->frameCount > 12) {
        src->count -= 2;
    }
    if (src->count < 2) {
        selfPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (selfPtr->frameCount)++;
}

/* Emits shaded Gouraud quad primitive for weapon flight ribbon trail. */
// FUNCTION: FF7 0x004d172a
void __cdecl BattleFx_ThrowCidWeaponsDrawGouraudQuad(BattleFxThrowCidTrailRibbon *quadRecord, int factor, int divisor)
{
    int baseColor;
    BattleFxPsxGouraudPrim *quadPrimPtr;
    int colorTail;
    int otDepth;
    int unusedEntry; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int shade;

    shade = 0x1000 / divisor * factor;
    Gfx_ShadePsxVertexColor((CVECTOR *)&quadRecord->unshadedColor, shade, (CVECTOR *)&quadRecord->baseValue);
    baseColor = quadRecord->baseValue;
    colorTail = quadRecord->prevColor;
    quadPrimPtr = (BattleFxPsxGouraudPrim *)g_BattleFxActiveDrawListPtr;
    otDepth = quadRecord->prevDepth + quadRecord->depth >> 1 >> 2;
    if (otDepth > 0) {
        quadPrimPtr->vertex0 = quadRecord->vertex0;
        quadPrimPtr->vertex1 = quadRecord->vertex1;
        quadPrimPtr->vertex2 = quadRecord->vertex2;
        quadPrimPtr->vertex3 = quadRecord->vertex3;
        quadPrimPtr->color0 = baseColor;
        quadPrimPtr->color1 = baseColor;
        quadPrimPtr->color2 = colorTail;
        quadPrimPtr->color3 = colorTail;
        quadPrimPtr->tag = quadPrimPtr->tag & 0xFFFFFF | 0x8000000;
        quadPrimPtr->code = 0x3A;
        Gfx_DrawPsxPrimitive(quadPrimPtr);
    }
    g_BattleFxActiveDrawListPtr = (int)quadPrimPtr;
}

/* Transforms and projects ribbon trail segment vertices. */
// FUNCTION: FF7 0x004d1818
void __cdecl BattleFx_ThrowCidWeaponsProjectSegmentPoints(BattleFxThrowCidTrailRibbon *srcMesh, SVECTOR *outPoints, SVECTOR *transformState)
{
    srcMesh->depth = RotTransPers(outPoints, (short *)&srcMesh->vertex0, &srcMesh->projP, &srcMesh->projFlag);
    RotTransPers(transformState, (short *)&srcMesh->vertex1, &srcMesh->projP, &srcMesh->projFlag);
}

/* Pushes new coordinate sample into circular trail ribbon buffer. */
// FUNCTION: FF7 0x004d1865
void __cdecl BattleFx_ThrowCidWeaponsPushTrailPoint(BattleFxCidTrailSource *trailBuffer, BattleFxThrowTrailPoint *pNewPoint)
{
    int tail;
    int head;
    int count;

    head = trailBuffer->head;
    tail = trailBuffer->tail;
    count = trailBuffer->count;
    trailBuffer->points[head] = pNewPoint;
    head++;
    count++;
    if (count >= 24) {
        count = 24;
    }
    if (head >= 24) {
        head = 0;
    }
    if (head == tail) {
        tail++;
        count--;
        if (count < 0) {
            count = 0;
        }
        if (tail >= 24) {
            tail = 0;
        }
    }
    trailBuffer->head = head;
    trailBuffer->tail = tail;
    trailBuffer->count = count;
}

/* Animates and renders 3D mesh for Kind 2 thrown weapon. */
// FUNCTION: FF7 0x004d1910
void __cdecl BattleFx_ThrowCidWeaponsKind2ThrownModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxPulseOfLifeSprite *thrownWeapon; };
        struct { unsigned char pad_to_24[0x24]; VECTOR offsetX; };
        struct { unsigned char pad_to_34[0x34]; int axisAngle; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR angleX; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix axisMatrix; };
        struct { unsigned char pad_to_60[0x60]; VECTOR axisVector; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.thrownWeapon = (BattleFxPulseOfLifeSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vx = 0;
    frame.angleX.vy = 0;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.thrownWeapon->posX;
    frame.rotMatrix.t[1] = frame.thrownWeapon->posY;
    frame.rotMatrix.t[2] = frame.thrownWeapon->posZ;
    frame.axisAngle = ComputeAngleBetweenVectors((VECTOR *)g_BattleThrowCidVectorOffset, (VECTOR *)&g_BattleThrowCidDirX, &frame.axisVector);
    Gte_BuildAxisAngleMatrix(frame.axisAngle, &frame.axisMatrix, &frame.axisVector);
    ConcatMatrixInPlace(&frame.axisMatrix, &frame.rotMatrix);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = 420;
    TransformVectorToInt32(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
    frame.rotMatrix.t[0] += frame.offsetX.vx;
    frame.rotMatrix.t[1] += frame.offsetX.vy;
    frame.rotMatrix.t[2] += frame.offsetX.vz;
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCidModelIndex, g_BattleThrowCidLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    if (frame.thrownWeapon->frame >= 12) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleThrowCidModelIndex + 0xE, g_BattleThrowCidLoadedResTable);
        frame.modelDraw->fadeLevel = (frame.thrownWeapon->frame - 12) * 227;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.thrownWeapon->frame >= 30) {
        frame.thrownWeapon->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.thrownWeapon->posX += frame.thrownWeapon->velocityX;
    frame.thrownWeapon->posY += frame.thrownWeapon->velocityY;
    frame.thrownWeapon->posZ += frame.thrownWeapon->velocityZ;
    g_BattleThrowCidPrevPosXY = ((int *)&g_BattleThrowCidHeadPos)[0];
    g_BattleThrowCidPrevPosZ = ((int *)&g_BattleThrowCidHeadPos)[1];
    *(Int64Pair *)&g_BattleThrowCidHeadPos = *(Int64Pair *)&frame.thrownWeapon->posX;
    (frame.thrownWeapon->frame)++;
}
} /* extern "C" */

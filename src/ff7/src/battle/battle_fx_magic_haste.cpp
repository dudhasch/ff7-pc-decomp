// src/ff7/src/battle/battle_fx_magic_haste.cpp
/* Magic Effect [19]: Haste (Japanese: ヘイスト, dev tag: "MAGIC/HEIST" / Heist) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Heist magic: pairs of models orbit each target, and a mesh
//   morphs between poses by blending polygon vertices.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/enums/battle.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off

// GLOBAL: FF7 0x008b32b0
unsigned int g_BattleMagicHasteResourceDefs[10] = {0x001900b4, (unsigned int)&g_BattleMagicHasteModelFilenames, 0x00000000, 0x00000000, 0x00000000, 0x001900b4, (unsigned int)((char *)&g_BattleMagicHasteModelFilenames + 0xc), 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008b32d8
struct SVECTOR g_BattleMagicHasteMeshModel1[130] = {{0, 176, 25, 0}, {95, 176, 25, 0}, {176, 176, 25, 0}, {156, 126, 25, 0}, {125, 51, 25, 0}, {88, -36, 25, 0}, {51, -125, 25, 0}, {20, -199, 25, 0}, {0, -250, 25, 0}, {-20, -199, 25, 0}, {-51, -125, 25, 0}, {-88, -36, 25, 0}, {-125, 51, 25, 0}, {-156, 126, 25, 0}, {-176, 176, 25, 0}, {-95, 176, 25, 0}, {-80, 149, 52, 0}, {0, 149, 52, 0}, {80, 149, 52, 0}, {149, 149, 52, 0}, {131, 106, 52, 0}, {105, 43, 52, 0}, {74, -30, 52, 0}, {43, -105, 52, 0}, {17, -168, 52, 0}, {0, -210, 52, 0}, {-17, -168, 52, 0}, {-43, -105, 52, 0}, {-74, -30, 52, 0}, {-105, 43, 52, 0}, {-131, 106, 52, 0}, {-149, 149, 52, 0}, {21, -209, 8, 0}, {54, -129, 8, 0}, {92, -38, 8, 0}, {130, 54, 8, 0}, {162, 132, 8, 0}, {184, 184, 8, 0}, {99, 184, 8, 0}, {0, -260, 8, 0}, {-21, -208, 8, 0}, {-53, -130, 8, 0}, {-92, -37, 8, 0}, {-129, 52, 8, 0}, {-162, 132, 8, 0}, {-184, 184, 8, 0}, {-99, 184, 8, 0}, {0, 184, 8, 0}, {-68, 126, 61, 0}, {0, 126, 61, 0}, {68, 126, 61, 0}, {126, 126, 61, 0}, {111, 90, 61, 0}, {89, 36, 61, 0}, {63, -26, 61, 0}, {36, -89, 61, 0}, {14, -142, 61, 0}, {0, -178, 61, 0}, {-14, -142, 61, 0}, {-36, -89, 61, 0}, {-63, -26, 61, 0}, {-89, 36, 61, 0}, {-111, 90, 61, 0}, {-126, 126, 61, 0}, {-126, 126, -61, 0}, {-111, 90, -61, 0}, {-89, 36, -61, 0}, {-63, -26, -61, 0}, {-36, -89, -61, 0}, {-14, -142, -61, 0}, {0, -178, -61, 0}, {14, -142, -61, 0}, {36, -89, -61, 0}, {63, -26, -61, 0}, {89, 36, -61, 0}, {111, 90, -61, 0}, {126, 126, -61, 0}, {68, 126, -61, 0}, {0, 126, -61, 0}, {-68, 126, -61, 0}, {0, 184, -8, 0}, {-99, 184, -8, 0}, {-184, 184, -8, 0}, {-161, 132, -8, 0}, {-129, 53, -8, 0}, {-91, -36, -8, 0}, {-53, -130, -8, 0}, {-21, -207, -8, 0}, {1, -260, -8, 0}, {99, 184, -8, 0}, {184, 184, -8, 0}, {163, 131, -8, 0}, {131, 53, -8, 0}, {93, -38, -8, 0}, {54, -130, -8, 0}, {22, -208, -8, 0}, {-149, 149, -52, 0}, {-131, 106, -52, 0}, {-105, 43, -52, 0}, {-74, -30, -52, 0}, {-43, -105, -52, 0}, {-17, -168, -52, 0}, {0, -210, -52, 0}, {17, -168, -52, 0}, {43, -105, -52, 0}, {74, -30, -52, 0}, {105, 43, -52, 0}, {131, 106, -52, 0}, {149, 149, -52, 0}, {80, 149, -52, 0}, {0, 149, -52, 0}, {-80, 149, -52, 0}, {-95, 176, -25, 0}, {-176, 176, -25, 0}, {-156, 126, -25, 0}, {-125, 51, -25, 0}, {-88, -36, -25, 0}, {-51, -125, -25, 0}, {-20, -199, -25, 0}, {0, -250, -25, 0}, {20, -199, -25, 0}, {51, -125, -25, 0}, {88, -36, -25, 0}, {125, 51, -25, 0}, {156, 126, -25, 0}, {176, 176, -25, 0}, {95, 176, -25, 0}, {0, 176, -25, 0}, {0, 0, -61, 0}, {0, 0, 61, 0}};
// GLOBAL: FF7 0x008b36e8
struct SVECTOR g_BattleMagicHasteMeshModel2[130] = {{0, 230, 25, 0}, {47, 183, 25, 0}, {115, 115, 25, 0}, {183, 47, 25, 0}, {230, 0, 25, 0}, {183, -47, 25, 0}, {115, -115, 25, 0}, {47, -183, 25, 0}, {0, -230, 25, 0}, {-47, -183, 25, 0}, {-115, -115, 25, 0}, {-183, -47, 25, 0}, {-230, 0, 25, 0}, {-183, 47, 25, 0}, {-115, 115, 25, 0}, {-47, 183, 25, 0}, {-40, 154, 52, 0}, {0, 194, 52, 0}, {40, 154, 52, 0}, {97, 97, 52, 0}, {154, 40, 52, 0}, {194, 0, 52, 0}, {154, -40, 52, 0}, {97, -97, 52, 0}, {40, -154, 52, 0}, {0, -194, 52, 0}, {-40, -154, 52, 0}, {-97, -97, 52, 0}, {-154, -40, 52, 0}, {-194, 0, 52, 0}, {-154, 40, 52, 0}, {-97, 97, 52, 0}, {49, -191, 8, 0}, {121, -119, 8, 0}, {190, -49, 8, 0}, {240, 0, 8, 0}, {190, 49, 8, 0}, {120, 120, 8, 0}, {49, 190, 8, 0}, {0, -240, 8, 0}, {-49, -190, 8, 0}, {-120, -120, 8, 0}, {-191, -49, 8, 0}, {-240, 0, 8, 0}, {-190, 49, 8, 0}, {-119, 119, 8, 0}, {-49, 190, 8, 0}, {0, 240, 8, 0}, {-34, 130, 61, 0}, {0, 165, 61, 0}, {34, 130, 61, 0}, {82, 82, 61, 0}, {130, 34, 61, 0}, {165, 0, 61, 0}, {130, -34, 61, 0}, {82, -82, 61, 0}, {34, -130, 61, 0}, {0, -165, 61, 0}, {-34, -130, 61, 0}, {-82, -82, 61, 0}, {-130, -34, 61, 0}, {-165, 0, 61, 0}, {-130, 34, 61, 0}, {-82, 82, 61, 0}, {-82, 82, -61, 0}, {-130, 34, -61, 0}, {-165, 0, -61, 0}, {-130, -34, -61, 0}, {-82, -82, -61, 0}, {-34, -130, -61, 0}, {0, -165, -61, 0}, {34, -130, -61, 0}, {82, -82, -61, 0}, {130, -34, -61, 0}, {165, 0, -61, 0}, {130, 34, -61, 0}, {82, 82, -61, 0}, {34, 130, -61, 0}, {0, 165, -61, 0}, {-34, 130, -61, 0}, {0, 240, -8, 0}, {-49, 190, -8, 0}, {-119, 119, -8, 0}, {-190, 49, -8, 0}, {-240, 0, -8, 0}, {-191, -49, -8, 0}, {-119, -119, -8, 0}, {-49, -189, -8, 0}, {1, -240, -8, 0}, {49, 190, -8, 0}, {120, 120, -8, 0}, {190, 49, -8, 0}, {240, 0, -8, 0}, {191, -50, -8, 0}, {120, -120, -8, 0}, {50, -191, -8, 0}, {-97, 97, -52, 0}, {-154, 40, -52, 0}, {-194, 0, -52, 0}, {-154, -40, -52, 0}, {-97, -97, -52, 0}, {-40, -154, -52, 0}, {0, -194, -52, 0}, {40, -154, -52, 0}, {97, -97, -52, 0}, {154, -40, -52, 0}, {194, 0, -52, 0}, {154, 40, -52, 0}, {97, 97, -52, 0}, {40, 154, -52, 0}, {0, 194, -52, 0}, {-40, 154, -52, 0}, {-47, 183, -25, 0}, {-115, 115, -25, 0}, {-183, 47, -25, 0}, {-231, 0, -25, 0}, {-183, -47, -25, 0}, {-115, -115, -25, 0}, {-47, -183, -25, 0}, {0, -230, -25, 0}, {47, -183, -25, 0}, {115, -115, -25, 0}, {183, -47, -25, 0}, {230, 0, -25, 0}, {183, 47, -25, 0}, {115, 115, -25, 0}, {47, 183, -25, 0}, {0, 230, -25, 0}, {0, 0, -61, 0}, {0, 0, 61, 0}};
// GLOBAL: FF7 0x008b3af8
struct SVECTOR g_BattleMagicHasteMeshModel3[130] = {{0, 230, 25, 0}, {95, 230, 25, 0}, {163, 163, 25, 0}, {230, 95, 25, 0}, {230, 0, 25, 0}, {230, -95, 25, 0}, {163, -163, 25, 0}, {95, -230, 25, 0}, {0, -230, 25, 0}, {-95, -230, 25, 0}, {-163, -163, 25, 0}, {-230, -95, 25, 0}, {-230, 0, 25, 0}, {-230, 95, 25, 0}, {-163, 163, 25, 0}, {-95, 230, 25, 0}, {-80, 194, 52, 0}, {0, 194, 52, 0}, {80, 194, 52, 0}, {137, 137, 52, 0}, {194, 80, 52, 0}, {194, 0, 52, 0}, {194, -80, 52, 0}, {137, -137, 52, 0}, {80, -194, 52, 0}, {0, -194, 52, 0}, {-80, -194, 52, 0}, {-137, -137, 52, 0}, {-194, -80, 52, 0}, {-194, 0, 52, 0}, {-194, 80, 52, 0}, {-137, 137, 52, 0}, {98, -241, 8, 0}, {170, -169, 8, 0}, {240, -99, 8, 0}, {240, 0, 8, 0}, {240, 99, 8, 0}, {170, 170, 8, 0}, {99, 240, 8, 0}, {0, -240, 8, 0}, {-99, -240, 8, 0}, {-169, -169, 8, 0}, {-240, -98, 8, 0}, {-240, 0, 8, 0}, {-240, 99, 8, 0}, {-170, 170, 8, 0}, {-99, 240, 8, 0}, {0, 240, 8, 0}, {-68, 165, 61, 0}, {0, 165, 61, 0}, {68, 165, 61, 0}, {116, 116, 61, 0}, {165, 68, 61, 0}, {165, 0, 61, 0}, {165, -68, 61, 0}, {116, -116, 61, 0}, {68, -165, 61, 0}, {0, -165, 61, 0}, {-68, -165, 61, 0}, {-116, -116, 61, 0}, {-165, -68, 61, 0}, {-165, 0, 61, 0}, {-165, 68, 61, 0}, {-116, 116, 61, 0}, {-116, 116, -61, 0}, {-165, 68, -61, 0}, {-165, 0, -61, 0}, {-165, -68, -61, 0}, {-116, -116, -61, 0}, {-68, -165, -61, 0}, {0, -165, -61, 0}, {68, -165, -61, 0}, {116, -116, -61, 0}, {165, -68, -61, 0}, {165, 0, -61, 0}, {165, 68, -61, 0}, {116, 116, -61, 0}, {68, 165, -61, 0}, {0, 165, -61, 0}, {-68, 165, -61, 0}, {0, 240, -8, 0}, {-99, 240, -8, 0}, {-170, 170, -8, 0}, {-240, 99, -8, 0}, {-240, 0, -8, 0}, {-240, -98, -8, 0}, {-170, -170, -8, 0}, {-100, -240, -8, 0}, {1, -240, -8, 0}, {99, 240, -8, 0}, {170, 170, -8, 0}, {240, 99, -8, 0}, {240, 0, -8, 0}, {240, -99, -8, 0}, {170, -170, -8, 0}, {99, -240, -8, 0}, {-137, 137, -52, 0}, {-194, 80, -52, 0}, {-194, 0, -52, 0}, {-194, -80, -52, 0}, {-137, -137, -52, 0}, {-80, -194, -52, 0}, {0, -194, -52, 0}, {80, -194, -52, 0}, {137, -137, -52, 0}, {194, -80, -52, 0}, {194, 0, -52, 0}, {194, 80, -52, 0}, {137, 137, -52, 0}, {80, 194, -52, 0}, {0, 194, -52, 0}, {-80, 194, -52, 0}, {-95, 230, -25, 0}, {-163, 163, -25, 0}, {-230, 95, -25, 0}, {-230, 0, -25, 0}, {-230, -95, -25, 0}, {-163, -163, -25, 0}, {-95, -230, -25, 0}, {0, -230, -25, 0}, {95, -230, -25, 0}, {163, -163, -25, 0}, {230, -95, -25, 0}, {230, 0, -25, 0}, {230, 95, -25, 0}, {163, 163, -25, 0}, {95, 230, -25, 0}, {0, 230, -25, 0}, {0, 0, -61, 0}, {0, 0, 61, 0}};
// GLOBAL: FF7 0x008b3f08
unsigned char g_BattleMagicHasteModelFilenames[24] =
    "HARI.RSD\0\0\0\0"
    "HEIST_1.RSD";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bb1d68
int g_BattleMagicHasteVertexBuffer = 0;
// GLOBAL: FF7 0x00bb1d6c
int g_BattleMagicHasteVertexDeltaBuffer = 0;
// GLOBAL: FF7 0x00bb1d70
struct SVECTOR g_BattleMagicHasteTargetMeshPtr[130] = {0};
// GLOBAL: FF7 0x00bb2180
float g_BattleMagicHasteWeight1 = 0;
// GLOBAL: FF7 0x00bb2184
float g_BattleMagicHasteWeight2 = 0;
// GLOBAL: FF7 0x00bb2188
struct GfxTableCursor *g_BattleMagicHasteLoadedResTable = 0;

/*
 * Loads resources for the Heist magic effect.
 */
// FUNCTION: FF7 0x00587c90
void __cdecl BattleFx_MagicLoadResHaste(void)
{
    char pathOut[200];
    GfxModelTableBank *param;
    GfxResourceRequest requestStorage;
    int kind;
    GfxResourceRequest *requestPtr;
    int resultDefLoadList;

    if (g_pBattle3dScene != 0 && g_BattleMagicHasteLoadedResTable == 0) {
        kind = 2;
        resultDefLoadList = (int)&g_BattleMagicHasteResourceDefs;
        param = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008b3f20
        requestPtr->resourcePath = BattleFx_BuildResourcePath("magic/heist", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefLoadList, requestPtr, &g_BattleMagicHasteLoadedResTable, param);
    }
}

/*
 * Entry point for the Heist magic effect.
 */
// FUNCTION: FF7 0x00587d20
void __cdecl BattleFx_MagicEnterHaste(int targetMask, int actorIdx)
{
    int field;

    memset(g_BattleMagicHasteTargetMeshPtr, 0, 0x410);
    g_BattleFxActiveDrawListPtr = (int)Gfx_GetPsxPacketScratchPtr();
    BattleFx_MagicLoadResHaste();
    field = Gfx_ModelTableGetRenderObject(1, g_BattleMagicHasteLoadedResTable);
    if (field != 0) {
        g_BattleMagicHasteWeight2 = 0.0f;
        g_BattleMagicHasteWeight1 = 1.0f;
        g_BattleMagicHasteVertexBuffer = (int)&g_BattleMagicHasteMeshModel1;
        Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_MagicHasteBlendPolygonSetVertices, *(int *)((char *)field + 4));
    }
    BattleFx_MagicBeginHaste(targetMask, actorIdx);
}

/*
 * Blends polygon vertex coordinates with weighted interpolation.
 */
// FUNCTION: FF7 0x00587da2
void __cdecl BattleFx_MagicHasteBlendPolygonSetVertices(int vertexIndex, int param, BattleFxSlowBlendVertex *vertex)
{
    Vec3sPacked *srcVertex;

    srcVertex = (Vec3sPacked *)(g_BattleMagicHasteVertexBuffer + vertexIndex * 8);
    vertex->x = vertex->x * g_BattleMagicHasteWeight2 + srcVertex->vx * g_BattleMagicHasteWeight1;
    vertex->y = vertex->y * g_BattleMagicHasteWeight2 + srcVertex->vy * g_BattleMagicHasteWeight1;
    vertex->z = vertex->z * g_BattleMagicHasteWeight2 + srcVertex->vz * g_BattleMagicHasteWeight1;
}

/*
 * Initializes Heist effect and starts orbiting pair spawner task.
 */
// FUNCTION: FF7 0x00587e2d
void __cdecl BattleFx_MagicBeginHaste(int entity, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_MagicHasteSpawnOrbitingPairsTask);
    g_BattleMagicHasteVertexDeltaBuffer = entity;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(entity), 0x53);
}

/*
 * Spawns orbiting model pairs for Heist effect per target.
 */
// FUNCTION: FF7 0x00587e7e
void __cdecl BattleFx_MagicHasteSpawnOrbitingPairsTask(void)
{
    int targetCount8;
    int i;
    int tiltSeed;
    int firstTarget;
    int meshResult;
    BattleFxMagicHasteOrbitingPair *pOrbit;
    BattleFxOrbitingPair *pairPtr;
    int tilt;

    i = 0;
    for (targetCount8 = 0; i < 10; i++) {
        if ((g_BattleMagicHasteVertexDeltaBuffer & 1 << i) != 0) {
            targetCount8++;
            if (targetCount8 == 1) {
                firstTarget = i;
            }
        }
    }
    pOrbit = (BattleFxMagicHasteOrbitingPair *)BATTLE_CURRENT_TASK_SLOT;
    pOrbit->ownerSlot = BATTLE_TASK_FREE;
    pOrbit = (BattleFxMagicHasteOrbitingPair *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicHasteMorphModelVerticesTask);
    pOrbit = (BattleFxMagicHasteOrbitingPair *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicHasteOrbitingModelPairTask);
    if (targetCount8 == 1) {
        Battle_GetBoneWorldPos(firstTarget, g_BattleActorRecords[firstTarget].targetBone, (short *)((char *)pOrbit + 4));
        pOrbit->orbitRadius = g_BattleActorRecords[firstTarget].radius + 224;
        if (pOrbit->orbitRadius > 1900) {
            pOrbit->orbitRadius = 1900;
        }
    } else {
        Battle_GetActorMaskCentroid(g_BattleMagicHasteVertexDeltaBuffer, (short *)((char *)pOrbit + 4));
        pOrbit->posY = 0xFC80;
        pOrbit->orbitRadius = 1344;
    }
    pOrbit->rotX = 0;
    pOrbit->rotZ = 0;
    pOrbit->tiltYOffset = 0;
    pOrbit->rotYOffset = 0;
    pOrbit->orbitAngle = 0;
    pOrbit->angle = 0x400 /* 90 deg */;
    tiltSeed = 0x100;
    if (tiltSeed < 0x20) {
        tilt = 0x20;
    } else {
        tilt = tiltSeed;
    }
    pOrbit->tilt = tilt;
    pOrbit->state = 0;
    pairPtr = (BattleFxOrbitingPair *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicHasteOrbitingModelPairTask);
    *(Int64Pair *)((char *)pairPtr + 4) = *(Int64Pair *)((char *)pOrbit + 4);
    pairPtr->spinX = 0;
    pairPtr->spinZ = 0;
    pairPtr->orbitSpin = 0;
    pairPtr->spinY = 0;
    pairPtr->orbitAngle = 0x400 /* 90 deg */;
    pairPtr->orbitRadius = pOrbit->orbitRadius;
    pairPtr->angle = pOrbit->angle;
    pairPtr->tilt = pOrbit->tilt;
    pairPtr->state = 0;
    pairPtr = (BattleFxOrbitingPair *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicHasteOrbitingModelPairTask);
    *(Int64Pair *)((char *)pairPtr + 4) = *(Int64Pair *)((char *)pOrbit + 4);
    pairPtr->spinX = 0;
    pairPtr->spinZ = 0;
    pairPtr->orbitSpin = 0;
    pairPtr->spinY = 0;
    pairPtr->orbitAngle = 0x800 /* 180 deg */;
    pairPtr->orbitRadius = pOrbit->orbitRadius;
    pairPtr->angle = pOrbit->angle;
    pairPtr->tilt = pOrbit->tilt;
    pairPtr->state = 0;
    pairPtr = (BattleFxOrbitingPair *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicHasteOrbitingModelPairTask);
    *(Int64Pair *)((char *)pairPtr + 4) = *(Int64Pair *)((char *)pOrbit + 4);
    pairPtr->spinX = 0;
    pairPtr->spinZ = 0;
    pairPtr->orbitSpin = 0;
    pairPtr->spinY = 0;
    pairPtr->orbitAngle = 0xC00 /* 270 deg */;
    pairPtr->orbitRadius = pOrbit->orbitRadius;
    pairPtr->angle = pOrbit->angle;
    pairPtr->tilt = pOrbit->tilt;
    pairPtr->state = 0;
    meshResult = Gfx_ModelTableGetRenderObject(1, g_BattleMagicHasteLoadedResTable);
    if (meshResult != 0) {
        *(int *)&g_BattleMagicHasteWeight2 = 0;
        *(int *)&g_BattleMagicHasteWeight1 = FLOAT_ONE_BITS;
        g_BattleMagicHasteVertexBuffer = (int)&g_BattleMagicHasteMeshModel1;
        Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_MagicHasteBlendPolygonSetVertices, *(int *)((char *)meshResult + 4));
    }
}

/*
 * Updates and renders orbiting model pair for Heist.
 */
// FUNCTION: FF7 0x00588205
void __cdecl BattleFx_MagicHasteOrbitingModelPairTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[204];
        int stepCount;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; BattleFxOrbitingPairRecord *pairRecord; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; int blendDivisor; };
        struct { unsigned char pad_to_1C[0x1C]; VECTOR dirX; };
        struct { unsigned char pad_to_2C[0x2C]; int tmpAngle; };
        struct { unsigned char pad_to_30[0x30]; int rotMatrixB; };
        struct { unsigned char pad_to_42[0x42]; GfxDirectionVec3 goalX; };
        struct { unsigned char pad_to_50[0x50]; PsxMatrix workMatrix; };
        struct { unsigned char pad_to_70[0x70]; int rotMatrixA; };
        struct { unsigned char pad_to_82[0x82]; GfxDirectionVec3 posX; };
        struct { unsigned char pad_to_90[0x90]; VECTOR camDirX; };
        struct { unsigned char pad_to_A0[0xA0]; VECTOR tmpVec; };
        struct { unsigned char pad_to_B0[0xB0]; SVECTOR tiltX; };
        struct { unsigned char pad_to_B8[0xB8]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_BC[0xBC]; VECTOR scaleVec; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.pairRecord = (BattleFxOrbitingPairRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.pairRecord->spinX;
    frame.rotX.vy = frame.pairRecord->spinY + frame.pairRecord->orbitAngle + -0x400;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrixA);
    frame.rotX.vz = frame.pairRecord->spinZ;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrixB);
    frame.tiltX.vx = 0;
    frame.tiltX.vy = frame.pairRecord->orbitSpin + frame.pairRecord->orbitAngle;
    frame.tiltX.vz = 0;
    RotMatrixXYZ(&frame.tiltX, &frame.workMatrix);
    frame.offsetX.vx = frame.pairRecord->orbitRadius;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = 0;
    ApplyMatrixToSVectorInt32(&frame.workMatrix, &frame.offsetX, &frame.posX);
    frame.tmpAngle = frame.pairRecord->frame * 3;
    if (frame.tmpAngle > 0x3C) {
        frame.stepCount = 60;
    } else {
        frame.stepCount = frame.tmpAngle;
    }
    frame.offsetX.vx += *(short *)&frame.stepCount;
    ApplyMatrixToSVectorInt32(&frame.workMatrix, &frame.offsetX, &frame.goalX);
    frame.posX.x += frame.pairRecord->velocityX;
    frame.posX.y += frame.pairRecord->velocityY;
    frame.posX.z += frame.pairRecord->velocityZ;
    frame.goalX.x += frame.pairRecord->velocityX;
    frame.goalX.y += frame.pairRecord->velocityY;
    frame.goalX.z += frame.pairRecord->velocityZ;
    if (frame.pairRecord->state == 0 && frame.pairRecord->frame >= 20) {
        frame.dirX.vx = frame.pairRecord->velocityX - frame.posX.x;
        frame.dirX.vy = 0;
        frame.dirX.vz = frame.pairRecord->velocityZ - frame.posX.z;
        frame.camDirX.vx = g_BattleCameraFocalPoint.focalPointX - g_BattleCameraPosition.cameraPosX;
        frame.camDirX.vy = 0;
        frame.camDirX.vz = g_BattleCameraFocusZ - g_BattleCameraPosZ;
        NormalizeVectorFixed(&frame.dirX, &frame.dirX);
        NormalizeVectorFixed(&frame.camDirX, &frame.camDirX);
        frame.tmpAngle = ComputeAngleBetweenVectors(&frame.camDirX, &frame.dirX, &frame.tmpVec);
        if (frame.tmpAngle <= 0x80) {
            frame.pairRecord->state = 4;
        }
        if (frame.tmpAngle >= 0x780 && frame.tmpAngle <= 0x880) {
            frame.pairRecord->state = 2;
        }
        if (frame.tmpAngle >= 0x380 && frame.tmpAngle <= 0x480) {
            if (frame.tmpVec.vy > 0) {
                frame.pairRecord->state = 3;
            } else {
                frame.pairRecord->state = 1;
            }
        }
    }
    if (frame.pairRecord->state != 0) {
        switch (frame.pairRecord->state) {
        case BATTLE_HASTE_ORBIT_TILT_2:
            frame.tiltX.vx = 64;
            frame.tiltX.vy = 0xFFB0;
            frame.tiltX.vz = 0;
            frame.blendDivisor = 0x10;
            break;
        case BATTLE_HASTE_ORBIT_TILT_3:
            frame.tiltX.vx = 32;
            frame.tiltX.vy = 64;
            frame.tiltX.vz = 0;
            frame.blendDivisor = 0xC;
            break;
        case BATTLE_HASTE_ORBIT_TILT_4:
            frame.tiltX.vx = 0xFFC0;
            frame.tiltX.vy = 0xFFA0;
            frame.tiltX.vz = 0;
            frame.blendDivisor = 0xA;
            break;
        case BATTLE_HASTE_ORBIT_TILT_1:
            frame.tiltX.vx = 0xFFB0;
            frame.tiltX.vy = 96;
            frame.tiltX.vz = 0;
            frame.blendDivisor = 0x15;
        }
        RotMatrixXYZ(&frame.tiltX, &frame.workMatrix);
        frame.offsetX.vx = g_BattleCameraPosition.cameraPosX - g_BattleCameraFocalPoint.focalPointX;
        frame.offsetX.vy = g_BattleCameraPosition.cameraPosY - g_BattleCameraFocalPoint.focalPointY;
        frame.offsetX.vz = g_BattleCameraPosZ - g_BattleCameraFocusZ;
        TransformVectorToInt16((void *)&g_BattleCameraTransform, &frame.offsetX, &frame.offsetX.vx);
        TransformVectorToInt16(&frame.workMatrix, &frame.offsetX, &frame.offsetX.vx);
        TransposeMatrix(&g_BattleCameraTransform, &frame.workMatrix);
        TransformVectorToInt16(&frame.workMatrix, &frame.offsetX, &frame.offsetX.vx);
        frame.offsetX.vx += g_BattleCameraFocalPoint.focalPointX;
        frame.offsetX.vy += g_BattleCameraFocalPoint.focalPointY;
        frame.offsetX.vz += g_BattleCameraFocusZ;
        frame.tmpAngle = frame.pairRecord->frame - 0x14;
        frame.posX.x += (frame.offsetX.vx - frame.posX.x) / frame.blendDivisor * frame.tmpAngle;
        frame.posX.y += (frame.offsetX.vy - frame.posX.y) / frame.blendDivisor * frame.tmpAngle;
        frame.posX.z += (frame.offsetX.vz - frame.posX.z) / frame.blendDivisor * frame.tmpAngle;
        frame.goalX.x += (frame.offsetX.vx - frame.goalX.x) / frame.blendDivisor * frame.tmpAngle;
        frame.goalX.y += (frame.offsetX.vy - frame.goalX.y) / frame.blendDivisor * frame.tmpAngle;
        frame.goalX.z += (frame.offsetX.vz - frame.goalX.z) / frame.blendDivisor * frame.tmpAngle;
    }
    frame.scaleVec.vz = frame.pairRecord->angle;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrixA, &frame.workMatrix);
    ScaleMatrix(&frame.workMatrix, &frame.scaleVec);
    SetRotMatrix(&frame.workMatrix);
    SetTransMatrix(&frame.workMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleMagicHasteLoadedResTable);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrixB, &frame.workMatrix);
    frame.workMatrix.t[2] -= frame.pairRecord->frame << 1;
    ScaleMatrix(&frame.workMatrix, &frame.scaleVec);
    SetRotMatrix(&frame.workMatrix);
    SetTransMatrix(&frame.workMatrix);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleMagicHasteLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.pairRecord->frame >= 41) {
        frame.pairRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.pairRecord->spinY -= 0x100 /* 22.5 deg */;
    frame.pairRecord->spinZ += 0x180;
    if (frame.pairRecord->state != 0) {
        frame.pairRecord->spinY += 0x180;
        frame.pairRecord->spinX += 0x40;
    }
    if (frame.pairRecord->state == 0) {
        frame.pairRecord->orbitSpin -= 0x100 /* 22.5 deg */;
    }
    if (frame.pairRecord->frame < 20) {
        frame.pairRecord->angle += frame.pairRecord->angleStep;
    }
    if (frame.pairRecord->frame < 20 || frame.pairRecord->state != 0) {
        (frame.pairRecord->frame)++;
    }
}

/*
 * Morphs Heist mesh vertices using linear interpolation between poses.
 */
// FUNCTION: FF7 0x00588a54
void __cdecl BattleFx_MagicHasteMorphModelVerticesTask(void)
{
    int i;
    BattleFxHasteMorphState *task;
    int resultA;
    int resultB;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxHasteMorphState *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step < 7) {
        if (task->step == 1) {
            task->morphWeight = 0;
            task->weightStep = 682;
            BattleFx_MagicHasteBuildMeshVertexDeltas(g_BattleMagicHasteMeshModel1, g_BattleMagicHasteMeshModel2, g_BattleMagicHasteTargetMeshPtr);
        }
        resultA = Gfx_ModelTableGetRenderObject(1, g_BattleMagicHasteLoadedResTable);
        if (resultA != 0) {
            *(int *)&g_BattleMagicHasteWeight2 = FLOAT_ONE_BITS;
            g_BattleMagicHasteWeight1 = (float)(task->morphWeight / 4096);
            g_BattleMagicHasteVertexBuffer = (int)g_BattleMagicHasteTargetMeshPtr;
            Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_MagicHasteBlendPolygonSetVertices, *(int *)((char *)resultA + 4));
        }
    } else {
        if (task->step == 7) {
            task->morphWeight = 0;
            task->weightStep = 819;
            BattleFx_MagicHasteBuildMeshVertexDeltas(g_BattleMagicHasteMeshModel2, g_BattleMagicHasteMeshModel3, g_BattleMagicHasteTargetMeshPtr);
        }
        resultB = Gfx_ModelTableGetRenderObject(1, g_BattleMagicHasteLoadedResTable);
        if (resultB != 0) {
            *(int *)&g_BattleMagicHasteWeight2 = 0;
            *(int *)&g_BattleMagicHasteWeight1 = FLOAT_ONE_BITS;
            g_BattleMagicHasteVertexBuffer = (int)&g_BattleMagicHasteMeshModel2;
            Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_MagicHasteBlendPolygonSetVertices, *(int *)((char *)resultB + 4));
            *(int *)&g_BattleMagicHasteWeight2 = FLOAT_ONE_BITS;
            g_BattleMagicHasteWeight1 = (float)(task->morphWeight / 4096);
            g_BattleMagicHasteVertexBuffer = (int)g_BattleMagicHasteTargetMeshPtr;
            Gfx_ForEachPolygonSetPart((DxGroupPartCallback)BattleFx_MagicHasteBlendPolygonSetVertices, *(int *)((char *)resultB + 4));
        }
    }
    if (task->step == 20) {
        for (i = 0; i < 10; i++) {
            if ((g_BattleMagicHasteVertexDeltaBuffer & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    task->morphWeight += task->weightStep;
    if (task->morphWeight > 4096) {
        task->morphWeight = 0x1000;
    }
    (task->step)++;
}

/*
 * Computes mesh vertex coordinate deltas between two poses for Heist.
 */
// FUNCTION: FF7 0x00588c82
void __cdecl BattleFx_MagicHasteBuildMeshVertexDeltas(SVECTOR *poseA, SVECTOR *poseB, SVECTOR *deltas)
{
    int i;

    for (i = 0; i < 130; i++) {
        deltas[i].vx = poseB[i].vx - poseA[i].vx;
        deltas[i].vy = poseB[i].vy - poseA[i].vy;
        deltas[i].vz = poseB[i].vz - poseA[i].vz;
    }
}
} /* extern "C" */

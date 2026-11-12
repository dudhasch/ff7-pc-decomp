// src/ff7/src/battle/battle_target_effect_matrices.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle target and effect geometry: finds the centroid of the bones of the selected
//   actors, builds randomized cone vectors, and installs oriented matrices for battle effects.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "lib/api/graphics.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
extern "C" {


/*
 * Averages the target-bone world positions of the actors selected in actorMask (actors 0..9)
 * and stores the centroid in centroid.
 */
// FUNCTION: FF7 0x005d2f40
void __cdecl Battle_GetTargetMaskBoneCentroid(short actorMask, Vec3sPacked *centroid)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[32];
        int sumX;
        struct { unsigned char pad_to_4[0x4]; int sumY; };
        struct { unsigned char pad_to_8[0x8]; int sumZ; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR boneX; };
        struct { unsigned char pad_to_18[0x18]; short actorIndex; };
        struct { unsigned char pad_to_1C[0x1C]; short selectedCount; };
    } frame;
#pragma pack(pop)

    frame.selectedCount = 0;
    frame.sumZ = 0;
    frame.sumY = frame.sumZ;
    frame.sumX = frame.sumY;
    for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
        if ((actorMask & 1 << frame.actorIndex) != 0) {
            frame.selectedCount++;
            Battle_GetBoneWorldPos(frame.actorIndex, g_BattleActorRecords[frame.actorIndex].targetBone, &frame.boneX.vx);
            frame.sumX += frame.boneX.vx;
            frame.sumY += frame.boneX.vy;
            frame.sumZ += frame.boneX.vz;
        }
    }
    centroid->vx = frame.sumX / frame.selectedCount;
    centroid->vy = frame.sumY / frame.selectedCount;
    centroid->vz = frame.sumZ / frame.selectedCount;
}

/*
 * Builds a randomized cone vector with jittered Y component and randomized X/Z angles.
 */
// FUNCTION: FF7 0x005d301c
void __cdecl BattleFx_MakeRandomConeVector(short baseY, short spread, void *scale)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotation; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR coneX; };
    } frame;
#pragma pack(pop)

    frame.coneX.vz = 0;
    frame.coneX.vx = frame.coneX.vz;
    frame.coneX.vy = baseY + ((rand() % spread << 1) - spread);
    frame.angleX.vy = 0;
    frame.angleX.vx = rand() % 0x320 - 0x190;
    frame.angleX.vz = rand() % 0x320 - 0x190;
    RotMatrixXYZ(&frame.angleX, &frame.rotation);
    TransformVectorToInt16(&frame.rotation, &frame.coneX, scale);
}

/*
 * Installs a rotation/translation matrix for an effect: transforms position into camera space,
 * pushes it pushAmount along its direction when nonzero, then builds the rotation matrix from the
 * angles in rotation, scales it uniformly by scale and installs it.
 */
// FUNCTION: FF7 0x005d30ac
void __cdecl BattleFx_InstallOrientedMatrixUniform(void *position, void *rotation, short scale, int pushAmount)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        int depth;
        struct { unsigned char pad_to_24[0x24]; VECTOR dirX; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_58[0x58]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(position, &frame.rotMatrix.t[0], &frame.depth);
    if (pushAmount != 0) {
        NormalizeVectorFixed(&frame.rotMatrix.t[0], &frame.dirX);
        frame.rotMatrix.t[0] += frame.dirX.vx * pushAmount >> 0xC;
        frame.rotMatrix.t[1] += frame.dirX.vy * pushAmount >> 0xC;
        frame.rotMatrix.t[2] += frame.dirX.vz * pushAmount >> 0xC;
    }
    RotMatrix((SVECTOR *)rotation, &frame.rotMatrix);
    frame.scaleX.vz = scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
}

/*
 * Same as BattleFx_InstallOrientedMatrixUniform, but scales each axis by the vector in scale.
 */
// FUNCTION: FF7 0x005d317b
void __cdecl BattleFx_InstallOrientedMatrixScaled(void *position, void *rotation, void *scale, int pushAmount)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        int depth;
        struct { unsigned char pad_to_24[0x24]; VECTOR dirX; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix rotMatrix; };
    } frame;
#pragma pack(pop)

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(position, &frame.rotMatrix.t[0], &frame.depth);
    if (pushAmount != 0) {
        NormalizeVectorFixed(&frame.rotMatrix.t[0], &frame.dirX);
        frame.rotMatrix.t[0] += frame.dirX.vx * pushAmount >> 0xC;
        frame.rotMatrix.t[1] += frame.dirX.vy * pushAmount >> 0xC;
        frame.rotMatrix.t[2] += frame.dirX.vz * pushAmount >> 0xC;
    }
    RotMatrix((SVECTOR *)rotation, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, scale);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
}
} /* extern "C" */

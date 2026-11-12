// src/ff7/src/battle/battle_bone_world_camera.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Converts bone positions to world coordinates and transforms world points into
//   camera-local space for battle rendering.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/types/battle.h"

#include "lib/api/graphics.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
extern "C" {

/*
 * Reads the world position of bone `boneIdx` of actor `actorIdx` (three floats, 12 bytes per bone
 * from the actor's boneWorldPosX) and narrows each coordinate to a short.
 */
// FUNCTION: FF7 0x005d2e00
void __cdecl Battle_GetBoneWorldPos(int actorIdx, int boneIdx, short *outPos)
{
    outPos[0] = (short)*(float *)((char *)&g_BattleActorRecords[actorIdx].boneWorldPosX + boneIdx * 0xC);
    outPos[1] = (short)*(float *)((char *)&g_BattleActorRecords[actorIdx].boneWorldPosY + boneIdx * 0xC);
    outPos[2] = (short)*(float *)((char *)&g_BattleActorRecords[actorIdx].boneWorldPosX + boneIdx * 0xC + 8);
}

/*
 * Narrows world translation minus camera translation into three shorts, transposes global matrix
 * into local w and transforms vector in place. Local w represents larger matrix across separate
 * reserved locals; original layout proof needed.
 */
// FUNCTION: FF7 0x005d2e6a
void __cdecl Battle_WorldPointToCameraLocal(struct BattleCameraWorldPointScratch *worldPoint, Vec3sPacked *localPoint)
{
    int localMatrix[8];

    localPoint->vx = worldPoint->worldX - (g_BattleCameraTransform.translation[0]);
    localPoint->vy = worldPoint->worldY - (g_BattleCameraTransform.translation[1]);
    localPoint->vz = worldPoint->worldZ - (g_BattleCameraTransform.translation[2]);
    TransposeMatrix((void *)g_BattleCameraTransform.matrix, localMatrix);
    TransformVectorToInt16(localMatrix, (short *)localPoint, (short *)localPoint);
}

/*
 * Narrows world-camera translation differences into three shorts, transposes global matrix into
 * local w and transforms in place. Separate reserved C locals used as matrix storage require
 * native frame proof.
 */
// FUNCTION: FF7 0x005d2ed1
void __cdecl Battle_WorldPointToCameraLocalDup(struct BattleCameraWorldPointScratch *worldPoint, BattleCameraLocalPoint2 *localPoint)
{
    int localMatrix[8];

    localPoint->x = worldPoint->worldX - (g_BattleCameraTransform.translation[0]);
    localPoint->y = worldPoint->worldY - (g_BattleCameraTransform.translation[1]);
    localPoint->z = worldPoint->worldZ - (g_BattleCameraTransform.translation[2]);
    TransposeMatrix((void *)g_BattleCameraTransform.matrix, localMatrix);
    TransformVectorToInt16(localMatrix, (short *)localPoint, (short *)localPoint);
}
} /* extern "C" */

// src/ff7/highway/highway_scene_camera_effects.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Scene, camera and effect code for the highway minigame: the scene stores its engine
//   context, positions and rotates its target, and updates its path and camera each frame. It also
//   derives sound pan and volume for actors from their distance, and holds effect coordinate and
//   init tables and skeleton sampling helpers.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/types/graphics.h"
#include "ff7/api/highway.h"
#include "ff7/api/snobo.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"
#include "ff7/enums/highway.h"
#include "lib/codegen.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
extern "C" {
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)
// GLOBAL: FF7 0x00d8d45c
int g_HighwayCameraYaw = 0;


void __cdecl SetPerspective(float fov, float nearPlane, float farPlane, float offsetX, float offsetY, float viewWidth, float viewHeight, struct GfxEngineContext *context);

#pragma intrinsic(abs)
} // extern "C"
static __inline int HighwayResolveFieldResource(int index)
{
    int resource;
    int tableOffset;
    tableOffset = g_HighwayPsxDataSet.adrOffsets[index];
    if (tableOffset >= g_HighwayPsxDataSet.binSize) {
        resource = 0;
    } else {
        resource = g_HighwayPsxDataSet.binData + tableOffset;
    }
    return resource;
}

/*
 * Stores the engine context and initializes the scene, builds a translation from the negated eye
 * position and submits the view and identity matrices. Copies the context's clip origin and view
 * size to float members and sets the perspective, passing the view width for both final dimensions
 * despite copying the height separately.
 */
// FUNCTION: FF7 0x0065a3a0
HighwayScene::HighwayScene()
{
    float screenMatrix[4][4];
    float negPos[3];
    float viewMatrix[4][4];

    this->context = GetEngineContext();
    initialize();
    negPos[0] = (float)(-this->eyeX) * 1.0f;
    negPos[1] = (float)(-this->eyeY) * 1.0f;
    negPos[2] = (float)(-this->eyeZ) * 1.0f;
    SetIdentityMatrix4x4(viewMatrix);
    BuildTranslationMatrixRowVec(negPos, (void *)viewMatrix);
    Gfx_SetContextMatrix((void *)viewMatrix, this->context);
    SetIdentityMatrix4x4(screenMatrix);
    Gfx_RebuildContextScreenTransform(screenMatrix, this->context);
    Gfx_SetUiScalingViewport(this->context);
    this->viewWidth = (float)this->context->viewSizeX;
    this->viewHeight = (float)this->context->viewSizeY;
    this->clipLeft = (float)this->context->clipLeft;
    this->clipTop = (float)this->context->clipTop;
    SetPerspective(56.15f, 50.0f, 40000.0f, 0.0f, 0.0f, this->viewWidth,
                   this->viewWidth, this->context);
    this->farDepthBits = FLOAT_39950_BITS;
    this->scaleBits = 0x38000000;
}

/*
 * Resolves scene resources, selects the camera mode and initializes its position fields.
 */
// FUNCTION: FF7 0x0065a563
void HighwayScene::initialize()
{
    this->pathCountTable = HighwayResolveFieldResource(0x20);
    this->pathOffsetTable = HighwayResolveFieldResource(0x21);
    if (g_HighwayControlFlags & HIGHWAY_CTRL_DEBUG_CAM_MODE7) {
        this->mode = 7;
    } else if (g_HighwayControlFlags & HIGHWAY_CTRL_DEBUG_CAM_MODE6_FREEZE_SCROLL) {
        this->mode = 6;
    } else {
        this->mode = HIGHWAY_CAMERA_MODE_CHASE;
    }
    this->eyeX = 0;
    this->eyeY = 0;
    this->eyeZ = -2000;
    this->focusX = 0;
    this->focusY = 0;
    this->focusZ = 0;
    this->targetX = 0;
    this->targetY = 0;
    this->targetZ = 0;
    this->targetOffsetX = 0;
    this->targetOffsetY = 0;
    this->targetOffsetZ = 0;
    this->eyeOffsetX = 0;
    this->eyeOffsetY = 0;
    this->eyeOffsetZ = -500;
    this->lift = 0;
    this->renderOffset = 0;
    this->cameraLateralOffset = 0;
    this->cameraHeightOffset = 0;
    this->initZero_A4 = 0;
}

/*
 * Builds a translation matrix from three coordinates and applies it to the scene target.
 * Forms a three-int vector from x, y, z, passes it and a local matrix to BuildTranslationMatrixRowVec, then passes
 * the matrix and the leading dword at this to Gfx_MultiplyContextMatrix.
 */
// FUNCTION: FF7 0x0065a6fa
void HighwayScene::positionMatrix(int x, int y, int z)
{
    int vector[3];
    int matrix[16];
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
    BuildTranslationMatrixRowVec(vector, matrix);
    Gfx_MultiplyContextMatrix(matrix, this->context);
}

/*
 * Stores x, y, z in the position of the file block's first record. this is unused.
 */
// FUNCTION: FF7 0x0065a73d
void HighwayScene::position(int x, int y, int z)
{
    HighwayScenePosition *rec = (HighwayScenePosition *)(g_pHighwayFileBlock->baseMatrix);
    rec->x = x;
    rec->y = y;
    rec->z = z;
}

/*
 * Builds an axis rotation matrix and applies it to the scene target.
 * Builds the selected axis rotation for axis zero, one, two and submits the local matrix to
 * Gfx_MultiplyContextMatrix. Other axis values submit an uninitialized matrix; no validation is present.
 */
// FUNCTION: FF7 0x0065a771
void HighwayScene::rotateAxis(int axis, int angle)
{
    int matrix[16];
    if (axis == 0) {
        /* Retail caller passes raw IEEE-754 float bits in int parameter angle without int-to-float conversion. */
        ((void(__cdecl *)(int, void *))BuildRotationXRowVec)(angle, matrix);
    } else if (axis == 1) {
        /* Retail caller prototype (int, void *) -> void preserved for /Od argument promotion on `BuildRotationYRowVec`. */
        ((void(__cdecl *)(int, void *))BuildRotationYRowVec)(angle, matrix);
    } else if (axis == 2) {
        /* Retail caller prototype (int, void *) -> void preserved for /Od argument promotion on `BuildRotationZRowVec`. */
        ((void(__cdecl *)(int, void *))BuildRotationZRowVec)(angle, matrix);
    }
    Gfx_MultiplyContextMatrix(matrix, this->context);
}

/*
 * For mode seven loads path position/rotation, builds a float rotation matrix, clears
 * translation/W, transforms the negative position with y offset 180 and installs matrix plus
 * transformed translation globally. Mode six does nothing; all other modes call draw with
 * g_HighwayActorRenderOffset.
 */
// FUNCTION: FF7 0x0065a7d8
void HighwayScene::update()
{
    int pathRot[2];
    float negPos[3];
    float viewPosition[3];
    HighwayMat64 floatMatrix;
    int pos[4];
    int psxMatrix[8];

    if (this->mode == 7) {
        g_pHighwayFieldManager->load(g_HighwayCameraZPosition + 0x2300, 0, pos, pathRot);
        RotMatrixXYZ((void *)pathRot, (void *)psxMatrix);
        GtePsxMatrixToFloat4x4((PsxMatrix *)psxMatrix, &floatMatrix);
        ClearMatrixTranslationAndW(&floatMatrix);
        negPos[0] = (float)(-pos[0]) * 1.0f;
        negPos[1] = (float)(-(pos[1] - 0xB4)) * 1.0f;
        negPos[2] = (float)(-pos[2]) * 1.0f;
        TransformPoint3RowVec(&floatMatrix, negPos, viewPosition);
        *(HighwayMat64 *)*(int *)g_pHighwayFileBlock = floatMatrix;
        ((HighwayFileBlockPosition *)(g_pHighwayFileBlock->baseMatrix))->posX = viewPosition[0];
        ((HighwayFileBlockPosition *)(g_pHighwayFileBlock->baseMatrix))->posY = viewPosition[1];
        ((HighwayFileBlockPosition *)(g_pHighwayFileBlock->baseMatrix))->posZ = viewPosition[2];
    } else if (this->mode == 6) { /* empty first arm: lib/codegen.h */
    } else {
        draw(g_HighwayActorRenderOffset);
    }
}

/*
 * Computes actor distances from the scene camera, derives bounded sound pan/volume for actor zero
 * and the nearest eligible actor 2 through 5, and calculates a proximity camera adjustment. Camera
 * modes 0, 1, 4 and 5 sample field positions using stored offsets or a timed skeletal path; mode 4
 * returns to mode 1 after its path limit. Builds target direction, pitch/yaw and roll matrices,
 * exports five float matrices and stores the camera yaw. Missing optional actors are skipped.
 * Unsupported camera modes leave the orientation sample e uninitialized even though its roll is
 * later used; distance square sums can overflow.
 */
// FUNCTION: FF7 0x0065a91c
void HighwayScene::draw(int context)
{
    char rotMatrix[32];
    int soundPan;
    int distances[20];
    struct HighwayModelBatchContext *drawInformation;
    int lift8;
    char viewMatrix[32];
    HighwaySlotState *carState;
    int attenuation[20];
    HighwayWords4 deltaVec[20];
    unsigned char rollFlag;
    int modeBlk;
    char shiftMtx[32];
    short rawRollVec[4] = {0, 0, 0};
    int aimVec[4] = {0, 0, 0};
    int dirNorm[4] = {0, 0, 0};
    short pitchYaw[4] = {0, 0, 0};
    short curOrientation[4] = {0, 0, 0};
    short positionEye[4] = {0, 0, 0};
    short targetPos[4] = {0, 0, 0};
    char rollMat[32];
    char staticMatrix[32];
    int actorIndex;
    HighwayWords4 curActorDir[20];
    carState = 0;
    drawInformation = 0;
    actorIndex = 0;
    carState = (HighwaySlotState *)(actorIndex * 0x1a8 + (char *)(g_HighwaySlotActors + 0x68));
    drawInformation = (struct HighwayModelBatchContext *)(((HighwayRenderSlotActor *)g_HighwaySlotActors)[actorIndex].actors[0]);
    deltaVec[actorIndex].words[0] = drawInformation->posX - this->eyeX;
    deltaVec[actorIndex].words[1] = drawInformation->posY - this->eyeY;
    deltaVec[actorIndex].words[2] = drawInformation->posZ - this->eyeZ;
    distances[actorIndex] = Mini_HighwayIntSqrtUnsigned(deltaVec[actorIndex].words[0] * deltaVec[actorIndex].words[0] + deltaVec[actorIndex].words[1] * deltaVec[actorIndex].words[1] + deltaVec[actorIndex].words[2] * deltaVec[actorIndex].words[2]);
    attenuation[actorIndex] = distances[actorIndex] >> 5;
    NormalizeVectorFixed(&deltaVec[actorIndex], &curActorDir[actorIndex]);
    soundPan = g_HighwayCameraYaw - (ArcTangentFixed(curActorDir[actorIndex].words[2], curActorDir[actorIndex].words[0]) - 0x400);
    if (soundPan > 0x800)
        soundPan -= 0x1000;
    if (soundPan < -0x800)
        soundPan += 0x1000;
    soundPan >>= 2;
    soundPan += 0x40;
    if (soundPan > 0x7f)
        soundPan = 0x7f;
    if (soundPan < 0)
        soundPan = 0;
    if (attenuation[actorIndex] > 0x5a)
        attenuation[actorIndex] = 0x5a;
    if (attenuation[actorIndex] < 0)
        attenuation[actorIndex] = 0;
    g_HighwaySoundParam = 0x7f - attenuation[actorIndex];
    g_HighwaySlot0Volume = 0x7f - attenuation[actorIndex];
    Mini_HighwaySendChannelCommandA8((short)soundPan, 2);
    Mini_HighwaySendChannelCommandA8((short)soundPan, 4);
    Mini_HighwaySendChannelCommandA0(g_HighwaySoundParam, 1);
    Mini_HighwaySendChannelCommandA0(g_HighwaySoundParam, 4);
    this->nearestDistance = distances[0];
    this->nearestRivalDistance = 0x7fffffff;
    this->nearestRivalSlot = 0;
    for (actorIndex = 1; actorIndex <= 5; actorIndex++) {
        carState = (HighwaySlotState *)(actorIndex * 0x1a8 + (char *)(g_HighwaySlotActors + 0x68));
        if (!((HighwayRenderSlotActor *)g_HighwaySlotActors)[actorIndex].actors[0])
            continue;
        drawInformation = (struct HighwayModelBatchContext *)(((HighwayRenderSlotActor *)g_HighwaySlotActors)[actorIndex].actors[0]);
        deltaVec[actorIndex].words[0] = drawInformation->posX - this->eyeX;
        deltaVec[actorIndex].words[1] = drawInformation->posY - this->eyeY;
        deltaVec[actorIndex].words[2] = drawInformation->posZ - this->eyeZ;
        distances[actorIndex] = Mini_HighwayIntSqrtUnsigned(deltaVec[actorIndex].words[0] * deltaVec[actorIndex].words[0] + deltaVec[actorIndex].words[1] * deltaVec[actorIndex].words[1] + deltaVec[actorIndex].words[2] * deltaVec[actorIndex].words[2]);
        attenuation[actorIndex] = distances[actorIndex] >> 5;
        if (distances[actorIndex] < this->nearestDistance && actorIndex >= 1 && carState->state != 5) {
            this->nearestDistance = distances[actorIndex];
        }
        if (distances[actorIndex] < this->nearestRivalDistance && actorIndex >= 2 && carState->state != 5) {
            this->nearestRivalSlot = actorIndex;
            this->nearestRivalDistance = distances[actorIndex];
        }
    }
    if (this->nearestRivalSlot) {
        /* Preserve VC5 /Tp /Od register choice when indexing stack array by `this->` member. */ NormalizeVectorFixed((void *)(int)&deltaVec[this->nearestRivalSlot], (void *)(int)&curActorDir[this->nearestRivalSlot]);
        soundPan = g_HighwayCameraYaw - (ArcTangentFixed(curActorDir[this->nearestRivalSlot].words[2], curActorDir[this->nearestRivalSlot].words[0]) - 0x400);
        if (soundPan > 0x800)
            soundPan -= 0x1000;
        if (soundPan < -0x800)
            soundPan += 0x1000;
        soundPan >>= 2;
        soundPan += 0x40;
        if (soundPan > 0x7f)
            soundPan = 0x7f;
        if (soundPan < 0)
            soundPan = 0;
        if (attenuation[this->nearestRivalSlot] > 0x5a)
            attenuation[this->nearestRivalSlot] = 0x5a;
        if (attenuation[this->nearestRivalSlot] < 0)
            attenuation[this->nearestRivalSlot] = 0;
        g_HighwayNearestRivalVolume = 0x7f - attenuation[this->nearestRivalSlot];
        Mini_HighwaySendChannelCommandA8((short)soundPan, 3);
        Mini_HighwaySendChannelCommandA0(g_HighwayNearestRivalVolume, 3);
    } else {
        Mini_HighwaySendChannelCommandA0(0, 3);
    }
    carState = (HighwaySlotState *)(g_HighwaySlotActors + 0x68);
    this->lift = 0;
    if (this->nearestDistance < 0x320) {
        this->lift = (0x320 - this->nearestDistance) / 4;
    }
    lift8 = (carState->posZ - 0x55730) / 0x800;
    if (lift8 < 0)
        lift8 = 0;
    if (lift8 > this->lift)
        this->lift = lift8;
    switch (this->mode) {
    case HIGHWAY_CAMERA_MODE_CHASE:
        positionEye[0] = (-(carState->posX >> 9) << 2) / 3;
        positionEye[1] = 0x55;
        positionEye[2] = carState->posZ >> 9;
        targetPos[0] = carState->posX >> 8;
        targetPos[1] = 0x78;
        targetPos[2] = carState->posZ >> 8;
        g_pHighwayFieldManager->load(context + positionEye[2], positionEye[0], &this->eyeX, (int *)curOrientation);
        g_pHighwayFieldManager->samplePosition(context + targetPos[2], targetPos[0], &this->targetX);
        this->eyeY -= positionEye[1];
        this->targetY -= targetPos[1];
        this->targetY += lift8 * 2;
        break;
    case HIGHWAY_CAMERA_MODE_PATH:
        ((HighwaySkelItem *)this)->samplePosition(this->accum, this->coordsStart, &this->lastEyeOffsetX, 1);
        positionEye[0] = (-(carState->posX >> 9) << 2) / 3 + this->lastEyeOffsetX;
        positionEye[1] = 0x55;
        positionEye[2] = (carState->posZ >> 9) + this->lastEyeOffsetZ;
        targetPos[0] = carState->posX >> 8;
        targetPos[1] = 0x78;
        targetPos[2] = carState->posZ >> 8;
        g_pHighwayFieldManager->load(context + positionEye[2], positionEye[0], &this->eyeX, (int *)curOrientation);
        g_pHighwayFieldManager->samplePosition(context + targetPos[2], targetPos[0], &this->targetX);
        this->eyeY -= positionEye[1];
        this->eyeY -= this->lastEyeOffsetY;
        this->targetY -= targetPos[1];
        this->targetY += lift8 * 2;
        this->accum += this->step;
        if (*(unsigned int *)&this->accum > this->limit)
            this->mode = HIGHWAY_CAMERA_MODE_CHASE;
        break;
    case HIGHWAY_CAMERA_MODE_FIXED_EYE:
        g_pHighwayFieldManager->load(this->renderOffset, this->eyeOffsetX + this->cameraLateralOffset, &this->eyeX, (int *)curOrientation);
        g_pHighwayFieldManager->samplePosition(context + (carState->posZ >> 8), carState->posX >> 8, &this->targetX);
        this->eyeY -= this->cameraHeightOffset;
        this->lastEyeOffsetX = this->eyeOffsetX;
        this->lastEyeOffsetY = this->eyeOffsetY;
        this->lastEyeOffsetZ = this->eyeOffsetZ;
        break;
    case HIGHWAY_CAMERA_MODE_OFFSETS:
        g_pHighwayFieldManager->load(context + this->eyeOffsetZ, this->eyeOffsetX, &this->eyeX, (int *)curOrientation);
        g_pHighwayFieldManager->samplePosition(context + this->targetOffsetZ + (carState->posZ >> 8), this->targetOffsetX, &this->targetX);
        this->targetY += this->targetOffsetY;
        this->eyeY += this->eyeOffsetY;
        this->lastEyeOffsetX = this->eyeOffsetX;
        this->lastEyeOffsetY = this->eyeOffsetY;
        this->lastEyeOffsetZ = this->eyeOffsetZ;
        break;
    }
    aimVec[0] = this->targetX - this->eyeX;
    aimVec[1] = this->targetY - (this->eyeY - this->lift);
    aimVec[2] = this->targetZ - this->eyeZ;
    this->focusX = this->targetX;
    this->focusY = this->targetY - (this->lift >> 1);
    this->focusZ = this->targetZ;
    if (Mini_HighwayIntSqrtUnsigned(aimVec[0] * aimVec[0] + aimVec[1] * aimVec[1] + aimVec[2] * aimVec[2]) > 0x1000) {
        NormalizeVectorFixed(aimVec, dirNorm);
        pitchYaw[0] = Mini_HighwayCameraAtan2Angle4096(dirNorm[1], Mini_HighwayIntSqrtUnsigned(dirNorm[0] * dirNorm[0] + dirNorm[2] * dirNorm[2]));
        pitchYaw[1] = Mini_HighwayCameraAtan2Angle4096(dirNorm[2], dirNorm[0]) - 0x400;
        pitchYaw[2] = 0;
    } else {
        pitchYaw[0] = Mini_HighwayCameraAtan2Angle4096(aimVec[1], Mini_HighwayIntSqrtUnsigned(aimVec[0] * aimVec[0] + aimVec[2] * aimVec[2]));
        pitchYaw[1] = Mini_HighwayCameraAtan2Angle4096(aimVec[2], aimVec[0]) - 0x400;
        pitchYaw[2] = 0;
    }
    SetIdentityPsxMatrix((PsxMatrix *)viewMatrix);
    SetIdentityPsxMatrix((PsxMatrix *)shiftMtx);
    SetIdentityPsxMatrix((PsxMatrix *)rotMatrix);
    SetIdentityPsxMatrix((PsxMatrix *)rollMat);
    SetIdentityPsxMatrix((PsxMatrix *)staticMatrix);
    ((PsxMatrix *)shiftMtx)->t[0] = aimVec[0];
    ((PsxMatrix *)shiftMtx)->t[1] = aimVec[1];
    ((PsxMatrix *)shiftMtx)->t[2] = aimVec[2];
    rawRollVec[2] = curOrientation[2];
    rollFlag = rawRollVec[2] != 0;
    modeBlk = ((int)((int)g_pHighwayModeBlock));
    *(unsigned char *)modeBlk = rollFlag;
    Matrix30SetRotationZ(rawRollVec[2], (struct SnoboMatrix *)rollMat);
    Matrix30SetRotationFromAngles((struct SnowboardAngles *)pitchYaw, rotMatrix);
    Matrix30ConcatenateTransform(rotMatrix, (struct SnowboardMatrix302 *)shiftMtx, viewMatrix);
    Matrix30ConcatenateTransform(rotMatrix, (struct SnowboardMatrix302 *)shiftMtx, staticMatrix);
    Matrix30ConcatenateTransform(rollMat, (struct SnowboardMatrix302 *)viewMatrix, viewMatrix);
    GtePsxMatrixToFloat4x4Transposed(viewMatrix, g_pHighwayFileBlock->baseMatrix);
    GtePsxMatrixToFloat4x4Transposed(shiftMtx, g_pHighwayFileBlock->translationMatrix);
    GtePsxMatrixToFloat4x4Transposed(rotMatrix, g_pHighwayFileBlock->rotationMatrix);
    GtePsxMatrixToFloat4x4Transposed(rollMat, g_pHighwayFileBlock->rollMatrix);
    GtePsxMatrixToFloat4x4Transposed(staticMatrix, g_pHighwayFileBlock->viewMatrixNoRoll);
    g_HighwayCameraYaw = pitchYaw[1];
}
extern "C" {

/*
 * Converts an integer-vector angle from atan2 into a 4096-unit turn.
 */
// FUNCTION: FF7 0x0065b918
int __cdecl Mini_HighwayCameraAtan2Angle4096(int dy, int dx)
{
    return (int)(atan2((double)dy, (double)dx) * 4096.0 / 6.28318530718);
}

/*
 * Returns the integer square root of an unsigned value.
 */
// FUNCTION: FF7 0x0065b939
int __cdecl Mini_HighwayIntSqrtUnsigned(unsigned int value)
{
    return (int)sqrt((double)value);
}
} // extern "C"

/*
 * Initializes an effect from a selected coordinate row and two owner values.
 * Stores the owner, loads the indexed coordinates, starts at their base pointer, clears accum, sets
 * limit to ((count - 1) << 16) - 1 and stores the data pointer.
 */
// FUNCTION: FF7 0x0065b95d
void HighwayEffect::init(char index, int owner, int data)
{
    this->owner = owner;
    loadCoords(index);
    this->coordsStart = this->coordsBase;
    this->accum = 0;
    this->limit = ((this->count - 1) << 16) - 1;
    this->dataPtr = data;
}

// Resolves the indexed effect coordinate tables through the resource offset map.
static __inline void HighwayResolveFieldResource(int index, int &result)
{
    int offset = g_HighwayPsxDataSet.adrOffsets[index];
    if (offset >= g_HighwayPsxDataSet.binSize) {
        result = 0;
    } else {
        result = g_HighwayPsxDataSet.binData + offset;
    }
}
/*
 * Resolves resources 0x20 and 0x21 through output arguments, reads indexed count and offset from
 * them and stores resource 0x22 plus offset as coordinate pointer.
 */
// FUNCTION: FF7 0x0065b9b6
void HighwayEffect::loadCoords(char index)
{
    int start;
    int cntTable;
    int ofsTable;

    HighwayResolveFieldResource(0x20, cntTable);
    HighwayResolveFieldResource(0x21, ofsTable);
    this->count = ((int *)cntTable)[index];
    start = ((int *)ofsTable)[index];
    this->coordsBase = HighwayResolveFieldResource(0x22) + start;
}

/*
 * Samples three shorts from successive stride-eight entries selected by packed high 16 bits,
 * multiplies their differences by low 16 fraction and shifts 16. Always negates x; negates y/z for
 * mirror zero and keeps their sign otherwise.
 */
// FUNCTION: FF7 0x0065ba86
void HighwaySkelItem::samplePosition(int packed, int table, int *output, char mirror)
{
    // VC5 local-name buckets place the six sampled shorts around the fixed-point delta.
    int p;
    int idx;
    int diff[3];
    short lo26[4];
    int frac;
    short hi29[4];

    idx = (packed & 0xFFFF0000) >> 16;
    frac = packed & 0xFFFF;
    lo26[0] = ((HighwaySkelSample *)table)[idx].x;
    lo26[1] = ((HighwaySkelSample *)table)[idx].y;
    lo26[2] = ((HighwaySkelSample *)table)[idx].z;
    hi29[0] = ((HighwaySkelSample *)table)[idx + 1].x;
    hi29[1] = ((HighwaySkelSample *)table)[idx + 1].y;
    hi29[2] = ((HighwaySkelSample *)table)[idx + 1].z;
    diff[0] = hi29[0] - lo26[0];
    diff[1] = hi29[1] - lo26[1];
    diff[2] = hi29[2] - lo26[2];
    diff[0] *= frac;
    diff[1] *= frac;
    diff[2] *= frac;
    diff[0] >>= 16;
    diff[1] >>= 16;
    diff[2] >>= 16;
    if (!mirror) {
        output[0] = -((HighwaySkelSample *)table)[idx].x - diff[0];
        output[1] = -((HighwaySkelSample *)table)[idx].y - diff[1];
        output[2] = -((HighwaySkelSample *)table)[idx].z - diff[2];
    } else {
        output[0] = -((HighwaySkelSample *)table)[idx].x - diff[0];
        output[1] = ((HighwaySkelSample *)table)[idx].y + diff[1];
        output[2] = ((HighwaySkelSample *)table)[idx].z + diff[2];
    }
}
extern "C" {

/*
 * Sets the camera lateral offset to 0x5A, the height offset to 0xB4, the render offset to
 * g_HighwayActorRenderOffset + 0x29E0 and the camera mode to HIGHWAY_CAMERA_MODE_FIXED_EYE.
 */
// FUNCTION: FF7 0x0065bbee
void __fastcall Mini_HighwaySetSceneBlockPresetEvent05(HighwayEffectSlotInitA *self)
{
    self->cameraLateralOffset = 0x5A;
    self->cameraHeightOffset = 0xB4;
    self->renderOffset = g_HighwayActorRenderOffset + 0x29E0;
    self->sceneKind = HIGHWAY_CAMERA_MODE_FIXED_EYE;
}

/*
 * Sets the camera lateral offset to 0x5A, the height offset to 0xB4, the render offset to
 * g_HighwayActorRenderOffset + 0x14F0 and the camera mode to HIGHWAY_CAMERA_MODE_FIXED_EYE.
 */
// FUNCTION: FF7 0x0065bc30
void __fastcall Mini_HighwaySetSceneBlockPresetEvent13(HighwayEffectSlotInitA *self)
{
    self->cameraLateralOffset = 0x5A;
    self->cameraHeightOffset = 0xB4;
    self->renderOffset = g_HighwayActorRenderOffset + 0x14F0;
    self->sceneKind = HIGHWAY_CAMERA_MODE_FIXED_EYE;
}
} // extern "C"

/*
 * Loads the coordinates for index, then activates the slot's effect record: sets its base
 * pointer, zeroes the track position and sets the limit to (count - 1) << 16 minus one.
 */
// FUNCTION: FF7 0x0065bc72
void HighwayEffect::activate(int slot, char index)
{
    HighwayEffectSlot2 *rec;

    rec = (HighwayEffectSlot2 *)(slot * 0x1A8 + (char *)(g_HighwaySlotActors + 0x68));
    loadCoords(index);
    rec->active = 1;
    rec->coordsBase = this->coordsBase;
    rec->trackPos = 0;
    rec->trackLimit = ((this->count - 1) << 16) - 1;
}

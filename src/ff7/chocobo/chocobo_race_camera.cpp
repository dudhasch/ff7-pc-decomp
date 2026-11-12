// src/ff7/chocobo/chocobo_race_camera.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Camera control for the chocobo race: picks the camera mode and targets from route
//   events, builds look-at matrices, tracks the rider, and applies a smoothed camera matrix to the
//   engine.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/chocobo.h"
#include "lib/types/graphics.h"
#include "ff7/api/chocobo.h"
#include "lib/api/graphics.h"
#include <math.h>
#include <string.h>
#include "ff7/gte.h"
#include "ff7/enums/chocobo.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x0097a108
short g_ChocoboCameraInitValues[11] = {(-500), (-500), 3, 250, 300, 3, 9500, 7500, 3, 0x0000, 0x0000};
// GLOBAL: FF7 0x0097a120
unsigned char g_ChocoboCourseVariantTable0[384] = {0x01, 0x01, 0x00, 0x00, 0x6d, 0xfd, 0x0a, 0x00, 0xfa, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0xf4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x37, 0x21, 0xf4, 0x01, 0x2f, 0xf3, 0x78, 0x00, 0x32, 0x10, 0xee, 0x00, 0xf4, 0x01, 0xd2, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x78, 0x05, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0xb1, 0x3c, 0x48, 0x05, 0xa6, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0xe8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x33, 0x2c, 0x08, 0x07, 0x8b, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0xe8, 0x03, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x2a, 0x0f, 0x08, 0x07, 0xb9, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xcf, 0xf7, 0x37, 0x09, 0xdc, 0x1e, 0x26, 0x02, 0xa7, 0x04, 0x40, 0x06, 0xd4, 0x1d, 0x94, 0x02, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0xd0, 0x07, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xda, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x6e, 0xde, 0xf0, 0x0a, 0x6d, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0xbc, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x78, 0x05, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x78, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x78, 0xdf, 0x22, 0x03, 0xce, 0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x78, 0x05, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x2c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0097a2a0
unsigned char g_ChocoboCourseVariantTable1[320] = {0x01, 0x01, 0x00, 0x00, 0x6d, 0xfd, 0xc8, 0x00, 0xfa, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xae, 0x08, 0xc8, 0x00, 0x59, 0xdf, 0x00, 0x00, 0x6f, 0xf1, 0x2c, 0x01, 0x88, 0xdb, 0x3c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x0b, 0x36, 0x01, 0xd8, 0xe2, 0x32, 0x00, 0x76, 0x09, 0x19, 0x01, 0x6b, 0xfc, 0x8c, 0x00, 0x02, 0x01, 0xa4, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x70, 0xf5, 0x1c, 0x0f, 0xf2, 0xf3, 0x27, 0x01, 0x16, 0xf8, 0xc4, 0x0b, 0xea, 0x06, 0xfc, 0x01, 0x01, 0x01, 0x00, 0x00, 0xab, 0xe8, 0x74, 0x0e, 0xa8, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x6d, 0xe0, 0x54, 0x0b, 0x4e, 0x05, 0xc2, 0x01, 0xa7, 0xe8, 0x98, 0x0a, 0x10, 0x17, 0x30, 0x02, 0x02, 0x02, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0xae, 0xc5, 0xc8, 0x07, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0xbc, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0xbc, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x6c, 0xc9, 0xea, 0x09, 0x13, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xa8, 0xd2, 0xe9, 0x03, 0x07, 0xf3, 0xda, 0x02, 0x35, 0xd4, 0x54, 0x04, 0x97, 0xff, 0x16, 0x03, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x78, 0x05, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x09, 0x00, 0xb0, 0x04, 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0xdc, 0x00, 0x02, 0x72, 0xeb, 0x18, 0x03, 0xc9, 0xdc, 0x38, 0x02, 0x3b, 0xf4, 0x48, 0x03};
// GLOBAL: FF7 0x00e72e10
SVECTOR g_ChocoboCameraSmoothedEye = {0};
// GLOBAL: FF7 0x00e72e18
struct GfxMatrix4x4 g_ChocoboViewMatrix = {0};
// GLOBAL: FF7 0x00e72e58
short g_ChocoboDeferredCameraRacer = 0;
// GLOBAL: FF7 0x00e72e5c
short g_ChocoboDeferredCameraEvent = 0;
// GLOBAL: FF7 0x00e72e60
unsigned char g_ChocoboViewScreenMatrix[64] = {0};
// GLOBAL: FF7 0x00e72ea0
SVECTOR g_ChocoboCameraSmoothedLook = {0};
unsigned char g_bssPad_00e72ea8[8] = {0};
// GLOBAL: FF7 0x00e72eb0
unsigned char g_ChocoboCameraWork[30] = {0};
// GLOBAL: FF7 0x00e72ed0
struct ChocoboCameraPos g_ChocoboCameraEyeGoal = {0};
// GLOBAL: FF7 0x00e72ed8
struct ChocoboRaceCameraPos g_ChocoboRaceCameraLookGoal = {0};
// GLOBAL: FF7 0x00e72ee0
int g_ChocoboCameraRouteReselectPending = 0;
unsigned char g_bssPad_00e72ee4[4] = {0};
unsigned char g_bssPad_00e72ee8[4] = {0};
// clang-format on

#pragma intrinsic(sqrt, atan2)

#pragma intrinsic(strcpy, strcat)

/*
 * Returns the address of course variant table 0 for variant zero and of table 1 otherwise.
 */
// FUNCTION: FF7 0x00771680
int __cdecl Mini_ChocoboGetCameraScriptByVariant(int variant)
{
    if (variant == 0) {
        return (int)&g_ChocoboCourseVariantTable0;
    } else {
        return (int)&g_ChocoboCourseVariantTable1;
    }
}
/*
 * Applies a route event to the race camera. When the player is not riding, the racer and event
 * indices are stored for later and the current route event is cleared. Otherwise the viewed racer
 * becomes racerIndex and, for a valid event, the event's 20-byte record is copied from the track's
 * route event table into g_ChocoboCurrentRouteEvent.
 */
// FUNCTION: FF7 0x00771697
void __cdecl Mini_ChocoboRaceSetCameraModeFromRouteEvent(short eventIndex, short racerIndex)
{
    if (g_ChocoboPlayerRides == -1) {
        g_ChocoboCurrentRouteEvent.eventKind = 0xFF;
        g_ChocoboDeferredCameraRacer = racerIndex;
        g_ChocoboDeferredCameraEvent = eventIndex;
    } else {
        g_ChocoboCameraRouteReselectPending = 0xFFFFFFFF;
        if (eventIndex != -1) {
            g_ChocoboViewedRacer = racerIndex;
            g_ChocoboRouteEventRacer = g_ChocoboViewedRacer;
            g_ChocoboCurrentRouteEvent = *(ChocoboRaceState *)(g_ChocoboTrackData->routeEvents + (eventIndex - 1) * 0x14);
        }
    }
}
/*
 * Builds the camera look-at matrix for an eye position and a look-at target: the yaw and pitch of
 * the direction between them, rolled by 180 degrees, with the rotated eye position as translation.
 */
// FUNCTION: FF7 0x0077170f
void __cdecl Mini_ChocoboBuildLookAtMatrix(PsxMatrix *outMatrix, SVECTOR *eyePos, SVECTOR *targetPos)
{
    float yRotation[16];
    float pitchRad;
    float rotMtx[16];
    int dirVecArr[4];
    float radians;
    char u[84];
    int v[4] = {0, 4095, 0};
    char w[16];
    float rollMatrix[16];
    float angleY;
    float angleZ;
    float rotXMatrix[16];
    float matrixYX[16];
    dirVecArr[0] = targetPos->vx - eyePos->vx;
    dirVecArr[1] = targetPos->vy - eyePos->vy;
    dirVecArr[2] = targetPos->vz - eyePos->vz;
    dirVecArr[0] = -dirVecArr[0];
    radians = (float)atan2((double)dirVecArr[1], sqrt((double)(dirVecArr[0] * dirVecArr[0] + dirVecArr[2] * dirVecArr[2])));
    pitchRad = RadiansToDegrees(radians);
    BuildRotationXRowVec(pitchRad, rotXMatrix);
    radians = (float)atan2((double)dirVecArr[0], (double)dirVecArr[2]);
    angleY = RadiansToDegrees(radians);
    BuildRotationYRowVec(angleY, yRotation);
    angleZ = 180.0f;
    BuildRotationZRowVec(angleZ, rollMatrix);
    MultiplyMatrix4x4Core(yRotation, rotXMatrix, matrixYX);
    MultiplyMatrix4x4Core(matrixYX, rollMatrix, rotMtx);
    ClearMatrixTranslationAndW(rotMtx);
    GteFloat4x4ToPsxMatrix(outMatrix, rotMtx);
    ApplyMatrixToSVectorInt32(outMatrix, eyePos, dirVecArr);
    outMatrix->t[0] = -dirVecArr[0];
    outMatrix->t[1] = -dirVecArr[1];
    outMatrix->t[2] = -dirVecArr[2];
}
/*
 * Picks the eye and look-at goals and the visible course segment range for the current route event
 * kind (follow, fixed, trailing, orbiting or finish), then applies the smoothed camera matrix.
 */
// FUNCTION: FF7 0x007718b0
void __cdecl Mini_ChocoboRaceUpdateCameraTargets(void)
{
    Mini_ChocoboRaceSelectCameraRacerState();
    switch (g_ChocoboCurrentRouteEvent.eventKind) {
    default:
        g_ChocoboCameraBaseSegment = (g_ChocoboSelectedRacerState.segment + g_ChocoboCourseSegmentCount + -0xA) % g_ChocoboCourseSegmentCount;
        g_ChocoboCameraTargetSegment = (g_ChocoboCameraBaseSegment + g_pChocoboCourseWaypoints[g_ChocoboSelectedRacerState.segment].segmentOffset) % g_ChocoboCourseSegmentCount;
        g_ChocoboCameraEyeGoal.camPosY = g_ChocoboCameraSmoothedNode + ((ChocoboCameraInitEntry *)g_ChocoboCameraInitValues)[g_ChocoboCameraInitIndex].eyeHeightOffset;
        g_ChocoboCameraEyeGoal.camPosX = ((g_ChocoboCameraEyeGoal.camPosX + g_ChocoboSelectedRacerState.posX - (SinFixed(g_ChocoboSelectedRacerState.heading) >> ((ChocoboCameraInitEntry *)g_ChocoboCameraInitValues)[g_ChocoboCameraInitIndex].angle)) / 2);
        g_ChocoboCameraEyeGoal.camPosZ = ((g_ChocoboCameraEyeGoal.camPosZ + g_ChocoboSelectedRacerState.posZ - (-CosFixed(g_ChocoboSelectedRacerState.heading) >> ((ChocoboCameraInitEntry *)g_ChocoboCameraInitValues)[g_ChocoboCameraInitIndex].angle)) / 2);
        g_ChocoboRaceCameraLookGoal.posY = ((g_ChocoboRaceCameraLookGoal.posY * 3 + g_ChocoboRacers[g_ChocoboViewedRacer].posY + ((ChocoboCameraInitEntry *)g_ChocoboCameraInitValues)[g_ChocoboCameraInitIndex].lookHeightOffset) / 4);
        g_ChocoboRaceCameraLookGoal.posX = ((g_ChocoboRacers[g_ChocoboViewedRacer].posX * 7 + g_ChocoboSelectedRacerState.posX) / 8);
        g_ChocoboRaceCameraLookGoal.posZ = ((g_ChocoboRacers[g_ChocoboViewedRacer].posZ * 7 + g_ChocoboSelectedRacerState.posZ) / 8);
        break;
    case CHOCOBO_CAMERA_EVENT_FIXED:
        *(int *)&g_ChocoboCameraEyeGoal.camPosX = g_ChocoboCurrentRouteEvent.packedPos;
        *(int *)&g_ChocoboCameraEyeGoal.camPosZ = g_ChocoboCurrentRouteEvent.cameraDistance;
        if (g_ChocoboCurrentRouteEvent.mode != 0) {
            g_ChocoboCameraBaseSegment = (g_ChocoboRacers[g_ChocoboViewedRacer].segment + g_ChocoboCourseSegmentCount + -0x3C) % g_ChocoboCourseSegmentCount;
            g_ChocoboCameraTargetSegment = (g_ChocoboCameraBaseSegment + 0x78) % g_ChocoboCourseSegmentCount;
            g_ChocoboRaceCameraLookGoal.posX = g_ChocoboRacers[g_ChocoboViewedRacer].posX;
            g_ChocoboRaceCameraLookGoal.posY = g_ChocoboRacers[g_ChocoboViewedRacer].posY;
            g_ChocoboRaceCameraLookGoal.posZ = g_ChocoboRacers[g_ChocoboViewedRacer].posZ;
        } else {
            g_ChocoboCameraBaseSegment = g_ChocoboCurrentRouteEvent.segment;
            g_ChocoboCameraTargetSegment = g_ChocoboCurrentRouteEvent.targetSegment;
            *(int *)&g_ChocoboRaceCameraLookGoal.posX = g_ChocoboCurrentRouteEvent.posX;
            *(int *)&g_ChocoboRaceCameraLookGoal.posZ = g_ChocoboCurrentRouteEvent.posY;
        }
        break;
    case CHOCOBO_CAMERA_EVENT_TRAIL:
        if (g_ChocoboSelectedRacerState.segment > g_ChocoboRacers[g_ChocoboViewedRacer].segment) {
            g_ChocoboCameraBaseSegment = (g_ChocoboRacers[g_ChocoboViewedRacer].segment + g_ChocoboCourseSegmentCount + -0x64) % g_ChocoboCourseSegmentCount;
            g_ChocoboCameraTargetSegment = g_ChocoboSelectedRacerState.segment + 0xA;
        } else {
            g_ChocoboCameraBaseSegment = g_ChocoboSelectedRacerState.segment - 0xF;
            g_ChocoboCameraTargetSegment = (g_ChocoboRacers[g_ChocoboViewedRacer].segment + g_ChocoboCourseSegmentCount + 0x64) % g_ChocoboCourseSegmentCount;
        }
        g_ChocoboCameraEyeGoal.camPosY = g_ChocoboSelectedRacerState.posY + g_ChocoboCurrentRouteEvent.cameraOffsetY;
        g_ChocoboCameraEyeGoal.camPosX = g_ChocoboSelectedRacerState.posX - (SinFixed(g_ChocoboSelectedRacerState.heading) >> ((ChocoboCameraInitEntry *)g_ChocoboCameraInitValues)[g_ChocoboCameraInitIndex].angle);
        g_ChocoboCameraEyeGoal.camPosZ = g_ChocoboSelectedRacerState.posZ - (-CosFixed(g_ChocoboSelectedRacerState.heading) >> ((ChocoboCameraInitEntry *)g_ChocoboCameraInitValues)[g_ChocoboCameraInitIndex].angle);
        g_ChocoboRaceCameraLookGoal.posX = ((g_ChocoboRacers[g_ChocoboViewedRacer].posX + g_ChocoboSelectedRacerState.posX) / 2);
        g_ChocoboRaceCameraLookGoal.posY = ((g_ChocoboRacers[g_ChocoboViewedRacer].posY + g_ChocoboSelectedRacerState.posY) / 2) + 200;
        g_ChocoboRaceCameraLookGoal.posZ = ((g_ChocoboRacers[g_ChocoboViewedRacer].posZ + g_ChocoboSelectedRacerState.posZ) / 2);
        break;
    case CHOCOBO_CAMERA_EVENT_ORBIT:
        g_ChocoboCameraBaseSegment = (g_ChocoboRacers[g_ChocoboViewedRacer].segment + g_ChocoboCourseSegmentCount + -0x3C) % g_ChocoboCourseSegmentCount;
        g_ChocoboCameraTargetSegment = (g_ChocoboCameraBaseSegment + 0x78) % g_ChocoboCourseSegmentCount;
        g_ChocoboCameraEyeGoal.camPosX = g_ChocoboRacers[g_ChocoboViewedRacer].posX;
        g_ChocoboCameraEyeGoal.camPosY = g_ChocoboRacers[g_ChocoboViewedRacer].posY;
        g_ChocoboCameraEyeGoal.camPosZ = g_ChocoboRacers[g_ChocoboViewedRacer].posZ;
        g_ChocoboCameraEyeGoal.camPosY += (short)(CosFixed(g_ChocoboFrameCounter << 4) / g_ChocoboCurrentRouteEvent.orbitDivisorY + g_ChocoboCurrentRouteEvent.orbitOffsetY);
        g_ChocoboCameraEyeGoal.camPosX -= (short)(CosFixed(g_ChocoboFrameCounter << 4) / g_ChocoboCurrentRouteEvent.orbitDivisorX);
        g_ChocoboCameraEyeGoal.camPosZ -= (short)(SinFixed(g_ChocoboFrameCounter << 4) / g_ChocoboCurrentRouteEvent.segment);
        g_ChocoboRaceCameraLookGoal.posX = g_ChocoboRacers[g_ChocoboViewedRacer].posX;
        g_ChocoboRaceCameraLookGoal.posY = g_ChocoboRacers[g_ChocoboViewedRacer].posY;
        g_ChocoboRaceCameraLookGoal.posZ = g_ChocoboRacers[g_ChocoboViewedRacer].posZ;
        break;
    case CHOCOBO_CAMERA_EVENT_FINISH:
        g_ChocoboCameraBaseSegment = g_ChocoboCourseSegmentCount - 0x14;
        g_ChocoboCameraTargetSegment = 0x46;
        g_ChocoboCameraEyeGoal.camPosX = g_ChocoboRacers[g_ChocoboViewedRacer].posX;
        g_ChocoboCameraEyeGoal.camPosY = g_ChocoboRacers[g_ChocoboViewedRacer].posY;
        g_ChocoboCameraEyeGoal.camPosZ = g_ChocoboRacers[g_ChocoboViewedRacer].posZ;
        g_ChocoboCameraEyeGoal.camPosY += 800;
        g_ChocoboCameraEyeGoal.camPosX -= (short)(CosFixed(g_ChocoboFrameCounter << 4) / 6);
        g_ChocoboCameraEyeGoal.camPosZ -= (short)(SinFixed(g_ChocoboFrameCounter << 4) / 6);
        g_ChocoboRaceCameraLookGoal.posX = g_ChocoboRacers[g_ChocoboViewedRacer].posX;
        g_ChocoboRaceCameraLookGoal.posY = g_ChocoboRacers[g_ChocoboViewedRacer].posY;
        g_ChocoboRaceCameraLookGoal.posZ = g_ChocoboRacers[g_ChocoboViewedRacer].posZ;
        g_ChocoboRaceCameraLookGoal.posY += 200;
    }
    Mini_ChocoboRaceApplySmoothedCameraMatrix();
}
/*
 * Chooses the racer state the camera follows. For a CHOCOBO_CAMERA_EVENT_TRAIL route event it
 * reselects the racer state at the event's segment, at a speed set by the event's mode, once per
 * pending flag; otherwise it reselects if the viewed racer changed. When the race is over it
 * switches to the finish view (CHOCOBO_CAMERA_EVENT_FINISH) and snaps the camera.
 */
// FUNCTION: FF7 0x00771f9f
void __cdecl Mini_ChocoboRaceSelectCameraRacerState(void)
{
    int val;

    if (g_ChocoboCurrentRouteEvent.eventKind == CHOCOBO_CAMERA_EVENT_TRAIL) {
        switch (g_ChocoboCurrentRouteEvent.mode) {
        case CHOCOBO_CAMERA_TRAIL_STOPPED:
            val = 0;
            break;
        case CHOCOBO_CAMERA_TRAIL_HALF:
            val = g_ChocoboRacers[g_ChocoboViewedRacer].stepSpeed / 2;
            break;
        case CHOCOBO_CAMERA_TRAIL_SAME:
            val = g_ChocoboRacers[g_ChocoboViewedRacer].stepSpeed;
            break;
        case CHOCOBO_CAMERA_TRAIL_DOUBLE:
            val = g_ChocoboRacers[g_ChocoboViewedRacer].stepSpeed << 1;
        }
        if (g_ChocoboCameraRouteReselectPending != 0) {
            Mini_ChocoboRaceSelectRacerStateAtRoutePoint(g_ChocoboViewedRacer, val, g_ChocoboCurrentRouteEvent.segment);
            g_ChocoboCameraRouteReselectPending = 0;
        }
    } else {
        Mini_ChocoboRaceSelectRacerStateIfChanged(g_ChocoboViewedRacer);
    }
    if ((g_ChocoboRaceTimer != 0 && g_ChocoboPlayerRides == 0) || (g_ChocoboPlayerRides != 0 && g_ChocoboRacers[0].finished != 0)) {
        g_ChocoboViewedRacer = g_ChocoboRaceWinnerIndex;
        g_ChocoboCurrentRouteEvent.eventKind = CHOCOBO_CAMERA_EVENT_FINISH;
        g_ChocoboCameraSnapFlag = 1;
    }
}
/*
 * Smooths the camera eye and look-at positions toward their goals, builds the look-at matrix from
 * them, loads it into the GTE and publishes the view matrices for rendering.
 */
// FUNCTION: FF7 0x007720b9
void __cdecl Mini_ChocoboRaceApplySmoothedCameraMatrix(void)
{
    GfxEngineContext *pEngine;
    PsxMatrix mtx;
    int v[4] = {0, 0, 0};
    int q = 0;
    short w[4] = {0, 0, 0};
    if (g_ChocoboCameraSnapFlag != 0) {
        *(int *)&g_ChocoboCameraSmoothedEye = *(int *)&g_ChocoboCameraEyeGoal;
        *(int *)&g_ChocoboCameraSmoothedEye.vz = *(int *)&g_ChocoboCameraEyeGoal.camPosZ;
        *(int *)&g_ChocoboCameraSmoothedLook = *(int *)&g_ChocoboRaceCameraLookGoal;
        *(int *)&g_ChocoboCameraSmoothedLook.vz = *(int *)&g_ChocoboRaceCameraLookGoal.posZ;
        g_ChocoboCameraSnapFlag = 0;
    }
    mtx.m[0][0] = 0;
    mtx.m[0][1] = 0;
    mtx.m[0][2] = 0;
    mtx.m[1][0] = 0;
    mtx.m[1][1] = 0;
    mtx.m[1][2] = 0;
    mtx.m[2][0] = 0;
    mtx.m[2][1] = 0;
    mtx.m[2][2] = 0;
    mtx.t[0] = 0;
    mtx.t[1] = 0;
    mtx.t[2] = 0;
    g_ChocoboCameraSmoothedLook.vx = (g_ChocoboCameraSmoothedLook.vx * 3 + g_ChocoboRaceCameraLookGoal.posX) / 4;
    g_ChocoboCameraSmoothedLook.vy = (g_ChocoboCameraSmoothedLook.vy * 7 + g_ChocoboRaceCameraLookGoal.posY) / 8;
    g_ChocoboCameraSmoothedLook.vz = (g_ChocoboCameraSmoothedLook.vz * 3 + g_ChocoboRaceCameraLookGoal.posZ) / 4;
    g_ChocoboCameraSmoothedEye.vx = (g_ChocoboCameraSmoothedEye.vx * 3 + g_ChocoboCameraEyeGoal.camPosX) / 4;
    g_ChocoboCameraSmoothedEye.vy = (g_ChocoboCameraSmoothedEye.vy * 7 + g_ChocoboCameraEyeGoal.camPosY) / 8;
    g_ChocoboCameraSmoothedEye.vz = (g_ChocoboCameraSmoothedEye.vz * 3 + g_ChocoboCameraEyeGoal.camPosZ) / 4;
    Mini_ChocoboBuildLookAtMatrix(&mtx, &g_ChocoboCameraSmoothedEye, &g_ChocoboCameraSmoothedLook);
    *(PsxMatrix *)&g_ChocoboCameraLookAtMatrix = mtx;
    *(PsxMatrix *)&g_ChocoboGteMatrix = *(PsxMatrix *)&g_ChocoboCameraLookAtMatrix;
    TransposeMatrixFull(&g_ChocoboGteMatrix, (PsxMatrix *)&g_ChocoboCameraWork);
    SetRotMatrix((PsxMatrix *)&g_ChocoboCameraWork);
    SetTransMatrix(&g_ChocoboCameraWork);
    pEngine = GetEngineContext();
    Mini_ChocoboSnapshotViewMatrix(pEngine);
    GtePsxMatrixToFloat4x4((PsxMatrix *)&g_ChocoboGteMatrix, &g_ChocoboViewMatrix);
    ComposeWithScreenMatrix(&g_ChocoboViewMatrix, &g_ChocoboViewScreenMatrix);
}
/*
 * Marks a scenery slot of the track as animating: slots above 0x80 get step 0xFFFF, the others 1.
 */
// FUNCTION: FF7 0x007722f6
void __cdecl Mini_ChocoboRaceSetSceneryAnimationStep(short sceneryIndex)
{
    if (sceneryIndex > 128) {
        ((ChocoboScenerySlot *)(g_ChocoboTrackData->scenerySlots))[sceneryIndex - 0x80].animStep = 0xFFFF;
    } else {
        ((ChocoboScenerySlot *)(g_ChocoboTrackData->scenerySlots))[sceneryIndex].animStep = 1;
    }
}
} /* extern "C" */

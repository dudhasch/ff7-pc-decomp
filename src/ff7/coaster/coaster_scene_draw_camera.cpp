// src/ff7/coaster/coaster_scene_draw_camera.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Coaster init and scene drawing: sets up the camera and renderer, then draws actors, the
//   player cart, score popups, scenery, rails, the crosshair and the beam, and builds the camera
//   and track transforms.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include <time.h>
#include "ff7/api/main.h"
#include "lib/api/file.h"

extern "C" {

#define g_CoasterScorePopupVx (g_CoasterScorePopupState.coasterScorePopupVx)

// clang-format off
// GLOBAL: FF7 0x009014a8
int g_CoasterDrawThisTick = 1;
unsigned char g_rawData_009014b0[64] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00c3f870
SVECTOR *g_pCoasterCameraPath = 0;
// GLOBAL: FF7 0x00c3f874
SVECTOR *g_pCoasterRailLeft = 0;
// GLOBAL: FF7 0x00c3f878
SVECTOR *g_pCoasterRailRight = 0;
unsigned char g_bssPad_00c3f87c[4] = {0};
// GLOBAL: FF7 0x00c3f880
CoasterNode *g_pCoasterScorePopupModel = 0;
unsigned char g_bssPad_00c3f884[4] = {0};
// clang-format on

// clang-format off
// clang-format on

__inline int CoasterLookupDataEntry(int index)
{
    int ofs;
    int res;
    ofs = g_CoasterPsxDataSet.adrOffsets[index];
    if (ofs >= g_CoasterPsxDataSet.binSize)
        res = 0;
    else
        res = g_CoasterPsxDataSet.binData + ofs;
    return res;
}
/*
 * Loads the ride; called last by `Mini_CoasterEnter`. Changes to the coaster directory
 * (`GetMinigameDirForCoaster()`), saving the old one in g_CoasterSavedDirectory for
 * `Mini_CoasterShutdown` to restore, loads the data and start state (`Mini_CoasterLoadData`) and
 * creates the CoasterRenderer, which loads the TIM textures. Points g_pCoasterCameraPath at
 * entry 7 of the xbin.bin address table, takes rail sets 0 and 1 as the left and right rails,
 * starts the music (song 0x37 and the engine loop) and creates the score popup's scene object
 * (model 30 under the root node at 1200, 50, 3000, rotation 0, 1000, 0).
 */
// FUNCTION: FF7 0x005e98e0
void __cdecl Mini_CoasterInit(void)
{
    ChangeDirectorySaving(GetMinigameDirForCoaster(), (char *)&g_CoasterSavedDirectory);
    Mini_CoasterLoadData();
    g_pCoasterRenderer = new CoasterRenderer;
    g_pCoasterCameraPath = (SVECTOR *)(CoasterLookupDataEntry(7));
    Mini_CoasterSelectRailSet(0);
    g_pCoasterRailLeft = (SVECTOR *)(g_pCoasterRailSet);
    Mini_CoasterSelectRailSet(1);
    g_pCoasterRailRight = (SVECTOR *)(g_pCoasterRailSet);
    Mini_CoasterStartMusic();
    g_pCoasterScorePopupModel = (CoasterNode *)Mini_CoasterCreateSceneObject(30, &g_CoasterRootSceneNode, 1200, 50, 3000, 0, 1000, 0);
}

/*
 * Renders an actor model and performs crosshair hit-testing against it.
 */
// FUNCTION: FF7 0x005e99fb
void __cdecl Mini_CoasterDrawActorAndHitTest(CoasterNode *actor, CoasterActor *target)
{
    struct {
        short minY;
        short pad0;
        int visible;
        short minX;
        short pad1;
        short ys[6];
        int pad2;
        int model[4];
        short xs[6];
        int pad3;
        unsigned char intermediate[32];
        unsigned char matrix[32];
        short maxY;
        short pad4;
        short index;
        short pad5;
        short maxX;
        short pad6;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_CoasterCameraMatrixBuf.t[2] = 0;
    g_CoasterCameraMatrixBuf.t[1] = g_CoasterCameraMatrixBuf.t[2];
    g_CoasterCameraMatrixBuf.t[0] = g_CoasterCameraMatrixBuf.t[1];
    if (actor->parentNode != &g_CoasterRootSceneNode) {
        MultiplyMatrix(&actor->parentNode->worldMatrix, &actor->worldMatrix, &frame.intermediate[0]);
        MultiplyMatrix(&frame.intermediate[0], &g_CoasterCameraMatrixBuf, &frame.matrix[0]);
    } else {
        MultiplyMatrix(&actor->worldMatrix, &g_CoasterCameraMatrixBuf, &frame.matrix[0]);
    }
    frame.model[0] = actor->model->vertices;
    frame.model[3] = (int)actor->model;
    g_pCoasterRenderer->setMatrix(frame.matrix);
    if (g_CoasterDrawThisTick != 0) {
        g_pCoasterRenderer->drawActor(&frame.model[0]);
    }
    g_pCoasterRenderer->projectEight(target->boundingBox, &target->boundingYX[0]);
    frame.ys[0] = (target->boundingYX[0] & 0xFFFF0000) >> 0x10;
    frame.xs[0] = target->boundingYX[0] & 0xFFFF;
    frame.minX = frame.xs[0];
    frame.maxX = frame.xs[0];
    frame.minY = frame.ys[0];
    frame.maxY = frame.ys[0];
    for (frame.index = 1; frame.index < 6; frame.index++) {
        frame.ys[frame.index] = (target->boundingYX[frame.index] & 0xFFFF0000) >> 0x10;
        frame.xs[frame.index] = target->boundingYX[frame.index] & 0xFFFF;
        if (frame.minX > frame.xs[frame.index]) {
            frame.minX = frame.xs[frame.index];
        }
        if (frame.maxX < frame.xs[frame.index]) {
            frame.maxX = frame.xs[frame.index];
        }
        if (frame.minY > frame.ys[frame.index]) {
            frame.minY = frame.ys[frame.index];
        }
        if (frame.maxY < frame.ys[frame.index]) {
            frame.maxY = frame.ys[frame.index];
        }
    }
    frame.visible = Mini_CoasterPointInsideClipPlanesVec32((MFVec32 *)(frame.matrix + 18));
    if (frame.visible != 0) {
        target->state.isHit = 0;
        if (frame.maxX > g_CoasterReticleX * g_coasterSubVolumeFactor + g_CoasterClipLeft && frame.minX < g_CoasterReticleX * g_coasterSubVolumeFactor + g_CoasterClipLeft && frame.maxY > g_CoasterReticleY * g_CoasterDisplayScaleY + g_CoasterClipTop && frame.minY < g_CoasterReticleY * g_CoasterDisplayScaleY + g_CoasterClipTop && *(char *)&g_CoasterShotFlag == 1) {
            target->state.isHit = 1;
        }
    }
}

/*
 * Computes camera matrix and renders the player's coaster cart model.
 */
// FUNCTION: FF7 0x005e9cb5
void __cdecl Mini_CoasterDrawPlayerCar(CoasterNode *car)
{
    struct {
        int points;
        int model[4];
        int screen[3];
        int reserved[3];
        unsigned char intermediate[32];
        unsigned char matrix[32];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_CoasterCameraMatrixBuf.t[2] = 0;
    g_CoasterCameraMatrixBuf.t[1] = g_CoasterCameraMatrixBuf.t[2];
    g_CoasterCameraMatrixBuf.t[0] = g_CoasterCameraMatrixBuf.t[1];
    if (car->parentNode != &g_CoasterRootSceneNode) {
        MultiplyMatrix(&car->parentNode->worldMatrix, &car->worldMatrix, &frame.intermediate[0]);
        MultiplyMatrix(&frame.intermediate[0], &g_CoasterCameraMatrixBuf, &frame.matrix[0]);
    } else {
        MultiplyMatrix(&car->worldMatrix, &g_CoasterCameraMatrixBuf, &frame.matrix[0]);
    }
    frame.model[0] = car->model->vertices;
    frame.model[3] = (int)car->model;
    g_pCoasterRenderer->setMatrix(frame.matrix);
    if (g_CoasterDrawThisTick != 0) {
        g_pCoasterRenderer->drawActor(&frame.model[0]);
    }
    frame.points = g_CoasterCarModelHandle->vertices;
    g_pCoasterRenderer->projectThree((void *)frame.points, &frame.screen[0]);
    g_CoasterCarScreenY = (frame.screen[1] & 0xFFFF0000) >> 0x10;
    g_CoasterCarScreenX = frame.screen[1] & 0xFFFF;
    g_CoasterCarPoint2ScreenY = (frame.screen[2] & 0xFFFF0000) >> 0x10;
    g_CoasterCarPoint2ScreenX = frame.screen[2] & 0xFFFF;
    frame.points = g_CoasterCarModelHandle->vertices + 0x24;
    g_pCoasterRenderer->projectThree((void *)frame.points, &frame.screen[0]);
    g_CoasterShotCharge = (frame.screen[1] & 0xFFFF0000) >> 0x10;
    g_CoasterChargeCenterX = frame.screen[1] & 0xFFFF;
    g_CoasterCrosshairX = (frame.screen[2] & 0xFFFF0000) >> 0x10;
    g_CoasterCrosshairY = frame.screen[2] & 0xFFFF;
}

/*
 * Renders the 3D score popup digits in the coaster minigame.
 */
// FUNCTION: FF7 0x005e9e37
void __cdecl Mini_CoasterDrawScorePopupModel(CoasterNode *sceneObj)
{
    CoasterModelDrawArg popupArg;

    popupArg.parts = (CoasterSceneryPart *)sceneObj->model->vertices;
    popupArg.modelRecord = sceneObj->model;
    g_pCoasterRenderer->setMatrix(&sceneObj->worldMatrix);
    if (g_CoasterDrawThisTick != 0) {
        g_pCoasterRenderer->drawPopup(&popupArg);
    }
}

/*
 * Renders the stream of scenery objects along the coaster track.
 */
// FUNCTION: FF7 0x005e9e7e
void __cdecl Mini_CoasterDrawSceneryStream(void)
{
    int recordPtr;
    int sceneryBase;
    unsigned short railIdx;
    int offset;

    offset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(0xA) << 2));
    if (offset >= g_CoasterPsxDataSet.binSize) {
        sceneryBase = 0;
    } else {
        sceneryBase = g_CoasterPsxDataSet.binData + offset;
    }
    if (g_CoasterSceneryCount != 0) {
        railIdx = g_CoasterSceneryListHead;
        g_pCoasterRenderer->setMatrix((int *)&g_CoasterCameraMatrixBuf);
        do {
            recordPtr = sceneryBase + railIdx * 0x24;
            if (g_CoasterDrawThisTick != 0) {
                g_pCoasterRenderer->drawScenery((void *)recordPtr);
            }
            railIdx = g_CoasterSceneryStreamNodes[railIdx].nextIndex;
        } while (railIdx != 0xFFFF);
    }
}

/*
 * Renders the visible rail segments of the coaster track.
 */
// FUNCTION: FF7 0x005e9f33
void __cdecl Mini_CoasterDrawRailStream(void)
{
    unsigned short val;

    val = g_CoasterRailListHead;
    g_pCoasterRenderer->setMatrix((int *)&g_CoasterCameraMatrixBuf);
    do {
        if (g_CoasterDrawThisTick != 0) {
            g_pCoasterRenderer->drawRail((void *)(((int)g_pCoasterRailLeft) + val * 8), (void *)(((int)g_pCoasterRailRight) + val * 8));
        }
        val = g_CoasterRailStreamNodes[val].nextIndex;
    } while (val != 0xFFFF);
}

/*
 * Constructs the coaster camera view matrix from camera angles and position.
 */
// FUNCTION: FF7 0x005e9fb3
void __cdecl Mini_CoasterBuildCameraMatrix(PsxMatrix *m)
{
    SetIdentityPsxMatrix(m);
    m->t[0] = -g_CoasterCameraPositionXCopy;
    m->t[1] = -g_CoasterCameraPositionYCopy;
    m->t[2] = -g_CoasterCameraPositionZCopy;
}

/*
 * Multiplies rotation and camera matrices to produce output matrix.
 */
// FUNCTION: FF7 0x005e9fed
void __cdecl Mini_CoasterConcatCameraMatrix(PsxMatrix32 *rot, PsxMatrix32 *cam, PsxMatrix32 *out)
{
    struct {
        CoasterFloatVector3 translation;
        CoasterFloatVector4 transformed;
        GfxMatrix4x4 matrix;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.translation.x = (float)cam->t[0];
    local.translation.y = (float)cam->t[1];
    local.translation.z = (float)cam->t[2];
    MultiplyMatrix(rot, cam, out);
    GtePsxMatrixToFloat4x4((PsxMatrix *)out, &local.matrix);
    local.matrix._41 = local.matrix._42 = local.matrix._43 = 0;
    TransformPointByMatrix4x4(&local.matrix, &local.translation, &local.transformed);
    out->t[0] = (int)local.transformed.x + rot->t[0];
    out->t[1] = (int)local.transformed.y + rot->t[1];
    out->t[2] = (int)local.transformed.z + rot->t[2];
}

/*
 * Converts Euler rotation angles to a PSX fixed-point rotation matrix.
 */
// FUNCTION: FF7 0x005ea099
short *__cdecl Mini_CoasterEulerToPsxMatrix(short *euler, short *psxMatrix)
{
    struct {
        GfxMatrix4x4 rotationZX;
        GfxMatrix4x4 rotationX;
        GfxMatrix4x4 rotationZ;
        GfxMatrix4x4 rotationZXY;
        GfxMatrix4x4 rotationY;
    } local;

    BuildRotationXRowVec((float)euler[0] * (360.0f / 4096.0f), &local.rotationX);
    BuildRotationYRowVec((float)euler[1] * (360.0f / 4096.0f), &local.rotationY);
    BuildRotationZRowVec((float)euler[2] * (360.0f / 4096.0f), &local.rotationZ);
    MultiplyMatrix4x4Core(&local.rotationZ, &local.rotationX, &local.rotationZX);
    MultiplyMatrix4x4Core(&local.rotationZX, &local.rotationY, &local.rotationZXY);
    ClearMatrixTranslationAndW(&local.rotationZXY);
    local.rotationZXY._41 = (float)((PsxMatrix32 *)psxMatrix)->t[0];
    local.rotationZXY._42 = (float)((PsxMatrix32 *)psxMatrix)->t[1];
    local.rotationZXY._43 = (float)((PsxMatrix32 *)psxMatrix)->t[2];
    GteFloat4x4ToPsxMatrix((PsxMatrix *)psxMatrix, (float *)&local.rotationZXY);
    return psxMatrix;
}

/*
 * Samples track spline position and rotation at the specified distance.
 */
// FUNCTION: FF7 0x005ea194
void __cdecl Mini_CoasterSampleTrack(unsigned int distance, int upOffset, VECTOR *posOut, SVECTOR *rotOut)
{
    struct {
        SVECTOR *left1;
        VECTOR middle0;
        VECTOR tangent;
        VECTOR rightIncrement;
        SVECTOR *right1;
        VECTOR middleInterpolated;
        int index;
        VECTOR cross;
        SVECTOR *camera1;
        VECTOR right;
        int gammaIncrement;
        int betaIncrement;
        SVECTOR *left0;
        VECTOR middle1;
        int alphaIncrement;
        VECTOR normal;
        SVECTOR *right0;
        VECTOR binormal;
        SVECTOR *camera0;
        int fraction;
        VECTOR left;
        VECTOR leftIncrement;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.index = (distance & 0xFFFF0000) >> 16;
    local.fraction = distance & 0xFFFF;
    local.camera0 = &g_pCoasterCameraPath[local.index];
    local.camera1 = &g_pCoasterCameraPath[local.index + 1];
    local.alphaIncrement = local.camera1->vx - local.camera0->vx;
    local.betaIncrement = (local.camera1->vy - local.camera0->vy) / 32;
    local.gammaIncrement = local.camera1->vz - local.camera0->vz;
    if (local.alphaIncrement > 0x800)
        local.alphaIncrement -= 0x1000;
    if (local.betaIncrement > 0x800)
        local.betaIncrement -= 0x1000;
    if (local.gammaIncrement > 0x800)
        local.gammaIncrement -= 0x1000;
    if (local.alphaIncrement < -0x800)
        local.alphaIncrement += 0x1000;
    if (local.betaIncrement < -0x800)
        local.betaIncrement += 0x1000;
    if (local.gammaIncrement < -0x800)
        local.gammaIncrement += 0x1000;
    local.alphaIncrement = local.fraction * local.alphaIncrement;
    local.betaIncrement = local.fraction * local.betaIncrement;
    local.gammaIncrement = local.fraction * local.gammaIncrement;
    local.alphaIncrement >>= 16;
    local.betaIncrement >>= 16;
    local.gammaIncrement >>= 16;

    local.left0 = &g_pCoasterRailLeft[local.index];
    local.left1 = &g_pCoasterRailLeft[local.index + 1];
    local.leftIncrement.vx = (local.left1->vx - local.left0->vx) * local.fraction;
    local.leftIncrement.vy = (local.left1->vy - local.left0->vy) * local.fraction;
    local.leftIncrement.vz = (local.left1->vz - local.left0->vz) * local.fraction;
    local.left.vx = local.left0->vx + (short)(local.leftIncrement.vx >> 16);
    local.left.vy = local.left0->vy + (short)(local.leftIncrement.vy >> 16);
    local.left.vz = local.left0->vz + (short)(local.leftIncrement.vz >> 16);

    local.right0 = &g_pCoasterRailRight[local.index];
    local.right1 = &g_pCoasterRailRight[local.index + 1];
    local.rightIncrement.vx = (local.right1->vx - local.right0->vx) * local.fraction;
    local.rightIncrement.vy = (local.right1->vy - local.right0->vy) * local.fraction;
    local.rightIncrement.vz = (local.right1->vz - local.right0->vz) * local.fraction;
    local.right.vx = local.right0->vx + (short)(local.rightIncrement.vx >> 16);
    local.right.vy = local.right0->vy + (short)(local.rightIncrement.vy >> 16);
    local.right.vz = local.right0->vz + (short)(local.rightIncrement.vz >> 16);

    local.middleInterpolated.vx = (local.right.vx + local.left.vx) >> 1;
    local.middleInterpolated.vy = (local.right.vy + local.left.vy) >> 1;
    local.middleInterpolated.vz = (local.right.vz + local.left.vz) >> 1;
    local.middle0.vx = (local.right0->vx + local.left0->vx) >> 1;
    local.middle0.vy = (local.right0->vy + local.left0->vy) >> 1;
    local.middle0.vz = (local.right0->vz + local.left0->vz) >> 1;
    local.middle1.vx = (local.right1->vx + local.left1->vx) >> 1;
    local.middle1.vy = (local.right1->vy + local.left1->vy) >> 1;
    local.middle1.vz = (local.right1->vz + local.left1->vz) >> 1;
    local.tangent.vx = local.middle1.vx - local.middle0.vx;
    local.tangent.vy = local.middle1.vy - local.middle0.vy;
    local.tangent.vz = local.middle1.vz - local.middle0.vz;
    local.normal.vx = local.right.vx - local.left.vx;
    local.normal.vy = local.right.vy - local.left.vy;
    local.normal.vz = local.right.vz - local.left.vz;
    CrossProductInt((VECTOR *)&local.tangent, (VECTOR *)&local.normal, &local.cross);
    NormalizeVectorFixed(&local.cross, &local.binormal);

    posOut->vx = (short)local.middleInterpolated.vx + ((local.binormal.vx * upOffset) >> 12);
    posOut->vy = (short)local.middleInterpolated.vy + ((local.binormal.vy * upOffset) >> 12);
    posOut->vz = (short)local.middleInterpolated.vz + ((local.binormal.vz * upOffset) >> 12);
    rotOut->vx = local.camera0->vx + local.alphaIncrement;
    rotOut->vy = -local.camera0->vy + local.betaIncrement;
    rotOut->vz = local.camera0->vz + local.gammaIncrement;
}

/*
 * Updates the 3D position and visibility of the score popup.
 */
// FUNCTION: FF7 0x005ea5fd
void __cdecl Mini_CoasterUpdateScorePopup(short model, short x, short y, short z)
{
    PsxMatrix rotation = {0};
    PsxMatrix baseMtx;
    baseMtx.m[0][0] = 512;
    baseMtx.m[0][1] = 0;
    baseMtx.m[0][2] = 0;
    baseMtx.m[1][0] = 0;
    baseMtx.m[1][1] = 512;
    baseMtx.m[1][2] = 0;
    baseMtx.m[2][0] = 0;
    baseMtx.m[2][1] = 0;
    baseMtx.m[2][2] = 512;
    baseMtx.t[0] = 130;
    baseMtx.t[1] = 0;
    baseMtx.t[2] = 400;
    if (model != 0 && model != 91) {
        g_pCoasterScorePopupModel->model = g_CoasterModelHandles[model];
        ((SVECTOR *)&g_CoasterScorePopupVx)->vx += x;
        ((SVECTOR *)&g_CoasterScorePopupVx)->vy += y;
        ((SVECTOR *)&g_CoasterScorePopupVx)->vz += z;
        if (g_coasterFreezeState == 1) {
            RotMatrix((SVECTOR *)&g_CoasterScorePopupVx, (PsxMatrix32 *)&rotation);
            MultiplyMatrix(&rotation, &baseMtx, &g_pCoasterScorePopupModel->worldMatrix);
            Mini_CoasterDrawScorePopupModel(g_pCoasterScorePopupModel);
            if (g_CoasterDrawThisTick != 0)
                g_pCoasterRenderer->drawNumber(g_CoasterScorePopupValue, 440, 320, 0);
        }
        g_CoasterScorePopupTimer--;
        if (g_CoasterScorePopupTimer < 50) {
            if (g_coasterFreezeState == 0)
                g_coasterFreezeState = 1;
            else
                g_coasterFreezeState = 0;
        }
        if (g_CoasterScorePopupTimer == 0)
            g_CoasterKillScore = 0;
    }
}

/*
 * Renders a full-screen fade overlay with the specified alpha level.
 */
// FUNCTION: FF7 0x005ea77e
void __cdecl Mini_CoasterDrawFadeOverlay(unsigned char level)
{
    if (g_CoasterDrawThisTick)
        g_pCoasterRenderer->DrawFadeOverlay(level);
}

/*
 * Renders the player targeting crosshair on screen.
 */
// FUNCTION: FF7 0x005ea79b
void __cdecl Mini_CoasterDrawCrosshair(void)
{
    if (g_CoasterDrawThisTick != 0) {
        g_pCoasterRenderer->drawReticle();
    }
}

/*
 * Renders the laser beam shot when active.
 */
// FUNCTION: FF7 0x005ea7b4
void __cdecl Mini_CoasterDrawBeam(void)
{
    if (g_CoasterShotFlag == 1) {
        g_pCoasterRenderer->drawShot();
    }
}
} /* extern "C" */

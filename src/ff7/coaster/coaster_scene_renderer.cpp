// src/ff7/coaster/coaster_scene_renderer.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The CoasterRenderer class builds and loads the roller coaster's render assets, projects
//   vertices into screen space, and draws the coaster actors, track rails, scenery, HUD panels,
//   reticle and fade overlay.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/types/coaster.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include "ff7/api/coaster.h"
#include "ff7/api/snobo.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"
#include "ff7/api/main.h"

#pragma intrinsic(abs)
extern "C" {

#pragma intrinsic(memset)
// GLOBAL: FF7 0x00901550
CoasterRailEntry g_CoasterRailUvCoordsA[4] = {{FLOAT_ONE_BITS, 0}, {FLOAT_ONE_BITS, FLOAT_ONE_BITS}, {0, 0}, {0, FLOAT_ONE_BITS}};
// GLOBAL: FF7 0x00901570
CoasterRailEntry g_CoasterRailUvCoordsB[4] = {{0, 0}, {0, FLOAT_ONE_BITS}, {FLOAT_ONE_BITS, 0}, {FLOAT_ONE_BITS, FLOAT_ONE_BITS}};
// GLOBAL: FF7 0x00901590
CoasterRailEntry g_CoasterSceneryUvCoordsA[3] = {{FLOAT_ONE_BITS, 0}, {FLOAT_ONE_BITS, FLOAT_ONE_BITS}, {0, 0}};
// clang-format off
unsigned char g_rawData_009015a8[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f};
// clang-format on

// GLOBAL: FF7 0x009015b0
CoasterRailEntry g_CoasterSceneryUvCoordsB[3] = {{0, 0}, {0, FLOAT_ONE_BITS}, {FLOAT_ONE_BITS, 0}};
// clang-format off
unsigned char g_rawData_009015c8[8] = {0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x80, 0x3f};
// clang-format on

} // extern "C"
/*
 * Constructs a CoasterRenderer instance, allocating matrix blocks and initializing identity matrices.
 */
// FUNCTION: FF7 0x005ef4d0
CoasterRenderer::CoasterRenderer()
{
    // STRING: FF7 0x009015d0
    this->scoreTim = (GfxRenderObject *)load(12, 4, "score.tim", 128);
    // STRING: FF7 0x009015dc
    this->co1Tim = (GfxRenderObject *)load(12, 4, "co1.tim", 128);
    this->chargeObject = (GfxRenderObject *)load(10, 4, 0, 0);
    // STRING: FF7 0x009015e4
    this->waku2Tim = (GfxRenderObject *)load(12, 4, "waku2.tim", 128);
    // STRING: FF7 0x009015f0
    this->pauseTim = (GfxRenderObject *)load(12, 4, "pause.tim", 128);
    this->partBatchesBlend4 = (GfxRenderObject *)load(2, 4, 0, 0);
    this->partCallsBlend4 = (GfxRenderObject *)load(3, 4, 0, 0);
    this->partDataFirst = (GfxBatchPrimitiveCursors *)operator new(8);
    this->partBatchesBlend1 = (GfxRenderObject *)load(2, 1, 0, 0);
    this->partCallsBlend1 = (GfxRenderObject *)load(3, 1, 0, 0);
    this->partDataSecond = (GfxBatchPrimitiveCursors *)operator new(8);
    // STRING: FF7 0x009015fc
    this->railBatches = (GfxRenderObject *)load(12, 4, "rail.tim", 0);
    // STRING: FF7 0x00901608
    this->railCalls = (GfxRenderObject *)load(13, 4, "rail.tim", 0);
    this->partDataThird = (GfxBatchPrimitiveCursors *)operator new(8);
    // STRING: FF7 0x00901614
    this->reticleTim = (GfxRenderObject *)load(12, 4, "sight.tim", 128);
    // STRING: FF7 0x00901620
    this->beamTim[0] = (GfxRenderObject *)load(12, 1, "beam1.tim", 0);
    // STRING: FF7 0x0090162c
    this->beamTim[1] = (GfxRenderObject *)load(12, 1, "beam2.tim", 0);
    // STRING: FF7 0x00901638
    this->beamTim[2] = (GfxRenderObject *)load(12, 1, "beam3.tim", 0);
    this->currentBeamTim = this->beamTim[0];
    this->fadeObject = (GfxRenderObject *)load(8, 0, 0, 0x4000);
    this->scratchMatrix = this->matrices[0];
    this->projectionMatrix = this->matrices[1];
    SetIdentityMatrix4x4(this->scratchMatrix);
    SetIdentityMatrix4x4(this->projectionMatrix);
}
/*
 * Destructs a CoasterRenderer instance, releasing allocated matrix heap blocks.
 */
// FUNCTION: FF7 0x005ef728
CoasterRenderer::~CoasterRenderer()
{
    DestroyRenderObject(this->partBatchesBlend4);
    DestroyRenderObject(this->partCallsBlend4);
    DestroyRenderObject(this->partBatchesBlend1);
    DestroyRenderObject(this->partCallsBlend1);
    DestroyRenderObject(this->railBatches);
    DestroyRenderObject(this->railCalls);
    DestroyRenderObject(this->reticleTim);
    DestroyRenderObject(this->beamTim[0]);
    DestroyRenderObject(this->beamTim[1]);
    DestroyRenderObject(this->beamTim[2]);
    DestroyRenderObject(this->scoreTim);
    DestroyRenderObject(this->co1Tim);
    DestroyRenderObject(this->chargeObject);
    DestroyRenderObject(this->waku2Tim);
    DestroyRenderObject(this->pauseTim);
    DestroyRenderObject(this->fadeObject);

    void *first = this->partDataFirst;
    operator delete(first);
    void *second = this->partDataSecond;
    operator delete(second);
    void *third = this->partDataThird;
    operator delete(third);
}
/*
 * Loads texture and model data for a coaster rendering asset from disk or archive.
 */
// FUNCTION: FF7 0x005ef89d
void *CoasterRenderer::load(int kind, int blend, char *name, int flags)
{
    GfxEngineContext *pEngine = GetEngineContext();
    GfxRenderObject *result = 0;
    PfileLoadOptions loadOptions;
    memset(&loadOptions, 0, sizeof(loadOptions));
    Pfile_InitLoadOptionsWithBlend(blend, &loadOptions);
    loadOptions.directory = GetMinigameDirForCoaster();
    if (blend != 4) {
        Pfile_SetLoadOptionsBlendMode(blend, &loadOptions);
    }
    loadOptions.sourceMode = 0;
    loadOptions.file.fromArchive = 1;
    loadOptions.file.archiveId = 9;
    loadOptions.file.callback = 0;
    loadOptions.renderFlags |= flags;
    result = (GfxRenderObject *)Gfx_CreateRenderObject(1, kind, &loadOptions, name, (void *)pEngine->renderObjectBank);
    if (result != 0) {
        result->emitterFlags |= 2;
    }
    return result;
}
/*
 * Transforms and projects an array of eight bounding box vertices into screen space.
 */
// FUNCTION: FF7 0x005ef95a
void CoasterRenderer::projectEight(void *points, int *out)
{
    int ptIdx;
    float projected[4];
    for (ptIdx = 0; ptIdx < 8; ptIdx++) {
        projectPoint((short *)((char *)points + ptIdx * 8), this->projectionMatrix, projected);
        out[ptIdx] = ((int)projected[1] << 16) | ((int)projected[0] & 0xffff);
    }
}
/*
 * Transforms and projects an array of three vertices into screen space.
 */
// FUNCTION: FF7 0x005ef9ca
void CoasterRenderer::projectThree(void *points, int *out)
{
    int ptIdx;
    float projected[4];
    for (ptIdx = 0; ptIdx < 3; ptIdx++) {
        projectPoint((short *)((char *)points + ptIdx * 8), this->projectionMatrix, projected);
        out[ptIdx] = ((int)projected[1] << 16) | ((int)projected[0] & 0xffff);
    }
}
/*
 * Renders a coaster actor model, transforming vertices and submitting polygon batches.
 */
// FUNCTION: FF7 0x005efa3a
void CoasterRenderer::drawActor(void *model)
{
    short partCount;
    CoasterSceneryPart *record;
    int remaining;
    record = *(CoasterSceneryPart **)model;
    partCount = ((CoasterModelDrawArg *)model)->modelRecord->vertexCount;
    for (remaining = partCount; remaining > 0; remaining--) {
        drawSceneryParts(record, this->partBatchesBlend4, this->partCallsBlend4,
                         this->partBatchesBlend1, this->partCallsBlend1,
                         this->partDataFirst, this->partDataSecond);
        record++;
    }
}
/*
 * Renders a floating score popup model in 3D world space.
 */
// FUNCTION: FF7 0x005efaca
void CoasterRenderer::drawPopup(void *model)
{
    short partCount;
    CoasterSceneryPart *record;
    int remaining;
    record = *(CoasterSceneryPart **)model;
    partCount = ((CoasterModelDrawArg *)model)->modelRecord->vertexCount;
    for (remaining = partCount; remaining > 0; remaining--) {
        drawSceneryParts(record, this->partBatchesBlend4, this->partCallsBlend4,
                         this->partBatchesBlend1, this->partCallsBlend1,
                         this->partDataFirst, this->partDataSecond);
        record++;
    }
}
/*
 * Renders a section of roller coaster track rail with textured ribbon quads.
 */
// FUNCTION: FF7 0x005efb5a
void CoasterRenderer::drawRail(void *railStart, void *railEnd)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[216];
        int outIndex;
        struct { unsigned char pad_to_4[0x4]; CoasterBatchPrimitive *drawBuf; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *railVertH; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *railVertG; };
        struct { unsigned char pad_to_10[0x10]; GfxTlVertex *railVertF; };
        struct { unsigned char pad_to_14[0x14]; GfxTlVertex *railVertE; };
        struct { unsigned char pad_to_18[0x18]; GfxTlVertex *railVertD; };
        struct { unsigned char pad_to_1C[0x1C]; GfxTlVertex *railVertC; };
        struct { unsigned char pad_to_20[0x20]; GfxTlVertex *railVertB; };
        struct { unsigned char pad_to_24[0x24]; GfxTlVertex *railVertA; };
        struct { unsigned char pad_to_28[0x28]; GfxClipVec4 pt3; };
        struct { unsigned char pad_to_38[0x38]; GfxClipVec4 pt2; };
        struct { unsigned char pad_to_48[0x48]; GfxClipVec4 pt1; };
        struct { unsigned char pad_to_58[0x58]; GfxClipVec4 pt0; };
        struct { unsigned char pad_to_68[0x68]; float depth; };
        struct { unsigned char pad_to_6C[0x6C]; int shadeLevel; };
        struct { unsigned char pad_to_70[0x70]; float depthFactor; };
        struct { unsigned char pad_to_74[0x74]; CoasterRailEntry *uvPairs; };
        struct { unsigned char pad_to_78[0x78]; int pointShades[4]; };
        struct { unsigned char pad_to_88[0x88]; int anyInFront; };
        struct { unsigned char pad_to_8C[0x8C]; int pointIndex; };
        struct { unsigned char pad_to_90[0x90]; short *railPoints[4]; };
        struct { unsigned char pad_to_A0[0xA0]; CoasterBatchVec3 vis[4]; };
        struct { unsigned char pad_to_D0[0xD0]; int anyBehind; };
        struct { unsigned char pad_to_D4[0xD4]; int whiteColor; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.whiteColor = 0xFFFFFFFF;
    frame.uvPairs = UNFOLDED(0) ? g_CoasterRailUvCoordsA : g_CoasterRailUvCoordsB;
    frame.anyBehind = 0;
    frame.anyInFront = 0;
    frame.railPoints[0] = (short *)railStart;
    frame.railPoints[1] = (short *)railEnd + 4;
    frame.railPoints[2] = (short *)railStart + 0x20;
    frame.railPoints[3] = (short *)railEnd + 0x24;
    for (frame.pointIndex = 0; frame.pointIndex < 4; frame.pointIndex++) {
        frame.depth = this->transformDepth(frame.railPoints[frame.pointIndex], this->scratchMatrix);
        if (frame.depth < 0.0f) {
            frame.anyBehind = 1;
        }
        if (frame.depth > 0.0f) {
            frame.anyInFront = 1;
        }
        frame.shadeLevel = (int)((frame.depthFactor = this->depthFactor((float)(frame.depth))) * 255.0f);
        frame.pointShades[frame.pointIndex] = frame.shadeLevel << 0x10 | 0xFF000000 | frame.shadeLevel << 8 | frame.shadeLevel;
    }
    if (frame.anyInFront != 0) {
        if (frame.anyBehind == 0) {
            AcquireBatchNode(2, (void *)this->railBatches);
            this->projectPoint((short *)railStart, this->projectionMatrix, &frame.pt0.x);
            this->projectPoint((short *)railEnd + 4, this->projectionMatrix, &frame.pt1.x);
            this->projectPoint((short *)railStart + 0x20, this->projectionMatrix, &frame.pt2.x);
            this->projectPoint((short *)railEnd + 0x24, this->projectionMatrix, &frame.pt3.x);
            frame.railVertA = (GfxTlVertex *)this->railBatches->vertexCursor;
            frame.railVertA->xBits = frame.pt0.xBits;
            frame.railVertA->yBits = frame.pt0.yBits;
            frame.railVertA->zBits = frame.pt0.zBits;
            frame.railVertA->rhwBits = frame.pt0.wBits;
            frame.railVertA->color = frame.pointShades[0];
            frame.railVertA->specular = frame.whiteColor;
            frame.railVertA->uBits = g_CoasterRailUvCoordsA[0].texU;
            frame.railVertA->vBits = g_CoasterRailUvCoordsA[0].texV;
            frame.railVertB = (GfxTlVertex *)(this->railBatches->vertexCursor + 0x40);
            frame.railVertB->xBits = frame.pt1.xBits;
            frame.railVertB->yBits = frame.pt1.yBits;
            frame.railVertB->zBits = frame.pt1.zBits;
            frame.railVertB->rhwBits = frame.pt1.wBits;
            frame.railVertB->color = frame.pointShades[1];
            frame.railVertB->specular = frame.whiteColor;
            frame.railVertB->uBits = g_CoasterRailUvCoordsA[1].texU;
            frame.railVertB->vBits = g_CoasterRailUvCoordsA[1].texV;
            frame.railVertC = (GfxTlVertex *)(this->railBatches->vertexCursor + 0x20);
            frame.railVertC->xBits = frame.pt2.xBits;
            frame.railVertC->yBits = frame.pt2.yBits;
            frame.railVertC->zBits = frame.pt2.zBits;
            frame.railVertC->rhwBits = frame.pt2.wBits;
            frame.railVertC->color = frame.pointShades[2];
            frame.railVertC->specular = frame.whiteColor;
            frame.railVertC->uBits = g_CoasterRailUvCoordsA[2].texU;
            frame.railVertC->vBits = g_CoasterRailUvCoordsA[2].texV;
            frame.railVertD = (GfxTlVertex *)(this->railBatches->vertexCursor + 0x60);
            frame.railVertD->xBits = frame.pt3.xBits;
            frame.railVertD->yBits = frame.pt3.yBits;
            frame.railVertD->zBits = frame.pt3.zBits;
            frame.railVertD->rhwBits = frame.pt3.wBits;
            frame.railVertD->color = frame.pointShades[3];
            frame.railVertD->specular = frame.whiteColor;
            frame.railVertD->uBits = g_CoasterRailUvCoordsA[3].texU;
            frame.railVertD->vBits = g_CoasterRailUvCoordsA[3].texV;
            this->railBatches->vertexCursor += this->railBatches->cursorStep;
            frame.railVertE = (GfxTlVertex *)this->railBatches->vertexCursor;
            frame.railVertE->xBits = frame.pt0.xBits;
            frame.railVertE->yBits = frame.pt0.yBits;
            frame.railVertE->zBits = frame.pt0.zBits;
            frame.railVertE->rhwBits = frame.pt0.wBits;
            frame.railVertE->color = frame.pointShades[0];
            frame.railVertE->specular = frame.whiteColor;
            frame.railVertE->uBits = frame.uvPairs[0].texU;
            frame.railVertE->vBits = frame.uvPairs[0].texV;
            frame.railVertF = (GfxTlVertex *)(this->railBatches->vertexCursor + 0x20);
            frame.railVertF->xBits = frame.pt1.xBits;
            frame.railVertF->yBits = frame.pt1.yBits;
            frame.railVertF->zBits = frame.pt1.zBits;
            frame.railVertF->rhwBits = frame.pt1.wBits;
            frame.railVertF->color = frame.pointShades[1];
            frame.railVertF->specular = frame.whiteColor;
            frame.railVertF->uBits = frame.uvPairs[1].texU;
            frame.railVertF->vBits = frame.uvPairs[1].texV;
            frame.railVertG = (GfxTlVertex *)(this->railBatches->vertexCursor + 0x40);
            frame.railVertG->xBits = frame.pt2.xBits;
            frame.railVertG->yBits = frame.pt2.yBits;
            frame.railVertG->zBits = frame.pt2.zBits;
            frame.railVertG->rhwBits = frame.pt2.wBits;
            frame.railVertG->color = frame.pointShades[2];
            frame.railVertG->specular = frame.whiteColor;
            frame.railVertG->uBits = frame.uvPairs[2].texU;
            frame.railVertG->vBits = frame.uvPairs[2].texV;
            frame.railVertH = (GfxTlVertex *)(this->railBatches->vertexCursor + 0x60);
            frame.railVertH->xBits = frame.pt3.xBits;
            frame.railVertH->yBits = frame.pt3.yBits;
            frame.railVertH->zBits = frame.pt3.zBits;
            frame.railVertH->rhwBits = frame.pt3.wBits;
            frame.railVertH->color = frame.pointShades[3];
            frame.railVertH->specular = frame.whiteColor;
            frame.railVertH->uBits = frame.uvPairs[3].texU;
            frame.railVertH->vBits = frame.uvPairs[3].texV;
        } else {
            frame.drawBuf = 0;
            this->transformVisible((short *)railStart, this->scratchMatrix, (float *)(&frame.vis[0]));
            this->transformVisible((short *)railEnd + 4, this->scratchMatrix, (float *)(&frame.vis[1]));
            this->transformVisible((short *)railStart + 0x20, this->scratchMatrix, (float *)(&frame.vis[2]));
            this->transformVisible((short *)railEnd + 0x24, this->scratchMatrix, (float *)(&frame.vis[3]));
            Gfx_AcquireBatchPrimitiveCursors((int)this->partDataThird, (int)this->railCalls);
            SetIdentityMatrix4x4((void *)this->partDataThird->matrix);
            frame.drawBuf = (CoasterBatchPrimitive *)this->partDataThird->primitive;
            for (frame.pointIndex = 0; frame.pointIndex < 4; frame.pointIndex++) {
                frame.drawBuf->positions[frame.pointIndex].x = frame.vis[frame.pointIndex].x;
                frame.drawBuf->positions[frame.pointIndex].y = frame.vis[frame.pointIndex].y;
                frame.drawBuf->positions[frame.pointIndex].z = frame.vis[frame.pointIndex].z;
                frame.drawBuf->colors[frame.pointIndex] = frame.pointShades[frame.pointIndex];
                frame.drawBuf->rhw[frame.pointIndex] = FLOAT_HALF_BITS;
                frame.drawBuf->texCoords[frame.pointIndex].texU = frame.uvPairs[frame.pointIndex].texU;
                frame.drawBuf->texCoords[frame.pointIndex].texV = frame.uvPairs[frame.pointIndex].texV;
            }
            Gfx_AcquireBatchPrimitiveCursors((int)this->partDataThird, (int)this->railCalls);
            SetIdentityMatrix4x4((void *)this->partDataThird->matrix);
            frame.drawBuf = (CoasterBatchPrimitive *)this->partDataThird->primitive;
            for (frame.pointIndex = 0; frame.pointIndex < 4; frame.pointIndex++) {
                frame.outIndex = frame.pointIndex;
                if (frame.pointIndex == 1) {
                    frame.outIndex = 2;
                } else if (frame.pointIndex == 2) {
                    frame.outIndex = 1;
                }
                frame.drawBuf->positions[frame.outIndex].x = frame.vis[frame.pointIndex].x;
                frame.drawBuf->positions[frame.outIndex].y = frame.vis[frame.pointIndex].y;
                frame.drawBuf->positions[frame.outIndex].z = frame.vis[frame.pointIndex].z;
                frame.drawBuf->colors[frame.outIndex] = frame.pointShades[frame.pointIndex];
                frame.drawBuf->rhw[frame.outIndex] = FLOAT_HALF_BITS;
                frame.drawBuf->texCoords[frame.outIndex].texU = g_CoasterRailUvCoordsA[frame.pointIndex].texU;
                frame.drawBuf->texCoords[frame.outIndex].texV = g_CoasterRailUvCoordsA[frame.pointIndex].texV;
            }
        }
    }
}
/*
 * High-level wrapper that forwards scenery model rendering to drawSceneryParts.
 */
// FUNCTION: FF7 0x005f0482
void CoasterRenderer::drawScenery(void *model)
{
    drawSceneryParts(model,
                     this->partBatchesBlend4,
                     this->partCallsBlend4,
                     this->partBatchesBlend1,
                     this->partCallsBlend1,
                     this->partDataFirst,
                     this->partDataSecond);
}
/*
 * Renders coaster scenery components with texture mapping and perspective projection.
 */
// FUNCTION: FF7 0x005f04d7
void CoasterRenderer::drawSceneryParts(void *part, void *nodeA, void *renderObj, void *nodeB, void *altRenderObj, void *cursors, void *altCursors)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[172];
        GfxTlVertex *sceneryVertC;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *sceneryVertB; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *sceneryVertA; };
        struct { unsigned char pad_to_C[0xC]; GfxClipVec4 pt2; };
        struct { unsigned char pad_to_1C[0x1C]; GfxClipVec4 pt1; };
        struct { unsigned char pad_to_2C[0x2C]; GfxClipVec4 pt0; };
        struct { unsigned char pad_to_3C[0x3C]; float depth; };
        struct { unsigned char pad_to_40[0x40]; CoasterRailEntry *uvEntries; };
        struct { unsigned char pad_to_44[0x44]; GfxRenderObject *renderObj; };
        struct { unsigned char pad_to_48[0x48]; int anyInFront; };
        struct { unsigned char pad_to_4C[0x4C]; int pointIndex; };
        struct { unsigned char pad_to_50[0x50]; GfxBatchPrimitiveCursors *cursor; };
        struct { unsigned char pad_to_54[0x54]; GfxRenderObject *batchNode; };
        struct { unsigned char pad_to_58[0x58]; short *partPts[3]; };
        struct { unsigned char pad_to_64[0x64]; CoasterBatchVec3 vis[3]; };
        struct { unsigned char pad_to_88[0x88]; int shades[3]; };
        struct { unsigned char pad_to_94[0x94]; CoasterBatchPrimitive *drawBuf; };
        struct { unsigned char pad_to_98[0x98]; int anyBehind; };
        struct { unsigned char pad_to_9C[0x9C]; float depthFactors[3]; };
        struct { unsigned char pad_to_A8[0xA8]; int whiteColor; };
    } frame;
    frame.whiteColor = 0xFFFFFFFF;
    frame.anyBehind = 0;
    frame.anyInFront = 0;
    frame.uvEntries = UNFOLDED(0) ? g_CoasterSceneryUvCoordsA : g_CoasterSceneryUvCoordsB;
    frame.drawBuf = 0;
    frame.partPts[0] = (short *)part;
    frame.partPts[1] = (short *)part + 4;
    frame.partPts[2] = (short *)part + 8;
    for (frame.pointIndex = 0; frame.pointIndex < 3; frame.pointIndex++) {
        frame.depth = this->transformDepth(frame.partPts[frame.pointIndex], this->scratchMatrix);
        if (frame.depth < -1e+02f) {
            frame.anyBehind = 1;
        }
        if (frame.depth > 0.0f) {
            frame.anyInFront = 1;
        }
        frame.depthFactors[frame.pointIndex] = this->depthFactor((float)(frame.depth));
    }
    if (frame.anyInFront != 0) {
        if ((((unsigned char *)part)[0x1B] & 2) != 0) {
            frame.batchNode = (GfxRenderObject *)nodeB;
            frame.renderObj = (GfxRenderObject *)altRenderObj;
            frame.cursor = (GfxBatchPrimitiveCursors *)altCursors;
        } else {
            frame.batchNode = (GfxRenderObject *)nodeA;
            frame.renderObj = (GfxRenderObject *)renderObj;
            frame.cursor = (GfxBatchPrimitiveCursors *)cursors;
        }
        if (frame.anyBehind == 0) {
            AcquireBatchNode(1, frame.batchNode);
            this->projectPoint((short *)part, this->projectionMatrix, &frame.pt0.x);
            this->projectPoint((short *)part + 4, this->projectionMatrix, &frame.pt1.x);
            this->projectPoint((short *)part + 8, this->projectionMatrix, &frame.pt2.x);
            frame.shades[0] = this->shadeColor((int *)part + 6, frame.depthFactors[0]);
            frame.shades[1] = this->shadeColor((int *)part + 7, frame.depthFactors[1]);
            frame.shades[2] = this->shadeColor((int *)part + 8, frame.depthFactors[2]);
            frame.sceneryVertA = (GfxTlVertex *)frame.batchNode->vertexCursor;
            frame.sceneryVertA->xBits = frame.pt0.xBits;
            frame.sceneryVertA->yBits = frame.pt0.yBits;
            frame.sceneryVertA->zBits = frame.pt0.zBits;
            frame.sceneryVertA->rhwBits = frame.pt0.wBits;
            frame.sceneryVertA->color = frame.shades[0];
            frame.sceneryVertA->specular = frame.whiteColor;
            frame.sceneryVertA->uBits = 0;
            frame.sceneryVertA->vBits = 0;
            frame.sceneryVertB = (GfxTlVertex *)(frame.batchNode->vertexCursor + 0x40);
            frame.sceneryVertB->xBits = frame.pt1.xBits;
            frame.sceneryVertB->yBits = frame.pt1.yBits;
            frame.sceneryVertB->zBits = frame.pt1.zBits;
            frame.sceneryVertB->rhwBits = frame.pt1.wBits;
            frame.sceneryVertB->color = frame.shades[1];
            frame.sceneryVertB->specular = frame.whiteColor;
            frame.sceneryVertB->uBits = 0;
            frame.sceneryVertB->vBits = 0;
            frame.sceneryVertC = (GfxTlVertex *)(frame.batchNode->vertexCursor + 0x20);
            frame.sceneryVertC->xBits = frame.pt2.xBits;
            frame.sceneryVertC->yBits = frame.pt2.yBits;
            frame.sceneryVertC->zBits = frame.pt2.zBits;
            frame.sceneryVertC->rhwBits = frame.pt2.wBits;
            frame.sceneryVertC->color = frame.shades[2];
            frame.sceneryVertC->specular = frame.whiteColor;
            frame.sceneryVertC->uBits = 0;
            frame.sceneryVertC->vBits = 0;
        } else {
            this->transformVisible((short *)part, this->scratchMatrix, (float *)(&frame.vis[0]));
            this->transformVisible((short *)part + 4, this->scratchMatrix, (float *)(&frame.vis[2]));
            this->transformVisible((short *)part + 8, this->scratchMatrix, (float *)(&frame.vis[1]));
            frame.shades[0] = this->shadeColor((int *)part + 6, frame.depthFactors[0]);
            frame.shades[2] = this->shadeColor((int *)part + 7, frame.depthFactors[1]);
            frame.shades[1] = this->shadeColor((int *)part + 8, frame.depthFactors[2]);
            Gfx_AcquireBatchPrimitiveCursors((int)frame.cursor, (int)frame.renderObj);
            SetIdentityMatrix4x4((void *)frame.cursor->matrix);
            frame.drawBuf = (CoasterBatchPrimitive *)frame.cursor->primitive;
            for (frame.pointIndex = 0; frame.pointIndex < 3; frame.pointIndex++) {
                frame.drawBuf->positions[frame.pointIndex].x = frame.vis[frame.pointIndex].x;
                frame.drawBuf->positions[frame.pointIndex].y = frame.vis[frame.pointIndex].y;
                frame.drawBuf->positions[frame.pointIndex].z = frame.vis[frame.pointIndex].z;
                frame.drawBuf->colors[frame.pointIndex] = frame.shades[frame.pointIndex];
                frame.drawBuf->rhw[frame.pointIndex] = FLOAT_HALF_BITS;
                frame.drawBuf->texCoords[frame.pointIndex].texU = frame.uvEntries[frame.pointIndex].texU;
                frame.drawBuf->texCoords[frame.pointIndex].texV = frame.uvEntries[frame.pointIndex].texV;
            }
        }
    }
}
/*
 * Renders the 2D targeting crosshair reticle on the HUD.
 */
// FUNCTION: FF7 0x005f09c4
void CoasterRenderer::drawReticle()
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        struct { unsigned char pad_to_10[0x10]; int whiteColor; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *reticleVertA; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *reticleVertB; };
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *reticleVertC; };
        GfxTlVertex *reticleVertD;
    } frame;
    frame.whiteColor = 0xFFFFFFFF;
    if (AcquireBatchNode(1, (void *)this->reticleTim) != 0) {
        frame.reticleVertA = (GfxTlVertex *)this->reticleTim->vertexCursor;
        frame.reticleVertA->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor - (g_CoasterReticleScaledSize / 2)) + g_CoasterClipLeft;
        frame.reticleVertA->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY - (g_CoasterReticleScaledSize / 2)) + g_CoasterClipTop;
        frame.reticleVertA->zBits = 0x3F7CA1AD;
        frame.reticleVertA->rhwBits = FLOAT_ONE_BITS;
        frame.reticleVertA->color = frame.whiteColor;
        frame.reticleVertA->specular = 0xFF000000;
        frame.reticleVertA->uBits = 0;
        frame.reticleVertA->vBits = 0;
        frame.reticleVertB = (GfxTlVertex *)(this->reticleTim->vertexCursor + 0x20);
        frame.reticleVertB->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor - (g_CoasterReticleScaledSize / 2)) + g_CoasterClipLeft;
        frame.reticleVertB->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY + (g_CoasterReticleScaledSize / 2)) + g_CoasterClipTop;
        frame.reticleVertB->zBits = 0x3F7CA1AD;
        frame.reticleVertB->rhwBits = FLOAT_ONE_BITS;
        frame.reticleVertB->color = frame.whiteColor;
        frame.reticleVertB->specular = 0xFF000000;
        frame.reticleVertB->uBits = 0;
        frame.reticleVertB->vBits = FLOAT_ONE_BITS;
        frame.reticleVertC = (GfxTlVertex *)(this->reticleTim->vertexCursor + 0x40);
        frame.reticleVertC->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor + (g_CoasterReticleScaledSize / 2)) + g_CoasterClipLeft;
        frame.reticleVertC->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY - (g_CoasterReticleScaledSize / 2)) + g_CoasterClipTop;
        frame.reticleVertC->zBits = 0x3F7CA1AD;
        frame.reticleVertC->rhwBits = FLOAT_ONE_BITS;
        frame.reticleVertC->color = frame.whiteColor;
        frame.reticleVertC->specular = 0xFF000000;
        frame.reticleVertC->uBits = FLOAT_ONE_BITS;
        frame.reticleVertC->vBits = 0;
        frame.reticleVertD = (GfxTlVertex *)(this->reticleTim->vertexCursor + 0x60);
        frame.reticleVertD->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor + (g_CoasterReticleScaledSize / 2)) + g_CoasterClipLeft;
        frame.reticleVertD->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY + (g_CoasterReticleScaledSize / 2)) + g_CoasterClipTop;
        frame.reticleVertD->zBits = 0x3F7CA1AD;
        frame.reticleVertD->rhwBits = FLOAT_ONE_BITS;
        frame.reticleVertD->color = frame.whiteColor;
        frame.reticleVertD->specular = 0xFF000000;
        frame.reticleVertD->uBits = FLOAT_ONE_BITS;
        frame.reticleVertD->vBits = FLOAT_ONE_BITS;
    }
}
/*
 * Renders laser beam firing effects and projectile visuals.
 */
// FUNCTION: FF7 0x005f0c81
void CoasterRenderer::drawShot()
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        struct { unsigned char pad_to_44[0x44]; int vertexColor; };
        struct { unsigned char pad_to_40[0x40]; int xOffset; };
        struct { unsigned char pad_to_3C[0x3C]; GfxTlVertex *xPlusVtx1; };
        struct { unsigned char pad_to_38[0x38]; GfxTlVertex *beamVtx1; };
        struct { unsigned char pad_to_34[0x34]; GfxTlVertex *xMinusVtx1; };
        struct { unsigned char pad_to_30[0x30]; GfxTlVertex *beamVtx2; };
        struct { unsigned char pad_to_2C[0x2C]; GfxTlVertex *xPlusVtx2; };
        struct { unsigned char pad_to_28[0x28]; GfxTlVertex *beamVtx3; };
        struct { unsigned char pad_to_24[0x24]; GfxTlVertex *xMinusVtx2; };
        struct { unsigned char pad_to_20[0x20]; GfxTlVertex *beamVtx4; };
        struct { unsigned char pad_to_1C[0x1C]; GfxTlVertex *chargePlusVtx1; };
        struct { unsigned char pad_to_18[0x18]; GfxTlVertex *beamVtx5; };
        struct { unsigned char pad_to_14[0x14]; GfxTlVertex *chargeMinusVtx1; };
        struct { unsigned char pad_to_10[0x10]; GfxTlVertex *beamVtx6; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *chargePlusVtx2; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *beamVtx7; };
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *chargeMinusVtx2; };
        GfxTlVertex *beamVtx8;
    } frame;
    frame.vertexColor = 0xFFFFFFFF;
    this->currentBeamTim = this->beamTim[g_CoasterBeamIndex];
    frame.xOffset = g_CoasterBeamCharge >> 3;
    InvalidateBatchGeneration((void *)this->currentBeamTim);
    if (AcquireBatchNode(4, (void *)this->currentBeamTim) != 0) {
        frame.xPlusVtx1 = (GfxTlVertex *)this->currentBeamTim->vertexCursor;
        frame.xPlusVtx1->x = (float)g_CoasterCarScreenX + frame.xOffset;
        frame.xPlusVtx1->y = (float)g_CoasterCarScreenY;
        frame.xPlusVtx1->zBits = 0x3F7CA1AD;
        frame.xPlusVtx1->rhwBits = FLOAT_ONE_BITS;
        frame.xPlusVtx1->color = frame.vertexColor;
        frame.xPlusVtx1->specular = 0xFF000000;
        frame.xPlusVtx1->uBits = 0;
        frame.xPlusVtx1->vBits = 0;
        frame.beamVtx1 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x20);
        frame.beamVtx1->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor) + g_CoasterClipLeft;
        frame.beamVtx1->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY) + g_CoasterClipTop;
        frame.beamVtx1->zBits = 0x3F7CA1AD;
        frame.beamVtx1->rhwBits = FLOAT_ONE_BITS;
        frame.beamVtx1->color = frame.vertexColor;
        frame.beamVtx1->specular = 0xFF000000;
        frame.beamVtx1->uBits = 0;
        frame.beamVtx1->vBits = FLOAT_ONE_BITS;
        frame.xMinusVtx1 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x40);
        frame.xMinusVtx1->x = (float)g_CoasterCarScreenX - frame.xOffset;
        frame.xMinusVtx1->y = (float)g_CoasterCarScreenY;
        frame.xMinusVtx1->zBits = 0x3F7CA1AD;
        frame.xMinusVtx1->rhwBits = FLOAT_ONE_BITS;
        frame.xMinusVtx1->color = frame.vertexColor;
        frame.xMinusVtx1->specular = 0xFF000000;
        frame.xMinusVtx1->uBits = FLOAT_ONE_BITS;
        frame.xMinusVtx1->vBits = 0;
        frame.beamVtx2 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x60);
        frame.beamVtx2->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor) + g_CoasterClipLeft;
        frame.beamVtx2->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY) + g_CoasterClipTop;
        frame.beamVtx2->zBits = 0x3F7CA1AD;
        frame.beamVtx2->rhwBits = FLOAT_ONE_BITS;
        frame.beamVtx2->color = frame.vertexColor;
        frame.beamVtx2->specular = 0xFF000000;
        frame.beamVtx2->uBits = FLOAT_ONE_BITS;
        frame.beamVtx2->vBits = FLOAT_ONE_BITS;
        this->currentBeamTim->vertexCursor += this->currentBeamTim->cursorStep;
        frame.xPlusVtx2 = (GfxTlVertex *)this->currentBeamTim->vertexCursor;
        frame.xPlusVtx2->x = (float)g_CoasterCarScreenX + frame.xOffset;
        frame.xPlusVtx2->y = (float)g_CoasterCarScreenY;
        frame.xPlusVtx2->zBits = 0x3F7CA1AD;
        frame.xPlusVtx2->rhwBits = FLOAT_ONE_BITS;
        frame.xPlusVtx2->color = frame.vertexColor;
        frame.xPlusVtx2->specular = 0xFF000000;
        frame.xPlusVtx2->uBits = 0;
        frame.xPlusVtx2->vBits = 0;
        frame.beamVtx3 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x40);
        frame.beamVtx3->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor) + g_CoasterClipLeft;
        frame.beamVtx3->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY) + g_CoasterClipTop;
        frame.beamVtx3->zBits = 0x3F7CA1AD;
        frame.beamVtx3->rhwBits = FLOAT_ONE_BITS;
        frame.beamVtx3->color = frame.vertexColor;
        frame.beamVtx3->specular = 0xFF000000;
        frame.beamVtx3->uBits = 0;
        frame.beamVtx3->vBits = FLOAT_ONE_BITS;
        frame.xMinusVtx2 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x20);
        frame.xMinusVtx2->x = (float)g_CoasterCarScreenX - frame.xOffset;
        frame.xMinusVtx2->y = (float)g_CoasterCarScreenY;
        frame.xMinusVtx2->zBits = 0x3F7CA1AD;
        frame.xMinusVtx2->rhwBits = FLOAT_ONE_BITS;
        frame.xMinusVtx2->color = frame.vertexColor;
        frame.xMinusVtx2->specular = 0xFF000000;
        frame.xMinusVtx2->uBits = FLOAT_ONE_BITS;
        frame.xMinusVtx2->vBits = 0;
        frame.beamVtx4 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x60);
        frame.beamVtx4->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor) + g_CoasterClipLeft;
        frame.beamVtx4->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY) + g_CoasterClipTop;
        frame.beamVtx4->zBits = 0x3F7CA1AD;
        frame.beamVtx4->rhwBits = FLOAT_ONE_BITS;
        frame.beamVtx4->color = frame.vertexColor;
        frame.beamVtx4->specular = 0xFF000000;
        frame.beamVtx4->uBits = FLOAT_ONE_BITS;
        frame.beamVtx4->vBits = FLOAT_ONE_BITS;
        this->currentBeamTim->vertexCursor += this->currentBeamTim->cursorStep;
        frame.chargePlusVtx1 = (GfxTlVertex *)this->currentBeamTim->vertexCursor;
        frame.chargePlusVtx1->x = (float)g_CoasterChargeCenterX + frame.xOffset;
        frame.chargePlusVtx1->y = (float)g_CoasterShotCharge;
        frame.chargePlusVtx1->zBits = 0x3F7CA1AD;
        frame.chargePlusVtx1->rhwBits = FLOAT_ONE_BITS;
        frame.chargePlusVtx1->color = frame.vertexColor;
        frame.chargePlusVtx1->specular = 0xFF000000;
        frame.chargePlusVtx1->uBits = 0;
        frame.chargePlusVtx1->vBits = 0;
        frame.beamVtx5 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x20);
        frame.beamVtx5->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor) + g_CoasterClipLeft;
        frame.beamVtx5->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY) + g_CoasterClipTop;
        frame.beamVtx5->zBits = 0x3F7CA1AD;
        frame.beamVtx5->rhwBits = FLOAT_ONE_BITS;
        frame.beamVtx5->color = frame.vertexColor;
        frame.beamVtx5->specular = 0xFF000000;
        frame.beamVtx5->uBits = 0;
        frame.beamVtx5->vBits = FLOAT_ONE_BITS;
        frame.chargeMinusVtx1 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x40);
        frame.chargeMinusVtx1->x = (float)g_CoasterChargeCenterX - frame.xOffset;
        frame.chargeMinusVtx1->y = (float)g_CoasterShotCharge;
        frame.chargeMinusVtx1->zBits = 0x3F7CA1AD;
        frame.chargeMinusVtx1->rhwBits = FLOAT_ONE_BITS;
        frame.chargeMinusVtx1->color = frame.vertexColor;
        frame.chargeMinusVtx1->specular = 0xFF000000;
        frame.chargeMinusVtx1->uBits = FLOAT_ONE_BITS;
        frame.chargeMinusVtx1->vBits = 0;
        frame.beamVtx6 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x60);
        frame.beamVtx6->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor) + g_CoasterClipLeft;
        frame.beamVtx6->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY) + g_CoasterClipTop;
        frame.beamVtx6->zBits = 0x3F7CA1AD;
        frame.beamVtx6->rhwBits = FLOAT_ONE_BITS;
        frame.beamVtx6->color = frame.vertexColor;
        frame.beamVtx6->specular = 0xFF000000;
        frame.beamVtx6->uBits = FLOAT_ONE_BITS;
        frame.beamVtx6->vBits = FLOAT_ONE_BITS;
        this->currentBeamTim->vertexCursor += this->currentBeamTim->cursorStep;
        frame.chargePlusVtx2 = (GfxTlVertex *)this->currentBeamTim->vertexCursor;
        frame.chargePlusVtx2->x = (float)g_CoasterChargeCenterX + frame.xOffset;
        frame.chargePlusVtx2->y = (float)g_CoasterShotCharge;
        frame.chargePlusVtx2->zBits = 0x3F7CA1AD;
        frame.chargePlusVtx2->rhwBits = FLOAT_ONE_BITS;
        frame.chargePlusVtx2->color = frame.vertexColor;
        frame.chargePlusVtx2->specular = 0xFF000000;
        frame.chargePlusVtx2->uBits = 0;
        frame.chargePlusVtx2->vBits = 0;
        frame.beamVtx7 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x40);
        frame.beamVtx7->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor) + g_CoasterClipLeft;
        frame.beamVtx7->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY) + g_CoasterClipTop;
        frame.beamVtx7->zBits = 0x3F7CA1AD;
        frame.beamVtx7->rhwBits = FLOAT_ONE_BITS;
        frame.beamVtx7->color = frame.vertexColor;
        frame.beamVtx7->specular = 0xFF000000;
        frame.beamVtx7->uBits = 0;
        frame.beamVtx7->vBits = FLOAT_ONE_BITS;
        frame.chargeMinusVtx2 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x20);
        frame.chargeMinusVtx2->x = (float)g_CoasterChargeCenterX - frame.xOffset;
        frame.chargeMinusVtx2->y = (float)g_CoasterShotCharge;
        frame.chargeMinusVtx2->zBits = 0x3F7CA1AD;
        frame.chargeMinusVtx2->rhwBits = FLOAT_ONE_BITS;
        frame.chargeMinusVtx2->color = frame.vertexColor;
        frame.chargeMinusVtx2->specular = 0xFF000000;
        frame.chargeMinusVtx2->uBits = FLOAT_ONE_BITS;
        frame.chargeMinusVtx2->vBits = 0;
        frame.beamVtx8 = (GfxTlVertex *)(this->currentBeamTim->vertexCursor + 0x60);
        frame.beamVtx8->x = (float)(g_CoasterReticleX * g_coasterSubVolumeFactor) + g_CoasterClipLeft;
        frame.beamVtx8->y = (float)(g_CoasterReticleY * g_CoasterDisplayScaleY) + g_CoasterClipTop;
        frame.beamVtx8->zBits = 0x3F7CA1AD;
        frame.beamVtx8->rhwBits = FLOAT_ONE_BITS;
        frame.beamVtx8->color = frame.vertexColor;
        frame.beamVtx8->specular = 0xFF000000;
        frame.beamVtx8->uBits = FLOAT_ONE_BITS;
        frame.beamVtx8->vBits = FLOAT_ONE_BITS;
    }
}
/*
 * Renders laser weapon charge gauge HUD element.
 */
// FUNCTION: FF7 0x005f15c7
void CoasterRenderer::drawCharge()
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        struct { unsigned char pad_to_14[0x14]; int chargeLevel; };
        struct { unsigned char pad_to_10[0x10]; int fillY; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *fillLeftVtx; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *baseLeftVtx; };
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *fillRightVtx; };
        GfxTlVertex *baseRightVtx;
    } frame;
    frame.chargeLevel = g_CoasterBeamCharge << 7 >> 7;
    frame.fillY = g_CoasterDisplayScaleY * 0xDC - g_CoasterBeamCharge * g_CoasterDisplayScaleY + g_CoasterClipTop;
    if (AcquireBatchNode(1, (void *)this->chargeObject) != 0) {
        frame.fillLeftVtx = (GfxTlVertex *)this->chargeObject->vertexCursor;
        frame.fillLeftVtx->x = (float)(g_coasterSubVolumeFactor * 0x14) + g_CoasterClipLeft;
        frame.fillLeftVtx->y = (float)frame.fillY;
        frame.fillLeftVtx->zBits = 0;
        frame.fillLeftVtx->rhwBits = FLOAT_ONE_BITS;
        frame.fillLeftVtx->color = 0x80 - frame.chargeLevel << 0x10 | 0xFF000000 | frame.chargeLevel << 8;
        frame.fillLeftVtx->specular = 0xFF000000;
        frame.fillLeftVtx->uBits = 0;
        frame.fillLeftVtx->vBits = 0;
        frame.baseLeftVtx = (GfxTlVertex *)(this->chargeObject->vertexCursor + 0x20);
        frame.baseLeftVtx->x = (float)(g_coasterSubVolumeFactor * 0x14) + g_CoasterClipLeft;
        frame.baseLeftVtx->y = (float)(g_CoasterDisplayScaleY * 0xDC) + g_CoasterClipTop;
        frame.baseLeftVtx->zBits = 0;
        frame.baseLeftVtx->rhwBits = FLOAT_ONE_BITS;
        frame.baseLeftVtx->color = 0xFF800000;
        frame.baseLeftVtx->specular = 0xFF000000;
        frame.baseLeftVtx->uBits = 0;
        frame.baseLeftVtx->vBits = 0;
        frame.fillRightVtx = (GfxTlVertex *)(this->chargeObject->vertexCursor + 0x40);
        frame.fillRightVtx->x = (float)(g_coasterSubVolumeFactor * 0x1C) + g_CoasterClipLeft;
        frame.fillRightVtx->y = (float)frame.fillY;
        frame.fillRightVtx->zBits = 0;
        frame.fillRightVtx->rhwBits = FLOAT_ONE_BITS;
        frame.fillRightVtx->color = 0x80 - frame.chargeLevel << 0x10 | 0xFF000000 | frame.chargeLevel << 8;
        frame.fillRightVtx->specular = 0xFF000000;
        frame.fillRightVtx->uBits = 0;
        frame.fillRightVtx->vBits = 0;
        frame.baseRightVtx = (GfxTlVertex *)(this->chargeObject->vertexCursor + 0x60);
        frame.baseRightVtx->x = (float)(g_coasterSubVolumeFactor * 0x1C) + g_CoasterClipLeft;
        frame.baseRightVtx->y = (float)(g_CoasterDisplayScaleY * 0xDC) + g_CoasterClipTop;
        frame.baseRightVtx->zBits = 0;
        frame.baseRightVtx->rhwBits = FLOAT_ONE_BITS;
        frame.baseRightVtx->color = 0xFF800000;
        frame.baseRightVtx->specular = 0xFF000000;
        frame.baseRightVtx->uBits = 0;
        frame.baseRightVtx->vBits = 0;
    }
}
/*
 * Renders coaster ride HUD panel component.
 */
// FUNCTION: FF7 0x005f1838
void CoasterRenderer::drawPanel5F1838()
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        struct { unsigned char pad_to_10[0x10]; int panelColor; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *topLeftVtx; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *bottomLeftVtx; };
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *topRightVtx; };
        GfxTlVertex *bottomRightVtx;
    } frame;
    frame.panelColor = 0xFFFFFFFF;
    if (AcquireBatchNode(1, (void *)this->waku2Tim) != 0) {
        frame.topLeftVtx = (GfxTlVertex *)this->waku2Tim->vertexCursor;
        frame.topLeftVtx->x = (float)(g_coasterSubVolumeFactor * 0x12) + g_CoasterClipLeft;
        frame.topLeftVtx->y = (float)(g_CoasterDisplayScaleY * 0x56) + g_CoasterClipTop;
        frame.topLeftVtx->zBits = 0;
        frame.topLeftVtx->rhwBits = FLOAT_ONE_BITS;
        frame.topLeftVtx->color = frame.panelColor;
        frame.topLeftVtx->specular = 0xFF000000;
        frame.topLeftVtx->uBits = 0;
        frame.topLeftVtx->vBits = 0;
        frame.bottomLeftVtx = (GfxTlVertex *)(this->waku2Tim->vertexCursor + 0x20);
        frame.bottomLeftVtx->x = (float)(g_coasterSubVolumeFactor * 0x12) + g_CoasterClipLeft;
        frame.bottomLeftVtx->y = (float)(g_CoasterDisplayScaleY * 0xE2) + g_CoasterClipTop;
        frame.bottomLeftVtx->zBits = 0;
        frame.bottomLeftVtx->rhwBits = FLOAT_ONE_BITS;
        frame.bottomLeftVtx->color = frame.panelColor;
        frame.bottomLeftVtx->specular = 0xFF000000;
        frame.bottomLeftVtx->uBits = 0;
        frame.bottomLeftVtx->vBits = FLOAT_ONE_BITS;
        frame.topRightVtx = (GfxTlVertex *)(this->waku2Tim->vertexCursor + 0x40);
        frame.topRightVtx->x = (float)(g_coasterSubVolumeFactor * 0x1E) + g_CoasterClipLeft;
        frame.topRightVtx->y = (float)(g_CoasterDisplayScaleY * 0x56) + g_CoasterClipTop;
        frame.topRightVtx->zBits = 0;
        frame.topRightVtx->rhwBits = FLOAT_ONE_BITS;
        frame.topRightVtx->color = frame.panelColor;
        frame.topRightVtx->specular = 0xFF000000;
        frame.topRightVtx->uBits = FLOAT_ONE_BITS;
        frame.topRightVtx->vBits = 0;
        frame.bottomRightVtx = (GfxTlVertex *)(this->waku2Tim->vertexCursor + 0x60);
        frame.bottomRightVtx->x = (float)(g_coasterSubVolumeFactor * 0x1E) + g_CoasterClipLeft;
        frame.bottomRightVtx->y = (float)(g_CoasterDisplayScaleY * 0xE2) + g_CoasterClipTop;
        frame.bottomRightVtx->zBits = 0;
        frame.bottomRightVtx->rhwBits = FLOAT_ONE_BITS;
        frame.bottomRightVtx->color = frame.panelColor;
        frame.bottomRightVtx->specular = 0xFF000000;
        frame.bottomRightVtx->uBits = FLOAT_ONE_BITS;
        frame.bottomRightVtx->vBits = FLOAT_ONE_BITS;
    }
}
/*
 * Renders coaster ride HUD overlay element.
 */
// FUNCTION: FF7 0x005f1a71
void CoasterRenderer::drawPanel5F1A71()
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        struct { unsigned char pad_to_18[0x18]; int leftX; };
        struct { unsigned char pad_to_14[0x14]; int panelColor; };
        struct { unsigned char pad_to_10[0x10]; int rightX; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *topLeftVtx; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *bottomLeftVtx; };
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *topRightVtx; };
        GfxTlVertex *bottomRightVtx;
    } frame;
    frame.leftX = g_coasterSubVolumeFactor * 0xC8 + g_CoasterClipLeft;
    frame.rightX = g_CoasterDisplayScaleY * 0x137 + g_CoasterClipLeft;
    if (g_CoasterScore >= 0x270F) {
        frame.rightX += g_coasterSubVolumeFactor * 0xF + g_CoasterClipLeft;
    }
    frame.panelColor = 0xFFFFFFFF;
    if (AcquireBatchNode(1, (void *)this->co1Tim) != 0) {
        frame.topLeftVtx = (GfxTlVertex *)this->co1Tim->vertexCursor;
        frame.topLeftVtx->x = (float)frame.leftX;
        frame.topLeftVtx->y = (float)(g_CoasterDisplayScaleY * 0xC0) + g_CoasterClipTop;
        frame.topLeftVtx->zBits = 0;
        frame.topLeftVtx->rhwBits = FLOAT_ONE_BITS;
        frame.topLeftVtx->color = frame.panelColor;
        frame.topLeftVtx->specular = 0xFF000000;
        frame.topLeftVtx->uBits = 0;
        frame.topLeftVtx->vBits = 0;
        frame.bottomLeftVtx = (GfxTlVertex *)(this->co1Tim->vertexCursor + 0x20);
        frame.bottomLeftVtx->x = (float)frame.leftX;
        frame.bottomLeftVtx->y = (float)(g_CoasterDisplayScaleY * 0xDF) + g_CoasterClipTop;
        frame.bottomLeftVtx->zBits = 0;
        frame.bottomLeftVtx->rhwBits = FLOAT_ONE_BITS;
        frame.bottomLeftVtx->color = frame.panelColor;
        frame.bottomLeftVtx->specular = 0xFF000000;
        frame.bottomLeftVtx->uBits = 0;
        frame.bottomLeftVtx->vBits = FLOAT_ONE_BITS;
        frame.topRightVtx = (GfxTlVertex *)(this->co1Tim->vertexCursor + 0x40);
        frame.topRightVtx->x = (float)frame.rightX;
        frame.topRightVtx->y = (float)(g_CoasterDisplayScaleY * 0xC0) + g_CoasterClipTop;
        frame.topRightVtx->zBits = 0;
        frame.topRightVtx->rhwBits = FLOAT_ONE_BITS;
        frame.topRightVtx->color = frame.panelColor;
        frame.topRightVtx->specular = 0xFF000000;
        frame.topRightVtx->uBits = FLOAT_ONE_BITS;
        frame.topRightVtx->vBits = 0;
        frame.bottomRightVtx = (GfxTlVertex *)(this->co1Tim->vertexCursor + 0x60);
        frame.bottomRightVtx->x = (float)frame.rightX;
        frame.bottomRightVtx->y = (float)(g_CoasterDisplayScaleY * 0xDF) + g_CoasterClipTop;
        frame.bottomRightVtx->zBits = 0;
        frame.bottomRightVtx->rhwBits = FLOAT_ONE_BITS;
        frame.bottomRightVtx->color = frame.panelColor;
        frame.bottomRightVtx->specular = 0xFF000000;
        frame.bottomRightVtx->uBits = FLOAT_ONE_BITS;
        frame.bottomRightVtx->vBits = FLOAT_ONE_BITS;
    }
}
/*
 * Renders speed/score gauge background panel.
 */
// FUNCTION: FF7 0x005f1cb5
void CoasterRenderer::drawPanel5F1CB5()
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        struct { unsigned char pad_to_10[0x10]; int panelColor; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *topLeftVtx; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *bottomLeftVtx; };
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *topRightVtx; };
        GfxTlVertex *bottomRightVtx;
    } frame;
    frame.panelColor = 0xFFFFFFFF;
    if (AcquireBatchNode(1, (void *)this->pauseTim) != 0) {
        frame.topLeftVtx = (GfxTlVertex *)this->pauseTim->vertexCursor;
        frame.topLeftVtx->x = (float)(g_coasterSubVolumeFactor * 0xC8) + g_CoasterClipLeft;
        frame.topLeftVtx->y = (float)(g_CoasterDisplayScaleY * 0xC0) + g_CoasterClipTop;
        frame.topLeftVtx->zBits = 0;
        frame.topLeftVtx->rhwBits = FLOAT_ONE_BITS;
        frame.topLeftVtx->color = frame.panelColor;
        frame.topLeftVtx->specular = 0xFF000000;
        frame.topLeftVtx->uBits = 0;
        frame.topLeftVtx->vBits = 0;
        frame.bottomLeftVtx = (GfxTlVertex *)(this->pauseTim->vertexCursor + 0x20);
        frame.bottomLeftVtx->x = (float)(g_coasterSubVolumeFactor * 0xC8) + g_CoasterClipLeft;
        frame.bottomLeftVtx->y = (float)(g_CoasterDisplayScaleY * 0xE0) + g_CoasterClipTop;
        frame.bottomLeftVtx->zBits = 0;
        frame.bottomLeftVtx->rhwBits = FLOAT_ONE_BITS;
        frame.bottomLeftVtx->color = frame.panelColor;
        frame.bottomLeftVtx->specular = 0xFF000000;
        frame.bottomLeftVtx->uBits = 0;
        frame.bottomLeftVtx->vBits = FLOAT_ONE_BITS;
        frame.topRightVtx = (GfxTlVertex *)(this->pauseTim->vertexCursor + 0x40);
        frame.topRightVtx->x = (float)(g_coasterSubVolumeFactor * 0x137) + g_CoasterClipLeft;
        frame.topRightVtx->y = (float)(g_CoasterDisplayScaleY * 0xC0) + g_CoasterClipTop;
        frame.topRightVtx->zBits = 0;
        frame.topRightVtx->rhwBits = FLOAT_ONE_BITS;
        frame.topRightVtx->color = frame.panelColor;
        frame.topRightVtx->specular = 0xFF000000;
        frame.topRightVtx->uBits = FLOAT_ONE_BITS;
        frame.topRightVtx->vBits = 0;
        frame.bottomRightVtx = (GfxTlVertex *)(this->pauseTim->vertexCursor + 0x60);
        frame.bottomRightVtx->x = (float)(g_coasterSubVolumeFactor * 0x137) + g_CoasterClipLeft;
        frame.bottomRightVtx->y = (float)(g_CoasterDisplayScaleY * 0xE0) + g_CoasterClipTop;
        frame.bottomRightVtx->zBits = 0;
        frame.bottomRightVtx->rhwBits = FLOAT_ONE_BITS;
        frame.bottomRightVtx->color = frame.panelColor;
        frame.bottomRightVtx->specular = 0xFF000000;
        frame.bottomRightVtx->uBits = FLOAT_ONE_BITS;
        frame.bottomRightVtx->vBits = FLOAT_ONE_BITS;
    }
}
/*
 * Renders current score readout HUD panel.
 */
// FUNCTION: FF7 0x005f1f00
void CoasterRenderer::drawScorePanel()
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        struct { unsigned char pad_to_20[0x20]; int topY; };
        struct { unsigned char pad_to_1C[0x1C]; int leftX; };
        struct { unsigned char pad_to_18[0x18]; int bottomY; };
        struct { unsigned char pad_to_14[0x14]; int panelColor; };
        struct { unsigned char pad_to_10[0x10]; int rightX; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *topLeftVtx; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *bottomLeftVtx; };
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *topRightVtx; };
        GfxTlVertex *bottomRightVtx;
    } frame;
    frame.panelColor = 0xFFFFFFFF;
    frame.leftX = g_coasterSubVolumeFactor * 0xCC + g_CoasterClipLeft;
    frame.rightX = g_coasterSubVolumeFactor * 0xF3 + g_CoasterClipLeft;
    frame.topY = g_CoasterDisplayScaleY * 0xC8 + g_CoasterClipTop;
    frame.bottomY = g_CoasterDisplayScaleY * 0xE8 + g_CoasterClipTop;
    AcquireBatchNode(1, (void *)this->scoreTim);
    frame.topLeftVtx = (GfxTlVertex *)this->scoreTim->vertexCursor;
    frame.topLeftVtx->x = (float)frame.leftX;
    frame.topLeftVtx->y = (float)frame.topY;
    frame.topLeftVtx->zBits = 0;
    frame.topLeftVtx->rhwBits = FLOAT_ONE_BITS;
    frame.topLeftVtx->color = frame.panelColor;
    frame.topLeftVtx->specular = 0xFF000000;
    frame.topLeftVtx->uBits = 0;
    frame.topLeftVtx->vBits = 0;
    frame.bottomLeftVtx = (GfxTlVertex *)(this->scoreTim->vertexCursor + 0x20);
    frame.bottomLeftVtx->x = (float)frame.leftX;
    frame.bottomLeftVtx->y = (float)frame.bottomY;
    frame.bottomLeftVtx->zBits = 0;
    frame.bottomLeftVtx->rhwBits = FLOAT_ONE_BITS;
    frame.bottomLeftVtx->color = frame.panelColor;
    frame.bottomLeftVtx->specular = 0xFF000000;
    frame.bottomLeftVtx->uBits = 0;
    frame.bottomLeftVtx->vBits = FLOAT_ONE_BITS;
    frame.topRightVtx = (GfxTlVertex *)(this->scoreTim->vertexCursor + 0x40);
    frame.topRightVtx->x = (float)frame.rightX;
    frame.topRightVtx->y = (float)frame.topY;
    frame.topRightVtx->zBits = 0;
    frame.topRightVtx->rhwBits = FLOAT_ONE_BITS;
    frame.topRightVtx->color = frame.panelColor;
    frame.topRightVtx->specular = 0xFF000000;
    frame.topRightVtx->uBits = 0x3E3CBCBD;
    frame.topRightVtx->vBits = 0;
    frame.bottomRightVtx = (GfxTlVertex *)(this->scoreTim->vertexCursor + 0x60);
    frame.bottomRightVtx->x = (float)frame.rightX;
    frame.bottomRightVtx->y = (float)frame.bottomY;
    frame.bottomRightVtx->zBits = 0;
    frame.bottomRightVtx->rhwBits = FLOAT_ONE_BITS;
    frame.bottomRightVtx->color = frame.panelColor;
    frame.bottomRightVtx->specular = 0xFF000000;
    frame.bottomRightVtx->uBits = 0x3E3CBCBD;
    frame.bottomRightVtx->vBits = FLOAT_ONE_BITS;
    this->drawNumber(g_CoasterScore, g_coasterSubVolumeFactor * 0xEC, g_CoasterDisplayScaleY * 0xC8, 0);
}
/*
 * Renders a multi-digit numeric string on HUD using digit texture glyphs.
 */
// FUNCTION: FF7 0x005f2119
void CoasterRenderer::drawNumber(int number, short x, short y, short forceDigits)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        struct { unsigned char pad_to_38[0x38]; int digitCount; };
        struct { unsigned char pad_to_34[0x34]; int divisor; };
        struct { unsigned char pad_to_30[0x30]; int digitIndex; };
        struct { unsigned char pad_to_2C[0x2C]; int color; };
        struct { unsigned char pad_to_28[0x28]; int remainder; };
        struct { unsigned char pad_to_24[0x24]; int leadingZero; };
        struct { unsigned char pad_to_20[0x20]; int digit; };
        struct { unsigned char pad_to_1C[0x1C]; float posX; };
        struct { unsigned char pad_to_18[0x18]; float posY; };
        struct { unsigned char pad_to_14[0x14]; float texLeft; };
        struct { unsigned char pad_to_10[0x10]; float texRight; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *vertTopLeft; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *vertBottomLeft; };
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *vertTopRight; };
        GfxTlVertex *vertBottomRight;
    } frame;
    frame.digitCount = 4;
    frame.divisor = 0x3E8;
    frame.color = 0xFFFFFFFF;
    frame.leadingZero = 1;
    frame.remainder = number + 1;
    if (number > 0x270F) {
        frame.digitCount = 5;
        frame.divisor = 0x2710;
    }
    for (frame.digitIndex = 0; frame.digitIndex < frame.digitCount; frame.digitIndex++) {
        frame.digit = 0;
        while (frame.remainder > frame.divisor) {
            frame.digit++;
            frame.remainder -= frame.divisor;
        }
        if (frame.digit != 0) {
            frame.leadingZero = 0;
        }
        if (number == 0 && frame.divisor == 1) {
            frame.leadingZero = 0;
        }
        if (forceDigits == 1 || (frame.digit != 0 || frame.leadingZero == 0)) {
            frame.posX = (float)(x + frame.digitIndex * (g_coasterSubVolumeFactor * 0xD)) + g_CoasterClipLeft;
            frame.posY = (float)y + g_CoasterClipTop;
            frame.texLeft = (float)((frame.digit << 4) + 0x30) / 255.0f;
            frame.texRight = (float)((frame.digit << 4) + 0x40) / 255.0f;
            AcquireBatchNode(1, (void *)this->scoreTim);
            frame.vertTopLeft = (GfxTlVertex *)this->scoreTim->vertexCursor;
            frame.vertTopLeft->x = frame.posX;
            frame.vertTopLeft->y = frame.posY;
            frame.vertTopLeft->zBits = 0;
            frame.vertTopLeft->rhwBits = FLOAT_ONE_BITS;
            frame.vertTopLeft->color = frame.color;
            frame.vertTopLeft->specular = 0xFF000000;
            frame.vertTopLeft->u = frame.texLeft;
            frame.vertTopLeft->vBits = 0;
            frame.vertBottomLeft = (GfxTlVertex *)(this->scoreTim->vertexCursor + 0x20);
            frame.vertBottomLeft->x = frame.posX;
            frame.vertBottomLeft->y = (float)(g_CoasterDisplayScaleY * 0x1D) + frame.posY;
            frame.vertBottomLeft->zBits = 0;
            frame.vertBottomLeft->rhwBits = FLOAT_ONE_BITS;
            frame.vertBottomLeft->color = frame.color;
            frame.vertBottomLeft->specular = 0xFF000000;
            frame.vertBottomLeft->u = frame.texLeft;
            frame.vertBottomLeft->vBits = FLOAT_ONE_BITS;
            frame.vertTopRight = (GfxTlVertex *)(this->scoreTim->vertexCursor + 0x40);
            frame.vertTopRight->x = (float)(g_coasterSubVolumeFactor * 0xF) + frame.posX;
            frame.vertTopRight->y = frame.posY;
            frame.vertTopRight->zBits = 0;
            frame.vertTopRight->rhwBits = FLOAT_ONE_BITS;
            frame.vertTopRight->color = frame.color;
            frame.vertTopRight->specular = 0xFF000000;
            frame.vertTopRight->u = frame.texRight;
            frame.vertTopRight->vBits = 0;
            frame.vertBottomRight = (GfxTlVertex *)(this->scoreTim->vertexCursor + 0x60);
            frame.vertBottomRight->x = (float)(g_coasterSubVolumeFactor * 0xF) + frame.posX;
            frame.vertBottomRight->y = (float)(g_CoasterDisplayScaleY * 0x1D) + frame.posY;
            frame.vertBottomRight->zBits = 0;
            frame.vertBottomRight->rhwBits = FLOAT_ONE_BITS;
            frame.vertBottomRight->color = frame.color;
            frame.vertBottomRight->specular = 0xFF000000;
            frame.vertBottomRight->u = frame.texRight;
            frame.vertBottomRight->vBits = FLOAT_ONE_BITS;
        }
        frame.divisor = frame.divisor / 0xA;
    }
}
/*
 * Renders a fullscreen fade polygon with alpha blending at the specified fade level.
 */
// FUNCTION: FF7 0x005f2417
void CoasterRenderer::DrawFadeOverlay(unsigned char level)
{
    struct {
        GfxTlVertex *p4, *p3, *p2, *p1;
        union {
            int packed;
            unsigned char channel[4];
        } color;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    local.color.channel[0] = 0;
    local.color.channel[1] = 0;
    local.color.channel[2] = 0;
    local.color.channel[3] = level;
    AcquireBatchNode(1, (void *)this->fadeObject);
    local.p1 = (GfxTlVertex *)this->fadeObject->vertexCursor;
    local.p1->x = (float)g_CoasterClipLeft + 0.0f;
    local.p1->y = (float)g_CoasterClipTop + 0.0f;
    local.p1->zBits = 0;
    local.p1->rhwBits = FLOAT_ONE_BITS;
    local.p1->color = local.color.packed;
    local.p1->specular = 0xff000000;
    local.p1->uBits = 0x0;
    local.p1->vBits = 0x0;
    local.p2 = (GfxTlVertex *)(this->fadeObject->vertexCursor + 32);
    local.p2->x = (float)g_CoasterClipLeft + 0.0f;
    local.p2->y = (float)(g_CoasterDisplayScaleY * 240) + g_CoasterClipTop;
    local.p2->zBits = 0;
    local.p2->rhwBits = FLOAT_ONE_BITS;
    local.p2->color = local.color.packed;
    local.p2->specular = 0xff000000;
    local.p2->uBits = 0x0;
    local.p2->vBits = FLOAT_ONE_BITS;
    local.p3 = (GfxTlVertex *)(this->fadeObject->vertexCursor + 64);
    local.p3->x = (float)(g_coasterSubVolumeFactor * 320) + g_CoasterClipLeft;
    local.p3->y = (float)g_CoasterClipTop + 0.0f;
    local.p3->zBits = 0;
    local.p3->rhwBits = FLOAT_ONE_BITS;
    local.p3->color = local.color.packed;
    local.p3->specular = 0xff000000;
    local.p3->uBits = FLOAT_ONE_BITS;
    local.p3->vBits = 0x0;
    local.p4 = (GfxTlVertex *)(this->fadeObject->vertexCursor + 96);
    local.p4->x = (float)(g_coasterSubVolumeFactor * 320) + g_CoasterClipLeft;
    local.p4->y = (float)(g_CoasterDisplayScaleY * 240) + g_CoasterClipTop;
    local.p4->zBits = 0;
    local.p4->rhwBits = FLOAT_ONE_BITS;
    local.p4->color = local.color.packed;
    local.p4->specular = 0xff000000;
    local.p4->uBits = FLOAT_ONE_BITS;
    local.p4->vBits = FLOAT_ONE_BITS;
}
/*
 * Transforms a 16-bit 3D point by a 4x4 matrix and performs perspective division.
 */
// FUNCTION: FF7 0x005f2639
void CoasterRenderer::projectPoint(short *point, void *matrix, float *out)
{
    float invW;
    float localPoint[3];
    localPoint[0] = point[0];
    localPoint[1] = point[1];
    localPoint[2] = point[2];
    TransformPointByMatrix4x4(matrix, localPoint, out);
    if (out[3] == 0.0f)
        out[3] = 0.1f;
    invW = 1.0f / out[3];
    out[0] = out[0] * invW;
    out[1] = out[1] * invW;
    out[2] = out[2] * invW;
    out[3] = invW;
}
/*
 * Transforms a point and checks whether it lies within the front viewing frustum.
 */
// FUNCTION: FF7 0x005f26eb
int CoasterRenderer::transformVisible(short *point, void *matrix, float *out)
{
    float val[3];
    val[0] = point[0];
    val[1] = point[1];
    val[2] = point[2];
    TransformPoint3RowVec(matrix, val, out);
    if (out[2] > 0.0f)
        return 1;
    return 0;
}
/*
 * Computes projected depth for a 16-bit point using the camera transformation matrix.
 */
// FUNCTION: FF7 0x005f2759
float CoasterRenderer::transformDepth(short *point, float *matrix)
{
    float zDepth;
    float coords[3];
    coords[0] = point[0];
    coords[1] = point[1];
    coords[2] = point[2];
    return zDepth = matrix[2] * coords[0] + matrix[6] * coords[1] + matrix[10] * coords[2] + matrix[14];
}
/*
 * Updates context transform matrix and recalculates combined world-view matrix.
 */
// FUNCTION: FF7 0x005f27bf
void CoasterRenderer::setMatrix(void *matrix)
{
    void *projection;
    float converted[16];
    GfxEngineContext *engine;
    void *worldViewPtr;
    engine = GetEngineContext();
    worldViewPtr = (void *)engine->cameraMatrix;
    projection = engine->screenTransform;
    GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, converted);
    MultiplyMatrix4x4Full(converted, worldViewPtr, this->scratchMatrix);
    MultiplyMatrix4x4Full(this->scratchMatrix, projection, this->projectionMatrix);
}
/*
 * Computes linear depth fog/shading attenuation factor between near and far planes.
 */
// FUNCTION: FF7 0x005f2838
float CoasterRenderer::depthFactor(float depth)
{
    if ((float)g_CoasterFadeNearDepth > depth)
        return 1.0f;
    if ((float)g_CoasterFadeFarDepth < depth)
        return 0.0f;
    return 1.0f - (depth - (float)g_CoasterFadeNearDepth) / (float)(g_CoasterFadeFarDepth - g_CoasterFadeNearDepth);
}
/*
 * Modulates RGB color channels by a depth shading factor.
 */
// FUNCTION: FF7 0x005f2897
int CoasterRenderer::shadeColor(void *color, float factor)
{
    union {
        int word;
        unsigned char byte[4];
    } v;
    v.word = *(int *)color;
    v.byte[0] = (unsigned char)((v.byte[0]) * factor);
    v.byte[1] = (unsigned char)((v.byte[1]) * factor);
    v.byte[2] = (unsigned char)((v.byte[2]) * factor);
    return ((v.byte[0]) << 16) | 0xff000000 | ((v.byte[1]) << 8) | (v.byte[2]);
}
/*
 * Submits queued primitive batch lists to the graphics rendering engine.
 */
// FUNCTION: FF7 0x005f2924
void CoasterRenderer::submitBatches()
{
    GfxEngineContext *engine;
    engine = GetEngineContext();
    Gfx_EmitPrimitiveBatchList((void *)this->partBatchesBlend4, engine);
    InvalidateBatchGeneration((void *)this->partBatchesBlend4);
    Gfx_DrawRenderObjectCall(this->partCallsBlend4, engine);
    InvalidateBatchGeneration((void *)this->partCallsBlend4);
    Gfx_SetScenePassCall(1, (struct GfxEngineContext *)engine);
    Gfx_EmitPrimitiveBatchList((void *)this->partBatchesBlend1, engine);
    InvalidateBatchGeneration((void *)this->partBatchesBlend1);
    Gfx_DrawRenderObjectCall(this->partCallsBlend1, engine);
    InvalidateBatchGeneration((void *)this->partCallsBlend1);
    Gfx_EmitPrimitiveBatchList((void *)this->currentBeamTim, engine);
    InvalidateBatchGeneration((void *)this->currentBeamTim);
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)engine);
    Gfx_EmitPrimitiveBatchList((void *)this->railBatches, engine);
    InvalidateBatchGeneration((void *)this->railBatches);
    Gfx_DrawRenderObjectCall(this->railCalls, engine);
    InvalidateBatchGeneration((void *)this->railCalls);
    Gfx_EmitPrimitiveBatchList((void *)this->reticleTim, engine);
    InvalidateBatchGeneration((void *)this->reticleTim);
    Gfx_EmitPrimitiveBatchList((void *)this->chargeObject, engine);
    InvalidateBatchGeneration((void *)this->chargeObject);
    Gfx_EmitPrimitiveBatchList((void *)this->waku2Tim, engine);
    InvalidateBatchGeneration((void *)this->waku2Tim);
    Gfx_EmitPrimitiveBatchList((void *)this->chargeObject, engine);
    InvalidateBatchGeneration((void *)this->chargeObject);
    Gfx_EmitPrimitiveBatchList((void *)this->waku2Tim, engine);
    InvalidateBatchGeneration((void *)this->waku2Tim);
    Gfx_EmitPrimitiveBatchList((void *)this->co1Tim, engine);
    InvalidateBatchGeneration((void *)this->co1Tim);
    Gfx_EmitPrimitiveBatchList((void *)this->scoreTim, engine);
    InvalidateBatchGeneration((void *)this->scoreTim);
    Gfx_EmitPrimitiveBatchList((void *)this->pauseTim, engine);
    InvalidateBatchGeneration((void *)this->pauseTim);
    Gfx_SetScenePassCall(1, (struct GfxEngineContext *)engine);
    Gfx_EmitPrimitiveBatchList((void *)this->fadeObject, engine);
    InvalidateBatchGeneration((void *)this->fadeObject);
}

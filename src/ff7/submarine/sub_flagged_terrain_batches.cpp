// src/ff7/submarine/sub_flagged_terrain_batches.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Collects the flagged terrain grid cells and emits the radial-band terrain batches for
//   the submarine scene, with a scene-pass draw hook.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/api/submarine.h"
#include "lib/api/graphics.h"
#include "ff7/types/submarine.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/submarine.h"
#include <math.h>
#include <stdlib.h>
#include "lib/codegen.h"
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"

extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(strcpy, strcat)

// GLOBAL: FF7 0x00e996e8
int g_SubmarineUpVectorX = 0;
// GLOBAL: FF7 0x00e996ec
int g_SubmarineUpVectorY = 0;
// GLOBAL: FF7 0x00e996f0
int g_SubmarineUpVectorZ = 0;
unsigned char g_bssPad_00e996f4[4] = {0};
// GLOBAL: FF7 0x00e996f8
PsxMatrix g_SubmarineViewMatrix = {0};
// GLOBAL: FF7 0x00e99718
int g_SubmarineViewPad = 0;
unsigned char g_bssPad_00e9971c[4] = {0};
unsigned char g_bssPad_00e99720[24] = {0};
// GLOBAL: FF7 0x00e99738
short g_SubmarineRecordInterpBaseA = 0;
// GLOBAL: FF7 0x00e9973c
short g_SubmarineRecordInterpBaseB = 0;
// GLOBAL: FF7 0x00e99740
short g_SubmarineRecordInterpTargetA = 0;
// GLOBAL: FF7 0x00e99744
short g_SubmarineRecordInterpTargetB = 0;
// GLOBAL: FF7 0x00e99748
short g_SubmarineRecordInterpDeltaA = 0;
// GLOBAL: FF7 0x00e9974c
short g_SubmarineRecordInterpDeltaB = 0;
// GLOBAL: FF7 0x00e99750
int g_SubmarineRecordInterpBaseC = 0;
// GLOBAL: FF7 0x00e99754
int g_SubmarineRecordInterpTargetC = 0;
// GLOBAL: FF7 0x00e99758
int g_SubmarineRecordInterpDeltaC = 0;
// GLOBAL: FF7 0x00e9975c
int g_SubmarineRecordInterpBaseD = 0;
// GLOBAL: FF7 0x00e99760
int g_SubmarineRecordInterpTargetD = 0;
// GLOBAL: FF7 0x00e99764
int g_SubmarineRecordInterpDeltaD = 0;
// GLOBAL: FF7 0x00e99768
int g_SubmarineRecordInterpFraction = 0;
// GLOBAL: FF7 0x00e9976c
int g_SubmarineRecordInterpStep = 0;
// GLOBAL: FF7 0x00e99770
int g_SubmarineRecordInterpActive = 0;
unsigned char g_rawData_00e99774[4] = {0};
unsigned char g_rawData_00e99778[160000] = {0};
// clang-format off
// GLOBAL: FF7 0x00ec0878
GfxRenderObject *g_SubmarineRadialTerrainBatchNode = 0;
// GLOBAL: FF7 0x00ec087c
struct GfxRenderObject *g_SubmarineRadialTerrainScaledBatchNode = 0;
// GLOBAL: FF7 0x00ec0880
SubmarineFlaggedCell g_SubmarineFlaggedCells[500] = {0};
// GLOBAL: FF7 0x00ec1820
int g_SubmarineFadeStep = 0;
// GLOBAL: FF7 0x00ec1824
int g_SubmarineAlarmActive = 0;
// GLOBAL: FF7 0x00ec1828
int g_SubmarineAccumulatedOffset = 0;
unsigned char g_rawData_00ec182c[4] = {0};
// GLOBAL: FF7 0x00ec1830
struct GfxRenderObject *g_SubmarineConnectorBatchContext = 0;
unsigned char g_bssPad_00ec1834[4] = {0};
// GLOBAL: FF7 0x00ec1838
struct SubmarineFlightRecord g_SubmarineFlightRecords[32] = {0};
// clang-format on

// clang-format on

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

// Scans the 4096 cells of g_SubmarineGridCellTable for SUBMARINE_CELL_FLAGGED and appends each
// flagged cell's grid X, raised height, grid Y and cell index to g_SubmarineFlaggedCells. Returns once the
// count exceeds maxCount, so maxCount plus one cells can be written.
// FUNCTION: FF7 0x0079a550
void __cdecl Submarine_CollectFlaggedGridCells(int maxCount)
{
    int recordIdx;
    int outCount;
    SubmarineGridCell *record;

    recordIdx = 0;
    outCount = 0;
    do {
        record = &g_SubmarineGridCellTable[recordIdx];
        if ((record->flags & SUBMARINE_CELL_FLAGGED) != 0) {
            g_SubmarineFlaggedCells[outCount].cellX = ((recordIdx % 64) << 6) - 0x7E0;
            g_SubmarineFlaggedCells[outCount].cellHeight = record->raisedHeight - 0x20;
            g_SubmarineFlaggedCells[outCount].cellY = 0x800 - (((recordIdx) / 64) << 6) - 0x20;
            g_SubmarineFlaggedCells[outCount].recordIndex = recordIdx;
            outCount++;
            if (outCount > maxCount) {
                return;
            }
        }
        recordIdx++;
    } while (recordIdx < 0x1000);
}

// Sets up camera-relative geometry and an advancing radial band, then examines eight indexed
// triangles for each flagged cell. Skips cells without SUBMARINE_CELL_FLAGGED or with
// SUBMARINE_CELL_TIMED, cells beyond bandBase, and cells outside the current radial band. Builds
// each triangle from the byte indices of g_SubmarineIndexTable into g_SubmarineOffsetTable and
// emits it only when all three transformed W values are positive and AcquireBatchNode succeeds,
// storing reciprocal W and the triangle colour. If any triangle was emitted for that cell, builds
// four side strips from its g_SubmarineGridVertexBuffer corners, closes the strips of the last
// column and row, offsets the vertices by -32 in X and +32 in Z, and projects all sixteen. Any
// nonpositive W suppresses the strips; otherwise, unless auxFlags is set, acquires eight triangles
// and writes the four strips with fading colours, keeping the original W. Clears the emitted flag
// after each cell and increments g_SubmarineFadeStep every call.
// FUNCTION: FF7 0x0079a61e
void __cdecl Submarine_EmitRadialBandTerrainBatches(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        SubmarineRadialTerrainBatch *bandSlotE;
        struct { unsigned char pad_to_4[0x4]; SubmarineRadialTerrainBatch *bandSlotD; };
        struct { unsigned char pad_to_8[0x8]; SubmarineRadialTerrainBatch *bandSlotC; };
        struct { unsigned char pad_to_C[0xC]; SubmarineRadialTerrainBatch *bandSlotB; };
        struct { unsigned char pad_to_10[0x10]; SubmarineRadialTerrainBatch *bandSlotA; };
        struct { unsigned char pad_to_14[0x14]; SubmarineRadialTerrainBatch *batchRec18; };
        struct { unsigned char pad_to_18[0x18]; SubmarineRadialTerrainBatch *batchRec17; };
        struct { unsigned char pad_to_1C[0x1C]; SubmarineRadialTerrainBatch *batchRec16; };
        struct { unsigned char pad_to_20[0x20]; SubmarineRadialTerrainBatch *batchRec15; };
        struct { unsigned char pad_to_24[0x24]; SubmarineRadialTerrainBatch *batchRec14; };
        struct { unsigned char pad_to_28[0x28]; SubmarineRadialTerrainBatch *batchRec13; };
        struct { unsigned char pad_to_2C[0x2C]; SubmarineRadialTerrainBatch *batchRec12; };
        struct { unsigned char pad_to_30[0x30]; SubmarineRadialTerrainBatch *batchRec11; };
        struct { unsigned char pad_to_34[0x34]; SubmarineRadialTerrainBatch *batchRec10; };
        struct { unsigned char pad_to_38[0x38]; SubmarineRadialTerrainBatch *batchRec09; };
        struct { unsigned char pad_to_3C[0x3C]; SubmarineRadialTerrainBatch *batchRec08; };
        struct { unsigned char pad_to_40[0x40]; SubmarineRadialTerrainBatch *batchRec07; };
        struct { unsigned char pad_to_44[0x44]; SubmarineRadialTerrainBatch *batchRec06; };
        struct { unsigned char pad_to_48[0x48]; SubmarineRadialTerrainBatch *batchRec05; };
        struct { unsigned char pad_to_4C[0x4C]; SubmarineRadialTerrainBatch *batchRec04; };
        struct { unsigned char pad_to_50[0x50]; SubmarineRadialTerrainBatch *batchRec03; };
        struct { unsigned char pad_to_54[0x54]; SubmarineRadialTerrainBatch *batchRec02; };
        struct { unsigned char pad_to_58[0x58]; SubmarineRadialTerrainBatch *batchRec01; };
        struct { unsigned char pad_to_5C[0x5C]; SubmarineRadialTerrainBatch *batchRec00; };
        struct { unsigned char pad_to_60[0x60]; float vtxInvW; };
        struct { unsigned char pad_to_64[0x64]; int emitted; };
        struct { unsigned char pad_to_68[0x68]; int vtxCol; };
        struct { unsigned char pad_to_6C[0x6C]; float cellTopY; };
        struct { unsigned char pad_to_70[0x70]; int vtxRow; };
        struct { unsigned char pad_to_74[0x74]; int cellRow; };
        struct { unsigned char pad_to_78[0x78]; int cellCol; };
        struct { unsigned char pad_to_7C[0x7C]; SubmarineRadialTerrainScaledA *scaledRecC; };
        struct { unsigned char pad_to_80[0x80]; float recipWC; };
        struct { unsigned char pad_to_84[0x84]; SubmarineRadialTerrainScaledA *scaledRecB; };
        struct { unsigned char pad_to_88[0x88]; float recipWB; };
        struct { unsigned char pad_to_8C[0x8C]; SubmarineRadialTerrainScaledA *scaledRecA; };
        struct { unsigned char pad_to_90[0x90]; float recipWA; };
        struct { unsigned char pad_to_94[0x94]; int distRemain; };
        struct { unsigned char pad_to_98[0x98]; int relZ; };
        struct { unsigned char pad_to_9C[0x9C]; int relY; };
        struct { unsigned char pad_to_A0[0xA0]; int relX; };
        struct { unsigned char pad_to_A4[0xA4]; int dzSq; };
        struct { unsigned char pad_to_A8[0xA8]; int dySq; };
        struct { unsigned char pad_to_AC[0xAC]; int dxSq; };
        struct { unsigned char pad_to_B0[0xB0]; SubmarineGridCell *cellEntry; };
        struct { unsigned char pad_to_B4[0xB4]; int bandOffset; };
        struct { unsigned char pad_to_B8[0xB8]; GfxMatrix4x4 composedMat; };
        struct { unsigned char pad_to_F8[0xF8]; GfxClipVec4 proj[4][4]; };
        struct { unsigned char pad_to_1F8[0x1F8]; int posZ; };
        struct { unsigned char pad_to_1FC[0x1FC]; int posY; };
        struct { unsigned char pad_to_200[0x200]; int posX; };
        struct { unsigned char pad_to_204[0x204]; int triIndex; };
        struct { unsigned char pad_to_208[0x208]; int levelIndex; };
        struct { unsigned char pad_to_20C[0x20C]; int packedIdx; };
        struct { unsigned char pad_to_210[0x210]; int vtxColor[4][4]; };
        struct { unsigned char pad_to_250[0x250]; union { GfxHomogeneousPoint f; SubmarineVec3Int v; } pt[4][4]; };
        struct { unsigned char pad_to_310[0x310]; SubmarineFlaggedCell cellCopy; };
        struct { unsigned char pad_to_318[0x318]; int triColor; };
        struct { unsigned char pad_to_31C[0x31C]; GfxHomogeneousPoint point2; };
        struct { unsigned char pad_to_328[0x328]; int tmpInit; };
        struct { unsigned char pad_to_32C[0x32C]; struct GfxEngineContext *engineCtx; };
        struct { unsigned char pad_to_330[0x330]; GfxMatrix4x4 rotMatrix; };
        struct { unsigned char pad_to_370[0x370]; GfxHomogeneousPoint point1; };
        struct { unsigned char pad_to_37C[0x37C]; GfxHomogeneousVec4 xform2; };
        struct { unsigned char pad_to_38C[0x38C]; SubmarineRadialBandState *bandState; };
        struct { unsigned char pad_to_390[0x390]; GfxHomogeneousPoint point0; };
        struct { unsigned char pad_to_39C[0x39C]; GfxHomogeneousVec4 xform1; };
        struct { unsigned char pad_to_3AC[0x3AC]; GfxHomogeneousVec4 xform0; };
        struct { unsigned char pad_to_3BC[0x3BC]; int colorByte2; };
        struct { unsigned char pad_to_3BD[0x3BD]; char colorByte1; };
        struct { unsigned char pad_to_3BE[0x3BE]; char colorByte0; };
        struct { unsigned char pad_to_3BF[0x3BF]; char fadeByte; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.engineCtx = GetEngineContext();
    frame.tmpInit = 0;
    frame.bandState = (SubmarineRadialBandState *)Gfx_GetPsxScratchpadPtr(0);
    frame.bandState->levelCount = g_SubmarineWorkingLevelLimit;
    frame.bandState->pairTable = g_SubmarineFlaggedCells;
    frame.bandState->offsetTable = g_SubmarineOffsetTable;
    frame.bandState->indexTable = (int *)g_SubmarineIndexTable;
    frame.bandState->originX = g_SubmarineState.player.posX >> 0xC;
    frame.bandState->originY = g_SubmarineState.player.posY >> 0xC;
    frame.bandState->originZ = g_SubmarineState.player.posZ >> 0xC;
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), &frame.rotMatrix);
    ComposeWithScreenMatrix(&frame.rotMatrix, &frame.composedMat);
    frame.bandState->bandStep = g_SubmarineState.currentBandStep;
    frame.bandState->bandBase = g_SubmarineState.currentBandBase;
    frame.bandState->bandOuterDistance = 1;
    if (g_SubmarineAlarmActive != 0) {
        frame.bandOffset = g_SubmarineAccumulatedOffset + g_SubmarineState.currentBandOffset;
        if (frame.bandOffset > frame.bandState->bandBase + frame.bandState->bandStep) {
            frame.bandOffset = 1;
            g_SubmarineAlarmActive = 0;
        }
        g_SubmarineAccumulatedOffset = frame.bandOffset;
        frame.bandState->bandOuterDistance = g_SubmarineAccumulatedOffset;
    }
    if (g_SubmarineState.auxFlags != 0) {
        frame.fadeByte = 0x7F;
    } else {
        frame.fadeByte = 0xFF - (g_SubmarineFadeStep << 2);
    }
    for (frame.levelIndex = 0; (unsigned int)frame.levelIndex < (unsigned int)frame.bandState->levelCount; frame.levelIndex++) {
        for (frame.triIndex = 0; (unsigned int)frame.triIndex < (unsigned int)8; frame.triIndex++) {
            frame.packedIdx = frame.bandState->indexTable[frame.triIndex];
            frame.cellCopy = frame.bandState->pairTable[frame.levelIndex];
            frame.triColor = ((int *)&g_SubmarinePointColors)[frame.triIndex];
            frame.posX = frame.cellCopy.cellX;
            frame.posY = frame.cellCopy.cellHeight;
            frame.posZ = frame.cellCopy.cellY;
            frame.cellEntry = &g_SubmarineGridCellTable[frame.cellCopy.recordIndex];
            if ((frame.cellEntry->flags & SUBMARINE_CELL_FLAGGED) == 0) {
                continue;
            }
            if ((frame.cellEntry->flags & SUBMARINE_CELL_TIMED) != 0) {
                continue;
            }
            frame.bandState->savedLevel = 0;
            frame.relX = frame.posX - frame.bandState->originX;
            frame.relY = frame.posY - frame.bandState->originY;
            frame.relZ = frame.posZ - frame.bandState->originZ;
            frame.dxSq = frame.relX * frame.relX;
            frame.dySq = frame.relY * frame.relY;
            frame.dzSq = frame.relZ * frame.relZ;
            frame.dxSq += frame.dySq + frame.dzSq;
            frame.dySq = Bit_CountLeadingSignBits(frame.dxSq);
            frame.dySq = frame.dySq >> 1 << 1;
            frame.dzSq = (0x1F - frame.dySq) / 2;
            if (frame.dySq == 0x20) {
                frame.dxSq = 0;
            }
            if (frame.dySq < 0x18) {
                frame.dxSq >>= 0x18 - frame.dySq;
            } else {
                frame.dxSq <<= frame.dySq - 0x18;
            }
            frame.dxSq = g_SubmarineModelIndicesA[frame.dxSq - 64] << frame.dzSq >> 0xC;
            frame.bandState->savedLevel = frame.dxSq;
            if (frame.dxSq >= frame.bandState->bandBase) {
                continue;
            }
            frame.distRemain = frame.bandState->bandOuterDistance - frame.dxSq;
            if (frame.distRemain < 0) {
                continue;
            }
            if (frame.distRemain > frame.bandState->bandStep) {
                continue;
            }
            frame.colorByte0 = frame.triColor << 0x18 >> 0x18;
            frame.colorByte1 = frame.triColor << 0x10 >> 0x18;
            *(char *)&frame.colorByte2 = frame.triColor << 8 >> 0x18;
            frame.point0.x = (float)(frame.bandState->offsetTable[frame.packedIdx << 0x18 >> 0x18].offsetX + frame.posX);
            frame.point0.y = (float)(frame.bandState->offsetTable[frame.packedIdx << 0x18 >> 0x18].offsetY + frame.posY);
            frame.point0.z = (float)(frame.bandState->offsetTable[frame.packedIdx << 0x18 >> 0x18].offsetZ + frame.posZ);
            frame.point1.x = (float)(frame.bandState->offsetTable[frame.packedIdx << 0x10 >> 0x18].offsetX + frame.posX);
            frame.point1.y = (float)(frame.bandState->offsetTable[frame.packedIdx << 0x10 >> 0x18].offsetY + frame.posY);
            frame.point1.z = (float)(frame.bandState->offsetTable[frame.packedIdx << 0x10 >> 0x18].offsetZ + frame.posZ);
            frame.point2.x = (float)(frame.bandState->offsetTable[frame.packedIdx << 8 >> 0x18].offsetX + frame.posX);
            frame.point2.y = (float)(frame.bandState->offsetTable[frame.packedIdx << 8 >> 0x18].offsetY + frame.posY);
            frame.point2.z = (float)(frame.bandState->offsetTable[frame.packedIdx << 8 >> 0x18].offsetZ + frame.posZ);
            TransformPointByMatrix4x4(&frame.composedMat, &frame.point0, &frame.xform0);
            if (frame.xform0.w > 0.0) {
                TransformPointByMatrix4x4(&frame.composedMat, &frame.point1, &frame.xform1);
                if (frame.xform1.w > 0.0) {
                    TransformPointByMatrix4x4(&frame.composedMat, &frame.point2, &frame.xform2);
                    if (frame.xform2.w > 0.0) {
                        if (AcquireBatchNode(1, (void *)(((int)g_SubmarineRadialTerrainScaledBatchNode))) != 0) {
                            frame.recipWA = 1.0f / frame.xform0.w;
                            frame.scaledRecA = (SubmarineRadialTerrainScaledA *)(g_SubmarineRadialTerrainScaledBatchNode->vertexCursor);
                            frame.scaledRecA->x = frame.xform0.x * frame.recipWA;
                            frame.scaledRecA->y = frame.xform0.y * frame.recipWA;
                            frame.scaledRecA->z = frame.xform0.z * frame.recipWA;
                            frame.scaledRecA->scale = frame.recipWA;
                            frame.scaledRecA->color = frame.colorByte2;
                            frame.recipWB = 1.0f / frame.xform1.w;
                            frame.scaledRecB = (SubmarineRadialTerrainScaledA *)(g_SubmarineRadialTerrainScaledBatchNode->vertexCursor + 0x20);
                            frame.scaledRecB->x = frame.xform1.x * frame.recipWB;
                            frame.scaledRecB->y = frame.xform1.y * frame.recipWB;
                            frame.scaledRecB->z = frame.xform1.z * frame.recipWB;
                            frame.scaledRecB->scale = frame.recipWB;
                            frame.scaledRecB->color = frame.colorByte2;
                            frame.recipWC = 1.0f / frame.xform2.w;
                            frame.scaledRecC = (SubmarineRadialTerrainScaledA *)(g_SubmarineRadialTerrainScaledBatchNode->vertexCursor + 0x40);
                            frame.scaledRecC->x = frame.xform2.x * frame.recipWC;
                            frame.scaledRecC->y = frame.xform2.y * frame.recipWC;
                            frame.scaledRecC->z = frame.xform2.z * frame.recipWC;
                            frame.scaledRecC->scale = frame.recipWC;
                            frame.scaledRecC->color = frame.colorByte2;
                            frame.tmpInit = 1;
                        }
                    }
                }
            }
        }
        if (frame.tmpInit != 0) {
            frame.emitted = 0;
            frame.cellTopY = (float)((frame.posY >> 6 << 6) + 0x40);
            frame.cellCol = frame.cellCopy.recordIndex % 64;
            frame.cellRow = (frame.cellCopy.recordIndex - frame.cellCol) / 64;
            frame.pt[0][2].v = *(SubmarineVec3Int *)&((SubmarineGridVertex (*)[64])&g_SubmarineGridVertexBuffer)[frame.cellRow][frame.cellCol];
            frame.pt[0][0].f.x = frame.pt[0][2].f.x;
            frame.pt[0][0].f.y = frame.pt[0][2].f.y;
            frame.pt[0][0].f.z = frame.pt[0][2].f.z;
            frame.pt[0][3].v = *(SubmarineVec3Int *)&((SubmarineGridVertex (*)[64])&g_SubmarineGridVertexBuffer)[frame.cellRow + 1][frame.cellCol];
            frame.pt[0][1].f.x = frame.pt[0][3].f.x;
            frame.pt[0][1].f.y = frame.pt[0][3].f.y;
            frame.pt[0][1].f.z = frame.pt[0][3].f.z;
            frame.pt[0][1].f.y = 1023.0f;
            frame.pt[0][0].f.y = frame.pt[0][1].f.y;
            frame.pt[0][3].f.y = frame.cellTopY;
            frame.pt[0][2].f.y = frame.pt[0][3].f.y;
            frame.pt[1][2].v = *(SubmarineVec3Int *)&((SubmarineGridVertex (*)[64])&g_SubmarineGridVertexBuffer)[frame.cellRow + 1][frame.cellCol];
            frame.pt[1][0].f.x = frame.pt[1][2].f.x;
            frame.pt[1][0].f.y = frame.pt[1][2].f.y;
            frame.pt[1][0].f.z = frame.pt[1][2].f.z;
            frame.pt[1][3].v = *(SubmarineVec3Int *)&((SubmarineGridVertex (*)[64])&g_SubmarineGridVertexBuffer)[frame.cellRow + 1][frame.cellCol + 1];
            frame.pt[1][1].f.x = frame.pt[1][3].f.x;
            frame.pt[1][1].f.y = frame.pt[1][3].f.y;
            frame.pt[1][1].f.z = frame.pt[1][3].f.z;
            frame.pt[1][1].f.y = 1023.0f;
            frame.pt[1][0].f.y = frame.pt[1][1].f.y;
            frame.pt[1][3].f.y = frame.cellTopY;
            frame.pt[1][2].f.y = frame.pt[1][3].f.y;
            frame.pt[2][2].v = *(SubmarineVec3Int *)&((SubmarineGridVertex (*)[64])&g_SubmarineGridVertexBuffer)[frame.cellRow + 1][frame.cellCol + 1];
            frame.pt[2][0].f.x = frame.pt[2][2].f.x;
            frame.pt[2][0].f.y = frame.pt[2][2].f.y;
            frame.pt[2][0].f.z = frame.pt[2][2].f.z;
            frame.pt[2][3].v = *(SubmarineVec3Int *)&((SubmarineGridVertex (*)[64])&g_SubmarineGridVertexBuffer)[frame.cellRow][frame.cellCol + 1];
            frame.pt[2][1].f.x = frame.pt[2][3].f.x;
            frame.pt[2][1].f.y = frame.pt[2][3].f.y;
            frame.pt[2][1].f.z = frame.pt[2][3].f.z;
            frame.pt[2][1].f.y = 1023.0f;
            frame.pt[2][0].f.y = frame.pt[2][1].f.y;
            frame.pt[2][3].f.y = frame.cellTopY;
            frame.pt[2][2].f.y = frame.pt[2][3].f.y;
            frame.pt[3][2].v = *(SubmarineVec3Int *)&((SubmarineGridVertex (*)[64])&g_SubmarineGridVertexBuffer)[frame.cellRow][frame.cellCol + 1];
            frame.pt[3][0].f.x = frame.pt[3][2].f.x;
            frame.pt[3][0].f.y = frame.pt[3][2].f.y;
            frame.pt[3][0].f.z = frame.pt[3][2].f.z;
            frame.pt[3][3].v = *(SubmarineVec3Int *)&((SubmarineGridVertex (*)[64])&g_SubmarineGridVertexBuffer)[frame.cellRow][frame.cellCol];
            frame.pt[3][1].f.x = frame.pt[3][3].f.x;
            frame.pt[3][1].f.y = frame.pt[3][3].f.y;
            frame.pt[3][1].f.z = frame.pt[3][3].f.z;
            frame.pt[3][1].f.y = 1023.0f;
            frame.pt[3][0].f.y = frame.pt[3][1].f.y;
            frame.pt[3][3].f.y = frame.cellTopY;
            frame.pt[3][2].f.y = frame.pt[3][3].f.y;
            if (frame.cellCol == 0x3F) {
                frame.pt[3][2].f.x = frame.pt[0][0].f.x + 64.0f;
                frame.pt[3][0].f.x = frame.pt[3][2].f.x;
                frame.pt[1][3].f.x = frame.pt[3][0].f.x;
                frame.pt[1][1].f.x = frame.pt[1][3].f.x;
                frame.pt[2][3].f.x = frame.pt[1][1].f.x;
                frame.pt[2][2].f.x = frame.pt[2][3].f.x;
                frame.pt[2][1].f.x = frame.pt[2][2].f.x;
                frame.pt[2][0].f.x = frame.pt[2][1].f.x;
                frame.pt[2][2].f.z = frame.pt[0][1].f.z;
                frame.pt[2][0].f.z = frame.pt[2][2].f.z;
                frame.pt[1][3].f.z = frame.pt[2][0].f.z;
                frame.pt[1][2].f.z = frame.pt[1][3].f.z;
                frame.pt[1][1].f.z = frame.pt[1][2].f.z;
                frame.pt[1][0].f.z = frame.pt[1][1].f.z;
                frame.pt[2][2].f.z = frame.pt[0][2].f.z;
                frame.pt[2][1].f.z = frame.pt[2][2].f.z;
                frame.pt[3][3].f.z = frame.pt[2][1].f.z;
                frame.pt[3][2].f.z = frame.pt[3][3].f.z;
                frame.pt[3][1].f.z = frame.pt[3][2].f.z;
                frame.pt[3][0].f.z = frame.pt[3][1].f.z;
            }
            if (frame.cellRow == 0x3F) {
                frame.pt[2][2].f.z = frame.pt[3][0].f.z - 64.0f;
                frame.pt[2][0].f.z = frame.pt[2][2].f.z;
                frame.pt[0][3].f.z = frame.pt[2][0].f.z;
                frame.pt[0][1].f.z = frame.pt[0][3].f.z;
                frame.pt[1][3].f.z = frame.pt[0][1].f.z;
                frame.pt[1][2].f.z = frame.pt[1][3].f.z;
                frame.pt[1][1].f.z = frame.pt[1][2].f.z;
                frame.pt[1][0].f.z = frame.pt[1][1].f.z;
                frame.pt[1][3].f.x = frame.pt[3][0].f.x;
                frame.pt[1][1].f.x = frame.pt[1][3].f.x;
                frame.pt[2][3].f.x = frame.pt[1][1].f.x;
                frame.pt[2][2].f.x = frame.pt[2][3].f.x;
                frame.pt[2][1].f.x = frame.pt[2][2].f.x;
                frame.pt[2][0].f.x = frame.pt[2][1].f.x;
                frame.pt[1][2].f.x = frame.pt[3][1].f.x;
                frame.pt[1][0].f.x = frame.pt[1][2].f.x;
                frame.pt[0][3].f.x = frame.pt[1][0].f.x;
                frame.pt[0][2].f.x = frame.pt[0][3].f.x;
                frame.pt[0][1].f.x = frame.pt[0][2].f.x;
                frame.pt[0][0].f.x = frame.pt[0][1].f.x;
            }
            for (frame.vtxRow = 0; frame.vtxRow < 4 && frame.emitted == 0; frame.vtxRow++) {
                {
                    for (frame.vtxCol = 0; frame.vtxCol < 4 && frame.emitted == 0; frame.vtxCol++) {
                        {
                            frame.pt[frame.vtxRow][frame.vtxCol].f.x -= 32.0f;
                            frame.pt[frame.vtxRow][frame.vtxCol].f.z += 32.0f;
                            TransformPointByMatrix4x4(&frame.composedMat, &frame.pt[frame.vtxRow][frame.vtxCol], &frame.proj[frame.vtxRow][frame.vtxCol]);
                            if (frame.proj[frame.vtxRow][frame.vtxCol].w > 0.0) {
                                frame.vtxInvW = 1.0f / frame.proj[frame.vtxRow][frame.vtxCol].w;
                                frame.proj[frame.vtxRow][frame.vtxCol].x *= frame.vtxInvW;
                                frame.proj[frame.vtxRow][frame.vtxCol].y *= frame.vtxInvW;
                                frame.proj[frame.vtxRow][frame.vtxCol].z *= frame.vtxInvW;
                                if (frame.vtxCol >> 1 != 0) {
                                    if (g_SubmarineState.auxFlags != 0) {
                                        frame.vtxColor[frame.vtxRow][frame.vtxCol] = 0x7FFF7F7F;
                                    } else {
                                        frame.vtxColor[frame.vtxRow][frame.vtxCol] = 0xFF - (g_SubmarineFadeStep << 2) << 0x18 | 0xFF7F7F;
                                    }
                                } else {
                                    frame.vtxColor[frame.vtxRow][frame.vtxCol] = 0xFF7F7F;
                                }
                            } else {
                                frame.emitted = 1;
                            }
                        }
                    }
                }
            }
            if (frame.emitted == 0 && g_SubmarineState.auxFlags == 0) {
                if (AcquireBatchNode(8, (void *)(((int)g_SubmarineRadialTerrainBatchNode))) != 0) {
                    frame.batchRec00 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor);
                    frame.batchRec00->x = frame.proj[0][0].xBits;
                    frame.batchRec00->y = frame.proj[0][0].yBits;
                    frame.batchRec00->z = frame.proj[0][0].zBits;
                    frame.batchRec00->w = frame.proj[0][0].wBits;
                    frame.batchRec00->color = frame.vtxColor[0][0];
                    frame.batchRec01 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x20);
                    frame.batchRec01->x = frame.proj[0][1].xBits;
                    frame.batchRec01->y = frame.proj[0][1].yBits;
                    frame.batchRec01->z = frame.proj[0][1].zBits;
                    frame.batchRec01->w = frame.proj[0][1].wBits;
                    frame.batchRec01->color = frame.vtxColor[0][1];
                    frame.batchRec02 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x40);
                    frame.batchRec02->x = frame.proj[0][2].xBits;
                    frame.batchRec02->y = frame.proj[0][2].yBits;
                    frame.batchRec02->z = frame.proj[0][2].zBits;
                    frame.batchRec02->w = frame.proj[0][2].wBits;
                    frame.batchRec02->color = frame.vtxColor[0][2];
                    frame.batchRec03 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x60);
                    frame.batchRec03->x = frame.proj[0][2].xBits;
                    frame.batchRec03->y = frame.proj[0][2].yBits;
                    frame.batchRec03->z = frame.proj[0][2].zBits;
                    frame.batchRec03->w = frame.proj[0][2].wBits;
                    frame.batchRec03->color = frame.vtxColor[0][2];
                    frame.batchRec04 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x80);
                    frame.batchRec04->x = frame.proj[0][1].xBits;
                    frame.batchRec04->y = frame.proj[0][1].yBits;
                    frame.batchRec04->z = frame.proj[0][1].zBits;
                    frame.batchRec04->w = frame.proj[0][1].wBits;
                    frame.batchRec04->color = frame.vtxColor[0][1];
                    frame.batchRec05 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0xA0);
                    frame.batchRec05->x = frame.proj[0][3].xBits;
                    frame.batchRec05->y = frame.proj[0][3].yBits;
                    frame.batchRec05->z = frame.proj[0][3].zBits;
                    frame.batchRec05->w = frame.proj[0][3].wBits;
                    frame.batchRec05->color = frame.vtxColor[0][3];
                    frame.batchRec06 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0xC0);
                    frame.batchRec06->x = frame.proj[1][0].xBits;
                    frame.batchRec06->y = frame.proj[1][0].yBits;
                    frame.batchRec06->z = frame.proj[1][0].zBits;
                    frame.batchRec06->w = frame.proj[1][0].wBits;
                    frame.batchRec06->color = frame.vtxColor[1][0];
                    frame.batchRec07 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0xE0);
                    frame.batchRec07->x = frame.proj[1][1].xBits;
                    frame.batchRec07->y = frame.proj[1][1].yBits;
                    frame.batchRec07->z = frame.proj[1][1].zBits;
                    frame.batchRec07->w = frame.proj[1][1].wBits;
                    frame.batchRec07->color = frame.vtxColor[1][1];
                    frame.batchRec08 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x100);
                    frame.batchRec08->x = frame.proj[1][2].xBits;
                    frame.batchRec08->y = frame.proj[1][2].yBits;
                    frame.batchRec08->z = frame.proj[1][2].zBits;
                    frame.batchRec08->w = frame.proj[1][2].wBits;
                    frame.batchRec08->color = frame.vtxColor[1][2];
                    frame.batchRec09 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x120);
                    frame.batchRec09->x = frame.proj[1][2].xBits;
                    frame.batchRec09->y = frame.proj[1][2].yBits;
                    frame.batchRec09->z = frame.proj[1][2].zBits;
                    frame.batchRec09->w = frame.proj[1][2].wBits;
                    frame.batchRec09->color = frame.vtxColor[1][2];
                    frame.batchRec10 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x140);
                    frame.batchRec10->x = frame.proj[1][1].xBits;
                    frame.batchRec10->y = frame.proj[1][1].yBits;
                    frame.batchRec10->z = frame.proj[1][1].zBits;
                    frame.batchRec10->w = frame.proj[1][1].wBits;
                    frame.batchRec10->color = frame.vtxColor[1][1];
                    frame.batchRec11 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x160);
                    frame.batchRec11->x = frame.proj[1][3].xBits;
                    frame.batchRec11->y = frame.proj[1][3].yBits;
                    frame.batchRec11->z = frame.proj[1][3].zBits;
                    frame.batchRec11->w = frame.proj[1][3].wBits;
                    frame.batchRec11->color = frame.vtxColor[1][3];
                    frame.batchRec12 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x180);
                    frame.batchRec12->x = frame.proj[2][0].xBits;
                    frame.batchRec12->y = frame.proj[2][0].yBits;
                    frame.batchRec12->z = frame.proj[2][0].zBits;
                    frame.batchRec12->w = frame.proj[2][0].wBits;
                    frame.batchRec12->color = frame.vtxColor[2][0];
                    frame.batchRec13 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x1A0);
                    frame.batchRec13->x = frame.proj[2][1].xBits;
                    frame.batchRec13->y = frame.proj[2][1].yBits;
                    frame.batchRec13->z = frame.proj[2][1].zBits;
                    frame.batchRec13->w = frame.proj[2][1].wBits;
                    frame.batchRec13->color = frame.vtxColor[2][1];
                    frame.batchRec14 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x1C0);
                    frame.batchRec14->x = frame.proj[2][2].xBits;
                    frame.batchRec14->y = frame.proj[2][2].yBits;
                    frame.batchRec14->z = frame.proj[2][2].zBits;
                    frame.batchRec14->w = frame.proj[2][2].wBits;
                    frame.batchRec14->color = frame.vtxColor[2][2];
                    frame.batchRec15 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x1E0);
                    frame.batchRec15->x = frame.proj[2][2].xBits;
                    frame.batchRec15->y = frame.proj[2][2].yBits;
                    frame.batchRec15->z = frame.proj[2][2].zBits;
                    frame.batchRec15->w = frame.proj[2][2].wBits;
                    frame.batchRec15->color = frame.vtxColor[2][2];
                    frame.batchRec16 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x200);
                    frame.batchRec16->x = frame.proj[2][1].xBits;
                    frame.batchRec16->y = frame.proj[2][1].yBits;
                    frame.batchRec16->z = frame.proj[2][1].zBits;
                    frame.batchRec16->w = frame.proj[2][1].wBits;
                    frame.batchRec16->color = frame.vtxColor[2][1];
                    frame.batchRec17 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x220);
                    frame.batchRec17->x = frame.proj[2][3].xBits;
                    frame.batchRec17->y = frame.proj[2][3].yBits;
                    frame.batchRec17->z = frame.proj[2][3].zBits;
                    frame.batchRec17->w = frame.proj[2][3].wBits;
                    frame.batchRec17->color = frame.vtxColor[2][3];
                    frame.batchRec18 = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x240);
                    frame.batchRec18->x = frame.proj[3][0].xBits;
                    frame.batchRec18->y = frame.proj[3][0].yBits;
                    frame.batchRec18->z = frame.proj[3][0].zBits;
                    frame.batchRec18->w = frame.proj[3][0].wBits;
                    frame.batchRec18->color = frame.vtxColor[3][0];
                    frame.bandSlotA = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x260);
                    frame.bandSlotA->x = frame.proj[3][1].xBits;
                    frame.bandSlotA->y = frame.proj[3][1].yBits;
                    frame.bandSlotA->z = frame.proj[3][1].zBits;
                    frame.bandSlotA->w = frame.proj[3][1].wBits;
                    frame.bandSlotA->color = frame.vtxColor[3][1];
                    frame.bandSlotB = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x280);
                    frame.bandSlotB->x = frame.proj[3][2].xBits;
                    frame.bandSlotB->y = frame.proj[3][2].yBits;
                    frame.bandSlotB->z = frame.proj[3][2].zBits;
                    frame.bandSlotB->w = frame.proj[3][2].wBits;
                    frame.bandSlotB->color = frame.vtxColor[3][2];
                    frame.bandSlotC = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x2A0);
                    frame.bandSlotC->x = frame.proj[3][2].xBits;
                    frame.bandSlotC->y = frame.proj[3][2].yBits;
                    frame.bandSlotC->z = frame.proj[3][2].zBits;
                    frame.bandSlotC->w = frame.proj[3][2].wBits;
                    frame.bandSlotC->color = frame.vtxColor[3][2];
                    frame.bandSlotD = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x2C0);
                    frame.bandSlotD->x = frame.proj[3][1].xBits;
                    frame.bandSlotD->y = frame.proj[3][1].yBits;
                    frame.bandSlotD->z = frame.proj[3][1].zBits;
                    frame.bandSlotD->w = frame.proj[3][1].wBits;
                    frame.bandSlotD->color = frame.vtxColor[3][1];
                    frame.bandSlotE = (SubmarineRadialTerrainBatch *)(g_SubmarineRadialTerrainBatchNode->vertexCursor + 0x2E0);
                    frame.bandSlotE->x = frame.proj[3][3].xBits;
                    frame.bandSlotE->y = frame.proj[3][3].yBits;
                    frame.bandSlotE->z = frame.proj[3][3].zBits;
                    frame.bandSlotE->w = frame.proj[3][3].wBits;
                    frame.bandSlotE->color = frame.vtxColor[3][3];
                }
            }
            frame.emitted = 0;
            frame.tmpInit = 0;
        }
    }
    g_SubmarineFadeStep++;
}

// Draw hook: selects scene pass 1, emits g_SubmarineRadialTerrainScaledBatchNode, emits
// g_SubmarineRadialTerrainBatchNode with culling off (the side strips are seen from both sides), and
// returns to scene pass 0.
// FUNCTION: FF7 0x0079bfbb
void __cdecl Submarine_DrawScenePassBatches(struct GfxEngineContext *frameContext)
{
    Gfx_SetScenePassCall(1, (struct GfxEngineContext *)frameContext);
    Gfx_EmitPrimitiveBatchList(g_SubmarineRadialTerrainScaledBatchNode, frameContext);
    Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, (struct GfxEngineContext *)frameContext);
    Gfx_EmitPrimitiveBatchList(g_SubmarineRadialTerrainBatchNode, frameContext);
    Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, (struct GfxEngineContext *)frameContext);
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)frameContext);
}
} /* extern "C" */

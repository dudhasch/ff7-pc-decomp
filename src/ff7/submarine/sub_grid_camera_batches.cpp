// src/ff7/submarine/sub_grid_camera_batches.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the submarine terrain grid records from the character grid and camera-oriented
//   batches, then emits those batches for each draw pass.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/api/submarine.h"
#include "lib/api/graphics.h"
#include "ff7/types/submarine.h"
#include "ff7/enums/submarine.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"

extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(strcpy, strcat)

// clang-format on

/* The record of g_SubmarineGridVertexBuffer at (row, col); the byte offset is row * 0x400 + col * 0x10. */
#define GRID_VERTEX(row, col) ((SubmarineGridVertex *)((char *)&g_SubmarineGridVertexBuffer + (((row) << 10) + ((col) << 4))))

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

/*
 * Composes the installed rotation with the screen matrix, configures two
 * matrix-backed batches, and chooses a clamped 24-by-24 cell region around the
 * camera on a 64-by-64 grid. Normally emits two triangles per cell, either
 * using each vertex color or a packed-color average selected by bit 0x20.
 */
// FUNCTION: FF7 0x007a0190
int __cdecl Submarine_BuildCameraGridBatches(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[516];
        int rowBase;
        struct { unsigned char pad_to_4[0x4]; int colBase; };
        struct { unsigned char pad_to_8[0x8]; SubmarineGridBatchEntryA *ent2Rec11; };
        struct { unsigned char pad_to_C[0xC]; SubmarineGridBatchEntryA *ent2Rec10; };
        struct { unsigned char pad_to_10[0x10]; SubmarineGridBatchEntryA *ent2Rec9; };
        struct { unsigned char pad_to_14[0x14]; SubmarineGridBatchEntryA *ent2Rec8; };
        struct { unsigned char pad_to_18[0x18]; SubmarineGridBatchEntryA *ent2Rec7; };
        struct { unsigned char pad_to_1C[0x1C]; SubmarineGridBatchEntryA *ent2Rec6; };
        struct { unsigned char pad_to_20[0x20]; SubmarineRadialTerrainBatch *modeBRec5; };
        struct { unsigned char pad_to_24[0x24]; SubmarineRadialTerrainBatch *modeBRec4; };
        struct { unsigned char pad_to_28[0x28]; SubmarineRadialTerrainBatch *modeBRec3; };
        struct { unsigned char pad_to_2C[0x2C]; SubmarineRadialTerrainBatch *modeBRec2; };
        struct { unsigned char pad_to_30[0x30]; SubmarineRadialTerrainBatch *modeBRec1; };
        struct { unsigned char pad_to_34[0x34]; SubmarineRadialTerrainBatch *modeBRec0; };
        struct { unsigned char pad_to_38[0x38]; int secondCornerIndex; };
        struct { unsigned char pad_to_3C[0x3C]; SubmarineGridBatchEntryA *ent0Rec11; };
        struct { unsigned char pad_to_40[0x40]; SubmarineGridBatchEntryA *ent0Rec10; };
        struct { unsigned char pad_to_44[0x44]; SubmarineGridBatchEntryA *ent0Rec9; };
        struct { unsigned char pad_to_48[0x48]; SubmarineGridBatchEntryA *ent0Rec8; };
        struct { unsigned char pad_to_4C[0x4C]; SubmarineGridBatchEntryA *ent0Rec7; };
        struct { unsigned char pad_to_50[0x50]; SubmarineGridBatchEntryA *ent0Rec6; };
        struct { unsigned char pad_to_54[0x54]; SubmarineGridBatchA *gridARec5; };
        struct { unsigned char pad_to_58[0x58]; SubmarineGridBatchA *gridARec4; };
        struct { unsigned char pad_to_5C[0x5C]; SubmarineGridBatchA *gridARec3; };
        struct { unsigned char pad_to_60[0x60]; SubmarineGridBatchA *gridARec2; };
        struct { unsigned char pad_to_64[0x64]; SubmarineGridBatchA *gridARec1; };
        struct { unsigned char pad_to_68[0x68]; SubmarineGridBatchA *gridARec0; };
        struct { unsigned char pad_to_6C[0x6C]; int cornerIndex; };
        struct { unsigned char pad_to_70[0x70]; SubmarineGridBatchEntryA *ent2Rec5; };
        struct { unsigned char pad_to_74[0x74]; SubmarineGridBatchEntryA *ent2Rec4; };
        struct { unsigned char pad_to_78[0x78]; SubmarineGridBatchEntryA *ent2Rec3; };
        struct { unsigned char pad_to_7C[0x7C]; SubmarineGridBatchEntryA *ent2Rec2; };
        struct { unsigned char pad_to_80[0x80]; SubmarineGridBatchEntryA *ent2Rec1; };
        struct { unsigned char pad_to_84[0x84]; SubmarineGridBatchEntryA *ent2Rec0; };
        struct { unsigned char pad_to_88[0x88]; SubmarineGridBatchEntryA *ent0Rec5; };
        struct { unsigned char pad_to_8C[0x8C]; SubmarineGridBatchEntryA *ent0Rec4; };
        struct { unsigned char pad_to_90[0x90]; SubmarineGridBatchEntryA *ent0Rec3; };
        struct { unsigned char pad_to_94[0x94]; SubmarineGridBatchEntryA *ent0Rec2; };
        struct { unsigned char pad_to_98[0x98]; SubmarineGridBatchEntryA *ent0Rec1; };
        struct { unsigned char pad_to_9C[0x9C]; SubmarineGridBatchEntryA *ent0Rec0; };
        struct { unsigned char pad_to_A0[0xA0]; SubmarineRadialTerrainBatch *camRec15; };
        struct { unsigned char pad_to_A4[0xA4]; SubmarineRadialTerrainBatch *camRec14; };
        struct { unsigned char pad_to_A8[0xA8]; SubmarineRadialTerrainBatch *camRec13; };
        struct { unsigned char pad_to_AC[0xAC]; SubmarineRadialTerrainBatch *camRec12; };
        struct { unsigned char pad_to_B0[0xB0]; SubmarineRadialTerrainBatch *camRec11; };
        struct { unsigned char pad_to_B4[0xB4]; SubmarineRadialTerrainBatch *camRec10; };
        struct { unsigned char pad_to_B8[0xB8]; SubmarineRadialTerrainBatch *camRec9; };
        struct { unsigned char pad_to_BC[0xBC]; SubmarineRadialTerrainBatch *camRec8; };
        struct { unsigned char pad_to_C0[0xC0]; SubmarineRadialTerrainBatch *modeRec5; };
        struct { unsigned char pad_to_C4[0xC4]; SubmarineRadialTerrainBatch *modeRec4; };
        struct { unsigned char pad_to_C8[0xC8]; SubmarineRadialTerrainBatch *modeRec3; };
        struct { unsigned char pad_to_CC[0xCC]; SubmarineRadialTerrainBatch *modeRec2; };
        struct { unsigned char pad_to_D0[0xD0]; SubmarineRadialTerrainBatch *modeRec1; };
        struct { unsigned char pad_to_D4[0xD4]; SubmarineRadialTerrainBatch *modeRec0; };
        struct { unsigned char pad_to_D8[0xD8]; SubmarineRadialTerrainBatch *gridRec5; };
        struct { unsigned char pad_to_DC[0xDC]; SubmarineRadialTerrainBatch *gridRec4; };
        struct { unsigned char pad_to_E0[0xE0]; SubmarineRadialTerrainBatch *gridRec3; };
        struct { unsigned char pad_to_E4[0xE4]; SubmarineRadialTerrainBatch *gridRec2; };
        struct { unsigned char pad_to_E8[0xE8]; SubmarineRadialTerrainBatch *gridRec1; };
        struct { unsigned char pad_to_EC[0xEC]; SubmarineRadialTerrainBatch *gridRec0; };
        struct { unsigned char pad_to_F0[0xF0]; SubmarineRadialTerrainBatch *camRec7; };
        struct { unsigned char pad_to_F4[0xF4]; SubmarineRadialTerrainBatch *camRec6; };
        struct { unsigned char pad_to_F8[0xF8]; SubmarineRadialTerrainBatch *camRec5; };
        struct { unsigned char pad_to_FC[0xFC]; SubmarineRadialTerrainBatch *camRec4; };
        struct { unsigned char pad_to_100[0x100]; SubmarineRadialTerrainBatch *camRec3; };
        struct { unsigned char pad_to_104[0x104]; SubmarineRadialTerrainBatch *camRec2; };
        struct { unsigned char pad_to_108[0x108]; SubmarineRadialTerrainBatch *camRec1; };
        struct { unsigned char pad_to_10C[0x10C]; SubmarineRadialTerrainBatch *camRec0; };
        struct { unsigned char pad_to_110[0x110]; int cornerIdx; };
        struct { unsigned char pad_to_114[0x114]; GfxClipVec4 corners[4]; };
        struct { unsigned char pad_to_154[0x154]; int colStart; };
        struct { unsigned char pad_to_158[0x158]; float invW; };
        struct { unsigned char pad_to_15C[0x15C]; int gridCol; };
        struct { unsigned char pad_to_160[0x160]; int gridRow; };
        struct { unsigned char pad_to_164[0x164]; int playerRow; };
        struct { unsigned char pad_to_168[0x168]; int rowEnd; };
        struct { unsigned char pad_to_16C[0x16C]; GfxMatrix4x4 screenMatrix; };
        struct { unsigned char pad_to_1AC[0x1AC]; int packedColor; };
        struct { unsigned char pad_to_1B0[0x1B0]; int clipCount; };
        struct { unsigned char pad_to_1B4[0x1B4]; int colEnd; };
        struct { unsigned char pad_to_1B8[0x1B8]; struct GfxEngineContext *engineCtx; };
        struct { unsigned char pad_to_1BC[0x1BC]; GfxMatrix4x4 rotMatrix; };
        struct { unsigned char pad_to_1FC[0x1FC]; int playerCol; };
        struct { unsigned char pad_to_200[0x200]; int rowStart; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.engineCtx = GetEngineContext();
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), &frame.rotMatrix);
    ComposeWithScreenMatrix(&frame.rotMatrix, &frame.screenMatrix);
    Gfx_SetBatchContextMatrix(0, (int)(&frame.rotMatrix), ((int)g_SubmarineModeBatchList0));
    Gfx_SetBatchContextMatrix(0, (int)(&frame.rotMatrix), ((int)g_SubmarineModeBatchList2));
    frame.playerCol = (g_SubmarineState.player.posX >> 12) + 2048 >> 6;
    frame.playerRow = 63 - ((g_SubmarineState.player.posZ >> 12) + 2048 >> 6);
    if (frame.playerCol - 12 > 0) {
        frame.colBase = frame.playerCol - 0xC;
    } else {
        frame.colBase = 0;
    }
    frame.colStart = frame.colBase;
    frame.colEnd = frame.colStart + 24;
    if (frame.colEnd > 63) {
        frame.colEnd = 63;
        frame.colStart = 39;
    }
    if (frame.playerRow - 12 > 0) {
        frame.rowBase = frame.playerRow - 0xC;
    } else {
        frame.rowBase = 0;
    }
    frame.rowStart = frame.rowBase;
    frame.rowEnd = frame.rowStart + 24;
    if (frame.rowEnd > 63) {
        frame.rowEnd = 63;
        frame.rowStart = 39;
    }
    if ((g_SubmarineFlags & SUBMARINE_FLAG_DEBUG_WIREFRAME) != 0) {
        for (frame.gridRow = 0; frame.gridRow < 63; frame.gridRow++) {
            for (frame.gridCol = 0; frame.gridCol < 63; frame.gridCol++) {
                frame.packedColor = 0xFF000000;
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow << 10) + (frame.gridCol << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[0]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow << 10) + (frame.gridCol + 1 << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[1]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow + 1 << 10) + (frame.gridCol << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[2]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow + 1 << 10) + (frame.gridCol + 1 << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[3]);
                frame.packedColor += GRID_VERTEX(frame.gridRow, frame.gridCol)->color >> 2;
                frame.packedColor += GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color >> 2;
                frame.packedColor += GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color >> 2;
                frame.packedColor += GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->color >> 2;
                frame.clipCount = 0;
                for (frame.cornerIdx = 0; frame.cornerIdx < 4; frame.cornerIdx++) {
                    if (frame.corners[frame.cornerIdx].w < 1.0f) {
                        frame.clipCount++;
                    } else {
                        frame.invW = 1.0f / frame.corners[frame.cornerIdx].w;
                        frame.corners[frame.cornerIdx].x *= frame.invW;
                        frame.corners[frame.cornerIdx].y *= frame.invW;
                        frame.corners[frame.cornerIdx].z *= frame.invW;
                    }
                }
                if (frame.clipCount == 0 && frame.engineCtx->driverType != GFX_DRIVER_SOFTWARE) {
                    if (AcquireBatchNodeNoCommandArea(2, g_SubmarineLineBatchList) != 0) {
                        frame.camRec0 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor);
                        frame.camRec0->x = frame.corners[0].xBits;
                        frame.camRec0->y = frame.corners[0].yBits;
                        frame.camRec0->z = frame.corners[0].zBits;
                        frame.camRec0->w = frame.corners[0].wBits;
                        frame.camRec0->color = 0xFF7F7F7F;
                        frame.camRec1 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x20);
                        frame.camRec1->x = frame.corners[1].xBits;
                        frame.camRec1->y = frame.corners[1].yBits;
                        frame.camRec1->z = frame.corners[1].zBits;
                        frame.camRec1->w = frame.corners[1].wBits;
                        frame.camRec1->color = 0xFF7F7F7F;
                        frame.camRec2 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x40);
                        frame.camRec2->x = frame.corners[1].xBits;
                        frame.camRec2->y = frame.corners[1].yBits;
                        frame.camRec2->z = frame.corners[1].zBits;
                        frame.camRec2->w = frame.corners[1].wBits;
                        frame.camRec2->color = 0xFF7F7F7F;
                        frame.camRec3 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x60);
                        frame.camRec3->x = frame.corners[3].xBits;
                        frame.camRec3->y = frame.corners[3].yBits;
                        frame.camRec3->z = frame.corners[3].zBits;
                        frame.camRec3->w = frame.corners[3].wBits;
                        frame.camRec3->color = 0xFF7F7F7F;
                    }
                    if (frame.gridCol == 0) {
                        if (AcquireBatchNodeNoCommandArea(1, g_SubmarineLineBatchList) != 0) {
                            frame.camRec4 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor);
                            frame.camRec4->x = frame.corners[2].xBits;
                            frame.camRec4->y = frame.corners[2].yBits;
                            frame.camRec4->z = frame.corners[2].zBits;
                            frame.camRec4->w = frame.corners[2].wBits;
                            frame.camRec4->color = 0xFF7F7F7F;
                            frame.camRec5 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x20);
                            frame.camRec5->x = frame.corners[0].xBits;
                            frame.camRec5->y = frame.corners[0].yBits;
                            frame.camRec5->z = frame.corners[0].zBits;
                            frame.camRec5->w = frame.corners[0].wBits;
                            frame.camRec5->color = 0xFF7F7F7F;
                        }
                    }
                    if (frame.gridRow == 62) {
                        if (AcquireBatchNodeNoCommandArea(1, g_SubmarineLineBatchList) != 0) {
                            frame.camRec6 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor);
                            frame.camRec6->x = frame.corners[3].xBits;
                            frame.camRec6->y = frame.corners[3].yBits;
                            frame.camRec6->z = frame.corners[3].zBits;
                            frame.camRec6->w = frame.corners[3].wBits;
                            frame.camRec6->color = 0xFF7F7F7F;
                            frame.camRec7 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x20);
                            frame.camRec7->x = frame.corners[2].xBits;
                            frame.camRec7->y = frame.corners[2].yBits;
                            frame.camRec7->z = frame.corners[2].zBits;
                            frame.camRec7->w = frame.corners[2].wBits;
                            frame.camRec7->color = 0xFF7F7F7F;
                        }
                    }
                    if (frame.gridRow >= frame.rowStart && frame.gridRow < frame.rowEnd && frame.gridCol >= frame.colStart && frame.gridCol < frame.colEnd) {
                        if ((g_SubmarineFlags & SUBMARINE_FLAG_NO_TERRAIN_SHADE) == 0) {
                            if (AcquireBatchNode(2, (void *)(((int)g_SubmarineGridBatchListA))) != 0) {
                                frame.gridRec0 = (SubmarineRadialTerrainBatch *)(g_SubmarineGridBatchListA->vertexCursor);
                                frame.gridRec0->x = frame.corners[0].xBits;
                                frame.gridRec0->y = frame.corners[0].yBits;
                                frame.gridRec0->z = frame.corners[0].zBits;
                                frame.gridRec0->w = frame.corners[0].wBits;
                                frame.gridRec0->color = GRID_VERTEX(frame.gridRow, frame.gridCol)->color;
                                frame.gridRec1 = (SubmarineRadialTerrainBatch *)(g_SubmarineGridBatchListA->vertexCursor + 0x20);
                                frame.gridRec1->x = frame.corners[1].xBits;
                                frame.gridRec1->y = frame.corners[1].yBits;
                                frame.gridRec1->z = frame.corners[1].zBits;
                                frame.gridRec1->w = frame.corners[1].wBits;
                                frame.gridRec1->color = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color;
                                frame.gridRec2 = (SubmarineRadialTerrainBatch *)(g_SubmarineGridBatchListA->vertexCursor + 0x40);
                                frame.gridRec2->x = frame.corners[2].xBits;
                                frame.gridRec2->y = frame.corners[2].yBits;
                                frame.gridRec2->z = frame.corners[2].zBits;
                                frame.gridRec2->w = frame.corners[2].wBits;
                                frame.gridRec2->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color;
                                frame.gridRec3 = (SubmarineRadialTerrainBatch *)(g_SubmarineGridBatchListA->vertexCursor + 0x60);
                                frame.gridRec3->x = frame.corners[2].xBits;
                                frame.gridRec3->y = frame.corners[2].yBits;
                                frame.gridRec3->z = frame.corners[2].zBits;
                                frame.gridRec3->w = frame.corners[2].wBits;
                                frame.gridRec3->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color;
                                frame.gridRec4 = (SubmarineRadialTerrainBatch *)(g_SubmarineGridBatchListA->vertexCursor + 0x80);
                                frame.gridRec4->x = frame.corners[1].xBits;
                                frame.gridRec4->y = frame.corners[1].yBits;
                                frame.gridRec4->z = frame.corners[1].zBits;
                                frame.gridRec4->w = frame.corners[1].wBits;
                                frame.gridRec4->color = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color;
                                frame.gridRec5 = (SubmarineRadialTerrainBatch *)(g_SubmarineGridBatchListA->vertexCursor + 0xA0);
                                frame.gridRec5->x = frame.corners[3].xBits;
                                frame.gridRec5->y = frame.corners[3].yBits;
                                frame.gridRec5->z = frame.corners[3].zBits;
                                frame.gridRec5->w = frame.corners[3].wBits;
                                frame.gridRec5->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->color;
                            }
                        } else if (AcquireBatchNode(2, (void *)(((int)g_SubmarineModeBatchList1))) != 0) {
                            frame.modeRec0 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor);
                            frame.modeRec0->x = frame.corners[0].xBits;
                            frame.modeRec0->y = frame.corners[0].yBits;
                            frame.modeRec0->z = frame.corners[0].zBits;
                            frame.modeRec0->w = frame.corners[0].wBits;
                            frame.modeRec0->color = frame.packedColor;
                            frame.modeRec1 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0x20);
                            frame.modeRec1->x = frame.corners[1].xBits;
                            frame.modeRec1->y = frame.corners[1].yBits;
                            frame.modeRec1->z = frame.corners[1].zBits;
                            frame.modeRec1->w = frame.corners[1].wBits;
                            frame.modeRec1->color = frame.packedColor;
                            frame.modeRec2 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0x40);
                            frame.modeRec2->x = frame.corners[2].xBits;
                            frame.modeRec2->y = frame.corners[2].yBits;
                            frame.modeRec2->z = frame.corners[2].zBits;
                            frame.modeRec2->w = frame.corners[2].wBits;
                            frame.modeRec2->color = frame.packedColor;
                            frame.modeRec3 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0x60);
                            frame.modeRec3->x = frame.corners[2].xBits;
                            frame.modeRec3->y = frame.corners[2].yBits;
                            frame.modeRec3->z = frame.corners[2].zBits;
                            frame.modeRec3->w = frame.corners[2].wBits;
                            frame.modeRec3->color = frame.packedColor;
                            frame.modeRec4 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0x80);
                            frame.modeRec4->x = frame.corners[1].xBits;
                            frame.modeRec4->y = frame.corners[1].yBits;
                            frame.modeRec4->z = frame.corners[1].zBits;
                            frame.modeRec4->w = frame.corners[1].wBits;
                            frame.modeRec4->color = frame.packedColor;
                            frame.modeRec5 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0xA0);
                            frame.modeRec5->x = frame.corners[3].xBits;
                            frame.modeRec5->y = frame.corners[3].yBits;
                            frame.modeRec5->z = frame.corners[3].zBits;
                            frame.modeRec5->w = frame.corners[3].wBits;
                            frame.modeRec5->color = frame.packedColor;
                        }
                    }
                } else if (frame.clipCount < 4 || frame.engineCtx->driverType == GFX_DRIVER_SOFTWARE) {
                    if (AcquireBatchNodeNoCommandArea(2, g_SubmarineLineBatchList) != 0) {
                        frame.camRec8 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor);
                        frame.camRec8->x = frame.corners[0].xBits;
                        frame.camRec8->y = frame.corners[0].yBits;
                        frame.camRec8->z = frame.corners[0].zBits;
                        frame.camRec8->w = frame.corners[0].wBits;
                        frame.camRec8->color = 0xFF7F7F7F;
                        frame.camRec9 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x20);
                        frame.camRec9->x = frame.corners[1].xBits;
                        frame.camRec9->y = frame.corners[1].yBits;
                        frame.camRec9->z = frame.corners[1].zBits;
                        frame.camRec9->w = frame.corners[1].wBits;
                        frame.camRec9->color = 0xFF7F7F7F;
                        frame.camRec10 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x40);
                        frame.camRec10->x = frame.corners[1].xBits;
                        frame.camRec10->y = frame.corners[1].yBits;
                        frame.camRec10->z = frame.corners[1].zBits;
                        frame.camRec10->w = frame.corners[1].wBits;
                        frame.camRec10->color = 0xFF7F7F7F;
                        frame.camRec11 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x60);
                        frame.camRec11->x = frame.corners[3].xBits;
                        frame.camRec11->y = frame.corners[3].yBits;
                        frame.camRec11->z = frame.corners[3].zBits;
                        frame.camRec11->w = frame.corners[3].wBits;
                        frame.camRec11->color = 0xFF7F7F7F;
                    }
                    if (frame.gridCol == 0) {
                        if (AcquireBatchNodeNoCommandArea(1, g_SubmarineLineBatchList) != 0) {
                            frame.camRec12 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor);
                            frame.camRec12->x = frame.corners[2].xBits;
                            frame.camRec12->y = frame.corners[2].yBits;
                            frame.camRec12->z = frame.corners[2].zBits;
                            frame.camRec12->w = frame.corners[2].wBits;
                            frame.camRec12->color = 0xFF7F7F7F;
                            frame.camRec13 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x20);
                            frame.camRec13->x = frame.corners[0].xBits;
                            frame.camRec13->y = frame.corners[0].yBits;
                            frame.camRec13->z = frame.corners[0].zBits;
                            frame.camRec13->w = frame.corners[0].wBits;
                            frame.camRec13->color = 0xFF7F7F7F;
                        }
                    }
                    if (frame.gridRow == 62) {
                        if (AcquireBatchNodeNoCommandArea(1, g_SubmarineLineBatchList) != 0) {
                            frame.camRec14 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor);
                            frame.camRec14->x = frame.corners[3].xBits;
                            frame.camRec14->y = frame.corners[3].yBits;
                            frame.camRec14->z = frame.corners[3].zBits;
                            frame.camRec14->w = frame.corners[3].wBits;
                            frame.camRec14->color = 0xFF7F7F7F;
                            frame.camRec15 = (SubmarineRadialTerrainBatch *)(g_SubmarineLineBatchList->vertexCursor + 0x20);
                            frame.camRec15->x = frame.corners[2].xBits;
                            frame.camRec15->y = frame.corners[2].yBits;
                            frame.camRec15->z = frame.corners[2].zBits;
                            frame.camRec15->w = frame.corners[2].wBits;
                            frame.camRec15->color = 0xFF7F7F7F;
                        }
                    }
                    if (frame.gridRow >= frame.rowStart && frame.gridRow < frame.rowEnd && frame.gridCol >= frame.colStart && frame.gridCol < frame.colEnd) {
                        if ((g_SubmarineFlags & SUBMARINE_FLAG_NO_TERRAIN_SHADE) == 0) {
                            if (AcquireBatchNode(2, (void *)(((int)g_SubmarineModeBatchList0))) != 0) {
                                frame.ent0Rec0 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor);
                                frame.ent0Rec0->x = GRID_VERTEX(frame.gridRow, frame.gridCol)->x;
                                frame.ent0Rec0->y = GRID_VERTEX(frame.gridRow, frame.gridCol)->y;
                                frame.ent0Rec0->z = GRID_VERTEX(frame.gridRow, frame.gridCol)->z;
                                frame.ent0Rec0->color = GRID_VERTEX(frame.gridRow, frame.gridCol)->color;
                                frame.ent0Rec1 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0x20);
                                frame.ent0Rec1->x = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->x;
                                frame.ent0Rec1->y = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->y;
                                frame.ent0Rec1->z = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->z;
                                frame.ent0Rec1->color = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color;
                                frame.ent0Rec2 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0x40);
                                frame.ent0Rec2->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->x;
                                frame.ent0Rec2->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->y;
                                frame.ent0Rec2->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->z;
                                frame.ent0Rec2->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color;
                                frame.ent0Rec3 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0x60);
                                frame.ent0Rec3->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->x;
                                frame.ent0Rec3->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->y;
                                frame.ent0Rec3->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->z;
                                frame.ent0Rec3->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color;
                                frame.ent0Rec4 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0x80);
                                frame.ent0Rec4->x = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->x;
                                frame.ent0Rec4->y = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->y;
                                frame.ent0Rec4->z = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->z;
                                frame.ent0Rec4->color = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color;
                                frame.ent0Rec5 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0xA0);
                                frame.ent0Rec5->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->x;
                                frame.ent0Rec5->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->y;
                                frame.ent0Rec5->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->z;
                                frame.ent0Rec5->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->color;
                            }
                        } else if (AcquireBatchNode(2, (void *)(((int)g_SubmarineModeBatchList2))) != 0) {
                            frame.ent2Rec0 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor);
                            frame.ent2Rec0->x = GRID_VERTEX(frame.gridRow, frame.gridCol)->x;
                            frame.ent2Rec0->y = GRID_VERTEX(frame.gridRow, frame.gridCol)->y;
                            frame.ent2Rec0->z = GRID_VERTEX(frame.gridRow, frame.gridCol)->z;
                            frame.ent2Rec0->color = frame.packedColor;
                            frame.ent2Rec1 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0x20);
                            frame.ent2Rec1->x = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->x;
                            frame.ent2Rec1->y = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->y;
                            frame.ent2Rec1->z = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->z;
                            frame.ent2Rec1->color = frame.packedColor;
                            frame.ent2Rec2 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0x40);
                            frame.ent2Rec2->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->x;
                            frame.ent2Rec2->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->y;
                            frame.ent2Rec2->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->z;
                            frame.ent2Rec2->color = frame.packedColor;
                            frame.ent2Rec3 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0x60);
                            frame.ent2Rec3->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->x;
                            frame.ent2Rec3->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->y;
                            frame.ent2Rec3->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->z;
                            frame.ent2Rec3->color = frame.packedColor;
                            frame.ent2Rec4 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0x80);
                            frame.ent2Rec4->x = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->x;
                            frame.ent2Rec4->y = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->y;
                            frame.ent2Rec4->z = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->z;
                            frame.ent2Rec4->color = frame.packedColor;
                            frame.ent2Rec5 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0xA0);
                            frame.ent2Rec5->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->x;
                            frame.ent2Rec5->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->y;
                            frame.ent2Rec5->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->z;
                            frame.ent2Rec5->color = frame.packedColor;
                        }
                    }
                }
            }
        }
    } else if ((g_SubmarineFlags & SUBMARINE_FLAG_NO_TERRAIN_SHADE) == 0) {
        for (frame.gridRow = frame.rowStart; frame.gridRow < frame.rowEnd; frame.gridRow++) {
            for (frame.gridCol = frame.colStart; frame.gridCol < frame.colEnd; frame.gridCol++) {
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow << 10) + (frame.gridCol << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[0]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow << 10) + (frame.gridCol + 1 << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[1]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow + 1 << 10) + (frame.gridCol << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[2]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow + 1 << 10) + (frame.gridCol + 1 << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[3]);
                frame.clipCount = 0;
                if (frame.engineCtx->driverType != GFX_DRIVER_SOFTWARE) {
                    for (frame.cornerIndex = 0; frame.cornerIndex < 4; frame.cornerIndex++) {
                        if (frame.corners[frame.cornerIndex].w < 1.0f) {
                            frame.clipCount++;
                        } else {
                            frame.invW = 1.0f / frame.corners[frame.cornerIndex].w;
                            frame.corners[frame.cornerIndex].x *= frame.invW;
                            frame.corners[frame.cornerIndex].y *= frame.invW;
                            frame.corners[frame.cornerIndex].z *= frame.invW;
                        }
                    }
                }
                if (frame.clipCount == 0 && frame.engineCtx->driverType != GFX_DRIVER_SOFTWARE) {
                    if (AcquireBatchNode(2, (void *)(((int)g_SubmarineGridBatchListA))) != 0) {
                        frame.gridARec0 = (SubmarineGridBatchA *)(g_SubmarineGridBatchListA->vertexCursor);
                        frame.gridARec0->x = frame.corners[0].xBits;
                        frame.gridARec0->y = frame.corners[0].yBits;
                        frame.gridARec0->z = frame.corners[0].zBits;
                        frame.gridARec0->w = frame.corners[0].wBits;
                        frame.gridARec0->gridValue = GRID_VERTEX(frame.gridRow, frame.gridCol)->color;
                        frame.gridARec1 = (SubmarineGridBatchA *)(g_SubmarineGridBatchListA->vertexCursor + 0x20);
                        frame.gridARec1->x = frame.corners[1].xBits;
                        frame.gridARec1->y = frame.corners[1].yBits;
                        frame.gridARec1->z = frame.corners[1].zBits;
                        frame.gridARec1->w = frame.corners[1].wBits;
                        frame.gridARec1->gridValue = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color;
                        frame.gridARec2 = (SubmarineGridBatchA *)(g_SubmarineGridBatchListA->vertexCursor + 0x40);
                        frame.gridARec2->x = frame.corners[2].xBits;
                        frame.gridARec2->y = frame.corners[2].yBits;
                        frame.gridARec2->z = frame.corners[2].zBits;
                        frame.gridARec2->w = frame.corners[2].wBits;
                        frame.gridARec2->gridValue = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color;
                        frame.gridARec3 = (SubmarineGridBatchA *)(g_SubmarineGridBatchListA->vertexCursor + 0x60);
                        frame.gridARec3->x = frame.corners[2].xBits;
                        frame.gridARec3->y = frame.corners[2].yBits;
                        frame.gridARec3->z = frame.corners[2].zBits;
                        frame.gridARec3->w = frame.corners[2].wBits;
                        frame.gridARec3->gridValue = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color;
                        frame.gridARec4 = (SubmarineGridBatchA *)(g_SubmarineGridBatchListA->vertexCursor + 0x80);
                        frame.gridARec4->x = frame.corners[1].xBits;
                        frame.gridARec4->y = frame.corners[1].yBits;
                        frame.gridARec4->z = frame.corners[1].zBits;
                        frame.gridARec4->w = frame.corners[1].wBits;
                        frame.gridARec4->gridValue = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color;
                        frame.gridARec5 = (SubmarineGridBatchA *)(g_SubmarineGridBatchListA->vertexCursor + 0xA0);
                        frame.gridARec5->x = frame.corners[3].xBits;
                        frame.gridARec5->y = frame.corners[3].yBits;
                        frame.gridARec5->z = frame.corners[3].zBits;
                        frame.gridARec5->w = frame.corners[3].wBits;
                        frame.gridARec5->gridValue = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->color;
                    }
                } else if (frame.clipCount < 4 || frame.engineCtx->driverType == GFX_DRIVER_SOFTWARE) {
                    if (AcquireBatchNode(2, (void *)(((int)g_SubmarineModeBatchList0))) != 0) {
                        frame.ent0Rec6 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor);
                        frame.ent0Rec6->x = GRID_VERTEX(frame.gridRow, frame.gridCol)->x;
                        frame.ent0Rec6->y = GRID_VERTEX(frame.gridRow, frame.gridCol)->y;
                        frame.ent0Rec6->z = GRID_VERTEX(frame.gridRow, frame.gridCol)->z;
                        frame.ent0Rec6->color = GRID_VERTEX(frame.gridRow, frame.gridCol)->color;
                        frame.ent0Rec7 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0x20);
                        frame.ent0Rec7->x = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->x;
                        frame.ent0Rec7->y = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->y;
                        frame.ent0Rec7->z = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->z;
                        frame.ent0Rec7->color = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color;
                        frame.ent0Rec8 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0x40);
                        frame.ent0Rec8->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->x;
                        frame.ent0Rec8->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->y;
                        frame.ent0Rec8->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->z;
                        frame.ent0Rec8->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color;
                        frame.ent0Rec9 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0x60);
                        frame.ent0Rec9->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->x;
                        frame.ent0Rec9->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->y;
                        frame.ent0Rec9->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->z;
                        frame.ent0Rec9->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color;
                        frame.ent0Rec10 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0x80);
                        frame.ent0Rec10->x = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->x;
                        frame.ent0Rec10->y = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->y;
                        frame.ent0Rec10->z = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->z;
                        frame.ent0Rec10->color = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color;
                        frame.ent0Rec11 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList0->vertexCursor + 0xA0);
                        frame.ent0Rec11->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->x;
                        frame.ent0Rec11->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->y;
                        frame.ent0Rec11->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->z;
                        frame.ent0Rec11->color = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->color;
                    }
                }
            }
        }
    } else {
        for (frame.gridRow = frame.rowStart; frame.gridRow < frame.rowEnd; frame.gridRow++) {
            for (frame.gridCol = frame.colStart; frame.gridCol < frame.colEnd; frame.gridCol++) {
                frame.packedColor = 0xFF000000;
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow << 10) + (frame.gridCol << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[0]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow << 10) + (frame.gridCol + 1 << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[1]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow + 1 << 10) + (frame.gridCol << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[2]);
                TransformPointByMatrix4x4(&frame.screenMatrix, (frame.gridRow + 1 << 10) + (frame.gridCol + 1 << 4) + (char *)&g_SubmarineGridVertexBuffer, &frame.corners[3]);
                frame.packedColor += GRID_VERTEX(frame.gridRow, frame.gridCol)->color >> 2;
                frame.packedColor += GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->color >> 2;
                frame.packedColor += GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->color >> 2;
                frame.packedColor += GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->color >> 2;
                frame.clipCount = 0;
                if (frame.engineCtx->driverType != GFX_DRIVER_SOFTWARE) {
                    for (frame.secondCornerIndex = 0; frame.secondCornerIndex < 4; frame.secondCornerIndex++) {
                        if (frame.corners[frame.secondCornerIndex].w < 1.0f) {
                            frame.clipCount++;
                        } else {
                            frame.invW = 1.0f / frame.corners[frame.secondCornerIndex].w;
                            frame.corners[frame.secondCornerIndex].x *= frame.invW;
                            frame.corners[frame.secondCornerIndex].y *= frame.invW;
                            frame.corners[frame.secondCornerIndex].z *= frame.invW;
                        }
                    }
                }
                if (frame.clipCount == 0 && frame.engineCtx->driverType != GFX_DRIVER_SOFTWARE) {
                    if (AcquireBatchNode(2, (void *)(((int)g_SubmarineModeBatchList1))) != 0) {
                        frame.modeBRec0 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor);
                        frame.modeBRec0->x = frame.corners[0].xBits;
                        frame.modeBRec0->y = frame.corners[0].yBits;
                        frame.modeBRec0->z = frame.corners[0].zBits;
                        frame.modeBRec0->w = frame.corners[0].wBits;
                        frame.modeBRec0->color = frame.packedColor;
                        frame.modeBRec1 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0x20);
                        frame.modeBRec1->x = frame.corners[1].xBits;
                        frame.modeBRec1->y = frame.corners[1].yBits;
                        frame.modeBRec1->z = frame.corners[1].zBits;
                        frame.modeBRec1->w = frame.corners[1].wBits;
                        frame.modeBRec1->color = frame.packedColor;
                        frame.modeBRec2 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0x40);
                        frame.modeBRec2->x = frame.corners[2].xBits;
                        frame.modeBRec2->y = frame.corners[2].yBits;
                        frame.modeBRec2->z = frame.corners[2].zBits;
                        frame.modeBRec2->w = frame.corners[2].wBits;
                        frame.modeBRec2->color = frame.packedColor;
                        frame.modeBRec3 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0x60);
                        frame.modeBRec3->x = frame.corners[2].xBits;
                        frame.modeBRec3->y = frame.corners[2].yBits;
                        frame.modeBRec3->z = frame.corners[2].zBits;
                        frame.modeBRec3->w = frame.corners[2].wBits;
                        frame.modeBRec3->color = frame.packedColor;
                        frame.modeBRec4 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0x80);
                        frame.modeBRec4->x = frame.corners[1].xBits;
                        frame.modeBRec4->y = frame.corners[1].yBits;
                        frame.modeBRec4->z = frame.corners[1].zBits;
                        frame.modeBRec4->w = frame.corners[1].wBits;
                        frame.modeBRec4->color = frame.packedColor;
                        frame.modeBRec5 = (SubmarineRadialTerrainBatch *)(g_SubmarineModeBatchList1->vertexCursor + 0xA0);
                        frame.modeBRec5->x = frame.corners[3].xBits;
                        frame.modeBRec5->y = frame.corners[3].yBits;
                        frame.modeBRec5->z = frame.corners[3].zBits;
                        frame.modeBRec5->w = frame.corners[3].wBits;
                        frame.modeBRec5->color = frame.packedColor;
                    }
                } else if (frame.clipCount < 4 || frame.engineCtx->driverType == GFX_DRIVER_SOFTWARE) {
                    if (AcquireBatchNode(2, (void *)(((int)g_SubmarineModeBatchList2))) != 0) {
                        frame.ent2Rec6 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor);
                        frame.ent2Rec6->x = GRID_VERTEX(frame.gridRow, frame.gridCol)->x;
                        frame.ent2Rec6->y = GRID_VERTEX(frame.gridRow, frame.gridCol)->y;
                        frame.ent2Rec6->z = GRID_VERTEX(frame.gridRow, frame.gridCol)->z;
                        frame.ent2Rec6->color = frame.packedColor;
                        frame.ent2Rec7 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0x20);
                        frame.ent2Rec7->x = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->x;
                        frame.ent2Rec7->y = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->y;
                        frame.ent2Rec7->z = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->z;
                        frame.ent2Rec7->color = frame.packedColor;
                        frame.ent2Rec8 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0x40);
                        frame.ent2Rec8->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->x;
                        frame.ent2Rec8->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->y;
                        frame.ent2Rec8->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->z;
                        frame.ent2Rec8->color = frame.packedColor;
                        frame.ent2Rec9 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0x60);
                        frame.ent2Rec9->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->x;
                        frame.ent2Rec9->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->y;
                        frame.ent2Rec9->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol)->z;
                        frame.ent2Rec9->color = frame.packedColor;
                        frame.ent2Rec10 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0x80);
                        frame.ent2Rec10->x = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->x;
                        frame.ent2Rec10->y = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->y;
                        frame.ent2Rec10->z = GRID_VERTEX(frame.gridRow, frame.gridCol + 1)->z;
                        frame.ent2Rec10->color = frame.packedColor;
                        frame.ent2Rec11 = (SubmarineGridBatchEntryA *)(g_SubmarineModeBatchList2->vertexCursor + 0xA0);
                        frame.ent2Rec11->x = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->x;
                        frame.ent2Rec11->y = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->y;
                        frame.ent2Rec11->z = GRID_VERTEX(frame.gridRow + 1, frame.gridCol + 1)->z;
                        frame.ent2Rec11->color = frame.packedColor;
                    }
                }
            }
        }
    }
    return 0;
}

/*
 * Draws one pass of this module's primitive lists into the frame context frameContext:
 * opens scene pass 0 with Gfx_SetScenePassCall, then emits two batch lists
 * chosen by bit 0x20 of g_SubmarineFlags -- g_SubmarineGridBatchListA and
 * g_SubmarineModeBatchList0 when the bit is clear, g_SubmarineModeBatchList1
 * and g_SubmarineModeBatchList2 when it is set.
 */
// FUNCTION: FF7 0x007a2ae5
void __cdecl Submarine_EmitGridBatchesByMode(struct GfxEngineContext *frameContext)
{
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)(frameContext));
    if ((g_SubmarineFlags & SUBMARINE_FLAG_NO_TERRAIN_SHADE) == 0) {
        Gfx_EmitPrimitiveBatchList(g_SubmarineGridBatchListA, frameContext);
        Gfx_EmitPrimitiveBatchList(g_SubmarineModeBatchList0, frameContext);
    } else {
        Gfx_EmitPrimitiveBatchList(g_SubmarineModeBatchList1, frameContext);
        Gfx_EmitPrimitiveBatchList(g_SubmarineModeBatchList2, frameContext);
    }
}

/*
 * Fills the 64x64 sixteen-byte records at g_SubmarineGridVertexBuffer from
 * charGrid's character grid. Derives XYZ from grid coordinates and the unsigned
 * byte minus 0x30, initializes packed color 0xFF0080C0 and adjusts its low two
 * bytes by that value.
 */
// FUNCTION: FF7 0x007a2b51
void __cdecl Submarine_BuildGridRecordsFromCharGrid(void *charGrid)
{
    int column;
    int rowIdx;
    unsigned char cellVal;

    for (rowIdx = 0; rowIdx < 64; rowIdx++) {
        for (column = 0; column < 64; column++) {
            GRID_VERTEX(rowIdx, column)->color = 0xFF0080C0;
            *(float *)&GRID_VERTEX(rowIdx, column)->x = (float)((column - 0x20 << 6) + 0x20);
            cellVal = *(char *)((char *)charGrid + ((rowIdx << 6) + column)) - 0x30;
            *(float *)&GRID_VERTEX(rowIdx, column)->y = (float)((cellVal << 6 << 12) + 0x1FF000 >> 12);
            *(float *)&GRID_VERTEX(rowIdx, column)->z = (float)((64 - (rowIdx + 0x20) << 6) - 0x20);
            *(char *)&GRID_VERTEX(rowIdx, column)->color = 0xC0 - cellVal * 0x15;
            *((char *)&GRID_VERTEX(rowIdx, column)->color + 1) = 0x80 - (cellVal << 4);
        }
    }
}
} /* extern "C" */

// src/ff7/submarine/sub_grid_init_and_step.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Initializes the submarine terrain grid from height bytes, allocates grid record slots,
//   and steps the cell heights each frame.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/submarine.h"
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

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

// Initialises the 64 by 64 grid of 4096 cells from the signed bytes at heightBytes.
// Each byte gets g_SubmarineWorkingLevelParams[1] added and ASCII '0' subtracted, is clamped to 0
// through 16, and is scaled by 64 into height, currentHeight and raisedHeight, with flags cleared.
// A second pass fills eastHeight, westHeight, southHeight and northHeight from the neighbouring
// cells' currentHeight, using 0x3C0 at the grid edges.
// FUNCTION: FF7 0x0079f5c0
void __cdecl Submarine_InitGridHeightsFromBytes(int heightBytes)
{
    int bytePtr;
    int cellIdx;
    SubmarineGridCell *cellPtr;
    int baseLevel;
    int cellHeight;
    int hE;
    int southH;
    int westHeight;
    int upperH;

    baseLevel = g_SubmarineWorkingLevelParams[1];
    bytePtr = heightBytes;
    cellPtr = &g_SubmarineGridCellTable[0];
    do {
        cellHeight = *(char *)bytePtr + baseLevel + -0x30;
        bytePtr++;
        if (cellHeight < 0) {
            cellHeight = 0;
        }
        if (cellHeight > 0x10) {
            cellHeight = 0x10;
        }
        cellPtr->height = cellHeight << 6;
        cellPtr->currentHeight = cellPtr->height;
        cellPtr->raisedHeight = cellPtr->currentHeight;
        cellPtr->flags = 0;
        cellPtr++;
    } while (cellPtr < &g_SubmarineGridCellTable[4096]);
    cellIdx = 0;
    cellPtr = &g_SubmarineGridCellTable[0];
    do {
        if ((cellIdx % 64) == 0x3F) {
            hE = 0x3C0;
        } else {
            hE = cellPtr[1].currentHeight;
        }
        cellPtr->eastHeight = hE;
        if ((cellIdx % 64) == 0) {
            westHeight = 0x3C0;
        } else {
            westHeight = cellPtr[-1].currentHeight;
        }
        cellPtr->westHeight = westHeight;
        if (((cellIdx) / 64) == 0x3F) {
            southH = 0x3C0;
        } else {
            southH = cellPtr[0x40].currentHeight;
        }
        cellPtr->southHeight = southH;
        if (((cellIdx) / 64) == 0) {
            upperH = 0x3C0;
        } else {
            upperH = cellPtr[-0x40].currentHeight;
        }
        cellPtr->northHeight = upperH;
        cellPtr++;
        cellIdx++;
    } while (cellIdx < 0x1000);
}

/*
 * Caps the requested positive count at 500, then repeatedly scans 4096
 * cells. Considers cells with SUBMARINE_CELL_FLAGGED clear, excludes the
 * five-by-five grid area around g_SubmarineState.currentCellIndex, and requires their height to
 * exceed g_SubmarineWorkingLevelParams[2]/4096.
 */
// FUNCTION: FF7 0x0079f74f
void __cdecl Submarine_AllocateGridRecordSlots(short value)
{
    struct {
        int randomHeight;
        int dquot;
        int drem;
        int found;
        int cellIndex;
        int state;
        int rem;
        int limit;
        int block;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.limit = g_SubmarineWorkingLevelParams[2] / 0x1000;
    frame.state = g_SubmarineState.currentCellIndex;
    frame.block = frame.state / 64;
    frame.rem = frame.state % 64;
    if (value > 500)
        value = 500;
    while (value > 0) {
        frame.found = 0;
        frame.cellIndex = 0;
        do {
            if ((g_SubmarineGridCellTable[frame.cellIndex].flags & SUBMARINE_CELL_FLAGGED) == 0) {
                frame.found = 1;
                frame.dquot = frame.cellIndex / 64 - frame.block;
                frame.drem = frame.cellIndex % 64 - frame.rem;
                if (frame.dquot >= -2 && frame.dquot <= 2 && frame.drem >= -2 && frame.drem <= 2) { /* empty first arm: lib/codegen.h */
                } else {
                    if (g_SubmarineGridCellTable[frame.cellIndex].height <= frame.limit) { /* empty first arm: lib/codegen.h */
                    } else {
                        if ((short)rand() < 127) {
                            g_SubmarineGridCellTable[frame.cellIndex].flags = SUBMARINE_CELL_FLAGGED;
                            frame.randomHeight = (rand() << 3) / 0x7fff;
                            frame.randomHeight = 0x400 - (frame.randomHeight << 6);
                            if (frame.randomHeight < 0)
                                frame.randomHeight = 0;
                            if (frame.randomHeight > g_SubmarineGridCellTable[frame.cellIndex].height) {
                                frame.randomHeight = g_SubmarineGridCellTable[frame.cellIndex].height;
                            }
                            g_SubmarineGridCellTable[frame.cellIndex].raisedHeight = (short)frame.randomHeight;
                            value--;
                            if (value <= 0)
                                break;
                        }
                    }
                }
            }
            frame.cellIndex++;
        } while (frame.cellIndex < 0x1000);
        if (frame.found == 0)
            break;
    }
}

/*
 * Updates all 4096 cells of the 64 by 64 grid. Moves currentHeight toward raisedHeight when
 * SUBMARINE_CELL_RAISED is set, otherwise toward height: upward steps are 1 or 2 and
 * downward steps are 1 or one quarter of the difference.
 */
// FUNCTION: FF7 0x0079f903
void __cdecl Submarine_StepGridCellHeights(void)
{
    int goal;
    unsigned int idx;
    SubmarineGridCell *cellPtr;
    SubmarineGridCell *westAddress;
    int currentHeight;
    SubmarineGridCell *southAddress;
    SubmarineGridCell *upAddress;
    SubmarineGridCell *nextAddr;
    int riseDelta;
    int dropDelta;

    idx = 0;
    cellPtr = &g_SubmarineGridCellTable[0];
    westAddress = cellPtr - 1;
    nextAddr = cellPtr + 1;
    upAddress = cellPtr - 0x40;
    southAddress = cellPtr + 0x40;
    do {
        currentHeight = cellPtr->currentHeight;
        goal = cellPtr->height;
        if ((cellPtr->flags & SUBMARINE_CELL_RAISED) != 0) {
            goal = cellPtr->raisedHeight;
        }
        if (currentHeight != goal) {
            if (currentHeight < goal) {
                riseDelta = goal - currentHeight;
                if (riseDelta > 2) {
                    currentHeight += 2;
                } else {
                    currentHeight++;
                }
            } else {
                dropDelta = currentHeight - goal;
                if (dropDelta > 4) {
                    currentHeight -= dropDelta / 4;
                } else {
                    currentHeight--;
                }
            }
            cellPtr->currentHeight = currentHeight;
            if (idx % 64 != 0) {
                westAddress->eastHeight = currentHeight;
            }
            if (idx % 64 != 63) {
                nextAddr->westHeight = currentHeight;
            }
            if (idx / 64 != 63) {
                southAddress->northHeight = currentHeight;
            }
            if (idx / 64 != 0) {
                upAddress->southHeight = currentHeight;
            }
        } else {
            cellPtr->flags &= 0xEFFF;
        }
        if ((cellPtr->flags & SUBMARINE_CELL_TIMED) != 0) {
            currentHeight = cellPtr->flags & 0xFF;
            goal = cellPtr->flags & 0xFF00;
            currentHeight--;
            if (currentHeight < 0) {
                currentHeight = 0;
                goal = 0;
                cellPtr->raisedHeight = cellPtr->height;
            }
            cellPtr->flags = goal | currentHeight;
        }
        cellPtr++;
        westAddress++;
        nextAddr++;
        southAddress++;
        upAddress++;
        idx++;
    } while (idx < 0x1000);
}
} /* extern "C" */

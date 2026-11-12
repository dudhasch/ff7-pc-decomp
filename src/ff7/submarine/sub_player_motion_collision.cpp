// src/ff7/submarine/sub_player_motion_collision.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Updates the player submarine from its controls, steps the record interpolation, and
//   handles motion, grid collision probing and the rotation basis.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/submarine.h"
#include "ff7/enums/submarine.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "ff7/types/submarine.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "lib/types/graphics.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"

extern "C" {
// clang-format on

#pragma intrinsic(abs)

#pragma intrinsic(strcpy, strcat)

#define g_SubmarineInterpStartAngleAHigh (*(short *)((char *)&g_SubmarineRecordInterpBaseA + 2))
#define g_SubmarineInterpTargetAngleAHigh (*(short *)((char *)&g_SubmarineRecordInterpTargetA + 2))
#define g_SubmarineInterpDeltaAngleAHigh (*(short *)((char *)&g_SubmarineRecordInterpDeltaA + 2))

void __cdecl Submarine_DrawVerticalSliceQuads(void);

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

// When control is unlocked, adjusts submarine speed-table selection, depth and orientation from
// held input, including boundary selector entries 0 and 18 after an initial 1-through-17 clamp.
// Depth changes are bounded by the global floor, 0x400000 and a per-record limit. Updates
// secondary orientation with mode-dependent lag/threshold behavior, wraps heading and changes
// bounded signed tilt indices from the separate input mask, mapping them through centered lookup
// tables. Pressed input cycles four view modes or toggles an alternate view and signals transform
// rebuilding; the rebuild helper runs outside the control gate when its flag is set. Preserves
// signed centered-table indexing and the source mode/threshold expressions; table layouts and
// divisors remain assumed.
// FUNCTION: FF7 0x00798580
void __cdecl Submarine_UpdatePlayerControls(void)
{
    int buttons;
    int batchNumber;
    int course;
    int angleX;
    int camRotY;
    int selHorizHeld;
    int offsetX;
    int axisY;
    int gridIdxX;
    int rollAngle;
    int viewIndex;
    int newDepthSpan;
    int edgeSel;
    int speedIdx;
    int viewX;
    int movedVertex;
    int camRotZ;
    int offsetY;
    int height;
    int axisZ;
    int axisJ;
    int altView;

    if (g_SubmarineResultCode == 0) {
        buttons = g_InputPressedMask;
        speedIdx = g_SubmarineState.speedScaleIndex;
        if (Input_GetHeld(INPUT_TRIANGLE) != 0) {
            speedIdx++;
        }
        if (Input_GetHeld(INPUT_CROSS) != 0) {
            speedIdx--;
        }
        if (speedIdx > 17) {
            speedIdx = 17;
        }
        if (speedIdx < 1) {
            speedIdx = 1;
        }
        g_SubmarineState.speedScaleIndex = speedIdx;
        g_SubmarineState.speedScale = g_SubmarineState.speedScaleTable[speedIdx];
        if (Input_GetPressed(INPUT_SQUARE) != 0) {
            Submarine_LaunchTorpedoRecord();
        }
        edgeSel = g_SubmarineState.speedScaleIndex;
        if (Input_GetHeld(INPUT_TRIANGLE) != 0 && edgeSel == 0x11) {
            edgeSel++;
        }
        if (Input_GetHeld(INPUT_CROSS) != 0 && edgeSel == 1) {
            edgeSel--;
        }
        g_SubmarineState.speedScaleIndex = edgeSel;
        g_SubmarineState.speedScale = g_SubmarineState.speedScaleTable[edgeSel];
        newDepthSpan = g_SubmarineState.player.posY;
        if (Input_GetHeld(INPUT_L1) != 0) {
            newDepthSpan -= 0x1000;
        }
        if (Input_GetHeld(INPUT_L2) != 0) {
            newDepthSpan += 0x1000;
        }
        if (newDepthSpan < g_SubmarineWorkingLevelParams[2]) {
            newDepthSpan = g_SubmarineWorkingLevelParams[2];
        }
        if (newDepthSpan > 4194304) {
            newDepthSpan = 0x400000;
        }
        if (newDepthSpan < g_SubmarineGridCellTable[g_SubmarineState.currentCellIndex].height << 12) {
            g_SubmarineState.player.posY = newDepthSpan;
        }
        angleX = g_SubmarineState.player.angle;
        course = g_SubmarineState.player.yaw;
        batchNumber = g_SubmarineState.player.roll;
        if (Input_GetHeld(INPUT_UP) != 0) {
            angleX -= 4;
        }
        if (Input_GetHeld(INPUT_DOWN) != 0) {
            angleX += 4;
        }
        if (Input_GetHeld(INPUT_RIGHT) != 0) {
            course += 8;
        }
        if (Input_GetHeld(INPUT_LEFT) != 0) {
            course -= 8;
        }
        if (angleX > 0x3F0) {
            angleX = 0x3F0;
        }
        if (angleX < -1008) {
            angleX = -1008;
        }
        if (course > 2048) {
            course -= 0x1000;
        }
        if (course < -2048) {
            course += 0x1000;
        }
        g_SubmarineState.player.angle = angleX;
        g_SubmarineState.player.yaw = course;
        g_SubmarineState.player.roll = batchNumber;
        viewX = g_SubmarineState.camRotX;
        camRotY = g_SubmarineState.camRotY;
        camRotZ = g_SubmarineState.camRotZ;
        selHorizHeld = 0;
        movedVertex = 0;
        if (Input_GetHeld(INPUT_UP) != 0) {
            movedVertex = 1;
            viewX -= 4;
        }
        if (Input_GetHeld(INPUT_DOWN) != 0) {
            movedVertex = 1;
            viewX += 4;
        }
        if (Input_GetHeld(INPUT_RIGHT) != 0) {
            selHorizHeld = 1;
            camRotY += 8;
        }
        if (Input_GetHeld(INPUT_LEFT) != 0) {
            selHorizHeld = 1;
            camRotY -= 8;
        }
        if (viewX > 1008) {
            viewX = 1008;
        }
        if (viewX < -1008) {
            viewX = -1008;
        }
        if (camRotY > 0x800) {
            camRotY -= 0x1000 /* 360 deg */;
        }
        if (camRotY < -2048) {
            camRotY += 0x1000 /* 360 deg */;
        }
        if (g_SubmarineState.tableIndex != 3 && g_SubmarineState.tableIndex != 2 && g_SubmarineState.auxFlags == 0) {
            offsetX = g_SubmarineState.camRotX - g_SubmarineState.player.angle;
            if (movedVertex != 0) {
                if (offsetX <= -0x80) {
                    offsetX = -128;
                    g_SubmarineState.camRotX = viewX;
                } else if (offsetX >= 0x80) {
                    offsetX = 0x80;
                    g_SubmarineState.camRotX = viewX;
                }
            } else if (offsetX < 0) {
                g_SubmarineState.camRotX += 4;
            } else if (offsetX > 0) {
                g_SubmarineState.camRotX -= 4;
            }
            offsetY = g_SubmarineState.camRotY - g_SubmarineState.player.yaw;
            if (offsetY >= 0x200) {
                offsetY -= 0x1000;
            }
            if (offsetY <= -512) {
                offsetY += 0x1000;
            }
            if (selHorizHeld != 0) {
                if (offsetY <= -256) {
                    offsetY = -256;
                    g_SubmarineState.camRotY = camRotY;
                } else if (offsetY >= 0x100) {
                    offsetY = 0x100;
                    g_SubmarineState.camRotY = camRotY;
                }
            } else if (offsetY < 0) {
                g_SubmarineState.camRotY += 8;
            } else if (offsetY > 0) {
                g_SubmarineState.camRotY -= 8;
            }
        } else {
            g_SubmarineState.camRotX = viewX;
            g_SubmarineState.camRotY = camRotY;
        }
        g_SubmarineState.camRotZ = camRotZ;
        height = g_SubmarineState.height;
        rollAngle = g_SubmarineState.aimYaw;
        gridIdxX = g_SubmarineState.gridIndexX;
        if (Input_GetHeld(INPUT_RIGHT) != 0) {
            rollAngle += 8;
        }
        if (Input_GetHeld(INPUT_LEFT) != 0) {
            rollAngle -= 8;
        }
        if (rollAngle > 0x800) {
            rollAngle -= 0x1000 /* 360 deg */;
        }
        if (rollAngle < -2048) {
            rollAngle += 0x1000 /* 360 deg */;
        }
        g_SubmarineState.height = height;
        g_SubmarineState.aimYaw = rollAngle;
        g_SubmarineState.gridIndexX = gridIdxX;
        axisY = g_SubmarineState.gridIndexY;
        axisJ = g_SubmarineState.gridIndexJ;
        axisZ = g_SubmarineState.gridIndexZ;
        if (g_SubmarineState.controlState == 0 || g_SubmarineState.speedScaleIndex == 9) {
            buttons &= ~INPUT_DPAD_MASK;
        }
        if ((buttons & INPUT_UP) != 0) {
            axisY++;
        } else if ((buttons & INPUT_DOWN) != 0) {
            axisY--;
        } else {
            if (axisY > 0) {
                axisY--;
            }
            if (axisY < 0) {
                axisY++;
            }
        }
        if ((buttons & INPUT_RIGHT) != 0) {
            axisJ--;
            axisZ++;
        } else if ((buttons & INPUT_LEFT) != 0) {
            axisJ++;
            axisZ--;
        } else {
            if (axisJ > 0) {
                axisJ--;
            }
            if (axisJ < 0) {
                axisJ++;
            }
            if (axisZ > 0) {
                axisZ--;
            }
            if (axisZ < 0) {
                axisZ++;
            }
        }
        if (axisY < -16) {
            axisY = -16;
        }
        if (axisJ < -16) {
            axisJ = -16;
        }
        if (axisZ < -16) {
            axisZ = -16;
        }
        if (axisY > 16) {
            axisY = 16;
        }
        if (axisJ > 16) {
            axisJ = 16;
        }
        if (axisZ > 16) {
            axisZ = 16;
        }
        g_SubmarineState.gridIndexY = axisY;
        g_SubmarineState.gridIndexJ = axisJ;
        g_SubmarineState.gridIndexZ = axisZ;
        g_SubmarineState.axisTermX = (g_SubmarineState.lookupTableY + 16)[axisY];
        g_SubmarineState.axisTermY = (g_SubmarineState.lookupTableX + 16)[axisJ];
        g_SubmarineState.axisTermZ = (g_SubmarineState.lookupTableZ + 16)[axisZ];
        g_SubmarineState.player.roll = (g_SubmarineState.batchCountTable + 16)[axisZ];
        viewIndex = g_SubmarineState.tableIndex;
        if (Input_GetPressed(INPUT_R2) != 0) {
            viewIndex++;
            if (viewIndex > 3) {
                viewIndex = 0;
            }
            Submarine_ArmRecordInterpolation(viewIndex);
            g_SubmarineRecordInterpActive = 1;
            g_SubmarineState.auxFlags = 0;
            g_SubmarineState.tableIndex = viewIndex;
        } else if (Input_GetPressed(INPUT_R1) != 0) {
            altView = g_SubmarineState.auxFlags ^ 1;
            g_SubmarineState.auxFlags = altView;
            if (altView == 1) {
                viewIndex = 4;
            }
            Submarine_ArmRecordInterpolation(viewIndex);
            g_SubmarineRecordInterpActive = 1;
        }
    }
    if (g_SubmarineRecordInterpActive != 0) {
        Submarine_StepRecordInterpolation();
    }
}

// Prepares interpolation from current submarine state to the indexed 56-byte target record. Adjusts
// the first current angle by g_SubmarineState.camRotX when switching the target state flag, then
// stores three start/target angles and their shortest masked 12-bit differences with one
// plus/minus-4096 correction. Stores two additional short start/target values and direct
// differences, resets interpolation progress to 0 and limit to 256, and copies the target
// control/state globals.
// FUNCTION: FF7 0x00798d5a
void __cdecl Submarine_ArmRecordInterpolation(int targetIndex)
{
    int angleDelta;
    int cur;
    int target;

    cur = g_SubmarineState.rotationStatePointer;
    if (g_SubmarineState.currentFlag == 0 && g_SubmarineState.tableEntries[targetIndex].stateFlag != 0) {
        cur += g_SubmarineState.camRotX;
    } else if (g_SubmarineState.currentFlag != 0 && g_SubmarineState.tableEntries[targetIndex].stateFlag == 0) {
        cur -= g_SubmarineState.camRotX;
    }
    g_SubmarineRecordInterpBaseA = cur;
    target = *(int *)&g_SubmarineState.tableEntries[targetIndex].rotation;
    g_SubmarineRecordInterpTargetA = target;
    angleDelta = (target + 0x1000 & 0xFFF) - (cur + 0x1000 & 0xFFF);
    if (angleDelta > 0x800) {
        angleDelta -= 0x1000 /* 360 deg */;
    }
    if (angleDelta < (-2048)) {
        angleDelta += 0x1000 /* 360 deg */;
    }
    g_SubmarineRecordInterpDeltaA = angleDelta;
    cur = g_SubmarineState.currentAngleA;
    g_SubmarineInterpStartAngleAHigh = cur;
    target = *(int *)&g_SubmarineState.tableEntries[targetIndex].currentAngleA;
    g_SubmarineInterpTargetAngleAHigh = target;
    angleDelta = (target + 0x1000 & 0xFFF) - (cur + 0x1000 & 0xFFF);
    if (angleDelta > 0x800) {
        angleDelta -= 0x1000 /* 360 deg */;
    }
    if (angleDelta < (-2048)) {
        angleDelta += 0x1000 /* 360 deg */;
    }
    g_SubmarineInterpDeltaAngleAHigh = angleDelta;
    cur = g_SubmarineState.currentAngleB;
    g_SubmarineRecordInterpBaseB = cur;
    target = *(int *)&g_SubmarineState.tableEntries[targetIndex].currentAngleB;
    g_SubmarineRecordInterpTargetB = target;
    angleDelta = (target + 0x1000 & 0xFFF) - (cur + 0x1000 & 0xFFF);
    if (angleDelta > 0x800) {
        angleDelta -= 0x1000 /* 360 deg */;
    }
    if (angleDelta < (-2048)) {
        angleDelta += 0x1000 /* 360 deg */;
    }
    g_SubmarineRecordInterpDeltaB = angleDelta;
    cur = g_SubmarineState.camOffset;
    g_SubmarineRecordInterpBaseC = (short)cur;
    target = g_SubmarineState.tableEntries[targetIndex].camOffset;
    g_SubmarineRecordInterpTargetC = (short)target;
    g_SubmarineRecordInterpDeltaC = (short)(target - cur);
    cur = g_SubmarineState.geomScreen;
    g_SubmarineRecordInterpBaseD = (short)cur;
    target = g_SubmarineState.tableEntries[targetIndex].geomScreen;
    g_SubmarineRecordInterpTargetD = (short)target;
    g_SubmarineRecordInterpDeltaD = (short)(target - cur);
    g_SubmarineRecordInterpFraction = 0;
    g_SubmarineRecordInterpStep = 256;
    g_SubmarineState.controlState = g_SubmarineState.tableEntries[targetIndex].controlState;
    g_SubmarineState.currentFlag = g_SubmarineState.tableEntries[targetIndex].stateFlag;
    g_SubmarineTorpedoLaunchOffsetFactor = g_SubmarineState.tableEntries[targetIndex].torpedoLaunchOffsetFactor;
    g_SubmarineTorpedoForwardOffsetFactor = g_SubmarineState.tableEntries[targetIndex].torpedoForwardOffsetFactor;
    g_SubmarineState.currentBandBase = g_SubmarineState.tableEntries[targetIndex].bandBase;
    g_SubmarineState.currentBandStep = g_SubmarineState.tableEntries[targetIndex].bandStep;
    g_SubmarineState.currentBandOffset = g_SubmarineState.tableEntries[targetIndex].bandOffset;
}

// Steps the interpolation armed by Submarine_StartStateLerp16, once per call. When
// g_SubmarineResultCode is set it also bleeds the engine context's farPlane down by one fiftieth
// of itself. While the progress g_SubmarineRecordInterpFraction is below 0x1000 it
// writes start plus delta times progress shifted right by 12 (4.12 fixed point) into
// g_SubmarineState.rotationStatePointer, g_SubmarineState.currentAngleA, g_SubmarineState.currentAngleB,
// g_SubmarineState.camOffset and g_SubmarineState.currentAngleC, then advances the progress by the step
// g_SubmarineRecordInterpStep. Once the progress reaches 0x1000 it snaps all five to their stored
// end values, zeroes the progress and clears the active flag g_SubmarineRecordInterpActive.
// FUNCTION: FF7 0x00799043
void __cdecl Submarine_StepRecordInterpolation(void)
{
    GfxEngineContext *pEngine;
    int scaledA;
    int fraction;
    int scaledB;
    int scaledC;

    if (g_SubmarineResultCode != 0) {
        pEngine = GetEngineContext();
        pEngine->farPlane -= pEngine->farPlane / 5e+01f;
    }
    if (g_SubmarineRecordInterpFraction < 4096) {
        fraction = g_SubmarineRecordInterpFraction;
        scaledA = g_SubmarineRecordInterpDeltaA;
        scaledB = g_SubmarineInterpDeltaAngleAHigh;
        scaledC = g_SubmarineRecordInterpDeltaB;
        scaledA *= fraction;
        scaledB *= fraction;
        scaledC *= fraction;
        g_SubmarineState.rotationStatePointer = g_SubmarineRecordInterpBaseA + (scaledA >> 12);
        g_SubmarineState.currentAngleA = g_SubmarineInterpStartAngleAHigh + (scaledB >> 12);
        g_SubmarineState.currentAngleB = g_SubmarineRecordInterpBaseB + (scaledC >> 12);
        scaledA = g_SubmarineRecordInterpDeltaC;
        scaledB = g_SubmarineRecordInterpDeltaD;
        scaledA *= fraction;
        scaledB *= fraction;
        g_SubmarineState.camOffset = g_SubmarineRecordInterpBaseC + (scaledA >> 12);
        g_SubmarineState.geomScreen = g_SubmarineRecordInterpBaseD + (scaledB >> 12);
        g_SubmarineRecordInterpFraction += g_SubmarineRecordInterpStep;
    } else {
        g_SubmarineState.rotationStatePointer = g_SubmarineRecordInterpTargetA;
        g_SubmarineState.currentAngleA = g_SubmarineInterpTargetAngleAHigh;
        g_SubmarineState.currentAngleB = g_SubmarineRecordInterpTargetB;
        g_SubmarineState.camOffset = g_SubmarineRecordInterpTargetC;
        g_SubmarineState.geomScreen = g_SubmarineRecordInterpTargetD;
        g_SubmarineRecordInterpFraction = 0;
        g_SubmarineRecordInterpActive = 0;
    }
}

// Updates submarine player motion and collision state from fixed-point orientation/speed globals.
// Builds direction/rotation data, computes candidate position and reads the candidate map-height
// cell before checking world bounds. Out-of-range or floor-crossing motion sets collision bit two
// and can relax negative pitch; ceiling crossing clamps Y and relaxes positive pitch, while
// accepted motion stores XYZ. Computes a reference point with X/Z displacement explicitly zeroed
// and an angle-dependent Y calculation, then clamps the current map-cell index. A flagged
// unconsumed cell within the narrow vertical range marks flags 0x8080, subtracts 2048 from the
// player resource, increases a global by six times its configured value and plays sound. Combines
// twelve directional collision probes, discarding bit zero from the first four before the
// remainder, and ORs the result into collision state. Advances four packed cooldown records from
// state zero to timed state one and then state two; unsupported states remain unchanged.
// FUNCTION: FF7 0x007991c6
void __cdecl Submarine_UpdateMotionAndCollision(void)
{
    int scale;
    int refX;
    int cellNum;
    int gridH;
    int gridCell;
    int heightDelta;
    int dirZVal;
    int yHeading;
    int probeOfsY;
    int probeXOff;
    int basisXVal;
    int floorLimit;
    int probeZ;
    int candidateYVal;
    int pointZ;
    int cellCol;
    int hudBonus;
    int headingXDir;
    int loopCollision;
    int rightZ;
    int counter;
    int n;
    int cooldownValue;
    int candPosX;
    int cellRow;
    int cost;
    int auxPitchAngle;
    int refY;
    int offsetZ;
    int basisY;

    Submarine_BuildDirectionVectorFromAngles((struct SubmarineDirectionAngles *)&g_SubmarineState.player.angle, (struct SubmarineDirectionVector *)&g_SubmarineState.directionX);
    Submarine_BuildRotationBasisFromAngles((struct SubmarineRotationAngles *)&g_SubmarineState.player.angle, (struct SubmarineRecordMatrixRow *)&g_SubmarineState.rightBasisX);
    Submarine_BuildDirectionVectorFromAngles((struct SubmarineDirectionAngles *)&g_SubmarineState.height, (struct SubmarineDirectionVector *)&g_SubmarineState.aimDirX);
    g_SubmarineState.motionFlags = 0;
    candPosX = g_SubmarineState.directionX;
    candidateYVal = g_SubmarineState.directionY;
    probeZ = g_SubmarineState.directionZ;
    scale = g_SubmarineState.speedScale;
    candPosX = (candPosX * scale >> 12) + g_SubmarineState.player.posX;
    candidateYVal = (candidateYVal * scale >> 12) + g_SubmarineState.player.posY;
    probeZ = (probeZ * scale >> 12) + g_SubmarineState.player.posZ;
    cellNum = ((2048 - (probeZ >> 12)) / 64 << 6) + (((candPosX >> 12) + 2048) / 64);
    gridH = g_SubmarineGridCellTable[cellNum].height << 12;
    floorLimit = g_SubmarineWorkingLevelParams[2];
    if (candPosX < -8388608 || (candPosX > 8388608 || (probeZ < -8388608 || probeZ > 8388608))) {
        g_SubmarineState.motionFlags |= SUBMARINE_MOTION_NEAR_TERRAIN;
    } else if (candidateYVal >= gridH) {
        if (g_SubmarineState.player.angle < 0) {
            if (g_SubmarineState.player.angle <= -0x10) {
                g_SubmarineState.player.angle += 0x10;
                g_SubmarineState.camRotX += 0x10;
            } else {
                g_SubmarineState.player.angle = 0;
                g_SubmarineState.camRotX = g_SubmarineState.player.angle;
            }
        }
        g_SubmarineState.motionFlags |= SUBMARINE_MOTION_NEAR_TERRAIN;
    } else if (candidateYVal < floorLimit) {
        if (g_SubmarineState.player.angle > 0) {
            if (g_SubmarineState.player.angle >= 0x10) {
                g_SubmarineState.player.angle -= 0x10;
                g_SubmarineState.camRotX -= 0x10;
            } else {
                g_SubmarineState.player.angle = 0;
                g_SubmarineState.camRotX = g_SubmarineState.player.angle;
            }
        }
        g_SubmarineState.player.posX = candPosX;
        g_SubmarineState.player.posY = floorLimit;
        g_SubmarineState.player.posZ = probeZ;
    } else {
        g_SubmarineState.player.posX = candPosX;
        g_SubmarineState.player.posY = candidateYVal;
        g_SubmarineState.player.posZ = probeZ;
    }
    refX = g_SubmarineState.aimDirX;
    refY = g_SubmarineState.orientationY;
    pointZ = g_SubmarineState.orientationZ;
    pointZ = 0;
    refX = pointZ;
    refY = 0x410000 - g_SubmarineState.player.posY >> 12;
    auxPitchAngle = -g_SubmarineState.height % 4096;
    if (auxPitchAngle > 0 && auxPitchAngle < 0x400) {
        auxPitchAngle = SinFixed(auxPitchAngle);
        if (auxPitchAngle != 0) {
            auxPitchAngle = (0x410000 - g_SubmarineState.player.posY) / auxPitchAngle;
            if (auxPitchAngle < 0x7FFF) {
                refX *= auxPitchAngle;
                refY *= auxPitchAngle;
                pointZ *= auxPitchAngle;
            }
        }
    }
    refX = (g_SubmarineState.player.posX >> 12) + refX;
    refY = (g_SubmarineState.player.posY >> 12) + refY;
    pointZ = (g_SubmarineState.player.posZ >> 12) + pointZ;
    g_SubmarineState.referencePointX = refX;
    g_SubmarineState.referencePointY = refY;
    g_SubmarineState.referencePointZ = pointZ;
    cellCol = g_SubmarineState.player.posX >> 12;
    cellRow = g_SubmarineState.player.posZ >> 12;
    cellCol = (cellCol + 2048) / 64;
    if (cellCol < 0) {
        cellCol = 0;
    }
    if (cellCol >= 64) {
        cellCol = 63;
    }
    cellRow = (2048 - cellRow) / 64;
    if (cellRow < 0) {
        cellRow = 0;
    }
    if (cellRow >= 64) {
        cellRow = 63;
    }
    gridCell = (cellRow << 6) + cellCol;
    g_SubmarineState.currentCellIndex = gridCell;
    if ((g_SubmarineGridCellTable[gridCell].flags & SUBMARINE_CELL_FLAGGED) != 0 && (g_SubmarineGridCellTable[gridCell].flags & SUBMARINE_CELL_TIMED) == 0) {
        cost = 0;
        hudBonus = 0;
        heightDelta = (g_SubmarineState.player.posY >> 12) - g_SubmarineGridCellTable[gridCell].raisedHeight;
        if (heightDelta > -64 && heightDelta < 0) {
            g_SubmarineGridCellTable[gridCell].flags |= SUBMARINE_CELL_TIMED | SUBMARINE_CELL_TIMER_INIT;
            cost = 0x800;
            hudBonus = g_SubmarineHudOffset * 6;
            g_SubmarineState.player.playerResource -= cost;
            g_SubmarineScreenShake.accumulator += hudBonus;
            SoundDispatchCommand(SOUND_SFX_PLAY_CH4, 0x40, 0x229, 0, 0, 0, 0, 0, 0);
        }
    }
    loopCollision = 0;
    headingXDir = g_SubmarineState.directionX;
    yHeading = g_SubmarineState.directionY;
    dirZVal = g_SubmarineState.directionZ;
    probeXOff = g_SubmarineState.probeOffsetX;
    probeOfsY = g_SubmarineState.probeOffsetY;
    offsetZ = g_SubmarineState.probeOffsetZ;
    loopCollision |= Submarine_ProbeGridCellCollision(headingXDir + probeXOff, yHeading + probeOfsY, dirZVal + offsetZ);
    loopCollision |= Submarine_ProbeGridCellCollision(headingXDir - probeXOff, yHeading - probeOfsY, dirZVal - offsetZ);
    loopCollision |= Submarine_ProbeGridCellCollision(-headingXDir + probeXOff, -yHeading + probeOfsY, -dirZVal + offsetZ);
    loopCollision |= Submarine_ProbeGridCellCollision(-headingXDir - probeXOff, -yHeading - probeOfsY, -dirZVal - offsetZ);
    loopCollision &= ~SUBMARINE_MOTION_HIT_TERRAIN;
    loopCollision |= Submarine_ProbeGridCellCollision(headingXDir, yHeading, dirZVal);
    loopCollision |= Submarine_ProbeGridCellCollision(headingXDir << 1, yHeading << 1, dirZVal << 1);
    loopCollision |= Submarine_ProbeGridCellCollision(-headingXDir, -yHeading, -dirZVal);
    loopCollision |= Submarine_ProbeGridCellCollision(-headingXDir << 1, -yHeading << 1, -dirZVal << 1);
    basisXVal = g_SubmarineState.rightBasisX;
    basisY = g_SubmarineState.rightBasisY;
    rightZ = g_SubmarineState.rightBasisZ;
    loopCollision |= Submarine_ProbeGridCellCollision((headingXDir + basisXVal) / 2, (yHeading + basisY) / 2, (dirZVal + rightZ) / 2);
    loopCollision |= Submarine_ProbeGridCellCollision((headingXDir - basisXVal) / 2, (yHeading - basisY) / 2, (dirZVal - rightZ) / 2);
    loopCollision |= Submarine_ProbeGridCellCollision((-headingXDir + basisXVal) / 2, (-yHeading + basisY) / 2, (-dirZVal + rightZ) / 2);
    loopCollision |= Submarine_ProbeGridCellCollision((-headingXDir - basisXVal) / 2, (-yHeading - basisY) / 2, (-dirZVal - rightZ) / 2);
    g_SubmarineState.motionFlags |= loopCollision;
    counter = 0;
    do {
        cooldownValue = g_SubmarineState.cooldownTable[counter];
        switch ((unsigned int)cooldownValue >> 16 << 16) {
        case SUBMARINE_LAUNCHER_FIRED:
            cooldownValue = g_SubmarineHudScale | SUBMARINE_LAUNCHER_RELOADING;
            break;
        case SUBMARINE_LAUNCHER_RELOADING:
            n = cooldownValue & SUBMARINE_LAUNCHER_TIMER_MASK;
            n--;
            if (n <= 0) {
                cooldownValue = SUBMARINE_LAUNCHER_READY;
            } else {
                cooldownValue = n & SUBMARINE_LAUNCHER_TIMER_MASK | SUBMARINE_LAUNCHER_RELOADING;
            }
        }
        g_SubmarineState.cooldownTable[counter] = cooldownValue;
        counter++;
    } while (counter < 4);
}

// Computes fixed sine/cosine of the two words at angles and writes a three-word vector to vector as
// cos(first)*sin(second)>>12, -sin(first), cos(first)*cos(second)>>12.
// FUNCTION: FF7 0x00799969
void __cdecl Submarine_BuildDirectionVectorFromAngles(SubmarineDirectionAngles *angles, SubmarineDirectionVector *vector)
{
    short cosA;
    short sinFirst;
    short subCosSecond;
    short sinSecond;

    sinFirst = SinFixed(angles->angleFirst);
    cosA = CosFixed(angles->angleFirst);
    sinSecond = SinFixed(angles->angleSecond);
    subCosSecond = CosFixed(angles->angleSecond);
    vector->x = cosA * sinSecond >> 12;
    vector->y = -sinFirst;
    vector->z = cosA * subCosSecond >> 12;
}

// Offsets caller XYZ values shifted left by 6 by the current submarine coordinates, shifts them
// right by 12 and maps X/Z into a clamped 64 by 64 grid. Returns bit 4 when the selected cell is
// flagged (0x2000) but not timed (0x8000) and the relative Y lies strictly between its
// raisedHeight minus 64 and raisedHeight. Returns bit 1 when its height is below Y. Also computes
// the current submarine cell and reads g_SubmarineState.currentCellIndex into unused locals. Signed arithmetic
// and indices rely on the fixed grid convention.
// FUNCTION: FF7 0x007999f7
int __cdecl Submarine_ProbeGridCellCollision(int probeX, int probeY, int probeZ)
{
    int flags;
    int curRow;
    int cellIdx;
    int storedCell;
    int currentIndex;
    int currentCol;
    int dH;

    probeX <<= 6;
    probeY <<= 6;
    probeZ <<= 6;
    probeX += g_SubmarineState.player.posX;
    probeY += g_SubmarineState.player.posY;
    probeZ += g_SubmarineState.player.posZ;
    probeX >>= 12;
    probeY >>= 12;
    probeZ >>= 12;
    probeX = ((probeX + 2048) / 64);
    if (probeX < 0) {
        probeX = 0;
    }
    if (probeX >= 64) {
        probeX = 63;
    }
    probeZ = ((2048 - probeZ) / 64);
    if (probeZ < 0) {
        probeZ = 0;
    }
    if (probeZ >= 64) {
        probeZ = 63;
    }
    cellIdx = (probeZ << 6) + probeX;
    storedCell = g_SubmarineState.currentCellIndex;
    currentCol = (((g_SubmarineState.player.posX >> 12) + 2048) / 64);
    if (currentCol < 0) {
        currentCol = 0;
    }
    if (currentCol >= 64) {
        currentCol = 63;
    }
    curRow = ((2048 - (g_SubmarineState.player.posZ >> 12)) / 64);
    if (curRow < 0) {
        curRow = 0;
    }
    if (curRow >= 64) {
        curRow = 63;
    }
    currentIndex = (curRow << 6) + currentCol;
    flags = 0;
    if ((g_SubmarineGridCellTable[cellIdx].flags & SUBMARINE_CELL_FLAGGED) != 0 && (g_SubmarineGridCellTable[cellIdx].flags & SUBMARINE_CELL_TIMED) == 0) {
        dH = probeY - g_SubmarineGridCellTable[cellIdx].raisedHeight;
        if (dH > -64 && dH < 0) {
            flags |= SUBMARINE_MOTION_HIT_MINE;
        }
    }
    if (g_SubmarineGridCellTable[cellIdx].height < probeY) {
        flags |= SUBMARINE_MOTION_HIT_TERRAIN;
    }
    return flags;
}

// Copies input position into fixed-point globals with successive shifts by 6 and 12, copies
// orientation and descriptor indices, and loads parameters from the selected 56-byte table record.
// Computes a 64-column grid cell from X and Z, stores indexed auxiliary pointers and initializes
// several interpolation endpoints and deltas. Captures g_SubmarineState.speedScale from the
// existing 19-entry table before scaling all nineteen table values by the second record's posX
// divided by 16.
// FUNCTION: FF7 0x00799bc6
void __cdecl Submarine_LoadStateFromRecord(SubmarineStateRecord *stateRecord)
{
    SubmarineTableEntry *pTableRow;
    int value;
    int gridRow;
    int gridX;
    int factor;
    int workTemp;
    int i;

    g_SubmarineState.player.posX = stateRecord->posX << 6 << 12;
    g_SubmarineState.player.posY = stateRecord->posY << 6 << 12;
    g_SubmarineState.player.posZ = stateRecord->posZ << 6 << 12;
    g_SubmarineState.player.playerResource = stateRecord->playerResource;
    g_SubmarineState.player.angle = stateRecord->angle;
    g_SubmarineState.camRotX = g_SubmarineState.player.angle;
    g_SubmarineState.player.yaw = stateRecord->yaw;
    g_SubmarineState.camRotY = g_SubmarineState.player.yaw;
    g_SubmarineState.player.roll = stateRecord->roll;
    g_SubmarineState.camRotZ = g_SubmarineState.player.roll;
    g_SubmarineState.aimYaw = stateRecord->yaw;
    g_SubmarineState.tableIndex = stateRecord[1].posY;
    g_SubmarineState.auxFlags = 0;
    pTableRow = &g_SubmarineState.tableEntries[stateRecord[1].posY];
    g_SubmarineState.rotationStatePointer = pTableRow->rotation;
    g_SubmarineState.currentAngleA = pTableRow->currentAngleA;
    g_SubmarineState.currentAngleB = pTableRow->currentAngleB;
    g_SubmarineState.camOffset = pTableRow->camOffset;
    g_SubmarineState.controlState = pTableRow->controlState;
    g_SubmarineState.geomScreen = pTableRow->geomScreen;
    g_SubmarineTorpedoLaunchOffsetFactor = pTableRow->torpedoLaunchOffsetFactor;
    g_SubmarineTorpedoForwardOffsetFactor = pTableRow->torpedoForwardOffsetFactor;
    g_SubmarineState.currentBandBase = pTableRow->bandBase;
    g_SubmarineState.currentBandStep = pTableRow->bandStep;
    g_SubmarineState.currentBandOffset = pTableRow->bandOffset;
    gridX = g_SubmarineState.player.posX >> 12;
    gridRow = g_SubmarineState.player.posZ >> 12;
    gridX = ((gridX + 2048) / 64);
    gridRow = ((2048 - gridRow) / 64);
    g_SubmarineState.currentCellIndex = (gridRow << 6) + gridX;
    g_SubmarineState.speedScale = g_SubmarineState.speedScaleTable[g_SubmarineState.speedScaleIndex];
    g_SubmarineState.modelDescriptor = (int)&g_SubmarineModelDescriptors[stateRecord[1].posZ];
    value = g_SubmarineState.rotationStatePointer - 0x200;
    g_SubmarineRecordInterpBaseA = value;
    workTemp = g_SubmarineState.rotationStatePointer;
    g_SubmarineRecordInterpTargetA = workTemp;
    g_SubmarineRecordInterpDeltaA = 0x200;
    value = g_SubmarineState.currentAngleA;
    g_SubmarineInterpStartAngleAHigh = value;
    workTemp = g_SubmarineState.currentAngleA;
    g_SubmarineInterpTargetAngleAHigh = workTemp;
    g_SubmarineInterpDeltaAngleAHigh = 0x1000;
    value = g_SubmarineState.currentAngleB;
    g_SubmarineRecordInterpBaseB = value;
    workTemp = g_SubmarineState.currentAngleB;
    g_SubmarineRecordInterpTargetB = workTemp;
    g_SubmarineRecordInterpDeltaB = 0;
    value = 0x800;
    g_SubmarineRecordInterpBaseC = (short)value;
    workTemp = g_SubmarineState.camOffset;
    g_SubmarineRecordInterpTargetC = (short)workTemp;
    g_SubmarineRecordInterpDeltaC = (short)(workTemp - value);
    value = 0x800;
    g_SubmarineRecordInterpBaseD = (short)value;
    workTemp = g_SubmarineState.geomScreen;
    g_SubmarineRecordInterpTargetD = (short)workTemp;
    g_SubmarineRecordInterpDeltaD = (short)(workTemp - value);
    g_SubmarineRecordInterpFraction = 0;
    g_SubmarineRecordInterpStep = 32;
    g_SubmarineRecordInterpActive = 1;
    factor = stateRecord[1].posX;
    for (i = 0; i < 19; i++) {
        g_SubmarineState.speedScaleTable[i] = ((factor * (g_SubmarineState.speedScaleTable[i])) / 16);
    }
}

// Builds three fixed-point rotation basis vectors from rotationAngles->angle0, angle2 and angle1,
// masking each to 12 bits before sine/cosine lookup. Forms an X-axis short
// matrix at scale 4096 and transforms the three Z-axis basis vectors through
// TransformVectorToInt32, then forms a Y-axis matrix and transforms those intermediates again.
// Stores only the low 16-bit XYZ results into the three matrix rows, leaving their padding
// untouched. The packed union preserves contiguous matrix/vector helper storage; no pointer or
// output-size checks occur.
// FUNCTION: FF7 0x00799ef1
void __cdecl Submarine_BuildRotationBasisFromAngles(SubmarineRotationAngles *rotationAngles, SubmarineRecordMatrixRow *matrix)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int cosAngle1;
        struct { unsigned char pad_to_4[0x4]; int sinAngle1; };
        struct { unsigned char pad_to_8[0x8]; int cosAngle2; };
        struct { unsigned char pad_to_C[0xC]; int sinAngle2; };
        struct { unsigned char pad_to_10[0x10]; int cosAngle0; };
        struct { unsigned char pad_to_14[0x14]; int sinAngle0; };
        struct { unsigned char pad_to_18[0x18]; VECTOR basisAX; };
        struct { unsigned char pad_to_28[0x28]; VECTOR basisCX; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_58[0x58]; VECTOR basisBX; };
        struct { unsigned char pad_to_68[0x68]; VECTOR xform; };
    } frame;
#pragma pack(pop)

    frame.cosAngle0 = CosFixed(rotationAngles->angle0 + 0x1000 & 0xFFF);
    frame.sinAngle0 = SinFixed(rotationAngles->angle0 + 0x1000 & 0xFFF);
    frame.rotMatrix.m[0][0] = 0x1000 /* 360 deg */;
    frame.rotMatrix.m[0][1] = 0;
    frame.rotMatrix.m[0][2] = 0;
    frame.rotMatrix.m[1][0] = 0;
    frame.rotMatrix.m[1][1] = *(short *)&frame.cosAngle0;
    frame.rotMatrix.m[1][2] = -frame.sinAngle0;
    frame.rotMatrix.m[2][0] = 0;
    frame.rotMatrix.m[2][1] = *(short *)&frame.sinAngle0;
    frame.rotMatrix.m[2][2] = *(short *)&frame.cosAngle0;
    frame.cosAngle2 = CosFixed(rotationAngles->angle2 + 0x1000 & 0xFFF);
    frame.sinAngle2 = SinFixed(rotationAngles->angle2 + 0x1000 & 0xFFF);
    frame.basisAX.vx = *(short *)&frame.cosAngle2;
    frame.basisAX.vy = *(short *)&frame.sinAngle2;
    frame.basisAX.vz = 0;
    frame.basisBX.vx = (short)-frame.sinAngle2;
    frame.basisBX.vy = *(short *)&frame.cosAngle2;
    frame.basisBX.vz = 0;
    frame.basisCX.vx = 0;
    frame.basisCX.vy = 0;
    frame.basisCX.vz = 4096;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisAX, &frame.xform.vx);
    frame.basisAX.vx = frame.xform.vx;
    frame.basisAX.vy = frame.xform.vy;
    frame.basisAX.vz = frame.xform.vz;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisBX, &frame.xform.vx);
    frame.basisBX.vx = frame.xform.vx;
    frame.basisBX.vy = frame.xform.vy;
    frame.basisBX.vz = frame.xform.vz;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisCX, &frame.xform.vx);
    frame.basisCX.vx = frame.xform.vx;
    frame.basisCX.vy = frame.xform.vy;
    frame.basisCX.vz = frame.xform.vz;
    frame.cosAngle1 = CosFixed(rotationAngles->angle1 + 0x1000 & 0xFFF);
    frame.sinAngle1 = SinFixed(rotationAngles->angle1 + 0x1000 & 0xFFF);
    frame.rotMatrix.m[0][0] = *(short *)&frame.cosAngle1;
    frame.rotMatrix.m[0][1] = 0;
    frame.rotMatrix.m[0][2] = *(short *)&frame.sinAngle1;
    frame.rotMatrix.m[1][0] = 0;
    frame.rotMatrix.m[1][1] = 0x1000 /* 360 deg */;
    frame.rotMatrix.m[1][2] = 0;
    frame.rotMatrix.m[2][0] = -frame.sinAngle1;
    frame.rotMatrix.m[2][1] = 0;
    frame.rotMatrix.m[2][2] = *(short *)&frame.cosAngle1;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisAX, &frame.xform.vx);
    matrix->elemX = *(short *)&frame.xform.vx;
    matrix->elemY = *(short *)&frame.xform.vy;
    matrix->elemZ = *(short *)&frame.xform.vz;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisBX, &frame.xform.vx);
    matrix[1].elemX = *(short *)&frame.xform.vx;
    matrix[1].elemY = *(short *)&frame.xform.vy;
    matrix[1].elemZ = *(short *)&frame.xform.vz;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisCX, &frame.xform.vx);
    matrix[2].elemX = *(short *)&frame.xform.vx;
    matrix[2].elemY = *(short *)&frame.xform.vy;
    matrix[2].elemZ = *(short *)&frame.xform.vz;
}

// Combines the installed rotation and screen matrices and draws vertical slices between the
// selected grid height and submarine Y. The first slice uses height modulo 64, possibly zero;
// subsequent slices are at most 64 units. Transforms three points and requires all W values
// positive, shifts the third projected X by twice its W, divides XYZ by each W and acquires a two-
// node batch. Writes a white quad with the middle point duplicated and retains original W in the
// fourth vertex field rather than replacing it with reciprocal W. Other vertex fields and explicit
// batch advancement are untouched.
// FUNCTION: FF7 0x0079a17e
void __cdecl Submarine_DrawVerticalSliceQuads(void)
{
    typedef struct {
        float x, y, z, w;
    } SliceVertex;
    typedef struct {
        float x, y, z;
    } SlicePoint;
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        char raw[0x104];
        struct { int baseZ; };
        struct { unsigned char pad_to_4[0x4]; int baseX; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *vertex3; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *vertex2; };
        struct { unsigned char pad_to_10[0x10]; GfxTlVertex *vertex1; };
        struct { unsigned char pad_to_14[0x14]; GfxTlVertex *vertex0; };
        struct { unsigned char pad_to_18[0x18]; float invW; };
        struct { unsigned char pad_to_1C[0x1C]; int vertexIndex; };
        struct { unsigned char pad_to_20[0x20]; SlicePoint points[3]; };
        struct { unsigned char pad_to_44[0x44]; float screenMatrix[16]; };
        struct { unsigned char pad_to_84[0x84]; int sliceStep; };
        struct { unsigned char pad_to_88[0x88]; int sliceTop; };
        struct { unsigned char pad_to_8C[0x8C]; int subY; };
        struct { unsigned char pad_to_90[0x90]; float rotMatrix[16]; };
        struct { unsigned char pad_to_D0[0xD0]; SliceVertex projected[3]; };
        struct { unsigned char pad_to_100[0x100]; int color; };
    } frame;
#pragma pack(pop)

    frame.color = -1;
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), frame.rotMatrix);
    ComposeWithScreenMatrix(frame.rotMatrix, frame.screenMatrix);
    frame.baseX = g_SubmarineState.player.posX >> 12;
    frame.points[2].x = (float)frame.baseX;
    *(int *)&frame.points[1].x = *(int *)&frame.points[2].x;
    *(int *)&frame.points[0].x = *(int *)&frame.points[1].x;
    frame.baseZ = g_SubmarineState.player.posZ >> 12;
    frame.points[2].z = (float)frame.baseZ;
    *(int *)&frame.points[1].z = *(int *)&frame.points[2].z;
    *(int *)&frame.points[0].z = *(int *)&frame.points[1].z;
    frame.sliceTop = g_SubmarineGridCellTable[g_SubmarineState.currentCellIndex].currentHeight;
    frame.subY = g_SubmarineState.player.posY >> 12;
    frame.sliceStep = frame.sliceTop % 64;
    if (frame.sliceTop - frame.sliceStep < frame.subY)
        frame.sliceStep = frame.sliceTop - frame.subY;
    while (frame.sliceTop > frame.subY) {
        frame.points[0].y = (float)frame.sliceTop;
        frame.sliceTop -= frame.sliceStep;
        frame.sliceStep = frame.sliceTop - frame.subY;
        if (frame.sliceStep > 64)
            frame.sliceStep = 64;
        if (frame.sliceTop <= frame.subY)
            frame.sliceTop = frame.subY;
        frame.points[2].y = (float)frame.sliceTop;
        *(int *)&frame.points[1].y = *(int *)&frame.points[2].y;
        TransformPointByMatrix4x4(frame.screenMatrix, &frame.points[0].x, &frame.projected[0].x);
        if (frame.projected[0].w > 0.0f) {
            TransformPointByMatrix4x4(frame.screenMatrix, &frame.points[1].x, &frame.projected[1].x);
            if (frame.projected[1].w > 0.0f) {
                TransformPointByMatrix4x4(frame.screenMatrix, &frame.points[2].x, &frame.projected[2].x);
                if (frame.projected[2].w > 0.0f) {
                    frame.projected[2].x = 2.0f * *(volatile float *)&frame.projected[2].w + frame.projected[2].x;
                    for (frame.vertexIndex = 0; frame.vertexIndex < 3; frame.vertexIndex++) {
                        frame.invW = 1.0f / frame.projected[frame.vertexIndex].w;
                        frame.projected[frame.vertexIndex].x = frame.projected[frame.vertexIndex].x * *(volatile float *)&frame.invW;
                        frame.projected[frame.vertexIndex].y = frame.projected[frame.vertexIndex].y * *(volatile float *)&frame.invW;
                        frame.projected[frame.vertexIndex].z = frame.projected[frame.vertexIndex].z * *(volatile float *)&frame.invW;
                    }
                    if (AcquireBatchNodeNoCommandArea(2, g_SubmarineLineBatchList)) {
                        frame.vertex0 = (GfxTlVertex *)g_SubmarineLineBatchList->vertexCursor;
                        *(int *)&frame.vertex0->x = *(int *)&frame.projected[0].x;
                        *(int *)&frame.vertex0->y = *(int *)&frame.projected[0].y;
                        frame.vertex0->zBits = *(int *)&frame.projected[0].z;
                        frame.vertex0->rhwBits = *(int *)&frame.projected[0].w;
                        frame.vertex0->color = frame.color;
                        frame.vertex1 = (GfxTlVertex *)(g_SubmarineLineBatchList->vertexCursor + 0x20);
                        *(int *)&frame.vertex1->x = *(int *)&frame.projected[1].x;
                        *(int *)&frame.vertex1->y = *(int *)&frame.projected[1].y;
                        frame.vertex1->zBits = *(int *)&frame.projected[1].z;
                        frame.vertex1->rhwBits = *(int *)&frame.projected[1].w;
                        frame.vertex1->color = frame.color;
                        frame.vertex2 = (GfxTlVertex *)(g_SubmarineLineBatchList->vertexCursor + 0x40);
                        *(int *)&frame.vertex2->x = *(int *)&frame.projected[1].x;
                        *(int *)&frame.vertex2->y = *(int *)&frame.projected[1].y;
                        frame.vertex2->zBits = *(int *)&frame.projected[1].z;
                        frame.vertex2->rhwBits = *(int *)&frame.projected[1].w;
                        frame.vertex2->color = frame.color;
                        frame.vertex3 = (GfxTlVertex *)(g_SubmarineLineBatchList->vertexCursor + 0x60);
                        *(int *)&frame.vertex3->x = *(int *)&frame.projected[2].x;
                        *(int *)&frame.vertex3->y = *(int *)&frame.projected[2].y;
                        frame.vertex3->zBits = *(int *)&frame.projected[2].z;
                        frame.vertex3->rhwBits = *(int *)&frame.projected[2].w;
                        frame.vertex3->color = frame.color;
                    }
                }
            }
        }
    }
}
} /* extern "C" */

// src/ff7/condor/condor_run_cursor_camera.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the Fort Condor per-frame interaction loop: updates record layout and selection
//   state, handles frame input, moves the cursor with held-direction acceleration and edge
//   scrolling, and scrolls the battlefield camera.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "ff7/types/condor.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/condor.h"
#include "lib/api/graphics.h"
#include "ff7/enums/condor.h"
#include "lib/enums/file.h"
extern "C" {

#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)


#pragma intrinsic(memset)

#pragma intrinsic(abs)


/*
 * Executes record layout update loop for Fort Condor units.
 */
// FUNCTION: FF7 0x005fd3a0
void __cdecl Mini_CondorRunRecordLayoutLoop(void)
{
    CondorRunRecordSource *sourceUnit;
    short tileX;
    GfxEngineContext *pCtx;
    short yPosition;
    short topEdge;
    short remainingX;
    CondorRunRecordLayout *pLayoutPtr;
    CondorLinkRecord *listCursor;
    short matchIdx;
    short cursorY;
    short pointX;
    short halfWidth;

    pLayoutPtr = (CondorRunRecordLayout *)&g_CondorRunRecordLayouts;
    listCursor = g_CondorMatchListState.entries;
    g_CondorRecordLayoutCount = g_CondorMatchCount;
    g_CondorLayoutCounter = 0;
    g_CondorLayoutStart = g_CondorLayoutCounter;
    g_CondorLayoutIndex = g_CondorLayoutStart;
    if (g_CondorRecordLayoutCount > 7) {
        halfWidth = 96;
        g_CondorLayoutPanelHeight = 256;
        g_CondorRecordPanelHeight = (g_CondorRecordLayoutCount / 7 + 1 << 5) + 16;
    } else {
        halfWidth = (g_CondorRecordLayoutCount / 2) << 5;
        g_CondorLayoutPanelHeight = (g_CondorRecordLayoutCount << 5) + 32;
        if ((g_CondorRecordLayoutCount & 1) == 0) {
            halfWidth -= 16;
            g_CondorLayoutPanelHeight -= 16;
        }
        g_CondorRecordPanelHeight = 48;
    }
    if (g_CondorRecordMode != CONDOR_RECORD_MODE_TARGET) {
        pointX = g_CondorCursorPos.x;
        cursorY = g_CondorCursorPos.y;
    } else {
        pointX = g_CondorCursor.x;
        cursorY = g_CondorCursor.y;
    }
    if (pointX - g_CondorCameraX - halfWidth <= 64) {
        pointX = g_CondorCameraX + halfWidth + 64;
    } else if (pointX + g_CondorLayoutPanelHeight - g_CondorCameraX - halfWidth >= 272) {
        pointX = g_CondorCameraX + 320 - g_CondorLayoutPanelHeight + halfWidth + -8;
    }
    tileX = pointX - 256 - halfWidth;
    remainingX = tileX;
    yPosition = cursorY - 496;
    topEdge = yPosition;
    g_CondorPanelRectX = pointX - halfWidth - 16;
    g_CondorPanelRectY = cursorY - 16;
    for (matchIdx = 0; matchIdx < g_CondorRecordLayoutCount; matchIdx++) {
        sourceUnit = (CondorRunRecordSource *)listCursor->peer;
        if (((int)sourceUnit - (int)&g_CondorRecordTable.records[0].status) / 0x78 == g_CondorSelectedRecord) {
            g_CondorLayoutIndex = matchIdx;
        }
        pLayoutPtr->curX = sourceUnit->curX;
        pLayoutPtr->prevX = pLayoutPtr->curX;
        pLayoutPtr->curY = sourceUnit->curY;
        pLayoutPtr->prevY = pLayoutPtr->curY;
        pLayoutPtr->sourceX = sourceUnit->layoutX;
        pLayoutPtr->prevZ = pLayoutPtr->sourceX;
        pLayoutPtr->linkedRecord = (int)listCursor->peer;
        pLayoutPtr->index = ((int)sourceUnit - (int)&g_CondorRecordTable.records[0].status) / 0x78;
        pLayoutPtr->pendingOffset = sourceUnit->pendingOffset;
        pLayoutPtr->savedAngle = sourceUnit->angle;
        Mini_CondorBeginLinearMotion(sourceUnit->targetX, sourceUnit->targetY, tileX + 0x100, yPosition + 0x200);
        pLayoutPtr->dirX = (short)g_CondorVecX;
        pLayoutPtr->dirY = (short)g_CondorVecY;
        pLayoutPtr->carryY = 0;
        pLayoutPtr->carryX = 0;
        pLayoutPtr->targetX = tileX;
        pLayoutPtr->targetY = yPosition;
        sourceUnit->layoutX = *(int *)&g_CondorScreenDistanceBuf - 0x50;
        sourceUnit->pendingOffset = 0;
        tileX += 32;
        if (matchIdx == 6 || matchIdx == 13) {
            tileX = remainingX;
            yPosition += 32;
        }
        pLayoutPtr++;
        listCursor++;
    }
    g_CondorMainState = CONDOR_STATE_RECORD_PANEL;
    g_CondorSelectPanelPhase = CONDOR_PANEL_OPEN;
    Mini_CondorRequestDisplayMode2();
    g_CondorSavedSelectedRecord = g_CondorSelectedRecord;
    pLayoutPtr = (CondorRunRecordLayout *)(g_CondorLayoutIndex * 0x2C + (char *)&g_CondorRunRecordLayouts);
    g_CondorSelectedRecord = (pLayoutPtr->linkedRecord - (int)&g_CondorRecordTable.records[0].status) / 0x78;
    Mini_CondorSetSelectedReadout(*(short *)&g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex, g_CondorSelectedRecord);
    Mini_CondorRefreshActiveRecordDisplay();
    while (g_CondorLayoutCounter == 0) {
        ReadTimestampCounter(&g_CondorFrameTimestamp);
        Mini_CondorUpdateFrameInput((int *)&g_CondorCameraMatrix, g_CondorMatchListState.cameraWorkMatrix);
        Mini_CondorCommitPendingCommand();
        Mini_CondorUpdateRecordSelectPanel();
        Mini_CondorUpdateUnitAnims();
        Mini_CondorDrawHudFrame();
        pCtx = (GfxEngineContext *)GetEngineContext();
        pCtx->frameCount += 1.0;
        pCtx->framesThisSecond += 1.0;
        GfxClearAllCall(pCtx);
        Mini_CondorTickAndPresentDirect(pCtx);
    }
}

/*
 * Begins selection mode for the active record.
 */
// FUNCTION: FF7 0x005fd832
short __cdecl Mini_CondorOpenSelectedRecordActionMenu(void)
{
    if (g_CondorPhase == 1) {
        return 1;
    } else {
        if (g_CondorSelectedRecord != -1) {
            Mini_CondorRefreshActiveRecordDisplay();
            g_CondorRecordMode = CONDOR_RECORD_MODE_ACTION_MENU;
            g_CondorActionCursor = 0;
            g_CondorSelectRowBaseX = 0xFFF8;
            g_CondorSelectRowX = g_CondorSelectRowBaseX;
            g_CondorSelectRowBase = 1;
            g_CondorSelectRowY = g_CondorSelectRowBase;
            Mini_CondorRequestDisplayMode2();
            Mini_CondorSetUnitMenuLayout();
        }
        return 0;
    }
}

/*
 * Clears current active record state.
 */
// FUNCTION: FF7 0x005fd8a5
void __cdecl Mini_CondorClearActiveRecord(void)
{
    g_CondorSelectedRecord = 0xFFFF;
    g_CondorCameraZoomInit = 0x100 /* 0.0625 */;
    g_CondorCameraOffsetInit = 0;
}

/*
 * Resets active record selection mode.
 */
// FUNCTION: FF7 0x005fd8c7
void __cdecl Mini_CondorResetRecordMode(void)
{
    g_CondorRecordMode = CONDOR_RECORD_MODE_CURSOR;
    Mini_CondorRequestDisplayMode1();
    g_CondorSelectedRecord = 0xFFFF;
    g_CondorMatchListState.cursorActive = 1;
}

/*
 * Saves the camera and enters the help screen (CONDOR_STATE_HELP_SCREEN), silencing three SFX channels.
 */
// FUNCTION: FF7 0x005fd8ed
void __cdecl Mini_CondorSaveCameraAndEnterState0E(void)
{
    Mini_CondorSubmitSfxPlay(1, 4);
    Mini_CondorSubmitSfxPlay(0, 1);
    Mini_CondorSubmitSfxPlay(0, 2);
    Mini_CondorSubmitSfxPlay(0, 3);
    g_CondorSavedCameraX = g_CondorCameraX;
    g_CondorSavedCameraY = g_CondorCameraY;
    g_CondorMainState = CONDOR_STATE_HELP_SCREEN;
    g_CondorCameraX = 0;
    g_CondorCameraY = 6;
}

/*
 * Updates frame input and dispatches interaction logic. Every caller passes the frame input block
 * as a second argument, which the body does not read.
 */
// FUNCTION: FF7 0x005fd958
int __cdecl Mini_CondorUpdateFrameInput(void *context, void *inputBlock)
{
    int res;
    int moveFlags;
    CondorRecord *record;
    short index;

    res = 0;
    Input_LatchButtonFlags();
    g_CondorInputMask = g_CondorInputLeft << 0xF | g_CondorInputDown << 0xE | g_CondorInputRight << 0xD | g_CondorInputUp << 0xC | g_CondorInputStart << 0xB | g_CondorInputSelect << 8 | g_CondorInputSquare << 7 | g_CondorInputCross << 6 | g_CondorInputCircle << 5 | g_CondorInputTriangle << 4 | g_CondorInputR1 << 3 | g_CondorInputR2 << 1 | g_CondorInputL1 << 2 | g_CondorInputL2;
    g_CondorInputPressed = g_CondorInputMask ^ g_CondorPrevInputMask;
    g_CondorInputPressed &= g_CondorInputMask;
    g_CondorPrevInputMask = g_CondorInputMask;
    if ((g_CondorInputMask & (INPUT_START | INPUT_SELECT | INPUT_L2 | INPUT_R2 | INPUT_L1 | INPUT_R1)) == (INPUT_START | INPUT_SELECT | INPUT_L2 | INPUT_R2 | INPUT_L1 | INPUT_R1)) {
        res = 1;
    }
    if (g_CondorStage != CONDOR_STAGE_RUN) {
        res = g_CondorStage;
    }
    g_CondorFrameInput = 0;
    if ((g_CondorInputMask & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT | INPUT_L2 | INPUT_R2 | INPUT_L1 | INPUT_R1)) != 0) {
        if ((g_CondorInputPressed & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT | INPUT_L2 | INPUT_R2 | INPUT_L1 | INPUT_R1)) != 0) {
            g_CondorCycleTimer = 10;
            g_CondorFrameInput = g_CondorInputPressed;
        }
        g_CondorCycleTimer--;
        if (g_CondorCycleTimer < 0) {
            g_CondorCycleTimer = 3;
            g_CondorFrameInput = g_CondorInputMask & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT | INPUT_L2 | INPUT_R2 | INPUT_L1 | INPUT_R1);
        }
    } else {
        g_CondorCycleTimer = 10;
    }
    if (g_CondorMainState != CONDOR_STATE_IDLE) {
        Mini_CondorDispatchStateHandler();
    } else if (g_CondorActiveActionRecord != 0) {
        if ((g_CondorInputPressed & (INPUT_CIRCLE | INPUT_SQUARE)) != 0) {
            if ((g_CondorInputPressed & INPUT_CIRCLE) != 0) {
                g_CondorDeselectAfterAction = 0;
            } else {
                g_CondorDeselectAfterAction = 1;
            }
            record = &g_CondorRecordTable.records[g_CondorActionRecordSlot];
            Mini_CondorBeginLinearMotion(g_CondorCursorPos.x, g_CondorCursorPos.y, record->posX, record->posY);
            g_CondorMotionStepX = g_CondorVecX;
            g_CondorMotionStepY = g_CondorVecY;
            g_CondorMainState = CONDOR_STATE_CURSOR_GLIDE_TO_ACTION;
            Mini_CondorArmRampLimit();
        }
        if ((g_CondorInputPressed & (INPUT_TRIANGLE | INPUT_CROSS)) != 0) {
            g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
            g_CondorActiveActionRecord = 0;
            Mini_CondorRequestDisplayMode1();
            Mini_CondorSetSelectedReadout(*(short *)&g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex, g_CondorSelectedRecord);
        }
    } else {
        if ((g_CondorInputPressed & INPUT_CIRCLE) != 0) {
            switch (g_CondorRecordMode) {
            case CONDOR_RECORD_MODE_TARGET:
                if (*(unsigned short *)&g_CondorCursorProbeFlag != 0) {
                    g_CondorCursorProbeFlag = 0;
                    Mini_CondorSubmitSfxPlay(1, 4);
                    g_CondorLayoutStart = 0;
                    Mini_CondorDispatchRecordCommand();
                    if (g_CondorLayoutStart != 2) {
                        Mini_CondorClearActiveRecord();
                        g_CondorSelectPanelPhase = CONDOR_PANEL_OPEN;
                        g_CondorWobbleTimer = 30;
                        g_CondorSpriteSlotAHeightX.screenX = 0xFFF8;
                        g_CondorSpriteSlotAScreenY = 0xFFFFFFE8 - ((int)*(unsigned short *)&g_CondorWobbleTimer / 4) % 3;
                        g_CondorMainState = CONDOR_STATE_CURSOR_GLIDE;
                        g_CondorRecordMode = CONDOR_RECORD_MODE_CURSOR;
                        Mini_CondorBeginLinearMotion(g_CondorCursor.x, g_CondorCursor.y, g_CondorCursorPos.x, g_CondorCursorPos.y);
                        g_CondorMotionStepX = g_CondorVecX;
                        g_CondorMotionStepY = g_CondorVecY;
                        Mini_CondorArmRampLimit();
                    }
                } else {
                    Mini_CondorSubmitSfxPlay(3, 4);
                }
                break;
            case CONDOR_RECORD_MODE_ACTION_MENU:
                Mini_CondorSubmitSfxPlay(1, 4);
                Mini_CondorSubmitSfxPlay(0, 1);
                Mini_CondorSubmitSfxPlay(0, 2);
                Mini_CondorSubmitSfxPlay(0, 3);
                Mini_CondorDispatchRecordAction();
                break;
            case CONDOR_RECORD_MODE_CURSOR:
                if (g_CondorSelectedRecord != -1) {
                    Mini_CondorSubmitSfxPlay(1, 4);
                    index = Mini_CondorPickRecordNearPoint(&g_CondorCursorPos);
                    if (g_CondorMatchCount != 0) {
                        if (g_CondorMatchCount == 1) {
                            if (g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex < 16) {
                                Mini_CondorOpenSelectedRecordActionMenu();
                            }
                        } else {
                            Mini_CondorRunRecordLayoutLoop();
                        }
                    }
                } else {
                    g_CondorRedFadeLevel = 192;
                    if (*(unsigned short *)&g_CondorCursorProbeFlag != 0) {
                        g_CondorChoiceTimer++;
                    } else {
                        Mini_CondorSubmitSfxPlay(3, 4);
                    }
                }
            }
        }
        if ((g_CondorInputPressed & INPUT_CROSS) != 0) {
            if (g_CondorPhase == 1) {
                Mini_CondorSubmitSfxPlay(1, 4);
                g_CondorMainState = CONDOR_STATE_CONFIRM_ROWS;
                g_CondorSelectPanelPhase = CONDOR_PANEL_OPEN;
                g_CondorSelectedRecord = 0xFFFF;
                Mini_CondorRequestDisplayMode2();
                g_CondorSelectRowY = 16;
                g_CondorSpriteSlot6HeightX.screenX = g_CondorCameraX + 48;
                g_CondorSpriteSlot6ScreenY = g_CondorCameraY + g_CondorSelectRowY + 76;
                g_CondorBannerIndex = 13;
                Mini_CondorSetSelectedReadout(-1, 0);
            } else {
                switch (g_CondorRecordMode) {
                case CONDOR_RECORD_MODE_ACTION_MENU:
                    Mini_CondorSubmitSfxPlay(4, 4);
                    g_CondorRecordMode = CONDOR_RECORD_MODE_CURSOR;
                    Mini_CondorRequestDisplayMode1();
                    break;
                case CONDOR_RECORD_MODE_TARGET:
                    Mini_CondorSubmitSfxPlay(4, 4);
                    g_CondorMainState = CONDOR_STATE_CURSOR_GLIDE_TO_ACTION;
                    Mini_CondorBeginLinearMotion(g_CondorCursor.x, g_CondorCursor.y, g_CondorCursorPos.x, g_CondorCursorPos.y);
                    g_CondorMotionStepX = g_CondorVecX;
                    g_CondorMotionStepY = g_CondorVecY;
                    Mini_CondorArmRampLimit();
                }
            }
        }
        if (g_CondorRecordMode <= CONDOR_RECORD_MODE_CURSOR) {
            g_CondorCursorProbeFlag = 0;
            if ((g_CondorInputMask & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)) != 0) {
                Mini_CondorAccelerateHeldStep();
                if ((g_CondorInputMask & INPUT_SQUARE) != 0) {
                    Mini_CondorAccelerateHeldStep();
                }
                Mini_CondorProbeGatedCursorPosPlacement();
                Mini_CondorClearActiveRecord();
                g_CondorMatchListState.cursorActive = 1;
            } else {
                g_CondorHeldStepCount = 0;
                if (g_CondorMatchListState.cursorActive == 1) {
                    g_CondorMatchListState.cursorActive = 0;
                    g_CondorSelectedRecord = Mini_CondorPickRecordNearPoint(&g_CondorCursorPos);
                    Mini_CondorRefreshActiveRecordDisplay();
                    g_CondorIndexRange = g_CondorCursorPos.x;
                    (*(short *)((char *)&g_CondorIndexRange + 2)) = g_CondorCursorPos.y;
                    if (g_CondorSelectedRecord != -1 && g_CondorRecordTable.records[g_CondorSelectedRecord].status == 0) {
                        g_CondorSelectedRecord = 0xFFFF;
                    }
                    if (g_CondorSelectedRecord != -1) {
                        if (g_CondorSelectedRecord >= 20) {
                            g_CondorHudBlinkPhase = 1;
                        } else {
                            g_CondorHudBlinkPhase = 0;
                        }
                        moveFlags = 0;
                        index = g_CondorRecordTable.records[g_CondorSelectedRecord].posX;
                        if (index < g_CondorIndexRange) {
                            moveFlags |= INPUT_LEFT;
                        }
                        if (index > g_CondorIndexRange) {
                            moveFlags |= INPUT_RIGHT;
                        }
                        index = g_CondorRecordTable.records[g_CondorSelectedRecord].posY;
                        if (index < (*(short *)((char *)&g_CondorIndexRange + 2))) {
                            moveFlags |= INPUT_UP;
                        }
                        if (index > (*(short *)((char *)&g_CondorIndexRange + 2))) {
                            moveFlags |= INPUT_DOWN;
                        }
                        g_CondorIndexRange = g_CondorCursorPos.x;
                        (*(short *)((char *)&g_CondorIndexRange + 2)) = g_CondorCursorPos.y;
                        if (moveFlags != 0) {
                            Mini_CondorMoveActiveCursor(moveFlags);
                        }
                        g_CondorCursorPos.x = g_CondorRecordTable.records[g_CondorSelectedRecord].posX;
                        g_CondorCursorPos.y = g_CondorRecordTable.records[g_CondorSelectedRecord].posY;
                    }
                }
                Mini_CondorProbeGatedCursorPosPlacement();
                if (g_CondorSelectedRecord != -1) {
                    g_CondorIndexRange = g_CondorCursorPos.x;
                    (*(short *)((char *)&g_CondorIndexRange + 2)) = g_CondorCursorPos.y;
                    moveFlags = 0;
                    index = g_CondorRecordTable.records[g_CondorSelectedRecord].posX;
                    if (index < g_CondorIndexRange) {
                        moveFlags |= INPUT_LEFT;
                    }
                    if (index > g_CondorIndexRange) {
                        moveFlags |= INPUT_RIGHT;
                    }
                    index = g_CondorRecordTable.records[g_CondorSelectedRecord].posY;
                    if (index < (*(short *)((char *)&g_CondorIndexRange + 2))) {
                        moveFlags |= INPUT_UP;
                    }
                    if (index > (*(short *)((char *)&g_CondorIndexRange + 2))) {
                        moveFlags |= INPUT_DOWN;
                    }
                    g_CondorIndexRange = g_CondorCursorPos.x;
                    (*(short *)((char *)&g_CondorIndexRange + 2)) = g_CondorCursorPos.y;
                    if (moveFlags != 0) {
                        Mini_CondorMoveActiveCursor(moveFlags);
                    }
                    g_CondorCursorPos.x = g_CondorRecordTable.records[g_CondorSelectedRecord].posX;
                    g_CondorCursorPos.y = g_CondorRecordTable.records[g_CondorSelectedRecord].posY;
                }
            }
        } else if (g_CondorRecordMode == CONDOR_RECORD_MODE_ACTION_MENU) {
            if ((g_CondorFrameInput & INPUT_UP) != 0) {
                Mini_CondorSubmitSfxPlay(1, 4);
                if (g_CondorActionCursor == 0) {
                    g_CondorActionCursor = g_CondorActionRowIndex;
                    g_CondorSelectRowY = g_CondorSelectRowBase + (g_CondorActionRowIndex << 4);
                }
                g_CondorActionCursor--;
                g_CondorSelectRowY -= 16;
            }
            if ((g_CondorFrameInput & INPUT_DOWN) != 0) {
                Mini_CondorSubmitSfxPlay(1, 4);
                if (g_CondorActionCursor == g_CondorActionRowIndex - 1) {
                    g_CondorActionCursor = 0xFFFF;
                    g_CondorSelectRowY = g_CondorSelectRowBase - 16;
                }
                g_CondorActionCursor++;
                g_CondorSelectRowY += 16;
            }
        } else {
            g_CondorCursorProbeFlag = 0;
            if ((g_CondorInputMask & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)) != 0) {
                Mini_CondorAccelerateHeldStep();
                if ((g_CondorInputMask & INPUT_SQUARE) != 0) {
                    Mini_CondorAccelerateHeldStep();
                }
                g_CondorCommandCode = 3;
            } else {
                g_CondorHeldStepCount = 0;
                g_CondorPickedRecordIndex = Mini_CondorPickRecordNearPoint(&g_CondorCursor);
                if (g_CondorPickedRecordIndex != -1) {
                    g_CondorCursor.x = g_CondorRecordTable.records[g_CondorPickedRecordIndex].posX;
                    g_CondorCursor.y = g_CondorRecordTable.records[g_CondorPickedRecordIndex].posY;
                    if (g_CondorSelectedRecord == g_CondorPickedRecordIndex) {
                        g_CondorCommandCode = 4;
                    } else if (g_CondorRecordTable.records[g_CondorPickedRecordIndex].unitIndex <= 15) {
                        if (g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex == 12) {
                            g_CondorCommandCode = 2;
                        } else {
                            g_CondorCommandCode = 3;
                        }
                    } else {
                        g_CondorCommandCode = 0;
                    }
                } else {
                    g_CondorCommandCode = 3;
                }
            }
            if (g_CondorMainState == CONDOR_STATE_IDLE) {
                Mini_CondorProbeCursorPlacement();
            }
        }
        if ((g_CondorInputPressed & INPUT_START) != 0 && g_CondorMainState == CONDOR_STATE_IDLE && g_CondorPhase != 1) {
            Mini_CondorSubmitSfxPlay(1, 4);
            g_CondorMainState = CONDOR_STATE_START_MENU;
            g_CondorSavedDisplayMode = g_CondorDisplayMode;
            if (g_CondorActionSelectState != 0) {
                g_CondorActionSelectState = 0;
            } else {
                g_CondorActionSelectState = 1;
            }
            Mini_CondorRequestDisplayMode3();
            g_CondorPrevOutcomeIndex = g_CondorOutcomeIndex;
            Mini_CondorBuildDigitDisplayRecords();
            Mini_CondorSubmitSfxPlay(0, 1);
            Mini_CondorSubmitSfxPlay(0, 2);
            Mini_CondorSubmitSfxPlay(0, 3);
            g_CondorCursorScreenX = g_CondorCursorPos.x - g_CondorCameraX;
            g_CondorCursorScreenY = g_CondorCursorPos.y - g_CondorCameraY;
            g_CondorCursorPos.x = g_CondorCameraX + 160;
            g_CondorCursorPos.y = g_CondorCameraY + 104;
        }
        if ((g_CondorInputPressed & INPUT_SELECT) != 0 && g_CondorRecordMode <= CONDOR_RECORD_MODE_CURSOR && g_CondorMainState == CONDOR_STATE_IDLE && g_CondorPhase != 1) {
            Mini_CondorSaveCameraAndEnterState0E();
        }
        if ((g_CondorInputPressed & INPUT_L2) != 0) {
            Mini_CondorSubmitSfxPlay(1, 4);
            if (g_CondorTickStep > 1) {
                g_CondorTickStep--;
            }
        }
        if ((g_CondorInputPressed & INPUT_R2) != 0) {
            Mini_CondorSubmitSfxPlay(1, 4);
            if (g_CondorTickStep < 4) {
                g_CondorTickStep++;
            }
        }
        if ((g_CondorInputPressed & INPUT_L1) != 0) {
            Mini_CondorSubmitSfxPlay(1, 4);
            if (g_CondorTickStep > 1) {
                g_CondorTickStep--;
            }
        }
        if ((g_CondorInputPressed & INPUT_R1) != 0) {
            Mini_CondorSubmitSfxPlay(1, 4);
            if (g_CondorTickStep < 4) {
                g_CondorTickStep++;
            }
        }
    }
    SetRotMatrix(context);
    SetTransMatrix(context);
    return res;
}

/*
 * Tests cursor collision against board points.
 */
// FUNCTION: FF7 0x005fe63c
short __cdecl Mini_CondorProbeGatedCursorPosPlacement(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        CondorCursor placeX;
        struct { unsigned char pad_to_4[0x4]; short placeZ; };
        struct { unsigned char pad_to_8[0x8]; short hitResult; };
    } frame;
#pragma pack(pop)

    if (g_CondorMainState != CONDOR_STATE_IDLE) {
        return 1;
    } else if (g_CondorActiveActionRecord != 0) {
        return 1;
    } else if (g_CondorSelectedRecord != -1) {
        return 1;
    } else if (g_CondorPlayerUnitCount >= 20) {
        return 1;
    } else if (g_CondorPhase == 1) {
        if (g_CondorCursorPos.y >= 672) {
            return 1;
        }
    } else if (g_CondorCursorPos.y >= (*(int *)&g_CondorHighestUnitY)) {
        return 1;
    }
    frame.placeX.x = g_CondorCursorPos.x;
    frame.placeX.y = 1024 - g_CondorCursorPos.y;
    frame.placeZ = -40;
    frame.hitResult = Mini_CondorTestRecordPlacementWithPeers(0, &frame.placeX);
    if (frame.hitResult == 1) {
        return 1;
    } else {
        g_CondorCursorProbeFlag = 1;
        return 0;
    }
}

/*
 * Tests saved position collision against board points.
 */
// FUNCTION: FF7 0x005fe718
short __cdecl Mini_CondorProbeCursorPlacement(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        CondorCursor placeX;
        struct { unsigned char pad_to_4[0x4]; short placeZ; };
        struct { unsigned char pad_to_8[0x8]; short hitResult; };
    } frame;
#pragma pack(pop)

    frame.placeX.x = g_CondorCursor.x;
    frame.placeX.y = 1024 - g_CondorCursor.y;
    frame.placeZ = -40;
    frame.hitResult = Mini_CondorTestRecordPlacement(0, &frame.placeX);
    if (frame.hitResult == 1) {
        return 1;
    } else {
        g_CondorCursorProbeFlag = 1;
        return 0;
    }
}

/*
 * Accelerates movement speed while direction is held.
 */
// FUNCTION: FF7 0x005fe771
short __cdecl Mini_CondorAccelerateHeldStep(void)
{
    g_CondorHeldStepCount++;
    if (*(unsigned short *)&g_CondorHeldStepCount > 16) {
        g_CondorHeldStepCount = 16;
    }
    Mini_CondorMoveActiveCursor(g_CondorInputMask);
    if (*(unsigned short *)&g_CondorHeldStepCount <= 2) {
        return 0;
    }
    Mini_CondorMoveActiveCursor(g_CondorInputMask);
    if (*(unsigned short *)&g_CondorHeldStepCount <= 3) {
        return 0;
    }
    Mini_CondorMoveActiveCursor(g_CondorInputMask);
    Mini_CondorMoveActiveCursor(g_CondorInputMask);
    return 0;
}

/*
 * Sets up unit menu layout parameters.
 */
// FUNCTION: FF7 0x005fe800
void __cdecl Mini_CondorSetUnitMenuLayout(void)
{
    switch (g_CondorRecordTable.records[g_CondorSelectedRecord].kind) {
    case CONDOR_BEHAVIOR_STATIONARY:
        g_CondorActionRowIndex = 1;
        g_CondorActionRowA = 2;
        break;
    case CONDOR_BEHAVIOR_FIGHTER:
        g_CondorActionRowIndex = 1;
        g_CondorActionRowA = 3;
        break;
    case CONDOR_BEHAVIOR_SUPPORT:
        g_CondorActionRowIndex = 1;
        g_CondorActionRowA = 3;
        break;
    case CONDOR_BEHAVIOR_TURRET_A:
    case CONDOR_BEHAVIOR_TURRET_B:
        g_CondorActionRowIndex = 2;
        g_CondorActionRowA = 5;
        g_CondorActionRowB = 2;
        break;
    case CONDOR_BEHAVIOR_CATAPULT:
        g_CondorActionRowIndex = 1;
        g_CondorActionRowA = 3;
        break;
    case CONDOR_BEHAVIOR_ONESHOT:
        g_CondorActionRowIndex = 2;
        g_CondorActionRowA = 3;
        g_CondorActionRowB = 0;
        break;
    case CONDOR_BEHAVIOR_AUTONOMOUS:
        g_CondorActionRowIndex = 0;
        break;
    default:
        g_CondorActionRowIndex = 0;
    }
}

/*
 * Moves cursor position based on directional input.
 */
// FUNCTION: FF7 0x005fe8cf
void __cdecl Mini_CondorMoveActiveCursor(int moveFlags)
{
    CondorCursor *cursor;

    if (g_CondorActiveActionRecord == 0) {
        cursor = &g_CondorCursor;
    } else {
        cursor = &g_CondorCursorPos;
    }
    if (g_CondorRecordMode != CONDOR_RECORD_MODE_TARGET && g_CondorMainState == CONDOR_STATE_IDLE) {
        cursor = &g_CondorCursorPos;
    }
    Mini_CondorMoveCursorAndScroll(moveFlags, cursor);
}

/*
 * Moves cursor and scrolls view when reaching edges.
 */
// FUNCTION: FF7 0x005fe91b
void __cdecl Mini_CondorMoveCursorAndScroll(int moveFlags, CondorCursor *cursor)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        short relX;
        struct { unsigned char pad_to_2[0x2]; short relY; };
        struct { unsigned char pad_to_8[0x8]; int scrollDelta; };
        struct { unsigned char pad_to_C[0xC]; int edgeCount; };
        struct { unsigned char pad_to_10[0x10]; short scrollY; };
        struct { unsigned char pad_to_14[0x14]; short scrollX; };
    } frame;
#pragma pack(pop)

    if (g_CondorCursorMode == 0) {
        frame.relX = cursor->x - g_CondorCameraX;
        frame.relY = cursor->y - g_CondorCameraY;
        if ((moveFlags & INPUT_UP) != 0) {
            frame.edgeCount = 0;
            if (frame.relY > 104) {
                frame.edgeCount++;
            } else {
                g_CondorCameraY--;
                if (g_CondorCameraY > 32768) {
                    g_CondorCameraY = 0;
                    frame.edgeCount++;
                } else {
                    g_CondorCameraTranslateY++;
                }
            }
            if (frame.edgeCount != 0) {
                frame.relY--;
                if (frame.relY < 0) {
                    frame.relY = 0;
                }
            }
        }
        if ((moveFlags & INPUT_DOWN) != 0) {
            frame.edgeCount = 0;
            if (frame.relY < 104) {
                frame.edgeCount++;
            } else {
                g_CondorCameraY++;
                if (g_CondorCameraY > 804) {
                    g_CondorCameraY = 804;
                    frame.edgeCount++;
                } else {
                    g_CondorCameraTranslateY--;
                }
            }
            if (frame.edgeCount != 0) {
                frame.relY++;
                if (frame.relY >= 204) {
                    frame.relY = 204;
                }
            }
        }
        if ((moveFlags & INPUT_LEFT) != 0) {
            frame.edgeCount = 0;
            if (frame.relX > 160) {
                frame.edgeCount++;
            } else {
                g_CondorCameraX--;
                if (g_CondorCameraX > 32768) {
                    g_CondorCameraX = 0;
                    g_CondorCameraTranslateX &= 0xFFFFFFF8;
                    frame.edgeCount++;
                } else {
                    g_CondorCameraTranslateX++;
                }
            }
            if (frame.edgeCount != 0) {
                frame.relX--;
                if (frame.relX < 0) {
                    frame.relX = 0;
                }
            }
        }
        if ((moveFlags & INPUT_RIGHT) != 0) {
            frame.edgeCount = 0;
            if (frame.relX < 160) {
                frame.edgeCount++;
            } else {
                g_CondorCameraX++;
                if (g_CondorCameraX > 192) {
                    g_CondorCameraX = 192;
                    g_CondorCameraTranslateX &= 0xFFFFFFF8;
                    frame.edgeCount++;
                } else {
                    g_CondorCameraTranslateX--;
                }
            }
            if (frame.edgeCount != 0) {
                frame.relX++;
                if (frame.relX >= 320) {
                    frame.relX = 320;
                }
            }
        }
        cursor->x = frame.relX + g_CondorCameraX;
        cursor->y = frame.relY + g_CondorCameraY;
    } else {
        frame.relX = cursor->x - g_CondorCameraX;
        frame.relY = cursor->y - g_CondorCameraY;
        frame.scrollX = g_CondorCameraX;
        frame.scrollY = g_CondorCameraY;
        if ((moveFlags & INPUT_UP) != 0) {
            frame.edgeCount = 0;
            frame.scrollDelta = g_CondorCursorSpeedY;
            if (frame.relY > 104) {
                frame.edgeCount++;
            } else {
                frame.scrollY -= (short)g_CondorCursorSpeedY;
                if (frame.scrollY < 0) {
                    frame.scrollDelta = 0 - frame.scrollY;
                    g_CondorCameraTranslateY += frame.scrollDelta;
                    frame.scrollY = 0;
                    frame.edgeCount++;
                } else {
                    g_CondorCameraTranslateY += g_CondorCursorSpeedY;
                }
            }
            if (frame.edgeCount != 0) {
                frame.relY -= (short)frame.scrollDelta;
                if (frame.relY < 0) {
                    frame.relY = 0;
                }
            }
            g_CondorCameraY = frame.scrollY;
        }
        if ((moveFlags & INPUT_DOWN) != 0) {
            frame.edgeCount = 0;
            frame.scrollDelta = g_CondorCursorSpeedY;
            if (frame.relY < 104) {
                frame.edgeCount++;
            } else {
                g_CondorCameraY += (short)g_CondorCursorSpeedY;
                if (g_CondorCameraY > 804) {
                    frame.scrollDelta = g_CondorCameraY - 0x324;
                    g_CondorCameraTranslateY += frame.scrollDelta;
                    g_CondorCameraY = 804;
                    frame.edgeCount++;
                } else {
                    g_CondorCameraTranslateY -= g_CondorCursorSpeedY;
                }
            }
            if (frame.edgeCount != 0) {
                frame.relY += (short)frame.scrollDelta;
                if (frame.relY >= 204) {
                    frame.relY = 204;
                }
            }
        }
        if ((moveFlags & INPUT_LEFT) != 0) {
            frame.edgeCount = 0;
            frame.scrollDelta = g_CondorCursorSpeedX;
            if (frame.relX > 160) {
                frame.edgeCount++;
            } else {
                frame.scrollX -= (short)g_CondorCursorSpeedX;
                if (frame.scrollX < 0) {
                    frame.scrollDelta = 0 - frame.scrollX;
                    frame.scrollX = 0;
                    g_CondorCameraTranslateX &= 0xFFFFFFF8;
                    g_CondorCameraTranslateX += frame.scrollDelta;
                    frame.edgeCount++;
                } else {
                    g_CondorCameraTranslateX += g_CondorCursorSpeedX;
                }
            }
            if (frame.edgeCount != 0) {
                frame.relX -= (short)frame.scrollDelta;
                if (frame.relX < 0) {
                    frame.relX = 0;
                }
            }
            g_CondorCameraX = frame.scrollX;
        }
        if ((moveFlags & INPUT_RIGHT) != 0) {
            frame.edgeCount = 0;
            frame.scrollDelta = g_CondorCursorSpeedX;
            if (frame.relX < 160) {
                frame.edgeCount++;
            } else {
                g_CondorCameraX += (short)g_CondorCursorSpeedX;
                if (g_CondorCameraX > 192) {
                    frame.scrollDelta = g_CondorCameraX - 0xC0;
                    g_CondorCameraTranslateY += frame.scrollDelta;
                    g_CondorCameraX = 192;
                    g_CondorCameraTranslateX &= 0xFFFFFFF8;
                    frame.edgeCount++;
                } else {
                    g_CondorCameraTranslateX -= g_CondorCursorSpeedX;
                }
            }
            if (frame.edgeCount != 0) {
                frame.relX += (short)frame.scrollDelta;
                if (frame.relX >= 320) {
                    frame.relX = 320;
                }
            }
        }
        cursor->x = frame.relX + g_CondorCameraX;
        cursor->y = frame.relY + g_CondorCameraY;
    }
}

/*
 * Updates camera scroll offset across battlefield.
 */
// FUNCTION: FF7 0x005fef60
void __cdecl Mini_CondorScrollCamera(int scrollFlags)
{
    short cameraX;
    short cameraY;
    int step;
    int edgeHits;

    if (g_CondorCursorMode == 0) {
        if ((scrollFlags & INPUT_UP) != 0) {
            g_CondorCameraY--;
            if (g_CondorCameraY > 32768) {
                g_CondorCameraY = 0;
                edgeHits++;
            } else {
                g_CondorCameraTranslateY++;
            }
        }
        if ((scrollFlags & INPUT_DOWN) != 0) {
            g_CondorCameraY++;
            if (g_CondorCameraY > 804) {
                g_CondorCameraY = 804;
                edgeHits++;
            } else {
                g_CondorCameraTranslateY--;
            }
        }
        if ((scrollFlags & INPUT_LEFT) != 0) {
            g_CondorCameraX--;
            if (g_CondorCameraX > 32768) {
                g_CondorCameraX = 0;
                g_CondorCameraTranslateX &= 0xFFFFFFF8;
                edgeHits++;
            } else {
                g_CondorCameraTranslateX++;
            }
        }
        if ((scrollFlags & INPUT_RIGHT) != 0) {
            g_CondorCameraX++;
            if (g_CondorCameraX > 192) {
                g_CondorCameraX = 192;
                g_CondorCameraTranslateX &= 0xFFFFFFF8;
                edgeHits++;
            } else {
                g_CondorCameraTranslateX--;
            }
        }
    } else {
        cameraY = g_CondorCameraY;
        cameraX = g_CondorCameraX;
        if ((scrollFlags & INPUT_UP) != 0) {
            step = g_CondorCursorSpeedY;
            cameraY -= (short)g_CondorCursorSpeedY;
            if (cameraY < 0) {
                step = 0 - cameraY;
                cameraY = 0;
                g_CondorCameraTranslateY += step;
                edgeHits++;
            } else {
                g_CondorCameraTranslateY += g_CondorCursorSpeedY;
            }
            g_CondorCameraY = cameraY;
        }
        if ((scrollFlags & INPUT_DOWN) != 0) {
            g_CondorCameraY += (short)g_CondorCursorSpeedY;
            if (g_CondorCameraY > 804) {
                step = g_CondorCameraY - 804;
                g_CondorCameraY = 804;
                g_CondorCameraTranslateY -= step;
                edgeHits++;
            } else {
                g_CondorCameraTranslateY -= g_CondorCursorSpeedY;
            }
        }
        if ((scrollFlags & INPUT_LEFT) != 0) {
            cameraX -= (short)g_CondorCursorSpeedX;
            if (cameraX < 0) {
                step = 0 - cameraX;
                cameraX = 0;
                g_CondorCameraTranslateX &= 0xFFFFFFF8;
                g_CondorCameraTranslateX += step;
                edgeHits++;
            } else {
                g_CondorCameraTranslateX += g_CondorCursorSpeedX;
            }
            g_CondorCameraX = cameraX;
        }
        if ((scrollFlags & INPUT_RIGHT) != 0) {
            g_CondorCameraX += (short)g_CondorCursorSpeedX;
            if (g_CondorCameraX > 192) {
                g_CondorCameraX = 192;
                step = g_CondorCameraX - 192;
                g_CondorCameraTranslateX &= 0xFFFFFFF8;
                g_CondorCameraTranslateX -= step;
                edgeHits++;
            } else {
                g_CondorCameraTranslateX -= g_CondorCursorSpeedX;
            }
        }
    }
}
} /* extern "C" */

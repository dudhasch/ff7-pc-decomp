// src/ff7/condor/condor_game_state_hud.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Holds the Fort Condor minigame state: loads and stores the battle outcome in the
//   savemap, resets and runs each simulation frame with victory and defeat handling, and draws the
//   battlefield HUD, banners, digits and sprites.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include "ff7/types/condor.h"
#include "lib/codegen.h"
#include <string.h>
#include "ff7/api/condor.h"
#include "lib/types/graphics.h"
#include "ff7/enums/condor.h"
#include "ff7/savemap.h"
extern "C" {

#pragma intrinsic(strcpy, strcat)
/* 60 contiguous 0x28-byte CondorSpriteEntry records live at 0x00C72E88..0x00C737E8; slots 0..4 have
 * no interior GLOBAL markers, while later slots carry per-field GLOBALs for direct field writes. */

/* Eight contiguous 0x48-byte CondorDigitGroup records span 0x00CBEED0..0x00CBF110; per-field GLOBALs
 * stay in place for direct addressing, while the 5 pinned buffer heads carry #define aliases. */
#define g_CondorDigitGroupTable ((CondorDigitGroup *)g_CondorGilDigitSprites)
// clang-format on

/*
 * Clears cached score/fund digit display groupings and reset counters.
 */
// FUNCTION: FF7 0x005f76a0
void __cdecl Mini_CondorClearDigitGroups(void)
{
    unsigned short groupIndex;
    unsigned short digitIndex;

    g_CondorSelectedUnitIndex = 0;
    g_CondorTensDigitSlot = 0;
    g_CondorOnesDigitSlot = 0;
    g_CondorSelectedReadoutVisible = 0;
    for (groupIndex = 0; groupIndex < 8; groupIndex++) {
        for (digitIndex = 0; digitIndex < 15; digitIndex++) {
            g_CondorDigitGroupTable[groupIndex].digitSprites[digitIndex] = 0;
        }
        g_CondorDigitGroupTable[groupIndex].isVisible = 0;
    }
}
/*
 * Empty per-frame hook callback.
 */
// FUNCTION: FF7 0x005f774e
void __cdecl Mini_CondorPerFrameHookNoOp(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
}
/*
 * Starts a Fort Condor battle from the savemap; called last by `Mini_CondorEnter` with the engine
 * context and the model load options. Sets the step divisor to 10 and the mode word g_CondorPhase
 * to 1. Reads the battle index from the savemap's outcome byte (0..6; anything else becomes 0), which
 * selects the battle's entry in data.bin, unitChoiceTier into g_CondorUnitChoiceTier, and the gil
 * into g_CondorFunds. Points the config entry at g_CondorConfigEntryBuffer and clears the
 * exit word g_CondorStage. Loads data.bin, camera.bin, id.bin, vert.bin, the help TIM and the
 * texture render objects (screen centre 160, 120), clears the digit displays, resets the game
 * state and plays mapped music track 0x14.
 */
// FUNCTION: FF7 0x005f7756
void __cdecl Mini_CondorLoadSavemapState(GfxEngineContext *dst, struct PfileLoadOptions *src)
{
    struct Savemap *saveState;
    unsigned char outcome;
    int *fundsPointer;

    g_CondorStepDivisor = 10;
    g_CondorPhase = 1;
    saveState = &gSavemap;
    outcome = saveState->condorOutcome;
    g_CondorOutcomeIndex = outcome & 0xFF;
    if (g_CondorOutcomeIndex >= 7) {
        g_CondorOutcomeIndex = 0;
    }
    g_CondorUnitChoiceTier = saveState->condorUnitChoiceTier;
    fundsPointer = &saveState->gil;
    g_CondorFunds = *fundsPointer;
    g_pCondorConfigEntry = (struct CondorConfigEntry *)&g_CondorConfigEntryBuffer;
    g_CondorStage = CONDOR_STAGE_RUN;
    Mini_CondorLoadAllResources(dst, src, 0xA0, 0x78);
    Mini_CondorClearDigitGroups();
    Mini_CondorResetGameState();
    Mini_CondorPlayMappedMusic(0x14);
}
/*
 * Writes minigame outcome, remaining funds, and victory flags back to the global savemap.
 */
// FUNCTION: FF7 0x005f7818
void __cdecl Mini_CondorStoreSavemapState(void)
{
    struct Savemap *pState;
    int *fundsAddr;

    pState = &gSavemap;
    pState->condorPlayerUnitCount = *(char *)&g_CondorPlayerUnitCount;
    pState->condorSpawnCount = *(char *)&g_CondorSpawnCount;
    pState->condorSpawnsLeft = (*(unsigned char *)&g_CondorScheduledCount) - g_CondorSpawnCount;
    if (g_CondorStage == CONDOR_STAGE_EXIT_FLAGGED) {
        pState->condorFlag = 1;
    } else {
        pState->condorFlag = 0;
    }
    fundsAddr = &pState->gil;
    *fundsAddr = g_CondorFunds;
}
/*
 * Runs the phase `Mini_CondorUpdateFrameInput` returned for this frame; called by
 * `Mini_CondorFrame` and again by `Mini_CondorTick` for a catch-up frame.
 * - RUN: one battle step (`Mini_CondorRunBattleFrame`).
 * - RESTART: stops the music, resets the game state and plays mapped track 0 when the mode
 *   word g_CondorPhase is 1, else track 0x14.
 * - SWITCH_MODE: stops the music, toggles g_CondorPhase between 0 and 1, resets the game
 *   state, plays track 0x14 for mode 1 and 0 otherwise, and queues request 0xC.
 * - RESTART_RECORDS: stops the music, builds the active record list, plays track 0, resets the
 *   game state (which sets g_CondorPhase to 2 at this exit word) and plays track 0 a second time.
 * - anything else (the exit words EXIT_FLAGGED and EXIT): stops the music.
 */
// FUNCTION: FF7 0x005f7893
void __cdecl Mini_CondorDispatchPhase(short phase)
{
    switch (phase) {
    case CONDOR_STAGE_RUN:
        Mini_CondorRunBattleFrame();
        break;
    case CONDOR_STAGE_RESTART:
        Mini_CondorStopMusic();
        Mini_CondorResetGameState();
        if (g_CondorPhase == 1) {
            Mini_CondorPlayMappedMusic(0);
        } else {
            Mini_CondorPlayMappedMusic(0x14);
        }
        break;
    case CONDOR_STAGE_SWITCH_MODE:
        Mini_CondorStopMusic();
        if (g_CondorPhase == 1) {
            g_CondorPhase = 0;
        } else {
            g_CondorPhase = 1;
        }
        Mini_CondorResetGameState();
        if (g_CondorPhase == 1) {
            Mini_CondorPlayMappedMusic(0x14);
        } else {
            Mini_CondorPlayMappedMusic(0);
        }
        Mini_CondorSetPendingRequest(0xC);
        break;
    case CONDOR_STAGE_RESTART_RECORDS:
        Mini_CondorStopMusic();
        Mini_CondorBuildActiveRecordList();
        Mini_CondorPlayMappedMusic(0);
        Mini_CondorResetGameState();
        Mini_CondorPlayMappedMusic(0);
        break;
    default:
        Mini_CondorStopMusic();
    }
}
/*
 * Resets minigame variables, unit counts, funds, timers, and battlefield state.
 */
// FUNCTION: FF7 0x005f7979
void __cdecl Mini_CondorResetGameState(void)
{
    short i;

    if (g_CondorStage == CONDOR_STAGE_RESTART_RECORDS) {
        g_CondorPhase = 2;
    }
    g_CondorNextSpawnTime = 0;
    g_CondorInitialSpawnTime = g_CondorNextSpawnTime;
    g_CondorCommittedCommandCount = 0;
    g_CondorBlinkTick = 0;
    g_CondorFrameCounter = g_CondorBlinkTick;
    g_CondorBattleFrame = g_CondorFrameCounter;
    g_CondorScheduleClock = 0;
    g_CondorStage = CONDOR_STAGE_RUN;
    g_CondorBattleOutcome = 0;
    g_CondorHudResetWord = 0;
    g_CondorHudBlinkCounter = 0;
    g_CondorWobbleTimer = 0;
    g_CondorTickStep = 2;
    g_CondorUnitDeathActive = 0;
    g_CondorChoiceTimer = g_CondorUnitDeathActive;
    g_CondorSequenceCounter = g_CondorChoiceTimer;
    g_CondorDisplayModeRequest = 0;
    g_CondorDisplayModePending = g_CondorDisplayModeRequest;
    g_CondorRequestedDisplayMode = g_CondorDisplayModePending;
    g_CondorDisplayMode = CONDOR_DISPLAY_NONE;
    *(short *)&g_CondorSelectedAttackRangeBuf = 0;
    g_CondorPendingCommandCount = 0;
    g_CondorPendingRequest = 0xFFFF;
    g_CondorHeldStepCount = 0;
    *(int *)&g_CondorScheduledCount = 0;
    g_CondorSpawnCount = *(int *)&g_CondorScheduledCount;
    g_CondorPlayerUnitCount = g_CondorSpawnCount;
    g_CondorNumberSpriteBaseField = 0;
    g_CondorRedFadeLevel = 0;
    *(int *)&g_CondorAlertLevel = g_CondorRedFadeLevel;
    (g_CondorRotationAngles.rotX) = 0;
    (g_CondorRotationAngles.rotY) = 0;
    (g_CondorRotationAngles.rotZ) = 0;
    (g_CondorUnitRotationReset.rotX) = 0;
    (g_CondorUnitRotationReset.rotY) = 0;
    (g_CondorUnitRotationReset.rotZ) = 0;
    g_CondorCameraTranslateX = 0;
    g_CondorCameraTranslateY = 0;
    g_CondorCameraTranslateZ = 0;
    g_CondorHudInitWordA = 0;
    g_CondorHudInitWordB = 0;
    g_CondorHudInitScale = 0x100 /* 0.0625 */;
    g_CondorSelectAngleState.initWord = 0x100;
    (g_CondorSelectAngleState.selectAngle) = 0;
    g_CondorHudResetShort = 0;
    g_CondorStepSpanX = *(int *)&g_CondorScreenDistanceBuf / *(unsigned short *)&g_CondorStepDivisor;
    g_CondorStepSpanY = *(int *)&g_CondorScreenDistanceBuf / *(unsigned short *)&g_CondorStepDivisor;
    g_CondorStepSpanZ = *(int *)&g_CondorScreenDistanceBuf / *(unsigned short *)&g_CondorStepDivisor;
    g_CondorCycleTimer = 10;
    g_CondorRecordMode = CONDOR_RECORD_MODE_CURSOR;
    g_CondorPhaseFrames = 0;
    g_CondorActiveActionRecord = g_CondorPhaseFrames;
    g_CondorBannerCounter = 96;
    g_CondorMainState = CONDOR_STATE_INTRO;
    g_CondorPrevInputMask = 0;
    g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
    g_CondorDefaultPickedRecord = 0xFFFF;
    g_CondorPickedRecordIndex = g_CondorDefaultPickedRecord;
    g_CondorSelectedRecord = g_CondorPickedRecordIndex;
    g_CondorMatchListState.cursorActive = 0;
    g_CondorHudBackgroundMode = 0;
    g_CondorCameraZoomInit = 0x100 /* 0.0625 */;
    g_CondorCameraOffsetInit = 0;
    g_CondorBattleClearWordB = 0;
    g_CondorBattleClearWordA = 0;
    g_CondorBattleClearCopy = g_CondorBattleClearWordA;
    g_CondorCursorProbeFlag = 0;
    g_CondorRingBase = g_CondorCursorProbeFlag;
    g_CondorChoiceUnaffordable = 0;
    for (i = 0; i < 32; i++) {
        *((unsigned char *)&g_CondorUnitFrameIndex + i) = 1;
    }
    Mini_CondorResetRowTables();
    Mini_CondorLoadRecordsForMode();
    Mini_CondorInitHudSprites();
    Mini_CondorInitAuxiliaryRecords();
    if (g_CondorPhase == 1) {
        Mini_CondorSetPendingRequest(0xC);
    } else {
        Mini_CondorSetPendingRequest(1);
    }
}
/*
 * Freezes battlefield timer, zeroes elapsed time, and resets banner display flags.
 */
// FUNCTION: FF7 0x005f7cd5
void __cdecl Mini_CondorFreezeTimerAndResetBanner(void)
{
    Mini_CondorSubmitSfxPlay(0, 1);
    Mini_CondorSubmitSfxPlay(0, 2);
    Mini_CondorSubmitSfxPlay(0, 3);
    Mini_CondorSubmitSfxPlay(0, 4);
    g_CondorSelectedRecord = 0xFFFF;
    g_CondorSelectPanelPhase = CONDOR_PANEL_OPEN;
    g_CondorPhaseFrames = 0;
    g_CondorBannerCounter = 0xFFE0 /* -32 */;
    Mini_CondorRequestDisplayMode2();
}
/*
 * Sets victory outcome state and initiates celebration sequence.
 */
// FUNCTION: FF7 0x005f7d33
void __cdecl Mini_CondorBeginOutcome1(void)
{
    Mini_CondorFreezeTimerAndResetBanner();
    Mini_CondorSetPendingRequest(2);
    Mini_CondorPlayMappedMusic(0xA);
    g_CondorMainState = CONDOR_STATE_END_BANNER_EXIT;
}
/*
 * Sets defeat outcome state and transitions to defeat handling.
 */
// FUNCTION: FF7 0x005f7d5b
void __cdecl Mini_CondorBeginOutcome2(void)
{
    Mini_CondorFreezeTimerAndResetBanner();
    Mini_CondorSetPendingRequest(7);
    if (g_CondorOutcomeIndex == 6) {
        Mini_CondorPlayMappedMusic(0xB);
    } else {
        Mini_CondorPlayMappedMusic(0xC);
    }
    g_CondorMainState = CONDOR_STATE_END_BANNER_EXIT_FLAGGED;
}
/*
 * Commits unit placement or command action and deducts required funds.
 */
// FUNCTION: FF7 0x005f7d9b
void __cdecl Mini_CondorCommitPendingCommand(void)
{
    if (g_CondorPendingCommandCount != 0) {
        g_CondorPendingCommandCount = 0;
        g_CondorCommandsCommitted = 1;
    } else {
        g_CondorCommandsCommitted = 0;
    }
    if (g_CondorPendingRequest != -1) {
        g_CondorBannerIndex = g_CondorPendingRequest;
        g_CondorPendingRequest = 0xFFFF;
    }
    g_CondorCommittedCommandCount++;
    g_CondorBlinkTick++;
    Mini_CondorDispatchFadeRamp();
}
/*
 * One step of the Fort Condor battle (phase 0). First commits the pending command and banner
 * request and advances the blink counters. Unless the battle state g_CondorMainState is a cursor
 * glide or a record wheel/angle state (states that run their own logic):
 * - with the state IDLE and no panel open, and the record mode at most CURSOR, advances the clock
 *   (g_CondorFrameCounter wraps at 0xE10 = 3600 frames, one minute at 60 fps, and then
 *   g_CondorBattleFrame counts up), spawns the enemy units due now and offers a choice;
 * - unless the state is START_MENU or a unit action is running, moves and fights the units and
 *   ticks their timers, or in mode 1 (g_CondorPhase) only animates them.
 * With the state IDLE, no action running and mode not 1, it checks how the battle ends: outcome 1
 * when no enemy unit is on the board (g_CondorSpawnCount 0), outcome 2 when the player has no
 * units (g_CondorPlayerUnitCount 0) and less than 400 gil. Once set, outcome 1 starts
 * `Mini_CondorBeginOutcome1` (music 0xA, state END_BANNER_EXIT, which ends in CONDOR_STAGE_EXIT)
 * and outcome 2 `Mini_CondorBeginOutcome2` (state END_BANNER_EXIT_FLAGGED, which ends in
 * CONDOR_STAGE_EXIT_FLAGGED). Last, draws the HUD.
 */
// FUNCTION: FF7 0x005f7e10
void __cdecl Mini_CondorRunBattleFrame(void)
{
    Mini_CondorCommitPendingCommand();
    switch (g_CondorMainState) {
    case CONDOR_STATE_CURSOR_GLIDE_TO_ACTION:
    case CONDOR_STATE_CURSOR_GLIDE:
    case CONDOR_STATE_RECORD_SELECT_WHEEL:
    case CONDOR_STATE_RECORD_ANGLE_INPUT:
    case CONDOR_STATE_RECORD_ANGLE_COMMIT:
        break;
    default:
        if ((g_CondorMainState | g_CondorSelectPanelPhase) == 0) {
            if (g_CondorRecordMode <= CONDOR_RECORD_MODE_CURSOR) {
                g_CondorFrameCounter++;
                if (g_CondorFrameCounter >= 3600) {
                    g_CondorFrameCounter = 0;
                    g_CondorBattleFrame++;
                }
                Mini_CondorSpawnScheduledRecord();
                Mini_CondorTryBeginChoice();
            }
        }
        if (g_CondorMainState != CONDOR_STATE_START_MENU && g_CondorActiveActionRecord == 0) {
            if (g_CondorPhase != 1) {
                Mini_CondorUpdateUnits();
                Mini_CondorTickUnitTimers();
            } else {
                Mini_CondorUpdateUnitAnims();
            }
        }
    }
    if (g_CondorMainState == CONDOR_STATE_IDLE && g_CondorActiveActionRecord == 0 && g_CondorPhase != 1) {
        if (g_CondorSpawnCount == 0 && g_CondorBattleOutcome == 0) {
            g_CondorBattleOutcome = 1;
        }
        if (g_CondorPlayerUnitCount == 0 && g_CondorBattleOutcome == 0 && (unsigned int)g_CondorFunds < 400) {
            g_CondorBattleOutcome = 2;
        }
    }
    if (g_CondorMainState == CONDOR_STATE_IDLE && g_CondorActiveActionRecord == 0) {
        if (g_CondorBattleOutcome == 1) {
            Mini_CondorBeginOutcome1();
        }
        if (g_CondorBattleOutcome == 2) {
            Mini_CondorBeginOutcome2();
        }
    }
    Mini_CondorDrawHudFrame();
}
/*
 * Unused empty callback hook.
 */
// FUNCTION: FF7 0x005f7f98
void __cdecl Mini_CondorUnusedNoOp(void)
{
}
/*
 * Renders tactical command menu action rows and unit selection buttons.
 */
// FUNCTION: FF7 0x005f7f9d
int __cdecl Mini_CondorDrawActionRows(void)
{
    if (g_CondorActionRowIndex == 0) {
        Mini_CondorDrawSelectedRecordReadout(-40, 16);
        return 0;
    } else {
        Mini_CondorDrawSpriteSlot(6, g_CondorSelectRowX + 0x38, g_CondorSelectRowY + 0x10);
        Mini_CondorDrawSelectedRecordReadout(-40, 16);
        Mini_CondorSetSpriteSlotSize(11, 192, g_CondorActionRowA << 4);
        Mini_CondorDrawSpriteSlot(11, 64, 16);
        Mini_CondorSetSpriteSlotPalette(14, g_CondorHudBlinkPhase + 2);
        Mini_CondorDrawSpriteSlot(14, 56, 16);
        if (g_CondorActionRowIndex == 1) {
            return 0;
        } else {
            Mini_CondorSetSpriteSlotSize(12, 192, g_CondorActionRowB << 4);
            Mini_CondorDrawSpriteSlot(12, 64, 32);
            Mini_CondorSetSpriteSlotPalette(15, g_CondorHudBlinkPhase + 2);
            Mini_CondorDrawSpriteSlot(15, 56, 32);
            if (g_CondorActionRowIndex == 2) {
                return 0;
            } else {
                Mini_CondorSetSpriteSlotSize(13, 192, g_Condor_Byte << 4);
                Mini_CondorDrawSpriteSlot(13, 64, 48);
                Mini_CondorSetSpriteSlotPalette(16, g_CondorHudBlinkPhase + 2);
                Mini_CondorDrawSpriteSlot(16, 56, 48);
            }
        }
    }
    return 0;
}
/*
 * Draws stats, cost, and information panel for the currently selected unit.
 */
// FUNCTION: FF7 0x005f80f1
void __cdecl Mini_CondorDrawSelectedRecordReadout(short x, short y)
{
    unsigned short flags;

    flags = -(g_CondorRecordTable.records[g_CondorSelectedRecord].flags);
    flags--;
    Mini_CondorDrawThreeSpriteLabel(x + g_pCondorReadoutOrigin->offsetX, y + g_pCondorReadoutOrigin->offsetY + 2);
    Mini_CondorDrawDigitString(x + g_pCondorReadoutOrigin->offsetX + 0xA, y + g_pCondorReadoutOrigin->offsetY + 0x15, 2);
    Mini_CondorSetSpriteSlotPalette(22, flags & 1);
    // These four calls use the original narrow forwarding sequence; the other callers keep their own prototype.
    ((int(__cdecl *)(int, short, short))Mini_CondorDrawSpriteSlot)(0x16, (short)x, y + 0x20);
    flags = flags >> 1;
    Mini_CondorSetSpriteSlotPalette(23, flags & 1);
    Mini_CondorDrawSpriteSlot(23, x + 0x10, y + 0x20);
    flags = flags >> 1;
    Mini_CondorSetSpriteSlotPalette(24, flags & 1);
    Mini_CondorDrawSpriteSlot(24, x + 0x20, y + 0x20);
    Mini_CondorSetSpriteSlotPalette(4, g_CondorHudBlinkPhase + 2);
    /* Retail call-site prototype (int, short, short) -> int preserved for /Od caller codegen. */
    ((int(__cdecl *)(int, short, short))Mini_CondorDrawSpriteSlot)(4, (short)x, (short)y);
}
/*
 * Draws the surrounding frame and background chrome for the battlefield HUD.
 */
// FUNCTION: FF7 0x005f824a
void __cdecl Mini_CondorDrawHudFrame(void)
{
    Mini_CondorSnapshotDisplayOrigin();
    switch (g_CondorHudBackgroundMode) {
    case CONDOR_DISPLAY_NONE:
    case CONDOR_DISPLAY_FULL:
        Mini_CondorDrawDisplayBackground();
        break;
    case CONDOR_DISPLAY_DARK:
        Mini_CondorDrawDisplayBackground();
        break;
    }
    Mini_CondorUpdateRedFadeSprite();
    Mini_CondorResetAuxiliaryRecords();
    if (g_CondorSubViewIndex == 0) {
        g_CondorHudCursorYLimit = 248;
    } else {
        g_CondorHudCursorYLimit = 8;
    }
    g_CondorHudCursor.x = g_CondorCursorPos.x;
    g_CondorHudCursor.y = g_CondorCursorPos.y;
    if (g_CondorHudCursor.x - g_CondorCameraX < 40) {
        g_CondorHudCursor.x = g_CondorCameraX + 40;
    }
    if (g_CondorHudCursor.x - g_CondorCameraX > 200) {
        g_CondorHudCursor.x = g_CondorCameraX + 200;
    }
    if (g_CondorHudCursor.y - g_CondorCameraY >= 160) {
        g_CondorHudCursor.y -= 112;
    }
    g_CondorHudCursor.x = g_CondorCameraX + 198;
    g_CondorHudCursor.y = g_CondorCameraY - 14;
    if (g_CondorHudBlinkCounter >= 16) {
        g_CondorHudBlinkCounter = -1;
    }
    g_CondorHudBlinkCounter += 1;
    if (*(unsigned short *)&g_CondorWobbleTimer != 0) {
        *(unsigned short *)&g_CondorWobbleTimer -= 1;
        Mini_CondorDrawSpriteSlot(10, 0xFFFF, 0xFFE8);
    }
    switch (g_CondorMainState) {
    case CONDOR_STATE_END_BANNER_EXIT_FLAGGED:
        if (g_CondorBannerCounter == 64) {
            Mini_CondorDrawBlinkingSpritePair(0x4F);
        }
        Mini_CondorDrawBannerBar(g_CondorBannerCounter);
        break;
    case CONDOR_STATE_END_BANNER_EXIT:
        Mini_CondorDrawBannerBar(g_CondorBannerCounter);
        break;
    case CONDOR_STATE_INTRO:
        if (g_CondorPhase != 1) {
            Mini_CondorDrawBannerBar(g_CondorBannerCounter);
        }
        break;
    }
    if (g_CondorMainState == CONDOR_STATE_START_MENU) {
        if ((g_CondorBlinkTick & 0x20) != 0) {
            g_CondorSpriteSlot2CHeightX.screenX = g_CondorCameraX + 136;
            g_CondorSpriteSlot2CScreenY = g_CondorCameraY + 104;
            Mini_CondorDrawSpriteSlot(44, 0xFFFF, 0);
        }
        Mini_CondorDrawHud();
        Mini_CondorStepCounterEffects();
    } else {
        Mini_CondorSetSpriteSlotSize(1, 32, 48);
        switch (g_CondorRecordMode) {
        case CONDOR_RECORD_MODE_NONE:
            break;
        case CONDOR_RECORD_MODE_CURSOR:
            if (g_CondorBattleOutcome == 0 && g_CondorMainState != CONDOR_STATE_CONFIRM_ROWS && g_CondorMainState != CONDOR_STATE_HELP_SCREEN) {
                if (g_CondorSelectedRecord != -1) {
                    if (g_CondorRecordTable.records[g_CondorSelectedRecord].status == 0 ||
                        g_CondorRecordTable.records[g_CondorSelectedRecord].retired != 0) {
                        g_CondorSelectedRecord = 0xFFFF;
                    } else if (g_CondorActiveActionRecord == 0) {
                        if (g_CondorSelectedRecord >= 20) {
                            g_CondorHudBlinkPhase = 1;
                        } else {
                            g_CondorHudBlinkPhase = 0;
                        }
                        Mini_CondorDrawSelectedRecordReadout(-40, 16);
                        Mini_CondorRefreshSelectionAuxRecords();
                    }
                } else if (g_CondorMainState == CONDOR_STATE_UNIT_ATTACK || g_CondorMainState == CONDOR_STATE_UNIT_COUNTERATTACK) {
                    Mini_CondorRefreshSelectionAuxRecords();
                } else {
                    if (*(unsigned short *)&g_CondorCursorProbeFlag == 0) {
                        g_pCondorSpriteSlot5PosSource = &g_CondorCursorPos;
                        Mini_CondorDrawSpriteSlot(5, 0xFFFC, 0xFFE0);
                    }
                    Mini_CondorDrawSpriteSlot(1, 0xFFFC, 0xFFE0);
                }
            }
            break;
        case CONDOR_RECORD_MODE_ACTION_MENU:
            if (g_CondorBattleOutcome == 0) {
                if (g_CondorSelectedRecord >= 20) {
                    g_CondorHudBlinkPhase = 1;
                } else {
                    g_CondorHudBlinkPhase = 0;
                }
                Mini_CondorRefreshSelectionAuxRecords();
                Mini_CondorDrawActionRows();
            }
            break;
        case CONDOR_RECORD_MODE_TARGET:
            if (g_CondorMainState != CONDOR_STATE_RECORD_PANEL) {
                if (*(unsigned short *)&g_CondorCursorProbeFlag == 0) {
                    g_pCondorSpriteSlot5PosSource = &g_CondorCursor;
                    Mini_CondorDrawSpriteSlot(5, 0xFFFC, 0xFFE0);
                }
                Mini_CondorSetSpriteSlotSize(9, 144, *(unsigned char *)&g_CondorCommandCode << 4);
                Mini_CondorDrawSpriteSlot(9, 14, 0xFFE0);
                Mini_CondorDrawSpriteSlot(46, 0xFFFC, 0xFFE0);
            } else if (g_CondorSelectedRecord != -1) {
                if (g_CondorSelectedRecord >= 20) {
                    g_CondorHudBlinkPhase = 1;
                } else {
                    g_CondorHudBlinkPhase = 0;
                }
                Mini_CondorDrawSelectedRecordReadout(-40, 16);
                Mini_CondorRefreshSelectionAuxRecords();
            }
            break;
        }
        Mini_CondorDrawHud();
        Mini_CondorStepCounterEffects();
    }
    g_CondorPeakValue = 0;
    Mini_CondorUpdateEffectSlots();
}
/*
 * Renders status announcement banner bar across the screen.
 */
// FUNCTION: FF7 0x005f86e4
void __cdecl Mini_CondorDrawBannerBar(short bannerId)
{
    int slot;

    g_CondorBannerTarget = bannerId;
    (g_CondorSpriteSlot17HeightX.posX) = g_CondorCameraX + 32;
    g_CondorSpriteSlot11ScreenY = g_CondorCameraY + g_CondorBannerCounter + -8;
    (g_CondorSpriteSlot18HeightX.posX) = g_CondorCameraX + 32;
    g_CondorSpriteSlot12ScreenY = g_CondorCameraY + g_CondorBannerCounter + 6;
    g_CondorSpriteSlot12OffsetX.offsetX = 0x100;
    g_CondorSpriteSlot11OffsetX.offsetX = g_CondorSpriteSlot12OffsetX.offsetX;
    (g_CondorSpriteSlot17HeightX.height) = 32;
    (g_CondorSpriteSlot18HeightX.height) = 16;
    if (g_CondorBannerIndex < 0) {
        return;
    }
    Mini_CondorSetSpriteSlotSize(17, 0, 136);
    slot = g_CondorBannerIndex + 0x4B;
    g_CondorSpriteEntries[slot].screenX = g_CondorCameraX + 32;
    g_CondorSpriteEntries[slot].screenY = g_CondorCameraY + g_CondorBannerCounter + 6;
    /* Retail call-site prototype (short, int, int) -> int preserved for /Od caller codegen. */
    ((int(__cdecl *)(short, int, int))Mini_CondorDrawSpriteSlot)((short)slot, 0xFFFF, 0);
    Mini_CondorDrawSpriteSlot(17, 0xFFFF, 0);
}
/*
 * Renders animated blinking indicator sprites on the tactical display.
 */
// FUNCTION: FF7 0x005f87ff
void __cdecl Mini_CondorDrawBlinkingSpritePair(short offsetY)
{
    if ((g_CondorBlinkTick >> 4 & 1) != 0) {
        Mini_CondorSetSpriteSlotSize(21, 240, 168);
    } else {
        Mini_CondorSetSpriteSlotSize(21, 240, 184);
    }
    if ((g_CondorBlinkTick >> 4 & 1) != 0) {
        Mini_CondorSetSpriteSlotSize(51, 224, 184);
    } else {
        Mini_CondorSetSpriteSlotSize(51, 224, 168);
    }
    g_CondorSpriteSlot15HeightX.screenX = g_CondorCameraX + 136;
    g_CondorSpriteSlot15ScreenY = g_CondorCameraY + offsetY;
    Mini_CondorDrawSpriteSlot(21, 0xFFFF, 0);
    g_CondorSpriteSlot33HeightX.screenX = g_CondorCameraX + 152;
    g_CondorSpriteSlot33ScreenY = g_CondorCameraY + offsetY;
    Mini_CondorDrawSpriteSlot(51, 0xFFFF, 0);
}
/*
 * Draws complete HUD interface including score, funds, unit rosters, and alerts.
 */
// FUNCTION: FF7 0x005f88f3
void __cdecl Mini_CondorDrawHud(void)
{
    short val;

    if (g_CondorPhase != 1 && g_CondorMainState != CONDOR_STATE_HELP_SCREEN) {
        if ((*(int *)&g_CondorAlertLevel) >= 88 && g_CondorBattleOutcome == 0 && (g_CondorHudBlinkCounter & 8) != 0) {
            if ((g_CondorHudBlinkCounter & 7) == 0) {
                Mini_CondorSubmitSfxPlay(0xDE, 1);
            }
            g_CondorSpriteSlot1EHeightX.screenX = g_CondorCameraX + 12;
            g_CondorSpriteSlot1EScreenY = g_CondorCameraY + 83;
            Mini_CondorDrawSpriteSlot(30, 0xFFFF, 0);
        }
        g_CondorSpriteSlot1BHeightX.screenX = g_CondorCameraX + 12;
        g_CondorSpriteSlot1BScreenY = g_CondorCameraY + 80;
        Mini_CondorDrawSpriteSlot(27, 0xFFFF, 0);
        g_CondorSpriteSlot1CHeightX.screenX = g_CondorCameraX + 16;
        g_CondorSpriteSlot1CScreenY = g_CondorCameraY + 66;
        Mini_CondorDrawSpriteSlot(28, 0xFFFF, 0);
        g_CondorSpriteSlot1DHeightX.screenX = g_CondorCameraX + 16;
        g_CondorSpriteSlot1DScreenY = g_CondorCameraY + 181;
        Mini_CondorDrawSpriteSlot(29, 0xFFFF, 0);
        Mini_CondorDrawTwoDigitNumbers(g_CondorCameraX + 0x10, g_CondorCameraY + 0x42, g_CondorCameraX + 0x10, g_CondorCameraY + 0xBD);
        g_CondorSpriteSlot1FHeightX.screenX = g_CondorCameraX + 12;
        g_CondorSpriteSlot20HeightX.screenX = g_CondorCameraX + 12;
        g_CondorSpriteSlot21HeightX.screenX = g_CondorCameraX + 12;
        g_CondorSpriteSlot22HeightX.screenX = g_CondorCameraX + 12;
        g_CondorSpriteSlot23HeightX.screenX = g_CondorCameraX + 12;
        g_CondorSpriteSlot24HeightX.screenX = g_CondorCameraX + 12;
        val = *(short *)&g_CondorAlertLevel;
        if (val >= 16) {
            Mini_CondorSetSpriteSlotSize(31, 16, 32);
            Mini_CondorDrawSpriteAtScreenY(0x1F, 163);
            Mini_CondorSetSpriteSlotSize(36, 16, 16);
            Mini_CondorDrawSpriteAtScreenY(0x24, 0xB3 - val);
        } else {
            if (val >= 2) {
                Mini_CondorSetSpriteSlotSize(31, 16, 47);
                Mini_CondorDrawSpriteAtScreenY(0x1F, 178);
            }
            /* Retail caller prototype (int, int, short) -> void preserved for /Od argument promotion on `Mini_CondorSetSpriteSlotSize`. */
            ((void(__cdecl *)(int, int, short))Mini_CondorSetSpriteSlotSize)(0x24, 0x10, (short)val);
            Mini_CondorDrawSpriteAtScreenY(0x24, 163);
        }
        val -= 0x10;
        if (val >= 16) {
            Mini_CondorDrawSpriteAtScreenY(0x20, 0xB3 - val);
        }
        val -= 0x10;
        if (val >= 16) {
            Mini_CondorDrawSpriteAtScreenY(0x21, 0xB3 - val);
        }
        val -= 0x10;
        if (val >= 16) {
            Mini_CondorDrawSpriteAtScreenY(0x22, 0xB3 - val);
        }
        val -= 0x10;
        if (val >= 16) {
            Mini_CondorDrawSpriteAtScreenY(0x23, 0xB3 - val);
        }
        if (g_CondorTickStep >= 2) {
            Mini_CondorSetSpriteSlotPalette(53, 0);
        } else {
            Mini_CondorSetSpriteSlotPalette(53, 2);
        }
        if (g_CondorTickStep >= 3) {
            Mini_CondorSetSpriteSlotPalette(54, 0);
        } else {
            Mini_CondorSetSpriteSlotPalette(54, 2);
        }
        if (g_CondorTickStep >= 4) {
            Mini_CondorSetSpriteSlotPalette(55, 0);
        } else {
            Mini_CondorSetSpriteSlotPalette(55, 2);
        }
        Mini_CondorDrawSpriteAtScreenXY(0x38, 272, 136);
        Mini_CondorDrawSpriteAtScreenXY(0x34, 280, 176);
        Mini_CondorDrawSpriteAtScreenXY(0x35, 280, 168);
        Mini_CondorDrawSpriteAtScreenXY(0x36, 280, 160);
        Mini_CondorDrawSpriteAtScreenXY(0x37, 280, 152);
        Mini_CondorDrawSpriteAtScreenXY(0x39, 272, 184);
    }
    if (g_CondorActiveActionRecord != 0) {
        if (g_CondorSelectPanelPhase == CONDOR_PANEL_CLOSED) {
            g_CondorSelectPanelPhase++;
            Mini_CondorRequestDisplayMode2();
        }
        if (g_CondorMainState != CONDOR_STATE_BANNER_GROW) {
            Mini_CondorDrawBlinkingSpritePair(0x3F);
        }
        g_CondorSpriteSlot13HeightX.screenX = g_CondorCameraX + 32;
        g_CondorSpriteSlot13ScreenY = g_CondorCameraY + g_CondorBannerCounter + -24;
        Mini_CondorDrawThreeSpriteLabel(g_CondorCameraX + 0x20, g_CondorCameraY + g_CondorBannerCounter + -0x15);
        Mini_CondorDrawSpriteSlot(19, 0xFFFF, 0);
        Mini_CondorDrawBannerBar(0x30);
        g_CondorPhaseFrames++;
        if (g_CondorPhaseFrames >= 480) {
            g_CondorActiveActionRecord = 0;
            g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
            Mini_CondorRequestDisplayMode1();
            Mini_CondorSetSelectedReadout(*(short *)&g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex, g_CondorSelectedRecord);
        }
    }
    if (g_CondorMainState == CONDOR_STATE_RECORD_SELECT_WHEEL) {
        Mini_CondorDrawDigitString(g_CondorCameraX + 0x88, g_CondorCameraY + 0x15, 0);
        Mini_CondorDrawDigitString(g_CondorCameraX + 0x88, g_CondorCameraY + 0x21, 1);
        (g_CondorSpriteSlot17HeightX.posX) = g_CondorCameraX + 104;
        g_CondorSpriteSlot11ScreenY = g_CondorCameraY + 16;
        g_CondorSpriteSlot11OffsetX.offsetX = 0x70;
        (g_CondorSpriteSlot17HeightX.height) = 32;
        Mini_CondorSetSpriteSlotSize(17, 0, 200);
        Mini_CondorDrawSpriteSlot(17, 0xFFFF, 0);
        Mini_CondorDrawThreeSpriteLabel(g_CondorCameraX + 0x40, g_CondorCameraY + 0x8D);
        g_CondorSpriteSlot27HeightX.screenX = g_CondorCameraX + 100 - g_CondorCounterDisplayOffset + 32;
        g_CondorSpriteSlot27ScreenY = g_CondorCameraY + 159;
        Mini_CondorDrawDigitString(*(unsigned short *)&g_CondorSpriteSlot27HeightX.screenX, *(unsigned short *)&g_CondorSpriteSlot27ScreenY, 2);
        g_CondorSpriteSlot28HeightX.screenX = g_CondorCameraX + 104 - g_CondorRecordDisplayOffset + 32;
        g_CondorSpriteSlot28ScreenY = g_CondorCameraY + 172;
        Mini_CondorDrawDigitString(*(unsigned short *)&g_CondorSpriteSlot28HeightX.screenX, *(unsigned short *)&g_CondorSpriteSlot28ScreenY, 3);
        g_CondorSpriteSlot29HeightX.screenX = g_CondorCameraX + 132;
        g_CondorSpriteSlot29ScreenY = g_CondorCameraY + 185;
        Mini_CondorDrawDigitString(*(unsigned short *)&g_CondorSpriteSlot29HeightX.screenX, *(unsigned short *)&g_CondorSpriteSlot29ScreenY, 4);
        if (g_CondorRecordDisplaySlot != -1) {
            g_CondorSpriteEntries[g_CondorRecordDisplaySlot + 0x82].screenX = g_CondorCameraX + 166;
            g_CondorSpriteEntries[g_CondorRecordDisplaySlot + 0x82].screenY = g_CondorCameraY + 170;
            Mini_CondorDrawSpriteSlot(g_CondorRecordDisplaySlot + 0x82, 0xFFFF, 0);
        }
        g_CondorSpriteSlot25HeightX.screenX = g_CondorCameraX + 64;
        g_CondorSpriteSlot25ScreenY = g_CondorCameraY + 136;
        Mini_CondorDrawSpriteSlot(37, 0xFFFF, 0);
        g_CondorSpriteSlot26HeightX.screenX = g_CondorCameraX + 64;
        g_CondorSpriteSlot26ScreenY = g_CondorCameraY + 168;
        Mini_CondorDrawSpriteSlot(38, 0xFFFF, 0);
        g_CondorSpriteSlot2FHeightX.screenX = g_CondorCameraX + 160;
        g_CondorSpriteSlot2FScreenY = g_CondorCameraY + 136;
        Mini_CondorDrawSpriteSlot(47, 0xFFFF, 0);
        val = g_CondorBlinkTick >> 3 & 3;
        if (val == 3) {
            val = 1;
        }
        Mini_CondorSetSpriteSlotSize(58, 96, (val << 5) + 0x10);
        g_CondorSpriteSlot3AHeightX.screenX = g_CondorCameraX + 144;
        g_CondorSpriteSlot3AScreenY = g_CondorCameraY + 100;
        Mini_CondorDrawSpriteSlot(58, 0xFFFF, 0);
    } else if (g_CondorPhase == 1) {
        if (g_CondorMainState == CONDOR_STATE_CONFIRM_ROWS) {
            g_pCondorSpriteSlot6PosSource = 0;
            Mini_CondorDrawSpriteSlot(6, 0xFFFF, 0);
            g_pCondorSpriteSlot6PosSource = &g_CondorHudCursor;
            g_CondorSpriteSlot31HeightX.screenX = g_CondorCameraX + 28;
            g_CondorSpriteSlot31ScreenY = g_CondorCameraY + 64;
            g_pCondorSpriteSlot31PosSource = 0;
            Mini_CondorDrawSpriteSlot(49, 0xFFFF, 0);
            g_CondorSpriteSlot2AHeightX.screenX = g_CondorCameraX + 64;
            g_CondorSpriteSlot2AScreenY = g_CondorCameraY + 75;
            g_CondorSpriteSlot2BHeightX.screenX = g_CondorCameraX + 64;
            g_CondorSpriteSlot2BScreenY = g_CondorCameraY + 89;
            Mini_CondorDrawSpriteSlot(42, 0xFFFF, 0);
            Mini_CondorDrawSpriteSlot(43, 0xFFFF, 0);
            Mini_CondorDrawBannerBar(0x30);
            g_CondorBannerCounter = g_CondorBannerTarget;
        } else {
            if (g_CondorFadeAlpha > 8) {
                Mini_CondorDrawBlinkingSpritePair(0xC0);
            }
            if (g_CondorChoiceTimer == 0 || g_CondorMainState != CONDOR_STATE_RECORD_SELECT_WHEEL) {
                if (g_CondorFadeAlpha > 8) {
                    Mini_CondorDrawBannerBar(0xB0);
                } else {
                    g_CondorBannerTarget = 0xB0;
                }
                g_CondorBannerCounter = g_CondorBannerTarget;
            }
        }
    }
}
/*
 * Draws a UI sprite at default horizontal coordinate and specified vertical position.
 */
// FUNCTION: FF7 0x005f92b9
void __cdecl Mini_CondorDrawSpriteAtScreenY(short slot, short y)
{
    g_CondorSpriteEntries[slot].screenY = g_CondorCameraY + y;
    /* Retail call-site prototype (short, int, int) -> int preserved for /Od caller codegen. */
    ((int(__cdecl *)(short, int, int))Mini_CondorDrawSpriteSlot)(slot, 0xFFFF, 0);
}
/*
 * Draws a UI sprite at specified 2D screen coordinates.
 */
// FUNCTION: FF7 0x005f92ee
void __cdecl Mini_CondorDrawSpriteAtScreenXY(short slot, short x, short y)
{
    g_CondorSpriteEntries[slot].screenX = g_CondorCameraX + x;
    g_CondorSpriteEntries[slot].screenY = g_CondorCameraY + y;
    /* Retail call-site prototype (short, int, int) -> int preserved for /Od caller codegen. */
    ((int(__cdecl *)(short, int, int))Mini_CondorDrawSpriteSlot)((short)slot, 0xFFFF, 0);
}
/*
 * Initializes vertex and UV coordinates for all battlefield HUD sprite elements.
 */
// FUNCTION: FF7 0x005f933f
void __cdecl Mini_CondorInitHudSprites(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[40];
        char cellX0;
        struct { unsigned char pad_to_1[0x1]; char cellY0; };
        struct { unsigned char pad_to_2[0x2]; char cellX1; };
        struct { unsigned char pad_to_3[0x3]; char cellY1; };
        struct { unsigned char pad_to_4[0x4]; char cellX2; };
        struct { unsigned char pad_to_5[0x5]; char cellY2; };
        struct { unsigned char pad_to_6[0x6]; char cellX3; };
        struct { unsigned char pad_to_7[0x7]; char cellY3; };
        struct { unsigned char pad_to_8[0x8]; char cellX4; };
        struct { unsigned char pad_to_9[0x9]; char cellY4; };
        struct { unsigned char pad_to_A[0xA]; char cellX5; };
        struct { unsigned char pad_to_B[0xB]; char cellY5; };
        struct { unsigned char pad_to_C[0xC]; char cellX6; };
        struct { unsigned char pad_to_D[0xD]; char cellY6; };
        struct { unsigned char pad_to_E[0xE]; char cellX7; };
        struct { unsigned char pad_to_F[0xF]; char cellY7; };
        struct { unsigned char pad_to_10[0x10]; char cellX8; };
        struct { unsigned char pad_to_11[0x11]; char cellY8; };
        struct { unsigned char pad_to_12[0x12]; char cellX9; };
        struct { unsigned char pad_to_13[0x13]; char cellY9; };
        struct { unsigned char pad_to_14[0x14]; char cellX10; };
        struct { unsigned char pad_to_15[0x15]; char cellY10; };
        struct { unsigned char pad_to_16[0x16]; char cellX11; };
        struct { unsigned char pad_to_17[0x17]; char cellY11; };
        struct { unsigned char pad_to_18[0x18]; char cellX12; };
        struct { unsigned char pad_to_19[0x19]; char cellY12; };
        struct { unsigned char pad_to_1A[0x1A]; char cellX13; };
        struct { unsigned char pad_to_1B[0x1B]; char cellY13; };
        struct { unsigned char pad_to_1C[0x1C]; int spriteIndex; };
        struct { unsigned char pad_to_20[0x20]; int spriteY; };
        struct { unsigned char pad_to_24[0x24]; int spriteX; };
    } frame;
#pragma pack(pop)

    frame.cellX0 = 10;
    frame.cellY0 = 6;
    frame.cellX1 = 0;
    frame.cellY1 = 0;
    frame.cellX2 = 0;
    frame.cellY2 = 2;
    frame.cellX3 = 0;
    frame.cellY3 = 4;
    frame.cellX4 = 0;
    frame.cellY4 = 6;
    frame.cellX5 = 5;
    frame.cellY5 = 0;
    frame.cellX6 = 5;
    frame.cellY6 = 2;
    frame.cellX7 = 5;
    frame.cellY7 = 4;
    frame.cellX8 = 5;
    frame.cellY8 = 6;
    frame.cellX9 = 10;
    frame.cellY9 = 6;
    frame.cellX10 = 10;
    frame.cellY10 = 6;
    frame.cellX11 = 10;
    frame.cellY11 = 6;
    frame.cellX12 = 10;
    frame.cellY12 = 0;
    frame.cellX13 = 10;
    frame.cellY13 = 2;
    g_CondorCursorPos.x = g_CondorCameraX + 152;
    g_CondorCursorPos.y = g_CondorCameraY + 96;
    Mini_CondorInitSprite(1, 32, 32, 0, 48, 6, 0, &g_CondorCursorPos, 2, FLOAT_0_15_BITS);
    Mini_CondorInitSprite(2, 96, 16, 160, 0, 7, 0, (int *)&g_CondorHudCursor, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(3, 96, 16, 0, 208, 0, 0, (int *)&g_CondorHudCursor, 2, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(4, 96, 48, 0, 0, 2, 1, (int *)&g_CondorHudCursor, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(5, 32, 32, 64, 48, 6, 0, &g_CondorCursorPos, 2, FLOAT_0_1_BITS);
    Mini_CondorInitSprite(6, 16, 16, 208, 120, 0, 0, (int *)&g_CondorHudCursor, 1, FLOAT_0_15_BITS);
    Mini_CondorInitSprite(7, 16, 16, 0, 48, 6, 0, &g_CondorCursor, 2, FLOAT_0_15_BITS);
    Mini_CondorInitSprite(9, 48, 16, 160, 8, 0, 0, &g_CondorCursor, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0xA, 16, 32, 32, 80, 6, 0, &g_CondorCursor, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0xB, 48, 16, 0, 0, 7, 0, (int *)&g_CondorHudCursor, 2, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0xC, 48, 16, 0, 0, 7, 0, (int *)&g_CondorHudCursor, 2, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0xD, 48, 16, 0, 0, 7, 0, (int *)&g_CondorHudCursor, 2, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0xE, 64, 16, 96, 0, 2, 1, (int *)&g_CondorHudCursor, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0xF, 64, 16, 96, 0, 2, 1, (int *)&g_CondorHudCursor, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x10, 64, 16, 96, 0, 2, 1, (int *)&g_CondorHudCursor, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x11, 256, 32, 0, 136, 2, 1, 0, 1, FLOAT_0_4_BITS);
    Mini_CondorInitSprite(0x12, 256, 16, 0, 224, 7, 0, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x13, 128, 32, 0, 112, 2, 1, 0, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x14, 96, 16, 160, 0, 7, 0, 0, 1, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x15, 16, 16, 240, 112, 5, 0, 0, 1, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x16, 16, 16, 128, 120, 0, 0, (int *)&g_CondorHudCursor, 1, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x17, 16, 16, 144, 120, 0, 0, (int *)&g_CondorHudCursor, 1, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x18, 16, 16, 160, 120, 0, 0, (int *)&g_CondorHudCursor, 1, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x19, 16, 16, 176, 120, 0, 0, (int *)&g_CondorHudCursor, 1, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x1A, 16, 16, 192, 120, 0, 0, (int *)&g_CondorHudCursor, 1, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x1B, 16, 112, 0, 16, 0, 0, 0, 2, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x1C, 32, 16, 32, 16, 0, 0, 0, 2, FLOAT_0_25_BITS);
    Mini_CondorInitSprite(0x1D, 32, 16, 32, 32, 0, 0, 0, 2, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x1E, 48, 16, 32, 0, 0, 0, 0, 2, FLOAT_0_18_BITS);
    Mini_CondorInitSprite(0x1F, 16, 16, 16, 32, 0, 0, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x20, 16, 16, 16, 24, 0, 0, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x21, 16, 16, 16, 24, 0, 0, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x22, 16, 16, 16, 24, 0, 0, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x23, 16, 16, 16, 24, 0, 0, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x24, 16, 16, 16, 16, 0, 0, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x25, 96, 32, 0, 168, 2, 1, 0, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x26, 96, 32, 96, 168, 2, 1, 0, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x27, 32, 16, 96, 192, 0, 0, 0, 2, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x28, 32, 16, 96, 208, 0, 0, 0, 2, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x29, 32, 16, 96, 224, 0, 0, 0, 2, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x2A, 40, 16, 0, 224, 0, 0, 0, 5, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x2B, 40, 16, 40, 224, 0, 0, 0, 5, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x2C, 48, 16, 0, 128, 7, 0, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x2D, 64, 32, 0, 144, 0x102, 1, 0, 2, FLOAT_0_3_BITS);
    Mini_CondorInitSprite(0x2E, 32, 32, 32, 80, 6, 0, &g_CondorCursor, 2, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x2F, 96, 64, 0, 48, 2, 1, 0, 1, FLOAT_0_952_BITS);
    Mini_CondorInitSprite(0x30, 80, 32, 0, 192, 0, 0, 0, 2, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x31, 96, 48, 96, 64, 2, 1, 0, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x32, 16, 16, 0, 224, 0, 0, 0, 1, FLOAT_HALF_BITS);
    Mini_CondorInitSprite(0x33, 16, 16, 224, 112, 5, 0, 0, 1, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x34, 16, 16, 240, 120, 0, 0, 0, 1, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x35, 16, 16, 240, 120, 0, 0, 0, 1, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x36, 16, 16, 240, 120, 0, 0, 0, 1, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x37, 16, 16, 240, 120, 0, 0, 0, 1, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x38, 48, 16, 208, 80, 0, 0, 0, 1, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x39, 48, 16, 208, 96, 0, 0, 0, 1, FLOAT_0_2_BITS);
    Mini_CondorInitSprite(0x3A, 32, 32, 96, 16, 6, 0, 0, 2, FLOAT_0_2_BITS);
    for (frame.spriteIndex = 0; frame.spriteIndex < 16; frame.spriteIndex++) {
        Mini_CondorInitSprite(frame.spriteIndex + 0x3B, 8, 8, frame.spriteIndex * 8 + 0x80, 16, 0, 0, 0, 6, FLOAT_0_3_BITS);
    }
    for (frame.spriteIndex = 0; frame.spriteIndex < 14; frame.spriteIndex++) {
        frame.spriteX = *(unsigned char *)(((char *)&frame.cellX0 + frame.spriteIndex * 2)) << 4;
        frame.spriteY = ((*(unsigned char *)((char *)&frame.cellY0 + frame.spriteIndex * 2) << 4 << 4) / 8) / 2;
        /* Retail caller prototype (int, int, int, short, short, int, int, int, int, int) -> void preserved for /Od argument promotion on `Mini_CondorInitSprite`. */
        ((void(__cdecl *)(int, int, int, short, short, int, int, int, int, int))Mini_CondorInitSprite)(frame.spriteIndex + 0x82, 0x50, 0x20, *(short *)&frame.spriteX, *(short *)&frame.spriteY, 0, 0, 0, 8, FLOAT_0_3_BITS);
    }
    for (frame.spriteIndex = 0; frame.spriteIndex < 24; frame.spriteIndex++) {
        Mini_CondorInitSprite(frame.spriteIndex + 0x5F, 64, 16, frame.spriteIndex / 6 << 6, (frame.spriteIndex % 6 << 4) + 0x20, 0, 0, 0, 6, FLOAT_0_3_BITS);
    }
    for (frame.spriteIndex = 0; frame.spriteIndex < 10; frame.spriteIndex++) {
        Mini_CondorInitSprite(frame.spriteIndex + 0x91, 8, 16, frame.spriteIndex * 8 + 0x80, 0, 0, 0, 0, 6, FLOAT_0_3_BITS);
    }
    for (frame.spriteIndex = 0; frame.spriteIndex < 14; frame.spriteIndex++) {
        Mini_CondorInitSprite(frame.spriteIndex + 0x9B, 8, 7, frame.spriteIndex * 8 + 0x80, 25, 0, 0, 0, 6, FLOAT_0_3_BITS);
    }
    for (frame.spriteIndex = 0; frame.spriteIndex < 19; frame.spriteIndex++) {
        if (frame.spriteIndex < 16) {
            Mini_CondorInitSprite(frame.spriteIndex + 0x4B, 256, 16, 0, frame.spriteIndex << 4, 0, 0, 0, 5, FLOAT_0_3_BITS);
        } else {
            /* Numeric HUD sprite slot index (16..18). */
            switch (frame.spriteIndex) {
            case CONDOR_HUD_BANNER_SPRITE_0:
                Mini_CondorInitSprite(frame.spriteIndex + 0x4B, 256, 16, 0, 16, 0, 0, 0, 7, FLOAT_0_3_BITS);
                break;
            case CONDOR_HUD_BANNER_SPRITE_1:
                Mini_CondorInitSprite(frame.spriteIndex + 0x4B, 256, 16, 0, 32, 0, 0, 0, 7, FLOAT_0_3_BITS);
                break;
            case CONDOR_HUD_BANNER_SPRITE_2:
                Mini_CondorInitSprite(frame.spriteIndex + 0x4B, 256, 16, 0, 128, 0, 0, 0, 7, FLOAT_0_3_BITS);
                break;
            }
        }
    }
}
/*
 * Renders two-digit decimal value using digit sprite glyphs.
 */
// FUNCTION: FF7 0x005f9e09
void __cdecl Mini_CondorDrawTwoDigitNumbers(int x1, int y1, int x2, int y2)
{
    if (g_CondorFirstNumberVisible != 0) {
        if (g_CondorFirstNumberLowSlot != 0) {
            g_CondorSpriteEntries[g_CondorFirstNumberLowSlot + 0x3B].screenX = x1;
            g_CondorSpriteEntries[g_CondorFirstNumberLowSlot + 0x3B].screenY = y1;
            Mini_CondorDrawSpriteSlot(g_CondorFirstNumberLowSlot + 0x3B, 0xFFFF, 0);
        }
        g_CondorSpriteEntries[g_CondorFirstNumberHighSlot + 0x3B].screenX = x1 + 8;
        g_CondorSpriteEntries[g_CondorFirstNumberHighSlot + 0x3B].screenY = y1;
        Mini_CondorDrawSpriteSlot(g_CondorFirstNumberHighSlot + 0x3B, 0xFFFF, 0);
    }
    if (g_CondorSecondNumberVisible != 0) {
        if (g_CondorSecondNumberLowSlot != 0) {
            g_CondorSpriteEntries[g_CondorSecondNumberLowSlot + 0x3B].screenX = x2;
            g_CondorSpriteEntries[g_CondorSecondNumberLowSlot + 0x3B].screenY = y2;
            Mini_CondorDrawSpriteSlot(g_CondorSecondNumberLowSlot + 0x3B, 0xFFFF, 0);
        }
        g_CondorSpriteEntries[g_CondorSecondNumberHighSlot + 0x3B].screenX = x2 + 8;
        g_CondorSpriteEntries[g_CondorSecondNumberHighSlot + 0x3B].screenY = y2;
        Mini_CondorDrawSpriteSlot(g_CondorSecondNumberHighSlot + 0x3B, 0xFFFF, 0);
    }
}
/*
 * Formats and renders a multi-digit numerical string on the display.
 */
// FUNCTION: FF7 0x005f9f56
void __cdecl Mini_CondorDrawDigitString(int x, int y, int stringIndex)
{
    int seenDigit;
    unsigned short digitIdx;
    int drawOn;
    int show;

    seenDigit = 0;
    if (g_CondorDigitGroupTable[stringIndex].isVisible != 0) {
        for (digitIdx = 0; digitIdx < g_CondorDigitGroupTable[stringIndex].digitCount; digitIdx++) {
            if (g_CondorDigitGroupTable[stringIndex].digitSprites[digitIdx] == 0 && digitIdx != g_CondorDigitGroupTable[stringIndex].digitCount - 1 && seenDigit == 0) {
                show = 0;
            } else {
                show = 1;
            }
            drawOn = show;
            if (drawOn != 0) {
                if (g_CondorDigitGroupTable[stringIndex].digitSprites[digitIdx] < 16) {
                    g_CondorSpriteEntries[g_CondorDigitGroupTable[stringIndex].digitSprites[digitIdx] + 0x3B].screenX = x + digitIdx * 8;
                    g_CondorSpriteEntries[g_CondorDigitGroupTable[stringIndex].digitSprites[digitIdx] + 0x3B].screenY = y;
                    Mini_CondorDrawSpriteSlot(g_CondorDigitGroupTable[stringIndex].digitSprites[digitIdx] + 0x3B, 0xFFFF, 0);
                    seenDigit = 1;
                } else {
                    g_CondorSpriteEntries[g_CondorDigitGroupTable[stringIndex].digitSprites[digitIdx] + 0x1B].screenX = x + digitIdx * 8;
                    g_CondorSpriteEntries[g_CondorDigitGroupTable[stringIndex].digitSprites[digitIdx] + 0x1B].screenY = y;
                    Mini_CondorDrawSpriteSlot(g_CondorDigitGroupTable[stringIndex].digitSprites[digitIdx] + 0x1B, 0xFFFF, 0);
                }
            }
        }
    }
}
/*
 * Draws a composite label consisting of three sprite components.
 */
// FUNCTION: FF7 0x005fa132
void __cdecl Mini_CondorDrawThreeSpriteLabel(int x, int y)
{
    if (g_CondorSelectedReadoutVisible != 0) {
        g_CondorSpriteEntries[g_CondorSelectedUnitIndex + 0x5F].screenX = x + 8;
        g_CondorSpriteEntries[g_CondorSelectedUnitIndex + 0x5F].screenY = y + 4;
        Mini_CondorDrawSpriteSlot(g_CondorSelectedUnitIndex + 0x5F, 0xFFFF, 0);
        g_CondorSpriteEntries[g_CondorTensDigitSlot + 0x91].screenX = 8 + x + g_CondorSpriteEntries[g_CondorSelectedUnitIndex + 0x5F].offsetX;
        g_CondorSpriteEntries[g_CondorTensDigitSlot + 0x91].screenY = y + 4;
        Mini_CondorDrawSpriteSlot(g_CondorTensDigitSlot + 0x91, 0xFFFF, 0);
        g_CondorSpriteEntries[g_CondorOnesDigitSlot + 0x91].screenX = 8 + x + g_CondorSpriteEntries[g_CondorSelectedUnitIndex + 0x5F].offsetX + g_CondorSpriteEntries[g_CondorTensDigitSlot + 0x91].offsetX;
        g_CondorSpriteEntries[g_CondorOnesDigitSlot + 0x91].screenY = y + 4;
        Mini_CondorDrawSpriteSlot(g_CondorOnesDigitSlot + 0x91, 0xFFFF, 0);
    }
}
} /* extern "C" */

// src/ff7/src/battle/battle_arena_penalty_rounds.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The arena state machines: the selection result, the slot penalty selection that runs
//   until a penalty is applied or skipped, and the round break between consecutive battle rounds.

// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "ff7/enums/menu.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
extern "C" {

// clang-format on



// clang-format off
// GLOBAL: FF7 0x009ae158
int g_ArenaSelectionResult = 0;
// GLOBAL: FF7 0x009ae15c
int g_ArenaSlotPenaltyState = 0;
// GLOBAL: FF7 0x009ae160
int g_ArenaSlotPenaltyCounter = 0;
// GLOBAL: FF7 0x009ae164
int g_ArenaSlotPenaltySubState = 0;
// GLOBAL: FF7 0x009ae168
int g_ArenaRoundBreakState = 0;
// GLOBAL: FF7 0x009ae16c
int g_ArenaRoundBreakCounter = 0;
// GLOBAL: FF7 0x009ae170
int g_ArenaRoundBreakSubState = 0;
unsigned char g_bssPad_009ae174[4] = {0};
// clang-format on

/*
 * Records the player's answer in a Battle Square window: g_ArenaSelectionResult becomes
 * BATTLE_ARENA_SELECTION_ANSWERED, with BATTLE_ARENA_SELECTION_YES when `selectedOption` is nonzero.
 * The battle menu calls it from the continue-or-quit choice between rounds
 * (1 for the first entry, 0 for the other), which Battle_RunArenaRoundBreak turns into "fight the
 * next round", and from the handicap slot screen (0 when the player skips the reels, 1 once they
 * have stopped), which makes Battle_RunArenaSlotPenaltySelection apply the slot penalty
 * (Mini_ApplySlotPenalty) only on 1.
 */
// FUNCTION: FF7 0x00431c80
void __cdecl Battle_SetArenaSelectionResult(int selectedOption)
{
    int result;

    result = BATTLE_ARENA_SELECTION_ANSWERED;
    if (selectedOption != 0) {
        result |= BATTLE_ARENA_SELECTION_YES;
    }
    g_ArenaSelectionResult = result;
}

/* Runs the arena penalty selection state machine until penalty is applied or skipped. */
// FUNCTION: FF7 0x00431ca6
int __cdecl Battle_RunArenaSlotPenaltySelection(void)
{
    int result;
    int partyIdx;
    int r;

    result = 0;
    switch (g_ArenaSlotPenaltyState) {
    case BATTLE_ARENA_PHASE_START:
        g_ArenaSelectionResult = 0;
        gBattleAiGlobals.arenaFlags |= BATTLE_ARENA_BREAK_ACTIVE;
        for (partyIdx = 0; partyIdx < 3; partyIdx++) {
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, partyIdx, BATTLE_ATB_EVENT_RELEASE_COMMAND_MENU, 0);
        }
        g_ArenaSlotPenaltyState = BATTLE_ARENA_PHASE_PLAY_INTRO;
    case BATTLE_ARENA_PHASE_PLAY_INTRO:
        Battle_PlaySequence();
        if (g_BattleSequencePlayDone != 0) {
            g_BattleSequencePlayDone = 0;
            g_ArenaSlotPenaltyCounter++;
            if (g_ArenaSlotPenaltyCounter >= 4) {
                g_ArenaSlotPenaltyCounter = 0;
                g_ArenaSlotPenaltyState = BATTLE_ARENA_PHASE_OPEN_WINDOW;
            }
        }
        g_BattleArenaSlotSequenceActive = 1;
        break;
    case BATTLE_ARENA_PHASE_OPEN_WINDOW:
        g_BattleMenuState = BATTLE_MENU_ARENA_SLOT;
        Battle_OpenWindow(-1, -1, BATTLE_MENU_ARENA_SLOT);
        g_ArenaSlotPenaltyState = BATTLE_ARENA_PHASE_WAIT_ANSWER;
    case BATTLE_ARENA_PHASE_WAIT_ANSWER:
        switch (g_ArenaSlotPenaltySubState) {
        case BATTLE_ARENA_WAIT_CHECK_ANSWER:
            if (g_ArenaSelectionResult != 0) {
                g_ArenaSlotPenaltySubState = BATTLE_ARENA_WAIT_CHECK_ANSWER;
                g_ArenaSlotPenaltyState = BATTLE_ARENA_PHASE_FINISH;
                break;
            }
            g_ArenaSlotPenaltySubState = BATTLE_ARENA_WAIT_PLAY_SEQUENCE;
        case BATTLE_ARENA_WAIT_PLAY_SEQUENCE:
            Battle_PlaySequence();
            if (g_BattleSequencePlayDone != 0) {
                g_BattleSequencePlayDone = 0;
                g_ArenaSlotPenaltySubState = BATTLE_ARENA_WAIT_CHECK_ANSWER;
            }
            g_BattleArenaSlotSequenceActive = 1;
        }
        break;
    case BATTLE_ARENA_PHASE_FINISH:
        if ((g_ArenaSelectionResult & BATTLE_ARENA_SELECTION_YES) != 0) {
            Mini_ApplySlotPenalty();
            result = 1;
        }
        g_ArenaSlotPenaltyState = BATTLE_ARENA_PHASE_START;
        g_BattleArenaSlotPenaltyDone = 1;
    }
    return result;
}

/* Runs the arena round-break state machine between consecutive battle rounds. */
// FUNCTION: FF7 0x00431e49
int __cdecl Battle_RunArenaRoundBreak(void)
{
    int continueBattle;
    int slotIdx;

    continueBattle = 0;
    switch (g_ArenaRoundBreakState) {
    case BATTLE_ARENA_PHASE_START:
        g_ArenaSelectionResult = 0;
        gBattleAiGlobals.arenaFlags |= BATTLE_ARENA_BREAK_ACTIVE;
        for (slotIdx = 0; slotIdx < 3; slotIdx++) {
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, slotIdx, BATTLE_ATB_EVENT_RELEASE_COMMAND_MENU, 0);
        }
        g_ArenaRoundBreakState = BATTLE_ARENA_PHASE_PLAY_INTRO;
    case BATTLE_ARENA_PHASE_PLAY_INTRO:
        Battle_PlaySequence();
        if (g_BattleSequencePlayDone != 0) {
            g_BattleSequencePlayDone = 0;
            g_ArenaRoundBreakCounter++;
            if (g_ArenaRoundBreakCounter > 3) {
                g_ArenaRoundBreakCounter = 0;
                g_ArenaRoundBreakState = BATTLE_ARENA_PHASE_OPEN_WINDOW;
            }
        }
        break;
    case BATTLE_ARENA_PHASE_OPEN_WINDOW:
        g_BattleMenuState = BATTLE_MENU_ARENA_CONTINUE_CHOICE;
        Battle_OpenWindow(-1, -1, BATTLE_MENU_ARENA_CONTINUE_CHOICE);
        g_ArenaRoundBreakState = BATTLE_ARENA_PHASE_WAIT_ANSWER;
    case BATTLE_ARENA_PHASE_WAIT_ANSWER:
        switch (g_ArenaRoundBreakSubState) {
        case BATTLE_ARENA_WAIT_CHECK_ANSWER:
            if (g_ArenaSelectionResult != 0) {
                g_ArenaRoundBreakSubState = BATTLE_ARENA_WAIT_CHECK_ANSWER;
                g_ArenaRoundBreakState = BATTLE_ARENA_PHASE_FINISH;
                break;
            }
            g_ArenaRoundBreakSubState = BATTLE_ARENA_WAIT_PLAY_SEQUENCE;
        case BATTLE_ARENA_WAIT_PLAY_SEQUENCE:
            Battle_PlaySequence();
            if (g_BattleSequencePlayDone != 0) {
                g_BattleSequencePlayDone = 0;
                g_ArenaRoundBreakSubState = BATTLE_ARENA_WAIT_CHECK_ANSWER;
            }
        }
        if (g_ArenaRoundBreakState != BATTLE_ARENA_PHASE_FINISH) {
            break;
        }
    case BATTLE_ARENA_PHASE_FINISH:
        gBattleAiGlobals.arenaFlags &= 0xFFFD; /* clears BATTLE_ARENA_BREAK_ACTIVE */
        for (slotIdx = 0; slotIdx < 3; slotIdx++) {
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, slotIdx, BATTLE_ATB_EVENT_REOPEN_COMMAND_MENU, 0);
        }
        if ((g_ArenaSelectionResult & BATTLE_ARENA_SELECTION_YES) != 0) {
            continueBattle = 1;
        }
        g_ArenaRoundBreakState = BATTLE_ARENA_PHASE_START;
        g_BattleArenaRoundBreakDone = 1;
        break;
    default:
        g_ArenaRoundBreakState = BATTLE_ARENA_PHASE_START;
        g_BattleArenaRoundBreakDone = 1;
    }
    return continueBattle;
}
} /* extern "C" */

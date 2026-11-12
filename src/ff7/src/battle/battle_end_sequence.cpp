// src/ff7/src/battle/battle_end_sequence.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The battle-end state machine, which resolves defeat, escape and victory transitions and
//   their rewards.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/api/battle.h"
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include <string.h>
#include <stdio.h>
#include "lib/codegen.h"
#include "ff7/enums/battle.h"
#include "ff7/enums/main.h"
#include "ff7/savemap.h"
#include "ff7/enums/menu.h"
#include "ff7/types/menu.h"
extern "C" {


// clang-format off
// GLOBAL: FF7 0x009ae11c
int g_BattleEndOutcomeFlags = 0;
// GLOBAL: FF7 0x009ae120
int g_BattleEndResultScreenFlag = 0;
// GLOBAL: FF7 0x009ae124
int g_BattleEndAvgHp = 0;
// GLOBAL: FF7 0x009ae128
int g_BattleEndReserveCharMask = 0;
// GLOBAL: FF7 0x009ae12c
int g_BattleEndDroppedItemCount = 0;
// GLOBAL: FF7 0x009ae130
int g_BattleEndAvgMp = 0;
// GLOBAL: FF7 0x009ae134
int g_BattleEndVictoryDefeatMask = 0;
// GLOBAL: FF7 0x009ae138
int g_BattleEndSequenceDelay = 0;
// GLOBAL: FF7 0x009ae13c
int g_BattleEndSequenceState = 0;
// GLOBAL: FF7 0x009ae140
int g_BattleEndPulseCounter = 0;
// GLOBAL: FF7 0x009ae144
int g_BattleEndSubState = 0;
// GLOBAL: FF7 0x009ae148
int g_BattleEndDropPhaseCounter = 0;
// clang-format on
#define g_aBattlePartyStat (g_BattleState.partyStats)

#pragma intrinsic(memset)


/*
 * Ends a battle: plays the closing animations, then hands out the rewards and writes the party
 * back into the savemap. A state machine on g_BattleEndSequenceState that the battle phase
 * machine runs each frame until it sets g_BattleEndSequenceDone.
 * RESOLVE_OUTCOME takes the end flags of gBattleAiGlobals (BattleOutcomeFlag: DEFEAT, FLED, OVER)
 * and adds the scene's battle flags 0x40 (Battle Square) and 0x80; 0x40 drops OVER, OVER drops
 * FLED, and a defeat while a party member is excluded counts as ESCAPED instead. It runs AI event 6
 * for every combatant present, records the enemies' HP for the Battle Square when its arena flag
 * BATTLE_ARENA_KEEP_ROUND_HP is set, averages the HP and MP of the party members present when g_BattleState.participatingPartyMask is set, keeps
 * only status bits 0x1831 of the party actors, and picks the closing: a defeat posts battle message
 * 8 and waits 30 sequence passes, an escape plays idle pose 0xE for 8 passes, a victory in a scene
 * with flag 8 (and without battle flag 0x100) plays the victory pose 0xC for 0x31 passes.
 * PLAY_CLOSING to RELEASE_DROPS play those sequences (Battle_PlaySequence), set
 * FIELD_BATTLE_FLAG_LOST for a defeat and FIELD_BATTLE_FLAG_ESCAPED for an escape or when no enemy
 * is left in the battle, and release pending item drops for 16 passes.
 * REWARDS writes the 320 inventory slots back into gSavemap.items. When no end flag in 0xDE is set
 * and the Battle Square's arena flag 0x20 is clear (an ordinary victory), it sums EXP, gil and AP
 * of each defeated enemy (status death or petrify) and rolls its drop (Battle_RollEnemyDropOrSteal)
 * into g_BattleRewardItems; scales the gil by the reward scale / 16 when one is set; gives each
 * living party member the AP (Battle_AwardApToEquippedMateria) and the EXP scaled by its stat-block
 * byte / 16, applies the EXP (0 for the fallen), limit unlocks and materia type bits to the
 * character records, and gives the characters outside the party half EXP. Then it writes the
 * party's HP and MP back (Battle_WritePartyStateBackToSavemap), stops the sound effects, opens the
 * result screens (Battle_InitResultScreenState) after a victory with rewards, resets the state and
 * sets g_BattleEndSequenceDone.
 * Defects of the original: a battle with more than four different dropped items writes past the
 * four entries of g_BattleRewardItems.
 */
// FUNCTION: FF7 0x00430dd0
void __cdecl Battle_RunEndSequence(void)
{
    int actorIdx;
    int squareIdx;
    int memberIdx;
    BattleEndItemRecord *pItemRecord;
    unsigned short dropId;
    int enemyPtr;
    int enemySlot;
    SavemapCharacter *charRecordPtr;
    unsigned int charExp;
    int nextStatBlock;
    int typeFlagsDiff;
    int subIdx;
    int presentCount;
    int popCnt;

    switch (g_BattleEndSequenceState) {
    case BATTLE_END_STATE_RESOLVE_OUTCOME:
        g_BattleEndSequenceDelay = 0;
        g_BattleEndDroppedItemCount = 0;
        g_BattleEndOutcomeFlags = gBattleAiGlobals.flags;
        g_BattleEndResultScreenFlag = 0;
        g_BattleEndReserveCharMask = gSavemap.phsVisibilityMask;
        g_BattleEndAvgHp = 0;
        g_BattleEndAvgMp = 0;
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if (gBattleAiGlobals.combatants[actorIdx].unitId != -1) {
                Battle_RunAiScriptEvent(actorIdx, 6, 6);
            }
        }
        if ((gBattleAiGlobals.arenaFlags & BATTLE_ARENA_KEEP_ROUND_HP) != 0) {
            squareIdx = g_BattleArenaRoundIndex;
            for (actorIdx = 0; actorIdx < 6; actorIdx++) {
                *(int *)((char *)&g_BattleArenaSavedRoundHp + (squareIdx * 0x18 + actorIdx * 4)) = gBattleAiGlobals.combatants[actorIdx + 4].curHp;
            }
        }
        if (g_BattleState.participatingPartyMask != 0) {
            popCnt = Battle_PopCountMask(g_BattleState.participatingPartyMask);
            presentCount = 0;
            for (actorIdx = 0; actorIdx < 4; actorIdx++) {
                if (gBattleAiGlobals.combatants[actorIdx].unitId != -1) {
                    g_BattleEndAvgHp += gBattleAiGlobals.combatants[actorIdx].curHp;
                    g_BattleEndAvgMp += gBattleAiGlobals.combatants[actorIdx].curMp;
                    presentCount++;
                }
            }
            g_BattleEndAvgHp = g_BattleEndAvgHp / popCnt;
            g_BattleEndAvgMp = g_BattleEndAvgMp / popCnt;
            if (presentCount == 0) {
                g_BattleState.participatingPartyMask = 0;
            }
        }
        if ((g_BattleScene.flags & 0x40) != 0) {
            g_BattleEndOutcomeFlags |= 0x40;
        }
        if ((g_BattleScene.flags & 0x80) != 0) {
            g_BattleEndOutcomeFlags |= 0x80;
        }
        if ((g_BattleEndOutcomeFlags & 0x40) != 0) {
            g_BattleEndOutcomeFlags &= ~BATTLE_OUTCOME_OVER;
        }
        if ((g_BattleEndOutcomeFlags & BATTLE_OUTCOME_OVER) != 0) {
            g_BattleEndOutcomeFlags &= ~BATTLE_OUTCOME_FLED;
        }
        if ((g_BattleEndOutcomeFlags & BATTLE_OUTCOME_DEFEAT) != 0) {
            g_BattleEndOutcomeFlags &= ~BATTLE_OUTCOME_ESCAPED;
            if ((g_BattleState.excludedActorMask & BATTLE_ACTOR_MASK_ALLIES) != 0) {
                g_BattleEndOutcomeFlags &= ~BATTLE_OUTCOME_DEFEAT;
                g_BattleEndOutcomeFlags |= BATTLE_OUTCOME_ESCAPED;
            }
        }
        Battle_ResetResultRewardTotals();
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actorIdx, BATTLE_ATB_EVENT_RELEASE_COMMAND_MENU, 0);
        }
        for (actorIdx = 0; actorIdx < 3; actorIdx++) {
            g_BattleScene.actors[actorIdx].savedStatus &= 0x1831;
            g_BattleScene.actors[actorIdx].statusFlags = g_BattleScene.actors[actorIdx].savedStatus;
        }
        if ((g_BattleEndOutcomeFlags & BATTLE_OUTCOME_ESCAPED) == 0) {
            if ((g_BattleEndOutcomeFlags & BATTLE_OUTCOME_DEFEAT) != 0) {
                if ((g_BattleEndOutcomeFlags & 0x40) == 0) {
                    g_BattleEndSequenceDelay = 0x1E;
                    Battle_PostBattleMessage(8);
                    g_BattleDefeatMusicPending = 1;
                }
            } else if ((g_BattleEndOutcomeFlags & BATTLE_OUTCOME_FLED) != 0) {
                if ((g_BattleEndOutcomeFlags & 0x40) == 0) {
                    g_BattleEndSequenceDelay = 8;
                    for (actorIdx = 0; actorIdx < 3; actorIdx++) {
                        gBattleAiGlobals.combatants[actorIdx].idleOverride = 0xE;
                    }
                }
            } else if ((g_BattleScene.setupFlags & 8) != 0 && (g_BattleScene.flags & 0x100) == 0) {
                for (actorIdx = 0; actorIdx < 3; actorIdx++) {
                    gBattleAiGlobals.combatants[actorIdx].idleOverride = 0xC;
                }
                g_BattleEndSequenceDelay = 0x31;
                g_BattleMenuGraphicsPathBuffer.victoryMusicPending = 1;
            }
        }
        g_BattleEndVictoryDefeatMask = *(unsigned short *)&g_BattleMenuGraphicsPathBuffer.victoryMusicPending | *(unsigned short *)&g_BattleDefeatMusicPending;
        g_BattleEndSequenceState = BATTLE_END_STATE_PLAY_CLOSING;
    case BATTLE_END_STATE_PLAY_CLOSING:
        Battle_PlaySequence();
        if (g_BattleSequencePlayDone != 0) {
            g_BattleSequencePlayDone = 0;
            g_BattleEndPulseCounter++;
            if (g_BattleEndPulseCounter >= g_BattleEndSequenceDelay) {
                g_BattleEndPulseCounter = 0;
                g_BattleEndSequenceState = BATTLE_END_STATE_HOLD_CLOSING;
            }
        }
        g_BattleSequenceBuffer.phasePendingFlag = 1;
        break;
    case BATTLE_END_STATE_HOLD_CLOSING:
        switch (g_BattleEndSubState) {
        case BATTLE_END_SUBSTATE_CHECK:
            if ((unsigned short)g_BattleMenuGraphicsPathBuffer.victoryMusicPending | (unsigned short)g_BattleDefeatMusicPending) {
                g_BattleEndSubState = BATTLE_END_SUBSTATE_PLAY;
            } else {
                g_BattleEndSequenceState = BATTLE_END_STATE_SET_FIELD_FLAGS;
                break;
            }
        case BATTLE_END_SUBSTATE_PLAY:
            Battle_PlaySequence();
            if (g_BattleSequencePlayDone != 0) {
                g_BattleSequencePlayDone = 0;
                g_BattleEndSubState = BATTLE_END_SUBSTATE_CHECK;
            }
            g_BattleSequenceBuffer.phasePendingFlag = 1;
        }
        if (g_BattleSequenceBuffer.phasePendingFlag != 0) {
            break;
        }
    case BATTLE_END_STATE_SET_FIELD_FLAGS:
        if ((gBattleAiGlobals.arenaFlags & BATTLE_ARENA_FIXED_ATB) == 0) {
            Battle_PushSequenceStep(0, 0, 0, 0);
        }
        if ((g_BattleEndOutcomeFlags & BATTLE_OUTCOME_DEFEAT) != 0) {
            *(short *)&g_BattleResultWord |= FIELD_BATTLE_FLAG_LOST;
        } else if ((g_BattleEndOutcomeFlags & (BATTLE_OUTCOME_FLED | BATTLE_OUTCOME_ESCAPED)) != 0) {
            *(short *)&g_BattleResultWord |= FIELD_BATTLE_FLAG_ESCAPED;
        }
        for (actorIdx = 0; actorIdx < 6; actorIdx++) {
            if (gBattleAiGlobals.combatants[actorIdx + 4].unitId != -1 && (g_BattleState.excludedActorMask & 1 << actorIdx + 4) == 0) {
                break;
            }
        }
        if (actorIdx == 6) {
            *(short *)&g_BattleResultWord |= FIELD_BATTLE_FLAG_ESCAPED;
        }
        g_BattleEndSequenceState = BATTLE_END_STATE_RELEASE_DROPS;
    case BATTLE_END_STATE_RELEASE_DROPS:
        Battle_PlaySequence();
        if (g_BattleSequencePlayDone != 0) {
            g_BattleSequencePlayDone = 0;
            Battle_ReleasePendingDrops(0xA);
            for (memberIdx = 0; memberIdx < 3; memberIdx++) {
                Battle_ReleasePendingDrops((short)memberIdx);
            }
            g_BattleEndDropPhaseCounter++;
            if (g_BattleEndDropPhaseCounter >= 0x10) {
                g_BattleEndDropPhaseCounter = 0;
                g_BattleEndSequenceState = BATTLE_END_STATE_REWARDS;
            }
        }
        g_BattleSequenceBuffer.phasePendingFlag = 1;
        break;
    case BATTLE_END_STATE_REWARDS:
        Battle_SetMessageWindowRequest(-1);
        for (actorIdx = 0; actorIdx < 0x140; actorIdx++) {
            pItemRecord = (BattleEndItemRecord *)(actorIdx * 6 + (char *)g_BattleItemListBlock.slots);
            gSavemap.items[actorIdx] = 0xFFFF;
            if (pItemRecord->itemId != 0xFFFF) {
                gSavemap.items[actorIdx] = pItemRecord->itemId & 0x1FF | pItemRecord->quantity << 9 & 0xFE00;
            }
        }
        if ((g_BattleEndOutcomeFlags & 0xDE) == 0 && (gBattleAiGlobals.arenaFlags & 0x20) == 0) {
            g_BattleEndResultScreenFlag = MENU_ENTRY_BATTLE_RESULT;
            for (actorIdx = 0; actorIdx < 6; actorIdx++) {
                enemySlot = g_BattleScene.enemies[actorIdx].enemyIndex;
                if (enemySlot != -1) {
                    enemyPtr = (int)&g_BattleState.enemyRecords[enemySlot];
                    if ((gBattleAiGlobals.combatants[actorIdx + 4].status & (BATTLE_STATUS_DEATH | BATTLE_STATUS_PETRIFY)) != 0 && (g_BattleState.excludedActorMask & 1 << actorIdx + 4) == 0) {
                        g_BattleRewardExp += gBattleAiGlobals.combatants[actorIdx + 4].expReward;
                        g_BattleRewardTotals.gil += gBattleAiGlobals.combatants[actorIdx + 4].gilReward;
                        g_BattleRewardAp += ((BattleEnemyRecord *)enemyPtr)->ap;
                        if ((g_BattleState.actors[actorIdx + 4].timingFlags & 0x11) == 0) {
                            dropId = Battle_RollEnemyDropOrSteal((void *)enemyPtr, 0, 0, 0x100);
                            if (dropId != 0xFFFF) {
                                for (subIdx = 0; subIdx < g_BattleEndDroppedItemCount; subIdx++) {
                                    if (g_BattleRewardItems[subIdx].itemId == dropId) {
                                        (g_BattleRewardItems[subIdx].count)++;
                                        break;
                                    }
                                }
                                if (subIdx == g_BattleEndDroppedItemCount) {
                                    g_BattleRewardItems[subIdx].itemId = dropId;
                                    g_BattleRewardItems[subIdx].count = 1;
                                    g_BattleEndDroppedItemCount++;
                                }
                            }
                        }
                    }
                }
            }
            if (g_PartyBattleModifiers.gilScale != 0) {
                g_BattleRewardTotals.gil = g_BattleRewardTotals.gil * g_PartyBattleModifiers.gilScale >> 4;
            }
            Savemap_ClearNewMateriaQueue();
            for (actorIdx = 0; actorIdx < 3; actorIdx++) {
                for (subIdx = 0; subIdx < 9; subIdx++) {
                    if (gSavemap.party[actorIdx] == gSavemap.chars[subIdx].id) {
                        charExp = 0;
                        charRecordPtr = &gSavemap.chars[subIdx];
                        nextStatBlock = (int)g_MenuPartyStats[actorIdx].statBlock;
                        g_BattleEndReserveCharMask &= ~(1 << gSavemap.chars[subIdx].id);
                        if ((gBattleAiGlobals.combatants[actorIdx].status & (BATTLE_STATUS_DEATH | BATTLE_STATUS_PETRIFY)) == 0) {
                            Battle_AwardApToEquippedMateria(actorIdx, g_BattleRewardAp);
                            charExp = g_BattleRewardExp;
                            if (*(unsigned char *)nextStatBlock != 0) {
                                charExp = charExp * *(unsigned char *)nextStatBlock >> 4;
                            }
                        }
                        Kernel_ApplyExperienceToCharacterRecord(charRecordPtr, charExp, actorIdx);
                        g_BattleRewardTotals.party[actorIdx].limitUnlocks = Kernel_UpdateLimitUnlocks(charRecordPtr, g_aBattlePartyStat[actorIdx].killCount, g_aBattlePartyStat[actorIdx].limitUseCount);
                        typeFlagsDiff = g_aBattlePartyStat[actorIdx].materiaTypeBits ^ g_aBattlePartyStat[actorIdx].loadMateriaTypeBits;
                        if (typeFlagsDiff != 0) {
                            Battle_OrBitsIntoEquippedMateriaType7(charRecordPtr, typeFlagsDiff);
                        }
                    }
                }
            }
            Battle_GrantHalfExpToNonPartyCharacters(g_BattleEndReserveCharMask);
        }
        Battle_WritePartyStateBackToSavemap(g_BattleEndAvgHp, g_BattleEndAvgMp);
        Battle_StopAllSfxAndRestoreVolume();
        g_MenuEntryState = *(short *)&g_BattleEndResultScreenFlag;
        if (g_MenuEntryState == MENU_ENTRY_BATTLE_RESULT) {
            Battle_InitResultScreenState();
        }
        g_BattleEndSequenceState = BATTLE_END_STATE_RESOLVE_OUTCOME;
        g_BattleEndSequenceDone = 1;
    }
}
} /* extern "C" */

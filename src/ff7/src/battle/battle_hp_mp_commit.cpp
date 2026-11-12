// src/ff7/src/battle/battle_hp_mp_commit.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Commits HP and MP changes from battle command results through the clamp helper, adds
//   transfers to the HP/MP pool, and selects the transfer flags for a hit.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"

#include "lib/codegen.h"
extern "C" {

/*
 * For a nonzero command damage, commits either MP or HP through the clamp helper, healing when the
 * heal display flag is set. An HP transition from nonzero to zero sets the death status and
 * optionally tries a morph reward. For a damaging enemy-to-party attack, raises the target's limit
 * gauge by damage * 300 / maxHp, scaled by a multiplier doubled for Fury or halved for Sadness and
 * divided by the target state's divisor, capped at 255. The gauge update uses the commanded damage
 * rather than the actual clamped HP loss. Zero damage changes nothing.
 */
// FUNCTION: FF7 0x005df0f0
void __cdecl Battle_CommitHpMpChange(void)
{
    int gauge;
    int prevValue;
    BattleCombatant *combatantPtr;
    int healing;
    int multiplier;
    int gaugeGain;

    combatantPtr = &g_aCombatants[g_pBattleCommandCtx->targetSlot];
    healing = g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_HEAL;
    if (g_pBattleCommandCtx->damage != 0) {
        if ((g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_MP) != 0) {
            prevValue = combatantPtr->curMp;
            combatantPtr->curMp = Battle_ClampHpMpDelta(prevValue, g_pBattleCommandCtx->damage, combatantPtr->maxMp, healing);
        } else {
            prevValue = combatantPtr->curHp;
            combatantPtr->curHp = Battle_ClampHpMpDelta(prevValue, g_pBattleCommandCtx->damage, combatantPtr->maxHp, healing);
            if (combatantPtr->curHp == 0 && prevValue != 0) {
                combatantPtr->status |= 1;
                if ((g_pBattleCommandCtx->attackFlags & 0x2000) != 0) {
                    Battle_TryMorphReward();
                }
            }
            if (g_pBattleCommandCtx->targetSlot < 3 && g_pBattleCommandCtx->attackerSlot >= 4 && healing == 0 && ((BattleCommandTargetState *)(g_pBattleCommandCtx->targetStatePtr))->limitLevelIndex < 4 && (g_BattleState.actors[g_pBattleCommandCtx->targetSlot].timingFlags & 8) == 0) {
                gauge = ((BattleCommandTargetState *)(g_pBattleCommandCtx->targetStatePtr))->limitGauge;
                multiplier = 0x100;
                gaugeGain = g_pBattleCommandCtx->damage * 0x12C / (unsigned int)combatantPtr->maxHp;
                if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_FURY) != 0) {
                    multiplier <<= 1;
                } else if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_SADNESS) != 0) {
                    multiplier >>= 1;
                }
                gauge += gaugeGain * multiplier / ((BattleCommandTargetState *)(g_pBattleCommandCtx->targetStatePtr))->divisor;
                if (gauge > 0xFF) {
                    gauge = 0xFF;
                }
                ((BattleCommandTargetState *)(g_pBattleCommandCtx->targetStatePtr))->limitGauge = gauge;
            }
        }
    }
}

/*
 * Flag bit one: caps amount by the command context's cap when targetId is the target slot, negates it
 * for healing and subtracts it from the actor's pendingMpAmount. Flag bit two then reuses the possibly
 * changed amount, caps it by the context's hp, may negate again and subtracts it from pendingHpAmount.
 * Both bits together differ from two independent transfers.
 */
// FUNCTION: FF7 0x005df30b
void __cdecl Battle_AddHpMpTransferToPool(int actorIndex, int targetId, int amount, unsigned int flags)
{
    int negate;
    int cap;
    int localLimit;

    negate = (g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_HEAL) != 0;
    if ((flags & 1) != 0) {
        if (targetId == g_pBattleCommandCtx->targetSlot) {
            cap = g_pBattleCommandCtx->cap;
            if (cap < amount) {
                amount = cap;
            }
        }
        if (negate != 0) {
            amount = -amount;
        }
        g_BattleState.actors[actorIndex].pendingMpAmount -= amount;
    }
    if ((flags & 2) != 0) {
        if (targetId == g_pBattleCommandCtx->targetSlot) {
            localLimit = g_pBattleCommandCtx->hp;
            if (localLimit < amount) {
                amount = localLimit;
            }
        }
        if (negate != 0) {
            amount = -amount;
        }
        g_BattleState.actors[actorIndex].pendingHpAmount -= amount;
    }
}

/*
 * Selects transfer flags from the command context: the MP display flag chooses 1 (MP) or 2 (HP), and
 * the not-drain special flag chooses 0 or 3. Passes the actor index, target slot and damage with the
 * OR of those flags to Battle_AddHpMpTransferToPool.
 */
// FUNCTION: FF7 0x005df3ed
void __cdecl Battle_QueueHitHpMpTransfer(void)
{
    int mpFlag;
    int drainFlag;
    mpFlag = (g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_MP) != 0 ? 1 : 2;
    drainFlag = (g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_DRAIN_HP_AND_MP) != 0 ? 0 : 3;
    Battle_AddHpMpTransferToPool(g_pBattleCommandCtx->actorIndex,
                                 g_pBattleCommandCtx->targetSlot,
                                 g_pBattleCommandCtx->damage, mpFlag | drainFlag);
}
} /* extern "C" */

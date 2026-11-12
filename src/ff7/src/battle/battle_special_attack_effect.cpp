// src/ff7/src/battle/battle_special_attack_effect.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Dispatches battle special attack effects, applying each one only when the selected
//   effect's stage mask allows it.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
extern "C" {

// GLOBAL: FF7 0x007b7748
extern const unsigned short g_BattleSpecialAttackMaskTable[36] = {0x4, 0x3c, 0x4, 0x20, 0x1, 0x24, 0x10, 0x10, 0x4, 0x2, 0x2, 0x2, 0x2, 0x1, 0x20, 0x4, 0x24, 0x10, 0x10, 0x4, 0x20, 0x10, 0x10, 0x10, 0x30, 0x10, 0x20, 0x10, 0x10, 0x14, 0x1, 0x1, 0x1, 0x1, 0x1, 0x18};

#define g_aBattlePartyStat (g_BattleState.partyStats)

/*
 * Dispatches special attack effects only when the selected effect's stage mask admits effectId
 * (the BattleAddedEffectStage being run); returns zero in every path. Cases alter damage/status
 * scaling, HP/MP and combat statistics, transfer gil, capture or mark actors, merge party state and
 * queue reactions or follow-up attacks. Several effects split work across the stages from
 * BATTLE_ADDED_FX_STAGE_SET to BATTLE_ADDED_FX_STAGE_AFTER_HITS; learned-limit follow-ups
 * inspect up to seven slots.
 */
// FUNCTION: FF7 0x005dc880
int __cdecl Battle_RunSpecialAttackEffect(int effectId)
{
    int idx;
    int actorIdx;
    BattlePartyStatBlock *statBlock;
    unsigned short transferAmount;
    int aliveCount;
    int targetPtr;
    int limitBase;
    int stepLearnedMask;
    int markBit;
    int newMask;
    int allowedBits;
    int result;
    int bitIdx;
    int flag;
    int total;
    int numberMemb;
    int targetBit;
    int flagBit;
    int charId;
    int targetFlags;
    int attackId;
    int tier;
    int enableFlags;

    result = 0;
    if (g_pBattleCommandCtx->effectId == BATTLE_ADDED_FX_NONE) {
        return result;
    } else {
        if ((g_BattleSpecialAttackMaskTable[g_pBattleCommandCtx->effectId] & 1 << effectId) != 0) {
            actorIdx = g_pBattleCommandCtx->attackerSlot;
            statBlock = (BattlePartyStatBlock *)&g_aBattlePartyStat[actorIdx];
            switch (g_pBattleCommandCtx->effectId) {
            case BATTLE_ADDED_FX_MULTI_HIT_RANDOM_TARGET:
                g_pBattleCommandCtx->countdown = g_pBattleCommandCtx->effectParam;
                break;
            case BATTLE_ADDED_FX_ODIN_DEATH_ELSE_GUNGE_LANCE:
                switch (effectId) {
                case BATTLE_ADDED_FX_STAGE_SET:
                    g_pBattleCommandCtx->attackFlags |= 0x40000;
                    break;
                case BATTLE_ADDED_FX_STAGE_AFTER_DAMAGE:
                    if ((g_pBattleCommandCtx->elemReaction & 0x80) != 0) {
                        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
                    }
                    break;
                case BATTLE_ADDED_FX_STAGE_TARGET_HIT:
                    if (g_pBattleCommandCtx->targetSlot >= 4) {
                        g_BattleState.deathClearedMask |= (short)(1 << g_pBattleCommandCtx->targetSlot);
                    }
                    break;
                case BATTLE_ADDED_FX_STAGE_AFTER_HITS:
                    if (g_pBattleCommandCtx->hitCount == 0) {
                        (g_pBattleCommandCtx->repeatRemaining)++;
                        g_pBattleCommandCtx->stepPower = 97;
                        Battle_RestoreSequenceCursors();
                        Battle_QueueFollowUpAttack(g_pBattleCommandCtx->stepPower);
                    }
                }
                break;
            case BATTLE_ADDED_FX_CHANCE_FAT_CHOCOBO:
                if (Battle_NextRandomByte() < g_pBattleCommandCtx->effectParam) {
                    g_pBattleCommandCtx->attackId = 0x60;
                    g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->attackId;
                    Battle_ReapplyAttackRecord();
                }
                break;
            case BATTLE_ADDED_FX_TRANSFORM_USER:
                Battle_ApplyActorStatMutation();
                break;
            case BATTLE_ADDED_FX_BACK_ATTACK_IF_SIDE_MATCHES:
                flag = (g_aCombatants[g_pBattleCommandCtx->targetSlot].flags & BATTLE_COMBATANT_FACING_FLIPPED) != 0;
                flag ^= g_pBattleCommandCtx->effectParam;
                g_pBattleCommandCtx->modifierFlags &= 0xFFFFFFFE;
                g_pBattleCommandCtx->displayFlags &= ~BATTLE_DISPLAY_CRITICAL;
                if (flag == 0) {
                    g_pBattleCommandCtx->modifierFlags |= 1;
                }
                break;
            case BATTLE_ADDED_FX_ESCAPE:
                switch (effectId) {
                case BATTLE_ADDED_FX_STAGE_SET:
                    if ((g_BattleScene.setupFlags & 4) == 0) {
                        g_pBattleCommandCtx->messageId = 0x36;
                    }
                    break;
                case BATTLE_ADDED_FX_STAGE_AFTER_HITS:
                    *(short *)&gBattleAiGlobals.flags |= BATTLE_OUTCOME_ESCAPED;
                }
                break;
            case BATTLE_ADDED_FX_STEAL_GIL:
                if (g_pBattleCommandCtx->targetSlot < 3) {
                    transferAmount = g_pBattleCommandCtx->attackerLevel * 20;
                    if ((unsigned int)transferAmount > (unsigned int)gSavemap.gil) {
                        transferAmount = *(short *)&gSavemap.gil;
                    }
                    if (transferAmount != 0 && transferAmount + (unsigned short)g_aCombatants[actorIdx].spoilA < 65535) {
                        g_aCombatants[actorIdx].spoilA += transferAmount;
                        gSavemap.gil -= transferAmount;
                        Battle_PostBattleMessageFormatted(actorIdx, 0x55, 1, &transferAmount);
                    }
                }
                if (g_pBattleCommandCtx->power == 0) {
                    g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_NO_DAMAGE_DISPLAY;
                    g_pBattleCommandCtx->animId = g_aCombatants[g_pBattleCommandCtx->targetSlot].animId;
                }
                break;
            case BATTLE_ADDED_FX_STEAL_ITEM:
                if ((unsigned short)g_aCombatants[actorIdx].spoilB == 0xFFFF && g_pBattleCommandCtx->targetSlot < 3) {
                    Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, actorIdx, BATTLE_ATB_EVENT_CONSUME_RANDOM_ITEM, 0);
                }
                if (g_pBattleCommandCtx->power == 0) {
                    g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_NO_DAMAGE_DISPLAY;
                    g_pBattleCommandCtx->animId = g_aCombatants[g_pBattleCommandCtx->targetSlot].animId;
                }
                break;
            case BATTLE_ADDED_FX_TOY_BOX_RANDOM_EFFECT:
                /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
                g_pBattleCommandCtx->power = ((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(7);
                g_pBattleCommandCtx->resolvedAttackId += g_pBattleCommandCtx->power;
                g_pBattleCommandCtx->power += 4;
                g_pBattleCommandCtx->power <<= 3;
                break;
            case BATTLE_ADDED_FX_DAMAGE_X8_IF_SAME_LEVEL:
                if (g_pBattleCommandCtx->attackerLevel == g_pBattleCommandCtx->targetLevel) {
                    g_pBattleCommandCtx->damage <<= 3;
                }
                break;
            case BATTLE_ADDED_FX_DAMAGE_BY_STATUS_COUNT:
                Battle_SpecialEffect0A_ScaleDamageByStatusCount();
                break;
            case BATTLE_ADDED_FX_DAMAGE_UP_NEAR_DEATH_SENTENCE:
                Battle_SpecialEffect0B_ScaleDamageByBuffCount();
                break;
            case BATTLE_ADDED_FX_DAMAGE_BY_KO_ALLY_COUNT:
                Battle_SpecialEffect0C_ScaleDamageByKoCount();
                break;
            case BATTLE_ADDED_FX_POWER_FROM_TARGET_AVG_LEVEL:
                if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                    total = 0;
                    numberMemb = 0;
                    for (targetPtr = 4; targetPtr < 0xA; targetPtr++) {
                        if ((g_pBattleCommandCtx->targetMask & 1 << targetPtr) != 0) {
                            total += g_aCombatants[targetPtr].level;
                            numberMemb++;
                        }
                    }
                    if (numberMemb != 0) {
                        total = total / numberMemb;
                    }
                    g_pBattleCommandCtx->power = total;
                }
                break;
            case BATTLE_ADDED_FX_REVIVE_PARTY_FULL_HP:
                for (idx = 0; idx < 3; idx++) {
                    if ((g_aCombatants[idx].status & BATTLE_STATUS_DEATH) != 0) {
                        g_aCombatants[idx].status &= ~BATTLE_STATUS_DEATH;
                        g_aCombatants[idx].curHp = g_aCombatants[idx].maxHp;
                    }
                }
                break;
            case BATTLE_ADDED_FX_SLOTS_PICK_ATTACK:
                Battle_SpecialEffect0F_PickSlotsAttack();
                break;
            case BATTLE_ADDED_FX_MERGE_PARTY_INTO_USER:
                switch (effectId) {
                case BATTLE_ADDED_FX_STAGE_SET:
                    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                        aliveCount = 0;
                        for (idx = 0; idx < 3; idx++) {
                            if (g_aCombatants[idx].unitId != -1) {
                                aliveCount++;
                            }
                        }
                        if (aliveCount < 2) {
                            g_pBattleCommandCtx->messageId = 0x50;
                        }
                    }
                    break;
                case BATTLE_ADDED_FX_STAGE_AFTER_HITS:
                    Battle_MergePartyStatsIntoActor();
                }
                break;
            case BATTLE_ADDED_FX_REMOVE_PARTY_TARGET:
                if (g_pBattleCommandCtx->targetSlot < 3) {
                    flagBit = 1 << g_pBattleCommandCtx->targetSlot;
                    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, g_pBattleCommandCtx->targetSlot, BATTLE_ACTION_EVENT_REMOVE_PARTY_ACTORS, flagBit);
                }
                g_pBattleCommandCtx->animId = Battle_GetDefaultHitReactionPose();
                break;
            case BATTLE_ADDED_FX_REMOVE_AND_EXCLUDE_TARGET:
                if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                    targetBit = 1 << g_pBattleCommandCtx->targetSlot;
                    if (g_pBattleCommandCtx->targetSlot < 3) {
                        Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, g_pBattleCommandCtx->targetSlot, BATTLE_ACTION_EVENT_REMOVE_PARTY_ACTORS, targetBit);
                    }
                    g_BattleState.excludedActorMask |= targetBit;
                    g_pBattleCommandCtx->animId = Battle_GetDefaultHitReactionPose();
                }
                break;
            case BATTLE_ADDED_FX_TIFA_LIMIT_COMBO:
                if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                    g_pBattleCommandCtx->targetMask = 0;
                    g_pBattleCommandCtx->repeatRemaining = 0;
                    charId = g_BattleScene.actors[actorIdx].unitIndex;
                    stepLearnedMask = g_aBattlePartyStat[actorIdx].partyMember->limits;
                    limitBase = Kernel_GetLimitAttackId(charId, 0);
                    for (idx = 0; idx < 7; idx++) {
                        tier = g_BattleScene.turnQueueSlots[idx];
                        attackId = 0x7F;
                        for (bitIdx = 0; bitIdx < 0xC; bitIdx++) {
                            if ((stepLearnedMask & 1 << bitIdx) != 0) {
                                attackId = Kernel_GetLimitAttackId(charId, bitIdx);
                                stepLearnedMask &= ~(1 << bitIdx);
                                break;
                            }
                        }
                        if (tier != 0 && attackId - limitBase < 7) {
                            attackId -= limitBase;
                            attackId += 0x62;
                            if (tier > 1) {
                                attackId |= 0x80;
                            }
                            Battle_QueueFollowUpAttack(attackId);
                            (g_pBattleCommandCtx->repeatRemaining)++;
                        }
                    }
                    g_pBattleCommandCtx->attackFlags |= 0x10000;
                }
                break;
            case BATTLE_ADDED_FX_FILL_ALLY_LIMIT_BARS:
                for (idx = 0; idx < 3; idx++) {
                    if (actorIdx != idx) {
                        g_aBattlePartyStat[idx].limitGauge = 0xFF;
                    }
                }
                break;
            case BATTLE_ADDED_FX_TARGET_ATK_MATK_DEF_MDEF_PCT:
                Battle_AddStatPercentModifiers(g_pBattleCommandCtx->targetSlot, g_pBattleCommandCtx->effectParam - 0x64, 0xF);
                break;
            case BATTLE_ADDED_FX_USER_DEFENSE_PCT:
                Battle_AddStatPercentModifiers(actorIdx, g_pBattleCommandCtx->effectParam - 0x64, 0x10);
                break;
            case BATTLE_ADDED_FX_USER_ATTACK_PCT:
                Battle_AddStatPercentModifiers(actorIdx, g_pBattleCommandCtx->effectParam - 0x64, 1);
                break;
            case BATTLE_ADDED_FX_CHAIN_ATTACK_SAME_TARGETS:
                switch (effectId) {
                case BATTLE_ADDED_FX_STAGE_TARGET_HIT:
                    if (g_pBattleCommandCtx->targetSlot >= 4) {
                        g_BattleState.deathClearedMask |= (short)(1 << g_pBattleCommandCtx->targetSlot);
                    }
                    break;
                case BATTLE_ADDED_FX_STAGE_AFTER_HITS:
                    (g_pBattleCommandCtx->repeatRemaining)++;
                    g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->effectParam;
                    g_pBattleCommandCtx->mask = g_pBattleCommandCtx->targetMask;
                    Battle_QueueFollowUpAttack(g_pBattleCommandCtx->stepPower);
                    g_pBattleCommandCtx->attackFlags |= 0x81000;
                }
                break;
            case BATTLE_ADDED_FX_TOGGLE_TARGET_ROW:
                if (Battle_IsPartyFirstLayout() != 0 && g_pBattleCommandCtx->targetSlot < 3) {
                    g_pBattleCommandCtx->animId = ((g_aCombatants[g_pBattleCommandCtx->targetSlot].flags & BATTLE_COMBATANT_BACK_ROW) != 0) + 0x31;
                    g_aCombatants[g_pBattleCommandCtx->targetSlot].flags ^= BATTLE_COMBATANT_BACK_ROW;
                }
                break;
            case BATTLE_ADDED_FX_CHAIN_ATTACK_OTHER_TARGETS:
                if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                    targetFlags = g_pBattleCommandCtx->targetMask;
                    allowedBits = ((targetFlags & BATTLE_ACTOR_MASK_ALLIES) != 0 ? 0xFFFFFC1F : 0) + BATTLE_ACTOR_MASK_ENEMIES;
                    newMask = g_BattleScene.targetMaskBits & ~(g_BattleScene.incapacitatedMask);
                    newMask &= allowedBits;
                    newMask &= ~targetFlags;
                    g_pBattleCommandCtx->mask = newMask;
                    (g_pBattleCommandCtx->repeatRemaining)++;
                    g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->effectParam;
                    Battle_QueueFollowUpAttack(g_pBattleCommandCtx->stepPower);
                    g_pBattleCommandCtx->attackFlags |= 0x281000;
                }
                break;
            case BATTLE_ADDED_FX_EXCLUDE_ENEMY_TARGET:
                if (g_pBattleCommandCtx->targetSlot >= 4) {
                    markBit = 1 << g_pBattleCommandCtx->targetSlot;
                    g_BattleState.excludedActorMask |= markBit;
                }
                break;
            case BATTLE_ADDED_FX_TARGET_DEF_MDEF_PCT:
                Battle_AddStatPercentModifiers(g_pBattleCommandCtx->targetSlot, g_pBattleCommandCtx->effectParam - 0x64, 0xC);
                break;
            case BATTLE_ADDED_FX_TARGET_ANIM_33:
                switch (effectId) {
                case BATTLE_ADDED_FX_STAGE_SET:
                    g_pBattleCommandCtx->attackFlags |= 0x1000000;
                    break;
                case BATTLE_ADDED_FX_STAGE_TARGET_HIT:
                    g_pBattleCommandCtx->animId = 0x33;
                }
                break;
            case BATTLE_ADDED_FX_POWER_FROM_USER_HP:
                g_pBattleCommandCtx->power = (unsigned int)(g_pBattleCommandCtx->basePower * 3 * g_BattleState.actors[actorIdx].hp) / (unsigned int)g_aCombatants[actorIdx].maxHp + 1;
                break;
            case BATTLE_ADDED_FX_POWER_FROM_USER_MP:
                g_pBattleCommandCtx->power = g_pBattleCommandCtx->basePower * 3 * (unsigned short)g_BattleState.actors[actorIdx].mp / g_aCombatants[actorIdx].maxMp + 1;
                break;
            case BATTLE_ADDED_FX_POWER_FROM_MATERIA_AP:
                Battle_SpecialEffect20_PowerFromMateriaAp();
                break;
            case BATTLE_ADDED_FX_POWER_FROM_KILL_COUNT:
                g_pBattleCommandCtx->power = ((statBlock->partyMember->killCount / 128) * g_pBattleCommandCtx->basePower) / 16 + 10;
                break;
            case BATTLE_ADDED_FX_POWER_FROM_LIMIT_BAR:
                g_pBattleCommandCtx->power = ((statBlock->statValue * (statBlock->scaleStep + 1) >> 4) * g_pBattleCommandCtx->basePower) / 16 + 1;
                break;
            case BATTLE_ADDED_FX_REMOVE_ENEMY_TARGET:
                switch (effectId) {
                case BATTLE_ADDED_FX_STAGE_AFTER_DAMAGE:
                    if ((g_pBattleCommandCtx->elemReaction & 0x80) != 0) {
                        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
                    }
                    break;
                case BATTLE_ADDED_FX_STAGE_TARGET_HIT:
                    if (g_pBattleCommandCtx->targetSlot >= 4) {
                        enableFlags = 1 << g_pBattleCommandCtx->targetSlot;
                        g_BattleState.deathClearedMask |= enableFlags;
                        g_BattleState.actors[g_pBattleCommandCtx->targetSlot].timingFlags |= 1;
                        g_aCombatants[g_pBattleCommandCtx->targetSlot].gilReward = 0;
                    }
                }
            }
        }
        return result;
    }
}
} /* extern "C" */

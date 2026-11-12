// src/ff7/src/battle/battle_special_effect_scaling.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Special battle effects that scale damage by status, buff and KO counts, or power it
//   from equipped materia AP, plus the stat merge and status mutation these effects use.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
#include "ff7/types/menu.h"
extern "C" {

// GLOBAL: FF7 0x007b77c8
extern const signed char g_BattleStatMutationPercentTable[8] = {100, 20, 20, 20, -20, -50, -50, 0};

// clang-format off
// GLOBAL: FF7 0x008ff278
struct BattleSlotsCombinationEntry g_BattleSlotsCombinationTable[7] = {{1, 1, 1}, {0}, {0, 0, 1}, {2, 2, 2}, {3, 3, 3}, {4, 4, 4}, {5, 5, 5}};
// clang-format on

#define g_aBattlePartyStat (g_BattleState.partyStats)

/*
 * Searches seven three-byte slot patterns. Any index other than zero, including no match index
 * seven, selects index plus 0x68. Match zero instead uses four random rolls and counter advances
 * to derive clamped attack 0x38..0x47 and command three. Reapplies attack while preserving field
 * 0x20, sets target 0xff and clears field 0x38.
 */
// FUNCTION: FF7 0x005df460
void __cdecl Battle_SpecialEffect0F_PickSlotsAttack(void)
{
    int savedStepId;
    int patternIdx;
    int rollTotal;

    for (patternIdx = 0; patternIdx < 7; patternIdx++) {
        if (g_BattleScene.squareSlots[0] == g_BattleSlotsCombinationTable[patternIdx].requiredSlot0 && g_BattleScene.squareSlots[1] == g_BattleSlotsCombinationTable[patternIdx].requiredSlot1 && g_BattleScene.squareSlots[2] == g_BattleSlotsCombinationTable[patternIdx].requiredSlot2) {
            break;
        }
    }
    if (patternIdx != 0) {
        g_pBattleCommandCtx->attackId = patternIdx + 0x68;
    } else {
        rollTotal = 4;
        for (patternIdx = 0; patternIdx < 4; patternIdx++) {
            /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
            rollTotal += ((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(0xA);
            Battle_AdvanceActionSequenceCounter();
        }
        rollTotal += g_pBattleCommandCtx->attackerLevel / 21;
        rollTotal /= 2;
        rollTotal -= 4;
        if (rollTotal < 0) {
            rollTotal = 0;
        }
        if (rollTotal > 0xF) {
            rollTotal = 0xF;
        }
        g_pBattleCommandCtx->attackId = rollTotal + 0x38;
        g_pBattleCommandCtx->stepFlags = 3;
    }
    g_pBattleCommandCtx->targetFlags = 0xFF;
    g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->attackId;
    savedStepId = g_pBattleCommandCtx->stepId;
    Battle_ReapplyAttackRecord();
    g_pBattleCommandCtx->stepId = savedStepId;
    g_pBattleCommandCtx->mpCost = 0;
}

/*
 * Merges the three present party combatants into the current attacker: accumulates eight statistics
 * and current/maximum HP/MP, posts event 0x13 for each other present member, and sets their bits in
 * the global party mask. Divides each statistic by the present count, applies a signed percentage
 * table bonus, and caps it above at 255; sums of HP/MP and their maxima are independently capped by
 * the attacker party-record limits. Writes the resulting statistics and totals to the attacker,
 * posts event 0x18, and sets party-record byte 5 to 0xFF. No present combatants causes division by
 * zero and negative bonuses have no lower clamp.
 */
// FUNCTION: FF7 0x005df5d7
void __cdecl Battle_MergePartyStatsIntoActor(void)
{
    /* The local names fix VC5's frame layout; do not rename them. */
    int maxhp;
    int maxMp;
    int stats[8];
    int hpsum;
    int i;
    int sumMp;
    int mplim;
    int num;
    int hpmax;
    int actor;

    num = 0;
    actor = g_pBattleCommandCtx->attackerSlot;
    hpsum = 0;
    sumMp = 0;
    hpmax = 0;
    maxMp = 0;
    maxhp = (unsigned short)g_aBattlePartyStat[actor].hpCap;
    mplim = (unsigned short)g_aBattlePartyStat[actor].mpCap;
    for (i = 0; i < 8; i++) {
        stats[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        if (g_aCombatants[i].unitId != -1) {
            stats[0] += g_aCombatants[i].attack;
            stats[1] += g_aCombatants[i].magicAttack;
            stats[2] += g_aCombatants[i].defense;
            stats[3] += g_aCombatants[i].magicDefense;
            stats[4] += g_aCombatants[i].defensePercent;
            stats[5] += g_aCombatants[i].magicDefensePercent;
            stats[6] += g_aCombatants[i].dexterity;
            stats[7] += g_aCombatants[i].luck;
            hpsum += g_aCombatants[i].curHp;
            sumMp += g_aCombatants[i].curMp;
            hpmax += g_aCombatants[i].maxHp;
            maxMp += g_aCombatants[i].maxMp;
            if (actor != i) {
                Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, i, BATTLE_ACTION_EVENT_REMOVE_PARTY_ACTORS, 1 << i);
            }
            g_BattleState.participatingPartyMask |= (short)(1 << i);
            num++;
        }
    }
    for (i = 0; i < 8; i++) {
        stats[i] = stats[i] / num;
        stats[i] += stats[i] * g_BattleStatMutationPercentTable[i] / 100;
        if (stats[i] > 255) {
            stats[i] = 0xFF;
        }
    }
    if (hpsum > maxhp) {
        hpsum = maxhp;
    }
    if (hpmax > maxhp) {
        hpmax = maxhp;
    }
    if (sumMp > mplim) {
        sumMp = mplim;
    }
    if (maxMp > mplim) {
        maxMp = mplim;
    }
    g_aCombatants[actor].attack = (unsigned char)stats[0];
    g_aCombatants[actor].magicAttack = (unsigned char)stats[1];
    g_aCombatants[actor].defense = (short)stats[2];
    g_aCombatants[actor].magicDefense = (short)stats[3];
    g_aCombatants[actor].defensePercent = (unsigned char)stats[4];
    g_aCombatants[actor].magicDefensePercent = (unsigned char)stats[5];
    g_aCombatants[actor].dexterity = (unsigned char)stats[6];
    g_aCombatants[actor].luck = (unsigned char)stats[7];
    g_aCombatants[actor].curHp = hpsum;
    g_aCombatants[actor].curMp = (short)sumMp;
    g_aCombatants[actor].maxHp = hpmax;
    g_aCombatants[actor].maxMp = (short)maxMp;
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, actor, BATTLE_ACTION_EVENT_COPY_PARTY_STAT_WORDS, 1);
    g_aBattlePartyStat[actor].limitLevelIndex = 0xFF;
}

/*
 * Marks actor mutation flags, clears selected statuses and enqueues direct action, then applies
 * mode-specific percent modifiers and maximum HP multiplication. Upper-clamps unsigned max HP to
 * party cap, fills current HP to maximum, refreshes tick rates and posts event. Unknown mode still
 * flags/refills.
 */
// FUNCTION: FF7 0x005df95a
void __cdecl Battle_ApplyActorStatMutation(void)
{
    int val;

    val = g_pBattleCommandCtx->attackerSlot;
    g_aCombatants[val].flags |= BATTLE_COMBATANT_AI_CONTROLLED;
    g_aCombatants[val].status &= ~(BATTLE_STATUS_CONFUSION | BATTLE_STATUS_FROG | BATTLE_STATUS_BERSERK);
    g_BattleState.actors[val].timingFlags |= 8;
    Battle_EnqueueActionDirect(val, 0, -1, 0, 0);
    gBattleAiGlobals.effectParam = g_pBattleCommandCtx->effectParam;
    switch (g_pBattleCommandCtx->effectParam) {
    case BATTLE_LIMIT_TRANSFORM_GALIAN_GALIAN_BEAST:
        Battle_AddStatPercentModifiers(val, 0x14, 0x10);
        Battle_AddStatPercentModifiers(val, 0x32, 0x20);
        g_aCombatants[val].maxHp = (unsigned int)(g_aCombatants[val].maxHp * 13) / 10;
        break;
    case BATTLE_LIMIT_TRANSFORM_DEATH_GIGAS:
        Battle_AddStatPercentModifiers(val, 0x32, 4);
        Battle_AddStatPercentModifiers(val, -0x46, 8);
        Battle_AddStatPercentModifiers(val, -0x14, 0x20);
        g_aCombatants[val].maxHp <<= 1;
        break;
    case BATTLE_LIMIT_TRANSFORM_HELLMASKER:
        Battle_AddStatPercentModifiers(val, 0x32, 4);
        Battle_AddStatPercentModifiers(val, 0x32, 0x10);
        break;
    case BATTLE_LIMIT_TRANSFORM_CHAOS:
        Battle_AddStatPercentModifiers(val, 0x64, 4);
        Battle_AddStatPercentModifiers(val, 0x64, 8);
    }
    if ((unsigned int)g_aCombatants[val].maxHp > (unsigned int)(unsigned short)g_aBattlePartyStat[val].hpCap) {
        g_aCombatants[val].maxHp = (unsigned int)(unsigned short)g_aBattlePartyStat[val].hpCap;
    }
    g_aCombatants[val].curHp = g_aCombatants[val].maxHp;
    Battle_RefreshActorTickRates(val);
    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, val, BATTLE_ACTION_EVENT_COPY_PARTY_STAT_WORDS, 1);
}

/*
 * Adds near-death bit two to attacker status, counts mask 0x0400029A with unit weight and
 * 0x00202000 with double weight, multiplies damage by sum plus one.
 */
// FUNCTION: FF7 0x005dfb5c
void __cdecl Battle_SpecialEffect0A_ScaleDamageByStatusCount(void)
{
    /* The local names fix VC5's frame layout; do not rename them. */
    int sum;
    int bits;
    unsigned int status;

    status = g_pBattleCommandCtx->attackerStatus;
    if (Battle_IsActorNearDeath(g_pBattleCommandCtx->attackerSlot) != 0) {
        status |= 2;
    }
    bits = Battle_PopCountMask(status & 0x0400029A);
    sum = bits;
    bits = Battle_PopCountMask(status & 0x00202000);
    sum = sum + bits * 2;
    g_pBattleCommandCtx->damage = g_pBattleCommandCtx->damage * (sum + 1);
}

/*
 * Multiplies damage by two for near-death attacker and by four for attackerStatus bit 0x200000,
 * giving factors one, two, four, eight; status gameplay meaning not inferred.
 */
// FUNCTION: FF7 0x005dfbee
void __cdecl Battle_SpecialEffect0B_ScaleDamageByBuffCount(void)
{
    int mult;

    mult = 1;
    if (Battle_IsActorNearDeath(g_pBattleCommandCtx->attackerSlot) != 0) {
        mult *= 2;
    }
    if (g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_DEATH_SENTENCE) {
        mult *= 4;
    }
    g_pBattleCommandCtx->damage = g_pBattleCommandCtx->damage * mult;
}

/*
 * Sums AP of eight equipped nonempty materia, each upper-capped by LAST non 0xffff of four
 * thresholds times 100, then power=(sum/10000*command 0xC4)/16+one. Integer divisions occur before
 * multiplication.
 */
// FUNCTION: FF7 0x005dfc52
void __cdecl Battle_SpecialEffect20_PowerFromMateriaAp(void)
{
    int slotIndex;
    int sum;
    int materiaRaw;
    int tierIndex;
    SavemapCharacter *member;
    int ap;
    int limit;
    int typeId;

    sum = 0;
    member = g_aBattlePartyStat[g_pBattleCommandCtx->attackerSlot].partyMember;
    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        materiaRaw = member->weaponMateria[slotIndex];
        if (materiaRaw != -1) {
            typeId = materiaRaw & 0xFF;
            ap = (unsigned int)materiaRaw >> 8;
            limit = 0;
            for (tierIndex = 0; tierIndex < 4; tierIndex++) {
                if (g_KernelMateriaDataArray[typeId].apLevels[tierIndex] != 0xFFFF) {
                    limit = g_KernelMateriaDataArray[typeId].apLevels[tierIndex] * 100;
                }
            }
            if (ap > limit) {
                ap = limit;
            }
            sum += ap;
        }
    }
    g_pBattleCommandCtx->power = ((sum / 10000 * g_pBattleCommandCtx->basePower) / 16) + 1;
}

/*
 * Multiplies command damage by one plus number of first three combatants whose status bit one is
 * set. Factor one..four.
 */
// FUNCTION: FF7 0x005dfd5b
void __cdecl Battle_SpecialEffect0C_ScaleDamageByKoCount(void)
{
    /* The local names fix VC5's frame layout; do not rename them. */
    int i;
    int mul;

    mul = 1;
    for (i = 0; i < 3; i++) {
        if (g_aCombatants[i].status & BATTLE_STATUS_DEATH) {
            mul++;
        }
    }
    g_pBattleCommandCtx->damage = g_pBattleCommandCtx->damage * mul;
}
} /* extern "C" */

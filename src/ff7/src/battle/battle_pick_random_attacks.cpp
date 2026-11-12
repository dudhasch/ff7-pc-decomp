// src/ff7/src/battle/battle_pick_random_attacks.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Chooses random attack identifiers for fixed, magic and summon attacks, and rolls
//   reaction triggers and resolves reaction attack descriptors for party actors.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/menu.h"
#include "ff7/enums/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)
// clang-format off
// Fixed attack ids that Battle_PickRandomFixedAttackId draws from (Battle_RandBelow(7) selects index 0..6;
// the last byte is a zero).
// GLOBAL: FF7 0x007b7688
extern const unsigned char g_BattleRandomFixedAttackIds[8] = {5, 6, 8, 9, 10, 11, 12, 0};
// The pick functions for the reaction attack ids 0xFD..0xFF, called as
// g_BattleReactionAttackPickFuncs[id - 0xFD]. The call operand lies before the table.
typedef int (__cdecl *BattleReactionAttackPickFunc)(void);
// GLOBAL: FF7 0x007b7690
extern const BattleReactionAttackPickFunc g_BattleReactionAttackPickFuncs[3] = {Battle_PickRandomFixedAttackId, Battle_PickRandomMagicAttackId,
                                              Battle_PickRandomSummonAttackId};
// Descriptor kind of reaction slot r (r < 3): 2 is an attack id, 1 is none.
// GLOBAL: FF7 0x007b769c
extern const unsigned char g_BattleReactionSlotDescriptorKinds[4] = {2, 0xff, 1, 0};
// clang-format on


/*
 * Calls Battle_RandBelow(7), masks its result to eight bits, and returns the unsigned byte at that
 * index in g_BattleRandomFixedAttackIds.
 */
// FUNCTION: FF7 0x005c8c90
int __cdecl Battle_PickRandomFixedAttackId(void)
{
    int randIndex;
    int attackId;

    randIndex = Battle_RandBelow(7) & 0xFF;
    attackId = g_BattleRandomFixedAttackIds[randIndex];
    return attackId;
}

/*
 * Chooses one of the 54 magic attack identifiers using the battle random source.
 */
// FUNCTION: FF7 0x005c8cbd
int __cdecl Battle_PickRandomMagicAttackId(void)
{
    int val;

    val = Battle_RandBelow(0x36) & 0xFF;
    return val;
}

/*
 * Chooses one of the 16 summon attack identifiers beginning at identifier 0x38.
 */
// FUNCTION: FF7 0x005c8cda
int __cdecl Battle_PickRandomSummonAttackId(void)
{
    int val;

    val = (Battle_RandBelow(0x10) & 0xFF) + 0x38;
    return val;
}

/*
 * For a player-controlled party member, scans its eight reaction triggers for the three
 * BattleReactionTypes of triggerMode (a BattleReactionTriggerMode). Outside Final Attack, an actor
 * that is asleep, confused, stopped, a frog, petrified, berserk, paralysed or has status bit 31
 * set does not react. Skips zero chance entries; Final Attack consumes one stored charge and
 * always fires. Successful random rolls resolve an attack descriptor and choose stored targets
 * or the actor bit according to result bit 2. Final Attack restricts targets to the party slots;
 * Sneak Attack clears targets and sets a separate actor flag; Sneak Attack and the counters
 * enqueue with flag one.
 */
// FUNCTION: FF7 0x005c8cfa
void __cdecl Battle_RollReactionTriggers(int actorIndex, int triggerMode)
{
    int idx;
    BattleReactionTrigger *entries;
    int typeOfs;
    int typeBase;
    int percent;
    int param;
    int resolveResult;
    int attackDef;
    int enqueueFlag;
    int targetFlags;

    if (triggerMode != BATTLE_REACTION_MODE_FINAL_ATTACK &&
        (g_aCombatants[actorIndex].status & (BATTLE_STATUS_SLEEP | BATTLE_STATUS_CONFUSION | BATTLE_STATUS_STOP |
                                             BATTLE_STATUS_FROG | BATTLE_STATUS_PETRIFY | BATTLE_STATUS_BERSERK |
                                             BATTLE_STATUS_PARALYSIS | 0x80000000)) != 0) { /* empty first arm: lib/codegen.h */
    } else if (actorIndex < 3 && (g_aCombatants[actorIndex].flags & BATTLE_COMBATANT_AI_CONTROLLED) == 0) {
        entries = g_MenuPartyStats[actorIndex].reactions;
        for (idx = 0; idx < 8; idx++) {
            typeBase = triggerMode * 3 + 1;
            for (typeOfs = 0; typeOfs < 3; typeOfs++) {
                if (entries[idx].typeId == typeBase + typeOfs) {
                    percent = entries[idx].chance;
                    if (percent != 0) {
                        if (triggerMode == BATTLE_REACTION_MODE_FINAL_ATTACK) {
                            percent = 0x64;
                            (*(char *)&entries[idx].chance)--;
                        }
                        if ((Battle_RandBelow(0x64) & 0xFF) < percent) {
                            resolveResult = Battle_ResolveReactionAttackDescriptor(actorIndex, typeOfs, entries[idx].attackIndex, &attackDef);
                            if ((resolveResult & 2) != 0) {
                                targetFlags = (*(unsigned short *)&g_aCombatants[actorIndex].attackerMask);
                            } else {
                                targetFlags = 1 << actorIndex;
                            }
                            switch (triggerMode) {
                            case BATTLE_REACTION_MODE_FINAL_ATTACK:
                                enqueueFlag = 0;
                                targetFlags &= BATTLE_ACTOR_MASK_ALLIES;
                                break;
                            case BATTLE_REACTION_MODE_SNEAK_ATTACK:
                                enqueueFlag = 1;
                                targetFlags = 0;
                                *(char *)&g_BattleState.actors[actorIndex].timingFlags |= 4;
                                break;
                            case BATTLE_REACTION_MODE_COUNTER:
                                enqueueFlag = 1;
                            }
                            Battle_EnqueueActionDirect(actorIndex, enqueueFlag, attackDef, param, targetFlags);
                        }
                    }
                }
            }
        }
    }
}

/*
 * Initial descriptor kind from slotIndex table and ID minus one; non-kind-one resolves attackId
 * direct or indexed callback for>=253. Kind two reads type and remaps IDs>=56 to kind three; others
 * remap into command table. Arg1 unused.
 */
// FUNCTION: FF7 0x005c8eeb
int __cdecl Battle_ResolveReactionAttackDescriptor(int unusedArg, int slotIndex, int attackId, int *descriptor)
{
    int result;

    result = 3;
    descriptor[0] = g_BattleReactionSlotDescriptorKinds[slotIndex];
    descriptor[1] = -1;
    if (descriptor[0] != 1) {
        descriptor[1] = attackId;
        if (descriptor[1] >= BATTLE_REACTION_RANDOM_COMMAND) {
            descriptor[1] = g_BattleReactionAttackPickFuncs[attackId - BATTLE_REACTION_RANDOM_COMMAND]();
        }
        if (descriptor[0] == 2) {
            result = g_BattleAttackTable[descriptor[1]].targetFlags;
            if (descriptor[1] >= 0x38) {
                descriptor[0] = 3;
                descriptor[1] -= 0x38;
            }
        } else {
            descriptor[0] = descriptor[1];
            descriptor[1] = -1;
            result = g_KernelCommandTable[descriptor[0]].targetFlags;
        }
    }
    return result;
}
} /* extern "C" */

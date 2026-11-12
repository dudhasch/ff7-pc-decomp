// src/ff7/src/battle/battle_recompute_actor_masks.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Recomputes the active, targetable, covered and selectable bitmasks for all battle
//   combatants from their current state.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
extern "C" {


/*
 * Recomputes combatant bitmasks (active, targetable, covered, selectable) for
 * all 10 battle actors and determines front/back row cover status among enemies.
 */
// FUNCTION: FF7 0x00432c60
void __cdecl Battle_RecomputeActorMasks(void)
{
    unsigned short selectMask;
    unsigned short incapMask;
    int partyAliveMask;
    unsigned short aiMask;
    int firstRow;
    int selEnemyIdx;
    int enemyFlags;
    int combatantIdx;
    int statusBits;
    unsigned short nextCoveredMask;
    unsigned short absentBits;
    unsigned short actorBit;
    int flagFlags;
    unsigned short ownRow;
    unsigned short blockers;
    int m5;
    int n1;
    unsigned short enemyBit;

    selectMask = 0;
    nextCoveredMask = 0;
    aiMask = 0;
    absentBits = 0;
    incapMask = 0;
    for (combatantIdx = 0; combatantIdx < 10; combatantIdx++) {
        actorBit = 1 << combatantIdx;
        flagFlags = g_aCombatants[combatantIdx].flags;
        statusBits = g_aCombatants[combatantIdx].status;
        if (flagFlags & BATTLE_COMBATANT_TARGETABLE) {
            selectMask |= actorBit;
        }
        if (flagFlags & BATTLE_COMBATANT_AI_CONTROLLED) {
            aiMask |= actorBit;
        }
        if (statusBits & 0x4000) {
            absentBits |= actorBit;
        }
        if (statusBits & 1) {
            incapMask |= actorBit;
        }
        if (statusBits & 0x80000000) {
            m5 = g_BattleState.statusChangeMode;
            if (m5 != 0 && m5 != 3) {
                ;
            } else {
                incapMask |= actorBit;
            }
        }
    }
    g_BattleState.absentActorMask = absentBits;
    g_BattleScene.targetMaskBits = selectMask;
    g_BattleScene.aiTargetableMask = aiMask;
    g_BattleScene.incapacitatedMask = incapMask;
    firstRow = 0xFFFF;
    for (combatantIdx = 0; combatantIdx < 6; combatantIdx++) {
        enemyBit = 1 << (combatantIdx + 4);
        if (selectMask & enemyBit) {
            blockers = 0;
            ownRow = g_aCombatants[combatantIdx + 4].row;
            g_aCombatants[combatantIdx + 4].flags &= ~(BATTLE_COMBATANT_BACK_ROW | BATTLE_COMBATANT_COVERED);
            for (selEnemyIdx = 0; selEnemyIdx < 6; selEnemyIdx++) {
                if (selectMask & (1 << (selEnemyIdx + 4))) {
                    if (g_aCombatants[selEnemyIdx + 4].row < ownRow) {
                        blockers |= g_BattleScene.enemies[selEnemyIdx].statusMask;
                    }
                }
            }
            if (blockers & g_BattleScene.enemies[combatantIdx].statusMask) {
                nextCoveredMask |= enemyBit;
                g_aCombatants[combatantIdx + 4].flags |= BATTLE_COMBATANT_COVERED;
            }
            if (firstRow > ownRow) {
                firstRow = ownRow;
            }
        }
    }
    for (combatantIdx = 0; combatantIdx < 6; combatantIdx++) {
        if (selectMask & (1 << (combatantIdx + 4))) {
            if (g_aCombatants[combatantIdx + 4].row != firstRow) {
                g_aCombatants[combatantIdx + 4].flags |= BATTLE_COMBATANT_BACK_ROW;
            }
        }
    }
    g_BattleScene.targetValidMask = selectMask ^ nextCoveredMask;
    g_BattleScene.targetExtraMask = selectMask;
    partyAliveMask = ~incapMask & BATTLE_ACTOR_MASK_ALLIES;
    enemyFlags = (aiMask | selectMask) & BATTLE_ACTOR_MASK_ENEMIES;
    g_BattleScene.targetMask = *(unsigned short *)&gBattleAiGlobals.actorMask & (partyAliveMask | enemyFlags);
    if (g_BattleState.layoutType == 4) {
        n1 = (unsigned short)g_BattleScene.targetMask & BATTLE_ACTOR_MASK_ENEMIES;
        for (combatantIdx = 0; combatantIdx < 2; combatantIdx++) {
            if ((n1 & g_BattleScene.targetMaskTable[combatantIdx * 2]) == 0) {
                break;
            }
        }
        g_BattleScene.occupiedEnemySideCount = combatantIdx;
    }
}
} /* extern "C" */

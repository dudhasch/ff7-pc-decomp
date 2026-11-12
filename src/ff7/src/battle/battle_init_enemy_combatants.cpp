// src/ff7/src/battle/battle_init_enemy_combatants.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sets up the enemy combatant records and their AI variables from the loaded battle data,
//   then runs each present enemy's init AI and snapshots its actor flags and status.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
extern "C" {


#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Initializes combatant records and AI variables for up to six enemy combatants
 * from the loaded battle scene data.
 */
// FUNCTION: FF7 0x005d0690
void __cdecl Battle_InitEnemyCombatants(void)
{
    int slot;
    short enemyIdx;
    BattleCombatant *recPtr;
    BattleActorRuntime *pActor;
    BattleEnemyCombatantSlot *slotRecord;
    BattleEnemyRecord *enemyRecord;
    int attackSlots;
    unsigned short attackId;
    BattleEnemyCombatantSlot *pSpareEntry;
    int aimBits;
    int scanIdx;
    int unused;
    int inner;

    unused = 0;
    gBattleAiGlobals.enemyMask = 0;
    for (slot = 0; slot < 3; slot++) {
        g_BattleState.enemyInitCount[slot] = 0;
    }
    for (slot = 0; slot < 6; slot++) {
        for (inner = 0; inner < 32; inner++) {
            *(int *)&gBattleAiGlobals.actorVars[slot + 4][inner * 4] = 0;
        }
    }
    for (slot = 0; slot < 6; slot++) {
        pActor = &g_BattleState.actors[slot + 4];
        recPtr = &g_aCombatants[slot + 4];
        enemyIdx = g_BattleScene.enemies[slot].enemyIndex;
        recPtr->unitId = 0xFF;
        recPtr->uniqueId = 0xFFFF;
        recPtr->flags = 0;
        recPtr->status = 0;
        recPtr->idlePose = 0xFF;
        g_BattleState.enemyInitSlotId[slot][0] = 0xFF;
        for (inner = 0; inner < 16; inner++) {
            *(char *)((char *)((int)pActor + inner) + 0x10) = 0;
        }
        for (inner = 0; inner < 8; inner++) {
            pActor->statPercent[inner] = 0;
        }
        if (enemyIdx != -1) {
            recPtr->uniqueId = enemyIdx;
            for (inner = 0; inner < 3; inner++) {
                if (g_BattleScene.enemyIds[inner] == enemyIdx) {
                    break;
                }
            }
            g_BattleState.enemyInitCount[inner]++;
            enemyIdx = inner;
            enemyRecord = &g_BattleState.enemyRecords[enemyIdx];
            g_BattleScene.enemies[slot].enemyIndex = enemyIdx;
            recPtr->unitId = enemyIdx;
            recPtr->maxHp = enemyRecord->hp;
            recPtr->curHp = recPtr->maxHp;
            recPtr->maxMp = enemyRecord->mp;
            recPtr->curMp = recPtr->maxMp;
            recPtr->attack = enemyRecord->strength;
            recPtr->magicAttack = enemyRecord->magic;
            recPtr->defense = enemyRecord->defense << 1;
            recPtr->magicDefense = enemyRecord->magicDefense << 1;
            recPtr->defensePercent = enemyRecord->defensePercent;
            recPtr->dexterity = enemyRecord->dexterity;
            recPtr->luck = enemyRecord->luck;
            recPtr->level = enemyRecord->level;
            recPtr->damageScale = enemyRecord->damageScale;
            recPtr->gilReward = enemyRecord->gil;
            recPtr->expReward = enemyRecord->exp;
            recPtr->animId = 1;
            recPtr->isEnemy = 1;
            recPtr->hitAnimId = 2;
            recPtr->scriptId = 0;
            recPtr->status = 0;
            recPtr->prevStatusFlags = 0;
            recPtr->spoilA = 0;
            recPtr->spoilB = 0xFFFF;
            recPtr->flags = g_BattleScene.enemies[slot].flags & 0x1F;
            recPtr->row = g_BattleScene.enemies[slot].initByte;
            pActor->enemyRecord = (int)enemyRecord;
            pActor->accessoryKind = 0xFF;
            pActor->linkedActor = 0xFF;
            pActor->nameSuffixIndex = 0xFF;
            pActor->statusImmunityMask = ~enemyRecord->statusImmunityMask;
            gBattleAiGlobals.enemyMask |= (short)(1 << slot + 4);
            recPtr->sideSlot = 0;
            for (inner = 0; inner < slot; inner++) {
                if (g_aCombatants[inner + 4].uniqueId == recPtr->uniqueId) {
                    (recPtr->sideSlot)++;
                }
            }
            attackSlots = 3;
            for (inner = 0; inner < attackSlots; inner++) {
                slotRecord = &g_BattleItemListBlock.enemyActions[slot][inner];
                attackId = enemyRecord->slotAttackIds[inner];
                slotRecord->attackIndex = 0xFF;
                slotRecord->actionKind = 0;
                slotRecord->targetFlags = 0;
                slotRecord->state = 3;
                if (attackId != 0xFFFF) {
                    for (scanIdx = 0; scanIdx < 32; scanIdx++) {
                        if (g_BattleState.attackIds[scanIdx] == attackId) {
                            aimBits = g_BattleState.attackEntries[scanIdx * 0x1C + 12];
                            if (aimBits != 0) {
                                aimBits ^= 2;
                            }
                            slotRecord->attackIndex = scanIdx;
                            slotRecord->targetFlags = aimBits;
                            slotRecord->state = 0;
                            break;
                        }
                    }
                }
            }
            for (inner = attackSlots; inner < 16; inner++) {
                pSpareEntry = &g_BattleItemListBlock.enemyActions[slot][inner];
                pSpareEntry->attackIndex = 0xFF;
                pSpareEntry->actionKind = 0;
                pSpareEntry->targetFlags = 0;
                pSpareEntry->state = 3;
            }
            pActor->timingFlags = 0;
        }
    }
    for (slot = 0; slot < 6; slot++) {
        if ((&g_BattleState.enemyInitCount[0])[g_BattleScene.enemies[slot].enemyIndex] > 1) {
            g_BattleState.actors[(slot + 4)].nameSuffixIndex = g_aCombatants[slot + 4].sideSlot;
        }
    }
}

/*
 * Runs the init AI event for each present enemy, then copies every enemy combatant's flags and
 * idle script into the scene and snapshots its status.
 */
// FUNCTION: FF7 0x005d0bdf
void __cdecl Battle_RunEnemyAiInitEvents(void)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (g_BattleScene.enemies[i].enemyIndex != -1) {
            Battle_RunAiScriptEvent(i + 4, 0, 0);
        }
    }
    for (i = 0; i < 6; i++) {
        g_BattleScene.enemies[i].flags = g_aCombatants[i + 4].flags;
        g_BattleScene.actors[(i + 4)].scriptId = g_aCombatants[i + 4].scriptId;
        g_aCombatants[i + 4].prevStatusFlags = g_aCombatants[i + 4].status;
    }
}
} /* extern "C" */

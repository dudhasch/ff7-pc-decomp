// src/ff7/src/battle/battle_steal_command_effects.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle steal and command effects: attempts a steal on hit, maps support materia flags,
//   finds ability slots, and queues command effect steps, default hit poses, follow-up attacks and
//   morph rewards.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/menu.h"
#include "ff7/api/menu.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x007b76c0
extern const int g_BattlePartyHitReactionPoses = 302450181;
// GLOBAL: FF7 0x007b76c4
extern const unsigned char g_BattleStealLevelScaleDivisor[4] = {0x0f, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

/*
 * Starts no-item message 0x31. Valid enemy entry with available steal candidate and not-stolen
 * target computes level-derived chance, floors to 100 for actor equipment code four, scales by
 * 512/100 and rolls. Failure selects 0x30; success posts item event, sets stolen bit and
 * selects 0x2f. Always posts formatted message.
 */
// FUNCTION: FF7 0x005ca860
void __cdecl Battle_TryStealOnHit(void)
{
    int msgId;
    unsigned short stolenItem;
    BattleEnemyRecord *enemyRecord;
    int targetIndex;
    int odds;

    targetIndex = g_pBattleCommandCtx->targetSlot;
    msgId = 0x31;
    stolenItem = 0;
    if (g_pBattleCommandCtx->enemyIndex != -1) {
        enemyRecord = &g_BattleState.enemyRecords[g_pBattleCommandCtx->enemyIndex];
        if ((unsigned short)Battle_RollEnemyDropOrSteal(enemyRecord, 0x80, 1, 0) != 0xFFFF && (g_BattleState.actors[targetIndex].timingFlags & 1) == 0) {
            odds = g_pBattleCommandCtx->attackerLevel + 0x28 - g_pBattleCommandCtx->targetLevel;
            if ((unsigned char)g_BattleState.actors[g_pBattleCommandCtx->attackerSlot].accessoryKind == 4 && odds < 100) {
                odds = 0x64;
            }
            odds = (odds << 9) / 100;
            stolenItem = Battle_RollEnemyDropOrSteal(enemyRecord, 0x80, 0, odds);
            if (stolenItem == 0xFFFF) {
                msgId = 0x30;
            } else {
                Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, g_pBattleCommandCtx->attackerSlot, BATTLE_ATB_EVENT_ADD_ITEM_DROP, stolenItem);
                g_BattleState.actors[targetIndex].timingFlags |= 1;
                msgId = 0x2F;
            }
        }
    }
    Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->attackerSlot, msgId, 1, &stolenItem);
}

/*
 * Maps support bits 1/2/8/16 to attackFlags bits 0x80/0x40/4/0x800. Any of bits 0xE0 set assigns
 * (supportBits >> 5) * 10 to bonusPercent, including all higher bits.
 */
// FUNCTION: FF7 0x005ca9d0
void __cdecl Battle_ApplySupportMateriaFlags(int supportBits)
{
    if ((supportBits & 1) != 0) {
        g_pBattleCommandCtx->attackFlags |= 0x80;
    }
    if ((supportBits & 2) != 0) {
        g_pBattleCommandCtx->attackFlags |= 0x40;
    }
    if ((supportBits & 8) != 0) {
        g_pBattleCommandCtx->attackFlags |= 4;
    }
    if ((supportBits & 0x10) != 0) {
        g_pBattleCommandCtx->attackFlags |= 0x800;
    }
    if ((supportBits & 0xE0) != 0) {
        g_pBattleCommandCtx->bonusPercent = (supportBits >> 5) * 10;
    }
}

/*
 * Selects command-dependent search range:56 magic entries, 16 summon entries from 56, 24 enemy
 * skill entries from 72. First matching byte stores slot; unsupported command/absent match
 * preserves prior slot.
 */
// FUNCTION: FF7 0x005caa86
void __cdecl Battle_FindAbilitySlotForCommand(int partyIndex)
{
    BattleAbilitySlot *recordBase;
    int cnt;
    int offset;
    int i;

    offset = 0;
    cnt = 0;
    recordBase = g_MenuPartyStats[partyIndex].spells;
    switch (g_pBattleCommandCtx->commandIndex) {
    case BATTLE_COMMAND_MAGIC:
    case BATTLE_COMMAND_W_MAGIC:
        cnt = 56;
        break;
    case BATTLE_COMMAND_SUMMON:
    case BATTLE_COMMAND_W_SUMMON:
        offset = 0x38;
        cnt = 16;
        break;
    case BATTLE_COMMAND_ENEMY_SKILL:
        offset = 0x48;
        cnt = 24;
        break;
    }
    if (cnt != 0) {
        for (i = 0; i < cnt; i++) {
            if (recordBase[i + offset].attackId == g_pBattleCommandCtx->attackIdBase) {
                g_pBattleCommandCtx->magicIndex = i + offset;
                break;
            }
        }
    }
}

/*
 * Pushes an animation step for the attacker with the command context's stepFlags,
 * resolvedAttackId and stepPower.
 */
// FUNCTION: FF7 0x005cab7c
void __cdecl Battle_QueueCommandEffectStep(int stepId)
{
    Battle_PushAnimStepWithHitRecord(g_pBattleCommandCtx->attackerSlot, stepId, g_pBattleCommandCtx->stepFlags, g_pBattleCommandCtx->resolvedAttackId, g_pBattleCommandCtx->stepPower, 0, 0);
}

/*
 * For a party target (slot below three) reads the pose table byte at hitReactionIndex; otherwise
 * returns the target combatant's animId.
 */
// FUNCTION: FF7 0x005cabba
int __cdecl Battle_GetDefaultHitReactionPose(void)
{
    unsigned char val;

    if (g_pBattleCommandCtx->targetSlot < 3) {
        val = *(unsigned char *)((char *)&g_BattlePartyHitReactionPoses + g_pBattleCommandCtx->hitReactionIndex);
    } else {
        val = g_aCombatants[g_pBattleCommandCtx->targetSlot].animId;
    }
    return val;
}

/*
 * Stores attackId in the first empty (0xFF) entry of followUpAttackIds; leaves them unchanged if
 * none is empty.
 */
// FUNCTION: FF7 0x005cac07
void __cdecl Battle_QueueFollowUpAttack(int attackId)
{
    int slot;

    for (slot = 0; slot < 8; slot++) {
        if (g_pBattleCommandCtx->followUpAttackIds[slot] == 0xFF) {
            g_pBattleCommandCtx->followUpAttackIds[slot] = attackId;
            break;
        }
    }
}

// Grants the morph reward for the target enemy once. It applies when the target slot is 4 or more
// and bit 0x10 of its stateFlags is clear. The enemy's morph item is read from its enemy record, and only a value other than 0xFFFF is granted. The target's state is set to 7 when it is
// below 0x11, then Battle_PostEvent posts event 3 for the item to the attacker, and
// Battle_PostBattleMessageFormatted shows message 0x56. Bit 0x10 of stateFlags is then set so the
// reward is granted only once.
// FUNCTION: FF7 0x005cac55
void __cdecl Battle_TryMorphReward(void)
{
    unsigned short morphItem;
    BattleEnemyRecord *enemyRec;

    if (g_pBattleCommandCtx->targetSlot >= 4) {
        if ((g_BattleState.actors[g_pBattleCommandCtx->targetSlot].timingFlags & 0x10) == 0) {
            enemyRec = &g_BattleState.enemyRecords[g_BattleScene.enemies[g_pBattleCommandCtx->targetSlot - 4].enemyIndex];
            morphItem = enemyRec->morphItem;
            if (morphItem != 0xFFFF) {
                if (g_BattleScene.actors[g_pBattleCommandCtx->targetSlot].state < 0x11) {
                    g_BattleScene.actors[g_pBattleCommandCtx->targetSlot].state = 7;
                }
                Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, g_pBattleCommandCtx->attackerSlot, BATTLE_ATB_EVENT_ADD_ITEM_DROP, morphItem);
                Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->attackerSlot, 0x56, 1, &morphItem);
                g_BattleState.actors[g_pBattleCommandCtx->targetSlot].timingFlags |= 0x10;
            }
        }
    }
}
} /* extern "C" */

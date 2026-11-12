// src/ff7/src/battle/battle_attack_op_handlers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Attack operation handlers run from the battle command program: they set attack IDs,
//   targets, repeat counts, power and facing, and apply weapon, item and attack records to the
//   current action.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/enums/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/menu.h"
#include "lib/codegen.h"
#include "lib/api/token.h"
#include "ff7/savemap.h"
#include "ff7/types/menu.h"
extern "C" {
// The attack-op callback table, indexed by attack opcode.
// GLOBAL: FF7 0x008fee48
BattleActionCallback g_BattleAttackOpCallbacks[31] = {
    (BattleActionCallback)Battle_AttackOpSetAttackIdMagic,
    (BattleActionCallback)Battle_AttackOpSetAttackIdSummon,
    (BattleActionCallback)Battle_AttackOpSetItemId,
    (BattleActionCallback)Battle_AttackOpSetAttackIdEnemySkill,
    (BattleActionCallback)Battle_AttackOpToggleFacing,
    (BattleActionCallback)Battle_AttackOpNop,
    (BattleActionCallback)Battle_AttackOpSetAttackIdLimit,
    (BattleActionCallback)Battle_AttackOpSetAttackIdFromEnemyList,
    (BattleActionCallback)Battle_AttackOpPostMessage,
    (BattleActionCallback)Battle_AttackOpExecute,
    (BattleActionCallback)Battle_AttackOpResolveTargets,
    (BattleActionCallback)Battle_AttackOpTargetSelf,
    (BattleActionCallback)Kernel_ApplyAttackToAction,
    (BattleActionCallback)Kernel_ApplyItemToAction,
    (BattleActionCallback)Battle_ApplyEquippedWeaponToAction,
    (BattleActionCallback)Battle_AttackOpQueueAnimOnly,
    (BattleActionCallback)Battle_AttackOpSetRepeat4,
    (BattleActionCallback)Battle_AttackOpRunAiEventOnTargets,
    (BattleActionCallback)Battle_AttackOpSetRepeat2,
    (BattleActionCallback)Battle_AttackOpMakeSlipDamage,
    (BattleActionCallback)Battle_AttackOpYieldUntilUiReady,
    (BattleActionCallback)Battle_AttackOpRedirectToEnemyActor,
    (BattleActionCallback)Battle_AttackOpShowSenseReadout,
    (BattleActionCallback)Battle_AttackOpReplayLastActions,
    (BattleActionCallback)Battle_AttackOpMakeInstantDeath,
    (BattleActionCallback)Kernel_ApplyWeaponToAction,
    (BattleActionCallback)Battle_AttackOpMakeManipulate,
    (BattleActionCallback)Battle_AttackOpMakeDeathblow,
    (BattleActionCallback)Battle_AttackOpSetPower2,
    (BattleActionCallback)Battle_AttackOpSpendGilForPower,
    (BattleActionCallback)Battle_AttackOpSetWeaponImpact,
};

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)
// clang-format off
// GLOBAL: FF7 0x007b76a0
extern const unsigned char g_BattleKernelAttackAnimOverride[32] = {0xff, 0xff, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x43, 0x48, 0x44, 0xff, 0xff, 0xff, 0xff, 0xff};
// clang-format on


/*
 * Sets the command's attackId to its attackIdBase.
 */
// FUNCTION: FF7 0x005c8fb0
void __cdecl Battle_AttackOpSetAttackIdMagic(void)
{
    g_pBattleCommandCtx->attackId = g_pBattleCommandCtx->attackIdBase;
}

/*
 * Sets the command's attackId to attackIdBase plus 0x38.
 */
// FUNCTION: FF7 0x005c8fc6
void __cdecl Battle_AttackOpSetAttackIdSummon(void)
{
    g_pBattleCommandCtx->attackId = g_pBattleCommandCtx->attackIdBase + 0x38;
}

/*
 * Copies command ID into two fields, invokes helper with actor/item shorts and selects field 0x20
 * between 0x20/0x21 from command 0x18 low nibble.
 */
// FUNCTION: FF7 0x005c8fdf
void __cdecl Battle_AttackOpSetItemId(void)
{
    g_pBattleCommandCtx->attackId = g_pBattleCommandCtx->attackIdBase;
    g_pBattleCommandCtx->resolvedAttackId = g_pBattleCommandCtx->attackIdBase;
    Battle_ClearPendingDropRecord((short)g_pBattleCommandCtx->attackerSlot, (short)g_pBattleCommandCtx->attackId);
    if ((g_pBattleCommandCtx->targetMask & BATTLE_ACTOR_MASK_ALLIES) != 0) {
        g_pBattleCommandCtx->stepId = 0x20;
    } else {
        g_pBattleCommandCtx->stepId = 0x21;
    }
}

/*
 * Missing attack ID invokes random throwable-weapon selection. A resulting ID populates command
 * IDs and relative index, resolves actor/item data and copies fixed power sixteen plus
 * weapon/actor fields. If selection stays 0xffff only sets command field 0x20 to minus one.
 */
// FUNCTION: FF7 0x005c904d
void __cdecl Kernel_ApplyWeaponToAction(void)
{
    int val;

    if (g_pBattleCommandCtx->attackIdBase == 0xFFFF) {
        /* Retail caller treats `Battle_PickRandomThrowableWeapon` return (`short`) as `unsigned short` for caller zero/sign extension. */
        g_pBattleCommandCtx->attackIdBase = ((unsigned short (__cdecl *)(void))Battle_PickRandomThrowableWeapon)();
    }
    if (g_pBattleCommandCtx->attackIdBase != 0xFFFF) {
        g_pBattleCommandCtx->attackId = g_pBattleCommandCtx->attackIdBase;
        g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->attackIdBase;
        g_pBattleCommandCtx->resolvedAttackId = g_pBattleCommandCtx->attackIdBase - 0x80;
        Battle_ClearPendingDropRecord((short)g_pBattleCommandCtx->attackerSlot, (short)g_pBattleCommandCtx->attackId);
        val = g_pBattleCommandCtx->resolvedAttackId;
        g_pBattleCommandCtx->power = 0x10;
        g_pBattleCommandCtx->weaponPower = g_KernelWeaponDataArray[val].attackBonusByte + g_MenuPartyStats[g_pBattleCommandCtx->attackerSlot].strength;
        g_pBattleCommandCtx->secondaryDisplayValue = g_KernelWeaponDataArray[val].secondaryDisplayValue;
    } else {
        g_pBattleCommandCtx->stepId = -1;
    }
}

/*
 * Computes spend as attack ID times ten; sentinel 0xffff instead uses maximum unsigned selected
 * enemy current value times selected count times ten, with signed maximum cap 10000. Caps
 * spend600000, optionally clamps to unsigned gil and subtracts unless bypass flag. Writes
 * spend/ten to three power/ID fields.
 */
// FUNCTION: FF7 0x005c9150
void __cdecl Battle_AttackOpSpendGilForPower(void)
{
    int spend;
    int enemyCount;
    int highestHp;
    int index;
    int enemyMask;

    spend = g_pBattleCommandCtx->attackIdBase * 10;
    if (g_pBattleCommandCtx->attackIdBase == 0xFFFF) {
        highestHp = 0;
        enemyCount = 0;
        enemyMask = g_BattleScene.targetMaskBits & BATTLE_ACTOR_MASK_ENEMIES;
        for (index = 4; index < 10; index++) {
            if ((enemyMask & 1 << index) != 0) {
                if ((unsigned int)highestHp < (unsigned int)g_aCombatants[index].curHp) {
                    highestHp = g_aCombatants[index].curHp;
                }
                enemyCount++;
            }
        }
        if (highestHp > 10000) {
            highestHp = 0x2710;
        }
        spend = highestHp * enemyCount * 10;
    }
    if (spend > 600000) {
        spend = 0x927C0;
    }
    if ((g_pBattleCommandCtx->attackFlags & 0x400000) == 0) {
        if ((unsigned int)spend > (unsigned int)gSavemap.gil) {
            spend = gSavemap.gil;
        }
        gSavemap.gil -= spend;
    }
    g_pBattleCommandCtx->attackIdBase = spend / 10;
    g_pBattleCommandCtx->power = g_pBattleCommandCtx->attackIdBase;
    g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->attackIdBase;
}

/*
 * Sets the command's attackId to attackIdBase plus 0x48.
 */
// FUNCTION: FF7 0x005c928e
void __cdecl Battle_AttackOpSetAttackIdEnemySkill(void)
{
    g_pBattleCommandCtx->attackId = g_pBattleCommandCtx->attackIdBase + 0x48;
}

/*
 * Sets stepId to -1; in the party-first layout selects step 3 when the attacker is in the back row
 * and 4 otherwise, then toggles its BATTLE_COMBATANT_BACK_ROW.
 */
// FUNCTION: FF7 0x005c92a7
void __cdecl Battle_AttackOpToggleFacing(void)
{
    g_pBattleCommandCtx->stepId = -1;
    if (Battle_IsPartyFirstLayout() != 0) {
        g_pBattleCommandCtx->stepId =
            (g_aCombatants[g_pBattleCommandCtx->attackerSlot].flags & BATTLE_COMBATANT_BACK_ROW) != 0 ? 3 : 4;
        g_aCombatants[g_pBattleCommandCtx->attackerSlot].flags ^= BATTLE_COMBATANT_BACK_ROW;
    }
}

/*
 * Does nothing.
 */
// FUNCTION: FF7 0x005c930a
void __cdecl Battle_AttackOpNop(void)
{
}

/*
 * Copies attack ID; if below 96 adds 128, clears actor short gauge and increments actor byte. Posts
 * event(2, actor, 17, 0) unless global bit eight set.
 */
// FUNCTION: FF7 0x005c930f
void __cdecl Battle_AttackOpSetAttackIdLimit(void)
{
    int actor;

    actor = g_pBattleCommandCtx->attackerSlot;
    g_pBattleCommandCtx->attackId = g_pBattleCommandCtx->attackIdBase;
    if (g_pBattleCommandCtx->attackId < 0x60) {
        g_pBattleCommandCtx->attackId += 0x80;
        g_BattleState.partyStats[actor].limitGauge = 0;
        (g_BattleState.partyStats[actor].limitUseCount)++;
        if ((gBattleAiGlobals.arenaFlags & 8) == 0) {
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, actor, BATTLE_ACTION_EVENT_CLEAR_LIMIT_GAUGE, 0);
        }
    }
}

/*
 * Copies indexed enemy attack ID. For command 32 maps first threshold interval below 56/72/96/256
 * into command 2/3/13/20 and relative ID. Negative IDs enter first interval; at least 256 leaves
 * prior mapping.
 */
// FUNCTION: FF7 0x005c93a1
void __cdecl Battle_AttackOpSetAttackIdFromEnemyList(void)
{
    struct {
        unsigned char ids[4];
        unsigned int i;
        unsigned short thresholds[5];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_pBattleCommandCtx->attackId = g_BattleState.attackIds[g_pBattleCommandCtx->attackIdBase];
    if (g_pBattleCommandCtx->commandIndex == 0x20) {
        local.thresholds[0] = 0;
        local.thresholds[1] = 0x38;
        local.thresholds[2] = 0x48;
        local.thresholds[3] = 0x60;
        local.thresholds[4] = 0x100;
        local.ids[0] = 2;
        local.ids[1] = 3;
        local.ids[2] = 0xD;
        local.ids[3] = 0x14;
        for (local.i = 0; local.i < 4; local.i++) {
            if (g_pBattleCommandCtx->attackId < local.thresholds[local.i + 1]) {
                g_pBattleCommandCtx->stepFlags = local.ids[local.i];
                g_pBattleCommandCtx->stepPower = g_pBattleCommandCtx->attackId - local.thresholds[local.i];
                break;
            }
        }
    }
}

/*
 * Configures the current attack operation as the Manipulate command.
 */
// FUNCTION: FF7 0x005c9467
void __cdecl Battle_AttackOpMakeManipulate(void)
{
    g_pBattleCommandCtx->statusMaskA = BATTLE_STATUS_MANIPULATE;
    g_pBattleCommandCtx->payload = 0x59;
}

/*
 * Passes the command's attackIdBase to Battle_PostBattleMessage.
 */
// FUNCTION: FF7 0x005c948b
void __cdecl Battle_AttackOpPostMessage(void)
{
    Battle_PostBattleMessage(g_pBattleCommandCtx->attackIdBase);
}

/*
 * A one-line forwarder: calls Battle_ResolveTargetMask (defined in battle_target_resolution.cpp)
 * and does nothing else.
 */
// FUNCTION: FF7 0x005c94a1
void __cdecl Battle_AttackOpResolveTargets(void)
{
    Battle_ResolveTargetMask();
}

/*
 * Clears the command's targetFlags and targets only the attacker: targetMask is one shifted by
 * attackerSlot.
 */
// FUNCTION: FF7 0x005c94ab
void __cdecl Battle_AttackOpTargetSelf(void)
{
    g_pBattleCommandCtx->targetFlags = 0;
    g_pBattleCommandCtx->targetMask = 1 << g_pBattleCommandCtx->attackerSlot;
}

/*
 * Selects attack record from scene lookup for command 0x20, kernel table for attack below 0x80, or
 * one of three actor limit records for player index below three, computing limit animation index
 * by counting non-0x7F IDs. Kernel IDs 0x60..0x7F may override animation and clear materia index.
 * Marks special command-field-two/attack-0x36 path. With valid materia index and flag 0x400000
 * clear, copies cost byte, consumes repeat-cast count and posts event or consumes limited uses for
 * command three/flag 0x200, setting failure 0x79 or flag 0x100000 as applicable; high battle mode
 * copies extra byte. Enemy actors scan sixteen scene attack IDs for animation/paired-value
 * override; special path sets animation 0x34. Defaults cost from record, forcing zero in mode three
 * or flag 0x400000. Copies record power/element/accuracy/additional fields, normalizes the element
 * mask 0xFFFF (none) to zero and applies target flags. Command 0x14 chooses paired value by AI bit
 * eight; otherwise uses enemy override or separate record values. Copies optional byte one only
 * when not 0xFF, flags and three signed field-six values; halves accuracy when special flag 4 is
 * clear and the attacker has Darkness. Applies status change and added effect; special path clears
 * the attacker's Death Sentence. Missing record selection can leave null r dereferenced.
 */
// FUNCTION: FF7 0x005c94d3
void __cdecl Kernel_ApplyAttackToAction(void)
{
    int overrideParam;
    int special36;
    BattleAttackEntry *pAttackRecord;
    unsigned char *limitBytes;
    int unitIndex;
    int limitOrdinal;
    BattleEnemyRecord *enemyRecord;
    BattleMagicMenuEntry *magicRow;
    int arenaBit;
    int enemySlot;
    int elementBits;
    int limitSlot;
    int entryIdx;
    int ofs;
    int lookupIdx;
    int learnedId;
    int kernelId;

    overrideParam = 0xFFFF;
    special36 = 0;
    pAttackRecord = 0;
    g_pBattleCommandCtx->mpCost = 0xFFFFFFFF;
    if (g_pBattleCommandCtx->stepFlags == 0x20) {
        lookupIdx = Battle_FindSceneAttackIndex(g_pBattleCommandCtx->attackId);
        pAttackRecord = (BattleAttackEntry *)&g_BattleState.attackEntries[lookupIdx * 0x1C];
        g_pBattleCommandCtx->stepPower = lookupIdx;
    } else if (g_pBattleCommandCtx->attackId < 0x80) {
        pAttackRecord = (BattleAttackEntry *)&g_BattleAttackTable[g_pBattleCommandCtx->attackId].accuracy;
        if (g_pBattleCommandCtx->attackId >= 0x60) {
            ofs = g_pBattleCommandCtx->attackId - 0x60;
            if (g_BattleKernelAttackAnimOverride[ofs] != 0xFF) {
                g_pBattleCommandCtx->stepId = g_BattleKernelAttackAnimOverride[ofs];
            }
            g_pBattleCommandCtx->magicIndex = 0xFFFFFFFF;
        }
    } else if (g_pBattleCommandCtx->attackerSlot < 3) {
        unitIndex = g_BattleScene.actors[g_pBattleCommandCtx->attackerSlot].unitIndex;
        limitBytes = g_MenuPartyStats[g_pBattleCommandCtx->attackerSlot].limitAttacks;
        for (limitSlot = 0; limitSlot < 3; limitSlot++) {
            learnedId = limitBytes[limitSlot];
            if (learnedId + 0x80 == g_pBattleCommandCtx->attackId) {
                limitOrdinal = 0;
                for (entryIdx = 0; entryIdx < 12; entryIdx++) {
                    kernelId = Kernel_GetLimitAttackId(unitIndex, entryIdx);
                    if (kernelId != 0x7F) {
                        if (kernelId == learnedId) {
                            break;
                        }
                        limitOrdinal++;
                    }
                }
                g_pBattleCommandCtx->stepId = limitOrdinal + 0x3C;
                pAttackRecord = (BattleAttackEntry *)(limitBytes + limitSlot * 0x1C + 8);
                break;
            }
        }
    }
    if (g_pBattleCommandCtx->commandIndex == 2 && g_pBattleCommandCtx->attackId == 0x36) {
        special36 = 1;
    }
    if (g_pBattleCommandCtx->magicIndex != -1 && (g_pBattleCommandCtx->attackFlags & 0x400000) == 0) {
        magicRow = (BattleMagicMenuEntry *)&g_MenuPartyStats[g_pBattleCommandCtx->attackerSlot].abilities[g_pBattleCommandCtx->magicIndex];
        g_pBattleCommandCtx->mpCost = magicRow->mpCost;
        if (magicRow->repeatsLeft != 0 && magicRow->repeatCasts != 0) {
            (magicRow->repeatsLeft)--;
            Battle_SetRepeatCasts(magicRow->repeatCasts);
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, g_pBattleCommandCtx->attackerSlot, BATTLE_ACTION_EVENT_SET_DIRTY_FLAGS, 6);
        } else if (g_pBattleCommandCtx->stepFlags == 3) {
            if (magicRow->countdown != 0) {
                if (magicRow->countdown != 0xFF) {
                    (magicRow->countdown)--;
                    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, g_pBattleCommandCtx->attackerSlot, BATTLE_ACTION_EVENT_SET_DIRTY_FLAGS, 4);
                }
            } else {
                g_pBattleCommandCtx->messageId = 0x79;
            }
        } else if ((g_pBattleCommandCtx->attackFlags & 0x200) != 0) {
            if (magicRow->countdown != 0) {
                if (magicRow->countdown != 0xFF) {
                    (magicRow->countdown)--;
                    Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, g_pBattleCommandCtx->attackerSlot, BATTLE_ACTION_EVENT_SET_DIRTY_FLAGS, 2);
                }
            } else if ((pAttackRecord->targetFlags & 8) != 0) {
                g_pBattleCommandCtx->attackFlags |= 0x100000;
            }
        }
        if (g_BattleState.currentActionKind >= 5 && (g_pBattleCommandCtx->attackFlags & 0x400000) == 0) {
            g_pBattleCommandCtx->supportBits = magicRow->supportBits;
        }
    }
    if (special36 == 0) {
        if (g_pBattleCommandCtx->attackerSlot >= 4) {
            enemyRecord = &g_BattleState.enemyRecords[g_pBattleCommandCtx->enemyRecordIndex];
            for (enemySlot = 0; enemySlot < 16; enemySlot++) {
                if (g_pBattleCommandCtx->attackId == enemyRecord->attackIds[enemySlot]) {
                    g_pBattleCommandCtx->stepId = enemyRecord->attackStepIds[enemySlot];
                    overrideParam = enemyRecord->attackAnimParams[enemySlot];
                    break;
                }
            }
        }
    } else {
        g_pBattleCommandCtx->stepId = 0x34;
    }
    if (g_pBattleCommandCtx->mpCost < 0) {
        g_pBattleCommandCtx->mpCost = pAttackRecord->mpCost;
    }
    if (g_BattleState.currentActionKind == 3 || (g_pBattleCommandCtx->attackFlags & 0x400000) != 0) {
        g_pBattleCommandCtx->mpCost = 0;
    }
    g_pBattleCommandCtx->accuracy = pAttackRecord->accuracy;
    g_pBattleCommandCtx->damageCalcByte = pAttackRecord->damageCalcByte;
    if (pAttackRecord->elementMask == 0xFFFF) {
        elementBits = 0;
    } else {
        elementBits = pAttackRecord->elementMask;
    }
    g_pBattleCommandCtx->elementMask = elementBits;
    g_pBattleCommandCtx->power = pAttackRecord->power;
    g_pBattleCommandCtx->hitReactionIndex = pAttackRecord->hitReactionIndex;
    Battle_SetTargetFlagsIfUnset(pAttackRecord->targetFlags);
    if (g_pBattleCommandCtx->stepFlags == 0x14) {
        arenaBit = (gBattleAiGlobals.arenaFlags & 8) != 0;
        overrideParam = (&pAttackRecord->animParam)[arenaBit];
    }
    if (overrideParam == 0xFFFF) {
        g_pBattleCommandCtx->animParam = pAttackRecord->animParam;
        g_pBattleCommandCtx->animParamAlt = pAttackRecord->animParamAlt;
    } else {
        g_pBattleCommandCtx->animParam = overrideParam;
        g_pBattleCommandCtx->animParamAlt = overrideParam;
    }
    if (pAttackRecord->secondaryDisplay != 0xFF) {
        g_pBattleCommandCtx->secondaryDisplayValue = pAttackRecord->secondaryDisplay;
    }
    g_pBattleCommandCtx->resolvedAttackId = pAttackRecord->resolvedAttackId;
    g_pBattleCommandCtx->specialFlags = pAttackRecord->specialFlags;
    g_pBattleCommandCtx->displayValueA = pAttackRecord->hitValue;
    g_pBattleCommandCtx->displayValueB = pAttackRecord->hitValue;
    g_pBattleCommandCtx->displayValueC = pAttackRecord->hitValue;
    if ((g_pBattleCommandCtx->specialFlags & 4) == 0 && (g_aCombatants[g_pBattleCommandCtx->attackerSlot].status & BATTLE_STATUS_DARKNESS) != 0) {
        g_pBattleCommandCtx->accuracy >>= 1;
    }
    Battle_SetStatusChange(pAttackRecord->statusChangeId, pAttackRecord->statusMask);
    if (special36 != 0) {
        g_aCombatants[g_pBattleCommandCtx->attackerSlot].status &= ~BATTLE_STATUS_DEATH_SENTENCE;
    }
    Battle_SetAddedEffect(pAttackRecord->addedEffectId, pAttackRecord->addedEffectParam);
}

// Copies the item record that g_pBattleCommandCtx->attackId selects from the 0x1C-byte item table
// at g_MenuKernelItemBuffer (0x00DBD160) into the command context. accuracy is set to 0xFF, and
// elementMask is copied unless the record's value is 0xFFFF. animParam and animParamAlt both take
// the record's value, and specialFlags, damageCalcByte, power and resolvedAttackId are copied too.
// Then it calls Battle_SetTargetFlagsIfUnset, Battle_SetStatusChange and Battle_SetAddedEffect with
// the record's target flags, status change and added effect fields.
// FUNCTION: FF7 0x005c9b6c
void __cdecl Kernel_ApplyItemToAction(void)
{
    BattleItemAction *rec;

    rec = (BattleItemAction *)(g_pBattleCommandCtx->attackId * 0x1C + (int)((unsigned char *)g_MenuKernelItemBuffer));
    g_pBattleCommandCtx->accuracy = 0xFF;
    g_pBattleCommandCtx->damageCalcByte = rec->damageCalcByte;
    g_pBattleCommandCtx->power = rec->power;
    if (rec->elementMask != 0xFFFF) {
        g_pBattleCommandCtx->elementMask = rec->elementMask;
    }
    g_pBattleCommandCtx->animParam = rec->animParam;
    g_pBattleCommandCtx->animParamAlt = rec->animParam;
    g_pBattleCommandCtx->resolvedAttackId = rec->resolvedAttackId;
    g_pBattleCommandCtx->specialFlags = rec->specialFlags;
    Battle_SetTargetFlagsIfUnset(rec->targetFlags);
    Battle_SetStatusChange(rec->statusChangeId, rec->statusMask);
    Battle_SetAddedEffect(rec->addedEffectId, rec->addedEffectParam);
}

/*
 * Sets action power to 16 and, for party actors (below three), copies the equipped weapon row into
 * the command context: attack id, damage formula, accuracy, display value and element mask, the
 * status mask (status change mode five), the camera value into animParam/animParamAlt where they
 * are still 0xFFFF, the special and target flags and three signed display values; then ORs the
 * actor's attackElements word into the element mask. For every attacker, Darkness halves the
 * accuracy, and a Frog's attack uses display values 0x16B, 0x16C and 5 with no secondary display.
 * An actor whose HP is exactly 7777 attacks with no element mask and no status masks.
 */
// FUNCTION: FF7 0x005c9c64
void __cdecl Battle_ApplyEquippedWeaponToAction(void)
{
    BattleWeaponActionRow *weaponRow;
    int displayIndex;

    g_pBattleCommandCtx->power = 0x10;
    if (g_pBattleCommandCtx->attackerSlot < 3) {
        weaponRow = &g_BattleState.weaponActionRows[g_pBattleCommandCtx->attackerSlot];
        g_pBattleCommandCtx->resolvedAttackId = weaponRow->resolvedAttackId;
        g_pBattleCommandCtx->damageCalcByte = weaponRow->damageCalcByte;
        g_pBattleCommandCtx->accuracy = weaponRow->accuracy;
        g_pBattleCommandCtx->secondaryDisplayValue = weaponRow->displayValue;
        g_pBattleCommandCtx->elementMask = weaponRow->elementMask;
        Battle_SetStatusChange(5, weaponRow->statusMask);
        if (g_pBattleCommandCtx->animParam == 0xFFFF) {
            g_pBattleCommandCtx->animParam = weaponRow->animParam;
        }
        if (g_pBattleCommandCtx->animParamAlt == 0xFFFF) {
            g_pBattleCommandCtx->animParamAlt = weaponRow->animParam;
        }
        g_pBattleCommandCtx->specialFlags = weaponRow->specialFlags;
        Battle_SetTargetFlagsIfUnset(weaponRow->targetFlags);
        for (displayIndex = 0; displayIndex < 3; displayIndex++) {
            (&g_pBattleCommandCtx->displayValueA)[displayIndex] = *(short *)((char *)weaponRow + displayIndex * 2 + 8);
        }
        g_pBattleCommandCtx->elementMask |= g_MenuPartyStats[g_pBattleCommandCtx->attackerSlot].attackElements;
    }
    if ((g_aCombatants[g_pBattleCommandCtx->attackerSlot].status & BATTLE_STATUS_DARKNESS) != 0) {
        g_pBattleCommandCtx->accuracy >>= 1;
    }
    if ((g_aCombatants[g_pBattleCommandCtx->attackerSlot].status & BATTLE_STATUS_FROG) != 0) {
        g_pBattleCommandCtx->displayValueA = 0x16B;
        g_pBattleCommandCtx->displayValueB = 0x16C;
        g_pBattleCommandCtx->displayValueC = 5;
        g_pBattleCommandCtx->secondaryDisplayValue = 0;
    }
    if (g_BattleState.actors[g_pBattleCommandCtx->attackerSlot].hp == 0x1E61) {
        g_pBattleCommandCtx->elementMask = 0;
        g_pBattleCommandCtx->statusMaskA = 0;
        g_pBattleCommandCtx->statusMaskB = 0;
        g_pBattleCommandCtx->statusMaskC = 0;
    }
}

/*
 * Negative command field 0x20 returns without allocation. Otherwise queues animation step with
 * selected command bytes, parameter short and zero flag, allocates hit record containing actor ID
 * twice and zero fields, then pushes empty hit record.
 */
// FUNCTION: FF7 0x005c9e8d
void __cdecl Battle_AttackOpQueueAnimOnly(void)
{
    BattleHitRecord *hitRecord;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleSeqStep *step;

    if (g_pBattleCommandCtx->stepId < 0) {
        return;
    }
    step = (BattleSeqStep *)Battle_AllocSequenceStep();
    step->actorIndex = (char)g_pBattleCommandCtx->attackerSlot;
    step->actionType = (char)g_pBattleCommandCtx->actionType;
    step->animId = (char)g_pBattleCommandCtx->stepId;
    step->stepFlags = (char)g_pBattleCommandCtx->stepFlags;
    step->effectId = (char)g_pBattleCommandCtx->resolvedAttackId;
    step->targetIndex = (short)g_pBattleCommandCtx->animParam;
    step->clearedFlag = 0;
    hitRecord = (BattleHitRecord *)Battle_AllocHitRecord();
    hitRecord->targetActor = (char)g_pBattleCommandCtx->attackerSlot;
    hitRecord->sourceActor = (char)g_pBattleCommandCtx->attackerSlot;
    hitRecord->animId = 0;
    hitRecord->flags = 0;
    Battle_PushEmptyHitRecord();
}

/*
 * Sets the command's repeatRemaining to four.
 */
// FUNCTION: FF7 0x005c9f46
void __cdecl Battle_AttackOpSetRepeat4(void)
{
    g_pBattleCommandCtx->repeatRemaining = 4;
}

/*
 * For each of the ten possible bits of the command's targetMask that is set, runs
 * Battle_RunAiScriptEvent(index, attackIdBase, 0).
 */
// FUNCTION: FF7 0x005c9f5a
void __cdecl Battle_AttackOpRunAiEventOnTargets(void)
{
    int i;

    for (i = 0; i < 10; i++) {
        if ((g_pBattleCommandCtx->targetMask & 1 << i) != 0) {
            Battle_RunAiScriptEvent(i, g_pBattleCommandCtx->attackIdBase, 0);
        }
    }
}

/*
 * Sets the command's repeatRemaining to two.
 */
// FUNCTION: FF7 0x005c9fac
void __cdecl Battle_AttackOpSetRepeat2(void)
{
    g_pBattleCommandCtx->repeatRemaining = 2;
}

/*
 * Sets the command's elementMask, power and targetFlags to 0x10, one and zero; if the attacker's
 * bit is absent from g_BattleScene.targetMask, sets stepId to minus one.
 */
// FUNCTION: FF7 0x005c9fc0
void __cdecl Battle_AttackOpMakeSlipDamage(void)
{
    g_pBattleCommandCtx->elementMask = 0x10;
    g_pBattleCommandCtx->power = 1;
    g_pBattleCommandCtx->targetFlags = 0;
    if ((g_BattleScene.targetMask & 1 << g_pBattleCommandCtx->attackerSlot) == 0) {
        g_pBattleCommandCtx->stepId = 0xFFFFFFFF;
    }
}

/*
 * Actor below three sets the command's stepId to minus one. Otherwise selects command one, clears targetFlags,
 * chooses target bit and replaces actor with highest-set-bit result, then sets pose 0x2F. Empty
 * mask semantics depend on helper.
 */
// FUNCTION: FF7 0x005ca014
void __cdecl Battle_AttackOpRedirectToEnemyActor(void)
{
    if (g_pBattleCommandCtx->attackerSlot < 3) {
        g_pBattleCommandCtx->stepId = 0xFFFFFFFF;
    } else {
        g_pBattleCommandCtx->commandIndex = 1;
        g_pBattleCommandCtx->targetFlags = 0;
        /* Retail caller treats `Battle_PickRandomTargetFromMask` return (`unsigned short`) as `int` for caller zero/sign extension. */
        g_pBattleCommandCtx->targetMask = ((int (__cdecl *)(short targetMask))Battle_PickRandomTargetFromMask)((short)g_pBattleCommandCtx->targetMask) & 0xFFFF;
        g_pBattleCommandCtx->attackerSlot = Battle_HighestSetBitIndex((int)g_pBattleCommandCtx->targetMask);
        g_pBattleCommandCtx->stepId = 0x2F;
    }
}

/*
 * Copies target mask and removes selected actors whose unsigned DWORD field 0x30 is at least 30000.
 * Empty mask posts formatted message 0x76 with flags 0x20 and currently uninitialized parameter
 * shorts. Otherwise clears ten per-actor weakness masks and tracks up to three distinct
 * unsigned-short actor IDs; only first occurrence of a remembered ID scans sixteen element bits
 * with Battle_FindElementAffinityIndex result below three, accumulating weakness count. IDs beyond
 * three unique slots are not remembered. Derives per-message duration from sixty or ninety divided
 * by two times target count plus weakness count, minimum four. For each target posts event (2,
 * actor, 0x16, 0), then message 0x74 with actor/optional scene byte/level, timed with 0x2D7; posts
 * message 0x72 or 0x75 depending on unsigned field 0x2A, with four combatant shorts, then one 0x6D
 * message per weakness bit using element index plus 0x5D. Subsequent target header flags change
 * from 0x20 to one.
 */
// FUNCTION: FF7 0x005ca093
void __cdecl Battle_AttackOpShowSenseReadout(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        int targetIndex;
        struct { unsigned char pad_to_4[0x4]; int senseMessage; };
        struct { unsigned char pad_to_8[0x8]; int uniqueId; };
        struct { unsigned char pad_to_C[0xC]; int targetMask; };
        struct { unsigned char pad_to_10[0x10]; int weaknessCount; };
        struct { unsigned char pad_to_14[0x14]; int idSlot; };
        struct { unsigned char pad_to_18[0x18]; int actorIndex; };
        struct { unsigned char pad_to_1C[0x1C]; int targetCount; };
        struct { unsigned char pad_to_20[0x20]; int durationDivisor; };
        struct { unsigned char pad_to_24[0x24]; int messageTiming; };
        struct { unsigned char pad_to_28[0x28]; short msgArg0; };
        struct { unsigned char pad_to_2A[0x2A]; short msgArg1; };
        struct { unsigned char pad_to_2C[0x2C]; short msgArg2; };
        struct { unsigned char pad_to_2E[0x2E]; short msgArg3; };
        struct { unsigned char pad_to_30[0x30]; int weaknessMasks[10]; };
        struct { unsigned char pad_to_58[0x58]; int msgFlags; };
        struct { unsigned char pad_to_5C[0x5C]; int rememberedIds[3]; };
    } frame;
#pragma pack(pop)

    frame.msgFlags = 0x20;
    frame.weaknessCount = 0;
    frame.targetCount = 0;
    frame.targetMask = (int)g_pBattleCommandCtx->targetMask;
    for (frame.actorIndex = 0; frame.actorIndex < 3; frame.actorIndex++) {
        frame.rememberedIds[frame.actorIndex] = 0xFFFFFFFF;
    }
    for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
        if ((frame.targetMask & 1 << frame.actorIndex) != 0 && (unsigned int)g_aCombatants[frame.actorIndex].maxHp >= (unsigned int)0x7530) {
            frame.targetMask &= ~(1 << frame.actorIndex);
        }
    }
    if (frame.targetMask == 0) {
        Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->attackerSlot, 0x76, frame.msgFlags, &frame.msgArg0);
    } else {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            frame.weaknessMasks[frame.actorIndex] = 0;
            if ((frame.targetMask & 1 << frame.actorIndex) != 0) {
                frame.uniqueId = g_aCombatants[frame.actorIndex].uniqueId;
                for (frame.idSlot = 0; frame.idSlot < 3; frame.idSlot++) {
                    if (frame.uniqueId == frame.rememberedIds[frame.idSlot]) {
                        break;
                    }
                }
                if (frame.idSlot == 3) {
                    for (frame.idSlot = 0; frame.idSlot < 3; frame.idSlot++) {
                        if (frame.rememberedIds[frame.idSlot] == -1) {
                            frame.rememberedIds[frame.idSlot] = frame.uniqueId;
                            break;
                        }
                    }
                    for (frame.idSlot = 0; frame.idSlot < 0x10; frame.idSlot++) {
                        if (Battle_FindElementAffinityIndex(frame.actorIndex, 1 << frame.idSlot, 0) < 3) {
                            frame.weaknessMasks[frame.actorIndex] |= 1 << frame.idSlot;
                            frame.weaknessCount++;
                        }
                    }
                }
                frame.targetCount++;
            }
        }
        frame.messageTiming = frame.targetCount > 1 ? 0x5A : 0x3C;
        frame.durationDivisor = frame.weaknessCount + frame.targetCount * 2;
        if (frame.durationDivisor != 0) {
            frame.messageTiming = frame.messageTiming / frame.durationDivisor;
        }
        if (frame.messageTiming < 4) {
            frame.messageTiming = 4;
        }
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((frame.targetMask & 1 << frame.actorIndex) != 0) {
                frame.targetIndex = frame.actorIndex;
                Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, frame.actorIndex, BATTLE_ACTION_EVENT_SET_TIMING_FLAG_40, 0);
                frame.msgArg0 = (short)frame.targetIndex;
                frame.msgArg1 = 0xFFFF;
                if ((unsigned char)g_BattleState.actors[frame.targetIndex].nameSuffixIndex != 0xFF) {
                    frame.msgArg1 = (unsigned char)g_BattleState.actors[frame.targetIndex].nameSuffixIndex;
                }
                frame.msgArg2 = g_aCombatants[frame.targetIndex].level;
                Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->attackerSlot, 0x74, frame.msgFlags, &frame.msgArg0);
                Battle_SetSequenceStepMessageTiming(frame.messageTiming, 0x2D7);
                frame.msgArg0 = (short)g_aCombatants[frame.targetIndex].curHp;
                frame.msgArg1 = (short)g_aCombatants[frame.targetIndex].maxHp;
                frame.msgArg2 = (short)g_aCombatants[frame.targetIndex].curMp;
                frame.msgArg3 = (short)g_aCombatants[frame.targetIndex].maxMp;
                frame.senseMessage = ((unsigned short)g_aCombatants[frame.targetIndex].maxMp != 0 ? 3 : 0) + 0x72;
                Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->attackerSlot, frame.senseMessage, 1, &frame.msgArg0);
                Battle_SetSequenceStepMessageTiming(frame.messageTiming, 0);
                for (frame.idSlot = 0; frame.idSlot < 0x10; frame.idSlot++) {
                    if ((frame.weaknessMasks[frame.actorIndex] & 1 << frame.idSlot) != 0) {
                        frame.msgArg0 = frame.idSlot + 0x5D;
                        Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->attackerSlot, 0x6D, 1, &frame.msgArg0);
                        Battle_SetSequenceStepMessageTiming(frame.messageTiming, 0);
                    }
                }
                frame.msgFlags = 1;
            }
        }
    }
}

/*
 * Attempts to replay two stored battle actions for the current attacker, clearing the two
 * party-record action slots first. Ignores absent actions and BATTLE_COMMAND_MIME; BATTLE_COMMAND_LIMIT
 * is replayed only when its attack identifier appears among the attacker character limit attacks, otherwise
 * records message 0x57 and effect step 0x35 with an adjusted attack parameter. Other accepted
 * actions enqueue with command bit 0x40 and mode one or four, increment the global counter, and
 * copy saved slot values. If none enqueue, posts the recorded message or default 0x58 and queues
 * the chosen effect step; default step 3 additionally queues steps 0x3B and 4. Explicit packed
 * SHORT message parameters share storage correctly.
 */
// FUNCTION: FF7 0x005ca465
void __cdecl Battle_AttackOpReplayLastActions(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        int unitIndex;
        struct { unsigned char pad_to_4[0x4]; int extraWord; };
        struct { unsigned char pad_to_8[0x8]; int messageId; };
        struct { unsigned char pad_to_C[0xC]; int attackerSlot; };
        struct { unsigned char pad_to_10[0x10]; int enqueuedCount; };
        struct { unsigned char pad_to_14[0x14]; short actorSlot; };
        struct { unsigned char pad_to_16[0x16]; short adjustedAttack; };
        struct { unsigned char pad_to_18[0x18]; int enqueueMode; };
        struct { unsigned char pad_to_1C[0x1C]; int limitIndex; };
        struct { unsigned char pad_to_20[0x20]; int slotIndex; };
        struct { unsigned char pad_to_24[0x24]; int paramWord; };
        struct { unsigned char pad_to_28[0x28]; int effectStep; };
        struct { unsigned char pad_to_2C[0x2C]; int commandId; };
    } frame;
#pragma pack(pop)

    frame.attackerSlot = g_pBattleCommandCtx->attackerSlot;
    frame.actorSlot = (short)frame.attackerSlot;
    frame.enqueuedCount = 0;
    frame.messageId = 0xFFFFFFFF;
    frame.effectStep = 3;
    frame.unitIndex = g_BattleScene.actors[frame.attackerSlot].unitIndex;
    for (frame.slotIndex = 0; frame.slotIndex < 2; frame.slotIndex++) {
        g_BattleState.partyStats[frame.attackerSlot].queuedSupportCodes[frame.slotIndex] = 0xFFFF;
        if (g_BattleState.activeActions[frame.slotIndex].actor != -1) {
            frame.commandId = g_BattleState.activeActions[frame.slotIndex].commandId & 0x3F;
            frame.paramWord = g_BattleState.activeActions[frame.slotIndex].param;
            frame.extraWord = g_BattleState.activeActions[frame.slotIndex].extra;
            switch (frame.commandId) {
            case BATTLE_COMMAND_MIME:
                break;
            case BATTLE_COMMAND_LIMIT:
                for (frame.limitIndex = 0; frame.limitIndex < 12; frame.limitIndex++) {
                    if (Kernel_GetLimitAttackId(frame.unitIndex, frame.limitIndex) == frame.paramWord) {
                        break;
                    }
                }
                if (frame.limitIndex == 12) {
                    g_pBattleCommandCtx->stepPower = frame.paramWord;
                    frame.adjustedAttack = frame.paramWord + 0x80;
                    frame.messageId = 0x57;
                    frame.effectStep = 53;
                    break;
                }
            default:
                frame.enqueueMode = g_BattleState.runningActionPriority;
                if (frame.enqueueMode != 1) {
                    frame.enqueueMode = 4;
                }
                g_BattleReplayActionCount++;
                Battle_EnqueueActionDirect(frame.attackerSlot, frame.enqueueMode, frame.commandId | 0x40, frame.paramWord, frame.extraWord);
                g_BattleState.partyStats[frame.attackerSlot].queuedSupportCodes[frame.slotIndex] = (&g_BattleState.actor)[frame.slotIndex];
                frame.enqueuedCount++;
            }
        }
    }
    if (frame.enqueuedCount == 0) {
        if (frame.messageId == -1) {
            frame.messageId = 0x58;
        }
        Battle_QueueCommandEffectStep(frame.effectStep);
        Battle_PostBattleMessageFormatted(frame.attackerSlot, frame.messageId, 1, &frame.actorSlot);
        if (frame.effectStep == 3) {
            Battle_QueueCommandEffectStep(0x3B);
            Battle_QueueCommandEffectStep(4);
        }
    }
}

/*
 * ORs BATTLE_STATUS_DEATH into the command's statusMaskA, sets statusChangeId to 0xFF and
 * damageCalcByte to 0xB0, then halves accuracy.
 */
// FUNCTION: FF7 0x005ca651
void __cdecl Battle_AttackOpMakeInstantDeath(void)
{
    g_pBattleCommandCtx->statusMaskA |= BATTLE_STATUS_DEATH;
    g_pBattleCommandCtx->statusChangeId = 0xFF;
    g_pBattleCommandCtx->damageCalcByte = 0xB0;
    g_pBattleCommandCtx->accuracy >>= 1;
}

/*
 * Signed-divides the command's accuracy by three toward zero and clears bit 0x2000 of specialFlags
 * (VC5's CH-lane AND).
 */
// FUNCTION: FF7 0x005ca6a0
void __cdecl Battle_AttackOpMakeDeathblow(void)
{
    g_pBattleCommandCtx->accuracy = g_pBattleCommandCtx->accuracy / 3;
    g_pBattleCommandCtx->specialFlags = (int)g_pBattleCommandCtx->specialFlags & ~BATTLE_SPECIAL_NO_FORCED_CRITICAL;
}

/*
 * Sets the command's power to two.
 */
// FUNCTION: FF7 0x005ca6d2
void __cdecl Battle_AttackOpSetPower2(void)
{
    g_pBattleCommandCtx->power = 2;
}

/*
 * Applies the active actor's weapon-impact flags when target flags are still unset.
 */
// FUNCTION: FF7 0x005ca6e3
void __cdecl Battle_AttackOpSetWeaponImpact(void)
{
    Battle_SetTargetFlagsIfUnset(g_BattleState.weaponActionRows[g_pBattleCommandCtx->attackerSlot].targetFlags);
}

/*
 * Stores target flags only while the current command retains its unset sentinel.
 */
// FUNCTION: FF7 0x005ca703
void __cdecl Battle_SetTargetFlagsIfUnset(int flags)
{
    if ((int)g_pBattleCommandCtx->targetFlags == 0xFF) {
        g_pBattleCommandCtx->targetFlags = flags;
    }
}

/*
 * Sets the command's effectId to minus one; for an effectId other than 0xff stores effectId and
 * effectParam in the command and invokes special attack effect mode two. Leaves effectParam
 * unchanged for the sentinel.
 */
// FUNCTION: FF7 0x005ca722
void __cdecl Battle_SetAddedEffect(int effectId, int effectParam)
{
    g_pBattleCommandCtx->effectId = 0xFFFFFFFF;
    if (effectId != 0xFF) {
        g_pBattleCommandCtx->effectId = effectId;
        g_pBattleCommandCtx->effectParam = effectParam;
        Battle_RunSpecialAttackEffect(2);
    }
}

/*
 * Clears three status masks, derives selector from unsigned mode shifted six and only selectors
 * below three write. High status bit stores special mask and low-two-bit global; otherwise sets
 * rate from low-six mode bits and selected mask.
 */
// FUNCTION: FF7 0x005ca766
void __cdecl Battle_SetStatusChange(int mode, int statusMask)
{
    int rate;
    int selector;

    selector = (unsigned int)mode >> 6;
    g_pBattleCommandCtx->statusMaskA = 0;
    g_pBattleCommandCtx->statusMaskB = 0;
    g_pBattleCommandCtx->statusMaskC = 0;
    if (selector < 3) {
        rate = (mode & 0x3F) << 2;
        if ((statusMask & 0x80000000) != 0) {
            g_pBattleCommandCtx->statusMaskA = 0x80000000;
            g_BattleState.statusChangeMode = statusMask & 3;
        } else {
            g_pBattleCommandCtx->statusChangeId = rate;
            (&g_pBattleCommandCtx->statusMaskA)[selector] = statusMask;
        }
    }
}

/*
 * A one-line forwarder: calls Kernel_ApplyAttackToAction
 * and does nothing else.
 */
// FUNCTION: FF7 0x005ca807
void __cdecl Battle_ReapplyAttackRecord(void)
{
    Kernel_ApplyAttackToAction();
}

/*
 * Stores repeatCount at command 0xF8 and repeatCount+three at 0xAC, upper-clamping latter eight
 * only. Negative values retained.
 */
// FUNCTION: FF7 0x005ca811
void __cdecl Battle_SetRepeatCasts(int repeatCount)
{
    g_pBattleCommandCtx->repeatCount = repeatCount;
    g_pBattleCommandCtx->repeatCasts = g_pBattleCommandCtx->repeatCount + 3;
    if (g_pBattleCommandCtx->repeatCasts > 8) {
        g_pBattleCommandCtx->repeatCasts = 8;
    }
}
} /* extern "C" */

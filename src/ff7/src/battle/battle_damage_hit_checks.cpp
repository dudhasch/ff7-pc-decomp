// src/ff7/src/battle/battle_damage_hit_checks.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Damage operations that decide hits: physical and magical hit checks, critical hit
//   rolls, the manipulate target check, attack facing and facing flip, and defence and accuracy
//   modifiers.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
extern "C" {

#define g_aBattlePartyStat (g_BattleState.partyStats)

/* The element reactions under which a hit cannot miss: Death, auto-hit, Nullify and Absorb
 * (the element's effect, not the roll, decides the hit). */
#define BATTLE_ELEM_REACTION_NEVER_MISSES \
    (BATTLE_ELEM_REACTION_DEATH | BATTLE_ELEM_REACTION_AUTO_HIT | BATTLE_ELEM_REACTION_NULLIFY | BATTLE_ELEM_REACTION_ABSORB)
/* A chance this high always hits. */
#define BATTLE_HIT_ALWAYS 0xFF

/*
 * Damage op 0: the physical hit check (Attack and the other physical attacks).
 *
 * Some hits cannot miss: a hit on the target's back, an element reaction that decides the hit
 * (BATTLE_ELEM_REACTION_NEVER_MISSES), a target that cannot dodge (KO, Sleep, Confusion, Stop,
 * Petrify, Manipulate, Paralysis), and a hit an ally took in Cover. A physical hit also wakes the
 * target from Sleep and ends its Confusion and Manipulate (workCureMask).
 *
 * Otherwise the chance, in percent, is the attack's accuracy + Dex / 4 + the attacker's Def% -
 * the target's Def% (Battle_GetActorDefensePercent), less 30% under Fury, at least 1. This is the
 * Battle Mechanics FAQ's physical hit formula (well established), including its oddity that the
 * attacker's own evasion adds to its accuracy.
 *
 * A lucky roll overrides it: one draw of 0..99 below the attacker's Luck / 4 always hits; failing
 * that, the same draw below the target's Luck / 4 makes an enemy's attack on a party member miss.
 * The hit roll is 1..100 against the chance with "<", so a chance of exactly 100 still misses on
 * a roll of 100. A hit lets the target turn to face the attacker (Battle_CommitFacingFlip); a
 * miss sets BATTLE_RESULT_MISS.
 */
// FUNCTION: FF7 0x005ddbb0
void __cdecl Battle_DmgOpPhysicalHitCheck(void)
{
    int roll;
    int chance;
    int hitRoll;
    int dexModified;
    BattleCombatant *targetRec;
    int defSource;
    int targetDefense;
    BattleCombatant *source;
    BattleActorRuntime *targetStatus;
    BattleActorRuntime *sourceState;
    int commandAccuracy;

    sourceState = &g_BattleState.actors[g_pBattleCommandCtx->attackerSlot];
    targetStatus = &g_BattleState.actors[g_pBattleCommandCtx->targetSlot];
    source = &g_aCombatants[g_pBattleCommandCtx->attackerSlot];
    targetRec = &g_aCombatants[g_pBattleCommandCtx->targetSlot];
    Battle_ResolveAttackFacing();
    chance = 0xFFFFFFFF;
    if ((g_pBattleCommandCtx->modifierFlags & BATTLE_MODIFIER_BACK_ATTACK) != 0) {
        chance = BATTLE_HIT_ALWAYS;
    }
    if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_NEVER_MISSES) != 0) {
        chance = BATTLE_HIT_ALWAYS;
    }
    // A target that cannot dodge; the blow wakes it and snaps it out of Confusion or Manipulate.

    if ((g_pBattleCommandCtx->targetStatus & (BATTLE_STATUS_DEATH | BATTLE_STATUS_SLEEP | BATTLE_STATUS_CONFUSION | BATTLE_STATUS_STOP | BATTLE_STATUS_PETRIFY | BATTLE_STATUS_MANIPULATE | BATTLE_STATUS_PARALYSIS)) != 0) {
        if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_SLEEP) != 0) {
            g_pBattleCommandCtx->workCureMask |= BATTLE_STATUS_SLEEP;
        }
        if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_CONFUSION) != 0) {
            g_pBattleCommandCtx->workCureMask |= BATTLE_STATUS_CONFUSION;
        }
        if ((g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_MANIPULATE) != 0) {
            g_pBattleCommandCtx->workCureMask |= BATTLE_STATUS_MANIPULATE;
        }
        chance = BATTLE_HIT_ALWAYS;
    }
    if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_COVERED) != 0) {
        chance = BATTLE_HIT_ALWAYS;
    }
    dexModified = Battle_ApplyStatPercentModifier(g_pBattleCommandCtx->attackerSlot, source->dexterity, 5);
    commandAccuracy = g_pBattleCommandCtx->accuracyWork + ((dexModified) / 4);
    defSource = Battle_GetActorDefensePercent(g_pBattleCommandCtx->attackerSlot);
    targetDefense = Battle_GetActorDefensePercent(g_pBattleCommandCtx->targetSlot);
    if (chance == -1) {
        chance = commandAccuracy + defSource - targetDefense;
        chance = Battle_ApplyFuryAccuracyPenalty(chance);
    }
    if (chance <= 0) {
        chance = 1;
    }
    /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
    roll = ((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(0x64);
    // Lucky hit for the attacker, or a lucky dodge for a party member an enemy attacks.
    if (roll < (g_aCombatants[g_pBattleCommandCtx->attackerSlot].luck) / 4) {
        chance = BATTLE_HIT_ALWAYS;
    } else if (g_pBattleCommandCtx->attackerSlot >= 4 && g_pBattleCommandCtx->targetSlot < 3 && roll < (g_aCombatants[g_pBattleCommandCtx->targetSlot].luck) / 4) {
        chance = 0;
    }
    hitRoll = Battle_Roll1To100();
    if (hitRoll < chance) {
        Battle_CommitFacingFlip();
    } else {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
    }
}

/*
 * Damage op 1: the magic hit check. Both rolls are drawn first, whatever happens after.
 *
 * It cannot miss with an accuracy of 255 or more, under an element reaction that decides the hit,
 * against a Reflect target when the attack can be reflected (the bounce decides where it goes),
 * or when the spell inflicts no status and the target cannot dodge (KO, Sleep, Confusion, Stop,
 * Petrify, Paralysis). Otherwise it misses when 0..99 falls below the target's magic evade
 * (magicDefensePercent), or when 1..100 reaches the attack's accuracy, less 30% under Fury, plus
 * the attacker's level - half the target's level. This is the FAQ's magic hit formula (well
 * established).
 */
// FUNCTION: FF7 0x005dde5e
void __cdecl Battle_DmgOpMagicalHitCheck(void)
{
    int levelGap;
    int accuracy;
    int evadeRoll;
    int secondRoll;

    accuracy = g_pBattleCommandCtx->accuracyWork;
    levelGap = g_pBattleCommandCtx->attackerLevel - ((g_aCombatants[g_pBattleCommandCtx->targetSlot].level) / 2);
    /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
    evadeRoll = ((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(0x64);
    /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
    secondRoll = ((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(0x64) + 1;
    if (accuracy >= BATTLE_HIT_ALWAYS) {
        return;
    }
    if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_NEVER_MISSES) != 0) {
        return;
    }
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_REFLECTABLE) == 0 && (g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_REFLECT) != 0) { /* empty first arm: lib/codegen.h */
    } else if (g_pBattleCommandCtx->statusMaskA == 0 && (g_pBattleCommandCtx->targetStatus & (BATTLE_STATUS_DEATH | BATTLE_STATUS_SLEEP | BATTLE_STATUS_CONFUSION | BATTLE_STATUS_STOP | BATTLE_STATUS_PETRIFY | BATTLE_STATUS_PARALYSIS)) != 0) { /* empty first arm: lib/codegen.h */
    } else {
        accuracy = Battle_ApplyFuryAccuracyPenalty(accuracy);
        if (evadeRoll < g_aCombatants[g_pBattleCommandCtx->targetSlot].magicDefensePercent || secondRoll >= accuracy + levelGap) {
            g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
        }
    }
}

/*
 * Damage op 2: the critical hit roll, after a physical hit check that did not miss. The chance in
 * percent is (Luck + level - the target's level) / 4, plus the weapon's critical bonus for a
 * party member; Lucky Girl makes every hit critical. A roll of 1..100 at or below it is a
 * critical hit, which the physical formula doubles. This is the FAQ's critical formula (well
 * established). The two actor-record pointers it computes first are never used.
 */
// FUNCTION: FF7 0x005ddf8e
void __cdecl Battle_RollCriticalHit(void)
{
    int roll;
    BattleActorRuntime *targetState;
    int chance;
    BattleActorRuntime *attackerRecord;

    attackerRecord = &g_BattleState.actors[g_pBattleCommandCtx->attackerSlot];
    targetState = &g_BattleState.actors[g_pBattleCommandCtx->targetSlot];
    if ((g_pBattleCommandCtx->resultFlags & BATTLE_RESULT_MISS) != 0) {
        return;
    }
    if ((g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_LUCKY_GIRL) != 0) {
        chance = BATTLE_HIT_ALWAYS;
    } else {
        chance = (g_pBattleCommandCtx->attackerLevel + g_aCombatants[g_pBattleCommandCtx->attackerSlot].luck - g_aCombatants[g_pBattleCommandCtx->targetSlot].level) / 4;
        if (g_pBattleCommandCtx->attackerSlot < 3) {
            chance += g_BattleState.weaponActionRows[g_pBattleCommandCtx->attackerSlot].criticalBonus;
        }
    }
    roll = Battle_Roll1To100();
    if (roll <= chance) {
        g_pBattleCommandCtx->displayFlags |= BATTLE_DISPLAY_CRITICAL;
    }
}

/*
 * Damage op 7: the level check of the "Level N" attacks (L5 Death, L4 Suicide, L3 Flare: well
 * established). The attack's accuracy holds N, and it hits only targets whose level is a multiple
 * of N; an accuracy of 0 hits everyone.
 */
// FUNCTION: FF7 0x005de08a
void __cdecl Battle_DmgOpLevelDivisibleCheck(void)
{
    if (g_pBattleCommandCtx->accuracy != 0 &&
        g_pBattleCommandCtx->targetLevel % g_pBattleCommandCtx->accuracy != 0) {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
    }
}

/*
 * Damage op 6: the Manipulate check. Manipulate takes control of an enemy only if no party member
 * controls it yet, the caster controls no one, the enemy is not already under Manipulate, can act
 * (its ATB is running) and does not nullify the attack's element.
 *
 * The chance in percent is the caster's level + 50 - the target's level (at least 0); four fifths
 * of that against several targets, half on a repeated cast, certain under an auto-hit element
 * reaction, and at least 100 with accessory kind 5. On success the caster's party slot records
 * the enemy (manipulatedSlot) and battle message 0x5A names it; otherwise the attack misses.
 */
// FUNCTION: FF7 0x005de0d0
void __cdecl Battle_DmgOpManipulateCheck(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        int chance;
        struct { unsigned char pad_to_4[0x4]; int targetSlot; };
        struct { unsigned char pad_to_8[0x8]; int partyIndex; };
        struct { unsigned char pad_to_C[0xC]; short targetId; };
        struct { unsigned char pad_to_E[0xE]; short linkedSlot; };
        struct { unsigned char pad_to_10[0x10]; int succeeded; };
    } frame;
#pragma pack(pop)

    frame.succeeded = 0;
    frame.targetSlot = g_pBattleCommandCtx->targetSlot;
    for (frame.partyIndex = 0; frame.partyIndex < 3; frame.partyIndex++) {
        if (g_aBattlePartyStat[frame.partyIndex].manipulatedSlot == frame.targetSlot) {
            break;
        }
    }
    if (frame.partyIndex == 3 && frame.targetSlot >= 4 && g_pBattleCommandCtx->attackerSlot < 3 && g_aBattlePartyStat[g_pBattleCommandCtx->attackerSlot].manipulatedSlot == 0 && (g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_MANIPULATE) == 0 && g_BattleState.actors[frame.targetSlot].atbTick != 0 && (g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_NULLIFY) == 0) {
        frame.chance = g_pBattleCommandCtx->attackerLevel + 0x32 - g_pBattleCommandCtx->targetLevel;
        if (frame.chance < 0) {
            frame.chance = 0;
        }
        if (g_pBattleCommandCtx->targetCount > 1) {
            frame.chance = (frame.chance << 2) / 5;
        }
        if (g_pBattleCommandCtx->repeatCasts != 0) {
            frame.chance >>= 1;
        }
        if ((g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_AUTO_HIT) != 0) {
            frame.chance = BATTLE_HIT_ALWAYS;
        }
        if ((unsigned char)g_BattleState.actors[g_pBattleCommandCtx->attackerSlot].accessoryKind == 5 && frame.chance < 100) {
            frame.chance = 0x64;
        }
        /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
        if (((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(0x64) < frame.chance) {
            frame.succeeded = 1;
        }
    }
    if (frame.succeeded != 0) {
        frame.targetId = *(short *)&frame.targetSlot;
        frame.linkedSlot = 0xFFFF;
        if ((*(unsigned char *)&g_BattleState.actors[frame.targetSlot].nameSuffixIndex) != 0xFF) {
            frame.linkedSlot = (*(unsigned char *)&g_BattleState.actors[frame.targetSlot].nameSuffixIndex);
        }
        g_aBattlePartyStat[g_pBattleCommandCtx->attackerSlot].manipulatedSlot = *(char *)&frame.targetSlot;
        Battle_PostBattleMessageFormatted(g_pBattleCommandCtx->attackerSlot, 0x5A, 1, &frame.targetId);
        g_pBattleCommandCtx->payload = 0xFFFFFFFF;
    } else {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MISS;
    }
}

/*
 * Damage-formula table entry that does nothing.
 */
// FUNCTION: FF7 0x005de2ec
void Battle_DmgOpNop(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x005de2ec,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * A combatant's physical evasion (Def%) for the hit check: its armour's Def% plus Dex / 4 for a
 * party member (an enemy's Def% comes from its record as is), with its Def% percent bonus.
 */
// FUNCTION: FF7 0x005de2f1
int __cdecl Battle_GetActorDefensePercent(int actorIndex)
{
    int defense;

    if (actorIndex < 4) {
        defense = ((g_aCombatants[actorIndex].dexterity) / 4) + g_aCombatants[actorIndex].defensePercent;
    } else {
        defense = g_aCombatants[actorIndex].defensePercent;
    }
    defense = Battle_ApplyStatPercentModifier(actorIndex, defense, 4);
    return defense;
}

/*
 * Decides which way attacker and target face for this hit, and whether the hit lands on the
 * target's back. In a side or pincer attack the combatants stand in three groups (left, centre,
 * right; g_BattleScene.targetMaskTable); an attacker in the centre group first turns towards its
 * target. A hit from the other group on a target that faces away (equal facing parity) is a back
 * attack (BATTLE_MODIFIER_BACK_ATTACK): it always hits and the physical formula applies the
 * target's back-attack multiplier. The attacker's turn is applied at once; the target's turn
 * waits for the hit check (BATTLE_MODIFIER_TURN_TARGET, Battle_CommitFacingFlip).
 */
// FUNCTION: FF7 0x005de356
void __cdecl Battle_ResolveAttackFacing(void)
{
    int facingMask;
    int tgtGroup;
    int attackerGroup;
    int attackerBit;
    int originalBits;
    int groupIdx;
    int targetBit;
    int parity;
    int members;
    int swapFlag;

    attackerGroup = 0;
    tgtGroup = 0;
    parity = 0;
    attackerBit = 1 << g_pBattleCommandCtx->attackerSlot;
    targetBit = 1 << g_pBattleCommandCtx->targetSlot;
    facingMask = 0;
    if ((g_aCombatants[g_pBattleCommandCtx->attackerSlot].flags & BATTLE_COMBATANT_FACING_FLIPPED) != 0) {
        facingMask |= attackerBit;
    }
    if ((g_aCombatants[g_pBattleCommandCtx->targetSlot].flags & BATTLE_COMBATANT_FACING_FLIPPED) != 0) {
        facingMask |= targetBit;
    }
    originalBits = facingMask;
    for (groupIdx = 0; groupIdx < 3; groupIdx++) {
        members = g_BattleScene.targetMaskTable[groupIdx];
        if ((members & attackerBit) != 0) {
            attackerGroup = groupIdx;
        }
        if ((members & targetBit) != 0) {
            tgtGroup = groupIdx;
        }
    }
    if (attackerGroup == 1) {
        swapFlag = 0;
        switch (tgtGroup) {
        case BATTLE_TARGET_GROUP_SIDE_A:
            swapFlag = 1;
        case BATTLE_TARGET_GROUP_SIDE_B:
            if ((facingMask & attackerBit) != 0) {
                swapFlag ^= 1;
            }
            if (swapFlag != 0) {
                facingMask ^= attackerBit;
            }
        }
    }
    if ((attackerBit & facingMask) != 0) {
        parity ^= 1;
    }
    if ((targetBit & facingMask) != 0) {
        parity ^= 1;
    }
    if (attackerGroup != tgtGroup && parity == 0) {
        g_pBattleCommandCtx->modifierFlags |= BATTLE_MODIFIER_BACK_ATTACK;
        facingMask ^= targetBit;
    }
    facingMask ^= originalBits;
    if ((facingMask & attackerBit) != 0) {
        g_aCombatants[g_pBattleCommandCtx->attackerSlot].flags ^= BATTLE_COMBATANT_FACING_FLIPPED;
    }
    if ((facingMask & targetBit) != 0) {
        g_pBattleCommandCtx->modifierFlags |= BATTLE_MODIFIER_TURN_TARGET;
    }
}

/*
 * If the hit raised BATTLE_MODIFIER_TURN_TARGET, toggle the target's
 * facing bit so the model now looks the other way.
 */
// FUNCTION: FF7 0x005de53b
void __cdecl Battle_CommitFacingFlip(void)
{
    if (g_pBattleCommandCtx->modifierFlags & BATTLE_MODIFIER_TURN_TARGET) {
        g_aCombatants[g_pBattleCommandCtx->targetSlot].flags ^= BATTLE_COMBATANT_FACING_FLIPPED;
    }
}

/*
 * Fury: the attacker's hit chance drops by 30% (a sure hit, 255, stays sure).
 */
// FUNCTION: FF7 0x005de582
int __cdecl Battle_ApplyFuryAccuracyPenalty(int hitRate)
{
    if ((hitRate < BATTLE_HIT_ALWAYS) && (g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_FURY)) {
        hitRate -= (hitRate * 3) / 10;
    }
    return hitRate;
}

// The eight damage stat-op handlers Battle_RunDamageProgram calls by op number
// (battle_damage_program_run.cpp).
typedef void (__cdecl *BattleDmgOpFn)(void);
// GLOBAL: FF7 0x007b77a8
const BattleDmgOpFn g_BattleDamageOpTable[8] = {
    Battle_DmgOpPhysicalHitCheck, Battle_DmgOpMagicalHitCheck, Battle_RollCriticalHit, Battle_DmgOpNop,
    Battle_DmgOpNop, Battle_DmgOpNop, Battle_DmgOpManipulateCheck, Battle_DmgOpLevelDivisibleCheck};
} /* extern "C" */

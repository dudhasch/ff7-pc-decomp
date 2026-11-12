// src/ff7/src/battle/battle_damage_formula_set.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The battle damage formula functions: physical, magical and percent-based formulas with
//   split, barrier and variance modifiers, plus dice, counters and stubs for formulas the game does
//   not use.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/types/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
extern "C" {

#define g_aBattlePartyStat (g_BattleState.partyStats)

/*
 * How the damage formulas work.
 *
 * An attack's damage calculation byte picks a hit-check program (its high nibble) and one of
 * these formulas (its low nibble, plus 0x10 for programs 6 and 7); see Battle_DecodeDamageCalcByte
 * in battle_damage_program_run.cpp. Battle_RunDamageProgram runs the checks, then the formula for
 * each target, which leaves the amount in the command context's damage. Elements, healing, drain,
 * the 9999 cap and the HP/MP change come afterwards (battle_hit_damage_resolution.cpp).
 *
 * power is in sixteenths: 16 is the attack's base strength. The two standard formulas, physical
 * (1) and magical (2), match the community's Battle Mechanics FAQ (well established):
 *   physical base = Att + [(Lvl + Att) / 32] * [(Lvl * Att) / 32]
 *   magical base  = 6 * (MAt + Lvl)
 *   damage        = base * (512 - Def) * power / (16 * 512)
 * and then share the modifiers below: Sadness, the multi-target split, Barrier/MBarrier, the
 * damage bonus, and the random variance of 3841/4096 to 4096/4096.
 */

/*
 * Damage formula 0: no damage; the hit shows no number.
 */
// FUNCTION: FF7 0x005de5c0
void __cdecl Battle_DmgFormula00_NoDamage(void)
{
    g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_NO_DAMAGE_DISPLAY;
}

/*
 * Damage formula 1: the standard physical attack (Attack, most physical skills and enemy blows).
 *
 * The base and the defence step are the FAQ's (see the top of the file); attackStat is the
 * attacker's Attack with its percent bonus (Battle_DecodeDamageCalcByte). Then, in order:
 * - a critical hit doubles it. Battle_RollCriticalHit rolls for one; an attack that clears
 *   BATTLE_SPECIAL_NO_FORCED_CRITICAL (Deathblow) is always critical;
 * - Berserk: x3/2;
 * - row: a short-range attack (BTL_TARGETF_SHORT_RANGE; Long Range materia clears it) and any
 *   enemy attack (command 0x20) are halved when the target or the attacker stands in the back
 *   row; other attacks ignore rows;
 * - a Defending target halves it;
 * - a hit on the target's back (BATTLE_MODIFIER_BACK_ATTACK, Battle_ResolveAttackFacing) scales
 *   it by the target's damageScale / 8: 0x10, double damage, for every party member, the
 *   enemy's own value from its scene record otherwise;
 * - a Frog attacker deals a quarter;
 * - Sadness, the multi-target split, Barrier and the damage bonus;
 * - a Mini attacker deals 0 here, which the variance step turns into 1;
 * - the random variance.
 */
// FUNCTION: FF7 0x005de5df
void __cdecl Battle_DmgFormula01_Physical(void)
{
    int level;
    int rowFlag;
    int res;
    int baseDamage;
    int attackStat;

    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NO_FORCED_CRITICAL) == 0) {
        g_pBattleCommandCtx->displayFlags |= BATTLE_DISPLAY_CRITICAL;
    }
    attackStat = g_pBattleCommandCtx->attackStat;
    level = g_pBattleCommandCtx->attackerLevel;
    baseDamage = ((level * attackStat) / 32) * ((level + attackStat) / 32) + attackStat;
    res = (baseDamage * (0x200 - g_pBattleCommandCtx->defence) * g_pBattleCommandCtx->power) / 8192;
    // Critical hit: double damage.
    if ((g_pBattleCommandCtx->displayFlags & BATTLE_DISPLAY_CRITICAL) != 0) {
        res <<= 1;
    }
    if ((g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_BERSERK) != 0) {
        res *= 3;
        res >>= 1;
    }
    // Rows count only for short-range attacks and enemy attacks: either side in the back row halves it.
    rowFlag = 0;
    if ((g_aCombatants[g_pBattleCommandCtx->targetSlot].flags & BATTLE_COMBATANT_BACK_ROW) != 0) {
        rowFlag = 1;
    }
    if ((g_pBattleCommandCtx->targetFlags & BTL_TARGETF_SHORT_RANGE) != 0 || g_pBattleCommandCtx->stepFlags == BATTLE_COMMAND_ENEMY_ATTACK) {
        if ((g_aCombatants[g_pBattleCommandCtx->attackerSlot].flags & BATTLE_COMBATANT_BACK_ROW) != 0) {
            rowFlag = 1;
        }
    } else {
        rowFlag = 0;
    }
    if (rowFlag != 0) {
        res = (res) / 2;
    }
    if ((g_aCombatants[g_pBattleCommandCtx->targetSlot].flags & BATTLE_COMBATANT_DEFENDING) != 0) {
        res = (res) / 2;
    }
    // Hit from behind: the target's back-attack multiplier, in eighths.
    if ((g_pBattleCommandCtx->modifierFlags & BATTLE_MODIFIER_BACK_ATTACK) != 0) {
        res = res * g_aCombatants[g_pBattleCommandCtx->targetSlot].damageScale >> 3;
    }
    if ((g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_FROG) != 0) {
        res >>= 2;
    }
    res = Battle_ApplySadnessReduction(res);
    res = Battle_ApplyMultiTargetSplit(res, 0);
    res = Battle_ApplyBarrierAndDamageBonus(res);
    if ((g_pBattleCommandCtx->attackerStatus & BATTLE_STATUS_SMALL) != 0) {
        res = 0;
    }
    res = Battle_ApplyDamageVariance(res);
    g_pBattleCommandCtx->damage = res;
}

/*
 * Barrier halves physical damage and MBarrier everything else; the result flag it sets tells the
 * hit display which barrier took the blow. A result bit set before the call also halves, since
 * neither bit is cleared here. Then the command's damage bonus is added: bonusPercent percent,
 * 10 per step of the support bits (Battle_ApplySupportMateriaFlags, battle_steal_command_effects.cpp).
 */
// FUNCTION: FF7 0x005de82c
int __cdecl Battle_ApplyBarrierAndDamageBonus(int damage)
{
    if (g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_PHYSICAL) {
        if (g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_MBARRIER) {
            g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_MBARRIER;
        }
    } else if (g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_BARRIER) {
        g_pBattleCommandCtx->resultFlags |= BATTLE_RESULT_BARRIER;
    }
    if (g_pBattleCommandCtx->resultFlags & (BATTLE_RESULT_BARRIER | BATTLE_RESULT_MBARRIER)) {
        damage = damage / 2;
    }
    if (g_pBattleCommandCtx->bonusPercent != 0) {
        damage += (damage * g_pBattleCommandCtx->bonusPercent) / 100;
    }
    return damage;
}

/*
 * The multi-target split: an attack aimed at several targets deals 2/3 to each. One target, a
 * random-target attack (BTL_TARGETF_RANDOM) or a caller's fullDamage keeps full damage. A
 * repeated cast (repeatCasts nonzero, Battle_SetRepeatCasts: a spell cast several times in a row)
 * is halved instead, even on a single target.
 */
// FUNCTION: FF7 0x005de8f4
int __cdecl Battle_ApplyMultiTargetSplit(int damage, int fullDamage)
{
    if (fullDamage == 0) {
        if ((g_pBattleCommandCtx->targetCount < 2) ||
            (g_pBattleCommandCtx->targetFlags & BTL_TARGETF_RANDOM)) {
            fullDamage = 1;
        }
    }
    if (g_pBattleCommandCtx->repeatCasts != 0) {
        damage >>= 1;
    } else if (fullDamage == 0) {
        damage = damage * 2 / 3;
    }
    return damage;
}

/*
 * Sadness: the target takes 30% less damage.
 */
// FUNCTION: FF7 0x005de958
int __cdecl Battle_ApplySadnessReduction(int damage)
{
    if (g_pBattleCommandCtx->targetStatus & BATTLE_STATUS_SADNESS) {
        damage -= (damage * 3) / 10;
    }
    return damage;
}

/*
 * The random variance every standard formula ends with: damage * (3841..4096) / 4096, so a hit
 * deals between about 93.8% and 100% of the computed damage. A result of 0 becomes 1, so these
 * formulas always deal at least 1.
 */
// FUNCTION: FF7 0x005de988
int __cdecl Battle_ApplyDamageVariance(int damage)
{
    // BUG: the product is a 32-bit multiply. Damage above 524,287 (559,240 on the lowest roll)
    // overflows it and the shift returns a negative amount, which Battle_ClampHpMpDelta can
    // turn into the target's HP set to 0 (the damage overflow glitch).
    damage = damage * (Battle_NextRandomByte() + 0xF01) >> 12;
    if (damage == 0) {
        damage = 1;
    }
    return damage;
}

/*
 * Damage formula 2: standard magic (Fire, Bolt and most spells): base 6 * (MAt + Lvl) against
 * Magic Defence (the FAQ's formula; see the top of the file), then Sadness, the multi-target
 * split, MBarrier, the damage bonus and the variance. An attack whose targeting defaults to
 * several targets and cannot be toggled to one (DEFAULT_MULTIPLE without TOGGLE_MULTIPLE) is not
 * split.
 */
// FUNCTION: FF7 0x005de9b8
void __cdecl Battle_DmgFormula02_Magical(void)
{
    int single;
    int damage;
    int base;

    single = (g_pBattleCommandCtx->targetFlags & (BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE)) == BTL_TARGETF_DEFAULT_MULTIPLE;
    base = (g_pBattleCommandCtx->attackStat +
            g_pBattleCommandCtx->attackerLevel) *
           6;
    damage = base * (0x200 - g_pBattleCommandCtx->defence) *
             g_pBattleCommandCtx->power / 8192;
    damage = Battle_ApplySadnessReduction(damage);
    damage = Battle_ApplyMultiTargetSplit(damage, single);
    damage = Battle_ApplyBarrierAndDamageBonus(damage);
    damage = Battle_ApplyDamageVariance(damage);
    g_pBattleCommandCtx->damage = damage;
}

/*
 * Damage formula 3: a fraction of what the target has left, power / 32 of its current HP (or MP
 * for an MP-damage attack), the gravity kind of attack (Demi's family, by the community's attack
 * data). It ignores defence, level and variance; a repeated cast takes half.
 */
// FUNCTION: FF7 0x005dea6d
void __cdecl Battle_DmgFormula03_PercentOfCurrent(void)
{
    int val;

    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_MP_DAMAGE) != 0) {
        val = g_aCombatants[g_pBattleCommandCtx->targetSlot].curHp;
    } else {
        val = g_aCombatants[g_pBattleCommandCtx->targetSlot].curMp;
    }
    val = (val * g_pBattleCommandCtx->power) / 32;
    if (g_pBattleCommandCtx->repeatCasts != 0) {
        val >>= 1;
    }
    g_pBattleCommandCtx->damage = val;
}

/*
 * Damage formula 4: power / 32 of the target's maximum HP (or MP for an MP-damage attack),
 * ignoring defence, level and variance; a repeated cast takes half.
 */
// FUNCTION: FF7 0x005deaf7
void __cdecl Battle_DmgFormula04_PercentOfMax(void)
{
    int val;

    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_MP_DAMAGE) != 0) {
        val = g_aCombatants[g_pBattleCommandCtx->targetSlot].maxHp;
    } else {
        val = g_aCombatants[g_pBattleCommandCtx->targetSlot].maxMp;
    }
    val = (val * g_pBattleCommandCtx->power) / 32;
    if (g_pBattleCommandCtx->repeatCasts != 0) {
        val >>= 1;
    }
    g_pBattleCommandCtx->damage = val;
}

/*
 * Damage formula 5: the cure formula, 22 * power + 6 * (MAt + Lvl), with no defence step (the
 * FAQ's "Cure" formula, well established: Cure, Cura and Curaga heal by it). The split, barrier,
 * bonus and variance still apply. Whether the amount heals or hurts is decided later, by the
 * attack's element (BATTLE_ELEMENT_RESTORATIVE) and the target.
 */
// FUNCTION: FF7 0x005deb81
void __cdecl Battle_DmgFormula05_MagicalNoDefence(void)
{
    int damage;

    damage = (g_pBattleCommandCtx->attackStat +
              g_pBattleCommandCtx->attackerLevel) *
                 6 +
             g_pBattleCommandCtx->power * 22;
    damage = Battle_ApplyMultiTargetSplit(damage, 0);
    damage = Battle_ApplyBarrierAndDamageBonus(damage);
    g_pBattleCommandCtx->damage = Battle_ApplyDamageVariance(damage);
}

/*
 * Damage formula 6: fixed damage, 20 * power, whoever attacks whom.
 */
// FUNCTION: FF7 0x005debe5
void __cdecl Battle_DmgFormula06_PowerTimes20(void)
{
    /* The original reserves one slot (`push ecx`) and round-trips the product
       through it before storing. Computing straight into the field skips the
       local entirely and loses three instructions. */
    int damage;

    damage = g_pBattleCommandCtx->power * 20;
    g_pBattleCommandCtx->damage = damage;
}

/*
 * Damage formula 7: power * (512 - Def) / 32 with the variance: a fixed strength that the
 * target's defence still reduces, independent of the attacker's stats and level.
 */
// FUNCTION: FF7 0x005dec0a
void __cdecl Battle_DmgFormula07_PowerVsDefence(void)
{
    int damage;

    damage = g_pBattleCommandCtx->power *
             (0x200 - g_pBattleCommandCtx->defence) / 32;
    g_pBattleCommandCtx->damage = Battle_ApplyDamageVariance(damage);
}

/*
 * Damage formula 8: full recovery (HP and MP to full, BATTLE_ELEM_REACTION_FULL_RECOVERY), or
 * Death for a target that absorbs the attack's element (BATTLE_ELEM_REACTION_ABSORB): the
 * reversal that makes restoratives hurt such targets turns full recovery into Death.
 */
// FUNCTION: FF7 0x005dec52
void __cdecl Battle_DmgFormula08_DeathOrFullCure(void)
{
    if (g_pBattleCommandCtx->elemReaction & BATTLE_ELEM_REACTION_ABSORB) {
        g_pBattleCommandCtx->elemReaction = BATTLE_ELEM_REACTION_DEATH;
    } else {
        g_pBattleCommandCtx->elemReaction = BATTLE_ELEM_REACTION_FULL_RECOVERY;
    }
}

/*
 * Damage formula 9: physical damage with the weapon power doubled as the attack stat.
 */
// FUNCTION: FF7 0x005dec8a
void Battle_DmgFormula09_WeaponPowerPhysical(void)
{
    g_pBattleCommandCtx->attackStat = g_pBattleCommandCtx->weaponPower * 2;
    Battle_DmgFormula01_Physical();
}

/*
 * Damage formula 0xA: a fixed total, power, shared out among the targets. Each target takes its
 * share rounded up, so the targets together can take a little more than power.
 */
// FUNCTION: FF7 0x005decaa
void Battle_DmgFormula0A_PowerSplitByTargets(void)
{
    int count;
    int damage;

    count = Battle_PopCountMask(g_pBattleCommandCtx->targetMask);
    damage = 0;
    if (count != 0) {
        damage = (g_pBattleCommandCtx->power + count - 1) / count;
    }
    g_pBattleCommandCtx->damage = damage;
}

/*
 * Damage formula 0x10: as much damage as the attacker has HP left, as of the end of the last
 * action (g_BattleState.actors[].hp, which Battle_SnapshotHpMp copies).
 */
// FUNCTION: FF7 0x005decfb
void Battle_DmgFormula10_AttackerHp(void)
{
    g_pBattleCommandCtx->damage =
        g_BattleState.actors[g_pBattleCommandCtx->attackerSlot].hp;
}

/*
 * Damage formula 0x11: as much damage as the attacker has lost: max HP minus its HP as of the
 * end of the last action. The more hurt the attacker, the harder it strikes back.
 */
// FUNCTION: FF7 0x005ded1e
void Battle_DmgFormula11_AttackerHpMissing(void)
{
    g_pBattleCommandCtx->damage =
        g_aCombatants[g_pBattleCommandCtx->attackerSlot].maxHp - (g_BattleState.actors[g_pBattleCommandCtx->attackerSlot].hp);
}

/*
 * Damage formula 0x12: not used; the table entry does nothing.
 */
// FUNCTION: FF7 0x005ded55
void Battle_DmgFormula12_Stub(void)
{
    /* The original body is empty. */
}

/*
 * Damage formula 0x13: not used; the table entry does nothing.
 */
// FUNCTION: FF7 0x005ded5a
void Battle_DmgFormula13_Stub(void)
{
    /* The original body is empty. */
}

/*
 * Damage formula 0x14: not used; the table entry does nothing.
 */
// FUNCTION: FF7 0x005ded5f
void Battle_DmgFormula14_Stub(void)
{
    /* The original body is empty. */
}

/*
 * Damage formula 0x15: not used; the table entry does nothing.
 */
// FUNCTION: FF7 0x005ded64
void Battle_DmgFormula15_Stub(void)
{
    /* The original body is empty. */
}

/*
 * Damage formula 0x16: not used; the table entry does nothing.
 */
// FUNCTION: FF7 0x005ded69
void Battle_DmgFormula16_Stub(void)
{
    /* The original body is empty. */
}

/*
 * Damage formula 0x17: not used; the table entry does nothing.
 */
// FUNCTION: FF7 0x005ded6e
void Battle_DmgFormula17_Stub(void)
{
    /* The original body is empty. */
}

/*
 * Damage formula 0x18: dice. One die per ten levels of the attacker, at least two and at most six.
 * Damage is 100 * the sum of the faces * the size of the largest group of equal faces: a pair
 * doubles it, three of a kind triples it. The faces (0..5) go two to a byte into
 * g_BattleScene.squareSlots, 0xF in an unused half, for the effect to show.
 */
// FUNCTION: FF7 0x005ded73
void __cdecl Battle_DmgFormula18_DiceRoll(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        int rollIndex;
        struct { unsigned char pad_to_4[0x4]; int faceCount; };
        struct { unsigned char pad_to_8[0x8]; int roll; };
        struct { unsigned char pad_to_C[0xC]; int rolls[8]; };
        struct { unsigned char pad_to_2C[0x2C]; int dieIndex; };
        struct { unsigned char pad_to_30[0x30]; int rollSum; };
        struct { unsigned char pad_to_34[0x34]; int maxFaceCount; };
        struct { unsigned char pad_to_38[0x38]; int diceCount; };
    } frame;
#pragma pack(pop)

    frame.diceCount = g_pBattleCommandCtx->attackerLevel / 0xA;
    if (frame.diceCount < 2) {
        frame.diceCount = 2;
    }
    if (frame.diceCount > 6) {
        frame.diceCount = 6;
    }
    for (frame.dieIndex = 0; frame.dieIndex < 4; frame.dieIndex++) {
        g_BattleScene.squareSlots[frame.dieIndex] = 0xFF;
    }
    frame.rollSum = 0;
    for (frame.dieIndex = 0; frame.dieIndex < frame.diceCount; frame.dieIndex++) {
        /* Retail caller treats `Battle_RandBelow` return (`int`) as `unsigned char` for caller zero/sign extension. */
        frame.roll = ((unsigned char (__cdecl *)(int limit))Battle_RandBelow)(6);
        frame.rolls[frame.dieIndex] = frame.roll;
        frame.rollSum = frame.rollSum + frame.roll + 1;
        if ((frame.dieIndex & 1) != 0) {
            g_BattleScene.squareSlots[frame.dieIndex / 2] = frame.roll << 4 | g_BattleScene.squareSlots[frame.dieIndex / 2] & 0xF;
        } else {
            g_BattleScene.squareSlots[frame.dieIndex / 2] = frame.roll | 0xF0;
        }
        Battle_AdvanceActionSequenceCounter();
    }
    frame.maxFaceCount = 0;
    for (frame.dieIndex = 0; frame.dieIndex < 6; frame.dieIndex++) {
        frame.faceCount = 0;
        for (frame.rollIndex = 0; frame.rollIndex < frame.diceCount; frame.rollIndex++) {
            if (frame.rolls[frame.rollIndex] == frame.dieIndex) {
                frame.faceCount++;
            }
        }
        if (frame.maxFaceCount < frame.faceCount) {
            frame.maxFaceCount = frame.faceCount;
        }
    }
    frame.rollSum *= frame.maxFaceCount * 0x64;
    g_pBattleCommandCtx->damage = frame.rollSum;
}

/*
 * Damage formula 0x19: as much damage as the number of battles the party has escaped from
 * (gSavemap's escape counter, a 16-bit count kept in two bytes).
 */
// FUNCTION: FF7 0x005deeef
void Battle_DmgFormula19_SavemapCounter(void)
{
    int counter;

    counter = gSavemap.escapesLo + (gSavemap.escapesHi << 8);
    g_pBattleCommandCtx->damage = counter;
}

/*
 * Damage formula 0x1A: takes the target down to 1 HP.
 */
// FUNCTION: FF7 0x005def1d
void __cdecl Battle_DmgFormula1A_TargetHpMinusOne(void)
{
    g_pBattleCommandCtx->damage =
        g_aCombatants[g_pBattleCommandCtx->targetSlot].curHp - 1;
}

/*
 * Damage formula 0x1B (Tonberry's Time Damage): the play clock
 * read as a decimal HHMM, i.e. hours*100 + minutes.
 */
// FUNCTION: FF7 0x005def44
void __cdecl Battle_DmgFormula1B_PlayTime(void)
{
    int minutes;

    minutes = gSavemap.seconds / 60;
    g_pBattleCommandCtx->damage = (minutes / 60) * 100 + minutes % 60;
}

/*
 * Damage formula 0x1C: ten times the number of enemies the target character has killed in the
 * whole game (the savemap kill count). Enemy targets (slot >= 3) take 0.
 */
// FUNCTION: FF7 0x005def88
void __cdecl Battle_DmgFormula1C_KillCountTimes10(void)
{
    int killCount;
    SavemapCharacter *member;

    killCount = 0;
    if (g_pBattleCommandCtx->targetSlot < 3) {
        member = g_aBattlePartyStat[g_pBattleCommandCtx->targetSlot].partyMember;
        killCount = member->killCount;
    }
    g_pBattleCommandCtx->damage = killCount * 10;
}

/*
 * Damage formula 0x1D: 1111 for each materia the target character has equipped, counting the
 * weapon's and the armor's eight slots each (an empty slot reads -1). Enemy targets take 0.
 */
// FUNCTION: FF7 0x005defdd
void __cdecl Battle_DmgFormula1D_MateriaCountTimes1111(void)
{
    int slotIndex;
    int sum;
    int slotValue;
    SavemapCharacter *memberRecord;

    sum = 0;
    if (g_pBattleCommandCtx->targetSlot < 3) {
        memberRecord = g_aBattlePartyStat[g_pBattleCommandCtx->targetSlot].partyMember;
        for (slotIndex = 0; slotIndex < 8; slotIndex++) {
            slotValue = memberRecord->weaponMateria[slotIndex];
            if (slotValue != -1) {
                sum++;
            }
            slotValue = memberRecord->armorMateria[slotIndex];
            if (slotValue != -1) {
                sum++;
            }
        }
    }
    g_pBattleCommandCtx->damage = sum * 0x457;
}

// The damage formula table Battle_RunDamageProgram indexes by the command's formula number (battle_damage_program_run.cpp);
// slots 11..15 and 30..31 are empty.
typedef void (__cdecl *BattleDmgFormulaFn)(void);
} /* extern "C" */

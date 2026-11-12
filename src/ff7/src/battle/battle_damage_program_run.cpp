// src/ff7/src/battle/battle_damage_program_run.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the start-offset table for the damage program bytes, runs the indexed damage
//   program until its end opcode, and decodes the calculation parameter byte for the active
//   command.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
extern "C" {

// The damage-program blob Battle_BuildDamageProgramTable scans to its end byte (8), and the
// effect-id table after it. Each program is the list of damage ops (g_BattleDamageOpTable) that
// decide whether a hit lands, picked by the high nibble of an attack's damage calculation byte:
//   0, 12..15  none (always hits)            6  physical hit and critical (formulas 0x10..0x1F)
//   1          physical hit and critical      7  magic hit (formulas 0x10..0x1F)
//   2          magic hit                      8  the "Level N" check
//   3, 4, 5    none (ops 3..5 do nothing)     9  Manipulate
//   10         physical hit and critical      11 physical hit, no critical
// So 0x11 is physical hit + critical + formula 1, and 0x22 magic hit + formula 2.
// GLOBAL: FF7 0x007b7700
const unsigned char g_abDamageStatProgramBlob[32] = {0x08, 0x00, 0x02, 0x08, 0x01, 0x08, 0x03, 0x08, 0x04, 0x08, 0x05, 0x08, 0x00, 0x02, 0x08, 0x01, 0x08, 0x07, 0x08, 0x06, 0x08, 0x00, 0x02, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00};
// GLOBAL: FF7 0x007b7720
extern const unsigned char g_abDamageCalcParamTable[12] = {0x0a, 0x0b, 0x0c, 0x0d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x00, 0x00, 0x00};

// clang-format off
// GLOBAL: FF7 0x008ff068
unsigned char g_abDamageFormulaFlags[16] = {0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x03, 0x02, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00c069c0
unsigned int g_anDamageProgramEntry[16] = {0};
// clang-format on

/*
 * Indexes the sixteen damage programs: the start of each one in g_abDamageStatProgramBlob, so that
 * Battle_RunDamageProgram can find program N.
 */
// FUNCTION: FF7 0x005d16d0
void __cdecl Battle_BuildDamageProgramTable(void)
{
    unsigned int byteIndex;
    int entryCount;
    int start;

    entryCount = 0;
    byteIndex = 0;
    start = 0;
    for (; byteIndex < 0x1E; byteIndex++) {
        if (entryCount < 16) {
            if (byteIndex == (unsigned int)start) {
                g_anDamageProgramEntry[entryCount] = byteIndex;
                entryCount++;
            }
            if (g_abDamageStatProgramBlob[byteIndex] == BATTLE_DAMAGE_PROGRAM_END) {
                start = byteIndex + 1;
            }
        }
    }
}

/*
 * Works out one hit on one target: runs the attack's damage program (its hit checks: physical or
 * magic hit, critical, Manipulate, the level check), then, for an attack with power, its damage
 * formula between the two halves of its special effect (Battle_RunSpecialAttackEffect before and
 * after). A miss does not stop the formula here; the caller reads BATTLE_RESULT_MISS.
 */
// FUNCTION: FF7 0x005d173f
void __cdecl Battle_RunDamageProgram(void)
{
    void(__cdecl * p)(void);
    int programPos;
    unsigned int opcode;
    programPos = g_anDamageProgramEntry[g_pBattleCommandCtx->damageProgramIndex];
    p = g_BattleDamageFormulaTable[g_pBattleCommandCtx->damageFormulaIndex];
    while ((opcode = g_abDamageStatProgramBlob[programPos++]) != BATTLE_DAMAGE_PROGRAM_END) {
        g_BattleDamageOpTable[opcode]();
    }
    if (g_pBattleCommandCtx->power != 0 && p != 0) {
        Battle_RunSpecialAttackEffect(0);
        p();
        Battle_RunSpecialAttackEffect(1);
    }
}

/*
 * Splits an attack's damage calculation byte into its damage program (high nibble: the hit
 * checks) and its damage formula (low nibble), and picks the stat it attacks with.
 * g_abDamageFormulaFlags then says, per program, whether the attack is physical (so Barrier, not
 * MBarrier, applies and it attacks with Attack), whether its formula comes from the second half
 * of the table (0x10..0x1F), or whether the low nibble names a special effect from
 * g_abDamageCalcParamTable and the formula is the physical one. A physical attack uses the
 * attacker's Attack, anything else its Magic Attack, each with its percent bonus.
 */
#define DAMAGE_CALC_PROGRAM(calcByte) (((calcByte) & 0xF0) >> 4)
#define DAMAGE_CALC_FORMULA(calcByte) ((calcByte) & 0xF)
// FUNCTION: FF7 0x005d17c7
void __cdecl Battle_DecodeDamageCalcByte(void)
{
    int formulaFlags;
    int packedByte;

    packedByte = g_pBattleCommandCtx->damageCalcByte;
    g_pBattleCommandCtx->damageProgramIndex = DAMAGE_CALC_PROGRAM(packedByte);
    g_pBattleCommandCtx->damageFormulaIndex = DAMAGE_CALC_FORMULA(packedByte);
    formulaFlags = g_abDamageFormulaFlags[g_pBattleCommandCtx->damageProgramIndex];
    if ((formulaFlags & BATTLE_DAMAGE_FORMULA_PHYSICAL) != 0) {
        g_pBattleCommandCtx->specialFlags &= ~BATTLE_SPECIAL_NOT_PHYSICAL;
    }
    if ((formulaFlags & BATTLE_DAMAGE_FORMULA_SECOND_BANK) != 0) {
        g_pBattleCommandCtx->damageFormulaIndex |= 0x10;
    }
    if ((formulaFlags & BATTLE_DAMAGE_FORMULA_EFFECT_FROM_TABLE) != 0) {
        g_pBattleCommandCtx->effectId = g_abDamageCalcParamTable[g_pBattleCommandCtx->damageFormulaIndex];
        g_pBattleCommandCtx->damageFormulaIndex = 1;
    }
    if ((g_pBattleCommandCtx->specialFlags & BATTLE_SPECIAL_NOT_PHYSICAL) != 0) {
        g_pBattleCommandCtx->attackStat = Battle_ApplyStatPercentModifier(g_pBattleCommandCtx->attackerSlot, g_aCombatants[g_pBattleCommandCtx->attackerSlot].magicAttack, 1);
    } else {
        g_pBattleCommandCtx->attackStat = Battle_ApplyStatPercentModifier(g_pBattleCommandCtx->attackerSlot, g_aCombatants[g_pBattleCommandCtx->attackerSlot].attack, 0);
    }
}
} /* extern "C" */

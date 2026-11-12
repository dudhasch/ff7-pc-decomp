// src/ff7/src/battle/battle_kernel_experience_growth.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Applies earned experience to character growth records: level-ups and stat growth from
//   the experience curves, HP and MP level-ups, joining characters, and writing growth data back to
//   the save record.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
extern "C" {

struct CharGrowth;
/* Array view across the 3 contiguous party-slot globals at 0x00C06640..0x00C06648. */
extern unsigned int g_BattleExpShownByPartySlot[];

// Per party slot: the experience at the loaded level's start, then the first eleven level thresholds.

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

/*
 * Loads the character's growth record from charRecord, binds it, earns exp and levels it up, then
 * writes it back. For a party slot (not -1) it also clears the slot's level-up flag and keeps, for
 * the result screen, the loaded level, the new and the loaded experience, the loaded level's start
 * and the first eleven thresholds.
 */
// FUNCTION: FF7 0x005c7260
void __cdecl Kernel_ApplyExperienceToCharacterRecord(SavemapCharacter *charRecord, int exp, int partySlot)
{
    int i;
    CharGrowth growthRecord;

    Kernel_LoadGrowthActor(&growthRecord, charRecord);
    if (partySlot != -1) {
        g_BattlePartyLevelUpFlags[partySlot] = 0;
        g_BattlePartyInitialLevels[partySlot] = growthRecord.level;
    }
    Kernel_BindGrowthRecord(&growthRecord);
    growthRecord.expEarned = exp;
    Kernel_ApplyExpGrowthLevelUps(&growthRecord);
    if (partySlot != -1) {
        g_BattleExpTargetByPartySlot[partySlot] = growthRecord.exp;
        g_BattleExpShownByPartySlot[partySlot] = growthRecord.expBase;
        g_BattlePartyGrowthStats[partySlot][0] = growthRecord.expForLevel;
        for (i = 0; i <= 10; i++) {
            g_BattlePartyGrowthStats[partySlot][1 + i] = growthRecord.thresholds[i];
        }
    }
    Savemap_WriteGrowthToRecord(&growthRecord, charRecord);
}

/*
 * Initializes a joining character growth record using the average level of visible saved
 * characters other than itself, or its current level when none qualify. A per-character signed
 * byte either chooses a global level when its low bit is set or adds half that byte to the
 * average, with minimum level one. Loads the growth record; with the low bit set it resets it
 * first (experience 0, level 1, the eight base stats 0, which Kernel_BindGrowthRecord raises to 1).
 * Binds it, earns the experience of the chosen level minus one, levels up and writes the record
 * back. No upper-level or record-pointer guard is present.
 */
// FUNCTION: FF7 0x005c7336
void __cdecl Kernel_InitJoiningCharacter(SavemapCharacter *charRecord)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[156];
        int savedId;
        struct { unsigned char pad_to_4[0x4]; int contributorCount; };
        struct { unsigned char pad_to_8[0x8]; char growthByte; };
        struct { unsigned char pad_to_C[0xC]; CharGrowth growth; };
        struct { unsigned char pad_to_90[0x90]; int charIdx; };
        struct { unsigned char pad_to_94[0x94]; int memberId; };
        struct { unsigned char pad_to_98[0x98]; int level; };
    } frame;
#pragma pack(pop)

    frame.level = 0;
    frame.contributorCount = 0;
    frame.growthByte = 0;
    if (charRecord->id < 9) {
        frame.growthByte = g_BattleKernelData.chars[charRecord->id].joinLevelOffset;
    }
    for (frame.memberId = 0; frame.memberId < 9; frame.memberId++) {
        if ((gSavemap.phsVisibilityMask & 1 << frame.memberId) != 0) {
            for (frame.charIdx = 0; frame.charIdx < 9; frame.charIdx++) {
                frame.savedId = gSavemap.chars[frame.charIdx].id;
                if (frame.savedId == frame.memberId) {
                    if (frame.savedId == charRecord->id) {
                        break;
                    }
                    frame.level += gSavemap.chars[frame.charIdx].level;
                    frame.contributorCount++;
                    break;
                }
            }
        }
    }
    if (frame.contributorCount == 0) {
        frame.contributorCount = 1;
        frame.level = charRecord->level;
    }
    frame.level = frame.level / frame.contributorCount;
    if ((frame.growthByte & 1) != 0) {
        frame.level = gSavemap.joinLevel;
    } else {
        frame.level += frame.growthByte / 2;
    }
    if (frame.level < 1) {
        frame.level = 1;
    }
    Kernel_LoadGrowthActor(&frame.growth, charRecord);
    if ((frame.growthByte & 1) != 0) {
        frame.growth.expBase = 0;
        frame.growth.level = 1;
        for (frame.memberId = 0; frame.memberId < 8; frame.memberId++) {
            frame.growth.baseStats[frame.memberId] = 0;
        }
    }
    Kernel_BindGrowthRecord(&frame.growth);
    frame.growth.expEarned = Kernel_ComputeExpForLevel(&frame.growth, frame.level - 1);
    Kernel_ApplyExpGrowthLevelUps(&frame.growth);
    Savemap_WriteGrowthToRecord(&frame.growth, charRecord);
}

/*
 * Copies the id, the level, the six primary stats (bytes widened to shorts), max HP and MP and the
 * experience from the savemap record into the growth record (base, expBase). Other members are
 * left as they are.
 */
// FUNCTION: FF7 0x005c7534
void __cdecl Kernel_LoadGrowthActor(CharGrowth *growthActor, SavemapCharacter *charRecord)
{
    growthActor->charId = charRecord->id;
    growthActor->level = charRecord->level;
    growthActor->base.strength = charRecord->strength;
    growthActor->base.vitality = charRecord->vitality;
    growthActor->base.magic = charRecord->magic;
    growthActor->base.spirit = charRecord->spirit;
    growthActor->base.dexterity = charRecord->dexterity;
    growthActor->base.luck = charRecord->luck;
    growthActor->base.maxHp = charRecord->baseHp;
    growthActor->base.maxMp = charRecord->baseMp;
    growthActor->expBase = charRecord->exp;
}

/*
 * Copies growth level/stat bytes, maximum shorts and experience into save record, stores next-
 * threshold minus current experience and unsigned scaled progress 61 when threshold span nonzero.
 * Level at least 99 overrides remaining experience zero and progress 61.
 */
// FUNCTION: FF7 0x005c75d2
void __cdecl Savemap_WriteGrowthToRecord(CharGrowth *growth, SavemapCharacter *charRecord)
{
    charRecord->level = growth->level;
    charRecord->strength = growth->cur.strength;
    charRecord->vitality = growth->cur.vitality;
    charRecord->magic = growth->cur.magic;
    charRecord->spirit = growth->cur.spirit;
    charRecord->dexterity = growth->cur.dexterity;
    charRecord->luck = growth->cur.luck;
    charRecord->baseHp = growth->cur.maxHp;
    charRecord->baseMp = growth->cur.maxMp;
    charRecord->exp = growth->exp;
    charRecord->expToNext = growth->expNextLevel - growth->exp;
    charRecord->levelProgress = 0;
    if (growth->expNextLevel - growth->expLevelStart != 0) {
        charRecord->levelProgress = (unsigned int)((growth->exp - growth->expLevelStart) * 61) / (unsigned int)(growth->expNextLevel - growth->expLevelStart);
    }
    if (growth->level >= 99) {
        charRecord->expToNext = 0;
        charRecord->levelProgress = 0x3D;
    }
}

/*
 * Points kernelRow at the character's kernel row, raises each zero base stat to 1, copies base to
 * cur and clears the sixteen thresholds.
 */
// FUNCTION: FF7 0x005c76d2
void __cdecl Kernel_BindGrowthRecord(CharGrowth *growthPtr)
{
    int index;

    growthPtr->kernelRow = &g_BattleKernelData.chars[growthPtr->charId];
    for (index = 0; index < 8; index++) {
        if (growthPtr->baseStats[index] == 0) {
            growthPtr->baseStats[index] = 1;
        }
        growthPtr->curStats[index] = growthPtr->baseStats[index];
    }
    for (index = 0; index < 16; index++) {
        growthPtr->thresholds[index] = 0;
    }
}

/*
 * Adds earned experience to baseline with unsigned cap 999999999, records current-level threshold
 * and loops until level 99 or next threshold exceeds experience. Stores first sixteen encountered
 * thresholds, applies six stat growth helpers plus maximum HP/MP each level and updates previous
 * threshold. Starting level 99 leaves next threshold unchanged.
 */
// FUNCTION: FF7 0x005c7769
void __cdecl Kernel_ApplyExpGrowthLevelUps(CharGrowth *growth)
{
    struct {
        int i;
        int nextExp;
        int count;
        int level;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.count = 0;
    growth->exp =
        growth->expBase + growth->expEarned;
    if ((unsigned int)growth->exp > 999999999) {
        growth->exp = 0x3B9AC9FF;
    }
    growth->expForLevel = Kernel_ComputeExpForLevel(
        growth, growth->level - 1);
    growth->expLevelStart = growth->expForLevel;
    while ((local.level = growth->level) < 99) {
        local.nextExp = Kernel_ComputeExpForLevel(growth, local.level);
        growth->expNextLevel = local.nextExp;
        if (local.count < 16) {
            growth->thresholds[local.count] = local.nextExp;
            local.count++;
        }
        if ((unsigned int)local.nextExp <=
            (unsigned int)growth->exp) {
            for (local.i = 0; local.i < 6; local.i++) {
                Kernel_LevelUpPrimaryStat(growth, local.i);
            }
            Kernel_LevelUpMaxHp(growth);
            Kernel_LevelUpMaxMp(growth);
            (growth->level)++;
            growth->expLevelStart = local.nextExp;
        } else {
            break;
        }
    }
}

/*
 * Stores level-region index in record first int then sums coefficient times q squared divided ten
 * for q one..level. Region chosen once from requested level, not per iteration. Negative levels
 * skip sum.
 */
// FUNCTION: FF7 0x005c788a
int __cdecl Kernel_ComputeExpForLevel(CharGrowth *growth, int level)
{
    int total;
    int tier;
    int step;

    total = 0;
    growth->tier = 0;
    if (level != 0) {
        if (level <= 60) {
            tier = (level - 1) / 10;
        } else {
            tier = (level - 0x3D) / 20 + 6;
        }
        growth->tier = tier;
    }
    for (step = 1; step <= level; step++) {
        total += g_BattleKernelData.curves[growth->kernelRow->expCurve].brackets[growth->tier].gradient * step * step / 10;
    }
    return total;
}

/*
 * Evaluates next-level primary-stat curve, chooses growth bonus from difference to current stat,
 * adds unsigned bonus byte and upper-clamps 100 before storing short.
 */
// FUNCTION: FF7 0x005c7932
void __cdecl Kernel_LevelUpPrimaryStat(CharGrowth *growth, int statIndex)
{
    int newStat;
    int targetStat;
    int bonusIndex;
    int delta;

    targetStat = Kernel_EvalPrimaryStatCurve(growth, growth->level + 1, statIndex);
    delta = targetStat - growth->curStats[statIndex];
    bonusIndex = Kernel_PickGrowthBonusIndex(delta);
    newStat = growth->curStats[statIndex];
    newStat += g_BattleKernelData.statBonus[bonusIndex];
    if (newStat > 100) {
        newStat = 0x64;
    }
    growth->curStats[statIndex] = newStat;
}

/*
 * Adds a random value from zero through seven plus one to delta and clamps the result to zero
 * through eleven.
 */
// FUNCTION: FF7 0x005c79be
int __cdecl Kernel_PickGrowthBonusIndex(int delta)
{
    int index;

    index = delta + (Battle_NextRandomByte() & 7) + 1;
    if (index < 0) {
        index = 0;
    } else if ((unsigned int)index >= (unsigned int)0xC) {
        index = 11;
    }
    return index;
}

/*
 * Evaluates one primary stat curve. The curve id is byte statIndex of the character's kernel row
 * (growth->kernelRow->statCurves); a curve is g_BattleKernelData.curves[id], eight two-byte brackets,
 * an unsigned gradient byte and the signed base byte after it, and growth->tier picks the bracket.
 * Returns gradient * level / 0x64 + base.
 */
// FUNCTION: FF7 0x005c79fc
int __cdecl Kernel_EvalPrimaryStatCurve(CharGrowth *growth, int level, int statIndex)
{
    return g_BattleKernelData.curves[growth->kernelRow->statCurves[statIndex]].brackets[growth->tier].gradient * level / 100 + g_BattleKernelData.curves[growth->kernelRow->statCurves[statIndex]].brackets[growth->tier].base;
}

/*
 * Raises growth->cur.maxHp on level up. It evaluates Kernel_EvalHpCurve at the current level and at
 * the next one, feeds the percentage overshoot (curve * 0x64 /
 * current max HP - 0x64) to Kernel_PickGrowthBonusIndex, and adds the curve step scaled by the
 * bonus percentage byte g_BattleKernelData.hpBonus[index] over 0x64. The result is clamped to 0x270F (9999).
 */
// FUNCTION: FF7 0x005c7a50
void __cdecl Kernel_LevelUpMaxHp(CharGrowth *growth)
{
    int hp;
    int nextCurve;
    int overshoot;
    int index;
    int currentCurve;

    currentCurve = Kernel_EvalHpCurve(growth, growth->level);
    nextCurve = Kernel_EvalHpCurve(growth, growth->level + 1);
    overshoot = currentCurve * 100 / growth->cur.maxHp - 0x64;
    index = Kernel_PickGrowthBonusIndex(overshoot);
    hp = growth->cur.maxHp;
    hp += (nextCurve - currentCurve) * g_BattleKernelData.hpBonus[index] / 100;
    if (hp > 9999) {
        hp = 0x270F;
    }
    growth->cur.maxHp = hp;
}

/*
 * Evaluates the HP curve (curve id hpCurve, byte 6 of the kernel row; brackets as in
 * Kernel_EvalPrimaryStatCurve): gradient * level + base * 0x28.
 */
// FUNCTION: FF7 0x005c7b01
int __cdecl Kernel_EvalHpCurve(CharGrowth *growth, int level)
{
    return g_BattleKernelData.curves[growth->kernelRow->hpCurve].brackets[growth->tier].gradient * level + g_BattleKernelData.curves[growth->kernelRow->hpCurve].brackets[growth->tier].base * 40;
}

/*
 * Evaluates MP curves at current/next level, derives bonus index from current curve/current maxMP
 * ratio minus 100 and adds scaled curve difference. Upper-clamps 999 only.
 */
// FUNCTION: FF7 0x005c7b46
void __cdecl Kernel_LevelUpMaxMp(CharGrowth *growth)
{
    int nextCurve;
    int overshoot;
    int index;
    int currentCurve;
    int mpCeil;

    currentCurve = Kernel_EvalMpCurve(growth, growth->level);
    nextCurve = Kernel_EvalMpCurve(growth, growth->level + 1);
    overshoot = currentCurve * 100 / growth->cur.maxMp - 0x64;
    index = Kernel_PickGrowthBonusIndex(overshoot);
    mpCeil = growth->cur.maxMp;
    mpCeil += (nextCurve - currentCurve) * g_BattleKernelData.mpBonus[index] / 100;
    if (mpCeil > 999) {
        mpCeil = 0x3E7;
    }
    growth->cur.maxMp = mpCeil;
}

/*
 * Evaluates the MP curve (curve id mpCurve, byte 7 of the kernel row; brackets as in
 * Kernel_EvalPrimaryStatCurve): gradient * level / 0xA + base * 2.
 */
// FUNCTION: FF7 0x005c7bf7
int __cdecl Kernel_EvalMpCurve(CharGrowth *growth, int level)
{
    return g_BattleKernelData.curves[growth->kernelRow->mpCurve].brackets[growth->tier].gradient * level / 10 + g_BattleKernelData.curves[growth->kernelRow->mpCurve].brackets[growth->tier].base * 2;
}

// clang-format on
} /* extern "C" */

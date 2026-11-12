// src/ff7/src/battle/battle_write_party_rewards.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Writes the party's battle state back to the savemap, including HP, MP, status and
//   limits, and adds the battle reward totals. Also sets materia type bits, grants half experience
//   to non-party characters, and resets the reward totals.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/menu.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/types/battle_actors.h"
#include "ff7/types/menu.h"
#include "ff7/enums/main.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Adds the battle's AP to the savemap's AP total (apTotal, four bytes), copies three party actors HP/MP/status/limit bytes
 * into matching character records with masked override and special HP7777 mapped one. Optional AI
 * flag unlocks twelve available limits for character zero; final flag restores entire saved
 * character snapshot after prior updates.
 */
// FUNCTION: FF7 0x005c6b60
void __cdecl Battle_WritePartyStateBackToSavemap(int hpOverride, int mpOverride)
{
    int slotIndex;
    int sum;
    int partyIdx;
    int limitIndex;
    int hp;
    SavemapCharacter *recPointer;
    int actorId;
    int mp;
    int savedCharSlot;
    int attackId;
    for (slotIndex = 0; (unsigned int)slotIndex < 4; slotIndex++) {
        ((char *)&sum)[slotIndex] = gSavemap.apTotal[slotIndex];
    }
    sum += g_BattleRewardAp;
    for (slotIndex = 0; (unsigned int)slotIndex < 4; slotIndex++) {
        gSavemap.apTotal[slotIndex] = ((char *)&sum)[slotIndex];
    }
    for (partyIdx = 0; partyIdx < 3; partyIdx++) {
        actorId = g_BattleSavedPartyBytes[partyIdx];
        hp = g_aCombatants[partyIdx].curHp;
        mp = g_aCombatants[partyIdx].curMp;
        if ((g_BattleState.participatingPartyMask & (1 << partyIdx)) != 0) {
            hp = hpOverride;
            mp = mpOverride;
        }
        if (hp == BATTLE_LUCKY_7S_HP)
            hp = 1;
        for (slotIndex = 0; slotIndex < 9; slotIndex++) {
            recPointer = &gSavemap.chars[slotIndex];
            if (actorId == recPointer->id) {
                recPointer->hp = hp;
                recPointer->mp = mp;
                recPointer->limitBar = *(char *)&g_BattleState.partyStats[partyIdx].limitGauge;
                recPointer->status = g_aCombatants[partyIdx].status & (BATTLE_STATUS_SADNESS | BATTLE_STATUS_FURY);
                if ((gBattleAiGlobals.arenaFlags & BATTLE_ARENA_ALL_LIMITS) != 0 && recPointer->id == PARTY_CHAR_CLOUD) {
                    for (limitIndex = 0; limitIndex < 12; limitIndex++) {
                        attackId = Kernel_GetLimitAttackId(actorId, limitIndex);
                        if (attackId != 0x7f)
                            *(short *)&recPointer->limits |= 1 << limitIndex;
                    }
                    recPointer->limitLevel = 4;
                    recPointer->limitBar = 0;
                }
                break;
            }
        }
    }
    if ((g_BattleScene.flags & 0x40) != 0) {
        savedCharSlot = g_BattleItemListBlock.savedCharacter.id;
        for (slotIndex = 0; slotIndex < 9; slotIndex++) {
            if (savedCharSlot == gSavemap.chars[slotIndex].id) {
                gSavemap.chars[slotIndex] = g_BattleItemListBlock.savedCharacter;
                break;
            }
        }
    }
}

/*
 * For two eight-entry packed materia arrays ORs bits shifted eight into entries whose low-byte ID
 * not 0xff and table type nibble seven.
 */
// FUNCTION: FF7 0x005c6db5
void __cdecl Battle_OrBitsIntoEquippedMateriaType7(SavemapCharacter *record, int bits)
{
    int slot;
    int materiaId;

    bits <<= 8;
    for (slot = 0; slot < 8; slot++) {
        materiaId = record->weaponMateria[slot] & 0xFF;
        if (materiaId != 0xFF && (g_KernelMateriaDataArray[materiaId].typeNibbleByte & 0xF) == 7) {
            record->weaponMateria[slot] |= bits;
        }
        materiaId = record->armorMateria[slot] & 0xFF;
        if (materiaId != 0xFF && (g_KernelMateriaDataArray[materiaId].typeNibbleByte & 0xF) == 7) {
            record->armorMateria[slot] |= bits;
        }
    }
}

/*
 * For nine selected save records with actor ID below nine, snapshots maximum HP/MP and applies
 * growth using half global experience and the character's level minus one. Only if current HP remains nonzero
 * adds positive maximum increases to current HP/MP; dead HP also suppresses MP adjustment.
 */
// FUNCTION: FF7 0x005c6e7b
void __cdecl Battle_GrantHalfExpToNonPartyCharacters(int actorMask)
{
    int level;
    int recordIdx;
    int oldMaxHp;
    SavemapCharacter *pRecord;
    int halfExp;
    int prevMaxMp;

    for (recordIdx = 0; recordIdx < 9; recordIdx++) {
        if (gSavemap.chars[recordIdx].id < 9 && (actorMask & 1 << gSavemap.chars[recordIdx].id) != 0) {
            pRecord = &gSavemap.chars[recordIdx];
            level = pRecord->level;
            halfExp = g_BattleRewardExp / 2;
            oldMaxHp = pRecord->baseHp;
            prevMaxMp = pRecord->baseMp;
            Kernel_ApplyExperienceToCharacterRecord(pRecord, halfExp, -1);
            if (pRecord->hp != 0) {
                if (pRecord->baseHp > oldMaxHp) {
                    *(short *)&pRecord->hp += (short)(pRecord->baseHp - oldMaxHp);
                }
                if (pRecord->baseMp > prevMaxMp) {
                    pRecord->mp += (short)(pRecord->baseMp - prevMaxMp);
                }
            }
        }
    }
}

/*
 * Clears three globals and selected fields of four/three record groups, builds mask of missing
 * savemap party slots and passes it to Battle_InitExpResultRows.
 */
// FUNCTION: FF7 0x005c6f9c
void __cdecl Battle_ResetResultRewardTotals(void)
{
    int idx;
    int missingMask;

    missingMask = 0;
    g_BattleRewardExp = 0;
    g_BattleRewardAp = 0;
    g_BattleRewardTotals.gil = 0;
    for (idx = 0; idx < 4; idx++) {
        g_BattleRewardItems[idx].itemId = 0xFFFF;
        g_BattleRewardItems[idx].selected = 0;
    }
    for (idx = 0; idx < 3; idx++) {
        g_BattleRewardTotals.party[idx].limitUnlocks = 0;
        g_BattleRewardTotals.party[idx].displayedLevel = 0;
    }
    for (idx = 0; idx < 3; idx++) {
        if (gSavemap.party[idx] == PARTY_CHAR_NONE) {
            missingMask |= 1 << idx;
        }
    }
    Battle_InitExpResultRows(missingMask);
}
} /* extern "C" */

// src/ff7/src/battle/battle_load_party_actors.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Loads and refreshes party combatants from the savemap character records, applying
//   equipment, accessory status, limit break slots, targeting and Battle Square handicaps.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/codegen.h"
#include "ff7/enums/battle.h"
#include "ff7/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "ff7/types/menu.h"
#include "lib/api/token.h"
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
extern "C" {

/* battle.cpp's battle staging block; the party loader copies the loaded character's savemap record
 * to +0x9C4 in it. */

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)
// The Battle Square handicap tables, indexed by handicap kind (a bit of g_BtlHandicapFlags): the
// status each applies and the parameter of the hit-animation event Battle_ApplyBattleSquareHandicap
// posts for it.
// GLOBAL: FF7 0x007b76c8
extern const int g_BattleSquareHandicapStatusMasks[4] = {0, BATTLE_STATUS_SMALL, BATTLE_STATUS_POISON, BATTLE_STATUS_FROG};
// GLOBAL: FF7 0x007b76d8
extern const int g_BattleSquareHandicapEventIds[4] = {0, 0xa, 0x27, 0xa};


/*
 * Loads party combatants from savemap character records, equipping weapons, armour,
 * accessories, and materia into battle actor structures.
 */
// FUNCTION: FF7 0x005cf650
void __cdecl Battle_LoadPartyActorsFromEquipment(void)
{
    int saveIdx;
    int slot;
    BattlePartyStatWords *statWords;
    BattleWeaponActionRow *partyMemberPtr;
    BattleCombatant *pBattler;
    int charId;
    BattlePartyEquipSetup *equipSetup;
    int workLimitLvl;
    short slotWord;
    BattleArmorDataRecord *armorRecord;
    int lane;
    int speedSum;
    int count;
    int attackSlot;
    BattleActorRuntime *runtimePtr;
    BattlePartyActor *partyActor;
    SavemapCharacter *charRec;

    speedSum = 0;
    count = 0;
    gBattleAiGlobals.targetMask = 0;
    for (slot = 0; slot < 3; slot++) {
        runtimePtr = &g_BattleState.actors[slot];
        partyActor = &g_BattleState.partyStats[slot];
        partyMemberPtr = &g_BattleState.weaponActionRows[slot];
        statWords = (BattlePartyStatWords *)&g_MenuPartyStats[slot];
        pBattler = &g_aCombatants[slot];
        charId = gSavemap.party[slot];
        g_BattleScene.actors[slot].unitIndex = 0xFF;
        if (charId != PARTY_CHAR_NONE) {
            for (saveIdx = 0; saveIdx < 9; saveIdx++) {
                charRec = &gSavemap.chars[saveIdx];
                if ((g_BattleScene.flags & 0x40) != 0) {
                    g_BattleItemListBlock.savedCharacter = gSavemap.chars[saveIdx];
                }
                if (charRec->id == charId) {
                    lane = 1;
                    runtimePtr->timingFlags = 1;
                    runtimePtr->linkedActor = 0xFF;
                    runtimePtr->nameSuffixIndex = 0xFF;
                    g_BattleScene.actors[slot].unitIndex = charId;
                    pBattler->unitId = charId;
                    pBattler->sideSlot = charId + 0x10;
                    pBattler->level = charRec->level;
                    pBattler->idleOverride = 0;
                    pBattler->hitAnimId = 8;
                    pBattler->animId = 5;
                    pBattler->flags = BATTLE_COMBATANT_TARGETABLE;
                    partyActor->partyMember = charRec;
                    *(short *)&gBattleAiGlobals.targetMask |= (short)(1 << slot);
                    if ((charRec->row & 1) == 0) {
                        pBattler->flags |= BATTLE_COMBATANT_BACK_ROW;
                    }
                    pBattler->curHp = statWords->hp;
                    pBattler->curMp = statWords->mp;
                    partyActor->hp = *(short *)&pBattler->curHp;
                    partyActor->mp = (*(short *)&pBattler->curMp);
                    Battle_CopyPartyStatsToCombatant(statWords, partyActor, pBattler);
                    pBattler->damageScale = 0x10;
                    pBattler->status = charRec->status & (BATTLE_STATUS_SADNESS | BATTLE_STATUS_FURY);
                    pBattler->prevStatusFlags = 0;
                    runtimePtr->statusImmunityMask = statWords->auxWord;
                    equipSetup = (BattlePartyEquipSetup *)&statWords->savedFlags;
                    partyMemberPtr->targetFlags = equipSetup->flags;
                    partyMemberPtr->resolvedAttackId = equipSetup->resolvedAttackId;
                    partyMemberPtr->damageCalcByte = equipSetup->damageCalcByte;
                    partyMemberPtr->accuracy = equipSetup->accuracy;
                    partyMemberPtr->displayValue = equipSetup->displayValue;
                    partyMemberPtr->criticalBonus = equipSetup->criticalBonus;
                    partyMemberPtr->elementMask = equipSetup->elementMask | statWords->extraBits;
                    partyMemberPtr->animParam = equipSetup->animParam;
                    partyMemberPtr->specialFlags = equipSetup->specialFlags;
                    partyMemberPtr->statusMask = statWords->stateWord;
                    armorRecord = (BattleArmorDataRecord *)g_KernelArmorDataArray[charRec->armor].headerBytes;
                    pBattler->defensePercent = armorRecord->defensePercent;
                    pBattler->magicDefensePercent = armorRecord->magicDefensePercent;
                    Kernel_ApplyAccessoryAutoStatus(slot, charRec->accessory);
                    pBattler->spoilA = 0;
                    pBattler->spoilB = 0xFFFF;
                    for (attackSlot = 0; attackSlot < 3; attackSlot++) {
                        slotWord = equipSetup->slotBytes[attackSlot];
                        if ((equipSetup->slotBits & lane) != 0) {
                            slotWord |= 0x100;
                        }
                        partyMemberPtr->slotWords[attackSlot] = slotWord;
                        lane <<= 1;
                    }
                    runtimePtr->timingFlags &= 0xFD;
                    if ((statWords->flags & 4) != 0) {
                        partyMemberPtr->targetFlags &= 0xDF;
                    }
                    if ((partyMemberPtr->targetFlags & BTL_TARGETF_SHORT_RANGE) == 0) {
                        runtimePtr->timingFlags |= 2;
                    }
                    runtimePtr->atbGauge = 0;
                    statWords->clearedByte = 0;
                    statWords->atbGauge = 0;
                    statWords->barrierGauge = 0;
                    statWords->mBarrierGauge = 0;
                    statWords->equipWord = 0;
                    statWords->rowFlag = 1;
                    partyActor->limitLevelIndex = 0xFF;
                    if (charId < 9) {
                        workLimitLvl = charRec->limitLevel;
                        if (workLimitLvl >= 1 && workLimitLvl <= 4) {
                            workLimitLvl--;
                            partyActor->limitGauge = charRec->limitBar;
                            partyActor->loadLimitGauge = partyActor->limitGauge;
                            partyActor->limitLevelIndex = workLimitLvl;
                            partyActor->limitLevelParam = g_BattleKernelData.chars[charId].limitLevelParams[workLimitLvl];
                            statWords->equipWord = *(unsigned short *)&partyActor->limitGauge << 8;
                            statWords->rowFlag = charRec->limitLevel;
                            Kernel_MarkUsableLimits(charId, charRec->limits, (BattleLimitSlots *)statWords->limitAttacks);
                        }
                    }
                    if (REG_ALLOC_HINT) {
                    }
                    pBattler->flags |= BATTLE_COMBATANT_TARGETABLE;
                    if (pBattler->curHp == 0) {
                        pBattler->status |= BATTLE_STATUS_DEATH;
                    }
                    Battle_RefreshActorCommandTargeting(slot);
                    Battle_BuildFrogCommandSubstitute(slot);
                    if (*(unsigned short *)&partyActor->limitGauge == 0xFF) {
                        Battle_SetLimitReady(slot);
                        g_BattleState.actors[slot].limitFlags &= 0xFFFE;
                    }
                    if (pBattler->status != 0) {
                        Battle_StopActorStatusEffect(slot);
                    }
                    partyActor->materiaTypeBits = Kernel_CollectEquippedMateriaType7Bits(charRec);
                    partyActor->loadMateriaTypeBits = partyActor->materiaTypeBits;
                    speedSum += charRec->dexterity;
                    count++;
                    break;
                }
            }
        }
    }
    if (count != 0) {
        g_BattleState.atbBaseDivisor = (speedSum + count + -1) / count + 0x32;
    }
}

/*
 * Reloads party actors from savemap records, updating stats, targeting, and applying
 * accessory auto-status and Battle Square handicaps.
 */
// FUNCTION: FF7 0x005cfbf5
void __cdecl Battle_ReloadPartyActorsFromSavemap(void)
{
    BattleCombatant *combatant;
    BattleActorRuntime *actorSnapshot;
    BattlePartyStatWords *stats;
    BattleWeaponActionRow *pWeaponRow;
    int saveIdx;
    int slot;
    BattlePartyActor *partyActor;
    int charId;
    BattlePartyEquipSetup *equipSetup;
    SavemapCharacter *charRec;

    for (slot = 0; slot < 3; slot++) {
        actorSnapshot = &g_BattleState.actors[slot];
        partyActor = &g_BattleState.partyStats[slot];
        pWeaponRow = &g_BattleState.weaponActionRows[slot];
        stats = (BattlePartyStatWords *)&g_MenuPartyStats[slot];
        combatant = &g_aCombatants[slot];
        charId = gSavemap.party[slot];
        if (charId != PARTY_CHAR_NONE) {
            for (saveIdx = 0; saveIdx < 9; saveIdx++) {
                charRec = &gSavemap.chars[saveIdx];
                if (charRec->id == charId) {
                    combatant->level = charRec->level;
                    combatant->curHp = charRec->hp;
                    combatant->curMp = charRec->mp;
                    actorSnapshot->hp = *(short *)&combatant->curHp;
                    actorSnapshot->mp = (*(short *)&combatant->curMp);
                    Battle_CopyPartyStatsToCombatant(stats, partyActor, combatant);
                    actorSnapshot->statusImmunityMask = stats->auxWord;
                    equipSetup = (BattlePartyEquipSetup *)&stats->savedFlags;
                    pWeaponRow->elementMask = equipSetup->elementMask | stats->extraBits;
                    pWeaponRow->statusMask = stats->stateWord;
                    pWeaponRow->accuracy = stats->weaponAccuracy;
                    pWeaponRow->targetFlags = stats->savedFlags;
                    actorSnapshot->timingFlags &= 0xFD;
                    if ((stats->flags & 4) != 0) {
                        pWeaponRow->targetFlags &= 0xDF;
                    }
                    if ((pWeaponRow->targetFlags & BTL_TARGETF_SHORT_RANGE) == 0) {
                        actorSnapshot->timingFlags |= 2;
                    }
                    Kernel_ApplyAccessoryAutoStatus(slot, charRec->accessory);
                    Battle_RefreshActorCommandTargeting(slot);
                    Battle_BuildFrogCommandSubstitute(slot);
                    if (Battle_ApplyBattleSquareHandicap(slot) == 0) {
                        Battle_StopActorStatusEffect(slot);
                    }
                    break;
                }
            }
        }
    }
}

/*
 * Executes initialization AI script events for each active party combatant.
 */
// FUNCTION: FF7 0x005cfe10
void __cdecl Battle_RunPartyAiInitEvents(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (g_BattleScene.actors[i].unitIndex != -1 &&
            (g_aCombatants[i].status & BATTLE_STATUS_DEATH) == 0) {
            Battle_RunAiScriptEvent(i, 0, 0);
        }
    }
}

/*
 * Updates command and ability targeting flags and page counts for a party combatant.
 */
// FUNCTION: FF7 0x005cfe67
void __cdecl Battle_RefreshActorCommandTargeting(int actorIndex)
{
    struct MenuPartyStats *statWords;
    BattleWeaponActionRow *actorBase;
    int slotIdx;
    int flagByte;
    int entIndex;
    int commandFlags;

    statWords = &g_MenuPartyStats[actorIndex];
    actorBase = &g_BattleState.weaponActionRows[actorIndex];
    statWords->commandColumns = 1;
    for (slotIdx = 0; slotIdx < 16; slotIdx++) {
        commandFlags = 0xFF;
        if (statWords->commands[slotIdx].commandId != 0xFF) {
            commandFlags = g_KernelCommandTable[statWords->commands[slotIdx].commandId].targetFlags;
            if (commandFlags == 0xFF) {
                commandFlags = g_BattleState.weaponActionRows[actorIndex].targetFlags;
            }
            switch (statWords->commands[slotIdx].commandId) {
            case BATTLE_COMMAND_SLASH_ALL:
            case BATTLE_COMMAND_2X_CUT:
            case BATTLE_COMMAND_FLASH:
            case BATTLE_COMMAND_4X_CUT:
                statWords->commands[slotIdx].allCount = 0xFF;
                break;
            }
            if (statWords->commands[slotIdx].cursorAction == BATTLE_CURSOR_ACTION_TARGET_VALID_MASK) {
                if ((g_BattleState.actors[actorIndex].timingFlags & 2) != 0) {
                    statWords->commands[slotIdx].cursorAction = BATTLE_CURSOR_ACTION_TARGET;
                }
                if (statWords->commands[slotIdx].allCount != 0 && statWords->commands[slotIdx].commandId != BATTLE_COMMAND_2X_CUT) {
                    commandFlags |= BTL_TARGETF_DEFAULT_MULTIPLE | BTL_TARGETF_TOGGLE_MULTIPLE;
                }
                switch (statWords->commands[slotIdx].commandId) {
                case BATTLE_COMMAND_STEAL:
                case BATTLE_COMMAND_MUG:
                    commandFlags |= BTL_TARGETF_ONE_SIDE_ONLY;
                    if (statWords->commands[slotIdx].allCount != 0) {
                        statWords->commands[slotIdx].cursorAction = BATTLE_CURSOR_ACTION_TARGET;
                    }
                    break;
                }
            }
            statWords->commandColumns = slotIdx / 4 + 1;
        }
        statWords->commands[slotIdx].targetFlags = commandFlags;
    }
    for (slotIdx = 0; slotIdx < 96; slotIdx++) {
        entIndex = statWords->abilities[slotIdx].attackId;
        if (entIndex != 0xFF) {
            if (slotIdx >= 72) {
                entIndex += 72;
            } else if (slotIdx >= 56) {
                entIndex += 56;
            }
            if (slotIdx < 56) {
                flagByte = g_BattleAttackTable[entIndex].targetFlags;
                if ((flagByte & BTL_TARGETF_TOGGLE_MULTIPLE) == 0) {
                    statWords->abilities[slotIdx].allCount = 0;
                }
            }
        }
    }
}

/*
 * Resets limit break command subrecords and enables the limit break flag for an actor.
 */
// FUNCTION: FF7 0x005d0097
void __cdecl Battle_ResetLimitBreakSlots(int actorIndex)
{
    struct MenuPartyStats *statWords;
    int slotIdx;

    statWords = &g_MenuPartyStats[actorIndex];
    statWords->commandColumns = 1;
    for (slotIdx = 1; slotIdx < 4; slotIdx++) {
        statWords->commands[slotIdx].commandId = 0xFF;
        statWords->commands[slotIdx].cursorAction = 0;
        statWords->commands[slotIdx].targetFlags = 0;
        statWords->commands[slotIdx].menuFlags = 3;
        statWords->commands[slotIdx].allCount = 0;
        statWords->commands[slotIdx].supportFlags = 0;
    }
}

/*
 * Checks learned limit break abilities against weapon attack IDs and marks usable limits.
 */
// FUNCTION: FF7 0x005d0127
void __cdecl Kernel_MarkUsableLimits(int charId, int learnedMask, BattleLimitSlots *limitSlots)
{
    int slotIndex;
    int usableCount;
    int bitIndex;

    usableCount = 0;
    for (slotIndex = 0; slotIndex < 3; slotIndex++) {
        if (limitSlots->attackIds[slotIndex] != 0xFF) {
            for (bitIndex = 0; bitIndex < 0xC; bitIndex++) {
                if (Kernel_GetLimitAttackId(charId, bitIndex) == limitSlots->attackIds[slotIndex]) {
                    break;
                }
            }
            if ((learnedMask & 1 << bitIndex) != 0) {
                /* the byte at +0x14 of the stat record's 0x1C-byte entry for the slot */
                limitSlots->targetFlags[slotIndex] = *(char *)((char *)limitSlots + slotIndex * 0x1C + 0x14);
                usableCount++;
            }
        }
    }
    limitSlots->clearedByte = 0;
    limitSlots->usableCount = usableCount;
}

/*
 * Accumulates type-7 materia bits across all equipped weapon and armour materia slots.
 */
// FUNCTION: FF7 0x005d01e7
int __cdecl Kernel_CollectEquippedMateriaType7Bits(SavemapCharacter *charRecord)
{
    int flags;
    int slotIndex;

    flags = 0;
    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        flags |= Kernel_GetMateriaType7Bits(charRecord->weaponMateria[slotIndex]);
        flags |= Kernel_GetMateriaType7Bits(charRecord->armorMateria[slotIndex]);
    }
    return flags;
}

/*
 * Extracts type-7 materia configuration bits for a single materia slot.
 */
// FUNCTION: FF7 0x005d024b
int __cdecl Kernel_GetMateriaType7Bits(int materiaSlot)
{
    int result;
    int materiaId;

    result = 0;
    materiaId = materiaSlot & 0xFF;
    if (materiaId != 0xFF && (g_KernelMateriaDataArray[materiaId].typeNibbleByte & 0xF) == 7) {
        result = (unsigned int)materiaSlot >> 8 | 0x80000000;
    }
    return result;
}

/*
 * Applies automatic status effects granted by the combatant's equipped accessory.
 */
// FUNCTION: FF7 0x005d0297
void __cdecl Kernel_ApplyAccessoryAutoStatus(int charIndex, int accessoryIndex)
{
    struct KernelAccessory *accessoryRecord;
    BattleCombatant *combatantRec;
    BattleAutoStatusRecord *stateRecord;
    BattlePartyActor *partyStat;

    stateRecord = (BattleAutoStatusRecord *)&g_BattleState.actors[charIndex];
    partyStat = &g_BattleState.partyStats[charIndex];
    combatantRec = &g_aCombatants[charIndex];
    combatantRec->status &= ~partyStat->accessoryStatus;
    stateRecord->statusFlags &= ~partyStat->accessoryStatus;
    partyStat->accessoryStatus = 0;
    stateRecord->accessoryKind = 0xFF;
    if (accessoryIndex != 0xFF) {
        accessoryRecord = &g_KernelAccessoryDataArray[accessoryIndex];
        stateRecord->accessoryKind = accessoryRecord->autoStatusKind;
        switch (*(unsigned char *)&stateRecord->accessoryKind) {
        case BATTLE_ACCESSORY_AUTO_HASTE:
            combatantRec->status |= BATTLE_STATUS_HASTE;
            stateRecord->statusFlags |= BATTLE_STATUS_HASTE;
            partyStat->accessoryStatus |= BATTLE_STATUS_HASTE;
            break;
        case BATTLE_ACCESSORY_AUTO_BERSERK:
            combatantRec->status |= BATTLE_STATUS_BERSERK;
            stateRecord->statusFlags |= BATTLE_STATUS_BERSERK;
            partyStat->accessoryStatus |= BATTLE_STATUS_BERSERK;
            break;
        case BATTLE_ACCESSORY_AUTO_DEATH_SENTENCE:
            combatantRec->status |= BATTLE_STATUS_DEATH_SENTENCE;
            stateRecord->statusFlags |= BATTLE_STATUS_DEATH_SENTENCE;
            partyStat->accessoryStatus |= BATTLE_STATUS_DEATH_SENTENCE;
            stateRecord->deathSentenceTimer = 0xFF;
            break;
        case BATTLE_ACCESSORY_AUTO_REFLECT:
            combatantRec->status |= BATTLE_STATUS_REFLECT;
            stateRecord->statusFlags |= BATTLE_STATUS_REFLECT;
            partyStat->accessoryStatus |= BATTLE_STATUS_REFLECT;
            break;
        case BATTLE_ACCESSORY_AUTO_BARRIERS:
            combatantRec->status |= BATTLE_STATUS_BARRIER | BATTLE_STATUS_MBARRIER;
            partyStat->accessoryStatus |= BATTLE_STATUS_BARRIER | BATTLE_STATUS_MBARRIER;
        }
    }
}

/*
 * Applies Battle Square handicap status effects and HP/MP penalties to the combatant.
 */
// FUNCTION: FF7 0x005d0463
int __cdecl Battle_ApplyBattleSquareHandicap(int actorIndex)
{
    int result;
    int handicapIdx;
    int exemptMask;

    exemptMask = g_BattleState.actors[actorIndex].statusImmunityMask;
    result = 0;
    g_aCombatants[actorIndex].status &= ~BATTLE_STATUS_DEATH_SENTENCE;
    if ((g_BattleState.actors[actorIndex].timingFlags & 8) != 0) {
        exemptMask |= BATTLE_STATUS_FROG;
    }
    if ((*(unsigned char *)&g_BtlHandicapFlags & 0x10) != 0) {
        g_aCombatants[actorIndex].curHp += (unsigned int)g_aCombatants[actorIndex].maxHp >> 1;
        if ((unsigned int)g_aCombatants[actorIndex].curHp > (unsigned int)g_aCombatants[actorIndex].maxHp) {
            g_aCombatants[actorIndex].curHp = g_aCombatants[actorIndex].maxHp;
        }
        Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, actorIndex, BATTLE_ACTION_EVENT_PUSH_HIT_ANIM, 0);
    }
    for (handicapIdx = 0; handicapIdx < 4; handicapIdx++) {
        if ((*(unsigned char *)&g_BtlHandicapFlags & 1 << handicapIdx) != 0) {
            g_aCombatants[actorIndex].status |= g_BattleSquareHandicapStatusMasks[handicapIdx] & ~exemptMask;
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ACTION, actorIndex, BATTLE_ACTION_EVENT_PUSH_HIT_ANIM, g_BattleSquareHandicapEventIds[handicapIdx]);
            result = 1;
        }
    }
    return result;
}

/*
 * Copies party character stats to the battle combatant record and enforces stat caps.
 */
// FUNCTION: FF7 0x005d05bd
void __cdecl Battle_CopyPartyStatsToCombatant(BattlePartyStatWords *partyStats, BattlePartyActor *partyActor, BattleCombatant *combatant)
{
    combatant->dexterity = partyStats->dexterity;
    combatant->luck = partyStats->luck;
    combatant->maxHp = partyStats->maxHp;
    combatant->maxMp = partyStats->maxMp;
    combatant->attack = partyStats->attack;
    combatant->magicAttack = partyStats->magicAttack;
    combatant->defense = partyStats->defense;
    combatant->magicDefense = partyStats->magicDefense;
    if (combatant->attack == 0) {
        combatant->attack = 1;
    }
    partyActor->maxHp = *(short *)&combatant->maxHp;
    partyActor->maxMp = (*(short *)&combatant->maxMp);
    if ((partyStats->flags & 8) != 0) {
        partyActor->hpCap = 0x3E7;
        partyActor->mpCap = 0x270F;
    } else {
        partyActor->hpCap = 0x270F;
        partyActor->mpCap = 0x3E7;
    }
}
} /* extern "C" */

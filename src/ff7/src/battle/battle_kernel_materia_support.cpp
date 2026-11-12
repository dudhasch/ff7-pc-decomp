// src/ff7/src/battle/battle_kernel_materia_support.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Kernel materia support: awards AP to equipped materia and applies each materia's effect
//   to the actor's command, magic, summon and ability lists. Also computes materia levels and the
//   party's encounter modifiers.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/types/menu.h"
#include "ff7/enums/battle.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "lib/api/token.h"
#include "ff7/savemap.h"
#include "ff7/enums/menu.h"
#include "ff7/enums/main.h"
extern "C" {

struct MenuPartyStats;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

struct MateriaScratchPair {
    union {
        short value;
        char low;
    };
    union {
        short second;
        char secondLow;
    };
};

// clang-format off
// Per stat-selector row (materia statSelectorByte): the bonus to strength, vitality, magic, spirit, dexterity, luck, max HP % and max MP %.
// GLOBAL: FF7 0x008feec8
short g_MateriaStatBonusTable[21][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {-2, -1, 2, 1, 0, 0, -5, 5},
    {-4, -2, 4, 2, 0, 0, -10, 10},
    {0, 0, 0, 0, 2, -2, 0, 0},
    {-1, -1, 1, 1, 0, 0, 0, 0},
    {1, 1, -1, -1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, -1, 0, 0},
    {0, 0, 0, 0, -2, 0, 0, 0},
    {0, 0, 0, 0, 2, 0, 0, 0},
    {-1, 0, 1, 0, 0, 0, -2, 2},
    {0, 0, 1, 0, 0, 0, -2, 2},
    {0, 0, 1, 1, 0, 0, -5, 5},
    {0, 0, 2, 2, 0, 0, -10, 10},
    {0, 0, 4, 4, 0, 0, -10, 15},
    {0, 0, 8, 8, 0, 0, -10, 20},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};
// GLOBAL: FF7 0x008ff018
short g_MateriaElementBitmaskTable[16] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, -0x8000};
// 56 five-byte magic list records: the magic id, then the all-count, quadra-link count, quadra count and support byte.
// GLOBAL: FF7 0x00c06750
struct BattleMagicListRecord g_MateriaMagicListRecords[56] = {0};
// The encounter-rate sum of the materia being applied (short): Kernel_ApplyMateriaEncounterRate adds the
// level bonus of materia 7 to it, Kernel_StoreEncounterModifiersToStatBlock clamps it to 255 and adds its low byte
// to the stat block (+0x437), and Kernel_InitActorMateriaScratchState clears it.
// GLOBAL: FF7 0x00c06868
short g_MateriaEncounterRateSum = 0;
// GLOBAL: FF7 0x00c0686c
int g_MateriaLookupLevel = 0;
// The level count of the materia last looked up (int): Kernel_GetMateriaLevel stores 1 + the number of its AP
// thresholds; Savemap_AwardApToEquippedMateria compares a new level with it (the mastered level).
// GLOBAL: FF7 0x00c06870
int g_MateriaMasteredLevelCount = 0;
unsigned char g_bssPad_00c06874[4] = {0};
// GLOBAL: FF7 0x00c06878
struct MateriaScratchPair g_MateriaStatScratchLevel = {0};
// GLOBAL: FF7 0x00c0687c
struct MateriaScratchPair g_MateriaStatScratchStrength = {0};
// GLOBAL: FF7 0x00c06880
struct MateriaScratchPair g_MateriaStatScratchMagic = {0};
// GLOBAL: FF7 0x00c06884
struct MateriaScratchPair g_MateriaStatScratchDexterity = {0};
// GLOBAL: FF7 0x00c06888
struct MateriaScratchPair g_MateriaStatScratchAttack = {0};
// GLOBAL: FF7 0x00c0688c
struct MateriaScratchPair g_MateriaStatScratchMagicAttack = {0};
// GLOBAL: FF7 0x00c06890
struct MateriaScratchPair g_MateriaStatScratchMaxHp = {0};
unsigned char g_bssPad_00c06894[4] = {0};
// The twelve percent modifiers, also addressable by number (byIndex).
// GLOBAL: FF7 0x00c06898
struct MateriaStatPercentModifiers g_MateriaStatPercentModifiers = {0};
// GLOBAL: FF7 0x00c068b0
struct MenuPartyStats *g_pCurrentCombatantStatBlock = 0;
// GLOBAL: FF7 0x00c068b4
char g_MateriaDetailPanelKind = 0;
// The short pendingTrigger, followed by the byte table of level parameters read as
// levelParamBase[level] for level 1..5.
// GLOBAL: FF7 0x00c068b8
struct MateriaTriggerDescriptor g_MateriaTriggerDescriptor = {0};
// GLOBAL: FF7 0x00c068c8
int g_MateriaTriggerCount = 0;
// GLOBAL: FF7 0x00c068cc
char g_MateriaSupportEffectFlags = 0;
// Materia slots of the combatant being loaded: 8 weapon then 8 armour ids (16 ints).
// Battle_LoadCombatantFromSavemap stores g_MateriaEquippedSlotIds[0..15] and reads the armour half from [8].
// GLOBAL: FF7 0x00c068d0
int g_MateriaEquippedSlotIds[16] = {0};
// GLOBAL: FF7 0x00c06910
struct BattleMateriaStatPlusAbility *g_MateriaCurrentAbilityMask = 0;
// GLOBAL: FF7 0x00c06914
unsigned char g_MateriaDescribeModeFlag = 0;
// GLOBAL: FF7 0x00c06918
char g_MateriaCommandListRecordCount = 0;
unsigned char g_bssPad_00c0691c[4] = {0};
// 16 three-byte command list entries: the command code, the level and the support flags.
// GLOBAL: FF7 0x00c06920
struct BattleCommandListEntry g_MateriaCommandListEntries[16] = {0};
// GLOBAL: FF7 0x00c06950
unsigned char g_MateriaPreEmptiveRateSum = 0;
unsigned char g_bssPad_00c06954[4] = {0};
// The four encounter/pre-emptive modifier sums (short[4]) indexed by the materia's sub-type byte:
// Kernel_ApplyMateriaEncounterModifier and Kernel_ApplyMateriaEncounterRate add to [type], the pre-emptive materia clamps
// [3] to 85, Kernel_StoreEncounterModifiersToStatBlock clamps [0], [1], [2] and adds their low bytes to the stat
// block (+0x43C..+0x43F), and Kernel_InitActorMateriaScratchState clears all four.
// GLOBAL: FF7 0x00c06958
short g_MateriaEncounterModifiers[4] = {0};
// The summon use counts of the actor being loaded (16 bytes, one per summon): Kernel_AddMpTurboTo*SummonEntries
// and the init read and write [p] for p < 16 and Kernel_CopySummonEntryBytesToStatBlock copies them to +0x2CB.
// Kernel_ApplySummonMateria addresses the parallel table g_MateriaSummonLevelCounts as [8 + summon id] past the end of this
// array (summon ids are 0x38..0x47): the TU's .bss keeps the retail layout of these objects.
// GLOBAL: FF7 0x00c06960
char g_MateriaSummonUseCounts[0x10] = {0};
// GLOBAL: FF7 0x00c06970
int g_MateriaTriggerEntryCode = 0;
// GLOBAL: FF7 0x00c06974
char g_MateriaCommandCount = 0;
// GLOBAL: FF7 0x00c06978
unsigned char g_MateriaMagicListCount = 0;
// The number of battle menu commands Kernel_BuildBattleCommandMenu placed.
// GLOBAL: FF7 0x00c0697c
int g_MateriaBattleCommandPlacedCount = 0;
// The six additive stat bonuses of the actor being loaded (short[6]): Kernel_ApplyMateriaStatBonus adds the
// materia's bonus row to [0..5], Kernel_ApplyMateriaStatModifiersToStatBlock masks and adds them to the six base
// stats, and Kernel_InitActorMateriaScratchState clears [0..5].
// GLOBAL: FF7 0x00c06980
short g_MateriaAdditiveStatBonuses[6] = {0};
// GLOBAL: FF7 0x00c0698c
unsigned char g_MateriaSummonCount = 0;
// g_MateriaQuadraMagicCounts is indexed by the code as an array (read by
// Kernel_AddQuadraMagicToAllSummonEntries, Kernel_CopySummonEntryBytesToStatBlock,
// Kernel_InitActorMateriaScratchState). The span runs to the next named definition of the TU.
// GLOBAL: FF7 0x00c06990
char g_MateriaQuadraMagicCounts[0x10] = {0};
// g_MateriaSummonLevelCounts is indexed by the code as an array (read by
// Kernel_ApplyMasterSummonMateria, Kernel_CopySummonEntryBytesToStatBlock,
// Kernel_InitActorMateriaScratchState). The span runs to the next named definition of the TU.
// GLOBAL: FF7 0x00c069a0
char g_MateriaSummonLevelCounts[0x10] = {0};
// GLOBAL: FF7 0x00c069b0
short g_MateriaLevelUpSlotMask[3] = {0};
// clang-format on

/*
 * Clears the party materia-level notification mask and global mode, then skips empty party slots.
 * Visits eight weapon and eight armor materia slots, excluding empty slots, Enemy Skill and the mastered AP
 * sentinel. Adds the low 16 bits of awarded AP with growth multiplier zero, one, two, or three
 * from weapon or armor data, repacks AP modulo 24 bits, and rechecks level. A level increase marks
 * its corresponding bit; reaching the global maximum level invokes the creation helper and stores
 * the mastered sentinel. No AP saturation occurs before repacking.
 */
// FUNCTION: FF7 0x005cad70
void __cdecl Savemap_AwardApToEquippedMateria(short partySlot, int apAmount)
{
    int p;
    int *materiaCursor;
    int a;
    int sum;
    int newLevel;
    int mid;
    int i;
    KernelArmorRecord *arm;
    int prev;

    g_MateriaDescribeModeFlag = 0;
    g_MateriaLevelUpSlotMask[partySlot] = 0;
    if (gSavemap.party[partySlot] == PARTY_CHAR_NONE) {
        return;
    }
    g_pCurrentCombatantStatBlock = Savemap_GetPartyStatBlock(partySlot);
    materiaCursor = gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].weaponMateria;
    for (i = 0; i < 8; i++) {
        /* Retail caller reads residual EAX/AX as `int` after `void Kernel_UnpackMateriaAndGetLevel` returns. */
        prev = ((int(__cdecl *)(int, int *, int *))Kernel_UnpackMateriaAndGetLevel)(*materiaCursor, &sum, &mid);
        if ((mid & 0xFF) != 0xFF && (mid & 0xFF) != MATERIA_ENEMY_SKILL && sum != 0xFFFFFF) {
            switch (g_pCurrentCombatantStatBlock->weaponGrowthRate) {
            case GROWTH_NONE:
                break;
            default:
                sum = sum + (apAmount & 0xFFFF);
                break;
            case GROWTH_DOUBLE:
                sum = sum + (apAmount & 0xFFFF) * 2;
                break;
            case GROWTH_TRIPLE:
                sum = sum + (apAmount & 0xFFFF) * 3;
                break;
            }
            *materiaCursor = (sum & 0xFFFFFF) << 8 | mid & 0xFF;
            newLevel = ((int(__cdecl *)(int, int *, int *))Kernel_UnpackMateriaAndGetLevel)(*materiaCursor, &sum, &mid);
            if (prev < newLevel) {
                g_MateriaLevelUpSlotMask[partySlot] |= (short)(1 << i);
                if (newLevel == g_MateriaMasteredLevelCount) {
                    Savemap_AddMateriaToStock(mid & 0xFF);
                    *materiaCursor = mid & 0xFF | 0xFFFFFF00;
                }
            }
        }
        materiaCursor++;
    }
    arm = (KernelArmorRecord *)Kernel_GetArmorRecord(
        gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].armor);
    materiaCursor = gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].armorMateria;
    for (i = 0; i < 8; i++) {
        /* Retail caller reads residual EAX/AX as `int` after `void Kernel_UnpackMateriaAndGetLevel` returns. */
        prev = ((int(__cdecl *)(int, int *, int *))Kernel_UnpackMateriaAndGetLevel)(*materiaCursor, &sum, &mid);
        if ((mid & 0xFF) != 0xFF && (mid & 0xFF) != MATERIA_ENEMY_SKILL && sum != 0xFFFFFF) {
            switch (arm->growthRate) {
            case GROWTH_NONE:
                break;
            default:
                sum = sum + (apAmount & 0xFFFF);
                break;
            case GROWTH_DOUBLE:
                sum = sum + (apAmount & 0xFFFF) * 2;
                break;
            case GROWTH_TRIPLE:
                sum = sum + (apAmount & 0xFFFF) * 3;
                break;
            }
            *materiaCursor = (sum & 0xFFFFFF) << 8 | mid & 0xFF;
            newLevel = ((int(__cdecl *)(int, int *, int *))Kernel_UnpackMateriaAndGetLevel)(*materiaCursor, &sum, &mid);
            if (prev < newLevel) {
                g_MateriaLevelUpSlotMask[partySlot] |= (short)(1 << (i + 8));
                if (newLevel == g_MateriaMasteredLevelCount) {
                    Savemap_AddMateriaToStock(mid & 0xFF);
                    *materiaCursor = mid & 0xFF | 0xFFFFFF00;
                }
            }
        }
        materiaCursor++;
    }
}

/*
 * Writes packed low byte through idOut and its masked high bits shifted right arithmetically through
 * apOut, then calls Kernel_GetMateriaLevel with those outputs.
 */
// FUNCTION: FF7 0x005cb0f3
void __cdecl Kernel_UnpackMateriaAndGetLevel(int packed, int *apOut, char *idOut)
{
    *idOut = packed & 0xFF;
    *apOut = (packed & 0xFFFFFF00) >> 8;
    Kernel_GetMateriaLevel(*idOut, *apOut);
}

/*
 * Resets the four battle modifiers and visibility bit, then accumulates three present-party record byte
 * effects, mixing signed byte additions and unsigned sums. Computes difference with floor two only
 * when subtraction would be negative, upper-clamps packed modifiers and optionally sets high flag.
 * Accumulation narrows before clamps.
 */
// FUNCTION: FF7 0x005cb127
void __cdecl Party_RecomputeBattleModifiers(void)
{
    int upSum;
    int partySlot;
    int downSum;

    upSum = 0;
    downSum = 0;
    g_PartyBattleModifiers.gilScale = 0x10;
    g_PartyBattleModifiers.encounterRate = 0x10;
    g_PartyBattleModifiers.chocoboLure = 0;
    g_PartyBattleModifiers.preemptiveOdds = 0x10;
    gSavemap.tutSub = gSavemap.tutSub & ~0x80;
    for (partySlot = 0; partySlot < 3; partySlot++) {
        if (gSavemap.party[partySlot] == PARTY_CHAR_NONE) {
            continue;
        }
        g_pCurrentCombatantStatBlock = Savemap_GetPartyStatBlock(partySlot);
        *(char *)&g_PartyBattleModifiers.gilScale += g_pCurrentCombatantStatBlock->gilScaleBonus;
        upSum += *(unsigned char *)&g_pCurrentCombatantStatBlock->encounterRateUp;
        downSum += *(unsigned char *)&g_pCurrentCombatantStatBlock->encounterRateDown;
        *(char *)&g_PartyBattleModifiers.chocoboLure += g_pCurrentCombatantStatBlock->chocoboLureBonus;
        *(char *)&g_PartyBattleModifiers.preemptiveOdds += g_pCurrentCombatantStatBlock->preemptiveBonus;
        if ((g_pCurrentCombatantStatBlock->flags & 1) != 0) {
            gSavemap.tutSub = gSavemap.tutSub | 0x80;
        }
    }
    if (upSum + 0x10 < downSum) {
        g_PartyBattleModifiers.encounterRate = 2;
    } else {
        g_PartyBattleModifiers.encounterRate = upSum + 0x10 - downSum;
    }
    if (g_PartyBattleModifiers.gilScale > 0x20) {
        g_PartyBattleModifiers.gilScale = 0x20;
    }
    if (g_PartyBattleModifiers.encounterRate > 0x20) {
        g_PartyBattleModifiers.encounterRate = 0x20;
    }
    if (g_PartyBattleModifiers.chocoboLure > 0x20) {
        g_PartyBattleModifiers.chocoboLure = 0x20;
    }
    if (g_PartyBattleModifiers.preemptiveOdds > 0x55) {
        g_PartyBattleModifiers.preemptiveOdds = 0x55;
    }
    if (g_MateriaPreEmptiveRateSum != 0) {
        *(char *)&g_PartyBattleModifiers.preemptiveOdds |= PARTY_PREEMPTIVE_MASTERED;
    }
}

/*
 * Returns for empty party byte 0xFF. Gets armour/stat records, initializes actor through
 * Kernel_InitActorMateriaScratchState, clears retained accumulator and copies eight weapon/eight armour materia DWORDs by
 * character save-slot mapping. Applies all sixteen materia, then four weapon and four armour
 * linked pairs using slot-link bytes and armour selector. Runs four aggregate helpers, adds base
 * stat bytes two through five to retained four signed accumulators, clamps each 0..255 and writes
 * shorts eight through fourteen. Runs Kernel_StoreEncounterModifiersToStatBlock, revisits all materia through Kernel_AddMegaAllLevelToMagicAndCommandCounts and
 * sorts magic menu. For 72 magic entries with upper three flag bits nonzero, raises byte 0x109 by
 * old value times flag factor divided by ten plus one, upper-clamped 255. PARTY_STATS_HP_MP_SWAP swaps maximum
 * shorts 0x12/0x16, both floored ten. Caps current shorts 0x10/0x14 to respective maxima and
 * mirrors changed currents into savemap; always writes maxima to retained save records.
 */
// FUNCTION: FF7 0x005cb2cc
void __cdecl Battle_LoadCombatantFromSavemap(char partySlot)
{
    int armourRecord;
    char *armourCursor;
    int *dwordCursor;
    unsigned char linkFirst;
    int materiaSecond;
    int materiaFirst;
    int newValue;
    int pos;
    unsigned char slotLinkB;

    if (gSavemap.party[partySlot] == PARTY_CHAR_NONE) {
        return;
    }
    armourRecord = Savemap_GetPartyArmourRecord(partySlot);
    g_pCurrentCombatantStatBlock = Savemap_GetPartyStatBlock(partySlot);
    Kernel_InitActorMateriaScratchState((char)partySlot, g_pCurrentCombatantStatBlock);
    g_MateriaDescribeModeFlag = 0;
    dwordCursor = gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].weaponMateria;
    for (pos = 0; pos < 8; pos++) {
        g_MateriaEquippedSlotIds[pos] = *dwordCursor;
        dwordCursor++;
    }
    dwordCursor = gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].armorMateria;
    for (pos = 8; pos < 0x10; pos++) {
        g_MateriaEquippedSlotIds[pos] = *dwordCursor;
        dwordCursor++;
    }
    for (pos = 0; pos < 0x10; pos++) {
        Kernel_ApplyMateriaToActor(g_MateriaEquippedSlotIds[pos]);
    }
    armourCursor = (char *)armourRecord;
    dwordCursor = g_MateriaEquippedSlotIds;
    for (pos = 0; pos < 4; pos++) {
        materiaFirst = *dwordCursor;
        dwordCursor++;
        materiaSecond = *dwordCursor;
        dwordCursor++;
        /* Kernel_ApplyLinkedMateriaPair takes int selectors; these calls push the two link bytes unextended. */
        ((void(__cdecl *)(char, char, int, int, int, int, int))Kernel_ApplyLinkedMateriaPair)(g_pCurrentCombatantStatBlock->weaponMateriaSlots[pos * 2], g_pCurrentCombatantStatBlock->weaponMateriaSlots[pos * 2 + 1], materiaFirst, materiaSecond, 0, 0, 0);
    }
    dwordCursor = &g_MateriaEquippedSlotIds[8];
    for (pos = 0; pos < 4; pos++) {
        materiaFirst = *dwordCursor;
        dwordCursor++;
        materiaSecond = *dwordCursor;
        dwordCursor++;
        linkFirst = *armourCursor;
        armourCursor++;
        slotLinkB = *armourCursor;
        armourCursor++;
        ((void(__cdecl *)(char, char, int, int, int, int, int))Kernel_ApplyLinkedMateriaPair)((char)linkFirst, (char)slotLinkB, materiaFirst, materiaSecond, 0, 0, 1);
    }
    Kernel_CopyMagicListToAbilityEntries();
    Kernel_BuildBattleCommandMenu();
    Kernel_CopySummonEntryBytesToStatBlock();
    Kernel_ApplyMateriaStatModifiersToStatBlock();
    g_MateriaStatScratchAttack.value = g_pCurrentCombatantStatBlock->strength + g_MateriaStatScratchAttack.value;
    g_MateriaStatScratchAttack.second = g_pCurrentCombatantStatBlock->vitality + g_MateriaStatScratchAttack.second;
    g_MateriaStatScratchMagicAttack.value = g_pCurrentCombatantStatBlock->magic + g_MateriaStatScratchMagicAttack.value;
    g_MateriaStatScratchMagicAttack.second = g_pCurrentCombatantStatBlock->spirit + g_MateriaStatScratchMagicAttack.second;
    if (g_MateriaStatScratchAttack.value > 0xFF) {
        g_MateriaStatScratchAttack.value = 0xFF;
    }
    if (g_MateriaStatScratchAttack.second > 0xFF) {
        g_MateriaStatScratchAttack.second = 0xFF;
    }
    if (g_MateriaStatScratchMagicAttack.value > 0xFF) {
        g_MateriaStatScratchMagicAttack.value = 0xFF;
    }
    if (g_MateriaStatScratchMagicAttack.second > 0xFF) {
        g_MateriaStatScratchMagicAttack.second = 0xFF;
    }
    if (g_MateriaStatScratchAttack.value < 0) {
        g_MateriaStatScratchAttack.value = 0;
    }
    if (g_MateriaStatScratchAttack.second < 0) {
        g_MateriaStatScratchAttack.second = 0;
    }
    if (g_MateriaStatScratchMagicAttack.value < 0) {
        g_MateriaStatScratchMagicAttack.value = 0;
    }
    if (g_MateriaStatScratchMagicAttack.second < 0) {
        g_MateriaStatScratchMagicAttack.second = 0;
    }
    g_pCurrentCombatantStatBlock->attack = g_MateriaStatScratchAttack.value;
    g_pCurrentCombatantStatBlock->defense = g_MateriaStatScratchAttack.second;
    g_pCurrentCombatantStatBlock->magicAttack = g_MateriaStatScratchMagicAttack.value;
    g_pCurrentCombatantStatBlock->magicDefense = g_MateriaStatScratchMagicAttack.second;
    Kernel_StoreEncounterModifiersToStatBlock();
    for (pos = 0; pos < 0x10; pos++) {
        Kernel_AddMegaAllLevelToMagicAndCommandCounts(g_MateriaEquippedSlotIds[pos]);
    }
    Kernel_SortMagicMenuOrder(partySlot);
    for (pos = 0; pos < 0x48; pos++) {
        if ((g_pCurrentCombatantStatBlock->abilities[pos].support & 0xE0) != 0) {
            newValue = g_pCurrentCombatantStatBlock->abilities[pos].mpCost + g_pCurrentCombatantStatBlock->abilities[pos].mpCost * ((g_pCurrentCombatantStatBlock->abilities[pos].support & 0xE0) >> 5) / 0xA + 1;
            if (newValue > 0xFF) {
                newValue = 0xFF;
            }
            g_pCurrentCombatantStatBlock->abilities[pos].mpCost = newValue;
        }
    }
    if ((g_pCurrentCombatantStatBlock->flags & PARTY_STATS_HP_MP_SWAP) != 0) {
        pos = g_pCurrentCombatantStatBlock->maxHp;
        g_pCurrentCombatantStatBlock->maxHp = g_pCurrentCombatantStatBlock->maxMp;
        g_pCurrentCombatantStatBlock->maxMp = pos;
    }
    if (g_pCurrentCombatantStatBlock->maxHp < 0xA) {
        g_pCurrentCombatantStatBlock->maxHp = 0xA;
    }
    if (g_pCurrentCombatantStatBlock->maxMp < 0xA) {
        g_pCurrentCombatantStatBlock->maxMp = 0xA;
    }
    if (g_pCurrentCombatantStatBlock->maxHp < g_pCurrentCombatantStatBlock->hp) {
        g_pCurrentCombatantStatBlock->hp = g_pCurrentCombatantStatBlock->maxHp;
        gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].hp = g_pCurrentCombatantStatBlock->hp;
    }
    if (g_pCurrentCombatantStatBlock->maxMp < g_pCurrentCombatantStatBlock->mp) {
        g_pCurrentCombatantStatBlock->mp = g_pCurrentCombatantStatBlock->maxMp;
        gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].mp = g_pCurrentCombatantStatBlock->mp;
    }
    gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].maxHp = g_pCurrentCombatantStatBlock->maxHp;
    gSavemap.chars[g_CharIdToSaveSlot[gSavemap.party[partySlot]]].maxMp = g_pCurrentCombatantStatBlock->maxMp;
}

/*
 * Calls support pairing only for low-byte selector pairs(2, 3) or(6, 7), passing three arguments
 * narrowed char. Both independent tests cannot be true together.
 */
// FUNCTION: FF7 0x005cb8d6
void __cdecl Kernel_ApplyLinkedMateriaPair(int selectorA, int selectorB, int materiaA, int materiaB, int extraA, int extraB, int effectArg)
{
    if ((selectorA & 0xFF) == 2 && (selectorB & 0xFF) == 3) {
        Kernel_ResolveSupportPairing(materiaA, materiaB, (char)extraA, (char)extraB, (char)effectArg);
    }
    if ((selectorA & 0xFF) == 6 && (selectorB & 0xFF) == 7) {
        Kernel_ResolveSupportPairing(materiaA, materiaB, (char)extraA, (char)extraB, (char)effectArg);
    }
}

/*
 * Reads both materia type table entries BEFORE rejecting 0xff IDs. Requires exactly one support
 * materia, then dispatches its effect onto the other with the arguments reordered to match.
 * Sentinel reads still occur.
 */
// FUNCTION: FF7 0x005cb94a
void __cdecl Kernel_ResolveSupportPairing(int materiaA, int materiaB, unsigned char extraA, unsigned char extraB, unsigned char effectArg)
{
    unsigned char types[2];
    unsigned char ids[2];

    ids[0] = materiaA & 0xFF;
    types[0] = g_KernelMateriaDataArray[ids[0]].typeNibbleByte & 0xF;
    ids[1] = materiaB & 0xFF;
    types[1] = g_KernelMateriaDataArray[ids[1]].typeNibbleByte & 0xF;
    if (ids[0] == 0xFF) {
        return;
    }
    if (ids[1] == 0xFF) {
        return;
    }
    if (types[0] != BATTLE_MATERIA_TYPE_SUPPORT && types[1] != BATTLE_MATERIA_TYPE_SUPPORT) {
        return;
    }
    if (types[0] == BATTLE_MATERIA_TYPE_SUPPORT && types[1] == BATTLE_MATERIA_TYPE_SUPPORT) {
        return;
    }
    if (types[0] == BATTLE_MATERIA_TYPE_SUPPORT) {
        Kernel_DispatchSupportEffect(materiaA, materiaB, extraB, effectArg);
    } else {
        Kernel_DispatchSupportEffect(materiaB, materiaA, extraA, effectArg);
    }
}

/*
 * Extracts the low-byte materia ID and arithmetic-shifted upper value from packedMateria, obtains its
 * level, and copies five descriptor bytes into support scratch storage. Dispatches the descriptor
 * type to the corresponding support helper: Quadra Magic, All, MP Turbo, ability flags, counter-
 * style effects, elemental, or status mask. Installs flag values 1, 2, 8, or 16 and counter mode
 * values 1, 4, 7, or 8 for their specific cases. Elemental and status cases receive mode; unused is
 * unused. Unsupported types still perform the initial scratch copy.
 */
// FUNCTION: FF7 0x005cba3f
void __cdecl Kernel_DispatchSupportEffect(int packedMateria, int partnerMateria, unsigned char unused, unsigned char mode)
{
    int level;
    int spareValue[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i;
    int ap;
    unsigned char id;

    id = packedMateria & 0xFF;
    ap = (packedMateria & 0xFFFFFF00) >> 8;
    level = Kernel_GetMateriaLevel((char)packedMateria, ap);
    for (i = 0; i < 5; i++) {
        g_MateriaTriggerDescriptor.levelParams[i] = (&g_KernelMateriaDataArray[id].secondaryCommandByte)[i];
    }
    switch (g_KernelMateriaDataArray[id].primaryCommandByte) {
    case BATTLE_SUPPORT_EFFECT_QUADRA_MAGIC:
        Kernel_ApplyQuadraMagicSupport((char)id, (char)level, partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_ALL:
        /* Retail caller prototype (char, char, int) -> void preserved for /Od argument promotion on `Kernel_ApplyAllSupport`. */
        ((void(__cdecl *)(char, char, int))Kernel_ApplyAllSupport)((char)id, (char)level, partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_MP_ABSORB:
        g_MateriaSupportEffectFlags = BATTLE_SUPPORT_MP_ABSORB;
        Kernel_ApplyAbilityFlagSupport(partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_HP_ABSORB:
        g_MateriaSupportEffectFlags = BATTLE_SUPPORT_HP_ABSORB;
        Kernel_ApplyAbilityFlagSupport(partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_MP_TURBO:
        Kernel_ApplyMpTurboSupport((char)level, partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_STEAL_AS_WELL:
        g_MateriaSupportEffectFlags = BATTLE_SUPPORT_STEAL_AS_WELL;
        Kernel_ApplyAbilityFlagSupport(partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_ADDED_CUT:
        g_MateriaSupportEffectFlags = BATTLE_SUPPORT_ADDED_CUT;
        Kernel_ApplyAbilityFlagSupport(partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_FINAL_ATTACK:
        g_MateriaTriggerEntryCode = BATTLE_REACTION_FINAL_ATTACK_ABILITY;
        /* Retail caller prototype (char, char, int) -> void preserved for /Od argument promotion on `Kernel_ApplyCommandAndMagicTriggerSupport`. */
        ((void(__cdecl *)(char, char, int))Kernel_ApplyCommandAndMagicTriggerSupport)((char)id, (char)level, partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_SNEAK_ATTACK:
        g_MateriaTriggerEntryCode = BATTLE_REACTION_SNEAK_ATTACK_ABILITY;
        /* Retail caller prototype (char, char, int) -> void preserved for /Od argument promotion on `Kernel_ApplyCommandAndMagicTriggerSupport`. */
        ((void(__cdecl *)(char, char, int))Kernel_ApplyCommandAndMagicTriggerSupport)((char)id, (char)level, partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_MAGIC_COUNTER:
        g_MateriaTriggerEntryCode = BATTLE_REACTION_MAGIC_COUNTER;
        /* Retail caller prototype (char, char, int) -> void preserved for /Od argument promotion on `Kernel_ApplyMagicCounterSupport`. */
        ((void(__cdecl *)(char, char, int))Kernel_ApplyMagicCounterSupport)((char)id, (char)level, partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_COUNTER:
        g_MateriaTriggerEntryCode = BATTLE_REACTION_COUNTER;
        /* Retail caller prototype (char, char, int) -> void preserved for /Od argument promotion on `Kernel_ApplyCounterSupport`. */
        ((void(__cdecl *)(char, char, int))Kernel_ApplyCounterSupport)((char)id, (char)level, partnerMateria);
        break;
    case BATTLE_SUPPORT_EFFECT_ELEMENTAL:
        Kernel_ApplyMateriaElement((char)level, partnerMateria, (char)mode);
        break;
    case BATTLE_SUPPORT_EFFECT_ADDED_EFFECT:
        Kernel_ApplyMateriaStatusMask((char)level, partnerMateria, (char)mode);
    }
}

/*
 * Rejects element ID at least 32, otherwise ORs mapped short mask into offset 0x3C for mode zero,
 * or 0x3E/0x40/0x42 by level one/two/default.
 */
// FUNCTION: FF7 0x005cbc7f
void __cdecl Kernel_ApplyMateriaElement(char level, int materiaId, char mode)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short mask;
    unsigned char index;

    index = materiaId & 0xFF;
    if (g_KernelMateriaDataArray[index].elementId >= 0x20) {
        return;
    }
    mask = g_MateriaElementBitmaskTable[g_KernelMateriaDataArray[index].elementId];
    if (mode == 0) {
        g_pCurrentCombatantStatBlock->attackElements |= mask;
    } else {
        switch (level) {
        case BATTLE_ELEMENTAL_ARMOR_HALVE:
            g_pCurrentCombatantStatBlock->halvedElements |= mask;
            break;
        case BATTLE_ELEMENTAL_ARMOR_NULLIFY:
            g_pCurrentCombatantStatBlock->nullifiedElements |= mask;
            break;
        case BATTLE_ELEMENTAL_ARMOR_ABSORB:
            g_pCurrentCombatantStatBlock->absorbedElements |= mask;
            break;
        default:
            g_pCurrentCombatantStatBlock->absorbedElements |= mask;
            break;
        }
    }
}

/*
 * Builds 24-bit mask from materia table selected by low-byte materiaId and ORs into destination
 * 0x44 when mode zero, otherwise 0x48. Arg1 unused.
 */
// FUNCTION: FF7 0x005cbd75
void __cdecl Kernel_ApplyMateriaStatusMask(char unused, int materiaId, char mode)
{
    int mask;
    unsigned char index;

    index = materiaId & 0xFF;
    mask = g_KernelMateriaDataArray[index].statusMaskLowByte + (g_KernelMateriaDataArray[index].statusMaskMidByte << 8) + (g_KernelMateriaDataArray[index].statusMaskHighByte << 0x10);
    if (mode == 0) {
        g_pCurrentCombatantStatBlock->attackStatuses |= mask;
    } else {
        g_pCurrentCombatantStatBlock->defenseStatuses |= mask;
    }
}

/*
 * Clears global g_MateriaTriggerDescriptor, extracts low-byte materia ID and remaining
 * bits shifted eight, then dispatches a command materia to
 * Kernel_AppendTriggerListEntriesForCommandMateria or a Master Command materia to
 * Kernel_AppendTriggerListEntryFixedCode with BATTLE_REACTION_RANDOM_COMMAND. Arg1 unused.
 */
// FUNCTION: FF7 0x005cbe03
void __cdecl Kernel_ApplyCounterSupport(int unused, char level, int partnerWord)
{
    unsigned char type;
    int upper;
    unsigned char id;

    id = partnerWord & 0xFF;
    upper = (partnerWord & 0xFFFFFF00) >> 8;
    type = g_KernelMateriaDataArray[id].typeNibbleByte & 0xF;
    (g_MateriaTriggerDescriptor.pendingTrigger) = 0;
    switch (type) {
    case BATTLE_MATERIA_TYPE_COMMAND:
        Kernel_AppendTriggerListEntriesForCommandMateria(level, id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_COMMAND:
        Kernel_AppendTriggerListEntryFixedCode(level, BATTLE_REACTION_RANDOM_COMMAND);
        break;
    }
}

/*
 * Clears global byte, splits packed partnerWord into low ID/rest and dispatches magic, master
 * magic, summon and master summon materia to their helpers; the master materia append
 * BATTLE_REACTION_RANDOM_MAGIC/SUMMON. Arg1 unused.
 */
// FUNCTION: FF7 0x005cbe82
void __cdecl Kernel_ApplyMagicCounterSupport(int unused, char level, int partnerWord)
{
    unsigned char type;
    int upper;
    unsigned char id;

    id = partnerWord & 0xFF;
    upper = (partnerWord & 0xFFFFFF00) >> 8;
    type = g_KernelMateriaDataArray[id].typeNibbleByte & 0xF;
    (g_MateriaTriggerDescriptor.pendingTrigger) = 0;
    switch (type) {
    case BATTLE_MATERIA_TYPE_MAGIC:
        Kernel_AppendTriggerListEntryFromLevelTable(level, id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_MAGIC:
        Kernel_AppendTriggerListEntryFixedCode(level, BATTLE_REACTION_RANDOM_MAGIC);
        break;
    case BATTLE_MATERIA_TYPE_SUMMON:
        Kernel_AppendTriggerListEntryFromDescriptor(level, id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_SUMMON:
        Kernel_AppendTriggerListEntryFixedCode(level, BATTLE_REACTION_RANDOM_SUMMON);
        break;
    }
}

/*
 * Splits materia ID/AP, clears global byte then dispatches command and Master Command materia with
 * the byte set to one, and magic and summon types with it zero, to their helpers. Arg1 unused.
 */
// FUNCTION: FF7 0x005cbf4b
void __cdecl Kernel_ApplyCommandAndMagicTriggerSupport(int unused, char level, int partnerWord)
{
    unsigned char type;
    int upper;
    unsigned char id;

    id = partnerWord & 0xFF;
    upper = (partnerWord & 0xFFFFFF00) >> 8;
    type = *((unsigned char *)&g_KernelMateriaDataArray[id].typeNibbleByte) & 0xF;
    (g_MateriaTriggerDescriptor.pendingTrigger) = 0;
    switch (type) {
    case BATTLE_MATERIA_TYPE_COMMAND:
        (g_MateriaTriggerDescriptor.pendingTrigger) = 1;
        Kernel_AppendTriggerListEntriesForCommandMateria(level, id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_COMMAND:
        (g_MateriaTriggerDescriptor.pendingTrigger) = 1;
        Kernel_AppendTriggerListEntryFixedCode(level, BATTLE_REACTION_RANDOM_COMMAND);
        break;
    case BATTLE_MATERIA_TYPE_MAGIC:
        Kernel_AppendTriggerListEntryFromLevelTable(level, id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_MAGIC:
        Kernel_AppendTriggerListEntryFixedCode(level, BATTLE_REACTION_RANDOM_MAGIC);
        break;
    case BATTLE_MATERIA_TYPE_SUMMON:
        Kernel_AppendTriggerListEntryFromDescriptor(level, id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_SUMMON:
        Kernel_AppendTriggerListEntryFixedCode(level, BATTLE_REACTION_RANDOM_SUMMON);
        break;
    }
}

/*
 * Appends three narrowed bytes to global record table when count is NOT EQUAL eight, then
 * increments count. Values above eight still write; char-argument int read retains ABI caveat.
 */
// FUNCTION: FF7 0x005cc05f
void __cdecl Kernel_AppendTriggerListEntryFixedCode(char level, int code)
{
    if (g_MateriaTriggerCount != 8) {
        g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].typeId = g_MateriaTriggerEntryCode + (g_MateriaTriggerDescriptor.pendingTrigger);
        g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].attackIndex = code;
        g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].chance =
            (&g_MateriaTriggerDescriptor.levelParamBase)[(*(int *)&level & 0xFF)];
        g_MateriaTriggerCount++;
    }
}

/*
 * Computes materia level for side effects, then selected first/second command codes append three-
 * byte association entries only when command lookup succeeds and count is not exactly eight.
 * Stores summed global index, code and indexed level association; count above eight still appends.
 * Char-address int read remains C ABI caveat.
 */
// FUNCTION: FF7 0x005cc0d4
void __cdecl Kernel_AppendTriggerListEntriesForCommandMateria(char level, unsigned char materiaId, int ap)
{
    int idx;
    int baseMateriaLevel;
    int r;
    baseMateriaLevel = Kernel_GetMateriaLevel(materiaId, ap);
    switch ((int)g_KernelMateriaDataArray[materiaId].primaryCommandByte) {
    case BATTLE_COMMAND_STEAL:
    case BATTLE_COMMAND_SENSE:
    case BATTLE_COMMAND_THROW:
    case BATTLE_COMMAND_MORPH:
    case BATTLE_COMMAND_DEATHBLOW:
    case BATTLE_COMMAND_MANIPULATE:
    case BATTLE_COMMAND_MIME:
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindCommandListSlot`. */
        idx = ((int(__cdecl *)(char))Kernel_FindCommandListSlot)(*(char *)&g_KernelMateriaDataArray[materiaId].primaryCommandByte);
        if (idx != -1 && g_MateriaTriggerCount != 8) {
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].typeId = g_MateriaTriggerEntryCode + (g_MateriaTriggerDescriptor.pendingTrigger);
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].attackIndex = *(char *)&g_KernelMateriaDataArray[materiaId].primaryCommandByte;
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].chance = (&g_MateriaTriggerDescriptor.levelParamBase)[(*(int *)&level & 255)];
            g_MateriaTriggerCount++;
        }
        break;
    }
    switch (*(unsigned char *)&g_KernelMateriaDataArray[materiaId].secondaryCommandByte) {
    case BATTLE_COMMAND_COIN:
    case BATTLE_COMMAND_MUG:
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindCommandListSlot`. */
        idx = ((int(__cdecl *)(char))Kernel_FindCommandListSlot)(g_KernelMateriaDataArray[materiaId].secondaryCommandByte);
        if (idx != -1 && g_MateriaTriggerCount != 8) {
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].typeId = g_MateriaTriggerEntryCode + (g_MateriaTriggerDescriptor.pendingTrigger);
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].attackIndex = g_KernelMateriaDataArray[materiaId].secondaryCommandByte;
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].chance = (&g_MateriaTriggerDescriptor.levelParamBase)[(*(int *)&level & 255)];
            g_MateriaTriggerCount++;
        }
        break;
    }
}

/*
 * Walks levels down to first non 0xff byte and appends record only when count not equal eight, then
 * stops even if full. Count above eight still writes.
 */
// FUNCTION: FF7 0x005cc2a8
void __cdecl Kernel_AppendTriggerListEntryFromLevelTable(unsigned char level, unsigned char materiaId, int ap)
{
    int materiaLevel;
    int i;

    materiaLevel = Kernel_GetMateriaLevel(materiaId, ap);
    for (i = materiaLevel; i > 0; i--) {
        if ((&g_KernelMateriaDataArray[materiaId].typeNibbleByte)[i] != 0xFF) {
            if (g_MateriaTriggerCount != 8) {
                g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].typeId = g_MateriaTriggerEntryCode + (g_MateriaTriggerDescriptor.pendingTrigger);
                g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].attackIndex = (char)(&g_KernelMateriaDataArray[materiaId].typeNibbleByte)[i];
                g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].chance = (&g_MateriaTriggerDescriptor.levelParamBase)[level];
                g_MateriaTriggerCount = g_MateriaTriggerCount + 1;
            }
            break;
        }
    }
}

/*
 * Computes unused materia level then appends three narrowed table/global bytes whenever count not
 * equal eight and increments count. Count above eight still writes.
 */
// FUNCTION: FF7 0x005cc38e
void __cdecl Kernel_AppendTriggerListEntryFromDescriptor(unsigned char level, unsigned char materiaId, int ap)
{
    int materiaLevel;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    materiaLevel = Kernel_GetMateriaLevel((char)materiaId, ap);
    if (g_MateriaTriggerCount != 8) {
        g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].typeId = g_MateriaTriggerEntryCode + (g_MateriaTriggerDescriptor.pendingTrigger);
        g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].attackIndex = *(char *)&g_KernelMateriaDataArray[materiaId].primaryCommandByte;
        g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].chance = (&g_MateriaTriggerDescriptor.levelParamBase)[level];
        g_MateriaTriggerCount++;
    }
}

/*
 * Splits packed materia into low ID/rest, dispatches magic, master magic, summon and master summon
 * materia to the matching MP-turbo helpers. Other types no call.
 */
// FUNCTION: FF7 0x005cc429
void __cdecl Kernel_ApplyMpTurboSupport(char level, int packedMateria)
{
    unsigned char type;
    int upper;
    unsigned char id;

    id = packedMateria & 0xFF;
    upper = (packedMateria & 0xFFFFFF00) >> 8;
    type = g_KernelMateriaDataArray[id].typeNibbleByte & 0xF;
    switch (type) {
    case BATTLE_MATERIA_TYPE_MAGIC:
        Kernel_AddMpTurboToMagicEntries((char)level, (char)id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_MAGIC:
        Kernel_AddMpTurboToAllMagicEntries((char)level);
        break;
    case BATTLE_MATERIA_TYPE_SUMMON:
        Kernel_AddMpTurboToSummonEntries((char)level, (char)id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_SUMMON:
        Kernel_AddMpTurboToAllSummonEntries((char)level);
    }
}

/*
 * Walks materia levels downward, adds low-byte amount to high-three-bit field of indexed summon
 * entry, clamps five and preserves low five bits. Char-argument int reads retain ABI caveat.
 */
// FUNCTION: FF7 0x005cc4df
void __cdecl Kernel_AddMpTurboToSummonEntries(char amount, char materiaId, int ap)
{
    int materiaLevel;
    int summonIdx;
    int i;
    int newValue;

    materiaLevel = Kernel_GetMateriaLevel((char)materiaId, ap);
    for (i = materiaLevel; i > 0; i--) {
        summonIdx = (&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].typeNibbleByte)[i];
        newValue = ((g_pCurrentCombatantStatBlock->abilities[summonIdx].support & 0xE0) >> 5) + (*(int *)&amount & 0xFF);
        if (newValue > 5) {
            newValue = 5;
        }
        g_pCurrentCombatantStatBlock->abilities[summonIdx].support = g_pCurrentCombatantStatBlock->abilities[summonIdx].support & 0x1F | (newValue & 7) << 5;
    }
}

/*
 * For sixteen records adds low-byte amount to high-three-bit field, upper-clamps five and replaces
 * high bits preserving low five. Int read through char argument address retains ABI portability
 * caveat.
 */
// FUNCTION: FF7 0x005cc59b
void __cdecl Kernel_AddMpTurboToAllSummonEntries(char amount)
{
    int idx;
    int sum;

    for (idx = 0; idx < 0x10; idx++) {
        sum = ((g_pCurrentCombatantStatBlock->summons[idx].support & 0xE0) >> 5) + (*(int *)&amount & 0xFF);
        if (sum > 5) {
            sum = 5;
        }
        g_pCurrentCombatantStatBlock->summons[idx].support = g_pCurrentCombatantStatBlock->summons[idx].support & 0x1F | (sum & 7) << 5;
    }
}

/*
 * Visits 56 magic IDs, resolves slots and adds low-byte amount to high-three-bit field,
 * upper-clamps five and preserves low-five bits.
 */
// FUNCTION: FF7 0x005cc623
void __cdecl Kernel_AddMpTurboToAllMagicEntries(char amount)
{
    struct {
        int turboLevel;
        int magicId;
        int slotIndex;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    for (local.magicId = 0; local.magicId < 0x38; local.magicId++) {
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindMagicListSlot`. */
        local.slotIndex = ((int(__cdecl *)(char))Kernel_FindMagicListSlot)((char)local.magicId);
        if (local.slotIndex != -1) {
            local.turboLevel = ((g_MateriaMagicListRecords[local.slotIndex].support & 0xE0) >> 5) + (*(int *)&amount & 0xFF);
            if (local.turboLevel > 5) {
                local.turboLevel = 5;
            }
            g_MateriaMagicListRecords[local.slotIndex].support =
                g_MateriaMagicListRecords[local.slotIndex].support & 0x1F | (local.turboLevel & 7) << 5;
        }
    }
}

/*
 * Walks materia levels downward, resolves magic slot, adds low-byte amount to high-three-bit field
 * and clamps five preserving low bits. Missing slots skipped; char-address int read ABI caveat.
 */
// FUNCTION: FF7 0x005cc6b5
void __cdecl Kernel_AddMpTurboToMagicEntries(char amount, char materiaId, int ap)
{
    struct {
        int r;
        int i;
        int slot;
        int level;
    } local;

    local.level = Kernel_GetMateriaLevel(materiaId, ap);
    for (local.i = local.level; local.i > 0; local.i--) {
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindMagicListSlot`. */
        local.slot = ((int(__cdecl *)(char))Kernel_FindMagicListSlot)(
            (char)(&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].typeNibbleByte)[local.i]);
        if (local.slot != -1) {
            local.r = ((g_MateriaMagicListRecords[local.slot].support & 0xE0) >> 5) + (*(int *)&amount & 0xFF);
            if (local.r > 5) {
                local.r = 5;
            }
            g_MateriaMagicListRecords[local.slot].support =
                g_MateriaMagicListRecords[local.slot].support & 0x1F | (local.r & 7) << 5;
        }
    }
}

/*
 * Splits packed materia ID/AP and dispatches command, magic and summon materia to the support-flag
 * helper for their entries, and their master materia to the helper for all entries. Other types no call.
 */
// FUNCTION: FF7 0x005cc773
void __cdecl Kernel_ApplyAbilityFlagSupport(int packedMateria)
{
    unsigned char type;
    int upper;
    unsigned char id;

    id = packedMateria & 0xFF;
    upper = (packedMateria & 0xFFFFFF00) >> 8;
    type = g_KernelMateriaDataArray[id].typeNibbleByte & 0xF;
    switch (type) {
    case BATTLE_MATERIA_TYPE_COMMAND:
        Kernel_OrSupportFlagIntoCommandEntry((char)id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_COMMAND:
        Kernel_OrSupportFlagIntoAllCommandEntries();
        break;
    case BATTLE_MATERIA_TYPE_MAGIC:
        /* Retail caller prototype (char, int) -> void preserved for /Od argument promotion on `Kernel_OrSupportFlagIntoMagicEntries`. */
        ((void(__cdecl *)(char, int))Kernel_OrSupportFlagIntoMagicEntries)((char)id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_MAGIC:
        Kernel_OrSupportFlagIntoAllMagicEntries();
        break;
    case BATTLE_MATERIA_TYPE_SUMMON:
        Kernel_OrSupportFlagIntoSummonEntry((char)id, upper);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_SUMMON:
        Kernel_OrSupportFlagIntoAllSummonEntries();
    }
}

/*
 * Gets materia level and walks levels downward, reads per-level byte index and ORs global support
 * byte into indexed entry offset 0x10F. Reconstructed int read through char argument address needs
 * separate ABI representation evidence.
 */
// FUNCTION: FF7 0x005cc838
void __cdecl Kernel_OrSupportFlagIntoSummonEntry(char materiaId, int ap)
{
    int materiaLevel;
    int summonIdx;
    int i;

    materiaLevel = Kernel_GetMateriaLevel((char)materiaId, ap);
    for (i = materiaLevel; i > 0; i--) {
        summonIdx = (&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].typeNibbleByte)[i];
        (*(char *)&g_pCurrentCombatantStatBlock->abilities[summonIdx].support) |= g_MateriaSupportEffectFlags;
    }
}

/*
 * ORs the byte g_MateriaSupportEffectFlags into each of sixteen bytes at g_pCurrentCombatantStatBlock + 0x2CF, stride 8 - one
 * flag byte per entry of an eight-byte-per-slot table.
 */
// FUNCTION: FF7 0x005cc8af
void __cdecl Kernel_OrSupportFlagIntoAllSummonEntries(void)
{
    int i;

    for (i = 0; i < 0x10; i++) {
        (*(char *)&g_pCurrentCombatantStatBlock->summons[i].support) |= g_MateriaSupportEffectFlags;
    }
}

/*
 * Resolves primary command ID and ORs support flag into found slot. For second table byte 7/17
 * similarly resolves/ORs second command. Level ap unused; char-address int read ABI caveat.
 * Direct byte switch adds the original fourth stack slot; the three explicit DWORD slots total
 * twelve bytes.
 */
// FUNCTION: FF7 0x005cc8f7
void __cdecl Kernel_OrSupportFlagIntoCommandEntry(char materiaId, int ap)
{
    struct {
        int id;
        int unused;
        int slot;
    } local;

    local.id = g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte;
    /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindCommandListSlot`. */
    local.slot = ((int(__cdecl *)(char))Kernel_FindCommandListSlot)(
        *(char *)&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte);
    if (local.slot != -1) {
        g_MateriaCommandListEntries[local.slot].supportFlags |= g_MateriaSupportEffectFlags;
    }
    switch (*(unsigned char *)&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].secondaryCommandByte) {
    case BATTLE_COMMAND_COIN:
    case BATTLE_COMMAND_MUG:
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindCommandListSlot`. */
        local.slot = ((int(__cdecl *)(char))Kernel_FindCommandListSlot)(
            g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].secondaryCommandByte);
        if (local.slot != -1) {
            g_MateriaCommandListEntries[local.slot].supportFlags |= g_MateriaSupportEffectFlags;
        }
        break;
    }
}

/*
 * For sixteen command IDs resolves slot and ORs global support flag into found slot byte.
 */
// FUNCTION: FF7 0x005cc9bd
void __cdecl Kernel_OrSupportFlagIntoAllCommandEntries(void)
{
    int slot;
    int index;
    int cmd;

    for (index = 0; index < 0x10; index++) {
        cmd = g_MateriaCommandListEntries[index].commandCode;
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindCommandListSlot`. */
        slot = ((int(__cdecl *)(char))Kernel_FindCommandListSlot)(
            *(char *)&g_MateriaCommandListEntries[index].commandCode);
        if (slot != -1) {
            g_MateriaCommandListEntries[slot].supportFlags |= g_MateriaSupportEffectFlags;
        }
    }
}

/*
 * For identifiers zero through 55, finds each magic-list slot and ORs g_MateriaSupportEffectFlags into its byte
 * at g_MateriaMagicListRecords[0] + 4+slot*5 when found.
 */
// FUNCTION: FF7 0x005cca2e
void __cdecl Kernel_OrSupportFlagIntoAllMagicEntries(void)
{
    int slot;
    int magicId;

    for (magicId = 0; magicId < 0x38; magicId++) {
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindMagicListSlot`. */
        slot = ((int(__cdecl *)(char))Kernel_FindMagicListSlot)((char)magicId);
        if (slot != -1) {
            g_MateriaMagicListRecords[slot].support |= g_MateriaSupportEffectFlags;
        }
    }
}

/*
 * Walks materia levels downward, resolves magic-list slot and ORs global support flag into found
 * stride-five byte. Missing slots skipped.
 */
// FUNCTION: FF7 0x005cca85
void __cdecl Kernel_OrSupportFlagIntoMagicEntries(int materiaId, int ap)
{
    int materiaLevel;
    int entryIndex;
    int i;

    materiaLevel = Kernel_GetMateriaLevel((char)materiaId, ap);
    for (i = materiaLevel; i > 0; i--) {
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindMagicListSlot`. */
        entryIndex = ((int(__cdecl *)(char))Kernel_FindMagicListSlot)(
            (char)(&g_KernelMateriaDataArray[(materiaId & 0xFF)].typeNibbleByte)[i]);
        if (entryIndex != -1) {
            g_MateriaMagicListRecords[entryIndex].support |= g_MateriaSupportEffectFlags;
        }
    }
}

/*
 * Splits packed and dispatches magic, master magic, summon and master summon materia to the four
 * Quadra Magic helpers with amount; the `unused` parameter is ignored. Other types no call.
 */
// FUNCTION: FF7 0x005ccb01
void __cdecl Kernel_ApplyQuadraMagicSupport(char unused, char amount, int packed)
{
    unsigned char category;
    int upperValue;
    unsigned char materia;

    materia = packed & 0xFF;
    upperValue = (packed & 0xFFFFFF00) >> 8;
    category = g_KernelMateriaDataArray[materia].typeNibbleByte & 0xF;
    switch (category) {
    case BATTLE_MATERIA_TYPE_MAGIC:
        Kernel_AddQuadraMagicToMagicEntries((char)amount, (char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_MAGIC:
        Kernel_AddQuadraMagicToAllMagicEntries((char)amount);
        break;
    case BATTLE_MATERIA_TYPE_SUMMON:
        /* Retail caller passes 3 arg(s) to 2-parameter `Kernel_AddQuadraMagicToSummonEntry` (__cdecl stack cleanup matches retail). */
        ((void(__cdecl *)(char, char, int))Kernel_AddQuadraMagicToSummonEntry)((char)amount, (char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_SUMMON:
        Kernel_AddQuadraMagicToAllSummonEntries((char)amount);
    }
}

/*
 * Walks materia levels downward; found magic slots increment byte 0x52 and add amount to byte 0x53
 * in stride-five table. Char-argument int read retains ABI caveat.
 */
// FUNCTION: FF7 0x005ccbb7
void __cdecl Kernel_AddQuadraMagicToMagicEntries(char amount, char materiaId, int packedValue)
{
    int slot;
    int cnt;
    int i;

    cnt = Kernel_GetMateriaLevel(materiaId, packedValue);
    for (i = cnt; i > 0; i--) {
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindMagicListSlot`. */
        slot = ((int(__cdecl *)(char))Kernel_FindMagicListSlot)(
            (char)(&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].typeNibbleByte)[i]);
        if (slot != -1) {
            (g_MateriaMagicListRecords[slot].quadraLinks)++;
            g_MateriaMagicListRecords[slot].quadraCount += amount;
        }
    }
}

/*
 * For 56 stride-five records increments byte at g_MateriaMagicListRecords[0] + 2 and adds amount to adjacent byte.
 * Narrowing wraps; reserved p/q locals unused.
 */
// FUNCTION: FF7 0x005ccc4b
void __cdecl Kernel_AddQuadraMagicToAllMagicEntries(char amount)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i;

    for (i = 0; i < 0x38; i++) {
        (g_MateriaMagicListRecords[i].quadraLinks)++;
        g_MateriaMagicListRecords[i].quadraCount += amount;
    }
}

/*
 * Reads the byte at &g_KernelMateriaDataArray[0].primaryCommandByte from the 0x14-byte record
 * selected by the low byte of materiaId, subtracts 0x38 to get an index, then increments the byte
 * at that index in &g_MateriaSummonUseCounts and adds amount to the byte at the same index in
 * &g_MateriaQuadraMagicCounts.
 */
// FUNCTION: FF7 0x005ccca5
void __cdecl Kernel_AddQuadraMagicToSummonEntry(int amount, int materiaId)
{
    int summonIndex;

    summonIndex = g_KernelMateriaDataArray[(materiaId & 0xFF)].primaryCommandByte - 0x38;
    g_MateriaSummonUseCounts[summonIndex]++;
    g_MateriaQuadraMagicCounts[summonIndex] += amount;
}

/*
 * Increments each of sixteen bytes at g_MateriaSummonUseCounts and adds amount to corresponding byte at
 * g_MateriaQuadraMagicCounts. Byte narrowing wraps; local p/q slots unused.
 */
// FUNCTION: FF7 0x005cccef
void __cdecl Kernel_AddQuadraMagicToAllSummonEntries(char amount)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i;

    for (i = 0; i < 0x10; i++) {
        g_MateriaSummonUseCounts[i]++;
        g_MateriaQuadraMagicCounts[i] += amount;
    }
}

/*
 * Splits packed materia into padded local low-byte ID and AP, dispatches a magic materia to the
 * per-level helper and a master magic materia to the all-magic helper using amount. Arg1 unused; padded local int read low-byte mask
 * retains alias portability caveat.
 */
// FUNCTION: FF7 0x005ccd3d
void __cdecl Kernel_ApplyAllSupport(int unused, int amount, int packed)
{
    struct {
        unsigned char materia;
        char pad1[3];
        unsigned char kind;
        char pad2[3];
        int ap;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.materia = packed & 0xFF;
    local.ap = (packed & 0xFFFFFF00) >> 8;
    local.kind = g_KernelMateriaDataArray[(*(int *)&local.materia & 0xFF)].typeNibbleByte &
                 0xF;
    switch (local.kind) {
    case BATTLE_MATERIA_TYPE_MAGIC:
        /* Retail caller prototype (char, char, int) -> void preserved for /Od argument promotion on `Kernel_AddAllSupportLevelToMagicEntries`. */
        ((void(__cdecl *)(char, char, int))Kernel_AddAllSupportLevelToMagicEntries)(
            (char)amount, local.materia, local.ap);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_MAGIC:
        Kernel_AddAllSupportToEveryMagicEntry((char)amount);
        break;
    }
}

/*
 * For identifiers zero through 55, finds each magic-list slot and adds amount to its byte at
 * g_MateriaMagicListRecords[0] + 1+slot*5 when found.
 */
// FUNCTION: FF7 0x005ccdae
void __cdecl Kernel_AddAllSupportToEveryMagicEntry(char amount)
{
    int slot;
    int magicId;

    for (magicId = 0; magicId < 0x38; magicId++) {
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindMagicListSlot`. */
        slot = ((int(__cdecl *)(char))Kernel_FindMagicListSlot)((char)magicId);
        if (slot != -1) {
            g_MateriaMagicListRecords[slot].allCount += amount;
        }
    }
}

/*
 * Walks materia levels downward, resolves each indexed magic-list slot and adds amount to
 * stride-five byte only for found slots.
 */
// FUNCTION: FF7 0x005cce02
void __cdecl Kernel_AddAllSupportLevelToMagicEntries(char amount, int materiaId, int packedValue)
{
    int slot;
    int cnt;
    int i;

    cnt = Kernel_GetMateriaLevel((char)materiaId, packedValue);
    for (i = cnt; i > 0; i--) {
        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Kernel_FindMagicListSlot`. */
        slot = ((int(__cdecl *)(char))Kernel_FindMagicListSlot)(
            (char)(&g_KernelMateriaDataArray[(materiaId & 0xFF)].typeNibbleByte)[i]);
        if (slot != -1) {
            g_MateriaMagicListRecords[slot].allCount += amount;
        }
    }
}

/*
 * Searches sixteen stride-three records at g_MateriaCommandListEntries for a leading byte equal to the low byte
 * of commandCode; returns the first index or minus one.
 */
// FUNCTION: FF7 0x005cce7b
int __cdecl Kernel_FindCommandListSlot(int commandCode)
{
    int i;

    for (i = 0; i < 0x10; i++) {
        if (g_MateriaCommandListEntries[i].commandCode == (commandCode & 0xff)) {
            return i;
        }
    }
    return -1;
}

/*
 * Searches 56 stride-five records at g_MateriaMagicListRecords for a leading byte equal to the low byte of
 * magicId; returns the first index or minus one.
 */
// FUNCTION: FF7 0x005ccebf
int __cdecl Kernel_FindMagicListSlot(int magicId)
{
    int i;

    for (i = 0; i < 0x38; i++) {
        if (*(unsigned char *)&g_MateriaMagicListRecords[i].magicId == (magicId & 0xFF)) {
            return i;
        }
    }
    return -1;
}

/*
 * For a Mega All materia, derives level, adds it to byte counters of all nonempty 56
 * entries and selected codes among sixteen command entries. Unless first command code 0x1a
 * installs command 0x18. Other types unchanged; packed upper value is signed shifted and byte
 * additions narrow.
 */
// FUNCTION: FF7 0x005ccf03
void __cdecl Kernel_AddMegaAllLevelToMagicAndCommandCounts(int packed)
{
    int level;
    int packedVal;
    unsigned char materiaId;
    int typeCode;
    int i;

    materiaId = packed & 0xFF;
    packedVal = (packed & 0xFFFFFF00) >> 8;
    typeCode = g_KernelMateriaDataArray[materiaId].typeNibbleByte & 0xF;
    if (typeCode == BATTLE_MATERIA_TYPE_MEGA_ALL) {
        level = Kernel_GetMateriaLevel((char)packed, packedVal);
        for (i = 0; i < 0x38; i++) {
            if (g_pCurrentCombatantStatBlock->abilities[i].attackId != 0xFF) {
                (*(char *)&g_pCurrentCombatantStatBlock->abilities[i].allCount) += level;
            }
        }
        for (i = 0; i < 0x10; i++) {
            if (g_pCurrentCombatantStatBlock->commands[i].commandId != 0xFF) {
                switch (g_pCurrentCombatantStatBlock->commands[i].commandId) {
                case BATTLE_COMMAND_STEAL:
                case BATTLE_COMMAND_SENSE:
                case BATTLE_COMMAND_MORPH:
                case BATTLE_COMMAND_DEATHBLOW:
                case BATTLE_COMMAND_MANIPULATE:
                case BATTLE_COMMAND_MUG:
                    (*(char *)&g_pCurrentCombatantStatBlock->commands[i].allCount) += level;
                }
            }
        }
        if (g_pCurrentCombatantStatBlock->commands[0].commandId != 0x1A) {
            Kernel_SetBattleMenuCommand(0x18, 0);
        }
    }
}

/*
 * Description mode clears eight output shorts even for sentinel materia ID. Nonsentinel packed ID
 * applies stat bonus then dispatches the materia type (low nibble) to its handler with the subtype
 * (high nibble) and unsigned upper 24 value. Unknown type only stat bonus.
 */
// FUNCTION: FF7 0x005cd078
void __cdecl Kernel_ApplyMateriaToActor(unsigned int packed)
{
    unsigned char category;
    int upperValue;
    unsigned char subtype;
    unsigned char materia;
    BattleMateriaStatPlusAbility *destPointer;

    if (g_MateriaDescribeModeFlag != 0) {
        destPointer = g_MateriaCurrentAbilityMask;
        for (category = 0; category < 8; category++) {
            destPointer->bonuses[category] = 0;
        }
    }
    materia = packed & 0xFF;
    upperValue = (packed & 0xFFFFFF00) >> 8;
    if (materia == 0xFF) {
        return;
    }
    /* Retail caller prototype (char) -> void preserved for /Od argument promotion on `Kernel_ApplyMateriaStatBonus`. */
    ((void(__cdecl *)(char))Kernel_ApplyMateriaStatBonus)((char)materia);
    subtype = (g_KernelMateriaDataArray[materia].typeNibbleByte & 0xF0) >> 4;
    category = g_KernelMateriaDataArray[materia].typeNibbleByte & 0xF;
    switch (category) {
    case BATTLE_MATERIA_TYPE_INDEPENDENT:
        /* Retail caller prototype (char, char, int) -> void preserved for /Od argument promotion on `Kernel_ApplyIndependentMateria`. */
        ((void(__cdecl *)(char, char, int))Kernel_ApplyIndependentMateria)((char)subtype, (char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_ENCOUNTER:
        Kernel_ApplyEncounterMateria((char)subtype, (char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_EXTRA_CUT:
        Kernel_ApplyExtraCutMateria((char)subtype, (char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_DOUBLE_CAST:
        Kernel_ApplyDoubleCastMateria((char)subtype, (char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_MEGA_ALL:
        Kernel_ApplyMegaAllMateria((char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_SUPPORT:
        Kernel_ApplySupportMateria((char)subtype, (char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_COMMAND:
        Kernel_ApplyCommandMateria((char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_ENEMY_SKILL:
        Kernel_ApplyEnemySkillMateria((char)subtype, (char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_COMMAND:
        Kernel_ApplyMasterCommandMateria();
        break;
    case BATTLE_MATERIA_TYPE_MAGIC:
        Kernel_ApplyMagicMateria((char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_MAGIC:
        Kernel_ApplyMasterMagicMateria();
        break;
    case BATTLE_MATERIA_TYPE_SUMMON:
        Kernel_ApplySummonMateria((char)materia, upperValue);
        break;
    case BATTLE_MATERIA_TYPE_MASTER_SUMMON:
        Kernel_ApplyMasterSummonMateria();
    }
}

/*
 * Looks up the stat-bonus table selector for the low-byte materia ID. Selector 255 makes no
 * changes. In accumulation mode adds the eight signed SHORT table fields to six global stat
 * bonuses and two additional global bonuses. In the other mode overwrites eight SHORT fields at
 * offsets ten through 24 of the global destination record, including an explicit redundant
 * self-assignment of its first field. No level-dependent scaling or clamping occurs here.
 */
// FUNCTION: FF7 0x005cd278
void __cdecl Kernel_ApplyMateriaStatBonus(int materiaId)
{
    BattleMateriaStatPlusAbility *bonusSet;

    if (g_MateriaDescribeModeFlag == 0) {
        if (g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte != 0xFF) {
            g_MateriaAdditiveStatBonuses[0] += g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][0];
            g_MateriaAdditiveStatBonuses[1] += g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][1];
            g_MateriaAdditiveStatBonuses[2] += g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][2];
            g_MateriaAdditiveStatBonuses[3] += g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][3];
            g_MateriaAdditiveStatBonuses[4] += g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][4];
            g_MateriaAdditiveStatBonuses[5] += g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][5];
            g_MateriaStatPercentModifiers.maxHpPercent += g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][6];
            g_MateriaStatPercentModifiers.maxMpPercent += g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][7];
        }
    } else if (g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte != 0xFF) {
        bonusSet = g_MateriaCurrentAbilityMask;
        bonusSet->strengthBonus = g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][0];
        bonusSet->strengthBonus = bonusSet->strengthBonus;
        bonusSet->vitalityBonus = g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][1];
        bonusSet->magicBonus = g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][2];
        bonusSet->spiritBonus = g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][3];
        bonusSet->dexterityBonus = g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][4];
        bonusSet->luckBonus = g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][5];
        bonusSet->maxHpPercentBonus = g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][6];
        bonusSet->maxMpPercentBonus = g_MateriaStatBonusTable[g_KernelMateriaDataArray[(materiaId & 0xFF)].statSelectorByte][7];
    }
}

/*
 * Gets a materia level from materiaId and packedValue; only for the subtype
 * BATTLE_MATERIA_SUBTYPE_EXTRA_CUT_COMMAND, calls Kernel_GrantExtraCutCommand with that level and materiaId.
 */
// FUNCTION: FF7 0x005cd567
void __cdecl Kernel_ApplyExtraCutMateria(char subtype, char materiaId, int packedValue)
{
    int level;

    level = Kernel_GetMateriaLevel((char)materiaId, packedValue);
    switch ((unsigned char)subtype) {
    case BATTLE_MATERIA_SUBTYPE_EXTRA_CUT_COMMAND:
        Kernel_GrantExtraCutCommand((char)level, (char)materiaId);
    }
}

/*
 * Normal mode scans level-minus-one down toward zero for command byte then stores it. Descriptor
 * mode writes five entries and marks selected level position, sets the extra-cut panel kind. Level zero produces
 * table offset minus one in normal path.
 */
// FUNCTION: FF7 0x005cd5a2
void __cdecl Kernel_GrantExtraCutCommand(unsigned char level, unsigned char materiaId)
{
    int idx;
    BattleMateriaStatPlusAbility *record;

    if (g_MateriaDescribeModeFlag == 0) {
        for (idx = level - 1; idx > 0; idx--) {
            if (g_KernelMateriaDataArray[materiaId].attributes[idx] != 0xFF) {
                break;
            }
        }
        g_MateriaCommandListEntries[0].commandCode = (char)g_KernelMateriaDataArray[materiaId].attributes[idx];
    } else {
        record = g_MateriaCurrentAbilityMask;
        for (idx = 0; idx < 5; idx++) {
            record->entries[idx].id = (char)g_KernelMateriaDataArray[materiaId].attributes[idx];
            record->entries[idx].marked = 0;
        }
        if (record->masteredCount == level) {
            record->entries[level - 2].marked = 1;
        } else {
            record->entries[level - 1].marked = 1;
        }
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_EXTRA_CUT;
    }
}

/*
 * Derives materia level. Normal mode: W-Magic and W-Summon set the magic or summon count to two and
 * find or add the Magic or Summon command, then set its level; W-Item writes itself into the Item
 * command's slot without a missing-slot check. Description mode writes code/level and the
 * double-cast panel kind. Arg1 unused; char-address int read and invalid slot remain
 * source caveats.
 */
// FUNCTION: FF7 0x005cd6a9
void __cdecl Kernel_ApplyDoubleCastMateria(char unused, char materiaId, int packedValue)
{
    struct {
        BattleMateriaStatPlusAbility *ptr;
        int slot;
        int level;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.level = Kernel_GetMateriaLevel(materiaId, packedValue);
    if (g_MateriaDescribeModeFlag == 0) {
        switch (*(char *)&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte) {
        case BATTLE_COMMAND_W_MAGIC:
            g_MateriaMagicListCount = 2;
            local.slot = Kernel_FindCommandListSlot(BATTLE_COMMAND_MAGIC);
            if (local.slot == -1) {
                g_MateriaCommandListEntries[Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_MAGIC)].level = (char)local.level;
            } else {
                g_MateriaCommandListEntries[local.slot].commandCode = BATTLE_COMMAND_MAGIC;
                g_MateriaCommandListEntries[local.slot].level = (char)local.level;
            }
            break;
        case BATTLE_COMMAND_W_SUMMON:
            g_MateriaSummonCount = 2;
            local.slot = Kernel_FindCommandListSlot(BATTLE_COMMAND_SUMMON);
            if (local.slot == -1) {
                g_MateriaCommandListEntries[Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_SUMMON)].level = (char)local.level;
            } else {
                g_MateriaCommandListEntries[local.slot].commandCode = BATTLE_COMMAND_SUMMON;
                g_MateriaCommandListEntries[local.slot].level = (char)local.level;
            }
            break;
        case BATTLE_COMMAND_W_ITEM:
            local.slot = Kernel_FindCommandListSlot(BATTLE_COMMAND_ITEM);
            g_MateriaCommandListEntries[local.slot].commandCode = *(char *)&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte;
            g_MateriaCommandListEntries[local.slot].level = (char)local.level;
            break;
        }
    } else {
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_DOUBLE_CAST;
        local.ptr = g_MateriaCurrentAbilityMask;
        local.ptr->primaryCommandByte = *(char *)&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte;
        local.ptr->value = (char)local.level;
    }
}

/*
 * Dispatches selector two to support materia percent-description helper and three to count-
 * description helper, passing remaining arguments. Other selectors perform no call.
 */
// FUNCTION: FF7 0x005cd815
void __cdecl Kernel_ApplySupportMateria(char selector, char materiaId, int packedValue)
{
    switch (selector) {
    case BATTLE_MATERIA_SUBTYPE_SUPPORT_PERCENT:
        /* Retail caller prototype (char, int) -> void preserved for /Od argument promotion on `Kernel_DescribeSupportMateriaPercent`. */
        ((void(__cdecl *)(char, int))Kernel_DescribeSupportMateriaPercent)(materiaId, packedValue);
        break;
    case BATTLE_MATERIA_SUBTYPE_SUPPORT_COUNT:
        /* Retail caller prototype (char, int) -> void preserved for /Od argument promotion on `Kernel_DescribeSupportMateriaCount`. */
        ((void(__cdecl *)(char, int))Kernel_DescribeSupportMateriaCount)(materiaId, packedValue);
        break;
    }
}

/*
 * In descriptor mode writes table selector and narrowed level, then All and Quadra Magic choose
 * panel kind 0xF and Final Attack 0x10. Other effects preserve prior kind; zero mode no writes.
 */
// FUNCTION: FF7 0x005cd853
void __cdecl Kernel_DescribeSupportMateriaCount(int materiaId, int packedValue)
{
    int level;
    BattleMateriaStatPlusAbility *record;

    level = Kernel_GetMateriaLevel((char)materiaId, packedValue);
    if (g_MateriaDescribeModeFlag != 0) {
        record = g_MateriaCurrentAbilityMask;
        record->primaryCommandByte = *(char *)&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte;
        record->value = (char)level;
        switch (*(char *)&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte) {
        case BATTLE_SUPPORT_EFFECT_ALL:
            g_MateriaDetailPanelKind = 0xF;
            break;
        case BATTLE_SUPPORT_EFFECT_QUADRA_MAGIC:
            g_MateriaDetailPanelKind = 0xF;
            break;
        case BATTLE_SUPPORT_EFFECT_FINAL_ATTACK:
            g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        }
    }
}

/*
 * Computes materia level before description-mode guard. In enabled mode writes indexed support
 * code and level times ten into descriptor bytes; Counter and Magic Counter select panel kind 0xF,
 * the other listed effects 0x10. Other codes preserve prior kind.
 */
// FUNCTION: FF7 0x005cd8e6
void __cdecl Kernel_DescribeSupportMateriaPercent(int materiaId, int packedValue)
{
    int level;
    BattleMateriaStatPlusAbility *record;

    level = Kernel_GetMateriaLevel((char)materiaId, packedValue);
    if (g_MateriaDescribeModeFlag == 0) {
        return;
    }
    record = g_MateriaCurrentAbilityMask;
    record->primaryCommandByte = *(char *)&g_KernelMateriaDataArray[(materiaId & 0xFF)].primaryCommandByte;
    record->value = level * 0xA;
    switch (g_KernelMateriaDataArray[(materiaId & 0xFF)].primaryCommandByte) {
    case BATTLE_SUPPORT_EFFECT_MEGA_ALL:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_MP_TURBO:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_MP_ABSORB:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_HP_ABSORB:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_SNEAK_ATTACK:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_ADDED_CUT:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_STEAL_AS_WELL:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_ELEMENTAL:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_ADDED_EFFECT:
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUPPORT;
        break;
    case BATTLE_SUPPORT_EFFECT_COUNTER:
        g_MateriaDetailPanelKind = 0xF;
        break;
    case BATTLE_SUPPORT_EFFECT_MAGIC_COUNTER:
        g_MateriaDetailPanelKind = 0xF;
    }
}

/*
 * Calls Kernel_GetMateriaLevel(materiaId, packedValue), then, if the flag byte g_MateriaDescribeModeFlag is nonzero, stores
 * MATERIA_DETAIL_PANEL_MEGA_ALL into g_MateriaDetailPanelKind.
 */
// FUNCTION: FF7 0x005cda14
void __cdecl Kernel_ApplyMegaAllMateria(char materiaId, int packedValue)
{
    Kernel_GetMateriaLevel((char)materiaId, packedValue);
    if (g_MateriaDescribeModeFlag != 0) {
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_MEGA_ALL;
    }
}

/*
 * When global byte zero walks low 24 argument bits and initializes selected enemy-skill entries at
 * indices 72..95, then adds the Enemy Skill command. Otherwise sets the enemy-skill panel kind.
 * Arg1/unusedSecond unused.
 */
// FUNCTION: FF7 0x005cda3c
void __cdecl Kernel_ApplyEnemySkillMateria(char unused, char unusedSecond, int skillMask)
{
    int i;
    int mask;

    if (g_MateriaDescribeModeFlag == 0) {
        mask = skillMask & 0xFFFFFF;
        for (i = 0; i < 0x18; i++) {
            if ((mask & 1) != 0) {
                /* Retail caller prototype (char, char, char) -> void preserved for /Od argument promotion on `Kernel_SetAbilityEntryFromAttackTable`. */
                ((void(__cdecl *)(char, char, char))Kernel_SetAbilityEntryFromAttackTable)((char)i, i + 0x48, i + 0x48);
            }
            mask >>= 1;
        }
        Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_ENEMY_SKILL);
    } else {
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_ENEMY_SKILL;
    }
}

/*
 * Outside describe mode adds commands 5, 6, 7, 9, 10, 11 and 12 to the command list; in describe
 * mode sets the master-command panel kind. Reserved local unused.
 */
// FUNCTION: FF7 0x005cdab6
void __cdecl Kernel_ApplyMasterCommandMateria(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (g_MateriaDescribeModeFlag == 0) {
        Kernel_FindOrAddCommandListSlot(5);
        Kernel_FindOrAddCommandListSlot(6);
        Kernel_FindOrAddCommandListSlot(7);
        Kernel_FindOrAddCommandListSlot(9);
        Kernel_FindOrAddCommandListSlot(0xA);
        Kernel_FindOrAddCommandListSlot(0xB);
        Kernel_FindOrAddCommandListSlot(0xC);
    } else {
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_MASTER_COMMAND;
    }
}

/*
 * Outside describe mode calls Kernel_AddMagicListEntryIfAbsent for 56 indices, adds the Magic command
 * and makes the magic count at least one. In describe mode sets the master-magic panel kind.
 */
// FUNCTION: FF7 0x005cdb18
void __cdecl Kernel_ApplyMasterMagicMateria(void)
{
    int i;

    if (g_MateriaDescribeModeFlag == 0) {
        for (i = 0; i < 0x38; i++) {
            Kernel_AddMagicListEntryIfAbsent((char)i);
        }
        Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_MAGIC);
        if (g_MateriaMagicListCount == 0) {
            g_MateriaMagicListCount = 1;
        }
    } else {
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_MASTER_MAGIC;
    }
}

/*
 * Outside describe mode fills sixteen bytes 0xff, initializes entries 56..71, adds the Summon command
 * and makes the summon count at least one. Describe mode sets the master-summon panel kind.
 */
// FUNCTION: FF7 0x005cdb76
void __cdecl Kernel_ApplyMasterSummonMateria(void)
{
    int i;

    if (g_MateriaDescribeModeFlag == 0) {
        for (i = 0; i < 0x10; i++) {
            g_MateriaSummonLevelCounts[i] = 0xFF;
        }
        for (i = 0x38; i < 0x48; i++) {
            /* Retail caller prototype (int, char, char) -> void preserved for /Od argument promotion on `Kernel_SetAbilityEntryFromAttackTable`. */
            ((void(__cdecl *)(int, char, char))Kernel_SetAbilityEntryFromAttackTable)(i - 0x38, (char)i, (char)i);
        }
        Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_SUMMON);
        if (g_MateriaSummonCount == 0) {
            g_MateriaSummonCount = 1;
        }
    } else {
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_MASTER_SUMMON;
    }
}

/*
 * Computes materia level then dispatches low-byte selector zero, two, four to ability/stat/EXP
 * helpers and three to long-range helper with narrowed level. Other selectors no call.
 */
// FUNCTION: FF7 0x005cdc04
void __cdecl Kernel_ApplyIndependentMateria(int selector, char materiaId, int packedValue)
{
    int level;

    level = Kernel_GetMateriaLevel((char)materiaId, packedValue);
    switch (selector & 0xFF) {
    case BATTLE_MATERIA_SUBTYPE_INDEPENDENT_ABILITY_FLAG:
        Kernel_ApplyMateriaAbilityFlag((char)materiaId, packedValue);
        break;
    case BATTLE_MATERIA_SUBTYPE_INDEPENDENT_STAT_PLUS:
        Kernel_ApplyMateriaStatPlus((char)materiaId, packedValue);
        break;
    case BATTLE_MATERIA_SUBTYPE_INDEPENDENT_EXP_BONUS:
        Kernel_ApplyMateriaExpBonus((char)materiaId, packedValue);
        break;
    case BATTLE_MATERIA_SUBTYPE_INDEPENDENT_LONG_RANGE:
        Kernel_ApplyMateriaLongRange((char)level, (char)materiaId);
    }
}

/*
 * Applies the materia long-range flag for the supported kernel operation selector.
 */
// FUNCTION: FF7 0x005cdc97
void __cdecl Kernel_ApplyMateriaLongRange(char flag, char opSelector)
{
    switch ((unsigned char)opSelector) {
    case MATERIA_LONG_RANGE:
        Kernel_SetLongRangeFlag(flag);
        break;
    }
}

/*
 * If the flag byte g_MateriaDescribeModeFlag is zero it sets PARTY_STATS_LONG_RANGE in g_pCurrentCombatantStatBlock->flags;
 * otherwise it stores panel kind 0x12 into g_MateriaDetailPanelKind.
 */
// FUNCTION: FF7 0x005cdcb9
void __cdecl Kernel_SetLongRangeFlag(char longRange)
{
    if (g_MateriaDescribeModeFlag == 0) {
        g_pCurrentCombatantStatBlock->flags |= PARTY_STATS_LONG_RANGE;
    } else {
        g_MateriaDetailPanelKind = 0x12;
    }
}

/*
 * Calls materia-level helper ignoring result; table selector 0x62 sets PARTY_STATS_HP_MP_SWAP and 0xC bit 0x01
 * in normal mode, or sets the ability-flag panel kind otherwise. Other selectors unchanged; char-address
 * int read ABI caveat.
 */
// FUNCTION: FF7 0x005cdce6
void __cdecl Kernel_ApplyMateriaAbilityFlag(char materiaId, int ap)
{
    Kernel_GetMateriaLevel((char)materiaId, ap);
    if (g_MateriaDescribeModeFlag == 0) {
        if (g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte == 0x62) {
            g_pCurrentCombatantStatBlock->flags |= PARTY_STATS_HP_MP_SWAP;
        }
        if (g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte == 0xC) {
            g_pCurrentCombatantStatBlock->flags = g_pCurrentCombatantStatBlock->flags | 1;
        }
    } else {
        if (g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte == 0x62) {
            g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_ABILITY_FLAG;
        }
        if (g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte == 0xC) {
            g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_ABILITY_FLAG;
        }
    }
}

/*
 * Gets materia level; Counter Attack appends its counter trigger instead. Otherwise mode zero adds
 * indexed byte to selected short stat; other mode writes two descriptor bytes and the stat-plus panel kind.
 */
// FUNCTION: FF7 0x005cdda3
void __cdecl Kernel_ApplyMateriaStatPlus(unsigned char materiaId, int ap)
{
    int level;
    BattleMateriaStatPlusAbility *record;

    level = Kernel_GetMateriaLevel(materiaId, ap);
    if (materiaId == MATERIA_COUNTER_ATTACK) {
        Kernel_AppendCounterAttackTrigger(level, materiaId);
    } else {
        if (g_MateriaDescribeModeFlag == 0) {
            g_MateriaStatPercentModifiers.byIndex[g_KernelMateriaDataArray[materiaId].primaryCommandByte] += g_KernelMateriaDataArray[materiaId].attributes[level];
        } else {
            record = g_MateriaCurrentAbilityMask;
            record->primaryCommandByte = (*(char *)&g_KernelMateriaDataArray[materiaId].primaryCommandByte);
            record->value = (char)g_KernelMateriaDataArray[materiaId].attributes[level];
            g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_STAT_PLUS;
        }
    }
}

/*
 * For mode zero and count not equal eight appends a counter-attack reaction (type 9, attack 9,
 * chance from the indexed table byte) then increments count. Other mode writes global 18. Count
 * above eight still writes; char-argument int reads retain ABI caveat.
 */
// FUNCTION: FF7 0x005cde86
void __cdecl Kernel_AppendCounterAttackTrigger(char level, char materiaId)
{
    if (g_MateriaDescribeModeFlag == 0) {
        if (g_MateriaTriggerCount != 8) {
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].typeId = BATTLE_REACTION_COUNTER_ATTACK;
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].attackIndex = 9;
            g_pCurrentCombatantStatBlock->reactions[g_MateriaTriggerCount].chance =
                (char)g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].attributes[(*(int *)&level & 0xFF)];
            g_MateriaTriggerCount++;
        }
    } else {
        g_MateriaDetailPanelKind = 0x12;
    }
}

/*
 * Searches materia levels downward for first non 0xff bonus byte, then adds to selected stat or
 * writes the descriptor and the exp-bonus panel kind, by mode. If no qualifying level q remains uninitialized.
 */
// FUNCTION: FF7 0x005cdf0c
void __cdecl Kernel_ApplyMateriaExpBonus(unsigned char materiaId, int ap)
{
    int levelIndex;
    int val;
    int level;
    BattleMateriaStatPlusAbility *record;

    level = Kernel_GetMateriaLevel(materiaId, ap);
    for (levelIndex = level; levelIndex > 0; levelIndex--) {
        if (g_KernelMateriaDataArray[materiaId].attributes[levelIndex] != 0xFF) {
            val = g_KernelMateriaDataArray[materiaId].attributes[levelIndex];
            break;
        }
    }
    if (g_MateriaDescribeModeFlag == 0) {
        g_MateriaStatPercentModifiers.byIndex[g_KernelMateriaDataArray[materiaId].primaryCommandByte] += val;
    } else {
        record = g_MateriaCurrentAbilityMask;
        record->primaryCommandByte = (*(char *)&g_KernelMateriaDataArray[materiaId].primaryCommandByte);
        record->value = val;
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_EXP_BONUS;
    }
}

/*
 * Dispatches selector four to encounter-rate materia helper and selector two to preemptive materia
 * helper, passing the remaining arguments. Other selectors perform no call.
 */
// FUNCTION: FF7 0x005cdff9
void __cdecl Kernel_ApplyEncounterMateria(char selector, char materiaId, int ap)
{
    switch (selector) {
    case BATTLE_MATERIA_SUBTYPE_ENCOUNTER_RATE:
        Kernel_ApplyMateriaEncounterRate(materiaId, ap);
        break;
    case BATTLE_MATERIA_SUBTYPE_ENCOUNTER_MODIFIER:
        Kernel_ApplyMateriaEncounterModifier(materiaId, ap);
        break;
    }
}

/*
 * Searches downward for first non 0xff bonus; normal mode adds it to selected short, and for
 * Pre-Emptive clamps its sum to 85 and at level five sets g_MateriaPreEmptiveRateSum. Descriptor mode writes selector/level. No found
 * bonus leaves q uninitialized.
 */
// FUNCTION: FF7 0x005ce037
void __cdecl Kernel_ApplyMateriaEncounterModifier(unsigned char materiaId, int ap)
{
    int levelIndex;
    int val;
    int level;
    BattleMateriaStatPlusAbility *record;

    level = Kernel_GetMateriaLevel(materiaId, ap);
    for (levelIndex = level; levelIndex > 0; levelIndex--) {
        if (g_KernelMateriaDataArray[materiaId].attributes[levelIndex] != 0xFF) {
            val = g_KernelMateriaDataArray[materiaId].attributes[levelIndex];
            break;
        }
    }
    if (g_MateriaDescribeModeFlag == 0) {
        g_MateriaEncounterModifiers[g_KernelMateriaDataArray[materiaId].primaryCommandByte] += val;
        if (materiaId == MATERIA_PRE_EMPTIVE) {
            if (g_MateriaEncounterModifiers[3] > 0x55) {
                g_MateriaEncounterModifiers[3] = 0x55;
            }
            if (level == 5) {
                g_MateriaPreEmptiveRateSum = 1;
            }
        }
    } else {
        record = g_MateriaCurrentAbilityMask;
        record->primaryCommandByte = g_KernelMateriaDataArray[materiaId].primaryCommandByte;
        record->value = level;
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_ENCOUNTER_MODIFIER;
    }
}

/*
 * Derives materia level and searches downward for first nonsentinel bonus into r for Enemy Away or q
 * otherwise. Normal mode adds r to global short for Enemy Away, else q to indexed short. Description
 * mode always writes q, including uninitialized q for Enemy Away; no found bonus also leaves selected
 * local uninitialized.
 */
// FUNCTION: FF7 0x005ce153
void __cdecl Kernel_ApplyMateriaEncounterRate(char materiaId, int ap)
{
    int levelIndex;
    int rateVal;
    int val;
    int level;
    BattleMateriaStatPlusAbility *record;

    level = Kernel_GetMateriaLevel((char)materiaId, ap);
    if ((*(int *)&materiaId & 0xFF) == MATERIA_ENEMY_AWAY) {
        for (levelIndex = level; levelIndex > 0; levelIndex--) {
            if (g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].attributes[levelIndex] != 0xFF) {
                rateVal = g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].attributes[levelIndex];
                break;
            }
        }
    } else {
        for (levelIndex = level; levelIndex > 0; levelIndex--) {
            if (g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].attributes[levelIndex] != 0xFF) {
                val = g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].attributes[levelIndex];
                break;
            }
        }
    }
    if (g_MateriaDescribeModeFlag == 0) {
        if ((*(int *)&materiaId & 0xFF) == MATERIA_ENEMY_AWAY) {
            (g_MateriaEncounterRateSum) += rateVal;
        } else {
            g_MateriaEncounterModifiers[g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte] += val;
        }
    } else {
        record = g_MateriaCurrentAbilityMask;
        record->primaryCommandByte = *(char *)&g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte;
        record->value = val;
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_ENCOUNTER_RATE;
    }
}

/*
 * Finds highest indexed non 0xffff threshold met by ap against table short times hundred, default
 * level one. Counts all valid thresholds plus one globally. Description mode writes level/max and
 * next-threshold difference, or zero for terminal/sentinel; threshold ordering not validated.
 */
// FUNCTION: FF7 0x005ce2c5
int __cdecl Kernel_GetMateriaLevel(unsigned char materiaId, int ap)
{
    int idx;
    int level;
    BattleMateriaStatPlusAbility *record;

    level = 1;
    for (idx = 3; idx >= 0; idx--) {
        if (g_KernelMateriaDataArray[materiaId].apLevels[idx] == 0xFFFF) {
            continue;
        }
        if (ap >= g_KernelMateriaDataArray[materiaId].apLevels[idx] * 0x64) {
            level = idx + 2;
            break;
        }
    }
    (g_MateriaMasteredLevelCount) = 1;
    for (idx = 0; idx < 4; idx++) {
        if (g_KernelMateriaDataArray[materiaId].apLevels[idx] != 0xFFFF) {
            (g_MateriaMasteredLevelCount)++;
        }
    }
    if (g_MateriaDescribeModeFlag != 0) {
        /* The AP threshold of the next level, apLevels[level - 1] of the materia, addressed from
         * the item table just below the materia table: its +0xDFE is two bytes before
         * g_KernelMateriaDataArray[0].apLevels[0]. */
        if (*(unsigned short *)((char *)g_MenuKernelItemBuffer + 0xdfe + (materiaId * 0x14 + level * 2)) == 0xFFFF || level == (g_MateriaMasteredLevelCount)) {
            g_MateriaLookupLevel = 0;
        } else {
            g_MateriaLookupLevel = *(unsigned short *)((char *)g_MenuKernelItemBuffer + 0xdfe + (materiaId * 0x14 + level * 2)) * 0x64 - ap;
        }
        record = g_MateriaCurrentAbilityMask;
        record->level = level;
        record->masteredCount = *(char *)&(g_MateriaMasteredLevelCount);
        record->lookupLevel = g_MateriaLookupLevel;
    }
    return level;
}

/*
 * Upper-clamps four global signed-short values to 32/255 then adds five low bytes into destination
 * fields, wrapping byte sums. No lower clamp and fifth value unclamped.
 */
// FUNCTION: FF7 0x005ce425
void __cdecl Kernel_StoreEncounterModifiersToStatBlock(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (g_MateriaEncounterModifiers[0] > 0x20) {
        g_MateriaEncounterModifiers[0] = 0x20;
    }
    if (g_MateriaEncounterModifiers[2] > 0x20) {
        g_MateriaEncounterModifiers[2] = 0x20;
    }
    if (g_MateriaEncounterModifiers[1] > 0xFF) {
        g_MateriaEncounterModifiers[1] = 0xFF;
    }
    if (g_MateriaEncounterRateSum > 0xFF) {
        g_MateriaEncounterRateSum = 0xFF;
    }
    g_pCurrentCombatantStatBlock->gilScaleBonus += *(char *)&g_MateriaEncounterModifiers[0];
    g_pCurrentCombatantStatBlock->encounterRateUp += *(char *)&g_MateriaEncounterModifiers[1];
    g_pCurrentCombatantStatBlock->encounterRateDown += *(char *)&g_MateriaEncounterRateSum;
    g_pCurrentCombatantStatBlock->chocoboLureBonus += *(char *)&g_MateriaEncounterModifiers[2];
    g_pCurrentCombatantStatBlock->preemptiveBonus += *(char *)&g_MateriaEncounterModifiers[3];
}

/*
 * Caps eleven percentage SHORTs only above 100 and another global field above 32. Masks six
 * nonnegative additive bonuses to their low byte, then applies percentage and additive modifiers
 * to six base stats and percentage modifiers to four further values. Clamps the six stats to zero
 * through 255 and two larger values to zero through 9999 and 999; the other modified values
 * receive no clamp here. Stores selected bytes and SHORTs into the global destination record,
 * including two unmodified fields. Negative percentages remain allowed, arithmetic narrows into
 * global types.
 */
// FUNCTION: FF7 0x005ce517
void __cdecl Kernel_ApplyMateriaStatModifiersToStatBlock(void)
{
    int i;

    for (i = 0; i < 0xB; i++) {
        if (g_MateriaStatPercentModifiers.byIndex[i] > 0x64) {
            g_MateriaStatPercentModifiers.byIndex[i] = 0x64;
        }
    }
    if (g_MateriaStatPercentModifiers.expPercent > 0x20) {
        g_MateriaStatPercentModifiers.expPercent = 0x20;
    }
    for (i = 0; i < 6; i++) {
        if ((g_MateriaAdditiveStatBonuses[i] & 0x8000) == 0) {
            g_MateriaAdditiveStatBonuses[i] &= 0xFF;
        }
    }
    g_MateriaStatScratchStrength.value = g_MateriaStatScratchStrength.value + g_MateriaStatScratchStrength.value * (g_MateriaStatPercentModifiers.scratchStrength) / 0x64 + g_MateriaAdditiveStatBonuses[0];
    g_MateriaStatScratchStrength.second = g_MateriaStatScratchStrength.second + g_MateriaStatScratchStrength.second * g_MateriaStatPercentModifiers.vitalityPercent / 0x64 + g_MateriaAdditiveStatBonuses[1];
    g_MateriaStatScratchMagic.value = g_MateriaStatScratchMagic.value + g_MateriaStatScratchMagic.value * g_MateriaStatPercentModifiers.magicPercent / 0x64 + g_MateriaAdditiveStatBonuses[2];
    g_MateriaStatScratchMagic.second = g_MateriaStatScratchMagic.second + g_MateriaStatScratchMagic.second * g_MateriaStatPercentModifiers.spiritPercent / 0x64 + g_MateriaAdditiveStatBonuses[3];
    g_MateriaStatScratchDexterity.value = g_MateriaStatScratchDexterity.value + g_MateriaStatScratchDexterity.value * g_MateriaStatPercentModifiers.dexterityPercent / 0x64 + g_MateriaAdditiveStatBonuses[4];
    g_MateriaStatScratchDexterity.second = g_MateriaStatScratchDexterity.second + g_MateriaStatScratchDexterity.second * g_MateriaStatPercentModifiers.luckPercent / 0x64 + g_MateriaAdditiveStatBonuses[5];
    g_MateriaStatScratchMaxHp.value = g_MateriaStatScratchMaxHp.value + g_MateriaStatScratchMaxHp.value * g_MateriaStatPercentModifiers.maxHpPercent / 0x64;
    g_MateriaStatScratchAttack.value = g_MateriaStatScratchAttack.value + g_MateriaStatScratchAttack.value * g_MateriaStatPercentModifiers.attackPercent / 0x64;
    g_MateriaStatScratchAttack.second = g_MateriaStatScratchAttack.second + g_MateriaStatScratchAttack.second * g_MateriaStatPercentModifiers.defensePercent / 0x64;
    g_MateriaStatScratchMaxHp.second = g_MateriaStatScratchMaxHp.second + g_MateriaStatScratchMaxHp.second * g_MateriaStatPercentModifiers.maxMpPercent / 0x64;
    if (g_MateriaStatScratchStrength.value > 0xFF) {
        g_MateriaStatScratchStrength.value = 0xFF;
    }
    if (g_MateriaStatScratchStrength.second > 0xFF) {
        g_MateriaStatScratchStrength.second = 0xFF;
    }
    if (g_MateriaStatScratchMagic.value > 0xFF) {
        g_MateriaStatScratchMagic.value = 0xFF;
    }
    if (g_MateriaStatScratchMagic.second > 0xFF) {
        g_MateriaStatScratchMagic.second = 0xFF;
    }
    if (g_MateriaStatScratchDexterity.value > 0xFF) {
        g_MateriaStatScratchDexterity.value = 0xFF;
    }
    if (g_MateriaStatScratchDexterity.second > 0xFF) {
        g_MateriaStatScratchDexterity.second = 0xFF;
    }
    if (g_MateriaStatScratchMaxHp.value > 0x270F) {
        g_MateriaStatScratchMaxHp.value = 0x270F;
    }
    if (g_MateriaStatScratchMaxHp.second > 0x3E7) {
        g_MateriaStatScratchMaxHp.second = 0x3E7;
    }
    if (g_MateriaStatScratchStrength.value < 0) {
        g_MateriaStatScratchStrength.value = 0;
    }
    if (g_MateriaStatScratchStrength.second < 0) {
        g_MateriaStatScratchStrength.second = 0;
    }
    if (g_MateriaStatScratchMagic.value < 0) {
        g_MateriaStatScratchMagic.value = 0;
    }
    if (g_MateriaStatScratchMagic.second < 0) {
        g_MateriaStatScratchMagic.second = 0;
    }
    if (g_MateriaStatScratchDexterity.value < 0) {
        g_MateriaStatScratchDexterity.value = 0;
    }
    if (g_MateriaStatScratchDexterity.second < 0) {
        g_MateriaStatScratchDexterity.second = 0;
    }
    if (g_MateriaStatScratchMaxHp.value < 0) {
        g_MateriaStatScratchMaxHp.value = 0;
    }
    if (g_MateriaStatScratchMaxHp.second < 0) {
        g_MateriaStatScratchMaxHp.second = 0;
    }
    g_pCurrentCombatantStatBlock->magicAttack = g_MateriaStatScratchMagicAttack.value;
    g_pCurrentCombatantStatBlock->magicDefense = g_MateriaStatScratchMagicAttack.second;
    g_pCurrentCombatantStatBlock->strength = g_MateriaStatScratchStrength.low;
    g_pCurrentCombatantStatBlock->vitality = g_MateriaStatScratchStrength.secondLow;
    g_pCurrentCombatantStatBlock->magic = g_MateriaStatScratchMagic.low;
    g_pCurrentCombatantStatBlock->spirit = g_MateriaStatScratchMagic.secondLow;
    g_pCurrentCombatantStatBlock->dexterity = g_MateriaStatScratchDexterity.low;
    g_pCurrentCombatantStatBlock->luck = g_MateriaStatScratchDexterity.secondLow;
    g_pCurrentCombatantStatBlock->maxHp = g_MateriaStatScratchMaxHp.value;
    g_pCurrentCombatantStatBlock->maxMp = g_MateriaStatScratchMaxHp.second;
    g_pCurrentCombatantStatBlock->statBlock[0] = *(char *)&g_MateriaStatPercentModifiers.expPercent;
    g_pCurrentCombatantStatBlock->coverChance = g_MateriaStatPercentModifiers.coverChance;
}

/*
 * Gets summon level/ID; normal mode adds level to non 0xff usage byte, initializes summon entry,
 * adds the Summon command and makes the summon count at least one. Descriptor mode writes
 * summon-minus 56/level and the summon panel kind. Char-
 * address int read caveat.
 */
// FUNCTION: FF7 0x005ce996
void __cdecl Kernel_ApplySummonMateria(char materiaId, int ap)
{
    struct {
        int summon;
        BattleMateriaStatPlusAbility *record;
        int unused;
        int level;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.level = Kernel_GetMateriaLevel(materiaId, ap);
    local.summon = g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].primaryCommandByte;
    if (g_MateriaDescribeModeFlag == 0) {
        if (*(unsigned char *)&(&g_MateriaSummonUseCounts[8])[local.summon] != 0xFF) {
            (&g_MateriaSummonUseCounts[8])[local.summon] += local.level;
        }
        /* Retail caller prototype (int, char, char) -> void preserved for /Od argument promotion on `Kernel_SetAbilityEntryFromAttackTable`. */
        ((void(__cdecl *)(int, char, char))Kernel_SetAbilityEntryFromAttackTable)(local.summon - 0x38, (char)local.summon, (char)local.summon);
        Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_SUMMON);
        if (g_MateriaSummonCount == 0) {
            g_MateriaSummonCount = 1;
        }
    } else {
        local.record = g_MateriaCurrentAbilityMask;
        local.record->primaryCommandByte = local.summon - 0x38;
        local.record->value = local.level;
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_SUMMON;
    }
}

/*
 * Copies sixteen three-byte groups from three separate global byte arrays into stride-eight
 * destination offsets 0x2CA/0x2CB/0x2CC.
 */
// FUNCTION: FF7 0x005cea58
void __cdecl Kernel_CopySummonEntryBytesToStatBlock(void)
{
    int i;

    for (i = 0; i < 0x10; i++) {
        g_pCurrentCombatantStatBlock->summons[i].allCount = g_MateriaSummonLevelCounts[i];
        g_pCurrentCombatantStatBlock->summons[i].quadraLinks = g_MateriaSummonUseCounts[i];
        g_pCurrentCombatantStatBlock->summons[i].quadraCount = g_MateriaQuadraMagicCounts[i];
    }
}

/*
 * Normal mode grants every non 0xff entry from level-minus-one downward, adds the Magic command and
 * makes the magic count at least one. Descriptor mode writes five entries then marks first level
 * entries enabled, sets the magic panel kind. Excess level overruns descriptor.
 */
// FUNCTION: FF7 0x005ceac5
void __cdecl Kernel_ApplyMagicMateria(char materiaId, int ap)
{
    int idx;
    int level;
    BattleMateriaStatPlusAbility *record;

    level = Kernel_GetMateriaLevel(materiaId, ap);
    if (g_MateriaDescribeModeFlag == 0) {
        for (idx = level - 1; idx >= 0; idx--) {
            if (g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].attributes[idx] != 0xFF) {
                Kernel_AddMagicListEntryIfAbsent(g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].attributes[idx]);
            }
        }
        Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_MAGIC);
        if (g_MateriaMagicListCount == 0) {
            g_MateriaMagicListCount = 1;
        }
    } else {
        record = g_MateriaCurrentAbilityMask;
        for (idx = 0; idx < 5; idx++) {
            record->entries[idx].id = (char)g_KernelMateriaDataArray[(*(int *)&materiaId & 0xFF)].attributes[idx];
            record->entries[idx].marked = 0;
        }
        for (idx = 0; idx < level; idx++) {
            record->entries[idx].marked = 1;
        }
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_MAGIC;
    }
}

/*
 * Normal mode scans level-minus-one down through zero for non 0xff command then calls helper;
 * absent entry uses index minus one. Descriptor mode writes five entries and level marker, sets
 * the command panel kind.
 */
// FUNCTION: FF7 0x005cebe5
void __cdecl Kernel_ApplyCommandMateria(unsigned char materiaId, int ap)
{
    int idx;
    int level;
    BattleMateriaStatPlusAbility *record;

    level = Kernel_GetMateriaLevel(materiaId, ap);
    if (g_MateriaDescribeModeFlag == 0) {
        for (idx = level - 1; idx >= 0; idx--) {
            if (g_KernelMateriaDataArray[materiaId].attributes[idx] != 0xFF) {
                break;
            }
        }
        Kernel_FindOrAddCommandListSlot(g_KernelMateriaDataArray[materiaId].attributes[idx]);
    } else {
        record = g_MateriaCurrentAbilityMask;
        for (idx = 0; idx < 5; idx++) {
            record->entries[idx].id = (char)g_KernelMateriaDataArray[materiaId].attributes[idx];
            record->entries[idx].marked = 0;
        }
        if (record->masteredCount == level) {
            record->entries[level - 2].marked = 1;
        } else {
            record->entries[level - 1].marked = 1;
        }
        g_MateriaDetailPanelKind = MATERIA_DETAIL_PANEL_COMMAND;
    }
}

/*
 * Searches sixteen stride-three leading bytes at g_MateriaCommandListEntries for commandId and returns the first
 * index; otherwise appends commandId at g_MateriaCommandCount*3, increments the count and returns the old count.
 * No capacity guard is present.
 */
// FUNCTION: FF7 0x005cece9
int __cdecl Kernel_FindOrAddCommandListSlot(unsigned char commandId)
{
    int index;

    for (index = 0; index < 0x10; index++) {
        if (g_MateriaCommandListEntries[index].commandCode == commandId) {
            return index;
        }
    }
    g_MateriaCommandListEntries[g_MateriaCommandCount].commandCode = commandId;
    g_MateriaCommandCount++;
    return g_MateriaCommandCount - 1;
}

/*
 * Builds the battle command menu. Zeroes the counter g_MateriaBattleCommandPlacedCount and
 * g_MateriaTriggerCount, calls Kernel_RemoveCommand5IfCommand17Present, then for each of the 16
 * three-byte entries at &g_MateriaCommandListEntries whose first byte is not 0xFF: rewrites Magic
 * to W-Magic when the byte at g_MateriaMagicListCount is 2 and Summon to W-Summon when
 * the byte at g_MateriaSummonCount is 2, asks Kernel_GetBattleMenuSlotForCommand for
 * its slot, calls Kernel_SetBattleMenuCommand(command, slot) and bumps the counter. Finally writes
 * (count - 1) / 4 + 1 (the number of menu rows) into the byte at g_pCurrentCombatantStatBlock +
 * 0x21.
 */
// FUNCTION: FF7 0x005ced53
void __cdecl Kernel_BuildBattleCommandMenu(void)
{
    int slot;
    int index;

    g_MateriaBattleCommandPlacedCount = 0;
    g_MateriaTriggerCount = 0;
    Kernel_RemoveCommand5IfCommand17Present();
    for (index = 0; index < 0x10; index++) {
        if (g_MateriaCommandListEntries[index].commandCode != 0xFF) {
            if (g_MateriaCommandListEntries[index].commandCode == BATTLE_COMMAND_MAGIC && g_MateriaMagicListCount == 2) {
                g_MateriaCommandListEntries[index].commandCode = BATTLE_COMMAND_W_MAGIC;
            }
            if (g_MateriaCommandListEntries[index].commandCode == BATTLE_COMMAND_SUMMON && g_MateriaSummonCount == 2) {
                g_MateriaCommandListEntries[index].commandCode = BATTLE_COMMAND_W_SUMMON;
            }
            slot = Kernel_GetBattleMenuSlotForCommand(g_MateriaCommandListEntries[index].commandCode);
            Kernel_SetBattleMenuCommand(g_MateriaCommandListEntries[index].commandCode, slot);
            (g_MateriaBattleCommandPlacedCount)++;
        }
    }
    g_pCurrentCombatantStatBlock->commandColumns = (g_MateriaBattleCommandPlacedCount - 1) / 4 + 1;
}

/*
 * Scans sixteen command entries for 0x11; if found replaces every entry five with 0xff.
 */
// FUNCTION: FF7 0x005cee69
void __cdecl Kernel_RemoveCommand5IfCommand17Present(void)
{
    struct {
        int i;
        int found;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.found = 0;
    for (local.i = 0; local.i < 0x10; local.i++) {
        if (g_MateriaCommandListEntries[local.i].commandCode == 0x11) {
            local.found = 1;
            break;
        }
    }
    for (local.i = 0; local.i < 0x10; local.i++) {
        if (local.found == 1 && g_MateriaCommandListEntries[local.i].commandCode == 5) {
            g_MateriaCommandListEntries[local.i].commandCode = 0xFF;
        }
    }
}

/*
 * Maps a battle command id to a menu slot. Ids 0x18-0x1B and Attack return 0; Item and W-Item
 * return 3; Magic and W-Magic return 1; Summon and W-Summon return 2. Any other id takes the first free slot: it sets the byte at
 * g_MateriaMagicListCount and returns 1 if that byte was clear, else sets the byte at
 * g_MateriaSummonCount and returns 2 if that was clear, else returns the first i in
 * 4..15 whose byte at g_pCurrentCombatantStatBlock + i * 6 + 0x4C is 0xFF (or 16 if none).
 */
// FUNCTION: FF7 0x005ceef0
int __cdecl Kernel_GetBattleMenuSlotForCommand(int commandId)
{
    int i;

    if (commandId == 0x18) {
        return 0;
    }
    if (commandId == 0x19) {
        return 0;
    }
    if (commandId == 0x1A) {
        return 0;
    }
    if (commandId == 0x1B) {
        return 0;
    }
    if (commandId == BATTLE_COMMAND_ATTACK) {
        return 0;
    }
    if (commandId == BATTLE_COMMAND_ITEM) {
        return 3;
    }
    if (commandId == BATTLE_COMMAND_W_ITEM) {
        return 3;
    }
    if (commandId == BATTLE_COMMAND_MAGIC) {
        return 1;
    }
    if (commandId == BATTLE_COMMAND_W_MAGIC) {
        return 1;
    }
    if (commandId == BATTLE_COMMAND_SUMMON) {
        return 2;
    }
    if (commandId == BATTLE_COMMAND_W_SUMMON) {
        return 2;
    }
    if (g_MateriaMagicListCount == 0) {
        g_MateriaMagicListCount = 1;
        return 1;
    }
    if (g_MateriaSummonCount == 0) {
        g_MateriaSummonCount = 1;
        return 2;
    }
    for (i = 4; i < 0x10; i++) {
        if (g_pCurrentCombatantStatBlock->commands[i].commandId == 0xFF) {
            return i;
        }
    }
    return i;
}

/*
 * Writes command ID/default/table fields into selected six-byte menu record then copies extra two
 * bytes from first matching command in sixteen-entry table. Absent match preserves previous
 * extras.
 */
// FUNCTION: FF7 0x005cf002
void __cdecl Kernel_SetBattleMenuCommand(unsigned char commandId, unsigned char slot)
{
    int i;

    g_pCurrentCombatantStatBlock->commands[slot].commandId = commandId;
    g_pCurrentCombatantStatBlock->commands[slot].menuFlags = 0;
    g_pCurrentCombatantStatBlock->commands[slot].cursorAction = g_KernelCommandTable[commandId].cursorAction;
    g_pCurrentCombatantStatBlock->commands[slot].targetFlags = *(char *)&g_KernelCommandTable[commandId].targetFlags;
    for (i = 0; i < 0x10; i++) {
        if (commandId == g_MateriaCommandListEntries[i].commandCode) {
            g_pCurrentCombatantStatBlock->commands[slot].allCount = g_MateriaCommandListEntries[i].level;
            g_pCurrentCombatantStatBlock->commands[slot].supportFlags = g_MateriaCommandListEntries[i].supportFlags;
            break;
        }
    }
}

/*
 * For 56 stride-five magic records invokes setup with signed-byte IDs and copies four further bytes
 * into destination stride-eight offsets 0x10A/B/C/F.
 */
// FUNCTION: FF7 0x005cf0fc
void __cdecl Kernel_CopyMagicListToAbilityEntries(void)
{
    int i;

    for (i = 0; i < 0x38; i++) {
        /* Retail caller prototype (char, char, char) -> void preserved for /Od argument promotion on `Kernel_SetAbilityEntryFromAttackTable`. */
        ((void(__cdecl *)(char, char, char))Kernel_SetAbilityEntryFromAttackTable)((char)g_MateriaMagicListRecords[i].magicId, (char)i,
                                                                                   (char)g_MateriaMagicListRecords[i].magicId);
        g_pCurrentCombatantStatBlock->abilities[i].allCount = g_MateriaMagicListRecords[i].allCount;
        g_pCurrentCombatantStatBlock->abilities[i].quadraLinks = g_MateriaMagicListRecords[i].quadraLinks;
        g_pCurrentCombatantStatBlock->abilities[i].quadraCount = g_MateriaMagicListRecords[i].quadraCount;
        g_pCurrentCombatantStatBlock->abilities[i].support = g_MateriaMagicListRecords[i].support;
    }
}

/*
 * Searches 56 stride-five leading bytes at g_MateriaMagicListRecords for magicId low byte; if absent, appends it
 * at g_MateriaCommandListRecordCount*5 and increments that count. No capacity guard is present.
 */
// FUNCTION: FF7 0x005cf1ba
void __cdecl Kernel_AddMagicListEntryIfAbsent(char magicId)
{
    int i;

    for (i = 0; i < 0x38; i++) {
        if (*(unsigned char *)&g_MateriaMagicListRecords[i].magicId == (*(int *)&magicId & 0xFF)) {
            return;
        }
    }
    g_MateriaMagicListRecords[g_MateriaCommandListRecordCount].magicId = *(int *)&magicId;
    g_MateriaCommandListRecordCount++;
}

/*
 * Masks destination and source indices to low byte, writes narrowed attackId and two source stride
 * 0x1C bytes into destination stride-eight offsets 0x108/0x109/0x10D.
 */
// FUNCTION: FF7 0x005cf217
void __cdecl Kernel_SetAbilityEntryFromAttackTable(int attackId, int entry, int attackIndex)
{
    g_pCurrentCombatantStatBlock->abilities[(entry & 0xFF)].attackId = attackId;
    g_pCurrentCombatantStatBlock->abilities[(entry & 0xFF)].mpCost = *(char *)&g_BattleAttackTable[(attackIndex & 0xFF)].mpCost;
    g_pCurrentCombatantStatBlock->abilities[(entry & 0xFF)].targetFlags = *(char *)&g_BattleAttackTable[(attackIndex & 0xFF)].targetFlags;
}

/*
 * Initializes materia/stat scratch state and selected actor arrays: clears 56 five-byte records,
 * selected fields in 96 eight-byte ability records, three 16-byte arrays, 12 percentage SHORTs,
 * six additive SHORTs, and 16 reaction triples. Marks 16 command IDs empty with default flags,
 * clears eight three-byte fields in the global stat record, copies its base stats and HP/MP limits
 * into globals, obtains four equipped stat values, runs two initialization helpers, and clears
 * remaining listed counters and extension flags. Unwritten record bytes are preserved rather than
 * a full memset.
 */
// FUNCTION: FF7 0x005cf282
void __cdecl Kernel_InitActorMateriaScratchState(char actorIndex, struct MenuPartyStats *actorState)
{
    int i;

    g_MateriaCommandCount = 0;
    g_MateriaCommandListRecordCount = 0;
    g_MateriaSummonCount = 0;
    g_MateriaMagicListCount = g_MateriaSummonCount;
    for (i = 0; i < 0x38; i++) {
        g_MateriaMagicListRecords[i].magicId = 0xFF;
        g_MateriaMagicListRecords[i].quadraCount = 0;
        g_MateriaMagicListRecords[i].quadraLinks = 0;
        g_MateriaMagicListRecords[i].allCount = 0;
        g_MateriaMagicListRecords[i].support = 0;
    }
    for (i = 0; i < 0x60; i++) {
        actorState->abilities[i].attackId = 0xFF;
        actorState->abilities[i].quadraCount = 0;
        actorState->abilities[i].quadraLinks = 0;
        actorState->abilities[i].allCount = 0;
        actorState->abilities[i].support = 0;
    }
    for (i = 0; i < 0x10; i++) {
        g_MateriaQuadraMagicCounts[i] = 0;
        g_MateriaSummonUseCounts[i] = 0;
        g_MateriaSummonLevelCounts[i] = 0;
    }
    for (i = 0; i < 0xC; i++) {
        g_MateriaStatPercentModifiers.byIndex[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        g_MateriaAdditiveStatBonuses[i] = 0;
    }
    for (i = 0; i < 0x10; i++) {
        g_MateriaCommandListEntries[i].commandCode = 0xFF;
        g_MateriaCommandListEntries[i].level = 0;
        g_MateriaCommandListEntries[i].supportFlags = 0;
    }
    for (i = 0; i < 0x10; i++) {
        actorState->commands[i].commandId = 0xFF;
        actorState->commands[i].menuFlags = 1;
        actorState->commands[i].supportFlags = 0;
    }
    for (i = 0; i < 8; i++) {
        g_pCurrentCombatantStatBlock->reactions[i].chance = 0;
        g_pCurrentCombatantStatBlock->reactions[i].attackIndex = 0;
        g_pCurrentCombatantStatBlock->reactions[i].typeId = 0;
    }
    g_MateriaStatScratchStrength.value = g_pCurrentCombatantStatBlock->strength;
    g_MateriaStatScratchStrength.second = g_pCurrentCombatantStatBlock->vitality;
    g_MateriaStatScratchMagic.value = g_pCurrentCombatantStatBlock->magic;
    g_MateriaStatScratchMagic.second = g_pCurrentCombatantStatBlock->spirit;
    g_MateriaStatScratchDexterity.value = g_pCurrentCombatantStatBlock->dexterity;
    g_MateriaStatScratchDexterity.second = g_pCurrentCombatantStatBlock->luck;
    g_MateriaStatScratchMaxHp.value = g_pCurrentCombatantStatBlock->maxHp;
    g_MateriaStatScratchMaxHp.second = g_pCurrentCombatantStatBlock->maxMp;
    g_MateriaStatScratchLevel.value = g_pCurrentCombatantStatBlock->statBlock[0];
    g_MateriaStatScratchLevel.second = g_pCurrentCombatantStatBlock->coverChance;
    g_pCurrentCombatantStatBlock->flags = 0;
    g_MateriaStatScratchAttack.value = Kernel_GetEquippedAttackOrDefense(actorIndex, 0);
    g_MateriaStatScratchAttack.second = Kernel_GetEquippedAttackOrDefense(actorIndex, 1);
    g_MateriaStatScratchMagicAttack.value = Kernel_GetEquippedAttackOrDefense(actorIndex, 2);
    // BUG: kind 3, the armour's magic defense, is always 0, so the battle magic defense this
    // seeds never includes the armour's MDEF (Kernel_GetEquippedAttackOrDefense).
    g_MateriaStatScratchMagicAttack.second = Kernel_GetEquippedAttackOrDefense(actorIndex, 3);
    Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_ATTACK);
    Kernel_FindOrAddCommandListSlot(BATTLE_COMMAND_ITEM);
    for (i = 0; i < 4; i++) {
        g_MateriaEncounterModifiers[i] = 0;
    }
    (g_MateriaEncounterRateSum) = 0;
    g_pCurrentCombatantStatBlock->gilScaleBonus = 0;
    g_pCurrentCombatantStatBlock->encounterRateUp = 0;
    g_pCurrentCombatantStatBlock->encounterRateDown = 0;
    g_pCurrentCombatantStatBlock->chocoboLureBonus = 0;
    g_pCurrentCombatantStatBlock->preemptiveBonus = 0;
    g_MateriaPreEmptiveRateSum = 0;
}
} /* extern "C" */

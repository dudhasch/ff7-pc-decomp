// src/ff7/field/src/field_encounter_random.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Drives random encounters in the field: advances the encounter and formation random
//   generators, checks encounter triggers while the player moves, and holds an unused
//   encounter-rate handler.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include "ff7/types/field.h"
#include "ff7/field.h"
#include "ff7/savemap.h"
#include "ff7/enums/main.h"
#include "ff7/enums/battle.h"

#include <stdlib.h>
#include <string.h>
#include "ff7/types/menu.h"
#include "lib/types/mem.h"
#include "ff7/types/condor.h"
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "ff7/enums/condor.h"
#include "lib/pfile.h"
#include "lib/types/polygon.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
#include "ff7/enums/field.h"
extern "C" {
#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)
// GLOBAL: FF7 0x00cc0d88
struct FieldActorState g_FieldActorState = {0};
// GLOBAL: FF7 0x00cc0ec0
short g_FieldEntityParams[4] = {0};
// GLOBAL: FF7 0x00cc0ec8
struct FieldLineDrawScratch g_FieldLineDrawScratch[64] = {0};
// g_FieldMovieOffset is indexed by the code as an array. The span runs to the next named definition
// of the TU; the TU's pieces inside 0xcc14c8..0xcc14d0 are aliases.
// GLOBAL: FF7 0x00cc14c8
unsigned char g_FieldMovieOffset[8] = {0};
// GLOBAL: FF7 0x00cc14d0
unsigned char g_FieldTempBank[256] = {0};
// GLOBAL: FF7 0x00cc15d0
short g_FieldEntryMapId = 0;
// GLOBAL: FF7 0x00cc15d4
short g_FieldPreloadMapId = 0;
// GLOBAL: FF7 0x00cc15d8
short g_FieldPreloadMapTarget = 0;
// GLOBAL: FF7 0x00cc15dc
short g_FieldPreloadState = 0;
// Field_CopyEntryMapIdWord keeps a copy of the current map id here (a word store).
// GLOBAL: FF7 0x00cc15e0
short g_FieldLayerScrollParam7 = 0;
// GLOBAL: FF7 0x00cc15e4
short g_FieldBgFlag = 0;
// GLOBAL: FF7 0x00cc15e8
short g_FieldLayerScrollSteps = 0;
// GLOBAL: FF7 0x00cc15ec
short g_FieldLayerScrollStepIndex = 0;
// GLOBAL: FF7 0x00cc15f0
short g_FieldLayerScrollX = 0;
// GLOBAL: FF7 0x00cc15f4
short g_FieldLayerScrollY = 0;
// GLOBAL: FF7 0x00cc15f8
short g_FieldLayerScrollStartX = 0;
// GLOBAL: FF7 0x00cc15fc
short g_FieldLayerScrollStartY = 0;
// GLOBAL: FF7 0x00cc1600
short g_FieldCameraScrollX = 0;
// GLOBAL: FF7 0x00cc1604
short g_FieldCameraScrollY = 0;
// GLOBAL: FF7 0x00cc1608
int g_FieldBgLayer1ScrollX = 0;
// GLOBAL: FF7 0x00cc160c
int g_FieldBgLayer1ScrollY = 0;
// GLOBAL: FF7 0x00cc1610
short g_FieldBgLayer2ScrollX = 0;
// GLOBAL: FF7 0x00cc1614
short g_FieldBgLayer2ScrollY = 0;
// GLOBAL: FF7 0x00cc1618
short g_FieldBgLayer3ScrollX = 0;
// GLOBAL: FF7 0x00cc161c
short g_FieldBgLayer3ScrollY = 0;
// GLOBAL: FF7 0x00cc1620
short g_FieldBgLayer4ScrollX = 0;
// GLOBAL: FF7 0x00cc1624
short g_FieldBgLayer4ScrollY = 0;
// GLOBAL: FF7 0x00cc1628
int g_FieldProjectedScreenPos = 0;
// GLOBAL: FF7 0x00cc162c
short g_FieldPlayerEntityIndex = 0;
// Field_WalkmeshSlideToPoint stores the walkmesh triangle it slid across (word) and which of its three
// edges (0, 1, 2) here.
// GLOBAL: FF7 0x00cc1630
short g_FieldWalkmeshEdge = 0;
// GLOBAL: FF7 0x00cc1634
short g_FieldWalkmeshAxis = 0;
// GLOBAL: FF7 0x00cc1638
short g_FieldLogicActive = 0;
unsigned char g_bssPad_00cc163c[4] = {0};
unsigned char g_bssPad_00cc1640[4] = {0};
// GLOBAL: FF7 0x00cc1644
int g_FieldReturnMode = 0;
// The world map's field-id slot: GameModeDispatcher_Frame stores the current map id before it enters the
// world map, WorldMap_ModuleFrame passes the slot to WorldMap_MainUpdate as its "field" argument, which
// reads it and writes the requested field back (World_ExportTransitionRequestAndSaveState).
// GLOBAL: FF7 0x00cc1648
int g_FieldNextModuleArg = 0;
// GLOBAL: FF7 0x00cc164c
unsigned char g_FieldEncounterState = 0;
// Field_NextFormationRandom's index into g_FieldRandomTable (a byte, incremented before each read).
// GLOBAL: FF7 0x00cc1650
unsigned char g_FieldRandomStepIndex = 0;
// GLOBAL: FF7 0x00cc1654
short g_FieldLastFormationId = 0;

#pragma intrinsic(memset)

#pragma intrinsic(abs)

// clang-format on

/*
 * Generates next pseudo-random value for field encounter calculation.
 */
// FUNCTION: FF7 0x0060b260
int __cdecl Field_NextEncounterRandom(void)
{
    g_FieldRandomIndex1++;
    if (*(unsigned char *)&g_FieldRandomIndex1 == 0) {
        g_FieldRandomIndex2 += 13;
    }
    return g_FieldRandomTable[*(unsigned char *)&g_FieldRandomIndex1] - *(unsigned char *)&g_FieldRandomIndex2;
}

/*
 * Generates next pseudo-random byte for enemy formation selection.
 */
// FUNCTION: FF7 0x0060b2a7
unsigned char __cdecl Field_NextFormationRandom(void)
{
    g_FieldRandomStepIndex++;
    return g_FieldRandomTable[g_FieldRandomStepIndex];
}

/*
 * An encounter table entry packs a formation and its chance: the formation id in the low 10 bits, the
 * chance in the top 6 (out of 64). A mastered Pre-emptive Materia halves the chance of the formations
 * that are bad for the party.
 */
#define ENCOUNTER_FORMATION(entry) ((entry) & 0x3FF)
#define ENCOUNTER_CHANCE(entry) ((char)((entry) >> 10))
#define ENCOUNTER_CHANCE_HALVED(entry) ((char)((entry) >> 11))

/*
 * The random encounter check, run after each walkmesh step of the player.
 *
 * Only every eighth step counts. It lets the Cat's Bell-style regen accessories tick and bumps the
 * savemap step counter, then, if this map's table has encounters on and nothing blocks them, adds the
 * player's speed divided by the table's rate to the danger counter: running fills it faster than
 * walking. Two random bytes follow: the first decides whether the coming battle is pre-emptive, the
 * second starts a battle when it falls below danger * encounterRate / 4096 (encounterRate is the
 * party's, which Enemy Lure and Enemy Away materia change).
 *
 * The formation is drawn from 0..63 against cumulative chances: first the special slots (two back
 * attacks, the side attack, the pincer attack), then the six normal ones, normal[5] being the fallback
 * when none hits. A normal formation equal to the previous battle's is drawn once more, so the same
 * fight rarely comes twice in a row. The result goes to eventCmdParam with a battle request.
 */
// FUNCTION: FF7 0x0060b2c6
void __cdecl Field_CheckRandomEncounter(void)
{
    FieldEncounterEntry *pTable;
    unsigned char cumulative;
    int slot;
    unsigned char rnd;

    if (*(char *)&g_FieldActorState.state.encounterTable == 0) {
        pTable = g_pFieldEncountersAlias;
    } else {
        pTable = &g_pFieldEncountersAlias[1];
    }
    // Wraps to 0 on every eighth step.
    g_FieldEncounterStepCounter += 32;
    if (g_FieldEncounterStepCounter == 0) {
        Party_TickAccessoryRegen();
        gSavemap.encounterStepCountLow++;
        if (*(unsigned char *)&gSavemap.encounterStepCountLow == 0 && gSavemap.encounterStepCountHigh != 0xFF) {
            gSavemap.encounterStepCountHigh++;
        }
        if ((pTable->flags & 1) != 0 && g_FieldLogicActive == 0 && *(char *)&g_FieldActorState.state.encountersDisabled == 0) {
            // The danger counter: faster movement, faster danger.
            g_FieldEncounterRateAccum += (short)(g_FieldEntityState[g_FieldPlayerEntityIndex].moveSpeed / ((pTable->flags & 0xFF00) >> 8));
            rnd = Field_NextEncounterRandom();
            if (rnd < (g_PartyBattleModifiers.preemptiveOdds & PARTY_PREEMPTIVE_ODDS_MASK)) {
                g_FieldEncounterState = FIELD_BATTLE_FLAG_PREEMPTIVE;
            } else {
                g_FieldEncounterState = 0;
            }
            rnd = Field_NextEncounterRandom();
            if ((unsigned int)rnd < (unsigned int)((unsigned int)(*(unsigned short *)&g_FieldEncounterRateAccum * g_PartyBattleModifiers.encounterRate) >> 12)) {
                g_FieldActorState.state.requestCode = FIELD_REQUEST_BATTLE;
                g_FieldEncounterTriggered = 1;
                cumulative = 0;
                /* Called without a prototype in retail: AL is masked as int without movsx/movzx. */
                rnd = (((int(__cdecl *)(void))Field_NextFormationRandom)() & 0xFF) >> 2;
                // Back attack 1. A back attack cancels the pre-emptive roll.
                if ((g_PartyBattleModifiers.preemptiveOdds & PARTY_PREEMPTIVE_MASTERED) == 0) {
                    cumulative += ENCOUNTER_CHANCE(pTable->special[0]);
                } else {
                    cumulative += ENCOUNTER_CHANCE_HALVED(pTable->special[0]);
                }
                if (rnd < cumulative) {
                    g_FieldActorState.state.eventCmdParam = ENCOUNTER_FORMATION(pTable->special[0]);
                    g_FieldEncounterState = 0;
                    return;
                }
                // Back attack 2.
                if ((g_PartyBattleModifiers.preemptiveOdds & PARTY_PREEMPTIVE_MASTERED) == 0) {
                    cumulative += ENCOUNTER_CHANCE(pTable->special[1]);
                } else {
                    cumulative += ENCOUNTER_CHANCE_HALVED(pTable->special[1]);
                }
                if (rnd < cumulative) {
                    g_FieldActorState.state.eventCmdParam = ENCOUNTER_FORMATION(pTable->special[1]);
                    g_FieldEncounterState = 0;
                } else {
                    // Side attack: the party surrounds the enemy, so its chance is never halved.
                    cumulative += ENCOUNTER_CHANCE(pTable->special[2]);
                    if (rnd < cumulative) {
                        g_FieldActorState.state.eventCmdParam = ENCOUNTER_FORMATION(pTable->special[2]);
                        return;
                    }
                    // Pincer attack: the enemy surrounds the party.
                    if ((g_PartyBattleModifiers.preemptiveOdds & PARTY_PREEMPTIVE_MASTERED) == 0) {
                        cumulative += ENCOUNTER_CHANCE(pTable->special[3]);
                    } else {
                        cumulative += ENCOUNTER_CHANCE_HALVED(pTable->special[3]);
                    }
                    if (rnd < cumulative) {
                        g_FieldActorState.state.eventCmdParam = ENCOUNTER_FORMATION(pTable->special[3]);
                    } else {
                        // A normal formation, with a fresh random number.
                        cumulative = 0;
                        /* Retail caller treats `Field_NextFormationRandom` return (`unsigned char`) as `int` for caller zero/sign extension. */
                        rnd = (((int(__cdecl *)(void))Field_NextFormationRandom)() & 0xFF) >> 2;
                        g_FieldActorState.state.eventCmdParam = ENCOUNTER_FORMATION(pTable->normal[5]);
                        for (slot = 0; slot < 5; slot++) {
                            cumulative += ENCOUNTER_CHANCE(pTable->normal[slot]);
                            if (rnd < cumulative) {
                                g_FieldActorState.state.eventCmdParam = ENCOUNTER_FORMATION(pTable->normal[slot]);
                                break;
                            }
                        }
                        if (g_FieldActorState.state.eventCmdParam != g_FieldLastFormationId) {
                            g_FieldLastFormationId = g_FieldActorState.state.eventCmdParam;
                        } else {
                            // The same fight as last time: draw once more.
                            cumulative = 0;
                            /* Retail caller treats `Field_NextFormationRandom` return (`unsigned char`) as `int` for caller zero/sign extension. */
                            rnd = (((int(__cdecl *)(void))Field_NextFormationRandom)() & 0xFF) >> 2;
                            g_FieldActorState.state.eventCmdParam = ENCOUNTER_FORMATION(pTable->normal[5]);
                            for (slot = 0; slot < 5; slot++) {
                                cumulative += ENCOUNTER_CHANCE(pTable->normal[slot]);
                                if (rnd < cumulative) {
                                    g_FieldActorState.state.eventCmdParam = ENCOUNTER_FORMATION(pTable->normal[slot]);
                                    g_FieldLastFormationId = g_FieldActorState.state.eventCmdParam;
                                    return;
                                }
                            }
                            // BUG: when the second draw falls through to normal[5], the previous
                            // formation is not updated, so normal[5] can follow itself unchecked.
                        }
                    }
                }
            }
        }
    }
}

/*
 * Unused encounter rate and formation update handler.
 */
// FUNCTION: FF7 0x0060b6f3
void __cdecl Field_UpdateEncountersUnused(void)
{
    FieldEncounterEntry *pTable;
    unsigned char sum;
    unsigned char rnd;
    int slot;
    short step[4];

    step[0] = 40;
    step[1] = 20;
    step[2] = 12;
    step[3] = 8;
    if (*(char *)&g_FieldActorState.state.encounterTable == 0) {
        pTable = g_pFieldEncountersAlias;
    } else {
        pTable = &g_pFieldEncountersAlias[1];
    }
    if ((pTable->flags & 1) != 0 && *(char *)&g_FieldActorState.state.encountersDisabled == 0) {
        g_FieldEncounterStepCounter += 16;
        if (g_FieldEncounterStepCounter == 0) {
            *(unsigned short *)&g_FieldEncounterRateAccum += g_FieldEntityState[g_FieldPlayerEntityIndex].moveSpeed / step[(pTable->flags & 0xC000) >> 14];
            *(unsigned char *)&g_FieldRandomIndex1 += 1;
            if (*(unsigned char *)&g_FieldRandomIndex1 == 0) {
                *(unsigned char *)&g_FieldRandomIndex2 += 13;
            }
            rnd = g_FieldRandomTable[*(unsigned char *)&g_FieldRandomIndex1] - *(unsigned char *)&g_FieldRandomIndex2;
            if (rnd < ((*(unsigned short *)&g_FieldEncounterRateAccum >> 8) & 0xFF)) {
                Field_ClearAdjacentPreloadReady();
                g_FieldActorState.state.requestCode = FIELD_REQUEST_BATTLE;
                g_FieldEncounterTriggered = 1;
                rnd = g_FieldRandomTable[*(unsigned char *)&g_FieldRandomIndex1] >> 2;
                *(unsigned char *)&g_FieldRandomIndex1 += 1;
                if (rnd < (pTable->special[0] >> 10)) {
                    g_FieldActorState.state.eventCmdParam = pTable->special[0] & 0x3FF;
                }
                if (rnd < (pTable->special[0] >> 10) + (pTable->special[1] >> 10)) {
                    g_FieldActorState.state.eventCmdParam = pTable->special[0] & 0x3FF;
                }
                rnd = g_FieldRandomTable[*(unsigned char *)&g_FieldRandomIndex1] >> 2;
                *(unsigned char *)&g_FieldRandomIndex1 += 1;
                if (rnd < (pTable->special[2] >> 10)) {
                    g_FieldActorState.state.eventCmdParam = pTable->special[2] & 0x3FF;
                }
                rnd = g_FieldRandomTable[*(unsigned char *)&g_FieldRandomIndex1] >> 2;
                *(unsigned char *)&g_FieldRandomIndex1 += 1;
                if (rnd < (pTable->special[3] >> 10)) {
                    g_FieldActorState.state.eventCmdParam = pTable->special[3] & 0x3FF;
                }
                sum = 0;
                for (slot = 0; slot < 6; slot++) {
                    rnd = g_FieldRandomTable[*(unsigned char *)&g_FieldRandomIndex1] >> 2;
                    *(unsigned char *)&g_FieldRandomIndex1 += 1;
                    sum += pTable->normal[slot] >> 10;
                    if (rnd < sum) {
                        g_FieldActorState.state.eventCmdParam = pTable->normal[slot] & 0x3FF;
                        break;
                    }
                }
            }
        }
    }
}
} /* extern "C" */

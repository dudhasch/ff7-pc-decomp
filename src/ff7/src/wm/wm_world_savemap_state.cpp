// src/ff7/src/wm/wm_world_savemap_state.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Clears the script variable bank and saves and restores the world map state in the
//   savemap. It also covers field transition requests, the countdown timer, party slot sync, the
//   area name and the random encounter counters and odds.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/wm.h"
#include "ff7/enums/wm.h"
#include "ff7/enums/battle.h"
#include "ff7/enums/main.h"
#include <string.h>
#include "ff7/world.h"
#include "ff7/api/wm.h"
#include "lib/api/token.h"
#include "lib/codegen.h"
#include "ff7/api/field.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
#include "ff7/types/field.h"
#include "ff7/field.h"
#include "ff7/types/menu.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)

// clang-format off
// GLOBAL: FF7 0x0096dea0
signed char g_WorldRegionTerrainSets[16][4] = {0, 9, 0, 17, 0, 0, 0, 17, 0, 9, 1, 17, 0, 20, 0, 17, 0, 9, 8, 17, 0, 0, 25, 17, 0, 9, 19, 17, 0, 0, 1, 17, 0, 0, 1, 17, 0, 9, 14, 17, 0, 9, 25, 17, 0, 10, 9, 17, 0, 9, 25, 17, 0, 0, 8, 11, 0, 0, 8, 0, 0, 0, 1, 0};
// GLOBAL: FF7 0x0096dee0
unsigned char g_WorldRegionYuffieChance[16] = {0x00, 0x00, 0x20, 0x00, 0x00, 0x40, 0x00, 0x40, 0xff, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80};
// GLOBAL: FF7 0x00e3a87c
int g_WorldEncounterDangerCounter = 0;
// GLOBAL: FF7 0x00e3a880
int g_WorldCountdownActive = 0;
// GLOBAL: FF7 0x00e3a884
int g_WorldFieldTransitionState = 0;
// GLOBAL: FF7 0x00e3a888
int g_WorldEncounterStepCounter = 0;
// GLOBAL: FF7 0x00e3a88c
int g_WorldFieldTransitionEntryWord = 0;
// GLOBAL: FF7 0x00e3a890
int g_WorldEncountersEnabled = 0;
// GLOBAL: FF7 0x00e3a894
int g_WorldFieldTransitionArg = 0;
// clang-format on

#define gFieldParty (gSavemap.fieldParty)

/*
 * Zeroes all 0x400 bytes of the savemap's script variable bank gSavemap.scriptBanks.
 */
// FUNCTION: FF7 0x00766b70
void __cdecl WorldScript_ClearVariableBank(void)
{
    int i;

    for (i = 0; i < 1024; i++) {
        gSavemap.scriptBanks[i] = 0;
    }
}

/*
 * Which world map to draw, 0 through 8, from gSavemap.storyProgress. Below 0x3E8 there is no
 * world map at all; 0x3E8..0x62B gives 1..2 depending on a flag; from 0x62C the tier is 5..8
 * chosen by whether progress has passed 0x654 and by bit 0x10 of gSavemap.worldMapVariantFlags.
 */
// FUNCTION: FF7 0x00766b9f
int __cdecl World_SelectMapVariantFromStoryProgress(void)
{
    int variant;
    int tier;

    if (gSavemap.storyProgress < 1000) {
        variant = 0;
    } else {
        if (gSavemap.storyProgress < 1580) {
            tier = (gSavemap.tutSub & 1 | (gSavemap.storyProgress >= 1620) << 1) + 1;
        } else {
            tier = (gSavemap.storyProgress >= 0x654 | ((gSavemap.worldMapVariantFlags & 0x10) != 0) << 1) + 5;
        }
        variant = tier;
    }
    return variant;
}

/*
 * Returns whether gSavemap.storyProgress lies from 0x3e8 inclusive through 0x4b0 exclusive.
 */
// FUNCTION: FF7 0x00766c33
int __cdecl World_IsStoryProgressInWindow3E8To4B0(void)
{
    int in;

    if (gSavemap.storyProgress >= 1000 && gSavemap.storyProgress < 1200) {
        in = 1;
    } else {
        in = 0;
    }
    return in;
}

/*
 * Returns gSavemap.worldMapVariant.
 */
// FUNCTION: FF7 0x00766c6e
int __cdecl World_GetSavedMapVariant(void)
{
    return gSavemap.worldMapVariant;
}

/*
 * Sums the last 0x80 bytes of gSavemap.scriptBanks and reports error 2 for a nonzero
 * low-byte checksum, then continues restoring world state. Reads optional
 * statePtr/argPtr dwords, restores camera and option bits, updates party slots,
 * time/state helpers and the actor pool from saved data.
 */
// FUNCTION: FF7 0x00766c7a
void __cdecl World_RestoreStateFromSavemap(int statePtr, int argPtr, int skipSteering)
{
    struct {
        VECTOR polePos;
        int i;
        int cameraMode;
        int dwChecksum;
    } frameSlot; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frameSlot.dwChecksum = 0;
    for (frameSlot.i = 896; frameSlot.i < 1024; frameSlot.i++) {
        frameSlot.dwChecksum += gSavemap.scriptBanks[frameSlot.i];
    }
    if ((frameSlot.dwChecksum & 0xff) != 0) {
        WorldScript_ReportError(2);
    }
    g_WorldFieldTransitionState = statePtr != 0 ? *(int *)statePtr : 0;
    g_WorldFieldTransitionArg = argPtr != 0 ? *(int *)argPtr : 0;
    frameSlot.cameraMode = (gSavemap.worldCameraState >> 12) & 3;
    World_SetCameraMode((frameSlot.cameraMode >= 0 && frameSlot.cameraMode < 3) ? frameSlot.cameraMode : 0);
    if ((g_WorldFieldTransitionState == 0 && g_WorldFieldTransitionArg == 0) || g_WorldFieldTransitionState == 1 || g_WorldFieldTransitionState == 2) {
        World_SetCameraFrontAndRotationY(gSavemap.worldCameraState & 0xfff);
    }
    World_SetMiniMapState((gSavemap.worldCameraState >> 14) & 3);
    WorldMap_UpdatePartySlots((unsigned char *)gSavemap.fieldParty, (unsigned char *)gSavemap.party, 0);
    WorldMap_SeedRandomTable(gSavemap.seconds);
    World_UnpackTerrainCurrentBytes(gSavemap.worldTerrainBytes);
    World_SetTerrainBlockCountdown(gSavemap.worldTerrainCountdown);
    WorldEntity_InitPool((int)gSavemap.lWorld);
    if (skipSteering != 0) {
        gSavemap.menuLockedWord &= 0xfcff;
    } else {
        World_ResetSteeringCycleAndSetSpawnIndex(gSavemap.worldSteeringSpawnIndex);
        if (g_WorldFieldTransitionState == 1) {
            for (frameSlot.i = 0; frameSlot.i < 3; frameSlot.i++) {
                if (gSavemap.polePositions[frameSlot.i] != 0) {
                    WorldEntity_Alloc();
                    WorldEntity_SetupForModel(0x15 + frameSlot.i);
                    frameSlot.polePos.vx = gSavemap.polePositions[frameSlot.i] & 0xffff;
                    frameSlot.polePos.vy = 0;
                    frameSlot.polePos.vz = (gSavemap.polePositions[frameSlot.i] >> 16) & 0xffff;
                    WorldEntity_SetCurrentPositionAndGround(&frameSlot.polePos);
                }
            }
        } else {
            for (frameSlot.i = 0; frameSlot.i < 3; frameSlot.i++) {
                gSavemap.polePositions[frameSlot.i] = 0;
            }
        }
    }
    World_StopCountdownTimer();
}

/*
 * Saves packed camera yaw/mode and camera flags, player model, vehicle slots,
 * map variant and world-state fields into the savemap. Converts model 3 riding
 * a rideable saved model to 0x2B.
 */
// FUNCTION: FF7 0x00766ecb
void __cdecl World_SaveStateToSavemap(void)
{
    struct {
        VECTOR currentPosition;
        int i;
        int dwChecksum;
    } frameSlot; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (World_GetCameraMode() == 0 && World_GetMapVariant() != 3) {
        World_SetCameraFrontAndRotationY(0);
    }
    gSavemap.worldCameraState = (World_GetCameraRotationY() & 0x0fff) |
                                              ((World_GetCameraMode() << 12) & 0x3000) |
                                              ((World_GetMiniMapState() << 14) & 0xc000);
    gSavemap.worldPlayerModelId = WorldEntity_GetPlayerModelId();
    /* Retail caller prototype (unsigned char) -> int preserved for /Od argument promotion on `World_IsRideableModel`. */
    if (gSavemap.worldPlayerModelId == WORLD_MODEL_HIGHWIND && ((int(__cdecl *)(unsigned char))World_IsRideableModel)(World_PeekVehicleModel())) {
        gSavemap.worldPlayerModelId = 0x2b;
    }
    World_SaveAllVehicleSlots();
    gSavemap.worldMapVariant = World_GetMapVariant();
    gSavemap.worldTerrainBytes = World_PackTerrainCurrentBytes();
    gSavemap.worldTerrainCountdown = World_GetTerrainBlockCountdown();
    if (gSavemap.worldMapVariant == 3) {
        gSavemap.worldSteeringSpawnIndex = World_GetSteeringSpawnIndex();
        for (frameSlot.i = 0; frameSlot.i < 3; frameSlot.i++) {
            if (WorldEntity_SetCurrentById(0x15 + frameSlot.i)) {
                WorldEntity_GetCurrentPosition(&frameSlot.currentPosition);
                gSavemap.polePositions[frameSlot.i] =
                    (frameSlot.currentPosition.vx & 0xffff) | (frameSlot.currentPosition.vz << 16);
            } else {
                gSavemap.polePositions[frameSlot.i] = 0;
            }
        }
    }
    frameSlot.dwChecksum = 0;
    for (frameSlot.i = 896; frameSlot.i < 1023; frameSlot.i++) {
        frameSlot.dwChecksum += gSavemap.scriptBanks[frameSlot.i];
    }
    gSavemap.worldChecksum = -frameSlot.dwChecksum;
}

/*
 * Reports the pending field transition: writes g_WorldFieldTransitionState
 * (transition requested) to *stateOut, g_WorldFieldTransitionArg to *argOut and the
 * field entry word g_WorldFieldTransitionEntryWord with bit 29 set when a
 * countdown is running (g_WorldCountdownActive) to *entryOut, each only if the
 * pointer is non-null. Then sets bits 8-9 of gSavemap.menuLockedWord and
 * saves the world state to the savemap.
 */
// FUNCTION: FF7 0x00767039
void __cdecl World_ExportTransitionRequestAndSaveState(int stateOut, int argOut, int entryOut)
{
    if (stateOut != 0) {
        *(int *)stateOut = g_WorldFieldTransitionState;
    }
    if (argOut != 0) {
        *(int *)argOut = g_WorldFieldTransitionArg;
    }
    if (entryOut != 0) {
        *(int *)entryOut = g_WorldFieldTransitionEntryWord | (g_WorldCountdownActive != 0) << 29;
    }
    gSavemap.menuLockedWord |= 0x300;
    World_SaveStateToSavemap();
}

/*
 * For saved model values at least 3, selects and boards that model below 0x2b,
 * or boards model 0x13 followed by model 3 otherwise. Then sets the relative
 * heading using WorldEntity_GetCurrentFacingPlusTurn.
 */
// FUNCTION: FF7 0x00767097
void __cdecl World_BoardSavedVehicleModel(void)
{
    if (gSavemap.worldPlayerModelId >= WORLD_MODEL_HIGHWIND) {
        if (gSavemap.worldPlayerModelId < 0x2B) {
            WorldEntity_SetCurrentById(gSavemap.worldPlayerModelId);
            World_BoardVehicle();
        } else {
            WorldEntity_SetCurrentById(0x13);
            World_BoardVehicle();
            WorldEntity_SetCurrentById(3);
            World_BoardVehicle();
        }
        World_SetPreviousPlayerDirectionFromFacing(WorldEntity_GetCurrentFacingPlusTurn());
    }
}

/*
 * Clears g_WorldFieldTransitionState, saves fieldArg in g_WorldFieldTransitionArg
 * and selects a g_WorldFieldTable entry using the original masked index
 * formula. Copies its event command parameter, tile coordinates, walkmesh id and entry id into
 * g_FieldActorState.state.
 */
// FUNCTION: FF7 0x007670f9
void __cdecl World_SetFieldEntryFromTable(int fieldArg)
{
    WorldFieldTableEntry *entry;

    g_WorldFieldTransitionState = 0;
    g_WorldFieldTransitionArg = fieldArg;
    entry = (WorldFieldTableEntry *)((((fieldArg >> 8) * 2 - 2 & 0x1FE) | (fieldArg & 1)) * 0xC + (char *)&g_WorldFieldTable);
    g_FieldActorState.state.eventCmdParam = entry->eventCmdParam;
    g_FieldActorState.state.pcTileX = entry->pcTileX;
    g_FieldActorState.state.pcTileY = entry->pcTileY;
    g_FieldActorState.state.pcWalkMeshId = entry->pcWalkMeshId;
    g_FieldActorState.state.entryId = entry->entryId;
}

/*
 * Requests a field transition: a value with bit 30 set first selects the field
 * entry 0x2100 from the table. Sets g_WorldFieldTransitionState and stores the
 * entry word in g_WorldFieldTransitionEntryWord.
 */
// FUNCTION: FF7 0x0076717c
void __cdecl World_QueueFieldTransitionRequest(int value)
{
    if ((value & 0x40000000) != 0) {
        World_SetFieldEntryFromTable(0x2100);
    }
    g_WorldFieldTransitionState = 1;
    g_WorldFieldTransitionEntryWord = value;
}

/*
 * Starts the savemap countdown timer at `value` seconds and raises
 * g_WorldCountdownActive and gSavemap.countdownFlags.
 */
// FUNCTION: FF7 0x007671ad
void __cdecl World_StartCountdownTimer(int value)
{
    gSavemap.countdownSeconds = value;
    g_WorldCountdownActive = 1;
    gSavemap.countdownFlags = 1;
}

/*
 * Stops the savemap countdown: clears g_WorldCountdownActive and the savemap
 * flag gSavemap.countdownFlags.
 */
// FUNCTION: FF7 0x007671cb
void __cdecl World_StopCountdownTimer(void)
{
    g_WorldCountdownActive = 0;
    gSavemap.countdownFlags = 0;
}

/*
 * Marks the field transition as a plain return (g_WorldFieldTransitionState =
 * 2) with no entry word and no g_WorldFieldTransitionArg argument.
 */
// FUNCTION: FF7 0x007671e1
void __cdecl World_RequestPlainReturnTransition(void)
{
    g_WorldFieldTransitionState = 2;
    g_WorldFieldTransitionArg = 0;
    g_WorldFieldTransitionEntryWord = 0;
}

/*
 * Returns the field transition request state g_WorldFieldTransitionState (0
 * none, 1 requested with an entry word, 2 plain return).
 */
// FUNCTION: FF7 0x00767204
int __cdecl World_GetTransitionRequestState(void)
{
    return g_WorldFieldTransitionState;
}

/*
 * Returns the field transition argument g_WorldFieldTransitionArg.
 */
// FUNCTION: FF7 0x0076720e
int __cdecl World_GetTransitionArgument(void)
{
    return g_WorldFieldTransitionArg;
}

/*
 * Builds membership masks for three old and three new party slots and
 * optionally updates script change registers. Marks removed old slots 0xFF,
 * then fills available old slots with newly added members in ascending mask-
 * bit order.
 */
// FUNCTION: FF7 0x00767218
void __cdecl WorldMap_UpdatePartySlots(unsigned char *oldParty, unsigned char *newParty, int update)
{
    unsigned diff;
    unsigned i;
    int old;
    int newMask;
    unsigned next;

    old = (1 << oldParty[0]) | (1 << oldParty[1]) | (1 << oldParty[2]);
    newMask = (1 << newParty[0]) | (1 << newParty[1]) | (1 << newParty[2]);
    if (update) {
        WorldScript_SetPartyChangeRegisters(old, newMask);
    }
    diff = old & ~newMask;
    for (i = 0; i < 3; i++) {
        if (diff & (1 << oldParty[i])) {
            oldParty[i] = PARTY_CHAR_NONE;
        }
    }
    diff = ~old & newMask;
    for (i = next = 0; diff; i++, diff >>= 1) {
        if (diff & 1) {
            while (next < 3 && oldParty[next] != PARTY_CHAR_NONE) {
                next++;
            }
            if (next >= 3) {
                break;
            }
            oldParty[next] = i;
            next++;
        }
    }
}

/*
 * Returns zero if any of the three gFieldParty bytes is zero. Otherwise
 * returns 1 when any byte is 2, else 2 when any byte is 8, else zero.
 */
// FUNCTION: FF7 0x0076736e
int __cdecl WorldMap_GetLeaderModelIndex(void)
{
    int innerPick;
    int res;
    int pickMid;

    if (gSavemap.fieldParty[0] != 0 && ((WorldMapPartyRoster *)&gFieldParty)->member1 != 0 && ((WorldMapPartyRoster *)&gFieldParty)->member2 != 0) {
        if (gSavemap.fieldParty[0] != 2 && ((WorldMapPartyRoster *)&gFieldParty)->member1 != 2 && ((WorldMapPartyRoster *)&gFieldParty)->member2 != 2) {
            if (gSavemap.fieldParty[0] != 8 && ((WorldMapPartyRoster *)&gFieldParty)->member1 != 8 && ((WorldMapPartyRoster *)&gFieldParty)->member2 != 8) {
                innerPick = 0;
            } else {
                innerPick = 2;
            }
            pickMid = innerPick;
        } else {
            pickMid = 1;
        }
        res = pickMid;
    } else {
        res = 0;
    }
    return res;
}

/*
 * Updates field party slots from the savemap party with the map-variant
 * condition. For variants other than 2, applies the leader model index to the
 * stacked vehicle base or the player model according to World_IsRidingStacked.
 */
// FUNCTION: FF7 0x0076741c
void __cdecl World_SyncPartyAndLeaderModel(void)
{
    WorldMap_UpdatePartySlots((unsigned char *)gSavemap.fieldParty, (unsigned char *)gSavemap.party, World_GetMapVariant() != 2);
    if (World_GetMapVariant() != 2) {
        (World_IsRidingStacked() ? World_SetVehicleStackBase : WorldEntity_SetPlayerModelId)(WorldMap_GetLeaderModelIndex());
    }
}

/*
 * Copies the encoded area message from World_GetMessage(value) to gSavemap.scriptBanks
 * entries 0x368..0x37F, including and stopping at 0xFF when encountered.
 * No terminator is added if the bound is reached first.
 */
// FUNCTION: FF7 0x00767473
void __cdecl World_StoreAreaNameToSavemap(int value)
{
    struct {
        int bankIndex;
        unsigned char *messageCursor;
    } frameSlot; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frameSlot.messageCursor = World_GetMessage(value);
    frameSlot.bankIndex = 0x368;
    while (frameSlot.bankIndex < 0x380 && (gSavemap.scriptBanks[frameSlot.bankIndex++] = *(frameSlot.messageCursor++)) != 0xff)
        ;
}

/*
 * Stores the chocobo encounter information in gSavemap.worldChocoboEncounterInfo.
 */
// FUNCTION: FF7 0x007674d2
void __cdecl World_SetChocoboEncounterInfo(int value)
{
    gSavemap.worldChocoboEncounterInfo = value;
}

/*
 * Returns gSavemap.worldChocoboEncounterInfo.
 */
// FUNCTION: FF7 0x007674df
int __cdecl World_GetChocoboEncounterInfo(void)
{
    return gSavemap.worldChocoboEncounterInfo;
}

/*
 * Returns whether the battle flags carry FIELD_BATTLE_FLAG_ESCAPED.
 */
// FUNCTION: FF7 0x007674eb
int __cdecl World_GetBattleEscaped(void)
{
    return (g_BattleResultWord & FIELD_BATTLE_FLAG_ESCAPED) != 0;
}

/*
 * Resets the random encounter step counter g_WorldEncounterStepCounter to 0
 * and re-arms the danger counter through World_ReloadEncounterCounterNormal.
 */
// FUNCTION: FF7 0x00767501
void __cdecl World_ResetEncounterStep(void)
{
    g_WorldEncounterStepCounter = 0;
    World_ReloadEncounterCounterNormal();
}

/*
 * Sets the encounter danger counter g_WorldEncounterDangerCounter to -140, the
 * normal reload after an encounter.
 */
// FUNCTION: FF7 0x00767515
void __cdecl World_ReloadEncounterCounterNormal(void)
{
    g_WorldEncounterDangerCounter = 0xFFFFFF74;
}

/*
 * Sets the encounter danger counter g_WorldEncounterDangerCounter to -30, the
 * short reload used when encounters should come sooner.
 */
// FUNCTION: FF7 0x00767524
void __cdecl World_ReloadEncounterCounterShort(void)
{
    g_WorldEncounterDangerCounter = 0xFFFFFFE2;
}

/*
 * Enables or disables random encounters on the world map
 * (g_WorldEncountersEnabled).
 */
// FUNCTION: FF7 0x00767533
void __cdecl World_SetEncountersEnabled(int value)
{
    g_WorldEncountersEnabled = value;
}

/*
 * When encounter state, map variant, terrain effect and player control permit,
 * checks movement in region zero for eligible player models with encounters
 * enabled. Ticks accessory regeneration at movement counter 8/16 and rolls an
 * encounter after the old counter reaches 16, resetting it.
 */
// FUNCTION: FF7 0x00767540
void __cdecl World_UpdateEncounters(void)
{
    struct {
        VECTOR previous_position;
        int terrain_script;
        int encounter_id;
        int map_id;
        VECTOR position;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.map_id = World_GetMapVariant();
    if (*(char *)&g_FieldActorState.state.encountersDisabled == 0 && local.map_id != 2 &&
        World_IsTerrainEffectActive() == 0 && World_IsPlayerControlEnabled() != 0) {
        local.terrain_script = WorldEntity_GetPlayerRegionId();
        WorldEntity_GetPlayerPosition(&local.position);
        WorldEntity_GetPlayerPrevPosition(&local.previous_position);
        if (WorldEntity_IsPlayerModelInMask((1 << 6) | (1 << 2) | (1 << 1) | (1 << 0)) != 0 &&
            g_WorldEncountersEnabled != 0) {
            if (local.terrain_script == 0 &&
                (local.position.vx != local.previous_position.vx ||
                 local.position.vz != local.previous_position.vz)) {
                if (g_WorldEncounterDangerCounter == 8 || g_WorldEncounterDangerCounter == 16) {
                    Party_TickAccessoryRegen();
                }
                if (g_WorldEncounterDangerCounter++ >= 16) {
                    g_WorldEncounterDangerCounter = 0;
                    local.encounter_id = World_RollEncounter();
                    if (local.encounter_id != -1) {
                        WorldEntity_SetPlayerPosition(&local.previous_position);
                        WorldMap_RequestBattle(local.encounter_id);
                    }
                }
            }
        } else {
            g_WorldEncounterDangerCounter = 0;
        }
    }
}

/*
 * Rolls a world encounter from the clamped region and mapped terrain table
 * when encounter-rate scale is nonzero. Accumulates danger by scaled inverse
 * rate or 0x7FFF for zero rate and checks the table enabled bit against a
 * random threshold.
 */
// FUNCTION: FF7 0x00767641
int __cdecl World_RollEncounter(void)
{
    struct {
        int retry;
        int chocoboRow;
        int levelRow;
        int level;
        int encounter;
        int special;
        int terrain;
        int region;
        int chocobo;
        int cumulative;
        int rate;
        int roll;
        int preemptive;
        unsigned short *record;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.preemptive = 0;
    frame.chocobo = 0;
    frame.special = 0;
    frame.encounter = -1;
    frame.terrain = WorldEntity_GetPlayerTerrainType();
    frame.region = WorldEntity_GetPlayerEncounterRegion();
    if (World_GetEncounterRateScale() != 0) {
        if (frame.region < 0)
            frame.region = 0;
        else if (frame.region > 15)
            frame.region = 15;
        if (frame.terrain == WORLD_TERRAIN_HILLSIDE)
            frame.terrain = WORLD_TERRAIN_GRASS;
        if (frame.terrain == WORLD_TERRAIN_GOLD_SAUCER_DESERT)
            frame.terrain = WORLD_TERRAIN_DESERT;
        /* from here frame.terrain is the type's index in the region's set of four */
        if (g_WorldRegionTerrainSets[frame.region][0] == frame.terrain)
            frame.terrain = 0;
        else if (g_WorldRegionTerrainSets[frame.region][1] == frame.terrain)
            frame.terrain = 1;
        else if (g_WorldRegionTerrainSets[frame.region][2] == frame.terrain)
            frame.terrain = 2;
        else if (g_WorldRegionTerrainSets[frame.region][3] == frame.terrain)
            frame.terrain = 3;
        else
            frame.terrain = 0;
        frame.record = (unsigned short *)((char *)&g_WorldEncounterTable[0x50] + ((frame.region << 7) + (frame.terrain << 5)));
        frame.rate = (frame.record[0] >> 8) & 0xff;
        g_WorldEncounterStepCounter += frame.rate ? (World_GetEncounterRateScale() << 10) / frame.rate : 32767;
        if (WorldMap_NextRandomByte() < (g_WorldEncounterStepCounter >> 8) && (frame.record[0] & 1)) {
            if (WorldMap_NextRandomByte() < g_WorldRegionYuffieChance[frame.region] &&
                (WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_FOREST || WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_JUNGLE) &&
                World_IsLevelEncounterEnabled()) {
                frame.level = Savemap_GetCloudLevel(0);
                for (frame.levelRow = 0; frame.levelRow < 8 && frame.level > ((unsigned short(*)[2])g_WorldEncounterTable)[frame.levelRow][0]; frame.levelRow++)
                    ;
                if (frame.levelRow >= 8)
                    frame.levelRow = 7;
                frame.encounter = (((unsigned short(*)[2])g_WorldEncounterTable)[frame.levelRow][1] & 0x3ff) + (WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_JUNGLE);
                frame.special = 1;
            } else {
                g_WorldEncounterStepCounter = 0;
                if (WorldEntity_GetPlayerTerrainTopBit() && World_GetChocoboLure() && WorldEntity_IsPlayerModelInMask(7)) {
                    frame.roll = (WorldMap_NextRandomByte() << 12) / World_GetChocoboLure();
                    frame.cumulative = frame.record[11];
                    if (frame.cumulative > frame.roll)
                        frame.encounter = frame.record[11] & 0x3ff;
                    else if ((frame.cumulative += frame.record[12]) > frame.roll)
                        frame.encounter = frame.record[12] & 0x3ff;
                    else if ((frame.cumulative += frame.record[13]) > frame.roll)
                        frame.encounter = frame.record[13] & 0x3ff;
                    else if ((frame.cumulative += frame.record[14]) > frame.roll)
                        frame.encounter = frame.record[14] & 0x3ff;
                    for (frame.chocoboRow = 0; frame.chocoboRow < 32 && ((unsigned short(*)[2]) & g_WorldEncounterTable[0x10])[frame.chocoboRow][0] != frame.encounter; frame.chocoboRow++)
                        ;
                    if (frame.chocoboRow < 32)
                        World_SetChocoboEncounterInfo(((unsigned short(*)[2]) & g_WorldEncounterTable[0x10])[frame.chocoboRow][1]);
                    frame.chocobo = frame.encounter != -1;
                }
                frame.preemptive = WorldMap_NextRandomByte() < World_GetPreemptiveRate();
                if (frame.preemptive == 0) {
                    if (World_IsEncounterSlot78Enabled() && frame.encounter < 0) {
                        frame.roll = WorldMap_NextRandomByte() << (World_IsPreemptiveMastered() + 8);
                        frame.cumulative = frame.record[7];
                        if (frame.cumulative > frame.roll)
                            frame.encounter = frame.record[7] & 0x3ff;
                        else {
                            frame.cumulative += frame.record[8];
                            if (frame.cumulative > frame.roll)
                                frame.encounter = frame.record[8] & 0x3ff;
                        }
                    }
                    if (World_PartyHasMultipleMembers() && frame.encounter < 0) {
                        frame.roll = WorldMap_NextRandomByte() << 8;
                        frame.cumulative = frame.record[9];
                        if (frame.cumulative > frame.roll)
                            frame.encounter = frame.record[9] & 0x3ff;
                    }
                    if (World_IsEncounterSlot10Enabled() && frame.encounter < 0) {
                        frame.roll = WorldMap_NextRandomByte() << (World_IsPreemptiveMastered() + 8);
                        frame.cumulative = frame.record[10];
                        if (frame.cumulative > frame.roll)
                            frame.encounter = frame.record[10] & 0x3ff;
                    }
                }
                if (frame.encounter < 0) {
                    frame.retry = 0;
                    do {
                        frame.roll = WorldMap_NextRandomByte() << 8;
                        frame.cumulative = frame.record[1];
                        if (frame.cumulative > frame.roll)
                            frame.encounter = frame.record[1] & 0x3ff;
                        else if ((frame.cumulative += frame.record[2]) > frame.roll)
                            frame.encounter = frame.record[2] & 0x3ff;
                        else if ((frame.cumulative += frame.record[3]) > frame.roll)
                            frame.encounter = frame.record[3] & 0x3ff;
                        else if ((frame.cumulative += frame.record[4]) > frame.roll)
                            frame.encounter = frame.record[4] & 0x3ff;
                        else if ((frame.cumulative += frame.record[5]) > frame.roll)
                            frame.encounter = frame.record[5] & 0x3ff;
                        else if ((frame.cumulative += frame.record[6]) > frame.roll)
                            frame.encounter = frame.record[6] & 0x3ff;
                    } while (frame.retry++ < 1 && frame.encounter == gSavemap.worldLastEncounter);
                }
            }
        }
    }
    if (frame.encounter != -1) {
        gSavemap.worldLastEncounter = frame.encounter;
        World_SetMusicChangeGate(1);
        World_SwitchMusicTrack(frame.chocobo ? 5 : 4);
    }
    return frame.encounter | (frame.preemptive << 31) | (frame.special << 30);
}

/*
 * Returns the party's encounter rate (g_PartyBattleModifiers.encounterRate), doubled when it
 * exceeds 16.
 */
// FUNCTION: FF7 0x00767c55
int __cdecl World_GetEncounterRateScale(void)
{
    return g_PartyBattleModifiers.encounterRate * ((g_PartyBattleModifiers.encounterRate > 16) + 1);
}

/*
 * Returns the party's pre-emptive odds (the low seven bits of
 * g_PartyBattleModifiers.preemptiveOdds), doubled when they exceed 16.
 */
// FUNCTION: FF7 0x00767c77
int __cdecl World_GetPreemptiveRate(void)
{
    int rate;

    rate = g_PartyBattleModifiers.preemptiveOdds & PARTY_PREEMPTIVE_ODDS_MASK;
    return rate * ((rate > 16) + 1);
}

/*
 * Returns whether the party has PARTY_PREEMPTIVE_MASTERED (g_PartyBattleModifiers.preemptiveOdds):
 * the encounter roll shifts one bit further, which halves the odds of rows 7, 8 and 10.
 */
// FUNCTION: FF7 0x00767c9e
int __cdecl World_IsPreemptiveMastered(void)
{
    return (g_PartyBattleModifiers.preemptiveOdds & PARTY_PREEMPTIVE_MASTERED) != 0;
}

/*
 * Returns the party's chocobo lure (g_PartyBattleModifiers.chocoboLure): 0 means no chocobo
 * formations.
 */
// FUNCTION: FF7 0x00767cb5
int __cdecl World_GetChocoboLure(void)
{
    return g_PartyBattleModifiers.chocoboLure;
}

/*
 * Returns bit 0 of gSavemap.yuffieForest: whether encounters are
 * enabled for the current level.
 */
// FUNCTION: FF7 0x00767cc1
int __cdecl World_IsLevelEncounterEnabled(void)
{
    return gSavemap.yuffieForest & 1;
}

/*
 * Always 1: the shipped build enables encounter slots 7 and 8 unconditionally.
 * Returns the constant 1; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x00767cd0
int __cdecl World_IsEncounterSlot78Enabled(void)
{
    return 1;
}

/*
 * Counts the party members that are present -- 0xFF is an empty slot -- and
 * returns whether there is more than one.
 */
// FUNCTION: FF7 0x00767cda
int __cdecl World_PartyHasMultipleMembers(void)
{
    int count;
    int i;

    count = 0;
    for (i = 0; i < 3; i++) {
        count += gSavemap.party[i] != PARTY_CHAR_NONE;
    }
    return count > 1;
}

/*
 * Always 1: the shipped build enables encounter slot 10 unconditionally.
 */
// FUNCTION: FF7 0x00767d2c
int __cdecl World_IsEncounterSlot10Enabled(void)
{
    return 1;
}
} /* extern "C" */

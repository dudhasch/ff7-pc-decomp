// src/ff7/src/wm/wm_world_vehicle_stack.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Keeps the world map's vehicle model stack and the boarding and disembarking transition
//   for vehicles. It also pushes the player off obstacles and checks whether the player can exit a
//   vehicle at the current terrain.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/wm.h"
#include <string.h>
#include "ff7/world.h"
#include "ff7/types/wm.h"
#include "ff7/api/wm.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "lib/codegen.h"
#include "lib/enums/file.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)

// clang-format off
// GLOBAL: FF7 0x00e3a860
unsigned char g_WorldVehicleModelStackBase = 0;
unsigned char g_bssPad_00e3a864[4] = {0};
// GLOBAL: FF7 0x00e3a868
unsigned char * g_pWorldVehicleModelStackTop = 0;
// GLOBAL: FF7 0x00e3a86c
int g_WorldVehicleDisembarkState = 0;
// GLOBAL: FF7 0x00e3a870
int g_WorldVehicleHeldCancelFrames = 0;
// GLOBAL: FF7 0x00e3a874
int g_WorldVehicleTransitionTimer = 0;
// GLOBAL: FF7 0x00e3a878
int g_WorldVehicleTransitionArgument = 0;
// clang-format on

/*
 * Resets the vehicle stack: empties it (top g_pWorldVehicleModelStackTop back to the base g_WorldVehicleModelStackBase) and clears
 * the transition state g_WorldVehicleDisembarkState, g_WorldVehicleHeldCancelFrames, g_WorldVehicleTransitionTimer and g_WorldVehicleTransitionArgument.
 */
// FUNCTION: FF7 0x007663e0
void __cdecl World_ResetVehicleState(void)
{
    g_WorldVehicleDisembarkState = 0;
    g_WorldVehicleTransitionTimer = 0;
    g_pWorldVehicleModelStackTop = &g_WorldVehicleModelStackBase;
    g_WorldVehicleTransitionArgument = 0;
    g_WorldVehicleHeldCancelFrames = 0;
}

/*
 * Reverts player position and runs the movement-state helper, then moves by a rotated short
 * vector. Models 5 and 0xD use the supplied nonzero distance or choose 100/800 according to
 * scripted altitude and current facing; other models use distance 300 with facing plus a quarter
 * turn.
 */
// FUNCTION: FF7 0x00766417
void __cdecl World_PushPlayerOffObstacle(int distance)
{
    SVECTOR moveVec;

    WorldEntity_RevertPlayerPosition();
    WorldEntity_SelectPlayerAsCurrentActor();
    moveVec.vx = moveVec.vy = 0;
    switch (WorldEntity_GetPlayerModelId()) {
    case WORLD_MODEL_TINY_BRONCO:
    case WORLD_MODEL_SUBMARINE:
        moveVec.vz = distance ? distance : WorldEntity_IsPlayerAltitudeScripted() ? 100
                                                                          : 800;
        WorldMap_RotateVec3ByYaw(&moveVec, WorldEntity_GetPlayerFacingPlusTurn());
        break;
    default:
        moveVec.vz = 300;
        WorldMap_RotateVec3ByYaw(&moveVec, WorldEntity_GetPlayerFacingPlusTurn() + 0x400);
        break;
    }
    WorldEntity_MoveCurrentBy(moveVec.vx, moveVec.vz);
}

/*
 * The stack is the eight bytes at g_WorldVehicleModelStackBase and its pointer is the word
 * immediately above it, so the full test is `sp < &sp`: the pointer variable doubles as the limit.
 */
// FUNCTION: FF7 0x007664c2
void __cdecl World_PushVehicleModel(char value)
{
    if (g_pWorldVehicleModelStackTop < (unsigned char *)&g_pWorldVehicleModelStackTop) {
        *g_pWorldVehicleModelStackTop = value;
        g_pWorldVehicleModelStackTop = g_pWorldVehicleModelStackTop + 1;
    }
}

/*
 * Pops the top vehicle model id off the stack at g_WorldVehicleModelStackBase, or returns 0 when it is empty.
 */
// FUNCTION: FF7 0x007664ec
unsigned char __cdecl World_PopVehicleModel(void)
{
    int top;

    if (g_pWorldVehicleModelStackTop > &g_WorldVehicleModelStackBase) {
        g_pWorldVehicleModelStackTop = g_pWorldVehicleModelStackTop - 1;
        top = *g_pWorldVehicleModelStackTop;
    } else {
        top = 0;
    }
    return (unsigned char)top;
}

/*
 * Returns the top vehicle model id without popping it, or 0 when the stack is empty.
 */
// FUNCTION: FF7 0x00766526
unsigned char __cdecl World_PeekVehicleModel(void)
{
    int top;

    if (g_pWorldVehicleModelStackTop > &g_WorldVehicleModelStackBase) {
        top = g_pWorldVehicleModelStackTop[-1];
    } else {
        top = 0;
    }
    return (unsigned char)top;
}

/*
 * Writes the vehicle stack's base slot g_WorldVehicleModelStackBase directly.
 */
// FUNCTION: FF7 0x00766553
void __cdecl World_SetVehicleStackBase(int value)
{
    g_WorldVehicleModelStackBase = value;
}

/*
 * Returns whether the unsigned vehicle-stack pointer lies above the base address g_WorldVehicleModelStackBase.
 */
// FUNCTION: FF7 0x00766560
int __cdecl World_IsRidingStacked(void)
{
    return (unsigned int)&g_WorldVehicleModelStackBase < (unsigned int)g_pWorldVehicleModelStackTop;
}

/*
 * For model 5 with g_WorldVehicleTransitionArgument set, enables the two vehicle helpers and returns. Otherwise
 * proceeds only when boarding eligibility succeeds, updates stream focus and saves the vehicle
 * model; a rideable actor takes its own setup path and returns. Remaining actors reset/control
 * state, then apply model-3 vehicle/animation helpers or the model-6 fixed 0x1EC helper.
 */
// FUNCTION: FF7 0x00766574
void __cdecl World_BoardVehicle(void)
{
    struct {
        VECTOR position;
        int modelType;
    } locals;

    locals.modelType = WorldEntity_GetPlayerModelId();
    if (locals.modelType == WORLD_MODEL_TINY_BRONCO && g_WorldVehicleTransitionArgument != 0) {
        WorldEntity_SetPlayerAltitudeScripted(1);
        World_SetCameraBlendDirection(1);
        return;
    }
    if (WorldEntity_IsCurrentNonPlayerRequestable() != 0) {
        WorldEntity_GetCurrentPosition(&locals.position);
        World_UpdateStreamFocus((WorldStreamFocusData *)&locals.position, WorldEntity_GetCurrentModelId() == WORLD_MODEL_HIGHWIND ? -1 : 1);
        World_PushVehicleModel(WorldEntity_GetPlayerModelId());
        if (WorldEntity_IsCurrentRideable() != 0) {
            WorldEntity_AttachPlayerToCurrentActor();
            World_SwitchMusicTrack(2);
            return;
        }
        WorldEntity_FreePlayerRiding();
        WorldEntity_FreePlayer();
        WorldEntity_SetPlayerControlled();
        switch (WorldEntity_GetPlayerModelId()) {
        case WORLD_MODEL_HIGHWIND:
            WorldEntity_SetPlayerAltitudeScripted(1);
            World_SetCameraBlendDirection(1);
            World_SpawnPlayerEffectNode7Sized16();
            if (World_GetMusicTrack() < 6) {
                World_SwitchMusicTrack(World_IsStoryProgressInWindow3E8To4B0() == 0 ? 3 : 1);
            }
            break;
        case WORLD_MODEL_BUGGY:
            World_RequestAmbienceSfx(0x1ec);
            break;
        }
    }
}

/*
 * When vehicle exit is not already armed, tests exit eligibility and either runs the descent
 * helper for model 3 or scripted-altitude model 5, or changes player-control mode. Arms exit and
 * runs the additional helper for models 3/4 when eligible. Independently requests system script 9
 * on terrain 0x1B within the unarmed branch.
 */
// FUNCTION: FF7 0x0076667c
void __cdecl World_ArmVehicleExit(void)
{
    int dwModelType;

    dwModelType = WorldEntity_GetPlayerModelId();
    if (g_WorldVehicleDisembarkState == 0) {
        if (World_CanExitVehicleHere()) {
            if (dwModelType == WORLD_MODEL_HIGHWIND || (dwModelType == WORLD_MODEL_TINY_BRONCO && WorldEntity_IsPlayerAltitudeScripted()))
                World_SetCameraBlendDirection(-1);
            else
                WorldMap_SetPlayerControl(0, 2);
            g_WorldVehicleDisembarkState = 1;
            if (dwModelType == WORLD_MODEL_HIGHWIND || dwModelType == WORLD_MODEL_CHOCOBO_4)
                WorldEntity_SetPlayerRequestsBlocked(1);
        }
        if (WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_NORTHERN_CRATER)
            WorldScript_RequestSystem(9);
    }
}

/*
 * Tests terrain eligibility for the player model. The mid-speed model predicate selects mask
 * 0x221B0F03; otherwise model 3 requires terrain zero, model 6 selects mask 0x221B0F83, model 5
 * selects mask 0x70, model 0xD always permits and other models reject. Mask cases return the
 * selected shifted bit.
 */
// FUNCTION: FF7 0x007666ff
int __cdecl World_CanExitVehicleHere(void)
{
    int terrain;

    terrain = WorldEntity_GetPlayerTerrainType();
    if (WorldEntity_IsPlayerRideable() != 0) {
        return WORLD_TERRAIN_MASK_EXIT_LAND >> terrain & 1;
    } else {
        switch (WorldEntity_GetPlayerModelId()) {
        case WORLD_MODEL_HIGHWIND:
            return terrain == WORLD_TERRAIN_GRASS;
        case WORLD_MODEL_BUGGY:
            return WORLD_TERRAIN_MASK_EXIT_BUGGY >> terrain & 1;
        case WORLD_MODEL_TINY_BRONCO:
            return WORLD_TERRAIN_MASK_SHALLOW_WATER >> terrain & 1;
        case WORLD_MODEL_SUBMARINE:
            return 1;
        default:
            return 0;
        }
    }
}

/*
 * Arms the vehicle exit: sets the transition state g_WorldVehicleDisembarkState to 1.
 */
// FUNCTION: FF7 0x007667a3
void __cdecl World_SetVehicleExitArmed(void)
{
    g_WorldVehicleDisembarkState = 1;
}

/*
 * Counts down the vehicle-transition cooldown or, when world state permits, polls held input and
 * starts boarding or arms exit. Advances the exit state through two phases, comparing current and
 * prior player X/Z. On movement, restores prior position, requests the entity script and either
 * transfers control to a rideable entity or allocates and sets up the popped vehicle models, then
 * updates return position or stream focus. Boat, airship and high-ID entities have additional mode
 * handling. With unchanged X/Z, restores selected vehicle flags. Tracks consecutive held cancel
 * input only in the active branch.
 */
// FUNCTION: FF7 0x007667b2
void __cdecl World_UpdateVehicleTransition(int action)
{
    struct {
        int lowerModel;
        int model;
        short heading;
        short pad;
        int entity;
        VECTOR previous;
        int isBoat;
        int held;
        VECTOR position;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.isBoat = WorldEntity_GetPlayerModelId() == WORLD_MODEL_HIGHWIND;
    if (g_WorldVehicleTransitionTimer) {
        g_WorldVehicleTransitionTimer--;
    } else if (World_GetCameraBlendDirection() == 0 && World_GetMapVariant() != 3) {
        frame.held = Input_GetHeldRaw();
        if (g_WorldVehicleDisembarkState == 0 && action == 1) {
            World_BoardVehicle();
        } else if (World_IsPlayerControlEnabled() &&
                   !(frame.held & (WorldEntity_IsPlayerRideable() ? 0xf040 : 0x40)) &&
                   g_WorldVehicleHeldCancelFrames > 0 && g_WorldVehicleHeldCancelFrames < 15 &&
                   (!WorldEntity_IsPlayerModelInMask(0x2000) || WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_SUB_PEN)) {
            World_ArmVehicleExit();
        } else if (g_WorldVehicleDisembarkState == 1) {
            g_WorldVehicleDisembarkState = 2;
        } else if (g_WorldVehicleDisembarkState == 2) {
            g_WorldVehicleDisembarkState = 0;
            WorldEntity_SelectPlayerAsCurrentActor();
            frame.entity = WorldEntity_GetCurrentModelId();
            WorldEntity_GetCurrentPosition(&frame.position);
            WorldEntity_GetCurrentPrevPosition(&frame.previous);
            WorldMap_SetPlayerControl(1, 2);
            if (frame.position.vx != frame.previous.vx || frame.position.vz != frame.previous.vz) {
                World_ReloadEncounterCounterShort();
                if (frame.isBoat) {
                    WorldEntity_SetPlayerAltitudeScripted(0);
                    WorldEntity_SetPlayerRequestsBlocked(0);
                }
                WorldEntity_RevertCurrentPosition();
                World_SetCameraRotationZ(0);
                if (frame.entity == WORLD_MODEL_TINY_BRONCO && WorldEntity_IsPlayerAltitudeScripted()) {
                    WorldEntity_SetPlayerAltitudeScripted(0);
                } else {
                    if (frame.entity == WORLD_MODEL_CHOCOBO_4)
                        WorldEntity_SetPlayerRequestsBlocked(0);
                    WorldScript_RequestOnEntity(frame.entity, 5);
                    if (frame.entity == WORLD_MODEL_CHOCOBO_4)
                        WorldEntity_SetPlayerRequestsBlocked(1);
                    if (WorldEntity_IsCurrentRideable()) {
                        frame.heading = WorldMap_AngleBetweenPositions(&frame.previous, &frame.position) - 0x400;
                        WorldEntity_SetCurrentFacingClearTurn(frame.heading);
                        World_PopVehicleModel();
                        WorldEntity_DetachRiderAsCurrent();
                        if (frame.entity < 41)
                            WorldEntity_SetPlayerControlled();
                        WorldEntity_SetCurrentAnimId(2);
                        WorldEntity_SetCurrentFacingClearTurn(frame.heading);
                        World_SwitchMusicTrack(1);
                        WorldMap_SetPlayerControl(0, 6);
                        if (frame.entity == WORLD_MODEL_CHOCOBO_4) {
                            World_RequestTerrainFloodFill();
                        } else if (frame.entity >= 41) {
                            WorldEntity_SetCurrentById(WorldMap_GetLeaderModelIndex());
                            WorldEntity_SetPlayerControlled();
                        }
                        World_SetVehicleReturnPosition(&frame.position);
                    } else {
                        WorldEntity_Alloc();
                        frame.model = World_PopVehicleModel();
                        /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `World_IsRideableModel`. */
                        if (((int(__cdecl *)(char))World_IsRideableModel)(frame.model)) {
                            frame.lowerModel = World_PeekVehicleModel();
                            WorldEntity_SetupForModel(frame.lowerModel);
                            WorldEntity_SetPlayerControlled();
                            WorldEntity_Alloc();
                            WorldEntity_SetupForModel(frame.model);
                            WorldEntity_AttachPlayerToCurrentActor();
                        } else {
                            WorldEntity_SetupForModel(frame.model);
                            WorldEntity_SetPlayerControlled();
                            if (frame.isBoat)
                                World_RestoreMiniMapRequestedMode();
                        }
                        WorldEntity_SetPlayerPositionAndGround(&frame.position);
                        World_SwitchMusicTrack(WorldEntity_IsPlayerRideable() ? 2 : 1);
                        if (frame.entity == WORLD_MODEL_BUGGY) {
                            World_RequestAmbienceSfx(-492);
                        } else if (frame.entity == WORLD_MODEL_TINY_BRONCO) {
                            World_RequestAmbienceSfx(-493);
                        }
                        World_UpdateStreamFocus((WorldStreamFocusData *)&frame.position, 1);
                    }
                }
            } else {
                if (WorldEntity_GetPlayerModelId() == WORLD_MODEL_HIGHWIND || WorldEntity_GetPlayerModelId() == WORLD_MODEL_CHOCOBO_4)
                    WorldEntity_SetPlayerRequestsBlocked(0);
                if (frame.isBoat || WorldEntity_GetPlayerModelId() == WORLD_MODEL_TINY_BRONCO && WorldEntity_IsPlayerAltitudeScripted())
                    World_SetCameraBlendDirection(1);
            }
        }
        g_WorldVehicleHeldCancelFrames = frame.held & INPUT_CROSS ? g_WorldVehicleHeldCancelFrames + 1 : 0;
    }
}

/*
 * Returns the vehicle transition state g_WorldVehicleDisembarkState.
 */
// FUNCTION: FF7 0x00766b53
int __cdecl World_GetVehicleTransitionState(void)
{
    return g_WorldVehicleDisembarkState;
}

/*
 * Sets g_WorldVehicleTransitionArgument, part of the vehicle transition state.
 */
// FUNCTION: FF7 0x00766b5d
void __cdecl World_SetVehicleTransitionArgument(int value)
{
    g_WorldVehicleTransitionArgument = value;
}
} /* extern "C" */

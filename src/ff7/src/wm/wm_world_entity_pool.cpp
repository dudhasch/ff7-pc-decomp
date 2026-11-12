// src/ff7/src/wm/wm_world_entity_pool.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The world actor pool and entity handling: allocating and freeing actor records, linking
//   them into the active list, riding and detaching passengers, selecting the current and player
//   actors, facing and altitude, and model id checks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/world.h"
#include "ff7/types/wm.h"
#include "ff7/enums/wm.h"
#include "ff7/api/wm.h"
#include "lib/api/sound.h"
#include "lib/api/token.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)
struct WorldPosition;

struct WorldActor;

// clang-format off
// GLOBAL: FF7 0x0096ddb0
unsigned char g_WorldActorCollisionMasks[208] = {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x7e, 0x7e, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x7e, 0x7e, 0x3c, 0x18, 0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00};
// GLOBAL: FF7 0x0096de80
char g_WorldModelToVehicleSlotTable[32] = {0x00, 0x00, 0x00, 0x03, 0x01, 0x02, 0x03, 0x09, 0x09, 0x09, 0x05, 0x05, 0x09, 0x04, 0x09, 0x09, 0x09, 0x09, 0x09, 0x02, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x05, 0x09, 0x09};
// GLOBAL: FF7 0x00e399b8
struct WorldScriptDirectoryEntry *g_pWorldScriptBase = 0;
unsigned char g_bssPad_00e399bc[4] = {0};
// GLOBAL: FF7 0x00e399c0
unsigned char g_WorldScriptAuxStorage[0x40] = {0};
// GLOBAL: FF7 0x00e39a00
WorldActor * g_pWorldActiveActors = 0;
unsigned char g_bssPad_00e39a04[4] = {0};
// GLOBAL: FF7 0x00e39a08
VECTOR g_WorldVehicleReturnPosition = {0};
// GLOBAL: FF7 0x00e39a18
struct WorldActor g_WorldSystemActor = {0};
// GLOBAL: FF7 0x00e39ad8
struct WorldActor *g_pWorldCurrentActor = 0;
// GLOBAL: FF7 0x00e39adc
WorldScriptOperand *g_WorldScriptStackCursor = 0;
// 0xC0-byte spare WorldActor split by the interior public marker at +0x50 (0x00e39b30, actorType).
// GLOBAL: FF7 0x00e39ae0
int g_WorldVehicleActor = 0;
unsigned char g_bssPad_00e39ae4[4] = {0};
unsigned char g_bssPad_00e39ae8[72] = {0};
// GLOBAL: FF7 0x00e39b30
char g_WorldVehicleActorType = 0;
unsigned char g_bssPad_00e39b34[4] = {0};
unsigned char g_bssPad_00e39b38[104] = {0};
// g_WorldVehicleSlotStack is indexed by the code as an array (read by
// WorldScript_SetPartyChangeRegisters). The span runs to the next named definition of the TU; the
// TU's pieces inside 0xe39ba0..0xe39ba8 are aliases.
// GLOBAL: FF7 0x00e39ba0
struct WorldVehicleSlotStack g_WorldVehicleSlotStack = {0};
// g_WorldVehicleModelStack is indexed by the code as an array (read by
// WorldScript_SetPartyChangeRegisters). The span runs to the next named definition of the TU; the
// TU's pieces inside 0xe39ba8..0xe39bb0 are aliases.
// GLOBAL: FF7 0x00e39ba8
struct WorldVehicleModelStack g_WorldVehicleModelStack = {0};
// GLOBAL: FF7 0x00e39bb0
int g_WorldPlayerGroundOffset = 0;
// GLOBAL: FF7 0x00e39bb4
short g_WorldVehicleSavedFacing = 0;
// GLOBAL: FF7 0x00e39bb8
short g_WorldVehicleSavedAltFacing = 0;
// GLOBAL: FF7 0x00e39bbc
short g_WorldVehicleTransitionFlags = 0;
// GLOBAL: FF7 0x00e39bc0
int g_WorldWaitFramesZero = 0;
unsigned char g_bssPad_00e39bc4[4] = {0};
// WorldEntity_InitPool threads 16 WorldActor records into the free list. This object holds the
// first 15; the 16th lies in the padding that follows it, where retail keeps it.
// GLOBAL: FF7 0x00e39bc8
WorldActor g_WorldActorPool[15] = {0};
unsigned char g_bssPad_00e3a708[192] = {0};
// GLOBAL: FF7 0x00e3a7c8
WorldModelRecord *g_pWorldDeactivatedModelRecord = 0;
// GLOBAL: FF7 0x00e3a7cc
struct WorldActor *g_pWorldExecutingActor = 0;
// GLOBAL: FF7 0x00e3a7d0
struct WorldActor *g_pWorldPlayerActor = 0;
// GLOBAL: FF7 0x00e3a7d4
WorldActor *g_pWorldActorFreeList = 0;
// GLOBAL: FF7 0x00e3a7d8
WorldVehicleSlot *g_pWorldVehicleSlotTable = 0;
// GLOBAL: FF7 0x00e3a7dc
int g_WorldVehicleBoardedFlag = 0;
// GLOBAL: FF7 0x00e3a7e0
unsigned char * g_pWorldScriptBank0 = 0;
// GLOBAL: FF7 0x00e3a7e4
unsigned char * g_pWorldScriptBank1 = 0;
// GLOBAL: FF7 0x00e3a7e8
unsigned char * g_pWorldScriptBank2 = 0;
// GLOBAL: FF7 0x00e3a7ec
unsigned char * g_pWorldScriptBank3 = 0;
// The world script's operand stack: four WorldScriptOperand cells up to g_pWorldScriptCode.
// WorldScript_Init and the clear opcode point g_WorldScriptStackCursor at its first cell, a push
// writes a cell and steps on (no capacity check), the pops report an underflow below the first.
// GLOBAL: FF7 0x00e3a7f0
WorldScriptOperand g_WorldScriptOperandStack[4] = {0};
// The world script's code: WorldScript_Init points it at base + 0x400; the interpreter fetches
// each opcode as g_pWorldScriptCode[scriptIdx].
// GLOBAL: FF7 0x00e3a810
unsigned short *g_pWorldScriptCode = 0;
unsigned char g_bssPad_00e3a814[4] = {0};
// The three positions WORLD_SCRIPT_SAVE_POSITION stores, which WORLD_SCRIPT_SAVED_POSITION_DISTANCE
// and WORLD_SCRIPT_FACE_SAVED_POSITION read; WorldScript_Init clears all three.
// GLOBAL: FF7 0x00e3a818
VECTOR g_WorldScriptSavedPositions[3] = {0};
// GLOBAL: FF7 0x00e3a848
int g_WorldMeshTriggerArg0 = 0;
// GLOBAL: FF7 0x00e3a84c
int g_WorldMeshTriggerArg1 = 0;
// GLOBAL: FF7 0x00e3a850
int g_WorldPlayerStepY = 0;
// GLOBAL: FF7 0x00e3a854
int g_WorldPlayerStepCountdown = 0;
// GLOBAL: FF7 0x00e3a858
int g_WorldVehicleTransitionState = 0;
// GLOBAL: FF7 0x00e3a85c
short g_WorldMeshTriggerCounter = 0;
// clang-format on
// The TU's own view of moved definitions declared with another type.

/*
 * Threads 0xC0-byte world actor records 0 through 15 into a free list at g_WorldActorPool: the loop
 * links the first 15 and writes a null link into record 15. Clears player/current/live pointers,
 * clears system and spare entities and sets the spare type byte to 9. Resets camera offsets and
 * return bookkeeping, stores slotTable in g_pWorldVehicleSlotTable, and clears g_WorldPlayerGroundOffset.
 */
// FUNCTION: FF7 0x00760fb0
void __cdecl WorldEntity_InitPool(int slotTable)
{
    int index;

    for (index = 0; index < 15; index++) {
        g_WorldActorPool[index].next = &g_WorldActorPool[index + 1];
    }
    g_WorldActorPool[index].next = 0;
    g_pWorldActorFreeList = g_WorldActorPool;
    g_pWorldPlayerActor = 0;
    g_pWorldCurrentActor = g_pWorldPlayerActor;
    g_pWorldActiveActors = g_pWorldCurrentActor;
    WorldEntity_Clear(&g_WorldSystemActor);
    WorldEntity_Clear((WorldActor *)&g_WorldVehicleActor);
    g_WorldVehicleActorType = 9;
    g_WorldSystemActor.next = 0;
    g_WorldVehicleActor = 0;
    World_SetCameraAngleOffsets(0, 0, 0);
    g_pWorldVehicleSlotTable = (WorldVehicleSlot *)slotTable;
    g_WorldMeshTriggerArg1 = 0;
    g_WorldMeshTriggerArg0 = g_WorldMeshTriggerArg1;
    g_WorldPlayerGroundOffset = 0;
    g_WorldPlayerStepCountdown = g_WorldPlayerGroundOffset;
    g_WorldPlayerStepY = g_WorldPlayerStepCountdown;
}

/*
 * Pop an actor off the free list at g_pWorldActorFreeList and push it onto the head of
 * the active list at g_pWorldActiveActors, leaving it as the current actor.
 *
 * The exhausted case is a bug in the shipped game and is reproduced here: when
 * the free list is empty the error is reported and then execution falls
 * straight through into `ent->next`, dereferencing the null. There is no
 * early return in the image -- the `je` skips only the call.
 */
// FUNCTION: FF7 0x007610b3
WorldActor *__cdecl WorldEntity_Alloc(void)
{
    WorldActor *ent;

    ent = g_pWorldActorFreeList;
    if (ent == 0) {
        WorldScript_ReportError(0x32);
    }
    g_pWorldActorFreeList = ent->next;
    ent->next = g_pWorldActiveActors;
    g_pWorldCurrentActor = ent;
    g_pWorldActiveActors = g_pWorldCurrentActor;
    WorldEntity_Clear(ent);
    return ent;
}

/*
 * Clears the per-frame effect byte on every live WorldActor, walking the list from g_pWorldActiveActors
 * through ->next.
 */
// FUNCTION: FF7 0x0076110b
void __cdecl WorldEntity_ClearPerFrameFlags(void)
{
    WorldActor *actor;

    for (actor = g_pWorldActiveActors; actor != 0; actor = actor->next) {
        actor->effect = 0;
    }
}

/*
 * Clears everything after the next link, a dword at a time. The increment sits at
 * the end of the body and the test at the top, which is a `while`;
 * a `for` header would have put the increment before the test.
 */
// FUNCTION: FF7 0x00761134
void __cdecl WorldEntity_Clear(WorldActor *actor)
{
    int *end;
    int *p;

    actor->effect = 0;
    end = (int *)((char *)actor + 0xc0);
    p = (int *)((char *)actor + 4);
    while (p < end) {
        *p = 0;
        p = p + 1;
    }
}

/*
 * Walks the active actor list while each actorType is below 0x20. Returns whether it encountered
 * an actor with actorType at least 0x20 before reaching the null link.
 */
// FUNCTION: FF7 0x00761176
int __cdecl WorldEntity_HasActorOfTypeAtLeast32(void)
{
    WorldActor *actor;

    for (actor = g_pWorldActiveActors; actor != 0 && actor->actorType < 0x20;
         actor = actor->next) {
    }
    return actor != 0;
}

/*
 * Attach the player actor to the actor that was just allocated, by unlinking
 * it from wherever it is and storing it in the new actor's riding slot -- a
 * rider, or a model carried by a vehicle. The player pointer then follows.
 *
 * Three tests reach the same address, so they are one `&&` chain and not
 * nested `if`s: a nested spelling would have produced three distinct join
 * points.
 */
// FUNCTION: FF7 0x007611ae
void __cdecl WorldEntity_AttachPlayerToCurrentActor(void)
{
    WorldActor *ent;

    if (g_pWorldPlayerActor != 0 && g_pWorldCurrentActor != 0 && g_pWorldCurrentActor->riding == 0) {
        ent = g_pWorldPlayerActor;
        WorldEntity_Unlink(ent);
        g_pWorldCurrentActor->riding = ent;
        g_pWorldPlayerActor = g_pWorldCurrentActor;
    }
}

/*
 * Clears current/player references equal to the supplied actor and removes all collision/riding
 * references to it from the actor list. Unlinks it when present and clears its next pointer, then
 * checks its model record: subtype byte 0xE runs the model cleanup helper and clears the actor's effect.
 */
// FUNCTION: FF7 0x007611ff
void __cdecl WorldEntity_Unlink(WorldActor *actor)
{
    struct {
        WorldModelRecord *modelRecord;
        WorldActor *prevActor;
        WorldActor *actorCursor;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (actor == g_pWorldCurrentActor) {
        g_pWorldCurrentActor = 0;
    }
    if (actor == g_pWorldPlayerActor) {
        g_pWorldPlayerActor = 0;
    }
    for (frame.actorCursor = g_pWorldActiveActors; frame.actorCursor != 0;
         frame.actorCursor = frame.actorCursor->next) {
        if (frame.actorCursor->collide == actor) {
            frame.actorCursor->collide = 0;
        }
        if (frame.actorCursor->riding == actor) {
            frame.actorCursor->riding = 0;
        }
    }
    for (frame.prevActor = 0, frame.actorCursor = g_pWorldActiveActors;
         frame.actorCursor != 0 && frame.actorCursor != actor;
         frame.prevActor = frame.actorCursor, frame.actorCursor = frame.actorCursor->next) {
    }
    if (frame.actorCursor != 0) {
        if (frame.prevActor != 0) {
            frame.prevActor->next = frame.actorCursor->next;
        } else {
            g_pWorldActiveActors = frame.actorCursor->next;
        }
    }
    if (frame.actorCursor != 0) {
        frame.actorCursor->next = 0;
    }
    frame.modelRecord = World_GetModelRecord(actor->actorType);
    if (frame.modelRecord != 0 && frame.modelRecord->effect == 0xe) {
        World_ModelEffect14EndNoOp(frame.modelRecord);
        actor->effect = 0;
    }
}

/*
 * Detach whatever the player actor is carrying (riding) and promote it in
 * place: the passenger inherits the carrier's position, previous position,
 * facing, tilt and two more shorts, is marked with bit 2, and becomes the
 * player actor itself. The carrier's riding link is cleared, so this is a
 * dismount.
 *
 * The two 16-byte copies are struct assignments, not four scalar stores each.
 * At that size VC5 holds both bases in registers and does one load/store pair
 * per dword; only the 8-byte form reloads the base. That distinction is what
 * let this function be read at all.
 */
// FUNCTION: FF7 0x00761313
void __cdecl WorldEntity_DetachRiderAsPlayer(void)
{
    WorldActor *ent;

    if (g_pWorldPlayerActor != 0 && g_pWorldPlayerActor->riding != 0) {
        ent = g_pWorldPlayerActor->riding;
        WorldEntity_LinkIntoActiveList(ent);
        ent->collide = g_pWorldPlayerActor;
        ent->pos = g_pWorldPlayerActor->pos;
        ent->altPos = g_pWorldPlayerActor->altPos;
        ent->direction = g_pWorldPlayerActor->direction;
        ent->facing = g_pWorldPlayerActor->facing;
        ent->heading = g_pWorldPlayerActor->heading;
        ent->headingOffset = g_pWorldPlayerActor->headingOffset;
        ent->flags1 |= 2;
        g_pWorldPlayerActor->riding = 0;
        g_pWorldPlayerActor = ent;
    }
}

/*
 * For a nonnull entity whose next pointer is null, sets next to the active-list head and installs
 * the entity as the new head. It does not scan membership, so a null next pointer alone is the
 * insertion gate.
 */
// FUNCTION: FF7 0x00761407
void __cdecl WorldEntity_LinkIntoActiveList(WorldActor *entity)
{
    if (entity != 0 && entity->next == 0) {
        entity->next = g_pWorldActiveActors;
        g_pWorldActiveActors = entity;
    }
}

/*
 * Detach whatever the current actor is carrying (riding) and promote it in
 * place: the passenger inherits the carrier's position, previous position,
 * facing, tilt and two more shorts, is marked with bit 2, and becomes the
 * current actor itself. The carrier's riding link is cleared, so this is a
 * dismount.
 *
 * The two 16-byte copies are struct assignments, not four scalar stores each.
 * At that size VC5 holds both bases in registers and does one load/store pair
 * per dword; only the 8-byte form reloads the base. That distinction is what
 * let this function be read at all.
 */
// FUNCTION: FF7 0x0076142d
void __cdecl WorldEntity_DetachRiderAsCurrent(void)
{
    WorldActor *ent;

    if (g_pWorldCurrentActor != 0 && g_pWorldCurrentActor->riding != 0) {
        ent = g_pWorldCurrentActor->riding;
        WorldEntity_LinkIntoActiveList(ent);
        ent->collide = g_pWorldCurrentActor;
        ent->pos = g_pWorldCurrentActor->pos;
        ent->altPos = g_pWorldCurrentActor->altPos;
        ent->direction = g_pWorldCurrentActor->direction;
        ent->facing = g_pWorldCurrentActor->facing;
        ent->heading = g_pWorldCurrentActor->heading;
        ent->headingOffset = g_pWorldCurrentActor->headingOffset;
        ent->flags1 |= 2;
        g_pWorldCurrentActor->riding = 0;
        g_pWorldCurrentActor = ent;
    }
}

/*
 * Returns whether the current script entity (g_pWorldCurrentActor) has a rider/vehicle attached (->riding
 * != 0); 0 when there is no current entity.
 */
// FUNCTION: FF7 0x00761521
int __cdecl WorldEntity_IsCurrentRiding(void)
{
    int riding;

    if (g_pWorldCurrentActor != 0) {
        riding = g_pWorldCurrentActor->riding != 0;
    } else {
        riding = 0;
    }
    return riding;
}

/*
 * Frees the player entity g_pWorldPlayerActor back to the pool if there is one.
 */
// FUNCTION: FF7 0x0076154f
void __cdecl WorldEntity_FreePlayer(void)
{
    if (g_pWorldPlayerActor != 0) {
        WorldEntity_Free(g_pWorldPlayerActor);
    }
}

/*
 * Unlinks the entity, pushes it onto the g_pWorldActorFreeList free list, selects its actorType model
 * record in g_pWorldDeactivatedModelRecord and clears that record's active flag when present.
 */
// FUNCTION: FF7 0x0076156b
void __cdecl WorldEntity_Free(WorldActor *entity)
{
    WorldEntity_Unlink(entity);
    entity->next = g_pWorldActorFreeList;
    g_pWorldActorFreeList = entity;
    g_pWorldDeactivatedModelRecord =
        World_GetModelRecord((unsigned short)entity->actorType);
    if (g_pWorldDeactivatedModelRecord != 0) {
        g_pWorldDeactivatedModelRecord->active = 0;
    }
}

/*
 * Frees the current script entity g_pWorldCurrentActor back to the pool if there is one.
 */
// FUNCTION: FF7 0x007615ba
void __cdecl WorldEntity_FreeCurrent(void)
{
    if (g_pWorldCurrentActor != 0) {
        WorldEntity_Free(g_pWorldCurrentActor);
    }
}

/*
 * Frees whatever the player entity is riding and clears its ->riding link.
 */
// FUNCTION: FF7 0x007615d6
void __cdecl WorldEntity_FreePlayerRiding(void)
{
    if (g_pWorldPlayerActor != 0 && g_pWorldPlayerActor->riding != 0) {
        WorldEntity_Free(g_pWorldPlayerActor->riding);
        g_pWorldPlayerActor->riding = 0;
    }
}

/*
 * Frees whatever the current script entity is riding and clears its ->riding link.
 */
// FUNCTION: FF7 0x0076160d
void __cdecl WorldEntity_FreeCurrentRiding(void)
{
    if (g_pWorldCurrentActor != 0 && g_pWorldCurrentActor->riding != 0) {
        WorldEntity_Free(g_pWorldCurrentActor->riding);
        g_pWorldCurrentActor->riding = 0;
    }
}

/*
 * Walk every actor and clear any of its six 8-byte triangle cache slots
 * whose chunkX and chunkY both match. Only the triangle at the start of the slot is zeroed;
 * the key is left behind.
 *
 * The two shorts are a two-part key and what they name is not established --
 * the slots are written elsewhere in this TU and nothing here tells what they are.
 *
 * The inner bound is compared unsigned (`jb`, not `jl`), which is why both
 * sides are cast: these are addresses, and the original walked them as
 * pointers.
 */
// FUNCTION: FF7 0x00761644
void __cdecl WorldEntity_ClearTriangleCacheSlotsByKey(short kind, short id)
{
    WorldActor *ent;
    WorldTriangleCacheSlot *rec;

    for (ent = g_pWorldActiveActors; ent != 0; ent = ent->next) {
        for (rec = ent->triangleCache; (unsigned int)rec < (unsigned int)((char *)ent + 0x90); rec++) {
            if (rec->chunkX == kind && rec->chunkY == id) {
                rec->triangle = 0;
            }
        }
    }
}

/*
 * If a player entity exists, makes it the current script entity.
 */
// FUNCTION: FF7 0x007616b3
void __cdecl WorldEntity_SelectPlayerAsCurrentActor(void)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldCurrentActor = g_pWorldPlayerActor;
    }
}

/*
 * Makes the current script entity the player-controlled one (g_pWorldPlayerActor = g_pWorldCurrentActor) when
 * there is a current entity.
 */
// FUNCTION: FF7 0x007616cb
void __cdecl WorldEntity_SetPlayerControlled(void)
{
    if (g_pWorldCurrentActor != 0) {
        g_pWorldPlayerActor = g_pWorldCurrentActor;
    }
}

/*
 * Returns the player actor's 6-slot terrain triangle cache (+0x60).
 */
// FUNCTION: FF7 0x007616e3
char *__cdecl WorldEntity_GetPlayerTriangleCache(void)
{
    char *tris;

    if (g_pWorldPlayerActor != 0) {
        tris = (char *)g_pWorldPlayerActor->triangleCache;
    } else {
        tris = 0;
    }
    return tris;
}

/*
 * Returns the current script entity's actorType (its model id), or 0 with no current entity.
 */
// FUNCTION: FF7 0x0076170b
int __cdecl WorldEntity_GetCurrentModelId(void)
{
    int type;

    if (g_pWorldCurrentActor != 0) {
        type = g_pWorldCurrentActor->actorType;
    } else {
        type = 0;
    }
    return type;
}

/*
 * Returns the player entity's actorType (its model id), or 0 with no player entity.
 */
// FUNCTION: FF7 0x00761735
int __cdecl WorldEntity_GetPlayerModelId(void)
{
    int id;

    if (g_pWorldPlayerActor != 0) {
        id = g_pWorldPlayerActor->actorType;
    } else {
        id = 0;
    }
    return id;
}

/*
 * Returns the current script entity pointer g_pWorldCurrentActor.
 */
// FUNCTION: FF7 0x0076175f
WorldActor *__cdecl WorldEntity_GetCurrent(void)
{
    return g_pWorldCurrentActor;
}

/*
 * Is the player's model id one of the bits set in `mask`? Model ids from 0x20
 * up cannot be tested at all, because the mask is 32 bits wide and the shift
 * is not reduced -- so the range check is load-bearing, not defensive.
 */
// FUNCTION: FF7 0x00761769
int __cdecl WorldEntity_IsPlayerModelInMask(int mask)
{
    int hit;

    if (g_pWorldPlayerActor != 0 && g_pWorldPlayerActor->actorType < 0x20) {
        hit = (mask & 1 << g_pWorldPlayerActor->actorType) != 0;
    } else {
        hit = 0;
    }
    return hit;
}

/*
 * Returns whether the mask contains the current actor model bit when a current actor exists and
 * its unsigned model byte is below 32; otherwise returns zero.
 */
// FUNCTION: FF7 0x007617b7
int __cdecl WorldEntity_IsCurrentModelInMask(int mask)
{
    int hit;

    if (g_pWorldCurrentActor != 0 && g_pWorldCurrentActor->actorType < 0x20) {
        hit = (mask & 1 << g_pWorldCurrentActor->actorType) != 0;
    } else {
        hit = 0;
    }
    return hit;
}

/*
 * Masks model to its low byte, returns whether that bit is present in mask if the byte is below
 * 32, and returns zero otherwise. Higher bits of the supplied model are ignored.
 */
// FUNCTION: FF7 0x00761805
int __cdecl World_IsModelInMask(int mask, int model)
{
    int present;

    if ((model & 0xff) < 0x20) {
        present = (mask & (1 << (model & 0xff))) != 0;
    } else {
        present = 0;
    }
    return present;
}

/*
 * Returns whether a player exists and its model byte is 4, 0x13, 0x29 or 0x2a. This body tests the
 * model field directly; it does not inspect the riding link.
 */
// FUNCTION: FF7 0x00761844
int __cdecl WorldEntity_IsPlayerRideable(void)
{
    int rideable;
    int res;

    if (g_pWorldPlayerActor != 0) {
        if (g_pWorldPlayerActor->actorType != WORLD_MODEL_CHOCOBO_4 && g_pWorldPlayerActor->actorType != WORLD_MODEL_CHOCOBO && g_pWorldPlayerActor->actorType != 0x29 && g_pWorldPlayerActor->actorType != 0x2A) {
            rideable = 0;
        } else {
            rideable = 1;
        }
        res = rideable;
    } else {
        res = 0;
    }
    return res;
}

/*
 * The current actor's copy of the test above: models 4, 0x13, 0x29 and 0x2A.
 * Same two unnamed temporaries, same reason.
 */
// FUNCTION: FF7 0x007618b7
int __cdecl WorldEntity_IsCurrentRideable(void)
{
    int rideable;
    int res;

    if (g_pWorldCurrentActor != 0) {
        if (g_pWorldCurrentActor->actorType != WORLD_MODEL_CHOCOBO_4 && g_pWorldCurrentActor->actorType != WORLD_MODEL_CHOCOBO && g_pWorldCurrentActor->actorType != 0x29 && g_pWorldCurrentActor->actorType != 0x2A) {
            rideable = 0;
        } else {
            rideable = 1;
        }
        res = rideable;
    } else {
        res = 0;
    }
    return res;
}

/*
 * The same four model ids again, this time on a value the caller supplies.
 * The argument is masked to a byte at every use, which is what an `int`
 * parameter compared against an `unsigned char` field looks like -- the
 * original almost certainly took a wider type and truncated.
 */
// FUNCTION: FF7 0x0076192a
int __cdecl World_IsRideableModel(int model)
{
    int hit;

    if ((model & 0xFF) != 4 && (model & 0xFF) != 0x13 && (model & 0xFF) != 0x29 && (model & 0xFF) != 0x2A) {
        hit = 0;
    } else {
        hit = 1;
    }
    return hit;
}

/*
 * For a current actor, sets its model and supplies model-dependent four-short setup parameters and
 * auxiliary fields. The Highwind may attach the fixed riding actor; models 10 and 11 set distinct
 * speed/setup values; WORLD_MODEL_CHOCOBO_4 calls World_SetModelRecordTintFromTable then falls through to the default
 * setup. The Tiny Bronco, the submarine and model 28 skip parameter initialization but still pass the local block to
 * World_SetShadowQuadTextureRect, so their native initialization behavior remains uncertain. Finally
 * restores the actor from its vehicle slot.
 */
// FUNCTION: FF7 0x0076197b
void __cdecl WorldEntity_SetupForModel(int model)
{
    short bp_08[4];

    if (g_pWorldCurrentActor != 0) {
        g_pWorldCurrentActor->actorType = model;
        switch (model) {
        case WORLD_MODEL_HIGHWIND:
            if (World_GetStageVariantFlag() == 0) {
                g_pWorldCurrentActor->riding = (WorldActor *)&g_WorldVehicleActor;
            }
            bp_08[0] = 24;
            bp_08[1] = 16;
            bp_08[2] = 14;
            bp_08[3] = 31;
            g_pWorldCurrentActor->renderObject = g_WorldShadowRenderObject;
            break;
        case WORLD_MODEL_DIAMOND_WEAPON:
            bp_08[0] = 16;
            bp_08[1] = 0;
            bp_08[2] = 15;
            bp_08[3] = 15;
            g_pWorldCurrentActor->animStepState = 8 << 4;
            g_pWorldCurrentActor->renderObject = g_WorldMapRenderObject;
            break;
        case WORLD_MODEL_RED_SUBMARINE:
            break;
        case WORLD_MODEL_ULTIMA_WEAPON:
            bp_08[0] = 0;
            bp_08[1] = 0;
            bp_08[2] = 23;
            bp_08[3] = 47;
            g_pWorldCurrentActor->animStepState = 2 << 4;
            g_pWorldCurrentActor->renderObject = g_WorldShadowRenderObject;
            break;
        case WORLD_MODEL_CHOCOBO_4:
            World_SetModelRecordTintFromTable(WORLD_MODEL_CHOCOBO_4, 0);
        default:
            bp_08[0] = 24;
            bp_08[1] = 0;
            bp_08[2] = 15;
            bp_08[3] = 15;
            g_pWorldCurrentActor->animStepState = 2 << 4;
            g_pWorldCurrentActor->renderObject = g_WorldShadowRenderObject;
        case WORLD_MODEL_TINY_BRONCO:
        case WORLD_MODEL_SUBMARINE:
            break;
        }
        World_SetShadowQuadTextureRect((WorldShadowQuad *)(((char *)g_pWorldCurrentActor + 0x90)), (WorldShadowTextureRect *)&bp_08);
        WorldEntity_RestoreFromVehicleSlot(g_pWorldCurrentActor);
    }
}

/*
 * Sets the current script entity's direction and facing to `value` and clears its heading
 * offset.
 */
// FUNCTION: FF7 0x00761b19
void __cdecl WorldEntity_SetCurrentFacingClearTurn(short value)
{
    if (g_pWorldCurrentActor != 0) {
        g_pWorldCurrentActor->direction = value;
        g_pWorldCurrentActor->facing = value;
        g_pWorldCurrentActor->headingOffset = 0;
    }
}

/*
 * Sets the player entity's direction and facing to `value` and clears its heading offset.
 */
// FUNCTION: FF7 0x00761b4e
void __cdecl WorldEntity_SetPlayerFacingClearTurn(short value)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->direction = value;
        g_pWorldPlayerActor->facing = value;
        g_pWorldPlayerActor->headingOffset = 0;
    }
}

/*
 * Sets the current script entity's direction and facing to `value`, leaving the turn remainder
 * alone.
 */
// FUNCTION: FF7 0x00761b83
void __cdecl WorldEntity_SetCurrentFacing(short value)
{
    if (g_pWorldCurrentActor != 0) {
        g_pWorldCurrentActor->direction = value;
        g_pWorldCurrentActor->facing = value;
    }
}

/*
 * Sets the current script entity's direction, heading and facing all to `value` and clears the
 * heading offset: an instant snap with no interpolation left to do.
 */
// FUNCTION: FF7 0x00761bac
void __cdecl WorldEntity_SnapCurrentFacing(short value)
{
    if (g_pWorldCurrentActor != 0) {
        g_pWorldCurrentActor->direction = value;
        g_pWorldCurrentActor->heading = value;
        g_pWorldCurrentActor->facing = value;
        g_pWorldCurrentActor->headingOffset = 0;
    }
}

/*
 * Steps the current script entity's facing toward `value` through WorldEntity_StepFacingToward.
 */
// FUNCTION: FF7 0x00761bee
void __cdecl WorldEntity_StepCurrentFacingToward(short value)
{
    WorldEntity_StepFacingToward(g_pWorldCurrentActor, value);
}

/*
 * For a nonnull entity, adds one full turn to a negative target and compares direct and
 * plus/minus-0x1000 distances to choose a nearby equivalent target. Updates the heading with
 * (7*old+target)>>3 when g_WorldMovementMultiplier is 1, otherwise (3*old+target)>>2, stores the short result
 * and applies one plus/minus-0x1000 wrap correction. Distance locals are shorts and the operation
 * uses arithmetic shifts rather than signed division; does not fully normalize arbitrary inputs.
 */
// FUNCTION: FF7 0x00761c07
void __cdecl WorldEntity_StepFacingToward(struct WorldActor *entity, short targetFacing)
{
    short diff;
    short upDist;
    short distDown;
    int absDirect;
    int magUp;
    int absDown;
    int blended;

    if (entity != 0) {
        if (targetFacing < 0) {
            targetFacing += 0x1000;
        }
        if (entity->heading - targetFacing > 0) {
            absDirect = entity->heading - targetFacing;
        } else {
            absDirect = -(entity->heading - targetFacing);
        }
        diff = absDirect;
        if (entity->heading - targetFacing + 4096 > 0) {
            magUp = entity->heading - targetFacing + 0x1000;
        } else {
            magUp = -(entity->heading - targetFacing + 0x1000);
        }
        upDist = magUp;
        if (entity->heading - targetFacing - 4096 > 0) {
            absDown = entity->heading - targetFacing - 0x1000;
        } else {
            absDown = -(entity->heading - targetFacing - 0x1000);
        }
        distDown = absDown;
        if (diff > upDist) {
            if (upDist > distDown) {
                targetFacing += 0x1000;
            } else {
                targetFacing -= 0x1000;
            }
        } else if (diff > distDown) {
            targetFacing += 0x1000;
        }
        if (g_WorldMovementMultiplier == 1) {
            blended = entity->heading * 7 + targetFacing >> 3;
        } else {
            blended = entity->heading * 3 + targetFacing >> 2;
        }
        entity->heading = blended;
        if (entity->heading < 0) {
            entity->heading += 0x1000 /* 360 deg */;
        } else if (entity->heading >= 0x1000) {
            entity->heading -= 0x1000 /* 360 deg */;
        }
    }
}

/*
 * Steps the player entity's facing toward `value` through WorldEntity_StepFacingToward.
 */
// FUNCTION: FF7 0x00761ddc
void __cdecl WorldEntity_StepPlayerFacingToward(short value)
{
    WorldEntity_StepFacingToward(g_pWorldPlayerActor, value);
}

/*
 * For a nonnull current actor, smooths the heading offset toward target using integer weighted sums and
 * arithmetic right shifts. When g_WorldMovementMultiplier is 1, flag1 bit 0 selects (15*tilt+target)>>4
 * if set or (3*tilt+target)>>2 if clear. Otherwise selects (7*tilt+target)>>3 if set or
 * (tilt+target)>>1 if clear. The flag-clear case gives the faster target response.
 */
// FUNCTION: FF7 0x00761df5
void __cdecl WorldEntity_EaseCurrentTilt(short target)
{
    int fastBlend;
    int easedTilt;
    int slowBlend;

    if (g_pWorldCurrentActor != 0) {
        if (g_WorldMovementMultiplier == 1) {
            if ((g_pWorldCurrentActor->flags1 & 1) != 0) {
                fastBlend = g_pWorldCurrentActor->headingOffset * 15 + target >> 4;
            } else {
                fastBlend = g_pWorldCurrentActor->headingOffset * 3 + target >> 2;
            }
            easedTilt = fastBlend;
        } else {
            if ((g_pWorldCurrentActor->flags1 & 1) != 0) {
                slowBlend = g_pWorldCurrentActor->headingOffset * 7 + target >> 3;
            } else {
                slowBlend = g_pWorldCurrentActor->headingOffset + target >> 1;
            }
            easedTilt = slowBlend;
        }
        g_pWorldCurrentActor->headingOffset = easedTilt;
    }
}

/*
 * Returns the current script entity's heading plus its heading offset as a
 * short, or 0 with no current entity.
 */
// FUNCTION: FF7 0x00761eb6
short __cdecl WorldEntity_GetCurrentFacingPlusTurn(void)
{
    int sum;

    if (g_pWorldCurrentActor != 0) {
        sum = g_pWorldCurrentActor->heading + g_pWorldCurrentActor->headingOffset;
    } else {
        sum = 0;
    }
    return (short)sum;
}

/*
 * Returns the player entity's heading plus its heading offset as a short, or
 * 0 with no player entity.
 */
// FUNCTION: FF7 0x00761eec
short __cdecl WorldEntity_GetPlayerFacingPlusTurn(void)
{
    int sum;

    if (g_pWorldPlayerActor != 0) {
        sum = g_pWorldPlayerActor->heading + g_pWorldPlayerActor->headingOffset;
    } else {
        sum = 0;
    }
    return (short)sum;
}

/*
 * Adds `value` to the current script entity's altitude (pos.vy).
 */
// FUNCTION: FF7 0x00761f22
void __cdecl WorldEntity_AddCurrentAltitude(int value)
{
    if (g_pWorldCurrentActor != 0) {
        g_pWorldCurrentActor->pos.vy += value;
    }
}

/*
 * Returns bit 0 of the current script entity's flags1, the moved-this-frame flag.
 */
// FUNCTION: FF7 0x00761f44
int __cdecl WorldEntity_HasCurrentMoved(void)
{
    int moved;

    if (g_pWorldCurrentActor != 0) {
        moved = (g_pWorldCurrentActor->flags1 & 1) != 0;
    } else {
        moved = 0;
    }
    return moved;
}

/*
 * Returns bit 0 of the player entity's flags1, the moved-this-frame flag.
 */
// FUNCTION: FF7 0x00761f77
int __cdecl WorldEntity_HasPlayerMoved(void)
{
    int moved;

    if (g_pWorldPlayerActor != 0) {
        moved = (g_pWorldPlayerActor->flags1 & 1) != 0;
    } else {
        moved = 0;
    }
    return moved;
}

/*
 * Sets or clears bit 7 of the player entity's flags1, the flag that says the altitude is under
 * script control.
 */
// FUNCTION: FF7 0x00761faa
void __cdecl WorldEntity_SetPlayerAltitudeScripted(int value)
{
    if (g_pWorldPlayerActor != 0) {
        if (value != 0) {
            g_pWorldPlayerActor->flags1 |= 0x80;
        } else {
            g_pWorldPlayerActor->flags1 &= 0x7f;
        }
    }
}

/*
 * Returns bit 7 of the player entity's flags1: whether its altitude is under script control.
 */
// FUNCTION: FF7 0x00761fe8
int __cdecl WorldEntity_IsPlayerAltitudeScripted(void)
{
    int scripted;

    if (g_pWorldPlayerActor != 0) {
        scripted = (g_pWorldPlayerActor->flags1 & 0x80) != 0;
    } else {
        scripted = 0;
    }
    return scripted;
}

/*
 * Returns the player entity's ground height groundY, or 0 with no player entity.
 */
// FUNCTION: FF7 0x0076201e
int __cdecl WorldEntity_GetPlayerGroundY(void)
{
    int y;

    if (g_pWorldPlayerActor != 0) {
        y = g_pWorldPlayerActor->groundY;
    } else {
        y = 0;
    }
    return y;
}

/*
 * Searches g_pWorldActiveActors's actor list by actorType, updates the current script actor g_pWorldCurrentActor
 * only on a match and returns whether one was found.
 */
// FUNCTION: FF7 0x00762047
int __cdecl WorldEntity_SetCurrentById(int dwModelType)
{
    WorldActor *actor;

    for (actor = g_pWorldActiveActors;
         actor != 0 && actor->actorType != dwModelType;
         actor = actor->next) {
    }
    if (actor != 0) {
        g_pWorldCurrentActor = actor;
    }
    return actor != 0;
}

/*
 * Makes the entity the player last collided with (->collide) the current script entity, when there
 * is one.
 */
// FUNCTION: FF7 0x0076208e
void __cdecl WorldEntity_SetCurrentToPlayerCollide(void)
{
    if (g_pWorldPlayerActor != 0 && g_pWorldPlayerActor->collide != 0) {
        g_pWorldCurrentActor = g_pWorldPlayerActor->collide;
    }
}

/*
 * Returns 1 when both player and current actor pointers exist, they differ, and bit 0x10 of the
 * current actor flags1 is clear. Returns zero otherwise; this body alone does not
 * establish a passenger relationship.
 */
// FUNCTION: FF7 0x007620b6
int __cdecl WorldEntity_IsCurrentNonPlayerRequestable(void)
{
    int ok;

    if (g_pWorldPlayerActor != 0 && g_pWorldCurrentActor != 0 && g_pWorldPlayerActor != g_pWorldCurrentActor && (g_pWorldCurrentActor->flags1 & 0x10) == 0) {
        ok = 1;
    } else {
        ok = 0;
    }
    return ok;
}

/*
 * Sets the player entity's actorType (model id).
 */
// FUNCTION: FF7 0x00762102
void __cdecl WorldEntity_SetPlayerModelId(int value)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->actorType = value;
    }
}

/*
 * Stores a packed walkmesh word on the player entity: terrain type in bits 0-4, region id in bits
 * 5-7, encounter region in bits 9-13, a flag in bit 15.
 */
// FUNCTION: FF7 0x0076211b
void __cdecl WorldEntity_SetPlayerTerrainInfo(int value)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->walkmesh = value;
    }
}

/*
 * Returns the player entity's terrain type, bits 0-4 of its walkmesh word.
 */
// FUNCTION: FF7 0x00762136
int __cdecl WorldEntity_GetPlayerTerrainType(void)
{
    int terrain;

    if (g_pWorldPlayerActor != 0) {
        terrain = g_pWorldPlayerActor->walkmesh & WORLD_WALKMESH_TERRAIN_MASK;
    } else {
        terrain = 0;
    }
    return terrain;
}

/*
 * Returns the player entity's encounter region, bits 9-13 of its walkmesh word.
 */
// FUNCTION: FF7 0x00762162
int __cdecl WorldEntity_GetPlayerEncounterRegion(void)
{
    int region;

    if (g_pWorldPlayerActor != 0) {
        region = (g_pWorldPlayerActor->walkmesh >> 9) & 0x1f;
    } else {
        region = 0;
    }
    return region;
}

/*
 * Returns bit 15 of the player entity's walkmesh word.
 */
// FUNCTION: FF7 0x00762191
int __cdecl WorldEntity_GetPlayerTerrainTopBit(void)
{
    int bit;

    if (g_pWorldPlayerActor != 0) {
        bit = (g_pWorldPlayerActor->walkmesh >> 15) & 1;
    } else {
        bit = 0;
    }
    return bit;
}

/*
 * Returns the player entity's region id, bits 5-7 of its walkmesh word.
 */
// FUNCTION: FF7 0x007621c0
int __cdecl WorldEntity_GetPlayerRegionId(void)
{
    int id;

    if (g_pWorldPlayerActor != 0) {
        id = (g_pWorldPlayerActor->walkmesh >> 5) & 7;
    } else {
        id = 0;
    }
    return id;
}

/*
 * Places the current script entity at `value` and resolves its ground height through
 * WorldEntity_SetPositionAndGround.
 */
// FUNCTION: FF7 0x007621ef
void __cdecl WorldEntity_SetCurrentPositionAndGround(void *value)
{
    WorldEntity_SetPositionAndGround(g_pWorldCurrentActor, (WorldPosition *)value);
}

/*
 * For nonzero position data and actor, calls the position helper, then copies X/Z only for flag
 * 0x80 or copies XYZ and adds model ground offset otherwise. Stores the supplied Y as ground
 * height and sets or clears movement flag 1 according to X/Z differences from the alternate
 * position.
 */
// FUNCTION: FF7 0x00762207
void __cdecl WorldEntity_SetPositionAndGround(WorldActor *entity, WorldPosition *value)
{
    if (value != 0 && entity != 0) {
        World_WrapPositionToMapBounds(value);
        if ((entity->flags1 & 0x80) != 0) {
            entity->pos.vx = value->x;
            entity->pos.vz = value->z;
        } else {
            entity->pos = *(VECTOR *)value;
            entity->pos.vy += World_GetModelGroundOffset(
                entity->walkmesh, entity->actorType);
        }
        entity->groundY = value->y;
        if (entity->pos.vx != entity->altPos.vx || entity->pos.vz != entity->altPos.vz) {
            entity->flags1 |= 1;
        } else {
            entity->flags1 &= 0xFE;
        }
    }
}

/*
 * Returns fixed offsets for the ship, the submarine and model 25. For the ridden chocobo on water
 * terrain, gradually raises or lowers shared ground offset according to player movement flag and
 * terrain, returning that shared value; other terrain under the chocobo clears it. Remaining terrain
 * cases return -0xC0, -0x40 or zero. The chocobo's handling dereferences the player actor without a
 * null check.
 */
// FUNCTION: FF7 0x007622e5
int __cdecl World_GetModelGroundOffset(int terrainInfo, unsigned char modelType)
{
    terrainInfo &= 0x1f;
    switch (modelType) {
    case WORLD_MODEL_SHIP:
        return -0xf;
    case WORLD_MODEL_SUBMARINE:
        return -0xf0;
    case WORLD_MODEL_ROCK:
        return 0;
    case WORLD_MODEL_CHOCOBO:
        if ((terrainInfo >= WORLD_TERRAIN_SEA && terrainInfo <= WORLD_TERRAIN_SHALLOW_WATER) || terrainInfo == WORLD_TERRAIN_SEA_2) {
            if (g_pWorldPlayerActor->flags1 & 1) {
                if (g_WorldPlayerGroundOffset < 0) {
                    g_WorldPlayerGroundOffset =
                        (g_WorldPlayerGroundOffset + 20 > 0) ? 0 : g_WorldPlayerGroundOffset + 20;
                }
            } else {
                int groundOffset = (terrainInfo != WORLD_TERRAIN_RIVER_CROSSING && terrainInfo != WORLD_TERRAIN_SHALLOW_WATER) ? -250 : -50;
                if (g_WorldPlayerGroundOffset > groundOffset) {
                    g_WorldPlayerGroundOffset -= 10;
                } else if (g_WorldPlayerGroundOffset < groundOffset - 20) {
                    g_WorldPlayerGroundOffset = groundOffset;
                }
            }
            return g_WorldPlayerGroundOffset;
        }
        g_WorldPlayerGroundOffset = 0;
    }

    return (terrainInfo == WORLD_TERRAIN_FOREST || terrainInfo == WORLD_TERRAIN_JUNGLE)
               ? -0xc0
           : (terrainInfo == WORLD_TERRAIN_RIVER_CROSSING || terrainInfo == WORLD_TERRAIN_SWAMP) ? -0x40
                                                    : 0;
}

/*
 * Places the player entity at `value` and resolves its ground height through
 * WorldEntity_SetPositionAndGround.
 */
// FUNCTION: FF7 0x00762465
void __cdecl WorldEntity_SetPlayerPosition(void *value)
{
    WorldEntity_SetPositionAndGround(g_pWorldPlayerActor, (WorldPosition *)value);
}

/*
 * Put the player on the ground at `ground` and then snap its previous
 * position to its current one, so nothing interpolates across the move.
 *
 * The null check is *after* the call, not before it -- the image passes the
 * pointer to WorldEntity_SetPositionAndGround unconditionally and only tests
 * it for the copy. Hoisting the test costs the match and would also be a
 * behaviour change.
 */
// FUNCTION: FF7 0x0076247d
void __cdecl WorldEntity_SetPlayerPositionAndGround(void *ground)
{
    WorldEntity_SetPositionAndGround(g_pWorldPlayerActor, (WorldPosition *)ground);
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->altPos = g_pWorldPlayerActor->pos;
    }
}

/*
 * Sets the player position and ground through WorldEntity_SetPositionAndGround, then arms a
 * 30-tick countdown on terrain 1 or 0x19 and clears it on terrain 2 or 0xC. While bit 0x80 is
 * clear and the countdown is active, decrements it only when flag bit 1 is set. A nonzero signed
 * step snaps altitude to ground when the comparison signs disagree; otherwise bit 1 permits
 * applying the step relative to ground and doubling it while the gap allows, or clearing it.
 * Without bit 1 it snaps altitude to ground. With no step, gaps beyond plus or minus 50 snap and
 * seed a step of 1 or -1. The inactive path clears both step and countdown.
 */
// FUNCTION: FF7 0x007624c5
void __cdecl WorldEntity_ApplyPlayerGroundStep(void *ground)
{
    int terrain;

    terrain = WorldEntity_GetPlayerTerrainType();
    WorldEntity_SetPositionAndGround(g_pWorldPlayerActor, (WorldPosition *)ground);
    if (terrain == WORLD_TERRAIN_FOREST || terrain == WORLD_TERRAIN_JUNGLE) {
        g_WorldPlayerStepCountdown = 30;
    } else if (terrain == WORLD_TERRAIN_MOUNTAIN || terrain == WORLD_TERRAIN_CLIFF) {
        g_WorldPlayerStepCountdown = 0;
    }
    if ((g_pWorldPlayerActor->flags1 & 0x80) == 0 && g_WorldPlayerStepCountdown != 0) {
        g_WorldPlayerStepCountdown -= (g_pWorldPlayerActor->flags1 & 1) != 0;
        if (g_WorldPlayerStepY != 0) {
            if ((g_pWorldPlayerActor->pos.vy > g_pWorldPlayerActor->altPos.vy ^ g_WorldPlayerStepY > 0) != 0) {
                g_WorldPlayerStepY = 0;
                g_pWorldPlayerActor->pos.vy = g_pWorldPlayerActor->altPos.vy;
            } else if ((g_pWorldPlayerActor->flags1 & 1) != 0) {
                if (g_pWorldPlayerActor->pos.vy > g_pWorldPlayerActor->altPos.vy) {
                    if (g_pWorldPlayerActor->pos.vy - g_pWorldPlayerActor->altPos.vy > g_WorldPlayerStepY) {
                        g_pWorldPlayerActor->pos.vy = g_pWorldPlayerActor->altPos.vy + g_WorldPlayerStepY;
                        g_WorldPlayerStepY <<= 1;
                    } else {
                        g_WorldPlayerStepY = 0;
                    }
                } else if (g_pWorldPlayerActor->pos.vy - g_pWorldPlayerActor->altPos.vy < g_WorldPlayerStepY) {
                    g_pWorldPlayerActor->pos.vy = g_pWorldPlayerActor->altPos.vy + g_WorldPlayerStepY;
                    g_WorldPlayerStepY <<= 1;
                } else {
                    g_WorldPlayerStepY = 0;
                }
            } else {
                g_pWorldPlayerActor->pos.vy = g_pWorldPlayerActor->altPos.vy;
            }
        } else if (g_pWorldPlayerActor->pos.vy - g_pWorldPlayerActor->altPos.vy < -50) {
            g_pWorldPlayerActor->pos.vy = g_pWorldPlayerActor->altPos.vy;
            g_WorldPlayerStepY = 0xFFFFFFFF;
        } else if (g_pWorldPlayerActor->pos.vy - g_pWorldPlayerActor->altPos.vy > 50) {
            g_pWorldPlayerActor->pos.vy = g_pWorldPlayerActor->altPos.vy;
            g_WorldPlayerStepY = 1;
        }
    } else {
        g_WorldPlayerStepY = 0;
        g_WorldPlayerStepCountdown = 0;
    }
}

/*
 * Sets the player entity's altitude (pos.vy).
 */
// FUNCTION: FF7 0x00762702
void __cdecl WorldEntity_SetPlayerAltitude(int value)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->pos.vy = value;
    }
}

/*
 * Sets the player entity's altitude and raises bit 2 of flags1, marking the height as explicitly
 * set.
 */
// FUNCTION: FF7 0x0076271b
void __cdecl WorldEntity_SetPlayerAltitudeExplicit(int value)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->pos.vy = value;
        g_pWorldPlayerActor->flags1 |= 4;
    }
}

/*
 * Sets the player entity's unused +0x4E attribute.
 */
// FUNCTION: FF7 0x00762748
void __cdecl WorldEntity_SetPlayerUnusedAttr(int value)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->unusedAttr4E = value;
    }
}

/*
 * Copies the current actor 16-byte position to out when both pointers are nonnull; otherwise
 * leaves the output unchanged.
 */
// FUNCTION: FF7 0x00762763
void __cdecl WorldEntity_GetCurrentPosition(VECTOR *out)
{
    if (out != 0 && g_pWorldCurrentActor != 0) {
        *out = g_pWorldCurrentActor->pos;
    }
}

/*
 * Gives the world position of the actor the player controls (g_pWorldPlayerActor; while the party
 * rides a vehicle or chocobo this is the mount): copies its 16-byte `pos` (x, y, z and the pad
 * word) into *out. Does nothing without an out pointer or a player actor, so *out keeps whatever
 * the caller put there.
 */
// FUNCTION: FF7 0x00762798
void __cdecl WorldEntity_GetPlayerPosition(VECTOR *out)
{
    if (out != 0 && g_pWorldPlayerActor != 0) {
        *out = g_pWorldPlayerActor->pos;
    }
}

/*
 * Copies the current script entity's previous position (altPos) into *out.
 */
// FUNCTION: FF7 0x007627cd
void __cdecl WorldEntity_GetCurrentPrevPosition(VECTOR *out)
{
    if (out != 0 && g_pWorldCurrentActor != 0) {
        *out = g_pWorldCurrentActor->altPos;
    }
}

/*
 * Copies the player entity's previous position (altPos) into *out.
 */
// FUNCTION: FF7 0x00762802
void __cdecl WorldEntity_GetPlayerPrevPosition(VECTOR *out)
{
    if (out != 0 && g_pWorldPlayerActor != 0) {
        *out = g_pWorldPlayerActor->altPos;
    }
}

/*
 * For a current actor, restores only x and z from the previous position when flags1 has
 * mask 4 set; otherwise restores the full 16-byte position. Clears the moved flag with mask 0xfe
 * afterwards.
 */
// FUNCTION: FF7 0x00762837
void __cdecl WorldEntity_RevertCurrentPosition(void)
{
    if (g_pWorldCurrentActor != 0) {
        if ((g_pWorldCurrentActor->flags1 & 4) != 0) {
            g_pWorldCurrentActor->pos.vx = g_pWorldCurrentActor->altPos.vx;
            g_pWorldCurrentActor->pos.vz = g_pWorldCurrentActor->altPos.vz;
        } else {
            g_pWorldCurrentActor->pos = g_pWorldCurrentActor->altPos;
        }
        g_pWorldCurrentActor->flags1 &= 0xFE;
    }
}

/*
 * For a player actor, restores only x and z from the previous position when flags1 has
 * mask 4 set; otherwise restores the full 16-byte position. Clears the moved flag with mask 0xfe
 * afterwards.
 */
// FUNCTION: FF7 0x007628b5
void __cdecl WorldEntity_RevertPlayerPosition(void)
{
    if (g_pWorldPlayerActor != 0) {
        if ((g_pWorldPlayerActor->flags1 & 4) != 0) {
            g_pWorldPlayerActor->pos.vx = g_pWorldPlayerActor->altPos.vx;
            g_pWorldPlayerActor->pos.vz = g_pWorldPlayerActor->altPos.vz;
        } else {
            g_pWorldPlayerActor->pos = g_pWorldPlayerActor->altPos;
        }
        g_pWorldPlayerActor->flags1 &= 0xFE;
    }
}

/*
 * Clears bit 1 of the player entity's flags1, the on-mesh flag.
 */
// FUNCTION: FF7 0x00762933
void __cdecl WorldEntity_ClearPlayerOnMesh(void)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->flags1 &= 0xfd;
    }
}

/*
 * Sets the current script entity's animation id.
 */
// FUNCTION: FF7 0x00762955
void __cdecl WorldEntity_SetCurrentAnimId(int value)
{
    if (g_pWorldCurrentActor != 0) {
        g_pWorldCurrentActor->animId = value;
    }
}

/*
 * Tests the current script entity against the world for a collision and, on a hit, puts it back
 * where it was. Returns the collision result.
 */
// FUNCTION: FF7 0x0076296e
int __cdecl WorldEntity_ResolveCurrentCollision(void)
{
    int hit = 0;

    hit = WorldEntity_TestCurrentCollision();
    if (hit != 0) {
        WorldEntity_RevertCurrentPosition();
    }
    return hit;
}

/*
 * For a current actor, tests actors in list order until collision helper returns nonzero, except
 * type 0xD on map variant zero. Stores the found actor as the collision target and returns its
 * pointer value when the collision result is below two; returns zero otherwise. An empty/no-hit
 * search leaves the previous collision field unchanged.
 */
// FUNCTION: FF7 0x00762993
int __cdecl WorldEntity_TestCurrentCollision(void)
{
    int collideResult;
    WorldActor *pTarget;

    collideResult = 0;
    pTarget = 0;
    if (g_pWorldCurrentActor != 0) {
        if (g_pWorldCurrentActor->actorType != WORLD_MODEL_SUBMARINE || World_GetMapVariant() != 0) {
            for (pTarget = g_pWorldActiveActors;
                 pTarget != 0 && (collideResult = WorldEntity_TestActorPairCollision(g_pWorldCurrentActor, (WorldActor *)pTarget)) == 0;
                 pTarget = pTarget->next) {
            }
            if (pTarget != 0) {
                g_pWorldCurrentActor->collide = pTarget;
            }
        }
    }
    return (int)((collideResult < 2) ? pTarget : 0);
}

/*
 * Returns 0 unless entities are nonnull, distinct, enabled, have types below 26 and the second
 * entity is not type 21, 22 or 23. Wraps X and Z separation over map dimensions 0x48000 and
 * 0x38000 and requires both magnitudes below 0x400. Uses the resulting 8 by 8 coordinates to test
 * either entity collision mask. For a hit, returns 2 when separation from the first entity
 * previous position has no greater Manhattan distance than current separation, otherwise 1.
 * Updates no entity fields or collision links.
 */
// FUNCTION: FF7 0x00762a21
int __cdecl WorldEntity_TestActorPairCollision(WorldActor *entity, WorldActor *collider)
{
    int result;
    int deltaZ;
    int xDelta;
    int distance;
    int absDxCur;
    int az;
    int absDx;
    int absZCurrent;
    int absPreviousX;
    int collide;
    int magZPrevious;

    result = 0;
    if (entity != 0 && collider != 0 && (int)entity != (int)collider && (entity->flags1 & 0x80) == 0 && (collider->flags1 & 0x80) == 0 && entity->actorType < 0x1A && collider->actorType < 0x1A && collider->actorType != 0x15 && collider->actorType != 0x16 && collider->actorType != 0x17) {
        xDelta = collider->pos.vx - entity->pos.vx;
        if (xDelta > 147456) {
            xDelta -= 0x48000;
        } else if (xDelta < -147456) {
            xDelta += 0x48000;
        }
        deltaZ = collider->pos.vz - entity->pos.vz;
        if (deltaZ > 114688) {
            deltaZ -= 0x38000;
        } else if (deltaZ < -114688) {
            deltaZ += 0x38000;
        }
        if (xDelta > 0) {
            absDx = xDelta;
        } else {
            absDx = -xDelta;
        }
        if (absDx < 1024) {
            if (deltaZ > 0) {
                az = deltaZ;
            } else {
                az = -deltaZ;
            }
            if (az < 1024) {
                if (xDelta > 0) {
                    absDxCur = xDelta;
                } else {
                    absDxCur = -xDelta;
                }
                if (deltaZ > 0) {
                    absZCurrent = deltaZ;
                } else {
                    absZCurrent = -deltaZ;
                }
                distance = absDxCur + absZCurrent;
                xDelta = xDelta + 1024 >> 8;
                deltaZ = deltaZ + 1024 >> 8;
                if (xDelta >= 0 && xDelta < 8 && deltaZ >= 0 && deltaZ < 8) {
                    if ((g_WorldActorCollisionMasks[(deltaZ + entity->actorType * 8)] >> xDelta & 1) == 0 && (g_WorldActorCollisionMasks[(collider->actorType * 8 + (7 - deltaZ))] >> 7 - xDelta & 1) == 0) {
                        collide = 0;
                    } else {
                        collide = 1;
                    }
                    result = collide;
                    if (result != 0) {
                        xDelta = collider->pos.vx - entity->altPos.vx;
                        if (xDelta > 147456) {
                            xDelta -= 0x48000;
                        } else if (xDelta < -147456) {
                            xDelta += 0x48000;
                        }
                        deltaZ = collider->pos.vz - entity->altPos.vz;
                        if (deltaZ > 114688) {
                            deltaZ -= 0x38000;
                        } else if (deltaZ < -114688) {
                            deltaZ += 0x38000;
                        }
                        if (xDelta > 0) {
                            absPreviousX = xDelta;
                        } else {
                            absPreviousX = -xDelta;
                        }
                        if (deltaZ > 0) {
                            magZPrevious = deltaZ;
                        } else {
                            magZPrevious = -deltaZ;
                        }
                        if (absPreviousX + magZPrevious <= distance) {
                            result = 2;
                        }
                    }
                }
            }
        }
    }
    return result;
}

/*
 * Returns the entity the current script entity last collided with (->collide), or 0.
 */
// FUNCTION: FF7 0x00762d2a
WorldActor *__cdecl WorldEntity_GetCurrentCollidedActor(void)
{
    WorldActor *other;

    if (g_pWorldCurrentActor != 0) {
        other = g_pWorldCurrentActor->collide;
    } else {
        other = 0;
    }
    return other;
}

/*
 * Sets bit 1 of the player entity's flags1, the on-mesh flag.
 */
// FUNCTION: FF7 0x00762d52
void __cdecl WorldEntity_MarkPlayerOnMesh(void)
{
    if (g_pWorldPlayerActor != 0) {
        g_pWorldPlayerActor->flags1 |= 2;
    }
}

/*
 * Visits all actors except the player actor and maps each position to mesh coordinates. For actors
 * on the supplied mesh, samples ground and walkmesh information, adjusts Y unless flag 0x80 is
 * set, and sets flag 2. Finally updates the two additional mesh-associated object groups through
 * their helpers.
 */
// FUNCTION: FF7 0x00762d74
void __cdecl World_PlaceEntitiesOnMesh(int meshRecord)
{
    short cellX;
    short localPosition[3];
    WorldActor *pActor;
    short cellZ;
    WorldActor *pPrevious;

    pPrevious = 0;
    pActor = g_pWorldActiveActors;
    for (; pActor != 0; pPrevious = pActor, pActor = pActor->next) {
        if (pActor != g_pWorldPlayerActor) {
            WorldMap_PositionToMesh(&pActor->pos.vx, localPosition, &cellX, &cellZ);
            if (cellX == ((WorldTerrainChunkInfo *)meshRecord)->wChunkX && cellZ == ((WorldTerrainChunkInfo *)meshRecord)->wChunkY) {
                World_SampleGroundAtEntity((WorldTerrainChunkInfo *)meshRecord, (SVECTOR *)localPosition, (WorldTriangleCache *)pActor->triangleCache, &pActor->groundY, 0, &pActor->walkmesh,
                                           pActor->actorType);
                if ((pActor->flags1 & 0x80) == 0) {
                    pActor->pos.vy = pActor->groundY +
                                World_GetModelGroundOffset(
                                    pActor->walkmesh, pActor->actorType);
                }
                pActor->flags1 |= 2;
            }
        }
    }
    World_SampleZolomHeadGround((WorldTerrainChunkInfo *)meshRecord);
    World_PushTerrainFloodCrossingRecord((struct WorldPushTerrainFloodCrossingRecordMesh *)meshRecord);
}

// Moves the current world actor by (dx, dz) when there is one and either delta is nonzero. The
// deltas are added to pos.vx and pos.vz, and each axis then gets at most one correction by its map
// extent (0x48000 for vx, 0x38000 for vz). Large deltas are therefore not fully normalized. Bit 0
// of flags1 is set.
// FUNCTION: FF7 0x00762e87
void __cdecl WorldEntity_MoveCurrentBy(int dx, int dz)
{
    if (g_pWorldCurrentActor != 0 && (dx != 0 || dz != 0)) {
        g_pWorldCurrentActor->pos.vx += dx;
        g_pWorldCurrentActor->pos.vz += dz;
        if (g_pWorldCurrentActor->pos.vx < 0) {
            g_pWorldCurrentActor->pos.vx += 0x48000;
        }
        if (g_pWorldCurrentActor->pos.vx >= 294912) {
            g_pWorldCurrentActor->pos.vx -= 0x48000;
        }
        if (g_pWorldCurrentActor->pos.vz < 0) {
            g_pWorldCurrentActor->pos.vz += 0x38000;
        }
        if (g_pWorldCurrentActor->pos.vz >= 229376) {
            g_pWorldCurrentActor->pos.vz -= 0x38000;
        }
        g_pWorldCurrentActor->flags1 |= 1;
    }
}

/*
 * Stores the three camera angle offsets g_WorldVehicleSavedFacing/b8/bc.
 */
// FUNCTION: FF7 0x00762f75
void __cdecl World_SetCameraAngleOffsets(short savedFacing, short savedAltFacing, short transitionFlags)
{
    g_WorldVehicleSavedFacing = savedFacing;
    g_WorldVehicleSavedAltFacing = savedAltFacing;
    g_WorldVehicleTransitionFlags = transitionFlags;
}

/*
 * Returns a squared-distance fade weight for the world point (x, 0, z). It projects the point
 * through the installed rotation and screen matrices, takes its depth (>> 2) less the fade target
 * and its horizontal distance from screen centre, and returns ((depth^2 >> 12) + (dx^2 >> 10)) >>
 * 2, or 0 once the depth is at or before the fade target. The two float-to-int conversions are the
 * _ftol helper. sz, m1, m2, dh, px and pos are the matrix and vector scratch the original's frame
 * lays out.
 */
// FUNCTION: FF7 0x00762f9a
int __cdecl World_GetPointFadeWeight(int x, int z)
{
    short sx;
    float m1[16];
    int i;
    float m2[16];
    short sz[4];
    float dh[3];
    float pos[3];
    int cur;
    float px[4];
    int amt;

    sz[0] = 0;
    sz[1] = 0;
    sz[2] = 0;
    sz[3] = 0;
    World_SetViewTransformForPosition((SVECTOR *)sz);
    cur = ((unsigned int)g_WorldRenderWidth >> 1) + g_WorldViewportX;
    pos[0] = (float)x;
    pos[1] = 0;
    pos[2] = (float)z;
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), m1);
    ComposeWithScreenMatrix(m1, m2);
    TransformPointByMatrix4x4(m2, pos, px);
    TransformPointDepthOnly(m1, pos, dh);
    amt = (int)dh[2] >> 2;
    sx = (short)(int)(px[0] / px[3]);
    sx = sx >= cur ? sx - cur : cur - sx;
    amt -= World_GetFadeTarget();
    amt = amt > 0 ? (amt * amt >> 12) + (sx * sx >> 10) : 0;
    return amt >> 2;
}

/*
 * Writes the vehicle return offset as a three-short vector: out[0] = g_WorldMeshTriggerArg0, out[1] = 0,
 * out[2] = g_WorldMeshTriggerArg1.
 */
// FUNCTION: FF7 0x0076312d
void __cdecl WorldEntity_GetPlayerSlopeTiltAngles(short *out)
{
    if (out != 0) {
        out[0] = (*(short *)&g_WorldMeshTriggerArg0);
        out[1] = 0;
        out[2] = (*(short *)&g_WorldMeshTriggerArg1);
    }
}

/*
 * Calls WorldEntity_BeginFrame for every actor, then makes a second pass clearing each actor's low
 * three flags1 bits and the corresponding model record's active flag when present; g_pWorldDeactivatedModelRecord
 * tracks the last selected record.
 */
// FUNCTION: FF7 0x0076315c
void __cdecl World_BeginEntityFrame(void)
{
    WorldActor *actor;

    for (actor = g_pWorldActiveActors; actor != 0; actor = actor->next) {
        WorldEntity_BeginFrame(actor);
    }
    for (actor = g_pWorldActiveActors; actor != 0; actor = actor->next) {
        actor->flags1 &= ~7;
        g_pWorldDeactivatedModelRecord =
            World_GetModelRecord((unsigned short)actor->actorType);
        if (g_pWorldDeactivatedModelRecord != 0) {
            g_pWorldDeactivatedModelRecord->active = 0;
        }
    }
}

/*
 * Break the collide link if either this actor or the one it is attached
 * to moved last frame, then snapshot the position block for the revert above.
 *
 * The `||` is a short-circuit, not a nested `if`: the second test jumps into
 * the body, and a nested reading duplicates the assignment.
 */
// FUNCTION: FF7 0x007631df
void __cdecl WorldEntity_BeginFrame(WorldActor *ent)
{
    if (ent->collide != 0 && ((ent->flags1 & 1) != 0 || (ent->collide->flags1 & 1) != 0)) {
        ent->collide = 0;
    }
    ent->altPos = ent->pos;
}

/*
 * Two full passes over the actor list, not one pass calling both -- every actor gets
 * WorldEntity_UpdateActorModel before any gets WorldEntity_DrawShadowIfEligible. The list head is
 * re-loaded for the second loop, which is the tell.
 */
// FUNCTION: FF7 0x0076323a
void __cdecl WorldEntity_UpdateAllActorModelsAndShadows(void)
{
    WorldActor *ent;

    for (ent = g_pWorldActiveActors; ent != 0; ent = ent->next) {
        WorldEntity_UpdateActorModel(ent);
    }
    for (ent = g_pWorldActiveActors; ent != 0; ent = ent->next) {
        WorldEntity_DrawShadowIfEligible(ent);
    }
}

/*
 * Updates an active world actor model and optional riding model, clearing activity when flag eight
 * is set or model lookup fails. Dereferences actor before its later null test. Selects explicit
 * animation or infers movement from wrapped X/Z displacement, applies type-six and mask-specific
 * rates, bounds animation/frame only at the upper end, and positions models relative to stream
 * focus with world wrapping and curvature. The special player-camera branch instead uses global
 * rotation and zero horizontal position. Copies transforms to the riding model, renders both with
 * type-specific animation rules and smoothed slope angles, services the actor effect helper, and
 * advances animation using a byte accumulator divided by the rate nibble. Type-three/five rates
 * and rider frames receive further updates. Finally computes yaw-rotated type-specific shadow
 * offsets and curvature-adjusted height. Missing riding model prevents the remaining actor update.
 */
// FUNCTION: FF7 0x0076328f
void __cdecl WorldEntity_UpdateActorModel(WorldActor *actor)
{
    struct {
        VECTOR slope;
        int frame;
        WorldModelRecord *rider;
        WorldShadowQuad *shadow;
        int dz, distance, curve, dy;
        WorldModelRecord *model;
        short rotation[4];
        int dx;
        short animation, pad;
        WorldStreamFocusData focus;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (actor->flags1 & 8)
        actor->flags1 &= ~2;
    if (actor && g_pWorldPlayerActor && (actor->flags1 & 2)) {
        s.model = World_GetModelRecord(actor->actorType);
        if (s.model == 0) {
            actor->flags1 &= ~2;
        } else {
            s.rider = 0;
            if (actor->riding && (s.rider = World_GetModelRecord(actor->riding->actorType)) == 0) {
                actor->riding->flags1 &= ~2;
            } else {
                s.distance = 0;
                if (actor->animId) {
                    s.animation = actor->animId > 0 ? actor->animId : 0;
                } else {
                    s.dx = actor->pos.vx - actor->altPos.vx > 0 ? actor->pos.vx - actor->altPos.vx : -(actor->pos.vx - actor->altPos.vx);
                    if (s.dx >= 147456)
                        s.dx = 0x48000 - s.dx;
                    s.dy = actor->pos.vz - actor->altPos.vz > 0 ? actor->pos.vz - actor->altPos.vz : -(actor->pos.vz - actor->altPos.vz);
                    if (s.dy >= 114688)
                        s.dy = 0x38000 - s.dy;
                    s.distance = s.dx * s.dx + s.dy * s.dy;
                    s.animation = s.distance >= 100;
                    if (actor->actorType == WORLD_MODEL_BUGGY) {
                        actor->animStepState = (actor == g_pWorldPlayerActor) << 5;
                        s.animation = s.distance ? WorldEntity_AdvanceTerrainStateMachine(&actor->animFrame) : 0;
                    /* Retail caller prototype (int, unsigned char) -> int preserved for /Od argument promotion on `World_IsModelInMask`. */
                    } else if (((int(__cdecl *)(int, unsigned char))World_IsModelInMask)(0x2000, actor->actorType)) {
                        actor->animStepState = (s.distance > 0) << 5;
                    }
                }
                s.model->effect = 0;
                if (s.animation >= s.model->animationCount)
                    s.animation = s.model->animationCount - 1;
                if (*(unsigned char *)&actor->animFrame >= s.model->animations[s.animation]->frameCount)
                    actor->animFrame = actor->flags1 & 0x20 ? s.model->animations[s.animation]->frameCount - 1 : 0;
                s.dx = s.dz = 0;
                s.curve = 0;
                s.rotation[0] = s.rotation[1] = s.rotation[2] = 0;
                if ((actor->flags1 & 0x80) && actor == g_pWorldPlayerActor) {
                    s.model->rotation.x = (float)(g_WorldVehicleSavedFacing - 64);
                    s.model->rotation.y = 0;
                    s.model->rotation.z = 0;
                    s.model->position.x = 0;
                    s.model->position.y = (float)(actor->pos.vy - g_WorldCameraDeltaY);
                    s.model->position.z = 0;
                    s.rotation[1] = g_WorldVehicleSavedAltFacing + actor->heading + actor->headingOffset;
                    s.rotation[2] = g_WorldVehicleTransitionFlags;
                } else {
                    World_GetStreamFocus(&s.focus);
                    s.dx = actor->pos.vx - s.focus.x;
                    if (s.dx < -147456)
                        s.dx += 0x48000;
                    else if (s.dx >= 147456)
                        s.dx -= 0x48000;
                    s.model->position.x = (float)s.dx;
                    s.dz = actor->pos.vz - s.focus.z;
                    if (s.dz < -114688)
                        s.dz += 0x38000;
                    else if (s.dz >= 114688)
                        s.dz -= 0x38000;
                    s.model->position.z = (float)s.dz;
                    if (actor != g_pWorldPlayerActor)
                        s.curve = World_GetPointFadeWeight(s.dx, s.dz);
                    s.dy = actor->pos.vy - g_WorldCameraDeltaY + actor->yOffset;
                    s.model->position.y = (float)(s.dy - s.curve);
                    s.model->rotation.x = -64.0f;
                    s.model->rotation.y = (float)((actor->heading + actor->headingOffset) >> 4);
                    s.model->rotation.z = 0;
                }
                if (s.rider) {
                    s.rider->position.x = s.model->position.x;
                    s.rider->position.y = s.model->position.y;
                    s.rider->position.z = s.model->position.z;
                    s.rider->rotation.x = s.model->rotation.x;
                    s.rider->rotation.y = s.model->rotation.y;
                    s.rider->rotation.z = s.model->rotation.z;
                }
                if (actor->actorType == WORLD_MODEL_BUGGY) {
                    s.frame = *(unsigned char *)&actor->animFrame;
                    if (g_pWorldPlayerActor->actorType == WORLD_MODEL_BUGGY && (g_pWorldPlayerActor->flags1 & 1)) {
                        World_SumThreeViewVectors((int *)&s.slope);
                        g_WorldMeshTriggerArg0 = (g_WorldMeshTriggerArg0 * 3 - WorldMap_ArcTangentFromDeltas(s.slope.vz, -s.slope.vy)) >> 2;
                        g_WorldMeshTriggerArg1 = (g_WorldMeshTriggerArg1 * 3 - WorldMap_ArcTangentFromDeltas(s.slope.vx, -s.slope.vy)) >> 2;
                    }
                    s.rotation[0] = (*(short *)&g_WorldMeshTriggerArg0);
                    s.rotation[2] = (*(short *)&g_WorldMeshTriggerArg1);
                    World_ApplyActorPoseToModelRecord(s.model, actor, s.animation, s.frame, s.rotation);
                } else {
                    World_ApplyActorPoseToModelRecord(s.model, actor, s.animation, *(unsigned char *)&actor->animFrame, s.rotation);
                }
                if (s.rider) {
                    if (actor->actorType == WORLD_MODEL_HIGHWIND) {
                        if (actor->riding)
                            World_ApplyActorPoseToModelRecord(s.rider, 0, 0, *(unsigned char *)&actor->riding->animFrame, s.rotation);
                    } else {
                        World_ApplyActorPoseToModelRecord(s.rider, 0, s.animation == 0 ? 2 : 3, *(unsigned char *)&actor->animFrame, s.rotation);
                    }
                }
                WorldEntity_SpawnTerrainEffects(actor, (actor->flags1 & 1) != 0);
                if (actor->animStepState >> 4) {
                    actor->animStepAccum += g_WorldMovementMultiplier;
                    actor->animFrame += *(unsigned char *)&actor->animStepAccum / (actor->animStepState >> 4);
                    *(unsigned char *)&actor->animStepAccum %= actor->animStepState >> 4;
                }
                if (actor->actorType == WORLD_MODEL_HIGHWIND) {
                    if (actor->flags1 & 0x80)
                        actor->animStepState = 32;
                    else if (actor->animStepState)
                        actor->animStepState++;
                    if (s.distance >= 200 && actor->riding) {
                        actor->riding->animFrame++;
                        if (*(unsigned char *)&actor->riding->animFrame >= s.model->animations[s.animation]->frameCount)
                            actor->riding->animFrame = 0;
                    }
                } else if (actor->actorType == WORLD_MODEL_TINY_BRONCO) {
                    if (actor == g_pWorldPlayerActor)
                        actor->animStepState = 32;
                    else if (actor->animStepState)
                        actor->animStepState++;
                }
                s.shadow = (WorldShadowQuad *)((char *)actor + 0x90);
                s.rotation[0] = s.rotation[2] = 0;
                if (actor->actorType == WORLD_MODEL_HIGHWIND) {
                    s.rotation[0] = -10;
                    s.rotation[2] = 80;
                } else if (actor->actorType == 11) {
                    s.rotation[2] = -360;
                }
                WorldMap_RotateVec3ByYaw((SVECTOR *)s.rotation, actor->heading + actor->headingOffset + World_GetCameraRoll() * 4);
                s.shadow->pad1 = s.dx + s.rotation[0];
                actor->curvedGroundY = actor->groundY - s.curve;
                s.shadow->pad2 = s.dz + s.rotation[2];
            }
        }
    }
}

/*
 * A four-state machine at g_WorldVehicleTransitionState, advanced by the player's
 * terrain type and by a byte counter the caller owns. 1 waits for riverside terrain; 2 waits for the
 * counter to pass 5; 3 waits for the terrain to be neither a river crossing nor riverside; 4 waits for
 * the counter again and returns to 1. Any other value resets to 1.
 *
 * The counter is zeroed on every transition and only on a transition, which is
 * what the trailing compare against the entry value is for.
 *
 * `prev` and `kind` are named to keep their slots: they are in buckets 12 and
 * 15, which is the order the frame wants. Two spellings in the same bucket
 * would have to be declared backwards.
 */
// FUNCTION: FF7 0x00763aae
int __cdecl WorldEntity_AdvanceTerrainStateMachine(void *counter)
{
    int prev;
    int kind;

    prev = g_WorldVehicleTransitionState;
    kind = WorldEntity_GetPlayerTerrainType();
    switch (g_WorldVehicleTransitionState) {
    case WORLD_FORD_WAIT_RIVERSIDE:
        if (kind == WORLD_TERRAIN_RIVERSIDE) {
            g_WorldVehicleTransitionState = 2;
        }
        break;
    case WORLD_FORD_ENTER_DELAY:
        if (*(unsigned char *)counter > 5) {
            g_WorldVehicleTransitionState = 3;
        }
        break;
    case WORLD_FORD_CROSSING:
        if (kind != WORLD_TERRAIN_RIVER_CROSSING && kind != WORLD_TERRAIN_RIVERSIDE) {
            g_WorldVehicleTransitionState = 4;
        }
        break;
    case WORLD_FORD_EXIT_DELAY:
        if (*(unsigned char *)counter > 5) {
            g_WorldVehicleTransitionState = 1;
        }
        break;
    default:
        g_WorldVehicleTransitionState = 1;
    }
    if (g_WorldVehicleTransitionState != prev) {
        *(char *)counter = 0;
    }
    return g_WorldVehicleTransitionState;
}

/*
 * For a nonnull actor/player and bit 2 of flags1, calls World_DrawActorShadow when the original model-
 * mask/rideability/player/terrain-bit condition holds, or the actorType equals 3 or 0xB.
 * Preserves the original grouped boolean evaluation and exception model checks.
 */
// FUNCTION: FF7 0x00763b74
void __cdecl WorldEntity_DrawShadowIfEligible(WorldActor *actor)
{
    if (actor != 0 && g_pWorldPlayerActor != 0 && (actor->flags1 & 2) != 0) {
        /* Retail caller prototype (int, char) -> int preserved for /Od argument promotion on `World_IsModelInMask`. */
        if (((((int(__cdecl *)(int, char))World_IsModelInMask)(7, *(char *)&actor->actorType) != 0 &&
              (actor->animId < 2 || actor->animId > 5)) ||
             /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `World_IsRideableModel`. */
             ((int(__cdecl *)(char))World_IsRideableModel)(*(char *)&actor->actorType) != 0) &&
                (actor != g_pWorldPlayerActor || g_WorldPlayerStepY == 0) &&
                ((WORLD_TERRAIN_MASK_SHADOW_ELIGIBLE >> (actor->walkmesh & WORLD_WALKMESH_TERRAIN_MASK)) & 1) != 0 ||
            actor->actorType == WORLD_MODEL_HIGHWIND || actor->actorType == 0xB) {
            World_DrawActorShadow(actor);
        }
    }
}

/*
 * Point the world script at its two variable windows and zero everything
 * else it keeps.
 *
 * The first window is the savemap: bank 0 starts at gSavemap.storyProgressLo, so world script
 * variables ARE savemap memory-bank bytes. The second is the 0x40-byte block at
 * g_WorldScriptAuxStorage, which the first loop clears and which three more globals
 * are seeded from.
 *
 * Every `x = 0; y = x;` pair here is written that way in the original --
 * the zero is stored, then read back, then stored again. Written as two
 * assignments to a constant it is a different instruction sequence.
 */
// FUNCTION: FF7 0x00763c35
void __cdecl WorldScript_Init(unsigned char *base)
{
    int i;

    g_pWorldScriptBase = (WorldScriptDirectoryEntry *)base;
    g_pWorldScriptCode = (unsigned short *)(base + 0x400);
    g_WorldScriptStackCursor = g_WorldScriptOperandStack;
    g_pWorldScriptBank0 = &gSavemap.storyProgressLo;
    g_pWorldScriptBank3 = g_WorldScriptAuxStorage;
    g_pWorldScriptBank2 = g_pWorldScriptBank3;
    g_pWorldScriptBank1 = g_pWorldScriptBank2;
    g_pWorldExecutingActor = 0;
    g_WorldMeshTriggerCounter = 0;
    g_WorldVehicleBoardedFlag = g_WorldMeshTriggerCounter;
    g_WorldWaitFramesZero = g_WorldVehicleBoardedFlag;
    for (i = 0; i < 64; i++) {
        g_WorldScriptAuxStorage[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        g_WorldScriptSavedPositions[i].vz = 0;
        g_WorldScriptSavedPositions[i].vy = 0;
        g_WorldScriptSavedPositions[i].vx = 0;
    }
    g_WorldVehicleModelStack.pendingModel = 0;
    (g_WorldVehicleModelStack.currentModel) = g_WorldVehicleModelStack.pendingModel;
    g_WorldVehicleSlotStack.pendingSlot = 0;
    (g_WorldVehicleSlotStack.currentSlot) = g_WorldVehicleSlotStack.pendingSlot;
    /* Three members of one VECTOR, not three globals: annotating the
       last two separately breaks World_SetVehicleReturnPosition. */
    g_WorldVehicleReturnPosition.vz = 0;
    g_WorldVehicleReturnPosition.vy = g_WorldVehicleReturnPosition.vz;
    g_WorldVehicleReturnPosition.vx = g_WorldVehicleReturnPosition.vy;
}

/*
 * Set or clear bit 0x10 of the player actor's flags1 -- the bit
 * WorldEntity_IsCurrentNonPlayerRequestable above tests.
 *
 * `&= 0xEF` rather than `&= ~0x10`: the image has `and byte ptr [...], 0xEF`,
 * an eight-bit mask on a byte field. Writing the complement of a wider
 * constant produces a wider AND.
 */
// FUNCTION: FF7 0x00763d6c
void __cdecl WorldEntity_SetPlayerRequestsBlocked(int on)
{
    if (g_pWorldPlayerActor != 0) {
        if (on != 0) {
            g_pWorldPlayerActor->flags1 |= 0x10;
        } else {
            g_pWorldPlayerActor->flags1 &= 0xEF;
        }
    }
}

/*
 * Run script entry `code` on the current actor, unless bit 0x10 of its flags
 * says it is not accepting requests. The priority handed on is `code - 2`
 * clamped to 0..3.
 *
 * The clamp is spelled out three times because that is what the image does:
 * `code - 2` is recomputed inside the else branch rather than reusing the
 * temporary, so the original wrote the max() twice. Folding it to one
 * expression drops the function to 89%.
 *
 * `p`, `q` and `r` are the original's temporaries for that clamp; their
 * spelling fixes their slots.
 */
// FUNCTION: FF7 0x00763daa
void __cdecl WorldScript_RequestOnCurrentEntity(int code)
{
    int clampA;
    int clampB;
    int prio;

    if (g_pWorldCurrentActor != 0 && (g_pWorldCurrentActor->flags1 & 0x10) == 0) {
        if (code - 2 < 0) {
            clampA = 0;
        } else {
            clampA = code - 2;
        }
        if (clampA > 3) {
            prio = 3;
        } else {
            if (code - 2 < 0) {
                clampB = 0;
            } else {
                clampB = code - 2;
            }
            prio = clampB;
        }
        WorldScript_Request(code & 0xFF, prio);
    }
}

/*
 * Accepts a script request for the current actor when priority increases, equal priority is three,
 * or no script is active. Saves an interrupted script, wait and priority on its four-entry stack,
 * searches the script directory for the ID and installs its index/priority or zeros them. Restores
 * the interrupted script when lookup fails. Stack overflow reports error 0x41 but continues.
 */
// FUNCTION: FF7 0x00763e31
void __cdecl WorldScript_Request(int dwScriptId, int dwPriority)
{
    struct {
        int searchStep;
        int didPushState;
        WorldScriptCallFrame *savedStateSlot;
        WorldScriptDirectoryEntry *scriptTableCursor;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.didPushState = 0;
    if (g_pWorldCurrentActor != 0) {
        if (g_pWorldCurrentActor->scriptPriority < dwPriority ||
            (g_pWorldCurrentActor->scriptPriority == dwPriority && dwPriority == 3) ||
            g_pWorldCurrentActor->scriptIdx == 0) {
            if (g_pWorldCurrentActor->scriptCallDepth >= 4)
                WorldScript_ReportError(0x41);
            if (g_pWorldCurrentActor->scriptIdx != 0) {
                frame.savedStateSlot = &g_pWorldCurrentActor->callStack[g_pWorldCurrentActor->scriptCallDepth];
                g_pWorldCurrentActor->scriptCallDepth++;
                frame.savedStateSlot->scriptIdx = g_pWorldCurrentActor->scriptIdx;
                frame.savedStateSlot->waitFrames = g_pWorldCurrentActor->waitFrames;
                frame.savedStateSlot->scriptPriority = g_pWorldCurrentActor->scriptPriority;
                frame.didPushState = 1;
            }
            frame.scriptTableCursor = (WorldScriptDirectoryEntry *)(((unsigned char *)((unsigned char *)g_pWorldScriptBase)) + 0x200);
            frame.searchStep = 64;
            for (; frame.searchStep != 0 && frame.scriptTableCursor->scriptId != dwScriptId; frame.searchStep >>= 1) {
                if (frame.scriptTableCursor->scriptId < dwScriptId)
                    frame.scriptTableCursor += frame.searchStep;
                else
                    frame.scriptTableCursor -= frame.searchStep;
            }
            if (frame.scriptTableCursor->scriptId == dwScriptId) {
                g_pWorldCurrentActor->scriptIdx = frame.scriptTableCursor->scriptIdx;
                g_pWorldCurrentActor->scriptPriority = dwPriority;
            } else {
                g_pWorldCurrentActor->scriptIdx = 0;
                g_pWorldCurrentActor->scriptPriority = 0;
            }
            if (frame.didPushState && g_pWorldCurrentActor->scriptIdx == 0) {
                g_pWorldCurrentActor->scriptCallDepth--;
                frame.savedStateSlot = &g_pWorldCurrentActor->callStack[g_pWorldCurrentActor->scriptCallDepth];
                g_pWorldCurrentActor->scriptIdx = frame.savedStateSlot->scriptIdx;
                g_pWorldCurrentActor->waitFrames = frame.savedStateSlot->waitFrames;
                g_pWorldCurrentActor->scriptPriority = frame.savedStateSlot->scriptPriority;
            }
        }
    }
}

/*
 * Requests script function funcId on entity entityId: makes it the current entity, and unless its flag
 * 0x10 of flags1 is set, queues WorldScript_Request with the packed id (entity in bits 8-13,
 * 0x4000, function in the low byte) at priority clamp(funcId - 2, 0, 3).
 */
// FUNCTION: FF7 0x00764014
void __cdecl WorldScript_RequestOnEntity(int entityId, int funcId)
{
    int floor;
    int priority;
    int shifted;

    WorldEntity_SetCurrentById(entityId);
    if (g_pWorldCurrentActor != 0 && (g_pWorldCurrentActor->flags1 & 0x10) == 0) {
        if (funcId - 2 < 0) {
            floor = 0;
        } else {
            floor = funcId - 2;
        }
        if (floor > 3) {
            priority = 3;
        } else {
            if (funcId - 2 < 0) {
                shifted = 0;
            } else {
                shifted = funcId - 2;
            }
            priority = shifted;
        }
        WorldScript_Request((entityId << 8 & 0x3F00) | 0x4000 | funcId & 0xFF, priority);
    }
}

/*
 * Requests system script function funcId on the system entity g_WorldSystemActor (which becomes the
 * current entity); function 1 also sets g_WorldVehicleBoardedFlag. Priority is clamp(funcId - 2, 0, 3).
 */
// FUNCTION: FF7 0x007640bc
void __cdecl WorldScript_RequestSystem(int funcId)
{
    int floor;
    int priority;
    int shifted;

    g_pWorldCurrentActor = &g_WorldSystemActor;
    if (funcId == 1) {
        g_WorldVehicleBoardedFlag = 1;
    }
    if (funcId - 2 < 0) {
        floor = 0;
    } else {
        floor = funcId - 2;
    }
    if (floor > 3) {
        priority = 3;
    } else {
        if (funcId - 2 < 0) {
            shifted = 0;
        } else {
            shifted = funcId - 2;
        }
        priority = shifted;
    }
    WorldScript_Request(funcId & 0xFF, priority);
}

/*
 * Sets current script actor g_pWorldCurrentActor to g_WorldSystemActor, converts the player position to
 * mesh coordinates and requests the encoded 0x8000 mesh script using the 36-column index and low
 * nibble of regionIndex+5*row, priority 3.
 */
// FUNCTION: FF7 0x00764142
void __cdecl WorldScript_RequestMeshScript(short regionIndex, short row)
{
    int meshZ;
    int gridX;

    g_pWorldCurrentActor = &g_WorldSystemActor;
    WorldMap_PositionToMesh(&g_pWorldPlayerActor->pos, 0, (short *)&gridX, (short *)&meshZ);
    WorldScript_Request(0x8000 | (((*(short *)&meshZ * 0x24 + *(short *)&gridX) << 4) & 0x3ff0) |
                            ((regionIndex + row * 5) & 0xf),
                        3);
}

/*
 * Runs one script step for every entity: the system entity g_WorldSystemActor first, then each live
 * WorldActor from g_pWorldActiveActors along ->next, with g_pWorldCurrentActor and g_pWorldExecutingActor pointing at the
 * one being run.
 */
// FUNCTION: FF7 0x007641a7
void __cdecl WorldScript_RunAllEntities(void)
{
    WorldActor *i;

    g_pWorldExecutingActor = &g_WorldSystemActor;
    g_pWorldCurrentActor = &g_WorldSystemActor;
    WorldScript_ExecEntity(&g_WorldSystemActor);
    for (i = g_pWorldActiveActors; i != 0; i = i->next) {
        g_pWorldCurrentActor = i;
        g_pWorldExecutingActor = g_pWorldCurrentActor;
        WorldScript_ExecEntity(i);
    }
}

/*
 * Clears g_WorldWaitFramesZero and runs the executing actor's bytecode until a control-flow or
 * command handler pauses, dispatching opcode ranges to operator, operand, control-flow and command
 * handlers while advancing the instruction index. Steps facing toward the supplied actor
 * direction, then requests a collision actor script at priority three or four according to held
 * input when player control permits.
 */
// FUNCTION: FF7 0x0076420a
void __cdecl WorldScript_ExecEntity(WorldActor *actor)
{
    struct {
        unsigned short wOpcode;
        int dwPauseScript;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_WorldWaitFramesZero = 0;
    if (g_pWorldExecutingActor->scriptIdx) {
        frame.dwPauseScript = 0;
        while (!frame.dwPauseScript) {
            frame.wOpcode = g_pWorldScriptCode[g_pWorldExecutingActor->scriptIdx];
            (g_pWorldExecutingActor->scriptIdx)++;
            if (frame.wOpcode < 0x100) {
                WorldScript_ExecOperator(frame.wOpcode);
            } else if (frame.wOpcode < 0x200) {
                /* Retail caller prototype (unsigned short) -> void preserved for /Od argument promotion on `WorldScript_PushOperand`. */
                ((void(__cdecl *)(unsigned short))WorldScript_PushOperand)(frame.wOpcode);
            } else if (frame.wOpcode < 0x300) {
                frame.dwPauseScript =
                    WorldScript_ExecControlFlow(frame.wOpcode);
            } else {
                frame.dwPauseScript =
                    WorldScript_ExecCommand(frame.wOpcode);
            }
        }
    }
    WorldEntity_StepCurrentFacingToward(actor->direction);
    if (actor->collide && g_WorldVehicleBoardedFlag == 0 && World_IsPlayerControlEnabled()) {
        WorldScript_RequestOnEntity(actor->collide->actorType,
                                    (Input_GetHeldRaw() & 0x20) == 0 ? 3 : 4);
    }
}

/*
 * Executes world-script unary negation/not, binary multiply/add/subtract, shifts, signed
 * comparisons, equality, bitwise AND/OR and logical AND/OR using popped operands. Both logical
 * operands are always popped before evaluation. Opcode 224 passes a popped value to
 * WorldScript_StoreToLValue; opcodes 0x18 through 0x1B query zone distance, entity distance, one
 * of three saved-point distances or zone angle with retained shifts. Entity distance temporarily
 * changes and then restores the current actor; invalid saved-point indices produce zero. After
 * every opcode, including unsupported ones, stamps the current stack slot kind 0x110 and advances
 * the stack by eight, so an unsupported opcode pushes the existing slot value.
 */
// FUNCTION: FF7 0x00764336
void __cdecl WorldScript_ExecOperator(unsigned short op)
{
    int first;
    WorldActor *keep;
    int right;
    VECTOR loc;

    switch (op) {
    case WORLD_SCRIPT_NEG:
        g_WorldScriptStackCursor->value = -WorldScript_PopStackValue();
        break;
    case WORLD_SCRIPT_NOT:
        g_WorldScriptStackCursor->value = !WorldScript_PopStackValue();
        break;
    case WORLD_SCRIPT_MUL:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first * right;
        break;
    case WORLD_SCRIPT_ADD:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first + right;
        break;
    case WORLD_SCRIPT_SUB:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first - right;
        break;
    case WORLD_SCRIPT_LT:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first < right;
        break;
    case WORLD_SCRIPT_GT:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first > right;
        break;
    case WORLD_SCRIPT_LE:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first <= right;
        break;
    case WORLD_SCRIPT_GE:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first >= right;
        break;
    case WORLD_SCRIPT_EQ:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first == right;
        break;
    case WORLD_SCRIPT_NE:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first != right;
        break;
    case WORLD_SCRIPT_SHL:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first << right;
        break;
    case WORLD_SCRIPT_SHR:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first >> right;
        break;
    case WORLD_SCRIPT_AND:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first & right;
        break;
    case WORLD_SCRIPT_OR:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first | right;
        break;
    case WORLD_SCRIPT_LOGICAL_AND:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first && right;
        break;
    case WORLD_SCRIPT_LOGICAL_OR:
        right = WorldScript_PopStackValue();
        first = WorldScript_PopStackValue();
        g_WorldScriptStackCursor->value = first || right;
        break;
    case WORLD_SCRIPT_ASSIGN:
        right = WorldScript_PopStackValue();
        WorldScript_StoreToLValue(right);
        break;
    case WORLD_SCRIPT_ZONE_DISTANCE:
        WorldZone_Select(WorldScript_PopStackValue(), -1);
        WorldEntity_GetCurrentPosition(&loc);
        g_WorldScriptStackCursor->value = WorldZone_DistanceFrom((WorldPoint *)&loc) >> 5;
        break;
    case WORLD_SCRIPT_ENTITY_DISTANCE:
        keep = g_pWorldCurrentActor;
        g_WorldScriptStackCursor->value = 0;
        if (WorldEntity_SetCurrentById(WorldScript_PopStackValue()) != 0) {
            g_WorldScriptStackCursor->value = WorldMap_WrappedDistance((WorldPoint *)&g_pWorldCurrentActor->pos, (WorldPoint *)&keep->pos) >> 4;
        }
        g_pWorldCurrentActor = keep;
        break;
    case WORLD_SCRIPT_SAVED_POSITION_DISTANCE:
        first = WorldScript_PopStackValue();
        loc = g_pWorldCurrentActor->pos;
        loc.vy = 0;
        g_WorldScriptStackCursor->value =
            first >= 0 && first < 3
                ? WorldMap_WrappedDistance((WorldPoint *)&loc, (WorldPoint *)&g_WorldScriptSavedPositions[first]) >> 4
                : 0;
        break;
    case WORLD_SCRIPT_ZONE_ANGLE:
        WorldZone_Select(WorldScript_PopStackValue(), -1);
        WorldEntity_GetCurrentPosition(&loc);
        g_WorldScriptStackCursor->value = WorldZone_AngleFrom(&loc) >> 4;
        break;
    }
    g_WorldScriptStackCursor->kind = WORLD_SCRIPT_OPERAND_CONSTANT;
    g_WorldScriptStackCursor++;
}

/*
 * Pops an eight-byte world-script operand, reporting underflow after decrementing the pointer but
 * continuing to read it if the error handler returns. Low kind bits 0 through 2 select a bank for
 * constant, bit, byte or signed-word reads; a misaligned word reports error but is still read. Low
 * kind bits 3 select entity position cell and offset, heading, actor and terrain fields, player or
 * leader model, message answer, random byte, four globals or helper queries. Unknown kinds and
 * special selectors return zero.
 */
// FUNCTION: FF7 0x0076488c
int __cdecl WorldScript_PopStackValue(void)
{
    short *p;
    int value;

    value = 0;
    if (g_WorldScriptStackCursor-- <= g_WorldScriptOperandStack) {
        WorldScript_ReportError(0x3E);
    }
    if ((g_WorldScriptStackCursor->kind & 3) < 3) {
        switch (g_WorldScriptStackCursor->kind & ~3) {
        case WORLD_SCRIPT_OPERAND_CONSTANT:
            value = g_WorldScriptStackCursor->value;
            break;
        case WORLD_SCRIPT_OPERAND_BIT:
            value = (g_WorldScriptBanks[g_WorldScriptStackCursor->kind & 3][g_WorldScriptStackCursor->value >> 3] >> (g_WorldScriptStackCursor->value & 7)) & 1;
            break;
        case WORLD_SCRIPT_OPERAND_BYTE:
            value = g_WorldScriptBanks[g_WorldScriptStackCursor->kind & 3][g_WorldScriptStackCursor->value];
            break;
        case WORLD_SCRIPT_OPERAND_WORD:
            p = (short *)(g_WorldScriptBanks[g_WorldScriptStackCursor->kind & 3] + g_WorldScriptStackCursor->value);
            if ((int)p & 1) {
                WorldScript_ReportError(0x3F);
            }
            value = *p;
            break;
        }
    } else {
        switch (g_WorldScriptStackCursor->value) {
        case WORLD_SCRIPT_SPECIAL_CELL_X:
            value = g_pWorldCurrentActor->pos.vx >> 13;
            break;
        case WORLD_SCRIPT_SPECIAL_CELL_Z:
            value = g_pWorldCurrentActor->pos.vz >> 13;
            break;
        case WORLD_SCRIPT_SPECIAL_OFFSET_X:
            value = g_pWorldCurrentActor->pos.vx & 0x1FFF;
            break;
        case WORLD_SCRIPT_SPECIAL_OFFSET_Z:
            value = g_pWorldCurrentActor->pos.vz & 0x1FFF;
            break;
        case WORLD_SCRIPT_SPECIAL_HEADING:
            value = (g_pWorldCurrentActor->direction >> 4) & 0xFF;
            break;
        case WORLD_SCRIPT_SPECIAL_FIELD_TRANSITION_STATE:
            value = World_GetTransitionRequestState();
            break;
        case WORLD_SCRIPT_SPECIAL_FIELD_TRANSITION_ARG:
            value = World_GetTransitionArgument();
            break;
        case WORLD_SCRIPT_SPECIAL_CAMERA_MINIMAP_STATE:
            value = World_GetCameraModeAndMiniMapState();
            break;
        case WORLD_SCRIPT_SPECIAL_PLAYER_MODEL:
            value = WorldEntity_GetPlayerModelId();
            break;
        case WORLD_SCRIPT_SPECIAL_ACTOR_TYPE:
            value = g_pWorldCurrentActor->actorType;
            break;
        case WORLD_SCRIPT_SPECIAL_CHOCOBO_ENCOUNTER:
            value = World_GetChocoboEncounterInfo();
            break;
        case WORLD_SCRIPT_SPECIAL_BATTLE_FLAG_8:
            value = World_GetBattleEscaped();
            break;
        case WORLD_SCRIPT_SPECIAL_SWAMP_ENTRY_X_FLAG:
            value = World_GetSwampEntryXFlag();
            break;
        case WORLD_SCRIPT_SPECIAL_MESSAGE_ANSWER:
            value = WorldMsg_GetAnswer();
            break;
        case WORLD_SCRIPT_SPECIAL_REGION:
            value = (g_pWorldCurrentActor->walkmesh >> 5) & 7;
            break;
        case WORLD_SCRIPT_SPECIAL_LEADER_MODEL:
            value = WorldMap_GetLeaderModelIndex();
            break;
        case WORLD_SCRIPT_SPECIAL_RANDOM_BYTE:
            value = WorldMap_NextRandomByte();
            break;
        case WORLD_SCRIPT_SPECIAL_VEHICLE_MODEL:
            value = (g_WorldVehicleModelStack.currentModel);
            break;
        case WORLD_SCRIPT_SPECIAL_PENDING_VEHICLE_MODEL:
            value = g_WorldVehicleModelStack.pendingModel;
            break;
        case WORLD_SCRIPT_SPECIAL_VEHICLE_SLOT:
            value = (g_WorldVehicleSlotStack.currentSlot);
            break;
        case WORLD_SCRIPT_SPECIAL_PENDING_VEHICLE_SLOT:
            value = g_WorldVehicleSlotStack.pendingSlot;
            break;
        }
    }
    return value;
}

/*
 * Pops a script lvalue cell, reports stack underflow without stopping, selects one of four storage
 * bases and stores the supplied value as a bit, byte or short according to the cell tag. Reports
 * odd short addresses or unsupported tags, then replaces the cell with an immediate value and
 * advances the stack pointer. The alignment diagnostic does not prevent the short write.
 */
// FUNCTION: FF7 0x00764b8c
void __cdecl WorldScript_StoreToLValue(int value)
{
    int bitIndex;
    unsigned char *bytePtr;

    if (g_WorldScriptStackCursor-- <= g_WorldScriptOperandStack) {
        WorldScript_ReportError(0x3E);
    }
    bytePtr = g_WorldScriptBanks[g_WorldScriptStackCursor->kind & 3];
    switch (g_WorldScriptStackCursor->kind & -4) {
    case WORLD_SCRIPT_OPERAND_BIT:
        bytePtr += g_WorldScriptStackCursor->value >> 3;
        bitIndex = g_WorldScriptStackCursor->value & 7;
        *bytePtr &= ~(1 << bitIndex);
        *bytePtr |= (value != 0) << bitIndex;
        break;
    case WORLD_SCRIPT_OPERAND_BYTE:
        bytePtr[g_WorldScriptStackCursor->value] = value;
        break;
    case WORLD_SCRIPT_OPERAND_WORD:
        bytePtr += g_WorldScriptStackCursor->value;
        if ((int)bytePtr & 1) {
            WorldScript_ReportError(0x3F);
        }
        *(short *)bytePtr = value;
        break;
    default:
        WorldScript_ReportError(0x40);
        break;
    }
    g_WorldScriptStackCursor->value = value;
    g_WorldScriptStackCursor->kind = WORLD_SCRIPT_OPERAND_CONSTANT;
    g_WorldScriptStackCursor++;
}

/*
 * For opcode low word 0x100, resets the operand pointer to g_WorldScriptOperandStack. Otherwise reports error
 * 0x3d if operand or running-entity pointers are null, but continues writing operand kind and the
 * next unsigned script word, increments the entity program counter, and advances the operand
 * pointer by eight with no capacity check.
 */
// FUNCTION: FF7 0x00764ccb
void __cdecl WorldScript_PushOperand(int opcode)
{
    if ((opcode & 0xFFFF) == WORLD_SCRIPT_OPERAND_CLEAR_STACK) {
        g_WorldScriptStackCursor = g_WorldScriptOperandStack;
    } else {
        if (g_WorldScriptStackCursor == 0 || g_pWorldExecutingActor == 0) {
            WorldScript_ReportError(0x3D);
        }
        g_WorldScriptStackCursor->kind = opcode;
        g_WorldScriptStackCursor->value = g_pWorldScriptCode[g_pWorldExecutingActor->scriptIdx];
        (g_pWorldExecutingActor->scriptIdx)++;
        g_WorldScriptStackCursor++;
    }
}

/*
 * Executes world-script control flow: 0x200 reads the next word as an absolute PC; 0x201 reads it
 * and jumps only when the popped stack value is zero. Opcode 0x203 restores PC and two state bytes
 * from a saved frame, or clears PC and +0x57 and yields when the call stack is empty. On that
 * empty return, a set g_WorldVehicleBoardedFlag requests function 1 on every entity only for the system
 * context; non-system contexts in that branch request nothing. With the flag clear it requests
 * system function 2 or the context entity function 2. Other opcodes pop an entity ID and request
 * op-0x204 on it, using the current entity for IDs at least 0x40, then yield. Returns the yield
 * flag.
 */
// FUNCTION: FF7 0x00764d59
int __cdecl WorldScript_ExecControlFlow(unsigned short op)
{
    WorldActor *ent;
    int done;
    WorldScriptCallFrame *frame;

    done = 0;
    switch (op) {
    case WORLD_SCRIPT_GOTO:
        op = g_pWorldScriptCode[g_pWorldExecutingActor->scriptIdx];
        (g_pWorldExecutingActor->scriptIdx)++;
        g_pWorldExecutingActor->scriptIdx = op;
        break;
    case WORLD_SCRIPT_GOTO_IF_FALSE:
        op = g_pWorldScriptCode[g_pWorldExecutingActor->scriptIdx];
        (g_pWorldExecutingActor->scriptIdx)++;
        if (WorldScript_PopStackValue() == 0) {
            g_pWorldExecutingActor->scriptIdx = op;
        }
        break;
    case WORLD_SCRIPT_RETURN:
        if (g_pWorldExecutingActor->scriptCallDepth > 0) {
            (g_pWorldExecutingActor->scriptCallDepth)--;
            frame = &g_pWorldExecutingActor->callStack[g_pWorldExecutingActor->scriptCallDepth];
            g_pWorldExecutingActor->scriptIdx = frame->scriptIdx;
            g_pWorldExecutingActor->waitFrames = frame->waitFrames;
            g_pWorldExecutingActor->scriptPriority = frame->scriptPriority;
        } else {
            g_pWorldExecutingActor->scriptIdx = 0;
            g_pWorldExecutingActor->scriptPriority = 0;
            if (g_WorldVehicleBoardedFlag != 0) {
                if (g_pWorldExecutingActor == &g_WorldSystemActor) {
                    for (ent = g_pWorldActiveActors; ent != 0; ent = ent->next) {
                        WorldScript_RequestOnEntity(ent->actorType, 1);
                    }
                }
            } else if (g_pWorldExecutingActor == &g_WorldSystemActor) {
                WorldScript_RequestSystem(2);
            } else {
                WorldScript_RequestOnEntity(g_pWorldExecutingActor->actorType, 2);
            }
            done = 1;
        }
        break;
    default:
        g_pWorldExecutingActor->scriptCallModel = WorldScript_PopStackValue();
        if (g_pWorldExecutingActor->scriptCallModel < 0x40) {
            WorldScript_RequestOnEntity(g_pWorldExecutingActor->scriptCallModel, op - 0x204);
        } else {
            g_pWorldExecutingActor->scriptCallModel = g_pWorldCurrentActor->actorType;
            WorldScript_RequestOnCurrentEntity(op - 0x204);
        }
        done = 1;
        break;
    }
    return done;
}

/*
 * Dispatches world-script command opcodes, popping operands in their original order to
 * select/create actors, attach a riding actor, set control/model/speed/facing/animation/position
 * flags, copy or save positions, configure zones and fog, request battle/field transitions, set
 * progress/encounters and invoke vehicle/terrain helpers. Temporarily selecting another actor for
 * angle, position-copy or script-wait operations restores the prior current actor. Message
 * commands set rectangle/mode, show/ask, and rewind the script index while waiting for an answer,
 * closure or open state when the global bypass flag is clear; other wait opcodes similarly poll
 * script priority or terrain effects. Opcode 0x306 rewinds while a frame latch is set; otherwise
 * decrements waitFrames, moves by rotated horizontal speed (scaled by sixteen when flag 0x40 is
 * set), changes vertical offset/position and returns whether waitFrames remains positive. Initial
 * zero waitFrames becomes -1 rather than completing. Ordinary and unknown opcodes return 0.
 * Reports missing current actor but continues dispatch.
 */
// FUNCTION: FF7 0x00764f9c
int __cdecl WorldScript_ExecCommand(unsigned short wOpcode)
{
    struct {
        VECTOR currentPos;
        short moveStep[4];
        int dwModelType;
        int dwOp3;
        WorldActor *savedActor;
        int dwOp2;
        int dwOp1;
        short windowRect[4];
    } frame; // Stack frame layout: see FRAME UNION in lib/codegen.h.

    /* Unprototyped retail call signature: api/wm.h declares narrower integers (and int for
       the two positions), which would convert the arguments. */
    void __cdecl World_SetEffectEmitterRecord(int, int, int, int);
    /* Unprototyped retail call signature (parameter width differs from callee definition). */
    void __cdecl WorldMsg_SetWindowMode(int, int);
    /* Unprototyped retail call signature (parameter width differs from callee definition). */
    void __cdecl WorldMsg_Ask(short, short, short);

    if (g_pWorldCurrentActor == 0) {
        WorldScript_ReportError(0x43);
    }
    switch (wOpcode) {
    case WORLD_SCRIPT_SPAWN_ENTITY:
        frame.dwModelType = WorldScript_PopStackValue();
        if (!WorldEntity_SetCurrentById(frame.dwModelType)) {
            WorldEntity_Alloc();
            WorldEntity_SetupForModel(frame.dwModelType);
            WorldScript_RequestOnEntity(frame.dwModelType, 0);
        }
        break;
    case WORLD_SCRIPT_MOUNT_ENTITY:
        World_PushVehicleModel(g_pWorldCurrentActor->actorType);
        frame.savedActor = g_pWorldCurrentActor;
        WorldEntity_Unlink(frame.savedActor);
        frame.dwModelType = WorldScript_PopStackValue();
        if (!WorldEntity_SetCurrentById(frame.dwModelType)) {
            WorldEntity_Alloc();
            WorldEntity_SetupForModel(frame.dwModelType);
        }
        g_pWorldCurrentActor->riding = frame.savedActor;
        g_pWorldCurrentActor->flags1 |= 2;
        break;
    case WORLD_SCRIPT_SET_PLAYER_ENTITY:
        WorldEntity_SetPlayerControlled();
        World_SetPreviousPlayerDirectionFromFacing(g_pWorldPlayerActor->direction);
        break;
    case WORLD_SCRIPT_SELECT_ENTITY:
        WorldEntity_SetCurrentById(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_MODEL_ID:
        g_pWorldCurrentActor->actorType = WorldScript_PopStackValue();
        break;
    case WORLD_SCRIPT_SET_PLAYER_CONTROL:
        WorldMap_SetPlayerControl(WorldScript_PopStackValue(), 3);
        break;
    case WORLD_SCRIPT_SET_SPEED_X16:
        g_pWorldCurrentActor->horizontalSpeed = WorldScript_PopStackValue();
        g_pWorldCurrentActor->flags1 |= 0x40;
        break;
    case WORLD_SCRIPT_SET_SPEED:
        g_pWorldCurrentActor->horizontalSpeed = WorldScript_PopStackValue();
        g_pWorldCurrentActor->flags1 &= ~0x40;
        break;
    case WORLD_SCRIPT_SET_DIRECTION:
        g_pWorldCurrentActor->facing =
            g_pWorldCurrentActor->direction = (WorldScript_PopStackValue() << 4) & 0xfff;
        if (g_pWorldCurrentActor == g_pWorldPlayerActor) {
            World_SetPreviousPlayerDirectionFromFacing(g_pWorldCurrentActor->direction);
        }
        break;
    case WORLD_SCRIPT_SET_FACING:
        g_pWorldCurrentActor->facing = (WorldScript_PopStackValue() << 4) & 0xfff;
        break;
    case WORLD_SCRIPT_FACE_ENTITY:
        frame.savedActor = g_pWorldCurrentActor;
        frame.dwOp1 = WorldScript_PopStackValue() << 4;
        if (g_pWorldCurrentActor && WorldEntity_SetCurrentById(WorldScript_PopStackValue())) {
            frame.savedActor->direction =
                WorldMap_AngleBetweenPositions(&frame.savedActor->pos, &g_pWorldCurrentActor->pos) + frame.dwOp1;
        }
        g_pWorldCurrentActor = frame.savedActor;
        break;
    case WORLD_SCRIPT_SAVE_POSITION:
        frame.dwOp1 = WorldScript_PopStackValue();
        if (frame.dwOp1 >= 0 && frame.dwOp1 < 3) {
            g_WorldScriptSavedPositions[frame.dwOp1] = g_pWorldCurrentActor->pos;
            g_WorldScriptSavedPositions[frame.dwOp1].vy = 0;
        }
        break;
    case WORLD_SCRIPT_FACE_SAVED_POSITION:
        frame.dwOp1 = WorldScript_PopStackValue();
        if (frame.dwOp1 >= 0 && frame.dwOp1 < 3) {
            g_pWorldCurrentActor->facing =
                g_pWorldCurrentActor->direction =
                    WorldMap_AngleBetweenPositions(&g_pWorldCurrentActor->pos,
                                                   &g_WorldScriptSavedPositions[frame.dwOp1]);
        }
        break;
    case WORLD_SCRIPT_SET_MESH_XZ:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        g_pWorldCurrentActor->pos.vx &= 0x1fff;
        g_pWorldCurrentActor->pos.vx |= frame.dwOp1 << 13;
        g_pWorldCurrentActor->pos.vz &= 0x1fff;
        g_pWorldCurrentActor->pos.vz |= frame.dwOp2 << 13;
        if (g_pWorldCurrentActor->pos.vx != g_pWorldCurrentActor->altPos.vx ||
            g_pWorldCurrentActor->pos.vz != g_pWorldCurrentActor->altPos.vz) {
            g_pWorldCurrentActor->flags1 |= 1;
        }
        break;
    case WORLD_SCRIPT_SET_MESH_OFFSET_XZ:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        g_pWorldCurrentActor->pos.vx &= ~0x1fff;
        g_pWorldCurrentActor->pos.vx |= frame.dwOp1 & 0x1fff;
        g_pWorldCurrentActor->pos.vz &= ~0x1fff;
        g_pWorldCurrentActor->pos.vz |= frame.dwOp2 & 0x1fff;
        if (g_pWorldCurrentActor->pos.vx != g_pWorldCurrentActor->altPos.vx ||
            g_pWorldCurrentActor->pos.vz != g_pWorldCurrentActor->altPos.vz) {
            g_pWorldCurrentActor->flags1 |= 1;
        }
        break;
    case WORLD_SCRIPT_COPY_POSITION_FROM:
        frame.savedActor = g_pWorldCurrentActor;
        if (frame.savedActor && WorldEntity_SetCurrentById(WorldScript_PopStackValue())) {
            frame.savedActor->pos = g_pWorldCurrentActor->pos;
            frame.savedActor->altPos = g_pWorldCurrentActor->altPos;
            frame.savedActor->flags1 |= 1;
        }
        g_pWorldCurrentActor = frame.savedActor;
        break;
    case WORLD_SCRIPT_SET_ANIMATION:
        if (WorldScript_PopStackValue()) {
            g_pWorldCurrentActor->flags1 &= ~0x20;
        } else {
            g_pWorldCurrentActor->flags1 |= 0x20;
        }
        g_pWorldCurrentActor->animId = WorldScript_PopStackValue();
        g_pWorldCurrentActor->animFrame = 0;
        break;
    case WORLD_SCRIPT_PLACE_PLAYER_AT_VEHICLE_RETURN:
        WorldEntity_SetPlayerPositionAndGround(&g_WorldVehicleReturnPosition);
        break;
    case WORLD_SCRIPT_SET_Y_OFFSET_SPEED:
        g_pWorldCurrentActor->verticalSpeed = WorldScript_PopStackValue();
        break;
    case WORLD_SCRIPT_SET_Y_OFFSET:
        g_pWorldCurrentActor->yOffset = WorldScript_PopStackValue();
        break;
    case WORLD_SCRIPT_SET_ALTITUDE_SPEED:
        g_pWorldCurrentActor->altitudeSpeed = WorldScript_PopStackValue();
        g_pWorldCurrentActor->flags1 |= 0x80;
        break;
    case WORLD_SCRIPT_SET_ALTITUDE:
        g_pWorldCurrentActor->pos.vy = WorldScript_PopStackValue();
        g_pWorldCurrentActor->flags1 |= 0x80;
        break;
    case WORLD_SCRIPT_REVERT_POSITION:
        WorldEntity_RevertCurrentPosition();
        break;
    case WORLD_SCRIPT_BOARD_VEHICLE:
        if (WorldEntity_IsCurrentNonPlayerRequestable()) {
            World_UpdateVehicleTransition(1);
        }
        break;
    case WORLD_SCRIPT_FREE_ENTITY:
        WorldEntity_FreeCurrent();
        break;
    case WORLD_SCRIPT_CLEAR_ACTOR_FLAG_8:
        g_pWorldCurrentActor->flags1 &= ~8;
        break;
    case WORLD_SCRIPT_SET_ACTOR_FLAG_8:
        g_pWorldCurrentActor->flags1 |= 8;
        break;
    case WORLD_SCRIPT_FADE_IN:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        World_BeginFadeFromBlack(frame.dwOp1, frame.dwOp2);
        break;
    case WORLD_SCRIPT_FADE_OUT:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        World_BeginFadeToBlack(frame.dwOp1, frame.dwOp2);
        break;
    case WORLD_SCRIPT_ZONE_SELECT:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldZone_Select(frame.dwOp1, frame.dwOp2);
        break;
    case WORLD_SCRIPT_ZONE_SET_MESH_XZ:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldZone_SetMeshXZ(frame.dwOp1, frame.dwOp2);
        break;
    case WORLD_SCRIPT_ZONE_SET_OFFSET_XZ:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldZone_SetOffsetXZ(frame.dwOp1, frame.dwOp2);
        break;
    case WORLD_SCRIPT_ZONE_SET_AMBIENT:
        frame.dwOp3 = WorldScript_PopStackValue();
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldZone_SetAmbientRGB(frame.dwOp1, frame.dwOp2, frame.dwOp3);
        break;
    case WORLD_SCRIPT_ZONE_SET_RADII:
        WorldZone_SetInnerRadius(WorldScript_PopStackValue());
        WorldZone_SetOuterRadius(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_ZONE_SET_FOG_NEAR:
        frame.dwOp3 = WorldScript_PopStackValue();
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldZone_SetFogNearRGB(frame.dwOp1, frame.dwOp2, frame.dwOp3);
        break;
    case WORLD_SCRIPT_ZONE_SET_FOG_FAR:
        frame.dwOp3 = WorldScript_PopStackValue();
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldZone_SetFogFarRGB(frame.dwOp1, frame.dwOp2, frame.dwOp3);
        break;
    case WORLD_SCRIPT_SET_CAMERA_ZOOM_STATE:
        World_SetCameraZoomState(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_CAMERA_FRONT:
        World_SetCameraFront(WorldScript_PopStackValue() << 4);
        break;
    case WORLD_SCRIPT_SET_CAMERA_FRONT_NOW:
        World_SetCameraFrontAndRotationY(WorldScript_PopStackValue() << 4);
        break;
    case WORLD_SCRIPT_SET_CAMERA_YAW_STEP:
        World_SetCameraYawStep(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_CAMERA_HEIGHT_STEP:
        World_SetCameraHeightStep(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_CAMERA_DISTANCE_STEP:
        World_SetCameraDistanceStep(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_PROJECTION_JITTER:
        World_SetProjectionJitterEnabled(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_PLAY_SFX:
        World_PlaySfxAndArmScreenFlash(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_INIT_ZOLOM_TRAIL:
        World_BuildZolomTrailOnce();
        break;
    case WORLD_SCRIPT_START_BATTLE:
        World_SetMusicChangeGate(1);
        World_SwitchMusicTrack(4);
        WorldMap_RequestBattle(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_START_COUNTDOWN_TIMER:
        World_StartCountdownTimer(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_ENCOUNTERS_ENABLED:
        World_SetEncountersEnabled(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_GOTO_FIELD:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldMap_RequestFieldTransition(frame.dwOp1 << 8 | frame.dwOp2);
        break;
    case WORLD_SCRIPT_GOTO_FIELD_ENTRY:
        World_BeginScriptFieldExit(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_CAMERA_MODE:
        World_SetCameraModeAndMiniMapState(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_WAIT_FRAMES:
        g_pWorldExecutingActor->waitFrames = WorldScript_PopStackValue();
        break;
    case WORLD_SCRIPT_ARM_VEHICLE_EXIT:
        World_ArmVehicleExit();
        break;
    case WORLD_SCRIPT_ASCEND_EXIT:
        World_BeginSubmarineSurfacing();
        break;
    case WORLD_SCRIPT_DETACH_RIDER_AS_PLAYER:
        WorldEntity_DetachRiderAsPlayer();
        break;
    case WORLD_SCRIPT_SET_SCRIPT_REQUESTS_ENABLED:
        if (WorldScript_PopStackValue()) {
            g_pWorldCurrentActor->flags1 &= ~0x10;
        } else {
            g_pWorldCurrentActor->flags1 |= 0x10;
        }
        break;
    case WORLD_SCRIPT_DISABLE_SCRIPT_REQUESTS:
        g_pWorldCurrentActor->flags1 |= 0x10;
        World_SetTerrainEffectKind(3);
        break;
    case WORLD_SCRIPT_SET_VEHICLE_TRANSITION_ARG:
        World_SetVehicleTransitionArgument(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_BOAT_TERRAIN_MASK:
        frame.dwOp1 = WorldScript_PopStackValue();
        World_SetChocoboTerrainSet(frame.dwOp1);
        World_SetModelRecordTintFromTable(WORLD_MODEL_CHOCOBO, frame.dwOp1);
        break;
    case WORLD_SCRIPT_SET_MODEL13_TINT:
        World_SetModelRecordTintFromTable(WORLD_MODEL_SUBMARINE, WorldScript_PopStackValue() + 5);
        break;
    case WORLD_SCRIPT_FACE_ZONE:
        if (g_pWorldCurrentActor) {
            WorldZone_Select(WorldScript_PopStackValue(), -1);
            WorldEntity_GetCurrentPosition(&frame.currentPos);
            g_pWorldCurrentActor->facing =
                g_pWorldCurrentActor->direction =
                    g_pWorldCurrentActor->heading = WorldZone_AngleFrom(&frame.currentPos);
            g_pWorldCurrentActor->headingOffset = 0;
        }
        break;
    case WORLD_SCRIPT_SET_WORLD_PROGRESS:
        World_SetWorldProgress(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SPAWN_ACTOR_EFFECT:
        World_SpawnActorEffectsByKind(g_pWorldCurrentActor, WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_EFFECT_EMITTER:
        frame.dwOp3 = WorldScript_PopStackValue();
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        if (g_pWorldCurrentActor) {
            World_SetEffectEmitterRecord(frame.dwOp1, g_pWorldCurrentActor->actorType, frame.dwOp2, frame.dwOp3);
        }
        break;
    case WORLD_SCRIPT_DISABLE_EFFECT_EMITTER:
        World_DisableEffectEmitterRecord(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_METEOR_VISIBLE:
        World_SetMeteorSpriteEnabled(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_PLAY_MUSIC:
        World_SetMusicChangeGate(1);
        frame.dwOp1 = WorldScript_PopStackValue();
        World_SwitchMusicTrack(frame.dwOp1);
        if (frame.dwOp1) {
            World_SetMusicChangeGate(0);
        }
        break;
    case WORLD_SCRIPT_SET_MIDI_VOLUME:
        World_SetMidiVolume(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_SET_WINDOW_RECT:
        frame.windowRect[3] = WorldScript_PopStackValue();
        frame.windowRect[2] = WorldScript_PopStackValue();
        frame.windowRect[1] = WorldScript_PopStackValue();
        frame.windowRect[0] = WorldScript_PopStackValue();
        WorldMsg_SetWindowRect(frame.windowRect);
        break;
    case WORLD_SCRIPT_SET_WINDOW_MODE:
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldMsg_SetWindowMode(frame.dwOp1, frame.dwOp2);
        break;
    case WORLD_SCRIPT_SHOW_MESSAGE:
        WorldMsg_Show(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_ASK_CHOICE:
        frame.dwOp3 = WorldScript_PopStackValue();
        frame.dwOp2 = WorldScript_PopStackValue();
        frame.dwOp1 = WorldScript_PopStackValue();
        WorldMsg_Ask(frame.dwOp1, frame.dwOp2, frame.dwOp3);
        break;
    case WORLD_SCRIPT_WAIT_ANSWER:
        if (WorldMsg_GetAnswer() < 0 && g_WorldVehicleBoardedFlag == 0) {
            g_pWorldCurrentActor->scriptIdx--;
            return 1;
        }
        break;
    case WORLD_SCRIPT_CLOSE_WINDOW:
        if (g_WorldVehicleBoardedFlag == 0) {
            frame.dwOp1 = WorldMsg_RequestCloseAndPoll();
            g_pWorldCurrentActor->scriptIdx -= frame.dwOp1;
            return frame.dwOp1;
        }
        break;
    case WORLD_SCRIPT_WAIT_WINDOW:
        if (g_WorldVehicleBoardedFlag == 0) {
            frame.dwOp1 = WorldMsg_IsOpen();
            g_pWorldCurrentActor->scriptIdx -= frame.dwOp1;
            return frame.dwOp1;
        }
        break;
    case WORLD_SCRIPT_WAIT_CALLED_SCRIPT:
        if (g_WorldVehicleBoardedFlag == 0) {
            frame.savedActor = g_pWorldCurrentActor;
            WorldEntity_SetCurrentById(g_pWorldExecutingActor->scriptCallModel);
            frame.dwOp1 = g_pWorldCurrentActor->scriptPriority > 0;
            frame.savedActor->scriptIdx -= frame.dwOp1;
            g_pWorldCurrentActor = frame.savedActor;
            return frame.dwOp1;
        }
        break;
    case WORLD_SCRIPT_START_FLOOD_FILL:
        World_RequestTerrainFloodFill();
        break;
    case WORLD_SCRIPT_WAIT_FLOOD_FILL:
        if (g_WorldVehicleBoardedFlag == 0) {
            frame.dwOp1 = !World_GetTerrainFloodFillOutcome();
            g_pWorldExecutingActor->scriptIdx -= frame.dwOp1;
            return frame.dwOp1;
        }
        break;
    case WORLD_SCRIPT_PLACE_AT_FLOOD_FILL_RESULT:
        World_StartTerrainEffectAtPathRecord(WorldScript_PopStackValue());
        break;
    case WORLD_SCRIPT_WAIT_TERRAIN_EFFECT:
        if (g_WorldVehicleBoardedFlag == 0) {
            frame.dwOp1 = World_IsTerrainEffectActive();
            g_pWorldExecutingActor->scriptIdx -= frame.dwOp1;
            return frame.dwOp1;
        }
        break;
    case WORLD_SCRIPT_STEP:
        if (g_WorldWaitFramesZero) {
            g_pWorldExecutingActor->scriptIdx--;
            return 1;
        }
        g_pWorldExecutingActor->waitFrames--;
        if (g_pWorldExecutingActor->waitFrames) {
            g_pWorldExecutingActor->scriptIdx--;
        } else {
            g_WorldWaitFramesZero = 1;
        }
        frame.moveStep[0] =
            frame.moveStep[1] = 0;
        frame.moveStep[2] =
            g_pWorldCurrentActor->horizontalSpeed << (((g_pWorldCurrentActor->flags1 & 0x40) ? 1 : 0) << 2);
        WorldMap_RotateVec3ByYaw((SVECTOR *)frame.moveStep, g_pWorldCurrentActor->facing);
        WorldEntity_MoveCurrentBy(frame.moveStep[0], frame.moveStep[2]);
        g_pWorldCurrentActor->yOffset -= g_pWorldCurrentActor->verticalSpeed;
        g_pWorldCurrentActor->pos.vy += g_pWorldCurrentActor->altitudeSpeed;
#undef WorldScript_PopStackValue
        return g_pWorldExecutingActor->waitFrames > 0;
    }
    return 0;
}

/*
 * When script transition state and player flag 8 permit, tests ground eligibility using altitude
 * for mask 0x2000 or flag 0x80 otherwise. For region IDs at least three, requests a mesh script
 * only on region change and reverts position in region seven for actors outside that model mask;
 * lower IDs clear the remembered region.
 */
// FUNCTION: FF7 0x00765f61
void __cdecl WorldMap_PollMeshScriptTrigger(void)
{
    int eligible;
    int regionId;

    if (g_WorldVehicleBoardedFlag == 0 && (g_pWorldPlayerActor->flags1 & 8) == 0) {
        if (WorldEntity_IsPlayerModelInMask(0x2000)) {
            eligible = (g_pWorldPlayerActor->pos.vy - g_pWorldPlayerActor->groundY) < 500;
        } else {
            eligible = (g_pWorldPlayerActor->flags1 & 0x80) == 0;
        }
        if (eligible) {
            regionId = WorldEntity_GetPlayerRegionId();
            if (regionId >= 3) {
                if (g_WorldMeshTriggerCounter != regionId) {
                    g_WorldMeshTriggerCounter = regionId;
                    WorldScript_RequestMeshScript(regionId - 3, 0);
                }
                if (regionId == 7 && !WorldEntity_IsPlayerModelInMask(0x2000)) {
                    WorldEntity_RevertPlayerPosition();
                }
            } else {
                g_WorldMeshTriggerCounter = 0;
            }
        }
    }
}

/*
 * Returns whether the special actor at g_WorldSystemActor has a nonzero scriptIdx or any actor
 * in g_pWorldActiveActors has nonzero scriptIdx; stops the list scan once found.
 */
// FUNCTION: FF7 0x0076603f
int __cdecl WorldScript_AnyScriptRunning(void)
{
    int running;
    WorldActor *pActor;

    running = 0;
    running |= g_WorldSystemActor.scriptIdx ? 1 : 0;
    for (pActor = g_pWorldActiveActors; pActor && running == 0; pActor = pActor->next) {
        running |= pActor->scriptIdx ? 1 : 0;
    }
    return running;
}

/*
 * Write every actor that is not marked transient (bit 8 of
 * flags1) into its vehicle slot, so the world map can be restored on reload.
 */
// FUNCTION: FF7 0x0076609e
void __cdecl World_SaveAllVehicleSlots(void)
{
    WorldActor *ent;

    for (ent = g_pWorldActiveActors; ent != 0; ent = ent->next) {
        if ((ent->flags1 & 8) == 0) {
            WorldEntity_SaveToVehicleSlot(ent);
        }
    }
}

/*
 * Pack one actor into the eight-byte vehicle slot its model maps to. The model-to-slot table is at
 * g_WorldModelToVehicleSlotTable and a model that maps to 6 or above has no slot and
 * is dropped.
 *
 * Two packed words, and the field widths are the reason the world map's
 * vehicle positions are as coarse as they are:
 *
 * +0  x           19 bits    model  5 bits    facing  8 bits
 * +4  z           18 bits    y     14 bits
 *
 * The masks are written as they appear in the image rather than derived from
 * the shifts, because `<< 0x13 & 0xF80000` and `& 0x1F << 19` do not
 * generate the same instructions at /Od.
 */
// FUNCTION: FF7 0x007660db
void __cdecl WorldEntity_SaveToVehicleSlot(WorldActor *ent)
{
    WorldVehicleSlot *slot;

    if (*(unsigned char *)(g_WorldModelToVehicleSlotTable + ent->actorType) < 6) {
        slot = &g_pWorldVehicleSlotTable[*(unsigned char *)(g_WorldModelToVehicleSlotTable + ent->actorType)];
        slot->packedXTypeDirection = ent->altPos.vx & 0x7FFFF | ent->actorType << 0x13 & 0xF80000 | ent->direction << 0x14 & 0xFF000000;
        slot->packedYAndZ = ent->altPos.vz & 0x3FFFF | ent->altPos.vy << 0x12 & 0xFFFC0000;
    }
}

/*
 * Searches the six packed eight-byte vehicle slots at g_pWorldVehicleSlotTable for actorType, then restores
 * current/alternate XYZ and facing/direction fields from the packed words and clears the heading offset. The
 * original search reads actorType before its later null check.
 */
// FUNCTION: FF7 0x0076616a
void __cdecl WorldEntity_RestoreFromVehicleSlot(WorldActor *actor)
{
    WorldVehicleSlot *slot;

    for (slot = g_pWorldVehicleSlotTable;
         slot < g_pWorldVehicleSlotTable + 6 &&
         (((slot->packedXTypeDirection) >> 19) & 0x1F) != actor->actorType;
         slot++) {
    }
    if (slot < g_pWorldVehicleSlotTable + 6 && actor) {
        actor->pos.vx = actor->altPos.vx = slot->packedXTypeDirection & 0x7FFFF;
        actor->pos.vy = actor->altPos.vy = slot->packedYAndZ >> 18;
        actor->pos.vz = actor->altPos.vz = slot->packedYAndZ & 0x3FFFF;
        actor->facing = actor->heading = actor->direction =
            (short)((slot->packedXTypeDirection >> 20) & 0xFF0);
        actor->headingOffset = 0;
    }
}

/*
 * Clears two removed-member and two added-member registers, then derives their values from old/new
 * party-mask differences. Prioritizes bit 0x40 as value 0x26, then scans remaining bits from low
 * to high as bit index plus 0x20, retaining at most two values in each register pair.
 */
// FUNCTION: FF7 0x00766255
void __cdecl WorldScript_SetPartyChangeRegisters(int dwCharMask_old, int dwCharMask_new)
{
    struct {
        unsigned slotCount;
        int bitIndex;
        unsigned dwDiff;
        int leaderMask;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_WorldVehicleModelStack.pendingModel = 0;
    (g_WorldVehicleModelStack.currentModel) = g_WorldVehicleModelStack.pendingModel;
    g_WorldVehicleSlotStack.pendingSlot = 0;
    (g_WorldVehicleSlotStack.currentSlot) = g_WorldVehicleSlotStack.pendingSlot;

    frame.leaderMask = 0x40;
    frame.dwDiff = dwCharMask_old & ~dwCharMask_new;
    frame.slotCount = 0;
    if (frame.dwDiff & frame.leaderMask) {
        g_WorldVehicleSlotStack.slots[frame.slotCount] = 0x26;
        frame.slotCount++;
        frame.dwDiff &= ~frame.leaderMask;
    }
    for (frame.bitIndex = 0; frame.slotCount < 2 && frame.dwDiff; frame.bitIndex++, frame.dwDiff >>= 1) {
        if (frame.dwDiff & 1) {
            g_WorldVehicleSlotStack.slots[frame.slotCount] = frame.bitIndex + 0x20;
            frame.slotCount++;
        }
    }

    frame.dwDiff = ~dwCharMask_old & dwCharMask_new;
    frame.slotCount = 0;
    if (frame.dwDiff & frame.leaderMask) {
        g_WorldVehicleModelStack.slots[frame.slotCount] = 0x26;
        frame.slotCount++;
        frame.dwDiff &= ~frame.leaderMask;
    }
    for (frame.bitIndex = 0; frame.slotCount < 2 && frame.dwDiff; frame.bitIndex++, frame.dwDiff >>= 1) {
        if (frame.dwDiff & 1) {
            g_WorldVehicleModelStack.slots[frame.slotCount] = frame.bitIndex + 0x20;
            frame.slotCount++;
        }
    }
}

/*
 * Copies the supplied 16-byte VECTOR into the vehicle-return position
 * g_WorldVehicleReturnPosition.
 */
// FUNCTION: FF7 0x007663a8
void __cdecl World_SetVehicleReturnPosition(VECTOR *v)
{
    g_WorldVehicleReturnPosition = *v;
}
} /* extern "C" */

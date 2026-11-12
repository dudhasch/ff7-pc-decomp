// src/ff7/coaster/coaster_actor_pool_spawn.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Actor pool for the coaster minigame: resets and allocates actor slots, spawns and
//   despawns actors from scripts and templates, updates them, and applies beam damage and kill
//   awards.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/crt.h"
#include <time.h>
#include "ff7/enums/coaster.h"

extern "C" {

// clang-format off
// GLOBAL: FF7 0x00c3f918
short g_CoasterCarScreenX = 0;
// GLOBAL: FF7 0x00c3f91c
int g_CoasterSpawnScriptCursor = 0;
// GLOBAL: FF7 0x00c3f920
short g_CoasterCarPoint2ScreenX = 0;
// GLOBAL: FF7 0x00c3f924
short g_CoasterCarPoint2ScreenY = 0;
// GLOBAL: FF7 0x00c3f928
short g_CoasterCarScreenY = 0;
unsigned char g_bssPad_00c3f92c[4] = {0};
// The actor image Mini_CoasterSpawnActor copies into a free slot.
// GLOBAL: FF7 0x00c3f930
CoasterActor g_CoasterSpawnRequestBlock = {0};
// GLOBAL: FF7 0x00c3fa6c
short g_CoasterFreeActorSlotHead = 0;
// GLOBAL: FF7 0x00c3fa70
signed char g_CoasterShotFlag = 0;
// GLOBAL: FF7 0x00c3fa74
char g_CoasterShotCooldown = 0;
// GLOBAL: FF7 0x00c3fa78
short g_CoasterCrosshairX = 0;
// GLOBAL: FF7 0x00c3fa7c
short g_CoasterCrosshairY = 0;
// The free list of actor slots: each entry is the next free slot after it.
// GLOBAL: FF7 0x00c3fa80
short g_CoasterActorFreeList[100] = {0};
// GLOBAL: FF7 0x00c3fb48
short g_CoasterShotCharge = 0;
// GLOBAL: FF7 0x00c3fb4c
short g_CoasterChargeCenterX = 0;
// GLOBAL: FF7 0x00c3fb50
short g_CoasterBeamCharge = 0;
// GLOBAL: FF7 0x00c3fb54
short g_CoasterActorCount = 0;
// GLOBAL: FF7 0x00c3fb58
short g_CoasterReticleX = 0;
// GLOBAL: FF7 0x00c3fb5c
short g_CoasterReticleY = 0;
// GLOBAL: FF7 0x00c3fb60
SVECTOR *g_pCoasterPath = 0;
unsigned char g_bssPad_00c3fb64[4] = {0};
// The 100-element CoasterActor pool spans 0x00c3fb68..0x00c476d8 (100 * 0x13C bytes). Retail COFF
// relocations address interior offsets +0xD4 (node) and +0xD8 (objIndex) via separate BSS symbols
// g_CoasterActorSceneObjectBase and g_CoasterActorRecords, preserved here for object-level identity.
// GLOBAL: FF7 0x00c3fb68
int g_CoasterActorPool = 0;
unsigned char g_bssPad_00c3fb6c[4] = {0};
unsigned char g_bssPad_00c3fb70[204] = {0};
// GLOBAL: FF7 0x00c3fc3c
int g_CoasterActorSceneObjectBase = 0;
// GLOBAL: FF7 0x00c3fc40
unsigned char g_CoasterActorRecords[31288] = {0};
unsigned char g_bssPad_00c47678[96] = {0};
// GLOBAL: FF7 0x00c476d8
int g_CoasterSpawnScriptIndex = 0;
// GLOBAL: FF7 0x00c476dc
char g_CoasterBeamIndex = 0;
// GLOBAL: FF7 0x00c476e0
int g_CoasterPathLength = 0;
unsigned char g_bssPad_00c476e4[4] = {0};
// clang-format on


/*
 * Resets the coaster actor pool, free list, targeting crosshairs, and shot charge.
 */
// FUNCTION: FF7 0x005eab70
void __cdecl Mini_CoasterResetActorPool(void)
{
    int i;

    for (i = 0; i < 100; i++) {
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[i].objIndex = 0xFFFF;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[i].isActive = 0;
    }
    g_CoasterFreeActorSlotHead = 0;
    for (i = 0; i < 100; i++) {
        g_CoasterActorFreeList[i] = i + 1;
    }
    g_CoasterBeamCharge = 0x80;
    g_CoasterShotFlag = 0;
    g_CoasterShotCooldown = 0;
    g_CoasterReticleX = 160;
    g_CoasterReticleY = 120;
    g_CoasterSpawnScriptCursor = 0;
    g_CoasterSpawnScriptIndex = 0;
    g_CoasterActorCount = 0;
}

/*
 * Resolves path `modelIndex` from the coaster PSX data set: its length into g_CoasterPathLength
 * and its points into g_pCoasterPath, from the data set's length, offset and point blocks (0xD,
 * 0xC and 0xB in mode 0; 6, 5 and 4 in mode 1). The spawn code copies both into the actor
 * (pathLength, path).
 */
// FUNCTION: FF7 0x005eac30
void __cdecl Mini_CoasterResolvePath(unsigned char modelIndex, int mode)
{
    int entOffset;
    int *animTable;
    int *modelBase;
    int modelOffset;
    int stateOffsetVal;
    int animOffset;
    int modelOfs;
    char *baseAddr;
    int animationOffset;
    char *resolvedBase;
    int stateOffset;

    if (mode == 0) {
        modelOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(0xD) << 2));
        if (modelOffset >= g_CoasterPsxDataSet.binSize) {
            modelBase = 0;
        } else {
            modelBase = (int *)(g_CoasterPsxDataSet.binData + modelOffset);
        }
        animOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(0xC) << 2));
        if (animOffset >= g_CoasterPsxDataSet.binSize) {
            animTable = 0;
        } else {
            animTable = (int *)(g_CoasterPsxDataSet.binData + animOffset);
        }
        g_CoasterPathLength = modelBase[modelIndex];
        entOffset = animTable[modelIndex];
        stateOffsetVal = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(0xB) << 2));
        if (stateOffsetVal >= g_CoasterPsxDataSet.binSize) {
            baseAddr = 0;
        } else {
            baseAddr = (char *)(g_CoasterPsxDataSet.binData + stateOffsetVal);
        }
        g_pCoasterPath = (SVECTOR *)(baseAddr + entOffset);
    }
    if (mode == 1) {
        modelOfs = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(6) << 2));
        if (modelOfs >= g_CoasterPsxDataSet.binSize) {
            modelBase = 0;
        } else {
            modelBase = (int *)(g_CoasterPsxDataSet.binData + modelOfs);
        }
        animationOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(5) << 2));
        if (animationOffset >= g_CoasterPsxDataSet.binSize) {
            animTable = 0;
        } else {
            animTable = (int *)(g_CoasterPsxDataSet.binData + animationOffset);
        }
        g_CoasterPathLength = modelBase[modelIndex];
        entOffset = animTable[modelIndex];
        stateOffset = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(4) << 2));
        if (stateOffset >= g_CoasterPsxDataSet.binSize) {
            resolvedBase = 0;
        } else {
            resolvedBase = (char *)(g_CoasterPsxDataSet.binData + stateOffset);
        }
        g_pCoasterPath = (SVECTOR *)(resolvedBase + entOffset);
    }
}

/*
 * Samples an actor animation movement spline at the specified cursor position.
 */
// FUNCTION: FF7 0x005eade6
void __cdecl Mini_CoasterSampleAnimPath(unsigned int cursor, SVECTOR *path, VECTOR *posOut, char rawSigns)
{
    struct {
        SVECTOR point1;
        int fraction;
        SVECTOR point0;
        VECTOR increment;
        int index;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.index = (cursor & 0xFFFF0000) >> 16;
    local.fraction = cursor & 0xFFFF;
    local.point0.vx = path[local.index].vx;
    local.point0.vy = path[local.index].vy;
    local.point0.vz = path[local.index].vz;
    local.point1.vx = path[local.index + 1].vx;
    local.point1.vy = path[local.index + 1].vy;
    local.point1.vz = path[local.index + 1].vz;
    local.increment.vx = local.point1.vx - local.point0.vx;
    local.increment.vy = local.point1.vy - local.point0.vy;
    local.increment.vz = local.point1.vz - local.point0.vz;
    local.increment.vx *= local.fraction;
    local.increment.vy *= local.fraction;
    local.increment.vz *= local.fraction;
    local.increment.vx >>= 16;
    local.increment.vy >>= 16;
    local.increment.vz >>= 16;
    if (rawSigns == 0) {
        posOut->vx = path[local.index].vx + local.increment.vx;
        posOut->vy = -path[local.index].vy - local.increment.vy;
        posOut->vz = -path[local.index].vz - local.increment.vz;
    } else {
        posOut->vx = path[local.index].vx + local.increment.vx;
        posOut->vy = path[local.index].vy + local.increment.vy;
        posOut->vz = path[local.index].vz + local.increment.vz;
    }
}

/*
 * Spawns a coaster actor instance from a template record into a free slot.
 */
// FUNCTION: FF7 0x005eaf45
short __cdecl Mini_CoasterSpawnActor(CoasterActor *spawnRecord, short parentSlot)
{
    short modelIndex;
    short yLow;
    short zHigh;
    short actorSlot;
    short minZ;
    short maxX;
    short selMinX;
    short maxY;

    if (g_CoasterActorCount < 99) {
        g_CoasterActorCount++;
        actorSlot = Mini_CoasterAllocActorSlot();
        ((CoasterActor *)&g_CoasterActorPool)[actorSlot] = *spawnRecord;
        modelIndex = (short)spawnRecord->state.modelId;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].objIndex = actorSlot;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].isActive = 1;
        if (parentSlot == 0) {
            ((CoasterActorPoolNode *)&g_CoasterActorSceneObjectBase)[actorSlot].node = (CoasterNode *)Mini_CoasterCreateSceneObject((short)modelIndex, &g_CoasterRootSceneNode, spawnRecord->pos.vx, spawnRecord->pos.vy, spawnRecord->pos.vz, spawnRecord->rot.vx, spawnRecord->rot.vy, spawnRecord->rot.vz);
        } else {
            ((CoasterActorPoolNode *)&g_CoasterActorSceneObjectBase)[actorSlot].node = (CoasterNode *)Mini_CoasterCreateSceneObject((short)modelIndex, (void *)(((CoasterActorPoolNode *)&g_CoasterActorSceneObjectBase)[parentSlot].node), spawnRecord->pos.vx, spawnRecord->pos.vy, spawnRecord->pos.vz, spawnRecord->rot.vx, spawnRecord->rot.vy, spawnRecord->rot.vz);
        }
        selMinX = g_CoasterModelTemplates[modelIndex].minX;
        maxX = g_CoasterModelTemplates[modelIndex].maxX;
        yLow = g_CoasterModelTemplates[modelIndex].minY;
        maxY = g_CoasterModelTemplates[modelIndex].maxY;
        minZ = g_CoasterModelTemplates[modelIndex].minZ;
        zHigh = g_CoasterModelTemplates[modelIndex].maxZ;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].centerX = maxX + selMinX >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].centerY = maxY + yLow >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].maxZ = zHigh;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minZFaceX = maxX + selMinX >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minZFaceY = maxY + yLow >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minZ = minZ;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].maxYFaceX = maxX + selMinX >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].maxY = maxY;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].centerZ = zHigh + minZ >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minYFaceX = maxX + selMinX >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minY = yLow;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minYFaceZ = zHigh + minZ >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].maxX = maxX;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].maxXFaceY = maxY + yLow >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].maxXFaceZ = zHigh + minZ >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minX = selMinX;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minXFaceY = maxY + yLow >> 1;
        ((CoasterActorPoolBounds *)&g_CoasterActorRecords)[actorSlot].minXFaceZ = zHigh + minZ >> 1;
    }
    return (short)actorSlot;
}

/*
 * Despawns an active coaster actor and decrements active actor count.
 */
// FUNCTION: FF7 0x005eb2df
void __cdecl Mini_CoasterDespawnActor(CoasterActor *actor)
{
    if (actor->objIndex != -1) {
        g_CoasterActorCount--;
        Mini_CoasterDestroySceneObject((void *)actor->node);
        Mini_CoasterFreeActorSlot(actor->objIndex);
        actor->objIndex = -1;
        actor->isActive = 0;
    }
}

/*
 * Allocates and returns a free actor slot index from the free list.
 */
// FUNCTION: FF7 0x005eb342
short __cdecl Mini_CoasterAllocActorSlot(void)
{
    short head;

    head = g_CoasterFreeActorSlotHead;
    g_CoasterFreeActorSlotHead = g_CoasterActorFreeList[g_CoasterFreeActorSlotHead];
    return head;
}

/*
 * Returns an actor slot index back to the coaster actor free list.
 */
// FUNCTION: FF7 0x005eb36e
void __cdecl Mini_CoasterFreeActorSlot(short value)
{
    g_CoasterActorFreeList[value] = g_CoasterFreeActorSlotHead;
    g_CoasterFreeActorSlotHead = value;
}

/*
 * Processes coaster track position spawn script triggers and spawns queued actors.
 */
// FUNCTION: FF7 0x005eb391
void __cdecl Mini_CoasterRunSpawnScript(void)
{
    int trackPos;
    int ent;
    char *countPointer;
    int fieldIndex;
    CoasterSpawnScriptRecord *spawnTable;
    char *baseA;
    int tableOffsetA;
    int ofsB;

    for (trackPos = g_CoasterSpawnScriptCursor; trackPos < g_CoasterTrackCursor; trackPos++) {
        tableOffsetA = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(0xF) << 2));
        if (tableOffsetA >= g_CoasterPsxDataSet.binSize) {
            baseA = 0;
        } else {
            baseA = (char *)(g_CoasterPsxDataSet.binData + tableOffsetA);
        }
        countPointer = baseA + trackPos;
        for (ent = 0; ent < *countPointer; ent++) {
            ofsB = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(0xE) << 2));
            if (ofsB >= g_CoasterPsxDataSet.binSize) {
                spawnTable = 0;
            } else {
                spawnTable = (CoasterSpawnScriptRecord *)(g_CoasterPsxDataSet.binData + ofsB);
            }
            for (fieldIndex = 0; fieldIndex < 20; fieldIndex++) {
                g_CoasterSpawnRequestBlock.state.params[fieldIndex] = spawnTable[g_CoasterSpawnScriptIndex].params[fieldIndex];
            }
            g_CoasterSpawnRequestBlock.state.pathIndex = spawnTable[g_CoasterSpawnScriptIndex].pathIndex;
            g_CoasterSpawnRequestBlock.state.speed = spawnTable[g_CoasterSpawnScriptIndex].speed;
            Mini_CoasterSpawnScripted(0, 0, 0, spawnTable[g_CoasterSpawnScriptIndex].behaviourType, spawnTable[g_CoasterSpawnScriptIndex].modelIndex);
            g_CoasterSpawnScriptIndex++;
        }
    }
    g_CoasterSpawnScriptCursor = g_CoasterTrackCursor;
}

/*
 * Spawns an actor with specified transform coordinates, behavior, and model index.
 */
// FUNCTION: FF7 0x005eb507
void __cdecl Mini_CoasterSpawnScripted(short x, short y, short z, short behaviourType, short modelIndex)
{
    short unused;

    g_CoasterSpawnRequestBlock.pos.vx = x;
    g_CoasterSpawnRequestBlock.pos.vy = y;
    g_CoasterSpawnRequestBlock.pos.vz = z;
    g_CoasterSpawnRequestBlock.state.type = behaviourType;
    g_CoasterSpawnRequestBlock.state.mustInit = 1;
    g_CoasterSpawnRequestBlock.state.modelId = modelIndex;
    g_CoasterSpawnRequestBlock.state.isHit = 0;
    unused = (short)Mini_CoasterSpawnActor(&g_CoasterSpawnRequestBlock, 0);
}

/*
 * Spawns an effect actor with specified coordinates, behavior type, and model.
 */
// FUNCTION: FF7 0x005eb566
void __cdecl Mini_CoasterSpawnEffect(short x, short y, short z, short behaviourType, short modelIndex)
{
    short unused;

    g_CoasterSpawnRequestBlock.pos.vx = x;
    g_CoasterSpawnRequestBlock.pos.vy = y;
    g_CoasterSpawnRequestBlock.pos.vz = z;
    g_CoasterSpawnRequestBlock.state.type = behaviourType;
    g_CoasterSpawnRequestBlock.state.mustInit = 1;
    g_CoasterSpawnRequestBlock.state.modelId = modelIndex;
    g_CoasterSpawnRequestBlock.state.isHit = 0;
    g_CoasterSpawnRequestBlock.state.params[12] = 0;
    unused = (short)Mini_CoasterSpawnActor(&g_CoasterSpawnRequestBlock, 0);
}

/*
 * Updates all active coaster actors, animations, projectile collisions, and behaviors.
 */
// FUNCTION: FF7 0x005eb5cf
void __cdecl Mini_CoasterUpdateActors(void)
{
    struct {
        CoasterActor *actor;
        int deltaY;
        int newX;
        int deltaX;
        int counter;
        CoasterCharSlot rotationType;
        CoasterCharSlot isPlayerCar;
        int i;
        VECTOR nextPoint;
        CoasterShortSlot unusedShort;
        SVECTOR cameraRotation;
        CoasterUCharSlot alpha;
        int j;
        CoasterObjectState *state;
        int unusedDistance;
        VECTOR cameraPosition;
        int newZ;
        int deltaZ;
        int newY;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Mini_CoasterRunSpawnScript();
    Mini_CoasterDrawCrosshair();
    Mini_CoasterDrawBeam();

    for (local.i = 0; local.i < 100; local.i++) {
        local.actor = &((CoasterActor *)&g_CoasterActorPool)[local.i];
        local.state = &local.actor->state;
        local.unusedShort.value = 0;
        if (local.actor->isActive) {
            local.rotationType.value = 0;
            local.isPlayerCar.value = 0;
            switch (local.state->type) {
            case COASTER_BEHAVIOUR_HOMING:
                if (local.state->mustInit == 1) {
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->frameCount = 0;
                    local.state->isHit = 0;
                    local.state->params[0xA] = COASTER_KILL_SPARKS;
                    local.state->pathPos = 0;
                    local.state->stateValA = local.actor->pos.vx;
                    local.state->stateValB = local.actor->pos.vy;
                    local.state->stateValC = local.actor->pos.vz;
                    local.state->params[0] = 0;
                } else {
                    local.state->frameCount++;
                    local.state->pathPos++;
                }
                if (local.state->hp) {
                    local.counter = local.state->pathPos;
                    Mini_CoasterSampleTrack(g_CoasterRidePosition + 0x2FFFD, -100,
                                            &local.cameraPosition,
                                            &local.cameraRotation);
                    local.newX = local.state->stateValA + ((local.counter * (-local.state->stateValA + local.cameraPosition.vx)) >> 7);
                    local.newY = local.state->stateValB + ((local.counter * (-local.state->stateValB + local.cameraPosition.vy)) >> 7);
                    local.newZ = local.state->stateValC + ((local.counter * (-local.state->stateValC + local.cameraPosition.vz)) >> 7);
                    local.actor->pos.vx = local.newX;
                    local.actor->pos.vy = local.newY;
                    local.actor->pos.vz = local.newZ;
                    Mini_CoasterSampleTrack(g_CoasterRidePosition + 0x3FFFC, -100,
                                            &local.cameraPosition,
                                            &local.cameraRotation);
                    local.deltaX = local.actor->pos.vx - local.cameraPosition.vx;
                    local.deltaY = local.actor->pos.vy - local.cameraPosition.vy;
                    local.deltaZ = local.actor->pos.vz - local.cameraPosition.vz;
                    local.unusedDistance = SqrtToInt(local.deltaX * local.deltaX + local.deltaY * local.deltaY + local.deltaZ * local.deltaZ);
                    if (local.state->frameCount > 128) {
                        if (g_CoasterScore > 5)
                            g_CoasterScore -= 5;
                        else
                            g_CoasterScore = 0;
                        local.state->hp = 0;
                    }
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_PATH_DELAYED:
                if (local.state->mustInit == 1) {
                    if (local.state->params[0x11])
                        Mini_CoasterPlaySfxAlternating((short)local.state->params[0x11]);
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->frameCount = 0;
                    local.state->isHit = 0;
                    local.state->pathPos = 0;
                    local.state->stateValA = (local.actor->pathLength - 2) << 16;
                } else {
                    local.state->frameCount++;
                }
                if (local.state->params[3] < g_CoasterTrackCursor)
                    local.state->pathPos += local.state->speed;
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->hp) {
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                    if (local.state->pathPos < local.state->stateValA) {
                        Mini_CoasterSampleAnimPath(local.state->pathPos, local.actor->path,
                                                   &local.actor->pos, 0);
                        local.actor->rot.vx = 0;
                        local.actor->rot.vy = 0;
                        local.actor->rot.vz = 0;
                    }
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_PATH_SPIN:
                if (local.state->mustInit == 1) {
                    if (local.state->params[0x11])
                        Mini_CoasterPlaySfxAlternating((short)local.state->params[0x11]);
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->frameCount = 0;
                    local.state->isHit = 0;
                    local.state->pathPos = 0;
                    local.state->stateValA = (local.actor->pathLength - 1) << 16;
                } else {
                    local.state->frameCount++;
                }
                local.state->pathPos += local.state->speed;
                if (local.state->params[1] == 1)
                    local.state->pathPos %= local.state->stateValA;
                if (local.state->pathPos > local.state->stateValA)
                    local.state->hp = 0;
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->hp) {
                    Mini_CoasterSampleAnimPath(local.state->pathPos, local.actor->path,
                                               &local.actor->pos, 0);
                    local.actor->rot.vx += local.state->params[3];
                    local.actor->rot.vy += local.state->params[4];
                    local.actor->rot.vz += local.state->params[5];
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_PATH_FACING:
                if (local.state->mustInit == 1) {
                    if (local.state->params[0x11])
                        Mini_CoasterPlaySfxAlternating((short)local.state->params[0x11]);
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->frameCount = 0;
                    local.state->isHit = 0;
                    local.state->pathPos = 0;
                    local.state->stateValA = (local.actor->pathLength - 1) << 16;
                } else {
                    local.state->frameCount++;
                }
                local.state->pathPos += local.state->speed;
                if (local.state->params[1] == 1)
                    local.state->pathPos %= local.state->stateValA;
                if (local.state->pathPos > local.state->stateValA)
                    local.state->hp = 0;
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->hp) {
                    Mini_CoasterSampleAnimPath(local.state->pathPos, local.actor->path,
                                               &local.actor->pos, 0);
                    Mini_CoasterSampleAnimPath((local.state->pathPos + (1 << 16)) % ((local.actor->pathLength - 1) << 16),
                                               local.actor->path, &local.nextPoint, 0);
                    local.deltaX = local.nextPoint.vx - local.actor->pos.vx;
                    local.deltaY = local.nextPoint.vy - local.actor->pos.vy;
                    local.deltaZ = local.nextPoint.vz - local.actor->pos.vz;
                    local.actor->rot.vx = ArcTangentFixed(local.deltaY,
                                                           SqrtToInt(local.deltaX * local.deltaX + local.deltaZ * local.deltaZ));
                    local.actor->rot.vy = -ArcTangentFixed(local.deltaZ, local.deltaX) - 0x400;
                    local.actor->rot.vz = 0;
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_PLAYER_CAR:
                if (local.state->mustInit == 1) {
                    if (local.state->params[0x11])
                        Mini_CoasterPlaySfxAlternating((short)local.state->params[0x11]);
                    Mini_CoasterResolvePath(0, 1);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->frameCount = 0;
                    local.state->isHit = 0;
                    local.state->pathPos = 0;
                    local.state->stateValA = (local.actor->pathLength - 1) << 16;
                } else {
                    local.state->frameCount++;
                }
                local.state->pathPos += local.state->speed;
                if (local.state->params[1] == 1)
                    local.state->pathPos %= local.state->stateValA;
                if (local.state->pathPos > local.state->stateValA)
                    local.state->hp = 0;
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->hp) {
                    local.isPlayerCar.value = 1;
                    Mini_CoasterSampleTrack(g_CoasterRidePosition + 0x3FFFC, 10,
                                            &local.actor->pos,
                                            &local.actor->rot);
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_FALL:
                if (local.state->mustInit == 1) {
                    if (local.state->params[0x11])
                        Mini_CoasterPlaySfxAlternating((short)local.state->params[0x11]);
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->isHit = 0;
                    Mini_CoasterSampleAnimPath(0, local.actor->path, &local.actor->pos, 0);
                }
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->params[3] < g_CoasterTrackCursor)
                    local.state->stateValA += 4;
                if (local.state->hp) {
                    local.actor->pos.vy += local.state->stateValA;
                    if (local.actor->pos.vy > 0)
                        local.state->hp = 0;
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_PATH_TUMBLE:
                if (local.state->mustInit == 1) {
                    if (local.state->params[0x11])
                        Mini_CoasterPlaySfxAlternating((short)local.state->params[0x11]);
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->isHit = 0;
                    local.state->pathPos = 0;
                    local.state->stateValA = (local.actor->pathLength - 1) << 16;
                    local.actor->rot.vx = local.state->params[3];
                    local.actor->rot.vy = local.state->params[4];
                    local.actor->rot.vz = local.state->params[5];
                }
                local.state->pathPos += local.state->speed;
                if (local.state->params[1] == 1)
                    local.state->pathPos %= local.state->stateValA;
                if (local.state->pathPos > local.state->stateValA)
                    local.state->hp = 0;
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                local.actor->rot.vx += local.state->params[6];
                local.actor->rot.vy += local.state->params[7];
                local.actor->rot.vz += local.state->params[8];
                if (local.state->params[0xA] == COASTER_KILL_BIG_EXPLOSION)
                    local.actor->node->model = ((CoasterModelRecord **)&g_CoasterExplosionModel)[local.state->params[0xE]];
                if (local.state->params[0xE] == 1)
                    local.state->params[0xE] = 0;
                if (local.state->hp) {
                    Mini_CoasterSampleAnimPath(local.state->pathPos, local.actor->path,
                                               &local.actor->pos, 0);
                    if (local.state->isHit) {
                        if (local.state->params[0xA] != COASTER_KILL_BIG_EXPLOSION)
                            Mini_CoasterApplyBeamDamage(local.actor);
                        else if (g_CoasterRideSpeed < 16405)
                            Mini_CoasterApplyBeamDamage(local.actor);
                        if (local.state->params[0xA] == COASTER_KILL_BIG_EXPLOSION)
                            local.state->params[0xE] = 1;
                    }
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_PATH_SWAY:
                if (local.state->mustInit == 1) {
                    if (local.state->params[0x11])
                        Mini_CoasterPlaySfxAlternating((short)local.state->params[0x11]);
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->isHit = 0;
                    local.state->pathPos = (rand() % local.state->params[3]) * 2 - local.state->params[3] - 1;
                    local.state->stateValA = 0;
                    local.state->stateValB = 0;
                    local.state->stateValC = (local.actor->pathLength - 1) << 16;
                    local.actor->rot.vx = 0;
                    local.actor->rot.vy = 0;
                    local.actor->rot.vz = 0;
                    Mini_CoasterSampleAnimPath(0, local.actor->path, &local.actor->pos, 0);
                }
                local.state->stateValB += local.state->speed;
                if (local.state->stateValB > local.state->stateValC)
                    local.state->hp = 0;
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->stateValA > local.state->pathPos)
                    local.state->stateValA -= 5;
                if (local.state->stateValA < local.state->pathPos)
                    local.state->stateValA += 5;
                local.actor->rot.vx = local.state->stateValA;
                local.actor->rot.vy += local.state->params[4];
                if (local.state->hp) {
                    Mini_CoasterSampleAnimPath(local.state->stateValB, local.actor->path,
                                               &local.actor->pos, 0);
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_STATIC_TARGET:
                if (local.state->mustInit == 1) {
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->isHit = 0;
                }
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->hp) {
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_TILT:
            case COASTER_BEHAVIOUR_TILT_ALT:
                if (local.state->mustInit == 1) {
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->isHit = 0;
                    local.actor->rot.vx = local.state->params[3];
                    local.actor->rot.vy = local.state->params[4];
                    local.actor->rot.vz = 0;
                    Mini_CoasterSampleAnimPath(0, local.actor->path, &local.actor->pos, 0);
                    local.state->pathPos = 0;
                }
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->params[5] < g_CoasterTrackCursor && (local.state->pathPos++) < local.state->params[7])
                    local.actor->rot.vx += local.state->params[6];
                if (local.state->hp) {
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_DEBRIS_BURST:
                Mini_CoasterPlaySfxAlternating(0x8E);
                for (local.j = 0; local.j < local.state->params[3]; local.j++)
                    Mini_CoasterSpawnEffect(13382, -10000, 8395, COASTER_BEHAVIOUR_FALLING_DEBRIS, 0x2A);
                Mini_CoasterDespawnActor(local.actor);
            case COASTER_BEHAVIOUR_FALLING_DEBRIS:
                if (local.state->mustInit == 1) {
                    local.state->isHit = 0;
                    local.state->hp = 100;
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    local.state->pathPos = -30 + rand() % 60;
                    local.state->stateValA = -rand() % 200;
                    local.state->stateValB = -30 + rand() % 60;
                } else {
                    local.state->frameCount++;
                }
                local.state->stateValA++;
                local.actor->pos.vx += local.state->pathPos;
                local.actor->pos.vy += local.state->stateValA;
                local.actor->pos.vz += local.state->stateValB;
                local.actor->rot.vx += 10;
                local.actor->rot.vy += 400;
                local.actor->rot.vz += 200;
                local.state->hp--;
                if (local.state->hp == 0)
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_ROCKET:
                if (local.state->mustInit == 1) {
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->isHit = 0;
                    local.state->pathPos = 0;
                    Mini_CoasterSampleAnimPath(0, local.actor->path, &local.actor->pos, 0);
                }
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->hp) {
                    local.actor->pos.vy -= local.state->params[3];
                    local.state->params[3] -= local.state->params[4];
                    if (local.state->params[3] < 0) {
                        Mini_CoasterPlaySfxAlternating(0x98);
                        for (local.j = 0; local.j < 20; local.j++)
                            Mini_CoasterSpawnEffect((short)local.actor->pos.vx,
                                                    (short)local.actor->pos.vy, (short)local.actor->pos.vz,
                                                    COASTER_BEHAVIOUR_DEBRIS, (short)(0x44 + rand() % 3));
                        local.state->hp = 0;
                    }
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_DEBRIS:
                if (local.state->mustInit == 1) {
                    local.state->isHit = 0;
                    local.state->hp = 100;
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    local.state->pathPos = -30 + rand() % 60;
                    local.state->stateValA = -30 + rand() % 60;
                    local.state->stateValB = -30 + rand() % 60;
                } else {
                    local.state->frameCount++;
                }
                local.actor->pos.vx += local.state->pathPos;
                local.actor->pos.vy += local.state->stateValA;
                local.actor->pos.vz += local.state->stateValB;
                local.actor->rot.vx += 10;
                local.actor->rot.vy += 400;
                local.actor->rot.vz += 200;
                local.state->hp--;
                if (local.state->hp == 0)
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_LEAP:
                if (local.state->mustInit == 1) {
                    Mini_CoasterPlaySfxAlternating(0x0A);
                    Mini_CoasterResolvePath(local.state->pathIndex, 0);
                    local.actor->path = g_pCoasterPath;
                    local.actor->pathLength = g_CoasterPathLength;
                    local.state->mustInit = 0;
                    local.state->hp = 1;
                    local.state->isHit = 0;
                    local.state->frameCount = 0;
                    local.state->pathPos = -70;
                    Mini_CoasterSampleAnimPath(0, local.actor->path, &local.actor->pos, 0);
                }
                local.state->pathPos++;
                local.state->frameCount++;
                if (local.state->frameCount == 5) {
                    for (local.j = 0; local.j < local.state->params[3]; local.j++) {
                        Mini_CoasterSpawnEffect((short)(local.actor->pos.vx + (-50 + rand() % 100)),
                                                (short)(local.actor->pos.vy + 500),
                                                (short)(local.actor->pos.vz + (-50 + rand() % 100)), COASTER_BEHAVIOUR_LEAP_SPRAY, 0x2A);
                    }
                    Mini_CoasterSpawnEffect((short)local.actor->pos.vx, (short)local.actor->pos.vy,
                                            (short)local.actor->pos.vz, COASTER_BEHAVIOUR_LEAP_TRAIL, 0x29);
                }
                if (local.state->pathPos < 0)
                    local.actor->rot.vy += 20;
                if (local.state->pathPos == 80)
                    local.state->hp = 0;
                local.actor->pos.vy += local.state->pathPos;
                if (local.state->params[2] < g_CoasterTrackCursor)
                    local.state->hp = 0;
                if (local.state->hp) {
                    if (local.state->isHit)
                        Mini_CoasterApplyBeamDamage(local.actor);
                } else {
                    Mini_CoasterDespawnActor(local.actor);
                }
                break;

            case COASTER_BEHAVIOUR_LEAP_SPRAY:
                if (local.state->mustInit == 1) {
                    local.state->isHit = 0;
                    local.state->hp = 200;
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    local.state->pathPos = -40 + rand() % 80;
                    local.state->stateValA = -40 + rand() % 80;
                    local.state->stateValB = -(40 + rand() % 40);
                } else {
                    local.state->frameCount++;
                }
                local.state->stateValB++;
                local.actor->rot.vx += 480;
                local.actor->rot.vy += 40;
                local.actor->rot.vz += 610;
                local.actor->pos.vx += local.state->pathPos;
                local.actor->pos.vy += local.state->stateValB;
                local.actor->pos.vz += local.state->stateValA;
                local.state->hp--;
                if (local.state->hp == 0)
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_LEAP_TRAIL:
                if (local.state->mustInit == 1) {
                    local.state->isHit = 0;
                    local.state->hp = 200;
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    local.state->pathPos = -60;
                } else {
                    local.state->frameCount++;
                }
                if (local.state->frameCount > 20) {
                    local.actor->pos.vy += local.state->pathPos;
                    local.state->pathPos++;
                }
                local.state->hp--;
                if (local.state->hp == 0)
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_ABOVE_CAMERA:
                local.actor->pos.vx = g_CoasterCameraPosition.vx;
                local.actor->pos.vy = g_CoasterCameraPosition.vy - 2500;
                local.actor->pos.vz = g_CoasterCameraPosition.vz;
                local.actor->rot.vx = 0;
                local.actor->rot.vy = 0;
                local.actor->rot.vz = 0;
                local.unusedShort.value = 1000;
                break;

            case COASTER_BEHAVIOUR_SPARK_EMITTER:
                if (local.state->mustInit == 1) {
                    local.state->hp = 20;
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    local.state->isHit = 0;
                } else {
                    local.state->frameCount++;
                }
                Mini_CoasterSpawnEffect((short)local.actor->pos.vx, (short)local.actor->pos.vy,
                                        (short)local.actor->pos.vz, COASTER_BEHAVIOUR_HIT_SPARK, 0x2A);
                local.state->hp--;
                if (local.state->hp == 0)
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_HIT_SPARK:
                if (local.state->mustInit == 1) {
                    local.state->isHit = 0;
                    local.state->hp = 50;
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    local.state->pathPos = -10 + rand() % 20;
                    local.state->stateValA = -20 + rand() % 40;
                    local.state->stateValB = -10 + rand() % 20;
                } else {
                    local.state->frameCount++;
                }
                local.actor->pos.vx += local.state->pathPos;
                local.actor->pos.vy += local.state->stateValA;
                local.actor->pos.vz += local.state->stateValB;
                local.actor->rot.vx += 10;
                local.actor->rot.vy += 100;
                local.actor->rot.vz += 20;
                local.state->hp--;
                if (local.state->hp == 0)
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_EXPLOSION_DEBRIS:
                if (local.state->mustInit == 1) {
                    local.state->isHit = 0;
                    local.state->hp = 50;
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    local.state->pathPos = -100 + rand() % 200;
                    local.state->stateValA = -100 + rand() % 200;
                    local.state->stateValB = -100 + rand() % 200;
                } else {
                    local.state->frameCount++;
                }
                local.actor->pos.vx += local.state->pathPos;
                local.actor->pos.vy += local.state->stateValA;
                local.actor->pos.vz += local.state->stateValB;
                local.actor->rot.vx += 0;
                local.actor->rot.vy += 300;
                local.actor->rot.vz += 0;
                local.state->hp--;
                if (local.state->hp == 0)
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_BRAKE:
                if (local.state->mustInit == 1) {
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                } else {
                    local.state->frameCount++;
                }
                if (g_CoasterRideSpeed > local.state->params[2])
                    g_CoasterRideSpeed -= local.state->params[0];
                if (g_CoasterRideSpeed < 0) {
                    g_CoasterRideSpeed = 0;
                    Mini_CoasterSpawnEffect(0, 0, 0, COASTER_BEHAVIOUR_RIDE_END, 0x1D);
                }
                if (local.state->frameCount > local.state->params[1])
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_STOP_AND_GO:
                if (local.state->mustInit == 1) {
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    local.state->pathPos = 0;
                    local.state->stateValA = clock();
                    local.state->stateValB = 0;
                } else {
                    local.state->frameCount++;
                }
                if (local.state->pathPos == 0)
                    g_CoasterRideSpeed = 0;
                if (local.state->pathPos == 1) {
                    g_CoasterRideSpeed += local.state->params[1];
                    local.state->stateValB++;
                }
                if ((clock() - local.state->stateValA) / 1000 > local.state->params[0] / 60)
                    local.state->pathPos = 1;
                if (local.state->stateValB > local.state->params[2])
                    Mini_CoasterDespawnActor(local.actor);
                break;

            case COASTER_BEHAVIOUR_RIDE_START:
                if (local.state->mustInit == 1) {
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    g_CoasterRideSpeed = 0;
                    g_CoasterRideActive = 1;
                    Mini_CoasterSpawnEffect(0, 0, 0, COASTER_BEHAVIOUR_ABOVE_CAMERA, 0x3B);
                } else {
                    local.state->frameCount++;
                }
                local.alpha.value = 0xFF - (local.state->frameCount * 2);
                Mini_CoasterDrawFadeOverlay(local.alpha.value);
                if (local.state->frameCount > 125) {
                    Mini_CoasterDespawnActor(local.actor);
                    g_CoasterRideSpeed = 16384;
                }
                break;

            case COASTER_BEHAVIOUR_RIDE_END:
                if (local.state->mustInit == 1) {
                    local.state->mustInit = 0;
                    local.state->frameCount = 0;
                    g_CoasterRideSpeed = 0;
                    Mini_CoasterStopSoundLoops();
                } else {
                    SoundUpdateFades();
                    local.state->frameCount++;
                }
                local.alpha.value = local.state->frameCount * 2;
                Mini_CoasterDrawFadeOverlay(local.alpha.value);
                if (local.state->frameCount > 127) {
                    Mini_CoasterDespawnActor(local.actor);
                    g_CoasterRideSpeed = 16384;
                    g_CoasterRideActive = 0;
                    g_CoasterRideFinished = 1;
                }
                break;

            case COASTER_BEHAVIOUR_SCORE_CHECK:
                if (g_CoasterScore < local.state->params[0]) {
                    g_CoasterSpawnRequestBlock.state.params[0] = 300;
                    g_CoasterSpawnRequestBlock.state.params[1] = 400;
                    g_CoasterSpawnRequestBlock.state.params[2] = 0;
                    Mini_CoasterSpawnEffect((short)local.actor->pos.vx, (short)local.actor->pos.vy,
                                            (short)local.actor->pos.vz, COASTER_BEHAVIOUR_BRAKE, 0x1E);
                }
                Mini_CoasterDespawnActor(local.actor);
                break;
            }

            SetIdentityPsxMatrix((PsxMatrix *)&local.actor->node->worldMatrix);
            if (local.actor->rot.vx || local.actor->rot.vy || local.actor->rot.vz) {
                if (local.rotationType.value == 0)
                    Mini_CoasterEulerToPsxMatrix((short *)&local.actor->rot,
                                                 (short *)&local.actor->node->worldMatrix);
                else if (local.rotationType.value == 1)
                    RotMatrixXYZ(&local.actor->rot, &local.actor->node->worldMatrix);
                else if (local.rotationType.value == 2)
                    RotMatrixZYX((SVECTOR *)&local.actor->rot, &local.actor->node->worldMatrix);
            }
            local.actor->node->worldMatrix.t[0] = local.actor->pos.vx - g_CoasterCameraPositionXCopy;
            local.actor->node->worldMatrix.t[1] = local.actor->pos.vy - g_CoasterCameraPositionYCopy;
            local.actor->node->worldMatrix.t[2] = local.actor->pos.vz - g_CoasterCameraPositionZCopy;
            if (local.isPlayerCar.value == 0)
                Mini_CoasterDrawActorAndHitTest(local.actor->node, local.actor);
            else if (local.isPlayerCar.value == 1)
                Mini_CoasterDrawPlayerCar(local.actor->node);
        }
    }
}

/*
 * Applies player beam weapon damage to a targeted coaster actor.
 */
// FUNCTION: FF7 0x005ed528
void __cdecl Mini_CoasterApplyBeamDamage(CoasterActor *target)
{
    CoasterObjectState *stats;
    char damage;

    stats = &target->state;
    damage = g_CoasterBeamCharge >> 5;
    if (damage == 0) {
        damage = 1;
    }
    stats->params[13] -= damage;
    if (stats->params[13] < 0) {
        Mini_CoasterAwardKill(target);
    } else {
        Mini_CoasterSpawnEffect((short)target->pos.vx, (short)target->pos.vy, (short)target->pos.vz, COASTER_BEHAVIOUR_HIT_SPARK, 0x3F);
    }
}

/*
 * Awards kill score, spawns explosion effects, and triggers score popup for defeated actor.
 */
// FUNCTION: FF7 0x005ed5ac
void __cdecl Mini_CoasterAwardKill(CoasterActor *target)
{
    int i;
    CoasterKillAward *award;

    award = (CoasterKillAward *)&target->state;
    if (award->killType == COASTER_KILL_SPARKS) {
        g_CoasterScore += award->score;
        Mini_CoasterPlaySfxAlternating((short)award->sfxId);
        award->hp = 0;
        for (i = 0; i < 3; i++) {
            Mini_CoasterSpawnEffect((short)target->pos.vx, (short)target->pos.vy, (short)target->pos.vz, COASTER_BEHAVIOUR_HIT_SPARK, rand() % 3 + 63);
        }
        g_CoasterKillScore = target->node->modelId;
        g_CoasterScorePopupValue = *(short *)&award->score;
        g_CoasterScorePopupTimer = 100;
        g_coasterFreezeState = 1;
        g_CoasterScorePopupState.coasterScorePopupVx = 0;
        g_CoasterScorePopupState.coasterScorePopupVy = 0;
        g_CoasterScorePopupState.coasterScorePopupVz = 0;
    }
    if (award->killType == COASTER_KILL_EXPLOSION) {
        g_CoasterScore += award->score;
        Mini_CoasterPlaySfxAlternating((short)award->sfxId);
        award->hp = 0;
        for (i = 0; i < 3; i++) {
            Mini_CoasterSpawnEffect((short)target->pos.vx, (short)target->pos.vy, (short)target->pos.vz, COASTER_BEHAVIOUR_EXPLOSION_DEBRIS, rand() % 3 + 60);
        }
        g_CoasterKillScore = target->node->modelId;
        g_CoasterScorePopupValue = *(short *)&award->score;
        g_CoasterScorePopupTimer = 100;
        g_coasterFreezeState = 1;
        g_CoasterScorePopupState.coasterScorePopupVx = 0;
        g_CoasterScorePopupState.coasterScorePopupVy = 0;
        g_CoasterScorePopupState.coasterScorePopupVz = 0;
    }
    if (award->killType == COASTER_KILL_SCORE_ONLY) {
        g_CoasterScore += award->score;
        target->rot.vx += award->bonus;
    }
    if (award->killType == COASTER_KILL_NO_EFFECT) {
        g_CoasterScore += award->score;
        Mini_CoasterPlaySfxAlternating((short)award->sfxId);
        award->hp = 0;
        g_CoasterKillScore = target->node->modelId;
        g_CoasterScorePopupValue = *(short *)&award->score;
        g_CoasterScorePopupTimer = 100;
        g_coasterFreezeState = 1;
        g_CoasterScorePopupState.coasterScorePopupVx = 0;
        g_CoasterScorePopupState.coasterScorePopupVy = 0;
        g_CoasterScorePopupState.coasterScorePopupVz = 0;
    }
    if (award->killType == COASTER_KILL_BIG_EXPLOSION) {
        g_CoasterScore += award->score;
        Mini_CoasterPlaySfxAlternating((short)award->sfxId);
        award->hp = 0;
        for (i = 0; i < 100; i++) {
            Mini_CoasterSpawnEffect((short)target->pos.vx, (short)target->pos.vy, (short)target->pos.vz, COASTER_BEHAVIOUR_EXPLOSION_DEBRIS, rand() % 3 + 63);
        }
        g_CoasterKillScore = target->node->modelId;
        g_CoasterScorePopupValue = *(short *)&award->score;
        g_CoasterScorePopupTimer = 100;
        g_coasterFreezeState = 1;
        g_CoasterScorePopupState.coasterScorePopupVx = 0;
        g_CoasterScorePopupState.coasterScorePopupVy = 0;
        g_CoasterScorePopupState.coasterScorePopupVz = 0;
    }
}
} /* extern "C" */

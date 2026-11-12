// src/ff7/src/wm/wm_world_terrain_streaming.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Decompresses the world map's LZS data and streams terrain blocks in and out around the
//   player. It also manages the block and mesh pools, the load and decompression queues, and the
//   resident block and mesh planning.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/wm.h"
#include "ff7/enums/wm.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include <string.h>
#include <stdio.h>
#include "lib/types/graphics.h"
#include "ff7/types/wm.h"
extern "C" {

#pragma intrinsic(memset, strcpy, strcat, strcmp)
// clang-format off
// GLOBAL: FF7 0x00e045f8
struct WorldTerrainChunkInfo *g_WorldStreamingState = 0;
// GLOBAL: FF7 0x00e045fc
void * g_WorldStreamingSlot = 0;
// GLOBAL: FF7 0x00e04600
int g_WorldStreamDestSlot = 0;
// GLOBAL: FF7 0x00e04604
int g_WorldBlockLoadInfoCount = 0;
// GLOBAL: FF7 0x00e04608
int g_pWorldCurrentProbeTriangle = 0;
unsigned char g_bssPad_00e0460c[4] = {0};
// g_WorldBlockLoadSlots is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0xe04610..0xe04910 are aliases.
// GLOBAL: FF7 0x00e04610
unsigned char g_WorldBlockLoadSlots[0x300] = {0};
// GLOBAL: FF7 0x00e04910
int g_WorldStreamFocusSmoothing = 0;
unsigned char g_bssPad_00e04914[4] = {0};
// GLOBAL: FF7 0x00e04918
WorldStreamFocusData g_WorldStreamFocus = {0};
// GLOBAL: FF7 0x00e04928
unsigned char g_WorldProbeOffsets[9 * 8] = {0};
// GLOBAL: FF7 0x00e04970
unsigned char g_WorldMeshLoadState[1] = {0};
unsigned char g_bssPad_00e04974[4] = {0};
unsigned char g_bssPad_00e04978[147448] = {0};
// GLOBAL: FF7 0x00e28970
unsigned char * g_WorldMeshBlockBuffer = 0;
unsigned char g_bssPad_00e28974[4] = {0};
// The mesh-decompression queue's 64 eight-byte nodes (next, mesh record): World_InitMeshQueuePool links them
// into the free list g_WorldFreeBlockNodeList, each node's next word the address of the node after it.
// GLOBAL: FF7 0x00e28978
int g_WorldMeshBlockNodes[0x80] = {0};
// GLOBAL: FF7 0x00e28b78
int g_pWorldMeshDecompressQueue = 0;
// GLOBAL: FF7 0x00e28b7c
struct WorldBlockLoadInfo *g_WorldActiveMeshBuffer = 0;
// GLOBAL: FF7 0x00e28b80
unsigned char g_WorldMeshRecords[0x14 * 8] = {0};
// GLOBAL: FF7 0x00e28c20
int g_WorldMeshQueueHead = 0;
// GLOBAL: FF7 0x00e28c24
int g_WorldMeshQueueTail = 0;
// GLOBAL: FF7 0x00e28c28
int g_WorldStreamingEnabled = 0;
// GLOBAL: FF7 0x00e28c2c
int g_WorldPendingBlockId = 0;
// GLOBAL: FF7 0x00e28c30
int g_WorldLoadedBlockIds[4] = {0};
// GLOBAL: FF7 0x00e28c40
int g_WorldTerrainWalkCompleted = 0;
// World_WalkTerrainAndPlaceEntities' frame counter since the probe rows last swapped (it swaps from 8 on).
// GLOBAL: FF7 0x00e28c44
int g_WorldProbeRowCounter = 0;
// The debug position text World_WalkTerrainAndPlaceEntities formats (sprintf) and World_DrawDebugTextOverlay
// draws (TextOutA); the span to g_WorldBlockLoadBudget is the buffer's.
// GLOBAL: FF7 0x00e28c48
char g_WorldWalkDebugText[0x64] = {0};
// GLOBAL: FF7 0x00e28cac
int g_WorldBlockLoadBudget = 0;
// GLOBAL: FF7 0x00e28cb0
int g_WorldBlockSlotList = 0;
// GLOBAL: FF7 0x00e28cb4
int g_WorldProgress = 0;
// Which of World_WalkTerrainAndPlaceEntities' two probe rows is current (0 or 1).
// GLOBAL: FF7 0x00e28cb8
int g_WorldCurrentProbeRow = 0;
// GLOBAL: FF7 0x00e28cbc
int g_WorldTerrainStreamingEnabled = 0;
// GLOBAL: FF7 0x00e28cc0
int g_WorldTerrainWalkPhase = 0;
// GLOBAL: FF7 0x00e28cc4
int g_WorldMeshRecordList = 0;
// GLOBAL: FF7 0x00e28cc8
int g_WorldStreamFocusChanged = 0;
// GLOBAL: FF7 0x00e28ccc
int g_WorldMeshRecordFreeList = 0;
// GLOBAL: FF7 0x00e28cd0
int g_WorldCurrentTerrainChunk = 0;
// GLOBAL: FF7 0x00e28cd4
int g_WorldTerrainBlockCountdown = 0;
// GLOBAL: FF7 0x00e28cd8
int g_WorldFreeBlockNodeList = 0;
// GLOBAL: FF7 0x00e28cdc
int g_WorldBlockLoadIndex = 0;
// The world map's size in terrain blocks per map variant (World_GetMapVariant 0..3): columns {9, 9, 3, 2}
// and rows {7, 7, 4, 2}; World_PreloadBlocksFromBot wraps the player's block into them.
// GLOBAL: FF7 0x00969b30
char g_WorldWrapGridCols[4] = {9, 9, 3, 2};
// GLOBAL: FF7 0x00969b34
char g_WorldWrapGridRows[4] = {7, 7, 4, 2};
// clang-format on

void __cdecl World_ServiceMeshDecompressQueue(void);
void __cdecl World_WalkTerrainAndPlaceEntities(VECTOR *previous, VECTOR *position, VECTOR *target, short angle);

/*
 * Decompresses the world LZS stream starting after its four-byte length header, using the end
 * pointer src+3+header. Consumes flag bytes least-significant bit first: set bits copy literals;
 * clear bits decode a 12-bit circular reference position and length 3 through 18, zero-filling
 * references before the destination start and then copying overlapping reference bytes forward.
 * Checks the source end before each token but does not separately check the second reference byte
 * or bound destination writes.
 */
// FUNCTION: FF7 0x0074ffc0
void __cdecl World_LzsDecompress(unsigned char *src, unsigned char *dst)
{
    struct {
        unsigned char *pRefEnd;
        unsigned char *dststart;
        int i;
        void *srcend;
        int j;
        int count;
        int flags;
        unsigned char *pRefData;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.count = 0;
    frame.srcend = src + 3 + *(int *)src;
    src += sizeof(int);
    frame.dststart = dst;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        if (frame.count == 0) {
            frame.count = 8;
            if (src >= frame.srcend) {
                break;
            }
            frame.flags = *(src++);
        }
        if (frame.flags & 1) {
            if (src >= frame.srcend) {
                break;
            }
            *(dst++) = *(src++);
        } else {
            if (src >= frame.srcend) {
                break;
            }
            frame.i = *(src++);
            frame.j = *(src++);
            frame.i |= (frame.j & 0xf0) << 4;
            frame.pRefEnd = dst + 3 + (frame.j & 0xf);
            frame.pRefData = dst - ((dst - frame.dststart - (frame.i - (4096 - 18))) & (4096 - 1));
            while (frame.pRefData < frame.dststart) {
                *(dst++) = 0;
                frame.pRefData++;
            }
            while (dst < frame.pRefEnd) {
                *(dst++) = *(frame.pRefData++);
            }
        }
        frame.flags >>= 1;
        frame.count--;
    }
}

/*
 * For each requested mesh-index output, shifts position X or Z right 13 bits, stores a short, then
 * adjusts it once by 0x24 columns or 0x1C rows when below zero or at/above the corresponding
 * extent. This is a single wrap step rather than full range normalization. An optional three-short
 * offset output receives low 13 bits of X, zero Y and low 13 bits of Z.
 */
// FUNCTION: FF7 0x00750134
void __cdecl WorldMap_PositionToMesh(void *position, void *subOffset, void *meshX, void *meshZ)
{
    if (meshX != 0) {
        *(short *)meshX = ((VECTOR *)position)->vx >> 0xD;
        if (*(short *)meshX < 0) {
            *(short *)meshX += 0x24;
        } else if (*(short *)meshX >= 0x24) {
            *(short *)meshX -= 0x24;
        }
    }
    if (meshZ != 0) {
        *(short *)meshZ = ((VECTOR *)position)->vz >> 0xD;
        if (*(short *)meshZ < 0) {
            *(short *)meshZ += 0x1C;
        } else if (*(short *)meshZ >= 0x1C) {
            *(short *)meshZ -= 0x1C;
        }
    }
    if (subOffset != 0) {
        ((SVECTOR *)subOffset)->vx = ((VECTOR *)position)->vx & 0x1FFF;
        ((SVECTOR *)subOffset)->vy = 0;
        ((SVECTOR *)subOffset)->vz = ((VECTOR *)position)->vz & 0x1FFF;
    }
}

/*
 * For a nonnull position, adjusts x by one map width (0x48000) when outside its interval and
 * adjusts z by one map height (0x38000) similarly. Performs one correction per axis; positions
 * more than one span out can remain outside.
 */
// FUNCTION: FF7 0x00750202
void __cdecl World_WrapPositionToMapBounds(void *position)
{
    if (position != 0) {
        if (((VECTOR *)position)->vx >= 0x48000) {
            ((VECTOR *)position)->vx -= 0x48000;
        } else if (((VECTOR *)position)->vx < 0) {
            ((VECTOR *)position)->vx += 0x48000;
        }
        if (((VECTOR *)position)->vz >= 0x38000) {
            ((VECTOR *)position)->vz -= 0x38000;
        } else if (((VECTOR *)position)->vz < 0) {
            ((VECTOR *)position)->vz += 0x38000;
        }
    }
}

/*
 * Unless World_GetCameraZoomState() is 2: a negative `smooth` clears g_WorldStreamFocusChanged, a
 * positive one sets it while g_WorldStreamFocusSmoothing is set.
 * While smoothing, unwraps X/Z around the 36x 28 mesh periods, blends prior and target
 * positions 3:1, wraps with World_WrapPositionToMapBounds and keeps smoothing only while summed absolute X/Z
 * differences exceed ten. Otherwise directly copies the supplied focus record.
 */
// FUNCTION: FF7 0x0075027c
void __cdecl World_UpdateStreamFocus(WorldStreamFocusData *target, int smooth)
{
    struct {
        int dwDiffZ;
        int dwDiffX;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (World_GetCameraZoomState() != 2) {
        if (smooth < 0) {
            g_WorldStreamFocusChanged = 0;
        } else if (g_WorldStreamFocusSmoothing) {
            g_WorldStreamFocusChanged |= smooth;
        }
        if (g_WorldStreamFocusChanged) {
            if (target->x - g_WorldStreamFocus.x > (0x12 << 13)) {
                g_WorldStreamFocus.x += (0x24 << 13);
            } else if (target->x - g_WorldStreamFocus.x < -(0x12 << 13)) {
                g_WorldStreamFocus.x -= (0x24 << 13);
            }
            if (target->z - g_WorldStreamFocus.z > (0xE << 13)) {
                g_WorldStreamFocus.z += (0x1C << 13);
            } else if (target->z - g_WorldStreamFocus.z < -(0xE << 13)) {
                g_WorldStreamFocus.z -= (0x1C << 13);
            }
            g_WorldStreamFocus.x = (g_WorldStreamFocus.x * 3 + target->x) >> 2;
            g_WorldStreamFocus.z = (g_WorldStreamFocus.z * 3 + target->z) >> 2;
            World_WrapPositionToMapBounds(&g_WorldStreamFocus);
            if (target->x - g_WorldStreamFocus.x > 0) {
                frame.dwDiffX = target->x - g_WorldStreamFocus.x;
            } else {
                frame.dwDiffX = -(target->x - g_WorldStreamFocus.x);
            }
            if (target->z - g_WorldStreamFocus.z > 0) {
                frame.dwDiffZ = target->z - g_WorldStreamFocus.z;
            } else {
                frame.dwDiffZ = -(target->z - g_WorldStreamFocus.z);
            }
            g_WorldStreamFocusChanged = (frame.dwDiffX + frame.dwDiffZ) > 10;
        } else {
            g_WorldStreamFocus = *target;
        }
    }
}

/*
 * Copies g_WorldStreamFocus into the supplied stream-focus record when focus is nonnull.
 */
// FUNCTION: FF7 0x0075042b
void __cdecl World_GetStreamFocus(WorldStreamFocusData *focus)
{
    if (focus != 0) {
        *focus = g_WorldStreamFocus;
    }
}

/*
 * Returns the terrain streaming flag g_WorldTerrainBlockCountdown.
 */
// FUNCTION: FF7 0x0075045c
int __cdecl World_GetTerrainBlockCountdown(void)
{
    return g_WorldTerrainBlockCountdown;
}

/*
 * Sets the terrain streaming flag g_WorldTerrainBlockCountdown.
 */
// FUNCTION: FF7 0x00750466
void __cdecl World_SetTerrainBlockCountdown(int value)
{
    g_WorldTerrainBlockCountdown = value;
}

/*
 * Initialises terrain streaming: the block, mesh-queue and mesh-record pools, and every streaming
 * state word (g_WorldBlockLoadInfoCount, g_WorldBlockLoadIndex, g_WorldCurrentTerrainChunk, g_pWorldCurrentProbeTriangle, g_WorldTerrainWalkPhase, g_WorldPendingBlockId,
 * g_WorldTerrainBlockCountdown to 0 and g_WorldTerrainStreamingEnabled to 1).
 */
// FUNCTION: FF7 0x00750473
void __cdecl World_InitTerrainStreaming(void)
{
    World_InitBlockPools();
    World_InitMeshQueuePool();
    World_InitMeshRecordPool();
    g_WorldBlockLoadInfoCount = 0;
    g_WorldBlockLoadIndex = 0;
    g_WorldCurrentTerrainChunk = 0;
    g_pWorldCurrentProbeTriangle = 0;
    g_WorldTerrainStreamingEnabled = 1;
    g_WorldTerrainWalkPhase = 0;
    g_WorldPendingBlockId = 0;
    g_WorldTerrainBlockCountdown = 0;
}

/*
 * Thread two free lists and clear the counters that index them -- the same
 * shape as World_InitEffectNodePool, twice over and with 8-byte records instead of
 * 0x54-byte ones.
 * 
 * Both loops run to one short of their bound and the terminating zero is
 * written afterwards, using the loop variable after it has run out. The
 * variable is a `short`: every index goes through `movsx`, and `i + 1`
 * promotes to int before the multiply, which is why the `lea` folds the
 * step into its displacement rather than adding it.
 */
// FUNCTION: FF7 0x007504d7
void __cdecl World_InitBlockPools(void)
{
    short i;

    g_WorldStreamingSlot = g_pWorldArenaRegion6;
    for (i = 0; i < 8; i++) {
        *(unsigned char **)&g_WorldProbeOffsets[i * 8] = &g_WorldProbeOffsets[(i + 1) * 8];
    }
    *(unsigned char **)&g_WorldProbeOffsets[i * 8] = 0;
    g_WorldMeshBlockBuffer = g_WorldProbeOffsets;
    g_WorldBlockSlotList = 0;
    g_WorldStreamingEnabled = g_WorldBlockSlotList;
    for (i = 0; i < 0x13; i++) {
        *(unsigned char **)&g_WorldMeshRecords[i * 8] = &g_WorldMeshRecords[(i + 1) * 8];
    }
    *(unsigned char **)&g_WorldMeshRecords[i * 8] = 0;
    g_WorldActiveMeshBuffer = (WorldBlockLoadInfo *)g_WorldMeshRecords;
    g_WorldMeshQueueTail = 0;
    g_WorldBlockLoadBudget = 0;
    g_WorldTerrainWalkCompleted = 0;
    g_WorldMeshQueueHead = 0;
    g_WorldStreamDestSlot = -1;
    g_WorldProgress = 0;
}

/*
 * Links 64 eight-byte entries at g_WorldMeshBlockNodes into a free list ending in zero, sets g_WorldFreeBlockNodeList
 * to its head and clears g_pWorldMeshDecompressQueue/g_WorldStreamFocusSmoothing.
 */
// FUNCTION: FF7 0x007505d7
void __cdecl World_InitMeshQueuePool(void)
{
    short i;

    for (i = 0; i < 0x3F; i++) {
        *(int *)((char *)&g_WorldMeshBlockNodes + i * 8) = i * 8 + (int)&g_WorldMeshBlockNodes[2];
    }
    *(int *)((char *)&g_WorldMeshBlockNodes + i * 8) = 0;
    g_WorldFreeBlockNodeList = (int)&g_WorldMeshBlockNodes;
    g_pWorldMeshDecompressQueue = 0;
    g_WorldStreamFocusSmoothing = 0;
}

/*
 * Links 32 0x18-byte entries at g_WorldBlockLoadSlots into a free list ending in zero, sets g_WorldMeshRecordFreeList to
 * its head and clears g_WorldMeshRecordList/g_WorldStreamingState.
 */
// FUNCTION: FF7 0x00750641
void __cdecl World_InitMeshRecordPool(void)
{
    short i;

    for (i = 0; i < 0x1F; i++) {
        *(int *)((char *)&g_WorldBlockLoadSlots + i * 0x18) = (i + 1) * 0x18 + (int)&g_WorldBlockLoadSlots;
    }
    *(int *)((char *)&g_WorldBlockLoadSlots + i * 0x18) = 0;
    g_WorldMeshRecordFreeList = (int)&g_WorldBlockLoadSlots;
    g_WorldMeshRecordList = 0;
    g_WorldStreamingState = (WorldTerrainChunkInfo *)g_WorldMeshRecordList;
}

/*
 * Calls World_DisableTerrainStreamingAndReleaseLoadingBlock.
 */
// FUNCTION: FF7 0x007506b6
void __cdecl World_DisableTerrainStreamingBeforeDeparture(void)
{
    World_DisableTerrainStreamingAndReleaseLoadingBlock();
}

/*
 * Clears g_WorldTerrainStreamingEnabled. When a loading block is selected and the delay has reached five times
 * g_WorldMovementMultiplier, finds its node in the loading list, reports error 0xD if absent, then unlinks and
 * returns the node to the free list, clears the delay and resets the selected block to -1. The
 * diagnostic does not stop the subsequent node access.
 */
// FUNCTION: FF7 0x007506c0
void __cdecl World_DisableTerrainStreamingAndReleaseLoadingBlock(void)
{
    int node;
    int prev;

    g_WorldTerrainStreamingEnabled = 0;
    if (g_WorldStreamDestSlot >= 0 && g_WorldBlockLoadBudget >= g_WorldMovementMultiplier * 5) {
        for (prev = 0, node = g_WorldBlockSlotList;
             node != 0 && node != (int)((char *)&g_WorldProbeOffsets + g_WorldStreamDestSlot * 8);
             prev = node, node = *(int *)node) {
        }
        if (node == 0) {
            WorldScript_ReportError(0xd);
        }
        if (prev != 0) {
            *(int *)prev = *(int *)node;
        } else {
            g_WorldBlockSlotList = *(int *)node;
        }
        *(int *)node = (int)g_WorldMeshBlockBuffer;
        g_WorldMeshBlockBuffer = (unsigned char *)node;
        g_WorldBlockLoadBudget = 0;
        g_WorldStreamDestSlot = -1;
    }
}

/*
 * Sets g_WorldTerrainStreamingEnabled, the streaming-idle flag.
 */
// FUNCTION: FF7 0x00750784
void __cdecl World_EnableTerrainStreaming(void)
{
    g_WorldTerrainStreamingEnabled = 1;
}

/*
 * Returns the streaming-idle flag g_WorldTerrainStreamingEnabled.
 */
// FUNCTION: FF7 0x00750793
int __cdecl World_IsTerrainStreamingEnabled(void)
{
    return g_WorldTerrainStreamingEnabled;
}

/*
 * Samples current and previous player positions and, when streaming is enabled, offsets the block
 * focus by a rotated -8192 vector, wraps it once over the world extents, plans resident
 * blocks/meshes and services load promotion/decompression. Tracks backlog calls above 2 and
 * invokes World_SetStreamingStallFadeState on crossing 300 or clearing that backlog counter. Separately pushes the
 * player off obstacles according to vehicle transition and pending state, resamples the resulting
 * position, sets the terrain-placement constants and optionally walks terrain using
 * previous/original/final positions. Calls World_TerrainStreamingPostUpdateNoOp when control lock is clear.
 */
// FUNCTION: FF7 0x0075079d
void __cdecl World_UpdateTerrainStreaming(short wAngle, int placeEntities)
{
    struct WorldUpdateShortVector {
        short pad;
        short vx;
        short vy;
        short vz;
    };
    struct {
        short wChunkY;
        int prevStallCounter;
        short wChunkX;
        struct WorldUpdateShortVector lookAheadOffset;
        WorldStreamFocusData lookAheadPos;
        WorldStreamFocusData playerPosAfterPush;
        WorldStreamFocusData playerPos;
        WorldStreamFocusData prevPlayerPos;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    WorldEntity_SelectPlayerAsCurrentActor();
    WorldEntity_GetPlayerPosition((VECTOR *)&frame.playerPos);
    WorldEntity_GetPlayerPrevPosition((VECTOR *)&frame.prevPlayerPos);
    if (g_WorldTerrainStreamingEnabled) {
        frame.lookAheadOffset.vx = frame.lookAheadOffset.vy = 0;
        frame.lookAheadOffset.vz = -0x2000;
        WorldMap_RotateVec3ByYaw((SVECTOR *)&frame.lookAheadOffset.vx, -wAngle);
        frame.lookAheadPos.x = frame.playerPos.x + frame.lookAheadOffset.vx;
        frame.lookAheadPos.y = frame.playerPos.y;
        frame.lookAheadPos.z = frame.playerPos.z + frame.lookAheadOffset.vz;
        if (frame.lookAheadPos.x < 0) {
            frame.lookAheadPos.x += 0x24 << 13;
        } else if (frame.lookAheadPos.x >= 0x24 << 13) {
            frame.lookAheadPos.x -= 0x24 << 13;
        }
        if (frame.lookAheadPos.z < 0) {
            frame.lookAheadPos.z += 0x1c << 13;
        } else if (frame.lookAheadPos.z >= 0x1c << 13) {
            frame.lookAheadPos.z -= 0x1c << 13;
        }
        World_PlanResidentBlocks((int *)&frame.lookAheadPos);
        World_PlanResidentMeshes((int *)&frame.playerPos,
                                                                                   wAngle);
        WorldMap_PositionToMesh(&frame.lookAheadPos, 0, &frame.wChunkX, &frame.wChunkY);
        World_ServiceNearestBlockLoad(frame.wChunkX, frame.wChunkY);
        World_PromoteLoadedBlock();
        World_ServiceMeshDecompressQueue();
        frame.prevStallCounter = g_WorldTerrainWalkCompleted;
        g_WorldTerrainWalkCompleted = (World_CountBlockSlotsInUse() > 2) ? g_WorldTerrainWalkCompleted + 1 : 0;
        if (frame.prevStallCounter < 300 && g_WorldTerrainWalkCompleted >= 300) {
            World_SetStreamingStallFadeState(1);
        } else if (frame.prevStallCounter >= 300 && g_WorldTerrainWalkCompleted == 0) {
            World_SetStreamingStallFadeState(4);
        }
    }
    if (World_GetVehicleTransitionState() == 2) {
        World_PushPlayerOffObstacle(0);
    } else if (g_WorldTerrainWalkPhase == 1) {
        World_PushPlayerOffObstacle(g_WorldPendingBlockId);
    }
    WorldEntity_GetPlayerPosition((VECTOR *)&frame.playerPosAfterPush);
    g_WorldTerrainOcclusionDepth = 5500;
    g_WorldTerrainFogDistance = (World_GetMapVariant() == 2) ? 16000 : 0;
    if (placeEntities) {
        World_WalkTerrainAndPlaceEntities(
            (VECTOR *)&frame.prevPlayerPos, (VECTOR *)&frame.playerPos, (VECTOR *)&frame.playerPosAfterPush, wAngle);
    }
    if (World_IsControlLockClear()) {
        World_TerrainStreamingPostUpdateNoOp();
    }
}

/*
 * Builds a wanted-block list for 0x8000-unit grid cells within 0x4000 of the supplied X and Z,
 * taking each request from the free list and storing its block index. Removes blocks already
 * released or in use, returns their requests to the free list and moves matched released slots to
 * the released-list front. Enqueues every remaining request. Free-list exhaustion reports error 10
 * but still dereferences the null request if the error handler returns; grid division retains
 * signed truncation toward zero.
 */
// FUNCTION: FF7 0x007509b2
void __cdecl World_PlanResidentBlocks(int *pos)
{
    int node;
    int head;
    int tmpRequest;
    int xmin;
    int last;
    int keep;
    int cellX;
    int x1;
    int cellZ;
    int zmax;
    int z0;

    xmin = (pos[0] - 0x4000) / 0x8000;
    z0 = (pos[2] - 0x4000) / 0x8000;
    x1 = (pos[0] + 0x4000) / 0x8000;
    zmax = (pos[2] + 0x4000) / 0x8000;
    head = 0;
    for (cellZ = z0; cellZ <= zmax; cellZ++) {
        for (cellX = xmin; cellX <= x1; cellX++) {
            tmpRequest = *(int *)&g_WorldActiveMeshBuffer;
            if (tmpRequest == 0) {
                WorldScript_ReportError(0xA);
            }
            *(int *)&g_WorldActiveMeshBuffer = *(int *)tmpRequest;
            *(int *)tmpRequest = head;
            head = tmpRequest;
            *(short *)(tmpRequest + 4) = World_BlockIndexFromGrid(cellX, cellZ);
        }
    }
    keep = 0;
    node = g_WorldStreamingEnabled;
    while (node != 0) {
        last = 0;
        for (tmpRequest = head; tmpRequest != 0 && *(short *)(tmpRequest + 4) != *(short *)(node + 4); last = tmpRequest, tmpRequest = *(int *)tmpRequest) {
        }
        if (tmpRequest != 0) {
            if (last != 0) {
                *(int *)last = *(int *)tmpRequest;
            } else {
                head = *(int *)tmpRequest;
            }
            *(int *)tmpRequest = *(int *)&g_WorldActiveMeshBuffer;
            *(int *)&g_WorldActiveMeshBuffer = tmpRequest;
            if (keep != 0) {
                *(int *)keep = *(int *)node;
                *(int *)node = g_WorldStreamingEnabled;
                g_WorldStreamingEnabled = node;
                node = *(int *)keep;
            } else {
                keep = node;
                node = *(int *)node;
            }
        } else {
            keep = node;
            node = *(int *)node;
        }
    }
    for (node = g_WorldBlockSlotList; node != 0; node = *(int *)node) {
        last = 0;
        for (tmpRequest = head; tmpRequest != 0 && *(short *)(tmpRequest + 4) != *(short *)(node + 4); last = tmpRequest, tmpRequest = *(int *)tmpRequest) {
        }
        if (tmpRequest != 0) {
            if (last != 0) {
                *(int *)last = *(int *)tmpRequest;
            } else {
                head = *(int *)tmpRequest;
            }
            *(int *)tmpRequest = *(int *)&g_WorldActiveMeshBuffer;
            *(int *)&g_WorldActiveMeshBuffer = tmpRequest;
        }
    }
    for (tmpRequest = head; tmpRequest != 0; tmpRequest = World_EnqueueBlockLoad((WorldBlockLoadRequest *)tmpRequest)) {
    }
}

/*
 * Turns a grid column (mod 9) and row (mod 7) into a block index, row * 9 + column, wrapping each
 * coordinate once in either direction.
 */
// FUNCTION: FF7 0x00750c20
int __cdecl World_BlockIndexFromGrid(short column, short row)
{
    if (column < 0) {
        column += 9;
    }
    if (column >= 9) {
        column -= 9;
    }
    if (row < 0) {
        row += 7;
    }
    if (row >= 7) {
        row -= 7;
    }
    return row * 9 + column;
}

/*
 * Takes one block-load request off the front of its list and either gives it a block slot (records
 * the slot index at +6 and its block number at +4 into the slot table at g_WorldProbeOffsets, then pushes
 * the request on the pending-load list g_WorldMeshQueueTail) or, with no free slot, returns it to the
 * free-request list g_WorldActiveMeshBuffer. Returns the next request in the list.
 */
// FUNCTION: FF7 0x00750c84
int __cdecl World_EnqueueBlockLoad(WorldBlockLoadRequest *request)
{
    int slot;
    int nextRequest;

    nextRequest = 0;
    slot = World_AcquireBlockSlot();
    if (slot != 0) {
        request->slot = slot - (int)&g_WorldProbeOffsets >> 3;
        *(short *)((char *)((char *)&g_WorldProbeOffsets + 4) + request->slot * 8) = request->cellIndex;
        nextRequest = request->next;
        request->next = g_WorldMeshQueueTail;
        g_WorldMeshQueueTail = (int)request;
    } else {
        nextRequest = request->next;
        request->next = *(int *)&g_WorldActiveMeshBuffer;
        *(int *)&g_WorldActiveMeshBuffer = (int)request;
    }
    return nextRequest;
}

/*
 * Pops a terrain block from the free list when available. Otherwise walks the released list to its
 * final entry and unlinks it only when a predecessor exists; a one-entry released list is left
 * intact and returns zero. A successfully acquired slot is pushed onto g_WorldBlockSlotList and returned.
 */
// FUNCTION: FF7 0x00750d06
int __cdecl World_AcquireBlockSlot(void)
{
    int slot;
    int prev;

    slot = 0;
    if (*(int *)&g_WorldMeshBlockBuffer != 0) {
        slot = *(int *)&g_WorldMeshBlockBuffer;
        *(int *)&g_WorldMeshBlockBuffer = *(int *)slot;
    } else {
        slot = g_WorldStreamingEnabled;
        if (slot != 0) {
            for (prev = 0; *(int *)slot != 0; prev = slot, slot = *(int *)slot) {
            }
            if (prev != 0) {
                *(int *)prev = 0;
            } else {
                slot = 0;
            }
        }
    }
    if (slot != 0) {
        *(int *)slot = g_WorldBlockSlotList;
        g_WorldBlockSlotList = slot;
    }
    return slot;
}

/*
 * When no block load is active, the pending list is nonempty and World_TryEnterInitStep(0) permits, scans
 * pending requests for the smallest wrapped Manhattan distance to column/row. Splits each signed-
 * short request ID into quotient/remainder by 4, folds horizontal differences above 18 with
 * 36-distance and vertical differences above 14 with 28-distance, and keeps the first strict
 * minimum. Reports error 0x22 if none was selected, then still calls World_PopAndLoadBlockRequest
 * with the selected node and predecessor. Does not normalize arbitrary input coordinates.
 */
// FUNCTION: FF7 0x00750d99
void __cdecl World_ServiceNearestBlockLoad(short column, short row)
{
    WorldBlockLoadRequest *requestPtr;
    int distance;
    WorldBlockLoadRequest *previous;
    WorldBlockLoadRequest *chosenPtr;
    int dxWrap;
    int rowIdx;
    int cellCol;
    int wrappedDz;
    WorldBlockLoadRequest *bestPrevPtr;
    int bestDist;
    int horizDist;
    int aa;

    if (g_WorldBlockLoadBudget == 0) {
        if (g_WorldMeshQueueTail != 0) {
            if (World_TryEnterInitStep(0) != 0) {
                bestPrevPtr = 0;
                chosenPtr = bestPrevPtr;
                bestDist = 0x7FFF;
                previous = 0;
                requestPtr = (WorldBlockLoadRequest *)g_WorldMeshQueueTail;
                for (; requestPtr != 0; previous = requestPtr, requestPtr = (WorldBlockLoadRequest *)(requestPtr->next)) {
                    rowIdx = requestPtr->cellIndex / 4;
                    cellCol = requestPtr->cellIndex - rowIdx * 4;
                    if (cellCol - column > 0) {
                        horizDist = cellCol - column;
                    } else {
                        horizDist = -(cellCol - column);
                    }
                    dxWrap = horizDist;
                    if (dxWrap > 0x12) {
                        dxWrap = 0x24 - dxWrap;
                    }
                    if (rowIdx - row > 0) {
                        aa = rowIdx - row;
                    } else {
                        aa = -(rowIdx - row);
                    }
                    wrappedDz = aa;
                    if (wrappedDz > 0x0E) {
                        wrappedDz = 0x1C - wrappedDz;
                    }
                    distance = dxWrap + wrappedDz;
                    if (bestDist > distance) {
                        bestDist = distance;
                        chosenPtr = requestPtr;
                        bestPrevPtr = previous;
                    }
                }
                if (chosenPtr == 0) {
                    WorldScript_ReportError(0x22);
                }
                World_PopAndLoadBlockRequest(chosenPtr, bestPrevPtr);
            }
        }
    }
}

/*
 * Unlinks block request request from its list (through the predecessor prev, or from the head
 * g_WorldMeshQueueTail), reads the block from the map file, and returns the request to the free list
 * g_WorldActiveMeshBuffer.
 */
// FUNCTION: FF7 0x00750efa
void __cdecl World_PopAndLoadBlockRequest(WorldBlockLoadRequest *request, WorldBlockLoadRequest *prev)
{
    if (prev != 0) {
        prev->next = request->next;
    } else {
        g_WorldMeshQueueTail = request->next;
    }
    World_ReadBlockFromMapFile(request);
    request->next = *(int *)&g_WorldActiveMeshBuffer;
    *(int *)&g_WorldActiveMeshBuffer = (int)request;
}

/*
 * Reads the terrain block requested at req+4 into the buffer slot req+6. On map variant 0 applies
 * the six progress-dependent substitutions and stores the remapped ID in g_WorldMeshQueueHead. Variant 2
 * folds the nine-column ID into a three-column, four-row file layout with single wrap adjustments;
 * variant 3 folds it into a two-by-two layout. Reads 0xB800 bytes at the resulting file offset,
 * sets streaming budget to five times g_WorldMovementMultiplier, stores the destination slot in g_WorldStreamDestSlot
 * and calls World_ArmBlockLoadBudgetAndClearInitStep0. Does not test the file-read result.
 */
// FUNCTION: FF7 0x00750f3c
void __cdecl World_ReadBlockFromMapFile(WorldBlockLoadRequest *req)
{
    int blk;
    int variant;
    int z;
    int cx;
    int cz;
    int qq;

    variant = World_GetMapVariant();
    g_WorldMeshQueueHead = variant == 0
                       ? (g_WorldProgress > 0 && req->cellIndex == 0x32   ? 0x3F
                          : g_WorldProgress > 1 && req->cellIndex == 0x29 ? 0x40
                          : g_WorldProgress > 1 && req->cellIndex == 0x2A ? 0x41
                          : g_WorldProgress > 2 && req->cellIndex == 0x3C ? 0x42
                          : g_WorldProgress > 3 && req->cellIndex == 0x2F ? 0x43
                          : g_WorldProgress > 3 && req->cellIndex == 0x30 ? 0x44
                                                                                               : req->cellIndex)
                       : req->cellIndex;
    blk = g_WorldMeshQueueHead;
    if (variant == 2) {
        z = g_WorldMeshQueueHead / 9 - 2;
        if (z < 0) {
            z += 4;
        } else if (z > 3) {
            z -= 4;
        }
        cx = g_WorldMeshQueueHead % 9 - 3;
        if (cx < 0) {
            cx += 3;
        } else if (cx > 2) {
            cx -= 3;
        }
        blk = z * 3 + cx;
    } else if (variant == 3) {
        cz = g_WorldMeshQueueHead / 9;
        if (cz >= 3) {
            cz -= 7;
        }
        if (cz < 0) {
            cz += 2;
        } else if (cz > 1) {
            cz -= 2;
        }
        qq = g_WorldMeshQueueHead % 9;
        if (qq >= 4) {
            qq -= 9;
        }
        if (qq < 0) {
            qq += 2;
        } else if (qq > 1) {
            qq -= 2;
        }
        blk = cz * 2 + qq;
    }
    World_ReadLooseFileBlock((int)World_BuildLoosePath(0, variant), blk * 0xB800, 0xB800, (char *)g_WorldStreamingSlot + req->slot * 0xB800, 0);
    g_WorldBlockLoadBudget = g_WorldMovementMultiplier * 5;
    g_WorldStreamDestSlot = req->slot;
    World_ArmBlockLoadBudgetAndClearInitStep0();
}

/*
 * When a selected block is ready before the five-times-delay threshold, finds its loading-list
 * node, reports error 0xB if absent, then unlinks it, pushes it onto the resident list and queues
 * its meshes. Clears the selected index, then decrements a nonzero delay below the threshold; a
 * missing node diagnostic does not prevent subsequent access.
 */
// FUNCTION: FF7 0x007511cd
void __cdecl World_PromoteLoadedBlock(void)
{
    struct {
        int pPrevElt;
        int pElt;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_WorldBlockLoadBudget < g_WorldMovementMultiplier * 5 && g_WorldStreamDestSlot >= 0) {
        for (frame.pPrevElt = 0, frame.pElt = g_WorldBlockSlotList;
             frame.pElt != 0 && frame.pElt != (int)((char *)&g_WorldProbeOffsets + g_WorldStreamDestSlot * 8);
             frame.pPrevElt = frame.pElt, frame.pElt = *(int *)frame.pElt) {
        }
        if (frame.pElt == 0) {
            WorldScript_ReportError(0xb);
        }
        if (frame.pPrevElt != 0) {
            *(int *)frame.pPrevElt = *(int *)frame.pElt;
        } else {
            g_WorldBlockSlotList = *(int *)frame.pElt;
        }
        *(int *)frame.pElt = g_WorldStreamingEnabled;
        g_WorldStreamingEnabled = frame.pElt;
        World_QueueMeshesForBlock(frame.pElt);
        g_WorldStreamDestSlot = -1;
    }
    g_WorldBlockLoadBudget -= (g_WorldBlockLoadBudget != 0 && g_WorldBlockLoadBudget < g_WorldMovementMultiplier * 5);
}

/*
 * When g_WorldStreamDestSlot is non-negative, sets the streaming budget g_WorldBlockLoadBudget to 5 * g_WorldMovementMultiplier -
 * 1 and calls World_LeaveInitStep(0).
 */
// FUNCTION: FF7 0x007512c2
void __cdecl World_ArmBlockLoadBudgetAndClearInitStep0(void)
{
    if (g_WorldStreamDestSlot >= 0) {
        g_WorldBlockLoadBudget = g_WorldMovementMultiplier * 5 - 1;
        World_LeaveInitStep(0);
    }
}

/*
 * Counts the block slots on the in-use list g_WorldBlockSlotList.
 */
// FUNCTION: FF7 0x007512ea
short __cdecl World_CountBlockSlotsInUse(void)
{
    int node;
    int count;

    count = 0;
    for (node = g_WorldBlockSlotList; node != 0; count++, node = *(int *)node) {
    }
    return (short)count;
}

/*
 * Queues decompression of every mesh record on the live list g_WorldMeshRecordList whose grid cell (its
 * chunkX/chunkY coordinates over 4, as row * 9 + column) equals the block number of request request.
 */
// FUNCTION: FF7 0x00751322
void __cdecl World_QueueMeshesForBlock(int request)
{
    WorldMeshRecord *mesh;

    for (mesh = (WorldMeshRecord *)g_WorldMeshRecordList; mesh != 0; mesh = (WorldMeshRecord *)mesh->next) {
        if ((mesh->chunkY >> 2) * 9 + (mesh->chunkX >> 2) == *(short *)((char *)request + 4)) {
            World_QueueMeshDecompress(mesh);
        }
    }
}

/*
 * Searches the mesh-decompression queue for a payload with the same two coordinate words as the
 * supplied mesh. If absent, reports error 0x16 when the free list is empty, pops a free node and
 * pushes a new payload at the queue head. The error path continues into node access.
 */
// FUNCTION: FF7 0x00751374
void __cdecl World_QueueMeshDecompress(WorldMeshRecord *mesh)
{
    WorldMeshRecordNode *node;

    for (node = (WorldMeshRecordNode *)g_pWorldMeshDecompressQueue;
         node != 0 && (((WorldMeshRecord *)node->mesh)->chunkX != mesh->chunkX ||
                    ((WorldMeshRecord *)node->mesh)->chunkY != mesh->chunkY);
         node = (WorldMeshRecordNode *)(node->next)) {
    }
    if (node == 0) {
        if (g_WorldFreeBlockNodeList == 0)
            WorldScript_ReportError(0x16);
        node = (WorldMeshRecordNode *)g_WorldFreeBlockNodeList;
        g_WorldFreeBlockNodeList = node->next;
        node->next = g_pWorldMeshDecompressQueue;
        g_pWorldMeshDecompressQueue = (int)node;
        node->mesh = (int)mesh;
    }
}

/*
 * Services the head mesh-decompression request while the queue is nonempty and the sampled
 * g_WorldStreamFocusSmoothing stop flag is zero. Derives its block ID from chunk coordinates divided by four,
 * finds that block in the active list and, when found, decompresses the chunk, removes it from the
 * pending chunk list and recycles both chunk and request nodes into their free lists. Reports
 * errors for a null request chunk or a chunk absent from its pending list, then proceeds without
 * an explicit early return. An unavailable block leaves the head request in place; with a zero
 * stop flag this can repeatedly retry it. Sets g_WorldStreamFocusSmoothing to 1 on exit.
 */
// FUNCTION: FF7 0x00751408
void __cdecl World_ServiceMeshDecompressQueue(void)
{
    typedef struct WorldMeshQueueBlock {
        struct WorldMeshQueueBlock *pNext;
        short wBlockId;
    } WorldMeshQueueBlock;
    typedef struct WorldMeshChunk {
        struct WorldMeshChunk *pNext;
        unsigned char pad04[0x0C];
        short wChunkX;
        short wChunkY;
    } WorldMeshChunk;
    typedef struct WorldMeshDecompressRequest {
        struct WorldMeshDecompressRequest *pNext;
        WorldMeshChunk *pChunk;
    } WorldMeshDecompressRequest;
    struct {
        WorldMeshQueueBlock *pBlock;
        int dwBlockId;
        WorldMeshChunk *pPrevChunk;
        int stopFlag;
        WorldMeshDecompressRequest *pRequest;
        WorldMeshChunk *pChunk;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    for (frame.stopFlag = 0; g_pWorldMeshDecompressQueue && !frame.stopFlag; frame.stopFlag = g_WorldStreamFocusSmoothing) {
        frame.pRequest = (WorldMeshDecompressRequest *)g_pWorldMeshDecompressQueue;
        if (frame.pRequest->pChunk == 0)
            WorldScript_ReportError(0xC);
        frame.dwBlockId = (frame.pRequest->pChunk->wChunkY >> 2) * 9 +
                         (frame.pRequest->pChunk->wChunkX >> 2);
        for (frame.pBlock = (WorldMeshQueueBlock *)g_WorldStreamingEnabled;
             frame.pBlock && frame.pBlock->wBlockId != frame.dwBlockId;
             frame.pBlock = frame.pBlock->pNext) {
        }
        if (frame.pBlock) {
            World_DecompressMeshIntoSlot((WorldMeshSlot *)frame.pRequest->pChunk, (int)frame.pBlock);
            for (frame.pPrevChunk = 0, frame.pChunk = (WorldMeshChunk *)g_WorldMeshRecordList;
                 frame.pChunk && frame.pChunk != frame.pRequest->pChunk;
                 frame.pPrevChunk = frame.pChunk, frame.pChunk = frame.pChunk->pNext) {
            }
            if (frame.pChunk == 0)
                WorldScript_ReportError(0x17);
            if (frame.pPrevChunk != 0)
                frame.pPrevChunk->pNext = frame.pChunk->pNext;
            else
                g_WorldMeshRecordList = (int)frame.pChunk->pNext;
            frame.pChunk->pNext = (WorldMeshChunk *)g_WorldStreamingState;
            g_WorldStreamingState = (WorldTerrainChunkInfo *)frame.pChunk;
            g_pWorldMeshDecompressQueue = (int)frame.pRequest->pNext;
            frame.pRequest->pNext = (WorldMeshDecompressRequest *)g_WorldFreeBlockNodeList;
            g_WorldFreeBlockNodeList = (int)frame.pRequest;
        }
    }
    g_WorldStreamFocusSmoothing = 1;
}

/*
 * Decompresses one mesh: finds the compressed data in block slot slotEntry's 0xB800-byte buffer (offset
 * table at the start, entry chosen by the mesh's position within the 4 x 4 block), LZS-
 * decompresses it into the record's 0x1200-byte slot in g_WorldMeshLoadState, then sets the record's
 * vertex pointer (+4), vertex count (+0x14), triangle pointer (+8) and end pointer (+0xC) from the
 * decompressed header and shrinks the UVs.
 */
// FUNCTION: FF7 0x0075155a
void __cdecl World_DecompressMeshIntoSlot(WorldMeshSlot *slot, int slotEntry)
{
    int source;
    int dst;
    int hdr;

    source = *(int *)&g_WorldStreamingSlot + (slotEntry - (int)&g_WorldProbeOffsets >> 3) * 0xB800;
    dst = ((int)slot - (int)&g_WorldBlockLoadSlots) / 0x18 * 0x1200 + (int)g_WorldMeshLoadState;
    source += (*(unsigned int *)((char *)source + ((slot->meshRow & 3) << 2 | slot->meshCol & 3) * 4) >> 2) * 4;
    World_LzsDecompress((unsigned char *)source, (unsigned char *)dst);
    hdr = dst;
    slot->vertices = hdr + 4;
    slot->vertexCount = *(short *)hdr;
    slot->faces = slot->vertices + slot->vertexCount * 0xC;
    slot->facesEnd = slot->faces + *(short *)((char *)hdr + 2) * 8;
    World_ShrinkMeshUVs((unsigned char *)slot->vertices, (unsigned char *)(slot->vertices + slot->vertexCount * 0xC), 1);
}

/*
 * Returns when the resident-mesh helper reports at least 16, otherwise builds a five-by-five
 * wanted mask around the player mesh and clears cells already present in either mesh list. For
 * each remaining cell wraps X once into 0 through 35 and Y once into 0 through 27 and acquires a
 * mesh record. Stores coordinates and derives its block ID; resident blocks queue decompression,
 * while blocks not already loading consume a free load request and enqueue it. Reports error 15
 * for an empty request list but still dereferences that list. Failed mesh acquisition skips that
 * cell; capacity is checked only before planning begins.
 */
// FUNCTION: FF7 0x0075164a
void __cdecl World_PlanResidentMeshes(int *position, short angle)
{
    struct {
        WorldBlockLoadInfo *block;
        short newX;
        short padX;
        int blockId;
        short centreY;
        short padY;
        short j;
        short padJ;
        short i;
        short padI;
        int mask[5][5];
        short centreX;
        short padCentre;
        WorldBlockLoadInfo *request;
        short newY;
        short padNew;
        WorldTerrainChunkInfo *chunk;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (World_ExpireStaleMeshRecordsAndCount() >= 16)
        return;
    WorldMap_PositionToMesh(position, 0, &s.centreX, &s.centreY);
    World_BuildMeshWantMask(position, angle, (void *)s.mask);
    for (s.chunk = g_WorldStreamingState; s.chunk; s.chunk = s.chunk->pNext) {
        if (World_MeshOffsetFromCentre((WorldMeshRecord *)s.chunk, s.centreX, s.centreY, &s.i, &s.j))
            s.mask[s.j + 2][s.i + 2] = 0;
    }
    for (s.chunk = (WorldTerrainChunkInfo *)g_WorldMeshRecordList; s.chunk; s.chunk = s.chunk->pNext) {
        if (World_MeshOffsetFromCentre((WorldMeshRecord *)s.chunk, s.centreX, s.centreY, &s.i, &s.j))
            s.mask[s.j + 2][s.i + 2] = 0;
    }
    for (s.j = 0; s.j < 5; s.j++) {
        for (s.i = 0; s.i < 5; s.i++) {
            if (s.mask[s.j][s.i]) {
                s.newX = s.i + s.centreX - 2;
                if (s.newX < 0)
                    s.newX += 36;
                else if (s.newX >= 36)
                    s.newX -= 36;
                s.newY = s.j + s.centreY - 2;
                if (s.newY < 0)
                    s.newY += 28;
                else if (s.newY >= 28)
                    s.newY -= 28;
                s.chunk = (WorldTerrainChunkInfo *)World_AcquireMeshRecord();
                if (s.chunk) {
                    s.chunk->wChunkX = s.newX;
                    s.chunk->wChunkY = s.newY;
                    s.blockId = (s.newY >> 2) * 9 + (s.newX >> 2);
                    s.block = World_FindResidentBlock(s.blockId);
                    if (s.block)
                        World_QueueMeshDecompress((WorldMeshRecord *)s.chunk);
                    else if (!World_IsBlockLoading(s.blockId)) {
                        if (!g_WorldActiveMeshBuffer)
                            WorldScript_ReportError(15);
                        s.request = g_WorldActiveMeshBuffer;
                        g_WorldActiveMeshBuffer = (WorldBlockLoadInfo *)s.request->pNext;
                        s.request->wBlockId = s.blockId;
                        World_EnqueueBlockLoad((WorldBlockLoadRequest *)s.request);
                    }
                }
            }
        }
    }
}

/*
 * Searches g_WorldStreamingEnabled's resident-block list for wBlockId, moves a found non-head entry to the
 * front and returns it; returns null when absent.
 */
// FUNCTION: FF7 0x007518f6
WorldBlockLoadInfo *__cdecl World_FindResidentBlock(short wBlockId)
{
    WorldBlockLoadInfo *pBlock;
    WorldBlockLoadInfo *pPrevElt;

    for (pPrevElt = 0, pBlock = (WorldBlockLoadInfo *)g_WorldStreamingEnabled;
         pBlock && pBlock->wBlockId != wBlockId;
         pPrevElt = pBlock, pBlock = pBlock->pNext) {
    }
    if (pBlock && pPrevElt) {
        pPrevElt->pNext = pBlock->pNext;
        pBlock->pNext = (WorldBlockLoadInfo *)g_WorldStreamingEnabled;
        g_WorldStreamingEnabled = (int)pBlock;
    }
    return pBlock;
}

/*
 * Returns whether g_WorldBlockSlotList's linked loading list contains the supplied block ID.
 */
// FUNCTION: FF7 0x00751962
int __cdecl World_IsBlockLoading(short wBlockId)
{
    WorldBlockLoadInfo *pElt;

    for (pElt = (WorldBlockLoadInfo *)g_WorldBlockSlotList;
         pElt && pElt->wBlockId != wBlockId;
         pElt = pElt->pNext) {
    }
    return pElt != 0;
}

/*
 * Postincrements each pending mesh lifetime word. For an old lifetime of at least 0x96, unlinks
 * the pending node into its free list and removes the corresponding mesh-decompression queue node
 * when present. Preserves traversal after removal and returns the number of surviving pending
 * nodes.
 */
// FUNCTION: FF7 0x0075199c
int __cdecl World_ExpireStaleMeshRecordsAndCount(void)
{
    WorldMeshRecord *recordPtr;
    WorldMeshRecordNode *pQueueNode;
    int previousQueue;
    WorldMeshRecord *previousRec;
    int alive;
    WorldMeshRecord *nextRec;

    alive = 0;
    previousRec = 0;
    recordPtr = (WorldMeshRecord *)g_WorldMeshRecordList;
    while (recordPtr != 0) {
        if ((recordPtr->staleFrames)++ >= 0x96) {
            if (previousRec != 0) {
                previousRec->next = recordPtr->next;
            } else {
                g_WorldMeshRecordList = recordPtr->next;
            }
            recordPtr->next = g_WorldMeshRecordFreeList;
            g_WorldMeshRecordFreeList = (int)recordPtr;
            previousQueue = 0;
            pQueueNode = (WorldMeshRecordNode *)g_pWorldMeshDecompressQueue;
            for (; pQueueNode != 0 && pQueueNode->mesh != (int)recordPtr; previousQueue = (int)pQueueNode, pQueueNode = (WorldMeshRecordNode *)(pQueueNode->next)) {
            }
            if (pQueueNode != 0) {
                if (previousQueue != 0) {
                    *(int *)previousQueue = pQueueNode->next;
                } else {
                    g_pWorldMeshDecompressQueue = pQueueNode->next;
                }
                pQueueNode->next = g_WorldFreeBlockNodeList;
                g_WorldFreeBlockNodeList = (int)pQueueNode;
            }
            if (previousRec != 0) {
                nextRec = (WorldMeshRecord *)(previousRec->next);
            } else {
                nextRec = (WorldMeshRecord *)g_WorldMeshRecordList;
            }
            recordPtr = nextRec;
        } else {
            previousRec = recordPtr;
            recordPtr = (WorldMeshRecord *)(recordPtr->next);
            alive++;
        }
    }
    return alive;
}

/*
 * Fills a 5x5 table of ints (rows of 0x14 bytes at mask, entry [row+2][col+2]) saying which meshes
 * around the position in pos (x at [0], z at [2]) should be resident: the 3x3 centre is always
 * wanted, and every outer cell is wanted only when it lies inside the view wedge, tested by
 * rotating the forward vector (0,0,-0x1000) by yaw +-0x380 through the rotation matrix and
 * checking the sign of the y component of each edge's cross product with the offset from the
 * position within its 0x2000-unit mesh.
 */
// FUNCTION: FF7 0x00751ac4
void __cdecl World_BuildMeshWantMask(int *pos, short yaw, void *mask)
{
    int m[8];
    short dir[4] = {0, 0, -0x1000, 0};
    short euler[4] = {0, 0, 0, 0};
    int zeroVec[4] = {0, 0, 0, 0};
    int pt1[4];
    int pt2[4];
    int rb[4];
    int ctr[4];
    short cellZ;
    short cx;
    int delta[4];
    int fl;
    int ro[4];

    SetMatrixTranslation(m, (void *)zeroVec);
    SetTransMatrix(m);
    euler[1] = -yaw + 0x380;
    RotMatrixXYZ((void *)euler, (void *)m);
    SetRotMatrix((void *)m);
    RotTransPoint((void *)dir, (int *)pt1, &fl);
    euler[1] = -yaw - 0x380;
    RotMatrixXYZ((void *)euler, (void *)m);
    SetRotMatrix((void *)m);
    RotTransPoint((void *)dir, (int *)pt2, &fl);
    ctr[0] = (pos[0] & 0x1FFF) - 0x1000;
    ctr[1] = 0;
    ctr[2] = (pos[2] & 0x1FFF) - 0x1000;
    delta[1] = 0;
    for (cellZ = -2; cellZ <= 2; cellZ++) {
        for (cx = -2; cx <= 2; cx++) {
            if ((cx > 0 ? cx : -cx) <= 1 && (cellZ > 0 ? cellZ : -cellZ) <= 1) {
                ((int *)((char *)mask + (cellZ + 2) * 0x14))[cx + 2] = 1;
            } else {
                delta[0] = (cx << 13) - ctr[0];
                delta[2] = (cellZ << 13) - ctr[2];
                CrossProductFixed12((VECTOR *)pt1, (VECTOR *)delta, (VECTOR *)ro);
                CrossProductFixed12((VECTOR *)pt2, (VECTOR *)delta, (VECTOR *)rb);
                ((int *)((char *)mask + (cellZ + 2) * 0x14))[cx + 2] = ro[1] < 0 && rb[1] > 0 ? 1 : 0;
            }
        }
    }
}

/*
 * Writes the differences from the record's chunkX/chunkY coordinates to offsetX/offsetZ, wrapping them
 * around 36/28 respectively into +/-18 and +/-14. Returns whether both absolute wrapped
 * differences are at most two.
 */
// FUNCTION: FF7 0x00751d44
int __cdecl World_MeshOffsetFromCentre(WorldMeshRecord *record, short centreX, short centreZ, short *offsetX, short *offsetZ)
{
    *offsetX = record->chunkX - centreX;
    if (*offsetX > 0x12) {
        *offsetX -= 0x24;
    } else if (*offsetX < -0x12) {
        *offsetX += 0x24;
    }
    *offsetZ = record->chunkY - centreZ;
    if (*offsetZ > 0xe) {
        *offsetZ -= 0x1c;
    } else if (*offsetZ < -0xe) {
        *offsetZ += 0x1c;
    }
    return ((*offsetX > 0) ? *offsetX : -*offsetX) <= 2 && ((*offsetZ > 0) ? *offsetZ : -*offsetZ) <= 2;
}

/*
 * Acquires a mesh record: from the free list g_WorldMeshRecordFreeList, or by unlinking the last record on the
 * cache list g_WorldStreamingState and telling WorldEntity_ClearTriangleCacheSlotsByKey which cell it held. The record is pushed on the
 * live list g_WorldMeshRecordList with its +0x16 cleared.
 */
// FUNCTION: FF7 0x00751e43
int __cdecl World_AcquireMeshRecord(void)
{
    WorldMeshRecord *rec;
    struct WorldMeshRecord *prev;

    rec = 0;
    if (g_WorldMeshRecordFreeList != 0) {
        rec = (WorldMeshRecord *)g_WorldMeshRecordFreeList;
        g_WorldMeshRecordFreeList = rec->next;
    } else if (g_WorldStreamingState != 0) {
        prev = 0;
        for (rec = (WorldMeshRecord *)g_WorldStreamingState; rec->next != 0; prev = (struct WorldMeshRecord *)rec, rec = (WorldMeshRecord *)(rec->next)) {
        }
        if (prev != 0) {
            prev->next = 0;
        } else {
            g_WorldStreamingState = 0;
        }
        WorldEntity_ClearTriangleCacheSlotsByKey(rec->chunkX, rec->chunkY);
    }
    if (rec != 0) {
        rec->next = g_WorldMeshRecordList;
        g_WorldMeshRecordList = (int)rec;
        rec->staleFrames = 0;
    }
    return (int)rec;
}

/*
 * Builds two eight-row ground-probe fans from wrapped previous-to-target displacement and walks
 * the needed streamed terrain chunks through a bounded ten-entry queue. Detaches processed chunks,
 * uploads/projects their geometry, resolves both probe fans, places entities and submits
 * triangles. Chooses the first fully resolved row in the preferred fan, or switches to the other
 * after the retry threshold, then applies player terrain, ground position and tilt and optionally
 * begins flood filling. Missing chunks skip resolution bookkeeping and lead to position reversion;
 * unsuccessful resolution also reverts position and sets the terrain-14 delay. Finally renders
 * additional chunks selected by a 5 by 5 want mask and reconnects all detached chunks to the
 * stream list. Probe indices, matrix setup and allocation behavior retain their source/helper
 * assumptions.
 */
// FUNCTION: FF7 0x00751efc
void __cdecl World_WalkTerrainAndPlaceEntities(VECTOR *previous, VECTOR *position, VECTOR *target, short angle)
{
    struct {
        WorldProbeRow rows[2][8];
        PsxMatrix32 matrix;
        WorldProbeRow **queueEnd;
        int increment;
        short terrainY, padTY;
        SVECTOR playerLocal;
        short dy, padDY;
        WorldProbeRow **queue;
        short dx, padDX;
        WorldProbeRow *row;
        WorldProbeRow *pending[10];
        SVECTOR delta;
        WorldTerrainChunkInfo *previousChunk;
        int mask[5][5];
        short terrainX, padTX;
        VECTOR *resolved;
        SVECTOR translation;
        int distance;
        WorldProbeRow **scan;
        WorldGroundProbe *probe;
        VECTOR view;
        SVECTOR rotation;
        WorldTerrainChunkInfo *head, *tail;
        short playerX, padPX, playerY, padPY;
        SVECTOR viewLocal;
        WorldTerrainChunkInfo *chunk;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.resolved = 0;
    s.head = 0;
    s.tail = 0;
    g_WorldBlockLoadIndex = 0;
    s.delta.vy = 0;
    s.distance = target->vx - previous->vx;
    if (s.distance < -(18 << 13))
        s.distance += 36 << 13;
    else if (s.distance >= (18 << 13))
        s.distance -= 36 << 13;
    s.delta.vx = s.distance;
    s.distance = target->vz - previous->vz;
    if (s.distance < -(14 << 13))
        s.distance += 28 << 13;
    else if (s.distance >= (14 << 13))
        s.distance -= 28 << 13;
    s.delta.vz = s.distance;
    SetMatrixTranslation(&s.matrix, previous);
    s.rotation.vx = s.rotation.vz = 0;
    WorldMap_PositionToMesh((int *)position, &s.playerLocal, &s.playerX, &s.playerY);
    World_UpdateStreamFocus((WorldStreamFocusData *)position, 0);
    World_GetStreamFocus((WorldStreamFocusData *)&s.view);
    WorldMap_PositionToMesh((int *)&s.view, &s.viewLocal, &s.terrainX, &s.terrainY);
    if (s.terrainX - s.playerX > 18)
        s.terrainX -= 36;
    else if (s.terrainX - s.playerX < -18)
        s.terrainX += 36;
    if (s.terrainY - s.playerY > 14)
        s.terrainY -= 28;
    else if (s.terrainY - s.playerY < -14)
        s.terrainY += 28;
    if (s.terrainX > s.playerX)
        s.viewLocal.vx += 8192;
    else if (s.terrainX < s.playerX)
        s.viewLocal.vx -= 8192;
    if (s.terrainY > s.playerY)
        s.viewLocal.vz += 8192;
    else if (s.terrainY < s.playerY)
        s.viewLocal.vz -= 8192;
    if (World_IsControlLockClear()) {
        // STRING: FF7 0x00969b38
        sprintf(g_WorldWalkDebugText, "PS%d%d%d%d.rsd  POS: %d %d", s.playerX / 10, s.playerX % 10, s.playerY / 10, s.playerY % 10, position->vx & 0x1fff, position->vz & 0x1fff);
    }
    s.rotation.vy = 0;
    SetTransMatrix(&s.matrix);
    s.distance = WorldEntity_GetPlayerModelId() == WORLD_MODEL_TINY_BRONCO ? 350 : 200;
    s.increment = (0x500 << ((g_WorldTerrainWalkPhase != 1 && g_WorldTerrainBlockCountdown == 0) ? 0 : 1)) >> 3;
    for (s.row = &s.rows[0][0]; s.row < &s.rows[0][8]; s.row++) {
        World_FillProbeRow(s.row, &s.delta, &s.rotation, s.distance);
        s.rotation.vy += s.increment;
    }
    s.rotation.vy = 0;
    for (s.row = &s.rows[1][0]; s.row < &s.rows[1][8]; s.row++) {
        World_FillProbeRow(s.row, &s.delta, &s.rotation, s.distance);
        s.rotation.vy -= s.increment;
    }
    s.pending[0] = &s.rows[0][0];
    s.queueEnd = &s.pending[1];
    s.queue = &s.pending[0];
    while (s.queue < s.queueEnd) {
        s.terrainX = (*s.queue)->probes[0].x;
        s.terrainY = (*s.queue)->probes[0].y;
        if (s.queue == &s.pending[0])
            WorldEntity_MarkPlayerOnMesh();
        s.queue++;
        for (s.previousChunk = 0, s.chunk = g_WorldStreamingState;
             s.chunk && (s.chunk->wChunkX != s.terrainX || s.chunk->wChunkY != s.terrainY);
             s.previousChunk = s.chunk, s.chunk = s.chunk->pNext) {
        }
        if (s.chunk == 0) {
            WorldEntity_ClearPlayerOnMesh();
            goto no_mesh;
        }
        if (s.previousChunk)
            s.previousChunk->pNext = s.chunk->pNext;
        else
            g_WorldStreamingState = (WorldTerrainChunkInfo *)s.chunk->pNext;
        s.chunk->pNext = 0;
        if (s.head == 0)
            s.head = s.tail = s.chunk;
        if (s.tail == 0)
            WorldScript_ReportError(17);
        s.tail->pNext = s.chunk;
        s.tail = s.chunk;
        World_MeshOffsetFromCentre((WorldMeshRecord *)s.chunk, s.playerX, s.playerY, &s.dx, &s.dy);
        s.translation.vx = (s.dx << 13) - s.viewLocal.vx;
        s.translation.vy = -s.viewLocal.vy;
        s.translation.vz = (s.dy << 13) - s.viewLocal.vz;
        World_SetViewTransformForPosition(&s.translation);
        World_UploadMeshVertices((short *)s.chunk->vertices, World_GetFadeTarget());
        GetRotMatrix((PsxMatrix *)&s.matrix);
        if (s.queue == &s.pending[1])
            World_ApplyTerrainCurrent(s.playerX, s.playerY);
        s.probe = World_ResolveProbesOnMesh(s.chunk, s.rows[g_WorldCurrentProbeRow], s.terrainX, s.terrainY);
        if (s.probe) {
            for (s.scan = s.queue; s.scan < s.queueEnd && ((*s.scan)->probes[0].x != s.probe->x || (*s.scan)->probes[0].y != s.probe->y); s.scan++) {
            }
            if (s.scan >= s.queueEnd) {
                if (s.queueEnd < &s.pending[10]) {
                    *(WorldGroundProbe **)s.queueEnd = s.probe;
                    s.queueEnd++;
                } else
                    WorldScript_ReportError(20);
            }
        }
        s.probe = World_ResolveProbesOnMesh(s.chunk, s.rows[!g_WorldCurrentProbeRow], s.terrainX, s.terrainY);
        if (s.probe) {
            for (s.scan = s.queue; s.scan < s.queueEnd && ((*s.scan)->probes[0].x != s.probe->x || (*s.scan)->probes[0].y != s.probe->y); s.scan++) {
            }
            if (s.scan >= s.queueEnd) {
                if (s.queueEnd < &s.pending[10]) {
                    *(WorldGroundProbe **)s.queueEnd = s.probe;
                    s.queueEnd++;
                } else
                    WorldScript_ReportError(21);
            }
        }
        for (s.row = &s.rows[0][0]; s.row < &s.rows[1][8]; s.row++) {
            for (s.probe = &s.row->probes[0]; s.probe < &s.row->probes[5]; s.probe++) {
                if (s.probe->x == s.terrainX && s.probe->y == s.terrainY && s.probe->state == 1)
                    s.probe->state = 4;
            }
        }
        SetRotMatrix(&s.matrix);
        World_PlaceEntitiesOnMesh((int)s.chunk);
        World_SetViewTransformForPosition(&s.translation);
        World_ProjectMeshVertices();
        World_SubmitTerrainChunkTriangles(&s.chunk->triangles[0], &s.chunk->triangles[s.chunk->count], (short (*)[4])s.chunk->normals, (char *)World_AllocRenderVertexBlock(s.chunk->count));
    }
    for (s.row = &s.rows[g_WorldCurrentProbeRow][0]; s.row < &s.rows[g_WorldCurrentProbeRow][8] && (s.row->probes[0].state != 2 || s.row->probes[1].state != 2 || s.row->probes[2].state != 2 || s.row->probes[3].state != 2 || s.row->probes[4].state != 2); s.row++) {
    }
    if (s.row < &s.rows[g_WorldCurrentProbeRow][8]) {
        *(VECTOR *)&g_WorldLoadedBlockIds = s.row->probes[0].position;
        ((VECTOR *)&g_WorldLoadedBlockIds)->vy = s.row->probes[0].height;
        WorldEntity_SetPlayerTerrainInfo(s.row->probes[0].terrain);
        WorldEntity_ApplyPlayerGroundStep(&g_WorldLoadedBlockIds);
        s.resolved = (VECTOR *)&g_WorldLoadedBlockIds;
        if (g_WorldBlockLoadInfoCount) {
            g_WorldBlockLoadInfoCount = 0;
            World_BeginTerrainFloodFill((int)s.row->probes[0].chunk, (int)s.row->probes[0].triangle);
        }
        g_WorldCurrentTerrainChunk = (int)s.row->probes[0].chunk;
        g_pWorldCurrentProbeTriangle = (int)s.row->probes[0].triangle;
        g_WorldPlayerTerrainTriangle = (int)s.row->probes[0].triangle;
        WorldEntity_EaseCurrentTilt(s.row->probes[0].angle);
    } else if (g_WorldProbeRowCounter >= 8) {
        for (s.row = &s.rows[!g_WorldCurrentProbeRow][0]; s.row < &s.rows[!g_WorldCurrentProbeRow][8] && (s.row->probes[0].state != 2 || s.row->probes[1].state != 2 || s.row->probes[2].state != 2 || s.row->probes[3].state != 2 || s.row->probes[4].state != 2); s.row++) {
        }
        if (s.row < &s.rows[!g_WorldCurrentProbeRow][8]) {
            *(VECTOR *)&g_WorldLoadedBlockIds = s.row->probes[0].position;
            ((VECTOR *)&g_WorldLoadedBlockIds)->vy = s.row->probes[0].height;
            WorldEntity_SetPlayerTerrainInfo(s.row->probes[0].terrain);
            WorldEntity_ApplyPlayerGroundStep(&g_WorldLoadedBlockIds);
            s.resolved = (VECTOR *)&g_WorldLoadedBlockIds;
            if (g_WorldBlockLoadInfoCount) {
                g_WorldBlockLoadInfoCount = 0;
                World_BeginTerrainFloodFill((int)s.row->probes[0].chunk, (int)s.row->probes[0].triangle);
            }
            g_WorldCurrentTerrainChunk = (int)s.row->probes[0].chunk;
            g_pWorldCurrentProbeTriangle = (int)s.row->probes[0].triangle;
            g_WorldPlayerTerrainTriangle = (int)s.row->probes[0].triangle;
            WorldEntity_EaseCurrentTilt(s.row->probes[0].angle);
            g_WorldCurrentProbeRow = !g_WorldCurrentProbeRow;
            g_WorldProbeRowCounter = 0;
        }
    }
    g_WorldBlockLoadIndex = 1;
    if (s.resolved && g_WorldTerrainWalkPhase != 0)
        g_WorldTerrainWalkPhase = 0;
    else if (s.resolved == 0 && g_WorldTerrainWalkPhase == 2)
        g_WorldTerrainWalkPhase = 1;
    g_WorldTerrainBlockCountdown -= g_WorldTerrainBlockCountdown > 0;
no_mesh:
    if (s.resolved == 0) {
        WorldEntity_RevertPlayerPosition();
        if (WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_WUTAI_BRIDGE)
            g_WorldTerrainBlockCountdown = 5;
    }
    g_WorldProbeRowCounter++;
    WorldMap_PositionToMesh((int *)position, &s.playerLocal, &s.terrainX, &s.terrainY);
    World_BuildMeshWantMask((int *)position, angle, (void *)s.mask);
    s.previousChunk = 0, s.chunk = g_WorldStreamingState;
    while (s.chunk) {
        if (World_MeshOffsetFromCentre((WorldMeshRecord *)s.chunk, s.terrainX, s.terrainY, &s.dx, &s.dy) && s.mask[s.dy + 2][s.dx + 2]) {
            s.translation.vx = (s.dx << 13) - s.viewLocal.vx;
            s.translation.vy = -s.viewLocal.vy;
            s.translation.vz = (s.dy << 13) - s.viewLocal.vz;
            World_SetViewTransformForPosition(&s.translation);
            World_UploadMeshVertices((short *)s.chunk->vertices, World_GetFadeTarget());
            World_PlaceEntitiesOnMesh((int)s.chunk);
            World_SetViewTransformForPosition(&s.translation);
            World_ProjectMeshVertices();
            World_SubmitTerrainChunkTriangles(&s.chunk->triangles[0], &s.chunk->triangles[s.chunk->count], (short (*)[4])s.chunk->normals, (char *)World_AllocRenderVertexBlock(s.chunk->count));
            if (s.previousChunk)
                s.previousChunk->pNext = s.chunk->pNext;
            else
                g_WorldStreamingState = (WorldTerrainChunkInfo *)s.chunk->pNext;
            s.chunk->pNext = 0;
            if (s.head == 0)
                s.head = s.tail = s.chunk;
            if (s.tail == 0)
                WorldScript_ReportError(19);
            s.tail->pNext = s.chunk;
            s.tail = s.chunk;
            s.chunk = s.previousChunk ? s.previousChunk->pNext : g_WorldStreamingState;
            continue;
        }
        s.previousChunk = s.chunk, s.chunk = s.chunk->pNext;
    }
    if (s.head && s.tail) {
        s.tail->pNext = g_WorldStreamingState;
        g_WorldStreamingState = (WorldTerrainChunkInfo *)s.head;
    }
    return;
}

/*
 * Scans up to eight rows of five ground probes in state 1. Probes whose mesh indices match x/y
 * receive the chunk and player-height seed and are sampled through World_SampleGroundAtEntity,
 * becoming state 2 on success or 3 on failure. During vehicle transition 2 for the qualifying
 * player model, success also requires height difference strictly below 200. Records the first
 * pending probe for another mesh. Stops early only when row 0 has all five probes in state 2;
 * otherwise scans all eight rows and returns that first pending pointer or null. Retrieves player
 * position, triangle cache and model once; the shifted local mask is unused.
 */
// FUNCTION: FF7 0x00752d02
WorldGroundProbe *__cdecl World_ResolveProbesOnMesh(WorldTerrainChunkInfo *chunk, WorldProbeRow *rows, short x, short y)
{
    struct {
        int mask;
        WorldGroundProbe *pending;
        int model;
        WorldTriangleCache *cache;
        int complete;
        VECTOR position;
        WorldGroundProbe *probe;
        int row;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.row = 0;
    s.mask = 4;
    s.pending = 0;
    WorldEntity_GetPlayerPosition((VECTOR *)&s.position);
    s.cache = (WorldTriangleCache *)WorldEntity_GetPlayerTriangleCache();
    s.model = WorldEntity_GetCurrentModelId();
    if (World_GetVehicleTransitionState() == 2 && WorldEntity_IsPlayerRideable()) {
        do {
            for (s.probe = &rows[s.row].probes[0]; s.probe < &rows[s.row].probes[5]; s.probe++) {
                if (s.probe->state == 1) {
                    if (s.probe->x == x && s.probe->y == y) {
                        s.probe->chunk = chunk;
                        s.probe->height = s.position.vy;
                        if (World_SampleGroundAtEntity(chunk, &s.probe->local, s.cache, &s.probe->height, &s.probe->triangle, &s.probe->terrain, s.model) && (s.probe->height - s.position.vy > 0 ? s.probe->height - s.position.vy : -(s.probe->height - s.position.vy)) < 200)
                            s.probe->state = 2;
                        else
                            s.probe->state = 3;
                    } else if (s.pending == 0)
                        s.pending = s.probe;
                }
            }
            s.complete = rows[s.row].probes[0].state == 2 && rows[s.row].probes[1].state == 2 && rows[s.row].probes[2].state == 2 && rows[s.row].probes[3].state == 2 && rows[s.row].probes[4].state == 2;
            if (s.row == 0 && s.complete)
                break;
            s.row++;
            s.mask >>= 1;
        } while (s.row < 8);
    } else {
        do {
            for (s.probe = &rows[s.row].probes[0]; s.probe < &rows[s.row].probes[5]; s.probe++) {
                if (s.probe->state == 1) {
                    if (s.probe->x == x && s.probe->y == y) {
                        s.probe->chunk = chunk;
                        s.probe->height = s.position.vy;
                        if (World_SampleGroundAtEntity(chunk, &s.probe->local, s.cache, &s.probe->height, &s.probe->triangle, &s.probe->terrain, s.model))
                            s.probe->state = 2;
                        else
                            s.probe->state = 3;
                    } else if (s.pending == 0)
                        s.pending = s.probe;
                }
            }
            s.complete = rows[s.row].probes[0].state == 2 && rows[s.row].probes[1].state == 2 && rows[s.row].probes[2].state == 2 && rows[s.row].probes[3].state == 2 && rows[s.row].probes[4].state == 2;
            if (s.row == 0 && s.complete)
                break;
            s.row++;
            s.mask >>= 1;
        } while (s.row < 8);
    }
    return s.pending;
}

/*
 * Builds and installs an XYZ rotation matrix, transforms position, and initializes five ground
 * probes at that transformed point and at plus/minus distance along X and Z. For each computes
 * mesh-local coordinates and indices through WorldMap_PositionToMesh, sets state 1, clears height
 * and triangle, and copies rotation vy as angle. Leaves the installed global rotation matrix as a
 * side effect.
 */
// FUNCTION: FF7 0x007530b3
void __cdecl World_FillProbeRow(WorldProbeRow *row, SVECTOR *position, SVECTOR *rotation, int distance)
{
    struct {
        PsxMatrix32 matrix;
        VECTOR position;
        int flags;
        WorldGroundProbe *probe;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    RotMatrixXYZ(rotation, &s.matrix);
    SetRotMatrix(&s.matrix);
    s.probe = &row->probes[0];
    RotTransPoint((void *)position, (int *)&s.position, &s.flags);
    s.probe->position = s.position;
    WorldMap_PositionToMesh((int *)&s.probe->position, &s.probe->local, &s.probe->x, &s.probe->y);
    s.probe->state = 1;
    s.probe->height = 0;
    s.probe->triangle = 0;
    s.probe->angle = rotation->vy;
    s.probe++;
    s.probe->position = s.position;
    s.probe->position.vx -= distance;
    WorldMap_PositionToMesh((int *)&s.probe->position, &s.probe->local, &s.probe->x, &s.probe->y);
    s.probe->state = 1;
    s.probe->height = 0;
    s.probe->triangle = 0;
    s.probe->angle = rotation->vy;
    s.probe++;
    s.probe->position = s.position;
    s.probe->position.vx += distance;
    WorldMap_PositionToMesh((int *)&s.probe->position, &s.probe->local, &s.probe->x, &s.probe->y);
    s.probe->state = 1;
    s.probe->height = 0;
    s.probe->triangle = 0;
    s.probe->angle = rotation->vy;
    s.probe++;
    s.probe->position = s.position;
    s.probe->position.vz -= distance;
    WorldMap_PositionToMesh((int *)&s.probe->position, &s.probe->local, &s.probe->x, &s.probe->y);
    s.probe->state = 1;
    s.probe->height = 0;
    s.probe->triangle = 0;
    s.probe->angle = rotation->vy;
    s.probe++;
    s.probe->position = s.position;
    s.probe->position.vz += distance;
    WorldMap_PositionToMesh((int *)&s.probe->position, &s.probe->local, &s.probe->x, &s.probe->y);
    s.probe->state = 1;
    s.probe->height = 0;
    s.probe->triangle = 0;
    s.probe->angle = rotation->vy;
}

/*
 * Empty. A streaming hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00753348
void __cdecl World_TerrainStreamingPostUpdateNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x00753348,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Requests a terrain flood fill (g_WorldBlockLoadInfoCount = 1).
 */
// FUNCTION: FF7 0x0075334d
void __cdecl World_RequestTerrainFloodFill(void)
{
    g_WorldBlockLoadInfoCount = 1;
}

/*
 * Returns g_WorldBlockLoadIndex.
 */
// FUNCTION: FF7 0x0075335c
int __cdecl World_GetTerrainWalkCompletedFlag(void)
{
    return g_WorldBlockLoadIndex;
}

/*
 * Returns whether g_WorldStreamingState's linked terrain list contains an entry matching both chunk
 * coordinates.
 */
// FUNCTION: FF7 0x00753366
int __cdecl World_IsTerrainChunkLoaded(short wChunkX, short wChunkY)
{
    WorldTerrainChunkInfo *chunk;

    for (chunk = g_WorldStreamingState;
         chunk && (chunk->wChunkX != wChunkX || chunk->wChunkY != wChunkY);
         chunk = chunk->pNext) {
    }
    return chunk != 0;
}

/*
 * Derives a terrain-block column and row from player coordinates minus 0x4000 shifted right by 15,
 * adjusts map variant 2 by columns 3 and rows 2, and adds the variant dimensions once for negative
 * coordinates. On variant 0 applies the four progress-dependent block substitutions. Reads 0x2E000
 * bytes from the selected loose path at block*0xB800*4 into g_WorldStreamingSlot, resets the active/free
 * slot heads, and assigns four block IDs. Variant 2 uses three columns and four rows offset into
 * the nine-column map; variant 3 uses a two-by-two mask; others wrap over nine columns and seven
 * rows. Does not test the file-read result.
 */
// FUNCTION: FF7 0x007533af
void __cdecl World_PreloadBlocksFromBot(void)
{
    int position[4];
    int zOffset;
    int column;
    int blockRow;
    int xx;
    int block;
    int mode;

    WorldEntity_GetPlayerPosition((VECTOR *)position);
    column = position[0] - 0x4000 >> 15;
    blockRow = position[2] - 0x4000 >> 15;
    mode = World_GetMapVariant();
    if (mode == 2) {
        column -= 3;
        blockRow -= 2;
    }
    if (column < 0) {
        column += g_WorldWrapGridCols[mode];
    }
    if (blockRow < 0) {
        blockRow += g_WorldWrapGridRows[mode];
    }
    block = g_WorldWrapGridCols[mode] * blockRow + column;
    if (mode == 0) {
        if (g_WorldProgress > 0 && column >= 4 && column <= 5 && blockRow >= 4 && blockRow <= 5) {
            block = column + blockRow * 2 + 0x33;
        }
        if (g_WorldProgress > 1 && column >= 4 && column <= 6 && blockRow >= 3 && blockRow <= 4) {
            block = (blockRow - 3) * 3 + column + 0x3F;
        }
        if (g_WorldProgress > 2 && column >= 5 && column <= 6 && blockRow >= 5) {
            block = column + blockRow * 2 + 0x3A;
        }
        if (g_WorldProgress > 3 && column >= 1 && column <= 3 && blockRow >= 4 && blockRow <= 5) {
            block = (blockRow - 4) * 3 + column + 0x4C;
        }
    }
    World_ReadLooseFileBlock((int)World_BuildLoosePath(1, mode), block * 0xB800 << 2, 0x2E000, g_WorldStreamingSlot, 0);
    *(int *)&g_WorldMeshBlockBuffer = (int)&g_WorldProbeOffsets + 0x20;
    g_WorldStreamingEnabled = (int)&g_WorldProbeOffsets;
    *(int *)((char *)&g_WorldProbeOffsets + 0x18) = 0;
    for (zOffset = 0; zOffset < 2; zOffset++) {
        for (xx = 0; xx < 2; xx++) {
            *(short *)((char *)((char *)&g_WorldProbeOffsets + 4) + (zOffset * 2 + xx) * 8) =
                mode == 2   ? ((blockRow + zOffset) % 4 + 2) * 9 + (column + xx) % 3 + 3
                : mode == 3 ? ((blockRow + zOffset) & 1) * 9 + ((column + xx) & 1)
                            : (blockRow + zOffset) % 7 * 9 + (column + xx) % 9;
        }
    }
}

/*
 * Reports error 0xE when g_WorldTerrainStreamingEnabled is nonzero, then appends the whole g_WorldMeshQueueTail list to
 * g_WorldActiveMeshBuffer and the g_WorldBlockSlotList list to the free-slot list, clearing both original heads.
 * Walks the active list g_WorldStreamingEnabled and removes nodes whose unsigned address is at or beyond
 * slot-table base plus slotIndex*8, pushing each onto the free list while retaining earlier slots.
 * Returns block-buffer base plus slotIndex*0xB800. Does not validate slotIndex or stop explicitly after
 * reporting the error.
 */
// FUNCTION: FF7 0x0075363d
int __cdecl World_TrimBlockCacheTo(short slotIndex)
{
    WorldBlockLoadRequest *pRequest;
    int node;
    int last;
    int prev;
    int next;

    if (g_WorldTerrainStreamingEnabled != 0) {
        WorldScript_ReportError(0xE);
    }
    last = 0;
    pRequest = (WorldBlockLoadRequest *)g_WorldMeshQueueTail;
    for (; pRequest != 0; last = (int)pRequest, pRequest = (WorldBlockLoadRequest *)(pRequest->next)) {
    }
    if (last != 0) {
        *(int *)last = *(int *)&g_WorldActiveMeshBuffer;
        *(int *)&g_WorldActiveMeshBuffer = g_WorldMeshQueueTail;
        g_WorldMeshQueueTail = 0;
    }
    prev = 0;
    node = g_WorldBlockSlotList;
    for (; node != 0; prev = node, node = *(int *)node) {
    }
    if (prev != 0) {
        *(int *)prev = *(int *)&g_WorldMeshBlockBuffer;
        *(int *)&g_WorldMeshBlockBuffer = g_WorldBlockSlotList;
        g_WorldBlockSlotList = 0;
    }
    prev = 0;
    node = g_WorldStreamingEnabled;
    while (node != 0) {
        if ((unsigned int)node >=
            (unsigned int)((char *)&g_WorldProbeOffsets + slotIndex * 8)) {
            if (prev != 0) {
                *(int *)prev = *(int *)node;
            } else {
                g_WorldStreamingEnabled = *(int *)node;
            }
            *(int *)node = *(int *)&g_WorldMeshBlockBuffer;
            *(int *)&g_WorldMeshBlockBuffer = node;
            if (prev != 0) {
                next = *(int *)prev;
            } else {
                next = g_WorldStreamingEnabled;
            }
            node = next;
        } else {
            prev = node;
            node = *(int *)node;
        }
    }
    return *(int *)&g_WorldStreamingSlot + slotIndex * 0xB800;
}

/*
 * Sets walker-unstick mode g_WorldTerrainWalkPhase to 2 and stores the supplied value in g_WorldPendingBlockId.
 */
// FUNCTION: FF7 0x0075378a
void __cdecl World_ArmWalkerUnstick(int value)
{
    g_WorldTerrainWalkPhase = 2;
    g_WorldPendingBlockId = value;
}

/*
 * Sets the world progress value g_WorldProgress.
 */
// FUNCTION: FF7 0x007537a1
void __cdecl World_SetWorldProgress(int value)
{
    g_WorldProgress = value;
}

/*
 * Selects three view vectors from the pointer at g_WorldCurrentTerrainChunk+0xC using the three index bytes at
 * g_pWorldCurrentProbeTriangle and writes their component sums to sumOut; no division or bounds check is performed.
 */
// FUNCTION: FF7 0x007537ae
void __cdecl World_SumThreeViewVectors(int *sumOut)
{
    SVECTOR *viewC;
    SVECTOR *pViewB;
    SVECTOR *pViewA;

    pViewA = (SVECTOR *)*(int *)((char *)g_WorldCurrentTerrainChunk + 0x0C) + *(unsigned char *)g_pWorldCurrentProbeTriangle;
    pViewB = (SVECTOR *)*(int *)((char *)g_WorldCurrentTerrainChunk + 0x0C) + *((unsigned char *)g_pWorldCurrentProbeTriangle + 1);
    viewC = (SVECTOR *)*(int *)((char *)g_WorldCurrentTerrainChunk + 0x0C) + *((unsigned char *)g_pWorldCurrentProbeTriangle + 2);
    sumOut[0] = pViewA->vx + pViewB->vx + viewC->vx;
    sumOut[1] = pViewA->vy + pViewB->vy + viewC->vy;
    sumOut[2] = pViewA->vz + pViewB->vz + viewC->vz;
}
} /* extern "C" */

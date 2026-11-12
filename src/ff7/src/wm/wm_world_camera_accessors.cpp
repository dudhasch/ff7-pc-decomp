// src/ff7/src/wm/wm_world_camera_accessors.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Provides the world map camera getters and setters for yaw, pitch, zoom, roll, mode and
//   distance, along with the player control flags. It also includes a few terrain entry queries and
//   helpers that build asset and debug paths and allocate render vertex blocks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/wm.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdio.h>
#include "lib/codegen.h"
#include "ff7/api/field.h"
#include "ff7/api/main.h"
#include "ff7/api/wm.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "ff7/types/wm.h"
#include "lib/enums/file.h"
extern "C" {

#pragma intrinsic(memset, strcpy, strcat, strcmp)
// clang-format off
// GLOBAL: FF7 0x009699b0
short g_WorldElevationTable[24] = {0, 8, 16, 24, 32, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 0x0000, 0x0000};
// GLOBAL: FF7 0x009699e0
int g_WorldCameraRange = 400;
int g_WorldCameraPresetDataTail[10] = {
    0x0, 0xc, 0x22, 0x0, (int)((char *)&g_WorldAssetNameStringPool + 0x0), (int)((char *)&g_WorldAssetNameStringPool + 0x8),
    (int)((char *)&g_WorldAssetNameStringPool + 0x10), (int)((char *)&g_WorldAssetNameStringPool + 0x18), (int)((char *)&g_WorldAssetNameStringPool + 0x20), 0x0
};
// World_BuildLoosePath's name suffixes: the words are the addresses of ".MAP" .. "S.TXZ" at 0x969AA0..0x969AC4,
// which the raw fill g_WorldAssetNameStringPool holds at +0x28..+0x4c.
// GLOBAL: FF7 0x00969a10
char * g_WorldAssetNames[6] = {(char *)g_WorldAssetNameStringPool + 0x28, (char *)g_WorldAssetNameStringPool + 0x30, (char *)g_WorldAssetNameStringPool + 0x38, (char *)g_WorldAssetNameStringPool + 0x40, (char *)g_WorldAssetNameStringPool + 0x44, (char *)g_WorldAssetNameStringPool + 0x4c};
static __inline void World_AppendLoosePrefix(void)
{
    // STRING: FF7 0x00969acc
    strcat(g_WorldDebugBuffer, "wm/WM");
}
// WorldMap_BuildAssetName's name suffixes (retail strings 0x969AD4..0x969AF8): initializer literals.
// GLOBAL: FF7 0x00969a28
const char * g_WorldAssetSuffixes[6] = {".MAP", ".BOT", ".TXZ", ".EV", ".BSZ", "S.TXZ"};
// g_WorldChocoboTerrainMasks is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0x969a40..0x969a58 are aliases.
// GLOBAL: FF7 0x00969a40
unsigned char g_WorldChocoboTerrainMasks[0x18] = {0x83, 0x6f, 0x1b, 0x32, 0x87, 0x6f, 0x1b, 0x32, 0xf3, 0x6f, 0x1b, 0x32, 0xf7, 0x7f, 0x5b, 0x32, 0xff, 0x7f, 0x5b, 0x37, 0x00, 0x00, 0x00, 0x00};
// g_WorldTerrainTable is indexed by the code as an array. The span runs to the next named
// definition of the TU; the TU's pieces inside 0x969a58..0x969a68 are aliases.
// GLOBAL: FF7 0x00969a58
unsigned char g_WorldTerrainTable[0x10] = {0xdc, 0x05, 0x00, 0x00, 0x08, 0x07, 0x00, 0x00, 0x08, 0x07, 0x00, 0x00, 0x08, 0x07, 0x00, 0x00};
// the four camera-height ints the code reads as g_WorldCameraHeights[g_WorldCameraViewtype]
// (10000, 7000, 7000, 4000); the last two were the head of the former fill at 0x969A70, whose rest
// (the asset suffix strings) starts at 0x969A78 now.
// GLOBAL: FF7 0x00969a68
int g_WorldCameraHeights[4] = {10000, 7000, 7000, 4000};
unsigned char g_WorldAssetNameStringPool[84] = {
0x53, 0x43, 0x52, 0x2d, 0x48, 0x00, 0x00, 0x00, 0x46, 0x52, 0x4f, 0x4d, 0x20, 0x00, 0x00, 0x00,
0x4a, 0x55, 0x4d, 0x50, 0x20, 0x00, 0x00, 0x00, 0x4f, 0x4c, 0x44, 0x20, 0x20, 0x00, 0x00, 0x00,
0x4e, 0x45, 0x57, 0x20, 0x20, 0x00, 0x00, 0x00, 0x2e, 0x4d, 0x41, 0x50, 0x00, 0x00, 0x00, 0x00,
0x2e, 0x42, 0x4f, 0x54, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x54, 0x58, 0x5a, 0x00, 0x00, 0x00, 0x00,
0x2e, 0x45, 0x56, 0x00, 0x2e, 0x42, 0x53, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x53, 0x2e, 0x54, 0x58,
0x5a, 0x00, 0x00, 0x00};
unsigned char g_bssPad_00de6a00[4] = {0};
// GLOBAL: FF7 0x00de6a04
int g_WorldCameraDeltaY = 0;
// GLOBAL: FF7 0x00de6a08
int g_WorldCameraBlendDirection = 0;
// GLOBAL: FF7 0x00de6a0c
int g_WorldCameraHeight = 0;
// GLOBAL: FF7 0x00de6a10
int g_WorldRenderVertexCursor = 0;
// Set while the player is more than 200 above the ground (WorldMap_MainUpdate); World_UpdatePlayerControl
// lets the vehicle descend only while it is set.
// GLOBAL: FF7 0x00de6a14
int g_WorldPlayerAirborneFlag = 0;
// The player's smoothed walk speed (World_UpdatePlayerControl: (old * 3 + acceleration) >> 2, a word).
// GLOBAL: FF7 0x00de6a18
short g_WorldSpecialDeltaMovement = 0;
// GLOBAL: FF7 0x00de6a1c
int g_WorldRenderBufferIndex = 0;
// The world camera's translation matrix, a 30-byte PSX matrix: SetMatrixTranslation writes its
// translation, SetTransMatrix reads it, World_BuildModelPsxMatrix reads t[0..2]. VC5 pads it to
// g_WorldCameraBlendWeight.
// GLOBAL: FF7 0x00de6a20
PsxMatrix g_WorldCameraTransMatrix = {0};
// GLOBAL: FF7 0x00de6a40
int g_WorldCameraBlendWeight = 0;
unsigned char g_bssPad_00de6a44[4] = {0};
// GLOBAL: FF7 0x00de6a48
char g_WorldDebugBuffer[0x100] = {0};
// GLOBAL: FF7 0x00de6b48
int g_WorldCameraFrontWrapped = 0;
// GLOBAL: FF7 0x00de6b4c
int g_WorldCameraVar2 = 0;
unsigned char g_bssPad_00de6b50[4] = {0};
// GLOBAL: FF7 0x00de6b54
int g_WorldChocoboTerrainSet = 0;
// GLOBAL: FF7 0x00de6b58
int g_WorldCameraOcclusionFlag = 0;
// GLOBAL: FF7 0x00de6b5c
int g_WorldIsControlEnabled = 0;
// GLOBAL: FF7 0x00de6b60
int g_WorldCameraHeightStep = 0;
// GLOBAL: FF7 0x00de6b64
int g_WorldCameraOcclusionRunLength = 0;
unsigned char g_bssPad_00de6b68[4] = {0};
// GLOBAL: FF7 0x00de6b6c
int g_WorldCameraWaterSmoothed = 0;
// GLOBAL: FF7 0x00de6b70
int g_WorldCameraRotationZ = 0;
unsigned char g_bssPad_00de6b74[4] = {0};
// The encounter table enc_w.bin, loaded whole (0x8A0 bytes) by WorldMap_LoadStageResources; World_RollEncounter
// reads its level rows (word pairs) and the region/terrain records from +0xA0.
// GLOBAL: FF7 0x00de6b78
unsigned short g_WorldEncounterTable[0x450] = {0};
// GLOBAL: FF7 0x00de7418
short g_WorldCurrentCameraRotationX = 0;
// GLOBAL: FF7 0x00de741c
int g_WorldCameraBaseDist = 0;
// The terrain-animation file wm.ta, loaded whole (0xE000 bytes) by WorldMap_LoadStageResources: the group count at
// +0, the animation groups from +4 (World_AnimateTerrainTextures), the frames they point at further on.
// GLOBAL: FF7 0x00de7420
int g_WorldTerrainAnimData[0x3800] = {0};
// GLOBAL: FF7 0x00df5420
int g_WorldHighwindHeightLowerbound = 0;
// GLOBAL: FF7 0x00df5424
int g_WorldCameraYawStep = 0;
// GLOBAL: FF7 0x00df5428
int g_WorldStallFadePrevTick = 0;
// GLOBAL: FF7 0x00df542c
int g_WorldCameraVar1 = 0;
// The player's requested facing on the world map (-1: none), kept by World_UpdatePlayerControl.
// GLOBAL: FF7 0x00df5430
short g_WorldPlayerFacing = 0;
// GLOBAL: FF7 0x00df5434
int g_WorldPreviousPlayerDirection = 0;
// GLOBAL: FF7 0x00df5438
int g_WorldCameraHeightOffset = 0;
// GLOBAL: FF7 0x00df543c
int g_pWorldRenderBufferHeader = 0;
// GLOBAL: FF7 0x00df5440
int g_WorldCameraPrevOcclusionFlag = 0;
// GLOBAL: FF7 0x00df5444
int g_WorldRenderVertexArenaBase = 0;
// GLOBAL: FF7 0x00df5448
unsigned char g_WorldEventScriptImage[28672] = {0};
// The world camera's rotation matrix (a 0x20-byte PSX MATRIX), set by SetRotMatrix and
// ConcatMatrixPreservingTranslation.
// GLOBAL: FF7 0x00dfc448
unsigned char g_WorldCameraRotMatrix[32] = {0};
// GLOBAL: FF7 0x00dfc468
int g_WorldCameraDistanceSmoothed = 0;
// GLOBAL: FF7 0x00dfc46c
int g_WorldBattleCountdown = 0;
// The previous frame's world input word (edge detection of World_UpdatePlayerControl).
// GLOBAL: FF7 0x00dfc470
int g_WorldPrevKeyInputStatus = 0;
// GLOBAL: FF7 0x00dfc474
int g_WorldCameraRotationY = 0;
// GLOBAL: FF7 0x00dfc478
int g_WorldCameraPositionZ = 0;
// GLOBAL: FF7 0x00dfc47c
int g_WorldCameraDistanceTarget = 0;
// GLOBAL: FF7 0x00dfc480
int g_WorldMovementMultiplier = 0;
// GLOBAL: FF7 0x00dfc484
int g_WorldCameraFront = 0;
// GLOBAL: FF7 0x00dfc488
int g_WorldMainUpdateMode = 0;
// GLOBAL: FF7 0x00dfc48c
int g_WorldCameraZoomState = 0;
// GLOBAL: FF7 0x00dfc490
int g_WorldExitAltitudeSpeed = 0;
// GLOBAL: FF7 0x00dfc494
int g_WorldStreamingStallFadeState = 0;
// GLOBAL: FF7 0x00dfc498
int g_WorldCameraDistanceStep = 0;
// GLOBAL: FF7 0x00dfc49c
int g_WorldCameraBlendVelocity = 0;
// GLOBAL: FF7 0x00dfc4a0
int g_WorldProjectionJitterEnabled = 0;
// GLOBAL: FF7 0x00dfc4a4
int g_WorldCameraHeightAdjustDelay = 0;
// GLOBAL: FF7 0x00dfc4a8
int g_WorldPlayerControlMode = 0;
// GLOBAL: FF7 0x00dfc4ac
int g_WorldFieldTransitionActive = 0;
unsigned char g_bssPad_00dfc4b0[4] = {0};
// GLOBAL: FF7 0x00dfc4b4
int g_WorldCameraViewtype = 0;
// GLOBAL: FF7 0x00dfc4b8
int g_WorldControlLockCount = 0;
// GLOBAL: FF7 0x00dfc4bc
int g_WorldCameraHeightAdjustActive = 0;
// GLOBAL: FF7 0x00dfc4c0
char g_WorldAssetNameBuffer[0x30] = {0};
// GLOBAL: FF7 0x00dfc4f0
int g_WorldRenderBuffer0Header = 0;
unsigned char g_bssPad_00dfc4f4[4] = {0};
unsigned char g_bssPad_00dfc4f8[104] = {0};
// GLOBAL: FF7 0x00dfc560
int g_WorldRenderBuffer0TerrainTable = 0;
unsigned char g_bssPad_00dfc564[4] = {0};
unsigned char g_bssPad_00dfc568[16376] = {0};
// GLOBAL: FF7 0x00e00560
unsigned char g_WorldRenderBuffers[16504] = {0};
// GLOBAL: FF7 0x00e045d8
int g_WorldCurveStartDepth = 0;
// GLOBAL: FF7 0x00e045dc
int g_WorldCameraMode = 0;
// The smoothed camera tilt offset (World_SetCameraAngleOffsets), bare 0xE045E0 before.
// GLOBAL: FF7 0x00e045e0
int g_WorldCameraPitchOffset = 0;
// GLOBAL: FF7 0x00e045e4
int g_WorldMode = 0;
// GLOBAL: FF7 0x00e045e8
int g_WorldMapType = 0;
// GLOBAL: FF7 0x00e045ec
int g_WorldMapInMenu = 0;
// GLOBAL: FF7 0x00e045f0
int g_WorldFieldTransitionRequest = 0;
unsigned char g_bssPad_00e045f4[4] = {0};
// clang-format on


/*
 * Empty. World script errors are reported nowhere in the shipped build; the error code passed by
 * the callers is discarded.
 */
// FUNCTION: FF7 0x0074c9a0
void __cdecl WorldScript_ReportError(int errorCode)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x0074c9a0,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Returns g_WorldRenderBufferIndex.
 */
// FUNCTION: FF7 0x0074c9a5
int __cdecl World_GetRenderBufferIndex(void)
{
    return g_WorldRenderBufferIndex;
}

/*
 * Computes the unsigned end address for count 0x28-byte render vertices. If it lies strictly below
 * arena base plus 0x20800, returns the current bump pointer and advances it by that size;
 * otherwise calls the empty error hook with code 1 and returns an uninitialized local.
 */
// FUNCTION: FF7 0x0074c9af
int __cdecl World_AllocRenderVertexBlock(int count)
{
    int block;

    if ((unsigned int)(g_WorldRenderVertexCursor + count * 0x28) < (unsigned int)(g_WorldRenderVertexArenaBase + 0x20800)) {
        block = g_WorldRenderVertexCursor;
        g_WorldRenderVertexCursor += count * 0x28;
    } else {
        WorldScript_ReportError(1);
    }
    return block;
}

/*
 * Builds g_WorldDebugBuffer from g_InitPath_DataPath, wm/WM, the single digit or letter encoded
 * from digit and g_WorldAssetNames[assetIndex], then returns the shared buffer.
 */
// FUNCTION: FF7 0x0074ca00
char *__cdecl World_BuildLoosePath(int assetIndex, int digit)
{
    char suffixBuf[2];

    strcpy(g_WorldDebugBuffer, g_InitPath_DataPath);
    World_AppendLoosePrefix();
    suffixBuf[0] = (digit < 10) ? digit + '0' : digit + 'A' - 10;
    suffixBuf[1] = 0;
    strcat(g_WorldDebugBuffer, suffixBuf);
    strcat(g_WorldDebugBuffer, g_WorldAssetNames[assetIndex]);

    return g_WorldDebugBuffer;
}

/*
 * Builds g_WorldAssetNameBuffer from the string at 0x00969B00, one digit or letter encoded from
 * digit and g_WorldAssetSuffixes[suffixIndex], then returns the shared buffer.
 */
// FUNCTION: FF7 0x0074caec
char *__cdecl WorldMap_BuildAssetName(int suffixIndex, int digit)
{
    char digitBuf[2];

    // STRING: FF7 0x00969b00
    strcpy(g_WorldAssetNameBuffer, "WM" + 0);
    digitBuf[0] = (digit < 10) ? digit + '0' : digit + 'A' - 10;
    digitBuf[1] = 0;
    strcat(g_WorldAssetNameBuffer, digitBuf);
    strcat(g_WorldAssetNameBuffer, g_WorldAssetSuffixes[suffixIndex]);

    return g_WorldAssetNameBuffer;
}

/*
 * Sets g_WorldProjectionJitterEnabled.
 */
// FUNCTION: FF7 0x0074cbb5
void __cdecl World_SetProjectionJitterEnabled(int value)
{
    g_WorldProjectionJitterEnabled = value;
}

/*
 * Returns the address of entry 0x9C4 (clamped to 0xFFF) in the dword table at g_pWorldTerrainTable when
 * g_WorldFieldTransitionActive is set, else 0. The clamp on a constant is the shipped code's own.
 */
// FUNCTION: FF7 0x0074cbc2
int __cdecl World_GetRenderTableEntryIfActive(void)
{
    int clamped;
    int entry;

    clamped = 0x9C4;
    if (clamped > 0xFFF) {
        clamped = 0xFFF;
    }
    if (g_WorldFieldTransitionActive != 0) {
        entry = g_pWorldTerrainTable + clamped * 4;
    } else {
        entry = 0;
    }
    return entry;
}

/*
 * Copies the query position to shared scratch and reads the initial height unconditionally.
 * Searches six matching cached triangles, accepting the first model-compatible hit; an
 * incompatible cached hit sets a negative result and suppresses the full mesh scan. Without a
 * cached hit scans intersecting triangles for minimum height difference from the previous height,
 * except models 3/5 or variant 2 use minimum signed height value. Promotes the chosen triangle in
 * the cache before testing model terrain compatibility. Writes optional outputs and returns
 * positive success. A no-hit path leaves terrain/height locals uninitialized; height cannot safely
 * be null despite the guarded final write.
 */
// FUNCTION: FF7 0x0074cc07
int __cdecl World_SampleGroundAtEntity(WorldTerrainChunkInfo *chunk, SVECTOR *position, WorldTriangleCache *cache, short *height, WorldTerrainTriangle **triangle, short *terrain, unsigned char model)
{
    struct {
        int terrain, height;
        WorldTerrainTriangle *best;
        int distance;
        WorldTerrainTriangle *current;
        int flags;
        SVECTOR *position;
        WorldTriangleCache *cache;
        int difference, found, previous, currentHeight;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.position = (SVECTOR *)g_WorldCameraViewVector;
    *s.position = *position;
    s.previous = *height;
    s.distance = 0x7fffffff;
    s.found = 0;
    s.best = 0;
    if (cache) {
        for (s.cache = &cache[0]; s.cache < &cache[6] && s.found <= 0; s.cache++) {
            if (s.cache->triangle)
                if (s.cache->x == chunk->wChunkX && s.cache->y == chunk->wChunkY)
                    if (World_TriangleHeightAt(s.cache->triangle, (short *)s.position, &s.currentHeight)) {
                        s.terrain = s.cache->triangle->terrain | (s.cache->triangle->region << 8);
                        s.height = s.currentHeight;
                        s.found = -1;
                        if (World_CanModelEnterTerrain(s.terrain, model)) {
                            s.best = s.cache->triangle;
                            s.found = 1;
                        }
                    }
        }
    }
    if (s.found == 0) {
        for (s.current = &chunk->triangles[0]; s.current < &chunk->triangles[chunk->count]; s.current++) {
            if (World_TriangleHeightAt(s.current, (short *)s.position, &s.currentHeight)) {
                s.flags = s.current->terrain | (s.current->region << 8);
                s.difference = model != 3 && model != 5 && g_WorldMapType != 2 ? (s.currentHeight - s.previous > 0 ? s.currentHeight - s.previous : -(s.currentHeight - s.previous)) : s.currentHeight;
                if (s.distance > s.difference) {
                    s.found++;
                    s.distance = s.difference;
                    s.best = s.current;
                    s.terrain = s.flags;
                    s.height = s.currentHeight;
                }
            }
        }
        if (s.found != 0) {
            if (cache && s.best) {
                for (s.cache = &cache[4]; s.cache >= &cache[0]; s.cache--)
                    s.cache[1] = s.cache[0];
                cache->triangle = s.best;
                cache->x = chunk->wChunkX;
                cache->y = chunk->wChunkY;
            }
            if (!World_CanModelEnterTerrain(s.terrain, model)) {
                s.best = 0;
                s.found = 0;
            }
        }
    }
    if (terrain)
        *terrain = s.terrain;
    if (height)
        *height = s.height;
    if (triangle)
        *triangle = s.best;
    return s.found > 0;
}

/*
 * Returns terrain-entry permission selected by model ID, low five terrain bits, bridge/current-
 * player state, vehicle transition state and altitude flags. Preserves each original bitmask and
 * terrain high-bit restriction: the leader on foot, the chocobos, the Highwind, the Tiny Bronco, the
 * buggy, the submarine, the ship and model 100 have distinct branches; all unlisted models return 1.
 */
// FUNCTION: FF7 0x0074ceca
int __cdecl World_CanModelEnterTerrain(int terrain, unsigned char model)
{
    struct {
        int allowed, bridge;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.allowed = (terrain & WORLD_WALKMESH_REGION_MASK) != WORLD_WALKMESH_REGION_MASK;
    s.bridge = (WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_COREL_BRIDGE || WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_WUTAI_BRIDGE) && WorldEntity_GetPlayerModelId() == model;
    switch (model) {
    case WORLD_MODEL_LEADER_0:
    case WORLD_MODEL_LEADER_1:
    case WORLD_MODEL_LEADER_2:
        return ((s.bridge ? WORLD_TERRAIN_MASK_FOOT_BRIDGES : WORLD_TERRAIN_MASK_FOOT_OVERWORLD) >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
    case WORLD_MODEL_CHOCOBO_4:
        if (World_GetVehicleTransitionState() == 2)
            return (((s.bridge ? WORLD_TERRAIN_MASK_FOOT_BRIDGES : WORLD_TERRAIN_MASK_CHOCOBO_DEFAULT) >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1) && s.allowed;
        return ((s.bridge ? WORLD_TERRAIN_MASK_FOOT_BRIDGES : WORLD_TERRAIN_MASK_CHOCOBO_DEFAULT) >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
    case WORLD_MODEL_CHOCOBO:
        if (World_GetVehicleTransitionState() == 2)
            return (((s.bridge ? WORLD_TERRAIN_MASK_FOOT_BRIDGES : WORLD_TERRAIN_MASK_CHOCOBO_DEFAULT) >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1) && s.allowed;
        return ((s.bridge ? WORLD_TERRAIN_MASK_FOOT_BRIDGES : ((int *)&g_WorldChocoboTerrainMasks)[World_GetChocoboTerrainSet()]) >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
    case WORLD_MODEL_HIGHWIND:
        if (World_GetVehicleTransitionState() == 2)
            return ((WORLD_TERRAIN_MASK_VEHICLE_LAND >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1) && s.allowed;
        return g_WorldCameraBlendDirection >= 0 ? 1 : ((terrain & WORLD_WALKMESH_TERRAIN_MASK) ? 0 : 1);
    case WORLD_MODEL_TINY_BRONCO:
        if (WorldEntity_IsPlayerAltitudeScripted()) {
            if (World_GetVehicleTransitionState() == 2)
                return (WORLD_TERRAIN_MASK_SHALLOW_WATER >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
            return g_WorldCameraBlendDirection >= 0 ? 1 : ((WORLD_TERRAIN_MASK_SHALLOW_WATER >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1);
        }
        if (World_GetVehicleTransitionState() == 2)
            return (WORLD_TERRAIN_MASK_BRONCO_DOCK >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
        return (WORLD_TERRAIN_MASK_SHALLOW_WATER >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
    case WORLD_MODEL_BUGGY:
        if (World_GetVehicleTransitionState() == 2)
            return ((WORLD_TERRAIN_MASK_VEHICLE_LAND >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1) && s.allowed;
        return (WORLD_TERRAIN_MASK_BUGGY_DRIVE >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
    case WORLD_MODEL_SUBMARINE:
        if (World_GetVehicleTransitionState() == 2)
            return ((WORLD_TERRAIN_MASK_VEHICLE_LAND >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1) && s.allowed;
        return (WORLD_TERRAIN_MASK_SUBMARINE_WATER >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
    case WORLD_MODEL_SHIP:
        return (WORLD_TERRAIN_MASK_SHIP_WATER >> (terrain & WORLD_WALKMESH_TERRAIN_MASK)) & 1;
    case WORLD_MODEL_ZOLOM:
        return (terrain & WORLD_WALKMESH_TERRAIN_MASK) == WORLD_TERRAIN_SWAMP;
    }
    return 1;
}

/*
 * Returns the world camera mode g_WorldCameraViewtype (0..3).
 */
// FUNCTION: FF7 0x0074d28e
int __cdecl World_GetCameraMode(void)
{
    return g_WorldCameraViewtype;
}

/*
 * Returns the camera's current yaw g_WorldCameraRotationY masked to 12 bits.
 */
// FUNCTION: FF7 0x0074d298
int __cdecl World_GetCameraRotationY(void)
{
    return g_WorldCameraRotationY & 0xFFF;
}

/*
 * Sets the camera's target yaw g_WorldCameraFront (12-bit).
 */
// FUNCTION: FF7 0x0074d2a7
void __cdecl World_SetCameraFront(int value)
{
    g_WorldCameraFront = value & 0xFFF;
}

/*
 * Sets both the camera's target yaw g_WorldCameraFront and its current yaw g_WorldCameraRotationY, a snap with no
 * interpolation.
 */
// FUNCTION: FF7 0x0074d2b9
void __cdecl World_SetCameraFrontAndRotationY(int value)
{
    g_WorldCameraFront = value & 0xFFF;
    g_WorldCameraRotationY = g_WorldCameraFront;
}

/*
 * Sets the camera zoom state g_WorldCameraZoomState; entering state 1 also loads the per-mode distance and
 * height from the tables at g_WorldCameraHeights and g_WorldTerrainTable indexed by the camera mode.
 */
// FUNCTION: FF7 0x0074d2d7
void __cdecl World_SetCameraZoomState(int zoomState)
{
    g_WorldCameraZoomState = zoomState;
    if (zoomState == 1) {
        g_WorldCameraDistanceTarget = g_WorldCameraHeights[g_WorldCameraViewtype];
        g_WorldCameraHeight = ((int *)&g_WorldTerrainTable)[g_WorldCameraViewtype];
    }
}

/*
 * Returns the camera zoom state g_WorldCameraZoomState.
 */
// FUNCTION: FF7 0x0074d30f
int __cdecl World_GetCameraZoomState(void)
{
    return g_WorldCameraZoomState;
}

/*
 * Returns the camera roll g_WorldCameraRotationZ.
 */
// FUNCTION: FF7 0x0074d319
int __cdecl World_GetCameraRoll(void)
{
    return g_WorldCameraRotationZ;
}

/*
 * Sets the camera roll g_WorldCameraRotationZ.
 */
// FUNCTION: FF7 0x0074d323
void __cdecl World_SetCameraRotationZ(int value)
{
    g_WorldCameraRotationZ = value;
}

/*
 * Returns the map variant g_WorldMapType (which world map is loaded: 0 the overworld, 3 the
 * snowfield, etc.).
 */
// FUNCTION: FF7 0x0074d330
int __cdecl World_GetMapVariant(void)
{
    return g_WorldMapType;
}

/*
 * Copies the supplied view vector and replaces its Y word with -g_WorldCameraDeltaY, selects
 * rotation/translation matrices g_WorldCameraRotMatrix and g_WorldCameraTransMatrix,
 * transforms that vector, constructs a local translation matrix and selects it.
 */
// FUNCTION: FF7 0x0074d33a
void __cdecl World_SetViewTransformForPosition(SVECTOR *view)
{
    SVECTOR viewCopy;
    int unused;
    char matrix[0x20];
    char translation[0x10];

    viewCopy = *view;
    viewCopy.vy = (short)-g_WorldCameraDeltaY;
    SetRotMatrix((void *)g_WorldCameraRotMatrix);
    SetTransMatrix(&g_WorldCameraTransMatrix);
    RotTransPoint(&viewCopy, (int *)translation, &unused);
    SetMatrixTranslation(matrix, translation);
    SetTransMatrix(matrix);
}

/*
 * Installs the world camera's rotation matrix (g_WorldCameraRotMatrix) and translation
 * (g_WorldCameraTransMatrix) as the GTE's current matrices.
 */
// FUNCTION: FF7 0x0074d3a7
void __cdecl World_LoadCameraMatrixToGte(void)
{
    SetRotMatrix((void *)g_WorldCameraRotMatrix);
    SetTransMatrix(&g_WorldCameraTransMatrix);
}

/*
 * Returns g_WorldCurrentCameraRotationX as a short.
 */
// FUNCTION: FF7 0x0074d3c6
short __cdecl World_GetCurrentCameraRotationX(void)
{
    return g_WorldCurrentCameraRotationX;
}

/*
 * Switches the camera mode: remembers the previous one in g_WorldCameraMode, and for modes 0 and 1
 * resets the target yaw unless the map variant is 3. Also sets the two draw-distance words
 * g_WorldCameraVar1 (0x78, or 0xA0 in any mode but 0) and g_WorldCameraBaseDist (0x78 in every mode but 3).
 */
// FUNCTION: FF7 0x0074d3d1
void __cdecl World_SetCameraMode(int mode)
{
    g_WorldCameraMode = g_WorldCameraViewtype;
    g_WorldCameraViewtype = mode;
    if (g_WorldCameraViewtype == 0 || g_WorldCameraViewtype == 1) {
        if (g_WorldMapType != 3) {
            g_WorldCameraFront = 0;
        }
    }
    g_WorldCameraVar1 = (g_WorldCameraViewtype != 0 ? 0x28 : 0) + 0x78;
    g_WorldCameraBaseDist = (g_WorldCameraViewtype != 3) * 0x78;
}

/*
 * Enables or disables player control on the world map. Enabling (enable != 0) only takes when mode
 * matches the pending control mode g_WorldPlayerControlMode or that mode is 6; it sets g_WorldIsControlEnabled and re-
 * focuses the terrain streaming on the player position, snapping (mode 3 from a mode other than 6)
 * or not. Disabling clears g_WorldIsControlEnabled and records mode as the pending mode.
 */
// FUNCTION: FF7 0x0074d438
void __cdecl WorldMap_SetPlayerControl(int enable, int mode)
{
    int position[4];
    int snap;

    if (enable != 0) {
        if (mode == g_WorldPlayerControlMode || g_WorldPlayerControlMode == 6) {
            g_WorldIsControlEnabled = 1;
            WorldEntity_GetPlayerPosition((VECTOR *)position);
            if (mode == 3 && g_WorldPlayerControlMode != 6) {
                snap = 0;
            } else {
                snap = 1;
            }
            World_UpdateStreamFocus((WorldStreamFocusData *)position, snap);
        }
    } else {
        g_WorldIsControlEnabled = 0;
        g_WorldPlayerControlMode = mode;
    }
}

/*
 * Returns whether player control is enabled (g_WorldIsControlEnabled).
 */
// FUNCTION: FF7 0x0074d4b6
int __cdecl World_IsPlayerControlEnabled(void)
{
    return g_WorldIsControlEnabled;
}

/*
 * Sets the model heading g_WorldPreviousPlayerDirection to heading relative to the camera's target yaw, minus half a
 * turn.
 */
// FUNCTION: FF7 0x0074d4c0
void __cdecl World_SetPreviousPlayerDirectionFromFacing(short heading)
{
    g_WorldPreviousPlayerDirection = heading + g_WorldCameraFront + -0x800;
}

/*
 * Returns whether the control lock counter g_WorldControlLockCount is zero.
 */
// FUNCTION: FF7 0x0074d4dc
int __cdecl World_IsControlLockClear(void)
{
    return g_WorldControlLockCount == 0;
}

/*
 * Returns the fade target g_WorldCurveStartDepth.
 */
// FUNCTION: FF7 0x0074d4ed
int __cdecl World_GetFadeTarget(void)
{
    return g_WorldCurveStartDepth;
}

/*
 * Sets g_WorldCameraBlendDirection.
 */
// FUNCTION: FF7 0x0074d4f7
void __cdecl World_SetCameraBlendDirection(int value)
{
    g_WorldCameraBlendDirection = value;
}

/*
 * Returns g_WorldCameraBlendDirection.
 */
// FUNCTION: FF7 0x0074d504
int __cdecl World_GetCameraBlendDirection(void)
{
    return g_WorldCameraBlendDirection;
}

/*
 * Builds a fixed identity matrix rotated in X/Z/Y order, concatenates it with the world rotation
 * matrix into matrix and sets its translation from the world offset, temporarily halved while
 * g_WorldBattleCountdown counts down with halveFlag. Transforms the model position through the selected
 * matrices, zeroes that model position and stores the transformed translation in matrix; calls
 * World_SwitchMusicTrack when the countdown reaches zero.
 */
// FUNCTION: FF7 0x0074d50e
void __cdecl World_BuildModelPsxMatrix(WorldMapModel *model, SVECTOR *rotation, void *matrix, int halveFlag)
{
    struct {
        PsxMatrix32 rotationMatrix;
        int dwFlags_unused;
        VECTOR cameraTranslation;
        SVECTOR modelPosition;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.rotationMatrix.m[0][0] =
        frame.rotationMatrix.m[1][1] =
            frame.rotationMatrix.m[2][2] = 0x1000;
    frame.rotationMatrix.m[0][1] =
        frame.rotationMatrix.m[0][2] =
            frame.rotationMatrix.m[1][0] =
                frame.rotationMatrix.m[1][2] =
                    frame.rotationMatrix.m[2][0] =
                        frame.rotationMatrix.m[2][1] = 0;
    RotateMatrixX(rotation->vx, &frame.rotationMatrix);
    RotateMatrixZ(rotation->vz, &frame.rotationMatrix);
    RotateMatrixY(rotation->vy, &frame.rotationMatrix);
    ConcatMatrixPreservingTranslation(g_WorldCameraRotMatrix, &frame.rotationMatrix, matrix);
    SetRotMatrix((void *)g_WorldCameraRotMatrix);
    if (g_WorldBattleCountdown > 0 && halveFlag) {
        frame.cameraTranslation.vx = g_WorldCameraTransMatrix.t[0] / 2;
        frame.cameraTranslation.vy = g_WorldCameraTransMatrix.t[1] / 2;
        frame.cameraTranslation.vz = g_WorldCameraTransMatrix.t[2] / 2;
        g_WorldBattleCountdown--;
        if (g_WorldBattleCountdown == 0) {
            World_SwitchMusicTrack(g_WorldPrevMusicTrack);
        }
    } else {
        frame.cameraTranslation.vx = g_WorldCameraTransMatrix.t[0];
        frame.cameraTranslation.vy = g_WorldCameraTransMatrix.t[1];
        frame.cameraTranslation.vz = g_WorldCameraTransMatrix.t[2];
    }
    SetMatrixTranslation(matrix, &frame.cameraTranslation);
    SetTransMatrix(matrix);
    frame.modelPosition.vx = (short)model->sPos.x;
    frame.modelPosition.vy = (short)model->sPos.y;
    frame.modelPosition.vz = (short)model->sPos.z;
    RotTransPoint(&frame.modelPosition, (int *)&frame.cameraTranslation, (int *)&frame.dwFlags_unused);
    model->sPos.x =
        model->sPos.y =
            model->sPos.z = 0;
    SetMatrixTranslation(matrix, &frame.cameraTranslation);
}

/*
 * Starts the submarine's surfacing (WORLD_MAP_STATE_SURFACING, 5): fade step g_WorldExitAltitudeSpeed = 0x14, no field
 * entry, player control off, and the fade request World_BeginFadeToBlack(0x10, 1).
 */
// FUNCTION: FF7 0x0074d6bb
void __cdecl World_BeginSubmarineSurfacing(void)
{
    g_WorldMode = WORLD_MAP_STATE_SURFACING;
    g_WorldExitAltitudeSpeed = 0x14;
    World_SetFieldEntryFromTable(0);
    WorldMap_SetPlayerControl(0, 0);
    World_BeginFadeToBlack(0x10, 1);
}

/*
 * Starts the submarine's dive (WORLD_MAP_STATE_DIVING, 4): like World_BeginSubmarineSurfacing with fade step -0x14.
 */
// FUNCTION: FF7 0x0074d6f6
void __cdecl World_BeginSubmarineDive(void)
{
    g_WorldMode = WORLD_MAP_STATE_DIVING;
    g_WorldExitAltitudeSpeed = 0xFFFFFFEC;
    World_SetFieldEntryFromTable(0);
    WorldMap_SetPlayerControl(0, 0);
    World_BeginFadeToBlack(0x10, 1);
}

/*
 * A world script's field exit: selects field entry `value` and sets WORLD_MAP_STATE_SCRIPT_EXIT (8).
 */
// FUNCTION: FF7 0x0074d731
void __cdecl World_BeginScriptFieldExit(int value)
{
    World_SetFieldEntryFromTable(value);
    g_WorldMode = WORLD_MAP_STATE_SCRIPT_EXIT;
}

/*
 * Requests a transition to a field: if no exit is in progress, clears the camera offsets, sets
 * g_WorldFieldTransitionActive and enters exit state 1; in state 1 it moves to state 2, disables player control,
 * selects the field entry, requests the fade, sets g_WorldFieldTransitionRequest and clears g_WorldFrameTimingFlag.
 */
// FUNCTION: FF7 0x0074d74c
void __cdecl WorldMap_RequestFieldTransition(int fieldId)
{
    if (g_WorldMode < WORLD_MAP_STATE_PLAYING) {
        g_WorldCameraHeight = 0;
        g_WorldCameraDistanceTarget = g_WorldCameraHeight;
        g_WorldFieldTransitionActive = 1;
        g_WorldMode = WORLD_MAP_STATE_PLAYING;
    }
    if (g_WorldMode == WORLD_MAP_STATE_PLAYING) {
        g_WorldMode = WORLD_MAP_STATE_FADE_OUT;
        WorldMap_SetPlayerControl(0, 0);
        World_SetFieldEntryFromTable(fieldId);
        World_BeginFadeToBlack(0x10, 1);
        g_WorldFieldTransitionRequest = 1;
        World_SetDoubleFrameEnabled(0);
    }
}

/*
 * Requests a battle with formation formation: enters the exit sequence like
 * WorldMap_RequestFieldTransition, and from state 1 or 2, when no battle lock (g_FieldMoviePending) is
 * set, moves to state 3, disables player control, queues the field entry through World_QueueFieldTransitionRequest,
 * stops a running sound if World_IsFlashSoundActive says one is playing, and enables terrain rendering. With
 * the lock set it instead delays 0x1E frames (g_WorldBattleCountdown) and re-arms the encounter counter.
 */
// FUNCTION: FF7 0x0074d7cd
void __cdecl WorldMap_RequestBattle(int formation)
{
    if (g_WorldMode < WORLD_MAP_STATE_PLAYING) {
        g_WorldCameraHeight = 0;
        g_WorldCameraDistanceTarget = g_WorldCameraHeight;
        g_WorldFieldTransitionActive = 1;
        g_WorldMode = WORLD_MAP_STATE_PLAYING;
    }
    if (g_WorldMode == WORLD_MAP_STATE_PLAYING || g_WorldMode == WORLD_MAP_STATE_FADE_OUT) {
        if (g_FieldMoviePending == 0) {
            g_WorldMode = WORLD_MAP_STATE_LEAVING;
            WorldMap_SetPlayerControl(0, 0);
            World_QueueFieldTransitionRequest(formation);
            if (World_IsFlashSoundActive() != 0) {
                World_StopFlashSoundAndClearScreenFlash();
            }
            World_SetTerrainRenderEnabled(1);
            World_SetDoubleFrameEnabled(0);
        } else {
            g_WorldBattleCountdown = 0x1E;
            World_ReloadEncounterCounterNormal();
        }
    }
}

/*
 * Requests a plain return from the world map: enters the exit sequence and from state 1 moves to
 * state 3, disables player control and marks the return through World_RequestPlainReturnTransition.
 */
// FUNCTION: FF7 0x0074d875
void __cdecl WorldMap_RequestPlainReturn(void)
{
    if (g_WorldMode < WORLD_MAP_STATE_PLAYING) {
        g_WorldCameraHeight = 0;
        g_WorldCameraDistanceTarget = g_WorldCameraHeight;
        g_WorldFieldTransitionActive = 1;
        g_WorldMode = WORLD_MAP_STATE_PLAYING;
    }
    if (g_WorldMode == WORLD_MAP_STATE_PLAYING) {
        g_WorldMode = WORLD_MAP_STATE_LEAVING;
        WorldMap_SetPlayerControl(0, 0);
        World_RequestPlainReturnTransition();
    }
}

/*
 * Returns g_WorldCameraViewtype's low two bits combined with bits 2..3 from World_GetMiniMapState's result shifted
 * left two.
 */
// FUNCTION: FF7 0x0074d8cf
int __cdecl World_GetCameraModeAndMiniMapState(void)
{
    return (g_WorldCameraViewtype & 3) | ((World_GetMiniMapState() << 2) & 0xC);
}

/*
 * Unpacks a camera request byte: the low two bits select the camera mode, bits 2-3 go to
 * World_SetMiniMapState.
 */
// FUNCTION: FF7 0x0074d8ef
void __cdecl World_SetCameraModeAndMiniMapState(int request)
{
    World_SetCameraMode(request & 3);
    World_SetMiniMapState(request >> 2 & 3);
}

/*
 * Returns the message pointer computed from g_WorldMsgBuffer's word-offset table when index is
 * nonnegative and below its first-word count, otherwise returns 0.
 */
// FUNCTION: FF7 0x0074d915
unsigned char *__cdecl World_GetMessage(short index)
{
    if (index >= 0 && index < g_WorldMsgBuffer[0]) {
        return (unsigned char *)g_WorldMsgBuffer + g_WorldMsgBuffer[index + 1];
    }
    return 0;
}

/*
 * Clears g_WorldStreamingStallFadeState.
 */
// FUNCTION: FF7 0x0074d946
void __cdecl World_ClearStreamingStallFadeState(void)
{
    g_WorldStreamingStallFadeState = WORLD_STREAM_STALL_NONE;
}

/*
 * For WORLD_STREAM_STALL_FADE_OUT disables player control and calls World_BeginFadeToBlack(4,1); for
 * WORLD_STREAM_STALL_RESUME enables player control, calls World_BeginFadeFromBlack(0x10,1) and stores NONE. Stores the resulting value in
 * g_WorldStreamingStallFadeState.
 */
// FUNCTION: FF7 0x0074d955
void __cdecl World_SetStreamingStallFadeState(int state)
{
    switch (state) {
    case WORLD_STREAM_STALL_FADE_OUT:
        WorldMap_SetPlayerControl(0, 4);
        World_BeginFadeToBlack(4, 1);
        break;
    case WORLD_STREAM_STALL_RESUME:
        WorldMap_SetPlayerControl(1, 4);
        World_BeginFadeFromBlack(0x10, 1);
        state = WORLD_STREAM_STALL_NONE;
        break;
    }
    g_WorldStreamingStallFadeState = state;
}

/*
 * Calls World_GetRenderBufferIndex and preserves its unused result. In FADE_OUT advances g_WorldStreamingStallFadeState to BLACK when
 * World_IsScreenFadeIdle succeeds; in BLACK obtains the frame count, advances to STEADY only for a difference
 * of four or five and records the current count in g_WorldStallFadePrevTick.
 */
// FUNCTION: FF7 0x0074d9b3
void __cdecl World_StepStreamingStallFadeState(void)
{
    int dwFrameCount;
    int local_1_unused;

    local_1_unused = World_GetRenderBufferIndex();
    switch (g_WorldStreamingStallFadeState) {
    case WORLD_STREAM_STALL_FADE_OUT:
        if (World_IsScreenFadeIdle())
            g_WorldStreamingStallFadeState = WORLD_STREAM_STALL_BLACK;
        break;
    case WORLD_STREAM_STALL_BLACK:
        dwFrameCount = Savemap_GetClockTickCount();
        if (dwFrameCount - g_WorldStallFadePrevTick > 3 &&
            dwFrameCount - g_WorldStallFadePrevTick < 6)
            g_WorldStreamingStallFadeState = WORLD_STREAM_STALL_STEADY;
        g_WorldStallFadePrevTick = dwFrameCount;
        break;
    }
}

/*
 * Sets g_WorldCameraYawStep.
 */
// FUNCTION: FF7 0x0074da26
void __cdecl World_SetCameraYawStep(int value)
{
    g_WorldCameraYawStep = value;
}

/*
 * Sets g_WorldCameraHeightStep.
 */
// FUNCTION: FF7 0x0074da33
void __cdecl World_SetCameraHeightStep(int value)
{
    g_WorldCameraHeightStep = value;
}

/*
 * Sets g_WorldCameraDistanceStep.
 */
// FUNCTION: FF7 0x0074da40
void __cdecl World_SetCameraDistanceStep(int value)
{
    g_WorldCameraDistanceStep = value;
}

/*
 * For camera mode zero, advances g_WorldCameraFront and clears pitch/distance globals. For mode one
 * advances the angle, clamps g_WorldCameraDistanceTarget to 1000..15000 and g_WorldCameraHeight to 1152..1920 after
 * adding their increments. Other modes do nothing.
 */
// FUNCTION: FF7 0x0074da4d
void __cdecl World_AdvanceScriptedCamera(void)
{
    switch (g_WorldCameraZoomState) {
    case WORLD_CAMERA_ZOOM_IDLE:
        g_WorldCameraFront += g_WorldCameraYawStep;
        g_WorldCameraHeight = 0;
        g_WorldCameraDistanceTarget = g_WorldCameraHeight;
        break;
    case WORLD_CAMERA_ZOOM_STEPPING:
        g_WorldCameraFront += g_WorldCameraYawStep;
        g_WorldCameraDistanceTarget += g_WorldCameraDistanceStep;
        if (g_WorldCameraDistanceTarget > 15000) {
            g_WorldCameraDistanceTarget = 15000;
        } else if (g_WorldCameraDistanceTarget < 1000) {
            g_WorldCameraDistanceTarget = 1000;
        }
        g_WorldCameraHeight += g_WorldCameraHeightStep;
        if (g_WorldCameraHeight > 1920) {
            g_WorldCameraHeight = 1920;
        } else if (g_WorldCameraHeight < 1152) {
            g_WorldCameraHeight = 1152;
        }
        break;
    }
}

/*
 * Sets the camera distance g_WorldCameraDistanceTarget.
 */
// FUNCTION: FF7 0x0074db2b
void __cdecl World_SetCameraDistance(int value)
{
    g_WorldCameraDistanceTarget = value;
}

/*
 * Sets the camera height g_WorldCameraHeight.
 */
// FUNCTION: FF7 0x0074db38
void __cdecl World_SetCameraHeight(int value)
{
    g_WorldCameraHeight = value;
}

/*
 * Sets g_WorldChocoboTerrainSet, the chocobo's terrain set (0..4: yellow, green, blue, black, gold;
 * the order is confirmed in game), which picks the mask of g_WorldChocoboTerrainMasks.
 */
// FUNCTION: FF7 0x0074db45
void __cdecl World_SetChocoboTerrainSet(int value)
{
    g_WorldChocoboTerrainSet = value;
}

/*
 * Returns g_WorldChocoboTerrainSet, the chocobo's terrain set (0..4: yellow, green, blue, black, gold).
 */
// FUNCTION: FF7 0x0074db52
int __cdecl World_GetChocoboTerrainSet(void)
{
    return g_WorldChocoboTerrainSet;
}

/*
 * Empty. A per-frame hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x0074db5c
void __cdecl World_UnusedNoOp_1(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x0074db5c,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * First-frame latch: when neither g_WorldMapInitialized nor g_WorldModeResetDone is set, clears the exit sequence
 * state and sets g_WorldModeResetDone.
 */
// FUNCTION: FF7 0x0074db61
void __cdecl WorldMap_ResetModeOnFirstUpdate(void)
{
    if (g_WorldMapInitialized == 0 && g_WorldModeResetDone == 0) {
        g_WorldMode = WORLD_MAP_STATE_CAMERA_INTRO;
        g_WorldModeResetDone = 1;
    }
}

/*
 * Stores mode and drives world-map initialization, frame updates and departure while the entry
 * state is below nine. On first entry selects stage resources from dispatch/field and variant,
 * initializes terrain, minimap, saved state, camera, entities, encounters and scripts, then sets
 * state to -15 and the initialized flag. For states below six, the pause flag suppresses
 * player/camera/script/terrain and rendering work, but the map state machine still runs. Smooths
 * the reference height unless camera mode is two, enforces altitude/ground restrictions for the
 * relevant vehicle or stage, renders overlays/minimap and runs encounters only in state one. If
 * state reaches six or higher, requests the departure script and runs it synchronously until no
 * script remains, writes dispatch/field/battle through the transition helper, performs cleanup and
 * clears initialization; state nine also sets g_WorldMapExitRequested. Always returns
 * World_GetRenderBufferIndex(), including when the entry state skips all world work.
 */
// FUNCTION: FF7 0x0074db8c
int __cdecl WorldMap_MainUpdate(int *dispatch, int *field, int *battle, int mode)
{
    struct {
        int ground;
        VECTOR position;
        int variant, distance;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    WorldMap_ResetModeOnFirstUpdate();
    g_WorldMainUpdateMode = 0;
    g_WorldMainUpdateMode = mode;
    if (g_WorldMode < WORLD_MAP_STATE_EXIT) {
        if (!g_WorldMapInitialized) {
            // Coming from a field (*dispatch 0), the field entry picks the map: entries 30, 31, 42 and 56
            // open the underwater map (type 2, WORLD_STAGE_UNDERWATER), 60 and up the snowfield (type 3,
            // WORLD_STAGE_SNOWFIELD) unless the submarine surfaced, 36..39 a WORLD_STAGE_WARSHIP stage
            // (by story progress); which places these are is not read here.
            g_WorldMapType = *dispatch != 0 ? World_GetSavedMapVariant() : (g_WorldMode == WORLD_MAP_STATE_DIVED || *field == 30 || *field == 31 || *field == 42 || *field == 56) ? 2
                                                                               : (g_WorldMode != WORLD_MAP_STATE_SURFACED && *field >= 60)                                                   ? 3
                                                                                                                                                                              : 0;
            s.variant = World_SelectMapVariantFromStoryProgress();
            WorldMap_LoadStageResources((*dispatch == 0 && *field >= 36 && *field <= 39) ? (s.variant > 0 ? 10 : 9) : (g_WorldMapType == 0 ? s.variant : g_WorldMapType + 9));
            World_StopAllSfxAndResetTranspose();
            World_SetStageVariantFlag(s.variant == 3 || s.variant == 4 || s.variant == 6 || s.variant == 8);
            g_WorldCameraViewtype = 0;
            World_InitMiniMap();
            World_InitTerrainStreaming();
            World_RestoreStateFromSavemap((int)dispatch, (int)field, g_WorldMapType != 3);
            if (g_WorldMapType == 2)
                World_SetCameraMode(2);
            g_WorldCameraBaseDist = 0;
            World_UpdateCameraBlendParameters();
            World_InitRenderVertexBuffersAndSky();
            World_ClearInitStepMask();
            World_ResetZonesAndDefaultLighting();
            World_ResetOverlayFadeState();
            World_ResetTerrainFloodAndEffectState();
            World_ResetEncounterStep();
            World_SetEncountersEnabled(1);
            World_ClearStreamingStallFadeState();
            WorldMsg_InitScriptContextAndWindows(g_WorldMsgHeader);
            World_InitZolomBodySegmentRecords();
            World_InitSkySprites();
            World_InitEffectNodePool();
            WorldEntity_ClearPerFrameFlags();
            World_ResetVehicleState();
            World_SetMidiVolume(127);
            WorldScript_Init((unsigned char *)g_WorldEventScriptImage);
            WorldScript_RequestSystem(0);
            WorldScript_RunAllEntities();
            g_WorldHighwindHeightLowerbound = WorldEntity_GetPlayerModelId() != WORLD_MODEL_HIGHWIND ? 2000 : 4000;
            if (*dispatch == 1 || *dispatch == 2 || g_WorldMode == WORLD_MAP_STATE_SURFACED || g_WorldMode == WORLD_MAP_STATE_DIVED)
                World_BoardSavedVehicleModel();
            if (g_WorldMode == WORLD_MAP_STATE_SURFACED || g_WorldMode == WORLD_MAP_STATE_DIVED)
                World_ArmWalkerUnstick(3000);
            if (g_WorldMapType == 2) {
                WorldEntity_SetPlayerAltitudeScripted(1);
                WorldEntity_SetPlayerAltitude(-3000);
            }
            World_PreloadBlocksFromBot();
            World_GetModelRecord(WorldMap_GetLeaderModelIndex());
            if (World_GetMusicTrack() < 6)
                World_SetMusicChangeGate(1);
            if (World_GetMusicTrack() != 0)
                World_ReplayCurrentMusicTrack();
            else
                World_SwitchMusicTrack(1);
            World_UpdatePlayerControl();
            g_WorldMode = WORLD_MAP_STATE_LOADED;
            g_WorldMapInitialized = 1;
        }
        if (g_WorldMode < WORLD_MAP_STATE_DIVED) {
            if (!g_WorldMapInMenu) {
                World_UpdateProjectionOffset();
                World_AnimateTerrainTextures(g_WorldMovementMultiplier - (g_WorldMovementMultiplier == 1));
                g_WorldTerrainHasWater = 0;
                World_BeginEntityFrame();
                WorldEntity_GetPlayerPosition((VECTOR *)&s.position);
                s.distance = s.position.vy - g_WorldCameraDeltaY > 0 ? s.position.vy - g_WorldCameraDeltaY : -(s.position.vy - g_WorldCameraDeltaY);
                if (g_WorldCameraZoomState != 2) {
                    g_WorldCameraDeltaY = (WorldEntity_HasPlayerMoved() == 0 && s.distance <= 200) ? (s.distance > 50 ? g_WorldCameraDeltaY + (s.position.vy > g_WorldCameraDeltaY ? 50 : -50) : s.position.vy) : (g_WorldCameraDeltaY * 7 + s.position.vy) >> 3;
                }
                World_UpdatePlayerControl();
                World_AdvanceScriptedCamera();
                WorldEntity_GetPlayerPosition((VECTOR *)&s.position);
                WorldZone_ApplyToCamera((WorldPoint *)&s.position);
                WorldScript_RunAllEntities();
                World_BuildCameraMatrix(g_WorldCameraRotationY);
                World_UpdateTerrainStreaming(g_WorldCameraRotationY, 1);
                WorldMap_PollMeshScriptTrigger();
                World_UpdateCameraHeightOffsetForOcclusion();
            }
            World_UpdateMapStateMachine();
            if (!g_WorldMapInMenu) {
                if (g_WorldMode < WORLD_MAP_STATE_EXIT)
                    WorldMsg_PollInputAndUpdateWindow();
                World_UpdateTerrainCurrentStrengths();
                if ((g_WorldCameraViewtype == 3 && g_WorldCameraBlendDirection == 0) || (g_WorldMapType == 2 && World_GetTerrainWalkCompletedFlag())) {
                    WorldEntity_GetPlayerPosition((VECTOR *)&s.position);
                    s.ground = WorldEntity_GetPlayerGroundY();
                    g_WorldPlayerAirborneFlag = s.position.vy > s.ground + 200;
                    if (g_WorldPlayerAirborneFlag == 0 && g_WorldCameraBlendDirection == 0) {
                        if (WorldEntity_IsPlayerModelInMask(1 << 13))
                            WorldEntity_SetPlayerAltitude(-3000 < s.ground + 200 ? -3000 : s.ground + 200);
                        else if (WorldEntity_GetPlayerModelId() == WORLD_MODEL_HIGHWIND || s.ground < 2000)
                            WorldEntity_SetPlayerAltitude(s.ground + 200);
                        else
                            WorldEntity_RevertPlayerPosition();
                    }
                }
                World_UpdateSkyHorizonPolygon(g_WorldCameraRotationY);
                World_UpdateAndDrawSkyLayers(g_WorldCameraRotationY);
                World_StepStreamingStallFadeState();
                World_UpdateAndDrawScreenOverlays();
                if (g_WorldMode < WORLD_MAP_STATE_EXIT && g_WorldMapType != 3)
                    World_DrawMiniMap(g_WorldCameraRotationY);
                World_UpdateVehicleTransition(0);
                World_UpdateTerrainEffect();
                SetGeomScreen(g_WorldCameraRange);
                if (g_WorldMode >= WORLD_MAP_STATE_CAMERA_INTRO)
                    WorldEntity_UpdateAllActorModelsAndShadows();
                World_RunEffectNodeFrame();
                World_UpdateZolom();
                World_UpdateOverlayModelStateAndIdleCountdown();
                if (g_WorldMode == WORLD_MAP_STATE_PLAYING)
                    World_UpdateEncounters();
                World_DrawSkyPolygonQuad(g_WorldMovementMultiplier - (g_WorldMovementMultiplier == 1));
            }
        }
        if (g_WorldMode >= WORLD_MAP_STATE_DIVED) {
            WorldScript_RequestSystem(1);
            while (WorldScript_AnyScriptRunning())
                WorldScript_RunAllEntities();
            World_DisableTerrainStreamingBeforeDeparture();
            World_ExportTransitionRequestAndSaveState((int)dispatch, (int)field, (int)battle);
            if (*dispatch == 1) {
                World_TryEnterInitStep(3);
                Party_ReloadStatsAndLimitTablesIfFlagged();
            }
            World_StopAllSfxAndResetTranspose();
            if (g_WorldMode == WORLD_MAP_STATE_EXIT)
                g_WorldMapExitRequested = 1;
            g_WorldMapInitialized = 0;
        }
    }
    return World_GetRenderBufferIndex();
}

/*
 * Sets world resource buffer pointers, clears g_WorldMainUpdateMode for stages above 10 and calls the stage
 * loaders/setup helpers. Loads wm.ta, the variant asset name of kind 3, mes, field.tbl, atn.tbl
 * and enc_w.bin into fixed buffers; clears g_WorldTerrainAnimData for variants above 1 and performs the
 * variant-2 extra setup. Resets the listed world motion/view/state globals and sets defaults
 * including mode 2, enable 1, paired values 160, 1500, 10000 and 5000. The call to World_ResetMusicState
 * receives s.music without an explicit assignment in this body; no asset-load result is checked.
 */
// FUNCTION: FF7 0x0074e1e9
void __cdecl WorldMap_LoadStageResources(short stage)
{
    struct {
        int unused0, i, unused1[2], music, unused2[5];
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    (((WorldRenderBuffers *)&g_WorldRenderBuffers)->arenaRegion7) = (int)g_pWorldArenaRegion7;
    ((WorldRenderBuffers *)&g_WorldRenderBuffers)->arenaRegion7Second = (int)g_pWorldArenaRegion7 + 0x20800;
    if (stage > 10)
        g_WorldMainUpdateMode = 0;
    World_LoadStageAssets(stage);
    World_ResetMusicState(s.music);
    World_SelectMusicParamTableByState(stage);
    World_ResetModelRecordState();
    World_SetStageAndCopyModelSlotTable(stage);
    // STRING: FF7 0x00969b04
    WorldMap_LoadLgpAsset("wm.ta", 0xe000, (int *)&g_WorldTerrainAnimData, 0);
    if (g_WorldMapType > 1)
        (*(int *)&g_WorldTerrainAnimData) = 0;
    WorldMap_LoadLgpAsset(WorldMap_BuildAssetName(3, g_WorldMapType), 0x7000, g_WorldEventScriptImage, 0);
    // STRING: FF7 0x00969b0c
    WorldMap_LoadLgpAsset("mes", 0x1000, g_WorldMsgBuffer, 0);
    // STRING: FF7 0x00969b10
    WorldMap_LoadLgpAsset("field.tbl", 0x600, &g_WorldFieldTable, 0);
    // STRING: FF7 0x00969b1c
    WorldMap_LoadLgpAsset("atn.tbl", 0x400, g_WorldArcTangentTable, 0);
    // STRING: FF7 0x00969b24
    WorldMap_LoadLgpAsset("enc_w.bin", 0x8a0, g_WorldEncounterTable, 0);
    if (g_WorldMapType == 2) {
        World_InitUnderwaterColorVectorsAndNoise();
        for (s.i = 1; s.i < 2; s.i++)
            World_BuildUnderwaterNoisePaletteAndGradient(s.i);
    }
    g_WorldMovementMultiplier = 2;
    g_WorldIsControlEnabled = 1;
    g_WorldPlayerControlMode = 0;
    g_WorldFieldTransitionActive = 0;
    g_WorldPlayerAirborneFlag = 0;
    g_WorldControlLockCount = 0;
    g_WorldCameraHeight = 0;
    g_WorldCameraVar2 = g_WorldCameraVar1 = 160;
    g_WorldCameraBlendDirection = 0;
    g_WorldCameraBlendVelocity = 0;
    g_WorldCameraBlendWeight = 0;
    g_WorldCameraRotationZ = 0;
    g_WorldCameraBaseDist = 0;
    g_WorldCameraFront = g_WorldCameraRotationY = 0;
    g_WorldCameraDeltaY = 0;
    g_WorldCameraHeightOffset = 0;
    g_pWorldSelectedTerrainTriangle = 0;
    g_WorldCameraDistanceSmoothed = 1500;
    g_WorldCameraPositionZ = 10000;
    g_WorldCameraYawStep = g_WorldCameraHeightStep = g_WorldCameraDistanceStep = 0;
    g_WorldPreviousPlayerDirection = 0;
    g_WorldBattleCountdown = 0;
    g_WorldChocoboTerrainSet = 0;
    g_WorldCurveStartDepth = 5000;
    g_WorldProjectionJitterEnabled = 0;
    g_WorldRenderBufferIndex = 0;
    g_WorldCameraZoomState = 0;
    g_WorldFieldTransitionRequest = 0;
}

/*
 * For each of two pointers stored 0x4074 bytes apart starting at g_WorldRenderBuffers, visits 0xd00 vertex
 * records of 0x28 bytes. Preserves the low 24 bits of each first dword while setting its top byte
 * to 9, writes byte +7 to 0x34, then calls World_ResetSkyGradientPolygon.
 */
// FUNCTION: FF7 0x0074e48a
void __cdecl World_InitRenderVertexBuffersAndSky(void)
{
    int vert;
    int bufferIdx;
    int vertexIdx;

    for (bufferIdx = 0; bufferIdx < 2; bufferIdx++) {
        for (vertexIdx = 0; vertexIdx < 0xD00; vertexIdx++) {
            vert = *(int *)((char *)&g_WorldRenderBuffers + bufferIdx * 0x4074) + vertexIdx * 0x28;
            *(int *)vert = *(int *)vert & 0xFFFFFF | 0x9000000;
            *(char *)((char *)vert + 7) = 0x34;
        }
    }
    World_ResetSkyGradientPolygon();
}

/*
 * Flips render buffers and sets the geometry screen value. With g_WorldProjectionJitterEnabled nonzero, generates
 * small random X/Y projection offsets using two random bytes; otherwise uses X zero and
 * g_WorldCameraVar2-120. Doubles offsets unless g_WorldDisplayScale equals 1 and calls
 * SetProjectionOffsetAndRebuild.
 */
// FUNCTION: FF7 0x0074e505
void __cdecl World_UpdateProjectionOffset(void)
{
    int offsetX;
    int offsetY;
    int baseY;

    World_FlipRenderBuffers();
    SetGeomScreen(g_WorldCameraRange);
    if (g_WorldProjectionJitterEnabled != 0) {
        if (g_WorldDisplayScale == 1) {
            offsetX = (WorldMap_NextRandomByte() >> 6) - 2;
            offsetY = g_WorldCameraVar2 + (WorldMap_NextRandomByte() >> 6) + -0x7A;
        } else {
            offsetX = ((WorldMap_NextRandomByte() >> 6) - 2) * 2;
            offsetY = (g_WorldCameraVar2 - 122 + (WorldMap_NextRandomByte() >> 6)) * 2;
        }
        SetProjectionOffsetAndRebuild(offsetX, offsetY);
    } else {
        if (g_WorldDisplayScale == 1) {
            baseY = g_WorldCameraVar2 - 0x78;
        } else {
            baseY = (g_WorldCameraVar2 - 0x78) * 2;
        }
        offsetY = baseY;
        SetProjectionOffsetAndRebuild(0, offsetY);
    }
}

/*
 * Swaps the world's double-buffered render tables: toggles g_WorldRenderBufferIndex and points the vertex
 * arena (g_WorldRenderVertexArenaBase, g_WorldRenderVertexCursor), the dword table g_pWorldTerrainTable and g_pWorldRenderBufferHeader at the other
 * of the two 0x4074-byte buffers.
 */
// FUNCTION: FF7 0x0074e5cb
void __cdecl World_FlipRenderBuffers(void)
{
    g_WorldRenderBufferIndex = g_WorldRenderBufferIndex == 0;
    g_WorldRenderVertexArenaBase = *(int *)((char *)&g_WorldRenderBuffers + g_WorldRenderBufferIndex * 0x4074);
    g_WorldRenderVertexCursor = g_WorldRenderVertexArenaBase;
    g_pWorldTerrainTable = g_WorldRenderBufferIndex * 0x4074 + (int)&g_WorldRenderBuffer0TerrainTable;
    g_pWorldRenderBufferHeader = g_WorldRenderBufferIndex * 0x4074 + (int)&g_WorldRenderBuffer0Header;
}

// Draws the sky polygon quad while g_WorldFieldTransitionActive is nonzero. It acquires the batch
// node for g_WorldSkyRenderObject, and only when terrain rendering is enabled does it read the
// screen rectangle from World_GetSkyPolygon and fill four 32-byte vertices. Each corner gets posX,
// posY, its own colour (bgra0 to bgra3), posZ 0x3F7FF972, rhw 1.0f and black specular 0xFF000000.
// UVs are not written, and the list is not advanced here. Acquisition happens before the terrain
// gate. The unused constant 2500 and its clamp to 0xFFF are kept, and the clamp is unreachable.
// FUNCTION: FF7 0x0074e633
void __cdecl World_DrawSkyPolygonQuad(int frameStep)
{
    int clampValue;
    GfxTlVertex *vert0;
    GfxTlVertex *vert2;
    int x0;
    int topRightX;
    int y0;
    int y1;
    int y2;
    int bottomRemainingX;
    int y3;
    int bottomRightX;
    struct t_wm_SkyPoly *pSkyPolygon;
    GfxTlVertex *vert1;
    GfxTlVertex *vert3;

    clampValue = 0x9C4;
    if (clampValue > 0xFFF) {
        clampValue = 0xFFF;
    }
    if (g_WorldFieldTransitionActive != 0) {
        if (AcquireBatchNode(1, g_WorldSkyRenderObject) != 0) {
            if (World_IsTerrainRenderEnabled() != 0) {
                pSkyPolygon = (struct t_wm_SkyPoly *)World_GetSkyPolygon();
                vert0 = (GfxTlVertex *)(g_WorldSkyRenderObject->vertexCursor);
                x0 = pSkyPolygon->polyG4.x0;
                vert0->x = x0;
                y0 = pSkyPolygon->polyG4.y0;
                vert0->y = y0;
                vert0->zBits = FLOAT_0_9999_BITS;
                vert0->rhwBits = FLOAT_ONE_BITS;
                vert0->color = *(int *)&pSkyPolygon->bgra0;
                vert0->specular = 0xFF000000;
                vert1 = (GfxTlVertex *)(g_WorldSkyRenderObject->vertexCursor + 0x20);
                topRightX = pSkyPolygon->polyG4.x1;
                vert1->x = topRightX;
                y1 = pSkyPolygon->polyG4.y1;
                vert1->y = y1;
                vert1->zBits = FLOAT_0_9999_BITS;
                vert1->rhwBits = FLOAT_ONE_BITS;
                vert1->color = *(int *)&pSkyPolygon->bgra1;
                vert1->specular = 0xFF000000;
                vert2 = (GfxTlVertex *)(g_WorldSkyRenderObject->vertexCursor + 0x40);
                bottomRemainingX = pSkyPolygon->polyG4.x2;
                vert2->x = bottomRemainingX;
                y2 = pSkyPolygon->polyG4.y2;
                vert2->y = y2;
                vert2->zBits = FLOAT_0_9999_BITS;
                vert2->rhwBits = FLOAT_ONE_BITS;
                vert2->color = *(int *)&pSkyPolygon->bgra2;
                vert2->specular = 0xFF000000;
                vert3 = (GfxTlVertex *)(g_WorldSkyRenderObject->vertexCursor + 0x60);
                bottomRightX = pSkyPolygon->polyG4.x3;
                vert3->x = bottomRightX;
                y3 = pSkyPolygon->polyG4.y3;
                vert3->y = y3;
                vert3->zBits = FLOAT_0_9999_BITS;
                vert3->rhwBits = FLOAT_ONE_BITS;
                vert3->color = *(int *)&pSkyPolygon->bgra3;
                vert3->specular = 0xFF000000;
            }
        }
    }
}

/*
 * Walks g_WorldTerrainAnimData's terrain animation groups, computes each frame from current frame count
 * divided by tickDivisor modulo frameCount and assigns its g_WorldTerrainAnimFrameRenderObjects frame object to the
 * g_WorldTexturePageRenderObjects page selected by g_WorldAnimGroupPageOrder. Retains the unused computed
 * pointer and repeated frame-count call.
 */
// FUNCTION: FF7 0x0074e802
void __cdecl World_AnimateTerrainTextures(int frameStep)
{
    struct {
        int _ocal_5;
        unsigned i;
        int _ocal_3;
        int *frameData;
        struct WorldTerrainAnimGroup *animGroup;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.animGroup = (struct WorldTerrainAnimGroup *)((char *)&g_WorldTerrainAnimData + 4);
    for (frame.i = 0; frame.i < (*(int *)&g_WorldTerrainAnimData); frame.i++) {
        frame.frameData = ((int *)&g_WorldTerrainAnimData) + (frame.animGroup->dataOffset >> 2) +
                       (frame.animGroup->frameStride >> 2) *
                           (((unsigned)Savemap_GetClockTickCount() / frame.animGroup->tickDivisor) %
                            frame.animGroup->frameCount);
        g_WorldTexturePageRenderObjects[g_WorldAnimGroupPageOrder[frame.i]] =
            ((GfxRenderObject **)&g_WorldTerrainAnimFrameRenderObjects)[g_WorldElevationTable[frame.i] +
                                                         (((unsigned)Savemap_GetClockTickCount() / frame.animGroup->tickDivisor) %
                                                          frame.animGroup->frameCount)];
        frame.animGroup++;
    }
}

/*
 * Builds the world camera pitch/distance transform, computes a ratio from the transformed distance
 * and g_WorldCameraDeltaY, clamps its table index to 0..511 and selects g_WorldArcTangentTable plus
 * 0x800 as pitch. Stores g_WorldCurrentCameraRotationX and combines the yaw and
 * g_WorldCameraRotationZ rotations into g_WorldCameraRotMatrix; the ratio division is not guarded
 * against zero.
 */
// FUNCTION: FF7 0x0074e8ce
void __cdecl World_BuildCameraMatrix(short yaw)
{
    SVECTOR angles;
    int idx;
    int rotated[3];
    int r;
    int t;
    int translation[3];
    char zMtx[0x20];
    char pitchYawMatrix[0x20];

    angles.vx = World_UpdateCameraDistanceAndPitch();
    angles.vz = 0;
    angles.vy = angles.vz;
    RotMatrixXYZ(&angles, &pitchYawMatrix);
    translation[2] = 0;
    translation[1] = translation[2];
    translation[0] = translation[1];
    SetMatrixTranslation(&pitchYawMatrix, &translation);
    SetRotMatrix(&pitchYawMatrix);
    SetTransMatrix(&pitchYawMatrix);
    angles.vy = 0;
    angles.vx = angles.vy;
    angles.vz = *(short *)&g_WorldCameraPositionZ;
    RotTransPoint(&angles, (int *)&rotated, &idx);
    translation[0] = 0;
    translation[1] = 0;
    idx = (rotated[1] << 4) - g_WorldCameraDeltaY;
    translation[2] = g_WorldCameraPositionZ;
    SetMatrixTranslation(&g_WorldCameraTransMatrix, &translation);
    idx = (idx << 8) / (rotated[2] << 4);
    if (idx < 0) {
        idx = 0;
    } else if (idx >= 0x200) {
        idx = 0x1FF;
    }
    angles.vx = g_WorldArcTangentTable[idx] + 0x800;
    g_WorldCurrentCameraRotationX = angles.vx;
    angles.vy = yaw;
    angles.vz = 0;
    RotMatrixXYZ(&angles, &pitchYawMatrix);
    angles.vy = 0;
    angles.vx = angles.vy;
    angles.vz = *(short *)&g_WorldCameraRotationZ;
    RotMatrixXYZ(&angles, &zMtx);
    ConcatMatrixPreservingTranslation(&zMtx, &pitchYawMatrix, g_WorldCameraRotMatrix);
}

/*
 * Updates world player movement from held input when control, cooldown and availability gates
 * allow it. Chooses model-dependent speed, combines directional input with camera-relative
 * rotation, acceleration or strafing, updates facing and altitude for the supported modes, and
 * smooths camera tilt. Handles input edges for camera changes, terrain-dependent mode changes, the
 * map display and system/menu requests; stores the filtered input, applies terrain movement
 * adjustment, moves the current entity and resolves collision. When control is disabled on terrain
 * 14, still applies the terrain movement helper and collision resolution. Always smooths camera
 * distance and wrapped heading after these branches; the final comma expression selects the slower
 * heading smoothing branch unconditionally.
 */
// FUNCTION: FF7 0x0074ea48
void __cdecl World_UpdatePlayerControl(void)
{
    struct {
        int acceleration, turns;
        VECTOR transformed;
        int dx, shift, vertical;
        PsxMatrix32 matrix;
        int strafe, input;
        SVECTOR movement;
        int flags, model;
        VECTOR position;
        SVECTOR rotation;
        int speed, tilt, dz;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.dx = s.dz = 0;
    WorldEntity_SelectPlayerAsCurrentActor();
    s.model = WorldEntity_GetPlayerModelId();
    if (g_WorldIsControlEnabled && g_WorldControlLockCount <= 0 && World_GetTerrainWalkCompletedFlag()) {
        s.input = Input_GetHeldRaw();
        if (g_WorldCameraViewtype != 2 && g_WorldCameraViewtype != 3 && g_WorldMapType != 3 && (s.input & (INPUT_L1 | INPUT_R1)))
            World_SetCameraMode(2);
        if (s.model == WORLD_MODEL_TINY_BRONCO && (s.input & INPUT_CROSS))
            s.input &= ~(INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT);
        if (WorldEntity_IsPlayerModelInMask(1 << WORLD_MODEL_SUBMARINE) && g_WorldCameraViewtype == 2 && g_WorldMapType != 2 && (s.input & INPUT_CIRCLE))
            s.input &= ~INPUT_DOWN;
        if (WorldEntity_IsPlayerRideable())
            s.speed = g_WorldMovementMultiplier * 60;
        else {
            switch (s.model) {
            case WORLD_MODEL_HIGHWIND:
                s.speed = g_WorldMovementMultiplier * 120;
                break;
            case WORLD_MODEL_TINY_BRONCO:
                s.speed = g_WorldMovementMultiplier * 60;
                break;
            case WORLD_MODEL_BUGGY:
                s.speed = g_WorldMovementMultiplier * 45;
                break;
            default:
                s.speed = g_WorldMovementMultiplier * 30;
            }
        }
        if (((s.input & INPUT_L2) && !(g_WorldPrevKeyInputStatus & INPUT_L2)) || ((s.input & INPUT_R2) && !(g_WorldPrevKeyInputStatus & INPUT_R2))) {
            if (g_WorldCameraViewtype != 3 && g_WorldCameraHeight == 0 && g_WorldMapType != 2 && g_WorldMapType != 3) {
                World_SetCameraMode(g_WorldCameraViewtype != 0 ? 0 : 2);
                g_WorldCameraVar1 = g_WorldCameraViewtype != 0 ? 160 : 120;
            }
        }
        if (WorldEntity_IsPlayerModelInMask(1 << WORLD_MODEL_SUBMARINE) && g_WorldCameraViewtype == 2 && g_WorldMapType != 2 && (s.input & INPUT_CIRCLE))
            s.input |= INPUT_UP;
        g_WorldPlayerFacing = -1;
        if (s.input & INPUT_LEFT) {
            s.dx = -s.speed;
            g_WorldPlayerFacing = -0x400;
        }
        if (s.input & INPUT_RIGHT) {
            s.dx = s.speed;
            g_WorldPlayerFacing = 0x400;
        }
        if (s.input & INPUT_UP) {
            if (g_WorldPlayerFacing == -1) {
                s.dz = -s.speed;
                g_WorldPlayerFacing = 0x800;
            } else {
                s.dx = (s.dx * 3) >> 2;
                s.dz = (s.speed * -3) >> 2;
                g_WorldPlayerFacing += g_WorldPlayerFacing >> 1;
            }
        }
        if (s.input & INPUT_DOWN) {
            if (g_WorldPlayerFacing == -1) {
                s.dz = s.speed;
                g_WorldPlayerFacing = 0;
            } else {
                s.dx = (s.dx * 3) >> 2;
                s.dz = (s.speed * 3) >> 2;
                g_WorldPlayerFacing -= g_WorldPlayerFacing >> 1;
            }
        }
        if (g_WorldCameraViewtype == 2 || g_WorldCameraViewtype == 3 || g_WorldMapType == 3) {
            s.strafe = g_WorldCameraViewtype == 3 && (s.input & INPUT_SQUARE);
            if (g_WorldMapType == 3) {
                g_WorldCameraFront += World_UpdateRandomSteeringCycleAndSpawnEntity(s.input & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT), (s.input & INPUT_CIRCLE) && !(g_WorldPrevKeyInputStatus & INPUT_CIRCLE));
            } else if (!s.strafe) {
                s.turns = ((s.input & INPUT_L1) != 0) + ((s.input & ((g_WorldCameraViewtype == 2 && g_WorldMapType != 2 && (s.input & INPUT_DOWN)) ? INPUT_RIGHT : INPUT_LEFT)) != 0);
                if (s.turns)
                    g_WorldCameraFront -= ((g_WorldMovementMultiplier << 3) * s.turns * ((s.input & (INPUT_UP | INPUT_DOWN)) ? 1 : 2)) >> (g_WorldMapType == 2 && (s.input & INPUT_L1) == 0);
                s.turns = ((s.input & INPUT_R1) != 0) + ((s.input & ((g_WorldCameraViewtype == 2 && g_WorldMapType != 2 && (s.input & INPUT_DOWN)) ? INPUT_LEFT : INPUT_RIGHT)) != 0);
                if (s.turns)
                    g_WorldCameraFront += ((g_WorldMovementMultiplier << 3) * s.turns * ((s.input & (INPUT_UP | INPUT_DOWN)) ? 1 : 2)) >> (g_WorldMapType == 2 && (s.input & INPUT_R1) == 0);
            }
            if (g_WorldCameraFront < 0)
                g_WorldCameraFront += 0x1000;
            else if (g_WorldCameraFront >= 0x1000)
                g_WorldCameraFront -= 0x1000;
            s.tilt = 0;
            if (s.input & INPUT_LEFT)
                s.tilt = 128;
            else if (s.input & INPUT_RIGHT)
                s.tilt = -128;
            if (g_WorldCameraViewtype == 3 || g_WorldMapType == 2) {
                s.acceleration = ((s.input & (s.strafe ? (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT) : INPUT_CIRCLE)) ? ~0 : 0) & s.speed;
                g_WorldSpecialDeltaMovement = g_WorldMapType == 2 ? s.acceleration : (g_WorldSpecialDeltaMovement * 3 + s.acceleration) >> 2;
                if (s.strafe) {
                    s.dx = (s.input & INPUT_LEFT) ? -g_WorldSpecialDeltaMovement : (s.input & INPUT_RIGHT) ? g_WorldSpecialDeltaMovement
                                                                                   : 0;
                    s.dz = (s.input & INPUT_UP) ? -g_WorldSpecialDeltaMovement : (s.input & INPUT_DOWN) ? g_WorldSpecialDeltaMovement
                                                                                   : 0;
                } else {
                    s.dx = 0;
                    s.dz = -g_WorldSpecialDeltaMovement;
                }
                if (!(s.input & INPUT_CIRCLE))
                    s.tilt = 0;
            } else if (g_WorldPlayerFacing != -1) {
                WorldEntity_SetCurrentFacing(g_WorldPlayerFacing - g_WorldCameraFront);
            }
            s.movement.vx = s.dx;
            s.movement.vy = 0;
            s.movement.vz = s.dz;
            s.rotation.vx = s.rotation.vz = 0;
            s.rotation.vy = -g_WorldCameraFront;
            RotMatrixXYZ(&s.rotation, &s.matrix);
            s.position.vx = s.position.vy = s.position.vz = 0;
            SetMatrixTranslation(&s.matrix, &s.position);
            SetRotMatrix(&s.matrix);
            SetTransMatrix(&s.matrix);
            RotTransPoint(&s.movement, (int *)&s.transformed, &s.flags);
            s.dx = s.transformed.vx;
            s.dz = s.transformed.vz;
            s.vertical = 0;
            if (g_WorldCameraViewtype == 3 && !s.strafe) {
                WorldEntity_GetPlayerPosition((VECTOR *)&s.position);
                if (((s.input & INPUT_UP) && s.position.vy > 500 && g_WorldPlayerAirborneFlag && WorldEntity_GetPlayerTerrainType() != WORLD_TERRAIN_NORTHERN_CRATER) || s.position.vy > g_WorldHighwindHeightLowerbound) {
                    WorldEntity_AddCurrentAltitude(g_WorldMovementMultiplier * -50);
                    s.vertical = 10;
                } else if ((s.input & INPUT_DOWN) && s.position.vy < g_WorldHighwindHeightLowerbound - g_WorldMovementMultiplier * 50) {
                    WorldEntity_AddCurrentAltitude(g_WorldMovementMultiplier * 50);
                    s.vertical = -10;
                }
            }
            if (g_WorldMapType == 2) {
                WorldEntity_GetPlayerPosition((VECTOR *)&s.position);
                if ((s.input & INPUT_UP) && s.position.vy > -5000 && g_WorldPlayerAirborneFlag) {
                    WorldEntity_AddCurrentAltitude(g_WorldMovementMultiplier * -30);
                    s.vertical = 10;
                }
                if ((s.input & INPUT_DOWN) && s.position.vy < -3000) {
                    WorldEntity_AddCurrentAltitude(g_WorldMovementMultiplier * 30);
                    s.vertical = -10;
                }
            }
            if (g_WorldCameraViewtype == 3) {
                s.shift = ((!((WorldEntity_HasCurrentMoved() == 0 || (g_WorldCameraRotationZ > 0 ? g_WorldCameraRotationZ : -g_WorldCameraRotationZ) < 16) && s.tilt == 0) << 1) | (g_WorldMovementMultiplier == 1)) + 3;
                g_WorldCameraRotationZ = (((1 << s.shift) - 1) * g_WorldCameraRotationZ + s.tilt + (1 << (s.shift - 1))) >> s.shift;
                g_WorldCameraPitchOffset = (g_WorldCameraPitchOffset * 3 + s.vertical) >> 2;
                World_SetCameraAngleOffsets(g_WorldCameraPitchOffset + 4, g_WorldCameraRotationZ << 2, -g_WorldCameraRotationZ << 1);
            }
            if (s.tilt == 0 && (g_WorldCameraRotationZ > 0 ? g_WorldCameraRotationZ : -g_WorldCameraRotationZ) <= 4)
                g_WorldCameraRotationZ = 0;
            if (g_WorldMapType == 3) { /* empty first arm: lib/codegen.h */
            } else {
                if (g_WorldPlayerFacing != -1) {
                    g_WorldPlayerFacing += 0x800;
                    if (g_WorldPlayerFacing > 0x800)
                        g_WorldPlayerFacing -= 0x1000;
                    if ((s.input & INPUT_DOWN) == 0)
                        g_WorldPlayerFacing >>= 1;
                    g_WorldPreviousPlayerDirection = g_WorldPlayerFacing;
                } else {
                    g_WorldPlayerFacing = g_WorldPreviousPlayerDirection;
                }
                WorldEntity_SetCurrentFacing(0x800 - g_WorldCameraFront + ((g_WorldCameraViewtype == 2 && g_WorldMapType != 2) ? g_WorldPlayerFacing : 0));
            }
        } else {
            g_WorldCameraRotationZ >>= 1;
            if (g_WorldPlayerFacing != -1)
                WorldEntity_SetCurrentFacing(g_WorldPlayerFacing);
        }
        if (s.model == 6) {
            if ((s.input & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)) && !(g_WorldPrevKeyInputStatus & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)))
                World_SetAllSfxChannelsTransposeTrans(8, 32);
            if ((s.input & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)) == 0 && (g_WorldPrevKeyInputStatus & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)))
                World_SetAllSfxChannelsTransposeTrans(8, 0);
        }
        if (WorldEntity_IsPlayerModelInMask(1 << WORLD_MODEL_SUBMARINE) && (s.input & INPUT_CROSS) && !(g_WorldPrevKeyInputStatus & INPUT_CROSS)) {
            if (g_WorldMapType == 2 && WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_SEA)
                World_BeginSubmarineSurfacing();
            else if (WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_SEA)
                World_BeginSubmarineDive();
        }
        if (((s.input & INPUT_START) && !(g_WorldPrevKeyInputStatus & INPUT_START)) || ((s.input & INPUT_SELECT) && !(g_WorldPrevKeyInputStatus & INPUT_SELECT))) {
            World_SetMiniMapState(g_WorldMapType == 0 ? (World_GetMiniMapState() + 1) % 3 : !World_GetMiniMapState() * 2);
        }
        if ((s.input & INPUT_TRIANGLE) && !(g_WorldPrevKeyInputStatus & INPUT_TRIANGLE) && g_WorldMode == WORLD_MAP_STATE_PLAYING) {
            if (s.model == 3)
                WorldScript_RequestSystem(6);
            else if (WorldEntity_IsPlayerModelInMask(0x47) && WorldEntity_GetPlayerTerrainType() != WORLD_TERRAIN_WUTAI_BRIDGE) {
                World_BeginFadeToBlack(16, 1);
                WorldMap_SetPlayerControl(0, 1);
                g_WorldMode = WORLD_MAP_STATE_FADE_OUT;
                World_SetDoubleFrameEnabled(0);
            }
        }
        if (s.input & (INPUT_LEFT | INPUT_RIGHT | INPUT_L1 | INPUT_R1))
            WorldMap_NextRandomByte();
        g_WorldPrevKeyInputStatus = s.input;
        if (WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_WUTAI_BRIDGE)
            World_AddTerrainCurrentDrift(&s.dx, &s.dz);
        if (s.model == 5)
            World_RequestAmbienceSfx((s.dx | s.dz) ? 493 : -493);
        WorldEntity_MoveCurrentBy(s.dx, s.dz);
        WorldEntity_ResolveCurrentCollision();
    }
    if (!g_WorldIsControlEnabled && WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_WUTAI_BRIDGE) {
        World_AddTerrainCurrentDrift(&s.dx, &s.dz);
        WorldEntity_MoveCurrentBy(s.dx, s.dz);
        WorldEntity_ResolveCurrentCollision();
    }
    if (g_WorldCameraViewtype != 3)
        g_WorldCameraVar2 = (g_WorldCameraVar2 * 3 + g_WorldCameraVar1) >> 2;
    if (g_WorldCameraRotationY < g_WorldCameraFront - 0x800)
        g_WorldCameraRotationY += 0x1000;
    else if (g_WorldCameraRotationY > g_WorldCameraFront + 0x800)
        g_WorldCameraRotationY -= 0x1000;
    g_WorldCameraRotationY = UNFOLDED(3) ? (g_WorldMovementMultiplier == 1 ? (g_WorldCameraRotationY * 31 + g_WorldCameraFront) >> 5 : (g_WorldCameraRotationY * 15 + g_WorldCameraFront) >> 4) : (g_WorldMovementMultiplier == 1 ? (g_WorldCameraRotationY * 7 + g_WorldCameraFront) >> 3 : (g_WorldCameraRotationY * 3 + g_WorldCameraFront) >> 2);
}

/*
 * Scales the four world draw distances by the fade level g_WorldCameraBlendWeight (0..0x100): g_WorldCameraVar2
 * from 0x78 or 0xA0 (by the previous camera mode), g_WorldCameraRange from 0x190, g_WorldCameraPositionZ from
 * 0x2710 and the fade target g_WorldCurveStartDepth from 0x1388, each reduced proportionally.
 */
// FUNCTION: FF7 0x0074f87d
void __cdecl World_UpdateCameraBlendParameters(void)
{
    int val;

    if (g_WorldCameraMode == 0) {
        val = 0x78 - (g_WorldCameraBlendWeight * 0x1E >> 8);
    } else {
        val = 0xA0 - (g_WorldCameraBlendWeight * 0x46 >> 8);
    }
    g_WorldCameraVar2 = val;
    g_WorldCameraRange = 0x190 - (g_WorldCameraBlendWeight * 0xC8 >> 8);
    g_WorldCameraPositionZ = 0x2710 - (g_WorldCameraBlendWeight * 0x1770 >> 8);
    g_WorldCurveStartDepth = 0x1388 - (g_WorldCameraBlendWeight * 0x9C4 >> 8);
}

/*
 * Advances the camera transition accumulator, switching to mode 3 on a positive transition and
 * restoring the saved mode and player control when a negative transition stops. Clamps its blend
 * weight to 0 through 256, interpolates player altitude against the selected target and optionally
 * blends camera heading. Selects distance and pitch targets from active transition values or
 * lookup tables, smoothing with weights 7/8 or 3/4 according to frame step. Returns a weighted
 * blend of smoothed distance and the alternate camera distance. Position helper outputs rely on
 * contiguous native locals.
 */
// FUNCTION: FF7 0x0074f916
int __cdecl World_UpdateCameraDistanceAndPitch(void)
{
    int outGroundY;
    int position[4];
    int tableIndex;
    int selector;
    int distSmoothed;
    int hTarget;
    int pickedZTarget;
    int zSmoothed;
    int dest;
    VECTOR lastPos;
    int targetAlt;

    if (g_WorldCameraBlendDirection != 0) {
        if (g_WorldCameraBlendVelocity == 0) {
            if (g_WorldCameraBlendDirection > 0) {
                World_SetCameraMode(3);
                g_WorldHighwindHeightLowerbound = (WorldEntity_GetPlayerModelId() - 3 != 0 ? 0xFFFFF830 : 0) + 0xFA0;
            }
            g_WorldCameraFrontWrapped = g_WorldCameraFront;
            if (g_WorldCameraFrontWrapped >= 0x800) {
                g_WorldCameraFrontWrapped -= 0x1000;
            }
        }
        g_WorldCameraBlendVelocity += g_WorldMovementMultiplier * ((g_WorldCameraBlendWeight < 0x80) * 2 - 1);
        if (g_WorldCameraBlendVelocity != 0) {
            g_WorldCameraBlendWeight += g_WorldCameraBlendVelocity;
            if (g_WorldCameraBlendWeight < 0) {
                g_WorldCameraBlendWeight = 0;
            } else if (g_WorldCameraBlendWeight > 0x100) {
                g_WorldCameraBlendWeight = 0x100;
            }
            World_UpdateCameraBlendParameters();
            WorldEntity_GetPlayerPosition((VECTOR *)position);
            WorldEntity_GetPlayerPrevPosition(&lastPos);
            outGroundY = WorldEntity_GetPlayerGroundY();
            if (g_WorldCameraBlendDirection > 0) {
                targetAlt = g_WorldHighwindHeightLowerbound;
            } else {
                targetAlt = lastPos.vy;
            }
            WorldEntity_SetPlayerAltitudeExplicit(outGroundY + ((targetAlt - outGroundY) * g_WorldCameraBlendWeight >> 8));
            if (g_WorldCameraMode == 0 || g_WorldCameraMode == 1) {
                g_WorldCameraFront = g_WorldCameraBlendWeight * g_WorldCameraFrontWrapped >> 8;
            }
        } else {
            if (g_WorldCameraBlendDirection < 0) {
                World_SetCameraMode(g_WorldCameraMode);
                WorldMap_SetPlayerControl(0, 2);
            }
            g_WorldCameraBlendDirection = 0;
        }
    }
    if (g_WorldCameraBlendDirection < 0 && g_WorldCameraMode == 0) {
        selector = 0;
    } else {
        selector = g_WorldCameraViewtype;
    }
    tableIndex = selector;
    if (g_WorldCameraHeight != 0) {
        hTarget = g_WorldCameraHeight;
    } else {
        hTarget = ((int *)&g_WorldTerrainTable)[tableIndex] + g_WorldCameraHeightOffset;
    }
    dest = hTarget;
    if (g_WorldMovementMultiplier == 1) {
        distSmoothed = g_WorldCameraDistanceSmoothed * 7 + dest >> 3;
    } else {
        distSmoothed = g_WorldCameraDistanceSmoothed * 3 + dest >> 2;
    }
    g_WorldCameraDistanceSmoothed = distSmoothed;
    if (g_WorldCameraDistanceTarget != 0) {
        pickedZTarget = g_WorldCameraDistanceTarget;
    } else {
        pickedZTarget = g_WorldCameraHeights[tableIndex];
    }
    dest = pickedZTarget;
    if (g_WorldMovementMultiplier == 1) {
        zSmoothed = g_WorldCameraPositionZ * 7 + dest >> 3;
    } else {
        zSmoothed = g_WorldCameraPositionZ * 3 + dest >> 2;
    }
    g_WorldCameraPositionZ = zSmoothed;
    return g_WorldCameraBlendWeight * ((g_WorldCameraDeltaY >> 5) + 0x6D6) + (0x100 - g_WorldCameraBlendWeight) * g_WorldCameraDistanceSmoothed >> 8;
}

/*
 * When g_PartyStatsDirty equals 1, reloads active party stats, refreshes the party limit tables, then
 * clears only the low byte of g_PartyStatsDirty.
 */
// FUNCTION: FF7 0x0074fbca
void __cdecl Party_ReloadStatsAndLimitTablesIfFlagged(void)
{
    if (g_PartyStatsDirty == 1) {
        Savemap_ReloadActivePartyStats();
        Menu_RefreshPartyLimitTables();
        *(char *)&g_PartyStatsDirty = 0;
    }
}

/*
 * Advances the world-map transition state. State 0 increments two active counters by 80 and 3, sets
 * completion flags at table thresholds and enters state 1 after both clear. State 2 waits for
 * World_IsScreenFadeIdle and either enters state 3, performs a one-time savemap and transition
 * setup, or restores player control and map state on the next phase. States 4 and 5 raise player
 * altitude by a growing signed step until entering 6 or 7; state 3 enters 9. Negative states count
 * toward zero, then enable the completion flag for eligible transition kinds, enter 1 and call
 * World_BeginFadeFromBlack.
 */
// FUNCTION: FF7 0x0074fbec
void __cdecl World_UpdateMapStateMachine(void)
{
    VECTOR position;
    switch (g_WorldMode) {
    case WORLD_MAP_STATE_CAMERA_INTRO:
        if (g_WorldCameraDistanceTarget) {
            g_WorldCameraDistanceTarget += 80;
            if (g_WorldCameraDistanceTarget >= (g_WorldCameraHeights[g_WorldCameraViewtype] >> 1) && g_WorldMapType != 2 && g_WorldMapType != 3)
                g_WorldFieldTransitionActive = 1;
            if (g_WorldCameraDistanceTarget >= g_WorldCameraHeights[g_WorldCameraViewtype]) {
                g_WorldCameraDistanceTarget = 0;
                if (g_WorldCameraHeight == 0)
                    g_WorldMode = WORLD_MAP_STATE_PLAYING;
            }
        }
        if (g_WorldCameraHeight) {
            g_WorldCameraHeight += 3;
            if (g_WorldCameraHeight >= ((int *)&g_WorldTerrainTable)[g_WorldCameraViewtype]) {
                g_WorldCameraHeight = 0;
                if (g_WorldMapType != 2 && g_WorldMapType != 3)
                    g_WorldFieldTransitionActive = 1;
                if (g_WorldCameraDistanceTarget == 0)
                    g_WorldMode = WORLD_MAP_STATE_PLAYING;
            }
        }
        break;
    case WORLD_MAP_STATE_FADE_OUT:
        if (World_IsScreenFadeIdle()) {
            if (g_WorldFieldTransitionRequest) {
                g_WorldMode = WORLD_MAP_STATE_LEAVING;
                g_WorldFieldTransitionRequest = 0;
            } else {
                if (!g_WorldMapInMenu) {
                    World_StoreAreaNameToSavemap(WorldEntity_GetPlayerEncounterRegion());
                    World_SaveStateToSavemap();
                    World_TryEnterInitStep(3);
                    Party_ReloadStatsAndLimitTablesIfFlagged();
                    World_MapStateHookNoOp();
                    g_WorldMapInMenu = 1;
                    World_UpdateAndDrawScreenOverlays();
                } else {
                    World_LeaveInitStep(3);
                    WorldMap_SetPlayerControl(1, 1);
                    World_SyncPartyAndLeaderModel();
                    World_InitRenderVertexBuffersAndSky();
                    World_UpdateProjectionOffset();
                    World_BeginFadeFromBlack(16, 1);
                    g_WorldMode = WORLD_MAP_STATE_PLAYING;
                    g_WorldMapInMenu = 0;
                    World_SetDoubleFrameEnabled(1);
                }
            }
        }
        break;
    case WORLD_MAP_STATE_DIVING:
    case WORLD_MAP_STATE_SURFACING:
        WorldEntity_GetPlayerPosition((VECTOR *)&position);
        WorldEntity_SetPlayerAltitude(position.vy + g_WorldExitAltitudeSpeed);
        g_WorldExitAltitudeSpeed += g_WorldExitAltitudeSpeed >> 2;
        if (World_IsScreenFadeIdle())
            g_WorldMode = g_WorldMode == WORLD_MAP_STATE_DIVING ? WORLD_MAP_STATE_DIVED : WORLD_MAP_STATE_SURFACED;
        break;
    case WORLD_MAP_STATE_LEAVING:
        g_WorldMode = WORLD_MAP_STATE_EXIT;
        break;
    default:
        if (g_WorldMode < WORLD_MAP_STATE_CAMERA_INTRO) {
            g_WorldMode++;
            if (g_WorldMode == WORLD_MAP_STATE_CAMERA_INTRO) {
                if (g_WorldMapType != 2 && g_WorldMapType != 3)
                    g_WorldFieldTransitionActive = 1;
                g_WorldMode = WORLD_MAP_STATE_PLAYING;
                World_BeginFadeFromBlack(16, 1);
            }
        }
    }
}

/*
 * Empty. A per-frame hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x0074fe84
void __cdecl World_MapStateHookNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x0074fe84,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * In mode g_WorldMode==1 with g_WorldMapType!=2, smooths g_WorldCameraWaterSmoothed toward g_WorldTerrainHasWater,
 * updates its threshold flag/run length and resets a fifteen-step delay when the flag changes.
 * After player movement or the run threshold, enables g_WorldCameraHeightAdjustActive when the old countdown is
 * negative and nudges g_WorldCameraHeightOffset by five toward -350 or zero according to the flag.
 */
// FUNCTION: FF7 0x0074fe89
void __cdecl World_UpdateCameraHeightOffsetForOcclusion(void)
{
    if (g_WorldMode == WORLD_MAP_STATE_PLAYING && g_WorldMapType != 2) {
        g_WorldCameraWaterSmoothed = ((g_WorldCameraWaterSmoothed * 7) + (g_WorldTerrainHasWater << 8)) >> 3;
        g_WorldCameraOcclusionFlag = g_WorldCameraWaterSmoothed > 0x80;
        g_WorldCameraOcclusionRunLength = g_WorldCameraOcclusionFlag ? g_WorldCameraOcclusionRunLength + 1 : 0;
        if (g_WorldCameraPrevOcclusionFlag != g_WorldCameraOcclusionFlag) {
            g_WorldCameraHeightAdjustActive = 0;
            g_WorldCameraHeightAdjustDelay = 15;
        }
        if (WorldEntity_HasPlayerMoved() || g_WorldCameraOcclusionRunLength >= 15) {
            if ((g_WorldCameraHeightAdjustDelay--) < 0) {
                g_WorldCameraHeightAdjustActive = 1;
            }
        }
        if (g_WorldCameraHeightAdjustActive) {
            if (g_WorldCameraOcclusionFlag) {
                if (g_WorldCameraHeightOffset > -350) {
                    g_WorldCameraHeightOffset -= 5;
                }
            } else if (g_WorldCameraHeightOffset < 0) {
                g_WorldCameraHeightOffset += 5;
            }
        }
        g_WorldCameraPrevOcclusionFlag = g_WorldCameraOcclusionFlag;
    }
}

/*
 * Returns the exit sequence state g_WorldMode.
 */
// FUNCTION: FF7 0x0074ffa8
int __cdecl World_GetMapState(void)
{
    return g_WorldMode;
}
} /* extern "C" */

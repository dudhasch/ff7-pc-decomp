// src/ff7/src/wm/wm_world_zones_sky.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: World map zones and sky: the zone record setters and queries, the sky gradient, horizon
//   polygons and sprites, the world fade and overlay channels, the Zolom trail and screen flash,
//   and the world random table and angle helpers.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/main.h"
#include "lib/types/graphics.h"
#include "ff7/enums/wm.h"
#include "ff7/api/wm.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"

#include <string.h>
#include <stdio.h>

#include "ff7/types/wm.h"
extern "C" {
// clang-format off
// GLOBAL: FF7 0x00969b58
unsigned char g_WorldSkyColor1[4] = {0x00, 0x00, 0xff, 0x00};
// GLOBAL: FF7 0x00969b5c
unsigned char g_WorldSkyColor2[4] = {0xfa, 0xff, 0xff, 0x00};
// Pre-built world-map sky backdrop polygon and trailing sprite record.
// GLOBAL: FF7 0x00969b60
struct t_wm_SkyPoly g_WorldSkyPoly = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 320, 0, 0, 0, 0, 0, 0, 180, 0, 0, 0, 0, 320, 180}, 0, 0, 0, 0};
// GLOBAL: FF7 0x00969b98
unsigned char g_WorldSkySprite[8] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00};
unsigned char g_rawData_00969ba0[104] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x8f, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0x3f, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00969c08
unsigned char g_WorldUnderwaterColor[36] = {0xee, 0x06, 0x00, 0x00, 0x00, 0x00, 0xee, 0x06, 0x00, 0x00, 0x00, 0x00, 0xee, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char g_rawData_00969c2c[4] = {0x20, 0x00, 0x00, 0x00};
unsigned char g_rawData_00969c30[56] = {0x30, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00969c68
unsigned char g_WorldUnderwaterNoise[4] = {0x80, 0x00, 0x00, 0x00};
unsigned char g_rawData_00969c6c[4] = {0x60, 0x00, 0x00, 0x00};
unsigned char g_rawData_00969c70[56] = {
    0x50, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
};
// Three 0x14-byte Midgar Zolom / terrain-current path records followed by a zero terminator word.
// GLOBAL: FF7 0x00969ca8
WorldTerrainCurrentTable g_WorldZolomSegments = {
    {{0x1464, 0x109a, 0x0190, 0, {0x5c, 0x5b, 0x60, 0x5f, 0x5e, 0x5d, 0x56, 0x55, 0x59, 0x5a, 0x57, 0x58}},
     {0x0fab, 0x0bbd, 0x03e8, 0, {0x64, 0x63, 0x67, 0x68, 0x66, 0x65, 0x5e, 0x5d, 0x61, 0x62, 0x5f, 0x60}},
     {0x0e93, 0x0e9e, 0x0064, 0, {0x66, 0x65, 0x68, 0x67, 0x61, 0x62, 0x60, 0x5f, 0x63, 0x64, 0x69, 0x6a}}},
    0};
// The 16 world zone records of 0x24 bytes: World_ResetZonesAndDefaultLighting clears posY and
// enabled of each, and the zone walk reads them as g_WorldZolomTrail.zones[i].
// GLOBAL: FF7 0x00e28ce0
struct WorldZolomTrail g_WorldZolomTrail = {0};
// The sky strips' scroll offset (0..255 texels of wm_kumo.tim), updated by World_UpdateAndDrawSkyLayers.
// GLOBAL: FF7 0x00e28f20
int g_WorldFlashState = 0;
// GLOBAL: FF7 0x00e28f24
short g_WorldSteeringCountdown = 0;
// GLOBAL: FF7 0x00e28f28
int g_WorldRandomSeed = 0;
#define g_WorldRandomSeeds (&g_WorldRandomSeed)
// GLOBAL: FF7 0x00e28f2c
int g_WorldRandomIndex = 0;
// GLOBAL: FF7 0x00e28f30
int g_WorldRandomCounter = 0;
unsigned char g_bssPad_00e28f34[4] = {0};
// GLOBAL: FF7 0x00e28f38
struct WorldSkyVertexQuad g_WorldSkyVertexQuad = {0};
// The world map's float colour ramp: 256 FloatColor entries (b, g, r, a) converted from g_WorldTerrainEffectSlots.
// World_BuildUnderwaterNoisePaletteAndGradient writes it at init and wm_world_terrain_geometry.cpp's terrain lighting reads it.
// GLOBAL: FF7 0x00e28f40
unsigned char g_WorldScratchColorBuffer[0x1000] = {0};
// GLOBAL: FF7 0x00e29f40
int g_WorldSkyHorizonY = 0;
// GLOBAL: FF7 0x00e29f44
int g_WorldSkyHorizonOffset = 0;
// GLOBAL: FF7 0x00e29f48
short g_WorldSteeringEntityIndex = 0;
unsigned char g_bssPad_00e29f4c[4] = {0};
// The three flood path followers' target positions (x, y, z, pad) updated by World_UpdateTerrainFloodPathFollowers.
// GLOBAL: FF7 0x00e29f50
VECTOR g_WorldSkySprites[3] = {0};
// The Zolom's trail ring: 48 eight-byte records (x, z, heading, 0) from here up to g_WorldSnakeDataPositionEnd, which
// World_InitZolomTrailRecords fills, World_AdvanceZolomTrailHead advances and World_DrawZolomBodySegments reads
// (its wrap start is the last record before g_WorldSnakeDataPositionEnd).
// GLOBAL: FF7 0x00e29f80
WorldZolomTrailPoint g_WorldSnakeDataPosition[48] = {0};
// GLOBAL: FF7 0x00e2a100
int g_WorldSnakeDataPositionEnd[3] = {0};
// GLOBAL: FF7 0x00e2a10c
short g_WorldZolomTurnCountdown = 0;
// World_UpdateZolom's previous terrain type (stored every frame).
// GLOBAL: FF7 0x00e2a110
int g_WorldZolomPrevTerrain = 0;
// GLOBAL: FF7 0x00e2a114
int g_WorldZolomGroundHit = 0;
// GLOBAL: FF7 0x00e2a118
short g_WorldSteeringTarget = 0;
unsigned char g_bssPad_00e2a11c[4] = {0};
// int[3], indexed [i*4 + 0xE2A120] for i < 3; 0xE2A124 and 0xE2A128 are its elements.
// GLOBAL: FF7 0x00e2a120
struct WorldZoneMeshCoords g_WorldZoneMeshCoords = {0};
// World_UpdateZolom's previous player model id (stored every frame).
// GLOBAL: FF7 0x00e2a12c
int g_WorldZolomPrevPlayerModel = 0;
unsigned char g_bssPad_00e2a130[4] = {0};
// GLOBAL: FF7 0x00e2a134
int g_WorldZoneOffsetXZ = 0;
// GLOBAL: FF7 0x00e2a138
int g_WorldRandomTableIndex = 0;
// GLOBAL: FF7 0x00e2a13c
int g_WorldTerrainCurrentX = 0;
// GLOBAL: FF7 0x00e2a140
int g_WorldTerrainCurrentZ = 0;
unsigned char g_bssPad_00e2a144[4] = {0};
// g_WorldZoneAmbientColor is indexed by the code as an array (read by
// World_InitZolomBodySegmentRecords). The span runs to the next named definition of the TU; the
// TU's pieces inside 0xe2a148..0xe2a178 are aliases.
// GLOBAL: FF7 0x00e2a148
unsigned char g_WorldZoneAmbientColor[48] = {0};
// GLOBAL: FF7 0x00e2a178
int g_WorldZoneOuterRadius = 0;
unsigned char g_bssPad_00e2a17c[4] = {0};
// GLOBAL: FF7 0x00e2a180
int g_WorldZoneInnerRadius = 0;
#define g_WorldOverlayChannelMaxLevels (&g_WorldZoneInnerRadius)
// GLOBAL: FF7 0x00e2a184
int g_WorldOverlayChannel1MaxLevel = 0;
// GLOBAL: FF7 0x00e2a188
int g_WorldOverlayChannel2MaxLevel = 0;
// GLOBAL: FF7 0x00e2a18c
WorldZolomTrailPoint *g_pWorldZolomTrailHead = 0;
// GLOBAL: FF7 0x00e2a190
unsigned char g_WorldZoneFogNearRGB[24] = {0};
// GLOBAL: FF7 0x00e2a1a8
unsigned char g_WorldZoneFogFarRGB[456] = {0};
// The world-map sky: three 0x30-byte sprite records (the wrapped strips of wm_kumo.tim) that World_InitSkySprites
// fills and World_UpdateAndDrawSkyLayers draws; the init loop runs from &g_WorldZoneData to &g_WorldSkySpritesEnd.
// GLOBAL: FF7 0x00e2a370
unsigned char g_WorldZoneData[0x90] = {0};
// GLOBAL: FF7 0x00e2a400
int g_WorldSkySpritesEnd = 0;
unsigned char g_bssPad_00e2a404[4] = {0};
unsigned char g_bssPad_00e2a408[136] = {0};
// The Zolom's twelve 48-byte body-segment effect records: World_InitZolomBodySegmentRecords fills them on
// every world-map entry, World_DrawZolomBodySegments walks them up to g_WorldSteeringCycleLimit.
// GLOBAL: FF7 0x00e2a490
WorldShadowQuad g_WorldOverlayChannels[12] = {0};
// GLOBAL: FF7 0x00e2a6d0
short g_WorldSteeringCycleLimit = 0;
unsigned char g_bssPad_00e2a6d4[4] = {0};
// GLOBAL: FF7 0x00e2a6d8
unsigned char g_WorldUnderwaterGrad[3][2] = {0};
// The underwater noise table: 256 ints, WorldMap_NextRandomByte() << 2, written by
// World_InitUnderwaterColorVectorsAndNoise and read by World_BuildUnderwaterNoisePaletteAndGradient.
// GLOBAL: FF7 0x00e2a6e0
int g_WorldUnderwaterPalette[256] = {0};
// GLOBAL: FF7 0x00e2aae0
short g_WorldMeteorSpriteEnabled = 0;
// GLOBAL: FF7 0x00e2aae4
int g_WorldIsMeteorFlagOn = 0;
// GLOBAL: FF7 0x00e2aae8
int g_WorldTerrainFloodVisitedMeshes = 0;
unsigned char g_bssPad_00e2aaec[4] = {0};
unsigned char g_bssPad_00e2aaf0[72] = {0};
// GLOBAL: FF7 0x00e2ab38
int g_WorldZolomPlayerXFlag = 0;
// GLOBAL: FF7 0x00e2ab3c
int g_WorldTerrainFloodInRange = 0;
// int[3], indexed [i*4 + 0xE2AB40] for i < 3; 0xE2AB44 and 0xE2AB48 are its elements.
// GLOBAL: FF7 0x00e2ab40
struct WorldTerrainFloodCrossing g_WorldTerrainFloodCrossing = {0};
unsigned char g_bssPad_00e2ab4c[4] = {0};
// World_InitUnderwaterColorVectorsAndNoise expands the 256 15-bit colours of g_WorldTerrainFloodBuffer1 into 256 vectors
// (vx, vy, vz, pad) here; World_BuildUnderwaterNoisePaletteAndGradient transforms them back into g_WorldTerrainFloodBuffer1.
// GLOBAL: FF7 0x00e2ab50
unsigned char g_WorldTerrainFloodMarks[2048] = {0};
// GLOBAL: FF7 0x00e2b350
unsigned char g_WorldTerrainFloodBuffer1[0x200] = {0};
// The world map's 0x209-byte random table (WorldMap_SeedRandomTable, WorldMap_ReshuffleRandomTable,
// WorldMap_NextRandomByte). The reshuffle reads its lag (table - 0x20, the last 0x20 bytes of
// g_WorldTerrainFloodBuffer1) and its last 0x20 bytes (from g_WorldRandomTable[0x1E9]) through
// WORLD_RANDOM_LAG and WORLD_RANDOM_TAIL (below).
// GLOBAL: FF7 0x00e2b550
unsigned char g_WorldRandomTable[521] = {0};
// GLOBAL: FF7 0x00e2b75c
short g_WorldSteeringTurn = 0;
// GLOBAL: FF7 0x00e2b760
int g_WorldTerrainCurrentByte3 = 0;
// GLOBAL: FF7 0x00e2b764
int g_WorldTerrainFloodState = 0;
// GLOBAL: FF7 0x00e2b768
int g_WorldOverlayChannelTimers = 0;
// GLOBAL: FF7 0x00e2b76c
int g_WorldOverlayChannel1Timer = 0;
// GLOBAL: FF7 0x00e2b770
int g_WorldTerrainEffectKind = 0;
unsigned char g_bssPad_00e2b774[4] = {0};
// The world map's byte colour ramp: 256 BGRA entries.
// World_BuildUnderwaterNoisePaletteAndGradient writes it at init and wm_world_terrain_geometry.cpp's terrain lighting reads it.
// GLOBAL: FF7 0x00e2b778
unsigned char g_WorldTerrainEffectSlots[1024] = {0};
// GLOBAL: FF7 0x00e2bb78
short g_WorldSteeringFacing = 0;
// GLOBAL: FF7 0x00e2bb7c
int g_WorldTerrainEffectActive = 0;
// GLOBAL: FF7 0x00e2bb80
short g_WorldTerrainFloodFinishReq = 0;
// GLOBAL: FF7 0x00e2bb84
int g_WorldInitStepsDone = 0;
// GLOBAL: FF7 0x00e2bb88
int g_WorldSteeringCycle = 0;
// GLOBAL: FF7 0x00e2bb8c
short g_WorldTerrainFloodMeshRow = 0;
// GLOBAL: FF7 0x00e2bb90
struct WorldTerrainProbe *g_pWorldTerrainFloodCursor = 0;
// GLOBAL: FF7 0x00e2bb94
WorldZone *g_pWorldCurrentZone = 0;
// Headings for the three terrain flood-path followers updated by World_UpdateTerrainFloodPathFollowers.
// GLOBAL: FF7 0x00e2bb98
int g_WorldTerrainCurrentBytes[3] = {0};
// GLOBAL: FF7 0x00e2bba4
int g_WorldSkyScrollAngle = 0;
// GLOBAL: FF7 0x00e2bba8
int g_WorldTerrainFloodFinishRequested = 0;
unsigned char g_bssPad_00e2bbac[4] = {0};
// int[3], indexed [i*4 + 0xE2BBB0] for i < 3; 0xE2BBB4 and 0xE2BBB8 are its elements.
// GLOBAL: FF7 0x00e2bbb0
struct WorldTerrainCurrentRecords g_WorldTerrainCurrentRecords = {0};
// GLOBAL: FF7 0x00e2bbbc
short g_WorldZolomRandomOffset = 0;
// GLOBAL: FF7 0x00e2bbc0
int g_WorldScreenFlashArmed = 0;
// GLOBAL: FF7 0x00e2bbc4
int g_WorldScreenFlashStarted = 0;
// GLOBAL: FF7 0x00e2bbc8
int g_WorldFlashSoundActive = 0;
unsigned char g_bssPad_00e2bbcc[4] = {0};
// clang-format on

#pragma intrinsic(memset, strcpy, strcat, strcmp)
typedef struct WorldTerrainProbe {
    WorldTerrainChunkInfo *chunk;
    WorldTerrainTriangle *triangle;
    unsigned char flags;
    unsigned char pad[3];
} WorldTerrainProbe;
// Lag operand at g_WorldRandomTable - 0x20, addressed as a positive offset from the preceding buffer so VC5 /Od
// folds the displacement at compile time instead of emitting runtime address subtraction.
#define WORLD_RANDOM_LAG (*(int *)(g_WorldTerrainFloodBuffer1 + 0x1e0))
#define WORLD_RANDOM_TAIL (*(int *)(g_WorldRandomTable + 0x1e9))

int __cdecl World_TerrainFloodChooseEdgeTowardFacing(WorldTerrainProbe *probe);

/*
 * Arms the world-map screen flash state cleared by World_DisarmScreenFlash.
 */
// FUNCTION: FF7 0x00753860
void __cdecl World_ArmScreenFlash(void)
{
    g_WorldScreenFlashArmed = 1;
    g_WorldScreenFlashStarted = 0;
}

/*
 * Clears the sound state g_WorldScreenFlashArmed.
 */
// FUNCTION: FF7 0x00753879
void __cdecl World_DisarmScreenFlash(void)
{
    g_WorldScreenFlashArmed = 0;
}

/*
 * The mirror of World_StopFlashSoundAndClearScreenFlash below: same two statements, both inverted.
 * 
 * Also missing from the corpus, for the same reason -- its bytes were
 * counted as part of World_DisarmScreenFlash, which ends at 0x00753887.
 */
// FUNCTION: FF7 0x00753888
void __cdecl World_StartFlashSoundAndArmScreenFlash(void)
{
    g_WorldFlashSoundActive = 1;
    World_ArmScreenFlash();
}

/*
 * Clears the sound-playing flag g_WorldFlashSoundActive and then g_WorldScreenFlashArmed.
 */
// FUNCTION: FF7 0x0075389c
void __cdecl World_StopFlashSoundAndClearScreenFlash(void)
{
    g_WorldFlashSoundActive = 0;
    World_DisarmScreenFlash();
}

/*
 * Returns the sound-playing flag g_WorldFlashSoundActive.
 */
// FUNCTION: FF7 0x007538b0
int __cdecl World_IsFlashSoundActive(void)
{
    return g_WorldFlashSoundActive;
}

/*
 * Clears the world map's initialisation bitmask g_WorldInitStepsDone.
 */
// FUNCTION: FF7 0x007538ba
void __cdecl World_ClearInitStepMask(void)
{
    g_WorldInitStepsDone = 0;
}

/*
 * Marks initialisation step step done in the bitmask g_WorldInitStepsDone: returns 1 without change if
 * that bit is the highest set; when the step is higher than anything done so far it runs the
 * transition out of the previous step (World_DisableTerrainStreamingAndReleaseLoadingBlock after step 0, World_ClearLeaderModelLoadLatch after step 2), sets
 * the bit and returns 1; a step lower than the highest done returns 0.
 */
// FUNCTION: FF7 0x007538c9
int __cdecl World_TryEnterInitStep(short step)
{
    if ((g_WorldInitStepsDone & 1 << step) != 0 && (g_WorldInitStepsDone & ~((1 << step + 1) - 1)) == 0) {
        return 1;
    } else if (1 << step > g_WorldInitStepsDone) {
        if (g_WorldInitStepsDone == 1) {
            World_DisableTerrainStreamingAndReleaseLoadingBlock();
        } else if ((g_WorldInitStepsDone & 4) != 0) {
            World_ClearLeaderModelLoadLatch();
        }
        g_WorldInitStepsDone |= 1 << step;
        return 1;
    } else {
        return 0;
    }
}

/*
 * Reports world-script error 0x50 if g_WorldInitStepsDone has a higher bit than value or lacks value's
 * bit, then clears that bit. For nonzero value searches downward for the next set bit and calls
 * World_EnableTerrainStreaming when that bit is 1.
 */
// FUNCTION: FF7 0x0075395e
void __cdecl World_LeaveInitStep(short value)
{
    unsigned bit;

    if ((g_WorldInitStepsDone & ~((1 << value + 1) - 1)) != 0 ||
        (g_WorldInitStepsDone & (1 << value)) == 0) {
        WorldScript_ReportError(0x50);
    }
    g_WorldInitStepsDone &= ~(1 << value);
    if (value != 0) {
        for (bit = 1 << (value - 1); bit != 0 && (g_WorldInitStepsDone & bit) == 0;
             bit >>= 1) {
        }
        if (bit == 1) {
            World_EnableTerrainStreaming();
        }
    }
}

/*
 * Seeds the random-byte state from repeated seed * 0x5D 588B 65 + 1 updates, assembling 17 words
 * from their high bits and extending the word array with a shift/XOR recurrence through index
 * 0x208. Copies each word low byte into the random-byte table, reshuffles it three times and
 * resets the table index to 0x208.
 */
// FUNCTION: FF7 0x00753a01
void __cdecl WorldMap_SeedRandomTable(int seed)
{
    unsigned int accum;
    int wordIdx;
    unsigned int randomWords[521];
    int bit;

    accum = 0;
    for (wordIdx = 0; wordIdx <= 0x10; wordIdx++) {
        for (bit = 0; bit < 0x20; bit++) {
            seed = seed * 1566083941 + 1;
            accum = (accum >> 1) | (seed & 0x80000000);
        }
        randomWords[wordIdx] = accum;
    }
    randomWords[0x10] = (randomWords[0x10] << 23) ^ (randomWords[0] >> 9) ^ randomWords[0xf];
    for (wordIdx = 0x11; wordIdx <= 0x208; wordIdx++) {
        randomWords[wordIdx] = (randomWords[wordIdx - 0x11] << 23) ^ (randomWords[wordIdx - 0x10] >> 9) ^
                       randomWords[wordIdx - 1];
    }
    for (wordIdx = 0; wordIdx < 0x209; wordIdx++) {
        g_WorldRandomTable[wordIdx] = *(unsigned char *)&randomWords[wordIdx];
    }
    WorldMap_ReshuffleRandomTable();
    WorldMap_ReshuffleRandomTable();
    WorldMap_ReshuffleRandomTable();
    g_WorldRandomTableIndex = 520;
}

/*
 * Reshuffles the 0x209-byte world random table at g_WorldRandomTable by XORing its first 0x20 bytes with
 * the bytes at g_WorldRandomTable[0x1E9] and the rest with the bytes 0x20 before it.
 */
// FUNCTION: FF7 0x00753b73
void __cdecl WorldMap_ReshuffleRandomTable(void)
{
    int i;

    for (i = 0; i < 32; i++) {
        *(char *)((char *)&g_WorldRandomTable + i) ^= *(char *)((char *)&WORLD_RANDOM_TAIL + i);
    }
    for (i = 32; i < 521; i++) {
        *(char *)((char *)&g_WorldRandomTable + i) ^= *(char *)((char *)&WORLD_RANDOM_LAG + i);
    }
}

/*
 * Increments the random-table index first. When the incremented index is at least 0x209,
 * reshuffles the table and resets the index to zero; returns the unsigned byte at the resulting
 * index.
 */
// FUNCTION: FF7 0x00753be8
int __cdecl WorldMap_NextRandomByte(void)
{
    g_WorldRandomTableIndex++;
    if (g_WorldRandomTableIndex >= 521) {
        WorldMap_ReshuffleRandomTable();
        g_WorldRandomTableIndex = 0;
    }
    return g_WorldRandomTable[g_WorldRandomTableIndex];
}

/*
 * Returns the sum of absolute Y difference and the X/Z differences folded once across map extents
 * 0x48000/0x38000 when at least half their extent. This gives wrapped Manhattan distance for
 * positions within those extents; differences greater than an extent become negative after the
 * single fold.
 */
// FUNCTION: FF7 0x00753c23
int __cdecl WorldMap_WrappedDistance(WorldPoint *pointA, WorldPoint *pointB)
{
    int absX;
    int absY;
    int absZ;
    int absDx;
    int absDy;
    int absDz;

    if (pointA->posX - pointB->posX > 0) {
        absDx = pointA->posX - pointB->posX;
    } else {
        absDx = -(pointA->posX - pointB->posX);
    }
    absX = absDx;
    if (absX >= 147456) {
        absX = 0x48000 - absX;
    }
    if (pointA->posY - pointB->posY > 0) {
        absDy = pointA->posY - pointB->posY;
    } else {
        absDy = -(pointA->posY - pointB->posY);
    }
    absY = absDy;
    if (pointA->posZ - pointB->posZ > 0) {
        absDz = pointA->posZ - pointB->posZ;
    } else {
        absDz = -(pointA->posZ - pointB->posZ);
    }
    absZ = absDz;
    if (absZ >= 114688) {
        absZ = 0x38000 - absZ;
    }
    return absX + absY + absZ;
}

/*
 * For a nonnull vector, selects a zero-translation matrix with the supplied yaw, transforms the
 * vector through RotTransPoint and writes the transformed XYZ values back as shorts. Also updates
 * the current rotation and translation matrices.
 */
// FUNCTION: FF7 0x00753d00
void __cdecl WorldMap_RotateVec3ByYaw(SVECTOR *vec, short yaw)
{
    char mtx[32];
    int ignored;
    SVECTOR yawVec;
    int s;
    int translation[3];

    if (vec) {
        translation[0] = translation[1] = translation[2] = 0;
        yawVec.vx = yawVec.vz = 0;
        yawVec.vy = yaw;
        RotMatrixXYZ(&yawVec, &mtx);
        SetRotMatrix(&mtx);
        SetMatrixTranslation(&mtx, &translation);
        SetTransMatrix(&mtx);
        RotTransPoint((void *)vec, (int *)&translation, &ignored);
        vec->vx = *(short *)&translation[0];
        vec->vy = *(short *)&translation[1];
        vec->vz = *(short *)&translation[2];
    }
}

/*
 * Returns zero for two zero inputs; otherwise selects an octant from the signs and relative
 * magnitudes of dx and dz, indexes the short angle table at g_WorldArcTangentTable with the smaller
 * magnitude shifted left by 8 and divided by the larger, and applies offsets of 0x400 or 0x800 and
 * sign changes. Returns the resulting angle as a signed short. The body does not establish a
 * conventional atan2 argument orientation or guard extreme signed arithmetic.
 */
// FUNCTION: FF7 0x00753da9
short __cdecl WorldMap_ArcTangentFromDeltas(int dx, int dz)
{
    unsigned short unused;
    int octantAngle;
    unsigned short angle;
    int negAngle;
    int shiftedAngle;
    int ofsAngle;

    angle = 0;
    unused = 0;
    if (dx != 0 || dz != 0) {
        if (dz >= 0) {
            if (dx >= 0) {
                if (dx >= dz) {
                    octantAngle = 0x400 - g_WorldArcTangentTable[(dz << 8) / dx];
                } else {
                    octantAngle = g_WorldArcTangentTable[(dx << 8) / dz];
                }
                angle = octantAngle;
            } else {
                if (-dx <= dz) {
                    negAngle = -g_WorldArcTangentTable[(-dx << 8) / dz];
                } else {
                    negAngle = g_WorldArcTangentTable[(dz << 8) / -dx] - 0x400;
                }
                angle = negAngle;
            }
        } else if (dx < 0) {
            if (-dx >= -dz) {
                ofsAngle = -g_WorldArcTangentTable[(-dz << 8) / -dx] - 0x400;
            } else {
                ofsAngle = g_WorldArcTangentTable[(-dx << 8) / -dz] - 0x800;
            }
            angle = ofsAngle;
        } else {
            if (dx <= -dz) {
                shiftedAngle = 0x800 - g_WorldArcTangentTable[(dx << 8) / -dz];
            } else {
                shiftedAngle = g_WorldArcTangentTable[(-dz << 8) / dx] + 0x400;
            }
            angle = shiftedAngle;
        }
    }
    return (short)angle;
}

/*
 * Returns WorldMap_ArcTangentFromDeltas of the x and z differences from from to to.
 */
// FUNCTION: FF7 0x00753f2e
short __cdecl WorldMap_AngleBetweenPositions(void *from, void *to)
{
    return WorldMap_ArcTangentFromDeltas(((WorldPoint *)to)->posX - ((WorldPoint *)from)->posX, ((WorldPoint *)to)->posZ - ((WorldPoint *)from)->posZ);
}

/*
 * Copies the two stored RGB triplets into the upper and lower sky-polygon colors and derives their
 * packed colors with alpha 0x80. Sets the right edge from g_WorldRenderWidth and lower edge from
 * g_WorldDisplayScale times 180, then clears g_WorldZoneOffsetXZ.
 */
// FUNCTION: FF7 0x00753f53
void __cdecl World_ResetSkyGradientPolygon(void)
{
    struct {
        int _ocal_2;
        struct t_wm_SkyPoly *skyPoly;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.skyPoly = &g_WorldSkyPoly;

    frame.skyPoly->polyG4.r0 = frame.skyPoly->polyG4.r1 = g_WorldSkyColor1[0];
    frame.skyPoly->polyG4.g0 = frame.skyPoly->polyG4.g1 = g_WorldSkyColor1[1];
    frame.skyPoly->polyG4.b0 = frame.skyPoly->polyG4.b1 = g_WorldSkyColor1[2];

    frame.skyPoly->polyG4.r2 = frame.skyPoly->polyG4.r3 = g_WorldSkyColor2[0];
    frame.skyPoly->polyG4.g2 = frame.skyPoly->polyG4.g3 = g_WorldSkyColor2[1];
    frame.skyPoly->polyG4.b2 = frame.skyPoly->polyG4.b3 = g_WorldSkyColor2[2];

    frame.skyPoly->bgra0 =
        frame.skyPoly->bgra1 = (g_WorldSkyColor1[0] << 16) | (g_WorldSkyColor1[1] << 8) | g_WorldSkyColor1[2];
    frame.skyPoly->bgra2 =
        frame.skyPoly->bgra3 = (g_WorldSkyColor2[0] << 16) | (g_WorldSkyColor2[1] << 8) | g_WorldSkyColor2[2];
    frame.skyPoly->bgra0 |= 0x80 << 24;
    frame.skyPoly->bgra1 |= 0x80 << 24;
    frame.skyPoly->bgra2 |= 0x80 << 24;
    frame.skyPoly->bgra3 |= 0x80 << 24;

    frame.skyPoly->polyG4.x1 = frame.skyPoly->polyG4.x3 = g_WorldRenderWidth;
    frame.skyPoly->polyG4.y2 = frame.skyPoly->polyG4.y3 = g_WorldDisplayScale * 180;
    g_WorldZoneOffsetXZ = 0;
}

/*
 * Returns the address of the world's screen rectangle record, g_WorldSkyPoly.
 */
// FUNCTION: FF7 0x007540e5
int __cdecl World_GetSkyPolygon(void)
{
    return (int)&g_WorldSkyPoly;
}

/*
 * Always 0.
 */
// FUNCTION: FF7 0x007540ef
int __cdecl World_UnusedReturnZero(void)
{
    return 0;
}

/*
 * Returns g_WorldZoneOffsetXZ.
 */
// FUNCTION: FF7 0x007540f6
int __cdecl World_GetSkyHorizonY(void)
{
    return g_WorldZoneOffsetXZ;
}

/*
 * Rotates a view vector containing negative g_WorldCameraDeltaY and -0x4000 by negative yaw, installs
 * its view transform, and projects the origin through the screen-composed camera matrix. Divides Y
 * by projected W times the global scale, truncates and adds 26, storing g_WorldZoneOffsetXZ. Installs a
 * camera-roll rotation with translation centered at viewport X+160 and that Y. Transforms four
 * local rectangle corners at X plus/minus 180 and Y from -nn-24 to 0, scales their XY by
 * g_WorldDisplayScale and writes the four short pairs into g_WorldSkyPoly. Leaves installed matrix state
 * changed and does not guard the projection denominator.
 */
// FUNCTION: FF7 0x00754100
void __cdecl World_UpdateSkyHorizonPolygon(short yaw)
{
    short av[4];
    int iv[3];
    float xyz[4];
    float mcam[16];
    short *pq;
    float projectedY;
    int sf;
    float mp[16];
    float recipWidth;
    short svc[4];
    short mr[16];
    int nn;
    int tv[4];

    pq = (short *)&g_WorldSkyPoly;
    svc[0] = 0;
    svc[1] = (short)-g_WorldCameraDeltaY;
    svc[2] = -0x4000;
    WorldMap_RotateVec3ByYaw((SVECTOR *)svc, -yaw);
    World_SetViewTransformForPosition((SVECTOR *)svc);
    iv[0] = iv[1] = iv[2] = 0;
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), mcam);
    ComposeWithScreenMatrix(mcam, mp);
    TransformPointByMatrix4x4(mp, iv, xyz);
    recipWidth = 1.0f / (xyz[3] * g_WorldUiScaleFactor);
    nn = (int)(projectedY = xyz[1] * recipWidth) + 0x1A;
    g_WorldZoneOffsetXZ = nn;
    av[0] = av[1] = 0;
    av[2] = (short)World_GetCameraRoll();
    RotMatrixXYZ(av, mr);
    SetRotMatrix(mr);
    tv[0] = g_WorldViewportX + 0xA0;
    tv[1] = nn;
    tv[2] = 0;
    SetMatrixTranslation(mr, tv);
    SetTransMatrix(mr);
    svc[2] = 0;
    svc[0] = -0xB4;
    svc[1] = (short)(-nn - 0x18);
    RotTransPoint(svc, tv, &sf);
    pq[4] = (short)(tv[0] * g_WorldDisplayScale);
    pq[5] = (short)(tv[1] * g_WorldDisplayScale);
    svc[0] = 0xB4;
    svc[1] = (short)(-nn - 0x18);
    RotTransPoint(svc, tv, &sf);
    pq[8] = (short)(tv[0] * g_WorldDisplayScale);
    pq[9] = (short)(tv[1] * g_WorldDisplayScale);
    svc[0] = -0xB4;
    svc[1] = 0;
    RotTransPoint(svc, tv, &sf);
    pq[12] = (short)(tv[0] * g_WorldDisplayScale);
    pq[13] = (short)(tv[1] * g_WorldDisplayScale);
    svc[0] = 0xB4;
    svc[1] = 0;
    RotTransPoint(svc, tv, &sf);
    pq[16] = (short)(tv[0] * g_WorldDisplayScale);
    pq[17] = (short)(tv[1] * g_WorldDisplayScale);
}

// Resets the three 0x30-byte cloud sky sprites in g_WorldZoneData. Each gets b, g and r set to
// 0x40, color set to 0x807F7F7F (BGRA), the top texture rows v0 and v1 cleared and the bottom rows
// v2 and v3 set to 0x3F, and renderObject set to g_WorldCloudRenderObject. The meteor sprite at
// g_WorldSkySprite gets renderObject set to g_WorldMeteorRenderObject. Finally
// g_WorldIsMeteorFlagOn is cleared.
// FUNCTION: FF7 0x007543d3
void __cdecl World_InitSkySprites(void)
{
    int group;
    WorldSkySprite *sprite;

    for (group = 0; group < 1; group++) {
        for (sprite = (WorldSkySprite *)(group * 0x90 + (char *)&g_WorldZoneData); sprite < (WorldSkySprite *)(group * 0x90 + (char *)&g_WorldSkySpritesEnd); sprite++) {
            sprite->b = 0x40;
            sprite->g = 0x40;
            sprite->r = 0x40;
            sprite->color = 0x807F7F7F;
            sprite->v1 = 0;
            sprite->v0 = 0;
            sprite->v3 = 0x3F;
            sprite->v2 = 0x3F;
            sprite->renderObject = (GfxRenderObject *)g_WorldCloudRenderObject;
        }
        sprite = (WorldSkySprite *)&g_WorldSkySprite;
        sprite->renderObject = (GfxRenderObject *)g_WorldMeteorRenderObject;
    }
    g_WorldIsMeteorFlagOn = 0;
}

/*
 * Returns for a null render object (renderObject). Computes UV endpoints from four byte texel coordinates
 * and the batch texture scales, then multiplies all eight corner shorts in place by the low signed
 * short at g_WorldDisplayScale. These coordinate changes occur even when acquisition fails or terrain
 * rendering is disabled, and repeated calls rescale again. On successful acquisition with terrain
 * enabled, writes four 32-byte vertices from the modified corners, depth 0.999, reciprocal W 1,
 * caller color and black specular with the computed UV pairs.
 */
// FUNCTION: FF7 0x00754493
void __cdecl World_DrawSkySpriteQuad(WorldSkySprite *quad)
{
    float uv[4];
    struct GfxTlVertex *p0;
    struct GfxTlVertex *p1;
    struct GfxTlVertex *p2;
    struct GfxTlVertex *p3;

    if (quad->renderObject == 0) {
        return;
    }
    uv[2] = quad->u0 * ((GfxRenderObject *)quad->renderObject)->invWidth;
    uv[1] = quad->u1 * ((GfxRenderObject *)quad->renderObject)->invWidth;
    uv[0] = quad->v0 * ((GfxRenderObject *)quad->renderObject)->invHeight;
    uv[3] = quad->v2 * ((GfxRenderObject *)quad->renderObject)->invHeight;
    quad->x0 *= *(short *)&g_WorldDisplayScale;
    quad->y0 *= *(short *)&g_WorldDisplayScale;
    quad->x1 *= *(short *)&g_WorldDisplayScale;
    quad->y1 *= *(short *)&g_WorldDisplayScale;
    quad->x2 *= *(short *)&g_WorldDisplayScale;
    quad->y2 *= *(short *)&g_WorldDisplayScale;
    quad->x3 *= *(short *)&g_WorldDisplayScale;
    quad->y3 *= *(short *)&g_WorldDisplayScale;
    if (AcquireBatchNode(1, (void *)quad->renderObject) != 0 && World_IsTerrainRenderEnabled() != 0) {
        p0 = (struct GfxTlVertex *)(((GfxRenderObject *)quad->renderObject)->vertexCursor);
        p0->x = quad->x0;
        p0->y = quad->y0;
        p0->z = 0.999f;
        p0->rhw = 1.0f;
        p0->color = quad->color;
        p0->specular = (int)0xFF000000;
        p0->u = uv[2];
        p0->v = uv[0];
        p1 = (struct GfxTlVertex *)(((GfxRenderObject *)quad->renderObject)->vertexCursor + 0x20);
        p1->x = quad->x1;
        p1->y = quad->y1;
        p1->z = 0.999f;
        p1->rhw = 1.0f;
        p1->color = quad->color;
        p1->specular = (int)0xFF000000;
        p1->u = uv[1];
        p1->v = uv[0];
        p2 = (struct GfxTlVertex *)(((GfxRenderObject *)quad->renderObject)->vertexCursor + 0x40);
        p2->x = quad->x2;
        p2->y = quad->y2;
        p2->z = 0.999f;
        p2->rhw = 1.0f;
        p2->color = quad->color;
        p2->specular = (int)0xFF000000;
        p2->u = uv[2];
        p2->v = uv[3];
        p3 = (struct GfxTlVertex *)(((GfxRenderObject *)quad->renderObject)->vertexCursor + 0x60);
        p3->x = quad->x3;
        p3->y = quad->y3;
        p3->z = 0.999f;
        p3->rhw = 1.0f;
        p3->color = quad->color;
        p3->specular = (int)0xFF000000;
        p3->u = uv[1];
        p3->v = uv[3];
    }
}

/*
 * When the world rendering context exists, computes sky vertical position from the fade distance,
 * substitutes distance 1 for zero, unwraps the supplied angle against the prior angle and updates
 * a scrolling byte-range offset with one wrap adjustment. Builds twelve screen points, rotates
 * them by camera roll and translates them to the viewport. Emits two or three wrapped sky strips
 * according to whether the offset is below 192, then emits the meteor sprite when enabled and its
 * transformed horizontal extent overlaps the viewport. Preserves the integer angle/scroll
 * feedback, 12-bit angle mask, native matrix layout and partially assigned point records. The
 * initially fetched buffer pointer is unused.
 */
// FUNCTION: FF7 0x007547a6
void __cdecl World_UpdateAndDrawSkyLayers(short angle)
{
    struct {
        WorldSkySprite *meteor;
        VECTOR transformed[12];
        PsxMatrix32 matrix;
        int distance, i, buffer, top;
        SVECTOR points[12];
        int context, flags;
        SVECTOR rotation;
        int scroll;
        WorldSkySprite *sprite;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.buffer = World_GetRenderBufferIndex();
    s.sprite = (WorldSkySprite *)&g_WorldZoneData;
    s.context = World_GetRenderTableEntryIfActive();
    if (s.context) {
        s.distance = World_GetFadeTarget();
        if (s.distance == 0)
            s.distance = 1;
        s.top = (World_GetSkyHorizonY() - 44) - (s.distance >> 8);
        if (angle - g_WorldSkyScrollAngle > 0x800)
            angle -= 0x1000 /* 360 deg */;
        else if (g_WorldSkyScrollAngle - angle > 0x800)
            angle += 0x1000 /* 360 deg */;
        s.scroll = ((angle - g_WorldSkyScrollAngle) * s.distance) / 10000;
        g_WorldFlashState -= s.scroll;
        if (g_WorldFlashState < 0)
            g_WorldFlashState += 256;
        else if (g_WorldFlashState > 255)
            g_WorldFlashState -= 256;
        g_WorldSkyScrollAngle += (s.scroll * 10000) / s.distance;
        g_WorldSkyScrollAngle &= 0xfff;
        s.points[0].vx = s.points[1].vx = -160;
        s.points[2].vx = s.points[3].vx = g_WorldFlashState - 352;
        s.points[4].vx = s.points[5].vx = g_WorldFlashState - 96;
        s.points[6].vx = s.points[7].vx = 160;
        s.points[0].vy = s.points[2].vy = s.points[4].vy = s.points[6].vy = s.top - 64;
        s.points[1].vy = s.points[3].vy = s.points[5].vy = s.points[7].vy = s.top;
        s.points[8].vx = s.points[9].vx = (((-angle << 20) >> 20) * (s.distance - 500)) / 10000 - 72;
        s.points[10].vx = s.points[11].vx = s.points[8].vx + 144;
        s.points[8].vy = s.points[10].vy = s.top - 112;
        s.points[9].vy = s.points[11].vy = s.top - 48;
        s.rotation.vx = s.rotation.vy = 0;
        s.rotation.vz = World_GetCameraRoll();
        RotMatrixXYZ(&s.rotation, &s.matrix);
        SetRotMatrix(&s.matrix);
        s.matrix.t[0] = g_WorldViewportX + 160;
        s.matrix.t[1] = 40;
        SetTransMatrix(&s.matrix);
        for (s.i = 0; s.i < 12; s.i++)
            RotTransPoint(&s.points[s.i], (int *)&s.transformed[s.i], &s.flags);
        if (g_WorldFlashState < 192) {
            s.sprite->x0 = s.transformed[0].vx;
            s.sprite->x2 = s.transformed[1].vx;
            s.sprite->x1 = s.transformed[4].vx;
            s.sprite->x3 = s.transformed[5].vx;
            s.sprite->y0 = s.transformed[0].vy;
            s.sprite->y2 = s.transformed[1].vy;
            s.sprite->y1 = s.transformed[4].vy;
            s.sprite->y3 = s.transformed[5].vy;
            s.sprite->u0 = s.sprite->u2 = 192 - g_WorldFlashState;
            s.sprite->u3 = 255;
            s.sprite->u1 = 255;
            World_DrawSkySpriteQuad(s.sprite);
        } else {
            s.sprite->x0 = s.transformed[0].vx;
            s.sprite->x2 = s.transformed[1].vx;
            s.sprite->x1 = s.transformed[2].vx;
            s.sprite->x3 = s.transformed[3].vx;
            s.sprite->y0 = s.transformed[0].vy;
            s.sprite->y2 = s.transformed[1].vy;
            s.sprite->y1 = s.transformed[2].vy;
            s.sprite->y3 = s.transformed[3].vy;
            s.sprite->u0 = s.sprite->u2 = 192 + (255 - g_WorldFlashState);
            s.sprite->u3 = 255;
            s.sprite->u1 = 255;
            World_DrawSkySpriteQuad(s.sprite);
            s.sprite++;
            s.sprite->x0 = s.transformed[2].vx;
            s.sprite->x2 = s.transformed[3].vx;
            s.sprite->x1 = s.transformed[4].vx;
            s.sprite->x3 = s.transformed[5].vx;
            s.sprite->y0 = s.transformed[2].vy;
            s.sprite->y2 = s.transformed[3].vy;
            s.sprite->y1 = s.transformed[4].vy;
            s.sprite->y3 = s.transformed[5].vy;
            s.sprite->u2 = 0;
            s.sprite->u0 = 0;
            s.sprite->u3 = 255;
            s.sprite->u1 = 255;
            World_DrawSkySpriteQuad(s.sprite);
        }
        s.sprite++;
        s.sprite->x0 = s.transformed[4].vx;
        s.sprite->x2 = s.transformed[5].vx;
        s.sprite->x1 = s.transformed[6].vx;
        s.sprite->x3 = s.transformed[7].vx;
        s.sprite->y0 = s.transformed[4].vy;
        s.sprite->y2 = s.transformed[5].vy;
        s.sprite->y1 = s.transformed[6].vy;
        s.sprite->y3 = s.transformed[7].vy;
        s.sprite->u2 = 0;
        s.sprite->u0 = 0;
        s.sprite->u1 = s.sprite->u3 = 255 - g_WorldFlashState;
        World_DrawSkySpriteQuad(s.sprite);
        if (g_WorldIsMeteorFlagOn && s.transformed[10].vx > g_WorldViewportX && s.transformed[8].vx < g_WorldViewportY + 320) {
            s.meteor = (WorldSkySprite *)&g_WorldSkySprite;
            s.meteor->x0 = s.transformed[8].vx;
            s.meteor->y0 = s.transformed[8].vy;
            s.meteor->x2 = s.transformed[9].vx;
            s.meteor->y2 = s.transformed[9].vy;
            s.meteor->x1 = s.transformed[10].vx;
            s.meteor->y1 = s.transformed[10].vy;
            s.meteor->x3 = s.transformed[11].vx;
            s.meteor->y3 = s.transformed[11].vy;
            World_DrawSkySpriteQuad(s.meteor);
        }
    }
}

/*
 * Sets g_WorldIsMeteorFlagOn.
 */
// FUNCTION: FF7 0x00754e10
void __cdecl World_SetMeteorSpriteEnabled(int value)
{
    g_WorldIsMeteorFlagOn = value;
}

/*
 * Clears the enabled byte and posY of the 16 world zone records in g_WorldZolomTrail, drops the
 * current zone pointer g_pWorldCurrentZone, and applies the default colours
 * through World_ApplyDefaultMapLighting.
 */
// FUNCTION: FF7 0x00754e1d
void __cdecl World_ResetZonesAndDefaultLighting(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        g_WorldZolomTrail.zones[i].enabled = 0;
        g_WorldZolomTrail.zones[i].posY = 0;
    }
    g_pWorldCurrentZone = 0;
    World_ApplyDefaultMapLighting();
}

/*
 * Applies the map's default lighting: the secondary colour is (0x20,0x20,0x30) on map variant 2
 * (the underwater map) and (0x40,0x40,0x40) otherwise, the draw colour black, and the light preset
 * is 1 for variant 2 and 0 for the rest.
 */
// FUNCTION: FF7 0x00754e6b
void __cdecl World_ApplyDefaultMapLighting(void)
{
    if (World_GetMapVariant() == 2) {
        SetSecondaryColorRGB(0x20, 0x20, 0x30);
    } else {
        SetSecondaryColorRGB(0x40, 0x40, 0x40);
    }
    SetDrawColorRGB(0, 0, 0);
    World_SetPrimaryLightGroupPreset(World_GetMapVariant() == 2);
}

/*
 * Selects world zone zoneIndex (0..15) as the current one (g_pWorldCurrentZone) and, when value is not -1,
 * stores it in the zone's enabled byte.
 */
// FUNCTION: FF7 0x00754ebc
void __cdecl WorldZone_Select(int zoneIndex, int value)
{
    if (zoneIndex >= 0 && zoneIndex < 16) {
        g_pWorldCurrentZone = &g_WorldZolomTrail.zones[zoneIndex];
        if (value != -1) {
            g_pWorldCurrentZone->enabled = value;
        }
    }
}

/*
 * Sets the current zone's mesh column (0..0x23) and row (0..0x1B) in the upper bits of its x and z
 * words, keeping the 13-bit offsets.
 */
// FUNCTION: FF7 0x00754eef
void __cdecl WorldZone_SetMeshXZ(int meshColumn, int meshRow)
{
    if (g_pWorldCurrentZone != 0 && meshColumn >= 0 && meshColumn < 36 && meshRow >= 0 && meshRow < 28) {
        g_pWorldCurrentZone->posX &= 0x1FFF;
        g_pWorldCurrentZone->posZ &= 0x1FFF;
        g_pWorldCurrentZone->posX |= meshColumn << 13;
        g_pWorldCurrentZone->posZ |= meshRow << 13;
    }
}

/*
 * Sets the current zone's 13-bit x and z offsets within its mesh, keeping the mesh indices in the
 * upper bits.
 */
// FUNCTION: FF7 0x00754f72
void __cdecl WorldZone_SetOffsetXZ(int offsetX, int offsetZ)
{
    if (g_pWorldCurrentZone != 0) {
        g_pWorldCurrentZone->posX &= 0xFFFFE000;
        g_pWorldCurrentZone->posZ &= 0xFFFFE000;
        g_pWorldCurrentZone->posX |= offsetX & 0x1FFF;
        g_pWorldCurrentZone->posZ |= offsetZ & 0x1FFF;
    }
}

/*
 * Sets the current zone's ambient colour bytes.
 */
// FUNCTION: FF7 0x00754fe2
void __cdecl WorldZone_SetAmbientRGB(int red, int green, int blue)
{
    if (g_pWorldCurrentZone != 0) {
        g_pWorldCurrentZone->ambient[0] = red;
        g_pWorldCurrentZone->ambient[1] = green;
        g_pWorldCurrentZone->ambient[2] = blue;
    }
}

/*
 * Sets the current zone's outer radius.
 */
// FUNCTION: FF7 0x00755013
void __cdecl WorldZone_SetOuterRadius(int value)
{
    if (g_pWorldCurrentZone != 0) {
        g_pWorldCurrentZone->outerRadius = value;
    }
}

/*
 * Sets the current zone's inner radius.
 */
// FUNCTION: FF7 0x0075502c
void __cdecl WorldZone_SetInnerRadius(int value)
{
    if (g_pWorldCurrentZone != 0) {
        g_pWorldCurrentZone->innerRadius = value;
    }
}

/*
 * Sets the current zone's near fog colour bytes.
 */
// FUNCTION: FF7 0x00755045
void __cdecl WorldZone_SetFogNearRGB(int red, int green, int blue)
{
    if (g_pWorldCurrentZone != 0) {
        g_pWorldCurrentZone->nearColor[0] = red;
        g_pWorldCurrentZone->nearColor[1] = green;
        g_pWorldCurrentZone->nearColor[2] = blue;
    }
}

/*
 * Sets the current zone's far fog colour bytes.
 */
// FUNCTION: FF7 0x00755076
void __cdecl WorldZone_SetFogFarRGB(int red, int green, int blue)
{
    if (g_pWorldCurrentZone != 0) {
        g_pWorldCurrentZone->farColor[0] = red;
        g_pWorldCurrentZone->farColor[1] = green;
        g_pWorldCurrentZone->farColor[2] = blue;
    }
}

/*
 * Scans sixteen enabled world lighting zones. For each position within its outer band after
 * subtracting inner radius, interpolates three ambient matrix components and near and far sky
 * colors against default values, applies lighting mode 2 and writes sky vertex colors with alpha
 * 128. Integer divisions are performed separately for ambient terms and color channels are capped
 * at 255. Multiple matching zones overwrite earlier results, so the last matching zone wins. With
 * no match, selects lighting mode 3 and restores default sky state; unused inner-hit and map-index
 * locals have no effect.
 */
// FUNCTION: FF7 0x007550a7
void __cdecl WorldZone_ApplyToCamera(WorldPoint *position)
{
    union Color {
        struct {
            unsigned char b, g, r, a;
        } c;
        unsigned int value;
    };
    struct {
        union Color nearColor;
        PsxMatrix32 matrix;
        int outer, i, unusedIndex, innerHit;
        unsigned int green;
        int found, distance;
        struct t_wm_SkyPoly *sky;
        unsigned int red, blue;
        union Color farColor;
        WorldZone *zone;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.found = 0;
    s.innerHit = 0;
    s.unusedIndex = World_GetRenderBufferIndex();
    s.sky = &g_WorldSkyPoly;
    for (s.i = 0; s.i < 16; s.i++) {
        s.zone = &g_WorldZolomTrail.zones[s.i];
        if (s.zone->enabled) {
            s.distance = WorldMap_WrappedDistance(position, (WorldPoint *)&s.zone->posX) - (s.zone->innerRadius << 10);
            if (s.distance < 0) {
                s.distance = 0;
                s.innerHit = 1;
            }
            s.outer = s.zone->outerRadius << 10;
            if (s.distance < s.outer) {
                s.found = 1;
                s.matrix.m[0][0] = (s.distance * *(short *)&g_WorldUnderwaterColor) / s.outer + ((s.outer - s.distance) * s.zone->ambient[0] * 7) / s.outer;
                s.matrix.m[1][0] = (s.distance * *(short *)((char *)&g_WorldUnderwaterColor + 6)) / s.outer + ((s.outer - s.distance) * s.zone->ambient[1] * 7) / s.outer;
                s.matrix.m[2][0] = (s.distance * *(short *)((char *)&g_WorldUnderwaterColor + 12)) / s.outer + ((s.outer - s.distance) * s.zone->ambient[2] * 7) / s.outer;
                World_SetAmbientColorFromMatrix((struct WorldAmbientMatrix *)&s.matrix);
                World_SetPrimaryLightGroupPreset(2);
                s.red = (s.distance * g_WorldSkyColor1[0] + (s.outer - s.distance) * s.zone->nearColor[0]) / s.outer;
                if (s.red > 255)
                    s.red = 255;
                s.nearColor.c.r = s.red;
                s.sky->polyG4.r1 = s.nearColor.c.r;
                s.sky->polyG4.r0 = s.nearColor.c.r;
                s.green = (s.distance * g_WorldSkyColor1[1] + (s.outer - s.distance) * s.zone->nearColor[1]) / s.outer;
                if (s.green > 255)
                    s.green = 255;
                s.nearColor.c.g = s.green;
                s.sky->polyG4.g1 = s.nearColor.c.g;
                s.sky->polyG4.g0 = s.nearColor.c.g;
                s.blue = (s.distance * g_WorldSkyColor1[2] + (s.outer - s.distance) * s.zone->nearColor[2]) / s.outer;
                if (s.blue > 255)
                    s.blue = 255;
                s.nearColor.c.b = s.blue;
                s.sky->polyG4.b1 = s.nearColor.c.b;
                s.sky->polyG4.b0 = s.nearColor.c.b;
                s.red = (s.distance * g_WorldSkyColor2[0] + (s.outer - s.distance) * s.zone->farColor[0]) / s.outer;
                if (s.red > 255)
                    s.red = 255;
                s.farColor.c.r = s.red;
                s.sky->polyG4.r3 = s.farColor.c.r;
                s.sky->polyG4.r2 = s.farColor.c.r;
                s.green = (s.distance * g_WorldSkyColor2[1] + (s.outer - s.distance) * s.zone->farColor[1]) / s.outer;
                if (s.green > 255)
                    s.green = 255;
                s.farColor.c.g = s.green;
                s.sky->polyG4.g3 = s.farColor.c.g;
                s.sky->polyG4.g2 = s.farColor.c.g;
                s.blue = (s.distance * g_WorldSkyColor2[2] + (s.outer - s.distance) * s.zone->farColor[2]) / s.outer;
                if (s.blue > 255)
                    s.blue = 255;
                s.farColor.c.b = s.blue;
                s.sky->polyG4.b3 = s.farColor.c.b;
                s.sky->polyG4.b2 = s.farColor.c.b;
                s.nearColor.c.a = s.farColor.c.a = 128;
                s.sky->bgra0 = s.sky->bgra1 = s.nearColor.value;
                s.sky->bgra2 = s.sky->bgra3 = s.farColor.value;
            }
        }
    }
    if (!s.found) {
        World_SetPrimaryLightGroupPreset(3);
        World_ResetSkyGradientPolygon();
    }
}

/*
 * Wrapped Manhattan distance from position position to the current zone's position, or 0 with no
 * current zone.
 */
// FUNCTION: FF7 0x0075542d
int __cdecl WorldZone_DistanceFrom(WorldPoint *position)
{
    int distance;

    if (g_pWorldCurrentZone != 0) {
        distance = WorldMap_WrappedDistance(position, (WorldPoint *)g_pWorldCurrentZone);
    } else {
        distance = 0;
    }
    return distance;
}

/*
 * Returns WorldMap_AngleBetweenPositions(position,g_pWorldCurrentZone) when the target pointer is
 * nonzero, otherwise returns 0.
 */
// FUNCTION: FF7 0x0075545f
short __cdecl WorldZone_AngleFrom(void *position)
{
    return g_pWorldCurrentZone ? WorldMap_AngleBetweenPositions(position, g_pWorldCurrentZone) : 0;
}

/*
 * Resets the world fade state: the three fade steps g_WorldZoneMeshCoords/24/28 and the three
 * channel timers g_WorldOverlayChannelTimers, g_WorldOverlayChannel1Timer and g_WorldTerrainEffectKind to 0, the brightness g_WorldTerrainCurrentRecords to
 * 0xFF with g_WorldTerrainCurrentRecords.secondaryBrightness/b8 cleared, and the channels' maximum levels
 * g_WorldZoneInnerRadius = 0xFF, g_WorldOverlayChannel1MaxLevel = g_WorldOverlayChannel2MaxLevel = 0x80.
 */
// FUNCTION: FF7 0x00755495
void __cdecl World_ResetOverlayFadeState(void)
{
    g_WorldZoneMeshCoords.fadeStepSource = 0;
    g_WorldZoneMeshCoords.pendingFadeStep = g_WorldZoneMeshCoords.fadeStepSource;
    (g_WorldZoneMeshCoords.fadeStep) = g_WorldZoneMeshCoords.pendingFadeStep;
    g_WorldTerrainEffectKind = 0;
    g_WorldOverlayChannel1Timer = g_WorldTerrainEffectKind;
    g_WorldOverlayChannelTimers = g_WorldOverlayChannel1Timer;
    (g_WorldTerrainCurrentRecords.brightness) = 0xFF;
    g_WorldTerrainCurrentRecords.pendingBrightness = 0;
    g_WorldTerrainCurrentRecords.secondaryBrightness = g_WorldTerrainCurrentRecords.pendingBrightness;
    g_WorldZoneInnerRadius = 0xFF;
    g_WorldOverlayChannel2MaxLevel = 0x80;
    g_WorldOverlayChannel1MaxLevel = g_WorldOverlayChannel2MaxLevel;
}

/*
 * Updates and draws three world overlay channels. Captures each current intensity into color bytes
 * before advancing/clamping its value; channel 0 uses intensity-dependent increments while
 * channels 1 and 2 use fixed increments. Acquires a full-viewport quad when the updated intensity
 * is nonzero, but fills it only when terrain rendering is enabled. Channels 1/2 use scrolling
 * texture UVs and force alpha 128; channel 0 draws black with prior alpha or a two-step grayscale
 * flash that resets its flags on the second successful render. Countdown expiration invokes
 * World_BeginOverlayChannelFadeDown independently of drawing. Finally updates texture scroll offsets from sine/cosine
 * and remaining counters. Acquisition occurs before the terrain gate, and the first channel does
 * not assign UVs. The fetched buffer pointer is unused.
 */
// FUNCTION: FF7 0x0075551a
void __cdecl World_UpdateAndDrawScreenOverlays(void)
{
    union Color {
        struct {
            unsigned char b, g, r, a;
        } c;
        unsigned int value;
    };
    struct {
        GfxTlVertex *fade[4], *snow[4];
        float height, width, texV, texU;
        int layerIdx, buffer;
        union Color color;
        int originX, originY;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.buffer = World_GetRenderBufferIndex();
    s.originX = g_WorldViewportX;
    s.originY = g_WorldViewportY;
    for (s.layerIdx = 0; s.layerIdx < 3; s.layerIdx++) {
        s.color.c.r = s.color.c.g = s.color.c.b = g_WorldTerrainCurrentRecords.channel[s.layerIdx];
        s.color.c.a = g_WorldTerrainCurrentRecords.channel[s.layerIdx];
        if (g_WorldZoneMeshCoords.channel[s.layerIdx]) {
            g_WorldTerrainCurrentRecords.channel[s.layerIdx] += s.layerIdx ? g_WorldZoneMeshCoords.channel[s.layerIdx] : (g_WorldZoneMeshCoords.channel[s.layerIdx] * (g_WorldTerrainCurrentRecords.channel[s.layerIdx] + 128)) >> 8;
            if (g_WorldTerrainCurrentRecords.channel[s.layerIdx] <= 0) {
                g_WorldTerrainCurrentRecords.channel[s.layerIdx] = 0;
                g_WorldZoneMeshCoords.channel[s.layerIdx] = 0;
            } else if (g_WorldTerrainCurrentRecords.channel[s.layerIdx] >= g_WorldOverlayChannelMaxLevels[s.layerIdx]) {
                g_WorldTerrainCurrentRecords.channel[s.layerIdx] = g_WorldOverlayChannelMaxLevels[s.layerIdx];
                g_WorldZoneMeshCoords.channel[s.layerIdx] = 0;
            }
        }
        if (g_WorldTerrainCurrentRecords.channel[s.layerIdx]) {
            if (s.layerIdx) {
                if (AcquireBatchNode(1, ((void **)&g_WorldScreenOverlayRenderObjects)[s.layerIdx])) {
                    if (World_IsTerrainRenderEnabled()) {
                        s.texU = g_WorldUnderwaterGrad[s.layerIdx][0] * (1.0f / 256.0f);
                        s.texV = g_WorldUnderwaterGrad[s.layerIdx][1] * (1.0f / 256.0f);
                        s.width = (float)(unsigned int)g_WorldRenderWidth * (1.0f / 256.0f);
                        s.height = (float)(unsigned int)g_WorldRenderHeight * (1.0f / 256.0f);
                        s.color.c.a = 128;
                        s.snow[3] = (GfxTlVertex *)(((GfxRenderObject *)((void **)&g_WorldScreenOverlayRenderObjects)[s.layerIdx])->vertexCursor + 0);
                        s.snow[3]->x = (float)s.originX;
                        s.snow[3]->y = (float)s.originY;
                        s.snow[3]->z = 0;
                        s.snow[3]->rhw = 1.0f;
                        s.snow[3]->colorUnsigned = s.color.value;
                        s.snow[3]->specularUnsigned = 0xff000000;
                        s.snow[3]->u = s.texU;
                        s.snow[3]->v = s.texV;
                        s.snow[2] = (GfxTlVertex *)(((GfxRenderObject *)((void **)&g_WorldScreenOverlayRenderObjects)[s.layerIdx])->vertexCursor + 32);
                        s.snow[2]->x = (float)s.originX;
                        s.snow[2]->y = (float)(unsigned int)g_WorldRenderHeight + (float)s.originY;
                        s.snow[2]->z = 0;
                        s.snow[2]->rhw = 1.0f;
                        s.snow[2]->colorUnsigned = s.color.value;
                        s.snow[2]->specularUnsigned = 0xff000000;
                        s.snow[2]->u = s.texU;
                        s.snow[2]->v = s.texV + s.height;
                        s.snow[1] = (GfxTlVertex *)(((GfxRenderObject *)((void **)&g_WorldScreenOverlayRenderObjects)[s.layerIdx])->vertexCursor + 64);
                        s.snow[1]->x = (float)(unsigned int)g_WorldRenderWidth + (float)s.originX;
                        s.snow[1]->y = (float)s.originY;
                        s.snow[1]->z = 0;
                        s.snow[1]->rhw = 1.0f;
                        s.snow[1]->colorUnsigned = s.color.value;
                        s.snow[1]->specularUnsigned = 0xff000000;
                        s.snow[1]->u = s.texU + s.width;
                        s.snow[1]->v = s.texV;
                        s.snow[0] = (GfxTlVertex *)(((GfxRenderObject *)((void **)&g_WorldScreenOverlayRenderObjects)[s.layerIdx])->vertexCursor + 96);
                        s.snow[0]->x = (float)(unsigned int)g_WorldRenderWidth + (float)s.originX;
                        s.snow[0]->y = (float)(unsigned int)g_WorldRenderHeight + (float)s.originY;
                        s.snow[0]->z = 0;
                        s.snow[0]->rhw = 1.0f;
                        s.snow[0]->colorUnsigned = s.color.value;
                        s.snow[0]->specularUnsigned = 0xff000000;
                        s.snow[0]->u = s.texU + s.width;
                        s.snow[0]->v = s.texV + s.height;
                    }
                }
            } else {
                if (AcquireBatchNode(1, g_WorldScreenOverlayRenderObjects)) {
                    if (World_IsTerrainRenderEnabled()) {
                        if (g_WorldScreenFlashArmed) {
                            s.color.c.r = s.color.c.g = s.color.c.b = g_WorldTerrainCurrentRecords.channel[s.layerIdx];
                            s.color.c.a = 255;
                            if (g_WorldScreenFlashStarted == 0)
                                g_WorldScreenFlashStarted = 1;
                            else {
                                g_WorldScreenFlashStarted = 0;
                                g_WorldScreenFlashArmed = 0;
                            }
                        } else {
                            s.color.c.r = s.color.c.g = s.color.c.b = 0;
                        }
                        s.fade[3] = (GfxTlVertex *)(g_WorldScreenOverlayRenderObjects->vertexCursor + 0);
                        s.fade[3]->x = (float)s.originX;
                        s.fade[3]->y = (float)s.originY;
                        s.fade[3]->z = 0;
                        s.fade[3]->rhw = 1.0f;
                        s.fade[3]->colorUnsigned = s.color.value;
                        s.fade[3]->specularUnsigned = 0xff000000;
                        s.fade[2] = (GfxTlVertex *)(g_WorldScreenOverlayRenderObjects->vertexCursor + 32);
                        s.fade[2]->x = (float)s.originX;
                        s.fade[2]->y = (float)(unsigned int)g_WorldRenderHeight + (float)s.originY;
                        s.fade[2]->z = 0;
                        s.fade[2]->rhw = 1.0f;
                        s.fade[2]->colorUnsigned = s.color.value;
                        s.fade[2]->specularUnsigned = 0xff000000;
                        s.fade[1] = (GfxTlVertex *)(g_WorldScreenOverlayRenderObjects->vertexCursor + 64);
                        s.fade[1]->x = (float)(unsigned int)g_WorldRenderWidth + (float)s.originX;
                        s.fade[1]->y = (float)s.originY;
                        s.fade[1]->z = 0;
                        s.fade[1]->rhw = 1.0f;
                        s.fade[1]->colorUnsigned = s.color.value;
                        s.fade[1]->specularUnsigned = 0xff000000;
                        s.fade[0] = (GfxTlVertex *)(g_WorldScreenOverlayRenderObjects->vertexCursor + 96);
                        s.fade[0]->x = (float)(unsigned int)g_WorldRenderWidth + (float)s.originX;
                        s.fade[0]->y = (float)(unsigned int)g_WorldRenderHeight + (float)s.originY;
                        s.fade[0]->z = 0;
                        s.fade[0]->rhw = 1.0f;
                        s.fade[0]->colorUnsigned = s.color.value;
                        s.fade[0]->specularUnsigned = 0xff000000;
                    }
                }
            }
        }
        if (g_WorldOverlayChannelTimerTable[s.layerIdx]) {
            g_WorldOverlayChannelTimerTable[s.layerIdx]--;
            if (g_WorldOverlayChannelTimerTable[s.layerIdx] == 0)
                World_BeginOverlayChannelFadeDown(s.layerIdx);
        }
    }
    g_WorldUnderwaterGrad[1][0] = SinFixed((Savemap_GetClockTickCount() << 4) & 0xfff) >> 3;
    g_WorldUnderwaterGrad[1][1] = g_WorldOverlayChannel1Timer << 3;
    g_WorldUnderwaterGrad[2][0] = CosFixed((Savemap_GetClockTickCount() << 4) & 0xfff) >> 4;
    g_WorldUnderwaterGrad[2][1] = g_WorldTerrainEffectKind << 2;
}

/*
 * Starts a fade out: step g_WorldZoneMeshCoords = -step while the brightness g_WorldTerrainCurrentRecords is above 0, else
 * 0.
 */
// FUNCTION: FF7 0x00755b70
void __cdecl World_BeginFadeFromBlack(int step, int option)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if ((g_WorldTerrainCurrentRecords.brightness) > 0) {
        (g_WorldZoneMeshCoords.fadeStep) = -step;
    } else {
        (g_WorldZoneMeshCoords.fadeStep) = 0;
    }
}

/*
 * Starts a fade in: step g_WorldZoneMeshCoords = step while the brightness is below its target
 * g_WorldZoneInnerRadius, else 0.
 */
// FUNCTION: FF7 0x00755b97
void __cdecl World_BeginFadeToBlack(int step, int option)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if ((g_WorldTerrainCurrentRecords.brightness) < g_WorldZoneInnerRadius) {
        (g_WorldZoneMeshCoords.fadeStep) = step;
    } else {
        (g_WorldZoneMeshCoords.fadeStep) = 0;
    }
}

/*
 * Sets g_WorldZoneMeshCoords[channel] to 0x10 when g_WorldTerrainCurrentRecords[channel] is below
 * g_WorldZoneInnerRadius[channel], otherwise zero.
 */
// FUNCTION: FF7 0x00755bc1
void __cdecl World_BeginOverlayChannelFadeUp(int channel)
{
    g_WorldZoneMeshCoords.channel[channel] =
        (g_WorldTerrainCurrentRecords.channel[channel] < g_WorldOverlayChannelMaxLevels[channel]) ? 0x10 : 0;
}

/*
 * Sets g_WorldZoneMeshCoords[channel] to -0x10 when g_WorldTerrainCurrentRecords[channel] is
 * positive, otherwise zero.
 */
// FUNCTION: FF7 0x00755bed
void __cdecl World_BeginOverlayChannelFadeDown(int channel)
{
    g_WorldZoneMeshCoords.channel[channel] =
        (g_WorldTerrainCurrentRecords.channel[channel] > 0) ? -0x10 : 0;
}

/*
 * Runs World_BeginOverlayChannelFadeUp for channel `channel` and sets that channel's level g_WorldOverlayChannelTimerTable[channel] to 100.
 */
// FUNCTION: FF7 0x00755c10
void __cdecl World_ShowOverlayChannelTimed(int channel)
{
    World_BeginOverlayChannelFadeUp(channel);
    g_WorldOverlayChannelTimerTable[channel] = 100;
}

/*
 * Returns whether no fade is in progress (step g_WorldZoneMeshCoords == 0).
 */
// FUNCTION: FF7 0x00755c2f
int __cdecl World_IsScreenFadeIdle(void)
{
    return (g_WorldZoneMeshCoords.fadeStep) == 0;
}

/*
 * Expands packed short color components from g_WorldTerrainFloodBuffer1 into three short components masked to
 * 0x7C00 in the preceding vector table. Fills the 256-word table at g_WorldUnderwaterPalette with successive
 * random bytes shifted left by two; the local four-short initializer is unused.
 */
// FUNCTION: FF7 0x00755c40
void __cdecl World_InitUnderwaterColorVectorsAndNoise(void)
{
    struct {
        SVECTOR *floodBufferEnd;
        int i;
        SVECTOR *floodMarkCursor;
        short local_3_unused[4];
        short *floodBufferCursor;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.local_3_unused[0] = 0;
    frame.local_3_unused[1] = 0x1e0;
    frame.local_3_unused[2] = 0x100;
    frame.local_3_unused[3] = 1;
    frame.floodBufferCursor = (short *)g_WorldTerrainFloodBuffer1;
    frame.floodBufferEnd = (SVECTOR *)g_WorldTerrainFloodBuffer1;
    for (frame.floodMarkCursor = (SVECTOR *)g_WorldTerrainFloodMarks; frame.floodMarkCursor < frame.floodBufferEnd;
         frame.floodMarkCursor++) {
        frame.floodMarkCursor->vx = (*frame.floodBufferCursor << 10) & 0x7c00;
        frame.floodMarkCursor->vy = (*frame.floodBufferCursor << 5) & 0x7c00;
        frame.floodMarkCursor->vz = *frame.floodBufferCursor & 0x7c00;
        frame.floodBufferCursor++;
    }
    for (frame.i = 0; frame.i < 256; frame.i++) {
        g_WorldUnderwaterPalette[frame.i] = WorldMap_NextRandomByte() << 2;
    }
}

/*
 * Installs an index-selected diagonal transform and Z translation, transforms the vector array at
 * g_WorldTerrainFloodMarks up to g_WorldTerrainFloodBuffer1, adds the
 * corresponding noise value to all three output components and packs them into 15-bit colors at
 * 0xE2B350. Builds a separate 256-entry byte-color gradient starting at RGB 187, 187, 255: green
 * falls each entry, red falls by two through the first half and blue falls by two through the
 * second, each clamped at zero. Copies those channels to float-color entries with alpha 255. The
 * initialized rectangle is unused.
 */
// FUNCTION: FF7 0x00755cff
void __cdecl World_BuildUnderwaterNoisePaletteAndGradient(int index)
{
    struct ByteColor {
        unsigned char b, g, r, a;
    };
    struct FloatColor {
        float b, g, r, a;
    };
    struct {
        int *noise;
        int green;
        PsxMatrix32 matrix;
        SVECTOR *end;
        int red, blue, i;
        short *palette;
        VECTOR output;
        int flags;
        SVECTOR *input;
        int value;
        short rect[4];
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.rect[0] = 0;
    s.rect[1] = 480 + index;
    s.rect[2] = 256;
    s.rect[3] = 1;
    s.matrix.m[0][0] = s.matrix.m[1][1] = s.matrix.m[2][2] = ((int *)&g_WorldUnderwaterNoise)[index] << 5;
    s.matrix.m[0][1] = s.matrix.m[0][2] = s.matrix.m[1][0] = s.matrix.m[1][2] = s.matrix.m[2][0] = s.matrix.m[2][1] = 0;
    s.matrix.t[0] = 0;
    s.matrix.t[1] = 0;
    s.matrix.t[2] = ((int *)((char *)&g_WorldUnderwaterColor + 32))[index] << 5;
    SetRotMatrix(&s.matrix);
    SetTransMatrix(&s.matrix);
    s.palette = (short *)g_WorldTerrainFloodBuffer1;
    s.noise = g_WorldUnderwaterPalette;
    s.end = (SVECTOR *)g_WorldTerrainFloodBuffer1;
    s.i = 0;
    for (s.input = (SVECTOR *)g_WorldTerrainFloodMarks; s.input < s.end; s.input++) {
        RotTransPoint((void *)s.input, (int *)&s.output, &s.flags);
        s.value = *s.noise++;
        s.output.vx += s.value;
        s.output.vy += s.value;
        s.output.vz += s.value;
        *s.palette++ = ((s.output.vx >> 10) & 31) | ((s.output.vy >> 5) & 0x3e0) | (s.output.vz & 0x7c00);
    }
    s.red = s.green = 187;
    s.blue = 255;
    for (s.i = 0; s.i < 256; s.i++) {
        ((struct ByteColor *)g_WorldTerrainEffectSlots)[s.i].r = s.red;
        ((struct ByteColor *)g_WorldTerrainEffectSlots)[s.i].g = s.green;
        ((struct ByteColor *)g_WorldTerrainEffectSlots)[s.i].b = s.blue;
        ((struct ByteColor *)g_WorldTerrainEffectSlots)[s.i].a = 255;
        if (s.i < 128) {
            s.red -= 2;
            if (s.red < 0)
                s.red = 0;
        } else {
            s.blue -= 2;
            if (s.blue < 0)
                s.blue = 0;
        }
        s.green--;
        if (s.green < 0)
            s.green = 0;
    }
    for (s.i = 0; s.i < 256; s.i++) {
        ((struct FloatColor *)g_WorldScratchColorBuffer)[s.i].r = (float)((struct ByteColor *)g_WorldTerrainEffectSlots)[s.i].r;
        ((struct FloatColor *)g_WorldScratchColorBuffer)[s.i].g = (float)((struct ByteColor *)g_WorldTerrainEffectSlots)[s.i].g;
        ((struct FloatColor *)g_WorldScratchColorBuffer)[s.i].b = (float)((struct ByteColor *)g_WorldTerrainEffectSlots)[s.i].b;
        ((struct FloatColor *)g_WorldScratchColorBuffer)[s.i].a = 255.0f;
    }
}

/*
 * Initializes the twelve WorldShadowQuad records of g_WorldOverlayChannels with packed color
 * 0x80808080 and level sequence 7, 6, 5, 4, four entries at 3, then 3, 2, 1, 0. Derives 16-wide
 * atlas coordinate ranges from each level, clears the lower coordinate bytes and copies
 * g_WorldMidgarRenderObject into each record's batch. Clears five state globals and the value field
 * of six two-integer pairs at g_WorldZoneAmbientColor; leaves each pair second field unchanged.
 */
// FUNCTION: FF7 0x00755f95
void __cdecl World_InitZolomBodySegmentRecords(void)
{
    struct WorldMapPair {
        int value;
        int unused;
    };
    int idx;
    WorldShadowQuad *currRec;
    int count;
    int v;
    int temp;
    int activeTmp;
    int loopLevel;

    for (count = 0; count < 12; count++) {
        currRec = &g_WorldOverlayChannels[count];
        currRec->color = 0x80808080;
        if (count < 4) {
            v = 7 - count;
        } else {
            if (count >= 8) {
                temp = 0x0c - count - 1;
            } else {
                temp = 3;
            }
            v = temp;
        }
        currRec->clut = v;
        if (count < 4) {
            loopLevel = 7 - count;
        } else {
            if (count >= 8) {
                activeTmp = 0x0c - count - 1;
            } else {
                activeTmp = 3;
            }
            loopLevel = activeTmp;
        }
        currRec->u2 = loopLevel << 4;
        currRec->u0 = currRec->u2;
        currRec->v1 = 0;
        currRec->v0 = 0;
        currRec->u3 = currRec->u0 + 0x0f;
        currRec->u1 = currRec->u3;
        currRec->v3 = currRec->v0 + 0x0f;
        currRec->v2 = currRec->v3;
        currRec->batch = g_WorldMidgarRenderObject;
    }
    g_WorldZolomRandomOffset = 0;
    g_WorldZolomTurnCountdown = g_WorldZolomRandomOffset;
    g_WorldZolomPlayerXFlag = 0;
    g_WorldSkyHorizonY = g_WorldZolomPlayerXFlag;
    g_WorldZolomGroundHit = g_WorldSkyHorizonY;
    for (idx = 0; idx < 6; idx++) {
        ((struct WorldMapPair *)&g_WorldZoneAmbientColor)[idx].value = 0;
    }
}

/*
 * Builds the cloud layer once (g_WorldSkyHorizonY latches it) through World_InitZolomTrailRecords.
 */
// FUNCTION: FF7 0x0075611b
void __cdecl World_BuildZolomTrailOnce(void)
{
    if (g_WorldSkyHorizonY == 0) {
        World_InitZolomTrailRecords();
        g_WorldSkyHorizonY = 1;
    }
}

/*
 * Calls WorldEntity_GetPlayerPosition on the contiguous local position storage and chooses cloud
 * coordinate pairs 0x2008/0x6338 when the consumed local component is below 0x23A98, otherwise
 * 0x1D4C/0x11F8. Fills sixteen eight-byte records at g_WorldSnakeDataPosition with that pair and zeroes their
 * other two shorts. Sets g_WorldMeteorSpriteEnabled to 0x400 and publishes the record pointer. The retained C
 * has no explicit assignment to local r; interpreting the component requires the native stack-
 * layout position-write convention.
 */
// FUNCTION: FF7 0x00756138
void __cdecl World_InitZolomTrailRecords(void)
{
    WorldZolomTrailPoint *trail;
    VECTOR posBuf;

    WorldEntity_GetPlayerPosition(&posBuf);
    if (posBuf.vz < 146072) {
        for (trail = g_WorldSnakeDataPosition; trail < (WorldZolomTrailPoint *)g_WorldSnakeDataPositionEnd; trail++) {
            trail->x = 8200;
            trail->z = 25400;
            trail->ground = 0;
            trail->heading = 0;
        }
    } else {
        for (trail = g_WorldSnakeDataPosition; trail < (WorldZolomTrailPoint *)g_WorldSnakeDataPositionEnd; trail++) {
            trail->x = 7500;
            trail->z = 4600;
            trail->ground = 0;
            trail->heading = 0;
        }
    }
    g_WorldMeteorSpriteEnabled = 0x400;
    g_pWorldZolomTrailHead = g_WorldSnakeDataPosition;
}

/*
 * Returns g_WorldZolomPlayerXFlag.
 */
// FUNCTION: FF7 0x007561ec
int __cdecl World_GetSwampEntryXFlag(void)
{
    return g_WorldZolomPlayerXFlag;
}

/*
 * When World_IsZolomActiveAndPlayerInRegion permits sampling, constructs the selected Zolom record position with X/Z
 * offsets 0x34000/0x20000 and converts it to mesh coordinates. For a matching terrain chunk,
 * samples the ground into the record, stores the sampling result and clears the ground field when
 * the result is zero.
 */
// FUNCTION: FF7 0x007561f6
void __cdecl World_SampleZolomHeadGround(WorldTerrainChunkInfo *chunk)
{
    short chunkY; /* target wChunkY */
    struct {
        short vx;
        short vy;
        short vz;
        short pad;
    } samplePosition; /* target local_7 */
    short meshColumn; /* target wChunkX */
    struct {
        int vx;
        int vy;
        int vz;
        int pad;
    } headPos;

    if (World_IsZolomActiveAndPlayerInRegion()) {
        headPos.vx = g_pWorldZolomTrailHead->x + 0x34000;
        headPos.vz = g_pWorldZolomTrailHead->z + 0x20000;
        WorldMap_PositionToMesh(&headPos, &samplePosition, &meshColumn, &chunkY);
        if (meshColumn == chunk->wChunkX && chunkY == chunk->wChunkY) {
            g_WorldZolomGroundHit = World_SampleGroundAtEntity(
                chunk, (SVECTOR *)&samplePosition, (WorldTriangleCache *)&g_WorldZoneAmbientColor,
                &g_pWorldZolomTrailHead->ground, 0, 0, 100);
            if (g_WorldZolomGroundHit == 0) {
                g_pWorldZolomTrailHead->ground = 0;
            }
        }
    }
}

/*
 * Calls WorldEntity_GetPlayerPosition with the address of local s, then returns whether the cloud
 * latch is set and s lies in [0x30000,0x40000) and q in [0x1c000,0x2c000). The standalone C
 * declarations do not explicitly initialize q; its position-component meaning depends on the
 * accepted native contiguous stack layout.
 */
// FUNCTION: FF7 0x007562a9
int __cdecl World_IsZolomActiveAndPlayerInRegion(void)
{
    VECTOR viewPos;
    int inRegion;

    WorldEntity_GetPlayerPosition(&viewPos);
    if (g_WorldSkyHorizonY != 0 && viewPos.vx >= 196608 && viewPos.vx < 262144 && viewPos.vz >= 114688 && viewPos.vz < 180224) {
        inRegion = 1;
    } else {
        inRegion = 0;
    }
    return inRegion;
}

/*
 * Samples player position and updates the world Zolom record when World_IsZolomActiveAndPlayerInRegion permits. With a
 * clear prior flag smooths its unsigned X/Z toward 10000/15000 and updates g_WorldMeteorSpriteEnabled; then
 * clears the flag and computes Manhattan distance from the record position offset by
 * 0x34000/0x20000. Calls World_AdvanceZolomTrailHead when distance exceeds 200 or neither player-model predicate
 * qualifies; otherwise requests system 7 and clears g_WorldSkyHorizonY. Calls World_DrawZolomBodySegments afterward.
 * Tracks terrain and model changes, retaining previous terrain on a model change and setting
 * g_WorldZolomPlayerXFlag from X>0x36000 on a terrain-0 to terrain-7 transition.
 */
// FUNCTION: FF7 0x007562ff
void __cdecl World_UpdateZolom(void)
{
    struct {
        int zolomDistance;
        int dwCurrentTerrain;
        struct {
            int vx;
            int vy;
            int vz;
            int pad;
        } playerPos;
        int dwCurrentVehicle;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    WorldEntity_GetPlayerPosition((VECTOR *)&frame.playerPos);
    if (World_IsZolomActiveAndPlayerInRegion()) {
        if (g_WorldZolomGroundHit == 0) {
            g_pWorldZolomTrailHead->x =
                (g_pWorldZolomTrailHead->x * 63 + 10000) >> 6;
            g_pWorldZolomTrailHead->z =
                (g_pWorldZolomTrailHead->z * 63 + 15000) >> 6;
            g_WorldMeteorSpriteEnabled = g_pWorldZolomTrailHead->headingUnsigned + 0x800;
        }
        g_WorldZolomGroundHit = 0;
        frame.zolomDistance =
            (frame.playerPos.vx - g_pWorldZolomTrailHead->x - 212992 > 0
                 ? frame.playerPos.vx - g_pWorldZolomTrailHead->x - 0x34000
                 : -(frame.playerPos.vx - g_pWorldZolomTrailHead->x - 0x34000)) +
            (frame.playerPos.vz - g_pWorldZolomTrailHead->z - 131072 > 0
                 ? frame.playerPos.vz - g_pWorldZolomTrailHead->z - 0x20000
                 : -(frame.playerPos.vz - g_pWorldZolomTrailHead->z - 0x20000));
        if (frame.zolomDistance > 200 ||
            (WorldEntity_IsPlayerModelInMask(7) == 0 && WorldEntity_IsPlayerRideable() == 0)) {
            World_AdvanceZolomTrailHead();
        } else {
            WorldScript_RequestSystem(7);
            g_WorldSkyHorizonY = 0;
        }
        World_DrawZolomBodySegments();
    }
    frame.dwCurrentTerrain = WorldEntity_GetPlayerTerrainType() & WORLD_WALKMESH_TERRAIN_MASK;
    frame.dwCurrentVehicle = WorldEntity_GetPlayerModelId();
    if (frame.dwCurrentVehicle != g_WorldZolomPrevPlayerModel)
        frame.dwCurrentTerrain = g_WorldZolomPrevTerrain;
    g_WorldZolomPrevPlayerModel = frame.dwCurrentVehicle;
    if (frame.dwCurrentTerrain == WORLD_TERRAIN_SWAMP && g_WorldZolomPrevTerrain == 0)
        g_WorldZolomPlayerXFlag = frame.playerPos.vx > 221184;
    g_WorldZolomPrevTerrain = frame.dwCurrentTerrain;
}

/*
 * Advances the eight-byte world path ring from g_WorldSnakeDataPosition up to g_WorldSnakeDataPositionEnd, placing the next
 * X/Z 100 units along the current heading and storing its heading. On terrain 7 with either
 * qualifying player-model predicate, tries heading offsets plus/minus 300000 divided by distance
 * when the signed-short Manhattan distance exceeds 300. Each trial compares against the original
 * straight-path distance, so an improving minus trial replaces an improving plus trial even if the
 * plus trial was closer. Otherwise postdecrements the timer and, when expired, randomizes the
 * timer and turn rate. Advances the ring pointer, adds the turn rate and applies one full-turn
 * wrap correction.
 */
// FUNCTION: FF7 0x007564cd
void __cdecl World_AdvanceZolomTrailHead(void)
{
    int playerPosition[3];
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short unusedZero;
    short straightDist;
    short dstStep[4];
    WorldZolomTrailPoint *pTrailPoint;
    short trialGap;
    short turnOffset;

    dstStep[0] = dstStep[1] = 0;
    dstStep[2] = 100;
    WorldMap_RotateVec3ByYaw((SVECTOR *)dstStep, g_WorldMeteorSpriteEnabled);
    pTrailPoint = g_pWorldZolomTrailHead + 1;
    if (pTrailPoint >= (WorldZolomTrailPoint *)g_WorldSnakeDataPositionEnd) {
        pTrailPoint = g_WorldSnakeDataPosition;
    }
    pTrailPoint->x = g_pWorldZolomTrailHead->x + dstStep[0];
    pTrailPoint->z = g_pWorldZolomTrailHead->z + dstStep[2];
    pTrailPoint->heading = g_WorldMeteorSpriteEnabled;
    if ((WorldEntity_IsPlayerModelInMask(7) != 0 || WorldEntity_IsPlayerRideable() != 0) && WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_SWAMP) {
        unusedZero = 0;
        WorldEntity_GetPlayerPosition((VECTOR *)playerPosition);
        playerPosition[0] -= 0x34000;
        playerPosition[2] -= 0x20000;
        straightDist = (pTrailPoint->x - playerPosition[0] > 0 ? pTrailPoint->x - playerPosition[0] : -(pTrailPoint->x - playerPosition[0])) + (pTrailPoint->z - playerPosition[2] > 0 ? pTrailPoint->z - playerPosition[2] : -(pTrailPoint->z - playerPosition[2]));
        turnOffset = straightDist > 300 ? 0x493E0 / straightDist : 0;
        dstStep[0] = dstStep[1] = 0;
        dstStep[2] = 100;
        WorldMap_RotateVec3ByYaw((SVECTOR *)dstStep, g_WorldMeteorSpriteEnabled + turnOffset);
        trialGap = (g_pWorldZolomTrailHead->x + dstStep[0] - playerPosition[0] > 0 ? g_pWorldZolomTrailHead->x + dstStep[0] - playerPosition[0] : -(g_pWorldZolomTrailHead->x + dstStep[0] - playerPosition[0])) + (g_pWorldZolomTrailHead->z + dstStep[2] - playerPosition[2] > 0 ? g_pWorldZolomTrailHead->z + dstStep[2] - playerPosition[2] : -(g_pWorldZolomTrailHead->z + dstStep[2] - playerPosition[2]));
        if (trialGap < straightDist) {
            pTrailPoint->x = g_pWorldZolomTrailHead->x + dstStep[0];
            pTrailPoint->z = g_pWorldZolomTrailHead->z + dstStep[2];
            pTrailPoint->heading = g_WorldMeteorSpriteEnabled + turnOffset;
        }
        dstStep[0] = dstStep[1] = 0;
        dstStep[2] = 100;
        WorldMap_RotateVec3ByYaw((SVECTOR *)dstStep, g_WorldMeteorSpriteEnabled - turnOffset);
        trialGap = (g_pWorldZolomTrailHead->x + dstStep[0] - playerPosition[0] > 0 ? g_pWorldZolomTrailHead->x + dstStep[0] - playerPosition[0] : -(g_pWorldZolomTrailHead->x + dstStep[0] - playerPosition[0])) + (g_pWorldZolomTrailHead->z + dstStep[2] - playerPosition[2] > 0 ? g_pWorldZolomTrailHead->z + dstStep[2] - playerPosition[2] : -(g_pWorldZolomTrailHead->z + dstStep[2] - playerPosition[2]));
        if (trialGap < straightDist) {
            pTrailPoint->x = g_pWorldZolomTrailHead->x + dstStep[0];
            pTrailPoint->z = g_pWorldZolomTrailHead->z + dstStep[2];
            pTrailPoint->heading = g_WorldMeteorSpriteEnabled - turnOffset;
        }
        g_WorldMeteorSpriteEnabled = pTrailPoint->heading;
    } else if (g_WorldZolomTurnCountdown-- <= 0) {
        g_WorldZolomTurnCountdown = WorldMap_NextRandomByte() + 64 >> 2;
        g_WorldZolomRandomOffset = WorldMap_NextRandomByte() - 0x80 >> 2;
    }
    g_pWorldZolomTrailHead = pTrailPoint;
    g_WorldMeteorSpriteEnabled += g_WorldZolomRandomOffset;
    if (g_WorldMeteorSpriteEnabled < 0) {
        g_WorldMeteorSpriteEnabled += 0x1000;
    } else if (g_WorldMeteorSpriteEnabled >= 0x1000) {
        g_WorldMeteorSpriteEnabled -= 0x1000;
    }
}

/*
 * Visits twelve effect slots and wraps their associated cloud records through the cloud table. For
 * clouds whose terrain chunk is resident and whose relative X/Z lie within 30000 of the stream
 * focus, stores the relative coordinates and updates the effect using terrain-adjusted height, the
 * cloud angle and fixed dimensions 300 by 300.
 */
// FUNCTION: FF7 0x0075692a
void __cdecl World_DrawZolomBodySegments(void)
{
    struct {
        int diff;
        int local_7_unused;
        WorldShadowQuad *effect;
        WorldZolomTrailPoint *cloud;
        int position[3];
        int frame_slot;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.local_7_unused = World_GetRenderBufferIndex();
    World_GetStreamFocus((WorldStreamFocusData *)local.position);
    local.cloud = g_pWorldZolomTrailHead - 1;
    if (local.cloud < g_WorldSnakeDataPosition) {
        local.cloud = &g_WorldSnakeDataPosition[47];
    }
    for (local.effect = g_WorldOverlayChannels; local.effect < (WorldShadowQuad *)&g_WorldSteeringCycleLimit;
         local.effect++) {
        local.cloud += 4;
        if (local.cloud >= (WorldZolomTrailPoint *)g_WorldSnakeDataPositionEnd) {
            local.cloud -= 48;
        }
        if (World_IsTerrainChunkLoaded((local.cloud->x >> 13) + 0x1A,
                                                                   (local.cloud->z >> 13) + 0x10) != 0) {
            local.diff = local.cloud->x + 0x34000 - local.position[0];
            if (local.diff < -30000 || local.diff > 30000) {
                continue;
            }
            local.effect->pad1 = local.diff;
            local.diff = local.cloud->z + 0x20000 - local.position[2];
            if (local.diff < -30000 || local.diff > 30000) {
                continue;
            }
            local.effect->pad2 = local.diff;
            World_DrawShadowQuad(300,
                                 300,
                                 local.cloud->groundUnsigned - World_GetPointFadeWeight(local.effect->pad1,
                                                                           local.effect->pad2) *
                                                      4,
                                 local.cloud->headingUnsigned + 0x800,
                                 local.effect,
                                 0);
        }
    }
}

/*
 * Resets the terrain flood-fill and effect state: g_WorldZoneOuterRadius, g_pWorldTerrainFloodCursor, g_WorldTerrainFloodState,
 * g_WorldTerrainEffectActive, g_pWorldSelectedTerrainTriangle, g_WorldSkyHorizonOffset, g_WorldSteeringCycle, g_WorldRandomSeed/2c/30 and g_WorldTerrainFloodFinishRequested all
 * to 0.
 */
// FUNCTION: FF7 0x00756a95
void __cdecl World_ResetTerrainFloodAndEffectState(void)
{
    g_WorldZoneOuterRadius = 0;
    g_pWorldTerrainFloodCursor = (WorldTerrainProbe *)g_WorldZoneOuterRadius;
    g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_NONE;
    g_WorldTerrainEffectActive = 0;
    g_pWorldSelectedTerrainTriangle = 0;
    g_WorldSkyHorizonOffset = 0;
    g_WorldSteeringCycle = g_WorldSkyHorizonOffset;
    g_WorldRandomCounter = 0;
    g_WorldRandomIndex = g_WorldRandomCounter;
    g_WorldRandomSeed = g_WorldRandomIndex;
    g_WorldTerrainFloodFinishRequested = 0;
}

/*
 * For a pending crossing and the matching nonnull mesh coordinates, clears flood marks and samples
 * the crossing point. Requires an unmarked triangle, a nonreserved terrain category and a set bit
 * in mask 0x321B6F83. Advances the stack pointer by 12 before checking capacity. Selects the entry
 * edge (enum WorldTerrainFloodEdge) from the vertex pair on the crossed boundary, using X/Z above
 * 0x1FF6 or below 10 for the crossing direction (enum WorldTerrainFloodCrossState). Reports error
 * 0x48 for no edge but still writes the record and clears crossing state; capacity failure reports
 * 0x49 with the pointer already advanced. An unusable sample sets WORLD_FLOOD_CROSS_FAILED.
 */
// FUNCTION: FF7 0x00756b0c
void __cdecl World_PushTerrainFloodCrossingRecord(struct WorldPushTerrainFloodCrossingRecordMesh *mesh)
{
    WorldTerrainTriangle *tri;
    int sampleMesh;
    int edgeBit;

    if (g_WorldTerrainFloodState != WORLD_FLOOD_CROSS_NONE && mesh != 0 && g_WorldTerrainFloodFinishReq == mesh->meshCol && g_WorldTerrainFloodMeshRow == mesh->meshRow) {
        World_ClearFloodMarksOnMesh((int)mesh);
        if (World_SampleGroundAtEntity((WorldTerrainChunkInfo *)mesh, (SVECTOR *)&g_WorldSkyVertexQuad, 0, (short *)&sampleMesh, &tri, 0, 0) != 0 && (tri->region & WORLD_TRIANGLE_FLOOD_VISITED) == 0 && (tri->terrain & WORLD_WALKMESH_REGION_MASK) != WORLD_WALKMESH_REGION_MASK && (WORLD_TERRAIN_MASK_CHOCOBO_DEFAULT >> (tri->terrain & WORLD_WALKMESH_TERRAIN_MASK) & 1) != 0) {
            g_pWorldTerrainFloodCursor++;
            if ((unsigned int)g_pWorldTerrainFloodCursor < (unsigned int)&g_WorldZoneData) {
                edgeBit = 0;
                switch (g_WorldTerrainFloodState) {
                case WORLD_FLOOD_CROSS_NEG_X:
                    if (mesh->vertexData[tri->vertices[0]].vx > 8182 && mesh->vertexData[tri->vertices[1]].vx > 8182) {
                        edgeBit = WORLD_FLOOD_EDGE_V0_V1;
                    } else if (mesh->vertexData[tri->vertices[1]].vx > 8182 && mesh->vertexData[tri->vertices[2]].vx > 8182) {
                        edgeBit = WORLD_FLOOD_EDGE_V1_V2;
                    } else if (mesh->vertexData[tri->vertices[2]].vx > 8182 && mesh->vertexData[tri->vertices[0]].vx > 8182) {
                        edgeBit = WORLD_FLOOD_EDGE_V2_V0;
                    }
                    break;
                case WORLD_FLOOD_CROSS_NEG_Z:
                    if (mesh->vertexData[tri->vertices[0]].vz > 8182 && mesh->vertexData[tri->vertices[1]].vz > 8182) {
                        edgeBit = WORLD_FLOOD_EDGE_V0_V1;
                    } else if (mesh->vertexData[tri->vertices[1]].vz > 8182 && mesh->vertexData[tri->vertices[2]].vz > 8182) {
                        edgeBit = WORLD_FLOOD_EDGE_V1_V2;
                    } else if (mesh->vertexData[tri->vertices[2]].vz > 8182 && mesh->vertexData[tri->vertices[0]].vz > 8182) {
                        edgeBit = WORLD_FLOOD_EDGE_V2_V0;
                    }
                    break;
                case WORLD_FLOOD_CROSS_POS_X:
                    if (mesh->vertexData[tri->vertices[0]].vx < 10 && mesh->vertexData[tri->vertices[1]].vx < 10) {
                        edgeBit = WORLD_FLOOD_EDGE_V0_V1;
                    } else if (mesh->vertexData[tri->vertices[1]].vx < 10 && mesh->vertexData[tri->vertices[2]].vx < 10) {
                        edgeBit = WORLD_FLOOD_EDGE_V1_V2;
                    } else if (mesh->vertexData[tri->vertices[2]].vx < 10 && mesh->vertexData[tri->vertices[0]].vx < 10) {
                        edgeBit = WORLD_FLOOD_EDGE_V2_V0;
                    }
                    break;
                case WORLD_FLOOD_CROSS_POS_Z:
                    if (mesh->vertexData[tri->vertices[0]].vz < 10 && mesh->vertexData[tri->vertices[1]].vz < 10) {
                        edgeBit = WORLD_FLOOD_EDGE_V0_V1;
                    } else if (mesh->vertexData[tri->vertices[1]].vz < 10 && mesh->vertexData[tri->vertices[2]].vz < 10) {
                        edgeBit = WORLD_FLOOD_EDGE_V1_V2;
                    } else if (mesh->vertexData[tri->vertices[2]].vz < 10 && mesh->vertexData[tri->vertices[0]].vz < 10) {
                        edgeBit = WORLD_FLOOD_EDGE_V2_V0;
                    }
                    break;
                }
                if (edgeBit == 0) {
                    WorldScript_ReportError(0x48);
                }
                g_pWorldTerrainFloodCursor->chunk = (WorldTerrainChunkInfo *)mesh;
                g_pWorldTerrainFloodCursor->triangle = tri;
                g_pWorldTerrainFloodCursor->flags = edgeBit;
                g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_NONE;
            } else {
                WorldScript_ReportError(0x49);
            }
        } else {
            g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_FAILED;
        }
    }
}

/*
 * For a nonnull mesh not already in the visited-mesh table, appends its address and clears
 * triangle flag bit 0x40 on every triangle. Reports error 0x47 when the append cursor has reached
 * its limit, but still performs the append and triangle traversal.
 */
// FUNCTION: FF7 0x00756f45
void __cdecl World_ClearFloodMarksOnMesh(int mesh)
{
    WorldTerrainTriangle *triPtr;
    int visitedSlot;

    if (mesh != 0) {
        visitedSlot = (unsigned int)&g_WorldTerrainFloodVisitedMeshes;
        for (; visitedSlot < (unsigned int)g_WorldTerrainEffectActive && *(int *)visitedSlot != mesh; visitedSlot += 4)
            ;
        if (visitedSlot >= (unsigned int)g_WorldTerrainEffectActive) {
            if ((unsigned int)g_WorldTerrainEffectActive >= (unsigned int)&g_WorldZolomPlayerXFlag)
                WorldScript_ReportError(0x47);
            *(int *)g_WorldTerrainEffectActive = mesh;
            g_WorldTerrainEffectActive += 4;
            triPtr = ((WorldTerrainChunkInfo *)mesh)->triangles;
            for (; triPtr <
                   ((WorldTerrainChunkInfo *)mesh)->triangles + ((WorldTerrainChunkInfo *)mesh)->count;
                 triPtr++) {
                triPtr->region &= 0xBF;
            }
        }
    }
}

/*
 * When flood state is 0, 2 or 3, initializes the seed record with mesh mesh and optional triangle
 * triangle, clears counters and mesh flood marks, and sets the triangle's region bit 6 when nonnull.
 * Selects state 1 and tests the player position against half-open ranges 0x36000..0x40000 and
 * 0x1C000..0x26000 to set g_WorldTerrainFloodInRange. The retained C obtains the vector through &s and reads q
 * without explicit assignment; its component layout relies on the native contiguous local-storage
 * convention.
 */
// FUNCTION: FF7 0x00756ff9
void __cdecl World_BeginTerrainFloodFill(int mesh, int triangle)
{
    VECTOR viewPos;
    int inRange;

    if (g_WorldSteeringCycle == 0 || (g_WorldSteeringCycle == 2 || g_WorldSteeringCycle == 3)) {
        g_pWorldTerrainFloodCursor = (WorldTerrainProbe *)&g_WorldZoneFogNearRGB;
        g_pWorldTerrainFloodCursor->chunk = (WorldTerrainChunkInfo *)mesh;
        g_pWorldTerrainFloodCursor->triangle = (WorldTerrainTriangle *)triangle;
        g_pWorldTerrainFloodCursor->flags = 0;
        g_WorldZoneOuterRadius = 0;
        g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_NONE;
        g_WorldTerrainEffectActive = (int)&g_WorldTerrainFloodVisitedMeshes;
        World_ClearFloodMarksOnMesh(mesh);
        if (triangle != 0) {
            ((WorldTerrainTriangle *)triangle)->region |= WORLD_TRIANGLE_FLOOD_VISITED;
        }
        g_pWorldSelectedTerrainTriangle = 0;
        g_WorldSteeringCycle = 1;
        WorldEntity_GetPlayerPosition(&viewPos);
        if (viewPos.vx >= 221184 && viewPos.vx < 262144 && viewPos.vz >= 114688 && viewPos.vz < 155648) {
            inRange = 1;
        } else {
            inRange = 0;
        }
        g_WorldTerrainFloodInRange = inRange;
    }
}

/*
 * Original naked assembly state selector, active only when g_WorldSteeringCycle equals 2. Maps the
 * current model value 0x29 to slot 1, 0x2A to slot 2 and all others to slot 0. State 1 stores
 * g_WorldZoneOuterRadius in that slot and sets slot mode 5; state 2 uses the unsigned minimum of
 * that value and address g_WorldZoneFogFarRGB and mode 7; state 3 uses the unsigned
 * minimum with address g_WorldZoneFogNearRGB, selects entity 4 and sets mode 7 after the helper
 * calls. These states obtain a local value through World_GetProbeTriangleCenterXZ and pass it to
 * WorldEntity_SetCurrentPositionAndGround. All input states, including unsupported ones, store
 * state in g_WorldSkyHorizonOffset while active. Retains the original stack-frame and address
 * comparisons.
 */
// FUNCTION: FF7 0x007570e8
void __cdecl World_StartTerrainEffectAtPathRecord(int state)
{
    int model;
    char xz[16];
    int slot;

    if (g_WorldSteeringCycle == 2) {
        model = WorldEntity_GetCurrentModelId();
        slot = model == 0x29 ? 1 : model == 0x2A ? 2
                                                 : 0;
        switch (state) {
        case WORLD_CAPTURE_STATE_MOUNT_OR_SCRIPT:
            g_WorldRandomSeeds[slot] = g_WorldZoneOuterRadius;
            g_WorldTerrainFloodCrossing.slot[slot] = 5;
            World_GetProbeTriangleCenterXZ(g_WorldRandomSeeds[slot], xz);
            WorldEntity_SetCurrentPositionAndGround(xz);
            break;
        case WORLD_CAPTURE_STATE_DISMOUNT_FREE:
            g_WorldRandomSeeds[slot] = (unsigned int)((WorldTerrainProbe *)&g_WorldZoneFogNearRGB + 2) < (unsigned int)g_WorldZoneOuterRadius
                                        ? (int)&g_WorldZoneFogFarRGB
                                        : g_WorldZoneOuterRadius;
            g_WorldTerrainFloodCrossing.slot[slot] = 7;
            World_GetProbeTriangleCenterXZ(g_WorldRandomSeeds[slot], xz);
            WorldEntity_SetCurrentPositionAndGround(xz);
            break;
        case WORLD_CAPTURE_STATE_CHOCOBO_4_FREE:
            g_WorldRandomSeeds[slot] = (unsigned int)g_WorldZoneOuterRadius > (unsigned int)&g_WorldZoneFogNearRGB ? (int)&g_WorldZoneFogNearRGB : g_WorldZoneOuterRadius;
            WorldEntity_SetCurrentById(4);
            World_GetProbeTriangleCenterXZ(g_WorldRandomSeeds[slot], xz);
            WorldEntity_SetCurrentPositionAndGround(xz);
            g_WorldTerrainFloodCrossing.slot[slot] = 7;
            break;
        }
        g_WorldSkyHorizonOffset = state;
    }
}

/*
 * For a nonnull output, reads the three indexed vertices of the supplied triangle and writes their
 * average X/Z plus the mesh-coordinate offsets shifted by 13. Sets output Y to zero.
 */
// FUNCTION: FF7 0x00757260
void __cdecl World_GetProbeTriangleCenterXZ(int probe, void *center)
{
    WorldTerrainTriangle *indices;
    SVECTOR *vertices;

    if (center != 0) {
        indices = ((WorldTerrainProbe *)probe)->triangle;
        vertices = ((WorldTerrainProbe *)probe)->chunk->vertices;
        ((WorldPoint *)center)->posX = (vertices[indices->vertices[0]].vx +
                        vertices[indices->vertices[1]].vx +
                        vertices[indices->vertices[2]].vx) /
                           3 +
                       (((WorldTerrainProbe *)probe)->chunk->wChunkX << 13);
        ((WorldPoint *)center)->posY = 0;
        ((WorldPoint *)center)->posZ =
            (vertices[indices->vertices[0]].vz +
             vertices[indices->vertices[1]].vz +
             vertices[indices->vertices[2]].vz) /
                3 +
            (((WorldTerrainProbe *)probe)->chunk->wChunkY << 13);
    }
}

/*
 * Selects actor ID 4 in state 3, otherwise maps arguments 1/2/other to IDs 41/42/38. State 3
 * performs the reset helper and clears the state; state 1 either takes player control and arms
 * vehicle exit or requests script 5; state 2 calls both reset helpers.
 */
// FUNCTION: FF7 0x0075732c
void __cdecl World_FinishTerrainEffectSlot(int slot)
{
    int entityId;
    int mappedId;

    if (g_WorldSkyHorizonOffset == 3) {
        entityId = 4;
    } else {
        if (slot == 1) {
            mappedId = 0x29;
        } else {
            mappedId = (slot == 2) ? 0x2A : 0x26;
        }
        entityId = mappedId;
    }
    WorldEntity_SetCurrentById(entityId);
    switch (g_WorldSkyHorizonOffset) {
    case WORLD_CAPTURE_STATE_CHOCOBO_4_FREE:
        WorldEntity_FreeCurrent();
        g_WorldSkyHorizonOffset = 0;
        break;
    case WORLD_CAPTURE_STATE_MOUNT_OR_SCRIPT:
        if (WorldEntity_IsCurrentRiding()) {
            WorldEntity_SetPlayerControlled();
            World_SetVehicleExitArmed();
        } else {
            WorldScript_RequestOnEntity(WorldEntity_GetCurrentModelId(), 5);
        }
        break;
    case WORLD_CAPTURE_STATE_DISMOUNT_FREE:
        WorldEntity_FreeCurrentRiding();
        WorldEntity_FreeCurrent();
        break;
    }
}

/*
 * Per-frame terrain effect driver: runs World_StepTerrainFloodFill, then on a pending finish (g_WorldTerrainFloodFinishRequested)
 * either leaves state 3 (restoring player control mode 5) or, from state 2, sets zoom state 1,
 * camera distance 0x1770 and height 0x5DC and requests system script 8. When the effect kind
 * g_WorldSkyHorizonOffset is 3 in state 2 or 3 and no effect is running it fires World_StartTerrainEffectAtPathRecord(3). Finally
 * World_UpdateTerrainFloodPathFollowers.
 */
// FUNCTION: FF7 0x007573d9
void __cdecl World_UpdateTerrainEffect(void)
{
    World_StepTerrainFloodFill();
    if (g_WorldTerrainFloodFinishRequested != 0) {
        if (g_WorldSteeringCycle == 3) {
            g_WorldTerrainFloodFinishRequested = 0;
            g_WorldSteeringCycle = 0;
            WorldMap_SetPlayerControl(1, 5);
        } else if (g_WorldSteeringCycle == 2) {
            World_SetCameraZoomState(1);
            World_SetCameraDistance(0x1770);
            World_SetCameraHeight(0x5DC);
            WorldScript_RequestSystem(8);
            g_WorldTerrainFloodFinishRequested = 0;
        }
    }
    if (g_WorldSkyHorizonOffset == 3 && (g_WorldSteeringCycle == 2 || g_WorldSteeringCycle == 3)) {
        if ((g_WorldTerrainFloodCrossing.floodTimer) == 0 && g_WorldTerrainFloodCrossing.isBusy == 0) {
            World_StartTerrainEffectAtPathRecord(3);
        }
    }
    World_UpdateTerrainFloodPathFollowers();
}

/*
 * Calls World_AdvanceTerrainFloodFillStep.
 */
// FUNCTION: FF7 0x00757494
void __cdecl World_StepTerrainFloodFill(void)
{
    World_AdvanceTerrainFloodFillStep();
}

/*
 * Advances the world terrain traversal stack when its probe and global state permit it. Uses the
 * low three probe flag bits to choose an unvisited triangle edge, orders the next two edges through
 * World_TerrainFloodCompareEdgeMidpointDistances and visits neighbors through
 * World_TerrainFloodVisitEdgeNeighbor. Fully visited probes pop the stack. A negative border state
 * clears and pops; other unavailable conditions clear border state and report overflow when a stack
 * exists. Completion (WORLD_FLOOD_VISIT_DONE) or overflow stores the final stack position, clears the
 * active pointer and sets g_WorldSteeringCycle to 3 or 2 respectively, correcting one-past-capacity
 * position.
 */
// FUNCTION: FF7 0x0075749e
void __cdecl World_AdvanceTerrainFloodFillStep(void)
{
    struct {
        WorldTerrainTriangle *triangle;
        int result, edge;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.result = WORLD_FLOOD_VISIT_BLOCKED;
    if (g_pWorldTerrainFloodCursor && g_WorldTerrainFloodState == WORLD_FLOOD_CROSS_NONE && (g_pWorldSelectedTerrainTriangle == 0 || g_WorldTerrainFloodInRange)) {
        s.triangle = g_pWorldTerrainFloodCursor->triangle;
        g_pWorldSelectedTerrainTriangle = s.triangle;
        switch (g_pWorldTerrainFloodCursor->flags & WORLD_FLOOD_EDGES_ALL) {
        case WORLD_FLOOD_EDGES_NONE:
            s.edge = World_TerrainFloodChooseEdgeTowardFacing(g_pWorldTerrainFloodCursor);
            g_pWorldTerrainFloodCursor->flags |= s.edge;
            switch (s.edge) {
            case WORLD_FLOOD_EDGE_V0_V1:
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[0], s.triangle->vertices[1]);
                break;
            case WORLD_FLOOD_EDGE_V1_V2:
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[1], s.triangle->vertices[2]);
                break;
            case WORLD_FLOOD_EDGE_V2_V0:
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[2], s.triangle->vertices[0]);
                break;
            }
            break;
        case WORLD_FLOOD_EDGE_V1_V2 | WORLD_FLOOD_EDGE_V2_V0:
            g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGES_ALL;
            /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
            s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[0], s.triangle->vertices[1]);
            break;
        case WORLD_FLOOD_EDGE_V0_V1 | WORLD_FLOOD_EDGE_V2_V0:
            g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGES_ALL;
            /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
            s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[1], s.triangle->vertices[2]);
            break;
        case WORLD_FLOOD_EDGE_V0_V1 | WORLD_FLOOD_EDGE_V1_V2:
            g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGES_ALL;
            /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
            s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[2], s.triangle->vertices[0]);
            break;
        case WORLD_FLOOD_EDGE_V2_V0:
            /* Retail caller prototype (unsigned char, unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodCompareEdgeMidpointDistances`. */
            if (((int(__cdecl *)(unsigned char, unsigned char, unsigned char))World_TerrainFloodCompareEdgeMidpointDistances)(s.triangle->vertices[0], s.triangle->vertices[1], s.triangle->vertices[2])) {
                g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGE_V0_V1 | WORLD_FLOOD_EDGE_V2_V0;
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[0], s.triangle->vertices[1]);
            } else {
                g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGE_V1_V2 | WORLD_FLOOD_EDGE_V2_V0;
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[1], s.triangle->vertices[2]);
            }
            break;
        case WORLD_FLOOD_EDGE_V0_V1:
            /* Retail caller prototype (unsigned char, unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodCompareEdgeMidpointDistances`. */
            if (((int(__cdecl *)(unsigned char, unsigned char, unsigned char))World_TerrainFloodCompareEdgeMidpointDistances)(s.triangle->vertices[1], s.triangle->vertices[2], s.triangle->vertices[0])) {
                g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGE_V0_V1 | WORLD_FLOOD_EDGE_V1_V2;
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[1], s.triangle->vertices[2]);
            } else {
                g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGE_V0_V1 | WORLD_FLOOD_EDGE_V2_V0;
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[2], s.triangle->vertices[0]);
            }
            break;
        case WORLD_FLOOD_EDGE_V1_V2:
            /* Retail caller prototype (unsigned char, unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodCompareEdgeMidpointDistances`. */
            if (((int(__cdecl *)(unsigned char, unsigned char, unsigned char))World_TerrainFloodCompareEdgeMidpointDistances)(s.triangle->vertices[2], s.triangle->vertices[0], s.triangle->vertices[1])) {
                g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGE_V1_V2 | WORLD_FLOOD_EDGE_V2_V0;
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[2], s.triangle->vertices[0]);
            } else {
                g_pWorldTerrainFloodCursor->flags = WORLD_FLOOD_EDGE_V0_V1 | WORLD_FLOOD_EDGE_V1_V2;
                /* Retail caller prototype (unsigned char, unsigned char) -> int preserved for /Od argument promotion on `World_TerrainFloodVisitEdgeNeighbor`. */
                s.result = ((int(__cdecl *)(unsigned char, unsigned char))World_TerrainFloodVisitEdgeNeighbor)(s.triangle->vertices[0], s.triangle->vertices[1]);
            }
            break;
        case WORLD_FLOOD_EDGES_ALL:
            g_pWorldTerrainFloodCursor--;
            if (g_pWorldTerrainFloodCursor < (WorldTerrainProbe *)&g_WorldZoneFogNearRGB)
                s.result = WORLD_FLOOD_VISIT_DONE;
            break;
        }
    } else {
        if (g_WorldTerrainFloodState < 0) {
            g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_NONE;
            g_pWorldTerrainFloodCursor--;
            if (g_pWorldTerrainFloodCursor < (WorldTerrainProbe *)&g_WorldZoneFogNearRGB)
                s.result = WORLD_FLOOD_VISIT_DONE;
        } else {
            g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_NONE;
            if (g_pWorldTerrainFloodCursor)
                s.result = WORLD_FLOOD_VISIT_OVERFLOW;
        }
    }
    if (s.result == WORLD_FLOOD_VISIT_OVERFLOW || s.result == WORLD_FLOOD_VISIT_DONE) {
        g_WorldZoneOuterRadius = (int)g_pWorldTerrainFloodCursor;
        g_pWorldTerrainFloodCursor = 0;
        if ((WorldTerrainProbe *)g_WorldZoneOuterRadius >= (WorldTerrainProbe *)&g_WorldZoneData)
            g_WorldZoneOuterRadius -= sizeof(WorldTerrainProbe);
        g_WorldSteeringCycle = s.result != WORLD_FLOOD_VISIT_OVERFLOW ? 3 : 2;
    }
}

/*
 * Searches other triangles in the current block for the unordered edge identified by the low bytes
 * of vertexA and vertexB. An eligible unvisited match is marked and pushed onto the traversal stack
 * (PUSHED); an ineligible match is BLOCKED and a full stack is OVERFLOW. The stack pointer advances
 * and triangle is marked before its overflow check. Without a match and with room, detects block-
 * border edges, wraps X over 36 blocks or Z over 28, stores an opposite-border midpoint, sets the
 * crossing direction and returns CROSSED_BORDER; other edges are BLOCKED.
 */
// FUNCTION: FF7 0x0075783c
int __cdecl World_TerrainFloodVisitEdgeNeighbor(int vertexA, int vertexB)
{
    WorldTerrainTriangle *tri;
    int res;
    short xA;
    short bz;
    int edgeMatch;
    short xB;
    short vertexAz;

    edgeMatch = 0;
    for (tri = g_pWorldTerrainFloodCursor->chunk->triangles; tri < g_pWorldTerrainFloodCursor->chunk->triangles + g_pWorldTerrainFloodCursor->chunk->count && edgeMatch == 0; tri++) {
        if (tri != g_pWorldTerrainFloodCursor->triangle) {
            if (((vertexA & 0xFF) == tri->vertices[0] && (vertexB & 0xFF) == tri->vertices[1]) || ((vertexA & 0xFF) == tri->vertices[1] && (vertexB & 0xFF) == tri->vertices[0])) {
                edgeMatch = WORLD_FLOOD_EDGE_V0_V1;
            } else if (((vertexA & 0xFF) == tri->vertices[1] && (vertexB & 0xFF) == tri->vertices[2]) || ((vertexA & 0xFF) == tri->vertices[2] && (vertexB & 0xFF) == tri->vertices[1])) {
                edgeMatch = WORLD_FLOOD_EDGE_V1_V2;
            } else if (((vertexA & 0xFF) == tri->vertices[2] && (vertexB & 0xFF) == tri->vertices[0]) || ((vertexA & 0xFF) == tri->vertices[0] && (vertexB & 0xFF) == tri->vertices[2])) {
                edgeMatch = WORLD_FLOOD_EDGE_V2_V0;
            }
        }
    }
    tri--;
    if (edgeMatch != 0) {
        if ((tri->region & WORLD_TRIANGLE_FLOOD_VISITED) == 0 && (tri->terrain & WORLD_WALKMESH_REGION_MASK) != WORLD_WALKMESH_REGION_MASK && (WORLD_TERRAIN_MASK_CHOCOBO_DEFAULT >> (tri->terrain & WORLD_WALKMESH_TERRAIN_MASK) & 1) != 0) {
            tri->region |= WORLD_TRIANGLE_FLOOD_VISITED;
            g_pWorldTerrainFloodCursor++;
            if ((unsigned int)g_pWorldTerrainFloodCursor < (unsigned int)&g_WorldZoneData) {
                g_pWorldTerrainFloodCursor->chunk = (g_pWorldTerrainFloodCursor - 1)->chunk;
                g_pWorldTerrainFloodCursor->triangle = tri;
                g_pWorldTerrainFloodCursor->flags = edgeMatch;
                res = WORLD_FLOOD_VISIT_PUSHED;
            } else {
                res = WORLD_FLOOD_VISIT_OVERFLOW;
            }
        } else {
            res = WORLD_FLOOD_VISIT_BLOCKED;
        }
    } else if ((unsigned int)g_pWorldTerrainFloodCursor < (unsigned int)((WorldTerrainProbe *)&g_WorldZoneFogNearRGB + 39)) {
        xA = g_pWorldTerrainFloodCursor->chunk->vertices[vertexA & 0xFF].vx;
        vertexAz = g_pWorldTerrainFloodCursor->chunk->vertices[vertexA & 0xFF].vz;
        xB = g_pWorldTerrainFloodCursor->chunk->vertices[vertexB & 0xFF].vx;
        bz = g_pWorldTerrainFloodCursor->chunk->vertices[vertexB & 0xFF].vz;
        g_WorldTerrainFloodFinishReq = g_pWorldTerrainFloodCursor->chunk->wChunkX;
        g_WorldTerrainFloodMeshRow = g_pWorldTerrainFloodCursor->chunk->wChunkY;
        g_WorldSkyVertexQuad.posY = 0;
        res = WORLD_FLOOD_VISIT_CROSSED_BORDER;
        if (xA < 10 && xB < 10) {
            g_WorldTerrainFloodFinishReq -= 1;
            if (g_WorldTerrainFloodFinishReq < 0) {
                g_WorldTerrainFloodFinishReq = 0x23;
            }
            g_WorldSkyVertexQuad.posX = 8182;
            g_WorldSkyVertexQuad.posZ = vertexAz + bz >> 1;
            g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_NEG_X;
        } else if (vertexAz < 10 && bz < 10) {
            g_WorldTerrainFloodMeshRow -= 1;
            if (g_WorldTerrainFloodMeshRow < 0) {
                g_WorldTerrainFloodMeshRow = 27;
            }
            g_WorldSkyVertexQuad.posX = xA + xB >> 1;
            g_WorldSkyVertexQuad.posZ = 8182;
            g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_NEG_Z;
        } else if (xA > 8182 && xB > 8182) {
            g_WorldTerrainFloodFinishReq += 1;
            if (g_WorldTerrainFloodFinishReq >= 36) {
                g_WorldTerrainFloodFinishReq = 0;
            }
            g_WorldSkyVertexQuad.posX = 10;
            g_WorldSkyVertexQuad.posZ = vertexAz + bz >> 1;
            g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_POS_X;
        } else if (vertexAz > 8182 && bz > 8182) {
            g_WorldTerrainFloodMeshRow += 1;
            if (g_WorldTerrainFloodMeshRow >= 28) {
                g_WorldTerrainFloodMeshRow = 0;
            }
            g_WorldSkyVertexQuad.posX = xA + xB >> 1;
            g_WorldSkyVertexQuad.posZ = 10;
            g_WorldTerrainFloodState = WORLD_FLOOD_CROSS_POS_Z;
        } else {
            res = WORLD_FLOOD_VISIT_BLOCKED;
        }
    } else {
        res = WORLD_FLOOD_VISIT_OVERFLOW;
    }
    return res;
}

/*
 * Compares Manhattan distances from a target to the midpoints of edges (vertexA,vertexB) and
 * (vertexB,vertexC) in the current flood-fill mesh. Masks vertex indices to bytes, averages short
 * X/Z with arithmetic shifts and adds mesh-coordinate offsets shifted left by 13. When
 * g_WorldTerrainFloodInRange is set uses target (0x3A681,0x2195F) and returns whether the first
 * edge is closer. Otherwise retrieves player position through a native contiguous local block and
 * returns whether the first edge is farther. Exact player-coordinate local aliasing depends on the
 * native frame convention.
 */
// FUNCTION: FF7 0x00757cd4
int __cdecl World_TerrainFloodCompareEdgeMidpointDistances(int vertexA, int vertexB, int vertexC)
{
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int refX[2];
    int absDxA8;
    SVECTOR *vertexTable;
    int srcAbsDxB;
    int absDzA;
    int basePlayerXGap;
    int playerZGap1;
    int playerDistX1;
    int mainDistZ;
    int newAbsPlayerDz;
    int positionZ;

    vertexTable = g_pWorldTerrainFloodCursor->chunk->vertices;
    if (g_WorldTerrainFloodInRange != 0) {
        refX[0] = 0x3A681;
        positionZ = 0x2195F;
        if (refX[0] - (vertexTable[vertexA & 0xFF].vx + vertexTable[vertexB & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13) > 0) {
            absDxA8 = refX[0] - (vertexTable[vertexA & 0xFF].vx + vertexTable[vertexB & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13);
        } else {
            absDxA8 = -(refX[0] - (vertexTable[vertexA & 0xFF].vx + vertexTable[vertexB & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13));
        }
        if (positionZ - (vertexTable[vertexA & 0xFF].vz + vertexTable[vertexB & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13) > 0) {
            absDzA = positionZ - (vertexTable[vertexA & 0xFF].vz + vertexTable[vertexB & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13);
        } else {
            absDzA = -(positionZ - (vertexTable[vertexA & 0xFF].vz + vertexTable[vertexB & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13));
        }
        if (refX[0] - (vertexTable[vertexB & 0xFF].vx + vertexTable[vertexC & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13) > 0) {
            srcAbsDxB = refX[0] - (vertexTable[vertexB & 0xFF].vx + vertexTable[vertexC & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13);
        } else {
            srcAbsDxB = -(refX[0] - (vertexTable[vertexB & 0xFF].vx + vertexTable[vertexC & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13));
        }
        if (positionZ - (vertexTable[vertexB & 0xFF].vz + vertexTable[vertexC & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13) > 0) {
            mainDistZ = positionZ - (vertexTable[vertexB & 0xFF].vz + vertexTable[vertexC & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13);
        } else {
            mainDistZ = -(positionZ - (vertexTable[vertexB & 0xFF].vz + vertexTable[vertexC & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13));
        }
        return absDxA8 + absDzA < srcAbsDxB + mainDistZ;
    } else {
        WorldEntity_GetPlayerPosition((VECTOR *)refX);
        if (refX[0] - (vertexTable[vertexA & 0xFF].vx + vertexTable[vertexB & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13) > 0) {
            playerDistX1 = refX[0] - (vertexTable[vertexA & 0xFF].vx + vertexTable[vertexB & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13);
        } else {
            playerDistX1 = -(refX[0] - (vertexTable[vertexA & 0xFF].vx + vertexTable[vertexB & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13));
        }
        if (positionZ - (vertexTable[vertexA & 0xFF].vz + vertexTable[vertexB & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13) > 0) {
            playerZGap1 = positionZ - (vertexTable[vertexA & 0xFF].vz + vertexTable[vertexB & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13);
        } else {
            playerZGap1 = -(positionZ - (vertexTable[vertexA & 0xFF].vz + vertexTable[vertexB & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13));
        }
        if (refX[0] - (vertexTable[vertexB & 0xFF].vx + vertexTable[vertexC & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13) > 0) {
            basePlayerXGap = refX[0] - (vertexTable[vertexB & 0xFF].vx + vertexTable[vertexC & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13);
        } else {
            basePlayerXGap = -(refX[0] - (vertexTable[vertexB & 0xFF].vx + vertexTable[vertexC & 0xFF].vx >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkX << 13));
        }
        if (positionZ - (vertexTable[vertexB & 0xFF].vz + vertexTable[vertexC & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13) > 0) {
            newAbsPlayerDz = positionZ - (vertexTable[vertexB & 0xFF].vz + vertexTable[vertexC & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13);
        } else {
            newAbsPlayerDz = -(positionZ - (vertexTable[vertexB & 0xFF].vz + vertexTable[vertexC & 0xFF].vz >> 1) - (g_pWorldTerrainFloodCursor->chunk->wChunkY << 13));
        }
        return playerDistX1 + playerZGap1 > basePlayerXGap + newAbsPlayerDz;
    }
}

/*
 * Computes the triangle centroid and compares the direction of each edge midpoint with the current
 * facing, folding angle differences into the half-turn range. When g_WorldTerrainFloodInRange is set, derives
 * facing toward the fixed world point instead. Returns the edge bit (WorldTerrainFloodEdge) with the smallest
 * difference, retaining the earlier edge on ties.
 */
// FUNCTION: FF7 0x00758313
int __cdecl World_TerrainFloodChooseEdgeTowardFacing(WorldTerrainProbe *probe)
{
    struct {
        VECTOR centre;
        int bestAngle, facing;
        VECTOR world;
        SVECTOR *vertices;
        WorldTerrainTriangle *triangle;
        int edge;
        VECTOR midpoint;
        int angle;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.triangle = probe->triangle;
    s.vertices = probe->chunk->vertices;
    s.edge = WORLD_FLOOD_EDGE_V0_V1;
    s.facing = World_GetCameraRotationY();
    s.centre.vx = (s.vertices[s.triangle->vertices[0]].vx + s.vertices[s.triangle->vertices[1]].vx + s.vertices[s.triangle->vertices[2]].vx) / 3;
    s.centre.vz = (s.vertices[s.triangle->vertices[0]].vz + s.vertices[s.triangle->vertices[1]].vz + s.vertices[s.triangle->vertices[2]].vz) / 3;
    if (g_WorldTerrainFloodInRange) {
        s.world.vx = s.centre.vx + (probe->chunk->wChunkX << 13);
        s.world.vz = s.centre.vz + (probe->chunk->wChunkY << 13);
        s.midpoint.vx = 0x3a681;
        s.midpoint.vz = 0x2195f;
        s.facing = WorldMap_AngleBetweenPositions(&s.world, &s.midpoint);
    } else {
        s.facing = World_GetCameraRotationY();
    }
    s.midpoint.vx = (s.vertices[s.triangle->vertices[0]].vx + s.vertices[s.triangle->vertices[1]].vx) >> 1;
    s.midpoint.vz = (s.vertices[s.triangle->vertices[0]].vz + s.vertices[s.triangle->vertices[1]].vz) >> 1;
    s.bestAngle = (WorldMap_AngleBetweenPositions(&s.centre, &s.midpoint) - s.facing) & 0xfff;
    if (s.bestAngle >= 0x800)
        s.bestAngle = 0x1000 - s.bestAngle;
    s.midpoint.vx = (s.vertices[s.triangle->vertices[1]].vx + s.vertices[s.triangle->vertices[2]].vx) >> 1;
    s.midpoint.vz = (s.vertices[s.triangle->vertices[1]].vz + s.vertices[s.triangle->vertices[2]].vz) >> 1;
    s.angle = (WorldMap_AngleBetweenPositions(&s.centre, &s.midpoint) - s.facing) & 0xfff;
    if (s.angle >= 0x800)
        s.angle = 0x1000 - s.angle;
    if (s.bestAngle > s.angle) {
        s.bestAngle = s.angle;
        s.edge = WORLD_FLOOD_EDGE_V1_V2;
    }
    s.midpoint.vx = (s.vertices[s.triangle->vertices[2]].vx + s.vertices[s.triangle->vertices[0]].vx) >> 1;
    s.midpoint.vz = (s.vertices[s.triangle->vertices[2]].vz + s.vertices[s.triangle->vertices[0]].vz) >> 1;
    s.angle = (WorldMap_AngleBetweenPositions(&s.centre, &s.midpoint) - s.facing) & 0xfff;
    if (s.angle >= 0x800)
        s.angle = 0x1000 - s.angle;
    if (s.bestAngle > s.angle)
        s.edge = WORLD_FLOOD_EDGE_V2_V0;
    return s.edge;
}

/*
 * Processes three enabled world path-following entities. Selects each actor, initializes a
 * zero-position actor from its path when flag 4 is set, otherwise advances to movement state. Flag
 * 1 selects the next or previous 12-byte path record from direction bit 2, optionally averages with
 * a neighbor and derives a heading. Moves 120 units along that heading, compares pre-move Manhattan
 * distance against the prior value, and either requests another record or clears state at path
 * boundaries. The optional stop region within distance 2000 of (0x3A681,0x2195F) also clears state.
 * Calls World_FinishTerrainEffectSlot on completion and conditionally restores player control for
 * mode 2.
 */
// FUNCTION: FF7 0x007585b7
void __cdecl World_UpdateTerrainFloodPathFollowers(void)
{
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int distance;
    int position[3];
    int followerIdx;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short stepVec[4];
    int neighborPosition[3];

    for (followerIdx = 0; followerIdx < 3; followerIdx++) {
        if (g_WorldTerrainFloodCrossing.slot[followerIdx] != 0) {
            WorldEntity_SetCurrentById(g_WorldSkyHorizonOffset == 3 ? 4 : (followerIdx == 1 ? 0x29 : (followerIdx == 2 ? 0x2A : 0x26)));
            WorldEntity_GetCurrentPosition((VECTOR *)position);
            if ((g_WorldTerrainFloodCrossing.slot[followerIdx] & 4) != 0) {
                if (position[0] == 0 && position[2] == 0) {
                    World_GetProbeTriangleCenterXZ(g_WorldRandomSeeds[followerIdx], position);
                    WorldEntity_SetCurrentPositionAndGround(position);
                } else {
                    g_WorldTerrainFloodCrossing.slot[followerIdx] += 4;
                }
            } else if ((g_WorldTerrainFloodCrossing.slot[followerIdx] & 8) != 0) {
                if ((g_WorldTerrainFloodCrossing.slot[followerIdx] & 1) != 0) {
                    g_WorldTerrainFloodCrossing.slot[followerIdx] &= 0xFFFFFFFE;
                    g_WorldRandomSeeds[followerIdx] += ((g_WorldTerrainFloodCrossing.slot[followerIdx] & 2) - 1) * 12;
                    World_GetProbeTriangleCenterXZ(g_WorldRandomSeeds[followerIdx], &g_WorldSkySprites[followerIdx]);
                    if ((unsigned int)g_WorldRandomSeeds[followerIdx] > (unsigned int)&g_WorldZoneFogNearRGB && (unsigned int)g_WorldRandomSeeds[followerIdx] < (unsigned int)g_WorldZoneOuterRadius) {
                        World_GetProbeTriangleCenterXZ((int)((WorldTerrainProbe *)g_WorldRandomSeeds[followerIdx] + (g_WorldTerrainFloodCrossing.slot[followerIdx] & 2) - 1), neighborPosition);
                        g_WorldSkySprites[followerIdx].vx = neighborPosition[0] + g_WorldSkySprites[followerIdx].vx >> 1;
                        g_WorldSkySprites[followerIdx].vz = neighborPosition[2] + g_WorldSkySprites[followerIdx].vz >> 1;
                    }
                    g_WorldTerrainCurrentBytes[followerIdx] = WorldMap_AngleBetweenPositions(position, &g_WorldSkySprites[followerIdx]);
                    g_WorldSnakeDataPositionEnd[followerIdx] = 0x7FFF;
                }
                stepVec[0] = stepVec[1] = 0;
                stepVec[2] = 120;
                WorldEntity_SetCurrentFacingClearTurn(g_WorldTerrainCurrentBytes[followerIdx]);
                WorldMap_RotateVec3ByYaw((SVECTOR *)stepVec, g_WorldTerrainCurrentBytes[followerIdx]);
                WorldEntity_MoveCurrentBy(stepVec[0], stepVec[2]);
                distance = (position[0] - g_WorldSkySprites[followerIdx].vx > 0 ? position[0] - g_WorldSkySprites[followerIdx].vx : -(position[0] - g_WorldSkySprites[followerIdx].vx)) + (position[2] - g_WorldSkySprites[followerIdx].vz > 0 ? position[2] - g_WorldSkySprites[followerIdx].vz : -(position[2] - g_WorldSkySprites[followerIdx].vz));
                if (distance > g_WorldSnakeDataPositionEnd[followerIdx]) {
                    g_WorldTerrainFloodCrossing.slot[followerIdx] = (g_WorldTerrainFloodCrossing.slot[followerIdx] & 2) != 0 ? ((unsigned int)g_WorldRandomSeeds[followerIdx] >= (unsigned int)g_WorldZoneOuterRadius ? 0 : g_WorldTerrainFloodCrossing.slot[followerIdx] | 1) : ((unsigned int)g_WorldRandomSeeds[followerIdx] <= (unsigned int)(followerIdx * 0xC + (followerIdx > 0) * 0xC + (int)&g_WorldZoneFogNearRGB) ? 0 : g_WorldTerrainFloodCrossing.slot[followerIdx] | 1);
                }
                g_WorldSnakeDataPositionEnd[followerIdx] = distance;
                if (g_WorldTerrainFloodInRange != 0) {
                    if ((position[0] - 239233 > 0 ? position[0] - 0x3A681 : -(position[0] - 0x3A681)) + (position[2] - 137567 > 0 ? position[2] - 0x2195F : -(position[2] - 0x2195F)) < 2000) {
                        g_WorldTerrainFloodCrossing.slot[followerIdx] = 0;
                    }
                }
                if (g_WorldTerrainFloodCrossing.slot[followerIdx] == 0) {
                    World_FinishTerrainEffectSlot(followerIdx);
                    if (g_WorldSkyHorizonOffset == 2) {
                        if (followerIdx == 2 || (followerIdx == 1 && WorldEntity_SetCurrentById(0x2A) == 0)) {
                            World_SetCameraZoomState(0);
                            WorldMap_SetPlayerControl(1, 5);
                        }
                    }
                }
            }
        }
    }
}

/*
 * Sets the terrain effect kind g_WorldSkyHorizonOffset.
 */
// FUNCTION: FF7 0x00758a32
void __cdecl World_SetTerrainEffectKind(int value)
{
    g_WorldSkyHorizonOffset = value;
}

/*
 * Returns 1 for g_WorldSteeringCycle==2, -1 for value 3 or zero for any other value.
 */
// FUNCTION: FF7 0x00758a3f
int __cdecl World_GetTerrainFloodFillOutcome(void)
{
    return (g_WorldSteeringCycle == 2) ? 1 : (g_WorldSteeringCycle == 3) ? -1
                                                         : 0;
}

/*
 * Returns whether any of the three terrain effect timers g_WorldTerrainFloodCrossing/44/48 is running.
 */
// FUNCTION: FF7 0x00758a6c
int __cdecl World_IsTerrainEffectActive(void)
{
    int val;

    if ((g_WorldTerrainFloodCrossing.floodTimer) == 0 && g_WorldTerrainFloodCrossing.isBusy == 0 && g_WorldTerrainFloodCrossing.effectTimer == 0) {
        val = 0;
    } else {
        val = 1;
    }
    return val;
}

/*
 * Requests the terrain effect to finish (g_WorldTerrainFloodFinishRequested = 1), resetting the state to 0 unless a fill
 * is in progress (state 1).
 */
// FUNCTION: FF7 0x00758aa2
void __cdecl World_RequestTerrainFloodFinish(void)
{
    g_WorldTerrainFloodFinishRequested = 1;
    if (g_WorldSteeringCycle != 1) {
        g_WorldSteeringCycle = 0;
    }
}

/*
 * Sets the steering cycle's next entity kind index g_WorldSteeringEntityIndex and clears the cycle's saved facing
 * g_WorldSteeringFacing, g_WorldSteeringCycleLimit, g_WorldSteeringTarget, g_WorldSteeringCountdown and g_WorldSteeringTurn.
 */
// FUNCTION: FF7 0x00758ac4
void __cdecl World_ResetSteeringCycleAndSetSpawnIndex(int spawnIndex)
{
    g_WorldSteeringEntityIndex = spawnIndex;
    g_WorldSteeringFacing = 0;
    g_WorldSteeringCycleLimit = g_WorldSteeringFacing;
    g_WorldSteeringTarget = g_WorldSteeringCycleLimit;
    g_WorldSteeringCountdown = g_WorldSteeringTarget;
    g_WorldSteeringTurn = g_WorldSteeringCountdown;
}

/*
 * Updates a countdown-driven steering cycle, saving and applying current facing around the world
 * yaw and choosing new random duration, threshold and signed steering when the countdown expires.
 * During the upper part of the cycle, a trigger selects a cycling entity type, positions it 300
 * units along a rotated vector from the player, and updates its facing/state. Returns steering
 * below the threshold and zero otherwise.
 */
// FUNCTION: FF7 0x00758b12
int __cdecl World_UpdateRandomSteeringCycleAndSpawnEntity(int dwMove, int dwTrigger)
{
    struct {
        VECTOR spawnPos;
        short spawnOffset[4];
        short wAngle;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.wAngle = World_GetCameraRotationY();
    if (g_WorldSteeringCountdown == g_WorldSteeringTarget) {
        g_WorldSteeringFacing = WorldEntity_GetCurrentFacingPlusTurn() + frame.wAngle;
        World_ShowOverlayChannelTimed(1);
        World_ShowOverlayChannelTimed(2);
    } else if (g_WorldSteeringCountdown < g_WorldSteeringTarget) {
        WorldEntity_SetCurrentFacing(g_WorldSteeringFacing - frame.wAngle);
    }
    g_WorldSteeringCountdown -= (dwMove == 0 && g_WorldSteeringCountdown >= g_WorldSteeringTarget) ? 0 : 1;
    if (g_WorldSteeringCountdown <= 0) {
        g_WorldSteeringCycleLimit = WorldMap_NextRandomByte() + 128;
        g_WorldSteeringCountdown = g_WorldSteeringCycleLimit;
        g_WorldSteeringTarget = (WorldMap_NextRandomByte() >> 3) + 0x40;
        g_WorldSteeringTurn = ((WorldMap_NextRandomByte() & 2) - 1) << 4;
    }
    if (g_WorldSteeringCountdown >= g_WorldSteeringTarget && dwTrigger) {
        if (!WorldEntity_SetCurrentById(g_WorldSteeringEntityIndex + 0x15)) {
            WorldEntity_Alloc();
            WorldEntity_SetupForModel(g_WorldSteeringEntityIndex + 0x15);
        }
        g_WorldSteeringEntityIndex = (g_WorldSteeringEntityIndex + 1) % 3;
        frame.spawnOffset[0] = frame.spawnOffset[1] = 0;
        frame.spawnOffset[2] = 300;
        WorldMap_RotateVec3ByYaw((SVECTOR *)frame.spawnOffset, -frame.wAngle);
        WorldEntity_GetPlayerPosition(&frame.spawnPos);
        frame.spawnPos.vx += frame.spawnOffset[0];
        frame.spawnPos.vz += frame.spawnOffset[2];
        WorldEntity_SetCurrentPositionAndGround(&frame.spawnPos);
        WorldEntity_SnapCurrentFacing(-frame.wAngle);
        WorldEntity_SelectPlayerAsCurrentActor();
    }
    return (g_WorldSteeringCountdown < g_WorldSteeringTarget) ? g_WorldSteeringTurn : 0;
}

/*
 * Returns the steering cycle's next entity kind index g_WorldSteeringEntityIndex (0..2; the entity id is it plus 0x15).
 */
// FUNCTION: FF7 0x00758d0b
int __cdecl World_GetSteeringSpawnIndex(void)
{
    return g_WorldSteeringEntityIndex;
}

/*
 * Unpacks a dword into the four terrain-current bytes: the strengths of the three records of
 * g_WorldZolomSegments and g_WorldTerrainCurrentByte3, and clears
 * the drift accumulators g_WorldTerrainCurrentX/40.
 */
// FUNCTION: FF7 0x00758d17
void __cdecl World_UnpackTerrainCurrentBytes(unsigned int packedBytes)
{
    g_WorldZolomSegments.records[0].strength = packedBytes & 0xFF;
    g_WorldZolomSegments.records[1].strength = packedBytes >> 8 & 0xFF;
    g_WorldZolomSegments.records[2].strength = packedBytes >> 0x10 & 0xFF;
    g_WorldTerrainCurrentByte3 = packedBytes >> 0x18 & 0xFF;
    g_WorldTerrainCurrentZ = 0;
    g_WorldTerrainCurrentX = g_WorldTerrainCurrentZ;
}

/*
 * Packs the four terrain-current bytes back into one dword, the inverse of World_UnpackTerrainCurrentBytes.
 */
// FUNCTION: FF7 0x00758d76
int __cdecl World_PackTerrainCurrentBytes(void)
{
    return g_WorldZolomSegments.records[0].strength | g_WorldZolomSegments.records[1].strength << 8 | g_WorldZolomSegments.records[2].strength << 0x10 | g_WorldTerrainCurrentByte3 << 24;
}

/*
 * For a terrain-current record with nonzero strength, advances a 12-bit phase and installs a yaw
 * and sinusoidal roll transform around height 20000. Transforms up to twelve indexed terrain
 * vertices, stopping at a negative index, adding transformed X and Z and replacing Y. Gets the
 * player mesh position: one movement mode sets a fixed length 40 vector toward the current center;
 * otherwise attenuates strength by Manhattan distance within 3500, uses a cosine roll transform
 * and stores transformed X and Z shifted right by four. Clears both movement globals when no
 * active current record exists.
 */
// FUNCTION: FF7 0x00758da5
void __cdecl World_ApplyTerrainCurrent(short chunkX, short chunkY)
{
    struct FloatVector {
        float x, y, z;
    };
    struct {
        SVECTOR movement;
        struct FloatVector *vertex;
        VECTOR transformed;
        PsxMatrix32 matrix;
        signed char *index;
        SVECTOR local;
        VECTOR position;
        int strength, phase;
        SVECTOR rotation;
        int flags;
        WorldTerrainCurrentRecord *bridge;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.bridge = World_FindTerrainCurrentRecord(chunkX, chunkY);
    if (s.bridge && s.bridge->strength) {
        s.phase = (g_WorldTerrainCurrentByte3 << 6) & 0xfff;
        g_WorldTerrainCurrentByte3++;
        s.rotation.vx = 0;
        s.rotation.vy = s.bridge->angle;
        s.rotation.vz = (s.bridge->strength * SinFixed(s.phase)) >> 17;
        RotMatrixXYZ(&s.rotation, &s.matrix);
        SetRotMatrix(&s.matrix);
        s.position.vx = s.position.vz = 0;
        s.position.vy = 20000;
        SetMatrixTranslation(&s.matrix, &s.position);
        SetTransMatrix(&s.matrix);
        for (s.index = &s.bridge->vertices[0]; s.index < &s.bridge->vertices[12] && *s.index >= 0; s.index++) {
            s.vertex = &((struct FloatVector *)((unsigned char *)&g_WorldMeshVertices))[*s.index];
            s.local.vx = 0;
            s.local.vy = (short)s.vertex->y - 20000;
            s.local.vz = 0;
            RotTransPoint(&s.local, (int *)&s.position, &s.flags);
            s.vertex->x += (float)s.position.vx;
            s.vertex->y = (float)s.position.vy;
            s.vertex->z += (float)s.position.vz;
        }
        WorldEntity_GetPlayerPosition((VECTOR *)&s.position);
        WorldMap_PositionToMesh((int *)&s.position, &s.local, 0, 0);
        if (World_GetTerrainBlockCountdown()) {
            s.movement.vx = 0;
            s.movement.vy = 0;
            s.movement.vz = 40;
            s.movement.pad = 0;
            WorldMap_RotateVec3ByYaw((SVECTOR *)&s.movement, WorldMap_ArcTangentFromDeltas(s.bridge->x - s.local.vx, s.bridge->y - s.local.vz));
            g_WorldTerrainCurrentX = s.movement.vx;
            g_WorldTerrainCurrentZ = s.movement.vz;
        } else {
            s.strength = (s.bridge->strength * (0 > 3500 - (s.local.vx - s.bridge->x > 0 ? s.local.vx - s.bridge->x : -(s.local.vx - s.bridge->x)) - (s.local.vz - s.bridge->y > 0 ? s.local.vz - s.bridge->y : -(s.local.vz - s.bridge->y)) ? 0 : 3500 - (s.local.vx - s.bridge->x > 0 ? s.local.vx - s.bridge->x : -(s.local.vx - s.bridge->x)) - (s.local.vz - s.bridge->y > 0 ? s.local.vz - s.bridge->y : -(s.local.vz - s.bridge->y)))) >> 12;
            s.rotation.vx = 0;
            s.rotation.vy = s.bridge->angle;
            s.rotation.vz = (s.strength * CosFixed(s.phase)) >> 16;
            RotMatrixXYZ(&s.rotation, &s.matrix);
            SetRotMatrix(&s.matrix);
            s.local.vx = 0;
            s.local.vy = s.position.vy - 20000;
            s.local.vz = 0;
            RotTransPoint(&s.local, (int *)&s.transformed, &s.flags);
            g_WorldTerrainCurrentX = s.transformed.vx >> 4;
            g_WorldTerrainCurrentZ = s.transformed.vz >> 4;
        }
    } else {
        g_WorldTerrainCurrentX = g_WorldTerrainCurrentZ = 0;
    }
}

/*
 * Returns the terrain-current record for a mesh cell: (5, 0x12) is records[0] of g_WorldZolomSegments, (4, 0x11) is
 * g_WorldZolomSegments.records[1], (4, 0xE) is records[2]; any other cell has none.
 */
// FUNCTION: FF7 0x00759151
WorldTerrainCurrentRecord *__cdecl World_FindTerrainCurrentRecord(short column, short row)
{
    WorldTerrainCurrentRecord *innerRecord;
    WorldTerrainCurrentRecord *result;
    WorldTerrainCurrentRecord *fallback;

    if (column == 5 && row == 18) {
        result = g_WorldZolomSegments.records;
    } else {
        if (column == 4 && row == 17) {
            innerRecord = &g_WorldZolomSegments.records[1];
        } else {
            if (column == 4 && row == 14) {
                fallback = &g_WorldZolomSegments.records[2];
            } else {
                fallback = 0;
            }
            innerRecord = fallback;
        }
        result = innerRecord;
    }
    return result;
}

/*
 * Adds the drift accumulators g_WorldTerrainCurrentX and g_WorldTerrainCurrentZ to *xPtr and *zPtr, each when the
 * pointer is non-null.
 */
// FUNCTION: FF7 0x007591c2
void __cdecl World_AddTerrainCurrentDrift(void *xPtr, void *zPtr)
{
    if (xPtr != 0) {
        *(int *)xPtr += g_WorldTerrainCurrentX;
    }
    if (zPtr != 0) {
        *(int *)zPtr += g_WorldTerrainCurrentZ;
    }
}

/*
 * On terrain type 0xE with any direction bit held, locates the player mesh current record and adds
 * 10 to strength +6 when it is below 100; this can overshoot 100. Dispatches sound 0xCB even if no
 * record is found. Then walks three 0x14-byte current records, decrementing each positive strength
 * by one and dispatching -0xCB whenever the resulting strength equals 1.
 */
// FUNCTION: FF7 0x007591f3
void __cdecl World_UpdateTerrainCurrentStrengths(void)
{
    int posBuffer[4];
    WorldTerrainCurrentRecord *current;
    int meshCol;
    int meshRow;

    if (WorldEntity_GetPlayerTerrainType() == WORLD_TERRAIN_WUTAI_BRIDGE) {
        if ((Input_GetHeldRaw() & 0xF000) != 0) {
            WorldEntity_GetPlayerPosition((VECTOR *)posBuffer);
            WorldMap_PositionToMesh(posBuffer, 0, &meshCol, &meshRow);
            current = World_FindTerrainCurrentRecord((short)meshCol, (short)meshRow);
            if (current != 0 && current->strength < 100) {
                current->strength += 0xA;
            }
            World_RequestAmbienceSfx(0xCB);
        }
    }
    for (current = g_WorldZolomSegments.records; current < (WorldTerrainCurrentRecord *)&g_WorldZolomSegments.end; current++) {
        current->strength -= (current->strength > 0);
        if (current->strength == 1) {
            World_RequestAmbienceSfx(0xFFFFFF35);
        }
    }
}
} /* extern "C" */

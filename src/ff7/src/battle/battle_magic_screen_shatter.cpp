// src/ff7/src/battle/battle_magic_screen_shatter.cpp
/* Magic Effect [25]: Escape & Screen Shatter Transition (Japanese: エスケプ) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The screen-shatter magic effect: it sets up the offscreen requests and a tile grid,
//   then runs a timeline that stages the shatter and flags the actors involved.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/enums/main.h"
#include "lib/enums/graphics.h"
extern "C" {



// clang-format off
// The two offscreen slots (-1 when free) and the scale vector (0x4000, 0x4000, 0x4000) the TU reads by address.
// GLOBAL: FF7 0x008ff0e8
struct BattleFxOffscreenSlotPair g_BattleMagicScreenShatter_OffscreenSlots = {-1, -1};
// GLOBAL: FF7 0x008ff0f0
int g_BattleMagicScreenShatter_ScaleVector[4] = {16384, 16384, 16384, 0};
// The deformed grid: 22 rows of 41 vertices around the screen centre (160, 120), read as
// BattleMagicScreenShatterGridRow rows.
// GLOBAL: FF7 0x00c1fa18
BattleMagicScreenShatter_GridRecords g_BattleMagicScreenShatter_GridRecords = {0};
// The 840 tiles, 0x80 bytes each: two textured quads of 0x28 bytes (g_BattleMagicScreenShatter_PacketBank
// picks one) and the tile's motion record. The definitions up to the tile grid are the first tile's
// quad fields; the code reads each field of every tile with the 0x80-byte stride.
// GLOBAL: FF7 0x00c22460
int g_BattleScreenShatterTiles = 0;
// GLOBAL: FF7 0x00c22464
BattleMagicScreenShatterQuadColor g_BattleMagicScreenShatter_Quad0Color = {0};
// GLOBAL: FF7 0x00c22468
int g_BattleMagicScreenShatter_Quad0Xy0 = 0;
// GLOBAL: FF7 0x00c2246c
BattleMagicScreenShatterQuadUv g_BattleMagicScreenShatter_Quad0Uv0 = {0};
// GLOBAL: FF7 0x00c22470
int g_BattleMagicScreenShatter_Quad0Xy1 = 0;
// GLOBAL: FF7 0x00c22474
BattleMagicScreenShatterQuadUv g_BattleMagicScreenShatter_Quad0Uv1 = {0};
// GLOBAL: FF7 0x00c22478
int g_BattleMagicScreenShatter_Quad0Xy2 = 0;
// GLOBAL: FF7 0x00c2247c
BattleMagicScreenShatterQuadUv g_BattleMagicScreenShatter_Quad0Uv2 = {0};
// GLOBAL: FF7 0x00c22480
int g_BattleMagicScreenShatter_Quad0Xy3 = 0;
// GLOBAL: FF7 0x00c22484
BattleMagicScreenShatterQuadUv g_BattleMagicScreenShatter_Quad0Uv3 = {0};
// GLOBAL: FF7 0x00c22488
int g_BattleMagicScreenShatter_Quad1Header = 0;
// GLOBAL: FF7 0x00c2248c
BattleMagicScreenShatterQuadColor g_BattleMagicScreenShatter_Quad1Color = {0};
unsigned char g_bssPad_00c22490[4] = {0};
// GLOBAL: FF7 0x00c22494
BattleMagicScreenShatterQuadUv g_BattleMagicScreenShatter_Quad1Uv0 = {0};
unsigned char g_bssPad_00c22498[4] = {0};
// GLOBAL: FF7 0x00c2249c
BattleMagicScreenShatterQuadUv g_BattleMagicScreenShatter_Quad1Uv1 = {0};
unsigned char g_bssPad_00c224a0[4] = {0};
// GLOBAL: FF7 0x00c224a4
BattleMagicScreenShatterQuadUv g_BattleMagicScreenShatter_Quad1Uv2 = {0};
unsigned char g_bssPad_00c224a8[4] = {0};
// GLOBAL: FF7 0x00c224ac
BattleMagicScreenShatterQuadUv g_BattleMagicScreenShatter_Quad1Uv3 = {0};
// GLOBAL: FF7 0x00c224b0
BattleMagicScreenShatter_TileGrid g_BattleMagicScreenShatterTileGrid = {0};
unsigned char g_bssPad_00c3c840[32] = {0};
// GLOBAL: FF7 0x00c3c860
int g_BattleMagicScreenShatter_WaveAmplitude = 0;
unsigned char g_bssPad_00c3c864[4] = {0};
// The grid scaled to the screen; the quads' corners are copied from it. Its rows run on into the
// padding after this object.
// GLOBAL: FF7 0x00c3c868
BattleMagicScreenShatter_ScreenGrid g_BattleMagicScreenShatter_ScreenGrid = {0};
unsigned char g_bssPad_00c3ca50[10336] = {0};
// GLOBAL: FF7 0x00c3f2b0
int g_BattleMagicScreenShatter_PacketBank = 0;
unsigned char g_bssPad_00c3f2b4[4] = {0};
// clang-format on

/* The tile array is defined as the first tile's fields, so a field of tile i is reached from the
 * first tile's field with the 0x80-byte tile stride; a grid row from the grid's first vertex. */
#define SHATTER_HEADER(field, tile) (((BattleMagicScreenShatterTileHeader *)&(field))[tile])
#define SHATTER_COLOR(field, tile) (((BattleMagicScreenShatterTileColor *)&(field))[tile])
#define SHATTER_UV(field, tile) (((BattleMagicScreenShatterTileUv *)&(field))[tile])
#define SHATTER_GRID_ROW(grid, row) (((BattleMagicScreenShatterGridRow *)&(grid))[row])

/*
 * Magic effect 0x25: clears the wave amplitude and starts the screen shatter on the targets.
 */
// FUNCTION: FF7 0x005d5720
void __cdecl BattleFx_MagicEnterEscape(unsigned int targetMask, int actingActor)
{
    g_BattleMagicScreenShatter_WaveAmplitude = 0;
    BattleFx_MagicBeginScreenShatter(targetMask, actingActor);
}

/*
 * Builds the tile grid, captures the screen into two offscreen requests, starts the timeline and
 * the actor-flag tasks and plays sound 0x5D at pan 0x40. Neither argument is read.
 */
// FUNCTION: FF7 0x005d573f
void BattleFx_MagicBeginScreenShatter(unsigned int targetMask, int actingActor)
{
    int unused[2];

    BattleFx_MagicScreenShatterInitTileGrid();
    BattleFx_MagicScreenShatterAcquireOffscreenRequests();
    Battle_AllocateResourceSlot(BattleFx_MagicScreenShatterTimelineTask);
    Battle_AllocateResourceSlot(BattleFx_MagicScreenShatterSetStageAndActorFlagsTask);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x5d);
}

/*
 * Configures two offscreen requests when the battle graphics context exists. Both regions are 160
 * by 168; the 320x240 graphics mode places them at X 0 and 160 with scale one, the centred 320x240
 * mode uses the global viewport origin and its X plus 160 with scale one, and other modes use X 0
 * and 320 with scale two. Each independently acquired request receives type/state fields, coordinates, and two scale
 * fields, with blendMode assigned one then immediately overwritten by four. Stores each request's
 * offscreenSlot in g_BattleMagicScreenShatter_OffscreenSlots.primarySlot/slot1 only when that acquisition
 * succeeds, leaving previous values on failures.
 */
// FUNCTION: FF7 0x005d577b
void __cdecl BattleFx_MagicScreenShatterAcquireOffscreenRequests(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        BattleMagicScreenShatterRequest *request;
        struct { unsigned char pad_to_4[0x4]; short firstX; };
        struct { unsigned char pad_to_6[0x6]; short firstY; };
        struct { unsigned char pad_to_8[0x8]; short firstWidth; };
        struct { unsigned char pad_to_A[0xA]; short firstHeight; };
        struct { unsigned char pad_to_C[0xC]; short secondX; };
        struct { unsigned char pad_to_E[0xE]; short secondY; };
        struct { unsigned char pad_to_10[0x10]; short secondWidth; };
        struct { unsigned char pad_to_12[0x12]; short secondHeight; };
        struct { unsigned char pad_to_14[0x14]; int scale; };
    } frame;
#pragma pack(pop)

    frame.firstX = 0;
    frame.firstY = 0;
    frame.firstWidth = 0xA0;
    frame.firstHeight = 0xA8;
    frame.secondX = 0x140;
    frame.secondY = 0;
    frame.secondWidth = 0xA0;
    frame.secondHeight = 0xA8;
    frame.scale = 2;
    switch (g_BattleUiScalingMode) {
    case MAIN_GRAPHICS_MODE_320X240:
        frame.secondX = 0xA0;
        frame.scale = 1;
        break;
    case MAIN_GRAPHICS_MODE_320X240_CENTERED:
        frame.firstX = *(short *)&g_BattleViewportOriginX;
        frame.firstY = *(short *)&g_BattleViewportOriginY;
        frame.secondX = g_BattleViewportOriginX + 0xA0;
        frame.secondY = *(short *)&g_BattleViewportOriginY;
        frame.scale = 1;
        break;
    default:
        frame.firstX = 0;
        frame.firstY = 0;
        frame.firstWidth = 0xA0;
        frame.firstHeight = 0xA8;
        frame.secondX = 0x140;
        frame.secondY = 0;
        frame.secondWidth = 0xA0;
        frame.secondHeight = 0xA8;
        frame.scale = 2;
    }
    if (g_pBattle3dScene != 0) {
        frame.request = (BattleMagicScreenShatterRequest *)Battle3d_AcquireOffscreenRequest();
        if (frame.request != 0) {
            frame.request->drawKind = 0;
            frame.request->colorKeyEnabled = 1;
            frame.request->renderKind = 0xC;
            frame.request->blendMode = BLEND_ADDITIVE;
            frame.request->blendMode = BLEND_NONE;
            frame.request->viewportX = frame.firstX;
            frame.request->viewportY = frame.firstY;
            frame.request->viewportW = frame.firstWidth;
            frame.request->viewportH = frame.firstHeight;
            frame.request->sampleStepX = frame.scale;
            frame.request->sampleStepY = frame.scale;
            g_BattleMagicScreenShatter_OffscreenSlots.primarySlot = frame.request->offscreenSlot;
        }
        frame.request = (BattleMagicScreenShatterRequest *)Battle3d_AcquireOffscreenRequest();
        if (frame.request != 0) {
            frame.request->drawKind = 0;
            frame.request->colorKeyEnabled = 1;
            frame.request->renderKind = 0xC;
            frame.request->blendMode = BLEND_ADDITIVE;
            frame.request->blendMode = BLEND_NONE;
            frame.request->viewportX = frame.secondX;
            frame.request->viewportY = frame.secondY;
            frame.request->viewportW = frame.secondWidth;
            frame.request->viewportH = frame.secondHeight;
            frame.request->sampleStepX = frame.scale;
            frame.request->sampleStepY = frame.scale;
            g_BattleMagicScreenShatter_OffscreenSlots.secondarySlot = frame.request->offscreenSlot;
        }
    }
}

/*
 * Stores each vertex's distance from the screen centre in both grids. For each of the 840 tiles
 * (40 columns by 21 rows) clears the rotation and displacement, picks a random spin (-512..511 per
 * axis) and a velocity pointing away from the centre, sets both quads' length, command and grey
 * colour, and their UVs: each half of the screen is its own 160-pixel capture, so the U of columns
 * 19 and 20 is trimmed at the seam. Points the tile's corners at its four screen-grid vertices.
 */
// FUNCTION: FF7 0x005d59b0
void __cdecl BattleFx_MagicScreenShatterInitTileGrid(void)
{
    int diffY;
    int dx8;
    int row;
    int column;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int tile;

    for (row = 0; row < 0x16; row++) {
        for (column = 0; column < 0x29; column++) {
            dx8 = column * 8 + -0xA0;
            diffY = row * 8 + -0x78;
            SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, row)[column].radius = SqrtToInt(dx8 * dx8 + diffY * diffY);
            SHATTER_GRID_ROW(g_BattleMagicScreenShatter_ScreenGrid, row)[column].radius = SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, row)[column].radius;
        }
    }
    tile = 0;
    for (row = 0; row < 0x15; row++) {
        for (column = 0; column < 0x28; column++) {
            g_BattleMagicScreenShatterTileGrid.tiles[tile].angle.vz = 0;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].angle.vy = 0;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].angle.vx = 0;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].spin.vx = (rand() & 0x3FF) - 0x200;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].spin.vy = (rand() & 0x3FF) - 0x200;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].spin.vz = (rand() & 0x3FF) - 0x200;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].pos.vz = 0;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].pos.vy = 0;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].pos.vx = 0;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].vel.vx = (rand() & 3) + column * 2 + -0x2A;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].vel.vy = (rand() & 3) + row * 2 + -0x20;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].vel.vz = (rand() & 0x7F) - 0x80;
            SHATTER_HEADER(g_BattleScreenShatterTiles, tile).header = SHATTER_HEADER(g_BattleScreenShatterTiles, tile).header & 0xFFFFFF | 0x9000000;
            SHATTER_COLOR(g_BattleMagicScreenShatter_Quad0Color, tile).code = PSX_PRIM_POLY_FT4;
            SHATTER_COLOR(g_BattleMagicScreenShatter_Quad0Color, tile).r = 0x80;
            SHATTER_COLOR(g_BattleMagicScreenShatter_Quad0Color, tile).g = 0x80;
            SHATTER_COLOR(g_BattleMagicScreenShatter_Quad0Color, tile).b = 0x80;
            SHATTER_HEADER(g_BattleMagicScreenShatter_Quad1Header, tile).header = SHATTER_HEADER(g_BattleMagicScreenShatter_Quad1Header, tile).header & 0xFFFFFF | 0x9000000;
            SHATTER_COLOR(g_BattleMagicScreenShatter_Quad1Color, tile).code = PSX_PRIM_POLY_FT4;
            SHATTER_COLOR(g_BattleMagicScreenShatter_Quad1Color, tile).r = 0x80;
            SHATTER_COLOR(g_BattleMagicScreenShatter_Quad1Color, tile).g = 0x80;
            SHATTER_COLOR(g_BattleMagicScreenShatter_Quad1Color, tile).b = 0x80;
            if (column < 0x14) {
                SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv2, tile).u = column << 3 & 0xFF;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv2, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv2, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv0, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv2, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv0, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv0, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv3, tile).u = column * 8 + 8 & 0xFF;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv3, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv3, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv1, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv3, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv1, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv1, tile).u;
                if (column == 0x13) {
                    SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv3, tile).u = column * 8 + 7 & 0xFF;
                    SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv3, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv3, tile).u;
                    SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv1, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv3, tile).u;
                    SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv1, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv1, tile).u;
                }
            } else {
                SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv2, tile).u = column * 8 + -0xA0 & 0xFF;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv2, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv2, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv0, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv2, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv0, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv0, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv3, tile).u = column * 8 + -0x98 & 0xFF;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv3, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv3, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv1, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv3, tile).u;
                SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv1, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv1, tile).u;
                if (column == 0x14) {
                    SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv2, tile).u = column * 8 + -0x9F & 0xFF;
                    SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv2, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv2, tile).u;
                    SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv0, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv2, tile).u;
                    SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv0, tile).u = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv0, tile).u;
                }
            }
            SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv1, tile).v = row << 3;
            SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv1, tile).v = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv1, tile).v;
            SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv0, tile).v = SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv1, tile).v;
            SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv0, tile).v = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv0, tile).v;
            SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv3, tile).v = row * 8 + 8;
            SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv3, tile).v = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv3, tile).v;
            SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv2, tile).v = SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv3, tile).v;
            SHATTER_UV(g_BattleMagicScreenShatter_Quad0Uv2, tile).v = SHATTER_UV(g_BattleMagicScreenShatter_Quad1Uv2, tile).v;
            g_BattleMagicScreenShatterTileGrid.tiles[tile].cornerPtr = &SHATTER_GRID_ROW(g_BattleMagicScreenShatter_ScreenGrid, row)[column];
            g_BattleMagicScreenShatterTileGrid.tiles[tile].rightNeighbor = &SHATTER_GRID_ROW(g_BattleMagicScreenShatter_ScreenGrid, row)[column + 1];
            g_BattleMagicScreenShatterTileGrid.tiles[tile].downNeighbor = &SHATTER_GRID_ROW(g_BattleMagicScreenShatter_ScreenGrid, row + 1)[column];
            g_BattleMagicScreenShatterTileGrid.tiles[tile].diagNeighbor = &SHATTER_GRID_ROW(g_BattleMagicScreenShatter_ScreenGrid, row + 1)[column + 1];
            tile++;
        }
    }
}

/*
 * Acquires scratch whose quad is followed by a matrix; no pause check. First thirty counts add 24 to
 * retained wave amplitude and deform 22-by-41 grid XY using radial sine phase distance times 100
 * minus count times 500, scale by screen factor/origin and copy four grid DWORDs into each of 840
 * selected-bank quads. Draws each with blend (zero, one), switching between two offscreen
 * resources every twenty columns of forty. Next thirty repoint cells to local grid on entry, build
 * four zero-Z points centred around (160, 120), subtract cell mean and integrate displacement;
 * damp Z step by sixteenth and add four to Y step. Builds retained cell rotation, applies scale,
 * sets translation from mean times four plus displacement and geometry-screen times four plus Z,
 * projects all four corners, advances angle triples and draws without testing projection
 * result/flags. Following five counts only increment and return. Later retires, conditionally
 * releases/reset both offscreen slots and returns without increment; active first sixty increment
 * after drawing; motion runs while the battle is paused.
 */
// FUNCTION: FF7 0x005d602a
void __cdecl BattleFx_MagicScreenShatterTimelineTask(void)
{
    struct PacketWord {
        int word;
    };
    int outProjDepth;
    BattleTaskSlot *taskPtr;
    int projFlags;
    int tmpDeltaX;
    int stepDeltaY;
    int resolvedRow;
    int localColumn;
    PsxMatrix *pMatrix;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int deform;
    int fillIndex;
    int nextFrame;
    int halfCol;
    int screenDist;
    BattleFxShardQuad *dstQuad;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    dstQuad = (BattleFxShardQuad *)Gfx_GetPsxScratchpadPtr(0);
    pMatrix = (PsxMatrix *)(dstQuad + 1);
    nextFrame = taskPtr->frameCounter;
    if (nextFrame < 0x1E) {
        g_BattleMagicScreenShatter_WaveAmplitude += 0x18;
        for (resolvedRow = 0; resolvedRow < 0x16; resolvedRow++) {
            for (localColumn = 0; localColumn < 0x29; localColumn++) {
                tmpDeltaX = localColumn * 8 + -0xA0;
                stepDeltaY = resolvedRow * 8 + -0x78;
                deform = g_BattleMagicScreenShatter_WaveAmplitude + (SinFixed(SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow)[localColumn].radius * 0x64 - nextFrame * 0x1F4) * g_BattleMagicScreenShatter_WaveAmplitude >> 0xC) + 0x1000;
                SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow)[localColumn].x = (tmpDeltaX * deform >> 0xC) + 0xA0;
                SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow)[localColumn].y = (stepDeltaY * deform >> 0xC) + 0x78;
                SHATTER_GRID_ROW(g_BattleMagicScreenShatter_ScreenGrid, resolvedRow)[localColumn].x = SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow)[localColumn].x * g_BattleUiScale + g_BattleViewportOriginX;
                SHATTER_GRID_ROW(g_BattleMagicScreenShatter_ScreenGrid, resolvedRow)[localColumn].y = SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow)[localColumn].y * g_BattleUiScale + g_BattleViewportOriginY;
            }
        }
        halfCol = 0;
        for (resolvedRow = 0; resolvedRow < 0x348; resolvedRow++) {
            *(struct PacketWord *)((char *)&g_BattleMagicScreenShatter_Quad0Xy0 + ((resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28)) = *(struct PacketWord *)g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].cornerPtr;
            *(struct PacketWord *)((char *)&g_BattleMagicScreenShatter_Quad0Xy1 + ((resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28)) = *(struct PacketWord *)g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].rightNeighbor;
            *(struct PacketWord *)((char *)&g_BattleMagicScreenShatter_Quad0Xy2 + ((resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28)) = *(struct PacketWord *)g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].downNeighbor;
            *(struct PacketWord *)((char *)&g_BattleMagicScreenShatter_Quad0Xy3 + ((resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28)) = *(struct PacketWord *)g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].diagNeighbor;
            Gfx_SetBlendFactorPair(0, FLOAT_ONE_BITS);
            if (halfCol < 0x14) {
                Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Battle3d_GetOffscreenRenderObject(g_BattleMagicScreenShatter_OffscreenSlots.primarySlot)));
            } else {
                Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Battle3d_GetOffscreenRenderObject(g_BattleMagicScreenShatter_OffscreenSlots.secondarySlot)));
            }
            Gfx_DrawPsxPrimitive((resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28 + (char *)&g_BattleScreenShatterTiles);
            halfCol++;
            if (halfCol >= 0x28) {
                halfCol = 0;
            }
        }
    } else {
        nextFrame -= 0x1E;
        if (nextFrame < 0x1E) {
            if (nextFrame == 0) {
                fillIndex = 0;
                for (resolvedRow = 0; resolvedRow < 0x15; resolvedRow++) {
                    for (localColumn = 0; localColumn < 0x28; localColumn++) {
                        g_BattleMagicScreenShatterTileGrid.tiles[fillIndex].cornerPtr = &SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow)[localColumn];
                        g_BattleMagicScreenShatterTileGrid.tiles[fillIndex].rightNeighbor = &SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow)[localColumn + 1];
                        g_BattleMagicScreenShatterTileGrid.tiles[fillIndex].downNeighbor = &SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow + 1)[localColumn];
                        g_BattleMagicScreenShatterTileGrid.tiles[fillIndex].diagNeighbor = &SHATTER_GRID_ROW(g_BattleMagicScreenShatter_GridRecords, resolvedRow + 1)[localColumn + 1];
                        fillIndex++;
                    }
                }
            }
            halfCol = 0;
            screenDist = GetGeomScreen() << 2;
            for (resolvedRow = 0; resolvedRow < 0x348; resolvedRow++) {
                dstQuad->corner0X = g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].cornerPtr->x - 0xA0;
                dstQuad->corner0Y = g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].cornerPtr->y - 0x78;
                dstQuad->depth = 0;
                dstQuad->corner1X = g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].rightNeighbor->x - 0xA0;
                dstQuad->corner1Y = g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].rightNeighbor->y - 0x78;
                dstQuad->stripStart = 0;
                dstQuad->corner2X = g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].downNeighbor->x - 0xA0;
                dstQuad->corner2Y = g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].downNeighbor->y - 0x78;
                dstQuad->stripLevel = 0;
                dstQuad->corner3X = g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].diagNeighbor->x - 0xA0;
                dstQuad->corner3Y = g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].diagNeighbor->y - 0x78;
                dstQuad->stripEnd = 0;
                dstQuad->centerX = dstQuad->corner0X + dstQuad->corner1X + dstQuad->corner2X + dstQuad->corner3X >> 2;
                dstQuad->centerY = dstQuad->corner0Y + dstQuad->corner1Y + dstQuad->corner2Y + dstQuad->corner3Y >> 2;
                dstQuad->corner0X -= dstQuad->centerX;
                dstQuad->corner0Y -= dstQuad->centerY;
                dstQuad->corner1X -= dstQuad->centerX;
                dstQuad->corner1Y -= dstQuad->centerY;
                dstQuad->corner2X -= dstQuad->centerX;
                dstQuad->corner2Y -= dstQuad->centerY;
                dstQuad->corner3X -= dstQuad->centerX;
                dstQuad->corner3Y -= dstQuad->centerY;
                g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].pos.vx += g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].vel.vx;
                g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].pos.vy += g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].vel.vy;
                g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].pos.vz += g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].vel.vz;
                g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].vel.vz -= (short)(g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].vel.vz >> 4);
                g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].vel.vy += 4;
                RotMatrix(&g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].angle, pMatrix);
                ScaleMatrix(pMatrix, g_BattleMagicScreenShatter_ScaleVector);
                pMatrix->t[0] = dstQuad->centerX * 4 + g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].pos.vx;
                pMatrix->t[1] = dstQuad->centerY * 4 + g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].pos.vy;
                pMatrix->t[2] = screenDist + g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].pos.vz;
                SetRotMatrix(pMatrix);
                SetTransMatrix(pMatrix);
                RefreshGlobalTransformState();
                Gfx_ProjectQuadToScreenAlt((SVECTOR *)&dstQuad->corner0X, (SVECTOR *)&dstQuad->corner1X, (SVECTOR *)&dstQuad->corner2X, (SVECTOR *)&dstQuad->corner3X, (resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28 + (char *)&g_BattleMagicScreenShatter_Quad0Xy0, (resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28 + (char *)&g_BattleMagicScreenShatter_Quad0Xy1, (resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28 + (char *)&g_BattleMagicScreenShatter_Quad0Xy2, (resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28 + (char *)&g_BattleMagicScreenShatter_Quad0Xy3, &outProjDepth, &projFlags);
                g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].angle.vx += g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].spin.vx;
                g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].angle.vy += g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].spin.vy;
                g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].angle.vz += g_BattleMagicScreenShatterTileGrid.tiles[resolvedRow].spin.vz;
                if (halfCol < 0x14) {
                    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Battle3d_GetOffscreenRenderObject(g_BattleMagicScreenShatter_OffscreenSlots.primarySlot)));
                } else {
                    Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Battle3d_GetOffscreenRenderObject(g_BattleMagicScreenShatter_OffscreenSlots.secondarySlot)));
                }
                Gfx_DrawPsxPrimitive((resolvedRow << 7) + g_BattleMagicScreenShatter_PacketBank * 0x28 + (char *)&g_BattleScreenShatterTiles);
                halfCol++;
                if (halfCol >= 0x28) {
                    halfCol = 0;
                }
            }
        } else {
            nextFrame -= 0x1E;
            if (nextFrame < 5) {
                (taskPtr->frameCounter)++;
                return;
            }
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
            if (g_BattleMagicScreenShatter_OffscreenSlots.primarySlot >= 0) {
                Battle3d_ReleaseOffscreenSlot(g_BattleMagicScreenShatter_OffscreenSlots.primarySlot);
                g_BattleMagicScreenShatter_OffscreenSlots.primarySlot = 0xFFFFFFFF;
            }
            if (g_BattleMagicScreenShatter_OffscreenSlots.secondarySlot >= 0) {
                Battle3d_ReleaseOffscreenSlot(g_BattleMagicScreenShatter_OffscreenSlots.secondarySlot);
                g_BattleMagicScreenShatter_OffscreenSlots.secondarySlot = 0xFFFFFFFF;
            }
            return;
        }
    }
    (taskPtr->frameCounter)++;
}

/*
 * On its second frame sets the three stage hide flags, sets draw flags 2 and 4 and clears flag
 * 0x80 on all ten actors, then frees itself. The counter still advances after the free.
 */
// FUNCTION: FF7 0x005d699c
void __cdecl BattleFx_MagicScreenShatterSetStageAndActorFlagsTask(void)
{
    BattleTaskSlot *task;
    int actor;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCounter == 1) {
        g_BattleStageHideFlags |= 7;
        for (actor = 0; actor < 0xA; actor++) {
            g_BattleActorRecords[actor].drawFlags |= 2;
            g_BattleActorRecords[actor].drawFlags &= 0x7F;
            g_BattleActorRecords[actor].drawFlags |= 4;
        }
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}
} /* extern "C" */

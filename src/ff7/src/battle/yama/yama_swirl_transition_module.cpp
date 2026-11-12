// src/ff7/src/battle/yama/yama_swirl_transition_module.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The battle swirl transition module: sets up the viewports, grid and sprite records,
//   runs the 3D swirl each frame at a fixed rate, and tears down when it exits.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include "lib/codegen.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/main.h"
#include "lib/api/mem.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "lib/pfile.h"
#include "lib/enums/graphics.h"
extern "C" {

struct GfxEngineContext;

union SwirlPacket {
    unsigned int tag;
    struct {
        unsigned int address : 24;
        unsigned int length : 8;
    } header;
    unsigned char bytes[40];
    short words[20];
    int dwords[10];
};

struct SwirlRecord {
    int leading;
    union SwirlPacket packets[10][8];
    int pad_c84;
    short points[11][9][4];
    short projected[11][9][2];
    short rotX;
    short rotY;
    short rotZ;
    short rotPad;
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_1140[0x8];
    int spinSpeed;
    int frameCounter;
    unsigned char pad_1150[0x4];
    int fade;
};

typedef char SwirlRecordViewSize[(sizeof(struct SwirlRecord) == 4440) ? 1 : -1];

// clang-format off
// GLOBAL: FF7 0x007ba1a8
int g_SwirlSpriteHandleFirst = (-1);
// GLOBAL: FF7 0x007ba1ac
int g_SwirlSpriteHandleThird = (-1);
// GLOBAL: FF7 0x007ba1b0
int g_SwirlSpriteHandleSecond = (-1);
// GLOBAL: FF7 0x007ba1b4
int g_SwirlSpriteHandleFourth = (-1);
// GLOBAL: FF7 0x007ba1b8
double g_SwirlTargetFps = 30.0;
// GLOBAL: FF7 0x0099f308
struct SwirlRecord *g_pSwirlRecord = 0;
unsigned char g_bssPad_0099f30c[4] = {0};
// GLOBAL: FF7 0x0099f310
unsigned char g_SwirlTransformMatrix[30] = {0};
// GLOBAL: FF7 0x0099f330
int g_SwirlScreenWidth = 0;
// GLOBAL: FF7 0x0099f334
int g_SwirlScreenHeight = 0;
// GLOBAL: FF7 0x0099f338
int g_SwirlResetViewport = 0;
// GLOBAL: FF7 0x0099f33c
int g_SwirlGridCenterY = 0;
// GLOBAL: FF7 0x0099f340
int g_SwirlGridCenterX = 0;
unsigned char g_bssPad_0099f344[4] = {0};
// GLOBAL: FF7 0x0099f348
struct SwirlRecord g_SwirlRecord = {0};
// GLOBAL: FF7 0x009a04a0
double g_SwirlFrameDuration = 0.0;
// GLOBAL: FF7 0x009a04a8
Timestamp64 g_SwirlCurrentTimestamp = {0};
// GLOBAL: FF7 0x009a04b0
int g_SwirlScaleMultiplier = 0;
unsigned char g_bssPad_009a04b4[4] = {0};
// GLOBAL: FF7 0x009a04b8
struct SwirlSpriteLeftPos g_swirlSpriteRightPos = {0};
// GLOBAL: FF7 0x009a04bc
struct SwirlSpriteLeftDim g_swirlSpriteRightDim = {0};
// GLOBAL: FF7 0x009a04c0
Timestamp64 g_SwirlStartTimestamp = {0};
// GLOBAL: FF7 0x009a04c8
struct SwirlSpriteLeftPos g_swirlSpriteLeftPos = {0};
// GLOBAL: FF7 0x009a04cc
struct SwirlSpriteLeftDim g_swirlSpriteLeftDim = {0};
// GLOBAL: FF7 0x009a04d0
int g_SwirlTexStep = 0;
// GLOBAL: FF7 0x009a04d4
int g_SwirlOriginY = 0;
// GLOBAL: FF7 0x009a04d8
int g_SwirlOriginX = 0;
// GLOBAL: FF7 0x009a04dc
int g_SwirlGridSpacing = 0;
// GLOBAL: FF7 0x009a04e0
int g_SwirlFrameCount = 0;
// GLOBAL: FF7 0x009a04e4
int g_SwirlActiveFlag = 0;
// GLOBAL: FF7 0x009a04e8
int g_SwirlOverrunFlag = 0;
unsigned char g_bssPad_009a04ec[4] = {0};
// GLOBAL: FF7 0x009a04f0
double g_SwirlOverrunExcess = 0.0;
// clang-format on

#pragma intrinsic(strcmp, memcmp, memset, strcpy, strcat)

/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern void __cdecl Gfx_EmitPsxTexturedFlatQuadScreen(int payload, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, int depth, float reciprocal, int emitter);


/*
 * Waits for the remainder of the swirl frame duration to maintain 30 FPS.
 */
// FUNCTION: FF7 0x00401500
void __cdecl Swirl_WaitFrameDuration(struct GfxEngineContext *context)
{
    double elapsed;
    unsigned int delta[2];
    elapsed = 0.0;
    ReadTimestampCounter(&g_SwirlCurrentTimestamp);
    Sub64(&g_SwirlCurrentTimestamp, &g_SwirlStartTimestamp, (struct Timestamp64 *)delta);
    elapsed = CombineSplitValueToFloat((struct Timestamp64 *)delta);
    if (g_SwirlOverrunFlag == 0) {
        if (elapsed <= g_SwirlFrameDuration) {
            while (elapsed < g_SwirlFrameDuration) {
                ReadTimestampCounter(&g_SwirlCurrentTimestamp);
                Sub64(&g_SwirlCurrentTimestamp, &g_SwirlStartTimestamp, (struct Timestamp64 *)delta);
                elapsed = CombineSplitValueToFloat((struct Timestamp64 *)delta);
            }
            g_SwirlOverrunFlag = 0;
            g_SwirlOverrunExcess = 0.0;
        } else {
            g_SwirlOverrunExcess = elapsed - g_SwirlFrameDuration;
            g_SwirlOverrunFlag = 1;
        }
    } else {
        while (g_SwirlFrameDuration - g_SwirlOverrunExcess > elapsed) {
            ReadTimestampCounter(&g_SwirlCurrentTimestamp);
            Sub64(&g_SwirlCurrentTimestamp, &g_SwirlStartTimestamp, (struct Timestamp64 *)delta);
            elapsed = CombineSplitValueToFloat((struct Timestamp64 *)delta);
        }
        g_SwirlOverrunFlag = 0;
        g_SwirlOverrunExcess = 0.0;
    }
}

/*
 * Enters the swirl that leads from the field or world map into a battle
 * (GAME_MODE_BATTLE_SWIRL). By GetUiScalingMode it picks the layout: mode 0 is 320x240 at the
 * origin, mode 1 is 320x240 centred at (160,120), mode 2 is 640x480 at the origin with scale 2;
 * the swirl grid spacing is 32 pixels times that scale. Any other mode keeps the values of the
 * last swirl. It places the two 160x240 halves of the screen image, the left at the origin and the
 * right at half the width, sets the texture step to 32, builds the 11x9 point grid and the 10x8
 * quad packets (Swirl_InitGridAndPackets) and clears the frame count and the active flag. It sets
 * g_SwirlFrameDuration to one 30 fps frame of the TSC frequency, which Swirl_WaitFrameDuration
 * waits for, the GTE matrix layout mode 1, clears the screen and clears the viewport-reset flag.
 */
// FUNCTION: FF7 0x0040164e
void __cdecl Swirl_ModuleEnter(struct GfxEngineContext *ctx)
{
    Gfx_DebugPrint(
        // STRING: FF7 0x007ba1c0
        "Entering FRAME_INITIALIZE SWIRL\n");
    switch (GetUiScalingMode()) {
    case MAIN_GRAPHICS_MODE_320X240:
        g_SwirlScreenWidth = 320;
        g_SwirlScreenHeight = 240;
        g_SwirlOriginX = 0;
        g_SwirlOriginY = 0;
        g_SwirlScaleMultiplier = 1;
        g_SwirlGridSpacing = 32;
        break;
    case MAIN_GRAPHICS_MODE_320X240_CENTERED:
        g_SwirlScreenWidth = 320;
        g_SwirlScreenHeight = 240;
        g_SwirlOriginX = 160;
        g_SwirlOriginY = 120;
        g_SwirlScaleMultiplier = 1;
        g_SwirlGridSpacing = 32;
        break;
    case MAIN_GRAPHICS_MODE_640X480:
        g_SwirlScreenWidth = 640;
        g_SwirlScreenHeight = 480;
        g_SwirlOriginX = 0;
        g_SwirlOriginY = 0;
        g_SwirlScaleMultiplier = 2;
        g_SwirlGridSpacing = 64;
        break;
    }
    g_swirlSpriteLeftPos.spriteX = (*(short *)&g_SwirlOriginX);
    g_swirlSpriteLeftPos.spriteY = (*(short *)&g_SwirlOriginY);
    g_swirlSpriteRightPos.spriteX = g_SwirlScreenWidth / 2 + g_SwirlOriginX;
    g_swirlSpriteRightPos.spriteY = (*(short *)&g_SwirlOriginY);
    g_swirlSpriteRightDim.spriteW = 160;
    g_swirlSpriteLeftDim.spriteW = g_swirlSpriteRightDim.spriteW;
    g_swirlSpriteRightDim.spriteH = 240;
    g_swirlSpriteLeftDim.spriteH = g_swirlSpriteRightDim.spriteH;
    g_SwirlTexStep = 32;
    Swirl_InitGridAndPackets();
    g_SwirlFrameCount = 0;
    g_SwirlActiveFlag = 0;
    g_SwirlFrameDuration = ctx->tscFrequency / g_SwirlTargetFps;
    SetMatrixLayoutMode(1);
    GfxClearCall(0, 1, 0, ctx);
    g_SwirlResetViewport = 0;
    Gfx_DebugPrint(
        // STRING: FF7 0x007ba1e4
        "Exitting FRAME_INITIALIZE SWIRL\n");
}

/*
 * Initializes the swirl 11x9 3D vertex grid and 10x8 textured quad packets.
 */
// FUNCTION: FF7 0x00401810
void __cdecl Swirl_InitGridAndPackets(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        struct { unsigned char pad_to_2[0x2]; char flagBit0; };
        struct { unsigned char pad_to_3[0x3]; char flagBit1; };
        struct { unsigned char pad_to_4[0x4]; struct SwirlRecord *swirlRecord; };
        struct { unsigned char pad_to_8[0x8]; int gridY; };
        struct { unsigned char pad_to_C[0xC]; int gridX; };
    } frame;
#pragma pack(pop)

    g_pSwirlRecord = &g_SwirlRecord;
    memset(&g_SwirlRecord, 0, 4440);
    g_SwirlGridCenterX = (g_SwirlScreenWidth / 2) + g_SwirlScaleMultiplier * 0x19;
    g_SwirlGridCenterY = (g_SwirlScreenHeight / 2) + g_SwirlScaleMultiplier * 0x19;
    g_pSwirlRecord->scaleX = 0x1040;
    g_pSwirlRecord->scaleY = 0x1040;
    g_pSwirlRecord->scaleZ = 0x1000;
    g_pSwirlRecord->spinSpeed = 4;
    g_pSwirlRecord->frameCounter = 0;
    g_pSwirlRecord->fade = 0x80;
    for (frame.gridY = 0; frame.gridY < 9; frame.gridY++) {
        for (frame.gridX = 0; frame.gridX < 0xB; frame.gridX++) {
            g_pSwirlRecord->points[frame.gridX][frame.gridY][0] = frame.gridX * g_SwirlGridSpacing - g_SwirlGridCenterX;
            g_pSwirlRecord->points[frame.gridX][frame.gridY][1] = frame.gridY * g_SwirlGridSpacing - g_SwirlGridCenterY;
            g_pSwirlRecord->points[frame.gridX][frame.gridY][2] = 0;
        }
    }
    frame.swirlRecord = g_pSwirlRecord;
    for (frame.gridY = 0; frame.gridY < 8; frame.gridY++) {
        for (frame.gridX = 0; frame.gridX < 0xA; frame.gridX++) {
            frame.swirlRecord->packets[frame.gridX][frame.gridY].header.length = 9;
            frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] = 0x2C;
            if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                frame.flagBit1 = frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] = frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] | 2;
            } else {
                frame.flagBit1 = frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] = frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] & 0xFFFFFFFD;
            }
            if (DEAD_IN_RETAIL != 0) {
                frame.flagBit0 = frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] = frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] | 1;
            } else {
                frame.flagBit0 = frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] = frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[7] & 0xFFFFFFFE;
            }
            frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[4] = 0x80;
            frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[5] = 0x80;
            frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[6] = 0x80;
            if (frame.gridX < 5) {
                if (frame.gridX != 5) {
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[12] = frame.gridX * g_SwirlTexStep;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[13] = frame.gridY * g_SwirlTexStep;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[20] = frame.gridX * g_SwirlTexStep + g_SwirlTexStep + -1;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[21] = frame.gridY * g_SwirlTexStep;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[28] = frame.gridX * g_SwirlTexStep;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[29] = frame.gridY * g_SwirlTexStep + g_SwirlTexStep + -1;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[36] = frame.gridX * g_SwirlTexStep + g_SwirlTexStep + -1;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[37] = frame.gridY * g_SwirlTexStep + g_SwirlTexStep + -1;
                } else {
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[12] = frame.gridX * g_SwirlTexStep;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[13] = frame.gridY * g_SwirlTexStep;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[20] = frame.gridX * g_SwirlTexStep + g_SwirlTexStep + -2;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[21] = frame.gridY * g_SwirlTexStep;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[28] = frame.gridX * g_SwirlTexStep;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[29] = frame.gridY * g_SwirlTexStep + g_SwirlTexStep + -1;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[36] = frame.gridX * g_SwirlTexStep + g_SwirlTexStep + -2;
                    frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[37] = frame.gridY * g_SwirlTexStep + g_SwirlTexStep + -1;
                }
            } else if (frame.gridX != 5) {
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[12] = (frame.gridX - 5) * g_SwirlTexStep;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[13] = frame.gridY * g_SwirlTexStep;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[20] = (frame.gridX - 5) * g_SwirlTexStep + g_SwirlTexStep + -1;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[21] = frame.gridY * g_SwirlTexStep;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[28] = (frame.gridX - 5) * g_SwirlTexStep;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[29] = frame.gridY * g_SwirlTexStep + g_SwirlTexStep + -1;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[36] = (frame.gridX - 5) * g_SwirlTexStep + g_SwirlTexStep + -1;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[37] = frame.gridY * g_SwirlTexStep + g_SwirlTexStep + -1;
            } else {
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[12] = (frame.gridX - 5) * g_SwirlTexStep + 1;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[13] = frame.gridY * g_SwirlTexStep;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[20] = (frame.gridX - 5) * g_SwirlTexStep + g_SwirlTexStep + -1;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[21] = frame.gridY * g_SwirlTexStep;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[28] = (frame.gridX - 5) * g_SwirlTexStep + 1;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[29] = frame.gridY * g_SwirlTexStep + g_SwirlTexStep + -1;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[36] = (frame.gridX - 5) * g_SwirlTexStep + g_SwirlTexStep + -1;
                frame.swirlRecord->packets[frame.gridX][frame.gridY].bytes[37] = frame.gridY * g_SwirlTexStep + g_SwirlTexStep + -1;
            }
        }
    }
}

/*
 * Teardown for the swirl module: releases sprite slots and selects the next game mode.
 */
// FUNCTION: FF7 0x0040213b
void __cdecl Swirl_ModuleExit(void)
{
    Gfx_DebugPrint(
        // STRING: FF7 0x007ba208
        "Entering FRAME_QUIT SWIRL\n");
    if (g_SwirlSpriteHandleFirst >= 0) {
        Gfx_ReleaseOffscreenSlotByIndex(g_SwirlSpriteHandleFirst);
        g_SwirlSpriteHandleFirst = 0xFFFFFFFF;
    }
    if (g_SwirlSpriteHandleThird >= 0) {
        Gfx_ReleaseOffscreenSlotByIndex(g_SwirlSpriteHandleThird);
        g_SwirlSpriteHandleThird = 0xFFFFFFFF;
    }
    if (g_SwirlSpriteHandleSecond >= 0) {
        Gfx_ReleaseOffscreenSlotByIndex(g_SwirlSpriteHandleSecond);
        g_SwirlSpriteHandleSecond = 0xFFFFFFFF;
    }
    if (g_SwirlSpriteHandleFourth >= 0) {
        Gfx_ReleaseOffscreenSlotByIndex(g_SwirlSpriteHandleFourth);
        g_SwirlSpriteHandleFourth = 0xFFFFFFFF;
    }
    g_EngineGameMode = GAME_MODE_BATTLE;
    Gfx_DebugPrint(
        // STRING: FF7 0x007ba224
        "Exitting FRAME_QUIT SWIRL\n");
}

/*
 * Per-frame update and render for the battle swirl transition effect.
 */
// FUNCTION: FF7 0x004021e9
void __cdecl Swirl_ModuleFrame(struct GfxEngineContext *context)
{
    GfxClearColorBgra bgColor;

    ReadTimestampCounter(&g_SwirlStartTimestamp);
    ResetFrameDrawPool(context);
    ResetRenderPools(context);
    Gfx_BeginSortedDrawFrame(context);
    Gfx_ResetNodeList(context);
    if (g_SwirlFrameCount == 0) {
        Swirl_SetupSpriteRecords(context);
    }
    Gfx_ServiceAllOffscreenSlots();
    if (((int (__cdecl *)(int scene, int context))Gfx_BeginSceneCall)(GFX_SCENE_PASS_OPAQUE, (int)context) != 0) { /* reads eax after a void call, as the original does */
        if (g_SwirlFrameCount == 3) {
            Gfx_DebugPrint(
                // STRING: FF7 0x007ba240
                "Swirl sound_effect1\n");
            App_PlayCachedSfxUnlessMuted(0x2B);
            Gfx_DebugPrint(
                // STRING: FF7 0x007ba258
                "End of Swirl sound_effect1\n");
        }
        if (g_SwirlOverrunFlag != 0) {
            Gfx_SetDrawEnabled(0);
        } else {
            Gfx_SetDrawEnabled(1);
        }
        Swirl_UpdateAndDrawGrid(context, g_SwirlActiveFlag);
        if (g_SwirlOverrunFlag != 0) {
            Swirl_WaitFrameDuration(context);
            g_SwirlOverrunFlag = 0;
            Gfx_SetDrawEnabled(1);
            (context)->framesThisSecond += 1.0;
            ReadTimestampCounter(&g_SwirlStartTimestamp);
            Swirl_UpdateAndDrawGrid(context, g_SwirlActiveFlag);
        }
        if (g_SwirlActiveFlag == 0) {
            Gfx_DrawAllOffscreenSlots(0);
        }
        Gfx_SetScenePassCall(GFX_SCENE_PASS_TRANSLUCENT, context);
        GfxFlushFrameDrawList(context);
        Gfx_EndSortedDrawFrame(context);
        Gfx_DrawAllOffscreenSlots(1);
        g_SwirlFrameCount++;
        if (g_SwirlFrameCount == 3) {
            g_SwirlActiveFlag = 1;
        }
        Gfx_EndSceneCall(context);
    }
    if (g_SwirlResetViewport != 0) {
        bgColor.r = 0;
        bgColor.g = 0;
        bgColor.b = 0;
        bgColor.a = FLOAT_ONE_BITS;
        GfxSetViewportRect(g_SwirlOriginX, g_SwirlOriginY, g_SwirlScreenWidth, g_SwirlScreenHeight, context);
        GfxSetBackgroundColorCall(&bgColor, context);
        GfxClearCall(1, 1, 1, context);
    }
}

/*
 * Claims and configures four offscreen sprite slots for the left and right swirl halves.
 */
// FUNCTION: FF7 0x004023f8
void __cdecl Swirl_SetupSpriteRecords(GfxEngineContext *context)
{
    GfxOffscreenRequest *slot;

    slot = (GfxOffscreenRequest *)Gfx_ClaimOffscreenSlotRequest(context);
    if (slot != 0) {
        slot->kind = 0;
        slot->pageFillEnabled = 1;
        slot->renderKind = GFX_DRAW_TEXTURED_QUAD_2D;
        slot->blendMode = BLEND_AVERAGE;
        slot->viewportX = g_swirlSpriteLeftPos.spriteX;
        slot->viewportY = g_swirlSpriteLeftPos.spriteY;
        slot->viewportW = g_swirlSpriteLeftDim.spriteW;
        slot->viewportH = g_swirlSpriteLeftDim.spriteH;
        slot->pageFillStepX = g_SwirlScaleMultiplier;
        slot->pageFillStepY = g_SwirlScaleMultiplier;
        g_SwirlSpriteHandleFirst = slot->slotIndex;
    }
    slot = (GfxOffscreenRequest *)Gfx_ClaimOffscreenSlotRequest(context);
    if (slot != 0) {
        slot->kind = 0;
        slot->pageFillEnabled = 0;
        slot->renderKind = GFX_DRAW_TEXTURED_QUAD_2D;
        slot->blendMode = BLEND_NONE;
        slot->viewportX = g_swirlSpriteLeftPos.spriteX;
        slot->viewportY = g_swirlSpriteLeftPos.spriteY;
        slot->viewportW = g_swirlSpriteLeftDim.spriteW;
        slot->viewportH = g_swirlSpriteLeftDim.spriteH;
        slot->pageFillStepX = g_SwirlScaleMultiplier;
        slot->pageFillStepY = g_SwirlScaleMultiplier;
        g_SwirlSpriteHandleSecond = slot->slotIndex;
    }
    slot = (GfxOffscreenRequest *)Gfx_ClaimOffscreenSlotRequest(context);
    if (slot != 0) {
        slot->kind = 0;
        slot->pageFillEnabled = 1;
        slot->renderKind = GFX_DRAW_TEXTURED_QUAD_2D;
        slot->blendMode = BLEND_AVERAGE;
        slot->viewportX = g_swirlSpriteRightPos.spriteX;
        slot->viewportY = g_swirlSpriteRightPos.spriteY;
        slot->viewportW = g_swirlSpriteRightDim.spriteW;
        slot->viewportH = g_swirlSpriteRightDim.spriteH;
        slot->pageFillStepX = g_SwirlScaleMultiplier;
        slot->pageFillStepY = g_SwirlScaleMultiplier;
        g_SwirlSpriteHandleThird = slot->slotIndex;
    }
    slot = (GfxOffscreenRequest *)Gfx_ClaimOffscreenSlotRequest(context);
    if (slot != 0) {
        slot->kind = 0;
        slot->pageFillEnabled = 0;
        slot->renderKind = GFX_DRAW_TEXTURED_QUAD_2D;
        slot->blendMode = BLEND_NONE;
        slot->viewportX = g_swirlSpriteRightPos.spriteX;
        slot->viewportY = g_swirlSpriteRightPos.spriteY;
        slot->viewportW = g_swirlSpriteRightDim.spriteW;
        slot->viewportH = g_swirlSpriteRightDim.spriteH;
        slot->pageFillStepX = g_SwirlScaleMultiplier;
        slot->pageFillStepY = g_SwirlScaleMultiplier;
        g_SwirlSpriteHandleFourth = slot->slotIndex;
    }
}

/*
 * Updates the 3D swirl rotation/scales, projects vertices, and submits quad draw batches.
 */
// FUNCTION: FF7 0x004026d4
void __cdecl Swirl_UpdateAndDrawGrid(GfxEngineContext *context, int active)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[140];
        int objectFlags;
        struct { unsigned char pad_to_4[0x4]; GfxMatrix4x4 rotMatrix; };
        struct { unsigned char pad_to_44[0x44]; GfxHomogeneousPoint pointX; };
        struct { unsigned char pad_to_50[0x50]; GfxHomogeneousPoint rotatedX; };
        struct { unsigned char pad_to_5C[0x5C]; GameModule module; };
        struct { unsigned char pad_to_78[0x78]; int colorWord; };
        struct { unsigned char pad_to_79[0x79]; char tintByte; };
        struct { unsigned char pad_to_7A[0x7A]; char tintByteCopy; };
        struct { unsigned char pad_to_7B[0x7B]; char recordByte; };
        struct { unsigned char pad_to_7C[0x7C]; struct SwirlRecord *swirlRecord; };
        struct { unsigned char pad_to_80[0x80]; int rowIndex; };
        struct { unsigned char pad_to_84[0x84]; int colIndex; };
        struct { unsigned char pad_to_88[0x88]; int angleStep; };
    } frame;
#pragma pack(pop)

    frame.swirlRecord = g_pSwirlRecord;
    if (active != 0) {
        g_pSwirlRecord->frameCounter++;
    }
    if (g_pSwirlRecord->frameCounter > 0x2E) {
        if (g_pSwirlRecord->fade >= 0) {
            g_pSwirlRecord->fade -= 4;
        } else {
            g_pSwirlRecord->fade = 0;
        }
        frame.recordByte = *(char *)&frame.swirlRecord->packets[0][0].bytes[7];
        *(char *)&frame.colorWord = (char)g_pSwirlRecord->fade;
        frame.tintByte = (char)g_pSwirlRecord->fade;
        frame.tintByteCopy = (char)g_pSwirlRecord->fade;
        for (frame.rowIndex = 0; frame.rowIndex < 8; frame.rowIndex++) {
            for (frame.colIndex = 0; frame.colIndex < 0xA; frame.colIndex++) {
                frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].dwords[1] = frame.colorWord;
            }
        }
        if (g_pSwirlRecord->frameCounter > 0x4E) {
            frame.module.enter = GameModeDispatcher_Enter;
            frame.module.frame = GameModeDispatcher_Frame;
            frame.module.exit = GameModeDispatcher_Exit;
            frame.module.onKey = App_OnKeyMessage;
            frame.module.onMouse = App_OnMouseMessage;
            RequestModuleSwitch(&frame.module, context);
            g_SwirlResetViewport = 1;
        }
    }
    RotMatrixXYZ(&g_pSwirlRecord->rotX, &g_SwirlTransformMatrix);
    ScaleMatrix(&g_SwirlTransformMatrix, &g_pSwirlRecord->scaleX);
    if (active != 0) {
        frame.angleStep = g_pSwirlRecord->spinSpeed;
        g_pSwirlRecord->rotZ -= (short)(int)((float)g_pSwirlRecord->frameCounter * 0.75 + 5.0);
        g_pSwirlRecord->spinSpeed += (short)(int)((float)g_pSwirlRecord->frameCounter * 0.1 + 8.0);
        g_pSwirlRecord->scaleX += frame.angleStep;
        g_pSwirlRecord->scaleY += frame.angleStep;
    }
    for (frame.rowIndex = 0; frame.rowIndex < 9; frame.rowIndex++) {
        for (frame.colIndex = 0; frame.colIndex < 0xB; frame.colIndex++) {
            GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)&g_SwirlTransformMatrix, frame.rotMatrix.v);
            frame.pointX.x = (float)g_pSwirlRecord->points[frame.colIndex][frame.rowIndex][0];
            frame.pointX.y = (float)g_pSwirlRecord->points[frame.colIndex][frame.rowIndex][1];
            frame.pointX.z = (float)g_pSwirlRecord->points[frame.colIndex][frame.rowIndex][2];
            RotateVector3ColVec(frame.rotMatrix.v, &frame.pointX.x, &frame.rotatedX.x);
            g_pSwirlRecord->projected[frame.colIndex][frame.rowIndex][0] = (short)(int)frame.rotatedX.x + g_SwirlGridCenterX + g_SwirlOriginX;
            g_pSwirlRecord->projected[frame.colIndex][frame.rowIndex][1] = (short)(int)frame.rotatedX.y + g_SwirlGridCenterY + g_SwirlOriginY;
        }
    }
    frame.objectFlags = 0x100000;
    Gfx_OrRenderObjectPolygonSetFlags(frame.objectFlags, Gfx_GetOffscreenSlotRenderObject(g_SwirlSpriteHandleFirst));
    Gfx_OrRenderObjectPolygonSetFlags(frame.objectFlags, Gfx_GetOffscreenSlotRenderObject(g_SwirlSpriteHandleThird));
    for (frame.rowIndex = 0; frame.rowIndex < 8; frame.rowIndex++) {
        for (frame.colIndex = 0; frame.colIndex < 0xA; frame.colIndex++) {
            frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].words[4] = g_pSwirlRecord->projected[frame.colIndex][frame.rowIndex][0];
            frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].words[5] = g_pSwirlRecord->projected[frame.colIndex][frame.rowIndex][1];
            frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].words[8] = g_pSwirlRecord->projected[(frame.colIndex + 1)][frame.rowIndex][0];
            frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].words[9] = g_pSwirlRecord->projected[(frame.colIndex + 1)][frame.rowIndex][1];
            frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].words[12] = g_pSwirlRecord->projected[frame.colIndex][frame.rowIndex + 1][0];
            frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].words[13] = g_pSwirlRecord->projected[frame.colIndex][frame.rowIndex + 1][1];
            frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].words[16] = g_pSwirlRecord->projected[(frame.colIndex + 1)][frame.rowIndex + 1][0];
            frame.swirlRecord->packets[frame.colIndex][frame.rowIndex].words[17] = g_pSwirlRecord->projected[(frame.colIndex + 1)][frame.rowIndex + 1][1];
            if (Gfx_IsDrawEnabled() != 0) {
                if (active != 0) {
                    if (frame.colIndex < 5) {
                        Swirl_DrawPacketQuad((char *)&frame.swirlRecord->packets[frame.colIndex][frame.rowIndex], Gfx_GetOffscreenSlotRenderObject(g_SwirlSpriteHandleFirst));
                    } else {
                        Swirl_DrawPacketQuad((char *)&frame.swirlRecord->packets[frame.colIndex][frame.rowIndex], Gfx_GetOffscreenSlotRenderObject(g_SwirlSpriteHandleThird));
                    }
                } else if (frame.colIndex < 5) {
                    Swirl_DrawPacketQuad((char *)&frame.swirlRecord->packets[frame.colIndex][frame.rowIndex], Gfx_GetOffscreenSlotRenderObject(g_SwirlSpriteHandleSecond));
                } else {
                    Swirl_DrawPacketQuad((char *)&frame.swirlRecord->packets[frame.colIndex][frame.rowIndex], Gfx_GetOffscreenSlotRenderObject(g_SwirlSpriteHandleFourth));
                }
            }
        }
    }
}

/*
 * Resolves CLUT/texture coordinates and submits a single textured quad to the screen.
 */
// FUNCTION: FF7 0x00402d92
void __cdecl Swirl_DrawPacketQuad(char *packet, int context)
{
    struct {
        int tex;
        int rect[5];
        int color;
        int alpha;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.alpha = 64;
    Gfx_DecodePsxClutPosition(((YamaSwirlQuadPacket *)packet)->clutWord, ((YamaSwirlQuadPacket *)packet)->texPage, context, (GfxPsxClutPosition *)frame.rect);
    frame.tex = frame.rect[4];
    Gfx_PackDoubledColorToBgra(packet[4], packet[5], packet[6], frame.alpha, (GfxBgraPixel *)&frame.color);
    Gfx_EmitPsxTexturedFlatQuadScreen(frame.color, (float)((YamaSwirlQuadPacket *)packet)->x0, (float)((YamaSwirlQuadPacket *)packet)->y0,
                                      (float)((YamaSwirlQuadPacket *)packet)->x1, (float)((YamaSwirlQuadPacket *)packet)->y1,
                                      (float)((YamaSwirlQuadPacket *)packet)->x2, (float)((YamaSwirlQuadPacket *)packet)->y2,
                                      (float)((YamaSwirlQuadPacket *)packet)->x3, (float)((YamaSwirlQuadPacket *)packet)->y3,
                                      (unsigned char)packet[12], (unsigned char)packet[13],
                                      (unsigned char)packet[20], (unsigned char)packet[21],
                                      (unsigned char)packet[28], (unsigned char)packet[29],
                                      (unsigned char)packet[36], (unsigned char)packet[37],
                                      frame.tex, 0, 1.0f, (int)(struct GfxPsxQuadEmitter *)context);
}

/*
 * Swirl module keyboard callback (no-op in original).
 */
// FUNCTION: FF7 0x00402edd
void __cdecl Swirl_ModuleKeyboard(void)
{
}

/*
 * Swirl module mouse callback (no-op in original).
 */
// FUNCTION: FF7 0x00402ee2
void __cdecl Swirl_ModuleMouse(void)
{
}
} /* extern "C" */

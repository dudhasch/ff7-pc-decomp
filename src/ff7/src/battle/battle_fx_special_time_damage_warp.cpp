// src/ff7/src/battle/battle_fx_special_time_damage_warp.cpp
/* Enemy Special Attack [119]: Time Damage (orig. SPECIAL/TIMEDMG, タイムダメージ) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Timedmg attack: builds and draws a screen warp grid that
//   distorts the view, with an orbiting and rotating sprite pair around the target and projection
//   of actor points to screen space.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/enums/main.h"
#include "lib/enums/graphics.h"
extern "C" {

struct GfxPsxSpriteQuadState;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008b3f98
int g_BattleSpecTimeDamageResourceDefs = 0x00196A39;
int g_BattleSpecTimeDamageResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialTimeDamageWarpResourceNames + 0x8)
};
int g_BattleSpecTimeDamageResourceDefsDataTail[9] = {
    (int)((char *)&g_BattleFxSpecialTimeDamageWarpResourceNames + 0x14), 0x0, 0x0, 0x196a39, (int)((char *)&g_BattleFxSpecialTimeDamageWarpResourceNames + 0x20), (int)((char *)&g_BattleFxSpecialTimeDamageWarpResourceNames + 0x2c),
    0x0, 0x0, 0x78
};
// The radius of the warp grid mesh in pixels.
// GLOBAL: FF7 0x008b3fc4
int g_BattleSpecTimeDamageGridRadius = 60;
// The offscreen render slot of the effect (-1 when none).
// GLOBAL: FF7 0x008b3fc8
int g_BattleSpecTimeDamageOffscreenSlot[2] = {-1, 0};
// GLOBAL: FF7 0x008b3fd0
int g_BattleSpecTimeDamageScale = 16384;
// GLOBAL: FF7 0x008b3fd8
struct BattleFxTimeDamageTransform g_BattleSpecTimeDamageTransformMatrix1 = {{0, 0x40, 0, 0, 0, 0, 0, 0, 0, 0x40, 0, 0, 0, 0, 0, 0, 0, 0}, 0, {0, 0}, 0, 0};
// GLOBAL: FF7 0x008b3ff8
BattleFxTimeDamageColor g_BattleSpecTimeDamageColor1 = {-128, -128, -128, 0x2e};
unsigned char g_rawData_008b4000[8] = {0xe8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008b4008
int g_BattleSpecTimeDamageRadius = 1024;
// GLOBAL: FF7 0x008b4010
struct BattleSpecTimeDamageTransform g_BattleSpecTimeDamageTransformMatrix2 = {{0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x008b4030
BattleFxTimeDamageColor g_BattleSpecTimeDamageColor2[2] = {{-128, -128, -128, 44}, {0}};
// GLOBAL: FF7 0x008b4038
unsigned char g_BattleSpecTimeDamageStep[16] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008b4048
unsigned char g_BattleSpecTimeDamageDistance[8] = {0x00, 0x02, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00};
unsigned char g_BattleFxSpecialTimeDamageWarpResourceNames[56] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x49, 0x4d, 0x45, 0x44, 0x4d, 0x47, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x48, 0x41, 0x52, 0x49, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00, 0x54, 0x49, 0x4d, 0x45, 0x44, 0x4d, 0x47, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x54, 0x41, 0x4d, 0x41, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00bb2190
BattleFxWarpMeshPoint g_BattleSpecTimeDamageGridMesh[17][16] = {0};
// The float bits of the last projected point's NDC w, passed on as the second blend factor.
// GLOBAL: FF7 0x00bb25d0
int g_BattleSpecTimeDamageProjectedNdcW = 0;
// GLOBAL: FF7 0x00bb25d4
float g_BattleSpecTimeDamageProjectedNdcZ = 0.0f;
// GLOBAL: FF7 0x00bb25d8
unsigned char g_BattleSpecTimeDamageGridRecords[24534] = {0};
unsigned char g_bssPad_00bb85b0[40] = {0};
// The target's projected screen position, clamped into the view.
// GLOBAL: FF7 0x00bb85d8
BattleFxWarpMeshPoint g_BattleSpecTimeDamageTargetScreenPos = {0};
// The geometry offset (screen centre) the grid is projected around, set from the UI scaling mode.
// GLOBAL: FF7 0x00bb85dc
int g_BattleSpecTimeDamageGeomOffsetX = 0;
// The centre of the 3D view in screen units.
// GLOBAL: FF7 0x00bb85e0
int g_BattleSpecTimeDamageViewCenterX = 0;
// GLOBAL: FF7 0x00bb85e4
int g_BattleSpecTimeDamageGeomOffsetY = 0;
// GLOBAL: FF7 0x00bb85e8
int g_BattleSpecTimeDamageViewCenterY = 0;
// GLOBAL: FF7 0x00bb85ec
int g_BattleSpecTimeDamageDrawHandle = 0;
// GLOBAL: FF7 0x00bb85f0
int g_BattleSpecTimeDamageTargetBits = 0;
unsigned char g_bssPad_00bb85f4[4] = {0};
// GLOBAL: FF7 0x00bb85f8
unsigned char g_BattleSpecTimeDamageActorPositions[308] = {0};
unsigned char g_bssPad_00bb872c[4] = {0};
unsigned char g_bssPad_00bb8730[328] = {0};
// The half size of the offscreen rectangle around the target and its full size.
// GLOBAL: FF7 0x00bb8878
int g_BattleSpecTimeDamageRectHalfSize = 0;
// GLOBAL: FF7 0x00bb887c
int g_BattleSpecTimeDamageRectSize = 0;
// GLOBAL: FF7 0x00bb8880
int g_BattleSpecTimeDamageGridScratch[17][16] = {0};
// GLOBAL: FF7 0x00bb8cc0
int g_BattleSpecTimeDamageMode = 0;
// GLOBAL: FF7 0x00bb8cc4
int g_BattleSpecTimeDamagePrimIndex = 0;
// GLOBAL: FF7 0x00bb8cc8
struct GfxTableCursor *g_BattleSpecTimeDamageModelTable = 0;
// GLOBAL: FF7 0x00bb8ccc
int g_BattleSpecTimeDamageSpriteCounter = 0;
// clang-format on

/* The warp grid's 16 x 16 cells; the object is a byte array a few bytes shorter than the cells. */
#define TIME_DAMAGE_CELLS ((BattleFxTimeDamageGridCell (*)[16])g_BattleSpecTimeDamageGridRecords)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Timedmg special effect.
 */
// FUNCTION: FF7 0x00589550
void __cdecl BattleFx_SpecialLoadResTimeDamage(void)
{
    GfxResourceRequest requestBlock;
    char pathBuf[200];
    GfxModelTableBank *sceneHandle;
    int kind;
    GfxResourceRequest *requestPtr;
    int resultDefTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecTimeDamageModelTable == 0) {
        kind = 2;
        resultDefTable = (int)&g_BattleSpecTimeDamageResourceDefs;
        sceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestBlock;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008b4088
        requestPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/TIMEDMG", pathBuf);
        Gfx_LoadModelTableIfAbsent(kind, resultDefTable, requestPtr, &g_BattleSpecTimeDamageModelTable, sceneHandle);
    }
}

/*
 * Entry point for the Timedmg special effect.
 */
// FUNCTION: FF7 0x005895e0
void __cdecl BattleFx_SpecialEnterTimeDamage(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResTimeDamage();
    switch (g_BattleUiScalingMode) {
    case MAIN_GRAPHICS_MODE_320X240:
        g_BattleSpecTimeDamageRectSize = 0x78;
        g_BattleSpecTimeDamageRectHalfSize = 0x3C;
        g_BattleSpecTimeDamageGeomOffsetX = 0xA0;
        g_BattleSpecTimeDamageGeomOffsetY = 0x78;
        g_BattleSpecTimeDamageViewCenterX = 0x50;
        g_BattleSpecTimeDamageViewCenterY = 0x3C;
        g_BattleSpecTimeDamageMode = 1;
        break;
    case MAIN_GRAPHICS_MODE_320X240_CENTERED:
        g_BattleSpecTimeDamageRectSize = 0x78;
        g_BattleSpecTimeDamageRectHalfSize = 0x3C;
        g_BattleSpecTimeDamageGeomOffsetX = 0x140;
        g_BattleSpecTimeDamageGeomOffsetY = 0xF0;
        g_BattleSpecTimeDamageViewCenterX = 0xA0;
        g_BattleSpecTimeDamageViewCenterY = 0x78;
        g_BattleSpecTimeDamageMode = 1;
        break;
    case MAIN_GRAPHICS_MODE_640X480:
        g_BattleSpecTimeDamageRectSize = 0xF0;
        g_BattleSpecTimeDamageRectHalfSize = 0x78;
        g_BattleSpecTimeDamageGeomOffsetX = 0x140;
        g_BattleSpecTimeDamageGeomOffsetY = 0xF0;
        g_BattleSpecTimeDamageViewCenterX = 0xA0;
        g_BattleSpecTimeDamageViewCenterY = 0x78;
        g_BattleSpecTimeDamageMode = 0;
        break;
    }
    BattleFx_SpecialBeginTimeDamage(targetMask, actorIdx);
}

/*
 * Acquires and configures offscreen drawing rectangle for Timedmg.
 */
// FUNCTION: FF7 0x005896f9
void __cdecl BattleFx_SpecialTimeDamageAcquireOffscreenRect(int left, int right)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        GfxOffscreenRequest *request;
        struct { unsigned char pad_to_4[0x4]; short viewportX; };
        struct { unsigned char pad_to_6[0x6]; short viewportY; };
        struct { unsigned char pad_to_8[0x8]; short viewportW; };
        struct { unsigned char pad_to_A[0xA]; short viewportH; };
    } frame;
#pragma pack(pop)

    frame.viewportX = left - g_BattleSpecTimeDamageRectHalfSize;
    frame.viewportY = right - g_BattleSpecTimeDamageRectHalfSize;
    frame.viewportW = g_BattleSpecTimeDamageRectSize + 1;
    frame.viewportH = g_BattleSpecTimeDamageRectSize + 1;
    if (g_BattleSpecTimeDamageOffscreenSlot[0] >= 0) {
        Battle3d_ReleaseOffscreenSlot(g_BattleSpecTimeDamageOffscreenSlot[0]);
        g_BattleSpecTimeDamageOffscreenSlot[0] = 0xFFFFFFFF;
    }
    if (g_pBattle3dScene != 0) {
        frame.request = (GfxOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (frame.request != 0) {
            if (g_BattleModeTransition == 0) {
                frame.request->pageFillMode = GFX_FILL_SOURCE_FRONT_BUFFER;
            }
            frame.request->kind = GFX_OFFSCREEN_REQUEST_NONE;
            frame.request->pageFillEnabled = 1;
            frame.request->renderKind = GFX_DRAW_TEXTURED_QUAD_2D;
            frame.request->blendMode = BLEND_ADDITIVE;
            frame.request->blendMode = BLEND_NONE;
            frame.request->viewportX = frame.viewportX;
            frame.request->viewportY = frame.viewportY;
            frame.request->viewportW = frame.viewportW;
            frame.request->viewportH = frame.viewportH;
            frame.request->pageFillStepX = 1;
            frame.request->pageFillStepY = 1;
            g_BattleSpecTimeDamageOffscreenSlot[0] = frame.request->slotIndex;
        }
    }
}

/*
 * Initializes Timedmg effect and schedules main timeline task.
 */
// FUNCTION: FF7 0x00589827
void __cdecl BattleFx_SpecialBeginTimeDamage(int targetMask, int actorIdx)
{
    struct {
        BattleFxTimeDamageTimeline *slot;
        int i;
        SVECTOR pos;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_BattleSpecTimeDamageTargetBits = targetMask;
    for (local.i = 0; local.i < 10; local.i++) {
        if ((targetMask & (1 << local.i)) != 0) {
            Battle_GetBoneWorldPos(local.i, g_BattleActorRecords[local.i].targetBone, (short *)&local.pos);
            break;
        }
    }
    BattleFx_SpecialTimeDamageBuildWarpGridMesh();
    local.slot = (BattleFxTimeDamageTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTimeDamageProjectActorPointTask);
    local.slot->pos = local.pos;
    local.slot = (BattleFxTimeDamageTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTimeDamageTimelineTask);
    local.slot->ownerSlot = local.i;
    local.slot->pos = local.pos;
}

/*
 * Builds screen warp grid mesh geometry for Timedmg.
 */
// FUNCTION: FF7 0x005898ed
void __cdecl BattleFx_SpecialTimeDamageBuildWarpGridMesh(void)
{
    int fall;
    int row;
    int unusedSpace[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxTimeDamageGridCell *gridCell;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int column;
    int primIdx;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int radius;
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    for (row = 0; row < 0x11; row++) {
        radius = g_BattleSpecTimeDamageGridRadius * row / 0x10;
        fall = (g_BattleSpecTimeDamageGridRadius - radius << 0xC) / g_BattleSpecTimeDamageGridRadius;
        fall = fall * fall >> 0xC;
        for (column = 0; column < 0x10; column++) {
            g_BattleSpecTimeDamageGridMesh[row][column].x = radius * SinFixed((column << 0xC) / 0x10) >> 0xC;
            g_BattleSpecTimeDamageGridMesh[row][column].y = radius * CosFixed((column << 0xC) / 0x10) >> 0xC;
            g_BattleSpecTimeDamageGridScratch[row][column] = fall;
        }
    }
    for (row = 0; row < 0x10; row++) {
        for (column = 0; column < 0x10; column++) {
            gridCell = (BattleFxTimeDamageGridCell *)(row * 0x600 + column * 0x60 + (char *)&g_BattleSpecTimeDamageGridRecords);
            gridCell->corner00 = (BattleFxWarpMeshPoint *)((row << 6) + column * 4 + (char *)&g_BattleSpecTimeDamageGridMesh);
            gridCell->corner01 = (BattleFxWarpMeshPoint *)((row << 6) + (column + 1) % 0x10 * 4 + (char *)&g_BattleSpecTimeDamageGridMesh);
            gridCell->corner10 = (BattleFxWarpMeshPoint *)((row + 1 << 6) + column * 4 + (char *)&g_BattleSpecTimeDamageGridMesh);
            gridCell->corner11 = (BattleFxWarpMeshPoint *)((row + 1 << 6) + (column + 1) % 0x10 * 4 + (char *)&g_BattleSpecTimeDamageGridMesh);
            for (primIdx = 0; primIdx < 1; primIdx++) {
                gridCell->quad[primIdx].command = gridCell->quad[primIdx].command & 0xFFFFFF | 0x9000000;
                gridCell->quad[primIdx].primCode = PSX_PRIM_POLY_FT4;
                gridCell->quad[primIdx].u0 = (gridCell->corner00->x + g_BattleSpecTimeDamageGridRadius) * g_BattleUiScale;
                gridCell->quad[primIdx].v0 = (gridCell->corner00->y + g_BattleSpecTimeDamageGridRadius) * g_BattleUiScale;
                gridCell->quad[primIdx].u1 = (gridCell->corner01->x + g_BattleSpecTimeDamageGridRadius) * g_BattleUiScale;
                gridCell->quad[primIdx].v1 = (gridCell->corner01->y + g_BattleSpecTimeDamageGridRadius) * g_BattleUiScale;
                gridCell->quad[primIdx].u2 = (gridCell->corner10->x + g_BattleSpecTimeDamageGridRadius) * g_BattleUiScale;
                gridCell->quad[primIdx].v2 = (gridCell->corner10->y + g_BattleSpecTimeDamageGridRadius) * g_BattleUiScale;
                gridCell->quad[primIdx].u3 = (gridCell->corner11->x + g_BattleSpecTimeDamageGridRadius) * g_BattleUiScale;
                gridCell->quad[primIdx].v3 = (gridCell->corner11->y + g_BattleSpecTimeDamageGridRadius) * g_BattleUiScale;
                gridCell->quad[primIdx].colorB = 0x80;
                gridCell->quad[primIdx].colorG = 0x80;
                gridCell->quad[primIdx].colorR = 0x80;
                gridCell->quad[primIdx].texPage = 0x10C;
            }
        }
    }
    for (row = 0; row < 1; row++) {
        for (column = 0; column < 0x10; column++) {
            ((BattleFxWarpTriangleRow *)g_BattleSpecTimeDamageActorPositions)[row][column].tag.length = 4;
            ((BattleFxWarpTriangleRow *)g_BattleSpecTimeDamageActorPositions)[row][column].primCode = PSX_PRIM_POLY_F3;
            ((BattleFxWarpTriangleRow *)g_BattleSpecTimeDamageActorPositions)[row][column].colorB = 0;
            ((BattleFxWarpTriangleRow *)g_BattleSpecTimeDamageActorPositions)[row][column].colorG = 0;
            ((BattleFxWarpTriangleRow *)g_BattleSpecTimeDamageActorPositions)[row][column].colorR = 0;
        }
    }
}

/*
 * Main timeline task controlling screen distortion and sprite spawning for Timedmg.
 */
// FUNCTION: FF7 0x00589d38
void __cdecl BattleFx_SpecialTimeDamageTimelineTask(void)
{
    BattleFxTimeDamageTimeline *pTimeline;
    BattleFxTimeDamageTimelineRecord *pChild;
    int ringIndex;
    int w;
    int colour;
    int texU;
    int texV;
    int screenX;
    int spriteIdx;
    int spriteHeight;
    int rot;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pTimeline = (BattleFxTimeDamageTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (DEAD_IN_RETAIL != 0) {
        spriteIdx = g_BattleSpecTimeDamageSpriteCounter % 0x200;
        g_BattleSpecTimeDamageSpriteCounter++;
        screenX = 0;
        w = 0x100;
        spriteHeight = 0x100;
        texU = 0;
        texV = 0;
        rot = 0;
        colour = 0xFFFFFFFF;
        if (Battle3d_GetOffscreenRenderObject(g_BattleSpecTimeDamageOffscreenSlot[0]) != 0) {
            Gfx_EmitPsxTexturedSpriteQuad(spriteIdx, screenX, w, spriteHeight, texU, texV, rot, FLOAT_ONE_BITS, colour, 0, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSpecTimeDamageOffscreenSlot[0]));
        }
    }
    if (pTimeline->age == 0) {
        Battle_StartFixedPointRamp(0x400, 4);
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    }
    if (pTimeline->age == 4) {
        BattleFx_SpecialTimeDamageAcquireOffscreenRect(g_BattleSpecTimeDamageTargetScreenPos.x, g_BattleSpecTimeDamageTargetScreenPos.y);
        pChild = (BattleFxTimeDamageTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTimeDamageWarpGridTask);
        pChild->pos = pTimeline->pos;
        for (ringIndex = 0; ringIndex < 4; ringIndex++) {
            pChild = (BattleFxTimeDamageTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTimeDamageOrbitingSpriteRingTask);
            pChild->ownerSlot = 0;
            pChild->angleY = 0;
            pChild->angleX = 0;
            pChild->angleZ = (ringIndex << 0xB) / 4;
            pChild->pos = pTimeline->pos;
        }
        pChild = (BattleFxTimeDamageTimelineRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTimeDamageRotatingSpritePairTask);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&pTimeline->pos), 0x181);
    }
    if (pTimeline->age == 0x27) {
        Battle_PostActorHitOrHurtMessage(pTimeline->ownerSlot);
        pTimeline->ownerSlot = BATTLE_TASK_FREE;
        if (g_BattleSpecTimeDamageOffscreenSlot[0] >= 0) {
            Battle3d_ReleaseOffscreenSlot(g_BattleSpecTimeDamageOffscreenSlot[0]);
            g_BattleSpecTimeDamageOffscreenSlot[0] = 0xFFFFFFFF;
        }
    }
    (pTimeline->age)++;
}

/*
 * Updates warp grid animation state for Timedmg.
 */
// FUNCTION: FF7 0x00589fae
void __cdecl BattleFx_SpecialTimeDamageWarpGridTask(void)
{
    int amount;
    int localFrame;
    int spreadVal;
    BattleTaskSlot *task;
    int padLocal[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int rotation;

    task = BATTLE_CURRENT_TASK_SLOT;
    localFrame = task->frameCounter;
    if (localFrame < 0xA) {
        amount = localFrame * 0x5DC / 0xA;
        rotation = 0;
        spreadVal = 0;
    } else {
        localFrame -= 0xA;
        if (localFrame < 5) {
            amount = 0x5DC;
            rotation = 0;
            spreadVal = 0;
        } else {
            localFrame -= 5;
            if (localFrame < 5) {
                amount = localFrame * -1500 / 5 + 0x5DC;
                rotation = -task->frameCounter << 9;
                spreadVal = (localFrame << 9) / 5;
            } else {
                localFrame -= 5;
                if (localFrame < 0xF) {
                    amount = 0;
                    rotation = -task->frameCounter << 9;
                    spreadVal = 0x200;
                } else {
                    localFrame -= 0xF;
                    if (localFrame < 0xA) {
                        amount = 0;
                        rotation = -task->frameCounter << 9;
                        spreadVal = localFrame * -512 / 0xA + 0x200;
                    } else {
                        task->ownerSlot = BATTLE_TASK_FREE;
                        return;
                    }
                }
            }
        }
    }
    BattleFx_SpecialTimeDamageBuildWarpGridOffsets(amount, rotation, spreadVal);
    BattleFx_SpecialTimeDamageDrawWarpGrid(g_BattleSpecTimeDamageTargetScreenPos.x, g_BattleSpecTimeDamageTargetScreenPos.y);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Transforms and submits screen distortion warp grid for Timedmg.
 */
// FUNCTION: FF7 0x0058a136
void __cdecl BattleFx_SpecialTimeDamageDrawWarpGrid(int deltaX, int deltaY)
{
    int row;
    int column;

    for (row = 0; row < 0x10; row++) {
        for (column = 0; column < 0x10; column++) {
            TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex].x0 = TIME_DAMAGE_CELLS[row][column].corner00->x + deltaX;
            TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex].y0 = TIME_DAMAGE_CELLS[row][column].corner00->y + deltaY;
            TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex].x1 = TIME_DAMAGE_CELLS[row][column].corner01->x + deltaX;
            TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex].y1 = TIME_DAMAGE_CELLS[row][column].corner01->y + deltaY;
            TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex].x2 = TIME_DAMAGE_CELLS[row][column].corner10->x + deltaX;
            TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex].y2 = TIME_DAMAGE_CELLS[row][column].corner10->y + deltaY;
            TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex].x3 = TIME_DAMAGE_CELLS[row][column].corner11->x + deltaX;
            TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex].y3 = TIME_DAMAGE_CELLS[row][column].corner11->y + deltaY;
            Gfx_SetBlendFactorPair(FLOAT_0_95_BITS, g_BattleSpecTimeDamageProjectedNdcW);
            Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSpecTimeDamageOffscreenSlot[0]));
            Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleSpecTimeDamageOffscreenSlot[0]));
            Gfx_DrawPsxPrimitive(&TIME_DAMAGE_CELLS[row][column].quad[g_BattleSpecTimeDamagePrimIndex]);
        }
    }
}

/*
 * Computes radial distortion vertex offsets for Timedmg warp grid.
 */
// FUNCTION: FF7 0x0058a3fd
void __cdecl BattleFx_SpecialTimeDamageBuildWarpGridOffsets(int depthScale, int phase, int amplitude)
{
    int screenPosition;
    int screenY;
    int column;
    int originX;
    int rowIdx;
    int length;
    int pointer;
    int rowRadius;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int dstCenterY;
    int warpScale;
    BattleFxTimeDamageWarpPoint *warpPointPtr;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    Vec3sPacked *scratchVector;

    scratchVector = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    warpPointPtr = (BattleFxTimeDamageWarpPoint *)((int)scratchVector + 8);
    pointer = (int)warpPointPtr + 4;
    g_BattleSpecTimeDamageTransformMatrix1.geomScreenX4 = GetGeomScreen() << 2;
    GetGeomOffset(&originX, &dstCenterY);
    originX = g_BattleSpecTimeDamageGeomOffsetX;
    dstCenterY = g_BattleSpecTimeDamageGeomOffsetY;
    SetRotMatrix(&g_BattleSpecTimeDamageScale);
    SetTransMatrix(&g_BattleSpecTimeDamageScale);
    for (rowIdx = 0; rowIdx < 0x10; rowIdx++) {
        rowRadius = (g_BattleSpecTimeDamageGridRadius * rowIdx) / 16;
        for (column = 0; column < 0x10; column++) {
            scratchVector->vx = rowRadius * SinFixed(((column << 0xC) / 16)) >> 0xC;
            scratchVector->vy = rowRadius * CosFixed(((column << 0xC) / 16)) >> 0xC;
            scratchVector->vz = depthScale * g_BattleSpecTimeDamageGridScratch[rowIdx][column] >> 0xC;
            ProjectPointHardware(scratchVector, (short *)warpPointPtr, &screenPosition, &screenY);
            if (amplitude != 0) {
                warpPointPtr->offsetX -= originX;
                warpPointPtr->offsetY -= dstCenterY;
                length = SqrtToInt(warpPointPtr->offsetX * warpPointPtr->offsetX + warpPointPtr->offsetY * warpPointPtr->offsetY);
                warpScale = (SinFixed(length * 0x64 + phase) * amplitude >> 0xC) + 0x1000;
                warpPointPtr->offsetX = (warpPointPtr->offsetX * warpScale >> 0xC) + originX;
                warpPointPtr->offsetY = (warpPointPtr->offsetY * warpScale >> 0xC) + dstCenterY;
            }
            g_BattleSpecTimeDamageGridMesh[rowIdx][column].x = warpPointPtr->offsetX - originX;
            g_BattleSpecTimeDamageGridMesh[rowIdx][column].y = warpPointPtr->offsetY - dstCenterY;
        }
    }
}

/*
 * Updates and renders orbiting sprite ring around target for Timedmg.
 */
// FUNCTION: FF7 0x0058a629
void __cdecl BattleFx_SpecialTimeDamageOrbitingSpriteRingTask(void)
{
    BattleFxTimeDamageOrbitRing *pPt2d;
    Vec3sPacked *vertPtr;
    int size;
    int flagOut;
    BattleFxOrbitRingSprite *taskPtr;
    int spokeIndex;
    int offsetX;
    PsxMatrix *matrix;
    int offsetY;
    int block;
    PsxMatrix *pSpriteXform;
    int rawDepth;
    int stage;

    taskPtr = (BattleFxOrbitRingSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecTimeDamageTransformMatrix1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecTimeDamageModelTable);
    (g_BattleSpecTimeDamageTransformMatrix1.drawObj) = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecTimeDamageModelTable);
    block = Gfx_GetPsxScratchpadPtr(0);
    vertPtr = (Vec3sPacked *)(block + 8);
    matrix = (PsxMatrix *)((int)vertPtr + 8);
    pPt2d = (BattleFxTimeDamageOrbitRing *)((char *)matrix + 0x1E);
    pSpriteXform = (PsxMatrix *)((int)pPt2d + 4);
    stage = taskPtr->frameIndex;
    if (stage < 0xA) {
        g_BattleSpecTimeDamageColor1.blue = (stage << 7) / 0xA;
        g_BattleSpecTimeDamageColor1.green = g_BattleSpecTimeDamageColor1.blue;
        g_BattleSpecTimeDamageColor1.red = g_BattleSpecTimeDamageColor1.green;
        size = stage * 0x3E8 / 0xA;
    } else {
        stage -= 0xA;
        if (stage < 0x19) {
            g_BattleSpecTimeDamageColor1.blue = 0x80;
            g_BattleSpecTimeDamageColor1.green = g_BattleSpecTimeDamageColor1.blue;
            g_BattleSpecTimeDamageColor1.red = g_BattleSpecTimeDamageColor1.green;
            size = 0x3E8;
        } else {
            stage -= 0x19;
            if (stage < 0xA) {
                g_BattleSpecTimeDamageColor1.blue = stage * -0x80 / 0xA + 0x80;
                g_BattleSpecTimeDamageColor1.green = g_BattleSpecTimeDamageColor1.blue;
                g_BattleSpecTimeDamageColor1.red = g_BattleSpecTimeDamageColor1.green;
                size = stage * 0xFA0 / 0xA + 0x3E8;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    GetGeomOffset(&offsetX, &offsetY);
    if (g_BattleSpecTimeDamageMode != 0 && g_BattleVariationIndex == 2) {
        offsetX = g_BattleSpecTimeDamageViewCenterX >> 1;
    } else {
        offsetX = g_BattleSpecTimeDamageViewCenterX;
    }
    offsetY = g_BattleSpecTimeDamageViewCenterY;
    RotMatrix((SVECTOR *)&taskPtr->angleX, matrix);
    matrix->t[0] = 0;
    matrix->t[1] = 0;
    matrix->t[2] = GetGeomScreen() * 5;
    for (spokeIndex = 0; spokeIndex < 0x10; spokeIndex++) {
        SetRotMatrix(matrix);
        SetTransMatrix(matrix);
        vertPtr->vx = SinFixed(((spokeIndex << 0xC) / 16)) * size >> 0xC;
        vertPtr->vy = SinFixed(((spokeIndex << 0xE) / 16)) * size >> 0xF;
        vertPtr->vz = CosFixed(((spokeIndex << 0xC) / 16)) * size >> 0xC;
        ProjectPointHardware(vertPtr, (short *)pPt2d, &flagOut, &rawDepth);
        pSpriteXform->t[0] = pPt2d->screenX - g_BattleSpecTimeDamageGeomOffsetX + (((g_BattleSpecTimeDamageTargetScreenPos.x) / 2) - offsetX) * 5;
        pSpriteXform->t[1] = pPt2d->screenY - g_BattleSpecTimeDamageGeomOffsetY + (((g_BattleSpecTimeDamageTargetScreenPos.y) / 2) - offsetY) * 5;
        pSpriteXform->t[2] = matrix->t[2];
        SetRotMatrix(&g_BattleSpecTimeDamageRadius);
        SetTransMatrix(pSpriteXform);
        g_BattleSpecTimeDamageDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecTimeDamageTransformMatrix1.spriteGroup, g_BattleCurrentOrderingTable + 0x4088, 0, g_BattleSpecTimeDamageDrawHandle);
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->frameIndex < 0x23) {
            taskPtr->angleX += 0x10;
            taskPtr->angleY += 0xC0;
            taskPtr->angleZ += 0x40;
        }
        (taskPtr->frameIndex)++;
    }
}

/*
 * Updates and renders rotating sprite pair for Timedmg.
 */
// FUNCTION: FF7 0x0058a9c2
void __cdecl BattleFx_SpecialTimeDamageRotatingSpritePairTask(void)
{
    int stage;
    BattleTaskSlot *taskPtr;
    BattleFxRotatingSpritePair *rotRecord;
    int offsetX;
    PsxMatrix *matrix;
    int offsetY;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecTimeDamageTransformMatrix2.spriteRecord) = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecTimeDamageModelTable);
    (g_BattleSpecTimeDamageTransformMatrix2.spriteObject) = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecTimeDamageModelTable);
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    rotRecord = (BattleFxRotatingSpritePair *)((char *)matrix + 0x1E);
    stage = taskPtr->frameCounter;
    if (stage < 0xA) {
        g_BattleSpecTimeDamageColor2[0].blue = (stage << 7) / 0xA;
        g_BattleSpecTimeDamageColor2[0].green = g_BattleSpecTimeDamageColor2[0].blue;
        g_BattleSpecTimeDamageColor2[0].red = g_BattleSpecTimeDamageColor2[0].green;
        g_BattleSpecTimeDamageColor2[0].primCode = PSX_PRIM_POLY_FT4_TRANS;
    } else {
        stage -= 0xA;
        if (stage < 0x19) {
            *(int *)&g_BattleSpecTimeDamageColor2 = 0x2C808080;
        } else {
            stage -= 0x19;
            if (stage < 0xA) {
                g_BattleSpecTimeDamageColor2[0].blue = stage * -0x80 / 0xA + 0x80;
                g_BattleSpecTimeDamageColor2[0].green = g_BattleSpecTimeDamageColor2[0].blue;
                g_BattleSpecTimeDamageColor2[0].red = g_BattleSpecTimeDamageColor2[0].green;
                g_BattleSpecTimeDamageColor2[0].primCode = PSX_PRIM_POLY_FT4_TRANS;
            } else {
                taskPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    GetGeomOffset(&offsetX, &offsetY);
    if (g_BattleSpecTimeDamageMode != 0 && g_BattleVariationIndex == 2) {
        offsetX = g_BattleSpecTimeDamageViewCenterX >> 1;
    } else {
        offsetX = g_BattleSpecTimeDamageViewCenterX;
    }
    offsetY = g_BattleSpecTimeDamageViewCenterY;
    rotRecord->rotX = 0;
    rotRecord->rotY = 0;
    rotRecord->angle = taskPtr->frameCounter << 8;
    RotMatrix((SVECTOR *)rotRecord, matrix);
    ScaleMatrix(matrix, &g_BattleSpecTimeDamageStep);
    matrix->t[0] = ((g_BattleSpecTimeDamageTargetScreenPos.x) / 2) - offsetX << 1;
    matrix->t[1] = ((g_BattleSpecTimeDamageTargetScreenPos.y) / 2) - offsetY << 1;
    matrix->t[2] = GetGeomScreen() << 1;
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleSpecTimeDamageDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecTimeDamageTransformMatrix2.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleSpecTimeDamageDrawHandle);
    rotRecord->angle = rotRecord->angle / 0xC;
    RotMatrix((SVECTOR *)rotRecord, matrix);
    ScaleMatrix(matrix, &g_BattleSpecTimeDamageDistance);
    SetRotMatrix(matrix);
    g_BattleSpecTimeDamageDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecTimeDamageTransformMatrix2.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleSpecTimeDamageDrawHandle);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
    }
}

/*
 * Projects actor bone world position to screen coordinates.
 */
// FUNCTION: FF7 0x0058ac59
void __cdecl BattleFx_SpecialTimeDamageProjectActorPointTask(void)
{
    BattleFxTimeDamageProjectTask *task;

    task = (BattleFxTimeDamageProjectTask *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialTimeDamageProjectClampedPoint(&task->pos, (short *)&g_BattleSpecTimeDamageTargetScreenPos);
    task->toggle ^= 1;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        Battle_StartFixedPointRamp(0, 4);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Projects 3D vector to screen space with boundary clamping.
 */
// FUNCTION: FF7 0x0058acb9
void __cdecl BattleFx_SpecialTimeDamageProjectClampedPoint(void *point3d, short *screenPoint)
{
    int projX;
    int projY;

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    ProjectPointHardware(point3d, screenPoint, &projX, &projY);
    *(float *)&g_BattleSpecTimeDamageProjectedNdcW = GetLastProjectedNdcW();
    g_BattleSpecTimeDamageProjectedNdcZ = GetLastProjectedNdcZ();
    if (screenPoint[0] < g_BattleViewportOriginX + g_BattleSpecTimeDamageRectHalfSize) {
        screenPoint[0] = g_BattleViewportOriginX + g_BattleSpecTimeDamageRectHalfSize;
    } else if (screenPoint[0] > g_BattleViewportMaxX - g_BattleSpecTimeDamageRectHalfSize) {
        screenPoint[0] = g_BattleViewportMaxX - g_BattleSpecTimeDamageRectHalfSize;
    }
    if (screenPoint[1] < g_BattleViewportOriginY + g_BattleSpecTimeDamageRectHalfSize) {
        screenPoint[1] = g_BattleViewportOriginY + g_BattleSpecTimeDamageRectHalfSize;
    } else if (screenPoint[1] > (*(int *)&g_BattleViewportMaxY) - g_BattleSpecTimeDamageRectHalfSize) {
        screenPoint[1] = (*(int *)&g_BattleViewportMaxY) - g_BattleSpecTimeDamageRectHalfSize;
    }
}
} /* extern "C" */

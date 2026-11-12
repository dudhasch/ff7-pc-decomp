// src/ff7/src/main/main_game_over_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the game-over screen: it builds the fade overlay and screen quads at three
//   resolutions, loads its textures from the disc archive, and draws each frame. It also handles
//   keyboard and mouse input and the transition to the next game mode when the screen exits.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "lib/pfile.h"
#include "lib/enums/file.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
#include "lib/types/graphics.h"
#include "ff7/types/main.h"
#include "ff7/enums/main.h"
#include "ff7/api/field.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include "ff7/field.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x007ba668
int g_GameOverIntroFadeAlpha = 255;
// GLOBAL: FF7 0x009a0550
struct GfxRenderObject *g_GameOverRenderObjectsMode2[6] = {0};
// GLOBAL: FF7 0x009a0568
struct GfxRenderObject *g_GameOverRenderObjectsMode01[2] = {0};
// GLOBAL: FF7 0x009a0570
struct GfxRenderObject *g_GameOverFadeRenderObject = 0;
// GLOBAL: FF7 0x009a0574
int g_GameOverViewportHeight = 0;
// GLOBAL: FF7 0x009a0578
int g_GameOverExitTriggered = 0;
// GLOBAL: FF7 0x009a057c
int g_GameOverViewportX = 0;
// GLOBAL: FF7 0x009a0580
int g_GameOverViewportWidth = 0;
// GLOBAL: FF7 0x009a0584
int g_GameOverFadeOutActive = 0;
// GLOBAL: FF7 0x009a0588
int g_GameOverViewportY = 0;
// GLOBAL: FF7 0x009a058c
int g_GameOverFadeInActive = 0;
// GLOBAL: FF7 0x009a0590
int g_GameOverFadeOutAlpha = 0;
// Empty load-option path byte at the end of this TU's .bss span ahead of initpath.c.
// GLOBAL: FF7 0x009a0594
char g_GameOverEmptyPath[1] = {0};
// clang-format on
#pragma intrinsic(strcmp, memcmp, memset, strcpy, strcat)
#pragma data_seg(".u401tim")
#pragma data_seg()
// Placed after #pragma data_seg() so VC5 emits g_GameOverTimNamesMode2 in the default .data section.
// GLOBAL: FF7 0x007ba670
char *g_GameOverTimNamesMode2[6] = {
    // STRING: FF7 0x007ba690
    "e_over_a.tim",
    // STRING: FF7 0x007ba6a0
    "e_over_b.tim",
    // STRING: FF7 0x007ba6b0
    "e_over_c.tim",
    // STRING: FF7 0x007ba6c0
    "e_over_d.tim",
    // STRING: FF7 0x007ba6d0
    "e_over_e.tim",
    // STRING: FF7 0x007ba6e0
    "e_over_f.tim",
};
// GLOBAL: FF7 0x007ba688
char *g_GameOverTimNamesMode01[2] = {
    // STRING: FF7 0x007ba6f0
    "e_over_la.tim",
    // STRING: FF7 0x007ba700
    "e_over_lb.tim",
};
// The scene calls below use this TU's signatures; the snobo_primitives.h declarations take int arguments.


/*
 * Fills a 4-vertex full-screen black fade quad with the given alpha value.
 */
// FUNCTION: FF7 0x00404e00
void __cdecl GameOver_FillFadeQuad(unsigned char alpha)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        GfxTlVertex *vertexBottomRight;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *vertexTopRight; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *vertexBottomLeft; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *vertexTopLeft; };
        struct { unsigned char pad_to_10[0x10]; int vertexZ; };
        struct { unsigned char pad_to_14[0x14]; int fadeColor; };
        struct { unsigned char pad_to_15[0x15]; char tmpColorA; };
        struct { unsigned char pad_to_16[0x16]; char tmpColorB; };
        struct { unsigned char pad_to_17[0x17]; char fadeAlpha; };
    } frame;
#pragma pack(pop)

    *(char *)&frame.fadeColor = 0;
    frame.tmpColorA = *(char *)&frame.fadeColor;
    frame.tmpColorB = frame.tmpColorA;
    frame.fadeAlpha = alpha;
    frame.vertexZ = 0;
    if (AcquireBatchNode(1, g_GameOverFadeRenderObject) != 0) {
        frame.vertexTopLeft = (GfxTlVertex *)(g_GameOverFadeRenderObject->vertexCursor);
        frame.vertexTopLeft->x = (float)g_GameOverViewportX;
        frame.vertexTopLeft->y = (float)g_GameOverViewportY;
        frame.vertexTopLeft->zBits = frame.vertexZ;
        frame.vertexTopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.vertexTopLeft->color = frame.fadeColor;
        frame.vertexTopLeft->specular = 0xFF000000;
        frame.vertexBottomLeft = (GfxTlVertex *)(g_GameOverFadeRenderObject->vertexCursor + 0x20);
        frame.vertexBottomLeft->x = (float)g_GameOverViewportX;
        frame.vertexBottomLeft->y = (float)g_GameOverViewportY + g_GameOverViewportHeight;
        frame.vertexBottomLeft->zBits = frame.vertexZ;
        frame.vertexBottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.vertexBottomLeft->color = frame.fadeColor;
        frame.vertexBottomLeft->specular = 0xFF000000;
        frame.vertexTopRight = (GfxTlVertex *)(g_GameOverFadeRenderObject->vertexCursor + 0x40);
        frame.vertexTopRight->x = (float)g_GameOverViewportX + g_GameOverViewportWidth;
        frame.vertexTopRight->y = (float)g_GameOverViewportY;
        frame.vertexTopRight->zBits = frame.vertexZ;
        frame.vertexTopRight->rhwBits = FLOAT_ONE_BITS;
        frame.vertexTopRight->color = frame.fadeColor;
        frame.vertexTopRight->specular = 0xFF000000;
        frame.vertexBottomRight = (GfxTlVertex *)(g_GameOverFadeRenderObject->vertexCursor + 0x60);
        frame.vertexBottomRight->x = (float)g_GameOverViewportX + g_GameOverViewportWidth;
        frame.vertexBottomRight->y = (float)g_GameOverViewportY + g_GameOverViewportHeight;
        frame.vertexBottomRight->zBits = frame.vertexZ;
        frame.vertexBottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.vertexBottomRight->color = frame.fadeColor;
        frame.vertexBottomRight->specular = 0xFF000000;
    }
}

/*
 * Emits the low-resolution (mode 0) game-over screen quads into two batch nodes.
 */
// FUNCTION: FF7 0x00404f83
void __cdecl GameOver_BuildScreenQuadsMode0(void)
{
    float uCoord;
    float textureV;
    GfxTlVertex *pFirstTopLeft;
    float vHeight;
    GfxTlVertex *firstTRPtr;
    GfxTlVertex *firstBottomLeftPtr;
    GfxTlVertex *secondV0Ptr;
    GfxTlVertex *firstBR;
    GfxTlVertex *secondTR;
    GfxTlVertex *secondBottomRemaining;
    GfxTlVertex *secondBottomRight;
    int pz;
    float texWidth;

    uCoord = 0.0f;
    textureV = 0.0f;
    texWidth = 1.0f;
    vHeight = 0.84375f;
    pz = FLOAT_0_01_BITS;
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode01[0]) != 0) {
        pFirstTopLeft = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[0]->vertexCursor);
        pFirstTopLeft->xBits = FLOAT_16_BITS;
        pFirstTopLeft->yBits = FLOAT_12_BITS;
        pFirstTopLeft->zBits = pz;
        pFirstTopLeft->rhwBits = FLOAT_ONE_BITS;
        pFirstTopLeft->color = 0x80FFFFFF;
        pFirstTopLeft->specular = 0xFF000000;
        pFirstTopLeft->u = uCoord;
        pFirstTopLeft->v = textureV;
        firstBottomLeftPtr = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[0]->vertexCursor + 0x20);
        firstBottomLeftPtr->xBits = FLOAT_16_BITS;
        firstBottomLeftPtr->yBits = FLOAT_228_BITS;
        firstBottomLeftPtr->zBits = pz;
        firstBottomLeftPtr->rhwBits = FLOAT_ONE_BITS;
        firstBottomLeftPtr->color = 0x80FFFFFF;
        firstBottomLeftPtr->specular = 0xFF000000;
        firstBottomLeftPtr->u = uCoord;
        firstBottomLeftPtr->v = textureV + vHeight;
        firstTRPtr = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[0]->vertexCursor + 0x40);
        firstTRPtr->xBits = FLOAT_272_BITS;
        firstTRPtr->yBits = FLOAT_12_BITS;
        firstTRPtr->zBits = pz;
        firstTRPtr->rhwBits = FLOAT_ONE_BITS;
        firstTRPtr->color = 0x80FFFFFF;
        firstTRPtr->specular = 0xFF000000;
        firstTRPtr->u = uCoord + texWidth;
        firstTRPtr->v = textureV;
        firstBR = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[0]->vertexCursor + 0x60);
        firstBR->xBits = FLOAT_272_BITS;
        firstBR->yBits = FLOAT_228_BITS;
        firstBR->zBits = pz;
        firstBR->rhwBits = FLOAT_ONE_BITS;
        firstBR->color = 0x80FFFFFF;
        firstBR->specular = 0xFF000000;
        firstBR->u = uCoord + texWidth;
        firstBR->v = textureV + vHeight;
    }
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode01[1]) != 0) {
        secondV0Ptr = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[1]->vertexCursor);
        secondV0Ptr->xBits = FLOAT_272_BITS;
        secondV0Ptr->yBits = FLOAT_12_BITS;
        secondV0Ptr->zBits = pz;
        secondV0Ptr->rhwBits = FLOAT_ONE_BITS;
        secondV0Ptr->color = 0x80FFFFFF;
        secondV0Ptr->specular = 0xFF000000;
        secondV0Ptr->u = uCoord;
        secondV0Ptr->v = textureV;
        secondBottomRemaining = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[1]->vertexCursor + 0x20);
        secondBottomRemaining->xBits = FLOAT_272_BITS;
        secondBottomRemaining->yBits = FLOAT_228_BITS;
        secondBottomRemaining->zBits = pz;
        secondBottomRemaining->rhwBits = FLOAT_ONE_BITS;
        secondBottomRemaining->color = 0x80FFFFFF;
        secondBottomRemaining->specular = 0xFF000000;
        secondBottomRemaining->u = uCoord;
        secondBottomRemaining->v = textureV + vHeight;
        secondTR = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[1]->vertexCursor + 0x40);
        secondTR->xBits = FLOAT_304_BITS;
        secondTR->yBits = FLOAT_12_BITS;
        secondTR->zBits = pz;
        secondTR->rhwBits = FLOAT_ONE_BITS;
        secondTR->color = 0x80FFFFFF;
        secondTR->specular = 0xFF000000;
        secondTR->u = uCoord + texWidth;
        secondTR->v = textureV;
        secondBottomRight = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[1]->vertexCursor + 0x60);
        secondBottomRight->xBits = FLOAT_304_BITS;
        secondBottomRight->yBits = FLOAT_228_BITS;
        secondBottomRight->zBits = pz;
        secondBottomRight->rhwBits = FLOAT_ONE_BITS;
        secondBottomRight->color = 0x80FFFFFF;
        secondBottomRight->specular = 0xFF000000;
        secondBottomRight->u = uCoord + texWidth;
        secondBottomRight->v = textureV + vHeight;
    }
}

/*
 * Emits the centered low-resolution (mode 1) game-over screen quads into two batch nodes.
 */
// FUNCTION: FF7 0x004052c8
void __cdecl GameOver_BuildScreenQuadsMode1(void)
{
    float uCoord;
    float textureV;
    GfxTlVertex *pFirstTopLeft;
    float vHeight;
    GfxTlVertex *firstTRPtr;
    GfxTlVertex *firstBottomLeftPtr;
    GfxTlVertex *secondV0Ptr;
    GfxTlVertex *firstBR;
    GfxTlVertex *secondTR;
    GfxTlVertex *secondBottomRemaining;
    GfxTlVertex *secondBottomRight;
    int pz;
    float texWidth;

    uCoord = 0.0f;
    textureV = 0.0f;
    texWidth = 1.0f;
    vHeight = 0.84375f;
    pz = FLOAT_0_01_BITS;
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode01[0]) != 0) {
        pFirstTopLeft = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[0]->vertexCursor);
        pFirstTopLeft->xBits = FLOAT_176_BITS;
        pFirstTopLeft->yBits = FLOAT_132_BITS;
        pFirstTopLeft->zBits = pz;
        pFirstTopLeft->rhwBits = FLOAT_ONE_BITS;
        pFirstTopLeft->color = 0x80FFFFFF;
        pFirstTopLeft->specular = 0xFF000000;
        pFirstTopLeft->u = uCoord;
        pFirstTopLeft->v = textureV;
        firstBottomLeftPtr = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[0]->vertexCursor + 0x20);
        firstBottomLeftPtr->xBits = FLOAT_176_BITS;
        firstBottomLeftPtr->yBits = FLOAT_348_BITS;
        firstBottomLeftPtr->zBits = pz;
        firstBottomLeftPtr->rhwBits = FLOAT_ONE_BITS;
        firstBottomLeftPtr->color = 0x80FFFFFF;
        firstBottomLeftPtr->specular = 0xFF000000;
        firstBottomLeftPtr->u = uCoord;
        firstBottomLeftPtr->v = textureV + vHeight;
        firstTRPtr = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[0]->vertexCursor + 0x40);
        firstTRPtr->xBits = FLOAT_432_BITS;
        firstTRPtr->yBits = FLOAT_132_BITS;
        firstTRPtr->zBits = pz;
        firstTRPtr->rhwBits = FLOAT_ONE_BITS;
        firstTRPtr->color = 0x80FFFFFF;
        firstTRPtr->specular = 0xFF000000;
        firstTRPtr->u = uCoord + texWidth;
        firstTRPtr->v = textureV;
        firstBR = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[0]->vertexCursor + 0x60);
        firstBR->xBits = FLOAT_432_BITS;
        firstBR->yBits = FLOAT_348_BITS;
        firstBR->zBits = pz;
        firstBR->rhwBits = FLOAT_ONE_BITS;
        firstBR->color = 0x80FFFFFF;
        firstBR->specular = 0xFF000000;
        firstBR->u = uCoord + texWidth;
        firstBR->v = textureV + vHeight;
    }
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode01[1]) != 0) {
        secondV0Ptr = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[1]->vertexCursor);
        secondV0Ptr->xBits = FLOAT_432_BITS;
        secondV0Ptr->yBits = FLOAT_132_BITS;
        secondV0Ptr->zBits = pz;
        secondV0Ptr->rhwBits = FLOAT_ONE_BITS;
        secondV0Ptr->color = 0x80FFFFFF;
        secondV0Ptr->specular = 0xFF000000;
        secondV0Ptr->u = uCoord;
        secondV0Ptr->v = textureV;
        secondBottomRemaining = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[1]->vertexCursor + 0x20);
        secondBottomRemaining->xBits = FLOAT_432_BITS;
        secondBottomRemaining->yBits = FLOAT_348_BITS;
        secondBottomRemaining->zBits = pz;
        secondBottomRemaining->rhwBits = FLOAT_ONE_BITS;
        secondBottomRemaining->color = 0x80FFFFFF;
        secondBottomRemaining->specular = 0xFF000000;
        secondBottomRemaining->u = uCoord;
        secondBottomRemaining->v = textureV + vHeight;
        secondTR = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[1]->vertexCursor + 0x40);
        secondTR->xBits = FLOAT_464_BITS;
        secondTR->yBits = FLOAT_132_BITS;
        secondTR->zBits = pz;
        secondTR->rhwBits = FLOAT_ONE_BITS;
        secondTR->color = 0x80FFFFFF;
        secondTR->specular = 0xFF000000;
        secondTR->u = uCoord + texWidth;
        secondTR->v = textureV;
        secondBottomRight = (GfxTlVertex *)(g_GameOverRenderObjectsMode01[1]->vertexCursor + 0x60);
        secondBottomRight->xBits = FLOAT_464_BITS;
        secondBottomRight->yBits = FLOAT_348_BITS;
        secondBottomRight->zBits = pz;
        secondBottomRight->rhwBits = FLOAT_ONE_BITS;
        secondBottomRight->color = 0x80FFFFFF;
        secondBottomRight->specular = 0xFF000000;
        secondBottomRight->u = uCoord + texWidth;
        secondBottomRight->v = textureV + vHeight;
    }
}

/*
 * Emits the high-resolution (mode 2) game-over screen quads across a 3x2 grid of batch nodes.
 */
// FUNCTION: FF7 0x0040560d
void __cdecl GameOver_BuildScreenQuadsMode2(void)
{
    int xStart;
    float inUStart;
    float uWidth;
    GfxTlVertex *batch0Vertex1;
    GfxTlVertex *batch3Vert3;
    GfxTlVertex *batch3SecondVertPtr;
    GfxTlVertex *pBatch3V1;
    GfxTlVertex *batch3V0;
    GfxTlVertex *batch2Corner3Ptr;
    GfxTlVertex *pBatch4Vert0;
    GfxTlVertex *batch5Vert1;
    int depth;
    float textureV;
    GfxTlVertex *batch0First;
    GfxTlVertex *pBatch2V2;
    GfxTlVertex *batch2V1;
    GfxTlVertex *pBatch2First;
    GfxTlVertex *batch1Vert3Ptr;
    GfxTlVertex *pBatch1V2;
    GfxTlVertex *batch5Vert0Ptr;
    GfxTlVertex *batch4Vert3Ptr;
    GfxTlVertex *pBatch5Vert2;
    GfxTlVertex *pBatch5Vert3;
    int yOrigin;
    GfxTlVertex *batch1Vertex1;
    GfxTlVertex *pBatch1First;
    float vHeight;
    GfxTlVertex *batch0V3Ptr;
    GfxTlVertex *batch4Vert1;
    GfxTlVertex *batch4Vert2;
    GfxTlVertex *pBatch0V2;

    yOrigin = 24;
    xStart = 32;
    inUStart = 0.0f;
    textureV = 0.0f;
    uWidth = 1.0f;
    vHeight = 1.0f;
    depth = FLOAT_0_01_BITS;
    if (AcquireBatchNode(1, (g_GameOverRenderObjectsMode2[0])) != 0) {
        batch0First = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[0]->vertexCursor);
        batch0First->x = (float)xStart;
        batch0First->y = (float)yOrigin;
        batch0First->zBits = depth;
        batch0First->rhwBits = FLOAT_ONE_BITS;
        batch0First->color = 0x80FFFFFF;
        batch0First->specular = 0xFF000000;
        batch0First->u = inUStart;
        batch0First->v = textureV;
        batch0Vertex1 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[0]->vertexCursor + 0x20);
        batch0Vertex1->x = (float)xStart;
        batch0Vertex1->y = (float)yOrigin + 256.0f;
        batch0Vertex1->zBits = depth;
        batch0Vertex1->rhwBits = FLOAT_ONE_BITS;
        batch0Vertex1->color = 0x80FFFFFF;
        batch0Vertex1->specular = 0xFF000000;
        batch0Vertex1->u = inUStart;
        batch0Vertex1->v = textureV + vHeight;
        pBatch0V2 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[0]->vertexCursor + 0x40);
        pBatch0V2->x = (float)xStart + 256.0f;
        pBatch0V2->y = (float)yOrigin;
        pBatch0V2->zBits = depth;
        pBatch0V2->rhwBits = FLOAT_ONE_BITS;
        pBatch0V2->color = 0x80FFFFFF;
        pBatch0V2->specular = 0xFF000000;
        pBatch0V2->u = inUStart + uWidth;
        pBatch0V2->v = textureV;
        batch0V3Ptr = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[0]->vertexCursor + 0x60);
        batch0V3Ptr->x = (float)xStart + 256.0f;
        batch0V3Ptr->y = (float)yOrigin + 256.0f;
        batch0V3Ptr->zBits = depth;
        batch0V3Ptr->rhwBits = FLOAT_ONE_BITS;
        batch0V3Ptr->color = 0x80FFFFFF;
        batch0V3Ptr->specular = 0xFF000000;
        batch0V3Ptr->u = inUStart + uWidth;
        batch0V3Ptr->v = textureV + vHeight;
    }
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode2[1]) != 0) {
        pBatch1First = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[1]->vertexCursor);
        pBatch1First->x = (float)xStart + 256.0f;
        pBatch1First->y = (float)yOrigin;
        pBatch1First->zBits = depth;
        pBatch1First->rhwBits = FLOAT_ONE_BITS;
        pBatch1First->color = 0x80FFFFFF;
        pBatch1First->specular = 0xFF000000;
        pBatch1First->u = inUStart;
        pBatch1First->v = textureV;
        batch1Vertex1 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[1]->vertexCursor + 0x20);
        batch1Vertex1->x = (float)xStart + 256.0f;
        batch1Vertex1->y = (float)yOrigin + 256.0f;
        batch1Vertex1->zBits = depth;
        batch1Vertex1->rhwBits = FLOAT_ONE_BITS;
        batch1Vertex1->color = 0x80FFFFFF;
        batch1Vertex1->specular = 0xFF000000;
        batch1Vertex1->u = inUStart;
        batch1Vertex1->v = textureV + vHeight;
        pBatch1V2 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[1]->vertexCursor + 0x40);
        pBatch1V2->x = (float)xStart + 256.0f + 256.0f;
        pBatch1V2->y = (float)yOrigin;
        pBatch1V2->zBits = depth;
        pBatch1V2->rhwBits = FLOAT_ONE_BITS;
        pBatch1V2->color = 0x80FFFFFF;
        pBatch1V2->specular = 0xFF000000;
        pBatch1V2->u = inUStart + uWidth;
        pBatch1V2->v = textureV;
        batch1Vert3Ptr = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[1]->vertexCursor + 0x60);
        batch1Vert3Ptr->x = (float)xStart + 256.0f + 256.0f;
        batch1Vert3Ptr->y = (float)yOrigin + 256.0f;
        batch1Vert3Ptr->zBits = depth;
        batch1Vert3Ptr->rhwBits = FLOAT_ONE_BITS;
        batch1Vert3Ptr->color = 0x80FFFFFF;
        batch1Vert3Ptr->specular = 0xFF000000;
        batch1Vert3Ptr->u = inUStart + uWidth;
        batch1Vert3Ptr->v = textureV + vHeight;
    }
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode2[2]) != 0) {
        pBatch2First = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[2]->vertexCursor);
        pBatch2First->x = (float)xStart + 256.0f + 256.0f;
        pBatch2First->y = (float)yOrigin;
        pBatch2First->zBits = depth;
        pBatch2First->rhwBits = FLOAT_ONE_BITS;
        pBatch2First->color = 0x80FFFFFF;
        pBatch2First->specular = 0xFF000000;
        pBatch2First->u = inUStart;
        pBatch2First->v = textureV;
        batch2V1 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[2]->vertexCursor + 0x20);
        batch2V1->x = (float)xStart + 256.0f + 256.0f;
        batch2V1->y = (float)yOrigin + 256.0f;
        batch2V1->zBits = depth;
        batch2V1->rhwBits = FLOAT_ONE_BITS;
        batch2V1->color = 0x80FFFFFF;
        batch2V1->specular = 0xFF000000;
        batch2V1->u = inUStart;
        batch2V1->v = textureV + vHeight;
        pBatch2V2 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[2]->vertexCursor + 0x40);
        pBatch2V2->x = (float)xStart + 256.0f + 256.0f + 64.0f;
        pBatch2V2->y = (float)yOrigin;
        pBatch2V2->zBits = depth;
        pBatch2V2->rhwBits = FLOAT_ONE_BITS;
        pBatch2V2->color = 0x80FFFFFF;
        pBatch2V2->specular = 0xFF000000;
        pBatch2V2->u = inUStart + uWidth;
        pBatch2V2->v = textureV;
        batch2Corner3Ptr = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[2]->vertexCursor + 0x60);
        batch2Corner3Ptr->x = (float)xStart + 256.0f + 256.0f + 64.0f;
        batch2Corner3Ptr->y = (float)yOrigin + 256.0f;
        batch2Corner3Ptr->zBits = depth;
        batch2Corner3Ptr->rhwBits = FLOAT_ONE_BITS;
        batch2Corner3Ptr->color = 0x80FFFFFF;
        batch2Corner3Ptr->specular = 0xFF000000;
        batch2Corner3Ptr->u = inUStart + uWidth;
        batch2Corner3Ptr->v = textureV + vHeight;
    }
    vHeight = 0.6875f;
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode2[3]) != 0) {
        batch3V0 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[3]->vertexCursor);
        batch3V0->x = (float)xStart;
        batch3V0->y = (float)yOrigin + 256.0f;
        batch3V0->zBits = depth;
        batch3V0->rhwBits = FLOAT_ONE_BITS;
        batch3V0->color = 0x80FFFFFF;
        batch3V0->specular = 0xFF000000;
        batch3V0->u = inUStart;
        batch3V0->v = textureV;
        pBatch3V1 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[3]->vertexCursor + 0x20);
        pBatch3V1->x = (float)xStart;
        pBatch3V1->y = (float)yOrigin + 256.0f + 176.0f;
        pBatch3V1->zBits = depth;
        pBatch3V1->rhwBits = FLOAT_ONE_BITS;
        pBatch3V1->color = 0x80FFFFFF;
        pBatch3V1->specular = 0xFF000000;
        pBatch3V1->u = inUStart;
        pBatch3V1->v = textureV + vHeight;
        batch3SecondVertPtr = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[3]->vertexCursor + 0x40);
        batch3SecondVertPtr->x = (float)xStart + 256.0f;
        batch3SecondVertPtr->y = (float)yOrigin + 256.0f;
        batch3SecondVertPtr->zBits = depth;
        batch3SecondVertPtr->rhwBits = FLOAT_ONE_BITS;
        batch3SecondVertPtr->color = 0x80FFFFFF;
        batch3SecondVertPtr->specular = 0xFF000000;
        batch3SecondVertPtr->u = inUStart + uWidth;
        batch3SecondVertPtr->v = textureV;
        batch3Vert3 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[3]->vertexCursor + 0x60);
        batch3Vert3->x = (float)xStart + 256.0f;
        batch3Vert3->y = (float)yOrigin + 256.0f + 176.0f;
        batch3Vert3->zBits = depth;
        batch3Vert3->rhwBits = FLOAT_ONE_BITS;
        batch3Vert3->color = 0x80FFFFFF;
        batch3Vert3->specular = 0xFF000000;
        batch3Vert3->u = inUStart + uWidth;
        batch3Vert3->v = textureV + vHeight;
    }
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode2[4]) != 0) {
        pBatch4Vert0 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[4]->vertexCursor);
        pBatch4Vert0->x = (float)xStart + 256.0f;
        pBatch4Vert0->y = (float)yOrigin + 256.0f;
        pBatch4Vert0->zBits = depth;
        pBatch4Vert0->rhwBits = FLOAT_ONE_BITS;
        pBatch4Vert0->color = 0x80FFFFFF;
        pBatch4Vert0->specular = 0xFF000000;
        pBatch4Vert0->u = inUStart;
        pBatch4Vert0->v = textureV;
        batch4Vert1 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[4]->vertexCursor + 0x20);
        batch4Vert1->x = (float)xStart + 256.0f;
        batch4Vert1->y = (float)yOrigin + 256.0f + 176.0f;
        batch4Vert1->zBits = depth;
        batch4Vert1->rhwBits = FLOAT_ONE_BITS;
        batch4Vert1->color = 0x80FFFFFF;
        batch4Vert1->specular = 0xFF000000;
        batch4Vert1->u = inUStart;
        batch4Vert1->v = textureV + vHeight;
        batch4Vert2 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[4]->vertexCursor + 0x40);
        batch4Vert2->x = (float)xStart + 256.0f + 256.0f;
        batch4Vert2->y = (float)yOrigin + 256.0f;
        batch4Vert2->zBits = depth;
        batch4Vert2->rhwBits = FLOAT_ONE_BITS;
        batch4Vert2->color = 0x80FFFFFF;
        batch4Vert2->specular = 0xFF000000;
        batch4Vert2->u = inUStart + uWidth;
        batch4Vert2->v = textureV;
        batch4Vert3Ptr = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[4]->vertexCursor + 0x60);
        batch4Vert3Ptr->x = (float)xStart + 256.0f + 256.0f;
        batch4Vert3Ptr->y = (float)yOrigin + 256.0f + 176.0f;
        batch4Vert3Ptr->zBits = depth;
        batch4Vert3Ptr->rhwBits = FLOAT_ONE_BITS;
        batch4Vert3Ptr->color = 0x80FFFFFF;
        batch4Vert3Ptr->specular = 0xFF000000;
        batch4Vert3Ptr->u = inUStart + uWidth;
        batch4Vert3Ptr->v = textureV + vHeight;
    }
    if (AcquireBatchNode(1, g_GameOverRenderObjectsMode2[5]) != 0) {
        batch5Vert0Ptr = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[5]->vertexCursor);
        batch5Vert0Ptr->x = (float)xStart + 256.0f + 256.0f;
        batch5Vert0Ptr->y = (float)yOrigin + 256.0f;
        batch5Vert0Ptr->zBits = depth;
        batch5Vert0Ptr->rhwBits = FLOAT_ONE_BITS;
        batch5Vert0Ptr->color = 0x80FFFFFF;
        batch5Vert0Ptr->specular = 0xFF000000;
        batch5Vert0Ptr->u = inUStart;
        batch5Vert0Ptr->v = textureV;
        batch5Vert1 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[5]->vertexCursor + 0x20);
        batch5Vert1->x = (float)xStart + 256.0f + 256.0f;
        batch5Vert1->y = (float)yOrigin + 256.0f + 176.0f;
        batch5Vert1->zBits = depth;
        batch5Vert1->rhwBits = FLOAT_ONE_BITS;
        batch5Vert1->color = 0x80FFFFFF;
        batch5Vert1->specular = 0xFF000000;
        batch5Vert1->u = inUStart;
        batch5Vert1->v = textureV + vHeight;
        pBatch5Vert2 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[5]->vertexCursor + 0x40);
        pBatch5Vert2->x = (float)xStart + 256.0f + 256.0f + 64.0f;
        pBatch5Vert2->y = (float)yOrigin + 256.0f;
        pBatch5Vert2->zBits = depth;
        pBatch5Vert2->rhwBits = FLOAT_ONE_BITS;
        pBatch5Vert2->color = 0x80FFFFFF;
        pBatch5Vert2->specular = 0xFF000000;
        pBatch5Vert2->u = inUStart + uWidth;
        pBatch5Vert2->v = textureV;
        pBatch5Vert3 = (GfxTlVertex *)(g_GameOverRenderObjectsMode2[5]->vertexCursor + 0x60);
        pBatch5Vert3->x = (float)xStart + 256.0f + 256.0f + 64.0f;
        pBatch5Vert3->y = (float)yOrigin + 256.0f + 176.0f;
        pBatch5Vert3->zBits = depth;
        pBatch5Vert3->rhwBits = FLOAT_ONE_BITS;
        pBatch5Vert3->color = 0x80FFFFFF;
        pBatch5Vert3->specular = 0xFF000000;
        pBatch5Vert3->u = inUStart + uWidth;
        pBatch5Vert3->v = textureV + vHeight;
    }
}

/*
 * Opens the disc_us.lgp archive and loads the game-over TIM textures for the current display mode.
 */
// FUNCTION: FF7 0x004060d1
void __cdecl GameOver_LoadDiscEOverTims(void)
{
    struct {
        int mode;
        char *file;
        PfileLoadOptions opt;
        unsigned int i;
        struct GfxEngineContext *ctx;
        char msg[256];
        char path[256];
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.ctx = GetEngineContext();
    f.mode = GetUiScalingMode();
    strcpy(f.path, GetDataPath());
    // STRING: FF7 0x007ba710
    strcat(f.path, "cd/disc_us.lgp");
    if (!OpenLgpArchive(f.path, LGP_ARCHIVE_DISC)) {
        // STRING: FF7 0x007ba720
        sprintf(f.msg, "Failed to load: %s\n", f.path);
        Gfx_DebugPrint(f.msg);
    }
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &f.opt);
    f.opt.file.fromArchive = 1;
    f.opt.file.archiveId = 16;
    f.opt.file.callback = 0;
    f.opt.directory = g_GameOverEmptyPath;
    if (f.mode == 2) {
        f.opt.extraFlags = f.opt.extraFlags | 1;
        f.opt.colorKey = 0xff141414;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &f.opt);
        for (f.i = 0; f.i < 6; f.i++) {
            f.file = g_GameOverTimNamesMode2[f.i];
            g_GameOverRenderObjectsMode2[f.i] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        }
    } else {
        f.opt.extraFlags = f.opt.extraFlags | 1;
        f.opt.colorKey = 0xff141414;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &f.opt);
        for (f.i = 0; f.i < 2; f.i++) {
            f.file = g_GameOverTimNamesMode01[f.i];
            g_GameOverRenderObjectsMode01[f.i] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        }
    }
    f.opt.renderFlags = f.opt.renderFlags | 0x4000;
    Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &f.opt);
    g_GameOverFadeRenderObject = (GfxRenderObject *)Gfx_CreateRenderObject(0, GFX_DRAW_FLAT_QUAD_2D, &f.opt, 0, (void *)f.ctx->renderObjectBank);
}

/*
 * Enters the game-over screen (GAME_MODE_GAME_OVER). It arms the fade-in from black (alpha 255,
 * which GameOver_ModuleFrame lowers by 51 a frame) and the fade-out, clears the exit trigger and
 * the fade-out alpha, and sets the viewport by GetUiScalingMode: mode 0 is 320x240 at the origin,
 * mode 1 is 320x240 at (160,120), any other mode 640x480 at the origin. Then it opens
 * cd/disc_us.lgp as archive 16 and creates the picture's render objects from it
 * (GameOver_LoadDiscEOverTims): the six tiles e_over_a.tim .. e_over_f.tim for mode 2, the
 * two halves e_over_la.tim and e_over_lb.tim for the others, and the fade quad.
 */
// FUNCTION: FF7 0x00406367
void __cdecl GameOver_ModuleEnter(void)
{
    int mode;

    mode = GetUiScalingMode();
    g_GameOverFadeInActive = 1;
    g_GameOverFadeOutActive = 1;
    g_GameOverExitTriggered = 0;
    g_GameOverFadeOutAlpha = 0;
    g_GameOverIntroFadeAlpha = 0xFF;
    if (mode == 0) {
        g_GameOverViewportX = 0;
        g_GameOverViewportY = 0;
        g_GameOverViewportWidth = 320;
        g_GameOverViewportHeight = 240;
    } else if (mode == 1) {
        g_GameOverViewportX = 160;
        g_GameOverViewportY = 120;
        g_GameOverViewportWidth = 320;
        g_GameOverViewportHeight = 240;
    } else {
        g_GameOverViewportX = 0;
        g_GameOverViewportY = 0;
        g_GameOverViewportWidth = 640;
        g_GameOverViewportHeight = 480;
    }
    GameOver_LoadDiscEOverTims();
}

/*
 * Cleans up game-over render objects, closes archives, and transitions to the next game mode.
 */
// FUNCTION: FF7 0x00406436
void __cdecl GameOver_ModuleExit(void)
{
    int i;

    SoundDispatchCommand(SOUND_MUSIC_VOLUME_TRANS, 0x3C, 0, 0, 0, 0, 0, 0, 0);
    Menu_ReleaseAllRenderObjects();
    for (i = 0; i < 6; i++) {
        DestroyRenderObjectAndClear(i * 4 + (char *)g_GameOverRenderObjectsMode2);
    }
    DestroyRenderObjectAndClear(&g_GameOverRenderObjectsMode01[0]);
    DestroyRenderObjectAndClear(&g_GameOverRenderObjectsMode01[1]);
    DestroyRenderObjectAndClear(&g_GameOverFadeRenderObject);
    g_FieldActorState.state.requestCode = FIELD_REQUEST_NONE;
    g_EngineGameMode = GAME_MODE_CREDITS;
    g_EnginePreviousMode = GAME_MODE_GAME_OVER;
    CloseLgpArchive(LGP_ARCHIVE_DISC);
}

/*
 * Renders one frame of the game-over screen, handling input and fade transitions.
 */
// FUNCTION: FF7 0x004064d7
void __cdecl GameOver_ModuleFrame(struct GfxEngineContext *ctx)
{
    GameModule nextModule;
    int beginArg;
    int uiScale;
    GfxClearColorBgra bgColor;
    uiScale = GetUiScalingMode();
    ResetFrameDrawPool(ctx);
    ResetRenderPools(ctx);
    bgColor.r = 0;
    bgColor.g = 0;
    bgColor.b = 0;
    bgColor.a = FLOAT_ONE_BITS;
    GfxSetBackgroundColorCall(&bgColor, ctx);
    GfxClearAllCall(ctx);
    beginArg = 0;
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    if (((int (__cdecl *)(int, struct GfxEngineContext *))Gfx_BeginSceneCall)(beginArg, ctx)) { /* reads eax after a void call, as the original does */
        if (uiScale == 2) {
            GameOver_BuildScreenQuadsMode2();
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode2[0], ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode2[1], ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode2[2], ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode2[3], ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode2[4], ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode2[5], ctx);
        } else if (uiScale == 0) {
            GameOver_BuildScreenQuadsMode0();
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode01[0], ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode01[1], ctx);
        } else if (uiScale == 1) {
            GameOver_BuildScreenQuadsMode1();
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode01[0], ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverRenderObjectsMode01[1], ctx);
        }
        Input_Update(0);
        if (Input_GetPressed(INPUT_ANY)) {
            g_GameOverExitTriggered = 1;
        }
        if (g_GameOverExitTriggered != 0 && g_GameOverFadeOutActive != 0) {
            GameOver_FillFadeQuad((unsigned char)g_GameOverFadeOutAlpha);
            Gfx_SetScenePassCall(1, ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverFadeRenderObject, ctx);
            g_GameOverFadeOutAlpha += 51;
            if (g_GameOverFadeOutAlpha > 255) {
                g_GameOverFadeOutActive = 0;
                nextModule.enter = GameModeDispatcher_Enter;
                nextModule.frame = GameModeDispatcher_Frame;
                nextModule.onMouse = App_OnMouseMessage;
                nextModule.onKey = App_OnKeyMessage;
                nextModule.exit = GameModeDispatcher_Exit;
                RequestModuleSwitch(&nextModule, ctx);
            }
        }
        if (g_GameOverFadeInActive != 0) {
            GameOver_FillFadeQuad((unsigned char)g_GameOverIntroFadeAlpha);
            Gfx_SetScenePassCall(1, ctx);
            Gfx_EmitPrimitiveBatchList(g_GameOverFadeRenderObject, ctx);
            g_GameOverIntroFadeAlpha -= 51;
            if (g_GameOverIntroFadeAlpha <= 51) {
                g_GameOverFadeInActive = 0;
            }
        }
        Gfx_EndSceneCall(ctx);
    }
}

/*
 * Handles keyboard input for the game-over module (discards WM_CHAR messages).
 */
// FUNCTION: FF7 0x00406770
void __cdecl GameOver_ModuleKeyboard(int message, int wParam)
{
    unsigned char charCopy;
    int msgCopy;
    unsigned char characterCode;

    msgCopy = message;
    if (msgCopy != 0x102) {
        return;
    }
    characterCode = (unsigned char)wParam;
    charCopy = characterCode;
}

/*
 * Mouse input handler for the game-over module (no-op).
 */
// FUNCTION: FF7 0x00406797
void __cdecl GameOver_ModuleMouse(void)
{
    // Intentionally empty: the module has no mouse handling.
}
} /* extern "C" */

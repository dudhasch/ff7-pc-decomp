// src/ff7/src/main/main_cd_check_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs the CD-check screen: it builds the fade overlay and screen quads at three
//   resolutions, loads the disc graphics from the disc archive, and draws each frame. A
//   device-change handler prompts for the requested disc and starts the disc-check program.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/types/main.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <dbt.h>
#include "lib/pfile.h"
#include "ff7/api/field.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/polygon.h"
#include "ff7/savemap.h"
#include "lib/enums/file.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x007ba278
int g_CdCheckIntroFadeAlpha = 255;
// GLOBAL: FF7 0x009a04f8
int g_CdCheckDrivePrefix = 0;
// GLOBAL: FF7 0x009a04fc
int g_CdCheckActiveFlag = 0;
// GLOBAL: FF7 0x009a0500
int g_CdCheckViewportHeight = 0;
// GLOBAL: FF7 0x009a0504
int g_CdCheckViewportX = 0;
// GLOBAL: FF7 0x009a0508
int g_CdCheckViewportWidth = 0;
unsigned char g_bssPad_009a050c[4] = {0};
// GLOBAL: FF7 0x009a0510
struct GfxRenderObject *g_CdCheckHighNodes[6] = {0};
// GLOBAL: FF7 0x009a0528
struct GfxRenderObject *g_CdCheckLowCharBatch = 0;
// GLOBAL: FF7 0x009a052c
struct GfxRenderObject *g_CdCheckLowDiscBatch = 0;
// GLOBAL: FF7 0x009a0530
struct GfxRenderObject *g_CdCheckFadeBatch = 0;
// GLOBAL: FF7 0x009a0534
int g_CdCheckFadeOutActive = 0;
// GLOBAL: FF7 0x009a0538
int g_CdCheckDetectedDisc = 0;
// GLOBAL: FF7 0x009a053c
int g_CdCheckViewportY = 0;
// GLOBAL: FF7 0x009a0540
int g_CdCheckFadeInActive = 0;
// GLOBAL: FF7 0x009a0544
int g_CdCheckFadeOutAlpha = 0;
// GLOBAL: FF7 0x009a0548
char g_CdCheckEmptyPath[1] = {0};
// GLOBAL: FF7 0x009a054c
int g_CdCheckDisplayMode = 0;
// clang-format on

#pragma intrinsic(strcmp, memcmp, memset, memcpy, strcpy, strcat)

#pragma data_seg(".u401tim")
#pragma data_seg()

// Bounded archive filename tables and empty lookup prefixes.
// GLOBAL: FF7 0x007ba280
char *g_cdCheckDiscTimNames[9] = {
    // STRING: FF7 0x007ba35c
    "disk1_a.tim",
    // STRING: FF7 0x007ba368
    "disk1_b.tim",
    // STRING: FF7 0x007ba374
    "disk2_a.tim",
    // STRING: FF7 0x007ba380
    "disk2_b.tim",
    // STRING: FF7 0x007ba38c
    "disk3_a.tim",
    // STRING: FF7 0x007ba398
    "disk3_b.tim",
    // STRING: FF7 0x007ba3a4
    "disk1_x.tim",
    // STRING: FF7 0x007ba3b0
    "disk2_x.tim",
    // STRING: FF7 0x007ba3bc
    "disk3_x.tim",
};

// GLOBAL: FF7 0x007ba2a8
char *g_cdCheckCharQuadTimNames[36] = {
    // STRING: FF7 0x007ba3c8
    "aeris_a.tim",
    // STRING: FF7 0x007ba3d4
    "aeris_b.tim",
    // STRING: FF7 0x007ba3e0
    "aeris_c.tim",
    // STRING: FF7 0x007ba3ec
    "aeris_d.tim",
    // STRING: FF7 0x007ba3f8
    "barr_a.tim",
    // STRING: FF7 0x007ba404
    "barr_b.tim",
    // STRING: FF7 0x007ba410
    "barr_c.tim",
    // STRING: FF7 0x007ba41c
    "barr_d.tim",
    // STRING: FF7 0x007ba428
    "cid_a.tim",
    // STRING: FF7 0x007ba434
    "cid_b.tim",
    // STRING: FF7 0x007ba440
    "cid_c.tim",
    // STRING: FF7 0x007ba44c
    "cid_d.tim",
    // STRING: FF7 0x007ba458
    "cloud_a.tim",
    // STRING: FF7 0x007ba464
    "cloud_b.tim",
    // STRING: FF7 0x007ba470
    "cloud_c.tim",
    // STRING: FF7 0x007ba47c
    "cloud_d.tim",
    // STRING: FF7 0x007ba488
    "kets_a.tim",
    // STRING: FF7 0x007ba494
    "kets_b.tim",
    // STRING: FF7 0x007ba4a0
    "kets_c.tim",
    // STRING: FF7 0x007ba4ac
    "kets_d.tim",
    // STRING: FF7 0x007ba4b8
    "red_a.tim",
    // STRING: FF7 0x007ba4c4
    "red_b.tim",
    // STRING: FF7 0x007ba4d0
    "red_c.tim",
    // STRING: FF7 0x007ba4dc
    "red_d.tim",
    // STRING: FF7 0x007ba4e8
    "tifa_a.tim",
    // STRING: FF7 0x007ba4f4
    "tifa_b.tim",
    // STRING: FF7 0x007ba500
    "tifa_c.tim",
    // STRING: FF7 0x007ba50c
    "tifa_d.tim",
    // STRING: FF7 0x007ba518
    "vinc_a.tim",
    // STRING: FF7 0x007ba524
    "vinc_b.tim",
    // STRING: FF7 0x007ba530
    "vinc_c.tim",
    // STRING: FF7 0x007ba53c
    "vinc_d.tim",
    // STRING: FF7 0x007ba548
    "yuff_a.tim",
    // STRING: FF7 0x007ba554
    "yuff_b.tim",
    // STRING: FF7 0x007ba560
    "yuff_c.tim",
    // STRING: FF7 0x007ba56c
    "yuff_d.tim",
};

// GLOBAL: FF7 0x007ba338
char *g_cdCheckCharTimNames[9] = {
    // STRING: FF7 0x007ba578
    "aeris_x.tim",
    // STRING: FF7 0x007ba584
    "barr_x.tim",
    // STRING: FF7 0x007ba590
    "cid_x.tim",
    // STRING: FF7 0x007ba59c
    "cloud_x.tim",
    // STRING: FF7 0x007ba5a8
    "kets_x.tim",
    // STRING: FF7 0x007ba5b4
    "red_x.tim",
    // STRING: FF7 0x007ba5c0
    "tifa_x.tim",
    // STRING: FF7 0x007ba5cc
    "vinc_x.tim",
    // STRING: FF7 0x007ba5d8
    "yuff_x.tim",
};


// GLOBAL: FF7 0x007b6408
static const char REG_ROOT_KEY[] = "Software\\Square Soft, Inc.\\Final Fantasy VII";

/*
 * Generates a full-screen vertex quad for the CD-check fade overlay.
 */
// FUNCTION: FF7 0x00402ef0
void __cdecl CdCheck_FillFadeQuad(unsigned char fadeLevel)
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
        struct { unsigned char pad_to_17[0x17]; char fadeLevel; };
    } frame;
#pragma pack(pop)

    *(char *)&frame.fadeColor = 0;
    frame.tmpColorA = *(char *)&frame.fadeColor;
    frame.tmpColorB = frame.tmpColorA;
    frame.fadeLevel = fadeLevel;
    frame.vertexZ = 0;
    if (AcquireBatchNode(1, g_CdCheckFadeBatch) != 0) {
        frame.vertexTopLeft = (GfxTlVertex *)(g_CdCheckFadeBatch->vertexCursor);
        frame.vertexTopLeft->x = (float)g_CdCheckViewportX;
        frame.vertexTopLeft->y = (float)g_CdCheckViewportY;
        frame.vertexTopLeft->zBits = frame.vertexZ;
        frame.vertexTopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.vertexTopLeft->color = frame.fadeColor;
        frame.vertexTopLeft->specular = 0xFF000000;
        frame.vertexBottomLeft = (GfxTlVertex *)(g_CdCheckFadeBatch->vertexCursor + 0x20);
        frame.vertexBottomLeft->x = (float)g_CdCheckViewportX;
        frame.vertexBottomLeft->y = (float)g_CdCheckViewportY + g_CdCheckViewportHeight;
        frame.vertexBottomLeft->zBits = frame.vertexZ;
        frame.vertexBottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.vertexBottomLeft->color = frame.fadeColor;
        frame.vertexBottomLeft->specular = 0xFF000000;
        frame.vertexTopRight = (GfxTlVertex *)(g_CdCheckFadeBatch->vertexCursor + 0x40);
        frame.vertexTopRight->x = (float)g_CdCheckViewportX + g_CdCheckViewportWidth;
        frame.vertexTopRight->y = (float)g_CdCheckViewportY;
        frame.vertexTopRight->zBits = frame.vertexZ;
        frame.vertexTopRight->rhwBits = FLOAT_ONE_BITS;
        frame.vertexTopRight->color = frame.fadeColor;
        frame.vertexTopRight->specular = 0xFF000000;
        frame.vertexBottomRight = (GfxTlVertex *)(g_CdCheckFadeBatch->vertexCursor + 0x60);
        frame.vertexBottomRight->x = (float)g_CdCheckViewportX + g_CdCheckViewportWidth;
        frame.vertexBottomRight->y = (float)g_CdCheckViewportY + g_CdCheckViewportHeight;
        frame.vertexBottomRight->zBits = frame.vertexZ;
        frame.vertexBottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.vertexBottomRight->color = frame.fadeColor;
        frame.vertexBottomRight->specular = 0xFF000000;
    }
}

/*
 * Builds low-resolution screen quads (mode 0: 320x240 at origin) for the CD-check screen.
 */
// FUNCTION: FF7 0x00403073
void __cdecl CdCheck_BuildScreenQuadsMode0(void)
{
    int depth;
    float texV;
    float texH;
    GfxTlVertex *characterTopRight;
    GfxTlVertex *charBottomRemaining;
    GfxTlVertex *pCharVert0;
    GfxTlVertex *discBL;
    GfxTlVertex *pDiscTopLeft;
    GfxTlVertex *characterBottomRight;
    GfxTlVertex *dBR;
    GfxTlVertex *calcPDiscVert;
    float texU;
    float texW;

    texU = 0.0f;
    texV = 0.0f;
    texW = 0.78125f;
    texH = 0.5859375f;
    depth = FLOAT_0_01_BITS;
    if (AcquireBatchNode(1, g_CdCheckLowCharBatch) != 0) {
        pCharVert0 = (GfxTlVertex *)(g_CdCheckLowCharBatch->vertexCursor);
        pCharVert0->x = 60.0f;
        pCharVert0->y = 8.0f;
        pCharVert0->zBits = depth;
        pCharVert0->rhw = 1.0f;
        pCharVert0->color = 0x80FFFFFF;
        pCharVert0->specular = 0xFF000000;
        pCharVert0->u = texU;
        pCharVert0->v = texV;
        charBottomRemaining = (GfxTlVertex *)(g_CdCheckLowCharBatch->vertexCursor + 0x20);
        charBottomRemaining->x = 60.0f;
        charBottomRemaining->y = 158.0f;
        charBottomRemaining->zBits = depth;
        charBottomRemaining->rhw = 1.0f;
        charBottomRemaining->color = 0x80FFFFFF;
        charBottomRemaining->specular = 0xFF000000;
        charBottomRemaining->u = texU;
        charBottomRemaining->v = texV + texH;
        characterTopRight = (GfxTlVertex *)(g_CdCheckLowCharBatch->vertexCursor + 0x40);
        characterTopRight->x = 260.0f;
        characterTopRight->y = 8.0f;
        characterTopRight->zBits = depth;
        characterTopRight->rhw = 1.0f;
        characterTopRight->color = 0x80FFFFFF;
        characterTopRight->specular = 0xFF000000;
        characterTopRight->u = texU + texW;
        characterTopRight->v = texV;
        characterBottomRight = (GfxTlVertex *)(g_CdCheckLowCharBatch->vertexCursor + 0x60);
        characterBottomRight->x = 260.0f;
        characterBottomRight->y = 158.0f;
        characterBottomRight->zBits = depth;
        characterBottomRight->rhw = 1.0f;
        characterBottomRight->color = 0x80FFFFFF;
        characterBottomRight->specular = 0xFF000000;
        characterBottomRight->u = texU + texW;
        characterBottomRight->v = texV + texH;
    }
    texH = 0.578125f;
    if (AcquireBatchNode(1, g_CdCheckLowDiscBatch) != 0) {
        pDiscTopLeft = (GfxTlVertex *)(g_CdCheckLowDiscBatch->vertexCursor);
        pDiscTopLeft->x = 60.0f;
        pDiscTopLeft->y = 158.0f;
        pDiscTopLeft->zBits = depth;
        pDiscTopLeft->rhw = 1.0f;
        pDiscTopLeft->color = 0x80FFFFFF;
        pDiscTopLeft->specular = 0xFF000000;
        pDiscTopLeft->u = texU;
        pDiscTopLeft->v = texV;
        discBL = (GfxTlVertex *)(g_CdCheckLowDiscBatch->vertexCursor + 0x20);
        discBL->x = 60.0f;
        discBL->y = 232.0f;
        discBL->zBits = depth;
        discBL->rhw = 1.0f;
        discBL->color = 0x80FFFFFF;
        discBL->specular = 0xFF000000;
        discBL->u = texU;
        discBL->v = texV + texH;
        calcPDiscVert = (GfxTlVertex *)(g_CdCheckLowDiscBatch->vertexCursor + 0x40);
        calcPDiscVert->x = 260.0f;
        calcPDiscVert->y = 158.0f;
        calcPDiscVert->zBits = depth;
        calcPDiscVert->rhw = 1.0f;
        calcPDiscVert->color = 0x80FFFFFF;
        calcPDiscVert->specular = 0xFF000000;
        calcPDiscVert->u = texU + texW;
        calcPDiscVert->v = texV;
        dBR = (GfxTlVertex *)(g_CdCheckLowDiscBatch->vertexCursor + 0x60);
        dBR->x = 260.0f;
        dBR->y = 232.0f;
        dBR->zBits = depth;
        dBR->rhw = 1.0f;
        dBR->color = 0x80FFFFFF;
        dBR->specular = 0xFF000000;
        dBR->u = texU + texW;
        dBR->v = texV + texH;
    }
}

/*
 * Builds centered low-resolution screen quads (mode 1: 320x240 centered) for the CD-check screen.
 */
// FUNCTION: FF7 0x004033bf
void __cdecl CdCheck_BuildScreenQuadsMode1(void)
{
    int depth;
    float texV;
    float texH;
    GfxTlVertex *characterTopRight;
    GfxTlVertex *charBottomRemaining;
    GfxTlVertex *pCharVert0;
    GfxTlVertex *discBL;
    GfxTlVertex *pDiscTopLeft;
    GfxTlVertex *characterBottomRight;
    GfxTlVertex *dBR;
    GfxTlVertex *calcPDiscVert;
    float texU;
    float texW;

    texU = 0.0f;
    texV = 0.0f;
    texW = 0.78125f;
    texH = 0.5859375f;
    depth = FLOAT_0_01_BITS;
    if (AcquireBatchNode(1, g_CdCheckLowCharBatch) != 0) {
        pCharVert0 = (GfxTlVertex *)(g_CdCheckLowCharBatch->vertexCursor);
        pCharVert0->x = 220.0f;
        pCharVert0->y = 128.0f;
        pCharVert0->zBits = depth;
        pCharVert0->rhw = 1.0f;
        pCharVert0->color = 0x80FFFFFF;
        pCharVert0->specular = 0xFF000000;
        pCharVert0->u = texU;
        pCharVert0->v = texV;
        charBottomRemaining = (GfxTlVertex *)(g_CdCheckLowCharBatch->vertexCursor + 0x20);
        charBottomRemaining->x = 220.0f;
        charBottomRemaining->y = 278.0f;
        charBottomRemaining->zBits = depth;
        charBottomRemaining->rhw = 1.0f;
        charBottomRemaining->color = 0x80FFFFFF;
        charBottomRemaining->specular = 0xFF000000;
        charBottomRemaining->u = texU;
        charBottomRemaining->v = texV + texH;
        characterTopRight = (GfxTlVertex *)(g_CdCheckLowCharBatch->vertexCursor + 0x40);
        characterTopRight->x = 420.0f;
        characterTopRight->y = 128.0f;
        characterTopRight->zBits = depth;
        characterTopRight->rhw = 1.0f;
        characterTopRight->color = 0x80FFFFFF;
        characterTopRight->specular = 0xFF000000;
        characterTopRight->u = texU + texW;
        characterTopRight->v = texV;
        characterBottomRight = (GfxTlVertex *)(g_CdCheckLowCharBatch->vertexCursor + 0x60);
        characterBottomRight->x = 420.0f;
        characterBottomRight->y = 278.0f;
        characterBottomRight->zBits = depth;
        characterBottomRight->rhw = 1.0f;
        characterBottomRight->color = 0x80FFFFFF;
        characterBottomRight->specular = 0xFF000000;
        characterBottomRight->u = texU + texW;
        characterBottomRight->v = texV + texH;
    }
    texH = 0.578125f;
    if (AcquireBatchNode(1, g_CdCheckLowDiscBatch) != 0) {
        pDiscTopLeft = (GfxTlVertex *)(g_CdCheckLowDiscBatch->vertexCursor);
        pDiscTopLeft->x = 220.0f;
        pDiscTopLeft->y = 278.0f;
        pDiscTopLeft->zBits = depth;
        pDiscTopLeft->rhw = 1.0f;
        pDiscTopLeft->color = 0x80FFFFFF;
        pDiscTopLeft->specular = 0xFF000000;
        pDiscTopLeft->u = texU;
        pDiscTopLeft->v = texV;
        discBL = (GfxTlVertex *)(g_CdCheckLowDiscBatch->vertexCursor + 0x20);
        discBL->x = 220.0f;
        discBL->y = 352.0f;
        discBL->zBits = depth;
        discBL->rhw = 1.0f;
        discBL->color = 0x80FFFFFF;
        discBL->specular = 0xFF000000;
        discBL->u = texU;
        discBL->v = texV + texH;
        calcPDiscVert = (GfxTlVertex *)(g_CdCheckLowDiscBatch->vertexCursor + 0x40);
        calcPDiscVert->x = 420.0f;
        calcPDiscVert->y = 278.0f;
        calcPDiscVert->zBits = depth;
        calcPDiscVert->rhw = 1.0f;
        calcPDiscVert->color = 0x80FFFFFF;
        calcPDiscVert->specular = 0xFF000000;
        calcPDiscVert->u = texU + texW;
        calcPDiscVert->v = texV;
        dBR = (GfxTlVertex *)(g_CdCheckLowDiscBatch->vertexCursor + 0x60);
        dBR->x = 420.0f;
        dBR->y = 352.0f;
        dBR->zBits = depth;
        dBR->rhw = 1.0f;
        dBR->color = 0x80FFFFFF;
        dBR->specular = 0xFF000000;
        dBR->u = texU + texW;
        dBR->v = texV + texH;
    }
}

/*
 * Builds high-resolution screen quads (mode 2: 640x480) for the CD-check screen.
 */
// FUNCTION: FF7 0x0040370b
void __cdecl CdCheck_BuildScreenQuadsMode2(void)
{
    int yRowBase;
    float vBase;
    GfxTlVertex *vert2;
    GfxTlVertex *vertex1Ptr;
    GfxTlVertex *node1Vertex1;
    GfxTlVertex *vert21;
    GfxTlVertex *screenVertexA1;
    GfxTlVertex *pScreenVertA0;
    GfxTlVertex *node3Vertex3;
    GfxTlVertex *vertexC2Ptr;
    GfxTlVertex *screenVertB0;
    float inUStart;
    float uSpan;
    GfxTlVertex *pFirstVertex;
    GfxTlVertex *vert3;
    GfxTlVertex *vertB0;
    GfxTlVertex *vertex13;
    GfxTlVertex *vertC1;
    GfxTlVertex *vertex30;
    GfxTlVertex *cdVertexPtrA;
    GfxTlVertex *screenVertB1;
    GfxTlVertex *screenVertB2;
    GfxTlVertex *screenVertB3;
    int depth;
    GfxTlVertex *node1Vertex2;
    GfxTlVertex *auxVertexB;
    float vHF;
    GfxTlVertex *pVertexA0;
    GfxTlVertex *nodeTwoVert2Ptr;
    GfxTlVertex *screenVertexA2;

    yRowBase = 0x18;
    inUStart = 0.0f;
    vBase = 0.0f;
    uSpan = 1.0f;
    vHF = 1.0f;
    depth = FLOAT_0_01_BITS;
    if (AcquireBatchNode(1, g_CdCheckHighNodes[0]) != 0) {
        pFirstVertex = (GfxTlVertex *)g_CdCheckHighNodes[0]->vertexCursor;
        pFirstVertex->x = 120.0f;
        pFirstVertex->y = (float)yRowBase;
        pFirstVertex->zBits = depth;
        pFirstVertex->rhw = 1.0f;
        pFirstVertex->color = 0x80FFFFFF;
        pFirstVertex->specular = 0xFF000000;
        pFirstVertex->u = inUStart;
        pFirstVertex->v = vBase;
        vertex1Ptr = (GfxTlVertex *)(g_CdCheckHighNodes[0]->vertexCursor + 0x20);
        vertex1Ptr->x = 120.0f;
        vertex1Ptr->y = (float)yRowBase + 256.0f;
        vertex1Ptr->zBits = depth;
        vertex1Ptr->rhw = 1.0f;
        vertex1Ptr->color = 0x80FFFFFF;
        vertex1Ptr->specular = 0xFF000000;
        vertex1Ptr->u = inUStart;
        vertex1Ptr->v = vBase + vHF;
        vert2 = (GfxTlVertex *)(g_CdCheckHighNodes[0]->vertexCursor + 0x40);
        vert2->x = 376.0f;
        vert2->y = (float)yRowBase;
        vert2->zBits = depth;
        vert2->rhw = 1.0f;
        vert2->color = 0x80FFFFFF;
        vert2->specular = 0xFF000000;
        vert2->u = inUStart + uSpan;
        vert2->v = vBase;
        vert3 = (GfxTlVertex *)(g_CdCheckHighNodes[0]->vertexCursor + 0x60);
        vert3->x = 376.0f;
        vert3->y = (float)yRowBase + 256.0f;
        vert3->zBits = depth;
        vert3->rhw = 1.0f;
        vert3->color = 0x80FFFFFF;
        vert3->specular = 0xFF000000;
        vert3->u = inUStart + uSpan;
        vert3->v = vBase + vHF;
    }
    uSpan = 0.5625f;
    if (AcquireBatchNode(1, g_CdCheckHighNodes[1]) != 0) {
        pVertexA0 = (GfxTlVertex *)g_CdCheckHighNodes[1]->vertexCursor;
        pVertexA0->x = 376.0f;
        pVertexA0->y = (float)yRowBase;
        pVertexA0->zBits = depth;
        pVertexA0->rhw = 1.0f;
        pVertexA0->color = 0x80FFFFFF;
        pVertexA0->specular = 0xFF000000;
        pVertexA0->u = inUStart;
        pVertexA0->v = vBase;
        node1Vertex1 = (GfxTlVertex *)(g_CdCheckHighNodes[1]->vertexCursor + 0x20);
        node1Vertex1->x = 376.0f;
        node1Vertex1->y = (float)yRowBase + 256.0f;
        node1Vertex1->zBits = depth;
        node1Vertex1->rhw = 1.0f;
        node1Vertex1->color = 0x80FFFFFF;
        node1Vertex1->specular = 0xFF000000;
        node1Vertex1->u = inUStart;
        node1Vertex1->v = vBase + vHF;
        node1Vertex2 = (GfxTlVertex *)(g_CdCheckHighNodes[1]->vertexCursor + 0x40);
        node1Vertex2->x = 520.0f;
        node1Vertex2->y = (float)yRowBase;
        node1Vertex2->zBits = depth;
        node1Vertex2->rhw = 1.0f;
        node1Vertex2->color = 0x80FFFFFF;
        node1Vertex2->specular = 0xFF000000;
        node1Vertex2->u = inUStart + uSpan;
        node1Vertex2->v = vBase;
        vertex13 = (GfxTlVertex *)(g_CdCheckHighNodes[1]->vertexCursor + 0x60);
        vertex13->x = 520.0f;
        vertex13->y = (float)yRowBase + 256.0f;
        vertex13->zBits = depth;
        vertex13->rhw = 1.0f;
        vertex13->color = 0x80FFFFFF;
        vertex13->specular = 0xFF000000;
        vertex13->u = inUStart + uSpan;
        vertex13->v = vBase + vHF;
    }
    vHF = 0.6875f;
    uSpan = 1.0f;
    if (AcquireBatchNode(1, g_CdCheckHighNodes[2]) != 0) {
        vertB0 = (GfxTlVertex *)g_CdCheckHighNodes[2]->vertexCursor;
        vertB0->x = 120.0f;
        vertB0->y = (float)yRowBase + 256.0f;
        vertB0->zBits = depth;
        vertB0->rhw = 1.0f;
        vertB0->color = 0x80FFFFFF;
        vertB0->specular = 0xFF000000;
        vertB0->u = inUStart;
        vertB0->v = vBase;
        vert21 = (GfxTlVertex *)(g_CdCheckHighNodes[2]->vertexCursor + 0x20);
        vert21->x = 120.0f;
        vert21->y = (float)yRowBase + 256.0f + 44.0f;
        vert21->zBits = depth;
        vert21->rhw = 1.0f;
        vert21->color = 0x80FFFFFF;
        vert21->specular = 0xFF000000;
        vert21->u = inUStart;
        vert21->v = vBase + vHF;
        nodeTwoVert2Ptr = (GfxTlVertex *)(g_CdCheckHighNodes[2]->vertexCursor + 0x40);
        nodeTwoVert2Ptr->x = 376.0f;
        nodeTwoVert2Ptr->y = (float)yRowBase + 256.0f;
        nodeTwoVert2Ptr->zBits = depth;
        nodeTwoVert2Ptr->rhw = 1.0f;
        nodeTwoVert2Ptr->color = 0x80FFFFFF;
        nodeTwoVert2Ptr->specular = 0xFF000000;
        nodeTwoVert2Ptr->u = inUStart + uSpan;
        nodeTwoVert2Ptr->v = vBase;
        auxVertexB = (GfxTlVertex *)(g_CdCheckHighNodes[2]->vertexCursor + 0x60);
        auxVertexB->x = 376.0f;
        auxVertexB->y = (float)yRowBase + 256.0f + 44.0f;
        auxVertexB->zBits = depth;
        auxVertexB->rhw = 1.0f;
        auxVertexB->color = 0x80FFFFFF;
        auxVertexB->specular = 0xFF000000;
        auxVertexB->u = inUStart + uSpan;
        auxVertexB->v = vBase + vHF;
    }
    uSpan = 0.5625f;
    if (AcquireBatchNode(1, g_CdCheckHighNodes[3]) != 0) {
        vertex30 = (GfxTlVertex *)g_CdCheckHighNodes[3]->vertexCursor;
        vertex30->x = 376.0f;
        vertex30->y = (float)yRowBase + 256.0f;
        vertex30->zBits = depth;
        vertex30->rhw = 1.0f;
        vertex30->color = 0x80FFFFFF;
        vertex30->specular = 0xFF000000;
        vertex30->u = inUStart;
        vertex30->v = vBase;
        vertC1 = (GfxTlVertex *)(g_CdCheckHighNodes[3]->vertexCursor + 0x20);
        vertC1->x = 376.0f;
        vertC1->y = (float)yRowBase + 256.0f + 44.0f;
        vertC1->zBits = depth;
        vertC1->rhw = 1.0f;
        vertC1->color = 0x80FFFFFF;
        vertC1->specular = 0xFF000000;
        vertC1->u = inUStart;
        vertC1->v = vBase + vHF;
        vertexC2Ptr = (GfxTlVertex *)(g_CdCheckHighNodes[3]->vertexCursor + 0x40);
        vertexC2Ptr->x = 520.0f;
        vertexC2Ptr->y = (float)yRowBase + 256.0f;
        vertexC2Ptr->zBits = depth;
        vertexC2Ptr->rhw = 1.0f;
        vertexC2Ptr->color = 0x80FFFFFF;
        vertexC2Ptr->specular = 0xFF000000;
        vertexC2Ptr->u = inUStart + uSpan;
        vertexC2Ptr->v = vBase;
        node3Vertex3 = (GfxTlVertex *)(g_CdCheckHighNodes[3]->vertexCursor + 0x60);
        node3Vertex3->x = 520.0f;
        node3Vertex3->y = (float)yRowBase + 256.0f + 44.0f;
        node3Vertex3->zBits = depth;
        node3Vertex3->rhw = 1.0f;
        node3Vertex3->color = 0x80FFFFFF;
        node3Vertex3->specular = 0xFF000000;
        node3Vertex3->u = inUStart + uSpan;
        node3Vertex3->v = vBase + vHF;
    }
    vHF = 0.578125f;
    uSpan = 1.0f;
    if (AcquireBatchNode(1, g_CdCheckHighNodes[4]) != 0) {
        pScreenVertA0 = (GfxTlVertex *)g_CdCheckHighNodes[4]->vertexCursor;
        pScreenVertA0->x = 120.0f;
        pScreenVertA0->y = (float)yRowBase + 3e+02f;
        pScreenVertA0->zBits = depth;
        pScreenVertA0->rhw = 1.0f;
        pScreenVertA0->color = 0x80FFFFFF;
        pScreenVertA0->specular = 0xFF000000;
        pScreenVertA0->u = inUStart;
        pScreenVertA0->v = vBase;
        screenVertexA1 = (GfxTlVertex *)(g_CdCheckHighNodes[4]->vertexCursor + 0x20);
        screenVertexA1->x = 120.0f;
        screenVertexA1->y = (float)yRowBase + 3e+02f + 148.0f;
        screenVertexA1->zBits = depth;
        screenVertexA1->rhw = 1.0f;
        screenVertexA1->color = 0x80FFFFFF;
        screenVertexA1->specular = 0xFF000000;
        screenVertexA1->u = inUStart;
        screenVertexA1->v = vBase + vHF;
        screenVertexA2 = (GfxTlVertex *)(g_CdCheckHighNodes[4]->vertexCursor + 0x40);
        screenVertexA2->x = 376.0f;
        screenVertexA2->y = (float)yRowBase + 3e+02f;
        screenVertexA2->zBits = depth;
        screenVertexA2->rhw = 1.0f;
        screenVertexA2->color = 0x80FFFFFF;
        screenVertexA2->specular = 0xFF000000;
        screenVertexA2->u = inUStart + uSpan;
        screenVertexA2->v = vBase;
        cdVertexPtrA = (GfxTlVertex *)(g_CdCheckHighNodes[4]->vertexCursor + 0x60);
        cdVertexPtrA->x = 376.0f;
        cdVertexPtrA->y = (float)yRowBase + 3e+02f + 148.0f;
        cdVertexPtrA->zBits = depth;
        cdVertexPtrA->rhw = 1.0f;
        cdVertexPtrA->color = 0x80FFFFFF;
        cdVertexPtrA->specular = 0xFF000000;
        cdVertexPtrA->u = inUStart + uSpan;
        cdVertexPtrA->v = vBase + vHF;
    }
    uSpan = 0.5625f;
    if (AcquireBatchNode(1, g_CdCheckHighNodes[5]) != 0) {
        screenVertB0 = (GfxTlVertex *)g_CdCheckHighNodes[5]->vertexCursor;
        screenVertB0->x = 376.0f;
        screenVertB0->y = (float)yRowBase + 3e+02f;
        screenVertB0->zBits = depth;
        screenVertB0->rhw = 1.0f;
        screenVertB0->color = 0x80FFFFFF;
        screenVertB0->specular = 0xFF000000;
        screenVertB0->u = inUStart;
        screenVertB0->v = vBase;
        screenVertB1 = (GfxTlVertex *)(g_CdCheckHighNodes[5]->vertexCursor + 0x20);
        screenVertB1->x = 376.0f;
        screenVertB1->y = (float)yRowBase + 3e+02f + 148.0f;
        screenVertB1->zBits = depth;
        screenVertB1->rhw = 1.0f;
        screenVertB1->color = 0x80FFFFFF;
        screenVertB1->specular = 0xFF000000;
        screenVertB1->u = inUStart;
        screenVertB1->v = vBase + vHF;
        screenVertB2 = (GfxTlVertex *)(g_CdCheckHighNodes[5]->vertexCursor + 0x40);
        screenVertB2->x = 520.0f;
        screenVertB2->y = (float)yRowBase + 3e+02f;
        screenVertB2->zBits = depth;
        screenVertB2->rhw = 1.0f;
        screenVertB2->color = 0x80FFFFFF;
        screenVertB2->specular = 0xFF000000;
        screenVertB2->u = inUStart + uSpan;
        screenVertB2->v = vBase;
        screenVertB3 = (GfxTlVertex *)(g_CdCheckHighNodes[5]->vertexCursor + 0x60);
        screenVertB3->x = 520.0f;
        screenVertB3->y = (float)yRowBase + 3e+02f + 148.0f;
        screenVertB3->zBits = depth;
        screenVertB3->rhw = 1.0f;
        screenVertB3->color = 0x80FFFFFF;
        screenVertB3->specular = 0xFF000000;
        screenVertB3->u = inUStart + uSpan;
        screenVertB3->v = vBase + vHF;
    }
}

/*
 * Opens disc_us.lgp and creates render objects for character and disc graphics.
 */
// FUNCTION: FF7 0x0040414a
void __cdecl CdCheck_LoadDiscCharXTims(void)
{
    struct {
        int mode;
        char *file;
        unsigned int random;
        PfileLoadOptions opt;
        unsigned int i;
        struct GfxEngineContext *ctx;
        char msg[256];
        char path[256];
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.ctx = GetEngineContext();
    f.mode = GetUiScalingMode();
    f.random = Savemap_GetClockTickCount();
    f.i = f.random % 9;
    strcpy(f.path, GetDataPath());
    // STRING: FF7 0x007ba5e4
    strcat(f.path, "cd/disc_us.lgp");
    if (!OpenLgpArchive(f.path, LGP_ARCHIVE_DISC)) {
        // STRING: FF7 0x007ba5f4
        sprintf(f.msg, "Failed to load: %s\n", f.path);
        Gfx_DebugPrint(f.msg);
    }
    Pfile_SetAssetSourceMode(0);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &f.opt);
    f.opt.file.fromArchive = 1;
    f.opt.file.archiveId = 16;
    f.opt.file.callback = 0;
    f.opt.directory = g_CdCheckEmptyPath;
    if (f.mode == 2) {
        f.i = f.i << 2;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &f.opt);
        f.file = g_cdCheckCharQuadTimNames[f.i];
        f.i++;
        g_CdCheckHighNodes[0] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        f.file = g_cdCheckCharQuadTimNames[f.i];
        f.i++;
        g_CdCheckHighNodes[1] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        f.file = g_cdCheckCharQuadTimNames[f.i];
        f.i++;
        g_CdCheckHighNodes[2] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        f.file = g_cdCheckCharQuadTimNames[f.i];
        f.i++;
        g_CdCheckHighNodes[3] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        f.opt.extraFlags = f.opt.extraFlags | 1;
        f.opt.colorKey = 0xff141414;
        if (gSavemap.discChangeRequest == 1) {
            f.file = g_cdCheckDiscTimNames[0];
            g_CdCheckHighNodes[4] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
            f.file = g_cdCheckDiscTimNames[1];
            g_CdCheckHighNodes[5] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        } else if (gSavemap.discChangeRequest == 2) {
            f.file = g_cdCheckDiscTimNames[2];
            g_CdCheckHighNodes[4] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
            f.file = g_cdCheckDiscTimNames[3];
            g_CdCheckHighNodes[5] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        } else if (gSavemap.discChangeRequest == 3) {
            f.file = g_cdCheckDiscTimNames[4];
            g_CdCheckHighNodes[4] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
            f.file = g_cdCheckDiscTimNames[5];
            g_CdCheckHighNodes[5] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        }
    } else {
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &f.opt);
        f.file = g_cdCheckCharTimNames[f.i];
        g_CdCheckLowCharBatch = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
        if (gSavemap.discChangeRequest == 1)
            f.file = g_cdCheckDiscTimNames[6];
        else if (gSavemap.discChangeRequest == 2)
            f.file = g_cdCheckDiscTimNames[7];
        else if (gSavemap.discChangeRequest == 3)
            f.file = g_cdCheckDiscTimNames[8];
        g_CdCheckLowDiscBatch = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &f.opt, f.file, (void *)f.ctx->renderObjectBank);
    }
    f.opt.renderFlags = f.opt.renderFlags | 0x4000;
    Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &f.opt);
    g_CdCheckFadeBatch = (GfxRenderObject *)Gfx_CreateRenderObject(0, GFX_DRAW_FLAT_QUAD_2D, &f.opt, 0, (void *)f.ctx->renderObjectBank);
}

/*
 * Enters the "insert disc" screen (GAME_MODE_DISC_CHECK), which GameModeDispatcher_Enter selects
 * when the disc in the drive is not gSavemap.discChangeRequest. It copies the drive prefix of
 * the data path, arms the fade-in from black (alpha 255) and the fade-out, sets
 * g_CdCheckActiveFlag (which keeps CdCheck_HandleDeviceChange from running cdcheck.exe while this
 * screen is up) and sets the viewport by GetUiScalingMode: mode 0 is 320x240 at the origin,
 * mode 1 is 320x240 at (160,120), any other mode 640x480 at the origin. Then
 * CdCheck_LoadDiscCharXTims opens cd/disc_us.lgp as archive 16 and creates the picture: one
 * of nine characters, picked by the play-time clock modulo 9, and the image of the requested
 * disc, plus the fade quad.
 */
// FUNCTION: FF7 0x00404688
void __cdecl CdCheck_ModuleEnter(void)
{
    int mode;

    mode = GetUiScalingMode();
    strncpy((char *)&g_CdCheckDrivePrefix, g_InitPath_DataDrive, 2);
    g_CdCheckFadeInActive = 1;
    g_CdCheckFadeOutActive = 1;
    g_CdCheckActiveFlag = 1;
    g_CdCheckFadeOutAlpha = 0;
    g_CdCheckIntroFadeAlpha = 0xFF;
    if (mode == 0) {
        g_CdCheckViewportX = 0;
        g_CdCheckViewportY = 0;
        g_CdCheckViewportWidth = 320;
        g_CdCheckViewportHeight = 240;
    } else if (mode == 1) {
        g_CdCheckViewportX = 160;
        g_CdCheckViewportY = 120;
        g_CdCheckViewportWidth = 320;
        g_CdCheckViewportHeight = 240;
    } else {
        g_CdCheckViewportX = 0;
        g_CdCheckViewportY = 0;
        g_CdCheckViewportWidth = 640;
        g_CdCheckViewportHeight = 480;
    }
    CdCheck_LoadDiscCharXTims();
}

/*
 * Leaves the "insert disc" screen once the right disc is in: reopens the core LGP archives on it
 * (App_OpenCoreArchives), releases the screen's render objects (the six 640x480 tiles, the two
 * 320x240 images and the fade quad), goes back to the game mode that was interrupted
 * (g_EnginePreviousMode, with GAME_MODE_DISC_CHECK as the new previous mode), closes archive 16
 * (cd/disc_us.lgp) and clears g_CdCheckActiveFlag so that disc removal prompts again.
 */
// FUNCTION: FF7 0x0040476c
void __cdecl CdCheck_ModuleExit(void)
{
    int i;

    App_OpenCoreArchives();
    for (i = 0; i < 6; i++) {
        DestroyRenderObjectAndClear(i * 4 + (char *)g_CdCheckHighNodes);
    }
    DestroyRenderObjectAndClear(&g_CdCheckLowCharBatch);
    DestroyRenderObjectAndClear(&g_CdCheckLowDiscBatch);
    DestroyRenderObjectAndClear(&g_CdCheckFadeBatch);
    g_EngineGameMode = g_EnginePreviousMode;
    g_EnginePreviousMode = GAME_MODE_DISC_CHECK;
    CloseLgpArchive(LGP_ARCHIVE_DISC);
    g_CdCheckActiveFlag = 0;
}

/*
 * One frame of the "insert disc" screen. It clears to black with linear filtering off and draws
 * the picture for the UI scaling mode: the six tiles in mode 2, the character and disc images in
 * modes 0 and 1 (each mode builds its own quads). It checks the drive every frame
 * (CdCheck_GetGameDiscNumber): once the requested disc (gSavemap.discChangeRequest) is in, it fades to
 * black by 51 a frame and, past 255, hands control back to the game-mode dispatcher
 * (RequestModuleSwitch with the GameModeDispatcher_* callbacks). The fade-in from black also
 * steps by 51 a frame until it reaches 0. There is no input: the screen only waits for the disc.
 */
// FUNCTION: FF7 0x004047f6
void __cdecl CdCheck_ModuleFrame(struct GfxEngineContext *ctx)
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
            CdCheck_BuildScreenQuadsMode2();
            Gfx_EmitPrimitiveBatchList(g_CdCheckHighNodes[0], ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckHighNodes[1], ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckHighNodes[2], ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckHighNodes[3], ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckHighNodes[4], ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckHighNodes[5], ctx);
        } else if (uiScale == 0) {
            CdCheck_BuildScreenQuadsMode0();
            Gfx_EmitPrimitiveBatchList(g_CdCheckLowCharBatch, ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckLowDiscBatch, ctx);
        } else if (uiScale == 1) {
            CdCheck_BuildScreenQuadsMode1();
            Gfx_EmitPrimitiveBatchList(g_CdCheckLowCharBatch, ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckLowDiscBatch, ctx);
        }
        if (CdCheck_GetGameDiscNumber() == gSavemap.discChangeRequest && g_CdCheckFadeOutActive != 0) {
            CdCheck_FillFadeQuad((unsigned char)g_CdCheckFadeOutAlpha);
            Gfx_SetScenePassCall(1, ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckFadeBatch, ctx);
            g_CdCheckFadeOutAlpha += 51;
            if (g_CdCheckFadeOutAlpha > 255) {
                g_CdCheckFadeOutActive = 0;
                nextModule.enter = GameModeDispatcher_Enter;
                nextModule.frame = GameModeDispatcher_Frame;
                nextModule.onMouse = App_OnMouseMessage;
                nextModule.onKey = App_OnKeyMessage;
                nextModule.exit = GameModeDispatcher_Exit;
                RequestModuleSwitch(&nextModule, ctx);
            }
        }
        if (g_CdCheckFadeInActive != 0) {
            CdCheck_FillFadeQuad((unsigned char)g_CdCheckIntroFadeAlpha);
            Gfx_SetScenePassCall(1, ctx);
            Gfx_EmitPrimitiveBatchList(g_CdCheckFadeBatch, ctx);
            g_CdCheckIntroFadeAlpha -= 51;
            if (g_CdCheckIntroFadeAlpha <= 0) {
                g_CdCheckFadeInActive = 0;
            }
        }
        Gfx_EndSceneCall(ctx);
    }
}

/*
 * CD-check module keyboard callback (no-op in original).
 */
// FUNCTION: FF7 0x00404a73
void __cdecl CdCheck_ModuleKeyboard(void)
{
}

/*
 * CD-check module mouse callback (no-op in original).
 */
// FUNCTION: FF7 0x00404a78
void __cdecl CdCheck_ModuleMouse(void)
{
}

/*
 * Returns which FF7 disc is in the drive: reads the volume label of the data drive
 * (g_InitPath_DataDrive, also copied into g_CdCheckDrivePrefix) with GetVolumeInformationA and
 * returns 1, 2 or 3 for the labels FF7DISC1, FF7DISC2 and FF7DISC3, or 0 for no disc, an unreadable
 * drive or any other label. The result is also kept in g_CdCheckDetectedDisc. It turns off the
 * critical-error dialog (SetErrorMode(1)) first so an empty drive does not prompt and does not
 * restore the mode. GameModeDispatcher_Enter compares it with gSavemap.discChangeRequest to decide
 * whether to show the "insert disc" screen; CdCheck_ModuleFrame polls it every frame.
 */
// FUNCTION: FF7 0x00404a7d
int __cdecl CdCheck_GetGameDiscNumber(void)
{
    int infoResult;
    char q[256];
    char s[256];
    char volumeLabel[256];
    g_CdCheckDetectedDisc = 0;
    strncpy((char *)&g_CdCheckDrivePrefix, g_InitPath_DataDrive, 2);
    SetErrorMode(1);
    infoResult = GetVolumeInformationA(g_InitPath_DataDrive, volumeLabel, 256, 0, 0, 0, 0, 0);
    if (infoResult != 0) {
        if (strcmp(volumeLabel,
                   // STRING: FF7 0x007ba608
                   "FF7DISC1") == 0) {
            g_CdCheckDetectedDisc = 1;
        } else if (strcmp(volumeLabel,
                          // STRING: FF7 0x007ba614
                          "FF7DISC2") == 0) {
            g_CdCheckDetectedDisc = 2;
        } else if (strcmp(volumeLabel,
                          // STRING: FF7 0x007ba620
                          "FF7DISC3") == 0) {
            g_CdCheckDetectedDisc = 3;
        }
    }
    return g_CdCheckDetectedDisc;
}

/*
 * Handles WM_DEVICECHANGE: prompts for the requested disc and spawns cdcheck.exe.
 */
// FUNCTION: FF7 0x00404b4a
void __cdecl CdCheck_HandleDeviceChange(int message, int deviceData)
{
    struct {
        HKEY key;
        DWORD disc;
        STARTUPINFOA startup;
        int failure;
        LONG status;
        char valueName[12];
        char label[256];
        PROCESS_INFORMATION process;
        char path[256];
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_CdCheckActiveFlag || message == DBT_DEVICEARRIVAL)
        return;
    if (message == DBT_DEVICEREMOVECOMPLETE) {
        f.startup.cb = 0;
        memset(&f.startup.lpReserved, 0, 64);
        f.process.hProcess = 0;
        memset(&f.process.hThread, 0, 12);
        f.failure = 1;
        // STRING: FF7 0x007ba62c
        memcpy(f.valueName, "DiscNo", 7);
        g_CdCheckActiveFlag = 0;
        // STRING: FF7 0x007ba634
        sprintf(f.label, "Please insert Disc %i to continue.", gSavemap.discChangeRequest);
        Menu_DialogShowCenteredMessage((int)f.label);
        f.status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, REG_ROOT_KEY, 0, KEY_WRITE, &f.key);
        if (!f.status) {
            f.failure = 1;
            f.disc = gSavemap.discChangeRequest;
            f.status = RegSetValueExA(f.key, f.valueName, 0, REG_DWORD, (unsigned char *)&f.disc, 4);
            if (!f.status)
                f.failure = 0;
            f.status = RegCloseKey(f.key);
        }
        f.startup.cb = 68;
        f.startup.lpReserved = 0;
        f.startup.lpReserved2 = 0;
        f.startup.cbReserved2 = 0;
        f.startup.lpDesktop = 0;
        f.startup.dwFlags = 0;
        strcpy(f.path, g_InitPath_AppPath);
        // STRING: FF7 0x007ba658
        strcat(f.path, "cdcheck.exe");
        CreateProcessA(0, f.path, 0, 0, 1, 32, 0, 0, &f.startup, &f.process);
        WaitForSingleObject(f.process.hProcess, INFINITE);
    }
}
} /* extern "C" */

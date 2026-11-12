// src/ff7/snobo/snobo_renderer_hud.cpp
// The snowboard renderer and HUD: a SnowboardRenderer class that owns the render objects and
// draws panels, course geometry, menus and text, and a SnowboardHud class that updates and draws the
// race time, record and speed readouts. Small helpers split times, pack digits and handle the
// input masks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/snobo_course.h"
#include <stdlib.h>
#include "lib/crt.h"
#include "lib/enums/graphics.h"
#include "ff7/types/snobo.h"
#include "lib/types/graphics.h"
#include <math.h>
#include <string.h>
#include "ff7/snobo_primitives.h"
#include "ff7/api/snobo.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"
#include "ff7/enums/snobo.h"
#include "ff7/api/main.h"

#pragma intrinsic(abs)
typedef struct {
    short x, y, count, rectX, rectY, width, height, reserved;
    char **items;
} SnowboardHudMenuDescriptor;
#pragma intrinsic(memset)
// One 36-byte row of g_SnoboHudVertices: a screen rectangle, its UV corners and the
// model whose batch it is drawn into.
typedef struct {
    int x, y, w, h;
    float u0, v0, u1, v1;
    GfxRenderObject **model;
} SnowboardHudQuad;
extern "C" {
__declspec(dllimport) unsigned long __stdcall timeGetTime();

// clang-format off
// The blank three-digit template SnowboardRenderer::drawNumber fills, then the "1ST." to "5TH."
// labels of the ranking screen.
typedef struct {
    int numberTemplate;
    char rankLabels[5][8];
    int pad;
} SnowboardRankText;
extern SnowboardRankText g_SnoboRankText; /* Defined later in this TU (retail data order). */
extern "C" char g_SnoboKeyNamePool[14][4];
// GLOBAL: FF7 0x00956638
SnowboardKeyBinding g_SnoboInputBindings[14] = {
    {0x8000, 0x8000, g_SnoboKeyNamePool[0]},
    {0x2000, 0x2000, g_SnoboKeyNamePool[1]},
    {0x1000, 0x1000, g_SnoboKeyNamePool[2]},
    {0x4000, 0x4000, g_SnoboKeyNamePool[3]},
    {0x20, 0x20, g_SnoboKeyNamePool[4]},
    {0x80, 0x80, g_SnoboKeyNamePool[5]},
    {0x40, 0x40, g_SnoboKeyNamePool[6]},
    {0x10, 0x10, g_SnoboKeyNamePool[7]},
    {0x4, 0x4, g_SnoboKeyNamePool[8]},
    {0x1, 0x1, g_SnoboKeyNamePool[9]},
    {0x8, 0x8, g_SnoboKeyNamePool[10]},
    {0x2, 0x2, g_SnoboKeyNamePool[11]},
    {0x800, 0x800, g_SnoboKeyNamePool[12]},
    {0x100, 0x100, g_SnoboKeyNamePool[13]}
};
// The table of 36-byte SnowboardHudQuad rows that renderStateMenu walks.
// GLOBAL: FF7 0x009566e0
SnowboardHudQuad g_SnoboHudVertices[12] = {
    {0x10, 0x13b, 0xc0, 0x60, 0.375f, 0.5625f, 0.75f, 0.75f, &g_SnoboEstamp0Model},
    {0x10, 0x13b, 0xc0, 0x60, 0.375f, 0.375f, 0.75f, 0.5625f, &g_SnoboEstamp0Model},
    {0x10, 0x13b, 0xc0, 0x60, 0.375f, 0.1875f, 0.75f, 0.375f, &g_SnoboEstamp0Model},
    {0x10, 0x13b, 0xc0, 0x60, 0.375f, 0.0f, 0.75f, 0.1875f, &g_SnoboEstamp0Model},
    {0x10, 0x13b, 0xc0, 0x60, 0.0f, 0.5625f, 0.375f, 0.75f, &g_SnoboEstamp0Model},
    {0x10, 0x13b, 0xc0, 0x60, 0.0f, 0.375f, 0.375f, 0.5625f, &g_SnoboEstamp0Model},
    {0x10, 0x13b, 0xc0, 0x60, 0.0f, 0.1875f, 0.375f, 0.375f, &g_SnoboEstamp0Model},
    {0x10, 0x13b, 0xc0, 0x60, 0.0f, 0.0f, 0.375f, 0.1875f, &g_SnoboEstamp0Model},
    {0x16, 0x12d, 0xb4, 0x94, 0.0f, 0.28515625f, 0.3515625f, 0.57421875f, &g_SnoboEstamp1Model},
    {0x15, 0x12e, 0xb6, 0x92, 0.3515625f, 0.37890625f, 0.70703125f, 0.6640625f, &g_SnoboEstamp1Model},
    {0x13, 0x12e, 0xb7, 0x92, 0.0f, 0.0f, 0.36328125f, 0.28515625f, &g_SnoboEstamp1Model},
    {0x16, 0x12d, 0xb4, 0x94, 0.36328125f, 0.0f, 0.8203125f, 0.37890625f, &g_SnoboEstamp1Model},
};
// The texture names SnowboardRenderer::initialize loads the time and stamp models from.
// GLOBAL: FF7 0x00956890
int g_SnoboTime1NamePtr = (int)g_SnoboTimeTextureNames;
// GLOBAL: FF7 0x00956894
int g_SnoboTime2NamePtr = (int)(g_SnoboTimeTextureNames + 12);
// GLOBAL: FF7 0x00956898
int g_SnoboEstamp0NamePtr = (int)g_SnoboEstampTextureNames;
// GLOBAL: FF7 0x0095689c
int g_SnoboEstamp1NamePtr = (int)(g_SnoboEstampTextureNames + 12);
// GLOBAL: FF7 0x009568a0
char g_SnoboTimeStr[16] = {0x30, 0x30, 0x1a, 0x30, 0x30, 0x1b, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x009568b0
char g_SnoboScoreStr[16] = {0x1f, 0x1f, 0x1a, 0x1f, 0x1f, 0x1b, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// The record time text packDigits fills (ten bytes used).
// GLOBAL: FF7 0x009568c0
char g_SnoboRecordTimeStr[16] = {0x30, 0x30, 0x27, 0x30, 0x30, 0x22, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// The record time text drawn when there is no record yet (ten bytes used).
// GLOBAL: FF7 0x009568d0
char g_SnoboNoRecordStr[16] = {0x2d, 0x2d, 0x27, 0x2d, 0x2d, 0x22, 0x2d, 0x2d, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x009568e0
char *g_SnoboMenuStrings0[3] = {g_SnoboMenuLabelPool, (g_SnoboMenuLabelPool + 12), (g_SnoboMenuLabelPool + 20)};
// GLOBAL: FF7 0x009568f0
char *g_SnoboMenuStrings1[4] = {(g_SnoboMenuLabelPool + 28), (g_SnoboMenuLabelPool + 36), (g_SnoboMenuLabelPool + 48), (g_SnoboMenuLabelPool + 56)};
// GLOBAL: FF7 0x00956900
char *g_SnoboMenuStrings2[2] = {(g_SnoboMenuLabelPool + 64), (g_SnoboMenuLabelPool + 72)};
// GLOBAL: FF7 0x00956908
char *g_SnoboMenuStrings3[4] = {(g_SnoboMenuLabelPool + 80), (g_SnoboMenuLabelPool + 84), (g_SnoboMenuLabelPool + 92), (g_SnoboMenuLabelPool + 100)};
// GLOBAL: FF7 0x00956918
char *g_SnoboMenuStrings4[2] = {(g_SnoboMenuLabelPool + 108), (g_SnoboMenuLabelPool + 116)};
// GLOBAL: FF7 0x00956920
char *g_SnoboMenuStrings5[2] = {(g_SnoboMenuLabelPool + 124), (g_SnoboMenuLabelPool + 136)};
// GLOBAL: FF7 0x00956928
SnowboardHudMenuDescriptor g_SnoboHudMenus[6] = {
    {248, 192, 3, 220, 170, 200, 128, 0, g_SnoboMenuStrings0},
    {248, 180, 4, 220, 158, 200, 162, 0, g_SnoboMenuStrings1},
    {248, 212, 2, 220, 190, 200, 98, 0, g_SnoboMenuStrings2},
    {248, 180, 4, 220, 158, 200, 162, 0, g_SnoboMenuStrings3},
    {248, 212, 2, 220, 190, 200, 98, 0, g_SnoboMenuStrings4},
    {248, 336, 2, 220, 314, 200, 98, 0, g_SnoboMenuStrings5},
};
// GLOBAL: FF7 0x009569a0
float g_SnoboHudFloats[16] = {0, 0, 0, 1, 0.5, 0, 0.5, 1, 0.5, 0, 0.5, 1, 1, 0, 1, 1};
// GLOBAL: FF7 0x009569e0
char *g_SnoboMenuTexts1[5] = {g_SnoboResultLabelPool, g_SnoboResultLabelPool + 12, g_SnoboResultLabelPool + 20, g_SnoboResultLabelPool + 28, g_SnoboResultLabelPool + 36};
// GLOBAL: FF7 0x009569f8
char *g_SnoboMenuTexts2[5] = {g_SnoboResultLabelPool + 44, g_SnoboResultLabelPool + 56, g_SnoboResultLabelPool + 68, g_SnoboResultLabelPool + 80, g_SnoboResultLabelPool + 88};
// GLOBAL: FF7 0x00956a10
char g_SnoboCourseARecordStr[16] = "COURSE A RECORD";
extern "C" unsigned char g_rawData_00956a20[32] = {
    0x30, 0x30, 0x27, 0x30, 0x30, 0x22, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x2d, 0x27, 0x2d, 0x2d, 0x22, 0x2d, 0x2d,
    0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x00956a40
char *g_SnoboTextPointers[5] = {g_SnoboRankText.rankLabels[0], g_SnoboRankText.rankLabels[1], g_SnoboRankText.rankLabels[2], g_SnoboRankText.rankLabels[3], g_SnoboRankText.rankLabels[4]};
// GLOBAL: FF7 0x00956a58
unsigned int g_SnoboHudTable[150] = {0, 1441808, 16, 1441808, 32, 1441808, 48, 1441808, 64, 1441808, 80, 1441808, 96, 1441808, 112, 1441808, 128, 1441808, 144, 1441808, 160, 655368, 168, 655372, 25228, 1441852, 5632, 1441852, 41984, 1572967, 20655, 1179664, 0, 0, 0, 0, 20685, 524300, 16032, 1179688, 25088, 1441864, 0, 0, 0, 0, 20673, 524296, 20700, 524298, 20712, 524300, 20724, 524300, 22748, 524300, 16116, 1179656, 20655, 1179664, 16088, 1179656, 16096, 1179668, 20480, 1179664, 20496, 1179664, 20512, 1179664, 20528, 1179664, 20544, 1179664, 20560, 1179664, 20576, 1179664, 20592, 1179664, 20608, 1179664, 20624, 1179664, 0, 0, 0, 0, 0, 0, 0, 0, 20640, 1179662, 16072, 1179664, 0, 0, 11264, 1179664, 11280, 1179664, 11296, 1179664, 11312, 1179664, 11328, 1179664, 11344, 1179664, 11360, 1179664, 11376, 1179664, 11392, 1179664, 11408, 1179664, 11424, 1179664, 11440, 1179664, 11456, 1179664, 11472, 1179664, 11488, 1179664, 11504, 1179664, 15872, 1179664, 15888, 1179664, 15904, 1179664, 15920, 1179664, 15936, 1179664, 15952, 1179664, 15968, 1179664, 15984, 1179664, 16000, 1179664, 16016, 1179664};
extern "C" char g_SnoboKeyNamePool[14][4] = {
    {'L', 0, 0, 0}, {'R', 0, 0, 0}, {'U', 0, 0, 0}, {'D', 0, 0, 0},
    {'X', 0, 0, 0}, {'S', 0, 0, 0}, {'T', 0, 0, 0}, {'O', 0, 0, 0},
    {'L', '1', 0, 0}, {'L', '2', 0, 0}, {'R', '1', 0, 0}, {'R', '2', 0, 0},
    {'S', 'T', 0, 0}, {'S', 'L', 0, 0}
};
// GLOBAL: FF7 0x00956ce8
char g_SnoboTimeTextureNames[24] =
    "time1.tim\0\0\0"
    "time2.tim";
// GLOBAL: FF7 0x00956d00
char g_SnoboEstampTextureNames[24] =
    "estamp0.tim\0"
    "estamp1.tim";
// GLOBAL: FF7 0x00956d18
char g_SnoboTextureNames[132] =
    "cfont.tim\0\0\0"
    "cfont.tim\0\0\0"
    "waku.tim\0\0\0\0"
    "bg0A.tim\0\0\0\0"
    "bg0B.tim\0\0\0\0"
    "bg0C.tim\0\0\0\0"
    "bg0D.tim\0\0\0\0"
    "bg0E.tim\0\0\0\0"
    "bg0F.tim\0\0\0\0"
    "bg0G.tim\0\0\0\0"
    "bg0H.tim";

// The two stamp models renderStateMenu draws into (render object handles from buildModel).
// GLOBAL: FF7 0x00dd86a8
GfxRenderObject *g_SnoboEstamp1Model = 0;
// GLOBAL: FF7 0x00dd86ac
GfxRenderObject *g_SnoboEstamp0Model = 0;
// The digit buffers splitTime fills and packDigits reads (7 ints each): the course time and the record.
// GLOBAL: FF7 0x00dd86b0
int g_SnoboCourseTimeDigits[7] = {0};
// GLOBAL: FF7 0x00dd86cc
int g_SnoboViewportScaleY = 0;
// GLOBAL: FF7 0x00dd86d0
int g_SnoboClipTop = 0;
// GLOBAL: FF7 0x00dd86d4
int g_SnoboViewportScaleX = 0;
// GLOBAL: FF7 0x00dd86d8
int g_SnoboRecordTimeDigits[7] = {0};
// GLOBAL: FF7 0x00dd86f4
int g_SnoboClipLeft = 0;
// clang-format on

#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)

#pragma intrinsic(abs)
#pragma function(abs)

/*
 * Rebuilds the module's two button masks each frame from g_SnoboInputBindings: the pressed
 * bits into modeFlags and the held bits into inputFlags.
 */
// FUNCTION: FF7 0x0072ca90
void __cdecl Mini_SnowboardUpdateInputMasks(void)
{
    int i;

    g_SnoboStatePtr->modeFlags = 0;
    g_SnoboStatePtr->inputFlags = 0;
    for (i = 0; i < 0xE; i++) {
        if (Input_GetPressed(g_SnoboInputBindings[i].key) != 0) {
            g_SnoboStatePtr->modeFlags |= g_SnoboInputBindings[i].bit;
        }
        if (Input_GetHeld(g_SnoboInputBindings[i].key) != 0) {
            g_SnoboStatePtr->inputFlags |= g_SnoboInputBindings[i].bit;
        }
    }
}
} // extern "C"

/*
 * Empty; initialize creates the render objects.
 */
// FUNCTION: FF7 0x0072cb4c
SnowboardRenderer::SnowboardRenderer()
{
}

/*
 * Creates renderer assets: two cfont.tim models and waku.tim, optionally eight
 * bg0A.tim through bg0H.tim models when g_SnoboModeFlag is 1, otherwise clears
 * their handles. Creates three untextured models and four additional table-
 * named models, storing two in globals.
 */
// FUNCTION: FF7 0x0072cb5a
unsigned char SnowboardRenderer::initialize()
{
    struct {
        int zeroIndex, checkIndex, count;
        int options[29];
        GfxEngineContext *context;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.context = GetEngineContext();
    frame.options[0] = 0;
    memset(frame.options + 1, 0, 112);
    fontModel = buildModel(12, 4, (int)g_SnoboTextureNames, 128);
    menuFontModel = buildModel(12, 4, (int)(g_SnoboTextureNames + 12), 128);
    if (!fontModel)
        return 0;
    if (!menuFontModel)
        return 0;
    wakuModel = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 24), 128);
    if (!wakuModel)
        return 0;
    if (g_SnoboModeFlag == 1) {
        frame.count = 3200;
        backgroundModels[0] = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 36), frame.count);
        backgroundModels[1] = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 48), frame.count);
        backgroundModels[2] = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 60), frame.count);
        backgroundModels[3] = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 72), frame.count);
        backgroundModels[4] = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 84), frame.count);
        backgroundModels[5] = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 96), frame.count);
        backgroundModels[6] = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 108), frame.count);
        backgroundModels[7] = (GfxRenderObject *)buildModel(12, 4, (int)(g_SnoboTextureNames + 120), frame.count);
        for (frame.checkIndex = 0; frame.checkIndex < 8; frame.checkIndex++)
            if (!backgroundModels[frame.checkIndex])
                return 0;
    } else {
        for (frame.zeroIndex = 0; frame.zeroIndex < 8; frame.zeroIndex++)
            backgroundModels[frame.zeroIndex] = 0;
    }
    stripModel = (GfxRenderObject *)buildModel(10, 4, 0, 3072);
    fadeModel = buildModel(8, 0, 0, 18432);
    rectPanelModel = buildModel(8, 0, 0, 0);
    time1Model = (GfxRenderObject *)buildModel(12, 4, g_SnoboTime1NamePtr, 0);
    time2Model = (GfxRenderObject *)buildModel(12, 0, g_SnoboTime2NamePtr, 0);
    g_SnoboEstamp0Model = (GfxRenderObject *)buildModel(12, 4, g_SnoboEstamp0NamePtr, 0);
    g_SnoboEstamp1Model = (GfxRenderObject *)buildModel(12, 4, g_SnoboEstamp1NamePtr, 0);
    if (!time1Model)
        return 0;
    if (!time2Model)
        return 0;
    if (!g_SnoboEstamp0Model)
        return 0;
    if (!g_SnoboEstamp1Model)
        return 0;
    Mini_SnowboardCaptureViewportScale(this);
    return 1;
}

extern "C" {
// GLOBAL: FF7 0x00956d9c
char g_SnoboTimeLabel[2] = {0x1c, 0x00};
// GLOBAL: FF7 0x00956da0
char g_SnoboBestLabel[5] = "BEST";
// Eight-character speed display template filled by SnowboardHud::renderSpeed.
// GLOBAL: FF7 0x00956da8
int g_SnoboSpeedTemplateHead = 0x20202024;
// GLOBAL: FF7 0x00956dac
int g_SnoboSpeedTemplateTail = 0x001d2020;
// GLOBAL: FF7 0x00956db0
char g_SnoboMenuLabelPool[144] = {0x43, 0x4f, 0x4e, 0x54, 0x49, 0x4e, 0x55, 0x45, 0x00, 0x00, 0x00, 0x00, 0x52, 0x45, 0x54, 0x52, 0x59, 0x00, 0x00, 0x00, 0x45, 0x58, 0x49, 0x54, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x50, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x00, 0x43, 0x4f, 0x4e, 0x54, 0x49, 0x4e, 0x55, 0x45, 0x00, 0x00, 0x00, 0x00, 0x52, 0x45, 0x54, 0x52, 0x59, 0x00, 0x00, 0x00, 0x45, 0x58, 0x49, 0x54, 0x00, 0x00, 0x00, 0x00, 0x47, 0x48, 0x4f, 0x53, 0x54, 0x00, 0x00, 0x00, 0x53, 0x55, 0x52, 0x46, 0x41, 0x43, 0x45, 0x00, 0x4f, 0x46, 0x46, 0x00, 0x42, 0x4f, 0x41, 0x52, 0x44, 0x00, 0x00, 0x00, 0x53, 0x4c, 0x45, 0x49, 0x47, 0x48, 0x00, 0x00, 0x3f, 0x20, 0x3f, 0x20, 0x3f, 0x00, 0x00, 0x00, 0x53, 0x4e, 0x4f, 0x57, 0x00, 0x00, 0x00, 0x00, 0x43, 0x48, 0x45, 0x43, 0x4b, 0x45, 0x44, 0x00, 0x54, 0x52, 0x59, 0x20, 0x41, 0x47, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x45, 0x58, 0x49, 0x54, 0x00, 0x00, 0x00, 0x00};
// The glyph Mini_SnowboardDrawPauseOverlay draws.
// GLOBAL: FF7 0x00956e40
short g_SnoboPauseGlyph = 30;
// GLOBAL: FF7 0x00956e44
char g_SnoboMenuCursorGlyph[2] = {0x3e, 0x00};
// GLOBAL: FF7 0x00956e48
char g_SnoboResultLabelPool[96] =
    "TECHNIQUE\0\0\0"
    "TIME\0\0\0\0"
    "BALLOON\0"
    "TOTAL\0\0\0"
    "BEST\0\0\0\0"
    "/30 PTS.\0\0\0\0"
    "/30 PTS.\0\0\0\0"
    "/40 PTS.\0\0\0\0"
    "/# PTS.\0"
    "/# PTS.";
// GLOBAL: FF7 0x00956ea8
SnowboardRankText g_SnoboRankText = {0x00202020, {"1ST.", "2ND.", "3RD.", "4TH.", "5TH."}, 0};
// The three sky bands of the course background (SnowboardCourseDesc rows); the event course
// draws the last two.
// GLOBAL: FF7 0x00956ed8
struct SnowboardCourseDesc g_SnoboHudShorts[3] = {{0, 32, 0x38e0e0e0, 0x38ffffff}, {-80, 80, 0x38feccad, 0x38e0e0e0}, {-160, 80, 0x38d75c0f, 0x38feccad}};
}

/*
 * Destroys fontModel, menuFontModel, wakuModel, rectPanelModel, stripModel and
 * fadeModel, additionally the eight backgroundModels when g_SnoboModeFlag==1,
 * then time1Model, time2Model and g_SnoboEstamp0Model/g_SnoboEstamp1Model; does
 * not clear these pointers.
 */
// FUNCTION: FF7 0x0072cecc
SnowboardRenderer::~SnowboardRenderer()
{
    int index;
    DestroyRenderObject((void *)fontModel);
    DestroyRenderObject((void *)menuFontModel);
    DestroyRenderObject(wakuModel);
    DestroyRenderObject((void *)rectPanelModel);
    DestroyRenderObject(stripModel);
    DestroyRenderObject((void *)fadeModel);
    if (g_SnoboModeFlag == 1)
        for (index = 0; index < 8; index++)
            DestroyRenderObject(backgroundModels[index]);
    DestroyRenderObject(time1Model);
    DestroyRenderObject(time2Model);
    DestroyRenderObject(g_SnoboEstamp0Model);
    DestroyRenderObject(g_SnoboEstamp1Model);
}

/*
 * Zeroes model-load options, initializes the requested blend and snowboard
 * resource path, sets fixed option fields and incorporates optional flags.
 * Creates a render object from the supplied type/name and engine resource,
 * sets flag two in the object's emitterFlags when successful and returns its pointer or
 * zero.
 */
// FUNCTION: FF7 0x0072cfa3
int SnowboardRenderer::buildModel(int type, int blendMode, int name, int flags)
{
    struct {
        PfileLoadOptions options;
        GfxEngineContext *context;
        GfxRenderObject *result;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.context = GetEngineContext();
    frame.result = 0;
    memset(&frame.options, 0, sizeof(frame.options));
    Pfile_InitLoadOptionsWithBlend(blendMode, &frame.options);
    frame.options.directory = GetMinigameDirForSnowboard();
    if (blendMode != BLEND_NONE)
        Pfile_SetLoadOptionsBlendMode(blendMode, &frame.options);
    frame.options.sourceMode = 0;
    frame.options.file.fromArchive = 1;
    frame.options.file.archiveId = 10;
    frame.options.file.callback = 0;
    if (flags)
        frame.options.renderFlags |= flags;
    frame.result = (GfxRenderObject *)Gfx_CreateRenderObject(1, type, &frame.options, (char *)name, (void *)frame.context->renderObjectBank);
    if (frame.result)
        frame.result->emitterFlags |= 2;
    return (int)frame.result;
}
extern "C" {

// Captures the clip origin and viewport size of the engine context. It stores clipLeft and clipTop
// in g_SnoboClipLeft and g_SnoboClipTop, and stores the integer ratios 640 / viewSizeX and 480 /
// viewSizeY in g_SnoboViewportScaleX and g_SnoboViewportScaleY. The self argument is unused.
// FUNCTION: FF7 0x0072d066
void __fastcall Mini_SnowboardCaptureViewportScale(struct SnowboardRenderer *self)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    g_SnoboClipLeft = ctx->clipLeft;
    g_SnoboClipTop = ctx->clipTop;
    g_SnoboViewportScaleX = 0x280 / ctx->viewSizeX;
    g_SnoboViewportScaleY = 0x1e0 / ctx->viewSizeY;
}
} // extern "C"

/*
 * For active rider updates, accumulates elapsed time from timeGetTime when
 * timing is enabled and caps it at 0x5B8D7F. Updates displayed speed on odd
 * frames, always renders time/speed and emits HUD batches.
 */
// FUNCTION: FF7 0x0072d0c0
void SnowboardHud::updateHud()
{
    if (g_SnoboStatePtr->running) {
        if (g_SnoboStatePtr->triggerLatched) {
            g_SnoboStatePtr->nowMs = timeGetTime();
            if (g_SnoboStatePtr->prevNowMs == 0)
                g_SnoboStatePtr->prevNowMs = g_SnoboStatePtr->nowMs;
            g_SnoboStatePtr->courseTime += g_SnoboStatePtr->nowMs - g_SnoboStatePtr->prevNowMs;
            if ((unsigned int)g_SnoboStatePtr->courseTime > 0x5b8d7f)
                g_SnoboStatePtr->courseTime = 0x5b8d7f;
            g_SnoboStatePtr->prevNowMs = g_SnoboStatePtr->nowMs;
        }
        if (g_SnoboStatePtr->tick & 1)
            g_SnoboStatePtr->displayedSpeed = g_SnoboStatePtr->followAngle * 150 / 2048;
    }
    renderTime(g_SnoboStatePtr->courseTime, 0x10, 0x30);
    renderSpeed(g_SnoboStatePtr->displayedSpeed, 0x207, 0x30);
    ((SnowboardRenderer *)this)->emitHud();
    if (g_SnoboStatePtr->paused) {
        g_SnoboStatePtr->prevNowMs = timeGetTime();
        if (g_SnoboModeFlag == 1)
            Mini_SnowboardDrawPauseOverlay((struct SnowboardRenderer *)this);
    }
}

/*
 * On the rendering frame, splits time and draws the time label plus packed
 * time digits for alternate/race modes or the fixed placeholder otherwise. In
 * normal mode also draws the selected course record or its absent-record form.
 */
// FUNCTION: FF7 0x0072d207
void SnowboardHud::renderTime(unsigned int ticks, short x, short y)
{
    int color = -1;
    if (g_SnoboStatePtr->renderFrame == 1) {
        ((SnowboardRenderer *)this)->splitTime((int)&ticks, (int)g_SnoboCourseTimeDigits);
        ((SnowboardRenderer *)this)->drawText(g_SnoboTimeLabel, x, y, color, this->fontModel);
        if (g_SnoboModeFlag == 1 || g_SnoboStatePtr->triggerStage) {
            ((SnowboardRenderer *)this)->packDigits((int)g_SnoboTimeStr, (int)g_SnoboCourseTimeDigits, 0x10);
            ((SnowboardRenderer *)this)->drawText(g_SnoboTimeStr, x + 0x48, y, color, this->fontModel);
        } else
            ((SnowboardRenderer *)this)->drawText(g_SnoboScoreStr, x + 0x48, y, color, this->fontModel);
        if (g_SnoboModeFlag == 0) {
            if (g_SnoboStatePtr->triggerStage)
                renderRecord((int)(g_SnoboStatePtr->bestTimes + (g_SnoboStatePtr->triggerStage - 1)), x, y + 0x1e);
            else
                renderRecord(0, x, y + 0x1e);
        }
    }
}

/*
 * Draws g_SnoboBestLabel at x/y with color -1 and fontModel. For nonnull
 * ticks with a nonzero first dword, splits/encodes the time and draws it at
 * x+0x48; otherwise draws g_SnoboNoRecordStr there.
 */
// FUNCTION: FF7 0x0072d333
void SnowboardHud::renderRecord(int ticks, short x, int y)
{
    int color = -1;
    ((SnowboardRenderer *)this)->drawText(g_SnoboBestLabel, x, (short)y, color, this->fontModel);
    if (ticks && *(int *)ticks) {
        ((SnowboardRenderer *)this)->splitTime(ticks, (int)g_SnoboRecordTimeDigits);
        ((SnowboardRenderer *)this)->packDigits((int)g_SnoboRecordTimeStr, (int)g_SnoboRecordTimeDigits, 0x30);
        ((SnowboardRenderer *)this)->drawText(g_SnoboRecordTimeStr, x + 0x48, (short)y, color, this->fontModel);
    } else
        ((SnowboardRenderer *)this)->drawText(g_SnoboNoRecordStr, x + 0x48, (short)y, color, this->fontModel);
}

/*
 * Packs seven digit values into the encoded-text buffer; the receiver is
 * unused.
 */
// FUNCTION: FF7 0x0072d3ea
void SnowboardRenderer::packDigits(int buffer, int digits, char base)
{
    ((SnowboardDigitBuffer *)buffer)->digit0 = ((SnowboardDigitSet *)digits)->digit0 + base;
    ((SnowboardDigitBuffer *)buffer)->digit1 = ((SnowboardDigitSet *)digits)->digit1 + base;
    ((SnowboardDigitBuffer *)buffer)->digit2 = ((SnowboardDigitSet *)digits)->digit2 + base;
    ((SnowboardDigitBuffer *)buffer)->digit3 = ((SnowboardDigitSet *)digits)->digit3 + base;
    ((SnowboardDigitBuffer *)buffer)->digit4 = ((SnowboardDigitSet *)digits)->digit4 + base;
    ((SnowboardDigitBuffer *)buffer)->digit5 = ((SnowboardDigitSet *)digits)->digit5 + base;
    if (g_SnoboModeFlag == 0) {
        ((SnowboardDigitBuffer *)buffer)->glyphIndex = ((SnowboardDigitSet *)digits)->glyphIndex + base;
    } else {
        ((SnowboardDigitBuffer *)buffer)->glyphIndex = 0;
    }
}

/*
 * Splits unsigned milliseconds into two minute, two second, and three fraction
 * digits.
 */
// FUNCTION: FF7 0x0072d485
void SnowboardRenderer::splitTime(int timeMs, int digits)
{
    unsigned int fraction;
    unsigned int sum;
    unsigned int seconds;

    sum = *(unsigned int *)timeMs / 60000;
    *(unsigned int *)digits = sum / 10;
    ((SnowboardTimeDigits *)digits)->minuteUnits = sum - *(unsigned int *)digits * 10;
    seconds = (*(unsigned int *)timeMs - sum * 1000 * 60) / 1000;
    ((SnowboardTimeDigits *)digits)->secondTens = seconds / 10;
    ((SnowboardTimeDigits *)digits)->secondUnits = seconds - ((SnowboardTimeDigits *)digits)->secondTens * 10;
    fraction = *(unsigned int *)timeMs - sum * 1000 * 60 - seconds * 1000;
    ((SnowboardTimeDigits *)digits)->fractionHundreds = fraction / 100;
    ((SnowboardTimeDigits *)digits)->fractionTens = (fraction - ((SnowboardTimeDigits *)digits)->fractionHundreds * 100) / 10;
    ((SnowboardTimeDigits *)digits)->fractionUnits = fraction - ((SnowboardTimeDigits *)digits)->fractionHundreds * 100 - ((SnowboardTimeDigits *)digits)->fractionTens * 10;
}

/*
 * Derives unsigned-short hundreds/tens/ones, modifies the initialized speed
 * text while suppressing leading positions and draws it at X minus 0x70 only
 * on the rendering frame. Uses the retained template and encoded digit offset
 * 0x10.
 */
// FUNCTION: FF7 0x0072d57d
void SnowboardHud::renderSpeed(short value, short x, short y)
{
    struct {
        int tens, hundreds, color, ones;
        union {
            char buffer[8];
            struct {
                int head;
                int tail;
            } words;
        };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.words.head = g_SnoboSpeedTemplateHead;
    frame.words.tail = g_SnoboSpeedTemplateTail;
    frame.color = -1;
    frame.hundreds = (unsigned short)value / 100;
    frame.tens = ((unsigned short)value - frame.hundreds * 100) / 10;
    frame.ones = (unsigned short)value - frame.hundreds * 100 - frame.tens * 10;
    if (g_SnoboStatePtr->renderFrame == 1) {
        if (frame.hundreds)
            frame.buffer[2] = frame.hundreds + 0x10;
        if (frame.hundreds || frame.tens)
            frame.buffer[3] = frame.tens + 0x10;
        frame.buffer[4] = frame.ones + 0x10;
        ((SnowboardRenderer *)this)->drawText(frame.buffer, x - 0x70, y, frame.color, this->fontModel);
    }
}
extern "C" {

/*
 * Forwards the fastcall self receiver to SnowboardRenderer::emitOverlay.
 */
// FUNCTION: FF7 0x0072d64e
void __fastcall Mini_SnowboardEmitOverlay(int self)
{
    ((SnowboardRenderer *)self)->emitOverlay();
}

/*
 * Alternate overlay helper; acts only on the rendering frame. Gets the overlay
 * color, draws the local two-byte text template at 0x10C/0xE5 through the
 * renderer helper, adds a black full-screen panel with alpha 0x80 and emits
 * overlay batches.
 */
// FUNCTION: FF7 0x0072d661
void __fastcall Mini_SnowboardDrawPauseOverlay(SnowboardRenderer *self)
{
    short glyph;
    int color;

    glyph = g_SnoboPauseGlyph;
    if (g_SnoboStatePtr->renderFrame == 1) {
        color = Mini_SnowboardGetPulseGrayColor(self);
        self->drawText((char *)&glyph, 0x10C, 0xE5, color, self->menuFontModel);
        self->fadeBlack(0x80);
        self->emitOverlay();
    }
}
} // extern "C"

/*
 * Selects a list descriptor in g_SnoboHudMenus. When the race
 * state's renderFrame equals 1, draws its entries at 32-pixel vertical intervals, colors the
 * selected entry with Mini_SnowboardGetPulseGrayColor, draws
 * g_SnoboMenuCursorGlyph as its marker and draws the descriptor rectangle.
 */
// FUNCTION: FF7 0x0072d6c8
void SnowboardHud::selectList(short list, short selected)
{
    struct {
        int color, index, y, x;
        SnowboardHudMenuDescriptor *descriptor;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.descriptor = g_SnoboHudMenus + list;
    frame.x = frame.descriptor->x;
    frame.y = frame.descriptor->y;
    if (g_SnoboStatePtr->renderFrame == 1) {
        frame.color = Mini_SnowboardGetPulseGrayColor(this);
        for (frame.index = 0; frame.index < frame.descriptor->count; frame.index++, frame.y += 32) {
            ((SnowboardRenderer *)this)->drawText(frame.descriptor->items[frame.index], frame.x, frame.y, frame.index == selected ? frame.color : -1, this->menuFontModel);
        }
        ((SnowboardRenderer *)this)->drawText(g_SnoboMenuCursorGlyph, frame.x - 18, (selected << 5) + frame.descriptor->y, frame.color, this->menuFontModel);
        ((SnowboardRenderer *)this)->drawRect(frame.descriptor->rectX, frame.descriptor->rectY, frame.descriptor->width, frame.descriptor->height);
    }
}
extern "C" {

/*
 * Forwards a 16-bit value to object slot zero.
 */
// FUNCTION: FF7 0x0072d7d8
void SnowboardRenderer::select0(short value)
{
    ((SnowboardHud *)this)->selectList(0, value);
}

/*
 * Forwards a 16-bit value to object slot one.
 */
// FUNCTION: FF7 0x0072d7f4
void SnowboardRenderer::select1(short value)
{
    ((SnowboardHud *)this)->selectList(1, value);
}

/*
 * Forwards a 16-bit value to object slot two.
 */
// FUNCTION: FF7 0x0072d810
void SnowboardRenderer::select2(short value)
{
    ((SnowboardHud *)this)->selectList(2, value);
}

/*
 * Forwards a 16-bit value to object slot three.
 */
// FUNCTION: FF7 0x0072d82c
void SnowboardRenderer::select3(short value)
{
    ((SnowboardHud *)this)->selectList(3, value);
}

/*
 * Forwards a 16-bit value to object slot four.
 */
// FUNCTION: FF7 0x0072d848
void SnowboardRenderer::select4(short value)
{
    ((SnowboardHud *)this)->selectList(4, value);
}

/*
 * Forwards a 16-bit value to object slot five.
 */
// FUNCTION: FF7 0x0072d864
void SnowboardRenderer::selectMenu(short selection)
{
    ((SnowboardHud *)this)->selectList(5, selection);
}
} // extern "C"

/*
 * Acquires a rectangle batch and writes four vertices for the supplied bounds
 * using integer division by display-scale fields before float conversion and
 * offsets. Uses white packed color, fixed depth/W and full UV range, then
 * draws an inset black panel with alpha 0x7F.
 */
// FUNCTION: FF7 0x0072d880
void SnowboardRenderer::drawRect(int x, int y, int w, int h)
{
    struct {
        GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        unsigned char color[4];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    AcquireBatchNode(1, (void *)wakuModel);
    local.vertTL = (GfxTlVertex *)wakuModel->vertexCursor;
    local.vertTL->x = (float)((x) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    local.vertTL->y = (float)((y) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    local.vertTL->z = 0.0f;
    local.vertTL->rhw = 1.0f;
    local.vertTL->color = -1;
    local.vertTL->specular = 0xFF000000;
    local.vertTL->u = 0.0f;
    local.vertTL->v = 0.0f;
    local.vertBL = (GfxTlVertex *)wakuModel->vertexCursor + 1;
    local.vertBL->x = (float)((x) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    local.vertBL->y = (float)((y + h) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    local.vertBL->z = 0.0f;
    local.vertBL->rhw = 1.0f;
    local.vertBL->color = -1;
    local.vertBL->specular = 0xFF000000;
    local.vertBL->u = 0.0f;
    local.vertBL->v = 1.0f;
    local.vertTR = (GfxTlVertex *)wakuModel->vertexCursor + 2;
    local.vertTR->x = (float)((x + w) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    local.vertTR->y = (float)((y) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    local.vertTR->z = 0.0f;
    local.vertTR->rhw = 1.0f;
    local.vertTR->color = -1;
    local.vertTR->specular = 0xFF000000;
    local.vertTR->u = 1.0f;
    local.vertTR->v = 0.0f;
    local.vertBR = (GfxTlVertex *)wakuModel->vertexCursor + 3;
    local.vertBR->x = (float)((x + w) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    local.vertBR->y = (float)((y + h) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    local.vertBR->z = 0.0f;
    local.vertBR->rhw = 1.0f;
    local.vertBR->color = -1;
    local.vertBR->specular = 0xFF000000;
    local.vertBR->u = 1.0f;
    local.vertBR->v = 1.0f;
    local.color[0] = 0;
    local.color[1] = 0;
    local.color[2] = 0;
    local.color[3] = 0x7F;
    panel(local.color, x + 4, y + 2, w - 8, h - 4, rectPanelModel);
}

/*
 * When the race state's renderFrame equals 1, converts two camera floats from radians to
 * narrowed 12-bit angle units and derives a vertical background position. In
 * g_SnoboModeFlag mode 1 builds consecutive 64-wide textured primitives across X
 * from a roll-derived starting tile, wrapping the byte texture coordinate and
 * cycling four texture indices.
 */
// FUNCTION: FF7 0x0072daf0
void SnowboardRenderer::buildCourse()
{
    struct {
        int temp, count;
        SnowboardCourseDesc *desc;
        unsigned char tex;
        unsigned char pad0[3];
        short idx;
        short pad1;
        short posX;
        short pad2;
        int posY;
        unsigned int prim[9];
        short roll;
        short pad3;
        short yaw;
        short pad4;
        unsigned int *primPtr;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_SnoboStatePtr->renderFrame == 1) {
        frame.primPtr = frame.prim;
        frame.yaw = (short)(int)(g_SnoboCameraPos[0] * 4096.0f / 6.28318530718);
        frame.roll = (short)(int)(g_SnoboCameraPos[1] * 4096.0f / 6.28318530718);
        frame.posX = frame.yaw;
        if (frame.posX >= 2048)
            frame.posX |= 0xF000;
        frame.posY = -(frame.posX >> 2) - 64;
        if (g_SnoboModeFlag == 1) {
            frame.posX = (1024 - (frame.roll >> 2)) & 1023;
            frame.tex = frame.posX & 0xC0;
            frame.idx = frame.posX >> 8;
            frame.posX = -((frame.posX & 63) + 160);
            while (0, 1) {
                frame.primPtr[1] = 0x65000000;
                frame.primPtr[2] = (frame.posY << 16) | (unsigned short)frame.posX;
                frame.primPtr[3] = frame.tex;
                frame.primPtr[4] = 0x800040;
                prim((SnowboardScreenQuadSrc *)frame.primPtr, frame.idx);
                frame.posX += 64;
                if (frame.posX >= 160)
                    break;
                frame.tex += 64;
                if (frame.tex == 0)
                    frame.idx = (frame.idx + 1) & 3;
            }
        }
        frame.posY += 96;
        if (g_SnoboModeFlag == 0) {
            frame.count = 3;
            frame.desc = g_SnoboHudShorts;
        } else {
            frame.count = 2;
            frame.desc = g_SnoboHudShorts + 1;
        }
        for (; frame.count--; frame.desc++) {
            frame.primPtr[3] = frame.desc->topColor;
            frame.primPtr[1] = frame.primPtr[3];
            frame.primPtr[7] = frame.desc->bottomColor;
            frame.primPtr[5] = frame.primPtr[7];
            frame.temp = (frame.posY + frame.desc->top) << 16;
            frame.primPtr[2] = frame.temp | 0xFF60;
            frame.primPtr[4] = frame.temp | 0xA0;
            frame.temp += frame.desc->height << 16;
            frame.primPtr[6] = frame.temp | 0xFF60;
            frame.primPtr[8] = frame.temp | 0xA0;
            strip((SnowboardStripVertexSrc *)frame.primPtr);
        }
        emitCourse();
    }
}
extern "C" {

/*
 * Draws a 640-by-480 panel at origin through the current renderer resource,
 * with RGB bytes 0xFF and the given alpha.
 */
// FUNCTION: FF7 0x0072dd53
void SnowboardRenderer::fadeWhite(unsigned char alpha)
{
    unsigned char color[4];

    color[0] = 0xFF;
    color[1] = 0xFF;
    color[2] = 0xFF;
    color[3] = alpha;
    panel(color, 0, 0, 0x280, 0x1E0, fadeModel);
}

/*
 * Draws a 640-by-480 panel at origin through the current renderer resource,
 * with RGB bytes zero and the given alpha.
 */
// FUNCTION: FF7 0x0072dd94
void SnowboardRenderer::fadeBlack(unsigned char alpha)
{
    unsigned char color[4];

    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    color[3] = alpha;
    panel(color, 0, 0, 0x280, 0x1E0, fadeModel);
}

/*
 * Draws a 640-by-480 panel at origin through the current renderer resource,
 * using the supplied packed color with its high alpha byte forced to 0xFF.
 */
// FUNCTION: FF7 0x0072ddd5
void SnowboardRenderer::fillColor(int rgb)
{
    unsigned char color[4];

    *(int *)color = rgb;
    color[3] = 0xFF;
    panel(color, 0, 0, 0x280, 0x1E0, fadeModel);
}
} // extern "C"

/*
 * On the rendering frame, acquires one batch in the supplied resource and
 * writes four colored rectangle vertices. Coordinates use integer division by
 * display-scale fields before float conversion plus offsets; depth zero, W
 * one, secondary packed color 0xFF 000000 and UVs span zero through one.
 */
// FUNCTION: FF7 0x0072de0e
void SnowboardRenderer::panel(unsigned char *color, int x, int y, int w, int h, int render)
{
    GfxTlVertex *topRemaining, *bottomLeft, *cornerTR, *bottomRight;
    if (g_SnoboStatePtr->renderFrame == 1) {
        AcquireBatchNode(1, (void *)render);
        topRemaining = (GfxTlVertex *)((GfxRenderObject *)render)->vertexCursor;
        topRemaining->x = (float)((x) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        topRemaining->y = (float)((y) / g_SnoboViewportScaleY) + g_SnoboClipTop;
        topRemaining->z = 0.0f;
        topRemaining->rhw = 1.0f;
        topRemaining->color = *(int *)color;
        topRemaining->specular = 0xFF000000;
        topRemaining->u = 0.0f;
        topRemaining->v = 0.0f;
        bottomLeft = (GfxTlVertex *)((GfxRenderObject *)render)->vertexCursor + 1;
        bottomLeft->x = (float)((x) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        bottomLeft->y = (float)((y + h) / g_SnoboViewportScaleY) + g_SnoboClipTop;
        bottomLeft->z = 0.0f;
        bottomLeft->rhw = 1.0f;
        bottomLeft->color = *(int *)color;
        bottomLeft->specular = 0xFF000000;
        bottomLeft->u = 0.0f;
        bottomLeft->v = 1.0f;
        cornerTR = (GfxTlVertex *)((GfxRenderObject *)render)->vertexCursor + 2;
        cornerTR->x = (float)((x + w) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        cornerTR->y = (float)((y) / g_SnoboViewportScaleY) + g_SnoboClipTop;
        cornerTR->z = 0.0f;
        cornerTR->rhw = 1.0f;
        cornerTR->color = *(int *)color;
        cornerTR->specular = 0xFF000000;
        cornerTR->u = 1.0f;
        cornerTR->v = 0.0f;
        bottomRight = (GfxTlVertex *)((GfxRenderObject *)render)->vertexCursor + 3;
        bottomRight->x = (float)((x + w) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        bottomRight->y = (float)((y + h) / g_SnoboViewportScaleY) + g_SnoboClipTop;
        bottomRight->z = 0.0f;
        bottomRight->rhw = 1.0f;
        bottomRight->color = *(int *)color;
        bottomRight->specular = 0xFF000000;
        bottomRight->u = 1.0f;
        bottomRight->v = 1.0f;
    }
}

// After AcquireBatchNode(1, node) it fills the four transformed vertices at the vertexCursor of
// backgroundModels[idx], with idx = quad->u/128 + page*2, as a screen quad. The left edge is
// (x*2 + 0x140)/g_SnoboViewportScaleX + g_SnoboClipLeft and the top is (y*2 +
// 0xF0)/g_SnoboViewportScaleY + g_SnoboClipTop; the right and bottom edges add the doubled w and h.
// z is 0.9999, rhw 1, colour -1, specular 0xFF000000, and the uv pairs come from the first eight
// floats of g_SnoboHudFloats (reached as the end of g_SnoboHudMenus, which it follows), or from
// g_SnoboHudFloats[8..15] when quad->u/64 is odd.
// FUNCTION: FF7 0x0072e046
void SnowboardRenderer::prim(SnowboardScreenQuadSrc *quad, int page)
{
    int width;
    int vertColor;
    int nodeIndex;
    int leftEdge;
    int pageSelect;
    int topEdge;
    int quadH;
    GfxTlVertex *topLeftVtxPtr;
    float *uvSource;
    GfxTlVertex *pBottomRight;
    GfxTlVertex *topRightVtxPtr;
    GfxTlVertex *bottomLeftPtr;

    uvSource = (float *)(g_SnoboHudMenus + 6);
    vertColor = -1;
    nodeIndex = quad->u / 128 + page * 2;
    pageSelect = quad->u / 64;
    if ((pageSelect & 1) != 0) {
        uvSource = &g_SnoboHudFloats[8];
    }
    AcquireBatchNode(1, (void *)this->backgroundModels[nodeIndex]);
    leftEdge = quad->x * 2 + 0x140;
    topEdge = quad->y * 2 + 0xF0;
    width = quad->w * 2;
    quadH = quad->h * 2;
    topLeftVtxPtr = (GfxTlVertex *)this->backgroundModels[nodeIndex]->vertexCursor;
    topLeftVtxPtr->x = (float)(leftEdge / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    topLeftVtxPtr->y = (float)(topEdge / g_SnoboViewportScaleY) + g_SnoboClipTop;
    topLeftVtxPtr->z = 0.9999f;
    topLeftVtxPtr->rhw = 1.0f;
    topLeftVtxPtr->color = vertColor;
    topLeftVtxPtr->specular = 0xFF000000;
    topLeftVtxPtr->u = uvSource[0];
    topLeftVtxPtr->v = uvSource[1];
    bottomLeftPtr = (GfxTlVertex *)this->backgroundModels[nodeIndex]->vertexCursor + 1;
    bottomLeftPtr->x = (float)(leftEdge / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    bottomLeftPtr->y = (float)((topEdge + quadH) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    bottomLeftPtr->z = 0.9999f;
    bottomLeftPtr->rhw = 1.0f;
    bottomLeftPtr->color = vertColor;
    bottomLeftPtr->specular = 0xFF000000;
    bottomLeftPtr->u = uvSource[2];
    bottomLeftPtr->v = uvSource[3];
    topRightVtxPtr = (GfxTlVertex *)this->backgroundModels[nodeIndex]->vertexCursor + 2;
    topRightVtxPtr->x = (float)((leftEdge + width) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    topRightVtxPtr->y = (float)(topEdge / g_SnoboViewportScaleY) + g_SnoboClipTop;
    topRightVtxPtr->z = 0.9999f;
    topRightVtxPtr->rhw = 1.0f;
    topRightVtxPtr->color = vertColor;
    topRightVtxPtr->specular = 0xFF000000;
    topRightVtxPtr->u = uvSource[4];
    topRightVtxPtr->v = uvSource[5];
    pBottomRight = (GfxTlVertex *)this->backgroundModels[nodeIndex]->vertexCursor + 3;
    pBottomRight->x = (float)((leftEdge + width) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    pBottomRight->y = (float)((topEdge + quadH) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    pBottomRight->z = 0.9999f;
    pBottomRight->rhw = 1.0f;
    pBottomRight->color = vertColor;
    pBottomRight->specular = 0xFF000000;
    pBottomRight->u = uvSource[6];
    pBottomRight->v = uvSource[7];
}

// After AcquireBatchNode(1, stripModel) it fills the four transformed vertices at the vertexCursor
// of that node as a full-width 640-pixel band. The left pair has x = g_SnoboClipLeft, and the right
// pair has x = 0x280/g_SnoboViewportScaleX + g_SnoboClipLeft. Each y is (y*2 +
// 0xF0)/g_SnoboViewportScaleY + g_SnoboClipTop, with corners 0 and 2 on the left and 1 and 3 on the
// right. z is 0.99999, rhw 1, specular 0xFF000000, and uv zero. Each colour is the corner's RGB
// with alpha 0xFF.
// FUNCTION: FF7 0x0072e31f
void SnowboardRenderer::strip(SnowboardStripVertexSrc *band)
{
    GfxTlVertex *pLeftTop;
    GfxTlVertex *leftBottom;
    GfxTlVertex *rightTop;
    GfxTlVertex *rightBottomPtr;

    AcquireBatchNode(1, (void *)this->stripModel);
    pLeftTop = (GfxTlVertex *)this->stripModel->vertexCursor;
    pLeftTop->x = (float)g_SnoboClipLeft + 0.0f;
    pLeftTop->y = (float)((band->y0 * 2 + 0xF0) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    pLeftTop->z = 0.99999f;
    pLeftTop->rhw = 1.0f;
    pLeftTop->color = (band->r0 << 16 | 0xFF000000) | band->g0 << 8 | band->b0;
    pLeftTop->specular = 0xFF000000;
    pLeftTop->u = 0;
    pLeftTop->v = 0;
    leftBottom = (GfxTlVertex *)this->stripModel->vertexCursor + 1;
    leftBottom->x = (float)g_SnoboClipLeft + 0.0f;
    leftBottom->y = (float)((band->y2 * 2 + 0xF0) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    leftBottom->z = 0.99999f;
    leftBottom->rhw = 1.0f;
    leftBottom->color = (band->r2 << 16 | 0xFF000000) | band->g2 << 8 | band->b2;
    leftBottom->specular = 0xFF000000;
    leftBottom->u = 0;
    leftBottom->v = 0;
    rightTop = (GfxTlVertex *)this->stripModel->vertexCursor + 2;
    rightTop->x = (float)(0x280 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    rightTop->y = (float)((band->y1 * 2 + 0xF0) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    rightTop->z = 0.99999f;
    rightTop->rhw = 1.0f;
    rightTop->color = (band->r1 << 16 | 0xFF000000) | band->g1 << 8 | band->b1;
    rightTop->specular = 0xFF000000;
    rightTop->u = 0;
    rightTop->v = 0;
    rightBottomPtr = (GfxTlVertex *)this->stripModel->vertexCursor + 3;
    rightBottomPtr->x = (float)(0x280 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
    rightBottomPtr->y = (float)((band->y3 * 2 + 0xF0) / g_SnoboViewportScaleY) + g_SnoboClipTop;
    rightBottomPtr->z = 0.99999f;
    rightBottomPtr->rhw = 1.0f;
    rightBottomPtr->color = (band->r3 << 16 | 0xFF000000) | band->g3 << 8 | band->b3;
    rightBottomPtr->specular = 0xFF000000;
    rightBottomPtr->u = 0;
    rightBottomPtr->v = 0;
}

extern "C" {

/*
 * Folds the low four bits of the race state's tick word into a triangular 0..7
 * phase and returns opaque grayscale 0xFFRRGGBB with each channel 0x8F plus
 * sixteen times that phase; self is unused.
 */
// FUNCTION: FF7 0x0072e5ec
int __fastcall Mini_SnowboardGetPulseGrayColor(void *self)
{
    int val;

    val = g_SnoboStatePtr->tick & 0xf;
    if (val >= 8) {
        val = 0xf - val;
    }
    val = (val << 4) + 0x8f;
    return 0xff000000 | (val << 16) | (val << 8) | val;
}
} // extern "C"

/*
 * Disables render state two, emits and invalidates four HUD batch resources in
 * order, switches scene pass to one and emits/invalidates two more. Restores
 * scene pass zero and render state two enabled.
 */
// FUNCTION: FF7 0x0072e63f
void SnowboardRenderer::emitHud()
{
    GfxEngineContext *context;
    context = GetEngineContext();
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)context);
    Gfx_EmitPrimitiveBatchList((void *)(fontModel), context);
    InvalidateBatchGeneration((void *)fontModel);
    Gfx_EmitPrimitiveBatchList(g_SnoboEstamp0Model, context);
    InvalidateBatchGeneration(g_SnoboEstamp0Model);
    Gfx_EmitPrimitiveBatchList(g_SnoboEstamp1Model, context);
    InvalidateBatchGeneration(g_SnoboEstamp1Model);
    Gfx_EmitPrimitiveBatchList(time1Model, context);
    InvalidateBatchGeneration((void *)time1Model);
    Gfx_SetScenePassCall(1, (struct GfxEngineContext *)context);
    Gfx_EmitPrimitiveBatchList(time2Model, context);
    InvalidateBatchGeneration((void *)time2Model);
    Gfx_EmitPrimitiveBatchList((void *)(fadeModel), context);
    InvalidateBatchGeneration((void *)fadeModel);
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)context);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, (struct GfxEngineContext *)context);
}

/*
 * Sets render state (2,0), emits and invalidates stripModel, additionally
 * emits/invalidates the eight backgroundModels when g_SnoboModeFlag==1, then
 * restores render state (2,1).
 */
// FUNCTION: FF7 0x0072e75a
void SnowboardRenderer::emitCourse()
{
    GfxEngineContext *context;
    int index;
    context = GetEngineContext();
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)context);
    Gfx_EmitPrimitiveBatchList(stripModel, context);
    InvalidateBatchGeneration((void *)stripModel);
    if (g_SnoboModeFlag == 1) {
        for (index = 0; index < 8; index++) {
            Gfx_EmitPrimitiveBatchList(backgroundModels[index], context);
            InvalidateBatchGeneration((void *)backgroundModels[index]);
        }
    }
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, (struct GfxEngineContext *)context);
}

/*
 * Disables render state two and selects scene pass one, emits/invalidates the
 * normal or alternate panel resource, then selects pass zero and
 * emits/invalidates two overlay resources. Restores render state two enabled.
 */
// FUNCTION: FF7 0x0072e7fe
void SnowboardRenderer::emitOverlay()
{
    GfxEngineContext *context;
    context = GetEngineContext();
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)context);
    Gfx_SetScenePassCall(1, (struct GfxEngineContext *)context);
    if (g_SnoboModeFlag == 0) {
        Gfx_EmitPrimitiveBatchList((void *)(rectPanelModel), context);
        InvalidateBatchGeneration((void *)rectPanelModel);
    } else {
        Gfx_EmitPrimitiveBatchList((void *)(fadeModel), context);
        InvalidateBatchGeneration((void *)fadeModel);
    }
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)context);
    Gfx_EmitPrimitiveBatchList(wakuModel, context);
    InvalidateBatchGeneration((void *)wakuModel);
    Gfx_EmitPrimitiveBatchList((void *)(menuFontModel), context);
    InvalidateBatchGeneration((void *)menuFontModel);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, (struct GfxEngineContext *)context);
}

/*
 * On the rendering frame emits a quad from 140/124 to 500/196 in time2Model,
 * using the helper color masked by 0xD0FFFFFF and fixed depth/W/secondary
 * color. Uses UV extents 0.703125 by 0.5625 and retains integer screen-scale
 * division.
 */
// FUNCTION: FF7 0x0072e8e0
void SnowboardRenderer::family0()
{
    struct {
        GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        int color;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_SnoboStatePtr->renderFrame == 1) {
        local.color = Mini_SnowboardGetPulseGrayColor(this) & 0xD0FFFFFF;
        AcquireBatchNode(1, (void *)time2Model);
        local.vertTL = (GfxTlVertex *)time2Model->vertexCursor;
        local.vertTL->x = (float)(140 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertTL->y = (float)(124 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertTL->z = 0.0f;
        local.vertTL->rhw = 1.0f;
        local.vertTL->color = local.color;
        local.vertTL->specular = 0xFF000000;
        local.vertTL->u = 0.0f;
        local.vertTL->v = 0.0f;
        local.vertBL = (GfxTlVertex *)time2Model->vertexCursor + 1;
        local.vertBL->x = (float)(140 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertBL->y = (float)(196 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertBL->z = 0.0f;
        local.vertBL->rhw = 1.0f;
        local.vertBL->color = local.color;
        local.vertBL->specular = 0xFF000000;
        local.vertBL->u = 0.0f;
        local.vertBL->v = 0.5625f;
        local.vertTR = (GfxTlVertex *)time2Model->vertexCursor + 2;
        local.vertTR->x = (float)(500 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertTR->y = (float)(124 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertTR->z = 0.0f;
        local.vertTR->rhw = 1.0f;
        local.vertTR->color = local.color;
        local.vertTR->specular = 0xFF000000;
        local.vertTR->u = 0.703125f;
        local.vertTR->v = 0.0f;
        local.vertBR = (GfxTlVertex *)time2Model->vertexCursor + 3;
        local.vertBR->x = (float)(500 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertBR->y = (float)(196 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertBR->z = 0.0f;
        local.vertBR->rhw = 1.0f;
        local.vertBR->color = local.color;
        local.vertBR->specular = 0xFF000000;
        local.vertBR->u = 0.703125f;
        local.vertBR->v = 0.5625f;
    }
}

/*
 * On the rendering frame emits a quad from 90/134 to 550/284 in time1Model,
 * using the helper color masked by 0xD0FFFFFF and fixed depth/W/secondary
 * color. Uses full U extent and V from 0.3951613 through one, retaining
 * integer screen-scale division.
 */
// FUNCTION: FF7 0x0072eb31
void SnowboardRenderer::family1()
{
    struct {
        GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        int color;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_SnoboStatePtr->renderFrame == 1) {
        local.color = Mini_SnowboardGetPulseGrayColor(this) & 0xD0FFFFFF;
        AcquireBatchNode(1, (void *)time1Model);
        local.vertTL = (GfxTlVertex *)time1Model->vertexCursor;
        local.vertTL->x = (float)(90 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertTL->y = (float)(134 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertTL->z = 0.0f;
        local.vertTL->rhw = 1.0f;
        local.vertTL->color = local.color;
        local.vertTL->specular = 0xFF000000;
        local.vertTL->u = 0.0f;
        local.vertTL->v = 0.39516130089759827f;
        local.vertBL = (GfxTlVertex *)time1Model->vertexCursor + 1;
        local.vertBL->x = (float)(90 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertBL->y = (float)(284 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertBL->z = 0.0f;
        local.vertBL->rhw = 1.0f;
        local.vertBL->color = local.color;
        local.vertBL->specular = 0xFF000000;
        local.vertBL->u = 0.0f;
        local.vertBL->v = 1.0f;
        local.vertTR = (GfxTlVertex *)time1Model->vertexCursor + 2;
        local.vertTR->x = (float)(550 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertTR->y = (float)(134 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertTR->z = 0.0f;
        local.vertTR->rhw = 1.0f;
        local.vertTR->color = local.color;
        local.vertTR->specular = 0xFF000000;
        local.vertTR->u = 1.0f;
        local.vertTR->v = 0.39516130089759827f;
        local.vertBR = (GfxTlVertex *)time1Model->vertexCursor + 3;
        local.vertBR->x = (float)(550 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertBR->y = (float)(284 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertBR->z = 0.0f;
        local.vertBR->rhw = 1.0f;
        local.vertBR->color = local.color;
        local.vertBR->specular = 0xFF000000;
        local.vertBR->u = 1.0f;
        local.vertBR->v = 1.0f;
    }
}

/*
 * On the rendering frame emits a quad from 132/128 to 508/226 in time1Model,
 * using the helper color masked by 0xD0FFFFFF and fixed depth/W/secondary
 * color. Uses UV extents 0.8173913 by 0.3951613 and retains integer screen-
 * scale division.
 */
// FUNCTION: FF7 0x0072ed82
void SnowboardRenderer::family2()
{
    struct {
        GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        int color;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_SnoboStatePtr->renderFrame == 1) {
        local.color = Mini_SnowboardGetPulseGrayColor(this) & 0xD0FFFFFF;
        AcquireBatchNode(1, (void *)time1Model);
        local.vertTL = (GfxTlVertex *)time1Model->vertexCursor;
        local.vertTL->x = (float)(132 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertTL->y = (float)(128 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertTL->z = 0.0f;
        local.vertTL->rhw = 1.0f;
        local.vertTL->color = local.color;
        local.vertTL->specular = 0xFF000000;
        local.vertTL->u = 0.0f;
        local.vertTL->v = 0.0f;
        local.vertBL = (GfxTlVertex *)time1Model->vertexCursor + 1;
        local.vertBL->x = (float)(132 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertBL->y = (float)(226 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertBL->z = 0.0f;
        local.vertBL->rhw = 1.0f;
        local.vertBL->color = local.color;
        local.vertBL->specular = 0xFF000000;
        local.vertBL->u = 0.0f;
        local.vertBL->v = 0.39516130089759827f;
        local.vertTR = (GfxTlVertex *)time1Model->vertexCursor + 2;
        local.vertTR->x = (float)(508 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertTR->y = (float)(128 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertTR->z = 0.0f;
        local.vertTR->rhw = 1.0f;
        local.vertTR->color = local.color;
        local.vertTR->specular = 0xFF000000;
        local.vertTR->u = 0.8173912763595581f;
        local.vertTR->v = 0.0f;
        local.vertBR = (GfxTlVertex *)time1Model->vertexCursor + 3;
        local.vertBR->x = (float)(508 / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertBR->y = (float)(226 / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertBR->z = 0.0f;
        local.vertBR->rhw = 1.0f;
        local.vertBR->color = local.color;
        local.vertBR->specular = 0xFF000000;
        local.vertBR->u = 0.8173912763595581f;
        local.vertBR->v = 0.39516130089759827f;
    }
}

/*
 * When the race state's renderFrame equals 1 and selection is less than 12,
 * selects its 36-byte descriptor in g_SnoboHudVertices, acquires a
 * batch node in the descriptor's model, and writes four 32-byte vertices
 * through that node's vertexCursor. Divides integer rectangle coordinates by the global
 * X/Y scale before adding screen offsets, sets depth 0, reciprocal W 1, white
 * diffuse and black specular, and copies the descriptor UV pairs.
 */
// FUNCTION: FF7 0x0072efd3
void SnowboardRenderer::renderStateMenu(int selection)
{
    struct {
        GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        SnowboardHudQuad *desc;
        int color;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    local.color = -1;
    if (g_SnoboStatePtr->renderFrame == 1 && selection < 12) {
        local.desc = g_SnoboHudVertices + selection;
        AcquireBatchNode(1, (void *)*local.desc->model);
        local.vertTL = (GfxTlVertex *)(*local.desc->model)->vertexCursor;
        local.vertTL->x = (float)((local.desc->x) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertTL->y = (float)((local.desc->y) / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertTL->z = 0.0f;
        local.vertTL->rhw = 1.0f;
        local.vertTL->color = local.color;
        local.vertTL->specular = 0xFF000000;
        local.vertTL->u = local.desc->u0;
        local.vertTL->v = local.desc->v0;
        local.vertBL = (GfxTlVertex *)(*local.desc->model)->vertexCursor + 1;
        local.vertBL->x = (float)((local.desc->x) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertBL->y = (float)((local.desc->y + local.desc->h) / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertBL->z = 0.0f;
        local.vertBL->rhw = 1.0f;
        local.vertBL->color = local.color;
        local.vertBL->specular = 0xFF000000;
        local.vertBL->u = local.desc->u0;
        local.vertBL->v = local.desc->v1;
        local.vertTR = (GfxTlVertex *)(*local.desc->model)->vertexCursor + 2;
        local.vertTR->x = (float)((local.desc->x + local.desc->w) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertTR->y = (float)((local.desc->y) / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertTR->z = 0.0f;
        local.vertTR->rhw = 1.0f;
        local.vertTR->color = local.color;
        local.vertTR->specular = 0xFF000000;
        local.vertTR->u = local.desc->u1;
        local.vertTR->v = local.desc->v0;
        local.vertBR = (GfxTlVertex *)(*local.desc->model)->vertexCursor + 3;
        local.vertBR->x = (float)((local.desc->x + local.desc->w) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
        local.vertBR->y = (float)((local.desc->y + local.desc->h) / g_SnoboViewportScaleY) + g_SnoboClipTop;
        local.vertBR->z = 0.0f;
        local.vertBR->rhw = 1.0f;
        local.vertBR->color = local.color;
        local.vertBR->specular = 0xFF000000;
        local.vertBR->u = local.desc->u1;
        local.vertBR->v = local.desc->v1;
    }
}

/*
 * When the race state's renderFrame equals 1, grows a centered rectangle from frame shifted
 * left by 4, capped above at 256. From frame 16 onward writes a visible-row
 * count capped at five and uses the second int of data as its loop cursor over the
 * result rows (enum SnowboardResultRow): technique, time, balloon, total and best.
 */
// FUNCTION: FF7 0x0072f26c
void SnowboardRenderer::showMenu(int data, int frame)
{
    int pulseColor;
    int rectGrowth;

    if (g_SnoboStatePtr->renderFrame == 1) {
        rectGrowth = frame << 4;
        if (rectGrowth > 0x100)
            rectGrowth = 0x100;
        if (rectGrowth != 0) {
            drawRect(0x140 - (rectGrowth >> 4) * 11, 0xC8 - (rectGrowth >> 4) * 6,
                     (rectGrowth >> 4) * 22, (rectGrowth >> 4) * 12);
        }
        if (frame >= 0x10) {
            ((int *)data)[0] = ((frame - 0x10) >> 4) + 1;
            if (((int *)data)[0] > 5)
                ((int *)data)[0] = 5;
            for (((int *)data)[1] = 0;
                 ((int *)data)[1] < ((int *)data)[0];
                 ((int *)data)[1]++) {
                switch (((int *)data)[1]) {
                case SNOWBOARD_RESULT_ROW_TECHNIQUE:
                    drawText(g_SnoboMenuTexts1[((int *)data)[1]], 0xAA, 0x7C, -1, menuFontModel);
                    drawNumber((unsigned short)g_SnoboStatePtr->resultPartA, 0x158, 0x7C, -1);
                    drawText(g_SnoboMenuTexts2[((int *)data)[1]], 0x168, 0x7C, -1, menuFontModel);
                    break;
                case SNOWBOARD_RESULT_ROW_TIME:
                    drawText(g_SnoboMenuTexts1[((int *)data)[1]], 0xAA, 0x9A, -1, menuFontModel);
                    drawNumber((unsigned short)g_SnoboStatePtr->resultPartB, 0x158, 0x9A, -1);
                    drawText(g_SnoboMenuTexts2[((int *)data)[1]], 0x168, 0x9A, -1, menuFontModel);
                    break;
                case SNOWBOARD_RESULT_ROW_BALLOON:
                    drawText(g_SnoboMenuTexts1[((int *)data)[1]], 0xAA, 0xB8, -1, menuFontModel);
                    drawNumber((unsigned short)g_SnoboStatePtr->resultPartC, 0x158, 0xB8, -1);
                    drawText(g_SnoboMenuTexts2[((int *)data)[1]], 0x168, 0xB8, -1, menuFontModel);
                    break;
                case SNOWBOARD_RESULT_ROW_TOTAL:
                    pulseColor = Mini_SnowboardGetPulseGrayColor(this);
                    drawText(g_SnoboMenuTexts1[((int *)data)[1]], 0xAA, 0xE0, pulseColor, menuFontModel);
                    drawNumber((unsigned short)g_SnoboStatePtr->resultTotal, 0x158, 0xE0, pulseColor);
                    drawText(g_SnoboMenuTexts2[((int *)data)[1]], 0x168, 0xE0, pulseColor, menuFontModel);
                    break;
                case SNOWBOARD_RESULT_ROW_BEST:
                    drawText(g_SnoboMenuTexts1[((int *)data)[1]], 0xAA, 0xFE, -1, menuFontModel);
                    drawNumber((unsigned short)g_SnoboStatePtr->bestScores[g_SnoboStatePtr->triggerStage - 1], 0x158, 0xFE, -1);
                    drawText(g_SnoboMenuTexts2[((int *)data)[1]], 0x168, 0xFE, -1, menuFontModel);
                    break;
                }
            }
        }
    }
}

/*
 * Formats the low sixteen bits of the supplied value into the initialized
 * three-position ASCII number template, suppressing leading hundreds/tens and
 * always writing units. Draws it at supplied X minus 28 using the selected
 * color and the renderer's menuFontModel; values above 999 retain the unbounded
 * hundreds-byte conversion.
 */
// FUNCTION: FF7 0x0072f5ce
void SnowboardRenderer::drawNumber(unsigned short number, short x, short y, int color)
{
    union {
        char bytes[4];
        int value;
    } text;
    int units;
    int hundredsCnt;
    int tens;
    text.value = g_SnoboRankText.numberTemplate;
    hundredsCnt = number / 100;
    tens = (number - hundredsCnt * 100) / 10;
    units = number - hundredsCnt * 100 - tens * 10;
    if (hundredsCnt != 0) {
        text.bytes[0] = hundredsCnt + 48;
    }
    if (hundredsCnt != 0 || tens != 0) {
        text.bytes[1] = tens + 48;
    }
    text.bytes[2] = units + 48;
    drawText(text.bytes, x - 28, y, color, menuFontModel);
}

/*
 * When the race state's renderFrame equals 1, grows a centered rectangle from frame shifted
 * left by 4, capped above at 256, and reveals up to five timed rows using
 * (frame-16)>>2. Updates the heading letter to 'A' + state, draws it once rows
 * are visible, and colors the selected phase with
 * Mini_SnowboardGetPulseGrayColor when phase is at most 4.
 */
// FUNCTION: FF7 0x0072f680
void SnowboardRenderer::renderRaceState(int state, int phase, int frame)
{
    struct {
        int index, box, count, colors[5];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    local.colors[0] = -1;
    local.colors[1] = -1;
    local.colors[2] = -1;
    local.colors[3] = -1;
    local.colors[4] = -1;
    if (g_SnoboStatePtr->renderFrame == 1) {
        local.box = frame << 4;
        if (local.box > 0x100)
            local.box = 0x100;
        if (local.box != 0)
            drawRect(0x140 - (local.box >> 4) * 8, 0xC8 - (local.box >> 4) * 6, (local.box >> 4) * 16, (local.box >> 4) * 12);
        local.count = (frame - 16) >> 2;
        if (local.count > 5)
            local.count = 5;
        if (local.count > 0) {
            g_SnoboCourseARecordStr[7] = (char)(state + 0x41);
            drawText(g_SnoboCourseARecordStr, 0xD8, 0x78, -1, menuFontModel);
            if (phase <= 4)
                local.colors[phase] = Mini_SnowboardGetPulseGrayColor(this);
            for (local.index = 0; local.index < local.count; local.index++) {
                /* Numeric leaderboard row index (0..4). */
                switch (local.index) {
                case 0:
                    drawText(g_SnoboTextPointers[local.index], 0xDC, 150, local.colors[0], menuFontModel);
                    drawTime((int)(g_SnoboStatePtr->rankings[state] + 0), 0x11C, 150, local.colors[0]);
                    break;
                case 1:
                    drawText(g_SnoboTextPointers[local.index], 0xDC, 178, local.colors[1], menuFontModel);
                    drawTime((int)(g_SnoboStatePtr->rankings[state] + 1), 0x11C, 178, local.colors[1]);
                    break;
                case 2:
                    drawText(g_SnoboTextPointers[local.index], 0xDC, 206, local.colors[2], menuFontModel);
                    drawTime((int)(g_SnoboStatePtr->rankings[state] + 2), 0x11C, 206, local.colors[2]);
                    break;
                case 3:
                    drawText(g_SnoboTextPointers[local.index], 0xDC, 234, local.colors[3], menuFontModel);
                    drawTime((int)(g_SnoboStatePtr->rankings[state] + 3), 0x11C, 234, local.colors[3]);
                    break;
                case 4:
                    drawText(g_SnoboTextPointers[local.index], 0xDC, 262, local.colors[4], menuFontModel);
                    drawTime((int)(g_SnoboStatePtr->rankings[state] + 4), 0x11C, 262, local.colors[4]);
                    break;
                }
            }
        }
    }
}
extern "C" {

/*
 * Calls splitTime (SnowboardRenderer::splitTime) with arg1 and a local output block. If the
 * first dword of arg1 is nonzero, calls SnowboardRenderer::packDigits with
 * g_SnoboRecordTimeStr, that block and 0x30, then passes
 * g_SnoboRecordTimeStr to SnowboardRenderer::drawText; otherwise passes
 * g_SnoboNoRecordStr directly.
 */
// FUNCTION: FF7 0x0072f96e
void SnowboardRenderer::drawTime(int source, short x, short y, int color)
{
    SnowboardTimeDigits *p;
    SnowboardTimeDigits buffer;

    p = &buffer;
    splitTime(source, (int)p);
    if (*(int *)source != 0) {
        packDigits((int)g_SnoboRecordTimeStr, (int)p, 0x30);
        drawText(g_SnoboRecordTimeStr, x, y, color, menuFontModel);
    } else {
        drawText(g_SnoboNoRecordStr, x, y, color, menuFontModel);
    }
}
} // extern "C"

// g_SnoboHudTable holds two DWORDs per character code from 16, the first packing u|v<<8, the
// second w|h<<16. drawText reads two of its coordinates through this 32-bit bit-field, which keeps
// the original's unsigned add.
struct SnowboardCoordinateRead {
    unsigned int value : 32;
};

/*
 * Draws text one glyph at a time as four 32-byte vertices. Space advances 14;
 * codes 40..43 are centred on the previous glyph's width and raised six
 * pixels, which reads the previous glyph record (uninitialized on the first
 * character) exactly as the original does.
 */
// FUNCTION: FF7 0x0072f9f4
void SnowboardRenderer::drawText(char *text, int x, int y, int color, int render)
{
    struct {
        GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        int drawY, drawX;
        unsigned int *glyph;
        float v1;
        unsigned char code;
        float u1, u0, v0;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (render == 0)
        return;
    while ((frame.code = *text++) != 0) {
        if (frame.code == 32) {
            x += 14;
            continue;
        }
        if (frame.code >= 40 && frame.code <= 43) {
            frame.drawX = x - ((frame.glyph[1] & 255) >> 1) - 4;
            frame.drawY = y - 6;
            frame.glyph = g_SnoboHudTable + ((frame.code - 16) * 2);
        } else {
            frame.drawX = x;
            frame.drawY = y;
            frame.glyph = g_SnoboHudTable + ((frame.code - 16) * 2);
            x += (frame.glyph[1] & 255) - 2;
        }
        if (AcquireBatchNode(1, (void *)render)) {
            frame.u0 = (float)(frame.glyph[0] & 255) / 256.0f;
            frame.u1 = (float)((frame.glyph[0] & 255) + (frame.glyph[1] & 65535)) / 256.0f;
            frame.v0 = (float)(frame.glyph[0] >> 8) / 256.0f;
            frame.v1 = (float)((frame.glyph[0] >> 8) + (frame.glyph[1] >> 16)) / 256.0f;
            frame.vertTL = (GfxTlVertex *)((GfxRenderObject *)render)->vertexCursor;
            frame.vertTL->x = (float)((unsigned int)(frame.drawX) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
            frame.vertTL->y = (float)((unsigned int)(frame.drawY) / g_SnoboViewportScaleY) + g_SnoboClipTop;
            frame.vertTL->z = 0.0f;
            frame.vertTL->rhw = 1.0f;
            frame.vertTL->color = color;
            frame.vertTL->specular = 0xFF000000;
            frame.vertTL->u = frame.u0;
            frame.vertTL->v = frame.v0;
            frame.vertBL = (GfxTlVertex *)((GfxRenderObject *)render)->vertexCursor + 1;
            frame.vertBL->x = (float)((unsigned int)(frame.drawX) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
            frame.vertBL->y = (float)((unsigned int)((frame.glyph[1] >> 16) + ((SnowboardCoordinateRead *)&frame.drawY)->value) / g_SnoboViewportScaleY) + g_SnoboClipTop;
            frame.vertBL->z = 0.0f;
            frame.vertBL->rhw = 1.0f;
            frame.vertBL->color = color;
            frame.vertBL->specular = 0xFF000000;
            frame.vertBL->u = frame.u0;
            frame.vertBL->v = frame.v1;
            frame.vertTR = (GfxTlVertex *)((GfxRenderObject *)render)->vertexCursor + 2;
            frame.vertTR->x = (float)((unsigned int)((frame.glyph[1] & 65535) + ((SnowboardCoordinateRead *)&frame.drawX)->value) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
            frame.vertTR->y = (float)((unsigned int)(frame.drawY) / g_SnoboViewportScaleY) + g_SnoboClipTop;
            frame.vertTR->z = 0.0f;
            frame.vertTR->rhw = 1.0f;
            frame.vertTR->color = color;
            frame.vertTR->specular = 0xFF000000;
            frame.vertTR->u = frame.u1;
            frame.vertTR->v = frame.v0;
            frame.vertBR = (GfxTlVertex *)((GfxRenderObject *)render)->vertexCursor + 3;
            frame.vertBR->x = (float)((unsigned int)(frame.drawX + (frame.glyph[1] & 65535)) / g_SnoboViewportScaleX) + g_SnoboClipLeft;
            frame.vertBR->y = (float)((unsigned int)(frame.drawY + (frame.glyph[1] >> 16)) / g_SnoboViewportScaleY) + g_SnoboClipTop;
            frame.vertBR->z = 0.0f;
            frame.vertBR->rhw = 1.0f;
            frame.vertBR->color = color;
            frame.vertBR->specular = 0xFF000000;
            frame.vertBR->u = frame.u1;
            frame.vertBR->v = frame.v1;
        }
    }
}

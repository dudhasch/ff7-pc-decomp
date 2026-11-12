// src/ff7/submarine/sub_hud_draw_and_update.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Draws the submarine HUD from its atlas quads, tint overlay, gauges, counters and
//   warning indicators, and updates the gauge records and torpedo and enemy audio.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/api/submarine.h"
#include "lib/api/graphics.h"
#include "ff7/api/main.h"
#include "lib/api/sound.h"
#include "ff7/types/submarine.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/enums/sound.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"
#include "ff7/enums/submarine.h"

extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(strcpy, strcat)

// GLOBAL: FF7 0x00e981d8
SubmarineHudQuad g_SubmarineHudPitchQuads[4] = {0};
// GLOBAL: FF7 0x00e98268
int g_SubmarineHudReadoutAtlasD = 0;
// GLOBAL: FF7 0x00e9826c
int g_SubmarineHudReadoutAtlas = 0;
// GLOBAL: FF7 0x00e98270
int g_SubmarineHudTextAtlas = 0;
unsigned char g_bssPad_00e98274[4] = {0};
// GLOBAL: FF7 0x00e98278
SubmarineHudQuad g_SubmarineHudGaugeQuads[5] = {0};
unsigned char g_bssPad_00e9832c[4] = {0};
// the submarine HUD's 36-byte atlas quad records 0xE98334.. and the short/int fields
// 0xE983E8..0xE9845C.
// GLOBAL: FF7 0x00e98330
struct SubmarineHudRenderState g_SubmarineHudRenderState = {0};
// GLOBAL: FF7 0x00e98460
struct SubmarineHudElements g_SubmarineHudElements = {0};
// GLOBAL: FF7 0x00e985b4
int g_SubmarineHudTextAtlasB = 0;
// GLOBAL: FF7 0x00e985b8
SubmarineHudQuad g_SubmarineHudValueQuads[6] = {0};
// the submarine HUD's 36-byte atlas quad records 0xE98690..0xE988B0.
// GLOBAL: FF7 0x00e98690
struct SubmarineHudFifteenQuads g_SubmarineHudFifteenElements = {0};
// GLOBAL: FF7 0x00e988b0
SubmarineHudQuad g_SubmarineHudRightFrameQuads[8] = {0};
// GLOBAL: FF7 0x00e989d0
int g_SubmarineHudBarAtlasD = 0;
unsigned char g_bssPad_00e989d4[4] = {0};
// the submarine HUD's 36-byte atlas quad records 0xE989D8..0xE98B1C.
// GLOBAL: FF7 0x00e989d8
SubmarineHudQuad g_SubmarineHudNineElements[9] = {0};
// GLOBAL: FF7 0x00e98b1c
int g_SubmarineHudTextAtlasD = 0;
// GLOBAL: FF7 0x00e98b20
int g_SubmarineHudBatchTexB = 0;
unsigned char g_bssPad_00e98b24[4] = {0};
// Submarine HUD Gouraud gauge rectangle (position, size, and vertex colors) passed to Gfx_EmitGouraudRect.
// GLOBAL: FF7 0x00e98b28
struct SubmarineHudGauge g_SubmarineHudGauge = {0};
// GLOBAL: FF7 0x00e98b50
SubmarineHudQuad g_SubmarineHudRectQuad = {0};
// GLOBAL: FF7 0x00e98b74
int g_SubmarineHudBarAtlasA = 0;
// GLOBAL: FF7 0x00e98b78
int g_SubmarineHudTextAtlasC = 0;
unsigned char g_bssPad_00e98b7c[4] = {0};
// the submarine HUD's 36-byte atlas quad records 0xE98B80..0xE98C10 (four records,
// Submarine_BuildHudElementTable).
// GLOBAL: FF7 0x00e98b80
SubmarineHudQuad g_SubmarineHudQuads[4] = {0};
// GLOBAL: FF7 0x00e98c10
int g_SubmarineHudSingleTextAtlasC = 0;
// GLOBAL: FF7 0x00e98c14
int g_SubmarineHudScreenTint = 0;
// GLOBAL: FF7 0x00e98c18
struct SubmarineHudLayout g_SubmarineHudLayout = {0};
unsigned char g_bssPad_00e98c94[4] = {0};
unsigned char g_bssPad_00e98c98[32] = {0};
// GLOBAL: FF7 0x00e98cb8
SubmarineHudQuad g_SubmarineHudDigitQuads[6] = {0};
// the submarine HUD's 36-byte atlas quad records 0xE98D90..0xE98FF4.
// GLOBAL: FF7 0x00e98d90
SubmarineHudQuad g_SubmarineHudSeventeenElements[17] = {0};
// GLOBAL: FF7 0x00e98ff4
int g_SubmarineHudBarAtlasC = 0;
// the submarine HUD's 36-byte atlas quad records 0xE98FF8..0xE99160.
// GLOBAL: FF7 0x00e98ff8
SubmarineHudQuad g_SubmarineHudScoreCounters[10] = {0};
// GLOBAL: FF7 0x00e99160
int g_SubmarineHudReadoutAtlasB = 0;
// GLOBAL: FF7 0x00e99164
int g_SubmarineHudSingleTextAtlasB = 0;
// GLOBAL: FF7 0x00e99168
int g_SubmarineHudTextAtlasA = 0;
// GLOBAL: FF7 0x00e9916c
int g_SubmarineHudBatchTexC = 0;
// GLOBAL: FF7 0x00e99170
int g_SubmarineHudReadoutAtlasA = 0;
unsigned char g_bssPad_00e99174[4] = {0};
// GLOBAL: FF7 0x00e99178
SubmarineHudQuad g_SubmarineHudCooldownQuads[8] = {0};
// the submarine HUD's 36-byte atlas quad records 0xE99298..0xE9944C (with the sprite record at
// 0xE99428).
// GLOBAL: FF7 0x00e99298
struct SubmarineHudTallyQuads g_SubmarineHudTallyElements = {0};
// GLOBAL: FF7 0x00e9944c
int g_SubmarineHudMainAtlas = 0;
// GLOBAL: FF7 0x00e99450
int g_SubmarineHudBarAtlas = 0;
// GLOBAL: FF7 0x00e99454
int g_SubmarineHudMainAtlasA = 0;
// GLOBAL: FF7 0x00e99458
int g_SubmarineHudRenderObjectA = 0;
unsigned char g_bssPad_00e9945c[4] = {0};
// the submarine HUD's 36-byte atlas quad records 0xE99460..0xE99518.
// GLOBAL: FF7 0x00e99460
struct SubmarineHudFiveQuads g_SubmarineHudFiveQuadRecords = {0};
// GLOBAL: FF7 0x00e99518
SubmarineHudQuad g_SubmarineHudYawStripQuads[3] = {0};
// GLOBAL: FF7 0x00e99584
int g_SubmarineHudRenderObjectB = 0;
// GLOBAL: FF7 0x00e99588
int g_SubmarineHudBatchTexD = 0;
unsigned char g_bssPad_00e9958c[4] = {0};
// the submarine HUD's 36-byte atlas quad records 0xE99590: six byte-coordinate pairs
// (Submarine_BuildHudElementTable, the short reads of Submarine_UpdateHudGauges (0x792216)).
// GLOBAL: FF7 0x00e99590
struct SubmarineHudAnimTable g_SubmarineHudAnimTable = {0};
// GLOBAL: FF7 0x00e9959c
int g_SubmarineHudRenderObjectD = 0;
// GLOBAL: FF7 0x00e995a0
SubmarineHudQuad g_SubmarineHudQuadTable[8] = {0};
// GLOBAL: FF7 0x00e996c0
int g_SubmarineHudRenderObjectE = 0;
// Active submarine HUD heading index read by Submarine_DrawHud and companion word cleared on module init.
// GLOBAL: FF7 0x00e996c4
int g_SubmarineHudHeadingBits = 0;
// GLOBAL: FF7 0x00e996c8
int g_SubmarineHudClearedWord = 0;
// GLOBAL: FF7 0x00e996cc
int g_SubmarineHudScrollPos = 0;
// GLOBAL: FF7 0x00e996d0
int g_SubmarineHudBarAnimFrame = 0;
// GLOBAL: FF7 0x00e996d4
int g_SubmarineHudBarAnimIndex = 0;
// GLOBAL: FF7 0x00e996d8
int g_SubmarineHudValueQuad5Timer = 0;
// GLOBAL: FF7 0x00e996dc
int g_SubmarineHudValueQuad1Timer = 0;
// GLOBAL: FF7 0x00e996e0
int g_SubmarineHudValueQuad3Timer = 0;
// GLOBAL: FF7 0x00e996e4
int g_SubmarineHudAlertToggle = 0;
// clang-format on

/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern void __cdecl SfxSetCachedBufferVolume(int soundId, char volume);

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

/* One HUD quad from its record: position offset by the viewport centre, atlas rectangle and
 * UV/colour words, drawn opaque white (the same 16 arguments for every quad). */
#define SUB_DRAW_HUD_QUAD(q) Submarine_DrawAtlasQuad((q.x) + g_SubmarineViewportCenterX, (q.y) + g_SubmarineViewportCenterY, (short)(q.srcX), (short)(q.srcY), (short)(q.width), (short)(q.height), (short)(q.alpha), (q.depth), (q.batches[0]), (q.batches[1]), (q.batches[2]), (q.batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF)

// Draws the submarine HUD from its fixed atlas-quad records, adding the current screen origin and
// mostly using half-alpha white colors. Updates several quad selectors, including copying
// g_SubmarineHudHeadingBits into the first selected record and setting others to 0, 1 or 2; two quads instead
// use their stored per-corner colors. Calls Gfx_EmitGouraudRect for the separate HUD rectangle. In low
// scaling, temporarily halves its integer position and dimensions, adds one to the dimensions,
// then doubles them back after drawing; odd original coordinates or dimensions lose their low bit
// during this restoration. Finally dispatches developer overlays from g_SubmarineGameState bits 1 through
// 0x100, with bit 2 taking precedence over bit 4. The rectangle helper result is unused.
// FUNCTION: FF7 0x0078f4a0
void __cdecl Submarine_DrawHud(void)
{
    int val;

    SUB_DRAW_HUD_QUAD(g_SubmarineHudQuadTable[0]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudQuadTable[1]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudQuadTable[2]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudQuadTable[3]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudQuadTable[4]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudQuadTable[5]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudQuadTable[6]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudQuadTable[7]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudRightFrameQuads[0]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudRightFrameQuads[1]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudRightFrameQuads[2]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudRightFrameQuads[3]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudRightFrameQuads[4]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudRightFrameQuads[5]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudRightFrameQuads[6]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudRightFrameQuads[7]);
    (g_SubmarineHudPitchQuads[0].alpha) = (*(short *)&g_SubmarineHudHeadingBits);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudPitchQuads[0]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudPitchQuads[1]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudPitchQuads[2]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudPitchQuads[3]);
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.quads2[1].x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.quads2[1].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.quads2[1].srcX), (short)(g_SubmarineHudElements.quads2[1].srcY), (short)(g_SubmarineHudElements.quads2[1].width), (short)(g_SubmarineHudElements.quads2[1].height), (short)(g_SubmarineHudElements.quads2[1].alpha), (g_SubmarineHudElements.quads2[1].depth), (g_SubmarineHudElements.quads2[1].batches[0]), (g_SubmarineHudElements.quads2[1].batches[1]), (g_SubmarineHudElements.quads2[1].batches[2]), (g_SubmarineHudElements.quads2[1].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.quads2[2].x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.quads2[2].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.quads2[2].srcX), (short)(g_SubmarineHudElements.quads2[2].srcY), (short)(g_SubmarineHudElements.quads2[2].width), (short)(g_SubmarineHudElements.quads2[2].height), (short)(g_SubmarineHudElements.quads2[2].alpha), (g_SubmarineHudElements.quads2[2].depth), (g_SubmarineHudElements.quads2[2].batches[0]), (g_SubmarineHudElements.quads2[2].batches[1]), (g_SubmarineHudElements.quads2[2].batches[2]), (g_SubmarineHudElements.quads2[2].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.quads2[3].x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.quads2[3].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.quads2[3].srcX), (short)(g_SubmarineHudElements.quads2[3].srcY), (short)(g_SubmarineHudElements.quads2[3].width), (short)(g_SubmarineHudElements.quads2[3].height), (short)(g_SubmarineHudElements.quads2[3].alpha), (g_SubmarineHudElements.quads2[3].depth), (g_SubmarineHudElements.quads2[3].batches[0]), (g_SubmarineHudElements.quads2[3].batches[1]), (g_SubmarineHudElements.quads2[3].batches[2]), (g_SubmarineHudElements.quads2[3].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.quads2[4].x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.quads2[4].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.quads2[4].srcX), (short)(g_SubmarineHudElements.quads2[4].srcY), (short)(g_SubmarineHudElements.quads2[4].width), (short)(g_SubmarineHudElements.quads2[4].height), (short)(g_SubmarineHudElements.quads2[4].alpha), (g_SubmarineHudElements.quads2[4].depth), (g_SubmarineHudElements.quads2[4].batches[0]), (g_SubmarineHudElements.quads2[4].batches[1]), (g_SubmarineHudElements.quads2[4].batches[2]), (g_SubmarineHudElements.quads2[4].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.quads2[5].x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.quads2[5].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.quads2[5].srcX), (short)(g_SubmarineHudElements.quads2[5].srcY), (short)(g_SubmarineHudElements.quads2[5].width), (short)(g_SubmarineHudElements.quads2[5].height), (short)(g_SubmarineHudElements.quads2[5].alpha), (g_SubmarineHudElements.quads2[5].depth), (g_SubmarineHudElements.quads2[5].batches[0]), (g_SubmarineHudElements.quads2[5].batches[1]), (g_SubmarineHudElements.quads2[5].batches[2]), (g_SubmarineHudElements.quads2[5].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudGaugeQuads[0]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudGaugeQuads[1]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudGaugeQuads[2]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudGaugeQuads[3]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudGaugeQuads[4]);
    (g_SubmarineHudYawStripQuads[0].alpha) = 0;
    SUB_DRAW_HUD_QUAD(g_SubmarineHudYawStripQuads[0]);
    (g_SubmarineHudYawStripQuads[1].alpha) = 0;
    SUB_DRAW_HUD_QUAD(g_SubmarineHudYawStripQuads[1]);
    (g_SubmarineHudYawStripQuads[2].alpha) = 0;
    SUB_DRAW_HUD_QUAD(g_SubmarineHudYawStripQuads[2]);
    Submarine_DrawAtlasQuad((g_SubmarineHudRectQuad.x) + g_SubmarineViewportCenterX, (g_SubmarineHudRectQuad.y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudRectQuad.srcX), (short)(g_SubmarineHudRectQuad.srcY), (short)(g_SubmarineHudRectQuad.width), (short)(g_SubmarineHudRectQuad.height), (short)(g_SubmarineHudRectQuad.alpha), (g_SubmarineHudRectQuad.depth), (g_SubmarineHudRectQuad.batches[0]), (g_SubmarineHudRectQuad.batches[1]), (g_SubmarineHudRectQuad.batches[2]), (g_SubmarineHudRectQuad.batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    if (GetUiScalingMode() != 2) {
        g_SubmarineHudGauge.x >>= 1;
        (g_SubmarineHudGauge.y) >>= 1;
        (g_SubmarineHudGauge.width) = ((g_SubmarineHudGauge.width) >> 1) + 1;
        (g_SubmarineHudGauge.height) = ((g_SubmarineHudGauge.height) >> 1) + 1;
        val = Gfx_EmitGouraudRect(g_SubmarineHudGauge.x + g_SubmarineViewportCenterX, (g_SubmarineHudGauge.y) + g_SubmarineViewportCenterY, (g_SubmarineHudGauge.width) << 1, (g_SubmarineHudGauge.height) << 1, (g_SubmarineHudGauge.colorTopLeft), (g_SubmarineHudGauge.colorTopRight), (g_SubmarineHudGauge.gaugeValue), (g_SubmarineHudGauge.frameValue), (g_SubmarineHudGauge.colorBottomLeft), (g_SubmarineHudGauge.colorBottomRight), g_SubmarineHudRenderObjectE);
        g_SubmarineHudGauge.x <<= 1;
        (g_SubmarineHudGauge.y) <<= 1;
        (g_SubmarineHudGauge.width) = (g_SubmarineHudGauge.width) * 2 - 2;
        (g_SubmarineHudGauge.height) = (g_SubmarineHudGauge.height) * 2 - 2;
    } else {
        val = Gfx_EmitGouraudRect(g_SubmarineHudGauge.x + g_SubmarineViewportCenterX, (g_SubmarineHudGauge.y) + g_SubmarineViewportCenterY, (g_SubmarineHudGauge.width) << 1, (g_SubmarineHudGauge.height) << 1, (g_SubmarineHudGauge.colorTopLeft), (g_SubmarineHudGauge.colorTopRight), (g_SubmarineHudGauge.gaugeValue), (g_SubmarineHudGauge.frameValue), (g_SubmarineHudGauge.colorBottomLeft), (g_SubmarineHudGauge.colorBottomRight), g_SubmarineHudRenderObjectE);
    }
    (g_SubmarineHudLayout.quads[0].alpha) = 2;
    Submarine_DrawAtlasQuad((g_SubmarineHudLayout.quads[0].x) + g_SubmarineViewportCenterX, (g_SubmarineHudLayout.quads[0].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudLayout.quads[0].srcX), (short)(g_SubmarineHudLayout.quads[0].srcY), (short)(g_SubmarineHudLayout.quads[0].width), (short)(g_SubmarineHudLayout.quads[0].height), (short)(g_SubmarineHudLayout.quads[0].alpha), (g_SubmarineHudLayout.quads[0].depth), (g_SubmarineHudLayout.quads[0].batches[0]), (g_SubmarineHudLayout.quads[0].batches[1]), (g_SubmarineHudLayout.quads[0].batches[2]), (g_SubmarineHudLayout.quads[0].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    (g_SubmarineHudLayout.quads[1].alpha) = 0;
    Submarine_DrawAtlasQuad((g_SubmarineHudLayout.quads[1].x) + g_SubmarineViewportCenterX, (g_SubmarineHudLayout.quads[1].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudLayout.quads[1].srcX), (short)(g_SubmarineHudLayout.quads[1].srcY), (short)(g_SubmarineHudLayout.quads[1].width), (short)(g_SubmarineHudLayout.quads[1].height), (short)(g_SubmarineHudLayout.quads[1].alpha), (g_SubmarineHudLayout.quads[1].depth), (g_SubmarineHudLayout.quads[1].batches[0]), (g_SubmarineHudLayout.quads[1].batches[1]), (g_SubmarineHudLayout.quads[1].batches[2]), (g_SubmarineHudLayout.quads[1].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    (g_SubmarineHudLayout.bar.alpha) = 1;
    Submarine_DrawAtlasQuad((g_SubmarineHudLayout.bar.x) + g_SubmarineViewportCenterX, (g_SubmarineHudLayout.bar.y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudLayout.bar.srcX), (short)(g_SubmarineHudLayout.bar.srcY), (short)(g_SubmarineHudLayout.bar.width), (short)(g_SubmarineHudLayout.bar.height), (short)(g_SubmarineHudLayout.bar.alpha), (g_SubmarineHudLayout.bar.depth), (g_SubmarineHudLayout.bar.batches[0]), (g_SubmarineHudLayout.bar.batches[1]), (g_SubmarineHudLayout.bar.batches[2]), (g_SubmarineHudLayout.bar.batches[3]), (g_SubmarineHudLayout.bar.colors[0]), (g_SubmarineHudLayout.bar.colors[1]), (g_SubmarineHudLayout.bar.colors[2]), (g_SubmarineHudLayout.bar.colors[3]));
    (g_SubmarineHudElements.quads[0].alpha) = 2;
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.quads[0].x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.quads[0].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.quads[0].srcX), (short)(g_SubmarineHudElements.quads[0].srcY), (short)(g_SubmarineHudElements.quads[0].width), (short)(g_SubmarineHudElements.quads[0].height), (short)(g_SubmarineHudElements.quads[0].alpha), (g_SubmarineHudElements.quads[0].depth), (g_SubmarineHudElements.quads[0].batches[0]), (g_SubmarineHudElements.quads[0].batches[1]), (g_SubmarineHudElements.quads[0].batches[2]), (g_SubmarineHudElements.quads[0].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    (g_SubmarineHudElements.quads[1].alpha) = 0;
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.quads[1].x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.quads[1].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.quads[1].srcX), (short)(g_SubmarineHudElements.quads[1].srcY), (short)(g_SubmarineHudElements.quads[1].width), (short)(g_SubmarineHudElements.quads[1].height), (short)(g_SubmarineHudElements.quads[1].alpha), (g_SubmarineHudElements.quads[1].depth), (g_SubmarineHudElements.quads[1].batches[0]), (g_SubmarineHudElements.quads[1].batches[1]), (g_SubmarineHudElements.quads[1].batches[2]), (g_SubmarineHudElements.quads[1].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    (g_SubmarineHudElements.bar.alpha) = 1;
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.bar.x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.bar.y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.bar.srcX), (short)(g_SubmarineHudElements.bar.srcY), (short)(g_SubmarineHudElements.bar.width), (short)(g_SubmarineHudElements.bar.height), (short)(g_SubmarineHudElements.bar.alpha), (g_SubmarineHudElements.bar.depth), (g_SubmarineHudElements.bar.batches[0]), (g_SubmarineHudElements.bar.batches[1]), (g_SubmarineHudElements.bar.batches[2]), (g_SubmarineHudElements.bar.batches[3]), (g_SubmarineHudElements.bar.colors[0]), (g_SubmarineHudElements.bar.colors[1]), (g_SubmarineHudElements.bar.colors[2]), (g_SubmarineHudElements.bar.colors[3]));
    (g_SubmarineHudElements.quads2[0].alpha) = 0;
    Submarine_DrawAtlasQuad((g_SubmarineHudElements.quads2[0].x) + g_SubmarineViewportCenterX, (g_SubmarineHudElements.quads2[0].y) + g_SubmarineViewportCenterY, (short)(g_SubmarineHudElements.quads2[0].srcX), (short)(g_SubmarineHudElements.quads2[0].srcY), (short)(g_SubmarineHudElements.quads2[0].width), (short)(g_SubmarineHudElements.quads2[0].height), (short)(g_SubmarineHudElements.quads2[0].alpha), (g_SubmarineHudElements.quads2[0].depth), (g_SubmarineHudElements.quads2[0].batches[0]), (g_SubmarineHudElements.quads2[0].batches[1]), (g_SubmarineHudElements.quads2[0].batches[2]), (g_SubmarineHudElements.quads2[0].batches[3]), 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudValueQuads[4]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudValueQuads[5]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudValueQuads[0]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudValueQuads[1]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudValueQuads[2]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudValueQuads[3]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudCooldownQuads[0]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudCooldownQuads[1]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudCooldownQuads[2]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudCooldownQuads[3]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudCooldownQuads[4]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudCooldownQuads[5]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudCooldownQuads[6]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudCooldownQuads[7]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudDigitQuads[0]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudDigitQuads[1]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudDigitQuads[2]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudDigitQuads[3]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudDigitQuads[4]);
    SUB_DRAW_HUD_QUAD(g_SubmarineHudDigitQuads[5]);
    if ((g_SubmarineGameState & SUBMARINE_HUD_PAUSE_BANNER) != 0) {
        Submarine_DrawSingleAtlasQuadRecord();
    }
    if ((g_SubmarineGameState & SUBMARINE_HUD_QUIT_PROMPT) != 0) {
        Submarine_DrawSwapFrameAtlasQuads();
    } else if ((g_SubmarineGameState & SUBMARINE_HUD_PAUSE_HELP) != 0) {
        Submarine_DrawFiveAtlasQuadRecords();
    }
    if ((g_SubmarineGameState & SUBMARINE_HUD_SCREEN_TINT) != 0) {
        Submarine_FillFullScreenTint();
    }
    if ((g_SubmarineGameState & SUBMARINE_HUD_MISSION_COMPLETE) != 0) {
        Submarine_DrawFifteenAtlasQuadRecords();
    }
    if ((g_SubmarineGameState & SUBMARINE_HUD_MISSION_FAILED) != 0) {
        Submarine_DrawSeventeenAtlasQuadRecords();
    }
    if ((g_SubmarineGameState & SUBMARINE_HUD_TIME_UP) != 0) {
        Submarine_DrawNineAtlasQuadRecords();
    }
    if ((g_SubmarineGameState & SUBMARINE_HUD_TARGET_TALLY) != 0) {
        Submarine_DrawTargetTallyPanel();
    }
    if ((g_SubmarineGameState & SUBMARINE_HUD_SCORE_PANEL) != 0) {
        Submarine_DrawScorePanel();
    }
}
/* Draws a single textured quad record from the HUD texture atlas. */
// FUNCTION: FF7 0x0079157f
void __cdecl Submarine_DrawSingleAtlasQuadRecord(void)
{
    Submarine_DrawAtlasQuad(g_SubmarineHudTallyElements.single.x + g_SubmarineViewportCenterX, g_SubmarineHudTallyElements.single.y + g_SubmarineViewportCenterY, g_SubmarineHudTallyElements.single.srcX, g_SubmarineHudTallyElements.single.srcY, g_SubmarineHudTallyElements.single.width, g_SubmarineHudTallyElements.single.height, g_SubmarineHudTallyElements.single.alpha, g_SubmarineHudTallyElements.single.depth, g_SubmarineHudTallyElements.single.batches[0], g_SubmarineHudTallyElements.single.batches[1], g_SubmarineHudTallyElements.single.batches[2], g_SubmarineHudTallyElements.single.batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
}
/* Draws a group of five textured quad records from the HUD texture atlas. */
// FUNCTION: FF7 0x00791603
void __cdecl Submarine_DrawFiveAtlasQuadRecords(void)
{
    int i;

    for (i = 0; i < 5; i++) {
        Submarine_DrawAtlasQuad(g_SubmarineHudFiveQuadRecords.quads[i].x + g_SubmarineViewportCenterX, g_SubmarineHudFiveQuadRecords.quads[i].y + g_SubmarineViewportCenterY, g_SubmarineHudFiveQuadRecords.quads[i].srcX, g_SubmarineHudFiveQuadRecords.quads[i].srcY, g_SubmarineHudFiveQuadRecords.quads[i].width, g_SubmarineHudFiveQuadRecords.quads[i].height, g_SubmarineHudFiveQuadRecords.quads[i].alpha, g_SubmarineHudFiveQuadRecords.quads[i].depth, g_SubmarineHudFiveQuadRecords.quads[i].batches[0], g_SubmarineHudFiveQuadRecords.quads[i].batches[1], g_SubmarineHudFiveQuadRecords.quads[i].batches[2], g_SubmarineHudFiveQuadRecords.quads[i].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    }
}
/* Fills screen with solid color tint for damage or underwater overlay. */
// FUNCTION: FF7 0x007916f7
void __cdecl Submarine_FillFullScreenTint(void)
{
    int val;

    val = Gfx_EmitFlatRect(0, 0, 640, 480, 0x7F7F0000, 0, 0, g_SubmarineHudScreenTint);
}
/* Draws swap-frame animated quad records from the HUD texture atlas. */
// FUNCTION: FF7 0x00791727
void __cdecl Submarine_DrawSwapFrameAtlasQuads(void)
{
    int swapIdx;
    SubmarineHudRecord *q;

    for (swapIdx = 0; swapIdx < 5; swapIdx++) {
        Submarine_DrawAtlasQuad(g_SubmarineHudRenderState.swapQuads[swapIdx].x + g_SubmarineViewportCenterX, g_SubmarineHudRenderState.swapQuads[swapIdx].y + g_SubmarineViewportCenterY, g_SubmarineHudRenderState.swapQuads[swapIdx].srcX, g_SubmarineHudRenderState.swapQuads[swapIdx].srcY, g_SubmarineHudRenderState.swapQuads[swapIdx].width, g_SubmarineHudRenderState.swapQuads[swapIdx].height, g_SubmarineHudRenderState.swapQuads[swapIdx].alpha, g_SubmarineHudRenderState.swapQuads[swapIdx].depth, g_SubmarineHudRenderState.swapQuads[swapIdx].batches[0], g_SubmarineHudRenderState.swapQuads[swapIdx].batches[1], g_SubmarineHudRenderState.swapQuads[swapIdx].batches[2], g_SubmarineHudRenderState.swapQuads[swapIdx].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    }
    Submarine_DrawAtlasQuad(g_SubmarineHudRenderState.swapQuads[6].x + g_SubmarineViewportCenterX, g_SubmarineHudRenderState.swapQuads[6].y + g_SubmarineViewportCenterY, g_SubmarineHudRenderState.swapQuads[6].srcX, g_SubmarineHudRenderState.swapQuads[6].srcY, g_SubmarineHudRenderState.swapQuads[6].width, g_SubmarineHudRenderState.swapQuads[6].height, g_SubmarineHudRenderState.swapQuads[6].alpha, g_SubmarineHudRenderState.swapQuads[6].depth, g_SubmarineHudRenderState.swapQuads[6].batches[0], g_SubmarineHudRenderState.swapQuads[6].batches[1], g_SubmarineHudRenderState.swapQuads[6].batches[2], g_SubmarineHudRenderState.swapQuads[6].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    q = (SubmarineHudRecord *)(g_SubmarineHudRenderState.hudRecordPtr);
    if (q != 0) {
        if ((int)q != 1) {
            return;
        }
        g_SubmarineHudRenderState.swapQuads[5].alpha = 6;
        Submarine_DrawAtlasQuad(g_SubmarineHudRenderState.swapQuads[5].x + g_SubmarineViewportCenterX, g_SubmarineHudRenderState.swapQuads[5].y + g_SubmarineViewportCenterY, g_SubmarineHudRenderState.swapQuads[5].srcX, g_SubmarineHudRenderState.swapQuads[5].srcY, g_SubmarineHudRenderState.swapQuads[5].width, g_SubmarineHudRenderState.swapQuads[5].height, g_SubmarineHudRenderState.swapQuads[5].alpha, g_SubmarineHudRenderState.swapQuads[5].depth, g_SubmarineHudRenderState.swapQuads[5].batches[0], g_SubmarineHudRenderState.swapQuads[5].batches[1], g_SubmarineHudRenderState.swapQuads[5].batches[2], g_SubmarineHudRenderState.swapQuads[5].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
        g_SubmarineHudRenderState.swapQuads[7].alpha = 5;
        Submarine_DrawAtlasQuad(g_SubmarineHudRenderState.swapQuads[7].x + g_SubmarineViewportCenterX, g_SubmarineHudRenderState.swapQuads[7].y + g_SubmarineViewportCenterY, g_SubmarineHudRenderState.swapQuads[7].srcX, g_SubmarineHudRenderState.swapQuads[7].srcY, g_SubmarineHudRenderState.swapQuads[7].width, g_SubmarineHudRenderState.swapQuads[7].height, g_SubmarineHudRenderState.swapQuads[7].alpha, g_SubmarineHudRenderState.swapQuads[7].depth, g_SubmarineHudRenderState.swapQuads[7].batches[0], g_SubmarineHudRenderState.swapQuads[7].batches[1], g_SubmarineHudRenderState.swapQuads[7].batches[2], g_SubmarineHudRenderState.swapQuads[7].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    } else {
        g_SubmarineHudRenderState.swapQuads[5].alpha = 5;
        Submarine_DrawAtlasQuad(g_SubmarineHudRenderState.swapQuads[5].x + g_SubmarineViewportCenterX, g_SubmarineHudRenderState.swapQuads[5].y + g_SubmarineViewportCenterY, g_SubmarineHudRenderState.swapQuads[5].srcX, g_SubmarineHudRenderState.swapQuads[5].srcY, g_SubmarineHudRenderState.swapQuads[5].width, g_SubmarineHudRenderState.swapQuads[5].height, g_SubmarineHudRenderState.swapQuads[5].alpha, g_SubmarineHudRenderState.swapQuads[5].depth, g_SubmarineHudRenderState.swapQuads[5].batches[0], g_SubmarineHudRenderState.swapQuads[5].batches[1], g_SubmarineHudRenderState.swapQuads[5].batches[2], g_SubmarineHudRenderState.swapQuads[5].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
        g_SubmarineHudRenderState.swapQuads[7].alpha = 6;
        Submarine_DrawAtlasQuad(g_SubmarineHudRenderState.swapQuads[7].x + g_SubmarineViewportCenterX, g_SubmarineHudRenderState.swapQuads[7].y + g_SubmarineViewportCenterY, g_SubmarineHudRenderState.swapQuads[7].srcX, g_SubmarineHudRenderState.swapQuads[7].srcY, g_SubmarineHudRenderState.swapQuads[7].width, g_SubmarineHudRenderState.swapQuads[7].height, g_SubmarineHudRenderState.swapQuads[7].alpha, g_SubmarineHudRenderState.swapQuads[7].depth, g_SubmarineHudRenderState.swapQuads[7].batches[0], g_SubmarineHudRenderState.swapQuads[7].batches[1], g_SubmarineHudRenderState.swapQuads[7].batches[2], g_SubmarineHudRenderState.swapQuads[7].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    }
}
/* Draws a group of fifteen quad records from the HUD texture atlas. */
// FUNCTION: FF7 0x00791ae4
void __cdecl Submarine_DrawFifteenAtlasQuadRecords(void)
{
    int i;

    for (i = 0; i < 15; i++) {
        Submarine_DrawAtlasQuad(g_SubmarineHudFifteenElements.quads[i].x + g_SubmarineViewportCenterX, g_SubmarineHudFifteenElements.quads[i].y + g_SubmarineViewportCenterY, g_SubmarineHudFifteenElements.quads[i].srcX, g_SubmarineHudFifteenElements.quads[i].srcY, g_SubmarineHudFifteenElements.quads[i].width, g_SubmarineHudFifteenElements.quads[i].height, g_SubmarineHudFifteenElements.quads[i].alpha, g_SubmarineHudFifteenElements.quads[i].depth, g_SubmarineHudFifteenElements.quads[i].batches[0], g_SubmarineHudFifteenElements.quads[i].batches[1], g_SubmarineHudFifteenElements.quads[i].batches[2], g_SubmarineHudFifteenElements.quads[i].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    }
}
/* Draws a group of seventeen quad records from the HUD texture atlas. */
// FUNCTION: FF7 0x00791bd8
void __cdecl Submarine_DrawSeventeenAtlasQuadRecords(void)
{
    int i;

    for (i = 0; i < 17; i++) {
        Submarine_DrawAtlasQuad(g_SubmarineHudSeventeenElements[i].x + g_SubmarineViewportCenterX, g_SubmarineHudSeventeenElements[i].y + g_SubmarineViewportCenterY, g_SubmarineHudSeventeenElements[i].srcX, g_SubmarineHudSeventeenElements[i].srcY, g_SubmarineHudSeventeenElements[i].width, g_SubmarineHudSeventeenElements[i].height, g_SubmarineHudSeventeenElements[i].alpha, g_SubmarineHudSeventeenElements[i].depth, g_SubmarineHudSeventeenElements[i].batches[0], g_SubmarineHudSeventeenElements[i].batches[1], g_SubmarineHudSeventeenElements[i].batches[2], g_SubmarineHudSeventeenElements[i].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    }
}
/* Draws a group of nine quad records from the HUD texture atlas. */
// FUNCTION: FF7 0x00791ccc
void __cdecl Submarine_DrawNineAtlasQuadRecords(void)
{
    int i;

    for (i = 0; i < 9; i++) {
        Submarine_DrawAtlasQuad(g_SubmarineHudNineElements[i].x + g_SubmarineViewportCenterX, g_SubmarineHudNineElements[i].y + g_SubmarineViewportCenterY, g_SubmarineHudNineElements[i].srcX, g_SubmarineHudNineElements[i].srcY, g_SubmarineHudNineElements[i].width, g_SubmarineHudNineElements[i].height, g_SubmarineHudNineElements[i].alpha, g_SubmarineHudNineElements[i].depth, g_SubmarineHudNineElements[i].batches[0], g_SubmarineHudNineElements[i].batches[1], g_SubmarineHudNineElements[i].batches[2], g_SubmarineHudNineElements[i].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    }
}
/* Draws target tally counter panel on the submarine HUD. */
// FUNCTION: FF7 0x00791dc0
void __cdecl Submarine_DrawTargetTallyPanel(void)
{
    int i;

    for (i = 0; i < 11; i++) {
        Submarine_DrawAtlasQuad(g_SubmarineHudTallyElements.quads[i].x + g_SubmarineViewportCenterX, g_SubmarineHudTallyElements.quads[i].y + g_SubmarineViewportCenterY, g_SubmarineHudTallyElements.quads[i].srcX, g_SubmarineHudTallyElements.quads[i].srcY, g_SubmarineHudTallyElements.quads[i].width, g_SubmarineHudTallyElements.quads[i].height, g_SubmarineHudTallyElements.quads[i].alpha, g_SubmarineHudTallyElements.quads[i].depth, g_SubmarineHudTallyElements.quads[i].batches[0], g_SubmarineHudTallyElements.quads[i].batches[1], g_SubmarineHudTallyElements.quads[i].batches[2], g_SubmarineHudTallyElements.quads[i].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    }
}
/* Draws score and points counter panel on the submarine HUD. */
// FUNCTION: FF7 0x00791eb4
void __cdecl Submarine_DrawScorePanel(void)
{
    int i;

    for (i = 0; i < 10; i++) {
        Submarine_DrawAtlasQuad(g_SubmarineHudScoreCounters[i].x + g_SubmarineViewportCenterX, g_SubmarineHudScoreCounters[i].y + g_SubmarineViewportCenterY, g_SubmarineHudScoreCounters[i].srcX, g_SubmarineHudScoreCounters[i].srcY, g_SubmarineHudScoreCounters[i].width, g_SubmarineHudScoreCounters[i].height, g_SubmarineHudScoreCounters[i].alpha, g_SubmarineHudScoreCounters[i].depth, g_SubmarineHudScoreCounters[i].batches[0], g_SubmarineHudScoreCounters[i].batches[1], g_SubmarineHudScoreCounters[i].batches[2], g_SubmarineHudScoreCounters[i].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
    }
}
/* Emits HUD render objects and batches to graphics display list. */
// FUNCTION: FF7 0x00791fa8
void __cdecl Submarine_EmitHudRenderObjects(struct GfxEngineContext *renderContext)
{
    Gfx_SetScenePassCall(1, (struct GfxEngineContext *)(renderContext));
    if ((g_SubmarineFlags & SUBMARINE_FLAG_HIDE_HUD_GAUGES) != 0) {
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)(renderContext));
    }
    Gfx_DrawRenderObjectCall((GfxRenderObject *)g_SubmarineHudRenderObjectE, renderContext);
    if (GetUiScalingMode() == 2) {
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudMainAtlasA), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudBatchTexB), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudBatchTexC), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudBatchTexD), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudBarAtlasA), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudRenderObjectB), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudBarAtlasC), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudBarAtlasD), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudRenderState.renderObjectAlt), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudSingleTextAtlasB), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudSingleTextAtlasC), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudRenderObjectD), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudTextAtlasA), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudTextAtlasB), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudTextAtlasC), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudTextAtlasD), renderContext);
    } else {
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudMainAtlas), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudBarAtlas), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudRenderState.renderObject), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudTextAtlas), renderContext);
    }
    Gfx_DrawRenderObjectCall((GfxRenderObject *)g_SubmarineHudScreenTint, renderContext);
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)(renderContext));
    if ((g_SubmarineFlags & SUBMARINE_FLAG_HIDE_HUD_GAUGES) != 0) {
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)(renderContext));
    }
    if (GetUiScalingMode() == 2) {
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudReadoutAtlasA), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudReadoutAtlasB), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudRenderObjectA), renderContext);
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudReadoutAtlasD), renderContext);
    } else {
        Gfx_EmitPrimitiveBatchList((void *)(g_SubmarineHudReadoutAtlas), renderContext);
    }
}

// Updates submarine HUD records from fixed-point depth, signed motion, heading, tilt, power and
// elapsed timer globals. Sets numeric digit atlas coordinates, sign glyphs, proportional indicator
// positions, a wrapped heading strip split across two records, and a nonnegative power width/color
// blend. Advances several looping atlas/position animations and four packed status icon selectors,
// then converts the nonnegative frame timer into four minute/second digits. Some computed status
// colors are never used. Power is clamped only below zero and the heading split leaves prior
// secondary fields when no split is needed.
// FUNCTION: FF7 0x00792216
void __cdecl Submarine_UpdateHudGauges(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        int digitPos;
        struct { unsigned char pad_to_4[0x4]; int remainingTime; };
        struct { unsigned char pad_to_8[0x8]; int iconColor; };
        struct { unsigned char pad_to_C[0xC]; int cooldownWord; };
        struct { unsigned char pad_to_10[0x10]; char iconLevel; };
        struct { unsigned char pad_to_14[0x14]; int cooldownIndex; };
        struct { unsigned char pad_to_18[0x18]; int scrollPos; };
        struct { unsigned char pad_to_1C[0x1C]; int animFrame; };
        struct { unsigned char pad_to_20[0x20]; int animIndex; };
        struct { unsigned char pad_to_24[0x24]; int power; };
        struct { unsigned char pad_to_28[0x28]; int gaugeCopy; };
        struct { unsigned char pad_to_29[0x29]; char blendAlpha; };
        struct { unsigned char pad_to_2A[0x2A]; char blendInverse; };
        struct { unsigned char pad_to_2C[0x2C]; int heading; };
        struct { unsigned char pad_to_30[0x30]; int heightPos; };
        struct { unsigned char pad_to_34[0x34]; int speedReading; };
        struct { unsigned char pad_to_38[0x38]; int depth; };
        struct { unsigned char pad_to_3C[0x3C]; int tmpValue; };
    } frame;
#pragma pack(pop)

    frame.depth = g_SubmarineState.player.posY >> 12;
    frame.tmpValue = (frame.depth * 11 / 100) * 2 + -0xD6;
    (g_SubmarineHudElements.quads2[1].y) = *(short *)&frame.tmpValue;
    frame.tmpValue += 6;
    (g_SubmarineHudElements.quads2[2].y) = *(short *)&frame.tmpValue;
    (g_SubmarineHudElements.quads2[3].y) = *(short *)&frame.tmpValue;
    (g_SubmarineHudElements.quads2[4].y) = *(short *)&frame.tmpValue;
    (g_SubmarineHudElements.quads2[5].y) = *(short *)&frame.tmpValue;
    frame.tmpValue = frame.depth % 10;
    frame.depth = frame.depth / 10;
    frame.tmpValue = frame.tmpValue * 4 + 0xC8;
    (g_SubmarineHudElements.quads2[5].srcX) = *(unsigned char *)&frame.tmpValue;
    frame.tmpValue = frame.depth % 10;
    frame.depth = frame.depth / 10;
    frame.tmpValue = frame.tmpValue * 4 + 0xC8;
    (g_SubmarineHudElements.quads2[4].srcX) = *(unsigned char *)&frame.tmpValue;
    frame.tmpValue = frame.depth % 10;
    frame.depth = frame.depth / 10;
    frame.tmpValue = frame.tmpValue * 4 + 0xC8;
    (g_SubmarineHudElements.quads2[3].srcX) = *(unsigned char *)&frame.tmpValue;
    frame.tmpValue = frame.depth % 10;
    frame.depth = frame.depth / 10;
    frame.tmpValue = frame.tmpValue * 4 + 0xC8;
    (g_SubmarineHudElements.quads2[2].srcX) = *(unsigned char *)&frame.tmpValue;
    frame.tmpValue = (g_SubmarineState.speedScale << 6) / (g_SubmarineWorkingLevelDepth << 1);
    frame.tmpValue = 0xFFFFFFAA - (frame.tmpValue << 1);
    (g_SubmarineHudGaugeQuads[0].y) = *(short *)&frame.tmpValue;
    frame.tmpValue += 6;
    (g_SubmarineHudGaugeQuads[1].y) = *(short *)&frame.tmpValue;
    (g_SubmarineHudGaugeQuads[2].y) = *(short *)&frame.tmpValue;
    (g_SubmarineHudGaugeQuads[3].y) = *(short *)&frame.tmpValue;
    (g_SubmarineHudGaugeQuads[4].y) = *(short *)&frame.tmpValue;
    frame.speedReading = g_SubmarineState.speedScale * 200 / (g_SubmarineWorkingLevelDepth << 1);
    if (frame.speedReading > 0) {
        (g_SubmarineHudGaugeQuads[1].srcX) = 240;
    } else if (frame.speedReading < 0) {
        (g_SubmarineHudGaugeQuads[1].srcX) = 244;
        frame.speedReading = -frame.speedReading;
    } else {
        (g_SubmarineHudGaugeQuads[1].srcX) = 248;
    }
    frame.tmpValue = frame.speedReading % 10;
    frame.speedReading = frame.speedReading / 10;
    frame.tmpValue = frame.tmpValue * 4 + 0xC8;
    (g_SubmarineHudGaugeQuads[4].srcX) = *(unsigned char *)&frame.tmpValue;
    frame.tmpValue = frame.speedReading % 10;
    frame.speedReading = frame.speedReading / 10;
    frame.tmpValue = frame.tmpValue * 4 + 0xC8;
    (g_SubmarineHudGaugeQuads[3].srcX) = *(unsigned char *)&frame.tmpValue;
    frame.tmpValue = frame.speedReading % 10;
    frame.tmpValue = frame.tmpValue * 4 + 0xC8;
    (g_SubmarineHudGaugeQuads[2].srcX) = *(unsigned char *)&frame.tmpValue;
    frame.heightPos = g_SubmarineState.player.yaw - 1024;
    frame.heightPos = frame.heightPos + 0x1000 & 0xFFF;
    frame.heightPos = ((frame.heightPos) / 16);
    frame.heightPos = frame.heightPos + 0xCB & 0xFF;
    frame.tmpValue = 0x100 - frame.heightPos;
    if (frame.tmpValue >= 125) {
        (g_SubmarineHudYawStripQuads[0].srcX) = *(unsigned char *)&frame.heightPos;
        (g_SubmarineHudYawStripQuads[0].width) = 125;
        (g_SubmarineHudYawStripQuads[1].width) = 0;
    } else {
        (g_SubmarineHudYawStripQuads[0].srcX) = *(unsigned char *)&frame.heightPos;
        (g_SubmarineHudYawStripQuads[0].width) = *(short *)&frame.tmpValue;
        (g_SubmarineHudYawStripQuads[1].x) = frame.tmpValue * 2 + -124;
        (g_SubmarineHudYawStripQuads[1].srcX) = 0;
        (g_SubmarineHudYawStripQuads[1].width) = 125 - frame.tmpValue;
    }
    frame.heading = g_SubmarineState.player.angle;
    g_SubmarineHudHeadingBits = frame.heading != 0 ? 7 : 0;
    frame.heading = ((frame.heading) / 32);
    (g_SubmarineHudPitchQuads[3].y) = 0xFFFFFFB0 - (frame.heading << 1);
    frame.power = g_SubmarineState.player.playerResource;
    if (frame.power < 0) {
        frame.power = 0;
    }
    frame.tmpValue = ((frame.power * 127) / 16384);
    if (frame.tmpValue == 0 && frame.power > 0) {
        frame.tmpValue = 1;
    }
    (g_SubmarineHudGauge.width) = frame.tmpValue;
    frame.tmpValue = ((frame.power * 255) / 16384);
    frame.gaugeCopy = (g_SubmarineHudGauge.gaugeValue);
    frame.blendAlpha = *(char *)&frame.tmpValue;
    frame.blendInverse = 0xFF - frame.tmpValue;
    (g_SubmarineHudGauge.gaugeValue) = frame.gaugeCopy;
    frame.gaugeCopy = (g_SubmarineHudGauge.frameValue);
    frame.blendAlpha = *(char *)&frame.tmpValue;
    frame.blendInverse = 0xFF - frame.tmpValue;
    (g_SubmarineHudGauge.frameValue) = frame.gaugeCopy;
    frame.animFrame = g_SubmarineHudBarAnimFrame + 1;
    *(short *)&frame.animIndex = *(short *)&g_SubmarineHudBarAnimIndex;
    if (frame.animFrame > 39) {
        frame.animFrame = 0;
        (*(short *)&frame.animIndex)++;
        if ((frame.animIndex & 0xFFFF) > 5) {
            *(short *)&frame.animIndex = 0;
        }
    }
    g_SubmarineHudBarAnimIndex = frame.animIndex & 0xFFFF;
    g_SubmarineHudBarAnimFrame = frame.animFrame;
    (g_SubmarineHudLayout.bar.x) = frame.animFrame * 2 + 164;
    *(short *)&frame.animIndex = ((short *)&g_SubmarineHudAnimTable)[frame.animIndex & 0xFFFF];
    (g_SubmarineHudLayout.bar.srcX) = (frame.animIndex & 0xFFFF & 0xFF) + frame.animFrame;
    (g_SubmarineHudLayout.bar.srcY) = (frame.animIndex & 0xFFFF) >> 8 & 0xFF;
    frame.scrollPos = g_SubmarineHudScrollPos + 1;
    if (frame.scrollPos > 123) {
        frame.scrollPos = 86;
    }
    g_SubmarineHudScrollPos = frame.scrollPos;
    frame.tmpValue = frame.scrollPos;
    if (frame.tmpValue >= 41) {
        frame.tmpValue -= 0x29;
    }
    if (frame.tmpValue >= 41) {
        frame.tmpValue -= 0x29;
    }
    if (frame.tmpValue >= 41) {
        frame.tmpValue -= 0x29;
    }
    (g_SubmarineHudElements.bar.x) = frame.tmpValue * 2 + -253;
    (g_SubmarineHudElements.bar.srcX) = frame.scrollPos + 112;
    frame.tmpValue = g_SubmarineHudValueQuad5Timer + 1;
    if (frame.tmpValue > 31) {
        frame.tmpValue = 0;
    }
    g_SubmarineHudValueQuad5Timer = frame.tmpValue;
    frame.tmpValue = ((frame.tmpValue) / 8);
    frame.tmpValue = (frame.tmpValue << 4) + 0x50;
    (g_SubmarineHudValueQuads[5].srcX) = *(unsigned char *)&frame.tmpValue;
    frame.tmpValue = g_SubmarineHudValueQuad1Timer + 1;
    if (frame.tmpValue > 31) {
        frame.tmpValue = 0;
    }
    g_SubmarineHudValueQuad1Timer = frame.tmpValue;
    frame.tmpValue = ((frame.tmpValue) / 8);
    frame.tmpValue *= 0x18;
    (g_SubmarineHudValueQuads[1].srcY) = *(unsigned char *)&frame.tmpValue;
    frame.tmpValue = g_SubmarineHudValueQuad3Timer + 1;
    if (frame.tmpValue > 15) {
        frame.tmpValue = 0;
    }
    g_SubmarineHudValueQuad3Timer = frame.tmpValue;
    frame.tmpValue = ((frame.tmpValue) / 4);
    frame.tmpValue = frame.tmpValue * 24 + 0x81;
    (g_SubmarineHudValueQuads[3].srcY) = *(unsigned char *)&frame.tmpValue;
    frame.cooldownIndex = 0;
    do {
        frame.cooldownWord = g_SubmarineState.cooldownTable[frame.cooldownIndex];
        frame.tmpValue = frame.cooldownWord & 0xFFFF;
        frame.iconLevel = 0x20;
        frame.iconColor = 0x66000000;
        if ((frame.cooldownWord & 0x10000) != 0) {
            frame.tmpValue = frame.tmpValue * 255 / g_SubmarineHudScale;
            frame.tmpValue = 0xFF - frame.tmpValue & 0xFF;
            frame.iconColor = frame.tmpValue | frame.tmpValue << 8 | frame.tmpValue << 0x10 | 0x66000000;
            frame.iconLevel = 0x18;
        } else if ((frame.cooldownWord & 0x20000) != 0) {
            frame.iconColor = 0x66FFFFFF;
            frame.iconLevel = 0x10;
        }
        g_SubmarineHudCooldownQuads[frame.cooldownIndex + 4].srcY = (unsigned short)*(volatile unsigned char *)&frame.iconLevel;
        frame.cooldownIndex++;
    } while (frame.cooldownIndex < 4);
    frame.remainingTime = g_SubmarineTimeRemaining;
    if (frame.remainingTime < 0) {
        frame.remainingTime = 0;
    }
    frame.remainingTime = frame.remainingTime / 60;
    frame.tmpValue = frame.remainingTime / 600;
    frame.digitPos = frame.tmpValue * 8 + 120;
    (g_SubmarineHudDigitQuads[2].srcX) = *(unsigned char *)&frame.digitPos;
    frame.remainingTime -= frame.tmpValue * 600;
    frame.tmpValue = frame.remainingTime / 60;
    frame.digitPos = frame.tmpValue * 8 + 120;
    (g_SubmarineHudDigitQuads[3].srcX) = *(unsigned char *)&frame.digitPos;
    frame.remainingTime -= frame.tmpValue * 60;
    frame.tmpValue = frame.remainingTime / 10;
    frame.digitPos = frame.tmpValue * 8 + 120;
    (g_SubmarineHudDigitQuads[4].srcX) = *(unsigned char *)&frame.digitPos;
    frame.remainingTime -= frame.tmpValue * 10;
    frame.digitPos = frame.remainingTime * 8 + 120;
    (g_SubmarineHudDigitQuads[5].srcX) = *(unsigned char *)&frame.digitPos;
}

// Draws whichever of four warning indicators the bits of g_SubmarineState.motionFlags select, and
// drives an alarm sound from how many are lit. Bits 0-1 draw the record at g_SubmarineHudQuads
// with frame g_SubmarineHudQuads[0].alpha set to 3, or to 1 if bit 1 is set, and add 0x20 to
// an accumulator; bit 2 draws g_SubmarineHudQuads[1].x with frame 1 and adds 0x2A; bit 10
// draws g_SubmarineHudQuads[2].x with frame 3 and adds 0x30; bit 11 draws
// g_SubmarineHudQuads[3].x with frame 3 and adds 0x3A. If the accumulator is non-zero it is
// clamped to 0x7F and used as the volume of cached sound 0x243, started once via
// SfxPlayCachedBuffer with g_SubmarineHudAlertToggle as the playing flag and updated with
// SfxSetCachedBufferVolume thereafter; with no indicator lit the sound is stopped and rewound
// (SfxStopAndRewindCachedBuffer) and the flag cleared.
// FUNCTION: FF7 0x007929b1
void __cdecl Submarine_UpdateAlertHudAndAlarm(void)
{
    unsigned char q;
    int motionBits;

    q = 0;
    motionBits = g_SubmarineState.motionFlags;
    if ((motionBits & (SUBMARINE_MOTION_HIT_TERRAIN | SUBMARINE_MOTION_NEAR_TERRAIN)) != 0) {
        g_SubmarineHudQuads[0].alpha = 3;
        if ((motionBits & SUBMARINE_MOTION_NEAR_TERRAIN) != 0) {
            g_SubmarineHudQuads[0].alpha = 1;
        }
        Submarine_DrawAtlasQuad((g_SubmarineHudQuads[0].x) + g_SubmarineViewportCenterX, g_SubmarineHudQuads[0].y + g_SubmarineViewportCenterY, g_SubmarineHudQuads[0].srcX, g_SubmarineHudQuads[0].srcY, g_SubmarineHudQuads[0].width, g_SubmarineHudQuads[0].height, g_SubmarineHudQuads[0].alpha, g_SubmarineHudQuads[0].depth, g_SubmarineHudQuads[0].batches[0], g_SubmarineHudQuads[0].batches[1], g_SubmarineHudQuads[0].batches[2], g_SubmarineHudQuads[0].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
        q += 0x20;
    }
    if ((motionBits & SUBMARINE_MOTION_HIT_MINE) != 0) {
        g_SubmarineHudQuads[1].alpha = 1;
        Submarine_DrawAtlasQuad(g_SubmarineHudQuads[1].x + g_SubmarineViewportCenterX, g_SubmarineHudQuads[1].y + g_SubmarineViewportCenterY, g_SubmarineHudQuads[1].srcX, g_SubmarineHudQuads[1].srcY, g_SubmarineHudQuads[1].width, g_SubmarineHudQuads[1].height, g_SubmarineHudQuads[1].alpha, g_SubmarineHudQuads[1].depth, g_SubmarineHudQuads[1].batches[0], g_SubmarineHudQuads[1].batches[1], g_SubmarineHudQuads[1].batches[2], g_SubmarineHudQuads[1].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
        q += 0x2A;
    }
    if ((motionBits & SUBMARINE_MOTION_HUD_ALERT_400) != 0) {
        g_SubmarineHudQuads[2].alpha = 3;
        Submarine_DrawAtlasQuad(g_SubmarineHudQuads[2].x + g_SubmarineViewportCenterX, g_SubmarineHudQuads[2].y + g_SubmarineViewportCenterY, g_SubmarineHudQuads[2].srcX, g_SubmarineHudQuads[2].srcY, g_SubmarineHudQuads[2].width, g_SubmarineHudQuads[2].height, g_SubmarineHudQuads[2].alpha, g_SubmarineHudQuads[2].depth, g_SubmarineHudQuads[2].batches[0], g_SubmarineHudQuads[2].batches[1], g_SubmarineHudQuads[2].batches[2], g_SubmarineHudQuads[2].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
        q += 0x30;
    }
    if ((motionBits & SUBMARINE_MOTION_HUD_ALERT_800) != 0) {
        g_SubmarineHudQuads[3].alpha = 3;
        Submarine_DrawAtlasQuad(g_SubmarineHudQuads[3].x + g_SubmarineViewportCenterX, g_SubmarineHudQuads[3].y + g_SubmarineViewportCenterY, g_SubmarineHudQuads[3].srcX, g_SubmarineHudQuads[3].srcY, g_SubmarineHudQuads[3].width, g_SubmarineHudQuads[3].height, g_SubmarineHudQuads[3].alpha, g_SubmarineHudQuads[3].depth, g_SubmarineHudQuads[3].batches[0], g_SubmarineHudQuads[3].batches[1], g_SubmarineHudQuads[3].batches[2], g_SubmarineHudQuads[3].batches[3], 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0x80FFFFFF);
        q += 0x3A;
    }
    if (q != 0) {
        if (q > 127) {
            q = 0x7F;
        }
        if (g_SubmarineHudAlertToggle == 0) {
            SfxPlayCachedBuffer(0x243, 0x40, (char)q, 0);
            g_SubmarineHudAlertToggle = 1;
        } else {
            SfxSetCachedBufferVolume(0x243, (char)q);
        }
    } else if (g_SubmarineHudAlertToggle != 0) {
        SfxStopAndRewindCachedBuffer(0x243);
        g_SubmarineHudAlertToggle = 0;
    }
    (g_SubmarineHudParams.musicVolume) = q;
}
/* Updates sound effects and volume for player and enemy torpedo firing. */
// FUNCTION: FF7 0x00792ce6
void __cdecl Submarine_UpdateTorpedoAudio(void)
{
    struct {
        int angle;
        int distance;
        struct SubmarineTorpedoAudioScratch *flightRecord;
        int sum;
        int recordIndex;
        int square;
        int count;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.flightRecord = (struct SubmarineTorpedoAudioScratch *)&g_SubmarineFlightRecords[0].posX;
    frame.count = 0;
    frame.square = frame.count;
    frame.sum = frame.square;
    frame.recordIndex = 0;
    do {
        if ((frame.flightRecord->state & 6) == 0) { /* empty first arm: lib/codegen.h */
        } else {
            if ((unsigned)(int)frame.flightRecord >= ((int)&g_SubmarineFlightRecords[16])) {
                g_SubmarineState.motionFlags |= SUBMARINE_MOTION_HUD_ALERT_800;
            }
            frame.distance = frame.flightRecord->playerDistance;
            frame.distance = 320 - frame.distance;
            if (frame.distance < 0) { /* empty first arm: lib/codegen.h */
            } else {
                frame.count++;
                frame.angle = frame.flightRecord->dotForward;
                if (frame.angle < -0x800 && (frame.flightRecord->state & 2) != 0) {
                    g_SubmarineState.motionFlags |= SUBMARINE_MOTION_HUD_ALERT_200;
                }
                if (frame.angle < 0) {
                    frame.angle = -frame.angle;
                }
                frame.angle = frame.angle / 4;
                frame.angle &= 0xfff;
                frame.angle = CosFixed(frame.angle) >> 1;
                if (frame.flightRecord->dotRight < 0) {
                    frame.angle = -frame.angle;
                }
                frame.sum += frame.angle * frame.distance;
                frame.square += frame.distance * frame.distance;
            }
        }

        frame.flightRecord = (struct SubmarineTorpedoAudioScratch *)((char *)frame.flightRecord + 0xc8);
        frame.recordIndex++;
    } while (frame.recordIndex < 32);
    if (frame.count != 0) {
        frame.sum /= 0x100;
        frame.square /= 0x100;
        frame.sum /= frame.count;
        frame.square /= frame.count;
        frame.sum /= 0x20;
        frame.square /= 4;
        if (frame.sum < -64)
            frame.sum = -0x40;
        if (frame.sum > 63)
            frame.sum = 0x3f;
        if (frame.square > 127)
            frame.square = 0x7f;
        if (frame.square == 0)
            frame.square = 1;
        frame.sum += 0x40;
        SoundDispatchCommand(SOUND_SFX_CH3_VOLUME_TRANS, 2, frame.square, 0, 0, 0, 0, 0, 0);
        SoundDispatchCommand(SOUND_SFX_CH3_PAN_TRANS, 2, frame.sum, 0, 0, 0, 0, 0, 0);
    } else {
        SoundDispatchCommand(SOUND_SFX_CH3_VOLUME_TRANS, 8, 1, 0, 0, 0, 0, 0, 0);
        SoundDispatchCommand(SOUND_SFX_CH3_PAN_TRANS, 2, 0x40, 0, 0, 0, 0, 0, 0);
    }
}
/* Updates audio effects and sound channels for nearby enemy submarines. */
// FUNCTION: FF7 0x00792f2f
void __cdecl Submarine_UpdateEnemyAudio(void)
{
    struct {
        int angle;
        int distance;
        int sum;
        int square;
        int count;
        struct SubmarineEnemyAudioScratch *enemyRecord;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.enemyRecord = (struct SubmarineEnemyAudioScratch *)&g_SubmarineModelVertexData;
    frame.count = 0;
    frame.square = frame.count;
    frame.sum = frame.square;
    do {
        if ((frame.enemyRecord->flags & 3) == 0) { /* empty first arm: lib/codegen.h */
        } else {
            if ((frame.enemyRecord->state & 0xf00) != 0) {
                g_SubmarineState.motionFlags |= SUBMARINE_MOTION_HUD_ALERT_400;
            }
            frame.distance = frame.enemyRecord->magnitude;
            frame.distance = 320 - frame.distance;
            if (frame.distance < 0) { /* empty first arm: lib/codegen.h */
            } else {
                frame.count++;
                frame.angle = frame.enemyRecord->dotResultA;
                if (frame.angle < -0x800 && (frame.enemyRecord->flags & 8) != 0) {
                    g_SubmarineState.motionFlags |= SUBMARINE_MOTION_HUD_ALERT_100;
                }
                if (frame.angle < 0) {
                    frame.angle = -frame.angle;
                }
                frame.angle = frame.angle / 4;
                frame.angle &= 0xfff;
                frame.angle = CosFixed(frame.angle) >> 1;
                if (frame.enemyRecord->dotResultB < 0) {
                    frame.angle = -frame.angle;
                }
                frame.sum += frame.angle * frame.distance;
                frame.square += frame.distance * frame.distance;
            }
        }

        frame.enemyRecord = (struct SubmarineEnemyAudioScratch *)((char *)frame.enemyRecord + 0x70);
    } while ((unsigned)(int)frame.enemyRecord < (unsigned)(int)&g_SubmarineLevel0Map);
    if (frame.count != 0) {
        frame.sum /= 0x100;
        frame.square /= 0x100;
        frame.sum /= frame.count;
        frame.square /= frame.count;
        frame.sum /= 0x20;
        frame.square /= 4;
        if (frame.sum < -64)
            frame.sum = -0x40;
        if (frame.sum > 63)
            frame.sum = 0x3f;
    }
    frame.sum += 0x40;
    frame.square += 0x18;
    if (frame.square > 127)
        frame.square = 0x7f;
    SoundDispatchCommand(SOUND_SFX_CH2_VOLUME_TRANS, 2, frame.square, 0, 0, 0, 0, 0, 0);
    SoundDispatchCommand(SOUND_SFX_CH2_PAN_TRANS, 2, frame.sum, 0, 0, 0, 0, 0, 0);
}
/* Initializes digit sprite indices for target count HUD display. */
// FUNCTION: FF7 0x00793128
void __cdecl Submarine_InitTargetCountDigits(void)
{
    unsigned char workSrcX;
    unsigned char q;
    int count;
    int digit;

    if (g_SubmarineMissionId == 0) {
        workSrcX = 0;
        q = 0x60;
    } else {
        workSrcX = 128;
        q = 0x60;
    }
    g_SubmarineHudTallyElements.single.srcX = workSrcX;
    g_SubmarineHudTallyElements.single.srcY = q;
    count = g_SubmarineEffectCount0;
    digit = count / 10;
    g_SubmarineHudTallyElements.quads[9].srcX = (unsigned char)(digit << 4);
    digit = count - digit * 10;
    g_SubmarineHudTallyElements.quads[10].srcX = (unsigned char)(digit << 4);
}

// On reset optionally copies initial phases, loads amplitude and clears reset; otherwise advances
// and wraps both phases to twelve bits. Stores two sine offsets scaled by amplitude and shifted
// eight, then changes amplitude by its increment with a zero floor.
// FUNCTION: FF7 0x007931ae
void __cdecl Submarine_UpdateScreenShake(void)
{
    int q;
    int baseSinX;

    if (g_SubmarineHudParams.screenShakeTimer != 0) {
        if (g_SubmarineScreenShake.accumulator == 0) {
            g_SubmarineScreenShake.pitchAngle = (g_SubmarineHudRange.pitch);
            g_SubmarineScreenShake.rollAngle = g_SubmarineHudRange.max;
        }
        g_SubmarineScreenShake.accumulator = g_SubmarineHudOffset;
        g_SubmarineHudParams.screenShakeTimer = 0;
    } else {
        g_SubmarineScreenShake.pitchAngle = g_SubmarineScreenShake.pitchAngle + (g_SubmarineScreenShake.pitchStep) & 0xFFF;
        g_SubmarineScreenShake.rollAngle = g_SubmarineScreenShake.rollAngle + g_SubmarineScreenShake.yawAngle & 0xFFF;
    }
    baseSinX = SinFixed(g_SubmarineScreenShake.pitchAngle);
    baseSinX = ((g_SubmarineScreenShake.accumulator * baseSinX) / 4096);
    baseSinX >>= 8;
    g_SubmarineScreenShake.projectionOffsetX = baseSinX;
    q = SinFixed(g_SubmarineScreenShake.rollAngle);
    q = ((g_SubmarineScreenShake.accumulator * q) / 4096);
    q >>= 8;
    g_SubmarineScreenShake.projectionOffsetY = q;
    g_SubmarineScreenShake.accumulator += g_SubmarineHudOrigin;
    if (g_SubmarineScreenShake.accumulator < 0) {
        g_SubmarineScreenShake.accumulator = 0;
    }
}
/* Updates 5-digit decimal counter display digits on submarine instrument panel. */
// FUNCTION: FF7 0x007932d5
void __cdecl Submarine_UpdateFiveDigitHudCounter(void)
{
    SubmarineContactRecord *q;
    int sum;
    int weightTotal;
    int shown;
    int digit;
    int leading;

    weightTotal = 0;
    q = (SubmarineContactRecord *)&g_SubmarineModelVertexData;
    do {
        if ((q->flags & 3) != 0) {
            weightTotal += q->slotWeight;
        }
        q = (SubmarineContactRecord *)((char *)q + 0x70);
    } while ((unsigned int)(int)q < (unsigned int)(int)&g_SubmarineLevel0Map);
    sum = g_SubmarineSlotTotalWeight - weightTotal;
    sum += g_SubmarineState.player.playerResource;
    sum += g_SubmarineRemainingTime;
    sum -= g_SubmarineActiveTorpedoCount * 100;
    if (sum < 0) {
        sum = 0;
    }
    if (sum > 99999) {
        sum = 0x1869F;
    }
    leading = 1;
    digit = sum / 10000;
    shown = digit;
    if (shown == 0 && leading == 1) {
        shown = 0xE;
    } else {
        leading = 0;
    }
    (g_SubmarineHudScoreCounters[5].srcX) = (unsigned char)(shown << 4);
    sum -= digit * 10000;
    digit = sum / 1000;
    shown = digit;
    if (shown == 0 && leading == 1) {
        shown = 0xE;
    } else {
        leading = 0;
    }
    (g_SubmarineHudScoreCounters[6].srcX) = (unsigned char)(shown << 4);
    sum -= digit * 1000;
    digit = sum / 100;
    shown = digit;
    if (shown == 0 && leading == 1) {
        shown = 0xE;
    } else {
        leading = 0;
    }
    (g_SubmarineHudScoreCounters[7].srcX) = (unsigned char)(shown << 4);
    sum -= digit * 100;
    digit = sum / 10;
    shown = digit;
    if (shown == 0 && leading == 1) {
        shown = 0xE;
    } else {
        leading = 0;
    }
    (g_SubmarineHudScoreCounters[8].srcX) = (unsigned char)(shown << 4);
    digit = sum - digit * 10;
    (g_SubmarineHudScoreCounters[9].srcX) = (unsigned char)(digit << 4);
}
/* Updates 2-digit decimal counter display digits on submarine instrument panel. */
// FUNCTION: FF7 0x007934c9
void __cdecl Submarine_UpdateTwoDigitHudCounter(void)
{
    int q;
    int sum;

    sum = g_SubmarineEffectCount1 + g_SubmarineActiveSlotCount;
    q = sum / 10;
    g_SubmarineHudTallyElements.quads[6].srcX = (unsigned char)(q << 4);
    q = sum - q * 10;
    g_SubmarineHudTallyElements.quads[7].srcX = (unsigned char)(q << 4);
}
/* Updates submarine HUD mode flag from parameter. */
// FUNCTION: FF7 0x00793520
void __cdecl Submarine_SetSwapQuadSelection(int mode)
{
    SubmarineHudRecord *hudRecord;

    if (mode == 0) {
        (g_SubmarineHudRenderState.hudRecordPtr) = 1;
        hudRecord = (SubmarineHudRecord *)(g_SubmarineHudRenderState.hudRecordPtr);
    } else {
        (g_SubmarineHudRenderState.hudRecordPtr) = 0;
        hudRecord = (SubmarineHudRecord *)(g_SubmarineHudRenderState.hudRecordPtr);
    }
}

// Clears g_SubmarineHudHeadingBits and seeds the submarine HUD atlas records with fixed signed
// screen positions, extents, texture coordinates, flags, depth values, resource pointers and
// several packed color gradients. Uses GetUiScalingMode once; for every mode other than 2, replaces
// each initialized resource group with its corresponding single-resource variant and clears its
// other three resource fields, including the 15-, 17-, 9-, 5-, 8-, 11- and 10-record text groups.
// Also initializes the six byte-coordinate pairs at g_SubmarineHudAnimTable and clears
// g_SubmarineHudRenderState. The routine builds data only and does not draw or allocate resources.
// FUNCTION: FF7 0x00793555
void __cdecl Submarine_BuildHudElementTable(void)
{
    int rightIdx;
    int q;
    int gaugeIdx;
    int readoutSlot;
    int valueTail;
    int coolQuad;
    int fiveIdx;
    int entry9;
    int workSeventeenIdx;
    int swapNo;
    int scoreIndex;
    int calcAlertIdx;
    int scaleMode;
    int pitchSlot;
    int headingIndex;
    int valueMid;
    int vhead;
    int entry15;
    int numberIdx;
    int tallyIndex;

    g_SubmarineHudHeadingBits = 0;
    scaleMode = GetUiScalingMode();
    (g_SubmarineHudQuads[0].x) = 0xFF2C;
    g_SubmarineHudQuads[0].y = 0xFF2A;
    g_SubmarineHudQuads[0].width = 62;
    g_SubmarineHudQuads[0].height = 13;
    g_SubmarineHudQuads[0].srcX = 0;
    g_SubmarineHudQuads[0].srcY = 208;
    g_SubmarineHudQuads[0].alpha = 0;
    g_SubmarineHudQuads[0].depth = FLOAT_0_01_BITS;
    g_SubmarineHudQuads[0].batches[0] = g_SubmarineHudMainAtlasA;
    g_SubmarineHudQuads[0].batches[1] = g_SubmarineHudBatchTexB;
    g_SubmarineHudQuads[0].batches[2] = g_SubmarineHudBatchTexC;
    g_SubmarineHudQuads[0].batches[3] = g_SubmarineHudBatchTexD;
    g_SubmarineHudQuads[1].x = 144;
    g_SubmarineHudQuads[1].y = 0xFF2A;
    g_SubmarineHudQuads[1].width = 34;
    g_SubmarineHudQuads[1].height = 13;
    g_SubmarineHudQuads[1].srcX = 0;
    g_SubmarineHudQuads[1].srcY = 192;
    g_SubmarineHudQuads[1].alpha = 0;
    g_SubmarineHudQuads[1].depth = FLOAT_0_01_BITS;
    g_SubmarineHudQuads[1].batches[0] = g_SubmarineHudMainAtlasA;
    g_SubmarineHudQuads[1].batches[1] = g_SubmarineHudBatchTexB;
    g_SubmarineHudQuads[1].batches[2] = g_SubmarineHudBatchTexC;
    g_SubmarineHudQuads[1].batches[3] = g_SubmarineHudBatchTexD;
    g_SubmarineHudQuads[2].x = 0xFFC2;
    g_SubmarineHudQuads[2].y = 0xFF2E;
    g_SubmarineHudQuads[2].width = 62;
    g_SubmarineHudQuads[2].height = 13;
    g_SubmarineHudQuads[2].srcX = 0;
    g_SubmarineHudQuads[2].srcY = 224;
    g_SubmarineHudQuads[2].alpha = 0;
    g_SubmarineHudQuads[2].depth = FLOAT_0_01_BITS;
    g_SubmarineHudQuads[2].batches[0] = g_SubmarineHudMainAtlasA;
    g_SubmarineHudQuads[2].batches[1] = g_SubmarineHudBatchTexB;
    g_SubmarineHudQuads[2].batches[2] = g_SubmarineHudBatchTexC;
    g_SubmarineHudQuads[2].batches[3] = g_SubmarineHudBatchTexD;
    g_SubmarineHudQuads[3].x = 0xFFC8;
    g_SubmarineHudQuads[3].y = 0xFF4A;
    g_SubmarineHudQuads[3].width = 55;
    g_SubmarineHudQuads[3].height = 13;
    g_SubmarineHudQuads[3].srcX = 64;
    g_SubmarineHudQuads[3].srcY = 224;
    g_SubmarineHudQuads[3].alpha = 0;
    g_SubmarineHudQuads[3].depth = FLOAT_0_01_BITS;
    g_SubmarineHudQuads[3].batches[0] = g_SubmarineHudMainAtlasA;
    g_SubmarineHudQuads[3].batches[1] = g_SubmarineHudBatchTexB;
    g_SubmarineHudQuads[3].batches[2] = g_SubmarineHudBatchTexC;
    g_SubmarineHudQuads[3].batches[3] = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (calcAlertIdx = 0; calcAlertIdx < 4; calcAlertIdx++) {
            g_SubmarineHudQuads[calcAlertIdx].batches[0] = g_SubmarineHudMainAtlas;
            g_SubmarineHudQuads[calcAlertIdx].batches[1] = 0;
            g_SubmarineHudQuads[calcAlertIdx].batches[2] = 0;
            g_SubmarineHudQuads[calcAlertIdx].batches[3] = 0;
        }
    }
    (g_SubmarineHudQuadTable[0].x) = 0xFF00;
    (g_SubmarineHudQuadTable[0].y) = 0xFF12;
    (g_SubmarineHudQuadTable[0].width) = 27;
    (g_SubmarineHudQuadTable[0].height) = 9;
    (g_SubmarineHudQuadTable[0].srcX) = 176;
    (g_SubmarineHudQuadTable[0].srcY) = 0;
    (g_SubmarineHudQuadTable[0].alpha) = 0;
    (g_SubmarineHudQuadTable[0].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudQuadTable[0].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudQuadTable[0].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudQuadTable[0].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudQuadTable[0].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        (g_SubmarineHudQuadTable[0].batches[0]) = g_SubmarineHudMainAtlas;
        (g_SubmarineHudQuadTable[0].batches[1]) = 0;
        (g_SubmarineHudQuadTable[0].batches[2]) = 0;
        (g_SubmarineHudQuadTable[0].batches[3]) = 0;
    }
    (g_SubmarineHudQuadTable[1].x) = 0xFF12;
    (g_SubmarineHudQuadTable[1].y) = 0xFF30;
    (g_SubmarineHudQuadTable[1].width) = 10;
    (g_SubmarineHudQuadTable[1].height) = 5;
    (g_SubmarineHudQuadTable[1].srcX) = 176;
    (g_SubmarineHudQuadTable[1].srcY) = 32;
    (g_SubmarineHudQuadTable[1].alpha) = 0;
    (g_SubmarineHudQuadTable[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudQuadTable[1].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudQuadTable[1].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudQuadTable[1].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudQuadTable[1].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudQuadTable[2].x) = 0xFF26;
    (g_SubmarineHudQuadTable[2].y) = 0xFF34;
    (g_SubmarineHudQuadTable[2].width) = 2;
    (g_SubmarineHudQuadTable[2].height) = 32;
    (g_SubmarineHudQuadTable[2].srcX) = 176;
    (g_SubmarineHudQuadTable[2].srcY) = 64;
    (g_SubmarineHudQuadTable[2].alpha) = 0;
    (g_SubmarineHudQuadTable[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudQuadTable[2].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudQuadTable[2].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudQuadTable[2].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudQuadTable[2].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudQuadTable[3].x) = 0xFF26;
    (g_SubmarineHudQuadTable[3].y) = 0xFF74;
    (g_SubmarineHudQuadTable[3].width) = 2;
    (g_SubmarineHudQuadTable[3].height) = 32;
    (g_SubmarineHudQuadTable[3].srcX) = 176;
    (g_SubmarineHudQuadTable[3].srcY) = 64;
    (g_SubmarineHudQuadTable[3].alpha) = 0;
    (g_SubmarineHudQuadTable[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudQuadTable[3].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudQuadTable[3].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudQuadTable[3].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudQuadTable[3].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudQuadTable[4].x) = 0xFF26;
    (g_SubmarineHudQuadTable[4].y) = 0xFFB4;
    (g_SubmarineHudQuadTable[4].width) = 2;
    (g_SubmarineHudQuadTable[4].height) = 32;
    (g_SubmarineHudQuadTable[4].srcX) = 192;
    (g_SubmarineHudQuadTable[4].srcY) = 64;
    (g_SubmarineHudQuadTable[4].alpha) = 0;
    (g_SubmarineHudQuadTable[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudQuadTable[4].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudQuadTable[4].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudQuadTable[4].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudQuadTable[4].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudQuadTable[5].x) = 0xFF26;
    (g_SubmarineHudQuadTable[5].y) = 0xFFF4;
    (g_SubmarineHudQuadTable[5].width) = 2;
    (g_SubmarineHudQuadTable[5].height) = 24;
    (g_SubmarineHudQuadTable[5].srcX) = 200;
    (g_SubmarineHudQuadTable[5].srcY) = 64;
    (g_SubmarineHudQuadTable[5].alpha) = 0;
    (g_SubmarineHudQuadTable[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudQuadTable[5].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudQuadTable[5].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudQuadTable[5].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudQuadTable[5].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudQuadTable[6].x) = 0xFF02;
    (g_SubmarineHudQuadTable[6].y) = 0xFFB4;
    (g_SubmarineHudQuadTable[6].width) = 18;
    (g_SubmarineHudQuadTable[6].height) = 5;
    (g_SubmarineHudQuadTable[6].srcX) = 176;
    (g_SubmarineHudQuadTable[6].srcY) = 40;
    (g_SubmarineHudQuadTable[6].alpha) = 0;
    (g_SubmarineHudQuadTable[6].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudQuadTable[6].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudQuadTable[6].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudQuadTable[6].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudQuadTable[6].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudQuadTable[7].x) = 0xFEFC;
    (g_SubmarineHudQuadTable[7].y) = 32;
    (g_SubmarineHudQuadTable[7].width) = 23;
    (g_SubmarineHudQuadTable[7].height) = 16;
    (g_SubmarineHudQuadTable[7].srcX) = 176;
    (g_SubmarineHudQuadTable[7].srcY) = 48;
    (g_SubmarineHudQuadTable[7].alpha) = 0;
    (g_SubmarineHudQuadTable[7].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudQuadTable[7].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudQuadTable[7].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudQuadTable[7].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudQuadTable[7].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (q = 0; q < 7; q++) {
            (&g_SubmarineHudQuadTable[1])[q].batches[0] = g_SubmarineHudMainAtlas;
            (&g_SubmarineHudQuadTable[1])[q].batches[1] = 0;
            (&g_SubmarineHudQuadTable[1])[q].batches[2] = 0;
            (&g_SubmarineHudQuadTable[1])[q].batches[3] = 0;
        }
    }
    (g_SubmarineHudRightFrameQuads[0].x) = 200;
    (g_SubmarineHudRightFrameQuads[0].y) = 0xFF12;
    (g_SubmarineHudRightFrameQuads[0].width) = 27;
    (g_SubmarineHudRightFrameQuads[0].height) = 9;
    (g_SubmarineHudRightFrameQuads[0].srcX) = 208;
    (g_SubmarineHudRightFrameQuads[0].srcY) = 0;
    (g_SubmarineHudRightFrameQuads[0].alpha) = 0;
    (g_SubmarineHudRightFrameQuads[0].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRightFrameQuads[0].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRightFrameQuads[0].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRightFrameQuads[0].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRightFrameQuads[0].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        (g_SubmarineHudRightFrameQuads[0].batches[0]) = g_SubmarineHudMainAtlas;
        (g_SubmarineHudRightFrameQuads[0].batches[1]) = 0;
        (g_SubmarineHudRightFrameQuads[0].batches[2]) = 0;
        (g_SubmarineHudRightFrameQuads[0].batches[3]) = 0;
    }
    (g_SubmarineHudRightFrameQuads[1].x) = 220;
    (g_SubmarineHudRightFrameQuads[1].y) = 0xFF30;
    (g_SubmarineHudRightFrameQuads[1].width) = 36;
    (g_SubmarineHudRightFrameQuads[1].height) = 5;
    (g_SubmarineHudRightFrameQuads[1].srcX) = 208;
    (g_SubmarineHudRightFrameQuads[1].srcY) = 32;
    (g_SubmarineHudRightFrameQuads[1].alpha) = 0;
    (g_SubmarineHudRightFrameQuads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRightFrameQuads[1].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRightFrameQuads[1].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRightFrameQuads[1].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRightFrameQuads[1].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudRightFrameQuads[2].x) = 216;
    (g_SubmarineHudRightFrameQuads[2].y) = 0xFF34;
    (g_SubmarineHudRightFrameQuads[2].width) = 2;
    (g_SubmarineHudRightFrameQuads[2].height) = 32;
    (g_SubmarineHudRightFrameQuads[2].srcX) = 208;
    (g_SubmarineHudRightFrameQuads[2].srcY) = 64;
    (g_SubmarineHudRightFrameQuads[2].alpha) = 0;
    (g_SubmarineHudRightFrameQuads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRightFrameQuads[2].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRightFrameQuads[2].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRightFrameQuads[2].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRightFrameQuads[2].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudRightFrameQuads[3].x) = 216;
    (g_SubmarineHudRightFrameQuads[3].y) = 0xFF74;
    (g_SubmarineHudRightFrameQuads[3].width) = 2;
    (g_SubmarineHudRightFrameQuads[3].height) = 32;
    (g_SubmarineHudRightFrameQuads[3].srcX) = 208;
    (g_SubmarineHudRightFrameQuads[3].srcY) = 64;
    (g_SubmarineHudRightFrameQuads[3].alpha) = 0;
    (g_SubmarineHudRightFrameQuads[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRightFrameQuads[3].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRightFrameQuads[3].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRightFrameQuads[3].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRightFrameQuads[3].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudRightFrameQuads[4].x) = 216;
    (g_SubmarineHudRightFrameQuads[4].y) = 0xFFB4;
    (g_SubmarineHudRightFrameQuads[4].width) = 2;
    (g_SubmarineHudRightFrameQuads[4].height) = 32;
    (g_SubmarineHudRightFrameQuads[4].srcX) = 208;
    (g_SubmarineHudRightFrameQuads[4].srcY) = 64;
    (g_SubmarineHudRightFrameQuads[4].alpha) = 0;
    (g_SubmarineHudRightFrameQuads[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRightFrameQuads[4].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRightFrameQuads[4].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRightFrameQuads[4].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRightFrameQuads[4].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudRightFrameQuads[5].x) = 216;
    (g_SubmarineHudRightFrameQuads[5].y) = 0xFFF4;
    (g_SubmarineHudRightFrameQuads[5].width) = 2;
    (g_SubmarineHudRightFrameQuads[5].height) = 32;
    (g_SubmarineHudRightFrameQuads[5].srcX) = 208;
    (g_SubmarineHudRightFrameQuads[5].srcY) = 64;
    (g_SubmarineHudRightFrameQuads[5].alpha) = 0;
    (g_SubmarineHudRightFrameQuads[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRightFrameQuads[5].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRightFrameQuads[5].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRightFrameQuads[5].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRightFrameQuads[5].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudRightFrameQuads[6].x) = 220;
    (g_SubmarineHudRightFrameQuads[6].y) = 0xFFB0;
    (g_SubmarineHudRightFrameQuads[6].width) = 22;
    (g_SubmarineHudRightFrameQuads[6].height) = 5;
    (g_SubmarineHudRightFrameQuads[6].srcX) = 208;
    (g_SubmarineHudRightFrameQuads[6].srcY) = 40;
    (g_SubmarineHudRightFrameQuads[6].alpha) = 0;
    (g_SubmarineHudRightFrameQuads[6].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRightFrameQuads[6].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRightFrameQuads[6].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRightFrameQuads[6].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRightFrameQuads[6].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudRightFrameQuads[7].x) = 216;
    (g_SubmarineHudRightFrameQuads[7].y) = 48;
    (g_SubmarineHudRightFrameQuads[7].width) = 42;
    (g_SubmarineHudRightFrameQuads[7].height) = 5;
    (g_SubmarineHudRightFrameQuads[7].srcX) = 208;
    (g_SubmarineHudRightFrameQuads[7].srcY) = 48;
    (g_SubmarineHudRightFrameQuads[7].alpha) = 0;
    (g_SubmarineHudRightFrameQuads[7].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRightFrameQuads[7].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRightFrameQuads[7].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRightFrameQuads[7].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRightFrameQuads[7].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (rightIdx = 0; rightIdx < 7; rightIdx++) {
            (&g_SubmarineHudRightFrameQuads[1])[rightIdx].batches[0] = g_SubmarineHudMainAtlas;
            (&g_SubmarineHudRightFrameQuads[1])[rightIdx].batches[1] = 0;
            (&g_SubmarineHudRightFrameQuads[1])[rightIdx].batches[2] = 0;
            (&g_SubmarineHudRightFrameQuads[1])[rightIdx].batches[3] = 0;
        }
    }
    (g_SubmarineHudPitchQuads[0].x) = 0xFF30;
    (g_SubmarineHudPitchQuads[0].y) = 0xFFB4;
    (g_SubmarineHudPitchQuads[0].width) = 22;
    (g_SubmarineHudPitchQuads[0].height) = 5;
    (g_SubmarineHudPitchQuads[0].srcX) = 216;
    (g_SubmarineHudPitchQuads[0].srcY) = 16;
    (g_SubmarineHudPitchQuads[0].alpha) = *(short *)&g_SubmarineHudHeadingBits;
    (g_SubmarineHudPitchQuads[0].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudPitchQuads[0].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudPitchQuads[0].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudPitchQuads[0].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudPitchQuads[0].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudPitchQuads[1].x) = 0xFF2C;
    (g_SubmarineHudPitchQuads[1].y) = 0xFF78;
    (g_SubmarineHudPitchQuads[1].width) = 2;
    (g_SubmarineHudPitchQuads[1].height) = 32;
    (g_SubmarineHudPitchQuads[1].srcX) = 208;
    (g_SubmarineHudPitchQuads[1].srcY) = 64;
    (g_SubmarineHudPitchQuads[1].alpha) = 0;
    (g_SubmarineHudPitchQuads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudPitchQuads[1].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudPitchQuads[1].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudPitchQuads[1].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudPitchQuads[1].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudPitchQuads[2].x) = 0xFF2C;
    (g_SubmarineHudPitchQuads[2].y) = 0xFFB8;
    (g_SubmarineHudPitchQuads[2].width) = 2;
    (g_SubmarineHudPitchQuads[2].height) = 33;
    (g_SubmarineHudPitchQuads[2].srcX) = 208;
    (g_SubmarineHudPitchQuads[2].srcY) = 64;
    (g_SubmarineHudPitchQuads[2].alpha) = 0;
    (g_SubmarineHudPitchQuads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudPitchQuads[2].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudPitchQuads[2].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudPitchQuads[2].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudPitchQuads[2].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudPitchQuads[3].x) = 0xFF30;
    (g_SubmarineHudPitchQuads[3].y) = 0xFFB0;
    (g_SubmarineHudPitchQuads[3].width) = 5;
    (g_SubmarineHudPitchQuads[3].height) = 9;
    (g_SubmarineHudPitchQuads[3].srcX) = 176;
    (g_SubmarineHudPitchQuads[3].srcY) = 17;
    (g_SubmarineHudPitchQuads[3].alpha) = 0;
    (g_SubmarineHudPitchQuads[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudPitchQuads[3].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudPitchQuads[3].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudPitchQuads[3].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudPitchQuads[3].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (pitchSlot = 0; pitchSlot < 4; pitchSlot++) {
            g_SubmarineHudPitchQuads[pitchSlot].batches[0] = g_SubmarineHudMainAtlas;
            g_SubmarineHudPitchQuads[pitchSlot].batches[1] = 0;
            g_SubmarineHudPitchQuads[pitchSlot].batches[2] = 0;
            g_SubmarineHudPitchQuads[pitchSlot].batches[3] = 0;
        }
    }
    (g_SubmarineHudElements.quads2[1].x) = 0xFEEA;
    (g_SubmarineHudElements.quads2[1].y) = 0xFF2A;
    (g_SubmarineHudElements.quads2[1].width) = 30;
    (g_SubmarineHudElements.quads2[1].height) = 11;
    (g_SubmarineHudElements.quads2[1].srcX) = 182;
    (g_SubmarineHudElements.quads2[1].srcY) = 16;
    (g_SubmarineHudElements.quads2[1].alpha) = 0;
    (g_SubmarineHudElements.quads2[1].depth) = FLOAT_0_005_BITS;
    (g_SubmarineHudElements.quads2[1].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudElements.quads2[1].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudElements.quads2[1].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudElements.quads2[1].batches[3]) = g_SubmarineHudReadoutAtlasD;
    if (scaleMode != 2) {
        (g_SubmarineHudElements.quads2[1].batches[0]) = g_SubmarineHudReadoutAtlas;
        (g_SubmarineHudElements.quads2[1].batches[1]) = 0;
        (g_SubmarineHudElements.quads2[1].batches[2]) = 0;
        (g_SubmarineHudElements.quads2[1].batches[3]) = 0;
    }
    (g_SubmarineHudElements.quads2[2].x) = 0xFEF2;
    (g_SubmarineHudElements.quads2[2].y) = 0xFF30;
    (g_SubmarineHudElements.quads2[2].width) = 4;
    (g_SubmarineHudElements.quads2[2].height) = 5;
    (g_SubmarineHudElements.quads2[2].srcX) = 200;
    (g_SubmarineHudElements.quads2[2].srcY) = 56;
    (g_SubmarineHudElements.quads2[2].alpha) = 0;
    (g_SubmarineHudElements.quads2[2].depth) = FLOAT_0_005_BITS;
    (g_SubmarineHudElements.quads2[2].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudElements.quads2[2].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudElements.quads2[2].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudElements.quads2[2].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudElements.quads2[3].x) = 0xFEFA;
    (g_SubmarineHudElements.quads2[3].y) = 0xFF30;
    (g_SubmarineHudElements.quads2[3].width) = 4;
    (g_SubmarineHudElements.quads2[3].height) = 5;
    (g_SubmarineHudElements.quads2[3].srcX) = 200;
    (g_SubmarineHudElements.quads2[3].srcY) = 56;
    (g_SubmarineHudElements.quads2[3].alpha) = 0;
    (g_SubmarineHudElements.quads2[3].depth) = FLOAT_0_005_BITS;
    (g_SubmarineHudElements.quads2[3].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudElements.quads2[3].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudElements.quads2[3].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudElements.quads2[3].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudElements.quads2[4].x) = 0xFF02;
    (g_SubmarineHudElements.quads2[4].y) = 0xFF30;
    (g_SubmarineHudElements.quads2[4].width) = 4;
    (g_SubmarineHudElements.quads2[4].height) = 5;
    (g_SubmarineHudElements.quads2[4].srcX) = 200;
    (g_SubmarineHudElements.quads2[4].srcY) = 56;
    (g_SubmarineHudElements.quads2[4].alpha) = 0;
    (g_SubmarineHudElements.quads2[4].depth) = FLOAT_0_005_BITS;
    (g_SubmarineHudElements.quads2[4].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudElements.quads2[4].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudElements.quads2[4].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudElements.quads2[4].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudElements.quads2[5].x) = 0xFF0A;
    (g_SubmarineHudElements.quads2[5].y) = 0xFF30;
    (g_SubmarineHudElements.quads2[5].width) = 4;
    (g_SubmarineHudElements.quads2[5].height) = 5;
    (g_SubmarineHudElements.quads2[5].srcX) = 200;
    (g_SubmarineHudElements.quads2[5].srcY) = 56;
    (g_SubmarineHudElements.quads2[5].alpha) = 0;
    (g_SubmarineHudElements.quads2[5].depth) = FLOAT_0_005_BITS;
    (g_SubmarineHudElements.quads2[5].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudElements.quads2[5].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudElements.quads2[5].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudElements.quads2[5].batches[3]) = g_SubmarineHudReadoutAtlasD;
    if (scaleMode != 2) {
        for (readoutSlot = 0; readoutSlot < 4; readoutSlot++) {
            (&g_SubmarineHudElements.quads2[2])[readoutSlot].batches[0] = g_SubmarineHudReadoutAtlas;
            (&g_SubmarineHudElements.quads2[2])[readoutSlot].batches[1] = 0;
            (&g_SubmarineHudElements.quads2[2])[readoutSlot].batches[2] = 0;
            (&g_SubmarineHudElements.quads2[2])[readoutSlot].batches[3] = 0;
        }
    }
    (g_SubmarineHudGaugeQuads[0].x) = 220;
    (g_SubmarineHudGaugeQuads[0].y) = 0xFFAA;
    (g_SubmarineHudGaugeQuads[0].width) = 29;
    (g_SubmarineHudGaugeQuads[0].height) = 11;
    (g_SubmarineHudGaugeQuads[0].srcX) = 176;
    (g_SubmarineHudGaugeQuads[0].srcY) = 16;
    (g_SubmarineHudGaugeQuads[0].alpha) = 0;
    (g_SubmarineHudGaugeQuads[0].depth) = FLOAT_0_005_BITS;
    (g_SubmarineHudGaugeQuads[0].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudGaugeQuads[0].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudGaugeQuads[0].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudGaugeQuads[0].batches[3]) = g_SubmarineHudReadoutAtlasD;
    if (scaleMode != 2) {
        (g_SubmarineHudGaugeQuads[0].batches[0]) = g_SubmarineHudReadoutAtlas;
        (g_SubmarineHudGaugeQuads[0].batches[1]) = 0;
        (g_SubmarineHudGaugeQuads[0].batches[2]) = 0;
        (g_SubmarineHudGaugeQuads[0].batches[3]) = 0;
    }
    (g_SubmarineHudGaugeQuads[1].x) = 240;
    (g_SubmarineHudGaugeQuads[1].y) = 0xFFB0;
    (g_SubmarineHudGaugeQuads[1].width) = 4;
    (g_SubmarineHudGaugeQuads[1].height) = 5;
    (g_SubmarineHudGaugeQuads[1].srcX) = 248;
    (g_SubmarineHudGaugeQuads[1].srcY) = 56;
    (g_SubmarineHudGaugeQuads[1].alpha) = 0;
    (g_SubmarineHudGaugeQuads[1].depth) = 0;
    (g_SubmarineHudGaugeQuads[1].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudGaugeQuads[1].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudGaugeQuads[1].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudGaugeQuads[1].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudGaugeQuads[2].x) = 248;
    (g_SubmarineHudGaugeQuads[2].y) = 0xFFB0;
    (g_SubmarineHudGaugeQuads[2].width) = 4;
    (g_SubmarineHudGaugeQuads[2].height) = 5;
    (g_SubmarineHudGaugeQuads[2].srcX) = 200;
    (g_SubmarineHudGaugeQuads[2].srcY) = 56;
    (g_SubmarineHudGaugeQuads[2].alpha) = 0;
    (g_SubmarineHudGaugeQuads[2].depth) = 0;
    (g_SubmarineHudGaugeQuads[2].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudGaugeQuads[2].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudGaugeQuads[2].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudGaugeQuads[2].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudGaugeQuads[3].x) = 256;
    (g_SubmarineHudGaugeQuads[3].y) = 0xFFB0;
    (g_SubmarineHudGaugeQuads[3].width) = 4;
    (g_SubmarineHudGaugeQuads[3].height) = 5;
    (g_SubmarineHudGaugeQuads[3].srcX) = 200;
    (g_SubmarineHudGaugeQuads[3].srcY) = 56;
    (g_SubmarineHudGaugeQuads[3].alpha) = 0;
    (g_SubmarineHudGaugeQuads[3].depth) = 0;
    (g_SubmarineHudGaugeQuads[3].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudGaugeQuads[3].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudGaugeQuads[3].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudGaugeQuads[3].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudGaugeQuads[4].x) = 264;
    (g_SubmarineHudGaugeQuads[4].y) = 0xFFB0;
    (g_SubmarineHudGaugeQuads[4].width) = 4;
    (g_SubmarineHudGaugeQuads[4].height) = 5;
    (g_SubmarineHudGaugeQuads[4].srcX) = 200;
    (g_SubmarineHudGaugeQuads[4].srcY) = 56;
    (g_SubmarineHudGaugeQuads[4].alpha) = 0;
    (g_SubmarineHudGaugeQuads[4].depth) = 0;
    (g_SubmarineHudGaugeQuads[4].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudGaugeQuads[4].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudGaugeQuads[4].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudGaugeQuads[4].batches[3]) = g_SubmarineHudReadoutAtlasD;
    if (scaleMode != 2) {
        for (gaugeIdx = 0; gaugeIdx < 4; gaugeIdx++) {
            (&g_SubmarineHudGaugeQuads[1])[gaugeIdx].batches[0] = g_SubmarineHudReadoutAtlas;
            (&g_SubmarineHudGaugeQuads[1])[gaugeIdx].batches[1] = 0;
            (&g_SubmarineHudGaugeQuads[1])[gaugeIdx].batches[2] = 0;
            (&g_SubmarineHudGaugeQuads[1])[gaugeIdx].batches[3] = 0;
        }
    }
    (g_SubmarineHudYawStripQuads[0].x) = 0xFF83;
    (g_SubmarineHudYawStripQuads[0].y) = 0xFF11;
    (g_SubmarineHudYawStripQuads[0].width) = 125;
    (g_SubmarineHudYawStripQuads[0].height) = 9;
    (g_SubmarineHudYawStripQuads[0].srcX) = 0;
    (g_SubmarineHudYawStripQuads[0].srcY) = 241;
    (g_SubmarineHudYawStripQuads[0].alpha) = 0;
    (g_SubmarineHudYawStripQuads[0].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudYawStripQuads[0].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudYawStripQuads[0].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudYawStripQuads[0].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudYawStripQuads[0].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudYawStripQuads[1].x) = 0xFF83;
    (g_SubmarineHudYawStripQuads[1].y) = 0xFF11;
    (g_SubmarineHudYawStripQuads[1].width) = 125;
    (g_SubmarineHudYawStripQuads[1].height) = 9;
    (g_SubmarineHudYawStripQuads[1].srcX) = 0;
    (g_SubmarineHudYawStripQuads[1].srcY) = 241;
    (g_SubmarineHudYawStripQuads[1].alpha) = 0;
    (g_SubmarineHudYawStripQuads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudYawStripQuads[1].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudYawStripQuads[1].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudYawStripQuads[1].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudYawStripQuads[1].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudYawStripQuads[2].x) = 0xFF82;
    (g_SubmarineHudYawStripQuads[2].y) = 0xFF10;
    (g_SubmarineHudYawStripQuads[2].width) = 0x7F;
    (g_SubmarineHudYawStripQuads[2].height) = 13;
    (g_SubmarineHudYawStripQuads[2].srcX) = 0;
    (g_SubmarineHudYawStripQuads[2].srcY) = 0;
    (g_SubmarineHudYawStripQuads[2].alpha) = 0;
    (g_SubmarineHudYawStripQuads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudYawStripQuads[2].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudYawStripQuads[2].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudYawStripQuads[2].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudYawStripQuads[2].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (headingIndex = 0; headingIndex < 3; headingIndex++) {
            g_SubmarineHudYawStripQuads[headingIndex].batches[0] = g_SubmarineHudMainAtlas;
            g_SubmarineHudYawStripQuads[headingIndex].batches[1] = 0;
            g_SubmarineHudYawStripQuads[headingIndex].batches[2] = 0;
            g_SubmarineHudYawStripQuads[headingIndex].batches[3] = 0;
        }
    }
    g_SubmarineHudGauge.x = 0xFFFFFF82;
    (g_SubmarineHudGauge.y) = 150;
    (g_SubmarineHudGauge.width) = 0x7F;
    (g_SubmarineHudGauge.height) = 11;
    (g_SubmarineHudGauge.colorTopLeft) = 0xFFFF0000;
    (g_SubmarineHudGauge.colorTopRight) = 0xFFFF0000;
    (g_SubmarineHudGauge.gaugeValue) = 0xFF00FF00;
    (g_SubmarineHudGauge.frameValue) = 0xFF00FF00;
    (g_SubmarineHudGauge.colorBottomLeft) = 0;
    (g_SubmarineHudGauge.colorBottomRight) = FLOAT_0_01_BITS;
    (g_SubmarineHudRectQuad.x) = 0xFF7E;
    (g_SubmarineHudRectQuad.y) = 146;
    (g_SubmarineHudRectQuad.width) = 131;
    (g_SubmarineHudRectQuad.height) = 15;
    (g_SubmarineHudRectQuad.srcX) = 0;
    (g_SubmarineHudRectQuad.srcY) = 16;
    (g_SubmarineHudRectQuad.alpha) = 0;
    (g_SubmarineHudRectQuad.depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRectQuad.batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudRectQuad.batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudRectQuad.batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudRectQuad.batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        (g_SubmarineHudRectQuad.batches[0]) = g_SubmarineHudMainAtlas;
        (g_SubmarineHudRectQuad.batches[1]) = 0;
        (g_SubmarineHudRectQuad.batches[2]) = 0;
        (g_SubmarineHudRectQuad.batches[3]) = 0;
    }
    (g_SubmarineHudAnimTable.anim0SrcX) = 0;
    (g_SubmarineHudAnimTable.anim0SrcY) = 106;
    (g_SubmarineHudAnimTable.anim1SrcX) = 0;
    (g_SubmarineHudAnimTable.anim1SrcY) = 132;
    (g_SubmarineHudAnimTable.anim2SrcX) = 0;
    (g_SubmarineHudAnimTable.anim2SrcY) = 158;
    (g_SubmarineHudAnimTable.anim3SrcX) = 48;
    (g_SubmarineHudAnimTable.anim3SrcY) = 106;
    (g_SubmarineHudAnimTable.anim4SrcX) = 48;
    (g_SubmarineHudAnimTable.anim4SrcY) = 132;
    (g_SubmarineHudAnimTable.anim5SrcX) = 48;
    (g_SubmarineHudAnimTable.anim5SrcY) = 158;
    (g_SubmarineHudLayout.quads[0].x) = 168;
    (g_SubmarineHudLayout.quads[0].y) = 82;
    (g_SubmarineHudLayout.quads[0].width) = 43;
    (g_SubmarineHudLayout.quads[0].height) = 43;
    (g_SubmarineHudLayout.quads[0].srcX) = 96;
    (g_SubmarineHudLayout.quads[0].srcY) = 32;
    (g_SubmarineHudLayout.quads[0].alpha) = 0;
    (g_SubmarineHudLayout.quads[0].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudLayout.quads[0].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudLayout.quads[0].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudLayout.quads[0].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudLayout.quads[0].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        (g_SubmarineHudLayout.quads[0].batches[0]) = g_SubmarineHudMainAtlas;
        (g_SubmarineHudLayout.quads[0].batches[1]) = 0;
        (g_SubmarineHudLayout.quads[0].batches[2]) = 0;
        (g_SubmarineHudLayout.quads[0].batches[3]) = 0;
    }
    (g_SubmarineHudLayout.quads[1].x) = 164;
    (g_SubmarineHudLayout.quads[1].y) = 78;
    (g_SubmarineHudLayout.quads[1].width) = 47;
    (g_SubmarineHudLayout.quads[1].height) = 49;
    (g_SubmarineHudLayout.quads[1].srcX) = 48;
    (g_SubmarineHudLayout.quads[1].srcY) = 32;
    (g_SubmarineHudLayout.quads[1].alpha) = 0;
    (g_SubmarineHudLayout.quads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudLayout.quads[1].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudLayout.quads[1].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudLayout.quads[1].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudLayout.quads[1].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        (g_SubmarineHudLayout.quads[1].batches[0]) = g_SubmarineHudMainAtlas;
        (g_SubmarineHudLayout.quads[1].batches[1]) = 0;
        (g_SubmarineHudLayout.quads[1].batches[2]) = 0;
        (g_SubmarineHudLayout.quads[1].batches[3]) = 0;
    }
    (g_SubmarineHudLayout.bar.x) = 164;
    (g_SubmarineHudLayout.bar.y) = 98;
    (g_SubmarineHudLayout.bar.width) = 8;
    (g_SubmarineHudLayout.bar.height) = 26;
    (g_SubmarineHudLayout.bar.srcX) = 0;
    (g_SubmarineHudLayout.bar.srcY) = 106;
    (g_SubmarineHudLayout.bar.alpha) = 0;
    (g_SubmarineHudLayout.bar.colors[0]) = 0x7F004000;
    (g_SubmarineHudLayout.bar.colors[1]) = 0x7F004000;
    (g_SubmarineHudLayout.bar.colors[2]) = 0x7F40FF00;
    (g_SubmarineHudLayout.bar.colors[3]) = 0x7F40FF00;
    (g_SubmarineHudLayout.bar.depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudLayout.bar.batches[0]) = g_SubmarineHudBarAtlasA;
    (g_SubmarineHudLayout.bar.batches[1]) = g_SubmarineHudRenderObjectB;
    (g_SubmarineHudLayout.bar.batches[2]) = g_SubmarineHudBarAtlasC;
    (g_SubmarineHudLayout.bar.batches[3]) = g_SubmarineHudBarAtlasD;
    if (scaleMode != 2) {
        (g_SubmarineHudLayout.bar.batches[0]) = g_SubmarineHudBarAtlas;
        (g_SubmarineHudLayout.bar.batches[1]) = 0;
        (g_SubmarineHudLayout.bar.batches[2]) = 0;
        (g_SubmarineHudLayout.bar.batches[3]) = 0;
    }
    (g_SubmarineHudElements.quads[0].x) = 0xFF06;
    (g_SubmarineHudElements.quads[0].y) = 82;
    (g_SubmarineHudElements.quads[0].width) = 43;
    (g_SubmarineHudElements.quads[0].height) = 43;
    (g_SubmarineHudElements.quads[0].srcX) = 96;
    (g_SubmarineHudElements.quads[0].srcY) = 32;
    (g_SubmarineHudElements.quads[0].alpha) = 0;
    (g_SubmarineHudElements.quads[0].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudElements.quads[0].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudElements.quads[0].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudElements.quads[0].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudElements.quads[0].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        (g_SubmarineHudElements.quads[0].batches[0]) = g_SubmarineHudMainAtlas;
        (g_SubmarineHudElements.quads[0].batches[1]) = 0;
        (g_SubmarineHudElements.quads[0].batches[2]) = 0;
        (g_SubmarineHudElements.quads[0].batches[3]) = 0;
    }
    (g_SubmarineHudElements.quads[1].x) = 0xFF02;
    (g_SubmarineHudElements.quads[1].y) = 78;
    (g_SubmarineHudElements.quads[1].width) = 47;
    (g_SubmarineHudElements.quads[1].height) = 48;
    (g_SubmarineHudElements.quads[1].srcX) = 0;
    (g_SubmarineHudElements.quads[1].srcY) = 32;
    (g_SubmarineHudElements.quads[1].alpha) = 0;
    (g_SubmarineHudElements.quads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudElements.quads[1].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudElements.quads[1].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudElements.quads[1].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudElements.quads[1].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        (g_SubmarineHudElements.quads[1].batches[0]) = g_SubmarineHudMainAtlas;
        (g_SubmarineHudElements.quads[1].batches[1]) = 0;
        (g_SubmarineHudElements.quads[1].batches[2]) = 0;
        (g_SubmarineHudElements.quads[1].batches[3]) = 0;
    }
    (g_SubmarineHudElements.bar.x) = 0xFF02;
    (g_SubmarineHudElements.bar.y) = 86;
    (g_SubmarineHudElements.bar.width) = 4;
    (g_SubmarineHudElements.bar.height) = 25;
    (g_SubmarineHudElements.bar.srcX) = 112;
    (g_SubmarineHudElements.bar.srcY) = 100;
    (g_SubmarineHudElements.bar.alpha) = 0;
    (g_SubmarineHudElements.bar.colors[0]) = 0x7F404000;
    (g_SubmarineHudElements.bar.colors[1]) = 0x7F404000;
    (g_SubmarineHudElements.bar.colors[2]) = 0x7FFFFF00;
    (g_SubmarineHudElements.bar.colors[3]) = 0x7FFFFF00;
    (g_SubmarineHudElements.bar.depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudElements.bar.batches[0]) = g_SubmarineHudBarAtlasA;
    (g_SubmarineHudElements.bar.batches[1]) = g_SubmarineHudRenderObjectB;
    (g_SubmarineHudElements.bar.batches[2]) = g_SubmarineHudBarAtlasC;
    (g_SubmarineHudElements.bar.batches[3]) = g_SubmarineHudBarAtlasD;
    if (scaleMode != 2) {
        (g_SubmarineHudElements.bar.batches[0]) = g_SubmarineHudBarAtlas;
        (g_SubmarineHudElements.bar.batches[1]) = 0;
        (g_SubmarineHudElements.bar.batches[2]) = 0;
        (g_SubmarineHudElements.bar.batches[3]) = 0;
    }
    (g_SubmarineHudElements.quads2[0].x) = 0xFF02;
    (g_SubmarineHudElements.quads2[0].y) = 146;
    (g_SubmarineHudElements.quads2[0].width) = 28;
    (g_SubmarineHudElements.quads2[0].height) = 15;
    (g_SubmarineHudElements.quads2[0].srcX) = 216;
    (g_SubmarineHudElements.quads2[0].srcY) = 64;
    (g_SubmarineHudElements.quads2[0].alpha) = 0;
    (g_SubmarineHudElements.quads2[0].depth) = FLOAT_0_005_BITS;
    (g_SubmarineHudElements.quads2[0].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudElements.quads2[0].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudElements.quads2[0].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudElements.quads2[0].batches[3]) = g_SubmarineHudReadoutAtlasD;
    if (scaleMode != 2) {
        (g_SubmarineHudElements.quads2[0].batches[0]) = g_SubmarineHudReadoutAtlas;
        (g_SubmarineHudElements.quads2[0].batches[1]) = 0;
        (g_SubmarineHudElements.quads2[0].batches[2]) = 0;
        (g_SubmarineHudElements.quads2[0].batches[3]) = 0;
    }
    (g_SubmarineHudValueQuads[4].x) = 0xFEDC;
    (g_SubmarineHudValueQuads[4].y) = 134;
    (g_SubmarineHudValueQuads[4].width) = 14;
    (g_SubmarineHudValueQuads[4].height) = 13;
    (g_SubmarineHudValueQuads[4].srcX) = 64;
    (g_SubmarineHudValueQuads[4].srcY) = 80;
    (g_SubmarineHudValueQuads[4].alpha) = 0;
    (g_SubmarineHudValueQuads[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudValueQuads[4].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudValueQuads[4].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudValueQuads[4].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudValueQuads[4].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudValueQuads[5].x) = 0xFEDC;
    (g_SubmarineHudValueQuads[5].y) = 134;
    (g_SubmarineHudValueQuads[5].width) = 14;
    (g_SubmarineHudValueQuads[5].height) = 13;
    (g_SubmarineHudValueQuads[5].srcX) = 80;
    (g_SubmarineHudValueQuads[5].srcY) = 80;
    (g_SubmarineHudValueQuads[5].alpha) = 0;
    (g_SubmarineHudValueQuads[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudValueQuads[5].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudValueQuads[5].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudValueQuads[5].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudValueQuads[5].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (valueTail = 0; valueTail < 2; valueTail++) {
            (&g_SubmarineHudValueQuads[4])[valueTail].batches[0] = g_SubmarineHudMainAtlas;
            (&g_SubmarineHudValueQuads[4])[valueTail].batches[1] = 0;
            (&g_SubmarineHudValueQuads[4])[valueTail].batches[2] = 0;
            (&g_SubmarineHudValueQuads[4])[valueTail].batches[3] = 0;
        }
    }
    (g_SubmarineHudValueQuads[0].x) = 260;
    (g_SubmarineHudValueQuads[0].y) = 80;
    (g_SubmarineHudValueQuads[0].width) = 23;
    (g_SubmarineHudValueQuads[0].height) = 23;
    (g_SubmarineHudValueQuads[0].srcX) = 0;
    (g_SubmarineHudValueQuads[0].srcY) = 80;
    (g_SubmarineHudValueQuads[0].alpha) = 0;
    (g_SubmarineHudValueQuads[0].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudValueQuads[0].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudValueQuads[0].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudValueQuads[0].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudValueQuads[0].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudValueQuads[1].x) = 260;
    (g_SubmarineHudValueQuads[1].y) = 80;
    (g_SubmarineHudValueQuads[1].width) = 23;
    (g_SubmarineHudValueQuads[1].height) = 23;
    (g_SubmarineHudValueQuads[1].srcX) = 144;
    (g_SubmarineHudValueQuads[1].srcY) = 0;
    (g_SubmarineHudValueQuads[1].alpha) = 0;
    (g_SubmarineHudValueQuads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudValueQuads[1].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudValueQuads[1].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudValueQuads[1].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudValueQuads[1].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (vhead = 0; vhead < 2; vhead++) {
            g_SubmarineHudValueQuads[vhead].batches[0] = g_SubmarineHudMainAtlas;
            g_SubmarineHudValueQuads[vhead].batches[1] = 0;
            g_SubmarineHudValueQuads[vhead].batches[2] = 0;
            g_SubmarineHudValueQuads[vhead].batches[3] = 0;
        }
    }
    (g_SubmarineHudValueQuads[2].x) = 0xFECE;
    (g_SubmarineHudValueQuads[2].y) = 80;
    (g_SubmarineHudValueQuads[2].width) = 21;
    (g_SubmarineHudValueQuads[2].height) = 21;
    (g_SubmarineHudValueQuads[2].srcX) = 32;
    (g_SubmarineHudValueQuads[2].srcY) = 80;
    (g_SubmarineHudValueQuads[2].alpha) = 0;
    (g_SubmarineHudValueQuads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudValueQuads[2].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudValueQuads[2].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudValueQuads[2].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudValueQuads[2].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudValueQuads[3].x) = 0xFECE;
    (g_SubmarineHudValueQuads[3].y) = 80;
    (g_SubmarineHudValueQuads[3].width) = 20;
    (g_SubmarineHudValueQuads[3].height) = 16;
    (g_SubmarineHudValueQuads[3].srcX) = 96;
    (g_SubmarineHudValueQuads[3].srcY) = 129;
    (g_SubmarineHudValueQuads[3].alpha) = 0;
    (g_SubmarineHudValueQuads[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudValueQuads[3].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudValueQuads[3].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudValueQuads[3].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudValueQuads[3].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (valueMid = 0; valueMid < 2; valueMid++) {
            (&g_SubmarineHudValueQuads[2])[valueMid].batches[0] = g_SubmarineHudMainAtlas;
            (&g_SubmarineHudValueQuads[2])[valueMid].batches[1] = 0;
            (&g_SubmarineHudValueQuads[2])[valueMid].batches[2] = 0;
            (&g_SubmarineHudValueQuads[2])[valueMid].batches[3] = 0;
        }
    }
    (g_SubmarineHudCooldownQuads[0].x) = 120;
    (g_SubmarineHudCooldownQuads[0].y) = 88;
    (g_SubmarineHudCooldownQuads[0].width) = 12;
    (g_SubmarineHudCooldownQuads[0].height) = 12;
    (g_SubmarineHudCooldownQuads[0].srcX) = 128;
    (g_SubmarineHudCooldownQuads[0].srcY) = 0;
    (g_SubmarineHudCooldownQuads[0].alpha) = 0;
    (g_SubmarineHudCooldownQuads[0].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudCooldownQuads[0].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudCooldownQuads[0].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudCooldownQuads[0].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudCooldownQuads[0].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudCooldownQuads[1].x) = 0xFF70;
    (g_SubmarineHudCooldownQuads[1].y) = 88;
    (g_SubmarineHudCooldownQuads[1].width) = 12;
    (g_SubmarineHudCooldownQuads[1].height) = 12;
    (g_SubmarineHudCooldownQuads[1].srcX) = 128;
    (g_SubmarineHudCooldownQuads[1].srcY) = 0;
    (g_SubmarineHudCooldownQuads[1].alpha) = 0;
    (g_SubmarineHudCooldownQuads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudCooldownQuads[1].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudCooldownQuads[1].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudCooldownQuads[1].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudCooldownQuads[1].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudCooldownQuads[2].x) = 94;
    (g_SubmarineHudCooldownQuads[2].y) = 110;
    (g_SubmarineHudCooldownQuads[2].width) = 12;
    (g_SubmarineHudCooldownQuads[2].height) = 12;
    (g_SubmarineHudCooldownQuads[2].srcX) = 128;
    (g_SubmarineHudCooldownQuads[2].srcY) = 0;
    (g_SubmarineHudCooldownQuads[2].alpha) = 0;
    (g_SubmarineHudCooldownQuads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudCooldownQuads[2].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudCooldownQuads[2].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudCooldownQuads[2].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudCooldownQuads[2].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudCooldownQuads[3].x) = 0xFF8A;
    (g_SubmarineHudCooldownQuads[3].y) = 110;
    (g_SubmarineHudCooldownQuads[3].width) = 12;
    (g_SubmarineHudCooldownQuads[3].height) = 12;
    (g_SubmarineHudCooldownQuads[3].srcX) = 128;
    (g_SubmarineHudCooldownQuads[3].srcY) = 0;
    (g_SubmarineHudCooldownQuads[3].alpha) = 0;
    (g_SubmarineHudCooldownQuads[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudCooldownQuads[3].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudCooldownQuads[3].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudCooldownQuads[3].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudCooldownQuads[3].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudCooldownQuads[4].x) = 124;
    (g_SubmarineHudCooldownQuads[4].y) = 92;
    (g_SubmarineHudCooldownQuads[4].width) = 8;
    (g_SubmarineHudCooldownQuads[4].height) = 8;
    (g_SubmarineHudCooldownQuads[4].srcX) = 136;
    (g_SubmarineHudCooldownQuads[4].srcY) = 16;
    (g_SubmarineHudCooldownQuads[4].alpha) = 3;
    (g_SubmarineHudCooldownQuads[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudCooldownQuads[4].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudCooldownQuads[4].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudCooldownQuads[4].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudCooldownQuads[4].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudCooldownQuads[5].x) = 0xFF74;
    (g_SubmarineHudCooldownQuads[5].y) = 92;
    (g_SubmarineHudCooldownQuads[5].width) = 8;
    (g_SubmarineHudCooldownQuads[5].height) = 8;
    (g_SubmarineHudCooldownQuads[5].srcX) = 136;
    (g_SubmarineHudCooldownQuads[5].srcY) = 16;
    (g_SubmarineHudCooldownQuads[5].alpha) = 3;
    (g_SubmarineHudCooldownQuads[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudCooldownQuads[5].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudCooldownQuads[5].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudCooldownQuads[5].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudCooldownQuads[5].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudCooldownQuads[6].x) = 98;
    (g_SubmarineHudCooldownQuads[6].y) = 114;
    (g_SubmarineHudCooldownQuads[6].width) = 8;
    (g_SubmarineHudCooldownQuads[6].height) = 8;
    (g_SubmarineHudCooldownQuads[6].srcX) = 136;
    (g_SubmarineHudCooldownQuads[6].srcY) = 16;
    (g_SubmarineHudCooldownQuads[6].alpha) = 3;
    (g_SubmarineHudCooldownQuads[6].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudCooldownQuads[6].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudCooldownQuads[6].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudCooldownQuads[6].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudCooldownQuads[6].batches[3]) = g_SubmarineHudBatchTexD;
    (g_SubmarineHudCooldownQuads[7].x) = 0xFF8E;
    (g_SubmarineHudCooldownQuads[7].y) = 114;
    (g_SubmarineHudCooldownQuads[7].width) = 8;
    (g_SubmarineHudCooldownQuads[7].height) = 8;
    (g_SubmarineHudCooldownQuads[7].srcX) = 136;
    (g_SubmarineHudCooldownQuads[7].srcY) = 16;
    (g_SubmarineHudCooldownQuads[7].alpha) = 3;
    (g_SubmarineHudCooldownQuads[7].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudCooldownQuads[7].batches[0]) = g_SubmarineHudMainAtlasA;
    (g_SubmarineHudCooldownQuads[7].batches[1]) = g_SubmarineHudBatchTexB;
    (g_SubmarineHudCooldownQuads[7].batches[2]) = g_SubmarineHudBatchTexC;
    (g_SubmarineHudCooldownQuads[7].batches[3]) = g_SubmarineHudBatchTexD;
    if (scaleMode != 2) {
        for (coolQuad = 0; coolQuad < 8; coolQuad++) {
            g_SubmarineHudCooldownQuads[coolQuad].batches[0] = g_SubmarineHudMainAtlas;
            g_SubmarineHudCooldownQuads[coolQuad].batches[1] = 0;
            g_SubmarineHudCooldownQuads[coolQuad].batches[2] = 0;
            g_SubmarineHudCooldownQuads[coolQuad].batches[3] = 0;
        }
    }
    g_SubmarineHudTallyElements.single.x = 0xFF80;
    g_SubmarineHudTallyElements.single.y = 0xFF64;
    g_SubmarineHudTallyElements.single.width = 128;
    g_SubmarineHudTallyElements.single.height = 60;
    g_SubmarineHudTallyElements.single.srcX = 0;
    g_SubmarineHudTallyElements.single.srcY = 96;
    g_SubmarineHudTallyElements.single.alpha = 4;
    g_SubmarineHudTallyElements.single.depth = FLOAT_0_01_BITS;
    g_SubmarineHudTallyElements.single.batches[0] = g_SubmarineHudRenderState.renderObjectAlt;
    g_SubmarineHudTallyElements.single.batches[1] = g_SubmarineHudSingleTextAtlasB;
    g_SubmarineHudTallyElements.single.batches[2] = g_SubmarineHudSingleTextAtlasC;
    g_SubmarineHudTallyElements.single.batches[3] = g_SubmarineHudRenderObjectD;
    if (scaleMode != 2) {
        g_SubmarineHudTallyElements.single.batches[0] = g_SubmarineHudRenderState.renderObject;
        g_SubmarineHudTallyElements.single.batches[1] = 0;
        g_SubmarineHudTallyElements.single.batches[2] = 0;
        g_SubmarineHudTallyElements.single.batches[3] = 0;
    }
    (g_SubmarineHudDigitQuads[0].x) = 208;
    (g_SubmarineHudDigitQuads[0].y) = 142;
    (g_SubmarineHudDigitQuads[0].width) = 47;
    (g_SubmarineHudDigitQuads[0].height) = 16;
    (g_SubmarineHudDigitQuads[0].srcX) = 120;
    (g_SubmarineHudDigitQuads[0].srcY) = 200;
    (g_SubmarineHudDigitQuads[0].alpha) = 7;
    (g_SubmarineHudDigitQuads[0].depth) = FLOAT_0_005_BITS;
    (g_SubmarineHudDigitQuads[0].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudDigitQuads[0].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudDigitQuads[0].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudDigitQuads[0].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudDigitQuads[1].x) = 248;
    (g_SubmarineHudDigitQuads[1].y) = 150;
    (g_SubmarineHudDigitQuads[1].width) = 8;
    (g_SubmarineHudDigitQuads[1].height) = 8;
    (g_SubmarineHudDigitQuads[1].srcX) = 200;
    (g_SubmarineHudDigitQuads[1].srcY) = 192;
    (g_SubmarineHudDigitQuads[1].alpha) = 7;
    (g_SubmarineHudDigitQuads[1].depth) = 0;
    (g_SubmarineHudDigitQuads[1].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudDigitQuads[1].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudDigitQuads[1].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudDigitQuads[1].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudDigitQuads[2].x) = 216;
    (g_SubmarineHudDigitQuads[2].y) = 150;
    (g_SubmarineHudDigitQuads[2].width) = 8;
    (g_SubmarineHudDigitQuads[2].height) = 8;
    (g_SubmarineHudDigitQuads[2].srcX) = 120;
    (g_SubmarineHudDigitQuads[2].srcY) = 192;
    (g_SubmarineHudDigitQuads[2].alpha) = 7;
    (g_SubmarineHudDigitQuads[2].depth) = 0;
    (g_SubmarineHudDigitQuads[2].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudDigitQuads[2].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudDigitQuads[2].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudDigitQuads[2].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudDigitQuads[3].x) = 234;
    (g_SubmarineHudDigitQuads[3].y) = 150;
    (g_SubmarineHudDigitQuads[3].width) = 8;
    (g_SubmarineHudDigitQuads[3].height) = 8;
    (g_SubmarineHudDigitQuads[3].srcX) = 120;
    (g_SubmarineHudDigitQuads[3].srcY) = 192;
    (g_SubmarineHudDigitQuads[3].alpha) = 7;
    (g_SubmarineHudDigitQuads[3].depth) = 0;
    (g_SubmarineHudDigitQuads[3].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudDigitQuads[3].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudDigitQuads[3].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudDigitQuads[3].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudDigitQuads[4].x) = 260;
    (g_SubmarineHudDigitQuads[4].y) = 150;
    (g_SubmarineHudDigitQuads[4].width) = 8;
    (g_SubmarineHudDigitQuads[4].height) = 8;
    (g_SubmarineHudDigitQuads[4].srcX) = 120;
    (g_SubmarineHudDigitQuads[4].srcY) = 192;
    (g_SubmarineHudDigitQuads[4].alpha) = 7;
    (g_SubmarineHudDigitQuads[4].depth) = 0;
    (g_SubmarineHudDigitQuads[4].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudDigitQuads[4].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudDigitQuads[4].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudDigitQuads[4].batches[3]) = g_SubmarineHudReadoutAtlasD;
    (g_SubmarineHudDigitQuads[5].x) = 278;
    (g_SubmarineHudDigitQuads[5].y) = 150;
    (g_SubmarineHudDigitQuads[5].width) = 8;
    (g_SubmarineHudDigitQuads[5].height) = 8;
    (g_SubmarineHudDigitQuads[5].srcX) = 120;
    (g_SubmarineHudDigitQuads[5].srcY) = 192;
    (g_SubmarineHudDigitQuads[5].alpha) = 7;
    (g_SubmarineHudDigitQuads[5].depth) = 0;
    (g_SubmarineHudDigitQuads[5].batches[0]) = g_SubmarineHudReadoutAtlasA;
    (g_SubmarineHudDigitQuads[5].batches[1]) = g_SubmarineHudReadoutAtlasB;
    (g_SubmarineHudDigitQuads[5].batches[2]) = g_SubmarineHudRenderObjectA;
    (g_SubmarineHudDigitQuads[5].batches[3]) = g_SubmarineHudReadoutAtlasD;
    if (scaleMode != 2) {
        for (numberIdx = 0; numberIdx < 6; numberIdx++) {
            g_SubmarineHudDigitQuads[numberIdx].batches[0] = g_SubmarineHudReadoutAtlas;
            g_SubmarineHudDigitQuads[numberIdx].batches[1] = 0;
            g_SubmarineHudDigitQuads[numberIdx].batches[2] = 0;
            g_SubmarineHudDigitQuads[numberIdx].batches[3] = 0;
        }
    }
    g_SubmarineHudFifteenElements.quads[0].x = 0xFF90;
    g_SubmarineHudFifteenElements.quads[0].y = 0xFF70;
    g_SubmarineHudFifteenElements.quads[0].width = 25;
    g_SubmarineHudFifteenElements.quads[0].height = 19;
    g_SubmarineHudFifteenElements.quads[0].srcX = 223;
    g_SubmarineHudFifteenElements.quads[0].srcY = 0;
    g_SubmarineHudFifteenElements.quads[0].alpha = 5;
    g_SubmarineHudFifteenElements.quads[0].depth = FLOAT_0_01_BITS;
    g_SubmarineHudFifteenElements.quads[0].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudFifteenElements.quads[0].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudFifteenElements.quads[0].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudFifteenElements.quads[0].batches[3] = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[1].x) = 0xFFC2;
    (g_SubmarineHudFifteenElements.quads[1].y) = 0xFF70;
    (g_SubmarineHudFifteenElements.quads[1].width) = 11;
    (g_SubmarineHudFifteenElements.quads[1].height) = 19;
    (g_SubmarineHudFifteenElements.quads[1].srcX) = 155;
    (g_SubmarineHudFifteenElements.quads[1].srcY) = 0;
    (g_SubmarineHudFifteenElements.quads[1].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[1].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[1].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[1].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[1].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[2].x) = 0xFFDA;
    (g_SubmarineHudFifteenElements.quads[2].y) = 0xFF70;
    (g_SubmarineHudFifteenElements.quads[2].width) = 14;
    (g_SubmarineHudFifteenElements.quads[2].height) = 20;
    (g_SubmarineHudFifteenElements.quads[2].srcX) = 99;
    (g_SubmarineHudFifteenElements.quads[2].srcY) = 22;
    (g_SubmarineHudFifteenElements.quads[2].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[2].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[2].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[2].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[2].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[3].x) = 0xFFFA;
    (g_SubmarineHudFifteenElements.quads[3].y) = 0xFF70;
    (g_SubmarineHudFifteenElements.quads[3].width) = 14;
    (g_SubmarineHudFifteenElements.quads[3].height) = 20;
    (g_SubmarineHudFifteenElements.quads[3].srcX) = 99;
    (g_SubmarineHudFifteenElements.quads[3].srcY) = 22;
    (g_SubmarineHudFifteenElements.quads[3].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[3].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[3].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[3].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[3].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[4].x) = 24;
    (g_SubmarineHudFifteenElements.quads[4].y) = 0xFF70;
    (g_SubmarineHudFifteenElements.quads[4].width) = 11;
    (g_SubmarineHudFifteenElements.quads[4].height) = 19;
    (g_SubmarineHudFifteenElements.quads[4].srcX) = 155;
    (g_SubmarineHudFifteenElements.quads[4].srcY) = 0;
    (g_SubmarineHudFifteenElements.quads[4].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[4].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[4].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[4].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[4].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[5].x) = 46;
    (g_SubmarineHudFifteenElements.quads[5].y) = 0xFF70;
    (g_SubmarineHudFifteenElements.quads[5].width) = 20;
    (g_SubmarineHudFifteenElements.quads[5].height) = 20;
    (g_SubmarineHudFifteenElements.quads[5].srcX) = 20;
    (g_SubmarineHudFifteenElements.quads[5].srcY) = 22;
    (g_SubmarineHudFifteenElements.quads[5].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[5].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[5].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[5].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[5].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[6].x) = 86;
    (g_SubmarineHudFifteenElements.quads[6].y) = 0xFF70;
    (g_SubmarineHudFifteenElements.quads[6].width) = 19;
    (g_SubmarineHudFifteenElements.quads[6].height) = 20;
    (g_SubmarineHudFifteenElements.quads[6].srcX) = 0;
    (g_SubmarineHudFifteenElements.quads[6].srcY) = 22;
    (g_SubmarineHudFifteenElements.quads[6].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[6].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[6].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[6].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[6].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[6].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[7].x) = 0xFF6C;
    (g_SubmarineHudFifteenElements.quads[7].y) = 0xFF9E;
    (g_SubmarineHudFifteenElements.quads[7].width) = 18;
    (g_SubmarineHudFifteenElements.quads[7].height) = 20;
    (g_SubmarineHudFifteenElements.quads[7].srcX) = 39;
    (g_SubmarineHudFifteenElements.quads[7].srcY) = 0;
    (g_SubmarineHudFifteenElements.quads[7].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[7].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[7].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[7].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[7].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[7].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[8].x) = 0xFF92;
    (g_SubmarineHudFifteenElements.quads[8].y) = 0xFF9E;
    (g_SubmarineHudFifteenElements.quads[8].width) = 20;
    (g_SubmarineHudFifteenElements.quads[8].height) = 20;
    (g_SubmarineHudFifteenElements.quads[8].srcX) = 20;
    (g_SubmarineHudFifteenElements.quads[8].srcY) = 22;
    (g_SubmarineHudFifteenElements.quads[8].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[8].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[8].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[8].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[8].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[8].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[9].x) = 0xFFBA;
    (g_SubmarineHudFifteenElements.quads[9].y) = 0xFF9E;
    (g_SubmarineHudFifteenElements.quads[9].width) = 25;
    (g_SubmarineHudFifteenElements.quads[9].height) = 19;
    (g_SubmarineHudFifteenElements.quads[9].srcX) = 223;
    (g_SubmarineHudFifteenElements.quads[9].srcY) = 0;
    (g_SubmarineHudFifteenElements.quads[9].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[9].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[9].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[9].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[9].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[9].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[10].x) = 0xFFEA;
    (g_SubmarineHudFifteenElements.quads[10].y) = 0xFF9E;
    (g_SubmarineHudFifteenElements.quads[10].width) = 17;
    (g_SubmarineHudFifteenElements.quads[10].height) = 19;
    (g_SubmarineHudFifteenElements.quads[10].srcX) = 40;
    (g_SubmarineHudFifteenElements.quads[10].srcY) = 22;
    (g_SubmarineHudFifteenElements.quads[10].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[10].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[10].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[10].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[10].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[10].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[11].x) = 14;
    (g_SubmarineHudFifteenElements.quads[11].y) = 0xFF9E;
    (g_SubmarineHudFifteenElements.quads[11].width) = 17;
    (g_SubmarineHudFifteenElements.quads[11].height) = 19;
    (g_SubmarineHudFifteenElements.quads[11].srcX) = 205;
    (g_SubmarineHudFifteenElements.quads[11].srcY) = 0;
    (g_SubmarineHudFifteenElements.quads[11].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[11].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[11].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[11].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[11].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[11].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[12].x) = 48;
    (g_SubmarineHudFifteenElements.quads[12].y) = 0xFF9E;
    (g_SubmarineHudFifteenElements.quads[12].width) = 17;
    (g_SubmarineHudFifteenElements.quads[12].height) = 19;
    (g_SubmarineHudFifteenElements.quads[12].srcX) = 77;
    (g_SubmarineHudFifteenElements.quads[12].srcY) = 0;
    (g_SubmarineHudFifteenElements.quads[12].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[12].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[12].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[12].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[12].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[12].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[13].x) = 82;
    (g_SubmarineHudFifteenElements.quads[13].y) = 0xFF9E;
    (g_SubmarineHudFifteenElements.quads[13].width) = 17;
    (g_SubmarineHudFifteenElements.quads[13].height) = 19;
    (g_SubmarineHudFifteenElements.quads[13].srcX) = 114;
    (g_SubmarineHudFifteenElements.quads[13].srcY) = 22;
    (g_SubmarineHudFifteenElements.quads[13].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[13].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[13].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[13].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[13].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[13].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFifteenElements.quads[14].x) = 118;
    (g_SubmarineHudFifteenElements.quads[14].y) = 0xFF9E;
    (g_SubmarineHudFifteenElements.quads[14].width) = 17;
    (g_SubmarineHudFifteenElements.quads[14].height) = 19;
    (g_SubmarineHudFifteenElements.quads[14].srcX) = 77;
    (g_SubmarineHudFifteenElements.quads[14].srcY) = 0;
    (g_SubmarineHudFifteenElements.quads[14].alpha) = 5;
    (g_SubmarineHudFifteenElements.quads[14].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFifteenElements.quads[14].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFifteenElements.quads[14].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFifteenElements.quads[14].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFifteenElements.quads[14].batches[3]) = g_SubmarineHudTextAtlasD;
    if (scaleMode != 2) {
        for (entry15 = 0; entry15 < 15; entry15++) {
            g_SubmarineHudFifteenElements.quads[entry15].batches[0] = g_SubmarineHudTextAtlas;
            g_SubmarineHudFifteenElements.quads[entry15].batches[1] = 0;
            g_SubmarineHudFifteenElements.quads[entry15].batches[2] = 0;
            g_SubmarineHudFifteenElements.quads[entry15].batches[3] = 0;
        }
    }
    g_SubmarineHudSeventeenElements[0].x = 0xFF90;
    g_SubmarineHudSeventeenElements[0].y = 0xFF70;
    g_SubmarineHudSeventeenElements[0].width = 25;
    g_SubmarineHudSeventeenElements[0].height = 19;
    g_SubmarineHudSeventeenElements[0].srcX = 223;
    g_SubmarineHudSeventeenElements[0].srcY = 0;
    g_SubmarineHudSeventeenElements[0].alpha = 5;
    g_SubmarineHudSeventeenElements[0].depth = FLOAT_0_01_BITS;
    g_SubmarineHudSeventeenElements[0].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudSeventeenElements[0].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudSeventeenElements[0].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudSeventeenElements[0].batches[3] = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[1].x) = 0xFFC2;
    (g_SubmarineHudSeventeenElements[1].y) = 0xFF70;
    (g_SubmarineHudSeventeenElements[1].width) = 11;
    (g_SubmarineHudSeventeenElements[1].height) = 19;
    (g_SubmarineHudSeventeenElements[1].srcX) = 155;
    (g_SubmarineHudSeventeenElements[1].srcY) = 0;
    (g_SubmarineHudSeventeenElements[1].alpha) = 5;
    (g_SubmarineHudSeventeenElements[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[1].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[1].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[1].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[1].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[2].x) = 0xFFDA;
    (g_SubmarineHudSeventeenElements[2].y) = 0xFF70;
    (g_SubmarineHudSeventeenElements[2].width) = 14;
    (g_SubmarineHudSeventeenElements[2].height) = 20;
    (g_SubmarineHudSeventeenElements[2].srcX) = 99;
    (g_SubmarineHudSeventeenElements[2].srcY) = 22;
    (g_SubmarineHudSeventeenElements[2].alpha) = 5;
    (g_SubmarineHudSeventeenElements[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[2].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[2].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[2].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[2].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[3].x) = 0xFFFA;
    (g_SubmarineHudSeventeenElements[3].y) = 0xFF70;
    (g_SubmarineHudSeventeenElements[3].width) = 14;
    (g_SubmarineHudSeventeenElements[3].height) = 20;
    (g_SubmarineHudSeventeenElements[3].srcX) = 99;
    (g_SubmarineHudSeventeenElements[3].srcY) = 22;
    (g_SubmarineHudSeventeenElements[3].alpha) = 5;
    (g_SubmarineHudSeventeenElements[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[3].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[3].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[3].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[3].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[4].x) = 24;
    (g_SubmarineHudSeventeenElements[4].y) = 0xFF70;
    (g_SubmarineHudSeventeenElements[4].width) = 11;
    (g_SubmarineHudSeventeenElements[4].height) = 19;
    (g_SubmarineHudSeventeenElements[4].srcX) = 155;
    (g_SubmarineHudSeventeenElements[4].srcY) = 0;
    (g_SubmarineHudSeventeenElements[4].alpha) = 5;
    (g_SubmarineHudSeventeenElements[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[4].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[4].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[4].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[4].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[5].x) = 46;
    (g_SubmarineHudSeventeenElements[5].y) = 0xFF70;
    (g_SubmarineHudSeventeenElements[5].width) = 20;
    (g_SubmarineHudSeventeenElements[5].height) = 20;
    (g_SubmarineHudSeventeenElements[5].srcX) = 20;
    (g_SubmarineHudSeventeenElements[5].srcY) = 22;
    (g_SubmarineHudSeventeenElements[5].alpha) = 5;
    (g_SubmarineHudSeventeenElements[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[5].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[5].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[5].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[5].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[6].x) = 86;
    (g_SubmarineHudSeventeenElements[6].y) = 0xFF70;
    (g_SubmarineHudSeventeenElements[6].width) = 19;
    (g_SubmarineHudSeventeenElements[6].height) = 20;
    (g_SubmarineHudSeventeenElements[6].srcX) = 0;
    (g_SubmarineHudSeventeenElements[6].srcY) = 22;
    (g_SubmarineHudSeventeenElements[6].alpha) = 5;
    (g_SubmarineHudSeventeenElements[6].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[6].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[6].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[6].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[6].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[7].x) = 0xFF4E;
    (g_SubmarineHudSeventeenElements[7].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[7].width) = 11;
    (g_SubmarineHudSeventeenElements[7].height) = 19;
    (g_SubmarineHudSeventeenElements[7].srcX) = 155;
    (g_SubmarineHudSeventeenElements[7].srcY) = 0;
    (g_SubmarineHudSeventeenElements[7].alpha) = 5;
    (g_SubmarineHudSeventeenElements[7].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[7].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[7].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[7].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[7].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[8].x) = 0xFF66;
    (g_SubmarineHudSeventeenElements[8].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[8].width) = 19;
    (g_SubmarineHudSeventeenElements[8].height) = 20;
    (g_SubmarineHudSeventeenElements[8].srcX) = 0;
    (g_SubmarineHudSeventeenElements[8].srcY) = 22;
    (g_SubmarineHudSeventeenElements[8].alpha) = 5;
    (g_SubmarineHudSeventeenElements[8].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[8].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[8].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[8].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[8].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[9].x) = 0xFF8C;
    (g_SubmarineHudSeventeenElements[9].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[9].width) = 18;
    (g_SubmarineHudSeventeenElements[9].height) = 20;
    (g_SubmarineHudSeventeenElements[9].srcX) = 39;
    (g_SubmarineHudSeventeenElements[9].srcY) = 0;
    (g_SubmarineHudSeventeenElements[9].alpha) = 5;
    (g_SubmarineHudSeventeenElements[9].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[9].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[9].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[9].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[9].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[10].x) = 0xFFB2;
    (g_SubmarineHudSeventeenElements[10].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[10].width) = 20;
    (g_SubmarineHudSeventeenElements[10].height) = 20;
    (g_SubmarineHudSeventeenElements[10].srcX) = 20;
    (g_SubmarineHudSeventeenElements[10].srcY) = 22;
    (g_SubmarineHudSeventeenElements[10].alpha) = 5;
    (g_SubmarineHudSeventeenElements[10].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[10].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[10].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[10].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[10].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[11].x) = 0xFFDA;
    (g_SubmarineHudSeventeenElements[11].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[11].width) = 25;
    (g_SubmarineHudSeventeenElements[11].height) = 19;
    (g_SubmarineHudSeventeenElements[11].srcX) = 223;
    (g_SubmarineHudSeventeenElements[11].srcY) = 0;
    (g_SubmarineHudSeventeenElements[11].alpha) = 5;
    (g_SubmarineHudSeventeenElements[11].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[11].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[11].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[11].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[11].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[12].x) = 10;
    (g_SubmarineHudSeventeenElements[12].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[12].width) = 17;
    (g_SubmarineHudSeventeenElements[12].height) = 19;
    (g_SubmarineHudSeventeenElements[12].srcX) = 40;
    (g_SubmarineHudSeventeenElements[12].srcY) = 22;
    (g_SubmarineHudSeventeenElements[12].alpha) = 5;
    (g_SubmarineHudSeventeenElements[12].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[12].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[12].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[12].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[12].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[13].x) = 46;
    (g_SubmarineHudSeventeenElements[13].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[13].width) = 17;
    (g_SubmarineHudSeventeenElements[13].height) = 19;
    (g_SubmarineHudSeventeenElements[13].srcX) = 205;
    (g_SubmarineHudSeventeenElements[13].srcY) = 0;
    (g_SubmarineHudSeventeenElements[13].alpha) = 5;
    (g_SubmarineHudSeventeenElements[13].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[13].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[13].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[13].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[13].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[14].x) = 80;
    (g_SubmarineHudSeventeenElements[14].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[14].width) = 17;
    (g_SubmarineHudSeventeenElements[14].height) = 19;
    (g_SubmarineHudSeventeenElements[14].srcX) = 77;
    (g_SubmarineHudSeventeenElements[14].srcY) = 0;
    (g_SubmarineHudSeventeenElements[14].alpha) = 5;
    (g_SubmarineHudSeventeenElements[14].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[14].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[14].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[14].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[14].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[15].x) = 114;
    (g_SubmarineHudSeventeenElements[15].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[15].width) = 17;
    (g_SubmarineHudSeventeenElements[15].height) = 19;
    (g_SubmarineHudSeventeenElements[15].srcX) = 114;
    (g_SubmarineHudSeventeenElements[15].srcY) = 22;
    (g_SubmarineHudSeventeenElements[15].alpha) = 5;
    (g_SubmarineHudSeventeenElements[15].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[15].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[15].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[15].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[15].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudSeventeenElements[16].x) = 150;
    (g_SubmarineHudSeventeenElements[16].y) = 0xFF9E;
    (g_SubmarineHudSeventeenElements[16].width) = 17;
    (g_SubmarineHudSeventeenElements[16].height) = 19;
    (g_SubmarineHudSeventeenElements[16].srcX) = 77;
    (g_SubmarineHudSeventeenElements[16].srcY) = 0;
    (g_SubmarineHudSeventeenElements[16].alpha) = 5;
    (g_SubmarineHudSeventeenElements[16].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudSeventeenElements[16].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudSeventeenElements[16].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudSeventeenElements[16].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudSeventeenElements[16].batches[3]) = g_SubmarineHudTextAtlasD;
    if (scaleMode != 2) {
        for (workSeventeenIdx = 0; workSeventeenIdx < 17; workSeventeenIdx++) {
            g_SubmarineHudSeventeenElements[workSeventeenIdx].batches[0] = g_SubmarineHudTextAtlas;
            g_SubmarineHudSeventeenElements[workSeventeenIdx].batches[1] = 0;
            g_SubmarineHudSeventeenElements[workSeventeenIdx].batches[2] = 0;
            g_SubmarineHudSeventeenElements[workSeventeenIdx].batches[3] = 0;
        }
    }
    g_SubmarineHudNineElements[0].x = 0xFF5E;
    g_SubmarineHudNineElements[0].y = 0xFF87;
    g_SubmarineHudNineElements[0].width = 17;
    g_SubmarineHudNineElements[0].height = 19;
    g_SubmarineHudNineElements[0].srcX = 114;
    g_SubmarineHudNineElements[0].srcY = 22;
    g_SubmarineHudNineElements[0].alpha = 5;
    g_SubmarineHudNineElements[0].depth = FLOAT_0_01_BITS;
    g_SubmarineHudNineElements[0].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudNineElements[0].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudNineElements[0].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudNineElements[0].batches[3] = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudNineElements[1].x) = 0xFF82;
    (g_SubmarineHudNineElements[1].y) = 0xFF87;
    (g_SubmarineHudNineElements[1].width) = 11;
    (g_SubmarineHudNineElements[1].height) = 19;
    (g_SubmarineHudNineElements[1].srcX) = 155;
    (g_SubmarineHudNineElements[1].srcY) = 0;
    (g_SubmarineHudNineElements[1].alpha) = 5;
    (g_SubmarineHudNineElements[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudNineElements[1].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudNineElements[1].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudNineElements[1].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudNineElements[1].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudNineElements[2].x) = 0xFF9A;
    (g_SubmarineHudNineElements[2].y) = 0xFF87;
    (g_SubmarineHudNineElements[2].width) = 25;
    (g_SubmarineHudNineElements[2].height) = 19;
    (g_SubmarineHudNineElements[2].srcX) = 223;
    (g_SubmarineHudNineElements[2].srcY) = 0;
    (g_SubmarineHudNineElements[2].alpha) = 5;
    (g_SubmarineHudNineElements[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudNineElements[2].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudNineElements[2].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudNineElements[2].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudNineElements[2].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudNineElements[3].x) = 0xFFCE;
    (g_SubmarineHudNineElements[3].y) = 0xFF87;
    (g_SubmarineHudNineElements[3].width) = 17;
    (g_SubmarineHudNineElements[3].height) = 19;
    (g_SubmarineHudNineElements[3].srcX) = 77;
    (g_SubmarineHudNineElements[3].srcY) = 0;
    (g_SubmarineHudNineElements[3].alpha) = 5;
    (g_SubmarineHudNineElements[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudNineElements[3].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudNineElements[3].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudNineElements[3].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudNineElements[3].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudNineElements[4].x) = 0xFFF2;
    (g_SubmarineHudNineElements[4].y) = 0xFF87;
    (g_SubmarineHudNineElements[4].width) = 17;
    (g_SubmarineHudNineElements[4].height) = 19;
    (g_SubmarineHudNineElements[4].srcX) = 205;
    (g_SubmarineHudNineElements[4].srcY) = 0;
    (g_SubmarineHudNineElements[4].alpha) = 5;
    (g_SubmarineHudNineElements[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudNineElements[4].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudNineElements[4].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudNineElements[4].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudNineElements[4].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudNineElements[5].x) = 20;
    (g_SubmarineHudNineElements[5].y) = 0xFF87;
    (g_SubmarineHudNineElements[5].width) = 11;
    (g_SubmarineHudNineElements[5].height) = 19;
    (g_SubmarineHudNineElements[5].srcX) = 155;
    (g_SubmarineHudNineElements[5].srcY) = 0;
    (g_SubmarineHudNineElements[5].alpha) = 5;
    (g_SubmarineHudNineElements[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudNineElements[5].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudNineElements[5].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudNineElements[5].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudNineElements[5].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudNineElements[6].x) = 44;
    (g_SubmarineHudNineElements[6].y) = 0xFF87;
    (g_SubmarineHudNineElements[6].width) = 25;
    (g_SubmarineHudNineElements[6].height) = 19;
    (g_SubmarineHudNineElements[6].srcX) = 223;
    (g_SubmarineHudNineElements[6].srcY) = 0;
    (g_SubmarineHudNineElements[6].alpha) = 5;
    (g_SubmarineHudNineElements[6].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudNineElements[6].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudNineElements[6].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudNineElements[6].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudNineElements[6].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudNineElements[7].x) = 96;
    (g_SubmarineHudNineElements[7].y) = 0xFF87;
    (g_SubmarineHudNineElements[7].width) = 11;
    (g_SubmarineHudNineElements[7].height) = 19;
    (g_SubmarineHudNineElements[7].srcX) = 155;
    (g_SubmarineHudNineElements[7].srcY) = 0;
    (g_SubmarineHudNineElements[7].alpha) = 5;
    (g_SubmarineHudNineElements[7].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudNineElements[7].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudNineElements[7].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudNineElements[7].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudNineElements[7].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudNineElements[8].x) = 120;
    (g_SubmarineHudNineElements[8].y) = 0xFF87;
    (g_SubmarineHudNineElements[8].width) = 17;
    (g_SubmarineHudNineElements[8].height) = 19;
    (g_SubmarineHudNineElements[8].srcX) = 114;
    (g_SubmarineHudNineElements[8].srcY) = 22;
    (g_SubmarineHudNineElements[8].alpha) = 5;
    (g_SubmarineHudNineElements[8].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudNineElements[8].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudNineElements[8].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudNineElements[8].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudNineElements[8].batches[3]) = g_SubmarineHudTextAtlasD;
    if (scaleMode != 2) {
        for (entry9 = 0; entry9 < 9; entry9++) {
            g_SubmarineHudNineElements[entry9].batches[0] = g_SubmarineHudTextAtlas;
            g_SubmarineHudNineElements[entry9].batches[1] = 0;
            g_SubmarineHudNineElements[entry9].batches[2] = 0;
            g_SubmarineHudNineElements[entry9].batches[3] = 0;
        }
    }
    g_SubmarineHudFiveQuadRecords.quads[0].x = 0xFFB8;
    g_SubmarineHudFiveQuadRecords.quads[0].y = 0xFFE4;
    g_SubmarineHudFiveQuadRecords.quads[0].width = 17;
    g_SubmarineHudFiveQuadRecords.quads[0].height = 19;
    g_SubmarineHudFiveQuadRecords.quads[0].srcX = 40;
    g_SubmarineHudFiveQuadRecords.quads[0].srcY = 22;
    g_SubmarineHudFiveQuadRecords.quads[0].alpha = 1;
    g_SubmarineHudFiveQuadRecords.quads[0].depth = FLOAT_0_01_BITS;
    g_SubmarineHudFiveQuadRecords.quads[0].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudFiveQuadRecords.quads[0].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudFiveQuadRecords.quads[0].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudFiveQuadRecords.quads[0].batches[3] = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFiveQuadRecords.quads[1].x) = 0xFFD0;
    (g_SubmarineHudFiveQuadRecords.quads[1].y) = 0xFFE4;
    (g_SubmarineHudFiveQuadRecords.quads[1].width) = 19;
    (g_SubmarineHudFiveQuadRecords.quads[1].height) = 19;
    (g_SubmarineHudFiveQuadRecords.quads[1].srcX) = 0;
    (g_SubmarineHudFiveQuadRecords.quads[1].srcY) = 0;
    (g_SubmarineHudFiveQuadRecords.quads[1].alpha) = 1;
    (g_SubmarineHudFiveQuadRecords.quads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFiveQuadRecords.quads[1].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFiveQuadRecords.quads[1].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFiveQuadRecords.quads[1].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFiveQuadRecords.quads[1].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFiveQuadRecords.quads[2].x) = 0xFFEE;
    (g_SubmarineHudFiveQuadRecords.quads[2].y) = 0xFFE4;
    (g_SubmarineHudFiveQuadRecords.quads[2].width) = 19;
    (g_SubmarineHudFiveQuadRecords.quads[2].height) = 20;
    (g_SubmarineHudFiveQuadRecords.quads[2].srcX) = 132;
    (g_SubmarineHudFiveQuadRecords.quads[2].srcY) = 22;
    (g_SubmarineHudFiveQuadRecords.quads[2].alpha) = 1;
    (g_SubmarineHudFiveQuadRecords.quads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFiveQuadRecords.quads[2].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFiveQuadRecords.quads[2].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFiveQuadRecords.quads[2].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFiveQuadRecords.quads[2].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFiveQuadRecords.quads[3].x) = 16;
    (g_SubmarineHudFiveQuadRecords.quads[3].y) = 0xFFE4;
    (g_SubmarineHudFiveQuadRecords.quads[3].width) = 14;
    (g_SubmarineHudFiveQuadRecords.quads[3].height) = 20;
    (g_SubmarineHudFiveQuadRecords.quads[3].srcX) = 99;
    (g_SubmarineHudFiveQuadRecords.quads[3].srcY) = 22;
    (g_SubmarineHudFiveQuadRecords.quads[3].alpha) = 1;
    (g_SubmarineHudFiveQuadRecords.quads[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFiveQuadRecords.quads[3].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFiveQuadRecords.quads[3].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFiveQuadRecords.quads[3].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFiveQuadRecords.quads[3].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudFiveQuadRecords.quads[4].x) = 40;
    (g_SubmarineHudFiveQuadRecords.quads[4].y) = 0xFFE4;
    (g_SubmarineHudFiveQuadRecords.quads[4].width) = 17;
    (g_SubmarineHudFiveQuadRecords.quads[4].height) = 19;
    (g_SubmarineHudFiveQuadRecords.quads[4].srcX) = 77;
    (g_SubmarineHudFiveQuadRecords.quads[4].srcY) = 0;
    (g_SubmarineHudFiveQuadRecords.quads[4].alpha) = 1;
    (g_SubmarineHudFiveQuadRecords.quads[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudFiveQuadRecords.quads[4].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudFiveQuadRecords.quads[4].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudFiveQuadRecords.quads[4].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudFiveQuadRecords.quads[4].batches[3]) = g_SubmarineHudTextAtlasD;
    if (scaleMode != 2) {
        for (fiveIdx = 0; fiveIdx < 5; fiveIdx++) {
            g_SubmarineHudFiveQuadRecords.quads[fiveIdx].batches[0] = g_SubmarineHudTextAtlas;
            g_SubmarineHudFiveQuadRecords.quads[fiveIdx].batches[1] = 0;
            g_SubmarineHudFiveQuadRecords.quads[fiveIdx].batches[2] = 0;
            g_SubmarineHudFiveQuadRecords.quads[fiveIdx].batches[3] = 0;
        }
    }
    (g_SubmarineHudRenderState.hudRecordPtr) = 0;
    g_SubmarineHudRenderState.swapQuads[0].x = 0xFFB4;
    g_SubmarineHudRenderState.swapQuads[0].y = 0xFF70;
    g_SubmarineHudRenderState.swapQuads[0].width = 20;
    g_SubmarineHudRenderState.swapQuads[0].height = 23;
    g_SubmarineHudRenderState.swapQuads[0].srcX = 58;
    g_SubmarineHudRenderState.swapQuads[0].srcY = 22;
    g_SubmarineHudRenderState.swapQuads[0].alpha = 5;
    g_SubmarineHudRenderState.swapQuads[0].depth = FLOAT_0_01_BITS;
    g_SubmarineHudRenderState.swapQuads[0].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudRenderState.swapQuads[0].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudRenderState.swapQuads[0].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudRenderState.swapQuads[0].batches[3] = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudRenderState.swapQuads[1].x) = 0xFFDC;
    (g_SubmarineHudRenderState.swapQuads[1].y) = 0xFF70;
    (g_SubmarineHudRenderState.swapQuads[1].width) = 19;
    (g_SubmarineHudRenderState.swapQuads[1].height) = 20;
    (g_SubmarineHudRenderState.swapQuads[1].srcX) = 132;
    (g_SubmarineHudRenderState.swapQuads[1].srcY) = 22;
    (g_SubmarineHudRenderState.swapQuads[1].alpha) = 5;
    (g_SubmarineHudRenderState.swapQuads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRenderState.swapQuads[1].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudRenderState.swapQuads[1].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudRenderState.swapQuads[1].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudRenderState.swapQuads[1].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudRenderState.swapQuads[2].x) = 4;
    (g_SubmarineHudRenderState.swapQuads[2].y) = 0xFF70;
    (g_SubmarineHudRenderState.swapQuads[2].width) = 11;
    (g_SubmarineHudRenderState.swapQuads[2].height) = 19;
    (g_SubmarineHudRenderState.swapQuads[2].srcX) = 155;
    (g_SubmarineHudRenderState.swapQuads[2].srcY) = 0;
    (g_SubmarineHudRenderState.swapQuads[2].alpha) = 5;
    (g_SubmarineHudRenderState.swapQuads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRenderState.swapQuads[2].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudRenderState.swapQuads[2].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudRenderState.swapQuads[2].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudRenderState.swapQuads[2].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudRenderState.swapQuads[3].x) = 28;
    (g_SubmarineHudRenderState.swapQuads[3].y) = 0xFF70;
    (g_SubmarineHudRenderState.swapQuads[3].width) = 17;
    (g_SubmarineHudRenderState.swapQuads[3].height) = 19;
    (g_SubmarineHudRenderState.swapQuads[3].srcX) = 114;
    (g_SubmarineHudRenderState.swapQuads[3].srcY) = 22;
    (g_SubmarineHudRenderState.swapQuads[3].alpha) = 5;
    (g_SubmarineHudRenderState.swapQuads[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRenderState.swapQuads[3].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudRenderState.swapQuads[3].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudRenderState.swapQuads[3].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudRenderState.swapQuads[3].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudRenderState.swapQuads[4].x) = 66;
    (g_SubmarineHudRenderState.swapQuads[4].y) = 0xFF70;
    (g_SubmarineHudRenderState.swapQuads[4].width) = 12;
    (g_SubmarineHudRenderState.swapQuads[4].height) = 20;
    (g_SubmarineHudRenderState.swapQuads[4].srcX) = 176;
    (g_SubmarineHudRenderState.swapQuads[4].srcY) = 48;
    (g_SubmarineHudRenderState.swapQuads[4].alpha) = 5;
    (g_SubmarineHudRenderState.swapQuads[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudRenderState.swapQuads[4].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudRenderState.swapQuads[4].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudRenderState.swapQuads[4].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudRenderState.swapQuads[4].batches[3]) = g_SubmarineHudTextAtlasD;
    g_SubmarineHudRenderState.swapQuads[5].x = 0xFFCC;
    g_SubmarineHudRenderState.swapQuads[5].y = 0xFFA8;
    g_SubmarineHudRenderState.swapQuads[5].width = 19;
    g_SubmarineHudRenderState.swapQuads[5].height = 19;
    g_SubmarineHudRenderState.swapQuads[5].srcX = 218;
    g_SubmarineHudRenderState.swapQuads[5].srcY = 22;
    g_SubmarineHudRenderState.swapQuads[5].alpha = 5;
    g_SubmarineHudRenderState.swapQuads[5].depth = FLOAT_0_01_BITS;
    g_SubmarineHudRenderState.swapQuads[5].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudRenderState.swapQuads[5].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudRenderState.swapQuads[5].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudRenderState.swapQuads[5].batches[3] = g_SubmarineHudTextAtlasD;
    g_SubmarineHudRenderState.swapQuads[6].x = 0xFFF2;
    g_SubmarineHudRenderState.swapQuads[6].y = 0xFFA8;
    g_SubmarineHudRenderState.swapQuads[6].width = 10;
    g_SubmarineHudRenderState.swapQuads[6].height = 19;
    g_SubmarineHudRenderState.swapQuads[6].srcX = 160;
    g_SubmarineHudRenderState.swapQuads[6].srcY = 48;
    g_SubmarineHudRenderState.swapQuads[6].alpha = 5;
    g_SubmarineHudRenderState.swapQuads[6].depth = FLOAT_0_01_BITS;
    g_SubmarineHudRenderState.swapQuads[6].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudRenderState.swapQuads[6].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudRenderState.swapQuads[6].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudRenderState.swapQuads[6].batches[3] = g_SubmarineHudTextAtlasD;
    g_SubmarineHudRenderState.swapQuads[7].x = 10;
    g_SubmarineHudRenderState.swapQuads[7].y = 0xFFA8;
    g_SubmarineHudRenderState.swapQuads[7].width = 19;
    g_SubmarineHudRenderState.swapQuads[7].height = 20;
    g_SubmarineHudRenderState.swapQuads[7].srcX = 0;
    g_SubmarineHudRenderState.swapQuads[7].srcY = 22;
    g_SubmarineHudRenderState.swapQuads[7].alpha = 5;
    g_SubmarineHudRenderState.swapQuads[7].depth = FLOAT_0_01_BITS;
    g_SubmarineHudRenderState.swapQuads[7].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudRenderState.swapQuads[7].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudRenderState.swapQuads[7].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudRenderState.swapQuads[7].batches[3] = g_SubmarineHudTextAtlasD;
    if (scaleMode != 2) {
        for (swapNo = 0; swapNo < 8; swapNo++) {
            g_SubmarineHudRenderState.swapQuads[swapNo].batches[0] = g_SubmarineHudTextAtlas;
            g_SubmarineHudRenderState.swapQuads[swapNo].batches[1] = 0;
            g_SubmarineHudRenderState.swapQuads[swapNo].batches[2] = 0;
            g_SubmarineHudRenderState.swapQuads[swapNo].batches[3] = 0;
        }
    }
    g_SubmarineHudTallyElements.quads[0].x = 0xFF4C;
    g_SubmarineHudTallyElements.quads[0].y = 0xFF68;
    g_SubmarineHudTallyElements.quads[0].width = 17;
    g_SubmarineHudTallyElements.quads[0].height = 19;
    g_SubmarineHudTallyElements.quads[0].srcX = 114;
    g_SubmarineHudTallyElements.quads[0].srcY = 22;
    g_SubmarineHudTallyElements.quads[0].alpha = 7;
    g_SubmarineHudTallyElements.quads[0].depth = FLOAT_0_01_BITS;
    g_SubmarineHudTallyElements.quads[0].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudTallyElements.quads[0].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudTallyElements.quads[0].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudTallyElements.quads[0].batches[3] = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[1].x) = 0xFF6A;
    (g_SubmarineHudTallyElements.quads[1].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[1].width) = 19;
    (g_SubmarineHudTallyElements.quads[1].height) = 19;
    (g_SubmarineHudTallyElements.quads[1].srcX) = 0;
    (g_SubmarineHudTallyElements.quads[1].srcY) = 0;
    (g_SubmarineHudTallyElements.quads[1].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[1].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[1].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[1].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[1].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[2].x) = 0xFF8E;
    (g_SubmarineHudTallyElements.quads[2].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[2].width) = 19;
    (g_SubmarineHudTallyElements.quads[2].height) = 19;
    (g_SubmarineHudTallyElements.quads[2].srcX) = 79;
    (g_SubmarineHudTallyElements.quads[2].srcY) = 22;
    (g_SubmarineHudTallyElements.quads[2].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[2].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[2].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[2].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[2].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[3].x) = 0xFFB2;
    (g_SubmarineHudTallyElements.quads[3].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[3].width) = 20;
    (g_SubmarineHudTallyElements.quads[3].height) = 20;
    (g_SubmarineHudTallyElements.quads[3].srcX) = 112;
    (g_SubmarineHudTallyElements.quads[3].srcY) = 0;
    (g_SubmarineHudTallyElements.quads[3].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[3].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[3].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[3].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[3].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[4].x) = 0xFFD8;
    (g_SubmarineHudTallyElements.quads[4].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[4].width) = 17;
    (g_SubmarineHudTallyElements.quads[4].height) = 19;
    (g_SubmarineHudTallyElements.quads[4].srcX) = 77;
    (g_SubmarineHudTallyElements.quads[4].srcY) = 0;
    (g_SubmarineHudTallyElements.quads[4].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[4].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[4].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[4].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[4].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[5].x) = 0xFFFA;
    (g_SubmarineHudTallyElements.quads[5].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[5].width) = 17;
    (g_SubmarineHudTallyElements.quads[5].height) = 19;
    (g_SubmarineHudTallyElements.quads[5].srcX) = 114;
    (g_SubmarineHudTallyElements.quads[5].srcY) = 22;
    (g_SubmarineHudTallyElements.quads[5].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[5].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[5].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[5].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[5].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[6].x) = 50;
    (g_SubmarineHudTallyElements.quads[6].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[6].width) = 13;
    (g_SubmarineHudTallyElements.quads[6].height) = 20;
    g_SubmarineHudTallyElements.quads[6].srcX = 0;
    (g_SubmarineHudTallyElements.quads[6].srcY) = 48;
    (g_SubmarineHudTallyElements.quads[6].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[6].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[6].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[6].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[6].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[6].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[7].x) = 80;
    (g_SubmarineHudTallyElements.quads[7].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[7].width) = 13;
    (g_SubmarineHudTallyElements.quads[7].height) = 20;
    g_SubmarineHudTallyElements.quads[7].srcX = 0;
    (g_SubmarineHudTallyElements.quads[7].srcY) = 48;
    (g_SubmarineHudTallyElements.quads[7].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[7].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[7].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[7].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[7].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[7].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[8].x) = 108;
    (g_SubmarineHudTallyElements.quads[8].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[8].width) = 10;
    (g_SubmarineHudTallyElements.quads[8].height) = 19;
    (g_SubmarineHudTallyElements.quads[8].srcX) = 160;
    (g_SubmarineHudTallyElements.quads[8].srcY) = 48;
    (g_SubmarineHudTallyElements.quads[8].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[8].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[8].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[8].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[8].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[8].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[9].x) = 130;
    (g_SubmarineHudTallyElements.quads[9].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[9].width) = 13;
    (g_SubmarineHudTallyElements.quads[9].height) = 20;
    g_SubmarineHudTallyElements.quads[9].srcX = 0;
    (g_SubmarineHudTallyElements.quads[9].srcY) = 48;
    (g_SubmarineHudTallyElements.quads[9].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[9].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[9].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[9].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[9].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[9].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudTallyElements.quads[10].x) = 160;
    (g_SubmarineHudTallyElements.quads[10].y) = 0xFF68;
    (g_SubmarineHudTallyElements.quads[10].width) = 13;
    (g_SubmarineHudTallyElements.quads[10].height) = 20;
    g_SubmarineHudTallyElements.quads[10].srcX = 0;
    (g_SubmarineHudTallyElements.quads[10].srcY) = 48;
    (g_SubmarineHudTallyElements.quads[10].alpha) = 7;
    (g_SubmarineHudTallyElements.quads[10].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudTallyElements.quads[10].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudTallyElements.quads[10].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudTallyElements.quads[10].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudTallyElements.quads[10].batches[3]) = g_SubmarineHudTextAtlasD;
    if (scaleMode != 2) {
        for (tallyIndex = 0; tallyIndex < 11; tallyIndex++) {
            g_SubmarineHudTallyElements.quads[tallyIndex].batches[0] = g_SubmarineHudTextAtlas;
            g_SubmarineHudTallyElements.quads[tallyIndex].batches[1] = 0;
            g_SubmarineHudTallyElements.quads[tallyIndex].batches[2] = 0;
            g_SubmarineHudTallyElements.quads[tallyIndex].batches[3] = 0;
        }
    }
    g_SubmarineHudScoreCounters[0].x = 0xFF4C;
    g_SubmarineHudScoreCounters[0].y = 0xFFE0;
    g_SubmarineHudScoreCounters[0].width = 14;
    g_SubmarineHudScoreCounters[0].height = 20;
    g_SubmarineHudScoreCounters[0].srcX = 99;
    g_SubmarineHudScoreCounters[0].srcY = 22;
    g_SubmarineHudScoreCounters[0].alpha = 5;
    g_SubmarineHudScoreCounters[0].depth = FLOAT_0_01_BITS;
    g_SubmarineHudScoreCounters[0].batches[0] = g_SubmarineHudTextAtlasA;
    g_SubmarineHudScoreCounters[0].batches[1] = g_SubmarineHudTextAtlasB;
    g_SubmarineHudScoreCounters[0].batches[2] = g_SubmarineHudTextAtlasC;
    g_SubmarineHudScoreCounters[0].batches[3] = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[1].x) = 0xFF6C;
    (g_SubmarineHudScoreCounters[1].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[1].width) = 18;
    (g_SubmarineHudScoreCounters[1].height) = 20;
    (g_SubmarineHudScoreCounters[1].srcX) = 39;
    (g_SubmarineHudScoreCounters[1].srcY) = 0;
    (g_SubmarineHudScoreCounters[1].alpha) = 5;
    (g_SubmarineHudScoreCounters[1].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[1].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[1].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[1].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[1].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[2].x) = 0xFF92;
    (g_SubmarineHudScoreCounters[2].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[2].width) = 20;
    (g_SubmarineHudScoreCounters[2].height) = 20;
    (g_SubmarineHudScoreCounters[2].srcX) = 20;
    (g_SubmarineHudScoreCounters[2].srcY) = 22;
    (g_SubmarineHudScoreCounters[2].alpha) = 5;
    (g_SubmarineHudScoreCounters[2].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[2].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[2].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[2].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[2].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[3].x) = 0xFFB8;
    (g_SubmarineHudScoreCounters[3].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[3].width) = 19;
    (g_SubmarineHudScoreCounters[3].height) = 19;
    (g_SubmarineHudScoreCounters[3].srcX) = 79;
    (g_SubmarineHudScoreCounters[3].srcY) = 22;
    (g_SubmarineHudScoreCounters[3].alpha) = 5;
    (g_SubmarineHudScoreCounters[3].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[3].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[3].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[3].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[3].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[4].x) = 0xFFDE;
    (g_SubmarineHudScoreCounters[4].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[4].width) = 17;
    (g_SubmarineHudScoreCounters[4].height) = 19;
    (g_SubmarineHudScoreCounters[4].srcX) = 77;
    (g_SubmarineHudScoreCounters[4].srcY) = 0;
    (g_SubmarineHudScoreCounters[4].alpha) = 5;
    (g_SubmarineHudScoreCounters[4].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[4].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[4].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[4].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[4].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[5].x) = 40;
    (g_SubmarineHudScoreCounters[5].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[5].width) = 13;
    (g_SubmarineHudScoreCounters[5].height) = 20;
    (g_SubmarineHudScoreCounters[5].srcX) = 0;
    (g_SubmarineHudScoreCounters[5].srcY) = 48;
    (g_SubmarineHudScoreCounters[5].alpha) = 5;
    (g_SubmarineHudScoreCounters[5].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[5].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[5].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[5].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[5].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[6].x) = 70;
    (g_SubmarineHudScoreCounters[6].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[6].width) = 13;
    (g_SubmarineHudScoreCounters[6].height) = 20;
    (g_SubmarineHudScoreCounters[6].srcX) = 0;
    (g_SubmarineHudScoreCounters[6].srcY) = 48;
    (g_SubmarineHudScoreCounters[6].alpha) = 5;
    (g_SubmarineHudScoreCounters[6].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[6].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[6].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[6].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[6].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[7].x) = 100;
    (g_SubmarineHudScoreCounters[7].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[7].width) = 13;
    (g_SubmarineHudScoreCounters[7].height) = 20;
    (g_SubmarineHudScoreCounters[7].srcX) = 0;
    (g_SubmarineHudScoreCounters[7].srcY) = 48;
    (g_SubmarineHudScoreCounters[7].alpha) = 5;
    (g_SubmarineHudScoreCounters[7].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[7].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[7].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[7].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[7].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[8].x) = 130;
    (g_SubmarineHudScoreCounters[8].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[8].width) = 13;
    (g_SubmarineHudScoreCounters[8].height) = 20;
    (g_SubmarineHudScoreCounters[8].srcX) = 0;
    (g_SubmarineHudScoreCounters[8].srcY) = 48;
    (g_SubmarineHudScoreCounters[8].alpha) = 5;
    (g_SubmarineHudScoreCounters[8].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[8].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[8].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[8].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[8].batches[3]) = g_SubmarineHudTextAtlasD;
    (g_SubmarineHudScoreCounters[9].x) = 160;
    (g_SubmarineHudScoreCounters[9].y) = 0xFFE0;
    (g_SubmarineHudScoreCounters[9].width) = 13;
    (g_SubmarineHudScoreCounters[9].height) = 20;
    (g_SubmarineHudScoreCounters[9].srcX) = 0;
    (g_SubmarineHudScoreCounters[9].srcY) = 48;
    (g_SubmarineHudScoreCounters[9].alpha) = 5;
    (g_SubmarineHudScoreCounters[9].depth) = FLOAT_0_01_BITS;
    (g_SubmarineHudScoreCounters[9].batches[0]) = g_SubmarineHudTextAtlasA;
    (g_SubmarineHudScoreCounters[9].batches[1]) = g_SubmarineHudTextAtlasB;
    (g_SubmarineHudScoreCounters[9].batches[2]) = g_SubmarineHudTextAtlasC;
    (g_SubmarineHudScoreCounters[9].batches[3]) = g_SubmarineHudTextAtlasD;
    if (scaleMode != 2) {
        for (scoreIndex = 0; scoreIndex < 10; scoreIndex++) {
            g_SubmarineHudScoreCounters[scoreIndex].batches[0] = g_SubmarineHudTextAtlas;
            g_SubmarineHudScoreCounters[scoreIndex].batches[1] = 0;
            g_SubmarineHudScoreCounters[scoreIndex].batches[2] = 0;
            g_SubmarineHudScoreCounters[scoreIndex].batches[3] = 0;
        }
    }
}
} /* extern "C" */

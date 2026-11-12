// src/ff7/field/src/field_fx_fade_overlays.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Selects and drives the field's effect render objects and fades: creates and releases
//   them, sets the viewport, steps fade-in and fade-out, and blends colors over time.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/pfile.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/enums/field.h"
#include "ff7/api/field.h"
#include "ff7/api/menu.h"
#include <string.h>
#include "ff7/field.h"
#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"
extern "C" {

#pragma intrinsic(strcpy)
// clang-format off
// GLOBAL: FF7 0x00906a70
int g_FieldFxFadeColor = 0x80FFFFFF;
// A byte table indexed by g_FieldActorState.state.fadeAdjust (0, 1, 2, ... with gaps).
// GLOBAL: FF7 0x00906a78
unsigned char g_FieldFxOffsetIndexTable[0x110] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
    0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x91, 0x92, 0x93, 0x94, 0x95,
    0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe,
    0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe,
    0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x00cffab0
FieldPackedBgra g_FieldFxLastPackedColor = {0};
// GLOBAL: FF7 0x00cffab4
unsigned char g_FieldFxFadeRed[0x4] = {0};
// GLOBAL: FF7 0x00cffab8
unsigned char g_FieldFxFadeBlue[0x4] = {0};
// GLOBAL: FF7 0x00cffabc
unsigned char g_FieldFxFadeGreen[0x4] = {0};
// GLOBAL: FF7 0x00cffac0
void * g_pFieldFxRenderObjects[3] = {0};
// The three per-slot "draw requested" words (g_FieldFxSlotDrawRequested, g_FieldFxSlotD0, g_FieldFxSlotD4), indexed
// by g_FieldFxActiveSlot. Defined as three consecutive ints because VC5 cannot start an array of
// 8 or more bytes at an address congruent to 4 mod 8.
int g_FieldFxSlotDrawRequested = 0;
#define g_FieldFxSlotDrawRequests (&g_FieldFxSlotDrawRequested)
// GLOBAL: FF7 0x00cffad0
int g_FieldFxSlotD0 = 0;
// GLOBAL: FF7 0x00cffad4
int g_FieldFxSlotD4 = 0;
// GLOBAL: FF7 0x00cffad8
int g_FieldFxActiveSlot = 0;
// GLOBAL: FF7 0x00cffadc
int g_FieldFxViewportX = 0;
// GLOBAL: FF7 0x00cffae0
int g_FieldFxViewportY = 0;
// GLOBAL: FF7 0x00cffae4
int g_FieldFxViewportW = 0;
// GLOBAL: FF7 0x00cffae8
int g_FieldFxViewportH = 0;
unsigned char g_bssPad_00cffaec[4] = {0};
// GLOBAL: FF7 0x00cffaf0
int g_FieldFxStepCounter = 0;
// GLOBAL: FF7 0x00cffaf4
int g_FieldFxFlagF4 = 0;
// GLOBAL: FF7 0x00cffaf8
unsigned char g_FieldEventCompleteMessage[32] = {0};
unsigned char g_bssPad_00cffb18[96] = {0};
// GLOBAL: FF7 0x00cffb78
int g_FieldLogicUnusedB78 = 0;
// GLOBAL: FF7 0x00cffb7c
int g_FieldLogicExitState = 0;
// GLOBAL: FF7 0x00cffb80
int g_FieldTransitionPendingFlag = 0;
// GLOBAL: FF7 0x00cffb84
int g_FieldLogicStateFlag = 0;
unsigned char g_bssPad_00cffb88[4] = {0};
// clang-format on

/*
 * Normalizes a one-valued request into the field state flag, clearing all other values.
 */
// FUNCTION: FF7 0x0063ac20
void __cdecl Field_SelectFxRenderObject(int value)
{
    if (value == 1) {
        g_FieldFxActiveSlot = value;
    } else {
        g_FieldFxActiveSlot = 0;
    }
}

/*
 * Stores the four arguments into g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW and g_FieldFxViewportH
 * respectively.
 */
// FUNCTION: FF7 0x0063ac3f
void __cdecl Field_SetFxViewportRect(int left, int top, int width, int height)
{
    g_FieldFxViewportX = left;
    g_FieldFxViewportY = top;
    g_FieldFxViewportW = width;
    g_FieldFxViewportH = height;
}

/*
 * Creates one render object per blend mode (0, 1 and 2) from the context's render object bank,
 * stores each handle in g_pFieldFxRenderObjects and clears its draw-requested word, then sets the
 * fade viewport to the field viewport. Creation failures are stored without further handling.
 */
// FUNCTION: FF7 0x0063ac66
void __cdecl Field_CreateFxRenderObjects(struct GfxEngineContext *context)
{
    PfileLoadOptions loadOptions;

    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &loadOptions);
    loadOptions.renderFlags |= 0x4000;
    g_pFieldFxRenderObjects[0] = Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)context->renderObjectBank);
    g_FieldFxSlotDrawRequested = 0;
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, &loadOptions);
    g_pFieldFxRenderObjects[1] = Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)context->renderObjectBank);
    g_FieldFxSlotDrawRequests[1] = 0;
    Pfile_InitLoadOptionsWithBlend(BLEND_SUBTRACTIVE, &loadOptions);
    g_pFieldFxRenderObjects[2] = Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)context->renderObjectBank);
    g_FieldFxSlotDrawRequests[2] = 0;
    Field_SetFxViewportRect(g_FieldViewportX, g_FieldViewportY, g_FieldViewportWidth, g_FieldViewportHeight);
}

/*
 * Releases the three fade render objects and clears their slots. Field_FreeModuleResources calls it.
 */
// FUNCTION: FF7 0x0063ad47
void __cdecl Field_DestroyFxRenderObjects(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (g_pFieldFxRenderObjects[i] != 0) {
            DestroyRenderObject(g_pFieldFxRenderObjects[i]);
            g_pFieldFxRenderObjects[i] = 0;
        }
    }
}

/*
 * When the draw-requested word of the active slot g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] is nonzero,
 * clears it and draws that slot's g_pFieldFxRenderObjects entry with drawParam.
 */
// FUNCTION: FF7 0x0063ad97
void __cdecl Field_FxSubmitFadeOverlay(GfxEngineContext *drawParam)
{
    if (g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] != 0) {
        g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 0;
        Gfx_DrawRenderObjectCall((GfxRenderObject *)g_pFieldFxRenderObjects[g_FieldFxActiveSlot], drawParam);
    }
}

/*
 * Clears the fade RGB bytes, packs black with alpha's low byte in the alpha channel, stores it in
 * g_FieldFxLastPackedColor, calls Menu_SetDigitBackdropQuad(1, alpha), submits a flat overlay quad
 * on the active render object, and marks the active slot draw-requested.
 */
// FUNCTION: FF7 0x0063add5
void __cdecl Field_FxDrawBlackOverlayAlpha(int alpha)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[4];
        int packedValue;
        struct {
            char blueByte;
            char zeroByteMid;
            char zeroByteLow;
            char alphaByte;
        };
    } frame;
#pragma pack(pop)

    g_FieldFxFadeRed[0] = 0;
    g_FieldFxFadeGreen[0] = 0;
    g_FieldFxFadeBlue[0] = 0;
    frame.blueByte = 0;
    frame.zeroByteMid = 0;
    frame.zeroByteLow = 0;
    frame.alphaByte = alpha;
    g_FieldFxLastPackedColor.packed = frame.packedValue;
    Menu_SetDigitBackdropQuad(1, (char)alpha);
    Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedValue, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
    g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
}

/*
 * Updates field fade colors through packed BGRA channels. All-three arguments 256 clear
 * g_FieldFxStepCounter. When g_FieldTransitionPendingFlag is set, draws black with alpha from
 * g_FieldFxOffsetIndexTable. In the ordinary path stores raw channel bytes: slot 0 draws direct RGB
 * with alpha capped at 255 and updates Menu_SetDigitBackdropQuad, while other slots draw
 * fadeAdjust-scaled RGB with alpha 128. When g_FieldFxStepCounter is nonzero, slots other than 1
 * compute complemented RGB into g_FieldFxFadeColor unless field data is ready, while slot 1 draws
 * scaled RGB.
 */
// FUNCTION: FF7 0x0063ae66
void __cdecl Field_FxApplyScaledFadeColor(short red, short green, short blue)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        union { int alphaValue; char alphaValueLo; };
        struct { unsigned char pad_to_4[0x4]; int packedColor; };
        struct { unsigned char pad_to_4b[0x4]; char blueByte; union { int greenWord; char greenByte; }; };
        struct { unsigned char pad_to_6[0x6]; union { int redWord; char redByte; }; };
        struct { unsigned char pad_to_7[0x7]; char alphaByte; };
    } frame;
#pragma pack(pop)

    if (red == 0x100 && green == 0x100 && blue == 0x100) {
        g_FieldFxStepCounter = 0;
    }
    if (g_FieldTransitionPendingFlag != 0) {
        frame.blueByte = 0;
        frame.greenByte = 0;
        frame.redByte = 0;
        frame.alphaByte = (char)g_FieldFxOffsetIndexTable[g_FieldActorState.state.fadeAdjust];
        g_FieldFxLastPackedColor.packed = frame.packedColor;
        Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
        g_FieldFxSlotDrawRequested = 1;
    } else if (g_FieldFxStepCounter == 0) {
        g_FieldFxFadeRed[0] = red;
        g_FieldFxFadeGreen[0] = green;
        g_FieldFxFadeBlue[0] = blue;
        if (g_FieldFxActiveSlot == 0) {
            frame.blueByte = blue;
            frame.greenByte = green;
            frame.redByte = red;
            if (g_FieldActorState.state.fadeAdjust > 0xFF) {
                frame.alphaValue = 0xFF;
            } else {
                frame.alphaValue = g_FieldActorState.state.fadeAdjust;
            }
            frame.alphaByte = frame.alphaValueLo;
            Menu_SetDigitBackdropQuad(1, g_FieldActorState.state.fadeAdjustLo);
            g_FieldFxLastPackedColor.packed = frame.packedColor;
            Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
            g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
        } else {
            frame.blueByte = (g_FieldActorState.state.fadeAdjust * blue) / 256;
            frame.greenByte = (g_FieldActorState.state.fadeAdjust * green) / 256;
            frame.redByte = (g_FieldActorState.state.fadeAdjust * red) / 256;
            frame.alphaByte = 0x80;
            g_FieldFxLastPackedColor.packed = frame.packedColor;
            Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
            g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
        }
    } else {
        if (g_FieldFxActiveSlot != 1) {
            blue = 0x100 - blue;
            green = 0x100 - green;
            red = 0x100 - red;
            frame.blueByte = ((0x100 - g_FieldActorState.state.fadeAdjust + 0x10) * blue) / 256;
            frame.greenByte = ((0x100 - g_FieldActorState.state.fadeAdjust + 0x10) * green) / 256;
            frame.redByte = ((0x100 - (g_FieldActorState.state.fadeAdjust / 2)) * red) / 256;
            if (g_FieldDataReady != 0) {
                return;
            }
            g_FieldFxFadeColor = (frame.redWord & 0xFF) << 0x10 | 0x80000000 | (frame.greenWord & 0xFF) << 8 | frame.packedColor & 0xFF;
        } else {
            g_FieldFxFadeRed[0] = red;
            g_FieldFxFadeGreen[0] = green;
            g_FieldFxFadeBlue[0] = blue;
            frame.blueByte = (g_FieldActorState.state.fadeAdjust * blue) / 256;
            frame.greenByte = (g_FieldActorState.state.fadeAdjust * green) / 256;
            frame.redByte = (g_FieldActorState.state.fadeAdjust * red) / 256;
            Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
            g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
        }
        g_FieldFxLastPackedColor.packed = frame.packedColor;
    }
}

/*
 * Packs three narrowed color arguments as BGRA. When g_FieldFxFlagF4 is zero, stores the component
 * globals, submits a quad with alpha from fadeAdjustLo for slot 0 or 128 otherwise, marks the slot,
 * and stores g_FieldFxLastPackedColor. When g_FieldFxFlagF4 is set, uses alpha 128; slot 1 stores
 * components and submits, while other slots return when field data is ready or compute an inverse
 * color from 256 minus each component.
 */
// FUNCTION: FF7 0x0063b25f
void __cdecl Field_FxApplyFlatFadeColor(short red, short green, short blue)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        int packedColor;
        struct { char blueByte; union { int greenWord; char greenByte; }; };
        struct { unsigned char pad_to_2[0x2]; union { int redWord; char redByte; }; };
        struct { unsigned char pad_to_3[0x3]; char alphaByte; };
    } frame;
#pragma pack(pop)

    if (g_FieldFxFlagF4 == 0) {
        g_FieldFxFadeRed[0] = red;
        g_FieldFxFadeGreen[0] = green;
        g_FieldFxFadeBlue[0] = blue;
        if (g_FieldFxActiveSlot == 0) {
            Menu_SetDigitBackdropQuad(1, g_FieldActorState.state.fadeAdjustLo);
            frame.blueByte = blue;
            frame.greenByte = green;
            frame.redByte = red;
            frame.alphaByte = g_FieldActorState.state.fadeAdjustLo;
            Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
            g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
        } else {
            frame.blueByte = blue;
            frame.greenByte = green;
            frame.redByte = red;
            frame.alphaByte = 0x80;
            Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
            g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
        }
        g_FieldFxLastPackedColor.packed = frame.packedColor;
    } else {
        frame.blueByte = blue;
        frame.greenByte = green;
        frame.redByte = red;
        frame.alphaByte = 0x80;
        if (g_FieldFxActiveSlot != 1) {
            if (g_FieldDataReady != 0) {
                return;
            }
            g_FieldFxFadeColor = 0x100 - (frame.redWord & 0xFF) << 0x10 | 0x80000000 | 0x100 - (frame.greenWord & 0xFF) << 8 | 0x100 - (frame.packedColor & 0xFF);
        } else {
            g_FieldFxFadeRed[0] = red;
            g_FieldFxFadeGreen[0] = green;
            g_FieldFxFadeBlue[0] = blue;
            Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
            g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
        }
        g_FieldFxLastPackedColor.packed = frame.packedColor;
    }
}

/*
 * Clears the byte at g_FieldActorState.state.fieldHidden and subtracts
 * g_FieldActorState.state.fadeSpeed from g_FieldActorState.state.fadeAdjust; clears
 * g_FieldActorState.state.fadeType and g_FieldActorState.state.fadeAdjust when the result is
 * nonpositive or g_FieldLogicActive equals one.
 */
// FUNCTION: FF7 0x0063b464
void __cdecl Field_FxFadeStepOut(void)
{
    g_FieldActorState.state.fieldHidden = 0;
    g_FieldActorState.state.fadeAdjust -= g_FieldActorState.state.fadeSpeed;
    if (g_FieldActorState.state.fadeAdjust <= 0 || g_FieldLogicActive == 1) {
        g_FieldActorState.state.fadeType = FIELD_FADE_NONE;
        g_FieldActorState.state.fadeAdjust = 0;
    }
}

/*
 * Clears g_FieldActorState.state.fieldHidden, adds g_FieldActorState.state.fadeSpeed to
 * g_FieldActorState.state.fadeAdjust, and replaces a result at least 0x100 with 0xFF.
 */
// FUNCTION: FF7 0x0063b4ac
void __cdecl Field_FxFadeStepIn(void)
{
    g_FieldActorState.state.fieldHidden = 0;
    g_FieldActorState.state.fadeAdjust += g_FieldActorState.state.fadeSpeed;
    if (g_FieldActorState.state.fadeAdjust >= 0x100) {
        g_FieldActorState.state.fadeAdjust = 0xFF;
    }
}

/*
 * Forms a short delta end-start and returns start+delta*step/total narrowed to short. No zero-
 * total check is present.
 */
// FUNCTION: FF7 0x0063b4e3
short __cdecl Field_FxLerpComponent(short start, short end, short step, short total)
{
    short delta;

    delta = end - start;
    start += delta * step / total;
    return start;
}

/*
 * Clears fieldHidden, increments/clamps fadeAdjust and copies target RGB shorts into start at
 * completion, then interpolates RGB components via Field_FxLerpComponent, submits an opaque flat
 * overlay quad, and marks the active slot draw-requested.
 */
// FUNCTION: FF7 0x0063b51a
void __cdecl Field_FxStepColorBlendTransition(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[4];
        int packedColor;
        struct {
            char blueByte;
            char greenByte;
            char redByte;
            char alphaByte;
        };
    } frame;
#pragma pack(pop)

    g_FieldActorState.state.fieldHidden = 0;
    g_FieldActorState.state.fadeAdjust++;
    if (g_FieldActorState.state.fadeAdjust >= g_FieldActorState.state.fadeSpeed) {
        g_FieldActorState.state.fadeRedFrom = g_FieldActorState.state.fadeRedTarget;
        g_FieldActorState.state.fadeGreenFrom = g_FieldActorState.state.fadeGreenTarget;
        g_FieldActorState.state.fadeBlueFrom = g_FieldActorState.state.fadeBlueTarget;
        g_FieldActorState.state.fadeAdjust = g_FieldActorState.state.fadeSpeed;
    }
    g_FieldActorState.state.red = Field_FxLerpComponent((short)g_FieldActorState.state.fadeRedFrom, (short)g_FieldActorState.state.fadeRedTarget, g_FieldActorState.state.fadeAdjust, g_FieldActorState.state.fadeSpeed);
    g_FieldActorState.state.green = Field_FxLerpComponent((short)g_FieldActorState.state.fadeGreenFrom, (short)g_FieldActorState.state.fadeGreenTarget, g_FieldActorState.state.fadeAdjust, g_FieldActorState.state.fadeSpeed);
    g_FieldActorState.state.blue = Field_FxLerpComponent((short)g_FieldActorState.state.fadeBlueFrom, (short)g_FieldActorState.state.fadeBlueTarget, g_FieldActorState.state.fadeAdjust, g_FieldActorState.state.fadeSpeed);
    frame.blueByte = g_FieldActorState.state.blueLo;
    frame.greenByte = g_FieldActorState.state.greenLo;
    frame.redByte = g_FieldActorState.state.redLo;
    frame.alphaByte = 0xFF;
    g_FieldFxFadeRed[0] = g_FieldActorState.state.redLo;
    g_FieldFxFadeGreen[0] = g_FieldActorState.state.greenLo;
    g_FieldFxFadeBlue[0] = g_FieldActorState.state.blueLo;
    Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
    g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
}

/*
 * Clears fieldHidden, increments/clamps fadeAdjust and copies target RGB shorts into start at
 * completion, then interpolates RGB components via Field_FxLerpComponent. When field data is ready,
 * submits a black quad whose alpha is redLo and marks the active slot; otherwise stores an opaque
 * color made from 255 minus the low bytes of the three components.
 */
// FUNCTION: FF7 0x0063b692
void __cdecl Field_FxStepInverseColorBlendTransition(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[4];
        int packedColor;
        struct {
            char blueByte;
            char greenByte;
            char redByte;
            char alphaByte;
        };
    } frame;
#pragma pack(pop)

    g_FieldActorState.state.fieldHidden = 0;
    g_FieldActorState.state.fadeAdjust++;
    if (g_FieldActorState.state.fadeAdjust >= g_FieldActorState.state.fadeSpeed) {
        g_FieldActorState.state.fadeRedFrom = g_FieldActorState.state.fadeRedTarget;
        g_FieldActorState.state.fadeGreenFrom = g_FieldActorState.state.fadeGreenTarget;
        g_FieldActorState.state.fadeBlueFrom = g_FieldActorState.state.fadeBlueTarget;
        g_FieldActorState.state.fadeAdjust = g_FieldActorState.state.fadeSpeed;
    }
    g_FieldActorState.state.red = Field_FxLerpComponent((short)g_FieldActorState.state.fadeRedFrom, (short)g_FieldActorState.state.fadeRedTarget, g_FieldActorState.state.fadeAdjust, g_FieldActorState.state.fadeSpeed);
    g_FieldActorState.state.green = Field_FxLerpComponent((short)g_FieldActorState.state.fadeGreenFrom, (short)g_FieldActorState.state.fadeGreenTarget, g_FieldActorState.state.fadeAdjust, g_FieldActorState.state.fadeSpeed);
    g_FieldActorState.state.blue = Field_FxLerpComponent((short)g_FieldActorState.state.fadeBlueFrom, (short)g_FieldActorState.state.fadeBlueTarget, g_FieldActorState.state.fadeAdjust, g_FieldActorState.state.fadeSpeed);
    frame.blueByte = g_FieldActorState.state.blueLo;
    frame.greenByte = g_FieldActorState.state.greenLo;
    frame.redByte = g_FieldActorState.state.redLo;
    frame.alphaByte = 0xFF;
    if (g_FieldDataReady != 0) {
        frame.blueByte = 0;
        frame.greenByte = 0;
        frame.redByte = 0;
        frame.alphaByte = g_FieldActorState.state.redLo;
        Gfx_EmitFlatRect(g_FieldFxViewportX, g_FieldFxViewportY, g_FieldFxViewportW, g_FieldFxViewportH, frame.packedColor, 0, 0x358637BD, (int)g_pFieldFxRenderObjects[g_FieldFxActiveSlot]);
        g_FieldFxSlotDrawRequests[g_FieldFxActiveSlot] = 1;
    } else {
        g_FieldFxFadeColor = ((0xFF - (*(int *)&frame.redByte & 0xFF)) << 16) | 0xFF000000 | ((0xFF - (*(int *)&frame.greenByte & 0xFF)) << 8) | (0xFF - (frame.packedColor & 0xFF));
    }
}

/*
 * Dispatches the field effect mode. Zero clears common state and resets white color/fade values,
 * except a special black-color condition that sets the mode-selector global to 3 and returns. Modes
 * 1/2/7/8 run mode-2 fade-out/in with complemented RGB and one of two color helpers; modes 5/6/9/10
 * use mode 1 and direct RGB, with 6/10 clearing the persistent flag. Mode 4 sets that flag. Modes
 * 3/13 increment the counter and finish only at exact counts 34/18, otherwise submit a proportional
 * 255 ramp. Modes 11/12 call their specialized transition helpers. Most cases clear
 * g_FieldTransitionPendingFlag, but mode 1 preserves it and unknown modes do nothing.
 */
// FUNCTION: FF7 0x0063b84b
void __cdecl Field_UpdateFieldFx(void)
{
    switch (g_FieldActorState.state.fadeType) {
    case FIELD_FADE_NONE:
        g_FieldTransitionPendingFlag = 0;
        if (g_FieldActorState.state.requestCode == FIELD_REQUEST_MAP_JUMP && g_FieldFxFadeColor == 0xFF000000) {
            g_FieldLogicExitState = FIELD_EXIT_FADE_DONE;
            return;
        }
        g_FieldFxStepCounter = 0;
        g_FieldFxFlagF4 = 0;
        g_FieldFxFadeColor = 0x80FFFFFF;
        g_FieldFxLastPackedColor.red = 0xFF;
        g_FieldActorState.state.fieldHidden = 0;
        g_FieldActorState.state.fadeRedFrom = 0;
        g_FieldActorState.state.fadeGreenFrom = 0;
        g_FieldActorState.state.fadeBlueFrom = 0;
        Menu_SetDigitBackdropQuad(0, 0);
        break;
    case FIELD_FADE_SCALED_COMPLEMENT_OUT:
        Field_SelectFxRenderObject(2);
        Field_FxFadeStepOut();
        Field_FxApplyScaledFadeColor(0x100 - g_FieldActorState.state.red, 0x100 - g_FieldActorState.state.green, 0x100 - g_FieldActorState.state.blue);
        break;
    case FIELD_FADE_SCALED_COMPLEMENT_IN:
        g_FieldTransitionPendingFlag = 0;
        Field_SelectFxRenderObject(2);
        Field_FxFadeStepIn();
        Field_FxApplyScaledFadeColor(0x100 - g_FieldActorState.state.red, 0x100 - g_FieldActorState.state.green, 0x100 - g_FieldActorState.state.blue);
        break;
    case FIELD_FADE_FLAT_COMPLEMENT_OUT:
        g_FieldTransitionPendingFlag = 0;
        Field_SelectFxRenderObject(2);
        Field_FxFadeStepOut();
        Field_FxApplyFlatFadeColor(0x100 - g_FieldActorState.state.red, 0x100 - g_FieldActorState.state.green, 0x100 - g_FieldActorState.state.blue);
        break;
    case FIELD_FADE_FLAT_COMPLEMENT_IN:
        g_FieldTransitionPendingFlag = 0;
        Field_SelectFxRenderObject(2);
        Field_FxFadeStepIn();
        Field_FxApplyFlatFadeColor(0x100 - g_FieldActorState.state.red, 0x100 - g_FieldActorState.state.green, 0x100 - g_FieldActorState.state.blue);
        break;
    case FIELD_FADE_BLACK_RAMP_LONG:
        g_FieldTransitionPendingFlag = 0;
        g_FieldActorState.state.fadeAdjust++;
        if (g_FieldActorState.state.fadeAdjust == 0x22) {
            g_FieldActorState.state.fadeType = FIELD_FADE_NONE;
            g_MenuEntryState = 0;
        } else {
            Field_SelectFxRenderObject(2);
            Field_FxDrawBlackOverlayAlpha(g_FieldActorState.state.fadeAdjust * 0xFF / 0x22);
        }
        break;
    case FIELD_FADE_BLACK_RAMP_SHORT:
        g_FieldTransitionPendingFlag = 0;
        g_FieldActorState.state.fadeAdjust++;
        if (g_FieldActorState.state.fadeAdjust == 0x12) {
            g_FieldActorState.state.fadeType = FIELD_FADE_NONE;
            g_MenuEntryState = 0;
        } else {
            Field_SelectFxRenderObject(2);
            Field_FxDrawBlackOverlayAlpha(g_FieldActorState.state.fadeAdjust * 0xFF / 0x12);
        }
        break;
    case FIELD_FADE_HIDE_FIELD:
        g_FieldTransitionPendingFlag = 0;
        g_FieldActorState.state.fieldHidden = 1;
        break;
    case FIELD_FADE_SCALED_OUT:
        g_FieldTransitionPendingFlag = 0;
        Field_SelectFxRenderObject(1);
        Field_FxFadeStepOut();
        Field_FxApplyScaledFadeColor((short)g_FieldActorState.state.red, (short)g_FieldActorState.state.green, (short)g_FieldActorState.state.blue);
        break;
    case FIELD_FADE_SCALED_IN:
        g_FieldTransitionPendingFlag = 0;
        g_FieldActorState.state.fieldHidden = 0;
        Field_SelectFxRenderObject(1);
        Field_FxFadeStepIn();
        Field_FxApplyScaledFadeColor((short)g_FieldActorState.state.red, (short)g_FieldActorState.state.green, (short)g_FieldActorState.state.blue);
        break;
    case FIELD_FADE_FLAT_OUT:
        g_FieldTransitionPendingFlag = 0;
        Field_SelectFxRenderObject(1);
        Field_FxFadeStepOut();
        Field_FxApplyFlatFadeColor((short)g_FieldActorState.state.red, (short)g_FieldActorState.state.green, (short)g_FieldActorState.state.blue);
        break;
    case FIELD_FADE_FLAT_IN:
        g_FieldTransitionPendingFlag = 0;
        g_FieldActorState.state.fieldHidden = 0;
        Field_SelectFxRenderObject(1);
        Field_FxFadeStepIn();
        Field_FxApplyFlatFadeColor((short)g_FieldActorState.state.red, (short)g_FieldActorState.state.green, (short)g_FieldActorState.state.blue);
        break;
    case FIELD_FADE_COLOR_BLEND:
        g_FieldTransitionPendingFlag = 0;
        Field_SelectFxRenderObject(1);
        Field_FxStepColorBlendTransition();
        break;
    case FIELD_FADE_INVERSE_COLOR_BLEND:
        g_FieldTransitionPendingFlag = 0;
        Field_SelectFxRenderObject(2);
        Field_FxStepInverseColorBlendTransition();
    }
}
} /* extern "C" */

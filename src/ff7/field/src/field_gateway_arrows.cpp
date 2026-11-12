// src/ff7/field/src/field_gateway_arrows.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Draws the animated gateway arrows and trigger markers that mark field gateways.
#include "ff7/types/field.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"

// The quad table below has a non-constant first member (the hand image buffer pointer), so VC5
// initializes the whole aggregate dynamically: its member stores run in declaration order from the
// compiler-generated initializer. That initializer carries no FUNCTION marker.

extern "C" {

// GLOBAL: FF7 0x00d84fb0
FieldQuadRecord g_FieldGatewayArrowQuads[5] = {
    {g_pFieldHandImageBuffer, {0, 0, 0, 0, 0, 0, 0, 0}, {0.125f, 0, 0.125f, 0.969f, 0.246f, 0, 0.246f, 0.969f}, 0.00002f, 9},
    {g_pFieldHandImageBuffer, {0, 0, 0, 0, 0, 0, 0, 0}, {0.25f, 0, 0.25f, 0.969f, 0.371f, 0, 0.371f, 0.969f}, 0.00001f, 9},
    {g_pFieldHandImageBuffer, {0, 0, 0, 0, 0, 0, 0, 0}, {0.375f, 0, 0.375f, 0.969f, 0.496f, 0, 0.496f, 0.969f}, 0.00001f, 9},
    {g_pFieldHandImageBuffer, {0, 0, 0, 0, 0, 0, 0, 0}, {0.5f, 0, 0.5f, 0.969f, 0.621f, 0, 0.621f, 0.969f}, 0.00001f, 9},
    {g_pFieldHandImageBuffer, {0, 0, 0, 0, 0, 0, 0, 0}, {0.625f, 0, 0.625f, 0.969f, 0.746f, 0, 0.746f, 0.969f}, 0.00001f, 9},
};


#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// Not declared in the API headers; C++ requires a prototype.

// clang-format off
// Left uninitialized, like the quad table g_FieldGatewayArrowQuads (its dynamic initializer makes it .bss): VC5 emits
// uninitialized definitions first, in its symbol-hash order, which for these three names is retail's order;
// with `= 0` the table came first.
// GLOBAL: FF7 0x00d84fa8
unsigned short g_FieldArrowAnimFrame;
unsigned char g_bssPad_00d84fac[4];
unsigned char g_bssPad_00d8512c[4] = {0};

// Draws the animated arrow over each active gateway box and a marker over each trigger marker.
// FUNCTION: FF7 0x0064da3b
void __cdecl Field_DrawGatewayArrows(void *matrix, FieldGatewayBox *gatewayBoxes)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        int persFlag;
        struct { unsigned char pad_to_4[0x4]; int arrowFrame; };
        struct { unsigned char pad_to_8[0x8]; short gatewayIndex; };
        struct { unsigned char pad_to_C[0xC]; short centerX; };
        struct { unsigned char pad_to_E[0xE]; short centerY; };
        struct { unsigned char pad_to_10[0x10]; short centerZ; };
        struct { unsigned char pad_to_14[0x14]; int depth; };
    } frame;
#pragma pack(pop)

    frame.arrowFrame = 0;
    if ((gSavemap.fieldHelp == 1 && ((char *)&g_EnginePreviousMode)[54] == 0) || gSavemap.fieldHelp == 2) {
        PushRotMatrix();
        SetRotMatrix(matrix);
        SetTransMatrix(matrix);
        SetGeomOffset(g_FieldDisplayOffsetX, g_FieldDisplayHeight);
        for (frame.gatewayIndex = 0; frame.gatewayIndex < 12; frame.gatewayIndex++) {
            if (g_pFieldScrollBounds->gatewayArrowShown[frame.gatewayIndex] == 1) {
                frame.centerX = (gatewayBoxes[frame.gatewayIndex].x0 + gatewayBoxes[frame.gatewayIndex].x1) / 2;
                frame.centerY = (gatewayBoxes[frame.gatewayIndex].y0 + gatewayBoxes[frame.gatewayIndex].y1) / 2;
                frame.centerZ = (gatewayBoxes[frame.gatewayIndex].z0 + gatewayBoxes[frame.gatewayIndex].z1) / 2;
                if (frame.centerX != 0 || frame.centerY != 0) {
                    RotTransPers((struct SVECTOR *)(&frame.centerX), &frame.centerX, &frame.depth, &frame.persFlag);
                    frame.arrowFrame = g_FieldArrowAnimFrame >> 2 & 3;
                    g_FieldGatewayArrowQuads[frame.arrowFrame].image = g_pFieldHandImageBuffer;
                    g_FieldGatewayArrowQuads[0].image = g_pFieldHandImageBuffer;
                    g_FieldGatewayArrowQuads[frame.arrowFrame].position[0] = (float)((frame.centerX * g_FieldDisplayScaleFactor - g_FieldDisplayScaleFactor * 7) + (g_FieldDisplayHalfCenterX + g_FieldPaletteTargetOrgX * g_FieldDisplayScaleFactor));
                    g_FieldGatewayArrowQuads[0].positionBits[0] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[0];
                    g_FieldGatewayArrowQuads[frame.arrowFrame].position[1] = (float)((frame.centerY * g_FieldDisplayScaleFactor - (g_FieldDisplayScaleFactor << 3)) + (g_FieldDisplayHalfCenterY + g_FieldPaletteTargetOrgY * g_FieldDisplayScaleFactor)) - (g_FieldDisplayScaleFactor << 2);
                    g_FieldGatewayArrowQuads[0].positionBits[1] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[1];
                    g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[2] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[0];
                    g_FieldGatewayArrowQuads[0].positionBits[2] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[2];
                    g_FieldGatewayArrowQuads[frame.arrowFrame].position[3] = g_FieldGatewayArrowQuads[frame.arrowFrame].position[1] + g_FieldDisplayCenterXFloat;
                    g_FieldGatewayArrowQuads[0].positionBits[3] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[3];
                    g_FieldGatewayArrowQuads[frame.arrowFrame].position[4] = g_FieldGatewayArrowQuads[frame.arrowFrame].position[0] + g_FieldDisplayCenterXFloat;
                    g_FieldGatewayArrowQuads[0].positionBits[4] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[4];
                    g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[5] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[1];
                    g_FieldGatewayArrowQuads[0].positionBits[5] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[5];
                    g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[6] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[4];
                    g_FieldGatewayArrowQuads[0].positionBits[6] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[6];
                    g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[7] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[3];
                    g_FieldGatewayArrowQuads[0].positionBits[7] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[7];
                    g_FieldGatewayArrowQuads[0].code = 9;
                    g_FieldGatewayArrowQuads[frame.arrowFrame].code = 9;
                    Field_EmitQuadFromRecord(&g_FieldGatewayArrowQuads[frame.arrowFrame]);
                    g_FieldHandImageType = 1;
                }
            }
        }
        for (frame.gatewayIndex = 0; frame.gatewayIndex < 12; frame.gatewayIndex++) {
            if (g_pFieldScrollBounds->triggerMarkers[frame.gatewayIndex].state != 0) {
                frame.centerX = g_pFieldScrollBounds->triggerMarkers[frame.gatewayIndex].x;
                frame.centerY = g_pFieldScrollBounds->triggerMarkers[frame.gatewayIndex].y;
                frame.centerZ = g_pFieldScrollBounds->triggerMarkers[frame.gatewayIndex].z;
                RotTransPers((struct SVECTOR *)(&frame.centerX), &frame.centerX, &frame.depth, &frame.persFlag);
                frame.arrowFrame = g_FieldArrowAnimFrame >> 2 & 3;
                g_FieldGatewayArrowQuads[frame.arrowFrame].image = g_pFieldHandImageBuffer;
                g_FieldGatewayArrowQuads[0].image = g_pFieldHandImageBuffer;
                g_FieldGatewayArrowQuads[frame.arrowFrame].position[0] = (float)((frame.centerX * g_FieldDisplayScaleFactor - g_FieldDisplayScaleFactor * 7) + (g_FieldDisplayHalfCenterX + g_FieldPaletteTargetOrgX * g_FieldDisplayScaleFactor));
                g_FieldGatewayArrowQuads[0].positionBits[0] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[0];
                g_FieldGatewayArrowQuads[frame.arrowFrame].position[1] = (float)((frame.centerY * g_FieldDisplayScaleFactor - (g_FieldDisplayScaleFactor << 3)) + (g_FieldDisplayHalfCenterY + g_FieldPaletteTargetOrgY * g_FieldDisplayScaleFactor) - (g_FieldDisplayScaleFactor << 2));
                g_FieldGatewayArrowQuads[0].positionBits[1] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[1];
                g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[2] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[0];
                g_FieldGatewayArrowQuads[0].positionBits[2] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[2];
                g_FieldGatewayArrowQuads[frame.arrowFrame].position[3] = g_FieldGatewayArrowQuads[frame.arrowFrame].position[1] + g_FieldDisplayCenterXFloat;
                g_FieldGatewayArrowQuads[0].positionBits[3] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[3];
                g_FieldGatewayArrowQuads[frame.arrowFrame].position[4] = g_FieldGatewayArrowQuads[frame.arrowFrame].position[0] + g_FieldDisplayCenterXFloat;
                g_FieldGatewayArrowQuads[0].positionBits[4] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[4];
                g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[5] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[1];
                g_FieldGatewayArrowQuads[0].positionBits[5] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[5];
                g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[6] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[4];
                g_FieldGatewayArrowQuads[0].positionBits[6] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[6];
                g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[7] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[3];
                g_FieldGatewayArrowQuads[0].positionBits[7] = g_FieldGatewayArrowQuads[frame.arrowFrame].positionBits[7];
                g_FieldGatewayArrowQuads[frame.arrowFrame].code = 9;
                if (g_pFieldScrollBounds->triggerMarkers[frame.gatewayIndex].state == 2) {
                    g_FieldGatewayArrowQuads[0].code = 4;
                    g_FieldGatewayArrowQuads[frame.arrowFrame].code = 4;
                }
                Field_EmitQuadFromRecord(&g_FieldGatewayArrowQuads[frame.arrowFrame]);
                g_FieldHandImageType = 1;
            }
        }
        PopRotMatrix();
        SetGeomOffset(g_FieldDisplayCenterX, g_FieldDisplayCenterY);
        g_FieldArrowAnimFrame++;
    }
}
} /* extern "C" */

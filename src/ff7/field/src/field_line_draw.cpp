// src/ff7/field/src/field_line_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Manages the field's line draw table and its batch buffer: resets the line records,
//   builds vertices for enabled lines, draws them with the supplied matrix, and emits or releases
//   the batch.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdlib.h>
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "ff7/types/field.h"
#include "lib/pfile.h"
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "ff7/field.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
#include "lib/enums/graphics.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00cffee0
struct FieldLineBatchRect g_FieldLineDrawBuffer = {0};
// GLOBAL: FF7 0x00cffef4
int g_FieldLineDrawActiveFlag = 0;
// GLOBAL: FF7 0x00cffef8
int g_FieldLineBatchPendingFlag = 0;
unsigned char g_bssPad_00cffefc[4] = {0};
// clang-format on

/*
 * Resets the 64 line records (disabled, random index i*4, step state i%8), sets the draw flag and the
 * three colour bytes to 0x20, initializes zero-blend load options and creates the line batch's render
 * object from the engine context's render object bank.
 */
// FUNCTION: FF7 0x00644a80
void __cdecl Field_InitLineDrawTable(struct GfxEngineContext *ctx, PfileLoadOptions *opts)
{
    int i;
    int zero;
    for (i = 0; i < 64; i++) {
        g_FieldLineDrawScratch[i].active = 0;
        g_FieldLineDrawScratch[i].randomIndex = i << 2;
        g_FieldLineDrawScratch[i].stepState = i % 8;
    }
    g_FieldLineDrawActiveFlag = 1;
    g_FieldLineDrawBuffer.colorChannel0 = 0x20;
    g_FieldLineDrawBuffer.colorChannel1 = 0x20;
    g_FieldLineDrawBuffer.colorChannel2 = 0x20;
    g_FieldLineBatchPendingFlag = 0;
    zero = 0;
    Pfile_InitLoadOptionsWithBlend(zero, opts);
    g_FieldLineDrawBuffer.batchNode = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, opts, 0,
                                               *(void **)&ctx->renderObjectBank);
}

/*
 * Destroys the line batch's render object when there is one and clears it.
 */
// FUNCTION: FF7 0x00644b43
void __cdecl Field_DestroyLineDrawRenderObject(void)
{
    if (g_FieldLineDrawBuffer.batchNode != 0) {
        DestroyRenderObject(g_FieldLineDrawBuffer.batchNode);
        g_FieldLineDrawBuffer.batchNode = 0;
    }
}

/*
 * Acquires a batch node and fills four 32-byte vertex records: the line's two display-scaled endpoints
 * and copies of both shifted one pixel in X, all with depth FLOAT_0_01_BITS and colour 0x80404040.
 * On success the batch is marked pending; on failure nothing changes.
 */
// FUNCTION: FF7 0x00644b69
void __cdecl Field_FillLineBatchNodeVertices(FieldLineBatchRect *lineRect)
{
    GfxTlVertex *vertPtr;
    int ax;
    GfxTlVertex *recordD;
    GfxTlVertex *vertexPtrC;
    int ay;
    int x1;
    int shadowStartX;
    int y1;
    int x1Shifted;
    int y0Copy;
    int endYSave;
    GfxTlVertex *lastRecPtr;

    if (AcquireBatchNode(1, (void *)lineRect->batchNode) != 0) {
        vertPtr = (GfxTlVertex *)(((GfxRenderObject *)lineRect->batchNode)->vertexCursor);
        ax = lineRect->startX * g_FieldDisplayScaleFactor;
        vertPtr->x = ax;
        ay = lineRect->startY * g_FieldDisplayScaleFactor;
        vertPtr->y = ay;
        vertPtr->zBits = FLOAT_0_01_BITS;
        vertPtr->color = 0x80404040;
        lastRecPtr = (GfxTlVertex *)(((GfxRenderObject *)lineRect->batchNode)->vertexCursor + 0x20);
        x1 = lineRect->endX * g_FieldDisplayScaleFactor;
        lastRecPtr->x = x1;
        y1 = lineRect->endY * g_FieldDisplayScaleFactor;
        lastRecPtr->y = y1;
        lastRecPtr->zBits = FLOAT_0_01_BITS;
        lastRecPtr->color = 0x80404040;
        vertexPtrC = (GfxTlVertex *)(((GfxRenderObject *)lineRect->batchNode)->vertexCursor + 0x40);
        shadowStartX = lineRect->startX * g_FieldDisplayScaleFactor + 1;
        vertexPtrC->x = shadowStartX;
        y0Copy = lineRect->startY * g_FieldDisplayScaleFactor;
        vertexPtrC->y = y0Copy;
        vertexPtrC->zBits = FLOAT_0_01_BITS;
        vertexPtrC->color = 0x80404040;
        recordD = (GfxTlVertex *)(((GfxRenderObject *)lineRect->batchNode)->vertexCursor + 0x60);
        x1Shifted = lineRect->endX * g_FieldDisplayScaleFactor + 1;
        recordD->x = x1Shifted;
        endYSave = lineRect->endY * g_FieldDisplayScaleFactor;
        recordD->y = endYSave;
        recordD->zBits = FLOAT_0_01_BITS;
        recordD->color = 0x80404040;
        g_FieldLineBatchPendingFlag = 1;
    }
}

/*
 * Pushes the rotation matrix, installs the supplied matrix and the field geometry offsets, then
 * projects both endpoints of each enabled line record, scales them to display coordinates into
 * g_FieldLineDrawBuffer and fills the batch node from them. Restores the rotation matrix and the
 * ordinary geometry offsets afterward.
 */
// FUNCTION: FF7 0x00644cec
void __cdecl Field_DrawScriptLines(void *matrix)
{
    struct {
        int flags;
        short x, y;
        int unknown_projection_tail;
        int index;
        int depth;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    PushRotMatrix();
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    SetGeomOffset(g_FieldDisplayOffsetX, g_FieldDisplayHeight);
    for (frame.index = 0; frame.index < 64; frame.index++) {
        if (g_FieldLineDrawScratch[frame.index].active == 1) {
            RotTransPers((SVECTOR *)&g_FieldLineDrawScratch[frame.index].upperX,
                         &frame.x, &frame.depth, &frame.flags);
            g_FieldLineDrawBuffer.startX = frame.x * g_FieldDisplayScaleFactor + (g_FieldDisplayCenterX + g_FieldPaletteTargetOrgX * g_FieldDisplayScaleFactor) - g_FieldDisplayOffsetX;
            g_FieldLineDrawBuffer.startY = frame.y * g_FieldDisplayScaleFactor + (g_FieldDisplayCenterY + g_FieldPaletteTargetOrgY * g_FieldDisplayScaleFactor) - g_FieldDisplayHeight;
            RotTransPers((SVECTOR *)&g_FieldLineDrawScratch[frame.index].lowerX,
                         &frame.x, &frame.depth, &frame.flags);
            g_FieldLineDrawBuffer.endX = frame.x * g_FieldDisplayScaleFactor + (g_FieldDisplayCenterX + g_FieldPaletteTargetOrgX * g_FieldDisplayScaleFactor) - g_FieldDisplayOffsetX;
            g_FieldLineDrawBuffer.endY = frame.y * g_FieldDisplayScaleFactor + (g_FieldDisplayCenterY + g_FieldPaletteTargetOrgY * g_FieldDisplayScaleFactor) - g_FieldDisplayHeight;
            g_FieldLineBatchPendingFlag = 1;
            Field_FillLineBatchNodeVertices(&g_FieldLineDrawBuffer);
        }
    }
    PopRotMatrix();
    SetGeomOffset(g_FieldDisplayCenterX, g_FieldDisplayCenterY);
}

/*
 * If the batch is pending, emits it through Gfx_EmitPrimitiveBatchList with the current engine
 * context. Does nothing otherwise.
 */
// FUNCTION: FF7 0x00644e90
void __cdecl Field_EmitLineDrawBatch(void)
{
    if (g_FieldLineBatchPendingFlag != 0) {
        Gfx_EmitPrimitiveBatchList(g_FieldLineDrawBuffer.batchNode, GetEngineContext());
    }
}

/*
 * If the batch is pending (Field_FillLineBatchNodeVertices raises the flag when it adds a line),
 * clears the flag and releases the batch through InvalidateBatchGeneration and
 * Gfx_EmptyRenderObjectBatch.
 */
// FUNCTION: FF7 0x00644eb2
void __cdecl Field_ReleaseLineDrawBatch(void)
{
    if (g_FieldLineBatchPendingFlag != 0) {
        g_FieldLineBatchPendingFlag = 0;
        InvalidateBatchGeneration(g_FieldLineDrawBuffer.batchNode);
        Gfx_EmptyRenderObjectBatch((int)g_FieldLineDrawBuffer.batchNode);
    }
}

/*
 * Raises or lowers g_FieldLineDrawIntensity by one, saturating at 255 or zero, as bit 0x80 of
 * gSavemap.fieldLineDrawFlags is set or clear. For each of the 64 records whose step state has run
 * out: records below intensity / 4 are enabled with a seven-step cycle and positioned at a random
 * offset (from g_FieldRandomTable) around the player entity, the others get a disabled one-step
 * cycle. Both endpoint Z values derive from the base Z plus the low three bits of the step state
 * times 128, 256 apart; every record's step state then counts down.
 */
// FUNCTION: FF7 0x00644ee7
void __cdecl Field_StepLineDrawRecords(void)
{
    int i;

    if ((gSavemap.fieldLineDrawFlags & 0x80) == 0) {
        if (*(unsigned char *)&g_FieldLineDrawIntensity != 0) {
            g_FieldLineDrawIntensity--;
        }
    } else if (*(unsigned char *)&g_FieldLineDrawIntensity != 0xFF) {
        g_FieldLineDrawIntensity++;
    }
    for (i = 0; i < 64; i++) {
        if (g_FieldLineDrawScratch[i].stepState == 0) {
            if (i < *(unsigned char *)&g_FieldLineDrawIntensity >> 2) {
                g_FieldLineDrawScratch[i].active = 1;
                (g_FieldLineDrawScratch[i].randomIndex)++;
                g_FieldLineDrawScratch[i].stepState = 7;
                g_FieldLineDrawScratch[i].lowerX = (g_FieldEntityState[g_FieldPlayerEntityIndex].posX >> 12) + (short)(g_FieldRandomTable[*(unsigned char *)&g_FieldLineDrawScratch[i].randomIndex] * 0xC) + -1536;
                g_FieldLineDrawScratch[i].lowerY = (g_FieldEntityState[g_FieldPlayerEntityIndex].posY >> 12) + (short)(g_FieldRandomTable[(g_FieldLineDrawScratch[i].randomIndex * 3 & 0xFF)] * 12) + -1536;
                g_FieldLineDrawScratch[i].upperX = g_FieldLineDrawScratch[i].lowerX;
                g_FieldLineDrawScratch[i].upperY = g_FieldLineDrawScratch[i].lowerY;
                g_FieldLineDrawScratch[i].baseZ = (g_FieldEntityState[g_FieldPlayerEntityIndex].posZ >> 12) - 0x300;
            } else {
                g_FieldLineDrawScratch[i].stepState = 1;
                g_FieldLineDrawScratch[i].active = 0;
            }
        }
        g_FieldLineDrawScratch[i].lowerZ = g_FieldLineDrawScratch[i].baseZ + ((g_FieldLineDrawScratch[i].stepState & 7) << 7);
        g_FieldLineDrawScratch[i].upperZ = g_FieldLineDrawScratch[i].baseZ + ((g_FieldLineDrawScratch[i].stepState & 7) << 7) + 256;
        (g_FieldLineDrawScratch[i].stepState)--;
    }
}
} /* extern "C" */

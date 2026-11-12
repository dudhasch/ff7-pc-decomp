// src/lib/src/graphics/graphics_group_vertex_draw_spec.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Applies draw specifications to polygon groups and polygon sets: UV scrolling and
//   restoration, vertex color set, scale, save and restore, and specular modes. It also holds an
//   empty placeholder routine.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"

#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
extern "C" {

/*
 * Does nothing.
 */
// FUNCTION: FF7 0x0068cf70
void __cdecl Gfx_UnusedNoOp(void)
{
}

/*
 * Initializes and zeroes a draw-spec record.
 */
// FUNCTION: FF7 0x0068cf75
void __cdecl Gfx_InitDrawSpec(int kind, GfxDrawSpec *drawSpec)
{
    GfxDrawSpec *cursor;

    cursor = drawSpec;
    cursor->kindWord = 0;
    cursor->modeWord = 0;
    drawSpec->kind = kind;
    drawSpec->textureState = 0;
    cursor = (GfxDrawSpec *)((int)drawSpec + 0x34);
    cursor->kindWord = 0;
    cursor->modeWord = 0;
    cursor->packedColor = 0;
    cursor->callback = 0;
}

/*
 * Restores group vertex UV coordinates from base coordinates.
 */
// FUNCTION: FF7 0x0068cfd8
void __cdecl Gfx_RestoreGroupUvFromBase(int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxGroupUvBase *source;
    GfxPolygonGroupVMirror *group;
    int i;
    GfxGroupUvVertex *vertex;

    ctx = GetEngineContext();
    group = (GfxPolygonGroupVMirror *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (group != 0) {
        if (group->source != 0) {
            source = (GfxGroupUvBase *)group->source;
            vertex = (GfxGroupUvVertex *)group->vertices;
            for (i = 0; i < group->vertexCount; i++) {
                vertex->u = source->baseU;
                vertex->v = source->baseV;
                vertex++;
                source++;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Scrolls and wraps group UV coordinates by float offsets.
 */
// FUNCTION: FF7 0x0068d07e
void __cdecl Gfx_ScrollGroupUvFromBaseWrapped(float scrollU, float scrollV, int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxScrollUvBase *source;
    GfxPolygonGroupVMirror *group;
    int i;
    GfxScrollGroup *vertex;

    ctx = GetEngineContext();
    group = (GfxPolygonGroupVMirror *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (group != 0) {
        if (group->source != 0) {
            source = (GfxScrollUvBase *)group->source;
            vertex = (GfxScrollGroup *)group->vertices;
            for (i = 0; i < group->vertexCount; i++) {
                vertex->scrollU = source->baseU + scrollU;
                if (vertex->scrollU >= 1.0f) {
                    vertex->scrollU -= 1.0f;
                }
                vertex->scrollV = source->baseV + scrollV;
                if (vertex->scrollV >= 1.0f) {
                    vertex->scrollV -= 1.0f;
                }
                vertex++;
                source++;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Restores group V coordinates from base vertex values.
 */
// FUNCTION: FF7 0x0068d17f
void __cdecl Gfx_RestoreGroupVFromBase(int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxGroupUvBase *source;
    GfxPolygonGroupVMirror *group;
    int i;
    GfxGroupUvVertex *vertex;

    ctx = GetEngineContext();
    group = (GfxPolygonGroupVMirror *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (group != 0) {
        if (group->source != 0) {
            source = (GfxGroupUvBase *)group->source;
            vertex = (GfxGroupUvVertex *)group->vertices;
            for (i = 0; i < group->vertexCount; i++) {
                vertex->v = source->baseV;
                vertex++;
                source++;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Mirrors group V texture coordinates about an axis.
 */
// FUNCTION: FF7 0x0068d21a
void __cdecl Gfx_MirrorGroupVFromBase(float axis, int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxScrollUvBase *source;
    GfxPolygonGroupVMirror *group;
    int i;
    GfxScrollGroup *vertex;

    ctx = GetEngineContext();
    group = (GfxPolygonGroupVMirror *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (group != 0) {
        if (group->source != 0) {
            source = (GfxScrollUvBase *)group->source;
            vertex = (GfxScrollGroup *)group->vertices;
            for (i = 0; i < group->vertexCount; i++) {
                vertex->scrollV = axis - source->baseV;
                vertex++;
                source++;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Applies lighting, UV modifiers, and material spec to a polygon group.
 */
// FUNCTION: FF7 0x0068d2b8
void __cdecl Gfx_ApplyDrawSpecToGroup(int groupIndex, GfxPolygonSet *polygonSet, GfxDrawSpec *drawSpec)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        struct {
            unsigned char pad[16];
            float value[4];
        } colors;
        float floatColor[4];
        struct { unsigned char pad_to_10[0x10]; float drawColorA; };
        struct { unsigned char pad_to_14[0x14]; float drawColorB; };
        struct { unsigned char pad_to_18[0x18]; float drawColorC; };
        struct { unsigned char pad_to_1C[0x1C]; int byteScale; };
        struct { unsigned char pad_to_20[0x20]; float alphaScale; };
    } frame;
#pragma pack(pop)

    switch (drawSpec->kind) {
    case GFX_DRAW_SPEC_KIND_NONE:
        break;
    case GFX_DRAW_SPEC_KIND_VERTEX_ALPHA:
        Gfx_SetGroupVertexAlpha((char)drawSpec->alpha, groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_KIND_VERTEX_COLOR:
        Gfx_SetGroupVertexColor(drawSpec->packedColor, groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_KIND_SCALE_SAVED_INT:
        Gfx_ScaleGroupVertexColorsFromSaved(drawSpec->alpha, groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_KIND_BLEND_SAVED:
        frame.alphaScale = (float)drawSpec->alpha * 0.003921569f;
        frame.drawColorA = (float)drawSpec->colorA;
        frame.drawColorB = (float)drawSpec->colorB;
        frame.drawColorC = (float)drawSpec->colorC;
        frame.byteScale = FLOAT_255_BITS;
        Gfx_BlendPolygonSetGroupVertexColorsFromSaved(frame.alphaScale, 1.0f - frame.alphaScale, (GfxFloatColor *)frame.colors.value, groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_KIND_SCALE_SAVED:
        frame.alphaScale = (float)drawSpec->alpha * 0.003921569f;
        Gfx_ScalePolygonSetGroupVertexColorsFromSaved(frame.alphaScale, groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_KIND_BLEND_SAVED_OR_PALETTE:
        frame.alphaScale = (float)drawSpec->alpha * 0.003921569f;
        frame.drawColorA = (float)drawSpec->colorA;
        frame.drawColorB = (float)drawSpec->colorB;
        frame.drawColorC = (float)drawSpec->colorC;
        frame.byteScale = FLOAT_255_BITS;
        if (groupIndex < 0x10) {
            Gfx_BlendPolygonSetGroupColorsOrPalette(frame.alphaScale, 1.0f - frame.alphaScale, (GfxFloatColor *)frame.colors.value, *(unsigned char *)((char *)((int)drawSpec + groupIndex) + 0x34), groupIndex, polygonSet);
        }
        break;
    case GFX_DRAW_SPEC_KIND_SCALE_SAVED_OR_PALETTE:
        frame.alphaScale = (float)drawSpec->alpha * 0.003921569f;
        if (groupIndex < 0x10) {
            Gfx_ScalePolygonSetGroupColorsOrPalette(frame.alphaScale, *(unsigned char *)((char *)((int)drawSpec + groupIndex) + 0x34), groupIndex, polygonSet);
        }
        break;
    case GFX_DRAW_SPEC_KIND_MODULATE_SAVED_OR_PALETTE:
        if (groupIndex < 0x10) {
            Gfx_UnpackBgraToFloat4(drawSpec->packedColor, (float *)frame.floatColor);
            Gfx_ModulateGroupVertexColorsFromSavedOrPalette((GfxFloatColor *)frame.floatColor, *(unsigned char *)((char *)((int)drawSpec + groupIndex) + 0x34), groupIndex, polygonSet);
        }
        break;
    case GFX_DRAW_SPEC_KIND_RESTORE_COLORS:
        Gfx_RestoreGroupVertexColors(groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_KIND_VERTEX_CALLBACK_WITH_SPEC:
        Gfx_ForEachGroupVertexColorWithContext(drawSpec->vertexAlphaCallback, drawSpec, groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_KIND_VERTEX_COLOR_CALLBACK:
        Gfx_ForEachGroupVertexAndColor(drawSpec->vertexAlphaCallback, groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_KIND_SAVED_COLOR_CALLBACK:
        Gfx_ForEachGroupSavedColorVertexAndColor(drawSpec->vertexAlphaCallback, groupIndex, polygonSet);
    }
    switch ((char)drawSpec->textureStateMode) {
    case GFX_DRAW_SPEC_TEX_STATE_KEEP:
        break;
    case GFX_DRAW_SPEC_TEX_STATE_APPLY:
        Gfx_ApplyStateTexturedCall((void *)drawSpec->textureState, (void *)GetEngineContext());
    }
    switch ((char)drawSpec->uvMode) {
    case GFX_DRAW_SPEC_UV_KEEP:
        break;
    case GFX_DRAW_SPEC_UV_RESTORE:
        Gfx_RestoreGroupUvFromBase(groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_UV_SCROLL:
        Gfx_ScrollGroupUvFromBaseWrapped(drawSpec->scrollU, drawSpec->scrollV, groupIndex, polygonSet);
    }
    switch ((char)drawSpec->vMode) {
    case GFX_DRAW_SPEC_V_KEEP:
        break;
    case GFX_DRAW_SPEC_V_RESTORE:
        Gfx_RestoreGroupVFromBase(groupIndex, polygonSet);
        break;
    case GFX_DRAW_SPEC_V_MIRROR:
        Gfx_MirrorGroupVFromBase(drawSpec->scrollV, groupIndex, polygonSet);
    }
    switch ((char)drawSpec->texturePageMode) {
    case GFX_DRAW_SPEC_TEX_PAGE_KEEP:
        break;
    case GFX_DRAW_SPEC_TEX_PAGE_SELECT:
        Gfx_SelectTexturePage(drawSpec->texturePage, (void *)drawSpec->textureState);
    }
}

/*
 * Applies draw specification across all groups of a polygon set.
 */
// FUNCTION: FF7 0x0068d68f
void __cdecl Gfx_ApplyDrawSpecToPolygonSet(GfxDrawSpec *drawSpec, GfxPolygonSet *polygonSet)
{
    int groupIndex;

    if (polygonSet != 0) {
        polygonSet->drawSpecApplied = 1;
        polygonSet->drawSpec = (int)drawSpec;
        for (groupIndex = 0; groupIndex < polygonSet->groupCount; groupIndex++) {
            Gfx_ApplyDrawSpecToGroup(groupIndex, polygonSet, drawSpec);
        }
    }
}
} /* extern "C" */

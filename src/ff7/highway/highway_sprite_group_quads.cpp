// src/ff7/highway/highway_sprite_group_quads.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sprite group for the highway minigame: it builds render objects and scales from
//   null-terminated source strings and releases them, and draws quads at the group's offset, either
//   from the origin or centred on it.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/api/snobo.h"
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/types/graphics.h"
#include "ff7/api/highway.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"
#include "lib/enums/graphics.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
extern "C" {
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)

// clang-format on

#pragma intrinsic(abs)
} // extern "C"

/*
 * Counts null-terminated source strings, allocates object pointers and paired scales, loads each
 * render object with blend/options and records reciprocal dimensions only on success. Captures
 * offsets and global scale divided by 320. Failed object leaves paired scales uninitialized.
 */
// FUNCTION: FF7 0x0065e560
HighwayScaledSpriteGroup::HighwayScaledSpriteGroup(int *sources)
{
    GfxEngineContext *ctx;
    int slotIndex;
    PfileLoadOptions loadOptions;

    ctx = GetEngineContext();
    slotIndex = 0;
    for (; sources[slotIndex] != 0; slotIndex++) {
    }
    count = slotIndex;
    objects = (GfxRenderObject **)operator new(count * 4);
    scales = (HighwaySpriteScale *)operator new(count * 8);
    for (slotIndex = 0; slotIndex < count; slotIndex++) {
        memset(&loadOptions, 0, sizeof(loadOptions));
        Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
        loadOptions.sourceMode = g_HighwayAssetSourceMode;
        loadOptions.directory = g_HighwayDataDirectory;
        if (g_HighwayUseLgpArchive) {
            loadOptions.file.fromArchive = 1;
            loadOptions.file.archiveId = 8;
            loadOptions.file.callback = 0;
        }
        loadOptions.renderFlags |= 0x80;
        loadOptions.renderFlags |= 0x800;
        objects[slotIndex] = (GfxRenderObject *)Gfx_CreateRenderObject(
            1, GFX_DRAW_TEXTURED_QUAD_2D, &loadOptions, (char *)sources[slotIndex],
            (void *)ctx->renderObjectBank);
        if (objects[slotIndex] != 0) {
            scales[slotIndex].scaleX = 1.0f / objects[slotIndex]->invWidth;
            scales[slotIndex].scaleY = 1.0f / objects[slotIndex]->invHeight;
        }
    }
    /* The scene block stores the offsets as float bits in int members; copy them bitwise. */
    *(int *)&xOffset = g_pHighwaySceneBlock->spriteOffsetX;
    *(int *)&yOffset = g_pHighwaySceneBlock->spriteOffsetY;
    globalScale = g_pHighwaySceneBlock->viewWidth / 320.0f;
}

/*
 * Destroys each nonzero render object, then releases the scales buffer and the objects buffer
 * in that order. Does not clear the members or the count.
 */
// FUNCTION: FF7 0x0065e715
HighwayScaledSpriteGroup::~HighwayScaledSpriteGroup()
{
    int p;
    int first;
    int second;

    for (p = 0; p < count; p++) {
        if (objects[p] != 0) {
            DestroyRenderObject(objects[p]);
        }
    }
    first = (int)scales;
    operator delete((void *)first);
    second = (int)objects;
    operator delete((void *)second);
}

/*
 * Offsets the rectangle origin by the sprite-group offsets and scales all four texture parameters
 * by the selected object's inverse texture size. When a batch node is acquired, writes four
 * vertices ordered (x, y), (x, y+height), (x+width, y), (x+width, y+height) with matching texture
 * corners, the supplied depth and color, w of one and an opaque-black specular. Emits the
 * primitive batch and invalidates its generation. Nothing is written if no node is acquired.
 */
// FUNCTION: FF7 0x0065e78e
void HighwayScaledSpriteGroup::drawQuad(int index, float x, float y, float width, float height, float u,
                              float v, float du, float dv, int color, float depth)
{
    GfxRenderObject *item;
    GfxTlVertex *vertex0;
    GfxTlVertex *vertex1;
    GfxTlVertex *vertex2;
    GfxTlVertex *vertex3;

    item = objects[index];
    x += xOffset;
    y += yOffset;
    u *= item->invWidth;
    v *= item->invHeight;
    du *= item->invWidth;
    dv *= item->invHeight;
    if (AcquireBatchNode(1, item)) {
        vertex0 = (GfxTlVertex *)(item->vertexCursor);
        vertex0->x = x;;
        vertex0->y = y;;
        vertex0->z = depth;;
        vertex0->rhw = 1.0f;;
        vertex0->color = color;;
        vertex0->specular = 0xFF000000;;
        vertex0->u = u;;
        vertex0->v = v;;

        vertex1 = (GfxTlVertex *)(item->vertexCursor + 0x20);
        vertex1->x = x;;
        vertex1->y = y + height;;
        vertex1->z = depth;;
        vertex1->rhw = 1.0f;;
        vertex1->color = color;;
        vertex1->specular = 0xFF000000;;
        vertex1->u = u;;
        vertex1->v = v + dv;;

        vertex2 = (GfxTlVertex *)(item->vertexCursor + 0x40);
        vertex2->x = x + width;;
        vertex2->y = y;;
        vertex2->z = depth;;
        vertex2->rhw = 1.0f;;
        vertex2->color = color;;
        vertex2->specular = 0xFF000000;;
        vertex2->u = u + du;;
        vertex2->v = v;;

        vertex3 = (GfxTlVertex *)(item->vertexCursor + 0x60);
        vertex3->x = x + width;;
        vertex3->y = y + height;;
        vertex3->z = depth;;
        vertex3->rhw = 1.0f;;
        vertex3->color = color;;
        vertex3->specular = 0xFF000000;;
        vertex3->u = u + du;;
        vertex3->v = v + dv;;

        Gfx_EmitPrimitiveBatchList((void *)item, GetEngineContext());
        InvalidateBatchGeneration(item);
    }
}

/*
 * Derives quad dimensions from per-item scales times half the global scale and centers its origin
 * within half the configured screen dimensions plus group offsets and scaled input displacement.
 * When a batch node is acquired, writes four vertices in rectangle
 * order with the supplied depth, w of one, white color, opaque-black specular and texture
 * corners spanning the object's inverse texture size to one minus it. Emits the batch and
 * invalidates its generation.
 */
// FUNCTION: FF7 0x0065e991
void HighwayScaledSpriteGroup::drawCentered(int index, float depth, float x, float y)
{
    // VC5 hashes local names into slots; these spellings preserve the retail layout.
    float sc5;
    int col8;
    float *sz16;
    float wd3;
    float ht11;
    float lx19;
    float ty1;
    float cx28;
    float v17;
    float u19;
    float texV;
    float u010;
    float cy0;
    GfxRenderObject *obj12;
    GfxTlVertex *p03;
    GfxTlVertex *p38;
    GfxTlVertex *p228;
    GfxTlVertex *p10;

    obj12 = objects[index];
    sz16 = &scales[index].scaleX;
    sc5 = globalScale / 2.0f;
    cx28 = g_pHighwaySceneBlock->viewWidth / 2.0f;
    cy0 = g_pHighwaySceneBlock->viewHeight / 2.0f;
    wd3 = sz16[0] * sc5;
    ht11 = sz16[1] * sc5;
    lx19 = xOffset + cx28 - wd3 / 2.0f + x * sc5;
    ty1 = yOffset + cy0 - ht11 / 2.0f + y * sc5;
    u010 = obj12->invWidth;
    texV = obj12->invHeight;
    u19 = 1.0f - obj12->invWidth;
    v17 = 1.0f - obj12->invHeight;
    col8 = -1;
    if (AcquireBatchNode(1, obj12)) {
        p03 = (GfxTlVertex *)obj12->vertexCursor;
        p03->x = lx19;
        p03->y = ty1;
        p03->z = depth;
        p03->rhw = 1.0f;
        p03->color = col8;
        p03->specular = 0xFF000000;
        p03->u = u010;
        p03->v = texV;

        p10 = (GfxTlVertex *)(obj12->vertexCursor + 0x20);
        p10->x = lx19;
        p10->y = ty1 + ht11;
        p10->z = depth;
        p10->rhw = 1.0f;
        p10->color = col8;
        p10->specular = 0xFF000000;
        p10->u = u010;
        p10->v = v17;

        p228 = (GfxTlVertex *)(obj12->vertexCursor + 0x40);
        p228->x = lx19 + wd3;
        p228->y = ty1;
        p228->z = depth;
        p228->rhw = 1.0f;
        p228->color = col8;
        p228->specular = 0xFF000000;
        p228->u = u19;
        p228->v = texV;

        p38 = (GfxTlVertex *)(obj12->vertexCursor + 0x60);
        p38->x = lx19 + wd3;
        p38->y = ty1 + ht11;
        p38->z = depth;
        p38->rhw = 1.0f;
        p38->color = col8;
        p38->specular = 0xFF000000;
        p38->u = u19;
        p38->v = v17;

        Gfx_EmitPrimitiveBatchList((void *)obj12, GetEngineContext());
        InvalidateBatchGeneration(obj12);
    }
}

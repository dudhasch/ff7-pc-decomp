// src/ff7/chocobo/chocobo_course_render_objects.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Submits the chocobo race course: walks the track's segment records from the camera's
//   segment to its target, projects and submits their triangles, quads and sprites, and picks the
//   flipped UVs where a sprite needs them. Also creates, destroys and flushes the five fixed
//   render objects the course draws through.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/chocobo.h"
#include "ff7/api/chocobo.h"
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "ff7/gte.h"
#include "ff7/enums/chocobo.h"
#include "lib/enums/graphics.h"
extern "C" {

// clang-format off
/* 18 ints no function reads. */
unsigned char g_rawData_00971cc0[72] = {
    0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x79, 0x00, 0x00, 0x00, 0x7a, 0x00, 0x00, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
    0x7d, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0xa2, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* The sprites (numbered from 1) of course variant 0 that take the vertically flipped UVs. */
// GLOBAL: FF7 0x00971d08
int g_ChocoboAltUvSelectorCodes[53] = {102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207};
// GLOBAL: FF7 0x00e719e8
ChocoboRacerEntry *g_pChocoboRacerTable = 0;
// GLOBAL: FF7 0x00e719ec
unsigned short g_ChocoboRacerCounts[2] = {0};
unsigned char g_bssPad_00e719f0[480] = {0};
unsigned char g_bssPad_00e71bd0[4] = {0};
/* Five fixed render objects of 0x1C bytes each: the object, unused padding bytes, then active, dirty (it
 * holds batches to flush) and the blend group it was created in (0: blend 4, 1: blend 1). */
// GLOBAL: FF7 0x00e71bd4
GfxRenderObject *g_ChocoboSubmitTriangleRenderObj = 0;
unsigned char g_bssPad_00e71bd8[12] = {0};
// GLOBAL: FF7 0x00e71be4
int g_ChocoboSubmitTrianglesActive = 0;
// GLOBAL: FF7 0x00e71be8
int g_ChocoboSubmitTrianglesDirty = 0;
// GLOBAL: FF7 0x00e71bec
int g_ChocoboSubmitTrianglesGroup = 0;
// GLOBAL: FF7 0x00e71bf0
GfxRenderObject *g_ChocoboEmitQuadRenderObj1 = 0;
unsigned char g_bssPad_00e71bf4[4] = {0};
unsigned char g_bssPad_00e71bf8[8] = {0};
// GLOBAL: FF7 0x00e71c00
int g_ChocoboEmitQuadActive1 = 0;
// GLOBAL: FF7 0x00e71c04
int g_ChocoboEmitQuadDirty1 = 0;
// GLOBAL: FF7 0x00e71c08
int g_ChocoboEmitQuadGroup1 = 0;
// GLOBAL: FF7 0x00e71c0c
GfxRenderObject *g_ChocoboEmitQuadRenderObj2 = 0;
unsigned char g_bssPad_00e71c10[12] = {0};
// GLOBAL: FF7 0x00e71c1c
int g_ChocoboEmitQuadActive2 = 0;
// GLOBAL: FF7 0x00e71c20
int g_ChocoboEmitQuadDirty2 = 0;
// GLOBAL: FF7 0x00e71c24
int g_ChocoboEmitQuadGroup2 = 0;
// GLOBAL: FF7 0x00e71c28
GfxRenderObject *g_ChocoboEmitTriangleRenderObj1 = 0;
unsigned char g_bssPad_00e71c2c[4] = {0};
unsigned char g_bssPad_00e71c30[8] = {0};
// GLOBAL: FF7 0x00e71c38
int g_ChocoboEmitTriangleActive1 = 0;
// GLOBAL: FF7 0x00e71c3c
int g_ChocoboEmitTriangleDirty1 = 0;
// GLOBAL: FF7 0x00e71c40
int g_ChocoboEmitTriangleGroup1 = 0;
// GLOBAL: FF7 0x00e71c44
GfxRenderObject *g_ChocoboEmitTriangleRenderObj2 = 0;
unsigned char g_bssPad_00e71c48[12] = {0};
// GLOBAL: FF7 0x00e71c54
int g_ChocoboEmitTriangleActive2 = 0;
// GLOBAL: FF7 0x00e71c58
int g_ChocoboEmitTriangleDirty2 = 0;
// GLOBAL: FF7 0x00e71c5c
int g_ChocoboEmitTriangleGroup2 = 0;
// GLOBAL: FF7 0x00e71c60
struct ChocoboSpriteGroup g_ChocoboSpriteRecords[36] = {0};
unsigned char g_bssPad_00e72830[336] = {0};
/* A table of ints: the g_ChocoboSpriteRecords index of each course sprite. */
// GLOBAL: FF7 0x00e72980
int g_ChocoboSpriteRecordMap = 0;
// GLOBAL: FF7 0x00e72984
int g_ChocoboSpriteRecordCount = 0;
// GLOBAL: FF7 0x00e72988
int g_ChocoboCourseSubmitGuard = 0;
unsigned char g_bssPad_00e7298c[4] = {0};
// clang-format on

/* The two texture coordinates of a course sprite, 0.03f and 0.94f, stored as float bits. */
#define CHOCOBO_UV_LOW 0x3CF5C28F
#define CHOCOBO_UV_HIGH 0x3F70A3D7

/*
 * Creates three render objects with blend four and two with blend one using the supplied option
 * buffer and engine resource, setting option bit two after each blend initialization.
 */
// FUNCTION: FF7 0x0076ff60
void __cdecl Mini_ChocoboCreateFixedRenderObjects(GfxEngineContext *engineResource, void *optionBuffer)
{
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, (struct PfileLoadOptions *)optionBuffer);
    *(int *)((char *)optionBuffer + 0x2C) |= 2;
    g_ChocoboSubmitTriangleRenderObj = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, (struct PfileLoadOptions *)optionBuffer, 0, (void *)engineResource->renderObjectBank);
    g_ChocoboSubmitTrianglesActive = 1;
    g_ChocoboSubmitTrianglesDirty = 0;
    g_ChocoboSubmitTrianglesGroup = 0;
    g_ChocoboEmitTriangleRenderObj1 = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, (struct PfileLoadOptions *)optionBuffer, 0, (void *)engineResource->renderObjectBank);
    g_ChocoboEmitTriangleActive1 = 1;
    g_ChocoboEmitTriangleDirty1 = 0;
    g_ChocoboEmitTriangleGroup1 = 0;
    g_ChocoboEmitQuadRenderObj1 = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, (struct PfileLoadOptions *)optionBuffer, 0, (void *)engineResource->renderObjectBank);
    g_ChocoboEmitQuadActive1 = 1;
    g_ChocoboEmitQuadDirty1 = 0;
    g_ChocoboEmitQuadGroup1 = 0;
    Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, (struct PfileLoadOptions *)optionBuffer);
    *(int *)((char *)optionBuffer + 0x2C) |= 2;
    g_ChocoboEmitTriangleRenderObj2 = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, (struct PfileLoadOptions *)optionBuffer, 0, (void *)engineResource->renderObjectBank);
    g_ChocoboEmitTriangleActive2 = 1;
    g_ChocoboEmitTriangleDirty2 = 0;
    g_ChocoboEmitTriangleGroup2 = 1;
    g_ChocoboEmitQuadRenderObj2 = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, (struct PfileLoadOptions *)optionBuffer, 0, (void *)engineResource->renderObjectBank);
    g_ChocoboEmitQuadActive2 = 1;
    g_ChocoboEmitQuadDirty2 = 0;
    g_ChocoboEmitQuadGroup2 = 1;
}
/*
 * Destroys the five render objects created by Mini_ChocoboCreateFixedRenderObjects.
 */
// FUNCTION: FF7 0x007700d9
void __cdecl Mini_ChocoboDestroyFixedRenderObjects(void)
{
    DestroyRenderObject(g_ChocoboSubmitTriangleRenderObj);
    g_ChocoboSubmitTriangleRenderObj = 0;
    DestroyRenderObject(g_ChocoboEmitTriangleRenderObj1);
    g_ChocoboEmitTriangleRenderObj1 = 0;
    DestroyRenderObject(g_ChocoboEmitQuadRenderObj1);
    g_ChocoboEmitQuadRenderObj1 = 0;
    DestroyRenderObject(g_ChocoboEmitTriangleRenderObj2);
    g_ChocoboEmitTriangleRenderObj2 = 0;
    DestroyRenderObject(g_ChocoboEmitQuadRenderObj2);
    g_ChocoboEmitQuadRenderObj2 = 0;
}
/*
 * Flushes dirty batches on the five fixed render objects and clears their dirty flags.
 */
// FUNCTION: FF7 0x00770159
void __cdecl Mini_ChocoboResetDirtyFixedRenderObjects(void)
{
    if (g_ChocoboSubmitTrianglesDirty != 0) {
        Gfx_EmptyRenderObjectBatch((int)g_ChocoboSubmitTriangleRenderObj);
        g_ChocoboSubmitTrianglesDirty = 0;
    }
    if (g_ChocoboEmitTriangleDirty1 != 0) {
        Gfx_EmptyRenderObjectBatch((int)g_ChocoboEmitTriangleRenderObj1);
        g_ChocoboEmitTriangleDirty1 = 0;
    }
    if (g_ChocoboEmitQuadDirty1 != 0) {
        Gfx_EmptyRenderObjectBatch((int)g_ChocoboEmitQuadRenderObj1);
        g_ChocoboEmitQuadDirty1 = 0;
    }
    if (g_ChocoboEmitTriangleDirty2 != 0) {
        Gfx_EmptyRenderObjectBatch((int)g_ChocoboEmitTriangleRenderObj2);
        g_ChocoboEmitTriangleDirty2 = 0;
    }
    if (g_ChocoboEmitQuadDirty2 != 0) {
        Gfx_EmptyRenderObjectBatch((int)g_ChocoboEmitQuadRenderObj2);
        g_ChocoboEmitQuadDirty2 = 0;
    }
}
/*
 * Rebuilds the geometry of the course triangle render object from the track data table.
 */
// FUNCTION: FF7 0x00770206
void __cdecl Mini_ChocoboRaceSubmitCourseTriangles(void)
{
    int vertIdx;
    int sourceAddress; /* stored, not read */
    ChocoboTrackEntry *entry;

    g_ChocoboCourseSubmitGuard = 1;
    for (vertIdx = 0; vertIdx < g_ChocoboTrackData->floatVertexCount; vertIdx++) {
        sourceAddress = g_ChocoboTrackData->trackVertexTable + vertIdx * 0x24;
        entry = (ChocoboTrackEntry *)(g_ChocoboTrackData->builtFloatVertices + vertIdx * 0x30);
        Mini_ChocoboEmitTriangleRecord((int)&g_ChocoboViewScreenMatrix, g_ChocoboSubmitTriangleRenderObj, (int)entry);
    }
    g_ChocoboSubmitTrianglesDirty = 1;
    g_ChocoboCourseSubmitGuard = 0;
}
/*
 * Clears draw counter and submits course segment records across the specified ring span.
 */
// FUNCTION: FF7 0x00770291
void __cdecl Mini_ChocoboRaceSubmitCourseRingSpan(void)
{
    int segmentCount;
    int endSegment;
    int startSegment;

    g_ChocoboSubmittedRecordCount = 0;
    segmentCount = g_ChocoboTrackData->spriteTable;
    startSegment = (g_ChocoboCameraBaseSegment + segmentCount) % segmentCount;
    endSegment = (g_ChocoboCameraTargetSegment + segmentCount) % segmentCount;
    if (startSegment > endSegment) {
        Mini_ChocoboRaceSubmitCourseSegmentRecords(0, endSegment);
        Mini_ChocoboRaceSubmitCourseSegmentRecords(startSegment, segmentCount);
    } else {
        Mini_ChocoboRaceSubmitCourseSegmentRecords(startSegment, endSegment);
    }
}
/*
 * Walks the course records from segment startIndex for endIndex - startIndex segments and submits
 * each one. A record word holds its kind in byte 2 (enum ChocoboCourseRecordKind) and its index in
 * the low 16 bits; a negative word opens the next segment, whose waypoint flags (& 0x80) go to the
 * emitters, and -1 ends the list. flags is not set before the first negative word, and & 0x80
 * never makes it negative, so the emitters' `flags < 0` test never skips a record.
 */
// FUNCTION: FF7 0x00770305
void __cdecl Mini_ChocoboRaceSubmitCourseSegmentRecords(int startIndex, int endIndex)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        int words[7];
        struct {
            int record, count, index, cursor, remaining;
            short id, pad;
            int flags;
        } f;
    } frame;
    frame.f.index = startIndex;
    frame.f.remaining = endIndex - startIndex;
    frame.f.count = g_ChocoboSubmittedRecordCount;
    frame.f.id = *(short *)(g_ChocoboTrackData->triangleTable + startIndex * 2);
    frame.f.cursor = g_ChocoboTrackData->quadTable + frame.f.id * 4;
    for (;;) {
        frame.f.record = *(int *)frame.f.cursor;
        if (frame.f.record == CHOCOBO_COURSE_RECORD_END)
            break;
        if (frame.f.record < 0) {
            frame.f.remaining--;
            if (frame.f.remaining < 0)
                break;
            frame.f.index++;
            frame.f.flags = g_pChocoboCourseWaypoints[frame.f.index].flags & 0x80;
        }
        switch (((char *)&frame.f.record)[2]) {
        case CHOCOBO_COURSE_RECORD_TRIANGLE:
            frame.f.count += Mini_ChocoboRaceEmitCourseTriangle((int)&g_ChocoboViewScreenMatrix, (short)frame.f.record, frame.f.count, frame.f.flags);
            break;
        case CHOCOBO_COURSE_RECORD_QUAD:
            frame.f.count += Mini_ChocoboRaceEmitCourseQuad((int)&g_ChocoboViewScreenMatrix, (short)frame.f.record, frame.f.count, frame.f.flags);
            break;
        case CHOCOBO_COURSE_RECORD_SPRITE:
            frame.f.count += Mini_ChocoboRaceEmitCourseSpriteQuad(&g_ChocoboViewScreenMatrix, (short)frame.f.record, frame.f.count);
            break;
        }
        frame.f.cursor += 4;
    }
    g_ChocoboSubmittedRecordCount = frame.f.count;
}
/*
 * Projects course sprite position, computes quad coordinates and UV orientation, and submits sprite quad.
 */
// FUNCTION: FF7 0x00770434
int __cdecl Mini_ChocoboRaceEmitCourseSpriteQuad(void *cameraMatrix, short spriteIndex, int unusedCount)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        ChocoboCourseSprite22 sprite;
        struct { unsigned char pad_to_18[0x18]; struct { short x, y; } proj; };
        struct { unsigned char pad_to_48[0x48]; ChocoboSpriteQuad *quad; };
        struct { unsigned char pad_to_4C[0x4C]; int depth; };
        struct { unsigned char pad_to_50[0x50]; int projFlag; };
        struct { unsigned char pad_to_54[0x54]; int scaledWidth; };
        struct { unsigned char pad_to_5C[0x5C]; short rightX; };
        struct { unsigned char pad_to_60[0x60]; short leftX; };
        struct { unsigned char pad_to_62[0x62]; short topY; };
        struct { unsigned char pad_to_64[0x64]; int projP; };
    } frame;
#pragma pack(pop)

    frame.sprite = *(ChocoboCourseSprite22 *)(g_ChocoboTrackData->courseSpriteTable + spriteIndex * 0x16);
    frame.quad = (ChocoboSpriteQuad *)&g_ChocoboSpriteRecords[((int *)g_ChocoboSpriteRecordMap)[spriteIndex]];
    frame.depth = RotTransPers((SVECTOR *)&frame.sprite, &frame.proj.x, &frame.projP, &frame.projFlag);
    if (frame.depth > 0xFA0 || frame.depth < 0) {
        return 0;
    } else {
        frame.leftX = frame.proj.x;
        frame.topY = frame.proj.y;
        if (frame.leftX < -0x28 || frame.topY < -0x28) {
            return 0;
        } else if (frame.leftX > 0x15E || frame.topY > 0x10E) {
            return 0;
        } else {
            if (frame.depth != 0) {
                frame.scaledWidth = frame.sprite.width * 0x10E / frame.depth;
            }
            frame.leftX -= (short)(frame.scaledWidth / 2);
            frame.rightX = frame.leftX + frame.scaledWidth;
            frame.quad->corner0X = frame.leftX;
            frame.quad->corner0Y = frame.topY;
            frame.quad->corner1X = frame.rightX;
            frame.quad->corner1Y = frame.topY;
            frame.quad->corner2X = frame.leftX;
            frame.quad->corner3X = frame.rightX;
            frame.sprite.y -= (short)(frame.sprite.height << 2);
            RotTransPers((SVECTOR *)&frame.sprite, &frame.proj.x, &frame.projP, &frame.projFlag);
            frame.quad->corner3Y = frame.proj.y;
            frame.quad->corner2Y = frame.proj.y;
            *(float *)&frame.quad->depth = Mini_ChocoboShortPointProjectedDepth((struct ChocoboShortPointDepth *)&frame.sprite, (int)cameraMatrix);
            /* the selector codes number the sprites from 1 */
            if (Mini_ChocoboGetAltUvMode(spriteIndex + 1) != 0) {
                /* flipped vertically: the bottom corners take the top row of the texture */
                frame.quad->corner2U = CHOCOBO_UV_LOW;
                frame.quad->corner2V = CHOCOBO_UV_LOW;
                frame.quad->corner3U = CHOCOBO_UV_HIGH;
                frame.quad->corner3V = CHOCOBO_UV_LOW;
                frame.quad->corner0U = CHOCOBO_UV_LOW;
                frame.quad->corner0V = CHOCOBO_UV_HIGH;
                frame.quad->corner1U = CHOCOBO_UV_HIGH;
                frame.quad->corner1V = CHOCOBO_UV_HIGH;
            } else {
                frame.quad->corner0U = CHOCOBO_UV_LOW;
                frame.quad->corner0V = CHOCOBO_UV_LOW;
                frame.quad->corner1U = CHOCOBO_UV_HIGH;
                frame.quad->corner1V = CHOCOBO_UV_LOW;
                frame.quad->corner2U = CHOCOBO_UV_LOW;
                frame.quad->corner2V = CHOCOBO_UV_HIGH;
                frame.quad->corner3U = CHOCOBO_UV_HIGH;
                frame.quad->corner3V = CHOCOBO_UV_HIGH;
            }
            Mini_ChocoboEmitSpriteBatchUvQuad((struct ChocoboSpriteQuad *)frame.quad);
            return 1;
        }
    }
}
/*
 * Returns 2 (flipped UVs) or 0 for sprite spriteIndex (numbered from 1): on course variant 0 for
 * the 53 sprites of g_ChocoboAltUvSelectorCodes, on the other variant for every sprite but 0x49.
 */
// FUNCTION: FF7 0x00770685
int __cdecl Mini_ChocoboGetAltUvMode(int spriteIndex)
{
    int i;

    if (g_ChocoboCourseVariant == 0) {
        for (i = 0; i < 53; i++) {
            if (g_ChocoboAltUvSelectorCodes[i] == spriteIndex) {
                return 2;
            }
        }
    } else if (spriteIndex != 0x49) {
        return 2;
    }
    return 0;
}
/*
 * Submits a course quad primitive batch to the appropriate fixed render object.
 */
// FUNCTION: FF7 0x007706d7
int __cdecl Mini_ChocoboRaceEmitCourseQuad(int cameraMatrix, int segmentIndex, int unusedCount, int flags)
{
    int segment;
    ChocoboCourseQuad *quadRecord;

    segment = g_ChocoboTrackData->block + segmentIndex * 0x28;
    quadRecord = (ChocoboCourseQuad *)(g_ChocoboTrackData->builtQuads + (segmentIndex << 6));
    if (flags < 0) {
        return 0;
    } else {
        if (*(unsigned char *)((char *)segment + 0x27) == 0x38) {
            Mini_ChocoboEmitQuadRecord(cameraMatrix, g_ChocoboEmitQuadRenderObj1, (int)quadRecord);
            g_ChocoboEmitQuadDirty1 = 1;
        } else {
            Mini_ChocoboEmitQuadRecord(cameraMatrix, g_ChocoboEmitQuadRenderObj2, (int)quadRecord);
            g_ChocoboEmitQuadDirty2 = 1;
        }
        return 1;
    }
}
/*
 * Submits a course triangle primitive batch to the appropriate fixed render object.
 */
// FUNCTION: FF7 0x00770767
int __cdecl Mini_ChocoboRaceEmitCourseTriangle(int cameraMatrix, int triangleIndex, int unusedCount, int flags)
{
    int vertex;
    ChocoboTrackEntry *triangle;

    vertex = g_ChocoboTrackData->floatVertexTable + triangleIndex * 0x1C;
    triangle = (ChocoboTrackEntry *)(g_ChocoboTrackData->builtTriangles + triangleIndex * 0x30);
    if (flags < 0) {
        return 0;
    } else {
        if (*(unsigned char *)((char *)vertex + 0x1B) == 0x30) {
            Mini_ChocoboEmitTriangleRecord(cameraMatrix, g_ChocoboEmitTriangleRenderObj1, (int)triangle);
            g_ChocoboEmitTriangleDirty1 = 1;
        } else {
            Mini_ChocoboEmitTriangleRecord(cameraMatrix, g_ChocoboEmitTriangleRenderObj2, (int)triangle);
            g_ChocoboEmitTriangleDirty2 = 1;
        }
        return 1;
    }
}
} /* extern "C" */

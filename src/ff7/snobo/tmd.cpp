// src/ff7/snobo/tmd.cpp
// Snowboard TMD model loader, LZS stream unpacker, 16-entry texture descriptor table,
// TMD primitive submission (SnowboardTmdRenderer), and camera-facing race sprite quad renderer.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/pfile.h"
#include "lib/codegen.h"
#include <string.h>
#include "lib/enums/graphics.h"
#include "ff7/types/snobo.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/snobo_primitives.h"
#include "ff7/snobo_course.h"
#include "ff7/enums/snobo.h"
#include "ff7/enums/main.h"
#include "ff7/api/snobo.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"

#pragma intrinsic(memset)


// The 16 76-byte snowboard texture descriptors, UV corner pairs, variant tint colors,
// four-frame smoke UV tables, and sprite animation frame counter.
extern "C" {
// clang-format off
// GLOBAL: FF7 0x009624a0
SnowboardTextureRecord76 g_SnoboTextureRecords[16] = {
    {0, 0, 0, 0, 320, 128, 14, 48, 0, 0, 0,
     // STRING: FF7 0x00962a40
     "eyes.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 576, 0, 32, 64, 0, 0, 0,
     // STRING: FF7 0x00962a4c
     "tifaeye.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 448, 256, 32, 64, 0, 0, 0,
     // STRING: FF7 0x00962a58
     "ea_k.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 480, 256, 32, 64, 0, 0, 0,
     // STRING: FF7 0x00962a64
     "eb_k.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 448, 320, 32, 64, 0, 0, 0,
     // STRING: FF7 0x00962a70
     "ec_k.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 480, 320, 32, 64, 0, 0, 0,
     // STRING: FF7 0x00962a7c
     "eg_k.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 480, 384, 32, 7, 0, 0, 0,
     // STRING: FF7 0x00962a88
     "ita_h1.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 480, 392, 4, 7, 0, 0, 0,
     // STRING: FF7 0x00962a94
     "ita_h2.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 448, 384, 32, 64, 0, 0, 0,
     // STRING: FF7 0x00962aa0
     "eita_k.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 448, 192, 8, 32, 0, 0, 0,
     // STRING: FF7 0x00962aac
     "jump.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 320, 256, 32, 64, 0, 0, 0,
     // STRING: FF7 0x00962ab8
     "ki0.tim",
     4, 4, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 320, 384, 16, 128, 0, 0, 0,
     // STRING: FF7 0x00962ac0
     "smoke.tim",
     12, 1, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 384, 128, 80, 160, 0, 0, 0,
     // STRING: FF7 0x00962acc
     "fuusen1a.tim",
     4, 0, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 384, 128, 80, 160, 0, 0, 0,
     // STRING: FF7 0x00962adc
     "fuusen1b.tim",
     4, 0, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 384, 128, 80, 160, 0, 0, 0,
     // STRING: FF7 0x00962aec
     "fuusen1c.tim",
     4, 0, 0, {0}, {0}, {0}, {0}},
    {0, 0, 0, 0, 384, 128, 80, 160, 0, 0, 0,
     // STRING: FF7 0x00962afc
     "fuusen1d.tim",
     4, 0, 0, {0}, {0}, {0}, {0}},
};
// GLOBAL: FF7 0x00962960
SnowboardUvPair g_SnoboUvPairs[4] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
// GLOBAL: FF7 0x00962980
int g_SnoboTextureVariantColors[16] = {0x9fff0000, 0x9f0000ff, 0x9f00ff00, 0x9fffff00, 0x9fffffff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
// GLOBAL: FF7 0x009629c0
float g_SnoboSmokeUvFrame0[8] = {0.0f, 0.0f, 0.0f, 0.25f, 0.5f, 0.0f, 0.5f, 0.25f};
// GLOBAL: FF7 0x009629e0
float g_SnoboSmokeUvFrame1[8] = {0.0f, 0.25f, 0.0f, 0.5f, 0.5f, 0.25f, 0.5f, 0.5f};
// GLOBAL: FF7 0x00962a00
float g_SnoboSmokeUvFrame2[8] = {0.0f, 0.5f, 0.0f, 0.75f, 0.5f, 0.5f, 0.5f, 0.75f};
// GLOBAL: FF7 0x00962a20
float g_SnoboSmokeUvFrame3[8] = {0.0f, 0.75f, 0.0f, 1.0f, 0.5f, 0.75f, 0.5f, 1.0f};
// GLOBAL: FF7 0x00dd8704
short g_SnoboSpriteAnimFrame;
// clang-format on
}

inline unsigned int SnowboardPackBatchColor(unsigned int c) { return (((c & 0xff0000) >> 16) | 0xff000000) | (c & 0xff00) | ((c & 255) << 16); }

/*
 * Loads the model through loadTmd, then textures through loadTextures,
 * returning zero immediately upon either failure. On success obtains the
 * engine context, stores its width divided by 640 and height divided by 480 in
 * viewScaleX and viewScaleY, and returns 1.
 */
// FUNCTION: FF7 0x00731aa0
unsigned char SnowboardCourseLoader::load(int tmdData, int textureData)
{
    GfxEngineContext *context;
    if (!loadTmd(tmdData))
        return 0;
    if (!((SnowboardTextureLoader *)this)->loadTextures(textureData))
        return 0;
    context = GetEngineContext();
    viewScaleX = (float)context->viewSizeX / 640.0f;
    viewScaleY = (float)context->viewSizeY / 480.0f;
    return 1;
}

extern "C" {

/*
 * Releases the fastcall receiver's textures via SnowboardTextureLoader::releaseTextures,
 * then frees its TMD model (SnowboardCourseLoader::freeTmd).
 */
// FUNCTION: FF7 0x00731b15
void __fastcall Mini_SnowboardReleaseTmdModel(int self)
{
    ((SnowboardTextureLoader *)self)->releaseTextures();
    ((SnowboardCourseLoader *)self)->freeTmd();
}

}

/*
 * The course loader's destructor: passes the loader to Mini_SnowboardReleaseTmdModel, the
 * texture/model cleanup wrapper.
 */
// FUNCTION: FF7 0x00731b30
SnowboardCourseLoader::~SnowboardCourseLoader()
{
    Mini_SnowboardReleaseTmdModel((int)this);
}

struct TmdHeader {
    int id;
    unsigned int flags;
    int count;
};
struct TmdObject {
    int vertex;
    int vertexCount;
    int normal;
    int normalCount;
    int primitive;
    int primitiveCount;
    int scale;
};
// One TMD vertex: three shorts and a pad word.
struct TmdVertex {
    short x;
    short y;
    short z;
    short pad;
};

/*
 * Reads the named TMD file from archive 10 into a new heap buffer, stores the
 * object count and the buffer, and rebases each object's vertex, normal and
 * primitive offsets to addresses unless the header flag 0x80000000 says they
 * already are. Returns 1 on success, 0 on any failure.
 */
// FUNCTION: FF7 0x00731b43
unsigned char SnowboardCourseLoader::loadTmd(int name)
{
    unsigned int p;
    void *q;
    int entrySize;
    void *buffer;
    TmdHeader *pHdr;
    TmdObject *object;
    int left;

    q = FindLgpFile((char *)name, LGP_ARCHIVE_SNOWBOARD);
    if (q == 0)
        return 0;
    entrySize = GetLgpEntrySize((int)q, LGP_ARCHIVE_SNOWBOARD);
    // STRING: FF7 0x00962b0c
    buffer = GameHeapAllocRaw(entrySize, "C:\\FF7\\snobo\\tmd.cpp", 0x10A);
    if (buffer == 0)
        return 0;
    if (ReadLgpEntry(q, LGP_ARCHIVE_SNOWBOARD, buffer, entrySize) == 0) {
        free(buffer);
        return 0;
    }
    tmdBuffer = (TmdHeader *)buffer;
    p = (unsigned int)tmdBuffer;
    if (tmdBuffer != 0) {
        pHdr = (TmdHeader *)p;
        modelCount = pHdr->count;
        if ((pHdr->flags & 0x80000000) != 0)
            return 0;
        p += 12;
        object = (TmdObject *)p;
        p >>= 2;
        for (left = pHdr->count; left--; object++) {
            object->vertex += p * 4;
            object->normal += p * 4;
            object->primitive += p * 4;
        }
        pHdr->flags |= 0x80000000;
        return 1;
    }
    return 0;
}

/*
 * Frees the TMD buffer loadTmd allocated and clears the pointer.
 */
// FUNCTION: FF7 0x00731ca4
void SnowboardCourseLoader::freeTmd()
{
    // STRING: FF7 0x00962b24
    GameHeapFree(tmdBuffer, "C:\\FF7\\snobo\\tmd.cpp", 0x142);
    tmdBuffer = 0;
}

struct SnowboardDecoder {
    void unpack(int src, int dst);
};

/*
 * Decodes the length-prefixed LZS stream into dst, with source end
 * src+header+4. Reads flag bytes least-significant bit first; set bits copy
 * literals, clear bits consume a 12-bit reference position and length 3
 * through 18.
 */
// FUNCTION: FF7 0x00731cd2
void SnowboardDecoder::unpack(int src, int dst)
{
    int p;
    int q;
    int tempRefByte;
    int flagsLeft;
    int sourceStop;
    int outStart;
    int offset;
    int copyEnd;

    flagsLeft = 0;
    q = flagsLeft;
    sourceStop = src + *(int *)src + 4;
    src += 4;
    outStart = dst;
    while (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        if (flagsLeft == 0) {
            flagsLeft = 8;
            if ((unsigned int)src >= (unsigned int)sourceStop) {
                return;
            }
            q = *(unsigned char *)src;
            src++;
        }
        if ((q & 1) != 0) {
            if ((unsigned int)src >= (unsigned int)sourceStop) {
                return;
            }
            *(char *)dst = *(char *)src;
            dst++;
            src++;
        } else {
            if ((unsigned int)src >= (unsigned int)sourceStop) {
                return;
            }
            offset = *(unsigned char *)src;
            src++;
            tempRefByte = *(unsigned char *)src;
            src++;
            offset |= (tempRefByte & 0xF0) << 4;
            copyEnd = dst + (tempRefByte & 0xF) + 3;
            p = dst - (dst - outStart - (offset - 0xFEE) & 0xFFF);
            while ((unsigned int)p < (unsigned int)outStart) {
                *(char *)dst = 0;
                dst++;
                p++;
            }
            while ((unsigned int)dst < (unsigned int)copyEnd) {
                *(char *)dst = *(char *)p;
                dst++;
                p++;
            }
        }
        q >>= 1;
        flagsLeft--;
    }
}

/*
 * Initializes page, region and scaled texture dimensions for sixteen
 * descriptor entries and creates each render object, with extra flags for
 * entries twelve through fifteen. Returns zero at the first failure without
 * cleaning up prior entries.
 */
// FUNCTION: FF7 0x00731e52
unsigned char SnowboardTextureLoader::loadTextures(int unused)
{
    GfxEngineContext *p;
    int q, extraFlags, shift;
    p = GetEngineContext();
    for (q = 0; q < 16; q++) {
        shift = 2 - g_SnoboTextureRecords[q].bppMode;
        g_SnoboTextureRecords[q].id = q;
        g_SnoboTextureRecords[q].pageX = (g_SnoboTextureRecords[q].vramX >> 6) & 15;
        g_SnoboTextureRecords[q].pageY = (g_SnoboTextureRecords[q].vramY >> 8) & 1;
        g_SnoboTextureRecords[q].localX = g_SnoboTextureRecords[q].vramX - (g_SnoboTextureRecords[q].pageX << 6);
        g_SnoboTextureRecords[q].localY = g_SnoboTextureRecords[q].vramY - (g_SnoboTextureRecords[q].pageY << 8);
        g_SnoboTextureRecords[q].originU = g_SnoboTextureRecords[q].localX << shift;
        g_SnoboTextureRecords[q].originV = g_SnoboTextureRecords[q].localY;
        g_SnoboTextureRecords[q].scaleU = g_SnoboTextureRecords[q].widthWords << shift;
        g_SnoboTextureRecords[q].scaleV = g_SnoboTextureRecords[q].height;
        if (q >= 12 && q <= 15)
            extraFlags = 0x1000000;
        else
            extraFlags = 0;
        g_SnoboTextureRecords[q].renderObject = (GfxRenderObject *)createRenderObject(g_SnoboTextureRecords[q].objectType, g_SnoboTextureRecords[q].blendMode, (int)g_SnoboTextureRecords[q].filename, extraFlags);
        if (g_SnoboTextureRecords[q].renderObject == 0)
            return 0;
    }
    batchList = (GfxRenderObject *)createRenderObject(2, 0, 0, 0x4000);
    return 1;
}

/*
 * Zeroes load options, selects blend/path and fixed fields, incorporates
 * supplied flags and enables two additional option bits for flag 0x1000000.
 * Creates and returns the render object through the engine resource; no
 * additional success flag is applied.
 */
// FUNCTION: FF7 0x0073202e
int SnowboardTextureLoader::createRenderObject(int objectType, int blendMode, int fileName, int flags)
{
    struct {
        PfileLoadOptions options;
        GfxEngineContext *context;
        int result;
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
    frame.options.renderFlags |= flags;
    if ((flags & 0x1000000) != 0) {
        frame.options.paletteFlags |= 1;
        frame.options.paletteFlags |= 2;
    }
    frame.result = (int)Gfx_CreateRenderObject(1, objectType, &frame.options, (char *)fileName, (void *)frame.context->renderObjectBank);
    return frame.result;
}


/*
 * Destroys and clears the untextured render object (batchList) and the render
 * objects of all sixteen texture records in g_SnoboTextureRecords.
 */
// FUNCTION: FF7 0x007320f4
void SnowboardTextureLoader::releaseTextures()
{
    int i;
    DestroyRenderObject(batchList);
    batchList = 0;
    for (i = 0; i < 16; i++) {
        DestroyRenderObject(g_SnoboTextureRecords[i].renderObject);
        g_SnoboTextureRecords[i].renderObject = 0;
    }
}

struct SnowboardTmdScratchBlock {
    int header[4];
    SnowboardClipInputVertex vertices[84];
};

/*
 * Returns for an invalid model index, resolves its vertex and primitive arrays
 * and selects the supplied clipper matrix. Transforms up to eighty-four
 * vertices from short XYZ records, then dispatches primitive arrays to
 * triangle, shaded, textured or colored rendering by mode (enum SnowboardModelDrawMode).
 */
// FUNCTION: FF7 0x00732159
void SnowboardTmdRenderer::model(int matrix, int modelIndex, int mode, int flags)
{
    struct {
        float posX;
        float posY;
        float posZ;
        int vertexCount;
        TmdVertex *vertices;
        int vertexIndex;
        SnowboardTmdScratchBlock *dataBlock;
        TmdObject *object;
    } v;
    if (modelIndex < 0 || modelIndex >= this->modelCount)
        return;
    v.dataBlock = (SnowboardTmdScratchBlock *)Gfx_GetPsxScratchpadPtr(0);
    v.object = (TmdObject *)(this->modelTable + (modelIndex * 28) + 12);
    v.vertices = (TmdVertex *)v.object->vertex;
    v.vertexCount = v.object->vertexCount;
    g_SnoboStatePtr->clipper->setMatrix(matrix);
    for (v.vertexIndex = 0; v.vertexIndex < v.vertexCount && v.vertexIndex < 84; v.vertexIndex++) {
        v.posX = (float)v.vertices[v.vertexIndex].x;
        v.posY = (float)v.vertices[v.vertexIndex].y;
        v.posZ = (float)v.vertices[v.vertexIndex].z;
        g_SnoboStatePtr->clipper->transform((int)&v.posX, (int)(v.dataBlock->vertices + v.vertexIndex));
    }
    switch (mode) {
    case SNOWBOARD_MODEL_DRAW_FLAT:
        triangle(v.object->primitive, v.object->primitiveCount, flags);
        break;
    case SNOWBOARD_MODEL_DRAW_SHADED:
        shaded(v.object->primitive, v.object->primitiveCount, flags);
        break;
    case SNOWBOARD_MODEL_DRAW_TEXTURED:
        textured(v.object->primitive, v.object->primitiveCount, flags);
        break;
    case SNOWBOARD_MODEL_DRAW_COLORED:
        colored(v.object->primitive, v.object->primitiveCount, flags);
        break;
    }
}

/*
 * Walks count 24-byte flat triangle records, packs their three vertex colors
 * into the batch byte order and passes the three indexed data-block vertices
 * with those colors to the clipper.
 */
// FUNCTION: FF7 0x007322d6
void SnowboardTmdRenderer::triangle(int records, int count, int unused)
{
    SnowboardTmdScratchBlock *p;
    unsigned int q, pickedVertexColor, baseColorA;
    int thirdIdx, indexB, idxA, calcTriIndex;
    p = (SnowboardTmdScratchBlock *)Gfx_GetPsxScratchpadPtr(0);
    for (calcTriIndex = 0; calcTriIndex < count; calcTriIndex++, records += 24) {
        idxA = ((SnowboardTriangleRecord *)records)->indexA;
        indexB = ((SnowboardTriangleRecord *)records)->indexC;
        thirdIdx = ((SnowboardTriangleRecord *)records)->indexB;
        baseColorA = SnowboardPackBatchColor(((SnowboardTriangleRecord *)records)->colorA);
        pickedVertexColor = SnowboardPackBatchColor(((SnowboardTriangleRecord *)records)->colorC);
        q = SnowboardPackBatchColor(((SnowboardTriangleRecord *)records)->colorB);
        g_SnoboStatePtr->clipper->emitVertices((int)(p->vertices + idxA), (int)(p->vertices + indexB), (int)(p->vertices + thirdIdx), baseColorA, pickedVertexColor, q);
    }
}

/*
 * Walks count 24-byte triangle records at records, shades their three colors using
 * indexed data-block values and lightLevel, then passes three indexed vertices and
 * the shaded colors to the race state's clipper.
 */
// FUNCTION: FF7 0x00732429
void SnowboardTmdRenderer::shaded(int records, int count, int lightLevel)
{
    struct {
        int corner;
        int colors[3];
        int triIndex;
        int indices[3];
        int shadedColors[3];
        SnowboardTmdScratchBlock *dataBlock;
    } v;
    v.dataBlock = (SnowboardTmdScratchBlock *)Gfx_GetPsxScratchpadPtr(0);
    for (v.triIndex = 0; v.triIndex < count; v.triIndex++, records += 24) {
        v.indices[0] = ((SnowboardShadedRecord *)records)->index0;
        v.indices[1] = ((SnowboardShadedRecord *)records)->index2;
        v.indices[2] = ((SnowboardShadedRecord *)records)->index1;
        v.colors[0] = ((SnowboardShadedRecord *)records)->color0;
        v.colors[1] = ((SnowboardShadedRecord *)records)->color2;
        v.colors[2] = ((SnowboardShadedRecord *)records)->color1;
        for (v.corner = 0; v.corner < 3; v.corner++) {
            v.shadedColors[v.corner] = shade(v.colors[v.corner], v.dataBlock->vertices[v.indices[v.corner]].clipZ, lightLevel);
        }
        g_SnoboStatePtr->clipper->emitVertices((int)(v.dataBlock->vertices + v.indices[0]), (int)(v.dataBlock->vertices + v.indices[1]), (int)(v.dataBlock->vertices + v.indices[2]), v.shadedColors[0], v.shadedColors[1], v.shadedColors[2]);
    }
}

/*
 * Walks count 24-byte vertex-colored triangle records, packs their three colors
 * (with alpha 0x0E when flags is set, 0x5F otherwise), then reserves one
 * triangle in this campaign's batch list and stores the three projected
 * vertices with their colors.
 */
// FUNCTION: FF7 0x00732546
void SnowboardTmdRenderer::colored(int records, int count, int flags)
{
    struct {
        GfxTlVertex *vertex;
        struct {
            int posX;
            int posY;
            int posZ;
            int rhw;
        } projected;
        int corner;
        int triIndex;
        int indices[3];
        int colors[3];
        int dataBlock;
    } v;
    v.dataBlock = Gfx_GetPsxScratchpadPtr(0);
    for (v.triIndex = 0; v.triIndex < count; v.triIndex++, records += 24) {
        v.indices[0] = ((SnowboardTmdColoredTri *)records)->index0;
        v.indices[1] = ((SnowboardTmdColoredTri *)records)->index2;
        v.indices[2] = ((SnowboardTmdColoredTri *)records)->index1;
        if (flags != 0) {
            v.colors[0] = SnowboardPackBatchColor(((SnowboardTmdColoredTri *)records)->color0) & 0x0effffff;
            v.colors[1] = SnowboardPackBatchColor(((SnowboardTmdColoredTri *)records)->color2) & 0x0effffff;
            v.colors[2] = SnowboardPackBatchColor(((SnowboardTmdColoredTri *)records)->color1) & 0x0effffff;
        } else {
            v.colors[0] = SnowboardPackBatchColor(((SnowboardTmdColoredTri *)records)->color0) & 0x5fffffff;
            v.colors[1] = SnowboardPackBatchColor(((SnowboardTmdColoredTri *)records)->color2) & 0x5fffffff;
            v.colors[2] = SnowboardPackBatchColor(((SnowboardTmdColoredTri *)records)->color1) & 0x5fffffff;
        }
        if (AcquireBatchNode(1, (void *)this->batchList) != 0) {
            for (v.corner = 0; v.corner < 3; v.corner++) {
                g_SnoboStatePtr->clipper->project(v.dataBlock + (v.indices[v.corner] << 4) + 16, (int)&v.projected);
                if (flags != 0)
                    v.projected.posZ = 0;
                v.vertex = (GfxTlVertex *)(this->batchList->vertexCursor + (v.corner << 5));
                v.vertex->xBits = v.projected.posX;
                v.vertex->yBits = v.projected.posY;
                v.vertex->zBits = v.projected.posZ;
                v.vertex->rhwBits = v.projected.rhw;
                v.vertex->color = v.colors[v.corner];
            }
        }
    }
}

/*
 * Returns an opaque packed color after multiplying each of the first three
 * color bytes by a depth fade factor: 1 below zero, 0 above 4096, and 1 minus
 * depth/4096 between those bounds. Places the scaled first byte in bits
 * 16..23, second in bits 8..15 and third in bits 0..7.
 */
// FUNCTION: FF7 0x007327d3
int SnowboardTextureLoader::shade(SnowboardColor color, float depth, int unused)
{
    union {
        unsigned char bytes[8];
        struct {
            int color;
            float factor;
        } values;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (depth < 0.0f)
        frame.values.factor = 1.0f;
    else if (depth > 4096.0f)
        frame.values.factor = 0.0f;
    else
        frame.values.factor = 1.0f - (depth - 0.0f) * 0.000244140625f;
    frame.bytes[0] = (char)((color.value & 0xff) * frame.values.factor);
    frame.bytes[1] = (char)((*(int *)((char *)&color + 1) & 0xff) * frame.values.factor);
    frame.bytes[2] = (char)((*(int *)((char *)&color + 2) & 0xff) * frame.values.factor);
    return ((frame.values.color & 0xff) << 16) | 0xff000000 | ((*(int *)&frame.bytes[1] & 0xff) << 8) | (*(int *)&frame.bytes[2] & 0xff);
}

/*
 * Returns the cached texture descriptor for an input tag with bit 0x8000 set.
 * Otherwise searches sixteen descriptors for matching format/page fields and a
 * containing half-open UV rectangle, caches the selected descriptor index in
 * the input tag and returns it.
 */
// FUNCTION: FF7 0x007328a2
int SnowboardTmdRenderer::lookup(int tag)
{
    unsigned short p;
    int q, pageIdx, variant;
    unsigned short rectangleV, bytesV, currOriginU, texU, vCoord, sizeU;
    int a;
    p = ((SnowboardTexTag *)tag)->cacheWord;
    if ((p & 0x8000) == 0x8000)
        return (p & 0x7fff) * 76 + (int)g_SnoboTextureRecords;
    else {
        for (a = 0; a < 16; a++) {
            q = (p >> 7) & 3;
            pageIdx = p & 15;
            variant = (p >> 4) & 1;
            if (q == g_SnoboTextureRecords[a].bppMode && pageIdx == g_SnoboTextureRecords[a].pageX && variant == g_SnoboTextureRecords[a].pageY) {
                texU = ((SnowboardTexTag *)tag)->uvPacked & 255;
                vCoord = (((SnowboardTexTag *)tag)->uvPacked >> 8) & 255;
                currOriginU = g_SnoboTextureRecords[a].originULo;
                rectangleV = g_SnoboTextureRecords[a].originVLo;
                sizeU = g_SnoboTextureRecords[a].scaleULo;
                bytesV = g_SnoboTextureRecords[a].scaleVLo;
                if (texU >= currOriginU && texU < currOriginU + sizeU && vCoord >= rectangleV && vCoord < rectangleV + bytesV) {
                    ((SnowboardTexTag *)tag)->cacheWord = a | 0x8000;
                    return a * 76 + (int)g_SnoboTextureRecords;
                }
            }
        }
        return 0;
    }
}

/*
 * For texture descriptors twelve through fifteen, selects one of four fixed UV
 * pairs from source- byte boundary conditions. Other descriptors normalize
 * source bytes by descriptor origin/dimensions and mirror U for descriptor ten
 * in normal mode.
 */
// FUNCTION: FF7 0x00732a73
int SnowboardTmdRenderer::uv(int dst, unsigned short uvBytes, int descriptor)
{
    unsigned char byteU;
    SnowboardUvPair p;
    unsigned char byteV;
    int pairIndex;
    byteU = uvBytes & 255;
    byteV = (uvBytes >> 8) & 255;
    if (((SnowboardTextureRecord76 *)descriptor)->id >= 12 && ((SnowboardTextureRecord76 *)descriptor)->id <= 15) {
        pairIndex = 0;
        if (byteU != 0 && (byteU == 159 || (byteU + 1) % 40 != 0))
            pairIndex += 2;
        if (byteV % 128 != 0)
            pairIndex += 1;
        p = g_SnoboUvPairs[pairIndex];
        *(SnowboardUvPair *)dst = p;
        return dst;
    }
    p.x = (float)(byteU - ((SnowboardTextureRecord76 *)descriptor)->originU) / ((SnowboardTextureRecord76 *)descriptor)->scaleU;
    p.y = (float)(byteV - ((SnowboardTextureRecord76 *)descriptor)->originV) / ((SnowboardTextureRecord76 *)descriptor)->scaleV;
    if (g_SnoboModeFlag == 0 && ((SnowboardTextureRecord76 *)descriptor)->id == 10)
        p.x = 1.0f - p.x;
    *(SnowboardUvPair *)dst = p;
    return dst;
}

/*
 * Visits textured triangle records, resolves their texture descriptor and
 * three UV pairs and optionally redirects to a variant descriptor/color
 * according to variant. Marks that descriptor used, acquires a batch and
 * projects/stores three vertices with packed color and UVs.
 */
// FUNCTION: FF7 0x00732bb9
void SnowboardTmdRenderer::textured(int records, int count, int variant)
{
    struct {
        SnowboardUvBits uvScratchB;
        SnowboardUvBits uvScratchC;
        SnowboardUvBits uvScratchA;
        GfxTlVertex *vertex;
        struct {
            int posX;
            int posY;
            int posZ;
            int rhw;
        } projected;
        int corner;
        SnowboardTextureRecord76 *texture;
        GfxRenderObject *renderObject;
        int triIndex;
        int indices[3];
        int color;
        SnowboardUvBits uvs[3];
        int dataBlock;
        int specular;
    } v;
    v.specular = -1;
    v.color = -1;
    v.dataBlock = Gfx_GetPsxScratchpadPtr(0);
    for (v.triIndex = 0; v.triIndex < count; v.triIndex++, records += 28) {
        v.texture = (SnowboardTextureRecord76 *)lookup(records);
        if (v.texture != 0) {
            v.renderObject = v.texture->renderObject;
            v.indices[0] = ((SnowboardTexturedRecord *)records)->indexA;
            v.indices[1] = ((SnowboardTexturedRecord *)records)->indexC;
            v.indices[2] = ((SnowboardTexturedRecord *)records)->indexB;
            v.uvs[0] = *(SnowboardUvBits *)uv((int)&v.uvScratchA, ((SnowboardTexturedRecord *)records)->uvA, (int)v.texture);
            v.uvs[1] = *(SnowboardUvBits *)uv((int)&v.uvScratchC, ((SnowboardTexturedRecord *)records)->uvC, (int)v.texture);
            v.uvs[2] = *(SnowboardUvBits *)uv((int)&v.uvScratchB, ((SnowboardTexturedRecord *)records)->uvB, (int)v.texture);
            if (v.texture->blendMode == BLEND_AVERAGE) {
                v.texture += (variant >> 4) & 15;
                v.renderObject = v.texture->renderObject;
                v.color = g_SnoboTextureVariantColors[variant & 15];
            }
            v.texture->used = 1;
            if (AcquireBatchNode(1, (void *)v.renderObject) != 0) {
                for (v.corner = 0; v.corner < 3; v.corner++) {
                    g_SnoboStatePtr->clipper->project(v.dataBlock + (v.indices[v.corner] << 4) + 16, (int)&v.projected);
                    v.vertex = (GfxTlVertex *)(v.renderObject->vertexCursor + (v.corner << 5));
                    v.vertex->xBits = v.projected.posX;
                    v.vertex->yBits = v.projected.posY;
                    v.vertex->zBits = v.projected.posZ;
                    v.vertex->rhwBits = v.projected.rhw;
                    v.vertex->color = v.color;
                    v.vertex->specular = v.specular;
                    v.vertex->uBits = v.uvs[v.corner].x;
                    v.vertex->vBits = v.uvs[v.corner].y;
                }
            }
        }
    }
}

/*
 * Flushes the clipper, then emits and invalidates each marked texture
 * descriptor and clears its used flag. Non-blend-four descriptors temporarily
 * select scene pass one and disable render state two; blend-four descriptors
 * emit directly.
 */
// FUNCTION: FF7 0x00732dbb
void SnowboardTmdRenderer::emit()
{
    GfxEngineContext *p;
    int q;
    p = GetEngineContext();
    g_SnoboStatePtr->clipper->flush();
    for (q = 0; q < 16; q++) {
        if (g_SnoboTextureRecords[q].used != 0) {
            if (g_SnoboTextureRecords[q].blendMode != BLEND_NONE) {
                Gfx_SetScenePassCall(1, (struct GfxEngineContext *)p);
                Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)p);
                Gfx_EmitPrimitiveBatchList(g_SnoboTextureRecords[q].renderObject, p);
                InvalidateBatchGeneration((void *)g_SnoboTextureRecords[q].renderObject);
                Gfx_SetScenePassCall(0, (struct GfxEngineContext *)p);
                Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, (struct GfxEngineContext *)p);
            } else {
                Gfx_EmitPrimitiveBatchList(g_SnoboTextureRecords[q].renderObject, p);
                InvalidateBatchGeneration((void *)g_SnoboTextureRecords[q].renderObject);
            }
            g_SnoboTextureRecords[q].used = 0;
        }
    }
    Gfx_SetScenePassCall(1, (struct GfxEngineContext *)p);
    Gfx_EmitPrimitiveBatchList(this->batchList, p);
    InvalidateBatchGeneration((void *)this->batchList);
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)p);
}

/*
 * Draws a race sprite as a camera-facing 128x128 quad (scaled to the view)
 * through texture record 11 ("smoke.tim"), 36 units above its projected
 * position. Skips sprites past state 2, faded below 0x80, or hidden by the
 * current segment or probe flags; advance steps the four-frame UV animation.
 */
// FUNCTION: FF7 0x00732f0b
void SnowboardRaceSpriteRenderer::draw(int sprite, int advance)
{
    struct {
        GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        float *uv;
        float height;
        float zero[3];
        float width;
        float *tables[4];
        SnowboardRelativePosition position;
        short factor;
        int color;
        float matrix[16];
        unsigned char brightness;
        float projected[3];
        int unused;
        int fill;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    local.fill = -1;
    local.color = -1;
    local.tables[0] = g_SnoboSmokeUvFrame0;
    local.tables[1] = g_SnoboSmokeUvFrame1;
    local.tables[2] = g_SnoboSmokeUvFrame2;
    local.tables[3] = g_SnoboSmokeUvFrame3;
    if (g_SnoboStatePtr->renderFrame == 0 ||
        ((SnowboardRaceSprite *)sprite)->state > 2 ||
        ((SnowboardRaceSprite *)sprite)->fadeLevel < 0x80)
        return;
    if (g_SnoboModeFlag == 0) {
        if ((g_SnoboStatePtr->segmentPtr->chainBitTable[g_SnoboStatePtr->chainIndex] & 0x10) != 0)
            return;
    } else {
        if ((g_SnoboStatePtr->probeFlagBits & 4) != 0)
            return;
    }
    local.zero[0] = 0.0f;
    local.zero[1] = 0.0f;
    local.zero[2] = 0.0f;
    SetIdentityMatrix4x4((float(*)[4])local.matrix);
    Mini_SnowboardWriteRelativePosition(&local.position);
    local.matrix[12] = local.position.posX;
    local.matrix[13] = local.position.posY;
    local.matrix[14] = local.position.posZ;
    g_SnoboStatePtr->clipper->setMatrix((int)local.matrix);
    g_SnoboStatePtr->clipper->transformProject((int)local.zero, (int)local.projected);
    local.factor = (((SnowboardRaceSprite *)sprite)->fadeLevel >> 3) | 0xff;
    local.brightness = (local.factor * 255) / 255;
    local.color = 0xff000000 | (local.brightness << 16) |
                  (local.brightness << 8) | local.brightness;
    if (advance != 0)
        g_SnoboSpriteAnimFrame++;
    local.uv = local.tables[(g_SnoboSpriteAnimFrame >> 1) & 3];
    local.projected[1] -= 36.0f * this->heightScale;
    local.width = 64.0f * this->widthScale;
    local.height = 64.0f * this->heightScale;
    if (AcquireBatchNode(1, (void *)g_SnoboTextureRecords[11].renderObject)) {
        local.vertTL = (GfxTlVertex *)g_SnoboTextureRecords[11].renderObject->vertexCursor;
        local.vertTL->x = local.projected[0] - local.width;
        local.vertTL->y = local.projected[1] - local.height;
        local.vertTL->z = 0.0f;
        local.vertTL->rhwBits = local.unused;
        local.vertTL->color = local.color;
        local.vertTL->specular = local.fill;
        local.vertTL->u = local.uv[0];
        local.vertTL->v = local.uv[1];
        local.vertBL = (GfxTlVertex *)g_SnoboTextureRecords[11].renderObject->vertexCursor + 1;
        local.vertBL->x = local.projected[0] - local.width;
        local.vertBL->y = local.projected[1] + local.height;
        local.vertBL->z = 0.0f;
        local.vertBL->rhwBits = local.unused;
        local.vertBL->color = local.color;
        local.vertBL->specular = local.fill;
        local.vertBL->u = local.uv[2];
        local.vertBL->v = local.uv[3];
        local.vertTR = (GfxTlVertex *)g_SnoboTextureRecords[11].renderObject->vertexCursor + 2;
        local.vertTR->x = local.projected[0] + local.width;
        local.vertTR->y = local.projected[1] - local.height;
        local.vertTR->z = 0.0f;
        local.vertTR->rhwBits = local.unused;
        local.vertTR->color = local.color;
        local.vertTR->specular = local.fill;
        local.vertTR->u = local.uv[4];
        local.vertTR->v = local.uv[5];
        local.vertBR = (GfxTlVertex *)g_SnoboTextureRecords[11].renderObject->vertexCursor + 3;
        local.vertBR->x = local.projected[0] + local.width;
        local.vertBR->y = local.projected[1] + local.height;
        local.vertBR->z = 0.0f;
        local.vertBR->rhwBits = local.unused;
        local.vertBR->color = local.color;
        local.vertBR->specular = local.fill;
        local.vertBR->u = local.uv[6];
        local.vertBR->v = local.uv[7];
        g_SnoboTextureRecords[11].used = 1;
    }
}

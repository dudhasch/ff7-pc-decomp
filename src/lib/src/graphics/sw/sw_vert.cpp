// src/lib/src/graphics/sw/sw_vert.cpp
// Original: C:\lib\src\graphics\sw\sw_vert.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
extern "C" {

/*
 * Frees a software vertex buffer: its vertexData, then the record. Null is
 * accepted.
 */
// FUNCTION: FF7 0x006abc60
void __cdecl Gfx_RasterFreeVertexBuffer(GfxRasterVertexBuffer *buffer)
{
    if (buffer != 0) {
        // STRING: FF7 0x00917cc0
        GameHeapFree((void *)buffer->vertexData, "C:\\lib\\src\\graphics\\sw\\sw_vert.cpp", 31);
        // STRING: FF7 0x00917ce4
        GameHeapFree(buffer, "C:\\lib\\src\\graphics\\sw\\sw_vert.cpp", 32);
    }
}

/*
 * Allocates and initializes a software vertex buffer record and buffer.
 */
// FUNCTION: FF7 0x006abc94
int __cdecl Gfx_RasterCreateVertexBuffer(int vertexCount)
{
    GfxRasterVertexBuffer *record;
    int bufferData;

    // STRING: FF7 0x00917d08
    record = (GfxRasterVertexBuffer *)GameHeapAlloc(1, 80, "C:\\lib\\src\\graphics\\sw\\sw_vert.cpp", 40);
    if (record != 0) {
        record->vertexCount = vertexCount;
        // STRING: FF7 0x00917d2c
        record->vertexData = (int)GameHeapAlloc(vertexCount + 1, 96, "C:\\lib\\src\\graphics\\sw\\sw_vert.cpp", 44);
        if (record->vertexData == 0) {
            Gfx_RasterFreeVertexBuffer(record);
            record = 0;
        } else {
            bufferData = record->vertexData;
            record->alignedVertices = (bufferData + 0x1F) & ~0x1F;
        }
    }
    return (int)record;
}

/* Takes the float's bits as an int, as the asm twin pushes them (the 64-bit result goes to out). */
void __cdecl crt_FloatToInt64(int value, void *out);

// Copies the six-field clip rectangle from the context into clipRect. clipLeft, clipTop, viewSizeX
// and viewSizeY are copied unchanged, while clipRight and clipBottom are stored with one added to
// each, turning an inclusive edge into an exclusive one.
// FUNCTION: FF7 0x006abd19
void __cdecl Gfx_RasterCopyContextRectExclusive(SwIntClipRect *clipRect, GfxEngineContext *context)
{
    clipRect->clipLeft = context->clipLeft;
    clipRect->clipTop = context->clipTop;
    clipRect->viewSizeX = context->viewSizeX;
    clipRect->viewSizeY = context->viewSizeY;
    clipRect->clipRight = context->clipRight + 1;
    clipRect->clipBottom = context->clipBottom + 1;
}

/*
 * Mechanical setter: stores value into the record's stateA.
 * Note the order, the value is the first parameter and the record the second.
 */
// FUNCTION: FF7 0x006abd7e
void __cdecl Gfx_RasterSetStateModelMatrix(int value, struct GfxRasterVertexBuffer *record)
{
    record->stateA = value;
}

/*
 * Mechanical setter: stores value into the record's stateB,
 * the sibling of Gfx_RasterSetStateModelMatrix, which writes stateA.
 */
// FUNCTION: FF7 0x006abd8c
void __cdecl Gfx_RasterSetStateCombinedMatrix(int value, struct GfxRasterVertexBuffer *record)
{
    record->stateB = value;
}

/*
 * Stores value0 and value1 into the record's stateA and stateB.
 */
// FUNCTION: FF7 0x006abd9a
void __cdecl Gfx_RasterSetStateMatrixPair(int value0, int value1, GfxRasterVertexBuffer *record)
{
    record->stateA = value0;
    record->stateB = value1;
}

// Stores the float value and the tag in the record, then converts the float to an int into
// intValue.
// FUNCTION: FF7 0x006abdb1
void __cdecl Gfx_RasterSetStateFloatTag(float floatValue, int tag, SwStateFloatTag *record)
{
    record->floatValue = floatValue;
    record->tag = tag;
    record->intValue = (int)floatValue;
}

/*
 * Stores the two integers into the record's scaleX and scaleY.
 */
// FUNCTION: FF7 0x006abdd6
void __cdecl Gfx_RasterSetStateScalePair(int value0, int value1, GfxRasterVertexBuffer *record)
{
    record->scaleX = value0;
    record->scaleY = value1;
}

/* The C vertex resolvers (the hand-written ones below are their twins). While a vertex is not
 * resolved, each replaces x and y by the pixel coordinates as floats, through the data block's
 * int-to-float table, then does its share of the rest:
 *   ZUvRgbCompiled  z scaled; u and v from the source, scaled; red, green, blue; packed colour
 *   ZCompiled       z scaled; packed colour
 *   ColorOnlyCompiled  packed colour
 *   ZUvCompiled     z scaled; u and v
 *   UvCompiled      u and v (by the scale object's own u and v scales)
 *   ZRgbCompiled    z scaled; red, green, blue; packed colour
 *   RgbCompiled     red, green, blue; packed colour
 * Some load the scale object into a local they do not read; the originals do. */

// FUNCTION: FF7 0x006abded
void __cdecl Gfx_RasterResolveVertexZUvRgbCompiled(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *scaleObj;
    SwRasterSourceVertex *src;
    SwRasterRgbPaletteIndices *colorPtr;

    if (vertex->resolved == 0) {
        scaleObj = context->scale;
        src = (SwRasterSourceVertex *)vertex->source;
        vertex->xBits = *(int *)&context->intToFloat[vertex->pixelX];
        vertex->yBits = *(int *)&context->intToFloat[vertex->pixelY];
        vertex->depth *= scaleObj->scaleDepth;
        vertex->texCoordU = src->u * scaleObj->scaleU;
        vertex->texCoordV = src->v * scaleObj->scaleV;
        colorPtr = (SwRasterRgbPaletteIndices *)&src->packedColor;
        vertex->redBits = *(int *)&context->intToFloat[colorPtr->colorIndex2];
        vertex->greenBits = *(int *)&context->intToFloat[colorPtr->colorIndex1];
        vertex->blueBits = *(int *)&context->intToFloat[colorPtr->colorIndex0];
        vertex->packedColor = *(int *)colorPtr;
    }
}

/*
 * Resolves screen X/Y floats, scaled Z depth, and packed color for a software-rasterized vertex.
 */
// FUNCTION: FF7 0x006abed5
void __cdecl Gfx_RasterResolveVertexZCompiled(SwRasterVertex *vertex, SwRasterData *context)
{
    SwRasterSourceVertex *subrec;
    SwScaleZUv *scaleObj;
    int *subField;

    if (vertex->resolved == 0) {
        scaleObj = context->scale;
        subrec = (SwRasterSourceVertex *)vertex->source;
        vertex->xBits = *(int *)&context->intToFloat[vertex->pixelX];
        vertex->yBits = *(int *)&context->intToFloat[vertex->pixelY];
        vertex->depth *= scaleObj->scaleDepth;
        subField = &subrec->packedColor;
        vertex->packedColor = *subField;
    }
}

/*
 * Resolves screen X/Y floats and packed color (without Z or UVs) for a software-rasterized vertex.
 */
// FUNCTION: FF7 0x006abf4e
void __cdecl Gfx_RasterResolveVertexColorOnlyCompiled(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *obj;
    int *subVal;
    SwRasterSourceVertex *subRecord;

    if (vertex->resolved == 0) {
        obj = context->scale;
        subRecord = (SwRasterSourceVertex *)vertex->source;
        vertex->xBits = *(int *)&context->intToFloat[vertex->pixelX];
        vertex->yBits = *(int *)&context->intToFloat[vertex->pixelY];
        subVal = &subRecord->packedColor;
        vertex->packedColor = *subVal;
    }
}

/*
 * Resolves screen X/Y floats, scaled Z depth, and scaled U/V coordinates for a software-rasterized vertex.
 */
// FUNCTION: FF7 0x006abfb5
void __cdecl Gfx_RasterResolveVertexZUvCompiled(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *obj;
    SwRasterSourceVertex *subRecord;

    if (vertex->resolved == 0) {
        obj = context->scale;
        subRecord = (SwRasterSourceVertex *)vertex->source;
        vertex->xBits = *(int *)&context->intToFloat[vertex->pixelX];
        vertex->yBits = *(int *)&context->intToFloat[vertex->pixelY];
        vertex->depth *= obj->scaleDepth;
        vertex->texCoordU = subRecord->u * obj->scaleU;
        vertex->texCoordV = subRecord->v * obj->scaleV;
    }
}

/*
 * Resolves screen X/Y floats and scaled U/V coordinates (without Z) for a software-rasterized vertex.
 */
// FUNCTION: FF7 0x006ac03e
void __cdecl Gfx_RasterResolveVertexUvCompiled(SwRasterVertex *vertex, SwRasterData *context)
{
    GfxRasterUvScale *obj;
    SwRasterSourceVertex *subRecord;

    if (vertex->resolved == 0) {
        obj = (GfxRasterUvScale *)context->scale;
        subRecord = (SwRasterSourceVertex *)vertex->source;
        vertex->xBits = *(int *)&context->intToFloat[vertex->pixelX];
        vertex->yBits = *(int *)&context->intToFloat[vertex->pixelY];
        vertex->texCoordU = subRecord->u * obj->texScaleU;
        vertex->texCoordV = subRecord->v * obj->texScaleV;
    }
}

/*
 * Resolves screen X/Y floats, scaled Z depth, and unpacked RGB channel floats for a software-rasterized vertex.
 */
// FUNCTION: FF7 0x006ac0b5
void __cdecl Gfx_RasterResolveVertexZRgbCompiled(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *obj;
    SwRasterSourceVertex *subRecord;
    SwRasterRgbPaletteIndices *colorPtr;

    if (vertex->resolved == 0) {
        obj = context->scale;
        subRecord = (SwRasterSourceVertex *)vertex->source;
        vertex->xBits = *(int *)&context->intToFloat[vertex->pixelX];
        vertex->yBits = *(int *)&context->intToFloat[vertex->pixelY];
        vertex->depth *= obj->scaleDepth;
        colorPtr = (SwRasterRgbPaletteIndices *)&subRecord->packedColor;
        vertex->redBits = *(int *)&context->intToFloat[colorPtr->colorIndex2];
        vertex->greenBits = *(int *)&context->intToFloat[colorPtr->colorIndex1];
        vertex->blueBits = *(int *)&context->intToFloat[colorPtr->colorIndex0];
        vertex->packedColor = *(int *)colorPtr;
    }
}

/*
 * Resolves screen X/Y floats and unpacked RGB channel floats (without Z) for a software-rasterized vertex.
 */
// FUNCTION: FF7 0x006ac179
void __cdecl Gfx_RasterResolveVertexRgbCompiled(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *obj;
    SwRasterSourceVertex *subRecord;
    SwRasterRgbPaletteIndices *colorPtr;

    if (vertex->resolved == 0) {
        obj = context->scale;
        subRecord = (SwRasterSourceVertex *)vertex->source;
        vertex->xBits = *(int *)&context->intToFloat[vertex->pixelX];
        vertex->yBits = *(int *)&context->intToFloat[vertex->pixelY];
        colorPtr = (SwRasterRgbPaletteIndices *)&subRecord->packedColor;
        vertex->redBits = *(int *)&context->intToFloat[colorPtr->colorIndex2];
        vertex->greenBits = *(int *)&context->intToFloat[colorPtr->colorIndex1];
        vertex->blueBits = *(int *)&context->intToFloat[colorPtr->colorIndex0];
        vertex->packedColor = *(int *)colorPtr;
    }
}

/*
 * Resets aggregate clip OR to zero and clip AND to -1, then converts 32-byte
 * source vertices into 96-byte software vertex records when count does not exceed
 * the buffer's capacity. Transforms by the matrix at the buffer's stateB, records the source
 * pointer, clears per-vertex flags, and classifies W, projected depth and screen
 * bounds.
 */
// FUNCTION: FF7 0x006ac22b
void __cdecl Gfx_RasterPrepareVertexRecordsAsm(int count, int sourceVertices, int buffer, GfxRasterContext *context)
{
    struct {
        float zero, one;
        int callbacks, output, vertices;
        float transformed[4];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    local.vertices = sourceVertices;
    local.output = ((SwVertexRecordBuffer *)buffer)->output;
    *(int *)buffer = 0;
    ((SwVertexRecordBuffer *)buffer)->clipAnd = -1;
    local.callbacks = (int)&context->callbacks;
    if (count <= ((SwVertexRecordBuffer *)buffer)->capacity) {
        local.one = 1.0f;
        local.zero = 0.0f;
        __asm {
        mov esi, local.vertices
        mov edi, local.output
        mov ebx, buffer
        mov eax, dword ptr [esi]
    L6ac28a:
        lea ecx, local.transformed
        push ecx
        push esi
        push dword ptr [ebx + 030h]
        call TransformPointByMatrix4x4
        sub esp, 0ch
        lea ecx, local.transformed
        mov dword ptr [edi + 038h], esi
        xor eax, eax
        mov dword ptr [edi + 03ch], eax
        mov dword ptr [edi + 020h], eax
        mov dword ptr [edi + 024h], eax
        fld dword ptr [ecx + 0ch]
        fcomp local.zero
        fnstsw ax
        test ah, 041h
        jne L6ac357
        push edi
        push ecx
        call PerspectiveDivideWithRhw
        sub esp, 8
        mov eax, dword ptr [edi + 8]
        and eax, 080000000h
        jne L6ac350
        fld dword ptr [edi + 8]
        fcomp local.one
        fnstsw ax
        test ah, 1
        je L6ac349
        lea edx, [edi + 028h]
        push edx
        push dword ptr [edi]
        call crt_FloatToInt64
        sub esp, 8
        lea edx, [edi + 030h]
        push edx
        push dword ptr [edi + 4]
        call crt_FloatToInt64
        sub esp, 8
        mov edx, dword ptr [edi + 02ch]
        xor ecx, ecx
        cmp edx, dword ptr [ebx + 014h]
        setl al
        mov cl, al
        cmp edx, dword ptr [ebx + 024h]
        setg al
        shl al, 1
        mov edx, dword ptr [edi + 034h]
        or cl, al
        cmp edx, dword ptr [ebx + 018h]
        setl al
        shl al, 2
        or cl, al
        cmp edx, dword ptr [ebx + 028h]
        setg al
        shl al, 3
        or cl, al
        or ecx, ecx
        jne L6ac35c
        mov eax, local.callbacks
        push context
        push edi
        call dword ptr [eax + 01ch]
        sub esp, 8
        mov byte ptr [edi + 020h], 1
        xor ecx, ecx
        jmp L6ac35c
    L6ac349:
        mov ecx, 020h
        jmp L6ac35c
    L6ac350:
        mov ecx, 010h
        jmp L6ac35c
    L6ac357:
        mov ecx, 0100h
    L6ac35c:
        mov edx, count
        mov dword ptr [edi + 024h], ecx
        or dword ptr [ebx], ecx
        and dword ptr [ebx + 4], ecx
        dec edx
        jle L6ac37b
        mov eax, dword ptr [esi + 020h]
        mov count, edx
        add esi, 020h
        add edi, 060h
        jmp L6ac28a
    L6ac37b:
        }
    } else {
        Gfx_DebugPrint(
            // STRING: FF7 0x00917d50
            "ERROR: SW VERTEX OVERFLOW \n");
    }
}

/*
 * The C twin of Gfx_RasterPrepareVertexRecordsAsm: sets the buffer's clip OR to 0 and clip AND to
 * -1, then, when count fits the capacity, turns each 32-byte source vertex into a 96-byte
 * SwRasterVertex (see its clipFlags) and runs the context's resolveVertex callback on each one in
 * view. The source position is copied as it is (the asm twin transforms it by the matrix in stateB).
 */
// FUNCTION: FF7 0x006ac391
void __cdecl Gfx_RasterPrepareVertexRecords(int count, SwLineVertexRecord *srcVerts, GfxRasterVertexBuffer *state, GfxRasterContext *context)
{
    SwRasterSourceVertex *srcVertex;
    int idx;
    GfxRasterCallbacks *vt;
    SwRasterVertex *dest;
    int bits;

    srcVertex = (SwRasterSourceVertex *)srcVerts;
    dest = (SwRasterVertex *)state->alignedVertices;
    state->clipFlags = 0;
    state->clipAnd = -1;
    vt = &context->callbacks;
    if (count <= state->vertexCount) {
        for (idx = count; idx > 0; idx--) {
            dest->source = (int)srcVertex;
            dest->isSet = 0;
            dest->resolved = 0;
            dest->clipFlags = 0;
            if (srcVertex->w > 0.0f) {
                dest->xBits = srcVertex->xBits;
                dest->yBits = srcVertex->yBits;
                dest->depthBits = srcVertex->zBits;
                dest->wBits = srcVertex->wBits;
                bits = dest->depthBits;
                if ((bits & 0x80000000) == 0) {
                    if (dest->depth < 1.0f) {
                        crt_FloatToInt64(dest->xBits, (char *)&dest->fixedXLow);
                        crt_FloatToInt64(dest->yBits, (char *)&dest->fixedYLow);
                        if (dest->pixelX < state->minX) {
                            dest->clipFlags |= 1;
                        }
                        if (dest->pixelX > state->maxX) {
                            dest->clipFlags |= 2;
                        }
                        if (dest->pixelY < state->minY) {
                            dest->clipFlags |= 4;
                        }
                        if (dest->pixelY > state->maxY) {
                            dest->clipFlags |= 8;
                        }
                        if (dest->clipFlags == 0) {
                            vt->resolveVertex((int)dest, (int)context);
                            dest->isSet = 1;
                        }
                    } else {
                        dest->clipFlags = 0x20;
                    }
                } else {
                    dest->clipFlags = 0x10;
                }
            } else {
                dest->clipFlags = 0x100;
            }
            state->clipFlags |= dest->clipFlags;
            state->clipAnd &= dest->clipFlags;
            srcVertex++;
            dest++;
        }
    } else {
        Gfx_DebugPrint(
            // STRING: FF7 0x00917d6c
            "ERROR: SW VERTEX OVERFLOW \n");
    }
}

// These original fixed-register variants retain ESI/EDI/EBX across the x87
// operations. VC5 supplies the prologue, register saves and epilogue.

// Assembly twin of a C vertex resolver. While the vertex is not resolved, it replaces x and y by
// the pixel coordinates as floats, through the data block's int-to-float table; it also scales the
// depth by the scale object, scales the source u and v into texCoordU and texCoordV, looks the
// source colour bytes up in the same table into red, green and blue, copies the source packed
// colour.
// FUNCTION: FF7 0x006ac5ba
void __cdecl Gfx_RasterResolveVertexZUvRgb(int vertex, int context)
{
    __asm {
        mov esi, context
        mov edi, vertex
        cmp byte ptr [edi+3Ch], 0
        jne done
        mov edx, dword ptr [esi+18Ch]
        lea esi, [esi+27B8h]
        mov eax, dword ptr [edi+2Ch]
        mov ebx, dword ptr [edi+34h]
        mov ecx, dword ptr [edi+38h]
        mov eax, dword ptr [esi+eax*4]
        mov ebx, dword ptr [esi+ebx*4]
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], ebx
        fld dword ptr [edi+8]
        fmul dword ptr [edx+38h]
        fstp dword ptr [edi+8]
        fld dword ptr [ecx+18h]
        fmul dword ptr [edx+3Ch]
        fstp dword ptr [edi+40h]
        fld dword ptr [ecx+1Ch]
        fmul dword ptr [edx+40h]
        fstp dword ptr [edi+44h]
        xor eax, eax
        mov al, byte ptr [ecx+12h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+48h], ebx
        mov al, byte ptr [ecx+11h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+4Ch], ebx
        mov al, byte ptr [ecx+10h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+50h], ebx
        mov eax, dword ptr [ecx+10h]
        mov dword ptr [edi+54h], eax
        done:
    }
}

// Assembly twin of a C vertex resolver. While the vertex is not resolved, it replaces x and y by
// the pixel coordinates as floats, through the data block's int-to-float table; it also scales the
// depth by the scale object, copies the source packed colour.
// FUNCTION: FF7 0x006ac62f
void __cdecl Gfx_RasterResolveVertexZ(int vertex, int context)
{
    __asm {
        mov esi, context
        mov edi, vertex
        cmp byte ptr [edi+3Ch], 0
        jne done
        mov edx, dword ptr [esi+18Ch]
        lea esi, [esi+27B8h]
        mov eax, dword ptr [edi+2Ch]
        mov ebx, dword ptr [edi+34h]
        mov ecx, dword ptr [edi+38h]
        mov eax, dword ptr [esi+eax*4]
        mov ebx, dword ptr [esi+ebx*4]
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], ebx
        fld dword ptr [edi+8]
        fmul dword ptr [edx+38h]
        fstp dword ptr [edi+8]
        mov eax, dword ptr [ecx+10h]
        mov dword ptr [edi+54h], eax
    done:
    }
}

// Assembly twin of a C vertex resolver. While the vertex is not resolved, it replaces x and y by
// the pixel coordinates as floats, through the data block's int-to-float table; it also copies the
// source packed colour.
// FUNCTION: FF7 0x006ac675
void __cdecl Gfx_RasterResolveVertexColorOnly(int vertex, int context)
{
    __asm {
        mov esi, context
        mov edi, vertex
        cmp byte ptr [edi+3Ch], 0
        jne done
        mov edx, dword ptr [esi+18Ch]
        lea esi, [esi+27B8h]
        mov eax, dword ptr [edi+2Ch]
        mov ebx, dword ptr [edi+34h]
        mov ecx, dword ptr [edi+38h]
        mov eax, dword ptr [esi+eax*4]
        mov ebx, dword ptr [esi+ebx*4]
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], ebx
        mov eax, dword ptr [ecx+10h]
        mov dword ptr [edi+54h], eax
    done:
    }
}

// Assembly twin of a C vertex resolver. While the vertex is not resolved, it replaces x and y by
// the pixel coordinates as floats, through the data block's int-to-float table; it also scales the
// depth by the scale object, scales the source u and v into texCoordU and texCoordV.
// FUNCTION: FF7 0x006ac6b2
void __cdecl Gfx_RasterResolveVertexZUv(int vertex, int context)
{
    __asm {
        mov esi, context
        mov edi, vertex
        cmp byte ptr [edi+3Ch], 0
        jne done
        mov edx, dword ptr [esi+18Ch]
        lea esi, [esi+27B8h]
        mov eax, dword ptr [edi+2Ch]
        mov ebx, dword ptr [edi+34h]
        mov ecx, dword ptr [edi+38h]
        mov eax, dword ptr [esi+eax*4]
        mov ebx, dword ptr [esi+ebx*4]
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], ebx
        fld dword ptr [edi+8]
        fmul dword ptr [edx+38h]
        fstp dword ptr [edi+8]
        fld dword ptr [ecx+18h]
        fmul dword ptr [edx+3Ch]
        fstp dword ptr [edi+40h]
        fld dword ptr [ecx+1Ch]
        fmul dword ptr [edx+40h]
        fstp dword ptr [edi+44h]
    done:
    }
}

// Assembly twin of a C vertex resolver. While the vertex is not resolved, it replaces x and y by
// the pixel coordinates as floats, through the data block's int-to-float table; it also scales the
// source u and v into texCoordU and texCoordV.
// FUNCTION: FF7 0x006ac704
void __cdecl Gfx_RasterResolveVertexUv(int vertex, int context)
{
    __asm {
        mov esi, context
        mov edi, vertex
        cmp byte ptr [edi+3Ch], 0
        jne done
        mov edx, dword ptr [esi+18Ch]
        lea esi, [esi+27B8h]
        mov eax, dword ptr [edi+2Ch]
        mov ebx, dword ptr [edi+34h]
        mov ecx, dword ptr [edi+38h]
        mov eax, dword ptr [esi+eax*4]
        mov ebx, dword ptr [esi+ebx*4]
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], ebx
        fld dword ptr [ecx+18h]
        fmul dword ptr [edx+3Ch]
        fstp dword ptr [edi+40h]
        fld dword ptr [ecx+1Ch]
        fmul dword ptr [edx+40h]
        fstp dword ptr [edi+44h]
    done:
    }
}

// Assembly twin of a C vertex resolver. While the vertex is not resolved, it replaces x and y by
// the pixel coordinates as floats, through the data block's int-to-float table; it also scales the
// depth by the scale object, scales the source u and v into texCoordU and texCoordV.
// FUNCTION: FF7 0x006ac74d
void __cdecl Gfx_RasterResolveVertexZUvStacked(int vertex, int context)
{
    __asm {
        mov esi, context
        mov edi, vertex
        cmp byte ptr [edi+3Ch], 0
        jne done
        mov edx, dword ptr [esi+18Ch]
        lea esi, [esi+27B8h]
        mov eax, dword ptr [edi+2Ch]
        mov ebx, dword ptr [edi+34h]
        mov ecx, dword ptr [edi+38h]
        fld dword ptr [edi+8]
        fmul dword ptr [edx+38h]
        fld dword ptr [ecx+18h]
        fmul dword ptr [edx+3Ch]
        fld dword ptr [ecx+1Ch]
        fmul dword ptr [edx+40h]
        fxch st(2)
        mov eax, dword ptr [esi+eax*4]
        mov ebx, dword ptr [esi+ebx*4]
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], ebx
        fstp dword ptr [edi+8]
        fstp dword ptr [edi+40h]
        fstp dword ptr [edi+44h]
    done:
    }
}

// Assembly twin of a C vertex resolver. While the vertex is not resolved, it replaces x and y by
// the pixel coordinates as floats, through the data block's int-to-float table; it also scales the
// depth by the scale object, looks the source colour bytes up in the same table into red, green and
// blue, copies the source packed colour.
// FUNCTION: FF7 0x006ac7a1
void __cdecl Gfx_RasterResolveVertexZRgb(int vertex, int context)
{
    __asm {
        mov esi, context
        mov edi, vertex
        cmp byte ptr [edi+3Ch], 0
        jne done
        mov edx, dword ptr [esi+18Ch]
        lea esi, [esi+27B8h]
        mov eax, dword ptr [edi+2Ch]
        mov ebx, dword ptr [edi+34h]
        mov ecx, dword ptr [edi+38h]
        mov eax, dword ptr [esi+eax*4]
        mov ebx, dword ptr [esi+ebx*4]
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], ebx
        fld dword ptr [edi+8]
        fmul dword ptr [edx+38h]
        fstp dword ptr [edi+8]
        xor eax, eax
        mov al, byte ptr [ecx+12h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+48h], ebx
        mov al, byte ptr [ecx+11h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+4Ch], ebx
        mov al, byte ptr [ecx+10h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+50h], ebx
        mov eax, dword ptr [ecx+10h]
        mov dword ptr [edi+54h], eax
        done:
    }
}

// Assembly twin of a C vertex resolver. While the vertex is not resolved, it replaces x and y by
// the pixel coordinates as floats, through the data block's int-to-float table; it also looks the
// source colour bytes up in the same table into red, green and blue, copies the source packed
// colour.
// FUNCTION: FF7 0x006ac804
void __cdecl Gfx_RasterResolveVertexRgb(int vertex, int context)
{
    __asm {
        mov esi, context
        mov edi, vertex
        cmp byte ptr [edi+3Ch], 0
        jne done
        mov edx, dword ptr [esi+18Ch]
        lea esi, [esi+27B8h]
        mov eax, dword ptr [edi+2Ch]
        mov ebx, dword ptr [edi+34h]
        mov ecx, dword ptr [edi+38h]
        mov eax, dword ptr [esi+eax*4]
        mov ebx, dword ptr [esi+ebx*4]
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], ebx
        xor eax, eax
        mov al, byte ptr [ecx+12h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+48h], ebx
        mov al, byte ptr [ecx+11h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+4Ch], ebx
        mov al, byte ptr [ecx+10h]
        mov ebx, dword ptr [esi+eax*4]
        mov dword ptr [edi+50h], ebx
        mov eax, dword ptr [ecx+10h]
        mov dword ptr [edi+54h], eax
        done:
    }
}
} /* extern "C" */

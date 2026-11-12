// Contents: The snowboard clipper: the SnowboardClipper class holds the engine context and matrices,
//   transforms and projects points with a native x87 routine, and clips triangles and quads against
//   the near plane before emitting them as render batches.
// Native member functions.
// Separate compilation preserves the period C code generation of its other bodies.
#include "ff7/types/snobo.h"
#include "lib/pfile.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <string.h>
#include "ff7/api/snobo.h"
#include "ff7/snobo_primitives.h"
#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"

#pragma intrinsic(memset)

// clang-format off
extern "C" {
// GLOBAL: FF7 0x00962b40
int g_SnoboQuadIndices[2][4] = {{0, 1, 3, 2}, {1, 0, 2, 3}};
}
// clang-format on

extern "C" void Mini_SnowboardTransformPoint3ByMatrix4(void *matrix, int point, int out);
#pragma code_seg(".text")

// Constructor for the snowboard clipper. It stores the engine context in ctx, its d3dDevice2 in
// d3dDevice2 and its cameraMatrix in cameraMatrix, and points screenTransform at the context's screen
// transform matrix. Matrix4x4Invert writes the inverse of that matrix to inverseScreenMatrix. It
// initialises the load options with blend 4, then creates two render objects with arguments (1, 2)
// and (1, 10), the first stored in triangleObject and the second in quadObject, both using the
// context's renderObjectBank.
// FUNCTION: FF7 0x00733360
SnowboardClipper::SnowboardClipper()
{
    PfileLoadOptions options = {0};
    this->ctx = GetEngineContext();
    this->d3dDevice2 = this->ctx->d3dDevice2;
    this->cameraMatrix = this->ctx->cameraMatrix;
    this->screenTransform = (int)this->ctx->screenTransform;
    Matrix4x4Invert(this->screenTransform, &this->inverseScreenMatrix);
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &options);
    this->triangleObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, &options, 0, (void *)this->ctx->renderObjectBank);
    this->quadObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, &options, 0, (void *)this->ctx->renderObjectBank);
}

/*
 * Clipper destructor: destroys triangleObject and quadObject; does not clear
 * those fields.
 */
// FUNCTION: FF7 0x00733451
SnowboardClipper::~SnowboardClipper()
{
    DestroyRenderObject(this->triangleObject);
    DestroyRenderObject(this->quadObject);
}

// Copies the 64-byte matrix argument into modelMatrix, multiplies it by cameraMatrix into viewMatrix,
// and multiplies that result by screenTransform into viewScreenMatrix.
// FUNCTION: FF7 0x00733479
void SnowboardClipper::setMatrix(int matrix)
{
    this->modelMatrix = *(SnowboardMatrixWords64 *)matrix;
    MultiplyMatrix4x4Full(&this->modelMatrix, (void *)this->cameraMatrix, &this->viewMatrix);
    MultiplyMatrix4x4Full(&this->viewMatrix, (void *)this->screenTransform, &this->viewScreenMatrix);
}

/*
 * Transforms the three floats at point by the 4x4 matrix at matrix (rows 0-2,
 * plus row 3 as the translation) into the four floats at out. Original
 * hand-written x87 code.
 */
// FUNCTION: FF7 0x007334da
extern "C" __declspec(naked) void __cdecl Mini_SnowboardTransformPoint3ByMatrix4(void *matrix, int point, int out)
{
    // EBX, ESI and EDI are saved though unused, as in the original.
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov ecx, dword ptr [ebp + 0Ch]
        mov edx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebp + 10h]
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 4]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 8]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 0Ch]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 1Ch]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 14h]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 18h]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 10h]
        fxch st(3)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 2Ch]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 24h]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 28h]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 20h]
        fxch st(3)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        fld dword ptr [edx + 30h]
        fld dword ptr [edx + 34h]
        fld dword ptr [edx + 38h]
        fld dword ptr [edx + 3Ch]
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        fstp dword ptr [eax + 0Ch]
        fstp dword ptr [eax + 8]
        fstp dword ptr [eax + 4]
        fstp dword ptr [eax]
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Forwards viewScreenMatrix and point/out to
 * Mini_SnowboardTransformPoint3ByMatrix4.
 */
// FUNCTION: FF7 0x00733564
void SnowboardClipper::transform(int point, int out)
{
    Mini_SnowboardTransformPoint3ByMatrix4(&this->viewScreenMatrix, point, out);
}

/*
 * Divides the x, y and z of the four floats at input by their w and stores them
 * with the reciprocal w at output. Does not check w for zero or negative
 * values. The body is the original inline x87 code.
 */
// FUNCTION: FF7 0x0073358b
void SnowboardClipper::project(int input, int output)
{
    int local = input;
    __asm {
        mov esi, local
        mov edi, output
        fld1
        fdiv dword ptr [esi + 0Ch]
        fld dword ptr [esi + 4]
        fmul st(0), st(1)
        fld dword ptr [esi + 8]
        fmul st(0), st(2)
        fld dword ptr [esi]
        fmul st(0), st(3)
        fstp dword ptr [edi]
        fxch st(1)
        fstp dword ptr [edi + 4]
        fstp dword ptr [edi + 8]
        fstp dword ptr [edi + 0Ch]
    }
}

/*
 * Transforms point by viewScreenMatrix into a local four-float vertex and
 * projects it into projected.
 */
// FUNCTION: FF7 0x007335cc
void SnowboardClipper::transformProject(int point, int projected)
{
    int vertex[4];
    transform(point, (int)vertex);
    project((int)vertex, projected);
}

/*
 * Clips a triangle of three vertices and colours against the plane clipZ = 0.5.
 * With no vertex behind the plane, copies all three with their reciprocal w and
 * returns 3; with one behind, writes the four vertices of the clipped quad and
 * returns 4; with two behind, writes the clipped triangle with its reciprocal w
 * and returns 3; with all three behind, writes nothing and returns 0. The
 * crossing vertices take colours interpolated by Mini_SnowboardLerpPackedColor.
 */
// FUNCTION: FF7 0x007335fb
int SnowboardClipper::clip(int vertices, int colors, int outVertices, int outColors, int reciprocalW)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[184];
        float recipWC3;
        struct { unsigned char pad_to_4[0x4]; float recipWIn2; };
        struct { unsigned char pad_to_8[0x8]; float recipWD3; };
        struct { unsigned char pad_to_C[0xC]; float recipWD2; };
        struct { unsigned char pad_to_10[0x10]; float recipWC2; };
        struct { unsigned char pad_to_14[0x14]; float recipWIn1; };
        struct { unsigned char pad_to_18[0x18]; float recipWIn0; };
        struct { unsigned char pad_to_1C[0x1C]; float recipWD1; };
        struct { unsigned char pad_to_20[0x20]; float recipWC1; };
        struct { unsigned char pad_to_24[0x24]; int keptIndex; };
        struct { unsigned char pad_to_28[0x28]; float lerpPrev2; };
        struct { unsigned char pad_to_2C[0x2C]; float lerpNext2; };
        struct { unsigned char pad_to_30[0x30]; SnowboardClipInputVertex crossC; };
        struct { unsigned char pad_to_40[0x40]; SnowboardClipInputVertex crossD; };
        struct { unsigned char pad_to_50[0x50]; int rejectedPrevIndex; };
        struct { unsigned char pad_to_54[0x54]; int crossCColor; };
        struct { unsigned char pad_to_58[0x58]; int rejectedNextIndex; };
        struct { unsigned char pad_to_5C[0x5C]; int crossDColor; };
        struct { unsigned char pad_to_60[0x60]; float lerpPrev; };
        struct { unsigned char pad_to_64[0x64]; float lerpNext; };
        struct { unsigned char pad_to_68[0x68]; SnowboardClipInputVertex crossA; };
        struct { unsigned char pad_to_78[0x78]; int rejectedIndex; };
        struct { unsigned char pad_to_7C[0x7C]; int keptPrevIndex; };
        struct { unsigned char pad_to_80[0x80]; SnowboardClipInputVertex crossB; };
        struct { unsigned char pad_to_90[0x90]; int keptNextIndex; };
        struct { unsigned char pad_to_94[0x94]; int crossAColor; };
        struct { unsigned char pad_to_98[0x98]; int crossBColor; };
        struct { unsigned char pad_to_9C[0x9C]; float recipW; };
        struct { unsigned char pad_to_A0[0xA0]; int outputIndex; };
        struct { unsigned char pad_to_A4[0xA4]; int rejectedCount; };
        struct { unsigned char pad_to_A8[0xA8]; int rejected[3]; };
        struct { unsigned char pad_to_B4[0xB4]; int vertexIndex; };
    } frame;
#pragma pack(pop)

    frame.rejectedCount = 0;
    for (frame.vertexIndex = 0; frame.vertexIndex < 3; frame.vertexIndex++) {
        frame.rejected[frame.vertexIndex] = (((SnowboardClipInputVertex *)vertices)[frame.vertexIndex].clipZ < 0.5f) ? 1 : 0;
        if (frame.rejected[frame.vertexIndex] != 0) {
            frame.rejectedCount++;
        }
    }
    if (frame.rejectedCount == 0) {
        for (frame.outputIndex = 0; frame.outputIndex < 3; frame.outputIndex++) {
            frame.recipW = 1.0f / ((SnowboardClipInputVertex *)vertices)[frame.outputIndex].w;
            ((SnowboardClipInputVertex *)outVertices)[frame.outputIndex] = ((SnowboardClipInputVertex *)vertices)[frame.outputIndex];
            ((int *)outColors)[frame.outputIndex] = ((int *)colors)[frame.outputIndex];
            ((float *)reciprocalW)[frame.outputIndex] = frame.recipW;
        }
        return 3;
    } else if (frame.rejectedCount == 1) {
        if (frame.rejected[0] != 0) {
            frame.rejectedIndex = 0;
            frame.keptNextIndex = 1;
            frame.keptPrevIndex = 2;
        }
        if (frame.rejected[1] != 0) {
            frame.rejectedIndex = 1;
            frame.keptNextIndex = 2;
            frame.keptPrevIndex = 0;
        }
        if (frame.rejected[2] != 0) {
            frame.rejectedIndex = 2;
            frame.keptNextIndex = 0;
            frame.keptPrevIndex = 1;
        }
        frame.lerpNext = (0.5f - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].clipZ) / (((SnowboardClipInputVertex *)vertices)[frame.keptNextIndex].clipZ - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].clipZ);
        frame.lerpPrev = (0.5f - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].clipZ) / (((SnowboardClipInputVertex *)vertices)[frame.keptPrevIndex].clipZ - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].clipZ);
        frame.crossA.x = ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].x + (((SnowboardClipInputVertex *)vertices)[frame.keptNextIndex].x - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].x) * frame.lerpNext;
        frame.crossA.y = ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].y + (((SnowboardClipInputVertex *)vertices)[frame.keptNextIndex].y - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].y) * frame.lerpNext;
        frame.crossA.w = ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].w + (((SnowboardClipInputVertex *)vertices)[frame.keptNextIndex].w - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].w) * frame.lerpNext;
        frame.crossA.zBits = FLOAT_HALF_BITS;
        frame.crossB.x = ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].x + (((SnowboardClipInputVertex *)vertices)[frame.keptPrevIndex].x - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].x) * frame.lerpPrev;
        frame.crossB.y = ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].y + (((SnowboardClipInputVertex *)vertices)[frame.keptPrevIndex].y - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].y) * frame.lerpPrev;
        frame.crossB.w = ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].w + (((SnowboardClipInputVertex *)vertices)[frame.keptPrevIndex].w - ((SnowboardClipInputVertex *)vertices)[frame.rejectedIndex].w) * frame.lerpPrev;
        frame.crossB.zBits = FLOAT_HALF_BITS;
        Mini_SnowboardLerpPackedColor(colors + frame.rejectedIndex * 4, colors + frame.keptNextIndex * 4, frame.lerpNext, &frame.crossAColor);
        Mini_SnowboardLerpPackedColor(colors + frame.rejectedIndex * 4, colors + frame.keptPrevIndex * 4, frame.lerpPrev, &frame.crossBColor);
        if (frame.rejectedIndex == 0) {
            ((SnowboardClipInputVertex *)outVertices)[0] = frame.crossA;
            ((int *)outColors)[0] = frame.crossAColor;
            ((SnowboardClipInputVertex *)outVertices)[1] = ((SnowboardClipInputVertex *)vertices)[1];
            ((int *)outColors)[1] = ((int *)colors)[1];
            ((SnowboardClipInputVertex *)outVertices)[2] = ((SnowboardClipInputVertex *)vertices)[2];
            ((int *)outColors)[2] = ((int *)colors)[2];
            ((SnowboardClipInputVertex *)outVertices)[3] = frame.crossB;
            ((int *)outColors)[3] = frame.crossBColor;
            return 4;
        }
        if (frame.rejectedIndex == 1) {
            ((SnowboardClipInputVertex *)outVertices)[0] = ((SnowboardClipInputVertex *)vertices)[0];
            ((int *)outColors)[0] = ((int *)colors)[0];
            ((SnowboardClipInputVertex *)outVertices)[1] = frame.crossB;
            ((int *)outColors)[1] = frame.crossBColor;
            ((SnowboardClipInputVertex *)outVertices)[2] = frame.crossA;
            ((int *)outColors)[2] = frame.crossAColor;
            ((SnowboardClipInputVertex *)outVertices)[3] = ((SnowboardClipInputVertex *)vertices)[2];
            ((int *)outColors)[3] = ((int *)colors)[2];
            return 4;
        }
        ((SnowboardClipInputVertex *)outVertices)[0] = ((SnowboardClipInputVertex *)vertices)[0];
        ((int *)outColors)[0] = ((int *)colors)[0];
        ((SnowboardClipInputVertex *)outVertices)[1] = ((SnowboardClipInputVertex *)vertices)[1];
        ((int *)outColors)[1] = ((int *)colors)[1];
        ((SnowboardClipInputVertex *)outVertices)[2] = frame.crossB;
        ((int *)outColors)[2] = frame.crossBColor;
        ((SnowboardClipInputVertex *)outVertices)[3] = frame.crossA;
        ((int *)outColors)[3] = frame.crossAColor;
        return 4;
    } else if (frame.rejectedCount == 2) {
        if (frame.rejected[0] == 0) {
            frame.keptIndex = 0;
            frame.rejectedNextIndex = 1;
            frame.rejectedPrevIndex = 2;
        } else if (frame.rejected[1] == 0) {
            frame.keptIndex = 1;
            frame.rejectedNextIndex = 2;
            frame.rejectedPrevIndex = 0;
        } else {
            frame.keptIndex = 2;
            frame.rejectedNextIndex = 0;
            frame.rejectedPrevIndex = 1;
        }
        frame.lerpNext2 = (0.5f - ((SnowboardClipInputVertex *)vertices)[frame.rejectedNextIndex].clipZ) / (((SnowboardClipInputVertex *)vertices)[frame.keptIndex].clipZ - ((SnowboardClipInputVertex *)vertices)[frame.rejectedNextIndex].clipZ);
        frame.lerpPrev2 = (0.5f - ((SnowboardClipInputVertex *)vertices)[frame.rejectedPrevIndex].clipZ) / (((SnowboardClipInputVertex *)vertices)[frame.keptIndex].clipZ - ((SnowboardClipInputVertex *)vertices)[frame.rejectedPrevIndex].clipZ);
        frame.crossC.x = ((SnowboardClipInputVertex *)vertices)[frame.rejectedNextIndex].x + (((SnowboardClipInputVertex *)vertices)[frame.keptIndex].x - ((SnowboardClipInputVertex *)vertices)[frame.rejectedNextIndex].x) * frame.lerpNext2;
        frame.crossC.y = ((SnowboardClipInputVertex *)vertices)[frame.rejectedNextIndex].y + (((SnowboardClipInputVertex *)vertices)[frame.keptIndex].y - ((SnowboardClipInputVertex *)vertices)[frame.rejectedNextIndex].y) * frame.lerpNext2;
        frame.crossC.w = ((SnowboardClipInputVertex *)vertices)[frame.rejectedNextIndex].w + (((SnowboardClipInputVertex *)vertices)[frame.keptIndex].w - ((SnowboardClipInputVertex *)vertices)[frame.rejectedNextIndex].w) * frame.lerpNext2;
        frame.crossC.zBits = FLOAT_HALF_BITS;
        frame.crossD.x = ((SnowboardClipInputVertex *)vertices)[frame.rejectedPrevIndex].x + (((SnowboardClipInputVertex *)vertices)[frame.keptIndex].x - ((SnowboardClipInputVertex *)vertices)[frame.rejectedPrevIndex].x) * frame.lerpPrev2;
        frame.crossD.y = ((SnowboardClipInputVertex *)vertices)[frame.rejectedPrevIndex].y + (((SnowboardClipInputVertex *)vertices)[frame.keptIndex].y - ((SnowboardClipInputVertex *)vertices)[frame.rejectedPrevIndex].y) * frame.lerpPrev2;
        frame.crossD.w = ((SnowboardClipInputVertex *)vertices)[frame.rejectedPrevIndex].w + (((SnowboardClipInputVertex *)vertices)[frame.keptIndex].w - ((SnowboardClipInputVertex *)vertices)[frame.rejectedPrevIndex].w) * frame.lerpPrev2;
        frame.crossD.zBits = FLOAT_HALF_BITS;
        Mini_SnowboardLerpPackedColor(colors + frame.rejectedNextIndex * 4, colors + frame.keptIndex * 4, frame.lerpNext2, &frame.crossCColor);
        Mini_SnowboardLerpPackedColor(colors + frame.rejectedPrevIndex * 4, colors + frame.keptIndex * 4, frame.lerpPrev2, &frame.crossDColor);
        if (frame.keptIndex == 0) {
            frame.recipWIn0 = 1.0f / ((SnowboardClipInputVertex *)vertices)[0].w;
            frame.recipWC1 = 1.0f / frame.crossC.w;
            frame.recipWD1 = 1.0f / frame.crossD.w;
            ((SnowboardClipInputVertex *)outVertices)[0] = ((SnowboardClipInputVertex *)vertices)[0];
            ((int *)outColors)[0] = ((int *)colors)[0];
            ((SnowboardClipInputVertex *)outVertices)[1] = frame.crossC;
            ((int *)outColors)[1] = frame.crossCColor;
            ((SnowboardClipInputVertex *)outVertices)[2] = frame.crossD;
            ((int *)outColors)[2] = frame.crossDColor;
            ((float *)reciprocalW)[0] = frame.recipWIn0;
            ((float *)reciprocalW)[1] = frame.recipWC1;
            ((float *)reciprocalW)[2] = frame.recipWD1;
            return 3;
        }
        if (frame.keptIndex == 1) {
            frame.recipWD2 = 1.0f / frame.crossD.w;
            frame.recipWIn1 = 1.0f / ((SnowboardClipInputVertex *)vertices)[1].w;
            frame.recipWC2 = 1.0f / frame.crossC.w;
            ((SnowboardClipInputVertex *)outVertices)[0] = frame.crossD;
            ((int *)outColors)[0] = frame.crossDColor;
            ((SnowboardClipInputVertex *)outVertices)[1] = ((SnowboardClipInputVertex *)vertices)[1];
            ((int *)outColors)[1] = ((int *)colors)[1];
            ((SnowboardClipInputVertex *)outVertices)[2] = frame.crossC;
            ((int *)outColors)[2] = frame.crossCColor;
            ((float *)reciprocalW)[0] = frame.recipWD2;
            ((float *)reciprocalW)[1] = frame.recipWIn1;
            ((float *)reciprocalW)[2] = frame.recipWC2;
            return 3;
        }
        frame.recipWC3 = 1.0f / frame.crossC.w;
        frame.recipWD3 = 1.0f / frame.crossD.w;
        frame.recipWIn2 = 1.0f / ((SnowboardClipInputVertex *)vertices)[2].w;
        ((SnowboardClipInputVertex *)outVertices)[0] = frame.crossC;
        ((int *)outColors)[0] = frame.crossCColor;
        ((SnowboardClipInputVertex *)outVertices)[1] = frame.crossD;
        ((int *)outColors)[1] = frame.crossDColor;
        ((SnowboardClipInputVertex *)outVertices)[2] = ((SnowboardClipInputVertex *)vertices)[2];
        ((int *)outColors)[2] = ((int *)colors)[2];
        ((float *)reciprocalW)[0] = frame.recipWC3;
        ((float *)reciprocalW)[1] = frame.recipWD3;
        ((float *)reciprocalW)[2] = frame.recipWIn2;
        return 3;
    } else {
        return 0;
    }
}

/*
 * Interpolates each of the four packed bytes from the word at from toward the
 * word at to by factor and writes the resulting packed word to result; preserves
 * truncation through char conversion.
 */
// FUNCTION: FF7 0x0073404c
extern "C" void __cdecl Mini_SnowboardLerpPackedColor(int from, int to, float factor, int *result)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        int words[3];
        char bytes[12];
    } frame;
    frame.words[1] = *(int *)from;
    frame.words[0] = *(int *)to;
    frame.bytes[8] = (char)(((*(int *)&frame.bytes[0] & 0xff) - (*(int *)&frame.bytes[4] & 0xff)) * factor + (*(int *)&frame.bytes[4] & 0xff));
    frame.bytes[9] = (char)(((*(int *)&frame.bytes[1] & 0xff) - (*(int *)&frame.bytes[5] & 0xff)) * factor + (*(int *)&frame.bytes[5] & 0xff));
    frame.bytes[10] = (char)(((*(int *)&frame.bytes[2] & 0xff) - (*(int *)&frame.bytes[6] & 0xff)) * factor + (*(int *)&frame.bytes[6] & 0xff));
    frame.bytes[11] = (char)(((*(int *)&frame.bytes[3] & 0xff) - (*(int *)&frame.bytes[7] & 0xff)) * factor + (*(int *)&frame.bytes[7] & 0xff));
    *result = frame.words[2];
}

/*
 * Clips a triangle of three vertices against the plane clipZ = 0.5, as clip()
 * does but without colours or reciprocal w: returns 3 or 4 output vertices, or 0
 * when the whole triangle is behind the plane, keeping the polygon order.
 */
// FUNCTION: FF7 0x0073413a
int SnowboardClipper::clipPositions(int inVerts, int outVerts)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        int keptIndex;
        struct { unsigned char pad_to_4[0x4]; float lerpPrev2; };
        struct { unsigned char pad_to_8[0x8]; float lerpNext2; };
        struct { unsigned char pad_to_C[0xC]; SnowboardClipInputVertex crossC; };
        struct { unsigned char pad_to_1C[0x1C]; SnowboardClipInputVertex crossD; };
        struct { unsigned char pad_to_2C[0x2C]; int rejectedPrevIndex; };
        struct { unsigned char pad_to_30[0x30]; int rejectedNextIndex; };
        struct { unsigned char pad_to_34[0x34]; float lerpPrev; };
        struct { unsigned char pad_to_38[0x38]; float lerpNext; };
        struct { unsigned char pad_to_3C[0x3C]; SnowboardClipInputVertex crossA; };
        struct { unsigned char pad_to_4C[0x4C]; int rejectedIndex; };
        struct { unsigned char pad_to_50[0x50]; int keptPrevIndex; };
        struct { unsigned char pad_to_54[0x54]; SnowboardClipInputVertex crossB; };
        struct { unsigned char pad_to_64[0x64]; int keptNextIndex; };
        struct { unsigned char pad_to_68[0x68]; int outputIndex; };
        struct { unsigned char pad_to_6C[0x6C]; int rejectedCount; };
        struct { unsigned char pad_to_70[0x70]; int rejected[3]; };
        struct { unsigned char pad_to_7C[0x7C]; int vertexIndex; };
    } frame;
#pragma pack(pop)

    frame.rejectedCount = 0;
    for (frame.vertexIndex = 0; frame.vertexIndex < 3; frame.vertexIndex++) {
        frame.rejected[frame.vertexIndex] = (((SnowboardClipInputVertex *)inVerts)[frame.vertexIndex].clipZ < 0.5f) ? 1 : 0;
        if (frame.rejected[frame.vertexIndex] != 0) {
            frame.rejectedCount++;
        }
    }
    if (frame.rejectedCount == 0) {
        for (frame.outputIndex = 0; frame.outputIndex < 3; frame.outputIndex++) {
            ((SnowboardClipInputVertex *)outVerts)[frame.outputIndex] = ((SnowboardClipInputVertex *)inVerts)[frame.outputIndex];
        }
        return 3;
    } else if (frame.rejectedCount == 1) {
        if (frame.rejected[0] != 0) {
            frame.rejectedIndex = 0;
            frame.keptNextIndex = 1;
            frame.keptPrevIndex = 2;
        }
        if (frame.rejected[1] != 0) {
            frame.rejectedIndex = 1;
            frame.keptNextIndex = 2;
            frame.keptPrevIndex = 0;
        }
        if (frame.rejected[2] != 0) {
            frame.rejectedIndex = 2;
            frame.keptNextIndex = 0;
            frame.keptPrevIndex = 1;
        }
        frame.lerpNext = (0.5f - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].clipZ) / (((SnowboardClipInputVertex *)inVerts)[frame.keptNextIndex].clipZ - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].clipZ);
        frame.lerpPrev = (0.5f - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].clipZ) / (((SnowboardClipInputVertex *)inVerts)[frame.keptPrevIndex].clipZ - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].clipZ);
        frame.crossA.x = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].x + (((SnowboardClipInputVertex *)inVerts)[frame.keptNextIndex].x - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].x) * frame.lerpNext;
        frame.crossA.y = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].y + (((SnowboardClipInputVertex *)inVerts)[frame.keptNextIndex].y - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].y) * frame.lerpNext;
        frame.crossA.w = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].w + (((SnowboardClipInputVertex *)inVerts)[frame.keptNextIndex].w - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].w) * frame.lerpNext;
        frame.crossA.zBits = FLOAT_HALF_BITS;
        frame.crossB.x = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].x + (((SnowboardClipInputVertex *)inVerts)[frame.keptPrevIndex].x - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].x) * frame.lerpPrev;
        frame.crossB.y = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].y + (((SnowboardClipInputVertex *)inVerts)[frame.keptPrevIndex].y - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].y) * frame.lerpPrev;
        frame.crossB.w = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].w + (((SnowboardClipInputVertex *)inVerts)[frame.keptPrevIndex].w - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedIndex].w) * frame.lerpPrev;
        frame.crossB.zBits = FLOAT_HALF_BITS;
        if (frame.rejectedIndex == 0) {
            ((SnowboardClipInputVertex *)outVerts)[0] = frame.crossA;
            ((SnowboardClipInputVertex *)outVerts)[1] = ((SnowboardClipInputVertex *)inVerts)[1];
            ((SnowboardClipInputVertex *)outVerts)[2] = ((SnowboardClipInputVertex *)inVerts)[2];
            ((SnowboardClipInputVertex *)outVerts)[3] = frame.crossB;
            return 4;
        }
        if (frame.rejectedIndex == 1) {
            ((SnowboardClipInputVertex *)outVerts)[0] = ((SnowboardClipInputVertex *)inVerts)[0];
            ((SnowboardClipInputVertex *)outVerts)[1] = frame.crossB;
            ((SnowboardClipInputVertex *)outVerts)[2] = frame.crossA;
            ((SnowboardClipInputVertex *)outVerts)[3] = ((SnowboardClipInputVertex *)inVerts)[2];
            return 4;
        }
        ((SnowboardClipInputVertex *)outVerts)[0] = ((SnowboardClipInputVertex *)inVerts)[0];
        ((SnowboardClipInputVertex *)outVerts)[1] = ((SnowboardClipInputVertex *)inVerts)[1];
        ((SnowboardClipInputVertex *)outVerts)[2] = frame.crossB;
        ((SnowboardClipInputVertex *)outVerts)[3] = frame.crossA;
        return 4;
    } else if (frame.rejectedCount == 2) {
        if (frame.rejected[0] == 0) {
            frame.keptIndex = 0;
            frame.rejectedNextIndex = 1;
            frame.rejectedPrevIndex = 2;
        } else if (frame.rejected[1] == 0) {
            frame.keptIndex = 1;
            frame.rejectedNextIndex = 2;
            frame.rejectedPrevIndex = 0;
        } else {
            frame.keptIndex = 2;
            frame.rejectedNextIndex = 0;
            frame.rejectedPrevIndex = 1;
        }
        frame.lerpNext2 = (0.5f - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedNextIndex].clipZ) / (((SnowboardClipInputVertex *)inVerts)[frame.keptIndex].clipZ - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedNextIndex].clipZ);
        frame.lerpPrev2 = (0.5f - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedPrevIndex].clipZ) / (((SnowboardClipInputVertex *)inVerts)[frame.keptIndex].clipZ - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedPrevIndex].clipZ);
        frame.crossC.x = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedNextIndex].x + (((SnowboardClipInputVertex *)inVerts)[frame.keptIndex].x - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedNextIndex].x) * frame.lerpNext2;
        frame.crossC.y = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedNextIndex].y + (((SnowboardClipInputVertex *)inVerts)[frame.keptIndex].y - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedNextIndex].y) * frame.lerpNext2;
        frame.crossC.w = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedNextIndex].w + (((SnowboardClipInputVertex *)inVerts)[frame.keptIndex].w - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedNextIndex].w) * frame.lerpNext2;
        frame.crossC.zBits = FLOAT_0_6_BITS;
        frame.crossD.x = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedPrevIndex].x + (((SnowboardClipInputVertex *)inVerts)[frame.keptIndex].x - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedPrevIndex].x) * frame.lerpPrev2;
        frame.crossD.y = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedPrevIndex].y + (((SnowboardClipInputVertex *)inVerts)[frame.keptIndex].y - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedPrevIndex].y) * frame.lerpPrev2;
        frame.crossD.w = ((SnowboardClipInputVertex *)inVerts)[frame.rejectedPrevIndex].w + (((SnowboardClipInputVertex *)inVerts)[frame.keptIndex].w - ((SnowboardClipInputVertex *)inVerts)[frame.rejectedPrevIndex].w) * frame.lerpPrev2;
        frame.crossD.zBits = FLOAT_0_6_BITS;
        if (frame.keptIndex == 0) {
            ((SnowboardClipInputVertex *)outVerts)[0] = ((SnowboardClipInputVertex *)inVerts)[0];
            ((SnowboardClipInputVertex *)outVerts)[1] = frame.crossC;
            ((SnowboardClipInputVertex *)outVerts)[2] = frame.crossD;
            return 3;
        }
        if (frame.keptIndex == 1) {
            ((SnowboardClipInputVertex *)outVerts)[0] = frame.crossD;
            ((SnowboardClipInputVertex *)outVerts)[1] = ((SnowboardClipInputVertex *)inVerts)[1];
            ((SnowboardClipInputVertex *)outVerts)[2] = frame.crossC;
            return 3;
        }
        ((SnowboardClipInputVertex *)outVerts)[0] = frame.crossC;
        ((SnowboardClipInputVertex *)outVerts)[1] = frame.crossD;
        ((SnowboardClipInputVertex *)outVerts)[2] = ((SnowboardClipInputVertex *)inVerts)[2];
        return 3;
    } else {
        return 0;
    }
}

/*
 * Clips the triangle into local vertex, colour and reciprocal-w arrays and
 * writes the result to triangleObject: three vertices as one triangle, four as
 * the two triangles (0, 1, 2) and (0, 2, 3). Each vertex is x, y and z times
 * the reciprocal w, the reciprocal w and the colour. Sets triangleDirty.
 */
// FUNCTION: FF7 0x0073488d
void SnowboardClipper::emitTriangle(int vertices, int colors)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[220];
        SnowboardClipInputVertex *srcVecC;
        struct { unsigned char pad_to_4[0x4]; float scaledZ2; };
        struct { unsigned char pad_to_8[0x8]; float scaledY2; };
        struct { unsigned char pad_to_C[0xC]; float scaledX2; };
        struct { unsigned char pad_to_10[0x10]; SnowboardClipInputVertex *srcVecB; };
        struct { unsigned char pad_to_14[0x14]; float scaledZ1; };
        struct { unsigned char pad_to_18[0x18]; float scaledY1; };
        struct { unsigned char pad_to_1C[0x1C]; float scaledX1; };
        struct { unsigned char pad_to_20[0x20]; SnowboardClipInputVertex *srcVecA; };
        struct { unsigned char pad_to_24[0x24]; float scaledZ; };
        struct { unsigned char pad_to_28[0x28]; float scaledY; };
        struct { unsigned char pad_to_2C[0x2C]; float scaledX; };
        struct { unsigned char pad_to_30[0x30]; float recipW2; };
        struct { unsigned char pad_to_34[0x34]; int srcIndex2; };
        struct { unsigned char pad_to_38[0x38]; float recipW1; };
        struct { unsigned char pad_to_3C[0x3C]; int srcIndex1; };
        struct { unsigned char pad_to_40[0x40]; int *indexPtr; };
        struct { unsigned char pad_to_44[0x44]; int i; };
        struct { unsigned char pad_to_48[0x48]; GfxTlVertex *quadVertex; };
        struct { unsigned char pad_to_4C[0x4C]; int quadIdx0; };
        struct { unsigned char pad_to_50[0x50]; int quadIdx1; };
        struct { unsigned char pad_to_54[0x54]; int quadIdx2; };
        struct { unsigned char pad_to_58[0x58]; int quadIdx3; };
        struct { unsigned char pad_to_5C[0x5C]; int quadIdx4; };
        struct { unsigned char pad_to_60[0x60]; int quadIdx5; };
        struct { unsigned char pad_to_64[0x64]; GfxRenderObject *clipState; };
        struct { unsigned char pad_to_68[0x68]; float vertexRecipW; };
        struct { unsigned char pad_to_6C[0x6C]; int vertexIndex; };
        struct { unsigned char pad_to_70[0x70]; GfxTlVertex *batchVertex; };
        struct { unsigned char pad_to_74[0x74]; GfxRenderObject *batchNode; };
        struct { unsigned char pad_to_78[0x78]; SnowboardClipInputVertex clippedVerts[4]; };
        struct { unsigned char pad_to_B8[0xB8]; int clipCount; };
        struct { unsigned char pad_to_BC[0xBC]; float clipRecipW[4]; };
        struct { unsigned char pad_to_CC[0xCC]; int clipColors[4]; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.clipCount = clip(vertices, colors, (int)&frame.clippedVerts[0], (int)&frame.clipColors[0], (int)&frame.clipRecipW[0]);
    if (frame.clipCount == 3) {
        frame.batchNode = this->triangleObject;
        if (AcquireBatchNode(1, frame.batchNode) != 0) {
            frame.batchVertex = frame.batchNode->vertices;
            frame.vertexIndex = 0;
            while (frame.vertexIndex < 3) {
                frame.vertexRecipW = frame.clipRecipW[frame.vertexIndex];
                frame.batchVertex->color = frame.clipColors[frame.vertexIndex];
                this->triangleDirty = 1;
                frame.srcVecA = &frame.clippedVerts[0] + frame.vertexIndex;
                frame.scaledX = frame.srcVecA->x * frame.vertexRecipW;
                frame.scaledY = frame.srcVecA->y * frame.vertexRecipW;
                frame.scaledZ = frame.srcVecA->clipZ * frame.vertexRecipW;
                frame.batchVertex->x = frame.scaledX;
                frame.batchVertex->y = frame.scaledY;
                frame.batchVertex->z = frame.scaledZ;
                frame.batchVertex->rhw = frame.vertexRecipW;
                frame.vertexIndex++;
                frame.batchVertex++;
            }
        }
    } else if (frame.clipCount == 4) {
        frame.quadIdx0 = 0;
        frame.quadIdx1 = 1;
        frame.quadIdx2 = 2;
        frame.quadIdx3 = 0;
        frame.quadIdx4 = 2;
        frame.quadIdx5 = 3;
        frame.clipState = this->triangleObject;
        if (AcquireBatchNode(2, frame.clipState) != 0) {
            frame.indexPtr = &frame.quadIdx0;
            frame.quadVertex = frame.clipState->vertices;
            for (frame.i = 0; frame.i < 4; frame.i++) {
                frame.clipRecipW[frame.i] = 1.0f / frame.clippedVerts[frame.i].w;
            }
            for (frame.i = 0; frame.i < 3; frame.i++) {
                frame.srcIndex1 = *frame.indexPtr;
                frame.indexPtr++;
                frame.recipW1 = frame.clipRecipW[frame.srcIndex1];
                frame.quadVertex->color = frame.clipColors[frame.srcIndex1];
                frame.srcVecB = &frame.clippedVerts[0] + frame.srcIndex1;
                frame.scaledX1 = frame.srcVecB->x * frame.recipW1;
                frame.scaledY1 = frame.srcVecB->y * frame.recipW1;
                frame.scaledZ1 = frame.srcVecB->clipZ * frame.recipW1;
                frame.quadVertex->x = frame.scaledX1;
                frame.quadVertex->y = frame.scaledY1;
                frame.quadVertex->z = frame.scaledZ1;
                frame.quadVertex->rhw = frame.recipW1;
                frame.quadVertex++;
            }
            frame.clipState->vertexCursor += frame.clipState->cursorStep;
            frame.quadVertex = frame.clipState->vertices;
            for (frame.i = 0; frame.i < 3; frame.i++) {
                frame.srcIndex2 = *frame.indexPtr;
                frame.indexPtr++;
                frame.recipW2 = frame.clipRecipW[frame.srcIndex2];
                frame.quadVertex->color = frame.clipColors[frame.srcIndex2];
                frame.srcVecC = &frame.clippedVerts[0] + frame.srcIndex2;
                frame.scaledX2 = frame.srcVecC->x * frame.recipW2;
                frame.scaledY2 = frame.srcVecC->y * frame.recipW2;
                frame.scaledZ2 = frame.srcVecC->clipZ * frame.recipW2;
                frame.quadVertex->x = frame.scaledX2;
                frame.quadVertex->y = frame.scaledY2;
                frame.quadVertex->z = frame.scaledZ2;
                frame.quadVertex->rhw = frame.recipW2;
                frame.quadVertex++;
            }
            this->triangleDirty = 1;
        }
    }
}

/*
 * Copies the three vertices and three colours into local arrays and passes them
 * to emitTriangle.
 */
// FUNCTION: FF7 0x00734cab
void SnowboardClipper::emitVertices(int vertex0, int vertex1, int vertex2, int color0, int color1, int color2)
{
    struct {
        SnowboardClipInputVertex vertices[3];
        int colors[3];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.vertices[0] = *(SnowboardClipInputVertex *)vertex0;
    frame.vertices[1] = *(SnowboardClipInputVertex *)vertex1;
    frame.vertices[2] = *(SnowboardClipInputVertex *)vertex2;
    frame.colors[0] = color0;
    frame.colors[1] = color1;
    frame.colors[2] = color2;
    emitTriangle((int)&frame.vertices[0], (int)&frame.colors[0]);
}

/*
 * Draws a quad in colour color. Does nothing when all four vertices are behind
 * the plane clipZ = 0.5. When none is, projects all four into quadObject.
 * Otherwise splits it into the triangles (vertex0, vertex1, vertex2) and
 * (vertex1, vertex2, vertex3), clips each with clipPositions and projects the
 * result into triangleObject (three vertices) or quadObject (four).
 */
// FUNCTION: FF7 0x00734d2a
void SnowboardClipper::emitQuad(int vertex0, int vertex2, int vertex1, int vertex3, int color)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[220];
        GfxTlVertex *quadOutVertex;
        struct { unsigned char pad_to_4[0x4]; int quadSourceIndex; };
        struct { unsigned char pad_to_8[0x8]; GfxHomogeneousVec4 quadProjX; };
        struct { unsigned char pad_to_18[0x18]; int quadVertexIndex; };
        struct { unsigned char pad_to_1C[0x1C]; GfxTlVertex *triOutVertex; };
        struct { unsigned char pad_to_20[0x20]; int triSourceIndex; };
        struct { unsigned char pad_to_24[0x24]; GfxHomogeneousVec4 triProjX; };
        struct { unsigned char pad_to_34[0x34]; int triVertexIndex; };
        struct { unsigned char pad_to_38[0x38]; int clipVertexCount; };
        struct { unsigned char pad_to_3C[0x3C]; SnowboardClipInputVertex clippedVerts[4]; };
        struct { unsigned char pad_to_7C[0x7C]; int triIndex; };
        struct { unsigned char pad_to_80[0x80]; GfxTlVertex *wholeOutVertex; };
        struct { unsigned char pad_to_84[0x84]; GfxHomogeneousVec4 wholeProjX; };
        struct { unsigned char pad_to_94[0x94]; int wholeIndex; };
        struct { unsigned char pad_to_98[0x98]; SnowboardClipInputVertex quadVerts[4]; };
        struct { unsigned char pad_to_D8[0xD8]; int clipClass; };
    } frame;
    if (((SnowboardClipInputVertex *)vertex0)->clipZ < 0.5f && ((SnowboardClipInputVertex *)vertex2)->clipZ < 0.5f && ((SnowboardClipInputVertex *)vertex1)->clipZ < 0.5f && ((SnowboardClipInputVertex *)vertex3)->clipZ < 0.5f) {
        frame.clipClass = 0;
    } else if (((SnowboardClipInputVertex *)vertex0)->clipZ < 0.5f || (((SnowboardClipInputVertex *)vertex2)->clipZ < 0.5f || (((SnowboardClipInputVertex *)vertex1)->clipZ < 0.5f || ((SnowboardClipInputVertex *)vertex3)->clipZ < 0.5f))) {
        frame.clipClass = 1;
    } else {
        frame.clipClass = 2;
    }
    if (frame.clipClass == 0) {
        return;
    }
    frame.quadVerts[0].x = ((SnowboardClipInputVertex *)vertex0)->x;
    frame.quadVerts[0].y = ((SnowboardClipInputVertex *)vertex0)->y;
    frame.quadVerts[0].clipZ = ((SnowboardClipInputVertex *)vertex0)->clipZ;
    frame.quadVerts[0].w = ((SnowboardClipInputVertex *)vertex0)->w;
    frame.quadVerts[1].x = ((SnowboardClipInputVertex *)vertex1)->x;
    frame.quadVerts[1].y = ((SnowboardClipInputVertex *)vertex1)->y;
    frame.quadVerts[1].clipZ = ((SnowboardClipInputVertex *)vertex1)->clipZ;
    frame.quadVerts[1].w = ((SnowboardClipInputVertex *)vertex1)->w;
    frame.quadVerts[2].x = ((SnowboardClipInputVertex *)vertex2)->x;
    frame.quadVerts[2].y = ((SnowboardClipInputVertex *)vertex2)->y;
    frame.quadVerts[2].clipZ = ((SnowboardClipInputVertex *)vertex2)->clipZ;
    frame.quadVerts[2].w = ((SnowboardClipInputVertex *)vertex2)->w;
    frame.quadVerts[3].x = ((SnowboardClipInputVertex *)vertex3)->x;
    frame.quadVerts[3].y = ((SnowboardClipInputVertex *)vertex3)->y;
    frame.quadVerts[3].clipZ = ((SnowboardClipInputVertex *)vertex3)->clipZ;
    frame.quadVerts[3].w = ((SnowboardClipInputVertex *)vertex3)->w;
    if (frame.clipClass == 2) {
        if (AcquireBatchNode(1, this->quadObject) != 0) {
            for (frame.wholeIndex = 0; frame.wholeIndex < 4; frame.wholeIndex++) {
                project((int)&frame.quadVerts[frame.wholeIndex], (int)&frame.wholeProjX);
                frame.wholeOutVertex = this->quadObject->vertices + frame.wholeIndex;
                frame.wholeOutVertex->x = frame.wholeProjX.x;
                frame.wholeOutVertex->y = frame.wholeProjX.y;
                frame.wholeOutVertex->z = frame.wholeProjX.z;
                frame.wholeOutVertex->rhw = frame.wholeProjX.w;
                frame.wholeOutVertex->color = color;
            }
            this->quadDirty = 1;
        }
    } else {
        for (frame.triIndex = 0; frame.triIndex < 2; frame.triIndex++) {
            frame.clipVertexCount = clipPositions((int)&frame.quadVerts[frame.triIndex], (int)&frame.clippedVerts[0]);
            if (frame.clipVertexCount == 3) {
                if (AcquireBatchNode(1, this->triangleObject) != 0) {
                    for (frame.triVertexIndex = 0; frame.triVertexIndex < 3; frame.triVertexIndex++) {
                        frame.triSourceIndex = frame.triIndex != 0 ? 2 - frame.triVertexIndex : frame.triVertexIndex;
                        project((int)&frame.clippedVerts[frame.triVertexIndex], (int)&frame.triProjX);
                        frame.triOutVertex = this->triangleObject->vertices + frame.triSourceIndex;
                        frame.triOutVertex->x = frame.triProjX.x;
                        frame.triOutVertex->y = frame.triProjX.y;
                        frame.triOutVertex->z = frame.triProjX.z;
                        frame.triOutVertex->rhw = frame.triProjX.w;
                        frame.triOutVertex->color = color;
                    }
                }
                this->triangleDirty = 1;
            } else if (frame.clipVertexCount == 4) {
                if (AcquireBatchNode(1, this->quadObject) != 0) {
                    for (frame.quadVertexIndex = 0; frame.quadVertexIndex < 4; frame.quadVertexIndex++) {
                        frame.quadSourceIndex = g_SnoboQuadIndices[frame.triIndex][frame.quadVertexIndex];
                        project((int)&frame.clippedVerts[frame.quadSourceIndex], (int)&frame.quadProjX);
                        frame.quadOutVertex = this->quadObject->vertices + frame.quadVertexIndex;
                        frame.quadOutVertex->x = frame.quadProjX.x;
                        frame.quadOutVertex->y = frame.quadProjX.y;
                        frame.quadOutVertex->z = frame.quadProjX.z;
                        frame.quadOutVertex->rhw = frame.quadProjX.w;
                        frame.quadOutVertex->color = color;
                    }
                }
                this->quadDirty = 1;
            }
        }
    }
}

/*
 * For each of triangleDirty and quadDirty that is set, emits the corresponding
 * render object using ctx, invalidates its batch generation and clears that
 * flag.
 */
// FUNCTION: FF7 0x00735192
void SnowboardClipper::flush()
{
    if (this->triangleDirty != 0) {
        Gfx_EmitPrimitiveBatchList(this->triangleObject, this->ctx);
        InvalidateBatchGeneration(this->triangleObject);
        this->triangleDirty = 0;
    }
    if (this->quadDirty != 0) {
        Gfx_EmitPrimitiveBatchList(this->quadObject, this->ctx);
        InvalidateBatchGeneration(this->quadObject);
        this->quadDirty = 0;
    }
}

#pragma code_seg()

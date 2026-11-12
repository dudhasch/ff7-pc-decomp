// src/ff7/submarine/sub_object_emit_batches.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the submarine object's rotation from its angles and view matrix, emits its
//   triangle batch, and provides a fixed-pass draw hook.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/api/submarine.h"
#include "lib/api/graphics.h"
#include "ff7/types/submarine.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"

extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(strcpy, strcat)

// clang-format on
#define g_SubmarineRenderBatchYawAngle (*(short *)((char *)&g_SubmarineRenderBatchState + 2))

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

/*
 * Builds the submarine object rotation from X/Z/Y angles, combines it with the
 * installed view matrix, transforms the global translation and composes a
 * screen transform. For each packed triangle index, loads three short XYZ
 * vertices, transforms them and emits divided positions, reciprocal W and
 * packed per-vertex colors only when all W values are positive and acquisition
 * succeeds.
 */
// FUNCTION: FF7 0x0079fb20
void __cdecl Submarine_EmitObjectTriangleBatch(SubmarineAngleSet *angles)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[368];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; GfxTlVertex *outVertC; };
        struct { unsigned char pad_to_24[0x24]; float invWC; };
        struct { unsigned char pad_to_28[0x28]; GfxTlVertex *outVertB; };
        struct { unsigned char pad_to_2C[0x2C]; float invWB; };
        struct { unsigned char pad_to_30[0x30]; GfxTlVertex *outVertA; };
        struct { unsigned char pad_to_34[0x34]; float invWA; };
        struct { unsigned char pad_to_38[0x38]; int idxA; };
        struct { unsigned char pad_to_3C[0x3C]; int packedTri; };
        struct { unsigned char pad_to_40[0x40]; int idxC; };
        struct { unsigned char pad_to_44[0x44]; int idxB; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix *installedMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; GfxMatrix4x4 screenMatrix; };
        struct { unsigned char pad_to_8C[0x8C]; int *triIndexList; };
        struct { unsigned char pad_to_90[0x90]; int triIndex; };
        struct { unsigned char pad_to_94[0x94]; SubmarineEmitVertex *vertexArray; };
        struct { unsigned char pad_to_98[0x98]; PsxMatrix composedMatrix; };
        struct { unsigned char pad_to_B8[0xB8]; int rotTransOut; };
        struct { unsigned char pad_to_BC[0xBC]; unsigned int *colorCursor; };
        struct { unsigned char pad_to_C0[0xC0]; GfxHomogeneousVec4 projA; };
        struct { unsigned char pad_to_D0[0xD0]; GfxHomogeneousVec4 projB; };
        struct { unsigned char pad_to_E0[0xE0]; GfxHomogeneousVec4 projC; };
        struct { unsigned char pad_to_F0[0xF0]; int batchNode; };
        struct { unsigned char pad_to_F4[0xF4]; GfxMatrix4x4 floatMatrix; };
        struct { unsigned char pad_to_134[0x134]; GfxHomogeneousPoint srcA; };
        struct { unsigned char pad_to_140[0x140]; GfxHomogeneousPoint srcB; };
        struct { unsigned char pad_to_14C[0x14C]; GfxHomogeneousPoint srcC; };
        struct { unsigned char pad_to_158[0x158]; VECTOR trans; };
        struct { unsigned char pad_to_168[0x168]; int vertexColor; };
        struct { unsigned char pad_to_169[0x169]; char colorByte1; };
        struct { unsigned char pad_to_16A[0x16A]; char colorByte0; };
        struct { unsigned char pad_to_16B[0x16B]; char alphaByte; };
        struct { unsigned char pad_to_16C[0x16C]; int triangleCount; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.vertexArray = angles->vertexArray;
    frame.triIndexList = angles->triIndexList;
    frame.colorCursor = angles->colorCursor;
    frame.triangleCount = angles->triangleCount;
    frame.alphaByte = 0xFF;
    frame.rotMatrix.m[2][2] = 0x1000 /* 360 deg */;
    frame.rotMatrix.m[1][1] = frame.rotMatrix.m[2][2];
    frame.rotMatrix.m[0][0] = frame.rotMatrix.m[1][1];
    frame.rotMatrix.m[2][1] = 0;
    frame.rotMatrix.m[2][0] = frame.rotMatrix.m[2][1];
    frame.rotMatrix.m[1][2] = frame.rotMatrix.m[2][0];
    frame.rotMatrix.m[1][0] = frame.rotMatrix.m[1][2];
    frame.rotMatrix.m[0][2] = frame.rotMatrix.m[1][0];
    frame.rotMatrix.m[0][1] = frame.rotMatrix.m[0][2];
    RotateMatrixX(g_SubmarineRenderBatchState, &frame.rotMatrix);
    RotateMatrixZ(g_SubmarineTriangleBatchCount, &frame.rotMatrix);
    RotateMatrixY(g_SubmarineRenderBatchYawAngle, &frame.rotMatrix);
    SetRotMatrix(&g_SubmarineViewMatrix);
    ConcatMatrixPreservingTranslation(&g_SubmarineViewMatrix, &frame.rotMatrix, &frame.composedMatrix);
    SetTransMatrix(&g_SubmarineViewMatrix);
    RotTransPoint(&g_SubmarineTriangleColorBuffer, &frame.trans, &frame.rotTransOut);
    SetRotMatrix(&frame.composedMatrix);
    frame.installedMatrix = GetInstalledRotMatrixPtr();
    frame.installedMatrix->t[0] = frame.trans.vx;
    frame.installedMatrix->t[1] = frame.trans.vy;
    frame.installedMatrix->t[2] = frame.trans.vz;
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), &frame.floatMatrix);
    ComposeWithScreenMatrix(&frame.floatMatrix, &frame.screenMatrix);
    for (frame.triIndex = 0; frame.triIndex < frame.triangleCount; frame.triIndex++) {
        frame.packedTri = frame.triIndexList[frame.triIndex];
        frame.idxA = frame.packedTri & 0xFF;
        frame.idxB = frame.packedTri >> 8 & 0xFF;
        frame.idxC = frame.packedTri >> 0x10 & 0xFF;
        frame.batchNode = 0;
        frame.srcA.x = (float)frame.vertexArray[frame.idxA].x;
        frame.srcA.y = (float)frame.vertexArray[frame.idxA].y;
        frame.srcA.z = (float)frame.vertexArray[frame.idxA].z;
        frame.srcB.x = (float)frame.vertexArray[frame.idxB].x;
        frame.srcB.y = (float)frame.vertexArray[frame.idxB].y;
        frame.srcB.z = (float)frame.vertexArray[frame.idxB].z;
        frame.srcC.x = (float)frame.vertexArray[frame.idxC].x;
        frame.srcC.y = (float)frame.vertexArray[frame.idxC].y;
        frame.srcC.z = (float)frame.vertexArray[frame.idxC].z;
        TransformPointByMatrix4x4(&frame.screenMatrix, &frame.srcA, &frame.projA);
        if (frame.projA.w > 0.0) {
            TransformPointByMatrix4x4(&frame.screenMatrix, &frame.srcB, &frame.projB);
            if (frame.projB.w > 0.0) {
                TransformPointByMatrix4x4(&frame.screenMatrix, &frame.srcC, &frame.projC);
                if (frame.projC.w > 0.0) {
                    frame.batchNode = AcquireBatchNode(1, (void *)(((int)g_SubmarineFixedBatchList)));
                    if (frame.batchNode != 0) {
                        frame.colorByte0 = *frame.colorCursor << 24 >> 24;
                        frame.colorByte1 = *frame.colorCursor << 16 >> 24;
                        *(char *)&frame.vertexColor = *frame.colorCursor << 8 >> 24;
                        frame.colorCursor++;
                        frame.invWA = 1.0f / frame.projA.w;
                        frame.outVertA = (GfxTlVertex *)(g_SubmarineFixedBatchList->vertexCursor);
                        frame.outVertA->x = frame.projA.x * frame.invWA;
                        frame.outVertA->y = frame.projA.y * frame.invWA;
                        frame.outVertA->z = frame.projA.z * frame.invWA;
                        frame.outVertA->rhw = frame.invWA;
                        frame.outVertA->color = frame.vertexColor;
                        frame.colorByte0 = *frame.colorCursor << 24 >> 24;
                        frame.colorByte1 = *frame.colorCursor << 16 >> 24;
                        *(char *)&frame.vertexColor = *frame.colorCursor << 8 >> 24;
                        frame.colorCursor++;
                        frame.invWB = 1.0f / frame.projB.w;
                        frame.outVertB = (GfxTlVertex *)(g_SubmarineFixedBatchList->vertexCursor + 0x20);
                        frame.outVertB->x = frame.projB.x * frame.invWB;
                        frame.outVertB->y = frame.projB.y * frame.invWB;
                        frame.outVertB->z = frame.projB.z * frame.invWB;
                        frame.outVertB->rhw = frame.invWB;
                        frame.outVertB->color = frame.vertexColor;
                        frame.colorByte0 = *frame.colorCursor << 24 >> 24;
                        frame.colorByte1 = *frame.colorCursor << 16 >> 24;
                        *(char *)&frame.vertexColor = *frame.colorCursor << 8 >> 24;
                        frame.colorCursor++;
                        frame.invWC = 1.0f / frame.projC.w;
                        frame.outVertC = (GfxTlVertex *)(g_SubmarineFixedBatchList->vertexCursor + 0x40);
                        frame.outVertC->x = frame.projC.x * frame.invWC;
                        frame.outVertC->y = frame.projC.y * frame.invWC;
                        frame.outVertC->z = frame.projC.z * frame.invWC;
                        frame.outVertC->rhw = frame.invWC;
                        frame.outVertC->color = frame.vertexColor;
                    }
                }
            }
        }
        if (frame.batchNode == 0) {
            frame.colorCursor += 3;
        }
    }
}

/*
 * Draw hook for render context renderContext: selects scene pass 0 with
 * Gfx_SetScenePassCall and emits the primitive batch list held in
 * g_SubmarineFixedBatchList.
 */
// FUNCTION: FF7 0x007a0168
void __cdecl Submarine_DrawFixedBatchPass(struct GfxEngineContext *renderContext)
{
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)(renderContext));
    Gfx_EmitPrimitiveBatchList(g_SubmarineFixedBatchList, renderContext);
}
} /* extern "C" */

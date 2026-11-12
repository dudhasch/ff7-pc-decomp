// src/ff7/src/battle/battle_fx_draw_polygon_block.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Drawing helper for battle effects: draws a PSX polygon block and returns a fallback
//   value when the mesh is null.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"

#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
extern "C" {

/*
 * Returns fallbackValue for null mesh; otherwise refreshes transform state, installs four caller
 * state values and walks signed-count textured triangle/quad and gouraud triangle/quad streams in
 * sixteen/twenty/twenty/twenty-four-byte records. Vertex offsets are signed. Every stream
 * transforms first three and fetches following topology before filter, including final lookahead;
 * flags 0x60000 reject, descriptor bit 0x20 bypasses orientation, textured triangles require
 * positive orientation and others allow zero. Textured quads add successive eight-byte displacement
 * triples from the descriptor's displacement pointer to all four vertices and advance displacement pointer even
 * rejected records; other streams use raw vertices. Page masks the stream word with 0x19F then ORs the
 * descriptor's pageBits. Textured streams copy UV and use bit 0x80 record low-byte grey optionally shaded or
 * descriptor grey; textured triangle clear-bit branch instead uses greyRgb without an
 * initializing assignment in that branch, so it reads a prior or uninitialized value. Gouraud streams shade or copy three/four colours, with no explicit command reset
 * after shade. All accepted records query unused depth and OR command bit two under descriptor bit
 * eight, then submit shared primitive. Returns last stream render-state pointer even empty;
 * reserved/unusedArg3 unused.
 */
// FUNCTION: FF7 0x005d38d0
BattleFxPsxPolygonPrim *__cdecl BattleFx_DrawPsxPolygonBlock(BattleFxPsxPolygonBlock *mesh, int reserved, int unusedArg3, BattleFxPsxPolygonPrim *fallbackValue, GfxPsxSpriteQuadState *quadStateA, GfxPsxSpriteQuadState *quadStateB, GfxPsxSpriteQuadState *quadStateC, GfxPsxSpriteQuadState *quadStateD)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        int elemCount;
        struct { unsigned char pad_to_4[0x4]; BattleFxPolygonBlockPrim *texQuadPrim; };
        struct { unsigned char pad_to_8[0x8]; BattleFxPsxPolygonBlockPrim *texTriPrim; };
        struct { unsigned char pad_to_C[0xC]; int recordIndex; };
        struct { unsigned char pad_to_10[0x10]; BattlePsxPolygonBlock *streamPtr; };
        struct { unsigned char pad_to_14[0x14]; SVECTOR vertCX; };
        struct { unsigned char pad_to_1C[0x1C]; BattleFxPsxPolygonPrim *gouraudQuadPrim; };
        struct { unsigned char pad_to_20[0x20]; SVECTOR vertBX; };
        struct { unsigned char pad_to_28[0x28]; int rejectFlags; };
        struct { unsigned char pad_to_2C[0x2C]; int queryResult; };
        struct { unsigned char pad_to_30[0x30]; BattleFxPsxPolygonPacket *gouraudTriPrim; };
        struct { unsigned char pad_to_34[0x34]; int greyRgb; };
        struct { unsigned char pad_to_38[0x38]; Vec3sPacked *offsetPtr; };
        struct { unsigned char pad_to_3C[0x3C]; SVECTOR vertDX; };
        struct { unsigned char pad_to_44[0x44]; Vec3sPacked *pointD; };
        struct { unsigned char pad_to_48[0x48]; int recordCount; };
        struct { unsigned char pad_to_4C[0x4C]; Vec3sPacked *pointC; };
        struct { unsigned char pad_to_50[0x50]; char *dataBase; };
        struct { unsigned char pad_to_54[0x54]; short pageBits; };
        struct { unsigned char pad_to_58[0x58]; Vec3sPacked *pointB; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR vertAX; };
        struct { unsigned char pad_to_64[0x64]; Vec3sPacked *pointA; };
    } frame;
#pragma pack(pop)

    if (mesh->dataPtr != 0) {
        frame.offsetPtr = (Vec3sPacked *)mesh->offsetPtr;
        RefreshGlobalTransformState();
        Gfx_SetPsxPrimQuadState(9, 4, quadStateA);
        Gfx_SetPsxPrimQuadState(9, 1, quadStateB);
        Gfx_SetPsxPrimQuadState(0xB, 4, quadStateC);
        Gfx_SetPsxPrimQuadState(0xB, 1, quadStateD);
        frame.dataBase = (char *)(mesh->dataPtr + 4);
        frame.streamPtr = (BattlePsxPolygonBlock *)(frame.dataBase + *(int *)mesh->dataPtr);
        frame.texTriPrim = (BattleFxPsxPolygonBlockPrim *)Gfx_GetPsxPacketScratchPtr();
        frame.recordCount = frame.streamPtr->vertexOffsetA;
        frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 2);
        frame.pageBits = frame.streamPtr->vertexOffsetA & 0x19F | mesh->pageBits;
        frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 2);
        frame.pointA = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetA);
        frame.pointB = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetB);
        frame.pointC = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetC);
        for (frame.recordIndex = 0; frame.recordIndex < frame.recordCount; frame.recordIndex++) {
            StoreDeferredDrawArgs(frame.pointA, frame.pointB, frame.pointC);
            ApplyDeferredDrawArgs();
            frame.pointA = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetA);
            frame.pointB = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetB);
            frame.pointC = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetC);
            Gfx_PsxStoreClearFlag(&frame.rejectFlags);
            if ((frame.rejectFlags & 0x60000) == 0) {
                if ((mesh->flags & 0x20) != 0) {
                    frame.queryResult = 1;
                } else {
                    Gfx_PsxStoreFrontFacing(&frame.queryResult);
                }
                if (frame.queryResult > 0) {
                    frame.texTriPrim->tag = 0x7000000;
                    Gfx_ReadTransformOrderingDepth2(&frame.elemCount);
                    frame.texTriPrim->uv0Clut = frame.streamPtr->primWord;
                    frame.texTriPrim->uv1 = frame.streamPtr->attrA;
                    frame.texTriPrim->uv2 = frame.streamPtr->attrB;
                    frame.texTriPrim->bits = frame.pageBits;
                    if ((mesh->flags & 0x80) != 0) {
                        frame.greyRgb = frame.streamPtr->vertexOffsetD & 0xFF;
                        frame.greyRgb = frame.greyRgb << 0x10 | 0x24000000 | frame.greyRgb << 8 | frame.greyRgb;
                        if (mesh->shade != 0) {
                            Gfx_ShadePsxVertexColor((CVECTOR *)&frame.greyRgb, mesh->shade, (CVECTOR *)&frame.texTriPrim->color);
                            frame.texTriPrim->gpuCode = 0x24;
                        } else {
                            frame.texTriPrim->color = frame.greyRgb;
                        }
                    } else {
                        frame.texTriPrim->color = frame.greyRgb << 0x10 | 0x24000000 | frame.greyRgb << 8 | frame.greyRgb;
                    }
                    if ((mesh->flags & 8) != 0) {
                        frame.texTriPrim->gpuCode |= 2;
                    }
                    Gfx_DrawPsxPrimitive(frame.texTriPrim);
                }
            }
            frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 0x10);
        }
        frame.recordCount = frame.streamPtr->vertexOffsetA;
        frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 4);
        frame.texQuadPrim = (BattleFxPolygonBlockPrim *)Gfx_GetPsxPacketScratchPtr();
        frame.pointA = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetA);
        frame.pointB = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetB);
        frame.pointC = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetC);
        for (frame.recordIndex = 0; frame.recordIndex < frame.recordCount; frame.recordIndex++) {
            frame.vertAX.vx = frame.pointA->vx + frame.offsetPtr->vx;
            frame.vertAX.vy = frame.pointA->vy + frame.offsetPtr->vy;
            frame.vertAX.vz = frame.pointA->vz + frame.offsetPtr->vz;
            frame.vertBX.vx = frame.pointB->vx + frame.offsetPtr->vx;
            frame.vertBX.vy = frame.pointB->vy + frame.offsetPtr->vy;
            frame.vertBX.vz = frame.pointB->vz + frame.offsetPtr->vz;
            frame.vertCX.vx = frame.pointC->vx + frame.offsetPtr->vx;
            frame.vertCX.vy = frame.pointC->vy + frame.offsetPtr->vy;
            frame.vertCX.vz = frame.pointC->vz + frame.offsetPtr->vz;
            StoreDeferredDrawArgs(&frame.vertAX, &frame.vertBX, &frame.vertCX);
            ApplyDeferredDrawArgs();
            frame.pointA = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetC);
            frame.pointB = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetD);
            frame.pointC = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetE);
            Gfx_PsxStoreClearFlag(&frame.rejectFlags);
            if ((frame.rejectFlags & 0x60000) == 0) {
                if ((mesh->flags & 0x20) != 0) {
                    frame.queryResult = 1;
                } else {
                    Gfx_PsxStoreFrontFacing(&frame.queryResult);
                }
                if (frame.queryResult >= 0) {
                    frame.pointD = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetD);
                    frame.vertDX.vx = frame.pointD->vx + frame.offsetPtr->vx;
                    frame.vertDX.vy = frame.pointD->vy + frame.offsetPtr->vy;
                    frame.vertDX.vz = frame.pointD->vz + frame.offsetPtr->vz;
                    SetPendingResourceHandle((GfxPsxShortVertexSlot *)&frame.vertDX);
                    ApplyPendingResourceHandle();
                    Gfx_ReadTransformOrderingDepth2(&frame.elemCount);
                    frame.texQuadPrim->tag = 0x9000000;
                    frame.texQuadPrim->uv0Clut = frame.streamPtr->primWord;
                    frame.texQuadPrim->uv1 = frame.streamPtr->attrA;
                    frame.texQuadPrim->uv2 = frame.streamPtr->attrB;
                    frame.texQuadPrim->uv3 = frame.streamPtr->pointOffsetA;
                    frame.texQuadPrim->tpage = frame.pageBits;
                    if ((mesh->flags & 0x80) != 0) {
                        frame.greyRgb = frame.streamPtr->pointOffsetB & 0xFF;
                        frame.greyRgb = frame.greyRgb << 0x10 | 0x2C000000 | frame.greyRgb << 8 | frame.greyRgb;
                        if (mesh->shade != 0) {
                            Gfx_ShadePsxVertexColor((CVECTOR *)&frame.greyRgb, mesh->shade, (CVECTOR *)&frame.texQuadPrim->colorCmd);
                            frame.texQuadPrim->gpuCode = 0x2C;
                        } else {
                            frame.texQuadPrim->colorCmd = frame.greyRgb;
                        }
                    } else {
                        frame.greyRgb = mesh->shade & 0xFF;
                        frame.texQuadPrim->colorCmd = frame.greyRgb << 0x10 | 0x2C000000 | frame.greyRgb << 8 | frame.greyRgb;
                    }
                    if ((mesh->flags & 8) != 0) {
                        frame.texQuadPrim->gpuCode |= 2;
                    }
                    Gfx_DrawPsxPrimitive(frame.texQuadPrim);
                }
            }
            frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 0x14);
            frame.offsetPtr = (Vec3sPacked *)((char *)frame.offsetPtr + 8);
        }
        frame.recordCount = frame.streamPtr->vertexOffsetA;
        frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 4);
        frame.gouraudTriPrim = (BattleFxPsxPolygonPacket *)Gfx_GetPsxPacketScratchPtr();
        frame.pointA = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetA);
        frame.pointB = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetB);
        frame.pointC = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetC);
        for (frame.recordIndex = 0; frame.recordIndex < frame.recordCount; frame.recordIndex++) {
            StoreDeferredDrawArgs(frame.pointA, frame.pointB, frame.pointC);
            ApplyDeferredDrawArgs();
            frame.pointA = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetC);
            frame.pointB = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetD);
            frame.pointC = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetE);
            Gfx_PsxStoreClearFlag(&frame.rejectFlags);
            if ((frame.rejectFlags & 0x60000) == 0) {
                if ((mesh->flags & 0x20) != 0) {
                    frame.queryResult = 1;
                } else {
                    Gfx_PsxStoreFrontFacing(&frame.queryResult);
                }
                if (frame.queryResult >= 0) {
                    frame.gouraudTriPrim->tag = 0x6000000;
                    Gfx_ReadTransformOrderingDepth2(&frame.elemCount);
                    if (mesh->shade != 0) {
                        Gfx_ShadePsxTriVertexColors((CVECTOR *)&frame.streamPtr->primWord, (CVECTOR *)&frame.streamPtr->color1, (CVECTOR *)&frame.streamPtr->color2, mesh->shade, (CVECTOR *)&frame.gouraudTriPrim->colorCmd, (CVECTOR *)&frame.gouraudTriPrim->color1, (CVECTOR *)&frame.gouraudTriPrim->color2);
                    } else {
                        frame.gouraudTriPrim->colorCmd = frame.streamPtr->primWord;
                        frame.gouraudTriPrim->color1 = frame.streamPtr->color1;
                        frame.gouraudTriPrim->color2 = frame.streamPtr->color2;
                    }
                    if ((mesh->flags & 8) != 0) {
                        frame.gouraudTriPrim->gpuCode |= 2;
                    }
                    Gfx_DrawPsxPrimitive(frame.gouraudTriPrim);
                }
            }
            frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 0x14);
        }
        frame.recordCount = frame.streamPtr->vertexOffsetA;
        frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 4);
        frame.gouraudQuadPrim = (BattleFxPsxPolygonPrim *)Gfx_GetPsxPacketScratchPtr();
        frame.pointA = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetA);
        frame.pointB = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetB);
        frame.pointC = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->vertexOffsetC);
        for (frame.recordIndex = 0; frame.recordIndex < frame.recordCount; frame.recordIndex++) {
            StoreDeferredDrawArgs(frame.pointA, frame.pointB, frame.pointC);
            ApplyDeferredDrawArgs();
            frame.pointA = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->pointOffsetE);
            frame.pointB = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->nextVertexOffsetB);
            frame.pointC = (Vec3sPacked *)(frame.dataBase + frame.streamPtr->nextVertexOffsetC);
            Gfx_PsxStoreClearFlag(&frame.rejectFlags);
            if ((frame.rejectFlags & 0x60000) == 0) {
                if ((mesh->flags & 0x20) != 0) {
                    frame.queryResult = 1;
                } else {
                    Gfx_PsxStoreFrontFacing(&frame.queryResult);
                }
                if (frame.queryResult >= 0) {
                    SetPendingResourceHandle((GfxPsxShortVertexSlot *)(frame.dataBase + frame.streamPtr->vertexOffsetD));
                    ApplyPendingResourceHandle();
                    Gfx_ReadTransformOrderingDepth2(&frame.elemCount);
                    frame.gouraudQuadPrim->tag = 0x8000000;
                    if (mesh->shade != 0) {
                        Gfx_ShadePsxTriVertexColors((CVECTOR *)&frame.streamPtr->primWord, (CVECTOR *)&frame.streamPtr->color1, (CVECTOR *)&frame.streamPtr->color2, mesh->shade, (CVECTOR *)&frame.gouraudQuadPrim->colorCmd, (CVECTOR *)&frame.gouraudQuadPrim->color1, (CVECTOR *)&frame.gouraudQuadPrim->color2);
                        Gfx_ShadePsxVertexColor((CVECTOR *)&frame.streamPtr->color3, mesh->shade, (CVECTOR *)&frame.gouraudQuadPrim->color3);
                    } else {
                        frame.gouraudQuadPrim->colorCmd = frame.streamPtr->primWord;
                        frame.gouraudQuadPrim->color1 = frame.streamPtr->color1;
                        frame.gouraudQuadPrim->color2 = frame.streamPtr->color2;
                        frame.gouraudQuadPrim->color3 = frame.streamPtr->color3;
                    }
                    if ((mesh->flags & 8) != 0) {
                        frame.gouraudQuadPrim->gpuCode |= 2;
                    }
                    Gfx_DrawPsxPrimitive(frame.gouraudQuadPrim);
                }
            }
            frame.streamPtr = (BattlePsxPolygonBlock *)((char *)frame.streamPtr + 0x18);
        }
        return frame.gouraudQuadPrim;
    } else {
        return fallbackValue;
    }
}
} /* extern "C" */

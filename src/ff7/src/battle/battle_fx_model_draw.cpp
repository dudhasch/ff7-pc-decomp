// src/ff7/src/battle/battle_fx_model_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Shared battle effect plumbing: finds model groups, latches the tint, blend, scalar and
//   texture overrides for the next draw, and submits model, sprite and sprite-quad draws to the
//   battle renderer.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/battle.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00c06a00
int g_BattleFxDrawTintR = 0;
// GLOBAL: FF7 0x00c06a04
struct BattleFxModelGroupEntry *g_BattleFxDrawTextureOverrideHandle = 0;
// GLOBAL: FF7 0x00c06a08
int g_BattleFxDrawBlendOverrideValue = 0;
// GLOBAL: FF7 0x00c06a0c
int g_BattleFxDrawTintG = 0;
// GLOBAL: FF7 0x00c06a10
int g_BattleFxDrawScalarOverrideMode = 0;
// GLOBAL: FF7 0x00c06a14
int g_BattleFxDrawScalarOverrideValue = 0;
// GLOBAL: FF7 0x00c06a18
int g_BattleFxDrawTintB = 0;
// GLOBAL: FF7 0x00c06a1c
int g_BattleFxDrawTintActive = 0;
// GLOBAL: FF7 0x00c06a20
int g_BattleFxDrawBlendOverrideActive = 0;
// GLOBAL: FF7 0x00c06a24
int g_BattleFxDrawBlendOverrideMode = 0;
// GLOBAL: FF7 0x00c06a28
int g_BattleFxDrawTextureOverrideActive = 0;
unsigned char g_bssPad_00c06a2c[4] = {0};
// clang-format on

/*
 * Scans model groups for the first one with a model and a matching key and returns that
 * model's draw source, else zero.
 */
// FUNCTION: FF7 0x005d1a30
unsigned int __cdecl BattleFx_GetModelGroupHandleUnused(int key, BattleFxModelGroupTable *groupTable)
{
    BattleFxModelGroupEntry *group;
    int res;
    int nonNullCnt;
    int index;

    res = 0;
    nonNullCnt = 0;
    group = groupTable->group;
    for (index = 0; index < groupTable->groupCount; index++) {
        if (group->model != 0) {
            if (group->key == key) {
                res = (int)group->model->drawSource;
                break;
            }
            nonNullCnt++;
        }
        group++;
    }
    return res;
}

/*
 * Scans the counted model groups and returns the first with a model and a matching key.
 */
// FUNCTION: FF7 0x005d1aaa
BattleFxModelGroupEntry *__cdecl BattleFx_FindModelGroupByKey(int key, BattleFxModelGroupContainer *container)
{
    BattleFxModelGroupEntry *result;
    BattleFxModelGroupEntry *group;
    int validCnt;
    int index;

    validCnt = 0;
    result = 0;
    group = container->group;
    for (index = 0; index < container->groupCount; index++) {
        if (group->model != 0) {
            if (group->key == key) {
                result = group;
                break;
            }
            validCnt++;
        }
        group++;
    }
    return result;
}

/*
 * Latches an RGB tint for the next battle effect draw and raises the tint-set flag.
 */
// FUNCTION: FF7 0x005d1b1b
void __cdecl BattleFx_LatchDrawTintRgb(int r, int g, int b)
{
    /* Plain dword stores, not the FPU: these are ints however the latch
       was named. g_BattleFxDrawTintActive is the "tint is set" flag. */
    g_BattleFxDrawTintR = r;
    g_BattleFxDrawTintG = g;
    g_BattleFxDrawTintB = b;
    g_BattleFxDrawTintActive = 1;
}

/*
 * Latches a blend-mode override for the next battle effect draw and raises its active flag.
 */
// FUNCTION: FF7 0x005d1b44
void __cdecl BattleFx_LatchDrawBlendOverride(int blendMode, int value)
{
    g_BattleFxDrawBlendOverrideMode = blendMode;
    g_BattleFxDrawBlendOverrideValue = value;
    g_BattleFxDrawBlendOverrideActive = 1;
}

/*
 * Latches a scalar override mode and value for the next battle effect draw.
 */
// FUNCTION: FF7 0x005d1b64
void __cdecl BattleFx_LatchDrawScalarOverride(int mode, int scalar)
{
    g_BattleFxDrawScalarOverrideMode = mode;
    g_BattleFxDrawScalarOverrideValue = scalar;
}

/*
 * Latches a texture override handle for the next battle effect draw and raises its active flag.
 */
// FUNCTION: FF7 0x005d1b7a
void __cdecl BattleFx_LatchDrawTextureOverride(int *pTextureRecord)
{
    if (pTextureRecord != 0 && pTextureRecord[3] != 0) {
        g_BattleFxDrawTextureOverrideHandle = (BattleFxModelGroupEntry *)pTextureRecord[3];
        g_BattleFxDrawTextureOverrideActive = 1;
    }
}

/*
 * Submits a 3D effect model draw request to the battle renderer, applying pending tint,
 * blend, scalar, and texture overrides.
 */
// FUNCTION: FF7 0x005d1ba4
int __cdecl BattleFx_SubmitModelDraw(void *modelDraw, int listHandle, int drawMode, int listHead)
{
    GfxEngineContext *auxContext;
    BattleFxModelGroupContainer *model;
    int rotMatrix;
    int alphaVal;
    float weightA;
    int colorRgb8;
    BattleFxModelGroupEntry *textureHandle;
    BattleFxModelDrawSubmit *pSubmit;
    float scalarB;
    int packedColor;
    BattleFxModelDrawFloats *pMatrixPtr;
    BattleFxModelGroupHandle *modelGroup;
    BattleFxModelDrawNode *drawNode;
    int variant;
    int drawColor;
    int zeroKey;

    if (Gfx_IsDrawEnabled() == 0) {
        g_BattleFxDrawTintActive = 0;
        g_BattleFxDrawBlendOverrideActive = 0;
        g_BattleFxDrawScalarOverrideMode = 0;
        g_BattleFxDrawTextureOverrideActive = 0;
        return listHead;
    } else {
        modelGroup = (BattleFxModelGroupHandle *)((BattleFxModelDraw *)modelDraw)->modelGroup;
        if (modelGroup != 0 && modelGroup->container != 0) {
            rotMatrix = (int)GetInstalledRotMatrixPtr();
            auxContext = GetEngineContext();
            model = modelGroup->container;
            drawNode = (BattleFxModelDrawNode *)(Gfx_AllocDrawPoolListNode(&model->drawPoolList, model->drawPoolDesc));
            if (drawNode != 0) {
                drawNode->nodeKind = 1;
                zeroKey = 0;
                textureHandle = BattleFx_FindModelGroupByKey(zeroKey, model);
                pSubmit = &drawNode->submit;
                if ((((BattleFxModelDraw *)modelDraw)->flags & BATTLE_FX_DRAW_FADE_TRANSPARENCY) != 0) {
                    alphaVal = Battle3d_FadeLevelToAlpha(((BattleFxModelDraw *)modelDraw)->fadeLevel);
                } else if (((BattleFxModelDraw *)modelDraw)->fadeLevel < 0x80) {
                    alphaVal = ((BattleFxModelDraw *)modelDraw)->fadeLevel << 1;
                } else {
                    alphaVal = 0xFF;
                }
                packedColor = GetDrawColorPacked();
                drawColor = packedColor;
                if (auxContext->driverType == GFX_DRIVER_SOFTWARE) {
                    colorRgb8 = drawColor & 0xFFFFFF;
                    if ((((BattleFxModelDraw *)modelDraw)->flags & BATTLE_FX_DRAW_SW_BLEND_COLOR) != 0) {
                        if (colorRgb8 != 0) {
                            Gfx_InitDrawSpec(GFX_DRAW_SPEC_KIND_BLEND_SAVED_OR_PALETTE, (GfxDrawSpec *)pSubmit);
                        } else {
                            Gfx_InitDrawSpec(GFX_DRAW_SPEC_KIND_SCALE_SAVED_OR_PALETTE, (GfxDrawSpec *)pSubmit);
                        }
                    } else if (colorRgb8 != 0) {
                        if (alphaVal == 0xFF) {
                            Gfx_InitDrawSpec(GFX_DRAW_SPEC_KIND_SCALE_SAVED_OR_PALETTE, (GfxDrawSpec *)pSubmit);
                        } else {
                            Gfx_InitDrawSpec(GFX_DRAW_SPEC_KIND_MODULATE_SAVED_OR_PALETTE, (GfxDrawSpec *)pSubmit);
                        }
                    } else {
                        Gfx_InitDrawSpec(GFX_DRAW_SPEC_KIND_SCALE_SAVED_OR_PALETTE, (GfxDrawSpec *)pSubmit);
                    }
                    pSubmit->drawColor = drawColor;
                    ((char *)&pSubmit->drawColor)[3] = alphaVal;
                } else {
                    if (drawColor == 0xFF000000) {
                        Gfx_InitDrawSpec(GFX_DRAW_SPEC_KIND_SCALE_SAVED, (GfxDrawSpec *)pSubmit);
                    } else {
                        Gfx_InitDrawSpec(GFX_DRAW_SPEC_KIND_BLEND_SAVED, (GfxDrawSpec *)pSubmit);
                    }
                    pSubmit->drawColor = drawColor;
                    ((char *)&pSubmit->drawColor)[3] = alphaVal;
                }
                variant = ((BattleFxModelDraw *)modelDraw)->stateFlags & 0xFFFFFFDF;
                if (variant != 0 && variant < 2) {
                    pSubmit->hasTextureOverride = 1;
                    textureHandle = model->textureHandles[variant];
                }
                if (g_BattleFxDrawTextureOverrideActive != 0) {
                    pSubmit->hasTextureOverride = 1;
                    textureHandle = g_BattleFxDrawTextureOverrideHandle;
                    g_BattleFxDrawTextureOverrideActive = 0;
                }
                if (((BattleFxModelDraw *)modelDraw)->uvScroll != 0) {
                    pSubmit->submitMode = 2;
                    weightA = (float)(*(unsigned short *)&((BattleFxModelDraw *)modelDraw)->uvScroll & 0xFF);
                    scalarB = (float)(*(unsigned short *)&((BattleFxModelDraw *)modelDraw)->uvScroll >> 8);
                    pSubmit->scrollU = weightA * 0.00390625f;
                    pSubmit->scrollV = scalarB * 0.00390625f;
                } else {
                    pSubmit->submitMode = 1;
                }
                if (((BattleFxModelDraw *)modelDraw)->texturePageBits != 0) {
                    pSubmit->isActive = 1;
                    pSubmit->texturePage = ((BattleFxModelDraw *)modelDraw)->texturePageBits >> 6;
                } else {
                    pSubmit->isActive = 0;
                    pSubmit->texturePage = 0;
                }
                if (g_BattleFxDrawTintActive != 0) {
                    pSubmit->hasTint = 1;
                    pSubmit->tintR = g_BattleFxDrawTintR;
                    pSubmit->tintG = g_BattleFxDrawTintG;
                    pSubmit->tintB = g_BattleFxDrawTintB;
                    g_BattleFxDrawTintActive = 0;
                }
                if (g_BattleFxDrawBlendOverrideActive != 0) {
                    pSubmit->blendOverrideMode = *(char *)&g_BattleFxDrawBlendOverrideMode;
                    pSubmit->blendOverrideValue = g_BattleFxDrawBlendOverrideValue;
                    g_BattleFxDrawBlendOverrideActive = 0;
                }
                if (g_BattleFxDrawScalarOverrideMode != 0) {
                    pSubmit->scalarOverrideMode = *(char *)&g_BattleFxDrawScalarOverrideMode;
                    *(int *)&pSubmit->scrollV = g_BattleFxDrawScalarOverrideValue;
                    g_BattleFxDrawScalarOverrideMode = 0;
                }
                pSubmit->textureHandle = (int)textureHandle;
                GtePsxMatrixToFloat4x4Transposed((void *)rotMatrix, &drawNode->matrix);
                if ((((BattleFxModelDraw *)modelDraw)->flags & (BATTLE_FX_DRAW_MIRROR_X | BATTLE_FX_DRAW_MIRROR_Y | BATTLE_FX_DRAW_MIRROR_Z)) != 0) {
                    pMatrixPtr = &drawNode->matrix;
                    if ((((BattleFxModelDraw *)modelDraw)->flags & BATTLE_FX_DRAW_MIRROR_X) != 0) {
                        pMatrixPtr->m00 = -pMatrixPtr->m00;
                        pMatrixPtr->m01 = -pMatrixPtr->m01;
                        pMatrixPtr->m02 = -pMatrixPtr->m02;
                    }
                    if ((((BattleFxModelDraw *)modelDraw)->flags & BATTLE_FX_DRAW_MIRROR_Y) != 0) {
                        pMatrixPtr->m10 = -pMatrixPtr->m10;
                        pMatrixPtr->m11 = -pMatrixPtr->m11;
                        pMatrixPtr->m12 = -pMatrixPtr->m12;
                    }
                    if ((((BattleFxModelDraw *)modelDraw)->flags & BATTLE_FX_DRAW_MIRROR_Z) != 0) {
                        pMatrixPtr->m20 = -pMatrixPtr->m20;
                        pMatrixPtr->m21 = -pMatrixPtr->m21;
                        pMatrixPtr->m22 = -pMatrixPtr->m22;
                    }
                }
            }
        }
        return listHead;
    }
}

/*
 * Submits a 2D effect sprite draw request to the battle renderer, setting up packet vertices,
 * UV coordinates, and color modulation.
 */
// FUNCTION: FF7 0x005d1ffb
int __cdecl BattleFx_SubmitSpriteDraw(void *pSpriteReq, int otBucket, int otShift, int packetCursor)
{
    struct SpriteColour {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char cd;
    };
    GfxEngineContext *dev;
    int mat;
    GfxRenderObject *obj0;
    BattleFxSpriteGroup *grp;
    float vbot8;
    int j4;
    int rest;
    BattleFxSpriteDrawRow *row;
    BattleFxSpriteEntry *auxEnt;
    int turn;
    int ix;
    GfxDrawPoolNode *pk;
    float fu;
    float du7;
    float y8;
    float wpx1;
    float dv3;
    float hm;
    int cr;
    float fv;
    float fx;
    float wm1;
    int g;
    float ura;
    struct SpriteColour clr;
    int zero;
    float tvt;
    GfxBatchPrimitive *rawVbuf;
    float hf;
    int i2;
    float tul0;
    int blu;
    int tv;
    int ub;
    int zz4;
    BattleFxSpriteDrawSource *dev2;

    if (Gfx_IsDrawEnabled() == 0) {
        return packetCursor;
    }
    mat = (int)GetInstalledRotMatrixPtr();
    grp = (BattleFxSpriteGroup *)((BattleFxSpriteRequest *)pSpriteReq)->spriteGroup;
    obj0 = (GfxRenderObject *)(((BattleFxSpriteRequest *)pSpriteReq)->drawObj);
    if (grp != 0 && obj0 != 0 && obj0->hundredIndex != 0 &&
        ((BattleFxModelGroupEntry *)obj0->hundredIndex)->model != 0) {
        ix = ((BattleFxSpriteRequest *)pSpriteReq)->frame;
        turn = 0;
        if ((ix & 0x8000) != 0) {
            turn = 1;
        }
        ix &= ~0x8000;
        if (ix >= grp->header->frameCount) {
            ix = grp->header->frameCount - 1;
        }
        if (ix >= 0 && ix < grp->header->frameCount) {
            dev = GetEngineContext();
            auxEnt = grp->entries + ix;
            row = auxEnt->parts;
            for (rest = auxEnt->head->partCount; rest > 0; rest--) {
                if (grp->entryCount > 1) {
                    obj0 = 0;
                    for (j4 = 0; grp->entryCount != 0; j4++) {
                        if (row->textureKey == grp->textureKeys[j4]) {
                            obj0 = grp->renderObjects[j4];
                            break;
                        }
                    }
                }
                pk = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive((int)obj0);
                if (pk != 0) {
                    zero = 0;
                    cr = *(unsigned char *)&((BattleFxSpriteRequest *)pSpriteReq)->colorR;
                    if (cr >= 0x80) {
                        cr = 0xff;
                    } else {
                        cr <<= 1;
                    }
                    g = *(unsigned char *)&((BattleFxSpriteRequest *)pSpriteReq)->colorG;
                    if (g >= 0x80) {
                        g = 0xff;
                    } else {
                        g <<= 1;
                    }
                    blu = *(unsigned char *)&((BattleFxSpriteRequest *)pSpriteReq)->colorB;
                    if (blu >= 0x80) {
                        blu = 0xff;
                    } else {
                        blu <<= 1;
                    }
                    clr.r = (unsigned char)cr;
                    clr.g = (unsigned char)g;
                    clr.b = (unsigned char)blu;
                    clr.cd = *(unsigned char *)&((PolygonHundredState *)((GfxPolygonSet *)obj0->polygonSet)->groupState)->alphaLevel;
                    fx = (float)(row->posX << 3);
                    y8 = (float)(row->posY << 3);
                    wpx1 = (float)((row->widthUnits & 0xff) << 3);
                    hf = (float)((row->heightUnits & 0xff) << 3);
                    fu = row->texCol * obj0->invWidth +
                         obj0->invWidth / 2.0f;
                    fv = row->texRow * obj0->invHeight +
                         obj0->invHeight / 2.0f;
                    wm1 = (float)((row->widthUnits & 0xff) - 1);
                    hm = (float)((row->heightUnits & 0xff) - 1);
                    du7 = obj0->invWidth * wm1;
                    dv3 = obj0->invHeight * hm;
                    i2 = 0;
                    rawVbuf = (GfxBatchPrimitive *)pk->batchBlock;
                    if ((row->drawFlags & 1) != 0) {
                        tul0 = fu;
                        ura = fu + du7;
                    } else {
                        ura = fu;
                        tul0 = fu + du7;
                    }
                    if ((row->drawFlags & 2) != 0) {
                        vbot8 = fv;
                        tvt = fv + dv3;
                    } else {
                        tvt = fv;
                        vbot8 = fv + dv3;
                    }
                    rawVbuf->positions[i2].x = fx;
                    rawVbuf->positions[i2].y = y8;
                    *(int *)&rawVbuf->positions[i2].z = zero;
                    rawVbuf->colors[i2] = *(int *)&clr;
                    rawVbuf->texCoords[i2].u = ura;
                    rawVbuf->texCoords[i2].v = tvt;
                    i2++;
                    rawVbuf->positions[i2].x = fx;
                    rawVbuf->positions[i2].y = y8 + hf;
                    *(int *)&rawVbuf->positions[i2].z = zero;
                    rawVbuf->colors[i2] = *(int *)&clr;
                    rawVbuf->texCoords[i2].u = ura;
                    rawVbuf->texCoords[i2].v = vbot8;
                    i2++;
                    rawVbuf->positions[i2].x = fx + wpx1;
                    rawVbuf->positions[i2].y = y8;
                    *(int *)&rawVbuf->positions[i2].z = zero;
                    rawVbuf->colors[i2] = *(int *)&clr;
                    rawVbuf->texCoords[i2].u = tul0;
                    rawVbuf->texCoords[i2].v = tvt;
                    i2++;
                    rawVbuf->positions[i2].x = fx + wpx1;
                    rawVbuf->positions[i2].y = y8 + hf;
                    *(int *)&rawVbuf->positions[i2].z = zero;
                    rawVbuf->colors[i2] = *(int *)&clr;
                    rawVbuf->texCoords[i2].u = tul0;
                    rawVbuf->texCoords[i2].v = vbot8;
                    if (turn != 0) {
                        rawVbuf->clutRow =
                            ((BattleFxSpriteRequest *)pSpriteReq)->uvPacked >> 6;
                    } else {
                        dev2 = ((BattleFxModelGroupEntry *)obj0->hundredIndex)->model->drawSource;
                        ub = (row->uvPacked & 0x3f) << 4;
                        tv = row->uvPacked >> 6;
                        // Two empty conditions preserve VC5's measured suffix register phase.
                        if (REG_ALLOC_HINT) {
                        }
                        if (REG_ALLOC_HINT) {
                        }
                        zz4 = tv - dev2->clutRowBase;
                        if (zz4 < 0) {
                            zz4 = 0;
                        }
                        rawVbuf->clutRow = zz4;
                    }
                    GtePsxMatrixToFloat4x4Transposed((void *)mat, (char *)pk + 0xc);
                }
                row++;
            }
        }
    }
    return packetCursor;
}

/*
 * Builds four sprite quad vertices in the selected packet, including colour, UVs, and
 * the installed transformation matrix. The draw source's mode is the texture's bits per pixel
 * (GfxTexHeader::bitsPerPixel at +0x38, with clutY at +0xE0): for a 4-bit texture it splits a
 * two-bit field out of flag bits 5-6, for an 8-bit one out of bits 7-8; nothing reads either.
 */
// FUNCTION: FF7 0x005d25c2
int __cdecl BattleFx_SubmitSpriteQuadDraw(BattleFxSpriteQuadRequest *pQuadReq, int otRoot, int otShift, int chainTail)
{
    struct QuadColour {
        unsigned char b, g, r, command;
    };
    float v120l;
    GfxBatchPrimitive *v116d;
    float v112d;
    int v108l;
    float v104l;
    int v100m;
    float v096c;
    struct QuadColour v092d;
    int v088k;
    int v084l;
    float v080l;
    float v076d;
    int v072d;
    float v068l;
    float v064l;
    float v060l;
    float v056d;
    float v052e;
    float v048l;
    float v044m;
    float v040m;
    BattleFxSpriteDrawSource *v036i;
    int v032i;
    int v028a;
    int v024a;
    int v020a;
    GfxRenderObject *v016i;
    GfxDrawPoolNode *v012j;
    int v008a;
    int v004b;

    if (Gfx_IsDrawEnabled() == 0) {
        return chainTail;
    } else {
        v016i = (GfxRenderObject *)pQuadReq->quadState;
        if (v016i != 0) {
            v036i = ((BattleFxModelGroupEntry *)v016i->hundredIndex)->model->drawSource;
            switch (v036i->mode) {
            case GFX_PSX_CLUT_DEPTH_4BPP:
                v008a = (pQuadReq->flags & 0x60) >> 5;
                v004b = pQuadReq->flags & 0xFFFFFF9F;
                break;
            case GFX_PSX_CLUT_DEPTH_8BPP:
                v008a = (pQuadReq->flags & 0x180) >> 7;
                v004b = pQuadReq->flags & 0xFFFFFE7F;
            }
            v028a = (pQuadReq->cellWord & 0x3F) << 4;
            v032i = pQuadReq->cellWord >> 6;
            // Same folded-condition register phase as the verified neighboring sprite path.
            if (REG_ALLOC_HINT) {
            }
            if (REG_ALLOC_HINT) {
            }
            v020a = v032i - v036i->clutRowBase;
            if (v020a < 0) {
                v020a = 0;
            }
            v024a = (int)GetInstalledRotMatrixPtr();
            v012j = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive((int)v016i);
            if (v012j != 0) {
                v088k = 0;
                v072d = pQuadReq->colorR;
                if (v072d >= 0x80) {
                    v072d = 0xFF;
                } else {
                    v072d <<= 1;
                }
                v084l = pQuadReq->colorG;
                if (v084l >= 0x80) {
                    v084l = 0xFF;
                } else {
                    v084l <<= 1;
                }
                v100m = pQuadReq->colorB;
                if (v100m >= 0x80) {
                    v100m = 0xFF;
                } else {
                    v100m <<= 1;
                }
                v092d.r = *(char *)&v072d;
                v092d.g = *(char *)&v084l;
                v092d.b = *(char *)&v100m;
                v092d.command = *(char *)&((PolygonHundredState *)((GfxPolygonSet *)v016i->polygonSet)->groupState)->alphaLevel;
                v080l = (float)(pQuadReq->offsetX << 3);
                v056d = (float)(pQuadReq->offsetY << 3);
                v052e = (float)((pQuadReq->widthUnits & 0xFF) << 3);
                v112d = (float)((pQuadReq->heightUnits & 0xFF) << 3);
                v064l = (float)*(unsigned char *)&pQuadReq->cellX * v016i->invWidth + v016i->invWidth / 2.0f;
                v068l = (float)pQuadReq->cellY * v016i->invHeight + v016i->invHeight / 2.0f;
                v076d = (float)(pQuadReq->widthUnits - 1);
                v044m = (float)(pQuadReq->heightUnits - 1);
                v060l = v016i->invWidth * v076d;
                v048l = v016i->invHeight * v044m;
                v108l = 0;
                v116d = (GfxBatchPrimitive *)v012j->batchBlock;
                v096c = v064l;
                v104l = v064l + v060l;
                v120l = v068l;
                v040m = v068l + v048l;
                v116d->positions[v108l].x = v080l;
                v116d->positions[v108l].y = v056d;
                *(int *)&v116d->positions[v108l].z = v088k;
                v116d->colors[v108l] = *(int *)&v092d;
                v116d->texCoords[v108l].u = v096c;
                v116d->texCoords[v108l].v = v120l;
                (v108l)++;
                v116d->positions[v108l].x = v080l;
                v116d->positions[v108l].y = v056d + v112d;
                *(int *)&v116d->positions[v108l].z = v088k;
                v116d->colors[v108l] = *(int *)&v092d;
                v116d->texCoords[v108l].u = v096c;
                v116d->texCoords[v108l].v = v040m;
                (v108l)++;
                v116d->positions[v108l].x = v080l + v052e;
                v116d->positions[v108l].y = v056d;
                *(int *)&v116d->positions[v108l].z = v088k;
                v116d->colors[v108l] = *(int *)&v092d;
                v116d->texCoords[v108l].u = v104l;
                v116d->texCoords[v108l].v = v120l;
                (v108l)++;
                v116d->positions[v108l].x = v080l + v052e;
                v116d->positions[v108l].y = v056d + v112d;
                *(int *)&v116d->positions[v108l].z = v088k;
                v116d->colors[v108l] = *(int *)&v092d;
                v116d->texCoords[v108l].u = v104l;
                v116d->texCoords[v108l].v = v040m;
                v116d->clutRow = v020a;
                GtePsxMatrixToFloat4x4Transposed((void *)v024a, (char *)v012j + 0xC);
            }
        }
        return chainTail;
    }
}
} /* extern "C" */

// src/ff7/field/src/field_model_vertex_lighting.cpp
// Vertex work on field models: appending polygon set records, walking flag records,
// adding colour biases and alpha, applying point and polygon lighting, and allocating textured
// polygon slots.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/pfile.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "ff7/enums/field.h"
#include "lib/enums/graphics.h"
extern "C" {

#pragma intrinsic(abs)
/* Per-vertex specular callbacks indexed by GfxLightPolygonSet.specularIndex. */
// GLOBAL: FF7 0x00909230
void (__cdecl *g_FieldSpecularDispatchTable[5])(FieldLightVertex *, GfxLightPolygonSet *) = {
    (void (__cdecl *)(FieldLightVertex *, GfxLightPolygonSet *))Gfx_SetVertexSpecularOpaque,
    (void (__cdecl *)(FieldLightVertex *, GfxLightPolygonSet *))Gfx_SetVertexSpecularNear,
    (void (__cdecl *)(FieldLightVertex *, GfxLightPolygonSet *))Gfx_SetVertexSpecularCached,
    (void (__cdecl *)(FieldLightVertex *, GfxLightPolygonSet *))Gfx_SetVertexSpecularByDepth,
    (void (__cdecl *)(FieldLightVertex *, GfxLightPolygonSet *))Gfx_SetVertexSpecularNone,
};
// GLOBAL: FF7 0x00909244
float g_FieldLightingScale = 1.85f;
// The two 32-byte head-mesh mode templates (their float words as bit patterns) that
// Field_ApplyModelDiffuseSelectorEffect copies by g_FieldRenderStateMode - 1.
// GLOBAL: FF7 0x00909248
FieldHeadTemplate g_FieldHeadTemplates[2] = {
    {35, 52, 40, 1.43f, 0.006f, 1.85f, 0.002f, 0.017f},
    {135, 140, 94, 1.68f, 0.003f, 1.83f, 0.002f, 0.012f},
};
// GLOBAL: FF7 0x00909288
int g_FieldModelVertexLimit = 46;

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// clang-format off
// GLOBAL: FF7 0x00d84f9c
float g_FieldSpecularFactor = 0;
// GLOBAL: FF7 0x00d84fa0
int g_FieldPolygonSetWalkIndex = 0;
unsigned char g_bssPad_00d84fa4[4] = {0};

/*
 * Appends a model polygon set's vertex and polygon counts to a FieldPolygonSetCountArray.
 */
// FUNCTION: FF7 0x0064b090
void __cdecl Field_AppendModelPolygonSetRecord(FieldPolygonSetCountArray *recordArray, GfxPolygonSet *model)
{
    FieldPolygonSetCountArray *countedArray;

    countedArray = recordArray;
    countedArray->entries[countedArray->count].vertexCount = model->modelSource->vertexCount;
    countedArray->entries[countedArray->count].polygonCount = model->modelSource->polygonCount;
    countedArray->entries[countedArray->count].appended = 1;
    countedArray->count++;
}

/*
 * Sets flag->drawEnabled to whether the next entry of a FieldPolygonSetCountArray was appended,
 * and steps to the entry after it.
 */
// FUNCTION: FF7 0x0064b0ee
void __cdecl Field_TraverseModelFlagRecord(FieldPolygonSetCountArray *flagRecord, GfxPolygonSet *flag)
{
    FieldPolygonSetCountArray *record;

    record = flagRecord;
    if (record->restart != 0) {
        g_FieldPolygonSetWalkIndex = 0;
        record->restart = 0;
    }
    if (record->entries[g_FieldPolygonSetWalkIndex].appended != 0) {
        flag->drawEnabled = 1;
    } else {
        flag->drawEnabled = 0;
    }
    g_FieldPolygonSetWalkIndex++;
}

/*
 * Enables drawing on a polygon set.
 */
// FUNCTION: FF7 0x0064b14e
void __cdecl Field_WriteTrueFlag(int context, GfxPolygonSet *flag)
{
    flag->drawEnabled = 1;
}

/*
 * Disables drawing on a polygon set.
 */
// FUNCTION: FF7 0x0064b15c
void __cdecl Field_StoreZeroFlagCallback(int context, GfxPolygonSet *flag)
{
    flag->drawEnabled = 0;
}

/*
 * Applies point lighting calculations to model vertices in world coordinates.
 */
// FUNCTION: FF7 0x0064b16a
void __cdecl Field_ApplyPointLightToModelVertices(FieldPointLight *light, GfxPolygonSet *polySet)
{
    FieldPointLight *unitLightBase;
    GfxEngineContext *ctx;
    float *af;
    float lightPos8[3];
    GfxPolygonSetEntry *a6;
    float lightTint[4];
    float deltaZ;
    float mixWeights[4];
    short a7;
    float position[3];
    int vIdx;
    float a8[3];
    float curDistSq;
    GfxLightVertexRecord *a9;
    float dotNormal;
    float surfNormal[3];
    FieldLightVertex *vert;
    struct Color4 color;
    float bVal;
    int g;
    int countdown;
    float invDistSq;
    float rChan;
    float gValue;
    float channelG;
    float outBlue;
    float a1;
    int j;
    float a2[3];
    float deltaX;
    float a3;
    float deltaY;
    unsigned char bytes[4];
    float a4[3];
    int n;

    unitLightBase = light;
    a7 = unitLightBase->range;
    af = polySet->matrixRecord->matrixFloats;
    a8[0] = (float)unitLightBase->posX;
    a8[1] = (float)unitLightBase->posY;
    a8[2] = (float)unitLightBase->posZ;
    Gfx_UnpackBgraToFloat4(unitLightBase->tint, lightTint);
    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)lightTint);
    Gfx_UnpackBgraToFloat4(unitLightBase->mixWeights, mixWeights);
    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)mixWeights);
    mixWeights[2] = 0;
    mixWeights[1] = 1.0f;
    mixWeights[0] = 1.0f;
    TransformPoint3RowVec(af, a8, lightPos8);
    if (polySet != 0 && polySet->drawEnabled != 0) {
        ctx = GetEngineContext();
        for (vIdx = 0; vIdx < polySet->groupCount; vIdx++) {
            a6 = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(vIdx, polySet, ctx));
            if (a6 != 0) {
                a9 = (GfxLightVertexRecord *)a6->lightRecords;
                if (a9 != 0) {
                    vert = (FieldLightVertex *)a6->vertices;
                    for (countdown = a6->elementCount; countdown > 0; countdown--) {
                        color = *(struct Color4 *)a9->color;
                        Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)color.e);
                        outBlue = color.e[2] * lightTint[2];
                        channelG = color.e[1] * lightTint[1];
                        a1 = color.e[0] * lightTint[0];
                        bVal = color.e[2] * mixWeights[2];
                        gValue = color.e[1] * mixWeights[1];
                        rChan = color.e[0] * mixWeights[0];
                        position[0] = vert->x;
                        position[1] = vert->y;
                        position[2] = vert->z;
                        TransformPoint3RowVec(af, position, a2);
                        TransformPoint3RowVec(af, (void *)a9->tag, surfNormal);
                        a3 = 1.0f / (float)pow(surfNormal[0] * surfNormal[0] + surfNormal[1] * surfNormal[1] + surfNormal[2] * surfNormal[2], 0.5);
                        surfNormal[0] *= a3;
                        surfNormal[1] *= a3;
                        surfNormal[2] *= a3;
                        deltaX = a2[0] - lightPos8[0];
                        deltaY = a2[1] - lightPos8[1];
                        deltaZ = a2[2] - lightPos8[2];
                        curDistSq = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
                        invDistSq = 1.0f / curDistSq;
                        dotNormal = deltaX * surfNormal[0] + deltaY * surfNormal[1] + deltaZ * surfNormal[2];
                        a4[2] = outBlue * color.e[2] + invDistSq * bVal * color.e[2] * dotNormal;
                        a4[1] = channelG * color.e[1] + invDistSq * gValue * color.e[1] * dotNormal;
                        a4[0] = a1 * color.e[0] + invDistSq * rChan * color.e[0] * dotNormal;
                        Blit_PackBgraFloatsToArgb8888(bytes, a4);
                        bytes[3] = 0xFF;
                        vert->color = *(int *)bytes;
                        vert++;
                        a9++;
                    }
                    Gfx_UnlockPolygonSetGroup(vIdx, polySet, ctx);
                }
            }
        }
    }
}

/*
 * Swaps arguments and forwards call to polygon set lighting.
 */
// FUNCTION: FF7 0x0064b563
void __cdecl Field_LightPolygonSetCallbackSwapped(GfxLightPolygonSet *context, struct GfxPolygonSet *polySet)
{
    GfxLightPolygonSet *lightSet;

    lightSet = context;
    Gfx_LightPolygonSet(polySet, lightSet);
}

/*
 * Adds color components to model vertex color records.
 */
// FUNCTION: FF7 0x0064b581
void __cdecl Field_AddColorToModelVertexColors(int color, FieldModelAlphaGroups *model)
{
    FieldDiffuseSelectorGroup *group;
    int grpNo;
    int entryIndex;
    FieldSelectorEntry *record;
    GfxPolygonSet *polySet;

    if (model != 0) {
        group = model->groups;
        for (grpNo = 0; grpNo < model->groupCount; grpNo++) {
            if (group->entryCount > 0) {
                record = group->records;
                if (record != 0) {
                    for (entryIndex = 0; entryIndex < group->entryCount; entryIndex++) {
                        if (record->polySetHolder != 0) {
                            polySet = record->polySetHolder->polySet;
                            Field_AddColorToPolygonSetVertexColors(color, polySet);
                        }
                        record++;
                    }
                }
            }
            group++;
        }
    }
}

/*
 * Adds saturated color bytes to vertex colors of a polygon set.
 */
// FUNCTION: FF7 0x0064b62e
void __cdecl Field_AddColorToPolygonSetVertexColors(int color, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *group;
    FieldLightVertex *vertex;
    int i;
    int vertexIndex;

    if (polySet != 0) {
        ctx = GetEngineContext();
        for (i = 0; i < polySet->groupCount; i++) {
            group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(i, polySet, ctx));
            if (group != 0) {
                vertex = (FieldLightVertex *)group->vertices;
                for (vertexIndex = 0; vertexIndex < group->elementCount; vertexIndex++) {
                    Field_AddColorBytesSaturated((unsigned char *)color, (unsigned char *)&vertex->color);
                    vertex++;
                }
                Gfx_UnlockPolygonSetGroup(i, polySet, ctx);
            }
        }
    }
}

/*
 * Adds RGB color bytes with saturation to 0xFF.
 */
// FUNCTION: FF7 0x0064b6e1
void __cdecl Field_AddColorBytesSaturated(unsigned char *add, unsigned char *color)
{
    /* Retail's inline x86 saturating RGB byte-add sequence. */
    __asm {
        mov esi, add
        mov edi, color
        mov al, byte ptr [esi + 2]
        add byte ptr [edi + 2], al
        jb blue_overflow
        jmp green
blue_overflow:
        mov byte ptr [edi + 2], 0ffh
green:
        mov bl, byte ptr [esi + 1]
        add byte ptr [edi + 1], bl
        jb green_overflow
        jmp red
green_overflow:
        mov byte ptr [edi + 1], 0ffh
red:
        mov al, byte ptr [esi]
        add byte ptr [edi], al
        jb red_overflow
        jmp done
red_overflow:
        mov byte ptr [edi], 0ffh
done:
    }
}

/*
 * Adds signed RGB color bias to model vertex color hierarchy.
 */
// FUNCTION: FF7 0x0064b719
void __cdecl Field_AddColorBiasToModelVertexColors(int blueBias, int greenBias, int redBias, FieldModelAlphaGroups *model)
{
    FieldDiffuseSelectorGroup *group;
    int entryIndex;
    FieldSelectorEntry *record;
    int groupSlot;
    GfxPolygonSet *polySet;

    if (model != 0) {
        group = model->groups;
        for (groupSlot = 0; groupSlot < model->groupCount; groupSlot++) {
            if (group->entryCount > 0) {
                record = group->records;
                if (record != 0) {
                    for (entryIndex = 0; entryIndex < group->entryCount; entryIndex++) {
                        if (record->polySetHolder != 0) {
                            polySet = record->polySetHolder->polySet;
                            Field_AddColorBiasToPolygonSetVertexColors((short)blueBias, (short)greenBias, (short)redBias, polySet);
                        }
                        record++;
                    }
                }
            }
            group++;
        }
    }
}

/*
 * Adds signed RGB color bias to polygon set vertices with clamping.
 */
// FUNCTION: FF7 0x0064b7d1
void __cdecl Field_AddColorBiasToPolygonSetVertexColors(short blueBias, short greenBias, short redBias, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *group;
    FieldLightVertex *vertex;
    int i;
    int vertexIndex;

    if (polySet != 0) {
        ctx = GetEngineContext();
        for (i = 0; i < polySet->groupCount; i++) {
            group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(i, polySet, ctx));
            if (group != 0) {
                vertex = (FieldLightVertex *)group->vertices;
                for (vertexIndex = 0; vertexIndex < group->elementCount; vertexIndex++) {
                    Field_AddColorBiasToVertexColor((short)blueBias, (short)greenBias, (short)redBias, (FieldVertexColor *)&vertex->color);
                    vertex++;
                }
                Gfx_UnlockPolygonSetGroup(i, polySet, ctx);
            }
        }
    }
}

/*
 * Adds signed bias to single vertex color and clamps to [0, 255].
 */
// FUNCTION: FF7 0x0064b893
void __cdecl Field_AddColorBiasToVertexColor(short blueBias, short greenBias, short redBias, FieldVertexColor *vertexColor)
{
    short red;
    short green;
    short blue;

    blue = vertexColor->colorB + blueBias;
    green = vertexColor->colorG + greenBias;
    red = vertexColor->colorR + redBias;
    if (blue < 0) {
        vertexColor->colorB = 0;
    } else if (blue > 0xFF) {
        vertexColor->colorB = 0xFF;
    } else {
        vertexColor->colorB = blue;
    }
    if (green < 0) {
        vertexColor->colorG = 0;
    } else if (green > 0xFF) {
        vertexColor->colorG = 0xFF;
    } else {
        vertexColor->colorG = green;
    }
    if (red < 0) {
        vertexColor->colorR = 0;
    } else if (red > 0xFF) {
        vertexColor->colorR = 0xFF;
    } else {
        vertexColor->colorR = red;
    }
}

/*
 * Sets alpha channel to 192 (0xC0) for all vertices in a model.
 */
// FUNCTION: FF7 0x0064b95c
void __cdecl Field_SetModelVertexAlpha192(FieldModelAlphaGroups *model, GfxEngineContext *context)
{
    int alphaIndex;
    FieldDiffuseSelectorGroup *group;
    int entryIndex;
    FieldSelectorEntry *record;
    GfxPolygonSet *polySet;

    if (model != 0) {
        group = model->groups;
        for (alphaIndex = 0; alphaIndex < model->groupCount; alphaIndex++) {
            if (group->entryCount > 0) {
                record = group->records;
                if (record != 0) {
                    for (entryIndex = 0; entryIndex < group->entryCount; entryIndex++) {
                        if (record->polySetHolder != 0) {
                            polySet = record->polySetHolder->polySet;
                            Field_SetPolygonSetVertexAlpha192(polySet);
                        }
                        record++;
                    }
                }
            }
            group++;
        }
    }
}

/*
 * Sets alpha channel to 192 (0xC0) for all vertices in a polygon set.
 */
// FUNCTION: FF7 0x0064ba02
void __cdecl Field_SetPolygonSetVertexAlpha192(GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *group;
    FieldLightVertex *vertex;
    int i;
    int vertexIndex;

    if (polySet != 0) {
        ctx = GetEngineContext();
        for (i = 0; i < polySet->groupCount; i++) {
            group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(i, polySet, ctx));
            if (group != 0) {
                vertex = (FieldLightVertex *)group->vertices;
                for (vertexIndex = 0; vertexIndex < group->elementCount; vertexIndex++) {
                    vertex->color = (unsigned int)(vertex->color << 8) >> 8 | 0xC0000000;
                    vertex++;
                }
                Gfx_UnlockPolygonSetGroup(i, polySet, ctx);
            }
        }
    }
}

/*
 * Applies diffuse color selectors, scaling, and thresholding to model lighting.
 */
// FUNCTION: FF7 0x0064bab9
void __cdecl Field_ApplyModelDiffuseSelectorEffect(FieldModelInstance *model, short red, short green, short blue, short scale, int flags)
{
#pragma pack(push, 1)
    union {
        unsigned char storage[200];
        float (*polyMatrix)[4];
        struct { unsigned char pad_to_4[0x4]; GfxPolygonSet *polySet; };
        struct { unsigned char pad_to_8[0x8]; int recordIndex; };
        struct { unsigned char pad_to_C[0xC]; FieldSelectorEntry *selectorRecord; };
        struct { unsigned char pad_to_10[0x10]; int groupIndex; };
        struct { unsigned char pad_to_14[0x14]; FieldDiffuseSelectorGroup *selectorGroup; };
        struct { unsigned char pad_to_18[0x18]; int vertexIndex; };
        struct { unsigned char pad_to_1C[0x1C]; int greenLevel; };
        struct { unsigned char pad_to_20[0x20]; int effectIndex; };
        struct { unsigned char pad_to_24[0x24]; FieldModelAlphaGroups *modelDiffuse; };
        struct { unsigned char pad_to_28[0x28]; float lightScale; };
        struct { unsigned char pad_to_2C[0x2C]; FieldLightVertex *diffuseEffect; };
        struct { unsigned char pad_to_30[0x30]; float depthValue; };
        struct { unsigned char pad_to_34[0x34]; float combinedMatrix[4][4]; };
        struct { unsigned char pad_to_74[0x74]; int firstLit; };
        struct { unsigned char pad_to_78[0x78]; int suppressed; };
        struct { unsigned char pad_to_7C[0x7C]; GfxEngineContext *engineContext; };
        struct { unsigned char pad_to_80[0x80]; FieldPolygonLightingRecord *groupEntry; };
        struct { unsigned char pad_to_84[0x84]; GfxPolygonSetEntry *diffuseGroup; };
        struct { unsigned char pad_to_88[0x88]; int blueLevel; };
        struct { unsigned char pad_to_8C[0x8C]; GfxVec3Dual diffusePos; };
        struct { unsigned char pad_to_98[0x98]; int redLevel; };
        struct { unsigned char pad_to_9C[0x9C]; FieldHeadTemplate headTemplate; };
        struct { unsigned char pad_to_BC[0xBC]; GfxHomogeneousPoint transformedPoint; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.modelDiffuse = model->alphaGroups;
    frame.engineContext = GetEngineContext();
    frame.firstLit = 0;
    frame.suppressed = 0;
    frame.redLevel = red << 4;
    frame.greenLevel = green << 4;
    frame.blueLevel = blue << 4;
    if (g_FieldRenderStateMode != FIELD_RENDER_STATE_NONE) {
        frame.headTemplate = g_FieldHeadTemplates[g_FieldRenderStateMode - 1];
    }
    if (frame.modelDiffuse != 0) {
        frame.selectorGroup = frame.modelDiffuse->groups;
        for (frame.groupIndex = 0; frame.groupIndex < frame.modelDiffuse->groupCount; frame.groupIndex++) {
            if (frame.selectorGroup->entryCount > 0) {
                frame.selectorRecord = frame.selectorGroup->records;
                if (frame.selectorRecord != 0) {
                    for (frame.recordIndex = 0; frame.recordIndex < frame.selectorGroup->entryCount; frame.recordIndex++) {
                        if (frame.selectorRecord->polySetHolder != 0) {
                            frame.polySet = frame.selectorRecord->polySetHolder->polySet;
                            if (frame.polySet != 0) {
                                for (frame.vertexIndex = 0; frame.vertexIndex < frame.polySet->groupCount; frame.vertexIndex++) {
                                    frame.diffuseGroup = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(frame.vertexIndex, (struct GfxPolygonSet *)frame.polySet, (struct GfxEngineContext *)frame.engineContext));
                                    if (frame.diffuseGroup != 0) {
                                        frame.polyMatrix = frame.polySet->matrixRecord->worldMatrix;
                                        frame.diffuseEffect = (FieldLightVertex *)frame.diffuseGroup->vertices;
                                        frame.groupEntry = (FieldPolygonLightingRecord *)frame.diffuseGroup->lightRecords;
                                        for (frame.effectIndex = 0; frame.effectIndex < frame.diffuseGroup->elementCount; frame.effectIndex++) {
                                            frame.diffuseEffect->color = frame.groupEntry->tag;
                                            if (g_FieldRenderStateMode != FIELD_RENDER_STATE_NONE) {
                                                switch (g_FieldRenderStateMode) {
                                                case FIELD_RENDER_STATE_MAP_1C1:
                                                    frame.diffusePos.xBits = frame.diffuseEffect->xBits;
                                                    frame.diffusePos.yBits = frame.diffuseEffect->yBits;
                                                    frame.diffusePos.z = frame.diffuseEffect->z / 4.0f;
                                                    MultiplyMatrix4x4Full(frame.polyMatrix, g_FieldModelRenderState, frame.combinedMatrix);
                                                    TransformPoint3RowVec(frame.combinedMatrix, &frame.diffusePos.x, &frame.transformedPoint.x);
                                                    if ((flags & 4) != 0) {
                                                        frame.depthValue = frame.transformedPoint.y;
                                                    } else {
                                                        frame.depthValue = frame.transformedPoint.z;
                                                    }
                                                    if (frame.firstLit == 0) {
                                                        frame.firstLit = 1;
                                                        if (frame.headTemplate.upperThreshold > frame.depthValue) {
                                                            g_FieldLightingScale = frame.headTemplate.rampBase;
                                                        } else {
                                                            g_FieldLightingScale = (frame.depthValue - frame.headTemplate.upperThreshold) * frame.headTemplate.upperSlope + frame.headTemplate.rampBase;
                                                        }
                                                    }
                                                    if (frame.headTemplate.lowerThreshold > frame.depthValue) {
                                                        frame.lightScale = frame.headTemplate.rampFloor;
                                                    } else {
                                                        frame.lightScale = (frame.depthValue - frame.headTemplate.lowerThreshold) * frame.headTemplate.lowerSlope + g_FieldLightingScale;
                                                    }
                                                    frame.lightScale = (float)frame.headTemplate.blendAmount * frame.lightScale / abs(scale) + (float)(scale + frame.headTemplate.blendAmount) * frame.headTemplate.blendTarget;
                                                    break;
                                                case FIELD_RENDER_STATE_MAP_058:
                                                    frame.diffusePos.xBits = frame.diffuseEffect->xBits;
                                                    frame.diffusePos.yBits = frame.diffuseEffect->yBits;
                                                    frame.diffusePos.z = frame.diffuseEffect->z / 4.0f;
                                                    MultiplyMatrix4x4Full(frame.polyMatrix, g_FieldModelRenderState, frame.combinedMatrix);
                                                    TransformPoint3RowVec(frame.combinedMatrix, &frame.diffusePos.x, &frame.transformedPoint.x);
                                                    if ((flags & 4) != 0) {
                                                        frame.depthValue = frame.transformedPoint.y;
                                                    } else {
                                                        frame.depthValue = frame.transformedPoint.z;
                                                    }
                                                    if (frame.firstLit == 0) {
                                                        frame.firstLit = 1;
                                                        if (frame.depthValue < 1.4e+02f) {
                                                            frame.headTemplate.rampFloor = 1.829200029373169f;
                                                            frame.lightScale = frame.headTemplate.rampFloor;
                                                        } else {
                                                            frame.lightScale = (frame.depthValue - 1.4e+02f) * 0.0122589f + 1.8292f;
                                                        }
                                                        g_FieldSpecularFactor = frame.lightScale;
                                                    }
                                                    frame.lightScale = g_FieldSpecularFactor - 0.2786f;
                                                    break;
                                                case FIELD_RENDER_STATE_MAP_1AD:
                                                    frame.diffusePos.xBits = frame.diffuseEffect->xBits;
                                                    frame.diffusePos.yBits = frame.diffuseEffect->yBits;
                                                    frame.diffusePos.z = frame.diffuseEffect->z / 4.0f;
                                                    MultiplyMatrix4x4Full(frame.polyMatrix, g_FieldModelRenderState, frame.combinedMatrix);
                                                    TransformPoint3RowVec(frame.combinedMatrix, &frame.diffusePos.x, &frame.transformedPoint.x);
                                                    if ((flags & 4) != 0) {
                                                        frame.depthValue = frame.transformedPoint.y;
                                                    } else {
                                                        frame.depthValue = frame.transformedPoint.z;
                                                    }
                                                    if (frame.firstLit == 0) {
                                                        frame.firstLit = 1;
                                                        if (frame.depthValue > -165.0f) {
                                                            g_FieldLightingScale = 70.0f;
                                                        } else {
                                                            g_FieldLightingScale = (frame.depthValue + 286.0f) * 0.84f + -6e+01f;
                                                        }
                                                    }
                                                    frame.lightScale = g_FieldLightingScale * 0.11f + -32.0f;
                                                    frame.lightScale = 7.0f * frame.lightScale / abs(scale);
                                                    break;
                                                case FIELD_RENDER_STATE_MAP_1AE:
                                                    frame.diffusePos.xBits = frame.diffuseEffect->xBits;
                                                    frame.diffusePos.yBits = frame.diffuseEffect->yBits;
                                                    frame.diffusePos.z = frame.diffuseEffect->z / 4.0f;
                                                    MultiplyMatrix4x4Full(frame.polyMatrix, g_FieldModelRenderState, frame.combinedMatrix);
                                                    TransformPoint3RowVec(frame.combinedMatrix, &frame.diffusePos.x, &frame.transformedPoint.x);
                                                    if ((flags & 4) != 0) {
                                                        frame.depthValue = frame.transformedPoint.y;
                                                    } else {
                                                        frame.depthValue = frame.transformedPoint.z;
                                                    }
                                                    if (frame.firstLit == 0) {
                                                        frame.firstLit = 1;
                                                        if (frame.transformedPoint.y > -1.35e+03f) {
                                                            frame.suppressed = 1;
                                                        }
                                                        if (frame.depthValue < 0.0f) {
                                                            if (frame.depthValue <= -229.0f) {
                                                                g_FieldLightingScale = 33.0f;
                                                            } else {
                                                                g_FieldLightingScale = (frame.depthValue + 229.0f) * -1.121f + 33.0f;
                                                            }
                                                        } else {
                                                            g_FieldLightingScale = frame.depthValue * -1.11f;
                                                        }
                                                    }
                                                    frame.lightScale = 2e+02f + g_FieldLightingScale;
                                                    if (scale != 1 && frame.suppressed == 0) {
                                                        frame.lightScale = 1e+03f;
                                                    }
                                                    break;
                                                case FIELD_RENDER_STATE_MAP_1B3:
                                                    frame.diffusePos.xBits = frame.diffuseEffect->xBits;
                                                    frame.diffusePos.yBits = frame.diffuseEffect->yBits;
                                                    frame.diffusePos.z = frame.diffuseEffect->z / 4.0f;
                                                    MultiplyMatrix4x4Full(frame.polyMatrix, g_FieldModelRenderState, frame.combinedMatrix);
                                                    TransformPoint3RowVec(frame.combinedMatrix, &frame.diffusePos.x, &frame.transformedPoint.x);
                                                    if ((flags & 4) != 0) {
                                                        frame.depthValue = frame.transformedPoint.y;
                                                    } else {
                                                        frame.depthValue = frame.transformedPoint.z;
                                                    }
                                                    frame.lightScale = 9.55f;
                                                    frame.lightScale = 16.0f * frame.lightScale / abs(scale);
                                                    break;
                                                case FIELD_RENDER_STATE_MAP_281:
                                                    frame.diffusePos.xBits = frame.diffuseEffect->xBits;
                                                    frame.diffusePos.y = frame.diffuseEffect->y / 4.0f;
                                                    frame.diffusePos.z = frame.diffuseEffect->z / 4.0f;
                                                    MultiplyMatrix4x4Full(frame.polyMatrix, g_FieldModelRenderState, frame.combinedMatrix);
                                                    TransformPoint3RowVec(frame.combinedMatrix, &frame.diffusePos.x, &frame.transformedPoint.x);
                                                    frame.depthValue = frame.transformedPoint.z;
                                                    frame.lightScale = -543.0f;
                                                    scale = 1;
                                                    blue += 0x1E;
                                                }
                                                if (-frame.depthValue < (float)scale * frame.lightScale && frame.suppressed == 0) {
                                                    Field_AddBgraColorSaturated(red << 0x10, green << 8, blue, (int)frame.groupEntry + 0x14, (int)frame.diffuseEffect + 0x10);
                                                }
                                            }
                                            frame.diffuseEffect++;
                                            frame.groupEntry++;
                                        }
                                        Gfx_UnlockPolygonSetGroup(frame.vertexIndex, (struct GfxPolygonSet *)frame.polySet, (struct GfxEngineContext *)frame.engineContext);
                                    }
                                }
                            }
                        }
                        frame.selectorRecord++;
                    }
                }
            }
            frame.selectorGroup++;
        }
    }
}

/*
 * Swaps arguments and forwards call to gated polygon set lighting.
 */
// FUNCTION: FF7 0x0064c364
void __cdecl Field_ApplyPolygonSetLightingGatedCallback(GfxLightPolygonSet *context, GfxPolygonSet *polySet)
{
    GfxLightPolygonSet *lightSet;

    lightSet = context;
    Field_ApplyPolygonSetLightingGated(polySet, lightSet);
}

/*
 * Computes gated vertex lighting and specular highlights for a polygon set.
 */
// FUNCTION: FF7 0x0064c382
void __cdecl Field_ApplyPolygonSetLightingGated(GfxPolygonSet *polySet, GfxLightPolygonSet *lightSet)
{
    struct {
        float pointColor[4];
        float floorColor[4];
        int remaining;
        FieldLightVertex *matchEntry;
        FieldPolygonLightingRecord *matchLightRecord;
        int remainingEntries;
        FieldLightVertex *specularCursor;
        FieldPolygonLightingRecord *specularLightRecord;
        float litNormalColor[4];
        float floorNormalColor[4];
        int elementCount;
        FieldLightVertex *groupEntry;
        FieldPolygonLightingRecord *lightRecord;
        int lightMode;
        int vertexIndex;
        FieldLightVertex *specularEntry;
        void (__cdecl *specularFunc)(FieldLightVertex *, GfxLightPolygonSet *);
        GfxPolygonSetEntry *polyGroup;
        struct GfxEngineContext *engineContext;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (polySet != 0 && lightSet != 0) {
        frame.engineContext = (struct GfxEngineContext *)GetEngineContext();
        frame.lightMode = lightSet->flags & GFX_LIGHT_MODE_MASK;
        switch (frame.lightMode) {
        case GFX_LIGHT_MODE_NONE:
            break;
        case GFX_LIGHT_MODE_DIFFUSE:
            for (frame.vertexIndex = 0; frame.vertexIndex < polySet->groupCount; frame.vertexIndex++) {
                frame.polyGroup = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.vertexIndex, polySet, frame.engineContext);
                if (frame.polyGroup != 0) {
                    frame.lightRecord = (FieldPolygonLightingRecord *)frame.polyGroup->lightRecords;
                    if (frame.lightRecord != 0) {
                        frame.groupEntry = (FieldLightVertex *)frame.polyGroup->vertices;
                        if (frame.groupEntry->color == frame.lightRecord->tag) {
                            if ((lightSet->flags & GFX_LIGHT_FLAG_FLOOR) != 0) {
                                if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                                    Gfx_UnpackBgraToFloat4(lightSet->colorBgra, frame.floorNormalColor);
                                    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.floorNormalColor);
                                    for (frame.elementCount = frame.polyGroup->elementCount; frame.elementCount > 0; frame.elementCount--) {
                                        Gfx_TransformPointFloorLitColorToBgra(lightSet->colorBgra, (int)frame.floorNormalColor, (GfxFloorVertex *)lightSet->floorVertex, frame.lightRecord->normal, (GfxLitLightColor *)frame.lightRecord->color, (char *)&frame.lightRecord->tag, (struct GfxBgraPixel *)((int)frame.groupEntry + 0x10));
                                        frame.groupEntry++;
                                        frame.lightRecord++;
                                    }
                                } else {
                                    for (frame.elementCount = frame.polyGroup->elementCount; frame.elementCount > 0; frame.elementCount--) {
                                        Gfx_TransformPointFloorColorToBgra((GfxFloorVertex *)lightSet->floorVertex, frame.lightRecord->normal, (GfxLitLightColor *)frame.lightRecord->color, (char *)&frame.lightRecord->tag, (struct GfxBgraPixel *)((int)frame.groupEntry + 0x10));
                                        frame.groupEntry++;
                                        frame.lightRecord++;
                                    }
                                }
                            } else if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                                Gfx_UnpackBgraToFloat4(lightSet->colorBgra, frame.litNormalColor);
                                Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.litNormalColor);
                                for (frame.elementCount = frame.polyGroup->elementCount; frame.elementCount > 0; frame.elementCount--) {
                                    Gfx_TransformPointLitColorToBgra(lightSet->colorBgra, (int)frame.litNormalColor, (GfxFloorVertex *)lightSet->floorVertex, frame.lightRecord->normal, (GfxFloatColorAddend *)frame.lightRecord->color, (char *)&frame.lightRecord->tag, (struct GfxBgraPixel *)((int)frame.groupEntry + 0x10));
                                    frame.groupEntry++;
                                    frame.lightRecord++;
                                }
                            } else {
                                for (frame.elementCount = frame.polyGroup->elementCount; frame.elementCount > 0; frame.elementCount--) {
                                    Gfx_LightVertexDiffuse((GfxFloorVertex *)lightSet->floorVertex, frame.lightRecord->normal, (GfxFloatColorAddend *)frame.lightRecord->color, &frame.lightRecord->tag, (struct GfxVertexColor *)((int)frame.groupEntry + 0x10));
                                    frame.groupEntry++;
                                    frame.lightRecord++;
                                }
                            }
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.vertexIndex, polySet, frame.engineContext);
                }
            }
            break;
        case GFX_LIGHT_MODE_SPECULAR:
            Gfx_UpdateCachedSpecularColor((struct GfxSpecularState *)lightSet);
            frame.specularFunc = g_FieldSpecularDispatchTable[lightSet->specularIndex];
            for (frame.vertexIndex = 0; frame.vertexIndex < polySet->groupCount; frame.vertexIndex++) {
                frame.polyGroup = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.vertexIndex, polySet, frame.engineContext);
                if (frame.polyGroup != 0) {
                    frame.specularLightRecord = (FieldPolygonLightingRecord *)frame.polyGroup->lightRecords;
                    if (frame.specularLightRecord != 0) {
                        frame.specularCursor = (FieldLightVertex *)frame.polyGroup->vertices;
                        for (frame.remainingEntries = frame.polyGroup->elementCount; frame.remainingEntries > 0; frame.remainingEntries--) {
                            frame.specularEntry = frame.specularCursor;
                            frame.specularFunc(frame.specularEntry, lightSet);
                            frame.specularCursor++;
                            frame.specularLightRecord++;
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.vertexIndex, polySet, frame.engineContext);
                }
            }
            break;
        case GFX_LIGHT_MODE_DIFFUSE_SPECULAR:
            Gfx_UpdateCachedSpecularColor((struct GfxSpecularState *)lightSet);
            frame.specularFunc = g_FieldSpecularDispatchTable[lightSet->specularIndex];
            for (frame.vertexIndex = 0; frame.vertexIndex < polySet->groupCount; frame.vertexIndex++) {
                frame.polyGroup = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.vertexIndex, polySet, frame.engineContext);
                if (frame.polyGroup != 0) {
                    frame.matchLightRecord = (FieldPolygonLightingRecord *)frame.polyGroup->lightRecords;
                    if (frame.matchLightRecord != 0) {
                        frame.matchEntry = (FieldLightVertex *)frame.polyGroup->vertices;
                        if (frame.matchEntry->color == frame.matchLightRecord->tag) {
                            if ((lightSet->flags & GFX_LIGHT_FLAG_FLOOR) != 0) {
                                if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                                    Gfx_UnpackBgraToFloat4(lightSet->colorBgra, frame.floorColor);
                                    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.floorColor);
                                    for (frame.remaining = frame.polyGroup->elementCount; frame.remaining > 0; frame.remaining--) {
                                        Gfx_TransformPointFloorLitColorToBgra(lightSet->colorBgra, (int)frame.floorColor, (GfxFloorVertex *)lightSet->floorVertex, frame.matchLightRecord->normal, (GfxLitLightColor *)frame.matchLightRecord->color, (char *)&frame.matchLightRecord->tag, (struct GfxBgraPixel *)((int)frame.matchEntry + 0x10));
                                        frame.specularEntry = frame.matchEntry;
                                        frame.specularFunc(frame.specularEntry, lightSet);
                                        frame.matchEntry++;
                                        frame.matchLightRecord++;
                                    }
                                } else {
                                    for (frame.remaining = frame.polyGroup->elementCount; frame.remaining > 0; frame.remaining--) {
                                        Gfx_TransformPointFloorColorToBgra((GfxFloorVertex *)lightSet->floorVertex, frame.matchLightRecord->normal, (GfxLitLightColor *)frame.matchLightRecord->color, (char *)&frame.matchLightRecord->tag, (struct GfxBgraPixel *)((int)frame.matchEntry + 0x10));
                                        frame.specularEntry = frame.matchEntry;
                                        frame.specularFunc(frame.specularEntry, lightSet);
                                        frame.matchEntry++;
                                        frame.matchLightRecord++;
                                    }
                                }
                            } else if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                                Gfx_UnpackBgraToFloat4(lightSet->colorBgra, frame.pointColor);
                                Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.pointColor);
                                for (frame.remaining = frame.polyGroup->elementCount; frame.remaining > 0; frame.remaining--) {
                                    Gfx_TransformPointLitColorToBgra(lightSet->colorBgra, (int)frame.pointColor, (GfxFloorVertex *)lightSet->floorVertex, frame.matchLightRecord->normal, (GfxFloatColorAddend *)frame.matchLightRecord->color, (char *)&frame.matchLightRecord->tag, (struct GfxBgraPixel *)((int)frame.matchEntry + 0x10));
                                    frame.specularEntry = frame.matchEntry;
                                    frame.specularFunc(frame.specularEntry, lightSet);
                                    frame.matchEntry++;
                                    frame.matchLightRecord++;
                                }
                            } else {
                                for (frame.remaining = frame.polyGroup->elementCount; frame.remaining > 0; frame.remaining--) {
                                    Gfx_LightVertexDiffuse((GfxFloorVertex *)lightSet->floorVertex, frame.matchLightRecord->normal, (GfxFloatColorAddend *)frame.matchLightRecord->color, &frame.matchLightRecord->tag, (struct GfxVertexColor *)((int)frame.matchEntry + 0x10));
                                    frame.specularEntry = frame.matchEntry;
                                    frame.specularFunc(frame.specularEntry, lightSet);
                                    frame.matchEntry++;
                                    frame.matchLightRecord++;
                                }
                            }
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.vertexIndex, polySet, frame.engineContext);
                }
            }
        }
    }
}

/*
 * Swaps arguments and forwards call to ungated polygon set lighting.
 */
// FUNCTION: FF7 0x0064ca47
void __cdecl Field_ApplyPolygonSetLightingUngatedCallback(GfxLightPolygonSet *lightSetPtr, GfxPolygonSet *polySet)
{
    GfxLightPolygonSet *lightSet;

    lightSet = lightSetPtr;
    Field_ApplyPolygonSetLightingUngated(polySet, lightSet);
}

/*
 * Computes ungated vertex lighting and specular highlights for a polygon set.
 */
// FUNCTION: FF7 0x0064ca65
void __cdecl Field_ApplyPolygonSetLightingUngated(GfxPolygonSet *polySet, GfxLightPolygonSet *lightSet)
{
    struct {
        int diffuseVertexBgra;
        float diffuseUnpackedColor[4];
        int litVertexBgra;
        float litFloatColor[4];
        float litNormLightColor[4];
        int floorPlainBgra;
        float floorPlainColor[4];
        int floorLitBgra;
        float floorLitColor[4];
        float floorLitNormLight[4];
        int elementCount;
        FieldLightVertex *vertexCursor;
        FieldPolygonLightingRecord *groupLightCursor;
        int specularRemaining;
        FieldLightVertex *specularCursor;
        FieldPolygonLightingRecord *specularLightRecord;
        int unlitDiffuseBgra;
        float unlitDiffuseColor[4];
        int unlitPointBgra;
        float unlitPointColor[4];
        float unlitNormLight[4];
        int floorBgra;
        float floorLightColor[4];
        int entryBgra;
        float entryColor[4];
        float floorNormalColor[4];
        int remainingEntries;
        FieldLightVertex *entryCursor;
        FieldPolygonLightingRecord *lightRecord;
        int lightMode;
        int groupIndex;
        FieldLightVertex *specularTarget;
        void (__cdecl *specularFunc)(FieldLightVertex *, GfxLightPolygonSet *);
        GfxPolygonSetEntry *polyGroup;
        struct GfxEngineContext *engineContext;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (polySet != 0 && lightSet != 0) {
        frame.engineContext = (struct GfxEngineContext *)GetEngineContext();
        frame.lightMode = lightSet->flags & GFX_LIGHT_MODE_MASK;
        switch (frame.lightMode) {
        case GFX_LIGHT_MODE_NONE:
            break;
        case GFX_LIGHT_MODE_DIFFUSE:
            for (frame.groupIndex = 0; frame.groupIndex < polySet->groupCount; frame.groupIndex++) {
                frame.polyGroup = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.groupIndex, polySet, frame.engineContext);
                if (frame.polyGroup != 0) {
                    frame.lightRecord = (FieldPolygonLightingRecord *)frame.polyGroup->lightRecords;
                    if (frame.lightRecord != 0) {
                        frame.entryCursor = (FieldLightVertex *)frame.polyGroup->vertices;
                        if ((lightSet->flags & GFX_LIGHT_FLAG_FLOOR) != 0) {
                            if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                                Gfx_UnpackBgraToFloat4(lightSet->colorBgra, frame.floorNormalColor);
                                Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.floorNormalColor);
                                for (frame.remainingEntries = frame.polyGroup->elementCount; frame.remainingEntries > 0; frame.remainingEntries--) {
                                    frame.entryBgra = frame.entryCursor->color;
                                    Gfx_UnpackBgraToFloat4(frame.entryBgra, frame.entryColor);
                                    Gfx_TransformPointFloorLitColorToBgra(lightSet->colorBgra, (int)frame.floorNormalColor, (GfxFloorVertex *)lightSet->floorVertex, frame.lightRecord->normal, (GfxLitLightColor *)frame.entryColor, &frame.entryBgra, (struct GfxBgraPixel *)((int)frame.entryCursor + 0x10));
                                    frame.entryCursor++;
                                    frame.lightRecord++;
                                }
                            } else {
                                for (frame.remainingEntries = frame.polyGroup->elementCount; frame.remainingEntries > 0; frame.remainingEntries--) {
                                    frame.floorBgra = frame.entryCursor->color;
                                    Gfx_UnpackBgraToFloat4(frame.floorBgra, frame.floorLightColor);
                                    Gfx_TransformPointFloorColorToBgra((GfxFloorVertex *)lightSet->floorVertex, frame.lightRecord->normal, (GfxLitLightColor *)frame.floorLightColor, &frame.floorBgra, (struct GfxBgraPixel *)((int)frame.entryCursor + 0x10));
                                    frame.entryCursor++;
                                    frame.lightRecord++;
                                }
                            }
                        } else if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                            Gfx_UnpackBgraToFloat4(lightSet->colorBgra, frame.unlitNormLight);
                            Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.unlitNormLight);
                            for (frame.remainingEntries = frame.polyGroup->elementCount; frame.remainingEntries > 0; frame.remainingEntries--) {
                                frame.unlitPointBgra = frame.entryCursor->color;
                                Gfx_UnpackBgraToFloat4(frame.unlitPointBgra, frame.unlitPointColor);
                                Gfx_TransformPointLitColorToBgra(lightSet->colorBgra, (int)frame.unlitNormLight, (GfxFloorVertex *)lightSet->floorVertex, frame.lightRecord->normal, (GfxFloatColorAddend *)frame.unlitPointColor, &frame.unlitPointBgra, (struct GfxBgraPixel *)((int)frame.entryCursor + 0x10));
                                frame.entryCursor++;
                                frame.lightRecord++;
                            }
                        } else {
                            for (frame.remainingEntries = frame.polyGroup->elementCount; frame.remainingEntries > 0; frame.remainingEntries--) {
                                frame.unlitDiffuseBgra = frame.entryCursor->color;
                                Gfx_UnpackBgraToFloat4(frame.unlitDiffuseBgra, frame.unlitDiffuseColor);
                                Gfx_LightVertexDiffuse((GfxFloorVertex *)lightSet->floorVertex, frame.lightRecord->normal, (GfxFloatColorAddend *)frame.unlitDiffuseColor, &frame.unlitDiffuseBgra, (struct GfxVertexColor *)((int)frame.entryCursor + 0x10));
                                frame.entryCursor++;
                                frame.lightRecord++;
                            }
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.groupIndex, polySet, frame.engineContext);
                }
            }
            break;
        case GFX_LIGHT_MODE_SPECULAR:
            Gfx_UpdateCachedSpecularColor((struct GfxSpecularState *)lightSet);
            frame.specularFunc = g_FieldSpecularDispatchTable[lightSet->specularIndex];
            for (frame.groupIndex = 0; frame.groupIndex < polySet->groupCount; frame.groupIndex++) {
                frame.polyGroup = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.groupIndex, polySet, frame.engineContext);
                if (frame.polyGroup != 0) {
                    frame.specularLightRecord = (FieldPolygonLightingRecord *)frame.polyGroup->lightRecords;
                    if (frame.specularLightRecord != 0) {
                        frame.specularCursor = (FieldLightVertex *)frame.polyGroup->vertices;
                        for (frame.specularRemaining = frame.polyGroup->elementCount; frame.specularRemaining > 0; frame.specularRemaining--) {
                            frame.specularTarget = frame.specularCursor;
                            frame.specularFunc(frame.specularTarget, lightSet);
                            frame.specularCursor++;
                            frame.specularLightRecord++;
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.groupIndex, polySet, frame.engineContext);
                }
            }
            break;
        case GFX_LIGHT_MODE_DIFFUSE_SPECULAR:
            Gfx_UpdateCachedSpecularColor((struct GfxSpecularState *)lightSet);
            frame.specularFunc = g_FieldSpecularDispatchTable[lightSet->specularIndex];
            for (frame.groupIndex = 0; frame.groupIndex < polySet->groupCount; frame.groupIndex++) {
                frame.polyGroup = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.groupIndex, polySet, frame.engineContext);
                if (frame.polyGroup != 0) {
                    frame.groupLightCursor = (FieldPolygonLightingRecord *)frame.polyGroup->lightRecords;
                    if (frame.groupLightCursor != 0) {
                        frame.vertexCursor = (FieldLightVertex *)frame.polyGroup->vertices;
                        if ((lightSet->flags & GFX_LIGHT_FLAG_FLOOR) != 0) {
                            if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                                Gfx_UnpackBgraToFloat4(lightSet->colorBgra, frame.floorLitNormLight);
                                Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.floorLitNormLight);
                                for (frame.elementCount = frame.polyGroup->elementCount; frame.elementCount > 0; frame.elementCount--) {
                                    frame.floorLitBgra = frame.vertexCursor->color;
                                    Gfx_UnpackBgraToFloat4(frame.floorLitBgra, frame.floorLitColor);
                                    Gfx_TransformPointFloorLitColorToBgra(lightSet->colorBgra, (int)frame.floorLitNormLight, (GfxFloorVertex *)lightSet->floorVertex, frame.groupLightCursor->normal, (GfxLitLightColor *)frame.floorLitColor, &frame.floorLitBgra, (struct GfxBgraPixel *)((int)frame.vertexCursor + 0x10));
                                    frame.specularTarget = frame.vertexCursor;
                                    frame.specularFunc(frame.specularTarget, lightSet);
                                    frame.vertexCursor++;
                                    frame.groupLightCursor++;
                                }
                            } else {
                                for (frame.elementCount = frame.polyGroup->elementCount; frame.elementCount > 0; frame.elementCount--) {
                                    frame.floorPlainBgra = frame.vertexCursor->color;
                                    Gfx_UnpackBgraToFloat4(frame.floorPlainBgra, frame.floorPlainColor);
                                    Gfx_TransformPointFloorColorToBgra((GfxFloorVertex *)lightSet->floorVertex, frame.groupLightCursor->normal, (GfxLitLightColor *)frame.floorPlainColor, &frame.floorPlainBgra, (struct GfxBgraPixel *)((int)frame.vertexCursor + 0x10));
                                    frame.specularTarget = frame.vertexCursor;
                                    frame.specularFunc(frame.specularTarget, lightSet);
                                    frame.vertexCursor++;
                                    frame.groupLightCursor++;
                                }
                            }
                        } else if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                            Gfx_UnpackBgraToFloat4(lightSet->colorBgra, frame.litNormLightColor);
                            Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.litNormLightColor);
                            for (frame.elementCount = frame.polyGroup->elementCount; frame.elementCount > 0; frame.elementCount--) {
                                frame.litVertexBgra = frame.vertexCursor->color;
                                Gfx_UnpackBgraToFloat4(frame.litVertexBgra, frame.litFloatColor);
                                Gfx_TransformPointLitColorToBgra(lightSet->colorBgra, (int)frame.litNormLightColor, (GfxFloorVertex *)lightSet->floorVertex, frame.groupLightCursor->normal, (GfxFloatColorAddend *)frame.litFloatColor, &frame.litVertexBgra, (struct GfxBgraPixel *)((int)frame.vertexCursor + 0x10));
                                frame.specularTarget = frame.vertexCursor;
                                frame.specularFunc(frame.specularTarget, lightSet);
                                frame.vertexCursor++;
                                frame.groupLightCursor++;
                            }
                        } else {
                            for (frame.elementCount = frame.polyGroup->elementCount; frame.elementCount > 0; frame.elementCount--) {
                                frame.diffuseVertexBgra = frame.vertexCursor->color;
                                Gfx_UnpackBgraToFloat4(frame.diffuseVertexBgra, frame.diffuseUnpackedColor);
                                Gfx_LightVertexDiffuse((GfxFloorVertex *)lightSet->floorVertex, frame.groupLightCursor->normal, (GfxFloatColorAddend *)frame.diffuseUnpackedColor, &frame.diffuseVertexBgra, (struct GfxVertexColor *)((int)frame.vertexCursor + 0x10));
                                frame.specularTarget = frame.vertexCursor;
                                frame.specularFunc(frame.specularTarget, lightSet);
                                frame.vertexCursor++;
                                frame.groupLightCursor++;
                            }
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.groupIndex, polySet, frame.engineContext);
                }
            }
        }
    }
}

/*
 * Allocates and initializes textured 100-byte polygon slot structures.
 */
// FUNCTION: FF7 0x0064d2e6
int __cdecl Field_AllocTexturedHundredSlots(FieldTexturedAllocRequest *request, FieldTexturedSlots *slots)
{
    GfxEngineContext *ctx;
    PfileLoadOptions loadBits;

    ctx = GetEngineContext();
    Pfile_InitLoadOptions(&loadBits);
    loadBits.directory = (char *)&g_FieldModelVertexLimit;
    if (g_FieldLoadAssets != 0) {
        loadBits.file.fromArchive = 1;
        loadBits.file.archiveId = 1;
        loadBits.file.callback = 0;
    }
    slots->hundredA = 0;
    slots->hundredB = 0;
    slots->hundredC = 0;
    slots->hundredD = 0;
    if (request->enabled != 0) {
        if (request->countA != 0) {
            slots->hundredA = Gfx_AllocTexturedHundred(0, 0, request->countA, (int)&loadBits, (int)ctx);
        } else {
            slots->hundredA = 0;
        }
        if (slots->hundredA != 0 || request->countA == 0) {
            if (request->countB != 0) {
                slots->hundredB = Gfx_AllocTexturedHundred(0, 0, request->countB, (int)&loadBits, (int)ctx);
            } else {
                slots->hundredB = 0;
            }
            if (slots->hundredB != 0 || request->countB == 0) {
                if (request->countC != 0) {
                    slots->hundredC = Gfx_AllocTexturedHundred(0, 0, request->countC, (int)&loadBits, (int)ctx);
                } else {
                    slots->hundredC = 0;
                }
                if (slots->hundredC != 0 || request->countC == 0) {
                    if (request->countD != 0) {
                        slots->hundredD = Gfx_AllocTexturedHundred(0, 0, request->countD, (int)&loadBits, (int)ctx);
                    } else {
                        slots->hundredD = 0;
                    }
                    if (slots->hundredD != 0 || request->countD == 0) {
                        return 1;
                    }
                    Gfx_DestroyTexturedHundred(slots->hundredC);
                }
                Gfx_DestroyTexturedHundred(slots->hundredB);
            }
            Gfx_DestroyTexturedHundred(slots->hundredA);
        }
    }
    return 0;
}

/*
 * Frees textured 100-byte polygon slot structures and clears head mesh flag.
 */
// FUNCTION: FF7 0x0064d4f6
void __cdecl Field_FreeTexturedHundredSlots(FieldTexturedSlots *slots)
{
    if (slots->hundredA != 0) {
        Gfx_DestroyTexturedHundred(slots->hundredA);
        slots->hundredA = 0;
    }
    if (slots->hundredB != 0) {
        Gfx_DestroyTexturedHundred(slots->hundredB);
        slots->hundredB = 0;
    }
    if (slots->hundredC != 0) {
        Gfx_DestroyTexturedHundred(slots->hundredC);
        slots->hundredC = 0;
    }
    if (slots->hundredD != 0) {
        Gfx_DestroyTexturedHundred(slots->hundredD);
        slots->hundredD = 0;
    }
    Field_ClearHeadMeshFlag(slots);
}

/*
 * Clears the group-state flag on the head mesh polygon set if found.
 */
// FUNCTION: FF7 0x0064d5b3
void __cdecl Field_ClearHeadMeshFlag(FieldTexturedSlots *slots)
{
    struct {
        int recordIndex;
        FieldHeadBoneRecord *record;
        GfxPolygonSet *mesh;
        FieldHeadResource *resource;
        FieldHeadPlayer *player;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.player = slots->headPlayer;
    if (frame.player != 0) {
        frame.resource = frame.player->resource;
        if (frame.resource != 0) {
            frame.recordIndex = 0;
            frame.record = frame.player->resource->records;
            while (frame.recordIndex < frame.resource->recordCount && _stricmp(frame.record->name,
                                                                               // STRING: FF7 0x0090928c
                                                                               "head") != 0) {
                frame.recordIndex++;
                frame.record++;
            }
            // STRING: FF7 0x00909294
            (void)"FPS: %.2f";
            // STRING: FF7 0x009092a0
            (void)"Frame: %.2d";
            if (frame.recordIndex == frame.resource->recordCount)
                return;
            frame.mesh = (GfxPolygonSet *)frame.record->link->next->next;
            D3DLinkPolygonSetGroupsIdentity((void *)frame.mesh);
            frame.mesh->useGroupStates = 0;
        }
    }
}
} /* extern "C" */

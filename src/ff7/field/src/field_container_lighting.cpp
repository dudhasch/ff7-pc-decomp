// src/ff7/field/src/field_container_lighting.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Applies lighting to the field's containers: sets the lighting flag, applies cached
//   vertex lighting or computes point lights per polygon set, and evaluates curve steps that update
//   the lighting.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00d85178
int g_FieldLightStepIndex = 0;
unsigned char g_bssPad_00d8517c[4] = {0};
// GLOBAL: FF7 0x00d85180
int g_FieldLightContext = 0;
// GLOBAL: FF7 0x00d85184
int g_FieldLightContextCount = 0;
// GLOBAL: FF7 0x00d85188
struct GfxLight *g_FieldLightContextPtr0 = 0;
// GLOBAL: FF7 0x00d8518c
struct GfxLight *g_FieldLightContextPtr1 = 0;
// GLOBAL: FF7 0x00d85190
struct GfxLight *g_FieldLightContextPtr2 = 0;
// GLOBAL: FF7 0x00d85194
float g_FieldLightAmbientColorRaw = 0;
unsigned char g_bssPad_00d85198[12] = {0};
// GLOBAL: FF7 0x00d851a4
float g_FieldLightAmbientColorNorm = 0;
unsigned char g_bssPad_00d851a8[12] = {0};
// GLOBAL: FF7 0x00d851b4
int g_FieldLightAmbientColorBgra = 0;
unsigned char g_bssPad_00d851b8[192] = {0};
// GLOBAL: FF7 0x00d85278
unsigned char g_FieldLightVertexColorBuffer[0x4] = {0};
unsigned char g_bssPad_00d8527c[4] = {0};
unsigned char g_bssPad_00d85280[56] = {0};
// GLOBAL: FF7 0x00d852b8
int g_FieldLightMatrixFlags = 0;
// GLOBAL: FF7 0x00d852bc
unsigned char g_FieldLightMatrixIdentity[0x4] = {0};
unsigned char g_bssPad_00d852c0[64] = {0};
// GLOBAL: FF7 0x00d85300
int g_FieldLightColorAlphaMask = 0;
unsigned char g_bssPad_00d85304[4] = {0};
unsigned char g_bssPad_00d85308[1064] = {0};
// GLOBAL: FF7 0x00d85730
int g_FieldLight0_Type = 0;
// GLOBAL: FF7 0x00d85734
int g_FieldLight0_Color = 0;
// GLOBAL: FF7 0x00d85738
float g_FieldLight0_DirX = 0;
// GLOBAL: FF7 0x00d8573c
float g_FieldLight0_DirY = 0;
// GLOBAL: FF7 0x00d85740
float g_FieldLight0_DirZ = 0;
// GLOBAL: FF7 0x00d85744
float g_FieldLight0_ColorNorm = 0;
unsigned char g_bssPad_00d85748[12] = {0};
// GLOBAL: FF7 0x00d85754
int g_FieldLight1_Type = 0;
// GLOBAL: FF7 0x00d85758
int g_FieldLight1_Color = 0;
// GLOBAL: FF7 0x00d8575c
float g_FieldLight1_DirX = 0;
// GLOBAL: FF7 0x00d85760
float g_FieldLight1_DirY = 0;
// GLOBAL: FF7 0x00d85764
float g_FieldLight1_DirZ = 0;
// GLOBAL: FF7 0x00d85768
float g_FieldLight1_ColorNorm = 0;
unsigned char g_bssPad_00d8576c[4] = {0};
unsigned char g_bssPad_00d85770[8] = {0};
// GLOBAL: FF7 0x00d85778
int g_FieldLight2_Type = 0;
// GLOBAL: FF7 0x00d8577c
int g_FieldLight2_Color = 0;
// GLOBAL: FF7 0x00d85780
float g_FieldLight2_DirX = 0;
// GLOBAL: FF7 0x00d85784
float g_FieldLight2_DirY = 0;
// GLOBAL: FF7 0x00d85788
float g_FieldLight2_DirZ = 0;
// GLOBAL: FF7 0x00d8578c
float g_FieldLight2_ColorNorm = 0;
unsigned char g_bssPad_00d85790[12] = {0};
// GLOBAL: FF7 0x00d8579c
int g_FieldLightSetupFlag = 0;
// clang-format on
#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Sets lighting setup flag and walks the two-level container to apply cached vertex lighting.
 */
// FUNCTION: FF7 0x0064fb20
int __cdecl Field_UpdateContainerLighting(FieldModelInstance *model, FieldLightSource *light, FieldModelCommandRecord *command, int bankIndex, int subIndex, int useCache)
{
    FieldModelAlphaGroups *animationDraw;
    FieldDiffuseSelectorGroup *group;
    GfxPolygonSet *pPolygonSet;
    int entIndex;
    FieldSelectorEntry *subEntry;
    int groupIndex;

    animationDraw = (FieldModelAlphaGroups *)model->skeleton;
    g_FieldLightSetupFlag = 1;
    if (animationDraw != 0) {
        group = animationDraw->groups;
        for (groupIndex = 0; groupIndex < animationDraw->groupCount; groupIndex++) {
            if (group->entryCount > 0) {
                subEntry = group->records;
                if (subEntry != 0) {
                    for (entIndex = 0; entIndex < group->entryCount; entIndex++) {
                        if (subEntry->polySetHolder != 0) {
                            pPolygonSet = subEntry->polySetHolder->polySet;
                            Field_ApplyCachedVertexLighting(pPolygonSet, light, bankIndex, subIndex, useCache);
                        }
                        subEntry++;
                    }
                }
            }
            group++;
        }
    }
    return 1;
}

/*
 * Applies cached vertex lighting or computes point lighting for each polygon set group.
 */
// FUNCTION: FF7 0x0064fbf4
void __cdecl Field_ApplyCachedVertexLighting(GfxPolygonSet *polySet, FieldLightSource *light, int bankIndex, int subIndex, int useCache)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *group;
    FieldLightPosition *dst;
    GfxLightVertexRecord *lightSrc;
    int groupNo;
    int vertexIndex;

    if (g_FieldLightSetupFlag != 0) {
        g_FieldLightStepIndex = 0;
        g_FieldLightSetupFlag = 0;
    }
    if (polySet != 0) {
        ctx = GetEngineContext();
        for (groupNo = 0; groupNo < polySet->groupCount; groupNo++) {
            group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupNo, polySet, ctx));
            if (group != 0) {
                dst = (FieldLightPosition *)group->vertices;
                lightSrc = (GfxLightVertexRecord *)group->lightRecords;
                if (dst != 0 && lightSrc != 0) {
                    for (vertexIndex = 0; vertexIndex < group->elementCount; vertexIndex++) {
                        if (useCache != 0) {
                            dst->color = *(int *)(g_FieldModelLightCaches[bankIndex].lightCache + subIndex * 0x4B0 + g_FieldLightStepIndex * 4);
                        } else {
                            Field_SetupPointLight(dst, lightSrc, ((GfxPolygonSetRecord *)polySet->matrices)->depthGroup, light, &dst->color);
                            *(int *)(g_FieldModelLightCaches[bankIndex].lightCache + subIndex * 0x4B0 + g_FieldLightStepIndex * 4) = dst->color;
                        }
                        dst++;
                        lightSrc++;
                        g_FieldLightStepIndex++;
                    }
                }
                Gfx_UnlockPolygonSetGroup(groupNo, polySet, ctx);
            }
        }
    }
}

/*
 * Calculates direction and attenuation for a point light and sets up lighting context matrices.
 */
// FUNCTION: FF7 0x0064fd70
void __cdecl Field_SetupPointLight(FieldLightPosition *pos, GfxLightVertexRecord *lightInput, int value, FieldLightSource *light, void *outColor)
{
    int distance;
    short range;
    int zero;
    float offsetZ;
    int lightColor;
    short position16[4];
    float cameraMatrix[4][4];
    float mtxTrans[4][4];
    int cameraPoint[4];
    int localFactor;
    float vecX;
    float deltaY;
    float normColor[4];

    range = light->range;
    position16[0] = (short)pos->x;
    position16[1] = (short)pos->y;
    position16[2] = (short)pos->z;
    range += 1;
    if (range == 0) {
        range = 1;
    }
    ApplyMatrixToSVectorInt32(g_FieldCameraTransform, (SVECTOR *)position16, cameraPoint);
    cameraPoint[0] += ((PsxMatrix *)g_FieldCameraTransform)->t[0];
    cameraPoint[1] += ((PsxMatrix *)g_FieldCameraTransform)->t[1];
    cameraPoint[2] += ((PsxMatrix *)g_FieldCameraTransform)->t[2];
    vecX = (float)(light->posX - cameraPoint[0]);
    deltaY = (float)(light->posY - cameraPoint[1]);
    offsetZ = (float)(light->posZ - cameraPoint[2]);
    distance = SqrtToInt((int)(vecX * vecX + deltaY * deltaY + offsetZ * offsetZ)) + 2;
    vecX = (float)((int)vecX << 0xC) / distance;
    deltaY = (float)((int)deltaY << 0xC) / distance;
    offsetZ = (float)((int)offsetZ << 0xC) / distance;
    localFactor = ((range - distance < 0 ? 0 : range - distance) << 0xC) / range;
    vecX = (float)((int)(localFactor * vecX) >> 0xC);
    deltaY = (float)((int)(localFactor * deltaY) >> 0xC);
    offsetZ = (float)((int)(localFactor * offsetZ) >> 0xC);
    g_FieldLight0_Type = 0;
    g_FieldLight0_Color = light->light0Color;
    g_FieldLight0_DirX = vecX;
    g_FieldLight0_DirY = deltaY;
    g_FieldLight0_DirZ = offsetZ;
    Gfx_UnpackBgraToFloat4(light->light0Color, &g_FieldLight0_ColorNorm);
    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)&g_FieldLight0_ColorNorm);
    vecX = 0;
    deltaY = 0;
    offsetZ = 0;
    zero = 0;
    g_FieldLight1_Type = 0;
    g_FieldLight1_Color = zero;
    g_FieldLight1_DirX = vecX;
    g_FieldLight1_DirY = deltaY;
    g_FieldLight1_DirZ = offsetZ;
    Gfx_UnpackBgraToFloat4(zero, &g_FieldLight1_ColorNorm);
    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)&g_FieldLight1_ColorNorm);
    g_FieldLight2_Type = 0;
    g_FieldLight2_Color = zero;
    g_FieldLight2_DirX = vecX;
    g_FieldLight2_DirY = deltaY;
    g_FieldLight2_DirZ = offsetZ;
    Gfx_UnpackBgraToFloat4(zero, &g_FieldLight2_ColorNorm);
    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)&g_FieldLight2_ColorNorm);
    g_FieldLightContext = 9;
    g_FieldLightContextCount = 3;
    g_FieldLightContextPtr0 = (struct GfxLight *)&g_FieldLight0_Type;
    g_FieldLightContextPtr1 = (struct GfxLight *)&g_FieldLight1_Type;
    g_FieldLightContextPtr2 = (struct GfxLight *)&g_FieldLight2_Type;
    g_FieldLightAmbientColorBgra = light->ambientColor;
    Gfx_UnpackBgraToFloat4(light->ambientColor, &g_FieldLightAmbientColorRaw);
    Gfx_LightNormalizeColorCopy((GfxNormalizedColor *)&g_FieldLightAmbientColorRaw, (GfxFloatColor *)&g_FieldLightAmbientColorNorm);
    Gfx_LightRebuildCachedMatrices((GfxLightOwner *)&g_FieldLightContext);
    g_FieldLightMatrixFlags = 0;
    SetIdentityMatrix4x4(&g_FieldLightMatrixIdentity);
    g_FieldLightColorAlphaMask = 0xFF000000;
    GtePsxMatrixToFloat4x4Transposed(g_FieldCameraTransform, cameraMatrix);
    TransposeMatrix4x4((struct GfxTransposedMatrix *)cameraMatrix, (struct GfxTransposedMatrix *)mtxTrans);
    Gfx_LightApplyTransposedMatrixToCachedProduct((PolygonTransform *)mtxTrans, (GfxLightOwner *)&g_FieldLightContext);
    lightColor = lightInput->packedColor;
    Gfx_UnpackBgraToFloat4(lightColor, normColor);
    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)normColor);
    Gfx_TransformPointFloorLitColorToBgra(lightColor, (int)normColor, (GfxFloorVertex *)&g_FieldLightVertexColorBuffer, lightInput->tag, (GfxLitLightColor *)lightInput->color, &lightInput->packedColor, (struct GfxBgraPixel *)&pos->color);
}

/*
 * Evaluates a parametric curve using trig tables and updates container lighting at each step.
 */
// FUNCTION: FF7 0x0065019e
int __cdecl Field_EvaluateCurveSteps(FieldModelInstance *container, FieldLightSource *lightPosition, FieldModelCommandRecord *command, int mode, FieldCurveStepRecord *curve)
{
    short shiftY;
    short shiftZ;
    short phaseDelta;
    short shiftX;
    short shiftBy;
    short sourceVec[4];
    int res[4];
    short offset;
    short magnitude;
    int i;
    short phase;

    i = 0;
    magnitude = curve->magnitude;
    shiftX = curve->shiftX;
    shiftY = curve->shiftY;
    shiftZ = curve->shiftZ;
    shiftBy = curve->shiftBy;
    offset = curve->offset;
    phaseDelta = curve->phaseDelta;
    while ((phase = i * phaseDelta) <= 0x120) {
        sourceVec[0] = (g_FieldTrigTable.v[phase & 0xFF][1] >> shiftX) * magnitude;
        sourceVec[1] = (-g_FieldTrigTable.v[phase & 0xFF][0] >> shiftY) * magnitude;
        sourceVec[2] = ((g_FieldTrigTable.v[(phase >> shiftBy) & 0xFF][0] >> shiftZ) + offset) * magnitude;
        ApplyMatrixToSVectorInt32(g_FieldCameraTransform, (SVECTOR *)sourceVec, res);
        lightPosition->posX = res[0] + ((PsxMatrix *)g_FieldCameraTransform)->t[0];
        lightPosition->posY = res[1] + ((PsxMatrix *)g_FieldCameraTransform)->t[1];
        lightPosition->posZ = res[2] + ((PsxMatrix *)g_FieldCameraTransform)->t[2];
        Field_UpdateContainerLighting(container, lightPosition, command, mode, i, 0);
        i++;
    }
    return 1;
}
} /* extern "C" */

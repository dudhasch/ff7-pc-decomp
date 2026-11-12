// src/ff7/field/src/field_indexed_record_command.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Runs a field model command on its runtime record: the setup step loads the record
//   from a packet, the draw step applies it as a model effect and draws splash quads where the
//   model's bones cross the water height.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include <stdlib.h>
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "ff7/api/main.h"
#include "lib/pfile.h"
#include "ff7/field.h"
#include "ff7/enums/field.h"
#include "ff7/enums/main.h"
#include "lib/types/graphics.h"
extern "C" {

#pragma intrinsic(abs)
// clang-format off
// GLOBAL: FF7 0x00907f60
float g_FieldSplashHeight = 1.85f;
// GLOBAL: FF7 0x00cfff00
void * g_pFieldSplashTexture[4] = {0};
// GLOBAL: FF7 0x00cfff10
float g_FieldSplashScaleX = 0.0f;
// GLOBAL: FF7 0x00cfff14
int g_FieldSplashTextureSlot = 0;
// clang-format on

/* The vertices of the primitives AcquireBatchNode reserved last on the splash render object in the current texture slot. */
#define SPLASH_VERTICES (((GfxRenderObject *)g_pFieldSplashTexture[g_FieldSplashTextureSlot])->vertexCursor)
/* Vertex colour (white, alpha 0x80) and specular (black, alpha 0xFF) of every splash vertex. */
#define SPLASH_COLOR 0x80FFFFFF
#define SPLASH_SPECULAR 0xFF000000

/* Transforms the bone's origin by its chain matrix into probeX/Y/Z; sampleZ is the probe's Z. */
#define SPLASH_PROBE_BONE_ORIGIN()                                                                        \
    frame.localPoint.xBits = 0;                                                                           \
    frame.localPoint.yBits = 0;                                                                           \
    frame.localPoint.zBits = 0;                                                                           \
    MultiplyMatrix4x4Full(frame.chainTransform, &g_FieldModelRenderState, &frame.probeMatrix); \
    TransformPoint3RowVec(&frame.probeMatrix, &frame.localPoint, &frame.probe);                           \
    frame.sampleZ = frame.probe.z

/*
 * FIELD_SPLASH_COMMAND_SETUP initializes an indexed 60-byte field record from four little-endian
 * shorts and a byte; FIELD_SPLASH_COMMAND_DRAW serializes its values and calls
 * Field_ApplyModelEffectPacket, then conditionally visits attached 28-byte model entries. Projects
 * two points and, for g_FieldRenderStateMode one through five (one per splash field), computes a mode-specific height intersection and emits a textured quad when its
 * ratio lies in zero through one and batch acquisition succeeds. Mode two uses screen depth zero
 * and a vertically offset quad; other modes use one minus interpolated depth. Rotates among four
 * texture slots after successful emission. Every call advances the selected record animation
 * counters and returns zero.
 */
// FUNCTION: FF7 0x00645150
int __cdecl Field_ApplyModelSplashCommand(FieldModelInstance *modelRecord, FieldModelCommandRecord *commandRecord)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[432];
        GfxTlVertex *depthQuadVertex3;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *depthQuadVertex2; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *depthQuadVertex1; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *depthQuadVertex0; };
        struct { unsigned char pad_to_10[0x10]; GfxTlVertex *flatQuadVertex3; };
        struct { unsigned char pad_to_14[0x14]; GfxTlVertex *flatQuadVertex2; };
        struct { unsigned char pad_to_18[0x18]; GfxTlVertex *flatQuadVertex1; };
        struct { unsigned char pad_to_1C[0x1C]; GfxTlVertex *flatQuadVertex0; };
        struct { unsigned char pad_to_20[0x20]; float depth; };
        struct { unsigned char pad_to_24[0x24]; short centerX; };
        struct { unsigned char pad_to_26[0x26]; short centerY; };
        struct { unsigned char pad_to_2C[0x2C]; float ratio; };
        struct { unsigned char pad_to_30[0x30]; float quadSize; };
        struct { unsigned char pad_to_34[0x34]; short halfSize; };
        struct { unsigned char pad_to_38[0x38]; GfxVec3Dual input; };
        struct { unsigned char pad_to_44[0x44]; GfxHomogeneousVec4 offsetScreen; };
        struct { unsigned char pad_to_54[0x54]; GfxHomogeneousVec4 baseScreen; };
        struct { unsigned char pad_to_64[0x64]; GfxHomogeneousVec4 offset; };
        struct { unsigned char pad_to_74[0x74]; GfxHomogeneousVec4 origin; };
        struct { unsigned char pad_to_84[0x84]; int commandIndex; };
        struct { unsigned char pad_to_88[0x88]; GfxMatrix4x4 chainScreenMatrix; };
        struct { unsigned char pad_to_C8[0xC8]; PolygonTransform *chainTransform; };
        struct { unsigned char pad_to_CC[0xCC]; FieldIndexedRecord *indexedRecord; };
        struct { unsigned char pad_to_D0[0xD0]; GfxMatrix4x4 chainMatrix; };
        struct { unsigned char pad_to_110[0x110]; FieldModelEffectPacket *effectPacket; };
        struct { unsigned char pad_to_114[0x114]; int command; };
        struct { unsigned char pad_to_118[0x118]; int entityIndex; };
        struct { unsigned char pad_to_11C[0x11C]; FieldModelRuntimeRecord *runtimeRecord; };
        struct { unsigned char pad_to_120[0x120]; FieldAnimDrawCommands *animDraw; };
        struct { unsigned char pad_to_124[0x124]; float intersectHeight; };
        struct { unsigned char pad_to_128[0x128]; float sampleZ; };
        struct { unsigned char pad_to_12C[0x12C]; GfxMatrix4x4 probeMatrix; };
        struct { unsigned char pad_to_16C[0x16C]; int heightLatched; };
        struct { unsigned char pad_to_170[0x170]; short renderMode; };
        struct { unsigned char pad_to_174[0x174]; int culled; };
        struct { unsigned char pad_to_178[0x178]; GfxVec3Dual localPoint; };
        struct { unsigned char pad_to_184[0x184]; FieldHeadTemplate headTemplate; };
        struct { unsigned char pad_to_1A4[0x1A4]; GfxVec3Dual probe; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.heightLatched = 0;
    frame.culled = 0;
    frame.animDraw = (FieldAnimDrawCommands *)modelRecord->skeleton;
    frame.command = commandRecord->command;
    frame.entityIndex = *(unsigned char *)&commandRecord->pHigh;
    /* records of 0x3C bytes; FieldModelRuntimeRecord types their first 0x1C */
    frame.runtimeRecord = (FieldModelRuntimeRecord *)(frame.entityIndex * 0x3C + (char *)&g_FieldModelRuntimeRecords);
    frame.renderMode = frame.runtimeRecord->rampPos;
    if (g_FieldRenderStateMode != 0) {
        /* the mode's 32-byte height template: thresholds, weight, slopes and heights */
        frame.headTemplate = g_FieldHeadTemplates[g_FieldRenderStateMode - 1];
    }
    switch (frame.command) {
    case FIELD_SPLASH_COMMAND_SETUP:
        frame.runtimeRecord->rampPos = ((commandRecord->qHigh << 8) & 0xFF00) | (commandRecord->qLow & 0xFF);
        frame.runtimeRecord->rampCounter = ((commandRecord->rHigh << 8) & 0xFF00) | (commandRecord->rLow & 0xFF);
        frame.runtimeRecord->rampPhase = ((commandRecord->param3High << 8) & 0xFF00) | (commandRecord->param3Low & 0xFF);
        frame.runtimeRecord->rampStep = ((commandRecord->param4High << 8) & 0xFF00) | (commandRecord->param4Low & 0xFF);
        frame.runtimeRecord->animFrame = 0;
        frame.runtimeRecord->animTick = 0;
        *(char *)&frame.runtimeRecord->effectFlags = *(char *)&commandRecord->param5Low;
        break;
    case FIELD_SPLASH_COMMAND_DRAW:
        frame.effectPacket = (FieldModelEffectPacket *)&g_FieldModelRuntimeFlags;
        frame.effectPacket->word0Lo = frame.runtimeRecord->rampCounter & 0xFF;
        frame.effectPacket->word0Hi = (frame.runtimeRecord->rampCounter >> 8) & 0xFF;
        frame.effectPacket->word1Lo = frame.runtimeRecord->rampPhase & 0xFF;
        frame.effectPacket->word1Hi = (frame.runtimeRecord->rampPhase >> 8) & 0xFF;
        frame.effectPacket->word2Lo = frame.runtimeRecord->rampStep & 0xFF;
        frame.effectPacket->word2Hi = (frame.runtimeRecord->rampStep >> 8) & 0xFF;
        frame.effectPacket->word3Lo = frame.runtimeRecord->rampPos & 0xFF;
        frame.effectPacket->word3Hi = (frame.runtimeRecord->rampPos >> 8) & 0xFF;
        frame.effectPacket->word4Lo = *(char *)&frame.runtimeRecord->effectFlags;
        Field_ApplyModelEffectPacket(modelRecord, frame.effectPacket);
        if ((char)g_FieldEntityState[frame.entityIndex].activeAnimId == 0) {
            break;
        }
        if (frame.animDraw != 0) {
            frame.indexedRecord = (FieldIndexedRecord *)frame.animDraw->commands;
            if (frame.indexedRecord != 0) {
                for (frame.commandIndex = 0; frame.commandIndex < frame.animDraw->commandCount; frame.commandIndex++) {
                    if (frame.indexedRecord->chainRef != 0) {
                        /* chainRef -> (+4) -> (+0) -> the bone's transform at +0x14 */
                        frame.chainTransform = frame.indexedRecord->chainRef->model->matrices->matrix4A;
                        MultiplyMatrix4x4Full(frame.chainTransform, &g_FieldModelRenderState, &frame.chainMatrix);
                        ComposeWithScreenMatrix(frame.chainTransform, &frame.chainScreenMatrix);
                        frame.input.xBits = 0;
                        frame.input.yBits = 0;
                        frame.input.z = 0.0f;
                        TransformPointByMatrix4x4(&frame.chainMatrix, &frame.input, &frame.origin);
                        TransformPointByMatrix4x4(&frame.chainScreenMatrix, &frame.input, &frame.baseScreen);
                        frame.baseScreen.w = 1.0f / frame.baseScreen.w;
                        frame.baseScreen.x *= frame.baseScreen.w;
                        frame.baseScreen.y *= frame.baseScreen.w;
                        frame.baseScreen.z *= frame.baseScreen.w;
                        frame.input.xBits = 0;
                        frame.input.yBits = 0;
                        frame.input.z = -(frame.indexedRecord->boneLength + 1.0f);
                        TransformPointByMatrix4x4(&frame.chainMatrix, &frame.input, &frame.offset);
                        TransformPointByMatrix4x4(&frame.chainScreenMatrix, &frame.input, &frame.offsetScreen);
                        frame.offsetScreen.w = 1.0f / frame.offsetScreen.w;
                        frame.offsetScreen.x *= frame.offsetScreen.w;
                        frame.offsetScreen.y *= frame.offsetScreen.w;
                        frame.offsetScreen.z *= frame.offsetScreen.w;
                        if (g_FieldRenderStateMode != 0) {
                            frame.quadSize = 48.0f;
                            switch (g_FieldRenderStateMode) {
                            case FIELD_RENDER_STATE_MAP_1C1:
                                SPLASH_PROBE_BONE_ORIGIN();
                                if (frame.heightLatched == 0) {
                                    frame.heightLatched = 1;
                                    if ((float)frame.headTemplate.upperThreshold > frame.sampleZ) {
                                        g_FieldSplashHeight = frame.headTemplate.rampBase;
                                    } else {
                                        g_FieldSplashHeight = (frame.sampleZ - (float)frame.headTemplate.upperThreshold) * frame.headTemplate.upperSlope + frame.headTemplate.rampBase;
                                    }
                                }
                                if ((float)frame.headTemplate.lowerThreshold > frame.sampleZ) {
                                    frame.intersectHeight = frame.headTemplate.rampFloor;
                                } else {
                                    frame.intersectHeight = (frame.sampleZ - (float)frame.headTemplate.lowerThreshold) * frame.headTemplate.lowerSlope + g_FieldSplashHeight;
                                }
                                frame.intersectHeight = (float)frame.headTemplate.blendAmount * frame.intersectHeight / abs(frame.renderMode) + (float)(frame.renderMode + frame.headTemplate.blendAmount) * frame.headTemplate.blendTarget;
                                break;
                            case FIELD_RENDER_STATE_MAP_058:
                                SPLASH_PROBE_BONE_ORIGIN();
                                if (frame.heightLatched == 0) {
                                    frame.heightLatched = 1;
                                    if (frame.sampleZ < 1.4e+02f) {
                                        *(int *)&frame.headTemplate.rampFloor = 0x3FEA233A;
                                        frame.intersectHeight = frame.headTemplate.rampFloor;
                                    } else {
                                        frame.intersectHeight = (frame.sampleZ - 1.4e+02f) * 0.0122589f + 1.8292f;
                                    }
                                    g_FieldSplashScaleX = frame.intersectHeight;
                                }
                                frame.intersectHeight = g_FieldSplashScaleX - 0.2786f;
                                frame.quadSize = 2e+01f;
                                break;
                            case FIELD_RENDER_STATE_MAP_1AD:
                                SPLASH_PROBE_BONE_ORIGIN();
                                if (frame.heightLatched == 0) {
                                    frame.heightLatched = 1;
                                    if (frame.sampleZ > -165.0f) {
                                        *(int *)&g_FieldSplashHeight = 0x428C0000;
                                    } else {
                                        g_FieldSplashHeight = (frame.sampleZ + 286.0f) * 0.84f + -6e+01f;
                                    }
                                }
                                frame.intersectHeight = g_FieldSplashHeight * 0.11f + -32.0f;
                                frame.intersectHeight = 7.0f * frame.intersectHeight / abs(frame.renderMode);
                                break;
                            case FIELD_RENDER_STATE_MAP_1AE:
                                SPLASH_PROBE_BONE_ORIGIN();
                                if (frame.heightLatched == 0) {
                                    frame.heightLatched = 1;
                                    if (frame.probe.y > -1.35e+03f) {
                                        frame.culled = 1;
                                    }
                                    if (frame.sampleZ < 0.0f) {
                                        if (frame.sampleZ <= -229.0f) {
                                            *(int *)&g_FieldSplashHeight = 0x42040000;
                                        } else {
                                            g_FieldSplashHeight = (frame.sampleZ + 229.0f) * -1.121f + 33.0f;
                                        }
                                    } else {
                                        g_FieldSplashHeight = frame.sampleZ * -1.11f;
                                    }
                                }
                                frame.intersectHeight = 2e+02f + g_FieldSplashHeight + (float)g_FieldSplashHeightBias * 1.0f;
                                if (frame.renderMode != 1 && frame.culled == 0) {
                                    frame.intersectHeight = 1e+03f;
                                }
                                break;
                            case FIELD_RENDER_STATE_MAP_1B3:
                                SPLASH_PROBE_BONE_ORIGIN();
                                frame.intersectHeight = 9.55f;
                                frame.intersectHeight = 16.0f * frame.intersectHeight / abs(frame.renderMode);
                            }
                            if (-frame.sampleZ < (float)frame.renderMode * frame.intersectHeight && frame.culled == 0 && frame.offset.z - frame.origin.z != 0.0f) {
                                frame.ratio = (-((float)frame.renderMode * frame.intersectHeight) - frame.origin.z) / (frame.offset.z - frame.origin.z);
                            // No goto reaches this label. VC5 stores ratio and reloads it after a
                            // label, as the original does; without the label the code differs.
                            rounded_ratio:
                                if (frame.ratio >= 0.0f && frame.ratio <= 1.0f) {
                                    frame.centerX = (int)((frame.offsetScreen.x - frame.baseScreen.x) * frame.ratio + frame.baseScreen.x);
                                    frame.centerY = (int)((frame.offsetScreen.y - frame.baseScreen.y) * frame.ratio + frame.baseScreen.y);
                                    frame.depth = (frame.offsetScreen.z - frame.baseScreen.z) * frame.ratio + frame.baseScreen.z;
                                    if (AcquireBatchNode(1, (void *)g_pFieldSplashTexture[g_FieldSplashTextureSlot]) != 0) {
                                        if (frame.quadSize < 16.0f) {
                                            frame.quadSize = 16.0f;
                                        }
                                        frame.quadSize = frame.quadSize * 0.5;
                                        frame.halfSize = (int)frame.quadSize;
                                        if (GetUiScalingMode() != MAIN_GRAPHICS_MODE_640X480) {
                                            frame.halfSize >>= 1;
                                        }
                                        if (g_FieldRenderStateMode == 2) {
                                            frame.flatQuadVertex0 = (GfxTlVertex *)SPLASH_VERTICES;
                                            frame.flatQuadVertex0->x = (float)frame.centerX - frame.halfSize;
                                            frame.flatQuadVertex0->y = (float)frame.centerY;
                                            frame.flatQuadVertex0->zBits = 0;
                                            frame.flatQuadVertex0->rhwBits = FLOAT_ONE_BITS;
                                            frame.flatQuadVertex0->color = SPLASH_COLOR;
                                            frame.flatQuadVertex0->specular = SPLASH_SPECULAR;
                                            frame.flatQuadVertex0->uBits = 0;
                                            frame.flatQuadVertex0->vBits = 0;
                                            frame.flatQuadVertex1 = (GfxTlVertex *)(SPLASH_VERTICES + 0x20);
                                            frame.flatQuadVertex1->x = (float)frame.centerX - frame.halfSize;
                                            frame.flatQuadVertex1->y = (float)frame.centerY + frame.halfSize + frame.halfSize;
                                            frame.flatQuadVertex1->zBits = 0;
                                            frame.flatQuadVertex1->rhwBits = FLOAT_ONE_BITS;
                                            frame.flatQuadVertex1->color = SPLASH_COLOR;
                                            frame.flatQuadVertex1->specular = SPLASH_SPECULAR;
                                            frame.flatQuadVertex1->uBits = 0;
                                            frame.flatQuadVertex1->vBits = FLOAT_ONE_BITS;
                                            frame.flatQuadVertex2 = (GfxTlVertex *)(SPLASH_VERTICES + 0x40);
                                            frame.flatQuadVertex2->x = (float)frame.centerX + frame.halfSize;
                                            frame.flatQuadVertex2->y = (float)frame.centerY;
                                            frame.flatQuadVertex2->zBits = 0;
                                            frame.flatQuadVertex2->rhwBits = FLOAT_ONE_BITS;
                                            frame.flatQuadVertex2->color = SPLASH_COLOR;
                                            frame.flatQuadVertex2->specular = SPLASH_SPECULAR;
                                            frame.flatQuadVertex2->uBits = FLOAT_ONE_BITS;
                                            frame.flatQuadVertex2->vBits = 0;
                                            frame.flatQuadVertex3 = (GfxTlVertex *)(SPLASH_VERTICES + 0x60);
                                            frame.flatQuadVertex3->x = (float)frame.centerX + frame.halfSize;
                                            frame.flatQuadVertex3->y = (float)frame.centerY + frame.halfSize + frame.halfSize;
                                            frame.flatQuadVertex3->zBits = 0;
                                            frame.flatQuadVertex3->rhwBits = FLOAT_ONE_BITS;
                                            frame.flatQuadVertex3->color = SPLASH_COLOR;
                                            frame.flatQuadVertex3->specular = SPLASH_SPECULAR;
                                            frame.flatQuadVertex3->uBits = FLOAT_ONE_BITS;
                                            frame.flatQuadVertex3->vBits = FLOAT_ONE_BITS;
                                        } else {
                                            frame.depthQuadVertex0 = (GfxTlVertex *)SPLASH_VERTICES;
                                            frame.depthQuadVertex0->x = (float)frame.centerX - frame.halfSize;
                                            frame.depthQuadVertex0->y = (float)frame.centerY - frame.halfSize;
                                            frame.depthQuadVertex0->z = 1.0f - frame.depth;
                                            frame.depthQuadVertex0->rhwBits = FLOAT_ONE_BITS;
                                            frame.depthQuadVertex0->color = SPLASH_COLOR;
                                            frame.depthQuadVertex0->specular = SPLASH_SPECULAR;
                                            frame.depthQuadVertex0->uBits = 0;
                                            frame.depthQuadVertex0->vBits = 0;
                                            frame.depthQuadVertex1 = (GfxTlVertex *)(SPLASH_VERTICES + 0x20);
                                            frame.depthQuadVertex1->x = (float)frame.centerX - frame.halfSize;
                                            frame.depthQuadVertex1->y = (float)frame.centerY + frame.halfSize;
                                            frame.depthQuadVertex1->z = 1.0f - frame.depth;
                                            frame.depthQuadVertex1->rhwBits = FLOAT_ONE_BITS;
                                            frame.depthQuadVertex1->color = SPLASH_COLOR;
                                            frame.depthQuadVertex1->specular = SPLASH_SPECULAR;
                                            frame.depthQuadVertex1->uBits = 0;
                                            frame.depthQuadVertex1->vBits = FLOAT_ONE_BITS;
                                            frame.depthQuadVertex2 = (GfxTlVertex *)(SPLASH_VERTICES + 0x40);
                                            frame.depthQuadVertex2->x = (float)frame.centerX + frame.halfSize;
                                            frame.depthQuadVertex2->y = (float)frame.centerY - frame.halfSize;
                                            frame.depthQuadVertex2->z = 1.0f - frame.depth;
                                            frame.depthQuadVertex2->rhwBits = FLOAT_ONE_BITS;
                                            frame.depthQuadVertex2->color = SPLASH_COLOR;
                                            frame.depthQuadVertex2->specular = SPLASH_SPECULAR;
                                            frame.depthQuadVertex2->uBits = FLOAT_ONE_BITS;
                                            frame.depthQuadVertex2->vBits = 0;
                                            frame.depthQuadVertex3 = (GfxTlVertex *)(SPLASH_VERTICES + 0x60);
                                            frame.depthQuadVertex3->x = (float)frame.centerX + frame.halfSize;
                                            frame.depthQuadVertex3->y = (float)frame.centerY + frame.halfSize;
                                            frame.depthQuadVertex3->z = 1.0f - frame.depth;
                                            frame.depthQuadVertex3->rhwBits = FLOAT_ONE_BITS;
                                            frame.depthQuadVertex3->color = SPLASH_COLOR;
                                            frame.depthQuadVertex3->specular = SPLASH_SPECULAR;
                                            frame.depthQuadVertex3->uBits = FLOAT_ONE_BITS;
                                            frame.depthQuadVertex3->vBits = FLOAT_ONE_BITS;
                                        }
                                        g_FieldSplashTextureSlot = (g_FieldSplashTextureSlot + 1) % 4;
                                    }
                                }
                            }
                        }
                    }
                    frame.indexedRecord++;
                }
            }
        }
    }
    /* the splash command counts animTick 0..2 and steps animFrame 0..3 with it (an animation frame) */
    (frame.runtimeRecord->animTick)++;
    if (frame.runtimeRecord->animTick >= 3) {
        frame.runtimeRecord->animTick = 0;
        (frame.runtimeRecord->animFrame)++;
        if (frame.runtimeRecord->animFrame >= 4) {
            frame.runtimeRecord->animFrame = 0;
        }
    }
    return 0;
}
} /* extern "C" */

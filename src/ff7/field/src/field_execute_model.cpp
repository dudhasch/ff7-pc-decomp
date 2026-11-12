// src/ff7/field/src/field_execute_model.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Executes one field model command, updating the model's polygon state, movement,
//   animation and editor controls.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/enums/field.h"
#include "ff7/types/field.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Executes a field model command, updating polygon state, movement, animation, and editor commands.
 */
// FUNCTION: FF7 0x0064a070
int __cdecl Field_ExecuteModelCommand(int command, FieldModelInstance *modelRecord, FieldModelCommandRecord *commandRecord, int rampStep, int rampCount, int modelIndex, int *resultPtr)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[1472];
        int polySetRoot;
        struct { unsigned char pad_to_4[0x4]; int tmpFlag; };
        struct { unsigned char pad_to_8[0x8]; unsigned char *runtimeFlags; };
        struct { unsigned char pad_to_C[0xC]; FieldRuntimeFlags *runtimeEntry; };
        struct { unsigned char pad_to_10[0x10]; int flagSetIndex; };
        struct { unsigned char pad_to_14[0x14]; FieldModelCommandRecord *runtimeCommand; };
        struct { unsigned char pad_to_18[0x18]; FieldModelCommand *commandBlock; };
        struct { unsigned char pad_to_1C[0x1C]; FieldModelCommandPacket *commandPacket; };
        struct { unsigned char pad_to_20[0x20]; GfxMatrix4x4 lightMatrixC; };
        struct { unsigned char pad_to_60[0x60]; GfxMatrix4x4 lightMatrixB; };
        struct { unsigned char pad_to_A0[0xA0]; unsigned char *recordFlags; };
        struct { unsigned char pad_to_A4[0xA4]; int indexedResult; };
        struct { unsigned char pad_to_A8[0xA8]; int lightOwner; };
        struct { unsigned char pad_to_AC[0xAC]; unsigned char *indexedFlags; };
        struct { unsigned char pad_to_B0[0xB0]; unsigned char *rampFlags; };
        struct { unsigned char pad_to_B4[0xB4]; int rampResult; };
        struct { unsigned char pad_to_B8[0xB8]; GfxMatrix4x4 lightMatrixA; };
        struct { unsigned char pad_to_F8[0xF8]; int setIndex; };
        struct { unsigned char pad_to_FC[0xFC]; int polySetHead; };
        struct { unsigned char pad_to_100[0x100]; PolygonModel *animModel; };
        struct {
            unsigned char pad_to_104[0x104];
            int setCount;
            int setRestart;
            FieldPolygonSetCountEntry setEntries[100];
        };
        struct { unsigned char pad_to_5BC[0x5BC]; int tmpLightOwner; };
    } frame;
#pragma pack(pop)

    frame.setCount = 0;
    frame.tmpLightOwner = (int)&frame.setCount;
    frame.animModel = (PolygonModel *)modelRecord->skeleton;
    Gfx_ForEachModelPolygonSet(frame.tmpLightOwner, (GfxPolygonSetFn)Field_AppendModelPolygonSetRecord, (PolygonModel *)frame.animModel);
    if (((int *)&g_FieldModelCommandGate)[modelIndex] != 0 && command == FIELD_MODEL_COMMAND_RAMP_CYCLE) {
        g_FieldModelLightCaches[modelIndex].isActive = 1;
        g_FieldCurveStepFlagTable[modelIndex] = 1;
        Mem_FillBytes(0, 0x7530, (int)g_FieldModelLightCaches[modelIndex].lightCache);
    }
    if ((char)g_FieldScriptExecFlags[modelIndex] == 2 || (char)g_FieldScriptExecFlags[modelIndex] == 1) {
        if (command != FIELD_MODEL_COMMAND_RAMP_CYCLE) {
            command = FIELD_MODEL_COMMAND_RAMP_CYCLE;
            g_FieldScriptExecFlags[modelIndex] = 0xFF;
        }
    }
    if (command == FIELD_MODEL_COMMAND_DONE) {
        for (frame.setIndex = 0; frame.setIndex < frame.setCount; frame.setIndex++) {
            frame.setEntries[frame.setIndex].appended = 1;
        }
        Gfx_ForEachModelPolygonSet(frame.polySetHead, (GfxPolygonSetFn)Field_WriteTrueFlag, (PolygonModel *)frame.animModel);
        *resultPtr = command;
        return 1;
    } else {
        switch (command) {
        case FIELD_MODEL_COMMAND_NONE:
            command = FIELD_MODEL_COMMAND_DONE;
            *resultPtr = command;
            return 1;
        case FIELD_MODEL_COMMAND_RAMP_DELTA:
            frame.rampFlags = g_FieldModelCommandFlags;
            MultiplyMatrix4x4Full(&g_FieldModelContextMatrix, &g_FieldModelRenderState, &frame.lightMatrixA);
            Gfx_LightApplyTransposedMatrixToCachedProduct((PolygonTransform *)&frame.lightMatrixA, g_FieldModelLightData[modelIndex]);
            if (commandRecord->command == 0) {
                frame.rampFlags[modelIndex * 2 + 2] = 0;
                frame.rampFlags[modelIndex * 2 + 3] = modelIndex;
                commandRecord->pHigh = modelIndex;
                frame.rampResult = Field_RampModelDeltaToTarget(frame.animModel, (unsigned char *)commandRecord);
                g_FieldModelLightData[modelIndex]->flags |= 1;
                frame.tmpLightOwner = (int)g_FieldModelLightData[modelIndex];
                Gfx_ForEachModelPolygonSet(frame.tmpLightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingUngatedCallback, (PolygonModel *)frame.animModel);
                g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
                *resultPtr = command;
                return frame.rampResult;
            }
            if (frame.rampFlags[modelIndex * 2 + 2] == 1) {
                frame.rampResult = Field_RampModelDeltaToTarget(frame.animModel, &frame.rampFlags[modelIndex * 2 + 2]);
                g_FieldModelLightData[modelIndex]->flags |= 1;
                frame.tmpLightOwner = (int)g_FieldModelLightData[modelIndex];
                Gfx_ForEachModelPolygonSet(frame.tmpLightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingUngatedCallback, (PolygonModel *)frame.animModel);
                g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
                *resultPtr = command;
                return frame.rampResult;
            }
            if (commandRecord->command == 1) {
                frame.rampFlags[modelIndex * 2 + 2] = 1;
                frame.rampFlags[modelIndex * 2 + 3] = modelIndex;
                frame.rampResult = Field_RampModelDeltaToTarget(frame.animModel, &frame.rampFlags[modelIndex * 2 + 2]);
                g_FieldModelLightData[modelIndex]->flags |= 1;
                frame.tmpLightOwner = (int)g_FieldModelLightData[modelIndex];
                Gfx_ForEachModelPolygonSet(frame.tmpLightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingUngatedCallback, (PolygonModel *)frame.animModel);
                g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
                *resultPtr = command;
                return frame.rampResult;
            }
            break;
        case FIELD_MODEL_COMMAND_INDEXED_MATRIX:
            frame.indexedFlags = g_FieldModelCommandFlags;
            if (commandRecord->command == 0) {
                frame.indexedFlags[modelIndex * 2 + 2] = 0;
                frame.indexedFlags[modelIndex * 2 + 3] = modelIndex;
                commandRecord->pHigh = modelIndex;
                *resultPtr = command;
                return Field_ApplyIndexedModelMatrixRecord((PolygonModel *)frame.animModel, (unsigned char *)commandRecord);
            }
            if (frame.indexedFlags[modelIndex * 2 + 2] == 1) {
                *resultPtr = command;
                return Field_ApplyIndexedModelMatrixRecord((PolygonModel *)frame.animModel, &frame.indexedFlags[modelIndex * 2 + 2]);
            }
            if (commandRecord->command == 1) {
                frame.indexedFlags[modelIndex * 2 + 2] = 1;
                frame.indexedFlags[modelIndex * 2 + 3] = modelIndex;
                *resultPtr = command;
                return Field_ApplyIndexedModelMatrixRecord((PolygonModel *)frame.animModel, &frame.indexedFlags[modelIndex * 2 + 2]);
            }
            break;
        case FIELD_MODEL_COMMAND_SPLASH_A:
        case FIELD_MODEL_COMMAND_SPLASH_B:
            g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
            frame.animModel->drawEnabled = 0;
            frame.animModel->flags |= 0x400;
            Anim_DrawSkeletonFrame(modelRecord->frameIndex, (PolygonSkeletonAnim *)(modelRecord->skeletonAnim), (PolygonBoneTransform *)((int)modelRecord + 0x38), (PolygonAnimDraw *)(modelRecord->skeleton), (GfxEngineContext *)modelRecord->drawContext);
            frame.animModel->drawEnabled = 1;
            frame.recordFlags = g_FieldModelCommandFlags;
            if (commandRecord->command == 0) {
                frame.recordFlags[modelIndex * 2 + 2] = 0;
                frame.recordFlags[modelIndex * 2 + 3] = modelIndex;
                commandRecord->pHigh = modelIndex;
                frame.indexedResult = Field_ApplyModelSplashCommand(modelRecord, commandRecord);
                g_FieldModelLightData[modelIndex]->flags |= 1;
                frame.lightOwner = (int)g_FieldModelLightData[modelIndex];
                if (g_FieldRenderStateMode == 1) {
                    Gfx_ForEachModelPolygonSet(frame.lightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingGatedCallback, (PolygonModel *)frame.animModel);
                } else {
                    Gfx_ForEachModelPolygonSet(frame.lightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingUngatedCallback, (PolygonModel *)frame.animModel);
                }
                g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
                *resultPtr = command;
                return frame.indexedResult;
            }
            if (frame.recordFlags[modelIndex * 2 + 2] == 1) {
                frame.indexedResult = Field_ApplyModelSplashCommand(modelRecord, (FieldModelCommandRecord *)&frame.recordFlags[modelIndex * 2 + 2]);
                g_FieldModelLightData[modelIndex]->flags |= 1;
                frame.lightOwner = (int)g_FieldModelLightData[modelIndex];
                if (g_FieldRenderStateMode == 1) {
                    Gfx_ForEachModelPolygonSet(frame.lightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingGatedCallback, (PolygonModel *)frame.animModel);
                } else {
                    Gfx_ForEachModelPolygonSet(frame.lightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingUngatedCallback, (PolygonModel *)frame.animModel);
                }
                g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
                *resultPtr = command;
                return frame.indexedResult;
            }
            if (commandRecord->command == 1) {
                frame.recordFlags[modelIndex * 2 + 2] = 1;
                frame.recordFlags[modelIndex * 2 + 3] = modelIndex;
                frame.indexedResult = Field_ApplyModelSplashCommand(modelRecord, (FieldModelCommandRecord *)&frame.recordFlags[modelIndex * 2 + 2]);
                g_FieldModelLightData[modelIndex]->flags |= 1;
                frame.lightOwner = (int)g_FieldModelLightData[modelIndex];
                if (g_FieldRenderStateMode == 1) {
                    Gfx_ForEachModelPolygonSet(frame.lightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingGatedCallback, (PolygonModel *)frame.animModel);
                } else {
                    Gfx_ForEachModelPolygonSet(frame.lightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingUngatedCallback, (PolygonModel *)frame.animModel);
                }
                g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
                *resultPtr = command;
                return frame.indexedResult;
            }
            break;
        case FIELD_MODEL_COMMAND_RELIGHT:
            MultiplyMatrix4x4Full(&g_FieldModelContextMatrix, &g_FieldModelRenderState, &frame.lightMatrixB);
            Gfx_LightApplyTransposedMatrixToCachedProduct((PolygonTransform *)&frame.lightMatrixB, g_FieldModelLightData[modelIndex]);
            g_FieldModelLightData[modelIndex]->flags |= 1;
            frame.tmpLightOwner = (int)g_FieldModelLightData[modelIndex];
            Gfx_ForEachModelPolygonSet(frame.tmpLightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingUngatedCallback, (PolygonModel *)frame.animModel);
            g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
            command = FIELD_MODEL_COMMAND_DONE;
            *resultPtr = command;
            return 1;
        case FIELD_MODEL_COMMAND_DELTA_PACKET:
            MultiplyMatrix4x4Full(&g_FieldModelContextMatrix, &g_FieldModelRenderState, &frame.lightMatrixC);
            Gfx_LightApplyTransposedMatrixToCachedProduct((PolygonTransform *)&frame.lightMatrixC, g_FieldModelLightData[modelIndex]);
            Field_ApplyModelDeltaPacket((PolygonModel *)frame.animModel, commandRecord);
            g_FieldModelLightData[modelIndex]->flags |= 1;
            frame.tmpLightOwner = (int)g_FieldModelLightData[modelIndex];
            Gfx_ForEachModelPolygonSet(frame.tmpLightOwner, (GfxPolygonSetFn)Field_ApplyPolygonSetLightingUngatedCallback, (PolygonModel *)frame.animModel);
            g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
            command = FIELD_MODEL_COMMAND_DONE;
            *resultPtr = command;
            return 1;
        case FIELD_MODEL_COMMAND_MATRIX_PACKET:
            Field_ApplyModelMatrixPacket((PolygonModel *)frame.animModel, commandRecord);
            command = FIELD_MODEL_COMMAND_DONE;
            *resultPtr = command;
            return 1;
        case FIELD_MODEL_COMMAND_SKIP:
            command = FIELD_MODEL_COMMAND_DONE;
            *resultPtr = command;
            return 1;
        case FIELD_MODEL_COMMAND_RECORD_OVERRIDES:
            Field_ApplyModelRecordOverrides(modelRecord, (FieldModelFlagTable *)&frame.setCount, commandRecord);
            command = FIELD_MODEL_COMMAND_DONE;
            *resultPtr = command;
            return 1;
        case FIELD_MODEL_COMMAND_POLYGON_RECORD_FRAME:
            frame.runtimeCommand = (FieldModelCommandRecord *)&g_FieldModelRuntimeFlags;
            g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
            frame.commandBlock = (FieldModelCommand *)Gfx_GetPsxScratchpadPtr(0);
            frame.commandPacket = (FieldModelCommandPacket *)((int)frame.commandBlock + 8);
            frame.commandBlock->valueP = *(unsigned char *)&commandRecord->pHigh << 8 & 0xFF00 | commandRecord->command & 0xFF;
            frame.commandBlock->valueQ = commandRecord->qHigh << 8 & 0xFF00 | commandRecord->qLow & 0xFF;
            frame.commandBlock->valueR = commandRecord->rHigh << 8 & 0xFF00 | commandRecord->rLow & 0xFF;
            frame.runtimeCommand->command = frame.commandPacket->commandWord & 0xFF;
            frame.runtimeCommand->pHigh = frame.commandPacket->commandWord >> 8 & 0xFF;
            frame.runtimeCommand->qLow = frame.commandPacket->qWord & 0xFF;
            frame.runtimeCommand->qHigh = frame.commandPacket->qWord >> 8 & 0xFF;
            frame.runtimeCommand->rLow = frame.commandPacket->rWord & 0xFF;
            frame.runtimeCommand->rHigh = frame.commandPacket->rWord >> 8 & 0xFF;
            frame.runtimeCommand->param3Low = (short)(commandRecord->param3High << 8 & 0xFF00 | commandRecord->param3Low & 0xFF) & 0xFF;
            frame.runtimeCommand->param3High = (short)(commandRecord->param3High << 8 & 0xFF00 | commandRecord->param3Low & 0xFF) >> 8 & 0xFF;
            frame.runtimeCommand->param4Low = (short)(commandRecord->param4High << 8 & 0xFF00 | commandRecord->param4Low & 0xFF) & 0xFF;
            frame.runtimeCommand->param4High = (short)(commandRecord->param4High << 8 & 0xFF00 | commandRecord->param4Low & 0xFF) >> 8 & 0xFF;
            frame.runtimeCommand->param5Low = (short)(commandRecord->param5High << 8 & 0xFF00 | commandRecord->param5Low & 0xFF) & 0xFF;
            frame.runtimeCommand->param5High = (short)(commandRecord->param5High << 8 & 0xFF00 | commandRecord->param5Low & 0xFF) >> 8 & 0xFF;
            frame.runtimeCommand->param6Low = (short)(commandRecord->param6High << 8 & 0xFF00 | commandRecord->param6Low & 0xFF) & 0xFF;
            frame.runtimeCommand->param6High = (short)(commandRecord->param6High << 8 & 0xFF00 | commandRecord->param6Low & 0xFF) >> 8 & 0xFF;
            frame.runtimeCommand->param7Low = (short)(commandRecord->param7High << 8 & 0xFF00 | commandRecord->param7Low & 0xFF) & 0xFF;
            frame.runtimeCommand->param7High = (short)(commandRecord->param7High << 8 & 0xFF00 | commandRecord->param7Low & 0xFF) >> 8 & 0xFF;
            frame.runtimeCommand->param8Low = (short)(commandRecord->param8High << 8 & 0xFF00 | commandRecord->param8Low & 0xFF) & 0xFF;
            frame.runtimeCommand->param8High = (short)(commandRecord->param8High << 8 & 0xFF00 | commandRecord->param8Low & 0xFF) >> 8 & 0xFF;
            frame.runtimeCommand->param9Low = (short)(commandRecord->param9High << 8 & 0xFF00 | commandRecord->param9Low & 0xFF) & 0xFF;
            frame.runtimeCommand->param9High = (short)(commandRecord->param9High << 8 & 0xFF00 | commandRecord->param9Low & 0xFF) >> 8 & 0xFF;
            Field_ApplyPolygonRecordFrame(modelRecord, (FieldPolygonRampRecord *)&frame.setCount, frame.runtimeCommand, -1, -1, 0);
            command = FIELD_MODEL_COMMAND_DONE;
            *resultPtr = command;
            return 1;
        case FIELD_MODEL_COMMAND_RAMP_CYCLE:
            frame.runtimeEntry = (FieldRuntimeFlags *)&g_FieldModelRuntimeFlags;
            frame.runtimeFlags = g_FieldModelCommandFlags;
            frame.tmpFlag = 0;
            g_FieldModelLightData[modelIndex]->flags &= 0xFFFFFFFE;
            if (commandRecord->command == 2) {
                g_FieldScriptExecFlags[modelIndex] = 0xFF;
                command = FIELD_MODEL_COMMAND_DONE;
                frame.runtimeFlags[modelIndex * 2 + 2] = 0;
                for (frame.flagSetIndex = 0; frame.flagSetIndex < frame.setCount; frame.flagSetIndex++) {
                    frame.setEntries[frame.flagSetIndex].appended = 1;
                    Gfx_ForEachModelPolygonSet(frame.polySetRoot, (GfxPolygonSetFn)Field_WriteTrueFlag, (PolygonModel *)frame.animModel);
                }
                g_FieldModelLightData[modelIndex]->flags |= 1;
                *resultPtr = command;
                return 0;
            }
            if (frame.runtimeFlags[modelIndex * 2 + 2] == 1) {
                g_FieldScriptExecFlags[modelIndex] = 2;
                frame.runtimeEntry->active = 1;
            } else if ((char)g_FieldScriptExecFlags[modelIndex] == -1) {
                g_FieldScriptExecFlags[modelIndex] = 1;
                frame.runtimeEntry->active = 0;
            } else if ((char)g_FieldScriptExecFlags[modelIndex] == 1) {
                g_FieldScriptExecFlags[modelIndex] = 2;
                frame.runtimeFlags[modelIndex * 2 + 2] = 1;
                frame.runtimeFlags[modelIndex * 2 + 3] = modelIndex;
                frame.runtimeEntry->active = 1;
            }
            frame.runtimeEntry->modelIndex = modelIndex;
            Field_RunPolygonRampCycle(modelRecord, (FieldPolygonRampRecord *)&frame.setCount, rampStep, rampCount, frame.runtimeEntry);
            *resultPtr = command;
            return 0;
        }
        *resultPtr = command;
        return 1;
    }
}
} /* extern "C" */

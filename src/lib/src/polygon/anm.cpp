// src/lib/src/polygon/anm.cpp
// Original: C:\lib\src\polygon\anm.cpp
// Contents: Animation transform and skeleton drawing: builds root and bone matrices, draws a
//   skeleton frame, bakes per-frame matrices, and applies vertex colour changes across a model's
//   polygon sets.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdio.h>
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "lib/api/stack.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/types/stack.h"
#include "lib/enums/graphics.h"
extern "C" {

#pragma intrinsic(strlen)
struct GfxFloatColorAddend;
struct PolygonModelVertexColors;
void __cdecl BuildUniformScaleMatrix(float scale, PolygonTransform *matrix);

typedef struct {
    PolygonRootRotation *rootPose;
    PolygonBoneRotation *boneRotations;
} AnmFramePose;
typedef void (__cdecl *AnmRootBuildFn)(PolygonTransform *, void *, void *, void *);
typedef PolygonTransform *(__cdecl *AnmBoneBuildFn)(int, PolygonTransform *, void *, void *, void *, PolygonSkelFrameEntry *, void *);
// clang-format off
// GLOBAL: FF7 0x0090f964
int g_AnmTransformFlags = 0x807B;
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00db2bd8
float g_AnmZeroBoneRotationDraw[3] = {0};
unsigned char g_bssPad_00db2be4[4] = {0};
// GLOBAL: FF7 0x00db2be8
float g_AnmZeroBoneRotationBake[3] = {0};
// clang-format on

/*
 * Writes default transform flags (0x807B) to the output destination.
 */
// FUNCTION: FF7 0x006833d0
void __cdecl Anim_GetDefaultTransformFlags(void *out)
{
    *(int *)out = 0x807B;
}

/*
 * Sets `g_AnmTransformFlags` to the int at `src`; the modules call it on entry (battle, field,
 * the world map, chocobo, condor, highway), field and condor with 0x4051, chocobo with 0x8051.
 * The initial value is 0x807B, the default `Anim_GetDefaultTransformFlags` gives.
 * The root builders test the rotate, translate and per-axis negate bits (`AnimTransformFlag`);
 * `Anim_DrawSkeletonFrame` tests three more: ANIM_TRANSFORM_PLACEMENT_MATRIX multiplies the root
 * by the placement's matrix, ANIM_TRANSFORM_OFFSET_BY_YAW turns the placement's offset by its Y
 * rotation before adding it, and ANIM_TRANSFORM_CAMERA_BONES combines each bone matrix with the
 * camera matrix before the bone's polygon sets get it.
 */
// FUNCTION: FF7 0x006833de
void __cdecl Anim_SetTransformFlags(void *src)
{
    g_AnmTransformFlags = *(int *)src;
}

/*
 * Returns pointer to the global animation transform flags variable.
 */
// FUNCTION: FF7 0x006833ee
int *__cdecl Anim_GetTransformFlagsPtr(void)
{
    return &g_AnmTransformFlags;
}

/*
 * Builds a root transform matrix from animation descriptor rotations and optional source translation.
 */
// FUNCTION: FF7 0x006833f8
void __cdecl Anim_BuildRootMatrix(PolygonTransform *dstMatrix, PolygonRootRotation **rotations, PolygonAnimDesc *desc, void *context)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[272];
        int axis;
        struct { unsigned char pad_to_4[0x4]; PolygonTransform tmpMatrix; };
        struct { unsigned char pad_to_44[0x44]; int rowCount; };
        struct { unsigned char pad_to_48[0x48]; PolygonTransform *rowPtr; };
        struct { unsigned char pad_to_4C[0x4C]; PolygonTransform rotMatrix; };
        struct { unsigned char pad_to_8C[0x8C]; PolygonTransform baseMatrix; };
        struct { unsigned char pad_to_CC[0xCC]; PolygonTransform extraMatrix; };
        struct { unsigned char pad_to_10C[0x10C]; PolygonTranslation *translation; };
    } frame;
#pragma pack(pop)

    if (desc->baked != 0) {
        *dstMatrix = *desc->matrices;
    } else {
        frame.rowCount = 0;
        if ((g_AnmTransformFlags & ANIM_TRANSFORM_ROTATE) != 0) {
            frame.rowPtr = &frame.rotMatrix;
            for (frame.axis = 0; frame.axis < 3; frame.axis++) {
                switch (desc->axisOrder[frame.axis]) {
                case ANIM_AXIS_X:
                    if ((*rotations)->rotX != 0.0f) {
                        if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_ROT_X) != 0) {
                            BuildRotationXRowVec(-(*rotations)->rotX, (struct GfxMatrix4x4 *)frame.rowPtr);
                        } else {
                            BuildRotationXRowVec((*rotations)->rotX, (struct GfxMatrix4x4 *)frame.rowPtr);
                        }
                        frame.rowCount++;
                        frame.rowPtr++;
                    }
                    break;
                case ANIM_AXIS_Y:
                    if ((*rotations)->rotY != 0.0f) {
                        if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_ROT_Y) != 0) {
                            BuildRotationYRowVec(-(*rotations)->rotY, (struct GfxMatrix4x4 *)frame.rowPtr);
                        } else {
                            BuildRotationYRowVec((*rotations)->rotY, (struct GfxMatrix4x4 *)frame.rowPtr);
                        }
                        frame.rowCount++;
                        frame.rowPtr++;
                    }
                    break;
                case ANIM_AXIS_Z:
                    if ((*rotations)->rotZ != 0.0f) {
                        if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_ROT_Z) != 0) {
                            BuildRotationZRowVec(-(*rotations)->rotZ, (struct GfxMatrix4x4 *)frame.rowPtr);
                        } else {
                            BuildRotationZRowVec((*rotations)->rotZ, (struct GfxMatrix4x4 *)frame.rowPtr);
                        }
                        frame.rowCount++;
                        frame.rowPtr++;
                    }
                }
            }
        }
        /* Numeric active rotation-axis count (0..3). */
        switch (frame.rowCount) {
        case ANIM_ROT_AXIS_NONE:
            SetIdentityMatrix4x4((void *)dstMatrix);
            break;
        case ANIM_ROT_AXIS_SINGLE:
            *dstMatrix = frame.rotMatrix;
            break;
        case ANIM_ROT_AXIS_DOUBLE:
            MultiplyMatrix4x4Core(&frame.baseMatrix, &frame.rotMatrix, (void *)dstMatrix);
            break;
        case ANIM_ROT_AXIS_TRIPLE:
            MultiplyMatrix4x4Core(&frame.baseMatrix, &frame.rotMatrix, &frame.tmpMatrix);
            MultiplyMatrix4x4Core(&frame.extraMatrix, &frame.tmpMatrix, (void *)dstMatrix);
        }
        if ((g_AnmTransformFlags & ANIM_TRANSFORM_TRANSLATE) != 0) {
            frame.translation = &(*rotations)->translation;
            if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_POS_X) != 0) {
                dstMatrix->posX = -frame.translation->x;
            } else {
                dstMatrix->posX = frame.translation->x;
            }
            if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_POS_Y) != 0) {
                dstMatrix->posY = -frame.translation->y;
            } else {
                dstMatrix->posY = frame.translation->y;
            }
            if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_POS_Z) != 0) {
                dstMatrix->posZ = -frame.translation->z;
            } else {
                dstMatrix->posZ = frame.translation->z;
            }
        }
    }
}

/*
 * Builds a biased root transform matrix applying per-axis rotation biases and translation offsets.
 */
// FUNCTION: FF7 0x00683755
void __cdecl Anim_BuildRootMatrixBiased(PolygonTransform *dstMatrix, AnimRootBias **rotations, AnimRootBiasDesc *desc, PolygonBiasModel *owner)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[288];
        PolygonTransformBias *bias;
        struct { unsigned char pad_to_4[0x4]; int axis; };
        struct { unsigned char pad_to_8[0x8]; PolygonTransform tmpMatrix; };
        struct { unsigned char pad_to_48[0x48]; int rowCount; };
        struct { unsigned char pad_to_4C[0x4C]; float biasZ; };
        struct { unsigned char pad_to_50[0x50]; float biasY; };
        struct { unsigned char pad_to_54[0x54]; float biasX; };
        struct { unsigned char pad_to_58[0x58]; PolygonTransform *rowPtr; };
        struct { unsigned char pad_to_5C[0x5C]; PolygonTransform rotMatrix; };
        struct { unsigned char pad_to_9C[0x9C]; PolygonTransform baseMatrix; };
        struct { unsigned char pad_to_DC[0xDC]; PolygonTransform extraMatrix; };
        struct { unsigned char pad_to_11C[0x11C]; PolygonTranslation *translation; };
    } frame;
#pragma pack(pop)

    if (desc->biasEnabled != 0) {
        *dstMatrix = *desc->matrixData;
    } else {
        frame.bias = (PolygonTransformBias *)(owner->transformBias);
        frame.rowCount = 0;
        if ((g_AnmTransformFlags & ANIM_TRANSFORM_ROTATE) != 0) {
            frame.rowPtr = &frame.rotMatrix;
            for (frame.axis = 0; frame.axis < 3; frame.axis++) {
                switch (desc->axisOrder[frame.axis]) {
                case ANIM_AXIS_X:
                    frame.biasX = (*rotations)->biasX + frame.bias->angleBiasX;
                    if (frame.biasX != 0.0f) {
                        if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_ROT_X) != 0) {
                            BuildRotationXRowVec(-frame.biasX, (struct GfxMatrix4x4 *)frame.rowPtr);
                        } else {
                            BuildRotationXRowVec(frame.biasX, (struct GfxMatrix4x4 *)frame.rowPtr);
                        }
                        frame.rowCount++;
                        frame.rowPtr++;
                    }
                    break;
                case ANIM_AXIS_Y:
                    frame.biasY = (*rotations)->biasY + frame.bias->angleBiasY;
                    if (frame.biasY != 0.0f) {
                        if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_ROT_Y) != 0) {
                            BuildRotationYRowVec(-frame.biasY, (struct GfxMatrix4x4 *)frame.rowPtr);
                        } else {
                            BuildRotationYRowVec(frame.biasY, (struct GfxMatrix4x4 *)frame.rowPtr);
                        }
                        frame.rowCount++;
                        frame.rowPtr++;
                    }
                    break;
                case ANIM_AXIS_Z:
                    frame.biasZ = (*rotations)->biasZ + frame.bias->angleBiasZ;
                    if (frame.biasZ != 0.0f) {
                        if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_ROT_Z) != 0) {
                            BuildRotationZRowVec(-frame.biasZ, (struct GfxMatrix4x4 *)frame.rowPtr);
                        } else {
                            BuildRotationZRowVec(frame.biasZ, (struct GfxMatrix4x4 *)frame.rowPtr);
                        }
                        frame.rowCount++;
                        frame.rowPtr++;
                    }
                }
            }
        }
        /* Numeric active rotation-axis count (0..3). */
        switch (frame.rowCount) {
        case ANIM_ROT_AXIS_NONE:
            SetIdentityMatrix4x4((void *)dstMatrix);
            break;
        case ANIM_ROT_AXIS_SINGLE:
            *dstMatrix = frame.rotMatrix;
            break;
        case ANIM_ROT_AXIS_DOUBLE:
            MultiplyMatrix4x4Core(&frame.baseMatrix, &frame.rotMatrix, (void *)dstMatrix);
            break;
        case ANIM_ROT_AXIS_TRIPLE:
            MultiplyMatrix4x4Core(&frame.baseMatrix, &frame.rotMatrix, &frame.tmpMatrix);
            MultiplyMatrix4x4Core(&frame.extraMatrix, &frame.tmpMatrix, (void *)dstMatrix);
        }
        if ((g_AnmTransformFlags & ANIM_TRANSFORM_TRANSLATE) != 0) {
            frame.translation = &(*rotations)->translation;
            if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_POS_X) != 0) {
                dstMatrix->posX = -frame.translation->x;
            } else {
                dstMatrix->posX = frame.translation->x;
            }
            if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_POS_Y) != 0) {
                dstMatrix->posY = -frame.translation->y;
            } else {
                dstMatrix->posY = frame.translation->y;
            }
            if ((g_AnmTransformFlags & ANIM_TRANSFORM_NEGATE_POS_Z) != 0) {
                dstMatrix->posZ = -frame.translation->z;
            } else {
                dstMatrix->posZ = frame.translation->z;
            }
        }
        dstMatrix->posX = (double)frame.bias->translateX + dstMatrix->posX;
        dstMatrix->posY = (double)frame.bias->translateY + dstMatrix->posY;
        dstMatrix->posZ = (double)frame.bias->translateZ + dstMatrix->posZ;
    }
}

/*
 * Builds an animated bone transformation matrix or returns precomputed override matrix.
 */
// FUNCTION: FF7 0x00683b33
PolygonTransform *__cdecl Anim_BuildBoneMatrix(int boneIndex, PolygonTransform *matrix, PolygonBoneRotation *rotation, void *extra, PolygonAnimDesc *anim, PolygonSkelFrameEntry *bone, struct PolygonAnimModel *skeleton)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[276];
        int axisMode;
        struct { unsigned char pad_to_4[0x4]; int axis; };
        struct { unsigned char pad_to_8[0x8]; PolygonTransform tmpMatrix; };
        struct { unsigned char pad_to_48[0x48]; PolygonBoneRotation *boneRot; };
        struct { unsigned char pad_to_4C[0x4C]; int rowCount; };
        struct { unsigned char pad_to_50[0x50]; PolygonTransform *rowPtr; };
        struct { unsigned char pad_to_54[0x54]; PolygonTransform rotMatrix; };
        struct { unsigned char pad_to_94[0x94]; PolygonTransform baseMatrix; };
        struct { unsigned char pad_to_D4[0xD4]; PolygonTransform extraMatrix; };
    } frame;
#pragma pack(pop)

    if (anim->baked != 0) {
        matrix = (PolygonTransform *)((int)anim->matrices + (boneIndex + 1 << 6));
    } else {
        frame.boneRot = rotation;
        frame.rowCount = 0;
        frame.rowPtr = &frame.rotMatrix;
        for (frame.axis = 0; frame.axis < 3; frame.axis++) {
            frame.axisMode = anim->axisOrder[frame.axis];
            switch (frame.axisMode) {
            case ANIM_AXIS_X:
                if (frame.boneRot->angleX != 0.0f) {
                    BuildRotationXRowVec(frame.boneRot->angleX, (struct GfxMatrix4x4 *)frame.rowPtr);
                    frame.rowCount++;
                    frame.rowPtr++;
                }
                break;
            case ANIM_AXIS_Y:
                if (frame.boneRot->angleY != 0.0f) {
                    BuildRotationYRowVec(frame.boneRot->angleY, (struct GfxMatrix4x4 *)frame.rowPtr);
                    frame.rowCount++;
                    frame.rowPtr++;
                }
                break;
            case ANIM_AXIS_Z:
                if (frame.boneRot->angleZ != 0.0f) {
                    BuildRotationZRowVec(frame.boneRot->angleZ, (struct GfxMatrix4x4 *)frame.rowPtr);
                    frame.rowCount++;
                    frame.rowPtr++;
                }
            }
        }
        /* Numeric active rotation-axis count (0..3). */
        switch (frame.rowCount) {
        case ANIM_ROT_AXIS_NONE:
            SetIdentityMatrix4x4((void *)matrix);
            break;
        case ANIM_ROT_AXIS_SINGLE:
            *matrix = frame.rotMatrix;
            break;
        case ANIM_ROT_AXIS_DOUBLE:
            ClearMatrixTranslationAndW((void *)matrix);
            MultiplyMatrix4x4Core(&frame.baseMatrix, &frame.rotMatrix, (void *)matrix);
            break;
        case ANIM_ROT_AXIS_TRIPLE:
            ClearMatrixTranslationAndW((void *)matrix);
            MultiplyMatrix4x4Core(&frame.baseMatrix, &frame.rotMatrix, &frame.tmpMatrix);
            MultiplyMatrix4x4Core(&frame.extraMatrix, &frame.tmpMatrix, (void *)matrix);
        }
        if (bone->parentIndex >= 0) {
            matrix->posZ = (double)((PolygonSkelFrameEntry *)skeleton->frames)[bone->parentIndex].boneLength + matrix->posZ;
        }
    }
    return matrix;
}

/*
 * Builds a bone transform matrix applying per-axis rotation biases and parent bone translation.
 */
// FUNCTION: FF7 0x00683dc4
PolygonTransform *__cdecl Anim_BuildBoneMatrixBiased(int boneIndex, PolygonTransform *matrix, PolygonBoneRotation *angles, int extra, AnimBoneBiasSource *anim, PolygonSkelFrameEntry *bone, PolygonBiasModel *model)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[292];
        int axisMode;
        struct { unsigned char pad_to_4[0x4]; PolygonTransformBias *bias; };
        struct { unsigned char pad_to_8[0x8]; int axis; };
        struct { unsigned char pad_to_C[0xC]; PolygonTransform tmpMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; PolygonBoneRotation *boneAngles; };
        struct { unsigned char pad_to_50[0x50]; int rowCount; };
        struct { unsigned char pad_to_54[0x54]; float angleZ; };
        struct { unsigned char pad_to_58[0x58]; float angleY; };
        struct { unsigned char pad_to_5C[0x5C]; float angleX; };
        struct { unsigned char pad_to_60[0x60]; PolygonTransform *rowPtr; };
        struct { unsigned char pad_to_64[0x64]; PolygonTransform rotMatrix; };
        struct { unsigned char pad_to_A4[0xA4]; PolygonTransform baseMatrix; };
        struct { unsigned char pad_to_E4[0xE4]; PolygonTransform extraMatrix; };
    } frame;
#pragma pack(pop)

    if (anim->biasEnabled != 0) {
        matrix = (PolygonTransform *)(anim->matrixBase + (boneIndex + 1 << 6));
    } else {
        frame.bias = (PolygonTransformBias *)(model->transformBias);
        frame.boneAngles = angles;
        frame.rowCount = 0;
        frame.rowPtr = &frame.rotMatrix;
        for (frame.axis = 0; frame.axis < 3; frame.axis++) {
            frame.axisMode = anim->axisOrder[frame.axis];
            switch (frame.axisMode) {
            case ANIM_AXIS_X:
                frame.angleX = frame.boneAngles->angleX + frame.bias->angleBiasX;
                if (frame.angleX != 0.0f) {
                    BuildRotationXRowVec(frame.angleX, (struct GfxMatrix4x4 *)frame.rowPtr);
                    frame.rowCount++;
                    frame.rowPtr++;
                }
                break;
            case ANIM_AXIS_Y:
                frame.angleY = frame.boneAngles->angleY + frame.bias->angleBiasY;
                if (frame.angleY != 0.0f) {
                    BuildRotationYRowVec(frame.angleY, (struct GfxMatrix4x4 *)frame.rowPtr);
                    frame.rowCount++;
                    frame.rowPtr++;
                }
                break;
            case ANIM_AXIS_Z:
                frame.angleZ = frame.boneAngles->angleZ + frame.bias->angleBiasZ;
                if (frame.angleZ != 0.0f) {
                    BuildRotationZRowVec(frame.angleZ, (struct GfxMatrix4x4 *)frame.rowPtr);
                    frame.rowCount++;
                    frame.rowPtr++;
                }
            }
        }
        /* Numeric active rotation-axis count (0..3). */
        switch (frame.rowCount) {
        case ANIM_ROT_AXIS_NONE:
            SetIdentityMatrix4x4((void *)matrix);
            break;
        case ANIM_ROT_AXIS_SINGLE:
            *matrix = frame.rotMatrix;
            break;
        case ANIM_ROT_AXIS_DOUBLE:
            ClearMatrixTranslationAndW((void *)matrix);
            MultiplyMatrix4x4Core(&frame.baseMatrix, &frame.rotMatrix, (void *)matrix);
            break;
        case ANIM_ROT_AXIS_TRIPLE:
            ClearMatrixTranslationAndW((void *)matrix);
            MultiplyMatrix4x4Core(&frame.baseMatrix, &frame.rotMatrix, &frame.tmpMatrix);
            MultiplyMatrix4x4Core(&frame.extraMatrix, &frame.tmpMatrix, (void *)matrix);
        }
        if (bone->parentIndex >= 0) {
            matrix->posZ = (double)((PolygonSkelFrameEntry *)model->boneTable)[bone->parentIndex].boneLength + matrix->posZ;
            matrix->posX = (double)frame.bias->translateX + matrix->posX;
            matrix->posY = (double)frame.bias->translateY + matrix->posY;
            matrix->posZ = (double)frame.bias->translateZ + matrix->posZ;
        }
    }
    return matrix;
}

/*
 * Poses skeleton `draw` in frame `frameIndex` of animation `skeleton`, placed by `transform`, and
 * draws it: field, battle, the world map, the minigames and the offscreen renderer call it once
 * per model per game frame. Nothing happens for a null animation or skeleton or a frame outside
 * 0..frameCount-1.
 *
 * It pushes the engine context's matrix stack and builds the root matrix from the frame's root
 * pose (`Anim_BuildRootMatrix`, or the `Biased` variants of root and bone builders when
 * `draw->bias` points to a nonzero word). With a placement it then applies the uniform scale, the
 * per-axis scale, the placement matrix (ANIM_TRANSFORM_PLACEMENT_MATRIX), the Y, X and Z rotations
 * and the offset (turned by the Y rotation under ANIM_TRANSFORM_OFFSET_BY_YAW). The skeleton's
 * POLYGON_MODEL_SAVE_ROOT_BEFORE/AFTER_PLACEMENT flags save the root matrix before and after the
 * placement in `draw->rootBeforePlacement` and `draw->rootAfterPlacement`;
 * POLYGON_MODEL_WRITE_BONE_POSITIONS writes the root position and each bone's position into
 * `transform->outPos`.
 *
 * Then it walks the skeleton's part list: a push entry builds the bone's matrix from its
 * rotation in this frame (a zero rotation when the animation has no bones) on top of its
 * parent's and pushes it, a pop entry pops. Under ANIM_TRANSFORM_CAMERA_BONES the bone matrix is
 * combined with the camera matrix, into `transform->transformBase` when the skeleton has
 * POLYGON_MODEL_STORE_CAMERA_BONES. Each of the bone's polygon sets gets that matrix
 * (`Gfx_SetMatrixCall`); with POLYGON_MODEL_LIGHT it is lit through its light owner, and with
 * `draw->drawEnabled` and POLYGON_MODEL_DRAW set it is drawn (`Gfx_DrawPolygonSetCall`). The root
 * is popped at the end.
 *
 * Defect: the WRITE_BONE_POSITIONS and STORE_CAMERA_BONES paths read `transform->outPos` and
 * `transform->transformBase` without the null check the placement steps have.
 */
// FUNCTION: FF7 0x006840da
void __cdecl Anim_DrawSkeletonFrame(int frameIndex, PolygonSkeletonAnim *skeleton, PolygonBoneTransform *transform, PolygonAnimDraw *draw, GfxEngineContext *ctx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[560];
        int rootPopResult;
        struct { unsigned char pad_to_4[0x4]; int nodePopResult; };
        struct { unsigned char pad_to_8[0x8]; PolygonTransform *tmpBoneTop; };
        struct { unsigned char pad_to_C[0xC]; PolygonTransform *tmpParentTop; };
        struct { unsigned char pad_to_10[0x10]; PolygonTransform *tmpRootTop; };
        struct { unsigned char pad_to_14[0x14]; PolygonTransform lightProduct; };
        struct { unsigned char pad_to_54[0x54]; GfxLightOwner *lightOwner; };
        struct { unsigned char pad_to_58[0x58]; PolygonSkeletonFrameRef *frameRef; };
        struct { unsigned char pad_to_5C[0x5C]; int entryIndex; };
        struct { unsigned char pad_to_60[0x60]; PolygonSetEntry *entryPtr; };
        struct { unsigned char pad_to_64[0x64]; PolygonBoneRotation *boneRotation; };
        struct { unsigned char pad_to_68[0x68]; PolygonTransform *builtBoneMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; PolygonTransform boneBuildOut; };
        struct { unsigned char pad_to_AC[0xAC]; PolygonAnimBakeNode *animNode; };
        struct { unsigned char pad_to_B0[0xB0]; PolygonAnimNodeCmd nodeOp; };
        struct { unsigned char pad_to_B4[0xB4]; int boneIndex; };
        struct { unsigned char pad_to_B8[0xB8]; PolygonSkelFrameEntry *skelFrameEntry; };
        struct { unsigned char pad_to_BC[0xBC]; GfxHomogeneousPoint offsetX; };
        struct { unsigned char pad_to_C8[0xC8]; PolygonTransform rotationMatrix; };
        struct { unsigned char pad_to_108[0x108]; PolygonTransform rootSavedXyz; };
        struct { unsigned char pad_to_148[0x148]; PolygonTransform rootSavedUniform; };
        struct { unsigned char pad_to_188[0x188]; PolygonTransform *rootMatrix; };
        struct { unsigned char pad_to_18C[0x18C]; PolygonTransform *boneMatrix; };
        struct { unsigned char pad_to_190[0x190]; PolygonTransform *parentMatrix; };
        struct { unsigned char pad_to_194[0x194]; AnmRootBuildFn rootBuildFn; };
        struct { unsigned char pad_to_198[0x198]; PolygonTransform tmpCameraProduct; };
        struct { unsigned char pad_to_1D8[0x1D8]; float uniformScale; };
        struct { unsigned char pad_to_1DC[0x1DC]; PolygonTransform scaleMatrix; };
        struct { unsigned char pad_to_21C[0x21C]; AnmFramePose *frameDataPtr; };
        struct { unsigned char pad_to_220[0x220]; MatrixStack *matrixStack; };
        struct { unsigned char pad_to_224[0x224]; PolygonTransform *boneOutMatrix; };
        struct { unsigned char pad_to_228[0x228]; AnmBoneBuildFn boneBuildFn; };
        struct { unsigned char pad_to_22C[0x22C]; int scaleApplied; };
    } frame;
#pragma pack(pop)

    if (skeleton != 0 && draw != 0 && frameIndex >= 0 && frameIndex < skeleton->frameCount) {
        frame.scaleApplied = 0;
        frame.uniformScale = 1.0f;
        frame.frameDataPtr = (AnmFramePose *)(skeleton->frameData + frameIndex * 8);
        if (skeleton->isBaked != 0) {
            skeleton->bufferCursor = skeleton->bufferBase + (frameIndex * (skeleton->subCountMinus1 + 1) << 6);
        }
        frame.matrixStack = (MatrixStack *)(ctx->matrixStack);
        MatrixStack_Push(frame.matrixStack);
        frame.tmpRootTop = (PolygonTransform *)MatrixStack_Top(frame.matrixStack);
        frame.rootMatrix = frame.tmpRootTop;
        if (draw->bias != 0 && *(int *)draw->bias != 0) {
            frame.rootBuildFn = (AnmRootBuildFn)Anim_BuildRootMatrixBiased;
            frame.boneBuildFn = (AnmBoneBuildFn)Anim_BuildBoneMatrixBiased;
        } else {
            frame.rootBuildFn = (AnmRootBuildFn)Anim_BuildRootMatrix;
            frame.boneBuildFn = (AnmBoneBuildFn)Anim_BuildBoneMatrix;
        }
        frame.rootBuildFn(frame.rootMatrix, frame.frameDataPtr, skeleton, draw);
        if ((draw->flags & POLYGON_MODEL_SAVE_ROOT_BEFORE_PLACEMENT) != 0) {
            draw->rootBeforePlacement = *frame.rootMatrix;
        }
        if (transform != 0) {
            if (transform->scale != 1.0f) {
                frame.uniformScale = transform->scale;
                frame.rootMatrix->posX *= frame.uniformScale;
                frame.rootMatrix->posY *= frame.uniformScale;
                frame.rootMatrix->posZ *= frame.uniformScale;
                frame.scaleApplied = 1;
            }
            if (frame.scaleApplied != 0) {
                frame.rootSavedUniform = *frame.rootMatrix;
                BuildUniformScaleMatrix(frame.uniformScale, &frame.scaleMatrix);
                ConcatAffineMatrix4x4(&frame.scaleMatrix, &frame.rootSavedUniform, (void *)frame.rootMatrix);
            }
            if (transform->scaleX != 1.0f || (transform->scaleY != 1.0f || transform->scaleZ != 1.0f)) {
                frame.rootSavedXyz = *frame.rootMatrix;
                BuildScaleMatrixXYZ((struct GfxExecPosition *)&transform->scaleX, (struct GfxScaleMatrix *)&frame.scaleMatrix);
                ConcatAffineMatrix4x4(&frame.scaleMatrix, &frame.rootSavedXyz, (void *)frame.rootMatrix);
                frame.rootMatrix->posX *= transform->scaleX;
                frame.rootMatrix->posY *= transform->scaleY;
                frame.rootMatrix->posZ *= transform->scaleZ;
            }
            if ((g_AnmTransformFlags & ANIM_TRANSFORM_PLACEMENT_MATRIX) != 0) {
                PostMultiplyMatrix4x4InPlace((int)transform->placementMatrixRows, (int)frame.rootMatrix);
            }
            if (transform->angleY != 0.0f) {
                BuildRotationYRowVec(transform->angleY, (struct GfxMatrix4x4 *)&frame.rotationMatrix);
                RotateMatrixAboutAxis(ANIM_AXIS_Y, transform->angleY, frame.rootMatrix);
            } else {
                SetIdentityMatrix4x4(&frame.rotationMatrix);
            }
            if (transform->angleX != 0.0f) {
                RotateMatrixAboutAxis(ANIM_AXIS_X, transform->angleX, frame.rootMatrix);
            }
            if (transform->angleZ != 0.0f) {
                RotateMatrixAboutAxis(ANIM_AXIS_Z, transform->angleZ, frame.rootMatrix);
            }
            if ((g_AnmTransformFlags & ANIM_TRANSFORM_OFFSET_BY_YAW) != 0) {
                RotateVector3RowVec(&frame.rotationMatrix, &transform->offsetX, &frame.offsetX.x);
                frame.rootMatrix->posX += frame.offsetX.x;
                frame.rootMatrix->posY += frame.offsetX.y;
                frame.rootMatrix->posZ += frame.offsetX.z;
            } else {
                frame.rootMatrix->posX += transform->offsetX;
                frame.rootMatrix->posY += transform->offsetY;
                frame.rootMatrix->posZ += transform->offsetZ;
            }
        }
        if ((draw->flags & POLYGON_MODEL_SAVE_ROOT_AFTER_PLACEMENT) != 0) {
            draw->rootAfterPlacement = *frame.rootMatrix;
        }
        if ((draw->flags & POLYGON_MODEL_WRITE_BONE_POSITIONS) != 0 && transform->outPos != 0) {
            transform->outPos->posX = frame.rootMatrix->posX;
            transform->outPos->posY = frame.rootMatrix->posY;
            transform->outPos->posZ = frame.rootMatrix->posZ;
        }
        if (draw->skeleton != 0) {
            frame.animNode = ((PolygonAnimNodeList *)draw->skeleton)->first;
            while (frame.animNode != 0) {
                frame.nodeOp = frame.animNode->cmd;
                switch (frame.nodeOp.opcode) {
                case ANIM_NODE_NONE:
                    break;
                case ANIM_NODE_PUSH_BONE:
                    frame.boneIndex = frame.nodeOp.boneIndex;
                    frame.skelFrameEntry = (PolygonSkelFrameEntry *)(draw->frameTable + frame.boneIndex * 0x1C);
                    frame.tmpParentTop = (PolygonTransform *)MatrixStack_Top(frame.matrixStack);
                    frame.parentMatrix = frame.tmpParentTop;
                    MatrixStack_Push(frame.matrixStack);
                    frame.tmpBoneTop = (PolygonTransform *)MatrixStack_Top(frame.matrixStack);
                    frame.boneMatrix = frame.tmpBoneTop;
                    if (skeleton->subCountMinus1 > 0) {
                        frame.boneRotation = &frame.frameDataPtr->boneRotations[frame.boneIndex];
                    } else {
                        frame.boneRotation = (PolygonBoneRotation *)g_AnmZeroBoneRotationDraw;
                    }
                    frame.builtBoneMatrix = frame.boneBuildFn(frame.boneIndex, &frame.boneBuildOut, frame.boneRotation, frame.frameDataPtr, skeleton, frame.skelFrameEntry, draw);
                    ConcatAffineMatrix4x4((void *)frame.builtBoneMatrix, (void *)frame.parentMatrix, (void *)frame.boneMatrix);
                    if ((g_AnmTransformFlags & ANIM_TRANSFORM_CAMERA_BONES) != 0) {
                        if ((draw->flags & POLYGON_MODEL_STORE_CAMERA_BONES) != 0 && transform->transformBase != 0) {
                            frame.boneOutMatrix = (PolygonTransform *)(transform->transformBase + (frame.boneIndex + 1 << 6));
                            ConcatAffineMatrix4x4((void *)frame.boneMatrix, (void *)ctx->cameraMatrix, (void *)frame.boneOutMatrix);
                            frame.boneOutMatrix->rot03 = 0;
                            frame.boneOutMatrix->rot13 = 0;
                            frame.boneOutMatrix->rot23 = 0;
                            frame.boneOutMatrix->posW = FLOAT_ONE_BITS;
                        } else {
                            frame.tmpCameraProduct.rot03 = 0;
                            frame.tmpCameraProduct.rot13 = 0;
                            frame.tmpCameraProduct.rot23 = 0;
                            frame.tmpCameraProduct.posW = FLOAT_ONE_BITS;
                            frame.boneOutMatrix = &frame.tmpCameraProduct;
                            ConcatAffineMatrix4x4((void *)frame.boneMatrix, (void *)ctx->cameraMatrix, (void *)frame.boneOutMatrix);
                        }
                        if ((draw->flags & POLYGON_MODEL_WRITE_BONE_POSITIONS) != 0 && transform->outPos != 0) {
                            transform->outPos[frame.boneIndex + 1].posX = frame.boneMatrix->posX;
                            transform->outPos[frame.boneIndex + 1].posY = frame.boneMatrix->posY;
                            transform->outPos[frame.boneIndex + 1].posZ = frame.boneMatrix->posZ;
                        }
                    } else {
                        frame.boneOutMatrix = frame.boneMatrix;
                    }
                    if (frame.skelFrameEntry->count > 0 && frame.skelFrameEntry->dataPointer != 0) {
                        frame.entryPtr = (PolygonSetEntry *)frame.skelFrameEntry->dataPointer;
                        for (frame.entryIndex = 0; frame.entryIndex < frame.skelFrameEntry->count; frame.entryIndex++) {
                            if (frame.entryPtr->modelRef != 0) {
                                frame.frameRef = (PolygonSkeletonFrameRef *)frame.entryPtr->modelRef->polySet;
                                if (frame.frameRef != 0) {
                                    Gfx_SetMatrixCall(0, (int)frame.boneOutMatrix, frame.frameRef->transformKey, ctx);
                                    if ((draw->flags & POLYGON_MODEL_LIGHT) != 0) {
                                        frame.lightOwner = (GfxLightOwner *)(frame.frameRef->lightOwner);
                                        if (frame.lightOwner != 0) {
                                            frame.lightOwner->deviceTransform = GetCachedDeviceTransform(0, (void *)frame.frameRef->transformKey);
                                            if (frame.lightOwner->lightMatrixEnabled != 0) {
                                                MultiplyMatrix4x4Core((void *)frame.boneMatrix, (void *)frame.lightOwner->identityMatrix, &frame.lightProduct);
                                                Gfx_LightApplyTransposedMatrixToCachedProduct(&frame.lightProduct, frame.lightOwner);
                                            } else {
                                                Gfx_LightApplyTransposedMatrixToCachedProduct(frame.boneMatrix, frame.lightOwner);
                                            }
                                            Gfx_LightPolygonSetCall(frame.frameRef, frame.lightOwner);
                                        }
                                    }
                                    if (draw->drawEnabled != 0 && (draw->flags & POLYGON_MODEL_DRAW) != 0) {
                                        Gfx_DrawPolygonSetCall(frame.frameRef, (int)ctx);
                                    }
                                }
                            }
                            frame.entryPtr++;
                        }
                    }
                    break;
                case ANIM_NODE_POP:
                    frame.nodePopResult = MatrixStack_Pop(frame.matrixStack);
                }
                frame.animNode = frame.animNode->next;
            }
        }
        frame.rootPopResult = MatrixStack_Pop(frame.matrixStack);
    }
}

/*
 * Bakes bone matrices for a single animation frame into the skeleton matrix cache.
 */
// FUNCTION: FF7 0x00684a10
void __cdecl Anim_BakeFrameMatrices(int frameIndex, PolygonAnimBake *bake, PolygonAnimModel *model)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        int rootPopResult;
        struct { unsigned char pad_to_4[0x4]; int bonePopResult; };
        struct { unsigned char pad_to_8[0x8]; PolygonTransform *tmpPushedTop; };
        struct { unsigned char pad_to_C[0xC]; PolygonTransform *tmpParentTop; };
        struct { unsigned char pad_to_10[0x10]; PolygonTransform *tmpRootTop; };
        struct { unsigned char pad_to_14[0x14]; PolygonBoneRotation *boneRotation; };
        struct { unsigned char pad_to_18[0x18]; PolygonTransform *builtBoneMatrix; };
        struct { unsigned char pad_to_1C[0x1C]; PolygonTransform boneBuildOut; };
        struct { unsigned char pad_to_5C[0x5C]; PolygonAnimBakeNode *bakeNode; };
        struct { unsigned char pad_to_60[0x60]; PolygonAnimNodeCmd nodeOp; };
        struct { unsigned char pad_to_64[0x64]; int boneIndex; };
        struct { unsigned char pad_to_68[0x68]; int boneFrameEntry; };
        struct { unsigned char pad_to_6C[0x6C]; PolygonTransform *rootMatrix; };
        struct { unsigned char pad_to_70[0x70]; PolygonTransform *boneStackTop; };
        struct { unsigned char pad_to_74[0x74]; PolygonTransform *parentMatrix; };
        struct { unsigned char pad_to_78[0x78]; AnmFramePose *bakeFrameEntry; };
        struct { unsigned char pad_to_7C[0x7C]; MatrixStack *matrixStack; };
    } frame;
#pragma pack(pop)

    if (bake != 0 && model != 0 && frameIndex >= 0 && frameIndex < bake->count) {
        frame.bakeFrameEntry = (AnmFramePose *)(bake->frameTable + frameIndex * 8);
        bake->matrixCursor = &bake->matrixBuffer[frameIndex * (bake->lastFrame + 1)];
        frame.matrixStack = (MatrixStack *)(GetEngineContext()->matrixStack);
        MatrixStack_Push(frame.matrixStack);
        frame.tmpRootTop = (PolygonTransform *)MatrixStack_Top(frame.matrixStack);
        frame.rootMatrix = frame.tmpRootTop;
        Anim_BuildRootMatrix(frame.rootMatrix, &frame.bakeFrameEntry->rootPose, (struct PolygonAnimDesc *)bake, model);
        if ((model->flags & POLYGON_MODEL_SAVE_ROOT_BEFORE_PLACEMENT) != 0) {
            model->rootBeforePlacement = *frame.rootMatrix;
        }
        *bake->matrixCursor = *frame.rootMatrix;
        if (model->skeleton != 0) {
            frame.bakeNode = ((PolygonAnimNodeList *)model->skeleton)->first;
            while (frame.bakeNode != 0) {
                frame.nodeOp = frame.bakeNode->cmd;
                switch (frame.nodeOp.opcode) {
                case ANIM_NODE_NONE:
                    break;
                case ANIM_NODE_PUSH_BONE:
                    frame.boneIndex = frame.nodeOp.boneIndex;
                    frame.boneFrameEntry = model->frames + frame.boneIndex * 28;
                    frame.tmpParentTop = (PolygonTransform *)MatrixStack_Top(frame.matrixStack);
                    frame.parentMatrix = frame.tmpParentTop;
                    MatrixStack_Push(frame.matrixStack);
                    frame.tmpPushedTop = (PolygonTransform *)MatrixStack_Top(frame.matrixStack);
                    frame.boneStackTop = frame.tmpPushedTop;
                    if (bake->lastFrame > 0) {
                        frame.boneRotation = &frame.bakeFrameEntry->boneRotations[frame.boneIndex];
                    } else {
                        frame.boneRotation = (PolygonBoneRotation *)g_AnmZeroBoneRotationBake;
                    }
                    frame.builtBoneMatrix = Anim_BuildBoneMatrix(frame.boneIndex, &frame.boneBuildOut, frame.boneRotation, frame.bakeFrameEntry, (struct PolygonAnimDesc *)bake, (PolygonSkelFrameEntry *)frame.boneFrameEntry, model);
                    bake->matrixCursor[frame.boneIndex + 1] = *frame.builtBoneMatrix;
                    break;
                case ANIM_NODE_POP:
                    frame.bonePopResult = MatrixStack_Pop(frame.matrixStack);
                }
                frame.bakeNode = frame.bakeNode->next;
            }
        }
        frame.rootPopResult = MatrixStack_Pop(frame.matrixStack);
    }
}

// The optimize("p") pairs in this file set /Op per function, as the retail build did (lib/codegen.h, "/Op pairs").
#pragma optimize("p", off)

/*
 * Allocates memory and precalculates bone matrices for all frames of an animation.
 */
// FUNCTION: FF7 0x00684c32
void __cdecl Anim_BakeAllFrameMatrices(PolygonAnimBake *bake, PolygonAnimModel *model)
{
    int frameIndex;

    if (bake != 0 && model != 0 && bake->baked == 0) {
        // STRING: FF7 0x0090f968
        bake->matrixBuffer = (PolygonTransform *)GameHeapAlloc(bake->count * (bake->lastFrame + 1), 64, "C:\\lib\\src\\polygon\\anm.cpp", 1028);
        if (bake->matrixBuffer != 0) {
            for (frameIndex = 0; frameIndex < bake->count; frameIndex++) {
                Anim_BakeFrameMatrices(frameIndex, bake, model);
            }
            bake->baked = 1;
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Walks model polygon set hierarchy and submits each polygon set for drawing.
 */
// FUNCTION: FF7 0x00684cc6
void __cdecl Gfx_DrawModelPolygonSets(PolygonModel *model, GfxEngineContext *drawParam)
{
    PolygonSetGroup *node;
    int groupNum;
    PolygonSetEntry *ent;
    int entryIndex;
    PolygonSkeletonFrameRef *polySet;

    if (model != 0) {
        node = (PolygonSetGroup *)model->polySets;
        for (groupNum = 0; groupNum < model->polySetCount; groupNum++) {
            if (node->entryCount > 0) {
                ent = (PolygonSetEntry *)node->entries;
                if (ent != 0) {
                    for (entryIndex = 0; entryIndex < node->entryCount; entryIndex++) {
                        if (ent->modelRef != 0) {
                            polySet = (PolygonSkeletonFrameRef *)ent->modelRef->polySet;
                            Gfx_DrawPolygonSetCall(polySet, (int)drawParam);
                        }
                        ent++;
                    }
                }
            }
            node++;
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Modulates vertex colors of all polygon sets in a model with the given color.
 */
// FUNCTION: FF7 0x00684d73
void __cdecl Gfx_ModulateModelVertexColors(int color, PolygonModelVertexColors *model)
{
    PolygonSetGroup *group;
    PolygonSetEntry *entry;
    int i;
    GfxPolygonSet *pPolygonSet;
    int entIndex;

    if (model != 0) {
        group = model->polySets;
        for (i = 0; i < model->polySetCount; i++) {
            if (group->entryCount > 0) {
                entry = (PolygonSetEntry *)group->entries;
                if (entry != 0) {
                    for (entIndex = 0; entIndex < group->entryCount; entIndex++) {
                        if (entry->modelRef != 0) {
                            pPolygonSet = (GfxPolygonSet *)entry->modelRef->polySet;
                            Gfx_ModulatePolygonSetVertexColors(color, pPolygonSet);
                        }
                        entry++;
                    }
                }
            }
            group++;
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Adds a color offset to vertex colors across all polygon sets in a model.
 */
// FUNCTION: FF7 0x00684e20
void __cdecl Gfx_AddColorToModelVertexColors(GfxFloatColorAddend *color, PolygonModelVertexColors *model, int context)
{
    PolygonSetGroup *group;
    PolygonSetEntry *entry;
    int i;
    GfxPolygonSet *pPolygonSet;
    int entIndex;

    if (model != 0) {
        group = model->polySets;
        for (i = 0; i < model->polySetCount; i++) {
            if (group->entryCount > 0) {
                entry = (PolygonSetEntry *)group->entries;
                if (entry != 0) {
                    for (entIndex = 0; entIndex < group->entryCount; entIndex++) {
                        if (entry->modelRef != 0) {
                            pPolygonSet = (GfxPolygonSet *)entry->modelRef->polySet;
                            Gfx_AddColorToPolygonSetVertexColors(color, pPolygonSet);
                        }
                        entry++;
                    }
                }
            }
            group++;
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Restores original vertex colors for all polygon sets in a model.
 */
// FUNCTION: FF7 0x00684ecd
void __cdecl Gfx_RestoreModelVertexColors(PolygonModelVertexColors *model, int context)
{
    PolygonSetGroup *group;
    PolygonSetEntry *entry;
    int i;
    GfxPolygonSet *pPolygonSet;
    int entIndex;

    if (model != 0) {
        group = model->polySets;
        for (i = 0; i < model->polySetCount; i++) {
            if (group->entryCount > 0) {
                entry = (PolygonSetEntry *)group->entries;
                if (entry != 0) {
                    for (entIndex = 0; entIndex < group->entryCount; entIndex++) {
                        if (entry->modelRef != 0) {
                            pPolygonSet = (GfxPolygonSet *)entry->modelRef->polySet;
                            Gfx_RestorePolygonSetVertexColors(pPolygonSet);
                        }
                        entry++;
                    }
                }
            }
            group++;
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Applies draw specifications to all polygon sets in a model.
 */
// FUNCTION: FF7 0x00684f73
void __cdecl Gfx_ApplyDrawSpecToModel(GfxDrawSpec *drawSpec, PolygonModel *model)
{
    PolygonSetGroup *node;
    GfxEngineContext *ctx;
    int groupNum;
    PolygonSetEntry *ent;
    int entryIndex;
    GfxPolygonSet *polySet;

    if (model != 0) {
        ctx = GetEngineContext();
        node = (PolygonSetGroup *)model->polySets;
        for (groupNum = 0; groupNum < model->polySetCount; groupNum++) {
            if (node->entryCount > 0) {
                ent = (PolygonSetEntry *)node->entries;
                if (ent != 0) {
                    for (entryIndex = 0; entryIndex < node->entryCount; entryIndex++) {
                        if (ent->modelRef != 0) {
                            polySet = (GfxPolygonSet *)ent->modelRef->polySet;
                            Gfx_ApplyDrawSpecToPolygonSet(drawSpec, polySet);
                        }
                        ent++;
                    }
                }
            }
            node++;
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Calls `a2(a1, polygonSet)` for the polygon set of every RSD model on every bone of skeleton
 * `a3`, in bone order; a model entry without a model is skipped. Field's model commands (lighting
 * and flag changes), battle and `Gfx_AndModelPolygonSetRenderFlags` / `Gfx_OrModelPolygonSetRenderFlags` change all of
 * a model's polygon sets this way. The callback is also called when a model has no polygon set,
 * with 0. The engine context is fetched and not used.
 */
// FUNCTION: FF7 0x00685028
void __cdecl Gfx_ForEachModelPolygonSet(int arg, GfxPolygonSetFn callback, PolygonModel *model)
{
    PolygonSetGroup *setGroup;
    GfxEngineContext *pContext;
    PolygonSetEntry *modelCursor;
    int boneIndex;
    int entryIndex;
    int polySet;

    if (model != 0) {
        pContext = GetEngineContext();
        setGroup = (PolygonSetGroup *)model->polySets;
        for (boneIndex = 0; boneIndex < model->polySetCount; boneIndex++) {
            if (setGroup->entryCount > 0) {
                modelCursor = (PolygonSetEntry *)setGroup->entries;
                if (modelCursor != 0) {
                    for (entryIndex = 0; entryIndex < setGroup->entryCount; entryIndex++) {
                        if (modelCursor->modelRef != 0) {
                            polySet = (int)modelCursor->modelRef->polySet;
                            callback(arg, polySet);
                        }
                        modelCursor++;
                    }
                }
            }
            setGroup++;
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Iterates over all polygon sets of a model and saves their current vertex colors.
 */
// FUNCTION: FF7 0x006850db
void __cdecl Gfx_SaveModelVertexColors(PolygonModel *model)
{
    int index;

    index = 0;
    Gfx_ForEachModelPolygonSet(index, (GfxPolygonSetFn)Gfx_SavePolygonSetVertexColorsCallback, model);
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Callback for Gfx_ForEachModelPolygonSet that saves vertex colors of a single polygon set.
 */
// FUNCTION: FF7 0x006850ff
void __cdecl Gfx_SavePolygonSetVertexColorsCallback(int index, GfxPolygonSet *polySet)
{
    Gfx_SavePolygonSetVertexColors(polySet);
}
#pragma optimize("p", on)
} /* extern "C" */

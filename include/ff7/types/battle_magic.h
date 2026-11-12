/* include/ff7/types/battle_magic.h: Spell (Magic) battle effect types. */
#ifndef FF7_TYPES_BATTLE_MAGIC_H
#define FF7_TYPES_BATTLE_MAGIC_H

#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"

/* --- battle_fx_magic_altema_ultima --- */
/* Altema timeline record: frame timer and two packed RGB colour ramps. */
typedef struct BattleFxUltimaTimeline {
    short ownerSlot;
    short timer;
    int colorRampUp;
    int colorRampDown;
} BattleFxUltimaTimeline;

/* Altema spin record initialised when its spin fade model is allocated. */
typedef struct BattleFxUltimaSpinState {
    unsigned char pad_00[0xc];
    short scale;
    short spinSpeed;
    short value;
    short velocity;
    short spinRate;
    short isActive;
} BattleFxUltimaSpinState;

/* Drifting quad-sprite particle of the Altema spell with per-sprite random velocity. */
typedef struct BattleMagicUltimaDriftSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short scale;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    short randomBits;
    unsigned char pad_14[0x2];
    short flag;
} BattleMagicUltimaDriftSprite;

/* Drifting quad sprite task record: per-axis position and velocity with a frame counter. */
typedef struct BattleFxDriftingQuadSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    short rotationSource;
    unsigned char pad_14[0x2];
    short isFadingOut;
} BattleFxDriftingQuadSprite;

/* Draw record of the Altema magic effect: a draw list, a render state and a colour ramp. */
typedef struct BattleFxUltimaDrawRecord {
    int drawList;
    unsigned char pad_04[0x4];
    int renderState;
    unsigned char pad_0C[0x30];
    int stepCount;
    int duration;
    int colorRamp;
} BattleFxUltimaDrawRecord;

/* Spin record of the Altema magic effect: a step counter and an accumulating value with a velocity. */
typedef struct BattleFxUltimaSpinSlot {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x8];
    short scale;
    unsigned char pad_0E[0x2];
    short value;
    short velocity;
    short spinRate;
} BattleFxUltimaSpinSlot;

/* Task slot for the Altema drifting quad sprites, with a phase counter. */
typedef struct BattleFxUltimaDriftState {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x12];
    short velocityTableIndex;
} BattleFxUltimaDriftState;

/* The battle task slot of BattleFx_MagicUltimaGrowingModelTask. */
typedef struct BattleFxUltimaGrowingModelSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x8];
    short value;
    short velocity;
} BattleFxUltimaGrowingModelSlot;

/* Task-slot record for the Magic Altema drifting quad sprites. */
typedef struct BattleFxUltimaDriftingQuad {
    unsigned char pad_00[0x4];
    int colorRampUp;
    int color;
} BattleFxUltimaDriftingQuad;

/* PSX Gouraud quad primitive written by the magic Altema row drawer. */
typedef struct BattleFxPsxGouraudQuad {
    int tag;
    int color0;
    int leftX0;
    int color1;
    int rightX0;
    int color2;
    int leftX1;
    int color3;
    int rightX1;
} BattleFxPsxGouraudQuad;

/* Gouraud quad gradient primitive with primCode 0x3A and doubled vertex words. */
typedef struct BattleFxGouraudQuadPrim {
    unsigned char pad_00[0x7];
    char primCode;
    unsigned char pad_08[0x2];
    short vertex0Y;
    unsigned char pad_0C[0x6];
    short vertex1Y;
    unsigned char pad_14[0x6];
    short vertex2Y;
    unsigned char pad_1C[0x6];
    short vertex3Y;
} BattleFxGouraudQuadPrim;

/* PSX Gouraud quad row whose command byte is set and whose coordinates are doubled. */
typedef struct BattleFxPsxGouraudQuadRow {
    unsigned char pad_00[0x7];
    char command;
    unsigned char pad_08[0x2];
    short vertex0Y;
    unsigned char pad_0C[0x6];
    short vertex1Y;
    unsigned char pad_14[0x6];
    short vertex2Y;
    unsigned char pad_1C[0x6];
    short vertex3Y;
} BattleFxPsxGouraudQuadRow;

/* Gouraud quad gradient descriptor: primitive pointer, row step, row count and colour delta. */
typedef struct BattleFxUltimaGradientDesc {
    unsigned char pad_00[0x8];
    int primPtr;
    unsigned char pad_0C[0x30];
    int rowStep;
    int rowCount;
    int colorDelta;
} BattleFxUltimaGradientDesc;

#pragma pack(push, 1)
typedef struct BattleMagicUltimaRotationVector {
    short spinAngle; /* +0x0 */
    short y; /* +0x2 */
} BattleMagicUltimaRotationVector;
#pragma pack(pop)

#pragma pack(push, 1)
/* The sprite quad request of the drifting quads (BattleFxSpriteQuadRequest, fadeLevel being its
 * cellX), followed by the transform the task sets for the draw: a PsxMatrix whose m[0][0]
 * (rotationValue), m[1][1] (rotationSource) and translation (posX, posY, posZ) the task fills. */
typedef struct BattleMagicUltimaQuadReq {
    short offsetX; /* +0x0 */
    short offsetY; /* +0x2 */
    char fadeLevel; /* +0x4 */
    unsigned char pad_5[0xb];
    int sceneObject; /* +0x10 */
    unsigned char pad_14[0x4];
    short rotationValue; /* +0x18 */
    unsigned char pad_1a[0x6];
    short rotationSource; /* +0x20 */
    unsigned char pad_22[0x6];
    short matrixScale; /* +0x28: m[2][2] */
    int posX; /* +0x2a */
    int posY; /* +0x2e */
    int posZ; /* +0x32 */
    unsigned char pad_36[0x2];
} BattleMagicUltimaQuadReq;
#pragma pack(pop)

/* --- battle_fx_magic_barrier --- */
/* Baria timeline record: effect phase and the record type to spawn; it hands the position and the
 * rotation to each model task it spawns. */
typedef struct BattleFxBarrierTimeline {
    short ownerSlot;
    short phase;
    short allocType;
    unsigned char pad_06[0x2];
    Int64Pair pos;
    Int64Pair rot;
} BattleFxBarrierTimeline;

/* Baria model task as the timeline spawns it: the copied step value, the position and rotation, and a mode value. */
typedef struct BattleFxBarrierState {
    short phase;
    unsigned char pad_02[0x6];
    Int64Pair pos;
    Int64Pair rot;
    short mode;
} BattleFxBarrierState;

/* Three-sample scale history of the Baria effect, shifted by one sample each frame. */
typedef struct BattleFxBarrierScaleHistory {
    int scalePrev;
    int scaleCur;
    int scaleNext;
} BattleFxBarrierScaleHistory;

/* Scale history of the dual Baria effect, padded to the 16 bytes between its two copies. */
typedef struct BattleFxBarrierDualScale {
    int scalePrev;
    int scaleCur;
    int scaleNext;
    int pad_0c;
} BattleFxBarrierDualScale;

/* Baria grow-and-fade model record: frame offset and counter, per-axis deltas, rotation and a flags word. */
typedef struct BattleFxMagicBarrierGrowFadeModel {
    short frameOffset;
    short frameCount;
    unsigned char pad_04[0x4];
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_0E[0x2];
    SVECTOR rot;
    unsigned short flags;
} BattleFxMagicBarrierGrowFadeModel;

/* Per-target Baria effect slot placed at a bone position with a radius offset. */
typedef struct BattleFxBarrierSpawnSlot {
    unsigned char pad_00[0x4];
    short actorIndex;
    unsigned char pad_06[0x2];
    short posX;
    unsigned char pad_0A[0x2];
    short posZ;
    unsigned char pad_0E[0x2];
    Int64Pair rot;
} BattleFxBarrierSpawnSlot;

/* Data block of the Baria model task: the model matrix and the scale history. */
#pragma pack(push, 1)
typedef struct BattleFxBarrierWork {
    PsxMatrix matrix;
    BattleFxBarrierScaleHistory scale;
} BattleFxBarrierWork;

/* Draw request of the Baria model: the model's offset vector, then the draw record that starts at modelHandle. */
typedef struct BattleMagicBarrier_ModelReq1 {
    SVECTOR offset;
    int modelHandle;
    int flags;
    unsigned char pad_10[0x2];
    short alpha;
} BattleMagicBarrier_ModelReq1;

/* Draw request of the dual Baria model. */
typedef struct BattleMagicBarrier_ModelReq2 {
    int modelHandle;
    int flags;
    unsigned char pad_08[0x2];
    short alpha;
    int animFrame;
} BattleMagicBarrier_ModelReq2;
#pragma pack(pop)

/* --- battle_fx_magic_barsac --- */
/* Barsac spinning model record: spin angle and three model values. */
typedef struct BattleFxBerserkSpinRecord {
    short ownerSlot;
    short angle;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
} BattleFxBerserkSpinRecord;

/* Effect-slot record of the Barsac falling sprite task: frame counter, position and velocity. */
typedef struct BattleFxBerserkFallingSprite {
    short slotTag;
    short frame;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxBerserkFallingSprite;

/* Barsac timeline task record with a step counter and a target actor index. */
typedef struct BattleFxBerserkTimelineTask {
    short ownerSlot;
    short step;
    short targetIndex;
} BattleFxBerserkTimelineTask;

/* Barsac sprite task record whose position is advanced by sine and cosine each frame. */
typedef struct BattleFxBerserkSpriteTask {
    unsigned char pad_00[0x8];
    short posX;
    short posHeight;
    short posY;
    unsigned char pad_0E[0x2];
    short orbitX;
    short velocityHeight;
    short orbitZ;
    unsigned char pad_16[0x2];
    short orbitY;
} BattleFxBerserkSpriteTask;

/* Task-slot record for the Magic Barsac effect, one per target. */
typedef struct BattleFxBerserkEffect {
    unsigned char pad_00[0x4];
    short actorIndex;
    short sourceActor;
} BattleFxBerserkEffect;

#pragma pack(push, 1)
typedef struct BattleMagicBerserk_LoadedRes {
    int drawHandle; /* +0x0 */
    struct GfxTableCursor *cursor; /* +0x4 */
    char rotByte8; /* +0x8 */
    unsigned char pad_9[0x1];
    short alpha; /* +0xa */
} BattleMagicBerserk_LoadedRes;
#pragma pack(pop)

/* --- battle_fx_magic_bio --- */
/* Bio timeline task state: step, record type and randomised spawn offsets. */
typedef struct BattleFxBioTimelineState {
    short ownerSlot;
    short step;
    short recordType;
    unsigned char pad_06[0x2];
    short spawnX;
    short spawnY;
    short spawnZ;
} BattleFxBioTimelineState;

/* Effect100 slot record for one Bio particle, spawned around the source position. */
typedef struct BattleFxBioParticle {
    unsigned char pad_00[0x8];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short scale;
    short timer;
} BattleFxBioParticle;

#pragma pack(push, 1)
typedef struct BattleMagicBio_SpriteDrawReq {
    int tableEntry; /* +0x0 */
    int drawObj; /* +0x4 */
    int color; /* +0x8 */
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int tableB; /* +0x10 */
    int tableC; /* +0x14 */
    unsigned char pad_18[0x4];
    short word; /* +0x1c */
    unsigned char pad_1e[0x2];
} BattleMagicBio_SpriteDrawReq;
#pragma pack(pop)

/* --- battle_fx_magic_bio_four --- */
/* Pulsing growth record of the Bio 4 magic effect: a frame counter read by the pulse formulas. */
typedef struct BattleFxBio3MultiPulseSlot {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x4];
    short drawReqValue1;
    unsigned char pad_0A[0x2];
    short drawReqValue2;
} BattleFxBio3MultiPulseSlot;

/* Effect-table particle for the Bio4 timeline, with its base position from the Bio4 table. */
typedef struct BattleBio3MultiParticle {
    unsigned char pad_00[0x4];
    short actorIndex;
    unsigned char pad_06[0x2];
    short posX;
    short height;
    short posY;
    unsigned char pad_0E[0x2];
    short size;
    short riseSpeed;
} BattleBio3MultiParticle;

/* Task slot record for the Bio 4 six-frame sprite emitter; frame counter at +0x02, actor index at +0x04. */
typedef struct BattleMagicBio3MultiEmitterRecord {
    short ownerSlot;
    short frame;
    short actorIndex;
} BattleMagicBio3MultiEmitterRecord;

/* Bio4 six-frame sprite record; position jitters on each spawn. */
typedef struct BattleFxBio3MultiSpriteRecord {
    unsigned char pad_00[0x8];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short scale;
} BattleFxBio3MultiSpriteRecord;

#pragma pack(push, 1)
typedef struct BattleMagicBio3Multi_SpriteDrawReq2 {
    unsigned char pad_0[0x1];
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    char code; /* +0x3 */
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
    int table; /* +0x8 */
    int tableAlt; /* +0xc */
    unsigned char pad_10[0x4];
    short word; /* +0x14 */
    unsigned char pad_16[0x2];
} BattleMagicBio3Multi_SpriteDrawReq2;
#pragma pack(pop)

/* --- battle_fx_magic_bio_three --- */
/* Bio3 wobbling mesh record: phase counter driving scale and angle sine waves. */
typedef struct BattleFxMagicBio3Wobble {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short ratio;
} BattleFxMagicBio3Wobble;

/* Magic Bio3 timeline slot: timeline step, record type and jittered base position. */
typedef struct BattleFxBio3Slot {
    short ownerSlot;
    short step;
    short recordType;
    unsigned char pad_06[0x2];
    union {
        struct {
            short baseX;
            short baseY;
            short baseZ;
        };
        SVECTOR base; /* +0x8 */
    };
    short scale;
} BattleFxBio3Slot;

/* Task slot record for the Bio 3 pulsing scaled model. */
typedef struct BattleFxBio3PulsingModel {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short scale;
} BattleFxBio3PulsingModel;

/* Task slot record for the Bio 3 rising fade sprite emitter. */
typedef struct BattleFxBio3RisingSpriteEmitter {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x4];
    short posX;
    unsigned char pad_0A[0x2];
    short posY;
    unsigned char pad_0E[0x2];
    short spread;
} BattleFxBio3RisingSpriteEmitter;

/* Bio 3 timeline record that stores a randomised position for its spawned model. */
typedef struct BattleFxBio3TimelineRecord {
    short timer;
    unsigned char pad_02[0x6];
    union {
        struct {
            short posX;
            short posY;
            short posZ;
        };
        SVECTOR pos; /* +0x8 */
    };
    short scale;
} BattleFxBio3TimelineRecord;

/* Task-slot record for the Magic Bio3 effect, one per target. */
typedef struct BattleFxBio3Task {
    unsigned char pad_00[0x4];
    short targetIndex;
    unsigned char pad_06[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short effectSize;
} BattleFxBio3Task;

/* GPU textured primitive packet written by BattleFx_MagicBio3DrawPsxMesh. */
typedef struct BattleFxBio3Prim {
    int tag;
    union {
        int colorCmd;
        CVECTOR color; /* +0x4: cd is the GPU command code */
    };
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    short vertexX;
    short bits;
    unsigned char pad_18[0x4];
    short vertexY;
    unsigned char pad_1E[0x6];
    short vertexAOffset;
} BattleFxBio3Prim;

#pragma pack(push, 1)
typedef struct BattleMagicBio3_SpriteDrawReq1 {
    unsigned char pad_0[0x12];
    int transX; /* +0x12: the translation of the matrix in the first 0x1e bytes */
    int transY;
    int transZ;
    unsigned char pad_1e[0x2];
} BattleMagicBio3_SpriteDrawReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBio3_SpriteDrawReq2 {
    unsigned char pad_0[0x10];
    int table; /* +0x10 */
    int tableAlt; /* +0x14 */
} BattleMagicBio3_SpriteDrawReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBio3_SpriteDrawReq3 {
    union {
        int colorWord;
        struct {
            char colorR;
            char colorG;
            char colorB;
            char code;
        };
    };
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleMagicBio3_SpriteDrawReq3;
#pragma pack(pop)

/* --- battle_fx_magic_bio_two --- */
/* Bio 2 timeline slot: frame timer and the record type allocated per step. */
typedef struct BattleFxBio2Timeline {
    short ownerSlot;
    short timer;
    short recordType;
    short param;
    short posX;
    short posY;
    short posZ;
} BattleFxBio2Timeline;

/* Bio2 wobbling mesh task state: step counter used for the sine phase. */
typedef struct BattleFxBio2WobbleState {
    short phaseOffset;
    short step;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
} BattleFxBio2WobbleState;

/* Bio2 effect task record filled with values from a source record and random offsets. */
typedef struct BattleFxBio2Task {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short scale;
} BattleFxBio2Task;

/* Effect-table state for the Bio2 rising fade sprite spawner, with frame index and base position. */
typedef struct BattleBio2SpawnState {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x4];
    short posX;
    unsigned char pad_0A[0x2];
    short posZ;
} BattleBio2SpawnState;

/* GPU textured primitive packet written by BattleFx_MagicBio2DrawPsxMesh. */
typedef struct BattleFxBio2Prim {
    int tag;
    int colorCmd;
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    short vertexX;
    short bits;
    unsigned char pad_18[0x4];
    short vertexY;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxBio2Prim;

/* --- battle_fx_magic_blizzaga_ice --- */
typedef struct PackedColorWord {
    unsigned char channels[4];
} PackedColorWord;

/* A position of the effect: three shorts that are copied as one 8-byte pair. */
typedef union BattleMagicBrizagPos {
    ShadowInt64Pair pair;
    struct {
        short x;
        short y;
        short z;
    };
} BattleMagicBrizagPos;

/* Single-target shatter model state for Brizag: frame counter and shatter parameters. */
typedef struct BattleMagicIce3SingleShatter {
    short ownerSlot;
    short frame;
    BattleMagicBrizagPos pos;
    unsigned char pad_0C[0x4];
    short scale;
} BattleMagicIce3SingleShatter;

/* Group shatter model state for Brizag: phase counter that selects the draw group. */
typedef struct BattleMagicIce3GroupShatter {
    short ownerSlot;
    short phase;
    BattleMagicBrizagPos pos;
    unsigned char pad_0C[0x4];
    short scale;
} BattleMagicIce3GroupShatter;

/* Brizag target timeline record: duration and step size, step divisor from the frame table. */
typedef struct BattleFxIce3Target {
    unsigned char pad_00[0x4];
    BattleMagicBrizagPos pos;
    short angleX;
    short angleY;
    short ratio;
    unsigned char pad_12[0x2];
    short duration;
    short stepSize;
    short stepDivisor;
} BattleFxIce3Target;

/* Brizag target timeline task slot: step index, fixed-point ratio, active flag and index. */
typedef struct BattleFxMagicIce3Target {
    short ownerSlot;
    short step;
    BattleMagicBrizagPos pos;
    unsigned char pad_0C[0x4];
    short ratio;
    unsigned char pad_12[0x6];
    short active;
    short index;
} BattleFxMagicIce3Target;

/* Three shorts per Brizag single target fragment, read through a u16 offset table. */
typedef struct BattleFxIce3FragmentMotion {
    short vertSumX;
    short yTally;
    short vertexZ;
} BattleFxIce3FragmentMotion;

/* Task slot record for the Brizag group timeline, stepped by keyframe. */
typedef struct BattleFxIce3GroupTimeline {
    short ownerSlot;
    short frameCount;
    BattleMagicBrizagPos pos;
    unsigned char pad_0C[0x4];
    short radius;
} BattleFxIce3GroupTimeline;

/* Brizag effect task record set up in the active battle task slot. */
typedef struct BattleFxIce3Task {
    short ownerSlot;
    unsigned char pad_02[0x2];
    BattleMagicBrizagPos pos;
    unsigned char pad_0C[0x4];
    short radius;
    unsigned char pad_12[0x6];
    short isGroup;
    short stageIndex;
} BattleFxIce3Task;

/* One ribbon of the group sequence: the spiral ribbon task slot from posY on, set from seven words
 * of the keyframe table. */
typedef struct BattleFxIce3GroupRecord {
    unsigned char pad_00[0x4];
    int params[7];
} BattleFxIce3GroupRecord;

/* A row of g_BattleMagicIce3KeyframeTable (32 bytes): the frame at which the group timeline starts
 * the ribbon, and the words it copies into the ribbon's task slot. */
typedef struct BattleMagicIce3Keyframe {
    short spawnFrame;
    short pad_02;
    int words[7];
} BattleMagicIce3Keyframe;

/* The battle task slot of BattleFx_MagicIce3SpiralRibbonTask. */
typedef struct BattleFxIce3SpiralRibbonSlot {
    short ownerSlot;
    short frameCounter;
    short posY;
    short angle;
    short angleStep;
    short progress;
    short progressStep;
    short progressLimit;
    short segmentTwistY;
    short endFrame;
    short startFrame;
    short duration;
    short baseX;
    short offsetY;
    short offsetX;
    short colorIndex;
} BattleFxIce3SpiralRibbonSlot;

/* The battle task slot of BattleFx_MagicIce3ConvergingFadeModelTask. */
typedef struct BattleFxIce3ConvergingFadeModelSlot {
    short ownerSlot;
    short frameCounter;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short scale;
    unsigned char pad_12[0x2];
    short decay;
    short decayStep;
    short fadeStart;
} BattleFxIce3ConvergingFadeModelSlot;

/* The battle task slot of BattleFx_MagicIce3ConvergingModelTask. */
typedef struct BattleFxIce3ConvergingModelSlot {
    short ownerSlot;
    short frameCounter;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short scale;
    unsigned char pad_12[0x2];
    short decay;
    short decayStep;
    short convergeStart;
} BattleFxIce3ConvergingModelSlot;

/* Task slot for the Brizag group timeline, copied from the target timeline slot. */
typedef struct BattleMagicIce3GroupSlot {
    unsigned char pad_00[0x4];
    BattleMagicBrizagPos pos;
    unsigned char pad_0C[0x4];
    short ratio;
    unsigned char pad_12[0x6];
    short isGroupTarget;
    short thresholdIndex;
} BattleMagicIce3GroupSlot;

/* First of two ribbon primitives (offsets 0 and 0x24) in the Brizag spiral ribbon record. */
typedef struct BattleFxIce3RibbonHead {
    int tag;
    union {
        int color0Cmd;
        struct {
            unsigned char color0Rgb[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    int color;
    unsigned char pad_10[0x4];
    int color2;
    int vertex2Xy;
    int color3;
    int vertex3Xy;
} BattleFxIce3RibbonHead;

/* Second ribbon primitive at offset 0x24 of the Brizag spiral ribbon record. */
typedef struct BattleFxIce3RibbonTail {
    int tag;
    union {
        int color0Cmd;
        struct {
            unsigned char color0Rgb[3];
            unsigned char code;
        };
    };
    int vertex0Xy;
    int color1;
    int vertex1Xy;
    int color;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxIce3RibbonTail;

/* Three block byte offsets for the A, B and C vertices of a Brizag fragment motion entry. */
typedef struct BattleFxIce3NormalOffsets {
    unsigned short vertexAOffset;
    unsigned short vertexBOffset;
    unsigned short vertexCOffset;
    unsigned char pad_06[0xe]; /* to 0x14 bytes, the step of every pointer through it */
} BattleFxIce3NormalOffsets;

/* Three block byte offsets in a Brizag single-target cursor that select fragment motion vectors. */
typedef struct BattleFxIce3MotionOffsets {
    unsigned short blockOffset0;
    unsigned short blockOffset1;
    unsigned short blockOffset2;
    unsigned char pad_06[0xe]; /* to 0x14 bytes, the step of every pointer through it */
} BattleFxIce3MotionOffsets;

/* Brizag shatter effect: mesh and render state, fragment count and index, base positions, velocities. */
typedef struct BattleIce3Shatter {
    int meshDesc;
    int drawList;
    int otShift;
    int renderState;
    int highWordMask;
    int fragmentCount;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int fragmentIndex;
    int fadeStartFrame;
    PsxMatrix cameraMatrix;    /* +0x2C: the camera's transform */
    PsxMatrix matrix;          /* +0x4A: the fragment's transform; its translation t is the position */
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_74[0x34];
    union {                    /* +0xA8: the effect's centre */
        ShadowInt64Pair base;
        struct {
            short baseX;
            short baseY;
            short baseZ;
        };
    };
    union {                    /* +0xB0: the three corners of the fragment's triangle */
        ShadowInt64Pair fragment0;
        struct {
            short fragment0X;
            short fragment0Y;
            short fragment0Z;
        };
    };
    union {
        ShadowInt64Pair fragment1;
        struct {
            short fragment1X;
            short fragment1Y;
            short fragment1Z;
        };
    };
    union {
        ShadowInt64Pair fragment2;
        struct {
            short fragment2X;
            short fragment2Y;
            short fragment2Z;
        };
    };
    unsigned char pad_C8[0x8];
    union {                    /* +0xD0: the fragment's offset from its centre */
        ShadowInt64Pair velocity;
        struct {
            short velocityX;
            short velocityY;
            short velocityZ;
        };
    };
} BattleIce3Shatter;

/* Brizag group-shatter draw record: data block and draw state copied between fields. */
typedef struct BattleFxMagicIce3ShatterDraw {
    int dataBlock;
    int drawListHandle;
    int otShift;
    int primitive;
    int highWordMask;
    int frontFacing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int fragIx;
    unsigned char pad_28[0x4];
    PsxMatrix cameraMatrix;    /* +0x2C: the camera's transform */
    PsxMatrix matrix;          /* +0x4A: the model transform; its translation t is the position */
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_74[0x34];
    union {                    /* +0xA8: the effect's centre */
        ShadowInt64Pair pos;
        struct {
            short posX;
            short posY;
            short posZ;
        };
    };
    union {                    /* +0xB0: the three vertices of the fragment's triangle */
        ShadowInt64Pair vertex0;
        struct {
            short vertex0X;
            short vertex0Y;
            short vertex0Z;
        };
    };
    union {
        ShadowInt64Pair vertex1;
        struct {
            short vertex1X;
            short vertex1Y;
            short vertex1Z;
        };
    };
    union {
        ShadowInt64Pair vertex2;
        struct {
            short vertex2X;
            short vertex2Y;
            short vertex2Z;
        };
    };
} BattleFxMagicIce3ShatterDraw;

/* Brizag fragment-motion setup block: the mesh data block, the fragment count and the edge and
 * normal vectors of the triangle being set up. */
typedef struct BattleFxMagicIce3Fragment {
    int dataBlock;
    unsigned char pad_04[0x10];
    int duration;
    int distBias;
    unsigned char pad_1C[0x8];
    int fragmentIndex;
    unsigned char pad_28[0x50];
    union {                    /* +0x78: the edge from corner B to corner C */
        VECTOR edgeBc;
        struct {
            int edgeBcX;
            int edgeBcY;
            int edgeBcZ;
        };
    };
    union {                    /* +0x88: the edge from corner A to corner B */
        VECTOR edgeAb;
        struct {
            int edgeAbX;
            int edgeAbY;
            int edgeAbZ;
        };
    };
    union {                    /* +0x98: their cross product, normalized */
        VECTOR normal;
        struct {
            int normalX;
            int normalY;
            int normalZ;
        };
    };
} BattleFxMagicIce3Fragment;

/* One triangle of the shatter mesh (0x14 bytes): the mesh block offsets of its three corners and
 * the vertex colours. */
typedef struct BattleFxShatterEntry {
    unsigned short fragOffsetA;
    unsigned short fragOffsetB;
    unsigned short fragOffsetC;
    unsigned char pad_06[0x2];
    CVECTOR colorA;
    CVECTOR colorB;
    CVECTOR colorC;
} BattleFxShatterEntry;

#pragma pack(push, 1)
typedef struct BattleMagicIce3CoordOffsets1 {
    unsigned char pad_0[0x8];
    short values; /* +0x8: the first of the six shorts the converging models index; the rest lie in the next objects */
    unsigned char pad_a[0x6];
} BattleMagicIce3CoordOffsets1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicIce3CoordOffsets3 {
    unsigned char pad_0[0x8];
    short values; /* +0x8: the first of the six shorts the converging models index; the rest lie in the next objects */
    unsigned char pad_a[0x2];
} BattleMagicIce3CoordOffsets3;
#pragma pack(pop)

/* --- battle_fx_magic_blizzara --- */
/* Child task started by the Brizar 2 timeline: the spin steps (or, for the grow-hold-fade model, the rotation) and the position copied from the timeline. holdFrames is the first word of the slot; 0xFFFF in it frees the task. */
typedef struct BattleFxIce2TimelineSlot {
    short holdFrames;
    unsigned char pad_02[0x6];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0E[0x2];
    SVECTOR pos;
} BattleFxIce2TimelineSlot;

/* Per-task data block of the Brizar 2 grow-hold-fade model: three fade levels shifted each step. */
typedef struct BattleFxIce2GrowHoldFadeState {
    int fadeLevel;
    int holdLevel;
    int growLevel;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix;
} BattleFxIce2GrowHoldFadeState;

/* Spinning shrink record of the Brizar 2 magic effect: a step counter and per-frame spin steps. */
typedef struct BattleFxIce2SpinSlot {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x4];
    SVECTOR spin;
    SVECTOR pos;
} BattleFxIce2SpinSlot;

/* Brizar2 timeline record: a phase timer and the record type it allocates. */
typedef struct BattleFxMagicIce2Timeline {
    short ownerSlot;
    short timer;
    short actorIndex;
    unsigned char pad_06[0xA];
    SVECTOR pos;
} BattleFxMagicIce2Timeline;

/* Brizar2 grow-hold-fade model task record with a frame counter. */
typedef struct BattleFxIce2GrowTask {
    short holdFrames;
    short frame;
    unsigned char pad_04[0x4];
    SVECTOR rotation;
    SVECTOR pos;
} BattleFxIce2GrowTask;

/* Task data block with three interpolated values for the Brizar2 shrinking spin model. */
typedef struct BattleIce2ShrinkState {
    int prevValue;
    int curValue;
    int nextValue;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix;
} BattleIce2ShrinkState;

#pragma pack(push, 1)
typedef struct BattleMagicIce2_ModelReq {
    int modelEntry; /* the spinning model: BattleFxModelDraw modelGroup */
    unsigned char pad_4[0x6];
    short spinFadeLevel; /* its fadeLevel */
    unsigned char pad_c[0x4];
    int growModelEntry; /* the grow-hold-fade model: a second BattleFxModelDraw */
    unsigned char pad_14[0x6];
    short growFadeLevel; /* its fadeLevel */
} BattleMagicIce2_ModelReq;
#pragma pack(pop)

/* --- battle_fx_magic_blizzard --- */
/* Growing ramp record of the Brizad magic effect: a step counter and a scale factor. */
typedef struct BattleFxIceRampSlot {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x4];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_0E[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxIceRampSlot;

/* Brizad ramp model record: a stage counter and the actor index it follows. */
typedef struct BattleFxIceRampRecord {
    short ownerSlot; /* +0x0 */
    short stage;
    short actorIdx;
} BattleFxIceRampRecord;

/* Main task slot of the Brizad ramp model, sized from the actor's scaled effect size. */
typedef struct BattleIceRampSlot {
    unsigned char pad_00[0x8];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_0E[0x2];
    short posX;
    unsigned char pad_12[0x6];
    short effectSize;
} BattleIceRampSlot;

#pragma pack(push, 1)
typedef struct BattleMagicIce_ModelReq {
    int modelEntry; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
} BattleMagicIce_ModelReq;
#pragma pack(pop)

/* --- battle_fx_magic_bolt --- */
/* Thunder timeline record: stage counter, jittered base position and the typed record to allocate. */
typedef struct BattleFxBoltTimeline {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short recordType;
    unsigned char pad_16[0x6];
    short negRadius;
} BattleFxBoltTimeline;

/* Thunder grow-and-fade record: step counter, offset with velocity, and copied model values. */
typedef struct BattleFxBoltGrowRecord {
    short ownerSlot;
    short step;
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0xc];
    short offset;
    short velocity;
} BattleFxBoltGrowRecord;

/* Effect-slot record of the Thunder timeline: position per axis and a random variant. */
typedef struct BattleFxBoltSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short offset;
    short velocity;
    short variant;
    short pushLength;
} BattleFxBoltSlot;

/* Thunder-magic flipped billboard sprite task: frame counter, position, mirror flags. */
typedef struct BattleFxBoltSprite {
    short ownerSlot; /* +0x0 */
    short frameCount;
    short posX;
    unsigned char pad_06[0x14];
    short flags;
    short pushLength;
} BattleFxBoltSprite;

/* Task slot for a Thunder billboard sprite that counts its frames. */
typedef struct BattleFxBoltBillboardSlot {
    short ownerSlot; /* +0x0 */
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x12];
    short pushLength;
} BattleFxBoltBillboardSlot;

/* Task-slot record for the Magic Thunder effect, one per target. */
typedef struct BattleFxBoltRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short actorId;
    unsigned char pad_16[0x6];
    short negRadius;
} BattleFxBoltRecord;

#pragma pack(push, 1)
typedef struct BattleMagicBolt_ActorMatrix {
    short scaleX; /* +0x0 */
    unsigned char pad_2[0x8];
    short alpha; /* +0xa */
    unsigned char pad_c[0x2];
    short offset; /* +0xe */
    unsigned char pad_10[0x2];
    int offsetX; /* +0x12 */
    int offsetY; /* +0x16 */
    int offsetZ; /* +0x1a */
} BattleMagicBolt_ActorMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBolt_ModelReq {
    int tableField; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
    int resourceHandle; /* +0x10 */
    int altHandle; /* +0x14 */
} BattleMagicBolt_ModelReq;
#pragma pack(pop)

/* --- battle_fx_magic_bolt3 --- */
/* Magic Thunder3 timeline slot: timeline step, battler index and jittered base position. */
typedef struct BattleFxBolt3Slot {
    short ownerSlot;
    short step;
    short battlerIndex;
    unsigned char pad_06[0x2];
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0E[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_16[0x2];
    short pushLength;
} BattleFxBolt3Slot;

/* Grow and fade model record for the Thunder3 effect. */
typedef struct BattleFxBolt3GrowModel {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short rotX;
    short rotY;
    short rotZ;
} BattleFxBolt3GrowModel;

/* Effect-slot particle of the Thunder 3 timeline: position and velocity per axis. */
typedef struct BattleFxBolt3Particle {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    char angleStepX;
    char angleStepY;
    char angleStepZ;
    unsigned char pad_1B[0x1];
    short scale;
} BattleFxBolt3Particle;

#pragma pack(push, 1)
typedef struct BattleMagicBolt3_ModelReq1 {
    unsigned char pad_0[0x18];
    int modelHandle; /* +0x18 */
    int flags; /* +0x1c */
    unsigned char pad_20[0x2];
    short scaleFraction; /* +0x22 */
} BattleMagicBolt3_ModelReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBolt3_ModelReq2 {
    int modelRequest; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
} BattleMagicBolt3_ModelReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBolt3_ModelReq3 {
    unsigned char pad_0[0x12];
    int transX; /* +0x12 */
    int transY; /* +0x16 */
    int transZ; /* +0x1a */
    unsigned char pad_1e[0x2];
    int spriteRecord; /* +0x20 */
    int spriteObject; /* +0x24 */
    unsigned char pad_28[0x4];
    short spriteFrame; /* +0x2c */
    unsigned char pad_2e[0x2];
    int shellModel; /* +0x30 */
    unsigned char pad_34[0x4];
    short shellUvOffset; /* +0x38 */
    short shellAlpha; /* +0x3a */
    short shellBlendMode; /* +0x3c */
    unsigned char pad_3e[0x2];
    int coreModel; /* +0x40 */
    unsigned char pad_44[0x6];
    short coreAlpha; /* +0x4a */
} BattleMagicBolt3_ModelReq3;
#pragma pack(pop)

/* --- battle_fx_magic_bolt4 --- */
/* Colour, primitive code and flip bit of the flipped thunder sprite request. */
typedef struct BattleFxThunderSpriteColor {
    char colorR;
    char colorG;
    char colorB;
    unsigned char code;
    short flip;
    unsigned char pad_06[0x2];
} BattleFxThunderSpriteColor;

/* Bouncing-model request for the Thunder4 magic; its fadeLevel is copied to the draw request. */
typedef struct BattleMagicThunderModelReq {
    unsigned char pad_00[0x18];
    int modelHandle;
    int flags;
    unsigned char pad_20[0x2];
    short fadeLevel;
} BattleMagicThunderModelReq;

/* Main task slot of the Thunder 4 flash sprite, counting frames. */
typedef struct BattleBolt3MultiFlashSlot {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x4];
    short posX;
    unsigned char pad_0A[0x2];
    short posZ;
} BattleBolt3MultiFlashSlot;

/* Sprite request for the Thunder4 magic, with its resource, frame and level. The model draw
 * record BattleFxModelDraw starts at modelGroup. */
typedef struct BattleFxBolt3MultiSpriteReq {
    BattleFxMagicDemi3MultiDrawReq *drawReq;
    int resource;
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char code;
    short frame;
    unsigned char pad_0E[0x2];
    int modelGroup;
    int flags;
    unsigned char pad_18[0x2];
    short level;
} BattleFxBolt3MultiSpriteReq;

/* Thunder 4 flipped sprite model request, filled from the table cursor. */
typedef struct BattleFxMagicBolt3MultiModelRequest {
    unsigned char pad_00[0x20];
    int tableValueA;
    int tableValueB;
} BattleFxMagicBolt3MultiModelRequest;

/* Thunder4 sprite request record for the grow-hold-fade model task. */
typedef struct BattleFxBolt3MultiBoltMatrix {
    unsigned char pad_00[0xa];
    short m12;
    unsigned char pad_0C[0x2];
    short m21;
} BattleFxBolt3MultiBoltMatrix;

/* --- battle_fx_magic_break --- */
/* Task slot of the Break effect. The per-target spawner stores the target actor index and the
 * bone's world position (Battle_GetBoneWorldPos) and the actor's effect size; the timeline task
 * counts frames in it and copies posX/posZ/size into the three model tasks, which use the same
 * layout and give the slot back with BATTLE_TASK_FREE in ownerSlot. */
typedef struct BattleFxMagicBreakTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short size;
} BattleFxMagicBreakTask;

/* Magic break black model data record, with a table field and a growth value. */
typedef struct BattleFxMagicBreakModelData {
    unsigned char pad_00[0x10];
    int modelGroup;
    unsigned char pad_14[0x6];
    short scale;
} BattleFxMagicBreakModelData;

/* The last element and the translation of the black model's matrix: the matrix starts at
 * g_BattleMagicBreak_BlackMatrix and ends in the first bytes of g_BattleMagicBreak_ModelData3. */
#pragma pack(push, 1)
typedef struct BattleFxMagicBreakMatrixTail {
    short m22;
    int transX;
    int transY;
    int transZ;
} BattleFxMagicBreakMatrixTail;
#pragma pack(pop)

/* --- battle_fx_magic_brizag2_debris --- */
/* BattleFx effect slot (0x20 stride) for the Brizag spiral ribbon: timing, angle and colour index. */
typedef struct BattleFxSpiralRibbonTask {
    short ownerSlot;
    short frameCount;
    short posY;
    short outerAngleY;
    short outerAngleStepY;
    short segmentCount;
    short segmentStep;
    short segmentLimit;
    short innerAngleY;
    short endFrame;
    short startFrame;
    short duration;
    short baseX;
    short offsetY;
    short offsetX;
    short colorIndex;
} BattleFxSpiralRibbonTask;

/* Effect-100 slot record (0x20 stride) for the Brizag 2 timeline task. */
typedef struct BattleFxIce3MultiTimelineSlot {
    short ownerSlot;
    short timer;
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short elapsed;
} BattleFxIce3MultiTimelineSlot;

/* Brizag 2 converging fade model slot: timer, decaying value with step, and convergence start frame. */
typedef struct BattleFxIce3MultiConvergingModel {
    short ownerSlot;
    short timer;
    union {
        Int64Pair pos;
        struct {
            short centerX;
            short centerY;
            short centerZ;
        };
    };
    short angleX;
    short angleY;
    short scale;
    unsigned char pad_12[0x2];
    short decay;
    short decayStep;
    short convergeStart;
} BattleFxIce3MultiConvergingModel;

/* Converging-model state for the second Brizag magic: frame counter, step values and duration. */
typedef struct BattleMagicIce3MultiConverging {
    short ownerSlot;
    short frame;
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short scale;
    unsigned char pad_12[0x2];
    short radius;
    short radiusStep;
    short duration;
} BattleMagicIce3MultiConverging;

/* Effect-100 slot timeline record for the Brizag 2 magic sequence. */
typedef struct BattleFxIce3MultiSlot {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos;
        struct {
            unsigned char pad_04[0x2];
            short timer;
        };
    };
    unsigned char pad_0C[0x4];
    short elapsed;
    short stepSize;
    short yOffset;
    short yOffsetStep;
    unsigned char pad_18[0x2];
    short stageIndex;
} BattleFxIce3MultiSlot;

/* Brizag2 shatter record: step counter, an origin and a velocity that accumulates an acceleration. */
typedef struct BattleFxIce3MultiShatterRecord {
    short ownerSlot;
    short step;
    short posX;
    short origin;
    short posZ;
    unsigned char pad_0A[0x6];
    short scale;
    unsigned char pad_12[0x2];
    short velocity;
    short acceleration;
} BattleFxIce3MultiShatterRecord;

/* Brizag2 directed sprite record: draw frame and a position advanced by per-axis velocities. */
typedef struct BattleFxIce3MultiDirectedSprite {
    short ownerSlot;
    short spriteFrame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    VECTOR direction;
    short scale;
    short speed;
} BattleFxIce3MultiDirectedSprite;

/* Brizag2 debris piece (0x28 bytes): centre, spin angles with steps, offset and velocity; flag is set once the piece is spent. */
typedef struct BattleFxBrizagDebrisShadedTri {
    union {
        Int64Pair center;
        struct {
            short centerX;
            short centerY;
            short centerZ;
        };
    };
    short angleX;
    short angleY;
    short angleZ;
    short flag;
    short angleStepX;
    unsigned char pad_12[0x2];
    short angleStepZ;
    unsigned char pad_16[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_1E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    short accelY;
} BattleFxBrizagDebrisShadedTri;

/* Effect-slot record of the Brizag 2 shatter model task: frame counter and pass-through values. */
typedef struct BattleFxIce3MultiShatterSlot {
    short slotTag;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short scale;
    short scaleStep;
} BattleFxIce3MultiShatterSlot;

/* Per-task data block of the Brizag 2 directed sprite: saved position, delayed values, velocities. */
#pragma pack(push, 1)
typedef struct BattleFxDirectedSpriteState {
    unsigned char pad_00[0x52];
    union {
        PsxMatrix matrix;
        struct {
            unsigned char pad_52[0x12];
            int savedX;
            int savedY;
            int savedZ;
        };
    };
    int olderValue;
    int oldValue;
    int newValue;
    unsigned char pad_7C[0x4];
    union {
        VECTOR velocity;
        struct {
            int velocityX;
            int velocityY;
            int velocityZ;
        };
    };
    VECTOR axis;
} BattleFxDirectedSpriteState;
#pragma pack(pop)

/* Per-slot state of the Brizag2 converging models task, with a stage index. */
typedef struct BattleFxMagicIce3MultiConvergeState {
    short ownerSlot;
    short stage;
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short savedScale;
    unsigned char pad_12[0x8];
    short stageIndex;
} BattleFxMagicIce3MultiConvergeState;

/* Per-slot state of the Brizag2 directed sprites task, with a frame counter and base positions. */
typedef struct BattleFxMagicIce3MultiDirectedSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x6];
    short size;
} BattleFxMagicIce3MultiDirectedSprite;

/* Brizag 2 spiral ribbon slot holding copied subeffect parameter words. */
typedef struct BattleFxIce3MultiSpiralRibbon {
    unsigned char pad_00[0x4];
    int param0;
    int param1;
    int param2;
    int param3;
    int param4;
    int param5;
    int colorIndex;
} BattleFxIce3MultiSpiralRibbon;

/* Textured debris quad primitive record for the Brizag 2 magic draw. */
typedef struct BattleFxIce3MultiDebrisQuad {
    int tag;
    int primColor;
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    int flags;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxIce3MultiDebrisQuad;

/* GPU textured-triangle primitive header written from a mesh record for Brizag2. */
typedef struct BattleMeshTexturedPrimitive {
    int tag;
    unsigned char rgb[0x3];
    char command;
    unsigned char pad_08[0x4];
    int uvWord0;
    unsigned char pad_10[0x4];
    int primFlags;
    unsigned char pad_18[0x4];
    short uvWord2;
} BattleMeshTexturedPrimitive;

/* GPU textured-triangle primitive header written for Brizag2 debris fragments. */
typedef struct BattleDebrisTexturedPrimitive {
    int tag;
    int colorCommand;
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    int primFlags;
    unsigned char pad_18[0x4];
    short uv2;
} BattleDebrisTexturedPrimitive;

/* Mesh draw descriptor for the Brizag magic: vertex-offset words and a primitive code word. */
typedef struct BattleFxBrizagMeshDesc {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
    int uv0Clut;
    unsigned short flags;
    short uv2;
    short uv3;
    unsigned short packedCommand;
} BattleFxBrizagMeshDesc;

/* Brizag2 mesh triangle: three vertex offsets and a packed command word for the textured draw. */
typedef struct BattleFxIce3MultiMeshTriangle {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short packedCommand;
    int uv0Clut;
    unsigned short flags;
    short uv2;
} BattleFxIce3MultiMeshTriangle;

/* Directed-sprite task slot whose position is the caster position plus a scaled offset. */
typedef struct BattleFxDirectedSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    VECTOR direction;
    short scale;
    short speed;
} BattleFxDirectedSprite;

/* Task slot set up for the Brizag 2 magic timeline. */
typedef struct BattleMagicIce3MultiSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    union {
        Int64Pair pos;
        struct {
            unsigned char pad_04[0x2];
            short posY;
        };
    };
    unsigned char pad_0C[0x4];
    short effectScale;
} BattleMagicIce3MultiSlot;

/* One 32-byte entry of the ribbon spawn table: the frame it starts on and the words copied into the ribbon task. */
typedef struct BattleMagicIce3MultiRibbonSpawn {
    short startFrame;
    short pad_02;
    int param0;
    int param1;
    int param2;
    int param3;
    int param4;
    int param5;
    int colorIndex;
} BattleMagicIce3MultiRibbonSpawn;

/* Centroid of the targets: the Int64Pair is copied from the timeline slot, the ribbons read x and z. */
typedef union BattleMagicBrizag2CentroidPos {
    Int64Pair pair;
    struct {
        short x;
        short y;
        short z;
    };
} BattleMagicBrizag2CentroidPos;

/* Colour bytes of the Brizag2 sprite quad draw request. */
typedef struct BattleMagicIce3MultiQuadColors {
    unsigned char pad_00[0x1];
    char colorG;
    char colorB;
} BattleMagicIce3MultiQuadColors;

/* Brizag2 sprite quad draw slot, one 0x14-byte entry per quad. */
typedef struct BattleMagicIce3MultiQuadSlot {
    int quadState;
    unsigned char pad_04[0x10];
} BattleMagicIce3MultiQuadSlot;

/* First of two ribbon primitives (offsets 0 and 0x24) in the Brizag 2 spiral ribbon record. */
typedef struct BattleFxIce3MultiRibbonHead {
    int tag;
    int color0;
    int vertex0;
    int color1;
    int vertex1;
    int color2;
    int vertex2;
    int color3;
    int vertex3;
} BattleFxIce3MultiRibbonHead;


/* Shaded quad record: four vertex offsets and a command word whose top byte is the command. */
typedef struct BattleFxShadedQuadRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned short vertexOffset3;
    unsigned int colorCommand;
    unsigned int color1;
    unsigned int color2;
    unsigned int color3;
} BattleFxShadedQuadRecord;

/* Mesh triangle record: three vertex byte offsets and a command word whose top byte is kept. */
typedef struct BattleFxIce3MultiMeshTriangleRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned char pad_06[0x2];
    unsigned int commandWord;
    unsigned int color1;
    unsigned int color2;
} BattleFxIce3MultiMeshTriangleRecord;

/* Brizag2 debris state: mesh and debris record pointers, a previous position triple and per-frame deltas. */
typedef struct BattleFxIce3MultiDebris {
    int meshPtr;
    unsigned char pad_04[0x28];
    struct BattleFxBrizagDebrisShadedTri *debrisPtr;
    unsigned char pad_30[0x50];
    union {
        VECTOR delta;
        struct {
            int deltaX;
            int deltaY;
            int deltaZ;
        };
    };
    unsigned char pad_90[0x20];
    short prevX;
    short prevY;
    short prevZ;
} BattleFxIce3MultiDebris;

/* Brizag2 shatter record: mesh, render state, debris corners and per-axis step deltas. */
#pragma pack(push, 1)
typedef struct BattleFxIce3MultiShatter {
    int meshData;
    int otBucket;
    int otShift;
    int renderState;
    int clearedFlag;
    int primFlags;
    int timer;
    unsigned char pad_1C[0x4];
    int orderingDepth;
    int flags;
    int color;
    BattleFxBrizagDebrisShadedTri *debris;
    unsigned char pad_30[0x4];
    PsxMatrix baseMatrix;
    union {
        PsxMatrix pieceMatrix;
        struct {
            unsigned char pad_52[0x12];
            int posX;
            int posY;
            int posZ;
        };
    };
    unsigned char pad_70[0x48];
    union {
        Int64Pair vertex0;
        struct {
            short vertex0X;
            short vertex0Y;
            short vertex0Z;
        };
    };
    union {
        Int64Pair vertex1;
        struct {
            short vertex1X;
            short vertex1Y;
            short vertex1Z;
        };
    };
    union {
        Int64Pair vertex2;
        struct {
            short vertex2X;
            short vertex2Y;
            short vertex2Z;
        };
    };
    union {
        Int64Pair vertex3;
        struct {
            short vertex3X;
            short vertex3Y;
            short vertex3Z;
        };
    };
    union {
        Int64Pair delta;
        struct {
            short deltaX;
            short deltaY;
            short deltaZ;
        };
    };
} BattleFxIce3MultiShatter;
#pragma pack(pop)

/* --- battle_fx_magic_comet --- */
/* Magic comet particle: position and velocity per axis, with random spread values. */
typedef struct BattleFxMagicCometParticle {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    char rotStepX;
    char rotStepY;
    char rotStepZ;
    unsigned char pad_1B[0x1];
    short scale;
    short modelVariant;
} BattleFxMagicCometParticle;

/* Task-slot record of the Comet magic timeline: a record type, a step counter and two target positions. */
typedef struct BattleFxCometTimelineSlot {
    short recordType;
    short step;
    short targetX;
    short targetY;
    short targetZ;
} BattleFxCometTimelineSlot;

/* --- battle_fx_magic_cometeo --- */
/* Effect slot for a meteor debris particle: position, velocity and tilt, randomised at spawn. */
typedef struct BattleFxMeteorParticle {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x6];
    char tiltX;
    char tiltY;
    char tiltZ;
    unsigned char pad_1B[0x1];
    short randomSpan;
    short randomFlag;
} BattleFxMeteorParticle;

/* Effect-100 slot holding the target point and step of the Cometeo meteor sequence. */
typedef struct BattleFxComet2MeteorSlot {
    short ownerSlot;
    short step;
    short targetX;
    short targetY;
    short targetZ;
} BattleFxComet2MeteorSlot;

/* Effect-table timeline record for the Cometeo task, with a sequence index and frame index. */
typedef struct BattleComet2Timeline {
    short sequenceIndex;
    short frameIndex;
} BattleComet2Timeline;

/* --- battle_fx_magic_debaria_shatter --- */
/* Three shorts per Debaria shatter fragment, read through a u16 offset table. */
typedef struct BattleFxDeBarrierShatterFragment {
    short positionX;
    short positionY;
    short zCenter;
} BattleFxDeBarrierShatterFragment;

/* Shatter-model spawn record written per target by the Debaria magic. */
typedef struct BattleFxDeBarrierSpawnRecord {
    unsigned char pad_00[0xc];
    short yaw;
    short zOffset;
    short scaledExtent;
    short targetIndex;
    short bufBank;
} BattleFxDeBarrierSpawnRecord;

/* The battle task slot of BattleFx_MagicDeBarrierFlashBillboardModelTask. */
typedef struct BattleFxDeBarrierFlashBillboardModelSlot {
    short ownerSlot;
    short frameCounter;
    short velA;
    short velB;
    short velC;
    unsigned char pad_0A[0x2];
    short yaw;
    short offsetDistance;
    short flashLevel;
} BattleFxDeBarrierFlashBillboardModelSlot;

/* The battle task slot of BattleFx_MagicDeBarrierShatterModelTask. */
typedef struct BattleFxDeBarrierShatterModelSlot {
    short ownerSlot;
    short frameCounter;
    short velA;
    short velB;
    short velC;
    unsigned char pad_0A[0x2];
    short yaw;
    short offsetDistance;
    short scale;
    short actorIndex;
    short bankIndex;
} BattleFxDeBarrierShatterModelSlot;

/* Debaria flash billboard task; yaw copies the shatter yaw and angle is an angle. */
typedef struct BattleFxDeBarrierFlashTask {
    unsigned char pad_00[0xc];
    short yaw;
    short angle;
    short extent;
} BattleFxDeBarrierFlashTask;

/* One shard of the Debaria barrier: position, depth, rotation angles and spin, as shorts. */
typedef struct BattleMagicDebariaShard {
    short posX;
    short posY;
    short posZ;
    short depth;
    short angleX;
    short angleY; /* always 0 */
    short angleZ;
    short age;
    short spinX;
    short velY;
    short spinZ;
    short accelY;
} BattleMagicDebariaShard;

/* Eight banks of 64 shards; the first bank is this object, the rest is the storage after it. */
typedef struct BattleMagicDeBarrierFragmentStates {
    BattleMagicDebariaShard shards[64];
} BattleMagicDeBarrierFragmentStates;

/* --- battle_fx_magic_dejon --- */
/* Dejon particle timeline slot: start position and velocity per axis toward the target, plus index. */
typedef struct BattleFxRemoveParticle {
    unsigned char pad_00[0x4];
    union {
        Int64Pair position;
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short scale;
    short index;
} BattleFxRemoveParticle;

/* Dejon effect read record with a value and a row tint. */
typedef struct BattleFxRemoveRecord {
    int value;
    int rowTint;
} BattleFxRemoveRecord;

#pragma pack(push, 1)
typedef struct BattleMagicRemove_LoadedRes {
    struct GfxTableCursor *tableCursor; /* +0x0 */
    unsigned char pad_4[0x4];
    char rotByte8; /* +0x8 */
    unsigned char pad_9[0x1];
    short alpha; /* +0xa */
} BattleMagicRemove_LoadedRes;
#pragma pack(pop)

/* One palette row of the vortex layers: a tint and the 14 colors the rows cycle through. */
typedef struct {
    int unused0;
    int tintColor;
    int cycleColor[14];
} BattleMagicRemove_QuadRecord;

/* Centroid of the target actors, filled by Battle_GetActorMaskCentroid. */
typedef struct {
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_6[0x2];
} BattleMagicRemove_TargetPosition;

#pragma pack(push, 1)
typedef struct BattleMagicRemove_ModelReq {
    int modelEntry; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleMagicRemove_ModelReq;
#pragma pack(pop)

/* --- battle_fx_magic_demi_grav1 --- */
/* Grav1 travel emitter slot: position, per-frame velocity and actor index. */
typedef struct BattleFxMagicDemiEmitter {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short actorIndex;
} BattleFxMagicDemiEmitter;

/* Travel record of the Grav 1 magic effect: target positions and velocities derived from the distance. */
typedef struct BattleFxDemiTravelSlot {
    unsigned char pad_00[0x4];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short targetActor;
} BattleFxDemiTravelSlot;

/* Grav1 expanding sprite task record with a frame counter and a size that grows by a step. */
typedef struct BattleFxDemiExpandTask {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x10];
    short size;
    short sizeStep;
} BattleFxDemiExpandTask;

/* Grav1 travel emitter slot borrowed from other Grav1 tasks; mode is set to 1 or 2. */
typedef struct BattleFxDemiEmitterSlot {
    short ownerSlot;
    short mode;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short size;
    short sizeStep;
} BattleFxDemiEmitterSlot;

/* Sprite request record for Magic Gravity 1 whose three words are table fields 2 to 4. */
typedef struct BattleMagicDemiSpriteReq {
    unsigned char pad_00[0x20];
    int spriteObject2;
    int spriteObject3;
    int spriteObject4;
} BattleMagicDemiSpriteReq;

/* Sprite colour record of the trail sprite: texture bytes and the current frame. */
typedef struct BattleMagicDemiSpriteColors {
    unsigned char texture[4];
    short frame;
    short pad_06;
} BattleMagicDemiSpriteColors;

#pragma pack(push, 1)
typedef struct BattleMagicDemi_SpriteReq {
    int expandFrameTable;
    int expandVariantField;
    unsigned char pad_8[0x8];
    int animFrameTable;
    int animVariantField;
    unsigned char pad_18[0x4];
    short animFramePhase;
    unsigned char pad_1e[0x2];
} BattleMagicDemi_SpriteReq;
#pragma pack(pop)

/* --- battle_fx_magic_deth --- */
/* One smoke particle of the Deth smoke ring: its position and billboard scale. */
typedef struct {
    short x;
    short y;
    short z;
    short scale;
} BattleMagicDethSmokePoint;

/* The Deth smoke ring: the write cursor, then 0x90 smoke points. The object is 20 bytes; the
 * points run on into the bytes that follow it. */
typedef struct {
    int writePos;
    unsigned char pad_4[0x4];
    BattleMagicDethSmokePoint points[1]; /* +0x8 */
    unsigned char pad_10[0x4];
} BattleMagicDeathSmokeRing;

/* Death spiral sprite slot: timer, velocity, radius, scale and phase with per-frame steps. */
typedef struct BattleFxDeathSpiralSprite {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleStep;
    short scale;
    short radialOffset;
    short yStep;
    short radialStep;
} BattleFxDeathSpiralSprite;

/* Per-slot Magic Deth timeline state: step counter, base position and a clamped stage counter. */
typedef struct BattleFxDeathState {
    short ownerSlot;
    short step;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x2];
    short angle;
    unsigned char pad_0E[0x4];
    short stage;
    unsigned char pad_14[0x4];
    short flag;
} BattleFxDeathState;

/* Deth magic target position: X, height and Z shorts, used by the curved strip trail. */
typedef struct BattleMagicDeathTarget {
    short targetX;
    short targetHeight;
    short targetZ;
    unsigned char pad_6[0x2];
} BattleMagicDeathTarget;

/* GPU render-state packet for the Deth Gouraud quad: tag word and vertex and colour links. */
typedef struct BattleFxPsxGouraudQuadPacket {
    int tag;
    int color0;
    int ndcX0;
    int color1;
    int ndcY0;
    int color2;
    int prevNdcX0;
    int color3;
    int prevNdcY0;
} BattleFxPsxGouraudQuadPacket;

/* GPU render-state packet for the Deth Gouraud quad strip: tag word and vertex and colour links. */
typedef struct BattleFxPsxGouraudStripPacket {
    int tag;
    int color0;
    int nextCoordA;
    int color1;
    int coordA;
    int color2;
    int nextCoordB;
    int color3;
    int coordB;
} BattleFxPsxGouraudStripPacket;

/* Sprite record of the Deth magic effect, filled with random ranges when each sprite spawns. */
typedef struct BattleFxDeathSpriteSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleStep;
    short scale;
    short radialOffset;
    short yStep;
    short radialStep;
    short ringStartIndex;
} BattleFxDeathSpriteSlot;

/* Ring-buffer record of the Deth strip tasks: the trail's start position and angle. */
typedef struct BattleFxDeathRingRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short angle;
} BattleFxDeathRingRecord;

/* Slots BattleFx_MagicDeathPlaceEffectTask allocates for BattleFx_MagicDeth{PlaceEffect, Timeline}Task. */
typedef struct BattleFxDeathPlaceEffectSpawnSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleBetweenVectors;
    unsigned char pad_0E[0x2];
    short spawnIndex;
    short stage;
    unsigned char pad_14[0x4];
    short flag;
} BattleFxDeathPlaceEffectSpawnSlot;

/* One step of the Death magic trail: start and end points, an active flag and the curve's normal. */
typedef struct BattleFxDeathTrailStep {
    short startPos;
    short startHeight;
    short posA;
    unsigned char pad_06[0x2];
    short endPos;
    short endHeight;
    short posB;
    short active;
    VECTOR normal;
} BattleFxDeathTrailStep;

/* Deth magic reference block: the attachment point, the forward vector the spawn angle is measured
 * against, and the sprite request. */
typedef struct BattleFxMagicDeathSpriteDrawReq {
    short attachPos[3];
    unsigned char pad_06[0x2];
    VECTOR reference;
    BattleFxSpriteRequest sprite;
} BattleFxMagicDeathSpriteDrawReq;

/* One edge of the curved Deth strip: the start and end points as {x, height, z} short vectors. */
typedef struct BattleFxDeathTrailEdge {
    short startX;
    short startHeight;
    short startY;
    unsigned char pad_06[0x2];
    short endX;
    short endHeight;
    short endY;
    unsigned char pad_0E[0x2];
} BattleFxDeathTrailEdge;

/* The block Gfx_GetPsxScratchpadPtr(0) returns, which both Deth strip trails project and draw through. The
 * linear trail uses the first two screen points and the projection results of the first two
 * edge points; the curved trail subdivides each segment into nine points and keeps the previous
 * segment's nine in the second set of arrays. It is packed: the three normal vectors start at
 * odd offsets. */
#pragma pack(push, 1)
typedef struct BattleFxDeathStripTrail {
    unsigned char pad_00[0x4];
    int projectFlag1;
    int depth;
    int projectFlag2;
    short stripAX;
    short stripAHeight;
    short stripAY;
    unsigned char pad_16[0x2];
    short stripBX;
    short stripBHeight;
    short stripBY;
    unsigned char pad_1E[0x2];
    short posAX;
    short posAHeight;
    short posAY;
    unsigned char pad_26[0x2];
    short posBX;
    short posBHeight;
    short posBY;
    unsigned char pad_2E[0x2];
    int screenPoint[9];
    unsigned char pad_54[0x4];
    int pointDepth[9];
    unsigned char pad_7C[0x4];
    int prevScreenPoint[9];
    unsigned char pad_A4[0x4];
    int prevPointDepth[9];
    unsigned char pad_CC[0x4];
    int prevDepth;
    union {
        int shadeStartColor;
        CVECTOR shadeStartRgb;
    };
    union {
        int colRemaining;
        CVECTOR colRemainingRgb;
    };
    union {
        int rColor;
        CVECTOR rColorRgb;
    };
    int distance;
    int segmentLength;
    int savedDistance;
    int savedSegmentLength;
    PsxMatrix rotMatrix;
    VECTOR normalA;
    VECTOR normalB;
    VECTOR cross;
    unsigned char pad_13E[0x12];
    int ndcZ[9];
    unsigned char pad_174[0x4];
    int ndcW[9];
    unsigned char pad_19C[0x4];
    int prevNdcZ[9];
    unsigned char pad_1C4[0x4];
    int prevNdcW[9];
} BattleFxDeathStripTrail;
#pragma pack(pop)

/* --- battle_fx_magic_dethper_fragments --- */
/* The world position of a target's bone (8 bytes, filled by Battle_GetBoneWorldPos and copied whole). */
typedef struct BattleFxDethperBonePos {
    short posX;
    short posY;
    short posZ;
    short pad;
} BattleFxDethperBonePos;

/* Dethper spawner record: frame counter, model size and stage index into the frame thresholds. */
typedef struct BattleFxDespellModelSpawner {
    short ownerSlot;
    short frameCount;
    BattleFxDethperBonePos pos;
    unsigned char pad_0C[0x4];
    short zDepth;
    short size;
    short scale;
    unsigned char pad_16[0x4];
    short stage;
} BattleFxDespellModelSpawner;

/* A fragment vertex: three coordinates in the 8-byte slot StoreDeferredDrawArgs reads. */
typedef struct BattleFxDespellVertex {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
} BattleFxDespellVertex;

/* The data block of the Dethper fragment stream: the effect data and draw list it walks, the
 * primitive cursor, per-fragment motion page and the four vertices of the fragment being drawn. */
typedef struct BattleFxDespellFragment {
    int *effectData;
    int drawList;
    int scaleShift;
    int primPtr;
    int flags;
    int frontFacing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int bufferIdx;
    unsigned char pad_28[0x4];
    int pageSlot;
    unsigned char pad_30[0x84];
    BattleFxDespellVertex vertex[4];
} BattleFxDespellFragment;

/* One fragment's motion: offset from its rest position and the step added each frame. */
typedef struct BattleFxDethperFragmentFace {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_0E[0xa];
} BattleFxDethperFragmentFace;

/* The motion records of one target's mesh; g_BattleMagicDespellFragmentBuffer is addressed
 * by target page. */
typedef struct BattleFxDespellFragmentPage {
    BattleFxDethperFragmentFace face[24];
} BattleFxDespellFragmentPage;

/* g_BattleMagicDespellReferenceVector: the camera-facing direction (0, 0, -0x1000) the three
 * Dethper model tasks measure the camera angle from. The third copy is only its first word here;
 * the rest lies in the bytes after the object. */
typedef struct BattleMagicDespellReferenceVector {
    VECTOR vector0;
    VECTOR vector1;
    int vector2;
} BattleMagicDespellReferenceVector;

/* Per-target timeline record for the Dethper spell effect. */
typedef struct BattleFxDespellTarget {
    unsigned char pad_00[0x4];
    BattleFxDethperBonePos pos;
    short angle;
    short angleSpeed;
    short zDepth;
    unsigned char pad_12[0x2];
    short scale;
    unsigned char pad_16[0x4];
    short targetIndex;
    short targetCount;
} BattleFxDespellTarget;

/* Per-slot state for the Dethper camera-facing models, set from a stage record. */
typedef struct BattleFxMagicDespellModelState {
    unsigned char pad_00[0x4];
    BattleFxDethperBonePos pos;
    short angle;
    short angleSpeed;
    short offsetZ;
    short size;
    short scale;
    short tiltSpeed;
    short duration;
    short stage;
} BattleFxMagicDespellModelState;

/* Effect-table record spawned per target by the Dethper camera-facing model tasks. */
typedef struct BattleDespellCameraSprite {
    unsigned char pad_00[0x4];
    BattleFxDethperBonePos pos;
    unsigned char pad_0C[0x4];
    short zDepth;
    short range;
    short scale;
    unsigned char pad_16[0x4];
    short stage;
} BattleDespellCameraSprite;

/* The battle task slot of BattleFx_MagicDespellDrawCameraFacingModelTask. */
typedef struct BattleFxDespellBillboardSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleSpeed;
    short offsetZ;
    short tilt;
    short scale;
    short tiltSpeed;
    short duration;
} BattleFxDespellBillboardSlot;

/* The battle task slot of BattleFx_MagicDespellTimelineTask. */
typedef struct BattleFxDespellTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleSpeed;
    short offsetZ;
    unsigned char pad_12[0x2];
    short scale;
    unsigned char pad_16[0x4];
    short actorIndex;
    short fragmentSlot;
} BattleFxDespellTimelineSlot;

/* The battle task slot of BattleFx_MagicDespellCameraOffsetFadeModelTask. */
typedef struct BattleFxDespellFadeModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short offsetZ;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxDespellFadeModelSlot;

/* PSX textured quad primitive for a Dethper fragment, filled from its quad record. */
typedef struct BattleFxDespellTexturedQuadPrim {
    int colorCmd;
    union {
        int color;
        CVECTOR colorVec;
    };
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    int flags;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxDespellTexturedQuadPrim;

/* PSX textured triangle primitive for a Dethper fragment, filled from its textured triangle record. */
typedef struct BattleFxDespellTexturedTriPrim {
    int colorCmd;
    union {
        int color;
        CVECTOR colorVec;
    };
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    int flags;
    unsigned char pad_18[0x4];
    short uv2;
} BattleFxDespellTexturedTriPrim;

#pragma pack(push, 1)
/* Zero-padded resource filename table referenced by g_BattleMagicDespellResourceDefsHeadPtr / g_BattleMagicDespellResourceDefsDataTail. */
typedef struct {
    char dethpe1TimA[0x10];
    char dethpe1TimB[0x10];
    char dethp1Rsd[0x0c];
    char dethp2Rsd[0x0c];
    char light1Rsd[0x0c];
} BattleFxDespellResourceNames;
#pragma pack(pop)

/* --- battle_fx_magic_esna --- */
/* Particle sprite for the Esna effect: timer, position, velocity and acceleration. */
typedef struct BattleFxEsunaParticleSprite {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velocity;
    unsigned char pad_12[0x2];
    short heightOffset;
    unsigned char pad_16[0x2];
    short velocityX;
    short scale;
    short accel;
} BattleFxEsunaParticleSprite;

#pragma pack(push, 1)
typedef struct BattleMagicEsuna_SpriteDrawReq {
    char colorR; /* +0x0 */
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    unsigned char pad_3[0x5];
    int tableField; /* +0x8 */
} BattleMagicEsuna_SpriteDrawReq;
#pragma pack(pop)

/* --- battle_fx_magic_faiga3_pillars --- */
/* Faiga 3 flash model: phase counter and shrinking billboard size. */
typedef struct BattleFxFire3MultiFlash {
    short ownerSlot;
    short phase;
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x6];
    short size;
    short shrinkStep;
    short startSize;
} BattleFxFire3MultiFlash;

/* Fading model record of the Faiga 3 effect: a step counter, a value and a velocity. */
typedef struct BattleFxFire3MultiFadeSlot {
    short ownerSlot;
    short step;
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x6];
    short value;
    short velocity;
} BattleFxFire3MultiFadeSlot;

/* Setup record for the Faiga3 flash model, with a scale that the spin task shrinks. */
typedef struct BattleFxMagicFire3MultiSetupState {
    short ownerSlot;
    unsigned char pad_02[0x2];
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x4];
    short halfRadius;
    short scale;
    short scaleStep;
    short startScale;
} BattleFxMagicFire3MultiSetupState;

/* Per-slot state of the Faiga3 shrinking spin model: frame counter, scale and scale step. */
typedef struct BattleFxMagicFire3MultiSpinState {
    short ownerSlot;
    short frameIndex;
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x6];
    short scale;
    short scaleStep;
} BattleFxMagicFire3MultiSpinState;

/* GPU textured-triangle primitive header written from a PSX mesh record for Faiga3. */
typedef struct BattleFaigaTexturedPrimitive {
    int tag;
    unsigned char rgb[0x3];
    char command;
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    int packedBits;
    unsigned char pad_18[0x4];
    short uv2;
} BattleFaigaTexturedPrimitive;

/* Task-slot record for a Faiga3 spinning model that shrinks over its lifetime; its position starts as the
 * spawner's and is jittered. */
typedef struct BattleFaigaShrinkingSpinModel {
    unsigned char pad_00[0x4];
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x6];
    short scale;
    short scaleStep;
} BattleFaigaShrinkingSpinModel;

/* Four vertex-pair offsets and quad attributes for the Faiga PSX textured-quad draw. */
typedef struct BattleFxFaigaQuadRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned short vertexOffset3;
    int uv0Clut;
    unsigned short flags;
    short uv2;
    short uv3;
    unsigned short shadeLevel;
} BattleFxFaigaQuadRecord;

/* PSX textured triangle record: three vertex offsets, a texture index and attribute words. */
typedef struct BattleFaigaTriangleRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned short textureIndex;
    int uv0Clut;
    unsigned short uv1;
    short uv2;
} BattleFaigaTriangleRecord;

/* The battle task slot of BattleFx_MagicFire3MultiGrowingFadeModelTask. */
typedef struct BattleFxFire3MultiGrowingFadeModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x8];
    short scale;
    short scaleStep;
} BattleFxFire3MultiGrowingFadeModelSlot;

/* Faiga3 growing-model slot the timeline spawns: its position, then a scale it grows from and a speed. */
typedef struct BattleFxFire3MultiTimelineRecord {
    unsigned char pad_00[0x4];
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x6];
    short scale;
    short speed;
} BattleFxFire3MultiTimelineRecord;

/* --- battle_fx_magic_fira --- */
/* Effect-100 slot driving the Faira cycling model task. */
typedef struct BattleFxFire2CycleSlot {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
} BattleFxFire2CycleSlot;

/* Task-slot record for the Magic Faira cycling model. */
typedef struct BattleFxFire2ModelTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short size;
} BattleFxFire2ModelTask;

#pragma pack(push, 1)
typedef struct BattleMagicFire2_ModelReq {
    int drawHandle; /* +0x0 */
    int cycleIndex; /* +0x4 */
    char rotationMatrix; /* +0x8 */
    unsigned char pad_9[0x11];
    int transX; /* +0x1a: the matrix's translation, unaligned */
    int transY; /* +0x1e */
    int transZ; /* +0x22 */
    unsigned char pad_26[0x2];
} BattleMagicFire2_ModelReq;
#pragma pack(pop)

/* --- battle_fx_magic_firaga_flames --- */
/* Faiga flash sprite record: frame timer and a decaying level that resets to a stored value. */
typedef struct BattleFxFire3FlashSprite {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0xc];
    short billboardDistance;
    short level;
    short levelStep;
    short levelReset;
} BattleFxFire3FlashSprite;

/* Faiga timeline effect slot; spin and scale values are set per variant. */
typedef struct BattleFxFire3Timeline {
    unsigned char pad_00[0xc];
    short spinAngle;
    short riseSpeed;
    short offsetY;
    short spread;
    short step;
} BattleFxFire3Timeline;

/* Faiga flash sprite task record with a 0x5555 start value and derived step. */
typedef struct BattleFxFire3FlashTask {
    short ownerSlot;
    unsigned char pad_02[0x4];
    short posY;
    unsigned char pad_08[0x8];
    short billboardDistance;
    short flashLevel;
    short flashStep;
    short flashLevelReset;
} BattleFxFire3FlashTask;

/* Effect-table record for the Faiga animated billboard sprite task. */
typedef struct BattleFire3AnimatedSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0xe];
    short size;
} BattleFire3AnimatedSprite;

/* The battle task slot of BattleFx_MagicFire3SpinGrowFadeModel2Task. */
typedef struct BattleFxFire3SpinGrowFadeModel2Slot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleOffset;
    short angleStep;
    short scale;
    short scaleStep;
} BattleFxFire3SpinGrowFadeModel2Slot;

/* The battle task slot of BattleFx_MagicFire3SpinGrowFadeModel3EarlyFadeTask. */
typedef struct BattleFxFire3SpinGrowFadeModel3Slot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    short scaleBias;
    short scaleStep;
    short angle;
    short angleStep;
} BattleFxFire3SpinGrowFadeModel3Slot;

/* Faiga animated sprite record; position jitters on each spawn. */
typedef struct BattleFxFire3SpriteRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short size;
} BattleFxFire3SpriteRecord;

#pragma pack(push, 1)
typedef struct BattleMagicFire3SpriteReq {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x8];
    int secondSpriteGroup; /* +0x10 */
    int secondDrawObj; /* +0x14 */
    unsigned char pad_18[0x4];
    short frameIndex; /* +0x1c */
    unsigned char pad_1e[0x2];
} BattleMagicFire3SpriteReq;
#pragma pack(pop)

/* --- battle_fx_magic_fire --- */
/* Fire timeline record: stage counter, plus the battle actor and bone used for each emitted effect. */
typedef struct BattleFxFireTimeline {
    short ownerSlot;
    short stage;
    short actorIndex;
    short boneIndex;
    unsigned char pad_08[0xa];
    short param;
} BattleFxFireTimeline;

/* Main task slot of the animated fire sprite, counting frames up to 0xE. */
typedef struct BattleMagicFireSpriteSlot {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0xe];
    short pushLength;
} BattleMagicFireSpriteSlot;

/* Fire task slot recording the target actor for one per-target flame. */
typedef struct BattleFxFireTargetRecord {
    unsigned char pad_00[0x4];
    short actorId;
    short boneIndex;
    unsigned char pad_08[0xa];
    short negativeRadius;
} BattleFxFireTargetRecord;

/* Task-slot record for the Magic Fire animated sprite. */
typedef struct BattleFxFireAnimatedSprite {
    unsigned char pad_00[0x8];
    short posX;
    unsigned char pad_0A[0x8];
    short param;
} BattleFxFireAnimatedSprite;

#pragma pack(push, 1)
typedef struct BattleMagicFire_SpriteDrawReq {
    int drawReq; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleMagicFire_SpriteDrawReq;
#pragma pack(pop)

/* --- battle_fx_magic_flare_detonation --- */
/* A world position in a Flea task slot: the eight bytes Battle_GetBoneWorldPos fills and the tasks copy together. */
typedef union BattleFxFleaPos {
    Int64Pair pair;
    struct {
        short x;
        short y;
        short z;
    } xyz;
} BattleFxFleaPos;

/* Magic flea timeline record: step, the target position, offset-table index and the target's radius. */
typedef struct BattleFxMagicFlareRecord {
    short ownerSlot;
    short step;
    BattleFxFleaPos pos;
    short offsetIndex;
    unsigned char pad_0E[0x6];
    short radius;
} BattleFxMagicFlareRecord;

/* Flea child task started by the delayed spawn: a position (posY holds the negated offset), animation
 * counters and steps. */
typedef struct BattleFxFlareDelayedSpawn {
    unsigned char pad_00[0x4];
    BattleFxFleaPos pos;
    short stepCount;
    short angle;
    short scale;
    short speed;
    short growScale;
    short rate;
} BattleFxFlareDelayedSpawn;

/* Flea grow and fade sprite task record: position, billboard angle, angle step and animation counters. */
typedef struct BattleFxFlareGrowFadeSprite {
    short ownerSlot;
    short phase;
    BattleFxFleaPos pos;
    short stepCount;
    unsigned char pad_0E[0x4];
    short billboardDistance;
    short angle;
    short angleStep;
} BattleFxFlareGrowFadeSprite;

/* Flea delayed-spawn task slot: step counter, position and a fixed-point ratio. */
typedef struct BattleFxMagicFlareSpawn {
    short ownerSlot;
    short step;
    BattleFxFleaPos pos;
    unsigned char pad_0C[0x8];
    short ratio;
} BattleFxMagicFlareSpawn;

/* Slot of the Flea timeline's moving sprite: position, velocities and billboard push. */
typedef struct BattleFxFlareSlot {
    unsigned char pad_00[0x4];
    BattleFxFleaPos pos;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short pushLength;
    short scale;
    short isLongLived;
} BattleFxFlareSlot;

/* The battle task slot of BattleFx_MagicFlareSpinGrowFadeModel3Task. */
typedef struct BattleFxFlareSpinGrowFadeModel3Slot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short angle;
    short angleOffset;
    short angleStep;
    short scale;
    short scaleStep;
} BattleFxFlareSpinGrowFadeModel3Slot;

/* The battle task slot of BattleFx_MagicFlareSpinGrowFadeModel4Task. */
typedef struct BattleFxFlareSpinGrowFadeModel4Slot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short angle;
    unsigned char pad_10[0x2];
    short angleStep;
    short scale;
    short scaleStep;
} BattleFxFlareSpinGrowFadeModel4Slot;

/* Task slot for the delayed Flea spawn: position, fade level and the ratio step. */
typedef struct BattleMagicFlareSpawnSlot {
    unsigned char pad_00[0x4];
    BattleFxFleaPos pos;
    unsigned char pad_0C[0x8];
    short fadeLevel;
    short ratioStep;
} BattleMagicFlareSpawnSlot;

/* Task slot of the frame-table sprite: frame counter and the jittered position. */
typedef struct BattleMagicFlareSpriteSlot {
    short ownerSlot;
    short frameCounter;
    BattleFxFleaPos pos;
} BattleMagicFlareSpriteSlot;

#pragma pack(push, 1)
typedef struct BattleMagicFlareSpriteColor {
    char colorR; /* +0x0 */
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    unsigned char pad_3[0x1];
    short step; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleMagicFlareSpriteColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicFlareSpriteReq3 {
    unsigned char pad_0[0x4];
    short frameValue; /* +0x4 */
    unsigned char pad_6[0x2];
    struct BattleFxSpriteGroup *spriteGroup; /* +0x8 */
    int drawObj; /* +0xc */
} BattleMagicFlareSpriteReq3;
#pragma pack(pop)

/* --- battle_fx_magic_freeze --- */
/* Effect-100 slot record (0x20 stride) for the freeze converging sprite task. */
typedef struct BattleFxFreezeSpriteSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxFreezeSpriteSlot;

/* Animated sprite particle spawned by the Magic Freeze effect. */
typedef struct BattleFxFreezeAnimatedSprite {
    short slotTag;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxFreezeAnimatedSprite;

/* Effect-slot record of the Freeze damped growth task: frame counter, scale and damped velocity. */
typedef struct BattleFxFreezeDampedGrowthSlot {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
    short growthVelocity;
} BattleFxFreezeDampedGrowthSlot;

/* Freeze growing model task record that drives the freeze model record scale. */
typedef struct BattleFxFreezeGrowTask {
    short ownerSlot;
    short growth;
    short posX;
    short posY;
    short posZ;
} BattleFxFreezeGrowTask;

/* Freeze magic animated sprite record; its frame counter ends at 40. */
typedef struct BattleFxFreezeSpriteRecord {
    short ownerSlot;
    short frame;
    short centerX;
    unsigned char pad_06[0x2];
    short centerZ;
} BattleFxFreezeSpriteRecord;

/* Effect-table state for the Freeze fade model task, writing translation into its matrix. */
typedef struct BattleFreezeFadeState {
    short ownerSlot;
    short frameIndex;
    short transX;
    short transY;
    short transZ;
} BattleFreezeFadeState;

/* The converging snowflakes' sprite draw request: a BattleFxSpriteRequest without its last
 * member (the object is 14 bytes), filled by BattleFx_MagicFreezeConvergingSpriteTask. */
#pragma pack(push, 2)
typedef struct BattleMagicFreezeSpriteDraw {
    struct BattleFxSpriteGroup *spriteGroup;
    int drawObj;
    char colorR;
    char colorG;
    char colorB;
    unsigned char code;
    unsigned short frame;
} BattleMagicFreezeSpriteDraw;
#pragma pack(pop)

/* The world position of the target's bone, as Battle_GetBoneWorldPos writes it. */
typedef struct BattleFxMagicFreezeTargetPos {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_6[0x2];
} BattleFxMagicFreezeTargetPos;

/* Task slot of one converging Freeze snowflake: its start frame, start position and a random burst value. */
typedef struct BattleMagicFreezeBurstSpriteRecord {
    short frameIndex;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short burstRandom;
} BattleMagicFreezeBurstSpriteRecord;

/* Model record spawned by the Freeze damped growth effect, seeded from a source record and random values. */
typedef struct BattleMagicFreezeDampedGrowthRecord {
    unsigned char pad_00[0x4];
    short posX;
    short negativeRandom;
    short posZ;
    unsigned char pad_0A[0xa];
    short growthRandom;
    short growthVelocity;
} BattleMagicFreezeDampedGrowthRecord;

/* Task slot for the Freeze growing model, seeded from the target's bone position. */
typedef struct BattleMagicFreezeGrowingSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
} BattleMagicFreezeGrowingSlot;

/* Three-short burst sprite parameters, randomised per sprite by the freeze spawn task. */
typedef struct BattleFxSpriteBurstParams {
    short startTime;
    short angle;
    short velocity;
} BattleFxSpriteBurstParams;

/* The tail of a Freeze model matrix (a PsxMatrix whose first two diagonal entries are separate
 * globals): the Z scale m[2][2], then the translation t[3]. */
#pragma pack(push, 1)
typedef struct BattleFxFreezeModelRecord {
    short scaleZ;
    int posX;
    int posY;
    int posZ;
} BattleFxFreezeModelRecord;
#pragma pack(pop)

/* --- battle_fx_magic_full_cure --- */
/* One 0x1C-byte sparkle of the Full Cure particle pool. The rising sparkle climbs from pos by
 * velocityY, which it damps each frame; the fading sparkle spins a point orbitRadius from pos by
 * angles and drifts down by fallSpeed. Both draw a billboard of the given scale. */
typedef struct BattleFxMagicFullCureSparkle {
    short state; /* -1 gives the pool slot back */
    short frame;
    SVECTOR pos;
    union {
        SVECTOR angles; /* the fading sparkle's spin, RotMatrix's input */
        struct {
            short unused0C;
            short velocityY; /* the rising sparkle's vertical speed */
        };
    };
    short scale;
    short orbitRadius;
    short fallSpeed;
    unsigned char pad_1A[0x2];
} BattleFxMagicFullCureSparkle;

/* A Full Cure task slot: the owner (the target actor, then BATTLE_TASK_FREE), a frame counter and
 * the target bone position each spawned child copies. */
typedef struct BattleFxFullCureTimelineSlot {
    short ownerSlot;
    short frame;
    SVECTOR targetPos;
} BattleFxFullCureTimelineSlot;

/* --- battle_fx_magic_grav_five --- */
/* Grav5 ring burst record: step counter, per-axis velocities and accumulating rates. */
typedef struct BattleFxDemi3MultiRingBurst {
    short ownerSlot;
    short step;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short angleZ;
} BattleFxDemi3MultiRingBurst;

/* PSX mesh primitive record for the Grav 5 magic, with vertex copies and a colour word. */
typedef struct BattleFxDemi3MultiPsxMesh {
    int tag;
    int primColor;
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    short uv1;
    short flags;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short ptAOffset;
} BattleFxDemi3MultiPsxMesh;

/* Per-frame translation increments added to the Grav5 stretched model's matrix. */
typedef struct BattleDemi3MultiStretchStep {
    int stepX;
    int stepY;
    int stepZ;
} BattleDemi3MultiStretchStep;

/* Effect-table ring burst particle with a position and an offset scaled by a factor. */
typedef struct BattleDemi3MultiRingBurstParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scaledOffsetX;
    short scaledOffsetY;
    short scaledOffsetZ;
} BattleDemi3MultiRingBurstParticle;

/* Render-state block that the Grav5 mesh draw fills before submitting the mesh. */
typedef struct BattleFxDemi3MultiRenderState {
    int tag;
    int primCommand;
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    short uv1;
    short flags;
    unsigned char pad_18[0x4];
    short uv2;
} BattleFxDemi3MultiRenderState;

/* The three scale values of the Grav 5 black mesh, all set from the frame counter. */
typedef struct BattleDemi3MultiBlackMeshRecord {
    int oldest;
    int previous;
    int current;
} BattleDemi3MultiBlackMeshRecord;

/* Grav5 black mesh spawn slot; counter advances every fourth step up to 16. */
typedef struct BattleFxDemi3MultiSpawnSlot {
    short ownerSlot;
    short spawnCounter;
} BattleFxDemi3MultiSpawnSlot;

/* Sprite draw request of the Grav5 magic effect. */
typedef struct {
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_03[0x5];
    int quadState;
} BattleFxDemi3MultiSpriteDrawRequest;

/* Grav5 PSX mesh packet: tag word and words copied from the mesh data. */
typedef struct BattleFxDemi3MultiPsxPacket {
    int tag;
    union {
        int primCommand;
        CVECTOR color0;
    };
    unsigned char pad_08[0x4];
    union {
        int primColor;
        CVECTOR color1;
    };
    unsigned char pad_10[0x4];
    union {
        int shadeColor;
        CVECTOR color2;
    };
    unsigned char pad_18[0x4];
    union {
        int color3;
        CVECTOR color3Vec;
    };
} BattleFxDemi3MultiPsxPacket;

/* Grav5 PSX mesh packet: tag word and words copied from the data pointer. */
typedef struct BattleFxDemi3MultiPacket {
    int tag;
    union {
        int primCommand;
        CVECTOR color0;
    };
    unsigned char pad_08[0x4];
    union {
        int primColor;
        CVECTOR color1;
    };
    unsigned char pad_10[0x4];
    union {
        int shadeColor;
        CVECTOR color2;
    };
} BattleFxDemi3MultiPacket;

/* PSX mesh draw data in battle effects: primitive count, vertex offsets and Gouraud colors. */
typedef struct BattleFxPsxMeshData {
    short primCount;
    short corner1Offset;
    short vertCOffset;
    short vertexSlotOffset;
    union {
        int primWord;
        CVECTOR color0;
    };
    union {
        struct {
            short uv1;
            short uv2;
        };
        int primColor;
        CVECTOR color1;
    };
    union {
        struct {
            short ptAOffset;
            short pointOffsetB;
        };
        int shadeColor;
        CVECTOR color2;
    };
    union {
        struct {
            short vertexOffsetA;
            short cornerOffset;
        };
        int color3;
        CVECTOR color3Vec;
    };
    short vertexOffsetB;
    short nextVertexOffsetB;
    short nextVertexOffsetC;
} BattleFxPsxMeshData;

/* Target centroid copied into Grav5 child task slots. */
typedef struct {
    short posX;
    short posY;
    short posZ;
    short pad;
} BattleFxDemi3MultiCoords;

/* Task slot carrying a copied target centroid at +0x04. */
typedef struct BattleFxDemi3MultiCoordTaskSlot {
    short ownerSlot;
    short frameCounter;
    BattleFxDemi3MultiCoords coords;
} BattleFxDemi3MultiCoordTaskSlot;

/* Converging particle / ring-burst child task slot. */
typedef struct BattleFxDemi3MultiEmitterSlot {
    short ownerSlot;
    short frameCounter;
    BattleFxDemi3MultiCoords coords;
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxDemi3MultiEmitterSlot;

/* Black background mesh task slot with copied centroid and Euler angles. */
typedef struct BattleFxDemi3MultiBlackMeshSlot {
    short ownerSlot;
    short frameCounter;
    BattleFxDemi3MultiCoords coords;
    short rotX;
    short rotY;
    short rotZ;
} BattleFxDemi3MultiBlackMeshSlot;

/* First 16 bytes of a BattleFxModelDraw filled by the stretched model task. */
typedef struct {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} BattleFxDemi3MultiModelReq;

/* 16-byte PSX mesh draw request passed to BattleFx_MagicDemi3MultiDrawPsxMesh. */
typedef struct {
    unsigned int meshPayload;
    unsigned int flags;
    short pad_08;
    short shade;
    short flagBits;
    short pad_0E;
} BattleFxDemi3MultiMeshRequest;

#pragma pack(push, 1)
/* Zero-padded resource filename table referenced by g_BattleMagicDemi3MultiResourceRequests. */
typedef struct {
    char gravig00Tim[0x10];
    char elec4Spt[0x0c];
    char gravig01Tim[0x10];
    char gravig02Tim[0x10];
    char grav2Rsd[0x0c];
    char tama1Tim[0x0b];
} BattleFxDemi3MultiResourcePaths;

typedef struct {
    BattleFxMagicDemi3MultiDrawReq *drawReq;
    int spriteObject;
    unsigned char pad_8[0x4];
    short frameSelector;
    unsigned char pad_e[0x2];
} BattleMagicDemi3Multi_SpriteDrawReq2;
#pragma pack(pop)

/* Random start angle and offset of a converging magic particle. */
typedef struct BattleFxConvergeParticleInit {
    short angleX;
    short startAngle;
    short startOffset;
} BattleFxConvergeParticleInit;

/* --- battle_fx_magic_grav_three --- */
/* A world position in the task slots of the Grav3 effect: the shorts Battle_GetBoneWorldPos writes. */
typedef struct BattleFxDemi3Position {
    short x;
    short y;
    short z;
    short pad_06;
} BattleFxDemi3Position;

/* Grav3 timeline task slot: one per target, ticking the time that spawns the other tasks. */
typedef struct BattleFxGrav3TimelineSlot {
    short ownerSlot;
    short time;
    short targetIndex;
    short param;
    BattleFxDemi3Position position;
} BattleFxGrav3TimelineSlot;

/* Grav3 trail emitter slot: moves its position by the velocity and drops a trail sprite at it. */
typedef struct BattleFxDemi3TrailEmitter {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x4];
    BattleFxDemi3Position position;
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxDemi3TrailEmitter;

/* Grav3 trail sprite slot: a position and the frame it draws. */
typedef struct BattleFxDemi3TrailSprite {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x4];
    BattleFxDemi3Position position;
} BattleFxDemi3TrailSprite;

/* Rotating sprite curve record for the Grav 3 magic effect. The timeline task starts two of them,
 * writing 0 and 15 to ownerSlot, which the pulse reads as a phase offset. */
typedef struct BattleFxDemi3CurveSprite {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x4];
    BattleFxDemi3Position center;
    SVECTOR rotation;
    short baseRadius;
} BattleFxDemi3CurveSprite;

/* Grav3 animated sprite record: a frame offset and index, and per-frame translation deltas. */
typedef struct BattleFxMagicDemi3AnimatedSprite {
    short frameOffset;
    short frameIndex;
    unsigned char pad_04[0x4];
    BattleFxDemi3Position position;
    short deltaX;
    short deltaY;
    unsigned char pad_14[0x4];
    short pushLength;
    short scale;
} BattleFxMagicDemi3AnimatedSprite;

/* Grav3 growing quad sprite slot; frame index advances each update. */
typedef struct BattleFxDemi3GrowingQuadSlot {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x4];
    BattleFxDemi3Position position;
    unsigned char pad_10[0x8];
    short pushLength;
} BattleFxDemi3GrowingQuadSlot;

/* The two shorts VectorNormalToSVector writes: the unit direction's components. */
typedef struct BattleFxDemi3SegmentEmitter {
    short componentA;
    short componentB;
} BattleFxDemi3SegmentEmitter;

/* Data-block scratch of the segment emitter: the direction VectorNormalToSVector reads, then its output. */
typedef struct BattleFxDemi3SegmentScratch {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    BattleFxDemi3SegmentEmitter basis;
} BattleFxDemi3SegmentScratch;

/* Segment emitter slot whose counter stops at 40 emitted segments. */
typedef struct BattleFxDemi3EmitterSlot {
    short ownerSlot;
    short emitCount;
    unsigned char pad_04[0x4];
    BattleFxDemi3Position position;
} BattleFxDemi3EmitterSlot;

/* The slot a new task of the Grav3 effect gets, as the timeline task fills it. */
typedef union BattleFxGrav3Slot {
    BattleFxGrav3TimelineSlot timeline;
    BattleFxDemi3TrailEmitter trailEmitter;
    BattleFxDemi3CurveSprite curve;
    BattleFxMagicDemi3AnimatedSprite animated;
    BattleFxDemi3GrowingQuadSlot quad;
    BattleFxDemi3EmitterSlot segmentEmitter;
} BattleFxGrav3Slot;

/* The three vectors of a segment beam: its start and end points, and the span between them. */
typedef struct BattleFxDemi3SegmentSlot {
    SVECTOR start;
    SVECTOR end;
    SVECTOR delta;
} BattleFxDemi3SegmentSlot;

/* The PSX gouraud quad (POLY_G4) a segment beam is drawn with; the projection fills in x1/y1 and x2/y2. */
#pragma pack(push, 1)
typedef struct BattleFxDemi3SegmentPrim {
    int tag;
    int rgbCode0;
    short x0;
    short y0;
    int rgb1;
    short x1;
    short y1;
    int rgb2;
    short x2;
    short y2;
    int rgb3;
    short x3;
    short y3;
} BattleFxDemi3SegmentPrim;
#pragma pack(pop)

/* --- battle_fx_magic_grav_two --- */
/* A Grav2 effect task slot: frame counter, a position (copied as eight bytes) and the per-frame
 * step added to it, and the billboard push length. */
typedef struct BattleFxDemi2Slot {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x4];
    union {
        Int64Pair pos;
        struct {
            short posX;
            short posY;
            short posZ;
        };
    };
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
    short pushLength;
} BattleFxDemi2Slot;

/* Ring sprite record of the Grav 2 magic effect: a step counter, per-frame spin steps and the ring's rotation angles. */
typedef struct BattleFxDemi2RingSlot {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x4];
    short spinX;
    short spinY;
    short spinZ;
    unsigned char pad_0E[0x2];
    SVECTOR rot;
    short pushLength;
} BattleFxDemi2RingSlot;

/* --- battle_fx_magic_heist --- */
/* Orbiting model pair timeline: per-frame velocities, spin angle, frame counter and state. */
typedef struct BattleFxOrbitingPairRecord {
    short ownerSlot;
    short frame;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0x2];
    short spinY;
    short orbitSpin;
    short orbitAngle;
    short spinZ;
    short spinX;
    short orbitRadius;
    short angle;
    short angleStep;
    short state;
} BattleFxOrbitingPairRecord;

/* Orbiting model pair task record: orbit angle seeded from a fixed offset, other fields zeroed. */
typedef struct BattleFxOrbitingPair {
    unsigned char pad_00[0xc];
    short spinY;
    short orbitSpin;
    short orbitAngle;
    short spinZ;
    short spinX;
    short orbitRadius;
    short angle;
    short tilt;
    short state;
} BattleFxOrbitingPair;

/* Heist orbiting-pair slot: spawn values, with a partner record that copies two fields. */
typedef struct BattleFxMagicHasteOrbitingPair {
    short ownerSlot;
    unsigned char pad_02[0x4];
    short posY;
    unsigned char pad_08[0x4];
    short rotYOffset;
    short tiltYOffset;
    short orbitAngle;
    short rotZ;
    short rotX;
    short orbitRadius;
    short angle;
    short tilt;
    short state;
} BattleFxMagicHasteOrbitingPair;

/* Heist morph task state: step counter and fixed-point morph weight with its per-frame increment. */
typedef struct BattleFxHasteMorphState {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x14];
    short morphWeight;
    short weightStep;
} BattleFxHasteMorphState;

/* --- battle_fx_magic_hitmark --- */
/* Hitmark impact sprite task record with a frame counter and billboard size. */
typedef struct BattleFxHitmarkImpactSpriteTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    unsigned char pad_0C[0x8];
    short scale;
} BattleFxHitmarkImpactSpriteTask;

/* GPU primitive words written into the Hitmark primitive buffer. */
typedef struct BattleHitmarkPrimitive {
    int tag;
    int color;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
} BattleHitmarkPrimitive;

/* The rotation angles of an impact particle's launch direction. RotMatrix reads them as a SVECTOR,
 * so its z angle and pad are the zero word after the object. */
#pragma pack(push, 1)
typedef struct BattleMagicHitmark_BurstAngle {
    short angleX; /* +0x0 */
    short angleY; /* +0x2 */
} BattleMagicHitmark_BurstAngle;
#pragma pack(pop)

/* The third corner of the particle triangle, then the impact sprite's draw request. */
typedef struct {
    SVECTOR particleVertex2;
    BattleFxSpriteRequest sprite;
} BattleMagicHitmarkSpriteDraw;

/* The sprite file of the effect, then the X of the centre of the circle the rays hit (its Z is
 * g_BattleMagicHitmark_ImpactCenterZ). */
typedef struct {
    unsigned char *spriteFile;
    int unused04[3];
    short impactCenterX;
    short pad12;
} BattleMagicHitmarkSpriteFileDef;

/* --- battle_fx_magic_limit_flash --- */
/* Limit Flash model record that grows over its frame counter and copies two words to globals. */
typedef struct BattleFxLimitFlashModel0Grow {
    short ownerSlot;
    short frameCount;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
} BattleFxLimitFlashModel0Grow;

/* Radial Limit Flash sprite record: a sprite dimension index, a frame counter and transform terms. */
typedef struct BattleFxLimitFlashRadialSprite {
    short dimensionIndex;
    short frameCount;
    short bonePos; /* the emitter's position, as three shorts */
    unsigned char pad_06[0x6];
    short velocityX;
    short velocityY;
    unsigned char pad_10[0x4];
    short accelX;
    short accelY;
} BattleFxLimitFlashRadialSprite;

/* Limit flash sine-scale task record that drives sine-based state globals. */
typedef struct BattleFxLimitFlashSineTask {
    short ownerSlot;
    short step;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
} BattleFxLimitFlashSineTask;

/* Radial flash sprite record; its offsets are the sine and cosine of the emitter angle. */
typedef struct BattleFxRadialFlashSprite {
    short spriteIndex; /* +0x0 */
    unsigned char pad_2[0x2];
    short bonePos; /* the emitter's position, as three shorts */
    unsigned char pad_6[0x6];
    short offsetX;
    short offsetY;
    unsigned char pad_10[0x4];
    short rotatedX;
    short rotatedY;
} BattleFxRadialFlashSprite;

/* Secondary task record that drives the Limit flash shrink effect each frame. */
typedef struct BattleFxLimitFlashShrink {
    short ownerSlot; /* +0x0 */
    short shrinkStep;
    short shrinkState;
    unsigned char pad_06[0x2];
    short shrinkValue;
} BattleFxLimitFlashShrink;

/* Secondary slot record of a Limit flash, placed at the actor's bone and running its model task. */
typedef struct BattleLimitFlashEmitter {
    unsigned char pad_00[0x4];
    short bonePos;
    unsigned char pad_06[0x16];
    void (__cdecl *updateTask)(void);
} BattleLimitFlashEmitter;

/* Slot of the task that spawns the radial flare sprites: a counter of the sprites made and the bone position they start at. */
typedef struct BattleFxLimitFlashRadialEmitter {
    short ownerSlot;
    short spriteCounter;
    short bonePos; /* the actor bone's position, as three shorts */
    unsigned char pad_06[0x1a];
} BattleFxLimitFlashRadialEmitter;

/* Limit flash pulse emitter slot; pulse counter spawns every fourth step up to 12. */
typedef struct BattleFxLimitFlashPulseSlot {
    short ownerSlot;
    short pulseCounter;
    short bonePos; /* the actor bone's position, as three shorts */
    unsigned char pad_06[0x16];
    int allocTask;
} BattleFxLimitFlashPulseSlot;

#pragma pack(push, 1)
/* The last element and the translation of the radial flare's rotation matrix (g_BattleLimitFlash_ModelScales1,
 * g_BattleLimitFlash_ModelScale0 and this record form one PsxMatrix), then the flare's sprite draw record. */
typedef struct BattleLimitFlash_SpriteData {
    short matrixM22;
    int transX;
    int transY;
    int transZ;
    unsigned char pad_e[0x2];
    int spriteRecord; /* Gfx_ModelTableGetLoadedRecord(3, g_pBattleLimitFlashTable) */
    int sceneField; /* Battle3d_GetSceneRenderObject(1, g_pBattle3dScene) */
    unsigned char pad_18[0x6];
    short dimension; /* g_BattleLimitFlash_SpriteDimensions[dimensionIndex] */
} BattleLimitFlash_SpriteData;
#pragma pack(pop)

/* --- battle_fx_magic_mbarrier --- */
/* Task slot of the Mbaria effect (spawn, timeline and scaled-model tasks): placed at a target's
 * bone position with a radius offset, with the target's rotation and a frame counter. */
typedef struct BattleFxMBarrierSlot {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    SVECTOR rot;
    short actorIndex;
} BattleFxMBarrierSlot;

/* --- battle_fx_magic_minimam_particles --- */
/* Magic Minimam record: base position, jitter ranges and scale for particle spawns. */
typedef struct BattleFxMiniRecord {
    short ownerSlot;
    short frame;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x2];
    short jitterOffset;
    short jitterRange;
    short jitterOffsetY;
    short jitterRangeY;
    short scale;
    unsigned char pad_16[0x2];
    short recordType;
} BattleFxMiniRecord;

/* Per-target timeline record for the Minimam spell effect. */
typedef struct BattleFxMiniTarget {
    unsigned char pad_00[0x6];
    short extent;
    unsigned char pad_08[0x4];
    short depth;
    short depthX2;
    short offset;
    short offsetX4;
    short scale;
    unsigned char pad_16[0x2];
    short targetIndex;
} BattleFxMiniTarget;

/* One billboard particle of the Minimam pool: position and scale. */
typedef struct BattleMagicMinimamParticle {
    short posX;
    short posY;
    short posZ;
    short scale;
} BattleMagicMinimamParticle;

/* The spawn cursor and the ring of 444 particles; this view starts at g_BattleMagicMiniParticleIndex and runs on over the pads behind it. */
typedef struct BattleMagicMiniParticlePool {
    int cursor;
    unsigned char pad_04[0x4];
    BattleMagicMinimamParticle particles[0x1BC];
} BattleMagicMiniParticlePool;

/* --- battle_fx_magic_queig --- */
/* Queig pulsing model record: frame timer, angle with offset, level ramp and model index. */
typedef struct BattleFxQuake3PulseRecord {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short angle;
    short angleOffset;
    short baseLevel;
    short modelIndex;
    unsigned char pad_16[0x2];
    short levelStep;
} BattleFxQuake3PulseRecord;

/* Queig multi-target burst timeline slot: frame timer, origin, burst indices and size. */
typedef struct BattleFxQuake3BurstTimeline {
    short ownerSlot;
    short timer;
    short originX;
    short originY;
    short originZ;
    unsigned char pad_0A[0x2];
    short burstIndexA;
    unsigned char pad_0E[0x4];
    short size;
    unsigned char pad_14[0x8];
    short burstIndexB;
} BattleFxQuake3BurstTimeline;

/* Magic Queig multi-target burst slot: burst position offsets and scale from the source record. */
typedef struct BattleFxQuake3Burst {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short clearedWord;
    short angle;
    short scale;
    short radius;
    short baseRadius;
    short morphWeight;
    short growStep;
} BattleFxQuake3Burst;

/* Draw context of one Queig shard mesh: the block Gfx_GetPsxScratchpadPtr(0) returns for
 * BattleFx_MagicQuake3DrawShardMesh and the triangle and quad drawers. frontFacing and orderingDepth
 * are stored by the GTE helpers; the points are the shard's moving corners (triangles use three). */
#pragma pack(push, 1)
typedef struct BattleFxQuake3Shard {
    int meshBase;
    int orderingTable;
    int drawMode;
    int primitive;
    int flags;
    int frontFacing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int recordIndex;
    int fadeStart;
    PsxMatrix cameraMatrix;
    BattleRotationMatrix rotation;
    int posX;
    int posY;
    int posZ;
    unsigned char pad_68[0x1e];
    BattleFxFixedScaleVec scale;
    unsigned char pad_92[0x14];
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_AC[0x2];
    short point0X;
    short point0Y;
    short point0Z;
    unsigned char pad_B4[0x2];
    short point1X;
    short point1Y;
    short point1Z;
    unsigned char pad_BC[0x2];
    short point2X;
    short point2Y;
    short point2Z;
    unsigned char pad_C4[0x2];
    short point3X;
    short point3Y;
    short point3Z;
    unsigned char pad_CC[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxQuake3Shard;
#pragma pack(pop)
typedef char CheckBattleFxQueigShard[(sizeof(BattleFxQuake3Shard) == 0xd4) ? 1 : -1];

/* Child slot of the Queig single-target effect: burst scale and burst size. */
typedef struct BattleFxQuake3BurstSprite {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short clearedWord;
    short angle;
    short scale;
    short burstSize;
    short burstIndex;
    short morphWeight;
    short growStep;
} BattleFxQuake3BurstSprite;

/* Ring model record of the Queig flattening effect: a step counter, a value and a velocity. */
typedef struct BattleFxQuake3RingSlot {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short radius;
    short growth;
} BattleFxQuake3RingSlot;

/* Queig ground quad task record; posX to posZ are read out to draw the quad. */
typedef struct BattleFxQuake3GroundQuadRecord {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
} BattleFxQuake3GroundQuadRecord;

/* Mesh draw request of BattleFx_MagicQuake3DrawPsxMesh, the block the burst tasks fill from
 * Gfx_GetPsxScratchpadPtr(0): the mesh blob, draw flags, vertex shade and attribute bits. */
typedef struct BattleFxQuake3MeshDraw {
    int meshData;
    int flags;
    short unused08;
    short vertexShade;
    short attrBits;
    short unused0E;
} BattleFxQuake3MeshDraw;

/* GPU textured primitive packet written by BattleFx_MagicQuake3DrawPsxMesh. */
typedef struct BattleFxQuake3Prim {
    int tag;
    union {
        int colorCmd;
        struct {
            unsigned char rgb[3];
            char gpuCode;
        };
    };
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    short uvWord0;
    short blendMode;
    unsigned char pad_18[0x4];
    short uvWord1;
    unsigned char pad_1E[0x6];
    short vertex0Offset;
} BattleFxQuake3Prim;

/* Queig PSX mesh packet: tag word and words copied from the mesh entry. */
typedef struct BattleFxQuake3PsxPacket {
    int tag;
    union {
        int colorCmd;
        struct {
            unsigned char rgb[3];
            char gpuCode;
        };
    };
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    int texWord;
    unsigned char pad_18[0x4];
    int texWord2;
} BattleFxQuake3PsxPacket;

/* Queig mesh record, one view per draw pass: vertex slot offsets, color bytes and primitive words.
 * The textured passes read the words at +0xC, +0x10 and +0x14 instead of the shorts. */
typedef struct BattleMagicQuake3MeshRecord {
    short numberEntries;
    short vertex1Offset;
    short vertex2Offset;
    short colorA;
    int primWord;
    union {
        struct {
            short colorC;
            short colorD;
            short vertex0Offset;
            short colorB;
            short vertexDeltaA;
            short vertexDeltaB;
            short vertexDeltaC;
            short nextVertex1Offset;
            short nextVertex2Offset;
        };
        struct {
            int texturedPrimWord;
            int texWord;
            int texWord2;
        };
    };
} BattleMagicQuake3MeshRecord;

/* The Queig mesh blob (0xd88 bytes): the shard records start recordsOffset bytes after the header.
 * vertices is the pose the effect morphs in place, seedVertices the pose it is reset from. */
#pragma pack(push, 1)
typedef struct BattleMagicQuake3MeshSource {
    int recordsOffset;
    SVECTOR vertices[97];
    unsigned char records[0x774];
    SVECTOR seedVertices[97];
} BattleMagicQuake3MeshSource;
#pragma pack(pop)
typedef char CheckBattleMagicQueigMeshSource[(sizeof(BattleMagicQuake3MeshSource) == 0xd88) ? 1 : -1];

/* --- battle_fx_magic_queir --- */
/* Timeline record for the Queir multi-target effect; its speed is copied from the parent record. */
typedef struct BattleFxQuake2TimelineRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short fadeStart;
    short spinAngleY;
    short baseAngleY;
    short size;
    short sizeStep;
    short fadeLevel;
    short fadeStep;
    short isActive;
} BattleFxQuake2TimelineRecord;

/* Queir multi-target effect slot (0x20 stride): timeline counter, phase and speed. */
typedef struct BattleFxQuake2TimelineTask {
    short ownerSlot;
    short frameCount;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x2];
    short phase;
    unsigned char pad_0E[0x4];
    short size;
} BattleFxQuake2TimelineTask;

/* Task slot record (0x20 stride) for the Queir fading model task. */
typedef struct BattleFxQuake2FadeSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short fadeStart;
    short spinAngleY;
    short baseAngleY;
    short modelSize;
    short altModelEnabled;
    short fadeLevel;
    short fadeStep;
} BattleFxQuake2FadeSlot;

/* Magic Queir timeline record in the battle task slot buffer, set from a source record. */
typedef struct BattleFxMagicQuake2Timeline {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short frameCount;
    short spinAngle;
    short scale;
    short progress;
    short stepSize;
    short fadeLevel;
    short shade;
    short clearedWord;
} BattleFxMagicQuake2Timeline;

/* Grow and shrink model record for the Queir effect: phase, scale and scale step. */
typedef struct BattleFxQuake2GrowShrinkModel {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short scale;
    short scaleStep;
} BattleFxQuake2GrowShrinkModel;

/* Queir single-target timeline record in the battle task slot buffer: stage and phase counters. */
typedef struct BattleFxQuake2SingleTarget {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short phase;
    unsigned char pad_0E[0x4];
    short progress;
} BattleFxQuake2SingleTarget;

/* The draw context of one Queir shard mesh, the block Gfx_GetPsxScratchpadPtr(0) returns to
   BattleFx_MagicQuake2ShardBurstTask: the mesh, ordering table and primitive cursor, the camera matrix
   and the shard rotation it is multiplied into, the burst centre in view space (offset), and the
   shard's moving corners (triangles use three of them; the quads a fourth) around its centre (base). */
#pragma pack(push, 1)
typedef struct BattleFxQuake2Shard {
    int meshBase;
    int orderingTable;
    int drawMode;
    int primitive;
    int flags;
    int frontFacing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int recordIndex;
    PsxMatrix cameraMatrix;
    BattleRotationMatrix rotation;
    int posX;
    int posY;
    int posZ;
    unsigned char pad_64[0x10];
    int offsetX;
    int offsetY;
    int offsetZ;
    unsigned char pad_80[0x4];
    SVECTOR burstPos;
    short point0X;
    short point0Y;
    short point0Z;
    unsigned char pad_92[0x2];
    short point1X;
    short point1Y;
    short point1Z;
    unsigned char pad_9A[0x2];
    short point2X;
    short point2Y;
    short point2Z;
    unsigned char pad_A2[0x2];
    short point3X;
    short point3Y;
    short point3Z;
    unsigned char pad_AA[0x2];
    short baseX;
    short baseY;
    short baseZ;
} BattleFxQuake2Shard;
#pragma pack(pop)

/* The Queir shard burst task slot: its frame counter, the burst centre and the burst size with its step. */
typedef struct BattleFxQuake2BurstTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short size;
    short sizeStep;
} BattleFxQuake2BurstTask;

/* Queir select timeline task record set up in the battle task buffer. */
typedef struct BattleFxQuake2SelectTask {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short originY;
    short posZ;
    unsigned char pad_0A[0x2];
    short phase;
    unsigned char pad_0E[0x4];
    short size;
} BattleFxQuake2SelectTask;

/* Model config for the Queir fading quad particle task. */
typedef struct BattleMagicQuake2QuadConfig {
    unsigned char pad_00[0x8];
    int resourceEntry;
    unsigned char pad_0C[0x4];
    short heightOffset;
    unsigned char pad_12[0x6];
    short heightOffsetB;
} BattleMagicQuake2QuadConfig;

/* --- battle_fx_magic_reflect --- */
/* Effect-slot record of the Rifrek scaled model task: frame counter, scale and scale step. */
typedef struct BattleFxReflectScaledSlot {
    short slotTag;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    SVECTOR rotation;
    short scale;
    short scaleStep;
} BattleFxReflectScaledSlot;

/* Rifrek timeline record: a step counter and a record type passed to the allocator. */
typedef struct BattleFxReflectTimelineRecord {
    short ownerSlot;
    short step;
    Int64Pair position;
    Int64Pair rotation;
    unsigned char pad_14[0x4];
    short recordType;
} BattleFxReflectTimelineRecord;

/* Timeline record for the Rifrek magic effect, allocated from the effect slot array. */
typedef struct BattleFxReflectTimeline {
    unsigned char pad_00[0x4];
    Int64Pair position;
    Int64Pair rotation;
    short scale;
    short distance;
} BattleFxReflectTimeline;

/* Per-target Rifrek effect slot placed at a bone position with a radius offset. */
typedef struct BattleFxReflectSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x2];
    Int64Pair rotation;
    unsigned char pad_14[0x4];
    short actorIndex;
} BattleFxReflectSpawnSlot;

/* --- battle_fx_magic_rezist --- */
/* Fading sprite particle for the Rezist effect: timer, position, velocity and acceleration. */
typedef struct BattleFxResistFadingSprite {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short rotX;
    unsigned char pad_12[0x2];
    short rotZ;
    unsigned char pad_16[0x2];
    short radialSpeed;
    short scale;
    short spinStep;
} BattleFxResistFadingSprite;

/* --- battle_fx_magic_seald --- */
/* Task-slot record of the Seald magic timeline; a step counter selects each phase. */
typedef struct BattleFxShieldTaskSlot {
    short ownerSlot;
    short step;
    Int64Pair pos;
    unsigned char pad_0C[0xc];
    short modelScale;
    short tumbleScale;
} BattleFxShieldTaskSlot;

/* Setup record for the Seald timeline models with randomized per-particle values. */
typedef struct BattleFxMagicShieldParticleState {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short angleX;
    short angleStepX;
    short angleY;
    short angleStepY;
    short angleZ;
    short angleStepZ;
    short scale;
} BattleFxMagicShieldParticleState;

/* Textured triangle record for the Seald draw: vertex offsets, a word and flags. */
typedef struct BattleFxShieldTexturedTriRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned char pad_06[0x2];
    int color0;
    unsigned short flags;
    short color1;
} BattleFxShieldTexturedTriRecord;

/* The battle task slot of BattleFx_MagicShieldTumblingModelTask. */
typedef struct BattleFxShieldTumblingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short positionA;
    short velocityA;
    short positionB;
    short velocityB;
    short positionC;
    short velocityC;
    short scale;
} BattleFxShieldTumblingModelSlot;

/* Task slot set up for the Seald magic timeline. */
typedef struct BattleMagicShieldTaskSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short pos[3]; /* world position of the target's bone (Battle_GetBoneWorldPos) */
    unsigned char pad_0A[0xe];
    short scaledModelScale;
    short tumblingModelScale;
} BattleMagicShieldTaskSlot;

/* Per-face motion record of the Seald effect: velocity and per-frame acceleration. */
typedef struct MagicShieldFaceMotion {
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_06[0x2];
    short accelX;
    short accelY;
    short accelZ;
    unsigned char pad_0E[0xa];
} MagicShieldFaceMotion;

/* PSX textured triangle primitive for a Seald effect, filled from its textured record. */
typedef struct BattleFxShieldTexturedTriPrim {
    int tag;
    int color;
    unsigned char pad_08[0x4];
    int color0;
    unsigned char pad_10[0x4];
    int flags;
    unsigned char pad_18[0x4];
    short color1;
} BattleFxShieldTexturedTriPrim;

/* Three vertex byte offsets for a Seald face's neighbor, vertex and previous-vertex entries. */
typedef struct BattleFxShieldVertexOffsets {
    unsigned short neighborOffset;
    unsigned short vertexOffset;
    unsigned short prevVertexOffset;
    unsigned char pad_06[0xa]; /* to 0x10 bytes, the step of every pointer through it */
} BattleFxShieldVertexOffsets;

/* --- battle_fx_magic_slipl --- */
/* Magic Slipl spin, grow and fade model record: phase, fade level and angle. */
typedef struct BattleFxSleepelSpinRecord {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinAngleY;
    short orbitAngleY;
    short orbitRadius;
    short orbitRadiusStep;
    short riseSpeed;
    short scale;
    short scaleStep;
    short actorSlot;
} BattleFxSleepelSpinRecord;

/* Spin and grow model spawned per target by the Slipl effect, keyed by the target slot. */
typedef struct BattleFxSleepelSpinModel {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    unsigned char pad_08[0x4];
    short spinAngle;
    short orbitAngle;
    short diameter;
    short steps;
    short riseSpeed;
    short scale;
    short scaleStep;
    short actorSlot;
} BattleFxSleepelSpinModel;

/* Quad particle record for the Slipl effect: timer and spawn jitter range and offset. */
typedef struct BattleFxSleepelQuadParticle {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short jitterOffset;
    short jitterRange;
    short depth;
} BattleFxSleepelQuadParticle;

/* Spawn record for the Slipl quad particles, written per target. */
typedef struct BattleFxSleepelSpawnRecord {
    unsigned char pad_00[0x10];
    short radius;
    short diameter;
    short sizeTotal;
} BattleFxSleepelSpawnRecord;

/* Effect-table quad particle spawned by the Sliple particle task from an emitter record. */
typedef struct BattleSleepelQuadParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short scaleX;
    short depth;
} BattleSleepelQuadParticle;

/* --- battle_fx_magic_slow --- */
/* One entry of the Slow vertex tables: a target position with a pad short, 8 bytes apart. */
typedef struct BattleFxSlowTableVertex {
    short vx;
    short vy;
    short vz;
    short pad;
} BattleFxSlowTableVertex;

#pragma pack(push, 1)
typedef struct BattleMagicSlow_Centroid {
    short targetX; /* +0x0 */
    short y2; /* +0x2 */
} BattleMagicSlow_Centroid;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicSlow_ModelReq1 {
    int modelHandle; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleMagicSlow_ModelReq1;
#pragma pack(pop)

/* --- battle_fx_magic_stop_clock --- */
/* Effect-100 array entry (0x20 bytes) for the Magic Stop model tasks. */
typedef struct BattleFxMagicStopEntry {
    unsigned char pad_00[0x4];
    Int64Pair worldPos; /* the target's bone world position (three shorts), copied from the spawn record */
    short scale;
    short yaw;
    short position;
    short offset;
    short extentTarget;
    short extentStep;
    short extentSaved;
    short extent;
} BattleFxMagicStopEntry;

/* Stop-spell spawn record: base coordinate and target index, set when the per-target effect is allocated. */
typedef struct BattleFxStopSpawnRecord {
    unsigned char pad_00[0x4];
    Int64Pair worldPos; /* the target's bone world position (three shorts) */
    short angle;
    short yaw;
    short base;
    unsigned char pad_12[0x2];
    short ramp;
    short currentExtent;
    unsigned char pad_18[0x2];
    short extent;
    short targetIndex;
} BattleFxStopSpawnRecord;

/* --- battle_fx_magic_thundara --- */
/* Thundera timeline record: step counter, jittered child spawn position, child record type. */
typedef struct BattleFxMagicBolt2Timeline {
    short ownerSlot;
    short step;
    union {
        Int64Pair position; /* +0x4: copied to the children as eight bytes */
        struct {
            short posX;
            short posY;
            short posZ;
        };
    };
    short allocType;
    unsigned char pad_0E[0x6];
    short pushLength;
} BattleFxMagicBolt2Timeline;

/* Effect-100 slot placed from a Thundara timeline parent record. */
typedef struct BattleFxBolt2Slot {
    unsigned char pad_00[0x4];
    union {
        Int64Pair position; /* +0x4: copied from the timeline */
        struct {
            short posX;
            short posY;
            short posZ;
        };
    };
    unsigned char pad_0C[0x2];
    short scale;
    short velocity;
    short flags;
    short pushLength;
} BattleFxBolt2Slot;

/* Thundera grow-and-fade model record: step counter, damped offset velocity and copied model values. */
typedef struct BattleFxBolt2GrowFade {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short offset;
    short velocity;
} BattleFxBolt2GrowFade;

/* Thundera flipped sprite record with a frame counter, flag bits and an angle step. */
typedef struct BattleFxBolt2FlipSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short flags;
    short depthStep;
} BattleFxBolt2FlipSprite;

/* Flipped sprite record of the Thundera magic, filled from the resource table. */
typedef struct BattleFxBolt2FlippedSprite {
    int sourceFirst;
    int sourceSecond;
    int primCommand;
    short frame;
} BattleFxBolt2FlippedSprite;

/* Task data block of the Thundera billboard sprite, holding a three-step fade history. */
typedef struct BattleFxBolt2FadeSprite {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj;
    char fadeOlder;
    char fadePrev;
    char fadeLevel;
    char primCode;
    short index;
} BattleFxBolt2FadeSprite;

/* Thundera fade billboard slot; the fade step drives the alpha level. */
typedef struct BattleFxBolt2FadeSlot {
    short ownerSlot;
    short fadeStep;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short pushLength;
} BattleFxBolt2FadeSlot;

/* Thundera billboard slot; halved frame counter drives the model request, ending at 16. */
typedef struct BattleFxBolt2BillboardSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short pushLength;
} BattleFxBolt2BillboardSlot;

/* Task-slot record for the Magic Thundera effect, one per target. */
typedef struct BattleFxBolt2Record {
    unsigned char pad_00[0x4];
    short bonePos[3]; /* +0x4: the target's bone position (Battle_GetBoneWorldPos) */
    unsigned char pad_0A[0x2];
    short actorId;
    unsigned char pad_0E[0x6];
    short negRadius;
} BattleFxBolt2Record;

#pragma pack(push, 1)
/* The tail of the flipped sprite's matrix, which starts at g_BattleMagicBolt2_Scale: m11, then the translation. */
typedef struct BattleMagicBolt2_SpriteReq {
    short rotM11; /* +0x0 */
    unsigned char pad_2[0x8];
    int transX; /* +0xa */
    int transY; /* +0xe */
    int transZ; /* +0x12: the flip step is added to it every frame */
    unsigned char pad_16[0x2];
} BattleMagicBolt2_SpriteReq;
#pragma pack(pop)

/* --- battle_fx_magic_todo_skulls --- */
/* Todo timeline record: base point, random spread, rise range, scale and record type. */
typedef struct BattleFxToadTimeline {
    short ownerSlot;
    short frameCount;
    short baseX;
    unsigned char pad_06[0x2];
    short baseY;
    unsigned char pad_0A[0x2];
    short jitterOffset;
    short jitterRange;
    short riseRange;
    unsigned char pad_12[0x2];
    short scale;
    unsigned char pad_16[0x2];
    short recordType;
} BattleFxToadTimeline;

/* Timeline slot spawned per target by the Todo magic. */
typedef struct BattleFxToadTimelineSlot {
    unsigned char pad_00[0x6];
    short posY;
    unsigned char pad_08[0x4];
    short depth;
    short jitterRange;
    short riseRange;
    unsigned char pad_12[0x2];
    short scale;
    unsigned char pad_16[0x2];
    short targetIndex;
} BattleFxToadTimelineSlot;

#pragma pack(push, 1)
typedef struct BattleMagicToadBillboardPoints {
    unsigned char pad_0[0x2];
    int pointRise; /* +0x2 */
    int scale; /* +0x6 */
    unsigned char pad_a[0x6];
} BattleMagicToadBillboardPoints;
#pragma pack(pop)

/* --- battle_fx_magic_tornado --- */
/* Scale of the Tornado model pair, shifted each step: previous and current. */
typedef struct BattleFxTornadoValuePair {
    int previous;
    int baseValue;
    int current;
} BattleFxTornadoValuePair;

/* Spiral particle scratch position; its period and height are the particle's radius and height. */
typedef struct BattleFxTornadoSpiralParticle {
    short period;
    short height;
    short baseY;
    unsigned char pad_06[0x2];
} BattleFxTornadoSpiralParticle;

/* Strip particle scratch position; its period and posY are the strip vertex's radius and height. */
typedef struct BattleFxTornadoStripScratch {
    short period;
    short posY;
    short baseY;
    unsigned char pad_06[0x2];
} BattleFxTornadoStripScratch;

/* Draw-list primitive for the Tornado strip particle: the colour of each of the four corners. */
typedef struct BattleTornadoStripPrimitive {
    int primCode;
    int colorA;
    unsigned char pad_08[0x4];
    int colorB;
    unsigned char pad_10[0x4];
    int colorC;
    unsigned char pad_18[0x4];
    int colorD;
} BattleTornadoStripPrimitive;

/* Task slot record for the Tornado model pair: the frame counter and the target position. */
typedef struct BattleTornadoModelPairRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x4];
    SVECTOR target;
} BattleTornadoModelPairRecord;

/* Task slot of a Tornado particle controller: 0 while it runs, 1 to end once no particle is left, -1 when freed. */
typedef struct BattleFxTornadoControllerTask {
    short state;
} BattleFxTornadoControllerTask;

/* Particle record (0x4c bytes) of the tornado, spiral or strip: -1 in claimState marks a free record. The
 * particle spirals inward; each step it adds a point to a ring of eight that the draw reads back. */
typedef struct BattleFxTornadoParticleRecord {
    short claimState;
    short step;
    short period;
    short height;
    short angle;
    short pointIndex;
    SVECTOR points[8];
} BattleFxTornadoParticleRecord;

/* Data block of the Tornado model pair task: the model matrix and its scale. */
#pragma pack(push, 1)
typedef struct BattleFxTornadoModelWork {
    PsxMatrix matrix;
    BattleFxTornadoValuePair scale;
} BattleFxTornadoModelWork;
#pragma pack(pop)

/* The model table cursor of the Tornado effect, the frame count of the strip controller and the model's spin angle. */
#pragma pack(push, 1)
typedef struct BattleMagicTornado_LoadedRes {
    struct GfxTableCursor *tableCursor; /* +0x0 */
    int counter4; /* +0x4 */
    char rotByte8; /* +0x8 */
    unsigned char pad_9[0x1];
    short angle10; /* +0xa */
} BattleMagicTornado_LoadedRes;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicTornado_ModelReq {
    int tableField; /* +0x0 */
    unsigned char pad_4[0x4];
    int modelHandle; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
    unsigned char pad_14[0x4];
} BattleMagicTornado_ModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicTornado_QuadData {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
} BattleMagicTornado_QuadData;
#pragma pack(pop)

#endif /* FF7_TYPES_BATTLE_MAGIC_H */

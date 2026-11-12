/* include/ff7/types/battle_summon.h: Summon sequence battle effect types. */
#ifndef FF7_TYPES_BATTLE_SUMMON_H
#define FF7_TYPES_BATTLE_SUMMON_H

#include "ff7/types/battle.h"
#include "ff7/camera_script.h"
#include "lib/codegen.h"
#include "lib/types/sound.h"
#include "ff7/types/battle_3d.h"
#include "lib/types/graphics.h"

/* --- battle_effect_summon_actors --- */
/* One row of g_BattleSummonModelFileTable: a summon's model file and the blend mode it loads with. */
typedef struct BattleSummonModelFile {
    const char *path;
    int blendMode;
} BattleSummonModelFile;

/* A task slot that only counts frames down: the summon start and the slot-3 actor wait tasks wait
 * waitFrames frames (0xE) before they end. */
typedef struct BattleSummonWaitSlot {
    short ownerSlot;
    short frameCounter;
    short waitFrames;
} BattleSummonWaitSlot;

/* --- battle_fx_alexander_summon --- */
/* Effect100 slot for the Alexander damped drift sprite task. */
typedef struct BattleSummonAlexanderDriftSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleSummonAlexanderDriftSprite;

/* Effect-slot record of the Alexander summon timeline: variant, spin rate and X/Z velocities per spawn. */
typedef struct BattleFxSummonAlexanderSlot {
    short variant;
    unsigned char pad_02[0x2];
    short heading;
    short spinRate;
    short yVel;
    unsigned char pad_0A[0x2];
    short velocityX;
    short liftStep;
    short velocityZ;
    unsigned char pad_12[0x2];
    int anglesPacked;
    short life;
    short dropAmount;
} BattleFxSummonAlexanderSlot;

/* Alexander shard mesh record: resource index, position, velocity, per-axis angles and frame counter. */
typedef struct BattleFxSummonAlexanderShard {
    short resourceIndex;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    unsigned char angleX;
    unsigned char angleY;
    unsigned char angleZ;
    unsigned char pad_17[0x1];
    short newRotation;
} BattleFxSummonAlexanderShard;

/* Alexander pair task: pair index, two angles with step rates and amplitude. */
typedef struct BattleFxAlexanderPairTask {
    short pendingPairIndex;
    short pairIndex;
    short angleA;
    short angleB;
    short wheelAngle;
    unsigned char pad_0A[0x2];
    short angleStepA;
    short angleStepB;
    unsigned char pad_10[0x8];
    short amplitude;
} BattleFxAlexanderPairTask;

/* Alexander scattered-shard record: camera position, spin angles and vertical velocity. */
typedef struct BattleFxAlexanderShardRecord {
    short meshIndex;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    unsigned char angleX;
    unsigned char angleY;
    unsigned char angleZ;
    unsigned char pad_17[0x1];
    short rotation;
    short velocityY;
} BattleFxAlexanderShardRecord;

/* Alexander summon sprite record with spawn-time velocities. */
typedef struct BattleFxAlexanderSprite {
    unsigned char pad_00[0x4];
    short posX;
    short timer;
    short posY;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    unsigned char pad_10[0x4];
    short scale;
} BattleFxAlexanderSprite;

/* Slot state of the Alexander gradient strip task: a sweep index over 35 strips. */
typedef struct BattleFxAlexanderStripTask {
    short startIndex;
    short stepCount;
    short corner2X;
    unsigned char pad_06[0x2];
    short corner0X;
    unsigned char pad_0A[0x2];
    short corner3X;
    unsigned char pad_0E[0x2];
    short corner1X;
} BattleFxAlexanderStripTask;

/* Per-triangle state for the Alexander gradient: point index, step counter and two blend values. */
typedef struct BattleFxAlexanderGradientState {
    short pointIndex;
    short step;
    SVECTOR cornerA; /* the two corners the spawner rotated into place */
    SVECTOR cornerB;
    unsigned char pad_14[0x8];
    short blendA;
    short blendB;
} BattleFxAlexanderGradientState;

/* Shard spawn record of the Alexander summon with a random variant index. */
typedef struct BattleFxAlexanderShardSlot {
    short variant;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxAlexanderShardSlot;

/* Scaling mesh record of the Alexander summon: a frame counter, a translation vector and a scale. */
typedef struct BattleFxAlexanderScalingMesh {
    short ownerSlot;
    short frameCount;
    short x;
    short y;
    short z;
    unsigned char pad_0A[0x2];
    short speed;
    short angle;
    short rotZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxAlexanderScalingMesh;

/* Primitive pair record of the Alexander gradient effects: pair index. */
typedef struct BattleFxAlexanderPrimitivePair {
    short pairIndex;
    short side;
    SVECTOR cornerA; /* the BattleFxAlexanderGradientState corners */
    SVECTOR cornerB;
    unsigned char pad_14[0x8];
    short stepA;
    short stepB;
} BattleFxAlexanderPrimitivePair;

/* Model draw request of the Alexander summon. */
typedef struct BattleSummonAlexanderModelRequest {
    unsigned char pad_00[0x8];
    int tableValue;
    unsigned char pad_0C[0x6];
    short scale;
} BattleSummonAlexanderModelRequest;

/* Render-state block for the Alexander flat-triangle task. */
typedef struct BattleFxFlatTriBlock {
    int lengthTag;
    union {
        int colorCmd;
        struct {
            unsigned char colorRGB[3];
            char code; /* the GPU command byte */
        };
    };
} BattleFxFlatTriBlock;

/* Transform block of the Alexander summon: the tail of the stage matrix that starts in
 * g_BattleSummonAlexanderMatrixSeam (its last elements and translation), then the
 * offscreen request BattleFx_SummonAlexanderQueueStageOffscreenRequests fills. */
#pragma pack(push, 2)
typedef struct BattleFxAlexanderTransform {
    short matrixTail[4]; /* m11, m12, m20, m21 */
    short billboardX; /* m22, the billboard scale */
    int translation[3];
    unsigned char pad_16[0x2];
    short originX;
    short originY;
    short scaleX;
    short scaleY;
} BattleFxAlexanderTransform;
#pragma pack(pop)

/* Alexander mesh stream record: vertex offsets and a primitive word, read per polygon. */
typedef struct BattleFxAlexanderMeshStream {
    short primSum;
    short offsetQ;
    short offsetVertex;
    short offsetHandle;
    int primWord;
    union {
        struct {
            short uvWord0;
            short uvWord1;
            short offsetP;
            short shadeOrOffset;
            short nextOffsetA;
            short nextOffsetB;
        };
        struct { /* the vertex colours of a gouraud record */
            int color1;
            int color2;
            int color3;
        };
    };
    short nextOffsetC;
    short gouraudQuadOffsetB;
    short gouraudQuadOffsetC;
} BattleFxAlexanderMeshStream;

/* Depth-shaded Alexander mesh stream record: vertex offsets and a primitive word. */
typedef struct BattleFxAlexanderDepthMeshStream {
    short streamWord;
    short offsetB;
    short vertexOffset;
    short handleOffset;
    int primWord;
    union {
        struct {
            short uvWord0;
            short uvWord1;
            short offsetA;
            short shadeOrOffset;
            short offsetC;
            short nextOffsetB;
        };
        struct { /* the vertex colours of a gouraud record */
            int color1;
            int color2;
            int color3;
        };
    };
    short offsetD;
    short gouraudQuadOffsetB;
    short gouraudQuadOffsetC;
} BattleFxAlexanderDepthMeshStream;

/* The data-block scratch of the Alexander scaling mesh task: its transform matrix, then the scale
 * vector ScaleMatrix reads. */
#pragma pack(push, 1)
typedef struct BattleFxAlexanderScalingScratch {
    PsxMatrix matrix;
    unsigned char pad_1E[0xc];
    int scaleX;
    int scaleY;
    int scaleZ;
} BattleFxAlexanderScalingScratch;
#pragma pack(pop)

/* The sprite matrix's point: a short value and the position RotTransPoint writes behind it. */
#pragma pack(push, 1)
typedef struct BattleFxAlexanderSpritePoint {
    short value;
    int x;
    int y;
    int z;
} BattleFxAlexanderSpritePoint;
#pragma pack(pop)

/* Data-block scratch of the Alexander shard tasks: the angles RotMatrixXYZ reads, the scale vector
 * ScaleMatrix reads, then the transform matrix built from them. */
typedef struct BattleFxAlexanderShardScratch {
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_06[0x2];
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix;
} BattleFxAlexanderShardScratch;

#pragma pack(push, 1)
typedef struct BattleSummonAlexanderAnimConfig {
    unsigned char pad_0[0xa];
    short scalingMeshParam; /* +0xa */
    unsigned char pad_c[0x4];
    int growingModelDraw; /* +0x10 */
    unsigned char pad_14[0x6];
    short growthFraction; /* +0x1a */
} BattleSummonAlexanderAnimConfig;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonAlexanderEffectDescriptor2 {
    unsigned char pad_0[0x18];
    int scatterMeshPtr; /* +0x18 */
    int scatterMeshFlags; /* +0x1c */
    unsigned char pad_20[0x2];
    short scatterFraction; /* +0x22 */
} BattleSummonAlexanderEffectDescriptor2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonAlexanderSpriteRequest1 {
    unsigned char pad_0[0x4];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int shardMeshPtr; /* +0x8 */
    int shardMeshFlags; /* +0xc */
    unsigned char pad_10[0x2];
    short shardFraction; /* +0x12 */
} BattleSummonAlexanderSpriteRequest1;
#pragma pack(pop)

/* --- battle_fx_chocomog_summon --- */
/* ChocoMog animated sprite task state: step, position and damped velocity, sprite index. */
typedef struct BattleFxChocoMogSpriteState {
    short ownerSlot;
    short step;
    Vec3sPacked pos;
    unsigned char pad_0A[0x2];
    Vec3sPacked velocity;
    unsigned char pad_12[0x2];
    short spriteIndex;
} BattleFxChocoMogSpriteState;

/* Orbiting sprite record of the Choco/Mog summon: a frame counter, an orbit centre and a phase. */
typedef struct BattleFxChocoMogOrbitSprite {
    short ownerSlot;
    short frameCount;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x4];
    short phase;
} BattleFxChocoMogOrbitSprite;

/* Rising sprite record of the Choco/Mog summon: a scale-table step index, a position and a velocity. */
typedef struct BattleFxChocoMogRisingSprite {
    short ownerSlot;
    short stepIndex;
    Vec3sPacked pos;
    unsigned char pad_0A[0x2];
    Vec3sPacked velocity;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxChocoMogRisingSprite;

/* ChocoMog animated sprite as the spawner fills it: start position and velocity from the orbit angle. */
typedef struct BattleFxChocoMogAnimatedSprite {
    unsigned char pad_00[0x4];
    Vec3sPacked pos;
    unsigned char pad_0A[0x2];
    Vec3sPacked velocity;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxChocoMogAnimatedSprite;

/* The result of RotTransPoint for the descent camera: the rotated point and the projection flags. */
typedef struct BattleFxChocoMogRotResult {
    int vx;
    int vy;
    int vz;
    unsigned char pad_0C[0x4];
    int flags;
} BattleFxChocoMogRotResult;

/* One point of the summon camera script (the eye or the focus): the move in progress, what the
 * point is attached to, its position, the base of the move and the per-frame step. */
typedef struct BattleSummonChocoMogCameraTrack {
    short mode;
    short frames;
    int distance;
    int delta;
    short actor;
    short bone;
    CameraScriptVector position;
    CameraScriptVector base;
    CameraScriptVector velocity;
} BattleSummonChocoMogCameraTrack;

/* One model part of the Choco/Mog polygon model; only whether it counts is read. */
typedef struct BattleFxChocoMogModelPart {
    unsigned char pad_00[0x30];
    int counts;
    unsigned char pad_34[0x4];
} BattleFxChocoMogModelPart;

/* Model header of the ChocoMog detach state, holding a part count and part list. */
typedef struct BattleFxChocoMogModelHeader {
    unsigned char pad_00[0x34];
    int partCount;
    unsigned char pad_38[0x34];
    BattleFxChocoMogModelPart *partList;
} BattleFxChocoMogModelHeader;

/* ChocoMog detach state: model header with part count, and the part pointer table cleared on detach. */
typedef struct BattleFxChocoMogDetachState {
    unsigned char pad_00[0x30];
    BattleFxChocoMogModelHeader *modelHeader;
    unsigned char pad_34[0x1c];
    int *partPointerTable;
} BattleFxChocoMogDetachState;

/* Record for detaching ChocoMog model part pointers: active flag, stage, the first two pointers taken
 * and the table slots they were taken from. */
typedef struct BattleFxModelPartDetach {
    int active;
    int stage;
    int savedA;
    int savedB;
    int *partSlotA;
    int *partSlotB;
} BattleFxModelPartDetach;

#pragma pack(push, 1)
typedef struct BattleSummonChocoMogActorBonePos {
    int *partSlotA; /* +0x0: where BattleFx_SummonChocoMogRestoreModelPartPointers writes the first saved pointer back */
    int *partSlotB; /* +0x4: where it writes the second */
} BattleSummonChocoMogActorBonePos;
#pragma pack(pop)

/* The pulsing sprite's quad request and one more word. */
typedef struct {
    BattleFxSpriteQuadRequest quad; /* +0x0 */
    unsigned char pad_14[0x4];
} BattleSummonChocoMogModelDrawRequest;

#pragma pack(push, 1)
/* The rising sprites' request: their two handles, then the colour. */
typedef struct {
    int spriteResource; /* +0x0 */
    int value; /* +0x4 */
    unsigned char pad_8[0x4];
} BattleSummonChocoMogSpriteConfig;
#pragma pack(pop)

/* The second bone sprite's matrix, then the request both bone sprites are drawn with. */
typedef struct {
    PsxMatrix matrix; /* +0x0 */
    unsigned char pad_1e[0x2];
    BattleFxSpriteRequest sprite; /* +0x20 */
} BattleSummonChocoMogSpriteRequest;

/* The pulsing sprite's scale and screen distance (the rest of the matrix that starts at
 * g_BattleSummonChocoMogScaleFixed12), the orbiting sprites' request, then the rising sprites'
 * two handles, whose colour is the word after this object. */
#pragma pack(push, 1)
typedef struct {
    short pulseScale; /* +0x0 */
    unsigned char pad_2[0x10];
    int geomScreenScale; /* +0x12 */
    unsigned char pad_16[0x2];
    BattleFxSpriteRequest orbitSprite; /* +0x18 */
    int riseSpriteGroup; /* +0x28 */
    int riseDrawObj; /* +0x2c */
} BattleSummonChocoMogTransformMatrix;
#pragma pack(pop)

/* The first bone sprite's matrix: a 0x200 scale, translated to the projected attach point. */
typedef struct {
    PsxMatrix matrix; /* +0x0 */
    unsigned char pad_1e[0x2];
} BattleSummonChocoMogVectorConfig;

/* --- battle_fx_fat_chocobo_summon --- */
/* Summon fat chocobo sprite record: step index, three position/velocity pairs, sprite index. */
typedef struct BattleFxSummonFatChocobo {
    short ownerSlot;
    short step;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velocityA;
    short velocityB;
    short velocityC;
    unsigned char pad_12[0x2];
    short spriteIndex;
} BattleFxSummonFatChocobo;

/* Fat Chocobo summon camera actor: state, bone reference and a rotated point, read by the camera script. */
typedef struct BattleFxFatChocoboActor {
    short state;
    short ready;
    unsigned char pad_04[0x8];
    short boneIndex;
    short boneSubIndex;
    int vector;
    int cameraPosZ;
    unsigned char pad_18[0x10];
} BattleFxFatChocoboActor;

/* Fat Chocobo summon target actor: the same record as the caster, read by the camera script. */
typedef struct BattleFxFatChocoboTargetActor {
    short state;
    short ready;
    unsigned char pad_04[0x8];
    short boneIndex;
    short boneSubIndex;
    int vector;
    int cameraFocusZ;
    unsigned char pad_18[0x10];
} BattleFxFatChocoboTargetActor;

/* Camera-script record that BattleFx_FatChocoboSpawnCameraScript fills for the Fat Chocobo run task. */
typedef struct BattleFxFatChocoboCameraTask {
    unsigned char pad_00[0xc];
    int scriptCursor;
    int matrix;
    short actorIndex;
} BattleFxFatChocoboCameraTask;

#pragma pack(push, 1)
typedef struct BattleSummonFatChocoboActorBonePos {
    int dataBlockPtr; /* +0x0 */
    unsigned char pad_4[0x2];
    short operandTable; /* +0x6 */
} BattleSummonFatChocoboActorBonePos;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonFatChocoboEffectDescriptor {
    int modelRequest; /* +0x0 */
    unsigned char pad_4[0x6];
    short fadeInverse; /* +0xa */
    unsigned char pad_c[0x4];
    int modelDraw; /* +0x10 */
    unsigned char pad_14[0x6];
    short growFraction; /* +0x1a */
} BattleSummonFatChocoboEffectDescriptor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonFatChocoboLightMatrix {
    unsigned char pad_0[0x1a];
    int value; /* +0x1a */
} BattleSummonFatChocoboLightMatrix;
#pragma pack(pop)

/* --- battle_fx_ifrit_summon --- */
/* Ifrit timeline slot: three timeline values and three random shake offsets. */
typedef struct BattleFxIfritTimeline {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short shakeX;
    short shakeY;
    short shakeZ;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxIfritTimeline;

/* Particle record spawned by an Ifrit summon task; position is copied from the basis matrix or vertex data. */
typedef struct BattleFxIfritParticle {
    unsigned char pad_00[0x2];
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short screenX;
    short screenY;
    short velocityZ;
} BattleFxIfritParticle;

/* Ifrit scale-fade state: frame counter and the two target scales it interpolates. */
typedef struct BattleFxIfritScaleFade {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetA;
    short targetB;
} BattleFxIfritScaleFade;

/* Ifrit pulsing-fade state: frame counter, phase accumulator and amplitude. */
typedef struct BattleFxIfritPulseState {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short phase;
    short angleZ;
    unsigned char pad_12[0x2];
    short amplitude;
} BattleFxIfritPulseState;

/* Effect record of the Ifrit animated-sprite task, kept in the Effect100 array. */
typedef struct BattleFxIfritSpriteRecord {
    short ownerSlot;
    short frameIndex;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x2];
    short sizeW;
    short sizeH;
} BattleFxIfritSpriteRecord;

/* Request record of the Ifrit twin rotating and scale-fade models, with table resources and steps. */
typedef struct BattleFxIfritTwinModelReq {
    unsigned char pad_00[0x10];
    int rotatingModel;
    unsigned char pad_14[0x6];
    short rotateStep;
    unsigned char pad_1C[0x4];
    int scaleFadeModel;
    unsigned char pad_24[0x6];
    short fadeStep;
} BattleFxIfritTwinModelReq;

/* Ring buffer record for the Ifrit summon, with rotation matrix and angle fields. */
typedef struct BattleFxSummonIfritRingRecord {
    unsigned char pad_00[0x4];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_0A[0x6];
    short angleY;
} BattleFxSummonIfritRingRecord;

/* Animated sprite child record with random-seeded position and size fields. */
typedef struct BattleFxSummonIfritAnimatedSprite {
    unsigned char pad_00[0x4];
    short posX;
    unsigned char pad_06[0x2];
    short posY;
    unsigned char pad_0A[0x2];
    short sizeW;
    short sizeH;
} BattleFxSummonIfritAnimatedSprite;

/* Camera-script task record for Ifrit: script, rotation matrix and actor index. */
typedef struct BattleFxIfritCameraTask {
    unsigned char pad_00[0xc];
    int cameraTrack;
    int rotMatrix;
    short actorIndex;
} BattleFxIfritCameraTask;

/* Particle spawn record at task offset 0x1E, filled from a short pair and a random value. */
typedef struct BattleFxIfritMeshParticleRecord {
    short halfExtentX;
    short halfExtentY;
    short jitterZ;
} BattleFxIfritMeshParticleRecord;

/* Per-vertex step toward the Ifrit morph target, one short per axis. */
typedef struct BattleFxIfritMorphStep {
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_06[0x2];
} BattleFxIfritMorphStep;

/* Per-axis camera shake amplitudes subtracted or added to a billboard position. */
typedef struct BattleFxIfritShakeAmplitude {
    short amplitudeX;
    short amplitudeY;
    short amplitudeZ;
} BattleFxIfritShakeAmplitude;

/* Timeline state whose three shorts are copied into the timeline task record. */
typedef struct BattleFxIfritTimelineState {
    unsigned char pad_00[0x1e];
    short transformedX;
    unsigned char pad_20[0x2];
    short transformedY;
    unsigned char pad_24[0x2];
    short transformedZ;
} BattleFxIfritTimelineState;

/* Flat textured PSX polygon the Ifrit summon mesh draw writes: a triangle (tag length 7) or a quad
 * (tag length 9). The GTE writes the screen positions into the pads. */
typedef struct BattleFxIfritMeshCursorPrim {
    int tag;
    union {
        int shade;
        struct {
            unsigned char shadeRGB[3];
            char primCode; /* the GPU command byte */
        };
    };
    unsigned char pad_08[0x4];
    int uvClut;
    unsigned char pad_10[0x4];
    short uv1;
    short tpage;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxIfritMeshCursorPrim;

/* Ifrit basis matrix record, filled by the mesh spawn particles task. */
#pragma pack(push, 2)
typedef struct BattleFxIfritBasisMatrix {
    short matrixM00;
    unsigned char pad_02[0x6];
    short matrixM11;
    unsigned char pad_0A[0x6];
    short matrixM22;
    short posX;
    unsigned char pad_14[0x2];
    union {
        short posY;
        int transY;
    } y;
    int transZ;
} BattleFxIfritBasisMatrix;
#pragma pack(pop)

/* The Ifrit summon mesh's rotation matrix (30 bytes, translation in ints) followed by the camera-script
 * operand table, which the interpreter indexes backwards from its first element. */
#pragma pack(push, 2)
typedef struct BattleFxIfritMeshState {
    short matrixM00;
    unsigned char pad_02[0x6];
    short matrixM11;
    unsigned char pad_0A[0x6];
    short matrixM22;
    int transX;
    int transY;
    int transZ;
    short table[1];
} BattleFxIfritMeshState;
#pragma pack(pop)

/* Per-sprite shake offsets of the Ifrit flame sprites; the pad is never read. */
typedef struct BattleFxIfritShakeOffset {
    short x;
    short y;
    short z;
    short pad;
} BattleFxIfritShakeOffset;

/* Sprite-transform record the animated flame sprites fill before drawing: two scale elements and the
 * sprite's position. */
#pragma pack(push, 2)
typedef struct BattleFxIfritSpriteTransform {
    short m00;
    unsigned char pad_02[0x2];
    short m02;
    unsigned char pad_06[0x4];
    int posX;
    int posY;
    int posZ;
    unsigned char pad_16[0x2];
} BattleFxIfritSpriteTransform;
#pragma pack(pop)

/* The Ifrit pulsing model's matrix: 3x3 shorts, translation, then the scale vector ScaleMatrix reads. */
#pragma pack(push, 2)
typedef struct BattleFxIfritPulseMatrix {
    unsigned char pad_00[0x12];
    int transX;
    int transY;
    int transZ;
    int scaleX;
    int scaleY;
    int scaleZ;
} BattleFxIfritPulseMatrix;
#pragma pack(pop)

/* One primitive of the PSX mesh stream BattleFx_SummonIfritDrawPsxMesh walks. The stream has four
 * sections: flat textured triangles (ft3, 0x10 bytes each), flat textured quads (ft4, 0x14),
 * gouraud triangles (g3, 0x14) and gouraud quads (g4, 0x18). Each section starts with a word holding
 * its primitive count; the first section's second word is the attribute word. A primitive starts with the offsets of its vertices from the mesh
 * data; the draw loop reads the next primitive's three offsets (nextVertex) before it advances. */
typedef struct BattleFxIfritMeshStream {
    union {
        short count;      /* section header: the number of primitives */
        short attributes; /* section header, second word: the semi-transparency and texture page bits */
        short vertexA;
    };
    short vertexB;
    short vertexC;
    union {
        short vertexD; /* quads */
        short shade;   /* ft3: the grey level in the low byte */
    };
    union {
        struct {
            int uvClut;
            short uv1;
            short uv2;
            short nextVertex[3];
        } ft3;
        struct {
            int uvClut;
            short uv1;
            short uv2;
            short uv3;
            short shade; /* the grey level in the low byte */
            short nextVertex[3];
        } ft4;
        struct {
            int color0;
            int color1;
            int color2;
            short nextVertex[3];
        } g3;
        struct {
            int color0;
            int color1;
            int color2;
            int color3;
            short nextVertex[3];
        } g4;
    };
} BattleFxIfritMeshStream;

#pragma pack(push, 1)
/* The PSX rectangle (TILE) an Ifrit spark draws. */
typedef struct BattleFxIfritPointDotPrim {
    int tag;   /* +0x0 */
    char r;    /* +0x4 */
    char g;    /* +0x5 */
    char b;    /* +0x6 */
    char code; /* +0x7: the GPU command byte */
    short x;   /* +0x8 */
    short y;   /* +0xa */
    short w;   /* +0xc */
    short h;   /* +0xe */
} BattleFxIfritPointDotPrim;
#pragma pack(pop)

#pragma pack(push, 1)
/* Task slot of an Ifrit spark: its frames left, position and velocity. */
typedef struct BattleFxIfritPointDotSlot {
    short ownerSlot;  /* +0x0 */
    short framesLeft; /* +0x2 */
    short posX;       /* +0x4 */
    short posY;       /* +0x6 */
    short posZ;       /* +0x8 */
    unsigned char pad_a[0x2];
    short velocityX;  /* +0xc */
    short velocityY;  /* +0xe */
    short velocityZ;  /* +0x10 */
    unsigned char pad_12[0x2];
} BattleFxIfritPointDotSlot;
#pragma pack(pop)

#pragma pack(push, 1)
/* Offscreen capture request of the Ifrit heat-warp strips (Battle3d_AcquireOffscreenRequest): the
 * screen rectangle to capture, the UI scale and the slot the capture lands in. */
typedef struct BattleFxIfritStripWarpReq {
    int kind; /* +0x0 */
    unsigned char pad_4[0xc4];
    int pageFillEnabled; /* +0xc8 */
    int renderKind; /* +0xcc */
    int blendMode; /* +0xd0 */
    unsigned char pad_d4[0x8];
    int rectX; /* +0xdc */
    int rectY; /* +0xe0 */
    int rectW; /* +0xe4 */
    int rectH; /* +0xe8 */
    unsigned char pad_ec[0x1c];
    int pageFillMode; /* +0x108 */
    int scaleX; /* +0x10c */
    int scaleY; /* +0x110 */
    int slot;   /* +0x114 */
} BattleFxIfritStripWarpReq;

/* One heat-warp strip: a flat textured, semi-transparent PSX quad (POLY_FT4) whose texture is the
 * captured screen. */
typedef struct BattleFxIfritStripPacket {
    int tag;
    union {
        int colorCode;
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char code;
        };
    };
    short x0;
    short y0;
    unsigned char u0;
    unsigned char v0;
    short clut;
    short x1;
    short y1;
    unsigned char u1;
    unsigned char v1;
    short tpage;
    short x2;
    short y2;
    unsigned char u2;
    unsigned char v2;
    unsigned char pad_1E[0x2];
    short x3;
    short y3;
    unsigned char u3;
    unsigned char v3;
    unsigned char pad_26[0x2];
} BattleFxIfritStripPacket;
#pragma pack(pop)

/* --- battle_fx_kjata_summon --- */
/* One segment of Kjata's branching trail: position, velocity, age and reload timer. */
typedef struct BattleFxKjataTrailSegment {
    short branchIndex;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short timer;
} BattleFxKjataTrailSegment;

/* One 8-byte entry of g_BattleSummonKjataColorTable, indexed by a trail node's mode bits: the texture words of a trail quad. */
typedef struct BattleSummonKjataColorEntry {
    unsigned short uv0;
    unsigned short uv1;
    short uv2;
    short uv3;
} BattleSummonKjataColorEntry;

/* Summon Kjata trail node, 0x14 bytes, copied into the branching trail quads. */
typedef struct BattleSummonKjataTrailNode {
    unsigned char branchIndex;
    char randomSeed;
    char resIndex;
    char flags;
    short posX;
    short posY;
    short posZ;
    short swayOffset;
    short savedPosX;
    short savedPosY;
    short savedPosZ;
    unsigned char pad_12[0x2]; /* to 0x14 bytes, the step of every pointer through it */
} BattleSummonKjataTrailNode;

/* Kjata hopping sprite slot: stage and tick counters, and a sine-driven hop height. */
typedef struct BattleFxKjataHoppingSprite {
    short stage;
    short tick;
    short posX;
    short height;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short posY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short hopHeight;
    short travelTime;
} BattleFxKjataHoppingSprite;

/* Kjata offset triangle: three vertices relative to their centroid, centroid at 0x20. */
typedef struct BattleFxKjataOffsetTriangle {
    short v0X;
    short v0Y;
    short v0Z;
    unsigned char pad_06[0x2];
    short v1X;
    short v1Y;
    short v1Z;
    unsigned char pad_0E[0x2];
    short v2X;
    short v2Y;
    short v2Z;
    unsigned char pad_16[0xa];
    short centroidX;
    short centroidY;
    short centroidZ;
} BattleFxKjataOffsetTriangle;

/* Kjata branching trail record: four wander angles that drive the trail offsets. */
typedef struct BattleFxKjataTrailRecord {
    short angleA;
    short angleB;
    short phaseX;
    short angleC;
    short phaseY;
    short angleD;
} BattleFxKjataTrailRecord;

/* GPU primitive packet for a blended Kjata triangle, filled from the render state block. */
typedef struct BattleFxKjataBlendedPacket {
    int tag;
    int colorCmd;
    unsigned char pad_08[0x4];
    int packedWord;
    unsigned char pad_10[0x4];
    short vertexOffsetA;
    short polyAttr;
    unsigned char pad_18[0x4];
    short vertexOffsetB;
} BattleFxKjataBlendedPacket;

/* GPU primitive packet for an offset Kjata triangle, filled from the render state block. */
typedef struct BattleFxKjataOffsetPacket {
    int tag;
    int colorCmd;
    unsigned char pad_08[0x4];
    int packedWord;
    unsigned char pad_10[0x4];
    short vertexOffsetA;
    short polyAttrib;
    unsigned char pad_18[0x4];
    short vertexOffsetB;
} BattleFxKjataOffsetPacket;

/* Kjata branching trail segment: polar offset from a base point, a frame counter and mode flags. */
typedef struct BattleFxKjataTrailBranch {
    unsigned char active;
    unsigned char modeFlags;
    unsigned char pad_02[0x1];
    unsigned char frameCounter;
    short length;
    short trailWidth;
    short angle;
    short branchAngle;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_12[0x2]; /* to 0x14 bytes, the step of every pointer through it */
} BattleFxKjataTrailBranch;

/* Kjata summon fragmented-mesh task state: fixed-point values shifted each frame. */
typedef struct BattleFxKjataFragment {
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_06[0x2];
    int prevValue;
    int value;
    int nextValue;
    unsigned char pad_14[0x4];
    PsxMatrix worldMatrix;
    PsxMatrix localMatrix;
} BattleFxKjataFragment;

/* Kjata summon offset triangle record with sprite fields, drawn with fragment sprites. */
typedef struct BattleFxKjataSpriteTriangle {
    short flags;
    short vertexBOffset;
    short vertexCOffset;
    short color;
    int packedWord;
    short vertexOffsetA;
    short vertexOffsetB;
} BattleFxKjataSpriteTriangle;

/* Per-effect slot used by the Kjata summon timeline task. */
typedef struct BattleFxKjataTimelineState {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
} BattleFxKjataTimelineState;

/* Kjata target-wave state: actor index, frame counter, and saved actor flag and height. */
typedef struct BattleFxKjataWaveTarget {
    short actorIndex;
    short frame;
    short savedFlags;
    short savedHeight;
} BattleFxKjataWaveTarget;

/* Battle actor record fields that the Kjata target wave reads and writes. */
typedef struct BattleActorWaveFields {
    unsigned char pad_00[0x25];
    unsigned char flags;
    unsigned char pad_26[0x140];
    short posX;
    short waveHeight;
    short posZ;
} BattleActorWaveFields;

/* Colour fade record of the Kjata summon: an RGB colour, fade progress and fade step. */
typedef struct BattleFxKjataColorFade {
    short ownerSlot;
    unsigned char pad_02[0x2];
    char colorChannel0;
    char colorChannel1;
    char colorChannel2;
    unsigned char pad_07[0x1];
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char pad_0B[0x1];
    short fadeProgress;
    short fadeStep;
} BattleFxKjataColorFade;

/* Scaling fade record of the first Kjata black model: a frame counter and a vector written to the draw handle. */
typedef struct BattleFxKjataBlackModel1Fade {
    short stepIndex;
    short frameCount;
    short x;
    short y;
    short z;
    unsigned char pad_0A[0x2];
    union {
        SVECTOR angles; /* +0xC: the Y angle is the fade level */
        struct { short angleX; short fadeLevel; };
    };
} BattleFxKjataBlackModel1Fade;

/* Kjata branching trail: start position and horizontal offsets from sine and cosine of the trail angle. */
typedef struct BattleFxKjataBranchTrail {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short branchTimer;
} BattleFxKjataBranchTrail;

/* Kjata black model flash record: frame counter, with flash parameters written to globals. */
typedef struct BattleFxKjataBlackFlash {
    short ownerSlot;
    short frameIndex;
    short transformA;
    short transformB;
    unsigned char pad_08[0xc];
    short flashLevel;
} BattleFxKjataBlackFlash;

/* Kjata sprite particle: horizontal direction from sine and cosine of its phase angle. */
typedef struct BattleFxKjataParticle {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short height;
    short posZ;
    unsigned char pad_0A[0x2];
    short dirX;
    short dirY;
    short dirZ;
} BattleFxKjataParticle;

/* Ring-buffer record for the Kjata timeline: a position and a table pointer. */
typedef struct BattleFxKjataTimelineRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    int normalData;
    short phase;
} BattleFxKjataTimelineRecord;

/* GPU packet for the branching Kjata trail quads: a tag word and colour words. */
typedef struct BattleFxKjataTrailPacket {
    int tag;
    int colorCommand;
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    int uv1Tpage;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxKjataTrailPacket;

/* Render packet for one Kjata sprite triangle, written into the render state block. */
typedef struct BattleFxKjataSpritePacket {
    int tag;
    int command;
    unsigned char pad_08[0x4];
    int packedWord;
    unsigned char pad_10[0x4];
    short vertexOffsetA;
    short polyFlagMask;
    unsigned char pad_18[0x4];
    short vertexOffsetB;
} BattleFxKjataSpritePacket;

/* Effect-array slot set up when a Kjata black model spawns; randomValue is a random value. */
typedef struct BattleFxKjataBlackModelSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short randomValue;
    short rotZ;
} BattleFxKjataBlackModelSpawnSlot;

/* Effect-array slot that grows a Kjata black model; frameCount counts frames. */
typedef struct BattleFxKjataBlackModelGrowSlot {
    short stepIndex;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    SVECTOR angles; /* +0xC */
} BattleFxKjataBlackModelGrowSlot;

/* Effect-array slot for a stretching, fading Kjata model; velocities come from an angle. */
typedef struct BattleFxKjataStretchFadeSlot {
    unsigned char pad_00[0x4];
    short velocitySin;
    short posY;
    short velocityCos;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
} BattleFxKjataStretchFadeSlot;

/* Kjata trail state: a three-sample value history shifted each frame, plus two short values. */
typedef struct BattleFxKjataTrailState {
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_06[0x2];
    int prevValue;
    int currValue;
    int nextValue;
    unsigned char pad_14[0x4];
    PsxMatrix worldMatrix;
    PsxMatrix localMatrix;
} BattleFxKjataTrailState;

/* Kjata target wave state: two wave offsets and the two computed heights for the target actor. */
typedef struct BattleFxKjataWaveTargets {
    short waveOffsetA;
    short waveOffsetY;
    short waveOffsetB;
    unsigned char pad_06[0x2];
    int heightA;
    int actorOffsetY;
    int heightB;
} BattleFxKjataWaveTargets;

/* Kjata summon particle sprite record with a pending flag and a frame index. */
typedef struct BattleSummonKjataParticle {
    short pending;
    short frame;
    unsigned char pad_04[0x2];
    short posY;
} BattleSummonKjataParticle;

/* Target position of the Kjata hopping sprite, lerped into the sprite each frame. */
typedef struct BattleFxKjataHopTarget {
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_06[0x2];
    int endX;
    int endY;
    int endZ;
} BattleFxKjataHopTarget;

/* Slot record of the Kjata branching trails task: countdown timer and frame counter. */
typedef struct BattleFxKjataBranchTrailTask {
    short timer;
    short frameCount;
} BattleFxKjataBranchTrailTask;

/* Slot record of the Kjata trail animated sprite task. */
typedef struct BattleFxKjataTrailSpriteTask {
    short ownerSlot;
    short spriteIndex;
    unsigned char pad_04[0x10];
    short spriteScale;
} BattleFxKjataTrailSpriteTask;

/* Slot record of the Kjata stretching fade models task. */
typedef struct BattleFxKjataStretchFadeTask {
    short stepIndex;
    short frameCount;
    unsigned char pad_04[0xa];
    short stretchAmount;
} BattleFxKjataStretchFadeTask;

/* Spawn record of a Kjata scaling model: direction sine and cosine, and angle. */
typedef struct BattleFxKjataScalingRecord {
    unsigned char pad_00[0x4];
    short dirSin;
    unsigned char pad_06[0x2];
    short dirCos;
    unsigned char pad_0A[0x2];
    short rotX;
    short angle;
    short rotZ;
} BattleFxKjataScalingRecord;

/* Slot record of the Kjata scaling model task; same slot as the Kjata scaling spawn record. */
typedef struct BattleFxKjataScalingTask {
    short stepIndex;
    short frameCount;
    short dirSin;
    unsigned char pad_06[0x2];
    short dirCos;
    unsigned char pad_0A[0x2];
    SVECTOR angles; /* +0xC */
} BattleFxKjataScalingTask;

/* Slot record of the Kjata scaling fade model task. */
typedef struct BattleFxKjataScalingFadeTask {
    short stepIndex;
    short frameCount;
    unsigned char pad_04[0x10];
    short fadeLevel;
} BattleFxKjataScalingFadeTask;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxSummonKjataRingWave {
    int vertexA;
    int vertexB;
    int quadC;
    int quadD;
} BattleFxSummonKjataRingWave;

/* Jitter envelope task record with four parameter words. */
typedef struct BattleFxSummonJitterEnvelope {
    unsigned char pad_00[0x4];
    short param1;
    short param2;
    short param3;
    short param4;
} BattleFxSummonJitterEnvelope;

/* Camera-script record that BattleFx_KjataSpawnCameraScript fills for the Kjata run task. */
typedef struct BattleFxKjataCameraTask {
    unsigned char pad_00[0xc];
    int scriptData;
    int matrixPtr;
    short actorSlot;
} BattleFxKjataCameraTask;

/* Child task-slot record for the Kjata black model flash, seeded with random values at offsetX and offsetY. */
typedef struct BattleFxKjataBlackModelFlashTask {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    unsigned char pad_08[0xc];
    short flashLevel;
} BattleFxKjataBlackModelFlashTask;

/* Task-slot record for the Kjata scaling models spawner; timer is a frame counter. */
typedef struct BattleFxKjataScalingModelsTask {
    short stepIndex;
    short timer;
} BattleFxKjataScalingModelsTask;

/* Particle sprite record for the Kjata summon, active when claimed. */
typedef struct BattleFxKjataParticleSprite {
    short active;
    short frame;
} BattleFxKjataParticleSprite;

/* Particle state for the Kjata summon, set up by its sprite array task. */
typedef struct BattleSummonKjataParticleState {
    short m00;
    short m01;
    short m02;
    short m10;
    short m11;
    short m12;
    short m20;
    short m21;
    short m22;
    unsigned char pad_12[0x12];
    int spriteResource;
    int drawObject;
    int color;
    short spriteIndex;
} BattleSummonKjataParticleState;

/* Vertex of the Kjata ring grid mesh, 0x28 bytes each. */
typedef struct BattleFxKjataRingVertex {
    unsigned char pad_00[0x10];
    int tag;
    unsigned char pad_14[0x8];
    char texU;
    char texV;
    unsigned char pad_1E[0x6];
    char texU2;
    char texV2;
    short texPage;
} BattleFxKjataRingVertex;


/* Model draw request of the Kjata scaling and fading summon. */
typedef struct BattleFxKjataScaleFadeRequest {
    unsigned char pad_00[0x28];
    int modelGroup;
    unsigned char pad_2C[0x6];
    short x;
} BattleFxKjataScaleFadeRequest;

/* Summon Kjata ring-grid effect state; holds a depth value and flag bits. */
typedef struct BattleFxKjataEffectState {
    int depth;
    unsigned char pad_04[0x4];
    int flags;
} BattleFxKjataEffectState;

/* Model descriptor for the Kjata black-model expanding effect. */
typedef struct BattleSummonKjataModelDescriptor {
    unsigned char pad_00[0x50];
    int modelRecord;
    unsigned char pad_54[0x6];
    short fadeLevel;
} BattleSummonKjataModelDescriptor;

/* GPU textured primitive packet written by BattleFx_SummonKjataDrawPsxMesh. */
typedef struct BattleFxKjataPrim {
    int tag;
    union {
        int colorCmd;
        CVECTOR color;
        struct { unsigned char pad_04[0x3]; char colorCode; };
    };
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    short cmdWord;
    short bits;
    unsigned char pad_18[0x4];
    short attrWord;
    unsigned char pad_1E[0x6];
    short vertAltOffset;
} BattleFxKjataPrim;

/* Kjata PSX mesh packet: tag word and words copied from the source stream. */
typedef struct BattleFxKjataPsxPacket {
    int tag;
    union {
        int primWord;
        CVECTOR color0;
        struct { unsigned char pad_04[0x3]; char code; };
    };
    unsigned char pad_08[0x4];
    union {
        int headerWord;
        CVECTOR color1;
    };
    unsigned char pad_10[0x4];
    union {
        int colorWord;
        CVECTOR color2;
    };
    unsigned char pad_18[0x4];
    union {
        int vertexColor3;
        CVECTOR color3;
    };
} BattleFxKjataPsxPacket;

/* Kjata summon transform matrix: short words for the offscreen stage request. */
typedef struct BattleSummonKjataTransform {
    unsigned char pad_00[0x8];
    short m11;
    unsigned char pad_0A[0xe];
    short viewportOriginX;
    short viewportOriginY;
    short prevScale;
    short scale;
} BattleSummonKjataTransform;

/* Kjata black growing state: first value set from r, then fixed at 0x4000. */
typedef struct BattleFxKjataBlackGrowState {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix;
} BattleFxKjataBlackGrowState;

/* Kjata summon mesh stream record: vertex offsets and a primitive word, read per polygon. */
typedef struct BattleFxKjataMeshStream {
    short vertAOffset;
    short vertBOffset;
    short vertCOffset;
    short resourceOffset;
    int primWord;
    short cmdWord;
    short attrWord;
    short vertAltOffset;
    short vertDOffset;
    short vertEOffset;
    short vertFOffset;
    short vertGOffset;
    short nextVertBOffset;
    short nextVertCOffset;
} BattleFxKjataMeshStream;

/* Kjata triangle fragment sprite record holding vertex-array offsets for its P, Q and C vertices. */
typedef struct BattleFxKjataFragmentRecord {
    short primWord;
    short vertexOffsetQ;
    short vertexOffsetC;
} BattleFxKjataFragmentRecord;

/* Byte offsets of one triangle's vertex and normal records in the Kjata block. */
typedef struct BattleFxKjataTriangleOffsets {
    short vertexBOffset;
    short normalOffset;
    short vertexAOffset;
} BattleFxKjataTriangleOffsets;

/* Kjata mesh record; its words are copied into triangle and previous primitives. */
typedef struct BattleFxKjataMeshRecord {
    unsigned char pad_00[0xc];
    int headerWord;
    int colorWord;
    int vertexColor3;
} BattleFxKjataMeshRecord;

/* The tail of the sprite matrix BattleFx_SummonKjataInstallSpriteMatrix builds from
 * g_BattleSummonKjataQuad: m[2][2], then the translation. */
#pragma pack(push, 1)
typedef struct {
    short m22;
    int t[3]; /* +0x2 */
} BattleSummonKjataMatrixTail;
#pragma pack(pop)

/* A cursor over the packed mesh stream of BattleFx_SummonKjataDrawPsxMesh: the sections hold
 * records of different sizes, so the walker steps it by byte counts. */
typedef union {
    int addr;
    BattleFxKjataMeshStream *stream;
    BattleFxKjataMeshRecord *record;
} BattleFxKjataMeshCursor;

#pragma pack(push, 1)
typedef struct BattleSummonKjataUnusedState {
    short auxVecX; /* +0x0 */
    short auxVecY; /* +0x2 */
} BattleSummonKjataUnusedState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataCameraData {
    unsigned char flags; /* +0x0 */
    unsigned char pad_1[0x1];
    short operandTable; /* +0x2 */
} BattleSummonKjataCameraData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataModelDescriptorObject {
    unsigned char pad_0[0x10];
    int blackModelRequest; /* +0x10 */
    unsigned char pad_14[0x6];
    short blackModelFlashWord; /* +0x1a */
    unsigned char pad_1c[0x4];
    short blackModelRot1; /* +0x20 */
    unsigned char pad_22[0x6];
    short blackModelRot2; /* +0x28 */
    unsigned char pad_2a[0x6];
    short blackModelRot3; /* +0x30 */
    int blackModelTransformA; /* +0x32 */
    int blackModelTransformB; /* +0x36 */
    unsigned char pad_3a[0x6];
    int growingModelDraw; /* +0x40 */
    unsigned char pad_44[0x6];
    short growingModelParam; /* +0x4a */
    unsigned char pad_4c[0x14];
    int fragmentRequest; /* +0x60 */
    int fragmentSpriteAux; /* +0x64 */
} BattleSummonKjataModelDescriptorObject;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataModelDrawRequest2 {
    unsigned char pad_0[0x18];
    int scalingModelDraw; /* +0x18 */
    unsigned char pad_1c[0x6];
    short scalingModelScale; /* +0x22 */
    unsigned char pad_24[0x10];
} BattleSummonKjataModelDrawRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataOffsetVector {
    unsigned char pad_0[0x18];
    int spriteOffsetX; /* +0x18 */
    int spriteOffsetY; /* +0x1c */
} BattleSummonKjataOffsetVector;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataSpriteRequest2 {
    unsigned char pad_0[0x8];
    int blackModel1Draw; /* +0x8 */
    unsigned char pad_c[0x6];
    short blackModel1Inverse; /* +0x12 */
} BattleSummonKjataSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataSpriteRequest3 {
    unsigned char pad_0[0x8];
    int counterModelDraw; /* +0x8 */
    unsigned char pad_c[0x6];
    short counterRotationAngle; /* +0x12 */
    unsigned char pad_14[0x4];
    int counterModelPart; /* +0x18 */
    unsigned char pad_1c[0x6];
    short counterRotationPhase; /* +0x22 */
} BattleSummonKjataSpriteRequest3;
#pragma pack(pop)

/* --- battle_fx_phoenix_summon --- */
typedef struct BattlePhoenixBlock64 {
    int words[16];
} BattlePhoenixBlock64;

/* Entry in the SUMMON/FENIC model/sprite resource loader table. */
typedef struct {
    int flags;
    unsigned char *pathA;
    unsigned char *pathB;
    int reservedA;
    int reservedB;
} BattleFxPhoenixResourceRecord;

/* Tail of the SUMMON/FENIC resource loader table after the first entry's flags and path. */
typedef struct {
    int firstRecordTail[3];
    BattleFxPhoenixResourceRecord records[8];
    int terminator;
} BattleFxPhoenixResourceTableTail;

/* Pitch and yaw rotation shorts for the Phoenix summon orientation. */
typedef struct {
    short rotX;
    short rotY;
} BattleFxPhoenixAnglesXY;

/* 32-bit fixed-point angle accessed both as a full int and narrowed to a 16-bit turn. */
typedef union {
    int value;
    short angle;
} BattleFxPhoenixAngleScalar;

/* Phoenix effect slot parameters written at spawn; 0x1000 is unity scale. */
typedef struct BattleFxPhoenixSpawnRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxPhoenixSpawnRecord;

/* Phoenix ring column record: spawn index, copied position, and a height that steps down. */
typedef struct BattleFxPhoenixRingColumnRecord {
    short ownerSlot;
    short spawnIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    union {
        SVECTOR angles;
        struct {
            short height;
            unsigned char pad_0E[0x6];
        };
    };
    short peakHeight;
    short heightStep;
} BattleFxPhoenixRingColumnRecord;

/* Phoenix summon trail task slot: frame counter and start and end positions. */
typedef struct BattleFxPhoenixTrailModel {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
} BattleFxPhoenixTrailModel;

/* Phoenix counter-spin model state: three fixed-point scalars shifted each step. */
typedef struct BattleFxPhoenixCounterSpinState {
    int previous;
    int current;
    int next;
} BattleFxPhoenixCounterSpinState;

/* Task-slot record for the Phoenix rising sprite emitter: frame counter, base position and rise speed. */
typedef struct BattleFxPhoenixRiseTask {
    short ownerSlot;
    short frame;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x4];
    short riseSpeed;
} BattleFxPhoenixRiseTask;

/* Task-slot record for the Phoenix spin quad: frame counter, position, rotation, size and spin speed. */
typedef struct BattleFxPhoenixSpinQuadTask {
    short ownerSlot;
    short frame;
    SVECTOR pos;
    union {
        SVECTOR rot;
        struct {
            short rotX;
            short rotY;
            short angle;
            short pad_12;
        };
    };
    short size;
    short spinSpeed;
} BattleFxPhoenixSpinQuadTask;

/* Falling fading sprite record of the Phoenix summon: frame counter, position, velocity and scale. */
typedef struct BattleFxPhoenixFallingFadeSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    short rotation;
} BattleFxPhoenixFallingFadeSprite;

/* Ring column record of the Phoenix summon: a position on a ring and its angle. */
typedef struct BattleFxPhoenixRingColumn {
    short ownerSlot;
    short stepCount;
    short x;
    short y;
    short z;
    unsigned char pad_0A[0x2];
    short rotX;
    short angle;
    short rotZ;
    unsigned char pad_12[0x2];
    short peakHeight;
    short rotXStep;
} BattleFxPhoenixRingColumn;

/* PSX POLY_FT4 GPU packet embedded in each ring grid quad. */
typedef struct {
    int tag;
    int color;
    short x0;
    short y0;
    char u0;
    char v0;
    short clut;
    short x1;
    short y1;
    char u1;
    char v1;
    short texPage;
    short x2;
    short y2;
    char u2;
    char v2;
    short pad2;
    short x3;
    short y3;
    char u3;
    char v3;
    short pad3;
} BattleFxPhoenixRingPolyFt4;

/* Ring grid quad of the Phoenix summon: four vertex pointers and an embedded POLY_FT4 packet. */
typedef struct {
    SVECTOR *vertexA;
    SVECTOR *vertexB;
    SVECTOR *vertexC;
    SVECTOR *vertexD;
    BattleFxPhoenixRingPolyFt4 poly[1];
} BattleFxPhoenixRingGridQuad;

/* Phoenix spin quad: copied 8-byte position, Euler angles, lifetime timer and signed spin speed. */
typedef struct BattleFxPhoenixSpinQuad {
    short ownerSlot;
    short spawnIndex;
    Int64Pair pos;
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x2];
    short timer;
    short spinSpeed;
} BattleFxPhoenixSpinQuad;

/* Effect-array slot for a Phoenix billboard sprite, animated over 30 frames. */
typedef struct BattleFxPhoenixBillboardSlot {
    short ownerSlot;
    short frameCount;
    union {
        SVECTOR pos;
        struct {
            short posX;
            short posY;
            short value;
            short pad_0A;
        };
    };
    unsigned char pad_0C[0x4];
    short step;
    unsigned char pad_12[0x2];
    short rotation;
} BattleFxPhoenixBillboardSlot;

/* Offscreen render request for the Phoenix summon stage, filled from the stage or actor mesh. */
typedef struct BattleSummonPhoenixRequest {
    int state;
    int partMesh;
    int animBank;
    int animFramePtr;
    int modelParts;
    unsigned char pad_14[0x74];
    union {
        BattlePhoenixBlock64 worldMatrix;
        float worldMatrixFloats[16];
    };
    int colorKeyEnabled;
    int otDepthShift;
    int blendMode;
    unsigned char pad_D4[0x8];
    int viewportX;
    int viewportY;
    int viewportW;
    int viewportH;
    unsigned char pad_EC[0x20];
    int sampleStepX;
    int sampleStepY;
    int offscreenSlot;
} BattleSummonPhoenixRequest;

/* Phoenix summon particle: position and damped velocity per axis. */
typedef struct BattleSummonPhoenixParticle {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0E[0x2];
    int spriteX;
} BattleSummonPhoenixParticle;

/* RotTransPoint output scratch copied into actor record 3 X/Y/Z position for Phoenix motion. */
typedef struct BattleFxPhoenixRotatePos {
    union {
        int outVec[3];
        struct {
            short posX;
            short padX;
            short posY;
            short padY;
            short posZ;
            short padZ;
        };
    };
    int pad_0C;
    int flag;
} BattleFxPhoenixRotatePos;

/* Transform vector of the Phoenix summon actor and growing model. */
typedef struct {
    short posX;
    short transformOffset;
    short posZ;
    short pad;
} BattleFxPhoenixTransformData;

/* Scratch block used by the Phoenix timeline task for camera and particle velocity transforms. */
typedef struct BattleFxPhoenixTimelineScratch {
    SVECTOR vec;
    union {
        int outVec[3];
        struct {
            short posX;
            short padX;
            short posY;
            short padY;
            short posZ;
            short padZ;
        };
    };
    int pad_14;
    PsxMatrix matrix;
    short pad_36;
    int rotTransFlag;
} BattleFxPhoenixTimelineScratch;

/* Phoenix billboard position: previous and current x/y/z values, shifted each frame. */
typedef struct BattleFxPhoenixBillboardState {
    short prevX;
    short prevY;
    short prevZ;
    unsigned char pad_06[0x2];
    union {
        int posWords[3];
        struct {
            short posX;
            short pad_0A;
            short posY;
            short pad_0E;
            short posZ;
            short pad_12;
        };
    };
    unsigned char pad_14[0x4];
    PsxMatrix matrix;
    unsigned char pad_36[0x2];
    int rotTransFlag;
} BattleFxPhoenixBillboardState;

/* Phoenix animated sprite slot: frame index compared against the animation's frame count. */
typedef struct BattleFxPhoenixAnimSlot {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xA];
    short rotation;
} BattleFxPhoenixAnimSlot;

/* Phoenix falling sprite slot: offset on a circle in X and Z, fixed height, and a negative fall speed. */
typedef struct BattleFxPhoenixFallingSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    short scale;
} BattleFxPhoenixFallingSprite;

/* Phoenix billboard sprite slot: offset on a circle in X and Z, plus a random value and an index. */
typedef struct BattleFxPhoenixBillboardSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short forwardPos;
    unsigned char pad_0A[0x6];
    short forwardStep;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxPhoenixBillboardSprite;

/* Phoenix timeline record: position offset with a fixed height and an angle flipped by half a turn. */
typedef struct BattleFxPhoenixOffset {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short angle;
} BattleFxPhoenixOffset;

/* Euler rotation state for the Phoenix trailed-models task. */
typedef struct BattleFxPhoenixTrailState {
    short rotX;
    short rotY;
    short rotZ;
} BattleFxPhoenixTrailState;

/* Scratch block used by BattleFx_SummonPhoenixCounterSpinModelsTask. */
#pragma pack(push, 1)
typedef struct BattleFxPhoenixCounterSpinScratch {
    union {
        PsxMatrix matrix;
        struct {
            short m[3][3];
            int posX;
            int posY;
            int posZ;
            BattleFxPhoenixTrailState angles;
            short pad_24;
            BattleFxPhoenixCounterSpinState scale;
            int pad_32;
            PsxMatrix viewMatrix;
        };
    };
} BattleFxPhoenixCounterSpinScratch;
#pragma pack(pop)


/* Camera script task record: script parameter, matrix pointer and actor index. */
typedef struct BattleFxPhoenixCameraTask {
    unsigned char pad_00[0xc];
    const void *scriptParam;
    PsxMatrix *matrixPtr;
    short actorIndex;
} BattleFxPhoenixCameraTask;

/* A flat GPU packet of 0x18 bytes from Gfx_GetPsxPacketScratchPtr: its tag word and colour word. */
typedef struct BattleFxPhoenixFlatPacket {
    int tag;
    int color;
    unsigned char pad_08[0x10];
} BattleFxPhoenixFlatPacket;

/* The billboard's scratch: the rotation short, then a point at the next byte (unaligned ints). */
#pragma pack(push, 1)
typedef struct BattleSummonPhoenixBillboardScratch {
    short rotation;
    int x;
    int y;
    int z;
} BattleSummonPhoenixBillboardScratch;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixMeshWorkingBlob {
    SVECTOR ringGrid[0x21][0x10];
    PsxMatrix billboardMatrix;
    short operandTable[1];
} BattleSummonPhoenixMeshWorkingBlob;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixBuffer3 {
    short growStep;
    unsigned char pad_2[0x8];
    int growingPosX;
    int growingPosY;
    int growingPosZ;
    unsigned char pad_16[0x2];
} BattleSummonPhoenixBuffer3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixSpriteRequest2 {
    int spriteValue;
    int fallingTableValue;
    union {
        int fallingFadeColor;
        struct {
            char colorR;
            char colorG;
            char colorB;
            unsigned char primCode;
        };
    };
    unsigned char pad_c[0x4];
} BattleSummonPhoenixSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixSpriteRequest6 {
    short vertexBX;
    unsigned char pad_2[0x6];
    union {
        int spinQuadPoint;
        SVECTOR vertexC;
    };
} BattleSummonPhoenixSpriteRequest6;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixSpriteRequest8 {
    int spriteValue;
    int dampedTableValue;
    union {
        int dampedPoolColor;
        struct {
            char colorR;
            char colorG;
            char colorB;
            unsigned char primCode;
        };
    };
    unsigned char pad_c[0x4];
} BattleSummonPhoenixSpriteRequest8;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixTransformMatrix3 {
    union {
        PsxMatrix matrix;
        struct {
            short m[3][3];
            union {
                int offsetX;
                short offsetXShort;
            };
            int offsetY;
            union {
                int offsetZ;
                short offsetZShort;
            };
        };
    };
} BattleSummonPhoenixTransformMatrix3;
#pragma pack(pop)

/* --- battle_fx_shiva_summon --- */
/* A placed 0x1E-byte matrix of the Shiva effect: the 3x3 rotation, then the translation as three ints. */
#pragma pack(push, 1)
typedef struct BattleFxShivaPlacedMatrix {
    unsigned char rotation[0x12];
    int transX;
    int transY;
    int transZ;
} BattleFxShivaPlacedMatrix;
#pragma pack(pop)

/* The scratch block of a Shiva model draw: the scale vector, then the placed matrix at +0x10. */
typedef struct BattleFxShivaCameraScratch {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    BattleFxShivaPlacedMatrix matrix;
} BattleFxShivaCameraScratch;

/* The scratch block of the burst-piece model draw: the scale vector at +8, the placed matrix at +0x18. */
typedef struct BattleFxShivaRingScratch {
    unsigned char pad_00[0x8];
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    BattleFxShivaPlacedMatrix matrix;
} BattleFxShivaRingScratch;

/* Shiva burst piece record: timer, position and velocity per axis, and the model it drives. */
typedef struct BattleFxShivaBurstPiece {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x2];
    BattleFxShivaPlacedMatrix *model;
    short sample;
} BattleFxShivaBurstPiece;

/* Shiva triangle fragment: model index, frame counter, position and velocity, and matrix pointer. */
typedef struct BattleFxShivaTriangleFragment {
    short fragmentIndex;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x2];
    int matrixPtr;
    unsigned char pad_18[0x2];
    short variant;
} BattleFxShivaTriangleFragment;

/* Force-field strengths per axis applied to particle velocity in the Shiva effect. */
typedef struct BattleFxShivaForceField {
    short forceX;
    short forceY;
    short forceZ;
} BattleFxShivaForceField;

/* Task-slot record for a Shiva shrinking fragment: color, alpha and three random angles. */
typedef struct BattleFxShivaShrinkingFragment {
    unsigned char pad_00[0x4];
    short color;
    short colorOffset;
    short alpha;
    unsigned char pad_0A[0x2];
    short angleA;
    short angleB;
    short angleC;
} BattleFxShivaShrinkingFragment;

/* Render packet for one Shiva triangle model, written into the render state block. */
typedef struct BattleFxShivaTrianglePacket {
    int tag;
    unsigned char pad_04[0x3];
    char commandCode;
    unsigned char pad_08[0x4];
    int uvClutWord;
    unsigned char pad_10[0x4];
    short uvWord1;
    short flags;
    unsigned char pad_18[0x4];
    short uvWord2;
} BattleFxShivaTrianglePacket;

/* Effect-array slot for a Shiva oriented sprite; frameIndex indexes the sprite frames. */
typedef struct BattleFxShivaOrientedSpriteSlot {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x10];
    BattleFxShivaPlacedMatrix *matrix;
    short sample;
} BattleFxShivaOrientedSpriteSlot;

/* Shiva summon target centroid Z history, shifted each frame in BattleFx_SummonShivaTriangleFragmentsTask. */
typedef struct BattleSummonShivaCentroidZ {
    unsigned char pad_00[0x8];
    int zPrev2; /* the scale vector (zPrev2, zPrev, zCurrent) of the triangle model */
    int zPrev;
    int zCurrent;
    unsigned char pad_14[0x4];
    BattleFxShivaPlacedMatrix matrix; /* +0x18 */
    BattleFxShivaPlacedMatrix matrix2; /* +0x36 */
} BattleSummonShivaCentroidZ;

/* Shiva per-triangle model record holding offsets into the vertex arrays, read by BattleFx_SummonShivaSubmitPerTriangleMod */
typedef struct BattleShivaTriangleRecord {
    short vertexPOffset;
    short vertexQOffset;
    short vertexCOffset;
    short flags;
    unsigned char pad_08[0x4];
    int uv0Clut;
    short uv1;
    short uv2;
} BattleShivaTriangleRecord;

/* Spawn record of one Shiva model burst piece. */
typedef struct BattleFxShivaModelBurstRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    int orientationMatrix;
    short scale;
} BattleFxShivaModelBurstRecord;

/* Ring buffer record for the Shiva summon, with centroid-offset position and angle fields. */
typedef struct BattleFxSummonShivaRingRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short colorWord;
} BattleFxSummonShivaRingRecord;

/* Camera-script record that BattleFx_ShivaSpawnCameraScript fills for the Shiva run task. */
typedef struct BattleFxShivaCameraTask {
    unsigned char pad_00[0xc];
    int scriptState;
    int matrixPtr;
    short actorIdx;
} BattleFxShivaCameraTask;

/* GPU packet for one Shiva particle dot, written at g_BattleSummonShivaBufferPtr. */
typedef struct BattleFxShivaParticleDotPacket {
    int tag;
    int colorWord;
    unsigned char pad_08[0x4];
    int dotSize;
} BattleFxShivaParticleDotPacket;

/* Oriented sprite record for the Shiva summon, pointing into the field records. */
typedef struct BattleFxShivaOrientedSprite {
    unsigned char pad_00[0x14];
    BattleFxShivaPlacedMatrix *fieldRecordPtr;
    short scale;
} BattleFxShivaOrientedSprite;

/* Shiva force-field entry: field kind, centre and direction; the list ends at kind -1. */
typedef struct BattleSummonShivaForceField {
    short kind;
    unsigned char pad_02[0x2];
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short dirX;
    short dirY;
    short dirZ;
    unsigned char pad_12[0x2];
} BattleSummonShivaForceField;

/* Approaching-point record of the S-Nova effect; its state gates the point field task. */
typedef struct BattleFxSNovaApproachPoint {
    short state;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
} BattleFxSNovaApproachPoint;

/* Shiva spawn particle record whose three shorts are set to random offsets. */
typedef struct BattleFxShivaParticleRecord2 {
    short spawnOffsetX;
    short spawnOffsetY;
    short spawnOffsetZ;
} BattleFxShivaParticleRecord2;

/* One ice particle record of g_BattleSummonShivaFragmentVertices (0x18 bytes): live while timer is nonzero, with the
   force-field mask, position, velocity and the callback that draws it. */
typedef struct BattleSummonShivaFragmentVertex {
    short forceMask;
    short timer;
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    union {
        Int64Pair vel;
        struct { short velocityX; short velocityY; short velocityZ; unsigned char pad_12[0x2]; };
    };
    void (__cdecl *update)(void *, int, int); /* +0x14: called with the new position, the position record and the timer */
} BattleSummonShivaFragmentVertex;

/* Fade record of the Shiva black model task; holds a scale value set from the fade progress. */
typedef struct BattleSummonShivaFadeRecord {
    unsigned char pad_00[0x8];
    int alpha;
    unsigned char pad_0C[0x6];
    short scale;
} BattleSummonShivaFadeRecord;

/* Triangle fragment record (0x20 bytes) of the Shiva summon. */
typedef struct BattleFxShivaTriangleMotion {
    short posX;   /* +0x00: the triangle's centroid */
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    short velX;   /* +0x08: -normal * 0x60 >> 12, damped each frame */
    short velY;
    short velZ;
    unsigned char pad_0E[0x2];
    short angleX; /* +0x10 */
    short angleY;
    short angleZ;
    unsigned char pad_16[0x2];
    short spinX;  /* +0x18: random spin, rand() % 256 - 128 */
    short spinY;
    short spinZ;
    unsigned char pad_1E[0x2];
} BattleFxShivaTriangleMotion;

/* Shiva timeline fragment index record: colour, colour value and alpha offsets. */
typedef struct BattleFxShivaFragmentIndex {
    short colorOffset;
    short colorValueOffset;
    short alphaOffset;
    unsigned char pad_06[0x2];
    BattleFxShivaPlacedMatrix matrix; /* +0x8: the attached bone's matrix relative to the camera */
} BattleFxShivaFragmentIndex;

/* Shiva fragment entry with byte offsets into its vertex data. */
typedef struct BattleFxShivaFragmentEntry {
    short triangleCnt;
    short vertexQOffset;
    short vertexSOffset;
} BattleFxShivaFragmentEntry;

#pragma pack(push, 1)
typedef struct BattleSummonShivaFragmentColor {
    short fragmentColor; /* +0x0 */
    short colorOffset; /* +0x2 */
} BattleSummonShivaFragmentColor;
#pragma pack(pop)

#pragma pack(push, 1)
/* The camera script's operand table ends inside the last fragment matrix: an operand -k reads
 * the k-th short before cameraValueTable. */
typedef struct BattleSummonShivaSpriteCallbacks {
    struct BattleFxTimelineState *timelineState; /* +0x0 */
    unsigned char pad_4[0x4];
    union {
        PsxMatrix fragmentMatrices[4]; /* +0x8: one per triangle fragment group */
        struct {
            unsigned char pad_8[0x76];
            short cameraValueTable; /* +0x7e */
        };
    };
} BattleSummonShivaSpriteCallbacks;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonShivaTransformMatrix2 {
    BattleFxTriBasisScratch *triangleScratch; /* +0x0 */
    unsigned char pad_4[0x4];
    PsxMatrix matrix; /* +0x8 */
} BattleSummonShivaTransformMatrix2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonShivaColorTable1 {
    unsigned char pad_0[0x20];
    struct BattleFxSpriteGroup *spriteGroup; /* +0x20 */
    int drawObj; /* +0x24 */
} BattleSummonShivaColorTable1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonShivaColorTable2 {
    unsigned char pad_0[0x18];
    int expandingTableValue; /* +0x18 */
    unsigned char pad_1c[0x6];
    short expandingFadeShort; /* +0x22 */
    unsigned char pad_24[0x4];
    int shrinkingTableValue; /* +0x28 */
    unsigned char pad_2c[0x6];
    short shrinkingShort; /* +0x32 */
    unsigned char pad_34[0x4];
    struct BattleFxSpriteGroup *spriteGroup; /* +0x38 */
    int drawObj; /* +0x3c */
} BattleSummonShivaColorTable2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonShivaColorTable3 {
    unsigned char pad_0[0x4];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int burstPieceTableValue; /* +0x8 */
} BattleSummonShivaColorTable3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonShivaForceFieldTable {
    unsigned char pad_0[0x8];
    SVECTOR fragmentDirections[4]; /* +0x8: one per triangle fragment group */
    int triangleTableValue; /* +0x28 */
    unsigned char pad_2c[0x6];
    short triangleFirstShort; /* +0x32 */
    unsigned char pad_34[0x4];
    int triangleModelOffset; /* +0x38 */
    unsigned char pad_3c[0x6];
    short triangleSecondShort; /* +0x42 */
    unsigned char pad_44[0x4];
    int orientedValueFirst; /* +0x48 */
    int orientedValueSecond; /* +0x4c */
} BattleSummonShivaForceFieldTable;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonShivaParticleColor {
    unsigned char pad_0[0x4];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSummonShivaParticleColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonShivaRotXYZ {
    unsigned char pad_0[0x8];
    int cameraShakeVector; /* +0x8 */
    unsigned char pad_c[0x4];
    char byte; /* +0x10 */
    unsigned char pad_11[0x3];
} BattleSummonShivaRotXYZ;
#pragma pack(pop)

/* The tail of the billboard matrix BattleFx_SummonShivaInstallBillboardMatrix installs: m[2][2]
 * (the sprite scale) and the translation, which RotTransPoint writes. */
#pragma pack(push, 1)
typedef struct {
    short scaleZ;
    int transX;
    int transY;
    int transZ;
    unsigned char pad_0E[0x2];
} BattleSummonShivaBillboardTail;
#pragma pack(pop)

/* The last two fragment model addresses (the table starts at g_BattleSummonShivaModelOffset),
 * then the point the target centroid descends from, in Shiva's space. */
typedef struct {
    int fragmentModels[2];
    SVECTOR vector;
} BattleSummonShivaTargetOffset;

/* --- battle_fx_summon_bahamut --- */
/* Random spawn offsets and angles for a Bahamut summon effect task. */
typedef struct BattleFxBahamutSpawnRecord {
    unsigned char pad_00[0x4];
    short spawnX;
    short spawnY;
    short spawnZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_12[0x2];
    short curValue;
    short targetValue;
} BattleFxBahamutSpawnRecord;

/* Task record for the spinning, expanding model of the Bahamut summon. */
typedef struct BattleFxSpinningModelTask {
    short step;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short spinAngle;
    unsigned char pad_10[0x4];
    short expandAmount;
    short expandSpeed;
} BattleFxSpinningModelTask;

/* Bahamut summon fading billboard task: resource fields and a three-step fade history. */
typedef struct BattleFxBahamutFade {
    int resourceId;
    int altResourceId;
    char fadeOld;
    char fadePrev;
    char fadeCurrent;
    char command;
    short frameIndex;
} BattleFxBahamutFade;

/* Bahamut expanding model: scale grows by a step that decays each frame. */
typedef struct BattleFxBahamutExpandingModel {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
    short scaleStep;
} BattleFxBahamutExpandingModel;

/* Streak line record of the Bahamut summon: a frame counter and a position with a per-frame step. */
typedef struct BattleFxBahamutStreakLine {
    short ownerSlot;
    short frameCount;
    short posX;
    short velocityX;
} BattleFxBahamutStreakLine;

/* Rotating model pair of the Bahamut summon: frame counter, accumulated angle and its per-frame step. */
typedef struct BattleFxBahamutRotatingPair {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x2];
    short angle;
    unsigned char pad_08[0x6];
    short angleStep;
} BattleFxBahamutRotatingPair;

/* Effect pool slot for the Bahamut animated sprite; frameIndex selects the table entry. */
typedef struct BattleFxAnimatedSpriteSlot {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x10];
    short scale;
} BattleFxAnimatedSpriteSlot;

/* Effect pool slot for one Bahamut streak line; angle is a random 12-bit value. */
typedef struct BattleFxStreakLineSlot {
    unsigned char pad_00[0x4];
    short startTime;
    short timeStep;
    unsigned char pad_08[0x4];
    short angle;
    short angleY;
    short angleZ;
} BattleFxStreakLineSlot;

/* Effect pool slot for the extending Bahamut beam: position and transform matrix. */
typedef struct BattleFxExtendingBeamSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
    unsigned char pad_16[0x2];
    void *transformMatrix;
} BattleFxExtendingBeamSlot;

/* Effect pool slot for the eased-scale Bahamut beam model; frameIndex steps once per frame. */
typedef struct BattleFxEasedBeamSlot {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x4];
    short travel;
    unsigned char pad_0A[0xe];
    void *transformMatrix;
} BattleFxEasedBeamSlot;

/* Effect task record for the Bahamut fading billboard sprite effect. */
typedef struct BattleFxBahamutFadingSpriteTask {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x10];
    short scale;
} BattleFxBahamutFadingSpriteTask;

/* Effect task record for the Bahamut expanding-model effect; keeps the last three scale values. */
typedef struct BattleFxBahamutExpandingModelTask {
    int prevPrevScale;
    int prevScale;
    int scale;
} BattleFxBahamutExpandingModelTask;

/* Effect task record for the Bahamut extending-beam effect. */
typedef struct BattleFxBahamutExtendingBeamTask {
    short ownerSlot;
    short step;
    unsigned char pad_04[0xc];
    short length;
} BattleFxBahamutExtendingBeamTask;

/* Task-slot record for the Bahamut summon timeline, shared by its expanding and billboard model tasks. */
typedef struct BattleFxBahamutTimelineRecord {
    unsigned char pad_00[0x14];
    short curValue;
    short targetValue;
} BattleFxBahamutTimelineRecord;

/* Screen shake envelope record for the Bahamut summon: amplitude, decay, duration and frequency. */
typedef struct BattleFxBahamutShakeEnvelope {
    unsigned char pad_00[0x4];
    short amplitude;
    short decay;
    short duration;
    short frequency;
} BattleFxBahamutShakeEnvelope;

/* Task-slot record for one Bahamut animated sprite, copied from a source sprite. */
typedef struct BattleFxBahamutAnimatedSpriteRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
} BattleFxBahamutAnimatedSpriteRecord;

/* Camera-script task slot the Bahamut spawn routine fills in. */
typedef struct BattleFxBahamutCameraScriptSlot {
    unsigned char pad_00[0xc];
    int dataA;
    int dataB;
    short mode;
} BattleFxBahamutCameraScriptSlot;

/* Task record for a Bahamut billboard model that grows over time. */
typedef struct BattleFxBahamutBillboardGrowingTask {
    short ownerSlot; /* +0x0 */
    short frameCount;
    unsigned char pad_04[0x10];
    short scale;
} BattleFxBahamutBillboardGrowingTask;

/* Header of the Bahamut animated sprite block, filled with random values. */
typedef struct BattleFxBahamutAnimatedSpriteHeader {
    short offsetX;
    short offsetY;
    short offsetZ;
} BattleFxBahamutAnimatedSpriteHeader;

/* A 30-byte GTE matrix as the Bahamut tasks store it: nine rotation shorts, then the translation
   ints from +0x12 (unaligned, so the packing is 1). */
#pragma pack(push, 1)

/* The data block of the spinning expanding model task: a scale vector at +8, then its matrix. */
typedef struct BattleFxBahamutExpandState {
    unsigned char pad_00[0x8];
    int prevValue;
    int curValue;
    int expandAmount;
    PsxMatrix matrix; /* +0x18 */
} BattleFxBahamutExpandState;

/* The data block of the counter-rotating model pair task: two matrices and a scale vector. */
typedef struct BattleFxBahamutMatrixPairBlock {
    PsxMatrix first;
    PsxMatrix second; /* +0x1E */
    BattleFxFixedScaleVec scale; /* +0x3C */
} BattleFxBahamutMatrixPairBlock;

/* The data block of the expanding model task: a matrix, then its scale history. */
typedef struct BattleFxBahamutMatrixScaleBlock {
    PsxMatrix matrix;
    BattleFxBahamutExpandingModelTask scale; /* +0x1E */
} BattleFxBahamutMatrixScaleBlock;
#pragma pack(pop)

/* The first 16 bytes of a BattleFxModelDraw: the request the Bahamut model tasks fill. */
typedef struct BattleFxBahamutDrawHead {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} BattleFxBahamutDrawHead;

/* Two consecutive draw requests: a full head, then the second request's model group, flags and fade
   (its remaining 4 bytes are a separate array). */
typedef struct BattleFxBahamutDrawPair {
    BattleFxBahamutDrawHead head;
    int modelGroup2;
    int flags2;
    short uvScroll2;
    short fadeLevel2;
} BattleFxBahamutDrawPair;

/* A draw request followed by the matrix and scale vector its task poses it with. */
typedef struct BattleFxBahamutRingDraw {
    BattleFxBahamutDrawHead head;
    PsxMatrix matrix; /* +0x10 */
    short pad_2E;
    BattleFxFixedScaleVec scale; /* +0x30 */
    int pad_3C;
} BattleFxBahamutRingDraw;

/* The scratch the billboard matrix install writes: a scale short, then the transformed point as
   three unaligned ints. */
#pragma pack(push, 1)
typedef struct BattleFxBahamutBillboardWork {
    short value;
    int x;
    int y;
    int z;
    unsigned char pad_0E[0x2];
} BattleFxBahamutBillboardWork;
#pragma pack(pop)

/* --- battle_fx_summon_hades --- */
/* Effect-array record for the Hades radius-clamped mesh: frame count and radius with its step. */
typedef struct BattleFxHadesRadiusRecord {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x8];
    short radius;
    short yaw;
    short yawSpeed;
    short radiusStep;
    short meshRadius;
    short meshRadiusStep;
    short scaleY;
    short scaleYStep;
} BattleFxHadesRadiusRecord;

/* Effect-array record for the Hades palette-cycling model: frame counter, palette offset and cycle length. */
typedef struct BattleFxHadesPaletteRecord {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short angleY;
    unsigned char pad_10[0x2];
    short angleYStep;
    short scale;
    short scaleStep;
    short colorIndex;
    short cycleLength;
} BattleFxHadesPaletteRecord;

/* Hades height-clipped mesh effect slot: elapsed frames, offset, height and their steps. */
typedef struct BattleFxHeightClippedMesh {
    short ownerSlot;
    short elapsed;
    short posX;
    short offset;
    short posZ;
    unsigned char pad_0A[0x4];
    short offsetStep;
    unsigned char pad_10[0x8];
    short height;
    short heightStep;
    short tableIndex;
    short duration;
} BattleFxHeightClippedMesh;

/* Hades two-colour ribbon slot: elapsed frames, ribbon width and per-frame deltas for its quad record. */
typedef struct BattleFxHadesRibbonSlot {
    short ownerSlot;
    short elapsed;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0x4];
    short angle;
    unsigned char pad_10[0x4];
    short ribbonWidth;
    short widthStep;
    short startRadius;
    short stepY;
    short stateIndex;
    short duration;
} BattleFxHadesRibbonSlot;

/* Hades growing radial sprite emitter: center, radius with step, emit count and state index. */
typedef struct BattleFxHadesGrowingEmitterRecord {
    short ownerSlot;
    short timer;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x4];
    short radius;
    unsigned char pad_10[0x2];
    short radiusStep;
    unsigned char pad_14[0x4];
    short emitCount;
    unsigned char pad_1A[0x2];
    short stateIndex;
} BattleFxHadesGrowingEmitterRecord;

/* Effect100 slot for the Hades fading sprite and model pair task. */
typedef struct BattleSummonHadesFadingSpriteRecord {
    short ownerSlot;
    short index;
    unsigned char pad_04[0x10];
    short randomFlagA;
    short randomFlagB;
    short indexOffset;
    short angle;
    short tableIndex;
} BattleSummonHadesFadingSpriteRecord;

/* Effect100 slot initialised by the Hades radius-clamped mesh spawn timeline. */
typedef struct BattleSummonHadesMeshSpawnSlot {
    unsigned char pad_00[0x4];
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short radius;
    short yawOrEmitSpeed;
    short yawSpeed;
    short radiusStep;
    short meshRadius;
    short meshRadiusStep;
    short scaleYOrEmitCount;
    short scaleYStep;
    short stateIndex;
} BattleSummonHadesMeshSpawnSlot;

/* Hades stretching model slot: timer, decaying velocity and offset. */
typedef struct BattleFxHadesStretching {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0xa];
    short angleY;
    unsigned char pad_10[0x4];
    short offset;
    short velocity;
} BattleFxHadesStretching;

/* Hades sprite pair record: phase, random flips, angle and table index. */
typedef struct BattleFxHadesSpritePairRecord {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x10];
    short flipA;
    short flipB;
    short phaseOffset;
    short angle;
    short tableIndex;
} BattleFxHadesSpritePairRecord;

/* Task record for the Hades tube summon: phase, scroll and an index into the Hades state tables. */
typedef struct BattleFxHadesTubeTask {
    short ownerSlot;
    short phase;
    short posX;
    short scrollOffset;
    unsigned char pad_08[0x12];
    short scrollSpeed;
    short stateIndex;
} BattleFxHadesTubeTask;

/* Hades two-colour ribbon spawn timeline record with frame counter and spawn index. */
typedef struct BattleFxHadesRibbonSpawner {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x1a];
    short spawnIndex;
} BattleFxHadesRibbonSpawner;

/* Hades two-colour ribbon segment created by the spawn timeline. */
typedef struct BattleFxHadesRibbonSegment {
    unsigned char pad_00[0x4];
    short initialVelocityX;
    short initialVelocityY;
    short initialVelocityZ;
    unsigned char pad_0A[0x4];
    short angleOffset;
    unsigned char pad_10[0x4];
    short size;
    short halfSize;
    short duration;
    short velocityY;
    short spawnIndex;
    short frameCount;
} BattleFxHadesRibbonSegment;

/* Hades ribbon record: frame index, an accumulated angle and an index into the Hades data table. */
typedef struct BattleFxHadesRibbonTask {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0xa];
    short angle;
    unsigned char pad_10[0xc];
    short ribbonIndex;
} BattleFxHadesRibbonTask;

/* Hades textured-quad draw packet, filled from a draw record and an argument block. */
typedef struct BattleFxHadesTexturedQuad {
    int tag;
    union {
        int shade;
        struct {
            unsigned char shadeRgb[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    short texCoord0;
    short clut;
    unsigned char pad_10[0x4];
    short texCoord1;
    short attr;
    unsigned char pad_18[0x4];
    short texCoord2;
    unsigned char pad_1E[0x6];
    short texCoord3;
} BattleFxHadesTexturedQuad;

/* Per-effect state for the Hades height-clipped mesh spawn timeline, one 32-byte slot. */
typedef struct BattleFxSummonHadesTimeline {
    unsigned char pad_00[0x4];
    short posX;
    short heightTarget;
    short posZ;
    unsigned char pad_0A[0x4];
    short offsetStep;
    unsigned char pad_10[0x8];
    short height;
    short heightStep;
    short index;
    short duration;
} BattleFxSummonHadesTimeline;

/* Draw packet for the Hades height-clipped textured quads, filled from the effect's arg1 record. */
typedef struct BattleFxSummonHadesQuadPacket {
    int packetTag;
    union {
        int colorRgb;
        struct {
            unsigned char colorBytes[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    short texCoord0;
    short clut;
    unsigned char pad_10[0x4];
    short texCoord1;
    short tpage;
    unsigned char pad_18[0x4];
    short texCoord2;
    unsigned char pad_1E[0x6];
    short texCoord3;
} BattleFxSummonHadesQuadPacket;

/* Sprite-pair slot of the Hades summon: a mirrored pair with an oscillation phase and base angle. */
typedef struct BattleFxHadesSpritePairSlot {
    unsigned char pad_00[0xc];
    short oscillation;
    unsigned char pad_0E[0x4];
    short phase;
    short side;
    short otherSide;
    short phaseOffset;
    short baseAngle;
    short pairIndex;
} BattleFxHadesSpritePairSlot;

/* Oscillating model record of the Hades summon: a frame counter, oscillation offset and phase. */
typedef struct BattleFxHadesOscillatingModel {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x8];
    short oscillation;
    unsigned char pad_0E[0x4];
    short phase;
} BattleFxHadesOscillatingModel;

/* Palette-cycling model slot of the Hades summon: a random-walk drift, phase and lifetime. */
typedef struct BattleFxHadesPaletteCycleSlot {
    unsigned char pad_00[0x6];
    short drift;
    unsigned char pad_08[0x6];
    short phase;
    unsigned char pad_10[0x2];
    short lifetime;
    short driftRange;
    short driftStep;
    short colorIndex;
    short cycleLength;
} BattleFxHadesPaletteCycleSlot;

/* Effect pool slot that emits a pair of fading Hades sprite models with random side and angle. */
typedef struct BattleFxFadingPairEmitterSlot {
    unsigned char pad_00[0x14];
    short side;
    short oppositeSide;
    short variant;
    short angle;
    short tableIndex;
} BattleFxFadingPairEmitterSlot;

/* Effect pool slot for the Hades palette-cycling model emitter; frameCount runs to 44. */
typedef struct BattleFxPaletteCycleEmitter {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x10];
    short jitterRange;
} BattleFxPaletteCycleEmitter;

/* Textured-quad draw descriptor for the Hades summon, with vertex offsets and primitive fields. */
typedef struct BattleFxHadesQuadDesc {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
    short texCoord0;
    short clut;
    short texCoord1;
    short texCoord2;
    short texCoord3;
    unsigned short grayShade; /* the low byte is the grey level */
} BattleFxHadesQuadDesc;

/* Hades textured triangle record: three vertex offsets and clamped radius and shape shorts. */
typedef struct BattleFxHadesTriangleRecord {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short grayShade;
    short attrA;
    short clut;
    short attrB;
    short centerValue;
} BattleFxHadesTriangleRecord;

/* Hades Gouraud quad record: four vertex offsets and four byte-masked int values. */
typedef struct BattleFxHadesGouraudQuadRecord {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
    int vertex0Color;
    int vertex1Color;
    int vertex2Color;
    int vertex3Color;
} BattleFxHadesGouraudQuadRecord;

/* Effect task record for the Hades height-clipped mesh spawn timeline. */
typedef struct BattleFxHadesHeightClipTimeline {
    unsigned char pad_00[0x6];
    short centerY;
    unsigned char pad_08[0x6];
    short radius;
    unsigned char pad_10[0x2];
    short radiusStep;
    unsigned char pad_14[0x4];
    short emitCount;
    unsigned char pad_1A[0x2];
    short poolSelector;
} BattleFxHadesHeightClipTimeline;

/* Effect task record for the Hades tube spawn timeline. */
typedef struct BattleFxHadesTubeSpawnTimeline {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x10];
    short scrollSpeed;
    short stateIndex;
} BattleFxHadesTubeSpawnTimeline;

/* Task-slot record for the Hades spawn timeline, compared against an actor position Z value. */
typedef struct BattleFxHadesSpawnRecord {
    short ownerSlot;
    unsigned char pad_02[0x6];
    short posZ;
} BattleFxHadesSpawnRecord;

/* Ring-buffer record for a delayed Hades ring spawn. */
typedef struct BattleFxHadesDelayedRingRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short selector;
} BattleFxHadesDelayedRingRecord;

/* Camera-script task slot the Hades spawn routine fills in. */
typedef struct BattleFxHadesCameraScriptSlot {
    unsigned char pad_00[0xc];
    int param1;
    int param2;
    short flags;
} BattleFxHadesCameraScriptSlot;

/* Task-slot record for the Hades stretching model spawn timeline. */
typedef struct BattleFxHadesStretchingModelTask {
    unsigned char pad_00[0xe];
    short angleY;
    unsigned char pad_10[0x4];
    short scaleX;
    short scaleY;
} BattleFxHadesStretchingModelTask;

/* Rising-ring sprite emitter record for the Hades summon, with a per-frame counter. */
typedef struct BattleFxHadesRisingRing {
    short ownerSlot;
    short counter;
} BattleFxHadesRisingRing;

/* One Hades summon color-cycling sprite emitter record: position, velocity, cycle index and fade state. */
typedef struct BattleFxHadesColorEmitter {
    short active;
    short cycleIndex;
    short posX;
    short posY;
    short posZ;
    short angleStep;
    short velocityX;
    short velocityY;
    short velocityZ;
    short accelY;
    short fadeLevel;
    short fadeValue;
    short fadeRate;
    short toggle;
    unsigned char pad_1C[0x2];
    short angle;
} BattleFxHadesColorEmitter;

/* Particle of the Hades rising-ring sprite emitter: position, velocity, colour index and life. */
typedef struct BattleFxHadesRisingRingParticle {
    short active;
    short age;
    short posX;
    short posY;
    short posZ;
    short lifeStep;
    short velocityX;
    short velocityY;
    short velocityZ;
    short accelY;
    short colorIndex;
    short life;
} BattleFxHadesRisingRingParticle;

/* Hades growing radial sprite particle: active flag, frame, position, and velocity in the emitter array. */
typedef struct BattleFxHadesGrowParticle {
    short active;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x4];
    short radius;
} BattleFxHadesGrowParticle;

/* Hades shrinking radial sprite particle: active flag, frame, position, and velocity in the emitter array. */
typedef struct BattleFxHadesShrinkParticle {
    short active;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x4];
    short size;
} BattleFxHadesShrinkParticle;

/* 8-byte circle point of the Hades tube; its Z value is averaged for draw order. */
typedef struct BattleSummonHadesCirclePoint {
    short vx;
    short vy;
    short vz;
    short depth;
} BattleSummonHadesCirclePoint;

/* Textured PSX triangle for the Hades radius-clamped draw; tag 0x07000000 in its top byte. */
typedef struct BattleFxHadesTexTriPrim {
    int tag;
    union {
        int shade;
        struct {
            unsigned char shadeRgb[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    short attrA;
    short clut;
    unsigned char pad_10[0x4];
    short attrB;
    short texturePage;
    unsigned char pad_18[0x4];
    short centerValue;
} BattleFxHadesTexTriPrim;

/* Textured PSX triangle for the Hades height-clipped draw; tag 0x07000000 in its top byte. */
typedef struct BattleFxHadesClippedTexTriPrim {
    int tagWord;
    union {
        int shade;
        struct {
            unsigned char shadeRgb[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    short texCoord0;
    short clut;
    unsigned char pad_10[0x4];
    short texCoord1;
    short texturePage;
    unsigned char pad_18[0x4];
    short texCoord2;
} BattleFxHadesClippedTexTriPrim;

/* PSX Gouraud quad primitive for the Hades effect, with its colour words zeroed. */
typedef struct BattleFxHadesGouraudQuadPrim {
    int tagWord;
    union {
        int color0;
        CVECTOR color0Vec;
        struct {
            unsigned char color0Rgb[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    union { int color1; CVECTOR color1Vec; };
    unsigned char pad_10[0x4];
    union { int color2; CVECTOR color2Vec; };
    unsigned char pad_18[0x4];
    union { int color3; CVECTOR color3Vec; };
} BattleFxHadesGouraudQuadPrim;

/* Scratch state of the Hades summon's PSX mesh draw (one object per mesh task): the mesh data and
   draw list, the packet cursor, the transform results of the current primitive, up to four
   projected vertices, and the texture bytes interpolated when a radius or height clips it. */
typedef struct BattleFxHadesRingState {
    int meshData;
    int drawList;
    int shift;
    int primPtr;
    union {
        int highWord;
        short texturePage;
    };
    int facing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int flags;
    union {
        struct { short rimAX; short rimAY; short rimAZ; unsigned char pad_2E[0x2]; };
        Int64Pair rimAPair;
        GfxPsxShortVertexSlot rimASlot;
    };
    union {
        struct { short rimBX; short rimBY; short rimBZ; unsigned char pad_36[0x2]; };
        Int64Pair rimBPair;
        GfxPsxShortVertexSlot rimBSlot;
    };
    union {
        struct { short centerX; short centerY; short centerZ; unsigned char pad_3E[0x2]; };
        Int64Pair centerPair;
        GfxPsxShortVertexSlot centerSlot;
    };
    union {
        struct { short vertex3X; short vertex3Y; short vertex3Z; unsigned char pad_46_low[0x2]; };
        Int64Pair vertex3Pair;
        GfxPsxShortVertexSlot vertex3Slot;
    };
    unsigned char pad_48[0x68];
    CVECTOR tempColor;
    unsigned char pad_B4[0x58];
    int rimAOffX;
    int rimAOffY;
    int rimAOffZ;
    unsigned char pad_118[0x4];
    int rimBOffX;
    int rimBOffY;
    int rimBOffZ;
    unsigned char pad_128[0x4];
    union {
        short attrA;
        short texCoord0;
        struct {
            unsigned char texU0;
            unsigned char texV0;
        };
    };
    union {
        short attrB;
        short texCoord1;
        struct {
            unsigned char texU1;
            unsigned char texV1;
        };
    };
    union {
        short centerValue;
        short texCoord2;
        struct {
            unsigned char texU2;
            unsigned char texV2;
        };
    };
    union {
        short texCoord3;
        struct {
            unsigned char texU3;
            unsigned char texV3;
        };
    };
    union { int baseColor; CVECTOR baseColorVec; };
    union {
        int nextColor;
        CVECTOR nextColorVec;
        struct {
            unsigned char colorR;
            unsigned char colorG;
            unsigned char colorB;
        };
    };
} BattleFxHadesRingState;

#pragma pack(push, 1)
typedef struct BattleSummonHadesTimerState {
    int timer; /* +0x0 */
    unsigned char pad_4[0x4];
    PsxMatrix rotMatrix; /* +0x8: the tube's transform */
} BattleSummonHadesTimerState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesTargetState {
    int actorMask; /* +0x0 */
    unsigned char pad_4[0x4];
    PsxMatrix transMatrix; /* +0x8 */
    unsigned char pad_26[0x2];
} BattleSummonHadesTargetState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesRibbonSpawnFrames {
    unsigned char pad_0[0x4];
    int ribbonThreshold; /* +0x4 */
} BattleSummonHadesRibbonSpawnFrames;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesShrinkSpriteParams {
    unsigned char pad_0[0x4];
    short shrinkRadialFlags; /* +0x4 */
    short shrinkRadialSize; /* +0x6 */
} BattleSummonHadesShrinkSpriteParams;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesColorState {
    int tubeColor; /* +0x0: the tube's colour, faded to black at the end */
    int tubeShade; /* +0x4: the tube's shade colour */
    int spriteTexture; /* +0x8: the rising ring sprite's draw request from here */
    int spriteClut; /* +0xc */
    int spriteColor; /* +0x10 */
    short spriteFrame; /* +0x14 */
    short spriteFlags; /* +0x16 */
} BattleSummonHadesColorState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesColorCycleSprite {
    unsigned char pad_0[0x10];
    int colorCycleDrawWord; /* +0x10 */
    int colorCycleLookupB; /* +0x14 */
} BattleSummonHadesColorCycleSprite;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesColorCycleSpriteParams {
    int spriteWord; /* +0x0 */
    short colorCycleNibble; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSummonHadesColorCycleSpriteParams;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesCameraScriptVars {
    unsigned char pad_0[0x7e];
    short operandTable; /* +0x7e */
} BattleSummonHadesCameraScriptVars;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesGrowState {
    short radius; /* +0x0 */
    int growStateWord2; /* +0x2 */
    int growStateWord6; /* +0x6 */
    int growStateWord10; /* +0xa */
} BattleSummonHadesGrowState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesShrinkState {
    short shrinkSize; /* +0x0 */
    int shrinkStateWord2; /* +0x2 */
    int shrinkStateWord6; /* +0x6 */
    int shrinkStateWord10; /* +0xa */
} BattleSummonHadesShrinkState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesColorCycleState {
    short cycleAngle; /* +0x0 */
    int cycleStateWord2; /* +0x2 */
    int cycleStateWord6; /* +0x6 */
    int cycleStateWord10; /* +0xa */
} BattleSummonHadesColorCycleState;
#pragma pack(pop)

/* --- battle_fx_summon_leviathan --- */
/* Effect slot spawned by the Leviathan summon timeline: position and velocity of one summon part. */
typedef struct BattleFxLeviathanPart {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0xa];
    short specialActorId;
    short targetIndex;
} BattleFxLeviathanPart;

/* Effect100 slot record driving the Leviathan summon timeline: frame, particle position and actor index. */
typedef struct BattleFxLeviathanTimeline {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x12];
    short specialActorId;
    short actorIndex;
} BattleFxLeviathanTimeline;

/* Leviathan part model record: part index step, two positions and an accumulating angle. */
typedef struct BattleFxLeviathanPartRecord {
    short ownerSlot;
    short step;
    short posA;
    short posB;
    short angle;
    unsigned char pad_0A[0x6];
    short angleStep;
} BattleFxLeviathanPartRecord;

/* Effect record of the Leviathan model-pair task: frame counter, two accumulators and a step. */
typedef struct BattleFxLeviathanPairRecord {
    short ownerSlot;
    short frameCount;
    short posX;
    unsigned char pad_06[0x2];
    short accumulator;
    unsigned char pad_0A[0x6];
    short step;
} BattleFxLeviathanPairRecord;

/* Four 16-bit values, each divided by 8, that seed Leviathan primitive fragment velocities. */
typedef struct BattleFxLeviathanFragmentSeed {
    short numSeeds;
    short vertex1Offset;
    short vertex2Offset;
    short vertex3Offset;
} BattleFxLeviathanFragmentSeed;

/* Slot record of the Leviathan summon timeline task: special actor id and target index. */
typedef struct BattleFxLeviathanTimelineTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x12];
    short actorId;
    short targetIndex;
} BattleFxLeviathanTimelineTask;

/* Camera-script task record for Leviathan: script, matrix and target index. */
typedef struct BattleFxLeviathanCameraTask {
    unsigned char pad_00[0xc];
    int cameraScript;
    int matrix;
    short targetIndex;
} BattleFxLeviathanCameraTask;

/* Leviathan particle record: a position and a shifted delta, written by the timeline task. */
typedef struct BattleFxLeviathanParticle {
    short state;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_12[0x2];
} BattleFxLeviathanParticle;

/* Colour shift register and sprite state for the Leviathan inward sprite effect. */
typedef struct BattleFxLeviathanSpriteColor {
    unsigned char pad_00[0x8];
    int packedOffsets;
    unsigned char pad_0C[0x4];
    char colorOldest;
    char colorMiddle;
    char colorNewest;
    unsigned char pad_13[0x5];
    int quadState;
    unsigned char pad_1c[0x4];
} BattleFxLeviathanSpriteColor;

/* Eight-byte per-fragment velocity of the Leviathan summon burst model. */
typedef struct BattleFxLeviathanVertexDelta {
    short deltaX;
    short deltaY;
    short deltaZ;
    short spare; /* of the last entry: the origin of the camera script's operand table (negative operands) */
} BattleFxLeviathanVertexDelta;

#pragma pack(push, 1)
typedef struct BattleSummonLeviathan_RotVector2 {
    short angleX; /* +0x0 */
    short angleY; /* +0x2 */
} BattleSummonLeviathan_RotVector2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonLeviathan_TargetMask {
    short targetMask; /* +0x0 */
    unsigned char pad_2[0x6];
    short rotationMatrix[9]; /* +0x8 */
    int transX; /* +0x1a */
    int transY; /* +0x1e */
    int transZ; /* +0x22 */
} BattleSummonLeviathan_TargetMask;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonLeviathan_ModelReq1 {
    unsigned char pad_0[0x8];
    int modelGroup; /* +0x8 */
} BattleSummonLeviathan_ModelReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonLeviathan_ModelReq2 {
    unsigned char pad_0[0x8];
    int halfModelPtr; /* +0x8 */
    int req2Threshold; /* +0xc */
    unsigned char pad_10[0x2];
    short req2Fraction; /* +0x12 */
    unsigned char pad_14[0x4];
    int offsetModelPtr; /* +0x18 */
} BattleSummonLeviathan_ModelReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonLeviathan_ModelReq3 {
    unsigned char pad_0[0x10];
    int spriteResource; /* +0x10 */
    int drawObject; /* +0x14 */
} BattleSummonLeviathan_ModelReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonLeviathan_SpriteConfig {
    unsigned char pad_0[0x8];
    int fieldPtr; /* +0x8 */
    unsigned char pad_c[0x4];
    int pulsingModelPtr; /* +0x10 */
    int pulsingWord1; /* +0x14 */
    unsigned char pad_18[0x2];
    short pulsingLevelWord; /* +0x1a */
} BattleSummonLeviathan_SpriteConfig;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonLeviathan_TimelineData {
    unsigned char pad_0[0xd0];
    int model; /* +0xd0 */
    unsigned char pad_d4[0x6];
    short value; /* +0xda */
} BattleSummonLeviathan_TimelineData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxSummonLeviathanModelPairTaskR {
    unsigned char pad_0[0x12];
    int transX; /* +0x12: the translation after the 3x3 rotation */
    int transY; /* +0x16 */
    int transZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleFxSummonLeviathanModelPairTaskR;
#pragma pack(pop)

/* --- battle_fx_summon_neo_bahamut --- */
/* Rock rotation state in a Neo Bahamut data block: angles, a three-sample value history and the model matrix. */
typedef struct BattleFxShardMeshRotation {
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_06[0x2];
    int valueOld;
    int valuePrev;
    int valueNew;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleFxShardMeshRotation;

/* Battle FX task slot for the Neo Bahamut rock effect: position, velocity and rotation per axis. */
typedef struct BattleFxNeoBahamutRock2Task {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short accelerationX;
    unsigned char pad_16[0x2];
    short accelerationZ;
    unsigned char pad_1A[0x2];
    struct BattleFxRockRotation *rotation;
} BattleFxNeoBahamutRock2Task;

/* Neo Bahamut rock particle: resource index, frame count, and position and velocity per axis. */
typedef struct BattleFxNeoBahamutRock {
    short resourceIndex;       /* +0x00: rand() % 3 picks the resource ids; 0xFFFF retires the slot */
    short frame;               /* +0x02 */
    short posX;                /* +0x04 */
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;           /* +0x0C: added to the position each frame */
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    union {                    /* +0x14 */
        int randomAngles;      /* the spawner seeds the three angles with one rand() & 0xFFFFFF */
        struct {
            unsigned char angleX;
            unsigned char angleY;
            unsigned char angleZ;
            unsigned char pad_17[0x1];
        };
    };
    short scale;               /* +0x18: the uniform scale */
    short alpha;               /* +0x1A */
} BattleFxNeoBahamutRock;

/* Neo Bahamut cloud-2 task record: phase and position copied to the cloud task slot. */
typedef struct BattleFxNeoBahamutCloud2Record {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short cloudValue;
} BattleFxNeoBahamutCloud2Record;

/* Neo Bahamut rock model slot: timer, base X, rising height with decaying speed, and rotation bytes. */
typedef struct BattleFxNeoBahamutRockModel {
    short ownerSlot;
    short timer;
    short baseX;
    short height;
    short baseZ;
    unsigned char pad_0A[0x4];
    short riseSpeed;
    unsigned char pad_10[0x4];
    unsigned char rotX;
    unsigned char rotY;
    unsigned char rotZ;
    unsigned char pad_17[0x1];
    short scale;
} BattleFxNeoBahamutRockModel;

/* Neo Bahamut rock spawn slot: variant, anchor-relative position, velocity and child model parameter. */
typedef struct BattleFxNeoBahamutRock2Spawn {
    short variant;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short dirX;
    short accelerationY;
    short dirZ;
    unsigned char pad_1A[0x2];
    int childModelParam;
} BattleFxNeoBahamutRock2Spawn;

/* Neo Bahamut rise-and-fall slot: actor index, timer, base position of an actor and damped velocity. */
typedef struct BattleFxNeoBahamutRiseFall {
    short actorIndex;
    short timer;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x4];
    short velocityY;
} BattleFxNeoBahamutRiseFall;

/* Battle actor record fields used by the Neo Bahamut rise and fall. */
typedef struct BattleActorRiseFallRecord {
    unsigned char pad_00[0x166];
    short posA;
    short posB;
    short posC;
} BattleActorRiseFallRecord;

/* Task record for a line-light model spawned by the Neo Bahamut summon. */
typedef struct BattleFxLineLightTask {
    short spawnIndex;
    unsigned char pad_02[0x2];
    short offsetX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short offsetY;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x6];
    short intensity;
} BattleFxLineLightTask;

/* Neo Bahamut fragment: position, velocity, rotation and a countdown timer. */
typedef struct BattleFxNeoBahamutFragment {
    short timer;
    short clearedWord;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_1A[0x2]; /* to 0x1c bytes, the step of every pointer through it */
} BattleFxNeoBahamutFragment;

/* Neo Bahamut line light record: step counter and a position copied into the task slot. */
typedef struct BattleFxNeoBahamutLineLight {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX; /* +0xC: rotX, rotY and angle are the vector the line light's matrix is built from */
    short rotY;
    short angle;
    unsigned char pad_12[0x6];
    short intensity;
    short angleStep;
} BattleFxNeoBahamutLineLight;

/* Neo Bahamut ball sprite: frame index, position advanced by velocity, and a billboard scale. */
typedef struct BattleFxNeoBahamutBallSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxNeoBahamutBallSprite;

/* Neo Bahamut thunder sprite task slot: frame counter. */
typedef struct BattleFxNeoBahamutThunderSlot {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
} BattleFxNeoBahamutThunderSlot;

/* Spawn record for the first Neo Bahamut line light, holding an angle and an angle step. */
typedef struct BattleFxNeoBahamutLineSpawn {
    short spawnIndex;
    unsigned char pad_02[0xa];
    short rotX;
    short rotY;
    short angle;
    unsigned char pad_12[0x6];
    short size;
    short angleStep;
} BattleFxNeoBahamutLineSpawn;

/* Spawn record for the second Neo Bahamut line light; most fields are constants set at spawn. */
typedef struct BattleFxNeoBahamutLine2Spawn {
    short spawnIndex;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x6];
    short size;
} BattleFxNeoBahamutLine2Spawn;

/* Timeline record shared by the Neo Bahamut summon children, initialized to constant start values. */
typedef struct BattleFxNeoBahamutTimeline {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short clearedWordA;
    short clearedWordB;
    short clearedWordC;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxNeoBahamutTimeline;

/* Neo Bahamut breath child model: frame counter, position words copied to the child slot, and an angle. */
typedef struct BattleFxNeoBahamutBreathChild {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short angle;
} BattleFxNeoBahamutBreathChild;

/* Neo Bahamut rock model: random variant, position and a random 24-bit value. */
typedef struct BattleFxNeoBahamutRock3Model {
    short variant;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short spawnX;
    unsigned char pad_10[0x4];
    int randomValue;
    short scale;
} BattleFxNeoBahamutRock3Model;

/* Line-light primitive for Neo Bahamut: three colour words, one per vertex. */
typedef struct BattleFxLineLightPrim {
    int command;
    int color0;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
} BattleFxLineLightPrim;

/* Effect pool slot for a Neo Bahamut breath model; angle is a random 12-bit value. */
typedef struct BattleFxNeoBahamutBreathSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short angle;
} BattleFxNeoBahamutBreathSlot;

/* Data block of the Neo Bahamut cloud-1 model: the model matrix follows 16 bytes this TU does not read. */
typedef struct BattleFxNeoBahamutCloud1Slot {
    unsigned char pad_00[0x10];
    PsxMatrix matrix; /* +0x10 */
} BattleFxNeoBahamutCloud1Slot;

/* Working data of the Neo Bahamut line light 3: an init flag, then the model matrix at +0x8. */
#pragma pack(push, 1)
typedef struct BattleFxNeoBahamutLine3Blob {
    char initFlag;
    unsigned char pad_01[0x7];
    PsxMatrix matrix; /* +0x8 */
} BattleFxNeoBahamutLine3Blob;

/* Working data of a Neo Bahamut ball sprite: a position vector and the matrix built from its angles. */
typedef struct BattleFxNeoBahamutBallBlob {
    short vector[3];
    unsigned char pad_06[0x2];
    PsxMatrix matrix; /* +0x8 */
} BattleFxNeoBahamutBallBlob;
#pragma pack(pop)

/* Neo Bahamut summon working state for sky mask and cloud sea, used by BattleFx_SummonNeoBahamut tasks. */
typedef struct BattleSummonNeoBahamutSkyMask {
    unsigned char pad_00[0x70];
    int lineLightResource;
    unsigned char pad_74[0x6];
    short phase;
    unsigned char pad_7C[0x4];
    int cloudSeaResource;
    unsigned char pad_84[0x6];
    short cloudScroll;
    unsigned char pad_8C[0x4];
    int modelRequest;
} BattleSummonNeoBahamutSkyMask;

/* Sky primitive state for Neo Bahamut, zeroed and seeded in BattleFx_SummonEnterNeoBahamut. */
typedef struct BattleFxNeoBahamutSkyPrim {
    int tag;
    union {
        struct {
            char color0R;
            char color0G;
            char color0B;
            char code;
        };
        int color0;
    };
    short vertex0X;
    short vertex0Y;
    union {
        struct {
            char color1R;
            char color1G;
            char color1B;
            char unused1;
        };
        int color1;
    };
    short vertex1X;
    short vertex1Y;
    union {
        struct {
            char color2R;
            char color2G;
            char color2B;
            char unused2;
        };
        int color2;
    };
    short vertex2X;
    short vertex2Y;
    union {
        struct {
            char color3R;
            char color3G;
            char color3B;
            char unused3;
        };
        int color3;
    };
    short vertex3X;
    short vertex3Y;
} BattleFxNeoBahamutSkyPrim;

/* Header of four 16-bit offsets to the vertex tables of the Neo Bahamut fragment set. */
typedef struct BattleFxNeoBahamutFragmentHeader {
    short vertexPOffset;
    short vertexAOffset;
    short vertexBOffset;
    short vertexCOffset;
} BattleFxNeoBahamutFragmentHeader;

/* Light rig angles for the Neo Bahamut summon, written each frame. */
typedef struct BattleSummonLightRigAngle {
    short vectorX; /* the vector the rig transform reads is (vectorX, angleX, angleY) */
    short angleX;
    short angleY;
} BattleSummonLightRigAngle;

/* Task-slot state of the Neo Bahamut cloud-sea model, set to fixed initial values. */
typedef struct BattleFxNeoBahamutCloudSea {
    union {
        struct {
            short rotX;
            short rotY;
            short rotZ;
        };
        SVECTOR rotation;
    };
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleFxNeoBahamutCloudSea;

/* Task-slot record holding four envelope parameters for the Neo Bahamut jitter effect. */
typedef struct BattleFxNeoBahamutJitterRecord {
    unsigned char pad_00[0x4];
    short first;
    short second;
    short third;
    short fourth;
} BattleFxNeoBahamutJitterRecord;

/* Thunder sprite record for the Neo Bahamut summon. */
typedef struct BattleFxNeoBahamutThunderSprite {
    short spawnIndex;
    unsigned char pad_02[0x16];
    short scale;
} BattleFxNeoBahamutThunderSprite;



/* Sky-backdrop primitive for the Neo Bahamut summon: colour bytes and four shorts. */
typedef struct BattleFxNeoBahamutSkyTile {
    int tag;
    union {
        struct {
            char colorR;
            char colorG;
            char colorB;
            char code;
        };
        int color;
    };
    short viewportX;
    short viewportY;
    short viewportWidth;
    short viewportHeight;
} BattleFxNeoBahamutSkyTile;

/* Vertex data for the Neo Bahamut cloud sprite and sky mask. */
typedef struct BattleFxCloud2VertexData {
    short viewportOriginX;
    short viewportOriginY;
    short dataOffset;
    short viewportBottomY;
    int resource;
} BattleFxCloud2VertexData;

/* Neo Bahamut summon parameter record shared by its ball and breath tasks. */
typedef struct BattleFxNeoBahamutParam {
    unsigned char pad_00[0x8];
    int resourceHandle;
    int drawObj;
    unsigned char pad_10[0x2];
    short childFadeLevel;
    unsigned char pad_14[0x4];
    int breathModelGroup;
} BattleFxNeoBahamutParam;

/* Rotation vector of a Neo Bahamut rock2 task, updated each frame. */
typedef struct BattleFxRockRotation {
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_06[0xa];
    short size;
} BattleFxRockRotation;

/* Child model parameter of the Neo Bahamut breath effect. */
typedef struct BattleSummonNeoBahamutBreathParam {
    unsigned char pad_00[0x8];
    int modelResource;
    unsigned char pad_0C[0x6];
    short rampValue;
} BattleSummonNeoBahamutBreathParam;


/* Model draw request of the Neo Bahamut rock summon. */
typedef struct BattleFxNeoBahamutRockRequest {
    unsigned char pad_00[0x8];
    int modelGroup;
    unsigned char pad_0C[0x6];
    short blendFraction;
} BattleFxNeoBahamutRockRequest;

/* Vertex data of the Neo Bahamut sky mask model. */
typedef struct BattleFxNeoBahamutSkyMaskVertex {
    short m00;
    unsigned char pad_02[0x8];
    short translateY;
    unsigned char pad_0C[0x2];
    short maskScale;
} BattleFxNeoBahamutSkyMaskVertex;



/* Output PSX textured triangle packet of the Neo Bahamut mesh draw. */
typedef struct BattleFxPsxTexturedTriPrim {
    int tag;
    union {
        int color;
        CVECTOR colorRgb;
    };
    unsigned char pad_08[0x4];
    union {
        int primWord;
        struct {
            unsigned char texU0;
            unsigned char texV0;
        };
    };
    unsigned char pad_10[0x4];
    union {
        short uvWord0;
        struct {
            unsigned char texU1;
            unsigned char texV1;
        };
    };
    short texturePage;
    unsigned char pad_18[0x4];
    union {
        short uvWord1;
        struct {
            unsigned char texU2;
            unsigned char texV2;
        };
    };
    unsigned char pad_1E[0x6];
    union {
        short uvWord2;
        struct {
            unsigned char texU3;
            unsigned char texV3;
        };
    };
} BattleFxPsxTexturedTriPrim;

/* Output PSX textured quad packet of the Neo Bahamut per-face matrix draw. */
typedef struct BattleFxPsxPerFaceQuadPrim {
    int tag;
    int color;
    unsigned char pad_08[0x4];
    union {
        int colorWord;
        struct {
            unsigned char texU0;
            unsigned char texV0;
        };
    };
    unsigned char pad_10[0x4];
    union {
        short uvWord0;
        struct {
            unsigned char texU1;
            unsigned char texV1;
        };
    };
    short textureFlags;
    unsigned char pad_18[0x4];
    union {
        short uvWord1;
        struct {
            unsigned char texU2;
            unsigned char texV2;
        };
    };
    unsigned char pad_1E[0x6];
    union {
        short offsetC;
        struct {
            unsigned char texU3;
            unsigned char texV3;
        };
    };
} BattleFxPsxPerFaceQuadPrim;

/* Task slot for the Neo Bahamut cloud2 model: phase step and a three-value history of a source value. */
typedef struct BattleSummonCloudSlot {
    union {
        struct {
            short angleX;
            short angleY;
            short phaseStep;
        };
        SVECTOR rotation;
    };
    int valueOld;
    int valueMid;
    int valueLatest;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleSummonCloudSlot;


/* Neo Bahamut line light slot: interpolated values and a screen depth for the line primitive. */
typedef struct BattleFxNeoBahamutLineLight1Slot {
    int blendedValue;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix; /* +0x10 */
    short screenX;
    short screenY;
    unsigned char pad_32[0x2];
    int depth;
    unsigned char pad_38[0x8];
    int geomOffsetX;
    int geomOffsetY;
} BattleFxNeoBahamutLineLight1Slot;

/* State of the Neo Bahamut line-light-2 primitive task: lerped value and projected depth. */
typedef struct BattleFxLineLight2Task {
    int lerpedValue;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix; /* +0x10 */
    short screenX;
    short screenY;
    unsigned char pad_32[0x2];
    int projectedDepth;
    unsigned char pad_38[0x8];
    int geomOffsetX;
    int geomOffsetY;
} BattleFxLineLight2Task;

/* Neo Bahamut rock2 state: previous, current and target values, copied from a rotation record. */
typedef struct BattleFxNeoBahamutRock2State {
    unsigned char pad_00[0x8];
    int prevValue;
    int curValue;
    int targetValue;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleFxNeoBahamutRock2State;

#pragma pack(push, 1)
/* Neo Bahamut back sky task state for the sky primitives. */
typedef struct BattleFxNeoBahamutBackSkyState {
    PsxMatrix matrix; /* +0x0 */
    int scaleX; /* +0x1E */
    int scaleY;
    int scaleZ;
    unsigned char pad_2A[0x4];
    short projectedX1; /* +0x2E: the projected corners are (X, Y) short pairs */
    short projectedY1;
    short projectedX2;
    short projectedY2;
    short projectedX3;
    short projectedY3;
    unsigned char pad_3A[0x2];
    int unusedOut;
    int projectFlags;
} BattleFxNeoBahamutBackSkyState;
#pragma pack(pop)

/* Neo Bahamut line light 3 task slot record. */
typedef struct BattleFxNeoBahamutLineLight3Slot {
    int intensity;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix; /* +0x10 */
} BattleFxNeoBahamutLineLight3Slot;

/* Scratch of the breath child model task: its uniform scale vector and its model matrix. */
typedef struct BattleFxNeoBahamutBreathScratch {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix;
} BattleFxNeoBahamutBreathScratch;

/* Neo Bahamut summon mesh record: vertex slot offsets, tint bytes and primitive words. */
typedef struct BattleSummonBahamutMeshRecord {
    short streamCnt;
    short vertexBOffset;
    short vertexCOffset;
    short tintA;
    int primWord;
    union {
        struct {
            short uvWord0;
            short uvWord1;
        };
        int color1; /* the second vertex colour of a gouraud polygon */
    };
    union {
        struct {
            short vertexAOffset;
            short tintB;
        };
        int color2; /* the third vertex colour of a gouraud polygon */
    };
    union {
        struct {
            short nextVertexOffset0;
            short nextVertexOffset1;
        };
        int color3; /* the fourth vertex colour of a gouraud quad */
    };
    short nextVertexOffset2;
    short nextVertexOffset3;
    short nextVertexOffset4;
} BattleSummonBahamutMeshRecord;


/* Sky mask vertex data record written by BattleFx_SummonNeoBahamutSkyMaskModelTask. */
typedef struct BattleFxSkyMaskVertexData {
    short m00;
    unsigned char pad_02[0x8];
    short translateY;
    unsigned char pad_0C[0x2];
    short scale;
} BattleFxSkyMaskVertexData;

/* --- battle_fx_summon_odin_gungnir --- */
/* Odin and Gungnir effect task record, one per spawned task slot. */
typedef struct BattleFxOdinGungnirRecord {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short sinOffset;
    short phase;
    short rotZOrCosOffset;
    unsigned char pad_12[0x2];
    short scale;
    short duration;
} BattleFxOdinGungnirRecord;

/* Render-state block from Gfx_GetPsxPacketScratchPtr with fade colour bytes and colour copies. */
typedef struct BattleFxOdinGungnirFadeQuadPrim {
    int tag;
    int color0;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    char colorRCopy;
    char colorGCopy;
    char colorBCopy;
    unsigned char pad_17[0x5];
    char colorR;
    char colorG;
    char colorB;
} BattleFxOdinGungnirFadeQuadPrim;

/* Odin/Gungnir animated billboard sprite: position, decaying velocity and sprite index. */
typedef struct BattleFxOdinSpriteRecord {
    short ownerSlot;
    short spriteIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x2];
    short angle;
} BattleFxOdinSpriteRecord;

/* Task record for the fading Gouraud triangle of the Odin Gungnir summon. */
typedef struct BattleFxOdinGungnirTriangleTask {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotationX; /* +0x0C: the triangle's rotation */
    short rotationY;
    short rotationZ;
    unsigned char pad_12[0x2];
    short scale;
    short spread;
} BattleFxOdinGungnirTriangleTask;

/* Odin Gungnir trail scratch block (data block zero): the model pair's rotation, its scale vector
   (which RotTransPoint then reuses for each transformed trail endpoint), the composed matrix, and
   the outputs of the trail quad's projection. */
typedef struct BattleFxOdinTrailState {
    short rotationX;
    short rotationY;
    short rotationZ;
    unsigned char pad_06[0x2];
    int pointX; /* +0x08 */
    int pointY;
    int pointZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
    unsigned char pad_36[0x2];
    int depth;  /* +0x38: the trail quad's depth */
    int interp; /* +0x3C */
    int flags;  /* +0x40: projection flags (0x60000: clipped) */
} BattleFxOdinTrailState;

/* Odin Gungnir fading Gouraud quad record and its task slot. */
typedef struct BattleFxOdinGungnirFadeQuad {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotationX; /* +0x0C: the quad's rotation */
    short rotationY;
    short rotationZ;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxOdinGungnirFadeQuad;

/* Pooled Odin Gungnir sprite (fading, falling or damped): a position set when it is spawned. */
typedef struct BattleFxOdinGungnirPooledSprite {
    short cycleIndex;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
} BattleFxOdinGungnirPooledSprite;

/* Growing Odin Gungnir model record: frame index, position, a rotating angle and a scale factor. */
typedef struct BattleFxOdinGungnirGrowingModel {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotationX; /* +0x0C: the model's rotation; angle is its Y */
    short angle;
    unsigned char pad_10[0x4];
    short scaleFactor;
} BattleFxOdinGungnirGrowingModel;

/* Gouraud triangle render packet for the fading Odin Gungnir effect, with per-channel colors. */
typedef struct BattleFxOdinGungnirGouraudTriangle {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char polyCommand;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
} BattleFxOdinGungnirGouraudTriangle;

/* Task-slot record for the Odin alternate camera: frame counter, start and end values, duration. */
typedef struct BattleFxOdinAltLerpTask {
    short ownerSlot;
    short frame;
    short startValue;
    short endValue;
    short duration;
} BattleFxOdinAltLerpTask;

/* Pool particle of the Odin Gungnir summon: a position with a random spread. */
typedef struct BattleFxOdinGungnirPoolParticle {
    short state;
    short clearedWord;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
} BattleFxOdinGungnirPoolParticle;

/* Effect pool slot that spawns pooled fading Odin sprites; cycleIndex cycles through three. */
typedef struct BattleFxPooledSpriteSpawner {
    short ownerSlot;
    short cycleIndex;
} BattleFxPooledSpriteSpawner;

/* Effect pool slot for the Odin flash overlay; steps through a timing schedule. */
typedef struct BattleFxFlashOverlaySlot {
    short scheduleIndex;
    short timer;
} BattleFxFlashOverlaySlot;

/* Falling sprite task for the Odin Gungnir summon: a frame counter and a fall speed. */
typedef struct BattleOdinFallingSprite {
    short ownerSlot; /* +0x0 */
    short frame;
    short posX;
    short fallSpeed;
    short posY;
} BattleOdinFallingSprite;

/* Spawn centre of the Odin Gungnir falling sprites; actor deltas are taken before spawning. */
typedef struct BattleFxOdinSpawnOrigin {
    short actorDeltaX;
    short actorDeltaY;
    short actorDeltaZ;
    unsigned char pad_06[0x2];
    int centerX; /* +0x08: the rotated delta */
    int centerY;
    int centerZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18: the transposed scene matrix */
} BattleFxOdinSpawnOrigin;

/* Screen shake envelope record for the Odin Gungnir summon: amplitude, decay, frequency and duration. */
typedef struct BattleFxOdinGungnirShakeEnvelope {
    unsigned char pad_00[0x4];
    short amplitude;
    short decay;
    short frequency;
    short duration;
} BattleFxOdinGungnirShakeEnvelope;

/* Draw-state words written to the render state block for the Odin Gungnir particle pool. */
typedef struct BattleFxOdinGungnirRenderStatePacket {
    int tag;
    int polyCommand;
    unsigned char pad_08[0x4];
    int colorWord;
} BattleFxOdinGungnirRenderStatePacket;

/* Fading sprite record for the Odin Gungnir summon, one per spawned sprite. */
typedef struct BattleOdinGungnirSprite {
    short phase;
    short rotationY;
    short rotationZ;
    unsigned char pad_06[0x2];
    short fadeLevel;
    short timer;
} BattleOdinGungnirSprite;

/* Sprite draw request for the Odin Gungnir billboard and scale tasks: the pulsing model's scale
   matrix, then the growing and ramp models' draw records. */
typedef struct BattleSummonOdinGSpriteDrawRequest {
    short pulseScale[3][3]; /* +0x00: the pulsing task writes one scale to the diagonal */
    unsigned char pad_12[0xE];
    int growingModelResource;
    unsigned char pad_24[0x6];
    short scale;
    unsigned char pad_2C[0x4];
    int rampModelResource;
} BattleSummonOdinGSpriteDrawRequest;

/* One bank of the Odin trail: the two transformed endpoints of the spear (screen X and Y, depth). */
typedef struct BattleFxOdinTrailSlot {
    SVECTOR start;
    SVECTOR end;
} BattleFxOdinTrailSlot;

/* Fade state with two zeroed shorts and one short copied from the matrix buffer. */
typedef struct BattleFxOdinGungnirFadeState {
    short vectorX;
    short fadeLevel;
    short vectorZ;
    unsigned char pad_06[0x2];
    PsxMatrix matrix; /* +0x08: the sprite's rotation */
} BattleFxOdinGungnirFadeState;

/* One 0x10-byte primitive row of the Odin G flash overlay, with its colour bytes. */
typedef struct BattleSummonOdinGFlashA {
    unsigned char pad_00[0x4];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x9];
} BattleSummonOdinGFlashA;

/* Odin Gungnir pool particle record; 0xFFFF marks a free slot. */
typedef struct BattleSummonOdinGungnirParticle {
    short state;
    short aux;
    unsigned char pad_04[0x10];
} BattleSummonOdinGungnirParticle;

/* Odin Gungnir effect animation data, filled from globals and the resource table. */
typedef struct BattleFxOdinGungnirAnimData {
    unsigned char pad_00[0x8];
    short viewportOriginX;
    short viewportOriginY;
    short viewportWidth;
    short viewportHeight;
    int modelGroup;
} BattleFxOdinGungnirAnimData;

#pragma pack(push, 1)
typedef struct BattleSummonOdinGVectorD4EC {
    short actorIndex; /* +0x0 */
    short boneIndex; /* +0x2 */
} BattleSummonOdinGVectorD4EC;
#pragma pack(pop)

/* Scratch block of the quad and triangle flash tasks: packed matrix, scale vector, projected depth and clip flags. */
#pragma pack(push, 1)
typedef struct BattleFxOdinGungnirScratchBlock {
    PsxMatrix matrix;
    BattleFxFixedScaleVec scale; /* +0x1E */
    unsigned char pad_2A[0x6];
    int depth; /* +0x30 */
    int projectedA; /* +0x34 */
    int clipFlags; /* +0x38 */
} BattleFxOdinGungnirScratchBlock;
#pragma pack(pop)

/* Placement point of the Odin Gungnir sprites: a state word, then the transformed position ints. */
#pragma pack(push, 1)
typedef struct BattleFxOdinGungnirPlacedPoint {
    short stateWord;
    int posX;
    int posY;
    int posZ;
} BattleFxOdinGungnirPlacedPoint;
#pragma pack(pop)

/* --- battle_fx_summon_odin_zantetsuken --- */
/* The target sequence's scratch block (Gfx_GetPsxScratchpadPtr(0)): rotation angles and the matrix they build. */
typedef struct BattleFxOdinZTargetScratch {
    short angle[3];
    unsigned char pad_06[0x2];
    PsxMatrix matrix;
} BattleFxOdinZTargetScratch;

/* The actor motion task's scratch block (Gfx_GetPsxScratchpadPtr(0)): the rotated offset and the matrix it composes. */
typedef struct BattleFxOdinZMotionScratch {
    Vec3sPacked pos;
    unsigned char pad_06[0x12];
    PsxMatrix matrix;
} BattleFxOdinZMotionScratch;

/* Odin Zantetsuken trail spawner state: record index and segments spawned. */
typedef struct BattleFxOdinZTrailSpawner {
    short recordIndex;
    short segmentCount;
} BattleFxOdinZTrailSpawner;

/* Per-task state of the Odin/Zantetsuken target sequence in a 32-byte effect slot. */
typedef struct BattleFxOdinTargetSeq {
    short bitIndex;
    short phase;
} BattleFxOdinTargetSeq;

/* Odin Zantetsuken trail segment slot: step counter and the quad vertex data it transforms. */
typedef struct BattleFxOdinZantetsukenSegment {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x14];
    struct BattleSummonOdinZRecordBuffer1Entry *record;
} BattleFxOdinZantetsukenSegment;

/* Odin Zantetsuken trail task: interpolated short coordinates and their fixed-point sources. */
typedef struct BattleFxOdinTrailSegment {
    union {
        struct {
            short prevX;
            short prevY;
            short prevZ;
            unsigned char pad_06[0x2];
        };
        SVECTOR prev;
    };
    union {
        struct {
            short prevX2;
            short prevY2;
            short prevZ2;
            unsigned char pad_0E[0x2];
        };
        SVECTOR prev2;
    };
    union {
        struct { short currentX; short currentY; short currentZ; };
        Vec3sPacked current;
    };
    unsigned char pad_16[0x2];
    union {
        struct { short currentX2; short currentY2; short currentZ2; };
        Vec3sPacked current2;
    };
    unsigned char pad_1E[0x2];
    union {
        struct { short trailX; short trailY; short trailZ; };
        Vec3sPacked trail;
    };
    unsigned char pad_26[0x2];
    union {
        int sourceX;
        short sourceXShort;
    };
    union {
        int sourceY;
        short sourceYShort;
    };
    union {
        int sourceZ;
        short sourceZShort;
    };
    unsigned char pad_34[0x4];
    union {
        int sourceX2;
        short sourceX2Short;
    };
    union {
        int sourceY2;
        short sourceY2Short;
    };
    union {
        int sourceZ2;
        short sourceZ2Short;
    };
    unsigned char pad_44[0x4];
    PsxMatrix matrix;
    unsigned char pad_66[0x2];
    int flags;
} BattleFxOdinTrailSegment;

/* Odin Zantetsuken actor motion record: frame index and an angular velocity accumulated per frame. */
typedef struct BattleFxOdinZantetsukenMotion {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0xa];
    short angularVelocity;
} BattleFxOdinZantetsukenMotion;

/* Afterimage record of the Zantetsuken summon, stepped by a frame counter. */
typedef struct BattleFxZantetsukenAfterimage {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x10];
    PsxMatrix *matrix;
} BattleFxZantetsukenAfterimage;

/* Odin Zantetsuken split actor task: actor index, frame counter and the actor's screen position. */
typedef struct BattleFxOdinZantetsukenSplitActor {
    union {
        short actorIndex;
        char actorId;
    };
    short frameIndex;
    BattleFxActorOffscreenRequest screen;
} BattleFxOdinZantetsukenSplitActor;

/* Effect pool slot that spawns Odin Zantetsuken afterimages; recordIndex selects a record. */
typedef struct BattleFxAfterimageSlot {
    short ownerSlot;
    short recordIndex;
} BattleFxAfterimageSlot;

/* GPU trail-quad packet for Odin Zantetsuken: a tag word and colour words. */
typedef struct BattleFxTrailQuadPacket {
    int tag;
    int colorWord;
    unsigned char pad_08[0x4];
    int colorCommand;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxTrailQuadPacket;

/* Effect pool slot for an Odin Zantetsuken static sprite; spawnIndex counts up. */
typedef struct BattleFxStaticSpriteSlot {
    short step;
    short spawnIndex;
    short posX;
    short posY;
    short posZ;
} BattleFxStaticSpriteSlot;

/* Effect task record for a Zantetsuken bone sprite: world position and scale. */
typedef struct BattleFxZantetsukenBoneSpriteTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x12];
    short scale;
} BattleFxZantetsukenBoneSpriteTask;

/* Effect task record for the Zantetsuken animated sprite effect. */
typedef struct BattleFxZantetsukenAnimatedSpriteTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x12];
    short scale;
} BattleFxZantetsukenAnimatedSpriteTask;

/* Ring-buffer record for the Odin Zantetsuken actor motion, with Y rotation. */
typedef struct BattleFxOdinZantetsukenMotionRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short rotationY;
} BattleFxOdinZantetsukenMotionRecord;

/* Task slot that spawns Zantetsuken trail segments, pointing at a recorded trail entry. */
typedef struct BattleFxOdinZantetsukenTrailSpawnTask {
    short endIndex;
    short frameCounter;
    unsigned char pad_04[0x14];
    struct BattleSummonOdinZRecordBuffer1Entry *record;
} BattleFxOdinZantetsukenTrailSpawnTask;

/* Timeline task slot record for the Odin Zantetsuken summon, taken from the task slot buffer. */
typedef struct BattleFxOdinSpriteSpawnSlot {
    short bitIndex;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
} BattleFxOdinSpriteSpawnSlot;

/* A textured, flat-coloured GPU quad packet (0x28 bytes): tag, colour, then x/y and u/v for each corner. */
typedef struct BattleFxOdinZSplitPacket {
    int tag;
    int color;
    short x0;
    short y0;
    unsigned char u0;
    unsigned char v0;
    short clut;
    short x1;
    short y1;
    unsigned char u1;
    unsigned char v1;
    short tpage;
    short x2;
    short y2;
    unsigned char u2;
    unsigned char v2;
    unsigned char pad_1e[0x2];
    short x3;
    short y3;
    unsigned char u3;
    unsigned char v3;
    unsigned char pad_26[0x2];
} BattleFxOdinZSplitPacket;

/* One half of the split actor: the four corner coordinates it moves from, in the packet's corner order. */
typedef struct BattleSummonOdinZSplitHalf {
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    short x3;
    short y3;
} BattleSummonOdinZSplitHalf;

/* One record of g_BattleSummonOdinZRecordBuffer1 (0x10 bytes). */
typedef struct BattleSummonOdinZRecordBuffer1Entry {
    union {
        struct {
            short posX; /*+0x00 */
            short posY; /*+0x02 */
            short posZ; /*+0x04 */
            unsigned char pad_06[0x2];
        };
        SVECTOR pos;
    };
    union {
        struct {
            short trailX; /*+0x08 */
            short trailY; /*+0x0A */
            short trailZ; /*+0x0C */
            unsigned char pad_0E[0x2];
        };
        SVECTOR trail;
    };
} BattleSummonOdinZRecordBuffer1Entry;

/* GPU textured quad packet written by BattleFx_SummonOdinZantetsukenDrawPsxMesh. */
typedef struct BattleFxOdinQuadPrim {
    int tag;
    union {
        int colorCmd;
        struct {
            unsigned char pad_04[0x3];
            char primCode;
        };
    };
    unsigned char pad_08[0x4];
    int xy0;
    unsigned char pad_10[0x4];
    short xy1;
    short texPage;
    unsigned char pad_18[0x4];
    short xy2;
    unsigned char pad_1E[0x6];
    short xy3;
} BattleFxOdinQuadPrim;

/* PSX Gouraud mesh packet for the Odin Zantetsuken effect, filled from its mesh header. */
typedef struct BattleFxZantetsukenQuadPacket {
    int tag;
    union {
        int color0;
        CVECTOR color0Vec;
        struct {
            unsigned char pad_04[0x3];
            char primCode;
        };
    };
    unsigned char pad_08[0x4];
    union { int texWord; CVECTOR texWordVec; };
    unsigned char pad_10[0x4];
    union { int attrWord; CVECTOR attrWordVec; };
    unsigned char pad_18[0x4];
    union { int color3; CVECTOR color3Vec; };
} BattleFxZantetsukenQuadPacket;

/* Odin Zantetsuken PSX quad packet: tag word and words copied from the record. */
typedef struct BattleFxOdinQuadPacket {
    int tag;
    union {
        int colorCmd;
        CVECTOR colorCmdVec;
        struct {
            unsigned char pad_04[0x3];
            char primCode;
        };
    };
    unsigned char pad_08[0x4];
    union { int color1; CVECTOR color1Vec; };
    unsigned char pad_10[0x4];
    union { int color2; CVECTOR color2Vec; };
    unsigned char pad_18[0x4];
    union { int color3; CVECTOR color3Vec; };
} BattleFxOdinQuadPacket;

/* The primitive records of a mesh BattleFx_SummonOdinZantetsukenDrawPsxMesh draws: vertex
   offsets into the mesh, texture coordinates (textured) or colours (gouraud). */
typedef struct BattleFxOdinMeshTexTri {
    short vertexA;
    short vertexB;
    short vertexC;
    short colorIndex;
    int uv0;
    short uv1;
    short uv2;
} BattleFxOdinMeshTexTri;

typedef struct BattleFxOdinMeshTexQuad {
    short vertexA;
    short vertexB;
    short vertexC;
    short vertexD;
    int uv0;
    short uv1;
    short uv2;
    short uv3;
    short colorIndex;
} BattleFxOdinMeshTexQuad;

typedef struct BattleFxOdinMeshGouraudTri {
    short vertexA;
    short vertexB;
    short vertexC;
    short pad_06;
    union { int color0; CVECTOR color0Vec; };
    union { int color1; CVECTOR color1Vec; };
    union { int color2; CVECTOR color2Vec; };
} BattleFxOdinMeshGouraudTri;

typedef struct BattleFxOdinMeshGouraudQuad {
    short vertexA;
    short vertexB;
    short vertexC;
    short vertexD;
    union { int color0; CVECTOR color0Vec; };
    union { int color1; CVECTOR color1Vec; };
    union { int color2; CVECTOR color2Vec; };
    union { int color3; CVECTOR color3Vec; };
} BattleFxOdinMeshGouraudQuad;

/* A position in the mesh's record stream: a section's count or texture page, or one record. */
typedef union BattleFxOdinMeshPrim {
    short count;
    short texPage;
    BattleFxOdinMeshTexTri texTri;
    BattleFxOdinMeshTexTri texTris[2];
    BattleFxOdinMeshTexQuad texQuad;
    BattleFxOdinMeshTexQuad texQuads[2];
    BattleFxOdinMeshGouraudTri gouraudTri;
    BattleFxOdinMeshGouraudTri gouraudTris[2];
    BattleFxOdinMeshGouraudQuad gouraudQuad;
    BattleFxOdinMeshGouraudQuad gouraudQuads[2];
} BattleFxOdinMeshPrim;

#pragma pack(push, 1)
typedef struct BattleSummonOdinZAnimDef {
    unsigned char pad_0[0xa];
    short shade; /* +0xa */
} BattleSummonOdinZAnimDef;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinZColorData {
    unsigned char pad_0[0x4];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSummonOdinZColorData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinZRefVector {
    int resourceField; /* +0x0 */
    int drawObj; /* +0x4 */
    union {
        int colorWord; /* +0x8 */
        CVECTOR color;
    };
    unsigned char pad_c[0x4];
} BattleSummonOdinZRefVector;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinZVector2 {
    int resourceField; /* +0x0 */
    int drawObj; /* +0x4 */
    union {
        int colorWord; /* +0x8 */
        CVECTOR color;
    };
    unsigned char pad_c[0x4];
} BattleSummonOdinZVector2;
#pragma pack(pop)

/* --- battle_fx_summon_ramuh --- */
/* PSX GPU sprite packet written into the render state block for the Ramuh summon sprites. */
typedef struct BattleFxRamuhSpritePacket {
    int packetTag;
    int colorWord;
    unsigned char pad_08[0x4];
    char packetU0;
    char packetV0;
    short clut;
    unsigned char pad_10[0x4];
    char packetU1;
    char v0;
    short tpage;
    unsigned char pad_18[0x4];
    char u0;
    char packetV1;
    unsigned char pad_1E[0x6];
    char u1;
    char v1;
} BattleFxRamuhSpritePacket;

/* Effect slot of the Ramuh tumbling model task: position, velocity and rotation bytes. */
typedef struct BattleFxRamuhTumbleSlot {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    unsigned char rotX;
    unsigned char rotY;
    unsigned char rotZ;
    unsigned char pad_17[0x1];
    char rotVelX;
    char rotVelY;
    char rotVelZ;
    unsigned char pad_1B[0x1];
    short historyNewest;
} BattleFxRamuhTumbleSlot;

/* Effect slot record that the Ramuh timeline seeds with an origin vector and a frame count. */
typedef struct BattleFxRamuhTimelineRecord {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
} BattleFxRamuhTimelineRecord;

/* Ramuh sprite record: current, start, end and target coordinates, each scaled by 8. */
typedef struct BattleFxRamuhSpriteRecord {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    short targetX;
    short startY;
    short vertex1Z;
    unsigned char pad_0E[0x2];
    short startX;
    short targetY;
    short vertex2Z;
    unsigned char pad_16[0x2];
    short endX;
    short endY;
    short vertex3Z;
} BattleFxRamuhSpriteRecord;

/* Effect100 slot record for a Ramuh morphing-mesh particle: position, velocity, spin and scale. */
typedef struct BattleFxRamuhMorphParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    char spinX;
    char spinY;
    char spinZ;
    unsigned char pad_1B[0x1];
    short scale;
} BattleFxRamuhMorphParticle;

/* Ramuh morphing mesh record: sprite table index and current sprite frame. */
typedef struct BattleFxRamuhMorphRecord {
    short tableIndex;
    short spriteFrame;
} BattleFxRamuhMorphRecord;

/* Ramuh sprite block: entry count, origin and texture coordinates, then sprite records. */
typedef struct BattleFxRamuhSpriteBlock {
    short flags;
    short entryCount;
    short posX;
    short posY;
    short texU;
    short texV;
    short tpage;
    short clut;
    short widthPair;
    short heightPair;
} BattleFxRamuhSpriteBlock;

/* Ramuh burst child particle: position and per-axis velocity. */
typedef struct BattleFxRamuhBurstChild {
    unsigned char pad_00[0x2];
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
} BattleFxRamuhBurstChild;

/* Ramuh angled sprite: frame counter, position words copied to the sprite colour block, and a short value. */
typedef struct BattleFxRamuhAngledSprite {
    short ownerSlot; /* +0x0 */
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x12];
    short color;
} BattleFxRamuhAngledSprite;

/* Effect-array slot that shakes the camera for Ramuh; shakeOffset is the offset applied each frame. */
typedef struct BattleFxRamuhCameraShakeSlot {
    short shakeOffset;
    short frameCount;
} BattleFxRamuhCameraShakeSlot;

/* Effect-array slot for one of the eight Ramuh models, with random offsets set at spawn. */
typedef struct BattleFxRamuhModelSlot {
    unsigned char pad_00[0x4];
    short clearedWordA;
    short clearedWordB;
    short clearedWordC;
    unsigned char pad_0A[0x2];
    short spinRate;
    short angle;
    short angleZ;
} BattleFxRamuhModelSlot;

/* Slot record of the Ramuh quad billboard task. */
typedef struct BattleFxRamuhQuadBillboardTask {
    short ownerSlot;
    short spriteIndex;
    unsigned char pad_04[0x2];
    short cameraAngleY;
} BattleFxRamuhQuadBillboardTask;

/* Slot record of the Ramuh billboard sprite task. */
typedef struct BattleFxRamuhBillboardSpriteTask {
    short ownerSlot;
    short spriteIndex;
    unsigned char pad_04[0x18];
    short scale;
} BattleFxRamuhBillboardSpriteTask;

/* Slot record of the Ramuh scaled sprite set billboard task. */
typedef struct BattleFxRamuhScaledSpriteTask {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0xa];
    short angle;
} BattleFxRamuhScaledSpriteTask;

/* Ring buffer record for the Ramuh summon, with transform and angle fields. */
typedef struct BattleFxSummonRamuhRingRecord {
    unsigned char pad_00[0x4];
    short transX;
    short angleY;
    short transZ;
    unsigned char pad_0A[0x6];
    short facing;
} BattleFxSummonRamuhRingRecord;

/* Angled sprite child record with random-seeded position and scale fields. */
typedef struct BattleFxSummonRamuhAngledSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short scale;
    unsigned char pad_0A[0x12];
    short sizeW;
} BattleFxSummonRamuhAngledSprite;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxSummonRamuhActorShake {
    short shakeX;
    short frameCount;
} BattleFxSummonRamuhActorShake;

/* Camera-script task record for Ramuh: script, matrix and actor slot. */
typedef struct BattleFxRamuhCameraTask {
    unsigned char pad_00[0xc];
    int cameraScript;
    int matrix;
    short actorSlot;
} BattleFxRamuhCameraTask;

/* PSX primitive written through a cursor by the Ramuh summon mesh draw; tag word 0x09000000. */
typedef struct BattleFxRamuhMeshCursorPrim {
    int tag;
    union {
        int color;
        CVECTOR colorVec;
    };
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    short uvWord1;
    short flags;
    unsigned char pad_18[0x4];
    short uvWord2;
    unsigned char pad_1E[0x6];
    short firstIndex;
} BattleFxRamuhMeshCursorPrim;

/* Ramuh PSX mesh packet: tag word and words copied from the data pointer. */
typedef struct BattleFxRamuhPsxPacket {
    int tag;
    union {
        int primWord;
        CVECTOR colorVec;
    };
    unsigned char pad_08[0x4];
    int uvWord0;
    unsigned char pad_10[0x4];
    int uvWord1;
    unsigned char pad_18[0x4];
    int uvWord2;
} BattleFxRamuhPsxPacket;

/* Ramuh summon mesh stream record: vertex offsets and a primitive word, read per polygon. */
typedef struct BattleFxRamuhMeshStream {
    short countFlags;
    short vertexIndexB;
    short vertexIndexC;
    short colorOffset;
    int primWord;
    union {
        struct {
            short uvWord1;
            short uvWord2;
            short firstIndex;
            short packedColor;
            short thirdIndex;
            short secondIndex;
        };
        struct { /* the vertex colours of a gouraud record */
            int color1;
            int color2;
            int color3;
        };
    };
    short fourthIndex;
    short vertexOffsetB;
    short vertexOffsetC;
} BattleFxRamuhMeshStream;

#pragma pack(push, 1)
typedef struct BattleSummonRamuhParticleColor {
    short particleFacing; /* +0x0 */
    short effectScale; /* +0x2 */
} BattleSummonRamuhParticleColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonRamuhRotXYZ {
    short angleX; /* +0x0 */
    short y; /* +0x2 */
    short z; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSummonRamuhRotXYZ;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonRamuhSpriteColor4 {
    char colorR; /* +0x0 */
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    unsigned char pad_3[0x1];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int frameTable; /* +0x8: the head of the angled-sprite draw request */
    int spriteValue; /* +0xc */
} BattleSummonRamuhSpriteColor4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonRamuhSpriteReq1 {
    unsigned char pad_0[0x4];
    char uvOffset; /* +0x4 */
    unsigned char pad_5[0xb];
    int object; /* +0x10 */
} BattleSummonRamuhSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonRamuhSpriteTransform1 {
    short matrixTail[5]; /* +0x0: the last five elements of the matrix that starts in g_BattleSummonRamuhSparkQuadTail */
    int translation[3]; /* +0xa */
    unsigned char pad_16[0x2];
    int fieldPtr; /* +0x18 */
    int secondPtr; /* +0x1c */
} BattleSummonRamuhSpriteTransform1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonRamuhTransformMatrix1 {
    unsigned char pad_0[0x10];
    int fieldPtr; /* +0x10 */
    int flagWord; /* +0x14 */
    unsigned char pad_18[0x2];
    short fixedFraction; /* +0x1a */
    unsigned char pad_1c[0x4];
    struct BattleFxSpriteGroup *spriteGroup; /* +0x20 */
    int drawObj; /* +0x24 */
} BattleSummonRamuhTransformMatrix1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxRamuhLineBurstPrim {
    int header; /* +0x0 */
    int color0; /* +0x4 */
    int xy0; /* +0x8 */
    int color1; /* +0xc */
    int xy1; /* +0x10 */
    int color2; /* +0x14 */
    int xy2; /* +0x18 */
} BattleFxRamuhLineBurstPrim;
#pragma pack(pop)

/* --- battle_fx_summon_titan --- */
/* Offscreen request queued for the Summon Titan stage: part mesh, position and camera-track term. */
typedef struct Battle3dSummonTitanRequest {
    int isActive;
    int partMesh;
    unsigned char pad_08[0x80];
    float transform[4][4]; /* the camera track as a transposed float matrix */
    int isOffscreen;
    int requestKind;
    int requestStage;
    unsigned char pad_D4[0x8];
    int posX;
    int posY;
    int posZ;
    int sourceIndex;
    unsigned char pad_EC[0x20];
    int isReady;
    int isQueued;
    int cameraTrackTerm;
} Battle3dSummonTitanRequest;

/* Titan debris particle: position, bouncing velocity, and three rotation angles with spin rates. */
typedef struct BattleFxTitanDebris {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    unsigned char pad_10[0x4];
    short historyNewest;
    unsigned char pad_16[0x2];
    char angleX;
    char angleY;
    char angleZ;
    unsigned char pad_1B[0x1];
    char spinX;
    char spinY;
    char spinZ;
} BattleFxTitanDebris;

/* Titan textured triangle: its three mesh vertices, averaged into the owner's centroid. */
typedef struct BattleFxTitanTriangleRef {
    Vec3sPacked *vertex[3];
} BattleFxTitanTriangleRef;

/* Titan textured quad: its four mesh vertices, averaged into the owner's centroid. */
typedef struct BattleFxTitanQuadRef {
    Vec3sPacked *vertex[4];
} BattleFxTitanQuadRef;

/* Titan fragment mesh record: centroid of a triangle and per-vertex offsets from it. */
typedef struct BattleTitanFragmentMesh {
    SVECTOR offsets[4];
    short centerX;
    short centerY;
    short centerZ;
} BattleTitanFragmentMesh;

/* Titan rotating fragment record: position copy, rotation speed and random angles. */
typedef struct BattleFxTitanFragmentRecord {
    unsigned char pad_00[0x4];
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x2];
    short rotSpeed;
    unsigned char pad_16[0x2];
    char angleA;
    char angleB;
    char angleC;
    unsigned char pad_1B[0x1];
    char rateA;
    char rateB;
    char rateC;
} BattleFxTitanFragmentRecord;

/* Task record for the Titan fragmenting mesh: frame counter and blend weights between two vertex tables. */
typedef struct BattleFxTitanFragmentTask {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x8];
    short sourceWeight;
    short targetWeight;
} BattleFxTitanFragmentTask;

/* Titan camera shake: vertical offset that decays each frame. */
typedef struct BattleFxTitanCameraShake {
    short amplitude;
    short frame;
    unsigned char pad_04[0x12];
    short decay;
} BattleFxTitanCameraShake;

/* Titan directed sprite with position and velocity copied from source vectors. */
typedef struct BattleFxTitanDirectedSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short scale;
    short angleX;
} BattleFxTitanDirectedSprite;

/* PSX mesh draw words for the Titan summon: the texture coordinate words are also written and
   halved byte by byte. */
typedef struct BattleFxTitanPsxMeshPacket {
    int tag;
    int colorCmd;
    unsigned char pad_08[0x4];
    union {
        int primWord;
        struct {
            unsigned char texU0;
            unsigned char texV0;
        };
    };
    unsigned char pad_10[0x4];
    union {
        int uvWordA;
        short uv1;
        struct {
            unsigned char texU1;
            unsigned char texV1;
            short tpage;
        };
    };
    unsigned char pad_18[0x4];
    union {
        int uvWordB;
        short uv2;
        struct {
            unsigned char texU2;
            unsigned char texV2;
        };
    };
    unsigned char pad_20[0x4];
    union {
        short vertexIndexA;
        struct {
            unsigned char texU3;
            unsigned char texV3;
        };
    };
} BattleFxTitanPsxMeshPacket;

/* Centroid of a textured triangle, used to offset its fragment vertices. */
typedef struct BattleFxTitanFragmentCentroid {
    SVECTOR offsets[4];
    short centroidX;
    short centroidY;
    short centroidZ;
} BattleFxTitanFragmentCentroid;

/* Effect record of the Titan directed-sprite task: a frame counter and an offset Z. */
typedef struct BattleFxTitanSpriteRecord {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocity[3]; /* the direction the sprite flies, which it faces */
    unsigned char pad_12[0x2];
    short historyNewest;
    short offsetZ;
} BattleFxTitanSpriteRecord;

/* Slot record of the Titan billboard sprite task. */
typedef struct BattleFxTitanBillboardSpriteTask {
    short ownerSlot;
    short spriteIndex;
    unsigned char pad_04[0x10];
    short scale;
} BattleFxTitanBillboardSpriteTask;

/* Ring buffer record for the Titan summon, with transform and angle fields. */
typedef struct BattleFxSummonTitanRingRecord {
    unsigned char pad_00[0x4];
    short transX;
    short flags;
    short transZ;
    unsigned char pad_0A[0x6];
    short angleY;
} BattleFxSummonTitanRingRecord;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxSummonTitanScatterSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short lifetime;
} BattleFxSummonTitanScatterSprite;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxSummonTitanPointSpriteTask {
    short stage;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
} BattleFxSummonTitanPointSpriteTask;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxSummonTitanVertexVelocity {
    short velocityX;
    unsigned char pad_02[0x2];
    short velocityZ;
} BattleFxSummonTitanVertexVelocity;

/* Camera-script task record for Titan: script, basis and slot. */
typedef struct BattleFxTitanCameraTask {
    unsigned char pad_00[0xc];
    int cameraScript;
    int basis;
    short slot;
} BattleFxTitanCameraTask;

/* Fragment vector read from the mesh task record at offset 0x1E into posA/posB/posC. */
typedef struct BattleFxTitanFragmentVector {
    short posA;
    unsigned char pad_02[0x2];
    short posB;
    unsigned char pad_06[0x2];
    short posC;
} BattleFxTitanFragmentVector;

/* Vertex of a Summon Titan rotating fragment: position and velocity pairs advanced each frame. */
typedef struct BattleTitanFragmentVertex {
    short angle;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    union {
        struct {
            short rotationX;
            short rotationY;
            short rotationZ;
        };
        SVECTOR rotation;
    };
    short rotationSpeedX;
    short rotationSpeedY;
    short rotationSpeedZ;
    unsigned char pad_22[0x2];
} BattleTitanFragmentVertex;

/* Titan fragment mesh vertex: timer plus two position/velocity triples advanced per frame. */
typedef struct BattleSummonTitanFragment {
    short shade;
    unsigned char pad_02[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    union {
        struct {
            short rotX;
            short rotY;
            short rotZ;
        };
        SVECTOR rot;
    };
    short spinX;
    short spinY;
    short spinZ;
    unsigned char pad_22[0x2];
} BattleSummonTitanFragment;

/* Sprite request for the Titan billboard sprite: its animation table, 3D scene object, colour and frame. */
typedef struct BattleSummonTitanModelRequest {
    int animTable;
    int sceneField;
    int color;
    short spriteIndex;
} BattleSummonTitanModelRequest;

/* Output PSX textured polygon packet of the Summon Titan fragment triangles and quads: the
   colour and texture coordinate words are also read and halved byte by byte. */
typedef struct BattleFxPsxTexturedFragmentPrim {
    int tag;
    union {
        int color;
        CVECTOR colorRgb;
    };
    unsigned char pad_08[0x4];
    union {
        int colorWord;
        struct {
            unsigned char texU0;
            unsigned char texV0;
        };
    };
    unsigned char pad_10[0x4];
    union {
        short uvWord0;
        struct {
            unsigned char texU1;
            unsigned char texV1;
        };
    };
    short texturePage;
    unsigned char pad_18[0x4];
    union {
        short uvWord1;
        struct {
            unsigned char texU2;
            unsigned char texV2;
        };
    };
    unsigned char pad_1E[0x6];
    union {
        short offsetC;
        struct {
            unsigned char texU3;
            unsigned char texV3;
        };
    };
} BattleFxPsxTexturedFragmentPrim;

/* GPU primitive packet written into the render-state block for the Summon Titan mesh draw: the
   texture coordinate words are also halved byte by byte. */
typedef struct BattleFxTitanMeshPrim {
    int tag;
    int colorCmd;
    unsigned char pad_08[0x4];
    union {
        int primWord;
        struct {
            unsigned char texU0;
            unsigned char texV0;
        };
    };
    unsigned char pad_10[0x4];
    union {
        short uvWordA;
        struct {
            unsigned char texU1;
            unsigned char texV1;
        };
    };
    short texPage;
    unsigned char pad_18[0x4];
    union {
        short uvWordB;
        struct {
            unsigned char texU2;
            unsigned char texV2;
        };
    };
} BattleFxTitanMeshPrim;

/* Titan summon mesh stream record: vertex offsets and a primitive word, read per polygon. */
typedef struct BattleFxTitanMeshStream {
    short headerWord;
    short vertexIndexB;
    short vertexIndexC;
    short colorWord;
    int primWord;
    union {
        struct {
            short uvWordA;
            short uvWordB;
        };
        int color1; /* the second vertex colour of a gouraud polygon */
    };
    union {
        struct {
            short vertexIndexA;
            short nextVertBOrShade;
        };
        int color2; /* the third vertex colour of a gouraud polygon */
    };
    union {
        struct {
            short nextVertCOrA;
            short nextVertB;
        };
        int color3; /* the fourth vertex colour of a gouraud quad */
    };
    short nextVertCOrQuadA;
    short nextQuadVertB;
    short nextQuadVertC;
} BattleFxTitanMeshStream;


/* Titan fragment entry with byte offsets into its vertex data. */
typedef struct BattleFxTitanFragmentOffsets {
    short vertexPOffset;
    short vertexQOffset;
    short vertexSOffset;
} BattleFxTitanFragmentOffsets;

#pragma pack(push, 1)
typedef struct BattleFxTitanSpriteBurstTask {
    unsigned char pad_0[0x12];
    int normalX; /* +0x12 */
    int normalY; /* +0x16 */
    int normalZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleFxTitanSpriteBurstTask;
#pragma pack(pop)

/* --- battle_fx_typhon_summon --- */
/* Typhon particle slot: phase, position and velocity fed into the summon model variant block. */
typedef struct BattleFxTyphonParticleSlot {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    unsigned char gridX;
    unsigned char gridY;
    unsigned char gridZ;
    unsigned char pad_17[0x1];
    short scale;
} BattleFxTyphonParticleSlot;

/* Typhon emitted tumbling model task: model index, position, rotation angles and frame counter. */
typedef struct BattleFxTyphonTumbleRecord {
    short modelIndex;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velY;
    unsigned char pad_10[0x4];
    unsigned char angleX;
    unsigned char angleY;
    unsigned char angleZ;
    unsigned char pad_17[0x1];
    short param;
    short drawParam;
} BattleFxTyphonTumbleRecord;

/* Typhon task record shared by the hexagon emitter, bouncing model and actor fall tasks. */
typedef struct BattleFxTyphonTaskRecord {
    short kind;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    int packedSpinAngles;
    short spread;
    short drawParam;
} BattleFxTyphonTaskRecord;

/* Typhon particle model slot: phase and step counters, rotation angle and scale for the draw handle. */
typedef struct BattleFxTyphonParticle3 {
    short phase;
    short step;
    short x;
    short y;
    short z;
    unsigned char pad_0A[0x2];
    short angleX;
    short angle;
    short angleZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxTyphonParticle3;

/* Task record for a particle on the Typhon orbit: origin, per-axis rates, step counter and scale. */
typedef struct BattleFxTyphonOrbitTask {
    short ownerSlot;
    short frameCount;
    union {
        Int64Pair origin;
        struct {
            short originX;
            short originY;
            short originZ;
        };
    };
    short rateX;
    short rateY;
    short rateZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxTyphonOrbitTask;

/* Start offset of the orbiting cloud particles: the Int64Pair is copied into the particle task. */
typedef union BattleFxTyphonInitialAngles {
    Int64Pair pair;
    struct {
        short x;
        short y;
        short z;
    };
} BattleFxTyphonInitialAngles;

/* Six vertices of one hexagon-emitter quad pair; the pair is drawn as quads (1, 2, 0, 3) and (0, 3, 5, 4). */
typedef struct BattleFxTyphonRingQuad {
    SVECTOR v[6];
} BattleFxTyphonRingQuad;

/* GPU textured-quad packet for one segment of the Typhon ring. */
typedef struct BattleFxTyphonRingPacket {
    int tag;
    int colorCommand;
    unsigned char pad_08[0x4];
    char u0;
    char v0;
    short clut;
    unsigned char pad_10[0x4];
    char u1;
    char v1;
    short texPage;
    unsigned char pad_18[0x4];
    char u2;
    char v2;
    unsigned char pad_1E[0x6];
    char u3;
    char v3;
} BattleFxTyphonRingPacket;

/* The tilt of the windstorm's bottom ring: X, Y and Z angles that random-walk by the sine of their
 * own random-walked phases; the rings above follow it. */
typedef struct BattleFxTyphonOrigin {
    short tiltX;
    short tiltY;
    short tiltZ;
    unsigned char pad_06[0x2];
    short phaseX;
    short phaseY;
    short phaseZ;
    unsigned char pad_e[0x2];
} BattleFxTyphonOrigin;

/* Task-slot record for the Typhon fade overlay: clamped fade level and a pending step. */
typedef struct BattleFxFadeOverlayTask {
    short ownerSlot;
    unsigned char pad_02[0x2];
    int fadeLevel;
    unsigned char pad_08[0x4];
    int pendingStep;
} BattleFxFadeOverlayTask;

/* Particle record emitted for the Typhon model-2 effect: polar offsets, phase and jitter. */
typedef struct BattleFxTyphonEmitterRecord {
    short frameCount;
    unsigned char pad_02[0x2];
    short sinOffset;
    short offsetX;
    short cosOffset;
    unsigned char pad_0A[0x4];
    short offsetY;
    unsigned char pad_10[0x4];
    char jitterA;
    char phase;
    char jitterB;
    unsigned char pad_17[0x1];
    short offsetZ;
} BattleFxTyphonEmitterRecord;

/* Actor record fields used by the Typhon fall: radius, base X and Z, and a clamped height offset. */
typedef struct BattleActorFallState {
    unsigned char pad_00[0x12];
    short radius;
    unsigned char pad_14[0x152];
    short baseX;
    short heightOffset;
    short baseZ;
} BattleActorFallState;

/* Particle of the Typhon summon's third model: a velocity vector and a random phase. */
typedef struct BattleFxTyphonParticleModel3 {
    short frameCount;
    unsigned char pad_02[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short phase;
    short angleZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxTyphonParticleModel3;

/* Particle emitted by the Typhon tumbling model: a random variant index and launch values. */
typedef struct BattleFxTyphonEmittedParticle {
    short variant;
    unsigned char pad_02[0x2];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    int colorWord;
    short posZ;
    short drawParam;
} BattleFxTyphonEmittedParticle;

/* Fall timeline for one battle actor of the Typhon summon: actor index, frame count and fall velocity. */
typedef struct BattleFxTyphonActorFall {
    short actorIndex;
    short frameCount;
    unsigned char pad_04[0xa];
    short fallVelocity;
} BattleFxTyphonActorFall;

/* Effect pool slot for the Typhon timeline; three random 12-bit angles. */
typedef struct BattleFxTyphonTimelineSlot {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pointA;
        struct {
            short angleX;
            short angleY;
            short angleZ;
        };
    };
    union {
        Int64Pair pointB;
        struct {
            short clearedShort0;
            short clearedShort1;
            short clearedShort2;
        };
    };
} BattleFxTyphonTimelineSlot;

/* Three-step fade history and viewport height for the Typhon sprite overlay. */
typedef struct BattleFxTyphonFadeConfig {
    unsigned char pad_00[0x4];
    unsigned char fadeHistory0;
    unsigned char fadeHistory1;
    unsigned char fadeHistory2;
    unsigned char pad_07[0x1];
    short viewportOriginX;
    short viewportOriginY;
    short viewportWidth;
    short viewportHeight;
} BattleFxTyphonFadeConfig;

/* Typhon ring state: ring centre, depth returned by the quad transform and a flags word. */
typedef struct BattleFxTyphonRingState {
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_06[0xa];
    SVECTOR vertices[2][16];
    int depth;
    int projectScratch;
    int flags;
} BattleFxTyphonRingState;

/* Ring-buffer record for the Typhon summon timeline, with fall position and Y rotation. */
typedef struct BattleFxTyphonTimelineRecord {
    unsigned char pad_00[0x4];
    short vectorX;
    short vectorY;
    short fallPosition;
    unsigned char pad_0A[0x6];
    short rotationY;
} BattleFxTyphonTimelineRecord;

/* Render state block written by the Typhon hexagon quads task, holding two packed words. */
typedef struct BattleFxTyphonRenderStateBlock {
    int renderWord0;
    int renderWord1;
} BattleFxTyphonRenderStateBlock;

/* Camera-script record that BattleFx_TyphonSpawnCameraScript fills for the Typhon run task. */
typedef struct BattleFxTyphonCameraTask {
    unsigned char pad_00[0xc];
    int scriptData;
    int basisMatrix;
    short actorIdx;
} BattleFxTyphonCameraTask;

/* Blend configuration for the Typhon offscreen stage requests. */
typedef struct BattleFxTyphonBlendConfig {
    short viewportOriginX;
    short viewportOriginY;
    short prevBlendLevel;
    short blendLevel;
} BattleFxTyphonBlendConfig;

/* Four-byte color entry of the Typhon flash color cycle; the channels follow the timeline state's. */
typedef struct BattleFxTyphonColorEntry {
    unsigned char red;
    unsigned char channel1;
    unsigned char channel2;
    unsigned char pad_03;
} BattleFxTyphonColorEntry;

/* One record of g_BattleSummonTyphonFallParam5 (0x14 bytes). */
typedef struct BattleSummonTyphonFallParam5 {
    short alpha; /*+0x00 */
    short ringPhase; /*+0x02 */
    short heightOffset; /*+0x04 */
    unsigned char pad_06[0xe];
} BattleSummonTyphonFallParam5;

/* Output PSX textured triangle packet of the Typhon mesh draw. */
typedef struct BattleFxPsxTyphonTriPrim {
    union {
        struct {
            int tag;
            union { int color; CVECTOR colorVec; };
            unsigned char pad_08[0x4];
            union { int primWord; unsigned char primWordLo; };
            unsigned char pad_10[0x4];
            union { short uvWord0; unsigned char uvWord0Lo; };
            short gpuAttrs;
            unsigned char pad_18[0x4];
            union { short uvWord1; unsigned char uvWord1Lo; };
            unsigned char pad_1E[0x6];
            union { short uvWord2; unsigned char uvWord2Lo; };
        };
        struct {
            unsigned char pad_prim_00[0x7];
            char primCode;
            unsigned char pad_prim_08[0x5];
            unsigned char shade0;
            unsigned char pad_prim_0E[0x7];
            unsigned char shade1;
            unsigned char pad_prim_16[0x7];
            unsigned char shade2;
            unsigned char pad_prim_1E[0x7];
            unsigned char texV3;
        };
    };
} BattleFxPsxTyphonTriPrim;

/* Typhon particle model variant: three shorts from particle bytes and scale values. */
#pragma pack(push, 1)
typedef struct BattleFxTyphonModelVariant {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
    int prevOffset;
    int scale;
    int offset;
    unsigned char pad_14[0x4];
    union {
        PsxMatrix matrix;
        struct {
            unsigned char pad_18[0x12];
            int transX;
            int transY;
            int transZ;
        };
    };
} BattleFxTyphonModelVariant;
#pragma pack(pop)

/* Typhon particle model draw handle: current and previous scale values. */
#pragma pack(push, 1)
typedef struct BattleFxTyphonDrawHandle {
    int scalePrev;
    int scaleY;
    int scale;
    unsigned char pad_0C[0x4];
    union {
        PsxMatrix matrix;
        struct {
            unsigned char pad_10[0x12];
            int transX;
            int transY;
            int transZ;
        };
    };
} BattleFxTyphonDrawHandle;
#pragma pack(pop)

/* Typhon emitted tumbling model state: angles, value history and a param. */
#pragma pack(push, 1)
typedef struct BattleFxTyphonTumbleState {
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_06[0x2];
    int valueOld;
    int valuePrev;
    int param;
    unsigned char pad_14[0x4];
    union {
        PsxMatrix matrix;
        struct {
            unsigned char pad_18[0x12];
            int transX;
            int transY;
            int transZ;
        };
    };
} BattleFxTyphonTumbleState;
#pragma pack(pop)

/* Typhon bouncing model record state: spin angles and value history. */
#pragma pack(push, 1)
typedef struct BattleFxTyphonBouncingState {
    short spinX;
    short spinY;
    short spinZ;
    unsigned char pad_06[0x2];
    int valueOld;
    int valuePrev;
    int valueNew;
    unsigned char pad_14[0x4];
    union {
        PsxMatrix matrix;
        struct {
            unsigned char pad_18[0x12];
            int transX;
            int transY;
            int transZ;
        };
    };
} BattleFxTyphonBouncingState;
#pragma pack(pop)

/* Typhon summon mesh record: vertex slot offsets, color bytes and primitive words. */
typedef struct BattleSummonTyphonMeshRecord {
    short numberEntries;
    short vertexBOffset;
    short vertexCOffset;
    short colorA;
    union { int primWord; CVECTOR color0Vec; };
    short uvWord0;
    short uvWord1;
    short vertexAOffset;
    short colorB;
    short vertexOffsetAlt1;
    short vertexOffsetAlt2;
    short vertexOffsetAlt3;
    short vertexOffsetAlt4;
    short vertexOffsetAlt5;
} BattleSummonTyphonMeshRecord;

/* PSX primitive drawn by the Typhon summon; code byte 0x24 or 0x2C, shade bytes halved. */
typedef struct BattleFxTyphonPrim {
    unsigned char pad_00[0x7];
    char primCode;
    unsigned char pad_08[0x5];
    unsigned char shade0;
    unsigned char pad_0E[0x7];
    unsigned char shade1;
    unsigned char pad_16[0x7];
    unsigned char shade2;
    unsigned char pad_1E[0x7];
    unsigned char texV3;
} BattleFxTyphonPrim;

#pragma pack(push, 1)
typedef struct BattleSummonTyphonFallParam0 {
    short state; /* +0x0 */
    short isReady; /* +0x2 */
} BattleSummonTyphonFallParam0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonTyphonBoundingCenter {
    unsigned char flags; /* +0x0 */
    unsigned char pad_1[0x5];
    short cameraVarTable; /* +0x6 */
} BattleSummonTyphonBoundingCenter;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonTyphonFallParam10 {
    int fallParam; /* +0x0 */
    unsigned char pad_4[0x16];
    int position; /* +0x1a */
} BattleSummonTyphonFallParam10;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonTyphonParticleParams0 {
    int orbitValue; /* +0x0 */
    int orbitLookup; /* +0x4 */
    unsigned char pad_8[0x8];
} BattleSummonTyphonParticleParams0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonTyphonParticleParams1 {
    int modelPointer; /* +0x0 */
    unsigned char pad_4[0x4];
    short paramWord; /* +0x8 */
    unsigned char pad_a[0x6];
} BattleSummonTyphonParticleParams1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonTyphonParticleParams2 {
    int scaledEntry; /* +0x0 */
    int scaledParam; /* +0x4 */
    unsigned char pad_8[0x4];
    short scaledSpriteFrame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSummonTyphonParticleParams2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonTyphonParticleParams3 {
    int billboardEntry; /* +0x0 */
    int billboardParam; /* +0x4 */
    unsigned char pad_8[0x4];
    short billboardSpriteFrame; /* +0xc */
    unsigned char pad_e[0xa];
} BattleSummonTyphonParticleParams3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonTyphonRingConfig0 {
    int ringValue; /* +0x0 */
    unsigned char pad_4[0x6];
    short blend; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonTyphonRingConfig0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonTyphonRingConfig2 {
    int fallParam; /* +0x0 */
    unsigned char pad_4[0x6];
    short blend; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonTyphonRingConfig2;
#pragma pack(pop)

#endif /* FF7_TYPES_BATTLE_SUMMON_H */

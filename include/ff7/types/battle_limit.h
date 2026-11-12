/* include/ff7/types/battle_limit.h: Limit Break battle effect types. */
#ifndef FF7_TYPES_BATTLE_LIMIT_H
#define FF7_TYPES_BATTLE_LIMIT_H

#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "lib/codegen.h"

/* --- battle_fx_limit_beast --- */
/* Per-slot state of the Limit beast spinning model pair, in the BattleFx effect-100 slot array. */
typedef struct BattleFxGalianBeastSpinPairState {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    short angle;
    short velocityX;
    short velocityY;
    short velocityZ;
    short countLimit;
    short spinAngleY;
    short spinRateY;
    short scale;
    short scaleRate;
    short count;
    short poolIndex;
} BattleFxGalianBeastSpinPairState;

/* Limit beast effect slot: timeline step, jittered base position and jitter range. */
typedef struct BattleFxLimitGalianBeastSlot {
    short ownerSlot;
    short step;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0xe];
    short spreadRadius;
    short jitterRange;
} BattleFxLimitGalianBeastSlot;

/* Effect100 slot for the Limit Beast spinning model: the source position, velocity, lifetime, angle and target actor. */
typedef struct BattleFxLimitGalianBeastSpin {
    unsigned char pad_00[0x4];
    union {
        Int64Pair position;
        struct {
            short posX;
            short posY;
            short posZ;
            short bobAngle;
        };
    };
    short velX;
    short velY;
    short velZ;
    short lifetime;
    short angle;
    short spinVelocity;
    short scale;
    short scaleVelocity;
    short delayCount;
    short targetActor;
} BattleFxLimitGalianBeastSpin;

/* Source actor position used by the Limit Beast timeline: the bone's world position as x, y, z and
 * the eight bytes together. */
typedef struct BattleLimitGalianBeastSource {
    union {
        Int64Pair pair;
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_6[0x2];
        };
    };
} BattleLimitGalianBeastSource;

/* The two rotation angles of one spinning model pair: about X and about Z. */
typedef struct BattleFxGalianBeastAnglePair {
    short angleX;
    unsigned char pad_02[0x2];
    short angleZ;
    unsigned char pad_06[0x2];
} BattleFxGalianBeastAnglePair;

/* PSX textured triangle primitive filled from a Limit Beast triangle record and its mesh descriptor. */
typedef struct BattleFxGalianBeastTexturedTriPrim {
    int tag;
    union {
        int colorCmd;
        struct {
            unsigned char pad_04[0x3];
            char command;
        };
    };
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    int flags;
    unsigned char pad_18[0x4];
    short uv2;
} BattleFxGalianBeastTexturedTriPrim;

/* Limit beast model record: mesh blob, draw list, render state, three working vertices, shade and colour. */
typedef struct BattleLimitGalianBeastModel {
    int meshData;
    int drawList;
    int otShift;
    int renderState;
    int flagMask;
    int frontFacing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    int flags;
    unsigned char pad_24[0x3c];
    Int64Pair vertex0;
    Int64Pair vertex1;
    Int64Pair vertex2;
    unsigned char pad_78[0x50];
    int shade;
    int fadeWeight;
    int shadedColor;
    unsigned char pad_D4[0x4];
    char baseColorR;
    char baseColorG;
    char baseColorB;
    unsigned char pad_DB[0x1];
    char colorR;
    char colorG;
    char colorB;
} BattleLimitGalianBeastModel;

/* --- battle_fx_limit_blaver --- */
/* Effect-100 record for the Blaver limit timeline, keyed by phase and actor slot. */
typedef struct BattleFxBraverTimeline {
    short ownerSlot;
    short phase;
    short actorSlot;
    short radius;
} BattleFxBraverTimeline;

/* PSX GPU packet words written into the Blaver trail draw list. */
typedef struct BattleFxBraverTrailPacket {
    int tag;
    int rgbCode;
    unsigned char pad_08[0x4];
    short uv0;
    short clut;
    unsigned char pad_10[0x4];
    short uv1;
    short texturePage;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxBraverTrailPacket;

/* Effect-table record for the Blaver actor matrix sprite task, with a phase and a matrix index. */
typedef struct BattleFxBraverActorSprite {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x2];
    short matrixIndex;
} BattleFxBraverActorSprite;

/* Frame-slot record for the Blaver trail ribbon; frame counter at +0x02 and actor matrix index at +0x06. */
typedef struct BattleBraverTrailRibbonRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x2];
    short actorMatrixIndex;
} BattleBraverTrailRibbonRecord;

/* Blaver billboard sprite task; frameAccumulator high byte is the frame index, angle the billboard angle. */
typedef struct BattleFxBraverBillboardTask {
    short ownerSlot;
    short frameAccumulator;
    unsigned char pad_04[0xc];
    short angle;
} BattleFxBraverBillboardTask;

/* Task slot for the Limit Blaver timeline, holding a negated actor radius. */
typedef struct BattleLimitBraverTaskSlot {
    unsigned char pad_00[0x6];
    short radius;
    unsigned char pad_08[0x8];
    short negRadius;
} BattleLimitBraverTaskSlot;

/* Billboard sprite request for the Blaver limit, with its resource and frame. */
typedef struct BattleFxBraverBillboardReq {
    int resource;
    int drawObject;
    unsigned char pad_08[0x4];
    short frame;
} BattleFxBraverBillboardReq;

#pragma pack(push, 1)
typedef struct BattleLimitBraver_RibbonPoints {
    unsigned char pad_0[0x2];
    short pointWord1; /* +0x2 */
    short pointWord2; /* +0x4 */
    unsigned char pad_6[0x2];
    short pointCoord; /* +0x8 */
    short pointWord5; /* +0xa */
    short pointWord6; /* +0xc */
    unsigned char pad_e[0xf2];
} BattleLimitBraver_RibbonPoints;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitBraver_SpriteReq1 {
    int handle; /* +0x0 */
    int resource; /* +0x4 */
    unsigned char pad_8[0x4];
    short frameIndex; /* +0xc */
    unsigned char pad_e[0x2];
    int vector; /* +0x10 */
} BattleLimitBraver_SpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitBraver_TrailColors {
    unsigned char pad_0[0x12];
    int vector; /* +0x12 */
    unsigned char pad_16[0x12];
} BattleLimitBraver_TrailColors;
#pragma pack(pop)

/* --- battle_fx_limit_boost_jump --- */
/* Limit B-Jump timeline record: step, position, actor index and dispatch level. */
typedef struct BattleFxBoostJumpTimeline {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x8];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_12[0xa];
    short actorIndex;
    short level;
} BattleFxBoostJumpTimeline;

/* --- battle_fx_limit_cm --- */
/* Battle effect slot for the Limit CM timeline; its position is copied into a second record. */
typedef struct BattleFxLimitCosmoMemoryTimeline {
    short ownerSlot;
    short timer;
    short originX;
    short originY;
    short state;
} BattleFxLimitCosmoMemoryTimeline;

/* Effect-100 slot record (0x20 stride) filled by the CM timeline task. */
typedef struct BattleFxCosmoMemorySlot {
    unsigned char pad_00[0x4];
    short originX;
    short originY;
    short state;
} BattleFxCosmoMemorySlot;

/* Effect-slot record (0x20 bytes) for the CM converging-projectile task: step and position. */
typedef struct BattleFxCosmoMemoryConvergeSlot {
    short state;
    short step;
    short posX;
    short posY;
    short posZ;
} BattleFxCosmoMemoryConvergeSlot;

/* Falling spinning model slot for Limit CM; frame counter, spin and model index. */
typedef struct BattleFxLimitCosmoMemorySpinRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short spin;
    short modelIndex;
} BattleFxLimitCosmoMemorySpinRecord;

/* Spawned CM model record placed around a centroid position. */
typedef struct BattleFxLimitCosmoMemorySpawn {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short spin;
    short modelIndex;
} BattleFxLimitCosmoMemorySpawn;

/* Limit CM effect record; offsets are taken against the draw list and state bases. */
typedef struct BattleFxCosmoMemorySpawnRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x14];
    short targetMask;
} BattleFxCosmoMemorySpawnRecord;

/* --- battle_fx_limit_ddive --- */
/* Limit D-dive impact task record in the effect slot array, keyed by actor index. */
typedef struct BattleFxLimitDragonDiveRecord {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short unusedWord;
    unsigned char pad_10[0x4];
    short yaw;
    unsigned char pad_16[0x4];
    short impactIndex;
    short targetActorIndex;
    short actorIndex;
} BattleFxLimitDragonDiveRecord;

/* Limit D-Dive timeline task record: frame, tracked positions and a slot index. */
typedef struct BattleLimitDragonDiveTimeline {
    short endFlag;
    short frame;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0xa];
    short savedYaw;
    unsigned char pad_16[0x8];
    short slotIndex;
} BattleLimitDragonDiveTimeline;

/* Limit D dive scaling record: frame counter and sprite position, offset by sin and cos. */
typedef struct BattleFxLimitDragonDiveScaling {
    short ownerSlot;
    short frameCount;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x2];
    short scaleX;
    short scaleY;
    short scaleZ;
} BattleFxLimitDragonDiveScaling;

/* Limit D-Dive impact slot that records the impact index and source slot. */
typedef struct BattleFxLimitDragonDiveImpactSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short yaw;
    unsigned char pad_16[0x4];
    short impactIndex;
    short targetActorIndex;
    short slotIndex;
} BattleFxLimitDragonDiveImpactSlot;

/* Vertical moving sprite task state for the DDive limit. */
typedef struct BattleFxDragonDiveVerticalSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    unsigned char pad_08[0x6];
    short velocityY;
} BattleFxDragonDiveVerticalSprite;

/* Record for the D-Dive limit effect holding the acting actor's position, yaw and index. */
typedef struct BattleDragonDiveRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short yaw;
    unsigned char pad_16[0x8];
    short actorIndex;
} BattleDragonDiveRecord;

/* D-Dive scaling model record; posX and posZ are a source actor's X and Y plus a circular offset. */
typedef struct BattleFxDragonDiveSpriteRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
} BattleFxDragonDiveSpriteRecord;

/* Ring-buffer record for the Limit D-Dive impact, copied from a parent record. */
typedef struct BattleFxDragonDiveImpact {
    short targetActorIndex;
    unsigned char pad_02[0xe];
    short impactIndex;
} BattleFxDragonDiveImpact;

#pragma pack(push, 1)
typedef struct BattleLimitDragonDiveSpriteRequest {
    int handle; /* +0x0 */
    int res; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleLimitDragonDiveSpriteRequest;
#pragma pack(pop)

/* --- battle_fx_limit_dise --- */
/* One tumbling Limit Dice die record; g_BattleLimitDiceDies holds six, 64 bytes each. */
typedef struct {
    short velocityX;
    short velocityY;
    short velocityZ;
    short gravity;
    short angle;
    short angleStep;
    short angleZ;
    short angleZStep;
    short spinAngle;
    short spinStep;
    int directionIndex;
    unsigned char pad_18[0x4];
    int spinFramesLeft;
    int turnAxis[3];
    unsigned char pad_2C[0x4];
    int moveDir[4];
} BattleLimitDiceDie;

/* Limit die tumble effect record: position, timer, state and dice record index. */
typedef struct BattleLimitDiceTumble {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short modelScale;
    unsigned char pad_0E[0x6];
    short state;
    short recordIndex;
} BattleLimitDiceTumble;

/* Tumbling die timeline record for the Dise limit. */
typedef struct BattleFxDiceTumbleState {
    unsigned char pad_00[0x4];
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    short spawned;
    short scale;
    unsigned char pad_0E[0x6];
    short state;
    short recordIndex;
} BattleFxDiceTumbleState;

/* --- battle_fx_limit_dolphin --- */
/* Limit DM timeline record: the source actor's bone position, the target's position, the phase
 * counter and the phase the projectile reaches the target (phaseStart). */
typedef struct BattleFxLimitDmTrack {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0x8];
    short phaseStart;
    short targetActor;
    short sourceActor;
} BattleFxLimitDmTrack;

/* Battle effect slot for the DM trailing projectile: position, velocity, the radius of the spiral
 * the trail draws, and the frame it reaches the target. */
typedef struct BattleFxDmTrailProjectile {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short invDistance; /* 1000 / distance to the target; the task does not read it */
    short velocityZ;
    unsigned char pad_12[0x6];
    short spiralRadius;
    short arrivalFrame;
} BattleFxDmTrailProjectile;

/* Limit DM pulsing radial sprite record: angle and radius orbit the base position on the ground
 * plane (X and Z). */
typedef struct BattleFxPulsingRadialSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY; /* set by the timeline, not read */
    short posZ;
    unsigned char pad_0A[0xa];
    short angle;
    short resIndex;
    short radius;
    short cycleIndex;
} BattleFxPulsingRadialSprite;

/* DM pulsing model task record: step counter, position copied into the model, pulse phase. */
typedef struct BattleFxLimitDragonPulseRecord {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short pulsePhase;
    short age; /* counted up each frame, not read */
} BattleFxLimitDragonPulseRecord;

/* Emitter state for the DM variant sprites: base position and frame counter. */
typedef struct BattleFxLimitDmSpriteEmitter {
    short ownerSlot;
    short frameIndex;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0xc];
    short variantIndex; /* copied into each sprite it emits */
} BattleFxLimitDmSpriteEmitter;

/* A task slot the DM timeline fills before the task runs: the record of the task it spawns. */
typedef union BattleFxLimitDMTaskRecord {
    BattleFxDmTrailProjectile projectile;
    BattleFxLimitDragonPulseRecord pulse;
    BattleFxPulsingRadialSprite radial;
    BattleFxLimitDmSpriteEmitter emitter;
} BattleFxLimitDMTaskRecord;

/* Limit DM variant sprite slot: position, animation frame and the sprite variant it draws. */
typedef struct BattleFxLimitDragonVariantSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variantIndex;
} BattleFxLimitDragonVariantSprite;

/* --- battle_fx_limit_dragon_dive --- */
/* Effect-array record for the Sinra limit timeline: step, position, angle and battler index. */
typedef struct BattleFxAllCreationRecord {
    short ownerSlot;
    short step;
    short posX;
    short baseY;
    short posY;
    unsigned char pad_0A[0xa];
    short angle;
    unsigned char pad_16[0x8];
    short battlerIndex;
} BattleFxAllCreationRecord;

/* Frame counter and rotation for the Sinra scaling model limit. */
typedef struct BattleLimitAllCreationScaling {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angleY;
} BattleLimitAllCreationScaling;

/* Effect-slot record (0x20 bytes) for the Sinra timeline task: orbit deltas and position. */
typedef struct BattleFxAllCreationOrbitSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    unsigned char pad_0E[0x2];
    short velZ;
    unsigned char pad_12[0x2];
    short angle;
} BattleFxAllCreationOrbitSlot;

#pragma pack(push, 1)
typedef struct BattleLimitAllCreationModelReq2 {
    int polySet; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short actorPos; /* +0xa */
    unsigned char pad_c[0x4];
} BattleLimitAllCreationModelReq2;
#pragma pack(pop)

/* --- battle_fx_limit_dynamite --- */
/* GPU Gouraud line packet (LINE_G2) of a Dynamite fuse spark; the spark dot uses its first eight bytes. */
typedef struct BattleFxLivewireLine {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    unsigned char commandCode;
    unsigned char pad_08[0x4];
    char endColorR;
    char endColorG;
    char endColorB;
} BattleFxLivewireLine;

/* Effect-100 slot record (0x20 stride) for the dynamite timeline; targets a bone. */
typedef struct BattleFxDynamiteSlot {
    short ownerSlot;
    short stage;
    unsigned char pad_04[0x8];
    short targetX;
    unsigned char pad_0E[0x2];
    short targetZ;
    unsigned char pad_12[0xc];
    short actorIndex;
} BattleFxDynamiteSlot;

/* Dynamite bouncing sprite record: horizontal and vertical velocity with a ground bounce. */
typedef struct BattleFxDynamiteBounceSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short angle;
    unsigned char pad_18[0x2];
    short variant;
} BattleFxDynamiteBounceSprite;

/* Limit Dynamite particle: position, per-frame velocity, angle, variant and level. */
typedef struct BattleFxDynamiteParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x4];
    short variant;
    unsigned char pad_1C[0x2];
    short level;
} BattleFxDynamiteParticle;

/* Ballistic state for the Dynamite limit: frame counter, position and per-frame velocity. */
typedef struct BattleLimitDynamiteBallistic {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x2];
    short yawAngle;
} BattleLimitDynamiteBallistic;

/* Twelve-byte per-bone rotation record of an actor animation bank, read by Limit Dynamite. */
typedef struct BattleBoneRotation {
    float angleX;
    float angleY;
    float angleZ;
} BattleBoneRotation;

/* Animation bank record, pointing at the bone rotation table for that bank. */
typedef struct BattleAnimBank {
    unsigned char pad_00[0x4];
    BattleBoneRotation *boneRotations;
} BattleAnimBank;

/* One animation of an actor model: its banks of per-bone rotations. */
typedef struct BattleModelAnimation {
    unsigned char pad_00[0x14];
    BattleAnimBank *banks;
} BattleModelAnimation;

/* Actor model header with two animation tables, each guarded by its own count. */
typedef struct BattleModelBoneTable {
    unsigned char pad_00[0x8];
    int rotationCount;
    BattleModelAnimation **rotationTable;
    int secondaryCount;
    BattleModelAnimation **secondaryTable;
} BattleModelBoneTable;

/* --- battle_fx_limit_er_effect --- */
typedef struct {
    int v[4];
} BattleErVec16;

/* Timeline state for Limit ER: frame counter, the position the sounds are panned from, timing-table
 * cursors and stand point. */
typedef struct BattleFxLimitEarthRaveRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short timingIndexA;
    short timingIndexB;
    short standPoint;
    short standIndex;
} BattleFxLimitEarthRaveRecord;

/* Limit ER impact burst slot: phase, the strike position, the strike's stage (0 to 4) and the hit
 * value of that strike's pending hit reaction. */
typedef struct BattleFxLimitEarthRaveBurst {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short stage;
    short linkedIndex;
} BattleFxLimitEarthRaveBurst;

/* Effect-100 slot record (0x20 stride) for the ER tumbling model task. */
typedef struct BattleFxTumblingSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    short angleX;
    short angleXStep;
    short angleZ;
    short angleZStep;
    short scale;
    short modelIndex;
} BattleFxTumblingSlot;

/* ER spinning shrinking record: angle and a scale that shrinks by a decaying step. */
typedef struct BattleFxLimitEarthRaveSpinShrink {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angularVelocity;
    short scale;
    short scaleStep;
} BattleFxLimitEarthRaveSpinShrink;

/* Emitter of the Limit ER sprite pool: frame counter, emit position, the strike's stage (which
 * pool) and its hit value (a positive one emits faster particles). */
typedef struct BattleFxLimitEarthRaveSpriteParticle {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short stage;
    short linkedIndex;
} BattleFxLimitEarthRaveSpriteParticle;

/* A task the impact burst spawns: the sprite-pool emitter (stage and hit value) or the spinning
 * shrinking model (angle and scale). */
typedef struct BattleFxEarthRaveImpactBurst {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angularVelocity;
    short scale;
    short scaleStep;
    unsigned char pad_14[0x4];
    short stage;
    short linkedIndex;
} BattleFxEarthRaveImpactBurst;

/* Ring-buffer timeline record for the Limit ER effect. */
typedef struct BattleFxEarthRaveRecord {
    short standPoint;
    unsigned char pad_02[0xe];
    short standIndex;
} BattleFxEarthRaveRecord;

/* Particle of the Limit ER sprite pool: position, velocity, size and stage. */
typedef struct BattleLimitEarthRaveParticle {
    short active;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    short accelY;
    unsigned char pad_14[0x2];
    short size;
} BattleLimitEarthRaveParticle;

/* Directional sprite state for the Limit ER effect: a frame index and a position advanced by a speed along a direction. */
typedef struct BattleFxLimitEarthRaveSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    short speed;
    BattleErVec16 direction;
    short sizeXZ;
    short scale;
} BattleFxLimitEarthRaveSprite;

/* --- battle_fx_limit_gaisei --- */
/* Limit Gaisei bouncing sprite task record: frame, position, velocity and angle. */
typedef struct BattleFxLimitLandscaperSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x4];
    short angle;
    unsigned char pad_18[0x2];
    short spriteKind;
} BattleFxLimitLandscaperSprite;

/* Timeline state for the Gaisei limit: a position, each axis with a per-step delta. */
typedef struct BattleLimitLandscaperTimeline {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short posXStep;
    short posYStep;
    short posZStep;
} BattleLimitLandscaperTimeline;

/* Frame counter and translation for a Gaisei fragment-morph limit particle. */
typedef struct BattleLimitLandscaperFragment {
    short ownerSlot;
    short frame;
    short x;
    short y;
    short z;
} BattleLimitLandscaperFragment;

/* Effect100 slot for the Gaisei limit timeline: step counter and effect level. */
typedef struct BattleFxLandscaperTimeline {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x14];
    short level;
} BattleFxLandscaperTimeline;

/* Effect100 slot for the Gaisei traveling sprite emitter: step counter, position and velocity per axis. */
typedef struct BattleFxLandscaperEmitter {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
} BattleFxLandscaperEmitter;

/* Effect slot for the Limit Gaisei actor fall; accumulates a fall speed per actor. */
typedef struct BattleFxLandscaperFallSlot {
    short ownerSlot;
    unsigned char pad_02[0xc];
    short fallSpeed;
    unsigned char pad_10[0xc];
    short actorIndex;
} BattleFxLandscaperFallSlot;

/* PSX mesh packet the Gaisei mesh draw fills: a textured quad (UV pairs, CLUT and page) or a
 * Gouraud triangle or quad (texCoord0..2 are its colours). */
typedef struct BattleFxLandscaperPacket {
    int tag;
    int colorCmd;
    unsigned char pad_08[0x4];
    union {
        int texCoord0;
        struct { unsigned char u0; unsigned char v0; };
    };
    unsigned char pad_10[0x4];
    union {
        int texCoord1;
        struct { short uvWord1; short flags; };
        struct { unsigned char u1; unsigned char v1; };
    };
    unsigned char pad_18[0x4];
    union {
        int texCoord2;
        short uvWord2;
        struct { unsigned char u2; unsigned char v2; };
    };
    unsigned char pad_20[0x4];
    union {
        short texCoord3;
        struct { unsigned char u3; unsigned char v3; };
    };
} BattleFxLandscaperPacket;

/* Per-slot state of the Gaisei animated sprite task: frame index, accumulating value and table index. */
typedef struct BattleFxLimitLandscaperSpriteState {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velY;
    unsigned char pad_10[0xa];
    short tableIndex;
} BattleFxLimitLandscaperSpriteState;

/* Gaisei fragment sprite slot that carries its fragment index. */
typedef struct BattleFxLandscaperFragmentSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short offsetY;
    unsigned char pad_10[0xa];
    short tableIndex;
    short fragmentIndex;
} BattleFxLandscaperFragmentSprite;

/* Task slot for the Gaisei flash and camera shake; counts steps up to 40. */
typedef struct BattleFxLandscaperShakeSlot {
    short ownerSlot;
    short step;
} BattleFxLandscaperShakeSlot;

/* Per-fragment fall position; each axis is advanced by the matching BattleLimitLandscaperFragmentVelocity. */
typedef struct BattleLimitLandscaperFallState {
    short fallPosition;
    short posY;
    short posX;
    unsigned char pad_06[0x2];
} BattleLimitLandscaperFallState;

/* Per-fragment velocity from BattleFx_MakeRandomConeVector; velocityY gains 0x14 each step and bounces. */
typedef struct BattleLimitLandscaperFragmentVelocity {
    short fallVelocity;
    short velocityY;
    short velocityX;
    unsigned char pad_06[0x2];
} BattleLimitLandscaperFragmentVelocity;

/* Where one actor stood when the Gaisei limit began; posY is the floor its fall stops at. */
typedef struct BattleLimitLandscaperSavedPosition {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
} BattleLimitLandscaperSavedPosition;

/* GPU primitive packet written into the render-state block for the Limit Gaisei mesh draw: a
 * textured triangle whose three UV pairs are halved outside UI scaling mode two. */
typedef struct BattleFxLandscaperMeshPrim {
    int tag;
    int colorCmd;
    unsigned char pad_08[0x4];
    union {
        int primWord;
        struct { unsigned char u0; unsigned char v0; };
    };
    unsigned char pad_10[0x4];
    union {
        short lightWordA;
        struct { unsigned char u1; unsigned char v1; };
    };
    short flags;
    unsigned char pad_18[0x4];
    union {
        short lightWordB;
        struct { unsigned char u2; unsigned char v2; };
    };
} BattleFxLandscaperMeshPrim;

/* Gaisei limit mesh record: vertex slot offsets, shade bytes and primitive words. */
typedef struct BattleLimitLandscaperMeshRecord {
    short flags;
    short vertex1Offset;
    short vertex2Offset;
    short shadeA;
    int primWord;
    union {
        struct { short clut; short texPage; };
        int primColor; /* a coloured primitive's second colour */
    };
    union {
        struct { short vertex0Offset; short shadeB; };
        int shadeColor; /* a coloured primitive's third colour */
    };
    union {
        struct { short vertex3Offset; short vertex4Offset; };
        int texCoord2; /* a coloured quad's fourth colour */
    };
    short vertex5Offset;
    short vertex6Offset;
    short vertex7Offset;
} BattleLimitLandscaperMeshRecord;

/* PSX primitive status word whose colour command is built from a shade value. */
typedef struct BattleFxPsxShadedStatus {
    unsigned char pad_00[0x4];
    int colorWord;
    unsigned char pad_08[0x5];
    unsigned char texV0;
    unsigned char pad_0E[0x7];
    unsigned char texV1;
    unsigned char pad_16[0x7];
    unsigned char texV2;
} BattleFxPsxShadedStatus;

#pragma pack(push, 1)
typedef struct BattleLimitLandscaperCenterXY {
    short posX; /* +0x0 */
    short posY; /* +0x2 */
} BattleLimitLandscaperCenterXY;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitLandscaperDrawRequest2 {
    char colorR; /* +0x0 */
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    char code; /* +0x3 */
    short kind; /* +0x4 */
    unsigned char pad_6[0x2];
    int frameTable; /* +0x8: the sprite's frame table; its first word points at the block whose +4 is the frame count */
    int spriteData; /* +0xC */
} BattleLimitLandscaperDrawRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitLandscaperFragmentRecords {
    BattleLimitLandscaperFragmentVelocity velocities[112]; /* +0x0 */
    unsigned char pad_380[0x12];
    int centerXSaved; /* +0x392 */
    int centerYSaved; /* +0x396 */
    int centerZSaved; /* +0x39a */
} BattleLimitLandscaperFragmentRecords;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitLandscaperMeshWorkingBlob {
    short progress; /* +0x0 */
    unsigned char pad_2[0x2];
    int ptr; /* +0x4 */
    unsigned char pad_8[0x934];
} BattleLimitLandscaperMeshWorkingBlob;
#pragma pack(pop)

/* --- battle_fx_limit_garyoten --- */
/* Garyoten actor lift-and-return record: actor index, lift velocity and step counters. */
typedef struct BattleFxFinishingTouchLift {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x2];
    short liftVelocity;
    unsigned char pad_08[0x2];
    short liftDecay;
    short yawVelocity;
    short yawAccel;
    unsigned char pad_10[0x4];
    short actorIndex;
    short stepLimit;
    short baseY;
    short returnBlocked;
    short stepCount;
} BattleFxFinishingTouchLift;

/* Garyoten ring chain effect: ring centre, drift velocities and fade timer in one Effect60 slot. */
typedef struct BattleFxFinishingTouchRing {
    short ownerSlot;
    short timer;
    short centerX;
    short centerHeight;
    short centerZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
} BattleFxFinishingTouchRing;

/* Effect-60 slot record (0x20 stride) for the falling Garyoten model task. */
typedef struct BattleFxFallingModelSlot {
    short ownerSlot;
    short timer;
    short posX;
    short fallPos;
    short posZ;
    unsigned char pad_0A[0x4];
    short fallVelocity;
    unsigned char pad_10[0x4];
    short angleX;
    short angleSpeedX;
    short angleZ;
    short angleSpeedZ;
    short scale;
    short modelIndex;
} BattleFxFallingModelSlot;

/* Limit Garyoten radial sprite burst: centre, speed and radius ranges, spawn limits and state index. */
typedef struct BattleFxFinishingTouchBurst {
    short ownerSlot;
    short frame;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short speedBase;
    short speedRange;
    short baseScale;
    unsigned char pad_12[0x2];
    short radiusBase;
    short radiusRange;
    short spawnLimit;
    short spriteCount;
    short stateIndex;
} BattleFxFinishingTouchBurst;

/* Timeline state for the Garyoten limit: its phase and the targets' centre on the ground (X, Z). */
typedef struct BattleLimitFinishingTouchTimeline {
    short ownerSlot;
    short phase;
    short targetX;
    unsigned char pad_06[0x2];
    short targetZ;
} BattleLimitFinishingTouchTimeline;

/* Effect100 slot for the Garyoten grow and shrink model: step, growth and scale with their rates. */
typedef struct BattleFxFinishingTouchGrowShrink {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short growth;
    short growthRate;
    unsigned char pad_10[0x4];
    short scale;
    short scaleRate;
    unsigned char pad_18[0x2];
    short scaleDecay;
} BattleFxFinishingTouchGrowShrink;

/* Psx GPU textured-quad packet in the Garyoten ring state: tag, command, uv, clut and texpage. */
typedef struct BattleFxFinishingTouchRingPacket {
    int tag;
    int command;
    unsigned char pad_08[0x4];
    char u0;
    char v0;
    short clut;
    unsigned char pad_10[0x4];
    char u1;
    char v1;
    short tpage;
    unsigned char pad_18[0x4];
    char u2;
    char v2;
    unsigned char pad_1E[0x6];
    char u3;
    char v3;
} BattleFxFinishingTouchRingPacket;

/* The packets of the sixteen quads between two rings, two banks each (drawn alternately);
   g_BattleLimitFinishingTouchRingQuadPackets starts fifteen of these rows. */
typedef BattleFxFinishingTouchRingPacket BattleLimitGaryotenQuadRow[16][2];

/* Scaled model task state for the Garyoten limit, stepping through a scale table. */
typedef struct BattleFxFinishingTouchScaledModel {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short baseScale;
} BattleFxFinishingTouchScaledModel;

/* A task slot the timeline task fills for one of the three tasks it spawns. */
typedef union BattleFxLimitGaryotenSlot {
    BattleFxFinishingTouchGrowShrink growShrink;
    BattleFxFinishingTouchScaledModel scaledModel;
    BattleFxFinishingTouchLift lift;
} BattleFxLimitGaryotenSlot;

/* Ring chain scratch: the current ring's position, two banks of sixteen circle points (the
   previous ring's and this one's), and Gfx_TransformQuadReturnDepth's depth and flag outputs. */
typedef struct BattleFxFinishingTouchState {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0xa];
    SVECTOR ringPoints[2][16]; /* +0x10 */
    int quadDepth;                /* +0x110 */
    int unused114;
    int flags;                    /* +0x118 */
} BattleFxFinishingTouchState;

/* Garyoten expiry task: counts frames (timer) while the event flags share a bit with actorMask. */
typedef struct BattleFxFinishingTouchMaskTask {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x18];
    short actorMask;
} BattleFxFinishingTouchMaskTask;

/* Garyoten radial burst particle: active flag, age, position and velocity, damped each frame. */
typedef struct BattleFxFinishingTouchBurstParticle {
    short active;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x4];
    short scale;
} BattleFxFinishingTouchBurstParticle;

/* One actor's burst sprite pool; ten of them start at g_BattleLimitFinishingTouchParticleBuf. */
typedef BattleFxFinishingTouchBurstParticle BattleLimitGaryotenBurstPool[48];

/* One actor's peer group (0x2C bytes; ten of them start at g_BattleLimitFinishingTouchPeerGroupHead):
   the timeline id of the actor's lift (-1 for none), and the actors standing on the same spot. */
typedef struct BattleLimitFinishingTouchPeerGroup {
    short timelineId;
    short peerCount;
    int peers[10];
} BattleLimitFinishingTouchPeerGroup;

/* A peer group read from its peer list (g_BattleLimitFinishingTouchPeerList is the first group's). */
typedef struct BattleLimitFinishingTouchPeerList {
    int peers[10];
    short nextTimelineId;
    short nextPeerCount;
} BattleLimitFinishingTouchPeerList;

/* One of the sixteen 20-byte ring records: position, the jittered angle triple and the radius.
   Three globals start inside the first record, so the angles and the radius are read through
   the record views below, which start at those members. */
typedef struct BattleLimitFinishingTouchRing {
    short posX;
    short height;
    short posZ;
    short pad_06;
    short angleX;
    short angleY;
    short angleZ;
    short pad_0E;
    int radius;
} BattleLimitFinishingTouchRing;

/* A ring record read from its angle triple (g_BattleLimitFinishingTouchRingAngles). */
typedef struct BattleLimitFinishingTouchRingAngles {
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_06[0xe];
} BattleLimitFinishingTouchRingAngles;

/* A ring record read from its radius (g_BattleLimitFinishingTouchRingScales). */
typedef struct BattleLimitFinishingTouchRingRadius {
    int radius;
    unsigned char pad_04[0x10];
} BattleLimitFinishingTouchRingRadius;

/* Scratch of BattleFx_LimitFinishingTouchUpdateRingChain: the jitter angles, then the offset they
   rotate, and the rotation matrix built from the angles. */
typedef struct BattleLimitFinishingTouchRingScratch {
    SVECTOR vector;
    unsigned char pad_08[0x10];
    PsxMatrix rotation; /* +0x18 */
} BattleLimitFinishingTouchRingScratch;

/* An actor position saved before the lift (one of ten, from g_BattleLimitFinishingTouchLiftState). */
typedef struct BattleLimitFinishingTouchLiftState {
    short posX;
    short posY;
    short posZ;
    short pad_06;
} BattleLimitFinishingTouchLiftState;

/* A point and a radius: the actor's attach point and the radial burst's centre. */
typedef struct BattleLimitFinishingTouchPoint {
    short posX;
    short posY;
    short posZ;
    short radius;
} BattleLimitFinishingTouchPoint;

#pragma pack(push, 1)
typedef struct BattleLimitFinishingTouchSwayState {
    short ringPhase; /* +0x0 */
    short angle; /* +0x2 */
} BattleLimitFinishingTouchSwayState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitFinishingTouchSwayWalk {
    short randomWalk; /* +0x0 */
    short angle; /* +0x2 */
} BattleLimitFinishingTouchSwayWalk;
#pragma pack(pop)

/* --- battle_fx_limit_highwind --- */
/* Hiwind missile: position and velocity per axis, with gravity on Y. The timeline task sets a new
 * one's position and velocity; the missile task moves it for ten frames. */
typedef struct BattleFxHighwindProjectile {
    short ownerSlot;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxHighwindProjectile;

/* A burst sprite around the Hiwind target: the emitter task places it on a circle about the
 * target centroid with a random rotation, and the burst sprite task steps its frame. */
typedef struct BattleFxHighwindBurstSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    SVECTOR rotation;
} BattleFxHighwindBurstSprite;

/* Ring-buffer record (tag 0xA) the Hiwind timeline queues for each missile hit: the hit target
 * and the hit's index. */
typedef struct BattleFxHighwindHitRecord {
    short targetActor;
    unsigned char pad_02[0xe];
    short hitIndex;
} BattleFxHighwindHitRecord;

/* Shrinking-model state for the Hiwind limit: a frame counter and the model's translation. */
typedef struct BattleFxHighwindShrinkRecord {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
} BattleFxHighwindShrinkRecord;

/* --- battle_fx_limit_hitumetu --- */
/* Fading Hitumetu particle sprite: position advanced by velocity over eight frames. */
typedef struct BattleFxDoomOfTheLivingParticle {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x8];
    short spriteIndex;
} BattleFxDoomOfTheLivingParticle;

/* Task-slot record for the Limit Hitumetu pulsing model pair: phase and three offset words. */
typedef struct BattleFxDoomOfTheLivingPulseRecord {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
} BattleFxDoomOfTheLivingPulseRecord;

/* Limit pulse model pair slot that counts to its spawn frame. */
typedef struct BattleFxLimitPulseModelSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
} BattleFxLimitPulseModelSlot;

/* Hitumetu animated sprite task state with a frame counter and animation index. */
typedef struct BattleFxDoomOfTheLivingAnimSprite {
    short ownerSlot;
    short frame;
    short posX;
    unsigned char pad_06[0x14];
    short animIndex;
} BattleFxDoomOfTheLivingAnimSprite;

/* Battle task slot for the Hitumetu timeline, with two fields set per frame. */
typedef struct BattleDoomOfTheLivingTimelineSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    unsigned char pad_08[0x4];
    short velocityX;
    unsigned char pad_0E[0xC];
    short variant;
} BattleDoomOfTheLivingTimelineSlot;

/* Ring-buffer timeline record for the Limit Hitumetu effect. */
typedef struct BattleFxDoomOfTheLivingRecord {
    short targetActorIndex;
    unsigned char pad_02[0xe];
    short sequenceIndex;
} BattleFxDoomOfTheLivingRecord;

/* --- battle_fx_limit_hjump --- */
/* Timeline state for Limit H-Jump: frame counter and the battle actor it targets. */
typedef struct BattleFxLimitHyperJumpRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x14];
    short actorIndex;
} BattleFxLimitHyperJumpRecord;

/* Limit H-jump task record: copies a request record and holds sin/cos offsets. */
typedef struct BattleFxLimitHyperJumpTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angleStep;
    unsigned char pad_16[0x2];
    short spinAngle;
    short direction;
    unsigned char pad_1C[0x2];
    short actorIndex;
} BattleFxLimitHyperJumpTask;

/* Limit H-jump model slot: timeline step, jump base and amplitude, and an accumulating angle. */
typedef struct BattleFxLimitHyperJumpModel {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short jumpBase;
    short angle;
    unsigned char pad_10[0x4];
    short angleStep;
    unsigned char pad_16[0x2];
    short spinAngle;
    short jumpAmplitude;
} BattleFxLimitHyperJumpModel;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxHyperJumpSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x10];
    short modelIndex;
} BattleFxHyperJumpSlot;

/* Per-slot state of the H-jump bone-oriented model task: frame counter and bone index. */
typedef struct BattleFxLimitHyperJumpBoneState {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x14];
    short boneIndex;
} BattleFxLimitHyperJumpBoneState;

/* A particle's position or velocity: three shorts and padding to eight bytes. */
typedef struct BattleLimitHyperJumpParticleVec {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
} BattleLimitHyperJumpParticleVec;

/* The H-Jump model request that the oscillating, displaced-mesh and growing tasks fill in: a model
 * request, a growth scale, a model request with a fade alpha, a displaced-mesh request and a second scale. */
#pragma pack(push, 1)
typedef struct {
    int modelRes;
    unsigned char pad_04[0x6];
    short fadeLevel;
    unsigned char pad_0C[0x4];
    int scale[3]; /* ScaleMatrix's vector; scale[1] is set from the oscillating model's scaleY */
    unsigned char pad_1C[0x4];
    int res;
    unsigned char pad_24[0x6];
    short fadeAlpha;
    unsigned char pad_2C[0x4];
    int meshReq;
    unsigned char pad_34[0xc];
    int scale2[3];
    unsigned char pad_4C[0x4];
} BattleLimitHJumpModelReqB;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitHyperJumpParticleState1 {
    short prevPosX; /* +0x0 */
    short prevPosY; /* +0x2 */
} BattleLimitHyperJumpParticleState1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitHyperJumpCompositeModelDrawReq {
    int modelHandle; /* +0x0 */
    unsigned char pad_4[0x1c];
    int scaleMatrix; /* +0x20 */
    unsigned char pad_24[0xc];
    int rotMatrix; /* +0x30 */
    unsigned char pad_34[0x4];
} BattleLimitHyperJumpCompositeModelDrawReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitHyperJumpModelReqA {
    int selectedRes; /* +0x0 */
    unsigned char pad_4[0x6];
    short timerStep; /* +0xa */
    unsigned char pad_c[0x4];
    int resArray; /* +0x10 */
    unsigned char pad_14[0x4];
} BattleLimitHyperJumpModelReqA;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitHyperJumpSpriteRequest2 {
    int spriteHandle; /* +0x0 */
    int res; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleLimitHyperJumpSpriteRequest2;
#pragma pack(pop)

/* --- battle_fx_limit_hmoon --- */
/* Limit H moon-wave strip effect slot: timer, base offsets and spread for the strip task. */
typedef struct BattleFxLimitHowlingMoonWaveStrip {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short spread;
    short uniformScale;
} BattleFxLimitHowlingMoonWaveStrip;

/* Limit H moon timeline record: frame counter, the caster's bone position (the spawner fills posX,
 * posY and posZ through Battle_GetBoneWorldPos), the wave spread and a flag set when the wave strip
 * task starts. */
typedef struct BattleFxLimitHowlingMoonTimeline {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short spread;
    unsigned char pad_16[0x6];
    short waveStarted;
} BattleFxLimitHowlingMoonTimeline;

/* Limit H-Moon fading billboard sprite: position, decaying velocity, frame counter and sprite index. */
typedef struct BattleFxHowlingMoonFadingSprite {
    short ownerSlot;
    short frame;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x2];
    short spriteIndex;
} BattleFxHowlingMoonFadingSprite;

/* Per-slot state of the H-moon orbit task, with position and per-step velocity. */
typedef struct BattleFxLimitHowlingMoonState {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x2];
    short scale;
    short modelScale;
} BattleFxLimitHowlingMoonState;

/* Stack-local primitive record for the H-Moon wave strips; tag word 0x09000000 in its top byte. */
typedef struct BattleFxLimitHowlingMoonStripPrim {
    int tagWord;
    union {
        int colorWord; /* r, g and b; primCode overwrites the top byte */
        struct {
            unsigned char pad_04[0x3];
            char primCode;
        };
    };
    unsigned char pad_08[0x4];
    int uvWord0;
    unsigned char pad_10[0x4];
    int uvWord1;
    unsigned char pad_18[0x4];
    int uvWord2;
    unsigned char pad_20[0x4];
    int uvWord3;
} BattleFxLimitHowlingMoonStripPrim;

/* --- battle_fx_limit_hvshot --- */
/* Effect-array record for a spinning Hvshot sprite: index, frame count, position, velocity and spin. */
typedef struct BattleFxBigShotSpriteRecord {
    short spriteIndex;
    short frameCount;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short angle;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short spinRate;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxBigShotSpriteRecord;

/* Hvshot hold fade sprite: slot index and frame counter with value and rate pairs. */
typedef struct BattleFxBigShotHoldFadeSprite {
    short slot;
    short frame;
    unsigned char pad_04[0x4];
    short posX;
    short fadeLevel;
    short posY;
    unsigned char pad_0E[0x2];
    short velX;
    short fadeStep;
    short velY;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxBigShotHoldFadeSprite;

/* Hvshot ramp-and-hold sprite record: quad index, frame counter, position and velocity vectors. */
typedef struct BattleFxBigShotRampRecord {
    short quadIndex;
    short frameCount;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxBigShotRampRecord;

/* Effect-100 slot record for the Hvshot limit timeline task, with a frame counter and an actor slot. */
typedef struct BattleFxBigShotTimelineRecord {
    short ownerSlot;
    short timer;
    short actorSlot;
    short sourceActor;
} BattleFxBigShotTimelineRecord;

/* Child particle of the Hvshot limit effect: variant, position, velocity and scale. */
typedef struct BattleFxBigShotParticle {
    short variant;
    unsigned char pad_02[0x2];
    short actorSlot;
    short sourceActor;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxBigShotParticle;

/* Effect-table record for the Hvshot model pair task, with a frame index and a matrix index. */
typedef struct BattleFxBigShotModelPair {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x2];
    short matrixIndex;
} BattleFxBigShotModelPair;

/* Hvshot target entry: a spin angle with the angle, radius and speed values the spinning sprites advance. */
typedef struct BattleLimitBigShotTargetState {
    short angle;
    short angleX;
    short angleY;
    unsigned char pad_06[0x2];
    short radius;
    short speedX;
    short speedY;
    unsigned char pad_0E[0x2];
} BattleLimitBigShotTargetState;

/* One state entry per battle task slot. */
typedef struct BattleLimitBigShot_TargetStateBlob {
    BattleLimitBigShotTargetState entries[100];
} BattleLimitBigShot_TargetStateBlob;

#pragma pack(push, 1)
typedef struct BattleLimitBigShot_ModelReq {
    int modelGroup; /* +0x0 */
    int mode; /* +0x4 */
    unsigned char pad_8[0x8];
} BattleLimitBigShot_ModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitBigShot_QuadColor1 {
    unsigned char pad_0[0x1];
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    unsigned char pad_3[0x1];
} BattleLimitBigShot_QuadColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitBigShot_QuadColor2 {
    unsigned char pad_0[0x1];
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    char code; /* +0x3 */
} BattleLimitBigShot_QuadColor2;
#pragma pack(pop)

/* --- battle_fx_limit_isshoku --- */
/* Isshoku effect task record: centroid copied from a global, and a random angle. */
typedef struct BattleFxLimitGauntletTaskRecord {
    unsigned char pad_00[0x4];
    short centroidXLow;
    short centroidXHigh;
    short centroidZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short angle;
    short rotZ;
    unsigned char pad_12[0xC];
    short actorIdx;
} BattleFxLimitGauntletTaskRecord;

#pragma pack(push, 1)
typedef struct BattleLimitGauntletCentroidX {
    short centroidXLow; /* +0x0 */
    short centroidXHigh; /* +0x2 */
} BattleLimitGauntletCentroidX;
#pragma pack(pop)

/* Draw request for the Isshoku models; the two angles are rewritten each frame. */
#pragma pack(push, 1)
typedef struct {
    int isshokuRes;
    int flags1;
    short uvScroll1;
    short reqAngle1;
    int stateFlags1;
    int reqModel;
    int flags2;
    short uvScroll2;
    short reqAngle2;
} BattleLimitIsshokuModelDrawRequest;
#pragma pack(pop)

/* --- battle_fx_limit_iyashi --- */
/* Effect-100 record for the Iyashi spiral sprites: step counter, actor slot and spiral angle. */
typedef struct BattleFxHealingWindSpiralSprite {
    short ownerSlot;
    short step;
    short actorSlot;
    unsigned char pad_06[0x4];
    short offsetY;
    unsigned char pad_0C[0x4];
    short angle;
    short angleStep;
} BattleFxHealingWindSpiralSprite;

/* Limit Iyashi animated sprite slot that counts frames. */
typedef struct BattleFxLimitHealingWindSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x10];
    int reqValueA;
    int reqValueB;
    short scale;
} BattleFxLimitHealingWindSprite;

/* Effect-table timeline record for the Iyashi spiral spawn task. */
typedef struct BattleFxHealingWindTimeline {
    unsigned char pad_00[0x4];
    short index;
    unsigned char pad_06[0x2];
    short radius;
    short offsetY;
    short offsetZ;
    unsigned char pad_0E[0x2];
    short angle;
    short angleStep;
} BattleFxHealingWindTimeline;

/* Effect-table record for the Iyashi midpoint model task, with a frame index. */
typedef struct BattleFxHealingWindMidpoint {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x4];
    short midX;
    short midY;
    short midZ;
} BattleFxHealingWindMidpoint;

/* Record for the Iyashi spiral sprites, filled from two table fields and a unit scale at +0x1C. */
typedef struct BattleHealingWindSpiralSpriteRecord {
    unsigned char pad_00[0x14];
    struct BattleFxSpriteGroup *spriteGroup;
    int drawObj;
    short scale;
} BattleHealingWindSpiralSpriteRecord;

/* Particle record for the Iyashi limit effect; zero marks an unclaimed slot. */
typedef struct BattleFxHealingWindParticleRecord {
    short active;
    unsigned char pad_02[0x5a];
} BattleFxHealingWindParticleRecord;

#pragma pack(push, 1)
typedef struct BattleLimitHealingWind_TaskHandle {
    int posXAddr; /* +0x0 */
    int cursor; /* +0x4 */
    unsigned char pad_8[0x8];
    short stepRotVectorX; /* +0x10 */
    short angle; /* +0x12 */
} BattleLimitHealingWind_TaskHandle;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitHealingWind_SpriteReq {
    unsigned char pad_0[0x8];
    int modelDrawRequest; /* +0x8 */
    unsigned char pad_c[0xc];
    int valueA; /* +0x18 */
    int valueB; /* +0x1c */
    unsigned char pad_20[0x4];
    short frameIndex; /* +0x24 */
    unsigned char pad_26[0x2];
} BattleLimitHealingWind_SpriteReq;
#pragma pack(pop)

/* --- battle_fx_limit_jinrai --- */
/* Task slot (0x20 bytes) of the Limit Jinrai timeline, one per target: BattleFx_LimitSpawnGreasedLightningPerTarget
 * writes actorIndex's bone position at +4, the target's at +0xC (posX..posZ), the target and actorIndex;
 * the timeline task draws its sprites at the target's position, allocates the target's typed record and
 * plays its sounds panned to actorIndex. */
typedef struct BattleFxGreasedLightningTask {
    short state;
    short step;
    unsigned char pad_04[0x8];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_12[0xa];
    short target;
    short actorIndex;
} BattleFxGreasedLightningTask;

/* --- battle_fx_limit_kodou --- */
/* Kodou rising spinning sprite: it climbs (posY grows each frame), tilts, and is drawn scaled
 * from startScale and pushed along the view by pushLength. */
typedef struct BattleFxLimitPulseOfLifeSpinSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short tiltAngle;
    unsigned char pad_12[0x2];
    short startScale;
    short pushLength;
} BattleFxLimitPulseOfLifeSpinSprite;

/* The Limit Kodou timeline task as it runs: the target actor, the step counter, the target's
 * position and its negated radius. */
typedef struct BattleFxPulseOfLifeTaskSlot {
    short actorIndex;
    short step;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0xc];
    short negRadius;
} BattleFxPulseOfLifeTaskSlot;

/* Kodou rising-sprite emitter: the actor whose bones the sprites start from, a spawn timer and the
 * actor's negated radius. */
typedef struct BattleFxLimitBoneSpawnSlot {
    short actorIndex;
    short timer;
    unsigned char pad_04[0x12];
    short negRadius;
} BattleFxLimitBoneSpawnSlot;

/* A task the Kodou timeline spawns: the target actor, a position, a velocity and the actor's
 * negated radius, copied from the timeline. */
typedef struct BattleFxPulseOfLifeTimelineRecord {
    short actorIndex;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short negRadius;
} BattleFxPulseOfLifeTimelineRecord;

/* Kodou pulsing model task record: the pulse step, the position the model is drawn at, and the
 * push length that feeds the billboard matrix. */
typedef struct BattleFxPulseOfLifePulseTask {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short pushLength;
} BattleFxPulseOfLifePulseTask;

/* Task-slot record for the Limit Kodou effect, one per target. */
typedef struct BattleFxPulseOfLifeTask {
    short actorIndex;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short negRadius;
} BattleFxPulseOfLifeTask;

/* Scratch block (Gfx_GetPsxScratchpadPtr(0)) the timeline uses to turn a random spin into a velocity: the
 * Euler angles (then the speed) and the rotation matrix RotMatrix builds from them. */
typedef struct BattleFxPulseOfLifeTransformScratch {
    SVECTOR angles;
    PsxMatrix matrix;
} BattleFxPulseOfLifeTransformScratch;

/* Scratch block of the rising sprites: the sprite's rotation matrix, the Euler angles it is built
 * from and the per-axis scale ScaleMatrix applies. */
#pragma pack(push, 1)
typedef struct BattleFxPulseOfLifeSpriteScratch {
    PsxMatrix matrix;
    SVECTOR angles;
    VECTOR scale;
} BattleFxPulseOfLifeSpriteScratch;
#pragma pack(pop)

/* --- battle_fx_limit_kyogiri --- */
/* The world position of a Kyogiri target's bone, as Battle_GetBoneWorldPos writes it; the tasks copy it as eight bytes. */
typedef union BattleFxKyogiriPos {
    Int64Pair pair;
    struct {
        short x;
        short y;
        short z;
    } xyz;
} BattleFxKyogiriPos;

/* Limit Kyogiri timeline slot: timeline step and a record type allocated per step. */
typedef struct BattleFxCrossSlashSlot {
    short ownerSlot;
    short step;
    short recordType;
    unsigned char pad_06[0x2];
    BattleFxKyogiriPos pos;
    unsigned char pad_10[0x8];
    short negRadius;
} BattleFxCrossSlashSlot;

/* Effect-100 record for the Kyogiri shrinking sprite task. */
typedef struct BattleFxCrossSlashShrinkSprite {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x4];
    BattleFxKyogiriPos pos;
    int spriteResource;
    int spriteResourceAlt;
    short negRadius;
} BattleFxCrossSlashShrinkSprite;

/* Kyogiri timeline record copied from a parent record, with two table-loaded values per step. */
typedef struct BattleFxCrossSlashTimeline {
    short step;
    unsigned char pad_02[0x6];
    BattleFxKyogiriPos pos;
    int tableValue8;
    int tableValue4;
    short negRadius;
} BattleFxCrossSlashTimeline;

/* Effect100 slot record for one Kyogiri trail sprite, faded over a frame count. */
typedef struct BattleFxCrossSlashTrailSprite {
    short progress;
    short frameCount;
    unsigned char pad_04[0x4];
    BattleFxKyogiriPos pos;
    int spriteResource;
    int spriteResourceAlt;
    short negRadius;
} BattleFxCrossSlashTrailSprite;

/* Sprite record spawned from the Kyogiri trail; copies the trail's fields and a shrunken size. */
typedef struct BattleCrossSlashShrinkingSpriteRecord {
    short ownerSlot;
    short size;
    unsigned char pad_04[0x4];
    BattleFxKyogiriPos pos;
    struct BattleFxSpriteGroup *spriteGroup;
    int drawObj;
    short negRadius;
} BattleCrossSlashShrinkingSpriteRecord;

/* Task-slot record for the Limit Kyogiri effect, one per target. */
typedef struct BattleFxCrossSlashTask {
    unsigned char pad_00[0x4];
    short targetIndex;
    unsigned char pad_06[0x2];
    BattleFxKyogiriPos pos;
    unsigned char pad_10[0x8];
    short negRadius;
} BattleFxCrossSlashTask;

/* --- battle_fx_limit_lgirl --- */
/* A sprite draw request BattleFx_SubmitSpriteDraw takes: texture, CLUT, colour and primitive code,
 * frame and flags. */
typedef struct {
    int texture;
    int clut;
    unsigned char colour[4];
    unsigned short frame;
    unsigned short flags;
} BattleLimitLgirlSpriteRequest;

/* Limit Lgirl trailing sprite record: orbiting point, scale, bob angle and history index. */
typedef struct BattleFxLuckyGirlTrail {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    short bobAmplitude;
    short orbitX;
    short orbitY;
    unsigned char pad_10[0x2];
    short orbitRadius;
    short scale;
    short scaleRate;
    short angle;
    short angleStep;
    short historyIndex;
} BattleFxLuckyGirlTrail;

/* A Lgirl sprite slot as the timeline fills it before the sprite task runs: position, the trail's
 * orbit, then the scale and angle each sprite task reads. */
typedef struct BattleLimitLuckyGirlSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    short bobAmplitude;
    short orbitX;
    short orbitY;
    unsigned char pad_10[0x2];
    short orbitRadius;
    short scale;
    short scaleRate;
    short angle;
    short angleStep;
    short historyIndex;
} BattleLimitLuckyGirlSprite;

/* Effect100 slot for the Lgirl sine-scaled sprite: step, scale, its amplitude and the sine angle. */
typedef struct BattleFxLuckyGirlSineSprite {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
    short scaleRate;
    short angle;
    short angleStep;
} BattleFxLuckyGirlSineSprite;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxLuckyGirlFadeSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
} BattleFxLuckyGirlFadeSlot;

/* --- battle_fx_limit_lunatic --- */
/* Limit Lunatic draw-entry task record, variant one: step counter and position. */
typedef struct BattleFxLimitLunaticHighEntryOne {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short uniformScale;
} BattleFxLimitLunaticHighEntryOne;

/* Spawn record of the Lunatic limit effect: a step counter and an index into the particle pool. */
typedef struct BattleFxLunaticHighSpawnSlot {
    short ownerSlot;
    short step;
    Int64Pair pos;
    short scale;
    unsigned char pad_0E[0x6];
    short poolIndex;
} BattleFxLunaticHighSpawnSlot;

/* Effect100 slot record for one Lunatic limit sprite particle, read by its pool task. */
typedef struct BattleFxLunaticHighSpriteParticle {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short speed;
    unsigned char pad_0E[0x6];
    short poolIndex;
} BattleFxLunaticHighSpriteParticle;

/* Lunatic spawn effect record: position, scale and the source pool index. */
typedef struct BattleFxLunaticHighSpawnRecord {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short scale;
    unsigned char pad_0E[0x6];
    short poolIndex;
} BattleFxLunaticHighSpawnRecord;

/* Task slot for the Limit Lunatic timeline, holding an effect radius and an actor index. */
typedef struct BattleLimitLunaticHighTimelineSlot {
    unsigned char pad_00[0x4];
    short bonePosX;
    short bonePosY;
    short bonePosZ;
    unsigned char pad_0A[0x2];
    short effectRadius;
    unsigned char pad_0E[0x6];
    short actorIndex;
} BattleLimitLunaticHighTimelineSlot;

/* One particle in the lunatic limit particle pool: active flag, age, position and velocity. */
typedef struct BattleLunaticHighParticle {
    short active;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short scale;
} BattleLunaticHighParticle;

/* --- battle_fx_limit_matsuri --- */
/* Ring-buffer timeline record for the Limit Matsuri effect. */
typedef struct BattleFxBloodfestRecord {
    short targetActorIndex;
    unsigned char pad_02[0xe];
    short hitIndex;
} BattleFxBloodfestRecord;

#pragma pack(push, 1)
typedef struct BattleLimitBloodfestPosDeltaX {
    short deltaX; /* +0x0 */
    short posY; /* +0x2 */
} BattleLimitBloodfestPosDeltaX;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitBloodfestHitFrames1 {
    unsigned char pad_0[0x2];
    short frameShort2; /* +0x2 */
} BattleLimitBloodfestHitFrames1;
#pragma pack(pop)

/* --- battle_fx_limit_meikyo --- */
/* Effect-100 slot record (0x20 stride) for the Meikyo growing model pair task. */
typedef struct BattleFxGrowingPairSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scaleStep;
} BattleFxGrowingPairSlot;

/* Meikyo effect pool record; tag is 0 when free, 1 for a gradient line (BattleFxClearTranquilGradientLine)
 * and 2 for a fading sprite (BattleFxClearTranquilFadeSprite). */
typedef struct BattleFxClearTranquilPoolRecord {
    short tag;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    union {
        SVECTOR rotation; /* a gradient line's rotation angles */
        struct {
            short velocityX; /* a fading sprite's per-frame step */
            short velocityY;
            short velocityZ;
        };
    };
} BattleFxClearTranquilPoolRecord;

/* The draw-list cursor and 0x12C effect records; this view starts at g_BattleLimitClearTranquilParticlePool and runs on over the pads behind it. */
#pragma pack(push, 1)
typedef struct BattleLimitClearTranquilParticlePool {
    int drawPtr;
    unsigned char pad_04[0x4];
    BattleFxClearTranquilPoolRecord records[0x12C];
} BattleLimitClearTranquilParticlePool;
#pragma pack(pop)

/* --- battle_fx_limit_meteo_r --- */
/* Limit Meteor R timeline slot: jittered offsets and per-slot scale values, set at spawn. */
typedef struct BattleFxLimitMeteorainTimeline {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    short countLimit;
    short rotYOrSeqStart;
    short rotStepOrActor;
    short size;
    short sizeStep;
    short ramp;
    short sizeStepX2;
    short scale;
    short velYOrScaleStep;
    short count;
} BattleFxLimitMeteorainTimeline;

/* Meteo growth effect record: timer, damped position and velocity, and angle and ramp accumulators. */
typedef struct BattleFxMeteoGrowRecord {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotY;
    short rotYStep;
    short scaleXZ;
    short scaleXZStep;
    short scaleY;
    short scaleYStep;
} BattleFxMeteoGrowRecord;

/* Meteor-R projectile: position, velocity per axis, age and scale. */
typedef struct BattleFxMeteorainProjectile {
    short ownerSlot;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0xa];
    short scale;
} BattleFxMeteorainProjectile;

/* Meteo R spinning fading record: angle with decaying angular velocity. */
typedef struct BattleFxMeteorainSpinFadeEntry {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angularVelocity;
    unsigned char pad_10[0x8];
    short scale;
    short scaleStep;
} BattleFxMeteorainSpinFadeEntry;

/* Limit meteor R timeline slot: timeline step and an index into the script timer sequence table. */
typedef struct BattleFxMeteorainTimeline {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x12];
    short sequenceIndex;
} BattleFxMeteorainTimeline;

/* Limit meteor R target slot: target position, scale, sequence start and a running count. */
typedef struct BattleFxMeteorainTarget {
    short ownerSlot;
    short step;
    short targetX;
    short targetY;
    short targetZ;
    short countLimit;
    short sequenceStart;
    unsigned char pad_0E[0xa];
    short scale;
    unsigned char pad_1A[0x2];
    short count;
} BattleFxMeteorainTarget;

/* Effect-slot record (0x20 bytes) for the MeteoR target timeline task: phase, velocity and sizes. */
typedef struct BattleFxMeteorainTargetSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short phase;
    short velocity;
    unsigned char pad_10[0x8];
    short size;
    short targetSize;
    short sequenceStart;
} BattleFxMeteorainTargetSlot;

/* Shrinking black model slot for Limit Meteo R; frame counter and rotation. */
typedef struct BattleFxMeteorainShrinkRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short rotation;
    short rotationStep;
} BattleFxMeteorainShrinkRecord;

/* Growing model slot for Limit Meteo R; frame counter and rotation. */
typedef struct BattleFxMeteorainGrowRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
    short scaleStep;
} BattleFxMeteorainGrowRecord;

/* Rising sprite record of the Meteor-R limit effect: a step index, a height and a decaying vertical velocity. */
typedef struct BattleFxMeteorainRisingSprite {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
    short velocityY;
} BattleFxMeteorainRisingSprite;

/* Particle pool task state: spawn origin words and a frame counter. */
typedef struct BattleFxMeteorainParticlePool {
    short ownerSlot;
    short timer;
    short originX;
    short originY;
    short originZ;
    unsigned char pad_0A[0xe];
    short spreadRadius;
    unsigned char pad_1A[0x2];
    short resourceIndex;
} BattleFxMeteorainParticlePool;

/* Limit Meteo-R target slot that steps toward its target position. */
typedef struct BattleFxLimitMeteorainTargetSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_12[0xa];
    short scale;
} BattleFxLimitMeteorainTargetSlot;

/* Meteo-R spark dot render primitive with colour bytes. */
typedef struct BattleFxMeteorainLivewireDot2 {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char command;
} BattleFxMeteorainLivewireDot2;

/* One 0x14-byte quad record of the Limit MeteoR mesh: four vertex offsets into the vertex table, then the
 * texture (u, v) of each vertex. */
typedef struct BattleFxMeteorainGradientQuad {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
    char u0;
    char v0;
    unsigned char pad_0A[0x2];
    char u1;
    char v1;
    char u2;
    char v2;
    char u3;
    char v3;
    unsigned char pad_12[0x2];
} BattleFxMeteorainGradientQuad;

/* The offset vector after the three matrices of the projectile task's data block: a point on a circle
 * and a random depth. */
typedef struct BattleFxMeteorainOffsetRecord {
    short offsetX;
    short offsetY;
    short jitter;
} BattleFxMeteorainOffsetRecord;

/* One sprite particle of the Limit Meteor R pool, placed at the origin plus a random offset. */
typedef struct BattleMeteorainParticle {
    short active;
    short stage;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short scale;
} BattleMeteorainParticle;

/* Sort record (4 bytes) of the Limit Meteo R angular texture gradient: the angle of one quad's centre
 * and the quad's position in the mesh. */
typedef struct BattleFxMeteorainQuadSortEntry {
    short angle;
    unsigned char quadPosition;
    char cleared;
} BattleFxMeteorainQuadSortEntry;

/* GPU textured triangle or quad packet (POLY_FT3/POLY_FT4) written by BattleFx_LimitMeteorainDrawPsxMesh;
 * the vertex positions between the words are written by the GTE draw helpers. */
typedef struct BattleFxMeteorainQuadPrim {
    int tag;
    union {
        int colorCmd; /* the grey level in r, g and b, the GPU command in the top byte */
        struct {
            unsigned char colorRgb[3];
            char code; /* bit 1: semi-transparent */
        };
    };
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    short uv1;
    short tpage;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxMeteorainQuadPrim;

/* GPU Gouraud-shaded triangle or quad packet (POLY_G3/POLY_G4) written by BattleFx_LimitMeteorainDrawPsxMesh. */
typedef struct BattleFxMeteorTailPrim {
    int tag;
    union {
        int color0; /* the first vertex colour, the GPU command in the top byte */
        struct {
            unsigned char color0Rgb[3];
            char code; /* bit 1: semi-transparent */
        };
    };
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxMeteorTailPrim;

/* The four primitive streams of the Meteo R mesh follow its vertex block. Each starts with a word whose
 * low half is its record count; the first stream's high half is its texture page. Vertices are byte
 * offsets into the vertex block. */

/* First stream: a textured triangle. */
typedef struct BattleFxMeteorainMeshTexTri {
    short vertex0;
    short vertex1;
    short vertex2;
    short color; /* the low byte is the grey level */
    int uv0Clut;
    short uv1;
    short uv2;
} BattleFxMeteorainMeshTexTri;

/* Second stream: a textured quad. */
typedef struct BattleFxMeteorainMeshTexQuad {
    short vertex0;
    short vertex1;
    short vertex2;
    short vertex3;
    int uv0Clut;
    short uv1;
    short uv2;
    short uv3;
    short color; /* the low byte is the grey level */
} BattleFxMeteorainMeshTexQuad;

/* Third stream: a Gouraud-shaded triangle. */
typedef struct BattleFxMeteorainMeshShadedTri {
    short vertex0;
    short vertex1;
    short vertex2;
    unsigned char pad_06[0x2];
    int color0;
    int color1;
    int color2;
} BattleFxMeteorainMeshShadedTri;

/* Fourth stream: a Gouraud-shaded quad. */
typedef struct BattleFxMeteorainMeshShadedQuad {
    short vertex0;
    short vertex1;
    short vertex2;
    short vertex3;
    int color0;
    int color1;
    int color2;
    int color3;
} BattleFxMeteorainMeshShadedQuad;

/* The draw walker's cursor into the streams. It reads the next record's vertices ahead, through [1]. */
typedef union BattleFxMeteoRMeshCursor {
    short header; /* a stream's record count, or the first stream's texture page */
    BattleFxMeteorainMeshTexTri texTri[2];
    BattleFxMeteorainMeshTexQuad texQuad[2];
    BattleFxMeteorainMeshShadedTri shadedTri[2];
    BattleFxMeteorainMeshShadedQuad shadedQuad[2];
} BattleFxMeteoRMeshCursor;

#pragma pack(push, 1)
typedef struct BattleLimitMeteorainStateC0 {
    short rotX;
    short rotY;
} BattleLimitMeteorainStateC0;
#pragma pack(pop)

/* --- battle_fx_limit_mind --- */
/* Mind-attack sprite particle: position and velocity per axis, damped each frame. */
typedef struct BattleFxMindDampedSprite {
    short ownerSlot;
    short age;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxMindDampedSprite;

/* Limit Mind particle record: kind, origin, per-frame step and scale, set at spawn. */
typedef struct BattleFxLimitMindblowParticle {
    short kind;
    unsigned char pad_02[0x2];
    short actorIndex;
    short matrixIndex;
    short originX;
    short originY;
    short originZ;
    unsigned char pad_0E[0x2];
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxLimitMindblowParticle;

/* Effect slot for the Limit Mind timeline; holds step and actor/slot indices. */
typedef struct BattleFxLimitMindblowTaskSlot {
    short ownerSlot;
    short step;
    short actorIndex;
    short slotIndex;
} BattleFxLimitMindblowTaskSlot;

/* Drifting sprite slot for Limit Mind: position advanced by velocity each frame. */
typedef struct BattleFxLimitMindblowDriftSprite {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxLimitMindblowDriftSprite;

/* Spinning model pair task state for the Mind limit. */
typedef struct BattleFxMindSpinModelPair {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x2];
    short matrixIndex;
} BattleFxMindSpinModelPair;

/* Task-slot record for the Limit Mind effect, one per target. */
typedef struct BattleFxMindTask {
    unsigned char pad_00[0x4];
    short targetMask;
    short actorIndex;
} BattleFxMindTask;

/* --- battle_fx_limit_rakuin --- */
/* Per-piece state of the Limit Rakuin effect, placed around a source record's position. */
typedef struct BattleFxLimitFuryBrandPiece {
    short ownerSlot;
    unsigned char pad_02[0x2];
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    short rotX;
    short velocityY;
    short rotZ;
    unsigned char pad_12[0x2];
    short pushLength;
} BattleFxLimitFuryBrandPiece;

/* BattleFx effect slot (0x20 bytes) for the Rakuin limit timeline: a record type, a phase and a centre. */
typedef struct BattleFxFuryBrandSlot {
    short recordType;
    short phase;
    union {
        Int64Pair pos;
        struct { short centerX; short centerY; short centerZ; unsigned char pad_0A[0x2]; };
    };
    unsigned char pad_0C[0x8];
    short negatedRadius;
} BattleFxFuryBrandSlot;

/* Per-slot state of the Rakuin animated sprite task: frame index, accumulating value and step. */
typedef struct BattleFxLimitFuryBrandSpriteState {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    short pushLength;
} BattleFxLimitFuryBrandSpriteState;

/* Per-slot state of the Rakuin fading model task, with a frame counter that drives alpha. */
typedef struct BattleFxLimitFuryBrandFadeState {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short scale;
} BattleFxLimitFuryBrandFadeState;

/* Growing fade model task state for the Rakuin effect. */
typedef struct BattleFxFuryBrandGrowFadeModel {
    short ownerSlot;
    short frame;
    short scaleDeltaX;
    unsigned char pad_06[0x2];
    short scaleDeltaY;
} BattleFxFuryBrandGrowFadeModel;

/* Task slot record for the Rakuin fading sprite: draw frame and mode, frame counter, position and rotation. */
typedef struct BattleFuryBrandSpriteRecord {
    short drawFrameAndMode;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x2];
    short pushLength;
} BattleFuryBrandSpriteRecord;

/* Task slot spawned by BattleFx_LimitFuryBrandSpawnFadingModelsTask. */
typedef struct BattleFxFuryBrandFadingSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short scale;
} BattleFxFuryBrandFadingSlot;

/* Task-slot record for the Limit Rakuin effect, one per target. */
typedef struct BattleFxFuryBrandTask {
    short targetActor;
    unsigned char pad_02[0x2];
    short posX; /* the target's bone position (Battle_GetBoneWorldPos) */
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short negatedRadius;
} BattleFxFuryBrandTask;

/* Task slot of the Rakuin sound timeline: frame counter and the centroid of the targets. */
typedef struct BattleFxLimitFuryBrandSoundTimeline {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxLimitFuryBrandSoundTimeline;

#pragma pack(push, 1)
typedef struct BattleLimitFuryBrandScaleDelta {
    short scale; /* +0x0 */
    int x; /* +0x2 */
    unsigned char pad_6[0x4];
    int y; /* +0xa */
} BattleLimitFuryBrandScaleDelta;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitFuryBrandSpriteReqA {
    struct BattleFxSpriteGroup *spriteGroup;
    int spriteData;
    union {
        int colorCommand;
        struct { char colorR; char colorG; char colorB; char command; };
    };
    short frame;
    unsigned char pad_e[0x2];
} BattleLimitFuryBrandSpriteReqA;
#pragma pack(pop)

/* The sprite request of the Rakuin animated sprites; the same layout as the fading sprite's request. */
#pragma pack(push, 1)
typedef struct BattleLimitFuryBrandSpriteReqB {
    struct BattleFxSpriteGroup *spriteGroup;
    int spriteData;
    unsigned char color[4];
    short frame;
    unsigned char pad_e[0x2];
} BattleLimitFuryBrandSpriteReqB;
#pragma pack(pop)

/* --- battle_fx_limit_ranntou --- */
/* Ranntou moving sprite record: orbit position and sine or cosine velocity. */
typedef struct BattleFxLimitBigBrawlSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxLimitBigBrawlSprite;

/* Ring-buffer timeline record for the Limit Ranntou effect. */
typedef struct BattleFxBigBrawlRecord {
    short actorIndex;
    unsigned char pad_02[0xe];
    short timelineFrame;
} BattleFxBigBrawlRecord;

/* --- battle_fx_limit_sd --- */
/* GPU textured-quad packet for the Limit SD effect: tag, command, corner x/y and source x/y. */
typedef struct BattleFxLimitStardustRayQuad {
    int tag;
    union {
        int command;
        struct { unsigned char pad_04[0x3]; char commandHigh; };
    };
    short x0;
    short y0;
    int clutWord;
    short x1;
    short srcY;
    int uvWord;
    short srcX;
    short y1;
    int tpageWord;
    short spanX;
    short spanY;
    int uvMask;
} BattleFxLimitStardustRayQuad;

/* Limit SD timeline slot: phase counter, jittered positions and a sequence index. */
typedef struct BattleFxLimitStardustRayTimeline {
    short ownerSlot;
    short phase;
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    unsigned char pad_0C[0x4];
    short spreadRadius;
    unsigned char pad_12[0x8];
    short sequenceIndex;
} BattleFxLimitStardustRayTimeline;

/* Effect-slot record (0x20 bytes) for the SD moving-model task: frame counter, position and speed. */
typedef struct BattleFxStardustRayMovingSlot {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    unsigned char pad_08[0x4];
    short rotZ;
    unsigned char pad_0E[0x2];
    short uniformScale;
    short speed;
} BattleFxStardustRayMovingSlot;

/* The task slot the Limit SD timeline fills in for each child task it starts: the position, then the child's own
 * rotation, radius and scale words (the moving model, the growing model and the sprite burst each read theirs). */
typedef struct BattleFxLimitStardustRayChildTask {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    short rotZ;
    unsigned char pad_0E[0x2];
    short radius;
    short scale;
} BattleFxLimitStardustRayChildTask;

/* Effect100 slot record for one SD spoke limit particle. */
typedef struct BattleFxStardustRaySpokeParticle {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
} BattleFxStardustRaySpokeParticle;

/* Seed record for an SD particle pool entry; particles are written at 0x18-byte stride. */
typedef struct BattleFxLimitStardustRayParticleSeed {
    short ownerSlot;
    short index;
    short posX;
    short posY;
    short posZ;
} BattleFxLimitStardustRayParticleSeed;

/* Limit SD burst sprite slot spawned with random spread around a base position. */
typedef struct BattleFxLimitStardustRayBurstSprite {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short radius;
} BattleFxLimitStardustRayBurstSprite;

/* Spoke render primitive for the SD limit. */
typedef struct BattleFxStardustRaySpokePrim {
    int tag;
    int nextPrim;
    int startXY;
    int endColor;
    int endXY;
} BattleFxStardustRaySpokePrim;

/* SD spawn timeline record with constant initial values. */
typedef struct BattleFxStardustRaySpawnRecord {
    short ownerSlot;
    unsigned char pad_02[0x2];
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    unsigned char pad_0C[0x4];
    short jitterRadius;
    unsigned char pad_12[0x8];
    short sequenceIndex;
} BattleFxStardustRaySpawnRecord;

/* SD sprite burst particle; position is a source actor's position plus random spread. */
typedef struct BattleFxStardustRaySpriteBurstRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short scale;
} BattleFxStardustRaySpriteBurstRecord;

/* Particle record of the Limit SD sprite pool, 0x18 bytes each. */
typedef struct BattleFxLimitStardustRayParticle {
    short active;
    short age;
    short posX;
    short posY;
    short posZ;
    short rotZ; /* the sprite pool's spin angle; the other pools leave it unused */
    short velX;
    short velY;
    short velZ;
    short accelY;
    union {
        short spokeLength; /* the spoke pool's spoke count */
        short rotStep; /* the sprite pool's spin step per frame */
    };
    short scale;
} BattleFxLimitStardustRayParticle;

/* Particle slot of the Limit SD textured-quad pool; a nonzero state marks a live slot. */
typedef struct BattleFxLimitStardustRayQuadRecord {
    short state;
    short variant; /* DrawTexturedQuad's texture choice (zero or not), set from the random-table index */
    BattleFxStardustRayCorner corner;
    unsigned char pad_08[0x10];
} BattleFxLimitStardustRayQuadRecord;

#pragma pack(push, 1)
typedef struct BattleLimitStardustDotTask {
    unsigned char pad_0[0xd0];
    short rotX; /* +0xd0 */
    short rotY; /* +0xd2 */
    short rotZ; /* +0xd4 */
    unsigned char pad_d6[0x8a];
    unsigned char matrix[0x1e]; /* +0x160 */
    int scaleX; /* +0x17e */
    int scaleY; /* +0x182 */
    int scaleZ; /* +0x186 */
    unsigned char pad_18a[0x2];
} BattleLimitStardustDotTask;
#pragma pack(pop)

/* --- battle_fx_limit_sld_fang --- */
/* Limit Sld Fang flat sprite task state: step, position and damped velocities. */
typedef struct BattleFxSledFangSprite {
    short ownerSlot;
    short step;
    short posX;
    short heightY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxSledFangSprite;

/* Sld fang alternate slash-trail timeline record: position, angle and lifetime. */
typedef struct BattleFxBloodFangRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    unsigned char pad_0E[0x2];
    short offsetY;
    short offsetZ;
    short trailIndex;
} BattleFxBloodFangRecord;

/* Sld Fang spawn record: stage counter and a base position for spawned sprites and particles. */
typedef struct BattleFxSledFangSpawnRecord {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
} BattleFxSledFangSpawnRecord;

/* Effect-slot record of the Sld Fang slash trail task: frame counter and trail ring index. */
typedef struct BattleFxFangSlashTrail {
    short slotTag;
    short frame;
    unsigned char pad_04[0x8];
    short baseValue;
    unsigned char pad_0E[0x2];
    short offsetY;
    short offsetZ;
    short trailIndex;
} BattleFxFangSlashTrail;

/* Effect-slot record of the Sld Fang alternate timeline task: step counter and target position. */
typedef struct BattleFxBloodFangTimeline {
    short slotTag;
    short step;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0xa];
    short nextTrailIndex;
} BattleFxBloodFangTimeline;

/* Per-slot state of the Sld fang timeline task, with a frame counter. */
typedef struct BattleFxLimitSledFangState {
    short ownerSlot;
    short frameIndex;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0xa];
    short nextRingIndex;
} BattleFxLimitSledFangState;

/* Per-slot state of the Sld fang impact task, which spawns particles at 0x18-byte stride. */
typedef struct BattleFxLimitSledFangImpact {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
} BattleFxLimitSledFangImpact;

/* Limit Sld fang trail slot filled from neighbouring slots. */
typedef struct BattleFxLimitSledFangTrailSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short baseAngle;
    unsigned char pad_0E[0x2];
    short offsetY;
    short offsetZ;
    short ringIndex;
} BattleFxLimitSledFangTrailSlot;

/* Limit Sld fang sprite slot positioned relative to the actor. */
typedef struct BattleFxLimitSledFangSpriteSlot {
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
} BattleFxLimitSledFangSpriteSlot;

/* Effect-table record for the Sald Fang particle draw task, with a frame index and particle count. */
typedef struct BattleFxSledFangParticle {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x8];
    short particleCount;
    unsigned char pad_0E[0x6];
    short firstParticleIndex;
} BattleFxSledFangParticle;

/* Task slot for the Limit Sld Fang timeline that receives a bone world position. */
typedef struct BattleLimitSledFangSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short boneWorldPos;
    unsigned char pad_06[0xe];
    short nextRingIndex;
} BattleLimitSledFangSlot;

/* Task-slot record for the Limit Sld Fang sprite and particle timeline. */
typedef struct BattleFxSledFangSprites {
    unsigned char pad_00[0xc];
    short particleCount;
    unsigned char pad_0E[0x6];
    short firstParticleIndex;
} BattleFxSledFangSprites;

/* Task-slot record for the alternate Limit Sld Fang timeline. */
typedef struct BattleFxBloodFangTask {
    short ownerSlot; /* +0x0 */
    unsigned char pad_2[0x2];
    short worldPos;
    unsigned char pad_06[0xe];
    short nextTrailIndex;
} BattleFxBloodFangTask;

/* One point of the Sword Fang trail, a midpoint computed from edge and mid points. */
typedef struct BattleLimitSledFangTrailPoint {
    short posX;
    short posZ;
    short posY;
    unsigned char pad_06[0x2];
} BattleLimitSledFangTrailPoint;

/* Bone record (0xC0 bytes) of the Limit Sld Fang effect. */
typedef struct BattleFxSledFangBoneRecord {
    unsigned char pad_00[0x2];
    short posY;
    unsigned char pad_04[0xbc];
} BattleFxSledFangBoneRecord;

/* PSX quad primitive for one segment of a Sld Fang trail. */
typedef struct BattleFxTrailSegmentQuadPrim {
    int colorCommand;
    union {
        int color0;
        struct {
            unsigned char color0Bytes[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxTrailSegmentQuadPrim;

/* Shared quad primitive for the Limit Sld Fang slash trail: draw colour, the tip colour, per-frame delta and base value. */
typedef struct BattleLimitSledFangQuad {
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char pad_03[0x1];
    union {
        int colorA;
        struct {
            unsigned char colorARed;
            unsigned char colorAGreen;
            unsigned char colorABlue;
            unsigned char colorAAlpha;
        };
    };
    int frameDelta;
    int baseValue;
} BattleLimitSledFangQuad;

/* Ring of the last 24 trail points of one Sld Fang slash: the write index, the tail, the count and
   the point addresses. */
typedef struct BattleLimitSledFangTrailRing {
    int battleLimitSldFangTrailPointIndex;
    int tailIndex;
    int headIndex;
    void *points[24];
} BattleLimitSledFangTrailRing;

/* One impact particle of the pool that starts at g_BattleLimitSledFangParticles (222 records). */
typedef struct BattleFxSledFangParticleRecord {
    short active;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short scale;
} BattleFxSledFangParticleRecord;

/* The scale of a particle record, reached through g_BattleLimitSledFangUnusedFlag, the symbol the
   code addresses the pool's +0x14 word by (the scale is its upper half). */
typedef struct BattleFxSledFangParticleScale {
    unsigned char pad_00[0x2];
    short scale;
    unsigned char pad_04[0x14];
} BattleFxSledFangParticleScale;

/* The scratch block Gfx_GetPsxScratchpadPtr(0) returns to the Sld Fang slash trail tasks: the edge points,
   the projected vertex slots, the rotation angles with the matrix and direction they build. */
#pragma pack(push, 1)
typedef struct BattleFxLimitSledFangTrail {
    unsigned char pad_00[0x4];
    int projUnused;
    int depth;
    int projFlags;
    short edgeAX;
    short originX;
    short edgeAY;
    unsigned char pad_16[0x2];
    short edgeBX;
    unsigned char pad_1A[0x2];
    short edgeBY;
    unsigned char pad_1E[0x2];
    short midX;
    short originY;
    short midY;
    unsigned char pad_26[0xa];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_36[0x2];
    short tipX;
    short tipY;
    short tipZ;
    unsigned char pad_3E[0x2];
    int valueA;
    int valueB;
    unsigned char pad_48[0x48];
    int prevValueA;
    int prevValueB;
    unsigned char pad_98[0x48];
    int prevDepth;
    int colorA;
    int quadA;
    int quadB;
    unsigned char pad_F0[0x10];
    short rotX;
    short trailIndex;
    short tableOffset;
    unsigned char pad_106[0x2];
    PsxMatrix rotMatrix;
    int dirX;
    int dirY;
    int dirZ;
    unsigned char pad_132[0x36];
    BattleVec4i vertexA;
    BattleVec4i vertexB;
    unsigned char pad_188[0x80];
    BattleVec4i prevVertexA;
    BattleVec4i prevVertexB;
} BattleFxLimitSledFangTrail;
#pragma pack(pop)

/* The first 0x14 bytes of the impact particle pool (BattleFxSledFangParticleRecord): the aggregate's
   extent is fixed, the code indexes past it. */
#pragma pack(push, 1)
typedef struct BattleLimitSledFangTrailRingActiveFlags {
    unsigned char pad_0[0x14];
} BattleLimitSledFangTrailRingActiveFlags;
#pragma pack(pop)

/* --- battle_fx_limit_toy --- */
/* Timeline state for the Toy limit: frame counter, with output and accumulator steps for globals. */
typedef struct BattleLimitToySoldierTimeline {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x6];
    short rotationY;
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
} BattleLimitToySoldierTimeline;

/* Limit toy spawn slot that moves in a circle around its origin. */
typedef struct BattleFxLimitToySoldierSpawnSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    short rotationY;
    short stepX;
    unsigned char pad_0E[0x2];
    short stepZ;
} BattleFxLimitToySoldierSpawnSlot;

#pragma pack(push, 1)
typedef struct BattleFxLimitToySoldierLocalMatrix {
    unsigned char pad_0[0x12];
    int matrixPhase; /* +0x12 */
    int matrixZeroWord; /* +0x16 */
    int matrixEmitCounter; /* +0x1a */
} BattleFxLimitToySoldierLocalMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxLimitToySoldierSpriteQuad {
    unsigned char pad_0[0x4];
    char animState; /* +0x4 */
    unsigned char pad_5[0xb];
    int sceneObject; /* +0x10 */
} BattleFxLimitToySoldierSpriteQuad;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxLimitToySoldierTransformMatrix {
    unsigned char pad_0[0x12];
    int matrixOffsetX; /* +0x12 */
    int matrixOffsetY; /* +0x16 */
    int matrixOffsetZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleFxLimitToySoldierTransformMatrix;
#pragma pack(pop)

/* --- battle_fx_limit_toybox --- */
/* Toybox-A limit effect slot: timer, accumulated phase and level with per-frame steps. */
typedef struct BattleFxToyBoxAnimSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short phase;
    short phaseStep;
    short yaw;
    short level;
    short levelStep;
    short radialOffset;
    short heightOffset;
} BattleFxToyBoxAnimSlot;

/* Effect100 slot for the Toybox shared draw-model task: step and an offset that advances each frame. */
typedef struct BattleFxToyBoxDrawModel {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short yaw;
    unsigned char pad_0E[0x4];
    short scale;
    unsigned char pad_14[0x2];
    short offset;
    short offsetStep;
} BattleFxToyBoxDrawModel;

/* Effect-slot record (0x20 bytes) for the Toybox A sprite task: frame counter, position and spread. */
typedef struct BattleFxToyBoxASprite {
    short state;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spreadCenter;
    short spreadHalfRange;
    unsigned char pad_10[0x2];
    short angle;
    short baseSpeed;
} BattleFxToyBoxASprite;

/* Effect-100 slot for the Toybox limit sprite emitter. */
typedef struct BattleFxLimitToyBoxSprite {
    short ownerSlot;
    short step;
    short posX;
    short heightY;
    short posY;
    unsigned char pad_0A[0x2];
    short randMax;
    short randMin;
    unsigned char pad_10[0x2];
    short spriteScale;
    short emitSpeed;
} BattleFxLimitToyBoxSprite;

/* Limit Toybox A effect slot; sizes and rates are derived from the frame parameters. */
typedef struct BattleFxLimitToyBoxASlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    Int64Pair worldPos; /* the centroid of the targets (three shorts) */
    short rollZOrRadius;
    short rollStepOrRange;
    short rotationY;
    short scale;
    short scaleStepOrSpeed;
    short radialOffset;
    short heightOffset;
} BattleFxLimitToyBoxASlot;

/* Effect100 slot record for a Toybox A sprite quad that drifts by its velocity each frame. */
typedef struct BattleFxToyBoxASpriteQuad {
    short ownerSlot;
    short frameIndex;
    short posX;
    short height;
    short posY;
    unsigned char pad_0A[0x8];
    short angle;
    unsigned char pad_14[0x2];
    short velocityX;
    short velocityY;
} BattleFxToyBoxASpriteQuad;

/* Effect100 slot record written by the Toybox B setup task, with scale and velocity. */
typedef struct BattleFxToyBoxBParticle {
    short ownerSlot;
    unsigned char pad_02[0x2];
    Int64Pair worldPos; /* the centroid of the targets (three shorts) */
    short spawnRadiusMax;
    short spawnRadiusMin;
    unsigned char pad_10[0x2];
    short scale;
    short emitSpeed;
    short velocityX;
    short velocityY;
} BattleFxToyBoxBParticle;

/* Effect100 slot record for a sprite emitted by the Toybox emitter task. */
typedef struct BattleFxToyBoxEmittedSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    unsigned char pad_06[0x2];
    short posY;
    unsigned char pad_0A[0x8];
    short angle;
    unsigned char pad_14[0x2];
    short velocityX;
    short velocityY;
} BattleFxToyBoxEmittedSprite;

/* Effect-table sprite emitted by the Limit Toybox task, positioned relative to a source record. */
typedef struct BattleFxToyBoxSprite {
    unsigned char pad_00[0x4];
    short posX;
    short height;
    short posY;
    unsigned char pad_0A[0x8];
    short scale;
    unsigned char pad_14[0x2];
    short velocityX;
    short velocityY;
} BattleFxToyBoxSprite;

#endif /* FF7_TYPES_BATTLE_LIMIT_H */

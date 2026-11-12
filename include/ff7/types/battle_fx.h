/* include/ff7/types/battle_fx.h: Shared battle effect, weapon trail and Enemy Skill (Blue Magic) types. */
#ifndef FF7_TYPES_BATTLE_FX_H
#define FF7_TYPES_BATTLE_FX_H

#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "lib/codegen.h"
#include "lib/types/graphics.h"

/* --- battle_effect_model_fade_draw --- */
/* Wrapper sub-record of a model bank, pointing at actor bone position and world matrix. */
typedef struct BattleFxModelFadeWrapper {
    unsigned char pad_00[0x6c];
    int boneWorldPosPtr;
    int worldMatrixPtr;
} BattleFxModelFadeWrapper;

/* --- battle_effect_program_ribbons --- */
/* Per-target tumble record for the actor knockback effect: velocities, gravity and target index. */
typedef struct BattleActorTumble {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    short velocityX;
    short velocityZ;
    short velocityY;
    short gravity;
    short targetIndex;
} BattleActorTumble;

/* Effect-100 slot for the Tifa limit program: step, bone-relative position and actor index. */
typedef struct BattleFxTifaLimitSlot {
    short ownerSlot;
    short step;
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            unsigned char pad_06[0x2];
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short effectRadius;
    unsigned char pad_0E[0x6];
    short actorIndex;
} BattleFxTifaLimitSlot;

/* Battle effect program 6 spawned-model record: position, angle, speed and fade level. */
typedef struct BattleFxProgram6Record {
    short ownerSlot;
    short step;
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short range;
    short stepSize;
    short angle;
    short speed;
    short typeIndex;
    short fadeLevel;
    short duration;
    short fadeStart;
} BattleFxProgram6Record;

/* Battle effect slot for an arcing model task: frame counter, amplitude and table index. */
typedef struct BattleFxArcingModel {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    short angleY;
    short angleX;
    short velocityY;
    short angleZ;
    short accelY;
    short amplitude;
    short angleXVelocity;
    short tableIndex;
    short angleZVelocity;
    short duration;
} BattleFxArcingModel;

/* Battle effect slot for an animated fade model: frame counter and fade start and end frames. */
typedef struct BattleFxAnimatedFadeModel {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    short accelY;
    short scale;
    unsigned char pad_16[0x2];
    short angleX;
    short angleXVelocity;
    short fadeEnd;
    short fadeStart;
} BattleFxAnimatedFadeModel;

/* Per-piece slot of effect program 7; position advances each frame by a per-frame step. */
typedef struct BattleFxProgram7Piece {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short posY;
            short posZ;
            short scaleY;
        };
    };
    short scaleOrSpeed;
    short scaleStep;
    short angleOrPull;
    short rotStepOrShade;
    short targetIndex;
    unsigned char pad_16[0x2];
    short fadeEndOrSlot;
    short fadeStartOrDur;
} BattleFxProgram7Piece;

/* Per-piece slot of effect program 7's arcing pieces, started at a jittered position. */
typedef struct BattleFxProgram7ArcPiece {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short posY;
            short posZ;
            short driftOrAngleY;
        };
    };
    short driftXOrAngleX;
    short driftYOrVelY;
    short driftZOrAngleZ;
    short accelerationY;
    short angleOrScale;
    short angleStep;
    short spinOrVariant;
    short spinStep;
    short lifetime;
    short driftOrFadeStart;
} BattleFxProgram7ArcPiece;

/* Ribbon effect record: start and end points per axis and the interpolated position. */
typedef struct BattleEffectRibbonRecord {
    unsigned char pad_00[0x34];
    short attachOffsetX;
    short attachOffsetY;
    short attachOffsetZ;
    unsigned char pad_3A[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_42[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_4A[0x2];
    short startX;
    short startY;
    short startZ;
} BattleEffectRibbonRecord;

/* Battle effect slot for the converging streak: position pulled toward a target with decelerating speed. */
typedef struct BattleFxConvergingStreak {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short speed;
    short deceleration;
    short attraction;
    short colorCounter;
    unsigned char pad_14[0x4];
    short prevDistance;
    short distance;
} BattleFxConvergingStreak;

/* Effect slot for the random rotating triangle: per-axis amplitudes, drift, angles and lifetime. */
typedef struct BattleFxRotatingTriangle {
    short ownerSlot;
    short frame;
    short ampX;
    short ampY;
    short ampZ;
    short driftScale;
    short driftX;
    short driftY;
    short driftZ;
    unsigned char pad_12[0x2];
    short angle;
    short angleStep;
    short spin;
    short spinStep;
    short lifetime;
    short driftEnabled;
} BattleFxRotatingTriangle;

/* Actor orbit task slot: phase counter and an actor index that selects the per-actor target records. */
typedef struct BattleFxActorOrbit {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x8];
    short rotStepX;
    unsigned char pad_0E[0x6];
    short actorIndex;
} BattleFxActorOrbit;

/* Effect-array record set up by Battle_EffectProgram3Step; several fields are jittered with rand(). */
typedef struct BattleFxProgram3Record {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short speed;
    short radialSpeed;
    short startFrame;
    short emitCount;
    unsigned char pad_14[0x4];
    short mode;
    short phase;
} BattleFxProgram3Record;

/* Effect-array record for a spinning, growing, fading model: timer, fade window, scale and angle. */
typedef struct BattleFxSpinGrowFadeRecord {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    short growRate;
    short angle;
    short spinRate;
    unsigned char pad_14[0x2];
    short drawAttribute;
    short fadeEnd;
    short fadeStart;
} BattleFxSpinGrowFadeRecord;

/* Limit Tifa particle: position around the actor's bone and a fixed lifetime in one Effect100 slot. */
typedef struct BattleFxTifaParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short speed;
    short verticalSpeed;
    unsigned char pad_14[0x4];
    short lifetime;
    short stepTimer;
} BattleFxTifaParticle;

/* Tifa limit particle with position and velocities, set by Battle_EffectProgram5Step. */
typedef struct BattleFxLimitTifaFadeParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    short accelY;
    short scale;
    unsigned char pad_16[0x2];
    short angle;
    short angularVelocity;
    short lifetime;
    short phase;
} BattleFxLimitTifaFadeParticle;

/* Spinning stretch fade model: timer, spin, stretch and fade window. */
typedef struct BattleFxSpinStretchFade {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    short scaleY;
    short scaleXZ;
    short scaleXZStep;
    short rotY;
    short rotYStep;
    unsigned char pad_14[0x4];
    short fadeEnd;
    short fadeStart;
} BattleFxSpinStretchFade;

/* Effect-100 slot record (0x20 stride) for the growing, stretching, fading model task. */
typedef struct BattleFxStretchFadeSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scaleXZ;
    short scaleXZVelocity;
    short scaleY;
    short scaleYVelocity;
    unsigned char pad_14[0x4];
    short fadeEnd;
    short fadeStart;
} BattleFxStretchFadeSlot;

/* Oscillating disc effect record: timer, position swayed by a sine of angle, and velocity. */
typedef struct BattleFxOscillatingDiscRecord {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x8];
    short position;
    short velocity;
    short angle;
} BattleFxOscillatingDiscRecord;

/* Decaying fade model effect slot: timer, phase with decaying step, fade start and end frames. */
typedef struct BattleFxDecayingFadeModel {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    short scaleStep;
    unsigned char pad_10[0x2];
    short decayDivisor;
    unsigned char pad_14[0x4];
    short fadeEnd;
    short fadeStart;
} BattleFxDecayingFadeModel;

/* Growing fade model effect slot: timer, phase with step, and fade start and end frames. */
typedef struct BattleFxGrowingFadeModel {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    short scaleStep;
    short angleY;
    unsigned char pad_12[0x6];
    short fadeEnd;
    short fadeStart;
} BattleFxGrowingFadeModel;

/* Ribbon effect state block: two eased current values pulled toward their targets. */
typedef struct BattleFxRibbonState {
    unsigned char pad_00[0x34];
    short originX;
    short originY;
    short originZ;
    unsigned char pad_3A[0xa];
    short currentA;
    short posY;
    short currentB;
    unsigned char pad_4A[0x2];
    short targetA;
    short targetY;
    short targetB;
} BattleFxRibbonState;

/* Battle effect program 4 slot: timeline step and the battler index it works on. */
typedef struct BattleFxEffectProgram {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x8];
    short offset;
    unsigned char pad_0E[0x6];
    short battlerIndex;
} BattleFxEffectProgram;

/* Battle effect program 2 record: step, clamped height value and actor index. */
typedef struct BattleFxProgram2Effect {
    short ownerSlot;
    short step;
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short peakValue;
            unsigned char pad_08[0x4];
        };
    };
    unsigned char pad_0C[0x8];
    short actorIndex;
} BattleFxProgram2Effect;

/* Battle effect program 3 expanding fade ring: radius growth, start and end frames for the fade. */
typedef struct BattleFxExpandingFadeRing {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short radius;
    short radiusSpeed;
    unsigned char pad_10[0x8];
    short endFrame;
    short startFrame;
} BattleFxExpandingFadeRing;


/* GPU gradient quad packet written into the render state block: tag word and command byte. */
typedef struct BattleGpuGradientQuad {
    int tag;
    char vertex0Red;
    char vertex0Green;
    char vertex0Blue;
    char command;
    unsigned char pad_08[0x4];
    char vertex1Red;
    char vertex1Green;
    char vertex1Blue;
    unsigned char pad_0F[0x5];
    char vertex2Red;
    char vertex2Green;
    char vertex2Blue;
    unsigned char pad_17[0x5];
    char vertex3Red;
    char vertex3Green;
    char vertex3Blue;
} BattleGpuGradientQuad;

/* Random rotating-triangle effect state set up by Battle_EffectProgram2Step. */
typedef struct BattleEffectRotatingTriangle {
    unsigned char pad_00[0x4];
    short axisX;
    short yComp;
    short dirZ;
    short size;
    short rotatedX;
    short rotatedY;
    short vectorZ;
    unsigned char pad_12[0x2];
    short counter;
    short counterReload;
    short counterB;
    short counterBReload;
    short lifetime;
    short driftEnabled;
} BattleEffectRotatingTriangle;

/* Effect100 slot for the Tifa ribbon trail: frame counter, ribbon segment index and duration. */
typedef struct BattleFxTifaRibbonTrail {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x14];
    short ribbonIndex;
    short duration;
} BattleFxTifaRibbonTrail;

/* Effect-100 slot shared by the Battle effect program 2 sprite, particle and ribbon tasks. */
typedef struct BattleFxProgram2Particle {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short scale;
    short speed;
    short startFrame;
    short emitCount;
    unsigned char pad_14[0x4];
    short mode;
    short phase;
} BattleFxProgram2Particle;

/* Effect-100 slot for the Battle effect program 3 step. */
typedef struct BattleFxProgram3Slot {
    short ownerSlot;
    short step;
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short offsetY;
            unsigned char pad_08[0x4];
        };
    };
    unsigned char pad_0C[0x8];
    short targetIndex;
} BattleFxProgram3Slot;

/* Battle effect program 4 particle record: spawn position jitter around the sound position. */
typedef struct BattleFxProgram4Particle {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short currentValue;
    short targetValue;
    unsigned char pad_10[0x4];
    short battlerIndex;
    unsigned char pad_16[0x2];
    short timer;
    short fadeStart;
} BattleFxProgram4Particle;

/* Tifa limit-break effect slot: timer, relative position and the target actor index. */
typedef struct BattleLimitTifaEffectSlot {
    short ownerSlot;
    short timer;
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    unsigned char pad_0C[0x4];
    short riseStepY;
    short fallStepY;
    short actorIndex;
} BattleLimitTifaEffectSlot;

/* Effect-slot record of the particle emitter task: frame counter, end frame and emit count. */
typedef struct BattleFxParticleEmitter {
    short slotTag;
    short frame;
    union {
        Int64Pair pos; /* +0x04: the position, copied whole */
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short baseScale;
    short speed;
    short startFrame;
    short emitCount;
    unsigned char pad_14[0x4];
    short endFrame;
    short lifetime;
} BattleFxParticleEmitter;

/* Effect-slot record of the aligned model task: frame counter, target index and end frame. */
typedef struct BattleFxAlignedModelSlot {
    short slotTag;
    short frame;
    unsigned char pad_04[0x8];
    short range;
    unsigned char pad_0E[0x6];
    short targetIndex;
    unsigned char pad_16[0x2];
    short endFrame;
} BattleFxAlignedModelSlot;

/* Effect100 slot record for one ribbon step, jittered on each update. */
typedef struct BattleFxRibbonStep {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    unsigned char pad_0E[0xa];
    short segmentCount;
    short sideCount;
} BattleFxRibbonStep;

/* Data block of effect program step 1, with the step values at 0x34 to 0x38. */
typedef struct BattleEffectProgram1Data {
    unsigned char pad_00[0x34];
    short offsetX;
    short offsetY;
    short offsetZ;
} BattleEffectProgram1Data;

/* Per-slot state of the program-2 ribbon effect, with a frame counter and a frame limit. */
typedef struct BattleFxProgram2RibbonState {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x16];
    short frameCount;
} BattleFxProgram2RibbonState;

/* Program animated sprite slot A that counts frames and installs a billboard. */
typedef struct BattleFxProgramSpriteSlotA {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    unsigned char pad_0E[0xa];
    short spriteScale;
} BattleFxProgramSpriteSlotA;

/* Header of the effect-program data block; fields at +0x34 to +0x3E are reset or read by the program. */
typedef struct BattleEffectProgram2Chunk {
    unsigned char pad_00[0x34];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_3A[0x4];
    short peakValue;
} BattleEffectProgram2Chunk;

/* Task slot record for a scaled billboard model; steps through a quad colour table. */
typedef struct BattleScaledBillboardRecord {
    short ownerSlot;
    short colorIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
} BattleScaledBillboardRecord;

/* Task slot that runs one effect program step, holding its target actor and program index. */
typedef struct BattleEffectProgramSlot {
    unsigned char pad_00[0x14];
    short targetActor;
    short programIndex;
    short clearedState;
} BattleEffectProgramSlot;


/* Ring-buffer record written by the effect program 7 step. */
typedef struct BattleEffectProgram7Record {
    unsigned char pad_00[0xc];
    int clearedWord;
    short messageParam;
} BattleEffectProgram7Record;

/* Trail section quad packet: tag word with code 0x8 and four corner words. */
typedef struct BattleEffectTrailQuad {
    int tag;
    union {
        int cornerA;
        struct {
            unsigned char cornerARgb[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    int cornerB;
    unsigned char pad_10[0x4];
    int cornerC;
    unsigned char pad_18[0x4];
    int cornerD;
} BattleEffectTrailQuad;

/* PSX primitive for one gradient quad of a ring effect, with angle and colour words. */
typedef struct BattleGradientRingPrim {
    int tag;
    union {
        int angleA;
        struct {
            unsigned char angleABytes[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    int angleB;
    unsigned char pad_10[0x4];
    int colorA;
    unsigned char pad_18[0x4];
    int colorB;
} BattleGradientRingPrim;

/* GPU gradient triangle-fan packet: tag word, angle and colour, from Gfx_GetPsxPacketScratchPtr. */
typedef struct BattleGradientFanPacket {
    int tag;
    union {
        int angle;
        struct {
            unsigned char angleBytes[3];
            unsigned char code;
        };
    };
    unsigned char pad_08[0x4];
    int color;
    unsigned char pad_10[0x4];
    int color2;
} BattleGradientFanPacket;

/* Three unsigned short vertex indices of a Gouraud triangle. */
typedef struct BattleFxGouraudIndices {
    unsigned short vertexA;
    unsigned short vertexB;
    unsigned short vertexC;
} BattleFxGouraudIndices;

#pragma pack(push, 1)
typedef struct BattleLimitTifaParamVector1 {
    int tableEntry; /* +0x0 */
    int resource; /* +0x4 */
    unsigned char pad_8[0x4];
    short frameIndex; /* +0xc */
    short scale; /* +0xe */
} BattleLimitTifaParamVector1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitTifaQuadColors3 {
    unsigned char pad_0[0x8];
    char fadeHistoryOldest; /* +0x8 */
    char fadeHistoryMiddle; /* +0x9 */
    char fadeHistoryNewest; /* +0xa */
    unsigned char pad_b[0x5];
} BattleLimitTifaQuadColors3;
#pragma pack(pop)

/* --- battle_effect_trail_ribbon --- */
/* Battle effect trail segment: two projected vertices (the second also the easing target) with a packed
 * 24-bit colour, chained to the next segment. */
typedef struct BattleEffectTrailSegment {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0x2];
    int color;
    int fadeStep;
    struct BattleEffectTrailSegment *nextSegment;
} BattleEffectTrailSegment;

/* Trail effect task: attached actor and point, age, lifetime and the newest segment. */
typedef struct BattleFxEffectTrail {
    short alpha;
    short age;
    int actorIndex;
    int attachIndex;
    BattleEffectTrailSegment *tailNode;
    int colorWord;
    int colorStep;
    short yOffset;
    short lifetime;
} BattleFxEffectTrail;

/* Sampled trail point: the attach position and the offset vertex projected from it. */
typedef struct BattleEffectTrailSample {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
    short trailX;
    short trailY;
    short yOffset;
} BattleEffectTrailSample;

/* Gouraud quad packet: tag, then a colour word and an xy word per vertex. */
typedef struct BattleEffectTrailQuadPrim {
    int tag;
    int color0;
    int xy0;
    int color1;
    int xy1;
    int color2;
    int xy2;
    int color3;
    int xy3;
} BattleEffectTrailQuadPrim;

/* Output of the trail vertex projection: the low shorts of three ints. */
typedef struct BattleEffectTrailSeed {
    short trailX;
    unsigned char pad_02[0x2];
    short trailY;
    unsigned char pad_06[0x2];
    short yOffset;
    unsigned char pad_0A[0x6];
} BattleEffectTrailSeed;

/* The data block of the trail task: the attach transform, the projection output and the sample. */
#pragma pack(push, 1)
typedef struct BattleEffectTrailBlock {
    PsxMatrix transform;
    BattleEffectTrailSeed seed;
    BattleEffectTrailSample trailSample;
} BattleEffectTrailBlock;
#pragma pack(pop)

/* The previous trail point (0x10 bytes) kept per ring index: position, then the offset vertex. */
typedef struct BattleEffectTrailPoint {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    short trailX;
    short trailY;
    short yOffset;
    unsigned char pad_0E[0x2];
} BattleEffectTrailPoint;

/* --- battle_fx_blue_aleiz_effect --- */
/* Arcing quad sprite task record: frame, position and velocity, acceleration and angle. */
typedef struct BattleFxArcingQuadSprite {
    short endFlag;
    short frame;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x2];
    short accelB;
} BattleFxArcingQuadSprite;

/* Blue Aleiz effect slot: jitter offsets and rising velocity for the fade, sprite and ring tasks. */
typedef struct BattleFxBlueAngelWhisperRecord {
    unsigned char pad_00[0x4];
    short jitterA;
    short velocity;
    short jitterB;
    unsigned char pad_0A[0x2];
    short startValue;
    short amplitude;
    short spread;
    short duration;
    short delay;
} BattleFxBlueAngelWhisperRecord;

/* Blue Aleiz effect record: position with per-frame random jitter. */
typedef struct BattleFxAngelWhisperRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short baseScale;
    short scale;
    short scaleStep;
    short velocity;
    short accel;
} BattleFxAngelWhisperRecord;

/* Aleiz arcing quad sprite spawner record: arc height and per-step arc change. */
typedef struct BattleFxAngelWhisperArcingQuad {
    unsigned char pad_00[0x4];
    short originX;
    short arcHeight;
    short originY;
    unsigned char pad_0A[0x2];
    short targetX;
    short velocity;
    short targetY;
    unsigned char pad_12[0x2];
    short scale;
    unsigned char pad_16[0x2];
    short arcStep;
} BattleFxAngelWhisperArcingQuad;

/* Blue Aleiz timeline record with a step counter. */
typedef struct BattleFxBlueAngelWhisperRecordA {
    short ownerSlot;
    short step;
    Int64Pair pos;
    short baseScale;
    short elapsed;
    short isMultiTarget;
} BattleFxBlueAngelWhisperRecordA;

/* Blue Aleiz timeline record with a step counter. */
typedef struct BattleFxBlueAngelWhisperRecordB {
    short ownerSlot;
    short step;
    Int64Pair pos;
    short startValue;
    short elapsed;
    short isMultiTarget;
} BattleFxBlueAngelWhisperRecordB;

/* Blue Aleiz timeline task slot: step index, a parameter forwarded to the model, and a counter delta. */
typedef struct BattleFxBlueAngelWhisperTimeline {
    short ownerSlot;
    short step;
    Int64Pair pos;
    short param;
    unsigned char pad_0E[0x6];
    short counterDelta;
} BattleFxBlueAngelWhisperTimeline;

/* Blue Aleiz spawner: base position, spread range and step counter for descending quad sprites. */
typedef struct BattleFxBlueAngelWhisperSpawner {
    short ownerSlot;
    short step;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x4];
    short spreadOffset;
    short spreadRange;
} BattleFxBlueAngelWhisperSpawner;

/* Battle effect task slot (0x20 bytes) for the Blue Aleiz timelines. */
typedef struct BattleFxBlueAngelWhisperTaskSlot {
    short ownerSlot;
    unsigned char pad_02[0x4];
    short posY;
    unsigned char pad_08[0x4];
    short yaw;
    short duration;
    short isActive;
} BattleFxBlueAngelWhisperTaskSlot;

/* Timeline task slot for the Blue Aleiz ring effect; step is its step counter. */
typedef struct BattleFxBlueAngelWhisperRingTaskSlot {
    short ownerSlot;
    short step;
    Int64Pair pos;
    short yaw;
    unsigned char pad_0E[0x6];
    short velocityY;
} BattleFxBlueAngelWhisperRingTaskSlot;

/* Task-slot record for the blue Aleiz descending quad sprite. */
typedef struct BattleFxAngelWhisperQuadTask {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short scale;
    unsigned char pad_10[0x2];
    short velocityY;
} BattleFxAngelWhisperQuadTask;

/* Blue Aleiz arcing quad sprite task state with a frame counter. */
typedef struct BattleFxAngelWhisperArcSprite {
    short ownerSlot;
    short frame;
    Int64Pair pos;
    unsigned char pad_0C[0x2];
    short radius;
} BattleFxAngelWhisperArcSprite;

/* The battle task slot of BattleFx_BlueAngelWhisperExpandingFadeModelTask. */
typedef struct BattleFxAngelWhisperExpandingFadeModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short scaleXZ;
    short scaleXZStep;
    short scaleY;
    short scaleYStep;
} BattleFxAngelWhisperExpandingFadeModelSlot;

/* Slots BattleFx_BlueAngelWhisperQuadSpritesSetupTask allocates for BattleFx_BlueAngelWhisperQuadSprites{SetupTask, TimelineTask}. */
typedef struct BattleFxAngelWhisperSetupSpawnSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short yaw;
    unsigned char pad_0E[0x6];
    short heightStep;
} BattleFxAngelWhisperSetupSpawnSlot;

/* Descending quad sprite task slot; spawn position is randomised around the target. */
typedef struct BattleFxDescendingQuadSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short scale;
    unsigned char pad_10[0x2];
    short spread;
} BattleFxDescendingQuadSprite;

/* Battle task slot for the Aleiz effect, with two short fields set from a frame value. */
typedef struct BattleAngelWhisperTaskSlot {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    unsigned char pad_0C[0x2];
    short spreadRadius;
    short spreadDiameter;
} BattleAngelWhisperTaskSlot;

/* Saved tint of one actor during the blue Aleiz fade. */
#pragma pack(push, 1)
typedef struct {
    short tintPhase;
    short tintPulse;
    char tintR;
    char tintG;
    char tintB;
    unsigned char pad_07[0x1];
} BattleBlueAleizSavedTint;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueAngelWhisperColor1 {
    char aleizColor1Byte1; /* +0x0 */
    char channelByte; /* +0x1 */
    char aleizColor1Byte2; /* +0x2 */
    unsigned char pad_3[0x1];
} BattleBlueAngelWhisperColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueAngelWhisperSpriteDrawReq1 {
    unsigned char pad_0[0x4];
    char arcSpriteRowByte; /* +0x4 */
    unsigned char pad_5[0x9];
    short arcDrawAttrWord; /* +0xe */
    int arcDrawObject; /* +0x10 */
} BattleBlueAngelWhisperSpriteDrawReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueAngelWhisperSpriteDrawReq2 {
    unsigned char pad_0[0x4];
    char descentSpriteRowByte; /* +0x4 */
    unsigned char pad_5[0x9];
    short descentDrawAttrWord; /* +0xe */
    int descentDrawObject; /* +0x10 */
    unsigned char pad_14[0x4];
    int risingFadeRecord; /* +0x18 */
    int risingFadeObject; /* +0x1c */
} BattleBlueAngelWhisperSpriteDrawReq2;
#pragma pack(pop)

/* The ring-buffer record Battle_AllocSceneMessage returns: a position and a yaw. */
typedef struct BattleFxAngelWhisperRingRecord {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short yaw;
} BattleFxAngelWhisperRingRecord;

/* --- battle_fx_blue_aqualung_effect --- */
/* Aqua particle timeline: frame counter, spread ranges and Y offset. */
typedef struct BattleFxAqualungTimeline {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short offsetX;
    short spreadX;
    short offsetY;
    short spreadY;
    short particleScale;
} BattleFxAqualungTimeline;

/* One bubble of the Aqualung ring: position, the size it returns to, its current size and the
 * per-frame growth and shrink rates. */
typedef struct BattleFxAqualungParticle {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    short baseSize;
    short size;
    short growRate;
    short shrinkRate;
} BattleFxAqualungParticle;

/* g_BattleBlueAqualungParticleCount: the index of the next free bubble, then the ring of bubbles (0xDE
 * of them, running on past the end of this object into the pad globals after it). */
typedef struct BattleFxAqualungRing {
    int nextIndex;
    unsigned char pad_04[0x4];
    BattleFxAqualungParticle particles[1];
    unsigned char pad_18[0x4];
} BattleFxAqualungRing;

/* Task slot of the bubble sprite task: the first ring entry it animates. */
typedef struct BattleFxAqualungSpriteTask {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x14];
    short firstParticle;
} BattleFxAqualungSpriteTask;

/* --- battle_fx_blue_bad_breath_effect --- */
/* Kusai particle record: base position, jitter ranges and scale for particle spawns. */
typedef struct BattleFxBadBreathRecord {
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
} BattleFxBadBreathRecord;

/* Particle record (0x10 bytes) of the Blue Kusai battle effect. */
typedef struct BattleFxBlueBadBreathParticle {
    short timeline;
    unsigned char pad_02[0xe];
} BattleFxBlueBadBreathParticle;

#pragma pack(push, 1)
typedef struct BattleBlueBadBreathParticleCountObject {
    unsigned char pad_0[0xa];
    int kusaiParticlePosY; /* +0xa */
    unsigned char pad_e[0x2];
} BattleBlueBadBreathParticleCountObject;
#pragma pack(pop)

/* --- battle_fx_blue_beta_effect --- */
/* Blue Beata timeline task slot: step index and a fixed-point ratio. */
typedef struct BattleFxBlueBetaTimeline {
    short ownerSlot;
    short step;
    short centroid[4]; /* the targets' centroid: x, y, z and a pad word, copied to the model tasks */
    unsigned char pad_0c[0x4];
    short stretchBase;
} BattleFxBlueBetaTimeline;

/* Timeline record for the Blue Beata effect, with values taken from a sibling record's ratio. */
typedef struct BattleFxBlueBetaTimeline2 {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0a[0x2];
    short spinAngle;
    short offset;
    short scale;
    short nextIndex;
} BattleFxBlueBetaTimeline2;

/* Particle field task state for the blue Beata effect. */
typedef struct BattleFxBetaParticleField {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0xe];
    short nextIndex;
} BattleFxBetaParticleField;

/* The battle task slot of BattleFx_BlueBetaSpinStretchFadeModelTask. */
typedef struct BattleFxBetaSpinStretchFadeModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinAngle;
    short spinSpeed;
    short stretch;
} BattleFxBetaSpinStretchFadeModelSlot;

/* Primitive packet for one Blue Beata particle-trail triangle, written in place. */
typedef struct BattleBlueBetaTrailPrim {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char code;
    int point0;
    char color1R;
    char color1G;
    char color1B;
    unsigned char pad_0F[0x1];
    int point0Shifted;
    char color2R;
    char color2G;
    char color2B;
    unsigned char pad_17[0x1];
    int point1;
} BattleBlueBetaTrailPrim;

/* Blue Beata particle offset (16 bytes): offset vector and per-frame step vector. */
typedef struct BattleBlueBetaOffsetPoint {
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_06[0x2];
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_0E[0x2];
} BattleBlueBetaOffsetPoint;

/* Quad of a blue Beata particle trail: colour bytes, flags and four packed vertex words. */
typedef struct BattleFxTrailQuad {
    int colorCommand;
    char colorR;
    char colorG;
    char colorB;
    char flags;
    int vertex0;
    int vertex1;
    int vertex2;
    int vertex3;
} BattleFxTrailQuad;

/* The centroid of the targets, from Battle_GetActorMaskCentroid. */
typedef struct BattleBlueBetaCentroid {
    short x;
    short y;
    short z;
    short pad;
} BattleBlueBetaCentroid;

/* --- battle_fx_blue_chyobo_effect --- */
/* Task slot for a bouncing billboard sprite: position, velocity, gravity and spin per frame. */
typedef struct BattleFxBillboardBounce {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    short angleStep;
    short scale;
    short phaseSlot;
    short gravity;
} BattleFxBillboardBounce;

/* Blue Chyobo bouncing-sprite spawn record: start and end positions and vertical velocity. */
typedef struct BattleFxBlueChocobuckleBounce {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos;
        struct { short startX; short startY; short startZ; unsigned char pad_0A[0x2]; };
    };
    short endX;
    short velocityY;
    short endZ;
    unsigned char pad_12[0x2];
    short angle;
    short bounceVelocity;
    short scale;
    unsigned char pad_1A[0x2];
    short accelY;
} BattleFxBlueChocobuckleBounce;

/* Blue Chyobo timeline task slot: step index, a parameter forwarded to the model, and a counter delta. */
typedef struct BattleFxBlueChocobuckleTimeline {
    short ownerSlot;
    short step;
    Int64Pair pos;
    short yaw;
    unsigned char pad_0E[0x8];
    short counterDelta;
} BattleFxBlueChocobuckleTimeline;

/* Task-slot record for the blue chocobo scatter sprite: position, velocity and frame count. */
typedef struct BattleFxBlueScatterSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxBlueScatterSprite;

/* Blue chyobo bouncing sprite slot in the battle task buffer, with spawn timing. */
typedef struct BattleFxChocobuckleSpriteSlot {
    short ownerSlot;
    short timer;
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short baseOffset;
} BattleFxChocobuckleSpriteSlot;

/* Blue chyobo scatter sprite task slot record. */
typedef struct BattleFxChocobuckleScatterSprite {
    short ownerSlot;
    short timer;
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short baseOffset;
    short scaleBase;
} BattleFxChocobuckleScatterSprite;

/* Blue chyobo bouncing billboard sprite task slot record. */
typedef struct BattleFxChocobuckleBillboardSpawner {
    short ownerSlot;
    short bounceCount;
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short speedBase;
    short scaleBase;
} BattleFxChocobuckleBillboardSpawner;

/* Blue Chyobo bouncing billboard sprite spawned with a random velocity and timer. */
typedef struct BattleFxChocobuckleBillboardSprite {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
    short phaseSlot;
    short gravity;
} BattleFxChocobuckleBillboardSprite;

/* The target bone's world position (short x, y, z) in the blue chyobo effect. */
typedef struct BattleFxChocobuckleTargetPos {
    short posX;
    short posY;
    short posZ;
} BattleFxChocobuckleTargetPos;

/* Blue Chyobo sprite color (r, g, b, code) with the two resource words the draw request reads after it. */
typedef struct {
    char r;
    char g;
    char b;
    char code;
    unsigned char pad_04[0x4];
    struct BattleFxSpriteGroup *spriteGroup;
    int spriteDrawObj;
} BattleBlueChyoboColorStateAlt;

/* The battle task slot of BattleFx_BlueChocobuckleBouncingSpriteTask. */
typedef struct BattleFxChocobuckleBouncingSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short bounceOffset;
    short bounceVelocity;
    short scale;
    unsigned char pad_1A[0x2];
    short accelY;
} BattleFxChocobuckleBouncingSpriteSlot;

/* The battle task slot of BattleFx_BlueChocobuckleOrbitSpriteTask. */
typedef struct BattleFxChocobuckleOrbitSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleStep;
    short scale;
    unsigned char pad_12[0x2];
    short orbitRadius;
} BattleFxChocobuckleOrbitSpriteSlot;

/* The battle task slot of BattleFx_BlueChocobuckleAltTimelineTask. */
typedef struct BattleFxChocobuckleAltTimelineSlot {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    unsigned char pad_0C[0x8];
    short yaw;
    unsigned char pad_16[0x2];
    short value;
    short posYStep;
    short state;
    short frameCount;
} BattleFxChocobuckleAltTimelineSlot;

/* Slots BattleFx_BlueChocobuckleSetupTask allocates for BattleFx_BlueChyobo{Setup, Timeline}Task. */
typedef struct BattleFxChocobuckleSetupSpawnSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    short yaw;
    unsigned char pad_0E[0x8];
    short actorYStep;
} BattleFxChocobuckleSetupSpawnSlot;

/* Battle task slot that spawns blue Chocobo scatter sprites with initial values. */
typedef struct BattleFxBlueChocobuckleSpawnTask {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxBlueChocobuckleSpawnTask;

/* Task slot that counts frames and spawns the blue chyobo orbit sprites. */
typedef struct BattleFxChocobuckleSpawnSlot {
    short ownerSlot; /* +0x0 */
    short frameCount;
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    union {
        short orbitRadius; /* the orbit spawner's radius */
        short baseOffset; /* the bouncing spawner's base distance */
    };
} BattleFxChocobuckleSpawnSlot;

/* Orbit sprite task slot spawned by BattleFx_BlueChocobuckleSpawnOrbitSpritesTask. */
typedef struct BattleFxChocobuckleOrbitSprite {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short angle;
    short angleStep;
    short scale;
    unsigned char pad_12[0x2];
    short orbitRadius;
} BattleFxChocobuckleOrbitSprite;

/* Per-frame position, velocity and state record of the blue chyobo alternate timeline task. */
typedef struct BattleFxBlueChocobuckleAltTrack {
    short ownerSlot;
    short timer;
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    short velocityX;
    short velocityY;
    short velocityZ;
    short accelerationY;
    short yaw;
    unsigned char pad_16[0x2];
    short value;
    short posYStep;
    short state;
    short frameCount;
} BattleFxBlueChocobuckleAltTrack;

/* Record kept in frame.f for the Blue Chyobo alt timeline; stepRate is set to fixed constants. */
typedef struct BattleFxBlueChocobuckleAltRecord {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short value;
    short stepRate;
} BattleFxBlueChocobuckleAltRecord;

/* The ring-buffer record Battle_AllocSceneMessage returns: a position and a yaw. */
typedef struct BattleFxChocobuckleRingRecord {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short yaw;
} BattleFxChocobuckleRingRecord;

/* Blue Chyobo bouncing-sprite color: r, g, b and a code byte. */
typedef struct {
    char r;
    char g;
    char b;
    char code;
} BattleBlueChyoboColorBytes;

/* Sprite draw request for the blue chyobo scatter and bouncing billboard tasks, filled from table-loaded resources. */
typedef struct {
    int scatterHandle;
    int scatterResource;
    char red;
    char green;
    char blue;
    char code;
    short frameCount;
    unsigned char pad_0E[0x2];
    int billboardHandle;
    int billboardResource;
} BattleFxBlueChyoboSpriteReq;

/* --- battle_fx_blue_death_force_effect --- */
/* Task slot shared by the Death Force effect's tasks. The setup task stores the target bone's world
 * position and the effect radius; the timeline copies the position into the spinning-sparks task,
 * which copies it into each animated sprite. The word at +0x18 is the effect radius in the
 * timeline, the model scale in the sparks task and the sprite's magnitude in the sprites. */
typedef struct BattleFxDeathForceTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleSpeed;
    short tilt;
    short tiltSpeed;
    short turn;
    short turnSpeed;
    union {
        short radius;
        short modelScale;
        short magnitude;
    };
    short sparkRadius;
} BattleFxDeathForceTask;

/* Blue Dethfo wave-strip tile, copied into quad records. The primitive's colour word is also read
 * as bytes, the texture word as the u/v pair, and the dimension word as width and height. */
typedef struct BattleFxWaveTile {
    int command;
    union {
        int color;
        struct {
            char colorR;
            char colorG;
            char colorB;
            char primCode;
        };
    };
    short x;
    short row;
    union {
        int texture;
        struct {
            unsigned char u;
            unsigned char v;
            unsigned char pad_0E[0x2];
        };
    };
    union {
        int dims;
        struct {
            short width;
            short height;
        };
    };
} BattleFxWaveTile;

/* Textured quad (four x/y corners and u/v pairs) written for a wave-strip draw. */
typedef struct BattleFxWaveStripQuad {
    int command;
    char colorR;
    char colorG;
    char colorB;
    unsigned char flags;
    short x0;
    short y0;
    char u0;
    char v0;
    unsigned char pad_0E[0x2];
    short x1;
    short y1;
    char u1;
    char v1;
    unsigned char pad_16[0x2];
    short x2;
    short y2;
    char u2;
    char v2;
    unsigned char pad_1E[0x2];
    short x3;
    short y3;
    char u3;
    char v3;
} BattleFxWaveStripQuad;

/* --- battle_fx_blue_dragon_force_effect --- */
/* Blue Dragonn record: frame counter and index into the trigger-frame table. */
typedef struct BattleFxDragonForceRecord {
    short ownerSlot;
    short frame;
    Int64Pair pos;
    unsigned char pad_0C[0x2];
    short angleStep;
    short posX;
    short posY;
    short growScale;
    short growScaleStep;
    short triggerIndex;
} BattleFxDragonForceRecord;

/* Task-slot record set up for the blue Dragonn effect. */
typedef struct BattleFxDragonForceSetupTask {
    short ownerSlot;
    unsigned char pad_02[0xa];
    short angle;
    short angleStep;
    short scale;
    short scaleStep;
    short growScale;
    short growScaleStep;
    short triggerIndex;
} BattleFxDragonForceSetupTask;

/* Task-slot timeline record for the blue Dragonn effect, with a trigger index. */
typedef struct BattleFxDragonForceTimeline {
    unsigned char pad_00[0xc];
    short scale;
    short angleStep;
    short posX;
    short posY;
    unsigned char pad_14[0x4];
    short triggerIndex;
} BattleFxDragonForceTimeline;

/* Spin slot for the Dragonn effect: lifetime and per-tick rate constants. */
typedef struct BattleFxDragonForceSpin {
    unsigned char pad_00[0xc];
    short angle;
    short lifetime;
    short rate;
    short scaleGrowStep;
    short scale;
    short scaleShrinkStep;
} BattleFxDragonForceSpin;

/* The battle task slot of BattleFx_BlueDragonForceGrowingModelTask. */
typedef struct BattleFxDragonForceGrowingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleStep;
    short scale;
    short scaleStep;
    unsigned char pad_14[0x4];
    short modelIndex;
} BattleFxDragonForceGrowingModelSlot;

/* The battle task slot of BattleFx_BlueDragonForceSpinScaledModelTask. */
typedef struct BattleFxDragonForceSpinScaledModelSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x8];
    short angle;
    short angleStep;
    short holdScale;
    short scaleGrowStep;
    short scale;
    short scaleShrinkStep;
} BattleFxDragonForceSpinScaledModelSlot;

/* --- battle_fx_blue_frog_song_effect --- */
/* Kaeru note emitter record: base point, random spread, rise range and scale. */
typedef struct BattleFxFrogSongNoteEmitter {
    short ownerSlot;
    short frameCount;
    union {
        Int64Pair pos;
        struct { short baseX; short baseY; short baseZ; unsigned char pad_0A[0x2]; };
    };
    short jitterOffset;
    short jitterRange;
    short riseRange;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxFrogSongNoteEmitter;

/* Task-slot record for the blue Kaeru aim effect: frame count and two offsets. */
typedef struct BattleFxFrogSongAimTask {
    short ownerSlot;
    short frameCount;
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    short scale;
    short aimValue;
} BattleFxFrogSongAimTask;

/* Blue Kaeru spawn slot for a note emitter. */
typedef struct BattleFxFrogSongNoteSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
    short noteIndex;
    short noteOffset;
    short riseRange;
    unsigned char pad_12[0x2];
    short scale;
} BattleFxFrogSongNoteSlot;

/* Blue Kaeru aim task record that copies its values from the ring record it reads. */
typedef struct BattleFxFrogSongAimState {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short scale;
    unsigned char pad_0E[0x2];
    short angle;
    short angularVelocity;
    short ringPhase;
    short aimStep;
} BattleFxFrogSongAimState;

/* The battle task slot of BattleFx_BlueFrogSongRingDrawTask. */
typedef struct BattleFxFrogSongRingDrawSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    unsigned char pad_0E[0x2];
    short angle;
    short angularVelocity;
    short ringPhase;
    short phaseStep;
} BattleFxFrogSongRingDrawSlot;

/* Resource record for the blue Kaeru effect. */
typedef struct BattleFxFrogSongResource {
    int groupTag;
    unsigned char pad_04[0x4];
    int resourceHandle;
} BattleFxFrogSongResource;

/* The tail of the Kaeru resource definitions (file names and their table rows), then the note
 * sprite's draw request, whose two handles the note drawer fills from the loaded table. */
typedef struct {
    int resourceRows[8];
    int noteSpriteHandle; /* +0x20 */
    int noteSpriteAltHandle; /* +0x24 */
} BattleBlueKaeruResources;

/* One note billboard point; the code walks a ring of 222 of them from g_BattleBlueFrogSongBillboardPoints. */
typedef struct BattleBlueFrogSongBillboardPoints {
    short posX;
    short riseY;
    short posZ;
    short scale;
} BattleBlueFrogSongBillboardPoints;

#pragma pack(push, 1)
typedef struct BattleBlueFrogSongNoteSpriteReq {
    unsigned char pad_0[0x4];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int kaeruNoteSpriteHandle; /* +0x8 */
    int kaeruNoteSpriteAltHandle; /* +0xc */
} BattleBlueFrogSongNoteSpriteReq;
#pragma pack(pop)

/* The ring sprites' colour and primitive code. From +0x8 it is also the ring's reference axis
 * (0, 0, -0x1000) for ComputeAngleBetweenVectors, whose Z is the first word after this object. */
#pragma pack(push, 1)
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char primCode; /* +0x3 */
    unsigned char pad_4[0x4];
    int ringAxisX; /* +0x8 */
    int ringAxisY; /* +0xc */
} BattleBlueKaeruRingSpriteReq;
#pragma pack(pop)

/* --- battle_fx_blue_goblin_punch_effect --- */
/* Battle Blue Goblin timeline slot: step, base position and random jitter parameters. */
typedef struct BattleFxBlueGoblinTimelineSlot {
    short ownerSlot;
    short step;
    SVECTOR pos;
    short jitterOffset;
    short jitterRange;
    short offsetY;
    short rangeY;
    short streakOffsetBase;
    short seed;
} BattleFxBlueGoblinTimelineSlot;

/* Battle effect task slot (0x20 bytes) for the Blue Goblinp timeline. */
typedef struct BattleFxGoblinPunchTaskSlot {
    short ownerSlot;
    unsigned char pad_02[0x2];
    SVECTOR pos;
    short baseValue;
    short jitterRange;
    short unitStep;
    short rangeY;
    short streakLength;
    short seed;
} BattleFxGoblinPunchTaskSlot;

/* Task-buffer slot for the Goblinp flash sprite. */
typedef struct BattleFxGoblinPunchFlashSlot {
    short ownerSlot;
    short timer;
    SVECTOR pos;
    short tableOffset;
    unsigned char pad_0E[0xa];
    short state;
} BattleFxGoblinPunchFlashSlot;

/* Goblinp particle slot positioned from its emitter with random jitter. */
typedef struct BattleFxGoblinPunchParticleSlot {
    unsigned char pad_00[0x4];
    SVECTOR pos;
    short scaledOffset;
    unsigned char pad_0E[0xa];
    short step;
} BattleFxGoblinPunchParticleSlot;

/* Blue Goblin streak vertex: position, draw offset, and per-frame velocity. */
typedef struct BattleFxBlueGoblinStreakVertex {
    SVECTOR pos;
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
} BattleFxBlueGoblinStreakVertex;

/* A colour as one packet word or as its bytes. */
typedef union BattleFxBlueGoblinColorWord {
    int word;
    CVECTOR rgb;
} BattleFxBlueGoblinColorWord;

/* Task data for the Blue Goblin streak lines: transform depth and per-line vertex position. */
typedef struct BattleFxBlueGoblinStreakState {
    unsigned char pad_00[0x4];
    int scratch;
    int depth;
    int flags;
    BattleFxBlueGoblinColorWord baseColor0;
    BattleFxBlueGoblinColorWord baseColor1;
    BattleFxBlueGoblinColorWord shadedColor0;
    BattleFxBlueGoblinColorWord shadedColor1;
    GfxPsxShortVertexSlot pos;
} BattleFxBlueGoblinStreakState;

/* --- battle_fx_blue_hatena_effect --- */
/* Draw state for the Blue Hatena PSX mesh: mesh data pointer, primitive cursor and counters. */
typedef struct BattleFxBlueQuestionMarksDraw {
    int meshData;
    int drawList;
    int stage;
    int primCursor;
    int primFlags;
    int count; /* +0x14: the front-facing result of Gfx_PsxStoreFrontFacing */
    unsigned char pad_18[0x4];
    int orderingDepth; /* +0x1C: the ordering depth Gfx_ReadTransformOrderingDepth2 stores */
    unsigned char pad_20[0x4];
    int index;
    Int64Pair vertices[4]; /* +0x28: the primitive's transformed vertex words */
} BattleFxBlueQuestionMarksDraw;

/* Drifting sprite slot for the Hatena effect: step counter and drift position. */
typedef struct BattleFxQuestionMarksDrifter {
    short ownerSlot;
    short step;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x2];
    short spriteScale;
} BattleFxQuestionMarksDrifter;

/* PSX textured quad written into the primitive buffer for the Hatena effect. */
typedef struct BattleFxQuestionMarksPrimQuad {
    int primTag;
    int code;
    unsigned char pad_08[0x4];
    int texCoord0Clut;
    unsigned char pad_10[0x4];
    int texCoord1Tpage;
    unsigned char pad_18[0x4];
    short texCoord2;
    unsigned char pad_1E[0x6];
    short texCoord3;
} BattleFxQuestionMarksPrimQuad;

/* Textured triangle primitive written into the render buffer for the blue Hatena effect. */
typedef struct BattleFxQuestionMarksTriPrim {
    int tag;
    int nextPrim;
    unsigned char pad_08[0x4];
    int texCoord0Clut;
    unsigned char pad_10[0x4];
    int texCoord1Tpage;
    unsigned char pad_18[0x4];
    short texCoord2;
} BattleFxQuestionMarksTriPrim;

/* Drifting sprite spawned around a source sprite by the blue Hatena effect. */
typedef struct BattleFxQuestionMarksDriftSprite {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityZ;
} BattleFxQuestionMarksDriftSprite;

/* PSX textured quad: four vertex offsets into the vertex table, with draw fields copied across. */
typedef struct BattleFxQuestionMarksTexQuad {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
    int texCoord0Clut;
    unsigned short texCoord1;
    short texCoord2;
    short texCoord3;
} BattleFxQuestionMarksTexQuad;

/* PSX Gouraud quad: four vertex offsets and a per-vertex color word copied into the draw packet. */
typedef struct BattleFxQuestionMarksGouraudQuad {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
    int color0;
    int color1;
    int color2;
    int color3;
} BattleFxQuestionMarksGouraudQuad;

/* Gouraud triangle record: three vertex byte offsets and three ints copied into a PSX primitive. */
typedef struct BattleFxGouraudTriRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned char pad_06[0x2];
    int color0;
    int color1;
    int color2;
} BattleFxGouraudTriRecord;

/* The battle task slot of BattleFx_BlueQuestionMarksTimelineTask. */
typedef struct BattleFxQuestionMarksTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short baseY;
    short posZ;
    unsigned char pad_0A[0x2];
    short modelScale;
    unsigned char pad_0E[0x2];
    short offset;
    short velocity;
} BattleFxQuestionMarksTimelineSlot;

/* PSX Gouraud triangle primitive for a Hatena effect, filled from its Gouraud triangle record. */
typedef struct BattleFxQuestionMarksGouraudTriPrim {
    int tag;
    int color0;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
} BattleFxQuestionMarksGouraudTriPrim;

/* --- battle_fx_blue_howin --- */
/* Effect100 slot for the Blue Howin fading sprite: position drift and fade level. */
typedef struct BattleFxWhiteWindFadeSprite {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short angleX;
    unsigned char pad_12[0x2];
    short angleZ;
    unsigned char pad_16[0x2];
    short fadeLevel;
    short scale;
    short spinStep;
} BattleFxWhiteWindFadeSprite;

/* Battle task slot for the Howin enter effect, holding the selected actor index. */
typedef struct BattleWhiteWindEnterSlot {
    unsigned char pad_00[0x4];
    short actorIndex;
    unsigned char pad_06[0x2];
    short bonePosX;
    short bonePosY;
} BattleWhiteWindEnterSlot;

/* --- battle_fx_blue_kaenn --- */
/* The battle task slot of the Kaenn timelines: ownerSlot is the actor or battler index while it runs and
 * BATTLE_TASK_FREE once it is released; posX..posZ is the flame's origin and dirX..dirZ its direction.
 * The fang and twin timelines also sway: angle gains angleStep each step. */
typedef struct BattleFxFlameThrowerSlot {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short dirX;
    short dirY;
    short dirZ;
    unsigned char pad_12[0x2];
    short angle;
    short angleStep;
} BattleFxFlameThrowerSlot;

#pragma pack(push, 1)
typedef struct BattleBlueFlameThrowerSpriteDrawReq {
    int drawReqPtr; /* +0x0 */
    int kaennSpriteDrawHandle; /* +0x4 */
    unsigned char pad_8[0x4];
    short kaennSpriteDrawFade; /* +0xc */
    short kaennSpriteDrawAux; /* +0xe */
} BattleBlueFlameThrowerSpriteDrawReq;
#pragma pack(pop)

/* --- battle_fx_blue_lv4_jibaku_effect --- */
/* One point of a ring path: where it is, the axis the ring turns about there and the turn angle. */
typedef struct BattleFxJibakuRingSource {
    SVECTOR pos;
    VECTOR axis;
    int angle;
} BattleFxJibakuRingSource;

/* The ring path of one target (stride 0x57C): the offset from the source to the target, the
 * perpendicular to it in the ground plane, and up to 48 path points. */
typedef struct BattleFxBlueJibakuRing {
    unsigned char pad_00[0x4];
    int segmentCount;
    int segmentIndex;
    unsigned char pad_0C[0x8];
    VECTOR offset;
    VECTOR perp;
    unsigned char pad_34[0x8];
    BattleFxJibakuRingSource segments[48];
} BattleFxBlueJibakuRing;
typedef char CheckBattleFxBlueJibakuRing[(sizeof(BattleFxBlueJibakuRing) == 0x57C) ? 1 : -1];

/* The scratch block BattleFx_BlueL4SuicideDrawRingSegments and its callees project ring quads through,
 * and draw explosion triangles from. The path planner views the same block as
 * BattleFxBlueJibakuTarget. */
typedef struct BattleFxJibakuShard {
    unsigned char pad_00[0x4];
    int projected;
    int depth;
    int clipFlags;
    SVECTOR vertex[4];
    unsigned char pad_30[0x10];
    int quadXY[4];
    int prevQuadXY[4];
    int prevDepth;
    unsigned char pad_64[0x1c];
    PsxMatrix ringMatrix;
    PsxMatrix cameraMatrix;
    VECTOR vel0;
    VECTOR vel1;
    unsigned char pad_DC[0x20];
    int blendFirst[4]; /* each corner's first blend factor, from the projected vertex record */
    int blendSecond[4];
    int prevBlendFirst[4];
    int prevBlendSecond[4];
} BattleFxJibakuShard;
typedef char CheckBattleFxJibakuShard[(sizeof(BattleFxJibakuShard) == 0x13C) ? 1 : -1];

/* Per-target path state of the Blue Lv4 effect: the path point stepped along, its wobble and
 * the vectors between points. */
typedef struct BattleFxBlueJibakuTarget {
    unsigned char pad_00[0x10];
    SVECTOR accum;
    SVECTOR goal;
    SVECTOR pos;
    unsigned char pad_28[0x38];
    int speed;
    int amplitude;
    int phase;
    unsigned char pad_6C[0xc];
    int phaseStep;
    unsigned char pad_7C[0x40];
    VECTOR rel;
    VECTOR vel;
    VECTOR swing;
    VECTOR axis;
} BattleFxBlueJibakuTarget;

/* Blue Lv4 Jibaku timeline task: frame, the target's position, ring index, next step frame and phase. */
typedef struct BattleFxBlueL4SuicideTask {
    short ownerSlot;
    short frame;
    SVECTOR pos;
    short spreadRadius;
    short scaleRatio;
    short ringIndex;
    short nextFrame;
    short phase;
    short actorIndex;
} BattleFxBlueL4SuicideTask;

/* An explosion sprite task: frame, position and scale. */
typedef struct BattleFxBlueL4SuicideSprite {
    short ownerSlot;
    short frame;
    SVECTOR pos;
    short scale;
} BattleFxBlueL4SuicideSprite;

/* Where the effect's ring paths start: the source's position and whether it was taken yet. */
typedef struct BattleFxBlueJibakuState {
    short posX;
    short posY;
    short posZ;
    short started;
} BattleFxBlueJibakuState;

/* PSX flat-shaded primitive for one quad of the Jibaku ring, with corner words from the ring state. */
typedef struct BattleFxRingQuadPrim {
    int tag;
    int color;
    int xyWord0;
    int xyWord1;
    int xyWord2;
    int xyWord3;
} BattleFxRingQuadPrim;

/* --- battle_fx_blue_lv5_death_effect --- */
/* Battle effect task slot for a fading sprite: frame counter, height and record type. */
typedef struct BattleFxFadingSprite {
    short ownerSlot;
    short frame;
    short pos[3];
    unsigned char pad_0A[0x2];
    short height;
    short recordType;
} BattleFxFadingSprite;

/* Task-slot record for the blue Lv5 Deth screen intensity effect: step and start step. */
typedef struct BattleFxScreenIntensityTask {
    short ownerSlot;
    short step;
    unsigned char pad_04[0xa];
    short startStep;
} BattleFxScreenIntensityTask;

/* Fading sprite record for the Lv5 Deth effect, one per target. */
typedef struct BattleFxL5DeathFadingSprite {
    unsigned char pad_00[0x4];
    short pos[3];
    unsigned char pad_0A[0x2];
    short scale;
    short targetIndex;
} BattleFxL5DeathFadingSprite;

#pragma pack(push, 1)
typedef struct BattleBlueL5DeathSpriteColor {
    char lv5DethSpriteColorByte0; /* +0x0 */
    char lv5DethSpriteColorByte1; /* +0x1 */
    char lv5DethSpriteColorByte2; /* +0x2 */
    unsigned char pad_3[0x1];
    short lv5DethSpriteFrameWord; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleBlueL5DeathSpriteColor;
#pragma pack(pop)

/* --- battle_fx_blue_magic_breath_effect --- */
/* Blue Magikal timeline record: phase, base position, random jitter bounds and scale. */
typedef struct BattleFxBlueMagicBreathTimeline {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short jitterBias;
    short jitterRange;
    short yBias;
    short yJitterRange;
    short scale;
    short scaleStep;
} BattleFxBlueMagicBreathTimeline;

/* Blue Magikal fading sprite: elapsed frames, position, per-frame velocity and billboard angle. */
typedef struct BattleFxBlueMagicBreathSprite {
    short ownerSlot;
    short elapsed;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    short angleStep;
} BattleFxBlueMagicBreathSprite;

/* Task-slot record for the Blue Magik timeline effect: scale and velocity. */
typedef struct BattleFxBlueMagikRecord {
    short ownerSlot; /* +0x0 */
    unsigned char pad_2[0x4];
    short initialVelocity;
    unsigned char pad_08[0x4];
    short scale;
    short scaleTimes2;
    short velocity;
    short velocityTimes3;
    short spriteScale;
} BattleFxBlueMagikRecord;

#pragma pack(push, 1)
typedef struct BattleBlueMagicBreathSpriteColor {
    char magikalSpriteColorByte0; /* +0x0 */
    char magikalSpriteColorByte1; /* +0x1 */
    char magikalSpriteColorByte2; /* +0x2 */
    unsigned char pad_3[0x1];
} BattleBlueMagicBreathSpriteColor;
#pragma pack(pop)

/* --- battle_fx_blue_magic_hammer_effect --- */
/* Blue Hanmmer spawner: base position, random spread and step counter for drifting sprites. */
typedef struct BattleFxBlueMagicHammerSpawner {
    short ownerSlot;
    short step;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x2];
    short center;
    short halfRange;
    unsigned char pad_10[0x2];
    short spriteScale;
    short driftSpeed;
} BattleFxBlueMagicHammerSpawner;

/* 32-byte BattleFx effect slot for the Hanmmer drifting sprite. */
typedef struct BattleFxMagicHammerDriftSlot {
    short ownerSlot;
    short timer;
    short posX;
    unsigned char pad_06[0x2];
    short posY;
    unsigned char pad_0A[0x8];
    short scale;
    unsigned char pad_14[0x2];
    short velocityX;
    short velocityY;
} BattleFxMagicHammerDriftSlot;

/* Swing slot for the Hanmmer effect, carrying the target index of its swing. */
typedef struct BattleFxMagicHammerSwing {
    unsigned char pad_00[0xc];
    short swingAngle;
    short swingStep;
    short yaw;
    short tiltAngle;
    short tiltStep;
    short swingRadius;
    short heightOffset;
    short targetIndex;
} BattleFxMagicHammerSwing;

/* Drifting sprite spawned around a base point by the blue Hanmmer effect. */
typedef struct BattleFxMagicHammerDriftSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short scale;
    unsigned char pad_14[0x2];
    short velocityX;
    short velocityZ;
} BattleFxMagicHammerDriftSprite;

/* The battle task slot of BattleFx_BlueMagicHammerSwingModelTask. */
typedef struct BattleFxMagicHammerSwingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short swingAngle;
    short swingStep;
    short angle;
    short tiltAngle;
    short tiltStep;
    short swingRadius;
    short heightOffset;
    short actorIndex;
} BattleFxMagicHammerSwingModelSlot;

/* Drifting sprite task slot for the blue Hammer effect, sized from the swing range. */
typedef struct BattleFxHammerDriftTask {
    unsigned char pad_00[0xc];
    short halfSwingRange;
    short quarterSwingRange;
    unsigned char pad_10[0x2];
    short spriteScale;
    short driftSpeed;
} BattleFxHammerDriftTask;

/* --- battle_fx_blue_matra_effect --- */
/* Blue Matra ribbon record: depth, colour and start/end position history used to draw the trail. */
typedef struct BattleFxBlueMatraMagicSegmentWork {
    unsigned char pad_00[0x8];
    int depth;
    unsigned char pad_0C[0x4];
    short corner0X;
    short corner0Y;
    short corner0Z;
    unsigned char pad_16[0x2];
    short corner1X;
    short corner1Y;
    short corner1Z;
    unsigned char pad_1E[0x2];
    short corner2X;
    short corner2Y;
    short corner2Z;
    unsigned char pad_26[0x2];
    short corner3X;
    short corner3Y;
    short corner3Z;
    unsigned char pad_2E[0x2];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_36[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_3E[0x2];
    int quadVertex[4]; /* packed screen XY of the four corners */
    int quadVertexPrev[4];
    int prevDepth;
    int baseColor;
    int colorBottom;
    int colorStart;
    unsigned char pad_70[0x10];
    PsxMatrix rotation;
    PsxMatrix cameraRotation;
    int deltaX;
    int deltaY;
    int deltaZ;
    unsigned char pad_C8[0x10];
    int deltaLength;
    VECTOR axis;
    unsigned char pad_EC[0x10];
    int screenX[4];
    int screenY[4];
    int blendFactorA[4];
    int blendFactorB[4];
} BattleFxBlueMatraMagicSegmentWork;


/* One trail point of a Blue Matra ribbon; valid is 0 once the point has been retired. */
typedef struct BattleFxMatraMagicTrailPoint {
    short x;
    short y;
    short z;
    short valid;
} BattleFxMatraMagicTrailPoint;

/* One Blue Matra ribbon record, stepped each frame by BattleFx_BlueMatraMagicRibbonTask. */
typedef struct BattleFxBlueMatraMagicRibbon {
    int state;
    int speed;
    int waveAmplitude;
    int speedAccel;
    int waveAmplitudeStep;
    int vectorLength;
    int clearedParam;
    unsigned char pad_1C[0x8];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_2A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    short jitterRadius;
    short anchorX; /* emitter position at spawn, the impact point once the ribbon has hit */
    short anchorY;
    short anchorZ;
    unsigned char pad_3A[0x2];
    int aimDirX;
    int aimDirY;
    int aimDirZ;
    int phase;
    int velocityX;
    int velocityY;
    int velocityZ;
    int phaseStep;
    int deltaX;
    int deltaY;
    int deltaZ;
    int phaseStepDecay;
    int arriveDistSq;
    int trailHeadIndex;
    int trailTailIndex;
    BattleFxMatraMagicTrailPoint trail[6];
    unsigned char pad_A8[0x30];
} BattleFxBlueMatraMagicRibbon;

/* PSX-style Gouraud quad packet for Blue Matra: tag word, link pointers and vertex words. */
typedef struct BattleFxMatraMagicQuadPacket {
    int tag;
    int colorStart;
    int vertex0;
    int color1;
    int vertex1;
    int colorBottom;
    int vertex2;
    int color3;
    int vertex3;
} BattleFxMatraMagicQuadPacket;

/* Blue Matra ribbon spawn slot that advances through ribbon records. */
typedef struct BattleFxMatraMagicRibbonSlot {
    short ownerSlot;
    short frameCounter;
    short recordIndex;
    short flags;
    unsigned char pad_08[0xc];
    short ribbonIndex;
} BattleFxMatraMagicRibbonSlot;

/* Battle task slot for the Matra ribbon task, holding the ribbon index. */
typedef struct BattleMatraMagicRibbonSlot {
    unsigned char pad_00[0x4];
    short ribbonIndex;
    short isActive;
} BattleMatraMagicRibbonSlot;

/* g_BattleBlueMatraMagicTrajectoryParams: the casting actor's bone position, its radius, and the ribbon spread limit. */
typedef struct BattleBlueMatraMagicTrajectory {
    short x;
    short y;
    short z;
    short radius;
    short peakRadius;
} BattleBlueMatraMagicTrajectory;

/* One Matra target: the actor it hits, its bone position and ribbon spread, and the ribbons sent at it. */
#pragma pack(push, 1)
typedef struct BattleMatraMagicEmitter {
    int targetHandle;
    short posX;
    short posY;
    short posZ;
    short spread;
    int hitCount;
    unsigned char pad_10[0x4];
} BattleMatraMagicEmitter;

/* Emitters shuffle order, per-target emitters, casting actor and the resource handle. */
typedef struct BattleBlueMatraMagicEmitterRecord {
    int order[6];
    int targetCount;
    unsigned char pad_1C[0x4];
    BattleMatraMagicEmitter emitters[6];
    int actorIndex;
    GfxTableCursor *resourceHandle;
} BattleBlueMatraMagicEmitterRecord;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueMatraMagicModelDrawReq {
    int restDirX; /* +0x0 */
    unsigned char pad_4[0xc];
    int matraModelDrawVector; /* +0x10 */
} BattleBlueMatraMagicModelDrawReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueMatraMagicSpriteDrawReq {
    int matraSpriteGroup; /* +0x0 */
    int matraSpriteDrawHandle; /* +0x4 */
    unsigned char pad_8[0x4];
    short matraSpriteDrawFrame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleBlueMatraMagicSpriteDrawReq;
#pragma pack(pop)

/* --- battle_fx_blue_mytyg_effect --- */
/* Mytyg particle: drift offsets, lifetime, scale and growth step. */
typedef struct BattleFxBigGuardParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short delay;
    short lifetime;
    short speed;
    short rotYStep;
    short rotZ;
    short rotZStep;
    short scale;
    short growStep;
} BattleFxBigGuardParticle;

/* Blue Mytyg effect record: frame counter and allocation type for the timeline task. */
typedef struct BattleFxBigGuardRecord {
    short ownerSlot;
    short frame;
    unsigned char position[8];
    unsigned char pad_0c[0xc];
    short growRate;
    short delay;
    short recordType;
} BattleFxBigGuardRecord;

/* Ascending sprite slot of the Mytyg effect: step counter, height and ascent speed. */
typedef struct BattleFxBigGuardSprite {
    short ownerSlot;
    short step;
    short posX;
    short height;
    unsigned char pad_08[0x6];
    short ascentSpeed;
    unsigned char pad_10[0x8];
    short scale;
} BattleFxBigGuardSprite;

/* The battle task slot of BattleFx_BlueBigGuardSpinGrowModelTask. */
typedef struct BattleFxBigGuardSpinGrowModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short spinStep;
    short growth;
    short growthStep;
    short spinAngle;
    short spinAngleStep;
    short growAmount;
    short growAmountStep;
} BattleFxBigGuardSpinGrowModelSlot;

/* The battle task slot of BattleFx_BlueBigGuardGrowingFadeModelTask. */
typedef struct BattleFxBigGuardGrowingFadeModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotationY;
    unsigned char pad_0E[0x2];
    short offsetDistance;
    unsigned char pad_12[0x6];
    short scale;
    short scaleStep;
} BattleFxBigGuardGrowingFadeModelSlot;

/* Battle task slot for the Mytyg effect, holding a target's yaw and index. */
typedef struct BattleBigGuardTargetSlot {
    unsigned char pad_00[0x4];
    unsigned char position[8];
    unsigned char pad_0c[0xc];
    short growRate;
    short targetYaw;
    short targetIndex;
} BattleBigGuardTargetSlot;

#pragma pack(push, 1)
typedef struct BattleBlueBigGuardSpriteDrawReq {
    int packedOffsetXY; /* +0x0 */
    char mytygSpriteAngle; /* +0x4 */
    unsigned char pad_5[0xb];
    int mytygSpriteSceneObject; /* +0x10 */
} BattleBlueBigGuardSpriteDrawReq;
#pragma pack(pop)

/* --- battle_fx_blue_nanntoka_effect --- */
/* Blue Nanntoka timeline slot: per-phase constants and a phase index for its model tasks. */
typedef struct BattleFxBluePandorasBoxSlot {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    short radius;
    unsigned char pad_0A[0x2];
    short speed;
    short drift;
    short velocityY;
    unsigned char pad_12[0x2];
    short gravity;
    short lifetime;
    short tumbleAngle;
    short delay;
    short scale;
    short phase;
} BattleFxBluePandorasBoxSlot;

/* Child record of the Blue Nanntoka timeline task, allocated from the resource slot pool. */
typedef struct BattleFxPandorasBoxChildRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotZ;
    short segmentCount;
    short rotY;
    short rotYStep;
    short scaleFraction;
    short scaleStep;
} BattleFxPandorasBoxChildRecord;

/* Blue Nanntoka self record: frame counter and state for its own timeline. */
typedef struct BattleFxPandorasBoxSelfRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x18];
    short state;
} BattleFxPandorasBoxSelfRecord;

/* The battle task slot of BattleFx_BluePandorasBoxTumblingModelTask. */
typedef struct BattleFxPandorasBoxTumblingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    short angleStep;
    short tumbleAngle;
    short tumbleAngleStep;
    short scale;
    short modelIndex;
} BattleFxPandorasBoxTumblingModelSlot;

/* The battle task slot of BattleFx_BluePandorasBoxSpinDriftModelTask. */
typedef struct BattleFxPandorasBoxSpinDriftModelSlot {
    short ownerSlot;
    short frameCounter;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0xa];
    short angle;
    short spinStep;
    unsigned char pad_18[0x4];
    short scale;
} BattleFxPandorasBoxSpinDriftModelSlot;

/* The battle task slot of BattleFx_BluePandorasBoxScaledSpinModelTask. */
typedef struct BattleFxPandorasBoxScaledSpinModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short spinStep;
    short growth;
    short growthStep;
    short scale;
} BattleFxPandorasBoxScaledSpinModelSlot;

/* The battle task slot of BattleFx_BluePandorasBoxGrowShrinkModelTask. */
typedef struct BattleFxPandorasBoxGrowShrinkModelSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x10];
    short scale;
    short scaleStep;
} BattleFxPandorasBoxGrowShrinkModelSlot;

/* Per-step velocity of one shatter fragment of the Blue Nanntoka effect, added to its position. */
typedef struct BattleFxShatterVelocity {
    short velX;
    short velY;
    short velZ;
    unsigned char pad_06[0x2];
} BattleFxShatterVelocity;

/* Scratch block the particle field task draws from: the GTE matrix at +0x30 and the velocity it
 * normalises at +0x220 (the shared nanntoka_render.h reads the same block by offset). */
typedef struct BattleFxPandorasBoxFieldState {
    unsigned char pad_00[0x28];
    short originX;
    short originY;
    short originZ;
    unsigned char pad_2E[0x2];
    PsxMatrix matrix;
    unsigned char pad_4E[0x1d2];
    int velocityX;
    int velocityY;
    int velocityZ;
} BattleFxPandorasBoxFieldState;

/* The same scratch block as the shatter fragment draw uses it: flags, four vertices, the GTE matrix
 * and one rotation matrix for each of the 15 groups. */
typedef struct BattleFxShatterState {
    unsigned char pad_00[0xc];
    int flags;
    short vertex0X;
    short vertex0Y;
    short vertex0Z;
    unsigned char pad_16[0x2];
    short vertex1X;
    short vertex1Y;
    short vertex1Z;
    unsigned char pad_1E[0x2];
    short vertex2X;
    short vertex2Y;
    short vertex2Z;
    unsigned char pad_26[0x2];
    short vertex3X;
    short vertex3Y;
    short vertex3Z;
    unsigned char pad_2E[0x2];
    PsxMatrix matrix;
    PsxMatrix rotations[15];
} BattleFxShatterState;

/* One cell (0x18 bytes) of the 40 by 21 shatter grid: its position and per-frame velocity (the cell
 * array starts at g_BattleBluePandorasBoxTransformWorking), then the texture words in
 * g_BattleBluePandorasBoxShatterCellUvs (the same stride, 0x10 bytes later). */
typedef struct BattleFxShatterCell {
    short x;
    short y;
    short z;
    short rotIndex;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0E[0xa];
} BattleFxShatterCell;

typedef struct BattleFxShatterCellUv {
    int tileWord;
    int tpageWord;
    unsigned char pad_08[0x10];
} BattleFxShatterCellUv;

typedef BattleFxShatterCell BattleFxShatterCellRow[40];
typedef BattleFxShatterCellUv BattleFxShatterCellUvRow[40];

/* PSX primitive for one shatter fragment quad, filled from the blue particle records and a decay value. */
typedef struct BattleFxShatterFragmentPrim {
    int tag;
    union {
        int color;
        struct {
            unsigned char rgb[3];
            char gpuCode;
        };
    };
    unsigned char pad_08[0x4];
    int decayVal;
    unsigned char pad_10[0x4];
    int uvTpageWord;
    unsigned char pad_18[0x4];
    int uvWord2;
    unsigned char pad_20[0x4];
    int uvWord3;
} BattleFxShatterFragmentPrim;

/* --- battle_fx_blue_reazer_effect --- */
/* Three short coordinates and a fourth lane: padding, or a trail point's live flag. */
typedef struct BattleFxLaserPoint {
    short x;
    short y;
    short z;
    short w;
} BattleFxLaserPoint;

#define BATTLE_FX_REAZER_TRAIL_LENGTH 0x18

/* One Blue Reazer ribbon record, stepped each frame by BattleFx_BlueLaserRibbonTask. */
typedef struct BattleFxBlueLaserRibbon {
    int state;
    int speed;
    int waveAmplitude;
    int speedAccel;
    int waveAmplitudeStep;
    int vectorLength;
    int timer;
    unsigned char pad_1C[0x8];
    BattleFxLaserPoint pos;
    union {
        BattleFxLaserPoint target;
        struct {
            short targetX;
            short targetY;
            short targetZ;
            short jitterRadius;
        };
    };
    BattleFxLaserPoint anchor;
    int aimDirX;
    int aimDirY;
    int aimDirZ;
    int phase;
    int velocityX;
    int velocityY;
    int velocityZ;
    int phaseStep;
    int deltaX;
    int deltaY;
    int deltaZ;
    int phaseStepDecay;
    int arriveDistSq;
    int trailHeadIndex;
    int trailTailIndex;
    BattleFxLaserPoint trail[BATTLE_FX_REAZER_TRAIL_LENGTH];
    unsigned char pad_138[0x40];
} BattleFxBlueLaserRibbon;
typedef char CheckBattleFxBlueReazerRibbon[(sizeof(BattleFxBlueLaserRibbon) == 0x178) ? 1 : -1];

/* Blue Reazer ribbon effect state, one global record. Twelve ribbons are used; the last one runs on
 * past the record into the bss padding after it. */
typedef struct BattleFxBlueLaserState {
    int actorIndex;
    int spawnStep;
    unsigned char pad_08[0x4];
    short ribbonCount;
    unsigned char pad_0E[0x2];
    short ribbonPhase;
    unsigned char pad_12[0x6];
    BattleFxLaserPoint target;
    short range;
    unsigned char pad_22[0x6];
    BattleFxBlueLaserRibbon ribbons[11];
    unsigned char pad_1050[0x138];
} BattleFxBlueLaserState;
typedef char CheckBattleFxBlueReazerState[(sizeof(BattleFxBlueLaserState) == 0x1188) ? 1 : -1];

/* PSX-style Gouraud quad packet for Blue Reazer: tag word, link pointers and vertex words. */
typedef struct BattleFxLaserQuadPacket {
    int tag;
    int colorStart;
    int colorCorner;
    int colorStartAlt;
    int colorNextCorner;
    int shadeTail;
    int shadeCorner;
    int shadeTailAlt;
    int shadeNextCorner;
} BattleFxLaserQuadPacket;

/* Blue Reazer ribbon spawn slot that counts frames and ribbon steps. */
typedef struct BattleFxLaserSpawnSlot {
    short ownerSlot;
    short frameCounter;
    BattleFxLaserPoint bonePos;
    short jitterRadius;
    unsigned char pad_0E[0x6];
    short stepIndex;
} BattleFxLaserSpawnSlot;

/* Task slot set up for the blue Reazer ribbon, receiving a bone world position. */
typedef struct BattleFxLaserSetupSlot {
    short status;
    unsigned char pad_02[0x2];
    BattleFxLaserPoint bonePos;
    short jitterRadius;
    unsigned char pad_0E[0x6];
    short stepIndex;
} BattleFxLaserSetupSlot;

/* Task slot for a blue Reazer ribbon, indexing the ribbon records. */
typedef struct BattleFxLaserRibbonTaskSlot {
    short ownerSlot;
    short step;
    short ribbonIndex;
} BattleFxLaserRibbonTaskSlot;

/* Work block of one Blue Reazer ribbon segment: the quad's corners and projected points, its start and
 * end, the matrices that place it and the previous segment's screen points. */
#pragma pack(push, 1)
typedef struct BattleFxBlueLaserSegmentWork {
    unsigned char pad_00[0x8];
    int depth;
    unsigned char pad_0C[0x4];
    SVECTOR corner[4];
    BattleFxLaserPoint start;
    BattleFxLaserPoint end;
    int projected[4];
    int prevProjected[4];
    int prevDepth;
    int baseColor;
    int colorBottom;
    int colorStart;
    PsxMatrix rotation;
    PsxMatrix view;
    int deltaX;
    int deltaY;
    int deltaZ;
    unsigned char pad_B8[0x10];
    int deltaLengthSq;
    VECTOR axis;
    unsigned char pad_DC[0x10];
    int screenX[4];
    int screenY[4];
    int prevScreenX[4];
    int prevScreenY[4];
} BattleFxBlueLaserSegmentWork;
#pragma pack(pop)

/* --- battle_fx_blue_ruretto_effect --- */
// --- END GENERATED EXTERNS ---
/* A battle task slot of the Ruretto effect: the task's owner, its frame counter, a bone world
 * position (the shorts Battle_GetBoneWorldPos writes, copied as eight bytes) and the actor's yaw. */
typedef struct BattleFxRouletteSlot {
    short ownerSlot;
    short frameCount;
    Int64Pair pos;
    short actorYaw;
} BattleFxRouletteSlot;

/* The ring-buffer record Battle_AllocSceneMessage returns: a position and a yaw. */
typedef struct BattleFxRouletteRingRecord {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    unsigned char pad_0C[0x4];
    short yaw;
} BattleFxRouletteRingRecord;

#pragma pack(push, 1)
/* One packed screen corner: the x and y shorts, or both as the word a primitive copies. */
typedef union BattleFxRurettoCorner {
    int packed;
    struct {
        short x;
        short y;
    } xy;
} BattleFxRurettoCorner;

/* The block Gfx_GetPsxScratchpadPtr(0) returns to the selection task: the projection flags, the target
 * bone's world position and the four projected corners of the cursor quad. */
typedef struct BattleFxBlueRouletteRunState {
    unsigned char pad_00[0x4];
    int projectFlagA;
    unsigned char pad_08[0x4];
    int projectFlagB;
    unsigned char pad_10[0x20];
    short bonePos[3];
    unsigned char pad_36[0x78];
    BattleFxRurettoCorner corner[4];
} BattleFxBlueRouletteRunState;
#pragma pack(pop)

/* One vertex of the cursor primitive: screen position, then the texture word (u, v). */
typedef union BattleFxRurettoUvWord {
    int word;
    struct {
        char u;
        char v;
    } uv;
} BattleFxRurettoUvWord;

typedef struct BattleFxRouletteVertex {
    int xy;
    BattleFxRurettoUvWord tex;
} BattleFxRouletteVertex;

/* The PSX textured quad the selection task draws: tag, colour and primitive code, four vertices. */
typedef struct BattleFxRoulettePrimitive {
    int tag;
    union {
        int color;
        struct {
            unsigned char pad_r[0x3];
            char primCode;
        } code;
    } rgbc;
    BattleFxRouletteVertex vertex[4];
} BattleFxRoulettePrimitive;

/* --- battle_fx_blue_sflea_effect --- */
/* Timeline state for the Blue Sflea effect: target position, timeline frame and offset index. */
typedef struct BattleFxBlueShadowFlareRecord {
    short ownerSlot;
    short frame;
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_0A[0x2];
    short offsetIndex;
    unsigned char pad_0E[0x6];
    short range;
} BattleFxBlueShadowFlareRecord;

/* Blue sflea spark seed: per-axis velocities and a random value set by the timeline task. */
typedef struct BattleFxShadowFlareLivewireSeed {
    unsigned char pad_00[0xc];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short pushLength;
    short scale;
    short flag;
} BattleFxShadowFlareLivewireSeed;

/* Sflea grow/fade sprite record: frame counter, sprite frame index, angle and spin rate. */
typedef struct BattleFxShadowFlareSpriteRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    short frameIndex;
    unsigned char pad_0E[0x4];
    short depth;
    short angle;
    short spinRate;
} BattleFxShadowFlareSpriteRecord;

/* Setup state for the Blue Sflea effect, keyed to a target actor. */
typedef struct BattleFxBlueShadowFlareState {
    short ownerSlot;
    unsigned char pad_02[0x4];
    short targetY;
    unsigned char pad_08[0x4];
    short offsetIndex;
    unsigned char pad_0E[0x6];
    short range;
} BattleFxBlueShadowFlareState;

/* Sflea delayed spawn slot counting up to its spawn frame. */
typedef struct BattleFxShadowFlareDelayedSpawnSlot {
    short ownerSlot;
    short timer;
    Int64Pair pos;
    unsigned char pad_0C[0x8];
    short progress;
} BattleFxShadowFlareDelayedSpawnSlot;

/* What BattleFx_BlueShadowFlareGrowFadeQuadTask reads through Gfx_GetPsxPacketScratchPtr(). */
typedef struct BattleFxShadowFlareFadeQuadRenderStateBlock {
    int tag;
    int polyCommand;
    unsigned char pad_08[0x4];
    short uvWord0;
    short clut;
    unsigned char pad_10[0x4];
    short uvWord1;
    short tpage;
    unsigned char pad_18[0x4];
    short uvWord2;
    unsigned char pad_1E[0x6];
    short uvWord3;
} BattleFxShadowFlareFadeQuadRenderStateBlock;

/* Task slot record spawned by the delayed Sflea effect, with values derived from a sibling record. */
typedef struct BattleShadowFlareDelayedSpawnRecord {
    unsigned char pad_00[0x6];
    short posY;
    unsigned char pad_08[0x4];
    short frameIndex;
    unsigned char pad_0E[0x4];
    short depth;
    short angle;
    short spinRate;
} BattleShadowFlareDelayedSpawnRecord;

/* Task slot for the blue sflea frame-table sprite; frameIndex indexes the frame index table. */
typedef struct BattleFxShadowFlareSpriteSlot {
    short status;
    short frameIndex;
} BattleFxShadowFlareSpriteSlot;

/* Battle task slot for the Sflea delayed spawn task. */
typedef struct BattleShadowFlareDelayedSlot {
    unsigned char pad_00[0x6];
    short posY;
    unsigned char pad_08[0xc];
    short scale;
    short progressFrac;
} BattleShadowFlareDelayedSlot;

/* One 8-byte record of g_BattleBlueShadowFlareSavedActorCoord: an actor's saved tint phase and tint pulse. */
typedef struct BattleBlueShadowFlareSavedActorCoordEntry {
    short tintPhase;
    short savedTintPulse;
    unsigned char pad_04[0x4];
} BattleBlueShadowFlareSavedActorCoordEntry;

/* Render state of the Blue Sflea flat colour strips: tag, colour word and strip x positions. */
typedef struct BattleFxFlatColorStripState {
    int tag;
    int color;
    int xLeft;
    int x1;
    int xy0;
    int xy1;
} BattleFxFlatColorStripState;

/* Render state for the PSX flat colour strip draw; its shorts are shifted by one. */
typedef struct BattleFxShadowFlareStripRecord {
    unsigned char pad_00[0x7];
    char polyCommand;
    unsigned char pad_08[0x2];
    short vertex0Y;
    unsigned char pad_0C[0x2];
    short vertex1Y;
    unsigned char pad_10[0x2];
    short vertex2Y;
    unsigned char pad_14[0x2];
    short vertex3Y;
} BattleFxShadowFlareStripRecord;

#pragma pack(push, 1)
typedef struct BattleBlueShadowFlareDrawColor {
    char colorR; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    char isActive; /* +0x3 */
} BattleBlueShadowFlareDrawColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueShadowFlareSavedActorAlpha {
    unsigned char pad_0[0x1];
    char savedTintG; /* +0x1 */
    char savedTintB; /* +0x2 */
    unsigned char pad_3[0x1];
} BattleBlueShadowFlareSavedActorAlpha;
#pragma pack(pop)

/* --- battle_fx_blue_shadow_flare_effect --- */
/* Task-slot record set up for the blue Trainn spin and grow effect. */
typedef struct BattleFxTrineSetupTask {
    short ownerSlot;
    unsigned char pad_02[0x4];
    short posY;
    unsigned char pad_08[0x4];
    short spinX;
    short spinStepX;
    short spinY;
    short spinStepY;
    short spinZ;
    short spinStepZ;
    short scale;
    short scaleStep;
} BattleFxTrineSetupTask;

/* Setup values for the Blue Trainn spin effect task slot. */
typedef struct BattleFxBlueTrineSpinState {
    unsigned char pad_00[0xc];
    short spinX;
    short spinStepX;
    short spinY;
    short spinStepY;
    short spinZ;
    short spinStepZ;
    short scale;
    short scaleStep;
} BattleFxBlueTrineSpinState;

/* The battle task slot of BattleFx_BlueTrineSpinGrowFadeModelTask. */
typedef struct BattleFxTrineSpinGrowFadeModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinX;
    short spinStepX;
    short spinY;
    short spinStepY;
    short spinZ;
    short spinStepZ;
    short scale;
    short scaleStep;
} BattleFxTrineSpinGrowFadeModelSlot;

/* --- battle_fx_builtin_hit_sparks --- */
/* Hit-spark emitter task record: frame counter and bone-follow actor and bone indices. */
typedef struct BattleFxHitLivewireEmitter {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x9];
    unsigned char cellRow;
    short texCoord;
    unsigned char pad_10[0x2];
    short actorIndex;
    short boneIndex;
    short spriteFlags;
} BattleFxHitLivewireEmitter;

/* Hit-spark particle task slot: frame timer, cell row, sprite cell word, scale and flags. */
typedef struct BattleFxHitLivewireSlot {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x9];
    char cellRow;
    short cellWord;
    short scale;
    unsigned char pad_12[0x4];
    short flags;
} BattleFxHitLivewireSlot;

/* Hit spark particle slot filled from its emitter. */
typedef struct BattleFxHitLivewireParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x3];
    unsigned char cellRow;
    short texCoord;
    short scale;
    unsigned char pad_12[0x4];
    short spriteFlags;
} BattleFxHitLivewireParticle;

/* Hit spark particle slot that picks a random texture row and coordinate. */
typedef struct BattleFxHitLivewireRowParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x3];
    char texRow;
    short texCoord;
    short scale;
    unsigned char pad_12[0x4];
    short spriteFlags;
} BattleFxHitLivewireRowParticle;

/* Short-lived hit spark particle slot with a fixed texture coordinate. */
typedef struct BattleFxHitLivewireShortParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x3];
    unsigned char selector;
    short texCoord;
    short scale;
    unsigned char pad_12[0x4];
    short spriteFlags;
} BattleFxHitLivewireShortParticle;

/* --- battle_fx_builtin_puff_mog --- */
/* Puff particle task record: step counter, position, Y velocity and scale. */
typedef struct BattleFxPuffParticle {
    short angleOffset;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    short scale;
} BattleFxPuffParticle;

/* Task-slot record for the Limit Mog hit-spark emitter: frame count, target slot and bone index. */
typedef struct BattleFxLimitMogDanceHitSparks {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0xe];
    short targetSlot;
    short boneIndex;
    short spriteFlags;
} BattleFxLimitMogDanceHitSparks;

/* Limit Mog animated sprite task slot: timer, position, sprite cell and scale. */
typedef struct BattleFxLimitMogDanceSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x3];
    char cellY;
    short cellWord;
    short scale;
    unsigned char pad_12[0x4];
    short flags;
} BattleFxLimitMogDanceSlot;

/* Limit Mog spark particle slot with a random texture row and coordinate. */
typedef struct BattleFxLimitMogDanceLivewireParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x3];
    unsigned char texRow;
    short texCoord;
    short scale;
    unsigned char pad_12[0x4];
    short spriteFlags;
} BattleFxLimitMogDanceLivewireParticle;

/* Puff emitter task record: frame counter and emitter base position. */
typedef struct BattleFxPuffEmitter {
    short actorIndex;
    short frame;
    short posX;
    short posY;
    short posZ;
} BattleFxPuffEmitter;

/* Timeline record for the Limit Mog spawn: the target centroid and the acting actor's yaw. */
typedef struct BattleLimitMogDanceSpawnRecord {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short yaw;
} BattleLimitMogDanceSpawnRecord;

#pragma pack(push, 1)
typedef struct BattleFxPuffColorDesc {
    char puffColorByte0; /* +0x0 */
    char puffColorByte1; /* +0x1 */
    char puffColorByte2; /* +0x2 */
    unsigned char pad_3[0x5];
    int puffSceneObject; /* +0x8 */
    unsigned char pad_c[0x4];
} BattleFxPuffColorDesc;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxPuffSpriteQuad {
    unsigned char pad_0[0x4];
    char puffSpriteAngle; /* +0x4 */
    unsigned char pad_5[0x3];
} BattleFxPuffSpriteQuad;
#pragma pack(pop)

/* --- battle_fx_builtin_ring_burst --- */
/* Particle state for the built-in ring-burst effect: colour step, position and billboard angle. */
typedef struct BattleFxRingBurstParticle {
    short ownerSlot;
    short colorIndex;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short spinAngle;
    unsigned char pad_12[0x2];
    short velocityY;
    unsigned char pad_16[0x2];
    short velocityX;
    short angle;
    short angularStep;
} BattleFxRingBurstParticle;

/* Task slot for the built-in ring burst emitter; counts steps up to 10. */
typedef struct BattleFxRingBurstEmitterSlot {
    short status;
    short step;
    short recordType;
} BattleFxRingBurstEmitterSlot;

/* Battle task slot for the built-in ring burst emitter, with three random-set fields. */
typedef struct BattleRingBurstEmitterSlot {
    unsigned char pad_00[0x18];
    short radius;
    short speed;
    short offset;
} BattleRingBurstEmitterSlot;

/* Emitter record for the ring-burst effect, one per target. */
typedef struct BattleFxRingBurstEmitter {
    unsigned char pad_00[0x4];
    short actorIndex;
    unsigned char pad_06[0x4];
    short posY;
} BattleFxRingBurstEmitter;

#pragma pack(push, 1)
typedef struct BattleFxRingBurstSpriteQuad {
    unsigned char pad_0[0x4];
    char ringBurstSpriteColor; /* +0x4 */
    unsigned char pad_5[0xb];
    int ringBurstSceneObject; /* +0x10 */
    unsigned char pad_14[0x4];
} BattleFxRingBurstSpriteQuad;
#pragma pack(pop)

/* --- battle_fx_draw_polygon_block --- */
/* GPU polygon packet written into the render-state block by BattleFx_DrawPsxPolygonBlock. */
typedef struct BattleFxPolygonBlockPrim {
    int tag;
    union {
        int colorCmd;
        struct {
            unsigned char rgb[3];
            char gpuCode;
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
} BattleFxPolygonBlockPrim;

/* PSX polygon primitive in the render-state block of the battle polygon draw; tag word 0x07000000. */
typedef struct BattleFxPsxPolygonBlockPrim {
    int tag;
    union {
        int color;
        struct {
            unsigned char rgb[3];
            char gpuCode;
        };
    };
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    short uv1;
    short bits;
    unsigned char pad_18[0x4];
    short uv2;
} BattleFxPsxPolygonBlockPrim;

/* PSX polygon block: vertex slot offsets and a primitive word for drawing polygons. */
typedef struct BattlePsxPolygonBlock {
    short vertexOffsetA;
    short vertexOffsetB;
    short vertexOffsetC;
    short vertexOffsetD;
    int primWord;
    union {
        struct {
            short attrA;
            short attrB;
        };
        int color1; /* the Gouraud streams hold three more colour words here */
    };
    union {
        struct {
            short pointOffsetA;
            short pointOffsetB;
        };
        int color2;
    };
    union {
        struct {
            short pointOffsetC;
            short pointOffsetD;
        };
        int color3;
    };
    short pointOffsetE;
    short nextVertexOffsetB;
    short nextVertexOffsetC;
} BattlePsxPolygonBlock;

/* --- battle_fx_hitmark_effect --- */
/* Hit-mark sprite slot in the Effect60 array: state, frame counter and drift offsets. */
typedef struct BattleFxHitMarkSpriteSlot {
    short flags;
    short frame;
    unsigned char position[8];
    unsigned char pad_0c[0x6];
    short scale;
    short offsetX;
    short offsetY;
    short pushLength;
    short clutWord;
} BattleFxHitMarkSpriteSlot;

/* Hit-mark sprite record: resolved object handles, colour word and sprite shorts. */
typedef struct BattleFxHitMarkSprite {
    int hitmarkObject;
    int sceneObject;
    int colorCode;
    short frame;
    short clutWord;
} BattleFxHitMarkSprite;

/* --- battle_fx_sbaria_barrier --- */
/* Fading actor model task state for the S-Baria effect. */
typedef struct BattleFxSBariaFadeModel {
    short ownerSlot;
    short frame;
    int model;
    short actorIndex;
} BattleFxSBariaFadeModel;

/* Secondary-slot record for a fading actor model in the S-Baria effect. */
typedef struct BattleFxSBariaFadingActor {
    unsigned char pad_00[0x4];
    int model;
    short actorIndex;
} BattleFxSBariaFadingActor;

#pragma pack(push, 1)
typedef struct BattleFxSecondaryModelDrawRequest {
    int modelHandle; /* +0x0 */
    int drawFlags; /* +0x4 */
    unsigned char pad_8[0x2];
    short fadeAlpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleFxSecondaryModelDrawRequest;
#pragma pack(pop)

/* --- battle_fx_throw_aeris --- */
/* Aeris weapon arc projectile slot: position, velocities, arc term and spin angle. */
typedef struct BattleFxAerisArcProjectile {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    short arcVelocity;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    short angleVelocity;
} BattleFxAerisArcProjectile;

/* Effect-array slot for an Aeris arc projectile; velocity is the delta vector scaled by q. */
typedef struct BattleFxAerisArcSlot {
    unsigned char pad_00[0xa];
    short arcVelocity;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    short angleVelocity;
} BattleFxAerisArcSlot;

/* --- battle_fx_throw_barret --- */
/* Barret straight projectile slot: position, velocities, wobble angle and angular velocity. */
typedef struct BattleFxBarretStraightProjectile {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    short angle;
    short velocityX;
    short velocityY;
    short velocityZ;
    short angleVelocity;
} BattleFxBarretStraightProjectile;

/* Barret straight throw projectile: base position and scaled offset per axis. */
typedef struct BattleFxBarretThrowStraight {
    unsigned char pad_00[0x4];
    short baseX;
    short baseY;
    short baseZ;
    short angle;
    short offsetX;
    short offsetY;
    short offsetZ;
    short angleVelocity;
} BattleFxBarretThrowStraight;

/* Barret tumbling throw projectile: scaled offset per axis and random tumble values. */
typedef struct BattleFxBarretThrowTumble {
    unsigned char pad_00[0xa];
    short angle;
    short offsetX;
    short offsetY;
    short offsetZ;
    short angleRate;
    short spinAngleX;
    short lifetime;
    short spinAngleZ;
    short tumbleRate;
} BattleFxBarretThrowTumble;

/* Barret's throw source: the bone's world position and the actor's yaw. */
typedef struct BattleFxThrowSourcePos {
    short x;
    short y;
    short z;
    short yaw;
} BattleFxThrowSourcePos;

/* --- battle_fx_throw_cait_sith --- */
/* Cait Sith thrown weapon record with offsets along the throw delta vector. */
typedef struct BattleFxCaitSithThrownWeapon {
    unsigned char pad_00[0xa];
    short angle;
    short offsetX;
    short offsetY;
    short offsetZ;
    short angleRate;
    short countdown;
    short countdownStart;
    short rotationZ;
    short lifetime;
} BattleFxCaitSithThrownWeapon;

/* A bone world position (short x, y, z) followed by the actor's yaw. */
typedef struct BattleFxCaitSithBonePos {
    short x;
    short y;
    short z;
    short yaw;
} BattleFxCaitSithBonePos;

/* The task slot of BattleFx_ThrowCaitSithWeaponsSpawnTimeline: frees itself, or becomes the flight timeline's slot. */
typedef struct BattleFxCaitSithSpawnSlot {
    short ownerSlot;
    unsigned char pad_02[0x1a];
    short unusedWord;
} BattleFxCaitSithSpawnSlot;

/* --- battle_fx_throw_cid --- */
/* Thrown Cid weapon model task record: end flag, frame, integrated position and angle. */
typedef struct BattleFxThrownModel {
    short endFlag;
    short frame;
    short posA;
    short posB;
    short posC;
    short angle;
    short velA;
    short velB;
    short velC;
    short angleVel;
} BattleFxThrownModel;

/* Cid kind-0 thrown weapon record with offsets from the target index. */
typedef struct BattleFxCidKind0ThrownWeapon {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    short angle;
    short offsetX;
    short offsetY;
    short offsetZ;
    short angleVel;
} BattleFxCidKind0ThrownWeapon;

/* Cid kind-2 thrown weapon record with offsets from the target index. */
typedef struct BattleFxCidKind2ThrownWeapon {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_12[0xa];
    short trailIndex;
} BattleFxCidKind2ThrownWeapon;

/* Effect record of the Cid weapon kind-2 trail ribbon task: frame counter and weapon index. */
typedef struct BattleFxCidTrailRecord {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x18];
    short weaponIndex;
} BattleFxCidTrailRecord;

/* Position of the Cid weapon one step back along its flight (position minus offset). */
typedef struct BattleFxCidThrowStep {
    short x;
    short y;
    short z;
} BattleFxCidThrowStep;

/* Thrown-weapon state for Cid: a target scale and two scaled delta components. */
typedef struct BattleFxCidThrowState {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetScaled;
    short deltaX;
    short deltaY;
} BattleFxCidThrowState;

/* One ribbon of g_BattleThrowCidTrailRibbonParams (0x34 bytes): draw colours and the angle, local position
 * and spread steps of the trail segments. */
typedef struct BattleThrowCidRibbonParams {
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char pad_03[0x1];
    union {
        int baseColor;
        struct {
            char baseColorR;
            char baseColorG;
            char baseColorB;
            unsigned char pad_07;
        };
    };
    int startAngle;
    int anglePerFrame;
    int anglePerSegment;
    unsigned char pad_14[0x4];
    int localYStart;
    int localYStep;
    unsigned char pad_20[0x4];
    int halfSpreadStart;
    int halfSpreadStep;
    unsigned char pad_2C[0x4];
    int localZ;
} BattleThrowCidRibbonParams;

/* Scratch block of the Cid weapon trail ribbon (see BattleFx_ThrowCidWeaponsKind2TrailRibbonTask): the two
 * projected end points of a segment, their midpoint, the projected vertices and the axis rotation. */
typedef struct BattleFxThrowCidTrailRibbon {
    unsigned char pad_00[0x4];
    int projP; /* RotTransPers interpolation factor out */
    int depth;
    int projFlag; /* RotTransPers flag out */
    short aX;
    short aY;
    short aZ;
    unsigned char pad_16[0x2];
    short bX;
    short bY;
    short bZ;
    unsigned char pad_1E[0x2];
    short midX;
    short midY;
    short midZ;
    unsigned char pad_26[0xa];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_36[0x2];
    short accX;
    short accY;
    short accZ;
    unsigned char pad_3E[0x2];
    int vertex0;
    int vertex1;
    int vertex2;
    int vertex3;
    int prevDepth;
    int unshadedColor;
    int baseValue;
    int prevColor;
    int packedPos;
    unsigned char pad_64[0x4];
    int offset;
    unsigned char pad_6C[0x4];
    short rotX;
    short rotY;
    short edgeCoord; /* rotZ */
    unsigned char pad_76[0x2];
    PsxMatrix axisMatrix;
    short edgeMatrix[9]; /* rotation of (rotX, rotY, edgeCoord) */
    unsigned char pad_A8[0xc];
    int driftX;
    int driftY;
    int driftZ;
    unsigned char pad_C0[0x4];
    VECTOR axisVector;
} BattleFxThrowCidTrailRibbon;

/* --- battle_fx_throw_vincent --- */
/* Effect-array record for Vincent's thrown weapons: position, velocity, spin angle and timer. */
typedef struct BattleFxVincentThrowRecord {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    short angle;
    short velocityX;
    short velocityY;
    short velocityZ;
    short spinRate;
    short angleX;
    short angleXStep;
    short angleZ;
    short angleZStep;
} BattleFxVincentThrowRecord;

/* Vincent thrown weapon record with offsets along the throw delta vector. */
typedef struct BattleFxVincentThrownWeapon {
    unsigned char pad_00[0x4];
    union {
        Int64Pair position; /* the launch pose; its last short overlaps angle */
        struct {
            unsigned char pad_04[0x6];
            short angle;
        };
    };
    short offsetX;
    short offsetY;
    short offsetZ;
    short spinRate;
    short angleX;
    short angleXStep;
    short angleZ;
    short angleZStep;
} BattleFxVincentThrownWeapon;

/* --- battle_fx_throw_yuffie --- */
/* The throw state the Yuffie weapon tasks share: the order the Kind 3 timeline visits its targets in, and,
 * overlapping the end of that list, the throw's flight kind, weapon model and target count. */
typedef union BattleFxYuffieThrowState {
    int order[10];
    struct {
        unsigned char pad_00[0x18];
        int flightKind;
        int weaponModel;
        int targetCount;
    };
} BattleFxYuffieThrowState;

/* One target of the Kind 3 throw: its actor, the world position of its bone, the radius the throws scatter
 * over, and how many throws it has had. */
typedef struct BattleFxYuffieKind3Target {
    int actorIndex;
    short boneX;
    short boneY;
    short boneZ;
    short spread;
    int throwCount;
    unsigned char pad_10[0x4];
} BattleFxYuffieKind3Target;

/* Yuffie thrown weapon in flight: timer, position and velocity per axis, and spin angle. */
typedef struct BattleFxYuffieThrownWeapon {
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
    short angle;
    short spin;
} BattleFxYuffieThrownWeapon;

/* Yuffie weapon trail slot: frame age, position, velocity, rotation angle and lifetime. */
typedef struct BattleFxYuffieWeaponTrail {
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
    short angle;
    short angularVelocity;
    unsigned char pad_18[0x4];
    short lifetime;
} BattleFxYuffieWeaponTrail;

/* Yuffie kind-0 thrown weapon model: position, velocity, frame counter and accumulator with step. */
typedef struct BattleFxYuffieKind0ThrownRecord {
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
    short accumA;
    short stepA;
} BattleFxYuffieKind0ThrownRecord;

/* Yuffie kind-2 timeline record: randomized position jitter, velocity from delta vectors, and accumulator setup. */
typedef struct BattleFxYuffieKind2TimelineRecord {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos;
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x2];
    short accumA;
    short stepA;
    unsigned char pad_18[0x4];
    short fadeLevel;
} BattleFxYuffieKind2TimelineRecord;

/* Yuffie's world position (the bone lookup writes the three shorts) and her yaw. */
typedef struct BattleFxThrowYuffieSourcePos {
    short sourceX;
    short sourceY;
    short sourceZ;
    short yaw;
} BattleFxThrowYuffieSourcePos;

/* World position of the target's bone; the throw aims from the source position to it. */
typedef struct BattleFxYuffieThrowTarget {
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_6[0x2];
} BattleFxYuffieThrowTarget;

/* Thrown Yuffie kind 0 weapon model: velocity components set by its timeline task. */
typedef struct BattleFxYuffieKind0ThrownModel {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    short spin;
} BattleFxYuffieKind0ThrownModel;

/* Kind 3 thrown weapon: start position, per-axis velocity and lifetime. */
typedef struct BattleFxYuffieKind3Projectile {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_12[0xa];
    short lifetime;
} BattleFxYuffieKind3Projectile;

/* --- battle_fx_weapon2_ring_grid --- */
/* One ring-grid vertex: the SVECTOR the quads' corner pointers address. */
typedef struct BattleFxSwirlVertex {
    short x;
    short y;
    short z;
    short pad;
} BattleFxSwirlVertex;

/* A textured, Gouraud-shaded GPU quad packet (0x34 bytes): tag word, then a colour, a corner
 * and a texture coordinate pair per corner. */
typedef struct BattleFxSwirlPacket {
    int code;
    int color0;
    int xy0;
    char u0;
    char v0;
    short clut;
    int color1;
    int xy1;
    char u1;
    char v1;
    short tpage;
    int color2;
    int xy2;
    char u2;
    char v2;
    unsigned char pad_26[0x2];
    int color3;
    int xy3;
    char u3;
    char v3;
    unsigned char pad_32[0x2];
} BattleFxSwirlPacket;

/* One grid cell: the four corner pointers, then six packets (the textured set, a spare, the shaded set). */
typedef struct BattleFxSwirlCell {
    BattleFxRingGridCell corners;
    BattleFxSwirlPacket packets[6];
} BattleFxSwirlCell;

/* The stage swirl grid buffer: seventeen rings of sixteen vertices, the 16 x 16 cells, and a colour word per ring. */
typedef struct BattleFxSwirlGrid {
    BattleFxSwirlVertex ring[17][16];
    BattleFxSwirlCell cells[16][16];
    int rowColor[17];
} BattleFxSwirlGrid;

/* Camera-script task record for Weapon2: the script cursor, the matrix the tracks are placed in, and the owning actor. */
typedef struct BattleFxWhirlsandCameraTask {
    short ownerSlot;
    short started;
    unsigned char pad_04[0x8];
    short *scriptPos;
    PsxMatrix *rotMatrix;
    short actor;
} BattleFxWhirlsandCameraTask;

/* Spiral-away scratch for a Weapon2 actor (Gfx_GetPsxScratchpadPtr(0)): offset, rotated copy and its matrix. */
typedef struct BattleFxWhirlsandSpiralOffset {
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_06[0x2];
    short posX;
    unsigned char pad_0A[0x2];
    short posZ;
    unsigned char pad_0E[0x2];
    PsxMatrix matrix;
} BattleFxWhirlsandSpiralOffset;

#pragma pack(push, 1)
/* Gfx_GetPsxScratchpadPtr(0) as BattleFx_WhirlsandSurfaceParticleTask uses it. */
typedef struct BattleFxWhirlsandParticleScratch {
    short angle[3];
    unsigned char pad_06[0x2];
    PsxMatrix matrix;
    unsigned char pad_26[0x2];
    int projectResult;
    int projectDepth;
    int projectFlags;
} BattleFxWhirlsandParticleScratch;

/* Gfx_GetPsxScratchpadPtr(0) as BattleFx_WhirlsandSetActorGroundHeightAndDrawShadow uses it. */
typedef struct BattleFxWhirlsandRideScratch {
    short angle[3];
    unsigned char pad_06[0x2];
    int scale[3];
    unsigned char pad_14[0x4];
    PsxMatrix matrix;
} BattleFxWhirlsandRideScratch;

/* Gfx_GetPsxScratchpadPtr(0) as the two ring grid draw tasks use it. */
typedef struct BattleFxWhirlsandGridScratch {
    unsigned char pad_00[0x18];
    int depth;
    int projectDepth;
    int projectFlags;
    PsxMatrix matrix;
    int scale[3];
    unsigned char pad_4E[0x4];
    CVECTOR color;
} BattleFxWhirlsandGridScratch;

/* A flat-shaded quad packet from Gfx_GetPsxPacketScratchPtr. */
typedef struct BattleFxWhirlsandQuadPacket {
    int code;
    CVECTOR color;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    short x3;
    short y3;
} BattleFxWhirlsandQuadPacket;

/* A surface particle task slot. */
typedef struct BattleFxWhirlsandSurfaceParticleTaskSlot {
    short ownerSlot;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_a[0x4];
    short velocityY;
} BattleFxWhirlsandSurfaceParticleTaskSlot;
#pragma pack(pop)

/* --- battle_fx_weapon3_projectile --- */
/* Weapon3 timeline task record: a frame timer and an animation frame index. */
typedef struct BattleFxEmeraldShootTask {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x18];
    short animFrame;
} BattleFxEmeraldShootTask;

/* Battle FX task slot for the Weapon 3 fading spinning model: angle, angular speed and timer. */
typedef struct BattleFxEmeraldShootFadingSpinTask {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angularSpeed;
    short angleZ;
    short angularSpeedZ;
    short scale;
    short scaleStep;
    unsigned char pad_18[0x4];
    short timer;
    short scaleGrowFrames;
} BattleFxEmeraldShootFadingSpinTask;

/* Weapon 3 timeline effect slot: emitter-relative position, kind, lifetime and rate. */
typedef struct BattleFxEmeraldShootParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angularSpeed;
    short halfAngle;
    short angularSpeedZ;
    short angleOffset;
    short rate;
    unsigned char pad_18[0x4];
    short kind;
    short lifetime;
} BattleFxEmeraldShootParticle;

/* Weapon3 stretching sprite record: timer, stretch value with growth rate, and an angle with step. */
typedef struct BattleFxEmeraldShootStretchRecord {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x10];
    short stretch;
    short stretchRate;
    short angle;
    short angleStep;
} BattleFxEmeraldShootStretchRecord;

/* Weapon3 impact trail point: position, per-step velocity and slot index. */
typedef struct BattleFxEmeraldShootImpactTrail {
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
    short expandStep;
    short slotIndex;
    unsigned char pad_1A[0x2];
    short variant;
} BattleFxEmeraldShootImpactTrail;

/* Weapon3 expanding model record: step, expansion level and expansion step. */
typedef struct BattleFxEmeraldShootExpandingRecord {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short expandLevel;
    short expandStep;
} BattleFxEmeraldShootExpandingRecord;

/* Weapon3 growing model record: step, three positions, expansion level and expansion step. */
typedef struct BattleFxEmeraldShootGrowingRecord {
    short ownerSlot;
    short step;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0xa];
    short expandLevel;
    short expandStep;
} BattleFxEmeraldShootGrowingRecord;

/* Sprite slot of the Weapon3 trail: position and velocity steps, a timer and a trail index. */
typedef struct BattleFxEmeraldShootTrailSprite {
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
    short trailIndex;
} BattleFxEmeraldShootTrailSprite;

/* Weapon impact trail spawner: step counter, target position and slot index. */
typedef struct BattleFxWeaponImpactTrail {
    short ownerSlot;
    short step;
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_0A[0x12];
    short slotIndex;
} BattleFxWeaponImpactTrail;

/* Delayed frame sprite record of the third weapon effect: a frame counter and a countdown delay. */
typedef struct BattleFxEmeraldShootDelayedFrameSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
    unsigned char pad_16[0x6];
    short delay;
} BattleFxEmeraldShootDelayedFrameSprite;

/* Stretching sprite emitter record in the BattleFx effect array, with random x and y offsets. */
typedef struct BattleFxStretchSprite {
    unsigned char pad_00[0x4];
    int offsetX;
    int offsetY;
    int offsetZ;
    unsigned char pad_10[0x4];
    short stretch;
    short stretchRate;
    short angle;
    short angleStep;
} BattleFxStretchSprite;

/* --- battle_fx_weapon_beam1 --- */
/* Effect100 slot record for the shrinking weapon beam: frame counter, length and shrink speed. */
typedef struct BattleFxWeaponBeamShrink {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    short length;
    short shrinkSpeed;
} BattleFxWeaponBeamShrink;

/* Effect record of the weapon beam 1 timeline tasks, with a random variant index. */
typedef struct BattleFxEmeraldBeamTimelineRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    short shrinkSpeed;
    unsigned char pad_10[0x4];
    short variant;
} BattleFxEmeraldBeamTimelineRecord;

/* Delayed sprite record of the weapon beam: a frame counter and a countdown delay. */
typedef struct BattleFxWeaponBeamDelayedSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    unsigned char pad_06[0x6];
    short scale;
    unsigned char pad_0E[0x6];
    short delay;
} BattleFxWeaponBeamDelayedSprite;

/* Timeline record for the WeaponBeam1 effect. */
typedef struct BattleFxWeaponEmeraldBeamTimeline {
    short ownerSlot;
    unsigned char pad_02[0x12];
    short beamState;
} BattleFxWeaponEmeraldBeamTimeline;

/* A world position and the radius that goes with it. */
typedef struct {
    short posX;
    short posY;
    short posZ;
    short radius;
} WeaponFxEmeraldBeamPoint;

#pragma pack(push, 1)
typedef struct WeaponFxEmeraldBeamDelayedSpriteReq {
    int sprite; /* +0x0 */
    int drawObj; /* +0x4 */
    char r; /* +0x8 */
    char g; /* +0x9 */
    char b; /* +0xa */
    unsigned char pad_b[0x5];
} WeaponFxEmeraldBeamDelayedSpriteReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WeaponFxEmeraldBeamFadingSpriteReq {
    int sprite; /* +0x0 */
    int drawObj; /* +0x4 */
    char r; /* +0x8 */
    char g; /* +0x9 */
    char b; /* +0xa */
    unsigned char pad_b[0x1];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} WeaponFxEmeraldBeamFadingSpriteReq;
#pragma pack(pop)

/* --- battle_fx_weapon_diamond --- */
/* Particle record of the WeaponDiaU timeline: position, velocity, angle and variant. */
typedef struct BattleFxDiamondFlashParticle {
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
    short variant;
    short fadeTimer;
    short convergeDistance;
} BattleFxDiamondFlashParticle;

/* Battle FX damped sprite task slot for the Weapon Dia U effect: spring positions with friction. */
typedef struct BattleFxWeaponDiamondFlashSpriteTask {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short tableIndex;
    short timer;
} BattleFxWeaponDiamondFlashSpriteTask;

/* Dia U trailing projectile record: step counter and three position/velocity pairs. */
typedef struct BattleFxDiamondFlashTrail {
    short ownerSlot;
    short step;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velocityA;
    short velocityB;
    short velocityC;
} BattleFxDiamondFlashTrail;

/* Weapon Dia-U billboard sprite: position, decaying velocity and sprite index. */
typedef struct BattleFxDiamondFlashBillboardRecord {
    short ownerSlot;
    short spriteIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x4];
    short active;
} BattleFxDiamondFlashBillboardRecord;

/* Dia U triple-model streak: step counter, position, velocity and a draw table index. */
typedef struct BattleFxDiamondFlashStreak {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short tableIndex;
} BattleFxDiamondFlashStreak;

/* Weapon Dia inward model state: frame counter, position and per-frame velocity. */
typedef struct BattleFxWeaponDiaInwardModel {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxWeaponDiaInwardModel;

/* Battle task slot of the Dia U pulsing model tasks: frame counter and position. */
typedef struct BattleFxDiamondFlashPulsingSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxDiamondFlashPulsingSlot;

/* Effect-array slot for Dia's converging weapon model; frameCount steps over 15 frames. */
typedef struct BattleFxDiaConvergeSlot {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x8];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_12[0x8];
    short projectedPosZ;
} BattleFxDiaConvergeSlot;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxDiamondFlashWeaponRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x14];
    short actorIdx;
} BattleFxDiamondFlashWeaponRecord;

/* Trail point copied from the Dia U trail model's position record. */
typedef struct BattleFxWeaponDiamondFlashTrailPoint {
    unsigned char pad_00[0x4];
    short posA;
    short posB;
    short posC;
} BattleFxWeaponDiamondFlashTrailPoint;

/* Model draw request for the Dia weapon shrinking model pair; model holds the model handle. */
typedef struct BattleFxWeaponDiamondFlashShrinkRequest {
    int model;
    unsigned char pad_04[0x6];
    short shrinkAmount;
    unsigned char pad_c[0x4];
} BattleFxWeaponDiamondFlashShrinkRequest;

/* Scale vector for the triple-model DiaU streak effect. */
typedef struct BattleFxDiamondFlashTripleScaleVector {
    int scaleX;
    unsigned char pad_04[0x4];
    int scale;
} BattleFxDiamondFlashTripleScaleVector;

#pragma pack(push, 1)
typedef struct WeaponFxDiamondFlashBillboardSpriteReq {
    int spriteTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short count; /* +0xc */
    short param; /* +0xe */
} WeaponFxDiamondFlashBillboardSpriteReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WeaponFxDiamondFlashConvergingDrawReq {
    unsigned char pad_0[0x10];
    int modelDrawReq; /* +0x10 */
    unsigned char pad_14[0x6];
    short angleParam; /* +0x1a */
} WeaponFxDiamondFlashConvergingDrawReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WeaponFxDiamondFlashInwardModelReq {
    unsigned char pad_0[0x8];
    int billboardDrawReq; /* +0x8 */
    unsigned char pad_c[0x6];
    short billboardScaleParam; /* +0x12 */
    unsigned char pad_14[0xc];
    int inwardModelDrawReq; /* +0x20 */
} WeaponFxDiamondFlashInwardModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WeaponFxDiamondFlashPulsingOrientedDrawReq {
    int model; /* +0x0 */
    unsigned char pad_4[0x6];
    short param; /* +0xa */
} WeaponFxDiamondFlashPulsingOrientedDrawReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WeaponFxDiamondFlashTrailModelDrawReq {
    unsigned char pad_0[0x8];
    int resource; /* +0x8 */
    unsigned char pad_c[0x6];
    short param; /* +0x12 */
} WeaponFxDiamondFlashTrailModelDrawReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WeaponFxDiamondFlashTrailingDrawReqComposite {
    unsigned char pad_0[0x10];
    int modelResource; /* +0x10 */
    unsigned char pad_14[0x6];
    short cosParam; /* +0x1a */
    unsigned char pad_1c[0x4];
    int spriteRecord; /* +0x20 */
    int spriteObject; /* +0x24 */
} WeaponFxDiamondFlashTrailingDrawReqComposite;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WeaponFxDiamondFlashTrailingSpriteParams {
    char r; /* +0x0 */
    char g; /* +0x1 */
    char b; /* +0x2 */
    unsigned char pad_3[0x1];
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
} WeaponFxDiamondFlashTrailingSpriteParams;
#pragma pack(pop)

/* --- battle_fx_weapon_tumble_burst --- */
/* Particle of the Weapon4 timeline: speed, lifetime, step size and variant. */
typedef struct BattleFxRevengeStampParticle {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleY;
    short angleYStep;
    unsigned char pad_10[0x4];
    short speed;
    short centerVelocity;
    short lifetime;
    short stepSize;
    short bufferIndex;
} BattleFxRevengeStampParticle;

/* Tumble state for one actor in the Weapon 4 effect: position, vertical velocity, frame count. */
typedef struct BattleFxRevengeStampTumble {
    short ownerSlot; /* +0x0 */
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short boneMinY;
    unsigned char pad_0E[0x4];
    short currentY;
    short rotVelX;
    short rotVelZ;
    short velocityY;
    short accelY;
    short actorIdx;
} BattleFxRevengeStampTumble;

/* Weapon 4 green gradient strip: center, width, rates, frame count and duration. */
typedef struct BattleFxGreenGradientStrip {
    short ownerSlot;
    short frameCount;
    short center;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short width;
    short centerVelocity;
    short duration;
    short widthRate;
} BattleFxGreenGradientStrip;

/* Weapon4 stretching model slot: elapsed frames, stretch and height with their steps and a countdown. */
typedef struct BattleFxRevengeStampStretchSlot {
    short ownerSlot;
    short elapsed;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotY;
    short rotYStep;
    unsigned char pad_10[0x4];
    short scaleXZ;
    unsigned char pad_16[0x2];
    short height;
    short heightStep;
    short countdown;
} BattleFxRevengeStampStretchSlot;

/* Weapon4 timeline record: timer and a counter copied into the spawned sprite record. */
typedef struct BattleFxRevengeStampTimeline {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x18];
    short counter;
} BattleFxRevengeStampTimeline;

/* Blue gradient strip record: strip centre, width that shrinks per frame, and a step limit. */
typedef struct BattleFxBlueGradientStrip {
    short ownerSlot;
    short step;
    short centerA;
    short centerB;
    short posZ;
    unsigned char pad_0A[0xa];
    short width;
    unsigned char pad_16[0x2];
    short endStep;
    short widthRate;
} BattleFxBlueGradientStrip;

/* Gray gradient strip effect record in the battle effect-100 pool, 32-byte slot. */
typedef struct BattleFxGrayGradientStrip {
    short ownerSlot;
    short timer;
    short centerX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short width;
    unsigned char pad_16[0x2];
    short duration;
    short shrinkStep;
} BattleFxGrayGradientStrip;

/* Effect100 slot record of the rising spinning weapon-4 model: frame, height, angles, model index and fade. */
typedef struct BattleFxRevengeStampRisingModel {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short velocityY;
    short spinAngle;
    short accelerationY;
    short scale;
    short angleStep;
    short modelIndex;
    short spinStep;
    short lifetimeFrames;
    short fadeStartFrame;
} BattleFxRevengeStampRisingModel;

/* Weapon 4 piece record: random spawn offsets, angles and timing values. */
typedef struct BattleFxRevengeStampPieceRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    short prevAngle;
    short angle;
    short velocityY;
    short startAngle;
    short accelerationY;
    short scale;
    short angleStep;
    short modelIndex;
    short spinStep;
    short lifetimeOrActor;
    short fadeStartFrame;
} BattleFxRevengeStampPieceRecord;

/* Effect-array slot for a Weapon4 bone particle; bufferIndex selects its particle buffer. */
typedef struct BattleFxRevengeStampParticleSlot {
    short ownerSlot; /* +0x0 */
    short timer;
    unsigned char pad_04[0x18];
    short bufferIndex;
} BattleFxRevengeStampParticleSlot;

/* One sprite particle in the weapon-4 burst buffers: slot state, frame, position, velocity and scale. */
typedef struct BattleFxRevengeStampBufferParticle {
    short active;
    short frame;
    short position;
    short height;
    short depth;
    unsigned char pad_0A[0x2];
    short velocity;
    short heightVelocity;
    short depthVelocity;
    unsigned char pad_12[0x4];
    short scale;
} BattleFxRevengeStampBufferParticle;

#endif /* FF7_TYPES_BATTLE_FX_H */

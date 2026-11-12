/* include/ff7/types/battle_special.h: Enemy special-attack battle effect types. */
#ifndef FF7_TYPES_BATTLE_SPECIAL_H
#define FF7_TYPES_BATTLE_SPECIAL_H

#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"

/* --- battle_fx_special_atmic --- */
/* Atmic stretch sprite task slot: frame counter. */
typedef struct BattleFxAtomicRayStretchSlot {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x6];
    short lengthScale;
} BattleFxAtomicRayStretchSlot;

/* Saved x, y, z origin of the special atmic effect, subtracted from the timeline frame position. */
typedef struct BattleFxAtomicRayOrigin {
    unsigned char pad_00[0x4];
    short savedX;
    short savedY;
    short savedZ;
} BattleFxAtomicRayOrigin;

/* Slots BattleFx_SpecialAtomicRayTimelineTask allocates for BattleFx_SpecialAtmic{FlickerSprite, 6 more}Task. */
typedef struct BattleFxAtomicRayTimelineSpawnSlot {
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
    short variantIndex;
    short scale;
} BattleFxAtomicRayTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialAtomicRayLayeredModelsTask. */
typedef struct BattleFxAtomicRayLayeredModelsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x6];
    short stepCount;
} BattleFxAtomicRayLayeredModelsSlot;

/* The battle task slot of BattleFx_SpecialAtomicRayOrbitSpriteTask. */
typedef struct BattleFxAtomicRayOrbitSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxAtomicRayOrbitSpriteSlot;

/* Atmic variant sprite task state: end marker, frame counter and variant index. */
typedef struct BattleFxAtomicRaySpriteState {
    short endMarker;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variant;
} BattleFxAtomicRaySpriteState;

/* Atmic flicker effect record holding two values read from its loaded resource table. */
typedef struct BattleFxAtomicRayColorRecord {
    unsigned char pad_00[0x8];
    struct BattleFxSpriteGroup *spriteGroup;
    int drawObject;
} BattleFxAtomicRayColorRecord;

#pragma pack(push, 1)
typedef struct BattleSpecialAtomicRayAnimConfig {
    unsigned char pad_0[0x8];
    int modelDraw; /* +0x8 */
    int flags; /* +0xc */
    unsigned char pad_10[0x2];
    short scale; /* +0x12 */
    unsigned char pad_14[0x4];
    int spriteRecord; /* +0x18 */
    int spriteObject; /* +0x1c */
} BattleSpecialAtomicRayAnimConfig;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialAtomicRayBuffer {
    int scaleFactor; /* +0x0 */
    unsigned char pad_4[0x4];
    PsxMatrix matrix; /* +0x8: the look-at basis; its translation is the saved origin */
} BattleSpecialAtomicRayBuffer;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialAtomicRayColor2 {
    int spriteField; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    short frameTable; /* +0x10 */
    unsigned char pad_12[0x2];
} BattleSpecialAtomicRayColor2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialAtomicRayColor3 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0xd];
} BattleSpecialAtomicRayColor3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialAtomicRayModelIndices {
    unsigned char pad_0[0x10];
    int firstModelRes; /* +0x10 */
    unsigned char pad_14[0x6];
    short indexWord26; /* +0x1a */
    unsigned char pad_1c[0x4];
    int secondModelRes; /* +0x20 */
    unsigned char pad_24[0x6];
    short indexWord42; /* +0x2a */
    unsigned char pad_2c[0x4];
    short indexWord48; /* +0x30 */
    unsigned char pad_32[0x2];
} BattleSpecialAtomicRayModelIndices;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialAtomicRayModelReq {
    int modelField; /* +0x0 */
    unsigned char pad_4[0x6];
    short angle; /* +0xa */
} BattleSpecialAtomicRayModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialAtomicRayMotionTable {
    unsigned char pad_0[0x58];
    int spriteRecord; /* +0x58 */
    int spriteObject; /* +0x5c */
} BattleSpecialAtomicRayMotionTable;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialAtomicRayScale {
    int modelField; /* +0x0 */
    unsigned char pad_4[0x6];
    short component10; /* +0xa */
} BattleSpecialAtomicRayScale;
#pragma pack(pop)

/* --- battle_fx_special_awa_second --- */
/* Slot state of the Awa2 special timeline: start and end points, moved over 20 frames. */
typedef struct BattleFxSpecialHellBubblesTask {
    short step;
    short timer;
    short startX;
    short baseX;
    short startY;
    unsigned char pad_0A[0x2];
    short endX;
    short jitterBaseX;
    short endY;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxSpecialHellBubblesTask;

/* Timeline record for the Awa2 special effect: current position and per-step deltas from start to end. */
typedef struct BattleFxHellBubblesTimeline {
    unsigned char pad_00[0x4];
    short posX;
    short jitteredX;
    short posY;
    unsigned char pad_0A[0x2];
    short stepX;
    short baseX;
    short stepY;
    unsigned char pad_12[0x2];
    short rotY;
    short stripPhase;
    short posZ;
    short phase;
} BattleFxHellBubblesTimeline;

/* Awa2 quad strip task record: frame counter and texture strip offset. */
typedef struct BattleFxHellBubblesQuadStrip {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short stripOffset;
} BattleFxHellBubblesQuadStrip;

/* What BattleFx_SpecialHellBubblesDrawPsxTexturedQuadStrip reads through Gfx_GetPsxPacketScratchPtr(). */
typedef struct BattleFxHellBubblesQuadStripRenderStateBlock {
    int tag;
    int rgbCode;
    unsigned char pad_08[0x4];
    char u0;
    char v0;
    short clut;
    unsigned char pad_10[0x4];
    char u1;
    char bandTop;
    short texturePage;
    unsigned char pad_18[0x4];
    char u2;
    char v2;
    unsigned char pad_1E[0x6];
    char u3;
    char bandBottom;
} BattleFxHellBubblesQuadStripRenderStateBlock;

/* The battle task slot of BattleFx_SpecialHellBubblesFadingSpinModelTask. */
typedef struct BattleFxHellBubblesFadingSpinModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    unsigned char pad_0E[0xa];
    short step;
    short phase;
} BattleFxHellBubblesFadingSpinModelSlot;

/* One record of g_BattleSpecHellBubblesParticleRecords (0x8 bytes). */
typedef struct BattleSpecHellBubblesParticleRecordEntry {
    short active; /*+0x00 */
    short radius; /*+0x02 */
    short angle; /*+0x04 */
    short phase; /*+0x06 */
} BattleSpecHellBubblesParticleRecordEntry;

#pragma pack(push, 1)
typedef struct BattleSpecHellBubblesParticleRecordsObject {
    unsigned char pad_0[0x6];
    int phase6; /* +0x6 */
    unsigned char pad_a[0x2];
} BattleSpecHellBubblesParticleRecordsObject;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBubblesSpriteReq1 {
    int resPtr; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int secondSpriteRecord; /* +0x10 */
    int secondDrawObj; /* +0x14 */
} BattleSpecHellBubblesSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBubblesSpriteReq2 {
    char red; /* +0x0 */
    char lowByte1; /* +0x1 */
    char highByte2; /* +0x2 */
    unsigned char pad_3[0x5];
} BattleSpecHellBubblesSpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_ayasii_effect --- */
/* Ayasii timeline record: frame counter, centre point and random spread for emitted points. */
typedef struct BattleFxShadyBreathTimeline {
    short ownerSlot;
    short frameCount;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short jitterOffset;
    short jitterRange;
    short jitterOffsetY;
    short jitterRangeY;
    short particleScale;
} BattleFxShadyBreathTimeline;

/* Light record (0x10 bytes) of the Ayasii special effect. */
typedef struct BattleFxShadyBreathLightRecord {
    short scale;
    unsigned char pad_02[0xe];
} BattleFxShadyBreathLightRecord;

/* One point of the Ayasii light ring (0x10 bytes); the ring starts at
 * g_BattleSpecShadyBreathTargetCoords.posX and the point's scale is the BattleFxShadyBreathLightRecord
 * eight bytes further on. */
typedef struct BattleFxShadyBreathLightPoint {
    short x;
    short y;
    short z;
    unsigned char pad_06[0xa];
} BattleFxShadyBreathLightPoint;

/* --- battle_fx_special_bakyu_segments --- */
/* Bakyu timeline record: step counter and start and target points for an arc. */
typedef struct BattleFxVacuumProjectile {
    short ownerSlot;
    short step;
    short startX;
    short posY;
    short startZ;
    unsigned char pad_0A[0x2];
    short targetX;
    unsigned char pad_0E[0x2];
    short targetZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxVacuumProjectile;

/* A child task of the Bakyu timeline: the model jitter task, the decelerating sprite task and the
 * segment spawner task. The timeline fills in the position, the initial velocity (a random direction
 * scaled by 0xBB8, with 0x1388 upward) and the rotation; the task then decays the velocity. */
typedef struct BattleFxVacuumChildSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityZ;
    short velocityY;
    unsigned char pad_12[0x2];
    short pitch;
    short yaw;
    short roll;
} BattleFxVacuumChildSlot;

/* What BattleFx_SpecialVacuumDrawSegmentRecord reads through g_BattleSpecVacuumPacketCursor. */
typedef struct BattleFxVacuumSegmentRecordPacketCursor {
    int tag;
    int color0;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxVacuumSegmentRecordPacketCursor;

#pragma pack(push, 1)
typedef struct BattleSpecVacuumRotVector {
    short pitch; /* +0x0 */
    short yaw; /* +0x2 */
} BattleSpecVacuumRotVector;
#pragma pack(pop)

/* The model draw request of the jittering Bakyu model: the scale vector ScaleMatrix reads, then the
 * head of a BattleFxModelDraw (BattleFx_SubmitModelDraw reads from `draw`). */
typedef struct BattleSpecVacuumModelReq {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    BattleFxModelDrawHead draw;
} BattleSpecVacuumModelReq;

#pragma pack(push, 1)
typedef struct BattleSpecVacuumSegmentRecords {
    struct BattleFxVacuumSegment segments[200]; /* the segment pool, 0x14 bytes each */
    int rotMatrixA; /* +0xfa0 */
    unsigned char pad_fa4[0xe];
    int historyA; /* +0xfb2 */
    int historyB; /* +0xfb6 */
    int historyC; /* +0xfba */
} BattleSpecVacuumSegmentRecords;
#pragma pack(pop)

/* --- battle_fx_special_burst_explosion --- */
/* Task slot for the Special B2 timeline: step, position and the record type to spawn. */
typedef struct BattleFxSpecialBlasterSlot {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0x4];
    short flag;
    short timer;
    short clearedWord;
    short recordType;
} BattleFxSpecialBlasterSlot;

/* Request record for the spinning model effect, with a three-entry alpha history and a parity flag. */
typedef struct BattleFxSpinModelReq {
    unsigned char pad_00[0x8];
    char alphaOld;
    char alphaPrev;
    char alpha;
    unsigned char pad_0B[0x1];
    short parity;
    unsigned char pad_e[0x2];
} BattleFxSpinModelReq;

/* Request record for the forward burst model effect, with a three-entry angle history and a parity flag. */
typedef struct BattleFxBlasterModelReq {
    unsigned char pad_00[0x8];
    char angleOld;
    char anglePrev;
    char angle;
    unsigned char pad_0B[0x1];
    short parity;
    unsigned char pad_e[0x2];
} BattleFxBlasterModelReq;

/* The battle task slot of BattleFx_SpecialBlasterDrawSpinModelsTask. */
typedef struct BattleFxBlasterDrawSpinModelsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short spinParam;
} BattleFxBlasterDrawSpinModelsSlot;

/* The battle task slot of BattleFx_SpecialBlasterStretchModelsTask. */
typedef struct BattleFxBlasterStretchModelsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short baseLength;
} BattleFxBlasterStretchModelsSlot;

/* Trail model slot carrying B2 colour values and state. */
typedef struct BattleFxBlasterBurstSlot {
    unsigned char pad_00[0x4];
    short colorLow;
    short colorHigh;
    short state;
} BattleFxBlasterBurstSlot;

/* Per-sprite state of the special B2 variant: frame index, position and velocity. */
typedef struct BattleFxSpriteFrameSlot {
    short step;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x4];
    short flag;
} BattleFxSpriteFrameSlot;

#pragma pack(push, 1)
typedef struct BattleSpecialBlasterColorBuffer {
    short colorLow; /* +0x0 */
    short colorHigh; /* +0x2 */
} BattleSpecialBlasterColorBuffer;
#pragma pack(pop)

/* --- battle_fx_special_chikasui --- */
/* Chikasui timeline task: its age runs to 70 frames, allocating a typed record at 60. */
typedef struct BattleFxGeyserTimeline {
    short ownerSlot;
    short age;
    unsigned char pad_04[0x8];
    short targetBonePos[3]; /* +0xC: world position of the target's bone (Battle_GetBoneWorldPos) */
    unsigned char pad_12[0x6];
    short actorIndex;
} BattleFxGeyserTimeline;

#pragma pack(push, 1)
/* The first two components of the source bone's world position; the third is
 * g_BattleSpecGeyserBonePosZ. */
typedef struct BattleSpecGeyserBonePos {
    short x;
    short y;
} BattleSpecGeyserBonePos;
#pragma pack(pop)

/* One of the 300 water particles. state is 0 when free and 1 when live; fade counts up after the
 * particle has passed the top of its arc and darkens the sprite; scale is the billboard size. */
typedef struct BattleSpecGeyserParticle {
    short state;
    short fade;
    short scale;
    short pad_06;
    SVECTOR pos;
    SVECTOR vel;
} BattleSpecGeyserParticle;

/* --- battle_fx_special_cold_one --- */
/* Slot state of the Cold1 special timeline: start and end points over 20 frames, plus an actor index. */
typedef struct BattleFxSpecialColdBreathTask {
    short step;
    short timer;
    short startX;
    short baseOffset;
    short startY;
    unsigned char pad_0A[0x2];
    short endX;
    short altBaseOffset;
    short endY;
    unsigned char pad_12[0xa];
    short actorIdx;
} BattleFxSpecialColdBreathTask;

/* The battle task slot of BattleFx_SpecialColdBreathSpinStretchModelTask. */
typedef struct BattleFxColdBreathSpinStretchModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinAngle;
    unsigned char pad_0E[0xa];
    short scale;
} BattleFxColdBreathSpinStretchModelSlot;

/* What BattleFx_SpecialColdBreathDrawParticleField reads through g_BattleSpecialColdBreathCentroidVec. */
typedef struct BattleFxColdBreathParticleFieldCentroidVec {
    int packetWord;
    char colorR;
    char colorG;
    char colorB;
    char polyCode;
} BattleFxColdBreathParticleFieldCentroidVec;

/* One record of g_BattleSpecialColdBreathParticleState (0x8 bytes). */
typedef struct BattleSpecialColdBreathParticleState {
    short active; /*+0x00 */
    short radius; /*+0x02 */
    short angle; /*+0x04 */
    short timer; /*+0x06 */
} BattleSpecialColdBreathParticleState;

/* g_BattleSpecialColdBreathModelReq: a sprite request, then the fields of the spin model's and the rising model's draw requests. */
#pragma pack(push, 1)
typedef struct BattleSpecialColdBreathModelReq {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int spinModel; /* +0x10 */
    unsigned char pad_14[0x6];
    short spinAlpha; /* +0x1a */
    unsigned char pad_1c[0x4];
    int risingModel; /* +0x20 */
    unsigned char pad_24[0x6];
    short risingAlpha; /* +0x2a */
} BattleSpecialColdBreathModelReq;
#pragma pack(pop)

/* --- battle_fx_special_daisenpu_effect --- */
/* A position of the Daisenpu whirlwind: x, y, z and the eight bytes together. */
typedef struct BattleFxWhirlwindPos {
    union {
        Int64Pair pair;
        struct {
            short x;
            short y;
            short z;
            short pad;
        };
    };
} BattleFxWhirlwindPos;

/* One whirlwind point of the Daisenpu converging sprites: position and sprite scale. */
typedef struct BattleFxDaisenpuWhirlPoint {
    short x;
    short y;
    short z;
    short scale;
} BattleFxDaisenpuWhirlPoint;

/* Ring pool of 311 Daisenpu whirlwind points and its write cursor. */
typedef struct BattleFxWhirlwindWhirlPool {
    int writeIndex;
    int pad_04;
    BattleFxDaisenpuWhirlPoint points[311];
} BattleFxWhirlwindWhirlPool;

/* Per-slot whirlwind state for the Daisenpu special: position, spin angle, jitter and particle slot. */
typedef struct BattleFxWhirlwindWhirlState {
    short ownerSlot;
    short step;
    union {
        Int64Pair position;
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short angle;
    short angularVelocity;
    short jitterOffset;
    short jitterRange;
    short baseScale;
    unsigned char pad_16[0x2];
    short stepSize;
} BattleFxWhirlwindWhirlState;

/* Daisenpu record: spread centre and angle set up for the Daisenpu timeline. */
typedef struct BattleFxWhirlwindRecord {
    short ownerSlot;
    unsigned char pad_02[0x2];
    union {
        Int64Pair center;
        struct {
            short centerX;
            short centerY;
            short centerZ;
            unsigned char pad_0A[0x2];
        };
    };
    short angle;
    short angularVelocity;
    short jitterOffset;
    short jitterRange;
    short baseScale;
    unsigned char pad_16[0x2];
    short stepSize;
} BattleFxWhirlwindRecord;

/* Daisenpu orbiting sprite: orbit angle set at spawn from the whirlwind coordinates. */
typedef struct BattleFxWhirlwindOrbit {
    short ownerSlot;
    short frame;
    Int64Pair position; /* copied from the timeline slot */
    short angle;
    short angleStep;
    short scale;
    short radius;
    short fallSpeed;
    short orbitSpeed;
    short poolStartIndex;
} BattleFxWhirlwindOrbit;

/* The battle task slot of BattleFx_SpecialWhirlwindOrbitingSpriteTask. */
typedef struct BattleFxWhirlwindOrbitingSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short fadeLevel;
    short decayRate;
    short scale;
    short posA;
    short accelB;
    short velA;
} BattleFxWhirlwindOrbitingSpriteSlot;

/* Sprite request for the Daisenpu special, with its resources, counter and frame. */
typedef struct BattleFxWhirlwindSpriteReq {
    int orbitHandle;
    int orbitResource;
    unsigned char pad_08[0x4];
    short frameCounter;
    unsigned char pad_0E[0x2];
    int convergeHandle;
    int convergeResource;
    unsigned char pad_18[0x4];
    short frame;
} BattleFxWhirlwindSpriteReq;

/* --- battle_fx_special_dengeki_effect --- */
/* Dengeki timeline task slot: timeline step counter and target index. */
typedef struct BattleFxElectricShockTimelineSlot {
    short ownerSlot; /* +0x0 */
    short timelineStep;
    short bitPattern;
    short targetIndex;
} BattleFxElectricShockTimelineSlot;

/* The battle task slot of BattleFx_SpecialElectricShockRingSpritesTask. */
typedef struct BattleFxElectricShockRingSpritesSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x4];
    short velA;
    short velB;
    short velC;
    unsigned char pad_0E[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_16[0x2];
    short pushLength;
} BattleFxElectricShockRingSpritesSlot;

/* Timeline record for the Dengeki special effect, entered with a target mask. */
typedef struct BattleFxElectricShockTimeline {
    unsigned char pad_00[0x4];
    short targetMask;
    short sourceActor;
} BattleFxElectricShockTimeline;

/* Fading sprite record for the Dengeki special effect. */
typedef struct BattleFxElectricShockFadingSprite {
    unsigned char pad_00[0x8];
    short bonePosX;
    unsigned char pad_0A[0xe];
    short negRadius;
} BattleFxElectricShockFadingSprite;

/* Timeline record for the Dengeki special effect, one per target. */
typedef struct BattleFxElectricShockPerTargetTimeline {
    unsigned char pad_00[0x4];
    short targetIndex;
    short sourceActor;
} BattleFxElectricShockPerTargetTimeline;

/* Sprite record for the Dengeki shrinking fade; frame is an incrementing frame counter. */
typedef struct BattleFxElectricShockFadeRecord {
    short ownerSlot; /* +0x0 */
    short frame;
    unsigned char pad_04[0x4];
    short posX; /* +0x8: the world position the billboard is installed at */
    unsigned char pad_0A[0xe];
    short pushLength;
} BattleFxElectricShockFadeRecord;

#pragma pack(push, 1)
typedef struct BattleSpecElectricShockSpriteReq2 {
    char savedByte1; /* +0x0 */
    char req2Byte1; /* +0x1 */
    char req2Byte2; /* +0x2 */
    unsigned char pad_3[0x5];
    int req2ResourceHandle; /* +0x8 */
    unsigned char pad_c[0x4];
    int fadingResourceHandle; /* +0x10 */
    int fadingSpriteHandle; /* +0x14 */
} BattleSpecElectricShockSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecElectricShockSpriteReq3 {
    char savedByte1; /* +0x0 */
    char req3Byte1; /* +0x1 */
    char req3Byte2; /* +0x2 */
    unsigned char pad_3[0x1];
    short req3Word4; /* +0x4 */
    unsigned char pad_6[0x2];
    int req3Handle8; /* +0x8 */
    int req3Handle12; /* +0xc */
} BattleSpecElectricShockSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecElectricShockSpriteReq4 {
    char savedByte1; /* +0x0 */
    char req4Byte1; /* +0x1 */
    char req4Byte2; /* +0x2 */
    unsigned char pad_3[0x1];
    short req4Word4; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSpecElectricShockSpriteReq4;
#pragma pack(pop)

/* --- battle_fx_special_dia1 --- */
/* Dia1 bouncing model record: step, position, velocity and angle. */
typedef struct BattleFxDiamondCardBounceModel {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    short gravity;
    short angleX;
    short angleSpeed;
    short angleZ;
    short angleSpeedZ;
    short scale;
} BattleFxDiamondCardBounceModel;

/* Dia1 impact timeline record: step counter, three base positions and a random jitter range. */
typedef struct BattleFxDiamondCardImpact {
    short ownerSlot;
    short step;
    short baseA;
    short baseB;
    short baseC;
    unsigned char pad_0A[0x12];
    short jitter;
} BattleFxDiamondCardImpact;

/* Effect-100 slot for the Dia 1 impact timeline sprites. */
typedef struct BattleFxDiamondCardImpactSlot {
    unsigned char pad_00[0xc];
    short velocityX;
    short velocityY;
    short velocityZ;
    short gravity;
    short angleX;
    short speed;
    short angleZ;
    short lateralSpeed;
    short scaleOrIndex;
} BattleFxDiamondCardImpactSlot;

/* Main task slot of the Dia 1 two-sprite draw, counting frames. */
typedef struct BattleDiamondCardTwoSpritesTask {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x18];
    short recordIndex;
} BattleDiamondCardTwoSpritesTask;

/* Model group record returned by the group lookup and used as a texture handle. */
typedef struct BattleFxDiamondCardAxisSprite {
    unsigned char axisVector[0x10];
    int frameTable;
    int spriteData;
} BattleFxDiamondCardAxisSprite;

/* Impact timeline record for the Dia special effect. */
typedef struct BattleFxDiamondCardImpactTimeline {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    unsigned char pad_08[0x14];
    short jitter;
} BattleFxDiamondCardImpactTimeline;

/* Sprite state of the Dia1 special effect. */
typedef struct BattleFxDiamondCardSpriteState {
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char code;
    short frame;
    unsigned char pad_06[0x2];
    int frameTable;
    int spriteData;
} BattleFxDiamondCardSpriteState;

/* Colour, primitive code and frame of the second Dia1 sprite request. */
typedef struct BattleFxDiamondCardSpriteColor {
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char code;
    short frame;
    unsigned char pad_06[0x2];
} BattleFxDiamondCardSpriteColor;

/* One billboard point of the Dia1 impact: position and sprite scale. */
typedef struct BattleFxDiamondCardBillboard {
    short posX;
    short posY;
    short posZ;
    short scale;
} BattleFxDiamondCardBillboard;

/* Target position of the Dia1 effect and the actor's facing. */
typedef struct BattleFxDiamondCardTargetPos {
    short posX;
    short posY;
    short posZ;
    short yaw;
} BattleFxDiamondCardTargetPos;

/* --- battle_fx_special_dia2 --- */
/* Dia 2 target hit model record written per target. */
typedef struct BattleFxSunCardTargetHitRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinAngle;
    short yaw;
    short radius;
    short offsetStep;
    short scale;
    short scaleStep;
    short targetIndex;
} BattleFxSunCardTargetHitRecord;

/* --- battle_fx_special_gas_effect --- */
/* Gas effect timeline task: start and end positions per axis, interpolated over eight steps. */
typedef struct BattleFxPoisonGasTimeline {
    short ownerSlot;
    short phase;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxPoisonGasTimeline;

/* Gas particle record for the Special Gas timeline: position and per-frame velocity. */
typedef struct BattleFxPoisonGasParticleRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short countdown;
} BattleFxPoisonGasParticleRecord;

#pragma pack(push, 1)
typedef struct BattleSpecPoisonGasSpriteReq3 {
    unsigned char pad_0[0x8];
    int res8; /* +0x8 */
    unsigned char pad_c[0xc];
    int res24; /* +0x18 */
    int res28; /* +0x1c */
} BattleSpecPoisonGasSpriteReq3;
#pragma pack(pop)

/* --- battle_fx_special_geki_first --- */
/* Geki1 timeline record: timer, scattered target position, and the record type to spawn. */
typedef struct BattleFxSwampShootTimeline {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x8];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxSwampShootTimeline;

/* Geki1 particle position, velocity and mode. */
typedef struct BattleFxSwampShootParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short mode;
} BattleFxSwampShootParticle;

#pragma pack(push, 1)
typedef struct BattleSpecSwampShootModelParam {
    unsigned char pad_0[0x8];
    int modelResource; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
} BattleSpecSwampShootModelParam;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSwampShootSpriteReq2 {
    int spriteEntry; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int secondSpriteRecord; /* +0x10 */
    int secondDrawObj; /* +0x14 */
} BattleSpecSwampShootSpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_geki_second --- */
/* Effect-slot record of the Geki2 burst: per-spawn velocities, angle and variant. */
typedef struct BattleFxSpecialSephirothShockSlot {
    unsigned char pad_00[0x4];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0xa];
    short angle;
    short variant;
} BattleFxSpecialSephirothShockSlot;

/* Geki2 rising-fade sprite record; its counter steps the fade value down. */
typedef struct BattleFxSephirothShockRisingFadeSprite {
    short ownerSlot;
    short fadeStep;
    unsigned char pad_04[0x2];
    short posY;
    unsigned char pad_08[0xe];
    short taskWord;
} BattleFxSephirothShockRisingFadeSprite;

/* The battle task slot of BattleFx_SpecialSephirothShockRisingSpinningModelTask. */
typedef struct BattleFxSephirothShockRisingSpinningModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short spinAngle;
    short tableIndex;
} BattleFxSephirothShockRisingSpinningModelSlot;

/* --- battle_fx_special_gust --- */
/* Gust swirl record: two centre offsets, an angle with radius and radius velocity. */
typedef struct BattleFxGustSwirl {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0xa];
    short angle;
    short radius;
    short radiusVelocity;
} BattleFxGustSwirl;

/* Builtin gust timeline record: angle-table step index, three positions and a record type. */
typedef struct BattleFxBuiltinGustRecord {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x8];
    short posA;
    short posB;
    short posC;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxBuiltinGustRecord;

/* One of the 200 pooled gust segments: an in-use flag, the colour row it cycles through, and the
 * segment's two end points. */
typedef struct {
    short active;
    short colorPhase;
    short x1;
    short y1;
    short z1;
    unsigned char pad_0A[0x2];
    short x2;
    short y2;
    short z2;
    unsigned char pad_12[0x2];
} BattleFxGustSegment;

/* Pool slot for one swirl particle of the built-in gust effect; position and angle. */
typedef struct BattleFxGustSwirlSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angle;
    short radius;
    short radiusVelocity;
} BattleFxGustSwirlSlot;

/* One RGB colour entry of the gust effect's colour table. */
typedef struct BattleFxGustColor {
    short colorR;
    short colorG;
    short colorB;
} BattleFxGustColor;

/* --- battle_fx_special_hadoho --- */
/* Per-slot state of the Hadoho variant spin model task. */
typedef struct BattleFxWaveCannonSpinRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angleZ;
    short variant;
} BattleFxWaveCannonSpinRecord;

/* --- battle_fx_special_hakai --- */
/* Hakai timeline task slot: timeline step counter and the actor and target the spawner stored. */
typedef struct BattleFxSabotageTimelineSlot {
    short ownerSlot;
    short timelineStep;
    unsigned char pad_04[0x18];
    short actor;
    short targetIndex;
} BattleFxSabotageTimelineSlot;

/* Hakai variant sprite task record: position, frame counter, variant index and billboard scale. */
typedef struct BattleFxSabotageVariantSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variant;
    short scale;
} BattleFxSabotageVariantSprite;

/* Sprite request record of the variant sprite task: texture bytes and the current frame. */
typedef struct BattleSpecSabotageSpriteReq {
    unsigned char texture[4];
    short frame;
    short pad_06;
} BattleSpecSabotageSpriteReq;

/* --- battle_fx_special_hakugek1_effect --- */
/* Effect slot record for the Hakugeki expanding-shells special, driven by a frame counter. */
typedef struct BattleFxDualMortarShellRecord {
    short ownerSlot;
    short timer;
    short startSize;
    unsigned char pad_06[0x2];
    short endSize;
} BattleFxDualMortarShellRecord;

/* Slots BattleFx_SpecialDualMortarTimelineTask allocates for BattleFx_SpecialHakugek1{AnimatedSprite, 3 more}Task. */
typedef struct BattleFxDualMortarTimelineSpawnSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotOrVelX;
    short yaw;
    short spinSpeed;
    unsigned char pad_12[0x2];
    short spinAngle;
    short variant;
    short scale;
} BattleFxDualMortarTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialDualMortarTimelineTask. */
typedef struct BattleFxDualMortarTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    unsigned char pad_06[0x2];
    short startZ;
    unsigned char pad_0A[0x2];
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_12[0xa];
    short actorIndex;
    short boneOwnerIndex;
} BattleFxDualMortarTimelineSlot;

/* The battle task slot of BattleFx_SpecialDualMortarExpandingModelTask. */
typedef struct BattleFxDualMortarExpandingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    short spinAngle;
    unsigned char pad_16[0x2];
    short tiltAngle;
} BattleFxDualMortarExpandingModelSlot;

#pragma pack(push, 1)
typedef struct BattleSpecDualMortarSpriteReq2 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
    int res8; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
} BattleSpecDualMortarSpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_hakugek2_effect --- */
/* Hakugeki2 timeline record: frame counter, end positions, spawned record type and actor index. */
typedef struct BattleFxMortarTimeline {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short recordType;
    short actorIndex;
} BattleFxMortarTimeline;

/* Hakugek2 shell record: position, spin rate, angle and scale for a shell sprite. */
typedef struct BattleFxMortarShellRecord {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotOrVelX;
    short spinRate;
    short rotOrVelZ;
    unsigned char pad_12[0x2];
    short angle;
    short variant;
    short scale;
} BattleFxMortarShellRecord;

/* The battle task slot of BattleFx_SpecialMortarExpandingShellsTask. */
typedef struct BattleFxMortarExpandingShellsSlot {
    short ownerSlot;
    short frameCounter;
    short startSize;
    unsigned char pad_06[0x2];
    short endSize;
} BattleFxMortarExpandingShellsSlot;

/* The battle task slot of BattleFx_SpecialMortarSpinModelTask. */
typedef struct BattleFxMortarSpinModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    short tiltAngle;
    unsigned char pad_16[0x2];
    short rollAngle;
} BattleFxMortarSpinModelSlot;

#pragma pack(push, 1)
typedef struct BattleSpecMortarSpriteReq2 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
    int res8; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
    unsigned char pad_14[0x4];
    int res24; /* +0x18 */
    int flags; /* +0x1c */
    unsigned char pad_20[0x2];
    short secondAlpha; /* +0x22 */
} BattleSpecMortarSpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_hari_needle --- */
/* Hari scattered sprite record; its counter is checked against the sprite frame count. */
typedef struct BattleFxThousandNeedlesScatteredSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
    short pushLength;
} BattleFxThousandNeedlesScatteredSprite;

/* The battle task slot of BattleFx_SpecialThousandNeedlesScatterModelTask. */
typedef struct BattleFxThousandNeedlesScatterModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short tiltAngle;
    short spinAngle;
} BattleFxThousandNeedlesScatterModelSlot;

/* Slots BattleFx_SpecialThousandNeedlesTimelineTask allocates for BattleFx_SpecialHari{ScatterModel, ScatteredSprite}Task. */
typedef struct BattleFxThousandNeedlesTimelineSpawnSlot {
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
    short pushLength;
    short angle;
    short spinAngle;
} BattleFxThousandNeedlesTimelineSpawnSlot;

/* The sprite request BattleFx_SubmitSpriteDraw reads: the sprite resource and field, a colour/mode block, the frame. */
#pragma pack(push, 1)
typedef struct {
    int resource; /* +0x0 */
    int scatterSpriteField; /* +0x4 */
    unsigned char colorAndMode[4]; /* +0x8 */
    short scatterFrameIndex; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialThousandNeedlesSpriteData;
#pragma pack(pop)

/* --- battle_fx_special_harrier_rocket --- */
/* Harrier variant sprite: frame index, position, velocity and variant. */
typedef struct BattleFxHarrierSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x8];
    short variant;
} BattleFxHarrierSprite;

/* --- battle_fx_special_heatbu_effect --- */
/* A world position in a Heatbu sprite: the eight bytes the spawner copies from the actor's bone position. */
typedef union BattleFxHeatbuPos {
    Int64Pair pair;
    struct {
        short x;
        short y;
        short z;
    } xyz;
} BattleFxHeatbuPos;

/* Heatbu animated sprite record: timer, position and velocity vectors, and an angle with step. */
typedef struct BattleFxHeatBlastSpriteRecord {
    short ownerSlot;
    short timer;
    BattleFxHeatbuPos pos;
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x2];
    short angle;
    short angleStep;
} BattleFxHeatBlastSpriteRecord;

/* --- battle_fx_special_hikari_flash --- */
/* Hikari special-effect timeline record: phase, position, arc target and actor index. */
typedef struct BattleFxStareDownTrack {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    unsigned char pad_0E[0x2];
    short targetZ;
    unsigned char pad_12[0x8];
    short actorIndex;
} BattleFxStareDownTrack;

#pragma pack(push, 1)
typedef struct BattleSpecialStareDownSpriteData {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short hikariSpriteArgWord; /* +0xc */
    unsigned char pad_e[0x2];
    int hikariSpriteDataTable16; /* +0x10 */
    int secondDrawObj; /* +0x14 */
    unsigned char pad_18[0x4];
    short hikariSpriteArgWord28; /* +0x1c */
    unsigned char pad_1e[0x2];
} BattleSpecialStareDownSpriteData;
#pragma pack(pop)

/* --- battle_fx_special_hubuki --- */
/* Slots BattleFx_SpecialSnowstormTimelineTask allocates for BattleFx_SpecialHubuki{AnimatedSprite, 4 more}Task. */
typedef struct BattleFxSnowstormTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxSnowstormTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialSnowstormFadeSpriteTask. */
typedef struct BattleFxSnowstormFadeSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxSnowstormFadeSpriteSlot;

/* The battle task slot of BattleFx_SpecialSnowstormShrinkingOrientedModelTask. */
typedef struct BattleFxSnowstormModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
} BattleFxSnowstormModelSlot;

/* Timeline slot holding bone world positions for the Hubuki beam. */
typedef struct BattleFxSnowstormTaskSlot {
    unsigned char pad_00[0x4];
    short sourcePos;
    unsigned char pad_06[0x6];
    short targetPos;
    unsigned char pad_0E[0xe];
    short targetIndex;
} BattleFxSnowstormTaskSlot;

/* One of the 400 particle records at g_BattleSpecialSnowstormParticleData (8 bytes each): two
   screen coordinates that wrap at the particle field's width and height. */
typedef struct BattleSpecialSnowstormParticle {
    unsigned char pad_00[0x2];
    short x;
    short y;
    unsigned char pad_06[0x2];
} BattleSpecialSnowstormParticle;

#pragma pack(push, 1)
typedef struct BattleSpecialSnowstormAnimParams {
    unsigned char pad_0[0x8];
    int modelRecord; /* +0x8 */
    unsigned char pad_c[0x6];
    short scaleLevel; /* +0x12 */
} BattleSpecialSnowstormAnimParams;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSnowstormColor1 {
    unsigned char pad_0[0x4];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int spriteRecord; /* +0x8 */
    int spriteObject; /* +0xc */
} BattleSpecialSnowstormColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSnowstormModelReq {
    int scaleValue; /* +0x0 */
    unsigned char pad_4[0x4];
    int tableField; /* +0x8 */
    unsigned char pad_c[0x6];
    short stepScale; /* +0x12 */
} BattleSpecialSnowstormModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSnowstormParticleConfig {
    unsigned char pad_0[0x4];
    char red; /* +0x4 */
    char green; /* +0x5 */
    char blue; /* +0x6 */
    unsigned char pad_7[0x1];
    short phase; /* +0x8 */
    short age; /* +0xa */
} BattleSpecialSnowstormParticleConfig;
#pragma pack(pop)

/* --- battle_fx_special_ito_string --- */
/* Draw-list line record: tag word and a three-sample byte trail. */
typedef struct BattleFxSpiderWebLinePrim {
    int tag;
    char trailOld;
    char trailMid;
    char trailNew;
    char primCode;
} BattleFxSpiderWebLinePrim;

/* Per-slot state of the Ito variant sprite task: frame counter and request index. */
typedef struct BattleFxSpiderWebVariantSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x12];
    short requestIndex;
} BattleFxSpiderWebVariantSprite;

/* The battle task slot of BattleFx_SpecialSpiderWebTimelineTask. */
typedef struct BattleFxSpiderWebTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short targetX;
    unsigned char pad_0E[0x2];
    short targetZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxSpiderWebTimelineSlot;

/* Slots BattleFx_SpecialSpiderWebTimelineTask allocates for BattleFx_SpecialIto{LoopingSprite, OrbitTrail, VariantSprite}Task. */
typedef struct BattleFxSpiderWebTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0x2];
    short angle;
    short spin;
    short radius;
    short radiusScale;
} BattleFxSpiderWebTimelineSpawnSlot;

/* Task slot for Ito with a frame counter, limit and target index. */
typedef struct BattleFxSpiderWebTaskSlot {
    unsigned char pad_00[0x8];
    short frameCount;
    unsigned char pad_0A[0x6];
    short frameLimit;
    unsigned char pad_12[0xa];
    short targetIndex;
} BattleFxSpiderWebTaskSlot;

/* One record of g_BattleSpecialSpiderWebLineRecords (0x14 bytes). */
typedef struct BattleSpecialSpiderWebLineRecord {
    short active; /*+0x00 */
    short timer; /*+0x02 */
    unsigned char startVertex[0x10]; /*+0x04: the line's start vertex, transformed when it is drawn */
} BattleSpecialSpiderWebLineRecord;

#pragma pack(push, 1)
typedef struct BattleSpecialSpiderWebSpriteData {
    short spriteIndex; /* +0x0 */
    unsigned char pad_2[0x6];
    int itoSpriteDataTable8; /* +0x8 */
    int drawObj; /* +0xc */
    unsigned char pad_10[0x4];
    short itoSpriteDataFrameIndex20; /* +0x14 */
    unsigned char pad_16[0x2];
    int itoVariantSpriteTable24; /* +0x18 */
    int variantDrawObj; /* +0x1c */
    unsigned char pad_20[0x4];
    short itoVariantFrameWord36; /* +0x24 */
    unsigned char pad_26[0x2];
    int itoVariantRequestTable40; /* +0x28 */
} BattleSpecialSpiderWebSpriteData;
#pragma pack(pop)

/* --- battle_fx_special_jammer2_effect --- */
/* The battle task slot of BattleFx_SpecialMultiJammerTimelineTask. */
typedef struct BattleFxMultiJammerTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
} BattleFxMultiJammerTimelineSlot;

/* --- battle_fx_special_jammer_effect --- */
/* The battle task slot of BattleFx_SpecialJammerTimelineTask. */
typedef struct BattleFxJammerTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxJammerTimelineSlot;

#pragma pack(push, 1)
typedef struct BattleSpecJammerSpriteReq2 {
    int modelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
} BattleSpecJammerSpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_jibaku --- */
/* The battle task slot of BattleFx_SpecialBombBlastTimelineTask. */
typedef struct BattleFxBombBlastTimelineSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x8];
    short posA;
    short posB;
    short posC;
    unsigned char pad_12[0xa];
    short actorMask;
    short targetIndex;
} BattleFxBombBlastTimelineSlot;

/* Slots BattleFx_SpecialBombBlastTimelineTask allocates for BattleFx_SpecialJibaku2{AnimatedSprite, 4 more}Task. */
typedef struct BattleFxBombBlastTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0x2];
    short angle;
    short variant;
} BattleFxBombBlastTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialBombBlastBouncingSpriteTask. */
typedef struct BattleFxBombBlastBouncingSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0x2];
    short angle;
    short variant;
} BattleFxBombBlastBouncingSpriteSlot;

/* The battle task slot of BattleFx_SpecialBombBlastRadialSparkSpriteTask. */
typedef struct BattleFxBombBlastRadialSparkSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0x2];
    short angle;
    short angleY;
} BattleFxBombBlastRadialSparkSpriteSlot;

#pragma pack(push, 1)
typedef struct BattleSpecBombBlastSpriteReq2 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    char code; /* +0x3 */
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
    int drawBlock; /* +0x8 */
    int handle; /* +0xc */
} BattleSpecBombBlastSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecBombBlastSpriteReq4 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x5];
    int modelDraw; /* +0x8 */
    unsigned char pad_c[0x6];
    short alphaWord; /* +0x12 */
} BattleSpecBombBlastSpriteReq4;
#pragma pack(pop)

/* --- battle_fx_special_jibasiri_effect --- */
/* Jibasiri particle slot: position, phase and the record type passed to Battle_PostActorHitOrHurtMessage. */
typedef struct BattleFxGrandSparkParticle {
    short ownerSlot;
    short phase;
    short spawnX;
    short spawnY;
    short spawnZ;
    unsigned char pad_0A[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_12[0x4];
    short variant;
    unsigned char pad_18[0x4];
    short actorIndex;
} BattleFxGrandSparkParticle;

/* Jibasiri model row holding the resource table pointer for its animated sprite task. */
typedef struct BattleSpecGrandSparkModelRows {
    unsigned char pad_00[0x70];
    int spriteEntry;
    int sceneObject;
} BattleSpecGrandSparkModelRows;

#pragma pack(push, 1)
typedef struct BattleSpecGrandSparkModelScale {
    unsigned char pad_0[0x8];
    int drawRecord; /* +0x8 */
    unsigned char pad_c[0x6];
    short offset; /* +0x12 */
} BattleSpecGrandSparkModelScale;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecGrandSparkSpriteReq2 {
    int modelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
    int res16; /* +0x10 */
    int res20; /* +0x14 */
    unsigned char pad_18[0x4];
    short frame; /* +0x1c */
    unsigned char pad_1e[0x2];
    int res32; /* +0x20 */
    int res36; /* +0x24 */
} BattleSpecGrandSparkSpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_joker1 --- */
/* Task slot record for the Joker special impact effect. */
typedef struct BattleFxJokerCardImpactTask2 {
    short ownerSlot;
    unsigned char pad_02[0xa];
    short yaw;
} BattleFxJokerCardImpactTask2;

/* --- battle_fx_special_joker2 --- */
/* Effect-100 array entry (0x20 bytes) for the Joker2 spawned model tasks; same layout as Magic Stop. */
typedef struct BattleFxWorldCardEntry {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short roll;
    short yaw;
    short position;
    short offset;
    short spawnTotal;
    short batchCount;
    short clampLimit;
    short spawnCount;
} BattleFxWorldCardEntry;

/* Joker2 spawn record: base coordinate and target index, set when the per-target effect is allocated. */
typedef struct BattleFxWorldCardSpawnRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short yaw;
    short base;
    unsigned char pad_12[0x2];
    short ramp;
    short batchCount;
    unsigned char pad_18[0x2];
    short spawnCount;
    short targetIndex;
} BattleFxWorldCardSpawnRecord;

/* --- battle_fx_special_kaen2_effect --- */
/* The battle task slot of BattleFx_SpecialFireShellTimelineTask. */
typedef struct BattleFxFireShellTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0xa];
    short actorIndex;
    short sourceActor;
} BattleFxFireShellTimelineSlot;

#pragma pack(push, 1)
typedef struct BattleSpecFireShellModelParams {
    unsigned char pad_0[0x10];
    int drawBlock; /* +0x10 */
    unsigned char pad_14[0xc];
    int spriteEntry; /* +0x20 */
    int handle; /* +0x24 */
} BattleSpecFireShellModelParams;
#pragma pack(pop)

/* --- battle_fx_special_kaen_effect --- */
/* Battle FX task slot for the Kaen special timeline: start and end x/y/z pairs, a phase and a type id. */
typedef struct BattleFxFireballTimelineTask {
    short ownerSlot;
    short phase;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxFireballTimelineTask;

#pragma pack(push, 1)
typedef struct BattleSpecFireballModelParams {
    unsigned char pad_0[0x8];
    int modelDraw; /* +0x8 */
    unsigned char pad_c[0xc];
    int spriteDraw; /* +0x18 */
    int handle; /* +0x1c */
} BattleSpecFireballModelParams;
#pragma pack(pop)

/* --- battle_fx_special_kahun --- */
/* Special Kahun timeline task: start and end positions on X and Z, plus a frame timer. */
typedef struct BattleFxSpecialPollenTask {
    short ownerSlot;
    short timer;
    short startX;
    short posY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short startY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxSpecialPollenTask;

/* Kahun effect slot: start position and per-frame steps toward the source record's end point. */
typedef struct BattleFxPollenTaskRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short stepX;
    short angle;
    short stepZ;
    unsigned char pad_12[0x2];
    short stage;
    short fadeTimer;
    short scaleValue;
    short growth;
} BattleFxPollenTaskRecord;

/* The battle task slot of BattleFx_SpecialPollenFadeInOutSpriteTask. */
typedef struct BattleFxPollenFadeInOutSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short fadeStep;
    short fadeTimer;
    short uniformScale;
} BattleFxPollenFadeInOutSpriteSlot;

/* The battle task slot of BattleFx_SpecialPollenFadingSpinModelTask. */
typedef struct BattleFxPollenFadingSpinModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    unsigned char pad_0E[0xa];
    short modelStep;
    short angle;
} BattleFxPollenFadingSpinModelSlot;

/* What BattleFx_SpecialPollenDrawPsxParticleDots reads through g_BattleSpecPollenDrawListHead. */
typedef struct BattleFxPollenDotBatchHead {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char command;
} BattleFxPollenDotBatchHead;

/* One record of g_BattleSpecPollenParticleRecords (0x8 bytes). */
typedef struct BattleSpecPollenParticleRecord {
    short active; /*+0x00 */
    short radius; /*+0x02 */
    short angle; /*+0x04 */
    short age; /*+0x06 */
} BattleSpecPollenParticleRecord;

/* --- battle_fx_special_kama_sickle --- */
/* Data block for the Kama variant sprite task, holding the current frame table pointer. */
typedef struct BattleSpecFlyingSickleDataBlob {
    unsigned char pad_00[0x88];
    int frameTable;
    int variantField;
} BattleSpecFlyingSickleDataBlob;

/* --- battle_fx_special_kemu --- */
/* Kemu1 projectile with position, velocity and the actor it hits. */
typedef struct BattleFxSmokeShotProjectile {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short actorIndex;
} BattleFxSmokeShotProjectile;

/* Effect task record for the Kemu1 animated sprite effect. */
typedef struct BattleFxSmokeShotSpriteTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xA];
    short scale;
} BattleFxSmokeShotSpriteTask;

/* --- battle_fx_special_kiengiri_slash --- */
/* The battle task slot of BattleFx_SpecialSilverWheelTimelineTask. */
typedef struct BattleFxSilverWheelTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short fromX;
    short fromY;
    short fromZ;
    unsigned char pad_0A[0x2];
    short toX;
    short toY;
    short toZ;
    unsigned char pad_12[0x2];
    short actorIndex;
    short casterIndex;
} BattleFxSilverWheelTimelineSlot;

/* The battle task slot of BattleFx_SpecialSilverWheelTravelModelTask. */
typedef struct BattleFxSilverWheelTravelModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short battleActorIndex;
    unsigned char pad_18[0x2];
    short angleY;
} BattleFxSilverWheelTravelModelSlot;

/* Slots BattleFx_SpecialSilverWheelTimelineTask allocates for BattleFx_SpecialKiengiri{AnimatedSpriteEntry7, 2 more}Task. */
typedef struct BattleFxSilverWheelTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short heading;
    short velocityZ;
    unsigned char pad_12[0x4];
    short casterIndex;
    short variantIndex;
    short angle;
} BattleFxSilverWheelTimelineSpawnSlot;

/* Timeline record for the Kiengiri special effect, one per target. */
typedef struct BattleFxSilverWheelTimeline {
    unsigned char pad_00[0x14];
    short targetIndex;
    short actorIndex;
} BattleFxSilverWheelTimeline;

/* Sprite data for the Kiengiri variant task, holding the current frame table pointer. */
typedef struct BattleSpecSilverWheelSpriteData {
    unsigned char pad_00[0x10];
    int frameTable;
    int variantField;
} BattleSpecSilverWheelSpriteData;

#pragma pack(push, 1)
typedef struct BattleSpecialSilverWheelModelReq {
    int modelResField; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short stepOffset; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialSilverWheelModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSilverWheelSpriteData {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short kiengiriSpriteArgWord; /* +0xc */
    unsigned char pad_e[0xa];
} BattleSpecialSilverWheelSpriteData;
#pragma pack(pop)

/* --- battle_fx_special_kinzan --- */
/* Task-slot record for the Kinzan special timeline: sentinel, frame counter and record type. */
typedef struct BattleFxCoinTossTimelineTask {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    short bonePos[3];
    unsigned char pad_12[0x6];
    short actorIndex;
} BattleFxCoinTossTimelineTask;

/* Kinzan bouncing sprite task slot: spawn position, launch velocity, spin angle and variant. */
typedef struct BattleFxCoinTossBounceSlot {
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
} BattleFxCoinTossBounceSlot;

/* --- battle_fx_special_kona_effect --- */
/* Kona timeline record: frame counter, start and end positions, spawned record type. */
typedef struct BattleFxWingScalesTimeline {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxWingScalesTimeline;

/* Kona effect record: current position interpolated from start to end, plus a heading angle. */
typedef struct BattleFxWingScalesTravel {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angle;
    unsigned char pad_16[0x2];
    short speed;
} BattleFxWingScalesTravel;

/* One 32-byte record of the Kona particle-pair pool (g_BattleSpecWingScalesParticlePairSlots): status 1 = live,
 * 0xFFFF = finished; velocity is the offset applied to the position each frame. */
typedef struct BattleFxWingScalesParticlePair {
    short status;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    short radius;
    short scale;
    short spriteFrame;
    unsigned char pad_1C[0x4];
} BattleFxWingScalesParticlePair;

/* The battle task slot of BattleFx_SpecialWingScalesSpawnParticlePairsTask. */
typedef struct BattleFxWingScalesSpawnParticlePairsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angleY;
    unsigned char pad_16[0x2];
    short speed;
} BattleFxWingScalesSpawnParticlePairsSlot;

/* The battle task slot of BattleFx_SpecialWingScalesImpactModelTask. */
typedef struct BattleFxWingScalesImpactModelSlot {
    short ownerSlot;
    short frameCounter;
    short baseX;
    short baseY;
    short baseZ;
} BattleFxWingScalesImpactModelSlot;

/* Per-instance task state for the Kona special sprite effect. */
typedef struct BattleSpecWingScalesSpriteTask {
    short ownerSlot; /* BATTLE_TASK_FREE when finished */
    short frameIndex;
    short posX;
    short posY; /* raised by 20 a frame for variant 0 */
    short posZ;
    unsigned char pad_0A[0x10];
    short variant;
} BattleSpecWingScalesSpriteTask;

#pragma pack(push, 1)
typedef struct BattleSpecWingScalesSpriteReq1 {
    int spriteResource; /* +0x0 */
    int res4; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int res16; /* +0x10 */
    unsigned char pad_14[0x6];
    short alpha; /* +0x1a */
} BattleSpecWingScalesSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecWingScalesSpriteReq2 {
    unsigned char pad_0[0x8];
    int res8; /* +0x8 */
    int res12; /* +0xc */
    unsigned char pad_10[0x4];
    short frame; /* +0x14 */
    unsigned char pad_16[0x2];
    int res24; /* +0x18 */
    int object28; /* +0x1c */
    unsigned char pad_20[0x4];
    short secondFrame; /* +0x24 */
    unsigned char pad_26[0x2];
} BattleSpecWingScalesSpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_konoha_leaves --- */
/* The battle task slot of BattleFx_SpecialAutumnLeavesLeafModelTask. */
typedef struct BattleFxAutumnLeavesLeafModelSlot {
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
    short modelIndex;
    short radius;
    short angle;
    short spinAngle;
} BattleFxAutumnLeavesLeafModelSlot;

/* The battle task slot of BattleFx_SpecialAutumnLeavesTimelineTask. */
typedef struct BattleFxAutumnLeavesTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX; /* the acting bone's position: x, (y), z */
    unsigned char pad_06[0x2];
    short startZ;
    unsigned char pad_0A[0x2];
    short endX; /* the targets' centroid: x, (y), z */
    unsigned char pad_0E[0x2];
    short endZ;
} BattleFxAutumnLeavesTimelineSlot;

#pragma pack(push, 1)
typedef struct BattleSpecialAutumnLeavesModelConfig {
    unsigned char pad_0[0x10];
    int modelHandle; /* +0x10 */
    unsigned char pad_14[0x6];
    short konohaModelConfigStepShift26; /* +0x1a */
} BattleSpecialAutumnLeavesModelConfig;
#pragma pack(pop)

/* --- battle_fx_special_koudan1_effect --- */
/* Koudan1 timeline record: frame counter, start and end positions, actor index. */
typedef struct BattleFxLightShellTimeline {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxLightShellTimeline;

/* Task slot record for the Koudan1 variant sprite effect; a frame counter and billboard matrix. */
typedef struct BattleFxLightShellSpriteTask {
    short ownerSlot;
    short frameIndex;
    short billboardMatrix;
    unsigned char pad_06[0x14];
    short resourceIndex;
} BattleFxLightShellSpriteTask;

/* --- battle_fx_special_koudan2_effect --- */
/* Growing sprite record for the Koudan 2 special effect. */
typedef struct BattleFxRiderBreathSprite {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short angle;
    short variant;
} BattleFxRiderBreathSprite;

/* Slots BattleFx_SpecialRiderBreathTimelineTask allocates for BattleFx_SpecialKoudan2{ExpandingModel, 3 more}Task. */
typedef struct BattleFxRiderBreathTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
    short variant;
} BattleFxRiderBreathTimelineSpawnSlot;

/* Koudan2 variant sprite task state: frame index, position and velocity pairs. */
typedef struct BattleFxRiderBreathVariantSprite {
    short ownerSlot;
    short frameIndex;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0x8];
    short variant; /* which of the three sprite animations to draw */
} BattleFxRiderBreathVariantSprite;

/* --- battle_fx_special_kueisa --- */
/* Task record for a drifting Kueisa sprite: position, velocity, fade step and billboard arg. */
typedef struct BattleFxQuasarDriftTask {
    short ownerSlot;
    short fadeStep;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxQuasarDriftTask;

/* Kueisa effect record: a step counter and a position copied into the target record. */
typedef struct BattleFxQuasarTimeline {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
} BattleFxQuasarTimeline;

/* Task-slot record for the Kueisa expanding spin model; a progress value drives its scale. */
typedef struct BattleFxQuasarSpinTask {
    short ownerSlot;
    short progress;
    short posX;
    short posY;
    short posZ;
} BattleFxQuasarSpinTask;

/* The battle task slot of BattleFx_SpecialQuasarBeamSpinModelTask. */
typedef struct BattleFxQuasarBeamSpinModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short modelIndex;
    short scaleZ;
} BattleFxQuasarBeamSpinModelSlot;

/* Slots BattleFx_SpecialQuasarPulsingSpriteSpawnBeamTask allocates for BattleFx_SpecialKueisa{3 more}Task. */
typedef struct BattleFxQuasarBeamTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x2];
    short timer;
    short modelIndex;
    short scale;
} BattleFxQuasarBeamTask;

/* The battle task slot of BattleFx_SpecialQuasarBeamAlignedSpriteTask. */
typedef struct BattleFxQuasarBeamAlignedSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    SVECTOR rotation;
    short timer;
    short modelIndex;
    short scaleY;
} BattleFxQuasarBeamAlignedSpriteSlot;

/* Effect task record for the Kueisa growing fade sprite effect; scale grows each frame. */
typedef struct BattleFxQuasarFadeSpriteTask {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xE];
    short scale;
} BattleFxQuasarFadeSpriteTask;

/* The beam-aligned sprites' frame sequences, one per modelIndex, then the first two words of the
 * drifting fade sprite's draw request. */
typedef struct {
    short *modelIndices[2];
    int spriteRecord;
    int spriteObject;
} BattleSpecialQuasarModelIndexTable;

#pragma pack(push, 1)
typedef struct BattleSpecialQuasarAnimParams {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x5];
    int expandModelGroup; /* +0x8 */
    unsigned char pad_c[0x6];
    short expandModelFade; /* +0x12 */
    unsigned char pad_14[0x4];
    int spinModelGroup; /* +0x18 */
    unsigned char pad_1c[0x6];
    short spinModelFade; /* +0x22 */
} BattleSpecialQuasarAnimParams;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialQuasarColor1 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short spriteFrame; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSpecialQuasarColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialQuasarColor2 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x5];
    int spriteRecord; /* +0x8 */
    int spriteObject; /* +0xc */
} BattleSpecialQuasarColor2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialQuasarMatrix {
    unsigned char pad_0[0x18];
    int spriteRecord; /* +0x18 */
    int spriteObject; /* +0x1c */
} BattleSpecialQuasarMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialQuasarModelReq {
    int modelGroup; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short fadeLevel; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialQuasarModelReq;
#pragma pack(pop)

/* --- battle_fx_special_kyuu --- */
/* Per-slot state of the Kyuu rising looping sprite task: frame counter, angle and sprite request index. */
typedef struct BattleFxDrainRisingSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x2];
    short angle;
    unsigned char pad_08[0xc];
    short requestIndex;
} BattleFxDrainRisingSprite;

/* The battle task slot of BattleFx_SpecialDrainOrbitingSpriteTask. */
typedef struct BattleFxDrainOrbitingSpriteSlot {
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
    short scale;
    short radius;
    short pulseAngle;
    short scaleDivisor;
} BattleFxDrainOrbitingSpriteSlot;

/* Timeline record for the Kyuu special effect, one per target. */
typedef struct BattleFxDrainTimeline {
    unsigned char pad_00[0x1c];
    short targetId;
    short attackerId;
} BattleFxDrainTimeline;

/* g_BattleSpecialDrainSpriteReq1: the sprite requests of the convergence spark (entry 4) and of the rising looping sprite. */
typedef struct BattleSpecialDrainSpriteReq1 {
    BattleFxSpriteRequest entry4;
    BattleFxSpriteRequest rising;
} BattleSpecialDrainSpriteReq1;

/* g_BattleSpecialDrainSpriteReq2: the two table indices the rising sprite picks from, then the sprite requests of the
   spiral (entry 2), burst (entry 1) and orbiting (entry 3) sprites. */
typedef struct BattleSpecialDrainSpriteReq2 {
    int risingEntry[2];
    BattleFxSpriteRequest spiral;
    BattleFxSpriteRequest burst;
    BattleFxSpriteRequest orbit;
} BattleSpecialDrainSpriteReq2;

/* --- battle_fx_special_kyuu_collapse --- */
/* Kyuu2 looping sprite task record: frame counter, a position that rises each frame and the resource variant. */
typedef struct BattleFxEnergySiphonLoopingSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short variant;
} BattleFxEnergySiphonLoopingSprite;

/* The battle task slot of BattleFx_SpecialEnergySiphonOrbitSpriteTask. */
typedef struct BattleFxEnergySiphonOrbitSpriteSlot {
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
    short phase;
    short radius;
    short pulseAngle;
    short scaleDivisor;
} BattleFxEnergySiphonOrbitSpriteSlot;

/* Slots BattleFx_SpecialEnergySiphonOrbitSpriteTask allocates for BattleFx_SpecialKyuu2{AnimatedSpriteEntry1, 1 more}Task. */
typedef struct BattleFxEnergySiphonOrbitSpriteSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short variant;
} BattleFxEnergySiphonOrbitSpriteSpawnSlot;

/* Timeline record for the Kyuu2 special effect, one per target: the sphere's path and the two actors. */
typedef struct BattleFxEnergySiphonTimeline {
    short ownerSlot;
    short timer;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short targetIndex;
    short casterIndex;
} BattleFxEnergySiphonTimeline;

/* A sprite draw request for BattleFx_SubmitSpriteDraw: the sprite group, its object, a colour and the frame. */
typedef struct BattleSpecialEnergySiphonSpriteRequest {
    int spriteResource;
    int spriteObject;
    unsigned char color[4];
    short frameIndex;
    unsigned char pad_0E[0x2];
} BattleSpecialEnergySiphonSpriteRequest;

/* The sprite variant to entry-index table, then the requests of the spiral, entry-1 and orbit sprites. */
typedef struct BattleSpecialEnergySiphonSpriteReq2 {
    int variantEntries[2];
    BattleSpecialEnergySiphonSpriteRequest request[3];
} BattleSpecialEnergySiphonSpriteReq2;

/* --- battle_fx_special_magma --- */
/* Magma special timeline task: its timer runs to 80 frames, allocating a typed record at 0x3C. */
typedef struct BattleFxMagmaTimeline {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x8];
    short targetBonePos[3]; /* world position of the target's bone (Battle_GetBoneWorldPos) */
    unsigned char pad_12[0x6];
    short actorIndex;
} BattleFxMagmaTimeline;

/* One of the 300 lava particles. state is 0 when free and 1 when live; fade counts up once the
 * particle has passed the top of its arc and darkens the sprite; scale is the billboard size. */
typedef struct BattleFxMagmaParticle {
    short state;
    short fade;
    short scale;
    short pad_06;
    SVECTOR pos;
    SVECTOR vel;
} BattleFxMagmaParticle;

/* --- battle_fx_special_mgbeam_energy --- */
/* Mgbeam one-shot sprite record; its counter is checked against the sprite frame count. */
typedef struct BattleFxBeamGunOneShotSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short pushLength;
} BattleFxBeamGunOneShotSprite;

/* Slots BattleFx_SpecialBeamGunTimelineTask allocates for BattleFx_SpecialMgbeam{AnimatedSprite, DriftModel, 1 more}Task. */
typedef struct BattleFxBeamGunTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short unusedWord;
    short negRadius;
} BattleFxBeamGunTimelineSpawnSlot;

#pragma pack(push, 1)
typedef struct BattleSpecialBeamGunScaleData1 {
    unsigned char pad_0[0x10];
    int spriteRecord; /* +0x10 */
    int spriteObject; /* +0x14 */
    unsigned char pad_18[0x4];
    short frameIndex; /* +0x1c */
    unsigned char pad_1e[0x2];
} BattleSpecialBeamGunScaleData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialBeamGunScaleData2 {
    unsigned char pad_0[0x10];
    int scale2ModelField; /* +0x10 */
} BattleSpecialBeamGunScaleData2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialBeamGunSpriteData1 {
    int spriteResource; /* +0x0 */
    int tableField; /* +0x4 */
    unsigned char pad_8[0x4];
    short frameIndex; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialBeamGunSpriteData1;
#pragma pack(pop)

/* --- battle_fx_special_misairu_missile --- */
/* Timeline record for the Misairu special effect: start and end points of the flight and the target actor. */
typedef struct BattleFxRocketLauncherTimeline {
    short ownerSlot;
    short phase;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxRocketLauncherTimeline;

/* Misairu missile state, interpolated from its start and end points. */
typedef struct BattleFxRocketLauncher {
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
    short arcVelocityY;
} BattleFxRocketLauncher;

/* The battle task slot of BattleFx_SpecialRocketLauncherTask. */
typedef struct BattleFxRocketLauncherSlot {
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
    unsigned char pad_16[0x4];
    short arcVelocityY;
} BattleFxRocketLauncherSlot;

/* What BattleFx_SpecialRocketLauncherTask reads through g_BattleSpecialRocketLauncherDrawListHandle. */
typedef struct BattleFxRocketLauncherDrawListHandle {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char colorCmd;
} BattleFxRocketLauncherDrawListHandle;

#pragma pack(push, 1)
typedef struct BattleSpecialRocketLauncherSpriteReq2 {
    unsigned char pad_0[0x10];
    int modelHandle; /* +0x10 */
    unsigned char pad_14[0xc];
    int misairuReq2Table32; /* +0x20 */
    int firstDrawObj; /* +0x24 */
    unsigned char pad_28[0x4];
    short misairuReq2FrameIndex44; /* +0x2c */
    unsigned char pad_2e[0x2];
    int misairuReq2Table48; /* +0x30 */
    int secondDrawObj; /* +0x34 */
    unsigned char pad_38[0x4];
    short misairuReq2HalfWord60; /* +0x3c */
    unsigned char pad_3e[0x2];
} BattleSpecialRocketLauncherSpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_mizu_water --- */
/* The battle task slot of BattleFx_SpecialWaterGunTimelineTask. */
typedef struct BattleFxWaterGunTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_12[0xa];
    short targetSlot;
} BattleFxWaterGunTimelineSlot;

/* The battle task slot of BattleFx_SpecialWaterGunStretchModelTask. */
typedef struct BattleFxWaterGunStretchModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short dirX;
    short dirY;
    short dirZ;
    unsigned char pad_12[0x6];
    short scaleZ;
} BattleFxWaterGunStretchModelSlot;

/* --- battle_fx_special_nadare_effect --- */
/* Nadare timeline record: random per-axis offsets for spawned models. */
typedef struct BattleFxAvalancheTimeline {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0x6];
    short directionOffset;
} BattleFxAvalancheTimeline;

/* Nadare sprite record that drifts in Y and Z; its counter is the frame index. */
typedef struct BattleFxAvalancheDriftSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x2];
    short posY;
    short posZ;
} BattleFxAvalancheDriftSprite;

/* The battle task slot of BattleFx_SpecialAvalancheGrowingModelTask. */
typedef struct BattleFxAvalancheGrowingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short velocity;
} BattleFxAvalancheGrowingModelSlot;

/* Main task slot of the Nadare Y-drift sprite, counting frames and drifting in y. */
typedef struct BattleAvalancheYDriftSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x2];
    short posY;
} BattleAvalancheYDriftSprite;

/* Task slot of the Nadare timeline: the caster's bone position and the target mask centroid. */
typedef struct BattleFxAvalancheTaskSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxAvalancheTaskSlot;

/* Task slot for a growing Nadare model, seeded from a random value. */
typedef struct BattleFxAvalancheGrowSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
} BattleFxAvalancheGrowSlot;

/* --- battle_fx_special_naparm_effect --- */
/* Effect-slot record of the Naparm timeline: positions, velocities, angle and variant. */
typedef struct BattleFxSpecialNapalmSlot {
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
} BattleFxSpecialNapalmSlot;

/* The battle task slot of BattleFx_SpecialNapalmImpactModelsTask. */
typedef struct BattleFxNapalmImpactModelsSlot {
    short ownerSlot;
    short frameCounter;
    short originX;
    short originY;
    short originZ;
} BattleFxNapalmImpactModelsSlot;

#pragma pack(push, 1)
typedef struct BattleSpecNapalmModelReq {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    char code; /* +0x3 */
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
    int res8; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
    unsigned char pad_14[0x4];
    int res24; /* +0x18 */
    int flags; /* +0x1c */
    unsigned char pad_20[0x2];
    short secondAlpha; /* +0x22 */
} BattleSpecNapalmModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecNapalmScale {
    unsigned char pad_0[0x10];
    int modelRes; /* +0x10 */
} BattleSpecNapalmScale;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecNapalmSpriteReq2 {
    unsigned char pad_0[0x8];
    int fadingRecord; /* +0x8 */
    int drawObj; /* +0xc */
} BattleSpecNapalmSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecNapalmSpriteReq3 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSpecNapalmSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecNapalmSpriteReq4 {
    short rotX; /* +0x0 */
    short rotY; /* +0x2 */
    short rotZ; /* +0x4 */
    unsigned char pad_6[0x2];
    int res8; /* +0x8 */
    int res12; /* +0xc */
    unsigned char pad_10[0x4];
    short firstFrame; /* +0x14 */
    unsigned char pad_16[0x2];
    int res24; /* +0x18 */
    int res28; /* +0x1c */
    unsigned char pad_20[0x4];
    short secondFrame; /* +0x24 */
    unsigned char pad_26[0x2];
    int res40; /* +0x28 */
    int res44; /* +0x2c */
} BattleSpecNapalmSpriteReq4;
#pragma pack(pop)

/* --- battle_fx_special_netu --- */
/* Slots BattleFx_SpecialHeatWaveDrawSpinModelsEmitTask allocates for BattleFx_SpecialNetu{AnimatedSprite, 2 more}Task. */
typedef struct BattleFxHeatWaveEmitterTask {
    unsigned char pad_00[0x4];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0xa];
    short rotation;
    short variant;
} BattleFxHeatWaveEmitterTask;

/* The battle task slot of BattleFx_SpecialHeatWaveRisingSpinningModelTask. */
typedef struct BattleFxHeatWaveRisingSpinningModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short spinAngle;
    short modelIndex;
} BattleFxHeatWaveRisingSpinningModelSlot;

/* The battle task slot BattleFx_SpecialBeginHeatWave allocates for BattleFx_SpecialHeatWaveTimelineTask: the
 * caster's bone position and the centroid of the target mask. */
typedef struct BattleFxHeatWaveTimelineSlot {
    unsigned char pad_00[0x4];
    short bonePos[3];
    unsigned char pad_0A[0x2];
    short targetPos[3];
} BattleFxHeatWaveTimelineSlot;

/* --- battle_fx_special_nonosi --- */
/* Per-slot Nonosi path: start and end positions per axis, a bone index and a step counter. */
typedef struct BattleFxBadMouthPath {
    short ownerSlot;
    short step;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxBadMouthPath;

/* Nonosi particle: position, velocity and scale. */
typedef struct BattleFxBadMouthParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short orbitAngle;
    short modelIndex;
    short orbitRadius;
    short scale;
    short spinPhase;
} BattleFxBadMouthParticle;

/* The battle task slot of BattleFx_SpecialBadMouthSpinModelTask. */
typedef struct BattleFxBadMouthSpinModelSlot {
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
    short modelIndex;
    short radius;
    short spinAngle;
    short spinPhase;
} BattleFxBadMouthSpinModelSlot;

/* Primitive written into the packet for the Nonosi fragmented model draw. */
typedef struct BattleFxBadMouthDrawPrim {
    int tag;
    unsigned char pad_04[0x3];
    char polyCommand;
    unsigned char pad_08[0x4];
    int colorWord;
    unsigned char pad_10[0x4];
    short uvWord0;
    short drawFlags;
    unsigned char pad_18[0x4];
    short uvWord1;
} BattleFxBadMouthDrawPrim;

/* Header of a fragmented battle model giving its block offset, flags and brightness. */
typedef struct BattleFxFragmentedModelHeader {
    int blockOffset;
    int flags;
    unsigned char pad_08[0x2];
    short brightness;
    short polyFlags;
} BattleFxFragmentedModelHeader;

/* One bouncing fragment's offset from its model position (g_BattleSpecialBadMouthFragmentState2, 128 records). */
typedef struct BattleFxBadMouthFragmentOffset {
    short fallPosition;
    short posY; /* clamped to 0 when it rises above it */
    short posX;
    unsigned char pad_06[0x2];
} BattleFxBadMouthFragmentOffset;

/* One fragment's velocity from BattleFx_MakeRandomConeVector (g_BattleSpecialBadMouthFragmentState1, 128 records); velocityY gains 0x14 each step and flips sign at the bounce. */
typedef struct BattleFxBadMouthFragmentVelocity {
    short fallVelocity;
    short velocityY;
    short velocityX;
    unsigned char pad_06[0x2];
} BattleFxBadMouthFragmentVelocity;

/* --- battle_fx_special_nozan --- */
/* Per-slot Nozan particle: spawn position around the centroid, phase and variant. */
typedef struct BattleFxNorthernCrossParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short phase;
    short variant;
    short startOffset;
} BattleFxNorthernCrossParticle;

/* The first 16 bytes of a model-draw request (BattleFxModelDraw) that Nozan fills in. */
typedef struct {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} BattleFxNorthernCrossModelReq;

/* The battle task slot of BattleFx_SpecialNorthernCrossFallingSpinModelTask. */
typedef struct BattleFxNorthernCrossFallingSpinModelSlot {
    short ownerSlot;
    short frameCounter;
    short velocityX;
    short posY;
    short velocityZ;
    unsigned char pad_0A[0xa];
    short angle;
    short spinRate;
    short radius;
} BattleFxNorthernCrossFallingSpinModelSlot;

/* --- battle_fx_special_oil --- */
#pragma pack(push, 1)
typedef struct BattleSpecialOilScale {
    int scaleX; /* +0x0 */
    int component4; /* +0x4 */
    int component8; /* +0x8 */
    unsigned char pad_c[0x4];
} BattleSpecialOilScale;
#pragma pack(pop)

/* --- battle_fx_special_onpa_radial --- */
/* Onpa timeline record: frame counter, start and end positions, spawned record type. */
typedef struct BattleFxUltrasoundTimeline {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0x6];
    short actorIndex;
} BattleFxUltrasoundTimeline;

/* An Onpa particle task slot, shared by the radial sprites (frame is the sprite frame) and the
 * growing models (frame counts their lifetime; scale grows each frame). */
typedef struct BattleFxUltrasoundParticle {
    short ownerSlot;
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
} BattleFxUltrasoundParticle;

#pragma pack(push, 1)
typedef struct BattleSpecUltrasoundDrawReq {
    BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int radialSpriteField; /* +0x4 */
    unsigned char pad_8[0x4];
    short radialFrameIndex; /* +0xc */
    unsigned char pad_e[0x2];
    int modelField; /* +0x10 */
} BattleSpecUltrasoundDrawReq;
#pragma pack(pop)

/* --- battle_fx_special_onsen --- */
/* Onsen timeline task slot: timeline step counter and the allocated record type. */
typedef struct BattleFxHotSpringsTimelineSlot {
    short ownerSlot;
    short timelineStep;
    unsigned char pad_04[0x8];
    short targetBonePos[3]; /* +0xC: world position of the target's bone (Battle_GetBoneWorldPos) */
    unsigned char pad_12[0x6];
    short actorIndex;
} BattleFxHotSpringsTimelineSlot;

#pragma pack(push, 1)
/* The first two components of the source bone's world position; the third is
 * g_BattleSpecHotSpringsSourceBonePosZ. */
typedef struct BattleFxHotSpringsBonePos {
    short x;
    short y;
} BattleFxHotSpringsBonePos;
#pragma pack(pop)

/* One of the 300 Onsen particles. state is 0 when free and 1 when live; fade counts up after the
 * particle has passed the top of its arc and darkens the sprite; scale is the billboard size. */
typedef struct BattleFxHotSpringsParticle {
    short state;
    short fade;
    short scale;
    short pad_06;
    SVECTOR pos;
    SVECTOR vel;
} BattleFxHotSpringsParticle;

/* --- battle_fx_special_osen_toxic --- */
/* Osen drifting ribbon task slot: frame counter, position and velocity accumulators, and wave phase. */
typedef struct BattleFxToxicSmogDriftRibbon {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x8];
    short wavePhase;
} BattleFxToxicSmogDriftRibbon;

/* Osen spin-fade sprite record; its age counter drives the fade and the spin angle. */
typedef struct BattleFxToxicSmogSpinFadeSprite {
    short ownerSlot;
    short age;
    Vec3sPacked pos;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short angle;
} BattleFxToxicSmogSpinFadeSprite;

/* The battle task slot of BattleFx_SpecialToxicSmogTimelineTask. */
typedef struct BattleFxToxicSmogTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
} BattleFxToxicSmogTimelineSlot;

/* What BattleFx_SpecialToxicSmogDrawWavyRibbon reads through Gfx_GetPsxPacketScratchPtr(). */
typedef struct BattleFxToxicSmogRibbonState {
    int tag;
    int colorCmd;
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
} BattleFxToxicSmogRibbonState;

#pragma pack(push, 1)
typedef struct BattleSpecialToxicSmogRibbonBuffer {
    int targetMask; /* +0x0 */
    short ribbonIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    PsxMatrix rotMatrix; /* +0x8 */
} BattleSpecialToxicSmogRibbonBuffer;
#pragma pack(pop)

#pragma pack(push, 1)
/* The tail of the third sprite request (its colour bytes) and the head of the fourth (the drift ribbon's
 * sprite group and draw object): the sprite requests run from g_BattleSpecialToxicSmogSpriteReq1 through
 * g_BattleSpecialToxicSmogColor2. */
typedef struct BattleSpecialToxicSmogColor1 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x5];
    struct BattleFxSpriteGroup *spriteGroup; /* +0x8 */
    int drawObj; /* +0xc */
} BattleSpecialToxicSmogColor1;
#pragma pack(pop)

#pragma pack(push, 1)
/* The colour bytes and frame of the fourth sprite request, the drift ribbon's. */
typedef struct BattleSpecialToxicSmogColor2 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSpecialToxicSmogColor2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialToxicSmogModelReq1 {
    int modelResField; /* +0x0 */
    unsigned char pad_4[0x6];
    short osenModelReq1StateShift; /* +0xa */
} BattleSpecialToxicSmogModelReq1;
#pragma pack(pop)

/* --- battle_fx_special_pink_effect --- */
/* Special pink effect slot: a timer and the start and end points of a moving segment (x, height, y). */
typedef struct BattleFxSpecialHeartBreathSegment {
    short step;
    short timer;
    SVECTOR start;
    SVECTOR end;
} BattleFxSpecialHeartBreathSegment;

/* Pink special timeline record that interpolates from a start position over 20 steps. */
typedef struct BattleFxHeartBreathTimelineRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posHeight;
    short posY;
    unsigned char pad_0A[0x2];
    short stepX;
    short angle;
    short stepY;
    unsigned char pad_12[0x2];
    short phase;
    unsigned char pad_16[0x2];
    short lengthScale;
    short scalePhase;
} BattleFxHeartBreathTimelineRecord;

/* Particle-sprite pool slot for the Pink effect, with a status short at status. */
typedef struct BattleFxHeartBreathParticleSpriteSlot {
    short status;
    short timer;
} BattleFxHeartBreathParticleSpriteSlot;

/* The battle task slot of BattleFx_SpecialHeartBreathGrowingModelTask. */
typedef struct BattleFxHeartBreathGrowingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    unsigned char pad_0E[0xa];
    short modelStep;
    short phase;
} BattleFxHeartBreathGrowingModelSlot;

/* Pink special particle sprite: position, velocity, timer and angle. */
typedef struct BattleFxHeartBreathParticleSprite {
    short status;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short angle;
} BattleFxHeartBreathParticleSprite;

/* One record of g_BattleSpecHeartBreathParticleRecords (8 bytes): whether it is live, its polar position and how far it has spread. */
typedef struct {
    short active; /*+0x00 */
    short radius; /*+0x02 */
    short angle; /*+0x04 */
    short radiusScale; /*+0x06 */
} BattleSpecHeartBreathParticleRecord;

/* The resource definition rows, then the particle sprites' two handles; the rest of that sprite
 * request is g_BattleSpecHeartBreathSpriteReq1's colour. */
typedef struct {
    int rows[32];
    struct BattleFxSpriteGroup *spriteGroup; /* +0x80 */
    int drawObj; /* +0x84 */
} BattleSpecHeartBreathResourceModelRows;

#pragma pack(push, 1)
typedef struct BattleSpecHeartBreathSpriteReq2 {
    int modelScale; /* +0x0 */
    unsigned char pad_4[0x4];
    int res8; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
} BattleSpecHeartBreathSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeartBreathSpriteReq4 {
    int modelField; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
} BattleSpecHeartBreathSpriteReq4;
#pragma pack(pop)

/* --- battle_fx_special_raku --- */
/* Raku timeline task slot: step index, the casting and target bone positions, and the target actor index. */
typedef struct BattleFxFallingRocksTimeline {
    short ownerSlot;
    short step;
    SVECTOR sourcePos;
    SVECTOR targetPos;
    unsigned char pad_14[0x6];
    short actorIndex;
} BattleFxFallingRocksTimeline;

/* Raku bouncing sprite record: start point and horizontal velocity from a fixed angle. */
typedef struct BattleFxFallingRocksBouncer {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short randomIndex;
} BattleFxFallingRocksBouncer;

/* The battle task slot of BattleFx_SpecialFallingRocksBouncingFragmentModelTask. */
typedef struct BattleFxFallingRocksBouncingFragmentModelSlot {
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
    short modelIndex;
    short angle;
    short angleVelocity;
} BattleFxFallingRocksBouncingFragmentModelSlot;

/* The battle task slot of BattleFx_SpecialFallingRocksBouncingSpriteTask. */
typedef struct BattleFxFallingRocksBouncingSpriteSlot {
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
    short color;
} BattleFxFallingRocksBouncingSpriteSlot;

/* --- battle_fx_special_redo --- */
/* ReDo centroid timeline record: step, position, centroid offsets and model index. */
typedef struct BattleFxEnergyOrbRecord {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_12[0xc];
    short actorIndex;
} BattleFxEnergyOrbRecord;

/* --- battle_fx_special_reiki_effect --- */
/* Timeline record of the Reiki special, allocated from the effect slot array. */
typedef struct BattleFxColdAirTimeline2 {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
} BattleFxColdAirTimeline2;

/* Reiki rising-fade sprite record; its counter steps the fade value down. */
typedef struct BattleFxColdAirRisingFadeSprite {
    short ownerSlot;
    short fadeStep;
    short posX;
    short posY;
    unsigned char pad_08[0xe];
    short frame;
} BattleFxColdAirRisingFadeSprite;

/* Slots BattleFx_SpecialColdAirDrawSpinModelsEmitTask allocates for BattleFx_SpecialReiki{AnimatedSprite, 2 more}Task. */
typedef struct BattleFxColdAirEmitterTask {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0xa];
    short randomAngle;
    short randomIndex;
} BattleFxColdAirEmitterTask;

/* The battle task slot of BattleFx_SpecialColdAirRisingSpinningModelTask. */
typedef struct BattleFxColdAirRisingSpinningModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short spinAngle;
    short modelIndex;
} BattleFxColdAirRisingSpinningModelSlot;

/* The rising spinning model's four model ids, then the rising fade sprite's two handles; the rest
 * of that sprite request is g_BattleSpecColdAirSpriteReq4's colour and frame. */
typedef struct {
    int modelIds[4];
    int risingFadeDrawBlock; /* +0x10 */
    int risingFadeHandle; /* +0x14 */
} BattleSpecColdAirModelIndices2;

#pragma pack(push, 1)
typedef struct BattleSpecColdAirSpriteReq1 {
    int modelResource; /* +0x0 */
    int size; /* +0x4 */
    unsigned char pad_8[0x2];
    short scaleWord; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecColdAirSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecColdAirSpriteReq2 {
    unsigned char pad_0[0x20];
    int drawBlock; /* +0x20 */
    int handle; /* +0x24 */
} BattleSpecColdAirSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
/* The expanding glow sprite's colour (its handles end g_BattleSpecColdAirSpriteReq2), the animated
 * sprite's request, then the head of the rising spinning model's draw request. */
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char primCode; /* +0x3 */
    unsigned char pad_4[0x4];
    int drawBlock; /* +0x8 */
    int sceneObject; /* +0xc */
    char animRed; /* +0x10 */
    char animGreen; /* +0x11 */
    char animBlue; /* +0x12 */
    unsigned char animPrimCode; /* +0x13 */
    short frameIndex; /* +0x14 */
    unsigned char pad_16[0x2];
    int modelGroup; /* +0x18 */
    int drawFlags; /* +0x1c */
    short uvScroll; /* +0x20 */
    short fadeLevel; /* +0x22 */
} BattleSpecColdAirSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
/* The rising fade sprite's colour and frame, the spin model's draw request head, and the
 * ground glow sprite's two handles (its colour is g_BattleSpecColdAirColorData). */
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char primCode; /* +0x3 */
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    BattleFxModelDrawHead modelDraw; /* +0x8 */
    int drawBlock; /* +0x18 */
    int handle; /* +0x1c */
} BattleSpecColdAirSpriteReq4;
#pragma pack(pop)

/* --- battle_fx_special_reito_effect --- */
// Copies the 0x664-byte Reito template g_BattleSpecFreezeBreathInitialTemplate over g_BattleSpecFreezeBreathWorkingTemplate (one aggregate copy:
// REP MOVSD of 0x199 dwords), resets its effect state, clears g_BattleSpecFreezeBreathFaceIndex and begins the effect.
// VC5 sets up that copy count-first only for a relocated source (a raw address loads ESI first),
// so the original names the template; it is one initialized 0x664-byte storage object
// with the original payload. g_BattleSpecFreezeBreathWorkingTemplate stays declared as a scalar
typedef struct FreezeBreathTemplate {
    int words[0x199];
} FreezeBreathTemplate;

/* Orbiting sprite record for the Reito special effect. */
typedef struct BattleFxFreezeBreathOrbitSprite {
    short ownerSlot;
    short step;
    short centerX;
    short posY;
    short centerZ;
    unsigned char pad_0A[0xe];
    short angle;
    short radius;
} BattleFxFreezeBreathOrbitSprite;

/* Reito per-target timeline record: start and end points, scaled by one eighth. */
typedef struct BattleFxFreezeBreathTimeline {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short sourceActor;
} BattleFxFreezeBreathTimeline;

/* Slot record of a Reito spin model, placed at the source record's position. */
typedef struct BattleFreezeBreathSpinModel {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short phase;
    short radius;
} BattleFreezeBreathSpinModel;

/* One face's offset in the per-face offset table of g_BattleSpecFreezeBreathFaceIndex. */
typedef struct {
    short x;
    short y;
    short z;
    short pad;
} BattleSpecReitoFaceOffset;

/* The next face to release, then the 0x80 per-face offsets; the object is 16 bytes and the offsets
 * run on into the bytes that follow it. */
typedef struct {
    short faceIndex; /* +0x0 */
    unsigned char pad_2[0x6];
    BattleSpecReitoFaceOffset offsets[1]; /* +0x8 */
} BattleSpecFreezeBreathFaceIndex;

/* The scaled sprite's scale vector, then the orbiting and scattered animated sprites' requests. */
typedef struct {
    VECTOR scale; /* +0x0 */
    BattleFxSpriteRequest orbitSprite; /* +0x10 */
    BattleFxSpriteRequest scatterSprite; /* +0x20 */
} BattleSpecFreezeBreathScaleVector1;

#pragma pack(push, 1)
typedef struct BattleSpecFreezeBreathScaleVector2 {
    unsigned char pad_0[0x10];
    int modelResource; /* +0x10 */
    unsigned char pad_14[0xc];
    int meshTemplate; /* +0x20 */
} BattleSpecFreezeBreathScaleVector2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecFreezeBreathSpriteReq {
    int spriteResource; /* +0x0 */
    int resourceHandle; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecFreezeBreathSpriteReq;
#pragma pack(pop)

/* --- battle_fx_special_renbaku_effect --- */
/* Timeline record for the Renbaku special effect: phase, the position it sweeps along, angle and random jitter. The setup task fills it before the phase counts. */
typedef struct BattleFxChainExplosionTimeline {
    short ownerSlot;
    short phase;
    SVECTOR pos;
    short angle;
    short angleStep;
    short jitterBias;
    short jitterRange;
    short scale;
    unsigned char pad_16[0x2];
    short stepSize;
} BattleFxChainExplosionTimeline;

/* Renbaku explosion point (8 bytes): position and scale, converged toward the target position. */
typedef struct BattleFxChainExplosionExplosionPoint {
    short posX;
    short posY;
    short posZ;
    short scale;
} BattleFxChainExplosionExplosionPoint;

/* --- battle_fx_special_reza_laser --- */
/* Timeline record of the Reza1 effect; two actor indices feed bone position lookups. */
typedef struct BattleFxLaserCannonTimelineRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x10];
    short actorA;
    short actorB;
} BattleFxLaserCannonTimelineRecord;

/* Timeline record for the Reza1 special effect, one per target. */
typedef struct BattleFxLaserCannonTimeline {
    unsigned char pad_00[0x14];
    short actorIndex;
    short secondIndex;
} BattleFxLaserCannonTimeline;

/* A child task slot of the Reza1 effect: the ribbon (both end points) and the sprites (the first). */
typedef struct BattleFxLaserCannonChild {
    short ownerSlot;
    short frame;
    short firstPos[3]; /* world position of the beam's source bone */
    unsigned char pad_0A[0x2];
    short secondPos[3]; /* world position of the target's bone */
} BattleFxLaserCannonChild;

/* The ribbon's flat-textured quad packet (a PSX POLY_FT4, 0x28 bytes) from Gfx_GetPsxPacketScratchPtr:
 * the command word, the colour word with primitive code, then four corners of screen position
 * and texture position. */
typedef struct BattleFxLaserCannonRibbonQuad {
    int command;
    union {
        int colorCmd;
        struct {
            char colorR;
            char colorG;
            char colorB;
            char primCode;
        };
    };
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
    unsigned char pad_1E[0x2];
    short x3;
    short y3;
    char u3;
    char v3;
} BattleFxLaserCannonRibbonQuad;

/* --- battle_fx_special_reza_quad_trail --- */
/* Slots BattleFx_SpecialFourLaserTimelineTask allocates for BattleFx_SpecialReza2{AnimatedSpriteEntry1, 2 more}Task. */
typedef struct BattleFxFourLaserTimelineSpawnSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short angle;
    short velocityZ;
    unsigned char pad_12[0x2];
    short orbitAngle;
    short angularVelocity;
    short radius;
    short radiusScale;
} BattleFxFourLaserTimelineSpawnSlot;

/* What BattleFx_SpecialFourLaserDrawQuadRecords reads through g_BattleSpecialFourLaserDrawListHandle. */
typedef struct BattleFxFourLaserQuadBatchHead {
    int tag;
    char colorR0;
    char colorG0;
    char colorB0;
    char primCode;
    unsigned char pad_08[0x4];
    char colorR1;
    char colorG1;
    char colorB1;
    unsigned char pad_0F[0x5];
    char colorR2;
    char colorG2;
    char colorB2;
    unsigned char pad_17[0x5];
    char colorR3;
    char colorG3;
    char colorB3;
} BattleFxFourLaserQuadBatchHead;

/* What BattleFx_SpecialFourLaserDrawQuadRecords reads through Gfx_GetProjectedVertexRecord(0). */
typedef struct BattleFxFourLaserProjectedVertexRecord {
    unsigned char pad_00[0x8];
    int projectedX;
    int projectedY;
} BattleFxFourLaserProjectedVertexRecord;

/* One of the 500 trail records of g_BattleSpecialFourLaserQuadRecords (0x14 bytes): live while active, aged by
 * timer until it reaches 8, and drawn as a quad between the two points the orbit task wrote. */
typedef struct BattleSpecialFourLaserQuadRecord {
    short active;
    short timer;
    Vec3sPacked startPoint;
    unsigned char pad_0A[0x2];
    Vec3sPacked endPoint;
    unsigned char pad_12[0x2];
} BattleSpecialFourLaserQuadRecord;

#pragma pack(push, 1)
typedef struct BattleSpecialFourLaserSpriteConfig {
    short targetSlot; /* +0x0 */
    unsigned char pad_2[0x6];
    int reza2SpriteConfigTable8; /* +0x8 */
    int drawObj; /* +0xc */
    unsigned char pad_10[0x4];
    short reza2FrameIndex20; /* +0x14 */
    unsigned char pad_16[0x2];
    int reza2DrawTable24; /* +0x18 */
    int reza2SceneObject28; /* +0x1c */
    unsigned char pad_20[0x4];
    short reza2ArgWord36; /* +0x24 */
    unsigned char pad_26[0x2];
} BattleSpecialFourLaserSpriteConfig;
#pragma pack(pop)

/* --- battle_fx_special_roket_effect --- */
/* Roket falling sprite record: position, velocity per axis and a frame counter. */
typedef struct BattleFxHellBomberFallingSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x8];
    short variant;
} BattleFxHellBomberFallingSprite;

/* The battle task slot of BattleFx_SpecialHellBomberGrowingMorphModelsTask. */
typedef struct BattleFxHellBomberGrowingMorphModelsSlot {
    short ownerSlot;
    short frameCounter;
    short centerX;
    short centerY;
    short centerZ;
} BattleFxHellBomberGrowingMorphModelsSlot;

/* --- battle_fx_special_sander --- */
/* Sander bolt emitter record: a parity counter and two position/step pairs. */
typedef struct BattleFxSwiftBoltBoltEmitterRecord {
    short ownerSlot;
    short emitIndex;
    short posA;
    unsigned char pad_06[0x2];
    short posB;
    unsigned char pad_0A[0x2];
    short stepA;
    unsigned char pad_0E[0x2];
    short stepB;
} BattleFxSwiftBoltBoltEmitterRecord;

/* Bolt segment of the Sander effect; offsets from cos and sin of an angle. */
typedef struct BattleFxSwiftBoltBoltSegment {
    unsigned char pad_00[0x4];
    short posA;
    short posY;
    short posB;
    unsigned char pad_0A[0x2];
    short cosOffset;
    short startOffset;
    short sinOffset;
    unsigned char pad_12[0x4];
    short stepCount;
} BattleFxSwiftBoltBoltSegment;

/* Slots BattleFx_SpecialSwiftBoltTimelineTask allocates for BattleFx_SpecialSander{AnimatedSprite, BoltEmitter, 3 more}Task. */
typedef struct BattleFxSwiftBoltTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short phase;
    short variant;
} BattleFxSwiftBoltTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialSwiftBoltAnimatedSpriteTask: a position, then the index into the frame table. */
typedef struct BattleFxSwiftBoltAnimatedSpriteSlot {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xC];
    short animIndex;
} BattleFxSwiftBoltAnimatedSpriteSlot;

/* The battle task slot of BattleFx_SpecialSwiftBoltGrowingFadeSpriteTask. */
typedef struct BattleFxSwiftBoltGrowingFadeSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxSwiftBoltGrowingFadeSpriteSlot;

/* The battle task slot of BattleFx_SpecialSwiftBoltBouncingSpriteTask. */
typedef struct BattleFxSwiftBoltBouncingSpriteSlot {
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
    short spinAngle;
    short variant;
} BattleFxSwiftBoltBouncingSpriteSlot;

/* The battle task slot of BattleFx_SpecialSwiftBoltDrawBoltSegmentTask. */
typedef struct BattleFxSwiftBoltDrawBoltSegmentSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x8];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_12[0x4];
    short spriteFrame;
} BattleFxSwiftBoltDrawBoltSegmentSlot;

/* The battle task slot of BattleFx_SpecialSwiftBoltRadialSparkSpriteTask. */
typedef struct BattleFxSwiftBoltRadialSparkSpriteSlot {
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
    short angleZ;
    short angleY;
} BattleFxSwiftBoltRadialSparkSpriteSlot;

#pragma pack(push, 1)
typedef struct BattleSpecSwiftBoltTargetIdx {
    short targetX; /* +0x0 */
    short targetY; /* +0x2 */
} BattleSpecSwiftBoltTargetIdx;
#pragma pack(pop)

/* --- battle_fx_special_sarchi_searchlight --- */
/* Sarchi timeline state: start and end positions per axis, a step counter and a record type. */
typedef struct BattleFxSearchlightTimelineState {
    short ownerSlot;
    short step;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0x6];
    short recordType;
} BattleFxSearchlightTimelineState;

/* Sarchi sweep timeline record: start position copied from the beam endpoints and a yaw angle. */
typedef struct BattleFxSearchlightSweep {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x4];
    short angle;
    unsigned char pad_10[0x4];
    short scaledLength;
    short variant;
    unsigned char pad_18[0x4];
    short pathLength;
} BattleFxSearchlightSweep;

/* The battle task slot of BattleFx_SpecialSearchlightSweepModelTask. */
typedef struct BattleFxSearchlightSweepModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short yaw;
    unsigned char pad_10[0x4];
    short scaledLength;
    short variant;
    unsigned char pad_18[0x4];
    short pathLength;
} BattleFxSearchlightSweepModelSlot;

/* --- battle_fx_special_sazan --- */
/* Per-slot Sazan particle: spawn position around the centroid, phase and variant. */
typedef struct BattleFxSouthernCrossParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short phase;
    short variant;
    short delay;
} BattleFxSouthernCrossParticle;

/* The battle task slot of BattleFx_SpecialSouthernCrossOrbitSpriteTask. */
typedef struct BattleFxSouthernCrossOrbitSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0xa];
    short angle;
    short spinRate;
    short radius;
} BattleFxSouthernCrossOrbitSpriteSlot;

/* --- battle_fx_special_scorp --- */
/* Scorpion timeline record: start and end positions on three axes. */
typedef struct BattleFxSearchScopePath {
    short ownerSlot;
    short step;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
} BattleFxSearchScopePath;

/* Task-slot record for the Scorp two-stage animated sprite: frame counter and resource index. */
typedef struct BattleFxSearchScopeSpriteTask {
    short ownerSlot;
    short frame;
    short pointX;
    unsigned char pad_06[0x14];
    short resourceIndex;
} BattleFxSearchScopeSpriteTask;

/* Scorp timeline record: start point and yaw angle toward the end point. */
typedef struct BattleFxSearchScopeTimeline {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x4];
    short yaw;
    unsigned char pad_10[0x4];
    short scaledLength;
    short variant;
    short pathLength;
} BattleFxSearchScopeTimeline;

/* Scorp line record: start and end vectors whose difference the begin routine stores. */
typedef struct BattleFxSearchScopeLine {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
} BattleFxSearchScopeLine;

/* The battle task slot of BattleFx_SpecialSearchScopeDrawWeavingModelTask. */
typedef struct BattleFxSearchScopeDrawWeavingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short yaw;
    unsigned char pad_10[0x4];
    short scaledLength;
    short variant;
    short pathLength;
} BattleFxSearchScopeDrawWeavingModelSlot;

/* State of the two-stage Scorp special effect sprite. */
typedef struct BattleFxSearchScopeTwoStageState {
    unsigned char pad_00[0x48];
    int resourceField;
    int resourceAlt;
} BattleFxSearchScopeTwoStageState;

#pragma pack(push, 1)
typedef struct BattleSpecialSearchScopeModelReq {
    int modelHandle; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialSearchScopeModelReq;
#pragma pack(pop)

/* --- battle_fx_special_sekika_effect --- */
/* The frame count of a sprite group, read through a sprite request's spriteGroup (the whole group is BattleFxSpriteGroup in battle_fx_model_draw.h). */
typedef struct BattleSekikaSpriteGroupHeader {
    unsigned char pad_00[0x4];
    int frameCount;
} BattleSekikaSpriteGroupHeader;

typedef struct BattlePetrifySpriteGroup {
    BattleSekikaSpriteGroupHeader *header;
} BattlePetrifySpriteGroup;

/* Sekika bouncing particle: a position or a velocity, one short per axis. */
typedef struct BattlePetrifyParticle {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
} BattlePetrifyParticle;

/* Task slot of BattleFx_SpecialPetrifyGrowingModelTask: frame counter and the bone position it is drawn at. */
typedef struct BattleFxPetrifyGrowingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxPetrifyGrowingModelSlot;

/* Orbiting sprite record for the Sekika special effect. */
typedef struct BattleFxPetrifyOrbitSprite {
    short ownerSlot;
    short step;
    short centerX;
    short posY;
    short centerZ;
    unsigned char pad_0A[0xc];
    short angle;
    short radius;
} BattleFxPetrifyOrbitSprite;

/* Sekika variant sprite record: step counter, position, velocities and an animation index. */
typedef struct BattleFxPetrifyVariantSprite {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x8];
    short animIndex;
} BattleFxPetrifyVariantSprite;

/* Sekika orbit sprite task slot: orbit step counter and sprite position words. */
typedef struct BattleFxPetrifyOrbitSlot {
    short ownerSlot;
    short orbitStep;
    short posX;
    short posY;
    short posZ;
} BattleFxPetrifyOrbitSlot;

/* Sekika per-target timeline record: start and end points. */
typedef struct BattleFxPetrifyTimeline {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxPetrifyTimeline;

/* The battle task slot of BattleFx_SpecialPetrifySpiralModelTask. */
typedef struct BattleFxPetrifySpiralModelSlot {
    short ownerSlot;
    short frameCounter;
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0x4];
    short headingAngleY;
    unsigned char pad_10[0x4];
    short scaledPathLength;
    unsigned char pad_16[0x2];
    short pathLength;
    short spiralArm;
} BattleFxPetrifySpiralModelSlot;

/* The battle task slot of BattleFx_SpecialPetrifyPerFaceOffsetModelTask. */
typedef struct BattleFxPetrifyPerFaceOffsetModelSlot {
    short ownerSlot;
    short frameCounter;
    short offsetX;
    short offsetY;
    short offsetZ;
} BattleFxPetrifyPerFaceOffsetModelSlot;

/* Slots BattleFx_SpecialPetrifyTimelineTask allocates for BattleFx_SpecialSekika{GrowingModel, 4 more}Task. */
typedef struct BattleFxPetrifyTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short angle;
    unsigned char pad_10[0x4];
    short scale;
    unsigned char pad_16[0x2];
    short pathLength;
    short variant;
} BattleFxPetrifyTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialPetrifyTwoPhaseSpriteTask. */
typedef struct BattleFxPetrifyTwoPhaseSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0x2];
    short clearedWordA;
    short headingAngleY;
    short clearedWordB;
    unsigned char pad_12[0x8];
    short phase;
} BattleFxPetrifyTwoPhaseSpriteSlot;

/* The battle task slot of BattleFx_SpecialPetrifyFallingSpriteTask. */
typedef struct BattleFxPetrifyFallingSpriteSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short offsetCos;
    short randomVelocity;
    short offsetSin;
} BattleFxPetrifyFallingSpriteSlot;

/* Orbit sprite record spawned by the Sekika spawner, placed at the spawner's position. */
typedef struct BattlePetrifyOrbitRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short phase;
    short radius;
} BattlePetrifyOrbitRecord;

/* Primitive written into the packet for the Sekika per-face offset draw. */
typedef struct BattleFxPetrifyDrawPrim {
    int tag;
    union {
        int colorCommand; /* the colour bytes and the polygon command byte, written as one word */
        struct {
            unsigned char pad_04[0x3];
            char polyCommand;
        };
    };
    unsigned char pad_08[0x4];
    int colorWord;
    unsigned char pad_10[0x4];
    short uvWord0;
    short tpageWord;
    unsigned char pad_18[0x4];
    short uvWord1;
} BattleFxPetrifyDrawPrim;

/* g_BattleSpecPetrifySpriteRequest1: two sprite draw requests, the second (sprite[1]) the variant sprite's. */
typedef struct BattleSpecPetrifySpriteRequest1 {
    BattleFxSpriteRequest sprite[2];
} BattleSpecPetrifySpriteRequest1;

/* g_BattleSpecPetrifySpriteRequest2: two sprite draw requests, then the growing model's draw record. */
#pragma pack(push, 1)
typedef struct BattleSpecPetrifySpriteRequest2 {
    BattleFxSpriteRequest sprite[2];
    int modelRecord; /* +0x20 */
    unsigned char pad_24[0x6];
    short req2Word42; /* +0x2a */
} BattleSpecPetrifySpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecPetrifySpriteRequest3 {
    unsigned char pad_0[0x10];
    int req3Model16; /* +0x10 */
    unsigned char pad_14[0xc];
    int faceOffsetRecord; /* +0x20 */
    unsigned char pad_24[0x6];
    short req3Word42; /* +0x2a */
} BattleSpecPetrifySpriteRequest3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecPetrifySpriteRequest4 {
    int model; /* +0x0 */
    unsigned char pad_4[0x6];
    short req4Word10; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecPetrifySpriteRequest4;
#pragma pack(pop)

/* --- battle_fx_special_shield --- */
/* Shield special timeline record: step counter, target positions, effect kind and bone index. */
typedef struct BattleFxShieldTimeline {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x8];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0xa];
    short targetIndex;
    short actorIndex;
} BattleFxShieldTimeline;

/* Special shield projectile task state: step, position and velocity triplets. */
typedef struct BattleFxShieldProjectileState {
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
    short modelIndex;
} BattleFxShieldProjectileState;

/* Shield timeline slot: per-step deltas toward the projectile position. */
typedef struct BattleFxShieldProjectile {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short modelIndex;
} BattleFxShieldProjectile;

/* Shield trail effect record: a variant flag and a model index used to pick the shield model. */
typedef struct BattleFxShieldTrail {
    short ownerSlot;
    short variant;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short modelIndex;
} BattleFxShieldTrail;

/* Main task slot of the shield flash camera shake, counting shake steps. */
typedef struct BattleShieldFlashShake {
    short ownerSlot;
    short shakeStep;
} BattleShieldFlashShake;

/* --- battle_fx_special_shower_effect --- */
/* Shower track record: start and end positions of the falling set, its frame counter and bone index. */
typedef struct BattleFxAcidShowerTrack {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxAcidShowerTrack;

/* Special Shower drop record: position moving toward a target, heading and frame count. */
typedef struct BattleFxAcidShowerDropRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short heading;
    unsigned char pad_16[0x2];
    short frameCount;
    short active;
} BattleFxAcidShowerDropRecord;

/* Shower falling sprite task slot: frame counter, position and velocity on each axis. */
typedef struct BattleFxAcidShowerFallingSprite {
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
} BattleFxAcidShowerFallingSprite;

/* The battle task slot of BattleFx_SpecialAcidShowerSpiralSpritePairTask. */
typedef struct BattleFxAcidShowerSpiralSpritePairSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posZ;
    short posY;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityZ;
    short velocityY;
    unsigned char pad_12[0x2];
    short angle;
    short radius;
    short scale;
    short spiralPhase;
} BattleFxAcidShowerSpiralSpritePairSlot;

/* The battle task slot of BattleFx_SpecialAcidShowerSpiralEmitterTask. */
typedef struct BattleFxAcidShowerSpiralEmitterSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posZ;
    short posY;
    unsigned char pad_0A[0xa];
    short angleY;
    unsigned char pad_16[0x2];
    short scaledPathLength;
} BattleFxAcidShowerSpiralEmitterSlot;

/* The battle task slot of BattleFx_SpecialAcidShowerFallingSpriteTask. */
typedef struct BattleFxAcidShowerFallingSpriteSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short scale;
} BattleFxAcidShowerFallingSpriteSlot;

/* g_BattleSpecAcidShowerModelParams: the animation drawn by each variant of the animated sprite, then the
 * sprite group and draw object of the falling sprite's request, whose colour and frame follow in
 * g_BattleSpecAcidShowerSpriteReq2. */
#pragma pack(push, 1)
typedef struct {
    int variantAnim[2];
    int fallingSpriteGroup;
    int fallingDrawObj;
} BattleSpecAcidShowerSpriteAnims;
#pragma pack(pop)

/* g_BattleSpecAcidShowerSpriteReq2: the rest of the falling sprite's request (colour, code, frame), then the
 * start of the impact model's BattleFxModelDraw, which runs on past this object. */
#pragma pack(push, 1)
typedef struct {
    char fallingColorR;
    char fallingColorG;
    char fallingColorB;
    unsigned char fallingCode;
    short fallingFrame;
    short fallingUvPacked;
    int modelGroup;
    int modelFlags;
    short modelUvScroll;
    short modelFadeLevel;
} BattleSpecAcidShowerImpactDraw;
#pragma pack(pop)

/* g_BattleSpecAcidShowerSpriteReq3: the impact model's rotation, the request of the spiral pair's centre
 * sprite, and the sprite group and draw object of its orbiting sprite, whose colour follows this object. */
#pragma pack(push, 1)
typedef struct {
    SVECTOR impactRotation;
    BattleFxSpriteRequest centreSprite;
    int orbitSpriteGroup;
    int orbitDrawObj;
} BattleSpecAcidShowerSpiralSprites;
#pragma pack(pop)

/* --- battle_fx_special_sickle_slash --- */
/* Effect task record for the Kama2 variant sprite effect; selects a table by index. */
typedef struct BattleFxShamSealVariantTask {
    short ownerSlot; /* +0x0 */
    short frame;
    unsigned char pad_04[0x14];
    short tableIndex;
} BattleFxShamSealVariantTask;

/* Data block for the Kama2 variant sprite task, holding the current frame table pointer. */
typedef struct BattleSpecShamSealDataBlob {
    unsigned char pad_00[0x88];
    int frameTable;
    int tableField;
} BattleSpecShamSealDataBlob;

/* --- battle_fx_special_smog --- */
/* Smog drifting ring record: step counter, three position/velocity pairs and an angle. */
typedef struct BattleFxSmogRing {
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
    short angle;
} BattleFxSmogRing;

/* Smog sprite that drifts and fades over eight frames. */
typedef struct BattleFxSmogDriftingFadeSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short ringIndex;
} BattleFxSmogDriftingFadeSprite;

/* The battle task slot of BattleFx_SpecialSmogTimelineTask. */
typedef struct BattleFxSmogTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0x6];
    short actorIndex;
} BattleFxSmogTimelineSlot;

/* Slots BattleFx_SpecialSmogTimelineTask allocates for BattleFx_SpecialSmog{AnimatedSprite, 2 more}Task. */
typedef struct BattleFxSmogTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short step;
    short state;
} BattleFxSmogTimelineSpawnSlot;

/* What BattleFx_SpecialSmogDrawPsxTexturedQuadStrip reads through Gfx_GetPsxPacketScratchPtr(). */
typedef struct BattleFxSmogQuadStripRenderStateBlock {
    int tag;
    union {
        int color; /* the packet's colour word */
        struct {
            unsigned char pad_04[0x3];
            char commandCode;
        };
    };
    unsigned char pad_08[0x4];
    char u0;
    char v0;
    short clut;
    unsigned char pad_10[0x4];
    char u1;
    char stripTop;
    short tpage;
    unsigned char pad_18[0x4];
    char u2;
    char v2;
    unsigned char pad_1E[0x6];
    char u3;
    char stripBottom;
} BattleFxSmogQuadStripRenderStateBlock;

/* --- battle_fx_special_sumi_ink --- */
/* Battle Sumi timeline slot: step, base position, spread per axis and target index. */
typedef struct BattleFxEvilPoisonTimelineSlot {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spreadX;
    short spreadY;
    short spreadZ;
    unsigned char pad_12[0x6];
    short targetIndex;
} BattleFxEvilPoisonTimelineSlot;

/* Effect100 slot filled by the Sumi special timeline from a source record. */
typedef struct BattleFxSpecialEvilPoisonSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spreadX;
    short spreadY;
    short spreadZ;
    unsigned char pad_12[0x2];
    short index;
} BattleFxSpecialEvilPoisonSlot;

/* Sumi sprite record: current position and per-step deltas toward the target. */
typedef struct BattleFxEvilPoisonSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short targetIndex;
} BattleFxEvilPoisonSprite;

/* Sumi variant sprite task record: frame counter, scale index and variant index. */
typedef struct BattleFxEvilPoisonVariantSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scaleIndex;
    short variant;
} BattleFxEvilPoisonVariantSprite;

/* The battle task slot of BattleFx_SpecialEvilPoisonPulseEmitterTask. */
typedef struct BattleFxEvilPoisonPulseEmitterSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short pulseRadius;
} BattleFxEvilPoisonPulseEmitterSlot;

/* The view of g_BattleSpecialEvilPoisonSpriteReq2: the four variant sprite group indices, the pulse request,
 * and the first two words of the drift request (its colour bytes are the next global's). */
typedef struct BattleSpecialEvilPoisonSpriteReq2 {
    int variantSprites[4];
    BattleFxSpriteRequest pulse;
    int driftSpriteGroup;
    int driftDrawObj;
} BattleSpecialEvilPoisonSpriteReq2;

/* --- battle_fx_special_suna_sandstorm --- */
/* Effect-array record for the Suna timeline: a line from a start point to an end point, with a step. */
typedef struct BattleFxSandstormRecord {
    short ownerSlot;
    short step;
    short startX;                /* the start point is the casting actor's bone position */
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;                  /* the end point is the centroid of the target actors */
    short endY;
    short endZ;
} BattleFxSandstormRecord;

/* Suna particle timeline slot: jittered start position, per-axis step and a random variant. */
typedef struct BattleFxSandstormParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short stepX;
    short angle;
    short stepZ;
    unsigned char pad_12[0x2];
    short rotationY;
    short variant;
    short scaleY;
    short growth;
} BattleFxSandstormParticle;

/* Suna variant sprite record; its counter walks the sprite data and the variant picks the table. */
typedef struct BattleFxSandstormVariantSprite {
    short ownerSlot;
    short frameIndex;
    short posX;                  /* the target's bone position, jittered */
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variantIndex;
} BattleFxSandstormVariantSprite;

/* The battle task slot of BattleFx_SpecialSandstormSpinGrowModelTask. */
typedef struct BattleFxSandstormSpinGrowModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinAngleX;
    short spinAngleY;
    short spinAngleZ;
    unsigned char pad_12[0x6];
    short scaleY;
    short growth;
} BattleFxSandstormSpinGrowModelSlot;

/* What BattleFx_SpecialSandstormDrawParticleField reads through g_BattleSpecialSandstormDrawListHandle. */
typedef struct BattleFxSandstormDotBatchHead {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char primCode;
} BattleFxSandstormDotBatchHead;

/* One of the 0xC8 sand particle records at g_BattleSpecialSandstormParticleRecords (8 bytes). */
typedef struct BattleSpecialSandstormParticleRecord {
    short active; /*+0x00 */
    short radius; /*+0x02 */
    short angle; /*+0x04 */
    short age; /*+0x06 */
} BattleSpecialSandstormParticleRecord;

#pragma pack(push, 1)
typedef struct BattleSpecialSandstormConfig {
    unsigned char pad_0[0x10];
    int modelHandle; /* +0x10 */
    unsigned char pad_14[0x6];
    short sunaConfigStepShift26; /* +0x1a */
} BattleSpecialSandstormConfig;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSandstormSpriteData {
    int animTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short sunaSpriteFrameWord; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialSandstormSpriteData;
#pragma pack(pop)

/* --- battle_fx_special_syaku --- */
#pragma pack(push, 1)
typedef struct BattleSpecialScorchParamVector1 {
    int loadedModel; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialScorchParamVector1;
#pragma pack(pop)

/* --- battle_fx_special_tan_spit --- */
/* Tan timeline slot: frame timer, base position with per-axis sizes, and the record type allocated. */
typedef struct BattleFxRefuseSpitTimeline {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short sizeX;
    short sizeY;
    short sizeZ;
    unsigned char pad_12[0x2];
    short scaleIndex;
    unsigned char pad_16[0x2];
    short recordType;
} BattleFxRefuseSpitTimeline;

/* Per-target Tan trajectory: start position and per-frame velocity, computed from the target points. */
typedef struct BattleFxRefuseSpitPath {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short targetIndex;
} BattleFxRefuseSpitPath;

/* The header of a sprite group: its frame count. */
typedef struct BattleTanSpriteGroupHeader {
    unsigned char pad_00[0x4];
    int frameCount;
} BattleTanSpriteGroupHeader;

/* The sprite group a sprite request's spriteGroup points at, as far as the impact sprite reads it. */
typedef struct BattleRefuseSpitSpriteGroup {
    BattleTanSpriteGroupHeader *header;
} BattleRefuseSpitSpriteGroup;

/* The impact sprite's draw request, then the droplet request's sprite group and draw object; the
 * droplet request's colour and frame are the eight bytes after this object. */
typedef struct BattleSpecialRefuseSpitSpriteData {
    BattleFxSpriteRequest impact;
    int dropletGroup;   /* +0x10 */
    int dropletDrawObj; /* +0x14 */
} BattleSpecialRefuseSpitSpriteData;

/* --- battle_fx_special_tane_seed --- */
/* The battle task slot of BattleFx_SpecialSeedShotBounceModelTask. */
typedef struct BattleFxSeedShotBounceModelSlot {
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
    short modelVariant;
    unsigned char pad_16[0x2];
    short angleA;
    short angleB;
} BattleFxSeedShotBounceModelSlot;

/* The battle task slot of BattleFx_SpecialSeedShotTimelineTask. */
typedef struct BattleFxSeedShotTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    short startZ;
    short startY;
    unsigned char pad_0A[0x2];
    short endX;
    short endZ;
    short endY;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxSeedShotTimelineSlot;

/* Slots BattleFx_SpecialSeedShotTimelineTask allocates for BattleFx_SpecialTane{BounceModel, ScatteredSprite}Task. */
typedef struct BattleFxSeedShotTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short modelVariant;
    short pushLength;
    short rotY;
    short rotX;
} BattleFxSeedShotTimelineSpawnSlot;

/* Effect task record for the Tane scattered sprite effect. */
typedef struct BattleFxSeedShotScatterSpriteTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xC];
    short pushLength;
} BattleFxSeedShotScatterSpriteTask;

#pragma pack(push, 1)
typedef struct BattleSpecialSeedShotModelConfig {
    unsigned char pad_0[0x10];
    int firstModel; /* +0x10 */
    unsigned char pad_14[0x6];
    short scale; /* +0x1a */
    unsigned char pad_1c[0x2];
    short flags; /* +0x1e */
    int secondModel; /* +0x20 */
} BattleSpecialSeedShotModelConfig;
#pragma pack(pop)

/* --- battle_fx_special_tele_effect --- */
/* Tele beam segment slot: base position, orbit offsets and lifetime. */
typedef struct BattleFxTailLaserBeamSegment {
    unsigned char pad_00[0x4];
    short posX;
    short height;
    short posY;
    unsigned char pad_0A[0x2];
    short offsetX;
    short driftSpeed;
    short offsetY;
    unsigned char pad_12[0x4];
    short lifetime;
    unsigned char pad_18[0x4];
    short actorId;
} BattleFxTailLaserBeamSegment;

/* Tele beam record: frame index, accumulated position with velocity, and a jittered base value. */
typedef struct BattleFxTailLaserBeamRecord {
    short ownerSlot;
    short index;
    short position;
    short clearedWordA;
    short base;
    unsigned char pad_0A[0x2];
    short velocity;
    short clearedWordB;
    short clearedWordC;
} BattleFxTailLaserBeamRecord;

/* Tele pulsing model task slot: pulse step counter. */
typedef struct BattleFxTailLaserPulseSlot {
    short ownerSlot;
    short pulseStep;
    short posX;
    short posY;
    short posZ;
} BattleFxTailLaserPulseSlot;

/* Tele timeline record that spawns the beam: the targets' centroid and an age that runs for 45 frames. */
typedef struct BattleFxTailLaserTimeline {
    short ownerSlot;
    short age;
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_0A[0x14];
    short sourceActor;
} BattleFxTailLaserTimeline;

/* The battle task slot of BattleFx_SpecialTailLaserTargetTimelineTask. */
typedef struct BattleFxTailLaserTargetTimelineSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x8];
    short originX;
    short originY;
    short originZ;
    unsigned char pad_12[0xa];
    short actorId;
} BattleFxTailLaserTargetTimelineSlot;

/* The battle task slot of BattleFx_SpecialTailLaserBeamSegmentTask. */
typedef struct BattleFxTailLaserBeamSegmentSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x8];
    short posX;
    short posY;
    short posZ;
} BattleFxTailLaserBeamSegmentSlot;

#pragma pack(push, 1)
typedef struct BattleSpecTailLaserScaleVector {
    unsigned char pad_0[0x4];
    int y; /* +0x4 */
    unsigned char pad_8[0x8];
    int matrix; /* +0x10 */
    unsigned char pad_14[0x2];
} BattleSpecTailLaserScaleVector;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecTailLaserSpriteReq5 {
    int resField; /* +0x0 */
    unsigned char pad_4[0x6];
    short req5Word10; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecTailLaserSpriteReq5;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecTailLaserSpriteReq6 {
    int modelField; /* +0x0 */
    unsigned char pad_4[0x6];
    short req6PulseWord; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecTailLaserSpriteReq6;
#pragma pack(pop)

/* --- battle_fx_special_timedmg_warp --- */
typedef struct PacketTagProbe {
    unsigned long link : 24;
    unsigned long length : 8;
} PacketTagProbe;

/* A point of the warp grid mesh: the screen offset of one grid corner. */
typedef struct BattleFxWarpMeshPoint {
    short x;
    short y;
} BattleFxWarpMeshPoint;

/* One flat-textured quad packet of a warp grid cell (0x28 bytes, a PSX POLY_FT4): the
 * command word, the vertex colour and primitive code, then four corners of screen position
 * (x, y) and texture position (u, v). */
typedef struct BattleFxWarpQuad {
    int command;
    char colorR;
    char colorG;
    char colorB;
    char primCode;
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
    short pad_1e;
    short x3;
    short y3;
    char u3;
    char v3;
    short pad_26;
} BattleFxWarpQuad;

/* One flat triangle packet (0x14 bytes, a PSX POLY_F3) of the warp grid's ordering-table buffer:
 * tag (link and length), colour and primitive code, then three screen positions. */
typedef struct BattleFxWarpTriangle {
    PacketTagProbe tag;
    char colorR;
    char colorG;
    char colorB;
    char primCode;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
} BattleFxWarpTriangle;

/* A row of 16 triangle packets. */
typedef BattleFxWarpTriangle BattleFxWarpTriangleRow[16];

/* One cell of the Timedmg warp grid (0x60 bytes): four corner pointers into the grid mesh,
 * then room for two quad packets (only the first is used). */
typedef struct BattleFxTimeDamageGridCell {
    BattleFxWarpMeshPoint *corner00;
    BattleFxWarpMeshPoint *corner01;
    BattleFxWarpMeshPoint *corner10;
    BattleFxWarpMeshPoint *corner11;
    BattleFxWarpQuad quad[2];
} BattleFxTimeDamageGridCell;

/* The vertex colour and primitive code of a Timedmg sprite request. */
typedef struct BattleFxTimeDamageColor {
    char red;
    char green;
    char blue;
    unsigned char primCode;
} BattleFxTimeDamageColor;

/* Orbiting sprite ring record of the timed damage effect: frame counter and three accumulated angles. */
typedef struct BattleFxOrbitRingSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x8];
    short angleX;
    short angleY;
    short angleZ;
} BattleFxOrbitRingSprite;

/* Timedmg timeline task: while it runs the owner word holds the index of the target actor (it
 * becomes BATTLE_TASK_FREE when the task ends); age counts the timeline stages; pos is the
 * target's bone position. */
typedef struct BattleFxTimeDamageTimeline {
    short ownerSlot;
    short age;
    SVECTOR pos;
} BattleFxTimeDamageTimeline;

/* The task that projects the target's bone position to the screen every frame. */
typedef struct BattleFxTimeDamageProjectTask {
    short ownerSlot;
    short toggle;
    SVECTOR pos;
} BattleFxTimeDamageProjectTask;

/* Task-slot record for the Timedmg timeline, allocated by one of three spawned sprite and model tasks. */
typedef struct BattleFxTimeDamageTimelineRecord {
    short ownerSlot;
    unsigned char pad_02[0x2];
    SVECTOR pos; /* +0x4: copied from the timeline task */
    short angleX;
    short angleY;
    short angleZ;
} BattleFxTimeDamageTimelineRecord;

/* Timed-damage transform record read by the rotating sprite pair task. */
typedef struct BattleSpecTimeDamageTransform {
    unsigned char pad_00[0x18];
    int spriteRecord;
    int spriteObject;
} BattleSpecTimeDamageTransform;

/* Timedmg transform matrix record, filled from table fields. */
#pragma pack(push, 1)
typedef struct BattleFxTimeDamageTransform {
    unsigned char pad_00[0x12];
    int geomScreenX4; /* +0x12: Timedmg's BuildWarpGridOffsets stores GetGeomScreen() << 2 here, in the translation of the matrix at g_BattleSpecTimeDamageScale */
    unsigned char pad_16[0x2];
    struct BattleFxSpriteGroup *spriteGroup;
    int drawObj;
} BattleFxTimeDamageTransform;
#pragma pack(pop)

/* --- battle_fx_special_toboe_second --- */
/* The battle task slot of BattleFx_SpecialWarCryTimelineTask. */
typedef struct BattleFxWarCryTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0x6];
    short targetIndex;
} BattleFxWarCryTimelineSlot;

#pragma pack(push, 1)
/* The drifting sprites' colour and primitive code; its last word starts the growing model's draw request. */
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char primCode; /* +0x3 */
    unsigned char pad_4[0x4];
    int modelGroup; /* +0x8 */
} BattleSpecWarCrySpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
/* The growing model's rotation angles, then the head of the expanding model's draw request. */
typedef struct {
    SVECTOR angles; /* +0x0 */
    int modelGroup; /* +0x8 */
    int drawFlags; /* +0xc */
    short uvScroll; /* +0x10 */
    short fadeLevel; /* +0x12 */
} BattleSpecWarCrySpriteReq2;
#pragma pack(pop)

/* --- battle_fx_special_triangle_electric --- */
/* Triangle special timeline track: previous and per-step positions and a random variant. */
typedef struct BattleFxTriangleAttackTrack {
    unsigned char pad_00[0x4];
    short prevX;
    short prevY;
    short prevZ;
    unsigned char pad_0A[0x2];
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_12[0x2];
    short variant;
    short spinPhase;
    short scale;
} BattleFxTriangleAttackTrack;

/* Triangle special track record: caster-bone origin, target position, frame counter and bone index. */
typedef struct BattleFxTriangleAttackRecord {
    short ownerSlot;
    short frame;
    short originX;
    short originY;
    short originZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxTriangleAttackRecord;

/* Triangle pulsing sprite: pulse phase and a position copied into the spawned sprite. */
typedef struct BattleFxTriangleAttackPulseSprite {
    short ownerSlot;
    short pulsePhase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_12[0x2];
    short modelIndex;
} BattleFxTriangleAttackPulseSprite;

/* Task-slot record for the Triangle variant sprite: frame counter and model variant. */
typedef struct BattleFxTriangleAttackSpriteTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short variant;
} BattleFxTriangleAttackSpriteTask;

/* The battle task slot of BattleFx_SpecialTriangleAttackSpinModelsTask. */
typedef struct BattleFxTriangleAttackSpinModelsSlot {
    short ownerSlot;
    short frameCounter;
    short spinX;
    short spinY;
    short spinZ;
    unsigned char pad_0A[0x2];
    short spinVelocityX;
    short spinVelocityY;
    short spinVelocityZ;
    unsigned char pad_12[0x4];
    short spinPhase;
    short timeLeft;
} BattleFxTriangleAttackSpinModelsSlot;

/* The battle task slot of BattleFx_SpecialTriangleAttackRotatedSpriteTask. */
typedef struct BattleFxTriangleAttackRotatedSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_12[0x2];
    short modelIndex;
} BattleFxTriangleAttackRotatedSpriteSlot;

/* --- battle_fx_special_tunami --- */
/* Tunami timeline task slot: jittered position per axis and an on/off flag. */
typedef struct BattleFxBigWaveSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x10];
    short active;
} BattleFxBigWaveSlot;

/* PSX textured quad packet for one tunami wave surface: GPU tag, command word, texture coords. */
#pragma pack(push, 1)
typedef struct BattleFxBigWaveWavePacket {
    int tag;
    union {
        int colorCommand;
        CVECTOR color0;
    };
    unsigned char pad_08[0x4];
    unsigned char u0;
    unsigned char v0;
    short waveAttr;
    unsigned char pad_10[0x4];
    unsigned char u1;
    unsigned char v1;
    short waveBias;
    unsigned char pad_18[0x4];
    unsigned char u2;
    unsigned char v2;
    unsigned char pad_1E[0x6];
    unsigned char u3;
    unsigned char v3;
    unsigned char pad_26[0x2];
} BattleFxBigWaveWavePacket;
#pragma pack(pop)

/* --- battle_fx_special_turara --- */
/* Turara record: base position and frame counter for the Turara effect. */
typedef struct BattleFxIcicleDropRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    short baseX;
    unsigned char pad_0E[0x2];
    short baseZ;
    unsigned char pad_12[0x8];
    short actorIndex;
} BattleFxIcicleDropRecord;

/* Turara expanding model task slot: growth step counter. */
typedef struct BattleFxIcicleDropGrowSlot {
    short ownerSlot;
    short growStep;
    short coordX;
    short coordY;
    short coordZ;
} BattleFxIcicleDropGrowSlot;

/* The battle task slot of BattleFx_SpecialIcicleDropFallingModelTask. */
typedef struct BattleFxIcicleDropFallingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    short modelIndex;
    short angleY;
    short spinAngle;
} BattleFxIcicleDropFallingModelSlot;

/* The battle task slot of BattleFx_SpecialIcicleDropFallingRotatingSpriteTask. */
typedef struct BattleFxIcicleDropFallingRotatingSpriteSlot {
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
    short modelIndex;
} BattleFxIcicleDropFallingRotatingSpriteSlot;

/* The resource definition rows, then the falling sprites' two handles; the rest of that sprite
 * request is g_BattleSpecIcicleDropSpriteReq1's colour. */
typedef struct {
    int rows[34];
    struct BattleFxSpriteGroup *spriteGroup; /* +0x88 */
    int drawObj; /* +0x8c */
} BattleSpecIcicleDropResourceDefRows;

/* The falling models' scale vector, their draw request head and model ids, then the head of the
 * expanding model's draw request, whose state flags are the word after this object. */
typedef struct {
    VECTOR scale; /* +0x0 */
    BattleFxModelDrawHead modelDraw; /* +0x10 */
    int modelIds[4]; /* +0x20 */
    int expandModelGroup; /* +0x30 */
    int expandDrawFlags; /* +0x34 */
    short expandUvScroll; /* +0x38 */
    short expandFadeLevel; /* +0x3a */
} BattleSpecIcicleDropSpriteReq3;

#pragma pack(push, 1)
typedef struct BattleSpecIcicleDropTransformCoord3 {
    short scale; /* +0x0 */
    int x; /* +0x2 */
    int y; /* +0x6 */
    int z; /* +0xa */
} BattleSpecIcicleDropTransformCoord3;
#pragma pack(pop)

/* --- battle_fx_special_tyokyu --- */
/* Tyokyu timeline record: step, position and model index. */
typedef struct BattleFxSuperOrbRecord {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0xc];
    short modelIndex;
} BattleFxSuperOrbRecord;

/* Tyokyu orbit effect slot: orbit offsets and radius, with the source's start offsets copied in. */
typedef struct BattleFxSuperOrbOrbitRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short offsetX;
    short offsetY;
    short radius;
} BattleFxSuperOrbOrbitRecord;

/* --- battle_fx_special_utagoe_effect --- */
/* Per-slot fading sprite state: position with velocity, wobble angle, amplitude and billboard rotation. */
typedef struct BattleFxSirenSongFadeSprite {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    short prevOffset;
    short velocityX;
    short velocityY;
    short velocityZ;
    short angularVelocity;
    short rotation;
    short rotationStep;
    short amplitude;
    short angle;
} BattleFxSirenSongFadeSprite;

/* --- battle_fx_special_uzumaki_whirlpool --- */
/* Scale history of the growing Uzumaki model. */
typedef struct BattleFxWhirlpoolScaleState {
    int scaleFrom;
    int scale;
    int scaleTo;
} BattleFxWhirlpoolScaleState;

/* Uzumaki timeline task slot: timeline step counter and the allocated record type. */
typedef struct BattleFxWhirlpoolTimelineSlot {
    short ownerSlot;
    short timelineStep;
    short recordType;
} BattleFxWhirlpoolTimelineSlot;

/* Effect task record for the Uzumaki growing-model effect. */
typedef struct BattleFxWhirlpoolGrowingTask {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x4];
    short posX;
    unsigned char pad_0A[0x2];
    short posZ;
} BattleFxWhirlpoolGrowingTask;

/* Timeline record for the Uzumaki special effect, one per target. */
typedef struct BattleFxWhirlpoolTimeline {
    unsigned char pad_00[0x4];
    short actorIndex;
    short secondIndex;
} BattleFxWhirlpoolTimeline;

#pragma pack(push, 1)
typedef struct BattleSpecWhirlpoolSecondaryRecords {
    int tableCursor; /* +0x0 */
    int counter; /* +0x4 */
    char rotByte; /* +0x8 */
    unsigned char pad_9[0x1];
    short angle; /* +0xa */
} BattleSpecWhirlpoolSecondaryRecords;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecWhirlpoolColor {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
} BattleSpecWhirlpoolColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecWhirlpoolParticleConfig {
    int resField; /* +0x0 */
    unsigned char pad_4[0x4];
    int modelField; /* +0x8 */
    unsigned char pad_c[0x6];
    short scale; /* +0x12 */
    unsigned char pad_14[0x4];
} BattleSpecWhirlpoolParticleConfig;
#pragma pack(pop)

/* --- battle_fx_special_wlaser --- */
/* A slot the Wlaser timeline fills: a beam slot (posX..posZ the start, endX..endZ the end) or an
 * animated sprite slot (posX..posZ its position, stage its variant). */
typedef struct BattleFxDoubleLaserBeamRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0x4];
    short stage;
} BattleFxDoubleLaserBeamRecord;

/* The battle task slot of BattleFx_SpecialDoubleLaserDrawBeamSpriteTask. */
typedef struct BattleFxDoubleLaserDrawBeamSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short beamStartX;
    short beamStartY;
    short beamStartZ;
    unsigned char pad_0A[0x2];
    short beamEndX;
    short beamEndY;
    short beamEndZ;
} BattleFxDoubleLaserDrawBeamSpriteSlot;

/* Timeline record for the Wlaser special effect, one per target. */
typedef struct BattleFxDoubleLaserTimeline {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x8];
    short targetBonePos[3]; /* world position of the hit actor's targetBone (Battle_GetBoneWorldPos); the beams end here */
    unsigned char pad_12[0xa];
    short hitActor;    /* the actor the beams hit */
    short casterActor; /* the actor whose primaryBone and secondaryBone the beams start from */
} BattleFxDoubleLaserTimeline;

/* g_BattleSpecDoubleLaserSpriteReq2: the model table index of each animated sprite variant, then the beam
 * sprite's request. */
typedef struct {
    int variantTableIndex[3]; /* indexed by the animated sprite's variant */
    int unused0c;
    BattleFxSpriteRequest beamRequest; /* drawn by BattleFx_SpecialDoubleLaserDrawBeamSpriteTask */
} BattleSpecDoubleLaserSpriteReq2;

/* --- battle_fx_special_yami_effect --- */
/* Yami special projectile record in the battle task slot buffer: position, velocity, angle and timer. */
typedef struct BattleFxDarkWaveProjectile {
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
    short angleY;
    unsigned char pad_16[0x4];
    short timer;
} BattleFxDarkWaveProjectile;

/* Slots BattleFx_SpecialDarkWaveTimelineTask allocates for BattleFx_SpecialYami{Projectile, RadialBurstSprite, 1 more}Task. */
typedef struct BattleFxDarkWaveTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angleY;
    unsigned char pad_16[0x2];
    short unusedAngle;
    short verticalDrift;
} BattleFxDarkWaveTimelineSpawnSlot;

/* --- battle_fx_special_yuwaku_projectile --- */
/* Task-slot record for the Yuwaku sine sprite: frame counter, sine offset, sprite index and phase. */
typedef struct BattleFxFascinationSineTask {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x2];
    short sineOffset;
    unsigned char pad_08[0xe];
    short spriteIndex;
    unsigned char pad_18[0x2];
    short phase;
} BattleFxFascinationSineTask;

/* Per-slot state of the Yuwaku rising variant sprite task: frame counter, angle and request index. */
typedef struct BattleFxFascinationRisingSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x2];
    short angle;
    unsigned char pad_08[0xe];
    short requestIndex;
} BattleFxFascinationRisingSprite;

/* The battle task slot of BattleFx_SpecialFascinationTask. */
typedef struct BattleFxFascinationSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
    unsigned char pad_12[0x4];
    short angle;
    short radius;
    short phase;
} BattleFxFascinationSlot;

/* Slots BattleFx_SpecialFascinationTimelineTask allocates for BattleFx_SpecialYuwaku{DampedSprite, Projectile}Task. */
typedef struct BattleFxFascinationTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angleY;
    short spinAngleY;
    short scale;
    short swayPhase;
} BattleFxFascinationTimelineSpawnSlot;

/* Slots BattleFx_SpecialFascinationTask allocates for BattleFx_SpecialYuwaku{RisingVariantSprite, SineSprite}Task. */
typedef struct BattleFxFascinationSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variant;
    unsigned char pad_18[0x2];
    short sinePhase;
} BattleFxFascinationSpawnSlot;

/* --- battle_fx_special_zenbaku_effect --- */
/* Setup record for the Zenbaku special effect and its timeline task. */
typedef struct BattleFxFullExplosionSlot {
    short ownerSlot;
    unsigned char pad_02[0x4];
    short launchSpeed;
    unsigned char pad_08[0x4];
    short radius;
    short diameter;
    short velocity;
    short scaledVelocity;
    short spriteScale;
} BattleFxFullExplosionSlot;

/* Per-slot state for the Zenbaku explosion timeline: base position, random spread, lift and step counter. */
typedef struct BattleFxFullExplosionExplosionState {
    short ownerSlot;
    short step;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x2];
    short jitterOffset;
    short jitterRange;
    short liftOffset;
    short liftRange;
    short timer;
} BattleFxFullExplosionExplosionState;

/* Explosion coordinate record for the Zenbaku timeline, one entry per burst point. */
typedef struct BattleFxZenbakuExplosionCoord {
    short posZ;
    short scaledOffset;
    unsigned char pad_04[0x4];
} BattleFxZenbakuExplosionCoord;

#endif /* FF7_TYPES_BATTLE_SPECIAL_H */

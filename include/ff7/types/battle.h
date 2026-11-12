/* include/ff7/types/battle.h: the types ff7/battle TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_BATTLE_H
#define FF7_TYPES_BATTLE_H
#include "lib/codegen.h"
#include "lib/types/graphics.h"

#ifdef __cplusplus
struct GzipHuffmanNode {
    unsigned char e, b;
    union {
        unsigned short n;
        GzipHuffmanNode *t;
    } v;
};

typedef struct GzipHuffmanNode GzipHuffmanNode;
#endif

typedef struct BattleSchedulePair {
    int a, b;
} BattleSchedulePair;

typedef void(__cdecl *BattleActionCallback)(void);

typedef struct ShadowInt64Pair {
    int lo;
    int hi;
} ShadowInt64Pair;

/* Sixteen-byte four-component vector block copied by battle effect tasks. */
typedef struct BattleVec4i {
    int x;
    int y;
    int z;
    int w;
} BattleVec4i;

/* 0x74-byte (116-byte) per-actor model placement and animation draw state block. */
typedef struct BattleModelDrawState {
    int w[29];
} BattleModelDrawState;

/* 756-byte mesh working/source buffer copied at the start of the Nadare special effect. */
typedef struct BattleAvalancheMeshData {
    int w[189];
} BattleAvalancheMeshData;

typedef struct BattleFxResourceSlot12 {
    unsigned char pad[0xc];
    int marker;
} BattleFxResourceSlot12;

/* SVECTOR without its pad: three shorts, 6 bytes. */
typedef struct Vec3sPacked {
    short vx;
    short vy;
    short vz;
} Vec3sPacked;

#pragma pack(push, 2)
typedef struct MatrixParticleRecord {
    short active;
    short frame;
    short x;
    short y;
    short z;
    char pad_0a[10];
    struct PsxMatrix *matrix;
} MatrixParticleRecord;
#pragma pack(pop)

/* Battle effect model draw record: flags and fade level, read by BattleFx_SubmitModelDraw. */
typedef struct BattleFxModelDraw {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
    int partHandle;
    int tintLevel;
    unsigned char pad_18[0x2];
    short blendFraction;
    unsigned char pad_1C[0x14];
    int auxHandle;
    int resourceHandle;
    unsigned char pad_38[0x2];
    short prevExtendAmount;
    unsigned char pad_3C[0x4];
    int extraHandle;
    unsigned char pad_44[0x6];
    short extendAmount;
} BattleFxModelDraw;

/* The first 16 bytes of a BattleFxModelDraw, which is all the effects that fill a request in
 * place initialize; BattleFx_SubmitModelDraw reads the rest as is. */
typedef struct BattleFxModelDrawHead {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} BattleFxModelDrawHead;

/* Battle actor animation script state: script pointer, opcode, running flag, target actor. */
typedef struct BattleActorAnimState {
    char running;
    char opcode;
    unsigned char pad_02[0x2];
    char *scriptPtr;
    short angle;
    short secondarySlot;
    short posY;
    short targetActor;
    short divisor;
    short distance;
    char taskSlot;
    char scriptArg;
    short targetX;
    short targetY;
    short targetZ;
} BattleActorAnimState;

/* Summon knight actor record: flags, position, rotation, scale, state and model index. */
typedef struct BattleKotrKnight {
    short flags;
    unsigned char pad_02[0x20];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_28[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_30[0x8];
    short composedPosX;
    short composedPosY;
    short composedPosZ;
    unsigned char pad_3E[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_46[0x2];
    int clearedWord;
    unsigned char pad_4C[0x2];
    short scale;
    short state;
    short fadeLevel;
    int tintColor;
    short attachIndex;
    unsigned char pad_5A[0x22];
    int animFrame;
    int knightIndex;
} BattleKotrKnight;

/* Hades summon ribbon task record: segment offsets, bases and vertices. */
typedef struct BattleFxHadesRibbon {
    unsigned char pad_00[0x1c];
    int depthA;
    int depthB;
    unsigned char pad_24[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_2E[0x2];
    short halfWidth;
    short vertexOffsetY;
    short vertexOffsetZ;
    unsigned char pad_36[0x2];
    union {
        struct { short baseX; short baseY; short baseZ; unsigned char pad_3E_low[0x2]; };
        Int64Pair basePair;
    };
    unsigned char pad_40[0x8];
    union {
        struct { short vertexX; short vertexY; short vertexZ; unsigned char pad_4E[0x2]; };
        SVECTOR vertexVec;
        Int64Pair vertexPair;
    };
    union {
        struct { short sideX; short sideY; short sideZ; unsigned char pad_56_low[0x2]; };
        SVECTOR sideVec;
    };
    unsigned char pad_58[0x10];
    int vertex0;
    int vertex1;
    int vertexThird;
    unsigned char pad_74[0x14];
    int vertex2;
    int vertex3;
    unsigned char pad_90[0x18];
    union { int drawColor; CVECTOR drawColorVec; };
    union { int colorTwo; CVECTOR colorTwoVec; };
    union { int colorOne; CVECTOR colorOneVec; };
    int colorRemaining;
    union { int endDrawColor; CVECTOR endDrawColorVec; };
    union { int endColorTwo; CVECTOR endColorTwoVec; };
    union { int endColor; CVECTOR endColorVec; };
    int endColorAlt;
    short angleX;
    short angle;
    short angleZ;
    unsigned char pad_CE[0x2];
    unsigned char rotMatrix[0x1e];
    unsigned char pad_EE[0x4e];
    int blendA;
    int blendA2;
    int blendFactorA;
    unsigned char pad_148[0x4];
    int blendB;
    int blendB2;
    int blendFactorB;
    unsigned char pad_158[0x4];
    int blendAInit;
    int blendA2Init;
    unsigned char pad_164[0x8];
    int blendBInit;
    int blendB2Init;
} BattleFxHadesRibbon;

/* Four corner points (x,y,z shorts, 8-byte stride) and a centre, used by shatter and strip effects. */
typedef struct BattleFxShardQuad {
    short corner0X;
    short corner0Y;
    short depth;
    unsigned char pad_06[0x2];
    short corner1X;
    short corner1Y;
    short stripStart;
    unsigned char pad_0E[0x2];
    short corner2X;
    short corner2Y;
    short stripLevel;
    unsigned char pad_16[0x2];
    short corner3X;
    short corner3Y;
    short stripEnd;
    unsigned char pad_1E[0x2];
    short centerX;
    short centerY;
    int flags;
} BattleFxShardQuad;

/* Three-component fixed-point vector (x,y,z ints) used by the battle summon and force-field effects. */
typedef struct BattleFxVector3 {
    int x;
    int y;
    int z;
    unsigned char pad_0C[0x4];
    int limitSparkVectorAt16;
    int drawObj;
    char colorR;
    char prevIndex;
    char currentIndex;
    unsigned char pad_1B[0x1];
    short frame;
    unsigned char pad_1E[0x2];
    int spriteGroup2;
    int drawObj2;
    unsigned char pad_28[0x4];
    short frameWord;
    short texRowPacked;
} BattleFxVector3;

/* Wave strip task data for the Limit H moon effect: the quad of the current strip segment (corners
 * 0 and 1 on the current row, 2 and 3 on the next), its depth, wave value, phase, amplitude and
 * enable flag. */
typedef struct BattleFxMoonWaveStrips {
    unsigned char pad_00[0x4];
    int unusedOut; /* Gfx_TransformQuadReturnDepth's unused output */
    int depth;
    int flags;
    SVECTOR quad[4];
    int waveValue;
    int stepOffset;
    int phase;
    int amplitude;
    int enabled;
} BattleFxMoonWaveStrips;

/* Camera script runner state: script read position, started flag and rotation matrix pointer. */
typedef struct BattleFxCameraScript {
    short ownerSlot;
    short started;
    unsigned char pad_04[0x8];
    int scriptPos;
    int rotMatrix;
    short actor;
} BattleFxCameraScript;

/* Scratch record: centroid of a PSX triangle and each vertex's offset from it. */
typedef struct YasuiBoulderThrowTriOffsets {
    short rel0X;
    short rel0Y;
    short rel0Z;
    unsigned char pad_06[0x2];
    short rel1X;
    short rel1Y;
    short rel1Z;
    unsigned char pad_0E[0x2];
    short rel2X;
    short rel2Y;
    short rel2Z;
    unsigned char pad_16[0xa];
    short centerX;
    short centerY;
    short centerZ;
} YasuiBoulderThrowTriOffsets;

/* Effect-100 slot that runs the S-Nova camera script: script cursor and matrix pointer. */
typedef struct BattleFxSuperNovaCameraScriptState {
    short ownerSlot;
    short initialized;
    unsigned char pad_04[0x8];
    union {
        int scriptCursor;
        short *scriptPtr;
    };
    union {
        int matrixPtr;
        void *matrix;
    };
    short attachPoint;
} BattleFxSuperNovaCameraScriptState;

/* One 12-byte step in the battle sequence buffer: actor, animation and hit-record index. */
typedef struct BattleSeqStep {
    char actorIndex;
    char actionType;
    char effectId;
    char stepFlags;
    char clearedFlag;
    char animId;
    short power;
    short targetIndex;
    short hitRecordIndex;
} BattleSeqStep;

/* Scratch vectors for the cross product of two fragment edges, used to derive velocities from normals. */
typedef struct BattleFxFragmentNormalScratch {
    int edgeAX;
    int edgeAY;
    int edgeAZ;
    unsigned char pad_0C[0x4];
    int edgeBX;
    int edgeBY;
    int edgeBZ;
    unsigned char pad_1C[0x4];
    int crossX;
    int crossY;
    int crossZ;
} BattleFxFragmentNormalScratch;

/* Battle effect task slot for the Ramuh camera script: script cursor and matrix. */
typedef struct BattleFxTimelineScriptTask {
    short ownerSlot;
    short initialized;
    unsigned char pad_04[0x8];
    union {
        int scriptCursor;
        short *script; /* the camera script's read position */
    };
    int matrix;
    short scriptWord;
} BattleFxTimelineScriptTask;


/* Comet debris particle: position, velocity, bounce step and colour fade per frame. */
typedef struct BattleCometDebris {
    short state;
    short step;
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
    unsigned char rotStepX;
    unsigned char rotStepY;
    unsigned char rotStepZ;
    unsigned char pad_1B[0x1];
    short scale;
    short modelVariant;
} BattleCometDebris;


/* BattleFx Thunder 4 bounce slot (0x20 stride): bounce position, velocity and frame count. */
typedef struct BattleFxBolt3MultiBounceTask {
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
    unsigned char angleX;
    unsigned char angleY;
    unsigned char angleZ;
    unsigned char pad_17[0x1];
    unsigned char angleStepX;
    unsigned char angleStepY;
    unsigned char angleStepZ;
    unsigned char pad_1B[0x1];
    short scale;
} BattleFxBolt3MultiBounceTask;

/* One 12-byte record of g_BattleEffectOrderTable (128 entries), which Battle_AllocHitRecord
   hands out: the actor hit and the actor hitting (Battle_ResolveHitOnTarget), the animation (pose 9
   when a cover takes the hit; the hit display copies it to the actor's state), the index of its
   damage display record (0xFF none; Battle_AllocDamageDisplayRecord), flags (bit 4 holds the
   animation, Battle_ClearActorAnimHoldFlags) and the target's status. A target of -1 ends a step's
   records (Battle_PushEmptyHitRecord). */
typedef struct BattleHitRecord {
    char targetActor;
    char sourceActor;
    char animId;
    char damageDisplayIndex;
    short flags;
    unsigned char pad_06[0x2];
    int status;
} BattleHitRecord;

/* Hades ribbon record: position-history indices, step counter, lifetime and step limit. */
typedef struct BattleFxHadesPositionHistory {
    int headIndex;
    int step;
    int timer;
    int stepLimit;
    int prevHeadIndex;
    struct Int64Pair *positions[64];
} BattleFxHadesPositionHistory;

/* Limit spark effect sprite record: phase counter and two linked values copied out per frame. */
typedef struct BattleFxLimitLivewireTimelineSlot {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short spreadX;
    short spreadY;
} BattleFxLimitLivewireTimelineSlot;

/* Magic Comet descending model: frame count and position stepped by velocity each frame. */
typedef struct BattleFxMeteorModelSlot {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxMeteorModelSlot;

/* Draw arguments for the Kjata summon mesh renderers: mesh data, flags, shading and polygon attributes. */
typedef struct BattleFxKjataDraw {
    int meshData;
    int flags;
    unsigned char pad_08[0x2];
    short vertexShade;
    short polyFlags;
} BattleFxKjataDraw;

/* Titan textured fragment record: four vertex pointers, averaged to find the fragment centre. */
typedef struct BattleFxShardTriVerts {
    int vertex0;
    int vertex1;
    int vertex2;
    int vertex3;
} BattleFxShardTriVerts;

/* Four words of a projected trail vertex slot (a 16-byte record copied as a unit). */
typedef struct BattleFxTrailVertex {
    int w[4];
} BattleFxTrailVertex;

/* Battle effect trail state in data block 0: current and previous trail section values. */
typedef struct BattleFxTrailState {
    unsigned char pad_00[0x24];
    int currentSection;
    int previousSection;
    unsigned char pad_2C[0x8];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_3A[0x3a];
    int posX;
    int posY;
    int posZ;
    unsigned char pad_80[0x14];
    int prevX;
    int prevY;
    int prevZ;
    unsigned char pad_A0[0x14];
    int headBlendColor;
    int headSourceColor;
    int headColor;
    int prevHeadColor;
    int tailBlendColor;
    int tailSourceColor;
    int tailColor;
    int prevTailColor;
    unsigned char pad_D4[0x74];
    BattleFxTrailVertex vertexA;
    BattleFxTrailVertex vertexB;
    BattleFxTrailVertex vertexC;
    unsigned char pad_178[0x50];
    BattleFxTrailVertex prevVertexA;
    BattleFxTrailVertex prevVertexB;
    BattleFxTrailVertex prevVertexC;
} BattleFxTrailState;

/* Tifa limit ribbon instance (0xD8 bytes): sample ring with head index, count and lifetime. */
typedef struct BattleLimitTifaRibbon {
    int sampleSlot;
    int head;
    int lifetime;
    int sampleCount;
    int lastSample;
    int prevSample;
    int samples[48];
} BattleLimitTifaRibbon;

/* Impact particle of the magic hitmark effect: position, velocity, rotation and a countdown timer
 * (0 marks a free record in the pool). */
typedef struct BattleFxMagicHitmarkParticle {
    short timer;
    unsigned char pad_02[0x2];
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
    unsigned char pad_1A[0x2];
    short rateX;
    short rateY;
    short rateZ;
    unsigned char pad_22[0x2];
} BattleFxMagicHitmarkParticle;

/* Limit CM expanding model record: frame counter and centre position used to place the model. */
typedef struct BattleFxCosmoMemoryExpandingModel {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
} BattleFxCosmoMemoryExpandingModel;

/* Meteor R mesh record: mesh header pointer, flags and draw attribute bits. */
typedef struct BattleFxLimitMeteorainMeshRecord {
    int *meshHeader; /* the vertex block's byte size; the vertices and the primitive streams follow it */
    int flags;
    short clearedWordA;
    short shadeWeight;
    short drawMode;
    short clearedWordB;
} BattleFxLimitMeteorainMeshRecord;

/* Iyashi particle record: a vector slot index, a frame-advancing angle and an age. */
typedef struct BattleFxLimitHealingWindParticle {
    short active;
    short vectorIndex;
    short angle;
    short angleStep;
    short age;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxLimitHealingWindParticle;

/* Hakou particle record: age, per-axis position and velocity, shared by trail and sprite updates. */
typedef struct BattleFxBladeBeamParticle {
    short frameIndex;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
} BattleFxBladeBeamParticle;

/* Battle effect slot for the Syaku special timeline: fields copied to a second record, plus a dispatch level. */
typedef struct BattleFxScorchTimeline {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0xc];
    short level;
} BattleFxScorchTimeline;

/* Dethper mesh header: vertex data pointer, per-face edge vectors and computed normals. */
typedef struct BattleFxDespellMesh {
    int *meshData;
    unsigned char pad_04[0x20];
    int normNum;
    unsigned char pad_28[0x4];
    int targetCount;
    unsigned char pad_30[0x4c];
    VECTOR edge1;
    VECTOR edge2;
    int normalX;
    int normalY;
    int normalZ;
} BattleFxDespellMesh;

/* Task slot for an angle-gated model in the magic stop effect: phase, angle steps and clamp limit. */
typedef struct BattleFxAngleGatedModel {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinAngle;
    short yaw;
    short sideDistance;
    short sideDistanceStep;
    short scale;
    short scaleStep;
    short clampLimit;
    short lateScaleStep;
} BattleFxAngleGatedModel;

/* ReDo projectile slot: position integrated each frame from per-axis velocity. */
typedef struct BattleFxEnergyOrbProjectile {
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
    short variant;
} BattleFxEnergyOrbProjectile;

/* PSX polygon block drawn by the battle effect code, with flags and a vertex shade value. */
typedef struct BattleFxPsxPolygonBlock {
    int dataPtr;
    int flags;
    unsigned char pad_08[0x2];
    short shade;
    short pageBits;
    unsigned char pad_0E[0x2];
    int offsetPtr;
} BattleFxPsxPolygonBlock;

/* Record passed to the Faiga PSX draw helpers; holds a packed gray color value. */
typedef struct BattleFxFaigaMeshRecord {
    int meshBlob;
    int drawListHandle;
    int scaleShift;
    int renderState;
    int packedBits;
    int primCount;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int color;
    int clearedWord;
    ShadowInt64Pair vertices[4];
} BattleFxFaigaMeshRecord;

/* PSX mesh block for the Odin Zantetsuken summon draw; holds flags and a vertex shade value. */
typedef struct BattleFxZantetsukenMeshBlock {
    int meshData;
    int flags;
    short offsetX;
    short shade;
    short texPageOffset;
    short offsetY;
} BattleFxZantetsukenMeshBlock;

/* Tumbling Barret weapon projectile: position, velocity, spin angle and timer in one Effect100 slot. */
typedef struct BattleFxBarretWeaponTumble {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    short angle;
    short velocityX;
    short velocityY;
    short velocityZ;
    short angleRate;
    short angleX;
    short angleXRate;
    short angleZ;
    short angleZRate;
} BattleFxBarretWeaponTumble;

/* Blue Beata particle field: per-particle positions and frame indices drawn from g_BattleBlueBetaParticles. */
typedef struct BattleFxBlueBetaParticleField {
    int drawListPtr; /* +0x0 */
    int renderMode; /* +0x4: 2 */
    int end;
    unsigned char pad_0C[0x4];
    int projectFlagA; /* +0x10: the first flag word of ProjectPointHardware */
    int depth;
    int flags; /* +0x18: the second flag word of ProjectPointHardware; bits 0x60000 skip the trail */
    PsxMatrix cameraTransform; /* +0x1c */
    short posX;
    short posY;
    short posZ;
    unsigned char pad_40[0x1c];
    int point0;
    int point1;
    int minDeltaX;
    int minDeltaXSeen;
    int frame;
    int prevFrame;
} BattleFxBlueBetaParticleField;

/* Ifrit PSX mesh draw record: data block pointer, draw flags, vertex shade and attribute bits. */
typedef struct BattleFxIfritPsxMesh {
    int dataPtr;
    int flags;
    unsigned char pad_08[0x2];
    short vertexShade;
    short attributeBits;
} BattleFxIfritPsxMesh;

/* Effect-100 slot driving a weapon-2 actor through ground-ride and spiral-away motion. */
typedef struct BattleFxWhirlsandRideSlot {
    short actorIndex;
    short step;
    short startOffset;
    short heightOffset;
    short targetPos;
    unsigned char pad_0A[0x2];
    short savedRotX;
    short savedYaw;
    short savedRotZ;
    unsigned char pad_12[0x2];
    short actorFlags;
} BattleFxWhirlsandRideSlot;

/* Blue Kaenn particle: base point, sine-driven offsets, vertical velocity and life. */
typedef struct BattleFxBlueFlameThrowerParticle {
    short claimState;
    short frameCount;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x2];
    short ampX;
    short ampY;
    short ampZ;
    unsigned char pad_12[0x2];
    short offsetY;
    short velocityY;
    short accelY;
    short angleSpeed;
    short kaennSpriteDrawAux;
    short lifeFrames;
} BattleFxBlueFlameThrowerParticle;

/* Seald face-velocity record: data block, face index and three normal components. */
typedef struct BattleFxMagicShieldFace {
    int dataBlock;
    unsigned char pad_04[0x20];
    int faceIndex;
    unsigned char pad_28[0x54];
    int edgeFromPrevX;
    int edgeFromPrevY;
    int edgeFromPrevZ;
    unsigned char pad_88[0x4];
    int edgeToNextX;
    int edgeToNextY;
    int edgeToNextZ;
    unsigned char pad_98[0x4];
    int normalX;
    int normalY;
    int normalZ;
} BattleFxMagicShieldFace;

/* Alexander PSX mesh draw record: mesh data block, draw flags, vertex shade and attribute bits. */
typedef struct BattleFxProceduralMesh {
    int meshData;
    int flags;
    unsigned char pad_08[0x2];
    short vertexShade;
    short attrBits;
} BattleFxProceduralMesh;

/* Grav2 timeline slot: step counter and two battle actor indices for bone positions. */
typedef struct BattleFxMagicDemi2Timeline {
    short ownerSlot;
    short step;
    short actorIndex;
    short targetIndex;
} BattleFxMagicDemi2Timeline;

/* One point of an Uzumaki spiral trail's ring buffer: a world position. */
typedef struct BattleFxUzumakiTrailPoint {
    short x;
    short y;
    short z;
    short pad;
} BattleFxUzumakiTrailPoint;

/* Uzumaki spiral trail particle (0x4C bytes; step 0xFFFF marks a free record): age, polar position and
 * angle around the whirlpool, and a ring buffer of the last 8 world positions that outIndex writes. */
typedef struct BattleFxSpecialWhirlpoolSpiralTrail {
    short step;
    short index;
    short radius;
    short height;
    short angle;
    short outIndex;
    BattleFxUzumakiTrailPoint points[8];
} BattleFxSpecialWhirlpoolSpiralTrail;


/* Primitive-list header for the Limit night PSX mesh draw, with flags, shade amount and attribute mask. */
typedef struct BattleFxLimitNightmarePsxMesh {
    int primData;
    int flags;
    unsigned char pad_08[0x2];
    short shade;
    short attrMask;
} BattleFxLimitNightmarePsxMesh;


/* One attack entry from the token section, copied into the battle command context. */
typedef struct BattleAttackEntry {
    unsigned char accuracy;
    unsigned char secondaryDisplay;
    unsigned char hitReactionIndex;
    unsigned char pad_03[0x1];
    unsigned short mpCost;
    short hitValue;
    unsigned short animParam;
    unsigned short animParamAlt;
    unsigned char targetFlags;
    unsigned char resolvedAttackId;
    unsigned char damageCalcByte;
    unsigned char power;
    /* +0x10 */ unsigned char useCondition;
    unsigned char statusChangeId;
    unsigned char addedEffectId;
    unsigned char addedEffectParam;
    int statusMask;
    unsigned short elementMask;
    unsigned short specialFlags;
} BattleAttackEntry;

/* Particle record for the Blue Kaenn effect: sine-driven position, drift and vertical velocity. */
typedef struct BattleBlueFlameThrowerParticle {
    short ownerSlot;
    short frameCounter;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x2];
    short driftX;
    short driftY;
    short driftZ;
    unsigned char pad_12[0x2];
    short offsetY;
    short velocityY;
    short accelerationY;
    short angleStep;
    unsigned char pad_1C[0x2];
    short period;
} BattleBlueFlameThrowerParticle;

/* Sprite quad draw request read by BattleFx_SubmitSpriteQuadDraw. */
typedef struct BattleFxSpriteQuadRequest {
    short offsetX;
    short offsetY;
    char cellX;
    unsigned char cellY;
    unsigned char widthUnits;
    unsigned char heightUnits;
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char pad_0B[0x1];
    short flags;
    short cellWord;
    int quadState;
} BattleFxSpriteQuadRequest;

/* Effect100 slot for the CM tri-model spin; step is the step that drives its angle table. */
typedef struct BattleFxCosmoMemoryTriSpin {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
} BattleFxCosmoMemoryTriSpin;

/* Magic stop side-model record: timer and spin angle with drift values. */
typedef struct BattleFxStopSideModel {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinAngle;
    short yaw;
    short offsetDistance;
    short offsetDistanceStep;
    short ramp;
    short rampAccel;
    unsigned char pad_18[0x2];
    short rampDrag;
} BattleFxStopSideModel;

/* Sailess growing model record: timer, spin angle and drift values. */
typedef struct BattleFxSilenceGrowingModel {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spinAngle;
    short yaw;
    short extent;
    short extentStep;
    short scale;
    short scaleStep;
    short targetIndex;
} BattleFxSilenceGrowingModel;

/* Shiva force-field particle: force mask, position and velocity. */
typedef struct BattleFxShivaParticle {
    short forceMask;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxShivaParticle;

/* One ring of trail points of a thrown Cid weapon (0x6C bytes): write and read positions, the number of
 * points in use, and the point pointers. */
typedef struct BattleFxCidTrailSource {
    int head;
    int tail;
    int count;
    struct BattleFxThrowTrailPoint *points[24];
} BattleFxCidTrailSource;

/* Load descriptor passed to BattleModel_LoadPartyDatLegacy. */
typedef struct Battle3dPartyLoadDesc {
    int useFaceTexturePage;
    int loadKind;
    int packedAnim;
    int boneCount;
    int loadValue;
    int auxValue;
    int partIndex; /* +0x18 */
    int firstCount;
    int baseIndex;
    int enabled;
    int secondCount;
    int loadCategory;
    int loaded;
    int nameId;
    int subCount;
    int useFullTimBounds;
    int hasPolygons;
} Battle3dPartyLoadDesc;

/* Task slot record (0x20 stride) for the Hikari 3 special timeline task. */
typedef struct BattleFxLightBlast3Slot {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    unsigned char pad_0E[0x2];
    short targetZ;
    unsigned char pad_12[0x8];
    short boneIndex;
} BattleFxLightBlast3Slot;

/* Seald textured-triangle block: data pointer and countdown, with per-vertex short accumulators. */
typedef struct BattleFxShieldTriangleBlock {
    int blockPtr;
    int orderingTableSlot;
    int countdown;
    int renderStateBlock;
    int flags;
    int frontFacing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int frameNumber;
    unsigned char pad_28[0x8c];
    short vertex0X;
    short vertex0Y;
    short vertex0Z;
    unsigned char pad_BA[0x2];
    short vertex1X;
    short vertex1Y;
    short vertex1Z;
    unsigned char pad_C2[0x2];
    short vertex2X;
    short vertex2Y;
    short vertex2Z;
} BattleFxShieldTriangleBlock;

/* Stop central model record: timer, angle and ramp accumulators, plus the record type to spawn. */
typedef struct BattleFxStopCentralRecord {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short yaw;
    short offsetDistance;
    unsigned char pad_12[0x2];
    short ramp;
    short rampAccel;
    unsigned char pad_18[0x2];
    short rampDrag;
    short allocType;
} BattleFxStopCentralRecord;

/* Utagoe timeline record: timer, base position with random jitter ranges, and a scale. */
typedef struct BattleFxSirenSongTimeline {
    short ownerSlot;
    short timer;
    short baseX;
    short baseY;
    short baseZ;
    unsigned char pad_0A[0x2];
    short jitterCenterXZ;
    short jitterRangeXZ;
    short jitterOffsetY;
    short jitterRangeY;
    short scale;
} BattleFxSirenSongTimeline;

/* Lgirl trail history: ring write index and count of recorded positions. */
typedef struct BattleLimitLuckyGirlTrail {
    int index;
    int readIndex;
    int count;
    int capacity;
    int writeIndex;
    int prevWriteIndex;
    int positions[8]; /* the ring of position addresses, capacity entries long */
} BattleLimitLuckyGirlTrail;

/* Rising fading sprite effect slot: timer, height, rise speed and acceleration, lifetime. */
typedef struct BattleFxRisingFadeSprite {
    short ownerSlot;
    short timer;
    short posX;
    short height;
    unsigned char pad_08[0x6];
    short riseSpeed;
    unsigned char pad_10[0x2];
    short riseAccel;
    short scale;
    unsigned char pad_16[0x6];
    short delay;
    short lifetime;
} BattleFxRisingFadeSprite;

/* Depth-shaded PSX mesh draw parameters: flag bits, vertex shade and attribute bits. */
typedef struct BattleFxPsxMeshDraw {
    int dataPtr;
    int flags;
    unsigned char pad_08[0x2];
    short shade;
    short attrBits;
} BattleFxPsxMeshDraw;

/* Fading beam sprite task record: integrated position and damped velocity, plus angle. */
typedef struct BattleFxBeamFadeSprite {
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
} BattleFxBeamFadeSprite;

/* Blue Kaenn spawn centre: three shorts, jittered for each spawned particle. */
typedef struct BattleFxSpawnCenter {
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_06[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short sourceX;
    unsigned char pad_12[0x2];
    short sourceY;
    unsigned char pad_16[0x2];
    short sourceZ;
} BattleFxSpawnCenter;

/* Syaku bouncing sprite record: bounce velocity and angle over a frame counter. */
typedef struct BattleFxScorchBounceSprite {
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
    short spriteFrame;
} BattleFxScorchBounceSprite;

/* ReDo bouncing sprite record: frame counter, three position/velocity pairs and an angle. */
typedef struct BattleFxEnergyOrbBouncingSprite {
    short ownerSlot;
    short frameCount;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velocityA;
    short velocityB;
    short velocityC;
    unsigned char pad_12[0x2];
    short angle;
    short variant;
} BattleFxEnergyOrbBouncingSprite;

/* Bakyu draw segment record: segment index and two centre values for a quad. */
typedef struct BattleFxVacuumSegment {
    short active;
    short segmentIndex;
    short startX;
    short centerA;
    short startY;
    unsigned char pad_0A[0x2];
    short endX;
    short centerB;
    short endY;
    unsigned char pad_12[0x2];
} BattleFxVacuumSegment;

/* Night-limit trail ring buffer record (stride 0x68) holding a write index and point history. */
typedef struct BattleFxLimitNightmareTrailRing {
    int writeIndex;
    int tail;
    int count;
    int capacity;
    int lastIndex;
    int prevIndex;
    int points[8]; /* +0x18: the capacity slots; each holds the address of a SVECTOR point */
    unsigned char pad_38[0x30];
} BattleFxLimitNightmareTrailRing;

/* Fading stacked-model layer record for the Kotr summon, moved by position plus velocity. */
typedef struct BattleFxScreenFadeTask {
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char pad_03[0x1];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x12];
    short phaseLength0;
    short phaseLength1;
    short phaseLength2;
    short layerSpacing;
    short angleOffset;
    short layerCount;
    short variant;
    short baseOffset;
} BattleFxScreenFadeTask;

/* One queued battle action: type, acting actor, flags and target mask. */
typedef struct BattleAction {
    unsigned char actionType;
    unsigned char pad_01[0x1];
    char actorIndex;
    char flags;
    short attackId;
    unsigned short targetMask;
} BattleAction;

/* Blue sflea moving spark sprite: position and velocity, stepped each frame. */
typedef struct BattleFxShadowFlareLivewire {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short pushLength;
    short scale;
    short flag;
} BattleFxShadowFlareLivewire;

/* Timeline record for the Kyuu special, with the same layout as the Kyuu2 timeline record. */
typedef struct BattleFxDrainTimelineRecord {
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
    short effectType;
    short targetIndex;
} BattleFxDrainTimelineRecord;

/* Weapon 3 trail history of point indices, with the draw start, count and limit used by the sprite task. */
typedef struct BattleFxEmeraldShootTrailState {
    int headIndex;
    int startIndex;
    int count;
    int limit;
    int prevIndex;
    int prevPrevIndex;
} BattleFxEmeraldShootTrailState;

/* Limit Kodou animated sprite particle: position, damped velocity with gravity, and frame counter. */
typedef struct BattleFxPulseOfLifeSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxPulseOfLifeSprite;

/* Queig magic shard setup: mesh payload and vertex pointers, plus shard count and index. */
typedef struct BattleFxQuake3ShardState {
    int meshPayload;
    int referenceVertices;
    unsigned char pad_08[0xc];
    int shardCount;
    int baseVelocity;
    unsigned char pad_1C[0x8];
    int shardIndex;
} BattleFxQuake3ShardState;

/* Dejon pull task state: step, slot index, per-actor velocities and start value of the pull. */
typedef struct BattleFxRemovePullState {
    short ownerSlot;
    short step;
    Int64Pair position;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short startValue;
    short slotIndex;
} BattleFxRemovePullState;

/* Spark pixel record of the Comet magic effect: a step counter, a position and a per-frame velocity. */
typedef struct BattleFxLivewireDotSlot {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxLivewireDotSlot;

/* Four shorts copied into the Alexander offscreen request slots. */
typedef struct BattleFxOffscreenCaptureReq {
    short viewportX;
    short viewportY;
    short viewportW;
    short viewportH;
} BattleFxOffscreenCaptureReq;

/* Four shorts copied into the Titan offscreen request slots. */
typedef struct BattleFxTitanOffscreenRequest {
    short posX;
    short posY;
    short posZ;
    short sourceIndex;
} BattleFxTitanOffscreenRequest;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxSatanSlamFragmentPrimitive {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned short levelAndOffset;
    int color0;
    unsigned short flags;
    short color1;
    short color2;
    unsigned short shade;
} BattleFxSatanSlamFragmentPrimitive;

/* Drifting animated sprite: frame index and position advanced by a per-frame velocity. */
typedef struct BattleFxEnergyOrbDriftSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxEnergyOrbDriftSprite;

/* Per-actor draw scratch record at +0x20 of a battle model bank: floats copied from actor records and fixed-point slots. */
typedef struct BattleActorDrawState {
    unsigned char pad_00[0x4];
    float posX;
    float posY;
    float posZ;
    float rotX;
    float yaw;
    float rotZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    unsigned char pad_28[0x44];
    union {
        float *boneWorldPosX;
        int outPos;
    };
    union {
        unsigned char *worldMatrixPtr;
        int transformBase;
    };
} BattleActorDrawState;

/* Battle actor model: sub-skeleton at +0x20 and per-part tables at +0x08 to +0x1C. */
typedef struct Battle3dActorModel {
    unsigned char pad_00[0x8];
    int subSkelCount;
    int subSkelTable;
    int skelCount;
    int skelTable;
    int partCount;
    struct PolygonModel **partTable;
    BattleActorDrawState drawState;
} Battle3dActorModel;

/* GPU Gouraud line packet: tag word and command byte, with vertex colours left as written. */
typedef struct BattleGpuGouraudLine {
    int tag;
    char colorR0;
    char colorG0;
    char colorB0;
    char command;
    unsigned char pad_08[0x4];
    char colorR1;
    char colorG1;
    char colorB1;
} BattleGpuGouraudLine;

/* Timeline state for the Ranntou limit: frame counter and target actor index. */
typedef struct BattleLimitBigBrawlTimeline {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x1a];
    short actorIndex;
} BattleLimitBigBrawlTimeline;

/* Projectile state for the CM limit: frame counter, position and per-frame velocity. */
typedef struct BattleLimitCosmoMemoryProjectile {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
} BattleLimitCosmoMemoryProjectile;

/* Effect100 slot record for the Queik grow/shrink model task. */
typedef struct BattleFxGrowShrinkRecord {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short value;
    short step;
    short recordType;
} BattleFxGrowShrinkRecord;

/* Three-short 3D point the Hakou trail particle fills from a target record. */
typedef struct BattleFxBladeBeamTrailPoint {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    int pad_08;
    int pad_0C;
    unsigned char pad_10[0x2];
    short pad_12;
    unsigned char pad_14[0x4];
    int pad_18;
    unsigned char pad_1C[0xc];
    int depth;
    unsigned char pad_2C[0x4];
    int flags;
} BattleFxBladeBeamTrailPoint;

/* Effect100 slot filled by the ReDo special timeline from a source record. */
typedef struct BattleFxSpecialEnergyOrbSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxSpecialEnergyOrbSlot;

/* Spawn task slot for the second Neo Bahamut rock: offset from the anchor, halved to fit range. */
typedef struct BattleFxDebrisSpawnTask {
    union { int offsetX; short offsetXShort; };
    union { int offsetY; short offsetYShort; };
    union { int offsetZ; short offsetZShort; };
} BattleFxDebrisSpawnTask;

/* Effect100 slot for the Leviathan sprite particle, stepped and drawn each frame. */
typedef struct BattleSummonLeviathanSpriteParticle {
    short state;
    short angle;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleSummonLeviathanSpriteParticle;

/* Slou effect timeline record: frame counter, position and velocity vectors. */
typedef struct BattleFxSlowParticle {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxSlowParticle;

/* Sephiroth Nova camera script state: per-axis camera offset deltas. */
typedef struct BattleFxSuperNovaCameraDelta {
    int deltaX;
    int deltaY;
    int deltaZ;
} BattleFxSuperNovaCameraDelta;

/* Blue Kaenn particle record: position, velocity, angle and timer, spawned by BattleFx_BlueFlameThrowerSpawnParticles. */
typedef struct BattleFxFlameThrowerParticle {
    short state;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short ampX;
    short ampY;
    short ampZ;
    unsigned char pad_12[0x2];
    short offsetY;
    short velocityY;
    short accelY;
    short angleSpeed;
    short clutWord;
    short lifeFrames;
} BattleFxFlameThrowerParticle;

/* Context for effect program 5: frame data pointer, polygon blob, draw-list handles and triangle count. */
typedef struct BattleFxProgram5Context {
    int frameData;
    int polygonBlob;
    int drawListHandle;
    int drawMode;
    int drawListHandle2;
    unsigned char pad_14[0x4];
    int highWordMask;
    int count;
    unsigned char pad_20[0x4];
    int orderingDepth;
} BattleFxProgram5Context;

/* Roket variant sprite slot: frame index, position with velocity, and sprite variant. */
typedef struct BattleFxHellBomberVariantSprite {
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
} BattleFxHellBomberVariantSprite;

/* Geki1 trail emitter projectile slot: start position with random spread and launch velocity. */
typedef struct BattleFxSwampShootTrailEmitter {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short frameIndex;
} BattleFxSwampShootTrailEmitter;

/* Geki1 driver slot: frame index, position and velocity that the emitter slots copy from. */
typedef struct BattleFxSwampShootDriver {
    short ownerSlot;
    short frameIndex;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
} BattleFxSwampShootDriver;

/* Hades tube state: projected circle points and tube section data for its draw helpers. */
typedef struct BattleFxHadesTube {
    unsigned char pad_00[0x1c];
    int projected;
    int prevProjected;
    unsigned char pad_24[0x14];
    short centerX;
    short circlePointY;
    short centerZ;
    unsigned char pad_3E[0xa];
    short radiusVectorX;
    short radiusVectorY;
    short radius;
    unsigned char pad_4E[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_56[0x52];
    int shadeWord;
    int colorWord;
    int colorStart;
    int colorEnd;
    unsigned char pad_B8[0x10];
    short angleX;
    short angle;
    short angleZ;
    unsigned char pad_CE[0x2];
    unsigned char rotMatrix[0x1e];
} BattleFxHadesTube;

/* Effect100 slot record for the Ifrit summon billboard sprite: frame counter and a shaken position vector. */
typedef struct BattleFxBillboardSpriteSlot {
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
} BattleFxBillboardSpriteSlot;

/* An enemy's record (0xB8 bytes; a scene's three are g_BattleState.enemyRecords). Named from the
   code that reads it: Battle_InitEnemyCombatants copies level, mp, hp, damageScale, exp and gil
   into the enemy's combatant, and the stat bytes +0x21..+0x27 into its stat members (a scene with
   flag 0x40 doubles hp and scales strength and magic by 125%; in game each equals the
   scene's stat); the attack op finds the attack in attackIds and takes attackStepIds and
   attackAnimParams at the same index (the command context's stepId and animParam);
   Battle_RollEnemyDropOrSteal reads the four drop/steal entries (bits 6-7 of a rate are the kind,
   0x80 steal; bits 0-5 the chance; then the item); the battle end adds ap; Battle_TryMorphReward
   grants morphItem; the kernel text formatter copies name, 0x20 bytes of FF7 text. */
typedef struct BattleEnemyRecord {
    unsigned char name[0x20]; /* +0x00 */
    char level; /* +0x20 */
    char dexterity; /* +0x21: the combatant's dexterity */
    char luck; /* +0x22: the combatant's luck */
    char defensePercent; /* +0x23: the combatant's defensePercent */
    char strength; /* +0x24: the combatant's attack */
    unsigned char defense; /* +0x25: twice it is the combatant's defense */
    char magic; /* +0x26: the combatant's magicAttack */
    unsigned char magicDefense; /* +0x27: twice it is the combatant's magicDefense */
    unsigned char elementIds[8]; /* +0x28: the elements the enemy has an affinity to; 0xFF none (Battle_BuildElementAffinityTable) */
    unsigned char elementCategories[8]; /* +0x30: the affinity group of each element id */
    unsigned char attackStepIds[0x10]; /* +0x38 */
    unsigned short attackIds[0x10]; /* +0x48 */
    unsigned short attackAnimParams[0x10]; /* +0x68 */
    unsigned char dropStealRates[4]; /* +0x88 */
    short dropStealItems[4]; /* +0x8C */
    short slotAttackIds[3]; /* +0x94: looked up in the scene's attack ids for the enemy's command slots */
    unsigned char pad_9A[0x2];
    short mp; /* +0x9C */
    unsigned short ap; /* +0x9E */
    unsigned short morphItem; /* +0xA0 */
    char damageScale; /* +0xA2 */
    unsigned char pad_A3[0x1];
    int hp; /* +0xA4 */
    int exp; /* +0xA8 */
    int gil; /* +0xAC */
    int statusImmunityMask; /* +0xB0: its complement is the combatant slot's auxWord */
    unsigned char pad_B4[0x4];
} BattleEnemyRecord;
typedef char ff7_size_enemy_record[(sizeof(BattleEnemyRecord) == 0xB8) ? 1 : -1];

/* A party member's battle record: g_BattleState.partyStats[3] at 0x009A8DB8 (BattleState.partyStats),
   0x34 bytes each (`imul edx, edx, 0x34` at 0x005defaf; the companion `mov eax, [edx + 0x9a8db8]`
   loads partyMember). Battle_LoadPartyActorsFromEquipment fills it from the member's savemap record
   and party stat words; the members are named from the code that reads them. */
typedef struct BattlePartyActor {
    struct SavemapCharacter *partyMember; /* +0x00: the member's savemap character record */
    unsigned char limitUseCount; /* +0x04: incremented when the limit is used (Kernel_UpdateLimitUnlocks) */
    char limitLevelIndex; /* +0x05 */
    unsigned char manipulatedSlot; /* +0x06: the slot of the enemy this member manipulates */
    unsigned char killCount; /* +0x07: passed to Kernel_UpdateLimitUnlocks with limitUseCount */
    short limitGauge; /* +0x08: 0xFF is a full gauge (Battle_SetLimitReady); cleared when the limit is used */
    short loadLimitGauge; /* +0x0A: the gauge as the battle loaded it */
    short mp; /* +0x0C */
    short hp; /* +0x0E */
    short maxMp; /* +0x10 */
    short maxHp; /* +0x12 */
    short mpCap; /* +0x14: 999, or 9999 with the party stat flag 8 (the caps swap) */
    short hpCap; /* +0x16: 9999, or 999 with flag 8 */
    unsigned short queuedSupportCodes[2]; /* +0x18: queued support entries for the next action (low byte support bits, high byte repeat casts), 0xFFFF = empty */
    int limitLevelParam; /* +0x1C */
    int accessoryStatus; /* +0x20: the statuses Kernel_ApplyAccessoryAutoStatus last applied for the worn accessory */
    int materiaTypeBits; /* +0x24 */
    int loadMateriaTypeBits; /* +0x28: materiaTypeBits as the battle loaded them; the battle's end
                                compares the two */
    unsigned char pad_2C[0x8];
} BattlePartyActor;
typedef char ff7_size_party_actor[(sizeof(BattlePartyActor) == 0x34) ? 1 : -1];

/* Party stat words of 0x440 bytes per party slot, read when battle loads its party actors. */
typedef struct BattlePartyStatWords {
    unsigned char pad_00[0x6];
    char dexterity;
    char luck;
    char attack;
    unsigned char pad_09[0x1];
    short defense;
    char magicAttack;
    unsigned char pad_0D[0x1];
    short magicDefense;
    short hp;
    short maxHp;
    short mp;
    short maxMp;
    short atbGauge;
    short equipWord;
    short barrierGauge;
    short mBarrierGauge;
    char rowFlag;
    unsigned char pad_21[0x1];
    char clearedByte;
    unsigned char flags;
    unsigned char pad_24[0x18];
    unsigned short extraBits;
    unsigned short elementBits;
    unsigned short statusBits;
    unsigned short elementFlags;
    int stateWord;
    int auxWord;
    unsigned char pad_4C[0x60];
    unsigned char limitAttacks[3]; /* +0xAC: the limit attacks the battle menu lists */
    unsigned char pad_AF[0x359];
    char savedFlags;
    unsigned char pad_409[0x7];
    char weaponAccuracy;
} BattlePartyStatWords;

/* Battle item action entry (0x1C bytes) copied into the battle command context. */
typedef struct BattleItemAction {
    unsigned char pad_00[0x8];
    unsigned short animParam;
    /* +0x0A */ unsigned short equipRestrictions;
    unsigned char targetFlags;
    unsigned char resolvedAttackId;
    unsigned char damageCalcByte;
    unsigned char power;
    /* +0x10 */ unsigned char useCondition;
    unsigned char statusChangeId;
    unsigned char addedEffectId;
    unsigned char addedEffectParam;
    int statusMask;
    unsigned short elementMask;
    unsigned short specialFlags;
} BattleItemAction;

/* Effect-slot record (0x20 bytes) for the Blue Aqua timeline task. */
typedef struct BattleFxBlueAqualungSlot {
    short state;
    unsigned char pad_02[0x4];
    short range;
    unsigned char pad_08[0x4];
    short baseValue;
    short baseTimes2;
    short step;
    short stepTimes3;
    short particleScale;
} BattleFxBlueAqualungSlot;

/* Gaisei mesh draw input: mesh data pointer, draw flags, shade value and attribute bits. */
typedef struct BattleFxLandscaperMesh {
    int meshData;
    int drawFlags;
    unsigned char pad_08[0x2];
    short shade;
    short attrBits;
} BattleFxLandscaperMesh;

/* Effect-100 slot for the Battle effect program 1 step. */
typedef struct BattleFxProgram1Slot {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0a[0xa];
    short recordType;
} BattleFxProgram1Slot;

/* Fading quad particle record for the Queig magic effect task. position is the vertical coordinate. */
typedef struct BattleFxDebrisParticle {
    short ownerSlot;
    short step;
    short posX;
    short position;
    short posZ;
    unsigned char pad_0A[0x8];
    union {
        char fadeLevel;
        short fadeWord;
    };
    short heightOffset;
    short velocity;
    short acceleration;
} BattleFxDebrisParticle;

/* Battle actor record fields used by the Bahamut descent and the Odin fade move. */
typedef struct BattleActorFadeDescentRecord {
    unsigned char pad_00[0x6];
    short fadeStep;
    unsigned char pad_08[0xc];
    short fadeLevel;
    unsigned char pad_16[0xf];
    char flags;
    unsigned char pad_26[0x2];
    char tintR;
    char tintG;
    char tintB;
    unsigned char pad_2B[0x13b];
    short posA;
    short posB;
    short posC;
} BattleActorFadeDescentRecord;

/* Enemy model descriptor built by the legacy DAT loader; holds skeleton bone vectors and per-part values. */
typedef struct Battle3dEnemyModel {
    int modelId;
    int isActive;
    int enabled;
    int entryCount;
    int reservedWordA;
    unsigned char pad_14[0x4];
    int textureCount;
    int animCount;
    int weaponAnimStart;
    int weaponModelCount;
    int weaponAnimCount;
    int reservedWordB;
    int boneVectors;
} Battle3dEnemyModel;

/* Element and status affinity bits for one combatant, built by Battle_BuildElementAffinityTable. */
typedef struct BattleAffinityTable {
    union {
        int entries[16];
        struct {
            int elementEntries[8];
            int statusEntries[8];
        };
        struct {
            unsigned char pad_00[0x10];
            int elementBits;
            int statusBits;
            int elementFlags;
            unsigned char pad_1C[0x18];
            int immunityBits;
            unsigned char pad_38[0x8];
        };
    };
} BattleAffinityTable;

/* Death strip timeline task slot: step index and a 4096-unit angle. */
typedef struct BattleFxMagicDeathStrip {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
} BattleFxMagicDeathStrip;

/* Freeze timeline record in an effect-array slot; its stage counter selects each step. */
typedef struct BattleFxFreezeTimeline {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
} BattleFxFreezeTimeline;

/* Heart1 bone emitter record: timer, and the battle actor and bone picked per particle. */
typedef struct BattleFxHeartCardBoneEmitter {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x9];
    char cellRow;
    short cellWord;
    unsigned char pad_10[0x2];
    short actorIndex;
    short boneIndex;
    short spriteFlags;
} BattleFxHeartCardBoneEmitter;

/* 2D point of the Timedmg warp grid, moved radially about the centre. */
typedef struct BattleFxTimeDamageWarpPoint {
    short offsetX;
    short offsetY;
} BattleFxTimeDamageWarpPoint;

/* Titan mesh draw state: pointer to mesh data, flags and texture-page bits. */
typedef struct BattleFxTitanMeshInstance {
    int meshData;
    int flags;
    unsigned char pad_08[0x2];
    short progress;
    short pageBits;
} BattleFxTitanMeshInstance;

/* Battle actor record used by the Weapon2 spiral-away task. */
typedef struct BattleActorSpiralRecord {
    unsigned char pad_00[0x8];
    unsigned short scaleX;
    unsigned short scaleY;
    short boneIndex;
    unsigned char pad_0E[0x2];
    short stepCount;
    unsigned char pad_12[0x2];
    short tintPhase;
    unsigned char pad_16[0xf];
    char flags;
    unsigned char pad_26[0x2];
    char tintR;
    char tintG;
    char tintB;
    unsigned char pad_2B[0x135];
    short spiralAngle;
    unsigned char pad_162[0x4];
    short posX;
    short groundHeight;
    short posZ;
    unsigned char pad_16C[0x2e];
    short bone0PosX;
    unsigned char pad_19C[0x2];
    short bone0PosZ;
} BattleActorSpiralRecord;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxCosmoMemoryRingSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
} BattleFxCosmoMemoryRingSlot;

/* PSX mesh header drawn by the Sting effect: data pointer, flags and a shade value. */
typedef struct BattleFxPsxMeshHeader {
    union {
        int *words;
        int dataPtr;
    };
    int flags;
    unsigned char pad_08[0x2];
    short shade;
} BattleFxPsxMeshHeader;

/* Three-short record addressed through the offset table in the Satan quad-fragment block. */
typedef struct BattleFxSatanSlamFragmentRecord {
    short posX;
    short posY;
    short posZ;
    unsigned short vertexOffset;
} BattleFxSatanSlamFragmentRecord;

/* Offscreen render request from Battle3d_AcquireOffscreenRequest, with a mode and a result slot. */
typedef struct BattleOffscreenRequest {
    int drawKind;
    int partMesh;
    unsigned char pad_08[0xc0];
    int colorKeyEnabled;
    int renderKind;
    int mode;
    unsigned char pad_D4[0x8];
    int viewportX;
    int viewportY;
    int viewportW;
    int viewportH;
    unsigned char pad_EC[0x20];
    int sampleStepX;
    int sampleStepY;
    int offscreenSlot;
} BattleOffscreenRequest;

/* Per-face record: three vertex offsets relative to the face centroid. */
typedef struct BattleFxFaceOffsets {
    short vertex0X;
    short vertex0Y;
    short vertex0Z;
    unsigned char pad_06[0x2];
    short vertex1X;
    short vertex1Y;
    short vertex1Z;
    unsigned char pad_0E[0x2];
    short vertex2X;
    short vertex2Y;
    short vertex2Z;
    unsigned char pad_16[0xa];
    short centroidX;
    short centroidY;
    short centroidZ;
} BattleFxFaceOffsets;

/* Trajectory task record: start and end points, a step counter and the dispatched effect type. */
typedef struct BattleFxTrajectory {
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
} BattleFxTrajectory;

/* Naparm arcing trail emitter slot; copies the position of a Naparm variant sprite. */
typedef struct BattleFxNapalmTrailSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x8];
    short variant;
} BattleFxNapalmTrailSlot;

/* Per-slot state for a Turara debris piece: position, drift velocity, angle and variant. */
typedef struct BattleFxIcicleDropDebrisState {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short variant;
    short angle;
    short spinAngle;
} BattleFxIcicleDropDebrisState;

/* Per-slot orbit trail state for the Ito special: centre, per-axis drift, orbit angle and radius. */
typedef struct BattleFxSpiderWebOrbit {
    short ownerSlot;
    short step;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    short angularVelocity;
    short radius;
    short radiusScale;
} BattleFxSpiderWebOrbit;

/* Per-slot Cold1 particle: start position, per-frame velocity and actor index. */
typedef struct BattleFxColdBreathParticle {
    short ownerSlot;
    short frame;
    short posX;
    short baseOffset;
    short posY;
    unsigned char pad_0A[0x2];
    short velocityX;
    short rate;
    short velocityY;
    unsigned char pad_12[0x2];
    short actorIndex;
    unsigned char pad_16[0x2];
    short scale;
    short fade;
} BattleFxColdBreathParticle;

/* Oscillating model record for the special-attack model tasks: damped offset, phase counter and angle. */
typedef struct BattleFxOscillatingModel {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x2];
    short offset;
    unsigned char pad_08[0x6];
    short velocity;
    unsigned char pad_10[0x4];
    short angle;
} BattleFxOscillatingModel;

/* Dokueki timeline record: frame counter, start and end positions, actor index. */
typedef struct BattleFxLiquidPoisonTimeline {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    unsigned char pad_0E[0x2];
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxLiquidPoisonTimeline;

/* Special accelerating-model effect slot: timer, two accumulating values with their velocities. */
typedef struct BattleFxSpecialAcceleratingSlot {
    short ownerSlot;
    short timer;
    short posA;
    short posY;
    short posB;
    unsigned char pad_0A[0x2];
    short velocityA;
    unsigned char pad_0E[0x2];
    short velocityB;
    unsigned char pad_12[0x2];
    short angleY;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxSpecialAcceleratingSlot;

/* Kaen particle record for the Special Kaen timeline: position and per-frame velocity. */
typedef struct BattleFxFireballParticleRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short variant;
} BattleFxFireballParticleRecord;

/* Grav2 effect slot: per-axis start position and per-step velocity, filled by the timeline. */
typedef struct BattleFxGrav2Particle {
    unsigned char pad_00[0x8];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
    short pushLength;
} BattleFxGrav2Particle;

/* Dia 2 spinning model slot: frame counter, position, vertical offset and velocity. */
typedef struct BattleFxSunCardSpinModel {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    short offsetY;
    unsigned char pad_0C[0x2];
    short velocityY;
    unsigned char pad_10[0x8];
    short modelId;
} BattleFxSunCardSpinModel;

/* Special Dia1 spinning model effect record: step counter, Y position and Y velocity. */
typedef struct BattleFxDiamondCardSpinRecord {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    short offsetDistance;
    unsigned char pad_0C[0x2];
    short velocityY;
    unsigned char pad_10[0x8];
    short scale;
} BattleFxDiamondCardSpinRecord;

/* Dejon inward arc sprite: step counter, position with velocity, and billboard angle. */
typedef struct BattleFxRemoveArcRecord {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
} BattleFxRemoveArcRecord;

/* Dokueki poison particle record: position from the source, heading toward a target, and scatter. */
typedef struct BattleFxLiquidPoisonParticleRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angle;
    unsigned char pad_16[0x2];
    short distance;
    short active;
} BattleFxLiquidPoisonParticleRecord;

/* Weapon Beam 1 fading sprite: position from the bone plus jitter, and a velocity. */
typedef struct BattleFxWeaponEmeraldBeamSprite {
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
} BattleFxWeaponEmeraldBeamSprite;

/* Per-slot position and per-step travel of the Kyuu timeline, stepping over 20 steps to the end point. */
typedef struct BattleFxDrainTimelineState {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
} BattleFxDrainTimelineState;

/* Four-short argument block copied into an offscreen stage request. */
typedef struct BattleFxWhirlsandStageRequestArgs {
    short argA;
    short argB;
    short argC;
    short argD;
} BattleFxWhirlsandStageRequestArgs;

/* Camera script state for the Knights of the Round summon camera (BattleFx_KotrRunCameraScript). */
typedef struct BattleFxCameraOrbitState {
    short mode;
    short stepCount;
    int progress;
    int stepSize;
    short attachPoint;
    short attachIndex;
    union {
        ShadowInt64Pair pos;
        struct {
            short posX;
            short posY;
            short posZ;
        };
    };
    union {
        ShadowInt64Pair delta;
        struct {
            short deltaX;
            short deltaY;
            short deltaZ;
        };
    };
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_26[0x2];
} BattleFxCameraOrbitState;

/* Per-frame camera move state of the Fat Chocobo summon camera script, held in a scratch global. */
typedef struct BattleFxFatChocoboCamera {
    short mode;
    short frameCount;
    int progress;
    int progressStep;
    short scriptWord;
    short scriptArgument;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_16[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_1E[0x2];
    short stepX;
    short stepY;
} BattleFxFatChocoboCamera;

/* ------------------------------------------------------------------------
 * BattleCombatant -- 10 records of 0x68 at 0x009AB0DC.
 *
 * Stride proved by `imul reg, reg, 0x68` at 0x005dc0ab, 0x005dc0c9, 0x005dc108,
 * 0x005def2b and 0x005de54x. The base is the displacement that accompanies the
 * lowest member: `mov edx, [ecx + 0x9ab0dc]` at 0x005dc10b reads `status`, so
 * status is at +0x00 and the array base is 0x009AB0DC.
 * ---------------------------------------------------------------------- */
typedef struct BattleCombatant {
    /* The types and the names past +0x04 are the ones the code's views of these records agree
     * on: an offset gets the name most casts onto g_aCombatants use for it. */
    /* +0x00 */ int status; /* 0x005dc10b, & 0x01004000                */
    /* +0x04 */ int flags; /* 0x005dc0ae / 0x005dc0cc, & 0x200 / 0x100 */
    /* +0x08 */ char unitId; /* the character id (Battle_LoadPartyActorsFromEquipment) or the scene's enemy
                                type 0..2 (Battle_InitEnemyCombatants); -1 (0xFF) an empty slot */
    /* +0x09 */ unsigned char level;
    unsigned char pad_0A[0x1];
    /* +0x0B */ char elementAffinity; /* Battle_ApplyStatusToActorMask: the element's affinity index
                                          (Battle_FindElementAffinityIndex, 3 for none) */
    /* +0x0C */ char sideSlot;
    /* Derived combatant stat bytes computed from base character stats and equipment. */
    /* +0x0D */ unsigned char attack; /* strength + weapon */
    /* +0x0E */ unsigned char magicAttack;
    /* +0x0F */ unsigned char defensePercent; /* the armour's defence %; the party's adds dexterity / 4 */
    /* +0x10 */ char scriptId;
    /* +0x11 */ unsigned char animId;
    /* +0x12 */ unsigned char damageScale;
    /* +0x13 */ unsigned char animStep;
    /* +0x14 */ unsigned char dexterity; /* with its bonus; hit % adds it / 4, the ATB tick scales by it */
    /* +0x15 */ unsigned char luck; /* with its bonus; a sure hit when rand(100) < luck / 4, and the
                                       critical roll adds it to the level */
    /* +0x16 */ char idleOverride; /* a party member's idle script while non-zero: the command ring copies
                                       it into scriptId each frame; 0xC/0xD/0xE at the battle's end */
    /* +0x17 */ char coverSlot; /* the side slot of the actor this one covers; 0xFF before a hit */
    /* +0x18 */ short postedActorMask; /* gBattleAiGlobals.lastActorMask, stored when Battle_AiVm_PostAction queues an action */
    /* +0x1A */ short attackerMask; /* the attacker's bit, set before the AI's general counter */
    /* +0x1C */ short physicalAttackerMask; /* set before the AI script's section 4 (physical counter) */
    /* +0x1E */ short magicalAttackerMask; /* set before section 5 (magical counter) */
    /* +0x20 */ unsigned short defense; /* vitality + armour */
    /* +0x22 */ unsigned short magicDefense;
    /* +0x24 */ unsigned short uniqueId;
    /* +0x26 */ unsigned short elementMask;
    /* +0x28 */ unsigned short curMp;
    /* +0x2A */ unsigned short maxMp;
    /* +0x2C */ int curHp; /* 0x005def2e, [ecx + 0x9ab108] = base+0x2c */
    /* +0x30 */ int maxHp; /* 0x005ded3f, [ecx + 0x9ab10c] = base+0x30;
                              Battle_DmgFormula11 subtracts the actor's current
                              HP from it, which is what makes it the maximum */
    unsigned char pad_34[0x10];
    /* +0x44 */ int prevStatusFlags;
    unsigned char pad_48[0x4];
    /* +0x4C */ char isEnemy; /* 1 for an enemy (Battle_InitEnemyCombatants) */
    /* +0x4D */ unsigned char magicDefensePercent; /* the armour's magic evade */
    /* +0x4E */ unsigned char row; /* an enemy's row, from its scene placement; the lowest row is the front
                                     (Battle_RecomputeActorMasks), 0x10 or more out of reach */
    /* +0x4F */ unsigned char idlePose;
    /* +0x50 */ short spoilA;
    /* +0x52 */ short spoilB;
    /* +0x54 */ unsigned short statusBits;
    /* +0x56 */ unsigned char hitAnimId; /* the command context's animId when the actor is targeted */
    unsigned char pad_57[0x1];
    /* +0x58 */ int gilReward;
    /* +0x5C */ int expReward;
    unsigned char pad_60[0x8];
} BattleCombatant;
/* The 0x68 stride and the members the image proves (status, flags, curHp), checked as ff7/battle.h checks
   its other records (offsetof spelled out: this header includes nothing). */
typedef char ff7_size_combatant[(sizeof(BattleCombatant) == 0x68) ? 1 : -1];
typedef char ff7_off_cmb_status[((int)&((BattleCombatant *)0)->status == 0x00) ? 1 : -1];
typedef char ff7_off_cmb_flags[((int)&((BattleCombatant *)0)->flags == 0x04) ? 1 : -1];
typedef char ff7_off_cmb_curhp[((int)&((BattleCombatant *)0)->curHp == 0x2C) ? 1 : -1];

/* Battle AI VM globals (gBattleAiGlobals, 0x94C bytes at 0x009AB0A0): the command being run, actor and target
   masks, formation id and round flags, the ten combatants and the AI script variables of each actor. */
typedef struct BattleAiGlobals {
    char commandIndex;
    char attackId;
    unsigned char savemapByte; /* +0x2: the byte Battle_AiVm_AccessSavemapByte moves to or from the savemap */
    unsigned char pad_03[0x1];
    char layout;
    unsigned char pad_05[0x2];
    char effectParam;
    unsigned char pad_08[0x2];
    short actorMask;
    unsigned short actorBit;
    short lastActorMask;
    short oppositeB;
    short sideBMatch;
    short opponentMask;
    short sideAMatch;
    unsigned short targetMask;
    short enemyMask;
    short targetableActorMask;
    unsigned char pad_1E[0x4];
    unsigned short flags;
    short elementMask;
    unsigned char pad_26[0x2];
    short formationId;
    short fullAttackId;
    unsigned short arenaFlags;
    short specialFlags;
    unsigned short atbPeriod;
    unsigned char pad_32[0x2];
    unsigned short messageArgs[2];          /* +0x34: the arguments Battle_QueueAiMessageText expands into an AI message */
    int gil;
    BattleCombatant combatants[10];         /* +0x3C, g_aCombatants */
    unsigned char actorVars[10][0x80];      /* +0x44C: each actor's AI variables (Battle_AiVm_ResolveAddress) */
} BattleAiGlobals;

/* Neo Bahamut summon per-face matrix slot: face centre and per-vertex offsets from it, plus flags. */
typedef struct BattleFxNeoBahamutFaceSlot {
    short pRelX;
    short pRelY;
    short pRelZ;
    unsigned char pad_06[0x2];
    short aRelX;
    short aRelY;
    short aRelZ;
    unsigned char pad_0E[0x2];
    short sRelX;
    short sRelY;
    short sRelZ;
    unsigned char pad_16[0x2];
    short uRelX;
    short uRelY;
    short uRelZ;
    unsigned char pad_1E[0x2];
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_26[0x2];
    int depth; /* the ordering-table depth of the projected face */
    unsigned char pad_2C[0x4];
    int flags;
} BattleFxNeoBahamutFaceSlot;

/* One point of the Death magic trail buffer; subdivision writes midpoints of neighbouring points. */
typedef struct BattleMagicDeathTrailPoint {
    short posX;
    short height;
    short posZ;
    unsigned char pad_06[0x2];
} BattleMagicDeathTrailPoint;

/* Per-fragment offset record of the Titan summon mesh: x, y and z are added to the summon centre. */
typedef struct BattleFxMeshShardSlot {
    short z;
    unsigned char pad_02[0x2];
    short x;
    short y;
} BattleFxMeshShardSlot;

/* PSX textured quad fragment: vertex offsets used by the Titan summon draw. */
typedef struct BattleFxPsxTexturedQuad {
    short flags;
    short vertex1;
    short vertex2;
    short vertex3;
    int colorWord;
    short uvWord0;
    short uvWord1;
    short offsetC;
    short offsetD;
} BattleFxPsxTexturedQuad;

/* Kjata branching trail quad: corner shorts with the transformed depth and flags. */
typedef struct BattleFxKjataTrailQuad {
    short x0;
    short y0;
    short z0;
    unsigned char pad_06[0x2];
    short x1;
    short y1;
    short z1;
    unsigned char pad_0E[0x2];
    short x2;
    short y2;
    short z2;
    unsigned char pad_16[0x2];
    short x3;
    short y3;
    short z3;
    unsigned char pad_1e[0x2];
    short centerX; /* +0x20 */
    short centerY; /* +0x22 */
    int pad_24; /* +0x24 */
    unsigned char pad_28[0x28];
    int depth;
    unsigned char pad_54[0x4];
    int flags;
} BattleFxKjataTrailQuad;

/* Typhon bouncing model record: position, vertical velocity and spin counters for the summon task. */
typedef struct BattleFxTyphonBouncingRecord {
    short modelIndex;
    short callCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
    unsigned char pad_10[0x4];
    unsigned char spinX;
    unsigned char spinY;
    unsigned char spinZ;
    unsigned char pad_17[0x1];
} BattleFxTyphonBouncingRecord;

/* Ramuh summon camera scratch record: a mode and a position, read by the camera script. */
typedef struct BattleFxCameraBasisScratch {
    short mode;
    short velocity;
    unsigned char pad_04[0xc];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_16[0xa];
    short frameCounter;
} BattleFxCameraBasisScratch;

/* Sting PSX mesh record: offsets into its resource data and a resource handle. */
typedef struct BattleFxStingMeshHeader {
    short vertexOffsetA;
    short vertexOffset1;
    short vertexOffsetC;
    short resourceOffset;
    union { int color0; CVECTOR color0Vec; };
    union { int color1; CVECTOR color1Vec; };
    union { int color2; CVECTOR color2Vec; };
    union {
        struct {
            short vertexSlotA;
            short vertexSlot1;
        };
        int color3;
        CVECTOR color3Vec;
    };
} BattleFxStingMeshHeader;

/* Shiva triangle scratch: vertex offsets from the triangle centroid, which element 2 holds. */
typedef struct BattleFxTriBasisScratch {
    short pDeltaX;
    short pDeltaY;
    short pDeltaZ;
    unsigned char pad_06[0x2];
    short qDeltaX;
    short qDeltaY;
    short qDeltaZ;
    unsigned char pad_0E[0x2];
} BattleFxTriBasisScratch;

/* Odin/Gungnir fading sprite task state: position, velocity and frame count. */
typedef struct BattleFxOdinSpriteTask {
    short frameOffset;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velX;
    short velY;
    short velZ;
} BattleFxOdinSpriteTask;

/* Scratch quad for the Daichi limit trail ribbon, rebuilt each frame. */
typedef struct YasuiLimitBreathOfTheEarthTrailScratch {
    short currentSampleA;
    short currentOffsetA;
    short currentSampleZA;
    unsigned char pad_06[0x2];
    short currentSampleB;
    short currentOffsetB;
    short currentSampleZB;
    unsigned char pad_0E[0x2];
    short nextSampleA;
    short nextOffsetA;
    short nextSampleZA;
    unsigned char pad_16[0x2];
    short nextSampleB;
    short nextOffsetB;
    short nextSampleZB;
    unsigned char pad_1E[0x2];
    int depth;
    unsigned char pad_24[0x4];
    int flags;
    int primitiveWord;
} YasuiLimitBreathOfTheEarthTrailScratch;

/* Odin Zantetsuken camera target: track-driven target position, blend and flags. */
typedef struct BattleFxOdinZCameraTarget {
    unsigned char pad_00[0x6];
    short scale;
    unsigned char pad_08[0x8];
    short trackCount;
    unsigned char pad_12[0x2];
    short blend;
    unsigned char pad_16[0xf];
    char flags;
    unsigned char pad_26[0x2];
    char tintR;
    char tintG;
    char tintB;
    unsigned char pad_2B[0x13b];
    short targetX;
    short targetY;
    short targetZ;
} BattleFxOdinZCameraTarget;

/* Alexander radial spark sprite record: position, damped velocity and frame counter. */
typedef struct BattleFxRadialLivewire {
    short spriteIndex;
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
} BattleFxRadialLivewire;

/* Limit-break spoke draw state: centre and spread offsets for each spoke. */
typedef struct BattleFxLimitSpokes {
    unsigned char pad_00[0x8];
    union {
        short vertex[25][4]; /* +0x8: x, y, z and a pad word per spoke end; vertex 0 is unused */
        Int64Pair vertexEnd[25]; /* the same spoke ends, copied whole */
        int depth; /* +0x8: the spoke particle's billboard depth, its matrix's t[2] >> 4 */
    };
    union {
        Int64Pair center; /* +0xd0: the point the spokes start from */
        struct {
            short centerX;
            short centerY;
            short centerZ;
            unsigned char pad_D6[0x2];
        };
    };
    short spreadX;
    short spreadY;
    union {
        int projected[26]; /* +0xdc: the screen word of each vertex, from index 1 */
        struct {
            short spreadZ;
            unsigned char pad_DE[0x2];
            int projectedCenter;
        };
    };
} BattleFxLimitSpokes;

/* Odin Gungnir debris sprite record: position, damped velocity and frame counter. */
typedef struct BattleFxOdinDebrisSprite {
    short spriteIndex;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxOdinDebrisSprite;

/* Swirling summon sprite task record: rotated offset, phase, angle and sprite index. */
typedef struct BattleFxSwirlingSprite {
    short ownerSlot;
    short index;
    short offsetX;
    short phase;
    short offsetY;
    unsigned char pad_0A[0x2];
    short angle;
    short phaseStep;
    unsigned char pad_10[0x4];
    short scale;
} BattleFxSwirlingSprite;

/* Battle stage floor flag table: one byte per floor part, set to 1 at init. */
typedef struct BattleStageFloorFlags {
    unsigned char pad_00[0xa];
    unsigned char stage0AFloor;
    unsigned char stage0BFloor;
    unsigned char pad_0C[0x6];
    unsigned char stage12Floor;
    unsigned char pad_13[0x8];
    unsigned char stage1BFloor;
    unsigned char stage1CFloor;
    unsigned char pad_1D[0xb];
    unsigned char partEnabled;
    unsigned char pad_29[0x1b];
    unsigned char stage44Floor;
    unsigned char stage45Floor;
    unsigned char stage46Floor;
    unsigned char stage47Floor;
    unsigned char pad_48[0x8];
    unsigned char stage50Floor;
    unsigned char stage51Floor;
    unsigned char pad_52[0x3];
    unsigned char stage55Floor;
} BattleStageFloorFlags;

/* Three 16-bit rotation angles that build a scaled Queig rotation matrix. */
typedef struct BattleFxEulerAngles16 {
    short angleX;
    short angleY;
    short angleZ;
} BattleFxEulerAngles16;

/* Alexander summon gradient strip: packed colour words for the strip quads. */
typedef struct BattleFxAlexanderGradient {
    int tag;
    union {
        int colorWord0;
        struct {
            unsigned char colorRGB[3];
            char code; /* the GPU command byte */
        };
    };
    unsigned char pad_08[0x4];
    int colorWord1;
    unsigned char pad_10[0x4];
    int colorWord2;
    unsigned char pad_18[0x4];
    int colorWord3;
} BattleFxAlexanderGradient;

/* Two offscreen render-object slot indices used by the screen-shatter effect. */
typedef struct BattleFxOffscreenSlotPair {
    int primarySlot;
    int secondarySlot;
} BattleFxOffscreenSlotPair;

/* Per-actor draw entry from Battle3d_GetActorDrawEntry; queued flag gates drawing. */
typedef struct BattleActorDrawEntry {
    int queued;
    int drawParam;
    int hasAttachment;
    int pass;
} BattleActorDrawEntry;

/* Effect slot for the Limit Matsuri timeline; holds step counter and actor index. */
typedef struct BattleFxLimitBloodfestTaskSlot {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x1a];
    short actorIndex;
} BattleFxLimitBloodfestTaskSlot;

/* Effect-slot record of the growing model task: frame counter and growth accumulator. */
typedef struct BattleFxGrowingModelSlot {
    short slotTag;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short growth;
    short growthStep;
} BattleFxGrowingModelSlot;

/* Effect-slot record of the ReDo falling fade model task: frame counter, Y position, model index. */
typedef struct BattleFxSpecialEnergyOrbFallingSlot {
    short slotTag;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angleZ;
    short modelIndex;
} BattleFxSpecialEnergyOrbFallingSlot;

/* Dokueki sprite that drifts and plays a frame animation. */
typedef struct BattleFxLiquidPoisonDriftingAnimatedSprite {
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
    short unusedParam;
    short slotIndex;
} BattleFxLiquidPoisonDriftingAnimatedSprite;

/* PSX gouraud quad primitive written word by word for the Altema gradient rows. */
typedef struct BattleFxPsxGouraudPrim {
    int tag;
    union {
        int color0;
        struct {
            unsigned char color0Rgb[3];
            unsigned char code;
        };
    };
    int vertex0;
    int color1;
    int vertex1;
    int color2;
    int vertex2;
    int color3;
    int vertex3;
} BattleFxPsxGouraudPrim;

/* Three-step scale history of the Ramuh scaled billboard sprite. */
typedef struct BattleFxScaleHistoryRing {
    int historyOldest;
    int historyMiddle;
    int historyNewest;
} BattleFxScaleHistoryRing;

/* Effect primitive header written at a node: flags and fixed fields. */
typedef struct BattleEffectPrimitive {
    int primitive;
    int flags;
    short clearedWordA;
    short startValue;
    short stateFlags;
    short clearedWordB;
} BattleEffectPrimitive;

typedef struct BattleLimitHighwindLoadedResTable {
    short posX; /* +0x0 */
    short posY; /* +0x2 */
} BattleLimitHighwindLoadedResTable;

/* Equipped weapon row, 0x18 bytes per entry, copied into the battle command context. */
typedef struct BattleWeaponActionRow {
    unsigned char targetFlags;
    unsigned char resolvedAttackId;
    unsigned char damageCalcByte;
    unsigned char accuracy;
    unsigned char displayValue;
    unsigned char criticalBonus; /* +0x05: added to the critical chance (Battle_RollCriticalHit) */
    unsigned char pad_06[0x2];
    unsigned short slotWords[3]; /* +0x08: per swing, the weapon's byte for it, plus 0x100 when its bit is set in the weapon's slot bits */
    unsigned short elementMask;
    unsigned short animParam;
    unsigned short specialFlags;
    int statusMask;
} BattleWeaponActionRow;

/* Fading sprite record of the Meikyo limit effect: a frame counter, a position and a per-frame velocity. */
typedef struct BattleFxClearTranquilFadeSprite {
    short recordTag;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxClearTranquilFadeSprite;

/* Data block of the Tifa ribbon emitter: three random spread factors scaled into the ribbon velocity. */
typedef struct BattleFxRibbonEmitterData {
    unsigned char pad_00[0x24];
    int orderingDepth;
    unsigned char pad_28[0xc];
    short originX;
    short originY;
    short originZ;
    unsigned char pad_3A[0x7e];
    int unusedSourceWord;
    int unusedCopiedWord;
    unsigned char pad_C0[0x4];
    int particleBase;
    int color;
    unsigned char pad_CC[0x4c];
    int spreadX;
    int spreadY;
    int spreadZ;
} BattleFxRibbonEmitterData;

/* The position at +0x4 of a Faiga effect task slot: x, y and z, or the same bytes as the 8-byte word the
 * tasks copy to the slots they spawn. */
typedef union BattleFxTaskPos {
    ShadowInt64Pair pair;
    struct {
        short x;
        short y;
        short z;
    };
} BattleFxTaskPos;

/* Task-slot record of the Faiga magic timeline; a step counter selects each phase. */
typedef struct BattleFxFire3TaskSlot {
    short ownerSlot;
    short step;
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x6];
    short radius;
} BattleFxFire3TaskSlot;

/* Draw packet of the Queig ground quad: a tag word, a command word and several short fields. */
typedef struct BattleFxGroundTilePacket {
    int tag;
    union {
        int command;
        struct {
            unsigned char rgb[3];
            char gpuCode;
        };
    };
    int screenXy0;
    short uv0;
    short clut;
    int screenXy1;
    short uv1;
    short tpage;
    int screenXy2;
    short uv2;
    unsigned char pad_1E[0x2];
    int screenXy3;
    short uv3;
} BattleFxGroundTilePacket;

/* Slots BattleFx_SpecialToxicSmogTimelineTask allocates for BattleFx_SpecialOsen{AnimatedSprite, 4 more}Task. */
typedef struct BattleFxToxicSmogTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x8];
    short phaseOrResIndex;
} BattleFxToxicSmogTimelineSpawnSlot;

/* Dokufuki emitter record: step counter, position and Y angle used for each emitted sprite. */
typedef struct BattleFxPoisonBreathEmit {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angleY;
    unsigned char pad_16[0x2];
    short speed;
} BattleFxPoisonBreathEmit;

/* Jibaku1 timeline record: step counter, jittered base position and the allocated record type. */
typedef struct BattleFxSelfDestructTimeline {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short spreadRadius;
    short ratio;
    short timer;
    short nextFrame;
    short actorIndex;
    short targetIndex;
} BattleFxSelfDestructTimeline;

/* Tan drifting sprite: frame index, position advanced by velocity, and a scale index. */
typedef struct BattleFxRefuseSpitDriftSprite {
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
    short scaleIndex;
} BattleFxRefuseSpitDriftSprite;

/* Odin Gungnir particle: position advanced by per-frame velocity, updated through a shared pool. */
typedef struct BattleFxOdinGungnirParticle {
    short state;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxOdinGungnirParticle;

/* Draw parameters for the Last Boss fragmented PSX mesh. */
typedef struct BattleFxSaferSephirothDeathMeshDraw {
    int meshData;
    int flags;
    unsigned char pad_08[0x2];
    short colorWord;
    short attrMask;
} BattleFxSaferSephirothDeathMeshDraw;

/* Trail ring of one Satan limit particle: write index, start index, count and eight trail-point addresses. */
typedef struct BattleFxLimitSatanSlamTrailRing {
    int writeIndex;
    int startIndex;
    int count;
    int points[8];
} BattleFxLimitSatanSlamTrailRing;

/* Limit-break animation bank: entry count and heap-allocated pointer table. */
typedef struct BattleLimitAnimBank {
    int heapBlock;
    void *skeleton;
    int animCount;
    int animPtrs;
    int entryCount;
    void *entryPtrs;
    int skelCount;
    int skelPtrs;
} BattleLimitAnimBank;

/* Queued damage-number record (14-byte stride) for the battle damage display. */
typedef struct BattleDamageDisplayRecord {
    short combatantIndex;  /* +0x0 */
    short damage;          /* +0x2 */
    short displayFlags;    /* +0x4 */
    unsigned short hp;     /* +0x6: the combatant's HP when the record was pushed; 0xFFFF = none */
    unsigned short mp;     /* +0x8: its MP; 0xFFFF = none */
    short displayValue;    /* +0xA */
    short secondaryValue;  /* +0xC */
} BattleDamageDisplayRecord;

/* 3x3 rotation matrix (nine shorts) built from an axis and an angle. */
typedef struct BattleRotationMatrix {
    short m00;
    short m01;
    short m02;
    short m10;
    short m11;
    short m12;
    short m20;
    short m21;
    short m22;
} BattleRotationMatrix;

/* Plane normal (three ints) for the signed height of a point above a plane. */
typedef struct BattlePlaneNormal {
    int normalX;
    int normalY;
    int normalZ;
} BattlePlaneNormal;

/* 32-byte BattleFx effect slot for the Dethramp rotation model task. */
typedef struct BattleFxDethRampSlot {
    short ownerSlot;
    short timer;
    short pos[3];
    unsigned char pad_0A[0x2];
    short scale;
} BattleFxDethRampSlot;

/* What a pointer no variable holds points at; only the fields the code reads. */
typedef struct BattleFxBzTimelineState {
    unsigned char pad_00[0x4];
    short rot04X;
    short rot04Y;
    short rot04Z;
    unsigned char pad_0A[0x2];
    short rot05X;
    short rot05Y;
    short rot05Z;
} BattleFxBzTimelineState;

/* Three shorts per textured debris shard, read through a u16 offset table. */
typedef struct BattleFxIce3MultiTexturedShard {
    short centroidX;
    short centroidY;
    short centroidZ;
} BattleFxIce3MultiTexturedShard;

/* Task slot record for a shrinking spinning model in the Faiga 3 effect. */
typedef struct BattleFxFire33SpinModel {
    short ownerSlot;
    short frameCount;
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x6];
    short radius;
    short angle;
    short angleStep;
} BattleFxFire33SpinModel;

/* Dengeki trail emitter slot: a step counter and three position accumulators, each advanced by a velocity. */
typedef struct BattleFxElectricShockTrailEmitter {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxElectricShockTrailEmitter;

/* ReDo trail model record: a frame counter that drives its trail, with three copied words. */
typedef struct BattleFxEnergyOrbTrailModel {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
} BattleFxEnergyOrbTrailModel;

/* Pink special emitter record: a center and a radius per axis, swept by a table angle while drawing sprites. */
typedef struct BattleFxHeartBreathParticleEmitter {
    short ownerSlot;
    short age;
    short centerX;
    short centerY;
    short centerZ;
    unsigned char pad_0A[0x2];
    short radiusX;
    short radiusY;
    short radiusZ;
    unsigned char pad_12[0x2];
    short rotationY;
    unsigned char pad_16[0x4];
    short angle;
} BattleFxHeartBreathParticleEmitter;

/* Hikari particle record: position jittered from the source entity and an arctangent angle. */
typedef struct BattleFxStareDownParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angle;
} BattleFxStareDownParticle;

/* Particle record for the Awa2 special: three base and amplitude pairs. */
typedef struct BattleFxHellBubblesParticleRecord {
    short ownerSlot;
    short age;
    short baseA;
    short baseB;
    short baseC;
    unsigned char pad_0A[0x2];
    short ampA;
    short ampB;
    short ampC;
    unsigned char pad_12[0x2];
    short rotY;
    unsigned char pad_16[0x4];
    short angle;
} BattleFxHellBubblesParticleRecord;

/* Particle record for the Kahun special: three base and amplitude pairs. */
typedef struct BattleFxPollenParticleRecord {
    short ownerSlot;
    short age;
    short baseA;
    short baseB;
    short baseC;
    unsigned char pad_0A[0x2];
    short ampA;
    short ampB;
    short ampC;
    unsigned char pad_12[0x2];
    short particleAngle;
    unsigned char pad_16[0x4];
    short angle;
} BattleFxPollenParticleRecord;

/* Particle record for the Suna particle field: three base and amplitude pairs. */
typedef struct BattleFxSandstormParticleRecord {
    short ownerSlot;
    short age;
    short baseA;
    short baseB;
    short baseC;
    unsigned char pad_0A[0x2];
    short ampA;
    short ampB;
    short ampC;
    unsigned char pad_12[0x2];
    short rotationY;
    unsigned char pad_16[0x4];
    short angle;
} BattleFxSandstormParticleRecord;

/* Anchor of the Hades sprite particle pool: a position and the step toward the next anchor. */
typedef struct BattleFxHadesAnchor {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_0E[0x2];
} BattleFxHadesAnchor;

/* Data-block pool for the Hades sprite particles: zeroed counters and two table-derived fields. */
#pragma pack(push, 1)
typedef struct BattleFxHadesParticlePool {
    int moveX;
    int minMoveX;
    unsigned char pad_08[0x8];
    int anchorIndex;
    int prevAnchorIndex;
    unsigned char pad_18[0x8];
    PsxMatrix combinedMatrix; /* +0x20 */
    short scalePrev2;
    short scaleMatrix01;
    short scaleMatrix02;
    short scaleMatrix10;
    short scalePrev;
    short scaleMatrix12;
    short scaleMatrix20;
    short scaleMatrix21;
    short scaleCurrent;
    int posX;
    int posY;
    int posZ;
    int transX;
    int transY;
    int transZ;
    unsigned char pad_68[0x6];
    int rotX;
    int rotY;
    int rotZ;
    unsigned char pad_7A[0x2];
    int spriteRecord;
    int spriteObject;
    int colorCmd;
    short age;
    unsigned char pad_8A[0x2];
    BattleFxHadesAnchor anchors[20];
} BattleFxHadesParticlePool;
#pragma pack(pop)

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxShivaForceFieldEntry {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short amplitudeX;
    short amplitudeY;
    short amplitudeZ;
} BattleFxShivaForceFieldEntry;

/* Three shorts copied into the Kjata timeline task position. */
typedef struct BattleFxPosition3s {
    short posX;
    unsigned char pad_02[0x2];
    short posY;
    unsigned char pad_06[0x2];
    short posZ;
} BattleFxPosition3s;

/* Falling streak particle record claimed by the Eva falling-streak emitter and task. */
typedef struct BattleFxGreatGospelStreakParticle {
    short state;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxGreatGospelStreakParticle;

/* Projected vertex record read for each ring point of the Lv4 Jibaku effect. */
typedef struct BattleFxProjectedPoint {
    unsigned char pad_00[0x8];
    int screenX;
    int screenY;
} BattleFxProjectedPoint;

/* Screen rectangle (x, y, width, height) queued with an offscreen stage request. */
typedef struct BattleFxScreenRect {
    short x;
    short y;
    short width;
    short height;
} BattleFxScreenRect;

/* Per-slot state of the CM growing model task, with a frame counter. */
typedef struct BattleFxLimitCosmoMemoryGrowState {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
} BattleFxLimitCosmoMemoryGrowState;

/* Request record passed to the PSX displaced-mesh draw routine. */
typedef struct BattleFxPsxDisplacedMeshRequest {
    int meshOffset;
    int flags;
    unsigned char pad_08[0x2];
    short shade;
    short flagBits;
} BattleFxPsxDisplacedMeshRequest;

/* Per-slot state of the SD textured quad pool task, with a pool index. */
typedef struct BattleFxLimitStardustRayQuadState {
    short ownerSlot;
    short index;
} BattleFxLimitStardustRayQuadState;

/* Debaria shatter effect record holding a mesh payload pointer and fragment state. */
typedef struct BattleFxDeBarrierShatter {
    int meshPayload;
    unsigned char pad_04[0x20];
    int fragNo;
    unsigned char pad_28[0x4];
    int bufBank;
} BattleFxDeBarrierShatter;

/* Club1 pulsing model task record for one target actor. */
typedef struct BattleFxClubCardPulsingModel {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short extent;
    short radius;
    short targetIndex;
} BattleFxClubCardPulsingModel;

/* Queir shard burst record that points at a relative mesh payload. */
typedef struct BattleFxQuake2ShardBurst {
    int meshPayload;
    unsigned char pad_04[0x20];
    int shardIndex;
} BattleFxQuake2ShardBurst;

/* Grav5 PSX mesh draw parameters: mesh payload pointer and shading inputs. */
typedef struct BattleFxDemi3MultiMeshDraw {
    int meshPayload;
    unsigned char pad_04[0x6];
    short shade;
    short flagBits;
} BattleFxDemi3MultiMeshDraw;

/* Tornado timeline task record with a step counter and an allocation type. */
typedef struct BattleFxSwirlColumnTask {
    short ownerSlot;
    short step;
    short recordType;
    unsigned char pad_06[0x2];
    Int64Pair origin;
} BattleFxSwirlColumnTask;

/* Weapon Beam 1 beam model: stage counter that selects the beam's drawing phase. */
typedef struct BattleFxWeaponEmeraldBeamBeam {
    short ownerSlot; /* +0x0 */
    short stage;
    unsigned char pad_04[0x8];
    short thicknessScale;
} BattleFxWeaponEmeraldBeamBeam;

/* Frog two-phase sprite task slot: animation frame counter and phase selector. */
typedef struct BattleFxFrogTwoPhaseSlot {
    short ownerSlot; /* +0x0 */
    short frameIndex;
    unsigned char pad_04[0x16];
    short phase;
} BattleFxFrogTwoPhaseSlot;

/* Reito per-face offset mesh draw request: mesh data, draw flags, shade and texture flags. */
typedef struct BattleFxFreezeBreathDrawRecord {
    int modelData;
    int drawFlags;
    unsigned char pad_08[0x2];
    short shade;
    short textureFlags;
} BattleFxFreezeBreathDrawRecord;

/* Roket expanding model task slot: growth step counter. */
typedef struct BattleFxHellBomberGrowSlot {
    short ownerSlot;
    short growStep;
    short posX;
    short posY;
    short posZ;
} BattleFxHellBomberGrowSlot;

/* Oil variant sprite task slot: frame counter and variant index. */
typedef struct BattleFxOilSpriteSlot {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x10];
    short variant;
} BattleFxOilSpriteSlot;

/* Nadare timeline task slot: timeline step counter. */
typedef struct BattleFxAvalancheTimelineSlot {
    short ownerSlot;
    short timelineStep;
} BattleFxAvalancheTimelineSlot;

/* Smog animated sprite task slot: frame counter and variant index. */
typedef struct BattleFxSmogSpriteSlot {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variant;
} BattleFxSmogSpriteSlot;

/* PSX primitive output cursor passed through the Hades mesh height-clipped draw helpers. */
typedef struct BattleFxHadesPsxDrawCursor {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short grayShade;
    short texCoord0;
    short clut;
    short texCoord1;
    short texCoord2;
} BattleFxHadesPsxDrawCursor;

/* Counter-spinning model record of the Phoenix summon: a frame counter and a translation vector. */
typedef struct BattleFxCounterSpinState {
    short ownerSlot;
    short frameCount;
    short x;
    short y;
    short z;
} BattleFxCounterSpinState;

/* Four 16-bit values copied from a sting offscreen request into its pair record. */
typedef struct BattleFxRubyTentaclesOffscreenRecord {
    short rectX;
    short rectY;
    short rectW;
    short rectH;
} BattleFxRubyTentaclesOffscreenRecord;

/* Effect-slot record driving the eased growing model of the Hamblow limit. */
typedef struct BattleFxEasedTransformSlot {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0xa];
    short scale;
} BattleFxEasedTransformSlot;

/* Effect-100 slot for the Jaki per-target timeline; phase step. */
typedef struct BattleFxLimitSealEvilTargetSlot {
    short actorIndex;
    short step;
    short posX; /* the target bone position Battle_GetBoneWorldPos writes */
    short posY;
    short posZ;
} BattleFxLimitSealEvilTargetSlot;

/* B2 final-flash model slot; phase steps through 0 to 3. */
typedef struct BattleFxBlasterFlashSlot {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
} BattleFxBlasterFlashSlot;

/* Descriptor for drawing a PSX displaced mesh: mesh data, flags and shading. */
typedef struct BattleFxPsxDisplacedMeshDesc {
    int meshData;
    int flags;
    unsigned char pad_08[0x2];
    short shade;
    short polyFlags;
} BattleFxPsxDisplacedMeshDesc;

/* 8-byte word pair used to copy packed bone positions across stack frames and task slots. */
typedef struct BattleBonePosCopy {
    int x;
    int y;
} BattleBonePosCopy;

typedef union BattleBonePosFrame {
    short s[4];
    BattleBonePosCopy pair;
} BattleBonePosFrame;

/* Stage descriptor: a stage type byte and a node count byte from the stage info block. */
typedef struct BattleStageDescriptor {
    unsigned char stageType;
    unsigned char nodeCount;
} BattleStageDescriptor;

/* Hit mark script step slot over the hit mark sprite array. */
typedef struct BattleFxHitMarkScriptSlot {
    short flags;               /* +0x00: selector & 0xFF00 (0x100 mirrors the sprites); -1 retires the slot */
    short frame;               /* +0x02: frames since the spawn, compared with the script's frame bytes */
    unsigned char position[8]; /* +0x04: the world position, copied from pPosition */
    signed char *scriptCursor; /* +0x0C: an entry of g_BattleFxHitmarkScriptTable */
    unsigned char pad_10[0x2];
    short scale;               /* +0x12: scales the sprites' offsets and size */
    unsigned char pad_14[0x4];
    short pushLength;          /* +0x18: copied into each spawned sprite's pushLength */
} BattleFxHitMarkScriptSlot;

/* Hado spin model record: position orbiting a base point at a fixed height. */
typedef struct BattleFxEnergyWaveSpinSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short spriteVariant;
} BattleFxEnergyWaveSpinSlot;

/* Meteo-R angular texture gradient state record. */
typedef struct BattleFxMeteorainGradientState {
    int *meshHeader; /* the vertex block's byte size; the vertices and the quad records follow it */
    unsigned char pad_04[0x8];
    int unusedReturn;
    int highWordMask;
    unsigned char pad_14[0x84];
    VECTOR refDir;
    VECTOR centroid;
    VECTOR cross; /* cross.vy's sign picks the turning direction of the angle */
} BattleFxMeteorainGradientState;

/* Textured quad primitive record filled by the Faiga 3 PSX magic draw. */
typedef struct BattleFxFire3MultiPsxQuad {
    int tag;
    unsigned char rgb[0x3];
    char primCode;
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    int flags;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxFire3MultiPsxQuad;

/* Shard record header read by the Queig shard record builders. */
typedef struct BattleFxShardTriSlot {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
} BattleFxShardTriSlot;

/* Shard record header read by the Queir shard record builders. */
typedef struct BattleFxQuake2ShardRecord {
    unsigned short shardOffset0;
    unsigned short shardOffset1;
    unsigned short shardOffset2;
    unsigned short shardOffset3;
} BattleFxQuake2ShardRecord;

/* Bio rising fade sprite record: position Y, size, rise speed and frame counter. */
typedef struct BattleFxBioRisingFadeSprite {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short size;
    short riseSpeed;
} BattleFxBioRisingFadeSprite;

/* Yami projectile primitive record: packed command word and colour bytes. */
typedef struct BattleFxPolyF4Prim {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char primCode;
} BattleFxPolyF4Prim;

/* Barsac fading sprite record with a frame counter that drives its fade. */
typedef struct BattleFxBerserkFadeSprite {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x14];
    short ringIndex;
} BattleFxBerserkFadeSprite;

/* Thunder 4 grow-hold-fade record; its frame counter drives the sprite request. */
typedef struct BattleFxBolt3MultiGrowRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
} BattleFxBolt3MultiGrowRecord;

/* Special Syaku particle record: position, velocity and the actor index it belongs to. */
typedef struct BattleFxScorchParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0xc];
    short actorIdx;
} BattleFxScorchParticle;

/* Shield projectile trail record: position and velocity copied from the projectile. */
typedef struct BattleFxShieldTrailRecord {
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
} BattleFxShieldTrailRecord;

/* Heart 1 bone particle emitter record: random-walk position updated each tick. */
typedef struct BattleFxHeartBoneEmitter {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x3];
    char cellRow;
    short cellWord;
    short scale;
    unsigned char pad_12[0x4];
    short spriteFlags;
} BattleFxHeartBoneEmitter;

/* Gesui1 animated sprite task record: frame counter and resource variant index. */
typedef struct BattleFxSewerTsunamiAnimSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x10];
    short variant;
} BattleFxSewerTsunamiAnimSprite;

/* Tele animated sprite task record: frame counter and resource variant index. */
typedef struct BattleFxTailLaserAnimSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variant;
} BattleFxTailLaserAnimSprite;

/* Per-target Mgbeam path: start and end positions whose difference gives the beam direction. */
typedef struct BattleFxBeamGunPath {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short targetIndex;
} BattleFxBeamGunPath;

/* Offscreen actor request read by the split task: position words from the caller and a projected result. */
typedef struct BattleFxActorOffscreenRequest {
    short posX;
    short posY;
    short projectResult;
} BattleFxActorOffscreenRequest;

/* Per-actor record at a 0x1AEC stride, with the x and z values used in the mesh height test. */
typedef struct BattleFxSatanSlamActorSurface {
    short posX;
    short height;
    short posZ;
} BattleFxSatanSlamActorSurface;

/* Quad record whose UV and angle fields are assigned by the night quad UV routine. */
typedef struct BattleFxNightmareUvScratch {
    int dataPtr;
    unsigned char pad_04[0x8];
    int unusedReturn;
    int highWordMask;
    unsigned char pad_14[0x84];
    int refAxisX;
    int refAxisY;
    int refAxisZ;
    unsigned char pad_A4[0x4];
    int sumX;
    int sumY;
    int sumZ;
    unsigned char pad_B4[0x4];
    int crossX;
    int crossY;
} BattleFxNightmareUvScratch;

/* Black puff record for the S-Beam effect, with a frame counter. */
typedef struct BattleFxSatelliteBeamBlackPuff {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
} BattleFxSatelliteBeamBlackPuff;

/* Battle model bank: skeleton and animation arrays drawn for a frame. */
typedef struct BattleModelBank {
    int fileData;
    void *skeleton;
    int animCount;
    void *anims;
    int secondAnimCount;
    void *secondAnims;
    int weaponCount;           /* +0x18 */
    void *weaponSkeletons;     /* +0x1C: weaponCount skeleton pointers */
    union {
        BattleModelDrawState drawState; /* +0x20 */
        BattleActorDrawState draw;
    };
} BattleModelBank;

/* Blue Matra animated sprite task state with a frame counter. */
typedef struct BattleFxMatraMagicAnimSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
} BattleFxMatraMagicAnimSprite;

/* Blue Dethfo animated sprite task state with a frame counter. */
typedef struct BattleFxDeathForceAnimSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
} BattleFxDeathForceAnimSprite;

/* Fading sprite velocity record for the blue Howin effect. */
typedef struct BattleFxWhiteWindFadeState {
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxWhiteWindFadeState;

/* Radial sprite emitter of the Sting effect; each particle is placed on a circle around its base. */
typedef struct BattleFxRubyTentaclesRadialEmitter {
    short ownerSlot;
    short frame;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
} BattleFxRubyTentaclesRadialEmitter;

/* Task slot of a sprite drawn at a position its spawner copies in (posX..posZ at +4): the Highwind,
 * Jinrai, Hyper Jump, CM and other sprite tasks read their slot through it. */
typedef struct BattleFxPositionedSpriteSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
} BattleFxPositionedSpriteSlot;

/* Billboard sprite task state for the SD limit with a frame counter. */
typedef struct BattleFxStardustRayBillboardSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short scale;
} BattleFxStardustRayBillboardSprite;

/* Colour bytes (r, g, b) for one Hades ribbon segment quad. */
typedef struct BattleFxRibbonColor {
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
} BattleFxRibbonColor;

/* Four shorts scaled by frame values when requesting the stage offscreen render. */
typedef struct BattleEffectStageCoords {
    short offsetX;
    short offsetY;
    short stageWidth;
    short stageHeight;
} BattleEffectStageCoords;

/* Mesh face record in the Dethper normal pass; its offset word points at a vertex. */
typedef struct BattleDethperMeshRecord {
    unsigned short nextVertexOffset;
    unsigned short vertexOffset;
    unsigned short prevVertexOffset;
} BattleDethperMeshRecord;

/* Effect-table record for the Freeze damped-growth model spawner, with a frame index. */
typedef struct BattleFreezeGrowthRecord {
    short ownerSlot;
    short frameIndex;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
} BattleFreezeGrowthRecord;

/* Effect-table state for the Comet expanding model pair, writing translation into its matrix. */
typedef struct BattleCometExpandingPairState {
    short ownerSlot;
    short frameIndex;
    short transX;
    unsigned char pad_06[0x2];
    short transZ;
} BattleCometExpandingPairState;

/* Twin-sprite layer record of the Dejon magic task; its counter picks the palette upload. */
typedef struct BattleFxRemoveLayerSprite {
    short ownerSlot;
    short layerIndex;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
} BattleFxRemoveLayerSprite;

/* Randomised offset record for the Thunder4 flash and billboard sprites. */
typedef struct BattleFxBolt3MultiParticle {
    unsigned char pad_00[0x8];
    short posX;
    short posY;
    short posZ;
} BattleFxBolt3MultiParticle;

/* Netu rising-fade sprite record; its counter steps the fade value down. */
typedef struct BattleFxHeatWaveRisingFadeSprite {
    short ownerSlot;
    short fadeStep;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variant;
} BattleFxHeatWaveRisingFadeSprite;

/* Bless frame-table sprite record; its counter walks the frame table. */
typedef struct BattleFxRadiantBreathFrameSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short animIndex;
} BattleFxRadiantBreathFrameSprite;

/* Kama variant sprite record; its counter feeds the color data and the variant table. */
typedef struct BattleFxFlyingSickleVariantSprite {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x14];
    short variantIndex;
} BattleFxFlyingSickleVariantSprite;

/* GPU packet for one gust segment: a tag word plus four colour words. */
typedef struct BattleFxGustFourColorPacket {
    int packetTag; /* +0x0 */
    int color0;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxGustFourColorPacket;

/* Three-int scale vector for the Queig scaled rotation matrix. */
typedef struct BattleFxFixedScaleVec {
    int scaleX;
    int scaleY;
    int scaleZ;
} BattleFxFixedScaleVec;

/* Parameter block copied into a sprite record by BattleFx_BzEmitSprite. */
typedef struct BattleFxBzSpriteParams {
    union {
        int packedXY;
        struct {
            short x;
            short y;
        };
    };
    char u;
    unsigned char pad_05[0x1];
    char v;
    unsigned char pad_07[0x1];
    int packedSize;
    int colorCmd;
    unsigned char pad_10[0x2];
    short clut;
} BattleFxBzSpriteParams;

/* A Bahamut Zero trail sprite's texture scroll origin: the U and V offsets the trail task subtracts its scroll from. */
typedef struct BattleFxBzTrailOrigin {
    short u;
    short v;
} BattleFxBzTrailOrigin;

/* Jaki billboard sprite particle with position, velocity and angle, updated by BattleFx_LimitSealEvilBillboardSpriteParticle. */
typedef struct YasuiSealEvilParticle {
    short limitJakiBillboardParticleWord;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short angle;
} YasuiSealEvilParticle;

/* Entry of the composed matrix that drives the Alexander morphing mesh, read as short words. */
typedef struct BattleFxAlexanderMatrixEntry {
    unsigned char pad_00[0x2];
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
} BattleFxAlexanderMatrixEntry;

/* Position and velocity of the Phoenix growing model, advanced by its task each frame. */
typedef struct BattleFxPhoenixGrowState {
    union {
        SVECTOR point;
        struct {
            short pointX;
            short transformOffset;
            short pointZ;
            unsigned char pad_06[0x2];
        };
    };
    union {
        VECTOR pos;
        struct {
            int posX;
            int posY;
            int posZ;
            unsigned char pad_14[0x4];
        };
    };
    union {
        VECTOR velocity;
        struct {
            int velocityX;
            int velocityY;
            int velocityZ;
            unsigned char pad_24[0x4];
        };
    };
    int rotTransFlag;
} BattleFxPhoenixGrowState;

/* Limit Meikyo gradient line record: a step counter, the line's origin and its rotation angles. */
typedef struct BattleFxClearTranquilGradientLine {
    short recordTag;
    short step;
    short lineX;
    short lineY;
    short lineZ;
    unsigned char pad_0A[0x2];
    SVECTOR rotation;
} BattleFxClearTranquilGradientLine;

/* Offset from the Tifa target bone to the sound position, used by the Limit orbit and tumble effects. */
typedef struct BattleLimitTifaRelativeVec {
    short x;
    short y;
    short z;
    short flag; /* 1 while the orbit is in its first phase */
} BattleLimitTifaRelativeVec;

/* Queig shard quad record: four vertex offsets, flags and two shorts. */
typedef struct BattleFxShardQuadSlot {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
    int uv0Clut;
    unsigned short flags;
    short uv2;
    short uv3;
} BattleFxShardQuadSlot;

/* Faiga Gouraud quad record: four vertex offsets and four int values. */
typedef struct BattleFxFaigaGouraudQuadRecord {
    unsigned short vertex0Offset;
    unsigned short vertex1Offset;
    unsigned short vertex2Offset;
    unsigned short vertex3Offset;
    int color0;
    int color1;
    int color2;
    int color3;
} BattleFxFaigaGouraudQuadRecord;

/* Rising sprite particle: frame counter and billboard angle, with a decaying angular step in the next record. */
typedef struct BattleFxRisingSpriteParticle {
    short angularVelocity;
    short frameCount;
    unsigned char pad_04[0x2];
    short angle;
    unsigned char pad_08[0x6];
} BattleFxRisingSpriteParticle;

/* Kjata summon trail record: a colour index and two offsets copied into draw triangles. */
typedef struct BattleSummonKjataTrailRecord {
    unsigned short colorIndex;
    short vertexOffsetA;
    short vertexOffsetB;
} BattleSummonKjataTrailRecord;

/* Per-slot state for BattleFx_BzDrawSlot15: a frame counter and a speed that advance each frame. */
typedef struct YasuiBzSlotState {
    short ownerSlot;
    short frame;
    short speed;
    unsigned char pad_06[0x2];
    short posZ;
} YasuiBzSlotState;

/* Textured triangle record: three vertex byte offsets and flags copied into a PSX primitive. */
typedef struct BattleFxTexturedTriRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned char pad_06[0x2];
    int uv0Clut;
    unsigned short uv1;
    short uv2;
} BattleFxTexturedTriRecord;

/* Shaded debris triangle record for the Brizag2 draw: vertex offsets and three ints. */
typedef struct BattleFxBrizagShadedTriRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned char pad_06[0x2];
    int color0;
    int color1;
    int color2;
} BattleFxBrizagShadedTriRecord;

/* Float vertex blended toward a source vertex by the Slou blend. */
typedef struct BattleFxSlowBlendVertex {
    float x;
    float y;
    float z;
} BattleFxSlowBlendVertex;

/* Light rig offsets of the Neo Bahamut summon, copied into the rig and scene actor each frame. */
typedef struct BattleFxNeoBahamutLightRig {
    short rigX;
    short rigY;
    short rigZ;
} BattleFxNeoBahamutLightRig;

/* Start vertex of the Bahamut blend polygon, lerped toward a target point by a blend factor. */
typedef struct BattleFxBahamutBlendVertex {
    float startX;
    float startY;
    float startZ;
} BattleFxBahamutBlendVertex;

/* A 0x34-byte model node (Battle_SetNodeParentMatrix, Battle3d_ComposeNodeWithParent): its GTE
 * matrix (3x3 rotation, 0x1000 = 1.0, then the translation), its rotation angles (0x1000 a
 * turn), its position, and the parent matrix it is composed with. A battle actor's root node
 * (BattleActor +0x140) and bone nodes have this layout. */
#pragma pack(push, 1)
typedef struct Battle3dNode {
    union {
        struct {
            short matrix[9];             /* +0x00 */
            int translation[3];          /* +0x12 */
        };
        PsxMatrix psxMatrix;
    };
    union {
        struct {
            short rotX;                  /* +0x1E */
            short yaw;                   /* +0x20 */
            short rotZ;                  /* +0x22 */
            unsigned char pad_24[0x2];
        };
        SVECTOR rot;
    };
    union {
        struct {
            short posX;                  /* +0x26 */
            short posY;                  /* +0x28 */
            short posZ;                  /* +0x2A */
            unsigned char pad_2c[0x4];
        };
        struct {
            SVECTOR pos;
            unsigned char pad_2e[0x2];
        };
    };
    int parentMatrix;            /* +0x30 */
} Battle3dNode;
#pragma pack(pop)

/* hub BattleActor begin */
/* One battle actor's 3D model state (g_BattleActorRecords, 10 x 0x1AEC at 0x00be1178; slot 3 also carries summon and effect models). +0x140..+0x173 is the actor's root model node and +0x174 the bone nodes, 0x34 bytes each (Battle_SetNodeParentMatrix). */
#pragma pack(push, 1)
struct BattleActor {
    short modelId; /* +0x0 */
    short scriptId; /* +0x2: index into the actor's animation script table; compared with its idle script */
    short state; /* +0x4 */
    short scale; /* +0x6: uniform scale, 0x1000 = 1.0 */
    unsigned short scaleX; /* +0x8: x scale, 0x1000 = 1.0 (from the model record) */
    unsigned short scaleY; /* +0xa: y scale, 0x1000 = 1.0 (from the model record) */
    short modelSlot; /* +0xc */
    short animId; /* +0xe: current animation, from the script */
    short boneCount; /* +0x10 */
    short radius; /* +0x12: size used to place effects and to space two actors */
    short tintPhase; /* +0x14: status-tint pulse phase, 0x1000 a turn */
    short rootYaw; /* +0x16 */
    short facing; /* +0x18: home direction; yaw returns to it */
    short burstOffsetA; /* +0x1a */
    short burstOffsetB; /* +0x1c */
    unsigned char pad_1e[0x4];
    unsigned char effectId; /* +0x22: the running action's effect id; indexes its category's dispatch table */
    unsigned char effectCategory; /* +0x23: the running action's effect category (2: magic); selects the dispatch table */
    char tintPulse; /* +0x24: status-tint pulse counter */
    char drawFlags; /* +0x25: bit 0: no target indicator, bit 2: no overlay icons, bit 3: tint pulses */
    char isDirty; /* +0x26 */
    unsigned char modelFlags; /* +0x27 */
    char tintR; /* +0x28: status tint, from the tint table */
    char tintG; /* +0x29 */
    char tintB; /* +0x2a */
    char targetBone; /* +0x2b: bone that effects aim at */
    unsigned char pad_2c[0x4];
    char bone30; /* +0x30 */
    char primaryBone; /* +0x31: bone for particle bursts and trails */
    char secondaryBone; /* +0x32: second bone for particle bursts */
    char bone33; /* +0x33 */
    char bone34; /* +0x34 */
    char altBone; /* +0x35 */
    char bone36; /* +0x36 */
    char bone37; /* +0x37 */
    unsigned char pad_38[0x2];
    char bone3a; /* +0x3a */
    unsigned char animRunning; /* +0x3b */
    unsigned char scriptPos; /* +0x3c: read position in the animation script */
    unsigned char waitFrames; /* +0x3d */
    unsigned char stateFlags; /* +0x3e: bit 0: frog model, bit 1/2: frozen by a summon, bit 5: status changed */
    char fadeFlags; /* +0x3f: bit 3: fading */
    unsigned char pad_40[0x34];
    int animTicks; /* +0x74 */
    unsigned char pad_78[0xc8];
    union { /* +0x140 */
        struct {
            unsigned char rootNode[0x1e]; /* +0x140: first 0x1e bytes of the root model node */
            short rotX; /* +0x15e: root node rotation, 0x1000 a turn */
            short yaw; /* +0x160: root node rotation about y */
            short rotZ; /* +0x162 */
            unsigned char pad_164[0x2];
            union {
                struct {
                    short posX; /* +0x166: root node position */
                    short posY; /* +0x168 */
                    short posZ; /* +0x16a */
                    unsigned char pad_16c[0x2];
                };
                Int64Pair posPacked;
            };
            unsigned char pad_16e[0x2];
            int rootParentMatrix; /* +0x170: root node's parent matrix */
        };
        Battle3dNode root;
    };
    union { /* +0x174 */
        struct {
            unsigned char boneNodes[0x20]; /* +0x174: bone nodes, 0x34 bytes each, up to the first one this layout splits out */
            int bone0Yaw; /* +0x194 */
            unsigned char pad_198[0x2];
            short bone0PosX; /* +0x19a: bone node 0 position */
            short bone0PosY; /* +0x19c */
            short bone0PosZ; /* +0x19e */
            unsigned char pad_1a0[0xd8];
            unsigned char boneNodes5[0x910]; /* +0x278: bone nodes from the sixth on */
            short spinAngle; /* +0xb88 */
            unsigned char pad_b8a[0x6];
            short savedPosY; /* +0xb90 */
            unsigned char pad_b92[0xa];
        };
        Battle3dNode nodes[50];
    };
    int nextAnimBank; /* +0xb9c */
    int animBank; /* +0xba0: animation bank in use */
    int animFrame; /* +0xba4: frame in the animation */
    struct BattleActorModel *model; /* +0xba8: model data */
    union { /* +0xbac */
        struct {
            float boneWorldPosX; /* +0xbac: world position of the bone Battle_GetBoneWorldPos last computed */
            float boneWorldPosY; /* +0xbb0 */
            float boneWorldPosZ; /* +0xbb4 */
            unsigned char pad_bb8[0x24c];
        };
        struct {
            float x;
            float y;
            float z;
        } boneWorldPos[50];
    };
    union { /* +0xe04 */
        unsigned char worldMatrix[0xcc0]; /* +0xe04: world matrix, then the attach matrices from +0xe44 (0x40 each) */
        struct {
            unsigned char worldMatrixBytes[0x40]; /* +0xe04 */
            unsigned char attachMatrices[0xc80]; /* +0xe44 */
        };
    };
    int useSecondAngleTable; /* +0x1ac4 */
    int deathActive; /* +0x1ac8: set while the death sink runs; cleared with the next 0x1c bytes */
    float deathVelocityX; /* +0x1acc */
    float deathSpinAngle; /* +0x1ad0 */
    float deathVelocityZ; /* +0x1ad4 */
    unsigned char pad_1ad8[0x4];
    float sinkOffset; /* +0x1adc */
    unsigned char pad_1ae0[0x4];
    int attachActor; /* +0x1ae4: actor and bone this model is attached to (Battle3d_GetActorAttachMatrix) */
    int attachBone; /* +0x1ae8 */
};
#pragma pack(pop)
/* hub BattleActor end */

/* Camera targets on an actor: the eye and the focus offset of one camera view (BattleActorState::cameraOffsets). */
#pragma pack(push, 1)
typedef struct BattleActorCameraOffsets {
    short eyeX; /* +0x0 */
    short eyeY; /* +0x2 */
    short eyeZ; /* +0x4 */
    short focusX; /* +0x6 */
    short focusY; /* +0x8 */
    short focusZ; /* +0xa */
} BattleActorCameraOffsets;
#pragma pack(pop)

/* hub BattleActorState begin */
#pragma pack(push, 1)
struct BattleActorState {
    int status; /* +0x0 */
    unsigned short ratioX; /* +0x4 */
    unsigned short ratioY; /* +0x6 */
    unsigned short ratioZ; /* +0x8 */
    short ratioExtra; /* +0xa */
    unsigned short flags; /* +0xc */
    union { /* +0xe */
        unsigned char statusOffsetArray[0x18];
        struct {
            short statusOffsetsA[6];
            short statusOffsetsB[6];
        };
        BattleActorCameraOffsets cameraOffsets[2]; /* the camera's eye and focus offsets, one record per view */
    };
    short posY; /* +0x26 */
    short bindIndex; /* +0x28 */
    short spriteY; /* +0x2a */
    short yaw; /* +0x2c */
    short mirrorWord; /* +0x2e */
    short heightBias; /* +0x30 */
    char frogModelIndex; /* +0x32 */
    char motionState; /* +0x33 */
    char frameCounter; /* +0x34 */
    char hidden; /* +0x35 */
    char timer; /* +0x36 */
    char drawEnabled; /* +0x37 */
    short poseAngleA; /* +0x38 */
    short poseAngleB; /* +0x3a */
    short poseAngleC; /* +0x3c */
    short power; /* +0x3e */
    Battle3dNode node; /* +0x40: the damage number's model node */
};
#pragma pack(pop)
/* hub BattleActorState end */

/* One battle task slot, 32 bytes: 100 main ones in g_BattleTaskSlotBuffer.mainSlots, run by
 * Battle_RunMainTaskSlots, and 60 secondary ones in .secondarySlots. Battle_AllocateResourceSlot stores the
 * allocating task's slot index at +0, and a task writes 0xFFFF there to end; +2 counts the task's
 * runs. The rest is the effect's own: each task reads it through its own
 * BattleFx<Effect><Task>Slot, and a task that reads only these two shorts reads BattleTaskSlot
 * itself. The callbacks are in g_BattleMainTaskCallbacks. */
typedef struct BattleTaskSlot {
    short ownerSlot;             /* +0x00 */
    short frameCounter;          /* +0x02 */
    union {
        unsigned char data[0x1C];    /* +0x04 */
        struct {
            short duration;
            short actor;
        } scaleRamp;
        struct {
            short delay;
            short actorWord;
            union {
                short actor;
                char actorByte;
            };
            short targetParam;
            short otherActor;
            short damage;
            union {
                int event;
                short eventSfx;
            };
            int packed;
            char mode;
            char commitByte;
        } hitReaction;
    };
} BattleTaskSlot;

/* One party member's row on the battle result screen (Battle_UpdateExpResultRows). */
typedef struct BattleRewardExpRow {
    int remainingExp;            /* +0x00: exp still needed for the next level */
    int displayedExp;            /* +0x04 */
    unsigned char expBar;        /* +0x08: the bar's fill, 0..61 */
    unsigned char displayedLevel; /* +0x09 */
    short limitUnlocks;          /* +0x0A: what Kernel_UpdateLimitUnlocks returned */
} BattleRewardExpRow;

/* hub BattleOverlayTaskSlot begin */
#pragma pack(push, 1)
struct BattleOverlayTaskSlot {
    short ownerSlot; /* +0x0 */
    short state; /* +0x2 */
    short sinkCountdown; /* +0x4 */
    short sfxPan; /* +0x6 */
    short actorId; /* +0x8 */
    short battleActorIndex; /* +0xa */
    short velocityX; /* +0xc */
    short velocityZ; /* +0xe */
    int duration; /* +0x10 */
    int velocityY; /* +0x14 */
    char timer; /* +0x18 */
    char spawnArg; /* +0x19 */
    char targetRef; /* +0x1a */
    unsigned char pad_1b[0x5];
};
#pragma pack(pop)
/* hub BattleOverlayTaskSlot end */

/* hub BattleAuxTaskSlot begin */
#pragma pack(push, 1)
struct BattleAuxTaskSlot {
    short ownerSlot; /* +0x0 */
    short timerWord; /* +0x2 */
    short countdown; /* +0x4 */
    short positionStep; /* +0x6 */
    short angleStep; /* +0x8 */
    short radius; /* +0xa */
    short orbitAngle; /* +0xc */
    short radiusStep; /* +0xe */
    int cameraValue; /* +0x10 */
    unsigned char pad_14[0x4];
    char channel; /* +0x18 */
    unsigned char pad_19[0xf];
};
#pragma pack(pop)
/* hub BattleAuxTaskSlot end */

/* hub BattleMagicIce3Fragment begin */
#pragma pack(push, 1)
struct BattleMagicIce3Fragment {
    union {
        ShadowInt64Pair position; /* +0x0: the fragment's offset, copied together with yTally */
        struct {
            short positionX; /* +0x0 */
            short positionY; /* +0x2 */
            short positionZ; /* +0x4 */
            short yTally; /* +0x6 */
        };
    };
    short rotationX; /* +0x8 */
    short rotationY; /* +0xa */
    short rotationZ; /* +0xc */
    short fadeCounter; /* +0xe */
    short velocityX; /* +0x10 */
    short velocityY; /* +0x12 */
    short velocityZ; /* +0x14 */
    short accelerationY; /* +0x16 */
};
#pragma pack(pop)
/* hub BattleMagicIce3Fragment end */

/* hub BattleMagicQuake3Shard begin */
#pragma pack(push, 1)
struct BattleMagicQuake3Shard {
    short positionX; /* +0x0 */
    short centroidY; /* +0x2 */
    short centroidZ; /* +0x4 */
    short positionY; /* +0x6 */
    short rotationX; /* +0x8 */
    short rotationY; /* +0xa */
    short rotationZ; /* +0xc */
    short fadeCounter; /* +0xe */
    short rotationSpeedX; /* +0x10 */
    short velocityY; /* +0x12 */
    short rotationSpeedZ; /* +0x14 */
    short accelerationY; /* +0x16 */
};
#pragma pack(pop)
/* hub BattleMagicQuake3Shard end */

/* hub BattleMagicQuake2Shard begin */
#pragma pack(push, 1)
struct BattleMagicQuake2Shard {
    short positionX; /* +0x0 */
    short centroidY; /* +0x2 */
    short centroidZ; /* +0x4 */
    short positionY; /* +0x6 */
    short rotationX; /* +0x8 */
    short rotationY; /* +0xa */
    short rotationZ; /* +0xc */
    short fadeCounter; /* +0xe */
    short rotationSpeedX; /* +0x10 */
    short offsetY; /* +0x12 */
    short rotationSpeedZ; /* +0x14 */
    short fallSpeed; /* +0x16 */
};
#pragma pack(pop)
/* hub BattleMagicQuake2Shard end */

/* Slots BattleFx_SpecialFallingRocksFallingModelTask allocates for BattleFx_SpecialRaku{FallingModel, SplittingSpinModel}Task. */
typedef struct BattleFxFallingRocksFallingModelSpawnSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short modelIndex;
    short angle;
    short angleVelocity;
} BattleFxFallingRocksFallingModelSpawnSlot;

/* The battle task slot of BattleFx_SpecialOilTimelineTask. */
typedef struct BattleFxOilTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxOilTimelineSlot;

/* The battle task slot of BattleFx_SpecialEnergyBallTimelineTask. */
typedef struct BattleFxEnergyBallTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxEnergyBallTimelineSlot;

/* The battle task slot of BattleFx_SpecialAtomicRayTimelineTask. */
typedef struct BattleFxAtomicRayTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short sourceX;
    short sourceY;
    short sourceZ;
    unsigned char pad_0A[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxAtomicRayTimelineSlot;

/* The battle task slot of BattleFx_SpecialEnergyWaveTimelineTask. */
typedef struct BattleFxEnergyWaveTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxEnergyWaveTimelineSlot;

/* The battle task slot of BattleFx_SpecialDarkWaveTimelineTask. */
typedef struct BattleFxDarkWaveTimelineSlot {
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
} BattleFxDarkWaveTimelineSlot;

/* The battle task slot of BattleFx_SpecialHellBomberRocketTask. */
typedef struct BattleFxHellBomberRocketSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxHellBomberRocketSlot;

/* The battle task slot of BattleFx_SpecialFlyingSickleTimelineTask. */
typedef struct BattleFxFlyingSickleTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short fromX;
    short fromY;
    short fromZ;
    unsigned char pad_0A[0x2];
    short toX;
    short toY;
    short toZ;
    unsigned char pad_12[0xa];
    short actorIndex;
} BattleFxFlyingSickleTimelineSlot;

/* The battle task slot of BattleFx_SpecialOilTargetModelTask. */
typedef struct BattleFxOilTargetModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxOilTargetModelSlot;

/* Slots BattleFx_SpecialOilTimelineTask allocates for BattleFx_SpecialOil{AimedModel, TargetModel, VariantSprite}Task. */
typedef struct BattleFxOilTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x2];
    short scaleRate;
    short isActive;
} BattleFxOilTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialNapalmMissileTask. */
typedef struct BattleFxNapalmMissileSlot {
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
} BattleFxNapalmMissileSlot;

/* Slots BattleFx_SpecialMultiJammerTimelineTask allocates for BattleFx_SpecialJammer2{GrowingModel, 3 more}Task. */
typedef struct BattleFxMultiJammerTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0x2];
    short variant;
    short angle;
    short unusedScale;
} BattleFxMultiJammerTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialEnergyWaveSpinModelsTask. */
typedef struct BattleFxEnergyWaveSpinModelsSlot {
    short ownerSlot;
    short frameCounter;
    short centerX;
    short centerY;
    short centerZ;
} BattleFxEnergyWaveSpinModelsSlot;

/* The battle task slot of BattleFx_SpecialPoisonGasBouncingSpriteTask. */
typedef struct BattleFxPoisonGasBouncingSpriteSlot {
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
    short phase;
    short variant;
} BattleFxPoisonGasBouncingSpriteSlot;

/* The battle task slot of BattleFx_SpecialTailLaserBouncingSpriteTask. */
typedef struct BattleFxTailLaserBouncingSpriteSlot {
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
    short variant;
} BattleFxTailLaserBouncingSpriteSlot;

/* Slots BattleFx_SpecialTailLaserTargetTimelineTask allocates for BattleFx_SpecialTele{AnimatedSprite, 3 more}Task. */
typedef struct BattleFxTailLaserTargetTimelineSpawnSlot {
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
} BattleFxTailLaserTargetTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialEnergyWaveBouncingSpriteTask. */
typedef struct BattleFxEnergyWaveBouncingSpriteSlot {
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
    short modelDesc;
} BattleFxEnergyWaveBouncingSpriteSlot;

/* The battle task slot of BattleFx_SpecialHellBomberBouncingSpriteTask. */
typedef struct BattleFxHellBomberBouncingSpriteSlot {
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
    short angle;
    unsigned char pad_18[0x2];
    short variant;
} BattleFxHellBomberBouncingSpriteSlot;

/* The battle task slot of BattleFx_SpecialMortarGravitySpriteTask. */
typedef struct BattleFxMortarGravitySpriteSlot {
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
    short spriteIndex;
} BattleFxMortarGravitySpriteSlot;

/* Slots BattleFx_SpecialDrainSpinModelTask allocates for BattleFx_SpecialKyuu{OrbitingSprite, SpiralSprite}Task. */
typedef struct BattleFxDrainSpinModelSpawnSlot {
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
    short scalePhase;
    short weightA;
    short weightB;
    short scaleDivisor;
} BattleFxDrainSpinModelSpawnSlot;

/* The battle task slot of BattleFx_SpecialDrainSpiralSpriteTask. */
typedef struct BattleFxDrainSpiralSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angle;
    unsigned char pad_16[0x2];
    short radius;
    short velocityY;
} BattleFxDrainSpiralSpriteSlot;

/* Slots BattleFx_SpecialFlyingSickleTimelineTask allocates for BattleFx_SpecialKama{OrbitModel, RisingAnimatedSprite, 1 more}Task. */
typedef struct BattleFxFlyingSickleTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angle;
    short angleZ;
    unsigned char pad_12[0x4];
    short orbitAngle;
    short spinRate;
    short distance;
} BattleFxFlyingSickleTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialFlyingSickleOrbitModelTask. */
typedef struct BattleFxFlyingSickleOrbitModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short angleY;
    short angleZ;
    unsigned char pad_12[0x4];
    short orbitAngle;
    short angleStep;
    short radius;
} BattleFxFlyingSickleOrbitModelSlot;

/* The battle task slot of BattleFx_SpecialJammerSpinFadeSpriteTask. */
typedef struct BattleFxJammerSpinFadeSpriteSlot {
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
    short tableIndex;
} BattleFxJammerSpinFadeSpriteSlot;

/* The battle task slot of BattleFx_SpecialColdBreathRisingGrowingModelTask. */
typedef struct BattleFxColdBreathRisingGrowingModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x2];
    short velocityZ;
} BattleFxColdBreathRisingGrowingModelSlot;

/* The battle task slot of BattleFx_BlueShadowFlareGrowFadeQuadTask. */
typedef struct BattleFxShadowFlareGrowFadeQuadSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
    short scaleStep;
} BattleFxShadowFlareGrowFadeQuadSlot;

/* The battle task slot of BattleFx_SpecialBlasterForwardBurstTask. */
typedef struct BattleFxBlasterForwardBurstSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short extent;
} BattleFxBlasterForwardBurstSlot;

/* The battle task slot of BattleFx_SpecialRadiantBreathRisingSpinModelTask. */
typedef struct BattleFxRadiantBreathRisingSpinModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short spinAngle;
    short animIndex;
} BattleFxRadiantBreathRisingSpinModelSlot;

/* The data block (Gfx_GetPsxScratchpadPtr(0)) the Debaria shatter model draw fills and fragment draw reads: the mesh
   description, the draw list and fade state, two matrices and the transformed fragment's vertices. */
typedef struct BattleFxDeBarrierShatterDesc {
    int meshDesc;
    int drawListHandle;
    int otShift;
    int primCursor; /* the render state block at first, then the cursor the fragment draw advances */
    int headerHighWord;
    int frontFacing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x4];
    int shardIndex;
    unsigned char pad_28[0x4];
    int bankIndex;
    unsigned char modelMatrix[0x1e]; /* a PsxMatrix */
    unsigned char transformRot[0x12]; /* the rotation of a PsxMatrix whose translation is prevDeltaX..Z */
    int prevDeltaX;
    int prevDeltaY;
    int prevDeltaZ;
    int scaleX;
    int prevValue;
    int curValue;
    unsigned char pad_78[0x1c];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_9A[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_A2[0x2];
    short offset2X;
    short offset2Y;
    short offset2Z;
    unsigned char pad_AA[0xa];
    short deltaX;
    short deltaY;
    short deltaZ;
} BattleFxDeBarrierShatterDesc;

/* The battle task slot of BattleFx_SpecialOilAimedModelTask. */
typedef struct BattleFxOilAimedModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_12[0x6];
    short scaleRate;
} BattleFxOilAimedModelSlot;

/* The battle task slot of BattleFx_SpecialFrogTwoPhaseAnimatedSpriteTask. */
typedef struct BattleFxFrogTwoPhaseAnimatedSpriteSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short yaw;
    short rotZ;
    unsigned char pad_12[0x8];
    short phase;
} BattleFxFrogTwoPhaseAnimatedSpriteSlot;

/* The battle task slot of BattleFx_SpecialGrandSparkStreakModelTask. */
typedef struct BattleFxGrandSparkStreakModelSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x8];
    short posX;
    unsigned char pad_0E[0x2];
    short posZ;
    unsigned char pad_12[0x4];
    short trailIndex;
} BattleFxGrandSparkStreakModelSlot;

/* Slots BattleFx_SpecialQuasarBeamCoreTask allocates for BattleFx_SpecialKueisa{BeamSpinModel, DriftingFadeSprite}Task. */
typedef struct BattleFxQuasarBeamCoreSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    SVECTOR rotation;
    unsigned char pad_14[0x2];
    short variant;
    short scale;
} BattleFxQuasarBeamCoreSpawnSlot;

/* The battle task slot of BattleFx_SpecialRadiantBreathAnimatedSpriteTask. */
typedef struct BattleFxRadiantBreathAnimatedSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotOrVelX;
    short rotOrVelY;
    short rotOrVelZ;
    unsigned char pad_12[0x4];
    short animIndex;
} BattleFxRadiantBreathAnimatedSpriteSlot;

/* Spark sprite placed at the beam scratch point plus the Alexander spark origin. */
typedef struct BattleFxRadialLivewireSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scaleOrVariant;
} BattleFxRadialLivewireSlot;

/* hub BattleActorSpawnPos begin */
/* An actor's spawn position, copied into its posX/posY/posZ (Battle_SnapActorToSpawnPos). */
#pragma pack(push, 1)
struct BattleActorSpawnPos {
    short posX; /* +0x0 */
    short posY; /* +0x2 */
    short posZ; /* +0x4 */
};
#pragma pack(pop)
/* hub BattleActorSpawnPos end */

/* The party's opening layouts: nine layouts of three positions (indexed layout, actor). */
struct OpeningPositions162 {
    struct BattleActorSpawnPos slot[9][3];
};

/* The enemies' opening layouts: nine layouts of two positions (indexed layout, enemy). */
struct OpeningPositions108 {
    struct BattleActorSpawnPos slot[9][2];
};

/* The party's facing for each opening layout and actor. */
struct OpeningFacing54 {
    short facing[9][3];
};

/* The enemies' facing for each opening layout and enemy. */
struct OpeningFacing36 {
    short facing[9][2];
};

/* hub BattleEffectTrailState begin */
/* Effect trail state: three channel bytes, a start value and three model nodes. */
#pragma pack(push, 1)
struct BattleEffectTrailState {
    char red; /* +0x0 */
    char channel1; /* +0x1 */
    char channel2; /* +0x2 */
    unsigned char initMask; /* +0x3 */
    short startValue; /* +0x4 */
    unsigned char pad_6[0x2];
    Battle3dNode nodes[3]; /* +0x8 */
};
#pragma pack(pop)
/* hub BattleEffectTrailState end */

/* hub BattleHitDisplayEntry begin */
/* An entry of the hit display table (Battle3d_PushHitDisplayEntry). */
#pragma pack(push, 1)
struct BattleHitDisplayEntry {
    char battleActorIndex; /* +0x0 */
    unsigned char pad_1[0x1];
    unsigned short targetMask; /* +0x2 */
    int status; /* +0x4 */
};
#pragma pack(pop)
/* hub BattleHitDisplayEntry end */

/* hub BattleRewardItem begin */
/* An item won in battle, as the result screen lists it. */
#pragma pack(push, 1)
struct BattleRewardItem {
    short itemId; /* +0x0: 0xFFFF: no item */
    short count; /* +0x2 */
    unsigned short selected; /* +0x4: the result menu toggles it; a set row is drawn in the right column */
};
#pragma pack(pop)
/* hub BattleRewardItem end */

/* hub BattleRewardTotals begin */
/* The battle result screen's totals: the gil won and one row per party member. */
#pragma pack(push, 1)
struct BattleRewardTotals {
    int gil; /* +0x0 */
    BattleRewardExpRow party[3]; /* +0x4 */
};
#pragma pack(pop)
/* hub BattleRewardTotals end */

/* hub BattleSequenceBuffer begin */
/* The battle sequence buffer: 0x40 BattleSeqStep records (Battle_AllocSequenceStep), then three words. */
#pragma pack(push, 1)
typedef struct BattleSequenceBuffer {
    BattleSeqStep steps[64]; /* +0x0 */
    int textFormatBase; /* +0x300 */
    int phasePendingFlag; /* +0x304 */
    int phaseSavedTargetMask; /* +0x308 */
} BattleSequenceBuffer;
#pragma pack(pop)
/* hub BattleSequenceBuffer end */

/* hub BattleTaskSlotBuffer begin */
/* The battle task slot tables: 100 main slots and 60 secondary ones, 32 bytes each. */
#pragma pack(push, 1)
struct BattleTaskSlotBuffer {
    unsigned char pad_0[0x18];
    char animRunning; /* +0x18 */
    unsigned char pad_19[0x7];
    BattleTaskSlot mainSlots[100]; /* +0x20: Battle_AllocateResourceSlot, Battle_RunMainTaskSlots */
    char isBusy; /* +0xca0 */
    unsigned char pad_ca1[0x7];
    BattleTaskSlot secondarySlots[60]; /* +0xca8: the secondary task table */
};
#pragma pack(pop)
/* hub BattleTaskSlotBuffer end */

/* hub KernelCommand begin */
/* One of the 32 battle command records of the kernel, 8 bytes. */
#pragma pack(push, 1)
struct KernelCommand {
    char cursorAction; /* +0x0 */
    unsigned char targetFlags; /* +0x1: copied to the command context's targetFlags */
    unsigned char pad_2[0x2];
    unsigned short animParam; /* +0x4: copied to the context's animParam */
    unsigned short animParamAlt; /* +0x6: copied to the context's animParamAlt */
};
#pragma pack(pop)
/* hub KernelCommand end */

/* Per-segment state of the Gravity magic beam, stepped and drawn each frame; state 0xFFFF marks a free record. */
typedef struct BattleFxMagicGravSegment {
    short state; /* +0x0 */
    short time;
    short startX;
    short startY;
    unsigned char pad_08[0x4];
    short endX;
    short endY;
    unsigned char pad_10[0x4];
    short offsetX;
    short offsetY;
    unsigned char pad_18[0x4];
    short offset2X;
    short offset2Y;
    unsigned char pad_20[0x4];
} BattleFxMagicGravSegment;

/* Battle limit animation bank: entry count and heap array of animation entries. */
typedef struct Battle3dLimitAnimBank {
    unsigned char pad_00[0x10];
    int entryCount;
    void *entries;
} Battle3dLimitAnimBank;

/* Task slot for drawing blue Kaeru notes; a sentinel word and a frame counter. */
typedef struct BattleFxFrogSongNoteTask {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x14];
    short ringIndex;
} BattleFxFrogSongNoteTask;

/* PSX polygon primitive for a battle effect block, filled from the block's words. */
typedef struct BattleFxPsxPolygonPrim {
    int tag;
    union {
        int colorCmd;
        struct {
            unsigned char rgb[3];
            char gpuCode;
        };
    };
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxPsxPolygonPrim;

/* Sprite record spawned by the Bio 3 rising-fade emitter; position is jittered from the emitter position. */
typedef struct BattleMagicBio3RisingSpriteRecord {
    unsigned char pad_00[0x8];
    short posX;
    short heightY;
    short posY;
    unsigned char pad_0E[0x2];
    short size;
    short riseSpeed;
} BattleMagicBio3RisingSpriteRecord;

/* Actor record as seen by the magic wall spawner: bone index, sine/cosine radius and angle. */
typedef struct BattleActorWallState {
    unsigned char pad_00[0x12];
    short radius;
    unsigned char pad_14[0x17];
    char boneIndex;
    unsigned char pad_2C[0x134];
    short angle;
} BattleActorWallState;

/* Main task slot of the ReDo pulsing sprite, counting frames. */
typedef struct BattleEnergyOrbPulsingSprite {
    short ownerSlot; /* +0x0 */
    short frameIndex;
    short billboardMatrix;
} BattleEnergyOrbPulsingSprite;

/* Secondary slot record of the Heart 1 bone particle emitter, aimed at a target actor. */
typedef struct BattleHeartCardBoneEmitter {
    unsigned char pad_00[0xd];
    char cellRow;
    short cellWord;
    unsigned char pad_10[0x2];
    short targetActor;
    short boneIndex;
    short spriteFlags;
} BattleHeartCardBoneEmitter;

/* Grav3 growing quad sprite slot; frame index advances each update. */
typedef struct BattleFxGrav3QuadSlot {
    short ownerSlot;
    short frameIndex;
    unsigned char pad_04[0x14];
    short pushLength;
} BattleFxGrav3QuadSlot;

/* Effect task record for the Raku timeline effect; holds a jittered position. */
typedef struct BattleFxFallingRocksTimelineTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short velocityY;
} BattleFxFallingRocksTimelineTask;

/* Secondary particle buffer whose four colour words are all set to one colour. */
typedef struct BattleFxGouraudQuadPacket {
    int tag;
    int color0;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxGouraudQuadPacket;

/* Task-slot record that spawns Bahamut animated sprites, counting spawns up to 15. */
typedef struct BattleFxBurstSpawnerTask {
    short ownerSlot;
    short spawnIndex;
} BattleFxBurstSpawnerTask;

/* Three-short source point passed to the KOTR billboard matrix install. */
typedef struct BattleFxSourcePoint {
    short x;
    short y;
    short z;
} BattleFxSourcePoint;

/* Three-step delay line of values for the Jaki oriented sprite particle, shifted each frame. */
typedef struct BattleFxLimitSealEvilDelay {
    int prevValue;
    int value;
    int nextValue;
} BattleFxLimitSealEvilDelay;

/* Descriptor of a PSX-style mesh drawn by the Toseki effect: a table pointer and flags. */
typedef struct BattleFxPsxMeshDescriptor {
    int table;
    int flags;
    unsigned char pad_08[0x4];
    short pageBits;
} BattleFxPsxMeshDescriptor;

/* Spark swarm state record: a sentinel short and a frame counter used to derive per-spark values. */
typedef struct BattleFxLivewireSwarm {
    short step;
    short frame;
} BattleFxLivewireSwarm;

/* Skeleton whose bone records (0x1C bytes each) supply the bone vectors gathered for a model. */
typedef struct Battle3dSkeleton {
    unsigned char pad_00[0x10];
    int boneCount;
    int bones;
} Battle3dSkeleton;

/* GPU streak-line packet whose first word holds the packet tag. */
typedef struct BattleFxStreakPacket {
    int tag;
    int colorWord0;
    unsigned char pad_08[0x4];
    int colorWord1;
} BattleFxStreakPacket;

/* Data block of the expanding fade ring effect; unusedStateA and unusedStateB are copied to copiedStateA and copiedStateB. */
typedef struct BattleFxFadeRingBlock {
    unsigned char pad_00[0x24];
    int orderingDepth;
    unsigned char pad_28[0x90];
    int unusedStateA;
    int copiedStateA;
    unsigned char pad_C0[0x8];
    int unusedStateB;
    int copiedStateB;
} BattleFxFadeRingBlock;

/* PSX-style GPU primitive for the program 5 gouraud triangle draw; tag is the tag word. */
typedef struct BattleEffectGouraudPrim {
    int tag;
    unsigned char color0Rgb[0x3];
    char gpuCode;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
} BattleEffectGouraudPrim;

/* Stage offscreen render size; width and height are the viewport width and height. */
typedef struct BattleStageViewSize {
    unsigned char pad_00[0x4];
    short width;
    short height;
} BattleStageViewSize;

/* Faiga setup record in a task slot; sets its parameter and initial values. */
typedef struct BattleFxFire3SetupRecord {
    unsigned char pad_00[0x4];
    BattleFxTaskPos pos;
    unsigned char pad_0C[0x6];
    short radius;
    short angle;
    short angleStep;
} BattleFxFire3SetupRecord;

/* Sin/cos offset pair and jitter in the descending comet model task slot. */
typedef struct BattleFxComet2Offset {
    short sinOffset;
    short cosOffset;
    short jitter;
} BattleFxComet2Offset;

/* Renmisa impact model record: position orbiting a base point at a fixed height. */
typedef struct BattleFxMissileSalvoImpactSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
} BattleFxMissileSalvoImpactSlot;

/* Ring-buffer record placing the Neo Bahamut light rig at its world position and Y rotation. */
typedef struct BattleFxLightRigSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short rotationY;
} BattleFxLightRigSlot;

/* Source position record copied into an offscreen actor request for the Odin Zantetsuken summon. */
typedef struct BattleFxOdinZantetsukenActorPos {
    short posX;
    short posY;
    short width;
    short height;
} BattleFxOdinZantetsukenActorPos;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxPhaseControllerTask {
    short stage;
    short frameCount;
} BattleFxPhaseControllerTask;

/* Phoenix rising sprite: position jittered around a base point taken from the emitter. */
typedef struct BattleFxAscentSpriteSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
} BattleFxAscentSpriteSlot;

/* Viewport rectangle copied into an offscreen stage request. */
typedef struct BattleFxSummonViewport {
    short x;
    short y;
    short w;
    short h;
} BattleFxSummonViewport;

/* Ring buffer record for the Choco/Mog summon, with transform and angle fields. */
typedef struct BattleFxRingWaveSlot {
    unsigned char pad_00[0x4];
    short flags;
    short lifetime;
    short transZ;
    unsigned char pad_0A[0x6];
    short angleY;
} BattleFxRingWaveSlot;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleFxExpandingSpriteTask {
    short ownerSlot; /* +0x0 */
    short frameCount;
    short billboardMatrix;
} BattleFxExpandingSpriteTask;

/* Titan mesh instance, holding mesh address, flags and page bits. */
typedef struct BattleFxSummonTitanMeshInstance {
    int meshData;
    int flags;
    unsigned char pad_08[0x4];
    short pageBits;
} BattleFxSummonTitanMeshInstance;

/* Four grid corner pointers for one ring-grid cell: [r][t], [r][t+1], [r+1][t], [r+1][t+1]. */
typedef struct BattleFxRingGridCell {
    int cornerRowCol;
    int cornerNextCol;
    int cornerNextRow;
    int cornerNextRowCol;
} BattleFxRingGridCell;

/* Six-byte battle inventory record: item id and a quantity capped at 99. */
typedef struct BattleInventoryItemRecord {
    unsigned short itemId;
    unsigned char quantity;
    unsigned char current;
    unsigned char mask;
} BattleInventoryItemRecord;

/* Battle model descriptor as written back to file: entry count and the entry table pointer. */
typedef struct Battle3dDescriptorFile {
    unsigned short packedHeaderWord;
    unsigned char pad_02[0xa];
    int entryCount;
    unsigned char pad_10[0x20];
    void *entries;
} Battle3dDescriptorFile;

/* Battle task slot for a Howin fading sprite, with three random-set fields. */
typedef struct BattleWhiteWindFadingSpriteSlot {
    unsigned char pad_00[0x8];
    Int64Pair origin;
    Int64Pair angle;
    short radialSpeed;
    short scale;
    short spinStep;
} BattleWhiteWindFadingSpriteSlot;

/* Short hit spark emitter record in the 0x20-byte emitter array, with a target index. */
typedef struct BattleHitLivewireShortEmitter {
    unsigned char pad_00[0x12];
    short targetIndex;
    short boneIndex;
    short lifetime;
} BattleHitLivewireShortEmitter;

/* Shatter render-state primitive with a tag word and a command byte. */
typedef struct BattleShatterPrimitive {
    int tag;
    unsigned char pad_04[0x3];
    char command;
} BattleShatterPrimitive;

/* Only the fields the code reads; the size is not known. */
typedef struct BattleMagicShieldFaceList {
    unsigned short pad_00;
    unsigned short pad_02;
    unsigned short pad_04;
} BattleMagicShieldFaceList;

/* Three-byte colour copied into the Grav5 sprite draw request. */
typedef struct BattleFxSpriteColor {
    char colorR;
    char colorG;
    char colorB;
} BattleFxSpriteColor;

/* Task slot holding the camera script, matrix and actor for Neo Bahamut. */
typedef struct BattleFxCameraTrackSlot {
    unsigned char pad_00[0xc];
    int cameraScript;
    int matrix;
    short actorIndex;
} BattleFxCameraTrackSlot;

/* Task record with a 0xFFFF sentinel and an index advanced for each scaling model pair. */
typedef struct BattleFxDualCounterTask {
    short ownerSlot;
    short nextIndex;
} BattleFxDualCounterTask;

/* PSX mesh resource header holding a data base address and texture flags. */
typedef struct BattleFxPsxMeshResource {
    int dataPtr;
    unsigned char pad_04[0x8];
    short textureFlags;
} BattleFxPsxMeshResource;

/* Euler rotation angles used to build the Bahamut rotation matrix. */
typedef struct BattleFxBahamutEulerRotation {
    short rotX;
    short rotY;
    short rotZ;
} BattleFxBahamutEulerRotation;

/* Render state block into which the Eva falling streak writes three words. */
typedef struct BattleFxGreatGospelRenderStateBlock {
    int tag;
    int color0;
    unsigned char pad_08[0x4];
    int color1;
} BattleFxGreatGospelRenderStateBlock;

/* Task slot for the Eva spawn camera script: script data, matrix pointer and actor index. */
typedef struct BattleFxGreatGospelCameraSlot {
    unsigned char pad_00[0xc];
    int scriptData;
    int matrixPtr;
    short actorIdx;
} BattleFxGreatGospelCameraSlot;

/* Resource block whose dataPtr points at model data and whose shade is passed as a shade argument. */
typedef struct BattleFxShivaTriangleModelBlock {
    int dataPtr;
    unsigned char pad_04[0x6];
    short shade;
} BattleFxShivaTriangleModelBlock;

/* PSX sprite-record block header for Ramuh: block offset and record count. */
typedef struct BattleFxRamuhPsxSpriteHeader {
    int blockOffset;
    union {
        int colorWord;
        struct {
            char colorR;
            char colorG;
            char colorB;
        };
    };
    unsigned short recordCount;
} BattleFxRamuhPsxSpriteHeader;

/* Thrown-weapon state for Aeris: three scaled delta components. */
typedef struct BattleFxAerisThrowDelta {
    unsigned char pad_00[0xc];
    short deltaX;
    short deltaY;
    short deltaZ;
} BattleFxAerisThrowDelta;

/* Translation words read from a matrix to offset a local actor position. */
typedef struct BattleFxWeapon2MatrixTranslation {
    unsigned char pad_00[0x12];
    short translateX;
    unsigned char pad_14[0x2];
    short translateY;
    unsigned char pad_18[0x2];
    short translateZ;
} BattleFxWeapon2MatrixTranslation;

/* Ring-grid point; its x and z feed the height lookup. */
typedef struct BattleFxRingGridPoint {
    short posX;
    unsigned char pad_02[0x2];
    short posZ;
} BattleFxRingGridPoint;

/* Actor matrix whose translation offsets local positions in the last boss and Weapon1 effects. */
typedef struct BattleFxActorMatrix {
    unsigned char pad_00[0x12];
    short translationX;
    unsigned char pad_14[0x2];
    short translationY;
    unsigned char pad_18[0x2];
    short translationZ;
} BattleFxActorMatrix;

/* Descriptor giving the record base and count for emitting Bz ground spark primitives. */
typedef struct BattleFxGroundLivewireDescriptor {
    int recordsBase;
    int color;
    unsigned short recordCount;
} BattleFxGroundLivewireDescriptor;

/* Leading three matrix entries saved for the Busin cylinder. */
typedef struct BattleFxLimitOmnislashMatrix {
    short vx;
    short vy;
    short vz;
} BattleFxLimitOmnislashMatrix;

/* Battle model animation bank loaded from a file and decoded per animation. */
typedef struct BattleModelAnimBank {
    unsigned char pad_00[0x2];
    short byteLimit; /* +0x2: size of the packed frames in bytes */
    unsigned char shift; /* +0x4: bits dropped from each angle; the packed frames start at +0x5 */
} BattleModelAnimBank;

/* Battle stage dat record holding a heap block and a model set to free. */
typedef struct BattleStageDat {
    unsigned char pad_00[0x4];
    void *heapBlock;
    void *modelSet;
} BattleStageDat;

/* Inflate section with start and end pointers cleared by BinInflateSectionAndClearTail. */
typedef struct BattleBinSection {
    int start;
    int end;
} BattleBinSection;

/* Timeline record for the Yuffie weapon throw dispatch. */
typedef struct BattleFxYuffieWeaponTimeline2 {
    short ownerSlot;
    unsigned char pad_02[0x1a];
    short subCounter;
} BattleFxYuffieWeaponTimeline2;

/* Task-slot record initialised by the Limit Dise timeline. */
typedef struct BattleFxDiceTask {
    short ownerSlot;
    unsigned char pad_02[0x12];
    short spawnCursor;
} BattleFxDiceTask;

/* Task-slot record initialised by the Blue Dethfo setup task. */
typedef struct BattleFxBlueDeathForceTask {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short radius;
} BattleFxBlueDeathForceTask;

/* Main task slot for the Satan stage mesh timeline. */
typedef struct BattleFxSatanSlamStageTask {
    unsigned char pad_00[0xc];
    short scale;
    short scaleStep;
} BattleFxSatanSlamStageTask;

/* PSX-style primitive written by the Limit Gaisei mesh draw. */
typedef struct BattleFxPsxPrimitive {
    int tag;
    union {
        int colorCommand;
        struct {
            unsigned char pad_04[0x3];
            char gpuCode;
        };
    };
    unsigned char pad_08[0x4];
    int colorWord;
    unsigned char pad_10[0x4];
    short uvWord0;
    short renderFlags;
    unsigned char pad_18[0x4];
    short uvWord1;
    unsigned char pad_1E[0x6];
    short uvWord2;
    unsigned char pad_26[0x2];
} BattleFxPsxPrimitive;

/* PSX-style polygon written by the Limit H-Jump displaced mesh draw. */
typedef struct BattlePsxDisplacedMeshPoly {
    int headerWord;
    unsigned char pad_04[0x3];
    char command;
    unsigned char pad_08[0x4];
    int colorWord;
    unsigned char pad_10[0x4];
    short uvWord0;
    short polyWord;
    unsigned char pad_18[0x4];
    short uvWord1;
} BattlePsxDisplacedMeshPoly;

/* Task-slot record for the Limit B-Jump effect, one per target. */
typedef struct BattleFxBoostJumpTask {
    unsigned char pad_00[0x1c];
    short targetMask;
    short sourceActor;
} BattleFxBoostJumpTask;

/* Ring-buffer timeline record for the Limit SD effect. */
typedef struct BattleFxPoint3s {
    short targetIndex;
    unsigned char pad_02[0xe];
    short sequenceIndex;
} BattleFxPoint3s;

/* Corner record that supplies source texture coordinates for a textured quad. */
typedef struct BattleFxStardustRayCorner {
    short srcX;
    short srcY;
} BattleFxStardustRayCorner;

/* Task-slot record for the Limit Hvshot effect, one per target. */
typedef struct BattleFxBigShotTask {
    unsigned char pad_00[0x4];
    short targetIndex;
    short sourceActor;
} BattleFxBigShotTask;

/* Task-slot record for the Magic Rezist radial sprites, one per target. */
typedef struct BattleFxResistTask {
    unsigned char pad_00[0x4];
    short targetIndex;
    unsigned char pad_06[0x2];
    short posX;
    short posY;
} BattleFxResistTask;

/* Ramp record for the Turara special effect, toggled while idle. */
typedef struct BattleFxIcicleDropRamp {
    short ownerSlot;
    short toggle;
} BattleFxIcicleDropRamp;

/* Task-slot record for the Magic Bio effect, one per target. */
typedef struct BattleFxOrbitCloudTask {
    unsigned char pad_00[0x4];
    short targetIndex;
    short param;
    short bonePos[3];
} BattleFxOrbitCloudTask;

/* Task-slot record for the Magic Thunder4 effect. */
typedef struct BattleFxBolt3MultiRecord {
    unsigned char pad_00[0x4];
    short actorId;
    short param;
} BattleFxBolt3MultiRecord;

/* Timeline record for the Tunami special effect, compared against a limit. */
typedef struct BattleFxBigWaveTimeline {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short timer;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short limit;
} BattleFxBigWaveTimeline;

/* Particle record for the Pink special effect emitter. */
typedef struct BattleFxClaimedParticleRecord {
    unsigned char pad_00[0x2];
    short radius;
    short angle;
} BattleFxClaimedParticleRecord;

/* Timeline record for the Jammer special effect, one per target. */
typedef struct BattleFxJammerTimeline {
    unsigned char pad_00[0x4];
    short sourceBonePos[3]; /* +0x4: world position of the source actor's bone */
    unsigned char pad_0A[0x2];
    short targetBonePos[3]; /* +0xC: world position of the target actor's bone */
    unsigned char pad_12[0xa];
    short targetIndex;
    short sourceActor;
} BattleFxJammerTimeline;

/* Primitive written into the packet for the Reito per-face offset mesh draw. */
typedef struct BattleFxFreezeBreathDrawPrim {
    int tag;
    unsigned char pad_04[0x3];
    char polyCommand;
    unsigned char pad_08[0x4];
    int colorWord;
    unsigned char pad_10[0x4];
    short uvWord0;
    short flagBits;
    unsigned char pad_18[0x4];
    short uvWord1;
} BattleFxFreezeBreathDrawPrim;

/* Main task slot for the Toseki effect, holding the target and source actors. */
typedef struct BattleFxBoulderThrowTask {
    unsigned char pad_00[0x4];
    short startPos[3];
    unsigned char pad_0A[0x2];
    short endPos[3];
    unsigned char pad_12[0xa];
    short targetActor;
    short sourceActor;
} BattleFxBoulderThrowTask;

/* Orbit-sprite ring record for the Timedmg special effect, read at offset x+0x1E. */
typedef struct BattleFxTimeDamageOrbitRing {
    short screenX;
    short screenY;
} BattleFxTimeDamageOrbitRing;

/* Vector argument of the Bahamut billboard matrix install. */
typedef struct BattleFxBillboardVector {
    short posX;
    short posY;
    short posZ;
} BattleFxBillboardVector;

/* Limit slot record holding the usable limit count and a cleared byte. */
typedef struct BattleLimitSlots {
    unsigned char attackIds[3]; /* the limit attack ids; 0xFF for none */
    char targetFlags[3]; /* set for each listed attack whose limit the character has learned */
    char usableCount;
    char clearedByte;
} BattleLimitSlots;

/* Data block set up for the Satan stage, pointing at its working mesh blob and vertex span. */
typedef struct BattleFxSatanSlamMeshSetup {
    int workingMeshBlob;
    short *vertexSpan;
    unsigned char pad_08[0x24];
    int fragmentCount;
} BattleFxSatanSlamMeshSetup;

/* Battle 3D scene context (0x4F4 bytes) reached through g_pBattle3dScene: stage models, SEFFECT and
 * battle-window render objects, rain batch, model-table bank, script/model bank, and offscreen slots. */
struct GfxRenderObject;
typedef struct Battle3dScene {
    union {
        int stageId;                         /* +0x000 */
        int stageDatModel;
    };
    union {
        struct PolygonRsdModel *model;       /* +0x004 */
        int rsdModelA;
    };
    union {
        int modelRequest;                    /* +0x008 */
        int meshPolygonSet;
        int rsdModelB;
    };
    union {
        int markModel;                       /* +0x00C */
        int rsdModelC;
    };
    struct GfxRenderObject *seffect1Object;  /* +0x010 */
    struct GfxRenderObject *seffect2Object;  /* +0x014 */
    struct GfxRenderObject *seffect3Object;  /* +0x018 */
    int seffect1Batch;                       /* +0x01C */
    int seffect2Batch;                       /* +0x020 */
    int seffect3Batch;                       /* +0x024 */
    int batchListA;                          /* +0x028 */
    union {
        int overlayQuadState;                /* +0x02C */
        struct GfxRenderObject *frame1x;
    };
    union {
        int spriteQuadState;                 /* +0x030 */
        int batchListB;
        struct GfxRenderObject *frame2x;
    };
    union {
        int quadRecord;                      /* +0x034 */
        struct GfxRenderObject *glyph1x;
    };
    union {
        int batchListC;                      /* +0x038 */
        struct GfxRenderObject *glyph2x;
    };
    union {
        int batchListD;                      /* +0x03C */
        struct GfxRenderObject *glyphOverflow;
    };
    union {
        int batchListE;                      /* +0x040 */
        struct GfxRenderObject *background;
    };
    union {
        int batchListF;                      /* +0x044 */
        struct GfxRenderObject *emphasis;
    };
    union {
        int rainSplashSpriteState;           /* +0x048 */
        int rainBatch;
    };
    union {
        int modelTableBank;                  /* +0x04C */
        int modelBank;
    };
    union {
        int scriptState;                     /* +0x050 */
        int battleModelBank;
    };
    unsigned char offscreenSlotsRaw[0x490];  /* +0x054: 4 x GfxOffscreenSlot (0x124 bytes each) */
    unsigned char pad_4E4[0x10];             /* +0x4E4 */
} Battle3dScene;
typedef char CheckBattle3dScene[(sizeof(Battle3dScene) == 0x4F4) ? 1 : -1];

/* Timeline record shared by the Battle FX effect tasks; the layout is inferred from copies between fields. */
typedef struct BattleFxTimelineState {
    short vecX;
    short wheelAngle;
    short spinSpeed;
    unsigned char pad_06[0x2];
    short posX;
    short vecY;
    short posY;
    unsigned char pad_0E[0x2];
    short posZ;
    short transX; /* +0x12 */
    short rotPoint1Y;
    short transY; /* +0x16 */
    short rotPoint1Z;
    short transZ; /* +0x1a */
    unsigned char pad_1c[0x4];
    short rotPoint2X;
    unsigned char pad_22[0x2];
    short rotPoint2Y;
    unsigned char pad_26[0x2];
    short rotPoint2Z;
    unsigned char pad_2A[0x6];
    int flags;
    unsigned char pad_34[0x4];
    int pad_38;
    int pad_3C;
    int pad_40;
} BattleFxTimelineState;

/* One battle camera channel (0xE bytes): g_BattleCameraEyeChannels[4] runs the position scripts
 * and g_BattleCameraFocusChannels[4] the focal ones (battle_camera_channel_scripts.cpp); channel 3 holds
 * the formation camera's eye or focus that the auto camera eases toward. */
typedef struct BattleCameraChannel {
    short x;                     /* +0x00 */
    short y;                     /* +0x02 */
    short z;                     /* +0x04 */
    unsigned char pad_06[0x2];
    unsigned short scriptOffset; /* +0x08: offset of the next script byte; 0xFF restarts the script */
    unsigned short waitFrames;   /* +0x0A: frames left of the script's wait (BATTLE_CAMERA_*_SET_WAIT) */
    union {                      /* +0x0C */
        unsigned char rotateOffsets;    /* eye channels: bone offsets turn with the actor (BATTLE_CAMERA_EYE_ROTATE_OFFSETS_ON) */
        unsigned char focusLinearMoves; /* focus channels: moves are linear (1) or eased (0) */
    };
    unsigned char eyeLinearMoves; /* +0x0D: eye channels: moves are linear (1) or eased (0) */
} BattleCameraChannel;

/* Transformed vertex: position, radius and depth for Battle FX line and triangle effects. */
typedef struct BattleFxVertex {
    short x;
    short baseY;
    short baseZ;
    short radius;
    short endX;
    short y;
    short z;
    unsigned char pad_0E[0x22];
    int depth;
    unsigned char pad_34[0x4];
    int flags;
} BattleFxVertex;

/* One Hades summon sprite particle: position, age, brightness fade, toggle and anchor index. */
typedef struct BattleFxHadesSpriteParticle {
    short age;
    short lifetime;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    union {
        int colorWord;
        struct {
            short brightness;
            short brightnessLevel;
        };
    };
    short brightnessRate;
    short toggle;
    short anchorIndex;
    short scale;
} BattleFxHadesSpriteParticle;

/* Battle command list index entry: command code and materia level. */
typedef struct BattleCommandListEntry {
    unsigned char commandCode;
    char level;
    char supportFlags;
} BattleCommandListEntry;

/* Model-draw request record that battle effect tasks fill before BattleFx_SubmitModelDraw. */
typedef struct BattleFxModelDrawRequest {
    int modelHandle;
    int flags;
    unsigned char pad_08[0x2];
    short alpha;
    unsigned char pad_0C[0x4];
    int resourceHandle;
    int altHandle;
    unsigned char pad_18[0x2];
    short scaleFraction;
    unsigned char pad_1C[0xc];
    short frameIndex;
} BattleFxModelDrawRequest;

/* Ribbon-tail particle: active flag, age, position and per-frame velocity. */
typedef struct BattleFxRibbonParticle {
    short active;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short phase;
    short scale;
} BattleFxRibbonParticle;

/* Per-actor screen point: previous latched X/Y and current projected X/Y. */
typedef struct BattleActorScreenPos {
    short prevX;
    short prevY;
    short projectedX;
    short projectedY;
} BattleActorScreenPos;

/* Jibaku2 special animated sprite: frame index, position, velocity and mode. */
typedef struct BattleFxBombBlastSprite {
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
    short mode;
} BattleFxBombBlastSprite;

/* Per-vertex pose delta of the Queig magic mesh, one short per axis, added scaled to the vertices. */
typedef struct BattleFxShatterMeshDelta {
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_06[0x2];
} BattleFxShatterMeshDelta;

/* Battle party model load slot: load state and the model id loaded into a bank slot. */
typedef struct BattlePartyModelLoadSlot {
    short loadState;
    short modelId;
} BattlePartyModelLoadSlot;

/* Limit Busin spinning cylinder pair state; step is a step counter. */
typedef struct BattleFxLimitOmnislashCylinderPair {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
} BattleFxLimitOmnislashCylinderPair;

/* One Blue Beata particle: position copied from the field, plus a countdown timer. */
typedef struct BattleBlueBetaParticle {
    int timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    int cleared[4]; /* +0xC: zeroed when the particle is placed; nothing reads them */
    int period;
} BattleBlueBetaParticle;

/* Timeline slot of one Dia 1 special effect particle, set from distance, offset and angle. */
typedef struct BattleFxDiamondCardSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    short distance;
    short offsetX;
    short velocityY;
    short offsetZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxDiamondCardSlot;

/* Timeline slot of one Dia 2 special effect particle, set from distance, offset and angle. */
typedef struct BattleFxSunCardSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    short distance;
    short offsetX;
    short velocityY;
    short offsetZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxSunCardSlot;

/* Timeline slot of one Club 2 special effect particle, set from distance, offset and angle. */
typedef struct BattleFxStarCardSlot {
    unsigned char pad_00[0x4];
    short posX;
    short offsetY;
    short posZ;
    short distance;
    short offsetX;
    short velocityY;
    short offsetZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxStarCardSlot;

/* Jitter envelope of the Kjata summon: frame count, amplitude and rise, hold and fall. */
typedef struct BattleFxJitterEnvelope {
    short ownerSlot; /* +0x0 */
    short frameCount;
    short amplitude;
    short riseFrames;
    short holdFrames;
    short fallFrames;
} BattleFxJitterEnvelope;

/* Jammer variant sprite task state: frame index, position and velocity pairs. */
typedef struct BattleFxJammerVariantSprite {
    short ownerSlot;
    short frameIndex;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0x2];
    short variant;
} BattleFxJammerVariantSprite;

/* Formation slot entry of 0xC bytes in the battle overlay; -1 marks an empty slot. */
typedef struct BattleFormationEntry {
    short slotId; /* +0x00: the enemy's model-bank slot (g_BattleScene.enemies[i].enemyIndex); -1 = none */
    short posX;   /* +0x02 */
    short posY;   /* +0x04 */
    short posZ;   /* +0x06 */
    int flags;    /* +0x08 */
} BattleFormationEntry;

/* Fragment vertex of 8 bytes in the Dead Sef model fragment buffer. */
typedef struct BattleFxShortPointEntry {
    short posX;
    unsigned char pad_02[0x6];
} BattleFxShortPointEntry;

/* Timeline record of the Spade-2 special effect, read through frame.current. */
typedef struct BattleFxEmperorCardRecord {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
} BattleFxEmperorCardRecord;

/* Per-task state of the Mizu variant sprite animation, reached through frame.slot. */
typedef struct BattleFxWaterGunSpriteTask {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x10];
    short variant;
} BattleFxWaterGunSpriteTask;

/* Trail point of a thrown Cid weapon ribbon, the midpoint position in three shorts. */
typedef struct BattleFxThrowTrailPoint {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
} BattleFxThrowTrailPoint;

/* Sprite quad draw request filled by the battle particle and sprite tasks. */
typedef struct BattleSpriteQuadRequest {
    unsigned char pad_00[0x4];
    char fadeLevel;
    unsigned char pad_05[0x3];
    int sourcePtr;
    unsigned char pad_0C[0x4];
    int resource;
} BattleSpriteQuadRequest;

/* Animated sprite task state for the Gas special effect. */
typedef struct BattleSpecPoisonGasSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short mode;
} BattleSpecPoisonGasSprite;

/* 8-byte vertex entry of the dead Sef fragment vertex buffer, filled from the timeline. */
typedef struct BattleFxDeadSefVertex {
    short posY;
    unsigned char pad_02[0x6];
} BattleFxDeadSefVertex;

/* Battle command target state: HP/MP value, a divisor and a learned-skill mask. */
typedef struct BattleCommandTargetState {
    unsigned char pad_00[0x5];
    unsigned char limitLevelIndex;
    unsigned char pad_06[0x2];
    unsigned short limitGauge;
    unsigned char pad_0A[0x12];
    unsigned int divisor;
    unsigned char pad_20[0x4];
    int learnMask;
} BattleCommandTargetState;

/* Per-vertex offset from the Titan center, one 8-byte record per vertex. */
typedef struct BattleFxVertexOffsetVec {
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_06[0x2];
} BattleFxVertexOffsetVec;

/* Model record a battle actor points at, with a model-part table and an animation frame table. */
typedef struct BattleActorModel {
    unsigned char pad_00[0x4];
    int modelParts;
    unsigned char pad_08[0x4];
    int frameTable;
} BattleActorModel;

/* Per-actor magic-fire state record cleared by the actor animation script. */
typedef struct BattleActorTransformRecord {
    int animState;
    unsigned char pad_04[0x4];
    union {
        struct {
            unsigned char auxNode[0x26]; /* +0x8: the actor's aux model node */
            short auxPosX;
            short auxPosY;
            short auxPosZ;
            unsigned char pad_34[0x8];
        };
        Battle3dNode aux;
    };
    short boneCount;
    char fadeFlags;
    unsigned char pad_3F[0x1];
} BattleActorTransformRecord;

/* Pending drop slot: the owning actor index and the item it drops, 0xFFFF in both when free. */
typedef struct BattlePendingDropRecord {
    short actor;
    short itemId;
} BattlePendingDropRecord;

/* Magic-list record: the magic id, the materia counts that go to the stat block's ability record, and its support byte (MP turbo count in the top three bits). */
typedef struct BattleMagicListRecord {
    char magicId;
    char allCount;
    char quadraLinks;
    char quadraCount;
    unsigned char support;
} BattleMagicListRecord;

/* Three-entry delay line of camera values for the Bahamut beam effect. */
typedef struct BattleFxBahamutCameraDelay {
    int oldSample;
    int midSample;
    int newSample;
} BattleFxBahamutCameraDelay;

/* Three-entry delay line of ring radius values for the Shiva burst. */
typedef struct BattleFxShivaRingDelay {
    unsigned char pad_00[0x8];
    int oldSample;
    int midSample;
    int newSample;
} BattleFxShivaRingDelay;

/* One character's row of kernel.bin's battle and growth data (KernelBattleData::chars, 0x38 bytes).
 * The growth code reads +0..+0xA (Kernel_EvalPrimaryStatCurve, Kernel_EvalHpCurve/MpCurve,
 * Kernel_ComputeExpForLevel, Kernel_InitJoiningCharacter), the limit code the rest
 * (Kernel_GetLimitAttackId, Kernel_UpdateLimitUnlocks, the party loader). */
typedef struct KernelCharRecord {
    unsigned char statCurves[6]; /* +0x00: the curve ids of strength, vitality, magic, spirit, dexterity, luck */
    unsigned char hpCurve; /* +0x06 */
    unsigned char mpCurve; /* +0x07 */
    unsigned char expCurve; /* +0x08 */
    unsigned char pad_09;
    signed char joinLevelOffset; /* +0x0A: a joining character's level is the party average + this / 2,
                                  * or gSavemap.joinLevel when bit 0 is set (Kernel_InitJoiningCharacter) */
    unsigned char pad_0B;
    unsigned char limitAttackIds[4][3]; /* +0x0C: per limit level and slot, the attack id + 0x80 */
    unsigned short limitLevelKills[2]; /* +0x18: the kill counts that unlock limit levels 2 and 3 (0xFFFF none) */
    unsigned short limitUnlockUses[3][2]; /* +0x1C: per limit level 1..3, the uses of its first limit that
                                           * unlock its second and third (0xFFFF none) */
    int limitLevelParams[4]; /* +0x28: per limit level; the party loader copies the current level's into
                              * its party actor's +0x1C */
} KernelCharRecord;
typedef char CheckKernelCharRecord[(sizeof(KernelCharRecord) == 0x38) ? 1 : -1];

/* One growth curve (KernelBattleData::curves): a bracket per experience tier (CharGrowth::tier).
 * A stat is gradient * level / 0x64 + base, HP gradient * level + base * 0x28, MP gradient * level
 * / 0xA + base * 2 (Kernel_EvalPrimaryStatCurve, Kernel_EvalHpCurve, Kernel_EvalMpCurve); the
 * experience for a level sums gradient * q * q / 0xA (Kernel_ComputeExpForLevel). */
typedef struct KernelCurveBracket {
    unsigned char gradient;
    signed char base;
} KernelCurveBracket;
typedef struct KernelStatCurve {
    KernelCurveBracket brackets[8];
} KernelStatCurve;

/* kernel.bin's battle and growth data (section 3, g_KernelBinSectionTargets[2]; 0xF94 bytes).
 * Each member is named from the code that reads it: the growth and limit code (chars, the three
 * bonus tables, curves), Battle_FindAiScript (aiScripts), the battle RNG streams (randomTable), the
 * battle loader's scene lookup (sceneLookup) and the magic menu sort (magicOrder). */
typedef struct KernelBattleData {
    KernelCharRecord chars[9]; /* +0x000 */
    unsigned char statBonus[12]; /* +0x1F8: the primary-stat level-up bonus per Kernel_PickGrowthBonusIndex index */
    unsigned char hpBonus[12]; /* +0x204: the HP level-up percentage of the curve step, per index */
    unsigned char mpBonus[12]; /* +0x210: the MP level-up percentage, per index */
    KernelStatCurve curves[64]; /* +0x21C */
    unsigned char aiScripts[0x800]; /* +0x61C: the characters' AI scripts */
    unsigned char randomTable[0x100]; /* +0xE1C */
    unsigned char sceneLookup[0x40]; /* +0xF1C: the first scene index of each scene.bin block */
    unsigned char magicOrder[0x38]; /* +0xF5C: the magic menu's order */
} KernelBattleData;
typedef char CheckKernelBattleData[(sizeof(KernelBattleData) == 0xF94) ? 1 : -1];

/* The eight growing stats, in the order the level-up code indexes them (statIndex 0..5 are the
 * primary stats, Kernel_LevelUpPrimaryStat). */
typedef struct CharGrowthStats {
    unsigned short strength;
    unsigned short vitality;
    unsigned short magic;
    unsigned short spirit;
    unsigned short dexterity;
    unsigned short luck;
    unsigned short maxHp;
    unsigned short maxMp;
} CharGrowthStats;

/* A character's growth record (0x84 bytes, a local of the experience functions).
 * Kernel_LoadGrowthActor copies the id, level, stats and experience from the savemap record into
 * base; Kernel_BindGrowthRecord points kernelRow at the character's kernel row, raises a zero
 * stat to 1, copies base to cur and clears the thresholds; Kernel_ApplyExpGrowthLevelUps adds
 * expEarned and levels cur up; Savemap_WriteGrowthToRecord copies cur, the level and the
 * experience back. */
typedef struct CharGrowth {
    int tier; /* +0x00: the experience bracket of the last level Kernel_ComputeExpForLevel computed */
    unsigned char charId; /* +0x04 */
    unsigned char level; /* +0x05 */
    unsigned char pad_06[0x2];
    union {
        unsigned short baseStats[8]; /* +0x08: as loaded from the savemap */
        CharGrowthStats base;
    };
    union {
        unsigned short curStats[8]; /* +0x18: as levelled up */
        CharGrowthStats cur;
    };
    KernelCharRecord *kernelRow; /* +0x28: g_BattleKernelData.chars[charId] */
    int expBase; /* +0x2C: the experience as loaded */
    int exp; /* +0x30: expBase + expEarned, capped at 999,999,999 */
    int expEarned; /* +0x34 */
    int expForLevel; /* +0x38: the experience the loaded level starts at */
    int thresholds[16]; /* +0x3C: the first 16 next-level thresholds the level-up loop computed */
    int expLevelStart; /* +0x7C: the experience the reached level starts at */
    int expNextLevel; /* +0x80: the experience the next level starts at */
} CharGrowth;
typedef char CheckCharGrowth[(sizeof(CharGrowth) == 0x84) ? 1 : -1];

/* Position record (8 bytes) for one die of the Dise limit. */
typedef struct BattleFxDicePosRecord {
    short posX;
    short posY;
    short posZ;
    short spawned;
} BattleFxDicePosRecord;

/* Typhon ring record in the summon's fall ring list. */
typedef struct BattleFxTyphonRingRecord {
    short angle;
    unsigned char pad_02[0x12];
} BattleFxTyphonRingRecord;

/* Fixed-point ramp state: current value, per-frame step and duration. */
typedef struct BattleFixedPointRamp {
    unsigned char pad_00[0x4];
    int value;
    int step;
    int duration;
} BattleFixedPointRamp;

/* Screen flash ramp state: current value and per-frame step. */
typedef struct BattleScreenFlashRamp {
    short ownerSlot;
    unsigned char pad_02[0x2];
    int value;
    unsigned char pad_08[0x4];
    int step;
} BattleScreenFlashRamp;

/* Alexander summon offset pair computed from magnitude and heading. */
typedef struct BattleFxShortOffsetVec {
    short offsetX;
    unsigned char pad_02[0x2];
    short offsetZ;
} BattleFxShortOffsetVec;

/* Per-actor slot of the magic-fire overlay state, with fade flags and aux node translation. */
typedef struct BattleActorTransformSlot {
    unsigned char pad_00[0x2e];
    short auxPosX;
    unsigned char pad_30[0x2];
    short auxPosZ;
    unsigned char pad_34[0x8];
    short boneCount;
    char fadeFlags;
    unsigned char pad_3F[0x1];
} BattleActorTransformSlot;

/* Status tint table entry: a mode byte and an RGB tint copied to the actor record. */
typedef struct BattleStatusTintEntry {
    unsigned char mode;
    char tintR;
    char tintG;
    char tintB;
} BattleStatusTintEntry;

/* Particle record of the Magma special effect; state 0 is free and 1 is live. */
typedef struct BattleSpecMagmaParticleRecord {
    short state;
    short age; /* frames since the particle began to fall; 0x10 retires it */
    short scale;
    unsigned char pad_06[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
} BattleSpecMagmaParticleRecord;

/* Point on the Limit Tifa ring as a radius-scaled cosine and sine pair. */
typedef struct BattleLimitTifaRingPoint {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
} BattleLimitTifaRingPoint;

/* Sprite data record for Magic Gravity 3 whose three words are table fields 1 to 3. */
typedef struct BattleMagicDemi3SpriteData {
    unsigned char pad_00[0x20];
    int spriteObject1;
    int spriteObject2;
    int spriteObject3;
} BattleMagicDemi3SpriteData;

/* The animation a sprite draw request plays: its frame count selects the frame. */
typedef struct BattleFxSpriteFrameTable {
    int pad_00;
    int frameCount;
} BattleFxSpriteFrameTable;

/* Draw request whose three int fields are filled from a loaded table at offsets 0-2. */
typedef struct BattleFxMagicDemi3MultiDrawReq {
    BattleFxSpriteFrameTable *frameTable;
    unsigned char pad_04[0x1C];
    int drawObj0;
    int drawObj1;
    int drawObj2;
} BattleFxMagicDemi3MultiDrawReq;

/* Scene actor whose three shorts are set from the light rig position. */
typedef struct BattleFxNeoBahamutSceneActor {
    unsigned char pad_00[0x166];
    short posX;
    short posY;
    short posZ;
} BattleFxNeoBahamutSceneActor;

/* Per-ring scale value of the Typhon summon fall effect, 0x14-byte records. */
typedef struct BattleFxTyphonRingScale {
    int scale;
    unsigned char pad_04[0x10];
} BattleFxTyphonRingScale;

/* Toseki mesh displacement; offset is accumulated by a velocity table each step. */
typedef struct YasuiBoulderThrowDisplacement {
    short offset;
    short offsetY;
    short offsetZ;
    unsigned char pad_06[0x2];
} YasuiBoulderThrowDisplacement;

/* One 8-byte entry in BattleState.queuedActions[64]: priority level (0..6, 0xFF when empty),
 * FIFO sequence within that level, combatant slot (-1 when invalidated), command id and two
 * 16-bit parameters. */
typedef struct BattleQueuedAction {
    unsigned char level;
    unsigned char sequence;
    char actorIndex;
    char commandId;
    short param;
    short extra;
} BattleQueuedAction;

/* 8-byte entry of the Dia1 sprite table, passed as a billboard argument. */
typedef struct BattleSpecialDia1Entry {
    unsigned char pad_00[0x2];
    short scale;
    unsigned char pad_04[0x4];
} BattleSpecialDia1Entry;

/* Resource slot loaded for SPECIAL/DIA1; resourcePath holds the built resource path. */
typedef struct BattleFxDiamondCardResource {
    int groupTag;
    unsigned char pad_04[0x4];
    int resourcePath;
} BattleFxDiamondCardResource;

/* 8-byte sprite entry of the Todo animated sprites, passed as a billboard argument. */
typedef struct BattleMagicToadSpriteEntry {
    short scale;
    unsigned char pad_02[0x6];
} BattleMagicToadSpriteEntry;

typedef struct BattleSpecialSnowstormScaleVector {
    short x; /* +0x0 */
    short y; /* +0x2 */
} BattleSpecialSnowstormScaleVector;

/* Blue Kaenn sprite draw request; two fields come from the loaded resource table. */
typedef struct BattleFxBlueFlameThrowerDrawReq {
    unsigned char pad_00[0x20];
    int drawObj0;
    int drawObj1;
} BattleFxBlueFlameThrowerDrawReq;

/* Summon Kjata draw handle; its height is interpolated into the wave height. */
typedef struct BattleFxKjataDrawHandle {
    unsigned char pad_00[0x2];
    short height;
    unsigned char pad_04[0x7c];
} BattleFxKjataDrawHandle;

/* Sprite draw request for the Limit ER directional sprite. */
/* Sprite draw row with position, atlas cell and size in 8-unit steps, read by BattleFx_SubmitSpriteDraw. */
typedef struct BattleFxSpriteDrawRow {
    int drawFlags; /* +0x0 */
    short posX;
    short posY;
    short texCol;
    short texRow;
    short textureKey;
    short uvPacked;
    short widthUnits;
    short heightUnits;
} BattleFxSpriteDrawRow;

/* Header of a sprite group: the number of frames. */
typedef struct BattleFxSpriteGroupHeader {
    unsigned char pad_00[0x4];
    int frameCount;
} BattleFxSpriteGroupHeader;

/* Start of a sprite frame: the number of parts it draws. */
typedef struct BattleFxSpriteEntryHead {
    unsigned char pad_00[0x2];
    unsigned short partCount;
} BattleFxSpriteEntryHead;

/* One frame of a sprite group: its head and its draw rows. */
typedef struct BattleFxSpriteEntry {
    BattleFxSpriteEntryHead *head;
    BattleFxSpriteDrawRow *parts;
} BattleFxSpriteEntry;

/* Sprite draw group: the frame header, the frame array, and the textures its rows pick by key. */
typedef struct BattleFxSpriteGroup {
    BattleFxSpriteGroupHeader *header;
    BattleFxSpriteEntry *entries;
    unsigned char pad_08[0x4];
    int entryCount;
    int textureKeys[4]; /* +0x10 */
    struct GfxRenderObject *renderObjects[4]; /* +0x20: the GfxRenderObject for each key */
} BattleFxSpriteGroup;

typedef struct BattleFxSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x00: the BattleFxSpriteGroup (Gfx_ModelTableGetLoadedRecord); 0 draws nothing */
    int drawObj;              /* +0x04: the BattleFxSpriteDrawObj (Gfx_ModelTableGetSpriteObject, Battle3d_GetSceneRenderObject) */
    char colorR;              /* +0x08: the vertex colour; BattleFx_SubmitSpriteDraw doubles it (0x80 = full) */
    char colorG;
    char colorB;
    unsigned char code;       /* +0x0B: the PSX primitive code (0x2C/0x2E in the initialized requests); not read */
    unsigned short frame;     /* +0x0C: the sprite group's frame; bit 15 takes the texture row from uvPacked */
    unsigned short uvPacked;  /* +0x0E: with bit 15 of frame set, uvPacked >> 6 is the packet's +0x80 word */
} BattleFxSpriteRequest;

/* Full-magic particle record; a state of -1 marks a free slot. */
typedef struct BattleMagicFullParticle {
    short state;
    short stepCounter;
    unsigned char pad_04[0x18];
} BattleMagicFullParticle;

/* Sprite request for the Weapon 4 bone 21/23 particle effect. */
typedef struct BattleFxWeapon4BoneReq {
    unsigned char pad_00[0x4];
    int drawObj;
    unsigned char pad_08[0x4];
    short frame;
} BattleFxWeapon4BoneReq;

/* Per-vertex wobble offset written from a base vertex plus a sine term. */
typedef struct BattleMagicBio3WobbleOffset {
    short offsetX;
    short offsetY;
    unsigned char pad_04[0x4];
} BattleMagicBio3WobbleOffset;

/* Per-vertex base position read by the Bio 3 wobbling mesh task. */
typedef struct BattleMagicBio3WobbleBase {
    short baseX;
    short baseY;
    unsigned char pad_04[0x4];
} BattleMagicBio3WobbleBase;

/* Brizag spiral ribbon record: base position, per-frame position triplets and colours. */
#pragma pack(push, 1)
typedef struct BattleFxSpiralRibbon {
    short outerAngleX;
    short outerAngleY;
    short outerAngleZ;
    unsigned char pad_06[0x2];
    short innerAngleX;
    short innerAngleY;
    short innerAngleZ;
    unsigned char pad_0E[0x2];
    union {                    /* +0x10: the outer spin, composed with the camera (a PsxMatrix) */
        unsigned char outerMatrix[0x1E];
        struct {
            unsigned char pad_10[0x12];
            int outerTransX;
            int outerTransY;
            int outerTransZ;
        };
    };
    unsigned char innerMatrix[0x1E]; /* +0x2E: the inner spin (a PsxMatrix) */
    ShadowInt64Pair rotIn0;          /* +0x4C: input of the rotation of each point below */
    union {                    /* +0x54: the ribbon's three points */
        ShadowInt64Pair pos0;
        struct {
            short pos0X;
            short pos0Y;
            short pos0Z;
        };
    };
    ShadowInt64Pair rotIn1;          /* +0x5C */
    union {                    /* +0x64 */
        ShadowInt64Pair pos1;
        struct {
            short pos1X;
            short pos1Y;
            short pos1Z;
        };
    };
    ShadowInt64Pair rotIn2;          /* +0x6C */
    union {                    /* +0x74 */
        ShadowInt64Pair pos2;
        struct {
            short pos2X;
            short pos2Y;
            short pos2Z;
        };
    };
    int colorBase;                  /* +0x7C: colour-table entry 0 */
    union {                    /* +0x80: entry 1, the head's draw colour */
        int colorPrimary;
        struct {
            unsigned char primaryR;
            unsigned char primaryG;
            unsigned char primaryB;
            unsigned char primaryCode;
        };
    };
    int colorSecondary;        /* +0x84: entry 2 */
    union {                    /* +0x88: entry 3, the tail's draw colour */
        int colorTertiary;
        struct {
            unsigned char tertiaryR;
            unsigned char tertiaryG;
            unsigned char tertiaryB;
            unsigned char tertiaryCode;
        };
    };
    int primColorA;            /* +0x8C */
    int primColorB;            /* +0x90 */
    unsigned char pad_94[0x4];
    int projectScratchA;       /* +0x98: unused argument of the triangle projection */
    int projectedDepth;        /* +0x9C */
    int projectScratchB;       /* +0xA0: flags argument of the triangle projection */
    short baseX;               /* +0xA4 */
    unsigned char pad_A6[0x2];
    short baseY;
    unsigned char pad_AA[0x2];
    short baseZ;
} BattleFxSpiralRibbon;
#pragma pack(pop)

/* Atlas slot of a battle model texture: CLUT and page ids, UV bounds, sizes and atlas origin. */
typedef struct Battle3dLmdTextureSlot {
    int state;            /* +0x00: 0 until a polygon first uses the slot (Battle_BuildPolygonDataFromLmd) */
    int clutId;           /* +0x04 */
    int texturePage;      /* +0x08 */
    union {               /* +0x0C..+0x1B: the UV bounds of the slot's polygons; the polygon builder */
        float uMin;       /*   moves them as float bits, Lmd_BuildSlotTexture reads them as floats */
        int uMinBits;
    };
    union {
        float uMax;
        int uMaxBits;
    };
    union {
        float vMin;
        int vMinBits;
    };
    union {
        float vMax;
        int vMaxBits;
    };
    float uSize;          /* +0x1C */
    float vSize;          /* +0x20 */
    int slotIndex;        /* +0x24: the slot's index in the face data's slot list */
    union {               /* +0x28: the slot's origin in the atlas; the builder copies it from the UV bounds as bits */
        float atlasU;
        int atlasUBits;
    };
    union {
        float atlasV;
        int atlasVBits;
    };
    union {               /* +0x30: the size rounded up to a power of two; the polygon builder reads */
        float uSizePow2;  /*   it as float bits */
        int uSizePow2Bits;
    };
    union {
        float vSizePow2;
        int vSizePow2Bits;
    };
} Battle3dLmdTextureSlot;

/* LMD face-data record: the slot list and its 100 atlas slots, then the CLUT and texture-page VRAM bounds. */
typedef struct Battle3dLmdFaceData {
    int faceKind;
    int fileName;
    struct List *slotList;
    Battle3dLmdTextureSlot slots[100];
    int polygonsBuilt;
    int clutMinX;
    int clutMinY;
    int clutMaxX;
    int clutMaxY;
    int tpageMinX;
    int tpageMinY;
    int tpageMaxX;
    int tpageMaxY;
    int allowLargeAtlas;
    struct PfileLoadOptions *loadOptions;
} Battle3dLmdFaceData;

/* Three-short point in camera space: world point minus the effect transform translation. */
typedef struct BattleCameraLocalPoint2 {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
    int pad_08;
    int pad_0C;
    int pad_10;
} BattleCameraLocalPoint2;

/* Three-short camera vector read from a camera script offset table. */
typedef struct BattleCameraScriptVec {
    short x;
    unsigned char pad_02[0x2];
    short y;
    unsigned char pad_06[0x2];
    short z;
} BattleCameraScriptVec;

/* PSX quad primitive for one Queig shard, filled from a shard record; tag word 0x09000000. */
typedef struct BattleFxShardQuadPrim {
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
    int flags;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleFxShardQuadPrim;

/* PSX triangle primitive for a Queig shard, filled from its textured record and the shard. */
typedef struct BattleFxShardTriPrim {
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
    int flags;
    unsigned char pad_18[0x4];
    short uv2;
} BattleFxShardTriPrim;

/* Sting mesh primitive: tag word and vertex words copied from the mesh header. */
typedef struct BattleFxRubyTentaclesMeshPacket {
    int tag;
    union {
        int color0;
        struct {
            unsigned char rgb[3];
            char gpuCode;
        };
    };
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxRubyTentaclesMeshPacket;

/* Polygon-block packet written by BattleFx_DrawPsxPolygonBlock: tag word and copied words. */
typedef struct BattleFxPsxPolygonPacket {
    int tag;
    union {
        int colorCmd;
        struct {
            unsigned char rgb[3];
            char gpuCode;
        };
    };
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
} BattleFxPsxPolygonPacket;

/* Bio3 PSX mesh packet: tag word and words copied from the mesh data. */
typedef struct BattleFxBio3PsxPacket {
    int tag;
    union {
        int primWord;
        CVECTOR color0; /* +0x4: cd is the GPU command code */
    };
    unsigned char pad_08[0x4];
    union {
        int colorWord;
        CVECTOR color1;
    };
    unsigned char pad_10[0x4];
    union {
        int coordWord;
        CVECTOR color2;
    };
    unsigned char pad_18[0x4];
    int color3;
} BattleFxBio3PsxPacket;

/* Ifrit PSX mesh packet: tag word and words copied from the entry data. */
typedef struct BattleFxPsxQuadPacket {
    int tag;
    union {
        int primWord;
        CVECTOR colorVec;
        struct {
            unsigned char colorRGB[3];
            char primCode; /* the GPU command byte */
        };
    };
    unsigned char pad_08[0x4];
    union { int color1; CVECTOR color1Vec; };
    unsigned char pad_10[0x4];
    union { int color2; CVECTOR color2Vec; };
    unsigned char pad_18[0x4];
    union { int color3; CVECTOR color3Vec; };
} BattleFxPsxQuadPacket;

/* Scratch task record shared by the Bahamut beam, Neo Bahamut ground and Kotr summon timeline tasks. */
typedef struct BattleSummonTaskRecord {
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_06[0x2];
    int scaleX;
    int scale;
    int scaleZ;
    unsigned char pad_14[0x14];
    int projectResult;
    unsigned char pad_2C[0xc];
    int transX;
    int transY;
    int transZ;
} BattleSummonTaskRecord;

/* Ramuh summon camera-script mesh slot: bone pair and a world-space point. */
typedef struct BattleSummonRamuhCameraSlot {
    short mode;
    short frameCount;
    unsigned char pad_04[0x8];
    short boneIndex;
    short secondBoneIndex;
    int cameraPos;
    int cameraPosZ;
    unsigned char pad_18[0x10];
} BattleSummonRamuhCameraSlot;

/* Ramuh summon camera-script saved actor slot: bone pair and a world-space point. */
typedef struct BattleSummonRamuhSavedActor {
    short mode;
    short frameCount;
    unsigned char pad_04[0x8];
    short boneIndex;
    short secondBoneIndex;
    int focalPoint;
    int focalPointZ;
    unsigned char pad_18[0x10];
} BattleSummonRamuhSavedActor;

/* Summon state: Kotr layer draw colour and geometry offset; Hades ring particle fields. */
#pragma pack(push, 1)
typedef struct BattleFxSummonState {
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char pad_03[0x5];
    struct BattleFxSpriteGroup *spriteGroup;
    int drawObj;
    int colorWord;
    short age;
    unsigned char pad_16[0x8];
    int transX;
    int transY;
    int transZ;
    unsigned char pad_2A[0x2];
    int offsetX;
    int offsetY;
} BattleFxSummonState;
#pragma pack(pop)

/* One 0xC-byte pending hit reaction (g_BattleHitReactionRecords, five of them), one per hit display
 * slot: queued from the damage display table and read into the hit-reaction task slot. */
typedef struct BattleHitReactionRecord {
    short targetParam;
    short damage;
    short damageValue;
    short linkIndex;
    short reactionParam;
    union {
        short hitRecordIndex;
        char hitKind;
    };
} BattleHitReactionRecord;

/* Kjata scaling state: first value set from s, then fixed at 0x2000; then the transform the
 * scale is applied to. */
typedef struct BattleFxKjataScalingState {
    int scale;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix; /* +0x10 */
} BattleFxKjataScalingState;

/* Alexander gradient triangle record, written by its task. */
typedef struct BattleFxPsxGouraudTriPacket {
    int tag;
    union {
        int colorWord0;
        struct {
            unsigned char colorRGB[3];
            char code; /* the GPU command byte */
        };
    };
    unsigned char pad_08[0x4];
    int colorWord1;
    unsigned char pad_10[0x4];
    int colorWord2;
} BattleFxPsxGouraudTriPacket;

/* Odin Zantetsuken camera script table: the target bone position, then the operand pool a
   negative script operand -k reads as (&operands)[k] (element 1 is frames). */
typedef struct BattleFxOdinZCameraScript {
    union {
        struct {
            short posX;
            short posY;
            short posZ;
            short operands;
        };
        struct {
            int posXYWord;
            int posZWord;
        };
    };
    short frames;
    unsigned char pad_a[0x6];
} BattleFxOdinZCameraScript;

/* Odin Zantetsuken task slot 7 record, written by the target sequence task. */
typedef struct BattleFxOdinZTaskSlot {
    union {
        struct {
            short cameraPosX;
            short cameraPosY;
        };
        int xyWord;
    };
} BattleFxOdinZTaskSlot;

/* Bahamut Zero sprite record, with a table field and a position word. */
typedef struct BattleFxFlareSpriteSlot {
    unsigned char pad_00[0x8];
    int tableValue;
    unsigned char pad_0C[0x6];
    short posX;
} BattleFxFlareSpriteSlot;

/* Bio2 / Bio3 magic mesh record: vertex slot offsets, shade bytes and PSX primitive words. */
#pragma pack(push, 1)
typedef struct BattleMagicBio3MeshRecord {
    short numPrims;
    short vertexBOffset;
    short vertexCOffset;
    short shadeA;
    union {
        int primWord;
        CVECTOR color0;
    };
    union {
        struct {
            short primParamA;
            short primParamB;
        };
        int colorWord;
        CVECTOR color1;
    };
    union {
        struct {
            short vertexAOffset;
            short shadeB;
        };
        int coordWord;
        CVECTOR color2;
    };
    union {
        struct {
            short primOffset;
            short altVertexOffset;
        };
        int color3;
        CVECTOR color3Vec;
    };
    short altPrimOffset;
    short nextVertexBOffset;
    short nextVertexCOffset;
} BattleMagicBio3MeshRecord;
#pragma pack(pop)



/* Camera vector (x, y, z) computed from the summon camera state plus block offsets. */
typedef struct BattleFxCameraVec {
    int x;
    int y;
    int z;
} BattleFxCameraVec;

/* Typhon billboard sprite task record that counts frames against a particle parameter. */
typedef struct BattleFxTyphonBillboardRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x14];
    short scale;
} BattleFxTyphonBillboardRecord;

/* Scene message read by the drain loop: a short vector, then an effect-order value. */
typedef struct Battle3dSceneMessage {
    short actor;               /* +0x00: the actor the record is for (read as *(short *)p by the drain loop) */
    unsigned char pad_02[0x2];
    short vectorX;
    short vectorY;
    short vectorZ;
    unsigned char pad_0A[0x6];
    short effectOrder;
} Battle3dSceneMessage;

/* Geometry header for the shrinking spinning triangle draw, with flags, brightness and texture bits. */
typedef struct BattleFxShrinkGeometry {
    int blockPtr;
    int flags;
    unsigned char pad_08[0x2];
    short brightness;
    short textureBits;
} BattleFxShrinkGeometry;

/* Geometry header for the SNova ranked triangle mesh draw, with flags, brightness and texture bits. */
typedef struct BattleFxSuperNovaGeometry {
    int recordBlock;
    int flags;
    unsigned char pad_08[0x2];
    short brightness;
    short textureBits;
} BattleFxSuperNovaGeometry;

/* Four vertex byte offsets (unsigned short) that index the vertex base for one quad. */
typedef struct BattleFxSatanSlamIndexEntry {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned short vertexOffset3;
} BattleFxSatanSlamIndexEntry;

/* Row block describing a run of PSX Gouraud quads for Altema. */
typedef struct BattleFxPsxGouraudRowBlock {
    unsigned char pad_00[0x8];
    int primPtr;
    unsigned char pad_0C[0x30];
    int rowStep;
    int rowCount;
    int packedRgb;
} BattleFxPsxGouraudRowBlock;

/* Animated sprite record whose frame index advances per draw. */
typedef struct BattleFxShivaSpriteRecord {
    short ownerSlot;
    short frameIndex;
} BattleFxShivaSpriteRecord;

/* Bahamut Zero ground spark record with a frame index. */
typedef struct BattleFxBzGroundLivewireSlot {
    short ownerSlot; /* +0x0 */
    short frameIndex;
    unsigned char pad_04[0x18];
    short aimAngle;
} BattleFxBzGroundLivewireSlot;

/* Three vertex byte offsets in a Debaria shatter record used to copy 8-byte vertex data into fragments. */
typedef struct BattleFxDeBarrierDrawOffsets {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
} BattleFxDeBarrierDrawOffsets;

/* Three-int bone vector copied from a skeleton bone. */
typedef struct Battle3dBoneVector {
    int vectorX;
    int vectorY;
    int vectorZ;
} Battle3dBoneVector;

/* Float end vertex of the Roket interpolation callback. */
typedef struct BattleFxHellBomberEndVertex {
    float posX;
    float posY;
    float posZ;
} BattleFxHellBomberEndVertex;

/* Velocity source for the Comet descending model, scaled by one sixth. */
typedef struct BattleFxVelocityVec3 {
    int velocityX;
    int velocityY;
    int velocityZ;
} BattleFxVelocityVec3;

/* Blended polygon-part position written as three floats t, u, v (fixed8 blend). */
typedef struct BattleFxSuperNovaPosFixed8 {
    float t;
    float u;
    float v;
} BattleFxSuperNovaPosFixed8;

/* Eva polygon-set vertex written as three floats r, s, t. */
typedef struct BattleFxGreatGospelPolygonVertex2 {
    float r;
    float s;
    float t;
} BattleFxGreatGospelPolygonVertex2;

/* A rain drop's splash state (Battle3d_RainUpdateAndDraw): 0 while the drop falls, then the
 * splash animation frame up to 8; and the splash's texture column. */
typedef struct Battle3dRainRecord {
    unsigned char splashFrame;
    unsigned char splashTexV;
} Battle3dRainRecord;

/* Skeleton header holding a record count at +0x10 and a record pointer at +0x34. */
typedef struct BattleSkeletonTextureHeader {
    unsigned char pad_00[0x10];
    int recordCount;
    unsigned char pad_14[0x20];
    int records;
} BattleSkeletonTextureHeader;

/* Ifrit mesh record; its words are copied into polygon and last primitives. */
typedef struct BattleFxPsxMeshPacket {
    unsigned char pad_00[0xc];
    union { int color1; CVECTOR color1Vec; };
    union { int color2; CVECTOR color2Vec; };
    union { int color3; CVECTOR color3Vec; };
} BattleFxPsxMeshPacket;

/* Orthonormal 3x3 axis matrix (nine shorts, pad at 0x0C) built from a Y axis by the orthonormaliser. */
typedef struct BattleOrthoMatrix {
    short firstAxis;
    short xVector;
    short crossOut;
    short axisNeg;
    short axisY;
    short crossY;
    short firstAxisZ;
    short axisZ;
    short crossZ;
} BattleOrthoMatrix;

/* Rotating sprite pair state; the angle is derived from the step index. */
typedef struct BattleFxRotatingSpritePair {
    short rotX;
    short rotY;
    short angle;
} BattleFxRotatingSpritePair;

/* Rotation vector for a tornado strip particle; angle is the strip angle. */
typedef struct BattleFxVortexStripAngle {
    short angleX;
    short angle;
    short angleZ;
} BattleFxVortexStripAngle;

/* Matrix record; its shorts at 0x12, 0x16, 0x1A are read as a vector by the Shiva and Catastro attach-point tasks. */
typedef struct BattleFxMatrixVector {
    unsigned char pad_00[0x12];
    short x;
    unsigned char pad_14[0x2];
    short y;
    unsigned char pad_18[0x2];
    short z;
} BattleFxMatrixVector;

/* One enemy's placement in the battle scene (BattleSceneState.enemies, six of them, read as
 * (actor - 4) << 4 from +0x4C): the enemy record it uses, where it stands, its start flags.
 * The battle opening copies posX/posY/posZ and flags into the formation entries. */
typedef struct BattleSceneEnemy {
    short enemyIndex; /* +0x00: index into g_BattleState.enemyRecords (0xB8 bytes each); -1 none */
    short posX; /* +0x02 */
    short posY; /* +0x04 */
    short posZ; /* +0x06 */
    unsigned char initByte; /* +0x08 */
    unsigned char pad_09[0x1];
    unsigned short statusMask; /* +0x0A */
    int flags; /* +0x0C: the enemy's record takes the low five bits */
} BattleSceneEnemy;

/* One actor's slot in the battle scene (BattleSceneState.actors, one per battle actor, read as
 * actor << 4 from +0xAC). */
typedef struct BattleSceneActor {
    char unitIndex; /* +0x00: 0xFF none */
    char scriptId; /* +0x01: the actor's idle script */
    char deathSentenceTimer; /* +0x02 */
    char slowNumbTimer; /* +0x03 */
    unsigned char state; /* +0x04 */
    unsigned char pad_05[0x1];
    unsigned short flags; /* +0x06 */
    int savedStatus; /* +0x08 */
    int statusFlags; /* +0x0C */
} BattleSceneActor;

/* hub BattleSceneState begin */
/* The battle scene: the formation's setup and camera data, six enemy placements, ten actor slots, then the targeting masks and the turn state. */
#pragma pack(push, 1)
struct BattleSceneState {
    short enemyIds[3]; /* +0x0: the formation's up to three distinct enemy model ids */
    unsigned char pad_6[0x2];
    unsigned short locationId; /* +0x8: compared with battle location ids (0x4E) */
    unsigned short nextScene; /* +0xa: the scene the phase machine's next-battle step loads */
    short setupSceneMode; /* +0xc: the formation's setup record sceneMode */
    unsigned char pad_e[0x2];
    unsigned short arenaOpponents[4]; /* +0x10: the Battle Square / Colosseum opponent scene ids, 0xFFFF for an empty slot */
    unsigned short setupFlags; /* +0x18: the formation's setup record flags */
    unsigned char layout; /* +0x1a */
    char cameraScriptIndex; /* +0x1b: opening camera script index */
    BattleActorCameraOffsets formationCamera[4]; /* +0x1c: the formation's camera poses */
    BattleSceneEnemy enemies[6]; /* +0x4c: the enemies' placements, indexed actor - 4 */
    BattleSceneActor actors[10]; /* +0xac: one slot per battle actor */
    unsigned short targetMaskBits; /* +0x14c */
    unsigned short targetMask; /* +0x14e */
    short targetValidMask; /* +0x150 */
    short targetExtraMask; /* +0x152 */
    unsigned char pad_154[0x2];
    unsigned short readyActorMask; /* +0x156 */
    unsigned char pad_158[0x2];
    unsigned short incapacitatedMask; /* +0x15a: actors unable to act or be targeted */
    short aiTargetableMask; /* +0x15c: actors eligible for AI targeting */
    unsigned short flags; /* +0x15e */
    unsigned short roundCounter; /* +0x160 */
    unsigned short targetMaskTable[0x3]; /* +0x162: indexed by the target selection (Battle_ResolveTargetMask) */
    unsigned char squareSlots[0x4]; /* +0x168 */
    unsigned char turnQueueSlots[0x8]; /* +0x16c */
    unsigned short occupiedEnemySideCount; /* +0x174: occupied enemy sides (0..2) in pincer/side layouts */
    unsigned char pad_176[0x2];
};
#pragma pack(pop)
/* hub BattleSceneState end */

/* One battle actor's state record (BattleState.actors, one per battle actor, read as
 * actor * 0x44): the ATB gauge and its rates, status timers and stat modifiers, status flags. */
typedef struct BattleActorRuntime {
    unsigned short speedRate; /* +0x00 */
    unsigned short atbTick; /* +0x02 */
    short atbGauge; /* +0x04 */
    short slipRate; /* +0x06 */
    unsigned short limitFlags; /* +0x08: bit 0 = limit ready (Battle_SetLimitReady, Battle_ClearLimitReady) */
    short tickRate; /* +0x0A */
    unsigned char linkedActor; /* +0x0C: 0xFF none (both inits); Battle_AiVm_CopyCombatantState stores the copy's actor and target resolution adds its bit */
    char accessoryKind; /* +0x0D: 0xFF for an enemy (Battle_InitEnemyCombatants) */
    char dirtyFlags; /* +0x0E: or'ed with a dirty mask */
    char nameSuffixIndex; /* +0x0F: 0xFF none; an enemy's count of earlier enemies of its type, the letter after its name */
    char statusTimers[4]; /* +0x10 */
    unsigned char gaugeRate; /* +0x14 */
    unsigned char gaugeUnit; /* +0x15 */
    unsigned char poisonTimer; /* +0x16 */
    unsigned char pad_17[0x9];
    char statPercent[8]; /* +0x20: a stat's percent modifier, base + base * statPercent[stat] / 100 */
    char reflectState; /* +0x28 */
    unsigned char timingFlags; /* +0x29: the party loader sets 1 */
    unsigned char turnCount; /* +0x2A: the actions run and turns closed for this actor (Battle_ExecuteAction, Battle_CloseActorTurn) */
    unsigned char sceneState; /* +0x2B: the scene actor's state, copied when the battle starts; nothing reads it */
    int pendingHpAmount; /* +0x2C: HP damage queued for the actor; negative heals (Battle_FlushHpMpTransferPool) */
    int pendingMpAmount; /* +0x30: MP damage queued for the actor; negative heals */
    int statusImmunityMask; /* +0x34: Battle_BuildStatusImmunityMask starts from it; an enemy's is ~BattleEnemyRecord.statusImmunityMask */
    int enemyRecord; /* +0x38: the enemy's BattleEnemyRecord (Battle_InitEnemyCombatants); the action queue reads it as a BattleActorQueueRecord */
    unsigned short hp; /* +0x3C: the low half of the combatant's curHp, Battle_SnapshotHpMp */
    short mp; /* +0x3E: the combatant's mp, Battle_SnapshotHpMp */
    unsigned char pad_40[0x4];
} BattleActorRuntime;

/* hub BattleState begin */
/* The battle's per-actor state (0x44 a record), then the party's stat records, the action queue and the ATB and turn words. */
#pragma pack(push, 1)
/* 8-byte active action slot within BattleState (+0xfcc..+0xfdc). */
#pragma pack(push, 1)
typedef struct BattleActiveActionSlot {
    unsigned char pad_00;
    unsigned char actionType;
    char actor;
    char commandId;
    short param;
    unsigned short extra;
} BattleActiveActionSlot;
#pragma pack(pop)

struct BattleState {
    BattleActorRuntime actors[10]; /* +0x0: one record per battle actor, actor * 0x44 */
    BattlePartyActor partyStats[3]; /* +0x2a8: the party records (BattleState.partyStats) */
    BattleWeaponActionRow weaponActionRows[3]; /* +0x344: the party's weapon rows (g_BattleState.weaponActionRows) */
    BattleEnemyRecord enemyRecords[3]; /* +0x38c (g_BattleState.enemyRecords) */
    unsigned char attackEntries[0x380]; /* +0x5b4: attack records of 0x1C */
    unsigned short attackIds[0x20]; /* +0x934: the scene's attack ids */
    unsigned char sceneRecordBlockD[0x400]; /* +0x974 */
    unsigned char currentActionKind; /* +0xd74 */
    unsigned char pad_d75[0x1];
    char queuedActorSlot; /* +0xd76 */
    char queuedBaseCommandId; /* +0xd77 */
    short actionQueueScratch; /* +0xd78 */
    short actionQueueReset; /* +0xd7a */
    BattleQueuedAction queuedActions[64]; /* +0xd7c: 64 pending action slots */
    char actorResumeMarker; /* +0xf7c */
    unsigned char pad_f7d[0x1f];
    char enemyInitSlotId[6][8]; /* +0xf9c: eight bytes per enemy; [0] is the slot's id, 0xFF none */
    union {
        struct {
            unsigned char pad_fcc[0x1];
            unsigned char actionSlotType; /* +0xfcd */
            char actionSlotActor; /* +0xfce */
            char actionSlotCommandId; /* +0xfcf */
            short actionSlotParam; /* +0xfd0 */
            unsigned short actionSlotExtra; /* +0xfd2 */
            unsigned char pad_fd4[0x2];
            char actionSlotTargetIndex; /* +0xfd6 */
            unsigned char pad_fd7[0x5];
        };
        BattleActiveActionSlot activeActions[2]; /* +0xfcc..+0xfdc: two 8-byte active action slots */
    };
    unsigned char actionRunSequence[7]; /* +0xfdc: next sequence number to run per priority level 0..6 */
    unsigned char actionEnqueueSequence[7]; /* +0xfe3: next sequence number to assign per priority level 0..6 */
    unsigned char pad_fea[0x2];
    unsigned char sceneRecordBlockE[0x1202]; /* +0xfec */
    unsigned short atbSpeedValue; /* +0x21ee */
    unsigned short atbBaseDivisor; /* +0x21f0: party average speed + 50 divisor for ATB tick rate */
    short atbMode; /* +0x21f2: 0 = Active, 1 = Recommended, 2 = Wait */
    unsigned short actorCommandPendingMask; /* +0x21f4 */
    short atbTickCountA; /* +0x21f6 */
    short atbTickCountB; /* +0x21f8 */
    short sceneMode; /* +0x21fa */
    short atbAccumulator; /* +0x21fc */
    unsigned short scenePrevMode; /* +0x21fe */
    short actionPending; /* +0x2200 */
    unsigned short runningActionPriority; /* +0x2202: priority level of the currently running queued action */
    unsigned short pendingAiEventMask; /* +0x2204: bitmask of queued AI script event indices */
    short messageWindowRequest; /* +0x2206 */
    short limitBanner; /* +0x2208 */
    unsigned short actorCommandIssuedMask; /* +0x220a */
    short actorAutoControlMask; /* +0x220c */
    unsigned short statusChangeMode; /* +0x220e */
    unsigned short layoutType; /* +0x2210 */
    unsigned short actorMenuSuppressMask; /* +0x2212 */
    unsigned short excludedActorMask; /* +0x2214 */
    unsigned short absentActorMask; /* +0x2216 */
    unsigned short turnRandomWord; /* +0x2218: per-turn 16-bit random sample */
    unsigned short participatingPartyMask; /* +0x221a: bitmask of party slots 0..2 present in battle */
    unsigned char enemyInitCount[3]; /* +0x221c: how many enemies use each of the scene's three enemy records */
    unsigned char pad_221f[0x1];
    short currentActorBitMask; /* +0x2220 */
    short actor; /* +0x2222 */
    unsigned char pad_2224[0x2];
    short selectedTargetIndex; /* +0x2226 */
    short deathClearedMask; /* +0x2228 */
    unsigned short lucky7Counter; /* +0x222a */
};
#pragma pack(pop)
/* hub BattleState end */

#pragma pack(push, 1)
typedef struct BattleBlueFrogSongState {
    int actorIndex; /* +0x0 */
    int nextNoteIndex; /* +0x4 */
} BattleBlueFrogSongState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxBadBreathParticleRing {
    int ringIndex; /* +0x0 */
    unsigned char pad_4[0x4];
    int posX; /* +0x8 */
    int posZ; /* +0xc */
} BattleFxBadBreathParticleRing;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecTailLaserSourceBonePos {
    short sourceX; /* +0x0 */
    short sourceY; /* +0x2 */
} BattleSpecTailLaserSourceBonePos;
#pragma pack(pop)

/* g_BattleBluePandorasBoxFragmentRecords (src/ff7/src/battle/battle_fx_blue_pandoras_box_effect.cpp): the 0x1f40-byte particle pool. */
#pragma pack(push, 1)
/* One of the 400 particles of the Nanntoka effect (0x14 bytes); the particle field spawns up to 300. */
typedef struct BattleBluePandorasBoxFragment {
    short posX;
    short posY;
    short posZ;
    short age;      /* 0: free slot; 1..9: still rising; 10 and up: drawn */
    short velocityX;
    short velocityY;
    short velocityZ;
    short step;
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_13[0x1];
} BattleBluePandorasBoxFragment;

typedef struct BattleBluePandorasBoxFragmentRecords {
    BattleBluePandorasBoxFragment fragments[400];
} BattleBluePandorasBoxFragmentRecords;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecMagmaDrawListHead {
    short originX; /* +0x0 */
    short originY; /* +0x2 */
} BattleSpecMagmaDrawListHead;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicDethperFragmentBuffer {
    int headIndex; /* +0x0 */
    int startIndex; /* +0x4 */
    int count; /* +0x8 */
    int limit; /* +0xc */
} BattleMagicDethperFragmentBuffer;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleModelEnemyBlendOverrides {
    unsigned char pad_0[0x13];
    unsigned char enemyBlendOverrideGroup5Byte19; /* +0x13 */
    unsigned char pad_14[0xa];
    unsigned char enemyBlendOverrideGroup5Byte30; /* +0x1e */
    unsigned char pad_1f[0x7];
    unsigned char enemyBlendOverrideGroup5Byte38; /* +0x26 */
    unsigned char pad_27[0x1];
} BattleModelEnemyBlendOverrides;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleOverlayRootLoadState {
    short loadState; /* +0x0 */
    short rootActorId; /* +0x2 */
} BattleOverlayRootLoadState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleCameraPosition {
    union {
        struct {
            short cameraPosX; /* +0x0 */
            short cameraPosY; /* +0x2 */
        };
        int xyWord;
    };
} BattleCameraPosition;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleCameraFocalPoint {
    union {
        struct {
            short focalPointX; /* +0x0 */
            short focalPointY; /* +0x2 */
        };
        int xyWord;
    };
} BattleCameraFocalPoint;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBubblesParticleRecords {
    int recordBase; /* +0x0 */
    int angle4; /* +0x4 */
    unsigned char pad_8[0x4];
} BattleSpecHellBubblesParticleRecords;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecRadiantBreathBuffer {
    short vx; /* +0x0 */
    short vy; /* +0x2 */
} BattleSpecRadiantBreathBuffer;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeatBlastActorBonePos {
    short bonePosX; /* +0x0 */
    short bonePosY; /* +0x2 */
} BattleSpecHeatBlastActorBonePos;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecPetrifyCoordData {
    short deltaX; /* +0x0 */
    short deltaY; /* +0x2 */
} BattleSpecPetrifyCoordData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialScorchCentroidPos {
    short centroidX; /* +0x0 */
    short y; /* +0x2 */
} BattleSpecialScorchCentroidPos;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSpiderWebLineRecords {
    int recordBase; /* +0x0 */
    int pad_04; /* +0x4 */
} BattleSpecialSpiderWebLineRecords;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialEnergyOrbCentroidPos {
    short centroidX; /* +0x0 */
    short centroidY; /* +0x2 */
} BattleSpecialEnergyOrbCentroidPos;
#pragma pack(pop)

/* One 0x18-byte record of the Alexander summon's sprite buffer: state is 0 when claimed and -1 when finished. */
typedef struct BattleSummonAlexanderParticleRecord {
    short state;
    short word;
    unsigned char pad_04[0x14];
} BattleSummonAlexanderParticleRecord;

typedef struct BattleSummonAlexanderParticleBuffer {
    BattleSummonAlexanderParticleRecord records[200];
} BattleSummonAlexanderParticleBuffer;

#pragma pack(push, 1)
typedef struct BattleSummonAlexanderSubState {
    short vertexAX; /* +0x0 */
    short vertexAY; /* +0x2 */
    short vertexAZ; /* +0x4 */
    unsigned char pad_6[0x2];
    int vertexB; /* +0x8 */
    unsigned char pad_c[0x4];
    int vertexC; /* +0x10 */
    unsigned char pad_14[0x4];
} BattleSummonAlexanderSubState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonAlexanderTable {
    struct BattleFxAlexanderScalingScratch *scalingScratch; /* +0x0 */
    char shardTaskInit; /* +0x4 */
    unsigned char pad_5[0x1];
    short operandTable; /* +0x6 */
} BattleSummonAlexanderTable;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonShivaRingColor {
    short ringColor; /* +0x0 */
    short ringStep; /* +0x2 */
} BattleSummonShivaRingColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxFallMotionParams {
    short actorIndex; /* +0x0 */
    short boneIdx; /* +0x2 */
} BattleFxFallMotionParams;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MateriaStatPercentModifiers {
    union {
        short byIndex[12]; /* the same shorts by number, as the kernel's stat-selector bytes index them */
        struct {
            short scratchStrength; /* +0x0 */
            short vitalityPercent; /* +0x2 */
            short magicPercent; /* +0x4 */
            short spiritPercent; /* +0x6 */
            short dexterityPercent; /* +0x8 */
            short luckPercent; /* +0xa */
            short attackPercent; /* +0xc */
            short defensePercent; /* +0xe */
            short maxHpPercent; /* +0x10 */
            short maxMpPercent; /* +0x12 */
            short expPercent; /* +0x14 */
            char coverChance; /* +0x16 */
            unsigned char pad_17[0x1];
        };
    };
} MateriaStatPercentModifiers;
#pragma pack(pop)

/* g_BattleBlueFlameThrowerDataBlockPtr (src/ff7/src/battle/battle_fx_blue_flame_thrower_attach_effect.cpp): 0xc bytes, the scratch block pointer and the start of the particle pool. */
#pragma pack(push, 1)
typedef struct BattleBlueFlameThrowerDataBlockPtr {
    struct BattleBlueFlameThrowerScratch *dataBlock; /* +0x0: the scratch block from Gfx_GetPsxScratchpadPtr(0) */
    unsigned char pad_4[0x4];
    int particlePool; /* +0x8: the first word of the 200 particle records (BattleFxFlameThrowerParticle) that start here */
} BattleBlueFlameThrowerDataBlockPtr;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueShadowFlareSpriteReq1 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int sfleaGrowSpriteHandle; /* +0x4 */
    char sfleaGrowFadeByte8; /* +0x8 */
    char sfleaGrowFadeByte9; /* +0x9 */
    char sfleaGrowFadeByte10; /* +0xa */
    unsigned char pad_b[0x1];
    short sfleaGrowFrameIndex; /* +0xc */
} BattleBlueShadowFlareSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueShadowFlareSpriteReq2 {
    int sfleaSpriteGroup; /* +0x0 */
    int sfleaFrameTableHandle; /* +0x4 */
    unsigned char pad_8[0x4];
    short sfleaFrameTableIndex; /* +0xc */
} BattleBlueShadowFlareSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBlueShadowFlareSpriteReq3 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int sfleaSparkSpriteHandle; /* +0x4 */
    char sfleaSparkByte8; /* +0x8 */
    char sfleaSparkByte9; /* +0x9 */
    char sfleaSparkByte10; /* +0xa */
    unsigned char pad_b[0x1];
    short sfleaSparkFrameWord; /* +0xc */
} BattleBlueShadowFlareSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxParticleAttachmentOffsetMatrix {
    unsigned char pad_0[0x1a];
    int transZ; /* +0x1a */
} BattleFxParticleAttachmentOffsetMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxParticleQuadTransformMatrix {
    unsigned char pad_0[0x12];
    int transX; /* +0x12 */
    unsigned char pad_16[0x4];
    int transZ; /* +0x1a */
} BattleFxParticleQuadTransformMatrix;
#pragma pack(pop)

/* g_BattleLimitFlashModel1MatrixTail (src/ff7/src/battle/battle_limit_fx_thunks.cpp): 0xe bytes, the end of the Limit flash sine task's rotation matrix: element [2][2], then the translation. */
#pragma pack(push, 1)
typedef struct BattleLimitFlashModel1MatrixTail {
    short matrixM22; /* +0x0 */
    int transX; /* +0x2 */
    int transY; /* +0x6 */
    int transZ; /* +0xa */
} BattleLimitFlashModel1MatrixTail;
#pragma pack(pop)

/* g_BattleLimitFlashModel2MatrixTail (src/ff7/src/battle/battle_limit_fx_thunks.cpp): 0xe bytes, the end of the Limit flash shrink task's rotation matrix: element [2][2], then the translation. */
#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryAnimatedSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int animatedBillboardRes; /* +0x4 */
    unsigned char pad_8[0x4];
    short animatedBillboardShort12; /* +0xc */
    unsigned char pad_e[0x2];
} BattleLimitCosmoMemoryAnimatedSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryFadingSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int fadingBillboardRes; /* +0x4 */
    char fadingBillboardAlphaOld2; /* +0x8 */
    char fadingBillboardAlphaOld1; /* +0x9 */
    char fadingBillboardAlpha; /* +0xa */
    unsigned char pad_b[0x1];
    short fadingBillboardFlag; /* +0xc */
    unsigned char pad_e[0x2];
} BattleLimitCosmoMemoryFadingSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryGrowingModelRequest {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short recordFrameAngleA; /* +0xa */
    unsigned char pad_c[0x4];
} BattleLimitCosmoMemoryGrowingModelRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryCentroidModelRequest {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short recordFrameAngleB; /* +0xa */
    unsigned char pad_c[0x4];
} BattleLimitCosmoMemoryCentroidModelRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryShrinkingModelRequest {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short recordStateAngleA; /* +0xa */
    unsigned char pad_c[0x4];
} BattleLimitCosmoMemoryShrinkingModelRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryExpandingModelRequest {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short recordTableOffset; /* +0xa */
    unsigned char pad_c[0x4];
} BattleLimitCosmoMemoryExpandingModelRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryConvergingSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int spriteResIndex30; /* +0x4 */
    unsigned char pad_8[0x4];
    short spriteStepParity; /* +0xc */
    unsigned char pad_e[0x2];
} BattleLimitCosmoMemoryConvergingSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryRingSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int recordResIndex28; /* +0x4 */
    unsigned char pad_8[0x8];
} BattleLimitCosmoMemoryRingSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryTriSpinModelRequest {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short recordStateAngleB; /* +0xa */
    unsigned char pad_c[0x4];
} BattleLimitCosmoMemoryTriSpinModelRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitCosmoMemoryTriSpinSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int recordResIndex28B; /* +0x4 */
    unsigned char pad_8[0x8];
} BattleLimitCosmoMemoryTriSpinSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitDragonDiveModelDrawRequest {
    int resource; /* +0x0 */
    unsigned char pad_4[0xc];
    short impactFrames; /* +0x10 */
    unsigned char pad_12[0xa];
    int reqWord28; /* +0x1c */
    unsigned char pad_20[0x8];
    int reqWord40; /* +0x28 */
    unsigned char pad_2c[0x8];
} BattleLimitDragonDiveModelDrawRequest;
#pragma pack(pop)

/* A Dolphin Miracle model draw request (src/ff7/src/battle/battle_fx_limit_dragon.cpp): the head of
 * a BattleFxModelDraw, with the state flags still raw. */
#pragma pack(push, 1)
typedef struct {
    int modelGroup; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short fadeLevel; /* +0xa */
    unsigned char pad_c[0x4];
} BattleLimitDMModelReq;
#pragma pack(pop)

/* The Dolphin Miracle variant sprite's draw request: texture, CLUT, colour and primitive code, the
 * animation frame. */
#pragma pack(push, 1)
typedef struct {
    int texture; /* +0x0 */
    int clut; /* +0x4 */
    unsigned char colour[4]; /* +0x8 */
    short frameIndex; /* +0xc */
} BattleLimitDMSpriteReq1;
#pragma pack(pop)

/* The Dolphin Miracle radial sprite's draw request: its grey level follows the sprite's radius. */
#pragma pack(push, 1)
typedef struct {
    int texture; /* +0x0 */
    int clut; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char primCode;
    short frameIndex; /* +0xc */
} BattleLimitDMSpriteReq2;
#pragma pack(pop)

/* g_BattleLimitDynamiteParamTable (src/ff7/src/battle/battle_fx_limit_dynamite.cpp): the explosion
 * model's draw request, then the file names the Dynamite resource definition points at. */
#pragma pack(push, 1)
typedef struct BattleLimitDynamiteParamTable {
    BattleFxModelDrawHead explosionDraw;
    char kayakuRsd[12];
    char dynamiteRsd[16];
    char pieceTim[12];
    char pieceSpt[12];
} BattleLimitDynamiteParamTable;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitLandscaperDrawRequest1 {
    unsigned char pad_0[0x18];
    int gaiseiDrawReq1Resource; /* +0x18 */
    int gaiseiDrawReq1Handle; /* +0x1c */
} BattleLimitLandscaperDrawRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitLandscaperMeshSourceBlob1 {
    unsigned char pad_0[0x4];
    int ptr; /* +0x4 */
    unsigned char pad_8[0x934];
} BattleLimitLandscaperMeshSourceBlob1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitLandscaperMeshSourceBlob2 {
    unsigned char pad_0[0x4];
    int ptr; /* +0x4 */
    unsigned char pad_8[0x224];
} BattleLimitLandscaperMeshSourceBlob2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitGreasedLightningSpriteRequest {
    int spriteHandle; /* +0x0 */
    int res; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleLimitGreasedLightningSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitHowlingMoonSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x1];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleLimitHowlingMoonSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitDoomOfTheLivingModelRequest {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short reqPhaseAngle; /* +0xa */
    unsigned char pad_c[0x2];
} BattleLimitDoomOfTheLivingModelRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitDoomOfTheLivingSpriteRequest1 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int req1Res; /* +0x4 */
    unsigned char pad_8[0x4];
    short req1Frame; /* +0xc */
} BattleLimitDoomOfTheLivingSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitDoomOfTheLivingSpriteRequest2 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int req2Res; /* +0x4 */
    char req2AlphaOld2; /* +0x8 */
    char req2AlphaOld1; /* +0x9 */
    char req2Alpha; /* +0xa */
    unsigned char pad_b[0x1];
    short req2Short12; /* +0xc */
} BattleLimitDoomOfTheLivingSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitDoomOfTheLivingTimingTable1 {
    short startStep; /* +0x0 */
    short endStep; /* +0x2 */
    unsigned char pad_4[0x1c];
} BattleLimitDoomOfTheLivingTimingTable1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitFuryBrandModelReqA {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short fadeLevel; /* +0xa: BattleFxModelDraw fade level, set from the fade ramp */
    unsigned char pad_c[0x4];
} BattleLimitFuryBrandModelReqA;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitStardustRayModelReq2 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frameIndex; /* +0xc */
    unsigned char pad_e[0x2];
} BattleLimitStardustRayModelReq2;
#pragma pack(pop)

/* A 0x10-byte sprite request (g_YasuiLimitBladeBeamFadeSpriteReq); members named by the defines that read them. */
#pragma pack(push, 1)
typedef struct BattleLimitStardustRayModelReq3 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x5];
} BattleLimitStardustRayModelReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitAllCreationRotAngles2 {
    unsigned char pad_0[0x2];
    short angle2; /* +0x2 */
    unsigned char pad_4[0x2];
} BattleLimitAllCreationRotAngles2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitSledFangSpriteRequest1 {
    int spriteEntry; /* +0x0 */
    int resource; /* +0x4 */
    unsigned char pad_8[0x4];
    short step; /* +0xc */
} BattleLimitSledFangSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitSledFangSpriteRequest2 {
    int spriteEntry; /* +0x0 */
    int resource; /* +0x4 */
    unsigned char pad_8[0x4];
    short frameIndex; /* +0xc */
} BattleLimitSledFangSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitTifaRibbonPointBuffer0 {
    unsigned char pad_0[0x2];
    short targetBoneMaxY; /* +0x2 */
} BattleLimitTifaRibbonPointBuffer0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSnowstormParticleData {
    unsigned char pad_0[0x2];
    int placementZ; /* +0x2 */
    unsigned char pad_6[0x2];
} BattleSpecialSnowstormParticleData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBerserk_SpriteReq {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x4];
    short uvOffset; /* +0x8 */
    short alpha; /* +0xa */
} BattleMagicBerserk_SpriteReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBio3Multi_ModelData1 {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short scale; /* +0xa */
} BattleMagicBio3Multi_ModelData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBio3Multi_SpriteDrawReq1 {
    short scaleZ; /* +0x0 */
    int value1; /* +0x2 */
    unsigned char pad_6[0x4];
    int value2; /* +0xa */
    unsigned char pad_e[0x2];
    int table; /* +0x10 */
    int tableAlt; /* +0x14 */
} BattleMagicBio3Multi_SpriteDrawReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicRemove_QuadColors {
    short scaleX; /* +0x0 */
    unsigned char pad_2[0x8];
    short scaleY; /* +0xa */
    unsigned char pad_c[0x2];
    short scaleZ; /* +0xe */
    unsigned char pad_10[0x2];
    int transX; /* +0x12 */
    int transY; /* +0x16 */
    int transZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleMagicRemove_QuadColors;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicRemove_SpriteData {
    char colorR; /* +0x0 */
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    unsigned char pad_3[0x5];
    int drawObj; /* +0x8 */
    unsigned char pad_c[0x4];
} BattleMagicRemove_SpriteData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicRemove_SpriteReq {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x8];
    int nextSpriteGroup; /* +0x10 */
    int paletteObj; /* +0x14 */
} BattleMagicRemove_SpriteReq;
#pragma pack(pop)

/* g_BattleMagicFlareSpriteReq1 (src/ff7/src/battle/battle_fx_magic_flare.cpp): 0xe bytes; a BattleFxSpriteRequest with its frame at +0xc. */
#pragma pack(push, 1)
typedef struct BattleMagicFlareSpriteReq1 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x1];
    short frame; /* +0xc */
} BattleMagicFlareSpriteReq1;
#pragma pack(pop)

/* g_BattleMagicFlareSpriteReq2 (src/ff7/src/battle/battle_fx_magic_flare.cpp): 0x18 bytes; a BattleFxSpriteRequest followed by the group and draw object of the next request. */
#pragma pack(push, 1)
typedef struct BattleMagicFlareSpriteReq2 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x1];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int frameSpriteGroup; /* +0x10: the frame-table sprite's request starts here and runs into g_BattleMagicFlareSpriteReq3 */
    int frameDrawObj; /* +0x14 */
} BattleMagicFlareSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicDemi2_SpriteData2 {
    char colorR; /* +0x0 */
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    unsigned char pad_3[0x5];
    int drawObj; /* +0x8 */
    unsigned char pad_c[0x4];
    int fadingRecord; /* +0x10 */
    int fadingObj; /* +0x14 */
} BattleMagicDemi2_SpriteData2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicDemi2_SpriteData3 {
    char colorR; /* +0x0 */
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    unsigned char pad_3[0x1];
    short framePhase; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleMagicDemi2_SpriteData3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicMBarrier_ModelReq {
    int tableField; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
} BattleMagicMBarrier_ModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialTyokyuBasisMatrix {
    unsigned char pad_0[0x12];
    int posX18; /* +0x12 */
    int posY22; /* +0x16 */
    int posZ26; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleSpecialTyokyuBasisMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicSlow_ModelReq2 {
    int modelHandle; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short alpha; /* +0xa */
    unsigned char pad_c[0x34];
} BattleMagicSlow_ModelReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicBolt_SpriteReq {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleMagicBolt_SpriteReq;
#pragma pack(pop)

/* g_BattleSummonLeviathan_SpriteColorConfig (src/ff7/src/battle/battle_fx_summon_leviathan.cpp): 0x10 bytes. */
#pragma pack(push, 1)
typedef struct BattleSummonLeviathan_SpriteColorConfig {
    unsigned char pad_0[0x8];
    int modelGroup; /* +0x8 */
    unsigned char pad_c[0x4];
} BattleSummonLeviathan_SpriteColorConfig;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBubblesSpriteReq3 {
    int modelScale; /* +0x0 */
    unsigned char pad_4[0x4];
    int modelHandle; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
} BattleSpecHellBubblesSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeatBlastColor {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
} BattleSpecHeatBlastColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeartBreathSpriteReq1 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x5];
} BattleSpecHeartBreathSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecPoisonGasSpriteReq1 {
    int spriteRes; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int res16; /* +0x10 */
    int res20; /* +0x14 */
} BattleSpecPoisonGasSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecPoisonGasSpriteReq2 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    char code; /* +0x3 */
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSpecPoisonGasSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSephirothShockSpriteReq1 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int handle; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x3];
} BattleSpecSephirothShockSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSephirothShockSpriteReq10 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int handle; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    char code; /* +0xb */
    short frame; /* +0xc */
} BattleSpecSephirothShockSpriteReq10;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSephirothShockSpriteReq11 {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecSephirothShockSpriteReq11;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSephirothShockSpriteReq2 {
    int frameResource; /* +0x0 */
    int size; /* +0x4 */
    unsigned char pad_8[0x2];
    short alphaWord; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecSephirothShockSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSephirothShockSpriteReq3 {
    int spriteEntry; /* +0x0 */
    int sceneObject; /* +0x4 */
    unsigned char pad_8[0x4];
    short taskWord; /* +0xc */
} BattleSpecSephirothShockSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSephirothShockSpriteReq5 {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short alphaWord; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecSephirothShockSpriteReq5;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSephirothShockSpriteReq6 {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short scaleWord; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecSephirothShockSpriteReq6;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSephirothShockSpriteReq9 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int handle; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x1];
    short taskWord; /* +0xc */
} BattleSpecSephirothShockSpriteReq9;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecDualMortarSpriteReq1 {
    int spriteEntry; /* +0x0 */
    int res4; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int res16; /* +0x10 */
    int res20; /* +0x14 */
} BattleSpecDualMortarSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecEnergyBallSpriteRequest1 {
    int spriteEntry; /* +0x0 */
    int request1ResField; /* +0x4 */
    unsigned char pad_8[0x4];
    short request1Short; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecEnergyBallSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecEnergyBallSpriteRequest2 {
    int spriteResource; /* +0x0 */
    int request2ResField; /* +0x4 */
    char request2Byte1; /* +0x8 */
    char request2Byte2; /* +0x9 */
    char request2Byte3; /* +0xa */
    unsigned char pad_b[0x1];
    short request2FrameParity; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecEnergyBallSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecEnergyBallSpriteRequest3 {
    int spriteResource; /* +0x0 */
    int request3ResField; /* +0x4 */
    char request3Byte1; /* +0x8 */
    char request3Byte2; /* +0x9 */
    char request3Byte3; /* +0xa */
    unsigned char pad_b[0x1];
    short request3FrameParity; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecEnergyBallSpriteRequest3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecEnergyBallTransformData1 {
    int transform1Value; /* +0x0 */
    unsigned char pad_4[0x6];
    short transform1ScaledShort; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecEnergyBallTransformData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecEnergyBallTransformData2 {
    int transform2Value; /* +0x0 */
    unsigned char pad_4[0x6];
    short transform2IndexWord; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecEnergyBallTransformData2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecEnergyBallTransformData3 {
    int resEntry; /* +0x0 */
    unsigned char pad_4[0x6];
    short transform3CountWord; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecEnergyBallTransformData3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecEnergyBallTransformData4 {
    int transform4Value; /* +0x0 */
    unsigned char pad_4[0x6];
    short transform4Scale; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecEnergyBallTransformData4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecMultiJammerScale12 {
    unsigned char pad_0[0x4];
    int value4; /* +0x4 */
    unsigned char pad_8[0x4];
} BattleSpecMultiJammerScale12;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeatWaveModelReq2 {
    int modelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecHeatWaveModelReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecMultiJammerSpriteReq4 {
    int spriteResource; /* +0x0 */
    int res4; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x5];
} BattleSpecMultiJammerSpriteReq4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecShowerModelParams {
    unsigned char pad_0[0x8];
    int res8; /* +0x8 */
    int res12; /* +0xc */
} BattleSpecShowerModelParams;
#pragma pack(pop)

/* g_BattleSpecBombBlastSpriteReq1 (src/ff7/src/battle/battle_fx_special_bomb_blast.cpp): the sprite request of the animated sprites, 0x18 bytes. */
#pragma pack(push, 1)
typedef struct BattleSpecBombBlastSpriteReq1 {
    int spriteEntry; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int secondSpriteResource; /* +0x10 */
    int secondDrawObj; /* +0x14 */
} BattleSpecBombBlastSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecFireballSpriteReq1 {
    int spriteEntry; /* +0x0 */
    int spriteObject; /* +0x4 */
    unsigned char pad_8[0x4];
    short frameIndex; /* +0xc */
    unsigned char pad_e[0x2];
    int drawBlock; /* +0x10 */
    int handle; /* +0x14 */
} BattleSpecFireballSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecFireballSpriteReq2 {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    char code; /* +0x3 */
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSpecFireballSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecPollenSpriteReq1 {
    unsigned char pad_0[0x4];
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
    int spriteRecord; /* +0x8 */
    int drawObj; /* +0xc */
} BattleSpecPollenSpriteReq1;
#pragma pack(pop)

/* g_BattleSpecChefsKnifeAnimData (src/ff7/src/battle/battle_special_chefs_knife_effects.cpp): 0x30 bytes: the
 * rest of the Korosi spawn-direction table and the head of the pulse sprite's draw request. */
#pragma pack(push, 1)
typedef struct BattleSpecChefsKnifeAnimData {
    int spawnDir0Z;        /* +0x00: the z and pad of the first spawn direction, whose x and y */
    int spawnDir0Pad;      /*        are g_BattleSpecChefsKnifeSpawnDir0X and g_BattleSpecChefsKnifeSpawnDir0Y */
    VECTOR spawnDir1;   /* +0x08: the second and third spawn directions */
    VECTOR spawnDir2;
    int pulseSpriteGroup;  /* +0x28: the pulse sprite's request starts here and ends in */
    int pulseDrawObj;      /*        g_BattleSpecChefsKnifeSpriteRequest */
} BattleSpecChefsKnifeAnimData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecLightShellModelReq {
    int modelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
    int matrix; /* +0x10 */
    unsigned char pad_14[0x2];
} BattleSpecLightShellModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecLightShellSpriteReq1 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int req1ResourceHandle; /* +0x4 */
    unsigned char pad_8[0x4];
    short req1Word12; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecLightShellSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecLightShellSpriteReq2 {
    int spriteResource; /* +0x0 */
    int req2ResourceHandle; /* +0x4 */
    unsigned char pad_8[0x8];
} BattleSpecLightShellSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecMizufukiSpriteRequest2 {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecMizufukiSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinGAnimDataAC8 {
    int spriteResource; /* +0x0 */
    int drawObject; /* +0x4 */
    unsigned char pad_8[0x8];
} BattleSummonOdinGAnimDataAC8;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeatWaveModelReq1 {
    int modelResource; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short flashWord10; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecHeatWaveModelReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeatWaveSpriteReq1 {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x3];
} BattleSpecHeatWaveSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeatWaveSpriteReq2 {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecHeatWaveSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHeatWaveSpriteReq3 {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char fadeByte10; /* +0xa */
    unsigned char pad_b[0x1];
    short frame; /* +0xc */
} BattleSpecHeatWaveSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialHarrierLookMatrix {
    unsigned char pad_0[0x12];
    int transX; /* +0x12 */
    int transY; /* +0x16 */
    int transZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleSpecialHarrierLookMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecFreezeBreathScaleVector3 {
    unsigned char pad_0[0x10];
    int modelHandle; /* +0x10 */
} BattleSpecFreezeBreathScaleVector3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBomberModelReq2 {
    int modelResource; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecHellBomberModelReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBomberModelReq3 {
    int modelField; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecHellBomberModelReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBomberSpriteReq1 {
    int spriteResource; /* +0x0 */
    int variantResourceHandle; /* +0x4 */
    unsigned char pad_8[0x4];
    short req1FrameIndex; /* +0xc */
} BattleSpecHellBomberSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBomberSpriteReq2 {
    int spriteResource; /* +0x0 */
    int fadeResourceHandle; /* +0x4 */
    char req2ChainByte0; /* +0x8 */
    char req2ChainByte1; /* +0x9 */
    char req2ChainByte2; /* +0xa */
    unsigned char pad_b[0x1];
    short req2Word12; /* +0xc */
} BattleSpecHellBomberSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBomberSpriteReq3 {
    int spriteField; /* +0x0 */
    int req3ResourceHandle; /* +0x4 */
    unsigned char pad_8[0x4];
    short req3Word12; /* +0xc */
} BattleSpecHellBomberSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecHellBomberSpriteReq4 {
    int spriteResource; /* +0x0 */
    int req4ResourceHandle; /* +0x4 */
    char req4ChainByte0; /* +0x8 */
    char req4ChainByte1; /* +0x9 */
    char req4ChainByte2; /* +0xa */
    unsigned char pad_b[0x1];
    short req4Word12; /* +0xc */
} BattleSpecHellBomberSpriteReq4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialFallingRocksModelReq1 {
    int modelResource; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short angle; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialFallingRocksModelReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSwiftBoltSpriteReq1 {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
} BattleSpecSwiftBoltSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSwiftBoltSpriteReq2 {
    int spriteField; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char frameByte10; /* +0xa */
    unsigned char pad_b[0x1];
    short frame; /* +0xc */
} BattleSpecSwiftBoltSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSwiftBoltSpriteReq3 {
    int spriteField; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char frameByte10; /* +0xa */
    char code; /* +0xb */
    short frame; /* +0xc */
} BattleSpecSwiftBoltSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSwiftBoltSpriteReq4 {
    int spriteField; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x3];
} BattleSpecSwiftBoltSpriteReq4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecSwiftBoltSpriteReq5 {
    int resource; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x1];
    short frame; /* +0xc */
} BattleSpecSwiftBoltSpriteReq5;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialLiquidPoisonColor1 {
    int resource; /* +0x0 */
    int object; /* +0x4 */
    char red; /* +0x8 */
    char green; /* +0x9 */
    char blue; /* +0xa */
    unsigned char pad_b[0x1];
    short frameSelector; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialLiquidPoisonColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialLiquidPoisonColor2 {
    int resource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short driftSlot; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialLiquidPoisonColor2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialPoisonBreathColor1 {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char variantByte8; /* +0x8 */
    unsigned char variantByte9; /* +0x9 */
    unsigned char variantByte10; /* +0xa */
    unsigned char pad_b[0x1];
    short variantShort12; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialPoisonBreathColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialEnergyWavePosition {
    unsigned char pad_0[0x12];
    int x; /* +0x12 */
    int y; /* +0x16 */
    int z; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleSpecialEnergyWavePosition;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialEnergyWaveSpriteConfig2 {
    unsigned char pad_0[0x10];
    int config2ModelField; /* +0x10 */
    unsigned char pad_14[0x6];
    short config2RollAngle; /* +0x1a */
    unsigned char pad_1c[0x4];
    int config2SpriteField; /* +0x20 */
    int config2SpriteAlt; /* +0x24 */
} BattleSpecialEnergyWaveSpriteConfig2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialEnergyWaveSpriteRequest2 {
    char red; /* +0x0 */
    char request2Byte1; /* +0x1 */
    char request2Byte2; /* +0x2 */
    unsigned char pad_3[0x1];
    short request2FrameParity; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSpecialEnergyWaveSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialWaveCannonModelReqMoto {
    int resource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialWaveCannonModelReqMoto;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialWaveCannonModelReqTama {
    int model; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialWaveCannonModelReqTama;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialWaveCannonVector1 {
    int modelResField; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialWaveCannonVector1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialWaveCannonRotationTrajectory {
    int spriteResField; /* +0x0 */
    int drawObj; /* +0x4 */
    char trajectoryByte8; /* +0x8 */
    char trajectoryByte9; /* +0x9 */
    char trajectoryByte10; /* +0xa */
    unsigned char pad_b[0x1];
    short trajectoryShort12; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialWaveCannonRotationTrajectory;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialWaveCannonScaleTama {
    int spriteResField; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    char code; /* +0xb */
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialWaveCannonScaleTama;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSpreadLaserSpriteConfig2 {
    int spriteResField; /* +0x0 */
    int config2ResField; /* +0x4 */
    char config2Byte1; /* +0x8 */
    char config2Byte2; /* +0x9 */
    char config2Byte3; /* +0xa */
    unsigned char pad_b[0x1];
    short config2IndexWord; /* +0xc */
} BattleSpecialSpreadLaserSpriteConfig2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSpreadLaserSpriteConfig3 {
    int spriteResource; /* +0x0 */
    int config3ResField; /* +0x4 */
    char config3Byte1; /* +0x8 */
    char config3Byte2; /* +0x9 */
    char config3Byte3; /* +0xa */
    unsigned char pad_b[0x1];
    short config3IndexShort; /* +0xc */
} BattleSpecialSpreadLaserSpriteConfig3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSpreadLaserSpriteConfig4 {
    int spriteResField; /* +0x0 */
    int config4ResField; /* +0x4 */
    char config4Byte1; /* +0x8 */
    char config4Byte2; /* +0x9 */
    char config4Byte3; /* +0xa */
    unsigned char pad_b[0x1];
    short config4FrameParity; /* +0xc */
} BattleSpecialSpreadLaserSpriteConfig4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialSpreadLaserSpriteConfig5 {
    int spriteResource; /* +0x0 */
    int config5ResField; /* +0x4 */
    unsigned char pad_8[0x4];
    short config5Short; /* +0xc */
} BattleSpecialSpreadLaserSpriteConfig5;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialQuasarTable {
    unsigned char pad_0[0x20];
    int spriteRecord; /* +0x20 */
    int spriteObject; /* +0x24 */
} BattleSpecialQuasarTable;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialBeamGunBasisMatrix {
    unsigned char pad_0[0x12];
    int vectorX; /* +0x12 */
    int vectorY; /* +0x16 */
    int vectorZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleSpecialBeamGunBasisMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialBadMouthColor1 {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
} BattleSpecialBadMouthColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialFallingRocksColor2 {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
} BattleSpecialFallingRocksColor2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialBadMouthModelReq1 {
    int modelResField; /* +0x0 */
    unsigned char pad_4[0x6];
    short scale; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialBadMouthModelReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialOilColor {
    int spriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    char variantByte8; /* +0x8 */
    char variantByte9; /* +0x9 */
    char variantByte10; /* +0xa */
    unsigned char pad_b[0x1];
    short variantShort12; /* +0xc */
} BattleSpecialOilColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialOilModelReq {
    int modelResField; /* +0x0 */
    unsigned char pad_4[0x6];
    short stride; /* +0xa */
} BattleSpecialOilModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialOilSpriteIndices {
    int spriteIndices[2]; /* the two variant sprite tables, selected by BattleFxOilSpriteSlot::variant */
    int modelDrawTableField; /* +0x8 */
} BattleSpecialOilSpriteIndices;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialEnergyOrbColor1 {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialEnergyOrbColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialEnergyOrbModelReqIsi {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
} BattleSpecialEnergyOrbModelReqIsi;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialEnergyOrbRotationTrajectory {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    char trajectoryByte8; /* +0x8 */
    char trajectoryByte9; /* +0x9 */
    char trajectoryByte10; /* +0xa */
    unsigned char pad_b[0x1];
    short trajectoryShort12; /* +0xc */
} BattleSpecialEnergyOrbRotationTrajectory;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialEnergyOrbScaleTama {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    char code; /* +0xb */
    short frame; /* +0xc */
} BattleSpecialEnergyOrbScaleTama;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialShieldModelReq {
    int modelDraw; /* +0x0 */
    int flags4; /* +0x4 */
    unsigned char pad_8[0x2];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialShieldModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialScorchBasisMatrix {
    unsigned char pad_0[0x18];
    int spriteRecord; /* +0x18 */
    int spriteObject; /* +0x1c */
} BattleSpecialScorchBasisMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialScorchColor1 {
    char colorR; /* +0x0 */
    char colorG; /* +0x1 */
    char colorB; /* +0x2 */
    char code; /* +0x3 */
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleSpecialScorchColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialScorchModelReqIsi {
    int modelDraw; /* +0x0 */
    int flags; /* +0x4 */
    short uvScroll; /* +0x8 */
    short fadeLevel; /* +0xa */
} BattleSpecialScorchModelReqIsi;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialTriangleAttackSpriteReq2 {
    int animTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialTriangleAttackSpriteReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialScorchVector1 {
    int modelDraw; /* +0x0 */
    int flags; /* +0x4 */
    short uvScroll; /* +0x8 */
    short fadeLevel; /* +0xa */
    short stateFlags; /* +0xc */
    short texturePageBits; /* +0xe */
} BattleSpecialScorchVector1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialScorchVectorTrajectory {
    int modelDraw; /* +0x0 */
    int flags; /* +0x4 */
    short uvScroll; /* +0x8 */
    short fadeLevel; /* +0xa */
    short stateFlags; /* +0xc */
    short texturePageBits; /* +0xe */
    struct BattleFxSpriteGroup *spriteGroup; /* +0x10: the sprite request that follows, completed by g_BattleSpecialScorchColor2 */
    int drawObj; /* +0x14 */
} BattleSpecialScorchVectorTrajectory;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialTriangleAttackModelReq1 {
    int model; /* +0x0 */
    unsigned char pad_4[0x6];
    short scale; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialTriangleAttackModelReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialTriangleAttackModelReq2 {
    int model; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecialTriangleAttackModelReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialTriangleAttackSpriteReq1 {
    int animTable; /* +0x0 */
    int drawObj; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x1];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecialTriangleAttackSpriteReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialTriangleAttackSpriteReq3 {
    int sprite; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short pulseShort12; /* +0xc */
} BattleSpecialTriangleAttackSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialFascinationColor1 {
    int animTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short yuwakuColor1StepWord; /* +0xc */
} BattleSpecialFascinationColor1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialFascinationColor2 {
    int animTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short yuwakuSineFrameIndex; /* +0xc */
} BattleSpecialFascinationColor2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialFascinationColor3 {
    int animTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short yuwakuRisingFrameWord; /* +0xc */
} BattleSpecialFascinationColor3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecialFascinationModelReq {
    int modelHandle; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x8];
} BattleSpecialFascinationModelReq;
#pragma pack(pop)

/* g_BattleStatusOverlayOffsets: the camera offsets of the status overlay model, 0x18 bytes. */
#pragma pack(push, 1)
typedef struct BattleStatusOverlayOffsets {
    BattleActorCameraOffsets views[2]; /* copied into the slot-3 actor's cameraOffsets */
} BattleStatusOverlayOffsets;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxActorBoneMatrixBuf {
    int rotation; /* +0x0 */
    unsigned char pad_4[0xe];
    int transX; /* +0x12 */
    int transY; /* +0x16 */
    int transZ; /* +0x1a */
} BattleFxActorBoneMatrixBuf;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonAlexanderDefaultPosition {
    short sparkOriginX; /* +0x0 */
    short sparkOriginY; /* +0x2 */
    short sparkOriginZ; /* +0x4 */
    unsigned char pad_6[0x6];
} BattleSummonAlexanderDefaultPosition;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxMatrixBuildDesc {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x8];
} BattleFxMatrixBuildDesc;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxQuadCornerColors {
    unsigned char pad_0[0x4];
    short spriteFrame; /* +0x4 */
    unsigned char pad_6[0x2];
} BattleFxQuadCornerColors;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxBzFlarePointSlot {
    unsigned char pad_0[0xa];
    int bahamutZeroSlot10And16Point; /* +0xa */
    unsigned char pad_e[0xa];
} BattleFxBzFlarePointSlot;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonChocoMogBillboardMatrix {
    short scaleX; /* +0x0 */
    unsigned char pad_2[0x6];
    short scaleY; /* +0x8 */
    unsigned char pad_a[0x6];
    short scaleZ; /* +0x10 */
    int vecX; /* +0x12 */
    int vecY; /* +0x16 */
    int vecZ; /* +0x1a */
} BattleSummonChocoMogBillboardMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonChocoMogEffectDescriptor {
    unsigned char pad_0[0x38];
    short moveX; /* +0x38 */
    short moveY; /* +0x3a */
} BattleSummonChocoMogEffectDescriptor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesRadialData {
    int growRadialDraw; /* +0x0 */
    int growRadialLookup4; /* +0x4 */
    unsigned char pad_8[0x4];
    short growRadialFlags; /* +0xc */
    short growRadialSize; /* +0xe */
    int shrinkRadialDraw; /* +0x10 */
    int shrinkRadialLookup; /* +0x14 */
} BattleSummonHadesRadialData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesSpriteReq3 {
    int texture; /* +0x0 */
    int clut; /* +0x4 */
    char colorR; /* +0x8 */
    char colorG; /* +0x9 */
    char colorB; /* +0xa */
    unsigned char pad_b[0x1];
    unsigned short frameIndex; /* +0xc */
} BattleSummonHadesSpriteReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonHadesMeshBlob {
    int pad_00; /* +0x0 */
    unsigned char pad_4[0x92];
    int meshHeader; /* +0x96 */
    unsigned char pad_9a[0xc446];
} BattleSummonHadesMeshBlob;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataModelDrawRequest1 {
    int blackModel2Resource; /* +0x0 */
    unsigned char pad_4[0x6];
    short blackModel2Stretch; /* +0xa */
    unsigned char pad_c[0x4];
    int hoppingSpriteRequest; /* +0x10 */
    int hoppingSpriteAux; /* +0x14 */
} BattleSummonKjataModelDrawRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataSpriteRequest1 {
    int trailSpriteGroup; /* +0x0 */
    int trailSpriteObject; /* +0x4 */
    unsigned char pad_8[0x4];
    short trailSpriteFlags; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSummonKjataSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKjataVertexData1 {
    int triangleOffset; /* +0x0 */
    int basePointer; /* +0x4 */
    unsigned char pad_8[0x161c];
} BattleSummonKjataVertexData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnightLookAtPoints {
    unsigned char pad_0[0x8];
    int summonKotrEndpointPointList; /* +0x8 */
    unsigned char pad_c[0xc2];
} BattleSummonKotrKnightLookAtPoints;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrStageRequestArgs {
    short viewportOriginX; /* +0x0 */
    short summonKotrOffscreenStageWord; /* +0x2 */
    unsigned char pad_4[0x4];
} BattleSummonKotrStageRequestArgs;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleKotrFragmentData {
    int meshData; /* +0x0 */
    unsigned char pad_4[0xc];
    int kotrFragmentPrimitiveTable; /* +0x10 */
    int kotrFragmentPrimCommandWord; /* +0x14 */
    short kotrFragmentWord12; /* +0x18 */
    short kotrFragmentWord13; /* +0x1a */
    short kotrFragmentWord14; /* +0x1c */
    short kotrFragmentWord15; /* +0x1e */
    unsigned char pad_20[0x8];
    short kotrFragmentWord20; /* +0x28 */
    short kotrFragmentWord21; /* +0x2a */
    short kotrFragmentWord22; /* +0x2c */
    short kotrFragmentWord23; /* +0x2e */
    int kotrDataPointer01; /* +0x30 */
    unsigned char pad_34[0x8];
    int kotrDataPointer02; /* +0x3c */
    unsigned char pad_40[0xc];
    int kotrDataPointer03; /* +0x4c */
    unsigned char pad_50[0xc];
    int kotrDataPointer04; /* +0x5c */
    unsigned char pad_60[0x8];
    int kotrDataPointer05; /* +0x68 */
    unsigned char pad_6c[0x8];
    int kotrDataPointer06; /* +0x74 */
    unsigned char pad_78[0x8];
    int kotrDataPointer07; /* +0x80 */
    unsigned char pad_84[0x8];
    int kotrDataPointer08; /* +0x8c */
    unsigned char pad_90[0x8];
    int kotrDataPointer09; /* +0x98 */
    unsigned char pad_9c[0x8];
    int kotrDataPointer10; /* +0xa4 */
    unsigned char pad_a8[0x8];
    int kotrDataPointer11; /* +0xb0 */
    unsigned char pad_b4[0x8];
    int kotrDataPointer12; /* +0xbc */
    unsigned char pad_c0[0x8];
    int kotrDataPointer13; /* +0xc8 */
    unsigned char pad_cc[0xc];
    int kotrDataPointer14; /* +0xd8 */
    unsigned char pad_dc[0xc];
    int kotrDataPointer15; /* +0xe8 */
    unsigned char pad_ec[0xc];
    int kotrDataPointer16; /* +0xf8 */
    unsigned char pad_fc[0x8];
    int kotrDataPointer17; /* +0x104 */
    unsigned char pad_108[0x8];
    int kotrDataPointer18; /* +0x110 */
    unsigned char pad_114[0x8];
    int kotrDataPointer19; /* +0x11c */
    unsigned char pad_120[0x8];
    int kotrDataPointer20; /* +0x128 */
    unsigned char pad_12c[0x8];
    int kotrDataPointer21; /* +0x134 */
    unsigned char pad_138[0x8];
} BattleKotrFragmentData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxNeoBahamutModelReq {
    int modelGroup; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short fadeLevel; /* +0xa: BattleFxModelDraw fade level, set from the fade ramp */
    unsigned char pad_c[0x4];
} BattleFxNeoBahamutModelReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxNeoBahamutResIds {
    unsigned char pad_0[0xc];
    int fadeResourceIds; /* +0xc */
    unsigned char pad_10[0x8];
} BattleFxNeoBahamutResIds;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinGAnimDataAF8 {
    int spriteResource; /* +0x0 */
    int drawObject; /* +0x4 */
    union {
        int packedColor; /* +0x8: packed colour command */
        struct {
            char colorR;
            char colorG;
            char colorB;
            char colorCode;
        };
    };
    unsigned char pad_c[0x4];
} BattleSummonOdinGAnimDataAF8;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinGAnimDataB88 {
    int modelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short fadeLevel; /* +0xa: BattleFxModelDraw fade level, set from the fade ramp */
    unsigned char pad_c[0x4];
} BattleSummonOdinGAnimDataB88;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinGAnimDataCC0 {
    int spriteResource; /* +0x0 */
    int drawObject; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    short uvPacked; /* +0xe */
} BattleSummonOdinGAnimDataCC0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinGAnimDataD10 {
    unsigned char pad_0[0x16];
    int transY; /* +0x16 */
    unsigned char pad_1a[0x4];
} BattleSummonOdinGAnimDataD10;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinGMatrixB168 {
    int matrix; /* +0x0 */
    unsigned char pad_4[0xe];
    int matrixOriginX; /* +0x12 */
    int matrixOriginY; /* +0x16 */
    int matrixOriginZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleSummonOdinGMatrixB168;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinZTrackMatrix {
    int pad_00; /* +0x0 */
    unsigned char pad_4[0xe];
    int transX; /* +0x12 */
    int transY; /* +0x16 */
    int transZ; /* +0x1a */
} BattleSummonOdinZTrackMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonOdinZSpriteDrawRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x6];
} BattleSummonOdinZSpriteDrawRequest;
#pragma pack(pop)

/* g_BattleSummonOdinZTaskParam (src/ff7/src/battle/battle_fx_summon_odin_zantetsuken.cpp): offscreen capture rect and animated bone-sprite request head. */
#pragma pack(push, 1)
typedef struct BattleSummonOdinZTaskParam {
    BattleFxOdinZantetsukenActorPos captureRect; /* +0x0 */
    struct BattleFxSpriteGroup *spriteField; /* +0x8 */
    int objField; /* +0xc */
} BattleSummonOdinZTaskParam;
#pragma pack(pop)

/* Viewport origin and dimensions for the Phoenix stage offscreen capture. */
#pragma pack(push, 1)
typedef struct BattleSummonPhoenixAngleData1 {
    short spriteX;
    short spriteY;
    short spriteW;
    short spriteH;
} BattleSummonPhoenixAngleData1;
#pragma pack(pop)

/* Animated sprite draw request for Phoenix sprite entry 7. */
#pragma pack(push, 1)
typedef struct BattleSummonPhoenixAnimData1 {
    struct BattleFxSpriteGroup *frameTable;
    int tableValue;
    unsigned char pad_8[0x4];
    short sourceShort;
} BattleSummonPhoenixAnimData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixAnimData2 {
    int spriteValue; /* +0x0 */
    int risingEmitterValue; /* +0x4 */
    unsigned char pad_8[0x6];
} BattleSummonPhoenixAnimData2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixAnimData3 {
    int spriteValue; /* +0x0 */
    int billboardValue; /* +0x4 */
    unsigned char pad_8[0x6];
} BattleSummonPhoenixAnimData3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixCoordData1 {
    short ringColumnCoord; /* +0x0 */
    short ringColumnCoord1; /* +0x2 */
    unsigned char pad_4[0x2];
} BattleSummonPhoenixCoordData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixCoordData2 {
    short ringColumnCoord; /* +0x0 */
    short ringColumnCoord2; /* +0x2 */
    unsigned char pad_4[0x2];
} BattleSummonPhoenixCoordData2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixCoordData3 {
    short ringColumnCoord; /* +0x0 */
    short ringColumnCoord3; /* +0x2 */
    unsigned char pad_4[0x2];
} BattleSummonPhoenixCoordData3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixCoordData4 {
    short ringColumnCoord; /* +0x0 */
    short ringColumnCoord4; /* +0x2 */
    unsigned char pad_4[0x2];
} BattleSummonPhoenixCoordData4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixSpriteRequest1 {
    int spinSpeed; /* +0x0 */
    unsigned char pad_4[0x6];
    short spinAngle; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonPhoenixSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixSpriteRequest3 {
    int requestValue; /* +0x0 */
    unsigned char pad_4[0x6];
    short growingDipValue; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonPhoenixSpriteRequest3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixSpriteRequest4 {
    int trailValue; /* +0x0 */
    unsigned char pad_4[0x6];
    short trailSpritePrev; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonPhoenixSpriteRequest4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixSpriteRequest5 {
    int trailValue; /* +0x0 */
    unsigned char pad_4[0x6];
    short trailSpriteCurrent; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonPhoenixSpriteRequest5;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonPhoenixSpriteRequest7 {
    int growingValue; /* +0x0 */
    unsigned char pad_4[0x6];
    short growingStepAngle; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonPhoenixSpriteRequest7;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxOffsetTriMeshDesc {
    unsigned char pad_0[0x12];
    int transX; /* +0x12 */
    int transY; /* +0x16 */
    int transZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleFxOffsetTriMeshDesc;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonRamuhMeshSource1 {
    int polygonOffset; /* +0x0 */
    int vertexBase; /* +0x4 */
    unsigned char pad_8[0xd4];
    int cameraScriptData; /* +0xdc */
    unsigned char pad_e0[0x1444];
} BattleSummonRamuhMeshSource1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SaferSephirothDeath_BlackModelDrawReq {
    int model; /* +0x0 */
    unsigned char pad_4[0x6];
    short fadeLevel; /* +0xa: BattleFxModelDraw fade level, set from the fade ramp */
    unsigned char pad_c[0x4];
} SaferSephirothDeath_BlackModelDrawReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct RubyTentaclesBaseModelVertices {
    int byteCount; /* +0x0: vertex data size in bytes, 8 per vertex */
    int vertexPtr; /* +0x4: first vertex */
    unsigned char pad_8[0x2684];
} RubyTentaclesBaseModelVertices;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct RubyTentaclesCombinedMatrix {
    PsxMatrix matrix;
    short lookupWord; /* +0x1e */
} RubyTentaclesCombinedMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BoulderThrow_DisplacementVelocities {
    short velocityX; /* +0x0 */
    short velocityY; /* +0x2 */
    short velocityZ; /* +0x4 */
    unsigned char pad_6[0x2];
} BoulderThrow_DisplacementVelocities;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BoulderThrow_SpriteResourcePointer {
    int resource; /* +0x0 */
    int objectPointer; /* +0x4 */
    unsigned char pad_8[0x4];
    short frameIndex; /* +0xc */
    unsigned char pad_e[0x2];
} BoulderThrow_SpriteResourcePointer;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamTransformMatrix {
    unsigned char pad_0[0x12];
    int translationX; /* +0x12 */
    int translationY; /* +0x16 */
    int translationDepth; /* +0x1a */
} YasuiLimitSatelliteBeamTransformMatrix;
#pragma pack(pop)

/* g_YasuiLimitSatanSlamSourceMeshAndPoseBlob (src/ff7/src/battle/yasui/yasui_limit_satan_slam_stage_shatter.cpp): 0x2b90 bytes. */
#pragma pack(push, 1)
typedef struct YasuiLimitSatanSlamSourceMeshAndPoseBlob {
    unsigned char pad_0[0x4];
    union {
        struct {
            int poseVertexBuffer; /* +0x4 */
            unsigned char pad_8[0x2b88];
        };
        struct {
            Int64Pair poseVertices[0x135];
            unsigned char polygons[0x21e4];
        };
    };
} YasuiLimitSatanSlamSourceMeshAndPoseBlob;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatanSlamSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int resourceHandle; /* +0x4 */
    char frameByte0; /* +0x8 */
    char frameByte1; /* +0x9 */
    char frameIndex; /* +0xa */
    unsigned char pad_b[0x1];
    short scale; /* +0xc */
} YasuiLimitSatanSlamSpriteRequest;
#pragma pack(pop)

/* g_YasuiLimitSatanSlamWorkingMeshBlob (src/ff7/src/battle/yasui/yasui_limit_satan_slam_stage_shatter.cpp): 0x2b90 bytes. */
#pragma pack(push, 1)
typedef struct YasuiLimitSatanSlamWorkingMeshBlob {
    union {
        unsigned char pad_0[0x4];
        int polygonOffset;
    };
    union {
        struct {
            int battleLimitSatanPoseVertexBuffer; /* +0x4 */
            unsigned char pad_8[0x2b88];
        };
        struct {
            SVECTOR vertices[0x135];
            unsigned char polygons[0x21e4];
        };
    };
} YasuiLimitSatanSlamWorkingMeshBlob;
#pragma pack(pop)

/* src/ff7/src/battle/battle_fx_magic_hitmark.cpp BattleFx_MagicHitmarkDrawAndStepParticle: what matrix points to, from its reads at 3 offsets. */
#pragma pack(push, 1)
typedef struct BattleFxShardTransformBatch {
    unsigned char pad_0[0x12];
    int posX; /* +0x12 */
    int posY; /* +0x16 */
    int posZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleFxShardTransformBatch;
#pragma pack(pop)

/* src/ff7/src/battle/battle_fx_summon_neo_bahamut.cpp BattleFx_SummonNeoBahamutUpdateFragmentMatrices: what matrixOut points to, from its reads at 8 offsets. */
#pragma pack(push, 1)
typedef struct BattleFxNeoBahamutShardMatrixBuf {
    short m00; /* +0x0 */
    short m01; /* +0x2 */
    short m02; /* +0x4 */
    short m10; /* +0x6 */
    short m11; /* +0x8 */
    short m12; /* +0xa */
    short m20; /* +0xc */
    short m21; /* +0xe */
    short m22; /* +0x10 */
    unsigned char pad_12[0x2];
} BattleFxNeoBahamutShardMatrixBuf;
#pragma pack(pop)

/* src/ff7/src/battle/battle_fx_summon_bahamut.cpp BattleFx_SummonBahamutExpandingModelTask: what s points to, from its reads at 3 offsets. */
#pragma pack(push, 1)
typedef struct BattleFxExpandingModelTask {
    unsigned char pad_0[0x12];
    int angleX; /* +0x12 */
    int angleY; /* +0x16 */
    int angleZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleFxExpandingModelTask;
#pragma pack(pop)

/* src/ff7/src/battle/battle_fx_limit_landscaper.cpp BattleFx_LimitLandscaperDrawPsxMeshPerTriangleTransform: what translation points to, from its reads at 3 offsets. */
#pragma pack(push, 1)
typedef struct BattleGteViewMatrixScratch {
    unsigned char pad_0[0x12];
    int translateX; /* +0x12 */
    int translateY; /* +0x16 */
    int translateZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleGteViewMatrixScratch;
#pragma pack(pop)

/* src/ff7/src/battle/battle_bone_world_camera.cpp Battle_WorldPointToCameraLocal: what worldPoint points to, from its reads at 3 offsets. */
#pragma pack(push, 1)
typedef struct BattleCameraWorldPointScratch {
    unsigned char pad_0[0x12];
    int worldX; /* +0x12 */
    int worldY; /* +0x16 */
    int worldZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleCameraWorldPointScratch;
#pragma pack(pop)

/* Shared 0x8D70-byte battle effect scratch block (g_BattleEffectScratch at 0x00bfd0f8):
 * starts with the active effect's targetMask (+0x0000), and holds the rain particle nodes
 * (+0x510C) and splash records (+0x890C). */
typedef struct BattleEffectScratchHead {
    short targetMask;
    unsigned char pad_02[0x510a];
    Battle3dNode rainDrops[256];
    unsigned char pad_850c[0x400];
    Battle3dRainRecord rainSplashes[256];
    unsigned char pad_8b0c[0x264];
} BattleEffectScratchHead;
typedef char CheckBattleEffectScratchHead[(sizeof(BattleEffectScratchHead) == 0x8d70) ? 1 : -1];

/* The menu graphics directory path, the battle-won music flag the end sequence raises and the
 * timestamp the session's timing starts from. */
typedef struct BattleMenuGraphicsPathBuffer {
    char path[0x100];
    short victoryMusicPending; /* Battle_RequestEndMusicWin plays the win music while it is set */
    unsigned char pad_102[0x6];
    Timestamp64 timestampSample;
} BattleMenuGraphicsPathBuffer;

/* The flamethrower timeline task's scratch block: the spawn centre, the point rotated by the attach matrix, and that matrix. */
typedef struct BattleBlueFlameThrowerScratch {
    BattleFxSpawnCenter spawn;
    unsigned char pad_1A[0x6];
    PsxMatrix matrix; /* +0x20 */
    unsigned char pad_3E[0x2];
    int rotateFlag; /* +0x40: RotTransPoint's flag output */
} BattleBlueFlameThrowerScratch;

#endif /* FF7_TYPES_BATTLE_H */

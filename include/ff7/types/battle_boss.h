/* include/ff7/types/battle_boss.h: Boss encounter, battle swirl transition and Arena sequence types. */
#ifndef FF7_TYPES_BATTLE_BOSS_H
#define FF7_TYPES_BATTLE_BOSS_H

#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include "lib/codegen.h"

/* --- deadsef --- */
/* Camera script track with 32-bit packed access to its 8-byte position SVECTOR. */
typedef struct {
    short mode;
    short frames;
    int distance;
    int delta;
    short actor;
    short bone;
    union {
        CameraScriptVector position;
        int positionWords[2];
    } u;
    CameraScriptVector base;
    CameraScriptVector velocity;
    short easing;
    short pad;
} DeadSefCameraTrack;

/* Resource table entry passed to Gfx_LoadModelTableIfAbsent for SUMMON/LASTBOSS. */
typedef struct {
    unsigned short flags;
    unsigned short scale;
    const void *name;
    const char *sptName;
    int optionValue;
    int extra;
} SaferSephirothDeathResourceRecord;

/* 14-byte PSX mesh draw request filled per enemy model part in BattleFx_SaferSephirothDeathShatterDrawTask. */
#pragma pack(push, 2)
typedef struct {
    int meshData;
    int flags;
    unsigned char pad_08[0x2];
    short colorWord;
    short attrMask;
} SaferSephirothDeathMultiPartDrawReq;
#pragma pack(pop)

/* 16-byte model draw request header filled in BattleFx_SaferSephirothDeathBillboardTask. */
typedef struct {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} SaferSephirothDeathBillboardDrawReq;

/* RotTransPoint output slot for one vertex (three words at +0x00, flag slot at index 4). */
typedef struct BattleFxSaferSephirothDeathTimelineStep {
    int x;
    int y;
    int z;
    unsigned char pad_0C[0x4];
} BattleFxSaferSephirothDeathTimelineStep;

/* One shatter fragment: its four corner offsets from the fragment centre kept in g_SaferSephirothDeath_FragmentVertexBuf. */
typedef struct SaferSephirothDeathFragmentQuad {
    SVECTOR corner[4];
} SaferSephirothDeathFragmentQuad;

/* Motion state of one shatter fragment: the frame it starts at and its falling speed. */
typedef struct SaferSephirothDeathFragmentMotion {
    short startFrame;
    short deltaZ;
} SaferSephirothDeathFragmentMotion;

/* Scratch block of the fading-model task, taken from Gfx_GetPsxScratchpadPtr; the rotation matrix sits at +0x10. */
typedef struct SaferSephirothDeathFadeBlock {
    unsigned char pad_00[0x10];
    PsxMatrix matrix;
} SaferSephirothDeathFadeBlock;

/* 8 bytes of state shared by the camera script operand reader and the fading-model task: an init flag and
 * either the scratch block pointer or the operand words (the reader indexes from operandWords[1]). */
typedef struct SaferSephirothDeathOperandPool {
    unsigned char initDone;
    unsigned char pad_01[0x3];
    union {
        SaferSephirothDeathFadeBlock *fadeBlock;
        short operandWords[2];
    } u;
} SaferSephirothDeathOperandPool;

/* Screen-flash sequence state: frame counter and index into the flash threshold table. */
typedef struct BattleFxSaferSephirothDeathFlashState {
    short stage;
    short frame;
} BattleFxSaferSephirothDeathFlashState;

/* Last-boss billboard summon task slot: frame counter, world position and initial scale eased toward zero. */
typedef struct BattleFxSaferSephirothDeathBillboardRecord {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short startValue;
} BattleFxSaferSephirothDeathBillboardRecord;

/* Fragment motion state of the Sef summon: per-frame deltas, 0x1000-scaled factors and the matrix built from them. */
typedef struct BattleFxSefFragmentState {
    SVECTOR delta;
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix;
} BattleFxSefFragmentState;

/* Fading silhouette model task slot: frame counter, rotation angles and scale/vertical offset. */
typedef struct BattleFxSaferSephirothDeathFadingTask {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    SVECTOR rot;
    short scaleX;
    short offsetY;
} BattleFxSaferSephirothDeathFadingTask;

/* Task slot for the last-boss camera script: script cursor, transform matrix and actor index. */
typedef struct BattleFxSaferSephirothDeathCameraSlot {
    short ownerSlot;
    short started;
    unsigned char pad_04[0x8];
    short *scriptAddr;
    PsxMatrix *matrixAddr;
    short actor;
} BattleFxSaferSephirothDeathCameraSlot;

/* PSX textured triangle/quad primitive written by BattleFx_SaferSephirothDeathDrawFragmentsPsxMesh. */
typedef struct BattleFxSaferSephirothDeathFragmentCursorQuad {
    int tagWord;
    union {
        int color;
        CVECTOR colorVec;
        struct {
            unsigned char rgb[0x3];
            char primCode;
        } code;
    } u;
    unsigned char pad_08[0x4];
    int colorCmd;
    unsigned char pad_10[0x4];
    short texUv1;
    short attrMask;
    unsigned char pad_18[0x4];
    short texUv2;
    unsigned char pad_1E[0x6];
    short texUv3;
} BattleFxSaferSephirothDeathFragmentCursorQuad;

/* PSX Gouraud triangle/quad packet for a last-boss fragment. */
typedef struct BattleFxSaferSephirothDeathFragmentPacket {
    int tag;
    union {
        int color0;
        CVECTOR colorVec0;
        struct {
            unsigned char rgb[0x3];
            char primCode;
        } code;
    } u;
    unsigned char pad_08[0x4];
    int colorCmd;
    unsigned char pad_10[0x4];
    int vertexColor2;
    unsigned char pad_18[0x4];
    int vertexColor3;
} BattleFxSaferSephirothDeathFragmentPacket;

/* Stream cursor view over ENEMY331.DAT polygon sections (counts, textured tris/quads, Gouraud tris/quads). */
typedef union DeadSefPrimStream {
    short count;
    struct {
        short vertOffsets[3];
        short color;
        int colorCmd;
        short texCoord1;
        short texCoord2;
    } texTri;
    struct {
        short vertOffsets[4];
        int colorCmd;
        short texCoord1;
        short texCoord2;
        short texCoord3;
        short color;
    } texQuad;
    struct {
        short vertOffsets[3];
        short pad;
        union {
            CVECTOR cvec[3];
            int words[3];
        } colors;
    } gouraudTri;
    struct {
        short vertOffsets[4];
        union {
            CVECTOR cvec[4];
            int words[4];
        } colors;
    } gouraudQuad;
} DeadSefPrimStream;

/* Header of an ENEMY331.DAT model part: byte offset from vertices[0] to the primitive stream. */
typedef struct SaferSephirothDeathModelPart {
    int primOffset;
    SVECTOR vertices[1];
} SaferSephirothDeathModelPart;

/* Stream cursor view for reading 32-bit offsets from ENEMY331.DAT part headers. */
typedef union DeadSefDatStream {
    short word;
    int dword;
} DeadSefDatStream;

/* --- lasboss3 --- */
/* Damped S-Nova particle: per-axis position and velocity, gravity, a timer and a resource index. */
typedef struct BattleFxSuperNovaDampedParticle {
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
    short duration;
    short resourceIndex;
    short scale;
    short gravity;
} BattleFxSuperNovaDampedParticle;

/* GPU textured-quad packet for the SNova point-field sprite, built from the render state block. */
typedef struct BattleFxSuperNovaPointField {
    int tag;
    int colorCommand;
    short x0;
    short y0;
    char u0;
    char v0;
    short clut;
    short x1;
    short y1;
    char u1;
    char v1;
    short texturePage;
    short x2;
    short y2;
    char u2;
    char v2;
    unsigned char pad_1E[0x2];
    short x3;
    short y3;
    char u3;
    char v3;
} BattleFxSuperNovaPointField;

/* Render-state block the projected quad task fills with vertex positions and colour bytes. */
typedef struct BattleFxSuperNovaQuadBlock {
    int tag;
    int colorCodeWord;
    short screenX0;
    short screenY0;
    char u0;
    char v0;
    short clut;
    short screenX1;
    short screenY1;
    char u1;
    char v1;
    short texturePage;
    short screenX2;
    short screenY2;
    char u2;
    char v2;
    unsigned char pad_1E[0x2];
    short screenX3;
    short screenY3;
    char u3;
    char v3;
} BattleFxSuperNovaQuadBlock;

/* Effect-100 slot for the spinning model pair: step counter, three spin angles and their steps. */
typedef struct BattleFxSuperNovaSpinSlot {
    short ownerSlot;
    short step;
    short spinX;
    short spinY;
    short spinZ;
    unsigned char pad_0A[0x2];
    short spinStepX;
    short spinStepY;
    short spinStepZ;
    unsigned char pad_12[0x8];
    short stepTotal;
} BattleFxSuperNovaSpinSlot;

/* SNova four-pass model slot with a packed pass index, packed position and a rotating angle. */
typedef struct BattleFxSuperNovaPassSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
} BattleFxSuperNovaPassSlot;

/* S-Nova expanding model emitter task record: end flag and growing radius. */
typedef struct BattleFxSuperNovaEmitter {
    short endFlag;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short radius;
} BattleFxSuperNovaEmitter;

/* S-Nova trailing sprite task record: frame, integrated position and damped velocity. */
typedef struct BattleFxSuperNovaTrailSprite {
    short endFlag;
    short frame;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
} BattleFxSuperNovaTrailSprite;

/* SNova decelerating sprite record: frame count, positions, decaying velocity and timer. */
typedef struct BattleFxSuperNovaDecelSpriteRecord {
    short ownerSlot;
    short frameCount;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    unsigned char pad_0E[0x8];
    short spriteValue;
    unsigned char pad_18[0x2];
    short timer;
} BattleFxSuperNovaDecelSpriteRecord;

/* SNova two-model polygon part record: frame count and three positions. */
typedef struct BattleFxSuperNovaTwoModelPolygonRecord {
    short ownerSlot;
    short frameCount;
    short posA;
    short posB;
    short posC;
} BattleFxSuperNovaTwoModelPolygonRecord;

/* SNova growing tumbling model record: frame count and three positions. */
typedef struct BattleFxSuperNovaTumblingModelRecord {
    short ownerSlot;
    short frameCount;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
} BattleFxSuperNovaTumblingModelRecord;

/* S-Nova variant model handoff record: sentinel, counter and three values to the camera. */
typedef struct BattleFxSuperNovaHandoff {
    short ownerSlot;
    short counter;
    short resourceIndex;
    short handoffWordA;
    short handoffWordB;
    unsigned char pad_0A[0x8];
} BattleFxSuperNovaHandoff;

/* S-Nova sprite fade effect: frame index and total frame count. */
typedef struct BattleFxSuperNovaSpriteFade {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short frameCount;
    short resourceRowIndex;
} BattleFxSuperNovaSpriteFade;

/* Effect record of the Super Nova projected quad task: frame counter and spread index. */
typedef struct BattleFxSuperNovaQuadRecord {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short spreadIndex;
} BattleFxSuperNovaQuadRecord;

/* Triangle-ranking record of the S-Nova effect: a frame counter and a centre offset. */
typedef struct SuperNovaRankTriangle {
    short ownerSlot;
    short frameCount;
    short centerX;
    short centerY;
    short centerZ;
} SuperNovaRankTriangle;

/* Pulsing sprite emitter record of the S-Nova effect: a frame counter and a sprite index. */
typedef struct SuperNovaPulseSpriteSlot {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x12];
    short spriteIndex;
} SuperNovaPulseSpriteSlot;

/* Pulsing sprite record of the S-Nova effect: a frame counter and a sprite index passed to the draw args. */
typedef struct SuperNovaPulsingSprite {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short spriteIndex;
} SuperNovaPulsingSprite;

/* Effect-100 slot of the Nova decelerating sprite emitter, with a timer and an index. */
typedef struct BattleFxSuperNovaEmitterSlot {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x12];
    short index;
} BattleFxSuperNovaEmitterSlot;

/* Effect-100 slot driving the Nova indexed-model task; the model variant is chosen by index. */
typedef struct BattleFxSuperNovaModelSlot {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short modelVariant;
    short scale;
} BattleFxSuperNovaModelSlot;

/* Effect-100 slot for the Nova fading sprite blend, with an angle-driven timer. */
typedef struct BattleFxSuperNovaFadeSprite {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short spriteFrame;
} BattleFxSuperNovaFadeSprite;

/* Effect-100 slot for the Nova rising fading sprite; its posY decreases by 20 each frame. */
typedef struct BattleFxSuperNovaRisingSprite {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
} BattleFxSuperNovaRisingSprite;

/* White flash pulse record for the S-Nova effect, with a frame counter and total frame count. */
typedef struct BattleFxSuperNovaFlashPulse {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x12];
    short frameCount;
} BattleFxSuperNovaFlashPulse;

/* Decelerating sprite emitter record spawned by the S-Nova flash pulse. */
typedef struct BattleFxSuperNovaDecelSprite {
    unsigned char pad_00[0x2];
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    unsigned char pad_0E[0x8];
    short frameCount;
    unsigned char pad_18[0x2];
    short moveCount;
} BattleFxSuperNovaDecelSprite;

/* Render-state block that draws ranked triangles with a single intensity colour. */
typedef struct BattleFxTriangleRenderBlock {
    int opcode;
    int color;
    unsigned char pad_08[0x4];
    int uv0Clut;
    unsigned char pad_10[0x4];
    short triangleCount;
    short textureWord;
    unsigned char pad_18[0x4];
    short vertexBOffset;
} BattleFxTriangleRenderBlock;

/* Billboard sprite record for the S-Nova effect, with a frame counter, variant index and angle. */
typedef struct BattleFxSuperNovaBillboardSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short variantIndex;
    short angle;
} BattleFxSuperNovaBillboardSprite;

/* White-flash fade task record: frame counter and the frame at which the fade ends. */
typedef struct BattleFxWhiteFlashRecord {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x12];
    short endFrame;
} BattleFxWhiteFlashRecord;

/* Last-boss render-state block: tag word and colour bytes set by the supernova approach task. */
typedef struct BattleFxSuperNovaPointPrim {
    int commandWord;
    char colorR;
    char colorG;
    char colorB;
    char command;
    short screenX;
    short screenY;
} BattleFxSuperNovaPointPrim;

/* Sine-scaled model task record: phase counter and amplitude scaling the sine value. */
typedef struct BattleFxSineScaledModelRecord {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x6];
    short amplitude;
} BattleFxSineScaledModelRecord;

/* Fixed-pose model task record: frame counter, with three parameters copied into the frame. */
typedef struct BattleFxFixedPoseModel4Record {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
} BattleFxFixedPoseModel4Record;

/* Ranked triangle record with vertex offsets, read by BattleFx_SuperNovaDrawRankedTriangleMesh. */
typedef struct BattleFxRankedTriangleRecord {
    short numberTriList;
    short vertexBOffset;
    short vertexCOffset;
    short brightness;
    int uv0Clut;
} BattleFxRankedTriangleRecord;

/* Sine-scaled billboard task of the SNova effect; a frame counter sets the sine phase. */
typedef struct BattleFxSuperNovaSineBillboardTask {
    short ownerSlot;
    short frame;
    short billboardMatrix;
} BattleFxSuperNovaSineBillboardTask;

/* Sine-scaled model task of the SNova effect, scaled by an amplitude over a fixed frame count. */
typedef struct BattleFxSuperNovaSineScaledModelTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short amplitude;
} BattleFxSuperNovaSineScaledModelTask;

/* Billboard task of the SNova effect whose scale value eases toward 0x1000 each frame. */
typedef struct BattleFxSuperNovaEasingBillboardTask {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short billboardMatrix;
    unsigned char pad_06[0x12];
    short easedValue;
} BattleFxSuperNovaEasingBillboardTask;

/* Three-step scale history for the shrinking spinning triangles, shifted each frame. */
typedef struct Battle3dShrinkScale {
    int prevScale;
    int scale;
    int nextScale;
} Battle3dShrinkScale;

/* Main-slot emitter for the nova pulsing sprites, keyed by sprite index at +0x16. */
typedef struct BattleFxNovaPulsingEmitter {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short spriteIndex;
} BattleFxNovaPulsingEmitter;

/* Task slot holding the centre point the nova triangle ranking measures distances from. */
typedef struct BattleFxSuperNovaRankCenter {
    unsigned char pad_00[0x4];
    short centerX;
    short centerY;
    short centerZ;
} BattleFxSuperNovaRankCenter;

/* Per-effect state of the SNova battle effect, reached through the pointer kept at frame+308. */
typedef struct BattleFxSuperNovaState {
    unsigned char pad_00[0x4];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short spinRate;
    unsigned char pad_12[0x2];
    short duration;
    short timer;
    short scale;
    short phase;
} BattleFxSuperNovaState;

/* Per-triangle displacement of three shorts, accumulated each step of the S-Nova scatter. */
typedef struct BattleFxSuperNovaDisplacement2 {
    short dispX;
    short dispY;
    short dispZ;
    unsigned char pad_06[0x2];
} BattleFxSuperNovaDisplacement2;

/* Per-task state for the S-Nova main task: a short counter that steps each frame. */
typedef struct BattleFxSuperNovaTaskState {
    short ownerSlot;
    short counter;
} BattleFxSuperNovaTaskState;

/* One 12-byte record of g_SuperNova_ApproachingPointFieldBuffer: state, age counter and point position. */
typedef struct SuperNova_ApproachingPointField {
    short state; /*+0x00 */
    short counter; /*+0x02 */
    short posX; /*+0x04 */
    short posY; /*+0x06 */
    short posZ; /*+0x08 */
    unsigned char pad_0A[0x2];
} SuperNova_ApproachingPointField;

/* Variant model state that receives handoff values from the S-Nova handoff record and clears two shorts. */
typedef struct BattleFxSuperNovaVariantModelState {
    unsigned char pad_00[0x4];
    short resourceIndex;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_12[0x6];
    short scale;
    short clearedWord;
} BattleFxSuperNovaVariantModelState;

/* Scratch block for one spinning triangle: per-vertex deltas from the centroid, plus the centroid. */
typedef struct BattleFxTriangleScratch {
    short deltaAX;
    short deltaAY;
    short deltaAZ;
    unsigned char pad_06[0x2];
    short deltaBX;
    short deltaBY;
    short deltaBZ;
    unsigned char pad_0E[0x2];
    short deltaCX;
    short deltaCY;
    short deltaCZ;
    unsigned char pad_16[0xa];
    short centroidX;
    short centroidY;
    short centroidZ;
} BattleFxTriangleScratch;

#pragma pack(push, 1)
typedef struct SuperNova_CameraEasingState {
    int thirdIndex; /* +0x0 */
    unsigned char pad_4[0x4];
    int easedValue; /* +0x8: the rotation matrix, whose translation vector follows */
    unsigned char pad_c[0xe];
    int transX; /* +0x1a */
    int transY; /* +0x1e */
    int transZ; /* +0x22 */
} SuperNova_CameraEasingState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SuperNova_CameraScriptScratchPointA {
    short status; /* +0x0 */
    short waiting; /* +0x2 */
} SuperNova_CameraScriptScratchPointA;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SuperNova_CameraScriptScratchPointB {
    short attachBone; /* +0x0 */
    short boneIdx; /* +0x2 */
} SuperNova_CameraScriptScratchPointB;
#pragma pack(pop)

/* The first 16 bytes of BattleFxModelDraw, which is all the SNova tasks initialize; the rest is read as is. */
typedef struct SuperNova_ModelDrawHead {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} SuperNova_ModelDrawHead;

/* --- sting --- */
/* Effect100 slot record for the Sting envelope: frame counter, amplitude and three phase durations. */
typedef struct BattleFxRubyTentaclesEnvelope {
    short step;
    short frame;
    short amplitude;
    short attackFrames;
    short sustainFrames;
    short releaseFrames;
} BattleFxRubyTentaclesEnvelope;

/* Effect-slot record for the Sting effect task: actor index and a timer. */
typedef struct BattleFxRubyTentaclesEffectRecord {
    short actorIndex;
    short timer;
} BattleFxRubyTentaclesEffectRecord;

/* Effect record of the weapon 1 record task: actor index and frame counter. */
typedef struct BattleFxRubyTentaclesRecord {
    short actorIndex;
    short frameCount;
} BattleFxRubyTentaclesRecord;

/* Radial sprite for the sting effect; x and z offset from the timeline slot by sin and cos. */
typedef struct BattleFxRubyTentaclesRadialSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short size;
} BattleFxRubyTentaclesRadialSprite;

/* Sprite particle task of the Sting effect; a frame counter walks the sprite table. */
typedef struct BattleFxRubyTentaclesParticleSprite {
    short ownerSlot;
    short frame;
    Vec3sPacked position;
    unsigned char pad_0A[0x2];
    short size;
} BattleFxRubyTentaclesParticleSprite;

/* Radial emitter slot the Sting timelines set up: the emitter centre in X and Z. */
typedef struct BattleFxRubyTentaclesTimelineSlot {
    unsigned char pad_00[0x4];
    short centerX;
    unsigned char pad_06[0x2];
    short centerZ;
} BattleFxRubyTentaclesTimelineSlot;

/* Main-slot envelope entry for the sting effect: start, end, duration and step. */
typedef struct BattleFxRubyTentaclesEnvelopeEntry {
    unsigned char pad_00[0x4];
    short start;
    short end;
    short duration;
    short step;
} BattleFxRubyTentaclesEnvelopeEntry;

/* Vertex scratch of the Sting mesh draw: RotTransPoint's output for the current vertex (its Y
 * is tested for negative), then the transformed vertices the primitives index. */
typedef struct BattleFxRubyTentaclesPsxMeshEntry {
    short rotTransX;
    unsigned char pad_02[0x2];
    int rotTransY;
    short rotTransZ;
    unsigned char pad_0A[0x6];
    SVECTOR projected[510];
} BattleFxRubyTentaclesPsxMeshEntry;


/* Task slot for the Weapon1 spawn camera script: script pointer and matrix pointer. */
typedef struct BattleFxRubyTentaclesCameraSlot {
    unsigned char pad_00[0xc];
    int scriptPtr;
    int matrixPtr;
    short value;
} BattleFxRubyTentaclesCameraSlot;

/* Actor record at +0 used by the sting effect, with an attached part count. */
typedef struct BattleFxActorAttachedParts {
    unsigned char pad_00[0x10];
    short partCount;
    unsigned char pad_12[0x4];
    short rootYaw;
} BattleFxActorAttachedParts;

#pragma pack(push, 1)
typedef struct RubyTentaclesDirectionMatrix {
    short m00; /* +0x0 */
    unsigned char pad_2[0xe];
    short m22; /* +0x10 */
    unsigned char pad_12[0x8];
    int transZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} RubyTentaclesDirectionMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct RubyTentaclesParticleState {
    short stateWord; /* +0x0 */
    int posXDword; /* +0x2 */
    int posYDword; /* +0x6 */
    int posZDword; /* +0xa */
} RubyTentaclesParticleState;
#pragma pack(pop)

/* Model render descriptor of the Sting model: the model data (a byte count, the vertices, then the
 * primitive stream), the list flags, the shade weight the animation blend sets and the tpage bits. */
typedef struct RubyTentaclesModelRenderDescriptor {
    unsigned int model;
    int flags;
    short pad_08;
    short shade;
    short attrMask;
    unsigned char pad_0E[0x2];
} RubyTentaclesModelRenderDescriptor;

/* Records of a Sting primitive stream. The stream holds four sections (flat textured triangles,
 * flat textured quads, gouraud triangles, gouraud quads); each opens with a count word and a word
 * of attribute bits and then packs its records. Every record starts with the byte offsets of its
 * vertices in the vertex block, and the draw loops read those of the next record one step ahead. */
typedef struct RubyTentaclesStreamFt3 {
    short vertex0;
    short vertex1;
    short vertex2;
    short gray; /* grey level in the low byte, used when the list is lit */
    int uv0Clut;
    short uv1;
    short uv2;
} RubyTentaclesStreamFt3;

typedef struct StingStreamFt4 {
    short vertex0;
    short vertex1;
    short vertex2;
    short vertex3;
    int uv0Clut;
    short uv1;
    short uv2;
    short uv3;
    short gray; /* grey level in the low byte, used when the list is lit */
} StingStreamFt4;

typedef struct StingStreamG3 {
    short vertex0;
    short vertex1;
    short vertex2;
    unsigned char pad_06[0x2];
    union {
        int color0;
        CVECTOR rgb0;
    };
    union {
        int color1;
        CVECTOR rgb1;
    };
    union {
        int color2;
        CVECTOR rgb2;
    };
} StingStreamG3;

typedef struct StingStreamG4 {
    short vertex0;
    short vertex1;
    short vertex2;
    short vertex3;
    union {
        int color0;
        CVECTOR rgb0;
    };
    union {
        int color1;
        CVECTOR rgb1;
    };
    union {
        int color2;
        CVECTOR rgb2;
    };
    union {
        int color3;
        CVECTOR rgb3;
    };
} StingStreamG4;

/* Cursor over a Sting primitive stream: a byte address, or the record of the section it is in. */
typedef union StingStreamCursor {
    int addr;
    short *word;
    RubyTentaclesStreamFt3 *ft3;
    StingStreamFt4 *ft4;
    StingStreamG3 *g3;
    StingStreamG4 *g4;
} StingStreamCursor;

/* PSX GPU packets the Sting stream draws fill: the ordering-table tag, the first colour with the
 * GPU command in its cd byte, then per vertex its screen XY word and its UV (textured packets)
 * or colour (gouraud packets). */
typedef struct RubyTentaclesPolyFt3 {
    int tag;
    union {
        int color;
        CVECTOR rgb;
    };
    int xy0;
    union {
        int uv0Clut;
        struct {
            unsigned char u0;
            unsigned char v0;
        };
    };
    int xy1;
    union {
        short uv1;
        struct {
            unsigned char u1;
            unsigned char v1;
        };
    };
    short tpage;
    int xy2;
    union {
        short uv2;
        struct {
            unsigned char u2;
            unsigned char v2;
        };
    };
    unsigned char pad_1E[0x2];
} RubyTentaclesPolyFt3;

typedef struct RubyTentaclesPolyFt4 {
    int tag;
    union {
        int color;
        CVECTOR rgb;
    };
    int xy0;
    union {
        int uv0Clut;
        struct {
            unsigned char u0;
            unsigned char v0;
        };
    };
    int xy1;
    union {
        short uv1;
        struct {
            unsigned char u1;
            unsigned char v1;
        };
    };
    short tpage;
    int xy2;
    union {
        short uv2;
        struct {
            unsigned char u2;
            unsigned char v2;
        };
    };
    unsigned char pad_1E[0x2];
    int xy3;
    union {
        short uv3;
        struct {
            unsigned char u3;
            unsigned char v3;
        };
    };
    unsigned char pad_26[0x2];
} RubyTentaclesPolyFt4;

typedef struct RubyTentaclesPolyG3 {
    int tag;
    union {
        int color0;
        CVECTOR rgb0;
    };
    int xy0;
    union {
        int color1;
        CVECTOR rgb1;
    };
    int xy1;
    union {
        int color2;
        CVECTOR rgb2;
    };
    int xy2;
} RubyTentaclesPolyG3;

typedef struct RubyTentaclesPolyG4 {
    int tag;
    union {
        int color0;
        CVECTOR rgb0;
    };
    int xy0;
    union {
        int color1;
        CVECTOR rgb1;
    };
    int xy1;
    union {
        int color2;
        CVECTOR rgb2;
    };
    int xy2;
    union {
        int color3;
        CVECTOR rgb3;
    };
    int xy3;
} RubyTentaclesPolyG4;

/* --- vahamut0 --- */
/* Camera script runner state: script read position, started flag and rotation matrix pointer. */
typedef struct BattleFxBahamutZeroCameraScript {
    short ownerSlot;
    short started;
    unsigned char pad_04[0x8];
    short *scriptPos;
    BattleFxActorMatrix *rotMatrix;
    short attachPoint;
} BattleFxBahamutZeroCameraScript;

/* Bahamut Zero light rig global holding a sweep angle and related light parameters. */
typedef struct BattleBzLightRig {
    unsigned char pad_00[0x6];
    short scale;
    unsigned char pad_08[0xc];
    short rampPhase;
    unsigned char pad_16[0x12];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_2B[0x133];
    short rotX;
    short yaw;
    unsigned char pad_162[0x4];
    short posX;
    short posY;
    short posZ;
} BattleBzLightRig;

/* Render-state block reused as a ground spark primitive, with paired color bytes and coordinates. */
typedef struct BattleFxGroundLivewirePrim {
    int tagWord;
    int colorRgb;
    unsigned char pad_08[0x4];
    char uCoord;
    char vCoord;
    short clut;
    unsigned char pad_10[0x4];
    char uNextTop;
    char vStart;
    short texturePage;
    unsigned char pad_18[0x4];
    char uStart;
    char vNextBottom;
    unsigned char pad_1E[0x6];
    char uNext;
    char vNext;
} BattleFxGroundLivewirePrim;

/* Bahamut Zero ground spark state: origin and target positions for each spark. */
typedef struct BattleFxBzGroundLivewireScratch {
    short curX;
    short curY;
    short vertex0Vz;
    unsigned char pad_06[0x2];
    short curTargetX;
    short originY;
    short vertex1Vz;
    unsigned char pad_0E[0x2];
    short originX;
    short curTargetY;
    short vertex2Vz;
    unsigned char pad_16[0x2];
    short targetX;
    short targetY;
    short vertex3Vz;
    unsigned char pad_1e[0x2];
    short centerX; /* +0x20 */
    short centerY; /* +0x22 */
    int flags;
} BattleFxBzGroundLivewireScratch;

/* Bahamut Zero sprite-trail state block, set up before the trail is drawn. */
typedef struct BattleFxBahamutZeroTrail {
    unsigned char pad_00[0x8];
    int startX;
    int startY;
    int depth;
    unsigned char pad_14[0x4];
    short m00;
    short m01;
    short deltaY;
    short m10;
    short halfHeight;
    short negHalfWidth;
    short m20;
    short halfWidth;
    short deltaX;
} BattleFxBahamutZeroTrail;

/* Bahamut Zero ground-spark list header: entry count, then packed spark records. */
typedef struct BattleFxBzGroundLivewireRecord {
    unsigned char pad_00[0x2];
    short count;
    short gridX;
    short gridY;
    char uStart;
    unsigned char pad_09[0x1];
    char vStart;
    unsigned char pad_0B[0x1];
    short tpage;
    short clut;
    short cellWidth;
    short cellHeight;
} BattleFxBzGroundLivewireRecord;

/* Bahamut Zero slot 8 record: frame counter, start position and per-axis delta. */
typedef struct BattleFxBzSlot08Record {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
} BattleFxBzSlot08Record;

/* Dust or ground-spark particle of the Bahamut Zero effects. */
typedef struct BattleFxBzParticle {
    unsigned char pad_00[0x4];
    short spawnValue;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotationX;
    short rotationY;
    short rotationZ;
    unsigned char pad_12[0xa];
    short scale;
} BattleFxBzParticle;

/* Effect-slot record for the Bahamut Zero spark swarm, stepped each frame. */
typedef struct BattleFxLivewireSwarmRecord {
    short step;
    short frame;
    unsigned char pad_04[0xc];
    short phase;
    unsigned char pad_12[0xa];
    short scale;
    short phaseStep;
} BattleFxLivewireSwarmRecord;

/* Swarm particle record spawned into the Bz slot 14 effect. */
typedef struct BattleFxBzSwarmParticle {
    unsigned char pad_00[0x4];
    short posX;
    short startOffset;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotationX;
    short phase;
    short rotationZ;
    unsigned char pad_12[0xa];
    short scale;
} BattleFxBzSwarmParticle;

/* Bz slot 04/05 draw record with a frame counter. */
typedef struct BattleFxBzSlotRecord {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x2];
    short rotY;
    unsigned char pad_08[0x6];
    short countdown;
} BattleFxBzSlotRecord;

/* Three integer state words of the Bahamut Zero sky dome, each set to one shared value. */
typedef struct BattleFxBahamutZeroState {
    int skyLayerA;
    int skyLayerB;
    int skyLayerC;
} BattleFxBahamutZeroState;

/* Ring-buffer light-rig record: position from the Bahamut Zero state plus offsets. */
typedef struct BattleFxBzLightRig {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x6];
    short effectScale;
} BattleFxBzLightRig;

/* Task slot for the Bz spawn camera script: eye track, focus track and duration. */
typedef struct BattleFxBzCameraSlot {
    unsigned char pad_00[0xc];
    int eyeTrack;
    int focusTrack;
    short duration;
} BattleFxBzCameraSlot;

/* Primitive packet for the Bz spark swarm, with its command word and vertex delta pointers. */
typedef struct BattleFxBzLivewireSwarmPrim {
    unsigned char pad_00[0x4];
    int color;
    unsigned char pad_08[0x4];
    int vertexDeltas;
    unsigned char pad_10[0x4];
    int deltaData;
} BattleFxBzLivewireSwarmPrim;

/* Spark swarm particle written by BattleFx_BzSpawnSparkSwarm. */
typedef struct BattleFxLivewireParticle {
    short spawnFrame;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotationX;
    short rotationY;
    short rotationZ;
    unsigned char pad_12[0xa];
    short paramT;
    short paramS;
} BattleFxLivewireParticle;

/* The PSX quad packet (POLY_GT4, 0x34 bytes) of one cell of the Bz energy tube mesh. The top
 * byte of color0 is the primitive code. */
typedef struct BattleFxBzEnergyTubeVertex {
    int tag;
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
} BattleFxBzEnergyTubeVertex;

/* The ring-buffer record of type 7 the Bahamut ZERO timeline allocates at four of its stages; the
 * timeline sets only its parameter, to 7, 8, 9 and 10 in turn. */
typedef struct BattleFxBzRingRecord {
    unsigned char pad_00[0x10];
    short param;
} BattleFxBzRingRecord;

/* One cell of the Bz energy tube, 0x44 bytes: pointers to its four ring vertices, then its quad. */
typedef struct BattleFxBzTubeCell {
    struct BattleBahamutZeroTubeVertex *cornerRowCol;
    struct BattleBahamutZeroTubeVertex *cornerNextCol;
    struct BattleBahamutZeroTubeVertex *cornerNextRow;
    struct BattleBahamutZeroTubeVertex *cornerNextRowCol;
    BattleFxBzEnergyTubeVertex packet[1];
} BattleFxBzTubeCell;

/* One vertex of a ring of the Bz energy tube. */
typedef struct BattleBahamutZeroTubeVertex {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
} BattleBahamutZeroTubeVertex;

/* Debris particle placed on a ring by BattleFx_BzSpawnRingDebris. */
typedef struct BattleFxRingDebrisRecord {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short offsetX;
    short offsetZ;
    short deltaZ;
    unsigned char pad_12[0x2];
    short rotX;
    short rotY;
    short angle;
} BattleFxRingDebrisRecord;

/* The Bahamut Zero energy tube (0x9A0C bytes): 33 rings of 16 vertices, the 32 x 16 cells that
 * join them, and per ring a height, a velocity and a shade colour. BattleFx_BzStepEnergyTube
 * steps it. */
typedef struct BattleBahamutZeroTube {
    BattleBahamutZeroTubeVertex ring[33][16];
    BattleFxBzTubeCell cell[32][16];
    int height[33];
    int velocity[33];
    int color[33];
} BattleBahamutZeroTube;

/* Slot record of the BZ master timeline; time is a counter used as the frame time. */
typedef struct BattleFxBzMasterSlot {
    short ownerSlot;
    short time;
} BattleFxBzMasterSlot;

/* Sub-task slot (0x20 bytes) of the Bahamut Zero summon; -1 marks a free slot. */
typedef struct BattleFxBzSubTask {
    short state;
    short frame;
    unsigned char pad_04[0x1c];
} BattleFxBzSubTask;

/* One record of g_BattleSummonBahamutZeroGlowOffsets (0x4 bytes). */
typedef struct BattleSummonBahamutZeroGlowOffset {
    short glowIndex; /*+0x00 */
    short glowOffset; /*+0x02 */
} BattleSummonBahamutZeroGlowOffset;

/* PSX sprite primitive filled from a sprite-parameter block. */
typedef struct BattleFxSpritePrim {
    int packetTag;
    union {
        int colorWord;
        struct {
            unsigned char colorRGB[3];
            char code; /* the GPU command byte */
        };
    };
    int packedXY;
    char u;
    char v;
    short clut;
    int packedSize;
} BattleFxSpritePrim;

/* Descriptor of a morphing LMD model for BattleFx_DrawMorphedLmdOuter/Inner: the model data,
 * draw flags, a shade amount and the texture page bits. */
typedef struct BattleFxBzMorphDesc {
    int lmd;
    unsigned int flags;
    unsigned char pad_08[0x2];
    short shade;
    short tpage;
} BattleFxBzMorphDesc;

/* The data block of a Bahamut Zero model draw (slots 04/05, 08, 0E, 0F, 13, 14): the scale triple
 * ScaleMatrix reads, then the transform matrix. */
typedef struct BattleFxBzModelBlock {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix;
} BattleFxBzModelBlock;

/* Scratch block of the Bahamut Zero slot 0 to 10 draw: three rotation shorts, the scale triple
 * and two matrices (the model's, then the one composed for the draw). */
typedef struct BattleBahamutZeroState {
    short vectorX;
    short stateWord;
    short timer;
    unsigned char pad_06[0x2];
    int offsetX;
    int offsetY;
    int progress;
    unsigned char pad_14[0x4];
    PsxMatrix matrix;
    PsxMatrix drawMatrix;
    int drawFlag;
} BattleBahamutZeroState;

/* Bahamut Zero draw block for slot 12: three rotation shorts, the scale triple and the matrix. */
typedef struct BattleFxBahamutZeroDrawState {
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_06[0x2];
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix;
} BattleFxBahamutZeroDrawState;

/* Bahamut Zero spark swarm record: the scale triple, the matrix, and the projected point. */
typedef struct BattleFxBahamutZeroLivewire {
    int scale;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix;
    short screenX;
    short screenY;
    unsigned char pad_32[0x2];
    int depth;
    int projectedA;
    int projectedB;
    int geomOffsetX;
    int geomOffsetY;
} BattleFxBahamutZeroLivewire;

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroAimMatrixTail {
    short stateValue; /* +0x0 */
    int aimVector; /* +0x2 */
    unsigned char pad_6[0x4];
    int aimAccumulator; /* +0xa */
    short bahamutZeroScriptCountBase; /* +0xe */
} BattleSummonBahamutZeroAimMatrixTail;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroEffectScaleState {
    short effectScale; /* +0x0 */
    short effectScaleCurrent; /* +0x2 */
} BattleSummonBahamutZeroEffectScaleState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroCameraState {
    short cameraState; /* +0x0 */
    short waitFlag; /* +0x2 */
} BattleSummonBahamutZeroCameraState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroAttachState {
    short actorSelector; /* +0x0 */
    short boneIndex; /* +0x2 */
} BattleSummonBahamutZeroAttachState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroCameraOffset {
    short offsetStepX; /* +0x0 */
    short offsetStepY; /* +0x2 */
} BattleSummonBahamutZeroCameraOffset;
#pragma pack(pop)

/* The data block BattleFx_BzDrawEnergyTubeTask takes from Gfx_GetPsxScratchpadPtr(0): the tube's
 * matrix, its scale triple and the outputs of the quad transform. */
#pragma pack(push, 1)
typedef struct BattleFxBzTubeDrawBlock {
    PsxMatrix matrix; /* +0x0 */
    int scaleX; /* +0x1e */
    int scaleY; /* +0x22 */
    int scaleZ; /* +0x26 */
    unsigned char pad_2a[0x6];
    int depth; /* +0x30 */
    int quadOut; /* +0x34 */
    int quadFlags; /* +0x38 */
} BattleFxBzTubeDrawBlock;
#pragma pack(pop)

/* The data block of BattleFx_BzRunLightRig: a short vector, then the matrix that carries it. */
typedef struct BattleFxBzRigBlock {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
    PsxMatrix matrix;
} BattleFxBzRigBlock;

/* The data block BattleFx_BzSpinWorldMatrix takes from Gfx_GetPsxScratchpadPtr(0): three rotation
 * shorts, then the matrix it composes into the world matrix. */
typedef struct BattleFxBzSpinBlock {
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_06[0x2];
    PsxMatrix matrix;
} BattleFxBzSpinBlock;

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroMatrixPair {
    unsigned char spinBlockLoaded; /* bit 0: BattleFx_BzSpinWorldMatrix has taken its block */
    unsigned char pad_1[0x3];
    BattleFxBzTubeDrawBlock *dataBlockPtr; /* +0x4 */
    PsxMatrix matrixA; /* +0x8 */
    unsigned char pad_26[0x2];
    PsxMatrix matrixB; /* +0x28 */
} BattleSummonBahamutZeroMatrixPair;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBahamutZeroMatrixAAngles {
    short matrixAState; /* +0x0 */
    short bahamutZeroMatrixAStateWord; /* +0x2 */
} BattleBahamutZeroMatrixAAngles;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBahamutZeroMatrixBAngles {
    short matrixBState; /* +0x0 */
    short bahamutZeroMatrixBStateWord; /* +0x2 */
} BattleBahamutZeroMatrixBAngles;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBahamutZeroBoneTarget {
    short boneIndex; /* +0x0 */
    short boneIdx; /* +0x2 */
} BattleBahamutZeroBoneTarget;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleBahamutZeroComposedMatrix {
    unsigned char pad_0[0x4];
    short startX; /* +0x4 */
    unsigned char pad_6[0x2];
    short halfHeight; /* +0x8 */
    short halfWidth; /* +0xa */
    unsigned char pad_c[0x4];
    short depth; /* +0x10 */
    unsigned char pad_12[0xc];
} BattleBahamutZeroComposedMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot04And05Data {
    int bahamutZeroSlot04ModelDword; /* +0x0 */
    unsigned char pad_4[0x6];
    short bahamutZeroCachedWordA; /* +0xa */
    unsigned char pad_c[0x4];
    int bahamutZeroSlot05ModelDword; /* +0x10 */
    unsigned char pad_14[0x6];
    short bahamutZeroCurrentWordA; /* +0x1a */
} BattleSummonBahamutZeroSlot04And05Data;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot06And07Data {
    int bahamutZeroSlot06ModelDword; /* +0x0 */
    unsigned char pad_4[0xc];
    int bahamutZeroSlot07ModelDword; /* +0x10 */
} BattleSummonBahamutZeroSlot06And07Data;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroGlowBaseColor {
    unsigned char pad_0[0x4];
    int bahamutZeroGlowTableBase; /* +0x4 */
} BattleSummonBahamutZeroGlowBaseColor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot08Data {
    unsigned char pad_0[0x10];
    int bahamutZeroSlot08ModelDword; /* +0x10 */
} BattleSummonBahamutZeroSlot08Data;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot09Data {
    unsigned char pad_0[0x8];
    int bahamutZeroSlot09ModelDword; /* +0x8 */
    unsigned char pad_c[0x6];
    short bahamutZeroSlot09Word; /* +0x12 */
} BattleSummonBahamutZeroSlot09Data;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot0AParams {
    unsigned char pad_0[0x8];
    int bahamutZeroSlot0ADrawHandle; /* +0x8 */
    unsigned char pad_c[0x6];
    short bahamutZeroSlot0AScaleFactor; /* +0x12 */
    unsigned char pad_14[0x4];
    short bahamutZeroSlot0ARotationAngle0; /* +0x18 */
    unsigned char pad_1a[0x6];
    short bahamutZeroSlot0ARotationAngle1; /* +0x20 */
    unsigned char pad_22[0x6];
    short bahamutZeroSlot0ARotationAngle2; /* +0x28 */
    unsigned char pad_2a[0x2];
} BattleSummonBahamutZeroSlot0AParams;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxBzPillarQuadSlot {
    unsigned char pad_0[0x8];
    int bahamutZeroSlot0BDrawHandle; /* +0x8 */
} BattleFxBzPillarQuadSlot;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot0CTo10Angle {
    short angleX; /* +0x0 */
    short bahamutZeroSlot0CTo10StateWord; /* +0x2 */
} BattleSummonBahamutZeroSlot0CTo10Angle;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot0ERandomData {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short bahamutZeroSlot0ERandomPhase; /* +0xa */
} BattleSummonBahamutZeroSlot0ERandomData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxBzCoreQuadSlot {
    unsigned char pad_0[0x8];
    int bahamutZeroSlot0FDrawHandle; /* +0x8 */
} BattleFxBzCoreQuadSlot;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroScreenPrim {
    unsigned char pad_0[0x8];
    short bahamutZeroScreenPrimOriginX; /* +0x8 */
    short bahamutZeroScreenPrimOriginY; /* +0xa */
} BattleSummonBahamutZeroScreenPrim;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroScreenPrimSize {
    short bahamutZeroScreenPrimWidth; /* +0x0 */
    short bahamutZeroScreenPrimHeight; /* +0x2 */
} BattleSummonBahamutZeroScreenPrimSize;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSkyLmdPtrs {
    int skyALmdPointer; /* +0x0 */
    unsigned char pad_4[0xc];
    int bahamutZeroSkyDomeLmdPointer; /* +0x10 */
} BattleSummonBahamutZeroSkyLmdPtrs;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroRingModelData {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0xc];
    short bahamutZeroRingRotation0; /* +0x10 */
    unsigned char pad_12[0x6];
    short bahamutZeroRingRotation1; /* +0x18 */
    unsigned char pad_1a[0x6];
    short bahamutZeroRingRadius; /* +0x20 */
    unsigned char pad_22[0x2];
} BattleSummonBahamutZeroRingModelData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxBzFlareQuadSlot {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0xc];
    int bahamutZeroSlot10And16DrawHandle; /* +0x10 */
} BattleFxBzFlareQuadSlot;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroLightRig {
    unsigned char pad_0[0x12];
    int bahamutZeroLightRigOffsetX; /* +0x12 */
    int bahamutZeroLightRigOffsetY; /* +0x16 */
    int bahamutZeroLightRigOffsetZ; /* +0x1a */
} BattleSummonBahamutZeroLightRig;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot0BState {
    short matrixM22; /* +0x0 */
    int bahamutZeroSlot0BPosition0; /* +0x2 */
    int bahamutZeroSlot0BPosition1; /* +0x6 */
    int bahamutZeroSlot0BPosition2; /* +0xa */
} BattleSummonBahamutZeroSlot0BState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxBzBeamScaleTail {
    short initialDepth; /* +0x0 */
    unsigned char pad_2[0x8];
    int bahamutZeroSlot0CTo10Depth; /* +0xa */
} BattleFxBzBeamScaleTail;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonBahamutZeroSlot15ScaleTail {
    short bahamutZeroScale; /* +0x0 */
    unsigned char pad_2[0x8];
    int bahamutZeroSlot15ZTranslation; /* +0xa */
} BattleSummonBahamutZeroSlot15ScaleTail;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleFxBzDrawGlowSpriteW {
    unsigned char pad_0[0x12];
    int baseX; /* +0x12 */
    int baseY; /* +0x16 */
    int baseZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleFxBzDrawGlowSpriteW;
#pragma pack(pop)

/* --- yama_swirl_transition_module --- */
/* Packet of four screen corners and a CLUT word, drawn as a textured flat quad by Swirl_DrawPacketQuad. */
typedef struct YamaSwirlQuadPacket {
    unsigned char pad_00[0x8];
    short x0;
    short y0;
    unsigned char pad_0C[0x2];
    unsigned short texPage;
    short x1;
    short y1;
    unsigned char pad_14[0x2];
    unsigned short clutWord;
    short x2;
    short y2;
    unsigned char pad_1C[0x4];
    short x3;
    short y3;
} YamaSwirlQuadPacket;

#pragma pack(push, 1)
typedef struct SwirlSpriteLeftDim {
    short spriteW; /* +0x0 */
    short spriteH; /* +0x2 */
} SwirlSpriteLeftDim;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SwirlSpriteLeftPos {
    short spriteX; /* +0x0 */
    short spriteY; /* +0x2 */
} SwirlSpriteLeftPos;
#pragma pack(pop)

/* --- yasui_battle_fx_magic --- */
/* Timeline record for the Magic Wall effect, stepping through frame-keyed events. */
typedef struct BattleFxMagicWallTimeline {
    short ownerSlot;
    short frame;
    Int64Pair startPos;
    Int64Pair actorPos;
    short actorIndex;
} BattleFxMagicWallTimeline;

/* Battle task slot for the magic wall effect, offset each frame and tied to a target actor. */
typedef struct BattleFxMagicWallTask {
    unsigned char pad_00[0x4];
    short x;
    unsigned char pad_06[0x2];
    short z;
    unsigned char pad_0A[0xa];
    short targetActor;
} BattleFxMagicWallTask;

/* --- yasui_item_echo_screen --- */
/* The Echo Screen effect's task record, shared by the per-target emitter and its animated sprites.
 * The emitter keeps the target actor in ownerSlot and, in offsetX/offsetY/pushLength, the burst's
 * spread and push; a sprite takes the target position and its own offsets, scale and frame. */
typedef struct BattleFxEchoScreenTask {
    short ownerSlot; /* the target actor for the emitter; BATTLE_TASK_FREE when a sprite is released */
    short frame;
    short pos[3]; /* the target's bone position, copied from emitter to sprite */
    unsigned char pad_0a[0x2];
    short offsetX;
    short offsetY;
    short scale;
    short pushLength;
} BattleFxEchoScreenTask;

#pragma pack(push, 1)
typedef struct YasuiItemEchoScreenSpriteRequest {
    int loadedRes; /* +0x0 */
    int battleYamabikoSpriteObjectField; /* +0x4 */
    unsigned char pad_8[0x4];
    short battleYamabikoSpriteFrameIndex; /* +0xc */
    unsigned char pad_e[0x2];
} YasuiItemEchoScreenSpriteRequest;
#pragma pack(pop)

/* --- yasui_item_smoke_bomb --- */
/* Effect-100 slot for the Kemuri animated sprite; frame index and screen translation. */
typedef struct BattleFxSmokeBombSprite {
    short ownerSlot;
    short frame;
    short translationX;
    short translationY;
} BattleFxSmokeBombSprite;

/* Task record with a 0xFFFF sentinel and a count capped at 0x1E for the kemuri sprite emitter. */
typedef struct BattleFxSmokeBombEmitterCounter {
    short ownerSlot;
    short count;
} BattleFxSmokeBombEmitterCounter;

/* Main task slot for a kemuri sprite, given random X and Y offsets. */
typedef struct BattleItemFxSmokeBombSpriteTask {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
} BattleItemFxSmokeBombSpriteTask;

#pragma pack(push, 1)
typedef struct YasuiItemSmokeBombTransformMatrix {
    unsigned char pad_0[0x12];
    int battleKemuriTranslationX; /* +0x12 */
    int battleKemuriTranslationY; /* +0x16 */
    int battleKemuriTranslationDepth; /* +0x1a */
} YasuiItemSmokeBombTransformMatrix;
#pragma pack(pop)

/* --- yasui_limit_angermax_impacts --- */
/* Effect-slot record for the A-Max limit timeline, indexed into a delay table. */
typedef struct BattleFxUngarmaxTimelineRecord {
    short sequenceIndex;
    short step;
} BattleFxUngarmaxTimelineRecord;

/* A task slot of the A-Max impact effects (billboard model, animated sprite, fading spark): frame
 * counter, position, the spark's velocity and colour, and the billboard push length. */
typedef struct YasuiLimitUngarmaxImpactSlot {
    short ownerSlot;
    short frameCount;
    SVECTOR pos;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    CVECTOR color;
    short pushLength;
} YasuiLimitUngarmaxImpactSlot;

/* The scratch block Gfx_GetPsxScratchpadPtr(0) returns to the impact spawner: the random bone position, the
 * spark's launch direction and the matrix built from it. */
typedef struct YasuiLimitUngarmaxImpactScratch {
    SVECTOR pos;
    SVECTOR dir;
    PsxMatrix matrix;
} YasuiLimitUngarmaxImpactScratch;

/* The scratch block Gfx_GetPsxScratchpadPtr(0) returns to the fading point particles: the projected depth and the two projection flags. */
typedef struct YasuiLimitUngarmaxProjectScratch {
    int depth;
    int flagA;
    int flagB;
} YasuiLimitUngarmaxProjectScratch;

/* Ring buffer record for the A-Max timeline, with the current target actor and sequence index. */
typedef struct BattleFxUngarmaxHitRingRecord {
    short targetActor;
    unsigned char pad_02[0xe];
    short sequenceIndex;
} BattleFxUngarmaxHitRingRecord;

/* Render state block written with GPU-style command words for the A-Max fading point particles. */
typedef struct BattleFxLimitUngarmaxPointPrim {
    int packetTag; /* +0x0 */
    CVECTOR color; /* +0x4: the shaded colour; cd is the primitive command */
    short xy[2]; /* +0x8 */
    int size;
} BattleFxLimitUngarmaxPointPrim;

#pragma pack(push, 1)
typedef struct YasuiLimitUngarmaxAttachModelDrawReqPool {
    int modelRes; /* +0x0 */
    unsigned char pad_4[0xc];
    int req2; /* +0x10 */
} YasuiLimitUngarmaxAttachModelDrawReqPool;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitUngarmaxDrawReqPool {
    int modelRes; /* +0x0 */
    unsigned char pad_4[0xc];
    int spriteDrawReqObject; /* +0x10 */
    int drawObj; /* +0x14 */
    unsigned char pad_18[0x4];
    short spriteDrawReqFrame; /* +0x1c */
    unsigned char pad_1e[0x2];
} YasuiLimitUngarmaxDrawReqPool;
#pragma pack(pop)

/* --- yasui_limit_blade_beam --- */
/* Limit hazard beam trail emitter: trail point index (1-based, 0 = none) and frame count. */
typedef struct BattleFxLimitClimhazzardTrail {
    short pointIndex;
    short frameCount;
} BattleFxLimitClimhazzardTrail;

/* Scratch state for the limit hazard beam trail: two interpolated points with base, delta and offset. */
typedef struct YasuiLimitClimhazzardTrail {
    short point1BaseX;
    short point1BaseY;
    short point1BaseZ;
    unsigned char pad_06[0x2];
    short point2BaseX;
    short point2BaseY;
    short point2BaseZ;
    unsigned char pad_0E[0x2];
    short point1X;
    short point1Y;
    short point1Z;
    unsigned char pad_16[0x2];
    short point2X;
    short point2Y;
    short point2Z;
    unsigned char pad_1E[0x2];
    short trailOffsetX;
    short trailOffsetY;
    short trailOffsetZ;
    unsigned char pad_26[0x2];
    int point1DeltaX;
    int point1DeltaY;
    int point1DeltaZ;
    unsigned char pad_34[0x4];
    int point2DeltaX;
    int point2DeltaY;
    int point2DeltaZ;
    unsigned char pad_44[0x4];
    unsigned char matrix[0x1E]; /* +0x48: the actor's matrix relative to the camera (a PsxMatrix) */
    unsigned char pad_66[0x2];
    int rotateFlag; /* +0x68: RotTransPoint's flag output */
} YasuiLimitClimhazzardTrail;

/* Scratch block of the limit hazard timeline: the flash position, then the actor's matrix
 * relative to the camera (a PsxMatrix, its translation spelled out as ints). */
#pragma pack(push, 1)
typedef struct YasuiLimitClimhazzardTimelineScratch {
    short vx;
    short vy;
    short vz;
    unsigned char pad_06[0x2];
    unsigned char matrix[0x12]; /* +0x08: the PsxMatrix's rotation part */
    int transX; /* +0x1A */
    int transY;
    int transZ;
} YasuiLimitClimhazzardTimelineScratch;
#pragma pack(pop)

/* Effect record of the Limit hazard shaded quad trail task. */
typedef struct BattleFxLimitClimhazzardQuadRecord {
    short ownerSlot;
    short phase;
    unsigned char pad_04[0x8];
    int vertexBase;
} BattleFxLimitClimhazzardQuadRecord;

/* Hazard shaded-quad draw node linked into the yasui draw list head. */
typedef struct BattleFxClimhazzardDrawNode {
    int tagWord;
    int color0;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxClimhazzardDrawNode;

/* Timeline record of the Limit hazard effect; each axis is a short plus an int from a scratch block. */
typedef struct BattleFxLimitClimhazzardTimeline2 {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short pushLength;
} BattleFxLimitClimhazzardTimeline2;

/* Animated sprite task of the Limit hazard effect; a frame counter walks the sprite draw requests. */
typedef struct BattleFxLimitClimhazzardSpriteTask {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x8];
    short pushLength;
} BattleFxLimitClimhazzardSpriteTask;

/* Task slot of the Limit hazard beam trail emitter, pointing into the trail point array. */
typedef struct BattleFxLimitClimhazzardBeamEmitter {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x8];
    int trailPointPtr;
} BattleFxLimitClimhazzardBeamEmitter;

/* One record of g_YasuiLimitClimhazzardTrailPoint1 (0x10 bytes): the trail point's x, y and z. */
typedef struct YasuiLimitClimhazzardTrailPoint1 {
    short posX; /*+0x00 */
    short posY; /*+0x02 */
    short posZ; /*+0x04 */
    unsigned char pad_06[0xa];
} YasuiLimitClimhazzardTrailPoint1;


#pragma pack(push, 1)
typedef struct YasuiLimitClimhazzardEndpoints {
    unsigned char pad_0[0x8];
    int endpoint2; /* +0x8 */
    unsigned char pad_c[0x4];
} YasuiLimitClimhazzardEndpoints;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitClimhazzardSpriteDrawReq {
    int animBlock; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} YasuiLimitClimhazzardSpriteDrawReq;
#pragma pack(pop)

/* --- yasui_limit_catastrophe_beam --- */
typedef struct ActorStatusBits {
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short rest : 14;
} ActorStatusBits;

/* Line segment task slot for the Limit Catastro beam: start and end x/y/z of each beam point. */
typedef struct BattleFxLimitCatastropheSegmentTask {
    short ownerSlot;
    short frameIndex;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
} BattleFxLimitCatastropheSegmentTask;

/* Limit Catastro damped sprite particle: position, damped velocity and billboard sprite index. */
typedef struct BattleFxCatastropheParticle {
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
    short spriteIndex;
} BattleFxCatastropheParticle;

/* Effect100 slot record for a Catastro beam model, holding one endpoint position. */
typedef struct BattleFxCatastropheBeamSegment {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short beamZ;
    short offset;
    short endZ;
    unsigned char pad_12[0x2];
    short beamX;
} BattleFxCatastropheBeamSegment;

/* Catastro limit trail sprite: start position from the gun point and a polar offset. */
typedef struct BattleFxLimitCatastropheTrailSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short radialSin;
    short velocityY;
    short radialCos;
    unsigned char pad_12[0x2];
    short spriteScale;
} BattleFxLimitCatastropheTrailSprite;

/* Effect-100 slot for a Catastro rising sprite; frame index, vertical position and velocity. */
typedef struct BattleFxCatastropheRisingSprite {
    short ownerSlot; /* +0x0 */
    short frame;
    unsigned char pad_04[0x2];
    short posY;
    unsigned char pad_08[0x6];
    short velocityY;
    unsigned char pad_10[0x4];
    short scale;
} BattleFxCatastropheRisingSprite;

/* Main-slot Catastro billboard; frame counter at +2 and a billboard matrix at +4. */
typedef struct BattleFxLimitCatastropheBillboard {
    short ownerSlot;
    short frameCounter;
    short billboardMatrix;
} BattleFxLimitCatastropheBillboard;

/* Task slot for the Catastro spawn camera script: script address, matrix address and actor. */
typedef struct BattleFxCatastropheCameraSlot {
    unsigned char pad_00[0xc];
    int scriptAddr;
    int matrixAddr;
    short sourceActor;
} BattleFxCatastropheCameraSlot;

/* Ring buffer record for the Catastro timeline, with a target slot and a slot index. */
typedef struct BattleFxCatastropheTimelineRecord {
    short targetSlot;
    unsigned char pad_02[0xe];
    short slotIndex;
} BattleFxCatastropheTimelineRecord;

/* Model packet written at the Catastro line segment cursor with two command words. */
typedef struct BattleFxCatastropheLinePacket {
    int primitiveWord;
    int color;
} BattleFxCatastropheLinePacket;

/* Catastro target slot entry; the sorted target order maps to a slot. */
typedef struct YasuiLimitCatastropheSlot {
    short targetSlot;
    short slotIndex;
} YasuiLimitCatastropheSlot;

/* A world position stored as three shorts; `raw` copies it as one eight-byte pair. */
typedef union {
    Int64Pair raw;
    struct {
        short x;
        short y;
        short z;
        short pad;
    };
} YasuiCatastroPoint;

/* The scratch block of the growing black model: a rotation matrix followed by its scale vector. */
#pragma pack(push, 1)
typedef struct {
    PsxMatrix matrix;
    int scaleX;
    int scaleY;
    int scaleZ;
} YasuiCatastroGrowBlock;
#pragma pack(pop)

/* The first two shorts of the beam's homing endpoint; its Z is g_YasuiLimitCatastropheBeamEndpointZ. */
typedef struct {
    short x;
    short y;
} YasuiCatastroEndpoint;

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheCameraAngles {
    short cameraAngleY; /* +0x0 */
    short cameraAngle; /* +0x2 */
} YasuiLimitCatastropheCameraAngles;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheResourceHandle {
    struct GfxTableCursor *resourceHandle; /* +0x0 */
    unsigned char pad_4[0x4];
    union {
        int hookState; /* +0x8 */
        struct {
            short hookStateLow;
            short hookFlag; /* +0xa */
        };
    };
} YasuiLimitCatastropheResourceHandle;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheTargetActor {
    int actorIndex; /* +0x0 */
    unsigned char pad_4[0x2];
    short limitCatastroCameraOperandTable; /* +0x6 */
} YasuiLimitCatastropheTargetActor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheCenterPos {
    unsigned char pad_0[0x2];
    int transX; /* +0x2 */
    int transY; /* +0x6 */
    short limitCatastroCenterFloorY; /* +0xa */
} YasuiLimitCatastropheCenterPos;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheModelDrawReq1 {
    unsigned char pad_0[0xa];
    int limitCatastroModelDrawReq1Fraction; /* +0xa */
    unsigned char pad_e[0x2];
} YasuiLimitCatastropheModelDrawReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheModelDrawReq2 {
    int catastroModelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short limitCatastroModelDrawReq2Value; /* +0xa */
    unsigned char pad_c[0x4];
} YasuiLimitCatastropheModelDrawReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheScaleVector {
    int limitCatastroScaleVectorX; /* +0x0 */
    int limitCatastroScaleVectorY; /* +0x4 */
    unsigned char pad_8[0x4];
} YasuiLimitCatastropheScaleVector;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheSpriteDrawReq1 {
    int animTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
} YasuiLimitCatastropheSpriteDrawReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheSpriteDrawReq2 {
    int spriteTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
} YasuiLimitCatastropheSpriteDrawReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitCatastropheSpriteDrawReq3 {
    int animTable; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short limitCatastroSpriteDrawReq3Frame; /* +0xc */
} YasuiLimitCatastropheSpriteDrawReq3;
#pragma pack(pop)

/* --- yasui_limit_daichi_orbit_trails --- */
typedef struct EffectTrailPoint {
    short x;
    short y;
    short z;
    short pad;
} EffectTrailPoint;

/* Daichi trail ring buffer: the head index, the count of valid points and 21 points. */
typedef struct BattleFxPsxTrailRibbon {
    short headIndex;
    short count;
    EffectTrailPoint points[21];
} BattleFxPsxTrailRibbon;

/* Effect slot of the Daichi timeline: the target actor, the frame step and the target's bone world position. */
typedef struct YasuiBreathOfTheEarthTimeline {
    short actorIndex;
    short step;
    unsigned char pad_04[0x4];
    Int64Pair worldPos;
} YasuiBreathOfTheEarthTimeline;

/* Daichi limit orbit trail: circular orbit state and the vertex output buffer. */
typedef struct YasuiBreathOfTheEarthOrbit {
    short ownerSlot;
    short frameCount;
    BattleFxPsxTrailRibbon *vertexBuffer;
    short centerX;
    unsigned char pad_0A[0x2];
    short centerY;
    unsigned char pad_0E[0x2];
    short radius;
    short radiusSpeed;
    short angle;
} YasuiBreathOfTheEarthOrbit;

/* Render-state packet words for the Daichi limit trail ribbon. */
typedef struct BattleFxBreathOfTheEarthRibbonPacket {
    int tag;
    int primitiveWord;
    unsigned char pad_08[0x4];
    short color0;
    short clut;
    unsigned char pad_10[0x4];
    short color1;
    short tpage;
    unsigned char pad_18[0x4];
    short color2;
    unsigned char pad_1E[0x6];
    short color3;
} BattleFxBreathOfTheEarthRibbonPacket;

/* Effect-slot record for the Daichi limit orbit trail, with its trail buffer pointer. */
typedef struct BattleFxBreathOfTheEarthTrailRecord {
    unsigned char pad_00[0x4];
    BattleFxPsxTrailRibbon *trailBuffer;
    Int64Pair worldPos;
    short velocityX;
    short velocityY;
    short orbitAngle;
} BattleFxBreathOfTheEarthTrailRecord;

#pragma pack(push, 1)
typedef struct YasuiLimitBreathOfTheEarthSpriteRequest {
    int animBlock; /* +0x0 */
    int battleLimitDaichiSpriteResource; /* +0x4 */
    unsigned char pad_8[0x4];
    short battleLimitDaichiSpriteFrameIndex; /* +0xc */
    unsigned char pad_e[0x2];
} YasuiLimitBreathOfTheEarthSpriteRequest;
#pragma pack(pop)

/* --- yasui_limit_great_gospel --- */
/* Eva expanding-fade particle slot: frame index, and position and velocity per axis. */
typedef struct BattleFxGreatGospelParticle {
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
    short scale;
} BattleFxGreatGospelParticle;

/* Eva spiraling model: radius, angle and height offset that drive the target actor position. */
typedef struct BattleFxGreatGospelSpiral {
    short index;
    short timer;
    short radius;
    short offsetY;
    short velocityY;
    unsigned char pad_0A[0x2];
    short rotX;
    short angle;
    short rotZ;
    unsigned char pad_12[0x6];
    short angleVelocity;
} BattleFxGreatGospelSpiral;

/* The task slot BattleFx_GreatGospelTimelineTask fills for the child it spawns: a spiraling model or an expanding fade model. */
typedef union BattleFxEvaTimeline {
    BattleFxGreatGospelSpiral spiral;
    BattleFxGreatGospelParticle particle;
} BattleFxEvaTimeline;

/* Falling Eva sprite particle record: palette colour index, frame counter, position and scale. */
typedef struct BattleFxGreatGospelSpriteParticle {
    short colorIndex;
    short frameIndex;
    short posX;
    short posY;
    unsigned char pad_08[0xc];
    short scale;
} BattleFxGreatGospelSpriteParticle;

/* Falling sprite emitter record for the Eva effect, with a base position and frame counter. */
typedef struct BattleFxGreatGospelFallingSpriteEmitter {
    short actorIndex;
    short frame;
    short posX;
    short posY;
    short posZ;
} BattleFxGreatGospelFallingSpriteEmitter;

/* Emitter record that seeds an Eva growing-fade model task from an angle. */
typedef struct BattleFxGreatGospelFadeEmitter {
    unsigned char pad_00[0x4];
    short offsetSin;
    short offsetY;
    short offsetCos;
    unsigned char pad_0A[0x2];
    short angleX;
    short angle;
    short angleZ;
} BattleFxGreatGospelFadeEmitter;

/* Effect-100 slot for the Eva growing-fade model task; frame counter and copied camera offsets. */
typedef struct BattleFxGreatGospelFadeSlot {
    short ownerSlot;
    short frame;
    short offsetSin;
    short offsetY;
    short offsetCos;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
} BattleFxGreatGospelFadeSlot;

/* Emitter record that seeds the wide Eva growing-fade model task from the camera target Z. */
typedef struct BattleFxGreatGospelWideFadeEmitter {
    unsigned char pad_00[0x4];
    short offsetSin;
    short offsetY;
    short offsetCos;
    unsigned char pad_0A[0x2];
    short angleX;
    short targetZ;
    short angleZ;
} BattleFxGreatGospelWideFadeEmitter;

/* Falling sprite of the Eva effect, placed near an actor's world position with random spread. */
typedef struct BattleFxGreatGospelFallingSpriteParticle {
    short colorIndex;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short scale;
} BattleFxGreatGospelFallingSpriteParticle;

/* Shared position and depth block for the EVA falling streak task. */
typedef struct BattleFxGreatGospelStreakState {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    int depth;
    unsigned char pad_0C[0x4];
    int flags;
} BattleFxGreatGospelStreakState;

/* A model draw record BattleFx_SubmitModelDraw takes: the model resource, flags and a scale. */
typedef struct YasuiLimitGreatGospelModelDrawRecord {
    int modelResource;
    unsigned char pad_04[0x4];
    short scale;
    unsigned char pad_0A[0x6];
} YasuiLimitGreatGospelModelDrawRecord;

/* The stepped and the spinning model's draw record, each followed by its matrix. */
typedef struct YasuiLimitGreatGospelModelDrawData {
    unsigned char pad_00[0x20];
    YasuiLimitGreatGospelModelDrawRecord stepped;
    PsxMatrix steppedMatrix;
    unsigned char pad_4E[0x2];
    YasuiLimitGreatGospelModelDrawRecord spinning;
    PsxMatrix spinningMatrix;
    unsigned char pad_7E[0x2];
} YasuiLimitGreatGospelModelDrawData;

/* Eva limit particle record, 0x14 bytes; state == -1 marks a free entry. */
typedef struct YasuiLimitGreatGospelParticle {
    short state;
    short frameIndex;
    unsigned char pad_04[0x10];
} YasuiLimitGreatGospelParticle;

/* The scratch block Gfx_GetPsxScratchpadPtr(0) hands the model tasks: a scale vector, then the model's matrix. */
typedef struct YasuiLimitGreatGospelScratchBlock {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix;
} YasuiLimitGreatGospelScratchBlock;

#pragma pack(push, 1)
typedef struct YasuiLimitGreatGospelCameraData {
    YasuiLimitGreatGospelScratchBlock *fadeState; /* +0x0 */
    BattleFxGreatGospelStreakState *streakState; /* +0x4 */
    unsigned char pad_8[0x6];
    short limitEvaCameraOperandWord; /* +0xe */
} YasuiLimitGreatGospelCameraData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGreatGospelBillboardTransform {
    unsigned char pad_0[0x12];
    int limitEvaBillboardParticleWordA; /* +0x12 */
    unsigned char pad_16[0x4];
    int limitEvaBillboardParticleWordB; /* +0x1a */
    unsigned char pad_1e[0x2];
} YasuiLimitGreatGospelBillboardTransform;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGreatGospelModelHeader1 {
    int growFadeModelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short limitEvaGrowFadeModelAlpha; /* +0xa */
    unsigned char pad_c[0x4];
} YasuiLimitGreatGospelModelHeader1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGreatGospelModelHeader2 {
    int expandFadeModelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short limitEvaExpandFadeModelAlpha; /* +0xa */
    unsigned char pad_c[0x4];
} YasuiLimitGreatGospelModelHeader2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGreatGospelModelHeader3 {
    int wideGrowFadeModelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short limitEvaWideGrowFadeModelAlpha; /* +0xa */
    unsigned char pad_c[0x4];
} YasuiLimitGreatGospelModelHeader3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGreatGospelSpriteRequest {
    int animBlock; /* +0x0 */
    int limitEvaSpriteResource; /* +0x4 */
    unsigned char pad_8[0x4];
    short limitEvaSpriteFrameIndex; /* +0xc */
    unsigned char pad_e[0x2];
} YasuiLimitGreatGospelSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGreatGospelSpriteRequestStreak {
    int animBlock; /* +0x0 */
    int limitEvaFallingSpriteResource; /* +0x4 */
    int limitEvaFallingSpriteColor; /* +0x8 */
    short limitEvaFallingSpriteFrameIndex; /* +0xc */
    unsigned char pad_e[0x2];
} YasuiLimitGreatGospelSpriteRequestStreak;
#pragma pack(pop)

/* --- yasui_limit_grenade_bomb --- */
/* Battle effect slot for the Glenade tumbling sprite: position, velocity, angles and spin. */
typedef struct BattleFxGrenadeBombSprite {
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
    unsigned char angleX;
    unsigned char angleY;
    unsigned char angleZ;
    unsigned char pad_17[0x1];
    char spinX;
    char spinY;
    char spinZ;
    unsigned char pad_1B[0x1];
    short nextValue;
} BattleFxGrenadeBombSprite;

/* Expanding radial sprite slot for the Limit Glenade effect. */
typedef struct BattleFxLimitGrenadeBombRadialSprite {
    short ownerSlot;
    short frameIndex;
    short baseX;
    short posY;
    short baseZ;
    unsigned char pad_0A[0x2];
    short expandX;
    unsigned char pad_0E[0x2];
    short expandZ;
    unsigned char pad_12[0xa];
    short angle;
} BattleFxLimitGrenadeBombRadialSprite;

/* Timeline slot of the Grenade Bomb effect: frame counter and the blast centre its child tasks copy. */
typedef struct BattleFxLimitGrenadeBombTimeline {
    short ownerSlot;
    short frameCounter;
    union {
        Int64Pair pos;
        struct { short posX; short posY; short posZ; unsigned char pad_0A[0x2]; };
    };
} BattleFxLimitGrenadeBombTimeline;

/* A child task slot the Grenade Bomb timeline starts: the blast centre copied from the timeline, then the
 * shell's rotation. */
typedef struct BattleFxLimitGrenadeBombChild {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short rotX;
    short rotY;
    short rotZ;
} BattleFxLimitGrenadeBombChild;

/* Effect-slot record for the Glenade expanding shell model. */
typedef struct BattleFxGrenadeBombShellRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short spin; /* the Y rotation */
    short rotZ;
} BattleFxGrenadeBombShellRecord;

/* Per-sprite record for the Glenade tumbling sprite emitter, with random spin values. */
typedef struct BattleFxGrenadeBombTumbleRecord {
    short spriteIndex;
    unsigned char pad_02[0x2];
    Int64Pair pos;
    short velocity[3];
    unsigned char pad_12[0x2];
    char angleX;
    char angleY;
    char angleZ;
    unsigned char pad_17[0x1];
    char spinX;
    char spinY;
    char spinZ;
    unsigned char pad_1B[0x1];
    short scale;
} BattleFxGrenadeBombTumbleRecord;

/* Scale history for the Glenade expanding shell, embedded at +0x1E in a parent record. */
typedef struct BattleFxGrenadeBombScaleDelay {
    int scale;
    int scaleDelayed;
    int scaleTarget;
} BattleFxGrenadeBombScaleDelay;

/* Glenade radial sprite emitter record with sine and cosine based offsets. */
typedef struct BattleFxGrenadeBombRadialSpriteTask {
    unsigned char pad_00[0x4];
    Int64Pair pos;
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_12[0xa];
    short scale;
} BattleFxGrenadeBombRadialSpriteTask;

/* Three-short tumbling sprite emitter parameters, randomised per sprite. */
typedef struct BattleFxTumblingParams {
    short angle;
    short spinSpeed;
    short tiltSpeed;
} BattleFxTumblingParams;

#pragma pack(push, 1)
typedef struct YasuiLimitGrenadeBombBlackBlastMatrixData {
    short scaleY; /* +0x0 */
    int limitGlenadeBlackBlastPosX; /* +0x2 */
    int limitGlenadeBlackBlastPosY; /* +0x6 */
    int limitGlenadeBlackBlastPosZ; /* +0xa */
} YasuiLimitGrenadeBombBlackBlastMatrixData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGrenadeBombExpandingScaleMatrix {
    unsigned char pad_0[0x10];
    int limitGlenadeExpandScaleResource; /* +0x10 */
    unsigned char pad_14[0x6];
    short limitGlenadeExpandScaleFactor; /* +0x1a */
} YasuiLimitGrenadeBombExpandingScaleMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleLimitGrenadeShellReq {
    int limitGlenadeShellDrawResource0; /* +0x0 */
    unsigned char pad_4[0x6];
    short limitGlenadeShellDrawWordA; /* +0xa */
    unsigned char pad_c[0x4];
    int limitGlenadeShellDrawResourceA; /* +0x10 */
    unsigned char pad_14[0x6];
    short limitGlenadeShellDrawWordB; /* +0x1a */
    unsigned char pad_1c[0x4];
    int limitGlenadeShellDrawResourceB; /* +0x20 */
    unsigned char pad_24[0x6];
    short limitGlenadeShellDrawWordC; /* +0x2a */
} BattleLimitGrenadeShellReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGrenadeBombGrowingScaleMatrix {
    short growScaleCopy; /* +0x0 */
    unsigned char pad_2[0x6];
    short limitGlenadeGrowScaleCurrent; /* +0x8 */
    unsigned char pad_a[0x6];
    short limitGlenadeGrowScaleProgress; /* +0x10 */
    int limitGlenadeGrowPosX; /* +0x12 */
    int limitGlenadeGrowPosY; /* +0x16 */
    int limitGlenadeGrowPosZ; /* +0x1a */
    unsigned char pad_1e[0x2];
    int limitGlenadeTumbleSpriteResource; /* +0x20 */
    int limitGlenadeTumbleSpriteResourceB; /* +0x24 */
} YasuiLimitGrenadeBombGrowingScaleMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitGrenadeBombSpriteDrawReqPool {
    union {
        int colorWord;
        struct { char colorR; char colorG; char colorB; char command; };
    };
    short limitGlenadeTumbleRecordWord; /* +0x4 */
    unsigned char pad_6[0x2];
    int limitGlenadeRadialSpriteResource; /* +0x8 */
    int limitGlenadeRadialSpriteResourceB; /* +0xc */
    unsigned char pad_10[0x4];
    short limitGlenadeRadialSpriteFrameIndex; /* +0x14 */
    unsigned char pad_16[0x2];
    int limitGlenadeBlastModelResource; /* +0x18 */
    unsigned char pad_1c[0x6];
    short limitGlenadeBlastModelScale; /* +0x22 */
} YasuiLimitGrenadeBombSpriteDrawReqPool;
#pragma pack(pop)

/* --- yasui_limit_hakou_effect --- */
typedef struct ColorLoopLocals {
    int index;
    char color[3];
    char pad;
    BattleTaskSlot *record;
} ColorLoopLocals;

#pragma pack(push, 2)
typedef struct BladeBeamEffectRecord {
    short active;
    short frame;
    int pad_04;
    short x;
    short y;
    short z;
    char pad_0e[2];
    short width;
    short stepY;
    short height;
    char pad_16[2];
    struct PsxMatrix *matrix;
} BladeBeamEffectRecord;
#pragma pack(pop)

/* Effect slot record for a Hakou traveling beam: variant, scale, position and per-frame velocity. */
typedef struct BattleFxBladeBeamBeamTravel {
    short variant;
    short frameCounter;
    unsigned char pad_04[0x2];
    short scale;
    short posX;
    short endY;
    short posY;
    unsigned char pad_0E[0x2];
    short velocityX;
    short dirY;
    short velocityY;
    unsigned char pad_16[0x2];
    int transform;
} BattleFxBladeBeamBeamTravel;

/* Hakou mirrored sprite pair: frame counter and centre X mirrored about a target. */
typedef struct BattleFxLimitBladeBeamSpritePair {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x2];
    short nextValue;
    short centerX;
    short centerY;
    short spriteOffsetX;
} BattleFxLimitBladeBeamSpritePair;

/* Render state block for the Hakou limit trail particle: tag word and RGB colour bytes. */
typedef struct BattleFxBladeBeamRenderBlock {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char flags;
    unsigned char pad_08[0x4];
    char color1R;
    char color1G;
    char color1B;
} BattleFxBladeBeamRenderBlock;

/* Effect-slot record for the Hakou oriented sprite, with a position and a matrix. */
typedef struct BattleFxBladeBeamSpriteRecord {
    short frameIndex;
    short frame;
    unsigned char pad_04[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0xa];
    int matrix;
} BattleFxBladeBeamSpriteRecord;

/* Mirrored sprite pair record for the Hakou effect, with per-axis offsets. */
typedef struct BattleFxBladeBeamMirroredSpritePair {
    short offsetX;
    short clearedMid;
    short offsetY;
} BattleFxBladeBeamMirroredSpritePair;

/* Effect-60 slot for the Hakou spawn timeline; timer and the actor index it pans to. */
typedef struct BattleFxLimitBladeBeamSpawnSlot {
    short ownerSlot;
    short timer;
    char firstTargetActor;
    char actorIndex;
} BattleFxLimitBladeBeamSpawnSlot;

/* Secondary task slot for Hakou, with the source actor for the effect. */
typedef struct BattleFxBladeBeamSlot {
    unsigned char pad_00[0x4];
    char firstTargetActor;
    char sourceActor;
} BattleFxBladeBeamSlot;

/* Secondary task slot for the Hakou timeline, copying the source actor index. */
typedef struct BattleFxBladeBeamTimelineSlot {
    unsigned char pad_00[0x4];
    char firstTargetActor;
    char actorIndex;
} BattleFxBladeBeamTimelineSlot;

/* Data block whose X and Y values, shifted right by 6, become sprite pair offsets. */
typedef struct BattleFxBladeBeamSpriteOffsets {
    unsigned char pad_00[0xc];
    short posX;
    unsigned char pad_0E[0x2];
    short posY;
} BattleFxBladeBeamSpriteOffsets;

/* Secondary task slot for the alternate Hakou per-target effect, with a target mask and actor. */
typedef struct BattleFxBladeBeamAltSlot {
    unsigned char pad_00[0x4];
    char targetMask;
    char firstTargetActor;
} BattleFxBladeBeamAltSlot;

/* Hakou limit timeline: stage counter and two actor slots. */
typedef struct BattleFxBladeBeamTimeline {
    short ownerSlot;
    short stage;
    char firstTargetActor;
    char sourceActor;
} BattleFxBladeBeamTimeline;

/* Bone-position record for the Limit Hakou timeline, stepped toward the target actor position. */
typedef struct BattleFxLimitBladeBeamBone {
    unsigned char pad_00[0x8];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
    int matrixCursor;
} BattleFxLimitBladeBeamBone;

/* Per-task timeline state of the Hakou alternate limit effect, reached through the task slot pointer in the task's frame. */
typedef struct BattleFxBladeBeamAltTimelineState {
    short ownerSlot;
    short step;
    char actorIndex;
} BattleFxBladeBeamAltTimelineState;

/* Record of the Hakou limit timeline with a random value set on each step. */
typedef struct BattleFxBladeBeamRecord {
    short x;
    short y;
    short lifetime;
} BattleFxBladeBeamRecord;

/* Particle record of the Hakou limit timeline with random initial values. */
typedef struct BattleFxBladeBeamSpawnRotation {
    short rotX;
    short rotY;
    short rotZ;
} BattleFxBladeBeamSpawnRotation;

#pragma pack(push, 1)
typedef struct YasuiLimitBladeBeamTargetBonePos {
    short posX; /* +0x0 */
    short limitHakouTargetBonePosY; /* +0x2 */
} YasuiLimitBladeBeamTargetBonePos;
#pragma pack(pop)

/* --- yasui_limit_hammer_blow --- */
/* Hamblow tumbling target: timer, position, velocity and origin offset per actor in the tumble table. */
typedef struct BattleFxHammerblowTumble {
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
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_1A[0x2];
    short startY;
} BattleFxHammerblowTumble;

/* Hamblow limit sprite animation record: frame counter, copied draw words and a Z rotation. */
typedef struct BattleFxLimitHammerblowSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short valueOld;
    unsigned char pad_0E[0x2];
    short angleZ;
    unsigned char pad_12[0xa];
    short valueNew;
} BattleFxLimitHammerblowSprite;

/* Effect-slot record driving the growing black model of the Hamblow limit. */
typedef struct BattleFxHammerblowGrowingRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short angle;
} BattleFxHammerblowGrowingRecord;

/* Growing model record spawned by the Hamblow effect, with y and z offsets. */
typedef struct BattleFxHammerblowGrowingModel {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_12[0xa];
    short scale;
} BattleFxHammerblowGrowingModel;

/* Radial sprite emitter record for the Hamblow effect, placed on a circle by angle. */
typedef struct BattleFxHammerblowRadialSprite {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0x2];
    short rotation;
    short clearedRotY;
    short angle;
    unsigned char pad_12[0xa];
    short scale;
} BattleFxHammerblowRadialSprite;

/* Hamblow growing-model record: index of the model being spawned and a frame counter. */
typedef struct BattleFxHammerblowRecord {
    short index;
    short frameCount;
} BattleFxHammerblowRecord;

/* Hamblow limit timeline task in g_BattleTaskSlotBuffer, with velocities and offsets taken from actor yaw and position. */
typedef struct BattleFxHammerblowTimeline {
    short ownerSlot; /* +0x0 */
    unsigned char pad_2[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_1A[0x2];
    short scale;
} BattleFxHammerblowTimeline;

/* Scratch record holding the previous tumbling actor offsets for the Hamblow limit. */
typedef struct BattleHammerblowTumbleScratch {
    short prevOffsetX;
    short prevOffsetY;
    short prevOffsetZ;
    unsigned char pad_06[0x2];
    PsxMatrix matrix;
} BattleHammerblowTumbleScratch;

/* Scratchpad block of a Hamblow model task: the three scale factors, then the model's matrix. */
typedef struct BattleHammerblowModelScratch {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix;
} BattleHammerblowModelScratch;

/* Limit hamblow sprite animation state: value history and sampled values. */
typedef struct BattleFxLimitHammerblowSpriteState {
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_06[0x2];
    int valueOld;
    int valuePrev;
    int valueNew;
    unsigned char pad_14[0x4];
    PsxMatrix matrix;
} BattleFxLimitHammerblowSpriteState;

#pragma pack(push, 1)
typedef struct YasuiLimitHammerblowModelDrawReq0 {
    int hamblowModelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} YasuiLimitHammerblowModelDrawReq0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitHammerblowModelDrawReq1 {
    int hamblowEasedModelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} YasuiLimitHammerblowModelDrawReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitHammerblowSpriteDrawReq {
    int animBlock; /* +0x0 */
    int object; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
} YasuiLimitHammerblowSpriteDrawReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitHammerblowTumbleMatrix {
    short scaleY; /* +0x0 */
    int posX; /* +0x2 */
    int posY; /* +0x6 */
    int posZ; /* +0xa */
} YasuiLimitHammerblowTumbleMatrix;
#pragma pack(pop)

/* --- yasui_limit_jaki_effect --- */
/* Limit Jaki particle burst record: variant, position, negated velocity, matrix pointer and lifetime. */
typedef struct BattleFxSealEvilBurstParticle {
    short variant;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    int matrix;
    short lifetime;
} BattleFxSealEvilBurstParticle;

/* Jaki limit particle record, 0x1C bytes; state == -1 marks a free entry. */
typedef struct YasuiLimitSealEvilParticle {
    short state;
    short frameCount;
    unsigned char pad_04[0x18];
} YasuiLimitSealEvilParticle;

/* Oriented sprite particle with per-axis velocity, a matrix and a next value. */
typedef struct BattleFxSealEvilSpriteParticle {
    unsigned char pad_00[0xc];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    int matrix;
    short nextValue;
} BattleFxSealEvilSpriteParticle;

#pragma pack(push, 1)
typedef struct YasuiLimitSealEvilSpriteRequest1 {
    int spriteResource; /* +0x0 */
    int limitJakiBillboardResource; /* +0x4 */
    char scaleSnapshot; /* +0x8 */
    char limitJakiBillboardScaleCopy; /* +0x9 */
    char limitJakiBillboardScale; /* +0xa */
    unsigned char pad_b[0x1];
    short limitJakiBillboardParticleWord; /* +0xc */
    unsigned char pad_e[0x2];
} YasuiLimitSealEvilSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSealEvilSpriteRequest2 {
    int animBlock; /* +0x0 */
    int limitJakiOrientedResource; /* +0x4 */
    char limitJakiOrientedScaleOut; /* +0x8 */
    char limitJakiOrientedScaleCopy; /* +0x9 */
    char limitJakiOrientedScale; /* +0xa */
    unsigned char pad_b[0x5];
} YasuiLimitSealEvilSpriteRequest2;
#pragma pack(pop)

/* --- yasui_limit_night_effect --- */
/* Limit night timeline effect record, seeded with random jitter and a fixed-point angle. */
typedef struct BattleFxLimitNightmareParticle {
    unsigned char pad_00[0x4];
    short velocityX;
    short velocityY;
    short velocityZ;
    short startDelay;
    short phase;
    short speed;
    short scale;
    short scaleStep;
    short angle;
    short scaleStepY;
    short uniformScale;
    short stepOrTexRow;
    short lifetime;
    short timer;
} BattleFxLimitNightmareParticle;

/* Battle effect slot for the Yasui limit night trail: step deltas, timers and ring-buffer index. */
typedef struct BattleFxLimitNightmareTrail {
    short ownerSlot;
    short stage;
    short stepX;
    short stepY;
    short stepZ;
    short limit;
    short trailY;
    short trailYStep;
    short orbitRadius;
    unsigned char pad_12[0x6];
    short uniformScale;
    short accel;
    short ringIndex;
    short timer;
} BattleFxLimitNightmareTrail;

/* BattleFx effect slot (0x20 bytes) for the Limit night timeline, with a phase counter. */
typedef struct BattleFxNightmareSlot {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short elapsed;
} BattleFxNightmareSlot;

/* Animated sprite record for the Limit night effect, with a frame counter and billboard angle. */
typedef struct BattleFxLimitNightmareSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short angle;
    short textureRow;
} BattleFxLimitNightmareSprite;

/* Night UV quad: four offsets to fragment records, and four byte fields zeroed on assignment. */
typedef struct BattleFxNightmareUvQuadOffsets {
    unsigned short fragment0Offset;
    unsigned short fragment1Offset;
    unsigned short fragment2Offset;
    unsigned short fragment3Offset;
    char u0;                   /* +0x08: the quad's texture coordinates */
    char v0;
    unsigned char pad_0A[0x2];
    char u1;                   /* +0x0C */
    char v1;
    char u2;
    char v2;
    char u3;
    char v3;
    unsigned char pad_12[0x2];
} BattleFxNightmareUvQuadOffsets;

/* Draw state of the night weighted Gouraud mesh, the block Gfx_GetPsxScratchpadPtr(0) returns: the mesh data, the
   ordering table shift, the primitive cursor and the colours the quad shading uses. */
typedef struct BattleFxNightmareQuadState {
    int dataPtr;
    int drawList;
    int otShift;
    int quadPtr; /* starts at the render state block, then follows the primitives written */
    int highWordMask;
    int frontFacing;
    unsigned char pad_18[0x4];
    int orderingDepth;
    unsigned char pad_20[0x40];
    Int64Pair vertex[4]; /* +0x60: the face's corners, copied from the mesh */
    unsigned char pad_80[0x50];
    CVECTOR shadeTemp;
    unsigned char pad_D4[0x4];
    CVECTOR baseColor; /* the gradient's first colour (black for the quad shading) */
    CVECTOR tintColor; /* the gradient's second colour */
} BattleFxNightmareQuadState;

/* One entry of the angle-sorted quad tables: the quad's angle, its index and its hemisphere; the whole
   word is swapped when the tables are sorted. */
typedef union YasuiNightQuadSortEntry {
    int word;
    struct {
        short angle;
        unsigned char quadIndex;
        unsigned char hemisphere;
    } f;
} YasuiNightQuadSortEntry;

/* Night spinning gradient record: elapsed time, duration, angle and spin state. */
typedef struct BattleFxNightmareSpinGradient {
    short ownerSlot;
    short elapsed;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angle;
    short angleStep;
    short tiltX;
    unsigned char pad_12[0x2];
    short tiltZ;
    unsigned char pad_16[0x2];
    short spinAngle;
    short spinSpeed;
    short duration;
} BattleFxNightmareSpinGradient;

/* State record of the night expanding model task slot. */
typedef struct BattleFxNightmareExpandState {
    short ownerSlot;
    short timer;
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
} BattleFxNightmareExpandState;

/* Yasui night expanding model task state, with a mesh header pointer. */
typedef struct YasuiLimitNightmareMeshState {
    int meshHeader;
    int flags;
    short uvScroll;
    short expandRemaining;
    short attrMask;
    short pad_0E;
} YasuiLimitNightmareMeshState;

/* PSX quad primitive written by the Limit Night draw from a render block; tag word 0x09000000. */
typedef struct BattleFxLimitNightmareQuadPrim {
    int command;
    int color;
    unsigned char pad_08[0x4];
    int primWord;
    unsigned char pad_10[0x4];
    short uv1;
    short bits;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short quadVertexOffset;
} BattleFxLimitNightmareQuadPrim;

/* Limit Night PSX mesh packet: tag word and words copied from the data pointer. */
typedef struct BattleFxLimitNightmarePsxPacket {
    int tag;
    int primWord;
    unsigned char pad_08[0x4];
    int colorCmd;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxLimitNightmarePsxPacket;

/* Weighted Gouraud quad primitive cleared by the Limit night draw. */
typedef struct BattleFxLimitWeightedQuad {
    int packetTag; /* +0x0 */
    int color0;
    unsigned char pad_08[0x4];
    int color1;
    unsigned char pad_10[0x4];
    int color2;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxLimitWeightedQuad;

/* Limit Night mesh stream record: vertex offsets, a colour byte and a primitive word. */
typedef struct YasuiLimitNightmareMeshStream {
    short vertexOffsetA;
    short vertexOffsetB;
    short vertexOffsetC;
    short resourceOffset;
    int primWord;
    union {
        struct {
            short uv1;
            short uv2;
        };
        int colorCmd; /* a triangle record's colour words, copied into the packet */
    };
    union {
        struct {
            short quadVertexOffset;
            short vertBWord;
        };
        int color2;
    };
    union {
        struct {
            short vertexOffsetD;
            short vertexOffsetE;
        };
        int color3;
    };
    short vertexOffsetF;
    short vertBOffset;
    short vertCOffset;
} YasuiLimitNightmareMeshStream;

/* Night weighted Gouraud quad record: four vertex indices and four texture slot values. */
typedef struct BattleFxNightmareQuad {
    unsigned short vertexIndex[4];
    int tex0;
    int tex1;
    int tex2;
    int tex3;
} BattleFxNightmareQuad;

/* --- yasui_limit_omnislash_effect --- */
/* Limit Busin damped sprite particle: position, damped velocity and billboard sprite index. */
typedef struct BattleFxOmnislashParticle {
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
    short angle;
    unsigned char pad_16[0x2];
    short spriteIndex;
    unsigned char pad_1A[0x6];
} BattleFxOmnislashParticle;

/* Limit Busin timeline: start and target values for three ramped parameters per source actor. */
typedef struct BattleFxLimitOmnislashRamp {
    unsigned char pad_00[0x4];
    short rampFromA;
    short rampFromB;
    short rampFromC;
    unsigned char pad_0A[0x2];
    short rampToA;
    short rampToB;
    short rampToC;
} BattleFxLimitOmnislashRamp;

/* Effect record of the Limit Busin growing billboard model task. */
typedef struct BattleFxOmnislashBillboardRecord {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x14];
    short angle;
    short spin;
} BattleFxOmnislashBillboardRecord;

/* Effect-slot record for the Busin bone sprite emitter, positioned on a bone. */
typedef struct BattleFxOmnislashBoneEmitterRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x10];
    short endFrame;
    unsigned char pad_16[0x2];
    short spriteScale;
    unsigned char pad_1A[0x2];
    short boneIndex;
} BattleFxOmnislashBoneEmitterRecord;

/* Damped sprite particle for the Limit Busin effect, launched from an origin with a velocity. */
typedef struct BattleFxOmnislashDampedSprite {
    unsigned char pad_00[0x4];
    short originX;
    short originY;
    short originZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short speed;
    short velocityZ;
    unsigned char pad_12[0x6];
    short lifetime;
} BattleFxOmnislashDampedSprite;

/* Timeline state for the Busin limit-break effect: phase, target Z, and per-tick position deltas. */
typedef struct BattleFxOmnislashTimeline {
    short ownerSlot;
    short state;
    unsigned char pad_04[0x4];
    short targetPosZ;
    unsigned char pad_0A[0x2];
    short deltaPosX;
    unsigned char pad_0E[0x2];
    short deltaPosZ;
} BattleFxOmnislashTimeline;

/* Busin slash record: world and local position, motion id, animation value and target actor. */
typedef struct BattleFxOmnislashSlashEntry {
    unsigned char pad_00[0x4];
    short worldX;
    short velocityY;
    short worldZ;
    unsigned char pad_0A[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_12[0x2];
    short motionId;
    unsigned char pad_16[0x2];
    short animValue;
    short rampValue;
    short targetActor;
} BattleFxOmnislashSlashEntry;

/* Textured cylinder primitive: command and colour words and its cylinder parameters. */
typedef struct BattleFxTexturedCylinder {
    int tag;
    int colorWord;
    unsigned char pad_08[0x4];
    char u0;
    char v0;
    short clut;
    unsigned char pad_10[0x4];
    char stepCount;
    char v1;
    short tpage;
    unsigned char pad_18[0x4];
    char scaleStep;
    char v2;
    unsigned char pad_1E[0x6];
    char u3;
    char v3;
} BattleFxTexturedCylinder;

/* Pulse state of the Limit Busin model: phase counter, position and velocity, amplitude. */
typedef struct BattleFxLimitOmnislashPulse {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short amplitude;
} BattleFxLimitOmnislashPulse;

/* Limit Busin scale entry 0 state: timer, base scale and percent scale moving toward 100. */
typedef struct YasuiLimitOmnislashScaleEntry0 {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x2];
    short scaleDivisor;
    unsigned char pad_16[0x2];
    short baseScale;
    short scalePercent;
} YasuiLimitOmnislashScaleEntry0;

/* State of the growing bone-model emitter task: a timer and an angle used for the billboard matrix. */
typedef struct BattleFxGrowingEmitterState {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x14];
    short angle;
} BattleFxGrowingEmitterState;

/* State of the shrinking bone-model emitter task: a timer and a scale used for the billboard matrix. */
typedef struct BattleFxShrinkingEmitterState {
    short ownerSlot;
    short timer;
    unsigned char pad_04[0x14];
    short scale;
} BattleFxShrinkingEmitterState;

/* Per-model record for the spinning, pulsing Limit Busin model task. */
typedef struct BattleFxLimitOmnislashSpinRecord {
    short ownerSlot;
    short angle;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scaleAmplitude;
} BattleFxLimitOmnislashSpinRecord;

/* Sine-scaled model emitter record of the Limit Busin effect: angle counter and position. */
typedef struct BattleFxSineScaleRecord {
    short ownerSlot;
    short angle;
    short posX;
    short posY;
    short posZ;
} BattleFxSineScaleRecord;

/* Per-task state of the Busin sine-scaled rotating model: phase counter, position, yaw and scale. */
typedef struct BattleFxLimitOmnislashRotationTask {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short yaw;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxLimitOmnislashRotationTask;

/* Growing bone-model emitter record with a copied position triple and random per-emitter values. */
typedef struct BattleFxLimitOmnislashBoneEmitter2 {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angle;
    unsigned char pad_16[0x2];
    short scale;
} BattleFxLimitOmnislashBoneEmitter2;

/* Sine-scaled model state: a sentinel short, a step counter driving the sine scale, and a pitch value. */
typedef struct BattleFxLimitOmnislashSineModel {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short modelPitch;
} BattleFxLimitOmnislashSineModel;

/* Sine-scaled model record that the shrinking bone-model emitter spawns at the bone position. */
typedef struct BattleFxOmnislashShrinkRecord {
    unsigned char pad_00[0x4];
    short anchorX;
    short anchorY;
    short anchorZ;
    unsigned char pad_0A[0xa];
    short randomOffset;
} BattleFxOmnislashShrinkRecord;

/* Ring-buffer record the timeline allocates, holding the Busin target actor and slash index. */
typedef struct BattleFxLimitOmnislashState {
    short targetActor;
    unsigned char pad_02[0xe];
    short slashIndex;
} BattleFxLimitOmnislashState;

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashEulerAngles {
    unsigned char pad_0[0x2];
    short limitBusinEulerAnglesY; /* +0x2 */
    unsigned char pad_4[0x4];
    short spinAngleX; /* +0x8: the spin task's rotation vector starts here */
    short spinAngleY; /* +0xA */
} YasuiLimitOmnislashEulerAngles;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashCylinderData {
    int spinStep; /* +0x0 */
    unsigned char pad_4[0x4];
    PsxMatrix rotMatrix; /* +0x8 */
} YasuiLimitOmnislashCylinderData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashDrawReq1 {
    int modelRes; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
} YasuiLimitOmnislashDrawReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashDrawReq2 {
    int modelRes; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
} YasuiLimitOmnislashDrawReq2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashDrawReq3 {
    int modelRes; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} YasuiLimitOmnislashDrawReq3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashDrawReqPool1 {
    int modelRes; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha0; /* +0xa */
    unsigned char pad_c[0x4];
    int modelRes1; /* +0x10 */
    unsigned char pad_14[0x6];
    short alpha1; /* +0x1a */
    unsigned char pad_1c[0x4];
    int modelRes2; /* +0x20 */
    unsigned char pad_24[0xc];
    int modelRes3; /* +0x30 */
    unsigned char pad_34[0xc];
    int modelRes4; /* +0x40 */
    unsigned char pad_44[0x6];
    short alpha4; /* +0x4a */
    unsigned char pad_4c[0x4];
    int modelRes5; /* +0x50 */
    unsigned char pad_54[0xc];
    int spriteRecord; /* +0x60 */
    int spriteObject; /* +0x64 */
} YasuiLimitOmnislashDrawReqPool1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashDrawReqPool2 {
    int modelRes; /* +0x0 */
    unsigned char pad_4[0xc];
    int ringModelRes; /* +0x10 */
    unsigned char pad_14[0x6];
    short ringScale; /* +0x1a */
    unsigned char pad_1c[0x4];
    int sparkSpriteRecord; /* +0x20 */
    int sparkSpriteObject; /* +0x24 */
    unsigned char pad_28[0x4];
    short limitBusinDrawReqPool2Frame; /* +0x2c */
    unsigned char pad_2e[0x2];
    int burstSpriteRecord; /* +0x30 */
    int burstSpriteObject; /* +0x34 */
    unsigned char pad_38[0x4];
    short burstFrame; /* +0x3c */
    unsigned char pad_3e[0x2];
} YasuiLimitOmnislashDrawReqPool2;
#pragma pack(pop)

/* One key of the slash colour table: the red, green and blue steps, each stored in a short. */
typedef struct YasuiLimitOmnislashFadeKey {
    char red;
    unsigned char pad_01[0x1];
    char green;
    unsigned char pad_03[0x1];
    char blue;
    unsigned char pad_05[0x1];
} YasuiLimitOmnislashFadeKey;

/* The slash colour table: one key per slash, 13 of them, then two unused bytes. */
typedef struct YasuiLimitOmnislashMotionTable9 {
    YasuiLimitOmnislashFadeKey keys[13];
    unsigned char pad_4E[0x2];
} YasuiLimitOmnislashMotionTable9;

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashRotationVector {
    unsigned char pad_0[0x2];
    short limitBusinRotationVectorY; /* +0x2 */
} YasuiLimitOmnislashRotationVector;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitOmnislashSpriteTint {
    char currentTintByte; /* +0x0 */
    char limitBusinSpriteTintByte1; /* +0x1 */
    char limitBusinSpriteTintByte2; /* +0x2 */
    unsigned char pad_3[0x1];
} YasuiLimitOmnislashSpriteTint;
#pragma pack(pop)

/* --- yasui_limit_planet_protector --- */
/* Effect-100 slot for the spiral sprite emitter: centre, height, radius, spin and rise speed. */
typedef struct BattleFxSpiralEmitter {
    short ownerSlot;
    short frameCount;
    short centerX;
    short posY;
    short centerZ;
    unsigned char pad_0A[0xa];
    short radius;
    short angle;
    short riseSpeed;
    short scale;
    short phase;
} BattleFxSpiralEmitter;

/* Damped sprite particle: delay, age, position and velocity per axis, angle. */
typedef struct BattleFxPlanetProtectorDampedSprite {
    short delay;
    short age;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x8];
    short nextValue;
    short angle;
} BattleFxPlanetProtectorDampedSprite;

/* Limit SGuard timeline record: record type, step and two positions. */
typedef struct BattleFxLimitPlanetProtectorTimelineRecord {
    short recordType;
    short step;
    short posA;
    unsigned char pad_06[0x2];
    short posB;
} BattleFxLimitPlanetProtectorTimelineRecord;

/* Particle slot of the Limit S-Guard timeline, randomised at spawn. */
typedef struct BattleFxPlanetProtectorParticle {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posA;
    short posY;
    short posB;
    unsigned char pad_0A[0xa];
    short startRadius;
    short startAngle;
    short riseSpeed;
    short radius;
    short phase;
} BattleFxPlanetProtectorParticle;

/* Guard spiral sprite particle: position computed on a circle around the spiral centre. */
typedef struct BattleFxPlanetProtectorSpiralParticle {
    short fadeLevel;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
    short phase;
} BattleFxPlanetProtectorSpiralParticle;

/* Fading sprite record of the S-Guard limit: fade level plus scale and colour words. */
typedef struct BattleFxLimitPlanetProtectorSprite {
    short fadeLevel;
    unsigned char pad_02[0xa];
    short baseLevel;
    short rotZ;
} BattleFxLimitPlanetProtectorSprite;

/* S-Guard particle pool entry; a free entry has state at or below zero. */
typedef struct YasuiLimitPlanetProtectorParticle {
    short state;
    unsigned char pad_02[0xe];
} YasuiLimitPlanetProtectorParticle;

/* Three-short S-Guard sprite parameters, randomised by the timeline task. */
typedef struct BattleFxPlanetProtectorParams {
    short offsetX;
    short velocityY;
    short offsetZ;
} BattleFxPlanetProtectorParams;

#pragma pack(push, 1)
typedef struct YasuiLimitPlanetProtectorSpriteDrawReq1 {
    int animBlock; /* +0x0 */
    int battleLimitSGuardSpriteResource; /* +0x4 */
    char colorRed; /* +0x8 */
    char battleLimitSGuardSpriteFadeCopy; /* +0x9 */
    char battleLimitSGuardSpriteFadeAlpha; /* +0xa */
    unsigned char pad_b[0x5];
} YasuiLimitPlanetProtectorSpriteDrawReq1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitPlanetProtectorSpriteDrawReq2 {
    int animBlock; /* +0x0 */
    int battleLimitSGuardDampedSpriteResource; /* +0x4 */
    int battleLimitSGuardDampedColorWord; /* +0x8 */
    unsigned char pad_c[0x4];
} YasuiLimitPlanetProtectorSpriteDrawReq2;
#pragma pack(pop)

/* --- yasui_limit_satan_stage_shatter --- */
/* Limit Satan stage mesh timeline slot: timer, stage state and index. */
typedef struct BattleFxLimitSatanSlamStageSlot {
    short ownerSlot;
    short timer;
    Int64Pair centroid;
    unsigned char pad_0C[0x8];
    short stage;
    short index;
} BattleFxLimitSatanSlamStageSlot;

/* Effect100 slot record for the Limit Satan trailed particle: frame, state, indices and z scale. */
typedef struct BattleFxSatanSlamTrailParticle {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    short scaleZ;
    short recordIndex;
    unsigned char pad_10[0x4];
    short state;
    short particleIndex;
} BattleFxSatanSlamTrailParticle;

/* Satan limit-break stage record; its shorts seed globals at setup. */
typedef struct BattleFxSatanSlamStageRecord {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short stageOffsetX;
    short posY;
    short stageOffsetY;
    unsigned char pad_0A[0xa];
    short stage;
    short startTimer;
} BattleFxSatanSlamStageRecord;

/* Four vertex-pair offsets and quad attributes for the Satan PSX textured-quad draw. */
typedef struct BattleFxSatanSlamQuadRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned short vertexOffset3;
    int color0;
    unsigned short flags;
    short color1;
    short color2;
    unsigned short level;
} BattleFxSatanSlamQuadRecord;

/* PSX textured triangle record for the Satan limit effect: vertex offsets and a texture index. */
typedef struct YasuiSatanSlamTriangleRecord {
    unsigned short vertexOffset0;
    unsigned short vertexOffset1;
    unsigned short vertexOffset2;
    unsigned short textureIndex;
    int color0;
    unsigned short primFlags;
    short color1;
} YasuiSatanSlamTriangleRecord;

/* Node of the Satan mesh surface grid: a link or tag at +0 and a vertex record at +4. */
typedef struct BattleFxSatanSlamMeshGridNode {
    int child;
    int vertexRecord;
    int isQuad;
} BattleFxSatanSlamMeshGridNode;

/* Task slot for the Satan spawn camera script: script address, state and mode. */
typedef struct BattleFxSatanSlamCameraSlot {
    unsigned char pad_00[0xc];
    int scriptAddr;
    int state;
    short mode;
} BattleFxSatanSlamCameraSlot;

/* Main task slot that retires when the Satan state flags match its mask. */
typedef struct BattleFxSatanSlamRetireTask {
    short ownerSlot;
    unsigned char pad_02[0x12];
    short stateMask;
} BattleFxSatanSlamRetireTask;

/* Limit Satan mesh draw state: mesh blob, draw list, camera and fragment matrices, vertex slots and the surface hit. */
#pragma pack(push, 1)
typedef struct BattleFxLimitSatanSlamMeshState {
    int meshBlob;
    int drawList;
    int otShift;
    int primCursor;
    int drawMode;
    int baseFlags;
    int translucency;
    unsigned char pad_1C[0x4];
    int orderingDepth;
    int flags;
    int shade;
    int fragmentIndex;
    PsxMatrix cameraMatrix;
    PsxMatrix fragmentMatrix;
    union {
        struct {
            short v0x;
            short v0y;
            short v0z;
            unsigned char pad_72[0x2];
        };
        SVECTOR v0; /* +0x6C: the polygon being drawn or hit-tested */
    };
    union {
        struct {
            short v1x;
            short v1y;
            short v1z;
            unsigned char pad_7A[0x2];
        };
        SVECTOR v1; /* +0x74 */
    };
    union {
        struct {
            short v2x;
            short v2y;
            short v2z;
            unsigned char pad_82[0x2];
        };
        SVECTOR v2; /* +0x7C */
    };
    union {
        struct {
            short v3x;
            short v3y;
            short v3z;
            unsigned char pad_8A[0x2];
        };
        SVECTOR v3; /* +0x84 */
    };
    union {
        struct {
            short deltaX;
            short deltaY;
            short deltaZ;
            unsigned char pad_92[0x2];
        };
        SVECTOR delta; /* +0x8C: the fragment position the vertices are taken relative to */
    };
    union {
        struct {
            short surfaceIndex;
            short height;
            short gridY;
            unsigned char pad_9A[0x2];
        };
        SVECTOR surface; /* +0x94: the point whose surface height is sought */
    };
    union {
        struct {
            short hitX;
            short hitY;
            short hitZ;
            unsigned char pad_A2[0x2];
        };
        SVECTOR hit; /* +0x9C: the first vertex of the polygon that was hit */
    };
    union {
        struct {
            int normalX;
            int normalY;
            int normalZ;
            unsigned char pad_B0[0x4];
        };
        VECTOR normal; /* +0xA4 */
    };
    union {
        struct {
            int edge1X;
            int edge1Y;
            int edge1Z;
            unsigned char pad_C0[0x4];
        };
        VECTOR edge1; /* +0xB4 */
    };
    union {
        struct {
            int edge2X;
            int edge2Y;
            int edge2Z;
            unsigned char pad_D0[0x4];
        };
        VECTOR edge2; /* +0xC4 */
    };
    int hitCount;
    int candidateCount;
} BattleFxLimitSatanSlamMeshState;
#pragma pack(pop)

/* One Limit Satan fragment record: position, drift, velocity, timer and fall offset. */
typedef struct YasuiLimitSatanSlamFragment {
    short posX;
    short posY;
    short posZ;
    short centroidY;
    short driftX;
    short driftY;
    short driftZ;
    short timer;
    short velocityX;
    short offsetY;
    short velocityZ;
    short velocityY;
} YasuiLimitSatanSlamFragment;

/* Timeline state for the Satan limit effect, reached through frame+32. */
typedef struct BattleFxLimitSatanSlamState {
    short ownerSlot;
    short timer;
    Int64Pair centroid;
    unsigned char pad_0C[0x8];
    short stage;
    short startTimer;
} BattleFxLimitSatanSlamState;

/* Burst-model record of the Limit Satan mirrored effect, pointed at by frame+36. */
typedef struct BattleFxMirroredBurstRecord {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x8];
    short position;
    short velocity;
} BattleFxMirroredBurstRecord;

/* Per-task state of the mirrored Limit Satan burst models: a table-derived value and zeroed counters. */
typedef struct BattleFxLimitSatanSlamBurstState {
    int tableField;
    int drawFlags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} BattleFxLimitSatanSlamBurstState;

/* Trailed particle spawn state for the Limit Satan task: a reset key and the current spawn kind. */
typedef struct BattleFxLimitSatanSlamTrailSpawn {
    short spawnKey;
    short spawnKind;
} BattleFxLimitSatanSlamTrailSpawn;

/* Satan limit cell-grid entry, 0xC bytes, filled with mesh triangle and quad indices. */
typedef struct YasuiLimitSatanSlamCellEntry {
    int cellHead;
    int primRef;
    int kind;
} YasuiLimitSatanSlamCellEntry;

/* An actor's position as the Satan limit saved it at setup: the first 8 bytes of its battle record. */
typedef struct BattleFxLimitSatanSlamSavedActorPos {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
} BattleFxLimitSatanSlamSavedActorPos;

/* Particle record for the Satan trailed-particle task: position, velocity, trail point and flags. */
#pragma pack(push, 1)
typedef struct YasuiLimitSatanSlamParticle {
    int flags;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short prevX;
    short prevY;
    short prevZ;
    unsigned char pad_12[0x2];
    union {
        struct {
            int velocityX;
            int velocityY;
            int velocityZ;
        };
        VECTOR velocity; /* +0x14: a unit direction (4096); its pad word is unused */
    };
    unsigned char pad_24[0x10];
    short trailX;
    short trailY;
    short trailZ;
    unsigned char pad_3A[0x2];
    union {
        struct {
            int deltaX;
            int deltaY;
            int deltaZ;
            unsigned char pad_48[0x4];
        };
        VECTOR delta; /* +0x3C: the normalized direction to the trail target */
    };
    int steerThetaLimit;
    int steerTheta;
    int steerThetaCopy;
    int distanceLimit;
    int steerDistance;
    int steerDistanceCopy;
    int arriveDistance;
    union {
        struct {
            int launchDirX;
            int launchDirY;
            int launchDirZ;
            unsigned char pad_74[0x4];
        };
        VECTOR launchDir; /* +0x68 */
    };
    PsxMatrix orientation;
    unsigned char pad_96[0x2];
} YasuiLimitSatanSlamParticle;
#pragma pack(pop)

/* One emission point of the Satan limit: when it fires and the vectors and limits its particle starts with. */
typedef struct YasuiLimitSatanSlamSpawnRecord {
    int spawnFrame;
    short position[4];
    short mirrorPosition[4];
    short targetPosition[4];
    int direction[3];
    unsigned char pad_28[0x4];
    int steerThetaLimit;
    int distanceLimit;
    int retargetThetaLimit;
} YasuiLimitSatanSlamSpawnRecord;

/* Output PSX textured quad packet of the Limit Satan quads. */
typedef struct BattleFxSatanSlamTexturedQuadPrim {
    int tag;
    unsigned char pad_04[0x3];
    char code;
    unsigned char pad_08[0x4];
    union {
        int color0; /* +0x0C: u0, v0 and the CLUT, copied from the quad record */
        struct {
            unsigned char u0;
            unsigned char v0;
        };
    };
    unsigned char pad_10[0x4];
    union {
        int flags; /* +0x14: u1, v1 and the texture page */
        struct {
            unsigned char u1;
            unsigned char v1;
        };
    };
    unsigned char pad_18[0x4];
    union {
        short color1; /* +0x1C: u2 and v2 */
        struct {
            unsigned char u2;
            unsigned char v2;
        };
    };
    unsigned char pad_1E[0x6];
    union {
        short color2; /* +0x24: u3 and v3 */
        struct {
            unsigned char u3;
            unsigned char v3;
        };
    };
} BattleFxSatanSlamTexturedQuadPrim;

/* GPU textured triangle packet built from a Satan triangle record. */
typedef struct YasuiSatanSlamTriPrim {
    int tag;
    unsigned char pad_04[0x3];
    char command;
    unsigned char pad_08[0x4];
    union {
        int color0; /* +0x0C: u0, v0 and the CLUT, copied from the triangle record */
        struct {
            unsigned char u0;
            unsigned char v0;
        };
    };
    unsigned char pad_10[0x4];
    union {
        int primFlags; /* +0x14: u1, v1 and the texture page */
        struct {
            unsigned char u1;
            unsigned char v1;
        };
    };
    unsigned char pad_18[0x4];
    union {
        short color1; /* +0x1C: u2 and v2 */
        struct {
            unsigned char u2;
            unsigned char v2;
        };
    };
} YasuiSatanSlamTriPrim;

/* g_YasuiLimitSatanSlamCellGridEntries: 1041 entries and the cellHead of a 0x412th, which
 * BattleFx_LimitSatanSlamBuildMeshCellGrid also clears (entries[0x411].cellHead). */
#pragma pack(push, 1)
typedef struct {
    YasuiLimitSatanSlamCellEntry entries[0x411];
    int lastCellHead; /* +0x30CC */
} YasuiLimitSatanCellGridEntries;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatanSlamCameraScriptShortTable {
    unsigned char pad_0[0x6];
    short battleLimitSatanCameraOperandTable; /* +0x6 */
} YasuiLimitSatanSlamCameraScriptShortTable;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatanSlamOffscreenViewportPos {
    short offscreenViewportX; /* +0x0 */
    short battleLimitSatanOffscreenViewportY; /* +0x2 */
} YasuiLimitSatanSlamOffscreenViewportPos;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatanSlamOffscreenViewportSize {
    short viewportWidth; /* +0x0 */
    short battleLimitSatanOffscreenViewportHeight; /* +0x2 */
} YasuiLimitSatanSlamOffscreenViewportSize;
#pragma pack(pop)

/* --- yasui_limit_satellite_beam --- */
/* Limit S-beam damped sprite particle: position, damped velocities and frame count. */
typedef struct BattleFxLimitSatelliteBeamParticle {
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
    short stepX;
    short stepY;
} BattleFxLimitSatelliteBeamParticle;

/* Beam crosshair line: current and target endpoints lerped per frame. */
typedef struct BattleFxBeamCrosshairLine {
    short ownerSlot;
    short age;
    short currentX;
    short currentY;
    short currentZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
} BattleFxBeamCrosshairLine;

/* Render-state block used to draw the S-beam crosshair line. */
typedef struct BattleFxSatelliteBeamLineState {
    int commandWord;
    int colorWord;
    short x;
    short y;
    short posX;
    short posY;
} BattleFxSatelliteBeamLineState;

/* S-Beam target emitter: mask bit cursor and frame counter for emission. */
typedef struct BattleFxLimitSatelliteBeamEmitter {
    short maskBit;
    short frameCounter;
} BattleFxLimitSatelliteBeamEmitter;

/* S-Beam target timeline: step counter and base value summed into an output. */
typedef struct BattleFxLimitSatelliteBeamTimeline {
    short value;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0a[0x2];
    short startU;
    short startV;
} BattleFxLimitSatelliteBeamTimeline;

/* Shared S-Beam impact effect slot: yaw is set from the beam yaw direction. */
typedef struct BattleFxLimitSatelliteBeamImpactSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0a[0x2];
    short yaw;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x4];
    short driftX;
    short driftY;
} BattleFxLimitSatelliteBeamImpactSlot;

/* S-Beam expanding shell: three size values shifted each frame. */
typedef struct BattleFxLimitSatelliteBeamShell {
    int prevSize;
    int size;
    int nextSize;
} BattleFxLimitSatelliteBeamShell;

/* Projected sprite-pair position for the S-beam effect, scaled by a divisor. */
typedef struct BattleFxSatelliteBeamProjectedPair {
    short projX;
    short projY;
} BattleFxSatelliteBeamProjectedPair;

/* Rates and eased target of the Limit SBeam impact column model task. */
typedef struct BattleFxLimitSatelliteBeamImpact {
    int rateA;
    int rateB;
    int targetRate;
} BattleFxLimitSatelliteBeamImpact;

/* Effect record of the S-beam projected sprite pair task: frame counter and start UV. */
typedef struct BattleFxSatelliteBeamSpriteRecord {
    short timer;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0a[0x2];
    short startU;
    short startV;
} BattleFxSatelliteBeamSpriteRecord;

/* Effect record of the S-beam impact column model task, with a scatter factor. */
typedef struct BattleFxSatelliteBeamImpactRecord {
    short ownerSlot;
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x6];
    short scatter;
} BattleFxSatelliteBeamImpactRecord;

/* Per-target effect record spawned for the S-Beam limit, around a target actor. */
typedef struct BattleFxSatelliteBeamTargetRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short actorIndex;
    unsigned char pad_16[0x2];
    short radius;
} BattleFxSatelliteBeamTargetRecord;

/* Effect-slot record for the S-Beam black puff model. */
typedef struct BattleFxSatelliteBeamPuffRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short scaleY;
} BattleFxSatelliteBeamPuffRecord;

/* Scattered column emitter record for the S-Beam effect, with a position, yaw and height offset. */
typedef struct BattleFxSatelliteBeamScatteredColumn {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x6];
    short offsetY;
} BattleFxSatelliteBeamScatteredColumn;

/* Growing model record for the S-Beam column, with a frame counter and target scale. */
typedef struct BattleFxSatelliteBeamGrowingModel {
    short ownerSlot;
    short frame;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0xc];
    short targetScale;
} BattleFxSatelliteBeamGrowingModel;

/* Main task slot for the black puff emitter, with a frame offset. */
typedef struct BattleFxBlackPuffEmitter {
    unsigned char pad_00[0x2];
    short frameOffset;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0a[0xc];
    short scaleY;
} BattleFxBlackPuffEmitter;

/* Three-short beam impact parameters; timer takes a random value between 500 and 799 when set. */
typedef struct BattleFxBeamImpactParams {
    short timer;
    short phase;
    short drift;
} BattleFxBeamImpactParams;

/* Limit S beam displacement: frameCount times step values, for X and Y. */
typedef struct BattleFxLimitSatelliteBeamDisplacement {
    short displaceX;
    short displaceY;
    short displaceZ;
} BattleFxLimitSatelliteBeamDisplacement;

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamColumnMatrixData {
    short scaleY; /* +0x0 */
    int limitSBeamColumnMatrixPosX; /* +0x2 */
    int limitSBeamColumnMatrixPosY; /* +0x6 */
    int limitSBeamColumnMatrixPosZ; /* +0xa */
} YasuiLimitSatelliteBeamColumnMatrixData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamLineColorParams1 {
    unsigned char pad_0[0x8];
    short limitSBeamLineColorParams1Word8; /* +0x8 */
    short limitSBeamLineColorParams1Word10; /* +0xa */
} YasuiLimitSatelliteBeamLineColorParams1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamLineColorParams2 {
    int beamModelResource; /* +0x0 */
    unsigned char pad_4[0xc];
    int flareModelRes; /* +0x10 */
    unsigned char pad_14[0x6];
    short flareScale; /* +0x1a */
    unsigned char pad_1c[0x4];
    int ringModelRes; /* +0x20 */
    unsigned char pad_24[0x6];
    short ringScale; /* +0x2a */
    unsigned char pad_2c[0x4];
    int smokeSpriteRecord; /* +0x30 */
    int smokeSpriteObject; /* +0x34 */
} YasuiLimitSatelliteBeamLineColorParams2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamLineCoordParams1 {
    short viewportWidth; /* +0x0 */
    short limitSBeamLineCoordViewportHeight; /* +0x2 */
} YasuiLimitSatelliteBeamLineCoordParams1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamLineCoordParams2 {
    unsigned char pad_0[0x1];
    char limitSBeamLineCoordParams2Byte1; /* +0x1 */
    char limitSBeamLineCoordParams2Byte2; /* +0x2 */
    char limitSBeamLineCoordParams2Byte3; /* +0x3 */
} YasuiLimitSatelliteBeamLineCoordParams2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamMeshDescriptor {
    int pillarModelResource; /* +0x0 */
    unsigned char pad_4[0x6];
    short limitSBeamMeshDescriptorWord10; /* +0xa */
    unsigned char pad_c[0x4];
    int nextModelResource; /* +0x10 */
} YasuiLimitSatelliteBeamMeshDescriptor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamModelDrawReq {
    int quadrantModelResource; /* +0x0 */
    int drawFlags; /* +0x4 */
    unsigned char pad_8[0x8];
} YasuiLimitSatelliteBeamModelDrawReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamPuffMatrixData {
    short scaleY; /* +0x0 */
    int limitSBeamPuffMatrixPosX; /* +0x2 */
    int limitSBeamPuffMatrixPosY; /* +0x6 */
    int limitSBeamPuffMatrixPosZ; /* +0xa */
} YasuiLimitSatelliteBeamPuffMatrixData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamScaleMatrix {
    short word8Copy; /* +0x0 */
    unsigned char pad_2[0x6];
    short limitSBeamScaleMatrixWord8; /* +0x8 */
    unsigned char pad_a[0x6];
    short battleLimitSBeamProjectedScale; /* +0x10 */
    int projectedPosX; /* +0x12: the matrix translation, which positions the sprites */
    int projectedPosY; /* +0x16 */
    int projectedScreenScale; /* +0x1a */
    unsigned char pad_1e[0x2];
    short targetOffsetX; /* +0x20 */
    short targetOffsetY; /* +0x22 */
} YasuiLimitSatelliteBeamScaleMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitSatelliteBeamSpriteDrawReq {
    int pairedSpriteResource; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short limitSBeamSpriteDrawReqFlag12; /* +0xc */
    unsigned char pad_e[0x2];
} YasuiLimitSatelliteBeamSpriteDrawReq;
#pragma pack(pop)

/* --- yasui_limit_spark_effect --- */
/* A point of the Spark effect: the three position shorts and a fourth short (the bone's radius), or the
 * 8 bytes as a pair for copying into and out of task slots. */
typedef union YasuiLimitSparkPoint {
    struct {
        short x;
        short y;
        short z;
        short radius;
    };
    struct {
        int packedXY;
        int packedZRadius;
    };
} YasuiLimitSparkPoint;

/* Limit Spark particle: position pulled toward the source bone, offset from source, random lifetime. */
typedef struct BattleFxLimitLivewireParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    short lifetime;
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_12[0x6];
    short scale;
    short scaleStep;
    short durationOrIndex;
} BattleFxLimitLivewireParticle;

/* Limit spark expanding model: step counter and angle with decaying angular velocity. */
typedef struct BattleFxLimitLivewireExpander {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short angle;
    short angularVelocity;
} BattleFxLimitLivewireExpander;

/* Limit spark burst sprite: jittered position per axis, set when a burst spawns. */
typedef struct BattleFxLimitLivewireBurstSprite {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotY;
    unsigned char pad_0E[0xa];
    short speed;
    short lifetime;
} BattleFxLimitLivewireBurstSprite;

/* Animated spark sprite record for the Limit effect, with a frame counter and billboard angle. */
typedef struct BattleFxLimitLivewireSecondaryBurstSprite {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short angle;
    char brightness;
} BattleFxLimitLivewireSecondaryBurstSprite;

/* Limit spark record: stage counter capped at 4, a base scale and an index into a scale table. */
typedef struct BattleFxLimitLivewireRecord {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short baseScale;
    unsigned char pad_1A[0x2];
    short tableIndex;
} BattleFxLimitLivewireRecord;

/* Timeline record for the Limit spark spawn, with constant start values. */
typedef struct BattleFxLimitLivewireTimeline {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short spreadX;
    short spreadY;
    short clearedWord;
} BattleFxLimitLivewireTimeline;

/* Spark streak state for the limit-break spark task, with paired current and saved values. */
typedef struct BattleFxLivewireStreakPrim {
    int packetTag;             /* +0x00 */
    union {                    /* +0x04: vertex 0's colour, the command byte on top */
        int color;
        struct {
            unsigned char pad_04[0x3];
            char code;
        };
    };
    unsigned char pad_08[0x4]; /* +0x08: vertex 0's xy, written by the projection */
    union {                    /* +0x0C: vertex 1's colour */
        int frontColor;
        struct {
            char frontRed;
            char frontGreen;
            char frontBlue;
            char frontUnused;
        };
    };
    short frontX;              /* +0x10 */
    short frontY;
    union {                    /* +0x14: vertex 2's colour */
        int backColor;
        struct {
            char backRed;
            char backGreen;
            char backBlue;
            char backUnused;
        };
    };
    short backX;               /* +0x18 */
    short backY;
} BattleFxLivewireStreakPrim;

/* Spark streak record: elapsed time, bone-relative position, velocity and phase. */
typedef struct BattleFxLivewireStreakSlot {
    short ownerSlot;
    short elapsed;
    short posX;
    short posY;
    short posZ;
    short speed;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x6];
    short phase;
    short phaseStep;
    short duration;
} BattleFxLivewireStreakSlot;

/* Limit spark oriented sprite: stage and position relative to the source bone. */
typedef struct BattleFxLimitLivewireOrientedSprite {
    short ownerSlot;
    short stage;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
} BattleFxLimitLivewireOrientedSprite;

/* Per-task state of the Limit spark animated sprite. */
typedef struct BattleFxLimitLivewireSpriteTask {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotY;
    unsigned char pad_0E[0xa];
    short size;
    char brightness;
} BattleFxLimitLivewireSpriteTask;

/* Spark limit per-target entry: rand() at creation, whose low bit flips the shake direction, and the
 * shake step counter, whose low bit alternates the direction. */
typedef struct YasuiLimitLivewireRandom {
    short randomValue;
    unsigned char pad_02[0x4];
    short shakeStep;
} YasuiLimitLivewireRandom;

#pragma pack(push, 1)
typedef struct YasuiLimitLivewireSpriteRequest1 {
    int sparkSpriteResource; /* +0x0 */
    int sparkDrawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short sparkFrame; /* +0xc */
    unsigned char pad_e[0x2];
    int flareSpriteRecord; /* +0x10 */
    int flareDrawObj; /* +0x14 */
    unsigned char pad_18[0x4];
    short flareFrame; /* +0x1c */
    unsigned char pad_1e[0x2];
    int burstSpriteRecord; /* +0x20 */
    int burstDrawObj; /* +0x24 */
} YasuiLimitLivewireSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitLivewireSpriteRequest2 {
    char colorRed; /* +0x0 */
    char limitSparkSpriteRequest2Byte1; /* +0x1 */
    char limitSparkSpriteRequest2Byte2; /* +0x2 */
    unsigned char pad_3[0x1];
    short frame; /* +0x4 */
    unsigned char pad_6[0x2];
    int nextSpriteRecord; /* +0x8 */
    int nextDrawObj; /* +0xc */
} YasuiLimitLivewireSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitLivewireSpriteRequest3 {
    char colorRed; /* +0x0 */
    char limitSparkSpriteRequest3Byte1; /* +0x1 */
    char limitSparkSpriteRequest3Byte2; /* +0x2 */
    unsigned char pad_3[0x1];
    short limitSparkSpriteRequest3Frame; /* +0x4 */
    unsigned char pad_6[0x2];
} YasuiLimitLivewireSpriteRequest3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiLimitLivewireVectorTable {
    unsigned char pad_0[0x10];
    int limitSparkVectorAt16; /* +0x10 */
} YasuiLimitLivewireVectorTable;
#pragma pack(pop)

/* --- yasui_special_lasboss_effect --- */
/* Timeline record for the Lasboss1 special effect; the field roles are not yet clear. */
typedef struct BattleFxBizarroEnergyTimelineRecord {
    unsigned char pad_00[0x4];
    short radius;
    short heightOffset;
    short angle;
    unsigned char pad_0A[0x2];
    short rotOrVelX;
    short accelY;
    short velocityY;
    unsigned char pad_12[0x2];
    short velocityX;
    unsigned char pad_16[0x2];
    short lifetime;
    short scale;
} BattleFxBizarroEnergyTimelineRecord;

/* Battle effect slot for the Lasboss 1 timeline task; frame counter used by its phases.
 * radius, heightOffset and angle hold the source bone's world position; centroidX/Y/Z the target centroid. */
typedef struct BattleFxBizarroEnergyTimeline {
    short ownerSlot;
    short frame;
    short radius;
    short heightOffset;
    short angle;
    unsigned char pad_0A[0x2];
    short centroidX;
    short centroidY;
    short centroidZ;
} BattleFxBizarroEnergyTimeline;

/* Lasboss1 scaled sprite task slot: frame index, the position RotTransPoint reads and the rotation angles RotMatrix reads. */
typedef struct BattleFxBizarroEnergyScaledSpriteSlot {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    SVECTOR rotation;
} BattleFxBizarroEnergyScaledSpriteSlot;

/* Lasboss1 rising sprite record: phase, three positions and three velocities. */
typedef struct BattleFxBizarroEnergyRisingSpriteRecord {
    short ownerSlot;
    short phase;
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
} BattleFxBizarroEnergyRisingSpriteRecord;

/* Effect-100 slot for the Lasboss1 rising-sprite spawner; spawn-step counter. */
typedef struct BattleFxBizarroEnergySpawnState {
    short ownerSlot;
    short timer;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
} BattleFxBizarroEnergySpawnState;

/* Effect-100 rising sprite spawned by the Lasboss1 spawner; vertical position and velocity. */
typedef struct BattleFxRisingSpriteSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxRisingSpriteSlot;

/* Battle task slot of the Lasboss1 camera script: the script cursor, the matrix that places the camera
 * and the actor the script attaches to. */
typedef struct BattleFxBizarroEnergyCameraTask {
    short ownerSlot;
    short initialized;
    unsigned char pad_04[0x8];
    short *scriptCursor;
    PsxMatrix *matrixPtr;
    short attachPoint;
} BattleFxBizarroEnergyCameraTask;

/* State of one spiralling sprite: centre, orbit radius and angle, advanced each frame. */
typedef struct BattleFxSpiralSprite {
    short step;
    short frameIndex;
    short centerX;
    short radius;
    short centerY;
    unsigned char pad_0A[0x4];
    short radiusVelocity;
    unsigned char pad_10[0x4];
    short angle;
    unsigned char pad_16[0x2];
    short phase;
    short scale;
} BattleFxSpiralSprite;

/* Lasboss1 scaling model emitter: timer, position and scale. */
typedef struct BattleFxBizarroEnergyScalingEmitter {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short scale;
} BattleFxBizarroEnergyScalingEmitter;

/* Per-model record for the growing Lasboss 1 model task; its step drives the model scale. */
typedef struct BattleFxBizarroEnergyGrowRecord {
    short ownerSlot;
    short growStep;
    short posX;
    short posY;
    short growAmount;
} BattleFxBizarroEnergyGrowRecord;

/* Lasboss1 sliding model task state: end marker, frame counter and slide position. */
typedef struct YasuiBizarroEnergySlideState {
    short endMarker;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} YasuiBizarroEnergySlideState;

/* The first 0x10 bytes of a BattleFxModelDraw: the request a Lasboss1 model task fills before BattleFx_SubmitModelDraw. */
typedef struct YasuiSpecBizarroEnergyModelDrawRequest {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} YasuiSpecBizarroEnergyModelDrawRequest;

#pragma pack(push, 1)
typedef struct YasuiSpecBizarroEnergyCameraEyeTrack {
    short trackState; /* +0x0 */
    short finished; /* +0x2 */
} YasuiSpecBizarroEnergyCameraEyeTrack;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiSpecBizarroEnergyCameraFocusTrack {
    short trackStep; /* +0x0 */
    short isReady; /* +0x2 */
} YasuiSpecBizarroEnergyCameraFocusTrack;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiSpecBizarroEnergyCameraMatrix {
    int cameraPosition; /* +0x0 */
    int lasboss1CameraMatrixValueA; /* +0x4 */
    unsigned char pad_8[0x18];
    PsxMatrix rotationMatrix; /* +0x20 */
} YasuiSpecBizarroEnergyCameraMatrix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiSpecBizarroEnergyCameraTargetActor {
    short targetActorIndex; /* +0x0 */
    short boneIdx; /* +0x2 */
} YasuiSpecBizarroEnergyCameraTargetActor;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiSpecBizarroEnergyDrawReqPool {
    int lasboss1DriftingSpriteAnimBlock; /* +0x0 */
    int lasboss1DriftingSpriteResource; /* +0x4 */
    unsigned char pad_8[0x8];
    int lasboss1AnimSpriteResourceA; /* +0x10 */
    int lasboss1AnimSpriteResourceB; /* +0x14 */
    unsigned char pad_18[0x4];
    short lasboss1AnimSpriteFieldWord; /* +0x1c */
    unsigned char pad_1e[0x2];
    int lasboss1ScaledSpriteResource; /* +0x20 */
    int lasboss1ScaledSpriteResourceB; /* +0x24 */
    unsigned char pad_28[0x4];
    short lasboss1ScaledSpriteFieldWord; /* +0x2c */
    unsigned char pad_2e[0x2];
    int lasboss1ScalingModelResource; /* +0x30 */
    unsigned char pad_34[0xc];
    int lasboss1ScalingModelPoint; /* +0x40 */
    unsigned char pad_44[0x4];
    int lasboss1SpiralSpriteResourceA; /* +0x48 */
    int lasboss1SpiralSpriteResourceB; /* +0x4c */
    unsigned char pad_50[0x4];
    short lasboss1SpiralSpriteFieldWord; /* +0x54 */
    unsigned char pad_56[0x2];
    int lasboss1RisingSpriteResourceA; /* +0x58 */
    int lasboss1RisingSpriteResourceB; /* +0x5c */
    unsigned char pad_60[0x4];
    short lasboss1RisingSpritePhase; /* +0x64 */
    unsigned char pad_66[0x2];
} YasuiSpecBizarroEnergyDrawReqPool;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct YasuiSpecBizarroEnergyModel0Scale {
    unsigned char pad_0[0x4];
    int scaleY; /* +0x4 */
    unsigned char pad_8[0x4];
} YasuiSpecBizarroEnergyModel0Scale;
#pragma pack(pop)

/* --- yasui_special_magend_effect --- */
/* Falling sprite particle slot of the Special Mag End effect. */
typedef struct BattleFxHeartlessAngelFallingSprite {
    short ownerSlot;
    short frameIndex;
    short posA;
    short fallSpeed;
    short posB;
    unsigned char pad_0A[0x2];
    short velocityA;
    short fallAccel;
    short velocityB;
} BattleFxHeartlessAngelFallingSprite;

/* Mag end emitter task state: end marker, frame counter, the target (whose typed record it allocates) and actorIndex. */
typedef struct YasuiHeartlessAngelEmitterState {
    short endMarker;
    short frameCounter;
    unsigned char pad_04[0x18];
    short target;
    short actorIndex;
} YasuiHeartlessAngelEmitterState;

/* Emitter point whose three shorts are jittered by random offsets each frame. */
typedef struct BattleFxHeartlessAngelEmitterPoint {
    unsigned char pad_00[0x4];
    short x;
    short y;
    short z;
    unsigned char pad_0A[0x2];
    short projected[3]; /* RotTransPointToInt16's output */
} BattleFxHeartlessAngelEmitterPoint;

#pragma pack(push, 1)
typedef struct HeartlessAngel_SpriteDrawReq {
    int spriteField; /* +0x0 */
    int fallingSpriteResource; /* +0x4 */
    unsigned char pad_8[0x8];
    int emitterScreenPoint; /* +0x10 */
} HeartlessAngel_SpriteDrawReq;
#pragma pack(pop)

/* --- yasui_special_punch_effect --- */
/* Special punch flight record in the battle effect-100 pool, with target position and actors. */
typedef struct BattleFxSpecialRocketPunch {
    short step;
    short timer;
    unsigned char pad_04[0x8];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_12[0xa];
    short hitActor;
    short actorIndex;
} BattleFxSpecialRocketPunch;

/* Punch trail particle: frame count against flight duration, and position and velocity per axis. */
typedef struct BattleFxRocketPunchTrailParticle {
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
    short slotIndex;
} BattleFxRocketPunchTrailParticle;

/* Effect100 slot record for the Special Punch timeline: start point, per-axis velocity and state. */
typedef struct BattleFxSpecialRocketPunchTimeline {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short state;
} BattleFxSpecialRocketPunchTimeline;

/* Punch-trail model record, pointed at by frame+56. */
typedef struct BattleFxRocketPunchTrailRecord {
    short ownerSlot; /* +0x0 */
    short frameCount;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xc];
    short slotIndex;
} BattleFxRocketPunchTrailRecord;

#pragma pack(push, 1)
typedef struct YasuiSpecRocketPunchTrailModelDrawReq {
    unsigned char pad_0[0x8];
    int punchTrailModelResource; /* +0x8 */
    int punchTrailModelFlags; /* +0xc */
    unsigned char pad_10[0x2];
    short punchTrailModelScale; /* +0x12 */
} YasuiSpecRocketPunchTrailModelDrawReq;
#pragma pack(pop)

/* --- yasui_special_toseki_effect --- */
typedef struct EffectVectorInitLocals {
    short angles[4];
    int matrix[8];
    short index;
    short pad;
    short vector[4];
} EffectVectorInitLocals;

/* PSX-style GPU primitive words written to the render state block for the Toseki special draw. */
typedef struct BattleFxBoulderThrowPrimitive {
    int tag;
    int colorCommand;
    unsigned char pad_08[0x4];
    int linkWord;
    unsigned char pad_10[0x4];
    short uvWordA;
    short tpage;
    unsigned char pad_18[0x4];
    short uvWordB;
    unsigned char pad_1E[0x6];
    short vertexOffset;
} BattleFxBoulderThrowPrimitive;

/* Toseki timeline record: 20-frame timer and start/end X, Y, Z pairs driving a child record. */
typedef struct BattleFxBoulderThrowTimeline {
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
    short recordType;
} BattleFxBoulderThrowTimeline;

/* Task slot of the Toseki shattering mesh effect: timer, position and per-step velocity. */
typedef struct BattleFxBoulderThrowShardState {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxBoulderThrowShardState;

/* Tosek timeline segment: position and per-step deltas. */
typedef struct BattleFxBoulderThrowSegment {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short stepX;
    short stepY;
    short stepZ;
} BattleFxBoulderThrowSegment;

/* Toseki special mesh stream record: vertex offsets and a word copied into primitive headers. */
typedef struct YasuiBoulderThrowMeshStream {
    short vertexWord;
    short vertexOffsetB;
    short vertexOffsetC;
    short resourceOffset;
    int primWord;
    short clutWord;
    short tpageWord;
    short vertexOffsetA;
    short colorVertexB;
    short sharedVertexOffset;
    short vertexOffsetB2;
    short sharedVertexOffset2;
    short quadOffset;
    short vertCOffset;
} YasuiBoulderThrowMeshStream;

/* Toseki mesh packet; words at 0x14 and 0x1C are copied from a 0x10/0x14 source. */
typedef struct BattleFxBoulderThrowPacket {
    unsigned char pad_00[0x14];
    int linkWord;
    unsigned char pad_18[0x4];
    int color3;
} BattleFxBoulderThrowPacket;

/* Toseki link record; its words are copied into packets, the first into the link word. */
typedef struct BattleFxBoulderThrowLinkRecord {
    unsigned char pad_00[0xc];
    int linkWord;
    int primWord;
    int color3;
} BattleFxBoulderThrowLinkRecord;

/* --- yasui_summon_knights_of_round --- */
typedef struct KotrLoaderLocals {
    char resourcePath[200];
    int index;
    char path[200];
} KotrLoaderLocals;

typedef struct Int32Box {
    int value;
} Int32Box;

/* One gouraud-shaded textured quad of the radial disc (0x34 bytes, the PSX POLY_GT4 layout). */
typedef struct BattleKotrDiscQuad {
    int tag;
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
    short pad26;
    int color3;
    int xy3;
    char u3;
    char v3;
    short pad32;
} BattleKotrDiscQuad;

/* One cell of the radial disc: its four corner points and one quad for each of the two render banks. */
typedef struct KotrRenderRecord {
    SVECTOR *vertexA;
    SVECTOR *vertexB;
    SVECTOR *vertexC;
    SVECTOR *vertexD;
    BattleKotrDiscQuad quads[2];
} KotrRenderRecord;

typedef struct KotrRenderRow {
    KotrRenderRecord records[0x20];
} KotrRenderRow;

#pragma pack(push, 1)
/* Scratch block of a knight's orbit: the rotation angles, reused as the point the matrix rotates. */
typedef struct BattleKotrOrbitPose {
    SVECTOR point;
    PsxMatrix matrix; /* +0x08 */
} BattleKotrOrbitPose;

/* Scratch block of a model task: rotation angles, scale vector and the model matrix. */
typedef struct BattleKotrModelPose {
    SVECTOR rotation;
    int scaleX; /* +0x08 */
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleKotrModelPose;

/* Scratch block of a point and the matrix that rotates it in place. */
typedef struct BattleKotrPointPose {
    SVECTOR point;
    unsigned char pad_08[0x10];
    PsxMatrix matrix; /* +0x18 */
} BattleKotrPointPose;

/* Scratch block of the spinning model: its angles, the scaled matrix and the spin matrix. */
typedef struct BattleKotrSpinModelPose {
    SVECTOR rotation;
    PsxMatrix scaled; /* +0x08 */
    PsxMatrix spin; /* +0x26 */
} BattleKotrSpinModelPose;
#pragma pack(pop)

/* The 16-byte head of a model draw record that BattleFx_SubmitModelDraw reads: the model group the
   resource table supplies, the draw flags, UV scroll, fade level, state flags and texture page bits. */
typedef struct {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
    short stateFlags;
    short texturePageBits;
} BattleKotrModelDrawHead;

/* The parameters BattleFx_KotrSpawnEasedSpinScaleModel draws one spin-scale model with: the model group
   the resource table supplies, then the draw and spin parameters. */
typedef struct {
    int modelGroup;
    unsigned char params[0x1c];
} BattleKotrSpinScaleParams;

/* Effect-100 array entry (0x20 bytes) that runs the Kotr summon camera script. */
typedef struct BattleFxKotrCamera {
    short ownerSlot;
    short started;
    unsigned char pad_04[0x8];
    short *scriptPtr;
    union { int rotMatrix; void *rotMatrixPtr; };
} BattleFxKotrCamera;

/* Fading offscreen textured quad primitive packet built from the render state block. */
typedef struct YasuiKotrFadeQuad {
    int tag;
    int color0;
    short initOffset1;
    short initOffset0;
    char fadeStart0;
    char fadeStart1;
    unsigned char pad_0E[0x2];
    int color1;
    short offset1Copy;
    short baseOffset0;
    char fadeValue0Copy;
    char fadeSource1;
    short tpage;
    int color2;
    short baseOffset1;
    short offset0Copy;
    char fadeSource0;
    char fadeValue1Copy;
    unsigned char pad_26[0x2];
    int color3;
    short offset1;
    short offset0;
    char fadeValue0;
    char fadeValue1;
} YasuiKotrFadeQuad;

/* Kotr actor lift task state: actor index, frame count, lift height and velocity. */
typedef struct BattleFxKotrLiftState {
    short actorIndex;
    short frameCount;
    unsigned char pad_04[0x2];
    short liftHeight;
    unsigned char pad_08[0x4];
    short bobPhase;
    short liftVelocity;
    short relativeHeight;
    unsigned char pad_12[0xa];
    short savedFlags;
} BattleFxKotrLiftState;

/* Battle actor record used by the Kotr actor lift task. */
typedef struct BattleActorLiftRecord {
    unsigned char pad_00[0x8];
    unsigned short scaleX;
    unsigned short scaleY;
    short boneIndex;
    unsigned char pad_0E[0x17];
    char flags;
    unsigned char pad_26[0x5];
    char attachPoint;
    unsigned char pad_2C[0x13a];
    short posX;
    short height;
    short posZ;
    unsigned char pad_16C[0x2e];
    short bone0PosX;
    unsigned char pad_19C[0x2];
    short bone0PosZ;
} BattleActorLiftRecord;

/* Kotr tumbling model: start and delta position, angle and angle step per axis. */
typedef struct BattleFxKotrTumbleModel {
    short ownerSlot;
    short age;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_12[0x2];
    unsigned char angleX;
    unsigned char angleY;
    unsigned char angleZ;
    unsigned char pad_17[0x1];
    char angleStepX;
    char angleStepY;
    char angleStepZ;
    unsigned char pad_1B[0x1];
    short startSize;
} BattleFxKotrTumbleModel;

/* Knight of the Round endpoint trail task record: end flag, frame counter, slot and frame count. */
typedef struct BattleKotrTrailTask {
    short endFlag;
    short frame;
    short startEndpoint0X;
    short startEndpoint0Y;
    short startEndpoint0Z;
    unsigned char pad_0A[0x2];
    short startEndpoint1X;
    short startEndpoint1Y;
    short startEndpoint1Z;
    unsigned char pad_12[0x2];
    struct BattleFxKnightEndpointDef *endpointDef;
    struct BattleFxKotrKnightTrail *slot;
    short frameCount;
} BattleKotrTrailTask;

/* Offscreen quad particle in the battle effect-100 pool, moved by position plus damped velocity. */
typedef struct BattleFxOffscreenParticle {
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
    char fadeSource0;
    char fadeSource1;
} BattleFxOffscreenParticle;

/* Effect100 slot record for the KOTR shake envelope: frame counter, amplitude and three phase durations. */
typedef struct BattleFxKotrShakeEnvelope {
    short ownerSlot;
    short frame;
    short amplitude;
    short attackFrames;
    short sustainFrames;
    short releaseFrames;
} BattleFxKotrShakeEnvelope;

/* Kotr tumbling model spawn record: three positions and three velocities. */
typedef struct BattleFxKotrTumblingSpawnRecord {
    unsigned char pad_00[0x4];
    short posA;
    short posB;
    short posC;
    unsigned char pad_0A[0x2];
    short velA;
    short velB;
    short velC;
    unsigned char pad_12[0xa];
    short amplitude;
} BattleFxKotrTumblingSpawnRecord;

/* Kotr knight endpoint trail state: two interpolated endpoints and a flags word. */
#pragma pack(push, 1)
typedef struct BattleFxKotrEndpointTrail {
    int depth;
    int quadScratch;
    int flags;
    short lastDeltaX;
    short lastDeltaY;
    short lastDeltaZ;
    unsigned char pad_12[0x2];
    short deltaXNow;
    short deltaYNow;
    short deltaZNow;
    unsigned char pad_1A[0x2];
    short endpoint0X;
    short endpoint0Y;
    short endpoint0Z;
    unsigned char pad_22[0x2];
    short endpoint1X;
    short endpoint1Y;
    short endpoint1Z;
    unsigned char pad_2A[0x2];
    short savedEndpoint0X;
    short savedEndpoint0Y;
    short savedEndpoint0Z;
    unsigned char pad_32[0x2];
    short prevEndpoint0X;
    short prevEndpoint0Y;
    short prevEndpoint0Z;
    unsigned char pad_3A[0x2];
    short prevEndpoint1X;
    short prevEndpoint1Y;
    short prevEndpoint1Z;
    unsigned char pad_42[0x2];
    int deltaX;
    int deltaY;
    int deltaZ;
    unsigned char pad_50[0x14];
    PsxMatrix matrix;
    int shadeColor;
} BattleFxKotrEndpointTrail;
#pragma pack(pop)

/* Kotr radial disc battle-summon state: sin/cos ring offsets, depth and flags */
typedef struct BattleFxKotrRadialDisc {
    SVECTOR center;
    SVECTOR edgeStart; /* the ring edge at the segment's first angle */
    SVECTOR edgeEnd; /* the ring edge at the segment's second angle */
    int depth;
    int projectScratch;
    int flags;
} BattleFxKotrRadialDisc;

/* Kotr offscreen quad particle: position, velocity and disc height at its radius. */
typedef struct BattleFxKotrQuadParticle {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short posX;
    short height;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0xc];
    char texU;
    char texV;
} BattleFxKotrQuadParticle;

/* Kotr knight timeline: step counter, height and the knight object it drives. */
typedef struct BattleFxKotrKnightTimeline {
    short ownerSlot;
    short step;
    short offsetX;
    short height;
    short offsetZ;
    unsigned char pad_0A[0xa];
    struct BattleKotrKnight *knight;
} BattleFxKotrKnightTimeline;

/* Kotr expanding fading model: frame counter from which the expansion fraction derives. */
typedef struct BattleFxKotrExpandingModel {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    short rotX;
    short rotY;
    short rotZ;
} BattleFxKotrExpandingModel;

/* Kotr tumbling timeline: step counter and three position values for spawned models. */
typedef struct BattleFxKotrTumblingTimeline {
    short ownerSlot;
    short step;
    short posA;
    short posB;
    short posC;
} BattleFxKotrTumblingTimeline;

/* Kotr tumbling shrinking model emitter: base position and offsets from Kotr state. */
typedef struct BattleFxKotrShrinkEmitter {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_12[0x2];
    int packedAngles;
    char angleStepX;
    char angleStepY;
    char angleStepZ;
    unsigned char pad_1B[0x1];
    short startSize;
} BattleFxKotrShrinkEmitter;

/* Eased spin and scale parameters for the Kotr summon model task. */
typedef struct BattleFxKotrEasedSpinParams {
    int modelGroup;
    short duration;
    short startValue;
    short endValue;
    short scaleEaseMode;
    unsigned char pad_0C[0x2];
    short drawEaseMode;
    short amplitude;
    short yawEaseMode;
} BattleFxKotrEasedSpinParams;

/* Phased fade state of a Kotr summon primitive slot. */
typedef struct BattleFxKotrFadingPrimitive {
    short ownerSlot;
    short frameIndex;
    short phase1Frames;
    short phase2Frames;
    short phase3Frames;
} BattleFxKotrFadingPrimitive;

/* Effect-slot record for the Kotr eased spin-scale model, with its target pointer. */
typedef struct BattleFxKotrSpinScaleRecord {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    int target;
} BattleFxKotrSpinScaleRecord;

/* Effect-slot record for the Kotr knight timeline, holding a pointer to its knight. */
typedef struct BattleFxKotrKnight8Record {
    short ownerSlot;
    short step;
    unsigned char pad_04[0x10];
    struct BattleKotrKnight *knight;
} BattleFxKotrKnight8Record;

/* Effect-slot record for the Kotr radial streak model, on a radius and angle. */
typedef struct BattleFxKotrRadialStreakRecord {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0x8];
    short radius;
    unsigned char pad_0E[0x2];
    short angle;
} BattleFxKotrRadialStreakRecord;

/* Render-state block for the Kotr knight endpoint trail, holding colour words. */
typedef struct BattleFxKotrRenderStateBlock {
    int opcode;
    int colorWord;
    unsigned char pad_08[0x4];
    int color;
    unsigned char pad_10[0x4];
    int rawColorWord;
    unsigned char pad_18[0x4];
    int rawColor;
} BattleFxKotrRenderStateBlock;

/* Spinning fading model record for the Kotr summon, writing positions into the Kotr state. */
typedef struct BattleFxKotrSpinningFadeModel {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    int fadeRecord;
} BattleFxKotrSpinningFadeModel;

/* Kotr knight endpoint trail slot; flags, fixed-point trail scale and actor attach index. */
typedef struct BattleFxKotrKnightTrail {
    short flags;
    unsigned char pad_02[0x4c];
    short trailScale;
    unsigned char pad_50[0x8];
    short attachIndex;
} BattleFxKotrKnightTrail;

/* Effect-100 slot for the Kotr spinning model pair; spin value written into the Kotr state. */
typedef struct BattleFxKotrSpinningPair {
    short ownerSlot;
    short spin;
} BattleFxKotrSpinningPair;

/* Kotr summon spin state with frame and scale values, read by BattleFx_KotrSpinningGrowingModelTask. */
#pragma pack(push, 1)
typedef struct YasuiKotrSpinState {
    short rotX;
    short angle;
    short rotZ;
    unsigned char pad_06[0x2];
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix taskRotation; /* +0x18 */
    PsxMatrix matrix; /* +0x36 */
} YasuiKotrSpinState;
#pragma pack(pop)

/* Kotr tumbling-shrink emitter jitter and drift state, read by BattleFx_KotrTumblingShrinkingModelEmitterTask. */
typedef struct YasuiKotrEmitterState {
    short jitterX;
    short driftY;
    short jitterZ;
} YasuiKotrEmitterState;

/* Orbit position state of Kotr Knight 7, advanced by its timeline task each frame. */
#pragma pack(push, 1)
typedef struct BattleFxKotrKnight7State {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleFxKotrKnight7State;
#pragma pack(pop)

/* Fragment record for the KOTR effect: three vertex-array offsets for the P, Q and S vertex types. */
typedef struct YasuiKotrFragmentRecord {
    short vertexPOffset;
    short vertexQOffset;
    short vertexSOffset;
    unsigned char pad_06[0xa];
} YasuiKotrFragmentRecord;

/* Task slot record for one Kotr impact burst particle, filled from its summon data row. */
typedef struct BattleFxKotrImpactBurstSlot {
    unsigned char pad_00[0x4];
    short originX;
    short originY;
    short originZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
} BattleFxKotrImpactBurstSlot;

/* Emitter of fading offscreen quad particles for the Kotr effect; positions are scaled to screen space. */
typedef struct BattleFxKotrQuadEmitter {
    short ownerSlot;
    short index;
    short posX;
    unsigned char pad_06[0x2];
    short posY;
} BattleFxKotrQuadEmitter;

/* Timeline record of the Kotr knight 1 effect; a counter advances it and points at the knight. */
typedef struct BattleFxKotrKnight1Timeline {
    short ownerSlot; /* +0x0 */
    short frame;
    unsigned char pad_04[0x10];
    struct BattleKotrKnight *knight;
} BattleFxKotrKnight1Timeline;

/* Pulsing triangle particle task of the Kotr effect; a counter drives a sine-based offset. */
typedef struct BattleFxKotrPulsingTriangleTask {
    short ownerSlot;
    short frame;
    unsigned char pad_04[0xc];
    short phase;
} BattleFxKotrPulsingTriangleTask;

/* Task slot record for a Kotr pulsing billboard model. */
typedef struct BattleFxKotrPulseBillboardTask {
    short ownerSlot;
    short frameCount;
    unsigned char pad_04[0x18];
    short amplitude;
} BattleFxKotrPulseBillboardTask;

/* Task slot record for the Kotr fading stacked model layers task. */
typedef struct BattleFxKotrFadeLayersTask {
    short ownerSlot;
    short layerIndex;
    int layerFade;
} BattleFxKotrFadeLayersTask;

/* Task slot record that fires one sound effect after a frame delay. */
typedef struct BattleFxKotrDelayedSoundTask {
    short triggerFrame;
    short frameCount;
    short soundArg;
} BattleFxKotrDelayedSoundTask;

/* Kotr camera script task in fixed slot 99, holding eye and focus tracks and a duration. */
typedef struct BattleFxKotrCameraScriptTask {
    short ownerSlot;
    unsigned char pad_02[0xa];
    int eyeTrack;
    int focusTrack;
    short duration;
} BattleFxKotrCameraScriptTask;

/* Kotr screen shake envelope task record. */
typedef struct BattleFxKotrShakeEnvelopeTask {
    unsigned char pad_00[0x4];
    short amplitude;
    short duration;
    short rate;
    short decay;
} BattleFxKotrShakeEnvelopeTask;

/* Kotr eased spin-scale model task record filled from a spawn record. */
typedef struct BattleFxKotrEasedSpinTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    int model;
} BattleFxKotrEasedSpinTask;

/* Kotr converging streak emitter task record. */
typedef struct BattleFxKotrConvergingStreakTask {
    unsigned char pad_00[0xc];
    short progress;
    short frame;
    short angle;
} BattleFxKotrConvergingStreakTask;

/* Kotr spinning models and layer task record with a callback parameter block. */
typedef struct BattleFxKotrSpinningLayerTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    int parameterBlock;
} BattleFxKotrSpinningLayerTask;

/* The KOTR radial disc buffer: 17 rings of 32 points, 16 render rows of 32 records, and one colour word
 * per ring. */
typedef struct BattleKotrDiscBuffer {
    SVECTOR points[17][32];
    KotrRenderRow rows[16];
    int colorWord[17];
} BattleKotrDiscBuffer;

/* Battle task slot for the Knight endpoint trail, holding its endpoint definition, trail and duration. */
typedef struct BattleFxKotrEndpointTrailTask {
    unsigned char pad_00[0x14];
    void *endpointDef;
    struct BattleKotrKnight *knightTrail;
    short duration;
} BattleFxKotrEndpointTrailTask;

/* Per-model timeline state that is reset to zero when the KOTR shrink or expand task starts. */
typedef struct BattleFxModelTimeline {
    unsigned char pad_00[0xc];
    short rotX;
    short rotY;
    short rotZ;
} BattleFxModelTimeline;

/* Render state block written with two GPU-style command words for the radial disc mesh. */
typedef struct BattleFxRadialDiscPacket {
    int tag;
    int command;
} BattleFxRadialDiscPacket;

/* Main task slot for the actor lift effect, holding a target index. */
typedef struct BattleFxActorLiftTask {
    short targetIndex;
    unsigned char pad_02[0xa];
    short randomValue;
} BattleFxActorLiftTask;

/* Endpoint definition for the Knight trail whose two radii are scaled by the trail scale. */
typedef struct BattleFxKnightEndpointDef {
    unsigned char pad_00[0x4];
    short radius0;
    unsigned char pad_06[0x6];
    short radius1;
} BattleFxKnightEndpointDef;

/* Main task slot for the radial streak emitter. */
typedef struct BattleFxRadialStreakTask {
    unsigned char pad_00[0xc];
    short contribution;
    unsigned char pad_0E[0x2];
    short angle;
} BattleFxRadialStreakTask;

/* Render state block with a command word and a color word for PSX fragment triangles. */
typedef struct BattleFxPsxFragmentPacket {
    int tag;
    int color;
} BattleFxPsxFragmentPacket;

/* Main task slot that plays a sound after a delay. */
typedef struct BattleFxDelayedSoundTask {
    short soundId;
    unsigned char pad_02[0x2];
    short delay;
} BattleFxDelayedSoundTask;

/* Kotr impact burst state: position, velocity and parallax offset per axis. */
typedef struct BattleKotrImpactBurst {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0E[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_16[0x14];
    short velocityX;
    unsigned char pad_2C[0x2];
    short velocityY;
    unsigned char pad_30[0x2];
    short velocityZ;
} BattleKotrImpactBurst;

/* KOTR fragment motion state, initialised from its face normals. */
typedef struct BattleFxKotrFragment {
    short delay;
    short scale;
    short shrinkRate;
    unsigned char pad_06[0x2];
    short centroidX;
    short centroidY;
    short centroidZ;
    unsigned char pad_0E[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_16[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_1E[0x2];
    short rotSpeedX;
    short rotSpeedY;
    short rotSpeedZ;
    unsigned char pad_26[0x2];
} BattleFxKotrFragment;

/* Layer record for the stacked model draw of the Kotr summon. */
typedef struct BattleFxKotrLayerRecord {
    int modelResource;
    int flags;
    short layerMode;
} BattleFxKotrLayerRecord;

/* Draw-state record of the knight model: rotation and scale floats per axis. */
typedef struct BattleFxKnightModelDraw {
    unsigned char pad_00[0x4];
    int offsetX;
    int offsetY;
    int offsetZ;
    float rotX;
    float rotY;
    float rotZ;
    float scaleX;
    float scaleY;
    float scaleZ;
} BattleFxKnightModelDraw;

/* Per-task state of the Kotr stacked model layers, reached through the pointer at frame+44. */
typedef struct BattleFxKotrLayerState {
    unsigned char pad_00[0x4];
    int flags;
    short drawFlags;
} BattleFxKotrLayerState;

/* Kotr knight draw record: position from the Kotr state with random jitter, plus a random value. */
typedef struct BattleFxKotrKnightFragment {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short randomValue;
} BattleFxKotrKnightFragment;

/* Eight-byte source entry of the Kotr impact burst. */
typedef struct BattleFxKotrBurstSource {
    short originX;
    short originY;
    short originZ;
    unsigned char pad_06[0x2];
} BattleFxKotrBurstSource;

/* Fragment state (0x28 bytes) of the Kotr summon spin, scatter and fade. */
typedef struct BattleFxKotrFragmentState {
    unsigned char pad_00[0x20];
    short spinX;
    short spinY;
    short spinZ;
    unsigned char pad_26[0x2];
} BattleFxKotrFragmentState;

/* Record read through the actor's state pointer during the Kotr actor lift. */
typedef struct BattleFxKotrActorLift {
    unsigned char pad_00[0xc];
    short bobPhase;
    unsigned char pad_0E[0x2];
    short relativeHeight;
} BattleFxKotrActorLift;

/* Per-fragment colour byte table written by the Kotr spin and scatter fade. */
typedef struct BattleFxKotrFragmentColorByte1 {
    char colorLevel;
    unsigned char pad_01[0xf];
} BattleFxKotrFragmentColorByte1;

/* Per-fragment primitive command word set by the Kotr fade. */
typedef struct BattleFxKotrFragmentPrimCommand {
    int command;
    unsigned char pad_04[0xc];
} BattleFxKotrFragmentPrimCommand;

/* Record a frame points at, read for the Kotr fragment motion from normals. */
typedef struct BattleFxKotrFragmentNormal {
    short vertexOffsetP;
    short vertexOffsetA;
    short vertexOffsetB;
} BattleFxKotrFragmentNormal;

/* Kotr summon offscreen quad particle state, read through a kept pointer. */
typedef struct BattleFxKotrFadeParticle {
    int depth;
    unsigned char pad_04[0x4];
    int flags;
} BattleFxKotrFadeParticle;

/* Kotr summon pulsing triangle particle state, read through a kept pointer. */
typedef struct BattleFxKotrTriangleParticle {
    unsigned char pad_00[0x20];
    int depth;
    unsigned char pad_24[0x4];
    int flags;
} BattleFxKotrTriangleParticle;

/* Render-state block holding a primitive pair; tag word 0x04000000 at offset 0. */
typedef struct BattleFxPrimitivePairPacket {
    int packetTag; /* +0x0 */
    unsigned char pad_4[0x3];
    char flags;
    int xyWord;
    char texU;
    char texV;
    short clut;
    int sizeWord;
} BattleFxPrimitivePairPacket;

/* PSX textured triangle primitive for a pulsing Kotr particle, with fixed colour and tag words. */
typedef struct BattleFxKotrPulseTrianglePrim {
    int header;
    int colorCommand;
    unsigned char pad_08[0x4];
    int color;
    unsigned char pad_10[0x4];
    int thirdColor;
    unsigned char pad_18[0x4];
    int endMarker;
} BattleFxKotrPulseTrianglePrim;

/* Tail of the billboard matrix the Kotr impact sprites install: the last rotation entry (set to the
 * billboard angle) and the three translation words. */
#pragma pack(push, 1)
typedef struct BattleKotrBillboardTail {
    short angle;
    int transX;
    int transY;
    int transZ;
} BattleKotrBillboardTail;
#pragma pack(pop)

/* Knight of the Round state: knight position minus offset, copied for the model draw. */
#pragma pack(push, 1)
typedef struct BattleSummonKotrKnightState {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_06[0x12];
    PsxMatrix pose; /* +0x18 */
    union {
        PsxMatrix local; /* +0x36 */
        struct {
            unsigned char pad_36[0x12];
            short composedX; /* the low halves of local.t */
            unsigned char pad_4A[0x2];
            short composedY;
            unsigned char pad_4E[0x2];
            short composedZ;
        };
    };
} BattleSummonKotrKnightState;
#pragma pack(pop)

/* Knight of the Round lift state: lift offset and copied actor fields. */
#pragma pack(push, 1)
typedef struct BattleSummonKotrLift {
    short angleX;
    short liftOffset;
    short angleZ;
    unsigned char pad_06[0x2];
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleSummonKotrLift;
#pragma pack(pop)

/* Kotr spinning model pair task state: spin values set from the spin input, or zeroed. */
#pragma pack(push, 1)
typedef struct BattleFxKotrSpinState {
    short spinA;
    short spinB;
    short spinC;
    unsigned char pad_06[0x2];
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleFxKotrSpinState;
#pragma pack(pop)

/* Kotr eased spin scale state: eased amplitude and value history. */
#pragma pack(push, 1)
typedef struct BattleFxKotrEasedSpinState {
    short rotX;
    short easedAmplitude;
    short rotZ;
    unsigned char pad_06[0x2];
    int valueOld;
    int valuePrev;
    int valueNew;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
    PsxMatrix tiltMatrix; /* +0x36 */
} BattleFxKotrEasedSpinState;
#pragma pack(pop)

/* Kotr tumbling shrinking model state: angles, value history and shrinking size. */
#pragma pack(push, 1)
typedef struct BattleFxKotrTumbleShrinkState {
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_06[0x2];
    int valueOld;
    int valuePrev;
    int size;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleFxKotrTumbleShrinkState;
#pragma pack(pop)

/* Kotr radial streak model state with an angle field. */
#pragma pack(push, 1)
typedef struct BattleFxKotrRadialStreakState {
    short rotX;
    short rotY;
    short angle;
    unsigned char pad_06[0x2];
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_14[0x4];
    PsxMatrix matrix; /* +0x18 */
} BattleFxKotrRadialStreakState;
#pragma pack(pop)

/* Kotr converging streak model state with an angle field. */
/* Kotr fragment spin and shrink matrix state record. */
#pragma pack(push, 1)
typedef struct BattleFxKotrFragmentScale {
    int scaleX;
    int scaleY;
    int scaleZ;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix; /* +0x10 */
} BattleFxKotrFragmentScale;
#pragma pack(pop)

/* Kotr fragment scatter matrix state record. */
#pragma pack(push, 1)
typedef struct BattleFxKotrScatterState {
    int scaledY;
    int scale;
    int savedScale;
    unsigned char pad_0C[0x4];
    PsxMatrix matrix; /* +0x10 */
} BattleFxKotrScatterState;
#pragma pack(pop)

/* Draw packet: int at 0x04 copied from a record, shorts at 0x0A and 0x12 set to constants. */
typedef struct BattleFxDrawPacket {
    unsigned char pad_00[0x4];
    int color;
    unsigned char pad_08[0x2];
    short posY;
    unsigned char pad_0C[0x6];
    short height;
} BattleFxDrawPacket;

/* Spin-and-fade model state: per-axis angles with their step values, and three fade phase lengths. */
typedef struct BattleFxSpinFade {
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_06[0x2];
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_0E[0x2];
    short phase0Length;
    short phase1Length;
    short phase2Length;
    short variant;
} BattleFxSpinFade;

#pragma pack(push, 1)
typedef struct BattleSummonKotrOffscreenCenterState {
    short offscreenStateWord;
    unsigned char pad_2[0x10];
    int centerX;
    union {
        int centerY;
        short stepDelta;
    };
    int centerZ;
    unsigned char pad_1E[0x2];
} BattleSummonKotrOffscreenCenterState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrScriptState {
    short scriptState; /* +0x0 */
    short scriptReady; /* +0x2 */
} BattleSummonKotrScriptState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrActorAttachState {
    short actorIndex; /* +0x0 */
    short attachIndex; /* +0x2 */
} BattleSummonKotrActorAttachState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrTumbleXYState {
    short kotrTumbleStateX; /* +0x0 */
    short kotrTumbleStateY; /* +0x2 */
} BattleSummonKotrTumbleXYState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrOffscreenRequest {
    short m00; /* +0x0 */
    unsigned char pad_2[0xe];
    short summonKotrOffscreenStateWord; /* +0x10 */
    unsigned char pad_12[0x8];
    int summonKotrOffscreenNegatedOffset; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleSummonKotrOffscreenRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnightEndpointModel {
    int modelRequest; /* +0x0 */
    unsigned char pad_4[0xc];
    int summonKotrKnightEndpointMatrix; /* +0x10 */
    unsigned char pad_14[0x1c];
} BattleSummonKotrKnightEndpointModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrBurstSprite {
    int burstSpriteEntry; /* +0x0 */
    int summonKotrBurstSpriteResource; /* +0x4 */
    unsigned char pad_8[0x4];
    short summonKotrBurstSpriteFrame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSummonKotrBurstSprite;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrEasedSpinScaleDraw {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short summonKotrEasedSpinScaleTime; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonKotrEasedSpinScaleDraw;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight2SpinScaleModel {
    int spinScaleModel; /* +0x0 */
    unsigned char pad_4[0x1c];
    int summonKotrKnight2EndpointPointA; /* +0x20 */
    unsigned char pad_24[0x14];
    int summonKotrKnight2EndpointPointB; /* +0x38 */
    unsigned char pad_3c[0x14];
} BattleSummonKotrKnight2SpinScaleModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrImpactSpriteParticle {
    int impactSpriteEntry; /* +0x0 */
    int summonKotrImpactSpriteResource; /* +0x4 */
    unsigned char pad_8[0x4];
    short summonKotrImpactSpriteFrame; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSummonKotrImpactSpriteParticle;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrSpinGrowModel {
    int spinGrowModelResource; /* +0x0 */
    int summonKotrSpinGrowModelFlags; /* +0x4 */
    unsigned char pad_8[0x2];
    short summonKotrSpinGrowModelScale; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSummonKotrSpinGrowModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrImpactSpriteRing {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int summonKotrImpactRingResource; /* +0x4 */
    unsigned char pad_8[0x4];
    short summonKotrImpactRingFieldWord; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSummonKotrImpactSpriteRing;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight4SpinScaleModel {
    int spinScaleModel; /* +0x0 */
    unsigned char pad_4[0x1c];
    int summonKotrKnight4EndpointPoint; /* +0x20 */
    unsigned char pad_24[0x14];
} BattleSummonKotrKnight4SpinScaleModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight5SpinScaleModel {
    int spinScaleModel; /* +0x0 */
    unsigned char pad_4[0x1c];
    int summonKotrKnight5EndpointPoint; /* +0x20 */
    unsigned char pad_24[0x14];
} BattleSummonKotrKnight5SpinScaleModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight6SpinScaleModel {
    int spinScaleModel; /* +0x0 */
    unsigned char pad_4[0x1c];
    int summonKotrKnight6EndpointPoint; /* +0x20 */
    unsigned char pad_24[0x14];
} BattleSummonKotrKnight6SpinScaleModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight7ExpandingModel {
    int expandingModelResource; /* +0x0 */
    int summonKotrExpandFadeFlags; /* +0x4 */
    unsigned char pad_8[0x2];
    short summonKotrExpandFadeAlpha; /* +0xa */
    unsigned char pad_c[0x4];
    int summonKotrKnight7RotationMatrix; /* +0x10 */
    unsigned char pad_14[0x4];
    int summonKotrKnight7EndpointPointA; /* +0x18 */
    unsigned char pad_1c[0x14];
    int summonKotrKnight7EndpointPointB; /* +0x30 */
    unsigned char pad_34[0x14];
} BattleSummonKotrKnight7ExpandingModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight8SpinScaleModel {
    int spinScaleModel; /* +0x0 */
    unsigned char pad_4[0x1c];
    int summonKotrKnight8EndpointPointA; /* +0x20 */
    unsigned char pad_24[0x14];
    int summonKotrKnight8EndpointPointB; /* +0x38 */
    unsigned char pad_3c[0x14];
} BattleSummonKotrKnight8SpinScaleModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrTumblingModel {
    int tumblingModel; /* +0x0 */
    unsigned char pad_4[0xc];
    short summonKotrTumbleShrinkBaseX; /* +0x10 */
    short kotrTumbleBasePosY; /* +0x12 */
    short kotrTumbleBasePosZ; /* +0x14 */
    unsigned char pad_16[0x2];
} BattleSummonKotrTumblingModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight9BillboardModel {
    int billboardModel; /* +0x0 */
    unsigned char pad_4[0xc];
    int kotrKnight9RotMatrix; /* +0x10 */
    unsigned char pad_14[0x4];
    int kotrKnight9TrailEndpoint; /* +0x18 */
    unsigned char pad_1c[0x14];
    int kotrKnight9TrailEndpointB; /* +0x30 */
    unsigned char pad_34[0x14];
} BattleSummonKotrKnight9BillboardModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight10SpinScaleModel {
    int spinScaleModel; /* +0x0 */
    unsigned char pad_4[0x1c];
    int kotrKnight10RotMatrix; /* +0x20 */
    unsigned char pad_24[0x4];
    int kotrKnight10TrailEndpoint; /* +0x28 */
    unsigned char pad_2c[0x14];
    int kotrKnight10TrailEndpointB; /* +0x40 */
    unsigned char pad_44[0x14];
} BattleSummonKotrKnight10SpinScaleModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight11SpinScaleModel {
    int spinScaleModel; /* +0x0 */
    unsigned char pad_4[0x1c];
    int kotrKnight11SpinScaleRecord; /* +0x20 */
    unsigned char pad_24[0x4];
    int kotrKnight11TrailEndpoint; /* +0x28 */
    unsigned char pad_2c[0x14];
} BattleSummonKotrKnight11SpinScaleModel;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnight12ScaleModelPair {
    int spinScaleModel; /* +0x0 */
    unsigned char pad_4[0x1c];
    int kotrKnight12RotMatrix; /* +0x20 */
    unsigned char pad_24[0x4];
    int kotrKnight12TrailEndpoint; /* +0x28 */
    unsigned char pad_2c[0x14];
} BattleSummonKotrKnight12ScaleModelPair;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrFadingScrollState {
    short scrollOffset; /* +0x0 */
    unsigned char pad_2[0xa];
    char kotrFadeScrollOldest; /* +0xc */
    char kotrFadeScrollMiddle; /* +0xd */
    char kotrFadeScrollNewest; /* +0xe */
    unsigned char pad_f[0x19];
} BattleSummonKotrFadingScrollState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleKotrSpinPairData {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short kotrSpinPairAngle; /* +0xa */
    unsigned char pad_c[0x4];
    int layerFade0; /* +0x10 */
    unsigned char pad_14[0x30];
    int layerFade1; /* +0x44 */
    unsigned char pad_48[0x30];
    int layerFade2; /* +0x78 */
    unsigned char pad_7c[0x30];
    int layerFade3; /* +0xac */
    unsigned char pad_b0[0x30];
    int layerFade4; /* +0xe0 */
    unsigned char pad_e4[0x34];
    int layerFade5; /* +0x118 */
    unsigned char pad_11c[0x64];
    int layerFade6; /* +0x180 */
    unsigned char pad_184[0x34];
} BattleKotrSpinPairData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrKnightComposedPos {
    short composedPosX;
    short composedPosY;
    short composedPosZ;
    unsigned char pad_6[0xca];
} BattleSummonKotrKnightComposedPos;
#pragma pack(pop)

/* The knight context record (BattleKotrKnight, 0x84 bytes) seen from its composedPos members, the way
 * BattleFx_KotrAttachPointToLocal indexes it from g_BattleSummonKotrKnightComposedPos. */
typedef struct BattleKotrKnightComposedPos {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x7e];
} BattleKotrKnightComposedPos;

#pragma pack(push, 1)
typedef struct BattleSummonKotrTumbleEmitterInit {
    unsigned char flags; /* +0x0 */
    unsigned char pad_1[0x5];
    short kotrScriptCountEntry; /* +0x6 */
} BattleSummonKotrTumbleEmitterInit;
#pragma pack(pop)

/* An actor attachment matrix and the transposed effect transform, side by side in one stack buffer. */
typedef struct BattleKotrAttachMatrices {
    PsxMatrix attach;
    unsigned char pad_1E[0x2];
    PsxMatrix transposed;
    unsigned char pad_3E[0x2];
} BattleKotrAttachMatrices;

#pragma pack(push, 1)
typedef struct BattleSummonKotrPulseTriangleV0 {
    unsigned char pad_0[0x2];
    short kotrTriangleOffsetNeg; /* +0x2 */
} BattleSummonKotrPulseTriangleV0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSummonKotrPulseTriangleV2 {
    unsigned char pad_0[0x2];
    short kotrTriangleOffsetSin; /* +0x2 */
} BattleSummonKotrPulseTriangleV2;
#pragma pack(pop)

#endif /* FF7_TYPES_BATTLE_BOSS_H */

/* include/ff7/types/snobo.h: the types ff7/snobo TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_SNOBO_H
#define FF7_TYPES_SNOBO_H
#include "lib/types/graphics.h"

#ifdef __cplusplus
struct SnowboardHud {
    unsigned char pad_00[0x4];
    int fontModel;     // SnowboardRenderer::fontModel
    int menuFontModel; // SnowboardRenderer::menuFontModel
    void updateHud();
    void renderTime(unsigned int, short, short);
    void renderRecord(int, short, int);
    void renderSpeed(short, short, short);
    void selectList(short, short);
};

typedef int (__cdecl *SnowboardOpcode)(int);
typedef void (__cdecl *SnowboardAction)(int, int, int);
typedef void (__fastcall *SnoboFcIntIntFloat)(int self, int a, int b, float c);
typedef int (__cdecl *SnoboSpawnFn)(short *pos, int rec, int gte);
typedef short (__cdecl *SnoboShortOfInt)(int arg);
#endif

typedef struct Copy16 {
    int v[4];
} Copy16;

typedef struct Copy30 {
    short v[15];
} Copy30;

/* One snowboard effect node (0x90 bytes): 32 at g_SnoboEffectRecords (Mini_SnowboardAllocEffectNode)
   and 32 at g_SnoboAltEffectRecords (Mini_SnowboardFindFreeEffectRecord). The allocator sets +0x00..+0x03,
   copies a 72-byte SnowboardEffectTemplate (g_SnoboEffectDef and its neighbours) over +0x04..+0x4B and clears the
   velocities and the counters. Every frame Mini_SnowboardUpdateEffectRecords drops a node whose section
   is behind the rider's (unless active is 2), runs its update callback, rebuilds the rotation matrix when
   matrixValid is 0, appends the render records for model and model2, and tests the rider against the
   contact cylinder (contactRadius, contactTop, contactSplit), setting contact and choosing one of the
   four contactResponse bytes. The words from +0x3C to +0x48 and the counters at +0x84..+0x88 belong to
   the kind of effect: each name in their unions is one kind's use of the slot. */
struct SnowboardEffect;
typedef void(__cdecl *SnowboardEffectUpdate)(struct SnowboardEffect *);
#pragma pack(push, 2)
typedef struct SnowboardEffect {
    /* +0x00 */ char active;              /* 0 free, 1 active, 2 also kept when it is behind the rider's section */
    /* +0x01 */ unsigned char contact;    /* bit 0: the rider touched it; bit 1: at speed (state +0x15C above 0x100) */
    /* +0x02 */ short section;            /* the course section; a hit posts it as g_SnoboSignalValue */
    /* +0x04 */ short model;              /* first model id drawn (0: none) */
    /* +0x06 */ short model2;             /* second model id drawn (0: none) */
    /* +0x08 */ short matrixShift;        /* the rotation matrix's elements are shifted left by this */
    /* +0x0A */ short pad_0a;
    /* +0x0C */ union { int renderArg0; int isFadingIn; int isFalling; };  /* both copied to the render record */
    /* +0x10 */ union { int renderArg1; int fadeAmount; int packedPhase; int packedFlags; };
    /* +0x14 */ short contactRadius;      /* 0: no contact test */
    /* +0x16 */ short contactTop;         /* the cylinder reaches this far above height */
    /* +0x18 */ short contactSplit;       /* above height - contactSplit the second response applies */
    /* +0x1A */ unsigned char contactResponse[4]; /* [split + 2 * at speed]; 0xFF in [0]: flag the contact only */
    /* +0x1E */ short rotX;
    /* +0x20 */ short rotY;
    /* +0x22 */ short rotZ;
    /* +0x24 */ short matrixValid;        /* 0: rebuild the matrix at +0x4C from the rotation */
    /* +0x26 */ int sidePos;
    /* +0x2A */ int height;
    /* +0x2E */ int forward;
    /* +0x32 */ int heightLimit;          /* the height the falling kinds land at */
    /* +0x36 */ short pad_36;
    /* +0x38 */ SnowboardEffectUpdate update;
    /* +0x3C */ union { int homeX; int orbitX; int linked; int forwardLimit; int mode; int targetX; int state; int variant; int pickupKind; int offsetPos; short angleStep; };
    /* +0x40 */ union { int homeY; int orbitZ; int triggerPos; int linkedA; int swingShift; };
    /* +0x44 */ union { int homeZ; int orbitY; int restY; int limit; int countdown; int linkedB; int holdTimer; int stepShift; };
    /* +0x48 */ union { int cycleCounter; int direction; int spinIndex; short spinRate; short timerReload; };
    /* +0x4C */ Copy30 matrix;            /* the rotation matrix Mini_SnowboardUpdateEffectRecords caches */
    /* +0x6A */ int sideVelocity;
    /* +0x6E */ int fallVelocity;
    /* +0x72 */ int forwardVelocity;
    /* +0x76 */ int reversed;             /* non-zero turns the spin of the cycling kinds the other way */
    /* +0x7A */ short pad_7a;
    /* +0x7C */ int hitVelocityX;         /* the rider's velocity when the contact was at speed */
    /* +0x80 */ int hitVelocityZ;
    /* +0x84 */ union { short age; short sideFlag; short isLatched; short angle; short accel; short enabled; };
    /* +0x86 */ union { short timer; short landed; short started; short spinning; short launched; };
    /* +0x88 */ union { short counter; short phaseOffset; short bounceCount; short spinVelocity; };
    /* +0x8A */ union { unsigned short fade; short waitState; short stepIndex; short startState; }; /* non-zero blinks the node, 0xFFFF hides it */
    /* +0x8C */ unsigned short blinkCounter;
    /* +0x8E */ short pad_8e;
} SnowboardEffect;

/* An effect template (72 bytes): what Mini_SnowboardAllocEffectNode and
   Mini_SnowboardAllocEffectNodeFromTemplate copy over a new node's +0x04..+0x4C. Each member is the
   node's member 4 bytes further on (the unions too). A trigger fills a template's model, rotY and
   position from its descriptor and the object, then allocates a node from it. */
typedef struct SnowboardEffectTemplate {
    /* +0x00 */ short model;
    /* +0x02 */ short model2;
    /* +0x04 */ short matrixShift;
    /* +0x06 */ short pad_06;
    /* +0x08 */ union { int renderArg0; int isFadingIn; int isFalling; };
    /* +0x0C */ union { int renderArg1; int fadeAmount; int packedPhase; int packedFlags; };
    /* +0x10 */ short contactRadius;
    /* +0x12 */ short contactTop;
    /* +0x14 */ short contactSplit;
    /* +0x16 */ unsigned char contactResponse[4];
    /* +0x1A */ short rotX;
    /* +0x1C */ short rotY;
    /* +0x1E */ short rotZ;
    /* +0x20 */ short matrixValid;
    /* +0x22 */ int sidePos;
    /* +0x26 */ int height;
    /* +0x2A */ int forward;
    /* +0x2E */ int heightLimit;
    /* +0x32 */ short pad_32;
    /* +0x34 */ SnowboardEffectUpdate update;
    /* +0x38 */ union { int homeX; int orbitX; int linked; int forwardLimit; int mode; int targetX; int state; int variant; int pickupKind; int offsetPos; short angleStep; };
    /* +0x3C */ union { int homeY; int orbitZ; int triggerPos; int linkedA; int swingShift; };
    /* +0x40 */ union { int homeZ; int orbitY; int restY; int limit; int countdown; int linkedB; int holdTimer; int stepShift; };
    /* +0x44 */ union { int cycleCounter; int direction; int spinIndex; short spinRate; short timerReload; };
} SnowboardEffectTemplate;
#pragma pack(pop)

typedef struct Copy64 {
    int v[16];
} Copy64;

typedef void(__cdecl *SnowboardCallback)(int, int);

/* Three-dword snowboarder world/relative position copy block. */
typedef struct SnowboardVec3Copy {
    int x;
    int y;
    int z;
} SnowboardVec3Copy;

/* A tagged snowboard effect's spawn record (Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ and
   ...XZAndField76): the effect's position in 20.12 (the template's sidePos, height and forward), the
   template's two home words (+0x38/+0x3C), then the new node's side and forward velocity and its
   `reversed` flag (read by the Field76 variant only). */
typedef struct SnowboardEffectSpawn {
    int sidePos;
    int height;
    int forward;
    int homeX;
    int homeY;
    int sideVelocity;
    int forwardVelocity;
    int reversed;
} SnowboardEffectSpawn;

/* Parameters for spawning a pair of linked snowboard effects. */
typedef struct SnoboLinkedEffectSpec {
    short flags;
    short pad_02;
    short pad_04;
    short pad_06;
    short startTimer;
    short swingShift;
    short stepShift;
    short flag;
} SnoboLinkedEffectSpec;

/* 3x3 matrix of 16-bit elements for the snowboard PSX-style transforms, stored row-major. */
typedef struct SnoboMatrix {
    short m00;
    short m01;
    short m02;
    short m10;
    short m11;
    short m12;
    short m20;
    short m21;
    short m22;
} SnoboMatrix;

/* Three 16-bit vector components used as the input of the snowboard matrix transforms. */
typedef struct SnoboVector {
    short x;
    short y;
    short z;
} SnoboVector;



/* One row of a 3x3 float rotation matrix built by SetRotationMatrix4x4FromAngles. */
typedef struct SnowboardFloatRowB {
    float col0;
    float col1;
    float col2;
    unsigned char pad_0C[0x4];
} SnowboardFloatRowB;



/* Snowboard course-header entry; its start position is the midpoint of each axis pair. */
typedef struct SnowboardCourseEntry {
    unsigned char pad_00[0xe];
    short x0;
    unsigned char pad_10[0xe];
    short x1;
    unsigned char pad_20[0x2];
    short y0;
    unsigned char pad_24[0x2];
    short z0;
    unsigned char pad_28[0xa];
    short height;
    unsigned char pad_34[0x2];
    short z1;
    unsigned char pad_38[0x11a];
    short startHeight;
    unsigned char pad_154[0x1e];
    short y1;
    unsigned char pad_174[0xe];
    short heightOffset;
} SnowboardCourseEntry;

/* A recorded snowboard run (16 bytes, g_SnoboCourseRecords.runs): runs[0] records the run being
 * ridden (frameIndex counts its 15-byte frames), Mini_SnowboardStoreRunSamplesToSlot copies it
 * into the stage's run with its frame count and course time, and Mini_SnowboardEmitAnimFrameActions
 * plays the stage's run back frame by frame. A courseTime of 0x99999999 is "no time yet". */
typedef struct SnowboardRunRecord {
    short enabled; /* +0x0 */
    short frameIndex; /* +0x2 */
    short frameCount; /* +0x4 */
    unsigned char pad_06[0x2];
    int frames; /* +0x8: the frame buffer */
    int courseTime; /* +0xC */
} SnowboardRunRecord;

/* Four-word sprite record copied into the snowboard effect definition. */
typedef struct SnowboardSpriteRecord {
    int sidePos;
    int anchorPos;
    int forward;
    int anchorOffset;
} SnowboardSpriteRecord;





/* A spawn position (20.12 fixed point): Mini_SnowboardSpawnEffectNodeTemplateF8 and ...TemplateWithId
   copy it into the effect template's sidePos, height (and heightLimit, 0x40 above) and forward. */
typedef struct SnowboardSpawnPos {
    int vecX;
    int vecY;
    int vecZ;
} SnowboardSpawnPos;

/* Effect record for the selector-offset snowboard spawn, copied into a snowboard effect definition. */
typedef struct SnowboardSelectorEffect {
    int baseValueA;
    int selectorOffset;
    int baseValueB;
} SnowboardSelectorEffect;

/* Three-int effect position from the effect position table, offset and shifted to fixed point before storing. */
typedef struct SnowboardEffectPos {
    int posX;
    int posY;
    int posZ;
} SnowboardEffectPos;


/* Descriptor for a raised snowboard effect spawned on a fall; its Y is lowered by fallHeight. */
typedef struct SnowboardRaisedEffect {
    unsigned char pad_00[0x8];
    short fallHeight;
    short selector;
    short noFallWhenNear;
} SnowboardRaisedEffect;

/* Snowboard course vertex (8 bytes): x, y (height) and z as shorts. */
typedef struct SnowboardTriangleVertex {
    short posX;
    short posY;
    short posZ;
    short bound; /* +0x06: in a course block, vertices 0-2 hold its X band bounds, 3-5 its Z band
                  * bounds and 6 the triangle search radius here; the other vertices leave it unused */
} SnowboardTriangleVertex;

/* One triangle of a course block: three indices into the block's vertices, then flag bits. */
typedef union SnowboardCourseFace {
    int word; /* the record as one word (SnowboardProbe.triangleEntry) */
    unsigned char vertexIndex[4];
} SnowboardCourseFace;

/* A 0x374-byte course block. SnowboardRaceState.courseHeader is an array of them and every node of a
 * segment names one (SnowboardSegment.blockIndexTable). Its 49 vertices are seven rows of seven; the
 * last row (42..48) is the edge the next block's first row meets. */
typedef struct SnowboardCourseBlock {
    short kind;         /* +0x000: 3 when the next row continues in the segment linkSegmentA names */
    short cameraPitch;  /* +0x002: the follow camera aims its pitch at three times this */
    short turn;         /* +0x004: the block's curve (Mini_SnowboardComputeHeading) */
    short headingRange; /* +0x006 */
    SnowboardTriangleVertex vertices[0x31]; /* +0x008 */
    int colors[0x31];                       /* +0x190: a packed colour per vertex */
    SnowboardCourseFace faces[0x48];        /* +0x254 */
} SnowboardCourseBlock;
typedef char CheckSnowboardCourseBlock[(sizeof(SnowboardCourseBlock) == 0x374) ? 1 : -1];

/* Three-int vector loaded as the snowboard origin by Mini_SnowboardLoadSegmentVector. */
typedef struct SnowboardVec {
    int x;
    int y;
    int z;
} SnowboardVec;



/* The engine context's cameraMatrix, a 4x4 float view matrix: only its translation row is named. */
typedef struct SnowboardCameraMatrix {
    unsigned char pad_00[0x30];
    float offsetX;
    float offsetY;
    float offsetZ;
} SnowboardCameraMatrix;









/* Transform whose two words are copied into a rotated snowboard effect copy. */
typedef struct SnowboardEffectTransform {
    unsigned char pad_00[0x20];
    int velocityX;
    unsigned char pad_24[0x4];
    int velocityZ;
} SnowboardEffectTransform;

/* Kind 2 effect descriptor whose fixed offset is added to an effect node position. */
typedef struct SnowboardKind2EffectDesc {
    unsigned char pad_00[0x8];
    short swayPhase;
    short fixedOffset;
} SnowboardKind2EffectDesc;



/* Sound dispatch block of the snowboard race state: command id, pitch and parameters. */
typedef struct SnowboardSoundCommand {
    short command;
    unsigned char pad_02[0x2];
    int volume;
    int pitch;
    int param3;
    int param4;
    int param5;
} SnowboardSoundCommand;


/* A 32-byte course segment record (SnowboardRaceState.segmentTable): its node count, the segments it
 * links to and one entry per node in each table. */
typedef struct SnowboardSegment {
    short nodeCount;
    char linkedBlock; /* +0x02 */
    unsigned char retreatSegment; /* +0x03: the segment a probe steps back into before node 0 (0xFF: none) */
    char linkSegmentA; /* +0x04: the segment a probe moves into past the last node, or a branch */
    char linkSegmentB; /* +0x05: the second branch */
    unsigned char pad_06[0x2];
    unsigned char *blockIndexTable; /* +0x08: per node, the index of its 0x374-byte course block */
    unsigned char *chainBitTable;   /* +0x0C: per node, flag bits */
    unsigned char *chainIndexTable; /* +0x10: per node, the chain the course mesh emits */
    int *actionTable; /* +0x14: one action word per node (Mini_SnowboardEmitCourseRecordActions) */
    short *angleTable; /* +0x18: per node, its rotation about Y */
    SnowboardVec *vectors; /* +0x1C: per node, its origin */
} SnowboardSegment;


/* Snowboard turn state values passed to the turn-asset reset and interpolate calls. */
typedef struct SnowboardTurnState {
    int opcodeBase; /* the course's opcode block, 64 bytes per rider action */
    int resetValue;
    int brakeFrom;
    int leftTurnValue;
    int rightTurnValue;
    int brakeTo;
} SnowboardTurnState;


/* A course background strip as a PSX gouraud quad (POLY_G4): one colour and one screen point per
 * corner. SnowboardRenderer::buildCourse fills it; SnowboardRenderer::strip scales it to the
 * screen. */
typedef struct SnowboardStripVertexSrc {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, pad1;
    short x1, y1;
    unsigned char r2, g2, b2, pad2;
    short x2, y2;
    unsigned char r3, g3, b3, pad3;
    short x3, y3;
} SnowboardStripVertexSrc;

/* A course background tile as a PSX sprite (SPRT): its screen position, the texture u that picks
 * the background page, and its size, scaled to the 640x480 screen by SnowboardRenderer::prim. */
typedef struct SnowboardScreenQuadSrc {
    unsigned char pad_00[0x8];
    short x;
    short y;
    unsigned char u;
    unsigned char pad_0D[0x3];
    short w;
    short h;
} SnowboardScreenQuadSrc;

/* The segment table (SnowboardRaceState.segmentTable) as the effect gates read it: recordFlag and
 * effectGate are segmentTable[2].linkedBlock and segmentTable[4].linkedBlock. */
typedef struct SnowboardRaceEffectState {
    unsigned char pad_00[0x42];
    char recordFlag;
    unsigned char pad_43[0x3f];
    char effectGate;
} SnowboardRaceEffectState;

/* Four-byte color entry read into the snowboard color bits. */
typedef struct SnowboardColorEntry {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
} SnowboardColorEntry;

/* Best-record value stored as four bytes in the savemap. */
typedef struct SnowboardRecordBytes {
    char loByte;
    char midLoByte;
    char midHiByte;
    char hiByte;
} SnowboardRecordBytes;




/* One sky band of the course background (g_SnoboHudShorts): its top relative to the horizon, its
 * height and the colours of its top and bottom edges. */
typedef struct SnowboardCourseDesc {
    short top;
    short height;
    int topColor;
    int bottomColor;
} SnowboardCourseDesc;




/* Snowboard animation action block filled from an animation frame's action bytes. */
typedef struct SnowboardAnimActionBlock {
    unsigned char pad_00[0x10];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_16[0x2a];
    Copy30 matrix;    /* +0x40: a PsxMatrix (rotation, then translation at +0x52) */
    Copy30 yawMatrix; /* +0x5E: the yaw rotation multiplied into matrix */
} SnowboardAnimActionBlock;

/* The scratch block (Gfx_GetPsxScratchpadPtr(0)) the hit-launch and drift handlers rotate a step in:
 * the yaw matrix, the rotated step as longs (x, y, forward) and the short step vector fed to it. */
#pragma pack(push, 2)
typedef struct SnowboardEffectScratch {
    unsigned char pad_00[0x44];
    SnoboMatrix yawMatrix;
    int rotatedX;
    int rotatedY;
    int rotatedForward;
    SnoboVector step;
} SnowboardEffectScratch;
#pragma pack(pop)


/* Snowboard effect node allocated with offsets from its effect descriptor. */
typedef struct SnowboardOffsetsEffectNode {
    unsigned char pad_00[0x44];
    int offsetPos;
    int spinRate;
    unsigned char pad_4C[0x3c];
    short phaseOffset;
} SnowboardOffsetsEffectNode;






/* Snowboard vertical motion record: frame counter, vertical position, velocity and acceleration. */
typedef struct SnowboardVerticalMotion {
    unsigned char pad_00[0x12];
    short counter;
    unsigned char pad_14[0x4];
    int limitY;
    unsigned char pad_1C[0xc];
    float posY;
    unsigned char pad_2C[0x8];
    float velocityY;
    unsigned char pad_38[0x4];
    float accelY;
} SnowboardVerticalMotion;


/* Race time split into display digits: minutes, seconds and hundredths. */
typedef struct SnowboardTimeDigits {
    unsigned int minuteTens;
    unsigned int minuteUnits;
    unsigned int secondTens;
    unsigned int secondUnits;
    unsigned int fractionHundreds;
    unsigned int fractionTens;
    unsigned int fractionUnits;
} SnowboardTimeDigits;

/* Records of three packed colours and three 16-bit values used when colouring campaign batches. */
typedef struct SnowboardTmdColoredTri {
    unsigned char pad_00[0x4];
    unsigned int color0;
    unsigned int color1;
    unsigned int color2;
    unsigned short index0;
    unsigned short index1;
    unsigned short index2;
} SnowboardTmdColoredTri;

/* Buffer of packed digit-glyph index bytes written by the snowboard number renderer. */
typedef struct SnowboardDigitBuffer {
    char digit0;
    char digit1;
    unsigned char pad_02[0x1];
    char digit2;
    char digit3;
    unsigned char pad_05[0x1];
    char digit4;
    char digit5;
    char glyphIndex;
} SnowboardDigitBuffer;

/* Seven integer digit values that are offset by a base and packed into a buffer. */
typedef struct SnowboardDigitSet {
    int digit0;
    int digit1;
    int digit2;
    int digit3;
    int digit4;
    int digit5;
    int glyphIndex;
} SnowboardDigitSet;

/* Bone definition copied into matrix state: three sources, two shorts, a float and a flag. */
typedef struct SnowboardBoneDef {
    int sourceA;
    int sourceB;
    int sourceC;
    short parentA;
    short parentB;
    short length;
    short enabled;
} SnowboardBoneDef;

/* Clipper input vertex (16 bytes): x, y, the clip value and w. */
typedef struct SnowboardClipInputVertex {
    /* clip() and clipPositions() compute with the floats; emitQuad copies x, y, z and w as ints */
    union { float x; int xBits; }; /* +0x00 */
    union { float y; int yBits; }; /* +0x04 */
    union { float clipZ; int zBits; }; /* +0x08: clipped below 0.5 */
    union { float w; int wBits; }; /* +0x0C */
} SnowboardClipInputVertex;




/* Shaded primitive record of three integers and three shorts copied into the frame. */
typedef struct SnowboardShadedRecord {
    unsigned char pad_00[0x4];
    int color0;
    int color1;
    int color2;
    unsigned short index0;
    unsigned short index1;
    unsigned short index2;
} SnowboardShadedRecord;

/* Textured primitive record with three UV indices and three shorts copied into the frame. */
typedef struct SnowboardTexturedRecord {
    unsigned char pad_00[0x4];
    unsigned short uvA;
    unsigned char pad_06[0x2];
    unsigned short uvB;
    unsigned char pad_0A[0x2];
    unsigned short uvC;
    unsigned char pad_0E[0x6];
    unsigned short indexA;
    unsigned short indexB;
    unsigned short indexC;
} SnowboardTexturedRecord;

/* Triangle record with three packed vertex colours and three vertex indices. */
typedef struct SnowboardTriangleRecord {
    unsigned char pad_00[0x4];
    unsigned int colorA;
    unsigned int colorB;
    unsigned int colorC;
    unsigned short indexA;
    unsigned short indexB;
    unsigned short indexC;
} SnowboardTriangleRecord;






/* Float position vector written relative to the rider and passed to the action frame. */
typedef struct SnowboardRelativePosition {
    float posX;
    float posY;
    float posZ;
} SnowboardRelativePosition;

/* Position pair whose angle feeds the snowboard heading computation. */
typedef struct SnowboardHeadingPos {
    short offsetX;
    unsigned char pad_02[0x2];
    short offsetZ;
} SnowboardHeadingPos;

/* Offsets added to an object position when filling an effect's position. */
typedef struct SnowboardEffectPosOffset {
    short tag;
    short offsetX;
    short offsetY;
    short offsetZ;
} SnowboardEffectPosOffset;

/* A snowboard effect descriptor (16 bytes): the effect's yaw and offset from the object, which
   Mini_SnowboardFillEffectPosFromObject adds to the object's position, and up to four parameters
   the trigger's spawner reads at +8..+0xE (the model offset the gated spawners add to 0xE6/0xB2,
   the pickup kind, the raised effect's fall height and selector, a linked pair's flags). */
typedef struct SnowboardEffectDesc {
    SnowboardEffectPosOffset pos;
    short param[4];
} SnowboardEffectDesc;

/* The 10-byte form of SnowboardEffectDesc: the effect's yaw and offset from the object and the one
   parameter the trigger's spawner reads at +8 (the model offset Mini_SnowboardSlot*QueueGatedEffect and
   Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_* add to 0xB2/0xAE, the mode, the pickup kind, the tile flag). A trigger that
   spawns two effects keeps its two descriptors back to back at this stride. */
typedef struct SnowboardCompactEffectDesc {
    SnowboardEffectPosOffset pos;
    short param;
} SnowboardCompactEffectDesc;

/* Descriptor of a snowboard effect spawn with offsets applied to the node position and fields:
   the effect's yaw and offset from the object, then the height (in 0x1000 units) subtracted from
   the node's y, the spin rate and the node's +0x88 word (Mini_SnowboardSpawnVerticalBobEffect).
   The paired triggers keep two of them back to back (14-byte stride). */
typedef struct SnowboardEffectOffsetDesc {
    SnowboardEffectPosOffset pos;
    short heightUnits;
    short spinRate;
    short phaseOffset;
} SnowboardEffectOffsetDesc;

/* Two-byte snowboard script instruction: the opcode and one operand byte (read signed or unsigned). */
typedef struct SnowboardByteOp {
    unsigned char opcode;
    union {
        unsigned char operand;
        signed char signedOperand;
    };
} SnowboardByteOp;

/* Opcode operand that arms a resume slot with a value. */
typedef struct SnowboardResumeSlotOp {
    unsigned char pad_00[0x1];
    unsigned char slot;
    char value;
} SnowboardResumeSlotOp;

/* Float angles used to build a rotation matrix. */
typedef struct SnowboardRotationAngles {
    float angleX; /* +0x0 */
    float angleY;
    float angle;
} SnowboardRotationAngles;


/* Packed texture coordinate tag with a cache word. */
typedef struct SnowboardTexTag {
    unsigned char pad_00[0x4];
    unsigned short uvPacked;
    unsigned char pad_06[0x4];
    unsigned short cacheWord;
} SnowboardTexTag;

/* Float course triangle normal written from three frame values. */
typedef struct SnowboardCourseNormal {
    float normalX;
    float normalY;
    float normalZ;
} SnowboardCourseNormal;

/* Course probe whose shorts at 0x10 to 0x14 hold a cross-product normal. */
typedef struct SnowboardNormalScratch {
    unsigned char pad_00[0x10];
    short normalX;
    short normalY;
    short normalZ;
} SnowboardNormalScratch;

/* Course action record (0x4C bytes): block index, node index, vertex base and the node's matrix. */
typedef struct SnowboardCourseActionRecord {
    short blockIndex;
    short nodeIndex;
    unsigned char pad_04[0x4];
    SnowboardCourseBlock *vertexBase;
    Copy64 matrix; /* +0xC: the node's matrixBlock when the record was appended */
} SnowboardCourseActionRecord;

/* Float camera angles subtracted from a camera target pose. */
typedef struct SnowboardCameraAngles {
    float angleX;
    float angleY;
    float angleZ;
} SnowboardCameraAngles;


/* The draw-data block a flushed matrix record is copied into; its model id, animation frame and
   draw flags and the matrix feed the model draw call. */
typedef struct SnowboardModelRecord {
    int mode;
    int modelId;
    int animFrame;
    int drawFlags;
    unsigned char pad_10[0x540];
    Copy64 matrix;
} SnowboardModelRecord;

/* Short x/y/z offsets added to a base position when a cycling effect spawns. */
typedef struct SnowboardCyclingEffectOffset {
    short offsetX;
    short offsetY;
    short offsetZ;
} SnowboardCyclingEffectOffset;

/* Byte-coded instruction that sets the snowboard state and selects a course frame. */
typedef struct SnowboardSelectFrameOp {
    unsigned char pad_00[0x1];
    unsigned char state;
    unsigned char duration;
    unsigned char easing;
} SnowboardSelectFrameOp;

/* Rider record that picks a sound effect and volume from its fields. */
typedef struct SnowboardRiderSound {
    unsigned char pad_00[0xc];
    unsigned char soundIndex;
    unsigned char pad_0D[0x5];
    short soundState;
    short soundVolumeRaw;
} SnowboardRiderSound;

/* Matrix record whose translation shorts sit at 0x12, 0x16 and 0x1A. */
typedef struct SnowboardMatrix302 {
    unsigned char pad_00[0x12];
    short translationX;
    unsigned char pad_14[0x2];
    short translationY;
    unsigned char pad_18[0x2];
    short translationZ;
} SnowboardMatrix302;

/* Race sprite whose state at 0x12 and fade level at 0x14 pick its draw mode. */
typedef struct SnowboardRaceSprite {
    unsigned char pad_00[0x12];
    short state;
    short fadeLevel;
} SnowboardRaceSprite;

/* Point of two shorts used when interpolating the height inside a snowboard triangle. */
typedef struct SnowboardHeightPoint {
    short x;
    unsigned char pad_02[0x2];
    short z;
} SnowboardHeightPoint;

/* Spawn descriptor whose shorts at +0x08 and +0x0A are copied to the snowboard offsets. */
typedef struct SnowboardEffectSpawnDesc {
    unsigned char pad_00[0x8];
    short offsetA;
    short offsetB;
} SnowboardEffectSpawnDesc;




/* Snowboard script opcode that waits on an input mask chosen by index. */
typedef struct SnowboardOpcodeWaitInput {
    unsigned char pad_00[0x1];
    unsigned char maskIndex;
    unsigned char invert;
} SnowboardOpcodeWaitInput;

/* Three-short rotation angles; this routine reads the Y and Z components. */
typedef struct SnowboardAngles {
    short angle; /* +0x0 */
    short angleY;
    short angleZ;
} SnowboardAngles;


/* Snowboard vertical motion fields past the shared header, read and written by the integrator. */
typedef struct SnowboardVerticalMotionTail {
    unsigned char pad_00[0x18];
    float limitY;
    unsigned char pad_1C[0xc];
    int posY;
    unsigned char pad_2C[0x8];
    int velocityY;
    unsigned char pad_38[0x4];
    int accelY;
} SnowboardVerticalMotionTail;


/* Snowboard probe block filled from the snowboard state; positions are 4096-based fixed point. */
typedef struct SnowboardProbe {
    short segment;
    short advanceCount;
    short triangleIndex;
    short opResult;
    short retryCount;
    unsigned char pad_0A[0x2];
    int edgeCross; /* +0x0C: the last cross product Mini_SnowboardFindCourseTriangleIndex formed */
    SnowboardSegment *courseTable;
    Copy30 matrix; /* +0x14: Mini_SnowboardFillCourseRecord's rotation of the course record */
    short deltaX;  /* +0x32: deltaX, the unused middle short and deltaZ are the vector the matrix rotates */
    unsigned char pad_34[0x2];
    short deltaZ;
    unsigned char pad_38[0x4];
    SnowboardCourseBlock *course;
    int p1X;
    unsigned char pad_44[0x4];
    int p1Z;
    unsigned char pad_4C[0x4];
    int p2X;
    unsigned char pad_54[0x4];
    int p2Z;
    unsigned char pad_5C[0x4];
    union {
        int triangleEntry; /* +0x60: the course's 4-byte triangle record, copied as one word */
        struct {
            unsigned char vertexIndexA; /* +0x60 */
            unsigned char vertexIndexB; /* +0x61 */
            unsigned char vertexIndexC; /* +0x62 */
            char stateFlags;            /* +0x63: published as SnowboardRaceState.probeFlags */
        };
    };
    int zSampleA;
    int zSampleB;
    int countDelta;
} SnowboardProbe;

/* Snowboard matrix record, 0x4C bytes, emitted for animation frames and course records. */
typedef struct SnowboardMatrixRecord {
    Copy64 matrix;
    short kind;
    short actionCode;
    int animFrame;
    int drawFlags;
} SnowboardMatrixRecord;

/* 15-byte packed snowboard rider sample; values are written as byte groups of one 32-bit word. */
typedef struct SnowboardRiderSample {
    char posXByte3;
    char posXByte2;
    char posXByte1;
    char posYByte3;
    char posYByte2;
    char posYByte1;
    char posZByte3;
    char posZByte2;
    char posZByte1;
    char angleAByte1;
    char angleAByte0;
    char angleBByte1;
    char angleBByte0;
    char angleCByte1;
    char angleCByte0;
} SnowboardRiderSample;

/* 15-byte packed snowboard animation frame: three 24-bit values, two shorts and a rotation. */
typedef struct SnowboardAnimFrame {
    unsigned char posXByte3;
    unsigned char posXByte2;
    unsigned char posXByte1;
    unsigned char posYByte3;
    unsigned char posYByte2;
    unsigned char posYByte1;
    unsigned char posZByte3;
    unsigned char posZByte2;
    unsigned char posZByte1;
    unsigned char angleXHi;
    unsigned char angleXLo;
    unsigned char rotationYHi;
    unsigned char rotationYLo;
    unsigned char angleZHi;
    unsigned char angleZLo;
} SnowboardAnimFrame;

/* Only the fields the code reads; the size is not known. */
typedef struct SnowboardStateRecord {
    unsigned char pad_00[0x4];
    short turn;
    short headingRange;
    unsigned char pad_08[0x18];
    short turnOffset;
    unsigned char pad_22[0x14];
    short riderRange;
    short centerX;
} SnowboardStateRecord;

/* Snowboard race state block reached through the global state pointer: rider position, heading, timers, score and course p */
typedef struct SnowboardRaceState {
    struct SnowboardCourseLoader *courseLoader;
    struct SnowboardCourseAssets *courseAssets;
    int courseIndex; /* 0 in event mode, else 1 or 2 by gSavemap.snowTempVar 2 or 8: the turn-state row */
    unsigned short tick;
    unsigned char pad_0E[0x2];
    union {
        struct {
            int courseTime; /* the current run's time; courseTime and bestTimes together are indexed by triggerStage */
            int bestTimes[3]; /* +0x14: the best time of each stage (the first entry of its rankings row) */
        };
        int stageTimes[4];
    };
    union {
        unsigned char bestScores[3]; /* +0x20: the best score of each stage */
        struct {
            unsigned char countA;
            unsigned char countB;
            unsigned char countC;
        };
    };
    unsigned char pad_23[0x1];
    union {
        int rankings[3][5]; /* +0x24: the five best times of each stage, 0 when unset */
        struct {
            int slotGate012;
            unsigned char pad_28[0x10];
            int slotGate219;
            unsigned char pad_3C[0x10];
            int slotGate425;
            unsigned char pad_50[0x10];
        };
    };
    unsigned int nowMs;  /* timeGetTime() at the last HUD update */
    int prevNowMs;            /* the previous nowMs (0 until the first update) */
    char triggerLatched;
    unsigned char pad_69[0x1];
    short displayedSpeed;
    short paused;
    short running;
    short timer;
    short resultPartA;
    short resultPartB;
    short resultPartC;
    short resultTotal;
    short spinCount;
    short pickupTotal;
    short triggerStage;
    short activeFlags;
    short pickupFlag;
    int originX;
    int originY;
    int originZ;
    unsigned char pad_90[0x4];
    float posX;
    float posY;
    float posZ;
    short snapshotIndex;
    short snapshotSection;
    short snapshotTriangle;
    unsigned char pad_A6[0x42];
    short segment;
    short chainIndex;
    short section;
    short triangle;
    short flags;
    short rotationY;
    short heading;
    short probeDeltaX;
    unsigned char pad_F8[0x2];
    short probeDeltaZ;
    unsigned char pad_FC[0x2];
    Copy30 headingMatrix;  /* +0xFE: Matrix30SetRotationY(flags) */
    Copy30 rotationMatrix; /* +0x11C: Matrix30SetRotationY(rotationY) */
    union {
        char probeFlags;
        unsigned char probeFlagBits;
    };
    unsigned char pad_13B[0x1];
    SnowboardSegment *segmentPtr;
    int inputFlags;
    int modeFlags;
    int motionFlags;
    unsigned char pad_14C[0x8];
    short riderState;
    short frameCount;
    unsigned char pad_158[0x2];
    short motionStage;
    short followAngle;
    unsigned char pad_15E[0x2];
    float terrainHeight;
    unsigned char pad_164[0x8];
    float relPosX;
    float relPosY;
    float riderPos;
    float velocityX;
    float fadeLevel;
    float velocityZ;
    int limitHigh;
    unsigned char pad_188[0x1c];
    float targetAngle0; /* the X and Z angles Mini_SnowboardDirectionToAngles derives from courseNormal */
    int targetAngleY;
    float targetAngle1;
    float currentAngle0;
    int currentAngleY;
    float currentAngle1;
    SnowboardCourseNormal courseNormal;
    unsigned char pad_1C8[0x4];
    int clearedRunWordA;
    float velocity;
    int clearedRunWordB;
    unsigned char pad_1D8[0x8];
    int limitLow;
    unsigned char pad_1E4[0xc];
    int clearedActionWordA;
    int clearedActionWordB;
    int clearedActionWordC;
    float turnMatrix[12]; /* rider turn matrix handed to SnowboardCourseAssets::transform */
    int savedInput0;
    int savedInput1;
    int savedInput2;
    unsigned char pad_238[0x4];
    SnowboardCourseBlock *courseHeader;
    unsigned char *blockIndexTable;
    int courseDataA;
    int effectPosTable;
    int courseDataB;
    int courseDataC;
    int courseDataD;
    SnowboardSegment *segmentTable;
    int sectionNames;
    short *sectionTable;
    short *blockTable;
    float frameTime;   /* frames' worth of elapsed time (Mini_SnowboardAccumulateFrameTime) */
    float framesRun;   /* state-callback runs so far; Mini_SnowboardUpdate runs until it catches up */
    int renderFrame;   /* 1 for the last callback run of an update, the one that is drawn */
    struct SnowboardClipper *clipper;
    SnowboardSoundCommand *soundCommand; /* the command block after this state, +0x27C */
} SnowboardRaceState;

/* Snowboard motion state: flags, speed, position and velocity on X and Z, with acceleration inputs. */
typedef struct SnowboardMotionState {
    int flags;
    unsigned char pad_04[0x8];
    unsigned short mode;
    unsigned char pad_0E[0x4];
    short frictionLevel;
    short speed;
    unsigned char pad_16[0xe];
    float posX;
    unsigned char pad_28[0x4];
    float posZ;
    float velocityX;
    float speedFloat;
    float velocityZ;
    int accelY;
    float directionX;
    int directionY;
    float directionZ;
    unsigned char pad_4C[0x28];
    float accelerationX;
    unsigned char pad_78[0x4];
    float accelerationZ;
} SnowboardMotionState;

/* Snowboard rider physics record: action code, speed and state. */
typedef struct SnowboardRiderPhysics {
    unsigned char pad_00[0xc];
    unsigned short actionCode;
    short frameCount;
    unsigned char pad_10[0x2];
    short stepCount;
    short speed;
    unsigned char pad_16[0x1a];
    float velocityX;
    unsigned char pad_34[0x4];
    float velocityZ;
    unsigned char pad_3C[0x30];
    int turnRate;
    unsigned char pad_70[0x10];
    int state;
} SnowboardRiderPhysics;

/* Snowboard rider input record: action code, turn limit, speed and heading. */
typedef struct SnowboardRiderControls {
    unsigned char pad_00[0xc];
    short actionCode;
    unsigned char pad_0E[0x2];
    short turnLimit;
    short turnStep;
    short speed;
    unsigned char pad_16[0x56];
    int turnRate;
    unsigned char pad_70[0x10];
    int actionMode;
    unsigned char pad_84[0x4];
    float heading;
} SnowboardRiderControls;


/* Snowboard state for the current course node: base position, node index and relative offset. */
#pragma pack(push, 2)
typedef struct SnowboardCourseNodeState {
    char active; /* Mini_SnowboardEmitCourseChain: activeFlags masked by g_SnoboActiveFlagMask */
    unsigned char pad_01[0x1];
    union {
        char raceRunning; /* the state's running flag, copied each frame; effects update only while it is set */
        char continuedSegment; /* Mini_SnowboardEmitCourseChain: the last row continued into another segment */
    };
    char chainFlag; /* cleared at the start of each emit pass */
    unsigned char pad_04[0x14];
    int baseX;
    int baseY;
    int baseZ;
    short nodeIndex; /* the effect update stores the rider's section here */
    short segmentIndex; /* +0x26: the current segment (the course mesh's colour-pair index) */
    short branchSegment; /* +0x28: a second segment to walk after the first, 0 for none */
    unsigned char pad_2A[0x4];
    short rowsLeft; /* +0x2E: the rows Mini_SnowboardEmitCourseChain may still emit */
    short savedRowsLeft; /* +0x30: rowsLeft when the branch segment was entered */
    short segment; /* +0x32: the state's segment */
    short chainIndex; /* +0x34: the state's chainIndex */
    short recordCount; /* +0x36: the records Mini_SnowboardAppendCourseRecord appended */
    SnowboardSegment *courseNodePtr; /* +0x38: the segment of the node being emitted */
    SnowboardCourseBlock *courseBlock; /* +0x3C: the node's course block */
    SnowboardVec *posPtr; /* +0x40: the node's origin in the segment's vectors */
    short rotation[9]; /* +0x44: PSX rotation matrix; with the translation, the 30 bytes an effect caches */
    int translateX;    /* +0x56 */
    int translateY;
    int translateZ;
    unsigned char pad_62[0xa];
    union {
        Copy64 matrixBlock; /* +0x6C: the float 4x4 matrix the records copy */
        struct {
            float worldMatrix[12]; /* the float 4x4 built from rotation and translation, rows 0-2 */
            float relX;            /* row 3 */
            float relY;
            float relZ;
        };
    };
    unsigned char pad_AC[0x40];
    SnowboardClipInputVertex vertices[0x31]; /* +0xEC: the block's vertices through the clipper; [42..48] is the edge row */
    unsigned char pad_3FC[0x20];
    SnowboardClipInputVertex edgeRow[13];    /* +0x41C: the edge row with a midpoint between each pair */
    int colors[0x31];      /* +0x4EC: a packed colour per vertex */
    int edgeColors[13];    /* +0x5B0: a colour per edgeRow entry */
} SnowboardCourseNodeState;
#pragma pack(pop)

/* Snowboard spin state: target and current spin rate, timer and accumulated angle. */
typedef struct SnowboardSpinState {
    unsigned char pad_00[0x4];
    short targetSpin;
    short spinRate;
    short timer;
    unsigned char pad_0A[0xa];
    short speed;
    unsigned char pad_16[0x72];
    float angle;
} SnowboardSpinState;



/* The rider record at g_SnoboStatePtr + 0x148 (0x8C bytes viewed), which every g_SnoboActionTable action receives. */
typedef struct SnowboardRiderState {
    int flags;                  /* +0x00: bit 0 set by ResetVelocity, ResetVelocityAlignYaw, TurnToTargetAngle */
    short turnImpulse;          /* +0x04: the spin target (BeginSpin* seed -0x80/+0x80) or the impact turn (ImpactTurn +-24, eased by 2) */
    short spinRate;             /* +0x06: the rate taken off the heading (TurnToTargetAngle ramps it by 8 up to 0x40) */
    short frameCountdown;       /* +0x08: the spin countdown (0x40 on entry) */
    short turnOffset;           /* +0x0A */
    union {
        unsigned short actionCode; /* +0x0C: g_SnoboActionTable index in the low byte, 0x8000 = entered */
        unsigned char actionIndex; /* +0x0C: the low byte, as ImpactTurn reads it */
    };
    unsigned char pad_0E[0x2];  /* +0x0E: the frame count (SnowboardRaceState.frameCount) */
    short turnAmount;           /* +0x10: the turn ramp 0..128 (TurnLeft/Right), the brake ramp 0..32 (Brake) */
    short airFrames;            /* +0x12: frames off the ground; 0 on landing (Mini_SnowboardIntegrateVerticalMotion) */
    short speed;                /* +0x14: compared with 0x80/0x100/0x400; the rider sound's volume is speed >> 3 */
    unsigned char pad_16[0x1a];
    float velocityX;            /* +0x30: the velocity, rotated about Y, damped, zeroed */
    unsigned char pad_34[0x4];  /* +0x34: velocity Y (SnowboardVerticalMotion) */
    float velocityZ;            /* +0x38 */
    unsigned char pad_3C[0x30];
    float turnRate;             /* +0x6C: the extra yaw of a hard turn (TurnLeft/Right); the other actions store 0 */
    unsigned char pad_70[0x10];
    int actionMode;             /* +0x80: 1 while pushing (SteerWithPush), 4 while braking (Brake), else 0 */
    unsigned char pad_84[0x4];
    float heading;              /* +0x88: the yaw; TurnYawTowardVelocity eases it toward the velocity's direction */
} SnowboardRiderState;

/* Snowboard camera offset record computed relative to the rider state. */
typedef struct SnowboardRelativeCamera {
    unsigned char pad_00[0x10];
    short yawOffset;
    short offsetY;
    short riderZ;
    unsigned char pad_16[0x2a];
    Copy30 matrix; /* +0x40: the heading rotation applied to the offset vector at +0x10 */
} SnowboardRelativeCamera;

/* Snowboard action selector state: table action index, speed and two damped float values. */
typedef struct SnowboardActionState {
    unsigned char pad_00[0xc];
    unsigned short actionIndex;
    unsigned char pad_0E[0x6];
    short speed;
    unsigned char pad_16[0x1a];
    float velocityX;
    unsigned char pad_34[0x4];
    float velocityY;
} SnowboardActionState;

/* Rider turn state: a turn timer and a squared turn rate used to rotate the rider vector. */
typedef struct SnowboardRiderTurn {
    unsigned char pad_00[0x12];
    short turnTimer;
    short turnRate;
    unsigned char pad_16[0x1a];
    float turnVectorX;
    unsigned char pad_34[0x4];
    float turnVectorZ;
} SnowboardRiderTurn;

/* Snowboard rider pose: position words and origin offsets copied into the race state. */
typedef struct SnowboardRiderPose {
    unsigned char pad_00[0x10];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_16[0xa];
    int offsetX;
    int offsetY;
    int offsetZ;
    unsigned char pad_2C[0x14];
    Copy30 matrix; /* +0x40: the yaw rotation that turns the position into the offsets */
} SnowboardRiderPose;




/* g_SnoboSlotFlags: four per-slot flag bytes, whose first two also serve as the landed and spin-settled flags. */
#pragma pack(push, 1)
typedef struct SnoboSlotFlags {
    union {
        unsigned char byIndex[4]; /* one flag per resume slot */
        struct {
            char landedFlag;
            char snowboardSpinSettledFlag;
            unsigned char pad_2[0x2];
        };
    };
} SnoboSlotFlags;
#pragma pack(pop)

/* The snowboard state callback: g_SnoboStateHandler holds the state the race loop runs. */
typedef void(__cdecl *SnoboStateFn)(void);

/* g_SnoboCourseRecords (src/ff7/snobo/snobo_minigame_states_loop.cpp, 0x60 bytes): the race camera's
 * offset vector and the values the intro's camera reaches (Mini_SnowboardStateIntro interpolates
 * toward them and then copies them), the four recorded runs, and the pause menu's cursor. */
#pragma pack(push, 1)
typedef struct SnoboCourseRecords {
    short cameraOffset[4]; /* +0x0: an SVECTOR (GteSVectorToFloat3 reads it) */
    short introEndAngles[4]; /* +0x8: copied to g_SnoboCameraPosWord's angle words */
    short introEndOffset[4]; /* +0x10: copied to cameraOffset */
    SnowboardRunRecord runs[4]; /* +0x18: [0] records the run being ridden; [triggerStage] holds one to play back */
    short animFrameVariantWord; /* +0x58 */
    unsigned char pad_5a[0x6];
} SnoboCourseRecords;
#pragma pack(pop)



/* src/ff7/snobo/snobo_course_assets.cpp SnowboardMatrixState::loadMatrices: what sourceC points to, from its reads at 9 offsets. */
#pragma pack(push, 1)
typedef struct LoadMatricesSourceC {
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
} LoadMatricesSourceC;
#pragma pack(pop)

/* Snowboard input binding entry {key id, bit, name}. */
typedef struct SnowboardKeyBinding {
    int key, bit;
    char *name;
} SnowboardKeyBinding;

#endif /* FF7_TYPES_SNOBO_H */

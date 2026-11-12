/* include/ff7/types/coaster.h: the types ff7/coaster TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_COASTER_H
#define FF7_TYPES_COASTER_H
#include "lib/types/graphics.h"

#ifdef __cplusplus
typedef struct CoasterFloatVector3 {
    float x;
    float y;
    float z;
} CoasterFloatVector3;

typedef struct CoasterFloatVector4 {
    float x;
    float y;
    float z;
    float w;
} CoasterFloatVector4;

typedef struct CoasterCharSlot {
    char value;
    char pad[3];
} CoasterCharSlot;

typedef struct CoasterUCharSlot {
    unsigned char value;
    char pad[3];
} CoasterUCharSlot;

typedef struct CoasterShortSlot {
    short value;
    short pad;
} CoasterShortSlot;

typedef struct CoasterObjectState {
    int type;
    int isHit;
    int modelId;
    int hp;
    int mustInit;
    int frameCount;
    int pathIndex;
    int speed;
    int unused20;
    int unused24;
    int pathPos;
    /* stateValA..stateValC mean different things per type: the path end (<< 16) where the object
       follows its path, the start x/y/z in case 0x64, random offsets elsewhere. */
    int stateValA;
    int stateValB;
    int stateValC;
    int unused38[6];
    int params[20];
} CoasterObjectState;

struct CoasterNode;

typedef struct CoasterActor {
    VECTOR pos;
    char unused10[8];
    SVECTOR rot;
    char unused20[8];
    CoasterObjectState state;
    int pathLength;
    SVECTOR *path;
    int unusedD0;
    CoasterNode *node;
    short objIndex;
    short isActive;
    SVECTOR boundingBox[8];
    int boundingYX[8];
} CoasterActor;

struct MFVec16 {
    short x, y, z, pad;
};

typedef struct MFVec32 {
    int x, y, z, pad;
} MFVec32;

/* The left and right frustum clip planes: each has a normal, a plane constant, a sign helper and the normal's length. */
typedef struct CoasterHalfSpaceState {
    int leftDistance;
    int rightDistance;
    VECTOR leftNormal;
    VECTOR rightNormal;
    int leftHelper;
    int rightHelper;
    int leftNormalLength;
    int rightNormalLength;
} CoasterHalfSpaceState;
#endif

struct CoasterModelRecord;
/* 56-byte (0x38) coaster scene hierarchy and depth-list node. */
#pragma pack(push, 1)
typedef struct CoasterNode {
    struct CoasterModelRecord *model;
    PsxMatrix32 worldMatrix;
    struct CoasterNode *parentNode;
    short modelId;
    short index;
    short depth;
    short depthPad;
    struct CoasterNode *prev;
    struct CoasterNode *next;
} CoasterNode;
#pragma pack(pop)
typedef char CheckCoasterNode[(sizeof(CoasterNode) == 0x38) ? 1 : -1];

/* Kill-award record at offset 0x28 of the coaster state: score, kill type, bonus, sound effect. */
typedef struct CoasterKillAward {
    unsigned char pad_00[0xc];
    int hp;
    unsigned char pad_10[0x40];
    int score;
    unsigned char pad_54[0x24];
    int killType;
    short bonus;
    unsigned char pad_7E[0x1a];
    short sfxId;
} CoasterKillAward;

/* One 0x60-byte record of the coaster PSX data set's spawn script: what Mini_CoasterRunSpawnScript
 * passes to Mini_CoasterSpawnScripted, and copies into the spawn request's state. */
typedef struct CoasterSpawnScriptRecord {
    short behaviourType;
    unsigned char pad_02[0x2];
    short modelIndex;
    unsigned char pad_06[0x2];
    int pathIndex;
    int speed;
    int params[0x14]; /* the actor state's params */
} CoasterSpawnScriptRecord;

/* One 0x20-byte coaster model record from the model pool, built from a template. */
typedef struct CoasterModelRecord {
    short totalCount;
    short clearedShort0;
    short vertexCount;
    short triangleCount;
    short clearedShort1;
    unsigned char pad_0A[0x2];
    int vertices;
    int triangles;
    short maxX;
    short minX;
    short maxZ;
    short minZ;
    unsigned char pad_1C[0x4];
} CoasterModelRecord;

/* Stream list node of the coaster rail or scenery list: the entry's neighbours in the active list,
   0xFFFF for none (a free entry has both). */
typedef struct CoasterStreamNode {
    short prevIndex;
    short nextIndex;
} CoasterStreamNode;

/* Coaster sound command block: sound id and two dispatch parameters. */
typedef struct CoasterSoundCommand {
    short soundId;
    unsigned char pad_02[0x2];
    int soundParam;
    int soundParam2;
} CoasterSoundCommand;

/* Interior pool slice starting at CoasterActor.objIndex (+0xD8, stride 0x13C) for g_CoasterActorRecords. */
typedef struct CoasterActorPoolBounds {
    short objIndex;
    short isActive;
    short centerX;
    short centerY;
    short maxZ;
    unsigned char pad_0A[0x2];
    short minZFaceX;
    short minZFaceY;
    short minZ;
    unsigned char pad_12[0x2];
    short maxYFaceX;
    short maxY;
    short centerZ;
    unsigned char pad_1A[0x2];
    short minYFaceX;
    short minY;
    short minYFaceZ;
    unsigned char pad_22[0x2];
    short maxX;
    short maxXFaceY;
    short maxXFaceZ;
    unsigned char pad_2A[0x2];
    short minX;
    short minXFaceY;
    short minXFaceZ;
    unsigned char pad_32[0x10a];
} CoasterActorPoolBounds;

/* Coaster model template (20 bytes): vertex and triangle counts plus bounding box limits. */
typedef struct CoasterModelTemplate {
    short vertexCount;
    short triangleCount;
    short minX;
    short minY;
    short minZ;
    unsigned char pad_0A[0x2];
    short maxX;
    short maxY;
    short maxZ;
    unsigned char pad_12[0x2];
} CoasterModelTemplate;

/* Interior pool slice starting at CoasterActor.node (+0xD4, stride 0x13C) for g_CoasterActorSceneObjectBase. */
typedef struct CoasterActorPoolNode {
    struct CoasterNode *node;
    unsigned char pad_04[0x138];
} CoasterActorPoolNode;

/* Rail record copied into the coaster renderer's per-rail table. */
typedef struct CoasterRailEntry {
    int texU;
    int texV;
} CoasterRailEntry;

#pragma pack(push, 1)
typedef struct CoasterScorePopupState {
    short coasterScorePopupVx; /* +0x0 */
    short coasterScorePopupVy; /* +0x2 */
    short coasterScorePopupVz; /* +0x4 */
} CoasterScorePopupState;
#pragma pack(pop)

/* 0x24-byte coaster scenery triangle part stepped by CoasterRenderer::drawActor/drawPopup. */
typedef struct CoasterSceneryPart {
    unsigned char bytes[0x24];
} CoasterSceneryPart;

/* Model draw wrapper passed to CoasterRenderer::drawActor and drawPopup. */
typedef struct CoasterModelDrawArg {
    CoasterSceneryPart *parts;
    unsigned char pad_04[0x8];
    CoasterModelRecord *modelRecord;
} CoasterModelDrawArg;

/* Three-word view-space coordinate triple copied into a coaster batch primitive. */
typedef struct CoasterBatchVec3 {
    int x;
    int y;
    int z;
} CoasterBatchVec3;

/* 0x8C-byte batch primitive written by CoasterRenderer::drawRail and drawSceneryParts. */
typedef struct CoasterBatchPrimitive {
    int renderObject;
    int renderKind;
    int pad_08;
    CoasterBatchVec3 positions[4];
    CoasterRailEntry texCoords[4];
    int colors[4];
    int rhw[4];
} CoasterBatchPrimitive;

#endif /* FF7_TYPES_COASTER_H */

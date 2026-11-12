/* include/ff7/types/chocobo.h: the types ff7/chocobo TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_CHOCOBO_H
#define FF7_TYPES_CHOCOBO_H

#include "lib/types/graphics.h"

typedef union ChocoboSprite {
    int raw[18];
    struct {
        char pad0[8];
        unsigned short u;
        unsigned short v;
        float fu;
        float fv;
        unsigned short x;
        unsigned short y;
        char pad24[48];
    } f;
} ChocoboSprite;

typedef struct {
    short x;
    short y;
    short z;
    short pad_06;
    short pad_08;
    short width;
    short height;
    short tail[4];
} ChocoboCourseSprite22;


/* Chocobo race racer state (0xA4 bytes): course segment, position, heading, speed, boost. */
typedef struct ChocoboRacer {
    short segment;
    short nextSegment;
    short stepSpeed; /* travel step: currentSpeed x (1 + line bonus / 1024), cut by stamina, terrain and rank */
    short mode;
    unsigned char pad_08[0x2];
    short prevPosY;
    unsigned char pad_0C[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_16[0x2];
    int subPosX;
    unsigned char pad_1C[0x4];
    int subPosZ;
    unsigned char pad_24[0x4];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_2E[0x2];
    short clearedHeadingX;
    short heading;
    short clearedHeadingZ;
    unsigned char pad_36[0x2];
    short clearedTargetHeadingX;
    short targetHeading;
    short clearedTargetHeadingZ;
    unsigned char pad_3E[0x2];
    short startPosX;
    short startPosY;
    short startPosZ;
    unsigned char pad_46[0x2];
    short dashTimer; /* frames left of a dash: stamina -= staminaMax / 300 a step, speed = topSpeed */
    short smoothedLaneBlend;
    short personality; /* enum ChocoboPersonality (observed in game) */
    short speed;
    short steerBias;
    short racingLine; /* segment-table column 0..3 the racer follows, from intelligence */
    short currentSpeed;
    short baseSpeed;
    short topSpeed; /* the bet screen's TOP SPEED is topSpeed / 34 km */
    short actionTimer;
    short intelligenceDiv8; /* intelligence / 8; nothing reads it */
    short acceleration; /* currentSpeed += (speed - currentSpeed) * acceleration / 100 a segment */
    short autoPilot; /* 1: the AI rides (HUD "AUTOMATIC SEQUENCE"); input bit 0x100 toggles it */
    short cooperation; /* how often the chocobo obeys input; low values add wobble and drain */
    short intelligence;
    short steerStep; /* largest lane step a segment, from intelligence */
    int stamina; /* the bet screen's STAMINA is stamina / 10 */
    int staminaMax;
    short place;
    short animPhase;
    short trackPhase;
    short stepRate;
    short lastIndex;
    unsigned char pad_7A[0x2];
    short laneBlend;
    short finished; /* 0xFFFF once the racer is within 10 segments of the end */
    short targetIndex;
    short rank; /* 0 = leader, sorted by segment (Mini_ChocoboRaceRankAndUpdateRacers) */
    short stuckFrames;
    short terrainPenaltyMask; /* enum ChocoboTrackFlag bits, from the colour */
    /* +0x88 */ int nameText;
    /* +0x8C */ short nameTail;
    /* +0x8E */ char nameTerminator;
    /* +0x8F */ unsigned char pad_8F[0x1];
    short tableIndex;
    short lane;
    short countdown;
    short homeLaneBlend;
    short dashSegment;
    short dashAllowed;
    short unusedEvent7Flag;
    short clearedModifierFlag;
    short hasStartBoost;
    short locksBaseSpeed;
} ChocoboRacer;

/* Course sprite quad record (0x54 bytes): four corners (top-left, top-right, bottom-left,
 * bottom-right), their texture u and v, and a depth. The u, v and depth floats are stored as ints. */
typedef struct ChocoboSpriteQuad {
    unsigned char pad_00[0x4];
    unsigned short corner0X;
    unsigned short corner0Y;
    unsigned short corner1X;
    unsigned short corner1Y;
    unsigned short corner2X;
    unsigned short corner2Y;
    unsigned short corner3X;
    unsigned short corner3Y;
    int corner0U;
    int corner0V;
    int corner1U;
    int corner1V;
    int corner2U;
    int corner2V;
    int corner3U;
    int corner3V;
    int depth;
    unsigned char pad_38[0x8];
    union {
        char colorByte;
        int colorWord; /* the batch node takes the whole word */
    };
    unsigned char pad_44[0x4];
    struct GfxRenderObject *batchNode;
    unsigned char pad_4C[0x4];
    int dirtyFlag;
} ChocoboSpriteQuad;

/* Mini chocobo track record: copied keyframe fields and an animated step position along the track. */
typedef struct ChocoboTrackRecord {
    short posX;
    short posY;
    short posZ;
    short flags;
    short step;
    short position;
    char texU;
    char yaw;
    char texV;
    unsigned char racerIdx;
} ChocoboTrackRecord;

/* Chocobo track record of three short-position vertices and three colours; 0x24 bytes per record. */
typedef struct ChocoboTrackVertex {
    short v0x;
    short v0y;
    short v0z;
    unsigned char pad_06[0x2];
    short v1x;
    short v1y;
    short v1z;
    unsigned char pad_0E[0x2];
    short v2x;
    short v2y;
    short v2z;
    unsigned char pad_16[0x2];
    int color0;
    int color1;
    int color2;
} ChocoboTrackVertex;

/* One 0x3C-byte record of the chocobo model table g_pChocoboRacerTable: the racers at 6..15, the
 * bet-screen and scenery models around them. */
typedef struct ChocoboRacerEntry {
    short posX;
    short posY;
    short posZ;
    char colorR; /* 128 = the model's own colour */
    char colorG;
    char colorB;
    char angleX; /* written by the bet screen and Mini_ChocoboResetRacerModelAngles; the sync does not read it */
    char yaw;    /* 0x100 - heading / 16 in the race; the sync turns it into degrees */
    char angleZ;
    short scale; /* 640 */
    unsigned char pad_0E[0x2];
    char modelIndex; /* the loader's index into its asset table */
    unsigned char pad_11[0x1];
    unsigned short animCount;
    int animIndex;
    int position;
    int additive;       /* drawn with BLEND_ADDITIVE, in the second pass */
    int drawPending;    /* cleared for every model each frame; the sync sets it */
    int headingFlipped; /* the sync adds 180 degrees */
    int syncEnabled;
    int noGroundOffset; /* clear for the racers (6..15): the sync lowers posY by 10 */
    short animId;
    short prevAnimId;
    union {
        int animPlayer;
        struct PolygonAnimPlayer *player;
    };
    int animTable;
} ChocoboRacerEntry;

/* Chocobo race course track record (40 bytes): four xyz quad corners plus colour and two int words. */
typedef struct ChocoboRaceCourseQuad {
    short corner0X;
    short corner0Y;
    short corner0Z;
    unsigned char pad_06[0x2];
    short corner1X;
    short corner1Y;
    short corner1Z;
    short colorBA;
    short corner2X;
    short corner2Y;
    short corner2Z;
    short colorRG;
    short corner3X;
    short corner3Y;
    short corner3Z;
    short colorRG2;
    int color3;
    int color0;
} ChocoboRaceCourseQuad;

/* Chocobo race course vertex: position and colour read when building the triangle mesh. */
typedef struct ChocoboCourseVertex {
    short x;
    short y;
    short z;
    short colorRG;
    short vertexBX;
    short vertexBY;
    short vertexBZ;
    short colorBA;
    short vertexCX;
    short vertexCY;
    short vertexCZ;
    short colorRG2;
    int color0;
} ChocoboCourseVertex;

/* Current mini-chocobo racer: sound command words and route-driven state. */
typedef struct ChocoboRacerEntity {
    short soundCommand;
    unsigned char pad_02[0x2];
    int soundArg;
    int soundParam1;
    int soundParam2;
    int soundParam3;
    int soundParam4;
    int soundParam5;
    int soundParam6;
    int soundParam7;
    int soundParam8;
} ChocoboRacerEntity;

/* Chocobo race camera control record: buffer pointer, toggle and frame counter. */
typedef struct ChocoboCameraControl {
    int clearedState0;
    int clearedState1;
    struct ChocoboCourseWaypoint *bufferPtr; /* the course record of the viewed racer's segment */
    int toggle;
    int frameCounter;
    int initialized;
} ChocoboCameraControl;

/* Chocobo sprite object: table slot and screen position of a textured rectangle. */
typedef struct ChocoboSpriteObject {
    int slot;
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char pad_07[0x1];
    unsigned short texX;
    unsigned short texY;
    unsigned char pad_0C[0x8];
    unsigned short posX;
    unsigned short posY;
    unsigned char pad_18[0xc];
    int depth;
    int texWidth;
    int texHeight;
} ChocoboSpriteObject;



/* Projected chocobo quad vertex: scaled x, y, z and w, followed by texture coordinates. */
typedef struct ChocoboQuadVertex {
    float x;
    float y;
    float z;
    float w;
    int diffuse;
    unsigned char pad_14[0x4];
    int texU;
    int texV;
} ChocoboQuadVertex;

/* Flat-colour sprite rectangle record drawn by Mini_ChocoboEmitFlatSpriteRect. */
typedef struct ChocoboFlatSpriteRect {
    int slotIndex;
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char pad_07[0xd];
    unsigned short posX;
    unsigned short posY;
    unsigned char pad_18[0xc];
    int width;
    int height;
} ChocoboFlatSpriteRect;

/* Black sprite rectangle record with a clamped level, drawn by Mini_ChocoboEmitBlackSpriteRect. */
typedef struct ChocoboBlackSpriteRect {
    int slotIndex;
    char level;
    unsigned char pad_05[0xf];
    unsigned short posX;
    unsigned short posY;
    unsigned char pad_18[0xc];
    int depth;
    int width;
} ChocoboBlackSpriteRect;

/* One cell of the 5 x 3 chocobo bet grid (g_ChocoboBetGrid[column][row]), 8 bytes. */
typedef struct ChocoboBetCell {
    short state;         /* animation state 0..6: 3/4 and 5/6 slide the card out and back */
    char slide;          /* 0..16, how far the card has turned */
    char showPrize;      /* set to -1 at the end of the result screen: draws the prize icon */
    char face;           /* 0 or -1: which of the two card faces is showing */
    char mode;           /* -1 none, 1 and 2 the two marked states of a bet */
    unsigned char prize; /* index into g_ChocoboBetPrizeSaveIds */
    char tier;           /* 0, 1 or 2: which of the three prize values the cell holds */
} ChocoboBetCell;

/* A 0x48-byte textured quad sprite of the chocobo bet screen: the grid's cell sprites
 * (g_ChocoboBetGridRecords0..2) and the legend icons. The corners are 0 top left, 1 top right,
 * 2 bottom left, 3 bottom right. */
typedef struct ChocoboQuadSprite {
    int kind;
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x1];
    unsigned short u;
    unsigned short v;
    float fu; /* u / 256 */
    float fv; /* v / 128 */
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    short x3;
    short y3;
    float depth;
    int outerWidth;
    int outerHeight;
    int palette;
    float texUOffset;
    float texVOffset;
    float quadWidth;
    float quadHeight;
    unsigned char pad_44[0x4];
} ChocoboQuadSprite;

/* Four 3D corner positions and four texture coordinate pairs copied into the projected chocobo quad batch. */
typedef struct ChocoboQuadUvs {
    GfxHomogeneousPoint corners[4];
    int tex0U;
    int tex0V;
    int tex1U;
    int tex1V;
    int tex2U;
    int tex2V;
    int tex3U;
    int tex3V;
} ChocoboQuadUvs;

/* Only the fields the code reads; the size is not known. */
typedef struct ChocoboCameraInitEntry {
    short eyeHeightOffset;
    short lookHeightOffset;
    short angle;
} ChocoboCameraInitEntry;

/* hub ChocoboHudRenderObject begin */
#pragma pack(push, 1)
struct ChocoboHudRenderObject {
    struct GfxRenderObject *renderObj; /* +0x0 */
    unsigned char pad_4[0x8];
    int alternateFlag; /* +0xc */
    int activeFlag; /* +0x10 */
    int slotUsedFlag; /* +0x14 */
    int readyFlag; /* +0x18 */
};
#pragma pack(pop)
/* Course quad record in the chocobo track quad table, one 0x40-byte record per segment. */
typedef struct ChocoboCourseQuad {
    float corner0X;             /* +0x00 */
    float corner0Y;
    float corner0Z;
    float corner1X;             /* +0x0C */
    float corner1Y;
    float corner1Z;
    float corner2X;             /* +0x18 */
    float corner2Y;
    float corner2Z;
    float corner3X;             /* +0x24 */
    float corner3Y;
    float corner3Z;
    /* the corner colours: written as words with alpha forced, then bytes 0 and 2 swapped (ch_init.cpp) */
    union {
        int color0;             /* +0x30 */
        struct { unsigned char color0Blue, color0Green, color0Red, color0Alpha; };
    };
    union {
        int color1;             /* +0x34 */
        struct { unsigned char color1Blue, color1Green, color1Red, color1Alpha; };
    };
    union {
        int color2;             /* +0x38 */
        struct { unsigned char color2Blue, color2Green, color2Red, color2Alpha; };
    };
    union {
        int color3;             /* +0x3C */
        struct { unsigned char color3Blue, color3Green, color3Red, color3Alpha; };
    };
} ChocoboCourseQuad;

/* The (x, z) point at +8 of a ChocoboCourseWaypoint (its auxX, auxZ), read along the chocobo course. */
typedef struct ChocoboWatPoint {
    short x;
    unsigned char pad_02[0x2];
    short z;
} ChocoboWatPoint;

/* Chocobo model record allocated by Mini_ChocoboAllocModelRecord, with three fields set to constants
 * and the RSD model filled in later. */
typedef struct ChocoboModelRecord {
    int count;
    int count2;
    int kind;
    void *rsdModel;
} ChocoboModelRecord;

/* Chocobo track table entry of 0x30 bytes, used for both the float vertex table and the triangle table. */
typedef struct ChocoboTrackEntry {
    unsigned char pad_00[0x24];
    int pad_24;
    int pad_28;
    int pad_2C;
} ChocoboTrackEntry;

/* One waypoint of the mini chocobo race course, 0x18 bytes, in the course buffer. */
typedef struct ChocoboCourseWaypoint {
    short x;
    short y;
    short z;
    short eventNumber;
    short auxX;
    short auxY;
    short auxZ;
    short eventId;
    char pendingEvent;
    char animRef;
    unsigned char segmentOffset;
    char flags;
    char overlayColorR; /* the screen overlay colour the camera flags enable */
    char overlayColorG;
    char overlayColorB;
    unsigned char cameraFlags; /* bit 0 submits the course triangles, bit 1 draws the overlay */
} ChocoboCourseWaypoint;

/* Chocobo HUD and bet/result screen sprite record (0x48 bytes). */
#pragma pack(push, 1)
typedef struct ChocoboSpriteRecord {
    union {
        int renderObjectIndex; /* +0x00: the slot of g_ChocoboHudRenderObjects the emitters draw into */
        int hudIndex;
    };
    union {
        char colorR;           /* +0x04 */
        unsigned char uColorR;
    };
    union {
        char colorG;           /* +0x05 */
        unsigned char uColorG;
    };
    union {
        char colorB;           /* +0x06 */
        unsigned char uColorB;
    };
    unsigned char pad_07[0x1];
    short texX;                /* +0x08: source texel position in the atlas */
    short texY;
    union {                    /* +0x0C: source UV origin */
        int texU;
        float texUBase;
    };
    union {                    /* +0x10 */
        int texV;
        float texVBase;
    };
    union {                    /* +0x14: corner A (top left) x */
        short left;
        unsigned short uiX;
        unsigned short uLeft;
    };
    union {                    /* +0x16: corner A (top left) y */
        short vertexAY;
        unsigned short uiY;
        unsigned short uVertexAY;
    };
    union {                    /* +0x18: corner C (top right) x */
        short right;
        unsigned short uRight;
    };
    union {                    /* +0x1A: corner C (top right) y */
        short vertexCY;
        unsigned short uVertexCY;
    };
    union {                    /* +0x1C: corner B (bottom left) x */
        short vertexBX;
        unsigned short uVertexBX;
    };
    union {                    /* +0x1E: corner B (bottom left) y */
        short vertexBY;
        unsigned short uVertexBY;
    };
    union {                    /* +0x20: corner D (bottom right) x */
        short vertexDX;
        unsigned short uVertexDX;
    };
    union {                    /* +0x22: corner D (bottom right) y */
        short vertexDY;
        unsigned short uVertexDY;
    };
    int depth;                 /* +0x24: float bits */
    union {                    /* +0x28: on-screen width in UI pixels */
        int width;
        int uiOffsetX;
        int outerWidth;
    };
    union {                    /* +0x2C: on-screen height in UI pixels */
        int height;
        int uiOffsetY;
        int outerHeight;
    };
    union {                    /* +0x30: palette index and colour byte */
        int palette;
        char paletteByte;
    };
    union {                    /* +0x34: UV extent added to texU/texV */
        int texUSize;
        float texUOffset;
    };
    union {                    /* +0x38 */
        int texVSize;
        float texVOffset;
    };
    int texelWidth;            /* +0x3C: source size in texels, float bits */
    int texelHeight;           /* +0x40 */
    int isActive;              /* +0x44 */
} ChocoboSpriteRecord;

/* Chocobo track data (0x54 bytes): course header, chocobo.wat section buffers, and built float/quad/triangle tables. */
typedef struct ChocoboTrackData {
    union {
        int courseId;          /* +0x00: waypoint/segment count */
        int spriteTable;
    };
    union {
        int racerFieldBase;    /* +0x04 */
        int segmentTable;
    };
    union {
        char *watBuffer;       /* +0x08 */
        int routeEventTable;
    };
    union {
        int entryCount40;      /* +0x0C */
        int quadCount;
    };
    union {
        char *entries40;       /* +0x10 */
        int block;
    };
    union {
        int entryCount28;      /* +0x14 */
        int triangleCount;
    };
    union {
        char *entries28;       /* +0x18 */
        int floatVertexTable;
    };
    union {
        char *entries2;        /* +0x1C */
        int triangleTable;
    };
    union {
        char *entries4;        /* +0x20 */
        int quadTable;
    };
    union {
        short entryCount36;    /* +0x24 */
        short floatVertexCount;
    };
    unsigned char pad_26[0x2];
    union {
        char *entries36;       /* +0x28 */
        int trackVertexTable;
    };
    short subEntryCount40;     /* +0x2C */
    unsigned char pad_2E[0x2];
    union {
        char *subEntries40;    /* +0x30 */
        int courseSpriteTable;
    };
    union {
        char *entries8;        /* +0x34 */
        int racingLineTable;
    };
    int routeEvents;           /* +0x38 */
    int segmentCount;          /* +0x3C */
    int scenerySlots;          /* +0x40 */
    int courseSounds;          /* +0x44 */
    int builtFloatVertices;    /* +0x48 */
    int builtTriangles;        /* +0x4C */
    int builtQuads;            /* +0x50 */
} ChocoboTrackData;
#pragma pack(pop)

/* One record of g_ChocoboColorHudInitTable (0x4 bytes). */
typedef struct ChocoboRacerStat {
    char colorR; /*+0x00 */
    char colorG; /*+0x01 */
    char colorB; /*+0x02 */
    unsigned char pad_03[0x1];
} ChocoboRacerStat;

/* The race's colour and camera parameter table (g_ChocoboColorHudInitTable, 0x98 bytes): the colour records,
   whose fields are copied into the chocobo course records at race init. */
typedef struct ChocoboRacerStats {
    ChocoboRacerStat racerColors[19]; /* +0x00: six racer colours, then the special colours from [6] */
    char course0ColorR;
    char course0ColorG;
    char course0ColorB;
    unsigned char pad_4F[0x1];
    short course0Left;
    short course0VertexAY;
    unsigned char course0TexX;
    unsigned char course0TexY;
    unsigned char pad_56[0x2];
    short course0Right;
    short course0VertexCY;
    unsigned char pad_5C[0x4];
    short course0VertexBX;
    short course0VertexBY;
    unsigned char pad_64[0x4];
    short course0VertexDX;
    short course0VertexDY;
    unsigned char pad_6C[0x8];
    char course2ColorR;
    char course2ColorG;
    char course2ColorB;
    unsigned char pad_77[0x1];
    short course2Left;
    short course2VertexAY;
    unsigned char pad_7C[0x4];
    short course2Right;
    short course2VertexCY;
    unsigned char pad_84[0x4];
    short course2VertexBX;
    short course2VertexBY;
    unsigned char pad_8C[0x4];
    short course2VertexDX;
    short course2VertexDY;
    unsigned char pad_94[0x4];
} ChocoboRacerStats;

/* Chocobo bet slot (4 bytes) read while building the bet grid. */
typedef struct ChocoboBetSlot {
    char betValue;
    char isLateGameOnly;
    unsigned char pad_02[0x1];
    char betPlaced;
} ChocoboBetSlot;

/* Racer model record whose pointer sits in the chocobo racer table entry at offset 0x34. */
typedef struct ChocoboRacerModelRecord {
    unsigned char pad_00[0x20];
    int model;
    unsigned char pad_24[0x10];
    int skeletonAnim;
    unsigned char pad_38[0x7c];
    int animDraw;
    int engineContext;
} ChocoboRacerModelRecord;

/* HUD layout entry of one bet-grid legend row, with current and previous positions. */
typedef struct ChocoboHudLayoutEntry {
    unsigned char pad_00[0x16];
    short prevPosX;
    unsigned char pad_18[0x2];
    short posX;
    unsigned char pad_1C[0x2];
    short prevPosY;
    unsigned char pad_20[0x2];
    short posY;
    unsigned char pad_24[0x24];
} ChocoboHudLayoutEntry;

/* Three-step fade level history for the chocobo race overlay quad. */
typedef struct ChocoboFadeHistory {
    unsigned char pad_00[0x4];
    char oldFade;
    char midFade;
    char newFade;
    unsigned char pad_7[0x1];
} ChocoboFadeHistory;

/* Chocobo course sprite record (0x54 bytes, g_ChocoboSpriteRecords[]): its tint, size, the render
 * object created for it, and the flags that gate drawing it. */
typedef struct ChocoboSpriteGroup {
    unsigned char tint[3]; /* +0x0: set to 0x80 each when the sprite is created */
    unsigned char pad_03[0x35];
    int sizeB; /* +0x38: g_ChocoboCourseSpriteSize */
    int sizeA; /* +0x3c: g_ChocoboCourseSpriteSize */
    int paletteIndex; /* +0x40: cleared when the sprite is created */
    int flag; /* +0x44: 0 for the first draw group, 1 for the second */
    int renderObject; /* +0x48: the sprite's render object */
    int active; /* +0x4c: nonzero while the render object exists */
    int dirtyFlag; /* +0x50: nonzero while the sprite needs drawing */
} ChocoboSpriteGroup;

/* Result-screen UI table entry, one per bet state. */
typedef struct ChocoboResultUiEntry {
    unsigned char pad_00[0x14];
    short screenY;
    short rowHeight;
    unsigned char pad_18[0x30];
} ChocoboResultUiEntry;

/* Pair of bet states with the two result values they produce. */
typedef struct ChocoboPairResult {
    short stateA;
    short stateB;
    short resultA;
    short resultB;
} ChocoboPairResult;

/* Race HUD sort entry (8 bytes): racer index and active progress sort key. */
typedef struct ChocoboRaceSortEntry {
    int racerIndex;
    int sortKey;
} ChocoboRaceSortEntry;

/* Race asset block that g_ChocoboFlags points at; three shorts are cleared at race init. */
typedef struct ChocoboRaceAssetState {
    short clearedShort0;
    short clearedShort1;
    short clearedShort2;
} ChocoboRaceAssetState;

/* One record of g_ChocoboRacerHudColors (0x4 bytes). */
typedef struct ChocoboColor {
    char color; /*+0x00 */
    char colorValue; /*+0x01 */
    char colorB; /*+0x02 */
    unsigned char pad_03[0x1];
} ChocoboColor;


/* Scenery animation slot in the track buffer, set to start or stop its step. */
typedef struct ChocoboScenerySlot {
    unsigned char pad_00[0x8];
    short animStep;
    unsigned char pad_0A[0x6];
} ChocoboScenerySlot;

/* One race class's ranges for the opponents' stats (g_ChocoboClassStats, indexed by the savemap's
 * raceClass): top speed is topSpeedBase + rand() % topSpeedRange, base speed likewise, stamina
 * topSpeedBase + rand() % 300. Every racer read off the bet screen at classes 0..3 lies
 * in its class's ranges. */
typedef struct ChocoboClassStats {
    short topSpeedBase;
    short topSpeedRange;
    short baseSpeedBase;
    short baseSpeedRange;
} ChocoboClassStats;

/* Racer asset entry with a value copied to the racer record and a model path suffix. */
typedef struct ChocoboRacerAssetEntry {
    short animCount;
    unsigned char pad_02[0x2];
    char *pathSuffix;
    char *animPaths[10]; /* +0x8: the path of each animation, animCount of them */
} ChocoboRacerAssetEntry;

/* 48-byte course triangle entry: three float vertices copied from the course's short coordinates. */
typedef struct ChocoboTriangleEntry {
    float vertexAX;
    float vertexAY;
    float vertexAZ;
    float vertexBX;
    float vertexBY;
    float vertexBZ;
    float vertexCX;
    float vertexCY;
    float vertexCZ;
    unsigned char pairA0;
    unsigned char pad_25[0x1];
    unsigned char pairB0;
    unsigned char pad_27[0x1];
    unsigned char pairA1;
    unsigned char pad_29[0x1];
    unsigned char pairB1;
    unsigned char pad_2B[0x1];
    unsigned char pairA2;
    unsigned char pad_2D[0x1];
    unsigned char pairB2;
} ChocoboTriangleEntry;

/* Chocobo race camera position in three axes, updated from the race camera targets each frame. */
typedef struct ChocoboCameraPos {
    short camPosX;
    short camPosY;
    short camPosZ;
    unsigned char pad_6[0x2];
} ChocoboCameraPos;

/* Race camera position record, written from the racer camera targets each frame. */
typedef struct ChocoboRaceCameraPos {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_6[0x2];
} ChocoboRaceCameraPos;

/* Chocobo race state: camera-follow position words read by the race camera update. */
typedef struct ChocoboRaceState {
    char eventKind; /* the camera case the route event selects; 0xFF = none */
    char mode;
    short cameraOffsetY;
    union {
        int packedPos;
        struct {
            short orbitDivisorY;
            short orbitOffsetY;
        };
    };
    union {
        int cameraDistance;
        struct {
            short orbitDivisorX;
            short segment;
        };
    };
    int posX;
    union {
        int posY;
        struct {
            short pad_10;
            short targetSegment;
        };
    };
} ChocoboRaceState;

/* Quad record holding four points passed to the projected quad append. */
typedef struct ChocoboProjectedQuadRecord {
    unsigned char pad_00[0x30];
    int point0;
    int point1;
    int point2;
    int point3;
} ChocoboProjectedQuadRecord;

/* Vertex record whose three colour words at 0x24 to 0x2C get full alpha in the float vertex table. */
typedef struct ChocoboFloatVertex {
    unsigned char pad_00[0x24];
    int color0;
    int color1;
    int color2;
} ChocoboFloatVertex;

/* Triangle record whose words at 0x24 to 0x2C are passed to Mini_ChocoboAppendProjectedTriangle. */
typedef struct ChocoboTriangleRecord {
    unsigned char pad_00[0x24];
    int vertex0Value;
    int vertex1Value;
    int vertex2Value;
} ChocoboTriangleRecord;

/* Signed 16-bit point (x, y, z) read by Mini_ChocoboShortPointProjectedDepth. */
typedef struct ChocoboShortPointDepth {
    short x;
    short y;
    short z;
} ChocoboShortPointDepth;

/* Record whose words at 0x3C to 0x44 are set from three arguments. */
typedef struct ChocoboRecordTail {
    unsigned char pad_00[0x3c];
    int tailValue0;
    int tailValue1;
    int tailValue2;
} ChocoboRecordTail;

/* Entity record holding a model name at +0x10 and its loaded animation player at +0x20. */
typedef struct ChocoboEntityRecord {
    unsigned char pad_00[0x10];
    int modelName;
    unsigned char pad_14[0xc];
    int animPlayer;
} ChocoboEntityRecord;

typedef struct ChocoboRaceRecordCopy {
    int w[5];
} ChocoboRaceRecordCopy;

#pragma pack(push, 1)
typedef struct ChocoboResultSpriteTable {
    union {
        struct {
            unsigned char header0[0x14];
            short x0, y0, x1, y1, x2, y2, x3, y3;
            unsigned char rest[0x18c];
        };
        ChocoboQuadSprite sprites[6];
    };
} ChocoboResultSpriteTable;
#pragma pack(pop)

#endif /* FF7_TYPES_CHOCOBO_H */

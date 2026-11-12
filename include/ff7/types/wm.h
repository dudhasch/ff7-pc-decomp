/* include/ff7/types/wm.h: the types ff7/wm TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_WM_H
#define FF7_TYPES_WM_H


struct GfxRenderObject;
#include "ff7/world.h"
#include "lib/types/graphics.h"

/* Decompressed mesh slot: vertex and face array pointers and the vertex count, built by World_DecompressMeshIntoSlot. */
typedef struct WorldMeshSlot {
    unsigned char pad_00[0x4];
    int vertices;
    int faces;
    int facesEnd;
    short meshCol;
    short meshRow;
    short vertexCount;
} WorldMeshSlot;

/* Three-int world position compared for wrapped distance on the world map. */
typedef struct WorldPoint {
    int posX;
    int posY;
    int posZ;
} WorldPoint;

struct t_wm_SkyPoly {
    struct {
        int tag;
        unsigned char r0, g0, b0, code;
        short x0, y0;
        unsigned char r1, g1, b1, pad1;
        short x1, y1;
        unsigned char r2, g2, b2, pad2;
        short x2, y2;
        unsigned char r3, g3, b3, pad3;
        short x3, y3;
    } polyG4;
    unsigned int bgra0, bgra1, bgra2, bgra3;
};

/* World block load request: queue link, grid cell index and streaming slot of one mesh block. */
typedef struct WorldBlockLoadRequest {
    int next;
    short cellIndex;
    short slot;
} WorldBlockLoadRequest;

typedef struct FloatVector3 {
    float x, y, z;
} FloatVector3;

typedef FloatVector3 WorldLightVector;

typedef BgraValue WorldLightColor;

typedef struct WorldAnimationHeader {
    int version, frameCount;
} WorldAnimationHeader;

/* Loaded 3D model record (0xB8 bytes) in g_WorldLeaderModels and g_WorldStageModels. */
typedef struct WorldModelRecord {
    char pad00, effect, pad02[2];
    unsigned char animationCount;
    char pad05[0x1b];
    unsigned short animation;
    char pad22[2];
    int active; /* 0 once the loader or the free routine has reset the record */
    char pad28[8];
    int frame;
    int needsScenePass; /* 1 for the "barrier" stage model: drawn with a second scene pass */
    float modelScale; /* the loader's scale, kept to scale each animation's root translation */
    WorldAnimationHeader **animations; /* one entry per animation of the model's WorldModelDef */
    void *skeleton;
    union {
        unsigned char bonePlacement[0x74]; /* PolygonAnimPlacement initialised by the loader */
        struct {
            int pad44;
            FloatVector3 position, rotation;
            char pad60[16];
            float matrix[4][4];
            char padb0[8];
        };
    };
} WorldModelRecord;

typedef struct WorldBlendColor {
    union {
        unsigned int value;
        struct {
            unsigned char b, g, r, a;
        } c;
    };
} WorldBlendColor;

typedef struct WorldBlendSpec {
    char pad00[8];
    WorldBlendColor color;
    char pad0c[0x28];
    unsigned char flags[16];
} WorldBlendSpec;

/* One cell of the world script's operand stack. The stack's cells start at
 * g_WorldScriptOperandStack; g_WorldScriptStackCursor points at the next free one.
 * `kind` is a WorldScriptOperandKind with the bank in its low two bits (0..2 select
 * g_pWorldScriptBank0..2; 3 makes `value` a WorldScriptSpecial selector), and `value` is the
 * constant, the bit or byte offset into the bank, or the selector. */
typedef struct WorldScriptOperand {
    int value;
    unsigned short kind;
} WorldScriptOperand;

typedef struct WorldShadowQuad {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short page;
    short x2, y2;
    unsigned char u2, v2;
    short pad1;
    short x3, y3;
    unsigned char u3, v3;
    short pad2;
    struct GfxRenderObject *batch;
    unsigned int color;
} WorldShadowQuad;

typedef struct WorldFloatColor {
    float b, g, r, a;
} WorldFloatColor;

typedef struct WorldTextureOrigin {
    unsigned short u, v;
} WorldTextureOrigin;

typedef struct WorldEffectSprite {
    unsigned char index, flags, width, height, u0, v0, u1, v1;
    unsigned short palette;
    char pad0a[2];
} WorldEffectSprite;

/* Three-float vector (x, y, z) read by the cross product. */
typedef struct WorldVec {
    float x;
    float y;
    float z;
} WorldVec;

/* Party bytes at gFieldParty: a lead byte and two member bytes compared against model indices. */
typedef struct WorldMapPartyRoster {
    unsigned char pad_00[0x1];
    unsigned char member1;
    unsigned char member2;
} WorldMapPartyRoster;

/* One 0x54-byte record of g_WorldEffectNodePool: a sprite effect of the world map. A node is either
 * in the active list, newest first (World_SpawnEffectNode links it in, World_UpdateEffectNodes moves
 * it out when its record runs out), in the retired list, or in the free list; all three chain
 * through nextNode. The quad at +0x24 is what World_DrawEffectNodeQuad fills and draws. */
typedef struct WorldEffectNode {
    struct WorldEffectNode *nextNode;
    int posX;
    int posY;
    int posZ;
    int active;
    short velocityX;
    short velocityY;
    short velocityZ;
    short yaw;
    unsigned char recordIndex;
    unsigned char timer;
    unsigned char horizontal;
    unsigned char deceleration;
    short fadeLevel;
    unsigned char pad_22[0x2];
    WorldShadowQuad quad;
} WorldEffectNode;

/* 12-byte entry of g_WorldEffectRecordTable, indexed by a world effect node's recordIndex. */
typedef struct WorldEffectRecord {
    unsigned char timer;
    unsigned char flags;
    unsigned char current;
    unsigned char next;
    unsigned char pad_04[0x8];
} WorldEffectRecord;

/* 12-byte world field table entry that World_SetFieldEntryFromTable copies into the field actor state. */
typedef struct WorldFieldTableEntry {
    short pcTileX;
    short pcTileY;
    short pcWalkMeshId;
    short eventCmdParam;
    unsigned char entryId;
} WorldFieldTableEntry;

/* Eight-byte vehicle save slot packing entity position and model fields into two words. */
typedef struct WorldVehicleSlot {
    int packedXTypeDirection;
    int packedYAndZ;
} WorldVehicleSlot;

/* Light colour block that the terrain chunk submit reads through a stored pointer. */
typedef struct WorldTerrainLightBlock {
    float colorB0;
    float colorG0;
    float colorR0;
    unsigned char pad_0C[0x34];
    float colorB1;
    float colorG1;
    float colorR1;
    unsigned char pad_4C[0x34];
    float colorB2;
    float colorG2;
    float colorR2;
} WorldTerrainLightBlock;

/* One record of g_WorldProjectedVertices (0xc bytes). */
typedef struct WorldProjectedVertexEntry {
    float x; /*+0x00 */
    float y; /*+0x04 */
    float depth; /*+0x08 */
} WorldProjectedVertexEntry;

/* One world mesh vertex stored as three floats. */
typedef struct WorldMeshVertex {
    float posX;
    float posY;
    float posZ;
} WorldMeshVertex;

/* Terrain effect slot holding vertex colours copied into the chunk's base colours. */
typedef struct WorldTerrainEffectSlot {
    unsigned int vertexColor0;
    unsigned char pad_04[0xc];
    unsigned int vertexColor1;
    unsigned char pad_14[0xc];
    unsigned int vertexColor2;
} WorldTerrainEffectSlot;

/* One record of g_WorldTransformedVertices (0x10 bytes). */
typedef struct WorldTransformedVertexEntry {
    float perspScale; /*+0x00 */
    float y; /*+0x04 */
    float z; /*+0x08 */
    float depthW; /*+0x0C */
} WorldTransformedVertexEntry;


/* Shadow quad source texture rectangle: x, y, width and height as shorts (the caller's
 * short[4]); the low bytes of x and y are read as chars for the quad's left and top. */
typedef struct WorldShadowTextureRect {
    union { short srcX; char srcXLow; };
    union { short srcY; char srcYLow; };
    short width;
    short height;
} WorldShadowTextureRect;

/* Short matrix whose values at 0x00, 0x06 and 0x0C set the world ambient color bytes. */
typedef struct WorldAmbientMatrix {
    short diagonal0;
    unsigned char pad_02[0x4];
    short diagonal1;
    unsigned char pad_08[0x4];
    short diagonal2;
} WorldAmbientMatrix;

/* One entry of the world script image's sorted directory (image + 0x200): script id and the index of its code. */
typedef struct WorldScriptDirectoryEntry {
    unsigned short scriptId;
    short scriptIdx;
} WorldScriptDirectoryEntry;

/* World entity position triple copied into an entity's position (x, z) and its groundY word. */
typedef struct WorldPosition {
    int x;
    int y;
    int z;
} WorldPosition;

typedef struct WorldMapModel {
    char pad[0x48];
    struct {
        float x, y, z;
    } sPos;
} WorldMapModel;

typedef struct WorldBlockLoadInfo {
    struct WorldBlockLoadInfo *pNext;
    short wBlockId;
} WorldBlockLoadInfo;

struct WorldTerrainAnimGroup {
    unsigned int dataOffset;
    unsigned short frameStride;
    unsigned char frameCount, tickDivisor;
};

/* A world sky sprite (a cloud layer of g_WorldZoneData or the meteor at g_WorldSkySprite): a
 * PSX POLY_FT4, the render object it is drawn with and a packed colour (0x30 bytes). Moved from
 * include/lib/types/sound.h. */
typedef struct WorldSkySprite {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0;
    unsigned char u0, v0;
    short clut;
    short x1, y1;
    unsigned char u1, v1;
    short page;
    short x2, y2;
    unsigned char u2, v2;
    short pad2;
    short x3, y3;
    unsigned char u3, v3;
    short pad3;
    struct GfxRenderObject *renderObject; /* g_WorldCloudRenderObject or g_WorldMeteorRenderObject */
    unsigned int color;
} WorldSkySprite;

/* Zolom trail record (8 bytes): x and z are advanced each frame by World_AdvanceZolomTrailHead. */
typedef struct WorldZolomTrailPoint {
    unsigned short x;
    unsigned short z;
    union {
        short heading; /* yaw 0..0xFFF; the record's last, unused-by-writers field when read unsigned */
        unsigned short headingUnsigned;
    };
    union {
        short ground; /* the ground height World_SampleZolomHeadGround samples */
        unsigned short groundUnsigned;
    };
} WorldZolomTrailPoint;

/* Mesh record in the world's linked list, with a triangle-cache key pair and an expiry counter. */
typedef struct WorldMeshRecord {
    int next;
    unsigned char pad_04[0xc];
    short chunkX;
    short chunkY;
    unsigned char pad_14[0x2];
    short staleFrames;
} WorldMeshRecord;

/* Node of the world mesh record list and free-block list: next link and mesh pointer. */
typedef struct WorldMeshRecordNode {
    int next;
    int mesh;
} WorldMeshRecordNode;

/* Scratch short triple reused by the terrain flood fill. */
typedef struct WorldSkyVertexQuad {
    short posX;
    short posY;
    short posZ;
    unsigned char pad_6[0x2];
} WorldSkyVertexQuad;

#pragma pack(push, 1)
typedef struct WorldAmbientColor {
    char b; /* BGRA bytes */
    char g;
    char r;
    char a;
} WorldAmbientColor;
#pragma pack(pop)

/* The x and y of g_WorldEffectOffset and g_WorldEffectVelocity; their z is the short global after each,
 * so RotTransPoint reads the three as one short vector. */
#pragma pack(push, 1)
typedef struct WorldEffectOffset {
    short x; /* +0x0 */
    short y; /* +0x2 */
} WorldEffectOffset;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WorldRenderBuffers {
    int arenaRegion7; /* +0x0 */
    unsigned char pad_4[0x4070];
    int arenaRegion7Second; /* +0x4074 */
} WorldRenderBuffers;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WorldTerrainCurrentRecords {
    union {
        struct {
        int brightness; /* +0x0 */
        int secondaryBrightness; /* +0x4 */
        int pendingBrightness; /* +0x8 */
        };
        int channel[3]; /* the same three words, indexed by channel */
    };
} WorldTerrainCurrentRecords;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WorldTerrainFloodCrossing {
    union {
        struct {
        int floodTimer; /* +0x0 */
        int isBusy; /* +0x4 */
        int effectTimer; /* +0x8 */
        };
        int slot[3]; /* the same three words, indexed by channel */
    };
} WorldTerrainFloodCrossing;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WorldVehicleModelStack {
    union {
        struct {
            int currentModel; /* +0x0 */
            int pendingModel; /* +0x4 */
        };
        int slots[2];
    };
} WorldVehicleModelStack;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WorldVehicleSlotStack {
    union {
        struct {
            int currentSlot; /* +0x0 */
            int pendingSlot; /* +0x4 */
        };
        int slots[2];
    };
} WorldVehicleSlotStack;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct WorldZoneMeshCoords {
    union {
        struct {
        int fadeStep; /* +0x0 */
        int pendingFadeStep; /* +0x4 */
        int fadeStepSource; /* +0x8 */
        };
        int channel[3]; /* the same three words, indexed by channel */
    };
} WorldZoneMeshCoords;
#pragma pack(pop)

/* A terrain current of the world map (0x14 bytes): where it flows from, its angle and strength, and the
 * indices of the terrain vertices it moves (a negative index ends the list). */
#pragma pack(push, 1)
typedef struct WorldTerrainCurrentRecord {
    short x;
    short y;
    short angle;
    short strength;
    signed char vertices[12];
} WorldTerrainCurrentRecord;

/* g_WorldZolomSegments (src/ff7/src/wm/wm_world_zones_sky.cpp): the three terrain currents and the word after them. */
typedef struct WorldTerrainCurrentTable {
    WorldTerrainCurrentRecord records[3];
    int end;
} WorldTerrainCurrentTable;
#pragma pack(pop)

/* One world lighting zone (0x24 bytes): its mesh-and-offset words, ambient and fog colours and radii. */
#pragma pack(push, 1)
typedef struct WorldZone {
    int posX; /* +0x00: mesh column in the upper 19 bits, 13-bit offset in the low bits */
    int posY; /* +0x04 */
    int posZ; /* +0x08: mesh row and offset, packed as posX */
    int pad_0C;
    unsigned char ambient[3]; /* +0x10 */
    unsigned char enabled; /* +0x13 */
    unsigned char nearColor[4]; /* +0x14: near fog colour, then an unused byte */
    unsigned char farColor[4]; /* +0x18 */
    int outerRadius; /* +0x1C */
    int innerRadius; /* +0x20 */
} WorldZone;

/* g_WorldZolomTrail (src/ff7/src/wm/wm_world_zones_sky.cpp): the 16 world zone records. */
typedef struct WorldZolomTrail {
    WorldZone zones[16];
} WorldZolomTrail;
#pragma pack(pop)

/* src/ff7/src/wm/wm_world_zones_sky.cpp World_PushTerrainFloodCrossingRecord: what mesh points to, from its reads at 3 offsets. */
#pragma pack(push, 1)
typedef struct WorldPushTerrainFloodCrossingRecordMesh {
    unsigned char pad_0[0x8];
    SVECTOR *vertexData; /* +0x8 */
    unsigned char pad_c[0x4];
    short meshCol; /* +0x10 */
    short meshRow; /* +0x12 */
} WorldPushTerrainFloodCrossingRecordMesh;
#pragma pack(pop)

/* A model the world map loads (World_LoadStageModels, World_LoadLeaderModel): 0x14 bytes, in
 * wmfile.cpp's stage tables (g_WorldStageModelTable) and leader records (g_WorldLeaderModelTable). */
typedef struct WorldModelDef {
    char *name; /* +0x00: "choko" gets the secondary light group, "barrier" the averaging blend */
    char *hrcFile; /* +0x04: the skeleton in the world archive */
    unsigned char animCount; /* +0x08: the length of animFiles (the loaders read a byte) */
    unsigned int scale; /* +0x0C: in 1/512; above 512 it enlarges the model (scale / 512), else 1.0 */
    char **animFiles; /* +0x10: a run of g_WorldAnimFileNames */
} WorldModelDef;

/* Shared world-map terrain chunk, triangle, cache, matrix and ground-probe types. */

/* One terrain triangle: three vertex indices, the terrain type, three texture coordinates and a
 * packed word whose bits 16..24 select the texture page. */
typedef struct WorldTerrainTriangle {
    union {
        unsigned char vertices[3];
        struct {
            unsigned char v0, v1, v2;
        };
    };
    unsigned char terrain;
    union {
        struct {
            unsigned char uv[6], packed, region;
        };
        struct {
            unsigned char u0, v0tex, u1, v1tex;
            union {
                struct {
                    unsigned char u2, v2tex, packed0, packed1;
                };
                int packedTexture;
            };
        };
    };
} WorldTerrainTriangle;

typedef struct WorldTriangleCache {
    WorldTerrainTriangle *triangle;
    short x, y;
} WorldTriangleCache;

typedef struct WorldTerrainChunkInfo {
    struct WorldTerrainChunkInfo *pNext;
    WorldTerrainTriangle *triangles;
    SVECTOR *vertices, *normals;
    short wChunkX, wChunkY, count, age;
} WorldTerrainChunkInfo;

typedef struct WorldGroundProbe {
    VECTOR position;
    SVECTOR local;
    WorldTerrainChunkInfo *chunk;
    WorldTerrainTriangle *triangle;
    short x, y, state, angle, height, terrain;
    int unused;
} WorldGroundProbe;

typedef struct WorldProbeRow {
    WorldGroundProbe probes[5];
} WorldProbeRow;

#endif /* FF7_TYPES_WM_H */

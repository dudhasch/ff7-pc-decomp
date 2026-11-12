/* include/ff7/types/field.h: the types ff7/field TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_FIELD_H
#define FF7_TYPES_FIELD_H


struct GfxRenderObject;
#include "ff7/gte.h"

// Exact original scalar WORDs: CFFDE4 has one reader; D0008E has three readers and two stores.
// Neither scalar is indexed or passed by address. They are defined in the .bss
// spans that hold them: g_FieldSurfaceDestX (0xCFFDE4) in ad_ddraw.cpp, 0xD0008E as the second word
// of ad_pal.cpp's g_AdPalWords (a separate short would be 4-aligned).
struct AdPalWordPair {
    unsigned short first, second;
};

typedef struct FieldHeadTemplate {
    int upperThreshold;
    int lowerThreshold;
    int blendAmount;
    float rampBase;
    float upperSlope;
    float rampFloor;
    float blendTarget;
    float lowerSlope;
} FieldHeadTemplate;

struct Color4 {
    float e[4];
};

// Field_InitModelRuntimeTables -- 144 bytes
// Sets the first flag of all eighteen D011F0 records, then copies four initialized templates
// into their runtime buffers. The original is a one-function contribution: 6496CA ends in
// twelve CC and the 129 code bytes are followed by fifteen LINK CC before aligned 649770.
typedef struct FieldTrigTable {
    short v[256][2];
} FieldTrigTable;

typedef struct FieldUnitVectorTable {
    short v[240][4];
} FieldUnitVectorTable;

typedef struct FieldIndexListA {
    unsigned char v[238];
} FieldIndexListA;

typedef struct FieldIndexListB {
    unsigned char v[102];
} FieldIndexListB;

// Original frames contain three separated 16-byte VECTOR slots.
// Callees read/write only their bounded X/Y/Z lanes.
typedef struct FieldGeometryFrame {
    int index;
    VECTOR moved;
    VECTOR closest;
    VECTOR current;
    struct FieldTriggerCrossingPoint *closestPtr;
    int movedSide;
    struct FieldTriggerCrossingPoint *currentPtr;
    struct FieldTriggerCrossingPoint *movedPtr;
    int currentSide;
    int distance;
} FieldGeometryFrame;


typedef struct SaveMapCopy {
    unsigned char data[4340];
} SaveMapCopy;

/* Field model runtime record: packed command bytes and polygon ramp settings. */
typedef struct FieldModelRuntimeRecord {
    short rampPos;
    short rampCounter;
    short rampPhase;
    short rampStep;
    short animFrame;
    short animTick;
    short effectFlags;
    short rampLimit;
    short rampRate;
    short curveShiftX;
    short curveShiftY;
    short curveShiftZ;
    short curveShiftBy;
    short curveOffset;
} FieldModelRuntimeRecord;

/* Field model command record of byte pairs that combine into 16-bit values for the model command handlers. */
typedef struct FieldModelCommandRecord {
    unsigned char command;
    char pHigh;
    unsigned char qLow;
    unsigned char qHigh;
    unsigned char rLow;
    unsigned char rHigh;
    unsigned char param3Low;
    unsigned char param3High;
    unsigned char param4Low;
    unsigned char param4High;
    unsigned char param5Low;
    unsigned char param5High;
    unsigned char param6Low;
    unsigned char param6High;
    unsigned char param7Low;
    unsigned char param7High;
    unsigned char param8Low;
    unsigned char param8High;
    unsigned char param9Low;
    unsigned char param9High;
    unsigned char param10Low;
    unsigned char param10High;
    unsigned char param11Low;
    unsigned char param11High;
    unsigned char param12Low;
    unsigned char param12High;
    unsigned char param13Low;
    unsigned char param13High;
    unsigned char param14Low;
    unsigned char param14High;
} FieldModelCommandRecord;

/* Screen shake state: random-table index, amplitude, current and previous offsets, step counters. */
typedef struct FieldScreenShake {
    char active;
    char phase;
    char randomIndex;
    char blendedOffset;
    short amplitude;
    short previousOffset;
    short currentOffset;
    short step;
    short stepCount;
} FieldScreenShake;


/* Camera clamp rectangle and mode used by Field_ClampCameraToDiagonal. */
typedef struct FieldCameraClamp {
    unsigned char pad_00[0xc];
    short minX;
    short minY;
    short maxX;
    short maxY;
    unsigned char clampMode;
} FieldCameraClamp;

/* Polygon ramp cycle state: a count and kind codes that select the ramp. */
typedef struct FieldPolygonRampRecord {
    int count;
    int restartTraversal;
    int active0;
    int kind0;
    int subKind0;
    int active1;
    int kind1;
    unsigned char pad_1C[0x4];
    int active2;
    unsigned char pad_24[0x8];
    int active3;
} FieldPolygonRampRecord;

/* 4x4 int matrix (row stride 0x10) whose Y and Z axes Field_SwapMatrixYZAxes swaps. */
typedef struct FieldMatrix4x4 {
    unsigned char pad_00[0x4];
    int m01;
    int m02;
    unsigned char pad_0C[0x4];
    int m10;
    int m11;
    int m12;
    int m13;
    int m20;
    int m21;
    int m22;
    int m23;
    unsigned char pad_30[0x4];
    int m31;
    int m32;
} FieldMatrix4x4;

/* Field quad record (0x4C bytes): image/batch handle, four corner positions, four UV pairs, depth, and code/flags. */
typedef struct FieldQuadRecord {
    union {
        void *image;
        struct GfxRenderObject *batchHandle;
    };
    union {
        float position[8];
        int positionBits[8];
        struct {
            union { int vertex0X; float vertex0XFloat; };
            union { int vertex0Y; float vertex0YFloat; };
            union { int vertex1X; float vertex1XFloat; };
            union { int vertex1Y; float vertex1YFloat; };
            union { int vertex2X; float vertex2XFloat; };
            union { int vertex2Y; float vertex2YFloat; };
            union { int vertex3X; float vertex3XFloat; };
            union { int vertex3Y; float vertex3YFloat; };
        };
    };
    union {
        float texcoord[8];
        struct {
            union { int vertex0Tu; float vertex0TuFloat; };
            union { int vertex0Tv; float vertex0TvFloat; };
            union { int vertex1Tu; float vertex1TuFloat; };
            union { int vertex1Tv; float vertex1TvFloat; };
            union { int vertex2Tu; float vertex2TuFloat; };
            union { int vertex2Tv; float vertex2TvFloat; };
            union { int vertex3Tu; float vertex3TuFloat; };
            union { int vertex3Tv; float vertex3TvFloat; };
        };
    };
    union {
        float depth;
        float quadZFloat;
        int quadZ;
    };
    union {
        int code;
        int flagsWord;
        struct {
            char flags;
            unsigned char pad_49[0x3];
        };
    };
} FieldQuadRecord;
typedef char CheckFieldQuadRecord[(sizeof(FieldQuadRecord) == 0x4C) ? 1 : -1];

/* Field line-batch node: a batch node handle and four scaled short coordinates. */
typedef struct FieldLineBatchRect {
    struct GfxRenderObject *batchNode;
    unsigned char colorChannel0;
    unsigned char colorChannel1;
    unsigned char colorChannel2;
    unsigned char pad_07[0x3];
    short startX;
    short startY;
    short endX;
    short endY;
} FieldLineBatchRect;

/* Four texture slot handles allocated for a field model's textures. */
typedef struct FieldTexturedSlots {
    unsigned char pad_0[0x178];
    union {
        int player; /* +0x178 */
        struct FieldHeadPlayer *headPlayer;
    };
    unsigned char pad_17c[0x4];
    int hundredA;
    int hundredB;
    int hundredC;
    int hundredD;
} FieldTexturedSlots;

/* Five packed 16-bit words stored as byte pairs in a field effect packet. */
typedef struct FieldModelEffectPacket {
    char word0Lo;
    char word0Hi;
    char word1Lo;
    char word1Hi;
    char word2Lo;
    char word2Hi;
    char word3Lo;
    char word3Hi;
    char word4Lo;
    unsigned char word4Hi;
} FieldModelEffectPacket;

/* 32-bit integer 3-vector used by the cross product helper. */
typedef struct FieldVec3i {
    int x;
    int y;
    int z;
} FieldVec3i;

/* Per-human model instance (0x190-byte stride) in g_pFieldModelSlots, updated by Field_UpdateModelInstances. */
typedef struct FieldModelInstance {
    int enabled;
    unsigned char pad_04[0x4];
    int lastFrame;
    unsigned char pad_0C[0xc];
    int holdLastFrame;
    unsigned char pad_1C[0x4];
    int frameIndex;
    int isPlaying;
    unsigned char pad_28[0xc];
    int skeletonAnim;
    unsigned char pad_38[0x4];
    int transformedX;
    int transformedY;
    int transformedZ;
    unsigned char pad_48[0x6c];
    union {
        int skeleton;
        struct FieldModelAlphaGroups *alphaGroups;
    };
    int drawContext;
} FieldModelInstance;

/* Request for textured-hundred slot allocations: an enable byte and four slot counts. */
typedef struct FieldTexturedAllocRequest {
    char enabled;
    unsigned char pad_01[0x3];
    int countA;
    int countB;
    int countC;
    int countD;
} FieldTexturedAllocRequest;

/* RGB bytes of a vertex colour; a bias is added to each channel and clamped to 0..255. */
typedef struct FieldVertexColor {
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
} FieldVertexColor;

/* Packed 32-bit BGRA fade colour word and its byte channels. */
typedef union {
    int packed;
    struct {
        unsigned char blue;
        unsigned char green;
        unsigned char red;
        unsigned char alpha;
    };
} FieldPackedBgra;

/* Field entity state for aiming at a walk target: last and current target, heading, speed. */
typedef struct FieldEntityWalkAim {
    unsigned char pad_00[0xc];
    int fromX;
    int fromY;
    unsigned char pad_14[0x21];
    unsigned char headingOffset;
    char heading;
    unsigned char pad_37[0x3b];
    unsigned short radius;
    unsigned char pad_74[0x2];
    unsigned short speed;
    unsigned char pad_78[0x4];
    int toX;
    int toY;
} FieldEntityWalkAim;

/* Gateway line segment tested against the moving party's sides. */
typedef struct FieldGateway {
    short startX;
    short startY;
    unsigned char pad_04[0x2];
    short endX;
    short endY;
    unsigned char pad_0A[0x8];
    unsigned short eventCmdParam;
    unsigned char pad_14[0x4];
} FieldGateway;

/* Position and state of a trigger marker (a hand-pointer arrow drawn at a field trigger). */
typedef struct FieldGatewayMarker {
    short x;
    unsigned char pad_02[0x2];
    short y;
    unsigned char pad_06[0x2];
    short z;
    unsigned char pad_0A[0x2];
    int state; /* 0: none, 2: type-2 trigger (drawn with quad code 4) */
} FieldGatewayMarker;

/* Camera bounds, scroll wrap sizes, layer scroll offsets and parallax factors for field scrolling.
 * g_pFieldScrollBounds points at this record, the field's trigger block. */
typedef struct FieldBoundsRect {
    unsigned char pad_00[0x9];
    char headingDelta;
    short viewOffset;
    short minX;
    short minY;
    short maxX;
    short maxY;
    unsigned char pad_14[0x4];
    short tileWidth;
    short tileHeight;
    short tileStrideX;
    short tileStrideY;
    short layer2ScrollX;
    short layer2ScrollY;
    short layer3ScrollX;
    short layer3ScrollY;
    short layer2ParallaxX;
    short layer2ParallaxY;
    short layer3ParallaxX;
    short layer3ParallaxY;
    unsigned char pad_30[0x1e8];
    unsigned char gatewayArrowShown[12]; /* 1: draw the gateway arrow of this gateway box */
    FieldGatewayMarker triggerMarkers[12];
} FieldBoundsRect;

/* Batch node for Field_EmitBatchNodeTexturedQuad; holds quad buffers and an output byte pointer. */
typedef struct FieldBatchNode {
    unsigned char pad_00[0xc];
    int hundredRecord;
    unsigned char pad_10[0x60];
    int quadBuffers;
    unsigned char pad_74[0x4];
    int outBytePtr;
    int metaId;
} FieldBatchNode;

/* Field light record: short position words and two packed BGRA colours. */
typedef struct FieldLightSource {
    short posX; /* +0x0 */
    short posY;
    short posZ;
    short range;
    int ambientColor;
    int light0Color;
} FieldLightSource;

/* Camera x and y shorts, clamped to the field trigger rectangle. */
typedef struct FieldCameraCoordsRect {
    short x;
    short y;
} FieldCameraCoordsRect;

/* Adjacent-map preload block: a map point compared against the field position. */
typedef struct FieldAdjacentMapPreload {
    int centerX;
    int centerY;
    int centerZ;
} FieldAdjacentMapPreload;

/* Curve step record: six shorts read during curve evaluation. */
typedef struct FieldCurveStepRecord {
    short magnitude; /* +0x0 */
    short shiftX;
    short shiftY;
    short shiftZ;
    short shiftBy;
    short offset;
    short phaseDelta;
} FieldCurveStepRecord;

/* Two-component direction used by the walkmesh slide test. */
typedef struct FieldSlideNormal {
    int normalX;
    int normalY;
} FieldSlideNormal;

/* Script line trigger: position in fixed point, facing angle and trigger radius. */
typedef struct FieldScriptLineTrigger {
    unsigned char pad_00[0xc];
    int x;
    int y;
    int z;
    unsigned char pad_18[0x1e];
    unsigned char angle;
    unsigned char pad_37[0x3b];
    unsigned short radius;
} FieldScriptLineTrigger;

/* Model command packet: command word split into bytes for the command frame. */
typedef struct FieldModelCommandPacket {
    short commandWord;
    short qWord;
    short rWord;
} FieldModelCommandPacket;

/* Runtime flags block read by the field polygon ramp cycle. */
typedef struct FieldRuntimeFlags {
    char active;
    char modelIndex;
} FieldRuntimeFlags;

/* Door trigger entry (0x10 bytes): the segment, byte and bit index into the door-trigger flag array, a mode and a sound index. */
typedef struct FieldDoorTriggerRecord {
    short startX;
    short startY;
    unsigned char pad_04[0x2];
    short endX;
    short endY;
    unsigned char pad_0A[0x2];
    unsigned char byteIndex;
    unsigned char bitIndex;
    unsigned char mode;
    unsigned char soundIndex;
} FieldDoorTriggerRecord;

/* Script trigger line: a 3-D point, a facing byte and a trigger radius for the crossing test. */
typedef struct FieldScriptTriggerLine {
    unsigned char pad_00[0xc];
    int posX;
    int posY;
    int posZ;
    unsigned char pad_18[0x1e];
    unsigned char direction;
    unsigned char pad_37[0x3b];
    unsigned short radius;
} FieldScriptTriggerLine;

/* hub FieldModelLightCache begin */
/* A field model's light cache: an active flag, then the cached lighting. */
#pragma pack(push, 1)
struct FieldModelLightCache {
    int isActive; /* +0x0 */
    unsigned char lightCache[0x7530]; /* +0x4 */
};
#pragma pack(pop)
/* hub FieldModelLightCache end */

/* Palette rectangle read by Field_ReadPaletteRect: start row, copy width and row count. */
typedef struct FieldPaletteRect {
    int startColumn; /* +0x0 */
    int startRow;
    int copyWidth;
    int rowCount;
} FieldPaletteRect;

/* Field point light: a tint and mix weights as packed BGRA words. */
typedef struct FieldPointLight {
    short posX;
    short posY;
    short posZ;
    short range;
    int tint;
    int mixWeights;
} FieldPointLight;

/* Field actor record; head mesh commands read the player index and group slots from it. */
typedef struct FieldActorRecord {
    unsigned char pad_00[0x20];
    char partyId;
    unsigned char pad_21[0x157];
    int player;
    unsigned char pad_17C[0x8];
    int groupSlot;
    unsigned char pad_188[0x4];
    int headSlot;
} FieldActorRecord;

/* Gateway exit record copied into the field actor state when the player leaves. */
typedef struct FieldGatewayRecord {
    short pad_00;
    short pad_02;
    unsigned char pad_04[0x2];
    short pad_06;
    short pad_08;
    unsigned char pad_0A[0x2];
    short pcTileX;
    short pcTileY;
    short walkMeshId;
    short eventCmdParam;
    unsigned char entryId;
    char pad_15;
    unsigned char pad_16;
} FieldGatewayRecord;

/* Model flag table: entry count followed by per-entry flags applied by Field_ApplyModelRecordOverrides. */
typedef struct FieldModelFlagTable {
    int count;
    int isOverridden;
} FieldModelFlagTable;

/* Field model record that tracks its current animation index, animation player and animation table. */
typedef struct FieldModel {
    unsigned char pad_00[0x22];
    unsigned short animCount;
    unsigned char pad_24[0x150];
    short animIndex;
    unsigned char pad_176[0x2];
    int animPlayer;
    int animTable;
} FieldModel;

/* Quarter-turn about X rotation matrix written by Field_BuildQuarterTurnAboutXMatrix. */
typedef struct FieldRotationMatrix {
    unsigned char pad_00[0x14];
    int m11;
    int m12;
    unsigned char pad_1C[0x8];
    int m21;
    int m22;
} FieldRotationMatrix;

/* Model placement whose position floats are offset by a scaled delta in Field_ApplyScaledOffsetToPlacement. */
typedef struct FieldPlacement {
    unsigned char pad_00[0x3c];
    float posX;
    float posY;
    float posZ;
} FieldPlacement;

/* Decoded 16-bit operands of a field model command, built from the command record's high and low bytes. */
typedef struct FieldModelCommand {
    short valueP;
    short valueQ;
    short valueR;
} FieldModelCommand;

/* 16-bit vector operand of Field_VecSub16To32, which writes a 32-bit difference. */
typedef struct FieldVec16 {
    short x;
    short y;
    short z;
} FieldVec16;

/* Model header with a group count and group array for the alpha 192 pass. */
typedef struct FieldModelAlphaGroups {
    unsigned char pad_00[0x10];
    int groupCount;
    struct FieldDiffuseSelectorGroup *groups;
} FieldModelAlphaGroups;

/* Anim draw record of indexed commands applied per frame. */
typedef struct FieldAnimDrawCommands {
    unsigned char pad_00[0x10];
    int commandCount;
    int commands;
} FieldAnimDrawCommands;

/* Integer point, the to end of a heading calculation. */
typedef struct FieldPointTo {
    int x;
    int y;
} FieldPointTo;

/* Animation player with a frame count and a current frame. */
typedef struct FieldAnimPlayer {
    unsigned char pad_00[0x8];
    int frameCount;
    unsigned char pad_0C[0x14];
    int currentFrame;
} FieldAnimPlayer;

/* Float light position whose y and z are stored as shorts. */
typedef struct FieldLightPosition {
    float x;
    float y;
    float z;
    unsigned char pad_0C[0x4];
    int color;
    unsigned char pad_14[0xc];
} FieldLightPosition;

/* One sample point of a layer 2 tile: a pixel position and the arguments of its brightness lookup. */
typedef struct FieldLayer2TileSample {
    unsigned short x;              /* +0x0 */
    unsigned short y;              /* +0x2 */
    unsigned char pad_04[0x2];
    unsigned char brightnessMode;  /* +0x6 */
    unsigned char pad_07[0x1];
    short brightnessDelta;         /* +0x8 */
    short pixel;                   /* +0xA: the last pixel written back */
    unsigned char pad_0C[0x4];
} FieldLayer2TileSample;

/* One Layer 2 background tile record in g_FieldLayer2Tiles (0x105C bytes per record). */
typedef struct FieldLayer2Tile {
    short tileX;
    short spanY;
    float depthZ;
    unsigned short srcX;
    unsigned short srcY;
    unsigned short texX;
    unsigned short texY;
    float texU;
    float texV;
    unsigned short altTexX;
    unsigned short altTexY;
    int sourceRect[4];
    unsigned short width;
    unsigned short height;
    unsigned short paletteRow;
    unsigned short sortKey;
    unsigned char zBufferValue;
    unsigned char visibilityMask;
    unsigned char pad_36[0x2];
    FieldLayer2TileSample samples[256];
    unsigned short sampleCount;
    unsigned char pad_103A[0x2];
    int inUse;
    int active;
    int drawn;
    int sampled;
    int clearedWord;
    int usesSecondTim;
    unsigned short transparencyMode;
    unsigned short textureId;
    unsigned short texPage;
    unsigned short colorMode;
} FieldLayer2Tile;

/* Background layer 3 tile record, one 0x105C-byte entry in g_FieldLayer3Tiles. */
typedef struct FieldBgLayer3Tile {
    short xPos;
    short tileY;
    float scale;
    unsigned short srcX;
    unsigned short srcY;
    unsigned short texelU;
    unsigned short texelV;
    float texU;
    float texV;
    unsigned short tileX;
    unsigned short posY;
    unsigned char pad_1C[0x10];
    unsigned short tileWidth;
    unsigned short tileHeight;
    unsigned short paletteRow;
    unsigned short fixedScale;
    unsigned char zDepth;
    unsigned char zFlags;
    unsigned char pad_36[0x1002];
    short freeMark;
    unsigned char pad_103A[0x2];
    int ownerTag;
    int inUse;
    unsigned char pad_1044[0xc];
    int flags;
    unsigned short dstX;
    unsigned short texId;
    unsigned short mapId;
    unsigned short dstY;
} FieldBgLayer3Tile;

/* Background layer 4 tile record, one 0x105C-byte entry in g_FieldLayer4Tiles. */
typedef struct FieldBgLayer4Tile {
    short posX;
    short posY;
    float scale;
    unsigned short srcX;
    unsigned short srcY;
    unsigned short texU;
    unsigned short texV;
    float texUNorm;
    float texVNorm;
    unsigned short tileX;
    unsigned short tileY;
    unsigned char pad_1C[0x10];
    unsigned short tileWidth;
    unsigned short tileHeight;
    unsigned short paletteRow;
    unsigned short fixedScale;
    unsigned char zDepth;
    unsigned char zFlags;
    unsigned char pad_36[0x1002];
    short freeMark;
    unsigned char pad_103A[0x2];
    int active;
    int inUse;
    unsigned char pad_1044[0xc];
    int flags;
    unsigned short dstX;
    unsigned short textureId;
    unsigned short texSlot;
    unsigned short dstY;
} FieldBgLayer4Tile;

/* One 0x105C-byte background layer-1 tile record, indexed per tile slot in the field's layer-1 table. */
typedef struct FieldLayer1Tile {
    short posX;
    short depth;
    float scaleFactor;
    unsigned short srcX;
    unsigned short srcY;
    unsigned short texX;
    unsigned short texY;
    float texU;
    float texV;
    unsigned short altTexX;
    unsigned short altTexY;
    unsigned char pad_1C[0x10];
    unsigned short width;
    unsigned short height;
    unsigned short paletteRow;
    unsigned short sortKey;
    unsigned char zBufferValue;
    unsigned char visibilityMask;
    unsigned char pad_36[0x1006];
    int active;
    unsigned char pad_1040[0x4];
    int drawn;
    unsigned char pad_1048[0x8];
    int isSet;
    unsigned short transparencyMode;
    unsigned short mapId;
    unsigned short texPage;
    unsigned short pixelMode;
} FieldLayer1Tile;

/* Another name for FieldBoundsRect, the record g_pFieldScrollBounds points at. */
typedef FieldBoundsRect FieldCameraScrollRecord;

/* One cell of the field palette grid: a palette index, a mode and a dirty flag. */
typedef struct FieldPaletteGridCell {
    short pixelWord;
    unsigned char pixelFlagA;
    unsigned char pixelFlagB;
    unsigned char pixelFlagC;
    unsigned char paletteIndex;
    unsigned char mode;
    unsigned char clearedByte;
    int dirty;
} FieldPaletteGridCell;

/* One 0x18-byte tile slot of an ad-list bank: screen x and y, depth, texture u and v
 * (Field_EmitBackgroundTile stores them, Field_BuildBackgroundQuads reads them) and an attribute word.
 * Floats the code copies as ints have an int spelling beside them. */
typedef struct FieldAdListEntry {
    union { int posXBits; float posX; };
    union { int posYBits; float posY; };
    union { int depthBits; float depth; };
    union { int texUBits; float texU; };
    union { int texVBits; float texV; };
    union { int attrD; char attrDByte; };
} FieldAdListEntry;

typedef struct FieldAdListBank {
    int count;
    FieldAdListEntry entries[0x100];
} FieldAdListBank;

/* Field tile group sprite: screen position, source offset and CLUT index. */
typedef struct FieldTileGroup {
    unsigned char pad_00[0x4];
    unsigned char indexLow;
    unsigned char indexHigh;
    unsigned char pad_06[0x2];
    short posX;
    short posY;
    unsigned char offsetX;
    unsigned char offsetY;
    short clut;
} FieldTileGroup;

/* Per-model record of the field human animation array: direction, animations, model, anim id. */
typedef struct FieldModelSlot {
    char unmapFlag;
    unsigned char pad_01[0x3];
    int worldX;
    int worldY;
    int worldZ;
    int initZero_10;
    int initZero_14;
    short scale; /* 4096 = 1.0, from the model definition's scale text */
    short depth;
    short direction;
    unsigned char pad_1E[0x2];
    char partyId; /* Field_GetPartyIdFromHrcName of the model's .hrc name */
    char effectPreset;
    unsigned short animationCount;
    int animTable[0x51];
    int active;
    int entityVisible;
    int reserved_170;
    unsigned short animId;
    unsigned char pad_176[0x2];
    struct FieldModelInstance *model;
    int animations;
    unsigned char pad_180[0x10];
} FieldModelSlot;

/* One tile entry in the field tile palette buffer: sprite offsets and CLUT index. */
typedef struct FieldTilePaletteEntry {
    unsigned char pad_00[0x4];
    unsigned char indexLow;
    unsigned char indexHigh;
    unsigned char pad_06[0x2];
    short xPos;
    short tileY;
    unsigned char offsetX;
    unsigned char offsetY;
    short clut;
    unsigned char pad_10[0x4];
} FieldTilePaletteEntry;

/* Tile sprite record filled from raw tile data by Field_BuildTileSprites. */
typedef struct FieldTileSprite {
    unsigned char pad_00[0x4];
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char pad_07[0x1];
    short x;
    short y;
    unsigned char texU;
    unsigned char texV;
    short clut;
    short width;
    short height;
} FieldTileSprite;

/* Field window text-reveal state; pointer per window index in g_FieldWindowTextPointers. */
typedef struct FieldWindowTextState {
    unsigned char code;
    unsigned char command;
    unsigned char valueLo;
    unsigned char valueHi;
    unsigned char countLo;
    unsigned char countHi;
    unsigned char pad_6[0xa];
} FieldWindowTextState;

/* Field tile sprite entry built by Field_BuildTileSprites from a tile node. */
typedef struct FieldSmallTileSprite {
    unsigned char pad_00[0x4];
    unsigned char indexLow;
    unsigned char indexHigh;
    unsigned char depth;
    unsigned char pad_07[0x1];
    short posX;
    short posY;
    unsigned char texU;
    unsigned char texV;
    short clut;
} FieldSmallTileSprite;

/* Header at the start of the field script blob: counts, dialogue offset and field scale. After
 * the 0x20-byte header come an 8-byte name per entity, akaoBlockCount 4-byte AKAO block offsets
 * (Script_GetAkaoBlockOffset) and 32 two-byte script entries per entity (FIELD_SCRIPT_ENTRY_LO/HI). */
typedef struct FieldScriptHeader {
    unsigned char pad_00[0x2];
    unsigned char entityCount;
    unsigned char modelCount;
    unsigned short dialogTextOffset;
    unsigned short akaoBlockCount; /* 4-byte AKAO block offsets after the entity names */
    short fieldScale;
} FieldScriptHeader;

/* g_FieldCharEntity (0xCC0998, 360 bytes): the field entity that plays each character, indexed by
 * character id (Script_OpBindCharacter writes it, Script_ResetState resets the first nine to 0xFF),
 * then per entity and priority the entity that requested that script with mode 3 of
 * Script_RequestEntityScript and waits for it to finish (0xFF: none). 48 rows, as the two other
 * per-entity and per-priority tables Script_ResetState clears in the same loop
 * (g_FieldScriptPriorityPC[0x30][8], g_FieldScriptPriorityState[0x30][8]). */
typedef struct FieldCharEntityMap {
    unsigned char charEntity[0x18];
    unsigned char scriptCaller[0x30][8];
} FieldCharEntityMap;
typedef char CheckFieldCharEntityMap[(sizeof(FieldCharEntityMap) == 0x198) ? 1 : -1];

/* Tile sprite record (0x18 bytes): texture page and animation frame range. */
typedef struct FieldTileAlphaRecord {
    short tPage;
    unsigned short frameCount;
    unsigned short frameStart;
    unsigned char pad_06[0x12];
} FieldTileAlphaRecord;

/* Descriptor of one image uploaded to the field back surface: pixel pointer and size. */
typedef struct FieldUploadDescriptor {
    unsigned char pad_00[0x1c];
    int pixels;
    unsigned char pad_20[0x8];
    unsigned short width;
    unsigned short height;
    unsigned char pad_2C[0x4];
} FieldUploadDescriptor;

/* Human animation table entry; animRecord points at its FieldModelInstance. */
typedef struct FieldHumanAnimEntry {
    unsigned char pad_00[0x22];
    unsigned short animId;
    unsigned char pad_24[0x148];
    int entityVisible;
    unsigned char pad_170[0x8];
    int animRecord;
    unsigned char pad_17C[0x14];
} FieldHumanAnimEntry;

/* One 4-byte BGRA palette entry written by the palette row conversion. */
typedef struct FieldPaletteEntryBgra {
    char blue;
    char green;
    char red;
    char alpha;
} FieldPaletteEntryBgra;

/* Packed color record (0xC bytes) built per cell by Field_BuildPackedColorRecords. */
typedef struct FieldPackedColorRecord {
    short pixelWord;
    unsigned char pad_02[0x4];
    char mode;
    unsigned char pad_07[0x5];
} FieldPackedColorRecord;

/* Current fixed-point position that the script trigger crossing test records. */
typedef struct FieldTriggerCrossingPoint {
    int posX;
    int posY;
    int posZ;
} FieldTriggerCrossingPoint;

/* Record of the palette pool buffer that the palette builder and allocator fill. */
typedef struct FieldPalettePoolRecord {
    int capacity;
    int transparentIndices[0x100];
    int usedSlots;
    int count;
} FieldPalettePoolRecord;

/* One script line of g_FieldLineDrawScratch (0x18 bytes): two endpoints stored as SVECTOR-shaped
   triples (the line is drawn between them), a countdown, a random-table index, a base Z and the enable flag. */
typedef struct FieldLineDrawScratch {
    short upperX;
    short upperY;
    short upperZ;
    unsigned char pad_06[0x2];
    short lowerX;
    short lowerY;
    short lowerZ;
    unsigned char pad_0E[0x2];
    short stepState;
    short randomIndex;
    short baseZ;
    short active;
} FieldLineDrawScratch;

/* Camera section data: scroll offsets for the palette origin and a geometry screen value. */
typedef struct FieldCameraData {
    short matrix[3][3];
    unsigned char pad_12[0x2];
    int translation[3];
    short scrollOffsetX;
    short scrollOffsetY;
    short geomScreen;
} FieldCameraData;

/* Gateway trigger box given as two corner values per axis, averaged to its centre. */
typedef struct FieldGatewayBox {
    short x0;
    short y0;
    short z0;
    short x1;
    short y1;
    short z1;
    unsigned char pad_0C[0xc];
} FieldGatewayBox;

/* One 0x18-byte lighting record of a polygon set group (the same record as GfxLightVertexRecord):
 * the vertex normal's address, a float light colour, and the tag the group's vertices are matched by. */
typedef struct FieldPolygonLightingRecord {
    int normal;
    float color[4];
    int tag;
} FieldPolygonLightingRecord;

/* Field tile draw order entry: tile index and sort key taken from the layer 2 tile. */
typedef struct FieldTileDrawOrder {
    short tileIndex;
    short sortKey;
} FieldTileDrawOrder;

/* One 4-byte entry of a background layer's draw-order index list: written as a 32-bit index by
 * Field_BuildTileDrawOrder and read as its low 16-bit tile index by the layer draw loops. */
typedef struct FieldLayerDrawOrder {
    union {
        int index;
        short index16;
    };
} FieldLayerDrawOrder;

/* 8-byte walkmesh vertex with three short coordinates. */
typedef struct FieldWalkmeshVertex {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
} FieldWalkmeshVertex;

/* 16-byte human actor buffer record cleared to zero by the buffer reset. */
typedef struct FieldHumanActorBuffer0 {
    int clearedWord0;
    int clearedWord1;
    int clearedWord2;
    unsigned char pad_0C[0x4];
} FieldHumanActorBuffer0;

/* Field effect parameter block that g_pFieldFx points at. */
typedef struct FieldFxParams {
    unsigned char pad_00[0x9e];
    unsigned char paramByte;
    unsigned char pad_9F[0x5];
    short paramWord;
} FieldFxParams;

/* Field model sort record filled once per model instance. */
typedef struct FieldModelSortRecord {
    int sortKey;
    short depth;
    short screenX;
    short screenY;
    unsigned char pad_0A[0x6];
} FieldModelSortRecord;

/* Model record override entry; overrideFlag is a flag value set per entity. */
typedef struct FieldModelOverrideRecord {
    unsigned char pad_00[0x8];
    int overrideFlag;
} FieldModelOverrideRecord;

/* Polygon ramp record; active is nonzero when the ramp frame applies. */
typedef struct FieldPolygonRampEntry {
    unsigned char pad_00[0x8];
    int active;
} FieldPolygonRampEntry;

/* Field layer record with a data buffer, dumped by the field debug writer. */
typedef struct FieldLayer {
    union {                      /* +0x00: the texture record (Gfx_AllocTexRecord) */
        int texRecord;
        struct GfxTexHeader *texHeader;
    };
    union {                      /* +0x04: 256x256 pixels of bytesPerPixel bytes each */
        unsigned char *pixels;
        unsigned short *pixels16;
    };
    union {                      /* +0x08: the render object and batch node the layer draws through */
        int renderObject;
        struct GfxRenderObject *batch;
        FieldBatchNode *batchNode;
    };
    union {                      /* +0x0C: nonzero once tiles were composed into the layer */
        int enabled;
        unsigned short enabled16; /* the debug dump writes only the low word */
    };
    int clearedState;                    /* +0x10: cleared with built */
    int built;                   /* +0x14: primitives were emitted this frame */
    unsigned short hasDoubleSizeTiles; /* +0x18: the 32x32 tiles of layers 3 and 4 */
    unsigned short bytesPerPixel; /* +0x1A: 1 for the 8-bit layers 0..0x19, else 2 */
} FieldLayer;

/* Script-line trigger record (0x18 bytes) tested by the script-line trigger routines. */
typedef struct FieldTriggerRecord {
    short startX;
    short startY;
    unsigned char pad_04[0x2];
    short endX;
    short endY;
    unsigned char pad_0A[0x2];
    char mode;
    unsigned char pad_0D[0x1];
    unsigned char state;
    unsigned char soundIndex;
    char segmentHit;
    char actionFlag;
    char hit;
    char fired;
    unsigned char heading;
    char crossed;
    unsigned char enabled;
    unsigned char pad_17[0x1];
} FieldTriggerRecord;

/* Ramp record: three target values and their signed rates, written from a packet. */
typedef struct FieldRampModelDelta {
    short currentA; /* +0x0 */
    short currentB;
    short currentC;
    short targetA;
    short targetB;
    short targetC;
    short rateA;
    short rateB;
    short rateC;
    unsigned char deltaParam; /* copied to byte 6 of the command record */
    unsigned char finished;   /* set once all three values reached their targets */
} FieldRampModelDelta;

/* Field model matrix record: nine little-endian shorts copied from the packet into the scratch buffer. */
typedef struct FieldModelMatrix {
    unsigned char pad_00[0xc];
    short matrixElem0;
    short matrixElem1;
    short matrixElem2;
    short matrixElem3;
    short matrixElem4;
    short matrixElem5;
    short matrixElem6;
    short matrixElem7;
    short matrixElem8;
} FieldModelMatrix;

/* Raw tile entry whose x and y are copied into a FieldTileSprite by Field_BuildTileSprites. */
typedef struct FieldTileRawEntry {
    short x;
    short y;
    unsigned char texU;
    unsigned char texV;
    short clut;
    char attribute[2];           /* copied to the attribute table; 0xa bytes in all, the step of every pointer through it */
} FieldTileRawEntry;

/* Tile node whose position and draw fields are copied into tile sprites. */
typedef struct FieldTileNode {
    short posX;
    short posY;
    unsigned char texU;
    unsigned char texV;
    short clut;
    short tPage;
    unsigned short packedIndex;
    char attribute[2];           /* copied to the attribute table; 0xe bytes in all, the step of every pointer through it */
} FieldTileNode;

/* Field tile sprite record with a layer index and a span value. */
typedef struct FieldTileGroupEntry {
    short marker; /* 0x7fff ends a list, 0x7ffe closes a draw group; any other value starts a run of tiles */
    short firstTile;
    short tileCount;
} FieldTileGroupEntry;

/* Source record whose position and attributes are copied into a FieldSmallTileSprite entry. */
typedef struct FieldTileSpriteSource {
    short posX;
    short posY;
    unsigned char texU;
    unsigned char texV;
    short clut;
} FieldTileSpriteSource;

/* Map gateway record with a position, used by the adjacent-map preload distance check. */
typedef struct FieldMapGateway {
    short posX;
    short posY;
    unsigned char pad_04[0xe];
    unsigned short targetMap;
    unsigned char pad_14[0x4];
} FieldMapGateway;

/* Tile sprite depth record; a 16-bit depth is built from its low and high bytes. */
typedef struct FieldTileSpriteDepthTile {
    unsigned char pad_00[0x4];
    unsigned char depthLow;
    unsigned char depthHigh;
} FieldTileSpriteDepthTile;

/* Model vertex (0x20-byte D3DTLVERTEX) with a float position and a packed BGRA color word. */
typedef struct FieldLightVertex {
    union {
        float x;
        int xBits;
    };
    union {
        float y;
        int yBits;
    };
    float z;
    unsigned char pad_0C[0x4];
    int color;
    unsigned char pad_14[0xc]; /* to 0x20 bytes, the step of every pointer through it */
} FieldLightVertex;

/* Player position in 12-bit fixed point; the adjacent-map preload reads its x, y and z. */
typedef struct FieldPlayerPos {
    unsigned char pad_00[0xc];
    int posX;
    int posY;
    int posZ;
} FieldPlayerPos;

/* The polygon set a selector entry draws, behind FieldSelectorEntry.polySetHolder. */
typedef struct FieldSelectorPolySet {
    unsigned char pad_00[0x4];
    struct GfxPolygonSet *polySet;
} FieldSelectorPolySet;

/* One 8-byte entry of a diffuse-selector group; polySetHolder is 0 for an entry without geometry. */
typedef struct FieldSelectorEntry {
    unsigned char pad_00[0x4];
    FieldSelectorPolySet *polySetHolder;
} FieldSelectorEntry;

/* Group of diffuse-selector records with an entry count at 0x10 and a record pointer at 0x18. */
typedef struct FieldDiffuseSelectorGroup {
    unsigned char pad_00[0x10];
    int entryCount;
    unsigned char pad_14[0x4];
    FieldSelectorEntry *records;
} FieldDiffuseSelectorGroup;

/* One entry of the counted array Field_AppendModelPolygonSetRecord appends to and
 * Field_TraverseModelFlagRecord walks: whether it was appended, and the polygon set's counts. */
typedef struct FieldPolygonSetCountEntry {
    int appended;
    int vertexCount;
    int polygonCount;
} FieldPolygonSetCountEntry;

/* The counted array itself; `restart` is set to make the walk start at entry 0 again. */
typedef struct FieldPolygonSetCountArray {
    int count;
    int restart;
    FieldPolygonSetCountEntry entries[1];
} FieldPolygonSetCountArray;

/* Indexed field record whose float at 0x0C feeds a negated command value and whose pointer at 0x18 leads to a transform. */
typedef struct FieldIndexedRecord {
    unsigned char pad_00[0xc];
    float boneLength;
    unsigned char pad_10[0x8];
    struct PolygonSkelModelEntry *chainRef;
} FieldIndexedRecord;

/* Two-word position whose values select a field model effect preset. */
typedef struct FieldEffectPosition {
    short xOffset;
    short yOffset;
} FieldEffectPosition;

/* Fixed-point position pair, shifted right by 12 bits, used by the script-line crossing test. */
typedef struct FieldFixedPosition {
    int posX;
    int posY;
} FieldFixedPosition;

typedef int(__cdecl *FieldOpcodeHandler)(void);

/* One of a field map's two random encounter tables (FieldState.encounterTable picks one; 0x18 bytes apart).
   Each formation word holds the formation id in its low 10 bits and its chance in 64ths in the top 6.
   Field_CheckRandomEncounter tries the special slots first (it halves the chance of 0, 1 and 3 when the
   party's pre-emptive bit 0x80 is set; community lead: back attack, back attack, side attack, pincer),
   then the normal ones, the sixth being the default. */
typedef struct FieldEncounterEntry {
    short flags;          /* bit 0: encounters on; high byte: the rate divisor of the player's speed */
    short normal[6];      /* +0x02 */
    short special[4];     /* +0x0E */
    short pad;            /* +0x16: section 6 stores two 24-byte tables back to back */
} FieldEncounterEntry;

#pragma pack(push, 1)
typedef struct FieldCameraScrollE70 {
    unsigned short clutId; /* +0x0 */
    unsigned short first; /* +0x2 */
} FieldCameraScrollE70;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldLayer1StateE58 {
    unsigned short spanWidth; /* +0x0 */
    unsigned short spanHeight; /* +0x2 */
} FieldLayer1StateE58;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldLayer4StateE8c {
    unsigned short width; /* +0x0 */
    unsigned short height; /* +0x2 */
} FieldLayer4StateE8c;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldLayer2StateE14 {
    unsigned short firstIndex; /* +0x0 */
    unsigned short nextIndex; /* +0x2 */
} FieldLayer2StateE14;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldLayer3StateE20 {
    short originX; /* +0x0 */
    short originY; /* +0x2 */
} FieldLayer3StateE20;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldLayer4StateE98 {
    unsigned short headerWord; /* +0x0 */
    short pageStart; /* +0x2 */
} FieldLayer4StateE98;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldLayer4StateE9c {
    unsigned short pageEnd; /* +0x0 */
    unsigned short directPageStart; /* +0x2 */
} FieldLayer4StateE9c;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldPackedPaletteRows {
    short rowsOrOffset; /* +0x0 */
    short colsOrMode; /* +0x2 */
} FieldPackedPaletteRows;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldHeadTrigTable {
    unsigned char pad_0[0x2];
    int cosineTable; /* +0x2 */
    unsigned char pad_6[0x3fa];
} FieldHeadTrigTable;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldHumanAnimState {
    unsigned char pad_0[0x2];
    unsigned short modelListCountCopy; /* +0x2 */
} FieldHumanAnimState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FieldLayer2StateE0c {
    short pageStart; /* +0x0 */
    unsigned short sectionWord; /* +0x2 */
} FieldLayer2StateE0c;
#pragma pack(pop)

/* Field_ApplyHeadMeshCommands: a chain of links from a bone record down to its polygon set. */
#pragma pack(push, 1)
typedef struct FieldHeadLink {
    unsigned char pad_0[0x4];
    struct FieldHeadLink *next; /* +0x4: on the last link, the GfxPolygonSet */
} FieldHeadLink;

/* One 0x1C-byte bone record of a field model resource. */
typedef struct FieldHeadBoneRecord {
    char *name; /* +0x0 */
    unsigned char pad_4[0x14];
    FieldHeadLink *link; /* +0x18 */
} FieldHeadBoneRecord;

/* A field model resource: its bone records. */
typedef struct FieldHeadResource {
    unsigned char pad_0[0x10];
    int recordCount; /* +0x10 */
    FieldHeadBoneRecord *records; /* +0x14 */
} FieldHeadResource;

/* A field model's player record. */
typedef struct FieldHeadPlayer {
    unsigned char pad_0[0xB4];
    FieldHeadResource *resource; /* +0xB4 */
} FieldHeadPlayer;

/* One 0x64-byte group record of a polygon set; the head mesh reads whether it has a draw state. */
typedef struct FieldHeadMeshGroup {
    unsigned char pad_0[0x14];
    int drawState; /* +0x14 */
    unsigned char pad_18[0x4C];
} FieldHeadMeshGroup;
#pragma pack(pop)

/* The 16-byte record Field_AllocModelRecord returns: three primed words, then the RSD model
 * Field_FreeModelRecord releases. */
typedef struct FieldModelDescriptor {
    int primed[3];
    void *rsdModel;
} FieldModelDescriptor;

/* The clear colour of a field map (FieldMapRecord.clearColor, copied to g_FieldClearColor): four
 * floats, the fade code writes the blue, green and red fade bytes / 255 into m0, m1 and m2 and 1.0
 * into m3. */
typedef struct FieldClearColor {
    float b;
    float g;
    float r;
    float a;
} FieldClearColor;

/* One field map's configuration record, 0xD0 bytes; g_FieldMapRecords[1200] is indexed by map id.
 * Field_InitMapConfigTable writes every record's defaults, its per-map exceptions, the
 * g_FieldMapList_* tables and flevel.siz's sizes; Field_ApplyMapConfig copies the current map's
 * record into the field globals named below. */
typedef struct FieldMapRecord {
    /* +0x00 nonzero: Field_InstantiateModels clears bit 8 of each model's light owner
     * (g_FieldClearModelLightFlag8; set for the 12 maps of g_FieldMapList_ClearLightFlag8). */
    int clearLightFlag8;
    /* +0x04 the camera's near plane (the engine context's nearPlane): 50.0 by default, 512.0 for the
     * 55 maps of g_FieldMapList_NearPlane512. */
    float nearPlane;
    /* +0x08 added to each model's y where the field draws it (g_FieldModelOffsetY). */
    int modelOffsetY[0x12];
    /* +0x50 added to each model's z (with -10) where the field draws it (g_FieldModelOffsetZ). */
    int modelOffsetZ[0x12];
    /* +0x98 added to background layer 2's y scroll (g_FieldBgLayer2OffsetY). */
    int bgLayer2OffsetY;
    /* +0x9C the entity whose animation does not advance while the field renders
     * (g_FieldAnimPausedEntity); -1: none. */
    int animPausedEntity;
    /* +0xA0 nonzero: opcode 0xAE leaves the model holding its animation (mode 3) instead of
     * returning it to idle (g_FieldHoldAnimAfterWait). */
    int holdAnimAfterWait;
    /* +0xA4 subtracted (scaled) from the background's y origin while the camera is near the top
     * (g_FieldBgOriginOffsetY). */
    int bgOriginOffsetY;
    /* +0xA8 a model is drawn only while its byte is nonzero (g_FieldModelVisible); 1 by default. */
    char modelVisible[0x12];
    /* +0xBA nonzero: the line triggers are not tested while the character is locked
     * (g_FieldLockedSkipsLines). */
    char lockedSkipsLines;
    char pad_bb;
    /* +0xBC the map's uncompressed file size: flevel.siz's word + 2,000,000
     * (g_FieldFileUncompressedSize). */
    int fileSize;
    /* +0xC0 the clear colour (g_FieldClearColor): black and opaque by default. */
    FieldClearColor clearColor;
} FieldMapRecord;

/* One entry of the field map list (maplist): a NUL-padded map name. */
typedef char FieldMapName[32];

/* One 0x18-byte draw group: a run of tiles drawn with one texture page. */
typedef struct FieldTileDrawGroup {
    short texPage;
    short tileCount;
    short firstTile;
    unsigned char pad_06[0x2];
    int unusedParam0;
    int unusedParam1;
    int unusedParam2;
    unsigned char pad_14[0x4];
} FieldTileDrawGroup;

#endif /* FF7_TYPES_FIELD_H */

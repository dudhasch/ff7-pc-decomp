/* include/ff7/types/condor.h: the types ff7/condor TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_CONDOR_H
#define FF7_TYPES_CONDOR_H


struct GfxRenderObject;
/* Condor unit record (0x78 bytes): flags, timers, counters, target and contact slots. */
typedef struct CondorRecord {
    unsigned short status;
    unsigned char state;
    char commandCode;
    unsigned char flags;
    unsigned char retired;
    union {
        unsigned short unitIndex;
        short unitIndexSigned;
        char unitIndexByte;
    };
    short level;
    unsigned short targetIndex;
    short initialValue;
    short costOrArcVelY;
    unsigned char counter;
    unsigned char counterMax;
    unsigned char scalePercent;
    unsigned char kind;
    union {
        char attackRange;
        unsigned char uAttackRange;
    };
    unsigned char pad_15[0x1];
    unsigned char offsetY;
    char unusedConfigByte;
    unsigned short actionCountdown;
    unsigned short timerReload;
    unsigned short cooldown;
    unsigned short timer;
    unsigned char targetMode;
    union {
        char frameIndex;
        unsigned char uFrameIndex;
    };
    unsigned char extentX;
    unsigned char extentY;
    unsigned short candidate;
    unsigned short targetX;
    unsigned short targetY;
    unsigned char sequenceIndex;
    union {
        char operand;
        unsigned char uOperand;
    };
    unsigned char sequenceCommand;
    unsigned char inContact;
    unsigned char bufferIndex;
    char sequenceFlag;
    short sequenceBase;
    short sequenceOffset;
    union {
        unsigned short angle;
        short sAngle;
    };
    union {
        unsigned short drawAngle;
        short sDrawAngle;
    };
    unsigned short turnRate;
    unsigned char subPixelX;
    unsigned char subPixelY;
    unsigned char cycleReload;
    unsigned char cyclePeriod;
    unsigned short cycleCountdown;
    unsigned char targetSlot;
    unsigned char contactSlot;
    unsigned char pad_42[0x1];
    unsigned char attackerSlot;
    int attackTarget;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_4E[0x2];
    short worldX;
    short worldY;
    short worldZ;
    unsigned char pad_56[0x2];
    short velX;
    short velY;
    short moveStep;
    unsigned char pad_5E[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_66[0x2];
    int ringOldest;
    int ringMiddle;
    int ringNewest;
    int pad_74;
} CondorRecord;
typedef char CheckCondorRecord[(sizeof(CondorRecord) == 0x78) ? 1 : -1];

/* One of the eight effect slots of g_CondorEffectSlots (0x10 bytes; Mini_CondorSpawnEffectSlot
 * fills a free one from a condor record, Mini_CondorUpdateEffectSlots steps each live one). */
typedef struct CondorEffectSequence {
    short posX; /* +0x00 */
    short posY; /* +0x02 */
    short posZ; /* +0x04 */
    unsigned char pad_06[0x2];
    unsigned char tableIndex; /* +0x08: 0xFF for a free slot; the sequence step */
    unsigned char timer; /* +0x09 */
    unsigned char flags; /* +0x0A */
    char scalePercent; /* +0x0B */
    union {
        char attackerSlot; /* +0x0C: 7 steps the alternative sequence */
        unsigned char uAttackerSlot;
    };
    unsigned char pad_0D[0x3];
} CondorEffectSequence;
typedef char CheckCondorEffectSequence[(sizeof(CondorEffectSequence) == 0x10) ? 1 : -1];

/* Condor slot motion state: current position, target position and step offsets. */
typedef struct CondorSlotMotion {
    short posX;
    short posY;
    unsigned char pad_04[0x4];
    short destX;
    short destY;
    short sourceX;
    unsigned char pad_0E[0x2];
    short targetX;
    short targetY;
    short targetZ;
    unsigned char pad_16[0x2];
    int panelPtr;
    unsigned char pad_1C[0x4];
    short savedRotY;
    short savedAngle;
    short pendingDeltaX;
    short pendingDeltaY;
    char carryX;
    char carryY;
} CondorSlotMotion;

/* Two-short cursor position used by the Condor mini-game. */
typedef struct CondorCursor {
    short x;
    short y;
} CondorCursor;

/* Quad vertex source record that Mini_CondorWriteQuadVertices writes into the batch node. */
typedef struct CondorQuadVertices {
    union {
        struct GfxRenderObject *batchNode;
        int batch;
    };
    union {
        char colorByte;
        int texturePage;
        int tex;
    };
    union {
        int x;
        float fx;
    };
    union {
        int y;
        float fy;
    };
    float dx;
    float dy;
    union {
        int posZ;
        float fz;
    };
    union {
        int x2;
        float u;
    };
    union {
        int y2;
        float v;
    };
    float dx2;
    float dy2;
    int color;
} CondorQuadVertices;

/* Condor record copied from a unit, used as a collision probe or a linked peer slot. */
typedef struct CondorLinkSlot {
    unsigned char pad_00[0x22];
    unsigned char prevOffsetY;
    unsigned char offsetY;
    unsigned char pad_24[0x24];
    short posX;
    short posY;
    short posZ;
} CondorLinkSlot;

/* Condor record tested for directional blocking against peers; bit 8 of the flags word is set on resolve. */
typedef struct CondorBlockRecord {
    unsigned short flags;
    unsigned char pad_02[0x20];
    unsigned char sizeX;
    unsigned char sizeY;
    unsigned char pad_24[0x1e];
    unsigned char dirMask;
    unsigned char pad_43[0x5];
    short posX;
    short posY;
    unsigned char pad_4C[0x4];
    short offsetX;
    short offsetY;
    unsigned char pad_54[0x4];
    short dirX;
    short dirY;
} CondorBlockRecord;

/* Rectangle (x, y, width, height) that Mini_CondorDrawTiledFadeOverlay clips against the fade tiles. */
typedef struct CondorFadeRect {
    int x;
    int y;
    int width;
    int height;
} CondorFadeRect;

/* Condor record-select panel; its position moves linearly toward a target taken from a source record. */
typedef struct CondorRecordSelectPanel {
    unsigned char pad_00[0x34];
    short angle;
    unsigned char pad_36[0x1a];
    short posX;
    short posY;
    short layoutX;
    unsigned char pad_56[0xc];
    short rotY;
} CondorRecordSelectPanel;

/* Twelve render-object handles for the condor textures, created together and released when nonzero. */
typedef struct CondorTextureRenderObjects {
    unsigned char pad_00[0x4];
    int renderObject04;
    int renderObject08;
    int renderObject0C;
    int renderObject10;
    int renderObject14;
    int renderObject18;
    int renderObject1C;
    int renderObject20;
    int renderObject24;
    int renderObject28;
    int renderObject2C;
    int renderObject30;
} CondorTextureRenderObjects;

/* Condor config entry read per record and copied into a CondorRecord. */
typedef struct CondorConfigEntry {
    char unitIndex;
    union {
        char tileX;
        unsigned char uTileX;
    };
    union {
        char tileY;
        unsigned char uTileY;
    };
    char candidate;
    short initialValue;
} CondorConfigEntry;

/* One record of g_CondorUnitModelTable (0x38 bytes, seven records): a
 * unit's four part render objects, whether it is loaded and which pass draws it. */
typedef struct CondorUnitPart {
    int renderObject; /* +0x00 */
    int batched; /* +0x04: set when a rect is emitted into renderObject; the frame empties the batch and clears it */
    unsigned char pad_08[0x4];
} CondorUnitPart;
typedef struct CondorUnitModel {
    CondorUnitPart parts[4]; /* +0x00 */
    int ready; /* +0x30: the part render objects are created */
    char drawPass; /* +0x34: 0 or 1, the frame's first or second unit pass (units 2 and 3 draw in the second) */
    unsigned char pad_35[0x3];
} CondorUnitModel;
typedef char CheckCondorUnitModel[(sizeof(CondorUnitModel) == 0x38) ? 1 : -1];

/* Sound command record written by the music and SFX submit helpers. */
typedef struct CondorSoundCommand {
    short command;
    unsigned char pad_02[0x2];
    int param;
    int soundId;
} CondorSoundCommand;

/* Condor run record that links to a condor buffer record. */
typedef struct CondorRunRecordLayout {
    short prevX;
    short prevY;
    short prevZ;
    unsigned char pad_06[0x2];
    short curX;
    short curY;
    short sourceX;
    unsigned char pad_0E[0x2];
    short targetX;
    short targetY;
    unsigned char pad_14[0x4];
    int linkedRecord;
    unsigned char pad_1C[0x2];
    short index;
    short pendingOffset;
    short savedAngle;
    short dirX;
    short dirY;
    char carryX;
    char carryY;
} CondorRunRecordLayout;

/* Condor mini-game configuration entry, copied into a CondorRecord when a unit is initialised. */
typedef struct CondorUnitTypeEntry {
    short cost;
    char counter;
    unsigned char pad_03[0x2];
    char scalePercent;
    char kind;
    unsigned char rangeCells;
    unsigned char pad_08[0x2];
    char offsetY;
    char unusedConfigByte;
    unsigned char timerReload;
    char extentX;
    char extentY;
    char cycleReload;
    char cycleCount;
} CondorUnitTypeEntry;

/* Condor input cursor position, clamped to the screen range. */
typedef struct CondorCursorPos {
    int cursorX;
    int cursorY;
} CondorCursorPos;

/* 76-byte (0x4C) Condor terrain region / unit-table record loaded from data\vert.bin. */
typedef struct CondorRegionRecord {
    unsigned char pad_00[0x20];
    short edge0;
    union {
        short edge1;
        short regionIndex;
    };
    short edge2;
    unsigned char pad_26[0x2];
    short corner0X; /* +0x28: the triangle's three corners, each an x/y pair followed by 4 padding bytes */
    short corner0Y;
    unsigned char pad_2c[0x4];
    short corner1X; /* +0x30 */
    short corner1Y;
    unsigned char pad_34[0x4];
    short corner2X; /* +0x38 */
    short corner2Y;
    unsigned char pad_3c[0x4];
    short minX;
    short maxX;
    short minY;
    short maxY;
    union {
        struct {
            short vertexX;
            short vertexY;
        };
        struct {
            short rotX;
            short rotZ;
        };
    };
} CondorRegionRecord;
typedef char CheckCondorRegionRecord[(sizeof(CondorRegionRecord) == 0x4C) ? 1 : -1];

/* Source record of a condor run-record layout entry, with its linear motion target. */
typedef struct CondorRunRecordSource {
    unsigned char pad_00[0x34];
    short angle;
    unsigned char pad_36[0x12];
    short targetX;
    short targetY;
    unsigned char pad_4C[0x4];
    short curX;
    short curY;
    short layoutX;
    unsigned char pad_56[0xc];
    short pendingOffset;
} CondorRunRecordSource;

/* Byte-packed config entry copied into a CondorConfigEntry record. */
typedef struct CondorConfigSourceEntry {
    char unitType;
    char tileX;
    char tileY;
    char candidate;
    short initialValue;
    char targetMode;
} CondorConfigSourceEntry;

/* Active-record list entry built from a CondorRecord; the list ends when posX is -1. */
typedef struct CondorActiveRecord {
    short posX;
    short invertedPosY;
    short unitIndex;
    short angle;
} CondorActiveRecord;

/* hub CondorCounterEffect begin */
/* A counter effect slot of the condor minigame (Mini_CondorFindInactiveCounterEffectSlot). */
#pragma pack(push, 1)
struct CondorCounterEffect {
    short counter; /* +0x0: counts up while nonzero; bit 15 is a flag, the low 15 bits the count */
    short displayNumber; /* +0x2 */
    short posX; /* +0x4 */
    short posY; /* +0x6 */
    int tensDigit; /* +0x8 */
    int onesDigit; /* +0xc */
};
#pragma pack(pop)
/* hub CondorCounterEffect end */

/* hub CondorRecordTable begin */
/* The condor minigame's unit records, 0x78 bytes each. */
#pragma pack(push, 1)
struct CondorRecordTable {
    CondorRecord records[69]; /* +0x0 */
    unsigned char auxiliaryRecords[0x4]; /* +0x2058 */
};
#pragma pack(pop)
/* hub CondorRecordTable end */

/* Per-unit-kind Condor model set (0xC bytes) pointing at a table of polygon set pointers. */
typedef struct CondorUnitModelSet {
    int pad_00;
    int polySetTable;
} CondorUnitModelSet;

/* Source rectangle (x, y, end row) for Mini_CondorCopyRectToSurface and ToBuffer. */
typedef struct CondorRect {
    int x;
    int y;
    unsigned char pad_08[0x4];
    int yEnd;
} CondorRect;

/* Compact entry that Mini_CondorInitRecordFromShortEntry expands into a condor record. */
typedef struct CondorShortEntry {
    short posX;
    short posY;
    char unitIndex;
    unsigned char pad_05[0x1];
    short angle;
} CondorShortEntry;

/* Holder block for one condor RSD model: three constant ints and the model pointer. */
typedef struct CondorRsdModelHolder {
    int initWord0;
    int initWord1;
    int initWord2;
    void *model;
} CondorRsdModelHolder;

/* Byte pair entry read while loading a condor ID header vertex table. */
typedef struct CondorVertexEntry {
    unsigned char pad_00[0x2];
} CondorVertexEntry;

/* Binary camera setup loaded from data\camera.bin into g_pCondorCameraBin (0x26 bytes). */
typedef struct CondorCameraBin {
    short rotation[9];
    unsigned char pad_12[0x2];
    int translation[3];
    unsigned char pad_20[0x4];
    short screenDistance;
} CondorCameraBin;

/* Integer origin point used as a base for the tiled fade overlay box. */
typedef struct CondorOrigin {
    int x;
    int y;
} CondorOrigin;

/* Short sprite offset added to a condor slot position. */
typedef struct CondorSpriteOffset {
    short offsetX;
    short offsetY;
} CondorSpriteOffset;

/* One pixel record of the Condor TIM texture being decoded. */
typedef struct CondorTimPixelRecord {
    short pixelWord;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char pad_05[0x1];
} CondorTimPixelRecord;

/* The display format word and the pixel record table pointer behind it (g_CondorDisplayFormat, g_pCondorTimPixelRecords),
   which the indexed-pixel expanders take as their palette. */
typedef struct CondorTimPalette {
    int displayFormat;
    struct CondorTimPixelRecord *records;
} CondorTimPalette;

/* Sprite entry in the condor screen buffer (0x28 bytes): screen position, size, UV origin and render slot. */
#pragma pack(push, 2)
typedef struct CondorSpriteEntry {
    short unusedBaseField;
    union {
        unsigned short offsetX;
        unsigned short width;
    };
    unsigned short height;
    union {
        short screenX;
        short posX;
        unsigned short uPosX;
    };
    union {
        short screenY;
        short posY;
        unsigned short uPosY;
    };
    unsigned short texU;
    unsigned short texV;
    unsigned char pad_0E[0x2];
    int depth;
    struct CondorSpriteOffset *offsets;
    int renderSlot;
    int isActive;
    int paletteIndex;
    int pageSlot;
} CondorSpriteEntry;
#pragma pack(pop)
typedef char CheckCondorSpriteEntry[(sizeof(CondorSpriteEntry) == 0x28) ? 1 : -1];

/* One record of g_CondorRenderObjectSlots (0xc bytes). */
typedef struct CondorRenderObjectSlotEntry {
    int renderObject; /*+0x00 */
    int drawn; /*+0x04 */
    char flag; /*+0x08 */
    unsigned char pad_09[0x3];
} CondorRenderObjectSlotEntry;

/* 8-byte Condor list entry: a unit record and a short, the unit kind in g_CondorCollectedRecords and the
 * record's slot index in the match list g_CondorMatchListState. */
typedef struct CondorLinkRecord {
    struct CondorRecord *peer;
    union {
        short kind;
        short recordIndex;
    };
    unsigned char pad_06[0x2];
} CondorLinkRecord;

/* 0x140-byte Condor match-list and camera-matrix state block at 0x00c60980 (g_CondorMatchListState):
 * 32 eight-byte CondorLinkRecord entries (+0x000..+0x0FF), 32-byte camera work matrix (+0x100..+0x11F),
 * cursor active flag (+0x120), region count (+0x124), and selected choice slot (+0x138). */
typedef struct CondorMatchListState {
    CondorLinkRecord entries[32];  /* +0x000 */
    int cameraWorkMatrix[8];       /* +0x100 */
    short cursorActive;            /* +0x120 */
    unsigned char pad_122[0x2];
    int regionCount;               /* +0x124 */
    unsigned char pad_128[0x10];
    short choiceActionSlot;        /* +0x138 */
    unsigned char pad_13A[0x6];
} CondorMatchListState;
typedef char CheckCondorMatchListState[(sizeof(CondorMatchListState) == 0x140) ? 1 : -1];

/* Packed BGRA byte view of the four vertex colours in a GfxBatchPrimitive (+0x5C). */
typedef struct CondorBatchColorBytes {
    char blue;
    char green;
    char red;
    char alpha;
} CondorBatchColorBytes;

typedef struct CondorBatchPrimitive {
    unsigned char pad_00[0x5C];
    CondorBatchColorBytes colorBytes[4];
} CondorBatchPrimitive;

/* One record of g_CondorEffectSpriteSlots (0x44 bytes). */
typedef struct CondorEffectSlotEntry {
    unsigned short texX; /*+0x00 */
    unsigned short texY; /*+0x02 */
    unsigned short baseSize; /*+0x04 */
    unsigned short baseHeight; /*+0x06 */
    unsigned short drawFlags; /*+0x08 */
    unsigned char pad_0A[0x2];
    int duration; /*+0x0C */
    int effectId; /*+0x10 */
    unsigned char pad_14[0x4];
    int screenX; /*+0x18 */
    int screenY; /*+0x1C */
    unsigned char pad_20[0x10];
    int unused30; /*+0x30 */
    int state; /*+0x34 */
    int renderSlot; /*+0x38 */
    int active; /*+0x3C */
    unsigned char pad_40[0x4];
} CondorEffectSlotEntry;

/* Per-slot condor render object handle, created with Gfx_CreateRenderObject and destroyed on release. */
typedef struct CondorRenderObject {
    int renderObject;
    unsigned char pad_04[0x4];
} CondorRenderObject;

/* Per-unit model-set slot of the condor game; holds the model set loaded for each unit. */
typedef struct CondorUnitModelSlot {
    int modelSet;
    unsigned char pad_04[0x8];
} CondorUnitModelSlot;

/* Row flags of the Condor rows, cleared on reset. */
typedef struct CondorRowFlags {
    unsigned char red; /* the red fade byte Mini_CondorUpdateRedFadeSprite stores here */
    char green;
    char blue;
} CondorRowFlags;

/* Condor render object slot whose mode is tested as 0 or 1 during scene render. */
typedef struct CondorRenderObjectSlot3 {
    char mode;
    unsigned char pad_01[0xb];
} CondorRenderObjectSlot3;

/* Condor unit model table entry holding the model kind for each unit. */
typedef struct CondorUnitModelEntry {
    int modelKind;
    unsigned char pad_04[0x8];
} CondorUnitModelEntry;


/* Condor buffer header cleared on game-state reset. */
typedef struct CondorResetBufferA {
    short rotX;
    short rotY;
    short rotZ;
} CondorResetBufferA;

/* Condor delta record that stores a scanned item and a list value. */
typedef struct CondorDeltaRecord {
    struct CondorRecord *item;
    short listValue;
    unsigned char pad_06[0x2];
} CondorDeltaRecord;

/* 0x48-byte HUD numeric readout group (8 contiguous groups at 0x00CBEED0..0x00CBF110). */
typedef struct CondorDigitGroup {
    int digitSprites[15];
    int digitCount;
    int pad_40;
    int isVisible;
} CondorDigitGroup;

/* Scheduled condor spawn record where a byte of 0xFF at 0x00 marks it unused. */
typedef struct CondorScheduledRecord {
    unsigned char unitType;
    unsigned char pad_01[0x3];
    short dueTime;
} CondorScheduledRecord;

/* Packed two-byte decimal digit pair used when building HUD number displays. */
typedef union CondorDigitPair {
    int word;
    struct {
        unsigned char tens;
        unsigned char ones;
    };
} CondorDigitPair;

/* Condor record shown in the record display: its cost word, counter, scale percent and range cells. */
typedef struct CondorRecordDisplay {
    unsigned short cost;
    unsigned char counter;
    unsigned char pad_03[0x2];
    unsigned char scalePercent;
    unsigned char pad_06[0x1];
    unsigned char rangeCells;
} CondorRecordDisplay;

/* Condor id header; bytes at +0x00 and +0x01 form a little-endian 16-bit value. */
typedef struct CondorIdHeader {
    unsigned char lowByte;
    unsigned char highByte;
} CondorIdHeader;

/* Condor type record; bytes at +0x0D and +0x0E are copied into a placement probe. */
typedef struct CondorTypeRecord {
    unsigned char pad_00[0xd];
    char extentX;
    char extentY;
} CondorTypeRecord;

#pragma pack(push, 1)
typedef struct CondorCurrentValues {
    short lineStartX; /* +0x0 */
    short currentValue; /* +0x2 */
} CondorCurrentValues;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct CondorDefaultValues {
    short lineEndX; /* +0x0 */
    short defaultValue; /* +0x2 */
} CondorDefaultValues;
#pragma pack(pop)

/* The 4 bytes at +4 of a CondorSpriteEntry that the HUD code stores as a global of their own: the
 * sprite's height, then its screenX. */
#pragma pack(push, 1)
typedef struct CondorSpriteSizeX {
    short height;
    short screenX;
} CondorSpriteSizeX;
#pragma pack(pop)

/* The first 4 bytes of a CondorSpriteEntry that the HUD code stores as a global of their own: the
 * word before offsetX. */
#pragma pack(push, 1)
typedef struct CondorSpriteHeadOffsetX {
    short unused0;
    short offsetX;
} CondorSpriteHeadOffsetX;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct CondorHeightPosPair {
    short height; /* +0x0 */
    short posX; /* +0x2 */
} CondorHeightPosPair;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct CondorSelectAngleState {
    short initWord; /* +0x0 */
    short selectAngle; /* +0x2 */
} CondorSelectAngleState;
#pragma pack(pop)

/* Header view over g_CondorUnitModelDefs exposing its file-table pointer at +0x4. */
typedef struct CondorUnitModelSlotView {
    unsigned char pad_00[0x4];
    int fileTable;
} CondorUnitModelSlotView;

#endif /* FF7_TYPES_CONDOR_H */

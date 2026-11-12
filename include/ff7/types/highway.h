/* include/ff7/types/highway.h: the types ff7/highway TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_HIGHWAY_H
#define FF7_TYPES_HIGHWAY_H

#include "lib/types/graphics.h"

#ifdef __cplusplus
struct HighwayFileLoader {
    int receiver_minimum[5];
    int recordMatrix; /* +0x14: the record matrix (g_pHighwayFileBlock->recordMatrix) */
    HighwayFileLoader();
    ~HighwayFileLoader();
    int load(int byteCount, char *name);
};

// Native C++ member-call contribution.
// C linkage preserves the existing dispatcher entrypoint.
struct HighwayEffect {
    unsigned char pad_00[0x4];
    int owner;
    unsigned char pad_08[0x58];
    int coordsBase;
    int count;
    int coordsStart;
    int limit;
    int accum;
    unsigned char pad_74[0x10];
    int dataPtr;
    void loadCoords(char index);
    void init(char index, int owner, int data);
    void activate(int slot, char index);
};

struct HighwayPart {
    HighwayPart *visible(int value);
};

/* One 0x78-byte actor in the highway's HighwayIndex pool: a PSX matrix, the float matrix built from it
 * for drawing, the parent actor, the model and the links of its chain. */
struct HighwayModelNode {
    PsxMatrix psx;             /* +0x0: rotation and (unaligned) position */
    unsigned char pad_1E[0x2];
    float matrix4x4[16];       /* +0x20: render matrix; translation at words 12..14 */
    HighwayModelNode *parent;  /* +0x60: the pool root for a top-level actor */
    short modelIndex;          /* +0x64: roster index (select) */
    unsigned char pad_66[0x2];
    struct PolygonRsdModelHandle *model; /* +0x68: the roster's model for modelIndex; 0 draws nothing */
    short slot;                /* +0x6C: the pool slot (set/select) */
    short depth;               /* +0x6E: parent's depth + 1; picks the chain attach links */
    HighwayModelNode *prev;    /* +0x70: chain links */
    HighwayModelNode *next;    /* +0x74 */
    void set(short value);
    void drawLayer(int unused1, int unused2, char flag);
    void rotate(const int *position, short *rotation);
    void clone(const HighwayModelNode *other);
    void select(short index, short value);
    void select(short index);
    void render(int flag);
    HighwayModelNode *adjustedMatrix(void *output);
};

struct HighwayTrackFrame;
struct HighwayRenderGroup;
struct HighwayPalette;
struct HighwayPaletteFieldBlock;

struct HighwaySlotList {
    unsigned char pad_00[0xc];
    HighwayTrackFrame *slots;
    unsigned char pad_10[0x8];
    int activeIndex;
    void clearActive();
};

/* The 0x18-byte layer block (g_pHighwayModeBlock, its constructor clears it): four
 * model layers createLayers makes and drawLayers draws when their flag is set. */
struct HighwayModeController {
    unsigned char layerArg; /* +0x0: drawLayers passes it to each drawLayer call */
    unsigned char visible[4]; /* +0x1 */
    unsigned char pad_05[0x3];
    struct HighwayModelNode *layers[4]; /* +0x8 */
    HighwayModeController();
    char createLayers();
    void drawLayers();
};

/* A 20-byte HUD slot of HighwayRenderOwner (initField fills five, process draws them through
 * HighwayScaledSpriteGroup::drawScaled): the icon and bar positions, the icon's texture
 * coordinates for each of its four states, and the sizes. */
typedef struct HighwayHudSlot {
    short posX;
    short posY;
    short drawX;
    short drawY;
    unsigned char texU[4];
    unsigned char texV[4];
    unsigned char width;
    unsigned char height;
    unsigned char size;
    unsigned char offsetX;
} HighwayHudSlot;

/* The highway's 0x78-byte HUD object (g_pHighwayRenderBlock): the banner, the sprite group, five
 * 20-byte effect slot records, and the fade quad's render object and alpha. */
struct HighwayRenderOwner {
    int banner; /* +0x0: drawHud draws sprite 0, 1 or 2 when it is 1, 3 or 2 (0: none) */
    struct HighwayScaledSpriteGroup *spriteGroup; /* +0x4 */
    HighwayHudSlot slots[5]; /* +0x8: initField fills them, process draws them */
    struct GfxRenderObject *renderObject; /* +0x6C: the fade quad's render object (create) */
    unsigned char fadeAlpha; /* +0x70 */
    unsigned char pad_71[0x3];
    int fadeActive; /* +0x74: drawFade raises fadeAlpha by 5 per frame while it is set */
    HighwayRenderOwner();
    ~HighwayRenderOwner();
    void initField();
    void *create();
    void tick();
    void process(short index, char value, char state);
    void drawNumber(int amount, int x, int y, short showZero, float depth);
    void drawDigits(int amount, int x, int y, short showZero, float depth);
    void drawHud();
    void dispatch();
    void drawFade();
};

struct HighwayMat64 {
    int words[16];
};

struct HighwayWords4 {
    int words[4];
};

struct HighwayIndex {
    HighwayModelNode root;          /* +0x0: depth 0, the parent of every top-level actor */
    HighwayModelNode heads[10];     /* +0x78: chain heads, one per depth (the ctor sets prev = 0) */
    HighwayModelNode tails[10];     /* +0x528: chain tails (the ctor sets next = 0) */
    HighwayModelNode actors[400];   /* +0x9D8 */
    short freeHead;                 /* +0xC558: first free actor slot */
    short nextFree[400];            /* +0xC55A: successor of each free slot */
    unsigned char pad_C87A[0x2];
    HighwayIndex();
    void set(short value);
    void attach(void *node, int slot);
    HighwayModelNode *create(short index, const HighwayModelNode *other, const int *position, short *rotation);
    HighwayModelNode *createRaw(short index, const HighwayModelNode *other, int x, int y, int z, short rx,
                        short ry, short rz);
};

struct HighwayArray {
    int *elements;
    int get(int index);
};

struct HighwayRoster {
    int *actors;
    char *flags;
    HighwayRoster();
    ~HighwayRoster();
    void loadModels();
};

struct HighwayScene {
    struct GfxEngineContext *context;
    int mode;
    int focusX;
    int focusY;
    int focusZ;
    unsigned char pad_14[0x4];
    int eyeX;
    int eyeY;
    int eyeZ;
    unsigned char pad_24[0x4];
    int targetX;
    int targetY;
    int targetZ;
    unsigned char pad_34[0x4];
    int targetOffsetX;
    int targetOffsetY;
    int targetOffsetZ;
    unsigned char pad_44[0x4];
    int eyeOffsetX;
    int eyeOffsetY;
    int eyeOffsetZ;
    unsigned char pad_54[0x4];
    int pathCountTable;
    int pathOffsetTable;
    unsigned char pad_60[0x8];
    int coordsStart;
    unsigned int limit;
    int accum;
    int lastEyeOffsetX;
    int lastEyeOffsetY;
    int lastEyeOffsetZ;
    unsigned char pad_80[0x4];
    int step;
    int lift;
    int nearestDistance;
    unsigned char pad_90[0x4];
    int nearestRivalDistance;
    int nearestRivalSlot;
    int renderOffset;
    short cameraLateralOffset;
    short cameraHeightOffset;
    short initZero_A4;
    unsigned char pad_A6[0x2];
    float clipLeft;
    float clipTop;
    float viewWidth;
    float viewHeight;
    int farDepthBits;
    int scaleBits;
    HighwayScene();
    void initialize();
    void update();
    void draw(int context);
    void position(int x, int y, int z);
    void positionMatrix(int x, int y, int z);
    void rotateAxis(int axis, int angle);
};

/* Planar sample position the field context caches per palette channel (16-byte stride). */
struct HighwayPlanarAnchor {
    int x;
    int y;
    int z;
    unsigned char pad_0C[0x4];
};

struct HighwayFieldManager {
    unsigned char pad_00[0x8];
    HighwayPalette *palette;
    int frames; /* the 80 HighwayTrackFrame ring; other TUs index it as an int */
    unsigned char pad_10[0x8];
    int frameIndex;
    unsigned char pad_1C[0x60];
    HighwayPlanarAnchor anchors[10];
    void samplePosition(int path, int unused, int *position);
    void load(int path, int unused, int *position, int *result);
    void loadPlanar(int path, int unused, int *position, int *result);
    void advanceActors();
};

struct HighwayTrackFrame {
    int posX;
    int posY;
    int posZ;
    unsigned char pad_0C[0x4];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_16[0x2];
    PsxMatrix matrix;
    unsigned char pad_36[0x6];
    int speed;
    int mode;
    short roadWidth;
    short info[10]; /* indexed by HighwayFrameInfo */
    unsigned char pad_5A[0x2];
    HighwayModelNode *actors[20];
    unsigned char actorCount;
    unsigned char pad_AD[0x3];
    HighwayTrackFrame();
    HighwayMat64 *toWorldMatrix(HighwayMat64 *result);
    void buildQuad(float *result);
};

/* One 12-byte motion command of the track script that HighwayFieldContext::update plays. */
struct HighwayMotionCommand {
    unsigned char mode;
    unsigned char speed;
    short stepCount;
    short targetA;
    short targetB;
    short roadWidth;
    short tableIndex;
};

struct HighwayFieldContext {
    HighwayRenderGroup *roadGroup;
    HighwayRenderGroup *wallGroup;
    HighwayPaletteFieldBlock *fieldBlock;
    HighwayTrackFrame *frames;
    HighwayMotionCommand *dataBase;
    HighwayMotionCommand *cursor;
    int frameIndex;
    int skipLatch; /* set once the animation entry 4 is seen; later frames are marked skipped */
    int offsetX;
    int offsetY;
    int offsetZ;
    unsigned char pad_2C[0x4];
    short stepCount;
    short counter;
    short roadWidth;
    unsigned char mode;
    unsigned char commandPending;
    int targetA;
    int prevTargetA;
    int stepA;
    int posA;
    int targetB;
    int prevTargetB;
    int stepB;
    int posB;
    int angleStep;
    int angle;
    int angleTotal;
    int speed;
    short tableIndex;
    unsigned char pad_6A[0x2];
    unsigned char *table;
    unsigned char *entry;
    unsigned char *entryBase;
    unsigned char entryCount;
    char entryCountCopy;
    unsigned char cycleIndex;
    unsigned char cycleIndexB;
    HighwayPlanarAnchor anchors[10];
    HighwayFieldContext();
    ~HighwayFieldContext();
    void reset();
    void update();
    void draw();
};
#endif

struct HighwayPlayerState;
struct PolygonSkelHierarchy;
#ifdef __cplusplus
struct HighwaySkelItem {
    struct PolygonSkelHierarchy *skeleton;
    unsigned char pad_04[0x8];
    int animationCount;
    int *animations;
    struct HighwayPlayerState *player;
    int compileAndLoadAnimation(int context, const char *path);
    void samplePosition(int packed, int table, int *output, char mirror);
    void release();
    int makePlayer(int model, int options, int animation);
    int loadHrc(int path, int metadata, int context, int parts);
    int loadAnimations(int count, int array, int context);
    int setup(const int *record, int context, int parts);
};

struct HighwaySkelList {
    int count;
    int reserved;
    char *items;
    void release();
    int load(int context, int parts);
    void tick(int context);
};

struct HighwayFieldItem {
    int skeleton;
    int index;
    int currentFrame;
    int count;
    int animations;
    struct HighwayPlayerState *player;
    void setAnimation(int value, int delta, int bake);
    void draw(int value);
};

struct HighwayFieldList {
    int count;
    int reserved;
    char *items;
    void draw(int value);
};

struct HighwayFloat3 {
    float x;
    float y;
    float z;
};

struct HighwayFloat4 {
    float x;
    float y;
    float z;
    float w;
};

struct HighwayRenderGroup {
    int count;
    int reserved;
    int *objects;
    int *first;
    int *masks;
    int *second;
    int extra[2];
    HighwayRenderGroup(int *sources, int reservedValue);
    void reset();
    ~HighwayRenderGroup();
    void drawSegment(int entry, int frame, HighwayFloat3 *a, HighwayFloat3 *b, HighwayFloat3 *c,
                     HighwayFloat3 *d, int mode, char flag);
};

typedef struct HighwayVec2 {
    float x;
    float y;
} HighwayVec2;

// The original object beginning at $CRT_CPP_Initializer (0x00652d40) starts with one file-scope
// object whose inline constructor generates no code: .CRT$XCU slot 0x007ba034 -> 0x00652d40 calls
// the empty 5-byte constructor body $E1 (0x00652d4a), and no at-exit registration follows. No
// instruction names the object, so its original identity, type, size and address are unknown; this
// minimal internal-linkage empty object reproduces the code only. The compiler-generated $E
// functions carry no FUNCTION markers. Retail starts
// this TU's 8-aligned .bss with g_HighwayActiveSlotCount at 0xD8B170, right after
// highway_effect_records_pool.cpp's 22,000-byte record block, so the object takes no .bss bytes
// before it (as a constructed object VC5 would emit it first). Declared in a bss_seg of its
// own, VC5 /Od allocates no storage for the unreferenced static at all; the $E function and the
// constructor keep their bytes (no instruction names the object).
/* Class name preserved: VC5 embeds `HighwayEmptyStaticInit` in decorated C++ constructor symbols. */
struct HighwayEmptyStaticInit {
    HighwayEmptyStaticInit()
    {
    }
};
#endif

/* Highway actor group entry in g_HighwaySlotActors, with constants set by Mini_HighwayInitActorGroups. */
typedef struct HighwayActorGroup {
    int state;
    int phase;
    int animItemA;
    int animItemB;
    int linkIdA;
    int linkIdB;
    int posX;
    int pad_1C;
    int posY;
    int pad_24;
    unsigned char pad_28[0x4];
    int posZ;
    unsigned char pad_30[0x8];
    int minPosition;
    unsigned char pad_3C[0x4];
    int rootActor;
    int childActorA;
    int childActorB;
    int childActorC;
    unsigned char pad_50[0x10];
    int segmentStartZ;
    int segmentEndZ;
    int slot0State;
    int endRadius;
    unsigned char pad_70[0x8];
    int timer;
    int lateralStep;
    int lateralReturnStep;
    unsigned char pad_84[0x4];
    int scrollPos;
    int maxHp;
    unsigned char pad_90[0x10];
    int isActive;
    unsigned char pad_A4[0xcc];
    int slot0AnimCountdown;
    unsigned char pad_174[0xbc];
    int slot1ScrollPos;
    unsigned char pad_234[0xd8];
    int slot1AnimCountdown;
    unsigned char pad_310[0x1bc];
    int slot2IsFlyingAway;
    unsigned char pad_4D0[0x1a4];
    int slot3IsFlyingAway;
    unsigned char pad_678[0x1a4];
    int slot4IsFlyingAway;
    unsigned char pad_820[0x34];
    int part;
    unsigned char pad_858[0x58];
    int slot5State;
    unsigned char pad_8B4[0x128];
    int frameCount;
} HighwayActorGroup;

/* Highway effect record: kind, timer, render offset, jitter, variant and animation id. */
typedef struct HighwayEffectRecord {
    int kind;
    unsigned char pad_04[0x8];
    int initialized;
    int timer;
    unsigned char pad_14[0x14];
    int rowOffset;
    int jitter;
    int variant;
    int animId;
    unsigned char pad_38[0x18];
    int groupIndex;
} HighwayEffectRecord;

/* Highway effect pool entry of 0xDC bytes: a phase counter, a record link and an active flag. */
typedef struct HighwayEffectPoolEntry {
    int posX;
    int posY;
    int posZ;
    unsigned char pad_0C[0xc];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_1E[0xa];
    union {
        HighwayEffectRecord effect; /* +0x28: the kind and per-kind state of the effect */
        struct {
            unsigned char pad_28[0x4];
            short typeId; /* +0x2c: the model the effect creates */
        };
    };
    unsigned char pad_7C[0x58];
    int actor;
    short slotIndex;
    short active;
} HighwayEffectPoolEntry;

/* The root actor's placement: its rotation matrix and the translation Mini_HighwayUpdateSlotTransform
 * writes after it (at +0x12, unaligned). */
#pragma pack(push, 1)
typedef struct HighwayRootPlacement {
    short matrix[9];
    int posX;
    int posY;
    int posZ;
} HighwayRootPlacement;
#pragma pack(pop)

/* Highway actor record (stride 0x1A8); its short offset vector shifts two endpoints. */
typedef struct HighwayRootActorShortPos {
    unsigned char pad_00[0x12];
    short offsetX;
    unsigned char pad_14[0x2];
    short offsetY;
    unsigned char pad_18[0x2];
    short offsetZ;
} HighwayRootActorShortPos;

/* Float vector that Mini_HighwayInterpolateVector interpolates toward a target vector. */
typedef struct HighwayLerpVector {
    float x;
    float y;
    unsigned char pad_08[0x4];
    float w;
} HighwayLerpVector;

/* Per-player record in the HighwaySkelList items array (stride 0x18), ticked by HighwaySkelList::tick. */
typedef struct HighwayPlayerState {
    int isEnabled; /* +0x0 */
    int resetPosition;
    int positionLimit;
    int speed;
    int isLooping;
    int delay;
    int holdLastFrame;
    int holdTimerStart;
    int position;
    int isActive;
    int catchUpEnabled;
    unsigned char pad_2C[0x4];
    float rootScale; /* +0x30: scale of the animation's root translation */
    int animation; /* +0x34: the playing PolygonAnimation */
    unsigned char pad_38[0x7c];
    int skel;
    unsigned char pad_B8[0x4];
    struct Timestamp64 interval; /* +0xBC: added to nextTime on each step */
    struct Timestamp64 nextTime; /* +0xC4: when the next step is due */
    unsigned char pad_CC[0x4];
    int endCallback;
} HighwayPlayerState;

/* Per-slot state, stride 0x1A8, starting at +0x68 of each HighwayRenderSlotActor record. The slot
 * kinds (mode 0 the player, 1 and 0xA the helper and the boss, 2 the enemies) use some words under
 * different names; those are anonymous unions. */
typedef struct HighwaySlotState {
    int state;
    int mode;
    int firstId;
    int secondId;
    int partCount;
    int actorCount;
    int posX;
    unsigned char pad_1C[0x4];
    int posZ;
    unsigned char pad_24[0x8];
    int maxPosZ;
    unsigned char pad_30[0x8];
    int minPosZ;
    int prevPosX;
    int prevPosZ;
    int prevSampleX;
    int clearedWord;
    unsigned char pad_4C[0x14];
    int segmentStartZ; /* +0x60: the z offset of the collision segment's start point */
    int segmentEndZ; /* +0x64: the z offset of its end point */
    int startRadius; /* +0x68: the collision radius at the start point (Mini_HighwayActorsCollidePairs) */
    int endRadius; /* +0x6C: at the end point */
    unsigned char pad_70[0x8];
    int randomChance;
    int lateralStep;
    union {
        int lateralReturnStep;
        int randomThreshold;
    };
    int randomRange;
    int hp;
    int maxHp;
    int aiMode;
    unsigned char pad_94[0xc];
    int isActive;
    int velocityX;
    unsigned char pad_A8[0x4];
    int velocityZ;
    int useConfigA;
    int configIndexA;
    int animFrameA;
    int useConfigB;
    int configIndexB;
    int animFrameB;
    union {
        int extraRadius; /* +0xC8: added to either endpoint's radius */
        int absAngle;
    };
    int subState;
    int laneId;
    int trackLimit;
    int trackPos;
    unsigned char pad_DC[0x14];
    int animCountA;
    union {
        int animDirty;
        int countdown;
    };
    union {
        int animIndex;
        int moveAngle;
    };
    union {
        int animCountdown;
        int targetAngle;
    };
    int stunTimer;
    int timer;
    int counter;
    union {
        int animCountB;
        int angle;
    };
    union {
        int lateralOffset;
        int zOffset;
    };
    int isFlyingAway;
    int variant;
    int wanderTimer;
    unsigned char pad_120[0xc];
    int laneCode;
    unsigned char pad_130[0x78];
} HighwaySlotState;

/* Highway effect slot record (0x1A8 bytes per slot) that HighwayEffect::activate initialises. */
typedef struct HighwayEffectSlot2 {
    unsigned char pad_00[0xcc];
    int active;
    int coordsBase;
    int trackLimit;
    int trackPos;
} HighwayEffectSlot2;

/* Highway scene position record; HighwayScene::position writes its x, y and z. */
typedef struct HighwayScenePosition {
    unsigned char pad_00[0x30];
    int x;
    int y;
    int z;
} HighwayScenePosition;

/* One entry of an animation table: the number of frames the cursor wraps at. */
typedef struct HighwayCursorEntry {
    unsigned char pad_00[0x4];
    int count;
} HighwayCursorEntry;

struct PolygonModelVertexColors;
/* One 0x18-byte item of the highway animation list (g_pHighwayActorAnimItems): the model's vertex
 * colours, an animation cursor into a table of entries, and the placement. */
typedef struct HighwayAnimItem {
    struct PolygonModelVertexColors *colors;
    int tableIndex;
    int cursor;
    unsigned char pad_0C[0x4];
    HighwayCursorEntry **entryTable;
    int player;
} HighwayAnimItem;

/* The head of a highway slot record, stride 0x1A8 in g_HighwaySlotActors: the two animation items
 * with their indices, the four highlight flags Mini_HighwayRenderSlotActor sets and clears per frame,
 * the transform and the model nodes from the root actor on. The slot's HighwaySlotState starts at
 * +0x68 of this record. */
typedef struct HighwayRenderSlotActor {
    int animIndexA;
    int animIndexB;
    HighwayAnimItem *animItemA;
    HighwayAnimItem *animItemB;
    int unusedSlotA;
    int unusedSlotB;
    int isVisible;
    int hideQueued;
    int isFrozen;
    int freezeQueued;
    int posX;
    int posY;
    int posZ;
    unsigned char pad_34[0x4];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_3E[0x2];
    int actors[6]; /* the root actor, then its child nodes */
    unsigned char pad_58[0x28];
    int statePosX; /* +0x80: the posX of the HighwaySlotState that starts at +0x68 */
    unsigned char pad_84[0x4];
    int statePosZ; /* +0x88: its posZ */
    unsigned char pad_8C[0x11c];
} HighwayRenderSlotActor;

/* Highway scene block: scene kind, origin offsets, sprite offsets and view size. */
typedef struct HighwaySceneBlock2 {
    unsigned char pad_00[0x4];
    int sceneKind;
    int offsetX;
    int offsetY;
    int offsetZ;
    unsigned char pad_14[0x94];
    int spriteOffsetX;
    int spriteOffsetY;
    float viewWidth;
    float viewHeight;
    unsigned char pad_B8[0x4];
    float scale;
} HighwaySceneBlock2;

/* Highway file block (g_pHighwayFileBlock) holding matrices shared by the highway scene and actors. */
typedef struct HighwayFileBlock3 {
    void *baseMatrix;
    void *translationMatrix;
    void *rotationMatrix;
    void *rollMatrix;
    void *viewMatrixNoRoll;
    int recordMatrix;
} HighwayFileBlock3;

/* Sampled x/y/z triple from a highway skeleton item position table. */
typedef struct HighwaySkelSample {
    short x;
    short y;
    short z;
    unsigned char pad_06[0x2];
} HighwaySkelSample;



/* Highway effect slot with a total and a remaining count. */
typedef struct HighwayHudStatusEntry {
    unsigned short total;
    unsigned short remaining;
    short damageTaken;
    short clearedState;
    unsigned char flashTimer;
    unsigned char iconState;
} HighwayHudStatusEntry;

/* Position vector stored in the highway file block header. */
typedef struct HighwayFileBlockPosition {
    unsigned char pad_00[0x30];
    float posX;
    float posY;
    float posZ;
} HighwayFileBlockPosition;

/* Highway quad vertex, 0xC bytes of floats built from a 3-component source. */
typedef struct HighwayQuadVertex {
    float x;
    float y;
    float z;
} HighwayQuadVertex;

/* Sprite scale pair per slot, two floats written by the HighwayScaledSpriteGroup constructor. */
typedef struct HighwaySpriteScale {
    float scaleX;
    float scaleY;
} HighwaySpriteScale;

/* Highway track event entry: progress threshold and event type. */
typedef struct HighwayTrackEvent {
    int triggerProgress;
    int eventType;
} HighwayTrackEvent;

/* One 0xB0-byte per-frame camera record in the highway field context frame array. */
typedef struct HighwayFrameRecord {
    int offsetX; /* +0x0 */
    int offsetY;
    int offsetZ;
    unsigned char pad_0C[0x4];
    short timer;
    short angle;
    short roll;
    unsigned char pad_16[0x26];
    int posX;
    int mode;
    short roadWidth;
} HighwayFrameRecord;


/* Highway render segment: a vertex (x, y, z, w) and two indexed points of a drawn segment. */
typedef struct HighwayRenderSegment {
    float x;
    float y;
    float z;
    float w;
    int color;
    unsigned char pad_14[0x4];
    float pointX;
    float pointY;
} HighwayRenderSegment;


/* Four-float transform row whose dot product with a point gives the depth. */
typedef struct HighwayDepthTransform {
    unsigned char pad_00[0x8];
    float depthWeightX;
    unsigned char pad_0C[0xc];
    float depthWeightY;
    unsigned char pad_1C[0xc];
    float depthWeightZ;
    unsigned char pad_2C[0xc];
    float depthOffset;
} HighwayDepthTransform;

/* Output vector of a highway interpolation between two lerp vectors. */
typedef struct HighwayInterpolatedVector {
    float x;
    float y;
    int z;
    float w;
} HighwayInterpolatedVector;

/* Highway effect slot initialised from the table, with a render offset and two short values. */
typedef struct HighwayEffectSlotInitA {
    unsigned char pad_00[0x4];
    int sceneKind;
    unsigned char pad_08[0x94];
    int renderOffset;
    short cameraLateralOffset;
    short cameraHeightOffset;
} HighwayEffectSlotInitA;


/* Short x/y/z end point of a highway actor, moved by the frame's per-axis offsets. */
typedef struct HighwayEndPoint {
    short posX;
    short posY;
    short posZ;
} HighwayEndPoint;


/* Float x/y/z point whose depth is a dot product with a transform's rows. */
typedef struct HighwayDepthPoint {
    float posX;
    float posY;
    float posZ;
} HighwayDepthPoint;

/* Float target vertex that Mini_HighwayInterpolateVector lerps from a HighwayLerpVector. */
typedef struct HighwayLerpTarget {
    float posX;
    float posY;
    unsigned char pad_08[0x4];
    float posW;
} HighwayLerpTarget;


/* Destination colour bytes written by the highway colour scaling routine. */
typedef struct HighwayColorTriplet {
    char channel0; /* +0x0 */
    char channel1;
    char channel2;
} HighwayColorTriplet;

/* Source colour bytes scaled by the highway colour scaling routine. */
typedef struct HighwayColorSourceTriplet {
    unsigned char channel0; /* +0x0 */
    unsigned char channel1;
    unsigned char channel2;
} HighwayColorSourceTriplet;

/* Highway sound command buffer: command id, parameter and category sent to SoundDispatchCommand. */
typedef struct HighwaySoundCommand {
    short command;
    unsigned char pad_02[0x2];
    int param;
    int category;
} HighwaySoundCommand;

/* The highway's HP bars of five characters: five 10-byte entries (HighwayHudStatusEntry: the full
   value, the fraction left, then the damage taken, the bar's flash timer and its icon state).
   Mini_HighwayResetRun sets each full value to 0xFFFF and the fraction to it, damage
   (Mini_HighwayDamageCharacter) lowers the fraction, and Mini_Mode6ExitStoreScores saves the
   five fractions as Savemap.highwayHpFractions. */
typedef struct HighwayEffectStatusTable {
    HighwayHudStatusEntry bars[5];
} HighwayEffectStatusTable;

/* Highway actor slot whose fields the event timeline sets for each event. */
typedef struct HighwayEventSlot {
    int stage;
    unsigned char pad_04[0x38];
    int velocityX;
    int posY;
    unsigned char pad_44[0xc4];
    int counter;
} HighwayEventSlot;

/* g_HighwayEffectRecordsPool (src/ff7/highway/highway_effect_records_pool.cpp): the 100 effect records, 0x55f0 bytes. */
typedef struct HighwayEffectRecordsPool {
    HighwayEffectPoolEntry entries[100];
} HighwayEffectRecordsPool;

/* src/ff7/highway/highway_effect_records_pool.cpp Mini_HighwayUpdateEffectRecords: what r points to, from its reads at 3 offsets. */
#pragma pack(push, 1)
typedef struct HighwayEffectTickScratch {
    unsigned char pad_0[0x12];
    int posX; /* +0x12 */
    int jitterPos; /* +0x16 */
    int posZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} HighwayEffectTickScratch;
#pragma pack(pop)

/* src/ff7/highway/highway_slot_actors_update_render.cpp Mini_HighwayActorDistance: what rootA points to, from its reads at 3 offsets. */
#pragma pack(push, 1)
typedef struct HighwayModelBatchContext {
    unsigned char pad_0[0x12];
    int posX; /* +0x12 */
    int posY; /* +0x16 */
    int posZ; /* +0x1a */
    unsigned char pad_1e[0x2];
} HighwayModelBatchContext;
#pragma pack(pop)

#ifdef __cplusplus
struct GfxRenderObject;
struct HighwayScaledSpriteGroup {
    int count;
    GfxRenderObject **objects;
    HighwaySpriteScale *scales;
    float globalScale;
    float xOffset;
    float yOffset;
    HighwayScaledSpriteGroup(int *indices);
    ~HighwayScaledSpriteGroup();
    void drawQuad(int index, float x, float y, float width, float height, float u, float v,
                  float du, float dv, int color, float depth);
    void drawCentered(int index, float depth, float x, float y);
    __inline void drawScaled(int index, int x, int y, int w, int h, int u, int v,
                             int uw, int vh, unsigned int red, unsigned int green,
                             unsigned int blue, float depth)
    {
        drawQuad(index, globalScale * (float)x, globalScale * (float)y,
                 globalScale * (float)w, globalScale * (float)h,
                 (float)u, (float)v, (float)uw, (float)vh,
                 (red << 16) | 0xFF000000 | (green << 8) | blue, depth);
    }
};
#endif

#endif /* FF7_TYPES_HIGHWAY_H */

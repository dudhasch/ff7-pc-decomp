/* include/lib/types/polygon.h: the types lib/polygon TUs share. */
#ifndef LIB_TYPES_POLYGON_H
#define LIB_TYPES_POLYGON_H

/* A TIM file's 8-byte header: the id (0x10 in the low byte, version 0 in the next) and the
 * flags (bits 0-2 the pixel mode: 0 4-bit, 1 8-bit, 2 16-bit, 3 24-bit; bit 3 a CLUT block
 * follows). Pfile_LoadTimFromContext reads it from the file, Pfile_WriteTimRecordToFile writes it
 * back. */
typedef struct PolygonTimHeader {
    int id; /* +0x00 */
    int flags; /* +0x04 */
} PolygonTimHeader;

/* A TIM block's frame-buffer rectangle, in 16-bit units (a 4-bit image's width is a quarter of its
 * pixel width, an 8-bit one's half: Pfile_TimHalfwordWidthToPixelWidth). */
typedef struct PolygonTimVramRect {
    short x; /* +0x00 */
    short y; /* +0x02 */
    short width; /* +0x04 */
    short height; /* +0x06 */
} PolygonTimVramRect;

/* A TIM's CLUT or image block (0x10 bytes, Pfile_LoadTimFromContext): the block's byte size as
 * the file gives it (header included: the data is size - 12 bytes), its rectangle, and the heap
 * copy of its data (the CLUT's 16-bit colours or the image's 16-bit words). */
typedef struct PolygonTimBlock {
    int size; /* +0x00 */
    PolygonTimVramRect rect; /* +0x04 */
    int data; /* +0x0C */
} PolygonTimBlock;
typedef char CheckPolygonTimBlock[(sizeof(PolygonTimBlock) == 0x10) ? 1 : -1];

/* A loaded TIM (0x44 bytes, Pfile_LoadTimFromContext; Pfile_DestroyTim frees it and its blocks).
 * The loader fills +0..+0x18 from the file and Pfile_ComputeTimTextureSize +0x1C..+0x2C; the
 * callers set +0x30..+0x42 (from their load options' +0x50 and +0x54) before
 * Pfile_BuildTextureFromTimRect converts it to a .TEX header (GfxTexHeader). */
typedef struct PolygonTim {
    PolygonTimHeader *header; /* +0x00 */
    int clutCount; /* +0x04: palettes in the CLUT block (its bytes / 32 or / 512) */
    PolygonTimBlock *clut; /* +0x08: present when header->flags has bit 3 */
    PolygonTimBlock *image; /* +0x0C */
    int bitsPerPixel; /* +0x10: 4, 8, 16 or 24 */
    int width; /* +0x14: in pixels */
    int height; /* +0x18 */
    int paddedWidth; /* +0x1C: width rounded up to a power of two */
    int paddedHeight; /* +0x20 */
    int texX; /* +0x24: set from the image position, then cleared */
    int texY; /* +0x28 */
    int texPage; /* +0x2C: the PSX texture page of the image; the converter copies it to the header's +0x10 */
    int flags; /* +0x30: bit 0 makes colours at or below maxRed/Green/Blue transparent, bit 1 repeats
                  the last column into the padding, bit 2 the last row, bit 0x10000 prints the palette */
    int transparentToIndex0; /* +0x34: move a palette's transparent black to index 0 and remap the pixels */
    int singlePalette; /* +0x38: convert only palette paletteRow */
    int paletteRow; /* +0x3C */
    union {
        struct {
            unsigned char maxBlue; /* +0x40 */
            unsigned char maxGreen; /* +0x41 */
            unsigned char maxRed; /* +0x42 */
            unsigned char pad_43;
        };
        int maxColorBits;
    };
} PolygonTim;
typedef char CheckPolygonTim[(sizeof(PolygonTim) == 0x44) ? 1 : -1];

/* Polygon RSD mesh record, built from a parsed RSD file or a battle LMD substitute. */
typedef struct PolygonRsdRecord {
    int fileName;
    int plyFileName;
    int matFileName;
    int count;
    int block;
    int texRecordTable;
    int textures;
    int meshCount;
    int meshes;
} PolygonRsdRecord;

/* Loaded animation (.a): frame count, bone count, frame data block and per-frame table. A frame
 * is 0x18 bytes of root pose then one 0xC-byte rotation per bone (Anim_Create); the table at +0x14
 * holds two pointers per frame, to its root pose and to its rotations. */
typedef struct PolygonAnimation {
    int isLoaded;
    int frameCount;
    int boneCount;
    int flags;
    int frameData;
    struct PolygonAnimFrameEntry *frameTable;
    int ownsBlock;
    int block;
    int matrixCursor;
} PolygonAnimation;

/* The record AnimPlayer_SelectAnimation steps through: a sequence of animation text files, one per
 * play-time step, compiled and loaded as the play time reaches them. Only the members it reads. */
typedef struct PolygonAnimSequence {
    int pad_00;
    int pad_04;
    int directory;  /* +0x08 directory of the text files, or 0 */
    int pad_0C;
    int pad_10;
    int stepCount;  /* +0x14 the play time wraps at it */
    int fileNames;  /* +0x18 char *[stepCount], or 0 */
} PolygonAnimSequence;

/* Binary .P polygon file header and owned heap arrays (0x80 bytes); freed by Pfile_FreeArrays. */
typedef struct PolygonFileArrays {
    int version;                         /* +0x00: file format version (1) */
    int initFlag;                        /* +0x04: runtime initialized flag (1) */
    int kind;                            /* +0x08: vertex format kind (enum GfxD3dVertexFormat) */
    int vertexCount;                     /* +0x0C: 12-byte vertex positions */
    int normalCount;                     /* +0x10: 12-byte vertex normals */
    int xyzCount;                        /* +0x14: 12-byte auxiliary XYZ records */
    int texCoordCount;                   /* +0x18: 8-byte UV pairs */
    int vertexColorCount;                /* +0x1C: 4-byte BGRA vertex colors */
    int edgeCount;                       /* +0x20: 4-byte edge index pairs */
    int polygonCount;                    /* +0x24: 0x18-byte triangles and 4-byte polygon colors */
    int extraPolyCount;                  /* +0x28: 0x18-byte auxiliary polygon records */
    int auxByteTripletCount;             /* +0x2C: 3-byte auxiliary records */
    int hundredCount;                    /* +0x30: 0x64-byte PolygonHundredRecord render states */
    int groupCount;                      /* +0x34: 0x38-byte PolygonFileGroup ranges */
    int boundingBoxCount;                /* +0x38: 0x1C-byte bounding boxes */
    int editMode;                        /* +0x3C: nonzero when sourceIndices[vertexCount] is present */
    union {
        int vertices;                    /* +0x40: PolygonTranslation[vertexCount] */
        struct GfxExecPosition *positions;
    };
    union {
        int normals;                     /* +0x44: PolygonTranslation[normalCount] */
        struct GfxVertexSourceEntry *sourceEntries;
    };
    int xyzRecords;                      /* +0x48: 12-byte records[xyzCount] */
    union {
        int texCoords;                   /* +0x4C: PolygonTexCoord[texCoordCount] */
        struct GfxTexCoordPair *texCoordPairs;
    };
    union {
        int vertexColors;                /* +0x50: BGRA dwords[vertexColorCount] */
        int *vertexColorWords;
    };
    int polygonColors;                   /* +0x54: BGRA dwords[polygonCount] */
    int edges;                           /* +0x58: 4-byte edges[edgeCount] */
    int polygons;                        /* +0x5C: PolygonTriangle[polygonCount] */
    int extraPolys;                      /* +0x60: 0x18-byte records[extraPolyCount] */
    int auxByteTriplets;                 /* +0x64: 3-byte records[auxByteTripletCount] */
    int hundreds;                        /* +0x68: PolygonHundredRecord[hundredCount] */
    int groups;                          /* +0x6C: PolygonFileGroup[groupCount] */
    int boundingBoxes;                   /* +0x70: 0x1C-byte records[boundingBoxCount] */
    int sourceIndices;                   /* +0x74: int[vertexCount] when editMode != 0 */
    int hostTransformFlag;               /* +0x78: runtime host-transform flag */
    struct PolygonEditLists *editSource; /* +0x7C: while the file is built, or 0 */
} PolygonFileArrays;

/* Skeleton hierarchy loaded from an HRC file: parts list and part count. */
typedef struct PolygonSkelHierarchy {
    int initFlag;
    int drawEnabled;
    int parentIndex;
    int loadOptions;
    int partCount;
    int parts;
    int partTable;
    int partList;
    int modelPool;
} PolygonSkelHierarchy;

/* Float transform whose translation column (posX..posZ) skeleton and light code accumulate into. */
typedef struct PolygonTransform {
    float rot00;
    float rot01;
    float rot02;
    int rot03;
    float rot10;
    float rot11;
    float rot12;
    int rot13;
    float rot20;
    float rot21;
    float rot22;
    int rot23;
    float posX;
    float posY;
    float posZ;
    int posW;
} PolygonTransform;

/* Per-bone animation record: offsets, axis angles and scales applied to the skeleton frame. */
typedef struct PolygonBoneTransform {
    unsigned char pad_00[0x4];
    float offsetX;
    float offsetY;
    float offsetZ;
    float angleX;
    float angleY;
    float angleZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    float scale;
    union {
        PolygonTransform placementMatrix; /* multiplied into the root under g_AnmTransformFlags 0x4000 */
        float placementMatrixRows[4][4];
    };
    struct PolygonBoneOutPos *outPos;
    int transformBase;
} PolygonBoneTransform;


/* 64-bit timestamp tick pair stored inside PolygonAnimPlayer. */
typedef struct PolygonPlayerTicks {
    int lo;
    int hi;
} PolygonPlayerTicks;

/* Animation player: start and current frame, frame count, step, hold timer and skeleton pointers. */
typedef struct PolygonAnimPlayer {
    int isActive;
    int startFrame;
    int frameCount;
    int frameStep;
    int isBound;
    int holdTimer;
    int holdLastFrame;
    int holdTimerStart;
    int frame;
    int isPlaying;
    int tickEnabled;
    int totalFrames;
    int speed;
    int animData;
    union {
        struct {
            unsigned char pad_38[0x4];
            float posX; /* +0x3C: the placement (Mini_ChocoboSyncDisplayEntryToObject) */
            float posY;
            float posZ;
            int rotationX; /* +0x48 */
            float rotationY; /* +0x4C: the heading */
            int rotationZ; /* +0x50: -180.0f, stored as its bits */
            unsigned char pad_54[0x58];
        };
        PolygonBoneTransform placement;
    };
    int animFileIndex; /* +0xAC: index into the group entry's file list (AnimGroup_FormatCurrentEntryStatusLine) */
    int loadInfo;
    int skeleton;
    int clip;
    PolygonPlayerTicks frameInterval;
    PolygonPlayerTicks nextFrameDeadline;
    unsigned char pad_CC[0x4];
    int (__cdecl *endCallback)(struct PolygonAnimPlayer *);
    int fileRequest[4];
} PolygonAnimPlayer;
typedef char CheckPolygonAnimPlayer[(sizeof(PolygonAnimPlayer) == 0xE4) ? 1 : -1];

/* Animation player instance holding the current animation, play time and root scale. */
typedef struct AnimPlayerInstance {
    unsigned char pad_00[0x4];
    int startFrame;
    int frameCount;
    unsigned char pad_0C[0x8];
    int time;
    unsigned char pad_18[0x4];
    int startTime;
    int frame;
    unsigned char pad_24[0xc];
    float rootScale;
    int animData;
    unsigned char pad_38[0x74];
    int playTime;
    int animation;
    int model;
    unsigned char pad_B8[0x1C];
    int fileRequest[4];
} AnimPlayerInstance;

/* Animation bake state: frame count, frame table and baked matrix buffer. */
typedef struct PolygonAnimBake {
    unsigned char pad_00[0x4];
    int count;
    int lastFrame;
    unsigned char pad_0C[0x8];
    int frameTable;
    int baked;
    PolygonTransform *matrixBuffer;
    PolygonTransform *matrixCursor;
} PolygonAnimBake;

/* Material face record of 0x38 bytes parsed from a PLY file (Pfile_ParsePlyAndMat). */
typedef struct PolygonPlyMatState {
    int isQuad;
    int isLine;
    int isPolygon;
    int isSprite;
    int spriteWidth;
    int spriteHeight;
    int vertex[4]; /* +0x18: indices into the file's vertex list */
    union {
        int normal[4]; /* +0x28: normal indices; Pfile_EmitTriangle reads the low 16 bits */
        struct {
            short value;
            short hi;
        } normalShort[4];
    };
} PolygonPlyMatState;

/* Group record of 0x38 bytes that Pfile_BuildGroupsFromFaces adds (Pfile_AddGroup): a run of
 * polygons of one kind and material, with where its polygons, positions and texture coordinates
 * start in the file's lists. */
typedef struct PolygonFileGroup {
    int kind; /* +0x00: the entry kind the polygons share */
    int firstPolygon;
    int polygonCount;
    int firstPosition;
    int vertexCount; /* polygonCount * 3 */
    unsigned char pad_14[0x18];
    int firstTexCoord; /* +0x2C */
    int hasTexture;
    int refIndex; /* +0x34: the texture record */
} PolygonFileGroup;

/* The edit lists of a polygon file being built (PolygonFileArrays.editSource, 0x24 bytes).
 * Pfile_CreateEditLists creates them, the Pfile_Add* functions append copies of the records, and
 * Pfile_FlattenEditLists turns each into the record's count and array (named after each member). */
typedef struct PolygonEditLists {
    struct List *positions; /* +0x00: 12-byte vertex positions (Pfile_AddVertex) -> count, vertices */
    struct List *normals; /* +0x04: 12-byte normals (Pfile_AddVertex, Pfile_AddNormal) -> normalCount, normals */
    struct List *unknown08; /* +0x08: 12-byte records Pfile_AddPolygonWithNormal adds with a polygon -> xyzCount, xyzRecords */
    struct List *texCoords; /* +0x0C: 8-byte texture coordinates (Pfile_AddVertex) -> texCoordCount, texCoords */
    struct List *vertexColors; /* +0x10: each vertex's colour word (Pfile_AddVertex; not freed) -> vertexColorCount, vertexColors */
    struct List *polygons; /* +0x14: 0x18-byte polygons (Pfile_AddPolygon, ..WithNormal) -> polygonCount, polygons */
    struct List *hundreds; /* +0x18: 0x64-byte hundreds, the render states (Pfile_AddHundred) -> hundredCount, hundreds */
    struct List *groups; /* +0x1C: 0x38-byte groups (Pfile_AddGroup) -> groupCount, groups */
    struct List *sourceIndices; /* +0x20: each vertex's index in its source mesh, in edit mode (Pfile_AddVertex; not freed) -> sourceIndices */
} PolygonEditLists;

/* The skeleton object an animation draw or model holds: the head of its chain of push and pop nodes. */
typedef struct PolygonAnimNodeList {
    unsigned char pad_00[0xc];
    struct PolygonAnimBakeNode *first;
} PolygonAnimNodeList;

/* Skeleton animation draw state: a flag word and a frame table pointer. */
typedef struct PolygonAnimDraw {
    unsigned char pad_00[0x4];
    int drawEnabled;
    unsigned char pad_08[0x4];
    int flags;
    unsigned char pad_10[0x4];
    int frameTable;
    unsigned char pad_18[0x4];
    int skeleton;
    unsigned char pad_20[0x4];
    PolygonTransform rootBeforePlacement; /* saved under flag 0x400 */
    PolygonTransform rootAfterPlacement; /* saved under flag 0x800 */
    int bias;
} PolygonAnimDraw;

/* Bone of a skeleton parsed from an HRC file: name and parent index, with child data. */
typedef struct PolygonSkelBone {
    int name; /* +0x0 */
    char *parentName;
    int parentIndex;
    float boneOffset;
    int childCount;
    int childTable;
    int modelEntries;
} PolygonSkelBone;

/* Animation placement record zeroed on init, with four floats set to 1.0 at +0x1C. */
typedef struct PolygonAnimPlacement {
    int flags;
    int offsetX;
    int offsetY;
    int offsetZ;
    int angleX;
    int angleY;
    int angleZ;
    int scaleX;
    int scaleY;
    int scaleZ;
    int scale;
    unsigned char pad_2C[0x10];
    float translateX;
    float translateY;
    float translateZ;
} PolygonAnimPlacement;

/* Anim group directory entry: name, directory, extension and entry pointer array. */
typedef struct PolygonAnimGroupEntry {
    int name;
    int block;
    int directory;
    int extension;
    int parsedParam;
    int entryCount;
    int entries;
    int directoryListing;
    struct PolygonAnimPlayer *animPlayer;
} PolygonAnimGroupEntry;


/* Animation group cursor: current index and the count it wraps at. */
typedef struct PolygonAnimGroup {
    int currentIndex;
    int count;
    struct PolygonAnimGroupEntry **entries;
} PolygonAnimGroup;

/* Skeleton animation state: frame count, per-frame data and output buffer cursor. */
typedef struct PolygonSkeletonAnim {
    unsigned char pad_00[0x4];
    int frameCount;
    int subCountMinus1;
    unsigned char pad_0C[0x8];
    int frameData;
    int isBaked;
    int bufferBase;
    int bufferCursor;
} PolygonSkeletonAnim;

/* Parse state while loading an animation group from a text file: entry count and array. */
typedef struct PolygonAnimGroupText {
    unsigned char pad_00[0x4];
    int count;
    struct PolygonAnimGroupEntry **entries;
} PolygonAnimGroupText;

/* Float offset vector added to a placement's translation. */
typedef struct PolygonTranslation {
    float x;
    float y;
    float z;
} PolygonTranslation;

/* Translation bias added to the bone or root matrix translation row. */
typedef struct PolygonTransformBias {
    unsigned char pad_00[0x4];
    float angleBiasX;
    float angleBiasY;
    float angleBiasZ;
    float translateX;
    float translateY;
    float translateZ;
} PolygonTransformBias;

/* Per-bone rotation angles about X, Y and Z used to build the bone matrix. */
typedef struct PolygonBoneRotation {
    float angleX;
    float angleY;
    float angleZ;
} PolygonBoneRotation;

/* Root animation pose record; six words are copied from a pose each time the root channel is set. */
typedef struct PolygonAnimRootPose {
    int rotationX;
    int rotationY;
    int rotationZ;
    union {
        struct {
            int translationX;
            int translationY;
            int translationZ;
        };
        struct {
            float translationXf;
            float translationYf;
            float translationZf;
        };
    };
} PolygonAnimRootPose;

/* Material record whose flags and four words Pfile_BuildHundredFromMaterial reads. */
typedef struct PolygonMaterialRecord {
    int materialKind; /* +0x0 */
    unsigned char pad_4[0x4];
    int flags;
    unsigned char pad_0C[0x1c];
    int textureIndex;
    unsigned char pad_2C[0x20];
    int hasTexturePage;
    int texturePage;
    int clutId;
} PolygonMaterialRecord;

/* Polygon model header: polygon set count and the set array. */
typedef struct PolygonModel {
    unsigned char pad_00[0x4];
    int drawEnabled;
    unsigned char pad_08[0x4];
    int flags;
    int polySetCount;
    struct PolygonSetGroup *polySets;
    unsigned char pad_18[0x4c];
    float matrix[4][4];
    int *deathActivePtr;
} PolygonModel;

/* Animated polygon model: flags and a table of 0x1C-byte frames. */
typedef struct PolygonAnimModel {
    unsigned char pad_00[0xc];
    int flags;
    unsigned char pad_10[0x4];
    int frames;
    unsigned char pad_18[0x4];
    int skeleton;
    unsigned char pad_20[0x4];
    PolygonTransform rootBeforePlacement; /* Anim_BakeFrameMatrices saves the root matrix here under flag 0x400 */
} PolygonAnimModel;

/* Model set of count pointers, allocated by Pfile_AllocModelSet and filled by Pfile_LoadModelSet. */
typedef struct PolygonModelSet {
    int count;
    struct PolygonRsdModel **modelPointers;
} PolygonModelSet;

/* Heap buffers owned by a polygon animation; Anim_Free releases them. */
typedef struct PolygonAnimBuffers {
    unsigned char pad_00[0x10];
    int frameData;
    int frameTable;
    int ownsBlock;
    int block;
} PolygonAnimBuffers;

/* Model handle returned by LoadRsdModel or Pfile_LoadRsdModelEx. */
typedef struct PolygonRsdModel {
    union {
        struct GfxRenderMaterial *matrices;
        int matricesRaw;
    };
    union {
        int polygonSet;
        struct GfxPolygonSet *polySet;
    };
    int rsdRecord;
} PolygonRsdModel;

/* One of a bone's RSD models (PolygonSkelBone.modelEntries, 8 bytes): its text name slot and the
 * model that Skel_BuildHierarchyAndLoadParts loads. */
typedef struct PolygonSkelModelEntry {
    int pad_00;
    PolygonRsdModel *model;
} PolygonSkelModelEntry;

/* 28-byte skeleton frame table entry holding a count and a data pointer. */
typedef struct PolygonSkelFrameEntry {
    unsigned char pad_00[0x8];
    int parentIndex; /* the entry of the parent bone, negative for none */
    float boneLength; /* added to the child's offset when the bone has a parent */
    int count;
    unsigned char pad_14[0x4];
    int dataPointer;
} PolygonSkelFrameEntry;

/* Texture coordinate pair compared for equality by Pfile_TexCoordEquals. */
typedef struct PolygonTexCoord {
    float u;
    float v;
} PolygonTexCoord;

/* Four-byte colour parsed from text tokens; alpha is always set to 0xFF. */
typedef struct PolygonBgraColor {
    union {
        struct {
            char channel0;
            char channel1;
            char channel2;
            char alpha;
        };
        int packed;
    };
} PolygonBgraColor;

/* Parsed polygon entry of 0x58 bytes from a PLY and material file. */
typedef struct PolygonPlyEntry {
    int kind;
    int transType;
    int flags;
    int isFlatShaded;
    int isGouraudShaded;
    PolygonBgraColor colors[4]; /* +0x14: one per vertex of the face */
    int hasTexture;
    int refIndex;
    union {
        float uv[4][2]; /* +0x2C: u, v per vertex */
        PolygonTexCoord texCoords[4];
    };
    unsigned char pad_4C[0xC];
} PolygonPlyEntry;

/* Texture record from the RSD table holding size, VRAM position and origin. */
typedef struct PolygonTexRecord {
    unsigned char pad_00[0x1c];
    int textureWidth;
    int height;
    int vramPositionX;
    int originY;
} PolygonTexRecord;

/* Page and region record from the RSD table holding page width, region size and position. */
typedef struct PolygonPageEntry {
    unsigned char pad_00[0x1c];
    int pageWidth;
    int regionHeight;
    int regionX;
    int pageY;
} PolygonPageEntry;

/* Sprite record from the RSD table holding size and source rectangle origin. */
typedef struct PolygonSpriteItem {
    unsigned char pad_00[0x1c];
    int spriteWidth;
    int spriteHeight;
    int srcX;
    int srcY;
} PolygonSpriteItem;

/* 100-byte polygon record holding flags, a texture index and the bound texture handle. */
typedef struct PolygonHundredRecord {
    unsigned char pad_00[0x8];
    int flags;
    unsigned char pad_0C[0x4];
    int textureIndex;
    int textureHandle;
    unsigned char pad_18[0x2c];
    int blendStateB;
    unsigned char pad_48[0x14];
    int blendStateA;
} PolygonHundredRecord;

/* Indexed image header with column and row counts and a pixel pointer. */
typedef struct PolygonIndexedImage {
    unsigned char pad_00[0x3c];
    int width;
    int height;
    unsigned char pad_44[0x24];
    int bytesPerPixel;
    unsigned char pad_6C[0x68];
    int pixels;
} PolygonIndexedImage;

/* Texture set: a count and a heap array of cached texture handles released by Pfile_FreeTextureSet. */
typedef struct PolygonTextureSet {
    int textureCount;
    int textures;
} PolygonTextureSet;

/* Polygon placement; AnimPlacement_SetScale sets bit 0x40 of flags and stores the scale. */
typedef struct PolygonPlacement {
    unsigned char pad_00[0x38];
    int flags;
    unsigned char pad_3C[0x24];
    union {
        int scale;
        float scaleFloat;
    };
} PolygonPlacement;

/* Blend options block passed to Pfile_ApplyBlendModeToHundreds (PfileLoadOptions +0x1C). */
typedef struct PolygonBlendSource {
    int blendFlags;
    int blendMode;
} PolygonBlendSource;


/* Polygon set record that Anim_DrawSkeletonFrame reads its transform key and light owner from. */
typedef struct PolygonSkeletonFrameRef {
    unsigned char pad_00[0x40];
    int transformKey;
    int lightOwner;
} PolygonSkeletonFrameRef;


/* Three-int vector copied into a bone channel by Anim_SetBoneChannel. */
typedef struct PolygonBoneVector {
    int x;
    int y;
    int z;
} PolygonBoneVector;


/* Float angle slots of an animation placement, read by index. */
typedef struct PolygonAnimAngles {
    unsigned char pad_00[0x48];
    float angleX;
    float angleY;
    float angleZ;
} PolygonAnimAngles;

/* Integer angle slots of an animation placement, written by index. */
typedef struct PolygonAnimAngleSlots {
    unsigned char pad_00[0x48];
    int angleX;
    int angleY;
    int angleZ;
} PolygonAnimAngleSlots;



/* 4-byte node command word in a skeleton animation chain: 16-bit opcode and 16-bit bone index. */
typedef struct PolygonAnimNodeCmd {
    short opcode;
    short boneIndex;
} PolygonAnimNodeCmd;

/* Frame node in a skeleton animation chain, linked through next in Anim_BakeFrameMatrices. */
typedef struct PolygonAnimBakeNode {
    struct PolygonAnimBakeNode *next;
    PolygonAnimNodeCmd cmd;
} PolygonAnimBakeNode;


/* Model fields read by the vertex color passes: polygon-set group count and group array. */
typedef struct PolygonModelVertexColors {
    unsigned char pad_00[0x10];
    int polySetCount;
    struct PolygonSetGroup *polySets;
} PolygonModelVertexColors;


/* Texture in a model's texture table; blend state is copied in from its hundred record. */
typedef struct PolygonTextureRecord {
    unsigned char pad_00[0x2c];
    int pad_2C;
    unsigned char pad_30[0x94];
    int blendStateA;
    int blendStateB;
} PolygonTextureRecord;

/* Skeleton with a partCount table of 0x1C-byte part records starting at parts. */
typedef struct PolygonSkeleton {
    unsigned char pad_00[0x10];
    int partCount;
    int parts;
} PolygonSkeleton;

/* 0x1C-byte skeleton part record with an entry count and a first entry pointer. */
typedef struct PolygonSkeletonPart {
    unsigned char pad_00[0x10];
    int entryCount;
    unsigned char pad_14[0x4];
    int firstEntry;
} PolygonSkeletonPart;

/* Output record that receives the cached device transform for a skeleton part texture. */
typedef struct PolygonPartTexInfo {
    int cachedTransform;
    int source;
} PolygonPartTexInfo;

/* Texture pair record: a TIM texture and a cached texture handle, both released on free. */
typedef struct PolygonTexturePair {
    unsigned char pad_00[0x4];
    int tim;
    int cachedTexture;
} PolygonTexturePair;

/* Surface record used by the 16-bit indexed image blit, giving pitch and pixel data. */
typedef struct PolygonSurface {
    unsigned char pad_00[0x8];
    int pitchBytes;
    int pixelData;
} PolygonSurface;




/* Per-axis root rotation values and root translation read by Anim_BuildRootMatrix. */
typedef struct PolygonRootRotation {
    float rotX;
    float rotY;
    float rotZ;
    PolygonTranslation translation;
} PolygonRootRotation;

/* Polygon triangle record: three consecutive vertex indices and three mesh vertex indices. */
typedef struct PolygonTriangle {
    short initZero_00;
    unsigned short vertexA;
    unsigned short vertexB;
    unsigned short vertexC;
    short meshVertexA;
    short meshVertexB;
    short meshVertexC;
    unsigned char pad_0E[0xa];
} PolygonTriangle;


/* Triangle index record for Pfile_EmitTriangle: three vertex indices into the mesh. */
typedef struct PolygonQuadTriIndex {
    int vertex[3];
} PolygonQuadTriIndex;

/* Per-axis float bias and root translation read by Anim_BuildRootMatrixBiased. */
typedef struct AnimRootBias {
    float biasX;
    float biasY;
    float biasZ;
    PolygonTranslation translation;
} AnimRootBias;

/* Root translation floats scaled in place by Anim_ScaleRootTranslation. */
typedef struct AnimRootTranslation {
    unsigned char pad_00[0xc];
    float translationX;
    float translationY;
    float translationZ;
} AnimRootTranslation;


/* Three float position coordinates written from a frame's bone matrix. */
typedef struct PolygonBoneOutPos {
    float posX;
    float posY;
    float posZ;
} PolygonBoneOutPos;


/* One 8-byte animation frame entry: root pose pointer and bone channel array pointer. */
typedef struct PolygonAnimFrameEntry {
    PolygonAnimRootPose *rootPose;
    PolygonBoneVector *boneChannels;
} PolygonAnimFrameEntry;

/* Blend and pass state for a polygon material, filled from PolygonMaterialRecord and used by the D3D pass setup. */
typedef struct PolygonHundredState {
    int initFlag;
    int emitStates; /* +0x04: 1 from Gfx_AllocPHundred; D3DExecEmitRenderStateBlock emits states only when set */
    int flags;
    int changeMask; /* +0x0C: which states the block sets (all ones from Gfx_AllocPHundred) */
    int textureIndex;
    int texture;
    int hasTexturePage;
    int texturePage;
    int clutId;
    int shadeMode;
    int ambientColor; /* +0x28: the D3DLIGHTSTATE_AMBIENT value (0xFFFFFFFF, white) */
    int clearedWord;
    int material; /* +0x30: points to the D3DLIGHTSTATE_MATERIAL handle (state flag 0x80000) */
    int srcBlend;
    int dstBlend;
    int reservedPreset;
    int alphaRef; /* +0x40: the D3DRENDERSTATE_ALPHAREF value D3DExecEmitRenderStateBlock emits with ALPHAFUNC */
    int blendMode;
    int depthSort; /* +0x48: the set's primitive batches are drawn depth-sorted (spec flag 0x10000000) */
    int sortPerTriangle;
    unsigned char pad_50[0xc];
    int alphaLevel;
    int unusedBlendWord; /* +0x60: copied from the blend record's +0x20; the block is 0x64 bytes */
} PolygonHundredState;

/* Parsed token file (0x14 bytes): token count, token pointer array, byte size, text buffer and preprocess callback. */
typedef struct PolygonTokenFile {
    int tokenCount;
    char **tokens;
    int size;
    char *buffer;
    void (__cdecl *preprocess)(int, char *);
} PolygonTokenFile;

/* Root matrix whose translation floats are written by Anim_BuildRootMatrixBiased. */
typedef struct PolygonRootMatrix {
    unsigned char pad_00[0x30];
    float translateX;
    float translateY;
    float translateZ;
} PolygonRootMatrix;


/* Four-byte colour and alpha read from a TIM texel record. */
typedef struct PolygonTimColor {
    unsigned char colorB;
    unsigned char colorG;
    unsigned char colorR;
    unsigned char alpha;
} PolygonTimColor;

/* File list header for loading a model set: a count and an entries pointer. */
typedef struct PolygonModelFileList {
    int hasFiles;
    int isEnumerated;
    int count;
    int entries;
} PolygonModelFileList;

/* Handle returned by the RSD model loader: matrices, polygon set and parsed record. */
typedef struct PolygonRsdModelHandle {
    int matrices;
    int polygonSet;
    int parsedRecord;
} PolygonRsdModelHandle;

/* Animation descriptor whose pointer at 0x20 addresses 64-byte bone matrices. */
typedef struct PolygonAnimDesc {
    unsigned char pad_00[0xc];
    char axisOrder[3]; /* the rotation axis (0 X, 1 Y, 2 Z) applied at each step */
    unsigned char pad_0F[0x9];
    int baked;
    unsigned char pad_1C[0x4];
    PolygonTransform *matrices;
} PolygonAnimDesc;

/* Model polygon-set group: entry count at +0x10 and entry pointer at +0x18. */
typedef struct PolygonSetGroup {
    unsigned char pad_00[0x10];
    int entryCount;
    unsigned char pad_14[0x4];
    struct PolygonSetEntry *entries; /* an array of entryCount PolygonSetEntry */
} PolygonSetGroup;

/* The RSD model reference of one polygon-set entry: the polygon set the model's draw and colour calls take. */
typedef struct PolygonSetModelRef {
    unsigned char pad_00[0x4];
    void *polySet;
} PolygonSetModelRef;

/* One 8-byte entry of a PolygonSetGroup's entry array; modelRef is null for a bone without a model. */
typedef struct PolygonSetEntry {
    unsigned char pad_00[0x4];
    PolygonSetModelRef *modelRef;
} PolygonSetEntry;










/* Animation record whose base value locates per-bone matrices in the biased build. */
typedef struct AnimBoneBiasSource {
    unsigned char pad_00[0xc];
    char axisOrder[3]; /* the rotation axis (0 X, 1 Y, 2 Z) applied at each step */
    unsigned char pad_0F[0x9];
    int biasEnabled;
    unsigned char pad_1C[0x4];
    int matrixBase;
} AnimBoneBiasSource;

/* Model record supplying the bone table and transform bias for biased matrix builds. */
typedef struct PolygonBiasModel {
    unsigned char pad_00[0x14];
    int boneTable;
    unsigned char pad_18[0x8c];
    int transformBias;
} PolygonBiasModel;

/* Descriptor whose 64-byte matrix block is copied to the destination root matrix. */
typedef struct AnimRootBiasDesc {
    unsigned char pad_00[0xc];
    char axisOrder[3]; /* the rotation axis (0 X, 1 Y, 2 Z) applied at each step */
    unsigned char pad_0F[0x9];
    int biasEnabled;
    unsigned char pad_1C[0x4];
    PolygonTransform *matrixData;
} AnimRootBiasDesc;


/* Model set whose entries each receive a matrix. */
typedef struct PolygonModelSetMatrix {
    int count;
    struct PolygonRsdModel **modelList;
} PolygonModelSetMatrix;



// Four byte channels are written independently by Pfile_ParseColorTokensToBgra.

/* Entity for skeleton load: resource and anim player, shared by the field and mini-game loaders. */
typedef struct PolygonSkeletonLoadSpec {
    int fileName; /* +0x0 */
    int dirName; /* +0x4 */
    unsigned char pad_8[0x8];
    int resource;
    unsigned char pad_14[0xc];
    int animPlayer;
} PolygonSkeletonLoadSpec;



#endif /* LIB_TYPES_POLYGON_H */

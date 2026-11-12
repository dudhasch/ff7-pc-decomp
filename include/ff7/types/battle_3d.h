/* include/ff7/types/battle_3d.h: Battle 3D scene, model, animation, stage and draw-pool types. */
#ifndef FF7_TYPES_BATTLE_3D_H
#define FF7_TYPES_BATTLE_3D_H

#include "ff7/types/battle.h"

/* --- battle_fx_model_draw --- */
/* Model draw submit record: override modes, tints, blend and scalar values for one draw. */
typedef struct BattleFxModelDrawSubmit {
    unsigned char pad_00[0x2];
    char submitMode;
    char scalarOverrideMode;
    char isActive;
    char hasTextureOverride;
    char hasTint;
    char blendOverrideMode;
    int drawColor; /* +0x08: the colour bytes; byte +0x0B is the alpha */
    unsigned char pad_0C[0x8];
    int tintR;
    int tintG;
    int tintB;
    int blendOverrideValue;
    float scrollU;
    float scrollV;
    int texturePage;
    int textureHandle;
} BattleFxModelDrawSubmit;

/* Float block at +0x0C of a model draw record, sign-flipped in place. */
typedef struct BattleFxModelDrawFloats {
    float m00;
    float m01;
    float m02;
    unsigned char pad_0C[0x4];
    float m10;
    float m11;
    float m12;
    unsigned char pad_1C[0x4];
    float m20;
    float m21;
    float m22;
} BattleFxModelDrawFloats;

/* Draw pool node that BattleFx_SubmitModelDraw fills: the matrix block, then the submit record. */
typedef struct BattleFxModelDrawNode {
    int nextNode;
    int nodeKind;
    int batchBlock;
    BattleFxModelDrawFloats matrix; /* +0x0C */
    unsigned char pad_38[0x14];
    BattleFxModelDrawSubmit submit; /* +0x4C */
} BattleFxModelDrawNode;

/* Model state read by the sprite quad draw to select a draw mode. */
typedef struct BattleFxSpriteDrawSource {
    unsigned char pad_00[0x38];
    int mode;
    unsigned char pad_3C[0xa4];
    int clutRowBase;
} BattleFxSpriteDrawSource;

/* Model record whose draw source the lookups and sprite draws read. */
typedef struct BattleFxModelRecord {
    unsigned char pad_00[0x84];
    BattleFxSpriteDrawSource *drawSource;
} BattleFxModelRecord;

/* Model group with a key at 0x10 and a model pointer at 0x14. */
typedef struct BattleFxModelGroupEntry {
    unsigned char pad_00[0x10];
    int key;
    BattleFxModelRecord *model;
    unsigned char pad_18[0x4c];
} BattleFxModelGroupEntry;

/* Model group table whose group count and group pointer the unused lookup reads. */
typedef struct BattleFxModelGroupTable {
    unsigned char pad_00[0x10];
    int groupCount;
    unsigned char pad_14[0x20];
    BattleFxModelGroupEntry *group;
} BattleFxModelGroupTable;

/* Container of model groups searched by key; it also holds the draw pool its draw nodes come from. */
typedef struct BattleFxModelGroupContainer {
    unsigned char pad_00[0x10];
    int groupCount;
    GfxDrawPoolList drawPoolList; /* +0x14 */
    GfxDrawPoolDesc *drawPoolDesc;
    unsigned char pad_2C[0x8];
    BattleFxModelGroupEntry *group;
    unsigned char pad_38[0x64];
    BattleFxModelGroupEntry *textureHandles[2]; /* +0x9C */
} BattleFxModelGroupContainer;

/* The model draw record's group handle: it leads to the container. */
typedef struct BattleFxModelGroupHandle {
    unsigned char pad_00[0x4];
    BattleFxModelGroupContainer *container;
} BattleFxModelGroupHandle;

/* --- b3ddata --- */

/* One battle model load category (0x14 bytes): a second count, the category, and the last three
   words of the FileOpenRequest the models are read with (categories 0..2 read battle.lgp, 2, through
   BattleModel_NameManglerHook; 3..5 read magic.lgp, 3). */
typedef struct BattleModelLoadCategory {
    int secondCount;
    int loadCategory;
    int fromArchive;
    int archiveId;
    int callback;
} BattleModelLoadCategory;

/* Per-actor animation table with two entry lists, read by Battle_GetActorAnimAngle. */
typedef struct BattleActorAnimTable {
    unsigned char pad_00[0x8];
    int entryCount;
    int entries;
    int entryCountB;
    int entriesB;
} BattleActorAnimTable;

/* --- battle_actor_overlay_draw --- */
/* Battle overlay quad primitive packet built from the render state block. */
typedef struct Battle3dOverlayQuad {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char command;
    short xMin;
    short yMin;
    char texU;
    char texV;
    short clutId;
    short xMax;
    short yInset;
    char texUEnd;
    char texVStart;
    short texturePage;
    short xMinLower;
    short yMax;
    char texUStart;
    char texVEnd;
    unsigned char pad_1E[0x2];
    short xMaxLower;
    unsigned char pad_22[0x2];
    char texUEndAlt;
    char texVEndAlt;
} Battle3dOverlayQuad;

/* GPU shaded-quad packet for the battle screen gradient band, linked into the overlay draw list. */
typedef struct BattleFxGradientQuad {
    int tag;
    char color0R;
    char color0G;
    char color0B;
    unsigned char command;
    short x0;
    short y0;
    char color1R;
    char color1G;
    char color1B;
    unsigned char pad_0F[0x1];
    short x1;
    short y1;
    char color2R;
    char color2G;
    char color2B;
    unsigned char pad_17[0x1];
    short x2;
    short y2;
    char color3R;
    char color3G;
    char color3B;
    unsigned char pad_1F[0x1];
    short x3;
    short y3;
} BattleFxGradientQuad;

/* Overlay draw-list quad (PS1-style POLY_F4 layout): tag, flat RGB, command, four corners. */
typedef struct BattleFxOverlayPlate {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    unsigned char command;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    short x3;
    short y3;
} BattleFxOverlayPlate;

/* Rain splash primitive built in the render-state block: RGB bytes, command byte and vertex bytes. */
typedef struct BattleRainSplashPrim {
    int cmdWord;
    char colorR;
    char colorG;
    char colorB;
    char gpuCode;
    unsigned char pad_08[0x4];
    char u0;
    char v0;
    short clut;
    unsigned char pad_10[0x4];
    char u1;
    char v1;
    short tpage;
    unsigned char pad_18[0x4];
    char u2;
    char v2;
    unsigned char pad_1E[0x6];
    char u3;
    char v3;
} BattleRainSplashPrim;

/* Battle graphics data record for screen-space textured quads: scale factors and a buffer table. */
typedef struct BattleFxQuadRecord {
    unsigned char pad_00[0x24];
    float scaleX;
    float scaleY;
    unsigned char pad_2C[0x44];
    int bufferBase;
    unsigned char pad_74[0x4];
    int primPointer;
    int primCode;
} BattleFxQuadRecord;

/* Render state block for battle rain streaks, with the streak colour bytes. */
typedef struct Battle3dRenderStateBlock {
    int colorCommand;
    char startColorR;
    char startColorG;
    char startColorB;
    char opcode;
    unsigned char pad_08[0x4];
    char colorR;
    char colorG;
    char colorB;
} Battle3dRenderStateBlock;

/* The corner alphas of a screen gradient band: the top two corners, then the bottom two. */
typedef struct BattleFxGradientBandColor {
    unsigned char alphaTop;
    unsigned char alphaBottom;
    unsigned char pad_02[0x2];
} BattleFxGradientBandColor;

/* Status-icon digit quad (PSX textured quad) filled by Battle3d_DrawActorStatusIconDigitQuads. */
typedef struct Battle3dStatusIconDigitQuad {
    int tag;
    char colorR;
    char colorG;
    char colorB;
    char opcode;
    short x0;
    short y0;
    char u0;
    char v0;
    short clutId;
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
} Battle3dStatusIconDigitQuad;

/* 6-byte entry of the battle message window queue, shifted down one slot per step. */
typedef struct BattleDisplayTextEntry {
    short textId;
    short param;
    char windowType;
    char duration;
} BattleDisplayTextEntry;

/* The damage number task's slot (Battle_DamageNumberTask, Battle_DrawDamageNumber): its life in
 * frames, screen position, rise step into g_BattleOverlayCurves.damageRise, value, actor and flags. */
#pragma pack(push, 1)
typedef struct BattleDamageNumberTaskSlot {
    short ownerSlot; /* +0x0 */
    short frameCounter; /* +0x2 */
    short lifeFrames; /* +0x4 */
    short screenX; /* +0x6 */
    short screenY; /* +0x8 */
    short baseScreenY; /* +0xa */
    short riseStep; /* +0xc */
    short value; /* +0xe */
    int actorIndex; /* +0x10 */
    short flags; /* +0x14 */
    unsigned char pad_16[0x2];
} BattleDamageNumberTaskSlot;
#pragma pack(pop)

/* --- char --- */
/* Bone of a battle model skeleton, filled from a bone table, with an animation block. */
typedef struct Battle3dModelBone {
    int name;
    int parentName;
    int parentIndex;
    int length;
    int partCount;
    unsigned char pad_14[0x4];
    int animation;
} Battle3dModelBone;

/* Battle stage model load: stage id, loaded file data and the model set. */
typedef struct Battle3dStageLoad {
    int stageId;
    int fileData;
    int modelSet;
} Battle3dStageLoad;

/* Battle model descriptor file header: entry count, with the entry table starting at offset 0x34. */
typedef struct Battle3dModelDescriptor {
    unsigned char pad_00[0xc];
    int entryCount;
    unsigned char pad_10[0x20];
    int entries;
} Battle3dModelDescriptor;

/* Stage descriptor whose model count sizes the model set built by BattleStage_LoadModelSet. */
typedef struct Battle3dStageDescriptor {
    unsigned char pad_00[0x14];
    int modelCount;
    int textureCount;
} Battle3dStageDescriptor;

/* Model index record holding a running base offset and a size. */
typedef struct Battle3dModelIndexRecord {
    unsigned char pad_00[0x4];
    int indexBase;
    unsigned char pad_08[0x8];
    int size;
    unsigned char pad_14[0x4];
} Battle3dModelIndexRecord;

/* 28-byte skeleton bone whose three ints are copied out as a bone vector. */
typedef struct Battle3dSkeletonBone {
    unsigned char pad_00[0x8];
    int vectorX;
    int vectorY;
    int vectorZ;
    unsigned char pad_14[0x8];
} Battle3dSkeletonBone;

#pragma pack(push, 1)
typedef struct BattleModelEnemyFlags232 {
    unsigned char blendMode; /* +0x0 */
    unsigned char pad_1[0x16];
    unsigned char enemyBlendOverrideGroup4Byte23; /* +0x17 */
} BattleModelEnemyFlags232;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleModelEnemyBlendModes48 {
    unsigned char blendMode; /* +0x0 */
    unsigned char pad_1[0x1c];
    unsigned char enemyBlendOverrideGroup6Byte29; /* +0x1d */
    unsigned char enemyBlendOverrideGroup6Byte30; /* +0x1e */
} BattleModelEnemyBlendModes48;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleEnemyBlendOverrideGroup7 {
    unsigned char blendMode; /* +0x0 */
    unsigned char enemyBlendOverrideGroup7Byte1; /* +0x1 */
    unsigned char enemyBlendOverrideGroup7Byte2; /* +0x2 */
    unsigned char pad_3[0x1c];
    unsigned char enemyBlendOverrideGroup7Byte31; /* +0x1f */
} BattleEnemyBlendOverrideGroup7;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleEnemyBlendOverrideGroup9 {
    unsigned char blendMode; /* +0x0 */
    unsigned char enemyBlendOverrideGroup9Byte1; /* +0x1 */
    unsigned char enemyBlendOverrideGroup9Byte2; /* +0x2 */
    unsigned char pad_3[0x13];
    unsigned char enemyBlendOverrideGroup9Byte22; /* +0x16 */
    unsigned char pad_17[0xc];
    unsigned char enemyBlendOverrideGroup9Byte35; /* +0x23 */
} BattleEnemyBlendOverrideGroup9;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleEnemyBlendOverrideGroup8 {
    unsigned char enemyBlendOverrideGroup8Byte0; /* +0x0 */
    unsigned char pad_1[0x4];
    unsigned char enemyBlendOverrideGroup8Byte5; /* +0x5 */
    unsigned char pad_6[0x4];
    unsigned char enemyBlendOverrideGroup8Byte10; /* +0xa */
    unsigned char pad_b[0x19];
    unsigned char enemyBlendOverrideGroup8Byte36; /* +0x24 */
    unsigned char pad_25[0x21];
    unsigned char enemyBlendOverrideGroup8Byte70; /* +0x46 */
    unsigned char enemyBlendOverrideGroup8Byte71; /* +0x47 */
    unsigned char pad_48[0xa];
    unsigned char enemyBlendOverrideGroup8Byte82; /* +0x52 */
    unsigned char pad_53[0x7];
    unsigned char enemyBlendOverrideGroup8Byte90; /* +0x5a */
} BattleEnemyBlendOverrideGroup8;
#pragma pack(pop)

/* --- lmd --- */
/* Battle LMD polygon data record (0x38 bytes) from lmd.cpp; field uses not yet recovered. */
typedef struct Battle3dPolygonData {
    int enabled;
    unsigned char pad_04[0x4];
    int isUsed;
    unsigned char pad_0C[0xc];
    int scaledX;
    int scaledY;
    int scaledZ;
    int widthUnits;
    unsigned char pad_28[0x10]; /* to 0x38 bytes, the step of every pointer through it */
} Battle3dPolygonData;

/* LMD polygon record read by Battle_BuildPolygonDataFromLmd: extents, four UV pairs and flags. */
typedef struct Battle3dLmdTintedQuad {
    unsigned short x;
    unsigned short y;
    unsigned short z;
    unsigned short width;
    int packedColor;
    unsigned char u0;
    unsigned char v0;
    unsigned short clutId;
    unsigned char u1;
    unsigned char v1;
    unsigned char u2;
    unsigned char v2;
    unsigned char u3;
    unsigned char v3;
    unsigned short flags;
} Battle3dLmdTintedQuad;

/* LMD quad face record: four texture coordinate pairs and a texture page byte. */
typedef struct Battle3dLmdQuad {
    unsigned short x;
    unsigned short y;
    unsigned short z;
    unsigned short width;
    unsigned char u0;
    unsigned char v0;
    unsigned short clutId;
    unsigned char u1;
    unsigned char v1;
    unsigned char u2;
    unsigned char v2;
    unsigned char u3;
    unsigned char v3;
    unsigned char shade;
    unsigned char tpageByte;
} Battle3dLmdQuad;

/* LMD triangle face record: three texture coordinate pairs and draw flags. */
typedef struct Battle3dLmdTriangle {
    unsigned short x;
    unsigned short y;
    unsigned short z;
    unsigned short flags;
    int packedColor;
    unsigned char u0;
    unsigned char v0;
    unsigned short clutId;
    unsigned char u1;
    unsigned char v1;
    unsigned char u2;
    unsigned char v2;
} Battle3dLmdTriangle;

/* Battle3d LMD face record: three vertex words, a texture page byte and byte pairs. */
typedef struct Battle3dLmdFaceRecord {
    unsigned short x;
    unsigned short y;
    unsigned short z;
    unsigned char shade;
    char tpageByte;
    unsigned char u0;
    unsigned char v0;
    unsigned short clutId;
    unsigned char u1;
    unsigned char v1;
    unsigned char u2;
    unsigned char v2;
} Battle3dLmdFaceRecord;

/* Twelve-byte float triple allocated per entry while building LMD polygon data. */
typedef struct Battle3dLmdFloatVec {
    float pad_00;
    float pad_04;
    float pad_08;
} Battle3dLmdFloatVec;

/* Only the fields the code reads; the size is not known. */
typedef struct Battle3dLmdPart {
    int pad_00;
    int pad_04;
    int pad_08;
    int pad_0C;
    int pad_10;
    int pad_14;
    int pad_18;
    int pad_1C;
    int pad_20;
    int pad_24;
    int pad_28;
    float pad_2C;
    float pad_30;
    float pad_34;
    float pad_38;
    float pad_3C;
    float pad_40;
    float pad_44;
    float pad_48;
    int pad_4C;
    int pad_50;
    int pad_54;
} Battle3dLmdPart;

/* Packed polygon vertex colour and its channels (stored blue, green, red, alpha). */
typedef union Battle3dLmdColor {
    int packed;
    struct {
        char b;
        char g;
        char r;
        char a;
    } byte;
} Battle3dLmdColor;

/* A texture coordinate, read as a float and copied into the atlas bounds as its raw bits. */
typedef union Battle3dLmdUv {
    float f;
    int bits;
} Battle3dLmdUv;

/* Polygon record filled from an LMD triangle or polygon when building battle model polygon data. */
typedef struct Battle3dPolygonRecord {
    int polyKind;
    int blendMode;
    int flags;
    int isFlatShaded;
    int isGouraudShaded;
    Battle3dLmdColor color0;
    Battle3dLmdColor color1;
    Battle3dLmdColor color2;
    Battle3dLmdColor color3;
    int hasTexture;
    int widthUnits;
    Battle3dLmdUv u0;
    Battle3dLmdUv v0;
    Battle3dLmdUv u1;
    Battle3dLmdUv v1;
    Battle3dLmdUv u2;
    Battle3dLmdUv v2;
    Battle3dLmdUv u3;
    Battle3dLmdUv v3;
    int hasTexturePage;
    int texturePage;
    int clutId;
} Battle3dPolygonRecord;

/* First colour of a Gouraud polygon (red, green, blue) with the attribute byte above it. */
typedef union Battle3dLmdPrimHead {
    int word;
    struct {
        char r;
        char g;
        char b;
        char attr;
    } rgb;
} Battle3dLmdPrimHead;

/* Battle LMD Gouraud quad: position, width, first colour and the other three colours. */
typedef struct Battle3dLmdGouraudQuad {
    unsigned short x;
    unsigned short y;
    unsigned short z;
    unsigned short width;
    Battle3dLmdPrimHead flags;
    char r1;
    char g1;
    char b1;
    unsigned char pad_0F[0x1];
    char r2;
    char g2;
    char b2;
    unsigned char pad_13[0x1];
    char r3;
    char g3;
    char b3;
} Battle3dLmdGouraudQuad;

/* Battle LMD Gouraud triangle: position, first colour and the other two colours. */
typedef struct Battle3dLmdGouraudTriangle {
    unsigned short x;
    unsigned short y;
    unsigned short z;
    unsigned char pad_06[0x2];
    Battle3dLmdPrimHead flags;
    char r1;
    char g1;
    char b1;
    unsigned char pad_0F[0x1];
    char r2;
    char g2;
    char b2;
} Battle3dLmdGouraudTriangle;

/* --- stage --- */
struct Battle3dStagePartTable;

/* Legacy battle stage record (0xC bytes): stage id, stage data copy and part table. */
typedef struct Battle3dStageRecord {
    int stageId;
    void *stageData;
    struct Battle3dStagePartTable *modelSet;
} Battle3dStageRecord;

/* Descriptor from BattleModel_AllocDescriptor, filled by BattleStage_LoadStageDatLegacy. */
typedef struct BattleStageDescriptor2 {
    unsigned char pad_00[0x4];
    int hasInfoFile;
    unsigned char pad_08[0x8];
    int clearedWord;
    int partCount;
    int subCount;
} BattleStageDescriptor2;

/* Part table of a stage: the number of part meshes and the array of mesh handles. */
typedef struct Battle3dStagePartTable {
    int partCount;
    int *partMeshes;
} Battle3dStagePartTable;

#endif /* FF7_TYPES_BATTLE_3D_H */

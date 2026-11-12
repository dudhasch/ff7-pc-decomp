/* include/lib/api/polygon.h: the functions and objects lib/polygon defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_POLYGON_H
#define LIB_API_POLYGON_H

#ifdef __cplusplus
extern "C" {
#endif
struct PolygonTranslation;
struct GfxDrawSpec;
struct PfileLoadOptions;
struct PolygonAnimAngleSlots;
struct PolygonAnimAngles;
struct PolygonAnimBuffers;
struct PolygonAnimGroup;
struct PolygonAnimModel;
struct PolygonAnimPlacement;
struct PolygonAnimPlayer;
struct PolygonBgraColor;
struct PolygonBlendSource;
struct PolygonBoneVector;
struct GfxEngineContext;
struct PolygonHundredRecord;
struct PolygonHundredState;
struct PolygonIndexedImage;
struct PolygonModel;
struct PolygonModelVertexColors;
struct PolygonPartTexInfo;
struct PolygonPlacement;
struct PolygonPlyEntry;
struct PolygonPlyMatState;
struct PolygonPlayerTicks;
struct PolygonSkeleton;
struct PolygonSurface;
struct PolygonTexCoord;
struct PolygonTextureRecord;
struct PolygonTextureSet;
struct PolygonTranslation;
struct PolygonSkeletonLoadSpec;
struct PolygonAnimGroupEntry;
struct PolygonAnimRootPose;
struct PolygonSkelHierarchy;
struct PolygonSkeletonAnim;
struct PolygonTim;
struct AnimPlayerInstance;
struct FileOpenRequest;
struct GfxPrimitiveMeshVertex;
struct PolygonAnimBake;
struct PolygonAnimDraw;
struct PolygonAnimation;
struct PolygonBoneTransform;
struct PolygonFileArrays;
struct PolygonRsdRecord;
struct GfxPolygonSet;
struct PolygonModelFileList;
struct PolygonRsdModel;
struct PolygonModelSetMatrix;
struct PolygonTokenFile;
typedef void(__cdecl *GfxPolygonSetFn)(int, int);

/* anm.cpp */
extern float g_AnmZeroBoneRotationDraw[3];
extern float g_AnmZeroBoneRotationBake[3];
void __cdecl Anim_GetDefaultTransformFlags(void *out);
int *__cdecl Anim_GetTransformFlagsPtr(void);
void __cdecl Anim_BuildRootMatrix(struct PolygonTransform *dstMatrix, struct PolygonRootRotation **rotations, struct PolygonAnimDesc *desc, void *context);
void __cdecl Anim_BuildRootMatrixBiased(struct PolygonTransform *dstMatrix, struct AnimRootBias **rotations, struct AnimRootBiasDesc *desc, struct PolygonBiasModel *owner);
struct PolygonTransform *__cdecl Anim_BuildBoneMatrix(int boneIndex, struct PolygonTransform *matrix, struct PolygonBoneRotation *rotation, void *extra, struct PolygonAnimDesc *anim, struct PolygonSkelFrameEntry *bone, struct PolygonAnimModel *skeleton);
struct PolygonTransform *__cdecl Anim_BuildBoneMatrixBiased(int boneIndex, struct PolygonTransform *matrix, struct PolygonBoneRotation *angles, int extra, struct AnimBoneBiasSource *anim, struct PolygonSkelFrameEntry *bone, struct PolygonBiasModel *model);
void __cdecl Anim_DrawSkeletonFrame(int frameIndex, struct PolygonSkeletonAnim *skeleton, struct PolygonBoneTransform *transform, struct PolygonAnimDraw *draw, struct GfxEngineContext *ctx);
void __cdecl Anim_BakeFrameMatrices(int frameIndex, struct PolygonAnimBake *bake, struct PolygonAnimModel *model);
void __cdecl Anim_BakeAllFrameMatrices(struct PolygonAnimBake *bake, struct PolygonAnimModel *model);
void __cdecl Gfx_DrawModelPolygonSets(struct PolygonModel *model, struct GfxEngineContext *drawParam);
void __cdecl Gfx_ModulateModelVertexColors(int color, struct PolygonModelVertexColors *model);
void __cdecl Gfx_ApplyDrawSpecToModel(struct GfxDrawSpec *drawSpec, struct PolygonModel *model);
void __cdecl Gfx_ForEachModelPolygonSet(int arg1, GfxPolygonSetFn callback, struct PolygonModel *arg3);
void __cdecl Gfx_SaveModelVertexColors(struct PolygonModel *model);
void __cdecl Gfx_SavePolygonSetVertexColorsCallback(int index, struct GfxPolygonSet *polySet);

/* plytopd.cpp */
int __cdecl IsPowerOfTwo(int value);
int __cdecl RoundUpToPowerOfTwo(int value);
void __cdecl Token_StripHashComments(int length, char *buffer);
int __cdecl Pfile_ParseColorTokensToBgra(struct PolygonBgraColor *color, int index, char **tokens);
int __cdecl Pfile_ParseUvTokensDiv256(float *output, int index, char **tokens);
int __cdecl Pfile_TimPixelToUv(int unused, float *uv, int index, char **tokens, int width, int height, int textureWidth, int textureHeight);
void __cdecl Pfile_EmitTriangle(int triIndex, int quadIndex, struct PolygonTranslation *vertices, int unused, struct PolygonPlyEntry *polygon, struct PolygonPlyMatState *mesh, struct PolygonFileArrays *geometry);
void __cdecl Pfile_ParsePlyAndMat(int texPage, int textured, float scale, struct PolygonRsdRecord *rsdRecord, struct PolygonFileArrays *fileArrays, struct PolygonTokenFile *plyTokens, struct PolygonTokenFile *matTokens);
int __cdecl Pfile_BuildFromPlyMat(int texPage, float scale, struct PolygonRsdRecord *rsdRecord, int options, int matPath, int plyPath, int outPath);
int __cdecl Anim_GetFrameDataSize(struct PolygonAnimation *animation);
void __cdecl Anim_Free(struct PolygonAnimBuffers *anim);
int __cdecl Anim_Create(int frameCount, int boneCount, int flags);
void __cdecl Anim_SetRootChannel(int channel, int frameIndex, struct PolygonAnimRootPose *pose, struct PolygonAnimation *anim);
void __cdecl Anim_SetBoneChannel(int channel, int frameIndex, int boneIndex, struct PolygonBoneVector *vector, struct PolygonAnimation *anim);
int __cdecl Anim_WriteBinary(struct PolygonAnimation *anim, int path);
int __cdecl AnimText_LookupKeyword(int tokenIndex, struct PolygonTokenFile *tokenFile);
void __cdecl Skel_FreeStringArray(int count, int strings);
void __cdecl Skel_Free(struct PolygonSkelHierarchy *skel);
int __cdecl Skel_Create(int partCount);
int __cdecl Skel_LoadHrc(int fileContext, int pushDirectory, int fileName);
int __cdecl Skel_LoadHrcFromDisk(int pushDirectory, int fileName);
int __cdecl Skel_EmitChildrenRecursive(int parentIndex, struct PolygonSkelHierarchy *skel);
void __cdecl Skel_ScaleBoneLengths(float scale, struct PolygonSkelHierarchy *skel);
void __cdecl Pfile_ReleaseHundredTextures(struct PolygonFileArrays *model);
void __cdecl Pfile_ApplyBlendModeToHundreds(struct PolygonBlendSource *blendSource, struct PolygonFileArrays *model);
void __cdecl Pfile_CopyBlendStateToTexHeader(struct PolygonTextureRecord *texHeader, struct PolygonHundredRecord *hundred);
void __cdecl Pfile_BindTexturesToHundreds(void *request, int textureCount, int textures, struct PolygonFileArrays *model, struct GfxEngineContext *flags);
void __cdecl Skel_BuildHierarchyAndLoadParts(int dirName, struct PolygonSkelHierarchy *skel, struct PfileLoadOptions *loadOptions, int modelPool);
void __cdecl Skel_SetPartsVisible(int visible, struct PolygonSkelHierarchy *skeleton);
int __cdecl Skel_GetFirstPartTexInfo(int partIndex, struct PolygonSkeleton *skeleton, struct PolygonPartTexInfo *out);
void __cdecl Pfile_FreeRsdMeshes(struct PolygonRsdRecord *rsd);
void __cdecl Pfile_FreeRsdRecord(struct PolygonRsdRecord *rsd);
int __cdecl Pfile_ParseRsdFile(void *fileBuffer, int fileName);
void __cdecl Pfile_BlitIndexedImageToSurface16(int destX, int destY, struct PolygonIndexedImage *image, struct PolygonSurface *surface);
void __cdecl Pfile_SetHundredBlendMode(int apply, int blendMode, struct PolygonHundredState *hundred);
int __cdecl Pfile_CompareIntsEqual(int lhs, int rhs);
void __cdecl Pfile_BuildGroupsFromFaces(int flags, void *pointData, int normalData, int faceCount, struct PolygonPlyMatState *matStates, int entryCount, struct PolygonPlyEntry *entries, struct PolygonFileArrays *fileArrays);
void __cdecl AnimPlacement_Init(struct PolygonAnimPlacement *placement);
void __cdecl AnimPlayer_Destroy(struct PolygonAnimPlayer *player);
int __cdecl AnimPlayer_Create(int frameCount, struct PolygonSkeletonLoadSpec *loadInfo, struct PolygonSkelHierarchy *skeleton, int clip);
void __cdecl AnimPlayer_Rewind(struct AnimPlayerInstance *player);
void __cdecl AnimPlayer_Tick(void *timestamp, struct PolygonAnimPlayer *player);
void __cdecl AnimPlayer_BindAnimation(int unused, int anim, struct PolygonAnimPlayer *player);
void __cdecl AnimPlayer_SelectAnimation(int time, int delta, int bake, struct AnimPlayerInstance *player);
void __cdecl AnimPlacement_AddTranslation(struct PolygonTranslation *offset, struct PolygonAnimPlacement *placement);
float __cdecl AnimPlacement_GetAngleX(struct PolygonAnimAngles *placement);
float __cdecl AnimPlacement_GetAngleY(struct PolygonAnimAngles *placement);
float __cdecl AnimPlacement_GetAngleZ(struct PolygonAnimAngles *placement);
float __cdecl AnimPlacement_GetAngleByIndex(int axis, struct PolygonAnimAngles *placement);
void __cdecl AnimPlacement_SetAngleX(int angle, struct PolygonAnimAngleSlots *placement);
void __cdecl AnimPlacement_SetAngleY(int angle, struct PolygonAnimAngleSlots *placement);
void __cdecl AnimPlacement_SetAngleZ(int angle, struct PolygonAnimAngleSlots *placement);
void __cdecl AnimPlacement_SetAngleByIndex(int axis, int angle, struct PolygonAnimAngleSlots *placement);
float __cdecl AnimPlacement_GetScale(struct PolygonPlacement *placement);
int __cdecl Skel_LoadHrcAndBuild(struct PolygonSkeletonLoadSpec *skelLoad, int model, struct GfxEngineContext *context);
void __cdecl AnimGroupEntry_Destroy(struct PolygonAnimGroupEntry *entry);
int __cdecl AnimGroupEntry_Create(void);
void __cdecl AnimGroup_Destroy(struct PolygonAnimGroup *group);
int __cdecl AnimGroup_LoadFromTextFile(int fileName);
void __cdecl AnimGroup_AdvanceCurrentIndex(int delta, struct PolygonAnimGroup *group);
void __cdecl AnimGroup_FormatCurrentEntryStatusLine(int isClosed, int xPos, int yPos, int color, void(__cdecl *drawTextFn)(int, int, char *, int), struct PolygonAnimGroup *group);
struct PolygonAnimGroupEntry *__cdecl AnimGroup_GetCurrentEntry(struct PolygonAnimGroup *group);
int __cdecl Skel_LoadHrcAndCreatePlayer(int kind, int time, struct PolygonSkeletonLoadSpec *skelLoad, int model, struct GfxEngineContext *context);
void __cdecl AnimPlayer_TickGroup(struct PolygonAnimGroup *group);
void __cdecl AnimGroup_ToggleCurrentPlayerPause(struct PolygonAnimGroup *group);
void __cdecl AnimGroup_TranslateCurrentPlayer(struct PolygonTranslation *offset, struct PolygonAnimGroup *group);
float __cdecl AnimGroup_GetCurrentPlayerFloat48(struct PolygonAnimGroup *group);
float __cdecl AnimGroup_GetCurrentPlayerFloat4C(struct PolygonAnimGroup *group);
float __cdecl AnimGroup_GetCurrentPlayerFloat50(struct PolygonAnimGroup *group);
float __cdecl AnimGroup_GetCurrentPlayerFloatByIndex(int axis, struct PolygonAnimGroup *group);
void __cdecl AnimGroup_SetCurrentPlayerAngleX(int angle, struct PolygonAnimGroup *group);
void __cdecl AnimGroup_SetCurrentPlayerAngleY(int angle, struct PolygonAnimGroup *group);
void __cdecl AnimGroup_SetCurrentPlayerAngleZ(int angle, struct PolygonAnimGroup *group);
void __cdecl AnimGroup_SetCurrentPlayerAngleByAxis(int axis, int angle, struct PolygonAnimGroup *group);
float __cdecl AnimGroup_GetCurrentPlayerScale(struct PolygonAnimGroup *group);
void __cdecl AnimGroup_SetCurrentPlayerScale(int scale, struct PolygonAnimGroup *group);
void __cdecl AnimGroup_ResetPlayerFrames(struct PolygonAnimGroup *group);
void __cdecl Pfile_RefreshHundredBlendModes(struct PolygonFileArrays *model);
extern char g_PlytopdDebugBuffer[200];
extern char g_PlytopdEmptyKeyword[4];
extern unsigned char g_PlytopdEmptyAnimName[4];

/* polygon.cpp */
void __cdecl Pfile_DestroyEditLists(struct PolygonFileArrays *record);
void __cdecl Pfile_FreeArrays(int freeRecord, struct PolygonFileArrays *record);
void __cdecl Pfile_Free(struct PolygonFileArrays *record);
struct PolygonFileArrays *__cdecl Pfile_CreatePolygonData(int createEdits, int kind);
void __cdecl Pfile_CreateEditLists(struct PolygonFileArrays *record);
int __cdecl Pfile_CloneRecordToHeap(int size, void *src, char *file, int line);
void __cdecl Pfile_AddVertex(int sourceIndex, int *color, void *texCoord, void *normal, void *position, struct PolygonFileArrays *record);
void __cdecl Pfile_AddPolygon(void *polygon, struct PolygonFileArrays *record);
void __cdecl Pfile_AddPolygonWithNormal(void *unknown08, void *polygon, struct PolygonFileArrays *record);
void __cdecl Pfile_AddNormal(void *normal, struct PolygonFileArrays *record);
void __cdecl Pfile_AddHundred(void *hundred, struct PolygonFileArrays *record);
void __cdecl Pfile_AddGroup(void *group, struct PolygonFileArrays *record);
void __cdecl Pfile_FlattenEditLists(struct PolygonFileArrays *record);
void __cdecl Pfile_PostLoadFixupStub(struct PolygonFileArrays *arrays);
void __cdecl Pfile_BuildPolygonColors(struct PolygonFileArrays *object);
struct PolygonFileArrays *__cdecl Pfile_LoadBinary(struct FileOpenRequest *request, int withEdits, int fileName);
void __cdecl Pfile_WriteBinaryEx(int skipWeld, int fileName, struct PolygonFileArrays *record);
void __cdecl Pfile_WriteBinary(int fileName, struct PolygonFileArrays *record);
int __cdecl Pfile_TexCoordEquals(struct PolygonTexCoord *uvA, struct PolygonTexCoord *uvB);
int __cdecl Pfile_VertexAndColorEqual(struct PolygonTranslation *vertexA, struct PolygonTranslation *vertexB, int *colorA, int *colorB);
void __cdecl Pfile_WeldDuplicateVertices(struct PolygonFileArrays *fileArrays);
void __cdecl Pfile_ScaleVertices(float scale, struct PolygonFileArrays *arrays);

/* tim.cpp */
int __cdecl Pfile_BuildTextureTileSetFromTim(int tileWidth, int tileHeight, struct PolygonTim *timRecord, int context);
int __cdecl Pfile_WriteTimRecordToFile(int useRegion, int regionX, int regionY, int regionWidth, int height, struct PolygonTim *timRecord, int fileName);

/* rsd.cpp */
int __cdecl Pfile_GetAssetSourceMode(void);
void __cdecl Pfile_SetAssetSourceMode(int value);
int __cdecl Pfile_GetDefaultModelFlags(void);
void __cdecl Pfile_SetDefaultModelFlags(int value);
void __cdecl Pfile_FreeRsdModel(void *model, void *ctx);
void __cdecl Pfile_FreeModelAndNull(int handlePtr, void *context);
int __cdecl Pfile_LoadRsdModelEx(struct PolygonRsdRecord *existingRecord, struct PfileLoadOptions *source, int fileName, struct GfxEngineContext *context);
void __cdecl Pfile_StoreValueInModelPolygonSetHead(int value, struct PolygonRsdModel *modelHandle);
void __cdecl Pfile_InstallDefaultMatrixOnModel(struct PolygonRsdModel *handlePtr, struct GfxEngineContext *context);
void __cdecl Gfx_SetModelMatrix(int matrix, int modelHandle, struct GfxEngineContext *context);
void __cdecl Pfile_SetModelMatrixSlot(int slot, int value, int option, struct PolygonRsdModel *modelHandle);
void __cdecl Pfile_SetModelVertexColor(int color, int modelHandle);
void __cdecl Pfile_RestoreModelVertexColors(int modelHandle);
int __cdecl Pfile_InitModelSetRequest(int request);
void __cdecl Pfile_FreeModelSet(void *models, void *ctx);
int __cdecl Pfile_AllocModelSet(int count, struct GfxEngineContext *context);
int __cdecl Pfile_LoadModelSet(struct PolygonModelFileList *fileList, struct PfileLoadOptions *context, struct GfxEngineContext *engine);
void __cdecl Pfile_StoreValueInEachModelSetEntry(int value, struct PolygonModelSetMatrix *modelSet);
void __cdecl Pfile_InstallDefaultMatrixOnModelSet(struct PolygonModelSetMatrix *modelSet, struct GfxEngineContext *context);
void __cdecl Pfile_SetModelSetMatrix(int matrix, struct PolygonModelSetMatrix *modelSet, struct GfxEngineContext *context);
void __cdecl Pfile_ForEachModelSetEntryMember(int context, void(__cdecl *)(int, int), int modelSet);

/* tim.cpp */
void __cdecl Pfile_ComputeTimTextureSize(struct PolygonTim *texture);
void __cdecl Pfile_DestroyTim(struct PolygonTim *timTexture);
int __cdecl Pfile_LoadTimFromContext(struct FileOpenRequest *fileContext, const char *fileName);
int __cdecl Pfile_TimHalfwordWidthToPixelWidth(int mode, int halfwordWidth);
int __cdecl Pfile_TimSpreadNibbles(int value);
char __cdecl Pfile_TimPackNibblePair(int value);
int __cdecl Pfile_BuildTextureFromTimRect(int rectX, int rectY, int rectWidth, int rectHeight, struct PolygonTim *timRecord, const char *fileName);
int __cdecl Pfile_BuildTextureFromTim(struct PolygonTim *timRecord, const char *fileName);
void __cdecl Pfile_FreeTextureSet(int releaseTextures, struct PolygonTextureSet *textureSet);
int __cdecl Pfile_AllocTextureSetHeader(int count);

/* tim.cpp */
void __cdecl Pfile_LoadTim(const char *fileName);

struct PolygonTexturePair;

/* plytopd.cpp */
void __cdecl Anim_ScaleRootTranslation(float scale, struct PolygonAnimation *anim);
void __cdecl Pfile_FreeTexturePairRecord(struct PolygonTexturePair *pair);

struct GfxFloatColorAddend;

/* plytopd.cpp */
int __cdecl Anim_LoadBinary(struct FileOpenRequest *fileRequest, int openFlags);
void __cdecl Anim_LoadBinaryFromDisk(int openFlags);
int __cdecl Anim_CompileTextToBinary(int fileName, int outputFile);

void __cdecl Gfx_AddColorToModelVertexColors(struct GfxFloatColorAddend *color, struct PolygonModelVertexColors *model, int context);
/* rsd.cpp, a C++ TU, declares a local extern "C" override with raw-bit argument types. */
#ifndef __cplusplus
int __cdecl Pfile_LoadRsdContents(struct PfileLoadOptions *options, float scale, struct PolygonRsdRecord *rsd);
#endif
int __cdecl LoadRsdModel(struct PfileLoadOptions *source, const char *fileName, void *context);
void __cdecl Anim_SetTransformFlags(void *src);
struct PolygonPlacement;
void __cdecl AnimPlacement_SetScale(int scale, struct PolygonPlacement *placement);
void __cdecl Gfx_RestoreModelVertexColors(struct PolygonModelVertexColors *model, int context);

/* rsd.cpp */
void __cdecl Pfile_InitLoadOptions(struct PfileLoadOptions *opts);
void __cdecl Pfile_InitLoadOptionsWithBlend(int blend, struct PfileLoadOptions *opts);
void __cdecl Pfile_SetLoadOptionsBlendMode(int blend, struct PfileLoadOptions *opts);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_POLYGON_H */

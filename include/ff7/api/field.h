/* include/ff7/api/field.h: the functions and objects ff7/field defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_FIELD_H
#define FF7_API_FIELD_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
struct GfxRenderObject;
#endif
struct FieldLineDrawScratch;
struct FieldPolygonSetCountArray;
struct FieldLayer;
struct FieldLayerDrawOrder;
struct GfxLightOwner;
struct FieldModelDescriptor;
struct FieldEncounterEntry;
struct FieldTileDrawOrder;
struct FieldHumanActorBuffer0;
struct FieldClearColor;
extern unsigned char g_FieldHeadMeshCommands[4];
struct FieldHeadTrigTable;
extern struct FieldHeadTrigTable g_FieldHeadTrigTable;
struct FieldCameraCoordsRect;
struct GfxEngineContext;
struct BattleRgbaColor;
struct FieldActorRecord;
struct PolygonModel;
struct FieldAnimPlayer;
struct FieldPointTo;
struct FieldGatewayRecord;
struct FieldLightPosition;
struct GfxLightVertexRecord;
struct FieldModel;
struct FieldModelAlphaGroups;
struct FieldModelFlagTable;
struct FieldPaletteRect;
struct FieldPlacement;
struct FieldVec16;
struct FieldVec3i;
struct FieldPointLight;
struct FieldRotationMatrix;
struct GfxFloat4;
struct PolygonPlacement;
struct FieldModelLightCache;
struct FieldActorState;
struct FieldState;
struct FieldLine;
struct FieldEntity;
struct PolygonSkeletonLoadSpec;
struct PsxSVectorZ32;
struct FieldBatchNode;
struct FieldCurveStepRecord;
struct FieldLightSource;
struct FieldModelCommandRecord;
struct FieldModelInstance;
struct FieldRuntimeFlags;
struct FieldScriptLineTrigger;
struct FieldSlideNormal;
struct GfxTexHeader;
struct AnimPlayerInstance;
struct FieldCameraClamp;
struct GfxLightPolygonSet;
struct FieldLineBatchRect;
struct FieldMatrix4x4;
struct FieldModelEffectPacket;
struct FieldPolygonRampRecord;
struct FieldQuadRecord;
struct FieldScreenShake;
struct FieldTexturedAllocRequest;
struct FieldTexturedSlots;
struct FieldVertexColor;
struct GfxPolygonSet;
struct PolygonAnimPlayer;
struct FieldModelSortRecord;
struct PolygonSkelHierarchy;
struct FieldScriptHeader;
struct FieldFxParams;
struct FieldPaletteGridCell;
struct FieldUploadDescriptor;
struct FieldPalettePoolRecord;
struct FieldTileGroup;
struct FieldTileDrawGroup;
struct FieldBoundsRect;
struct FieldTileSpriteDepthTile;
struct FieldTriggerCrossingPoint;
struct FieldEffectPosition;
struct FieldEntityWalkAim;
struct FieldFixedPosition;
struct FieldGatewayBox;
struct FieldPaletteEntryBgra;
struct FieldScriptTriggerLine;
struct PfileLoadOptions;
struct FieldLayer2StateE14;
struct FieldLayer4StateE9c;
struct FieldCameraScrollE70;
struct FieldModelSlot;
struct FieldLayer1StateE58;
struct FieldLayer4StateE8c;
struct FieldLayer2Tile;
struct FieldLayer4StateE98;
struct FieldRequest;
struct FieldCameraData;
struct GfxHomogeneousVec4;
struct GfxMatrix4x4;
struct FieldHumanAnimState;
struct FieldLayer2StateE0c;
struct FieldDoorTriggerRecord;
struct FieldTriggerRecord;
struct FieldGateway;
struct FieldPackedPaletteRows;

struct AdPalWordPair;

/* ad_app.cpp */
extern int g_FieldDisplayModeDirty;
extern int g_FieldEnableRendering;
extern int g_FieldLoadAssets;
extern int g_FieldAnimPausedEntity;
extern int g_FieldMapList_ClearLightFlag8[];
extern int g_FieldMapList_AnimPausedEntity[];
extern int g_FieldMapList_LockedSkipsLines[];
extern int g_FieldMapList_NearPlane512[];
void __cdecl Field_SetViewportRect(int viewportX, int viewportY, int viewportWidth, int viewportHeight);
void __cdecl Field_SetDisplayMode(int dataReady, int displayMode);
void __cdecl Field_FreeModelRecord(struct FieldModelDescriptor *record, void *ctx);
void *__cdecl Field_AllocModelRecord(void);
void __cdecl Field_ApplyMapConfig(struct GfxEngineContext *ctx);
int __cdecl Field_InitMapRuntime(struct GfxEngineContext *ctx);
void __cdecl Field_LoadSplashTextures(struct GfxEngineContext *ctx);
void __cdecl Field_FreeSplashTextures(void);
void __cdecl Field_InitModuleResources(struct GfxEngineContext *ctx, char *dir, char *objArg);
void __cdecl Field_FreeModuleResources(void);
void __cdecl Field_ModuleEnter(struct GfxEngineContext *ctx);
void __cdecl Field_ShutdownModule(struct GfxEngineContext *ctx);
void __cdecl Field_ReturnToDispatcher(struct GfxEngineContext *ctx, int shutdown);
void __cdecl Field_AbortToQuit(struct GfxEngineContext *ctx, void *message);
void __cdecl Field_ResetFrameCounter(void);
void __cdecl Field_ModuleFrame(struct GfxEngineContext *ctx);
int __cdecl Field_ModuleCatchUpFrame(struct GfxEngineContext *ctx);
void __cdecl Field_ModuleMouse(int message, int wparam, unsigned int lparam, struct GfxEngineContext *ctx);
void __cdecl Field_ModuleKeyboard(void);
void __cdecl Field_InitEngineContext(int context, int config);
void __cdecl Field_RenderFrameNoOp(struct GfxEngineContext *context);
void __cdecl Field_InitMapConfigTable(void);
int __cdecl Field_LoadFieldSizeTable(void);

/* ad_bk.cpp */
extern struct FieldLayer4StateE8c g_FieldLayer2StateDf8;
extern unsigned short g_FieldLayer2TilesNum;
extern struct FieldLayer2Tile *g_FieldLayer2Tiles;
extern int g_FieldLayer2ParamE04;
extern struct FieldLayer2StateE0c g_FieldLayer2StateE0c;
extern int g_FieldLayer2ParamE18;
extern struct FieldLayer4StateE8c g_FieldLayer3StateE24;
extern unsigned short g_FieldLayer3TilesNum;
extern struct FieldLayer2StateE14 g_FieldLayer3StateE30;
extern unsigned short g_FieldLayer3StateE38;
extern int g_FieldDoDrawLayer3;
extern int g_FieldLayer3Flag;
extern int g_FieldLayer1StateE48;
extern struct FieldLayer1StateE58 g_FieldLayer1StateE58;
extern unsigned short g_FieldLayer1TilesNum;
extern struct FieldCameraScrollE70 g_FieldCameraScrollE70;
extern int g_FieldCameraScrollE74;
extern int g_FieldCameraScrollE80;
extern struct FieldLayer4StateE8c g_FieldLayer4StateE8c;
extern unsigned short g_FieldLayer4TilesNum;
extern struct FieldLayer4StateE98 g_FieldLayer4StateE98;
extern struct FieldLayer4StateE9c g_FieldLayer4StateE9c;
extern unsigned short g_FieldLayer4StateEa0;
extern int g_FieldDoDrawLayer4;
extern int g_FieldLayer4Flag;
extern int g_FieldCameraScrollEcc;
void __cdecl Field_ResetBackgroundLayers(void);
void __cdecl Field_FreeTileRecordArray(void);
void __cdecl Field_FreeBackgroundLayersAndTileRecords(void);
void __cdecl Field_FreeBackgroundLayers(void);
void __cdecl Field_BuildModelDrawOrder(void);
int __cdecl Field_CompareDrawOrderDepthDescending(void *firstPair, void *secondPair);
void __cdecl Field_MapResetHookNoOp(void);
int __cdecl Field_InterpolateLinear(int start, int end, int duration, int elapsed);
int __cdecl Field_InterpolateCosine(int value, int end, int duration, int elapsed);
int __cdecl Field_ProjectWorldToScreen(struct PsxSVectorZ32 *point, void *screenPos);
void __cdecl Field_UpdateScreenShakeAxis(struct FieldScreenShake *shake);
void __cdecl Field_BeginCameraTransitionFromRequest(void);
void __cdecl Field_ClampCameraToDiagonal(struct FieldCameraClamp *clamp, struct FieldCameraCoordsRect *camera);
void __cdecl Field_ClampCameraToRect(struct FieldCameraCoordsRect *camera);
void __cdecl Field_ResetScrollAccelerationCounters(void);
void __cdecl Field_AccelerateScrollPositions(short *scrollX, short *scrollY, short *scrollX2, short *scrollY2);
void __cdecl Field_ProjectCameraTargetEntityToScreen(struct FieldCameraCoordsRect *screenPos);
void __cdecl Field_StepCameraScrollTransition(void);
void __cdecl Field_UpdateCameraScroll(void);

/* ad_cdr.cpp */
void __cdecl Field_DecompressLzs(unsigned char *src, unsigned char *dst);
extern void * g_pFieldFileBuffer;
extern int g_FieldFileUncompressedSize;
extern int g_FieldFileLoadPending;
extern int g_FieldFileLoadParam;
int __cdecl Field_LoadFieldFile(char *path);
void *__cdecl Field_GetSectionPtr(int section);
void __cdecl Field_FreeFlevelBuffer(void);

/* ad_data.cpp */
extern unsigned char g_FieldTileDepthThreshold[4];
struct FieldMapRecord;
extern struct FieldMapRecord g_FieldMapRecords[1200];
extern int g_FieldDataReady;
extern int g_FieldDisplayMode;
extern int g_FieldViewportX;
extern int g_FieldViewportY;
extern int g_FieldViewportWidth;
extern int g_FieldViewportHeight;
extern int g_FieldDisplayScaleFactor;
extern int g_FieldDisplayCenterX;
extern int g_FieldDisplayCenterY;
extern int g_FieldDisplayHalfCenterX;
extern int g_FieldDisplayHalfCenterY;
extern int g_FieldDisplayOffsetX;
extern int g_FieldDisplayHeight;
extern int g_FieldDisplayClipWidth;
extern int g_FieldDisplayClipHeight;
extern float g_FieldDisplayCenterXFloat;
extern float g_FieldDisplayCenterYFloat;
extern float g_FieldDisplayScaleX;
extern float g_FieldDisplayScaleY;
extern float g_FieldDisplayAspectScale;
extern unsigned char g_FieldHandTimName[11];
extern int g_FieldBgOriginOffsetY;
extern int g_FieldModuleEntered;
extern int g_FieldReloadPending;
extern int g_FieldClearModelLightFlag8;
extern char g_FieldLockedSkipsLines;
extern int g_FieldFrameReloadMode;
extern int g_FieldResumingFromBattle;
extern int g_FieldMapChangePending;
extern int g_FieldBgLayersDirty;
extern int g_FieldHoldAnimAfterWait;
extern int g_FieldSplashHeightBias;
extern int g_FieldMusicIdAlt;
extern short g_FieldCameraMatrixWork[15];
extern unsigned char g_FieldCameraTransform[30];
extern struct FieldLayerDrawOrder *g_pFieldLayer1DrawOrder;
extern struct FieldLayerDrawOrder *g_pFieldLayer2DrawOrder;
extern struct FieldLayerDrawOrder *g_pFieldLayer4DrawOrder;
extern struct FieldLayerDrawOrder *g_pFieldLayer3DrawOrder;
extern struct FieldTileGroup *g_pFieldTileGroupBuffer;
extern short g_FieldTileGroupIndex;
extern int g_FieldFieldFileLoaded;
extern void * g_pFieldWalkmesh;
extern struct FieldCameraData *g_pFieldSection1;
extern void * g_pFieldPalette;
extern struct FieldBoundsRect *g_pFieldScrollBounds;
extern struct FieldCameraData *g_pFieldCameraData;
extern struct FieldEncounterEntry *g_pFieldEncountersAlias;
extern struct FieldTileDrawOrder *g_pFieldTileDrawOrder;
extern unsigned int g_FieldMapCount;
extern unsigned short g_FieldCurrentMapId;
extern int g_FieldMinMapId;
extern int g_FieldMaxMapId;
int __cdecl Field_AllocTileBuffers(void);
void __cdecl Field_FreeTileBuffers(void);
void __cdecl Field_ScanMapIdRangeAlt(void);
void __cdecl Field_ScanMapIdRange(void);
int __cdecl Field_IsMapIdInRange(int mapId);
void __cdecl Field_FreeBackgroundIndexArrays(void);
void __cdecl Field_ComposeBackgroundFromTileMapThunk(void);
void __cdecl Field_SetDataDir(char *dir);
int __cdecl Field_LoadMapListFromFile(void);
int __cdecl Field_LoadMapListFromLgp(void);
void __cdecl Field_FreeAdDataBuffer(void);
int __cdecl Field_GetMapNameById(unsigned short mapId, char *dst);
int __cdecl Field_ParsePaletteSection(void);
int __cdecl Field_LoadTextFile(char *name);
int __cdecl Field_ParseCameraSection(void);
int __cdecl Field_ParseEncounterSection(void);
int __cdecl Field_ParseTileMapSection(void);
int __cdecl Field_ParseTriggersSection(void);
int __cdecl Field_ParseWalkmeshSection(void);
void __cdecl Field_ReleaseFieldFileBuffers(void);
int __cdecl Field_ParseAllSections(void);
int __cdecl Field_ParseAllSectionsByMapIndex(void);
void __cdecl Field_FindMapIndexByName(char *name);
int __cdecl Field_DebugDumpTileSprites(void);
int __cdecl Field_DebugDumpBgLayer1(void);
int __cdecl Field_DebugDumpBgLayer2(void);
int __cdecl Field_DebugDumpBgLayer3(void);
int __cdecl Field_DebugDumpBgLayer4(void);
int __cdecl Field_AddTileDrawGroup(struct FieldTileDrawGroup *groups, unsigned short index, short at02, short at04, int at08, int at0c, short at00, int at10);
int __cdecl Field_RasterizeTileSpritesToImage(unsigned short *count, unsigned char *pixels0, unsigned char *pixels1);
void __cdecl Field_BuildTileDrawOrder(struct FieldLayerDrawOrder *order, int count, void *tiles, int lastLayer);
unsigned char __cdecl Field_RemapTransparentIndex(unsigned char value, int pal);
float __cdecl Field_ComputeTileDepthZ(void *camera, int depth, int x, int y);
void __cdecl Field_DebugDumpTexturePage(void);
int __cdecl Field_DebugDumpFieldState(void);
int __cdecl Field_BgReadU16(int file, unsigned short *out);
void __cdecl Field_BgStreamReadU16(unsigned short *out);
int __cdecl Field_BgReadU8(int file, unsigned char *out);
void __cdecl Field_BgStreamReadU8(unsigned char *out);
int __cdecl Field_BgReadU32(int file, unsigned int *out);
void __cdecl Field_BgStreamReadU32(unsigned int *out);
int __cdecl Field_BgReadBytes(int file, void *dst, int size);
void __cdecl Field_BgStreamReadBytes(void *dst, int size);
void __cdecl Field_BgSkipBytes(int file, int count);
void __cdecl Field_BgStreamSkipBytes(int count);
int __cdecl Field_ParseBackgroundSection(void);

/* ad_ddraw.cpp */
extern int g_pFieldBackSurface;
extern int g_FieldDDrawSurfaceDd4;
extern int g_FieldDDrawSurfaceDd8;
extern unsigned short g_FieldSurfaceDestX;
extern int g_pFieldBlitSourceSurface;
int __cdecl Field_UnusedReturnOne_2(void);
void __cdecl Field_CacheDirectDrawSurfaceHandles(struct GfxEngineContext *context);
int __cdecl Field_InitGraphicsDeviceAndCacheSurfaces(void);
void __cdecl Field_ReleaseGraphicsNoOp(void);
int __stdcall Field_DefaultWindowProc(void *hWnd, unsigned int msg, unsigned int wParam, int lParam);
int __cdecl Field_SetDisplayModeAndCreateSurfaces(int width, int height, int bpp, char *errorMsg);
int __cdecl Field_UnusedReturnOne_3(void);
int __cdecl Field_ReadSurfacePixelFormat(struct GfxEngineContext *context);
int __cdecl Field_PresentEngineFrame(void);
int __cdecl Field_UploadImageToBackSurface(void *descriptors, unsigned short selector);
int __cdecl Field_UploadImageToBackSurfaceNoSourceOffset(struct FieldUploadDescriptor *descriptors, unsigned short selector);
void __cdecl Field_ClearBackSurfaceBltFillOrLock(void);
void __cdecl Field_ClearBackSurface(void);
int __cdecl Field_AllocSurfaceBuffer16(int width, int height);
void __cdecl Field_ClearBackSurfaceTopAndBottomBands(void);
void __cdecl Field_CopyTexturePageToPrimarySurface(int page);

/* ad_human.cpp */
void __cdecl Field_AllocHumanBuffers(void);
void __cdecl Field_FreeHumanBuffers(void);

/* ad_image.cpp */
extern int g_FieldHandImageType;
extern short g_FieldPackedColorOrgX1;
extern short g_FieldPackedColorCount1;
extern short g_FieldPackedColorOrgX2;
extern short g_FieldPackedColorPixelWidth2;
extern unsigned short *g_pFieldTimPaletteData;
extern short g_FieldTimPaletteOrgX;
extern short g_FieldTimPaletteOrgY;
extern int g_pFieldTimPixelData1;
extern short g_FieldTimPixelWidth1;
extern short g_FieldTimImage1Width;
extern int g_pFieldTimPixelData2;
extern int g_FieldTimPixelSize2;
extern short g_FieldTimPixelWidth2;
extern short g_FieldTimImage2Width;
extern short g_FieldPaletteCurrentOrgX;
extern short g_FieldPaletteCurrentOrgY;
extern short g_FieldPaletteTargetOrgX;
extern short g_FieldPaletteTargetOrgY;
void __cdecl Field_ClearAdjacentPreloadReady(void);
int __cdecl Field_ClampBackgroundPositionToBounds(short *xPtr, short *yPtr);
int __cdecl Field_ComputeBackgroundViewportClip(short *xPtr, short *yPtr, unsigned short *offsetXOut, unsigned short *offsetYOut, unsigned short *extentXOut, unsigned short *extentYOut);
int __cdecl Field_WriteTileSamplesToSurface(int surface, short centerX, short centerY, int width, int height, int scale);
int __cdecl Field_WriteTileSamplesToBuffer(int buffer, int tileIndex, short originX, short originY, int width, int height, int scale, int pitch);
void __cdecl Field_ClipBackgroundRecordGroup1ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent);
void __cdecl Field_ClipBackgroundRecordGroup2ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent);
void __cdecl Field_ClipBackgroundRecordGroup3ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent);
int __cdecl Field_BltBackgroundLayer3Entries(short centerX, short centerY, int nativeScale);
int __cdecl Field_BltBackgroundLayer4Entries(short centerX, short centerY, int nativeScale);
int __cdecl Field_CompositeBackgroundFrame(short scrollX, short scrollY);
void __cdecl Field_DebugWriteFplFile(void);
int __cdecl Field_UploadPalette(int blockPtr);
int __cdecl Field_ClassifyPaletteFormat(unsigned short colorCount, unsigned short sizeHigh);
int __cdecl Field_ParsePaletteImageBlocks(int blockPtr);
int __cdecl Field_ParseBackgroundHeaders(void *blockPtr);
int __cdecl Field_ExtractImageFormatBits(int flags);
int __cdecl Field_SetImageUploadedAndUploadTimHeader1(void);
void __cdecl Field_SortTileSpritesByDepth(void);
int __cdecl Field_CompareTileSpriteDepth(struct FieldTileSpriteDepthTile *tilePtr, struct FieldTileSpriteDepthTile *otherTile);
int __cdecl Field_ColorFillSurfaceRect(int left, int top, int width, int height);
void __cdecl Field_FillFirstActiveWindowRect(void);
int __cdecl Field_ParsePaletteBlockPair(void *blockPtr);
void __cdecl Field_LoadHandCursorImage(struct GfxEngineContext *context, struct PfileLoadOptions *opts);
void __cdecl Field_BlitTile32At(short x, short y);
int __cdecl Field_IsPointClearOfModels(short screenX, short screenY);

/* ad_list.cpp */
int __cdecl Field_AllocAdListBuffer(void);
void __cdecl Field_FreeAdListBuffer(void);
void __cdecl Field_ResetAdListBankCounts(void);
void __cdecl Field_EmitBackgroundTile(float offsetX, float offsetY, float depthBits, float attrB, float attrC, int attrD, int bank);
void __cdecl Field_BuildBackgroundQuads(void);

/* ad_obj.cpp */
extern unsigned char g_FieldMapModelConfigTable[3148];
extern char *g_pFieldModelDefaultCharDir;
int __cdecl Field_AllocModelDefs(int count);
void __cdecl Field_FreeAdObjBuffer(void);
void __cdecl Field_SetModelDataDir(char *name);
void __cdecl Field_WriteRenameScriptFromNameLists(void);
int __cdecl Field_ParseModelListFile(char *source);
int __cdecl Field_ModelReadU16(int file, unsigned short *out);
void __cdecl Field_ModelStreamReadU16(unsigned short *out);
int __cdecl Field_ModelReadU8(int file, unsigned char *out);
void __cdecl Field_ModelStreamReadU8(unsigned char *out);
int __cdecl Field_ModelReadU32(int file, unsigned int *out);
void __cdecl Field_ModelStreamReadU32(unsigned int *out);
int __cdecl Field_ModelReadBytes(int file, void *dst, int size);
void __cdecl Field_ModelStreamReadBytes(void *dst, int size);
int __cdecl Field_ParseModelSection(void *source);
void __cdecl Field_ResolveModelFilePath(char *name);
void __cdecl Field_FreeModels(void);
int __cdecl Field_GetPartyIdFromHrcName(int name);

/* ad_pal.cpp */
extern struct FieldPackedPaletteRows g_FieldPackedPaletteRows;
extern struct FieldPackedPaletteRows g_FieldPackedPaletteOffset;
extern int g_FieldPaletteFormatFlag;
extern struct FieldPaletteGridCell *g_FieldPaletteGridBuffer;
extern unsigned short g_FieldPaletteGridHeight;
extern struct AdPalWordPair g_AdPalWords;
extern unsigned char g_FieldPaletteFlags[20];
extern struct FieldPalettePoolRecord *g_FieldPalettePoolBuffer;
void __cdecl Field_AllocPalettePool(int count);
void __cdecl Field_FreeAdPalBuffer(void);
int __cdecl Field_BuildPaletteRecords(unsigned short width, unsigned short height, unsigned short *source, int mode);
int __cdecl Field_RefreshPaletteRectEntries(int *rect, unsigned short *source, int mode);
int __cdecl Field_BuildPackedColorRecords(unsigned short columns, unsigned short rows, unsigned short *srcPalette, int mode);
void __cdecl Field_SelectPaletteRowIfInRange(unsigned short value);
void __cdecl Field_FreePaletteWorkBuffers(void);
int __cdecl Field_BuildBrightnessTables(void);
void __cdecl Field_FreeBrightnessTables(void);
short __cdecl Field_LookupBrightnessPaletteEntry(short color, short delta, int mode, int rawDelta);
short __cdecl Field_LookupScaledPaletteValue(short color, int mode);
void __cdecl Field_ReadPaletteRect(struct FieldPaletteRect *rect, void *dst);
void __cdecl Field_WritePaletteRect(struct FieldPaletteRect *range, void *src);
int __cdecl Field_ConvertPaletteRowToBgra(struct FieldPaletteRect *range, unsigned short *palette, struct FieldPaletteEntryBgra *dst, short row);
void __cdecl Field_ClearPaletteGridDirtyFlags(void);
void __cdecl Field_PromotePaletteDirtyFlag(void);
void __cdecl Field_ClearPalettePendingFlag(void);
int __cdecl Field_GetPalettePendingFlag(void);

/* ad_tile.cpp */
extern struct GfxPixelFormat g_FieldBrightnessLutFormat;
extern int g_FieldTileColorRed;
extern int g_FieldTileColorGreen;
extern int g_FieldTileColorBlue;
extern int g_FieldTileStateDac;
extern int g_FieldTileDumpPaletteFlag;
extern int g_FieldTileStateDbc;
extern unsigned short g_FieldTileStateDc0;
extern int g_FieldTileStateDc4;
int __cdecl Gfx_ConvertRgb565WordToRgb555(int color);
void __cdecl Field_ConvertTexturePagePixelsRgb565To555(void);
int __cdecl Field_AllocTileRecords(void);
void __cdecl Field_FreeTileRecords(void);
void __cdecl Field_FreeTileTextures(void);
int __cdecl Field_DumpPaletteTextFile(int count);
void __cdecl Field_InitTileTextureFormat(struct GfxTexHeader *tex);
void __cdecl Field_InitTileIndexedTexture(struct GfxTexHeader *tex);

/* field_model_vertex_lighting.cpp */
extern int g_FieldModelVertexLimit;
void __cdecl Field_AppendModelPolygonSetRecord(struct FieldPolygonSetCountArray *recordArray, struct GfxPolygonSet *model);
void __cdecl Field_TraverseModelFlagRecord(struct FieldPolygonSetCountArray *flagRecord, struct GfxPolygonSet *flag);
void __cdecl Field_WriteTrueFlag(int context, struct GfxPolygonSet *flag);
void __cdecl Field_StoreZeroFlagCallback(int context, struct GfxPolygonSet *flag);
void __cdecl Field_ApplyPointLightToModelVertices(struct FieldPointLight *light, struct GfxPolygonSet *polySet);
void __cdecl Field_LightPolygonSetCallbackSwapped(struct GfxLightPolygonSet *context, struct GfxPolygonSet *polySet);
void __cdecl Field_AddColorToModelVertexColors(int color, struct FieldModelAlphaGroups *model);
void __cdecl Field_AddColorToPolygonSetVertexColors(int color, struct GfxPolygonSet *polySet);
void __cdecl Field_AddColorBytesSaturated(unsigned char *add, unsigned char *color);
void __cdecl Field_AddColorBiasToModelVertexColors(int blueBias, int greenBias, int redBias, struct FieldModelAlphaGroups *model);
void __cdecl Field_AddColorBiasToPolygonSetVertexColors(short blueBias, short greenBias, short redBias, struct GfxPolygonSet *polySet);
void __cdecl Field_AddColorBiasToVertexColor(short blueBias, short greenBias, short redBias, struct FieldVertexColor *vertexColor);
void __cdecl Field_SetModelVertexAlpha192(struct FieldModelAlphaGroups *model, struct GfxEngineContext *context);
void __cdecl Field_SetPolygonSetVertexAlpha192(struct GfxPolygonSet *polySet);
void __cdecl Field_ApplyModelDiffuseSelectorEffect(struct FieldModelInstance *model, short red, short green, short blue, short scale, int flags);
void __cdecl Field_ApplyPolygonSetLightingGatedCallback(struct GfxLightPolygonSet *context, struct GfxPolygonSet *polySet);
void __cdecl Field_ApplyPolygonSetLightingGated(struct GfxPolygonSet *polySet, struct GfxLightPolygonSet *lightSet);
void __cdecl Field_ApplyPolygonSetLightingUngatedCallback(struct GfxLightPolygonSet *lightSetPtr, struct GfxPolygonSet *polySet);
void __cdecl Field_ApplyPolygonSetLightingUngated(struct GfxPolygonSet *polySet, struct GfxLightPolygonSet *lightSet);
int __cdecl Field_AllocTexturedHundredSlots(struct FieldTexturedAllocRequest *request, struct FieldTexturedSlots *slots);
void __cdecl Field_FreeTexturedHundredSlots(struct FieldTexturedSlots *slots);
void __cdecl Field_ClearHeadMeshFlag(struct FieldTexturedSlots *slots);

/* field_apply_color_deltas.cpp */
int __cdecl Field_ApplyColorDeltaToContainer(struct PolygonModel *container, short red, short green, short blue);
void __cdecl Field_ApplyColorDeltaToPolygonSet(short red, short green, short blue, struct GfxPolygonSet *polySet);
void __cdecl Field_AddBgraColorSaturated(int redLane, int greenLane, int blueLane, int srcPtr, int dstPtr);

/* field_head_mesh_commands.cpp */
int __cdecl Field_ApplyHeadMeshCommands(struct FieldActorRecord *actor, unsigned char *commands);

/* field_indexed_record_command.cpp */
extern void * g_pFieldSplashTexture[4];
int __cdecl Field_ApplyModelSplashCommand(struct FieldModelInstance *modelRecord, struct FieldModelCommandRecord *commandRecord);

/* field_model_effect_packet.cpp */
int __cdecl Field_ApplyModelEffectPacket(struct FieldModelInstance *modelRecord, struct FieldModelEffectPacket *effectPacket);

/* field_model_record_overrides.cpp */
int __cdecl Field_ApplyModelRecordOverrides(struct FieldModelInstance *modelRecord, struct FieldModelFlagTable *flagTable, struct FieldModelCommandRecord *overrideRecord);

/* field_model_matrix_packet.cpp */
int __cdecl Field_ApplyModelMatrixPacket(struct PolygonModel *lightContext, struct FieldModelCommandRecord *matrixPacket);

/* field_model_delta_packet.cpp */
int __cdecl Field_ApplyModelDeltaPacket(struct PolygonModel *container, struct FieldModelCommandRecord *deltaPacket);
int __cdecl Field_ApplyIndexedModelMatrixRecord(struct PolygonModel *context, unsigned char *packet);

/* field_polygon_frame_animation.cpp */
extern int g_FieldCurveStepFlags;
#define g_FieldCurveStepFlagTable (&g_FieldCurveStepFlags)
int __cdecl Field_ApplyPolygonRecordFrame(struct FieldModelInstance *modelRecord, struct FieldPolygonRampRecord *rampRecord, struct FieldModelCommandRecord *commandRecord, int slot, int mode, struct FieldCurveStepRecord *curveStep);

/* field_request_and_sound.cpp */
extern struct FieldRequest g_FieldRequestBuffer;
extern int g_FieldFxBuffer;
void __cdecl Field_BindRequestBlock(void);
void __cdecl Field_BindFxBlock(void);
void __cdecl Field_UnusedNoOp_6(void);
void __cdecl Field_UnusedNoOp_7(void);
void __cdecl Field_PostSoundCommandF0(void);
void __cdecl Field_PlayDoorTriggerSoundEffect(unsigned short soundId);

/* ad_human.cpp */
extern struct FieldModelSlot *g_pFieldModelSlots;
extern struct FieldHumanAnimState g_FieldHumanAnimState;
extern unsigned short g_FieldHumanAnimFlag;
extern struct FieldHumanActorBuffer0 *g_pHumanActorBuffer0;
extern struct FieldHumanActorBuffer0 *g_pHumanActorBuffer1;
extern struct FieldHumanActorBuffer0 *g_pHumanActorBuffer2;
extern struct FieldHumanActorBuffer0 *g_pHumanActorBuffer3;
void __cdecl Field_ClearHumanBufferRecords(void);
void __cdecl Field_PlacePlayerOnWalkmesh(void);
void __cdecl Field_UnusedReadWalkmeshHeader(void);
void __cdecl Field_UnusedNoOp_5(void);
void __cdecl Field_SetPlayerHeadingOffsetFromInput(int unused, short entityIndex);
void __cdecl Field_AdvanceEntityAnimation(short entityIndex);
void __cdecl Field_UpdateEntities(int input);
void __cdecl Field_TriggerGatewayExit(struct FieldGatewayRecord *gateway);
void __cdecl Field_FlagNearestFacedEntityOnInputEdge(void);
short __cdecl Field_SineByteAngle(int angle);
int __cdecl Field_AimEntityAtWalkTarget(struct FieldEntityWalkAim *aim, short radiusBias);
int __cdecl Field_WalkmeshSlideToPoint(unsigned short *triangle, struct FieldVec3i *position, struct FieldSlideNormal *normal, struct FieldVec3i *blockEdge);
int __cdecl Field_StepEntityOnWalkmesh(short entityIndex);
int __cdecl Field_TestEntityCollision(short entityIndex, struct FieldTriggerCrossingPoint *position);
int __cdecl Field_ClosestPointOnSegment(short *line, struct FieldTriggerCrossingPoint *point, struct FieldTriggerCrossingPoint *closest);
char __cdecl Field_TestScriptLineTriggersWithCrossing(struct FieldScriptTriggerLine *triggerLine, struct FieldLine *segments, struct FieldFixedPosition *movedPos);
void __cdecl Field_TestScriptLineTriggers(struct FieldScriptLineTrigger *triggerLine, int segments);
void __cdecl Field_ResetScriptLineTalkFlags(int records);
void __cdecl Field_TestGateways(struct FieldEntity *entity, struct FieldGateway *gateways, struct FieldPointTo *movedPos);
short __cdecl Field_SetDoorTriggerBit(struct FieldDoorTriggerRecord *doorTrigger, int mode);
void __cdecl Field_TestDoorTriggers(struct FieldEntity *entity, struct FieldDoorTriggerRecord *doors, struct FieldPointTo *movedPos);
void __cdecl Field_InitDoorTriggers(int doors);

/* ad_pal.cpp */
short __cdecl Field_ConvertPixelWordMode(unsigned short pixel, int mode);
unsigned short __cdecl Field_DecodePixelWordMode(unsigned short pixel, int mode, void *red, void *comp1, void *comp2);

/* field_engine_model_anim.cpp */
extern unsigned short g_FieldModelSortCount;
extern double g_FieldFrameTimeDelta;
extern unsigned char g_FieldModelRenderState[64];
extern int g_FieldModelOffsetZ[18];
extern int g_FieldModelOffsetY[18];
extern struct GfxFloat4 g_FieldModelContextMatrix[4];
extern int g_FieldModelVisible[18];
extern int g_FieldMovieCameraActive;
extern int g_FieldFrameCounter;
extern int g_FieldRenderActive;
extern int g_FieldRenderStateMode;
void __cdecl Field_SetEngineContextThunk(int context);
void __cdecl Field_GetEngineContextThunk(void);
void __cdecl Field_RequestEngineQuitThunk(struct GfxEngineContext *exitCode);
void __cdecl Field_DestroyEngineContextThunk(void);
void __cdecl Field_ThrottleFrameRate(struct GfxEngineContext *unused);
void __cdecl Field_ThrottleFrameRateCapped(struct GfxEngineContext *unused);
void __cdecl Field_RenderFrameAndResetPools(struct GfxEngineContext *context);
void __cdecl Field_RenderFrameUnused(struct GfxEngineContext *context);
void __cdecl Field_RenderFrame(struct GfxEngineContext *context);
void __cdecl Field_SeedRandomFromContext(struct GfxEngineContext *context);
void __cdecl Field_CreateEngineContextThunk(void);
void __cdecl Field_InitEngineContextThunk(int context, int config);
int __cdecl Field_ApplyScaledOffsetToPlacement(struct GfxEngineContext *context, struct FieldPlacement *placement, int offsetX, int offsetY, int offsetZ, int applyMatrix, float scale);
int __cdecl Field_SetModelUniformScale(struct PolygonPlacement *placement, int scale);
int __cdecl Field_SetPlacementAngleYWrapped(int placement, float angleDegrees);
int __cdecl Field_SetAnimPlayerPauseFlag(struct PolygonAnimPlayer *animPlayer, int pauseFlag);
int __cdecl Field_SetAnimPlayerFrame(struct FieldAnimPlayer *animPlayer, int frameIndex);
int __cdecl Field_SetContextMatrixFromStackArgs(struct GfxEngineContext *context, int matrix);
int __cdecl Field_SelectAnimPlayerAnimationIndex(struct AnimPlayerInstance *player, int animIndex, int slot);
int __cdecl Field_SetModelAnimation(struct FieldModel *model, int animIndex);
void __cdecl Field_ReleaseBatchesAndResetDrawPools(struct GfxEngineContext *pool);
int __cdecl Field_LoadEntitySkeletonIfAbsent(struct GfxEngineContext *context, struct PolygonSkeletonLoadSpec *entityLoad, int blendMode);
int __cdecl Field_LoadSkeleton(int name, int context, int loader, struct GfxEngineContext *engine);
int __cdecl Field_CreateAnimPlayerWithAnimation(int context, struct PolygonSkelHierarchy *skeleton, struct GfxEngineContext *engine, int animIndex);
int __cdecl Field_LoadAnimation(int loader, int fileName);
void __cdecl Field_BindAnimPlayerAnimationThunk(int mode, int animIndex, struct PolygonAnimPlayer *player);
int __cdecl Field_LoadSkeletonAndCreateAnimPlayer(int context, int animIndex, int loader, struct GfxEngineContext *engine, int name, int skeletonOwner);
void __cdecl Field_SwapMatrixYZAxes(struct FieldMatrix4x4 *matrix);
void __cdecl Field_PsxMatrixToFloat4x4Thunk(int psxMatrix, struct GfxFloat4 *dst);
void __cdecl Field_TransformPointByMatrix(struct GfxMatrix4x4 *matrix, int x, int y, int z, struct GfxHomogeneousVec4 *out);
void __cdecl Field_BuildQuarterTurnAboutXMatrix(struct FieldRotationMatrix *matrix);
void __cdecl Field_BuildAxisSwappedRotationMatrix(int psxMatrix, void *dst);
void __cdecl Field_RebuildCameraMatrices(struct GfxEngineContext *context);
void __cdecl Field_SortModelsByDepth(void);
int __cdecl Field_CompareModelDepthDescending(struct FieldModelSortRecord *lhs, struct FieldModelSortRecord *rhs);
void __cdecl Field_SetModelInstanceTransformedPosition(struct FieldModelInstance *instance, int x, int y, int z);
int __cdecl Field_IsOnScreen(int x, int y);
void __cdecl Field_UpdateModelInstances(void);
int __cdecl Field_SelectRenderStateModeByMap(int mode, struct FieldEffectPosition *position);
void __cdecl Field_EmitBatchNodeTexturedQuad(struct FieldBatchNode *node, float x, float y, float z, float uOrigin, float vOrigin, float uSpan, float vSpan, int metaId, float width, float height, int mode);
void __cdecl Field_EmitBatchNodeTexturedQuadWithColor(struct GfxRenderObject *node, float x, float y, int z, float uOrigin, float vOrigin, float uSpan, float vSpan, int metaId, float width, float height, int color);
void __cdecl Field_EmitQuadFromRecord(struct FieldQuadRecord *record);
void __cdecl Field_UploadTexturePagePalettes(int context, int mode, struct BattleRgbaColor *slot, int value);
void __cdecl Field_UpdateMovieCamera(void);
void __cdecl Field_BltLayer2TileSpritesDoubled(int rangeEnd);
int __cdecl Field_RenderModels(struct GfxEngineContext *context);

/* field_execute_model.cpp */
int __cdecl Field_ExecuteModelCommand(int command, struct FieldModelInstance *modelRecord, struct FieldModelCommandRecord *commandRecord, int rampStep, int rampCount, int modelIndex, int *resultPtr);

/* field_gateway_arrows.cpp */
extern unsigned short g_FieldArrowAnimFrame;
void __cdecl Field_DrawGatewayArrows(void *matrix, struct FieldGatewayBox *gatewayBoxes);

/* field_line_draw.cpp */
void __cdecl Field_InitLineDrawTable(struct GfxEngineContext *ctx, struct PfileLoadOptions *opts);
void __cdecl Field_DestroyLineDrawRenderObject(void);
void __cdecl Field_FillLineBatchNodeVertices(struct FieldLineBatchRect *lineRect);
void __cdecl Field_DrawScriptLines(void *matrix);
void __cdecl Field_EmitLineDrawBatch(void);
void __cdecl Field_ReleaseLineDrawBatch(void);
void __cdecl Field_StepLineDrawRecords(void);

/* field_input_buttons.cpp */
extern struct FieldFxParams *g_pFieldFx;
void __cdecl Input_ClearButtonLatches(void);
int __cdecl Input_TestHeld(int buttonMask);
int __cdecl Input_TestPressed(int buttonMask);
int __cdecl Input_IsCtrlF2Down(void);
void __cdecl Input_UpdateButtonLatches(void);
int __cdecl Field_ReadInput(short *scrollX, short *scrollY);

/* ad_tile.cpp */
void __cdecl Field_InvalidateTexturePageBatches(void);
void __cdecl Field_DrawTexturePageRange(struct GfxEngineContext *context, int startIndex, int endIndex);
int __cdecl Field_CreateTexturePages(struct GfxEngineContext *ctx, struct PfileLoadOptions *opts);
int __cdecl Field_DrawBackgroundLayer2DoubledQuads(short cameraX, short cameraY);
int __cdecl Field_DrawBackgroundLayer2(short cameraX, short cameraY);
int __cdecl Field_DrawBackgroundLayer1(short cameraX, short cameraY);
int __cdecl Field_UnusedReturnOne_1(void);
void __cdecl Field_SyncProjectionOffsetToCamera(void);
int __cdecl Field_DrawBackgroundLayers(short cameraX, short cameraY);
int __cdecl Field_DrawBackgroundLayer3(short cameraX, short cameraY);
int __cdecl Field_DrawBackgroundLayer4(short viewX, short viewY);

/* field_map_script_state.cpp */
int __cdecl Field_LoadMapByName(char *name);
void __cdecl Script_UnusedNoOp(void);
void __cdecl Script_BeginMap(struct FieldEntity *entities);
void __cdecl Script_UpdateFrame(void);
void __cdecl Savemap_ResetForNewGame(void);
void __cdecl Script_ResetState(void);
void __cdecl Script_RunAllInitScripts(void);
void __cdecl Field_UnmapUnretainedEntityModels(void);
void __cdecl Script_ExecuteFrame(void);
void __cdecl Field_UpdateModelAnimStates(void);
int __cdecl Script_TriggerEntityScript(short entity, short priority, short script);
void __cdecl Field_InitHandCursor(struct GfxEngineContext *context, struct PfileLoadOptions *opts);
void __cdecl Field_UpdateHandCursor(void);
void __cdecl Field_ToggleHandCursorBlink(void);
void __cdecl Field_DrawHandCursor(void);

/* field_make_point.cpp */
void __cdecl Field_MakePoint(struct FieldVec3i *point, int x, int y, int z);

/* field_model_init_tables.cpp */
struct FieldTrigTable;
struct FieldIndexListB;
extern struct FieldTrigTable g_FieldTrigTable;
extern struct FieldIndexListB g_FieldIndexListB;
extern unsigned char g_FieldModelCommandFlags[4];
extern struct FieldRuntimeFlags g_FieldModelRuntimeFlags[15];
extern unsigned char g_FieldModelRuntimeRecords[30];
void __cdecl Field_InitModelRuntimeTables(void);

/* field_encounter_random.cpp */
extern char g_FieldDigitMapping[16];
int __cdecl Field_NextEncounterRandom(void);
unsigned char __cdecl Field_NextFormationRandom(void);
void __cdecl Field_CheckRandomEncounter(void);
void __cdecl Field_UpdateEncountersUnused(void);

/* field_model_ramp_delta.cpp */
int __cdecl Field_RampModelDeltaToTarget(struct PolygonModel *model, unsigned char *packet);

/* field_exit_map_requests.cpp */
extern unsigned char g_FieldEventCompleteMessage[32];
extern int g_FieldLogicUnusedB78;
extern int g_FieldLogicExitState;
extern int g_FieldTransitionPendingFlag;
extern int g_FieldLogicStateFlag;
void __cdecl Field_RequestExit(int exitRequest);
int __cdecl Field_GetExitRequest(void);
void __cdecl Field_BeginMapScripts(void);
void __cdecl Field_ClearCurrentMapIdAndPreloadState(void);
void __cdecl Field_CopyEntryMapIdWord(void);
void __cdecl Field_SetPartyEntityDrawFlagsOnMap1F6(void);
void __cdecl Field_ResetMapState(void);
int __cdecl Field_UpdateLogic(void);
void __cdecl Field_DebugLoadOnLineSav(void);

/* field_message_window.cpp */
void __cdecl Field_ResetMessageWindowsForField(void);
void __cdecl Field_SetMessageTableFromScriptHeader(void);
void __cdecl Field_WindowClose(short windowIndex);
int __cdecl Field_WindowIsClosed(short window);
void __cdecl Field_WindowSetMode(short window, short style, short preventClose);
void __cdecl Field_ResetMessageWindowState(void);
void __cdecl Field_WindowSetRectClamped(short windowIndex, short, short, short width, short height);
void __cdecl Field_WindowMove(short window, short dx, short dy);
void __cdecl Field_WindowSetHeight(short window, short height);
int __cdecl Field_WindowShowMessage(unsigned char windowIndex, unsigned char textIndex);
int __cdecl Field_WindowUpdateChoice(unsigned char windowIndex, unsigned char textIndex, unsigned char minChoice, unsigned char maxChoice, short *choicePtr);
void __cdecl Field_WindowPlayCursorSound(void);
int __cdecl Field_WindowBeginOpen(short windowIndex, short textIndex);
void __cdecl Field_WindowStepOpenGrow(short windowIndex);
void __cdecl Field_WindowStepTextReveal(short windowIndex);
void __cdecl Field_WindowStepRowScroll(short window);
void __cdecl Field_WindowStepPageScrollOut(short windowIndex);
void __cdecl Field_WindowClearText(short window);
int __cdecl Field_WindowStepCloseAnimation(short windowIndex);
unsigned short __cdecl Field_WindowReadReplacementValue(short windowIndex);
void __cdecl Field_FormatDecimalTextTrimmed(unsigned short number, unsigned char *dst);
void __cdecl Field_FormatDecimalTextPadded(unsigned short number, unsigned char *dst);
void __cdecl Field_FormatHexText(unsigned short number, unsigned char *dst);
int __cdecl Field_BuildLocationName(short id);
void __cdecl Field_SetCharacterNameFromDialogueString(short charIndex, short stringIndex);

/* field_run_polygon.cpp */
int __cdecl Field_RunPolygonRampCycle(struct FieldModelInstance *model, struct FieldPolygonRampRecord *ramp, int mode, int delay, struct FieldRuntimeFlags *flags);

/* field_script_operands.cpp */
extern short g_FieldWindowDelay[4];
extern struct FieldScriptHeader *g_FieldScripts;
extern short g_FieldWindowSpeed[4];
extern unsigned char g_FieldRequestPosted;
extern char g_FieldAudioDisabled;
extern unsigned char g_FieldScriptExecFlags[16];
extern short g_EngineGameMode;
extern unsigned char g_FieldEntityScriptTrack[8];
extern unsigned char g_FieldMovieRequested;
extern char g_FieldMoviePending;
extern unsigned char g_FieldEntityScriptTable[64];
extern unsigned char g_WindowReplaceBank[4][8];
extern short g_FieldWindowPos[4];
extern char g_FieldDialogueLock;
extern short g_FieldHandCursorY;
extern short g_FieldHandCursorX;
extern short g_FieldWindowRevealProgress[4];
extern short g_MenuEntryState;
extern char g_FieldLineDrawIntensity;
extern short g_FieldFollowerCoords[4];
extern char g_FieldPlayerCrossedLine;
extern unsigned char g_PartyStatsDirty;
extern short g_FieldLineCount;
extern unsigned char g_FieldScriptOpcode;
extern unsigned char g_FieldModelIndex;
extern unsigned char g_FieldModelCount;
extern unsigned short g_WindowReplaceBankAddr[4][8];
extern void *g_FieldDialogTextPtr;
extern int g_FieldDialogState;
extern unsigned short g_FieldScriptWait[48];
extern unsigned char g_FieldWindowOwner[4];
extern unsigned char g_CurrentEntity;
extern unsigned char g_FieldHandCursorState;
extern short g_FieldWorldMapResult;
extern unsigned char g_FieldModelAnimMode[16];
extern unsigned char g_FieldBlinkRandomIndex;
extern struct FieldCharEntityMap g_FieldCharEntity;
extern char g_FieldOpenWindowCount;
extern char g_FieldMoviePlaying;
extern unsigned short g_FieldScriptPC[52];
extern short g_EnginePreviousMode;
extern int g_WorldBattleRequest;
extern short g_FieldEntityParams[4];
extern struct FieldLineDrawScratch g_FieldLineDrawScratch[64];
extern short g_FieldEntryMapId;
extern short g_FieldPreloadMapId;
extern short g_FieldPreloadMapTarget;
extern short g_FieldPreloadState;
extern short g_FieldLayerScrollParam7;
extern short g_FieldBgFlag;
extern short g_FieldLayerScrollSteps;
extern short g_FieldLayerScrollStepIndex;
extern short g_FieldLayerScrollX;
extern short g_FieldLayerScrollY;
extern short g_FieldLayerScrollStartX;
extern short g_FieldLayerScrollStartY;
extern short g_FieldCameraScrollX;
extern short g_FieldCameraScrollY;
extern int g_FieldBgLayer1ScrollX;
extern int g_FieldBgLayer1ScrollY;
extern short g_FieldBgLayer2ScrollX;
extern short g_FieldBgLayer2ScrollY;
extern short g_FieldBgLayer3ScrollX;
extern short g_FieldBgLayer3ScrollY;
extern short g_FieldBgLayer4ScrollX;
extern short g_FieldBgLayer4ScrollY;
extern int g_FieldProjectedScreenPos;
extern short g_FieldPlayerEntityIndex;
extern short g_FieldWalkmeshEdge;
extern short g_FieldWalkmeshAxis;
extern short g_FieldLogicActive;
extern int g_FieldReturnMode;
extern int g_FieldNextModuleArg;
extern unsigned char g_FieldEncounterState;
extern unsigned char g_FieldRandomStepIndex;
extern short g_FieldLastFormationId;
extern unsigned char g_FieldEncounterTriggered;
extern char g_FieldRandomIndex1;
extern char g_FieldRandomIndex2;
extern unsigned char g_FieldEncounterStepCounter;
extern short g_FieldEncounterRateAccum;
extern float g_FieldCursorScaleX;
extern float g_FieldCursorScaleY;
unsigned char __cdecl Script_ReadOperand8(short nibbleSelector, short instrOffset);
void __cdecl Script_WriteOperand8(short nibbleSelector, short instrOffset, unsigned char value);
short __cdecl Script_ReadOperand16(short bank, short addr);
void __cdecl Script_WriteOperand16(short nibbleSelector, short instrOffset, short value);
int __cdecl Script_OpUnimplementedStall(void);
int __cdecl Script_OpYieldOneOpcode(void);
int __cdecl Script_OpWaitFrames(void);
int __cdecl Script_OpSetByte(void);
int __cdecl Script_OpSetWord(void);
int __cdecl Script_OpSetByteAlias(void);
int __cdecl Script_OpGetHighByte(void);
int __cdecl Script_OpSetWordFromBytes(void);
int __cdecl Script_OpStoreByteIndexed(void);
int __cdecl Script_OpLoadByteIndexed(void);
int __cdecl Script_OpSearchBankForByte(void);
int __cdecl Script_OpBitSet(void);
int __cdecl Script_OpBitClear(void);
int __cdecl Script_OpBitToggle(void);
int __cdecl Script_OpDefineLine(void);
int __cdecl Script_OpSetLinePoints(void);
int __cdecl Script_OpSetLineEnabled(void);
int __cdecl Script_OpSetLineSlipDisabled(void);
int __cdecl Script_OpIfByte(void);
int __cdecl Script_OpIfByteLong(void);
int __cdecl Script_EvalCompare8(void);
int __cdecl Script_OpIfWordSigned(void);
int __cdecl Script_OpIfWordSignedLong(void);
int __cdecl Script_EvalCompare16Signed(void);
int __cdecl Script_OpIfWordUnsigned(void);
int __cdecl Script_OpIfWordUnsignedLong(void);
int __cdecl Script_EvalCompare16Unsigned(void);
int __cdecl Script_OpIfButtonHeld(void);
int __cdecl Script_OpIfButtonPressed(void);
int __cdecl Script_OpIfButtonReleased(void);
int __cdecl Script_BranchOnButtonMask(unsigned short mask);
int __cdecl Script_OpReq(void);
int __cdecl Script_OpReqSyncWait(void);
int __cdecl Script_OpReqEndWait(void);
int __cdecl Script_OpPartyReq(void);
int __cdecl Script_OpPartyReqSyncWait(void);
int __cdecl Script_OpPartyReqEndWait(void);
int __cdecl Script_RequestEntityScript(short mode, unsigned char entity, unsigned char priority, unsigned char script);
int __cdecl Script_OpReturn(void);
int __cdecl Script_OpReturnTo(void);
int __cdecl Script_OpJumpBack(void);
int __cdecl Script_OpJumpBackLong(void);
int __cdecl Script_OpJumpForward(void);
int __cdecl Script_OpJumpForwardLong(void);
int __cdecl Script_OpMapJump(void);
int __cdecl Script_OpSetPreloadMapId(void);
int __cdecl Script_OpWaitPreloadDone(void);
int __cdecl Script_OpMinigame(void);
int __cdecl Script_OpEnterBattle(void);
void __cdecl Field_ClearRequest(void);
int __cdecl Script_OpSoundCommand(void);
int __cdecl Script_OpSoundCommandLong(void);
int __cdecl Script_OpSoundEffect(void);
void __cdecl Script_OpSubmitMusicRequest10(void);
void __cdecl Script_OpSubmitMusicRequest14(void);
void __cdecl Script_OpSubmitMusicRequest15(void);
int __cdecl Script_OpSubmitRequestWithArgs(void);
int __cdecl Field_SubmitMusicRequest(void);
int __cdecl Script_GetAkaoBlockOffset(short id);
int __cdecl Script_OpSetMusicId(void);
int __cdecl Script_OpSetMusicIdAlt(void);
int __cdecl Script_OpMenu(void);
int __cdecl Script_OpSetAudioDisabled(void);
int __cdecl Script_OpSetSpeedBoost(void);
int __cdecl Script_OpSetUnusedFlag37(void);
int __cdecl Script_OpRequestDiscChange(void);
int __cdecl Script_OpSetDialogueLock(void);
int __cdecl Script_OpSetEncountersDisabled(void);
int __cdecl Script_OpSetFieldHidden(void);
int __cdecl Script_OpSetMovieCameraDisabled(void);
int __cdecl Script_OpFieldRequest1A(void);
int __cdecl Script_OpSetPlayerEntity(void);
int __cdecl Script_OpAttachModel(void);
int __cdecl Script_OpSetIdleAnimation(void);
int __cdecl Script_OpSetStandardAnimation(void);
void __cdecl Script_BeginModelAnimationFromOperands(void);
int __cdecl Script_OpAnimateAndWait(void);
int __cdecl Script_OpAnimateAndWaitAlt(void);
int __cdecl Script_OpPlayAnimation(void);
int __cdecl Script_OpPlayAnimationFrom(void);
int __cdecl Script_OpWaitAnimationDone(void);
int __cdecl Script_OpStopAnimation(void);
int __cdecl Script_OpMoveTo(void);
int __cdecl Script_OpMoveToDirect(void);
int __cdecl Script_OpMoveToStyled(void);
int __cdecl Script_OpSetMoveStyle(void);
int __cdecl Script_OpMoveTo3D(void);
int __cdecl Script_OpMoveToMode(void);
int __cdecl Script_OpMoveToPartyMember(void);
int __cdecl Script_OpMoveToEntity(void);
int __cdecl Script_StepModelToModel(unsigned char targetEntity);
int __cdecl Script_OpFaceEntity(void);
int __cdecl Script_OpFacePartyMember(void);
int __cdecl Script_FaceModelAtModel(short otherEntity);
int __cdecl Script_OpTurnToEntity(void);
int __cdecl Script_OpTurnToPartyMember(void);
int __cdecl Script_TurnModelToModel(short entity);
int __cdecl Script_OpJumpTo(void);
int __cdecl Script_OpWaitOffsetDone(void);
int __cdecl Script_OpWaitTurnDone(void);
int __cdecl Script_OpTurnToAngle(void);
int __cdecl Script_OpTurnStart(void);
int __cdecl Script_OpSetDirection(void);
int __cdecl Script_OpSetTalkRadius(void);
int __cdecl Script_OpSetTalkRadiusW(void);
int __cdecl Script_OpSetCollisionRadius(void);
int __cdecl Script_OpSetCollisionRadiusW(void);
int __cdecl Script_OpSetMoveSpeed(void);
int __cdecl Script_OpSetAnimationSpeed(void);
int __cdecl Script_OpGetEntityDirection(void);
int __cdecl Script_OpGetPartyDirection(void);
int __cdecl Script_OpGetEntityTriangle(void);
int __cdecl Script_OpGetEntityXY(void);
int __cdecl Script_OpGetEntityXYZI(void);
int __cdecl Script_OpGetPartyXYZI(void);
int __cdecl Script_OpSetVisible(void);
int __cdecl Script_OpSetTalkOff(void);
int __cdecl Script_OpSetPositionXYZI(void);
int __cdecl Script_OpSetPositionXYZ(void);
int __cdecl Script_OpSetPositionXYI(void);
int __cdecl Script_OpShowMessage(void);
int __cdecl Script_OpSetLocationName(void);
int __cdecl Script_OpAskChoice(void);
int __cdecl Script_OpCloseWindowAndWait(void);
int __cdecl Script_OpReopenWindow(void);
int __cdecl Script_OpWindowCreate(void);
int __cdecl Script_OpWindowSetRows(void);
int __cdecl Script_OpWindowResize(void);
int __cdecl Script_OpWindowClose(void);
int __cdecl Script_OpWindowWaitClosed(void);
int __cdecl Script_OpWindowSetMode(void);
int __cdecl Script_OpAndByte(void);
int __cdecl Script_OpAndWord(void);
int __cdecl Script_OpOrByte(void);
int __cdecl Script_OpOrWord(void);
int __cdecl Script_OpXorByte(void);
int __cdecl Script_OpXorWord(void);
int __cdecl Script_OpAddByte(void);
int __cdecl Script_OpAddByteSat(void);
int __cdecl Script_OpAddWord(void);
int __cdecl Script_OpAddWordSat(void);
int __cdecl Script_OpSubByte(void);
int __cdecl Script_OpSubByteSat(void);
int __cdecl Script_OpSubWord(void);
int __cdecl Script_OpSubWordSat(void);
int __cdecl Script_OpMulByte(void);
int __cdecl Script_OpMulWord(void);
int __cdecl Script_OpDivByte(void);
int __cdecl Script_OpDivWord(void);
int __cdecl Script_OpModByte(void);
int __cdecl Script_OpModWord(void);
int __cdecl Script_OpIncByte(void);
int __cdecl Script_OpIncByteSat(void);
int __cdecl Script_OpIncWord(void);
int __cdecl Script_OpIncWordSat(void);
int __cdecl Script_OpDecByte(void);
int __cdecl Script_OpDecByteSat(void);
int __cdecl Script_OpDecWord(void);
int __cdecl Script_OpDecWordSat(void);
int __cdecl Script_OpRandomByte(void);
int __cdecl Script_OpSetRandomStride(void);
int __cdecl Script_OpBgOn(void);
int __cdecl Script_OpBgOff(void);
int __cdecl Script_OpBgClr(void);
int __cdecl Script_OpBgRol(void);
int __cdecl Script_OpBgRol2(void);
int __cdecl Script_OpMovieOpen(void);
int __cdecl Script_OpMoviePlay(void);
int __cdecl Script_OpGetMovieFrame(void);
int __cdecl Script_OpSetGatewaysDisabled(void);
int __cdecl Script_OpScrollToPoint(void);
int __cdecl Script_OpScrollToPlayer(void);
int __cdecl Script_OpScrollToEntity(void);
int __cdecl Script_OpScrollToPartyMember(void);
int __cdecl Script_OpScrollReset(void);
int __cdecl Script_OpScrollToPointLinear(void);
int __cdecl Script_OpScrollToPointSmooth(void);
int __cdecl Script_OpWaitScroll(void);
int __cdecl Script_OpPaletteStore(void);
int __cdecl Script_OpPaletteStorePartial(void);
int __cdecl Script_OpPaletteLoad(void);
int __cdecl Script_OpPaletteLoadPartial(void);
void __cdecl Field_ResetPaletteRect(int *rect);
int __cdecl Script_OpPaletteCopy(void);
int __cdecl Script_OpPaletteCopyRange(void);
int __cdecl Script_OpPaletteRotate(void);
int __cdecl Script_OpPaletteRotateRange(void);
int __cdecl Script_OpPaletteAdd(void);
int __cdecl Script_OpPaletteAddRange(void);
int __cdecl Script_OpPaletteMultiply(void);
int __cdecl Script_OpPaletteMultiplyRange(void);
void __cdecl Field_SetPlayerModelFromParty(void);
int __cdecl Script_OpBindCharacter(void);
int __cdecl Script_OpPartyAdd(void);
int __cdecl Script_OpPartyRemove(void);
int __cdecl Script_OpPartySet(void);
int __cdecl Script_OpSetFieldParty(void);
int __cdecl Script_OpGetFieldParty(void);
void __cdecl Savemap_SetFieldParty(unsigned char *party);
void __cdecl Savemap_PartyDiffSlots(unsigned char *, unsigned char *, unsigned char *onlyB, unsigned char *onlyA);
void __cdecl Field_PartySyncToSavemap(int unused);
void __cdecl Savemap_ReconcileFieldParty(void);
void __cdecl Savemap_PartyClearSlots(unsigned char *dst, unsigned char *src);
void __cdecl Savemap_PartyFillSlots(unsigned char *dst, unsigned char *src);
int __cdecl Script_OpIfCharInParty(void);
int __cdecl Script_OpIfCharAvailable(void);
int __cdecl Script_OpSetCharAvailable(void);
int __cdecl Script_OpMemberLock(void);
int __cdecl Script_OpMemberUnlock(void);
int __cdecl Script_OpSetSolid(void);
int __cdecl Script_OpSetViewOffset(void);
int __cdecl Script_OpPartyJoinLeader(void);
int __cdecl Script_OpPartySplitTo(void);
int __cdecl Script_StepFollowerToLeader(short entity, short moveArg);
int __cdecl Script_StepFollowerToPoint(short entity, short x, short y, unsigned char dir, short moveArg);
void __cdecl Field_StartScriptedMove(short entity, short x, short y, short moveArg, short mode);
int __cdecl Field_TakeMoveDone(short entity);
void __cdecl Field_StartEntityTurn(short entity, unsigned char from, unsigned char to);
int __cdecl Field_TakeTurnDone(short entity);
int __cdecl Script_OpScreenFade(void);
int __cdecl Script_OpFadeParams(void);
int __cdecl Script_OpWaitFade(void);
int __cdecl Script_OpSetAccessBlocked(void);
int __cdecl Script_OpGetWindowColour(void);
int __cdecl Script_OpSetWindowColour(void);
int __cdecl Script_OpGetFieldMapId(void);
int __cdecl Script_OpShake(void);
int __cdecl Script_OpAddItem(void);
int __cdecl Script_OpRemoveItem(void);
int __cdecl Script_OpGetItemQty(void);
int __cdecl Script_OpSpecial(void);
int __cdecl Script_OpSetLayerScroll(void);
int __cdecl Script_OpSetLayerDepth(void);
int __cdecl Script_OpAddMateria(void);
int __cdecl Script_OpRemoveMateria(void);
int __cdecl Script_OpCountMateria(void);
int __cdecl Script_OpFieldRequest(void);
int __cdecl Script_OpSetMenuLocked(void);
int __cdecl Script_OpGetPartyCharId(void);
int __cdecl Script_OpWindowBindParam8(void);
int __cdecl Script_OpWindowBindParam16(void);
int __cdecl Script_OpSinScaled(void);
int __cdecl Script_OpCosScaled(void);
int __cdecl Script_OpHealEveryone(void);
void __cdecl Field_HealPartyToFull(void);
void __cdecl Party_ReloadCombatStats(void);
int __cdecl Script_OpFullHeal(void);
int __cdecl Script_OpHealMp(void);
int __cdecl Script_OpDamageMp(void);
int __cdecl Script_OpHealHp(void);
int __cdecl Script_OpDamageHp(void);
int __cdecl Script_OpAddGil(void);
int __cdecl Script_OpSpendGil(void);
int __cdecl Script_OpGetGil(void);
int __cdecl Script_OpGetFieldFxParams(void);
int __cdecl Script_OpGetMusicPlaying(void);
int __cdecl Script_OpSetCountdown(void);
int __cdecl Script_OpWindowSetNumberDisplay(void);
int __cdecl Script_OpWindowSetNumber(void);
int __cdecl Script_OpSetBattleFlagsByte(void);
int __cdecl Script_OpSetBattleFlagsWord(void);
int __cdecl Script_OpGetBattleResult(void);
int __cdecl Script_OpSetEncounterTable(void);
int __cdecl Script_OpSetModelBlink(void);
int __cdecl Script_OpModelGfxCommand(void);
int __cdecl Script_OpWaitModelGfxCommand(void);

/* field_fx_fade_overlays.cpp */
extern int g_FieldFxFadeColor;
extern int g_FieldFxSlotDrawRequested;
extern int g_FieldFxStepCounter;
extern int g_FieldFxFlagF4;
void __cdecl Field_SelectFxRenderObject(int value);
void __cdecl Field_SetFxViewportRect(int left, int top, int width, int height);
void __cdecl Field_CreateFxRenderObjects(struct GfxEngineContext *context);
void __cdecl Field_DestroyFxRenderObjects(void);
void __cdecl Field_FxSubmitFadeOverlay(struct GfxEngineContext *drawParam);
void __cdecl Field_FxDrawBlackOverlayAlpha(int alpha);
void __cdecl Field_FxApplyScaledFadeColor(short red, short green, short blue);
void __cdecl Field_FxApplyFlatFadeColor(short red, short green, short blue);
void __cdecl Field_FxFadeStepOut(void);
void __cdecl Field_FxFadeStepIn(void);
short __cdecl Field_FxLerpComponent(short start, short end, short step, short total);
void __cdecl Field_FxStepColorBlendTransition(void);
void __cdecl Field_FxStepInverseColorBlendTransition(void);
void __cdecl Field_UpdateFieldFx(void);

/* field_math_psx_helpers.cpp */
int __cdecl Field_ReturnTrue_1(void);
int __cdecl Field_ReturnTrue_2(void);
int __cdecl Field_ReturnFalse(void);
int __cdecl Field_SqrtFixed12(int fixedValue);
void __cdecl Field_VecCross32(struct FieldVec3i *vecA, struct FieldVec3i *vecB, struct FieldVec3i *out);
void __cdecl Field_NormalizeVectorFixedThunk(int vector, int out);
void __cdecl Field_DecodeTPage(short tpage, void *colorMode, void *pageX, void *pageY, void *transparency);
void __cdecl Field_VecSub16To32(struct FieldVec3i *dst, struct FieldVec16 *vecA, struct FieldVec16 *vecB);
int __cdecl Field_PointHeightInTriangle(struct FieldVec3i *vec, struct FieldVec3i *edge, struct FieldPointTo *basePoint, struct FieldVec16 *point);

/* field_empty_stubs.cpp */
void __cdecl Field_UnusedNoOp_2(void);
void __cdecl Field_UnusedNoOp_3(void);
void __cdecl Field_NoOp(char *message, int value);
void __cdecl Field_UnusedNoOp_4(void);

/* field_container_lighting.cpp */
int __cdecl Field_UpdateContainerLighting(struct FieldModelInstance *model, struct FieldLightSource *light, struct FieldModelCommandRecord *command, int bankIndex, int subIndex, int useCache);
void __cdecl Field_ApplyCachedVertexLighting(struct GfxPolygonSet *polySet, struct FieldLightSource *light, int bankIndex, int subIndex, int useCache);
void __cdecl Field_SetupPointLight(struct FieldLightPosition *pos, struct GfxLightVertexRecord *lightInput, int value, struct FieldLightSource *light, void *outColor);
int __cdecl Field_EvaluateCurveSteps(struct FieldModelInstance *container, struct FieldLightSource *lightPosition, struct FieldModelCommandRecord *command, int mode, struct FieldCurveStepRecord *curve);

/* tutaddr.cpp */
void __cdecl Field_LoadTutorialText(int fieldId);
void __cdecl Field_FreeTutaddrBuffer(void);
int __cdecl Field_ResolveOffsetTableEntry(int base, int index);

extern struct FieldEntity g_FieldEntityState[16];
extern struct FieldLine g_FieldLines[32];
extern struct FieldEntity *g_pFieldEntities;
extern struct FieldState *g_pFieldState;
extern struct FieldActorState g_FieldActorState;
extern struct FieldModelLightCache g_FieldModelLightCaches[18];
struct FieldBgLayer3Tile;
struct FieldBgLayer4Tile;
struct FieldLayer1Tile;
struct FieldLayer2StateE14;
struct FieldLayer3StateE20;

/* ad_bk.cpp */
extern struct FieldLayer2StateE14 g_FieldLayer2StateE08;
extern struct FieldLayer2StateE14 g_FieldLayer2StateE10;
extern struct FieldLayer2StateE14 g_FieldLayer2StateE14;
extern struct FieldLayer3StateE20 g_FieldLayer3StateE20;
extern struct FieldBgLayer3Tile *g_FieldLayer3Tiles;
extern struct FieldLayer2StateE14 g_FieldLayer3StateE34;
extern struct FieldLayer1Tile *g_FieldLayer1Tiles;
extern struct FieldLayer3StateE20 g_FieldLayer4StateE88;
extern struct FieldBgLayer4Tile *g_FieldLayer4Tiles;

/* ad_pal.cpp */
short __cdecl Field_RepackPixelWordMode2(int input, int mode);

struct Timestamp64;

/* field_engine_model_anim.cpp */
extern struct Timestamp64 g_FieldTimestampCounter;

/* ad_human.cpp */
int __cdecl Field_HeadingBetweenPoints(struct FieldPointTo *from, struct FieldPointTo *to, int *distance);

struct FieldWalkmeshVertex;
struct FieldHeadTemplate;

/* field_engine_model_anim.cpp */
extern struct FieldModelSortRecord g_FieldModelSortRecords[18];

/* ad_human.cpp */
extern struct FieldWalkmeshVertex *g_pFieldWalkmeshVertices;
extern short *g_pFieldWalkmeshAccessTable;

/* field_model_vertex_lighting.cpp */
extern struct FieldHeadTemplate g_FieldHeadTemplates[2];

struct WindowData;

/* ad_bk.cpp */
extern int g_FieldLayer1StateE4c;
extern short g_FieldLayer1StateE50[2];

/* ad_data.cpp */
extern unsigned char g_FieldModelCommandGate[72];
extern struct FieldClearColor g_FieldClearColor;
extern unsigned char g_FieldBgLayer2OffsetY[0x4];
extern char g_FieldDataDir[0x100];

/* ad_obj.cpp */
extern struct GfxLightOwner *g_FieldModelLightData[14];
int __cdecl Field_InstantiateModels(struct PfileLoadOptions *opts, struct GfxEngineContext *ctx);

/* ad_tile.cpp */
extern struct FieldLayer *g_FieldLayers[0x2a];

/* field_encounter_random.cpp */
extern unsigned char g_FieldRandomTable[0x100];

/* field_engine_model_anim.cpp */
extern unsigned char g_FieldModelSortBuffer[0x48];

/* field_fx_fade_overlays.cpp */
extern unsigned char g_FieldFxFadeRed[0x4];
extern unsigned char g_FieldFxFadeBlue[0x4];
extern unsigned char g_FieldFxFadeGreen[0x4];

/* field_message_window.cpp */
extern struct WindowData g_FieldWindows[4];

/* field_request_and_sound.cpp */
extern struct FieldRequest * g_pFieldRequest;

/* field_script_operands.cpp */
extern short g_FieldModelIdleAnimSpeed[0x10];
extern unsigned char g_FieldEntityLine[0xb8];
extern unsigned short g_FieldScriptPriorityPC[0x30][8];
extern unsigned char g_FieldEntityModel[0x30];
extern unsigned short g_FieldPalettes[0x40][16];
extern unsigned char g_FieldWindowText[4][0x100];
extern unsigned char g_FieldFollowerState[0x30];
extern unsigned char g_FieldModelIdleAnimId[0x10];
extern unsigned char g_FieldDialogTextState[0x4];
extern unsigned char g_FieldScriptPriority[0x30];
extern unsigned char g_FieldScriptPriorityState[0x30][8];
extern short g_FieldModelAnimSpeed[0x10];
extern unsigned char g_FieldMovieOffset[0x8];
extern unsigned char g_FieldTempBank[0x100];
extern char g_FieldCurrentMapName[0x80];
FILE *__cdecl Field_FileOpen(char *name, char *mode);
short __cdecl Field_FileClose(FILE *file);
void __cdecl Field_FileSeek(FILE *file, int offset, int origin);
int __cdecl Field_FileTell(FILE *file);
int __cdecl Field_FileSize(FILE *file);
int __cdecl Field_DebugReadU8(FILE *file, void *dst);
int __cdecl Field_DebugReadU16(FILE *file, void *dst);
int __cdecl Field_DebugReadU32(FILE *file, void *dst);
short __cdecl Field_FileRead(FILE *file, void *dst, unsigned int size);
int __cdecl Field_FileWrite(FILE *file, void *src, int size);
int __cdecl Field_DebugWriteU8(FILE *file, unsigned char value);
int __cdecl Field_DebugWriteU16(FILE *file, unsigned short value);
int __cdecl Field_DebugWriteU32(FILE *file, unsigned int value);

/* tutaddr.cpp */
extern char *g_pFieldTutorialTextBuffer;

/* ad_image.cpp */
extern void * g_pFieldHandImageBuffer;

/* ad_human.cpp */
extern short g_FieldArcsinTable[130];

/* Defined in condor_unit_hud_readouts.cpp (field window text pointer & script opcode tables). */
extern int g_FieldWindowTextPointers[4];
extern int (__cdecl *g_FieldScriptOpTable[256])(void);

extern unsigned short g_FieldModelDefaultSpeed[0x18];
extern short g_FieldMovieFrame;
extern unsigned char g_FieldRandomIndex;
#ifdef __cplusplus
}
#endif

#endif /* FF7_API_FIELD_H */

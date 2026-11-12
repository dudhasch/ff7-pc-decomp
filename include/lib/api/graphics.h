/* include/lib/api/graphics.h: the functions and objects lib/graphics defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_GRAPHICS_H
#define LIB_API_GRAPHICS_H

#include "lib/types/graphics.h"

#ifdef __cplusplus
extern "C" {
#endif
struct GfxRasterClipState;
typedef void(__cdecl *DxGroupCallback)(int part, int geometry);
typedef void(__cdecl *DxGroupPartCallback)(int part, int vector, int geometry);
typedef void(__cdecl *DxGroupPairCallback)(int part, int geometry, int vector, int other);
struct SVECTOR;
struct VECTOR;
struct CVECTOR;
struct PsxMatrix;
struct GfxPsxMatrixTransposed;
struct BattleFxScreenFadeTask;
struct GfxEngineContext;
struct ComDirect3DDevice2;
struct GfxBatchNode;
struct PfileLoadOptions;
struct GfxDrawSpec;
struct GfxRasterClipScratch;
struct GameModule;
struct GfxAmbientColor;
struct GfxBgraPixel;
struct GfxBlendCaps;
struct GfxBlendRecord;
struct PolygonHundredState;
struct GfxDrawPoolDesc;
struct GfxDrawPoolList;
struct GfxDriverDesc;
struct GfxDriverLoadSpec;
struct GfxFlaggedRecord;
struct GfxFloatColor;
struct GfxFloatColorAddend;
struct GfxLitLightColor;
struct GfxHomogeneousVec4;
struct GfxClipVec4;
struct GfxHomogeneousPoint;
struct GfxFloorVertex;
struct GfxLight;
struct GfxLightSource2;
struct GfxLightEntryList;
struct GfxLightMatrixTransposed;
struct GfxLightOwner;
struct GfxLightRange;
struct GfxModelTableBank;
struct GfxPolygonSet;
struct GfxPsxModelTable;
struct GfxPsxRgbColorIn;
struct GfxPsxShortVertexSlot;
struct GfxRasterVertexBuffer;
struct GfxRasterIntermediates;
struct GfxRasterStatePairIn;
struct GfxRasterStatePairOut;
struct SwRasterData;
struct SwRasterVertex;
struct SwTriangleGradients;
struct SwRasterSourceVertex;
struct GfxExecPosition;
struct GfxScaledColor;
struct GfxShpRecord;
struct GfxTriangleVertexSet;
struct GfxVertexTextured;
struct GfxWireMesh;
struct GlColorArrays;
struct GlConfig;
struct GlContext;
struct GlContextConfig;
struct GlTextureNameTable;
struct PolygonModel;
struct PolygonSkeletonFrameRef;
struct SwClippedLine;
struct SwGradientMatrixX;
struct SwGradientMatrixY;
struct SwLineOps;
struct SwLineSpanPrim;
struct SwLineVertexRecord;
struct SwRasterTransform;
struct SwRasterTriangle;
struct SwStateFloatTag;
struct SwZBuffer;
struct PsxSVectorZ32;
struct GfxDebugLogTarget;
struct GfxLightColorMatrix;
struct GfxLightDirectionMatrix;
struct PolygonFileArrays;
struct GfxModelTableDesc;
struct GfxModelTableCreateDesc;
struct GfxNormalizedColor;
struct GfxPsxProjectedPoint;
struct GfxPsxRenderObjectSet;
struct GfxPsxSpriteQuadState;
struct GfxPsxTexturedFlatTriangle;
struct GfxRasterCallbacks;
struct GfxRasterContext;
struct GfxTextureSurface;
struct GfxRasterGroupSetup;
struct GfxRasterSetupBlock;
struct GfxRasterTriangle;
struct GfxRasterUvDepthGradients;
struct GfxRasterZBuffer;
struct GfxZBufferClearState;
struct GlColorBytes;
struct GlRasterClipBounds;
struct GlRasterEdgeSlope;
struct SwClipVertex;
struct SwFloatClipRect;
struct SwIntClipRect;
struct GfxDrawSpec;
struct BattleFxBladeBeamTrailPoint;
struct BattleFxShardQuad;
struct Vec3sPacked;
struct BattleFxVector3;
struct GfxClipResult;
struct GfxClipVertex;
struct GfxClippedLine;
struct GfxD3DDeviceDesc;
struct GfxD3dPrimCaps;
struct GfxLightMatrixA;
struct PolygonTransform;
struct GfxLineVertex;
struct GfxModelTable;
struct GfxModelTableRecord;
struct GfxPolygonBatch;
struct GfxPsxClutPosition;
struct GfxRasterClipVertex;
struct GfxRasterDrawGroup;
struct GfxRasterEdgeChain;
struct GfxRasterEdgeVertex;
struct GfxRasterLineEndpointA;
struct GfxRasterTexRect;
struct GfxRasterVertex;
struct GfxRasterVertexCallbacks;
struct GfxRenderObject;
struct PolygonHundredRecord;
struct GfxResourceRequest;
struct GfxTableCursor;
struct GfxTexHeader;
struct GlDrawNode;
struct GlDrawState;
struct GfxTexturedFlatQuadProjection;
struct GlDrawGroup;
struct GlPolygonSet;
struct GlPixelFormatDescriptor;
struct GlRasterVertex;
struct GfxPixelFormat;
struct GfxPixelMaskTemplate;
struct GfxDdPixelFormat;
struct SwRasterState;
struct GfxOffscreenRequest;
struct GfxOffscreenSlot;
struct BlitLineFlatState;
struct BlitDraw3DTexturedSource;
struct BlitDraw3DTexturedState;
struct GfxBgraColorSource;
struct GfxBlockHeapNodeList;
struct GfxColorRowTable;
struct GfxD3DExecuteBufferHeader;
struct GfxD3DExecuteDrawNode;
struct GfxD3DMatrixSlots;
struct GfxD3DViewport;
struct GfxD3DScaledViewport;
struct GfxD3d5BatchHeader;
struct GfxDraw2DRecord;
struct GfxDraw2DTexturedRecord;
struct GfxDraw3DRecord;
struct GfxDraw3DTexturedRecord;
struct GfxDrawListNode;
struct GfxExecBuffer;
struct GfxExecTransformBlock;
struct GfxExecGroup;
struct GfxExecTriangleGroup;
struct GfxMatrix3Row;
struct GfxMatrix3x;
struct GfxMatrix4x4;
struct GfxTransposedMatrix;
struct GfxMatrix4x4Translation;
struct GfxMatrixRow4;
struct GfxMatrixState;
struct GfxMatrixTable;
struct GfxMatrixTable2;
struct GfxMatrixTranslationColumn;
struct GfxMatrixTranslationRow;
struct GfxMatrixZRowSource;
struct GfxMeshDrawParams;
struct GfxPaletteBlock;
struct GfxPaletteState;
struct GfxPolySet;
struct GfxPolySetBatchArray;
struct GfxPrimitiveMeshBank;
struct GfxPsxQuadState;
struct GfxPsxTexturedFlatTriangleState;
struct GfxTriangleDepthRange;
struct SwRasterVertexSource;
struct GfxRasterRenderDesc;
struct GfxRasterVertexSeed;
struct GfxRasterVertexTexV;
struct GfxRawMatrix3Row;
struct GlTexelDest;
struct GfxRenderMaterial;
struct GfxRenderPool;
struct GfxRenderStateCache;
struct GfxRenderStateRecord;
struct GfxRenderTextureInfo;
struct GfxSortedBatchRecord;
struct GfxSortedDrawQueue;
struct GfxSortedGroupRecord;
struct GfxSortedDrawRecord;
struct GfxSortedPrimitive;
struct GfxSortedTriangleRecord;
struct GfxSpecularDepthState;
struct GfxSpecularState;
struct GfxSpriteBatch;
struct GfxSpriteBatchHeader;
struct GfxTexturePageFill;
struct GfxTransformMatrix;
struct GfxTransformMatrixRowForm;
struct GfxTransposeZSource;
struct GfxTriangleRange;
struct GfxVertexBgr;
struct GlTexelDest;
struct GfxViewportMatrix;
struct GlBlit2DNode;
struct GlBlit2DTexturedSource;
struct GlDeferredDrawNode;
struct GlDraw2DNode;
struct GlMatrixDevice;
struct MemFloatColorRowBuffer;
struct Rgba;
struct SwBlitDescriptor;
struct SwRasterizerState;
struct SwBlitState;
struct SwBlitTargetTable;
struct SwDeferredDrawNode;
struct SwEdgePair;
struct SwEdgePairNext;
struct SwEdgePairRgb;
struct SwEdgePairZ;
struct SwEdgePairNextZ;
struct SwEdgePairZRgb;
struct SwEdgePairNextZRgb;
struct SwLineDrawFlatNode;
struct SwPaletteDesc;
struct SwRasterSlots;
struct SwUvTriangle;
struct GfxAppDescriptor;
struct DxGuid;
struct IUnknown;
struct GfxTextureSurfaceDesc;
struct GfxD3DDeviceRecord;
struct GfxD3d5BatchState;
struct GfxDdDriverRecord;
struct GfxDisplayModeDesc;
struct GfxHomogeneousVec43;
struct GfxIndexedDrawRequest;
struct GfxPsxInputVertex;
struct GfxRecipWVec4;
struct GfxSpriteIndexBuffer;
struct GfxTextureFormatRecord;
struct GfxTlVertex;
struct GfxVertexColor;
struct GfxVertexGroup;
struct GfxPaletteRecord;

/* directx.cpp */
extern char g_DirectDrawEnumDevicePrefix[1];
void __cdecl InitAppDescriptor(struct GfxAppDescriptor *descriptor);
void *__cdecl AllocEngineContext(struct GfxAppDescriptor *descriptor);
void __cdecl CreateEngineContextFromDefaultDescriptor(void);
void __cdecl SetEngineContext(int value);
int __cdecl EngineContextToCallbackParam(struct GfxEngineContext *context);
struct GfxEngineContext *__cdecl GetEngineContext(void);
int __cdecl GetEngineContextFromCallbackParam(int value);
int __cdecl GetEngineWindowHandle(void);
void __cdecl DestroyEngineContext(void);
void __cdecl RequestEngineQuit(int engine);
int __stdcall GameWindowProc(int hWnd, unsigned int msg, unsigned int wParam, int lParam);
int __cdecl CreateGameWindow(int hInstance, struct GfxEngineContext *ctx);
int __cdecl ReportEngineError(struct GfxEngineContext *ctx, char *message, char *title, int type);
void *__cdecl CreateEngineContextDefaults(struct GfxAppDescriptor *descriptor);
void *__cdecl CreateEngineContext(void);
void __cdecl UpdateFrameTiming(struct GfxEngineContext *ctx);
int __cdecl RestoreLostSurfaces(struct GfxEngineContext *context);
void __cdecl GfxPresentBackBuffer(struct GfxEngineContext *context);
void __cdecl FormatDDEnumText(int index, int value, int buffer);
int __cdecl AllocDDDriverRecord(void);
void __cdecl FreeDDDriverRecord(struct GfxDdDriverRecord *driverRecord);
void __cdecl AppendDDDriverRecord(int record, struct GfxEngineContext *holder);
int __stdcall DDEnumDriversCallback(struct DxGuid *guid, int description, int name, int data);
int __stdcall DDEnumDisplayModesCallback(struct GfxDisplayModeDesc *desc, int data);
int __cdecl AllocD3DDeviceRecord(void);
void __cdecl FreeD3DDeviceRecord(struct GfxD3DDeviceRecord *deviceRecord);
void __cdecl AppendD3DDeviceRecord(int record, struct GfxEngineContext *holder);
int __stdcall D3DEnumDevicesCallback(struct DxGuid *guid, int description, int name, struct GfxD3DDeviceDesc *hardware, struct GfxD3DDeviceDesc *software, int data);
int __cdecl TestD3DDeviceCapabilityMask(int mask, struct GfxD3DDeviceDesc *record);
int __cdecl D3DDeviceColorModelMatches(struct GfxD3DDeviceDesc *device, struct GfxEngineContext *config);
int __cdecl D3DPickDeviceCallback(struct GfxD3DDeviceRecord *candidate, struct GfxD3DDeviceRecord **outDevice);
int __cdecl GfxGetSurfaceByIndex(int bufferIndex);
int __cdecl GfxGetRenderTargetSurface(struct GfxEngineContext *context);
int __cdecl D3DCreateDeviceFromSurface(int surface, int iid);
int __cdecl GfxCreateD3DDevice(int iid, struct GfxEngineContext *context);
int __cdecl D3DCreateDevice2FromSurface(int iid, struct GfxEngineContext *context);
int __cdecl GfxCreateD3DDevice2(int iid, struct GfxEngineContext *context);
void __cdecl FreeTextureFormatRecord(int formatRecord);
int __cdecl CompareTextureFormatRecords(struct GfxTextureFormatRecord *recordA, struct GfxTextureFormatRecord *recordB);
int __stdcall D3DEnumTextureFormatsCallback(struct GfxTextureSurfaceDesc *desc, int data);
int __cdecl LogTextureFormatRecord(struct GfxTextureFormatRecord *record);
int __cdecl ZBufferDepthSupported(int bitDepth, int zCaps);
int __cdecl PickZBufferDepth(int desc);
void __cdecl D3DDeviceDescInit(struct GfxD3DDeviceDesc *desc);
void __cdecl DDSurfaceDescInit(struct GfxTextureSurfaceDesc *desc);
int __cdecl GfxDriverInitStub(void);
void __cdecl GfxDriverCleanupStub(void);
int __cdecl GfxInitRenderResources(struct GfxEngineContext *context);
int __cdecl InitGraphicsDevice(struct GfxEngineContext *context);
void __cdecl GfxShutdownRenderResources(struct GfxEngineContext *context);
void __cdecl ShutdownGraphicsDevice(struct GfxEngineContext *context);
int __cdecl GfxLockSurface(int index);
int __cdecl GfxUnlockSurface(int index);
int __cdecl DDSurfaceSetSrcColorKey(int value, int surface);

/* dx_3d2d.cpp */
int __cdecl Gfx_IsEdgeCrossNonPositive2d(float *ptA, float *ptB, float *ptC);
void __cdecl Gfx_DrawSortedScreenPrimitive(struct GfxSortedDrawRecord *drawRecord);
void __cdecl Gfx_DrawSortedGroupRecord(struct GfxSortedGroupRecord *groupRecord);
void __cdecl Gfx_DrawSortedTriangleRecord(struct GfxSortedTriangleRecord *triangleRecord);
void __cdecl Gfx_DrawSortedPrimitiveBatchRecord(struct GfxSortedBatchRecord *batchRecord);
void __cdecl Gfx_Draw2DRecordD3D(struct GfxSortedPrimitive *drawRecord);
void __cdecl Gfx_DestroySortedDrawQueue(struct GfxSortedDrawQueue *drawQueue);
struct GfxSortedDrawQueue *__cdecl Gfx_CreateSortedDrawQueue(int blockCount, int depthRange, int bucketCount);
struct GfxSortedPrimitive *__cdecl Gfx_AllocSortedDrawRecords(int count, struct GfxSortedDrawQueue *queue);
void __cdecl Gfx_ResetSortedDrawQueue(struct GfxSortedDrawQueue *drawQueue);
void __cdecl Gfx_FlushSortedDrawQueue(struct GfxSortedDrawQueue *drawQueue, struct GfxEngineContext *context);
void __cdecl Gfx_BeginSortedDrawFrame(struct GfxEngineContext *context);
void __cdecl Gfx_EndSortedDrawFrame(struct GfxEngineContext *context);
int __cdecl Gfx_InsertSortedDrawRecord(float depth, struct GfxSortedPrimitive *record, struct GfxSortedDrawQueue *queue);
void __cdecl Gfx_EnqueueGroupForDepthSort(int inlineMatrix, int matrixRef, struct GfxPolygonBatch *batch, int group, int renderState, int rasterGroup, struct GfxEngineContext *context);
void __cdecl Gfx_EnqueuePsxPrimitiveForDepthSort(int model, int extra, int diffuse, int texAttr, struct GfxDrawListNode *node, struct GfxEngineContext *context);

/* dx_mat.cpp */
#ifndef FF7_SETPERSPECTIVE_NARROW_ABI
void __cdecl SetPerspective(float fov, float nearPlane, float farPlane, float offsetX, float offsetY, float viewWidth, float viewHeight, struct GfxEngineContext *context);
#endif
/* dx_dbg.cpp */
char *__cdecl Gfx_GetDirectXErrorString(int result);
void __cdecl Gfx_DestroyDebugLogTarget(struct GfxDebugLogTarget *logTarget);
int __cdecl Gfx_CreateDebugLogTarget(int truncate, void *path);

/* dx_graph.cpp */
int __cdecl CountSetBits32(int word);
int __cdecl BitsLostFromEight(int bits);
void __cdecl Blit_BuildColorRampTables(unsigned int *rampR, unsigned int *rampG, unsigned int *rampB, struct GfxPixelFormat *format);
int __cdecl Gfx_SelectTextureFormat(int *flagsPtr, struct GfxTexHeader *formatDesc, struct GfxEngineContext *target);
int __cdecl Gfx_TextureFormatEnumRangeFallbackCallback(struct GfxTextureFormatRecord *candidate, struct GfxTextureFormatProbe **stateRef);
int __cdecl Gfx_TextureFormatEnumCurrentModeCallback(struct GfxTextureFormatRecord *candidate, struct GfxTextureFormatProbe **stateRef);
int __cdecl Gfx_TextureFormatEnumRangeCallback(struct GfxTextureFormatRecord *candidate, struct GfxTextureFormatProbe **stateRef);
int __cdecl Gfx_TextureFormatEnumPalettizedCallback(struct GfxTextureFormatRecord *candidate, struct GfxTextureFormatProbe **stateRef);
void __cdecl GfxUploadTexturePixels(struct GfxTexHeader *params, struct GfxTextureSurfaceDesc *texture, int ddSurface);
char __cdecl Gfx_PackNibblePairToByte(int value);
void __cdecl Gfx_ReleaseTextureSurfacesAndObject(struct GfxTextureSurface *texture);
void __cdecl GfxReleaseTextureSurfaces(struct GfxTextureSurface *texture);
void __cdecl Gfx_FreeTextureRecord(struct GfxTextureSurface *texture);
int __cdecl GfxCreateTextureSurfaces(struct GfxTextureSurface *texture, struct GfxTexHeader *convertParams, struct GfxTextureFormatRecord *surfaceSpec);
int __cdecl Gfx_AcquireTextureSurface(int unused, int request, int format);
int __cdecl Gfx_AcquireTextureForRequest(int options, int texHeader, int device);
void __cdecl Gfx_DestroyTexture(struct GfxTexHeader *texture);
void __cdecl Gfx_ReleaseCachedTexture(int texture);
int __cdecl Gfx_AllocTexRecord(void);
int __cdecl Gfx_LoadTexFile(int request, const char *fileName);
int __cdecl Gfx_LoadTexFileCached(int outTexture, const char *fileName);
int __cdecl Gfx_WriteTexFile(char *fileName, struct GfxTexHeader *texRecord);
int __cdecl Gfx_LogTexFileRegistryEntry(int entry, int totalPixels);
void __cdecl Gfx_LogTexFileRegistryTotalPixelSize(int holder);
int __cdecl Gfx_UploadTexturePalette(int device, int surface, int startIndex, int entryCount, int texRecord);

/* dx_mat.cpp */
void __cdecl D3DReleaseComSlotAndClear(struct IUnknown **slot);
int __cdecl D3DCreateMaterial(struct GfxEngineContext *ctx);
void __cdecl Gfx_FreeD3DMaterialStruct(void *record);
int __cdecl Gfx_AllocD3DMaterialStruct(void);
void __cdecl D3DFillMaterialColorFromBgra(int colorSlot, struct GfxBgraColorSource *color, void *material);
void __cdecl Gfx_FreePHundred(int renderState);
int __cdecl Gfx_AllocPHundred(void);
float __cdecl DegreesToRadians(float degrees);
float __cdecl RadiansToDegrees(float radians);
void __cdecl D3DDeleteMatrixHandle(int matrix, struct GfxEngineContext *ctx);
int __cdecl D3DCreateMatrixHandle(struct GfxEngineContext *ctx);
void __cdecl D3DUploadMatrixToHandle(int matrix, int handle, struct GfxEngineContext *ctx);
void __cdecl D3DGetMatrix(int matrix, int handle, struct GfxEngineContext *ctx);
void __cdecl Gfx_FreeMatrix4x4(void *matrix);
int __cdecl Gfx_AllocIdentityMatrix4x4(void);
void __cdecl MultiplyRotation3x3(struct GfxRotationMatrixA *lhs, struct GfxMatrix3Row *rhs, struct GfxRotationMatrixA *result);
void __cdecl MultiplyRotation3x3ByTranspose(struct GfxLightMatrixA *lhs, struct PolygonTransform *rhs, struct GfxMatrix3Row *result);
void __cdecl ConcatRotationAndTranslationRow(struct GfxMatrix3Row *lhs, struct GfxMatrix3Row *rhs, struct GfxMatrix3Row *dest);
void __cdecl ConcatRotationAndTranslationColumn(struct GfxMatrix4x4 *lhs, struct GfxMatrix3x *rhs, struct GfxMatrix3x *dest);
void __cdecl MultiplyMatrix4x4(struct GfxMatrix4x4 *lhs, struct GfxMatrix4x4 *rhs, struct GfxMatrix4x4 *dest);
void __cdecl PostMultiplyMatrix4x4InPlace(int lhs, int matrix);
void __cdecl MultiplyMatrix4x4RightTransposed(struct GfxMatrix4x4 *matrixA, struct GfxMatrix4x4 *matrixB, struct GfxMatrix4x4 *product);
void __cdecl TransformVector3ByTranspose3x3(struct GfxMatrix3Row *matrix, struct GfxHomogeneousPoint *inVec, struct GfxHomogeneousPoint *outVec);
void __cdecl TransformPointByTransposeHomogeneous(struct GfxMatrix4x4 *matrix, struct GfxHomogeneousPoint *inPoint, struct GfxHomogeneousVec4 *outPoint);
void __cdecl TransformPoint3ByTranspose(struct GfxMatrix3Row *matrix, struct GfxHomogeneousPoint *inPoint, struct GfxHomogeneousPoint *outPoint);
void __cdecl TransformVector4ByMatrix4x4(struct GfxMatrix4x4 *matrix, struct GfxHomogeneousVec4 *inVec, struct GfxHomogeneousVec4 *outVec);
void __cdecl TransformPointZByTranspose(struct GfxMatrixZRowSource *matrix, struct GfxHomogeneousPoint *inPoint, struct GfxHomogeneousPoint *outPoint);
void __cdecl TransformVector3ByMatrix3x3(struct GfxMatrix3Row *matrix, struct GfxHomogeneousPoint *inVec, struct GfxHomogeneousPoint *outVec);
void __cdecl TransformPointHomogeneous(struct GfxMatrix4x4 *matrix, struct GfxHomogeneousPoint *inPoint, struct GfxHomogeneousVec4 *outPoint);
void __cdecl TransformPoint3ByMatrix4x4(struct GfxMatrix3x *matrix, struct GfxHomogeneousPoint *inPoint, struct GfxHomogeneousPoint *outPoint);
void __cdecl TransposeTransformVec4(struct GfxMatrix4x4 *matrix, struct GfxHomogeneousVec4 *inVec, struct GfxHomogeneousVec4 *outVec);
void __cdecl TransposeTransformPointZ(struct GfxTransposeZSource *matrix, struct GfxHomogeneousPoint *inPoint, struct GfxHomogeneousPoint *outPoint);
void __cdecl PerspectiveDivideInPlace(struct GfxHomogeneousVec4 *vec4);
void __cdecl PerspectiveDivide(struct GfxHomogeneousVec4 *inVec, struct GfxHomogeneousVec43 *outVec);
void __cdecl PerspectiveDivideRecipW(struct GfxHomogeneousVec4 *inVec, struct GfxRecipWVec4 *outVec);
void __cdecl PerspectiveDivideWithRhw(struct GfxHomogeneousVec4 *inVec, struct GfxClipVertex *clipVertex);
void __cdecl WriteIdentityMatrix4x4(struct GfxTransposedMatrix *matrix);
void __cdecl WriteIdentityMatrix3x3(struct GfxRawMatrix3Row *matrix3x3);
void __cdecl ClearMatrixRow4(struct GfxMatrixRow4 *matrix);
void __cdecl ClearMatrixColumn4(void *matrix);
void __cdecl ClearMatrixTranslation(struct GfxTransformMatrix *matrix);
void __cdecl SetMatrixTranslationRow(struct GfxExecPosition *translation, struct GfxMatrix4x4Translation *matrix);
void __cdecl SetMatrixTranslationRowDup(struct GfxExecPosition *translation, struct GfxTransformMatrixRowForm *matrix);
void __cdecl BuildTranslationMatrixRowVec(void *translation, void *matrix);
void __cdecl SetMatrixTranslationColumn(struct GfxExecPosition *translation, struct GfxTransformMatrix *matrix);
void __cdecl SetMatrixTranslationColumnDup(struct GfxExecPosition *translation, struct GfxTransformMatrix *matrix);
void __cdecl BuildTranslationMatrixColVec(int translation, void *matrix);
void __cdecl TranslateMatrixRowVec(void *translation, void *matrix, void *outMatrix);
void __cdecl AddMatrixTranslationRow(struct GfxHomogeneousPoint *offset, struct GfxMatrixTranslationRow *matrix);
void __cdecl PreTranslateMatrix4x4(int translation, void *matrix, void *outMatrix);
void __cdecl AddTranslationColumn(struct GfxHomogeneousPoint *offset, struct GfxMatrixTranslationColumn *matrix);
void __cdecl MultiplyAxisRotationRowVec(int axis, float angle, void *src, void *dst);
void __cdecl MultiplyAxisRotationColVec(int axis, float angle, void *src, void *dst);
void __cdecl RotateMatrixAboutAxisColVec(int axis, float angle, void *matrix);
void __cdecl TransposeMatrix4x4(struct GfxTransposedMatrix *src, struct GfxTransposedMatrix *dst);
void __cdecl TransposeMatrix4x4InPlace(int matrix);
void __cdecl BuildProjectionMatrix(float sx, float fov, float zn, float zf, float cx, float cy, float vw, float vh, float (*out)[4]);
void __cdecl BuildSquareProjectionMatrix(float fov, float zn, float zf, float (*out)[4]);
void __cdecl DestroyRenderMaterial(struct GfxRenderMaterial *material);
int __cdecl CreateMatrixArray(int n);
void __cdecl D3DSetMatrix(int slot, struct GfxMatrix4x4 *matrix, struct GfxMatrixState *state, struct GfxEngineContext *context);
int __cdecl GetCachedDeviceTransform(int slot, void *record);
void __cdecl Gfx_SetMatrixTableSlotStatus(int value, int slot, struct GfxMatrixTable *table);
void __cdecl Gfx_SetMatrixTableSlot(int status, int slot, int matrix, struct GfxMatrixTable *table);
void __cdecl Gfx_SetMatrixTableSlotCall(int mode, int slot, struct GfxMatrixTable2 *table, struct GfxEngineContext *context);
void __cdecl D3DMultiplyMatrix(int slot, void *rhs, struct GfxD3DMatrixSlots *slots, struct GfxEngineContext *context);
void __cdecl Gfx_MultiplyContextMatrix(void *matrix, struct GfxEngineContext *context);
void __cdecl BuildViewportMatrix(struct GfxViewportMatrix *matrix, struct GfxEngineContext *context);
int __cdecl ComposeModelToScreenMatrix(void *matrix, void *dst);
int __cdecl ProjectPointWithMatrix(int matrix, int point, int dst, int scale);
int __cdecl BuildModelToScreenMatrix(int matrix, void *dst);
int __cdecl ProjectPointWithBuiltMatrix(int matrix, int point, struct GfxHomogeneousVec4 *dst, int scale);
int __cdecl ProjectModelPointToScreen(int matrix, void *dst, struct GfxHomogeneousVec43 *scale);
int __cdecl ProjectPointWithContextMatrix(int point, int dst, int scale);
int __cdecl ProjectPointWithContextBuiltMatrix(int point, struct GfxHomogeneousVec4 *dst, int scale);
int __cdecl ComposeInputWithCachedTransforms(int matrix, void *dst);
int __cdecl BuildScreenTransformMatrix(void *dst);
void __cdecl ComposeWithScreenMatrix(void *out, void *context);
float __cdecl Matrix4x4Minor(int skipRow, int skipCol, struct GfxMatrix4x4 *matrix);
float __cdecl Matrix4x4Cofactor(int row, int column, struct GfxMatrix4x4 *matrix);
void __cdecl Matrix4x4CofactorRow(int row, struct GfxMatrix4x4 *matrix, struct GfxMatrix4x4 *output);
void __cdecl Matrix4x4ScaleInPlace(float scale, struct GfxMatrix4x4 *matrix);
void __cdecl Matrix4x4DivideByScalar(float scale, void *matrix);
float __cdecl Vector4Dot(struct GfxHomogeneousVec4 *vecA, struct GfxHomogeneousVec4 *vecB);
void __cdecl Gfx_ProbeProjectionMatrix(struct GfxEngineContext *viewport);

/* dx_mesh.cpp */
void __cdecl DxMesh_DrawPrimitiveRetry(int vertices, struct ComDirect3DDevice2 *device, struct GfxEngineContext *context);
void __cdecl DxMesh_DrawIndexedPrimitiveRetry(struct GfxPolygonBatch *batch, struct ComDirect3DDevice2 *device, struct GfxEngineContext *context);
void __cdecl DxMesh_TransformAndDrawGroup(int matrix, struct GfxPolygonBatch *batch, struct GfxPolygonSet *drawGroup, void *reserved, struct GfxRasterDrawGroup *subrange, struct GfxMeshDrawParams *drawParams, void *context);

/* dx_pal.cpp */
int __cdecl D3DCreatePaletteObject(int bitDepth, struct GfxPaletteRecord *paletteDesc);
int __cdecl D3DSurfaceSetPalette(struct GfxPaletteRecord *surface, int vtableSlot);
int __cdecl D3DPaletteSetEntries(int bufferIndex, int count, int startEntry, struct GfxPaletteState *palette);
int __cdecl D3DTexturePaletteChanged(int entryOffset, int count, int startEntry, int palette, struct GfxTextureSurface *texture);
int __cdecl Gfx_WritePaletteEntriesD3D(int srcStart, int count, int src, int dstStart, struct GfxPaletteRecord *record);

/* dx_rend.cpp */
void __cdecl D3DExecApplyColorModulate(unsigned int *color, void *material);
void __cdecl D3DFreePolygonSet(struct GfxPolygonSet *polygonSet);
int __cdecl D3DAllocPolygonSet(void);
int __cdecl D3DExecAllocGroupArrays(struct GfxExecGroup *group);
int __cdecl D3DExecCompileGroup(int groupIndex, struct GfxExecTransformBlock *transformBlock, int renderStates, int triangleList, int vertexBuffer, struct GfxPolygonSet *polygonSet, struct GfxEngineContext *target);
void __cdecl D3DExecResetCompileInfo(void *compileInfo);
int __cdecl D3DExecCompileBuffer(int mode, struct GfxExecTransformBlock *transformBlock, int renderStates, int triangleList, int vertexBuffer, void *compileState, void *polygonSet);
int __cdecl D3DExecEmitInstruction(int opcode, int instrSize, int count, int *sizeOut, int stream);
int __cdecl D3DExecEmitTransformState(int stateType, int value, int *countOut, int *sizeOut, int stream);
int __cdecl D3DExecBuildVertexArray(int vertexOut, int modulate, struct GfxExecTriangleGroup *group, struct GfxExecBuffer *vertexBuffer, struct GfxD3DExecuteBufferHeader *header);
int __cdecl D3DExecAlignInstructionStream(int alignment, int *sizePtr, int stream);
int __cdecl D3DExecVertexStride(int buffer);
int __cdecl D3DExecEmitRenderStateBlock(int blockOffset, int mode, int *sizeOut, int buffer, struct PolygonHundredState *stateBlock, struct GfxRenderStateCache *stateCache);
int __cdecl D3DExecEmitRenderState(int stateType, int stateValue, int *countPtr, int *sizePtr, int buffer);
int __cdecl D3DExecEmitLightState(int lightStateType, int lightValue, int *countPtr, int *sizePtr, int buffer);
int __cdecl D3DExecEmitTriangleList(int emitData, int *sizePtr, int buffer, struct GfxTriangleRange *range, struct GfxPolySet *polySet);
void __cdecl D3DExecRenderDeferredPass(int passKind, int deferredList, struct GfxEngineContext *renderer);
void __cdecl D3D_RenderDeferredLists(int passSelect, struct GfxEngineContext *device);
void __cdecl Gfx_RenderDeferredListsD3D(struct GfxEngineContext *device);
void __cdecl D3DListNoopCallback(void);
void __cdecl D3DAppendPolygonSetToList(struct GfxPolygonSet *newSet, struct GfxEngineContext *device);
void __cdecl D3DRemovePolygonSetFromList(void *polySet, void *device);
int __cdecl D3DAllocOpaquePassState(struct GfxEngineContext *device);
int __cdecl D3DAllocTranslucentPassState(struct GfxEngineContext *device);
void __cdecl D3DSetupDevicePolygonSets(struct GfxEngineContext *device);
void __cdecl D3DReleaseDevicePolygonSets(struct GfxEngineContext *slots);
void __cdecl D3DSetPolygonSetHasGroupState(int value, void *record);
int __cdecl D3DGetPolygonSetGroup(int index, void *polySet);
void __cdecl D3DSetPolygonSetUseGroupStates(int value, void *record);
void __cdecl D3DLinkPolygonSetGroupsIdentity(void *polySet);
void __cdecl D3DSetPolygonSetGroupSlot(int index, int groupRecord, void *polySet);
void __cdecl D3DSetAllPolygonSetGroupSlots(int group, void *polySet);
void __cdecl DestroyBlockHeapNodeList(int list);
int __cdecl CreateBlockHeapNodeList(int count);
void __cdecl ResetBlockHeapNodeList(struct GfxBlockHeapNodeList *list);

/* dx_rend5.cpp */
void __cdecl D3DReleaseContextInterfaces(struct GfxEngineContext *context);
int __cdecl D3DQueryIDirect3D2(int context);
extern int g_GfxStillDrawingCount;
int __cdecl D3D5SetRenderTargetAndViewport(int setTarget, int setViewport, int surface, int viewport, struct GfxEngineContext *state);
void __cdecl D3D5InitBatch(struct GfxD3d5BatchHeader *batch);
void __cdecl D3D5FreeBatch(struct GfxD3d5BatchState *batch);
int __cdecl D3D5AllocBatch(void);
int __cdecl D3D5CompileGroupBatch(int groupIndex, struct PolygonFileArrays *batchState);
int __cdecl D3D5BuildVertexArray(struct GfxTlVertex *vertexOut, void *colorSource, struct GfxVertexGroup *groupRecord, struct PolygonFileArrays *source);
void __cdecl ApplyGlobalColorModulate(void *colorPtr, void *record);
int __cdecl D3D5GetVertexStride(struct PolygonFileArrays *batchState);
int __cdecl D3D5VertexStride(int kind);
int __cdecl D3D5AllocBatchArray(struct GfxPolySetBatchArray *polySet);
void __cdecl D3D5ApplyRenderState(struct GfxRenderStateRecord *renderState, struct GfxEngineContext *context);
void __cdecl D3D5CountWasStillDrawing(int step, struct GfxEngineContext *context);
void __cdecl D3D5DrawGroupListPass(int passKind, int groupHead, struct GfxEngineContext *context);

/* dx_rendi.cpp */
int __cdecl Gfx_CreatePresetPolygonSetTable(struct GfxEngineContext *context);
int __cdecl Gfx_StubReturnZero(void);
void __cdecl Gfx_SetRenderStateD3D(int state, int enable, struct GfxEngineContext *context);
void __cdecl Gfx_ApplyStateUntexturedD3D(int polygon, struct GfxEngineContext *context);
void __cdecl Gfx_ApplyStateTexturedD3D(struct GfxPolygonTexture *polygon, struct GfxEngineContext *context);
void __cdecl Gfx_SelectTexturePage(int page, void *polygon);
void __cdecl Gfx_FreePresetHundreds(struct GfxEngineContext *context);
void __cdecl Gfx_BuildPresetHundreds(struct GfxEngineContext *context);
void __cdecl Gfx_ApplyPresetStateD3D(int preset, struct GfxEngineContext *context);
void __cdecl Gfx_SubmitPolygonSetFallback(struct GfxPolygonSet *drawGroup, struct GfxEngineContext *context);
void __cdecl D3D5SubmitPolygonSet(struct GfxPolygonSet *drawGroup, struct GfxEngineContext *context);
void __cdecl D3D5DrawPolygonSet(struct GfxPolygonSet *drawGroup, struct GfxEngineContext *context);
void __cdecl Gfx_DrawPolygonSetDispatch(struct GfxPolygonSet *drawGroup, struct GfxEngineContext *context);
void __cdecl Gfx_DrawRenderObjectD3D(struct GfxRenderObject *renderObject, int context);
void __cdecl Gfx_DrawMeshPolygonSet(int meshNode, int context);
void __cdecl Gfx_DrawPolygonSetList(struct GfxLightEntryList *list, int context);
void __cdecl Gfx_SetScenePassD3D(int passMode, struct GfxEngineContext *context);
int __cdecl Gfx_BeginSceneD3D(int passMode, struct GfxEngineContext *context);
void __cdecl Gfx_EndSceneD3D(struct GfxEngineContext *context);
void __cdecl D3DExecuteBufferDrawNode(struct GfxD3DExecuteDrawNode *drawNode, struct GfxEngineContext *context);
void __cdecl D3D5DrawBatchNode(struct SwDeferredDrawNode *batchNode, struct GfxEngineContext *context);

/* dx_rendx.cpp */
void __cdecl DxRendx_SetStateAndDrawIndexedRetry(struct GfxIndexedDrawRequest *drawArgs, struct GfxEngineContext *context);

/* dx_sfx.cpp */
void __cdecl Gfx_FreePolygonSetGroups(int count, int groups);
void __cdecl Gfx_UnlockPolygonSetGroup(int group, struct GfxPolygonSet *model, struct GfxEngineContext *engine);
int __cdecl Gfx_LockPolygonSetGroup(int group, struct GfxPolygonSet *model, struct GfxEngineContext *engine);
int __cdecl Gfx_ShadeModeFromGouraudFlag(int value);
void __cdecl Gfx_MakeHundredFlat(int enable, int gouraud, int blendMode, struct PolygonHundredState *record);
void __cdecl Gfx_MakeHundredTextured(int enable, int gouraud, int blendMode, struct PolygonHundredState *record);
void __cdecl Gfx_DestroyPHundredRecord(int releaseTexture, struct PolygonHundredRecord *record);
void __cdecl Gfx_DestroyTexturedHundred(int value);
int __cdecl Gfx_AllocTexturedHundred(int kind, int gouraud, int texName, int options, int context);
int __cdecl Gfx_BuildPrimitiveMesh(int kind, int fileName);
void __cdecl Gfx_DestroyPrimitiveMeshBank(struct GfxPrimitiveMeshBank *bank);
int __cdecl Gfx_CreatePrimitiveMeshBank(int recordCount, int vertexCount, int indexCount, int entryCount, int extraFlags);
void __cdecl Gfx_ResetRenderObjectPool(int pool);
void __cdecl Gfx_TrimRenderPoolOverflow(struct GfxRenderPool *pool);
void __cdecl DestroyRenderObjectAndClear(void *slotPtr);
void __cdecl Gfx_SetRenderObjectDrawUnsorted(int drawUnsorted, struct GfxRenderObject *renderObj);
void __cdecl Gfx_SetRenderObjectDrawEnabled(int drawEnabled, struct GfxRenderObject *renderObj);
void __cdecl Gfx_ResetRenderObjectBatchChain(int renderObj);
void __cdecl Gfx_EmptyRenderObjectBatch(int renderObj);
void __cdecl Gfx_EmitPolygonSetPrimitive(int noTexture, int texturePage, struct GfxDrawListNode *drawNode);
int __cdecl Gfx_AcquireBatchPrimitive(int renderObject);
int __cdecl Gfx_EmitGouraudRect(int posX, int posY, int width, int height, int colorTopLeft, int colorBottomLeft, int colorTopRight, int colorBottomRight, int reserved, int depth, int renderObj);
int __cdecl Gfx_EmitTexturedRect(int posX, int posY, int width, int height, int texX, int texY, int texWidth, int texHeight, int flags, int depth, int specular, int renderObj);
int __cdecl Gfx_EmitTexturedRectFlipped(int posX, int posY, int width, int height, int texX, int texY, int texWidth, int texHeight, int diffuse, int flags, int depth, int specular, int renderObj);
int __cdecl Gfx_EmitTexturedQuadFlipped(int posX, int posY, int width, int height, int texX, int texY, int texWidth, int texHeight, int colorTopLeft, int colorBottomLeft, int colorTopRight, int colorBottomRight, int flags, int depth, int specular, struct GfxRenderObject *renderObj);
int __cdecl Gfx_EmitFlatTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color, int reserved, int depth, int renderObj);
int __cdecl Gfx_EmitGouraudTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color0, int color1, int color2, int reserved, int depth, int renderObj);
int __cdecl Gfx_EmitTexturedTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int flags, int depth, int specular, int renderObj);
void __cdecl Gfx_AcquireBatchPrimitiveCursors(int cursors, int renderObj);
int __cdecl Gfx_GetRenderObjectTextureInfo(struct GfxRenderTextureInfo *info, struct GfxRenderObject *renderObj);
void __cdecl Gfx_InitTLVertices(int vertexCount, struct GfxSpriteBatchHeader *vertexCursor, struct GfxSpriteBatch *spriteBatch);
void __cdecl Gfx_FillSpriteBatchVertices(struct GfxSpriteBatch *spriteBatch, int desc);
void __cdecl Gfx_DestroySpriteBatch(struct GfxSpriteBatch *spriteBatch);
int __cdecl Gfx_CreateSpriteBatch(int width, int height, int srcX, int srcY, int blockCount, int mode, int renderSource, struct PfileLoadOptions *loadOptions, int name, int data);
void __cdecl Gfx_EmitScreenSprite(int posX, int posY, int sizeX, int sizeY, struct GfxRenderObject *renderObj);
void __cdecl Gfx_EmitScreenSpriteWithWord(int posX, int posY, int sizeX, int sizeY, int wordValue, struct GfxRenderObject *renderObj);
void __cdecl Gfx_EmitScaledSpriteQuad(int posX, int posY, int sizeX, int sizeY, int color, int renderObj);
void __cdecl Gfx_EmitSpriteQuad(int posX, int posY, float sizeX, float sizeY, int renderObj);
void __cdecl Gfx_EmitSpriteQuadWithWord(int posX, int posY, float sizeX, float sizeY, int wordValue, int renderObj);
int __cdecl Gfx_UploadRenderObjectPalette(int context, int index, int count, int offset, int renderObj);
void __cdecl Gfx_FillTexturePageBuffer(struct GfxTexturePageFill *fill);
int __cdecl Gfx_CreateSurfaceTexture(struct GfxTexturePageFill *fill);
int __cdecl Gfx_CreateFrameBufferTexture32(struct GfxTexturePageFill *fill);
void __cdecl Gfx_ClearTexturePageFill(int value);
int __cdecl Gfx_CreateSurfaceRenderObject(int parent, int blendMode, int flag, struct GfxTexturePageFill *surface, int layer, int userData);

/* dx_spr.cpp */
void __cdecl Gfx_EmitBegin2DUntextured(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitBegin2DTexturedNoTex(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitBegin2DTexturedOpaque(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitBegin2DTexturedBlended(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitBegin3DUntextured(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitBegin3DTexturedNoTex(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitBegin3DTexturedOpaque(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitBegin3DTexturedBlended(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitBeginLine(struct GfxPolygonSet *emitter, struct GfxBatchNode *model, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitDraw2DUntextured(struct GfxPolygonSet *emitter, struct GfxDraw2DRecord *record, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitDraw2DTextured(struct GfxPolygonSet *emitter, struct GfxDraw2DTexturedRecord *record, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitDrawLine(struct GfxPolygonSet *emitter, struct GfxDraw3DRecord *record, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitDraw3DUntextured(struct GfxPolygonSet *emitter, struct GfxDraw3DRecord *record, struct GfxEngineContext *driver);
void __cdecl Gfx_EmitDraw3DTextured(struct GfxPolygonSet *emitter, struct GfxDraw3DTexturedRecord *record, struct GfxEngineContext *driver);
void __cdecl GfxSelectPrimitiveEmitters(int flags, struct GfxRenderObject *primitive);
void __cdecl Gfx_FillPrimitiveIndexRun(int reversed, int vertsPerPrim, int entryStride, int count, int baseIndex, struct GfxSpriteIndexBuffer *dst);
void __cdecl Gfx_FillContextIndexRun(int count, struct GfxSpriteIndexBuffer *dst, struct GfxRenderObject *context);
void __cdecl Gfx_FillSequentialIndexPairs(int count, struct GfxSpriteIndexBuffer *dst);
void __cdecl Gfx_AllocSpriteIndexBuffers(int count, struct GfxPrimitiveMeshBank *meshBank);
void __cdecl AdvanceBatchVertexPointer(struct GfxRenderObject *batchContext);
void __cdecl AdvanceBatchVertexPointerAndCommandIndex(struct GfxRenderObject *batchContext);
void __cdecl InvalidateBatchGeneration(void *renderObject);
void __cdecl Gfx_SetBatchContextMatrix(int byReference, int matrixSource, int batchContext);
int __cdecl AcquireBatchNode(int count, void *batchContext);
int __cdecl AcquireBatchNodeNoCommandArea(int count, struct GfxRenderObject *batchContext);
void __cdecl AdvanceBatchVertexPointerNoCommandArea(struct GfxRenderObject *batchContext);
void __cdecl Gfx_SubmitFixedTwoQuadBatch(struct GfxRenderObject *quadBatch);
void __cdecl Gfx_SubmitFixedFourTexturedQuadBatch(struct GfxRenderObject *batch);
void __cdecl Gfx_SubmitFixedTriangleBatch(struct GfxRenderObject *batch);
void __cdecl Gfx_SubmitFixedQuadGridBatches(struct GfxRenderObject *batch);

/* dx_stat.cpp */
void __cdecl Gfx_StatsFreeRecord(int recordBlock);
int __cdecl Gfx_StatsCreateRecord(void);
void __cdecl Gfx_StatsBumpM3Count(void);
void __cdecl Gfx_StatsBumpM43Count(void);
void __cdecl Gfx_StatsBumpM4Count(void);
void __cdecl Gfx_StatsBumpMrCount(void);
void __cdecl Gfx_StatsBumpMsCount(void);
void __cdecl Gfx_StatsBumpS2Count(void);
void __cdecl Gfx_StatsBumpS3Count(void);
void __cdecl Gfx_StatsAddM3Time(void *accumulator);
void __cdecl Gfx_StatsAddM43Time(void *accumulator);
void __cdecl Gfx_StatsAddM4Time(void *accumulator);
void __cdecl Gfx_StatsAddMrTime(void *accumulator);
void __cdecl Gfx_StatsAddMsTime(void *accumulator);

/* dx_view.cpp */
void __cdecl D3DDetachViewport(struct GfxEngineContext *viewState);
void __cdecl D3DCreateAndAttachViewport(struct GfxEngineContext *viewState);
void __cdecl BuildD3DViewportData(int originX, int originY, int width, int height, struct GfxD3DScaledViewport *viewportData);
void __cdecl BuildFullscreenD3DViewportData(struct GfxD3DScaledViewport *viewportData, struct GfxEngineContext *engineContext);
void __cdecl SetD3DViewportData(int viewport, struct GfxEngineContext *viewState);
void __cdecl D3DResetFullscreenViewportRect(struct GfxEngineContext *viewState);
void __cdecl D3DDetachViewport2(struct GfxEngineContext *viewState);
void __cdecl D3DCreateAndAttachViewport2(struct GfxEngineContext *viewState);
void __cdecl BuildD3DViewport(int originX, int originY, int width, int height, struct GfxD3DViewport *viewport);
void __cdecl BuildFullscreenD3DViewport2(struct GfxD3DViewport *viewport, struct GfxEngineContext *context);
void __cdecl SetD3DViewport2Data(int viewport, struct GfxEngineContext *context);
void __cdecl D3DViewportClear(int clearColor, int clearDepth, int value, struct GfxEngineContext *context);
void __cdecl GfxClearColorAndDepth(struct GfxEngineContext *context);
void __cdecl D3DSetViewportRect(int left, int top, int width, int height, struct GfxEngineContext *context);
void __cdecl D3DSetBackgroundMaterial(struct GfxBgraColorSource *color, struct GfxEngineContext *context);

/* g_drv.cpp */
void __cdecl GfxSetBackgroundColorCall(void *color, struct GfxEngineContext *context);
int __cdecl GfxLockSurfaceCall(int surface);
void __cdecl GfxUnlockSurfaceCall(int surface);
void __cdecl PresentFrame(struct GfxEngineContext *context);
float __cdecl GetViewportAspectRatio(struct GfxEngineContext *viewport);
void __cdecl GfxClearAllCall(struct GfxEngineContext *context);
void __cdecl Gfx_SetMatrixCall(int matrix, int index, int mode, struct GfxEngineContext *context);
void __cdecl Gfx_RenderDeferredListsCall(struct GfxEngineContext *context);
void __cdecl Gfx_ApplyStateUntexturedPresetA(struct GfxEngineContext *ctx);
void __cdecl Gfx_ApplyStateUntexturedPresetB(struct GfxEngineContext *ctx);
void __cdecl Gfx_ApplyPresetStateCall(int preset, struct GfxEngineContext *context);
void __cdecl Gfx_ApplyRenderStateCall(int renderState, struct GfxEngineContext *context);
void __cdecl Gfx_DrawPolygonSetCall(struct PolygonSkeletonFrameRef *polySet, int context);
void __cdecl GfxFlushDrawList(int drawList, struct GfxEngineContext *context);
void __cdecl Gfx_DrawRenderObjectCall(struct GfxRenderObject *renderObject, struct GfxEngineContext *context);
void __cdecl Gfx_LightPolygonSetCall(struct PolygonSkeletonFrameRef *light, struct GfxLightOwner *owner);

/* gl.cpp */
void *__cdecl Gl_GetState(struct GfxEngineContext *dev);
void *__cdecl Gl_GetPaletteColorArrays(struct GfxPaletteRecord *lut);
void *__cdecl Gl_GetTextureNameTable(void *tex);
int __cdecl Gl_SetUnusedStateFlag(int value, struct GfxEngineContext *dev);
void __cdecl Gl_PublishTextureFormats(struct GfxEngineContext *dev);
void __cdecl Gl_PixelFormatFromPfd(struct GlPixelFormatDescriptor *pfd, struct GfxPixelFormat *format);
void __cdecl Gl_OpsCleanup(struct GfxEngineContext *dev);
int __cdecl Gl_OpsInit(struct GfxEngineContext *dev);
int __cdecl Gl_OpsLockSurface(int which, void *dev);
int __cdecl Gl_OpsUnlockSurface(void);
void __cdecl Gl_OpsSetViewport(int x, int y, int w, int h, struct GfxEngineContext *dev);
void __cdecl Gl_OpsClearAll(int value);
void __cdecl Gl_OpsClear(int mask, int flags, int unused, struct GfxEngineContext *dev);
void __cdecl Gl_OpsSetBackgroundColor(struct Rgba *color, struct GfxEngineContext *dev);
void __cdecl Gl_OpsUnloadTexture(struct GfxTextureSurface *tex);
void __cdecl Gl_ConvertTexturePixels(int flipVertical, int indexed, struct GfxTexHeader *convertParams, struct GfxPixelFormat *texDesc, int dstPixels);
int __cdecl Gl_OpsLoadTexture(struct GfxTextureSurface *texture, struct GfxTexHeader *convertParams, int srcDesc);
void *__cdecl Gl_OpsGetBlendDescriptor(int mode);
void __cdecl Gl_OpsSetRenderStateByte(int index, unsigned char value, struct GfxEngineContext *out);
void __cdecl Gl_OpsApplyRenderStateB(struct GlDrawGroup *state, struct GfxEngineContext *out);
void __cdecl Gl_OpsApplyRenderStateC(struct GlDrawGroup *state, struct GfxEngineContext *out);
void __cdecl Gl_ApplyBlendMode(int mode, struct GfxEngineContext *dev);
void __cdecl Gl_OpsSetBlendMode(int mode, struct GfxEngineContext *out);
void __cdecl Gl_ApplyRenderState(struct GlDrawGroup *drawGroup, struct GfxEngineContext *context);
void __cdecl Gl_OpsDrawPolygonSetIndirect(void *node, struct GfxEngineContext *context);
void __cdecl Gl_OpsSetScenePass(int which, struct GfxEngineContext *dev);
int __cdecl Gl_OpsBeginScene(int pass, struct GfxEngineContext *dev);
void __cdecl Gl_OpsEndScene(struct GfxEngineContext *dev);
int __cdecl Gl_OpsPaletteChanged(int paletteIndex, int firstEntry, int entryCount, int flags, struct GfxTextureSurface *tex);
int __cdecl Gl_OpsUpdatePaletteEntries(int first, int fmt, unsigned int *entries, int count, struct GfxPaletteRecord *lut, struct GfxTextureSurface *tex);
void __cdecl Gl_OpsNoOp(void *arg);
void __cdecl Gl_OpsSetMatrix(int which, void *src, struct GlMatrixDevice *dev);
void __cdecl Gl_DrawPolygonBatch(int matrix, struct GfxPolygonBatch *polyBatch, struct GlDrawGroup *group, struct GfxEngineContext *context);
void __cdecl Gl_DrawPolygonBatchPreTransformed(struct GfxPolygonBatch *polyBatch, struct GlDrawGroup *group, struct GfxEngineContext *context);
void __cdecl Gl_OpsDrawPolygonSet(struct GlPolygonSet *set, struct GfxEngineContext *context);
void __cdecl Gl_OpsDrawDeferredNode(struct GlDeferredDrawNode *req, struct GfxEngineContext *context);
void __cdecl Gl_OpsFlip(struct GfxEngineContext *dev);
void __cdecl Gl_OpsShutdownNoOp(void);

/* gl_node_draw_emitters.cpp */
extern unsigned char g_GfxDefaultBlendRecords[264];
void __cdecl Gl_BindNodeTexture(struct GlDrawState *state, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupScreenSpaceA(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupScreenSpaceB(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupScreenSpaceC(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupScreenSpaceD(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitTrisScreenSpace(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitTrisScreenSpaceTextured(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitTrisScreenSpaceTexturedPaged(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupModelSpaceA(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitTrisModelSpace(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupModelSpaceB(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupModelSpaceC(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupModelSpaceD(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitTrisModelSpaceTextured(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitTrisModelSpaceTexturedPaged(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupLinesA(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitSetupLinesB(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitLinesFlat(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);
void __cdecl Gl_EmitLinesGouraud(struct GlDrawState *state, struct GlDrawNode *node, struct GfxEngineContext *out);

/* gl_code.cpp */
void __cdecl Gl_LogWin32Error(const char *apiName);
void __cdecl Gl_CheckError(const char *message);
void __cdecl Gl_CheckErrorWithValue(const char *message, int value);
const unsigned char *__cdecl Gl_GetString(unsigned int name);
void __cdecl Gl_LogImplementationInfo(void);
void __cdecl Gl_InitConfigDefaults(struct GlConfig *config);
void __cdecl Gl_DeleteDisplayList(unsigned int list);
unsigned int __cdecl Gl_BeginDisplayList(void);
void __cdecl Gl_EndDisplayList(void);
void __cdecl Gl_CallDisplayList(unsigned int listId);
int __cdecl Gl_IsDoubleBuffered(void);
void __cdecl Gl_CullBackFaces(void);
void __cdecl Gl_CullFrontFaces(void);
void __cdecl Gl_DisableCulling(void);
void __cdecl Gl_SetTexEnvMode(int mode);
struct GlContext *__cdecl Gl_AllocState(void);
void __cdecl Gl_DestroyContext(struct GlContext *glState);
struct GlContext *__cdecl Gl_CreateContext(struct GlContextConfig *config, struct GfxEngineContext *display);
void __cdecl Gl_PresentFrame(struct GlContext *ctx);
void __cdecl Gl_SetPolygonModeLine(void);
void __cdecl Gl_SetPolygonModeFill(void);
void __cdecl Gl_SetShadeModelFlat(void);
void __cdecl Gl_SetShadeModelSmooth(void);
void __cdecl Gl_SetDither(int enable);
void __cdecl Gl_SetDepthTest(int enable);
void __cdecl Gl_SetDepthMask(int enable);
void __cdecl Gl_SetBlendDepthMaskStub(int unused);
void __cdecl Gl_SetBlendModeAverage(void);
void __cdecl Gl_SetBlendModeAdditive(void);
void __cdecl Gl_SetBlendModeSubtractive(struct GlContext *ctx);
void __cdecl Gl_SetBlendModeQuarterAdd(void);
void __cdecl Gl_SetBlendModeAlphaTest(void);
void __cdecl Gl_SetTexture2DEnable(int enable);
void __cdecl Gl_SetTextureFilter(int bilinear);
void __cdecl Gl_SetPerspectiveCorrection(int correct);
void __cdecl Gl_SetTextureWrapU(int repeat);
void __cdecl Gl_SetTextureWrapV(int repeat);
void __cdecl Gl_DeleteTexture(unsigned int textureName);
void __cdecl Gl_BindTexture(int textureName);
void __cdecl Gl_UnbindTexture(void);
int __cdecl Gl_CreateTexture2D(int indexed, int bytesPerPixel, int width, int height, int pixels);
void __cdecl Gl_FreeColorArrays(struct GlColorArrays *colorArrays);
int __cdecl Gl_AllocColorArrays(int count);
void __cdecl Gl_BuildColorArrays(int count, int startIndex, struct GlColorBytes *indices, struct GlColorArrays *arrays, int palette);
void __cdecl Gl_UploadPaletteEXT(int keyed, int keyIndex, int entryCount, int page, int data, int enable, struct GfxTextureSurface *texture);
void __cdecl Gl_FreeTextureNameTable(struct GlTextureNameTable *nameTable);
int __cdecl Gl_AllocTextureNameTable(int count);
int __cdecl Gl_GetTextureName(int index, int table);
void __cdecl Gl_SetTextureName(int name, int index, int table);
void __cdecl Gl_ReadFrameBuffer(int left, int top, int width, int height, int surfaceHeight, int scaleX, int scaleY, int bufferMode, int dst);
void __cdecl Gl_DrawTrisTransformedFlat(int indexCount, unsigned short *indices, int vertices, int matrix);
void __cdecl Gl_DrawTrisTransformedGouraud(int indexCount, unsigned short *indices, int vertices, int matrix);
void __cdecl Gl_DrawTrisFlat(int indexCount, unsigned short *indices, int vertexBase);
void __cdecl Gl_DrawTrisGouraud(int indexCount, unsigned short *indices, int vertexBase);
void __cdecl Gl_DrawTrisTransformedTexFlat(int indexCount, unsigned short *indices, int vertexBase, int matrix);
void __cdecl Gl_DrawTrisTransformedTexGouraud(int indexCount, unsigned short *indices, int vertexBase, int matrix);
void __cdecl Gl_DrawTrisTexFlat(int indexCount, unsigned short *indices, int vertexBase);
void __cdecl Gl_DrawTrisTexGouraud(int indexCount, unsigned short *indices, int vertexBase);
void __cdecl Gl_DrawLinesFlat(unsigned short *indexPtr, int lineCount, int vertexBase);
void __cdecl Gl_DrawLinesGouraud(unsigned short *indexPtr, int lineCount, int vertexBase);

/* graphics_blend_record_lookup.cpp */
void *__cdecl Gfx_FindSupportedBlendRecord(int count, struct GfxBlendRecord *first, struct GfxBlendCaps *caps);
void *__cdecl Gfx_FindBlendModeRecordD3D(int mode, struct GfxBlendCaps *caps);

/* sw_blit_emit_begin_draw.cpp */
void __cdecl Blit_EmitFinishNode(void *state, struct GfxEngineContext *out);
void __cdecl Blit_EmitBegin2DUntextured(struct BlitLineFlatState *state, void *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitBegin2DTexturedNoTex(struct BlitLineFlatState *state, void *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitBegin2DTexturedOpaque(struct BlitLineFlatState *state, void *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitBegin2DTexturedBlended(struct BlitLineFlatState *state, void *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitDraw2DUntextured(void *state, struct GlDraw2DNode *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitDraw2DTexturedNoTex(void *state, struct GlBlit2DNode *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitDraw2DTextured(struct SwBlitState *blitState, struct GlBlit2DTexturedSource *source, struct GfxEngineContext *context);
void __cdecl Blit_EmitBegin3DUntextured(struct BlitLineFlatState *state, void *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitDraw3DUntextured(void *state, struct GlDraw2DNode *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitBegin3DTexturedNoTex(struct BlitLineFlatState *state, void *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitBegin3DTexturedOpaque(struct BlitLineFlatState *state, void *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitBegin3DTexturedBlended(struct BlitLineFlatState *state, void *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitDraw3DTexturedNoTex(void *state, struct GlDraw2DNode *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitDraw3DTextured(struct BlitDraw3DTexturedState *blitState, struct BlitDraw3DTexturedSource *source, struct GfxEngineContext *context);
void __cdecl Blit_EmitBeginLineFlat(struct BlitLineFlatState *state, void *unused, struct GfxEngineContext *out);
void __cdecl Blit_EmitBeginLineGouraud(struct BlitLineFlatState *state, void *unused, struct GfxEngineContext *out);
void __cdecl Blit_EmitDrawLineFlat(void *state, struct SwLineDrawFlatNode *node, struct GfxEngineContext *out);
void __cdecl Blit_EmitDrawLineGouraud(void *state, struct SwLineDrawFlatNode *node, struct GfxEngineContext *out);

/* graphics_raster_line_clip.cpp */
void __cdecl Gfx_RasterClipLineToViewportBounds(int startX, int startY, int endX, int endY, struct GfxClippedLine *outLine, struct GfxEngineContext *clipCtx);
void __cdecl Gfx_RasterClipAndDrawLine16(int x0, int y0, int x1, int y1, int color, struct GfxEngineContext *context);

/* graphics_raster_line_pairs.cpp */
extern int g_GfxRasterLineUnused;
void __cdecl Gfx_RasterDrawLine16(int x0, int y0, int x1, int y1, int pixel, struct GfxEngineContext *context);

/* graphics_matrix_transform_asm.cpp */
void __cdecl Gfx_AndPolygonSetRenderFlags(int mask, int object);
void __cdecl Gfx_OrPolygonSetRenderFlags(int flags, int object);
void __cdecl Gfx_AndLmdPolygonSetRenderFlags(int mask, void *wrapper);
void __cdecl Gfx_OrLmdPolygonSetRenderFlags(int flags, void *wrapper);
void __cdecl Gfx_AndModelSetPolygonSetRenderFlags(int modelSet, int mask);
void __cdecl Gfx_OrModelSetPolygonSetRenderFlags(int modelSet, int flags);
void __cdecl Gfx_AndRenderObjectPolygonSetFlags(int mask, void *wrapper);
void __cdecl Gfx_OrRenderObjectPolygonSetFlags(int flags, int wrapper);
void __cdecl Gfx_AndModelPolygonSetRenderFlags(int polygonSet, struct PolygonModel *mask);
void __cdecl Gfx_OrModelPolygonSetRenderFlags(int polygonSet, struct PolygonModel *flags);
void __cdecl TransformPointDepthOnly(float *matrix, float *point, float *out);
void __cdecl TransformPoint3ColVec(float *matrix, float *point, float *out);

/* graphics_apply_blend_record.cpp */
void __cdecl Gfx_ApplyBlendRecordToHundred(struct GfxBlendRecord *rec, struct PolygonHundredState *dst);
int __cdecl Gfx_GetBlendModeRecordCall(int mode, void *ctx);

/* graphics_bind_driver_d3d.cpp / ddraw / gl */
struct GfxDriverOps *__cdecl GfxBindDriverOpsD3D(struct GfxEngineContext *context);
struct GfxDriverOps *__cdecl GfxBindDriverOpsDDrawBlit(struct GfxEngineContext *unused);
struct GfxDriverOps *__cdecl Gl_BindDriverOps(struct GfxEngineContext *unused);
void __cdecl Blit_OpsClearTargetRect(int clearColor, int clearDepth, int useViewport, struct GfxEngineContext *context);

/* dx_dbg.cpp */
int __cdecl Gfx_CreateDebugLogTargetTruncating(int logName);
void __cdecl WriteDebugLog(const char *message, struct GfxDebugLogTarget *target);
/* The debug-log print: message goes to the engine context's debug log target. */
void __cdecl Gfx_DebugPrint(const char *message);

/* graphics_palette_row_fill.cpp */
void __cdecl Gfx_FillColorRowBytes(int row, struct GfxColorRowTable *table);
void __cdecl Gfx_ScaleFloatColorRowToBytes(int scale, int row, struct MemFloatColorRowBuffer *buffer);
void __cdecl Gfx_BlendPaletteRowToBgr(float factor, float factor2, struct GfxFloatColor *factor3, int row, struct GfxPaletteBlock *palette);
void __cdecl Gfx_ModulatePaletteRowToBytes(void *factor, int row, struct GfxPaletteBlock *palette);

/* graphics_capability_logging.cpp */
void __cdecl Gfx_InitFlaggedRecord(struct GfxFlaggedRecord *record);
int __cdecl ReportFlagAndTest(int flags, int mask, const char *label);
void __cdecl Gfx_LogDirectDrawSurfaceCaps(int *surfaceDesc);
int __cdecl Gfx_LogD3DCompareCaps(int compareCaps);
void __cdecl Gfx_LogD3DPrimCapsToFlags(struct GfxD3dPrimCaps *primCaps, struct GfxD3dPrimCaps *outCaps);
void __cdecl Gfx_LogD3DDeviceDesc(int verbosity, struct GfxD3DDeviceDesc *deviceDesc, struct GfxD3dPrimCaps *triangleCaps, struct GfxD3dPrimCaps *lineCaps);

/* graphics_offscreen_render_slots.cpp */
void __cdecl Gfx_InitOffscreenRequest(struct GfxOffscreenRequest *request);
int __cdecl Gfx_BuildOffscreenRenderObject(struct GfxOffscreenRequest *request, struct GfxEngineContext *context);
void __cdecl Gfx_ReleaseOffscreenSlot(struct GfxOffscreenSlot *slot);
void __cdecl Gfx_ReleaseOffscreenSlots(int count, struct GfxOffscreenSlot *slots);
struct GfxOffscreenSlot *__cdecl Gfx_AllocOffscreenSlot(int count, struct GfxOffscreenSlot *slots);
void __cdecl Gfx_ServiceOffscreenSlots(int count, struct GfxOffscreenSlot *slots);
void __cdecl Gfx_DrawOffscreenSlots(int batchMode, int count, struct GfxOffscreenSlot *slots);

/* graphics_app_config_loader.cpp */
void __cdecl Gfx_LoadAppConfigFile(char *fileName, struct GfxEngineContext *config);

/* sw_gfx_raster_asm_spans.cpp */
void __cdecl Gfx_NoOpHandler(void);
void __cdecl Gfx_RasterFloatToInt64Fistp(float value, int *out);
void __cdecl Gfx_RasterFloatToInt64Framed(float value, unsigned int *out);

/* graphics_surface_pixel_raster.cpp */
int __cdecl Gfx_PixelAddressFromContext(int x, int y, int context);
void __cdecl Gfx_PutSurfacePixel16(int x, int y, int color, int surface);
void __cdecl Gfx_PutTriangleVertexPixels16(int color, struct GfxTriangleVertexSet *vertices, int surface);
void __cdecl Gfx_RasterDrawTriangleWireframe(int color, struct GfxRasterTriangle *triangle, struct GfxEngineContext *surface);
void __cdecl Gfx_RasterDrawWireEdges(struct GfxWireMesh *mesh, struct GfxEngineContext *surface);
void __cdecl Gfx_CopySurfaceRectDwords(int widthBytes, int height, int srcX, int srcY, int srcPitch, int srcBase, int dstX, int dstY, int dstPitch, int dstBase);
void __cdecl Gfx_CopySurfaceRectDoubled16(int widthPixels, int height, int srcX, int srcY, int srcPitch, int srcBase, int dstX, int dstY, int dstPitch, int dstBase);

/* sw_gfx_raster_gradient_basis.cpp */
void __cdecl Gfx_RasterBuildTriangleEdgeBasis(float *vtxA, float *vtxB, float *vtxC, struct GfxRasterUvDepthGradients *out);
float __cdecl Gfx_RasterTriangleGradientX(float dx, float dy, struct SwGradientMatrixX *matrix);
float __cdecl Gfx_RasterTriangleGradientY(float dx, float dy, struct SwGradientMatrixY *matrix);

/* sw_raster_rect_clip_test.cpp */
int __cdecl Gfx_RasterClassifyRectAgainstClipBounds(int x, int y, int width, int height, struct GfxClipResult *clipResult, struct GfxEngineContext *context);

/* sw_gfx_raster_seed_gradients.cpp */
void __cdecl Gfx_RasterComputeEdgeXSlope(int i0, int i1, struct GlRasterEdgeSlope *out, struct SwRasterVertex **verts, struct SwRasterData *ctx);
struct SwRasterSetupState;
struct SwRasterSeedBlock;
void __cdecl Gfx_RasterSeedDepthFromVertex(float t, float u, struct GfxRasterVertexTexV *xf, struct SwRasterSetupState *out);
void __cdecl Gfx_RasterSeedColorChannel1FromVertex(float scale, float value, struct GfxRasterVertexTexV *src, struct SwRasterSeedBlock *dst);
void __cdecl Gfx_RasterSeedColorChannel2FromVertex(float scale, float value, struct GfxRasterVertexTexV *src, struct SwRasterSeedBlock *dst);
void __cdecl Gfx_RasterSeedColorChannel3FromVertex(float scale, float value, struct GfxRasterVertexTexV *src, struct SwRasterSeedBlock *dst);
void __cdecl Gfx_RasterSeedTextureCoordUFromVertex(float scale, float x, struct GfxRasterVertexTexV *src, struct GfxRasterVertexSeed *dst);
void __cdecl Gfx_RasterSeedTextureCoordVFromVertex(float t, float u, struct GfxRasterVertexTexV *xf, struct GfxRasterVertexSeed *out);

/* graphics_raster_dda_span.cpp */
void __cdecl Gfx_RasterStepDdaSpanOutputs(int count, int *output, void *span);

/* sw_gfx_raster_vertex_snap.cpp */
void __cdecl Gfx_RasterInterpolateVertexZ(float tx, float ty, struct SwTriangleGradients *xf, struct SwRasterVertex *src, struct SwRasterVertex *dst, void *unused);
void __cdecl Gfx_RasterInterpolateVertexUv(float tx, float ty, struct SwTriangleGradients *xf, struct SwRasterVertex *src, struct SwRasterVertex *dst, void *unused);
void __cdecl Gfx_RasterInterpolateVertexRgbClamped(float tx, float ty, struct SwTriangleGradients *xf, struct SwRasterVertex *src, struct SwRasterVertex *dst, void *unused);
int __cdecl Gfx_RasterSnapGeneratedVertexZUvRgbOutsideFlag(struct SwTriangleGradients *index, struct SwRasterSourceVertex *srcRecord, struct SwRasterVertex *prev, struct SwRasterVertex *vertex, struct SwRasterData *coordTable);
void __cdecl Gfx_RasterSnapGeneratedVertexZ(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexColorOnly(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexZRgbNodeColorRead(struct SwTriangleGradients *xf, struct SwRasterSourceVertex *node, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexRgbNodeColorRead(struct SwTriangleGradients *xf, struct SwRasterSourceVertex *node, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexZUv(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexUv(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexRoundedZUv(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexRoundedUv(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexRoundedZUvPreDelta(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexRoundedUvPreDelta(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexZRgb(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterSnapGeneratedVertexRgb(struct SwTriangleGradients *xf, void *unused, struct SwRasterVertex *src, struct SwRasterVertex *dst, struct SwRasterData *ctx);
void __cdecl Gfx_RasterPrepareVertexZUvRgb(struct SwRasterVertex *v, struct SwRasterData *ctx);
void __cdecl Gfx_RasterPrepareVertexZ(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterPrepareVertexColorOnly(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterPrepareVertexZUv(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterPrepareVertexUv(struct SwRasterVertex *v, struct SwRasterData *ctx);
void __cdecl Gfx_RasterPrepareVertexZRgb(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterPrepareVertexRgb(struct SwRasterVertex *vertex, struct SwRasterData *context);
struct SwRasterData;
void __cdecl Gfx_RasterProjectClippedVertex(struct GfxClipVertex *vertex, struct GlRasterClipBounds *bounds, struct SwRasterData *rasterData, struct GfxRasterContext *ctx);
void __cdecl Gfx_RasterClipAndDrawTriangle(int prepare, int flipWinding, struct GfxRasterVertex *v0, struct GfxRasterVertex *v1, struct GfxRasterVertex *v2, struct GfxRasterCallbacks *callbacks, struct GfxRasterContext *ctx);
extern struct GfxRasterClipState g_GfxClipPolygonFanIndices[8];

/* sw_gfx_raster_line_draw.cpp */
extern void *g_GfxRasterLineSpanCallbacksMode4[];
extern void *g_GfxRasterLineSpanCallbacksMode0[];
extern void *g_GfxRasterLineSpanCallbacksMode1[];
void __cdecl Gfx_RasterLineSpanNoOp(void);
void __cdecl Gfx_RasterSelectLineSpanWriter(struct SwLineSpanPrim *prim, struct SwLineOps *span);
void __cdecl Gfx_RasterLineSpanFlatNoOp(void);
void __cdecl Gfx_RasterLineSpanGouraudNoOp(void);
void __cdecl Gfx_RasterDrawLineBetweenVertices(struct GlRasterVertex *startVertex, struct GfxLineVertex *endVertex, struct SwLineOps *ops, struct GfxRasterContext *rasterContext);
void __cdecl Gfx_RasterDrawClippedLineEndpoints(struct SwClippedLine *clipLine, struct GfxRasterLineEndpointA *endpointA, struct GfxRasterLineEndpointA *endpointB, struct SwLineOps *ops, struct GfxRasterContext *context);
void __cdecl Gfx_RasterDrawLineIfNotRejected(struct GfxRasterLineEndpointA *endpointA, struct GfxRasterLineEndpointA *endpointB, struct SwLineOps *ops, struct GfxRasterContext *rasterContext);
void __cdecl Gfx_RasterDrawIndexedLineList(int vertexBase, int indices, int lineCount, struct SwLineVertexRecord *vertexCount, struct GfxEngineContext *engineContext);

/* sw_gfx_raster_blit_indexed_rect.cpp */
int __cdecl Gfx_RasterRoundFloatToInt(float value);
void __cdecl Gfx_RasterBlitIndexedRectDoubledDepthTestBlendRemap(int width, int height, int sourceStride, int rasterContext);
void __cdecl Gfx_RasterBuildBlitOffsetTables(int count, int countB, int dstHigh, int dstShifted, struct GfxRasterSetupBlock *tables);
void __cdecl Gfx_RasterBlitIndexedRectOffsetTableColorKey(int width, int height, int columnOffsets, int rowOffsets, int rasterContext);
void __cdecl Gfx_RasterBlitIndexedRectOffsetTable(int width, int height, int columnOffsets, int rowOffsets, int rasterContext);
int __cdecl Gfx_RasterTryDrawTexturedRectQuads(int flags, int quadCount, int c, struct GfxRasterTexRect *quads, struct GfxEngineContext *engineContext);

/* graphics_raster_triangle_dispatch.cpp */
void __cdecl Gfx_RasterSelectGeneratedVertexCallbacks(int flags, struct GfxRasterGroupSetup *setup, struct GfxRasterVertexCallbacks *callbacks);
void __cdecl Gfx_RasterDrawTriangle(struct GfxWireEndVertex *vertexA, struct GfxWireEndVertex *vertexB, struct GfxWireEndVertex *vertexC, struct GfxRasterCallbacks *callbacks, struct GfxRasterTriangleContext *context);
void __cdecl Gfx_RasterDrawTriangleDepth(struct SwRasterVertexSource *vertexA, struct GfxTriangleDepthRange *vertexB, struct GfxTriangleDepthRange *vertexC, struct GfxRasterCallbacks *callbacks, struct GfxRasterTriangleContext *context);

/* graphics_group_vertex_draw_spec.cpp */
extern int g_GfxSpecularDispatchTable[5];
void __cdecl Gfx_UnusedNoOp(void);
void __cdecl Gfx_InitDrawSpec(int kind, struct GfxDrawSpec *drawSpec);
void __cdecl Gfx_RestoreGroupUvFromBase(int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_ScrollGroupUvFromBaseWrapped(float scrollU, float scrollV, int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_RestoreGroupVFromBase(int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_MirrorGroupVFromBase(float axis, int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_ApplyDrawSpecToGroup(int groupIndex, struct GfxPolygonSet *polygonSet, struct GfxDrawSpec *drawSpec);
void __cdecl Gfx_ApplyDrawSpecToPolygonSet(struct GfxDrawSpec *drawSpec, struct GfxPolygonSet *polygonSet);

/* graphics_vertex_color_arithmetic.cpp */
void __cdecl Gfx_UnusedNoOpGroupWalk(int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_UnusedNoOpPolygonSetWalk(struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_AddFloatColorsToVertexBgr(struct GfxFloatColorAddend *colorA, struct GfxFloatColorAddend *colorB, struct GfxVertexBgr *vertexColor);
void __cdecl Gfx_ModulateUnitFloatColorsToBytes(struct GfxFloatColor *unitColor, struct GfxFloatColorAddend *floatColor, struct GlTexelDest *vertexColor);
void __cdecl Gfx_ModulateFloatColorsDiv255ToBytes(struct GfxFloatColor *normalizedColor, struct GfxLitLightColor *floatColor, struct GlTexelDest *vertexColor);
void __cdecl Gfx_TransformedColorScaleClamped(struct GfxFloorVertex *matrix, int normal, struct GfxFloatColorAddend *lightColor, struct GfxScaledColor *maxColor, struct GfxBgraPixel *vertexColor);
void __cdecl Gfx_LightVertexDiffuse(struct GfxFloorVertex *matrix, int normal, struct GfxFloatColorAddend *lightColor, void *srcColor, struct GfxVertexColor *vertexColor);
void __cdecl Gfx_TransformPointLitColorToBgra(int color, int colorScale, struct GfxFloorVertex *vertex, int matrix, struct GfxFloatColorAddend *lightColor, void *srcColor, struct GfxBgraPixel *dst);
void __cdecl Gfx_TransformPointFloorColorToBgra(struct GfxFloorVertex *vertex, int matrix, struct GfxLitLightColor *lightColor, void *srcColor, struct GfxBgraPixel *dst);
void __cdecl Gfx_TransformPointFloorLitColorToBgra(int color, int colorScale, struct GfxFloorVertex *vertex, int matrix, struct GfxLitLightColor *lightColor, void *srcColor, struct GfxBgraPixel *dst);
void __cdecl Gfx_SetGroupVertexColor(int color, int groupIndex, struct GfxPolygonSet *polySet);
void __cdecl Gfx_SetGroupVertexAlpha(unsigned char alpha, int groupIndex, struct GfxPolygonSet *polySet);
void __cdecl Gfx_ScaleGroupVertexColorsFromSaved(int scale, int groupIndex, struct GfxPolygonSet *polySet);
void __cdecl Gfx_ForEachGroupVertexColorWithContext(int callback, struct GfxDrawSpec *userData, int groupIndex, struct GfxPolygonSet *polySet);
void __cdecl Gfx_ForEachGroupVertexAndColor(int callback, int groupIndex, struct GfxPolygonSet *polySet);
void __cdecl Gfx_ForEachGroupSavedColorVertexAndColor(int callback, int groupIndex, struct GfxPolygonSet *polySet);
void __cdecl Gfx_RestoreGroupVertexColors(int groupIndex, struct GfxPolygonSet *polySet);
void __cdecl Gfx_SaveGroupVertexColors(int groupIndex, struct GfxPolygonSet *polySet);
void __cdecl Gfx_SetPolygonSetVertexColor(int color, struct GfxPolygonSet *polySet);
void __cdecl Gfx_SetPolygonSetVertexAlpha(int alpha, struct GfxPolygonSet *polySet);
void __cdecl Gfx_ModulatePolygonSetVertexColors(int color, struct GfxPolygonSet *polySet);
void __cdecl Gfx_AddColorToPolygonSetVertexColors(struct GfxFloatColorAddend *bias, struct GfxPolygonSet *polySet);
void __cdecl Gfx_RestorePolygonSetVertexColors(struct GfxPolygonSet *polySet);
void __cdecl Gfx_SavePolygonSetVertexColors(struct GfxPolygonSet *polySet);
void __cdecl Gfx_SetVertexSpecularOpaque(struct GfxTlVertex *vertex);
void __cdecl Gfx_SetVertexSpecularNear(struct GfxTlVertex *vertex, struct GfxSpecularDepthState *specState);
void __cdecl Gfx_SetVertexSpecularCached(struct GfxTlVertex *vertex, struct GfxSpecularDepthState *specState);
void __cdecl Gfx_SetVertexSpecularByDepth(struct GfxTlVertex *vertex, struct GfxSpecularDepthState *specState);
void __cdecl Gfx_SetVertexSpecularNone(void);

/* graphics_vertex_color_arithmetic.cpp */
void __cdecl Gfx_UpdateCachedSpecularColor(struct GfxSpecularState *specState);
void __cdecl Gfx_ScaleFloatColorToBytes(int scaleBits, int input, int output);
void __cdecl Gfx_BlendRedScaleBgr(float scale, float blendWeight, void *colorB, struct GfxFloatColorAddend *colorA, struct GlTexelDest *dstBgr);
void __cdecl Gfx_BlendFloatColorsToBgr(float weightA, float blendWeight, struct GfxFloatColor *colorB, struct GfxFloatColor *colorA, int dstBgr);
void __cdecl Gfx_ScalePolygonSetGroupVertexColorsFromSaved(float scale, int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_BlendPolygonSetGroupVertexColorsFromSaved(float weightA, float weightB, struct GfxFloatColor *colorB, int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_ScalePolygonSetVertexColorsFromSaved(void *scale, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_BlendPolygonSetVertexColorsFromSavedWithColor(void *weightA, void *weightB, struct GfxFloatColor *colorB, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_ScalePolygonSetGroupColorsOrPalette(float scale, int rowIndex, int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_BlendPolygonSetGroupColorsOrPalette(float weightA, float weightB, struct GfxFloatColor *colorB, int rowIndex, int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_ModulateGroupVertexColorsFromSavedOrPalette(struct GfxFloatColor *modulation, int rowIndex, int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_MaskGroupSavedBlueToGreenRed(int colorMask, int groupIndex, struct GfxPolygonSet *polygonSet);
void __cdecl Gfx_MaskFirstGroupSavedBlueToGreenRed(int colorMask, struct GfxPolygonSet *polygonSet);

/* graphics_driver_context_fields.cpp */
void __cdecl InitGraphicsDriverDesc(int driverType, struct GfxDriverDesc *driverDesc);
int __cdecl InstallGraphicsDriver(struct GfxDriverLoadSpec *driverBlob, struct GfxEngineContext *context);
void __cdecl SetRenderStateObject(int renderState, struct GfxEngineContext *context);
void __cdecl SetGraphicsDriverObject(struct GfxDriverOps *driverObject, struct GfxEngineContext *context);
struct GfxDriverOps *__cdecl GetGraphicsDriverObject(struct GfxEngineContext *context);
int __cdecl GetLogVerbosity(struct GfxEngineContext *context);
void __cdecl SetLogVerbosity(int verbosity, struct GfxEngineContext *context);
void __cdecl CommitPendingModule(struct GfxEngineContext *context);
void __cdecl ServiceModuleSwitch(struct GfxEngineContext *ctx);
void __cdecl GetActiveModule(struct GameModule *moduleRecord, struct GfxEngineContext *context);
void __cdecl GfxFlushFrameDrawList(struct GfxEngineContext *context);
void __cdecl SelectContextSlotOverride(int slot, struct GfxEngineContext *context);
void __cdecl ClearContextSlotOverride(struct GfxEngineContext *context);
void __cdecl Gfx_SetContextBlitRectsEnabled(int value, struct GfxEngineContext *context);
void __cdecl Gfx_SetContextBlitRect(int slot, int left, int top, int right, int bottom, struct GfxEngineContext *context);
void __cdecl Gfx_ResetRasterFrameCounter(struct GfxEngineContext *context);
int __cdecl Gfx_ReadRasterFrameCounter(struct GfxEngineContext *context);
void __cdecl Gfx_OffsetVertexTexCoords(float deltaU, float deltaV, int count, struct GfxVertexTextured *vertices);

/* instance.cpp */
void __cdecl Gfx_InitPolygonSetStateRecord(void *record);
void __cdecl Gfx_DestroyDrawPool(int pool);
int __cdecl Gfx_CreateDrawPool(int nodeCount);
void __cdecl Gfx_AdvanceDrawPoolGeneration(int pool);
int __cdecl Gfx_AllocDrawPoolListNode(struct GfxDrawPoolList *list, struct GfxDrawPoolDesc *pool);
void __cdecl Gfx_FreeDrawPoolListHeadNode(struct GfxDrawPoolList *list, struct GfxDrawPool *pool);

/* light.cpp */
void __cdecl Gfx_UnpackBgraToFloat4(int color, float *out);
void __cdecl Blit_PackBgraFloatsToArgb8888Unscaled(unsigned char *destination, float *source);
void __cdecl Blit_PackBgraFloatsToArgb8888(unsigned char *destination, float *source);
void __cdecl Gfx_UnpackBgraToFloat4OpaqueAlpha(int color, float *out);
void __cdecl Blit_PackBgraFloatsToArgb8888UnscaledOpaque(unsigned char *destination, float *source);
void __cdecl Gfx_LightNormalizeColorInPlace(struct GfxNormalizedColor *color);
void __cdecl Gfx_LightNormalizeColorCopy(struct GfxNormalizedColor *src, struct GfxFloatColor *dst);
void __cdecl Gfx_LightFreeRecord(void *record);
int __cdecl Gfx_LightAllocRecord(void);
int __cdecl Gfx_LightCreate(int color, void *direction);
void __cdecl Gfx_LightBuildDirectionMatrix(struct GfxLightSource2 *dir0, struct GfxLightSource2 *dir1, struct GfxLightSource2 *dir2, struct GfxLightDirectionMatrix *dirMatrix);
void __cdecl Gfx_LightBuildColorMatrix(struct GfxLightSource2 *color0, struct GfxLightSource2 *color1, struct GfxLightSource2 *color2, struct GfxLightColorMatrix *colorMatrix);
void __cdecl Gfx_LightMultiplyMatrices(void *lhs, void *rhs, void *dst);
void __cdecl Gfx_LightMultiplyMatricesWithAmbient(void *lhs, int rhs, struct GfxAmbientColor *ambient, struct GfxLightMatrixTransposed *dst);
void __cdecl Gfx_LightMultiplyTransposedWithAmbient(struct GfxLightMatrixA *rotation, struct PolygonTransform *transform, struct GfxAmbientColor *ambient, struct GfxLightMatrixTransposed *dst);
int __cdecl Gfx_LightBuildModelVertexRecords(struct GfxLightRange *range, struct PolygonFileArrays *source);
void __cdecl Gfx_LightRebuildCachedMatrices(struct GfxLightOwner *owner);
void __cdecl Gfx_LightApplyMatrixToCachedProduct(int transform, struct GfxLightOwner *owner);
void __cdecl Gfx_LightApplyTransposedMatrixToCachedProduct(struct PolygonTransform *transform, struct GfxLightOwner *owner);
void __cdecl Gfx_LightFreeOwner(int what, struct GfxLightOwner *handle);
int __cdecl Gfx_LightCreateOwner(struct GfxLight *light0, struct GfxLight *light1, struct GfxLight *light2, int color);
void __cdecl Gfx_LightPolygonSetCallFromEntry(int entry, struct GfxLightOwner *owner);
void __cdecl Gfx_LightPolygonSetListCall(struct GfxLightEntryList *list, struct GfxLightOwner *owner);
void __cdecl Gfx_LightSetSpecularDepthMatrix(int matrix, struct GfxLightOwner *owner);
void __cdecl Gfx_LightResetSpecular(struct GfxLightOwner *owner);
void __cdecl Gfx_LightSetSpecularNearColor(int nearColor, struct GfxLightOwner *owner);
void __cdecl Gfx_LightSetSpecularCachedDepth(int depth, struct GfxLightOwner *owner);
void __cdecl Gfx_LightSetSpecularCachedRamp(int depth, float rampStart, float rampEnd, int depthMatrix, int color, struct GfxLightOwner *owner);
void __cdecl Gfx_LightBuildSpecularRamp(float rampStart, float rampEnd, int depthMatrix, unsigned int color, struct GfxLightOwner *owner);
int __cdecl Gfx_LightCreateDefaultThreeLightSet(void);

/* psx.cpp */
extern unsigned char g_GfxPsxDataBuffer[1508];
int __cdecl Gfx_GetPsxScratchpadPtr(int offset);
struct PsxMatrix *__cdecl GetInstalledRotMatrixPtr(void);
int __cdecl GetDrawColorPacked(void);
float __cdecl GetLastProjectedNdcZ(void);
float __cdecl GetLastProjectedNdcW(void);
float __cdecl GteInt16ToFloat(short value);
void __cdecl GteSVectorToFloat3Fast(struct SVECTOR *src, struct GfxHomogeneousPoint *dst);
float __cdecl GteInt32ToFloat(int value);
void __cdecl GteFixed12VectorToFloat3(struct VECTOR *vec, struct GfxHomogeneousPoint *dst);
void __cdecl GteFloat3ToFixed12Vector(int *out, float *in);
float __cdecl GteTransUnitToFloat(int value);
int __cdecl GteFloatToTransUnit(float value);
void __cdecl GteFloat4x4ToPsxMatrixColVec(struct PsxMatrix *dst, float *mm);
void __cdecl GteFloat4x4TransposedToPsxMatrix(struct PsxMatrix *dst, float *mm);
void __cdecl SetMatrixLayoutMode(int mode);
void __cdecl SetGeomOffset(int offsetX, int offsetY);
void __cdecl SetGeomScreenReference(double distance);
double __cdecl FovToProjectionDistance(double width, double fov);
double __cdecl ProjectionDistanceToFov(double width, double distance);
void __cdecl SetGeomScreen(int distance);
void __cdecl GetGeomOffset(void *outX, void *outY);
int __cdecl GetGeomScreen(void);
void __cdecl SetProjectionOffset(int offsetX, int offsetY);
void __cdecl SetProjectionOffsetAndRebuild(int x, int y);
void __cdecl GetProjectionOffset(int *outX, int *outY);
int __cdecl ApplyMatrixToSVectorInt32(void *matrix, struct SVECTOR *input, void *output);
int __cdecl ConcatMatrixIntoFirst(void *matrix, void *src);
int __cdecl ConcatMatrixPreservingTranslation(void *srcA, void *srcB, void *dst);
int __cdecl ConcatMatrixInPlace(void *src, void *matrix);
int __cdecl PreMultiplyByInstalledMatrix(struct PsxMatrix *matrix);
int __cdecl MultiplyByInstalledMatrixInto(struct PsxMatrix *src, struct PsxMatrix *dst);
void __cdecl PopRotMatrix(void);
void __cdecl PushRotMatrix(void);
int __cdecl CosFixed(int angle);
void __cdecl CosFixedAlias(int value);
int __cdecl SinFixed(int angle);
void __cdecl SinFixedAlias(int value);
int __cdecl ArcTangentFixed(int y, int x);
void __cdecl RotTransPointToInt16(void *input, short *output, int *flag);
int __cdecl RotTransPers(struct SVECTOR *input, short *output, int *flag1, int *flag2);
void __cdecl ScreenPointToWorld(int x, int y, int depth, int matrix, float *out, short *result);
void __cdecl SetSecondaryColorRGB(int red, int green, int blue);
void __cdecl SetDrawColorRGB(int red, int green, int blue);
int __cdecl ScaleMatrix(void *matrix, void *scale);
int __cdecl ScaleMatrixL(void *matrix, struct VECTOR *scale);
void __cdecl GetRotMatrix(struct PsxMatrix *rotMatrix);
void __cdecl SetTransMatrix(void *src);
int __cdecl SqrtToInt(int value);
int __cdecl SquareRoot12(int value);
int __cdecl SetMatrixTranslation(void *matrix, void *translation);
int __cdecl TransposeMatrix(void *input, void *output);
int __cdecl TransposeMatrixFull(void *input, struct PsxMatrix *output);
int __cdecl VectorNormalSVectorToSVector(short *input, short *output);
int __cdecl Gfx_PsxReturnArgument(int value);
int __cdecl Gfx_PsxReturnZero(void);
void __cdecl Gfx_PsxHookNoOp_1(int flag);
void __cdecl Gfx_PsxHookNoOp_2(void);
void __cdecl Gfx_PsxHookNoOp_3(void);
void __cdecl Gfx_PsxHookNoOp_4(int code, int value);
void __cdecl TransformPointByWorldMatrix(struct SVECTOR *point, int *dst, int *flag);
void __cdecl Psx_FreeModelTable(struct GfxPsxModelTable *modelTable);
void __cdecl Psx_FreeModelTableAndNull(struct GfxPsxModelTable **slot);
struct GfxPsxModelTable *__cdecl Psx_LoadModelTableFromRecords(char *owner, int count, struct GfxModelRecord *recordTable);
int __cdecl Psx_GetModelTablePrimary(int index, struct GfxPsxModelTable *owner);
int __cdecl Psx_GetModelTableSecondary(int index, struct GfxPsxModelTable *owner);
int __cdecl Psx_GetModelTablePrimaryAlt(int index, struct GfxPsxModelTable *owner);
int __cdecl Psx_GetModelTableSecondaryAlt(int index, struct GfxPsxModelTable *owner);

/* psxgraph.cpp */
extern unsigned char g_GfxPsxPacketScratch[36];
void __cdecl Gfx_SetBlendFactorPairEnable(int enable);
void __cdecl Gfx_SetBlendFactorFirst(int factor);
void __cdecl Gfx_SetBlendFactorSecond(int factor);
void __cdecl Gfx_SetPsxOpaqueTableAlt(int table);
int __cdecl Gfx_GetPsxPacketScratchPtr(void);
void __cdecl Gfx_LoadInstalledRotMatrixFloatTransposed(void);
void __cdecl Gfx_ComposeRotMatrixFloatWithScreenMatrix(void);
void __cdecl Gfx_PackDoubledColorToBgra(char redByte, char greenByte, char blueByte, int alphaByte, struct GfxBgraPixel *dst);
void __cdecl Gfx_DecodePsxClutPosition(int flagsWord, int clutWord, int owner, struct GfxPsxClutPosition *clutPos);
int __cdecl TransformVertexSlot0(struct GfxPsxInputVertex *vertex);
int __cdecl TransformVertexSlot1(struct GfxPsxShortVertexSlot *vertex);
int __cdecl TransformVertexSlot3(struct GfxPsxShortVertexSlot *vertex);
int __cdecl TransformVertices3(short *vertexA, short *vertexB, short *vertexC);
int __cdecl TransformVertices4(short *vertexA, short *vertexB, short *vertexC, short *vertexD);
void __cdecl Gfx_SetPendingVertexInput(const void *value);
void __cdecl Gfx_TransformPendingVertexSlot0(void);
void __cdecl Gfx_ReadVertexSlotVector(int slot, struct GfxHomogeneousVec4 *vertex);
void __cdecl Gfx_SetVertexSlotClipRect(int slot, struct GfxHomogeneousVec4 *vertex);
void __cdecl ApplyDeferredDrawArgs(void);
void __cdecl ApplyPendingResourceHandle(void);
void __cdecl Gfx_SetPendingShadeWeight(int weight);
void __cdecl Gfx_SetPendingColorRgb(struct GfxPsxRgbColorIn *color);
void __cdecl Gfx_ReadPendingColorRgb(struct GfxPsxRgbColorIn *color);
void __cdecl Gfx_ShadePendingColor(void);
void __cdecl Gfx_RotatePendingVectorByInstalledMatrix(void);
void __cdecl Gfx_ReadRotatedVector(struct GfxExecPosition *vector);
int __cdecl Gfx_TransformTriangleReturnDepth(void *vertexA, struct SVECTOR *vertexB, struct SVECTOR *vertexC, void *unused, void *flags);
int __cdecl Gfx_TransformQuadReturnDepth(void *vertexA, struct SVECTOR *vertexB, struct SVECTOR *vertexC, struct SVECTOR *vertexD, int *unused, int *flags);
void __cdecl Gfx_PackProjectedXY(float x, float y, float w, unsigned int *out);
int __cdecl Gfx_ProjectTriangleToScreen(void *vertexA, struct SVECTOR *vertexB, struct SVECTOR *vertexC, void *outPtr0, void *outPtr1, void *outPtr2, void *unused, void *flags);
int __cdecl Gfx_TransformTriangleReturnDepthAlt(void *vertexA, struct SVECTOR *vertexB, struct SVECTOR *vertexC, void *unused, void *flags);
int __cdecl Gfx_ProjectTriangleToScreenAlt(void *vertexA, struct SVECTOR *vertexB, struct SVECTOR *vertexC, void *outPtr0, void *outPtr1, void *outPtr2, void *unused, void *flags);
int __cdecl Gfx_ProjectQuadToScreenAlt(struct SVECTOR *vertexA, struct SVECTOR *vertexB, struct SVECTOR *vertexC, struct SVECTOR *vertexD, void *outPtr0, void *outPtr1, void *outPtr2, void *outPtr3, void *unused, void *flags);
void __cdecl Gfx_EmitPsxFlatLineProjected(int color, struct GfxHomogeneousVec4 *vertexA, struct GfxHomogeneousVec4 *vertexB, struct GfxRenderObject *batch);
void __cdecl Gfx_EmitPsxFlatLineScreen(int color, int x0, int y0, int x1, int y1, int z, int w, struct GfxRenderObject *batch);
void __cdecl Gfx_EmitPsxGouraudLineProjected(int color0, int color1, struct GfxHomogeneousVec4 *vertexA, struct GfxHomogeneousVec4 *vertexB, struct GfxRenderObject *batch);
void __cdecl Gfx_EmitPsxGouraudLineScreen(int color0, int color1, int x0, int y0, int x1, int y1, int z, int w, struct GfxRenderObject *batch);
void __cdecl Gfx_EmitPsxGouraudPolylineProjected(int color, int color1, int color2, struct GfxHomogeneousVec4 *vertexA, struct GfxPsxProjectedPoint *secondPoint, struct GfxHomogeneousVec4 *vertexC, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxGouraudPolylineScreen(int color0, int color1, int color2, int x0, int y0, int x1, int y1, int x2, int y2, int word0, int word1, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxFlatTriangleProjected(int color, struct GfxHomogeneousVec4 *vertex0, struct GfxHomogeneousVec4 *vertex1, struct GfxHomogeneousVec4 *vertex2, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxFlatQuadProjected(int color, struct GfxHomogeneousVec4 *vertex0, struct GfxHomogeneousVec4 *vertex1, struct GfxHomogeneousVec4 *vertex2, struct GfxHomogeneousVec4 *vertex3, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxFlatTriangleScreen(int color, int x0, int y0, int x1, int y1, int x2, int y2, int word0, int word1, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxFlatQuadScreen(int color, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int word0, int word1, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxGouraudTriangleProjected(int color0, int color1, int color2, struct GfxHomogeneousVec4 *vertex0, struct GfxHomogeneousVec4 *vertex1, struct GfxHomogeneousVec4 *vertex2, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxGouraudQuadProjected(int color0, int color1, int color2, int color3, struct GfxHomogeneousVec4 *vertex0, struct GfxHomogeneousVec4 *vertex1, struct GfxHomogeneousVec4 *vertex2, struct GfxHomogeneousVec4 *vertex3, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxGouraudTriangleScreen(int color0, int color1, int color2, int x0, int y0, int x1, int y1, int x2, int y2, int word0, int word1, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxGouraudQuadScreen(int color0, int color1, int color2, int color3, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int word0, int word1, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxGouraudQuadScreenPerVertexBlend(int color0, int color1, int color2, int color3, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int word0, int word1, int word2, int word3, int word4, int word5, int word6, int word7, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxDotTriangleProjected(int color, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxDotTriangleScreen(int x, int y, int word0, int word1, int color, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxFlatRectScreen(int x, int y, int width, int height, int word0, int word1, int color, struct GfxRenderObject *batchNode);
void __cdecl Gfx_EmitPsxTexturedFlatTriangleProjected(int payload, struct GfxHomogeneousVec4 *vertex0, struct GfxHomogeneousVec4 *vertex1, struct GfxHomogeneousVec4 *vertex2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int attr, struct GfxPsxTexturedFlatTriangle *batch);
void __cdecl Gfx_UnusedWriteProjectedVertexAtIndex(struct GfxHomogeneousVec4 *vertex, int payload, int raw0, int raw1, int index, struct GfxRenderObject *context);
void __cdecl Gfx_EmitPsxTexturedFlatQuadProjected(int payload, struct GfxHomogeneousVec4 *vertex0, struct GfxHomogeneousVec4 *vertex1, struct GfxHomogeneousVec4 *vertex2, struct GfxHomogeneousVec4 *vertex3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, struct GfxTexturedFlatQuadProjection *batch);
void __cdecl Gfx_EmitPsxTexturedFlatTriangleScreen(int payload, int x0, int y0, int x1, int y1, int x2, int y2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int attr, int depth, int reciprocal, struct GfxPsxTexturedFlatTriangleState *batch);
void __cdecl Gfx_EmitPsxTexturedGouraudTriangleProjected(int payload0, int payload1, int payload2, struct GfxHomogeneousVec4 *vertex0, struct GfxHomogeneousVec4 *vertex1, struct GfxHomogeneousVec4 *vertex2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int attr, struct GfxPsxTexturedFlatTriangle *batch);
void __cdecl Gfx_EmitPsxTexturedGouraudQuadProjected(int payload0, int payload1, int payload2, int payload3, struct GfxHomogeneousVec4 *vertex0, struct GfxHomogeneousVec4 *vertex1, struct GfxHomogeneousVec4 *vertex2, struct GfxHomogeneousVec4 *vertex3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, struct GfxTexturedFlatQuadProjection *batch);
void __cdecl Gfx_EmitPsxTexturedGouraudTriangleScreen(int payload0, int payload1, int payload2, int x0, int y0, int x1, int y1, int x2, int y2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int attr, int depth, int reciprocal, struct GfxPsxTexturedFlatTriangleState *batch);
void __cdecl Gfx_EmitPsxTexturedGouraudQuadScreen(int payload0, int payload1, int payload2, int payload3, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, int depth, int reciprocal, struct GfxPsxQuadState *quadState);
void __cdecl Gfx_EmitPsxTexturedSpriteQuad(int x, int y, int width, int height, int texX, int texY, int raw0, int raw1, int raw2, int attr, struct GfxPsxSpriteQuadState *batch);
void __cdecl Gfx_DrawPsxPrimitive(void *primitive);
void __cdecl Gfx_PsxFreeRenderObjectSet(struct GfxPsxRenderObjectSet *objectSet);
int __cdecl Gfx_PsxCreateRenderObjectSet(int blendMode, int flags, int context);
void __cdecl Gfx_PsxFreePrimitiveSystem(void);
int __cdecl Gfx_PsxCreatePrimitiveSystem(int flags, int context);
void __cdecl Gfx_DrawDisplayListRecord(struct GfxPsxRenderObjectSet *record);
void __cdecl Gfx_DrawDisplayListGroup(int group);
void __cdecl Gfx_ShadePsxVertexColor(struct CVECTOR *srcPtr, int weightRaw, struct CVECTOR *dst);
void __cdecl Gfx_SetDrawEnabled(int blendMode);
int __cdecl Gfx_IsDrawEnabled(void);

/* render.cpp */
extern struct GfxModelTable *g_GfxLastCreatedModelTable;
int __cdecl Gfx_TakeLastCreatedModelTable(void);
void __cdecl Gfx_FreeModelTable(struct GfxModelTable *table);
void __cdecl Gfx_FreeModelTableAndClear(void *tableRef);
int __cdecl Gfx_LoadModelTableFromRecords(int groupTag, int lifetime, int fileContext, int count, struct GfxModelTableRecord *records, int ownerRef, struct GfxModelTableCreateDesc *defaults);
int __cdecl Gfx_ModelTableGetRenderObject(int index, struct GfxTableCursor *cursor);
int __cdecl Gfx_ModelTableGetSpriteObject(int index, struct GfxTableCursor *cursor);
int __cdecl Gfx_ModelTableGetLoadedRecord(int index, struct GfxTableCursor *cursor);
int __cdecl Gfx_ModelTableDrawLayerOrExpire(int advanceTick, int layer, struct GfxModelTable *table);
void __cdecl Gfx_FreeModelTableBank(struct GfxModelTableBank *bank);
int __cdecl Gfx_CreateModelTableBank(int count, struct GfxModelTableDesc *desc);
int __cdecl Gfx_ModelTableBankRegisterTable(struct GfxModelTable *table, struct GfxModelTableBank *bank);
void __cdecl Gfx_ModelTableBankRemoveTable(int index, struct GfxModelTable *table, struct GfxModelTableBank *bank);
void __cdecl Gfx_ModelTableBankResetFramePools(struct GfxModelTableBank *bank);
void __cdecl Gfx_ModelTableBankDrawLayerAndRemoveExpired(int advance, int layer, struct GfxModelTableBank *bank);
void __cdecl Gfx_ModelTableBankRemoveTablesByGroupTag(int groupTag, struct GfxModelTableBank *bank);
void __cdecl Gfx_InitModelTableCreateDescriptor(struct GfxModelTableCreateDesc *desc);
void __cdecl Gfx_InitResourceRequest(struct GfxResourceRequest *record);
void __cdecl Gfx_LoadModelTableIfAbsent(int flags, int records, struct GfxResourceRequest *request, void *tableSlot, struct GfxModelTableBank *bank);

/* shp.cpp */
void __cdecl Gfx_ShpFreeRecord(struct GfxShpRecord *record);
void __cdecl Gfx_ShpFreeRecordAndNull(struct GfxShpRecord **recordSlot);
int __cdecl Gfx_ShpLoadFile(int fileName, struct PfileLoadOptions *context);
int __cdecl Gfx_SptConvertTextToBinaryFile(int fileName);
void __cdecl Gfx_SptFreeRecord(struct GfxSptRecord *record);
void __cdecl Gfx_SptFreeRecordAndNull(int recordSlot);
int __cdecl Gfx_SptLoadRecord(int fileName, struct PfileLoadOptions *context);

/* sw.cpp */
void __cdecl Blit_InsertTextureFormatRecords(struct GfxEngineContext *context);
void __cdecl Blit_FillRenderDescriptor(struct SwBlitDescriptor *descriptor, struct GfxEngineContext *context);
void __cdecl Blit_InitTargetTables(struct SwBlitTargetTable *target, struct GfxEngineContext *context);
void __cdecl Blit_FreeRenderState(struct SwRasterizerState *state);
int __cdecl Blit_CreateRenderState(struct GfxRasterRenderDesc *desc, struct GfxEngineContext *context);
void __cdecl Blit_OpsDestroyRenderState(struct GfxEngineContext *context);
int __cdecl Blit_OpsInitRenderState(struct GfxEngineContext *context);
void __cdecl Gfx_RasterClearTargetAllFlags(void *context);
void __cdecl Gfx_RasterRefreshClipRects(int unused1, int unused2, int unused3, int unused4, struct GfxEngineContext *context);
void __cdecl Blit_OpsSetBackgroundColor(void *color, struct GfxEngineContext *context);
int __cdecl Gfx_RasterAllocBatchArray(void *count);
int __cdecl Gfx_RasterCompileGroup(int group, int batch, int flags, int stride, int option, int enable, int extra);
void __cdecl Gfx_RasterSetMatrixSlot(int slot, int value, struct SwRasterSlots *slots);
void __cdecl Gfx_RasterUnloadTextureNoOp(void);
struct GfxTextureSurface *__cdecl Gfx_RasterPrepareGroupRecord(struct GfxTextureSurface *record, struct GfxTexHeader *groupDesc, struct GfxPixelFormat *texture);
void __cdecl Blit_RenderDeferredListsStub(void);
int __cdecl Blit_GetBlendModeRecord(void);
void __cdecl Blit_SetRenderState(int offset, int value, struct GfxEngineContext *ctx);
void __cdecl Blit_ApplyStateUntextured(void *group, struct GfxEngineContext *ctx);
void __cdecl Blit_ApplyStateTextured(void *group, struct GfxEngineContext *ctx);
void __cdecl Blit_ApplyPresetState(int flags, struct GfxEngineContext *ctx);
void __cdecl Blit_ApplyRenderSpec(struct GlDrawGroup *group, struct GfxEngineContext *ctx);
void __cdecl Blit_DrawRenderObject(void *object, struct GfxEngineContext *ctx);
void __cdecl Blit_OpsSetScenePass(int pass, struct GfxEngineContext *ctx);
int __cdecl Blit_OpsBeginScene(int pass, struct GfxEngineContext *ctx);
void __cdecl Blit_OpsEndScene(struct GfxEngineContext *ctx);
int __cdecl Blit_OpsPaletteChangedStub(void);
int __cdecl Blit_WritePaletteEntries(int start, int table, int rampA, int rampB, struct SwPaletteDesc *paletteDesc);
void __cdecl Blit_LightPolygonSet(int polySet, int ctx);
void __cdecl Blit_Draw2DRecordStub(int record);
void __cdecl Blit_DrawPolygonSet(struct GfxPolygonSet *drawGroup, struct GfxEngineContext *engine);
void __cdecl Blit_DrawDeferredNode(struct SwDeferredDrawNode *deferredNode, struct GfxEngineContext *engine);
void __cdecl Gfx_BlitContextRectsAndPresent(struct GfxEngineContext *context);
void __cdecl InstallScratchArena(int arena, struct GfxEngineContext *engine);

/* sw_debug_line_formatting.cpp */
extern char g_GfxLogNewline[];
extern int g_GfxOffsetErrorMsg;
extern int g_GfxFormatDefineLine;
extern int g_GfxFormatDefineVertexSize;
void __cdecl FormatDefineLine(const char *name, int second, void *buffer);
void __cdecl FormatDefineLineVertexSize(int value, int second, void *buffer);
void __cdecl Gfx_DumpDriverCapsToLog(void);
void __cdecl Gfx_RasterNoOpDispatchGuards(void);

/* sw_gfx_raster_edge_setup.cpp */
void __cdecl Gfx_RasterBuildTriangleEdgeChains(int vertexA, int vertexB, struct GfxRasterEdgeChain *chain, struct GfxRasterIntermediates *intermediates);
void __cdecl Gfx_RasterFillRenderSetupBlock(struct GfxRasterSetupBlock *setupBlock, struct GfxEngineContext *engine);
void __cdecl Gfx_RasterCopyStatePair(struct GfxRasterStatePairIn *src, struct GfxRasterStatePairOut *dst);
void __cdecl Gfx_RasterMapPackedColorTables(struct SwRasterState *state);
void __cdecl Gfx_RasterSetupTriangleFlatColorNoDepth(int unused, struct SwRasterState *dst, struct GfxTriangleVertexSet *vertices);
void __cdecl Gfx_RasterSetupTriangleFlatColorDepth(struct SwTriangleGradients *gradients, struct SwRasterState *dst, struct GfxTriangleVertexSet *vertices);
void __cdecl Gfx_RasterSetupTriangleFlatColorDepthAlphaRemap(struct SwTriangleGradients *gradients, struct SwRasterState *dst, struct GfxTriangleVertexSet *vertices);
void __cdecl Gfx_RasterSetupTriangleVertexColorSumNoDepth(int unused, struct SwRasterState *dest, struct GfxTriangleVertexSet *vertices);
void __cdecl Gfx_RasterSetupTriangleVertexColorSumDepth(struct SwTriangleGradients *gradients, struct SwRasterState *dest, struct GfxTriangleVertexSet *vertices);
void __cdecl Gfx_RasterSetupTriangleUvNoDepth(struct SwTriangleGradients *gradients, struct SwRasterState *dest, struct SwUvTriangle *vertices);
void __cdecl Gfx_RasterSetupTriangleUvDepth(struct SwTriangleGradients *gradients, struct SwRasterState *dest, struct SwUvTriangle *vertices);
void __cdecl Gfx_RasterSetupTriangleUvDepthSpanTable(struct GfxRasterUvDepthGradients *gradients, struct SwRasterState *dest, struct SwRasterTriangle *vertices);
void __cdecl Gfx_RasterSetupTriangleRgbGradientNoDepth(struct SwTriangleGradients *gradients, struct SwRasterState *dest, struct GfxTriangleVertexSet *vertices);
void __cdecl Gfx_RasterSetupTriangleRgbGradientDepth(struct SwTriangleGradients *gradients, struct SwRasterState *dest, struct GfxTriangleVertexSet *vertices);
int __cdecl Gfx_RasterSetupEdgePair(struct SwEdgePair *pair, struct GfxRasterEdgeChain *segment);
void __cdecl Gfx_RasterSetupEdgePairNext(int side, struct SwEdgePairNext *next, struct GfxRasterEdgeChain *segment);
int __cdecl Gfx_RasterSetupEdgePairRgb(struct SwEdgePairRgb *pair, struct GfxRasterEdgeChain *segment, struct GfxRasterVertexTexV *gradients);
void __cdecl Gfx_RasterSetupEdgePairNextZRgbSeeded(int side, struct SwEdgePairNextZRgb *next, struct GfxRasterEdgeChain *segment, struct GfxRasterVertexTexV *gradients);
int __cdecl Gfx_RasterSetupEdgePairUv(struct GfxRasterVertexSeed *seed, struct GfxRasterEdgeChain *segment, struct GfxRasterVertexTexV *gradients);
void __cdecl Gfx_RasterSetupEdgePairNextUv(int side, struct GfxRasterVertexSeed *next, struct GfxRasterEdgeChain *segment, struct GfxRasterVertexTexV *gradients);
int __cdecl Gfx_RasterSetupEdgePairZ(struct SwEdgePairZ *pair, struct GfxRasterEdgeChain *segment, struct GfxRasterVertexTexV *gradients);
void __cdecl Gfx_RasterSetupEdgePairNextZ(int mode, struct SwEdgePairNextZ *dst, struct GfxRasterEdgeChain *src, struct GfxRasterVertexTexV *seedCtx);
int __cdecl Gfx_RasterSetupEdgePairZRgb(struct SwEdgePairZRgb *dst, struct GfxRasterEdgeChain *src, struct GfxRasterVertexTexV *seedCtx);
void __cdecl Gfx_RasterSetupEdgePairNextZRgb(int mode, struct SwEdgePairNextZRgb *dst, struct GfxRasterEdgeChain *src, struct GfxRasterVertexTexV *seedCtx);
int __cdecl Gfx_RasterSetupEdgePairZUv(struct GfxRasterVertexSeed *dst, struct GfxRasterEdgeChain *src, struct GfxRasterVertexTexV *seedCtx);
void __cdecl Gfx_RasterSetupEdgePairNextZUv(int mode, struct GfxRasterVertexSeed *dst, struct GfxRasterEdgeChain *src, struct GfxRasterVertexTexV *seedCtx);
void __cdecl Gfx_RasterSetTextureMaskSetup(struct GfxTextureSurface *drawGroup, struct GfxRasterSetupBlock *maskState);
void __cdecl Gfx_RasterSetTextureMaskSetupRows(struct GfxTextureSurface *drawGroup, struct GfxRasterSetupBlock *maskState);

/* sw_gfx_raster_clip_planes.cpp */
void __cdecl Gfx_RasterClearBatchArena(void *block);
void __cdecl Gfx_RasterBuildFloatClipRect(struct SwFloatClipRect *rect, struct GfxEngineContext *context);
int __cdecl Gfx_RasterIsVertexInsideClipPlane(int plane, struct SwClipVertex *vertex, struct GfxRasterClipScratch *thresholds);
void __cdecl Gfx_RasterIntersectEdgeWithClipPlane(int plane, struct GfxRasterEdgeVertex *startVertex, struct SwClipVertex *endVertex, struct GfxRasterClipVertex *outVertex, struct GfxRasterClipScratch *thresholds);
int __cdecl Gfx_RasterClipEdgeToPlane(int plane, int count, void *outList, struct GfxRasterEdgeVertex *startVertex, struct SwClipVertex *endVertex, struct GfxRasterClipScratch *context);
void __cdecl Gfx_RasterClipTriangleToPlaneMask(int planeMask, struct GfxRasterVertex *vertexA, struct GfxRasterVertex *vertexB, struct GfxRasterVertex *vertexC, int maxPlane, struct GfxRasterClipScratch *clipCtx);
void __cdecl Gfx_RasterTransformAndClipTriangleDepthPlanes(struct GfxRasterVertex *vertexA, struct GfxRasterVertex *vertexB, struct GfxRasterVertex *vertexC, int maxPlane, struct GfxRasterClipScratch *scratch, struct GfxRasterContext *context);

/* sw_gfx_raster_draw_groups.cpp */
void __cdecl Gfx_RasterDrawIndexedTransformedTriangles(int useAsm, int srcVertices, int indices, int triCount, int vertexCount, struct GfxEngineContext *context);
void __cdecl Gfx_RasterDrawGroupWithMatrix(int value, struct GfxPolygonBatch *batch, struct GfxPolygonSet *drawGroup, struct GlDrawGroup *glGroup, struct GfxRasterDrawGroup *rasterGroup, struct GfxEngineContext *context);
void __cdecl Gfx_RasterDrawGroupWithoutMatrix(struct GfxPolygonBatch *batch, struct GfxPolygonSet *drawGroup, struct GlDrawGroup *glGroup, struct GfxRasterDrawGroup *rasterGroup, struct GfxEngineContext *context);
void __cdecl Gfx_RasterSubmitTriangleVertices(int vertexA, int vertexB, int vertexC, struct SwRasterTransform *transform, struct GfxEngineContext *context);
void __cdecl Gfx_RasterDrawIndexedDepthMesh(int value, struct GfxPolygonBatch *mesh, struct GfxPolygonSet *drawGroup, struct GlDrawGroup *glGroup, struct GfxRasterDrawGroup *rasterGroup, struct SwRasterTransform *depthValue, struct GfxEngineContext *context);

/* sw_vert.cpp */
void __cdecl Gfx_RasterFreeVertexBuffer(struct GfxRasterVertexBuffer *buffer);
int __cdecl Gfx_RasterCreateVertexBuffer(int vertexCount);
void __cdecl Gfx_RasterCopyContextRectExclusive(struct SwIntClipRect *clipRect, struct GfxEngineContext *context);
void __cdecl Gfx_RasterSetStateModelMatrix(int value, struct GfxRasterVertexBuffer *record);
void __cdecl Gfx_RasterSetStateCombinedMatrix(int value, struct GfxRasterVertexBuffer *record);
void __cdecl Gfx_RasterSetStateMatrixPair(int value0, int value1, struct GfxRasterVertexBuffer *record);
void __cdecl Gfx_RasterSetStateFloatTag(float floatValue, int tag, struct SwStateFloatTag *record);
void __cdecl Gfx_RasterSetStateScalePair(int value0, int value1, struct GfxRasterVertexBuffer *record);
void __cdecl Gfx_RasterResolveVertexZUvRgbCompiled(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterResolveVertexZCompiled(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterResolveVertexColorOnlyCompiled(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterResolveVertexZUvCompiled(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterResolveVertexUvCompiled(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterResolveVertexZRgbCompiled(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterResolveVertexRgbCompiled(struct SwRasterVertex *vertex, struct SwRasterData *context);
void __cdecl Gfx_RasterPrepareVertexRecordsAsm(int count, int sourceVertices, int buffer, struct GfxRasterContext *context);
void __cdecl Gfx_RasterPrepareVertexRecords(int count, struct SwLineVertexRecord *srcVerts, struct GfxRasterVertexBuffer *state, struct GfxRasterContext *context);
void __cdecl Gfx_RasterResolveVertexZUvRgb(int vertex, int context);
void __cdecl Gfx_RasterResolveVertexZ(int vertex, int context);
void __cdecl Gfx_RasterResolveVertexColorOnly(int vertex, int context);
void __cdecl Gfx_RasterResolveVertexZUv(int vertex, int context);
void __cdecl Gfx_RasterResolveVertexUv(int vertex, int context);
void __cdecl Gfx_RasterResolveVertexZUvStacked(int vertex, int context);
void __cdecl Gfx_RasterResolveVertexZRgb(int vertex, int context);
void __cdecl Gfx_RasterResolveVertexRgb(int vertex, int context);

/* z.cpp */
void __cdecl Gfx_RasterFreeZBuffer(struct GfxRasterZBuffer *zBuffer);
int __cdecl Gfx_RasterCreateZBuffer(int width, int height, int bytesPerPixel, int format);
int __cdecl Gfx_RasterCreateZBufferWithStore(int width, int height, int bytesPerPixel, int format, int stride, int store);
void __cdecl Gfx_RasterClearZBuffer(struct SwZBuffer *zBuffer);
void __cdecl Gfx_RasterClearZBufferRectIfDue(int x, int y, int width, int rows, struct GfxZBufferClearState *clearState);

/* directx.cpp */
int __cdecl AllocTextureFormatRecord(void);
void __cdecl InsertTextureFormatRecord(struct GfxTextureFormatRecord *formatRecord, struct GfxEngineContext *context);

/* driver.cpp */
void __cdecl GfxFreeDriverOps(struct GfxDriverOps *driverOps);
struct GfxDriverOps *__cdecl GfxAllocDriverOps(void);

/* dx_graph.cpp */
int __cdecl LowestSetBitIndex32(int word);
void __cdecl PixelFormatSetRgbMasks(int rgbBitCount, int redMask, int greenMask, int blueMask, int alphaMask, struct GfxPixelFormat *format);
void __cdecl PixelFormatFromDDPixelFormat(struct GfxDdPixelFormat *ddFormat, struct GfxPixelFormat *format);
void __cdecl PixelFormatLoadMaskTemplate(struct GfxPixelMaskTemplate *maskTemplate, struct GfxPixelFormat *format);
int __cdecl PackRgbToContextPixelFormat(unsigned int red, unsigned int green, unsigned int blue);
int __cdecl UnpackPixelToArgb8888(unsigned int pixel, struct GfxPixelFormat *format);
unsigned int __cdecl PackArgb8888ToPixelFormat(union GfxArgb32 color, struct GfxPixelFormat *format);
unsigned int __cdecl Blit_MapArgb8888ViaRampTables(union GfxArgb32 color, struct GfxEngineContext *context);
int __cdecl UnpackPixelRowToArgb8888(unsigned int pixelIndex, int row, struct GfxPixelFormat *format);
void __cdecl Gfx_ReleaseTexturePalette(struct GfxTextureSurface *texture);
int __cdecl Gfx_CreateSurfacePaletteRecord(int bitDepth, struct GfxTexHeader *params, struct GfxTextureSurface *texture);
void __cdecl Gfx_ConvertTexturePixelsToSurface(struct GfxTexHeader *params, struct GfxPixelFormat *pixelFormat, int dest);
void __cdecl Gfx_ReleaseTextureRecordRef(int texture);
int __cdecl Gfx_AllocSurfaceRecord(void);

/* dx_mat.cpp */
void __cdecl Gfx_CreateEmptyMatrixTable(void);

/* dx_pal.cpp */
void __cdecl Gfx_CopyPaletteEntriesD3D(int count, int srcStart, int srcEntries, int dstStart, int dstEntries);
void __cdecl Gfx_ConvertPaletteEntriesToFloat(int count, int srcStart, int srcEntries, int dstStart, int dstFloats);
void __cdecl Gfx_MapPaletteEntriesViaRampTables(int count, int srcStart, int srcEntries, int dstStart, int dstEntries);
void __cdecl Gfx_GatherPaletteEntriesByIndex(int count, int indexStart, int dstStart, int indexArray, int palette, int dstEntries);
void __cdecl Gfx_FreePaletteRecord(struct GfxPaletteRecord *paletteRecord);
int __cdecl Gfx_CreatePaletteRecord(int flags, int bitDepth, int entriesPerGroup, int entryCount, int srcEntries);

/* dx_rend.cpp */
int __cdecl AppendBlockHeapNode(int list);

/* dx_rend5.cpp */
int __cdecl D3D5CompileGroup(int index, int reserved2, int reserved3, int reserved4, struct PolygonFileArrays *batchState, struct GfxPolySetBatchArray *group);

/* g_drv.cpp */
int __cdecl Gfx_PaletteChangedCall(int index, int target);

/* light.cpp */
int __cdecl Gfx_LightCreateThreeLightOwner(void *dir0, void *dir1, void *dir2, int color0, int color1, int color2, int ambient);

/* psx.cpp */
int __cdecl MultiplyMatrix(void *srcA, void *srcB, void *dst);
int __cdecl RotateMatrixX(int angle, void *matrix);
int __cdecl RotateMatrixY(int angle, void *matrix);
int __cdecl RotateMatrixZ(int angle, void *matrix);
int __cdecl ProjectPointHardware(void *input, short *output, int *flag1, int *flag2);

/* psxgraph.cpp */
void __cdecl Gfx_SetBlendFactorPair(int firstFactor, int secondFactor);
int __cdecl Gfx_GetProjectedVertexRecord(int index);
void __cdecl Gfx_SetPsxAlpha(int alpha);
void __cdecl Gfx_SetPsxCornerAlphas(int alpha0, int alpha1, int alpha2, int alpha3);
void __cdecl Gfx_SetPsxBlendMode(int mode);
void __cdecl Gfx_SetPsxBlendEnable(int enable);
void __cdecl RefreshGlobalTransformState(void);
void __cdecl StoreDeferredDrawArgs(const void *value0, const void *value1, const void *value2);
void __cdecl Gfx_PsxStoreClearFlag(int *result);
void __cdecl Gfx_ReadTransformOrderingDepth(int *outPtr);
void __cdecl Gfx_ReadTransformOrderingDepth2(int *count);
void __cdecl Gfx_PsxStoreFrontFacing(int *result);
int __cdecl Gfx_TransformVertexSlot0ReturnDepth(void *vertex, void *unused, int *flags);

/* sw.cpp */
int __cdecl Blit_CopyContextSurfaceSize(void *sizeOut, struct GfxEngineContext *engine);

struct GfxRasterEdgeSlope;

/* g_drv.cpp */
void __cdecl GfxClearCall(int clearTarget, int clearDepth, int clearValue, struct GfxEngineContext *context);
void __cdecl GfxSetViewportRect(int x, int y, int width, int height, struct GfxEngineContext *ctx);
void __cdecl Gfx_EndSceneCall(struct GfxEngineContext *context);

/* graphics_driver_context_fields.cpp */
int __cdecl RequestModuleSwitch(struct GameModule *moduleRecord, struct GfxEngineContext *context);
void __cdecl ResetFrameDrawPool(struct GfxEngineContext *context);
void __cdecl ResetRenderPools(struct GfxEngineContext *context);
void __cdecl Gfx_ResetNodeList(struct GfxEngineContext *context);

/* psx.cpp */
void __cdecl GteVectorToFloat3(int *in, float *out);
void __cdecl GteFloat3ToVector(int *out, float *in);
void __cdecl SetIdentityPsxMatrix(struct PsxMatrix *dst);
void __cdecl SetRotMatrix(void *src);
int __cdecl RotMatrix(void *angles, void *dst);
int __cdecl RotMatrixXYZ(void *angles, void *dst);
void __cdecl RotTransPoint(void *input, void *output, void *flag);
int __cdecl RotateSVectorByInstalledMatrix(void *input, void *output);
int __cdecl NormalizeVectorFixed(void *input, void *output);
int __cdecl TransformVectorToInt16(void *matrix, void *input, void *output);
int __cdecl TransformVectorToInt32(struct PsxMatrix *matrix, void *input, void *output);
int __cdecl RotMatrixZYX(struct SVECTOR *angles, void *dst);
void __cdecl CrossProductInt(struct VECTOR *vecA, struct VECTOR *vecB, void *result);

/* psxgraph.cpp */
void __cdecl SetPendingResourceHandle(struct GfxPsxShortVertexSlot *handle);
int __cdecl Gfx_TransformVertexSlot1ReturnDepth(struct GfxPsxShortVertexSlot *vertex, void *unused, int *flags);
int __cdecl Gfx_ProjectQuadToScreen(struct SVECTOR *vertexA, struct SVECTOR *vertexB, struct SVECTOR *vertexC, struct SVECTOR *vertexD, void *outPtr0, void *outPtr1, void *outPtr2, void *outPtr3, void *unused, void *flags);
void __cdecl Gfx_ShadePsxTriVertexColors(struct CVECTOR *src0, struct CVECTOR *src1, struct CVECTOR *src2, int weightRaw, struct CVECTOR *dst0, struct CVECTOR *dst1, struct CVECTOR *dst2);

/* sw_gfx_raster_gradient_basis.cpp */
void __cdecl Gfx_RasterTriangleGradientSetup(float *vtxA, float *vtxB, float *vtxC, struct GfxRasterUvDepthGradients *out);

/* sw_gfx_raster_seed_gradients.cpp */
void __cdecl Gfx_RasterComputeEdgeSlope(int i0, int i1, struct GfxRasterEdgeSlope *out, void **verts, struct GfxRasterIntermediates *ctx);

/* psx.cpp */
int __cdecl VectorNormalToSVector(int *input, short *output);

/* psxgraph.cpp */
void __cdecl Gfx_SetPsxPrimQuadState(int opcode, int index, struct GfxPsxSpriteQuadState *state);
int __cdecl Gfx_TransformQuadReturnDepthAlt(struct SVECTOR *vertexA, struct SVECTOR *vertexB, struct SVECTOR *vertexC, struct SVECTOR *vertexD, int *unused, int *flags);

struct GfxDirectDrawDriverCaps;

/* dx_mat.cpp */
void __cdecl BuildRotationXColVec(float angle, struct GfxMatrix4x4 *matrix);
void __cdecl BuildRotationYColVec(float angle, struct GfxMatrix4x4 *matrix);
void __cdecl BuildRotationZColVec(float angle, struct GfxMatrix4x4 *matrix);
void __cdecl RotateMatrixAboutAxis(int axis, float angle, void *matrix);
int __cdecl Matrix4x4Invert(int matrix, void *output);
void __cdecl Gfx_SetContextMatrix(void *matrix, struct GfxEngineContext *context);
void __cdecl Gfx_BeginSceneCall(int scene, struct GfxEngineContext *context);
#ifndef FF7_GFX_EMITFLATRECT_FLOAT_Z
int __cdecl Gfx_EmitFlatRect(int posX, int posY, int width, int height, int color, int reserved, int depth, int renderObj);
#endif

/* dx_sfx.cpp */
void __cdecl Gfx_ForEachPolygonSetGeometry(DxGroupCallback callback, int model);
void __cdecl Gfx_ForEachPolygonSetPartPair(DxGroupPairCallback callback, int left, int right);

/* graphics_capability_logging.cpp */
void __cdecl Gfx_LogDirectDrawDriverCaps(struct GfxDirectDrawDriverCaps *driverCaps);

struct GfxLightPolygonSet;
struct GfxMatrixArray;
struct GfxPolygonSetSpec;
struct GfxPresetPolygonSetTable;
struct GfxScaleMatrix;
struct GfxStatsRecord;
struct GfxTexturedHundredOptions;

struct GfxRenderObject;
struct PfileLoadOptions;
/* dx_3d2d.cpp */
void __cdecl Gfx_EnqueuePrimitiveBatchForDepthSort(struct GfxRenderObject *batch, struct GfxEngineContext *context);
/* dx_sfx.cpp */
void *__cdecl Gfx_CreateRenderObject(int drawUnsorted, int kind, struct PfileLoadOptions *loadOptions, char *name, void *init);
/* dx_spr.cpp */
void __cdecl Gfx_EmitPrimitiveBatchList(void *list, struct GfxEngineContext *ctx);
/* psx.cpp */
void __cdecl GteFloat4x4ToPsxMatrix(struct PsxMatrix *dst, float *mm);
/* graphics_driver_context_fields.cpp */
typedef int (__cdecl *GfxDllExportFn)();
extern GfxDllExportFn g_GfxDllDriverExportTable[36];

/* dx_graph.cpp */
struct GfxTextureSurface *__cdecl Gfx_RestoreTextureSurfaces(void *texture);
/* dx_mat.cpp */
void __cdecl BuildScaleMatrixXYZ(struct GfxExecPosition *scaleVec, struct GfxScaleMatrix *matrix);
void __cdecl BuildRotationXRowVec(float angle, void *matrix);
void __cdecl BuildRotationYRowVec(float angle, void *matrix);
void __cdecl BuildRotationZRowVec(float angle, void *matrix);
void __cdecl TransposeMatrix3x3(void *src, void *dst);
void __cdecl Gfx_RebuildContextScreenTransform(void *slot, void *context);
/* dx_stat.cpp */
void __cdecl Gfx_StatsClearRecord(void *recordBlock);
/* g_drv.cpp */
void __cdecl Gfx_ApplyStateUntexturedCall(void *state, struct GfxEngineContext *context);
void __cdecl Gfx_ApplyStateTexturedCall(void *state, void *context);
/* graphics_driver_context_fields.cpp */
int __cdecl GetRenderStateObject(void *context);
/* graphics_vertex_color_arithmetic.cpp */
void __cdecl Gfx_LightPolygonSet(struct GfxPolygonSet *polygonSet, struct GfxLightPolygonSet *lightSet);
/* graphics_matrix_transform_asm.cpp */
void __cdecl SetIdentityMatrix4x4(void *matrix);
void __cdecl ClearMatrixTranslationAndW(void *m);
void __cdecl MultiplyMatrix4x4Core(void *a, void *b, void *out);
void __cdecl MultiplyMatrix4x4ByTranspose(void *a, void *b, void *out);
void __cdecl MultiplyMatrix4x4Full(void *a, void *b, void *out);
void __cdecl ConcatAffineMatrix4x4(void *a, void *b, void *out);
void __cdecl RotateVector3RowVec(void *matrix, void *point, void *out);
void __cdecl TransformPointByMatrix4x4(void *matrix, void *point, void *out);
void __cdecl TransformPoint3RowVec(void *matrix, void *point, void *out);
void __cdecl RotateVector3ColVec(void *matrix, void *point, void *out);
/* psx.cpp */
void __cdecl GteSVectorToFloat3(void *in, void *out);
void __cdecl GtePsxMatrixToFloat4x4(void *src, void *mm);
void __cdecl GtePsxMatrixToFloat4x4ColVec(void *src, void *mm);
void __cdecl GtePsxMatrixToFloat4x4Transposed(void *src, void *mm);
void __cdecl CrossProductFixed12(struct VECTOR *vecA, struct VECTOR *vecB, struct VECTOR *result);

void __cdecl DestroyRenderObject(void *renderObject);
struct GfxPolygonSet *__cdecl GfxLoadPolygonSet(struct GfxMatrixArray *matrices, struct PolygonHundredState *groupState, struct PolygonFileArrays *lightModel, struct GfxPolygonSetSpec *spec, struct GfxEngineContext *context);
void __cdecl Gfx_ForEachPolygonSetPart(DxGroupPartCallback callback, int model);

/* C++ TUs declare local extern "C" overrides for these 4 callees where retail callers push narrower/extra args. */
#ifndef __cplusplus
void __cdecl SetPerspective(float fov, float nearPlane, float farPlane, float offsetX, float offsetY, float viewWidth, float viewHeight, struct GfxEngineContext *context);
void __cdecl Gfx_DestroyPresetStateTable(struct GfxPresetPolygonSetTable *table);
int __cdecl Gfx_AllocUntexturedHundred(int gouraud, struct GfxTexturedHundredOptions *desc);
void __cdecl Gfx_StatsDrawTextRows(int context, int baseY, int callbackArg, void(__cdecl *arg4)(int, int, const char *, int), struct GfxStatsRecord *statsRecord);
#endif
void __cdecl Gfx_SetBlendFactorQuad(float first0, float second0, float first1, float second1, float first2, float second2, float first3, float second3);
int __cdecl Gfx_WritePaletteEntriesCall(int startIndex, int count, int entries, int flags, int palette, int extra);

/* dx_dbg.cpp */
int __cdecl CheckDirectDrawResult(int result, const char *file, int line);
/* g_drv.cpp */
void __cdecl Gfx_SetRenderStateCall(int state, int value, struct GfxEngineContext *context);
void __cdecl Gfx_SetScenePassCall(int pass, struct GfxEngineContext *context);
/* sw_gfx_raster_line_draw.cpp */
extern int g_CharIdToSaveSlot[14];

#ifdef __cplusplus
}

/* texcache.cpp: the texture cache, keyed by (texture header, palette). Find and Insert have C
 * linkage, the rest C++ linkage, as texcache.cpp defines them. */
extern "C" struct IDirectDrawSurface *__cdecl Gfx_TextureCacheFindSurface(struct GfxTexHeader *header, unsigned int palette);
extern "C" bool __cdecl Gfx_TextureCacheInsertSurface(struct GfxTexHeader *header, unsigned int palette, struct IDirectDrawSurface *surface);
void __cdecl Gfx_TextureCacheReleaseAllSurfaces(void);
bool __cdecl Gfx_TextureCacheReleaseByTexture(struct GfxTexHeader *header);
bool __cdecl Gfx_TextureCacheEvictForBytes(unsigned int needed);
#endif

#endif /* LIB_API_GRAPHICS_H */

/* include/ff7/api/chocobo.h: the functions and objects ff7/chocobo defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_CHOCOBO_H
#define FF7_API_CHOCOBO_H

#ifdef __cplusplus
extern "C" {
#endif
struct ChocoboSpriteRecord;
struct ChocoboCourseWaypoint;
struct ChocoboTrackData;
struct ChocoboFadeHistory;
extern struct ChocoboSpriteRecord g_ChocoboCourseRecord0;
struct GfxEngineContext;
struct ChocoboRacer;
struct ChocoboHudRenderObject;
struct PsxSVectorZ32;
struct ChocoboBetSlot;
struct ChocoboResultUiEntry;
struct ChocoboRacerEntity;
struct ChocoboBlackSpriteRect;
struct ChocoboFlatSpriteRect;
struct ChocoboQuadUvs;
struct ChocoboRacerEntry;
struct ChocoboShortPointDepth;
struct ChocoboRecordTail;
struct PfileLoadOptions;
struct ChocoboSpriteObject;
struct PolygonSkelHierarchy;
struct ChocoboRaceState;
struct ChocoboQuadSprite;
struct ChocoboCameraPos;
struct ChocoboRaceCameraPos;
struct PolygonAnimPlayer;
struct ChocoboRaceAssetState;
struct ChocoboPairResult;
struct ChocoboResultSpriteTable;
struct ChocoboSpriteQuad;

/* ch_app.cpp */
extern struct Timestamp64 g_ChocoboFrameStartTimestamp;
extern int g_ChocoboUiContext;
extern int g_ChocoboUiScalingMode;
extern int g_ChocoboViewportX;
extern int g_ChocoboViewportY;
extern int g_ChocoboViewportWidth;
extern int g_ChocoboViewportHeight;
extern int g_ChocoboUiScaleFactor;
extern int g_ChocoboViewportOffsetX;
extern int g_ChocoboViewportOffsetY;
extern int g_ChocoboViewportOffsetBase;
extern int g_ChocoboBetScreenLabelCount;
extern int g_ChocoboCourseSpriteSize;
extern int g_ChocoboScreenReference;
extern struct ChocoboSpriteRecord *g_ChocoboBetScreenLabels;
extern struct ChocoboSpriteRecord *g_ChocoboResultScreenLabels;
extern struct ChocoboSpriteRecord *g_ChocoboBetScreenPanels;
extern struct ChocoboSpriteRecord *g_ChocoboResultScreenPanels;
extern struct ChocoboSpriteRecord *g_ChocoboBetRacerSprites;
extern struct ChocoboResultUiEntry *g_ChocoboResultLayout;
extern struct ChocoboSpriteRecord *g_ChocoboHudSpriteA;
extern struct ChocoboSpriteRecord *g_ChocoboHudSpriteB;
extern void(__cdecl *g_ChocoboDrawNumberFn)(unsigned int number, int x, int y);
extern int g_ChocoboFrameLate;
extern int g_ChocoboExitFlag;
extern int g_ChocoboFramePacingOn;
extern int g_ChocoboCameraSmoothedNode;
extern int g_ChocoboCameraTargetSegment;
extern int g_ChocoboCameraClearedWord;
extern int g_ChocoboRacerScreenDepth[6];
extern int g_ChocoboFrameCounter;
extern int g_ChocoboRacerStepDivisor;
extern int g_ChocoboRacerOnScreen[6];
extern int g_ChocoboSkipPromptActive;
extern int g_ChocoboRouteEventRacer;
extern struct ChocoboRaceAssetState g_ChocoboFlags;
extern int g_ChocoboCameraInitIndex;
extern int g_ChocoboCameraSnapFlag;
extern int g_ChocoboRaceTimer;
extern int g_ChocoboCourseSegmentCount;
extern int g_ChocoboDashActiveFlag;
extern int g_ChocoboFadeLevel;
extern char *g_ChocoboMarkerYTable;
extern int g_ChocoboRaceAdvanceEnabled;
extern int g_ChocoboRaceResetSentinel;
extern int g_ChocoboPrevInputLatch;
extern int g_ChocoboSpriteClearedWord;
extern int g_ChocoboSubmittedRecordCount;
extern struct ChocoboRaceState g_ChocoboCurrentRouteEvent;
extern int g_ChocoboCameraBaseSegment;
extern int g_ChocoboPlayerRides;
extern struct ChocoboTrackData *g_ChocoboTrackData;
extern int g_ChocoboRaceClass;
extern int g_ChocoboRaceWinnerIndex;
extern unsigned char g_ChocoboCameraLookAtMatrix[30];
extern struct ChocoboRacer g_ChocoboSelectedRacerState;
extern int g_ChocoboFadeStep;
extern int g_ChocoboStartTimer;
extern int g_ChocoboSelectedRacerIndex;
extern struct ChocoboCourseWaypoint *g_pChocoboCourseWaypoints;
extern int g_ChocoboLateGamePrizesEnabled;
void __cdecl Mini_ChocoboApplyUiScalingModeConfig(int context, int mode);
void __cdecl Mini_ChocoboSetViewportRect(int viewX, int viewY, int viewWidth, int viewHeight);
struct ChocoboModelRecord;
void __cdecl Mini_ChocoboFreeModelRecord(struct ChocoboModelRecord *record, void *context);
int __cdecl Mini_ChocoboAllocModelRecord(void);
void __cdecl Mini_ChocoboSetSoundCommandBlockPointer(void);
void __cdecl Mini_ChocoboEnter(struct GfxEngineContext *ctx);
void __cdecl Mini_ChocoboSetupCamera(void);
void __cdecl Mini_ChocoboFreeTrackData(void);
void __cdecl Mini_ChocoboExit(void);
void __cdecl Mini_ChocoboDrawDebugText(int x, int y, const char *text, void *dc);
void __cdecl Mini_ChocoboFrame(struct GfxEngineContext *ctx);
void __cdecl Mini_ChocoboStepFrame(struct GfxEngineContext *ctx);
void __cdecl Mini_ChocoboMouse(void);
void __cdecl Mini_ChocoboKeyboard(void);
void __cdecl Mini_ChocoboInitEngineContextAndLoadAppCfg(int value, int param);
void __cdecl Mini_ChocoboDebugOverlayNoOp(struct GfxEngineContext *ctx);

/* ch_chr.cpp */
extern struct ChocoboRacerEntry *g_pChocoboRacerTable;
int __cdecl Mini_ChocoboFreeRacers(void);
extern char *g_pChocoboSpriteNames0[7];
extern char *g_pChocoboSpriteNames1[7];
extern char *g_pChocoboSpriteNames2[7];
extern char *g_pChocoboSpriteNames3[7];
extern char *g_pChocoboSpriteNames4[36];
extern char *g_pChocoboSpriteNames5[36];
extern char *g_pChocoboSpriteNames6[36];
extern char *g_pChocoboSpriteNames7[36];
extern int g_ChocoboRacerLut0[256];
extern int g_ChocoboRacerLut1[90];
void __cdecl Mini_ChocoboRaceCreateCourseSpriteObjects(struct PfileLoadOptions *loadOptions, struct GfxEngineContext *engine, int variant);
void __cdecl Mini_ChocoboRaceDestroyCourseSpriteObjects(void);
void __cdecl Mini_ChocoboClearSpriteObjectDirtyFlags(void);
void __cdecl Mini_ChocoboEmitSpriteObjectsGroup0(struct GfxEngineContext *renderContext);
void __cdecl Mini_ChocoboEmitSpriteObjectsGroup1(struct GfxEngineContext *renderContext);

/* ch_ddraw.cpp */
extern int g_ChocoboDirectDraw;
extern int g_ChocoboPrimarySurface;
void __cdecl Mini_ChocoboCacheDDrawIfaces(struct GfxEngineContext *engineCtx);
int __cdecl Mini_ChocoboInitGraphicsDevice(void);
void __cdecl Mini_ChocoboShutdownGraphicsDevice(void);
int __cdecl Mini_ChocoboCachePixelFormat(void *engineCtx);
int __cdecl Mini_ChocoboPresentFrame(void);
int __cdecl Mini_ChocoboAllocSurface16Bit(int width, int height);

/* ch_init.cpp */
extern char g_ChocoboAssetDirPath[256];
extern int g_ChocoboRacePhase;
extern int g_ChocoboHudPlayerRides;
extern int g_ChocoboHudRaceClass;
extern int g_ChocoboCourseVariant;
extern int g_ChocoboRaceHeapBlock;
extern struct ChocoboSpriteRecord g_ChocoboCameraRecord;
extern char *g_pChocoboWatBuffer;
extern struct Timestamp64 g_ChocoboTimestampNow;
void __cdecl Mini_ChocoboRaceSetAssetDirPath(char *src);
void __cdecl Mini_ChocoboRaceInitFromSavemap(void);
void __cdecl Mini_ChocoboRaceSendSoundCommand23(void);
void __cdecl Mini_ChocoboRaceInitRenderAndAssets(struct GfxEngineContext *engineCtx, void *loadOptions);
void __cdecl Mini_ChocoboRaceInitRacerModels(int course);
char *__cdecl Mini_ChocoboLoadFile(char *path);
void __cdecl Mini_ChocoboLoadAllAssets(int variant, int unused);
void __cdecl Mini_ChocoboBuildFloatVertexTable(void);
void __cdecl Mini_ChocoboRaceBuildCourseTriangleVertices(void);
void __cdecl Mini_ChocoboRaceBuildCourseQuadVertices(void);
void __cdecl Mini_ChocoboRaceInitHookNoOp(void);

/* chocobo_input_latches.cpp */
extern unsigned int g_ChocoboPackedInputLatches;
extern int g_ChocoboViewedRacer;
extern struct ChocoboRacerEntity *g_pChocoboSoundCommand;
void __cdecl Mini_ChocoboClearInputLatches(void);
void __cdecl Input_ApplyAltKeyboardBindings_B(void);
int __cdecl Mini_ChocoboGetBindingInputMask(int index);
void __cdecl Mini_ChocoboPollInputLatches(void);
int __cdecl Mini_ChocoboIsInputPressed(int mask);
int __cdecl Mini_ChocoboIsInputHeld(int mask);
void __cdecl Mini_ChocoboPackInputLatches(void);

/* chocobo_course_render_objects.cpp */
extern int g_ChocoboAltUvSelectorCodes[53];
extern struct GfxRenderObject *g_ChocoboSubmitTriangleRenderObj;
extern int g_ChocoboSubmitTrianglesActive;
extern int g_ChocoboSubmitTrianglesDirty;
extern int g_ChocoboSubmitTrianglesGroup;
extern struct GfxRenderObject *g_ChocoboEmitQuadRenderObj1;
extern int g_ChocoboEmitQuadActive1;
extern int g_ChocoboEmitQuadDirty1;
extern int g_ChocoboEmitQuadGroup1;
extern struct GfxRenderObject *g_ChocoboEmitQuadRenderObj2;
extern int g_ChocoboEmitQuadActive2;
extern int g_ChocoboEmitQuadDirty2;
extern int g_ChocoboEmitQuadGroup2;
extern struct GfxRenderObject *g_ChocoboEmitTriangleRenderObj1;
extern int g_ChocoboEmitTriangleActive1;
extern int g_ChocoboEmitTriangleDirty1;
extern int g_ChocoboEmitTriangleGroup1;
extern struct GfxRenderObject *g_ChocoboEmitTriangleRenderObj2;
extern int g_ChocoboEmitTriangleActive2;
extern int g_ChocoboEmitTriangleDirty2;
extern int g_ChocoboEmitTriangleGroup2;
extern int g_ChocoboSpriteRecordMap;
extern int g_ChocoboSpriteRecordCount;
extern int g_ChocoboCourseSubmitGuard;
void __cdecl Mini_ChocoboCreateFixedRenderObjects(struct GfxEngineContext *engineResource, void *optionBuffer);
void __cdecl Mini_ChocoboDestroyFixedRenderObjects(void);
void __cdecl Mini_ChocoboResetDirtyFixedRenderObjects(void);
void __cdecl Mini_ChocoboRaceSubmitCourseTriangles(void);
void __cdecl Mini_ChocoboRaceSubmitCourseRingSpan(void);
void __cdecl Mini_ChocoboRaceSubmitCourseSegmentRecords(int startIndex, int endIndex);
int __cdecl Mini_ChocoboRaceEmitCourseSpriteQuad(void *cameraMatrix, short spriteIndex, int unusedCount);
int __cdecl Mini_ChocoboGetAltUvMode(int spriteIndex);
int __cdecl Mini_ChocoboRaceEmitCourseQuad(int cameraMatrix, int segmentIndex, int unusedCount, int flags);
int __cdecl Mini_ChocoboRaceEmitCourseTriangle(int cameraMatrix, int triangleIndex, int unusedCount, int flags);

/* chocobo_hud_render_objects.cpp */
extern int g_ChocoboRetailFlag;
extern int g_ChocoboHudRenderObject24;
extern int g_ChocoboHudRenderObject24Active;
extern int g_ChocoboHudRenderObject24SlotUsed;
extern int g_ChocoboHudRenderObject26;
extern int g_ChocoboHudRenderObject26Active;
extern int g_ChocoboHudRenderObject26SlotUsed;
extern int g_ChocoboHudRenderObject26Ready;
extern int g_ChocoboHudObj;
extern int g_ChocoboHudObjInUse;
extern int g_ChocoboHudObjDirty;
extern int g_ChocoboHudObjCount;
void __cdecl Mini_ChocoboCreateHudRenderObjects(struct GfxEngineContext *engineResource, struct PfileLoadOptions *optionBuffer, int includeAll, int textureIndex);
void __cdecl Mini_ChocoboDestroyHudRenderObjects(void);
void __cdecl Mini_ChocoboEmptyDirtyHudRenderObjectBatches(void);

/* chocobo_engine_frame_sprites.cpp */
extern struct GfxMatrix4x4 g_ChocoboContextMatrix;
void __cdecl Mini_ChocoboSetEngineContext(int engineContext);
void __cdecl Mini_ChocoboGetEngineContext(void);
void __cdecl Mini_ChocoboRequestQuit(int quitCode);
void __cdecl Mini_ChocoboDestroyEngineContext(void);
void __cdecl Mini_ChocoboWaitFrameSlice(struct GfxEngineContext *context);
void __cdecl Mini_ChocoboTick(struct GfxEngineContext *context);
void __cdecl Mini_ChocoboSeedRandomFromContext(struct GfxEngineContext *context);
void __cdecl Mini_ChocoboCreateEngineContext(void);
void __cdecl Mini_ChocoboInitEngineContextWrapper(int context, int cfg);
void __cdecl Mini_ChocoboSetRecordTailFields(struct ChocoboRecordTail *record, int value0, int value1, int value2);
int __cdecl Mini_ChocoboSetContextMatrix(struct GfxEngineContext *context, int matrix);
int __cdecl Mini_ChocoboLoadEntitySkeletonIfAbsent(struct GfxEngineContext *context, void *entity, int blendMode);
int __cdecl Mini_ChocoboLoadHrcAndAttach(int hrcName, int entity, int context, struct GfxEngineContext *engine);
int __cdecl Mini_ChocoboCreateAnimPlayerAndBind(int context, struct PolygonSkelHierarchy *hrc, struct GfxEngineContext *engine, int animIndex);
int __cdecl Mini_ChocoboCompileAndLoadAnimation(void *context, int srcPath);
void __cdecl Mini_ChocoboBindAnimation(int ignored, int animation, struct PolygonAnimPlayer *player);
int __cdecl Mini_ChocoboLoadHrcAndCreateAnimPlayer(int owner, int animIndex, int context, struct GfxEngineContext *engine, int hrcName, int entity);
int __cdecl Mini_ChocoboSelectAnimationIndex(struct ChocoboRacerEntry *record, int animIndex);
void __cdecl Mini_ChocoboEmitSpriteObject(struct ChocoboSpriteObject *sprite);
void __cdecl Mini_ChocoboEmitSpriteBatchQuad(struct ChocoboSpriteRecord *hudState);
void __cdecl Mini_ChocoboEmitFlatSpriteRect(struct ChocoboFlatSpriteRect *rect);
void __cdecl Mini_ChocoboEmitSpriteBatchCornerQuad(struct ChocoboSpriteRecord *cornerSprite);
void __cdecl Mini_ChocoboEmitInsetRect(struct ChocoboSpriteRecord *rect);
void __cdecl Mini_ChocoboEmitSpriteBatchUvQuad(struct ChocoboSpriteQuad *quad);
void __cdecl Mini_ChocoboEmitSpriteObjectDoubledColor(struct ChocoboSpriteObject *object);
void __cdecl Mini_ChocoboEmitBlackSpriteRect(struct ChocoboBlackSpriteRect *rect);
void __cdecl Mini_ChocoboResetPerFrameDrawState(struct GfxEngineContext *context);
void __cdecl Mini_ChocoboDrawFrame(void);
void __cdecl Mini_ChocoboSyncDisplayEntryToObject(struct ChocoboRacerEntry *entry);
void __cdecl Mini_ChocoboConvertPsxMatrixToFloat4x4(struct PsxMatrix *psxMatrix, void *dstMatrix);
void __cdecl Mini_ChocoboTransformPointXyzByMatrix(int matrix, int x, int y, int z, int dst);
void __cdecl Mini_ChocoboSnapshotViewMatrix(struct GfxEngineContext *context);
void __cdecl Mini_ChocoboAppendProjectedTriangle(int value0, int value1, int value2, int point0, int point1, int point2, int matrix, struct GfxRenderObject *batch);
void __cdecl Mini_ChocoboEmitTriangleRecord(int matrix, struct GfxRenderObject *batch, int record);
void __cdecl Mini_ChocoboEmitRawTriangle(int matrix, struct GfxRenderObject *batch, struct ChocoboShortPointDepth *point0, struct ChocoboShortPointDepth *point1, struct ChocoboShortPointDepth *point2, int color0, int color1, int color2);
void __cdecl Mini_ChocoboAppendProjectedQuad(int value0, int value1, int value2, int value3, int point0, int point1, int point2, int point3, int matrix, struct GfxRenderObject *batch);
void __cdecl Mini_ChocoboEmitQuadRecord(int matrix, struct GfxRenderObject *batch, int record);
void __cdecl Mini_ChocoboSubmitShortXyzQuadColors(int matrix, struct GfxRenderObject *batch, struct ChocoboShortPointDepth *point0, struct ChocoboShortPointDepth *point1, struct ChocoboShortPointDepth *point2, struct ChocoboShortPointDepth *point3, int color0, int color1, int color2, int color3);
void __cdecl Mini_ChocoboProjectQuadToBatch(struct ChocoboQuadUvs *quad, int matrix, struct GfxRenderObject *batch);
float __cdecl Mini_ChocoboShortPointProjectedDepth(struct ChocoboShortPointDepth *point, int matrix);
void __cdecl Mini_ChocoboSubmitShortQuadWithUvs(int matrix, struct GfxRenderObject *batch, struct ChocoboShortPointDepth *point0, struct ChocoboShortPointDepth *point1, struct ChocoboShortPointDepth *point2, struct ChocoboShortPointDepth *point3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3);
void __cdecl Mini_ChocoboStartRaceSequence(struct GfxEngineContext *renderSetup, void *assetSetup);
void __cdecl Mini_ChocoboRunRaceResultLoop(void);
void __cdecl Mini_ChocoboAdvanceRingSpan(void);
void __cdecl Mini_ChocoboStepAnimatedRecordRange(int start, int end);
void __cdecl Mini_ChocoboResetRacerModelAngles(void);

/* chocobo_heading_angles.cpp */
short __cdecl Mini_ChocoboHeadingFromXZ(short dx, short dz);

/* chocobo_palette_load.cpp */
extern unsigned char g_ChocoboPaletteSource0[];
extern unsigned char g_ChocoboPaletteSource1[];
extern unsigned char g_ChocoboPaletteSource2[];
extern unsigned char g_ChocoboPaletteSource3[];
extern unsigned char g_ChocoboPaletteSource4[];
extern unsigned char g_ChocoboPaletteSource5[];
extern unsigned char g_ChocoboPaletteSource6[];
extern unsigned char g_ChocoboPaletteSource7[];
extern unsigned char g_ChocoboPaletteSource8[];
extern unsigned char g_ChocoboPaletteSource9[];
extern int g_ChocoboPaletteBuffer1[8];
extern unsigned char g_ChocoboGteMatrix[30];
void __cdecl Mini_ChocoboLoadPaletteTable(void);

/* chocobo_racer_state_update.cpp */
extern struct ChocoboClassStats g_ChocoboClassStats[4];
extern int g_ChocoboRaceState;
int __cdecl Mini_ChocoboGetRacerStatBaseByVariant(int variant);
void __cdecl Mini_ChocoboRaceBuildRacers(void);
void __cdecl Mini_ChocoboRaceApplyTwoRacerEventCodes(int racerIndex, int eventCode0, int eventCode1);
void __cdecl Mini_ChocoboRaceApplyRacerEventCode(int racerIndex, int actionCode);
void __cdecl Mini_ChocoboRaceRankAndUpdateRacers(void);
void __cdecl Mini_ChocoboRaceUpdateRacerAnimationAndCollisions(short racerIndex);
void __cdecl Mini_ChocoboRaceAdvanceRacerAlongCourse(short racerIndex);
void __cdecl Mini_ChocoboRaceSelectRacerStateIfChanged(int racerIndex);
void __cdecl Mini_ChocoboRaceSelectRacerStateAtRoutePoint(int racerIndex, int progress, int segment);
void __cdecl Mini_ChocoboRaceAdvanceSelectedRacerAlongRoute(int updateSpeed);

/* chocobo_course_sounds.cpp */
void *__cdecl Mini_ChocoboSelectCourseSounds(int course);
void __cdecl Mini_ChocoboLoadCourseSounds(void);
void __cdecl Mini_ChocoboReleaseCourseSounds(void);

/* chocobo_race_hud_submit.cpp */
extern unsigned char g_ChocoboHudSpriteRecord0[132];
extern unsigned char g_ChocoboHudSpriteRecord1[132];
extern unsigned char g_ChocoboHudSpriteRecord2[132];
extern unsigned char g_ChocoboHudSpriteRecord3[132];
extern int g_ChocoboIconOffsets[6];
extern struct ChocoboSpriteRecord g_ChocoboRacerTable[6];
extern struct ChocoboSpriteRecord g_ChocoboCourseRecord1;
extern struct ChocoboSpriteRecord g_ChocoboStaminaGaugeRecord;
extern struct ChocoboSpriteRecord g_ChocoboHudRacerRecords0[6];
void __cdecl Mini_ChocoboSubmitRaceHud(void);
void __cdecl Mini_ChocoboRaceInitLaneHudStatesFromSavedColor(void);

/* chocobo_race_camera.cpp */
extern unsigned char g_ChocoboCourseVariantTable0[384];
extern unsigned char g_ChocoboCourseVariantTable1[320];
extern short g_ChocoboDeferredCameraRacer;
extern short g_ChocoboDeferredCameraEvent;
extern unsigned char g_ChocoboViewScreenMatrix[64];
extern struct ChocoboCameraPos g_ChocoboCameraEyeGoal;
extern struct ChocoboRaceCameraPos g_ChocoboRaceCameraLookGoal;
extern int g_ChocoboCameraRouteReselectPending;
int __cdecl Mini_ChocoboGetCameraScriptByVariant(int variant);
void __cdecl Mini_ChocoboRaceSetCameraModeFromRouteEvent(short eventIndex, short racerIndex);
void __cdecl Mini_ChocoboBuildLookAtMatrix(struct PsxMatrix *outMatrix, struct SVECTOR *eyePos, struct SVECTOR *targetPos);
void __cdecl Mini_ChocoboRaceUpdateCameraTargets(void);
void __cdecl Mini_ChocoboRaceSelectCameraRacerState(void);
void __cdecl Mini_ChocoboRaceApplySmoothedCameraMatrix(void);
void __cdecl Mini_ChocoboRaceSetSceneryAnimationStep(short sceneryIndex);

/* chocobo_variant_table_select.cpp */
extern unsigned char g_ChocoboTrackTable40Variant0[240];
extern unsigned char g_ChocoboTrackTable40Variant1[30112];
int __cdecl Mini_ChocoboGetSplineTableByVariant(int variant);

/* chocobo_bet_and_result_screens.cpp */
extern unsigned char g_ChocoboBetBaseOffsets[16];
extern struct ChocoboFadeHistory g_ChocoboFadeQuadData;
extern int g_ChocoboBetGridRows[6];
extern int g_ChocoboBetScreenState;
extern int g_ChocoboWinningPairA;
extern int *g_pChocoboGil;
extern struct ChocoboQuadSprite g_ChocoboBetGridRecords0[5][3];
extern int g_ChocoboFirstPlaceRacer;
extern int g_ChocoboDebugTuneA;
extern int g_ChocoboDebugTuneB;
extern int g_ChocoboDebugTuneC;
extern struct ChocoboQuadSprite g_ChocoboBetGridRecords1[5][3];
extern int g_ChocoboBetCursorHighlight;
extern int g_ChocoboBetSlotIndex;
extern int g_ChocoboResultTimer;
extern int g_ChocoboBetGridRow;
extern int g_ChocoboBetGridCol;
extern int g_ChocoboWinningPairB;
extern struct ChocoboQuadSprite g_ChocoboBetGridRecords2[5][3];
extern struct ChocoboBetSlot *g_pChocoboBetTableBase;
extern int g_ChocoboBetMoveSoundPending;
extern int g_ChocoboBetClearedWord;
extern int g_ChocoboBetStepIndex;
void __cdecl Mini_ChocoboUnusedNoOp(void);
void __cdecl Mini_ChocoboRaceResetBetScreen(void);
void __cdecl Mini_ChocoboRaceResetDebugTuningValues(void);
void __cdecl Mini_ChocoboRaceUpdateBetScreen(void);
void __cdecl Mini_ChocoboDrawNumber1x(unsigned int number, int x, int y);
void __cdecl Mini_ChocoboDrawNumber2x(unsigned int number, int x, int y);
void __cdecl Mini_ChocoboRaceUpdateSelectedRacerModelEntries(int racerIndex);
void __cdecl Mini_ChocoboRaceBuildBetGrid(void);
void __cdecl Mini_ChocoboRaceUpdateBetGridCells(int showPairResults);
void __cdecl Mini_ChocoboRaceBeginResultScreen(void);
void __cdecl Mini_ChocoboRaceUpdateResultScreen(void);
void __cdecl Mini_ChocoboRaceDrawBetGridLegend(void);
void __cdecl Mini_ChocoboDrawEncodedText(int encodedText, int reserved, int x, int y);
void __cdecl Mini_ChocoboRaceUpdateFadeOverlay(void);

/* chocobo_bet_and_result_screens.cpp */
extern unsigned char g_ChocoboBetScreenLabels2x[780];
extern unsigned char g_ChocoboResultScreenLabels2x[780];
extern unsigned char g_ChocoboBetRacerSprites2x[420];
extern unsigned char g_ChocoboBetScreenLabels1x[708];
extern unsigned char g_ChocoboResultScreenLabels1x[780];
extern unsigned char g_ChocoboBetRacerSprites1x[420];
extern struct ChocoboPairResult g_ChocoboPairResultGridMap[15];
extern int g_ChocoboUiSpriteRecord[18];
extern int g_ChocoboSpriteQueueRequest[18];
extern int g_ChocoboDigitSprite2x0To6[18];
extern int g_ChocoboDigitSprite2x7To9[18];
extern int g_ChocoboDigitSprite1x[18];
extern struct ChocoboResultSpriteTable g_ChocoboResultDisplaySpriteRecords;
extern unsigned char g_ChocoboResultRacerSprites2x[420];
extern unsigned char g_ChocoboResultRacerSprites1x[420];
extern int g_ChocoboBetPrizeSaveIds[24];
extern unsigned char g_ChocoboResultScreenPanels2x[636];
extern unsigned char g_ChocoboResultScreenPanels1x[636];
extern unsigned char g_ChocoboBetScreenPanels2x[708];
extern unsigned char g_ChocoboBetScreenPanels1x[712];

/* chocobo_engine_frame_sprites.cpp */
extern unsigned char g_ChocoboTeiohName[8];
extern struct ChocoboBetSlot g_ChocoboBetChoiceTable0[23];
extern struct ChocoboBetSlot g_ChocoboBetChoiceTable1[21];
extern struct ChocoboBetSlot g_ChocoboBetChoiceTable2[16];
extern struct ChocoboBetSlot g_ChocoboBetChoiceTable3[10];
extern unsigned char g_ChocoboRacePrizeNames[384];
extern unsigned char g_ChocoboJockeyNames[328];
extern short g_ChocoboArcTanTable[8192];

extern struct ChocoboHudRenderObject g_ChocoboHudRenderObjects[24];
extern struct ChocoboRacer g_ChocoboRacers[6];
extern double g_ChocoboFrameDuration;

/* chocobo_bet_and_result_screens.cpp */
extern int g_ChocoboBetPrizes[3];
extern int g_ChocoboBetWork[3];

/* chocobo_heading_angles.cpp */
void __cdecl Mini_ChocoboHeadingFromPositionDelta(struct ChocoboShortPointDepth *fromPos, struct ChocoboShortPointDepth *toPos);

/* chocobo_race_camera.cpp */
extern struct SVECTOR g_ChocoboCameraSmoothedEye;
extern struct SVECTOR g_ChocoboCameraSmoothedLook;

struct ChocoboSpriteGroup;

/* chocobo_course_render_objects.cpp */
extern struct ChocoboSpriteGroup g_ChocoboSpriteRecords[36];

struct ChocoboHudLayoutEntry;
struct GfxMatrix4x4;

/* ch_app.cpp */
extern unsigned char g_ChocoboBetGrid[120];

/* ch_chr.cpp */
extern unsigned short g_ChocoboRacerCounts[2];
int __cdecl Mini_ChocoboBuildRaceObjects(struct PfileLoadOptions *options, struct GfxEngineContext *engine, int variant);

/* chocobo_race_camera.cpp */
extern struct GfxMatrix4x4 g_ChocoboViewMatrix;

/* chocobo_bet_and_result_screens.cpp */
extern struct ChocoboHudLayoutEntry g_ChocoboHudLayoutEntries[3];

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_CHOCOBO_H */

/* include/ff7/api/coaster.h: the functions and objects ff7/coaster defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_COASTER_H
#define FF7_API_COASTER_H

#ifdef __cplusplus
extern "C" {
#endif
struct CoasterSoundCommand;
struct CoasterActor;
struct CoasterNode;
extern struct CoasterNode g_CoasterRootSceneNode;
struct PsxMatrix32;
struct CoasterModelRecord;
struct CoasterPsxDataSet;
struct SVECTOR;
struct GfxEngineContext;
struct CoasterModelTemplate;
struct VECTOR;

/* coaster.cpp */
extern int g_coasterSubVolumeFactor;
extern int g_CoasterDisplayScaleY;
extern char DAT_00c3f6f0;
#define g_CoasterSfxAlternateToggle DAT_00c3f6f0
extern short DAT_00c3f6f4;
#define g_CoasterScorePopupValue DAT_00c3f6f4
extern struct CoasterSoundCommand DAT_00c3f700[3];
#define g_CoasterSoundCommandBuffer DAT_00c3f700
extern short DAT_00c3f724;
#define g_CoasterScorePopupTimer DAT_00c3f724
extern int g_CoasterScore;
extern int g_CoasterFadeNearDepth;
extern int g_CoasterFadeFarDepth;
extern int g_CoasterTrackCursor;
extern char g_CoasterPaused;
extern char g_CoasterRideActive;
extern int g_CoasterRideSpeed;
extern short g_CoasterKillScore;
extern char g_CoasterRideFinished;
extern int g_CoasterRidePosition;
extern int g_CoasterEngineVolume;
extern int DAT_00c3f6f8;
#define g_CoasterReticleScaledSize DAT_00c3f6f8
void __cdecl Mini_CoasterLoadData(void);
void __cdecl Mini_CoasterReleaseSfxAndPsxData(void);
void __cdecl Mini_CoasterOpenArchive(void);
void __cdecl Mini_CoasterStartMusic(void);
void __cdecl Mini_CoasterStopSoundLoops(void);
void __cdecl Mini_CoasterPlaySfxAlternating(short sfxId);
void __cdecl Mini_CoasterUpdateBeamLoopSound(short volume);
void __cdecl Mini_CoasterUpdateLoopVolumes(void);
void __cdecl Mini_CoasterStopMusic(void);

/* coaster_helpers.cpp */
extern int g_CoasterTriangleStorageBase;
extern unsigned char g_CoasterModelRecordPool[3200];
extern int g_CoasterVertexCount;
extern struct CoasterModelTemplate *g_CoasterModelTemplates;
extern int g_CoasterVertexStorageBase;
extern int g_CoasterModelRecordCount;
extern struct CoasterModelRecord *g_CoasterModelHandles[79];
extern struct CoasterModelRecord *g_CoasterCarModelHandle;
extern void *g_CoasterExplosionModel;
extern int g_CoasterTriangleCount;
void __cdecl Mini_CoasterInitModelPools(void);
struct CoasterModelRecord *__cdecl Mini_CoasterBuildModelFromTemplate(int templateIndex);
int __cdecl Mini_CoasterAllocModelRecord(void);
int __cdecl Mini_CoasterAllocVertexStorage(int vertexCount);
int __cdecl Mini_CoasterAllocTriangleStorage(int count);

/* coaster_clip_plane_tests.cpp */
extern struct CoasterHalfSpaceState g_CoasterClipPlanes;
void __cdecl Mini_CoasterBuildClipPlanes(void);
int __cdecl Mini_CoasterLeftClipPlaneDistance(int x, int y, int z);
int __cdecl Mini_CoasterRightClipPlaneDistance(int x, int y, int z);
int __cdecl Mini_CoasterSphereInsideLeftClipPlane(int, int, int, short radius);
int __cdecl Mini_CoasterSphereInsideClipPlane(int, int, int, short radius);

/* coaster_module_entry_frame.cpp */
extern float g_CoasterAccumulatedTime;
extern float g_CoasterSimulatedTime;
void __cdecl Mini_CoasterDebugTextOut(int x, int y, char *text, struct GfxEngineContext *ctx);
void __cdecl Mini_CoasterSetupCamera(struct GfxEngineContext *ctx);
void __cdecl Mini_CoasterSetViewport(struct GfxEngineContext *ctx);
void __cdecl Mini_SetBackgroundColorAndClear(int red, int green, int blue, int alpha);
void __cdecl Mini_CoasterEnter(struct GfxEngineContext *context);
void __cdecl Mini_CoasterExit(struct GfxEngineContext *context);
void __cdecl Mini_CoasterEmptyHook(void);
void __cdecl Mini_CoasterReturnToField(struct GfxEngineContext *context);
void __cdecl Mini_CoasterFrame(struct GfxEngineContext *ctx);
void __cdecl Mini_CoasterAccumulateStepTarget(struct GfxEngineContext *ctx);
void __cdecl Mini_CoasterTick(struct GfxEngineContext *context);
void __cdecl Mini_CoasterMouse(void);
void __cdecl Mini_CoasterKeyboard(int message, int key);

/* coaster_hud_draw_passes.cpp */
void __cdecl Mini_CoasterDrawHud(void);
void __cdecl Mini_CoasterDrawPausedHud(void);
void __cdecl Mini_CoasterSubmitRendererBatches(void);
void __cdecl Mini_CoasterShutdown(void);

/* coaster_scene_draw_camera.cpp */
extern int g_CoasterDrawThisTick;
extern int g_CoasterBeamLoopVolume;
extern int g_CoasterClipLeft;
extern int g_CoasterClipTop;
extern char g_CoasterSavedDirectory[200];
#ifdef __cplusplus
extern class CoasterRenderer *g_pCoasterRenderer;
#else
extern struct CoasterRenderer *g_pCoasterRenderer;
#endif
extern char g_CoasterRideMode;
extern int g_CoasterTrackLength;
extern int g_CoasterRailSetTable;
extern struct PsxMatrix32 g_CoasterCameraMatrixBuf;
void __cdecl Mini_CoasterInit(void);
void __cdecl Mini_CoasterDrawActorAndHitTest(struct CoasterNode *actor, struct CoasterActor *target);
void __cdecl Mini_CoasterDrawPlayerCar(struct CoasterNode *car);
void __cdecl Mini_CoasterDrawScorePopupModel(struct CoasterNode *sceneObj);
void __cdecl Mini_CoasterDrawSceneryStream(void);
void __cdecl Mini_CoasterDrawRailStream(void);
void __cdecl Mini_CoasterBuildCameraMatrix(struct PsxMatrix *m);
void __cdecl Mini_CoasterConcatCameraMatrix(struct PsxMatrix32 *rot, struct PsxMatrix32 *cam, struct PsxMatrix32 *out);
short *__cdecl Mini_CoasterEulerToPsxMatrix(short *euler, short *psxMatrix);
void __cdecl Mini_CoasterSampleTrack(unsigned int distance, int upOffset, struct VECTOR *posOut, struct SVECTOR *rotOut);
void __cdecl Mini_CoasterUpdateScorePopup(short model, short spinX, short spinY, short spinZ);
void __cdecl Mini_CoasterDrawFadeOverlay(unsigned char level);
void __cdecl Mini_CoasterDrawCrosshair(void);
void __cdecl Mini_CoasterDrawBeam(void);

/* coaster_input_poll.cpp */
extern int g_CoasterInputUnused;
extern int g_CoasterInputDirection;
extern int g_CoasterPauseButtonCounter;
extern int g_CoasterBeamFiringActive;
void __cdecl Mini_CoasterPollInput(void);

/* coaster_actor_pool_spawn.cpp */
extern short g_CoasterCarScreenX;
extern short g_CoasterCarPoint2ScreenX;
extern short g_CoasterCarPoint2ScreenY;
extern short g_CoasterCarScreenY;
extern signed char g_CoasterShotFlag;
extern char g_CoasterShotCooldown;
extern short g_CoasterCrosshairX;
extern short g_CoasterCrosshairY;
extern short g_CoasterShotCharge;
extern short g_CoasterChargeCenterX;
extern short g_CoasterBeamCharge;
extern short g_CoasterReticleX;
extern short g_CoasterReticleY;
extern char g_CoasterBeamIndex;
void __cdecl Mini_CoasterResetActorPool(void);
void __cdecl Mini_CoasterResolvePath(unsigned char modelIndex, int mode);
void __cdecl Mini_CoasterSampleAnimPath(unsigned int cursor, struct SVECTOR *path, struct VECTOR *posOut, char rawSigns);
short __cdecl Mini_CoasterSpawnActor(struct CoasterActor *spawnRecord, short parentSlot);
void __cdecl Mini_CoasterDespawnActor(struct CoasterActor *actor);
short __cdecl Mini_CoasterAllocActorSlot(void);
void __cdecl Mini_CoasterFreeActorSlot(short value);
void __cdecl Mini_CoasterRunSpawnScript(void);
void __cdecl Mini_CoasterSpawnScripted(short x, short y, short z, short behaviourType, short modelIndex);
void __cdecl Mini_CoasterSpawnEffect(short x, short y, short z, short behaviourType, short modelIndex);
void __cdecl Mini_CoasterUpdateActors(void);
void __cdecl Mini_CoasterApplyBeamDamage(struct CoasterActor *target);
void __cdecl Mini_CoasterAwardKill(struct CoasterActor *target);

/* coaster_track_stream_lists.cpp */
extern unsigned short *g_pCoasterRailRemoveTable;
extern unsigned short *g_pCoasterSceneryStreamTable;
extern unsigned int g_CoasterStreamPosition;
extern unsigned short *g_pCoasterSceneryRemoveTable;
extern unsigned short g_CoasterSceneryCount;
extern int g_CoasterCellDelta;
extern unsigned short g_CoasterRailListHead;
extern unsigned short g_CoasterSceneryListTail;
extern unsigned short g_CoasterRailCount;
extern unsigned char g_CoasterStreamPrimePending;
extern unsigned short g_CoasterRailListTail;
extern unsigned short *g_pCoasterRailStreamTable;
extern unsigned int g_CoasterPrevStreamPosition;
extern unsigned short g_CoasterSceneryListHead;
void __cdecl Mini_CoasterResetStreamLists(void);
void __cdecl Mini_CoasterAdvanceTrackCursor(int speed);
void __cdecl Mini_CoasterRetireStreamedEntries(void);
void __cdecl Mini_CoasterSceneryListInsert(unsigned short entryIndex);
void __cdecl Mini_CoasterSceneryListRemove(unsigned short nodeIndex);
void __cdecl Mini_CoasterRailListInsert(unsigned short entryIndex);
void __cdecl Mini_CoasterRailListRemove(unsigned short nodeIndex);

/* coaster_scene_node_pool.cpp */
extern struct CoasterNode g_CoasterDepthTailNodes[10];
extern unsigned char g_CoasterSceneNodePool[5600];
void __cdecl Mini_CoasterResetSceneNodePool(void);
void __cdecl Mini_CoasterInitLinkedNode(struct CoasterNode *value, int flags);
int __cdecl Mini_CoasterCreateSceneObject(short modelId, void *parentNode, int posX, int posY, int posZ, short rotX, short rotY, short rotZ);
void __cdecl Mini_CoasterDestroySceneObject(void *handle);
short __cdecl Mini_CoasterPopSceneNode(void);
void __cdecl Mini_CoasterPushSceneNode(short index);
void __cdecl Mini_CoasterSceneNodeUnlink(struct CoasterNode *node);

/* coaster_track_tables_advance.cpp */
extern int *g_pCoasterTrackData;
extern struct SVECTOR *g_pCoasterRailSet;
extern int g_CoasterCameraPositionXCopy;
extern int g_CoasterCameraPositionYCopy;
extern int g_CoasterCameraPositionZCopy;
extern int g_CoasterCameraPositionPadCopy;
extern struct VECTOR g_CoasterCameraPosition;
extern int g_CoasterRailInitX;
extern int g_CoasterRailInitY;
extern int g_CoasterRailInitZ;
extern int g_CoasterDebugTuneSquareCircle;
extern int g_CoasterDebugTuneR1R2;
extern int g_CoasterDebugTuneCrossTriangle;
void __cdecl Mini_CoasterResolveTrackTables(void);
void __cdecl Mini_CoasterAdvanceRide(void);
void __cdecl Mini_CoasterSelectRailSet(unsigned int railSetIndex);

/* psxdata_c.cpp */
void __fastcall Mini_CoasterFreePsxDataSet(struct CoasterPsxDataSet *state);

/* coaster.cpp */
extern char g_coasterFreezeState;


/* coaster_scene_node_pool.cpp */
void __cdecl Mini_CoasterSceneNodeAttachChild(struct CoasterNode *node, struct CoasterNode *parent);

struct CoasterPsxDataSet;
struct MFVec32;

/* coaster.cpp */
struct CoasterScorePopupState;
extern struct CoasterScorePopupState DAT_00c3f728;
#define g_CoasterScorePopupState DAT_00c3f728
extern struct CoasterPsxDataSet DAT_00c3f738;
#define g_CoasterPsxDataSet DAT_00c3f738

/* coaster_clip_plane_tests.cpp */
int __cdecl Mini_CoasterSphereInsideBothClipPlanes(struct MFVec32 *, short radius);

/* coaster_clip_plane_tests.cpp */
int __cdecl Mini_CoasterPointInsideClipPlanesVec32(struct MFVec32 *v);

struct CoasterStreamNode;
/* coaster_track_stream_lists.cpp */
extern struct CoasterStreamNode g_CoasterRailStreamNodes[9000];
extern struct CoasterStreamNode g_CoasterSceneryStreamNodes[12000];

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_COASTER_H */

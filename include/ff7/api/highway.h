/* include/ff7/api/highway.h: the functions and objects ff7/highway defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_HIGHWAY_H
#define FF7_API_HIGHWAY_H

#ifdef __cplusplus
extern "C" {
#endif
struct HighwayColorSourceTriplet;
struct HighwayColorTriplet;
struct HighwayDepthPoint;
struct HighwayDepthTransform;
struct HighwayEffectPoolEntry;
struct HighwayEffectSlotInitA;
struct HighwayEndPoint;
struct HighwayLerpVector;
struct HighwayModelNode;
struct HighwayIndex;
struct HighwayRenderSlotActor;
struct PolygonRootMatrix;
struct HighwaySoundCommand;
struct HighwaySceneBlock2;
struct HighwayLerpTarget;
struct HighwayInterpolatedVector;
struct HighwayArray;
struct HighwayFieldManager;
struct HighwayFileBlock3;
struct HighwayModeController;

/* highway.cpp */
extern struct HighwayIndex *g_pHighwayIndexBlock;
extern struct HighwayArray *g_HighwayRosterBlock;
void __fastcall Mini_HighwayListNodeUnlink(struct HighwayModelNode *self);
void __fastcall Mini_HighwayActorUnlinkAndRelease(void *self);
void __cdecl Mini_HighwayPsxMatrixToFloatTransposed(int psxMatrix, struct PolygonRootMatrix *dst);
short __fastcall Mini_HighwayIndexPopFreeSlot(struct HighwayIndex *self);

/* highway_hud_render_setup.cpp */
extern const char *g_HighwaySpriteTimNames1[6];

/* highway_minigame_lifecycle.cpp */
extern char g_HighwaySavedDirectory[260];
extern int g_HighwayCountdown;
extern struct HighwayFileBlock3 *g_pHighwayFileBlock;
extern char *g_HighwayDataDirectory;
void __cdecl Mini_HighwayEnter(void);
void __cdecl Mini_HighwayExit(struct GfxEngineContext *context);
void __cdecl Mini_HighwayReturnToField(struct GfxEngineContext *context);
void __cdecl Mini_HighwaySetBackgroundColorAndClear(int red, int green, int blue, int alpha);
void __cdecl Mini_HighwayFrame(struct GfxEngineContext *context);
void __cdecl Mini_HighwayMouse(void);
void __cdecl Mini_HighwayKeyboard(void);

/* highway_effect_records_pool.cpp */
extern int g_HighwaySpawnEffectX;
extern int g_HighwaySpawnEffectY;
extern int g_HighwaySpawnEffectZ;
extern int g_HighwaySpawnEffectKind;
extern int g_HighwaySpawnEffectTypeId;
extern int g_HighwaySpawnEffectInitialized;
extern int g_HighwaySpawnEffectType;
extern short g_HighwayActiveEffectCount;
extern short g_HighwayEffectFreeHead;
void __cdecl Mini_HighwayInitEffectPool(void);
short __cdecl Mini_HighwayCreateEffectRecord(struct HighwayEffectPoolEntry *sourceRecord, short parentSlot);
void __cdecl Mini_HighwayReleaseEffectRecord(struct HighwayEffectPoolEntry *record);
short __cdecl Mini_HighwayAllocEffectSlot(void);
void __cdecl Mini_HighwayFreeEffectSlot(short slot);
void __cdecl Mini_HighwayUpdateEffectRecords(void);
int __cdecl Mini_HighwaySpawnEffect(int posX, int posY, int posZ, short rotY, short rotZ);

/* highway_slot_actors_update_render.cpp */
extern unsigned char g_HighwaySlotActors[8480];
extern struct HighwayFieldManager *g_pHighwayFieldManager;
extern int g_pHighwayRenderBlock;
extern struct HighwayModeController *g_pHighwayModeBlock;
void __cdecl Mini_HighwayUpdateAndRenderActors(int slotBase, struct GfxEngineContext *context);
void __cdecl Mini_HighwayResetSlot(int slot);
int __cdecl Mini_HighwayLoadAnimations(int animSet);
void __cdecl Mini_HighwayReleaseSkeletonList(void);
int __cdecl Mini_HighwayInitActorGroups(int animSet);
void __cdecl Mini_HighwayCreateSlotActors(int slot);
void __cdecl Mini_HighwayClampActiveSlots(void);
void __cdecl Mini_HighwayClampSlotPosition(int slot);
void __cdecl Mini_HighwayUpdateSpawns(void);
void __cdecl Mini_HighwaySpawnSlot(int slot);
void __cdecl Mini_HighwayDespawnSlot(int slot);
void __cdecl Mini_HighwayUpdateActiveSlotMovement(void);
void __cdecl Mini_HighwayUpdateSlotMovement(int slotIndex);
void __cdecl Mini_HighwayStepSlotAndClampPosition(int slotIndex);
void __cdecl Mini_HighwayUpdateActiveSlotTransforms(void);
void __cdecl Mini_HighwayUpdateSlotTransform(int slotIndex);
void __cdecl Mini_HighwayRenderActiveSlotActors(void);
void __cdecl Mini_HighwayRenderSlotActor(int slotIndex);
void __cdecl Mini_HighwayUpdateActiveSlotAnimations(void);
void __cdecl Mini_HighwayUpdateSlotAnimationCounters(int slotIndex);
int __cdecl Mini_HighwayAngleBetweenRecords(struct HighwayRenderSlotActor *fromRecord, struct HighwayRenderSlotActor *toRecord);
int __cdecl Mini_HighwayActorDistance(struct HighwayRenderSlotActor *groupA, struct HighwayRenderSlotActor *groupB);
void __cdecl Mini_HighwayActorComputeEndpoints(short slotIndex, struct HighwayEndPoint *startPoint, struct HighwayEndPoint *endPoint, void *flag);
void __cdecl Mini_HighwayActorsCollidePairs(void);
void __cdecl Mini_HighwayActorApplyCollisionVelocity(short slotA, short slotB, int angle);
void __cdecl Mini_HighwayActorApplyHit(int slotIndex, int amount);
void __cdecl Mini_HighwayActorDirectionalCue(int slotIndex, int angle);
void __cdecl Mini_HighwayActorIntegrateMotion(int slotIndex, int unused);

/* highway_mode_controller_layers.cpp */

/* highway_input_state_polling.cpp */
extern int g_HighwayInputAction;
extern int g_HighwayInputDirection;
extern int g_HighwayInputButtonMask;
extern int g_HighwayInputLock;
void __cdecl Mini_HighwayResetInputState(void);
void __cdecl Mini_HighwayDecodeInputMask(void);
void __cdecl Mini_HighwayReadInput(void);

/* highway_minigame_frame_loop.cpp */
extern unsigned char g_HighwayUseLgpArchive;
extern struct HighwaySoundCommand *g_pHighwaySoundCommandBuffer;
extern int g_HighwaySlot0Volume;
extern int g_HighwaySoundParam;
extern struct HighwaySoundCommand g_HighwaySoundCommandBuffer;
extern int g_HighwayNearestRivalVolume;
extern unsigned char g_HighwayAssetSourceMode;
extern int g_HighwayGameMode;
extern int g_HighwayControlFlags;
extern unsigned char g_HighwayRunningFlag;
extern int g_HighwayActorRenderOffset;
extern int g_HighwayCameraZPosition;
extern int g_HighwayTrackProgress;
extern int g_HighwayCurrentScore;
extern int g_HighwayHighScore;
extern unsigned char g_HighwayExitRequested;
extern unsigned char g_HighwaySpawningStopped;
void __cdecl Mini_HighwayResetRun(void);
int __cdecl Mini_HighwayInit();
void __cdecl Mini_Mode6ExitStoreScores(void);
void __cdecl Mini_HighwayUpdateFrameDelta(struct GfxEngineContext *context);
void __cdecl Mini_HighwayRunFrame(struct GfxEngineContext *context);
void __cdecl Mini_HighwayAdvanceTimeline(void);
void __cdecl Mini_HighwayUpdateScene(void);
void __cdecl Mini_HighwayDrawFieldContext(void);
void __cdecl Mini_HighwayDrawLayers(void);
void __cdecl Mini_HighwayDispatchModeController(void);
void __cdecl Mini_HighwayRunEventTimeline(void);
void __cdecl Mini_HighwayDamageCharacter(char category, short amount);
float __cdecl Mini_HighwayLinearFalloff(float distance);
float __cdecl Mini_HighwayTransformDepth(struct HighwayDepthTransform *transform, struct HighwayDepthPoint *point);
void __cdecl Mini_HighwaySetRecordMatrix(int recordMatrix);
void __cdecl Mini_HighwayScaleColorTriplet(struct HighwayColorSourceTriplet *srcColor, struct HighwayDepthPoint *point, struct HighwayColorTriplet *dstColor);
void __cdecl Mini_HighwaySendSoundCommandB9(void);
void __cdecl Mini_HighwayStartMusic(void);
void __cdecl Mini_HighwayDispatchSoundCategory(int category, int kind, int duration);
void __cdecl Mini_HighwaySendChannelCommandB0(int param, int channel);
void __cdecl Mini_HighwaySendChannelCommandA0(int param, char channel);
void __cdecl Mini_HighwaySendChannelCommandA8(short param, char channel);

/* highway_field_frames_motion.cpp */
extern const char *g_HighwayTrackTimNames1[12];
extern const char *g_HighwayTrackTimNames2[9];
int __cdecl Mini_HighwaySqrtFixed12(int fixedValue);
int __cdecl Mini_HighwayAtan2Angle4096(int dy, int dx);

/* highway_render_group_clipping.cpp */
void __cdecl Mini_HighwayInterpolatePackedColor(const int *first, const int *second, float t, int *result);
void __cdecl Mini_HighwayInterpolateVector(struct HighwayLerpVector *from, struct HighwayLerpTarget *to, float t, struct HighwayInterpolatedVector *out);
void __cdecl Mini_HighwayTransformPoint3ByMatrixForward(int matrix, int point, int out);
void __cdecl Mini_HighwayPerspectiveDivideVector4(int input, int output);

/* highway_u65_roster.cpp */
extern const char *g_HighwayModelNames[182];

/* highway_scene_camera_effects.cpp */
int __cdecl Mini_HighwayCameraAtan2Angle4096(int dy, int dx);
int __cdecl Mini_HighwayIntSqrtUnsigned(unsigned int value);
void __fastcall Mini_HighwaySetSceneBlockPresetEvent05(struct HighwayEffectSlotInitA *self);
void __fastcall Mini_HighwaySetSceneBlockPresetEvent13(struct HighwayEffectSlotInitA *self);

/* highway_sprite_group_quads.cpp */
extern struct HighwaySceneBlock2 *g_pHighwaySceneBlock;
extern int g_HighwayCameraYaw;

/* highway_slot_actors_update_render.cpp */
int __cdecl Mini_HighwayInitSlotArray(int context);

struct HighwayEffectStatusTable;

/* highway_minigame_lifecycle.cpp */
extern struct HighwayEffectStatusTable g_HighwayHpBarTable;

struct HighwayVec2;

/* highway_render_group_clipping.cpp */
void __cdecl Mini_HighwayInterpolateVec2(const struct HighwayVec2 *first, const struct HighwayVec2 *second, float t, struct HighwayVec2 *result);

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_HIGHWAY_H */

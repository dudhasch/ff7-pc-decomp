/* include/ff7/api/wm.h: the functions and objects ff7/wm defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_WM_H
#define FF7_API_WM_H

#ifdef __cplusplus
extern "C" {
#endif
struct WorldShadowTextureRect;
struct GfxFloat4;
struct GfxEngineContext;
struct WorldProjectedVertexEntry;
struct WorldVec;
struct WorldActor;
struct GfxMatrix4x4;
struct WorldShadowQuad;
struct WorldBlockLoadRequest;
struct WorldMeshSlot;
struct WorldPoint;
struct t_wm_SkyPoly;
struct GfxLightOwner;
struct WorldAmbientMatrix;
struct WorldAmbientColor;
struct WorldSkySprite;
struct WorldPushTerrainFloodCrossingRecordMesh;
struct WorldTerrainCurrentRecord;
struct WorldEffectOffset;
struct WorldEffectNode;
struct GfxRenderObject;
struct WorldScriptDirectoryEntry;

/* wm_world_zones_sky.cpp */
extern struct t_wm_SkyPoly g_WorldSkyPoly;
extern unsigned char g_WorldSkySprite[8];
extern unsigned char g_WorldScratchColorBuffer[];
extern int g_WorldSkySpritesEnd;
extern int g_WorldTerrainFloodVisitedMeshes;
extern int g_WorldOverlayChannelTimers;
#define g_WorldOverlayChannelTimerTable (&g_WorldOverlayChannelTimers)
extern unsigned char g_WorldTerrainEffectSlots[];
void __cdecl World_ArmScreenFlash(void);
void __cdecl World_DisarmScreenFlash(void);
void __cdecl World_StartFlashSoundAndArmScreenFlash(void);
void __cdecl World_StopFlashSoundAndClearScreenFlash(void);
int __cdecl World_IsFlashSoundActive(void);
void __cdecl World_ClearInitStepMask(void);
int __cdecl World_TryEnterInitStep(short step);
void __cdecl World_LeaveInitStep(short value);
void __cdecl WorldMap_SeedRandomTable(int seed);
void __cdecl WorldMap_ReshuffleRandomTable(void);
int __cdecl WorldMap_NextRandomByte(void);
int __cdecl WorldMap_WrappedDistance(struct WorldPoint *pointA, struct WorldPoint *pointB);
short __cdecl WorldMap_ArcTangentFromDeltas(int dx, int dz);
short __cdecl WorldMap_AngleBetweenPositions(void *from, void *to);
void __cdecl World_ResetSkyGradientPolygon(void);
int __cdecl World_GetSkyPolygon(void);
int __cdecl World_UnusedReturnZero(void);
int __cdecl World_GetSkyHorizonY(void);
void __cdecl World_UpdateSkyHorizonPolygon(short yaw);
void __cdecl World_InitSkySprites(void);
void __cdecl World_DrawSkySpriteQuad(struct WorldSkySprite *quad);
void __cdecl World_UpdateAndDrawSkyLayers(short angle);
void __cdecl World_SetMeteorSpriteEnabled(int value);
void __cdecl World_ResetZonesAndDefaultLighting(void);
void __cdecl World_ApplyDefaultMapLighting(void);
void __cdecl WorldZone_Select(int zoneIndex, int value);
void __cdecl WorldZone_SetMeshXZ(int meshColumn, int meshRow);
void __cdecl WorldZone_SetOffsetXZ(int offsetX, int offsetZ);
void __cdecl WorldZone_SetAmbientRGB(int red, int green, int blue);
void __cdecl WorldZone_SetOuterRadius(int value);
void __cdecl WorldZone_SetInnerRadius(int value);
void __cdecl WorldZone_SetFogNearRGB(int red, int green, int blue);
void __cdecl WorldZone_SetFogFarRGB(int red, int green, int blue);
void __cdecl WorldZone_ApplyToCamera(struct WorldPoint *position);
int __cdecl WorldZone_DistanceFrom(struct WorldPoint *position);
short __cdecl WorldZone_AngleFrom(void *position);
void __cdecl World_ResetOverlayFadeState(void);
void __cdecl World_UpdateAndDrawScreenOverlays(void);
void __cdecl World_BeginFadeFromBlack(int step, int option);
void __cdecl World_BeginFadeToBlack(int step, int option);
void __cdecl World_BeginOverlayChannelFadeUp(int channel);
void __cdecl World_BeginOverlayChannelFadeDown(int channel);
void __cdecl World_ShowOverlayChannelTimed(int channel);
int __cdecl World_IsScreenFadeIdle(void);
void __cdecl World_InitUnderwaterColorVectorsAndNoise(void);
void __cdecl World_BuildUnderwaterNoisePaletteAndGradient(int index);
void __cdecl World_InitZolomBodySegmentRecords(void);
void __cdecl World_BuildZolomTrailOnce(void);
void __cdecl World_InitZolomTrailRecords(void);
int __cdecl World_GetSwampEntryXFlag(void);
int __cdecl World_IsZolomActiveAndPlayerInRegion(void);
void __cdecl World_AdvanceZolomTrailHead(void);
void __cdecl World_DrawZolomBodySegments(void);
void __cdecl World_ResetTerrainFloodAndEffectState(void);
void __cdecl World_PushTerrainFloodCrossingRecord(struct WorldPushTerrainFloodCrossingRecordMesh *mesh);
void __cdecl World_ClearFloodMarksOnMesh(int mesh);
void __cdecl World_BeginTerrainFloodFill(int mesh, int triangle);
void __cdecl World_StartTerrainEffectAtPathRecord(int state);
void __cdecl World_GetProbeTriangleCenterXZ(int probe, void *center);
void __cdecl World_FinishTerrainEffectSlot(int slot);
void __cdecl World_UpdateTerrainEffect(void);
void __cdecl World_StepTerrainFloodFill(void);
void __cdecl World_AdvanceTerrainFloodFillStep(void);
int __cdecl World_TerrainFloodVisitEdgeNeighbor(int vertexA, int vertexB);
int __cdecl World_TerrainFloodCompareEdgeMidpointDistances(int vertexA, int vertexB, int vertexC);
void __cdecl World_UpdateTerrainFloodPathFollowers(void);
void __cdecl World_SetTerrainEffectKind(int value);
int __cdecl World_GetTerrainFloodFillOutcome(void);
int __cdecl World_IsTerrainEffectActive(void);
void __cdecl World_RequestTerrainFloodFinish(void);
void __cdecl World_ResetSteeringCycleAndSetSpawnIndex(int spawnIndex);
int __cdecl World_UpdateRandomSteeringCycleAndSpawnEntity(int dwMove, int dwTrigger);
int __cdecl World_GetSteeringSpawnIndex(void);
void __cdecl World_UnpackTerrainCurrentBytes(unsigned int packedBytes);
int __cdecl World_PackTerrainCurrentBytes(void);
void __cdecl World_ApplyTerrainCurrent(short chunkX, short chunkY);
struct WorldTerrainCurrentRecord *__cdecl World_FindTerrainCurrentRecord(short column, short row);
void __cdecl World_AddTerrainCurrentDrift(void *xPtr, void *zPtr);
void __cdecl World_UpdateTerrainCurrentStrengths(void);

/* wm_world_light_groups.cpp */
extern struct WorldAmbientColor g_WorldAmbientColor;
extern struct GfxLightOwner *g_pWorldPrimaryLightGroup;
extern void *g_pWorldSecondaryLightGroup;
void *__cdecl World_CreateSecondaryLightGroup(int setId);
void *__cdecl World_CreatePrimaryLightGroup(int preset);
void __cdecl World_FreePrimaryLightGroup(void);
void __cdecl World_FreeSecondaryLightGroup(void);
void __cdecl World_FreeLightGroups(void);
void __cdecl World_SetPrimaryLightGroupPreset(int preset);
void __cdecl World_SetAmbientColorFromMatrix(struct WorldAmbientMatrix *m);

/* wm_world_entity_pool.cpp */
extern unsigned char g_WorldActorCollisionMasks[208];
extern char g_WorldModelToVehicleSlotTable[32];
extern struct WorldScriptDirectoryEntry *g_pWorldScriptBase;
extern struct WorldScriptOperand *g_WorldScriptStackCursor;
extern int g_WorldVehicleActor;
extern char g_WorldVehicleActorType;
extern int g_WorldPlayerGroundOffset;
extern short g_WorldVehicleSavedFacing;
extern short g_WorldVehicleSavedAltFacing;
extern short g_WorldVehicleTransitionFlags;
extern int g_WorldWaitFramesZero;
extern struct WorldActor g_WorldActorPool[15];
extern struct WorldModelRecord *g_pWorldDeactivatedModelRecord;
extern struct WorldActor *g_pWorldExecutingActor;
extern struct WorldActor *g_pWorldActorFreeList;
extern struct WorldVehicleSlot *g_pWorldVehicleSlotTable;
extern int g_WorldVehicleBoardedFlag;
extern struct WorldScriptOperand g_WorldScriptOperandStack[4];
extern unsigned short *g_pWorldScriptCode;
extern struct VECTOR g_WorldScriptSavedPositions[3];
extern int g_WorldMeshTriggerArg0;
extern int g_WorldMeshTriggerArg1;
extern int g_WorldPlayerStepY;
extern int g_WorldPlayerStepCountdown;
extern int g_WorldVehicleTransitionState;
extern short g_WorldMeshTriggerCounter;
void __cdecl WorldEntity_InitPool(int slotTable);
struct WorldActor *__cdecl WorldEntity_Alloc(void);
void __cdecl WorldEntity_ClearPerFrameFlags(void);
int __cdecl WorldEntity_HasActorOfTypeAtLeast32(void);
void __cdecl WorldEntity_AttachPlayerToCurrentActor(void);
void __cdecl WorldEntity_DetachRiderAsPlayer(void);
void __cdecl WorldEntity_DetachRiderAsCurrent(void);
int __cdecl WorldEntity_IsCurrentRiding(void);
void __cdecl WorldEntity_FreePlayer(void);
void __cdecl WorldEntity_FreeCurrent(void);
void __cdecl WorldEntity_FreePlayerRiding(void);
void __cdecl WorldEntity_FreeCurrentRiding(void);
void __cdecl WorldEntity_ClearTriangleCacheSlotsByKey(short kind, short id);
void __cdecl WorldEntity_SelectPlayerAsCurrentActor(void);
void __cdecl WorldEntity_SetPlayerControlled(void);
char *__cdecl WorldEntity_GetPlayerTriangleCache(void);
int __cdecl WorldEntity_GetCurrentModelId(void);
int __cdecl WorldEntity_GetPlayerModelId(void);
int __cdecl WorldEntity_IsPlayerModelInMask(int mask);
int __cdecl WorldEntity_IsCurrentModelInMask(int mask);
int __cdecl World_IsModelInMask(int mask, int model);
int __cdecl WorldEntity_IsPlayerRideable(void);
int __cdecl WorldEntity_IsCurrentRideable(void);
int __cdecl World_IsRideableModel(int model);
void __cdecl WorldEntity_SetupForModel(int model);
void __cdecl WorldEntity_SetCurrentFacingClearTurn(short value);
void __cdecl WorldEntity_SetPlayerFacingClearTurn(short value);
void __cdecl WorldEntity_SetCurrentFacing(short value);
void __cdecl WorldEntity_SnapCurrentFacing(short value);
void __cdecl WorldEntity_StepCurrentFacingToward(short value);
void __cdecl WorldEntity_StepFacingToward(struct WorldActor *entity, short targetFacing);
void __cdecl WorldEntity_StepPlayerFacingToward(short value);
void __cdecl WorldEntity_EaseCurrentTilt(short target);
short __cdecl WorldEntity_GetCurrentFacingPlusTurn(void);
short __cdecl WorldEntity_GetPlayerFacingPlusTurn(void);
void __cdecl WorldEntity_AddCurrentAltitude(int value);
int __cdecl WorldEntity_HasCurrentMoved(void);
int __cdecl WorldEntity_HasPlayerMoved(void);
void __cdecl WorldEntity_SetPlayerAltitudeScripted(int value);
int __cdecl WorldEntity_IsPlayerAltitudeScripted(void);
int __cdecl WorldEntity_GetPlayerGroundY(void);
int __cdecl WorldEntity_SetCurrentById(int dwModelType);
void __cdecl WorldEntity_SetCurrentToPlayerCollide(void);
int __cdecl WorldEntity_IsCurrentNonPlayerRequestable(void);
void __cdecl WorldEntity_SetPlayerModelId(int value);
void __cdecl WorldEntity_SetPlayerTerrainInfo(int value);
int __cdecl WorldEntity_GetPlayerTerrainType(void);
int __cdecl WorldEntity_GetPlayerEncounterRegion(void);
int __cdecl WorldEntity_GetPlayerTerrainTopBit(void);
int __cdecl WorldEntity_GetPlayerRegionId(void);
void __cdecl WorldEntity_SetCurrentPositionAndGround(void *value);
int __cdecl World_GetModelGroundOffset(int terrainInfo, unsigned char modelType);
void __cdecl WorldEntity_SetPlayerPosition(void *value);
void __cdecl WorldEntity_SetPlayerPositionAndGround(void *ground);
void __cdecl WorldEntity_ApplyPlayerGroundStep(void *ground);
void __cdecl WorldEntity_SetPlayerAltitude(int value);
void __cdecl WorldEntity_SetPlayerAltitudeExplicit(int value);
void __cdecl WorldEntity_SetPlayerUnusedAttr(int value);
void __cdecl WorldEntity_RevertCurrentPosition(void);
void __cdecl WorldEntity_RevertPlayerPosition(void);
void __cdecl WorldEntity_ClearPlayerOnMesh(void);
void __cdecl WorldEntity_SetCurrentAnimId(int value);
int __cdecl WorldEntity_ResolveCurrentCollision(void);
int __cdecl WorldEntity_TestCurrentCollision(void);
int __cdecl WorldEntity_TestActorPairCollision(struct WorldActor *entity, struct WorldActor *collider);
void __cdecl WorldEntity_MarkPlayerOnMesh(void);
void __cdecl World_PlaceEntitiesOnMesh(int meshRecord);
void __cdecl WorldEntity_MoveCurrentBy(int dx, int dz);
void __cdecl World_SetCameraAngleOffsets(short savedFacing, short savedAltFacing, short transitionFlags);
int __cdecl World_GetPointFadeWeight(int x, int z);
void __cdecl WorldEntity_GetPlayerSlopeTiltAngles(short *out);
void __cdecl World_BeginEntityFrame(void);
void __cdecl WorldEntity_BeginFrame(struct WorldActor *ent);
void __cdecl WorldEntity_UpdateAllActorModelsAndShadows(void);
int __cdecl WorldEntity_AdvanceTerrainStateMachine(void *counter);
void __cdecl WorldEntity_DrawShadowIfEligible(struct WorldActor *actor);
void __cdecl WorldScript_Init(unsigned char *base);
void __cdecl WorldEntity_SetPlayerRequestsBlocked(int on);
void __cdecl WorldScript_RequestOnCurrentEntity(int code);
void __cdecl WorldScript_Request(int dwScriptId, int dwPriority);
void __cdecl WorldScript_RequestOnEntity(int entityId, int funcId);
void __cdecl WorldScript_RequestSystem(int funcId);
void __cdecl WorldScript_RequestMeshScript(short regionIndex, short row);
void __cdecl WorldScript_RunAllEntities(void);
void __cdecl WorldScript_ExecOperator(unsigned short op);
int __cdecl WorldScript_PopStackValue(void);
void __cdecl WorldScript_StoreToLValue(int value);
void __cdecl WorldScript_PushOperand(int opcode);
int __cdecl WorldScript_ExecControlFlow(unsigned short op);
int __cdecl WorldScript_ExecCommand(unsigned short wOpcode);
void __cdecl WorldMap_PollMeshScriptTrigger(void);
int __cdecl WorldScript_AnyScriptRunning(void);
void __cdecl World_SaveAllVehicleSlots(void);
void __cdecl WorldEntity_SaveToVehicleSlot(struct WorldActor *ent);
void __cdecl WorldScript_SetPartyChangeRegisters(int dwCharMask_old, int dwCharMask_new);

/* wm_world_effect_nodes.cpp */
extern unsigned char g_WorldMusicTrackRecord0[8];
extern unsigned char g_WorldMusicTrackRecord1[8];
extern int g_WorldMusicTrackRecord2;
extern unsigned char g_WorldEffectConfig[12];
extern unsigned char g_WorldEffectRecordIndex[4];
extern short g_WorldModelTintTable[7][4];
extern struct WorldEffectOffset g_WorldEffectOffset;
extern short g_WorldEffectOffsetZ;
extern struct WorldEffectOffset g_WorldEffectVelocity;
extern short g_WorldEffectVelocityZ;
extern short g_WorldEffectAngle;
extern short g_WorldCameraViewVector[];
extern unsigned char *g_pWorldArenaRegion0;
extern unsigned char *g_pWorldArenaRegion1;
extern unsigned char *g_pWorldArenaRegion2;
extern unsigned char *g_pWorldArenaRegion3;
extern unsigned char *g_pWorldArenaRegion4;
extern unsigned char *g_pWorldArenaRegion5;
extern unsigned char *g_pWorldArenaRegion6;
extern unsigned char *g_pWorldArenaRegion7;
extern unsigned char *gWorldArena;
extern unsigned char *g_pWorldMusicTrackTable;
extern struct WorldEffectNode *g_pWorldEffectRetiredLast;
extern int g_WorldEffectState;
extern unsigned char g_WorldEffectEmitterTable[][4];
extern struct WorldEffectNode *g_pWorldEffectNodeFreeList;
extern struct WorldEffectNode *g_pWorldEffectActiveList;
extern int g_WorldPrevMusicTrack;
extern struct WorldEffectNode *g_pWorldEffectRetiredList;
extern int g_WorldMusicChangeGate;
extern int g_WorldMusicTrack;
extern int g_WorldAmbienceId;
void __cdecl World_InitEffectNodePool(void);
void __cdecl World_SpawnEffectNode(short spawnYaw, short vectorYaw, short effect, int byte1F);
void __cdecl World_SpawnPlayerEffectNode7Sized16(void);
void __cdecl World_SetEffectEmitterRecord(int index, char, char, char);
void __cdecl World_DisableEffectEmitterRecord(int index);
void __cdecl World_RunEffectNodeFrame(void);
void __cdecl World_DrawEffectNodes(void);
void __cdecl World_UpdateEffectNodes(void);
void __cdecl World_SetShadowQuadTextureRect(struct WorldShadowQuad *dst, struct WorldShadowTextureRect *src);
void __cdecl World_ModelEffect14EndNoOp(struct WorldModelRecord *model);
void __cdecl World_TransformPositionByMatrix4x4(struct GfxMatrix4x4 *, int *, struct GfxFloat4 *);
int __cdecl World_ModelUsesBlendKind11(int value);
void __cdecl World_SetModelRecordTintFromTable(short modelId, short tableIndex);
void __cdecl World_ResetMusicState(int music);
void __cdecl World_SetMusicChangeGate(int value);
void __cdecl World_SwitchMusicTrack(int track);
void __cdecl World_ReplayCurrentMusicTrack(void);
int __cdecl World_GetMusicTrack(void);
void __cdecl World_PlaySfxAndArmScreenFlash(int value);
void __cdecl World_StopAllSfxAndResetTranspose(void);
void __cdecl World_SetMidiVolume(int value);
void __cdecl World_SetAllSfxChannelsTransposeTrans(char, char);
void __cdecl World_ClearAmbienceId(void);
void __cdecl World_RequestAmbienceSfx(int req);
void __cdecl World_SelectMusicParamTableByState(int state);

/* wm_world_minimap_render.cpp */
extern int g_WorldMiniMapMode;
extern int g_WorldMiniMapScale;
extern int g_WorldMiniMapRequestedMode;
void __cdecl World_InitMiniMap(void);
void __cdecl World_DrawMiniMap(short angle);
void __cdecl World_SetMiniMapState(short value);
short __cdecl World_GetMiniMapState(void);
void __cdecl World_RestoreMiniMapRequestedMode(void);
void __cdecl World_DrawMiniMapBackdropLarge(float x0, float y0, float sx, float sy);
void __cdecl World_DrawMiniMapBackdropSmall(float x0, float y0, float sx, float sy);
void __cdecl World_DrawMiniMapMarker(float x0, float y0, float v0, int colour, struct GfxRenderObject *node);

/* wm_world_terrain_streaming.cpp */
extern unsigned char g_WorldProbeOffsets[9 * 8];
extern unsigned char g_WorldMeshLoadState[1];
extern int g_WorldLoadedBlockIds[4];
extern int g_WorldProbeRowCounter;
extern int g_WorldCurrentProbeRow;
void __cdecl World_LzsDecompress(unsigned char *src, unsigned char *dst);
void __cdecl WorldMap_PositionToMesh(void *position, void *subOffset, void *meshX, void *meshZ);
void __cdecl World_WrapPositionToMapBounds(void *position);
int __cdecl World_GetTerrainBlockCountdown(void);
void __cdecl World_SetTerrainBlockCountdown(int value);
void __cdecl World_InitTerrainStreaming(void);
void __cdecl World_InitBlockPools(void);
void __cdecl World_InitMeshQueuePool(void);
void __cdecl World_InitMeshRecordPool(void);
void __cdecl World_DisableTerrainStreamingBeforeDeparture(void);
void __cdecl World_DisableTerrainStreamingAndReleaseLoadingBlock(void);
void __cdecl World_EnableTerrainStreaming(void);
int __cdecl World_IsTerrainStreamingEnabled(void);
void __cdecl World_UpdateTerrainStreaming(short wAngle, int placeEntities);
void __cdecl World_PlanResidentBlocks(int *pos);
int __cdecl World_BlockIndexFromGrid(short column, short row);
int __cdecl World_EnqueueBlockLoad(struct WorldBlockLoadRequest *request);
int __cdecl World_AcquireBlockSlot(void);
void __cdecl World_ServiceNearestBlockLoad(short column, short row);
void __cdecl World_PopAndLoadBlockRequest(struct WorldBlockLoadRequest *request, struct WorldBlockLoadRequest *prev);
void __cdecl World_ReadBlockFromMapFile(struct WorldBlockLoadRequest *req);
void __cdecl World_PromoteLoadedBlock(void);
void __cdecl World_ArmBlockLoadBudgetAndClearInitStep0(void);
short __cdecl World_CountBlockSlotsInUse(void);
void __cdecl World_QueueMeshesForBlock(int request);
void __cdecl World_QueueMeshDecompress(struct WorldMeshRecord *mesh);
void __cdecl World_DecompressMeshIntoSlot(struct WorldMeshSlot *slot, int slotEntry);
void __cdecl World_PlanResidentMeshes(int *position, short angle);
int __cdecl World_IsBlockLoading(short wBlockId);
int __cdecl World_ExpireStaleMeshRecordsAndCount(void);
void __cdecl World_BuildMeshWantMask(int *pos, short yaw, void *mask);
int __cdecl World_MeshOffsetFromCentre(struct WorldMeshRecord *record, short centreX, short centreZ, short *offsetX, short *offsetZ);
int __cdecl World_AcquireMeshRecord(void);
void __cdecl World_TerrainStreamingPostUpdateNoOp(void);
void __cdecl World_RequestTerrainFloodFill(void);
int __cdecl World_GetTerrainWalkCompletedFlag(void);
int __cdecl World_IsTerrainChunkLoaded(short wChunkX, short wChunkY);
void __cdecl World_PreloadBlocksFromBot(void);
int __cdecl World_TrimBlockCacheTo(short slotIndex);
void __cdecl World_ArmWalkerUnstick(int value);
void __cdecl World_SetWorldProgress(int value);
void __cdecl World_SumThreeViewVectors(int *sumOut);

/* wm_world_module_render.cpp */
extern int g_WorldRenderHeight;
extern int g_WorldRenderWidth;
extern float g_WorldUiScaleFactor;
extern double g_WorldTimestampDiff;
extern double g_WorldFpsInterval;
extern int g_WorldDisplayScale;
extern int g_WorldMapExitRequested;
extern int g_WorldMapInitialized;
extern int g_WorldModeResetDone;
void __cdecl WorldMap_ModuleMouse(void);
void __cdecl WorldMap_ModuleKeyboard(void);
void __cdecl World_DrawDebugTextOverlay(int x, int y, const char *text, void *dc);
void __cdecl WorldMap_ModuleEnter(struct GfxEngineContext *ctx);
void __cdecl World_ReleaseResourcesIfSuspended(void);
void __cdecl WorldMap_ModuleExit(void);
void __cdecl WorldMap_ModuleFrame(struct GfxEngineContext *ctx);
void __cdecl World_FlushTerrainBatches(void);
void __cdecl World_FlushTerrainBatchesUnderwater(void);
void __cdecl World_FlushTerrainBatchesSnow(void);
void __cdecl World_DrawStageModels(struct GfxEngineContext *drawContext);
void __cdecl World_WaitForFrameInterval(struct GfxEngineContext *ctx);
int __cdecl World_IsTerrainRenderEnabled(void);
void __cdecl World_SetTerrainRenderEnabled(int value);
void __cdecl World_SetDoubleFrameEnabled(int value);
int __cdecl World_GetDoubleFrameEnabled(void);

/* wm_world_message_window.cpp */
extern short g_WorldMsgChoiceLastLine;
extern short g_WorldMsgChoiceAnswer;
extern short g_WorldMsgChoiceFirstLine;
extern int g_WorldMsgInputUpdated;
void __cdecl WorldMsg_InitScriptContextAndWindows(void *script);
void __cdecl WorldMsg_Show(char value);
void __cdecl WorldMsg_Ask(char messageId, short firstLine, short lastLine);
int __cdecl WorldMsg_IsOpen(void);
int __cdecl WorldMsg_RequestCloseAndPoll(void);
void __cdecl WorldMsg_PollInputAndUpdateWindow(void);
short __cdecl WorldMsg_GetAnswer(void);
void __cdecl WorldMsg_SetWindowRect(short *rect);
void __cdecl WorldMsg_SetWindowMode(short, short);

/* wm_world_model_state.cpp */
extern unsigned char g_WorldModelSlotTable[32];
extern int g_WorldLeaderModelLoading;
extern int g_WorldLeaderModelReady;
extern int g_WorldStageVariantFlag;
extern int g_WorldOverlayModelActive;
extern int g_WorldOverlayModelCountdown;
extern int g_WorldOverlayModelState;
extern signed char g_WorldOverlayModelRecord[44];
extern int g_WorldStageModelIndex;
extern int g_WorldStageModelCount;
extern signed char g_WorldStageModelRecord[44];
extern int g_WorldLeaderModelIndex;
extern int g_WorldLeaderModelCount;
extern int g_WorldLeaderModelId;
void __cdecl World_ResetModelRecordState(void);
void __cdecl World_ClearOverlayModelState(void);
void __cdecl World_UpdateOverlayModelStateAndIdleCountdown(void);
void __cdecl World_RequestLeaderModelLoad(short model);
void __cdecl World_FinishPendingLeaderModelLoad(void);
void __cdecl World_MarkLeaderModelReady(void);
void __cdecl World_SetStageAndCopyModelSlotTable(short value);
void __cdecl World_CopyStageModelSlotTable(void);
void __cdecl World_ClearLeaderModelLoadLatch(void);
void __cdecl World_SetStageVariantFlag(int value);
int __cdecl World_GetStageVariantFlag(void);

/* wm_world_vehicle_stack.cpp */
extern unsigned char g_WorldVehicleModelStackBase;
extern unsigned char * g_pWorldVehicleModelStackTop;
extern int g_WorldVehicleDisembarkState;
extern int g_WorldVehicleHeldCancelFrames;
extern int g_WorldVehicleTransitionTimer;
extern int g_WorldVehicleTransitionArgument;
void __cdecl World_ResetVehicleState(void);
void __cdecl World_PushPlayerOffObstacle(int distance);
void __cdecl World_PushVehicleModel(char value);
unsigned char __cdecl World_PopVehicleModel(void);
unsigned char __cdecl World_PeekVehicleModel(void);
void __cdecl World_SetVehicleStackBase(int value);
int __cdecl World_IsRidingStacked(void);
void __cdecl World_BoardVehicle(void);
void __cdecl World_ArmVehicleExit(void);
int __cdecl World_CanExitVehicleHere(void);
void __cdecl World_SetVehicleExitArmed(void);
void __cdecl World_UpdateVehicleTransition(int action);
int __cdecl World_GetVehicleTransitionState(void);
void __cdecl World_SetVehicleTransitionArgument(int value);

/* wm_world_terrain_geometry.cpp */
extern struct WorldTerrainTriangle *g_pWorldSelectedTerrainTriangle;
extern int g_pWorldTerrainTable;
extern unsigned char g_WorldMsgHeader[];
extern short g_WorldMsgBuffer[];
extern short g_WorldArcTangentTable[512];
extern int g_WorldTerrainHasWater;
extern int g_WorldTerrainFogDistance;
extern int g_WorldTerrainOcclusionDepth;
extern int g_WorldFieldTable;
extern int g_WorldPlayerTerrainTriangle;
extern struct GfxMatrix4x4 g_WorldScreenMatrix;
int __cdecl World_RoundFloatToInt(float value);
void __cdecl World_UploadMeshVertices(short *meshVerts, int fadeScale);
void __cdecl World_ProjectMeshVertices(void);
int __cdecl World_TriangleHeightAt(struct WorldTerrainTriangle *triangle, short *position, int *out);
void __cdecl World_CrossProductFloat3(struct WorldVec *lhs, struct WorldVec *rhs, struct WorldVec *out);
float __cdecl World_TriangleSignedArea2(struct WorldProjectedVertexEntry *vtxA, struct WorldProjectedVertexEntry *vtxB, struct WorldProjectedVertexEntry *vtxC);
void __cdecl World_SortTriangleIndicesByDepth(float *pfZ0, float *pfZ1, float *pfZ2, int *pInd0, int *pInd1, int *pInd2);
void __cdecl World_ShrinkMeshUVs(unsigned char *tri, unsigned char *end, unsigned char delta);
void __cdecl World_SubmitTerrainChunkTriangles(struct WorldTerrainTriangle *first, struct WorldTerrainTriangle *end, short (*normals)[4], char *unused);

/* wm_world_savemap_state.cpp */
extern int g_WorldEncounterDangerCounter;
extern int g_WorldCountdownActive;
extern int g_WorldFieldTransitionState;
extern int g_WorldEncounterStepCounter;
extern int g_WorldFieldTransitionEntryWord;
extern int g_WorldEncountersEnabled;
extern int g_WorldFieldTransitionArg;
void __cdecl WorldScript_ClearVariableBank(void);
int __cdecl World_SelectMapVariantFromStoryProgress(void);
int __cdecl World_IsStoryProgressInWindow3E8To4B0(void);
int __cdecl World_GetSavedMapVariant(void);
void __cdecl World_RestoreStateFromSavemap(int statePtr, int argPtr, int skipSteering);
void __cdecl World_SaveStateToSavemap(void);
void __cdecl World_ExportTransitionRequestAndSaveState(int stateOut, int argOut, int entryOut);
void __cdecl World_BoardSavedVehicleModel(void);
void __cdecl World_SetFieldEntryFromTable(int fieldArg);
void __cdecl World_QueueFieldTransitionRequest(int value);
void __cdecl World_StartCountdownTimer(int value);
void __cdecl World_StopCountdownTimer(void);
void __cdecl World_RequestPlainReturnTransition(void);
int __cdecl World_GetTransitionRequestState(void);
int __cdecl World_GetTransitionArgument(void);
void __cdecl WorldMap_UpdatePartySlots(unsigned char *oldParty, unsigned char *newParty, int update);
int __cdecl WorldMap_GetLeaderModelIndex(void);
void __cdecl World_SyncPartyAndLeaderModel(void);
void __cdecl World_StoreAreaNameToSavemap(int value);
void __cdecl World_SetChocoboEncounterInfo(int value);
int __cdecl World_GetChocoboEncounterInfo(void);
int __cdecl World_GetBattleEscaped(void);
void __cdecl World_ResetEncounterStep(void);
void __cdecl World_ReloadEncounterCounterNormal(void);
void __cdecl World_ReloadEncounterCounterShort(void);
void __cdecl World_SetEncountersEnabled(int value);
void __cdecl World_UpdateEncounters(void);
int __cdecl World_RollEncounter(void);
int __cdecl World_GetEncounterRateScale(void);
int __cdecl World_GetPreemptiveRate(void);
int __cdecl World_IsPreemptiveMastered(void);
int __cdecl World_GetChocoboLure(void);
int __cdecl World_IsLevelEncounterEnabled(void);
int __cdecl World_IsEncounterSlot78Enabled(void);
int __cdecl World_PartyHasMultipleMembers(void);
int __cdecl World_IsEncounterSlot10Enabled(void);

/* wm_world_camera_accessors.cpp */
extern int g_WorldCameraDeltaY;
extern int g_WorldPlayerAirborneFlag;
extern int g_WorldPrevKeyInputStatus;
extern int g_WorldMovementMultiplier;
extern int g_WorldCameraPitchOffset;
extern int g_WorldMapInMenu;
int __cdecl World_GetRenderBufferIndex(void);
int __cdecl World_AllocRenderVertexBlock(int count);
char *__cdecl World_BuildLoosePath(int assetIndex, int digit);
char *__cdecl WorldMap_BuildAssetName(int suffixIndex, int digit);
void __cdecl World_SetProjectionJitterEnabled(int value);
int __cdecl World_GetRenderTableEntryIfActive(void);
int __cdecl World_CanModelEnterTerrain(int terrain, unsigned char model);
int __cdecl World_GetCameraMode(void);
int __cdecl World_GetCameraRotationY(void);
void __cdecl World_SetCameraFront(int value);
void __cdecl World_SetCameraFrontAndRotationY(int value);
void __cdecl World_SetCameraZoomState(int zoomState);
int __cdecl World_GetCameraZoomState(void);
int __cdecl World_GetCameraRoll(void);
void __cdecl World_SetCameraRotationZ(int value);
int __cdecl World_GetMapVariant(void);
void __cdecl World_LoadCameraMatrixToGte(void);
short __cdecl World_GetCurrentCameraRotationX(void);
void __cdecl World_SetCameraMode(int mode);
void __cdecl WorldMap_SetPlayerControl(int enable, int mode);
int __cdecl World_IsPlayerControlEnabled(void);
void __cdecl World_SetPreviousPlayerDirectionFromFacing(short heading);
int __cdecl World_IsControlLockClear(void);
int __cdecl World_GetFadeTarget(void);
void __cdecl World_SetCameraBlendDirection(int value);
int __cdecl World_GetCameraBlendDirection(void);
void __cdecl World_BeginSubmarineSurfacing(void);
void __cdecl World_BeginSubmarineDive(void);
void __cdecl World_BeginScriptFieldExit(int value);
void __cdecl WorldMap_RequestFieldTransition(int fieldId);
void __cdecl WorldMap_RequestBattle(int formation);
void __cdecl WorldMap_RequestPlainReturn(void);
int __cdecl World_GetCameraModeAndMiniMapState(void);
void __cdecl World_SetCameraModeAndMiniMapState(int request);
unsigned char *__cdecl World_GetMessage(short index);
void __cdecl World_ClearStreamingStallFadeState(void);
void __cdecl World_SetStreamingStallFadeState(int state);
void __cdecl World_StepStreamingStallFadeState(void);
void __cdecl World_SetCameraYawStep(int value);
void __cdecl World_SetCameraHeightStep(int value);
void __cdecl World_SetCameraDistanceStep(int value);
void __cdecl World_AdvanceScriptedCamera(void);
void __cdecl World_SetCameraDistance(int value);
void __cdecl World_SetCameraHeight(int value);
void __cdecl World_SetChocoboTerrainSet(int value);
int __cdecl World_GetChocoboTerrainSet(void);
void __cdecl World_UnusedNoOp_1(void);
void __cdecl WorldMap_ResetModeOnFirstUpdate(void);
int __cdecl WorldMap_MainUpdate(int *dispatch, int *field, int *battle, int mode);
void __cdecl WorldMap_LoadStageResources(short stage);
void __cdecl World_InitRenderVertexBuffersAndSky(void);
void __cdecl World_UpdateProjectionOffset(void);
void __cdecl World_FlipRenderBuffers(void);
void __cdecl World_DrawSkyPolygonQuad(int frameStep);
void __cdecl World_AnimateTerrainTextures(int frameStep);
void __cdecl World_BuildCameraMatrix(short yaw);
void __cdecl World_UpdatePlayerControl(void);
void __cdecl World_UpdateCameraBlendParameters(void);
int __cdecl World_UpdateCameraDistanceAndPitch(void);
void __cdecl Party_ReloadStatsAndLimitTablesIfFlagged(void);
void __cdecl World_UpdateMapStateMachine(void);
void __cdecl World_MapStateHookNoOp(void);
void __cdecl World_UpdateCameraHeightOffsetForOcclusion(void);
int __cdecl World_GetMapState(void);

/* wm_world_message_helpers.cpp */
extern short g_WorldMsgWindowOperandIndex[4];
extern unsigned char *g_WorldMsgDialogTable;
extern short g_WorldMsgWindowNumberPos[4];
extern short g_WorldMsgWindowRevealBudget[4];
extern short g_WorldMsgWindowNamePos[4];
extern short g_WorldMsgWindowCursorState[4];
extern unsigned char *g_WorldMsgWindowStringPointers[4];
extern short g_WorldMsgWindowScrolledRows[4];
void __cdecl World_UnusedNoOp_2(void);
void __cdecl World_DebugLogNoOp(char *message);
void __cdecl WorldMsg_InitWindowsAndMessageTable(void);
void __cdecl WorldMsg_ResetWindow(short windowIndex);
int __cdecl WorldMsg_RequestWindowClose(short windowIndex);
void __cdecl WorldMsg_SetWindowStyleAndPreventClose(short index, short, short);
void __cdecl WorldMsg_ClearAllWindows(void);
void __cdecl WorldMsg_SetWindowRectClamped(short windowIndex, short x, short y, short width, short height);
void __cdecl WorldMsg_MoveWindow(short windowIndex, int dx, int dy);
void __cdecl WorldMsg_SetWindowHeight(short index, short value);
int __cdecl WorldMsg_DispatchWindowState(unsigned char index, unsigned char mode);
int __cdecl WorldMsg_UpdateChoiceWindow(unsigned char index, unsigned char mode, unsigned char first, unsigned char last, short *choice);
void __cdecl WorldMsg_PlayCursorSound(void);
int __cdecl WorldMsg_BeginOpenWindow(short wDialogId, short wScriptId);
void __cdecl WorldMsg_StepWindowOpenGrow(short wDialogId);
void __cdecl WorldMsg_StepWindowTextReveal(short id);
void __cdecl WorldMsg_StepWindowRowScroll(short windowIndex);
void __cdecl WorldMsg_StepWindowPageScroll(short windowIndex);
void __cdecl WorldMsg_ResetWindowTextRun(short windowIndex);
int __cdecl WorldMsg_StepWindowClose(short windowIndex);
short __cdecl WorldMsg_ReadScriptOperand(short windowIndex);
void __cdecl WorldMsg_FormatDecimalNoPad(unsigned short value, unsigned char *dst);
void __cdecl WorldMsg_FormatDecimalPadded5(unsigned short value, unsigned char *dst);
void __cdecl WorldMsg_FormatHexNoPad(unsigned short value, unsigned char *dst);
int __cdecl WorldMsg_DecodeMessageToTextBuffer(short idx);
void __cdecl WorldMsg_CopyMessageToCharacterName(short charIndex, short messageIndex);

/* wmdefine.cpp */
void __cdecl World_AllocArena(void);
void __cdecl World_FreeArena(void);

/* wmfile.cpp */
extern int g_WorldCurrentTextureSetId;
extern short g_WorldAnimTexturePages[22];
extern struct GfxRenderObject *g_WorldTexturePageRenderObjects[0x11A];
extern struct GfxRenderObject *g_WorldRadarRenderObject;
extern struct GfxRenderObject *g_WorldMiniMapRenderObject;
extern struct GfxRenderObject *g_WorldTerrainRenderObject;
extern struct GfxRenderObject *g_WorldMapRenderObject;
extern struct GfxRenderObject *g_WorldSkyRenderObject;
extern void * g_WorldCloudRenderObject;
extern void * g_WorldMeteorRenderObject;
extern struct GfxRenderObject *g_WorldScreenOverlayRenderObjects;
extern unsigned char g_WorldSnowRenderObjects[4];
extern int g_WorldSnow5RenderObject;
extern void * g_WorldShadowRenderObject;
extern struct GfxRenderObject *g_WorldDfxRenderObject0;
extern struct GfxRenderObject *g_WorldDfxRenderObject1;
extern void * g_WorldDfxRenderObject2;
extern struct GfxRenderObject *g_WorldDfxRenderObject3;
extern struct GfxRenderObject *g_WorldMidgarRenderObject;
extern unsigned int g_WorldTerrainTextureCount;
extern unsigned int g_WorldTexturePageCount;
extern int g_WorldViewportX;
extern int g_WorldViewportY;
extern struct WorldModelRecord g_WorldLeaderModels[3];
extern struct WorldModelRecord g_WorldStageModels[13];
extern void ** g_WorldTextureAuxTable;
extern char g_WorldStageTextureDir[8];
int __cdecl World_IsAnimatedTexturePage(unsigned int page);
int __cdecl WorldMap_LoadLgpAsset(char *path, int size, void *buffer, int unused);
int __cdecl World_ReadLooseFileBlock(int fileName, int offset, int size, void *buffer, int unused);
void __cdecl World_LoadStageModels(int stageId);
void __cdecl World_LoadLeaderModel(int modelId);
int __cdecl World_GetLoadedStageModelCount(void);
void __cdecl World_FreeStageModels(void);
void __cdecl World_FreeLeaderModels(void);
int __cdecl World_GetStageTextureSetId(int stageId);
void __cdecl World_LoadStageAssets(int stageId);
void __cdecl World_LoadTerrainTextures(int setId);
void __cdecl World_LoadStageTextures(int setId);
void __cdecl World_ReleaseStageTextures(void);
int __cdecl World_LoadAndDecompressLzsFile(char *name, int unused, void *dst, void(__cdecl *onDone)(void));
void __cdecl World_ClearStageAndLeaderModelCounts(void);

extern struct WorldActor *g_pWorldPlayerActor;
extern struct WorldActor *g_pWorldCurrentActor;
struct WorldVehicleModelStack;
struct WorldVehicleSlotStack;

/* wm_world_camera_accessors.cpp */
void __cdecl WorldScript_ReportError(int errorCode);

/* wm_world_entity_pool.cpp */
extern struct WorldActor g_WorldSystemActor;
extern struct WorldVehicleSlotStack g_WorldVehicleSlotStack;
extern struct WorldVehicleModelStack g_WorldVehicleModelStack;

struct WorldBlockLoadInfo;
struct WorldEffectSprite;
struct WorldMapModel;
struct WorldModelRecord;
struct SVECTOR;

/* wm_world_camera_accessors.cpp */
void __cdecl World_SetViewTransformForPosition(struct SVECTOR *view);
void __cdecl World_BuildModelPsxMatrix(struct WorldMapModel *model, struct SVECTOR *rotation, void *matrix, int halveFlag);

/* wm_world_effect_nodes.cpp */
void __cdecl World_DrawEffectNodeQuad(struct WorldShadowQuad *quad, struct WorldEffectSprite *sprite, short *position, short horizontal);
void __cdecl World_DrawShadowQuad(short width, short depth, short height, short yaw, struct WorldShadowQuad *quad, short offset);

/* wm_world_model_state.cpp */
struct WorldModelRecord *__cdecl World_GetModelRecord(short model);

/* wm_world_terrain_streaming.cpp */
struct WorldBlockLoadInfo *__cdecl World_FindResidentBlock(short wBlockId);

/* wm_world_zones_sky.cpp */
void __cdecl WorldMap_RotateVec3ByYaw(struct SVECTOR *vec, short yaw);

/* wm_world_zones_sky.cpp */
void __cdecl World_UpdateZolom(void);

struct WorldGroundProbe;
struct WorldMeshVertex;
struct WorldPosition;
struct WorldProbeRow;
struct WorldStreamFocusData;
struct WorldTerrainChunkInfo;
struct WorldTerrainTriangle;
struct WorldTransformedVertexEntry;
struct WorldTriangleCache;
struct VECTOR;

/* wm_world_camera_accessors.cpp */
extern unsigned short g_WorldEncounterTable[0x450];
int __cdecl World_SampleGroundAtEntity(struct WorldTerrainChunkInfo *chunk, struct SVECTOR *position, struct WorldTriangleCache *cache, short *height, struct WorldTerrainTriangle **triangle, short *terrain, unsigned char model);

/* wm_world_effect_nodes.cpp */
extern unsigned char g_WorldEffectRecordTable[0x6d0];
extern struct VECTOR g_WorldEffectOrigin;
extern unsigned char g_WorldEffectNodePool[0x20 * 0x54];
void __cdecl WorldEntity_SpawnTerrainEffects(struct WorldActor *actor, int active);
void __cdecl World_SpawnActorEffectsByKind(struct WorldActor *actor, int effect);
void __cdecl World_DrawActorShadow(struct WorldActor *actor);
void __cdecl World_SetQuarterTurnXMatrix4x4(float m[4][4]);
void __cdecl World_ApplyActorPoseToModelRecord(struct WorldModelRecord *model, struct WorldActor *actor, short animation, short frame, void *rotation);

/* wm_world_entity_pool.cpp */
extern unsigned char g_WorldScriptAuxStorage[0x40];
extern struct WorldActor * g_pWorldActiveActors;
extern struct VECTOR g_WorldVehicleReturnPosition;
extern unsigned char * g_pWorldScriptBank0;
#define g_WorldScriptBanks (&g_pWorldScriptBank0)
extern unsigned char * g_pWorldScriptBank1;
extern unsigned char * g_pWorldScriptBank2;
extern unsigned char * g_pWorldScriptBank3;
void __cdecl WorldEntity_Clear(struct WorldActor *actor);
void __cdecl WorldEntity_Unlink(struct WorldActor *actor);
void __cdecl WorldEntity_LinkIntoActiveList(struct WorldActor *entity);
void __cdecl WorldEntity_Free(struct WorldActor *entity);
struct WorldActor *__cdecl WorldEntity_GetCurrent(void);
void __cdecl WorldEntity_SetPositionAndGround(struct WorldActor *entity, struct WorldPosition *value);
void __cdecl WorldEntity_GetCurrentPosition(struct VECTOR *out);
void __cdecl WorldEntity_GetPlayerPosition(struct VECTOR *out);
void __cdecl WorldEntity_GetCurrentPrevPosition(struct VECTOR *out);
void __cdecl WorldEntity_GetPlayerPrevPosition(struct VECTOR *out);
struct WorldActor *__cdecl WorldEntity_GetCurrentCollidedActor(void);
void __cdecl WorldEntity_UpdateActorModel(struct WorldActor *actor);
void __cdecl WorldScript_ExecEntity(struct WorldActor *actor);
void __cdecl WorldEntity_RestoreFromVehicleSlot(struct WorldActor *actor);
void __cdecl World_SetVehicleReturnPosition(struct VECTOR *);

/* wm_world_message_helpers.cpp */
extern unsigned char g_WorldMsgDigitGlyphs[0x10];
extern short g_WorldMsgWindowScrollX[4];
extern short g_WorldMsgWindowCursorPos[4];
extern unsigned char g_WorldMsgTextBuffer[4][256];

/* wm_world_terrain_geometry.cpp */
extern struct WorldMeshVertex g_WorldMeshVertices[0x80];
extern struct WorldProjectedVertexEntry g_WorldProjectedVertices[0x80];
extern struct WorldTransformedVertexEntry g_WorldTransformedVertices[0x80];

/* wm_world_terrain_streaming.cpp */
extern char g_WorldWalkDebugText[0x64];
void __cdecl World_UpdateStreamFocus(struct WorldStreamFocusData *target, int smooth);
void __cdecl World_GetStreamFocus(struct WorldStreamFocusData *focus);
struct WorldGroundProbe *__cdecl World_ResolveProbesOnMesh(struct WorldTerrainChunkInfo *chunk, struct WorldProbeRow *rows, short x, short y);
void __cdecl World_FillProbeRow(struct WorldProbeRow *row, struct SVECTOR *position, struct SVECTOR *rotation, int distance);

/* wm_world_zones_sky.cpp */
void __cdecl World_SampleZolomHeadGround(struct WorldTerrainChunkInfo *chunk);

/* wmfile.cpp */
extern short g_WorldAnimGroupPageOrder[0x16];
extern void * g_WorldTerrainAnimFrameRenderObjects[0x6c];

/* wm_world_camera_accessors.cpp */
extern char g_WorldDebugBuffer[0x100];

/* wm_world_camera_accessors.cpp */
extern unsigned char g_WorldAssetNameStringPool[84];

/* wmfile.cpp */
extern unsigned char g_WorldEmptyTextureName0[4];
extern unsigned char g_WorldEmptyTextureNames[84];

/* wm_world_module_render.cpp */
extern void (__cdecl *g_WorldTerrainFlushFn)(void);

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_WM_H */

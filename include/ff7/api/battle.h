/* include/ff7/api/battle.h: the functions and objects ff7/battle defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_BATTLE_H
#define FF7_API_BATTLE_H

#include "lib/codegen.h"

#ifdef __cplusplus
extern "C" {
#endif
struct BattleAiGlobals;
struct BattleFxPsxPolygonBlock;
struct BattleFxPsxPolygonPrim;
struct GfxPsxSpriteQuadState;
struct BattleCameraWorldPointScratch;
struct BattleCameraLocalPoint2;
struct BattleSummonHadesSpriteReq3;
struct BattleSummonHadesMeshBlob;
struct BattleSummonHadesSpriteReq3;
struct Timestamp64;
struct BattleSummonPhoenixParticle;
struct VECTOR;
struct BattleFxSpriteRequest;
struct SoundGroup;
struct YasuiLimitSatelliteBeamModelDrawReq;
extern struct BattleAiGlobals gBattleAiGlobals;
struct BattleState;
struct BattleItemListBlock;
struct BattleSceneState;
struct PsxMatrix;
struct Vec3sPacked;
struct GfxPsxMatrixTransposed;
struct GfxEngineContext;
struct Battle3dLmdFaceData;
struct BattleFxBlueMatraMagicSegmentWork;
struct BattleFxSatanSlamMeshSetup;
struct BattleFxRubyTentaclesMeshPacket;
struct RubyTentaclesModelRenderDescriptor;
struct Battle3dDescriptorFile;
struct Battle3dSkeleton;
struct BattleBinSection;
struct BattleFxActorMatrix;
struct BattleFxGroundLivewireDescriptor;
struct BattleFxSpriteColor;
struct BattleFxJibakuRingSource;
struct BattleFxWhirlsandStageRequestArgs;
struct BattleFxLimitOmnislashMatrix;
struct BattleFxModelGroupContainer;
struct BattleFxModelGroupEntry;
struct BattleFxNightmareQuadState;
struct BattleFxBzSpriteParams;
struct BattleFxPsxMeshDescriptor;
struct BattleFxPsxPrimitive;
struct BattleFxPsxTrailRibbon;
struct BattleFxRamuhPsxSpriteHeader;
struct BattleFxRingGridPoint;
struct BattleFxSourcePoint;
struct BattleFxSummonViewport;
struct BattleFxWeapon2MatrixTranslation;
struct CharGrowth;
struct BattleLimitSlots;
struct BattleFxDespellFragment;
struct BattleFxQuake3Shard;
struct BattleFxQuake2Shard;
struct BattleModelAnimBank;
struct BattleStageDat;
struct PolygonModel;
struct BattleTaskSlotBuffer;
struct BattleSequenceBuffer;
struct BattleRewardTotals;
struct BattleRewardItem;
struct Battle3dNode;
struct BattleHitDisplayEntry;
struct BattleEffectTrailState;
struct BattleActorSpawnPos;
struct BattleFxDeBarrierShatterDesc;
struct BattleFxFallingRocksFallingModelSpawnSlot;
struct BattleCommandContext;
struct BattleMagicQuake2Shard;
struct BattleMagicQuake3Shard;
struct BattleMagicIce3Fragment;
struct BattleAuxTaskSlot;
struct BattleOverlayTaskSlot;
struct BattleActorState;
struct BattleActor;
struct BattleLimitAnimBank;
struct BattleActorDrawEntry;
struct BattleFxDeBarrierShatter;
struct BattleFxHitMarkScriptSlot;
struct BattleFxSaferSephirothDeathMeshDraw;
struct SaferSephirothDeathModelPart;
struct BattleFxLimitSatanSlamTrailRing;
struct BattleFxNightmareUvScratch;
struct BattleFxPsxDisplacedMeshDesc;
struct BattleFxQuake2ShardBurst;
struct BattleFxSatanSlamActorSurface;
struct BattleFxOffscreenCaptureReq;
struct BattleFxShivaForceFieldEntry;
struct BattleFxRubyTentaclesOffscreenRecord;
struct BattlePartyStatWords;
struct BattlePlaneNormal;
struct SavemapCharacter;
struct SavemapCharacter;
struct BattleFxBlueJibakuRing;
struct BattleFxSpiderWebOrbit;
struct BattlePartyActor;
struct Battle3dPartyLoadDesc;
struct BattleAction;
struct BattleActorSpiralRecord;
struct BattleAffinityTable;
struct BattleCombatant;
struct BattleFxBlueBetaParticleField;
struct BattleFxBlueQuestionMarksDraw;
struct BattleFxBlueLaserSegmentWork;
struct BattleFxMagicIce3ShatterDraw;
struct BattleFxDeathStripTrail;
struct BattleFxDespellMesh;
struct BattleFxFaigaMeshRecord;
struct BattleFxBladeBeamParticle;
struct BattleFxJibakuShard;
struct BattleFxLimitNightmarePsxMesh;
struct BattleFxLimitNightmareTrailRing;
struct BattleFxLimitSatanSlamMeshState;
struct BattleFxLimitLivewireTimelineSlot;
struct BattleFxMagicIce3Fragment;
struct BattleFxMagicHitmarkParticle;
struct BattleFxMagicShieldFace;
struct BattleFxPsxMeshHeader;
struct BattleFxQuake3ShardState;
struct BattleFxShieldTriangleBlock;
struct BattleFxSpriteQuadRequest;
struct BattleFxTitanOffscreenRequest;
struct BattleFxVector3;
struct BattleFxWhirlsandRideSlot;
struct BattleFxEmeraldShootTrailState;
struct BattleHitRecord;
struct BattleKotrKnight;
struct PolygonFileArrays;
struct SVECTOR;
struct YasuiLimitSatanSlamParticle;
struct GfxTableCursor;
struct Int64Pair;
struct BattleFxBahamutDrawPair;
struct BattleFxIce3MultiTexturedShard;
struct BattleScreenFlashRamp;
struct BattleFxDicePosRecord;
struct BattleFxAlexanderMatrixEntry;
struct GfxRotationAngles2;
struct BattleStageFloorFlags;
struct BattleFixedPointRamp;
struct BattleFxCameraBasisScratch;
struct BattleFxPosition3s;
struct BattleFxDebrisSpawnTask;
struct BattleFxAlexanderGradient;
struct BattleFxNeoBahamutLightRig;
struct BattleFxPhoenixGrowState;
struct BattleFxRibbonParticle;
struct BattleFxGustFourColorPacket;
struct BattleFxShortOffsetVec;
struct BattleFxBillboardVector;
struct BattleFxPsxGouraudTriPacket;
struct BattleFxFatChocoboCamera;
struct BattleFxShardQuad;
struct BattleFxNeoBahamutFaceSlot;
struct BattleFxTimelineState;
struct BattleFxKjataBlackGrowState;
struct BattleFxKjataTrailQuad;
struct BattleRotationMatrix;
struct BattleFxHellBomberEndVertex;
struct BattleFxBzGroundLivewireSlot;
struct BattleFxChocoMogDetachState;
struct BattleFxGreatGospelPolygonVertex2;
struct BattleFxGreatGospelStreakParticle;
struct BattleFxSlowBlendVertex;
struct BattleFxShatterMeshDelta;
struct BattleFxModelPartDetach;
struct BattleFxPsxGouraudRowBlock;
struct BattleFxRadialLivewire;
struct BattleFxRisingSpriteParticle;
struct BattleFxSuperNovaGeometry;
struct BattleFxSuperNovaPosFixed8;
struct BattleFxShivaSpriteRecord;
struct BattleFxShrinkGeometry;
struct BattleFxSwirlingSprite;
struct BattleFxMeshShardSlot;
struct BattleSkeletonTextureHeader;
struct PfileLoadOptions;
struct YasuiBzSlotState;
struct YasuiSealEvilParticle;
struct BattleOverlayRootLoadState;
struct BattleBlueFrogSongState;
struct BattleDamageDisplayRecord;
struct BattleActorAnimState;
struct BattleEffectPrimitive;
struct BattleBluePandorasBoxFragmentRecords;
struct BattleLimitTifaRelativeVec;
struct BattleMagicDeathTrailPoint;
struct BattleFxMagicGravSegment;
struct BattleModelEnemyBlendOverrides;
struct BattleFxNeoBahamutSceneActor;
struct BattleFxShivaRingDelay;
struct YasuiBoulderThrowDisplacement;
struct YasuiLimitBreathOfTheEarthTrailScratch;
struct ThreadRecord;
struct BattleFxDeathTrailEdge;
struct BattleFxOdinZCameraScript;
struct BattleFxOdinZCameraTarget;
struct BattleSummonRamuhCameraSlot;
struct BattleSummonRamuhSavedActor;
struct PolygonTim;
struct BattleBlueShadowFlareSpriteReq1;
struct BattleBlueShadowFlareSpriteReq3;
struct BattleFxParticleAttachmentOffsetMatrix;
struct BattleFxParticleQuadTransformMatrix;

struct BattleLimitDragonDiveModelDrawRequest;
struct BattleLimitDynamiteParamTable;
struct BattleLimitLandscaperMeshSourceBlob1;
struct BattleLimitLandscaperMeshSourceBlob2;
struct BattleLimitAllCreationRotAngles2;
struct BattleLimitSledFangSpriteRequest1;
struct BattleLimitSledFangSpriteRequest2;
struct BattleLimitStardustRayModelReq2;
struct PsxSVectorZ32;
struct BattleFxOffscreenSlotPair;
struct BattleMagicSlow_ModelReq2;
struct OpeningFacing36;
struct OpeningPositions108;
struct OpeningFacing54;
struct OpeningPositions162;
struct BattleStatusTintEntry;
struct BattleSpecPoisonGasSpriteReq1;
struct BattleSpecSephirothShockSpriteReq1;
struct BattleSpecSephirothShockSpriteReq10;
struct BattleSpecSephirothShockSpriteReq11;
struct BattleSpecSephirothShockSpriteReq2;
struct BattleSpecSephirothShockSpriteReq3;
struct BattleSpecSephirothShockSpriteReq5;
struct BattleSpecSephirothShockSpriteReq6;
struct BattleSpecSephirothShockSpriteReq9;
struct BattleSpecEnergyBallSpriteRequest1;
struct BattleSpecEnergyBallSpriteRequest2;
struct BattleSpecEnergyBallSpriteRequest3;
struct BattleSpecEnergyBallTransformData1;
struct BattleSpecEnergyBallTransformData2;
struct BattleSpecEnergyBallTransformData3;
struct BattleSpecEnergyBallTransformData4;
struct BattleSpecMultiJammerScale12;
struct BattleSpecHeatWaveModelReq2;
struct BattleSpecMultiJammerSpriteReq4;
struct BattleSpecBombBlastSpriteReq1;
struct BattleSpecFireballSpriteReq1;
struct BattleSpecLightShellModelReq;
struct BattleSpecLightShellSpriteReq2;
struct BattleSpecHeatWaveModelReq1;
struct BattleSpecHeatWaveSpriteReq1;
struct BattleSpecHeatWaveSpriteReq2;
struct BattleSpecHeatWaveSpriteReq3;
struct BattleSpecHellBomberModelReq2;
struct BattleSpecHellBomberModelReq3;
struct BattleSpecHellBomberSpriteReq1;
struct BattleSpecHellBomberSpriteReq2;
struct BattleSpecHellBomberSpriteReq3;
struct BattleSpecHellBomberSpriteReq4;
struct BattleSpecSwiftBoltSpriteReq1;
struct BattleSpecSwiftBoltSpriteReq2;
struct BattleSpecSwiftBoltSpriteReq3;
struct BattleSpecSwiftBoltSpriteReq4;
struct BattleSpecSwiftBoltSpriteReq5;
struct BattleSpecialLiquidPoisonColor1;
struct BattleSpecialWaveCannonModelReqMoto;
struct BattleSpecialWaveCannonModelReqTama;
struct BattleSpecialWaveCannonVector1;
struct BattleSpecialWaveCannonRotationTrajectory;
struct BattleSpecialWaveCannonScaleTama;
struct BattleSpecialHarrierLookMatrix;
struct BattleSpecialSpreadLaserSpriteConfig2;
struct BattleSpecialSpreadLaserSpriteConfig3;
struct BattleSpecialSpreadLaserSpriteConfig4;
struct BattleSpecialSpreadLaserSpriteConfig5;
struct BattleSpecialBeamGunBasisMatrix;
struct BattleLimitHowlingMoonSpriteRequest;
struct BattleSpecialBadMouthColor1;
struct BattleSpecialFallingRocksColor2;
struct BattleSpecialBadMouthModelReq1;
struct BattleSpecialOilColor;
struct BattleSpecialOilModelReq;
struct BattleSpecialOilSpriteIndices;
struct BattleSpecialFallingRocksModelReq1;
struct BattleSpecialTriangleAttackModelReq1;
struct BattleSpecialTriangleAttackModelReq2;
struct BattleSpecialTriangleAttackSpriteReq1;
struct BattleSpecialTriangleAttackSpriteReq2;
struct BattleSpecialTriangleAttackSpriteReq3;
struct BattleSpecialFascinationColor1;
struct BattleSpecialFascinationColor2;
struct BattleSpecialFascinationColor3;
struct BattleSpecialFascinationModelReq;
struct BattleStatusOverlayOffsets;
struct BattleSummonAlexanderDefaultPosition;
struct BattleFxBillboardVector;
struct BattleSummonBahamutZeroSpriteParams;
struct BattleLimitFuryBrandModelReqA;
struct BattleLimitFuryBrandSpriteReqB;
struct BattleSummonHadesRadialData;
struct BattleSummonKjataModelDrawRequest1;
struct BattleSummonKotrKnightLookAtPoints;
struct BattleSummonKotrStageRequestArgs;
struct BattleKotrFragmentData;
struct BattleFxNeoBahamutResIds;
struct BattleSummonOdinGAnimDataAC8;
struct BattleSummonOdinGAnimDataAF8;
struct BattleSpecMizufukiSpriteRequest2;
struct BattleSummonOdinGAnimDataB88;
struct BattleSummonOdinGAnimDataCC0;
struct BattleSummonOdinGAnimDataD10;
struct BattleSummonPhoenixAngleData1;
struct BattleSummonPhoenixAnimData1;
struct BattleSummonPhoenixAnimData3;
struct BattleSummonPhoenixCoordData1;
struct BattleSummonPhoenixCoordData2;
struct BattleSummonPhoenixCoordData3;
struct BattleSummonPhoenixCoordData4;
struct BattleSummonPhoenixSpriteRequest1;
struct BattleSummonPhoenixSpriteRequest3;
struct BattleSummonPhoenixSpriteRequest4;
struct BattleSummonPhoenixSpriteRequest5;
struct BattleSummonPhoenixSpriteRequest7;
struct SaferSephirothDeath_BlackModelDrawReq;
struct RubyTentaclesBaseModelVertices;
struct BoulderThrow_DisplacementVelocities;
struct BoulderThrow_SpriteResourcePointer;
struct YasuiLimitSatelliteBeamTransformMatrix;
struct YasuiLimitSatanSlamSourceMeshAndPoseBlob;
struct YasuiLimitSatanSlamWorkingMeshBlob;
struct Battle3dBoneVector;
struct Battle3dScene;
struct BattleDethperMeshRecord;
struct BattleFxShardTriSlot;
struct BattleFxQuake2ShardRecord;
struct BattleFxSatanSlamFragmentPrimitive;
struct BattleFxSatanSlamFragmentRecord;
struct BattleMagicShieldFaceList;
struct BattleFxOffsetTriMeshDesc;
struct BattleFxShardTransformBatch;
struct BattleGteViewMatrixScratch;
struct BattleBlueBetaParticle;
struct BattleBlueShadowFlareSpriteReq2;
struct BattleLimitCosmoMemoryAnimatedSpriteRequest;
struct BattleLimitCosmoMemoryFadingSpriteRequest;
struct BattleLimitCosmoMemoryGrowingModelRequest;
struct BattleLimitCosmoMemoryCentroidModelRequest;
struct BattleLimitCosmoMemoryShrinkingModelRequest;
struct BattleLimitCosmoMemoryExpandingModelRequest;
struct BattleLimitCosmoMemoryConvergingSpriteRequest;
struct BattleLimitCosmoMemoryRingSpriteRequest;
struct BattleLimitCosmoMemoryTriSpinModelRequest;
struct BattleLimitCosmoMemoryTriSpinSpriteRequest;
struct BattleLimitDoomOfTheLivingModelRequest;
struct BattleLimitDoomOfTheLivingSpriteRequest1;
struct BattleLimitDoomOfTheLivingSpriteRequest2;
struct BattleLimitDoomOfTheLivingTimingTable1;
struct BattleLimitStardustRayModelReq3;
struct BattleMagicBerserk_SpriteReq;
struct BattleMagicBio3Multi_ModelData1;
struct BattleMagicBio3Multi_SpriteDrawReq1;
struct BattleMagicRemove_SpriteReq;
struct BattleMagicDespellReferenceVector;
struct BattleMagicFlareSpriteReq1;
struct BattleMagicFlareSpriteReq2;
struct BattleMagicBolt_SpriteReq;
struct BattleSpecLightShellSpriteReq1;
struct BattleSpecialPoisonBreathColor1;
struct BattleSpecialEnergyOrbColor1;
struct BattleSpecialEnergyOrbModelReqIsi;
struct BattleSpecialEnergyOrbRotationTrajectory;
struct BattleSpecialEnergyOrbScaleTama;
struct BattleSpecialShieldModelReq;
struct BattleFxActorBoneMatrixBuf;
struct BattleSummonKjataSpriteRequest1;
struct BattleSummonKjataVertexData1;
struct BattleFxNeoBahamutShardMatrixBuf;
struct BattleFxNeoBahamutModelReq;
struct BattleFxEulerAngles16;
struct BattleSummonOdinZTrackMatrix;
struct BattleSummonOdinZSpriteDrawRequest;
struct BattleSummonOdinZTaskParam;
struct BattleSummonPhoenixAnimData2;
struct BattleSummonRamuhMeshSource1;
struct YasuiLimitSatanSlamSpriteRequest;
typedef void(__cdecl *BattleTaskFn)(void);
typedef void(__cdecl *BattleDmgFormulaFn)(void);
typedef void(__cdecl *BattleDmgOpFn)(void);
/* The battle task and effect-draw entry points every effect TU calls, with their definitions'
 * types. C++ TUs declare them with the types their calls pass (C++ converts no other pointer
 * type to these), so these are for C only. */
struct BattleFxModelDraw;
short __cdecl Battle_AllocateResourceSlot(BattleTaskFn callback);
void *__cdecl BattleFx_InstallBillboardMatrix(void *srcPoint, short scale, int pushLength);
int __cdecl BattleFx_SubmitModelDraw(void *modelDraw, int listHandle, int drawMode, int listHead);
void __cdecl Battle_CreateBitPatternCallbackTask(unsigned int mask, int callbackArg, int interval, void *callback);
struct BattleEffectScratchHead;
extern struct BattleEffectScratchHead g_BattleEffectScratch;
extern void (__cdecl *g_BattleDamageFormulaTable[32])(void);
extern void (__cdecl *const g_BattleDamageOpTable[8])(void);

/* Defined in battle_vm_register_slots.cpp as C++ `int &` references (ABI-identical to `int *` in C). */
#ifdef __cplusplus
extern int &g_VmSlot312_00c05fe8;
#ifndef g_BattleVmReg_0
#define g_BattleVmReg_0 g_VmSlot312_00c05fe8
#endif
extern int &g_BattleVmReg_1;
extern int &g_BattleVmReg_2;
extern int &g_BattleVmReg_3;
extern int &g_BattleVmReg_4;
extern int &g_BattleVmSlot_5;
#ifndef g_BattleVmReg_5
#define g_BattleVmReg_5 g_BattleVmSlot_5
#endif
extern int &g_BattleVmWord_6;
#ifndef g_BattleVmReg_6
#define g_BattleVmReg_6 g_BattleVmWord_6
#endif
extern int &g_BattleVmVar_7;
#ifndef g_BattleVmReg_7
#define g_BattleVmReg_7 g_BattleVmVar_7
#endif
#else
extern int *g_VmSlot312_00c05fe8;
#ifndef g_BattleVmReg_0
#define g_BattleVmReg_0 g_VmSlot312_00c05fe8
#endif
extern int *g_BattleVmReg_1;
extern int *g_BattleVmReg_2;
extern int *g_BattleVmReg_3;
extern int *g_BattleVmReg_4;
extern int *g_BattleVmSlot_5;
#ifndef g_BattleVmReg_5
#define g_BattleVmReg_5 g_BattleVmSlot_5
#endif
extern int *g_BattleVmWord_6;
#ifndef g_BattleVmReg_6
#define g_BattleVmReg_6 g_BattleVmWord_6
#endif
extern int *g_BattleVmVar_7;
#ifndef g_BattleVmReg_7
#define g_BattleVmReg_7 g_BattleVmVar_7
#endif
#endif

/* b3ddata.cpp */
int __cdecl Battle3d_GetSceneRenderObject(int selector, void *record);
int __cdecl Battle3d_GetSceneRenderObjectHigh(int selector, struct Battle3dScene *record);
void __cdecl Battle3d_DestroySceneRenderObjects(int sceneRecord);
void __cdecl Battle3d_CreateSceneRenderObjects(struct Battle3dScene *sceneRecord);
void __cdecl Battle3d_DestroySeffectRenderObjects(int seffectRecord);
void __cdecl Battle3d_CreateSeffectRenderObjects(struct Battle3dScene *seffectRecord);
void *Battle_Init3dData(void);
int __cdecl Battle3d_GetStageInfoBlockRaw(int record);
int __cdecl Battle3d_GetStagePartCount(struct Battle3dScene *stage);
int __cdecl Battle3d_GetStageDescriptor(struct Battle3dScene *stage);
void __cdecl Battle_LoadStageModel(int forceFlag, int stageNum, void *outModel);
void __cdecl Battle_DestroyScene(void);
void __cdecl Battle_ResetSceneFramePools(void);
void __cdecl Battle_DrawLayer(int context, int layer);
void __cdecl Battle_EffectFreeGroupTag0(void);
int __cdecl Battle3d_FadeLevelToAlpha(int fadeLevel);
void __cdecl Battle3d_FreeEffectModelBank(int effect);
void __cdecl Battle3d_SetEffectModelBank(struct BattleLimitAnimBank *bank, int effect);
void __cdecl Battle3d_GetActorAttachMatrix(int actorIndex, int attachIndex, struct PsxMatrix *outMatrix);
int __cdecl Battle_GetActorAnimAngle(int actorIndex);
void __cdecl Battle3d_ClearDrawTable(int count, struct BattleActorDrawEntry *table);
void __cdecl Battle_DrawActorRange(int pass, int startIndex, int count, struct BattleActorDrawEntry *drawTable);
void __cdecl Battle3d_DrawActorRangeUnsuppressed(int pass, int startIndex, int count, struct BattleActorDrawEntry *entry);
void __cdecl Battle3d_ResetActorDrawTable(void);
void __cdecl Battle3d_SweepActorDrawTable(int pass);
void __cdecl Battle_DrawHudSprites(int pass);
int __cdecl Battle3d_GetActorDrawEntry(int index);
void __cdecl Battle3d_QueueActorDraw(int enabled, int drawParam, int actorIndex);
void __cdecl Battle3d_ReleaseOffscreenSlot(int slot);
int __cdecl Battle3d_AcquireOffscreenRequest(void);
int __cdecl Battle3d_GetOffscreenRenderObject(int slot);
void __cdecl Battle3d_ServiceOffscreenSlots(void);
void __cdecl BattleModel_ConfigureLoadCategory(int forceFlag, int category, struct Battle3dPartyLoadDesc *loadDesc, struct PfileLoadOptions *modelDesc);

/* battle.cpp */
extern int g_BattleDarkenScreenGlobalAlphaEnabled;
extern int g_BattleSceneRecordLoaded;
extern struct Timestamp64 g_BattleTimestampCounterSample;
extern struct Timestamp64 g_BattleFrameStepTicks;
extern char g_BattleDataDirectoryPathBuffer[256];
extern int g_BattleSequenceWaitPlayed;
extern int g_BattleSequenceEndReached;
extern short g_BattleDefeatMusicPending;
extern short g_BattleAtbFrozen;
extern int g_BattleEndSequenceDone;
extern int g_BattleViewportOriginX;
extern int g_BattleViewportOriginY;
extern int g_BattleArenaChooserPending;
extern int g_BattleViewportWidth;
extern int g_BattleArenaChooserDone;
extern int g_BattleSceneViewportHeight;
extern unsigned short g_BattleResultMenuNextMode;
extern struct Timestamp64 g_BattleSessionStartTimestamp;
extern int g_BattleArenaSlotPenaltyDone;
extern int g_BattleArenaRoundBreakDone;
extern int g_BattleSequenceWaitDone;
extern struct BattleDamageDisplayRecord g_BattleDamageDisplayRecords[128];
extern int g_BattleViewportMaxX;
extern int g_BattleRoundSequenceDone;
extern int g_BattleUiScalingMode;
extern int g_BattleSceneLoadDone;
extern int g_BattleSequencePlayDone;
extern struct Timestamp64 g_BattleFrameLimiterLastTimestamp;
extern struct Timestamp64 g_BattleTimestampScaledAccumulator;
extern int g_BattleArenaSlotSequenceActive;
extern int g_BattleDigitGlyphBase;
extern int g_BattleModeTransition;
extern unsigned char g_BattleViewportMaxY[8];
extern int g_BattleUiScale;
extern int g_BattleIsRunning;
extern unsigned char g_BattleSavedPartyBytes[4];
extern int g_BattleReplayActionCount;
extern int g_BattleDebugKeyState;
void __cdecl Battle_ModuleEnter(struct GfxEngineContext *ctx);
void __cdecl Battle_ModuleExit(struct GfxEngineContext *ctx);
void __cdecl Battle_FrameLimiterWait(struct GfxEngineContext *ctx);
void __cdecl Battle_ModuleFrame(struct GfxEngineContext *ctx);
void __cdecl Battle_ClearWorkRam(void);
void __cdecl Battle_HandleDebugKeyMessage(int message, int key);
void __cdecl Battle_EmptyHook(void);

/* amptoanm.cpp */
short __cdecl BattleModel_ReadSignedDeltaFromBitstream(struct BattleModelAnimBank *stream, int *bitPos);
int __cdecl BattleModel_ReadSignedBits(struct BattleModelAnimBank *stream, int *bitPos, int count);
short __cdecl BattleModel_ReadAngleDeltaFromBitstream(struct BattleModelAnimBank *stream, int *bitPos, int shift);
void *BattleModel_OpenAnimFileWithHeader(int animSubIndex, int headerValue, char *modelName);
void __cdecl BattleModel_MakeAnimName(int index, char *modelName, char *outPath);
void __cdecl BattleModel_CloseAnimFile(int value);
void __cdecl Battle_WriteAnimDumpRecord(unsigned int frameCount, unsigned int boneCount, int size, struct Battle3dDescriptorFile *buffer, int fileHandle);
void __cdecl BattleModel_LoadAnimBank(int negateRootY, int animSubIndex, void *animArray, void *fileCtx, char *modelName);
void *BattleModel_LoadSingleAnimation(int negateRootY, int animSubIndex, void *fileCtx, char *modelName);
void __cdecl BattleModel_DumpAnimRecordToFile(int boneCount, int frameCount, int byteCount, struct Battle3dDescriptorFile *buffer, int animClass, char *animIndex);

/* battle_bin_section_decompress.cpp */
extern unsigned char *g_BinReaderStream;
void __cdecl BinSectionReaderInit(unsigned char *stream);
short __cdecl BinSectionPeekType(void);
int __cdecl BinSectionRead(void *dest);
int __cdecl BinExtractSectionByType(unsigned char *stream, void *dest, int targetType);
void __cdecl BinInflateSectionAndClearTail(struct BattleBinSection *src, char *dest);

/* battle_camera_script_tables.cpp */
extern unsigned char g_BattleCameraPositionScript000[];
extern unsigned char g_BattleCameraFocalScript000[];
extern unsigned char g_BattleCameraPositionScript001[];
extern unsigned char g_BattleCameraFocalScript001[];
extern unsigned char g_BattleCameraPositionScript002[];
extern unsigned char g_BattleCameraFocalScript002[];
extern unsigned char g_BattleCameraPositionScript003[];
extern unsigned char g_BattleCameraFocalScript003[];
extern unsigned char g_BattleCameraPositionScript004[];
extern unsigned char g_BattleCameraFocalScript004[];
extern unsigned char g_BattleCameraPositionScript005[];
extern unsigned char g_BattleCameraFocalScript005[];
extern unsigned char g_BattleCameraPositionScript006[];
extern unsigned char g_BattleCameraFocalScript006[];
extern unsigned char g_BattleCameraPositionScript007[];
extern unsigned char g_BattleCameraFocalScript007[];
extern unsigned char g_BattleCameraPositionScript008[];
extern unsigned char g_BattleCameraFocalScript008[];
extern unsigned char g_BattleCameraPositionScript009[];
extern unsigned char g_BattleCameraFocalScript009[];
extern unsigned char g_BattleCameraPositionScript010[];
extern unsigned char g_BattleCameraFocalScript010[];
extern unsigned char g_BattleCameraPositionScript011[];
extern unsigned char g_BattleCameraFocalScript011[];
extern unsigned char g_BattleCameraPositionScript012[];
extern unsigned char g_BattleCameraFocalScript012[];
extern unsigned char g_BattleCameraPositionScript013[];
extern unsigned char g_BattleCameraFocalScript013[];
extern unsigned char g_BattleCameraPositionScript014[];
extern unsigned char g_BattleCameraFocalScript014[];
extern unsigned char g_BattleCameraPositionScript015[];
extern unsigned char g_BattleCameraFocalScript015[];
extern unsigned char g_BattleCameraPositionScript016[];
extern unsigned char g_BattleCameraFocalScript016[];
extern unsigned char g_BattleCameraPositionScript017[];
extern unsigned char g_BattleCameraFocalScript017[];
extern unsigned char g_BattleCameraPositionScript018[];
extern unsigned char g_BattleCameraFocalScript018[];
extern unsigned char g_BattleCameraPositionScript019[];
extern unsigned char g_BattleCameraFocalScript019[];
extern unsigned char g_BattleCameraPositionScript020[];
extern unsigned char g_BattleCameraFocalScript020[];
extern unsigned char g_BattleCameraPositionScript021[];
extern unsigned char g_BattleCameraFocalScript021[];
extern unsigned char g_BattleCameraPositionScript022[];
extern unsigned char g_BattleCameraFocalScript022[];
extern unsigned char g_BattleCameraPositionScript023[];
extern unsigned char g_BattleCameraFocalScript023[];
extern unsigned char g_BattleCameraPositionScript024[];
extern unsigned char g_BattleCameraFocalScript024[];
extern unsigned char g_BattleCameraPositionScript025[];
extern unsigned char g_BattleCameraFocalScript025[];
extern unsigned char g_BattleCameraPositionScript026[];
extern unsigned char g_BattleCameraFocalScript026[];
extern unsigned char g_BattleCameraPositionScript027[];
extern unsigned char g_BattleCameraFocalScript027[];
extern unsigned char g_BattleCameraPositionScript028[];
extern unsigned char g_BattleCameraFocalScript028[];
extern unsigned char g_BattleCameraPositionScript029[];
extern unsigned char g_BattleCameraFocalScript029[];
extern unsigned char g_BattleCameraPositionScript030[];
extern unsigned char g_BattleCameraFocalScript030[];
extern unsigned char g_BattleCameraPositionScript031[];
extern unsigned char g_BattleCameraFocalScript031[];
extern unsigned char g_BattleCameraPositionScript032[];
extern unsigned char g_BattleCameraFocalScript032[];
extern unsigned char g_BattleCameraPositionScript033[];
extern unsigned char g_BattleCameraFocalScript033[];
extern unsigned char g_BattleCameraPositionScript034[];
extern unsigned char g_BattleCameraFocalScript034[];
extern unsigned char g_BattleCameraPositionScript035[];
extern unsigned char g_BattleCameraFocalScript035[];
extern unsigned char g_BattleCameraPositionScript036[];
extern unsigned char g_BattleCameraFocalScript036[];
extern unsigned char g_BattleCameraPositionScript037[];
extern unsigned char g_BattleCameraFocalScript037[];
extern unsigned char g_BattleCameraPositionScript038[];
extern unsigned char g_BattleCameraFocalScript038[];
extern unsigned char g_BattleCameraPositionScript039[];
extern unsigned char g_BattleCameraFocalScript039[];
extern unsigned char g_BattleCameraPositionScript040[];
extern unsigned char g_BattleCameraFocalScript040[];
extern unsigned char g_BattleCameraPositionScript041[];
extern unsigned char g_BattleCameraFocalScript041[];
extern unsigned char g_BattleCameraPositionScript042[];
extern unsigned char g_BattleCameraFocalScript042[];
extern unsigned char g_BattleCameraPositionScript043[];
extern unsigned char g_BattleCameraFocalScript043[];
extern unsigned char g_BattleCameraPositionScript044[];
extern unsigned char g_BattleCameraFocalScript044[];
extern unsigned char g_BattleCameraPositionScript045[];
extern unsigned char g_BattleCameraFocalScript045[];
extern unsigned char g_BattleCameraPositionScript046[];
extern unsigned char g_BattleCameraFocalScript046[];
extern unsigned char g_BattleCameraPositionScript047[];
extern unsigned char g_BattleCameraFocalScript047[];
extern unsigned char g_BattleCameraPositionScript048[];
extern unsigned char g_BattleCameraFocalScript048[];
extern unsigned char g_BattleCameraPositionScript049[];
extern unsigned char g_BattleCameraFocalScript049[];
extern unsigned char g_BattleCameraPositionScript050[];
extern unsigned char g_BattleCameraFocalScript050[];
extern unsigned char g_BattleCameraPositionScript051[];
extern unsigned char g_BattleCameraFocalScript051[];
extern unsigned char g_BattleCameraPositionScript052[];
extern unsigned char g_BattleCameraFocalScript052[];
extern unsigned char g_BattleCameraPositionScript053[];
extern unsigned char g_BattleCameraFocalScript053[];
extern unsigned char g_BattleCameraPositionScript054[];
extern unsigned char g_BattleCameraFocalScript054[];
extern unsigned char g_BattleCameraPositionScript055[];
extern unsigned char g_BattleCameraFocalScript055[];
extern unsigned char g_BattleCameraPositionScript056[];
extern unsigned char g_BattleCameraFocalScript056[];
extern unsigned char g_BattleCameraPositionScript057[];
extern unsigned char g_BattleCameraFocalScript057[];
extern unsigned char g_BattleCameraPositionScript058[];
extern unsigned char g_BattleCameraFocalScript058[];
extern unsigned char g_BattleCameraPositionScript059[];
extern unsigned char g_BattleCameraFocalScript059[];
extern unsigned char g_BattleCameraPositionScript060[];
extern unsigned char g_BattleCameraFocalScript060[];
extern unsigned char g_BattleCameraPositionScript061[];
extern unsigned char g_BattleCameraFocalScript061[];
extern unsigned char g_BattleCameraPositionScript062[];
extern unsigned char g_BattleCameraFocalScript062[];
extern unsigned char g_BattleCameraPositionScript063[];
extern unsigned char g_BattleCameraFocalScript063[];
extern unsigned char g_BattleCameraPositionScript064[];
extern unsigned char g_BattleCameraFocalScript064[];
extern unsigned char g_BattleCameraPositionScript065[];
extern unsigned char g_BattleCameraFocalScript065[];
extern unsigned char g_BattleCameraPositionScript066[];
extern unsigned char g_BattleCameraFocalScript066[];
extern unsigned char g_BattleCameraPositionScript067[];
extern unsigned char g_BattleCameraFocalScript067[];
extern unsigned char g_BattleCameraPositionScript068[];
extern unsigned char g_BattleCameraFocalScript068[];
extern unsigned char g_BattleCameraPositionScript069[];
extern unsigned char g_BattleCameraFocalScript069[];
extern unsigned char g_BattleCameraPositionScript070[];
extern unsigned char g_BattleCameraFocalScript070[];
extern unsigned char g_BattleCameraPositionScript071[];
extern unsigned char g_BattleCameraFocalScript071[];
extern unsigned char g_BattleCameraPositionScript072[];
extern unsigned char g_BattleCameraFocalScript072[];
extern unsigned char g_BattleCameraPositionScript073[];
extern unsigned char g_BattleCameraFocalScript073[];
extern unsigned char g_BattleCameraPositionScript074[];
extern unsigned char g_BattleCameraFocalScript074[];
extern unsigned char g_BattleCameraPositionScript075[];
extern unsigned char g_BattleCameraFocalScript075[];
extern unsigned char g_BattleCameraPositionScript076[];
extern unsigned char g_BattleCameraFocalScript076[];
extern unsigned char g_BattleCameraPositionScript077[];
extern unsigned char g_BattleCameraFocalScript077[];
extern unsigned char g_BattleCameraPositionScript078[];
extern unsigned char g_BattleCameraFocalScript078[];
extern unsigned char g_BattleCameraPositionScript079[];
extern unsigned char g_BattleCameraFocalScript079[];
extern unsigned char g_BattleCameraPositionScript080[];
extern unsigned char g_BattleCameraFocalScript080[];
extern unsigned char g_BattleCameraPositionScript081[];
extern unsigned char g_BattleCameraFocalScript081[];
extern unsigned char g_BattleCameraPositionScript082[];
extern unsigned char g_BattleCameraFocalScript082[];
extern unsigned char g_BattleCameraPositionScript083[];
extern unsigned char g_BattleCameraFocalScript083[];
extern unsigned char g_BattleCameraPositionScript084[];
extern unsigned char g_BattleCameraFocalScript084[];
extern unsigned char g_BattleCameraPositionScript085[];
extern unsigned char g_BattleCameraFocalScript085[];
extern unsigned char g_BattleCameraPositionScript086[];
extern unsigned char g_BattleCameraFocalScript086[];
extern unsigned char g_BattleCameraPositionScript087[];
extern unsigned char g_BattleCameraFocalScript087[];
extern unsigned char g_BattleCameraPositionScript088[];
extern unsigned char g_BattleCameraFocalScript088[];
extern unsigned char g_BattleCameraPositionScript089[];
extern unsigned char g_BattleCameraFocalScript089[];
extern unsigned char g_BattleCameraPositionScript090[];
extern unsigned char g_BattleCameraFocalScript090[];
extern unsigned char g_BattleCameraPositionScript091[];
extern unsigned char g_BattleCameraFocalScript091[];
extern unsigned char g_BattleCameraPositionScript092[];
extern unsigned char g_BattleCameraFocalScript092[];
extern unsigned char g_BattleCameraPositionScript093[];
extern unsigned char g_BattleCameraFocalScript093[];
extern unsigned char g_BattleCameraPositionScript094[];
extern unsigned char g_BattleCameraFocalScript094[];
extern unsigned char g_BattleCameraPositionScript095[];
extern unsigned char g_BattleCameraFocalScript095[];
extern unsigned char g_BattleCameraPositionScript096[];
extern unsigned char g_BattleCameraFocalScript096[];
extern unsigned char g_BattleCameraPositionScript097[];
extern unsigned char g_BattleCameraFocalScript097[];
extern unsigned char g_BattleCameraPositionScript098[];
extern unsigned char g_BattleCameraFocalScript098[];
extern unsigned char g_BattleCameraPositionScript099[];
extern unsigned char g_BattleCameraFocalScript099[];
extern unsigned char g_BattleCameraPositionScript100[];
extern unsigned char g_BattleCameraFocalScript100[];
extern unsigned char g_BattleCameraPositionScript101[];
extern unsigned char g_BattleCameraFocalScript101[];
extern unsigned char g_BattleCameraPositionScript102[];
extern unsigned char g_BattleCameraFocalScript102[];
extern unsigned char *g_BattleCameraPositionScripts[103];
extern unsigned char *g_BattleCameraFocalScripts[103];

/* battle_limit_fx_thunks.cpp */
extern int (*g_BattleFx_GeneralDispatchTable[70])();
extern int (*g_BattleFx_MagicDispatchTable[54])();
extern int (*g_BattleFx_BlueMagicDispatchTable[24])();
extern int (*g_BattleFx_SpecialDispatchTable[158])();
extern int (*g_BattleFx_LimitDispatchTable[80])();
extern int (*g_BattleFx_ThrowWeaponDispatchTable[128])();
extern int g_BattleVmSlotStorage_0;
extern int g_BattleVmSlotStorage_1;
extern int g_BattleVmSlotStorage_2;
extern int g_BattleVmSlotStorage_3;
extern int g_BattleVmSlotStorage_4;
extern int g_BattleVmSlotStorage_5;
extern int g_BattleVmSlotStorage_6;
extern int g_BattleVmSlotStorage_7;
extern unsigned char g_BattleActorAnimStateBuffer[];
extern unsigned char g_BattleRingBufferTags[64];
extern unsigned char g_BattleRingBufferRecords[320];
extern int g_BattleRingBufferWriteIndex;
extern int g_BattleRingBufferReadIndex;
extern struct BattleScreenFlashRamp *g_BattleScreenFlashRampTaskHandle;
extern struct BattleFixedPointRamp *g_BattleFixedPointRampTaskHandle;
extern char g_BattleLimitFlashModel2Started;
extern int g_BattleLimitFlashModel0Data;
extern char g_BattleLimitFlashModel0Started;
extern int g_BattleLimitFlashModel1Data;
extern int g_BattleLimitFlashModel2Data;
extern char g_BattleLimitFlashModel1Started;
extern struct GfxTableCursor *g_pBattleLimitFlashTable;
extern short g_BattleLimitFlashModel1Matrix;
extern short g_BattleLimitFlashModel1Scale;
extern short g_BattleLimitFlashModel2Matrix;
extern short g_BattleLimitFlashModel2Scale;
extern char g_BattleCameraFocusScriptActive;

/* battle_gfx_offscreen_slots.cpp */
extern unsigned char g_GfxOffscreenSlots[];
void __cdecl Gfx_ReleaseAllOffscreenSlots(void);
void __cdecl Gfx_ReleaseOffscreenSlotByIndex(int index);
int __cdecl Gfx_ClaimOffscreenSlotRequest(struct GfxEngineContext *context);
int __cdecl Gfx_GetOffscreenSlotRenderObject(int index);
void __cdecl Gfx_ServiceAllOffscreenSlots(void);
void __cdecl Gfx_DrawAllOffscreenSlots(int value);

/* bdata.cpp */
extern char *gBattleDataDirectory;
extern char *gBattleFxResourceRoot;
extern char *gBattleFxResourceRootAlt;
void __cdecl Battle_InitLmdLoadDesc(int *desc);
void __cdecl Battle_SetDataDirectory(char *dir);
void __cdecl Battle_PushDataDirectory(int *saveSlot);
void __cdecl Battle_PopDataDirectory(int *saveSlot);
char *__cdecl Battle_BuildDataPath(char *leaf, char *out);
void __cdecl BattleFx_SetResourceRoot(char *dir);
void __cdecl BattleFx_PushResourceDirectory(void *fileCtx, int *saveSlot);
void __cdecl BattleFx_PopResourceDirectory(void *fileCtx, int *saveSlot);
void __cdecl BattleFx_SetResourceRootAlt(char *dir);
void *__cdecl Battle_LoadLmdModelByName(int wantMatrixSet, unsigned int blendMode, char *fileName, void *timObject);
void *__cdecl Battle_LoadShadowModel(int slot);
char *__cdecl BattleFx_GetResourceRootAlt(void);

/* char.cpp */
extern unsigned char g_BattleModel_EnemyFlag256;
extern unsigned char g_BattleModel_EnemyFlag300;
extern struct BattleModelEnemyBlendOverrides g_BattleModelEnemyBlendOverrides;
extern unsigned char g_BattleModel_EnemyBlendMode252;
extern unsigned char g_BattleModel_EnemyBlendMode332;
void __cdecl BattleModel_FreeBank(struct BattleLimitAnimBank *bank);
void *__cdecl BattleModel_AllocBank(int animCount, int weaponCount);
void *__cdecl BattleModel_LoadPartyDatLegacy(struct Battle3dPartyLoadDesc *battleLoadDesc, void *modelCtx, char *datName);
void __cdecl BattleModel_FreeDescriptor(int freeSub, void *model);
void BattleModel_FreeDescriptorWithBones(void *model);
int __cdecl BattleModel_AllocDescriptor(int id, int count);
void *__cdecl BattleModel_LoadDescriptor(int useCallerCtx, void *fileCtx, char *modelName);
void __cdecl BattleModel_DrawAnimFrame(int useWeaponBank, int animIndex, int frameIndex, struct BattleModelBank *bank);
struct Battle3dActorModel;
void __cdecl BattleModel_DrawAnimFrameOnSubSkeleton(int subFlag, int skelIndex, int frameTime, int partIndex, struct Battle3dActorModel *actor);
void *__cdecl BattleModel_LoadEnemy(int forceLegacy, int enemyModelId, struct Battle3dPartyLoadDesc *battleLoadDesc, struct PfileLoadOptions *pfileLoadOptions);
void *__cdecl BattleModel_LoadPartyMember(int category, int partyIndex, struct Battle3dPartyLoadDesc *loadDesc, struct PfileLoadOptions *model, void *context);
void *__cdecl BattleStage_LoadModelSet(int forceLegacy, int stageId, struct Battle3dPartyLoadDesc *battleLoadDesc, struct PfileLoadOptions *pfileLoadOptions, char *stageName);
void BattleModel_WriteDescriptorFile(struct Battle3dDescriptorFile *descriptor, char *modelName);
void __cdecl BattleModel_GatherSkeletonBoneVectors(struct Battle3dBoneVector *dst, struct Battle3dSkeleton *skeleton);
void __cdecl BattleModel_WriteInfoFile(int size, void *data, char *modelName);
void *__cdecl BattleModel_LoadBank(int useCallerCtx, int weaponIndex, struct Battle3dPartyLoadDesc *descriptor, struct Battle3dPartyLoadDesc *battleLoadDesc, struct PfileLoadOptions *pfileLoadOptions, char *modelName);
void __cdecl BattleModel_SetLoadFlags(struct PfileLoadOptions *model);
struct Battle3dLimitAnimBank;
void __cdecl Battle_LoadLimitAnimBank(struct Battle3dLimitAnimBank *bank, struct Battle3dPartyLoadDesc *battleLoadDesc, struct PfileLoadOptions *modelCtx, char *charName);
void __cdecl BattleModel_InitEnemyBlendOverrideTables(void);
void __cdecl Battle_ConvertEnemyModelTool(void);
void BattleModel_RenameBattleFilesToMangledNames(void);
void BattleModel_RenameFilesMatchingPatternToMangledNames(void *wildcardPattern);
void __cdecl BattleModel_NameManglerHook(char *name, char *out);
void __cdecl BattleModel_MangleName(char *srcName, char *outName4);

/* enemy.cpp */
void *__cdecl BattleModel_LoadEnemyDatLegacy(struct Battle3dPartyLoadDesc *battleLoadDesc, void *modelCtx, char *datName);

/* lmd.cpp */
void __cdecl Lmd_DestroyTextureAtlas(struct Battle3dLmdFaceData *atlas);
void *__cdecl Lmd_CreateTextureAtlas(int owner, void *timObject);
int __cdecl Battle_BuildPolygonDataFromLmd(struct Battle3dPartyLoadDesc *desc, int count, struct Battle3dLmdFaceData *faceData, struct PolygonTim *lmdHeader);
void __cdecl Lmd_BuildAtlasTextures(struct Battle3dLmdFaceData *lmdFile, struct PolygonTim *texCtx);
void __cdecl BattleModel_MakePartName(int index, char *modelName, char *outPath);
void __cdecl BattleModel_MakeWeaponName(int index, char *modelName, char *outPath);
void *__cdecl BattleModel_LoadPartMesh(struct Battle3dPartyLoadDesc *loadDesc, int count, int baseName, struct PolygonFileArrays *polyArrays, struct PolygonTim *unused, struct PfileLoadOptions *loader, struct Battle3dLmdFaceData *reserved, struct GfxEngineContext *context);
void *__cdecl Lmd_BuildAndRegisterPart(struct Battle3dPartyLoadDesc *loadDesc, int partIndex, struct PolygonTim *tim, void *loader, struct Battle3dLmdFaceData *lmdFile, char *baseName);

/* limitbrk.cpp */
void __cdecl Battle_FreeLimitAnims(struct BattleLimitAnimBank *bank);
void __cdecl Battle_ConvertLimitAnimsLegacy(int unused, struct Battle3dPartyLoadDesc *battleLoadDesc, struct BattleLimitAnimBank *bank, char *charName);

/* mdl.cpp */
void *__cdecl BattleModel_BuildSkeletonFromLmd(struct Battle3dPartyLoadDesc *battleLoadDesc, int unusedA, int unusedB, int *modelBlob, struct PolygonTim *timObject, void *loadOptions, char *baseName);

/* stage.cpp */
void __cdecl BattleStage_FreeStageDatModel(struct BattleStageDat *stage);
void *__cdecl BattleStage_LoadStageDatLegacy(int stageId, int *battleLoadDesc, void *loadOptions, char *datName);
extern struct BattleStageFloorFlags g_BattleStageFloorFlagTable;
void __cdecl BattleStage_InitFloorFlagTable(void);
void __cdecl BattleStage_FreePartMesh(int partNumber, int stage);
void *__cdecl BattleStage_GetPartMesh(int partNumber, struct Battle3dStageRecord *stage);
int __cdecl BattleStage_UsesDepthTestedFloor(int stage);
void __cdecl BattleStage_SubmitPartGeometry(unsigned int packedColorAlpha, int partIndex, struct Battle3dStageRecord *stageObject);

/* battle_actor_overlay_draw.cpp */
extern struct BattleCameraChannel g_BattleCameraFocusChannels[4];
void __cdecl Battle_EnqueueMessageWindow(short textId, char windowType, int duration, short param);
extern struct VECTOR g_BattleOverlayVertexOffset;
extern struct BattleStatusTintEntry g_BattleOverlay_StatusTintTable[11];
extern struct PsxMatrix g_BattleCameraLookAt;
extern BattleTaskFn g_BattleAuxTaskCallbackArray[16];
extern unsigned char g_BattleActorIdleScripts[10];
extern unsigned char g_BattlePartyPlaceStep;
extern struct BattleOverlayRootLoadState g_BattleOverlayRootLoadState;
extern int g_BattlePartyStatusIconMasks[3];
extern int g_BattleOverlayDrawListHead;
void __cdecl Battle3d_DrawActorOverlays(char actorIndex);
int __cdecl Battle3d_DrawActorOverlayQuad(int actorIndex, void *position, short texU, short texV);
void __cdecl Battle3d_DrawActorOverlaySprite(char actorIndex, short texV);
void __cdecl Battle3d_DrawActorStatusIcon(char actorIndex, unsigned char countdown);
void __cdecl Battle3d_SplitDecimalDigits(short value, void *digits);
int __cdecl Battle3d_DrawActorStatusIconDigitQuads(int drawListHandle, void *position, char tensDigit, char onesDigit);
int __cdecl BattleFx_SubmitScreenTexturedQuad(int drawListHandle, int screenX, short screenY, short texU, short texV, short width, short height, short flags);
void __cdecl Battle_DamageNumberTask(void);
void __cdecl Battle_ActorFadeOutEffectTask(void);
void __cdecl Battle_PlayPannedSfxForActor(char actorIndex, short sfxParam, char enabled);
void __cdecl Battle_StatusPoseMode3UpdateActor(char poseSlot);
void __cdecl Battle_StatusPoseMode0Task(void);
void __cdecl Battle_StatusPoseMode0UpdateActor(char poseSlot);
void __cdecl Battle_OverlayTaskDeathSink(void);
void __cdecl Battle_OverlayTaskDeathSinkUpdate(char taskSlot);
void __cdecl Battle_StatusPoseMode2Task(void);
void __cdecl Battle_StatusPoseMode2UpdateActor(char taskSlot);
void __cdecl Battle_StatusPoseMode17Task(void);
void __cdecl Battle_StatusPoseMode17ShakeFlashTask(void);
void __cdecl Battle_StatusPoseMode17UpdateActor(char taskSlot);
void __cdecl Battle_StatusPoseMode7Task(void);
void __cdecl Battle_StatusPoseMode7UpdateActor(char taskSlot);
void __cdecl Battle_StatusPoseMode8Task(void);
void __cdecl Battle_StatusPoseMode8UpdateActor(char taskSlot);
void __cdecl Battle_EffectTaskScreenColourRamp(void);
void __cdecl BattleFx_ScreenBandWipeTask(void);
int __cdecl BattleFx_FlashScreenWhite(unsigned char red, unsigned char green, unsigned char blue);
void __cdecl BattleFx_DarkenScreenGlobalAlpha(char alpha, char green, char blue);
void __cdecl Battle_ApplyFrogModelSwap(char actor);
void __cdecl Battle3d_SyncMiniScaleState(char actor);
void __cdecl Battle3d_ActorScaleRampTask(void);
void __cdecl Battle3d_ScriptActorScaleRampTask(void);
void __cdecl Battle3d_RainInit(void);
void __cdecl Battle3d_RainUpdateAndDraw(void);
int __cdecl Battle3d_DrawRainSplash(int unused, short baseY);
int __cdecl Battle3d_DrawRainStreak(int orderingTable);
void __cdecl Battle_ClearMessageWindowQueue(void);
void __cdecl Battle_StepMessageWindowQueue(void);
void __cdecl Battle_SortPartyModelLoadOrder(void);
int __cdecl Battle_GetTextSpeedDelay(void);
void __cdecl Battle_TaskNop(void);

/* battle_actor_sequence_messages.cpp */
void __cdecl Battle_PostBattleMessageFormatted(int actor, int messageId, int mode, void *queueArg);
void __cdecl Battle_PostBattleMessage(int payload);
void __cdecl Battle_StopActorStatusEffect(int actor);
int __cdecl Battle_IsActorNearDeath(int actorSlot);
void __cdecl Battle_PushAnimStepWithHitRecord(int actorIndex, int animId, int stepFlags, int param, int power, int amount, int combatant);
int __cdecl Battle_ApplyStatPercentModifier(int actorIndex, int baseValue, int field);
void __cdecl Battle_AddStatPercentModifiers(int actorIndex, int delta, int mask);

/* battle_random_stream_helpers.cpp */
extern int g_BattleRngStreamIndex;
extern int g_BattleRngCallCount;
void __cdecl Battle_AdvanceActionSequenceCounter(void);
unsigned short __cdecl Battle_NextRandom16(void);
void __cdecl Battle_UnpackSceneFlagsToBytes(int sceneFlags);

/* battle_ai_vm_interpreter.cpp */
extern unsigned char *g_BattleAiVmBytecodePtr;
void __cdecl Battle_AiVm_WriteVar(int varIndex, unsigned int width, int address, int value);
int __cdecl Battle_AiVm_ResolveAddress(int actorIndex, int bitAddress, void **outBase);
unsigned int __cdecl Battle_AiVm_ReadVar(int varIndex, unsigned int width, int address);
int __cdecl Battle_AiVm_CollapseToScalar(int slot);
void __cdecl Battle_AiVm_Run(int actorIndex, unsigned char *bytecode, int partyOverride);
unsigned short __cdecl Battle_AiVm_FetchImm16(void);
void __cdecl Battle_AiVm_PushValue(int descriptor);
void __cdecl Battle_AiVm_PushRaw(int size, unsigned int value);
unsigned int __cdecl Battle_AiVm_PopValue(int operand);
unsigned int __cdecl Battle_AiVm_PopRaw(int size);
unsigned int __cdecl Battle_AiVm_PeekValue(int slot);
unsigned int __cdecl Battle_AiVm_EvalArithmetic(int leftLane, int rightLane);
int __cdecl Battle_AiVm_EvalCompare(int leftLane, int rightLane);
int __cdecl Battle_AiVm_AnyLaneNonZero(int slot);

/* battle_hit_damage_resolution.cpp */
extern unsigned short g_BattleActorCurrentTargetMask[10];
extern char g_BattleActorReflectSource[10];
extern int g_BattleAttackOpStateFlags;
extern short g_BattleActorNextTargetMask[10];
extern unsigned short g_BattleAttackReactedActorMask;
extern unsigned char g_BattleDataDirBuffer[16];
int __cdecl Battle_ChargeMpAndCheckCastable(void);
void __cdecl Battle_AiVm_PrepareGlobals(int actorIndex, int vmActor, int actionContext);
void __cdecl Battle_AiVm_PostAction(short actorIndex, unsigned short actionType, short actionArg);
int __cdecl Battle_AiVm_GetAttackMpCost(int attackId);
void __cdecl Battle_AiVm_AccessSavemapByte(int mode, int byteOffset);
void __cdecl Battle_AiVm_CopyCombatantState(int dstActor, int srcActor);
void __cdecl Battle_AttackOpExecute(void);
void __cdecl Battle_QueueAttackAnimStep(unsigned int targetMask);
void __cdecl Battle_FilterTargetsByEnemyLevel(void);
void __cdecl Battle_QueueActorHpMpFlushStep(int actorIndex, int flushMode);
void __cdecl Battle_FlushHpMpTransferPool(int actorIndex, int poolSelect);
int __cdecl Battle_PushDamageDisplay(struct BattleHitRecord *hitRecord, short displayParamA, short displayParamB, short displayParamC, short displayParamD);
void __cdecl Battle_ResolveHitOnTarget(int sourceActor, int targetActor);
int __cdecl Battle_AreActorsOnSameSide(int slotA, int slotB);
int __cdecl Battle_PickRandomOpposingActorMask(int actorIndex);
void __cdecl Battle_ApplyCoverRedirect(struct BattleHitRecord *hitRecord);
void __cdecl Battle_ApplyElementalPreDamage(void);
void __cdecl Battle_ApplyElementalPostDamage(void);
void __cdecl Battle_LearnEnemySkill(void);
void __cdecl Battle_SelectHitReactionPose(int altPose);
void __cdecl Battle_BeginTargetHit(int targetIndex);
void __cdecl Battle_ClearHitScratch(void);
void __cdecl Battle_AbortActionWithMessage(int messageId);
int __cdecl Battle_IsTargetImmuneToAttackKind(int targetSlot);
int __cdecl Battle_TargetTakesNoDamage(int targetSlot);
int __cdecl Battle_RollStatusChangeSuccess(void);
void __cdecl Battle_PushStatusOnlyDisplay(struct BattleHitRecord *displayRecord);

/* battle_ring_buffer_effects.cpp */
int __cdecl Battle_AllocSceneMessage(int tag);
int __cdecl Battle_DequeueSceneMessage(void *recordOut);
void __cdecl BattleFx_ScreenFlashRampTask(void);
void __cdecl BattleFx_FadeOutScreenFlashRamp(int duration);
void __cdecl Battle_StartFixedPointRamp(int targetValue, int duration);
void __cdecl Battle_FixedPointRampTaskHandler(void);
void __cdecl Battle_BitPatternCallbackTaskHandler(void);
int __cdecl Battle_PopCountActorMask(unsigned int actorMask);
short *__cdecl Battle_GetActorMaskCentroid(int mask, short *out);
int __cdecl Battle_GetActorScaledEffectSize(int actorIndex);
void __cdecl Battle_DispatchSfxOpcodeDirect(int opcode, int pan, int id);
int __cdecl Battle_ComputeScreenPanFromPosition(void *worldPosition);
int __cdecl Battle_ComputePanForEntity(int entityMask);
int __cdecl Battle_PostActorHitOrHurtMessage(int actorIndex);

/* battle_arena_penalty_rounds.cpp */
extern int g_ArenaSelectionResult;
extern int g_ArenaSlotPenaltyState;
extern int g_ArenaSlotPenaltyCounter;
extern int g_ArenaSlotPenaltySubState;
extern int g_ArenaRoundBreakState;
extern int g_ArenaRoundBreakCounter;
extern int g_ArenaRoundBreakSubState;
void __cdecl Battle_SetArenaSelectionResult(int selectedOption);
int __cdecl Battle_RunArenaSlotPenaltySelection(void);
int __cdecl Battle_RunArenaRoundBreak(void);

/* battle_assign_actor_sides.cpp */
void __cdecl Battle_AssignActorSideMasks(void);

/* battle_attack_op_handlers.cpp */
void __cdecl Battle_AttackOpSetAttackIdMagic(void);
void __cdecl Battle_AttackOpSetAttackIdSummon(void);
void __cdecl Battle_AttackOpSetItemId(void);
void __cdecl Kernel_ApplyWeaponToAction(void);
void __cdecl Battle_AttackOpSpendGilForPower(void);
void __cdecl Battle_AttackOpSetAttackIdEnemySkill(void);
void __cdecl Battle_AttackOpToggleFacing(void);
void __cdecl Battle_AttackOpNop(void);
void __cdecl Battle_AttackOpSetAttackIdLimit(void);
void __cdecl Battle_AttackOpSetAttackIdFromEnemyList(void);
void __cdecl Battle_AttackOpMakeManipulate(void);
void __cdecl Battle_AttackOpPostMessage(void);
void __cdecl Battle_AttackOpResolveTargets(void);
void __cdecl Battle_AttackOpTargetSelf(void);
void __cdecl Kernel_ApplyAttackToAction(void);
void __cdecl Kernel_ApplyItemToAction(void);
void __cdecl Battle_ApplyEquippedWeaponToAction(void);
void __cdecl Battle_AttackOpQueueAnimOnly(void);
void __cdecl Battle_AttackOpSetRepeat4(void);
void __cdecl Battle_AttackOpRunAiEventOnTargets(void);
void __cdecl Battle_AttackOpSetRepeat2(void);
void __cdecl Battle_AttackOpMakeSlipDamage(void);
void __cdecl Battle_AttackOpRedirectToEnemyActor(void);
void __cdecl Battle_AttackOpShowSenseReadout(void);
void __cdecl Battle_AttackOpReplayLastActions(void);
void __cdecl Battle_AttackOpMakeInstantDeath(void);
void __cdecl Battle_AttackOpMakeDeathblow(void);
void __cdecl Battle_AttackOpSetPower2(void);
void __cdecl Battle_AttackOpSetWeaponImpact(void);
void __cdecl Battle_SetTargetFlagsIfUnset(int flags);
void __cdecl Battle_SetAddedEffect(int effectId, int effectParam);
void __cdecl Battle_SetStatusChange(int mode, int statusMask);
void __cdecl Battle_ReapplyAttackRecord(void);
void __cdecl Battle_SetRepeatCasts(int repeatCount);

/* battle_damage_program_run.cpp */
extern const unsigned char g_abDamageStatProgramBlob[];
void __cdecl Battle_BuildDamageProgramTable(void);
void __cdecl Battle_RunDamageProgram(void);
void __cdecl Battle_DecodeDamageCalcByte(void);

/* battle_command_menu_queue.cpp */
void __cdecl Battle_BuildFrogCommandSubstitute(int actorIdx);
int __cdecl Battle_GetActorBaseCommandId(int actorIdx);
void __cdecl Battle_RequestCommandMenu(int actorIdx);
int __cdecl Battle_ReleaseCommandMenu(int actorIdx);
int __cdecl Battle_MapPartySlotToController(int partySlot);
void __cdecl Battle_SetLimitReady(int actorIdx);
void __cdecl Battle_ClearLimitReady(int actorIdx);
void __cdecl Battle_RefreshPendingCommandAvailability(int actorIdx, int selector);
int __cdecl Battle_UpdateSpellMenuAvailability(int actorIdx, int statusMask, int groupOffset, int count);
void __cdecl Battle_UpdateCommandAvailability(int actorIdx, int statusMask);
void __cdecl Battle_InitActorAbilityRecord(int actorIdx, int abilityIndex);

/* battle_action_queue_run.cpp */
extern int g_BattleSweepComplete;
extern int g_BattleActionDoneFlag;
extern int g_BattleAttackOpContinue;
extern int g_BattleActionQueuePhase;
void __cdecl Battle_ClearTrackedTurnActorIf(int actor);
int __cdecl Battle_RunActionQueue(int maxPriority);
void __cdecl Battle_ExecuteAction(struct BattleAction *actionPtr);
void __cdecl Battle_CloseActorTurn(int actor);
void __cdecl Battle_BuildAttackProgramTable(void);
void __cdecl Battle_CheckLucky7s(void);

/* battle_hp_mp_commit.cpp */
void __cdecl Battle_CommitHpMpChange(void);
void __cdecl Battle_AddHpMpTransferToPool(int actorIndex, int targetId, int amount, unsigned int flags);
void __cdecl Battle_QueueHitHpMpTransfer(void);

/* battle_camera_matrix_compose.cpp */
int __cdecl BattleFx_MakeViewSpacePointPushed(void *srcPoint, int pushLength, struct Vec3sPacked *outPoint);
void __cdecl Gte_BuildLookBasisFromDirection(short *direction, short *basisOut);

/* battle_vector_angle_math.cpp */
extern unsigned char *g_BattleFxDrawListBufferTaskSlotPtr;
extern int g_BattleFxActiveDrawListPtr;
short __cdecl Gte_PlaneHeightDelta(struct Vec3sPacked *pointA, struct Vec3sPacked *pointB, struct BattlePlaneNormal *normal);
void __cdecl BattleFx_SelectDrawListBuffer(void);

/* battle_bit_mask_helpers.cpp */
void __cdecl Battle_CopySceneDataBlock(void *dst, void *src, int byteCount);
int __cdecl Battle_HighestSetBitIndex(int value);
int __cdecl Battle_PopCountMask(unsigned int mask);
int __cdecl Battle_ClampHpMpDelta(int value, int delta, int maxValue, int isAdd);

/* battle_effect_trail_ribbon.cpp */
extern char g_BattleCurrFormationCameraIdx;
extern short g_BattleEffectTrail_StateShort;
extern char g_BattleStageHideFlags;
extern char g_BattleSequenceActorIndex;
extern char g_BattleSpecialActorId;
extern char g_BattleHitReactionCount;
extern char g_BattleSlot3ActorState;
extern char g_BattleEscapedEndState;
extern signed char g_BattleSceneState;
extern short g_BattleEffectTrail_ShortVector[3];
extern char g_BattleHitDisplayCount;
extern short g_BattleTimelineTargetMask;
extern short g_BattleEffectTargetMask;
extern char g_Battle3dPendingActorEffect;
extern short g_BattleAuxTaskActiveCount;
extern char g_BattleHitReactionActor;
extern unsigned char g_BattleLayout;
extern char g_Battle3dFrameTick;
extern short g_BattleOverlayTaskActiveCount;
extern int g_BattleEffectEnterResult;
extern char g_BattleScriptWaitFrames;
extern char g_BattleEnterFramesToWait;
extern unsigned char g_BattleHitTargetActors[78];
extern short g_BattleCameraTargetIndex;
extern short g_BattleSecondaryTaskActiveCount;
extern short g_BattleOverlayTaskSlotIdx;
extern char g_BattleOpeningUiMode;
extern short g_BattleHurtMessageActorMask;
extern short g_Battle3dReadyActorMask;
extern char g_BattleVariationIndex;
extern short g_BattleCameraScriptValue;
extern char g_BattleAnimStaggerCounter;
extern struct BattleDamageNumberPos g_BattleDamageNumberPos;
extern short g_BattleDamageNumberPosZ;
extern short g_BattleDamageNumberDigits[4];
extern short g_BattleOverlaySpritePos[3];
extern short g_BattleStatusIconPos[3];
extern short g_BattleStatusIconDigits[4];
extern short g_BattleOverlaySpriteCount;
int __cdecl Battle_DivideRgb888(int color, int divisor);
void __cdecl Battle_CreateEffectTrailTask(int actorIndex, int attachIndex, int yOffset, int colorRgba, int fadeDivisor, int lifetime);
void __cdecl Battle_EffectTrailTaskHandler(void);
void __cdecl Battle_EffectTrailSegmentTask(void);

/* battle_damage_formula_set.cpp */
void __cdecl Battle_DmgFormula00_NoDamage(void);
void __cdecl Battle_DmgFormula01_Physical(void);
void __cdecl Battle_DmgFormula02_Magical(void);
void __cdecl Battle_DmgFormula03_PercentOfCurrent(void);
void __cdecl Battle_DmgFormula04_PercentOfMax(void);
void __cdecl Battle_DmgFormula05_MagicalNoDefence(void);
void __cdecl Battle_DmgFormula06_PowerTimes20(void);
void __cdecl Battle_DmgFormula07_PowerVsDefence(void);
void __cdecl Battle_DmgFormula08_DeathOrFullCure(void);
void __cdecl Battle_DmgFormula09_WeaponPowerPhysical(void);
void __cdecl Battle_DmgFormula0A_PowerSplitByTargets(void);
void __cdecl Battle_DmgFormula10_AttackerHp(void);
void __cdecl Battle_DmgFormula11_AttackerHpMissing(void);
void __cdecl Battle_DmgFormula12_Stub(void);
void __cdecl Battle_DmgFormula13_Stub(void);
void __cdecl Battle_DmgFormula14_Stub(void);
void __cdecl Battle_DmgFormula15_Stub(void);
void __cdecl Battle_DmgFormula16_Stub(void);
void __cdecl Battle_DmgFormula17_Stub(void);
void __cdecl Battle_DmgFormula18_DiceRoll(void);
void __cdecl Battle_DmgFormula19_SavemapCounter(void);
void __cdecl Battle_DmgFormula1A_TargetHpMinusOne(void);
void __cdecl Battle_DmgFormula1B_PlayTime(void);
void __cdecl Battle_DmgFormula1C_KillCountTimes10(void);
void __cdecl Battle_DmgFormula1D_MateriaCountTimes1111(void);
int __cdecl Battle_ApplyBarrierAndDamageBonus(int damage);
int __cdecl Battle_ApplyMultiTargetSplit(int damage, int fullDamage);
int __cdecl Battle_ApplySadnessReduction(int damage);
int __cdecl Battle_ApplyDamageVariance(int damage);

/* battle_damage_hit_checks.cpp */
int __cdecl Battle_GetActorDefensePercent(int actorIndex);
void __cdecl Battle_ResolveAttackFacing(void);
void __cdecl Battle_CommitFacingFlip(void);
int __cdecl Battle_ApplyFuryAccuracyPenalty(int hitRate);

/* battle_particle_burst_effects.cpp */
extern struct BattleFxParticleQuadTransformMatrix g_BattleFxParticleQuadTransformMatrix;
extern struct BattleFxParticleAttachmentOffsetMatrix g_BattleFxParticleAttachmentOffsetMatrix;
extern unsigned char g_BattleFxParticleBurstJoTamRsdLeaf[11];
extern int g_BattleParticleDrawCursor;
extern struct GfxTableCursor *g_BattleParticleResource;
void __cdecl Battle_EnsureParticleResource(void);
void __cdecl Battle_SpawnParticleBurstDarkQuad(struct Int64Pair *pPos);
void __cdecl Battle_ParticleBurstQuadTask(void);
void __cdecl Battle_ParticleBurstTask(void);
void __cdecl Battle_ParticleBurstBouncingModelTask(void);
void __cdecl Battle_ParticleBurstAttachedModelTask(void);
void __cdecl Battle_SpawnBoneParticleBurstA(int burstType, int actorIdx, int delay, int duration);
void __cdecl Battle_SpawnBoneParticleBurstB(int burstType, int actorIdx, int delay, int duration);
void __cdecl Battle_SpawnBoneParticleBurstBothBones(int burstType, int actorIdx, int delay, int duration);

/* battle_actor_event_handlers.cpp */
void __cdecl Battle_EventResetActorAtb(int actor, int payload);
void __cdecl Battle_EventReleaseActorCommandMenu(int actor);
void __cdecl Battle_ResetLimitReadyState(int actor, int payload);
void __cdecl Battle_EventReopenActorCommandMenu(int actor);
void __cdecl Battle_EventRemoveActorFromTurnOrder(int actor, int payload);
void __cdecl Battle_EventAddPendingItemDrop(int actor, int itemId);
void __cdecl Battle_EventCancelActorTurn(int actor, int payload);
void __cdecl Battle_EventSetAtbFreezeFlag(int actor, int value);
void __cdecl Battle_EventEnterAutoControl(int actor);
void __cdecl Battle_EventLeaveAutoControl(int actor);
void __cdecl Battle_EventSetActorMenuSuppressed(int actor, int suppress);
void __cdecl Battle_ConsumeRandomInventoryItem(int actor);
short __cdecl Battle_ConsumeInventorySlot(int slot);
void __cdecl Battle_EventSetActorDirtyFlags(int actor, int dirtyMask);
void __cdecl Battle_EventRebuildActorAbilityLists(int actor, int payload);
void __cdecl Battle_EventClearEnemySlotMenuMasks(void);
void __cdecl Battle_ReopenPendingCommandMenus(void);
void __cdecl Battle_ConsumeInventoryItemById(int actor, int itemId);
void __cdecl Battle_EventQueue1NoOp0(void);
void __cdecl Battle_EventShowLimitBreakBanner(int actor);
void __cdecl Battle_EventQueueItemStolen(int actor, int itemId);
void __cdecl Battle_StatusTimerExpireGeneric(int actor, int timerSlot);
void __cdecl Battle_StatusTimerExpireDeathSentence(int actor);
void __cdecl Battle_StatusTimerExpireSlowNumb(int actor);
void __cdecl Battle_StatusTimerExpirePoison(int actor);
void __cdecl Battle_StatusTimerExpireNoOp(void);
void __cdecl Battle_EventQueue2NoOp16(void);
void __cdecl Battle_ClearLimitGauge(int actor);
void __cdecl Battle_EventQueueRebuildAbilityLists(int actor, int payload);
void __cdecl Battle_RemovePartyActorsFromBattle(int actor, int partyMask);
void __cdecl Battle_EventPostItemStolenMessage(int actor, int itemId);
void __cdecl Battle_EventQueueSetDirtyFlags(int actor, int payload);
void __cdecl Battle_SetActorTimingFlag40(int actor);
void __cdecl Battle_EventPushHitAnimStep(int actor, int attackIndex);
void __cdecl Battle_CopyPartyStatWords(int actor, int fromCombatant);
void __cdecl Battle_EventQueue2NoOp25(void);
void __cdecl Battle_ResetEventQueues(void);
void __cdecl Battle_PostEvent(int queueIndex, int actor, int opcode, int payload);
int __cdecl Battle_NextEventSlot(int slot);
void __cdecl Battle_FlushEventQueue(int queueIndex);

/* battle_message_expansion.cpp */
int __cdecl Battle_ExpandMessageArgs(char *outBuffer, char *messageText, unsigned short *messageArgs);
int __cdecl Battle_QueueAiMessageText(char *text, unsigned short *args);
char *__cdecl Battle_GetAiMessageText(int index);

/* battle_attack_record_lookup.cpp */
unsigned char *__cdecl Battle_FindAttackRecord(unsigned int attackId);

/* battle_status_effect_handlers.cpp */
extern const unsigned char g_BattleStatusTimerSlots[16];
void __cdecl Battle_BuildElementAffinityTable(int elementMask, int statusMask, int actor, struct BattleAffinityTable *outAffinityTable, int isCommandNotType4, int statusFlag1);
void __cdecl Battle_ApplyStatusToActorMask(int actorMask, int elementMask, int statusMask);
void __cdecl Battle_ResolveElementAffinity(void);
void __cdecl Battle_RefreshActorTickRates(int actor);
void __cdecl Battle_StartStatusTimer(int actor, int status, int set);
void __cdecl Battle_StopStatusTimer(int actor, int status, int set);
void __cdecl Battle_OnStatusSpeedChanged(int actor, int status, int set);
void __cdecl Battle_RemoveActorAndAwardSpoils(int actor, int status, int set);
void __cdecl Battle_OnStatusDeathCleared(int actor, int status, int set);
void __cdecl Battle_ResumeActorAfterStatus(int actor, int status, int set);
void __cdecl Battle_OnStatusAutoControlSet(int actor);
void __cdecl Battle_OnStatusAutoControlCleared(int actor, int status, int set);
void __cdecl Battle_OnStatusIncapacitateSet(int actor, int status, int set);
void __cdecl Battle_OnStatusIncapacitateCleared(int actor, int status, int set);
void __cdecl Battle_OnStatusCommandSetChanged(int actor);
void __cdecl Battle_RefreshSlipHpRate(int actor, int status, int set);
void __cdecl Battle_OnStatusReflectSet(int actor);
void __cdecl Battle_OnStatusCapturedChanged(int actor, int status, int set);
void __cdecl Battle_OnStatusManipulateChanged(int actor, int status, int set);
void __cdecl Battle_OnStatusReflectCleared(int actor);
int __cdecl Battle_DispatchStatusTransitions(void);
int __cdecl Battle_FindStatusTimerSlot(int status);
int __cdecl Battle_BuildStatusImmunityMask(int actor, int isCommandNotType4, int statusFlag1);

/* battle_fx_blue_flame_thrower_attach_effect.cpp */
extern char g_BattleBlueFlameThrowerInitializedFlag;
extern int g_BattleBlueFlameThrowerUnused;
extern int g_BattleBlueFlameThrowerTargetMask;
extern char g_BattleBlueFlameThrowerParticleActiveFlag;
extern int g_BattleBlueFlameThrowerActorIndex;
extern struct Vec3sPacked *g_pBattleBlueFlameThrowerScratchPos;
extern struct GfxTableCursor *g_BattleBlueFlameThrowerLoadedResTable;
void __cdecl BattleFx_BlueLoadResFlameThrowerAttach(void);
void __cdecl BattleFx_BlueEnterFlameThrowerAttach(int targetMask, int actorIdx);

/* battle_fx_blue_chocobuckle_effect.cpp */
void __cdecl BattleFx_BlueLoadResChocobuckle(void);
int __cdecl BattleFx_BlueEnterChocobuckle(int targetMask, int actorIdx);
int __cdecl BattleFx_BlueEnterChocobuckleAlt(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginChocobuckle(int targetMask, int originActor, int useAltSetup);
void __cdecl BattleFx_BlueChocobuckleSetupTask(void);
void __cdecl BattleFx_BlueChocobuckleTimelineTask(void);
void __cdecl BattleFx_BlueChocobuckleSpawnOrbitSpritesTask(void);
void __cdecl BattleFx_BlueChocobuckleOrbitSpriteTask(void);
void __cdecl BattleFx_BlueChocobuckleSpawnBouncingSpritesTask(void);
void __cdecl BattleFx_BlueChocobuckleBouncingSpriteTask(void);
void __cdecl BattleFx_BlueChocobuckleAltSetupTask(void);
void __cdecl BattleFx_BlueChocobuckleAltTimelineTask(void);
void __cdecl BattleFx_BlueChocobuckleSpawnScatterSpritesTask(void);
void __cdecl BattleFx_BlueChocobuckleScatterSpriteTask(void);
void __cdecl BattleFx_BlueChocobuckleSpawnBouncingBillboardSpritesTask(void);
void __cdecl BattleFx_BlueChocobuckleBouncingBillboardSpriteTask(void);
int __cdecl BattleFx_BlueChocobuckleCountStepsToTarget(int position, int velocity, int accel, int target);

/* battle_fx_blue_angel_whisper_effect.cpp */
extern int g_BattleBlueAngelWhisperResourceDef;
extern int g_BattleBlueAngelWhisperStepCount;
extern int g_BattleBlueAngelWhisperVariantMode;
extern int g_BattleBlueAngelWhisperTargetMask;
extern int g_BattleBlueAngelWhisperOriginActor;
extern struct GfxTableCursor *g_BattleBlueAngelWhisperLoadedRes;
void __cdecl BattleFx_BlueLoadResAngelWhisper(void);
int __cdecl BattleFx_BlueEnterAngelWhisperB(int targetMask, int actorIdx);
int __cdecl BattleFx_BlueEnterAngelWhisperC(int targetMask, int actorIdx);
int __cdecl BattleFx_BlueEnterAngelWhisper(int targetMask, int actorIdx);
int __cdecl BattleFx_BlueEnterAngelWhisperD(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginAngelWhisperQuadSprites(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueAngelWhisperQuadSpritesSetupTask(void);
void __cdecl BattleFx_BlueAngelWhisperQuadSpritesTimelineTask(void);
void __cdecl BattleFx_BlueAngelWhisperSpawnArcingQuadSpritesTask(void);
void __cdecl BattleFx_BlueAngelWhisperArcingQuadSpriteTask(void);
void __cdecl BattleFx_BlueAngelWhisperSpawnDescendingQuadSpritesTask(void);
void __cdecl BattleFx_BlueAngelWhisperDescendingQuadSpriteTask(void);
void __cdecl BattleFx_BlueBeginAngelWhisperActorFade(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueAngelWhisperActorFadeSetupTask(void);
void __cdecl BattleFx_BlueAngelWhisperActorFadeTask(void);
void __cdecl BattleFx_BlueAngelWhisperActorFadeTimelineTask(void);
void __cdecl BattleFx_BlueAngelWhisperRingRecordTimelineTask(void);
void __cdecl BattleFx_BlueAngelWhisperRisingFadeSpriteTask(void);
void __cdecl BattleFx_BlueAngelWhisperExpandingFadeModelTask(void);
void __cdecl BattleFx_BlueAngelWhisperActorFadeVariantTimelineTask(void);

/* battle_fx_blue_roulette_effect.cpp */
extern int g_BattleBlueRouletteResourceDef;
extern int g_BattleBlueRouletteStopDelay;
extern int g_BattleBlueRouletteSpinTimer;
extern int g_BattleBlueRouletteSelectedActor;
extern int g_BattleBlueRouletteTargetIndex;
extern int g_BattleBlueRouletteTargetCount;
extern int g_BattleBlueRouletteStepIndex;
extern int g_BattleBlueRouletteVariantMode;
extern int g_BattleBlueRouletteCursorIndex;
extern int g_BattleBlueRouletteTargetMask;
extern int g_BattleBlueRouletteSpinState;
extern struct GfxTableCursor *g_BattleBlueRouletteLoadedRes;
void __cdecl BattleFx_BlueLoadResRoulette(void);
int __cdecl BattleFx_BlueEnterRoulette(int targetMask, int actorIdx);
int __cdecl BattleFx_BlueEnterRouletteAlt(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginRoulette(int targetMask, int casterIdx);
void __cdecl BattleFx_BlueRouletteBeginTask(void);
void __cdecl BattleFx_BlueRouletteTimelineTask(void);
void __cdecl BattleFx_BlueRouletteRunSelectionTask(void);

/* battle_fx_blue_pandoras_box_effect.cpp */
extern int g_BattleBluePandorasBoxResourceDef;
extern struct BattleBluePandorasBoxFragmentRecords g_BattleBluePandorasBoxFragmentRecords;
void __cdecl BattleFx_BlueLoadResPandorasBox(void);
void __cdecl BattleFx_BlueEnterPandorasBox(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginPandorasBox(int targetMask, int actorIdx);
void __cdecl BattleFx_BluePandorasBoxSetupTask(void);
void __cdecl BattleFx_BluePandorasBoxInitShatterFragments(void);
void __cdecl BattleFx_BluePandorasBoxTimelineTask(void);
void __cdecl BattleFx_BluePandorasBoxSpinDriftModelTask(void);
void __cdecl BattleFx_BluePandorasBoxTumblingModelTask(void);
void __cdecl BattleFx_BluePandorasBoxScaledSpinModelTask(void);
void __cdecl BattleFx_BluePandorasBoxQueueOffscreenRequests(int count);
void __cdecl BattleFx_BluePandorasBoxShatterFragmentsDrawTask(void);
void __cdecl BattleFx_BluePandorasBoxGrowShrinkModelTask(void);
void __cdecl BattleFx_BluePandorasBoxParticleFieldTask(void);
void __cdecl BattleFx_BluePandorasBoxClearRecordPool(void);

/* battle_fx_blue_big_guard_effect.cpp */
extern int g_BattleBlueBigGuardResourceDef;
extern struct GfxTableCursor *g_BattleBlueBigGuardLoadedRes;
void __cdecl BattleFx_BlueLoadResBigGuard(void);
void __cdecl BattleFx_BlueEnterBigGuard(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginBigGuard(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueSpawnBigGuardPerTarget(int actor);
void __cdecl BattleFx_BlueBigGuardTimelineTask(void);
void __cdecl BattleFx_BlueBigGuardGrowingFadeModelTask(void);
void __cdecl BattleFx_BlueBigGuardSpinGrowModelTask(void);
void __cdecl BattleFx_BlueBigGuardAscendingQuadSpriteTask(void);

/* battle_fx_blue_l4_suicide_effect.cpp */
extern int g_BattleBlueL4SuicideResourceDef;
extern int g_BattleBlueL4SuicideRefAxis[3];
extern int g_BattleBlueL4SuicideTargetCount;
extern unsigned char g_BattleBlueL4SuicideRingRecords[984];
extern struct GfxTableCursor *g_BattleBlueL4SuicideLoadedRes;
void __cdecl BattleFx_BlueLoadResL4Suicide(void);
void __cdecl BattleFx_BlueEnterL4Suicide(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginL4Suicide(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueL4SuicideTimelineTask(void);
void __cdecl BattleFx_BlueL4SuicideAnimatedSpriteTask(void);
void __cdecl BattleFx_BlueL4SuicideDrawPsxGouraudTriangles(struct BattleFxJibakuShard *shard, struct BattleFxBlueJibakuRing *ringRecord);
void __cdecl BattleFx_BlueL4SuicideDrawRingSegments(struct BattleFxJibakuShard *ringState, struct BattleFxBlueJibakuRing *ringRecord, int color);
void __cdecl BattleFx_BlueL4SuicideDrawPsxFlatRingQuads(struct BattleFxJibakuShard *ringState, void *context, int color);
void __cdecl BattleFx_BlueL4SuicideProjectRingPoints(struct BattleFxJibakuShard *ring, struct BattleFxJibakuRingSource *source, int size);

/* battle_fx_blue_matra_magic_effect.cpp */
extern int g_BattleBlueMatraMagicResourceDef;
extern int g_BattleBlueMatraMagicDrawBufferSlot;
extern int g_BattleBlueMatraMagicDrawListBuffer1;
extern int g_BattleBlueMatraMagicRibbonCount;
extern short g_BattleBlueMatraMagicRibbonAngle;
extern short g_BattleBlueMatraMagicRibbonPitch;
extern short g_BattleBlueMatraMagicRibbonRoll;
extern int g_BattleBlueMatraMagicTargetMask;
extern int g_BattleBlueMatraMagicStateFlag;
void __cdecl BattleFx_BlueLoadResMatraMagic(void);
void __cdecl BattleFx_BlueEnterMatraMagic(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueMatraMagicSelectDrawListBuffer(void);
void __cdecl BattleFx_BlueMatraMagicSetupTask(void);
void __cdecl BattleFx_BlueMatraMagicSpawnRibbonsTask(void);
void __cdecl BattleFx_BlueMatraMagicRibbonTask(void);
void __cdecl BattleFx_BlueMatraMagicAnimatedSpriteTask(void);
void __cdecl BattleFx_BlueMatraMagicDrawPsxGouraudQuads(struct BattleFxBlueMatraMagicSegmentWork *ribbon, int step, int count, int useCache);
void __cdecl BattleFx_BlueMatraMagicProjectRibbonSegmentQuad(struct BattleFxBlueMatraMagicSegmentWork *segment, int halfSize, int advanceFlag);
void __cdecl BattleFx_BlueMatraMagicAdvanceDrawModel(struct BattleFxBlueMatraMagicSegmentWork *missile);

/* battle_fx_blue_laser_effect.cpp */
extern int g_BattleBlueLaserResourceDef;
extern int g_BattleBlueLaserTargetMask;
extern struct GfxTableCursor *g_BattleBlueLaserLoadedRes;
void __cdecl BattleFx_BlueLoadResLaser(void);
void __cdecl BattleFx_BlueEnterLaser(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueLaserSetupTask(void);
void __cdecl BattleFx_BlueLaserSpawnRibbonsTask(void);
void __cdecl BattleFx_BlueLaserRibbonTask(void);
void __cdecl BattleFx_BlueLaserAnimatedSpriteTask(void);
void __cdecl BattleFx_BlueLaserDrawPsxGouraudQuads(struct BattleFxBlueLaserSegmentWork *ribbon, int step, int count, int useCache);
void __cdecl BattleFx_BlueLaserProjectRibbonSegmentQuad(struct BattleFxBlueLaserSegmentWork *ribbon, int halfSize, int advanceFlag);
void __cdecl BattleFx_LaserSelectDrawListBuffer(void);

/* battle_fx_blue_l5_death_effect.cpp */
extern int g_BattleBlueL5DeathActiveCount;
extern struct GfxTableCursor *g_BattleBlueL5DeathLoadedResTable;
void __cdecl BattleFx_BlueLoadResL5Death(void);
void __cdecl BattleFx_BlueEnterL5Death(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginL5Death(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueL5DeathScreenIntensityTask(void);
void __cdecl BattleFx_BlueSpawnL5DeathPerTarget(int actor);
void __cdecl BattleFx_BlueL5DeathRampRotationModelTask(void);
void __cdecl BattleFx_BlueL5DeathFadingSpriteTask(void);

/* battle_fx_blue_death_force_effect.cpp */
extern int g_BattleBlueDeathForceTargetIndex;
extern struct GfxTableCursor *g_BattleBlueDeathForceModelTable;
void __cdecl BattleFx_BlueLoadResDeathForce(void);
void __cdecl BattleFx_BlueEnterDeathForce(int targetMask, int userData);
void __cdecl BattleFx_BlueBeginDeathForce(int targetMask);
void __cdecl BattleFx_BlueDeathForceSetupTask(void);
void __cdecl BattleFx_BlueDeathForceTimelineTask(void);
void __cdecl BattleFx_BlueDeathForceSpinModelSparksTask(void);
void __cdecl BattleFx_BlueDeathForceAnimatedSpriteTask(void);

/* battle_fx_blue_frog_song_effect.cpp */
extern int g_BattleBlueFrogSongResourceDefs;
extern struct BattleBlueFrogSongState g_BattleBlueFrogSongState;
extern struct PsxMatrix32 g_BattleBlueFrogSongAimVector;
extern struct GfxTableCursor *g_BattleBlueFrogSongLoadedResTable;
void __cdecl BattleFx_BlueLoadResFrogSong(void);
void __cdecl BattleFx_BlueEnterFrogSong(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginFrogSong(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueFrogSongSpawnTask(void);
void __cdecl BattleFx_BlueFrogSongNoteEmitTask(void);
void __cdecl BattleFx_BlueFrogSongDrawNotesTask(void);
void __cdecl BattleFx_BlueFrogSongAimTask(void);
void __cdecl BattleFx_BlueFrogSongRingDrawTask(void);

/* battle_fx_blue_dragon_force_effect.cpp */
extern int g_BattleBlueDragonForceResourceDefs;
extern int g_BattleBlueDragonForceTargetIndex;
extern struct GfxTableCursor *g_BattleBlueDragonForceLoadedResTable;
void __cdecl BattleFx_BlueLoadResDragonForce(void);
void __cdecl BattleFx_BlueEnterDragonForce(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginDragonForce(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueDragonForceSetupTask(void);
void __cdecl BattleFx_BlueDragonForceSpinScaledModelTask(void);
void __cdecl BattleFx_BlueDragonForceTimelineTask(void);
void __cdecl BattleFx_BlueDragonForceShrinkingModelTask(void);
void __cdecl BattleFx_BlueDragonForceGrowingModelTask(void);

/* battle_fx_blue_magic_hammer_effect.cpp */
extern struct GfxTableCursor *g_BattleBlueMagicHammerLoadedResTable;
void __cdecl BattleFx_BlueLoadResMagicHammer(void);
void __cdecl BattleFx_BlueEnterMagicHammer(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginMagicHammer(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueSpawnMagicHammerPerTarget(int actor);
void __cdecl BattleFx_BlueMagicHammerSwingModelTask(void);
void __cdecl BattleFx_BlueMagicHammerSpawnDriftingSpritesTask(void);
void __cdecl BattleFx_BlueMagicHammerDriftingAnimatedSpriteTask(void);

/* battle_fx_blue_bad_breath_effect.cpp */
extern int g_BattleBlueBadBreathResourceDefs;
extern int g_BattleBlueBadBreathTargetMask;
extern struct GfxTableCursor *g_BattleBlueBadBreathLoadedResTable;
void __cdecl BattleFx_BlueLoadResBadBreath(void);
void __cdecl BattleFx_BlueEnterBadBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginBadBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBadBreathSetupTask(void);
void __cdecl BattleFx_BlueBadBreathTimelineTask(void);
void __cdecl BattleFx_BlueBadBreathAnimatedSpritesTask(void);

/* battle_fx_blue_question_marks_effect.cpp */
extern int g_BattleBlueQuestionMarksTargetIndex;
extern struct GfxTableCursor *g_BattleBlueQuestionMarksLoadedResTable;
extern int g_BattleBlueQuestionMarksResourceDefs;
extern unsigned char g_BattleBlueQuestionMarksFileNames[];
void __cdecl BattleFx_BlueLoadResQuestionMarks(void);
void __cdecl BattleFx_BlueEnterQuestionMarks(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginQuestionMarks(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueQuestionMarksSetupTask(void);
void __cdecl BattleFx_BlueQuestionMarksTimelineTask(void);
int __cdecl BattleFx_BlueQuestionMarksDrawPsxMesh(struct BattleFxBlueQuestionMarksDraw *draw);
int __cdecl BattleFx_BlueQuestionMarksDrawPsxTexturedTriangles(struct BattleFxBlueQuestionMarksDraw *draw, int vertexBase, int primBlock);
int __cdecl BattleFx_BlueQuestionMarksDrawPsxTexturedQuads(struct BattleFxBlueQuestionMarksDraw *draw, int vertexBase, int primBlock);
int __cdecl BattleFx_BlueQuestionMarksDrawPsxGouraudTriangles(struct BattleFxBlueQuestionMarksDraw *draw, int vertexBase, int primBlock);
int __cdecl BattleFx_BlueQuestionMarksDrawPsxGouraudQuads(struct BattleFxBlueQuestionMarksDraw *draw, int vertexBase, int primBlock);
void __cdecl BattleFx_BlueQuestionMarksSpawnDriftingSpritesTask(void);
void __cdecl BattleFx_BlueQuestionMarksDriftingAnimatedSpriteTask(void);

/* battle_fx_blue_goblin_punch_effect.cpp */
extern int g_BattleBlueGoblinPunchResourceDefs;
extern int g_BattleBlueGoblinPunchTargetIndex;
extern int g_BattleBlueGoblinPunchStreakCount;
extern unsigned char g_BattleBlueGoblinPunchStreakVertices[5326];
extern struct GfxTableCursor *g_BattleBlueGoblinPunchLoadedResTable;
void __cdecl BattleFx_BlueLoadResGoblinPunch(void);
void __cdecl BattleFx_BlueEnterGoblinPunch(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueGoblinPunchSetupTask(void);
void __cdecl BattleFx_BlueGoblinPunchTimelineTask(void);
void __cdecl BattleFx_BlueGoblinPunchFlashSpriteTask(void);
void __cdecl BattleFx_BlueGoblinPunchStreakLinesTask(void);
void __cdecl BattleFx_BlueGoblinPunchAnimatedSpriteTask(void);

/* battle_fx_blue_aqualung_effect.cpp */
extern int g_BattleBlueAqualungResourceDefs;
extern unsigned char g_BattleBlueAqualungResourceNames[];
extern int g_BattleBlueAqualungTargetMask;
extern struct GfxTableCursor *g_BattleBlueAqualungLoadedResTable;
void __cdecl BattleFx_BlueLoadResAqualung(void);
void __cdecl BattleFx_BlueEnterAqualung(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginAqualung(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueAqualungSetupTask(void);
void __cdecl BattleFx_BlueAqualungTimelineTask(void);
void __cdecl BattleFx_BlueAqualungGrowShrinkSpritesTask(void);

/* battle_fx_blue_magic_breath_effect.cpp */
extern int g_BattleBlueMagicBreathTargetMask;
extern int g_BattleBlueMagicBreathCasterIndex;
extern int g_BattleBlueMagicBreathSourcePos;
extern int g_BattleBlueMagicBreathSourcePosZ;
extern struct GfxTableCursor *g_BattleBlueMagicBreathLoadedResTable;
void __cdecl BattleFx_BlueLoadResMagicBreath(void);
void __cdecl BattleFx_BlueEnterMagicBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginMagicBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueMagicBreathSetupTask(void);
void __cdecl BattleFx_BlueMagicBreathTimelineTask(void);
void __cdecl BattleFx_BlueMagicBreathFadingSpriteTask(void);

/* battle_fx_blue_trine_effect.cpp */
extern int g_BattleBlueTrineResourceDefs;
extern struct BattleFxModelDrawHead g_BattleBlueTrineModelReq;
extern int g_BattleBlueTrineTargetMask;
extern struct GfxTableCursor *g_BattleBlueTrineLoadedResTable;
void __cdecl BattleFx_BlueLoadResTrine(void);
void __cdecl BattleFx_BlueEnterTrine(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginTrine(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueTrineSetupTask(void);
void __cdecl BattleFx_BlueTrineSpinGrowFlashModelTask(void);
void __cdecl BattleFx_BlueTrineSpinGrowFadeModelTask(void);

/* battle_fx_blue_shadow_flare_effect.cpp */
extern int g_BattleBlueShadowFlareResourceDefs;
extern struct BattleBlueShadowFlareSpriteReq1 g_BattleBlueShadowFlareSpriteReq1;
extern struct BattleBlueShadowFlareSpriteReq2 g_BattleBlueShadowFlareSpriteReq2;
extern struct BattleBlueShadowFlareSpriteReq3 g_BattleBlueShadowFlareSpriteReq3;
void __cdecl BattleFx_BlueLoadResShadowFlare(void);
void __cdecl BattleFx_BlueEnterShadowFlare(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginShadowFlare(int targetMask, int sourceActorIdx);
void __cdecl BattleFx_BlueShadowFlareActorFadeTask(void);
void __cdecl BattleFx_BlueShadowFlareSetupTask(void);
void __cdecl BattleFx_BlueShadowFlareDelayedSpawnTask(void);
void __cdecl BattleFx_BlueShadowFlareGrowFadeAnimatedSpriteTask(void);
void __cdecl BattleFx_BlueShadowFlareGrowFadeQuadTask(void);
void __cdecl BattleFx_BlueShadowFlareTimelineTask(void);
void __cdecl BattleFx_BlueShadowFlareFrameTableSpriteTask(void);
void __cdecl BattleFx_BlueShadowFlareMovingLivewireSpriteTask(void);

/* battle_fx_blue_beta_effect.cpp */
extern int g_BattleBlueBetaResourceDefs;
extern short g_BattleBlueBetaScreenBrightness;
extern int g_BattleBlueBetaTargetMask;
extern struct BattleBlueBetaParticle g_BattleBlueBetaParticles[120];
extern struct GfxTableCursor *g_BattleBlueBetaModelTable;
void __cdecl BattleFx_BlueLoadResBeta(void);
void __cdecl BattleFx_BlueEnterBeta(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBeginBeta(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueBetaSetupTask(void);
void __cdecl BattleFx_BlueBetaInitParticles(void);
void __cdecl BattleFx_BlueBetaTimelineTask(void);
void __cdecl BattleFx_BlueBetaSpinStretchFadeModelTask(void);
void __cdecl BattleFx_BlueBetaSpinStretchFadeModelEntry1Task(void);
void __cdecl BattleFx_BlueBetaParticleFieldTask(void);
int __cdecl BattleFx_BlueBetaDrawParticleField(struct BattleFxBlueBetaParticleField *field);

/* battle_fx_blue_flame_thrower_effect.cpp */
extern int g_BattleBlueFlameThrowerResourceDefs;
extern unsigned char g_BattleBlueFlameThrowerRotTransPoint1[8];
extern unsigned char g_BattleBlueFlameThrowerRotTransPoint2[8];
extern struct BattleFxTimelineState *g_BattleBlueFlameThrowerDataBlock0;
extern char g_BattleBlueFlameThrowerFlag1;
extern struct BattleFxTimelineState *g_pBattleSpecialTwinBurnerScratch;
extern int g_BattleBlueFlameThrowerPacketCursor;
extern int g_BattleBlueFlameThrowerTargetActorCentroid;
extern char g_BattleBlueFlameThrowerFlag2;
extern char g_BattleBlueFlameThrowerFlag3;
extern struct Vec3sPacked *g_BattleBlueFlameThrowerDataBlock1;
extern int g_BattleBlueFlameThrowerVariantFlag;
extern struct GfxTableCursor *g_BattleBlueFlameThrowerLoadedResTable58bd1;
void __cdecl BattleFx_BlueLoadResFlameThrower(void);
void __cdecl BattleFx_BlueEnterFlameThrower(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterFlameThrowerVar1(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterFlameThrowerLeft(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterFlameThrowerFang(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterFlameThrowerFangNoSound(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterFlameThrowerVar2(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterTwinBurner(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueRegisterFlameThrowerPerTargetCallback(int targetMask, int casterIndex);
void __cdecl BattleFx_BlueFlameThrowerTimelineTask(void);
void __cdecl BattleFx_BlueFlameThrowerParticleArrayTask(void);
void __cdecl BattleFx_BlueFlameThrowerClearParticleRecords(void);
void __cdecl BattleFx_BlueRegisterFlameThrowerLeftPerTargetCallback(int targetMask, int casterIndex);
void __cdecl BattleFx_BlueFlameThrowerFangTimelineTask(void);
void __cdecl BattleFx_SpecialTwinBurnerTimelineTask(void);

/* battle_fx_blue_white_wind_effect.cpp */
extern int g_BattleBlueWhiteWind_ResourceDef;
extern int g_BattleBlueWhiteWind_LoadedRes;
extern int g_BattleBlueWhiteWind_TargetMask;
extern int g_BattleBlueWhiteWind_ParticleCount;
extern struct GfxTableCursor *g_BattleBlueWhiteWind_EffectState;
void __cdecl BattleFx_BlueLoadResWhiteWind(void);
void __cdecl BattleFx_BlueEnterWhiteWind(int targetMask, int casterIdx);
void __cdecl BattleFx_BlueWhiteWindSpawnRadialSpritesTask(void);
void __cdecl BattleFx_BlueWhiteWindFadingSpriteTask(void);

/* battle_fx_builtin_hit_sparks.cpp */
void __cdecl BattleFx_BuiltinFlashSnd06(unsigned int targetMask, int casterIndex);
void __cdecl BattleFx_SpawnHitSparkEmitter(short actor);
void __cdecl BattleFx_HitSparkEmitterTask(void);
void __cdecl BattleFx_HitSparkParticleTask(void);
void __cdecl BattleFx_BuiltinFlashSnd08(unsigned int targetMask, int casterIndex);
void BattleFx_SpawnHitSparkEmitterSnd08(short actor);
void __cdecl BattleFx_BuiltinFlashSnd45(unsigned int targetMask, int casterIndex);
void BattleFx_SpawnHitSparkEmitterRandomRow(short actor);
void __cdecl BattleFx_HitSparkEmitterTaskRandomRow(void);
void __cdecl BattleFx_BuiltinFlashSnd5C(unsigned int targetMask, int casterIndex);
void BattleFx_SpawnHitSparkEmitterShort(short actor);
void __cdecl BattleFx_HitSparkEmitterTaskShort(void);
void __cdecl BattleFx_BuiltinFlashSnd5B(unsigned int targetMask, int casterIndex);
void BattleFx_SpawnHitSparkEmitterSnd5B(short actor);
void __cdecl BattleFx_BuiltinFlashSnd1Fa(unsigned int targetMask, int actingActor);
void BattleFx_SpawnHitSparkEmitterSnd1Fa(short actor);
void __cdecl BattleFx_BuiltinFlashSnd1Fb(unsigned int targetMask, int actingActor);
void BattleFx_SpawnHitSparkEmitterSnd1Fb(short actor);
void __cdecl BattleFx_BuiltinFlashSnd17E(unsigned int targetMask, int actingActor);
void BattleFx_SpawnHitSparkEmitterSnd17E(short actor);
void BattleFx_BuiltinFlashSnd166(unsigned int targetMask, int actingActor);
void BattleFx_SpawnHitSparkEmitterSnd166(short actor);
void __cdecl BattleFx_BuiltinFlashSnd07(unsigned int targetMask, int actingActor);
void BattleFx_SpawnHitSparkEmitterSnd07(short actor);

/* battle_fx_hitmark_effect.cpp */
extern unsigned int g_BattleFxHitMarkSpriteRecordTable[40];
extern int g_BattleFxHitmarkObjectFieldIndexByKind[5];
extern int g_BattleFxHitmarkType8ObjectFieldIndex;
extern unsigned char g_BattleFxHitmarkNestedScriptIndex0[7];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex1[13];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex2[19];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex3[7];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex4[7];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex5[25];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex6[19];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex7[19];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex8[7];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex9[13];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex10[19];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex11[13];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex12[19];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex13[7];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex14[13];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex15[19];
extern unsigned char g_BattleFxHitmarkNestedScriptIndex16[19];
extern signed char * g_BattleFxHitmarkNestedScriptTable[17];
extern int g_BattleFxHitmarkScriptIndex0;
extern unsigned char g_BattleFxHitmarkScriptIndex1[7];
extern unsigned char g_BattleFxHitmarkScriptIndex2[7];
extern int g_BattleFxHitmarkScriptIndex3;
extern unsigned char g_BattleFxHitmarkScriptIndex4[7];
extern unsigned char g_BattleFxHitmarkScriptIndex5[7];
extern int g_BattleFxHitmarkScriptIndex6;
extern unsigned char g_BattleFxHitmarkScriptIndex7[7];
extern unsigned char g_BattleFxHitmarkScriptIndex8[7];
extern int g_BattleFxHitmarkScriptIndex9;
extern unsigned char g_BattleFxHitmarkScriptIndex10[7];
extern unsigned char g_BattleFxHitmarkScriptIndex11[7];
extern int g_BattleFxHitmarkScriptIndex12;
extern unsigned char g_BattleFxHitmarkScriptIndex13[7];
extern unsigned char g_BattleFxHitmarkScriptIndex14[7];
extern int g_BattleFxHitmarkScriptIndex15;
extern unsigned char g_BattleFxHitmarkScriptIndex16[7];
extern unsigned char g_BattleFxHitmarkScriptIndex17[7];
extern int g_BattleFxHitmarkScriptIndex18;
extern unsigned char g_BattleFxHitmarkScriptIndex19[7];
extern unsigned char g_BattleFxHitmarkScriptIndex20[7];
extern int g_BattleFxHitmarkScriptIndex21;
extern unsigned char g_BattleFxHitmarkScriptIndex22[7];
extern unsigned char g_BattleFxHitmarkScriptIndex23[7];
extern int g_BattleFxHitmarkScriptIndex24;
extern unsigned char g_BattleFxHitmarkScriptIndex25[7];
extern unsigned char g_BattleFxHitmarkScriptIndex26[7];
extern int g_BattleFxHitmarkScriptIndex27;
extern unsigned char g_BattleFxHitmarkScriptIndex28[7];
extern unsigned char g_BattleFxHitmarkScriptIndex29[7];
extern int g_BattleFxHitmarkScriptIndex30;
extern unsigned char g_BattleFxHitmarkScriptIndex31[7];
extern unsigned char g_BattleFxHitmarkScriptIndex32[7];
extern int g_BattleFxHitmarkScriptIndex33;
extern unsigned char g_BattleFxHitmarkScriptIndex34[7];
extern unsigned char g_BattleFxHitmarkScriptIndex35[7];
extern unsigned char g_BattleFxHitmarkScriptIndex36[7];
extern unsigned char g_BattleFxHitmarkScriptIndex37[7];
extern unsigned char g_BattleFxHitmarkScriptIndex38[7];
extern unsigned char g_BattleFxHitmarkScriptIndex39[25];
extern unsigned char g_BattleFxHitmarkScriptIndex40[19];
extern unsigned char g_BattleFxHitmarkScriptIndex41[19];
extern unsigned char g_BattleFxHitmarkScriptIndex42[7];
extern int g_BattleFxHitmarkScriptIndex43;
extern int g_BattleFxHitmarkScriptIndex44;
extern unsigned char g_BattleFxHitmarkScriptIndex45[25];
extern unsigned char g_BattleFxHitmarkScriptIndex46[19];
extern unsigned char g_BattleFxHitmarkScriptIndex47[19];
extern unsigned char g_BattleFxHitmarkScriptIndex48[7];
extern unsigned char g_BattleFxHitmarkScriptIndex49[7];
extern unsigned char g_BattleFxHitmarkScriptIndex50[7];
extern unsigned char g_BattleFxHitmarkScriptIndex51[25];
extern unsigned char g_BattleFxHitmarkScriptIndex52[19];
extern unsigned char g_BattleFxHitmarkScriptIndex53[19];
extern unsigned char g_BattleFxHitmarkScriptIndex54[7];
extern unsigned char g_BattleFxHitmarkScriptIndex55[7];
extern unsigned char g_BattleFxHitmarkScriptIndex56[7];
extern unsigned char g_BattleFxHitmarkScriptIndex57[25];
extern unsigned char g_BattleFxHitmarkScriptIndex58[19];
extern unsigned char g_BattleFxHitmarkScriptIndex59[19];
extern unsigned char g_BattleFxHitmarkScriptIndex60[7];
extern unsigned char g_BattleFxHitmarkScriptIndex61[7];
extern unsigned char g_BattleFxHitmarkScriptIndex62[7];
extern unsigned char g_BattleFxHitmarkScriptIndex63[25];
extern unsigned char g_BattleFxHitmarkScriptIndex64[19];
extern unsigned char g_BattleFxHitmarkScriptIndex65[19];
extern unsigned char g_BattleFxHitmarkScriptIndex66[7];
extern unsigned char g_BattleFxHitmarkScriptIndex67[7];
extern unsigned char g_BattleFxHitmarkScriptIndex68[7];
extern unsigned char g_BattleFxHitmarkScriptIndex69[25];
extern unsigned char g_BattleFxHitmarkScriptIndex70[19];
extern unsigned int g_BattleFxHitmarkScriptTable[71];
extern unsigned char g_BattleFxSpriteTransformMatrixM00[8];
extern unsigned char g_BattleFxSpriteTransformMatrixM11[8];
extern unsigned char g_BattleFxSpriteTransformMatrixM22[14];
extern unsigned char g_BattleFxHitmarkSpriteName_JO_C01[11];
extern struct GfxTableCursor *g_BattleFxHitmarkResource;
void __cdecl BattleFx_EffectLoadResHitMark(void);
void __cdecl BattleFx_HitMarkSpawnScript(void *pPosition, int scriptSelector, int arg3, int arg4);
void __cdecl BattleFx_HitMarkStepScriptSlot(void);
void __cdecl BattleFx_HitMarkSpawnSprite(char *pScriptPayload, void *pPosition, struct BattleFxHitMarkScriptSlot *pParentTask);
void __cdecl BattleFx_HitMarkSpriteTask(void);
void __cdecl BattleFx_HitMarkSpawnScriptAtActorBone(int actorIdx, int scriptSelector, int scale);
void __cdecl BattleFx_HitMarkSpawnAtActorBone(int actorIdx, int scriptSelector);
void __cdecl BattleFx_HitMarkSpawnAtPoint(void *pPosition, int scriptSelector, int zOffset);

/* battle_fx_sbaria_barrier.cpp */
extern struct GfxTableCursor *g_BattleFxSBariaResource;
void __cdecl BattleFx_EffectLoadResSBaria(void);
void __cdecl BattleFx_SBariaBeginActorVariant0(int actorIdx);
void __cdecl BattleFx_SBariaSpawnFadingActorModel(int actorIdx, int modelVariant);
void __cdecl BattleFx_SBariaFadingActorModelTask(void);
void __cdecl BattleFx_SBariaDrawModelFourPasses(int modelHandle, short fadeAlpha);
void __cdecl BattleFx_SBariaBeginActorVariant1(int actorIdx);
void __cdecl Battle_TriggerActorHurtSequence(int actorIdx);

/* battle_fx_actor_scale_grow.cpp */
int __cdecl BattleFx_EnterGrowActorScale(int targetMask, int actorIdx);
void __cdecl BattleFx_TaskHoldFrames(void);

/* battle_fx_builtin_ring_burst.cpp */
extern char g_BattleFxRingBurstColorTable[8];
extern int g_BattleFxRingBurstSpeed;
extern struct Int64Pair g_BattleFxRingBurstAngleVector;
void BattleFx_EnterBuiltinRingBurst(int targetMask, int actorIdx);
void __cdecl BattleFx_BuiltinBeginRingBurst(int targetMask, int actorIdx);
void __cdecl BattleFx_BuiltinSpawnRingBurstPerTarget(int actor);
void BattleFx_BuiltinRingBurstEmitterTask(void);
void BattleFx_BuiltinRingBurstParticleTask(void);

/* battle_fx_builtin_puff_mog_dance.cpp */
extern short g_BattleFxLimitMogDanceHitLivewireTexCoords[5];
extern int g_BattleFxPuffDrawPacketPtr;
extern int g_BattleFxLimitMogDanceTargetActorMask;
extern int g_BattleFxLimitMogDanceActingActorIdx;
void __cdecl BattleFx_EnterBuiltinPuff(unsigned int targetMask, int actingActor);
void __cdecl BattleFx_BuiltinBeginPuff(unsigned int targetMask, int actingActor);
void __cdecl BattleFx_BuiltinSpawnPuffPerTarget(int actor);
void __cdecl BattleFx_BuiltinPuffEmitterTask(void);
void __cdecl BattleFx_BuiltinPuffParticleTask(void);
void __cdecl BattleFx_LimitMogDanceBeginHitSparks(unsigned int targetMask, int actingActor);
void BattleFx_LimitMogDanceSpawnHitSparksPerTarget(short actor);
void BattleFx_LimitMogDanceHitSparksEmitterTask(void);
void __cdecl BattleFx_LimitMogDanceAnimatedSpriteTask(void);
void BattleFx_LimitBeginMogDance(int targetMask, int actorIdx);
int BattleFx_LimitEnterMogDance(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitMogDanceSpawnTimeline(void);
void __cdecl BattleFx_LimitMogDanceTimelineTask(void);

/* battle_fx_limit_landscaper.cpp */
extern struct BattleLimitLandscaperMeshSourceBlob1 g_BattleLimitLandscaperMeshSourceBlob1;
extern struct BattleLimitLandscaperMeshSourceBlob2 g_BattleLimitLandscaperMeshSourceBlob2;
extern struct GfxRotationAngles2 g_BattleLimitLandscaperRotation;
void __cdecl BattleFx_LimitLoadResLandscaper(void);
int __cdecl BattleFx_LimitEnterLandscaper(int targetMask, int actorIdx);

/* battle_fx_limit_fury_brand.cpp */
extern struct BattleLimitFuryBrandSpriteReqB g_BattleLimitFuryBrandSpriteReqB;
extern struct BattleLimitFuryBrandModelReqA g_BattleLimitFuryBrandModelReqA;
extern struct BattleLimitFuryBrandModelReqA g_BattleLimitFuryBrandModelReqB;
void __cdecl BattleFx_LimitLoadResFuryBrand(void);
int __cdecl BattleFx_LimitEnterFuryBrand(int targetMask, int actorIdx);

/* battle_fx_limit_dice.cpp */
extern int g_BattleLimitDiceResourceDef;
extern int g_BattleLimitDiceCollisionDirections[64];
extern int g_BattleLimitDiceCollisionRefVector[3];
extern unsigned char g_BattleLimitDiceModelDrawRequest[];
extern int g_BattleLimitDiceFirstTargetIdx;
extern struct BattleFxDicePosRecord g_BattleLimitDicePosRecords[6];
extern int g_BattleLimitDiceTargetMask;
extern int g_BattleLimitDiceActorIdx;
extern struct GfxTableCursor *g_BattleLimitDiceLoadedResTable;
void __cdecl BattleFx_LimitLoadResDice(void);
int __cdecl Battle_LimitFx42_Dice(int targetMask, int actorIdx);

/* battle_fx_limit_galian_beast.cpp */
extern unsigned char g_BattleLimitGalianBeastMeshBlob[6440];
extern unsigned int g_BattleLimitGalianBeastResourceDef[10];
extern int g_BattleLimitGalianBeastForwardVector[3];
extern int g_BattleLimitGalianBeastPoolCount;
extern int g_BattleLimitGalianBeastSoundPlayed;
extern int g_BattleLimitGalianBeastActorMask;
extern int g_BattleLimitGalianBeastActorIndex;
extern struct GfxTableCursor *g_BattleLimitGalianBeastResources;
void __cdecl BattleFx_LimitLoadResGalianBeast(void);
int __cdecl BattleFx_LimitEnterGalianBeast(int targetMask, int actorIdx);

/* battle_fx_limit_pulse_of_life.cpp */
void __cdecl BattleFx_LimitLoadResPulseOfLife(void);
int __cdecl Battle_LimitFx19_PulseOfLife(int targetMask, int actorIdx);

/* battle_fx_limit_mindblow.cpp */
void __cdecl BattleFx_LimitLoadResMindblow(void);
int __cdecl BattleFx_LimitEnterMindblow(int targetMask, int actorIdx);

/* battle_fx_limit_doom_of_the_living.cpp */
extern struct BattleLimitDoomOfTheLivingSpriteRequest1 g_BattleLimitDoomOfTheLivingSpriteRequest1;
extern struct BattleLimitDoomOfTheLivingSpriteRequest2 g_BattleLimitDoomOfTheLivingSpriteRequest2;
extern struct BattleLimitDoomOfTheLivingModelRequest g_BattleLimitDoomOfTheLivingModelRequest;
extern struct BattleLimitDoomOfTheLivingTimingTable1 g_BattleLimitDoomOfTheLivingTimingTable1;
extern unsigned char g_BattleLimitDoomOfTheLivingHitRsdName[8];
extern unsigned char g_BattleLimitDoomOfTheLivingHikariRsdName[11];
extern int g_BattleLimitDoomOfTheLivingTargetCentroid;
extern short g_BattleLimitDoomOfTheLivingModelRotAngles[3];
void __cdecl BattleFx_LimitLoadResDoomOfTheLiving(void);
int __cdecl BattleFx_LimitEnterDoomOfTheLiving(int targetMask, int actorIdx);

/* battle_fx_limit_gauntlet.cpp */
void __cdecl BattleFx_LimitLoadResGauntlet(void);
int __cdecl BattleFx_LimitEnterGauntlet(int targetMask, int actorIdx);

/* battle_fx_limit_bloodfest.cpp */
void __cdecl BattleFx_LimitLoadResBloodfest(void);
int __cdecl BattleFx_LimitEnterBloodfest(int targetMask, int actorIdx);

/* battle_fx_limit_greased_lightning.cpp */
void __cdecl BattleFx_LimitLoadResGreasedLightning(void);
int __cdecl BattleFx_LimitEnterGreasedLightning(int targetMask, int actorIdx);

/* battle_fx_limit_big_brawl.cpp */
void __cdecl BattleFx_LimitLoadResBigBrawl(void);
int __cdecl BattleFx_LimitEnterBigBrawl(int targetMask, int actorIdx);

/* battle_fx_limit_dragon_dive.cpp */
extern struct BattleLimitDragonDiveModelDrawRequest g_BattleLimitDragonDiveModelDrawRequest;
void __cdecl BattleFx_LimitLoadResDragonDive(void);
int __cdecl BattleFx_LimitEnterDragonDive(int targetMask, int actorIdx);

/* battle_fx_limit_hyper_jump.cpp */
extern unsigned char g_BattleLimitHyperJumpMeshTable0[912];
extern int g_BattleLimitHyperJumpMeshTable1;
extern unsigned char g_BattleLimitHyperJumpMeshTable2[912];
extern unsigned char g_BattleLimitHyperJumpMeshTable3[912];
extern int g_BattleLimitHyperJumpCentroid;
extern int g_BattleLimitHyperJumpZeroAnglesA;
extern int g_BattleLimitHyperJumpZeroAnglesB;
void __cdecl BattleFx_LimitLoadResHyperJump(void);
int __cdecl BattleFx_LimitEnterHyperJump(int targetMask, int actorIdx);

/* battle_fx_limit_dragon.cpp */
extern unsigned char g_BattleLimitDragonHeadModelName[9];
void __cdecl BattleFx_LimitLoadResDragon(void);
int __cdecl BattleFx_LimitEnterDragon(int targetMask, int actorIdx);

/* battle_fx_limit_boost_jump.cpp */
void __cdecl BattleFx_LimitLoadResBoostJump(void);
int __cdecl BattleFx_LimitEnterBoostJump(int targetMask, int actorIdx);

/* battle_fx_limit_all_creation.cpp */
extern struct BattleLimitAllCreationModelReq2 g_BattleLimitAllCreationModelReq1;
extern struct BattleLimitAllCreationRotAngles2 g_BattleLimitAllCreationRotAngles2;
void __cdecl BattleFx_LimitLoadResAllCreation(void);
int __cdecl BattleFx_LimitEnterAllCreation(int targetMask, int actorIdx);

/* battle_fx_limit_highwind.cpp */
extern int g_BattleLimitHighwindScaleVector[3];
void __cdecl BattleFx_LimitLoadResHighwind(void);
int __cdecl BattleFx_LimitEnterHighwind(int targetMask, int actorIdx);

/* battle_fx_limit_clear_tranquil.cpp */
extern int g_BattleLimitClearTranquilResourceDef;
extern short g_BattleLimitClearTranquilTargetMask;
extern struct GfxTableCursor *g_BattleLimitClearTranquilLoadedResTable;
void __cdecl BattleFx_LimitLoadResClearTranquil(void);
int __cdecl BattleFx_LimitEnterClearTranquil(int targetMask, int actorIdx);

/* battle_fx_limit_dynamite.cpp */
extern struct BattleLimitDynamiteParamTable g_BattleLimitDynamiteParamTable;
void __cdecl BattleFx_LimitLoadResDynamite(void);
int __cdecl BattleFx_LimitEnterDynamite(int targetMask, int actorIdx);

/* battle_fx_limit_finishing_touch.cpp */
void __cdecl BattleFx_LimitLoadResFinishingTouch(void);
int __cdecl BattleFx_LimitEnterFinishingTouch(int targetMask, int actorIdx);
int __cdecl BattleFx_LimitEnterFinishingTouchAlt(int targetMask, int actorIdx);

/* battle_fx_limit_meteorain.cpp */
extern int g_BattleLimitMeteorainMeshHeader;
extern struct BattleFxSpriteQuadRequest g_BattleLimitMeteorainParam60;
void __cdecl BattleFx_LimitLoadResMeteorain(void);
int __cdecl BattleFx_LimitEnterMeteorain(int targetMask, int actorIdx);

/* battle_fx_limit_lucky_girl.cpp */
extern int g_BattleLimitLuckyGirlResourceDef;
extern struct BattleLimitLuckyGirlTrail g_BattleLimitLuckyGirlTrailHistory[2];
extern int g_BattleLimitLuckyGirlTargetActorMask;
extern int g_BattleLimitLuckyGirlSourceActor;
extern struct GfxTableCursor *g_BattleLimitLuckyGirlLoadedResTable;
void __cdecl BattleFx_LimitLoadResLuckyGirl(void);
int __cdecl BattleFx_LimitEnterLuckyGirl(int targetMask, int actorIdx);

/* battle_fx_limit_cosmo_memory.cpp */
extern struct BattleLimitCosmoMemoryAnimatedSpriteRequest g_BattleLimitCosmoMemoryAnimatedSpriteRequest;
extern struct BattleLimitCosmoMemoryFadingSpriteRequest g_BattleLimitCosmoMemoryFadingSpriteRequest;
extern struct BattleLimitCosmoMemoryGrowingModelRequest g_BattleLimitCosmoMemoryGrowingModelRequest;
extern struct BattleLimitCosmoMemoryCentroidModelRequest g_BattleLimitCosmoMemoryCentroidModelRequest;
extern struct BattleLimitCosmoMemoryShrinkingModelRequest g_BattleLimitCosmoMemoryShrinkingModelRequest;
extern struct BattleLimitCosmoMemoryExpandingModelRequest g_BattleLimitCosmoMemoryExpandingModelRequest;
extern struct BattleLimitCosmoMemoryConvergingSpriteRequest g_BattleLimitCosmoMemoryConvergingSpriteRequest;
extern struct BattleLimitCosmoMemoryRingSpriteRequest g_BattleLimitCosmoMemoryRingSpriteRequest;
extern struct BattleLimitCosmoMemoryTriSpinModelRequest g_BattleLimitCosmoMemoryTriSpinModelRequest;
extern struct BattleLimitCosmoMemoryTriSpinSpriteRequest g_BattleLimitCosmoMemoryTriSpinSpriteRequest;
void __cdecl BattleFx_LimitLoadResCosmoMemory(void);
int __cdecl BattleFx_LimitEnterCosmoMemory(int targetMask, int actorIdx);

/* battle_fx_limit_earth_rave_effect.cpp */
extern int g_BattleLimitEarthRaveResourceDef;
extern int g_BattleLimitEarthRaveUpVector[3];
extern struct BattleFxSpriteRequest g_BattleLimitEarthRaveSpriteReq;
extern int g_BattleLimitEarthRaveTimingTable1[6];
extern int g_BattleLimitEarthRaveTimingTable2[6];
extern int g_BattleLimitEarthRaveTimingTable3[10];
extern unsigned char g_BattleLimitEarthRaveResPaths2[];
extern unsigned char g_BattleLimitEarthRaveResPaths4[];
extern int g_BattleLimitEarthRaveTargetMask;
extern short g_BattleLimitEarthRaveStepX;
extern short g_BattleLimitEarthRaveStepZ;
extern int g_BattleLimitEarthRaveActorIndex;
extern struct GfxTableCursor *g_BattleLimitEarthRaveModelTable;
void __cdecl BattleFx_LimitLoadResEarthRave(void);
int __cdecl BattleFx_LimitEnterEarthRave(int targetMask, int actorIdx);

/* battle_fx_limit_lunatic_high.cpp */
extern int g_BattleLimitLunaticHighFirstSelected;
extern int g_BattleLimitLunaticHighSelectMask;
extern int g_BattleLimitLunaticHighTargetMask;
extern struct GfxTableCursor *g_BattleLimitLunaticHighLoadedResTable;
void __cdecl BattleFx_LimitLoadResLunaticHigh(void);
int __cdecl BattleFx_LimitEnterLunaticHigh(int targetMask, int actorIdx);

/* battle_fx_limit_stardust_ray.cpp */
extern int g_BattleLimitStardustRayResourceDef;
extern int g_BattleLimitStardustRayRandomTable[256];
extern struct BattleFxSpriteRequest g_BattleLimitStardustRaySpriteReq2;
extern struct BattleFxSpriteRequest g_BattleLimitStardustRayParticleReq3;
extern struct BattleFxSpriteRequest g_BattleLimitStardustRayParticleReq4;
extern struct BattleFxSpriteRequest g_BattleLimitStardustRayParticleReq5;
extern int g_BattleLimitStardustRayRandomIndex;
extern struct BattleTaskSlot *g_BattleLimitStardustRayDrawListTask;
extern int g_BattleLimitStardustRayDrawBuffer1;
extern int g_BattleLimitStardustRayDrawBuffer2;
extern int g_BattleLimitStardustRayDrawListHead;
extern int g_BattleLimitStardustRayTargetActorMask;
extern struct GfxTableCursor *g_BattleLimitStardustRayModelTable;
void __cdecl BattleFx_LimitLoadResStardustRay(void);
int __cdecl BattleFx_LimitEnterStardustRay(int targetMask, int actorIdx);

/* battle_fx_limit_howling_moon.cpp */
extern unsigned int g_BattleLimitHowlingMoonModelRecords[20];
extern struct BattleFxSpriteRequest g_BattleLimitHowlingMoonSpriteRequest;
extern unsigned char g_BattleLimitHowlingMoonNegZVector[12];
extern unsigned char g_BattleLimitHowlingMoonResNameMoonTim[9];
extern int g_BattleLimitHowlingMoonTargetMask;
extern int g_BattleLimitHowlingMoonActorIndex;
extern struct PsxMatrix g_BattleLimitHowlingMoonRotationMatrix;
extern int g_BattleLimitHowlingMoonCameraDirX;
extern int g_BattleLimitHowlingMoonCameraDirY;
extern int g_BattleLimitHowlingMoonCameraDirZ;
extern struct GfxTableCursor *g_BattleLimitHowlingMoonLoadedResTable;
void __cdecl BattleFx_LimitLoadResHowlingMoon(void);
int __cdecl BattleFx_LimitEnterHowlingMoon(int targetMask, int actorIdx);

/* battle_fx_limit_toy_box.cpp */
extern struct BattleFxSpriteQuadRequest g_BattleLimitToyBoxQuadRequest;
extern struct BattleFxSpriteRequest g_BattleLimitToyBoxSpriteRequest;
extern unsigned char g_BattleLimitToyBoxResNameQuestionMarksTim[13];
extern unsigned char g_BattleLimitToyBoxResNameQuestionMarksSpt[12];
extern struct BattleFxModelDrawHead g_BattleLimitToyBoxModelMatrix;
extern int g_BattleLimitToyBoxTableField;
extern short g_BattleLimitToyBoxActorBoneCoord;
extern short g_BattleLimitToyBoxActorAngle;
extern int g_BattleLimitToyBoxTargetMask;
extern int g_BattleLimitToyBoxActorIndex;
extern struct GfxTableCursor *g_BattleLimitToyBoxLoadedResTable;
void __cdecl BattleFx_LimitLoadResToyBox(void);
int __cdecl BattleFx_LimitEnterToyBox(int targetMask, int actorIdx);
int __cdecl BattleFx_LimitEnterToyBoxB(int targetMask, int actorIdx);
int __cdecl BattleFx_LimitEnterToyBoxC(int targetMask, int actorIdx);
int __cdecl BattleFx_LimitEnterToyBoxD(int targetMask, int actorIdx);

/* battle_effect_program_ribbons.cpp */
extern unsigned char g_BattleLimitTifaPolygonBlob[14104];
extern unsigned char g_BattleLimitTifaAnimFrame0[2358];
extern unsigned char g_BattleLimitTifaAnimFrame1[2358];
extern unsigned char g_BattleLimitTifaAnimFrame2[2358];
extern unsigned char g_BattleLimitTifaAnimFrame3[2358];
extern unsigned char g_BattleLimitTifaAnimFrame4[2358];
extern unsigned char g_BattleLimitTifaAnimFrame5[2358];
extern unsigned char g_BattleLimitTifaAnimFrame6[2358];
extern unsigned char g_BattleLimitTifaAnimFrame7[2358];
extern unsigned char g_BattleLimitTifaAnimFrame8[2358];
extern unsigned char g_BattleLimitTifaAnimFrame9[2358];
extern unsigned char g_BattleLimitTifaAnimFrame10[2358];
extern unsigned char g_BattleLimitTifaAnimFrame11[2358];
extern int g_BattleLimitTifaNegativeOne;
extern unsigned char g_BattleLimitTifaResFilenameHit1[22];
extern unsigned char g_BattleLimitTifaResFilenameHit2[22];
extern unsigned char g_BattleLimitTifaResFilenameWater[24];
extern unsigned char g_BattleLimitTifaResFilename01Rsd[7];
extern unsigned char g_BattleLimitTifaResFilenameExp1[22];
extern unsigned char g_BattleLimitTifaResFilenameMete[23];
extern unsigned char g_BattleLimitTifaResFilenameSoni[24];
extern unsigned char g_BattleLimitTifaResFilenameWate2[22];
extern unsigned char g_BattleLimitTifaResFilenameExp2[23];
extern unsigned char g_BattleLimitTifaResFilenameRock[23];
extern struct Int64Pair g_BattleLimitTifaTargetPos2;
extern struct BattleFxRibbonParticle *g_pBattleLimitTifaParticlePool;
extern int g_BattleLimitTifaDrawListHandle2;
extern int g_BattleLimitTifaActiveRibbonHead;
extern int g_BattleLimitTifaRibbonNextSlot;
extern int g_BattleLimitTifaTargetLoopIndex;
extern int g_BattleLimitTifaParticleTracks;
extern int g_BattleLimitTifaTargetMask;
extern int g_BattleLimitTifaDrawListHandle1;
extern int g_BattleLimitTifaActorIdx;
extern int g_BattleLimitTifaSortedTargetIndices[10];
extern int g_BattleLimitTifaRibbonPointCount0;
extern int g_BattleLimitTifaRibbonPointCount2;
extern unsigned char g_BattleLimitTifaActorActiveStates[10];
extern unsigned char g_BattleLimitTifaTargetState;
extern struct BattleLimitTifaRelativeVec g_BattleLimitTifaRelativeVec;
extern int g_BattleLimitTifaRenderStatePtr;
extern int g_BattleLimitTifaRibbonPointCount1;
extern int g_BattleLimitTifaRibbonPointCount3;
extern int g_BattleLimitTifaRibbonPointCount4;
extern struct GfxTableCursor *g_BattleLimitTifaLoadedResTable;
void __cdecl BattleFx_LimitLoadResTifa1(void);
int __cdecl Battle_LimitFx21_LimFast(int targetMask, int actorIdx);

/* battle_fx_limit_sled_fang.cpp */
extern unsigned char g_BattleLimitSledFangOffsetTable1[192];
extern unsigned char g_BattleLimitSledFangOffsetTable2[120];
extern struct BattleLimitSledFangSpriteRequest1 g_BattleLimitSledFangSpriteRequest1;
extern struct BattleLimitSledFangSpriteRequest2 g_BattleLimitSledFangSpriteRequest2;
extern unsigned char g_BattleLimitSledFangResFilenameSlmo[13];
extern unsigned char g_BattleLimitSledFangResFilenameSmok[41];
void __cdecl BattleFx_LimitLoadResSledFang(void);
int __cdecl BattleFx_LimitEnterSledFang(int targetMask, int actorIdx);
int __cdecl BattleFx_LimitEnterBloodFang(int targetMask, int actorIdx);

/* battle_fx_limit_big_shot.cpp */
extern int g_BattleLimitBigShot_ResourceDefs;
extern int g_BattleLimitBigShot_Scale;
extern int g_BattleLimitBigShot_RotStep;
extern int g_BattleLimitBigShot_QuadReq1;
extern char g_BattleLimitBigShot_QuadReq2[8];
extern char g_BattleLimitBigShot_QuadReq3[8];
extern int g_BattleLimitBigShot_State1;
extern int g_BattleLimitBigShot_State2;
void __cdecl BattleFx_LimitLoadResBigShot(void);
int __cdecl Battle_LimitFx07_BigShot(int targetMask, int context);
void __cdecl BattleFx_LimitRegisterBigShotPerTargetCallback(int targetMask, int context);
void __cdecl BattleFx_LimitSpawnBigShotPerTarget(int actor, int value);
void __cdecl BattleFx_LimitBigShotTimelineTask(void);
void __cdecl BattleFx_LimitBigShotModelPairTask(void);
void __cdecl BattleFx_LimitBigShotRampHoldSpriteTask(void);
void __cdecl BattleFx_LimitBigShotSpinningSpriteTask(void);
void __cdecl BattleFx_LimitBigShotHoldFadeSpriteTask(void);

/* battle_fx_limit_cross_slash.cpp */
extern int g_BattleLimitCrossSlash_ResourceDefs;
extern int g_BattleLimitCrossSlash_DrawListHandle;
extern struct GfxTableCursor *g_BattleLimitCrossSlash_LoadedRes;
void __cdecl BattleFx_LimitLoadResCrossSlash(void);
int __cdecl Battle_LimitFx01_CrossSlash(int targetMask, int actorIndex);
void __cdecl BattleFx_LimitSpawnCrossSlashPerTarget(int actor);
void __cdecl BattleFx_LimitCrossSlashTimelineTask(void);
void __cdecl BattleFx_LimitCrossSlashShrinkingSpriteTask(void);
void __cdecl BattleFx_LimitCrossSlashTrailSpriteTask(void);

/* battle_fx_limit_braver.cpp */
extern int g_BattleLimitBraver_ResourceDefs;
void __cdecl BattleFx_LimitLoadResBraver(void);
int __cdecl Battle_LimitFx00_Braver(int targetMask, int actorIndex);
void __cdecl BattleFx_LimitRegisterBraverPerTargetCallback(int targetMask, int actorIndex);
void __cdecl BattleFx_LimitSpawnBraverPerTarget(int actor, int value);
void __cdecl BattleFx_LimitBraverTimelineTask(void);
void __cdecl BattleFx_LimitBraverActorMatrixSpriteTask(void);
void __cdecl BattleFx_LimitBraverTrailRibbonTask(void);
void __cdecl BattleFx_LimitBraverBillboardSpriteTask(void);

/* battle_fx_limit_healing_wind.cpp */
extern int g_BattleLimitHealingWind_ResourceDefs;
void __cdecl BattleFx_LimitLoadResHealingWind(void);
int __cdecl Battle_LimitFx14_HealingWind(int targetMask, int actorIndex);
void __cdecl BattleFx_LimitBeginHealingWind(int targetMask, int actorIndex);
void __cdecl BattleFx_LimitHealingWindTimelineTask(void);
void __cdecl BattleFx_LimitHealingWindMidpointModelTask(void);
void __cdecl BattleFx_LimitHealingWindSpawnSpiralSpritesTask(void);
void __cdecl BattleFx_LimitHealingWindAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitHealingWindParticleControllerTask(void);

/* battle_fx_limit_toy_soldier.cpp */
extern int g_BattleFxLimitToySoldierWorldMatrix;
int BattleFx_LimitEnterToySoldier(int targetMask, int actorIdx);
void BattleFx_LimitBeginToySoldier(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitToySoldierSpawnTimeline(void);
void BattleFx_LimitToySoldierTimelineTask(void);
void __cdecl BattleFx_LimitToySoldierAnimatedSpritesTask(void);

/* battle_fx_magic_ice3_multi.cpp */
extern unsigned char g_BattleMagicIce3MultiMesh1[5156];
extern unsigned char g_BattleMagicIce3MultiMesh2[5084];
extern unsigned int g_BattleMagicIce3MultiResourceDef[40];
extern int g_BattleMagicIce3MultiVertexColorLut[32];
extern unsigned char g_BattleMagicIce3MultiSpriteQuadDrawReqBase[8];
extern unsigned char g_BattleMagicIce3MultiSpriteQuadDrawReqColors[8];
extern unsigned char g_BattleMagicIce3MultiSpriteQuadDrawReqTail[144];
extern int g_BattleMagicIce3MultiSpriteQuadDrawIndexTable[30];
extern struct VECTOR g_BattleMagicIce3MultiUpVector;
extern int g_BattleMagicIce3MultiResIdx1;
extern int g_BattleMagicIce3MultiResIdx2;
extern struct BattleFxSpriteRequest g_BattleMagicIce3MultiSpriteDrawRequest;
extern short g_BattleMagicIce3MultiConvergingXBase[6];
extern short g_BattleMagicIce3MultiConvergingYBase[6];
extern short g_BattleMagicIce3MultiConvergingStageIndex[7];
extern short g_BattleMagicIce3MultiConvergingStepTable[6];
extern short g_BattleMagicIce3MultiConvergingXOffset[6];
extern short g_BattleMagicIce3MultiConvergingYOffset[6];
extern int *g_BattleMagicIce3MultiDrawListTaskSlot;
extern int g_BattleMagicIce3MultiDebrisRecord1;
extern int g_BattleMagicIce3MultiInitDoneFlag;
extern int g_BattleMagicIce3MultiDebrisRecord2;
extern int g_BattleMagicIce3MultiDrawBufferA;
extern int g_BattleMagicIce3MultiDrawBufferB;
extern int g_BattleMagicIce3MultiDrawBufferCurrent;
extern int g_BattleMagicIce3MultiTargetMask;
extern struct GfxTableCursor *g_BattleMagicIce3MultiLoadedResTable;
void __cdecl BattleFx_MagicLoadResIce3Multi(void);

/* battle_fx_magic_full_cure.cpp */
void __cdecl BattleFx_MagicLoadResFullCure(void);
void __cdecl BattleFx_MagicEnterFullCure(int targetMask, int actorIdx);

/* battle_fx_magic_shield.cpp */
extern unsigned char g_BattleMagicShieldModelMeshBlob[];
extern unsigned char g_BattleMagicShieldResNameTimRsd[22];
extern unsigned char g_BattleMagicShieldResNameRsd2[11];
void __cdecl BattleFx_MagicLoadResShield(void);
void __cdecl BattleFx_MagicEnterShield(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicShieldSetupTask(void);
void __cdecl BattleFx_MagicShieldComputeFaceVelocitiesFromBlock(struct BattleFxMagicShieldFace *face);
int __cdecl BattleFx_MagicShieldComputeFaceVelocities(struct BattleFxMagicShieldFace *face, int vertexBase, struct BattleMagicShieldFaceList *listHeader);
void __cdecl BattleFx_MagicShieldTimelineTask(void);
void __cdecl BattleFx_MagicShieldTumblingModelTask(void);
void __cdecl BattleFx_MagicShieldScaledModelTask(void);
int __cdecl BattleFx_MagicShieldDrawPsxTexturedTrianglesFromBlock(struct BattleFxShieldTriangleBlock *block, int extra);
int __cdecl BattleFx_MagicShieldDrawPsxTexturedTriangles(struct BattleFxShieldTriangleBlock *block, int vertices, int records, int fadeLevel);

/* battle_fx_magic_death.cpp */
extern struct BattleFxSpriteRequest g_BattleMagicDeathSpriteColor;
extern struct BattleFxDeathTrailEdge g_BattleMagicDeathTrailWorkingBlob1[17];
extern struct BattleMagicDeathTrailPoint g_BattleMagicDeathTrailSubdivisionPoints[9];
extern short g_BattleMagicDeathAttachPos[3];
void __cdecl BattleFx_MagicLoadResDeath(void);
int __cdecl BattleFx_MagicEnterDeath(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginDeath(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicDeathPlaceEffectTask(void);
void __cdecl BattleFx_MagicDeathTimelineTask(void);
void __cdecl BattleFx_MagicDeathLinearStripTimelineTask(void);
void __cdecl BattleFx_MagicDeathDrawPsxGouraudQuad(struct BattleFxDeathStripTrail *trail, int segment, int segmentCount);
void __cdecl BattleFx_MagicDeathCurvedStripTimelineTask(void);
void __cdecl BattleFx_MagicDeathDrawPsxGouraudQuadStrip(struct BattleFxDeathStripTrail *trail, int colorA, int colorB);
void __cdecl BattleFx_MagicDeathScatterSpritesFromTarget(void);
void __cdecl BattleFx_MagicDeathSpiralSpriteTask(void);
void __cdecl BattleFx_MagicDeathGatherSpritesToTarget(void);

/* battle_fx_magic_fire3_multi.cpp */
extern unsigned char g_BattleMagicFire3MultiPsxMeshBlob[5084];
extern unsigned char g_BattleMagicFire3MultiTextureNames[23];
extern unsigned char g_BattleMagicFire3MultiModelNames[24];
void __cdecl BattleFx_MagicLoadResFire3Multi(void);
void __cdecl BattleFx_MagicEnterFire3Multi(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginFire3Multi(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicFire3MultiSetupTask(void);
void __cdecl BattleFx_MagicFire3MultiTimelineTask(void);
void __cdecl BattleFx_MagicFire3MultiGrowingFadePsxModelTask(void);
int __cdecl BattleFx_MagicFire3MultiDrawPsxMesh(struct BattleFxFaigaMeshRecord *mesh, int shadeScale);
int __cdecl BattleFx_MagicFire3MultiDrawPsxTexturedTriangles(struct BattleFxFaigaMeshRecord *mesh, int vertexBase, int packetCursor, int shadeScale);
int __cdecl BattleFx_MagicFire3MultiDrawPsxTexturedQuads(struct BattleFxFaigaMeshRecord *mesh, int vertexBase, int packet, int shade);
int __cdecl BattleFx_MagicFire3MultiDrawPsxGouraudTriangles(struct BattleFxFaigaMeshRecord *mesh, int vertexBase, int packet, int shade);
int __cdecl BattleFx_MagicFire3MultiDrawPsxGouraudQuads(struct BattleFxFaigaMeshRecord *mesh, int vertexBase, int packet, int shade);
void __cdecl BattleFx_MagicFire3MultiGrowingFadeModelTask(void);
void __cdecl BattleFx_MagicFire3MultiFlashModelTask(void);
void __cdecl BattleFx_MagicFire3MultiSpawnShrinkingSpinModelsTask(void);
void __cdecl BattleFx_MagicFire3MultiShrinkingSpinModelTask(void);

/* battle_fx_magic_toad.cpp */
extern int g_BattleMagicToadParticleCount;
extern struct GfxTableCursor *g_BattleMagicToadLoadedResTable;
void __cdecl BattleFx_MagicLoadResToad(void);
void __cdecl BattleFx_MagicEnterToad(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginToad(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnToadPerTarget(int actor);
void __cdecl BattleFx_MagicToadTimelineTask(void);
void __cdecl BattleFx_MagicToadAnimatedSpritesTask(void);

/* battle_fx_magic_mini.cpp */
extern int g_BattleMagicMiniResourceDefs;
extern struct GfxTableCursor *g_BattleMagicMiniLoadedResTable;
void __cdecl BattleFx_MagicLoadResMini(void);
void __cdecl BattleFx_MagicEnterMini(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginMini(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnMiniPerTarget(int actor);
void __cdecl BattleFx_MagicMiniTimelineTask(void);
void __cdecl BattleFx_MagicMiniAnimatedSpritesTask(void);

/* battle_fx_magic_despell.cpp */
extern int g_BattleMagicDespellEffectData;
extern int g_BattleMagicDespellResourceDefs;
extern struct BattleMagicDespellReferenceVector g_BattleMagicDespellReferenceVector;
extern int g_BattleMagicDespellTargetCount;
extern struct GfxTableCursor *g_BattleMagicDespellLoadedResTable;
void __cdecl BattleFx_MagicLoadResDespell(void);
void __cdecl BattleFx_MagicEnterDespell(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginDespell(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnDespellPerTarget(int actor);
void __cdecl BattleFx_MagicDespellComputeMeshFaceNormals(struct BattleFxDespellMesh *mesh);
int * __cdecl BattleFx_MagicDespellComputeTriangleNormals(struct BattleFxDespellMesh *mesh, int base, int *packet);
int * __cdecl BattleFx_MagicDespellComputeQuadNormals(struct BattleFxDespellMesh *mesh, int base, int *packet);
void __cdecl BattleFx_MagicDespellTimelineTask(void);
int __cdecl BattleFx_MagicDespellDrawPsxFragmentStream(struct BattleFxDespellFragment *fragment, int shade);
void __cdecl BattleFx_MagicDespellCameraOffsetFadeModelTask(void);
void __cdecl BattleFx_MagicDespellSpawnCameraFacingModelsTask(void);
void __cdecl BattleFx_MagicDespellDrawCameraFacingModelTask(void);

/* battle_fx_magic_debarrier.cpp */
extern unsigned char g_BattleMagicDeBarrierModelNameLight1[12];
void __cdecl BattleFx_MagicLoadResDeBarrier(void);
void __cdecl BattleFx_MagicEnterDeBarrier(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginDeBarrier(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnDeBarrierPerTarget(int actor);
void __cdecl BattleFx_MagicDeBarrierInitShatterModel(struct BattleFxDeBarrierShatter *shatter);
int __cdecl BattleFx_MagicDeBarrierInitShatterFragments(struct BattleFxDeBarrierShatter *shatter, int vertexBase, int fragList);
void __cdecl BattleFx_MagicDeBarrierShatterModelTask(void);
int __cdecl BattleFx_MagicDeBarrierDrawShatterModel(struct BattleFxDeBarrierShatterDesc *shatter, int fadeLevel);
int __cdecl BattleFx_MagicDeBarrierCosineLookup(int angle);
void __cdecl BattleFx_MagicDeBarrierFlashBillboardModelTask(void);

/* battle_fx_magic_ultima.cpp */
extern int g_BattleMagicUltimaResourceDefs;
extern int g_BattleMagicUltimaMatrix;
void __cdecl BattleFx_MagicLoadResUltima(void);
void __cdecl BattleFx_MagicEnterUltima(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginUltima(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicUltimaSetupTask(void);
void __cdecl BattleFx_MagicUltimaSpawnDriftingQuadSpritesTask(void);
void __cdecl BattleFx_MagicUltimaDriftingQuadSpriteTask(void);
void __cdecl BattleFx_MagicUltimaGrowingModelTask(void);
void __cdecl BattleFx_MagicUltimaTimelineTask(void);
void __cdecl BattleFx_MagicUltimaSpinFadeModelTask(void);
int __cdecl BattleFx_MagicUltimaDrawPsxGouraudQuadRows(struct BattleFxPsxGouraudRowBlock *rowBlock);

/* battle_fx_magic_ice3.cpp */
void __cdecl BattleFx_MagicLoadResIce3(void);
extern unsigned char g_BattleMagicIce3MeshDesc[5672];
extern int g_BattleMagicIce3ResourceDefs;
extern int g_BattleMagicIce3TargetMask;
extern struct GfxTableCursor *g_BattleMagicIce3LoadedResTable;
void __cdecl BattleFx_MagicEnterIce3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginIce3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicIce3SetupTask(void);
void __cdecl BattleFx_MagicIce3InitSingleTargetShatterModel(struct BattleFxMagicIce3Fragment *fragment);
int __cdecl BattleFx_MagicIce3InitSingleTargetFragmentMotion(struct BattleFxMagicIce3Fragment *fragment, int blockBase, int entryBlock);
void __cdecl BattleFx_MagicIce3InitGroupShatterModel(struct BattleFxMagicIce3Fragment *fragment);
int __cdecl BattleFx_MagicIce3InitFragmentMotionFromNormals(struct BattleFxMagicIce3Fragment *fragment, int blockBase, int entryBlock);
void __cdecl BattleFx_MagicIce3TargetTimelineTask(void);
void __cdecl BattleFx_MagicIce3SingleTargetShatterModelTask(void);
int __cdecl BattleFx_MagicIce3CosineLookup(int angle);
void __cdecl BattleFx_MagicIce3GroupShatterModelTask(void);
int __cdecl BattleFx_MagicIce3DrawGroupShatterModel(struct BattleFxMagicIce3ShatterDraw *shatter, int threshold);
int __cdecl BattleFx_MagicIce3DrawPsxGouraudFragmentTriangles(struct BattleFxMagicIce3ShatterDraw *shatter, int blockBase, int entryBlock, int threshold);
void __cdecl BattleFx_MagicIce3ConvergingModelTask(void);
void __cdecl BattleFx_MagicIce3ConvergingFadeModelTask(void);
void __cdecl BattleFx_MagicIce3GroupTimelineTask(void);
void __cdecl BattleFx_MagicIce3SpiralRibbonTask(void);

/* battle_fx_magic_fire3.cpp */
extern int g_BattleMagicFire3ResourceDefs;
extern int g_BattleMagicFire3TargetMask;
extern struct GfxTableCursor *g_BattleMagicFire3LoadedResTable;
void __cdecl BattleFx_MagicLoadResFire3(void);
void __cdecl BattleFx_MagicEnterFire3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginFire3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicFire3SetupTask(void);
void __cdecl BattleFx_MagicFire3TimelineTask(void);
void __cdecl BattleFx_MagicFire3SpinGrowFadeModel2Task(void);
void __cdecl BattleFx_MagicFire3SpinGrowFadeModel3EarlyFadeTask(void);
void __cdecl BattleFx_MagicFire3SpinGrowFadeModel3Task(void);
void __cdecl BattleFx_MagicFire3FlashSpriteTask(void);
void __cdecl BattleFx_MagicFire3SpawnAnimatedSpritesTask(void);
void __cdecl BattleFx_MagicFire3AnimatedSpriteTask(void);

/* battle_fx_magic_flare.cpp */
extern int g_BattleMagicFlareTargetActor;
extern struct GfxTableCursor *g_BattleMagicFlareLoadedResTable;
void __cdecl BattleFx_MagicLoadResFlare(void);
void __cdecl BattleFx_MagicEnterFlare(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicFlareSetupTask(void);
void __cdecl BattleFx_MagicFlareDelayedSpawnTask(void);
void __cdecl BattleFx_MagicFlareSpinGrowFadeModel3Task(void);
void __cdecl BattleFx_MagicFlareSpinGrowFadeModel4Task(void);
void __cdecl BattleFx_MagicFlareSpinGrowFadeModel4LateFadeTask(void);
void __cdecl BattleFx_MagicFlareGrowFadeAnimatedSpriteTask(void);
void __cdecl BattleFx_MagicFlareGrowFadeRotatedSpriteTask(void);
void __cdecl BattleFx_MagicFlareTimelineTask(void);
extern unsigned int g_BattleMagicFlareResourceDefs[25];
extern short g_BattleMagicFlareFrameTable[];
extern struct BattleMagicFlareSpriteReq1 g_BattleMagicFlareSpriteReq1;
extern struct BattleMagicFlareSpriteReq2 g_BattleMagicFlareSpriteReq2;
extern short g_BattleMagicFlareOffsetTableX[44];
extern short g_BattleMagicFlareOffsetTableY[44];
void __cdecl BattleFx_MagicFlareFrameTableSpriteTask(void);
void __cdecl BattleFx_MagicFlareMovingSpriteTask(void);

/* battle_fx_magic_stop.cpp */
extern int g_BattleMagicStopResourceDefs;
extern short g_BattleMagicStopAngles[4];
extern short g_BattleMagicStopSubAngles[4];
extern struct BattleFxModelDrawHead g_BattleMagicStopModelReq;
extern struct GfxTableCursor *g_BattleMagicStopLoadedResTable;
void __cdecl BattleFx_MagicLoadResStop(void);
void __cdecl BattleFx_MagicEnterStop(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginStop(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicStopCentralModelTask(void);
void __cdecl BattleFx_MagicStopSideModelTask(void);
void __cdecl BattleFx_MagicStopAngleGatedModelTask(void);

/* battle_fx_magic_silence.cpp */
extern int g_BattleMagicSilenceResourceDefs;
extern struct GfxTableCursor *g_BattleMagicSilenceLoadedResTable;
void __cdecl BattleFx_MagicLoadResSilence(void);
void __cdecl BattleFx_MagicEnterSilence(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginSilence(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnSilencePerTarget(int actor);
void __cdecl BattleFx_MagicSilenceGrowingModelTask(void);

/* battle_fx_magic_quake.cpp */
extern int g_BattleMagicQuakeResourceDefs;
extern struct GfxTableCursor *g_BattleMagicQuakeLoadedResTable;
void __cdecl BattleFx_MagicLoadResQuake(void);
void __cdecl BattleFx_MagicEnterQuake(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginQuake(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnQuakePerTarget(int actor);
void __cdecl BattleFx_MagicQuakeGrowShrinkModelTask(void);

/* battle_fx_magic_quake3.cpp */
void __cdecl BattleFx_MagicLoadResQuake3(void);
void __cdecl BattleFx_MagicEnterQuake3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicQuake3SelectTimelineTask(void);
void __cdecl BattleFx_MagicQuake3BuildShardRecords(struct BattleFxQuake3ShardState *shardState);
int __cdecl BattleFx_MagicQuake3BuildTriangleShardRecords(struct BattleFxQuake3ShardState *shardState, int vertexBase, struct BattleFxShardTriSlot *records);
int __cdecl BattleFx_MagicQuake3BuildQuadShardRecords(struct BattleFxQuake3ShardState *shardState, int vertexBase, struct BattleFxShardTriSlot *records);
void __cdecl BattleFx_MagicQuake3CopyMeshVertices(struct SVECTOR *srcVertices);
void __cdecl BattleFx_MagicQuake3MultiTargetTimelineTask(void);
void __cdecl BattleFx_MagicQuake3FlatteningRingModelTask(void);
void __cdecl BattleFx_MagicQuake3BuildMeshVertexDeltas(struct SVECTOR *prevPose, struct SVECTOR *nextPose, struct BattleFxShatterMeshDelta *deltas);
void __cdecl BattleFx_MagicQuake3MultiTargetBurstModelTask(void);
void __cdecl BattleFx_MagicQuake3AddScaledMeshVertexDeltas(struct SVECTOR *dstVertices, struct BattleFxShatterMeshDelta *srcDeltas, int count, int scale);
int __cdecl BattleFx_MagicQuake3DrawShardMesh(struct BattleFxQuake3Shard *shard, int threshold);
int __cdecl BattleFx_MagicQuake3LookupCosine(int angle);
void __cdecl BattleFx_MagicQuake3FadingQuadParticleTask(void);
void __cdecl BattleFx_MagicQuake3GroundQuadTask(void);
void __cdecl BattleFx_MagicQuake3PulsingIndexedModelTask(void);
void __cdecl BattleFx_MagicQuake3SingleTargetTimelineTask(void);
void __cdecl BattleFx_MagicQuake3SingleTargetBurstModelTask(void);

/* battle_fx_magic_quake2.cpp */
void __cdecl BattleFx_MagicLoadResQuake2(void);
void __cdecl BattleFx_MagicEnterQuake2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginQuake2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicQuake2SelectTimelineTask(void);
void __cdecl BattleFx_MagicQuake2BuildShardRecords(struct BattleFxQuake2ShardBurst *burst);
int __cdecl BattleFx_MagicQuake2BuildTriangleShardRecords(struct BattleFxQuake2ShardBurst *burst, int meshBase, struct BattleFxQuake2ShardRecord *recordHeader);
int __cdecl BattleFx_MagicQuake2BuildQuadShardRecords(struct BattleFxQuake2ShardBurst *burst, int base, struct BattleFxQuake2ShardRecord *recordList);
void __cdecl BattleFx_MagicQuake2MultiTargetTimelineTask(void);
void __cdecl BattleFx_MagicQuake2ShardBurstTask(void);
int __cdecl BattleFx_MagicQuake2DrawShardMesh(struct BattleFxQuake2Shard *shard, int threshold);
int __cdecl BattleFx_MagicQuake2LookupCosine(int angle);
void __cdecl BattleFx_MagicQuake2FadingQuadParticleTask(void);
void __cdecl BattleFx_MagicQuake2ShrinkingSquareTask(void);
void __cdecl BattleFx_MagicQuake2FadingModelTask(void);
void __cdecl BattleFx_MagicQuake2SingleTargetTimelineTask(void);
void __cdecl BattleFx_MagicQuake2GrowShrinkModelTask(void);

/* battle_fx_magic_haste.cpp */
extern int g_BattleMagicHasteVertexBuffer;
extern int g_BattleMagicHasteVertexDeltaBuffer;
extern struct SVECTOR g_BattleMagicHasteTargetMeshPtr[130];
extern float g_BattleMagicHasteWeight1;
extern float g_BattleMagicHasteWeight2;
extern struct GfxTableCursor *g_BattleMagicHasteLoadedResTable;
void __cdecl BattleFx_MagicLoadResHaste(void);
void __cdecl BattleFx_MagicHasteBlendPolygonSetVertices(int vertexIndex, int param, struct BattleFxSlowBlendVertex *vertex);
void __cdecl BattleFx_MagicBeginHaste(int entity, int actorIdx);
void __cdecl BattleFx_MagicHasteSpawnOrbitingPairsTask(void);
void __cdecl BattleFx_MagicHasteOrbitingModelPairTask(void);
void __cdecl BattleFx_MagicHasteMorphModelVerticesTask(void);
void __cdecl BattleFx_MagicHasteBuildMeshVertexDeltas(struct SVECTOR *poseA, struct SVECTOR *poseB, struct SVECTOR *deltas);

/* battle_fx_magic_sleepel.cpp */
extern struct GfxTableCursor *g_BattleMagicSleepelLoadedResTable;
void __cdecl BattleFx_MagicLoadResSleepel(void);
void __cdecl BattleFx_MagicEnterSleepel(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginSleepel(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnSleepelPerTarget(int actor);
void __cdecl BattleFx_MagicSleepelSpinGrowFadeModelTask(void);
void __cdecl BattleFx_MagicSleepelSpawnQuadParticlesTask(void);
void __cdecl BattleFx_MagicSleepelQuadParticleTask(void);

/* battle_fx_magic_freeze.cpp */
extern struct SoundGroup g_BattleMagicFreezeSoundData;
extern unsigned char g_BattleMagicFreezeFilenameFreeze[13];
extern unsigned char g_BattleMagicFreezeFilenameSnow[22];
extern unsigned char g_BattleMagicFreezeFilenameIce[24];
extern unsigned char g_BattleMagicFreezeFilenameIceb[56];
extern unsigned char g_BattleMagicFreezeFilenameRing[24];
void __cdecl BattleFx_MagicLoadResFreeze(void);
void __cdecl BattleFx_MagicEnterFreeze(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicRegisterFreezePerTargetCallback(int entity, int bitPattern);
void __cdecl BattleFx_MagicFreezeTimelineTask(void);
void __cdecl BattleFx_MagicFreezeSpawnSpriteBurstTask(void);
void __cdecl BattleFx_MagicFreezeConvergingSpriteTask(void);
void __cdecl BattleFx_MagicFreezeFadeModelTask(void);
void __cdecl BattleFx_MagicFreezeScreenScaledModelTask(void);
void __cdecl BattleFx_MagicFreezeSpawnGrowingModelsTask(void);
void __cdecl BattleFx_MagicFreezeGrowingModelTask(void);
void __cdecl BattleFx_MagicFreezeGrowHoldFadeModelTask(void);
void __cdecl BattleFx_MagicFreezeSpawnDampedGrowthModelsTask(void);
void __cdecl BattleFx_MagicFreezeDampedGrowthModelTask(void);
void __cdecl BattleFx_MagicFreezeSpawnAnimatedSpritesTask(void);
void __cdecl BattleFx_MagicFreezeAnimatedSpriteTask(void);

/* battle_fx_magic_hitmark.cpp */
extern int g_BattleMagicHitmark_ResourceDef;
extern int g_BattleMagicHitmark_ParticleVertex1;
extern int g_BattleMagicHitmark_BurstScale;
extern struct Vec3sPacked g_BattleMagicHitmark_HitPos2;
extern int g_BattleMagicHitmark_TriangleCenter;
void __cdecl BattleFx_MagicLoadResHitmark(void);
void __cdecl BattleFx_MagicEnterHitmark(void);
void __cdecl BattleFx_MagicBeginHitmark(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicHitmarkParticlePoolTask(void);
void __cdecl BattleFx_MagicHitmarkDrawAndStepParticle(struct BattleFxMagicHitmarkParticle *particle);
void __cdecl BattleFx_MagicHitmarkTimelineTask(void);
void __cdecl BattleFx_MagicHitmarkImpactSpriteTask(void);

/* battle_fx_magic_resist.cpp */
extern int g_BattleMagicResist_ResourceDef;
extern int g_BattleMagicResist_TargetCount;
extern int g_BattleMagicResist_SpritesPerTick;
extern struct GfxTableCursor *g_BattleMagicResist_ResourceHandle;
void __cdecl BattleFx_MagicLoadResResist(void);
void __cdecl BattleFx_MagicEnterResist(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnResistPerTarget(int actor);
void __cdecl BattleFx_MagicResistSpawnRadialSpritesTask(void);
void __cdecl BattleFx_MagicResistFadingSpriteTask(void);

/* battle_fx_magic_esuna.cpp */
extern int g_BattleMagicEsuna_ResourceDef;
extern int g_BattleMagicEsuna_DrawArg1;
extern int g_BattleMagicEsuna_ParticleCount;
extern int g_BattleMagicEsuna_DrawHandle;
extern int g_BattleMagicEsuna_EffectState;
extern struct GfxTableCursor *g_pBattleMagicEsunaTableCursor;
void __cdecl BattleFx_MagicLoadResEsuna(void);
void __cdecl BattleFx_MagicEnterEsuna(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginEsuna(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnEsunaPerTarget(int actor);
void __cdecl BattleFx_MagicEsunaParticleEmitterTask(void);
void __cdecl BattleFx_MagicEsunaParticleSpriteTask(void);

/* battle_fx_magic_break.cpp */
extern int g_BattleMagicBreak_ResourceDef;
extern short g_BattleMagicBreak_BlackMatrix;
extern int g_BattleMagicBreak_DrawHandle;
extern struct GfxTableCursor *g_BattleMagicBreak_ModelTable;
void __cdecl BattleFx_MagicLoadResBreak(void);
void __cdecl BattleFx_MagicEnterBreak(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnBreakPerTarget(int actor);
void __cdecl BattleFx_MagicBreakTimelineTask(void);
void __cdecl BattleFx_MagicBreakGrowingModelFadeTask(void);
void __cdecl BattleFx_MagicBreakStagedModelFadeTask(void);
void __cdecl BattleFx_MagicBreakBlackModelGrowTask(void);

/* battle_fx_magic_bio3_multi.cpp */
extern int g_BattleMagicBio3Multi_ResourceDef;
#define g_BattleMagicBio3Multi_ResourceDefs (&g_BattleMagicBio3Multi_ResourceDef)
extern int g_BattleMagicBio3Multi_ResourceRequests;
extern struct BattleMagicBio3Multi_ModelData1 g_BattleMagicBio3Multi_ModelData1;
extern short g_BattleMagicBio3Multi_InitialScale;
extern struct BattleMagicBio3Multi_SpriteDrawReq1 g_BattleMagicBio3Multi_SpriteDrawReq1;
extern struct Vec3sPacked *g_BattleMagicBio3Multi_TargetMask;
extern int g_BattleMagicBio3Multi_LoadedRes;
extern int g_BattleMagicBio3Multi_EffectState;
extern struct Int64Pair g_BattleMagicBio3Multi_TargetCentroid;
extern struct GfxTableCursor *g_pBattleMagicBio3MultiTableCursor;
void __cdecl BattleFx_MagicLoadResBio3Multi(void);
void __cdecl BattleFx_MagicBio3MultiReadPolygonSetVertexFloat(int index, int userData, struct BattleFxHellBomberEndVertex *outVec);
void __cdecl BattleFx_MagicEnterBio3Multi(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginBio3Multi(int actorMask, int actorIdx);
void __cdecl BattleFx_MagicBio3MultiTimelineTask(void);
void __cdecl BattleFx_MagicBio3MultiPulsingGrowingModelTask(void);
void __cdecl BattleFx_MagicBio3MultiRisingFadeSpriteTask(void);
void __cdecl BattleFx_MagicBio3MultiSixFrameSpriteEmitterTask(void);
void __cdecl BattleFx_MagicBio3MultiSixFrameSpriteTask(void);

/* battle_fx_magic_bio3.cpp */
extern int g_BattleMagicBio3_ResourceRequests;
extern unsigned char g_BattleMagicBio3_ModelData1[];
void __cdecl BattleFx_MagicLoadResBio3(void);
void __cdecl BattleFx_MagicEnterBio3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBio3ReadPolygonSetVertexFloat(int index, int userData, struct BattleFxHellBomberEndVertex *outVec);
void __cdecl BattleFx_MagicBeginBio3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnBio3PerTarget(int actor);
void __cdecl BattleFx_MagicBio3TimelineTask(void);
void __cdecl BattleFx_MagicBio3WobblingMeshTask(void);
void __cdecl BattleFx_MagicBio3PulsingScaledModelTask(void);
void __cdecl BattleFx_MagicBio3RisingFadeSpriteEmitterTask(void);
void __cdecl BattleFx_MagicBio3RisingFadeSpriteTask(void);

/* battle_fx_magic_comet2.cpp */
extern struct PsxMatrix g_BattleMagicComet2_Matrix1;
extern struct BattleFxModelDrawHead g_BattleMagicComet2_ModelReq6;
extern struct PsxMatrix g_BattleMagicComet2_Matrix2;
extern struct BattleFxModelDrawHead g_BattleMagicComet2_ModelReq7;
extern struct PsxMatrix g_BattleMagicComet2_Matrix3;
extern struct BattleFxSpriteRequest g_BattleMagicComet2_SpriteDrawReq;
extern unsigned char g_BattleMagicComet2_ModelNames1[24];
extern unsigned char g_BattleMagicComet2_ModelNames2[24];
extern unsigned char g_BattleMagicComet2_ModelNames3[24];
extern unsigned char g_BattleMagicComet2_ModelNames4[24];
extern unsigned char g_BattleMagicComet2_TextureNames[23];
void __cdecl BattleFx_MagicLoadResComet2(void);
void __cdecl BattleFx_MagicEnterComet2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginComet2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicComet2TimelineTask(void);
void __cdecl BattleFx_MagicComet2MeteorSequenceTask(void);
void __cdecl BattleFx_MagicComet2DescendingModelTask(void);
void __cdecl BattleFx_MagicComet2SparkPixelTask(void);
void __cdecl BattleFx_MagicComet2ExpandingModelPairTask(void);
void __cdecl BattleFx_MagicComet2BouncingDebrisTask(void);
void __cdecl BattleFx_MagicComet2AnimatedSpriteTask(void);
void __cdecl BattleFx_MagicComet2WaitRampEndTask(void);

/* battle_fx_magic_bio2.cpp */
extern unsigned char g_BattleMagicBio2_MeshFrame1A[676];
extern unsigned char g_BattleMagicBio2_MeshFrame1B[676];
extern unsigned char g_BattleMagicBio2_MeshFrame2A[700];
extern unsigned char g_BattleMagicBio2_MeshFrame2B[700];
extern unsigned char g_BattleMagicBio2_MeshFrame3A[700];
extern unsigned char g_BattleMagicBio2_MeshFrame3B[700];
extern unsigned char g_BattleMagicBio2_MeshFrame4A[644];
extern unsigned char g_BattleMagicBio2_MeshFrame4B[644];
extern int g_BattleMagicBio2_ResourceDef;
extern unsigned char *g_BattleMagicBio2_MeshFramesA[4];
extern unsigned char *g_BattleMagicBio2_MeshFramesB[4];
extern int g_BattleMagicBio2_Indices1[4];
extern int g_BattleMagicBio2_Indices2[4];
extern short g_BattleMagicBio2_InitialScale;
extern unsigned char g_BattleMagicBio2_FileNames[];
extern int g_BattleMagicBio2_LoadedRes;
extern struct GfxTableCursor *g_BattleMagicBio2_TargetMask;
void __cdecl BattleFx_MagicLoadResBio2(void);
void __cdecl BattleFx_MagicEnterBio2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicRegisterBio2PerTargetCallback(int targetMask, int userData);
void __cdecl BattleFx_MagicSpawnBio2PerTarget(int actor, int param);
void __cdecl BattleFx_MagicBio2TimelineTask(void);
void __cdecl BattleFx_MagicBio2WobblingMeshTask(void);
void __cdecl BattleFx_MagicBio2SixFrameSpriteTask(void);
void __cdecl BattleFx_MagicBio2SpawnRisingFadeSpritesTask(void);
void __cdecl BattleFx_MagicBio2RisingFadeSpriteTask(void);

/* battle_fx_magic_bio.cpp */
extern int g_BattleMagicBio_ResourceDef;
extern int g_BattleMagicBio_DrawHandle;
extern struct GfxTableCursor *g_BattleMagicBio_ModelTable;
void __cdecl BattleFx_MagicLoadResBio(void);
void __cdecl BattleFx_MagicEnterBio(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnBioPerTarget(int actor, int param);
void __cdecl BattleFx_MagicBioTimelineTask(void);
void __cdecl BattleFx_MagicBioRisingFadeSpriteTask(void);
void __cdecl BattleFx_MagicBioSixFrameSpriteTask(void);

/* battle_fx_magic_comet.cpp */
extern unsigned char g_BattleMagicComet_MeteorDelayTable[16];
extern unsigned char g_BattleMagicComet_ModelNames1[24];
extern unsigned char g_BattleMagicComet_ModelNames2[24];
extern unsigned char g_BattleMagicComet_ModelNames3[24];
extern unsigned char g_BattleMagicComet_ModelNames4[24];
extern unsigned char g_BattleMagicComet_TextureNames[23];
void __cdecl BattleFx_MagicLoadResComet(void);
void __cdecl BattleFx_MagicEnterComet(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginComet(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnCometPerTarget(int actor);
void __cdecl BattleFx_MagicCometTimelineTask(void);
void __cdecl BattleFx_MagicCometDescendingModelTask(void);
void __cdecl BattleFx_MagicCometSparkPixelTask(void);
void __cdecl BattleFx_MagicCometExpandingModelPairTask(void);
void __cdecl BattleFx_MagicCometBouncingDebrisTask(void);
void __cdecl BattleFx_MagicCometAnimatedSpriteTask(void);
void __cdecl BattleFx_MagicCometWaitRampEndTask(void);

/* battle_fx_magic_demi3_multi.cpp */
extern int g_BattleMagicDemi3Multi_PacketCursor;
extern int g_BattleMagicDemi3Multi_ActorMask;
extern int g_BattleMagicDemi3Multi_PushLength;
extern struct GfxTableCursor *g_BattleMagicDemi3Multi_LoadedRes2;
extern unsigned char g_BattleMagicDemi3Multi_StepCounter[];
void __cdecl BattleFx_MagicLoadResDemi3Multi(void);
void __cdecl BattleFx_MagicLoadAndBeginDemi3Multi(int actor, int param);
void __cdecl BattleFx_MagicBeginDemi3Multi(int actor, int param);
void __cdecl BattleFx_MagicDemi3MultiTimelineTask(void);
void __cdecl BattleFx_MagicDemi3MultiStretchedLookModelTask(void);
void __cdecl BattleFx_MagicDemi3MultiConvergingParticleSpawnTask(void);
void __cdecl BattleFx_MagicDemi3MultiFadingQuadSpriteTask(void);
void __cdecl BattleFx_MagicDemi3MultiDrawQuadSprite(struct Vec3sPacked *vertexOffset, struct BattleFxSpriteColor *color, int flags);
void __cdecl BattleFx_MagicDemi3MultiRingBurstTask(void);
void __cdecl BattleFx_MagicDemi3MultiBurstQuadFadeOutTask(void);
void __cdecl BattleFx_MagicDemi3MultiScaledSpriteMeshTask(void);
void __cdecl BattleFx_MagicDemi3MultiSpawnBlackMeshTask(void);
void __cdecl BattleFx_MagicDemi3MultiBlackMeshTask(void);
void __cdecl BattleFx_MagicDemi3MultiWaitRampEndTask(void);

/* battle_fx_magic_demi3.cpp */
extern struct BattleFxMagicGravSegment g_BattleMagicDemi3SegmentRecords[20];
extern unsigned char g_BattleMagicDemi3_QuadReq1[8];
extern unsigned char g_BattleMagicDemi3_QuadReq2[8];
void __cdecl BattleFx_MagicLoadResDemi3(void);
void __cdecl BattleFx_MagicEnterDemi3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnDemi3PerTarget(int actor, int param);
void __cdecl BattleFx_MagicDemi3TimelineTask(void);
void __cdecl BattleFx_MagicDemi3TrailEmitterTask(void);
void __cdecl BattleFx_MagicDemi3TrailSpriteTask(void);
void __cdecl BattleFx_MagicDemi3GrowingQuadSpriteTask(void);
void __cdecl BattleFx_MagicDemi3RotatingSpriteCurveTask(void);
void __cdecl BattleFx_MagicDemi3AnimatedSpriteTask(void);
void __cdecl BattleFx_MagicDemi3SegmentEmitterTask(void);
int __cdecl BattleFx_MagicDemi3DrawSegmentRecords(void);

/* battle_fx_magic_demi2.cpp */
extern unsigned char g_BattleMagicDemi2_QuadReq1[8];
extern unsigned char g_BattleMagicDemi2_QuadReq2[8];
void __cdecl BattleFx_MagicLoadResDemi2(void);
void __cdecl BattleFx_MagicEnterDemi2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnDemi2PerTarget(int actor, int param);
void __cdecl BattleFx_MagicDemi2TimelineTask(void);
void __cdecl BattleFx_MagicDemi2TrailEmitterTask(void);
void __cdecl BattleFx_MagicDemi2TrailSpriteTask(void);
void __cdecl BattleFx_MagicDemi2ShrinkingQuadSpriteTask(void);
void __cdecl BattleFx_MagicDemi2SpriteRingTask(void);
void __cdecl BattleFx_MagicDemi2AnimatedSpriteTask(void);

/* battle_fx_magic_tornado.cpp */
extern int g_BattleMagicTornado_ResourceDefs;
extern unsigned char g_BattleMagicTornado_QuadReq[8];
void __cdecl BattleFx_MagicLoadResTornado(void);
void __cdecl BattleFx_MagicEnterTornado(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicRegisterTornadoPerTargetCallback(int targetMask, int param);
void __cdecl BattleFx_MagicSpawnTornadoPerTarget(int actor, int param);
void __cdecl BattleFx_MagicTornadoTimelineTask(void);
void __cdecl BattleFx_MagicTornadoSpiralParticleControllerTask(void);
void __cdecl BattleFx_MagicTornadoStripParticleControllerTask(void);
void __cdecl BattleFx_MagicTornadoModelPairTask(void);

/* battle_fx_magic_remove.cpp */
extern int g_BattleMagicRemove_ResourceDefs;
extern struct BattleMagicRemove_SpriteReq g_BattleMagicRemove_SpriteReq;
extern unsigned char g_BattleMagicRemove_QuadReq[8];
void __cdecl BattleFx_MagicLoadResRemove(void);
void __cdecl BattleFx_MagicEnterRemove(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginRemove(int targetIndex, int actorIdx);
void __cdecl BattleFx_MagicRemoveTimelineTask(void);
void __cdecl BattleFx_MagicRemoveTwinSpriteLayersTask(void);
void __cdecl BattleFx_MagicRemoveInwardArcSpriteTask(void);
void __cdecl BattleFx_MagicRemoveScaledModelTask(void);
void __cdecl BattleFx_MagicRemovePullActorTask(void);

/* battle_fx_magic_berserk.cpp */
extern int g_BattleMagicBerserk_ResourceDefs;
extern struct BattleMagicBerserk_SpriteReq g_BattleMagicBerserk_SpriteReq;
void __cdecl BattleFx_MagicLoadResBerserk(void);
void __cdecl BattleFx_MagicEnterBerserk(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnBerserkPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_MagicBerserkTimelineTask(void);
void __cdecl BattleFx_MagicBerserkSpinningModelTask(void);
void __cdecl BattleFx_MagicBerserkFadingSpriteTask(void);
void __cdecl BattleFx_MagicBerserkFallingSpriteTask(void);

/* battle_fx_magic_reflect.cpp */
void __cdecl BattleFx_MagicLoadResReflect(void);
void __cdecl BattleFx_MagicEnterReflect(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginReflect(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnReflectPerTarget(int actor);
void __cdecl BattleFx_MagicReflectTimelineTask(void);
void __cdecl BattleFx_MagicReflectScaledModelTask(void);

/* battle_fx_magic_mbarrier.cpp */
void __cdecl BattleFx_MagicLoadResMBarrier(void);
void __cdecl BattleFx_MagicEnterMBarrier(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginMBarrier(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnMBarrierPerTarget(int actor);
void __cdecl BattleFx_MagicMBarrierTimelineTask(void);
void __cdecl BattleFx_MagicMBarrierScaledModelTask(void);

/* battle_fx_magic_barrier.cpp */
extern struct PsxSVectorZ32 g_BattleMagicBarrier_OffsetVector;
void __cdecl BattleFx_MagicLoadResBarrier(void);
void __cdecl BattleFx_MagicEnterBarrier(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginBarrier(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnBarrierPerTarget(int actor);
void __cdecl BattleFx_MagicBarrierTimelineTask(void);
void __cdecl BattleFx_MagicBarrierGrowFadeModelTask(void);
void __cdecl BattleFx_MagicBarrierDualGrowFadeModelTask(void);

/* battle_fx_magic_slow.cpp */
extern struct BattleMagicSlow_ModelReq2 g_BattleMagicSlow_ModelReq2;
void __cdecl BattleFx_MagicLoadResSlow(void);
void __cdecl BattleFx_MagicEnterSlow(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSlowBlendPolygonSetVertices(int vertexIndex, struct BattleFxSlowBlendVertex *sourceVertex, struct BattleFxSlowBlendVertex *blendVertex);
void __cdecl BattleFx_MagicSlowTimelineTask(void);

/* battle_fx_magic_demi.cpp */
void __cdecl BattleFx_MagicLoadResDemi(void);
void __cdecl BattleFx_MagicEnterDemi(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnDemiPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_MagicDemiTravelEmitterTask(void);
void __cdecl BattleFx_MagicDemiTrailSpriteTask(void);
void __cdecl BattleFx_MagicDemiExpandingSpriteTask(void);
void __cdecl BattleFx_MagicDemiAnimatedSpriteTask(void);

/* battle_fx_magic_ice2.cpp */
void __cdecl BattleFx_MagicLoadResIce2(void);
void __cdecl BattleFx_MagicEnterIce2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnIce2PerTarget(int actor);
void __cdecl BattleFx_MagicIce2TimelineTask(void);
void __cdecl BattleFx_MagicIce2ShrinkingSpinModelTask(void);
void __cdecl BattleFx_MagicIce2GrowHoldFadeModelTask(void);

/* battle_fx_magic_ice.cpp */
void __cdecl BattleFx_MagicLoadResIce(void);
void __cdecl BattleFx_MagicEnterIce(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnIcePerTarget(int actor);
void __cdecl BattleFx_MagicIceSpawnRampModelTask(void);
void __cdecl BattleFx_MagicIceGrowingRampModelTask(void);

/* battle_fx_magic_bolt3_multi.cpp */
extern unsigned char g_BattleMagicBolt3Multi_Filename_Thundg02[13];
extern unsigned char g_BattleMagicBolt3Multi_Filename_Kami7[22];
extern unsigned char g_BattleMagicBolt3Multi_Filename_Stone[40];
extern unsigned char g_BattleMagicBolt3Multi_Filename_Thundg00[13];
extern unsigned char g_BattleMagicBolt3Multi_Filename_ElecY1[40];
extern unsigned char g_BattleMagicBolt3Multi_Filename_Thund3[12];
extern int g_BattleMagicBolt3Multi_ChainTail;
extern int g_BattleMagicBolt3Multi_ActorMask;
extern int g_BattleMagicBolt3Multi_CentroidXY;
extern short g_BattleMagicBolt3Multi_CentroidZ;
extern struct GfxTableCursor *g_BattleMagicBolt3Multi_ResourceTable;
void __cdecl BattleFx_MagicLoadResBolt3Multi(void);
void __cdecl BattleFx_MagicEnterBolt3Multi(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginBolt3Multi(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBolt3MultiTimelineTask(void);
void __cdecl BattleFx_MagicBolt3MultiFlashSpriteTask(void);
void __cdecl BattleFx_MagicBolt3MultiBouncingModelTask(void);
void __cdecl BattleFx_MagicBolt3MultiFlippedSpriteTask(void);
void __cdecl BattleFx_MagicBolt3MultiBillboardSpriteTask(void);
void __cdecl BattleFx_MagicBolt3MultiGrowHoldFadeModelTask(void);

/* battle_fx_magic_bolt3.cpp */
extern int g_BattleMagicBolt3_ResourceDefs;
extern short g_BattleMagicBolt3_InitialRotation;
extern int g_BattleMagicBolt3_ChainTail;
extern struct GfxTableCursor *g_BattleMagicBolt3_ResourceHandle;
void __cdecl BattleFx_MagicLoadResBolt3(void);
void __cdecl BattleFx_MagicEnterBolt3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginBolt3(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnBolt3PerTarget(int actor, int param);
void __cdecl BattleFx_MagicBolt3TimelineTask(void);
void __cdecl BattleFx_MagicBolt3FlashSpriteTask(void);
void __cdecl BattleFx_MagicBolt3BouncingModelTask(void);
void __cdecl BattleFx_MagicBolt3BillboardSpriteTask(void);
void __cdecl BattleFx_MagicBolt3GrowFadeModelTask(void);
void __cdecl BattleFx_MagicBolt3GrowHoldFadeModelTask(void);

/* battle_fx_magic_bolt2.cpp */
extern int g_BattleMagicBolt2_ResourceDefs;
extern struct GfxTableCursor *g_BattleMagicBolt2_ResourceHandle;
void __cdecl BattleFx_MagicLoadResBolt2(void);
void __cdecl BattleFx_MagicEnterBolt2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginBolt2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnBolt2PerTarget(int actor);
void __cdecl BattleFx_MagicBolt2TimelineTask(void);
void __cdecl BattleFx_MagicBolt2GrowFadeModelTask(void);
void __cdecl BattleFx_MagicBolt2FadeBillboardSpriteTask(void);
void __cdecl BattleFx_MagicBolt2BillboardSpriteTask(void);
void __cdecl BattleFx_MagicBolt2FlippedSpriteTask(void);

/* battle_fx_magic_bolt.cpp */
extern int g_BattleMagicBolt_ResourceDefs;
extern struct BattleMagicBolt_SpriteReq g_BattleMagicBolt_SpriteReq;
void __cdecl BattleFx_MagicLoadResBolt(void);
void __cdecl BattleFx_MagicEnterBolt(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicRegisterBoltPerTargetCallback(int targetId, int param);
void __cdecl BattleFx_MagicSpawnBoltPerTarget(int actor);
void __cdecl BattleFx_MagicBoltTimelineTask(void);
void __cdecl BattleFx_MagicBoltGrowFadeModelTask(void);
void __cdecl BattleFx_MagicBoltBillboardSpriteTask(void);
void __cdecl BattleFx_MagicBoltFlippedBillboardSpriteTask(void);

/* battle_fx_magic_fire2.cpp */
extern int g_BattleMagicFire2_ResourceDefs;
void __cdecl BattleFx_MagicLoadResFire2(void);
void __cdecl BattleFx_MagicEnterFire2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginFire2(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnFire2PerTarget(int actor);
void __cdecl BattleFx_MagicFire2CyclingModelTask(void);

/* battle_fx_magic_fire.cpp */
extern struct BattleCameraChannel g_BattleCameraEyeChannels[4];
extern int g_BattleMagicFire_ResourceDescriptor;
extern int g_BattleMagicFire_DrawListHandle;
extern struct GfxTableCursor *g_BattleMagicFire_LoadedRes;
extern int g_BattleSummonSavedDrawFlags;
extern char g_BattleApproachHoldFrames;
extern short g_BattleCameraAuxTaskSlot;
extern BattleTaskFn g_BattleOverlayTaskCallbacks[10];
extern short g_BattleSecondaryResourceCount;
extern char g_BattlePsxHookCalled;
void __cdecl BattleFx_MagicLoadResFire(void);
void __cdecl BattleFx_MagicEnterFire(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnFirePerTarget(int actor);
void __cdecl BattleFx_MagicFireTimelineTask(void);
void __cdecl BattleFx_MagicFireAnimatedSpriteTask(void);

/* battle_fx_magic_limit_flash.cpp */
extern int g_BattleLimitFlash_ResourceDescriptor;
extern short g_BattleLimitFlash_ModelScale0;
void __cdecl BattleFx_MagicEnterLimitFlash(void);
void __cdecl BattleFx_MagicLimitFlashSpawnForActor(int actorId, int mode);
void __cdecl BattleFx_MagicLimitFlashModel1SineScaleTask(void);
void __cdecl BattleFx_MagicLimitFlashDrawBlackModelPasses(int transform);
void __cdecl BattleFx_MagicLimitFlashModel0GrowTask(void);
void __cdecl BattleFx_MagicLimitFlashModel2ShrinkTask(void);
void __cdecl BattleFx_MagicLimitFlashRadialSpriteEmitterTask(void);
void __cdecl BattleFx_MagicLimitFlashRadialSpriteTask(void);
void __cdecl BattleFx_MagicLimitFlashPulseEmitterTask(void);
void __cdecl BattleFx_SpawnThreeFrameScreenFlashTask(void);
void __cdecl BattleFx_ThreeFrameScreenFlashTask(void);

/* battle_fx_model_draw.cpp */
struct BattleFxModelGroupEntry *__cdecl BattleFx_FindModelGroupByKey(int key, struct BattleFxModelGroupContainer *container);
void __cdecl BattleFx_LatchDrawTintRgb(int r, int g, int b);
void __cdecl BattleFx_LatchDrawBlendOverride(int blendMode, int value);
void __cdecl BattleFx_LatchDrawScalarOverride(int mode, int scalar);
void __cdecl BattleFx_LatchDrawTextureOverride(int *pTextureRecord);
int __cdecl BattleFx_SubmitSpriteDraw(void *pSpriteReq, int otBucket, int otShift, int packetCursor);
int __cdecl BattleFx_SubmitSpriteQuadDraw(struct BattleFxSpriteQuadRequest *pQuadReq, int otRoot, int otShift, int chainTail);

/* battle_fx_special_super_orb.cpp */
extern unsigned char g_BattleSpecialSuperOrbResFilenameHado[22];
extern unsigned char g_BattleSpecialSuperOrbResFilenameTama[22];
extern unsigned char g_BattleSpecialSuperOrbResFilenameMoto[21];
extern unsigned char g_BattleSpecialSuperOrbResFilenameSenkou[23];
extern unsigned char g_BattleSpecialSuperOrbResFilenameIwa1[21];
extern unsigned char g_BattleSpecialSuperOrbResFilenameIwa2[21];
extern unsigned char g_BattleSpecialSuperOrbResFilenameUchi[22];
extern unsigned char g_BattleSpecialSuperOrbResFilenameEnergy00[13];
extern unsigned char g_BattleSpecialSuperOrbResFilenameKou[8];
extern unsigned char g_BattleSpecialSuperOrbResFilenameEnergy03[13];
extern unsigned char g_BattleSpecialSuperOrbResFilenameKemu[39];
extern unsigned char g_BattleSpecialSuperOrbResFilenameEnergy03B[13];
extern unsigned char g_BattleSpecialSuperOrbResFilenameBaku[36];
void __cdecl BattleFx_SpecialLoadResSuperOrb(void);
void __cdecl BattleFx_SpecialEnterSuperOrb(int targetMask, int actorIdx);

/* battle_fx_special_scorch.cpp */
extern unsigned char g_BattleSpecialScorchResFilenameHado[22];
extern unsigned char g_BattleSpecialScorchResFilenameTama[22];
extern unsigned char g_BattleSpecialScorchResFilenameMoto[21];
extern unsigned char g_BattleSpecialScorchResFilenameSenkou[21];
extern unsigned char g_BattleSpecialScorchResFilenameIsi1[21];
extern unsigned char g_BattleSpecialScorchResFilenameIsi2[24];
extern unsigned char g_BattleSpecialScorchResFilenameKou[8];
extern unsigned char g_BattleSpecialScorchResFilenameShak1[23];
extern unsigned char g_BattleSpecialScorchResFilenameShak2[23];
extern unsigned char g_BattleSpecialScorchResFilenameBaku[22];
extern struct PsxMatrix g_BattleSpecialScorchDrawListHandle;
void __cdecl BattleFx_SpecialLoadResScorch(void);
void __cdecl BattleFx_SpecialEnterScorch(int targetMask, int actorIdx);

/* battle_fx_special_energy_orb.cpp */
extern unsigned int g_BattleSpecialEnergyOrbResourceDefs[73];
extern struct BattleSpecialEnergyOrbScaleTama g_BattleSpecialEnergyOrbScaleTama;
extern unsigned char g_BattleSpecialEnergyOrbRotationTama[12];
extern struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbVector1;
extern unsigned char g_BattleSpecialEnergyOrbModelConfig[16];
extern struct GfxRotationAngles2 g_BattleSpecialEnergyOrbRotationMoto;
extern struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbParamVector1;
extern struct GfxRotationAngles2 g_BattleSpecialEnergyOrbRotationIsi;
extern struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbParamVector2;
extern struct BattleSpecialEnergyOrbModelReqIsi g_BattleSpecialEnergyOrbModelReqMoto;
extern struct BattleSpecialEnergyOrbModelReqIsi g_BattleSpecialEnergyOrbModelReqIsi;
extern struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbModelReqTama;
extern struct GfxRotationAngles2 g_BattleSpecialEnergyOrbScaleTrajectory;
extern unsigned char g_BattleSpecialEnergyOrbVectorTrajectory[12];
extern struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbColor1;
extern struct BattleSpecialEnergyOrbRotationTrajectory g_BattleSpecialEnergyOrbRotationTrajectory;
extern struct GfxRotationAngles2 g_BattleSpecialEnergyOrbColor2;
extern unsigned char g_BattleSpecialEnergyOrbResFilenameHado[9];
extern short g_BattleSpecialEnergyOrbScaleCounter;
extern struct PsxMatrix g_BattleSpecialEnergyOrbBasisMatrix;
extern short g_BattleSpecialEnergyOrbLifetimeCounter;
extern int g_BattleSpecialEnergyOrbCentroidPos;
extern short g_BattleSpecialEnergyOrbCentroidZ;
extern short g_BattleSpecialEnergyOrbTargetMask;
extern short g_BattleSpecialEnergyOrbRandomAngle;
extern short g_BattleSpecialEnergyOrbFlashIntensity;
extern struct GfxTableCursor *g_BattleSpecialEnergyOrbLoadedResTable;
void __cdecl BattleFx_SpecialLoadResEnergyOrb(void);
void __cdecl BattleFx_SpecialEnterEnergyOrb(int targetMask, int actorIdx);

/* battle_fx_special_wave_cannon.cpp */
extern struct BattleSpecialWaveCannonScaleTama g_BattleSpecialWaveCannonScaleTama;
extern struct BattleSpecialWaveCannonVector1 g_BattleSpecialWaveCannonVector1;
extern struct BattleSpecialWaveCannonVector1 g_BattleSpecialWaveCannonParamVector1;
extern struct BattleSpecialWaveCannonVector1 g_BattleSpecialWaveCannonParamVector2;
extern struct BattleSpecialWaveCannonModelReqMoto g_BattleSpecialWaveCannonModelReqMoto;
extern struct BattleSpecialWaveCannonModelReqTama g_BattleSpecialWaveCannonModelReqTama;
extern struct BattleSpecialWaveCannonModelReqTama g_BattleSpecialWaveCannonRotationTama;
extern struct BattleSpecialWaveCannonRotationTrajectory g_BattleSpecialWaveCannonRotationTrajectory;
extern unsigned char g_BattleSpecialWaveCannonResFilenameHado[9];
void __cdecl BattleFx_SpecialLoadResWaveCannon(void);
void __cdecl BattleFx_SpecialEnterWaveCannon(int targetMask, int actorIdx);

/* battle_fx_special_shield.cpp */
extern short g_BattleSpecialShieldRotationVector[3];
extern struct BattleSpecialShieldModelReq g_BattleSpecialShieldModelReq;
extern int g_BattleSpecialShieldModelIndices[4];
extern int g_BattleSpecialShieldDrawListHandle;
void __cdecl BattleFx_SpecialLoadResShield(void);
void __cdecl BattleFx_SpecialEnterDualShield(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterShield(int targetMask, int actorIdx);

/* battle_fx_special_world_card.cpp */
extern int g_BattleSpecialWorldCardResourceDefs;
extern struct VECTOR g_BattleWorldCardNegativeZUnitVector;
extern short g_BattleSpecialWorldCardAngleOffsets[4];
extern short g_BattleSpecialWorldCardFadeLevels[4];
extern int g_BattleSpecialWorldCardTargetActorIdx;
extern int g_BattleSpecialWorldCardTargetMask;
extern int g_BattleSpecialWorldCardActorIdx;
extern struct GfxTableCursor *g_BattleSpecialWorldCardLoadedResTable;
void __cdecl BattleFx_SpecialLoadResWorldCard(void);
void __cdecl BattleFx_SpecialEnterWorldCard(int targetMask, int actorIdx);

/* battle_fx_special_joker_card.cpp */
extern int g_BattleSpecialJokerCardResourceDefs;
extern struct VECTOR g_BattleSpecialJokerCardNegativeZUnitVector;
extern unsigned char g_BattleSpecialJokerCardAxisAngleMatrix[];
extern int g_BattleSpecialJokerCardDrawListHandle;
extern int g_BattleSpecialJokerCardTargetMask;
extern int g_BattleSpecialJokerCardActorIdx;
extern struct GfxTableCursor *g_BattleSpecialJokerCardLoadedResTable;
void __cdecl BattleFx_SpecialLoadResJokerCard(void);
int __cdecl BattleFx_SpecialEnterJokerCard(int targetMask, int actorIdx);

/* battle_fx_special_sun_card.cpp */
extern int g_BattleSpecialSunCardResourceDefs;
extern struct VECTOR g_BattleSpecialSunCardNegativeZUnitVector;
extern unsigned char g_BattleSpecialSunCardAxisAngleMatrix[];
extern int g_BattleSpecialSunCardDrawListHandle;
extern int g_BattleSpecialSunCardTargetMask;
extern int g_BattleSpecialSunCardActorIdx;
extern struct GfxTableCursor *g_BattleSpecialSunCardLoadedResTable;
void __cdecl BattleFx_SpecialLoadResSunCard(void);
void __cdecl BattleFx_SpecialEnterSunCard(int targetMask, int actorIdx);

/* battle_fx_special_diamond_card.cpp */
extern int g_BattleSpecialDiamondCardResourceDefs;
extern struct VECTOR g_BattleSpecialDiamondCardNegativeZUnitVector;
extern int g_BattleSpecialDiamondCardFirstTargetIdx;
extern int g_BattleSpecialDiamondCardTargetMask;
extern int g_BattleSpecialDiamondCardActorIdx;
extern struct GfxTableCursor *g_BattleSpecialDiamondCardLoadedResTable;
void __cdecl BattleFx_SpecialLoadResDiamondCard(void);
void __cdecl BattleFx_SpecialEnterDiamondCard(int targetMask, int actorIdx);

/* battle_fx_special_star_card.cpp */
extern unsigned int g_BattleSpecialStarCardModelResourceTable[115];
extern unsigned char g_BattleSpecialStarCardFallingModelIndices[24];
extern unsigned char g_BattleSpecialStarCardCyclingModelIndices[40];
extern unsigned char g_BattleSpecialStarCardUpAxis[12];
extern struct VECTOR g_BattleSpecialStarCardForwardAxis;
extern struct BattleLimitHowlingMoonSpriteRequest g_BattleSpecialStarCardSpriteRequest1;
extern struct BattleLimitHowlingMoonSpriteRequest g_BattleSpecialStarCardSpriteRequest2;
extern struct BattleLimitStardustRayModelReq2 g_BattleSpecialStarCardSpriteRequest3;
extern unsigned char g_BattleSpecialStarCardResFilenameCard[22];
extern unsigned char g_BattleSpecialStarCardResFilenameBimu1[22];
extern unsigned char g_BattleSpecialStarCardResFilenameBimu2[22];
extern unsigned char g_BattleSpecialStarCardResFilenameElec[21];
extern unsigned char g_BattleSpecialStarCardResFilenameIsi2[21];
extern unsigned char g_BattleSpecialStarCardResFilenameIsi4[21];
extern unsigned char g_BattleSpecialStarCardResFilenameIsi6[23];
extern unsigned char g_BattleSpecialStarCardResFilenameSoto1[22];
extern unsigned char g_BattleSpecialStarCardResFilenameSoto2[22];
extern unsigned char g_BattleSpecialStarCardResFilenameSoto3[22];
extern unsigned char g_BattleSpecialStarCardResFilenameSoto4[22];
extern unsigned char g_BattleSpecialStarCardResFilenameSoto5[23];
extern unsigned char g_BattleSpecialStarCardResFilenameJoC[11];
extern int g_BattleSpecialStarCardDrawListHandle;
extern struct Int64Pair g_BattleSpecialStarCardTargetPos;
extern int g_BattleSpecialStarCardTargetMask;
extern int g_BattleSpecialStarCardActorIdx;
extern struct PsxMatrix g_BattleSpecialStarCardLoadedResTable;
extern struct GfxTableCursor *g_BattleSpecialStarCardFadeStep;
void __cdecl BattleFx_SpecialLoadResStarCard(void);
void __cdecl BattleFx_SpecialEnterStarCard(int targetMask, int actorIdx);

/* battle_fx_special_club_card.cpp */
extern struct VECTOR g_BattleSpecialClubCardForwardAxis;
extern unsigned char g_BattleFxClubCardCard1ModelNames[22];
extern unsigned char g_BattleFxClubCardQueikModelName[10];
extern int g_BattleFxClubCardTargetActorIdx;
extern int g_BattleFxClubCardTargetMask;
extern int g_BattleFxClubCardActorIndex;
extern struct GfxTableCursor *g_BattleFxClubCardLoadedResTable;
void __cdecl BattleFx_SpecialLoadResClubCard(void);
void __cdecl BattleFx_SpecialEnterClubCard(int targetMask, int actorIdx);

/* battle_fx_special_hidden_one_card.cpp */
extern struct VECTOR g_BattleSpecialHiddenOneCardForwardAxisFixed12;
extern int g_BattleSpecialHiddenOneCardTargetActorIdx;
extern int g_BattleSpecialHiddenOneCardActorMask;
extern int g_BattleSpecialHiddenOneCardParticleSizeMin;
extern int g_BattleSpecialHiddenOneCardParticleSizeMax;
extern int g_BattleSpecialHiddenOneCardCasterActor;
extern int g_BattleSpecialHiddenOneCardRadiusScaleFixed12;
extern int g_BattleSpecialHiddenOneCardSpreadRadius;
extern short g_BattleSpecialHiddenOneCardPosition[4];
extern int g_BattleSpecialHiddenOneCardScale;
extern struct GfxTableCursor *g_BattleSpecialHiddenOneCardLoadedRes;
extern struct GfxRotationAngles2 g_BattleSpecialHiddenOneCardAngles;
void __cdecl BattleFx_SpecialLoadResHiddenOneCard(void);
void __cdecl BattleFx_SpecialEnterHiddenOneCard(int targetMask, int actorIdx);

/* battle_fx_special_heart_card.cpp */
extern int g_BattleSpecialHeartCardResourceDefs;
extern struct VECTOR g_BattleSpecialHeartCardForwardAxisFixed12;
extern int g_BattleSpecialHeartCardTargetActorIdx;
extern int g_BattleSpecialHeartCardTargetMask;
extern int g_BattleSpecialHeartCardActorIndex;
extern struct GfxTableCursor *g_BattleSpecialHeartCardLoadedResTable;
void __cdecl BattleFx_SpecialLoadResHeartCard(void);
void __cdecl BattleFx_SpecialEnterHeartCard(int targetMask, int actorIdx);

/* battle_fx_special_emperor_card.cpp */
extern struct VECTOR g_BattleSpecialEmperorCardForwardAxisFixed12;
extern int g_BattleSpecialEmperorCardTargetActorIdx;
extern int g_BattleSpecialEmperorCardTargetMask;
extern int g_BattleSpecialEmperorCardActorIndex;
extern struct GfxTableCursor *g_BattleSpecialEmperorCardLoadedResTable;
void __cdecl BattleFx_SpecialLoadResEmperorCard(void);
void __cdecl BattleFx_SpecialEnterEmperorCard(int targetMask, int actorIdx);

/* battle_fx_special_spade_card.cpp */
extern struct VECTOR g_BattleSpecialSpadeCardForwardAxisFixed12;
extern int g_BattleSpecialSpadeCardTargetActorIdx;
extern int g_BattleSpecialSpadeCardTargetMask;
extern int g_BattleSpecialSpadeCardActorIndex;
extern struct GfxTableCursor *g_BattleSpecialSpadeCardLoadedResTable;
void __cdecl BattleFx_SpecialLoadResSpadeCard(void);
void __cdecl BattleFx_SpecialEnterSpadeCard(int targetMask, int actorIdx);

/* battle_fx_special_emerald_shoot.cpp */
extern unsigned char g_BattleSpecialEnergyWaveModelNames1[21];
extern unsigned char g_BattleSpecialEnergyWaveModelNames2[22];
extern unsigned char g_BattleSpecialEnergyWaveModelNames3[21];
extern unsigned char g_BattleSpecialEnergyWaveModelNames4[7];
extern unsigned char g_BattleSpecialEnergyWaveModelNames5[23];
extern unsigned char g_BattleSpecialEnergyWaveTextureName1[13];
extern unsigned char g_BattleSpecialEnergyWaveSpriteName1[8];
extern unsigned char g_BattleSpecialEnergyWaveTextureSpriteNames1[39];
extern unsigned char g_BattleSpecialEnergyWaveTextureName2[13];
extern unsigned char g_BattleSpecialEnergyWaveTextureSpriteNames2[38];
extern int g_BattleSpecialEnergyWaveModelRotation;
void __cdecl BattleFx_SpecialLoadResEnergyWave(void);
void __cdecl BattleFx_SpecialEnterEnergyWave(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterEnergyWavePerTargetCallback(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnEnergyWavePerTarget(int actor, int actorIdx);
void __cdecl BattleFx_SpecialEnergyWaveTimelineTask(void);
void __cdecl BattleFx_SpecialEnergyWaveBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyWaveExpandingModelTask(void);
void __cdecl BattleFx_SpecialEnergyWaveSpinModelsTask(void);
void __cdecl BattleFx_SpecialEnergyWaveAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyWaveMainSlotAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyWaveShrinkingSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyWaveStretchingModelsTask(void);
void __cdecl BattleFx_SpecialEnergyWaveBeamModelsTask(void);

/* battle_special_energy_effects.cpp */
extern unsigned char g_BattleSpecialEnergyModelNames1[21];
extern unsigned char g_BattleSpecialEnergyModelNames2[22];
extern unsigned char g_BattleSpecialEnergyModelNames3[21];
extern unsigned char g_BattleSpecialEnergyModelNames4[7];
extern unsigned char g_BattleSpecialEnergyModelNames5[23];
extern unsigned char g_BattleSpecialEnergyTextureName1[13];
extern unsigned char g_BattleSpecialEnergySpriteName1[8];
extern unsigned char g_BattleSpecialEnergyTextureSpriteNames1[39];
extern unsigned char g_BattleSpecialEnergyTextureName2[13];
extern unsigned char g_BattleSpecialEnergySpriteName2[10];
extern int g_BattleSpecialEnergyTimer;
void __cdecl BattleFx_SpecialLoadResEnergy(void);
void __cdecl BattleFx_SpecialEnterEnergy(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginEnergy(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnEnergyPerTarget(int actor, int actorIdx);
void __cdecl BattleFx_SpecialEnergyTimelineTask(void);
void __cdecl BattleFx_SpecialEnergyBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyExpandingModelTask(void);
void __cdecl BattleFx_SpecialEnergySpinModelsTask(void);
void __cdecl BattleFx_SpecialEnergyAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyMainSlotAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyShrinkingSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyStretchingModelsTask(void);
void __cdecl BattleFx_SpecialEnergyBeamModelsTask(void);

/* battle_special_spread_laser_effects.cpp */
extern struct BattleSpecialSpreadLaserSpriteConfig2 g_BattleSpecialSpreadLaserSpriteConfig2;
extern struct BattleSpecialSpreadLaserSpriteConfig3 g_BattleSpecialSpreadLaserSpriteConfig3;
extern unsigned char g_BattleSpecialSpreadLaserSpriteIndices1[40];
extern unsigned char g_BattleSpecialSpreadLaserSpriteIndices2[24];
extern struct BattleSpecialSpreadLaserSpriteConfig4 g_BattleSpecialSpreadLaserSpriteConfig4;
extern struct BattleSpecialSpreadLaserSpriteConfig5 g_BattleSpecialSpreadLaserSpriteConfig5;
extern unsigned char g_BattleSpecialSpreadLaserModelNames1[22];
extern unsigned char g_BattleSpecialSpreadLaserModelNames2[22];
extern unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames1[24];
extern unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames2[31];
extern unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames3[23];
extern unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames4[23];
extern unsigned char g_BattleSpecialSpreadLaserTextureSpriteNames5[23];
extern unsigned char g_BattleSpecialSpreadLaserSpriteName1[9];
extern struct PsxMatrix g_BattleSpecialSpreadLaserLookBasis;
extern int g_BattleSpecialSpreadLaserZeroRot;
void __cdecl BattleFx_SpecialLoadResSpreadLaser(void);
void __cdecl BattleFx_SpecialEnterSpreadLaser(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpreadLaserSpawnTimeline(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpreadLaserTimelineTask(void);
void __cdecl BattleFx_SpecialSpreadLaserAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialSpreadLaserAimedSpriteTask(void);
void __cdecl BattleFx_SpecialSpreadLaserStreakSpriteTask(void);
void __cdecl BattleFx_SpecialSpreadLaserModelSpriteTask(void);
void __cdecl BattleFx_SpecialSpreadLaserBeamSpriteTask(void);

/* battle_special_ultimate_blast_effects.cpp */
extern unsigned char g_BattleSpecUltimateBlastResName_wa[7];
extern unsigned char g_BattleSpecUltimateBlastResName_kage_netu1[22];
extern unsigned char g_BattleSpecUltimateBlastResName_uchi1_uchi2[22];
extern unsigned char g_BattleSpecUltimateBlastResName_uchi3_uchi4[22];
extern unsigned char g_BattleSpecUltimateBlastResName_uchi5_uchi6[22];
extern unsigned char g_BattleSpecUltimateBlastResName_uchi7_uchi8[22];
extern unsigned char g_BattleSpecUltimateBlastResName_iwa1_iwa2[21];
extern unsigned char g_BattleSpecUltimateBlastResName_iwa3_iwa4[21];
extern unsigned char g_BattleSpecUltimateBlastResName_breath03_1[13];
extern unsigned char g_BattleSpecUltimateBlastResName_hikari1_1[39];
extern unsigned char g_BattleSpecUltimateBlastResName_breath03_2[13];
extern unsigned char g_BattleSpecUltimateBlastResName_kemu1_1[38];
extern unsigned char g_BattleSpecUltimateBlastResName_breath02_1[13];
extern unsigned char g_BattleSpecUltimateBlastResName_onpa2[38];
extern unsigned char g_BattleSpecUltimateBlastResName_breath02_2[13];
extern unsigned char g_BattleSpecUltimateBlastResName_onpa4[38];
extern unsigned char g_BattleSpecUltimateBlastResName_breath02_3[13];
extern unsigned char g_BattleSpecUltimateBlastResName_onpa6[38];
void __cdecl BattleFx_SpecialLoadResUltimateBlast(void);
void __cdecl BattleFx_SpecialEnterUltimateBlast(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginUltimateBlast(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialUltimateBlastTimelineTask(void);
void __cdecl BattleFx_SpecialUltimateBlastScreenFlashTask(void);
void __cdecl BattleFx_SpecialUltimateBlastGrowingModelSparksTask(void);
void __cdecl BattleFx_SpecialUltimateBlastDriftingSpriteTask(void);
void __cdecl BattleFx_SpecialUltimateBlastExpandingShellsTask(void);
void __cdecl BattleFx_SpecialUltimateBlastAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialUltimateBlastRisingSpinModelTask(void);
void __cdecl BattleFx_SpecialUltimateBlastRisingFadeSpriteTask(void);

/* battle_fx_special_energy_ball.cpp */
extern struct BattleSpecEnergyBallSpriteRequest1 g_BattleSpecEnergyBallSpriteRequest1;
extern struct BattleSpecEnergyBallSpriteRequest2 g_BattleSpecEnergyBallSpriteRequest2;
extern struct BattleSpecEnergyBallTransformData1 g_BattleSpecEnergyBallTransformData1;
extern struct BattleSpecEnergyBallTransformData2 g_BattleSpecEnergyBallTransformData2;
extern struct BattleSpecEnergyBallTransformData3 g_BattleSpecEnergyBallTransformData3;
extern struct BattleSpecEnergyBallSpriteRequest3 g_BattleSpecEnergyBallSpriteRequest3;
extern struct BattleSpecEnergyBallTransformData4 g_BattleSpecEnergyBallTransformData4;
extern unsigned char g_BattleSpecEnergyBallModelName[10];
void __cdecl BattleFx_SpecialLoadResEnergyBall(void);
void __cdecl BattleFx_SpecialEnterEnergyBall(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginEnergyBall(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnEnergyBallPerTarget(int actor, int actorIdx);
void __cdecl BattleFx_SpecialEnergyBallTimelineTask(void);
void __cdecl BattleFx_SpecialEnergyBallAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyBallGrowingFadeSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyBallCounterSpinModelsTask(void);
void __cdecl BattleFx_SpecialEnergyBallStretchingModelTask(void);
void __cdecl BattleFx_SpecialEnergyBallBeamModelsTask(void);
void __cdecl BattleFx_SpecialEnergyBallFadingModelTask(void);

/* battle_special_radiant_breath_effects.cpp */
extern unsigned char g_BattleSpecRadiantBreathResName_wa[7];
extern unsigned char g_BattleSpecRadiantBreathResName_kage_netu1[22];
extern unsigned char g_BattleSpecRadiantBreathResName_uchi1_uchi2[22];
extern unsigned char g_BattleSpecRadiantBreathResName_uchi3_uchi4[22];
extern unsigned char g_BattleSpecRadiantBreathResName_uchi5_uchi6[22];
extern unsigned char g_BattleSpecRadiantBreathResName_uchi7_uchi8[22];
extern unsigned char g_BattleSpecRadiantBreathResName_iwa1_iwa2[21];
extern unsigned char g_BattleSpecRadiantBreathResName_iwa3_iwa4[21];
extern unsigned char g_BattleSpecRadiantBreathResName_domu_hikari[23];
extern unsigned char g_BattleSpecRadiantBreathResName_breath02_1[13];
extern unsigned char g_BattleSpecRadiantBreathResName_hahen1[38];
extern unsigned char g_BattleSpecRadiantBreathResName_breath02_2[13];
extern unsigned char g_BattleSpecRadiantBreathResName_hikari2[40];
extern unsigned char g_BattleSpecRadiantBreathResName_breath02_3[13];
extern unsigned char g_BattleSpecRadiantBreathResName_elec2_1[38];
extern unsigned char g_BattleSpecRadiantBreathResName_breath02_4[13];
extern unsigned char g_BattleSpecRadiantBreathResName_elec2_2[38];
void __cdecl BattleFx_SpecialLoadResRadiantBreath(void);
void __cdecl BattleFx_SpecialEnterRadiantBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginRadiantBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRadiantBreathTimelineTask(void);
void __cdecl BattleFx_SpecialRadiantBreathIntensityRampTask(void);
void __cdecl BattleFx_SpecialRadiantBreathGroundBurstTask(void);
void __cdecl BattleFx_SpecialRadiantBreathFrameTableSpriteTask(void);
void __cdecl BattleFx_SpecialRadiantBreathGrowingModelSparksTask(void);
void __cdecl BattleFx_SpecialRadiantBreathAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialRadiantBreathExpandingShellsTask(void);
void __cdecl BattleFx_SpecialRadiantBreathLargeAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialRadiantBreathRisingSpinModelTask(void);
void __cdecl BattleFx_SpecialRadiantBreathRisingFadeSpriteTask(void);

/* battle_fx_special_sewer_tsunami.cpp */
extern int g_BattleSpecSewerTsunamiResourceDef;
extern int g_BattleSpecSewerTsunamiState;
extern short g_BattleSpecSewerTsunamiActiveCount;
extern short g_BattleSpecSewerTsunamiTimer;
extern short g_BattleSpecSewerTsunamiStep;
extern struct GfxTableCursor *g_BattleSpecSewerTsunamiLoadedResTable;
void __cdecl BattleFx_SpecialLoadResSewerTsunami(void);
void __cdecl BattleFx_SpecialEnterSewerTsunami(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterSewerTsunamiAlt(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSewerTsunamiSpawnTimeline(int targetIdx, int actorIdx);
void __cdecl BattleFx_SpecialSewerTsunamiTimelineTask(void);
void __cdecl BattleFx_SpecialSewerTsunamiAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialSewerTsunamiWaveSurfaceTask(void);
void __cdecl BattleFx_SpecialSewerTsunamiMirrorTexCoordPair(void *coordPtrA, void *coordPtrB);
void __cdecl BattleFx_SpecialSewerTsunamiSpawnTimelineAlt(int targetIdx, int actorIdx);

/* battle_fx_special_waterball.cpp */
extern unsigned char g_BattleSpecWaterballResName_nemoto_doku03[23];
extern unsigned char g_BattleSpecWaterballResName_hassya_doku00[23];
extern unsigned char g_BattleSpecWaterballResName_doku1_doku01[23];
extern unsigned char g_BattleSpecWaterballResName_doku2_doku02[23];
extern unsigned char g_BattleSpecWaterballResName_baku1_joc05[23];
extern unsigned char g_BattleSpecWaterballResName_doku00_snow1[22];
extern unsigned char g_BattleSpecWaterballResName_doku00_snow2[22];
extern unsigned char g_BattleSpecWaterballResName_doku00_mizu[21];
extern unsigned char g_BattleSpecWaterballResName_doku00_tama[21];
void __cdecl BattleFx_SpecialLoadResWaterball(void);
void __cdecl BattleFx_SpecialEnterWaterball(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginWaterball(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialWaterballTimelineTask(void);
void __cdecl BattleFx_SpecialWaterballTargetAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialWaterballAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialWaterballSpinModelTask(void);
void __cdecl BattleFx_SpecialWaterballDriftingAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialWaterballSpiralSpriteTask(void);
void __cdecl BattleFx_SpecialWaterballFallingFadeSpriteTask(void);
void __cdecl BattleFx_SpecialWaterballProjectileSpriteTask(void);

/* battle_special_chefs_knife_effects.cpp */
void __cdecl BattleFx_SpecialLoadResChefsKnife(void);
void __cdecl BattleFx_SpecialEnterChefsKnife(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginChefsKnife(int actorMask, int actorIdx);
void __cdecl BattleFx_SpecialChefsKnifeSpawnTimelineTask(void);
void __cdecl BattleFx_SpecialChefsKnifeTimelineTask(void);
void __cdecl BattleFx_SpecialChefsKnifePulseFadeSpriteTask(void);
void __cdecl BattleFx_SpecialChefsKnifeAnimatedSpriteTask(void);

/* battle_special_frog_effects.cpp */
void __cdecl BattleFx_SpecialLoadResFrog(void);
void __cdecl BattleFx_SpecialEnterFrog(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterFrogPerTargetCallback(int actorMask, int sourceIndex);
void __cdecl BattleFx_SpecialSpawnFrogPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialFrogTimelineTask(void);
void __cdecl BattleFx_SpecialFrogAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialFrogGrowingModelTask(void);
void __cdecl BattleFx_SpecialFrogSpiralModelTask(void);
void __cdecl BattleFx_SpecialFrogTwoPhaseAnimatedSpriteTask(void);

/* battle_special_giant_sonic_wave_effects.cpp */
extern int g_BattleSpecGiantSonicWaveResourceDef;
extern struct BattleFxSpriteRequest g_BattleSpecGiantSonicWaveSpriteRequest;
extern int g_BattleSpecGiantSonicWaveSpinScaleX;
extern int g_BattleSpecGiantSonicWaveSpinScaleY;
extern unsigned char g_BattleSpecGiantSonicWaveResPaths0[];
extern unsigned char g_BattleSpecGiantSonicWaveResPaths1[];
extern int g_BattleSpecGiantSonicWaveDrawHandle;
extern int g_BattleSpecGiantSonicWaveTargetMask;
extern struct GfxTableCursor *g_BattleSpecGiantSonicWaveResTable;
extern short g_BattleSpecGiantSonicWaveFade;
void __cdecl BattleFx_SpecialLoadResGiantSonicWave(void);
void __cdecl BattleFx_SpecialEnterGiantSonicWave(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialGiantSonicWaveSpawnTimeline(int actorMask, int sourceIndex);
void __cdecl BattleFx_SpecialGiantSonicWaveTimelineTask(void);
void __cdecl BattleFx_SpecialGiantSonicWaveVariantSpriteTask(void);
void __cdecl BattleFx_SpecialGiantSonicWaveGrowingModelTask(void);
void __cdecl BattleFx_SpecialGiantSonicWaveSpinGrowingModelTask(void);
void __cdecl BattleFx_SpecialGiantSonicWaveSpinFadeSpriteTask(void);

/* battle_special_missile_salvo_effects.cpp */
extern int g_BattleSpecMissileSalvoResourceDef;
extern int g_BattleSpecMissileSalvoDrawListHead;
extern short g_BattleSpecMissileSalvoImpactCountdown;
extern struct GfxTableCursor *g_BattleSpecMissileSalvoLoadedResTable;
void __cdecl BattleFx_SpecialLoadResMissileSalvo(void);
void __cdecl BattleFx_SpecialEnterMissileSalvo(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterMissileSalvoPerTargetCallback(int actorMask, int sourceIndex);
void __cdecl BattleFx_SpecialSpawnMissileSalvoPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialMissileSalvoTimelineTask(void);
void __cdecl BattleFx_SpecialMissileSalvoBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialMissileSalvoGrowingModelTask(void);
void __cdecl BattleFx_SpecialMissileSalvoImpactModelTask(void);
void __cdecl BattleFx_SpecialMissileSalvoAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialMissileSalvoProjectileTask(void);

/* battle_fx_special_petrify_effect.cpp */
extern int g_BattleSpecPetrifyResourceDef;
extern int g_BattleSpecPetrifyGrowingModelAngles;
extern int g_BattleSpecPetrifyFaceSpinAngles;
void __cdecl BattleFx_SpecialLoadResPetrify(void);
void __cdecl BattleFx_SpecialEnterPetrify(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginPetrify(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialPetrifyInitParticleVelocities(void);
void __cdecl BattleFx_SpecialSpawnPetrifyPerTarget(int actor, int targetIndex);
void __cdecl BattleFx_SpecialPetrifyTimelineTask(void);
void __cdecl BattleFx_SpecialPetrifyVariantSpriteTask(void);
void __cdecl BattleFx_SpecialPetrifyGrowingModelTask(void);
void __cdecl BattleFx_SpecialPetrifySpawnOrbitSpritesTask(void);
void __cdecl BattleFx_SpecialPetrifyOrbitSpriteTask(void);
void __cdecl BattleFx_SpecialPetrifyPerFaceOffsetModelTask(void);
void __cdecl BattleFx_SpecialPetrifyFallingSpriteTask(void);
void __cdecl BattleFx_SpecialPetrifyUpdateBouncingParticles(void);
void __cdecl BattleFx_SpecialPetrifySpiralModelTask(void);
void __cdecl BattleFx_SpecialPetrifyTwoPhaseSpriteTask(void);

/* battle_fx_special_napalm_effect.cpp */
extern unsigned char g_BattleSpecNapalmRsdNames1[24];
extern unsigned char g_BattleSpecNapalmRsdNames2[23];
extern unsigned char g_BattleSpecNapalmRsdNames3[24];
extern unsigned char g_BattleSpecNapalmRsdNameWa[7];
extern unsigned char g_BattleSpecNapalmTimName02a[13];
extern unsigned char g_BattleSpecNapalmSptTimNames1[39];
extern unsigned char g_BattleSpecNapalmTimName02b[13];
extern unsigned char g_BattleSpecNapalmSptTimNames2[38];
extern unsigned char g_BattleSpecNapalmTimName01[13];
extern unsigned char g_BattleSpecNapalmSptTimNames3[39];
extern int g_BattleSpecNapalmDrawListHead;
extern int g_BattleSpecNapalmBlendStep;
extern short g_BattleSpecNapalmModelCounter;
extern struct GfxTableCursor *g_BattleSpecNapalmLoadedResTable;
extern int g_BattleSpecNapalmRotationMatrix;
void __cdecl BattleFx_SpecialLoadResNapalm(void);
void __cdecl BattleFx_SpecialEnterNapalm(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialNapalmBlendPartVectorsCallback(int unused, struct BattleFxHellBomberEndVertex *srcVec, struct BattleFxSlowBlendVertex *dstVec, struct BattleFxHellBomberEndVertex *outVec);
void __cdecl BattleFx_SpecialRegisterNapalmPerTargetCallback(int targetMask, int sourceActor);
void __cdecl BattleFx_SpecialSpawnNapalmPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialNapalmTimelineTask(void);
void __cdecl BattleFx_SpecialNapalmVariantSpriteTask(void);
void __cdecl BattleFx_SpecialNapalmArcingTrailEmitterTask(void);
void __cdecl BattleFx_SpecialNapalmMissileTask(void);
void __cdecl BattleFx_SpecialNapalmBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialNapalmGrowingModelTask(void);
void __cdecl BattleFx_SpecialNapalmImpactModelsTask(void);
void __cdecl BattleFx_SpecialNapalmFadingSpriteTask(void);

/* battle_fx_special_wing_scales_effect.cpp */
extern int g_BattleSpecWingScalesResourceDefs;
extern int g_BattleSpecWingScalesDrawListHead;
extern short g_BattleSpecWingScalesParticleLoopIndex;
extern short g_BattleSpecWingScalesFlashIntensity;
extern struct GfxTableCursor *g_BattleSpecWingScalesLoadedResTable;
extern short g_BattleSpecWingScalesParticleActiveFlag;
void __cdecl BattleFx_SpecialLoadResWingScales(void);
void __cdecl BattleFx_SpecialEnterWingScales(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginWingScales(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialWingScalesScreenFlashTask(void);
void __cdecl BattleFx_SpecialSpawnWingScalesPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialWingScalesTimelineTask(void);
void __cdecl BattleFx_SpecialWingScalesImpactModelTask(void);
void __cdecl BattleFx_SpecialWingScalesVariantSpriteTask(void);
void __cdecl BattleFx_SpecialWingScalesSpawnParticlePairsTask(void);
void __cdecl BattleFx_SpecialWingScalesInitParticlePairPool(void);
void __cdecl BattleFx_SpecialWingScalesParticlePairPoolTask(void);
void __cdecl BattleFx_SpecialWingScalesDrawParticlePairSprites(void);
short __cdecl BattleFx_SpecialWingScalesClaimParticlePairSlot(void);

/* battle_fx_special_heart_breath_effect.cpp */
extern struct BattleFxSpriteRequest g_BattleSpecHeartBreathParticleSpriteReq;
void __cdecl BattleFx_SpecialLoadResHeartBreath(void);
void __cdecl BattleFx_SpecialEnterHeartBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginHeartBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialHeartBreathTimelineTask(void);
void __cdecl BattleFx_SpecialHeartBreathParticleEmitterTask(void);
int __cdecl BattleFx_SpecialHeartBreathClaimParticleRecord(void);
void __cdecl BattleFx_SpecialHeartBreathGrowingModelTask(void);
void __cdecl BattleFx_SpecialHeartBreathTargetGrowingModelTask(void);
void __cdecl BattleFx_SpecialHeartBreathInitParticleSpritePool(void);
void __cdecl BattleFx_SpecialHeartBreathParticleSpritePoolTask(void);
void __cdecl BattleFx_SpecialHeartBreathUpdateParticleSprite(void);
short __cdecl BattleFx_SpecialHeartBreathClaimParticleSpriteSlot(void);

/* battle_fx_special_multi_jammer_effect.cpp */
extern struct BattleLimitStardustRayModelReq2 g_BattleSpecMultiJammerSpriteReq1;
extern struct BattleSpecHeatWaveModelReq2 g_BattleSpecMultiJammerSpriteReq2;
extern struct BattleSpecMultiJammerScale12 g_BattleSpecMultiJammerScale12;
extern struct BattleSpecHeatWaveModelReq2 g_BattleSpecMultiJammerSpriteReq3;
extern struct BattleSpecMultiJammerSpriteReq4 g_BattleSpecMultiJammerSpriteReq4;
extern int g_BattleSpecMultiJammerDrawHandle;
extern int g_BattleSpecMultiJammerTargetMask;
extern struct GfxTableCursor *g_BattleSpecMultiJammerLoadedResTable;
extern short g_BattleSpecMultiJammerSpinStep;
void __cdecl BattleFx_SpecialLoadResMultiJammer(void);
void __cdecl BattleFx_SpecialEnterMultiJammer(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginMultiJammer(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialMultiJammerTimelineTask(void);
void __cdecl BattleFx_SpecialMultiJammerVariantSpriteTask(void);
void __cdecl BattleFx_SpecialMultiJammerGrowingModelTask(void);
void __cdecl BattleFx_SpecialMultiJammerSpinGrowingModelTask(void);
void __cdecl BattleFx_SpecialMultiJammerSpinFadeSpriteTask(void);

/* battle_fx_special_jammer_effect.cpp */
extern int g_BattleSpecJammerResourceDefs;
extern struct BattleLimitStardustRayModelReq2 g_BattleSpecJammerSpriteReq1;
extern int g_BattleSpecJammerDrawListHead;
extern struct GfxTableCursor *g_BattleSpecJammerLoadedResTable;
void __cdecl BattleFx_SpecialLoadResJammer(void);
void __cdecl BattleFx_SpecialEnterJammer(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginJammer(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnJammerPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialJammerTimelineTask(void);
void __cdecl BattleFx_SpecialJammerVariantSpriteTask(void);
void __cdecl BattleFx_SpecialJammerGrowingModelTask(void);
void __cdecl BattleFx_SpecialJammerSpinFadeSpriteTask(void);

/* battle_fx_special_mortar_effect.cpp */
extern unsigned char g_BattleSpecMortarResourceNames[];
void __cdecl BattleFx_SpecialLoadResMortar(void);
void __cdecl BattleFx_SpecialEnterMortar(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginMortar(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnMortarPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialMortarTimelineTask(void);
void __cdecl BattleFx_SpecialMortarOrientedSpriteTask(void);
void __cdecl BattleFx_SpecialMortarGravitySpriteTask(void);
void __cdecl BattleFx_SpecialMortarSpinModelTask(void);
void __cdecl BattleFx_SpecialMortarExpandingShellsTask(void);

/* battle_fx_special_dual_mortar_effect.cpp */
extern int g_BattleSpecDualMortarResourceDefs;
extern struct GfxTableCursor *g_BattleSpecDualMortarLoadedResTable;
void __cdecl BattleFx_SpecialLoadResDualMortar(void);
void __cdecl BattleFx_SpecialEnterDualMortar(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginDualMortar(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnDualMortarPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialDualMortarTimelineTask(void);
void __cdecl BattleFx_SpecialDualMortarAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialDualMortarFallingDebrisSpriteTask(void);
void __cdecl BattleFx_SpecialDualMortarExpandingModelTask(void);
void __cdecl BattleFx_SpecialDualMortarExpandingShellsTask(void);

/* battle_fx_special_poison_gas_effect.cpp */
extern int g_BattleSpecPoisonGasResourceDefs;
extern struct BattleSpecPoisonGasSpriteReq1 g_BattleSpecPoisonGasSpriteReq1;
extern int g_BattleSpecPoisonGasDrawListHead;
extern struct GfxTableCursor *g_BattleSpecPoisonGasLoadedResTable;
void __cdecl BattleFx_SpecialLoadResPoisonGas(void);
void __cdecl BattleFx_SpecialEnterPoisonGas(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginPoisonGas(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnPoisonGasPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialPoisonGasTimelineTask(void);
void __cdecl BattleFx_SpecialPoisonGasAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialPoisonGasBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialPoisonGasProjectileTask(void);

/* battle_fx_special_acid_shower_effect.cpp */
extern int g_BattleSpecAcidShowerResourceDefs;
extern int g_BattleSpecAcidShowerDrawListHead;
extern struct GfxTableCursor *g_BattleSpecAcidShowerLoadedResTable;
void __cdecl BattleFx_SpecialLoadResAcidShower(void);
void __cdecl BattleFx_SpecialEnterAcidShower(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginAcidShower(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnAcidShowerPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialAcidShowerTimelineTask(void);
void __cdecl BattleFx_SpecialAcidShowerAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialAcidShowerImpactModelTask(void);
void __cdecl BattleFx_SpecialAcidShowerFallingSpriteTask(void);
void __cdecl BattleFx_SpecialAcidShowerSpiralEmitterTask(void);
void __cdecl BattleFx_SpecialAcidShowerSpiralSpritePairTask(void);

/* battle_fx_special_grand_spark.cpp */
void __cdecl BattleFx_SpecialLoadResGrandSpark(void);
void __cdecl BattleFx_SpecialEnterGrandSpark(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginGrandSpark(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnGrandSparkPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialGrandSparkTimelineTask(void);
void __cdecl BattleFx_SpecialGrandSparkAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialGrandSparkGrowingSpinModelTask(void);
void __cdecl BattleFx_SpecialGrandSparkBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialGrandSparkProjectileTask(void);
void __cdecl BattleFx_SpecialGrandSparkFixedAnimSpriteTask(void);
void __cdecl BattleFx_SpecialGrandSparkStreakModelTask(void);

/* battle_fx_special_tail_laser.cpp */
extern unsigned char g_BattleSpecTailLaserRsdNames1[21];
extern unsigned char g_BattleSpecTailLaserRsdNames2[23];
extern unsigned char g_BattleSpecTailLaserSptNames1[23];
extern unsigned char g_BattleSpecTailLaserSptNames2[23];
extern unsigned char g_BattleSpecTailLaserSptNames3[23];
extern unsigned char g_BattleSpecTailLaserSptNames4[23];
extern unsigned char g_BattleSpecTailLaserSptNames5[23];
extern unsigned char g_BattleSpecTailLaserSptNames6[23];
extern unsigned char g_BattleSpecTailLaserSptNames7[22];
extern unsigned char g_BattleSpecTailLaserTimNames1[23];
extern unsigned char g_BattleSpecTailLaserTimNames2[23];
extern int g_BattleSpecTailLaserSourceBonePos;
extern short g_BattleSpecTailLaserSourceBonePosZ;
extern int g_BattleSpecTailLaserDrawListHead;
extern short g_BattleSpecTailLaserTargetMask;
extern struct GfxTableCursor *g_BattleSpecTailLaserLoadedResTable;
extern int g_BattleSpecTailLaserRotationVector;
void __cdecl BattleFx_SpecialLoadResTailLaser(void);
void __cdecl BattleFx_SpecialEnterTailLaser(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialTailLaserSpawnTimeline(int targetMask, int sourceActor);
void __cdecl BattleFx_SpecialTailLaserTimelineTask(void);
void __cdecl BattleFx_SpecialTailLaserBeamTask(void);
void __cdecl BattleFx_SpecialTailLaserAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialTailLaserBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialTailLaserBeamSegmentTask(void);
void __cdecl BattleFx_SpecialTailLaserTargetTimelineTask(void);
void __cdecl BattleFx_SpecialTailLaserGrowingFadeSpriteTask(void);
void __cdecl BattleFx_SpecialTailLaserRadialSparkSpriteTask(void);
void __cdecl BattleFx_SpecialTailLaserPulsingModelTask(void);

/* battle_fx_special_electric_shock_effect.cpp */
extern char g_BattleSpecElectricShockOffsets24[24];
extern unsigned char g_BattleSpecElectricShockColorParam[8];
void __cdecl BattleFx_SpecialLoadResElectricShock(void);
void __cdecl BattleFx_SpecialEnterElectricShock(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialElectricShockTimelineTask(void);
void __cdecl BattleFx_SpecialSpawnElectricShockPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialElectricShockPerTargetTimelineTask(void);
void __cdecl BattleFx_SpecialElectricShockTrailEmitterTask(void);
void __cdecl BattleFx_SpecialElectricShockTrailSpriteTask(void);
void __cdecl BattleFx_SpecialElectricShockShrinkingFadeSpriteTask(void);
void __cdecl BattleFx_SpecialElectricShockRingSpritesTask(void);
void __cdecl BattleFx_SpecialElectricShockLargeFadingSpriteTask(void);
void __cdecl BattleFx_SpecialElectricShockSmallFadingSpriteTask(void);

/* battle_fx_special_avalanche.cpp */
extern unsigned char g_BattleSpecAvalancheRsdNames[24];
extern unsigned char g_BattleSpecAvalancheBakuNames[22];
extern unsigned char g_BattleSpecAvalancheHitNames[21];
extern unsigned char g_BattleSpecAvalancheJoNames[21];
extern unsigned char g_BattleSpecAvalancheKoNames[9];
extern int g_BattleSpecAvalancheScale;
extern int g_BattleSpecAvalancheScaleY;
extern int g_BattleSpecAvalancheScaleZ;
extern int g_BattleSpecAvalancheDrawListHead;
extern short g_BattleSpecAvalancheTargetMask;
extern struct GfxTableCursor *g_BattleSpecAvalancheLoadedResTable;
extern int g_BattleSpecAvalancheRotationVector;
void __cdecl BattleFx_SpecialLoadResAvalanche(void);
void __cdecl BattleFx_SpecialEnterAvalanche(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginAvalanche(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialAvalancheSortFaceOrder(void);
void __cdecl BattleFx_SpecialAvalancheTimelineTask(void);
void __cdecl BattleFx_SpecialAvalancheStaggeredFaceOffsetTask(void);
void __cdecl BattleFx_SpecialAvalanchePerTargetFaceOffsetModelTask(void);
void __cdecl BattleFx_SpecialAvalancheYDriftAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialAvalancheStaticAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialAvalancheGrowingModelTask(void);
void __cdecl BattleFx_SpecialAvalancheYZDriftAnimatedSpriteTask(void);

/* battle_fx_special_hell_bomber.cpp */
extern struct BattleSpecHellBomberSpriteReq1 g_BattleSpecHellBomberSpriteReq1;
extern struct BattleSpecHellBomberSpriteReq2 g_BattleSpecHellBomberSpriteReq2;
extern struct BattleSpecHellBomberSpriteReq3 g_BattleSpecHellBomberSpriteReq3;
extern struct BattleSpecHellBomberSpriteReq4 g_BattleSpecHellBomberSpriteReq4;
extern struct BattleSpecHellBomberModelReq3 g_BattleSpecHellBomberModelReq3;
extern struct BattleSpecHellBomberModelReq2 g_BattleSpecHellBomberModelReq2;
extern unsigned char g_BattleSpecHellBomberMisaName[9];
extern int g_BattleSpecHellBomberDrawListHead;
extern int g_BattleSpecHellBomberCounter;
extern short g_BattleSpecHellBomberTargetMask;
extern struct GfxTableCursor *g_BattleSpecHellBomberLoadedResTable;
extern int g_BattleSpecHellBomberRotVector;
void __cdecl BattleFx_SpecialLoadResHellBomber(void);
void __cdecl BattleFx_SpecialEnterHellBomber(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialHellBomberInterpolateVertexCallback(int ignored, struct BattleFxHellBomberEndVertex *endVertex, struct BattleFxSlowBlendVertex *startVertex, struct BattleFxHellBomberEndVertex *outVertex);
void __cdecl BattleFx_SpecialRegisterHellBomberPerTargetCallback(int sourceActor, int targetActor);
void __cdecl BattleFx_SpecialSpawnHellBomberPerTarget(int actor, int targetActor);
void __cdecl BattleFx_SpecialHellBomberTimelineTask(void);
void __cdecl BattleFx_SpecialHellBomberVariantSpriteTask(void);
void __cdecl BattleFx_SpecialHellBomberFallingSpriteEmitterTask(void);
void __cdecl BattleFx_SpecialHellBomberRocketTask(void);
void __cdecl BattleFx_SpecialHellBomberBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialHellBomberExpandingModelTask(void);
void __cdecl BattleFx_SpecialHellBomberGrowingMorphModelsTask(void);
void __cdecl BattleFx_SpecialHellBomberFadingAnimatedSpriteTask(void);

/* battle_fx_special_rider_breath.cpp */
void __cdecl BattleFx_SpecialLoadResRiderBreath(void);
void __cdecl BattleFx_SpecialEnterRiderBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginRiderBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnRiderBreathPerTarget(int actor, int targetActor);
void __cdecl BattleFx_SpecialRiderBreathTimelineTask(void);
void __cdecl BattleFx_SpecialRiderBreathGrowingSpriteTask(void);
void __cdecl BattleFx_SpecialRiderBreathVariantSpriteTask(void);
void __cdecl BattleFx_SpecialRiderBreathTrailSpritesTask(void);
void __cdecl BattleFx_SpecialRiderBreathExpandingModelTask(void);

/* battle_fx_special_light_shell.cpp */
extern struct BattleSpecLightShellSpriteReq1 g_BattleSpecLightShellSpriteReq1;
extern struct BattleSpecLightShellSpriteReq2 g_BattleSpecLightShellSpriteReq2;
extern struct BattleSpecLightShellModelReq g_BattleSpecLightShellModelReq;
extern unsigned char g_BattleSpecLightShellHikariName[11];
extern int g_BattleSpecLightShellDrawListHead;
extern struct GfxTableCursor *g_BattleSpecLightShellLoadedResTable;
void __cdecl BattleFx_SpecialLoadResLightShell(void);
void __cdecl BattleFx_SpecialEnterLightShell(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRunLightShell(int sourceActor, int targetActor);
void __cdecl BattleFx_SpecialSpawnLightShellPerTarget(int actor, int targetActor);
void __cdecl BattleFx_SpecialLightShellTimelineTask(void);
void __cdecl BattleFx_SpecialLightShellVariantSpriteTask(void);
void __cdecl BattleFx_SpecialLightShellTrailSpritesTask(void);
void __cdecl BattleFx_SpecialLightShellExpandingModelTask(void);

/* battle_fx_special_freeze_breath_effect.cpp */
extern int g_BattleSpecFreezeBreathRotationVector;
void __cdecl BattleFx_SpecialLoadResFreezeBreath(void);
void __cdecl BattleFx_SpecialEnterFreezeBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginFreezeBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialFreezeBreathBuildDescendingOrder(void);
void __cdecl BattleFx_SpecialSpawnFreezeBreathPerTarget(int actor, int targetActor);
void __cdecl BattleFx_SpecialFreezeBreathTimelineTask(void);
void __cdecl BattleFx_SpecialFreezeBreathDrawScaledSpriteTask(void);
void __cdecl BattleFx_SpecialFreezeBreathScatteredAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialFreezeBreathStagedFaceOffsetModelTask(void);
void __cdecl BattleFx_SpecialFreezeBreathSpinModelsEmitTask(void);
void __cdecl BattleFx_SpecialFreezeBreathOrbitingSpriteTask(void);
void __cdecl BattleFx_SpecialFreezeBreathMovingModelTask(void);

/* battle_fx_special_dark_wave_effect.cpp */
extern unsigned int g_BattleSpecDarkWaveResourceDefs[70];
extern struct BattleFxSpriteRequest g_BattleSpecDarkWaveSpriteReq1;
extern struct BattleFxSpriteRequest g_BattleSpecDarkWaveSpriteReq2;
extern unsigned char g_BattleSpecDarkWaveScaleVector[12];
extern struct BattleFxSpriteRequest g_BattleSpecDarkWaveSpriteReq3;
extern unsigned char g_BattleSpecDarkWaveModelIndices[40];
extern unsigned char g_BattleSpecDarkWaveMisa1Name[22];
extern unsigned char g_BattleSpecDarkWaveMisa3Name[22];
extern unsigned char g_BattleSpecDarkWaveMisa5Name[22];
extern unsigned char g_BattleSpecDarkWaveMisa7Name[22];
extern unsigned char g_BattleSpecDarkWaveMisa9Name[23];
extern unsigned char g_BattleSpecDarkWaveTimName1[23];
extern unsigned char g_BattleSpecDarkWaveTimName2[22];
extern unsigned char g_BattleSpecDarkWaveTimName3[22];
extern unsigned char g_BattleSpecDarkWaveTimName4[21];
extern int g_BattleSpecDarkWaveDrawListHead;
extern struct GfxTableCursor *g_BattleSpecDarkWaveLoadedResTable;
void __cdecl BattleFx_SpecialLoadResDarkWave(void);
void __cdecl BattleFx_SpecialEnterDarkWave(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterDarkWavePerTargetCallback(int targetActor, int sourceActor);
void __cdecl BattleFx_SpecialSpawnDarkWavePerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialDarkWaveTimelineTask(void);
void __cdecl BattleFx_SpecialDarkWaveScatteredAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialDarkWaveRadialBurstSpriteTask(void);
void __cdecl BattleFx_SpecialDarkWaveProjectileTask(void);

/* battle_fx_special_chain_explosion_effect.cpp */
extern int g_BattleSpecChainExplosionResourceDefs;
extern int g_BattleSpecChainExplosionExplosionHead;
extern int g_BattleSpecChainExplosionTargetMask;
extern int g_BattleSpecChainExplosionDeltaX;
extern int g_BattleSpecChainExplosionDeltaY;
extern int g_BattleSpecChainExplosionDeltaZ;
extern int g_BattleSpecChainExplosionNormalX;
extern int g_BattleSpecChainExplosionNormalY;
extern int g_BattleSpecChainExplosionNormalZ;
extern int g_BattleSpecChainExplosionStepX;
extern int g_BattleSpecChainExplosionStepY;
extern int g_BattleSpecChainExplosionStepZ;
extern struct GfxTableCursor *g_BattleSpecChainExplosionLoadedResTable;
void __cdecl BattleFx_SpecialLoadResChainExplosion(void);
void __cdecl BattleFx_SpecialEnterChainExplosion(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginChainExplosion(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialChainExplosionSetupTask(void);
void __cdecl BattleFx_SpecialChainExplosionTimelineTask(void);
void __cdecl BattleFx_SpecialChainExplosionConvergingSpritesTask(void);

/* battle_fx_special_full_explosion_effect.cpp */
extern int g_BattleSpecFullExplosionResourceDefs;
extern int g_BattleSpecFullExplosionSourceActor;
extern int g_BattleSpecFullExplosionTargetMask;
extern struct GfxTableCursor *g_BattleSpecFullExplosionLoadedResTable;
void __cdecl BattleFx_SpecialLoadResFullExplosion(void);
void __cdecl BattleFx_SpecialEnterFullExplosion(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginFullExplosion(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialFullExplosionSetupTask(void);
void __cdecl BattleFx_SpecialFullExplosionTimelineTask(void);
void __cdecl BattleFx_SpecialFullExplosionPooledSpriteEntry0Task(void);
void __cdecl BattleFx_SpecialFullExplosionPooledSpriteEntry1Task(void);

/* battle_fx_special_siren_song_effect.cpp */
void __cdecl BattleFx_SpecialLoadResSirenSong(void);
void __cdecl BattleFx_SpecialBeginSirenSong(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSirenSongSetupTask(void);
void __cdecl BattleFx_SpecialSirenSongTimelineTask(void);
void __cdecl BattleFx_SpecialSirenSongFadingSpriteTask(void);
void __cdecl BattleFx_SpecialEnterSirenSong(int targetMask, int actorIdx);

/* battle_fx_special_whirlwind_effect.cpp */
extern int g_BattleSpecWhirlwindResourceDefs;
extern int g_BattleSpecWhirlwindVariant;
extern int g_BattleSpecWhirlwindSourceActor;
extern int g_BattleSpecWhirlwindDeltaX;
extern int g_BattleSpecWhirlwindDeltaY;
extern int g_BattleSpecWhirlwindDeltaZ;
extern int g_BattleSpecWhirlwindNormalX;
extern int g_BattleSpecWhirlwindNormalY;
extern int g_BattleSpecWhirlwindNormalZ;
extern int g_BattleSpecWhirlwindStepX;
extern int g_BattleSpecWhirlwindStepY;
extern int g_BattleSpecWhirlwindStepZ;
extern struct GfxTableCursor *g_BattleSpecWhirlwindLoadedResTable;
void __cdecl BattleFx_SpecialLoadResWhirlwind(void);
void __cdecl BattleFx_SpecialEnterWhirlwindVar0(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterWhirlwindVar1(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginWhirlwind(int sourceActor, int actorIdx);
void __cdecl BattleFx_SpecialWhirlwindSetupTask(void);
void __cdecl BattleFx_SpecialWhirlwindTimelineTask(void);
void __cdecl BattleFx_SpecialWhirlwindOrbitingSpriteTask(void);
void __cdecl BattleFx_SpecialWhirlwindConvergingSpritesTask(void);

/* battle_fx_special_shady_breath.cpp */
extern int g_BattleSpecShadyBreathResourceDefs;
extern int g_BattleSpecShadyBreathTargetMask;
extern struct GfxTableCursor *g_BattleSpecShadyBreathLoadedResTable;
void __cdecl BattleFx_SpecialLoadResShadyBreath(void);
void __cdecl BattleFx_SpecialEnterShadyBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginShadyBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialShadyBreathSetupTask(void);
void __cdecl BattleFx_SpecialShadyBreathTimelineTask(void);
void __cdecl BattleFx_SpecialShadyBreathScatteredSpritesTask(void);

/* battle_fx_special_heat_blast_effect.cpp */
extern unsigned char g_BattleSpecHeatBlastResourceNames[];
void __cdecl BattleFx_SpecialLoadResHeatBlast(void);
void __cdecl BattleFx_SpecialEnterHeatBlast(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginHeatBlast(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialHeatBlastSpawnTimeline(void);
void __cdecl BattleFx_SpecialHeatBlastTimelineTask(void);
void __cdecl BattleFx_SpecialHeatBlastAnimatedSpriteTask(void);

/* battle_fx_special_magma_eruption_effect.cpp */
extern unsigned char g_BattleSpecMagmaEruptionResourceNames[];
void __cdecl BattleFx_SpecialLoadResMagmaEruption(void);
void __cdecl BattleFx_SpecialEnterMagmaEruption(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginMagmaEruption(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialMagmaEruptionTimelineTask(void);
void __cdecl BattleFx_SpecialMagmaEruptionParticleSpritesTask(void);
int __cdecl BattleFx_SpecialMagmaEruptionClaimSpriteRecord(void);

/* battle_fx_special_light_blast3_effect.cpp */
extern int g_BattleSpecLightBlast3ResourceDefs;
extern short g_BattleSpecLightBlast3SavedActorAlpha[10];
extern int g_BattleSpecLightBlast3PacketCursor;
extern struct GfxTableCursor *g_BattleSpecLightBlast3LoadedResTable;
extern short g_BattleSpecLightBlast3FlashIntensity;
extern short g_BattleSpecLightBlast3FlashStep;
void __cdecl BattleFx_SpecialLoadResLightBlast3(void);
void __cdecl BattleFx_SpecialEnterLightBlast3(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginLightBlast3(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnLightBlast3PerTarget(int actor, int actorIndex);
void __cdecl BattleFx_SpecialLightBlast3TimelineTask(void);
void __cdecl BattleFx_SpecialLightBlast3BeginWhiteFlashFadeIn(short duration);
void __cdecl BattleFx_SpecialLightBlast3WhiteFlashTask(void);
void __cdecl BattleFx_SpecialLightBlast3BeginWhiteFlashFadeOut(short duration);
void __cdecl BattleFx_SpecialLightBlast3AnimatedSpriteEntry0Task(void);
void __cdecl BattleFx_SpecialLightBlast3AnimatedSpriteEntry1Task(void);

/* battle_fx_special_light_blast2_effect.cpp */
extern int g_BattleSpecLightBlast2ResourceDefs;
extern short g_BattleSpecLightBlast2SavedActorAlpha[10];
extern int g_BattleSpecLightBlast2PacketCursor;
extern struct GfxTableCursor *g_BattleSpecLightBlast2LoadedResTable;
extern short g_BattleSpecLightBlast2FlashIntensity;
extern short g_BattleSpecLightBlast2FlashStep;
void __cdecl BattleFx_SpecialLoadResLightBlast2(void);
void __cdecl BattleFx_SpecialEnterLightBlast2(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginLightBlast2(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnLightBlast2PerTarget(int actor, int actorIndex);
void __cdecl BattleFx_SpecialLightBlast2TimelineTask(void);
void __cdecl BattleFx_SpecialLightBlast2BeginWhiteFlashFadeIn(short duration);
void __cdecl BattleFx_SpecialLightBlast2WhiteFlashTask(void);
void __cdecl BattleFx_SpecialLightBlast2BeginWhiteFlashFadeOut(short duration);
void __cdecl BattleFx_SpecialLightBlast2AnimatedSpriteEntry0Task(void);
void __cdecl BattleFx_SpecialLightBlast2AnimatedSpriteEntry1Task(void);

/* battle_fx_special_cold_air_effect.cpp */
extern unsigned char g_BattleSpecColdAirResNameWa[7];
extern unsigned char g_BattleSpecColdAirResNameNetu1Uchi1[22];
extern unsigned char g_BattleSpecColdAirResNameUchi2Uchi3[22];
extern unsigned char g_BattleSpecColdAirResNameUchi4Uchi5[22];
extern unsigned char g_BattleSpecColdAirResNameUchi6Uchi7[22];
extern unsigned char g_BattleSpecColdAirResNameUchi8Uchi9[22];
extern unsigned char g_BattleSpecColdAirResNameUchi10Isi1[21];
extern unsigned char g_BattleSpecColdAirResNameIsi2Isi3[21];
extern unsigned char g_BattleSpecColdAirResNameIsi4Netu03[24];
extern unsigned char g_BattleSpecColdAirResNameHahen1Kemu1[22];
extern unsigned char g_BattleSpecColdAirResNameNetu03Hikari[23];
extern unsigned char g_BattleSpecColdAirResNameNetu03Kage[21];
extern int g_BattleSpecColdAirPacketCursor;
extern short g_BattleSpecColdAirTargetMask;
extern int g_BattleSpecColdAirScaleVector1_X;
extern int g_BattleSpecColdAirScaleVector1_Y;
extern int g_BattleSpecColdAirScaleVector1_Z;
extern int g_BattleSpecColdAirScaleVector2_X;
extern int g_BattleSpecColdAirScaleVector2_Y;
extern int g_BattleSpecColdAirScaleVector2_Z;
extern int g_BattleSpecColdAirScaleVector3_X;
extern int g_BattleSpecColdAirScaleVector3_Y;
extern int g_BattleSpecColdAirScaleVector3_Z;
extern short g_BattleSpecColdAirScaleFactor;
extern short g_BattleSpecColdAirFlashIntensity;
extern struct GfxTableCursor *g_BattleSpecColdAirLoadedResTable;
void __cdecl BattleFx_SpecialLoadResColdAir(void);
void __cdecl BattleFx_SpecialEnterColdAir(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginColdAir(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialColdAirTimelineTask(void);
void __cdecl BattleFx_SpecialColdAirWhiteFlashPulseTask(void);
void __cdecl BattleFx_SpecialColdAirDrawExpandingModelsTask(void);
void __cdecl BattleFx_SpecialColdAirDrawSpinModelsEmitTask(void);
void __cdecl BattleFx_SpecialColdAirAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialColdAirRisingSpinningModelTask(void);
void __cdecl BattleFx_SpecialColdAirRisingFadeSpriteTask(void);

/* battle_fx_special_fire_shell.cpp */
extern int g_BattleSpecFireShellResourceDefs;
extern struct BattleSpecFireballSpriteReq1 g_BattleSpecFireShellSpriteReq1;
extern int g_BattleSpecFireShellScaleParam;
extern int g_BattleSpecFireShellPacketCursor;
extern struct GfxTableCursor *g_BattleSpecFireShellLoadedResTable;
void __cdecl BattleFx_SpecialLoadResFireShell(void);
void __cdecl BattleFx_SpecialEnterFireShell(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginFireShell(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnFireShellPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialFireShellTimelineTask(void);
void __cdecl BattleFx_SpecialFireShellVariantSpriteTask(void);
void __cdecl BattleFx_SpecialFireShellBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialFireShellProjectileTask(void);

/* battle_fx_special_fireball.cpp */
extern int g_BattleSpecFireballResourceDefs;
extern struct BattleSpecFireballSpriteReq1 g_BattleSpecFireballSpriteReq1;
extern unsigned char g_BattleSpecFireballModelNames[];
extern unsigned char g_BattleSpecFireballSpriteNames[];
extern int g_BattleSpecFireballPacketCursor;
extern struct GfxTableCursor *g_BattleSpecFireballLoadedResTable;
void __cdecl BattleFx_SpecialLoadResFireball(void);
void __cdecl BattleFx_SpecialEnterFireball(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginFireball(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnFireballPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialFireballTimelineTask(void);
void __cdecl BattleFx_SpecialFireballAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialFireballBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialFireballProjectileTask(void);

/* battle_fx_special_bomb_blast.cpp */
extern int g_BattleSpecBombBlastResourceDefs;
extern struct BattleSpecBombBlastSpriteReq1 g_BattleSpecBombBlastSpriteReq1;
extern int g_BattleSpecBombBlastPacketCursor;
extern int g_BattleSpecBombBlastScaleVectorX;
extern int g_BattleSpecBombBlastScaleVectorY;
extern int g_BattleSpecBombBlastScaleVectorZ;
extern struct GfxTableCursor *g_BattleSpecBombBlastLoadedResTable;
void __cdecl BattleFx_SpecialLoadResBombBlast(void);
void __cdecl BattleFx_SpecialEnterBombBlast(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginBombBlast(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBombBlastScreenRampTimerTask(void);
void __cdecl BattleFx_SpecialSpawnBombBlastPerTarget(int actor, int targetIndex);
void __cdecl BattleFx_SpecialBombBlastTimelineTask(void);
void __cdecl BattleFx_SpecialBombBlastAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialBombBlastGrowFadeSpriteTask(void);
void __cdecl BattleFx_SpecialBombBlastBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialBombBlastRadialSparkSpriteTask(void);
void __cdecl BattleFx_SpecialBombBlastGrowingSpinningModelTask(void);

/* battle_fx_special_icicle_drop.cpp */
extern int g_BattleSpecIcicleDropResourceDefs;
void __cdecl BattleFx_SpecialLoadResIcicleDrop(void);
void __cdecl BattleFx_SpecialEnterIcicleDrop(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginIcicleDrop(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialIcicleDropRampDownWhenIdleTask(void);
void __cdecl BattleFx_SpecialSpawnIcicleDropPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialIcicleDropTimelineTask(void);
void __cdecl BattleFx_SpecialIcicleDropFallingRotatingSpriteTask(void);
int __cdecl BattleFx_SpecialIcicleDropInstallRotatedScaledMatrix(void *point, void *rotation, short scale);
void __cdecl BattleFx_SpecialIcicleDropDriftAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialIcicleDropFallingModelTask(void);
void __cdecl BattleFx_SpecialIcicleDropExpandingModelTask(void);

/* battle_fx_special_double_laser.cpp */
extern int g_BattleSpecDoubleLaserResourceDefs;
extern short g_BattleSpecDoubleLaserBeamAngles[2];
extern unsigned char g_BattleSpecDoubleLaserFileNames[];
extern struct GfxTableCursor *g_BattleSpecDoubleLaserLoadedResTable;
void __cdecl BattleFx_SpecialLoadResDoubleLaser(void);
void __cdecl BattleFx_SpecialEnterDoubleLaser(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginDoubleLaser(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnDoubleLaserPerTarget(int actor, int targetIndex);
void __cdecl BattleFx_SpecialDoubleLaserTimelineTask(void);
void __cdecl BattleFx_SpecialDoubleLaserAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialDoubleLaserDrawBeamSpriteTask(void);

/* battle_fx_special_sabotage.cpp */
extern int g_BattleSpecSabotageResourceDefs;
extern struct GfxTableCursor *g_BattleSpecSabotageLoadedResTable;
void __cdecl BattleFx_SpecialLoadResSabotage(void);
void __cdecl BattleFx_SpecialEnterSabotage(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterSabotagePerTargetCallback(int actorMask, int targetIndex);
void __cdecl BattleFx_SpecialSpawnSabotagePerTarget(int actor, int targetIndex);
void __cdecl BattleFx_SpecialSabotageTimelineTask(void);
void __cdecl BattleFx_SpecialSabotageVariantSpriteTask(void);

/* battle_fx_special_sephiroth_shock.cpp */
extern struct BattleSpecSephirothShockSpriteReq1 g_BattleSpecSephirothShockSpriteReq1;
extern struct BattleSpecSephirothShockSpriteReq2 g_BattleSpecSephirothShockSpriteReq2;
extern struct BattleSpecSephirothShockSpriteReq3 g_BattleSpecSephirothShockSpriteReq3;
extern struct BattleSpecSephirothShockSpriteReq3 g_BattleSpecSephirothShockSpriteReq4;
extern struct BattleSpecSephirothShockSpriteReq5 g_BattleSpecSephirothShockSpriteReq5;
extern struct BattleSpecSephirothShockSpriteReq6 g_BattleSpecSephirothShockSpriteReq6;
extern struct BattleSpecSephirothShockSpriteReq5 g_BattleSpecSephirothShockSpriteReq7;
extern struct BattleSpecSephirothShockSpriteReq5 g_BattleSpecSephirothShockSpriteReq8;
extern struct BattleSpecSephirothShockSpriteReq9 g_BattleSpecSephirothShockSpriteReq9;
extern struct BattleSpecSephirothShockSpriteReq10 g_BattleSpecSephirothShockSpriteReq10;
extern struct BattleSpecSephirothShockSpriteReq11 g_BattleSpecSephirothShockSpriteReq11;
extern unsigned char g_BattleSpecSephirothShockResNameSephirothShockRsd[10];
extern int g_BattleSpecSephirothShockPacketCursor;
extern short g_BattleSpecSephirothShockTargetMask;
extern int g_BattleSpecSephirothShockScaleVector1_X;
extern int g_BattleSpecSephirothShockScaleVector1_Y;
extern int g_BattleSpecSephirothShockScaleVector1_Z;
extern int g_BattleSpecSephirothShockScaleVector2_X;
extern int g_BattleSpecSephirothShockScaleVector2_Y;
extern int g_BattleSpecSephirothShockScaleVector2_Z;
extern int g_BattleSpecSephirothShockScaleVector3_X;
extern int g_BattleSpecSephirothShockScaleVector3_Y;
extern int g_BattleSpecSephirothShockScaleVector3_Z;
extern struct GfxTableCursor *g_BattleSpecSephirothShockLoadedResTable;
extern int g_BattleSpecSephirothShockRotationVector;
void __cdecl BattleFx_SpecialLoadResSephirothShock(void);
void __cdecl BattleFx_SpecialEnterSephirothShock(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSephirothShockSpawnTimeline(int actorMask, int actorIndex);
void __cdecl BattleFx_SpecialSephirothShockTimelineTask(void);
void __cdecl BattleFx_SpecialSephirothShockDrawExpandingBurstTask(void);
void __cdecl BattleFx_SpecialSephirothShockAnimatedSpriteEntry15Task(void);
void __cdecl BattleFx_SpecialSephirothShockAnimatedSpriteEntry19Task(void);
void __cdecl BattleFx_SpecialSephirothShockExpandingModelTask(void);
void __cdecl BattleFx_SpecialSephirothShockExpandingBlackModelTask(void);
void __cdecl BattleFx_SpecialSephirothShockExpandingCyclingModelTask(void);
void __cdecl BattleFx_SpecialSephirothShockRisingSpinningModelTask(void);
void __cdecl BattleFx_SpecialSephirothShockRisingFadeSpriteTask(void);
void __cdecl BattleFx_SpecialSephirothShockBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialSephirothShockGrowingSpinningModelTask(void);

/* battle_fx_special_swamp_shoot.cpp */
void __cdecl BattleFx_SpecialLoadResSwampShoot(void);
void __cdecl BattleFx_SpecialEnterSwampShoot(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterSwampShootPerTargetCallback(int actorMask, int targetIndex);
void __cdecl BattleFx_SpecialSpawnSwampShootPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialSwampShootTimelineTask(void);
void __cdecl BattleFx_SpecialSwampShootAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialSwampShootBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialSwampShootTrailEmitterTask(void);
void __cdecl BattleFx_SpecialSwampShootAnimatedSpriteEntry4Task(void);
void __cdecl BattleFx_SpecialSwampShootFadingOrientedModelTask(void);

/* battle_fx_special_heat_wave.cpp */
extern struct BattleSpecHeatWaveModelReq1 g_BattleSpecHeatWaveModelReq1;
extern struct BattleSpecHeatWaveSpriteReq1 g_BattleSpecHeatWaveSpriteReq1;
extern struct BattleSpecHeatWaveSpriteReq2 g_BattleSpecHeatWaveSpriteReq2;
extern struct BattleSpecHeatWaveModelReq2 g_BattleSpecHeatWaveModelReq2;
extern struct BattleSpecHeatWaveSpriteReq3 g_BattleSpecHeatWaveSpriteReq3;
extern struct BattleSpecHeatWaveModelReq2 g_BattleSpecHeatWaveModelReq3;
extern struct BattleSpecHeatWaveSpriteReq1 g_BattleSpecHeatWaveSpriteReq4;
extern unsigned char g_BattleSpecHeatWaveResName_wa[7];
extern unsigned char g_BattleSpecHeatWaveResName_netu1[10];
extern int g_BattleSpecHeatWaveDrawHandle;
extern short g_BattleSpecHeatWaveTargetMask;
extern int g_BattleSpecHeatWaveScaleX;
extern int g_BattleSpecHeatWaveScaleY;
extern int g_BattleSpecHeatWaveScaleZ;
extern short g_BattleSpecHeatWaveFlashStep;
extern short g_BattleSpecHeatWaveFlashTimer;
extern struct GfxTableCursor *g_BattleSpecHeatWaveLoadedResTable;
void __cdecl BattleFx_SpecialLoadResHeatWave(void);
void __cdecl BattleFx_SpecialEnterHeatWave(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginHeatWave(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialHeatWaveTimelineTask(void);
void __cdecl BattleFx_SpecialHeatWaveWhiteFlashPulseTask(void);
void __cdecl BattleFx_SpecialHeatWaveDrawExpandingModelsTask(void);
void __cdecl BattleFx_SpecialHeatWaveDrawSpinModelsEmitTask(void);
void __cdecl BattleFx_SpecialHeatWaveAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialHeatWaveRisingSpinningModelTask(void);
void __cdecl BattleFx_SpecialHeatWaveRisingFadeSpriteTask(void);

/* battle_fx_special_magma.cpp */
void __cdecl BattleFx_SpecialLoadResMagma(void);
void __cdecl BattleFx_SpecialEnterMagma(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginMagma(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnMagmaPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialMagmaTimelineTask(void);
void __cdecl BattleFx_SpecialMagmaParticleSpritesTask(void);
int __cdecl BattleFx_SpecialMagmaClaimParticleRecord(void);

/* battle_fx_special_hot_springs.cpp */
void __cdecl BattleFx_SpecialLoadResHotSprings(void);
void __cdecl BattleFx_SpecialEnterHotSprings(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginHotSprings(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnHotSpringsPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialHotSpringsTimelineTask(void);
void __cdecl BattleFx_SpecialHotSpringsParticleSpritesTask(void);
int __cdecl BattleFx_SpecialHotSpringsClaimParticleRecord(void);

/* battle_fx_special_swift_bolt.cpp */
extern unsigned int g_BattleSpecSwiftBoltResourceDefs[65];
extern struct BattleSpecSwiftBoltSpriteReq1 g_BattleSpecSwiftBoltSpriteReq1;
extern unsigned char g_BattleSpecSwiftBoltAnimFrameIndices[16];
extern struct BattleSpecSwiftBoltSpriteReq2 g_BattleSpecSwiftBoltSpriteReq2;
extern struct BattleSpecSwiftBoltSpriteReq3 g_BattleSpecSwiftBoltSpriteReq3;
extern struct BattleSpecSwiftBoltSpriteReq4 g_BattleSpecSwiftBoltSpriteReq4;
extern struct BattleSpecSwiftBoltSpriteReq5 g_BattleSpecSwiftBoltSpriteReq5;
extern unsigned char g_BattleSpecSwiftBoltOffsets1[6];
extern struct GfxRotationAngles2 g_BattleSpecSwiftBoltOffsets2;
extern struct BattleSpecHellBomberModelReq3 g_BattleSpecSwiftBoltModelReq1;
extern struct BattleSpecHellBomberModelReq3 g_BattleSpecSwiftBoltModelReq2;
extern unsigned char g_BattleSpecSwiftBoltResName_geki1[10];
extern int g_BattleSpecSwiftBoltTargetBonePosXY;
extern short g_BattleSpecSwiftBoltTargetBonePosZ;
extern int g_BattleSpecSwiftBoltDrawListHead;
extern int g_BattleSpecSwiftBoltModelScaleX;
extern int g_BattleSpecSwiftBoltModelScaleY;
extern int g_BattleSpecSwiftBoltModelScaleZ;
extern struct GfxTableCursor *g_BattleSpecSwiftBoltModelTable;
void __cdecl BattleFx_SpecialLoadResSwiftBolt(void);
void __cdecl BattleFx_SpecialEnterSwiftBolt(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginSwiftBolt(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSwiftBoltRampDownWhenIdleTask(void);
void __cdecl BattleFx_SpecialSpawnSwiftBoltPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialSwiftBoltTimelineTask(void);
void __cdecl BattleFx_SpecialSwiftBoltAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialSwiftBoltGrowingFadeSpriteTask(void);
void __cdecl BattleFx_SpecialSwiftBoltRadialSparkSpriteTask(void);
void __cdecl BattleFx_SpecialSwiftBoltGrowingSpinningModelTask(void);
void __cdecl BattleFx_SpecialSwiftBoltBoltEmitterTask(void);
void __cdecl BattleFx_SpecialSwiftBoltBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialSwiftBoltDrawBoltSegmentTask(void);

/* battle_fx_special_hell_bubbles.cpp */
extern int g_BattleSpecHellBubblesResourceDefs;
extern int g_BattleSpecHellBubblesModelScale;
extern int g_BattleSpecHellBubblesModelStep;
extern short g_BattleSpecHellBubblesDrawListHead;
extern short g_BattleSpecHellBubblesTargetMask;
extern int g_BattleSpecHellBubblesPoolActiveFlag;
extern short g_BattleSpecHellBubblesParticleLoopIndex;
extern struct GfxTableCursor *g_BattleSpecHellBubblesLoadedResTable;
void __cdecl BattleFx_SpecialLoadResHellBubbles(void);
void __cdecl BattleFx_SpecialEnterHellBubbles(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterHellBubblesPerTargetCallback(int targetIndex, int sourceIndex);
void __cdecl BattleFx_SpecialSpawnHellBubblesPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialHellBubblesTimelineTask(void);
void __cdecl BattleFx_SpecialHellBubblesAnimatedSpriteEntry2Task(void);
void __cdecl BattleFx_SpecialHellBubblesQuadStripTask(void);
void __cdecl BattleFx_SpecialHellBubblesDrawPsxTexturedQuadStrip(short amplitude, short phase, short angleStep, short intensity);
void __cdecl BattleFx_SpecialHellBubblesParticleEmitterTask(void);
int __cdecl BattleFx_SpecialHellBubblesClaimParticleRecord(void);
void __cdecl BattleFx_SpecialHellBubblesFadingSpinModelTask(void);

/* battle_fx_special_geyser.cpp */
void __cdecl BattleFx_SpecialLoadResGeyser(void);
void __cdecl BattleFx_SpecialEnterGeyser(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterGeyserPerTargetCallback(int targetIndex, int sourceIndex);
void __cdecl BattleFx_SpecialSpawnGeyserPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialGeyserTimelineTask(void);
void __cdecl BattleFx_SpecialGeyserParticleSpritesTask(void);
int __cdecl BattleFx_SpecialGeyserClaimParticleRecord(void);

/* battle_fx_special_coin_toss.cpp */
void __cdecl BattleFx_SpecialLoadResCoinToss(void);
void __cdecl BattleFx_SpecialEnterCoinToss(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginCoinToss(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnCoinTossPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialCoinTossTimelineTask(void);
void __cdecl BattleFx_SpecialCoinTossBouncingSpriteTask(void);

/* battle_fx_special_smog.cpp */
extern unsigned char g_BattleSpecSmogQuadRingOffsets[];
extern unsigned char g_BattleSpecSmogResName_smoke[23];
extern unsigned char g_BattleSpecSmogResName_SmokePlume[22];
extern unsigned char g_BattleSpecSmogResName_Burst[23];
extern unsigned char g_BattleSpecSmogResName_hit1[23];
extern unsigned char g_BattleSpecSmogResName_elehit1[23];
extern unsigned char g_BattleSpecSmogResName_elehit2[23];
extern unsigned char g_BattleSpecSmogResName_elehit3[23];
extern unsigned char g_BattleSpecSmogResName_elehit4[23];
extern unsigned char g_BattleSpecSmogResName_SmokePlumeTim[10];
extern int g_BattleSpecSmogDrawListHead;
extern struct GfxTableCursor *g_BattleSpecSmogLoadedResTable;
void __cdecl BattleFx_SpecialLoadResSmog(void);
void __cdecl BattleFx_SpecialEnterSmog(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterSmogPerTargetCallback(int targetIndex, int sourceIndex);
void __cdecl BattleFx_SpecialSpawnSmogPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialSmogTimelineTask(void);
void __cdecl BattleFx_SpecialSmogAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialSmogDriftingRingTask(void);
void __cdecl BattleFx_SpecialSmogDriftingFadeSpriteTask(void);

/* battle_fx_special_big_wave.cpp */
void __cdecl BattleFx_SpecialLoadResBigWave(void);
void __cdecl BattleFx_SpecialEnterBigWave(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBigWaveSpawnTimeline(int actorMask, int sourceIndex);
void __cdecl BattleFx_SpecialBigWaveTimelineTask(void);
void __cdecl BattleFx_SpecialBigWaveAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialBigWaveWaveSurfaceTask(void);
void __cdecl BattleFx_SpecialBigWaveInvertUvBytePair(unsigned char *firstUvByte, unsigned char *secondUvByte);

/* battle_fx_special_pollen.cpp */
extern int g_BattleSpecPollenResourceDefs;
extern int g_BattleSpecPollenModelScale;
extern int g_BattleSpecPollenModelStep;
extern int g_BattleSpecPollenDrawListHead;
extern short g_BattleSpecPollenParticleLoopIndex;
extern struct GfxTableCursor *g_BattleSpecPollenLoadedResTable;
void __cdecl BattleFx_SpecialLoadResPollen(void);
void __cdecl BattleFx_SpecialEnterPollen(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnPollenPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialPollenTimelineTask(void);
void __cdecl BattleFx_SpecialPollenAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialPollenFadeInOutSpriteTask(void);
void __cdecl BattleFx_SpecialPollenParticleEmitterTask(void);
void __cdecl BattleFx_SpecialPollenFadingSpinModelTask(void);
void __cdecl BattleFx_SpecialEnterPollenAlt(int targetMask, int actorIdx);

/* battle_fx_special_war_cry.cpp */
void __cdecl BattleFx_SpecialLoadResWarCry(void);
void __cdecl BattleFx_SpecialEnterWarCry(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterWarCryPerTargetCallback(int targetMask, int actorId);
void __cdecl BattleFx_SpecialSpawnWarCryPerTarget(int actor, int actorId);
void __cdecl BattleFx_SpecialWarCryTimelineTask(void);
void __cdecl BattleFx_SpecialWarCryDriftingFadeSpriteTask(void);
void __cdecl BattleFx_SpecialWarCryGrowingOrientedModelTask(void);
void __cdecl BattleFx_SpecialWarCryExpandingModelTask(void);

/* battle_fx_special_search_scope.cpp */
void __cdecl BattleFx_SpecialLoadResSearchScope(void);
void __cdecl BattleFx_SpecialEnterSearchScope(short targetMask, int actorId);
void __cdecl BattleFx_SpecialSearchScopeTimelineTask(void);
void __cdecl BattleFx_SpecialSearchScopeDrawWeavingModelTask(void);
void __cdecl BattleFx_SpecialSearchScopeTwoStageAnimatedSpriteTask(void);

/* battle_fx_special_northern_cross.cpp */
extern unsigned char g_BattleSpecialNorthernCrossResFilenameNorthernCross[22];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameDomu[21];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameNorthe03[13];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameHosi[23];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameNorthe01A[13];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameElehit1[40];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameNorthe01B[13];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameElehit3[40];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameNorthe00[13];
extern unsigned char g_BattleSpecialNorthernCrossResFilenameBaku[10];
extern int g_BattleSpecialNorthernCrossCentroidVec;
extern short g_BattleSpecialNorthernCrossCentroidZ;
extern int g_BattleSpecialNorthernCrossDrawListHandle;
extern short g_BattleSpecialNorthernCrossTargetMask;
extern int g_BattleSpecialNorthernCrossScaleX;
extern int g_BattleSpecialNorthernCrossScaleY;
extern int g_BattleSpecialNorthernCrossScaleZ;
extern struct GfxTableCursor *g_BattleSpecialNorthernCrossLoadedRes;
void __cdecl BattleFx_SpecialLoadResNorthernCross(void);
void __cdecl BattleFx_SpecialEnterNorthernCross(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginNorthernCross(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialNorthernCrossTimelineTask(void);
void __cdecl BattleFx_SpecialNorthernCrossAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialNorthernCrossFallingSpiralSpriteTask(void);
void __cdecl BattleFx_SpecialNorthernCrossFallingSpinModelTask(void);
void __cdecl BattleFx_SpecialNorthernCrossExpandingSpinModelsTask(void);

/* battle_fx_special_southern_cross.cpp */
extern unsigned char g_BattleSpecialSouthernCrossResFilenameSouthernCross[22];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameDomu[21];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameSouthe03[13];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameHosi[23];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameSouthe01A[13];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameElehit1[40];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameSouthe01B[13];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameElehit3[40];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameSouthe00[13];
extern unsigned char g_BattleSpecialSouthernCrossResFilenameBaku[10];
extern int g_BattleSpecialSouthernCrossCentroidVec;
extern short g_BattleSpecialSouthernCrossCentroidZ;
extern int g_BattleSpecialSouthernCrossDrawListHandle;
extern short g_BattleSpecialSouthernCrossTargetMask;
extern int g_BattleSpecialSouthernCrossScaleX;
extern int g_BattleSpecialSouthernCrossScaleY;
extern int g_BattleSpecialSouthernCrossScaleZ;
extern struct GfxTableCursor *g_BattleSpecialSouthernCrossLoadedRes;
void __cdecl BattleFx_SpecialLoadResSouthernCross(void);
void __cdecl BattleFx_SpecialEnterSouthernCross(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginSouthernCross(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSouthernCrossTimelineTask(void);
void __cdecl BattleFx_SpecialSouthernCrossVariantSpriteTask(void);
void __cdecl BattleFx_SpecialSouthernCrossOrbitSpriteTask(void);
void __cdecl BattleFx_SpecialSouthernCrossFallingSpinModelTask(void);
void __cdecl BattleFx_SpecialSouthernCrossExpandingSpinModelsTask(void);

/* battle_fx_special_quasar.cpp */
extern unsigned int g_BattleSpecialQuasarResourceDefs[128];
extern int g_BattleSpecialQuasarAlignedScaleX;
extern int g_BattleSpecialQuasarAlignedScaleY;
extern int g_BattleSpecialQuasarAlignedScaleZ;
extern short g_BattleSpecialQuasarModelIndices1[8];
extern short g_BattleSpecialQuasarModelIndices2[8];
extern struct PsxMatrix g_BattleSpecialQuasarBeamBasis;
extern struct BattleSpecialEnergyOrbCentroidPos g_BattleSpecialQuasarCentroidVec;
extern short g_BattleSpecialQuasarCentroidZ;
extern int g_BattleSpecialQuasarDrawListHandle;
extern short g_BattleSpecialQuasarTargetMask;
extern int g_BattleSpecialQuasarExpandScaleX;
extern int g_BattleSpecialQuasarExpandScaleY;
extern int g_BattleSpecialQuasarExpandScaleZ;
extern int g_BattleSpecialQuasarSpinScaleX;
extern int g_BattleSpecialQuasarSpinScaleY;
extern int g_BattleSpecialQuasarSpinScaleZ;
extern int g_BattleSpecialQuasarCoreScaleX;
extern int g_BattleSpecialQuasarCoreScaleY;
extern int g_BattleSpecialQuasarCoreScaleZ;
extern short g_BattleSpecialQuasarGlowLevel;
extern short g_BattleSpecialQuasarBeamLength;
extern short g_BattleSpecialQuasarCoreFade;
extern struct GfxTableCursor *g_BattleSpecialQuasarModelTable;
extern short g_BattleSpecialQuasarPulseSpriteFrame;
void __cdecl BattleFx_SpecialLoadResQuasar(void);
void __cdecl BattleFx_SpecialEnterQuasar(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginQuasar(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialQuasarTimelineTask(void);
void __cdecl BattleFx_SpecialQuasarPulsingSpriteSpawnBeamTask(void);
void __cdecl BattleFx_SpecialQuasarBeamAlignedSpriteTask(void);
void __cdecl BattleFx_SpecialQuasarBeamCoreTask(void);
void __cdecl BattleFx_SpecialQuasarDriftingFadeSpriteTask(void);
void __cdecl BattleFx_SpecialQuasarBeamSpinModelTask(void);
void __cdecl BattleFx_SpecialQuasarExpandingSpinModelTask(void);
void __cdecl BattleFx_SpecialQuasarGrowingFadeSpriteTask(void);

/* battle_fx_special_snowstorm.cpp */
void __cdecl BattleFx_SpecialLoadResSnowstorm(void);
void __cdecl BattleFx_SpecialBeginSnowstorm(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnSnowstormPerTarget(int actor, int actorId);
void __cdecl BattleFx_SpecialSnowstormTimelineTask(void);
void __cdecl BattleFx_SpecialSnowstormAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialSnowstormFadeSpriteTask(void);
void __cdecl BattleFx_SpecialSnowstormParticleFieldTask(void);
void __cdecl BattleFx_SpecialSnowstormDrawParticleField(short intensity);
void __cdecl BattleFx_SpecialSnowstormShrinkingOrientedModelTask(void);
void __cdecl BattleFx_SpecialSnowstormExpandingRisingModelTask(void);

/* battle_fx_special_liquid_poison.cpp */
extern int g_BattleSpecialLiquidPoisonResourceDefs;
extern struct BattleSpecialLiquidPoisonColor1 g_BattleSpecialLiquidPoisonSpriteReq1;
extern int g_BattleSpecialLiquidPoisonDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialLiquidPoisonLoadedRes;
void __cdecl BattleFx_SpecialLoadResLiquidPoison(void);
void __cdecl BattleFx_SpecialEnterLiquidPoison(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnLiquidPoisonPerTarget(int actor, int actorId);
void __cdecl BattleFx_SpecialLiquidPoisonTimelineTask(void);
void __cdecl BattleFx_SpecialLiquidPoisonVariantSpriteTask(void);
void __cdecl BattleFx_SpecialLiquidPoisonStreamSpawnerTask(void);
void __cdecl BattleFx_SpecialLiquidPoisonDriftingAnimatedSpriteTask(void);

/* battle_fx_special_poison_breath.cpp */
extern int g_BattleSpecialPoisonBreathResourceDefs;
extern short g_BattleSpecialPoisonBreathCount;
extern struct BattleSpecialPoisonBreathColor1 g_BattleSpecialPoisonBreathColor1;
extern int g_BattleSpecialPoisonBreathDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialPoisonBreathLoadedRes;
void __cdecl BattleFx_SpecialLoadResPoisonBreath(void);
void __cdecl BattleFx_SpecialEnterPoisonBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnPoisonBreathPerTarget(int actor, int actorId);
void __cdecl BattleFx_SpecialPoisonBreathTimelineTask(void);
void __cdecl BattleFx_SpecialPoisonBreathVariantSpriteTask(void);
void __cdecl BattleFx_SpecialPoisonBreathEmitSpriteTask(void);
void __cdecl BattleFx_SpecialPoisonBreathDriftingAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialEnterPoisonBreathLoud(int targetMask, int actorIdx);

/* battle_fx_special_oil.cpp */
extern unsigned int g_BattleSpecialOilResourceDefs[60];
extern struct BattleSpecialOilColor g_BattleSpecialOilColor;
extern struct BattleSpecialOilSpriteIndices g_BattleSpecialOilSpriteIndices;
extern struct SVECTOR g_BattleSpecialOilRotation;
extern short g_BattleSpecialOilCounter;
extern struct BattleSpecialOilModelReq g_BattleSpecialOilModelReq;
extern int g_BattleSpecialOilModelIndices[8];
extern int g_BattleSpecialOilDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialOilLoadedRes;
extern short g_BattleSpecialOilTargetMask;
void __cdecl BattleFx_SpecialLoadResOil(void);
void __cdecl BattleFx_SpecialEnterOil(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginOil(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnOilPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialOilTimelineTask(void);
void __cdecl BattleFx_SpecialOilVariantSpriteTask(void);
void __cdecl BattleFx_SpecialOilAimedModelTask(void);
void __cdecl BattleFx_SpecialOilTargetModelTask(void);

/* battle_fx_special_bad_mouth.cpp */
extern unsigned char g_BattleSpecialBadMouthDataBlob[2822];
extern unsigned int g_BattleSpecialBadMouthResourceDefs[40];
extern struct BattleSpecialBadMouthColor1 g_BattleSpecialBadMouthColor1;
extern struct BattleSpecialFallingRocksColor2 g_BattleSpecialBadMouthColor2;
extern struct BattleSpecialBadMouthModelReq1 g_BattleSpecialBadMouthModelReq1;
extern int g_BattleSpecialBadMouthScale[3];
extern int g_BattleSpecialBadMouthModelIndices[5];
extern int g_BattleSpecialBadMouthDrawListHead;
extern struct GfxTableCursor *g_BattleSpecialBadMouthLoadedResTable;
extern short g_BattleSpecialBadMouthFragmentSpawnedFlag;
extern int g_BattleSpecialBadMouthGrowRotation;
extern int g_BattleSpecialBadMouthFragmentRotation;
void __cdecl BattleFx_SpecialLoadResBadMouth(void);
void __cdecl BattleFx_SpecialEnterBadMouth(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginBadMouth(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBadMouthInitFragmentMotion(void);
void __cdecl BattleFx_SpecialSpawnBadMouthPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialBadMouthTimelineTask(void);
void __cdecl BattleFx_SpecialBadMouthFixedAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialBadMouthRisingAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialBadMouthGrowingModelTask(void);
void __cdecl BattleFx_SpecialBadMouthSpinModelTask(void);
void __cdecl BattleFx_SpecialBadMouthFragmentedModelTask(void);
void __cdecl BattleFx_SpecialBadMouthStepBouncingFragments(void);

/* battle_fx_special_atomic_ray.cpp */
extern unsigned char g_BattleSpecialAtomicRayScaleConst[16];
void __cdecl BattleFx_SpecialLoadResAtomicRay(void);
void __cdecl BattleFx_SpecialEnterAtomicRay(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnAtomicRayPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialAtomicRayTimelineTask(void);
void __cdecl BattleFx_SpecialAtomicRayVariantSpriteTask(void);
void __cdecl BattleFx_SpecialAtomicRayStretchSpriteTask(void);
void __cdecl BattleFx_SpecialAtomicRayOrbitSpriteTask(void);
void __cdecl BattleFx_SpecialAtomicRayFlickerSpriteTask(void);
void __cdecl BattleFx_SpecialAtomicRayDrawGrowingModelsTask(void);
void __cdecl BattleFx_SpecialAtomicRayFrameTableSpriteTask(void);
void __cdecl BattleFx_SpecialAtomicRaySpinFadeModelTask(void);
void __cdecl BattleFx_SpecialAtomicRayLayeredModelsTask(void);
void __cdecl BattleFx_SpecialAtomicRayScaleUpModelTask(void);

/* battle_fx_special_cold_breath.cpp */
extern int g_BattleSpecialColdBreathResourceDefs;
extern short g_BattleSpecialColdBreathTargetMask;
extern short g_BattleSpecialColdBreathActorIdx;
extern int g_BattleSpecialColdBreathCentroidVec;
extern int g_BattleSpecialColdBreathCentroidZ;
extern int g_BattleSpecialColdBreathScale1;
extern int g_BattleSpecialColdBreathScale2;
extern short g_BattleSpecialColdBreathCounter;
extern struct GfxTableCursor *g_BattleSpecialColdBreathLoadedRes;
void __cdecl BattleFx_SpecialLoadResColdBreath(void);
void __cdecl BattleFx_SpecialEnterColdBreath(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnColdBreathPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialColdBreathTimelineTask(void);
void __cdecl BattleFx_SpecialColdBreathAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialColdBreathParticleFieldTask(void);
int __cdecl BattleFx_SpecialColdBreathClaimParticleRecord(void);
void __cdecl BattleFx_SpecialColdBreathSpinStretchModelTask(void);
void __cdecl BattleFx_SpecialColdBreathRisingGrowingModelTask(void);

/* battle_fx_special_falling_rocks.cpp */
extern struct BattleFxSpriteRequest g_BattleSpecialFallingRocksDebrisSpriteReq;
extern struct BattleFxSpriteRequest g_BattleSpecialFallingRocksHitSpriteReq;
extern struct BattleSpecialFallingRocksModelReq1 g_BattleSpecialFallingRocksFragmentModelReq;
extern struct BattleFxSpriteRequest g_BattleSpecialFallingRocksShadowSpriteReq;
extern unsigned char g_BattleSpecialFallingRocksResFilenameIsia[9];
extern int g_BattleSpecialFallingRocksDrawListHandle;
extern int g_BattleSpecialFallingRocksModelOffset;
extern struct GfxTableCursor *g_BattleSpecialFallingRocksLoadedRes;
void __cdecl BattleFx_SpecialLoadResFallingRocks(void);
void __cdecl BattleFx_SpecialEnterFallingRocks(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginFallingRocks(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnFallingRocksPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialFallingRocksTimelineTask(void);
void __cdecl BattleFx_SpecialFallingRocksAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialFallingRocksFallingModelTask(void);
void __cdecl BattleFx_SpecialFallingRocksSpawnBouncingSprites(struct BattleFxFallingRocksFallingModelSpawnSlot *origin, short value);
void __cdecl BattleFx_SpecialFallingRocksBouncingSpriteTask(void);
int __cdecl BattleFx_SpecialFallingRocksInstallScaledSpriteMatrix(void *point, short *rotation, short scale);
void __cdecl BattleFx_SpecialFallingRocksSplittingSpinModelTask(void);
void __cdecl BattleFx_SpecialFallingRocksBouncingFragmentModelTask(void);
void __cdecl BattleFx_SpecialFallingRocksCameraShakeTask(void);

/* battle_fx_special_harrier.cpp */
extern int g_BattleSpecialHarrierResourceDefs;
extern int g_BattleSpecialHarrierScale1;
extern struct BattleLimitStardustRayModelReq2 g_BattleSpecialHarrierSpriteReq2;
extern short g_BattleSpecialHarrierScaleByVariant[3];
extern int g_BattleSpecialHarrierRotXYZ;
extern struct BattleSpecialHarrierLookMatrix g_BattleSpecialHarrierLookMatrix;
extern short g_BattleSpecialHarrierRandomAngle;
extern int g_BattleSpecialHarrierDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialHarrierLoadedRes;
void __cdecl BattleFx_SpecialLoadResHarrier(void);
void __cdecl BattleFx_SpecialEnterHarrier(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginHarrier(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnHarrierPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialHarrierTimelineTask(void);
void __cdecl BattleFx_SpecialHarrierAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialHarrierVariantSpriteTask(void);
void __cdecl BattleFx_SpecialHarrierDriftModelsTask(void);

/* battle_fx_special_water_gun.cpp */
extern int g_BattleSpecialWaterGunResourceDefs;
extern struct BattleLimitHowlingMoonSpriteRequest g_BattleSpecialWaterGunSpriteReq;
extern int g_BattleSpecialWaterGunRotXYZ;
extern short g_BattleSpecialWaterGunScaleStep;
extern int g_BattleSpecialWaterGunDrawListHandle;
extern int g_BattleSpecialWaterGunScale;
extern int g_BattleSpecialWaterGunModelScaleZ;
extern int g_BattleSpecialWaterGunAlphaFactor;
extern struct GfxTableCursor *g_BattleSpecialWaterGunLoadedRes;
void __cdecl BattleFx_SpecialLoadResWaterGun(void);
void __cdecl BattleFx_SpecialEnterWaterGun(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginWaterGun(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnWaterGunPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialWaterGunTimelineTask(void);
void __cdecl BattleFx_SpecialWaterGunVariantSpriteTask(void);
void __cdecl BattleFx_SpecialWaterGunStretchModelTask(void);

/* battle_fx_special_triangle_attack.cpp */
extern struct BattleSpecialTriangleAttackSpriteReq1 g_BattleSpecialTriangleAttackSpriteReq1;
extern struct BattleSpecialTriangleAttackModelReq1 g_BattleSpecialTriangleAttackModelReq1;
extern struct BattleSpecialTriangleAttackModelReq2 g_BattleSpecialTriangleAttackModelReq2;
extern struct BattleSpecialTriangleAttackModelReq1 g_BattleSpecialTriangleAttackModelReq3;
extern struct BattleSpecialTriangleAttackModelReq1 g_BattleSpecialTriangleAttackModelReq4;
extern struct BattleSpecialTriangleAttackSpriteReq2 g_BattleSpecialTriangleAttackSpriteReq2;
extern struct BattleSpecialTriangleAttackModelReq2 g_BattleSpecialTriangleAttackModelReq5;
extern struct BattleSpecialTriangleAttackSpriteReq3 g_BattleSpecialTriangleAttackSpriteReq3;
extern unsigned char g_BattleSpecialTriangleAttackResFilenameEletri1[12];
extern int g_BattleSpecialTriangleAttackDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialTriangleAttackLoadedRes;
extern short g_BattleSpecialTriangleAttackAlphaParam1;
extern short g_BattleSpecialTriangleAttackAlphaParam2;
void __cdecl BattleFx_SpecialEnterTriangleAttack(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialLoadResTriangleAttack(void);
void __cdecl BattleFx_SpecialBeginTriangleAttack(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnTriangleAttackPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialTriangleAttackTimelineTask(void);
void __cdecl BattleFx_SpecialTriangleAttackVariantSpriteTask(void);
void __cdecl BattleFx_SpecialTriangleAttackGrowingModelTask(void);
void __cdecl BattleFx_SpecialTriangleAttackSpinModelsTask(void);
void __cdecl BattleFx_SpecialTriangleAttackCounterSpinModelsTask(void);
void __cdecl BattleFx_SpecialTriangleAttackPulsingSpriteSpawnTask(void);
void __cdecl BattleFx_SpecialTriangleAttackRotatedSpriteTask(void);

/* battle_fx_special_toxic_smog.cpp */
extern int g_BattleSpecialToxicSmogResourceDefs;
extern int g_BattleSpecialToxicSmogScale1;
void __cdecl BattleFx_SpecialLoadResToxicSmog(void);
void __cdecl BattleFx_SpecialEnterToxicSmog(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginToxicSmog(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialToxicSmogTimelineTask(void);
void __cdecl BattleFx_SpecialToxicSmogAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialToxicSmogSecondaryAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialToxicSmogSpinFadeSpriteTask(void);
int __cdecl BattleFx_SpecialToxicSmogInstallRotatedScaledMatrix(struct Vec3sPacked *translation, void *rotation, short scale);
void __cdecl BattleFx_SpecialToxicSmogDriftingRibbonSpriteTask(void);
void __cdecl BattleFx_SpecialToxicSmogSpinGrowFadeModelTask(void);

/* battle_fx_special_energy_siphon.cpp */
extern int g_BattleSpecialEnergySiphonResourceDefs;
extern int g_BattleSpecialEnergySiphonScale1;
extern int g_BattleSpecialEnergySiphonDrawListHandle;
extern short g_BattleSpecialEnergySiphonState;
extern struct GfxTableCursor *g_BattleSpecialEnergySiphonLoadedRes;
void __cdecl BattleFx_SpecialLoadResEnergySiphon(void);
void __cdecl BattleFx_SpecialEnterEnergySiphon(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnEnergySiphonPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialEnergySiphonTimelineTask(void);
void __cdecl BattleFx_SpecialEnergySiphonAnimatedSpriteEntry4Task(void);
void __cdecl BattleFx_SpecialEnergySiphonSpinModelTask(void);
void __cdecl BattleFx_SpecialEnergySiphonSpiralSpriteTask(void);
void __cdecl BattleFx_SpecialEnergySiphonOrbitSpriteTask(void);
void __cdecl BattleFx_SpecialEnergySiphonVariantLoopingSpriteTask(void);
void __cdecl BattleFx_SpecialEnergySiphonAnimatedSpriteEntry1Task(void);

/* battle_fx_special_self_destruct.cpp */
void __cdecl BattleFx_SpecialLoadResSelfDestruct(void);
void __cdecl BattleFx_SpecialEnterSelfDestruct(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnSelfDestructPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialSelfDestructTimelineTask(void);
void __cdecl BattleFx_SpecialSelfDestructAnimatedSpriteTask(void);

/* battle_fx_special_sham_seal.cpp */
extern unsigned char g_BattleSpecialShamSealScale1[8];
void __cdecl BattleFx_SpecialLoadResShamSeal(void);
void __cdecl BattleFx_SpecialEnterShamSeal(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnShamSealPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialShamSealTimelineTask(void);
void __cdecl BattleFx_SpecialShamSealVariantSpriteTask(void);
void __cdecl BattleFx_SpecialShamSealAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialShamSealOrbitModelTask(void);

/* battle_fx_special_evil_poison.cpp */
void __cdecl BattleFx_SpecialLoadResEvilPoison(void);
void __cdecl BattleFx_SpecialEnterEvilPoison(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEvilPoisonTimelineTask(void);
void __cdecl BattleFx_SpecialEvilPoisonPulseEmitterTask(void);
void __cdecl BattleFx_SpecialEvilPoisonVariantSpriteTask(void);
void __cdecl BattleFx_SpecialEvilPoisonDriftingSpriteTask(void);
void __cdecl BattleFx_SpecialEnterEvilPoisonAlt(int targetMask, int actorIdx);

/* battle_fx_special_drain.cpp */
extern int g_BattleSpecialDrainResourceDefs;
extern int g_BattleSpecialDrainScale1;
extern int g_BattleSpecialDrainDrawListHandle;
extern short g_BattleSpecialDrainState;
extern struct GfxTableCursor *g_BattleSpecialDrainLoadedRes;
void __cdecl BattleFx_SpecialLoadResDrain(void);
void __cdecl BattleFx_SpecialEnterDrain(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnDrainPerTarget(int actor, int attackerId);
void __cdecl BattleFx_SpecialDrainTimelineTask(void);
void __cdecl BattleFx_SpecialDrainAnimatedSpriteEntry4Task(void);
void __cdecl BattleFx_SpecialDrainSpinModelTask(void);
void __cdecl BattleFx_SpecialDrainSpiralSpriteTask(void);
void __cdecl BattleFx_SpecialDrainOrbitingSpriteTask(void);
void __cdecl BattleFx_SpecialDrainRisingLoopingSpriteTask(void);
void __cdecl BattleFx_SpecialDrainAnimatedSpriteEntry1Task(void);

/* battle_fx_special_flying_sickle.cpp */
extern unsigned char g_BattleSpecialFlyingSickleScale1[8];
void __cdecl BattleFx_SpecialLoadResFlyingSickle(void);
void __cdecl BattleFx_SpecialEnterFlyingSickle(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginFlyingSickle(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnFlyingSicklePerTarget(int actor, int attackerId);
void __cdecl BattleFx_SpecialFlyingSickleTimelineTask(void);
void __cdecl BattleFx_SpecialFlyingSickleVariantSpriteTask(void);
void __cdecl BattleFx_SpecialFlyingSickleRisingAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialFlyingSickleOrbitModelTask(void);

/* battle_fx_special_rocket_launcher.cpp */
extern int g_BattleSpecialRocketLauncherResourceDefs;
extern int g_BattleSpecialRocketLauncherDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialRocketLauncherLoadedRes;
void __cdecl BattleFx_SpecialLoadResRocketLauncher(void);
void __cdecl BattleFx_SpecialEnterRocketLauncher(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginRocketLauncher(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnRocketLauncherPerTarget(int actor, int attackerId);
void __cdecl BattleFx_SpecialRocketLauncherTimelineTask(void);
void __cdecl BattleFx_SpecialRocketLauncherAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialRocketLauncherTask(void);

/* battle_fx_special_blaster.cpp */
extern unsigned char g_BattleSpecialBlasterResFilenameWa[7];
extern unsigned char g_BattleSpecialBlasterResFilenameSenkou1[32];
extern unsigned char g_BattleSpecialBlasterResFilenameTama1[22];
extern unsigned char g_BattleSpecialBlasterResFilenameMoto[21];
extern unsigned char g_BattleSpecialBlasterResFilenameNetu1[23];
extern unsigned char g_BattleSpecialBlasterResFilenameUchi1[22];
extern unsigned char g_BattleSpecialBlasterResFilenameUchi3[22];
extern unsigned char g_BattleSpecialBlasterResFilenameUchi5[22];
extern unsigned char g_BattleSpecialBlasterResFilenameUchi7[22];
extern unsigned char g_BattleSpecialBlasterResFilenameHikari1[24];
extern unsigned char g_BattleSpecialBlasterResFilenameBlaste03[13];
extern unsigned char g_BattleSpecialBlasterResFilenameKou[8];
extern unsigned char g_BattleSpecialBlasterResFilenameBlaste03b[13];
extern unsigned char g_BattleSpecialBlasterResFilenameBaku2[38];
extern short g_BattleSpecialBlasterState;
void __cdecl BattleFx_SpecialLoadResBlaster(void);
void __cdecl BattleFx_SpecialEnterBlaster(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginBlaster(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBlasterTimelineTask(void);
void __cdecl BattleFx_SpecialBlasterVariantSpriteTask(void);
void __cdecl BattleFx_SpecialBlasterShrinkingModelTask(void);
void __cdecl BattleFx_SpecialBlasterDrawSpinModelsTask(void);
void __cdecl BattleFx_SpecialBlasterFinalFlashModelsTask(void);
void __cdecl BattleFx_SpecialBlasterStretchModelsTask(void);
void __cdecl BattleFx_SpecialBlasterForwardBurstTask(void);
void __cdecl BattleFx_SpecialBlasterTrailModelTask(void);
void __cdecl BattleFx_SpecialBlasterPulseModelsTask(void);
void __cdecl BattleFx_SpecialBlasterGrowModelTask(void);

/* battle_fx_special_fascination.cpp */
extern unsigned int g_BattleSpecialFascinationResourceDefs[65];
extern struct BattleSpecialFascinationColor1 g_BattleSpecialFascinationColor1;
extern struct BattleSpecialFascinationColor2 g_BattleSpecialFascinationColor2;
extern unsigned char g_BattleSpecialFascinationSpriteIndices1[16];
extern struct BattleSpecialFascinationColor3 g_BattleSpecialFascinationColor3;
extern unsigned char g_BattleSpecialFascinationSpriteIndices2[16];
extern struct BattleSpecialFascinationModelReq g_BattleSpecialFascinationModelReq;
extern int g_BattleSpecialFascinationDrawListHandle;
extern short g_BattleSpecialFascinationAngle;
extern struct GfxTableCursor *g_BattleSpecialFascinationLoadedRes;
extern short g_BattleSpecialFascinationSpin;
void __cdecl BattleFx_SpecialLoadResFascination(void);
void __cdecl BattleFx_SpecialEnterFascination(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginFascination(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialFascinationWaitRampEndTask(void);
void __cdecl BattleFx_SpecialSpawnFascinationPerTarget(int actor, int attackerId);
void __cdecl BattleFx_SpecialFascinationTimelineTask(void);
void __cdecl BattleFx_SpecialFascinationDampedSpriteTask(void);
void __cdecl BattleFx_SpecialFascinationTask(void);
void __cdecl BattleFx_SpecialFascinationSineSpriteTask(void);
void __cdecl BattleFx_SpecialFascinationRisingVariantSpriteTask(void);

/* battle_fx_special_four_laser.cpp */
extern int g_BattleSpecialFourLaserResourceDefs;
extern int g_BattleSpecialFourLaserDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialFourLaserLoadedRes;
void __cdecl BattleFx_SpecialLoadResFourLaser(void);
void __cdecl BattleFx_SpecialEnterFourLaser(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginFourLaser(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnFourLaserPerTarget(int actor, int attackerId);
void __cdecl BattleFx_SpecialFourLaserQuadRecordsTask(void);
void __cdecl BattleFx_SpecialFourLaserDrawQuadRecords(void);
void __cdecl BattleFx_SpecialFourLaserTimelineTask(void);
void __cdecl BattleFx_SpecialFourLaserDirectedSpriteTask(void);
void __cdecl BattleFx_SpecialFourLaserAnimatedSpriteEntry1Task(void);
void __cdecl BattleFx_SpecialFourLaserOrbitTrailTask(void);
void __cdecl BattleFx_SpecialFourLaserGetOrbitOffsetPoint(struct BattleFxSpiderWebOrbit *orbit, struct Vec3sPacked *outPoint);

/* battle_fx_special_stare_down.cpp */
extern int g_BattleSpecialStareDownResourceDefs;
extern short g_BattleSpecialStareDownSavedTargetStates[10];
extern int g_BattleSpecialStareDownDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialStareDownLoadedRes;
void __cdecl BattleFx_SpecialLoadResStareDown(void);
void __cdecl BattleFx_SpecialEnterStareDown(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnStareDownPerTarget(int actor, int attackerId);
void __cdecl BattleFx_SpecialStareDownTimelineTask(void);
void __cdecl BattleFx_SpecialStareDownAnimatedSpriteEntry0Task(void);
void __cdecl BattleFx_SpecialStareDownAnimatedSpriteEntry1Task(void);

/* battle_fx_special_spider_web.cpp */
extern int g_BattleSpecialSpiderWebResourceDefs;
extern int g_BattleSpecialSpiderWebLineVertices;
extern int g_BattleSpecialSpiderWebDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialSpiderWebLoadedRes;
void __cdecl BattleFx_SpecialLoadResSpiderWeb(void);
void __cdecl BattleFx_SpecialEnterSpiderWeb(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginSpiderWeb(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnSpiderWebPerTarget(int actor, int actorIndex);
void __cdecl BattleFx_SpecialSpiderWebLineRecordsTask(void);
void __cdecl BattleFx_SpecialSpiderWebDrawLineRecords(short trailWidth);
void __cdecl BattleFx_SpecialSpiderWebTimelineTask(void);
void __cdecl BattleFx_SpecialSpiderWebLoopingSpriteTask(void);
void __cdecl BattleFx_SpecialSpiderWebVariantSpriteTask(void);
void __cdecl BattleFx_SpecialSpiderWebOrbitTrailTask(void);
void __cdecl BattleFx_SpecialSpiderWebGetOrbitOffsetPoint(struct BattleFxSpiderWebOrbit *orbit, struct Vec3sPacked *outPoint);

/* battle_fx_special_sandstorm.cpp */
extern int g_BattleSpecialSandstormResourceDefs;
extern int g_BattleSpecialSandstormDrawListHandle;
extern short g_BattleSpecialSandstormTargetMask;
extern int g_BattleSpecialSandstormScaleVector;
extern int g_BattleSpecialSandstormScaleFactor;
extern int g_BattleSpecialSandstormScale;
extern short g_BattleSpecialSandstormStep;
extern struct GfxTableCursor *g_BattleSpecialSandstormLoadedRes;
void __cdecl BattleFx_SpecialLoadResSandstorm(void);
void __cdecl BattleFx_SpecialSandstormNoOp(void);
void __cdecl BattleFx_SpecialEnterSandstorm(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSandstormTimelineTask(void);
void __cdecl BattleFx_SpecialSandstormVariantSpriteTask(void);
void __cdecl BattleFx_SpecialSandstormParticleFieldTask(void);
struct BattleSpecialSandstormParticleRecord *__cdecl BattleFx_SpecialSandstormClaimParticleRecord(void);
void __cdecl BattleFx_SpecialSandstormSpinGrowModelTask(void);

/* battle_fx_special_autumn_leaves.cpp */
extern int g_BattleSpecialAutumnLeavesResourceDefs;
extern int g_BattleSpecialAutumnLeavesDrawListHandle;
extern int g_BattleSpecialAutumnLeavesTargetMask;
extern struct GfxTableCursor *g_BattleSpecialAutumnLeavesLoadedRes;
extern short g_BattleSpecialAutumnLeavesStep;
void __cdecl BattleFx_SpecialLoadResAutumnLeaves(void);
void __cdecl BattleFx_SpecialEnterAutumnLeaves(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginAutumnLeaves(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialAutumnLeavesTimelineTask(void);
void __cdecl BattleFx_SpecialAutumnLeavesLeafModelTask(void);

/* battle_fx_special_silver_wheel.cpp */
void __cdecl BattleFx_SpecialLoadResSilverWheel(void);
void __cdecl BattleFx_SpecialEnterSilverWheel(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnSilverWheelPerTarget(int actor, int actorIndex);
void __cdecl BattleFx_SpecialSilverWheelTimelineTask(void);
void __cdecl BattleFx_SpecialSilverWheelAnimatedSpriteEntry7Task(void);
void __cdecl BattleFx_SpecialSilverWheelVariantSpriteTask(void);
void __cdecl BattleFx_SpecialSilverWheelTravelModelTask(void);

/* battle_fx_special_refuse_spit.cpp */
void __cdecl BattleFx_SpecialLoadResRefuseSpit(void);
void __cdecl BattleFx_SpecialEnterRefuseSpit(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnRefuseSpitPerTarget(int actor, int actorIndex);
void __cdecl BattleFx_SpecialRefuseSpitTimelineTask(void);
void __cdecl BattleFx_SpecialRefuseSpitAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialRefuseSpitDriftSpriteTask(void);

/* battle_fx_special_beam_gun.cpp */
extern int g_BattleSpecialBeamGunResourceDefs;
extern int g_BattleSpecialBeamGunRotationVector1;
extern int g_BattleSpecialBeamGunRotationVector2;
extern struct BattleSpecialBeamGunBasisMatrix g_BattleSpecialBeamGunBasisMatrix;
extern int g_BattleSpecialBeamGunDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialBeamGunLoadedRes;
void __cdecl BattleFx_SpecialLoadResBeamGun(void);
void __cdecl BattleFx_SpecialEnterBeamGun(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterBeamGunPerTargetCallback(int targetMask, int actorIndex);
void __cdecl BattleFx_SpecialSpawnBeamGunPerTarget(int actor, int actorIndex);
void __cdecl BattleFx_SpecialBeamGunTimelineTask(void);
void __cdecl BattleFx_SpecialBeamGunAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialBeamGunOneShotSpriteTask(void);
void __cdecl BattleFx_SpecialBeamGunDriftModelTask(void);

/* battle_fx_special_seed_shot.cpp */
extern int g_BattleSpecialSeedShotResourceDefs;
extern int g_BattleSpecialSeedShotDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialSeedShotLoadedRes;
void __cdecl BattleFx_SpecialLoadResSeedShot(void);
void __cdecl BattleFx_SpecialEnterSeedShot(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnSeedShotPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialSeedShotTimelineTask(void);
void __cdecl BattleFx_SpecialSeedShotScatteredSpriteTask(void);
void __cdecl BattleFx_SpecialSeedShotBounceModelTask(void);

/* battle_fx_special_thousand_needles.cpp */
extern int g_BattleSpecialThousandNeedlesResourceDefs;
extern int g_BattleSpecialThousandNeedlesScaleVector;
extern int g_BattleSpecialThousandNeedlesDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialThousandNeedlesLoadedRes;
void __cdecl BattleFx_SpecialLoadResThousandNeedles(void);
void __cdecl BattleFx_SpecialEnterThousandNeedles(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnThousandNeedlesPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialThousandNeedlesTimelineTask(void);
void __cdecl BattleFx_SpecialThousandNeedlesScatteredSpriteTask(void);
void __cdecl BattleFx_SpecialThousandNeedlesScatterModelTask(void);

/* battle_fx_special_searchlight.cpp */
extern int g_BattleSpecialSearchlightResourceDefs;
extern short g_BattleSpecialSearchlightDeltaVector[3];
extern int g_BattleSpecialSearchlightDrawListHandle;
extern struct GfxTableCursor *g_BattleSpecialSearchlightLoadedRes;
extern short g_BattleSpecialSearchlightStep;
void __cdecl BattleFx_SpecialLoadResSearchlight(void);
void __cdecl BattleFx_SpecialEnterSearchlight(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSearchlightTimelineTask(void);
void __cdecl BattleFx_SpecialSearchlightSweepModelTask(void);
void __cdecl BattleFx_SpecialSearchlightTrailSpriteTask(void);

/* battle_fx_special_vacuum.cpp */
extern int g_BattleSpecVacuumResourceDef;
void __cdecl BattleFx_SpecialLoadResVacuum(void);
void __cdecl BattleFx_SpecialEnterVacuum(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginVacuum(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnVacuumPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialVacuumTimelineTask(void);
int __cdecl BattleFx_SpecialVacuumDrawLiveSegmentRecords(void);
void __cdecl BattleFx_SpecialVacuumDecelSpriteTask(void);
void __cdecl BattleFx_SpecialVacuumDecelSegmentSpawnerTask(void);
void __cdecl BattleFx_SpecialVacuumJitterModelTask(void);

/* battle_fx_special_ultrasound.cpp */
extern int g_BattleSpecUltrasoundResourceDef;
extern int g_BattleSpecUltrasoundRotation;
extern int g_BattleSpecUltrasoundPacketCursor;
extern struct GfxTableCursor *g_BattleSpecUltrasoundLoadedRes;
void __cdecl BattleFx_SpecialLoadResUltrasound(void);
void __cdecl BattleFx_SpecialEnterUltrasound(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginUltrasound(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnUltrasoundPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_SpecialUltrasoundTimelineTask(void);
void __cdecl BattleFx_SpecialUltrasoundSpriteTask(void);
void __cdecl BattleFx_SpecialUltrasoundGrowingModelTask(void);

/* battle_fx_special_time_damage_warp.cpp */
void __cdecl BattleFx_SpecialLoadResTimeDamage(void);
void __cdecl BattleFx_SpecialEnterTimeDamage(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginTimeDamage(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialTimeDamageBuildWarpGridMesh(void);
void __cdecl BattleFx_SpecialTimeDamageTimelineTask(void);
void __cdecl BattleFx_SpecialTimeDamageWarpGridTask(void);
void __cdecl BattleFx_SpecialTimeDamageDrawWarpGrid(int deltaX, int deltaY);
void __cdecl BattleFx_SpecialTimeDamageOrbitingSpriteRingTask(void);
void __cdecl BattleFx_SpecialTimeDamageRotatingSpritePairTask(void);
void __cdecl BattleFx_SpecialTimeDamageProjectActorPointTask(void);
void __cdecl BattleFx_SpecialTimeDamageProjectClampedPoint(void *point3d, short *screenPoint);

/* battle_fx_special_whirlpool.cpp */
extern int g_BattleSpecWhirlpoolResourceDefs;
extern unsigned char g_BattleSpecWhirlpoolConfig[8];
void __cdecl BattleFx_SpecialLoadResWhirlpool(void);
void __cdecl BattleFx_SpecialEnterWhirlpool(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRegisterWhirlpoolPerTargetCallback(int actorIndex, int secondIndex);
void __cdecl BattleFx_SpecialSpawnWhirlpoolPerTarget(int actor, int secondIndex);
void __cdecl BattleFx_SpecialWhirlpoolTimelineTask(void);
int __cdecl BattleFx_SpecialWhirlpoolClaimSpiralTrailParticleRecord(void);
int __cdecl BattleFx_SpecialWhirlpoolClaimSecondaryParticleRecord(void);
void __cdecl BattleFx_SpecialWhirlpoolSpiralTrailParticleArrayTask(void);
void __cdecl BattleFx_SpecialWhirlpoolSecondaryParticleArrayTask(void);
void __cdecl BattleFx_SpecialWhirlpoolGrowingModelTask(void);

/* battle_fx_special_laser_cannon.cpp */
extern int g_BattleSpecLaserCannon_ResourceDefs;
extern struct GfxTableCursor *g_BattleSpecLaserCannon_LoadedRes;
void __cdecl BattleFx_SpecialLoadResLaserCannon(void);
void __cdecl BattleFx_SpecialEnterLaserCannon(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnLaserCannonPerTarget(int actor, int secondIndex);
void __cdecl BattleFx_SpecialLaserCannonTimelineTask(void);
void __cdecl BattleFx_SpecialLaserCannonRibbonTask(void);
void __cdecl BattleFx_SpecialLaserCannonCyclingSpriteTask(void);
void __cdecl BattleFx_SpecialLaserCannonSequentialSpriteTask(void);

/* battle_fx_special_smoke_shot.cpp */
void __cdecl BattleFx_SpecialLoadResSmokeShot(void);
void __cdecl BattleFx_SpecialEnterSmokeShot(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnSmokeShotPerTarget(int actor, int sourceIndex);
void __cdecl BattleFx_SpecialSmokeShotProjectileTask(void);
void __cdecl BattleFx_SpecialSmokeShotAnimatedSpriteTask(void);

/* battle_fx_special_gust.cpp */
extern short g_BattleFxGustColorScale;
extern short g_BattleFxGustAngleSteps[12];
extern struct BattleFxGustFourColorPacket *g_BattleFxGustResourcePtr;
extern short g_BattleFxGustActiveFlag;
void __cdecl BattleFx_SpecialEnterBuiltinGust(unsigned int targetMask, int actingActor);
void BattleFx_SpecialBeginBuiltinGust(unsigned int targetMask, int actingActor);
void __cdecl BattleFx_SpecialSpawnBuiltinGustPerTarget(int actor);
void BattleFx_SpecialBuiltinGustTimelineTask(void);
void BattleFx_SpecialBuiltinGustDrawActiveSegments(void);
void __cdecl BattleFx_SpecialBuiltinGustSwirlTask(void);

/* battle_fx_summon_neo_bahamut.cpp */
extern int g_BattleSummonNeoBahamutSoundTable;
extern unsigned char g_BattleSummonNeoBahamutPolygonVertexBlob[9080];
extern int g_BattleSummonNeoBahamutResourceDef;
extern struct BattleFxNeoBahamutResIds g_BattleNeoBahamutPrimaryResourceIds;
extern short g_BattleSummonNeoBahamutAnchorPosition[4];
extern int g_BattleNeoBahamutField4OffscreenSlot;
extern int g_BattleNeoBahamutField12OffscreenSlot;
extern unsigned char g_BattleSummonNeoBahamutCameraScript1[344];
extern unsigned char g_BattleSummonNeoBahamutCameraScript2[352];
extern struct BattleLimitStardustRayModelReq2 g_NeoBahamutThunderSpriteDrawRequest1;
extern struct BattleLimitStardustRayModelReq2 g_NeoBahamutThunderSpriteDrawRequest2;
extern struct BattleFxNeoBahamutModelReq g_BattleNeoBahamutParticleDrawRequest;
extern unsigned char g_BattleSummonNeoBahamutCloud1Matrix[32];
extern unsigned char g_BattleSummonNeoBahamutCloud2Prim[8];
extern int g_BattleSummonNeoBahamutBackSkyShadeColor1;
extern int g_BattleSummonNeoBahamutBackSkyShadeColor2;
extern unsigned char g_BattleSummonNeoBahamutBackSkyVertex1[8];
extern struct SVECTOR g_BattleSummonNeoBahamutBackSkyVertex2;
extern unsigned char g_BattleSummonNeoBahamutBackSkyVertex3[16];
extern struct BattleFxNeoBahamutSceneActor *g_BattleSummonNeoBahamutSceneActorRef;
extern struct BattleSummonCloudSlot *g_BattleSummonNeoBahamutCloud2TaskSlot;
extern struct Vec3sPacked *g_BattleSummonNeoBahamutActorRiseAndFallTaskSlot;
extern char g_BattleSummonNeoBahamutFragmentUpdateInitFlag;
extern struct BattleFxDebrisSpawnTask *g_BattleSummonNeoBahamutRock2SpawnTaskSlot;
extern char g_BattleSummonNeoBahamutRock3InitFlag;
extern struct BattleFxNeoBahamutBackSkyState *g_BattleSummonNeoBahamutBackSkyTaskSlot;
extern int g_BattleSummonNeoBahamutLineLight2State;
extern struct PsxMatrix g_BattleSummonNeoBahamutComposedMatrix;
extern short g_BattleSummonNeoBahamutLineLight2Rotation[4];
extern short g_BattleSummonNeoBahamutLineLight2Offset[4];
extern short g_BattleSummonNeoBahamutLineLight2Step[7];
extern struct BattleFxNeoBahamutCloud1Slot *g_BattleSummonNeoBahamutCloud1TaskSlot;
extern struct BattleFxNeoBahamutShardMatrixBuf g_BattleSummonNeoBahamutGroundVertexBlob[651];
extern struct BattleFxNeoBahamutRock2State *g_BattleSummonNeoBahamutRock2TaskSlot;
extern struct BattleFxNeoBahamutCloudSea *g_BattleSummonNeoBahamutCloudSeaTaskSlot;
extern struct BattleFxNeoBahamutLineLight1Slot *g_BattleSummonNeoBahamutLineLight1TaskSlot;
extern char g_BattleSummonNeoBahamutRock2InitFlag;
extern unsigned char g_BattleSummonNeoBahamutLightRigInitFlag;
extern struct PsxMatrix g_BattleSummonNeoBahamutLineLightMatrix;
extern int g_BattleSummonNeoBahamutTargetMask;
extern struct BattleFxShardMeshRotation *g_BattleSummonNeoBahamutRock3TaskSlot;
extern char g_BattleSummonNeoBahamutBackSkyInitFlag;
extern char g_BattleSummonNeoBahamutBreathChildInitFlag;
extern char g_BattleSummonNeoBahamutCloudSeaInitFlag;
extern struct BattleFxNeoBahamutFaceSlot *g_BattleSummonNeoBahamutPsxMeshTaskSlot;
extern char g_BattleSummonNeoBahamutCloud2InitFlag;
extern char g_BattleSummonNeoBahamutRock1SpawnInitFlag;
extern struct BattleFxNeoBahamutLineLight3Slot *g_BattleSummonNeoBahamutLineLight3TaskSlot;
extern struct BattleFxNeoBahamutLightRig *g_BattleSummonNeoBahamutLightRigTaskSlot;
extern struct PsxMatrix *g_BattleSummonNeoBahamutFragmentMatrixTaskSlot;
extern struct BattleFxShardMeshRotation *g_BattleSummonNeoBahamutRock1TaskSlot;
extern struct BattleFxLineLight2Task *g_BattleSummonNeoBahamutLineLight2TaskSlot;
extern char g_BattleSummonNeoBahamutActorRiseAndFallInitFlag;
extern int g_BattleSummonNeoBahamutCircleModelAngle;
extern char g_BattleSummonNeoBahamutPsxMeshInitFlag;
extern char g_BattleSummonNeoBahamutCloud1InitFlag;
extern char g_BattleSummonNeoBahamutLineLight1InitFlag;
extern struct GfxTableCursor *g_BattleSummonNeoBahamutLoadedResTable;
extern int g_BattleSummonNeoBahamutCloud2ModelDrawHandle;
extern int g_BattleSummonNeoBahamutBackSkyPrimIndex;
void __cdecl BattleFx_SummonLoadResNeoBahamut(void);
int __cdecl BattleFx_SummonEnterNeoBahamut(int targetMask, int actorIdx);

/* battle_fx_summon_bahamut.cpp */
extern unsigned char g_BattleSummonBahamutSoundGroup1[16];
extern unsigned char g_BattleSummonBahamutSoundGroup2[16];
extern unsigned char g_BattleSummonBahamutSoundGroup3[16];
extern unsigned char g_BattleSummonBahamutSoundGroup4[16];
extern struct BattleFxBillboardVector g_BattleSummonBahamutModelOffset;
extern struct BattleFxBahamutDrawPair g_BattleSummonBahamutModelDrawRequest1;
extern short g_BattleSummonBahamutDefaultPosition[4];
extern struct PsxMatrix g_BattleSummonBahamutTransformMatrix;
extern struct PsxMatrix g_BattleSummonBahamutComposedMatrix;
extern struct GfxRotationAngles2 g_BattleSummonBahamutScaleVector;
void __cdecl BattleFx_SummonLoadResBahamut(void);
int __cdecl BattleFx_SummonEnterBahamut(int targetMask, int actorIdx);

/* battle_fx_summon_odin_gungnir.cpp */
extern unsigned char g_BattleSummonOdinGSoundGroup0[16];
extern unsigned char g_BattleSummonOdinGSoundGroup1[16];
extern unsigned char g_BattleSummonOdinGSoundGroup2[16];
extern unsigned char g_BattleSummonOdinGSoundGroup3[16];
extern unsigned char g_BattleSummonOdinGSoundGroup4[16];
extern int g_BattleSummonOdinGVertexDeltasData1;
extern int g_BattleSummonOdinGVertexDeltasData2;
extern struct BattleSummonOdinGAnimDataAC8 g_BattleSummonOdinGParticleSprite;
extern struct BattleSummonOdinGAnimDataAF8 g_BattleSummonOdinGFadingSprite;
extern struct PsxMatrix g_BattleSummonOdinGBillboardMatrix;
extern struct BattleSpecMizufukiSpriteRequest2 g_BattleSummonOdinGAttachmentSprite;
extern struct PsxMatrix g_BattleSummonOdinGParticleMatrix;
extern struct BattleSpecMizufukiSpriteRequest2 g_BattleSummonOdinGDebrisSprite;
extern struct PsxMatrix g_BattleSummonOdinGDebrisMatrix;
extern struct BattleSummonOdinGAnimDataB88 g_BattleSummonOdinGSpinningModelA;
extern struct BattleSummonOdinGAnimDataB88 g_BattleSummonOdinGSpinningModelB;
extern struct PsxMatrix g_BattleSummonOdinGRightMatrix;
extern struct BattleSummonOdinGAnimDataB88 g_BattleSummonOdinGGrowingModel;
extern struct BattleSummonOdinGAnimDataB88 g_BattleSummonOdinGPulsingModel;
extern struct BattleSummonOdinGAnimDataCC0 g_BattleSummonOdinGAnimatedSprite;
extern struct BattleSummonOdinGAnimDataD10 g_BattleSummonOdinGTaskTransformMatrix;
extern short g_BattleSummonOdinGVectorD40[4];
extern unsigned char g_BattleSummonOdinGParticleRecordPool[9998];
extern struct PsxMatrix g_BattleSummonOdinGMatrixD530;
void __cdecl BattleFx_SummonLoadResOdinGungnir(void);
int __cdecl BattleFx_SummonEnterOdinGungnir(int targetMask, int actorIdx);

/* battle_fx_summon_odin_zantetsuken.cpp */
extern unsigned char g_BattleSummonOdinZMesh[932];
extern unsigned int g_BattleSummonOdinZResourceDef[25];
extern struct BattleFxEulerAngles16 g_BattleSummonOdinZInitialAngles;
extern short g_BattleSummonOdinZSubeffectTable1[124];
extern short g_BattleSummonOdinZSubeffectTable2[124];
extern int g_BattleSummonOdinZScale;
extern struct BattleSummonOdinZSpriteDrawRequest g_BattleSummonOdinZSpriteDrawRequest;
extern struct BattleFxModelDrawHead g_BattleSummonOdinZAnimParams1;
extern struct PsxMatrix g_BattleSummonOdinZMatrix1;
extern int g_BattleSummonOdinZState;
extern struct BattleSummonOdinZTaskParam g_BattleSummonOdinZTaskParam;
extern struct BattleActor *g_BattleSummonOdinZCameraTarget;
extern struct PsxMatrix g_BattleSummonOdinZMatrix2;
extern struct BattleFxModelDrawHead g_BattleSummonOdinZAnimParams2;
extern struct BattleFxPsxMeshHeader g_BattleSummonOdinZMeshDef;
void __cdecl BattleFx_SummonLoadResOdinZantetsuken(void);
int __cdecl BattleFx_SummonEnterOdinZantetsuken(int targetMask, int actorIdx);
extern short g_BattleSummonOdinZTimer1;
extern unsigned char g_BattleSummonOdinZSpriteReq1[14];
extern struct BattleFxOdinTrailSegment *g_BattleSummonOdinZTrailDataBlock;
extern unsigned char g_BattleSummonOdinZTrackBuffer[116];
extern struct BattleFxOdinZTargetScratch *g_pBattleSummonOdinZTargetScratch;
extern struct BattleFxOdinZCameraScript g_BattleSummonOdinZCameraScriptTable;
extern struct BattleFxOdinZSplitPacket g_BattleSummonOdinZMatrixBuffer[1][2];
extern struct BattleSummonOdinZSplitHalf g_BattleSummonOdinZAnimState[2];
extern unsigned char g_BattleSummonOdinZFlag1;
extern unsigned char g_BattleSummonOdinZFlag2;
extern int g_BattleSummonOdinZStateWord;
extern int g_BattleSummonOdinZSplitSpeed;
extern int g_BattleSummonOdinZBlendFactorW;
extern int g_BattleSummonOdinZSplitScratch;
extern int g_BattleSummonOdinZBlendFactorZ;
extern int g_BattleSummonOdinZVar98;
extern unsigned char g_BattleSummonOdinZByteA0;
extern int g_BattleSummonOdinZWordB0;
extern int g_BattleSummonOdinZSplitColor;
extern int g_BattleSummonOdinZSplitColorStep;
extern struct PsxMatrix g_BattleSummonOdinZMatrixE0;
extern int g_BattleSummonOdinZSplitOffsetB;
extern struct BattleFxOdinZMotionScratch *g_pBattleSummonOdinZMotionScratch;
extern unsigned char g_BattleSummonOdinZModelMatrixReady;
extern short g_BattleSummonOdinZFrames;
extern char g_BattleSummonOdinZRecordBuffer2[10];
extern int g_BattleSummonOdinZTimelineScratch;
extern int g_BattleSummonOdinZSplitOffsetA;
extern struct PsxMatrix *g_pBattleSummonOdinZModelMatrix;
extern int g_BattleSummonOdinZSplitScale;
extern unsigned char g_BattleSummonOdinZByteBC;
extern short g_BattleSummonOdinZCameraEyeZ;
extern unsigned char g_BattleSummonOdinZByteC8;
extern struct PsxMatrix *g_pBattleSummonOdinZAfterimageMatrix;
extern struct GfxTableCursor *g_BattleSummonOdinZLoadedResTable;
extern struct PsxMatrix g_BattleSummonOdinZMatrixD8;
extern short g_BattleSummonOdinZAnglesF8[3];
extern short g_BattleSummonOdinZAngles00[3];
extern int g_BattleSummonOdinZDrawListTaskSlot;
extern int g_BattleSummonOdinZDrawBufferCurrent;

/* battle_fx_summon_hades.cpp */
extern struct BattleSummonHadesMeshBlob g_BattleSummonHadesMeshBlob;
extern struct BattleSummonHadesSpriteReq3 g_BattleSummonHadesSpriteReq3;
extern int g_BattleSummonHadesFadingPairEndFrame;
extern struct BattleSummonHadesRadialData g_BattleSummonHadesRadialData;
extern struct BattleSummonHadesSpriteReq3 g_BattleSummonHadesPairSprite;
extern struct CVECTOR g_BattleSummonHadesTubeFullColor;
extern struct CVECTOR g_BattleSummonHadesTubeShadeSource;
void __cdecl BattleFx_SummonLoadResHades(void);
int __cdecl BattleFx_SummonEnterHades(int targetMask, int actorIdx);

/* battle_fx_typhon_summon.cpp */
extern unsigned char g_BattleSummonTyphonAnimTable1[];
extern unsigned char g_BattleSummonTyphonAnimTable2[];
extern unsigned char g_BattleSummonTyphonAnimTable3[];
extern unsigned char g_BattleSummonTyphonModelMesh0[];
extern unsigned char g_BattleSummonTyphonModelMesh1[];
extern unsigned char g_BattleSummonTyphonModelMesh2[];
extern int g_BattleSummonTyphonCameraScriptHeader;
extern unsigned char g_BattleSummonTyphonResEyeSkyRsd[22];
extern unsigned char g_BattleSummonTyphonResWindRsd[23];
extern unsigned char g_BattleSummonTyphonResSmoke1TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke2TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke3TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke4TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke5TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke6TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke7TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke8TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke9TimSpt[24];
extern unsigned char g_BattleSummonTyphonResSmoke10Tim[13];
extern unsigned char g_BattleSummonTyphonResSmoke10Spt[13];
extern unsigned char g_BattleSummonTyphonResSmoke11Tim[13];
extern unsigned char g_BattleSummonTyphonResSmoke11Spt[13];
extern unsigned char g_BattleSummonTyphonResSmoke12Tim[13];
extern unsigned char g_BattleSummonTyphonResSmoke12Spt[13];
extern unsigned char g_BattleSummonTyphonResSmoke13Tim[13];
extern unsigned char g_BattleSummonTyphonResSmoke13Spt[13];
extern unsigned char g_BattleSummonTyphonResSmoke14Tim[13];
extern unsigned char g_BattleSummonTyphonResSmoke14Spt[13];
extern unsigned char g_BattleSummonTyphonResSmoke15Tim[13];
extern unsigned char g_BattleSummonTyphonResSmoke15Spt[13];
extern unsigned char g_BattleSummonTyphonResSmoke16Tim[13];
extern unsigned char g_BattleSummonTyphonResSmoke16Spt[13];
extern unsigned char g_BattleSummonTyphonResSmokeATimSpt[24];
extern unsigned char g_BattleSummonTyphonResTupon1rTim[24];
extern unsigned char g_BattleSummonTyphonResTupon2rTim[24];
void __cdecl BattleFx_SummonLoadResTyphon(void);
int __cdecl BattleFx_SummonEnterTyphon(int targetMask, int actorIdx);

/* battle_fx_kjata_summon.cpp */
extern struct BattleSummonKjataVertexData1 g_BattleSummonKjataVertexData1;
extern struct BattleSummonKjataSpriteRequest1 g_BattleSummonKjataSpriteRequest1;
extern struct BattleSummonKjataModelDrawRequest1 g_BattleSummonKjataModelDrawRequest1;
extern int g_BattleSummonKjataLightFactor;
extern int g_BattleSummonKjataLightIntensity;
extern char g_BattleSummonKjataTimelineInitFlag;
extern struct BattleFxKjataTrailQuad *g_BattleSummonKjataTrailQuad;
extern int g_BattleSummonKjataWaveBank;
extern char g_BattleSummonKjataCameraFlag2;
extern struct BattleFxKjataBlackGrowState *g_BattleSummonKjataTempVector;
extern char g_BattleSummonKjataCameraFlag7;
extern struct BattleFxPosition3s *g_BattleSummonKjataPosition;
extern struct PsxMatrix g_BattleSummonKjataLightMatrix;
extern int g_BattleSummonKjataTrailNodeCount;
extern int g_BattleSummonKjataLoadedResTable2;
void __cdecl BattleFx_SummonLoadResKjata(void);
int __cdecl BattleFx_SummonEnterKjata(int targetMask, int actorIdx);
void __cdecl BattleFx_SummonKjataSpawnScalingModelsTask(void);
void __cdecl BattleFx_SummonKjataScalingModelTask(void);
void __cdecl BattleFx_SummonKjataSpawnScalingFadeModelsTask(void);
void __cdecl BattleFx_SummonKjataScalingFadeModelTask(void);
void __cdecl BattleFx_SummonKjataRingGridWaveTask(void);
void __cdecl BattleFx_SummonKjataBuildAndQueueStageOffscreenRequest(void);
void __cdecl BattleFx_SummonKjataQueueStageOffscreenRequest(int matrix, struct BattleFxSummonViewport *viewport);
void __cdecl BattleFx_SummonKjataBuildRingGridMesh(void);
void __cdecl BattleFx_SummonKjataScrollWaveHeightHistory(int height);
void __cdecl BattleFx_SummonSpawnKjataWaveHeightPerTarget(int actor);
void __cdecl BattleFx_SummonKjataTargetWaveHeightTask(void);
void __cdecl BattleFx_SummonBeginKjataJitterEnvelope(int amplitude, int duration, int holdFrames, int fadeStep);
void __cdecl BattleFx_SummonKjataJitterEnvelopeTask(void);
void __cdecl BattleFx_SummonKjataScreenFlashPulseTask(void);

/* battle_fx_alexander_summon.cpp */
extern int g_BattleSummonAlexanderResourceTable;
extern unsigned char g_BattleSummonAlexanderVertexBlob1[21400];
extern unsigned char g_BattleSummonAlexanderVertexBlob2[21396];
extern unsigned char g_BattleSummonAlexanderPoseFrame1[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame2[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame3[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame4[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame5[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame6[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame7[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame8[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame9[7094];
extern unsigned char g_BattleSummonAlexanderPoseFrame10[7094];
extern unsigned int g_BattleSummonAlexanderResourceDefs[75];
extern int g_BattleSummonAlexanderOffscreenSlot1;
extern int g_BattleSummonAlexanderOffscreenSlot2;
extern int g_BattleSummonAlexanderOffscreenSlot3;
extern int g_BattleSummonAlexanderOffscreenSlot4;
extern unsigned char g_BattleSummonAlexanderCameraScript1[288];
extern unsigned char g_BattleSummonAlexanderCameraScript2[308];
extern struct PsxMatrix g_BattleSummonAlexanderInitialMatrix;
extern unsigned int g_BattleSummonAlexanderPoseTable[12];
extern int g_BattleSummonAlexanderResourceSubTable;
#define g_BattleSummonAlexanderResourceSubTablePtrs (&g_BattleSummonAlexanderResourceSubTable)
extern struct BattleSummonAlexanderDefaultPosition g_BattleSummonAlexanderDefaultPosition;
extern short g_BattleSummonAlexanderScaleFixed12;
extern short g_BattleSummonAlexanderScaleFactor;
extern int g_BattleSummonAlexanderBaseDistance;
extern int g_BattleSummonAlexanderResourceSubTable2;
#define g_BattleSummonAlexanderResourceSubTable2Ptrs (&g_BattleSummonAlexanderResourceSubTable2)
void __cdecl BattleFx_SummonLoadResAlexander(void);
void __cdecl BattleFx_SummonBeginAlexander(int targetMask, int actorIdx);
void __cdecl BattleFx_AlexanderSpawnCameraScript(void *script, void *matrix, int actorIndex);
void __cdecl BattleFx_AlexanderRunCameraScript(void);
short __cdecl BattleFx_AlexanderReadCameraScriptOperand(short **cursorPtr);
void __cdecl BattleFx_SummonAlexanderClearCallbackRecords(void);
void __cdecl BattleFx_SummonAlexanderRunCallbackRecordsTask(void);
void __cdecl BattleFx_SummonAlexanderTimelineTask(void);
void __cdecl BattleFx_SummonAlexanderCameraYThenZShakeTask(void);
void __cdecl BattleFx_SummonAlexanderBuildAndQueueStageOffscreenRequests(void);
void __cdecl BattleFx_SummonAlexanderQueueStageOffscreenRequests(int matrix, struct BattleFxOffscreenCaptureReq *params);
void __cdecl BattleFx_SummonAlexanderMorphingMeshTask(void);
void __cdecl BattleFx_SummonAlexanderComputeMorphDeltas(void *target, struct Vec3sPacked *base, struct SVECTOR *deltas);
void __cdecl BattleFx_SummonAlexanderApplyMorphBlend(void *dst, struct Vec3sPacked *base, struct SVECTOR *deltas, int blend);
void __cdecl BattleFx_SummonAlexanderSpawnSwirlingSpritesTask(void);
int __cdecl BattleFx_SummonAlexanderClaimSpriteRecord(int tag);
void __cdecl BattleFx_SummonAlexanderSwirlingSpriteRecordTask(struct BattleFxSwirlingSprite *sprite);
void __cdecl BattleFx_SummonAlexanderDrawAnimatedSprite(void *point, int frameIndex, int scale);
int __cdecl BattleFx_SummonAlexanderInstallSpriteMatrix(void *point, short value, int distance);
void __cdecl BattleFx_SummonAlexanderSpawnShardsTask(void);
void __cdecl BattleFx_SummonAlexanderShardMeshTask(void);
void __cdecl BattleFx_SummonAlexanderSpawnYStepAnimatedSpritesTask(void);
void __cdecl BattleFx_SummonAlexanderYStepAnimatedSpriteRecordTask(struct BattleFxRisingSpriteParticle *record);
void __cdecl BattleFx_SummonAlexanderCameraDecayShakeTask(void);
void __cdecl BattleFx_SummonAlexanderDampedDriftSpriteTask(void);
void __cdecl BattleFx_SummonAlexanderSpawnRadialSparkSpritesTask(void);
void __cdecl BattleFx_SummonAlexanderRadialSparkSpriteRecordTask(struct BattleFxRadialLivewire *record);
void __cdecl BattleFx_SummonAlexanderSpawnPrimitivePairsTask(void);
void __cdecl BattleFx_SummonAlexanderGradientTriangleTask(void);
int __cdecl BattleFx_SummonAlexanderGradientToPackedColor(int hue);
void __cdecl BattleFx_SummonAlexanderGradientStripQuadsTask(void);
void __cdecl BattleFx_SummonAlexanderScalingModelTask(void);
void __cdecl BattleFx_SummonAlexanderSpawnScalingMeshesTask(void);
void __cdecl BattleFx_SummonAlexanderScalingMeshTask(void);
void __cdecl BattleFx_SummonAlexanderGrowingModelTask(void);
void __cdecl BattleFx_SummonAlexanderAnimatedSpriteTask(void);
void __cdecl BattleFx_SummonAlexanderScatteredShardMeshTask(void);
void __cdecl BattleFx_SummonAlexanderFlatTrianglesTask(void);
void __cdecl BattleFx_SummonAlexanderEaseOffsetsTask(void);
extern unsigned char g_BattleFxSummonAlexanderSpawnShardsTaskInit;
extern unsigned char g_BattleFxSummonAlexanderMorphingMeshTaskInit;
extern void (__cdecl *g_BattleSummonAlexanderVertexBuffer[200])(int);
extern unsigned char g_BattleFxSummonAlexanderSpawnRadialSparkSpritesTaskInit;
extern int g_BattleSummonAlexanderShakeAmplitude;
extern unsigned char g_BattleSummonAlexanderInterpolatedPose[7094];
extern unsigned char g_BattleFxSummonAlexanderSpawnSwirlingSpritesTaskInit;
extern char g_BattleSummonAlexanderDescentInitFlag;
extern struct BattleFxAlexanderMatrixEntry *g_BattleSummonAlexanderComposedMatrix;
extern int g_BattleSummonAlexanderDrawHandle;
extern int g_BattleSummonAlexanderBlendTimer;
extern unsigned char g_BattleFxSummonAlexanderShardMeshTaskInit;
extern struct BattleFxAlexanderShardScratch *g_BattleSummonAlexanderScatterRotation;
extern int g_BattleSummonAlexanderActorIdx;
extern struct BattleFxCameraOrbitState g_BattleSummonAlexanderTargetActor;
extern unsigned char g_BattleFxSummonAlexanderGrowingModelTaskInit;
extern unsigned char g_BattleFxSummonAlexanderGradientTriangleTaskInit;
extern int g_BattleSummonAlexanderTimer;
extern unsigned char g_BattleFxSummonAlexanderGradientStripQuadsTaskInit;
extern short g_BattleSummonAlexanderRotation[3];
extern struct BattleFxAlexanderGradient *g_pBattleSummonAlexanderStripPacket;
extern int g_BattleSummonAlexanderTargetActorIdx;
extern int g_BattleSummonAlexanderTargetParam1;
extern struct PsxMatrix *g_pBattleSummonAlexanderGrowingModelMatrix;
extern short g_BattleSummonAlexanderPosDelta[3];
extern struct BattleFxActorBoneMatrixBuf g_BattleSummonAlexanderBillboardMatrix;
extern struct BattleFxCameraOrbitState *g_BattleSummonAlexanderCameraState;
extern struct BattleFxTimelineState *g_BattleSummonAlexanderBeamScratch;
extern struct BattleFxCameraBasisScratch *g_BattleSummonAlexanderCameraScratch;
extern struct BattleFxAlexanderShardScratch *g_BattleSummonAlexanderShardRotation;
extern struct PsxMatrix g_BattleSummonAlexanderLightMatrix;
extern struct BattleFxShardQuad *g_BattleSummonAlexanderStripQuad;
extern unsigned char g_BattleFxSummonAlexanderTimelineTaskInit;
extern int g_BattleSummonAlexanderShakeOffset;
extern struct BattleFxPsxGouraudTriPacket *g_pBattleSummonAlexanderGradientTriPacket;
extern struct BattleFxShortOffsetVec *g_BattleSummonAlexanderOffset;
extern int g_BattleSummonAlexanderTriangleState;
extern unsigned char g_BattleFxSummonAlexanderSpawnPrimitivePairsTaskInit;
extern struct BattleFxCameraOrbitState g_BattleSummonAlexanderActor;
extern short g_BattleSummonAlexanderAngles[3];
extern struct BattleFxTimelineState *g_BattleSummonAlexanderTimelineState;
extern unsigned char g_BattleFxSummonAlexanderFlatTrianglesTaskInit;
extern unsigned char g_BattleFxSummonAlexanderScalingMeshTaskInit;
extern struct GfxTableCursor *g_BattleSummonAlexanderTableCursor;
extern int g_BattleSummonAlexanderDword;
extern short g_BattleSummonAlexanderRotationZ;
extern short g_BattleSummonAlexanderSpriteScaleY;
extern struct BattleFxAlexanderSpritePoint g_BattleSummonAlexanderSpriteRecord;
extern int g_BattleSummonAlexanderLoadedResTable2;

/* battle_fx_fat_chocobo_summon.cpp */
extern unsigned char g_BattleSummonFatChocoboSoundGroup[16];
extern int g_BattleSummonFatChocoboUnused1;
extern unsigned char g_BattleSummonFatChocoboUnused2[128];
void __cdecl BattleFx_SummonLoadResFatChocobo(void);
int __cdecl BattleFx_SummonEnterFatChocobo(int targetMask, int actorIdx);
void __cdecl BattleFx_SummonBeginFatChocobo(int targetMask, int actorIdx);
void __cdecl BattleFx_FatChocoboSpawnCameraScript(void *value, void *value2, int value3);
void __cdecl BattleFx_FatChocoboRunCameraScript(void);
short __cdecl BattleFx_FatChocoboReadCameraScriptOperand(void *cursor);
void __cdecl BattleFx_SummonFatChocoboTimelineTask(void);
void __cdecl BattleFx_SummonFatChocoboScalingFadeModelTask(void);
void __cdecl BattleFx_SummonFatChocoboGrowingModelTask(void);
void __cdecl BattleFx_SummonFatChocoboSpawnAnimatedSpritesTask(void);
void __cdecl BattleFx_SummonFatChocoboAnimatedSpriteTask(void);
int __cdecl BattleFx_SummonFatChocoboInstallSpriteMatrix(void *vector, short scale, int offset);
void __cdecl BattleFx_SummonFatChocoboCameraShakeTask(void);
void __cdecl BattleFx_SummonFatChocoboOffsetRampTask(void);
void __cdecl BattleFx_SummonFatChocoboScreenFillTask(void);

/* battle_fx_chocomog_summon.cpp */
extern unsigned char g_BattleSummonChocoMogSoundGroup1[16];
extern unsigned char g_BattleSummonChocoMogSoundGroup2[16];
extern unsigned int g_BattleSummonChocoMogSoundGroups[4];
extern unsigned char g_BattleSummonChocoMogCameraScript1[48];
extern unsigned char g_BattleSummonChocoMogCameraScript2[44];
extern struct PsxMatrix g_BattleSummonChocoMogLightMatrix;
void __cdecl BattleFx_SummonChocoMogDetachModelPartPointers(struct BattleFxModelPartDetach *detach, struct BattleFxChocoMogDetachState *object);
void __cdecl BattleFx_SummonChocoMogRestoreModelPartPointers(void);
void __cdecl BattleFx_SummonChocoMogTakeModelPartPointers(struct PolygonModel *context);
void __cdecl BattleFx_SummonLoadResChocoMog(void);
int __cdecl BattleFx_SummonEnterChocoMog(int targetMask, int actorIdx);
void __cdecl BattleFx_SummonBeginChocoMog(int targetMask, int actorIdx);
void __cdecl BattleFx_ChocoMogSpawnCameraScript(void *scriptCursor, void *auxPtr, int flags);
void __cdecl BattleFx_ChocoMogRunCameraScript(void);
short __cdecl BattleFx_ChocoMogReadCameraScriptOperand(short **cursor);
void __cdecl BattleFx_SummonChocoMogTimelineTask(void);
void __cdecl BattleFx_SummonChocoMogSpawnAnimatedSpritesTask(void);
void __cdecl BattleFx_SummonChocoMogAnimatedSpriteTask(void);
int __cdecl BattleFx_SummonChocoMogInstallSpriteMatrix(struct Vec3sPacked *vector, short scale, int offset);
void __cdecl BattleFx_SummonChocoMogPulsingSpriteTask(void);
void __cdecl BattleFx_SummonChocoMogMoveCameraTask(void);
void __cdecl BattleFx_SummonChocoMogOrbitingSpritesTask(void);
void __cdecl BattleFx_SummonChocoMogSpawnRisingSpritesTask(void);
void __cdecl BattleFx_SummonChocoMogRisingSpriteTask(void);
void __cdecl BattleFx_SummonChocoMogBoneSpritePairTask(void);

/* battle_fx_phoenix_summon.cpp */
extern struct BattleSummonPhoenixAnimData1 g_BattleSummonPhoenixAnimData1;
extern struct BattleSummonPhoenixAnimData2 g_BattleSummonPhoenixAnimData2;
extern struct BattleSummonPhoenixSpriteRequest1 g_BattleSummonPhoenixSpriteRequest1;
extern struct BattleSummonPhoenixCoordData1 g_BattleSummonPhoenixCoordData1;
extern struct BattleSummonPhoenixCoordData2 g_BattleSummonPhoenixCoordData2;
extern struct BattleSummonPhoenixCoordData3 g_BattleSummonPhoenixCoordData3;
extern struct BattleSummonPhoenixCoordData4 g_BattleSummonPhoenixCoordData4;
extern struct BattleSummonPhoenixSpriteRequest3 g_BattleSummonPhoenixSpriteRequest3;
extern struct BattleSummonPhoenixSpriteRequest4 g_BattleSummonPhoenixSpriteRequest4;
extern struct BattleSummonPhoenixSpriteRequest5 g_BattleSummonPhoenixSpriteRequest5;
extern struct BattleSummonPhoenixAnimData3 g_BattleSummonPhoenixAnimData3;
extern int g_BattleSummonPhoenixParam3;
extern unsigned char g_BattleSummonPhoenixTransformMatrix1[30];
extern unsigned char g_BattleSummonPhoenixTransformMatrix2[30];
extern struct BattleSummonPhoenixSpriteRequest7 g_BattleSummonPhoenixSpriteRequest7;
extern struct BattleSummonPhoenixAngleData1 g_BattleSummonPhoenixAngleData1;
extern short g_BattleSummonPhoenixAngleData2[3];
extern short g_BattleSummonPhoenixAngleData3[3];
extern struct BattleFxPhoenixBillboardState *g_BattleSummonPhoenixState;
extern struct BattleSummonPhoenixParticle g_BattleSummonPhoenixBuffer2[100];
extern struct BattleFxPhoenixTimelineScratch *g_BattleSummonPhoenixTimelineState;
extern unsigned char g_BattleSummonPhoenixOpacity;
extern struct BattleFxPhoenixGrowState *g_BattleSummonPhoenixLoadedResTable;
extern int g_BattleSummonPhoenixActiveCount;
extern struct Vec3sPacked g_BattleSummonPhoenixCoordBuffer3;
extern int g_BattleSummonPhoenixStateData3;
void __cdecl BattleFx_SummonLoadResPhoenix(void);
int __cdecl BattleFx_SummonEnterPhoenix(int targetMask, int actorIdx);
void __cdecl BattleFx_SummonBeginPhoenix(int targetMask, int actorIdx);
void __cdecl BattleFx_PhoenixSpawnCameraScript(const void *scriptParam, struct PsxMatrix *matrixPtr, int actorIndex);
void __cdecl BattleFx_PhoenixRunCameraScript(void);
short __cdecl BattleFx_PhoenixReadCameraScriptOperand(short **cursor);
int __cdecl BattleFx_PhoenixApplyCameraEasing(int value, int easeParam);
void __cdecl BattleFx_SummonPhoenixTimelineTask(void);
void __cdecl BattleFx_SummonPhoenixRisingSpriteEmitterTask(void);
short *__cdecl BattleFx_SummonPhoenixInstallBillboardMatrix(void *positionPtr, short rotation, int distance);
void __cdecl BattleFx_SummonPhoenixAnimatedSpriteEntry7Task(void);
void __cdecl BattleFx_SummonPhoenixCounterSpinModelsTask(void);
void __cdecl BattleFx_SummonPhoenixSpawnFallingSpritesTask(void);
void __cdecl BattleFx_SummonPhoenixFallingFadeSpriteTask(void);
void __cdecl BattleFx_SummonPhoenixSpawnRingColumnsTask(void);
void __cdecl BattleFx_SummonPhoenixRingColumnTask(void);
void __cdecl BattleFx_SummonPhoenixGrowingDipModelTask(void);
void __cdecl BattleFx_SummonPhoenixTrailedModelsTask(void);
void __cdecl BattleFx_SummonPhoenixSpawnSpinQuadsTask(void);
void __cdecl BattleFx_SummonPhoenixSpinQuadTask(void);
void __cdecl BattleFx_SummonPhoenixSpawnBillboardSpritesTask(void);
void __cdecl BattleFx_SummonPhoenixBillboardSpriteTask(void);
void __cdecl BattleFx_SummonPhoenixBuildRingGridMesh(void);
void __cdecl BattleFx_SummonPhoenixRingGridWaveTask(void);
void __cdecl BattleFx_SummonPhoenixSetRingGridRowHeights(int centerRow, int range, int rowHeight);
void __cdecl BattleFx_SummonPhoenixMoveSummonActorTask(void);
void __cdecl BattleFx_SummonPhoenixGrowingModelTask(void);
void __cdecl BattleFx_SummonPhoenixDampedSpritePoolTask(void);

/* battle_fx_shiva_summon.cpp */
extern unsigned char g_BattleSummonShivaSoundData[64];
extern unsigned char g_BattleSummonShivaCameraState[216];
extern unsigned char g_BattleSummonShivaCameraTracksAlt[216];
extern char g_BattleSummonShivaEasingType;
extern char g_BattleSummonShivaEasingParam;
extern char g_BattleSummonShivaEasingMode;
extern struct BattleFxIce3MultiTexturedShard *g_BattleSummonShivaTargetCentroidX;
extern unsigned char g_BattleSummonShivaFragmentIndexInit;
extern unsigned char g_BattleSummonShivaTargetCentroidXInit;
extern unsigned char g_BattleSummonShivaRingRadiusInit;
extern struct BattleFxShivaFragmentIndex *g_pBattleSummonShivaColorScratch;
extern struct BattleFxShivaRingScratch *g_BattleSummonShivaRingRadius;
void __cdecl BattleFx_SummonLoadResShiva(void);
int __cdecl BattleFx_SummonEnterShiva(int targetMask, int actorIdx);
void __cdecl BattleFx_SummonBeginShiva(int targetMask, int actorIdx);
void __cdecl BattleFx_ShivaSpawnCameraScript(void *scriptState, void *matrixPtr, int actorIndex);
void __cdecl BattleFx_ShivaRunCameraScript(void);
short __cdecl BattleFx_ShivaReadCameraScriptOperand(short **cursor);
int __cdecl BattleFx_ShivaApplyCameraEasing(int value, int easeParam);
void __cdecl BattleFx_SummonShivaClearParticleRecords(void);
void __cdecl BattleFx_SummonShivaClearSpriteCallbacks(void);
void __cdecl BattleFx_SummonShivaRunSpriteCallbacksTask(void);
void __cdecl BattleFx_SummonShivaTimelineTask(void);
void __cdecl BattleFx_SummonShivaSetForceFieldTable(struct BattleFxShivaForceFieldEntry *forceFieldTable);
void __cdecl BattleFx_SummonShivaSpawnParticleRecordsTask(void);
void __cdecl BattleFx_SummonShivaDrawParticleSprite(void *particle, int extra, int age);
int __cdecl BattleFx_SummonShivaInstallBillboardMatrix(void *vertexPtr, short ringRadius, int offsetScale);
void __cdecl BattleFx_SummonShivaDrawParticleDot(void *vertexPtr, int extra, int age);
void __cdecl BattleFx_SummonShivaBlackModelFadeTask(void);
void __cdecl BattleFx_SummonShivaSpawnShrinkingModelsTask(void);
void __cdecl BattleFx_SummonShivaExpandingFadeModelTask(void);
void __cdecl BattleFx_SummonShivaShrinkingModelTask(void);
void __cdecl BattleFx_SummonShivaSpawnModelBurstTask(void);
void __cdecl BattleFx_SummonShivaModelBurstPieceTask(void);
int __cdecl BattleFx_SummonShivaClaimSpriteCallbackSlot(int callback);
void __cdecl BattleFx_SummonShivaDrawAnimatedSprite(struct BattleFxShivaSpriteRecord *spriteRecord);
void __cdecl BattleFx_SummonShivaSpawnTriangleFragmentsTask(void);
int __cdecl BattleFx_SummonShivaInitTriangleFragments(int resourceBlock, int recordIndex);
void __cdecl BattleFx_SummonShivaTriangleFragmentsTask(void);
void __cdecl BattleFx_SummonShivaUpdateTriangleFragmentMatrices(int resourceBlock, int startIndex, struct BattleFxShardTransformBatch *positionPtr, void *matrix);
void __cdecl BattleFx_SummonShivaSpawnOrientedSpritesTask(void);
void __cdecl BattleFx_SummonShivaOrientedSpriteTask(void);
void __cdecl BattleFx_SummonShivaUpdateAttachPointTableTask(void);
void __cdecl BattleFx_SummonShivaMoveSummonActorTask(void);

/* battle_fx_ifrit_summon.cpp */
extern unsigned char g_BattleSummonIfritMeshSource3[1614];
extern unsigned char g_BattleSummonIfritMeshSource4[1614];
extern unsigned char g_BattleSummonIfritMeshSource5[1614];
extern unsigned char g_BattleSummonIfritMeshSource6[1614];
extern unsigned char g_BattleSummonIfritMeshSource7[1614];
extern unsigned char g_BattleSummonIfritMeshSource8[1614];
extern unsigned char g_BattleSummonIfritMeshSource9[1614];
extern unsigned char g_BattleSummonIfritMeshSource10[1614];
extern unsigned char g_BattleSummonIfritMeshSource11[1614];
extern int g_BattleSummonIfritWarpSlot1;
extern int g_BattleSummonIfritWarpSlot2;
extern int g_BattleSummonIfritWarpSlot3;
extern short g_BattleSummonIfritActorMoveParams[4];
extern unsigned char g_BattleSummonIfritTimelineReady;
extern struct BattleFxPosition3s *g_BattleSummonIfritActorPosScratch;
extern struct BattleFxIfritTimelineState *g_BattleSummonIfritTimelineState;
extern short g_BattleSummonIfritModelPosition[3];
extern unsigned char g_BattleSummonIfritMoveReady;
void __cdecl BattleFx_SummonLoadResIfrit(void);
void __cdecl BattleFx_SummonBeginIfrit(int targetMask, int actorIdx);
void __cdecl BattleFx_IfritSpawnCameraScript(void *cameraTrack, void *rotMatrix, int actorIndex);
void __cdecl BattleFx_IfritRunCameraScript(void);
short __cdecl BattleFx_IfritReadCameraScriptOperand(short **cursorPtr);
int __cdecl BattleFx_IfritApplyCameraEasing(int value, int easeAmount);
void __cdecl BattleFx_SummonIfritTimelineTask(void);
void __cdecl BattleFx_SummonIfritMorphMeshTask(void);
void __cdecl BattleFx_SummonIfritGrowFadeModelTask(void);
void __cdecl BattleFx_SummonIfritTwinRotatingModelTask(void);
void __cdecl BattleFx_SummonIfritDrawMeshSpawnParticlesTask(void);
void __cdecl BattleFx_SummonIfritPointParticleTask(void);
void __cdecl BattleFx_SummonIfritScaleFadeModelTask(void);
void __cdecl BattleFx_SummonIfritSpawnAnimatedSpritesTask(void);
void __cdecl BattleFx_SummonIfritAnimatedSpriteTask(void);
void __cdecl BattleFx_SummonIfritScreenStripWarpTask(void);
void __cdecl BattleFx_SummonIfritMoveSummonActorTask(void);
void __cdecl BattleFx_SummonIfritSpinningMeshTask(void);
void __cdecl BattleFx_SummonIfritPulsingFadeModelTask(void);
void __cdecl BattleFx_SummonIfritBillboardSpriteTask(void);

/* battle_fx_summon_ramuh.cpp */
extern unsigned char g_BattleSummonRamuhSoundData[80];
extern struct BattleSummonRamuhMeshSource1 g_BattleSummonRamuhMeshSource1;
extern unsigned char g_BattleSummonRamuhMeshSource3[1086];
extern unsigned char g_BattleSummonRamuhMeshSource4[1086];
extern unsigned char g_BattleSummonRamuhMeshSource5[1086];
extern struct BattleSummonRamuhSavedActor g_BattleSummonRamuhSavedActorBitmasks;
extern struct BattleSummonRamuhCameraSlot g_BattleSummonRamuhMeshDeltaBuffer;
void __cdecl BattleFx_SummonLoadResRamuh(void);
int __cdecl BattleFx_SummonEnterRamuh(int targetMask, int actorIdx);
void __cdecl BattleFx_RamuhSpawnCameraScript(void *script, void *matrix, int actorSlot);
void __cdecl BattleFx_RamuhRunCameraScript(void);
short __cdecl BattleFx_RamuhReadCameraScriptOperand(short **cursor);
void __cdecl BattleFx_SummonRamuhTimelineTask(void);
void __cdecl BattleFx_SummonRamuhSetCameraShakeAmplitude(int amplitude);
void __cdecl BattleFx_SummonRamuhCameraShakeTask(void);
void __cdecl BattleFx_SummonRamuhQuadBillboardTask(void);
void __cdecl BattleFx_SummonRamuhFourFrameSpriteSetTask(void);
int __cdecl BattleFx_SummonRamuhDrawPsxSpriteRecords(struct BattleFxRamuhPsxSpriteHeader *header, int orderingTable, int depthShift, int packetCursor);
void __cdecl BattleFx_SummonRamuhSpawnBurstChildrenTask(void);
void __cdecl BattleFx_SummonRamuhMorphingMeshTask(void);
void __cdecl BattleFx_SummonRamuhTumblingModelTask(void);
void __cdecl BattleFx_SummonRamuhBillboardSpriteTask(void);
void __cdecl BattleFx_SummonRamuhComputeMeshVertexDeltas(struct Vec3sPacked *poseA, struct Vec3sPacked *poseB, struct Vec3sPacked *deltas, int divisor);
void __cdecl BattleFx_SummonRamuhAddMeshVertexDeltas(struct Vec3sPacked *dest, struct Vec3sPacked *deltas);
void __cdecl BattleFx_SummonRamuhExpandingSpriteSetTask(void);
void __cdecl BattleFx_SummonRamuhRadialLineBurstTask(void);
void __cdecl BattleFx_SummonRamuhDrawRadialLineBurst(int color);
void __cdecl BattleFx_SummonRamuhSpawnEightModelsTask(void);
void __cdecl BattleFx_SummonRamuhScaledSpriteSetBillboardTask(void);
void __cdecl BattleFx_SummonRamuhFadingBillboardSpriteTask(void);
void __cdecl BattleFx_SummonRamuhSpawnAngledSpritesTask(void);
void __cdecl BattleFx_SummonRamuhAngledSpriteTask(void);
void __cdecl BattleFx_SummonRamuhTransformAttachPointToEffectSpace(void);
void __cdecl BattleFx_SummonRamuhSetActorShakeAmplitude(int amplitude);
void __cdecl BattleFx_SummonRamuhActorShakeTask(void);

/* battle_fx_summon_titan.cpp */
extern unsigned char g_BattleSummonTitanVertexNormalList[160];
extern short g_BattleSummonTitanOffsetZ;
extern unsigned char g_BattleSummonTitanRotStep1[16];
extern int g_BattleSummonTitanScaleRate1;
extern int g_BattleSummonTitanScaleRate2;
extern struct BattleFxTitanOffscreenRequest g_BattleSummonTitanJitterParams;
extern struct Vec3sPacked g_BattleSummonTitanCameraShakeLimits;
extern struct Vec3sPacked g_BattleSummonTitanCameraShakeParams;
void __cdecl BattleFx_SummonLoadResTitan(void);
void __cdecl BattleFx_TitanSpawnCameraScript(void *script, void *basis, int slot);
void __cdecl BattleFx_TitanRunCameraScript(void);
short __cdecl BattleFx_TitanReadCameraScriptOperand(short **cursor);
int __cdecl BattleFx_TitanApplyCameraEasing(int value, int easing);
void __cdecl BattleFx_SummonTitanTimelineTask(void);
void __cdecl BattleFx_SummonTitanSetCameraShakeWithDecay(int amplitude, int decay);
void __cdecl BattleFx_SummonTitanCameraShakeWithDecayTask(void);
void __cdecl BattleFx_SummonTitanQueueStageOffscreenRequests(struct Vec3sPacked *stageVec, struct BattleFxTitanOffscreenRequest *offscreenReq);
void __cdecl BattleFx_SummonTitanFragmentingMeshTask(void);
void __cdecl BattleFx_SummonTitanSpawnScatterSprites(void);
void __cdecl BattleFx_SummonTitanBillboardSpriteTask(void);
void __cdecl BattleFx_SummonTitanRotatingFragmentMeshTask(void);
void __cdecl BattleFx_SummonTitanSpawnDirectedSpritesTask(void);
void __cdecl BattleFx_SummonTitanDirectedSpriteTask(void);
void __cdecl BattleFx_SummonTitanBouncingDebrisTask(void);
void __cdecl BattleFx_SummonTitanRotateSelectedActorPositions(struct GfxRotationAngles2 *rotVec, int tiltStep);
void __cdecl BattleFx_SummonTitanSpawnTablePointSpritesTask(void);
void __cdecl BattleFx_SummonTitanInitFragmentVelocitiesFromNormals(void);
void __cdecl BattleFx_SummonTitanInitFragmentVelocitiesFromVertexXZ(void);

/* battle_fx_summon_leviathan.cpp */
extern int g_BattleSummonLeviathan_ResourceDefs;
extern unsigned char g_BattleSummonLeviathan_CameraScript1[200];
extern unsigned char g_BattleSummonLeviathan_CameraScript2[8];
extern unsigned char g_BattleSummonLeviathan_QuadReq[8];
extern int g_BattleSummonLeviathan_RotAngles;
extern struct PsxMatrix g_BattleSummonLeviathan_TransformMatrix;
extern int g_BattleSummonLeviathan_RotAngle2;
extern int g_BattleSummonLeviathan_RotVector4;
void __cdecl BattleFx_SummonLoadResLeviathan(void);
void __cdecl BattleFx_SummonLeviathanOffsetVertexPartToFloat(int vertexIndex, struct BattleFxSlowBlendVertex *srcVec, struct BattleFxSlowBlendVertex *dstVec);
int __cdecl BattleFx_SummonEnterLeviathan(int targetMask, int actorIdx);
void __cdecl BattleFx_LeviathanSpawnCameraScript(void *cameraScript, void *matrix, int targetIndex);
void __cdecl BattleFx_LeviathanRunCameraScript(void);
struct BattleFxSummonLeviathanModelPairTaskR;
short __cdecl BattleFx_LeviathanReadCameraScriptOperand(short **cursorRef);
int __cdecl BattleFx_LeviathanApplyCameraEasing(int value, int easeCount);
void __cdecl BattleFx_SummonLeviathanTimelineTask(void);
void __cdecl BattleFx_SummonLeviathanBurstModelTask(void);
void __cdecl BattleFx_SummonLeviathanInitPrimitiveFragmentVelocities(void *block);
void __cdecl BattleFx_SummonLeviathanInwardSpriteTask(void);
int __cdecl BattleFx_SummonLeviathanTransformPointWithPush(void *srcPoint, int push, struct BattleFxSummonLeviathanModelPairTaskR *transform);
void __cdecl BattleFx_SummonLeviathanSpriteParticlePoolTask(void);
void __cdecl BattleFx_SummonLeviathanPulsingModelTask(void);
void __cdecl BattleFx_SummonLeviathanModelPairTask(void);
void __cdecl BattleFx_SummonLeviathanPartModelTask(void);
void __cdecl BattleFx_SummonLeviathanAnimatedSpriteTask(void);

/* battle_fx_throw_vincent.cpp */
extern unsigned char g_BattleThrowVincentResNameGun01[14];
extern unsigned char g_BattleThrowVincentResNameGun02[14];
extern unsigned char g_BattleThrowVincentResNameGun03[14];
extern unsigned char g_BattleThrowVincentResNameGun04[14];
extern unsigned char g_BattleThrowVincentResNameGun05[14];
extern unsigned char g_BattleThrowVincentResNameGun06[14];
extern unsigned char g_BattleThrowVincentResNameGun07[14];
extern unsigned char g_BattleThrowVincentResNameGun08[14];
extern unsigned char g_BattleThrowVincentResNameGun09[14];
extern unsigned char g_BattleThrowVincentResNameGun10[14];
extern unsigned char g_BattleThrowVincentResNameGun11[14];
extern unsigned char g_BattleThrowVincentResNameGun12[14];
extern unsigned char g_BattleThrowVincentResNameGun13_B[14];
extern unsigned char g_BattleThrowVincentResNameGun01_B[14];
extern unsigned char g_BattleThrowVincentResNameGun02_B[14];
extern unsigned char g_BattleThrowVincentResNameGun03_B[14];
extern unsigned char g_BattleThrowVincentResNameGun04_B[14];
extern unsigned char g_BattleThrowVincentResNameGun05_B[14];
extern unsigned char g_BattleThrowVincentResNameGun06_B[14];
extern unsigned char g_BattleThrowVincentResNameGun07_B[14];
extern unsigned char g_BattleThrowVincentResNameGun08_B[14];
extern unsigned char g_BattleThrowVincentResNameGun09_B[14];
extern unsigned char g_BattleThrowVincentResNameGun10_B[14];
extern unsigned char g_BattleThrowVincentResNameGun11_B[14];
extern unsigned char g_BattleThrowVincentResNameGun12_B[14];
extern unsigned char g_BattleThrowVincentResNameGun13[14];
extern int g_BattleThrowVincentTargetIndex;
extern int g_BattleThrowVincentDeltaVectorX;
extern int g_BattleThrowVincentDeltaVectorY;
extern int g_BattleThrowVincentDeltaVectorZ;
extern int g_BattleThrowVincentWeaponFlag;
extern int g_BattleThrowVincentWeaponIndex;
extern int g_BattleThrowVincentTargetMask;
extern int g_BattleThrowVincentActorIndex;
extern struct GfxTableCursor *g_BattleThrowVincentLoadedResTable;
void __cdecl BattleFx_ThrowLoadResVincentWeapons(void);
void __cdecl Battle_ThrowFxWeaponBlock114(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index1(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index2(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index3(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index4(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index5(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index6(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index7(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index8(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index9(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index10(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index11(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock114Index12(int targetMask, int actorIdx);

/* battle_fx_throw_cait_sith.cpp */
extern unsigned int g_BattleThrowCaitSithResourceDef[130];
extern int g_BattleThrowCaitSithTargetIndex;
extern int g_BattleThrowCaitSithDeltaVectorX;
extern int g_BattleThrowCaitSithDeltaVectorY;
extern int g_BattleThrowCaitSithDeltaVectorZ;
extern int g_BattleThrowCaitSithWeaponFlag;
extern int g_BattleThrowCaitSithWeaponIndex;
extern int g_BattleThrowCaitSithTargetMask;
extern int g_BattleThrowCaitSithActorIndex;
extern struct GfxTableCursor *g_BattleThrowCaitSithLoadedResTable;
void __cdecl BattleFx_ThrowLoadResCaitSithWeapons(void);
void __cdecl Battle_ThrowFxWeaponBlock101(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index1(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index2(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index3(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index4(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index5(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index6(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index7(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index8(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index9(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index10(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index11(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock101Index12(int targetMask, int actorIdx);

/* battle_fx_throw_yuffie.cpp */
extern unsigned int g_BattleThrowYuffieResourceDef[150];
extern int g_BattleThrowYuffieTargetIndex;
extern int g_BattleThrowYuffieDeltaVector;
extern int g_BattleThrowYuffieDeltaVectorY;
extern int g_BattleThrowYuffieDeltaVectorZ;
extern int g_BattleThrowYuffieKind3Cursor;
extern int g_BattleThrowYuffieKind3Phase;
extern union BattleFxYuffieThrowState g_BattleThrowYuffieThrowState;
extern int g_BattleThrowYuffieTargetMask;
extern int g_BattleThrowYuffieStepRatio;
extern struct BattleFxYuffieKind3Target g_BattleThrowYuffieKind3Targets[6];
void __cdecl BattleFx_ThrowLoadResYuffieWeapons(void);
void __cdecl Battle_ThrowFxWeaponBlock87(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index1(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index2(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index3(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index4(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index5(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index6(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index7(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index8(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index9(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index10(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index11(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index12(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock87Index13(int targetMask, int actorIdx);
void __cdecl Battle_CoinThrowEffect(int targetMask, int actorIdx);

/* battle_fx_throw_cid.cpp */
extern unsigned int g_BattleThrowCidResourceDef[140];
extern unsigned char g_BattleThrowCidResNameWeapon01[13];
extern unsigned char g_BattleThrowCidResNameWeapon02[13];
extern unsigned char g_BattleThrowCidResNameWeapon03[13];
extern unsigned char g_BattleThrowCidResNameWeapon04[13];
extern unsigned char g_BattleThrowCidResNameWeapon05[13];
extern unsigned char g_BattleThrowCidResNameWeapon06[13];
extern unsigned char g_BattleThrowCidResNameWeapon07[13];
extern unsigned char g_BattleThrowCidResNameWeapon08[13];
extern unsigned char g_BattleThrowCidResNameWeapon09[13];
extern unsigned char g_BattleThrowCidResNameWeapon10[13];
extern unsigned char g_BattleThrowCidResNameWeapon11[13];
extern unsigned char g_BattleThrowCidResNameWeapon12[13];
extern unsigned char g_BattleThrowCidResNameWeapon13[13];
extern unsigned char g_BattleThrowCidResNameWeapon14[13];
extern unsigned char g_BattleThrowCidResNameWeapon01_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon02_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon03_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon04_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon05_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon06_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon07_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon08_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon09_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon10_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon11_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon12_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon13_B[13];
extern unsigned char g_BattleThrowCidResNameWeapon14_B[13];
extern struct BattleThrowCidRibbonParams g_BattleThrowCidTrailRibbonParams[2];
extern int g_BattleThrowCidDirX;
extern int g_BattleThrowCidDirY;
extern int g_BattleThrowCidDirZ;
extern short g_BattleThrowCidActorPos[4];
extern int g_BattleThrowCidFlightKind;
extern int g_BattleThrowCidModelIndex;
extern short g_BattleThrowCidTargetBonePos[3];
extern int g_BattleThrowCidTargetMask;
extern int g_BattleThrowCidActorIndex;
extern struct Int64Pair g_BattleThrowCidHeadPos;
extern struct VECTOR g_BattleThrowCidRotatedAxis;
extern struct GfxTableCursor *g_BattleThrowCidLoadedResTable;
void __cdecl BattleFx_ThrowLoadResCidWeapons(void);
void __cdecl Battle_ThrowFxWeaponBlock73(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index1(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index2(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index3(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index4(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index5(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index6(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index7(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index8(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index9(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index10(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index11(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index12(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock73Index13(int targetMask, int actorIdx);

/* battle_fx_throw_aeris.cpp */
extern unsigned int g_BattleThrowAerisResourceDef[110];
extern struct VECTOR g_BattleThrowAerisVectorOffset;
extern unsigned char g_BattleThrowAerisResNameRod_01_02[22];
extern unsigned char g_BattleThrowAerisResNameRod_03_04[22];
extern unsigned char g_BattleThrowAerisResNameRod_05_06[22];
extern unsigned char g_BattleThrowAerisResNameRod_07_08[22];
extern unsigned char g_BattleThrowAerisResNameRod_09_10[22];
extern unsigned char g_BattleThrowAerisResNameRod_11_01[22];
extern unsigned char g_BattleThrowAerisResNameRod_02_03[22];
extern unsigned char g_BattleThrowAerisResNameRod_04_05[22];
extern unsigned char g_BattleThrowAerisResNameRod_06_07[22];
extern unsigned char g_BattleThrowAerisResNameRod_08_09[22];
extern unsigned char g_BattleThrowAerisResNameRod_10_11[22];
extern int g_BattleThrowAerisTargetIndex;
extern int g_BattleThrowAerisDeltaVectorX;
extern int g_BattleThrowAerisDeltaVectorY;
extern int g_BattleThrowAerisDeltaVectorZ;
extern int g_BattleThrowAerisWeaponFlag;
extern int g_BattleThrowAerisWeaponIndex;
extern int g_BattleThrowAerisTargetMask;
extern int g_BattleThrowAerisActorIndex;
extern struct GfxTableCursor *g_BattleThrowAerisLoadedResTable;
void __cdecl BattleFx_ThrowLoadResAerisWeapons(void);
void __cdecl Battle_ThrowFxWeaponBlock62(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index1(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index2(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index3(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index4(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index5(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index6(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index7(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index8(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index9(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock62Index10(int targetMask, int actorIdx);

/* battle_fx_throw_barret.cpp */
extern unsigned char g_BattleThrowBarretResNameWeapon01[13];
extern unsigned char g_BattleThrowBarretResNameWeapon02[13];
extern unsigned char g_BattleThrowBarretResNameWeapon03[13];
extern unsigned char g_BattleThrowBarretResNameWeapon04[13];
extern unsigned char g_BattleThrowBarretResNameWeapon05[13];
extern unsigned char g_BattleThrowBarretResNameWeapon06[13];
extern unsigned char g_BattleThrowBarretResNameWeapon07[13];
extern unsigned char g_BattleThrowBarretResNameWeapon08[13];
extern unsigned char g_BattleThrowBarretResNameWeapon09[13];
extern unsigned char g_BattleThrowBarretResNameWeapon10[13];
extern unsigned char g_BattleThrowBarretResNameWeapon11[13];
extern unsigned char g_BattleThrowBarretResNameWeapon12[13];
extern unsigned char g_BattleThrowBarretResNameWeapon13[13];
extern unsigned char g_BattleThrowBarretResNameWeapon14[13];
extern unsigned char g_BattleThrowBarretResNameWeapon15_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon01_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon02_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon03_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon04_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon05_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon06_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon07_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon08_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon09_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon10_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon11_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon12_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon13_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon14_B[13];
extern unsigned char g_BattleThrowBarretResNameWeapon15[13];
extern int g_BattleThrowBarretDeltaVectorX;
extern int g_BattleThrowBarretDeltaVectorY;
extern int g_BattleThrowBarretDeltaVectorZ;
void __cdecl BattleFx_ThrowLoadResBarretWeapons(void);
void __cdecl Battle_ThrowFxWeaponBlock32(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index1(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index2(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index3(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index4(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index5(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index6(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index7(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index8(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index9(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index10(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index11(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index12(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index13(int targetMask, int actorIndex);
void __cdecl Battle_ThrowFxWeaponBlock32Index14(int targetMask, int actorIndex);

/* battle_fx_throw_cloud.cpp */
extern unsigned char g_BattleThrowCloudResNameWeapon01[14];
extern unsigned char g_BattleThrowCloudResNameWeapon02[14];
extern unsigned char g_BattleThrowCloudResNameWeapon03[14];
extern unsigned char g_BattleThrowCloudResNameWeapon04[14];
extern unsigned char g_BattleThrowCloudResNameWeapon05[14];
extern unsigned char g_BattleThrowCloudResNameWeapon06[14];
extern unsigned char g_BattleThrowCloudResNameWeapon07[14];
extern unsigned char g_BattleThrowCloudResNameWeapon08[14];
extern unsigned char g_BattleThrowCloudResNameWeapon09[14];
extern unsigned char g_BattleThrowCloudResNameWeapon10[14];
extern unsigned char g_BattleThrowCloudResNameWeapon11[14];
extern unsigned char g_BattleThrowCloudResNameWeapon12[14];
extern unsigned char g_BattleThrowCloudResNameWeapon13[14];
extern unsigned char g_BattleThrowCloudResNameWeapon14[14];
extern unsigned char g_BattleThrowCloudResNameWeapon15_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon01_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon02_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon03_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon04_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon05_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon06_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon07_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon08_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon09_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon10_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon11_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon12_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon13_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon14_B[14];
extern unsigned char g_BattleThrowCloudResNameWeapon15[14];
extern int g_BattleThrowCloudDeltaVectorX;
extern int g_BattleThrowCloudDeltaVectorY;
extern int g_BattleThrowCloudDeltaVectorZ;
void __cdecl BattleFx_ThrowLoadResCloudWeapons(void);
void __cdecl Battle_ThrowFxWeaponBlock00(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index1(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index2(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index3(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index4(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index5(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index6(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index7(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index8(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index9(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index10(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index11(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index12(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock00Index13(int targetMask, int actorIdx);
void __cdecl Battle_ThrowFxWeaponBlock16(int targetMask, int actorIdx);

/* battle_fx_weapon_diamond_flash.cpp */
void __cdecl BattleFx_WeaponLoadResDiamondFlash(void);
void __cdecl BattleFx_WeaponEnterDiamondFlash(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponBeginDiamondFlash(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponDiamondFlashTimelineTask(void);
void __cdecl BattleFx_WeaponDiamondFlashPulsingOrientedModelTask(void);
void __cdecl BattleFx_WeaponDiamondFlashPulsingBillboardModelTask(void);
void __cdecl BattleFx_WeaponDiamondFlashInwardModelTask(void);
void __cdecl BattleFx_WeaponDiamondFlashConvergingModelTask(void);
void __cdecl BattleFx_WeaponDiamondFlashDampedSpriteTask(void);
void __cdecl BattleFx_WeaponDiamondFlashTripleModelStreakTask(void);
void __cdecl BattleFx_WeaponDiamondFlashBillboardSpriteTask(void);
void __cdecl BattleFx_WeaponDiamondFlashShrinkingModelPairTask(void);
void __cdecl BattleFx_WeaponDiamondFlashTrailingProjectileTask(void);
void __cdecl BattleFx_WeaponDiamondFlashTrailModelTask(void);
void __cdecl BattleFx_WeaponDiamondFlashScreenFlashTask(void);

/* battle_fx_weapon_revenge_stamp.cpp */
void __cdecl BattleFx_WeaponLoadResRevengeStamp(void);
void __cdecl BattleFx_WeaponEnterRevengeStamp(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponBeginRevengeStamp(int targetMask, int actorIdx);
void __cdecl BattleFx_RevengeStampSpawnTimeline(void);
void __cdecl BattleFx_RevengeStampTimelineTask(void);
void __cdecl BattleFx_RevengeStampActorTumbleTask(void);
void __cdecl Battle_GetActorBoneYRange(short actorIndex, short *minY, short *maxY);
void __cdecl BattleFx_RevengeStampRisingSpinningModelTask(void);
void __cdecl BattleFx_RevengeStampStretchingModelTask(void);
void __cdecl BattleFx_RevengeStampGreenGradientStripTask(void);
void __cdecl BattleFx_RevengeStampBlueGradientStripTask(void);
void __cdecl BattleFx_RevengeStampGrayGradientStripTask(void);
void __cdecl BattleFx_RevengeStampBone8And10ParticleTask(void);
void __cdecl BattleFx_RevengeStampBone21And23ParticleTask(void);
void __cdecl BattleFx_RevengeStampSideBurstParticleTask(void);

/* battle_fx_weapon_emerald_beam.cpp */
void __cdecl BattleFx_WeaponLoadResEmeraldBeam(void);
void __cdecl BattleFx_WeaponEnterEmeraldBeamVariant0(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponEnterEmeraldBeamVariant1(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponBeginEmeraldBeam(int targetMask, int casterIdx);
void __cdecl BattleFx_WeaponEmeraldBeamSpawnTimeline(void);
void __cdecl BattleFx_WeaponEmeraldBeamTimelineTask(void);
void __cdecl BattleFx_WeaponEmeraldBeamFadingSpriteTask(void);
void __cdecl BattleFx_WeaponEmeraldBeamDelayedSpriteTask(void);
void __cdecl BattleFx_WeaponEmeraldBeamBeamModelTask(void);
void __cdecl BattleFx_WeaponEmeraldBeamShrinkingModelTask(void);

/* battle_fx_weapon_emerald_shoot.cpp */
void __cdecl BattleFx_WeaponLoadResEmeraldShoot(void);
void __cdecl BattleFx_WeaponEnterEmeraldShoot(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponBeginEmeraldShoot(int targetMask, int actorIdx);
void __cdecl BattleFx_EmeraldShootSpawnTimeline(void);
void __cdecl BattleFx_EmeraldShootTimelineTask(void);
void __cdecl BattleFx_EmeraldShootExpandingModelTask(void);
void __cdecl BattleFx_EmeraldShootFadingSpinningModelTask(void);
void __cdecl BattleFx_EmeraldShootRadialFadingSpriteTask(void);
void __cdecl BattleFx_EmeraldShootImpactTrailSpawnTask(void);
void __cdecl BattleFx_EmeraldShootPushTrailPoint(struct BattleFxEmeraldShootTrailState *trailStatePtr, int pointBufferPtr);
void __cdecl BattleFx_EmeraldShootDelayedFrameSpriteTask(void);
void __cdecl BattleFx_EmeraldShootTrailSpriteTask(void);
void __cdecl BattleFx_EmeraldShootGrowingModelTask(void);
void __cdecl BattleFx_EmeraldShootStretchingSpriteEmitterTask(void);
void __cdecl BattleFx_EmeraldShootStretchingSpriteTask(void);

/* battle_fx_weapon_whirlsand.cpp */
extern unsigned char g_WhirlsandTexturePath_ANT1[9];
void __cdecl BattleFx_WeaponLoadResWhirlsand(void);
void __cdecl BattleFx_WeaponEnterWhirlsand(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponBeginWhirlsand(int targetMask, int actorIdx);
void __cdecl BattleFx_WhirlsandSpawnCameraScript(short *cameraScript, struct PsxMatrix *matrix, int param);
void __cdecl BattleFx_WhirlsandRunCameraScript(void);
short __cdecl BattleFx_WhirlsandReadCameraScriptOperand(short **cursor);
int __cdecl BattleFx_WhirlsandApplyCameraEasing(int value, int easing);
void __cdecl BattleFx_WhirlsandGetActorLocalPosition(int actor, int bone, struct PsxMatrix *matrix, struct Vec3sPacked *offset);
void __cdecl BattleFx_WhirlsandTimelineTask(void);
void __cdecl BattleFx_WhirlsandRingGridHeightEnvelopeTask(void);
void __cdecl BattleFx_WhirlsandBuildAndQueueStageOffscreenRequest(void);
void __cdecl BattleFx_WhirlsandQueueStageOffscreenRequest(struct PsxMatrix *matrix, struct BattleFxWhirlsandStageRequestArgs *args);
void __cdecl BattleFx_WhirlsandBuildRingGridMesh(void);
void __cdecl BattleFx_WhirlsandSetRingGridRowHeights(int height, int colorFrom, int colorTo);
void __cdecl BattleFx_WhirlsandTexturedRingGridDrawTask(void);
void __cdecl BattleFx_WhirlsandDrawTexturedRingGrid(struct PsxMatrix *matrix);
void __cdecl BattleFx_WhirlsandShadedRingGridPairTask(void);
void __cdecl BattleFx_WhirlsandDrawShadedRingGrid(struct PsxMatrix *matrix, int shade);
void __cdecl BattleFx_WhirlsandSurfaceParticleEmitterTask(void);
int __cdecl BattleFx_WhirlsandGetRingGridHeightAtPoint(struct BattleFxRingGridPoint *point);
void __cdecl BattleFx_WhirlsandSurfaceParticleTask(void);
void __cdecl BattleFx_WeaponSpawnWhirlsandPerTarget(int actor);
void __cdecl BattleFx_WhirlsandActorGroundRideTask(void);
void __cdecl BattleFx_WhirlsandSetActorGroundHeightAndDrawShadow(struct BattleActor *actor, struct BattleFxWhirlsandRideSlot *slot);
void __cdecl BattleFx_WhirlsandActorSpiralAwayTask(void);

/* battle_gte_matrix_builders.cpp */
extern int g_BattleGteZeroTranslation;
void __cdecl Gte_CopyMatrixWithTranslation(struct PsxMatrix *dst, struct PsxMatrix *src);
void __cdecl Gte_BuildInverseEulerXyzViewMatrix(struct BattleGteViewMatrixScratch *matrix, void *vector, void *rotation);
void __cdecl Gte_BuildLookAtMatrix(struct PsxMatrix *matrix, struct SVECTOR *eye, struct SVECTOR *target, struct VECTOR *up);
void __cdecl Gte_BuildLookAtMatrixPositiveTranslation(struct PsxMatrix *matrix, struct Vec3sPacked *eye, struct Vec3sPacked *target, struct BattleFxVector3 *up);

/* battle_gzip_inflate_decoder.cpp */
int __cdecl GzipInflate_BinSection(void *input, void *output);
int __cdecl GzipInflate_DecodeBlocks(void);
int __cdecl GzipInflate_DecodeBlock(void *finalFlag);
int __cdecl GzipInflate_CopyStoredBlock(void);
int __cdecl GzipInflate_DecodeFixedBlock(void);
int __cdecl GzipInflate_AllocFromPool(int size);
int __cdecl GzipInflate_WalkHuffmanChain(void *node);
int __cdecl GzipInflate_DecodeDynamicBlock(void);

/* battle_init_enemy_combatants.cpp */
void __cdecl Battle_InitEnemyCombatants(void);
void __cdecl Battle_RunEnemyAiInitEvents(void);

/* battle_init_exp_results.cpp */
extern int g_BattleExpPartyMask;
extern int g_BattleExpAnimFrameCount;
extern int g_BattleExpCompletedMask;
int __cdecl Battle_InitExpResultRows(int partyMask);
void __cdecl Battle_AddRewardItemsToInventory(void);
int __cdecl Kernel_GetLimitAttackId(int charIndex, int limitSlot);

/* battle_scene_misc_helpers.cpp */
int __cdecl Battle_IsSceneInterruptible(void);
void __cdecl Battle_AwardApToEquippedMateria(int partySlot, int equipSlot);
void __cdecl Battle_AttackOpYieldUntilUiReady(void);
int __cdecl Battle_GetSetActorIdlePose(int actorIdx, int pose);
void __cdecl Battle_UnusedNoOp_1(void);

/* battle_item_fx_greens.cpp */
extern struct BattleFxModelDrawHead g_BattleItemGreensSpriteRequest1;
extern struct BattleFxModelDrawHead g_BattleItemGreensSpriteRequest2;
extern struct PsxMatrix g_BattleItemGreensTransformMatrix;
extern char g_BattleItemGreensModelNames[16][12];
extern int g_BattleItemGreensVariantIndex;
struct BattleItemGreensScratch;
extern struct BattleItemGreensScratch *g_pBattleItemGreensScratch;
extern int g_BattleItemGreensFading;
extern unsigned char g_BattleItemGreensScratchInit;
extern unsigned char g_BattleItemGreensSourcePosInit;
extern int g_BattleItemGreensPacketCursor;
extern struct SVECTOR *g_BattleItemGreensSourcePos;
extern int g_BattleItemGreensModelResourceIndex;
extern struct GfxTableCursor *g_BattleItemGreensLoadedResTable;
void __cdecl Battle_ItemFxGreensInit(void);
void __cdecl Battle_ItemFxGreens0(int target, int param);
void __cdecl Battle_ItemFxGreens1(int target, int param);
void __cdecl Battle_ItemFxGreens2(int target, int param);
void __cdecl Battle_ItemFxGreens3(int target, int param);
void __cdecl Battle_ItemFxGreens4(int targetIndex, int sourceIndex);
void __cdecl Battle_ItemFxGreens5(int targetIndex, int sourceIndex);
void __cdecl Battle_ItemFxGreens6(int targetIndex, int sourceIndex);
void __cdecl Battle_ItemFxGreens7(int targetIndex, int sourceIndex);
void __cdecl Battle_ItemFxGreensSpawn(int targetIndex, int sourceIndex);
void __cdecl Battle_ItemFxGreensProjectileTask(void);

/* battle_kernel_experience_growth.cpp */
void __cdecl Kernel_ApplyExperienceToCharacterRecord(struct SavemapCharacter *charRecord, int exp, int partySlot);
/* kernel.bin's battle and growth data (yama/init.c). */
extern struct KernelBattleData g_BattleKernelData;
void __cdecl Kernel_LoadGrowthActor(struct CharGrowth *growthActor, struct SavemapCharacter *charRecord);
void __cdecl Savemap_WriteGrowthToRecord(struct CharGrowth *growth, struct SavemapCharacter *charRecord);
void __cdecl Kernel_BindGrowthRecord(struct CharGrowth *growthPtr);
int __cdecl Kernel_ComputeExpForLevel(struct CharGrowth *growth, int level);
void __cdecl Kernel_LevelUpPrimaryStat(struct CharGrowth *growth, int statIndex);
int __cdecl Kernel_PickGrowthBonusIndex(int delta);
int __cdecl Kernel_EvalPrimaryStatCurve(struct CharGrowth *growth, int level, int statIndex);
void __cdecl Kernel_LevelUpMaxHp(struct CharGrowth *growth);
int __cdecl Kernel_EvalHpCurve(struct CharGrowth *growth, int level);
void __cdecl Kernel_LevelUpMaxMp(struct CharGrowth *growth);
int __cdecl Kernel_EvalMpCurve(struct CharGrowth *growth, int level);
extern int g_BattleArenaRoundIndex;
extern int g_BattleArenaSavedRoundHp;
extern unsigned int g_BattleExpTargetByPartySlot[3];
extern unsigned char g_BattlePartyLevelUpFlags[3];
extern unsigned char g_BattlePartyInitialLevels[3];

/* battle_kernel_item_list.cpp */
void __cdecl Kernel_BuildBattleItemList(void);

/* battle_kernel_limit_unlocks.cpp */
int __cdecl Kernel_UpdateLimitUnlocks(struct SavemapCharacter *charRecord, int killDelta, int useDelta);

/* battle_kernel_magic_menu_sort.cpp */
void __cdecl Kernel_SortMagicMenuOrder(int partySlot);

/* battle_limit_fx_thunks.cpp */
int __cdecl Battle_LimitFxThunk76(int context, int param);
int __cdecl Battle_LimitFxThunk71(int context, int param);
int __cdecl Battle_LimitFxStatusOverlayThunk(int context, int param);
int __cdecl Battle_LimitFxThunk60(int context, int param);
int __cdecl Battle_LimitFxThunk70(int context, int param);

/* battle_effect_summon_actors.cpp */
int __cdecl Battle_LoadEffectModelCategory5(int blendMode, int loadId);
void __cdecl Battle_RunSummonEffect(void);
void __cdecl Battle_SummonEffectTask(void);
void __cdecl Battle_SummonFreezeNonTargetActors(void);
void __cdecl Battle3d_Slot3ActorWaitEffectTask(void);
void __cdecl Battle_ResolveEnemyActorFlagsBySelectedMask(void);
void __cdecl Battle3d_Slot3ActorTask(void);
void __cdecl Battle3d_Slot3ActorTimedTask(void);
void __cdecl Battle_RunExtraActorScript(void);
void __cdecl Battle_ResetEffectModelActor(void);
void __cdecl Battle_LoadAndFreeSummonEffectModels(void);
void __cdecl Battle_LoadAndFreeLimitEffectModels(void);
extern struct BattleStatusOverlayOffsets g_BattleStatusOverlayOffsets;

/* battle_effect_model_fade_draw.cpp */
void *__cdecl Battle_LoadEffectModelCategory4(int groupIndex, unsigned int blendMode, int loadId);
void __cdecl BattleFx_DrawModelWithFade(int fadeLevel, struct PolygonTransform *transform, int animFrame, struct BattleModelBank *modelBank);

/* battle_load_party_actors.cpp */
void __cdecl Battle_LoadPartyActorsFromEquipment(void);
void __cdecl Battle_RunPartyAiInitEvents(void);
void __cdecl Battle_ResetLimitBreakSlots(int actorIndex);
void __cdecl Kernel_MarkUsableLimits(int charId, int learnedMask, struct BattleLimitSlots *limitSlots);
int __cdecl Kernel_CollectEquippedMateriaType7Bits(struct SavemapCharacter *charRecord);
int __cdecl Kernel_GetMateriaType7Bits(int materiaSlot);
void __cdecl Battle_CopyPartyStatsToCombatant(struct BattlePartyStatWords *partyStats, struct BattlePartyActor *partyActor, struct BattleCombatant *combatant);

/* battle_load_scene_rounds.cpp */
void __cdecl Battle_LoadSceneFirstRound(int formationId);
void __cdecl Battle_LoadSceneNextRound(int formationId);
void __cdecl Battle_SeedInitialAtbGauges(void);
void __cdecl Battle_SetSpeedFromConfig(int speedConfig);

/* battle_lzs_decompress.cpp */
void __cdecl LzsDecompress(void *inputStream, void *output);

/* battle_magic_screen_shatter.cpp */
extern struct BattleFxOffscreenSlotPair g_BattleMagicScreenShatter_OffscreenSlots;
extern int g_BattleMagicScreenShatter_ScaleVector[4];
void __cdecl BattleFx_MagicEnterEscape(unsigned int targetMask, int actingActor);
void BattleFx_MagicBeginScreenShatter(unsigned int targetMask, int actingActor);
void __cdecl BattleFx_MagicScreenShatterAcquireOffscreenRequests(void);
void __cdecl BattleFx_MagicScreenShatterInitTileGrid(void);
void __cdecl BattleFx_MagicScreenShatterTimelineTask(void);
void __cdecl BattleFx_MagicScreenShatterSetStageAndActorFlagsTask(void);

/* battle_random_source_helpers.cpp */
unsigned char __cdecl Battle_NextRandomByteWrapper(void);
unsigned short __cdecl Battle_AiVm_Random16(void);
int __cdecl Battle_Roll1To100(void);
int __cdecl Battle_PopCount16(unsigned short mask);

/* battle_pick_random_attacks.cpp */
int __cdecl Battle_PickRandomFixedAttackId(void);
int __cdecl Battle_PickRandomMagicAttackId(void);
int __cdecl Battle_PickRandomSummonAttackId(void);

/* battle_recompute_actor_masks.cpp */
void __cdecl Battle_RecomputeActorMasks(void);

/* battle_actor_anim_effect_tasks.cpp */
extern char g_VincentGallDat[9];
extern char g_VincentDeathgigDat[13];
extern char g_VincentHellmaskDat[13];
extern char g_VincentChaosDat[10];
extern char g_VincentDat[12];
extern char g_Vincent2Dat[13];
extern char g_Vincent3Dat[13];
extern int g_VincentModelFilenames[7];
extern short g_BattleBoneParticleBurstParam[16];
extern int g_BattleSkipFreeEffectGroupTag0;
extern int g_BattleAssetSourceModeSelector;
extern unsigned char g_BattleBuiltinAnimScript2E[15];
extern unsigned char g_BattleBuiltinAnimScript2F[5];
extern unsigned char g_BattleBuiltinAnimScript30[16];
extern unsigned char g_BattleBuiltinAnimScript3A[1];
extern unsigned char g_BattleBuiltinAnimScript3B[2];
extern unsigned char g_BattleBuiltinAnimScript39[7];
extern unsigned char g_BattleBuiltinAnimScript34[8];
extern unsigned char g_BattleBuiltinAnimScript31[12];
extern unsigned char g_BattleBuiltinAnimScript32[12];
extern unsigned char g_BattleBuiltinAnimScript38[12];
extern unsigned char g_BattleBuiltinAnimScript37[12];
extern unsigned char g_BattleBuiltinAnimScript36[12];
extern unsigned char g_BattleBuiltinAnimScript35[8];
extern int g_BattleRestingYArrayData;
extern void (__cdecl *g_BattleFxOverlayCallbacks[322])(void);
extern short g_BattleFxOverlayResourceMap1[70];
extern short g_BattleFxOverlayResourceMap2[54];
extern short g_BattleFxOverlayResourceMap3[24];
extern short g_BattleFxOverlayResourceMap4[18];
extern short g_BattleFxOverlayResourceMap5[158];
extern short g_BattleFxOverlayResourceMap6[80];
extern short g_BattleFxOverlayResourceMap7[128];
extern unsigned char g_BattleLimitAnimBraverDat[11];
extern unsigned int g_BattleLimitAnimBankPointers[80];
extern int g_VincentLimitModelIndex;
extern int g_BattleAnimTextureSetHandles[2];
extern int g_BattleAnimTextureSetCount;
extern struct BattleSkeletonTextureRecord *g_BattleAnimTextureSetRecords[2];
extern char g_BattleAnimTransformActive;
extern char g_BattleAnimPreloadReady;
extern char g_BattleAnimPreloadActor;
extern char g_BattleEffectOpStalled;
extern char g_BattleAnimTextureSetLoaded;
extern struct ThreadRecord *g_BattleMagicEffectThread;
void __cdecl Battle_ResetAnimDebugFlags(void);
void __cdecl Battle_ClearActorNodeTranslation(void *unused, int unusedB, short actorIndex);
void __cdecl Battle_ClearAuxNodeTranslation(void *unused, int unusedB, short slotIndex);
char __cdecl Battle_ReturnOne(void);
void __cdecl Battle_CollectSkeletonTextureSets(int unused, struct BattleSkeletonTextureHeader *skeleton);
void __cdecl Battle_RunActorAnimScript(short actor, int scriptTable, int animMode, int animFlags);
int __cdecl Battle_IsWeaponAnimIndex(int partySlot, int animIndex);
void __cdecl Battle_AdvanceActorAnimation(short actor, int animMode, int animFlags);
int __cdecl Battle_ScriptFetchWord(short actor, char *script);
void __cdecl Battle_PlaceActorInFrontOfActor(char actor, char anchor, short distance, short height);
void __cdecl Battle_SnapActorToSpawnPos(char actor);
void __cdecl Battle_EffectTaskRaiseScriptBusy(void);
void __cdecl Battle_SpawnActorMoveToTargetTask(short actor, char *unused, short distance, BattleTaskFn callback, short duration, short *targetPos);
void __cdecl Battle_RestartActorScript(short actor);
void __cdecl Battle_ApplyVmFlagArrayBit0(void);
void __cdecl Battle_EffectTaskFireEventAndRetire(void);
void __cdecl BattleFx_EnterBuiltinHitReaction(int targetMask, int actorIdx);
void __cdecl Battle_EffectTaskDelayedWhiteFlash(void);
void __cdecl Battle_ResetActorScaleAxes(short actor);
void __cdecl Battle_SpawnStatusPoseTask(short actor, char poseKind, char param);
void __cdecl Battle_SpawnPoseTaskFromHud(short actor, char poseArg);
void __cdecl Battle_EffectTaskRunUvScrollScript(void);
void __cdecl Battle_MainTaskSpawnDelayedEffectPair(void);
void __cdecl Battle_HitReactionDelayedCommitTask(void);
void __cdecl Battle_SpawnEffectPairForActor(void);
void __cdecl Battle_HitReactionTask(void);
void __cdecl Battle_CompleteCurrentMainTask(void);
void __cdecl Battle_OverlayTaskSpawnHitMark(void);
void __cdecl Battle_OverlayTaskTranslateY(void);
void __cdecl Battle_OverlayTaskRotateYaw(void);
void __cdecl Battle_OverlayTaskTranslateXZ(void);
void __cdecl Battle_OverlayTaskApproachTarget(void);
void __cdecl Battle_SpawnReturnToSpawnPosTask(short actor, short duration, short hold);
void __cdecl Battle_OverlayTaskReturnToSpawnPos(void);
void __cdecl Battle_OverlayTaskHopArc(void);
void __cdecl Battle_OverlayTaskLiftThenTravel(void);
void __cdecl Battle_OverlayTaskTravelThenDrop(void);
void __cdecl Battle_EffectTaskDelayedHudPulse(void);
void __cdecl Battle_EffectTaskDelayedEvent(void);
void __cdecl Battle_EffectTaskDelayedActorEffect(void);
void __cdecl Battle_StopMagicEffectThread(void);
void __cdecl Battle_StartMagicEffectThread(void (__cdecl *effect)(void));
void __cdecl Battle_PollMagicEffectThread(void);
void __cdecl Battle_WaitAsyncLoadsIdle(void);
void __cdecl Battle_PollMagicEffectThreadDefault(void);
void __cdecl Battle_EmptyDebugLoop(void);
void __cdecl Battle_UnfrogPartyActorStub(char actor);
void __cdecl BattleFx_SpecialEnterLastBossStage(int summonId);
void __cdecl Battle_DelayedFadeOutMusicTask(void);
void __cdecl Battle_RunActorEffectImpact(char actor);
void __cdecl Battle_RunActorEffect(char actor);
void __cdecl Battle_LimitEffectResultStub(char actor, int limitResult);
void __cdecl Battle_SelectEffectOverlayProgram(char actor);
void __cdecl Battle_PreloadEffectResources(void (__cdecl *resource)(void));

/* battle_command_ring_atb.cpp */
extern int g_BattleCommandRingHead;
extern int g_BattleFrameCount;
extern int g_BattleAtbFreezeAccumulator;
extern int g_BattleCommandRingTail;
extern int g_BattleAtbFreezeFlag;
void __cdecl Battle_ResetCommandRing(void);
int __cdecl Battle_GetSetAtbFreezeFlag(int mode);
void __cdecl Battle_TickAtbAndGauges(void);
int __cdecl Battle_IsAtbFrozen(void);
void __cdecl Battle_EnqueueAction(void *action);
void __cdecl Battle_ServiceTurnQueueAndEscape(void);
void __cdecl Battle_PostCommand(short actor, short commandId, short target, short extra);
void __cdecl Battle_SnapshotHpMp(void);
int __cdecl Battle_ResolveCommandingActor(int actor);

/* battle_pending_drops.cpp */
extern int g_BattleDropRingTail;
extern int g_BattleDropRingHead;
extern unsigned char g_BattleDropRing[32];
void __cdecl Battle_ResetPendingDrops(void);
void __cdecl Battle_ClearPendingDropRecord(short actor, short item);
void __cdecl Battle_ClaimPendingDropRecord(short actor, short item);
void __cdecl Battle_ReleasePendingDrops(short actor);
void __cdecl Battle_CollectDropsToInventory(void);

/* battle_ai_script_dispatch.cpp */
int __cdecl Battle_FindAiScript(unsigned char *tableBase, int index, int subIndex);
void __cdecl Battle_RunFormationAiEvents(void);
void __cdecl Battle_SetPartyAiEntityOverride(int partyIndex, int override);
extern unsigned char g_BattlePartyAiEntityOverrides[4];

/* battle_camera_channel_scripts.cpp */
void __cdecl Battle_RunCameraChannels(short scriptArg, short subArg);
void __cdecl Battle_PublishAutoCamera(void);
void __cdecl Battle_PublishFixedPlacementCamera(void);
void __cdecl Battle_PublishCameraChannel(short channel);
void __cdecl Battle_CameraEyeOrbitTask(void);
int __cdecl Battle_CameraEyeFetchWord(char *scriptPtr, char slot);
int __cdecl Battle_CameraFocusFetchWord(char *scriptPtr, char slot);
void __cdecl Battle_CameraEyeFollowBoneTask(void);
void __cdecl Battle_CameraFocusFollowBoneTask(void);
int __cdecl Battle_CameraApplyFocusHeightBias(int height, char actorId);
int __cdecl Battle_CopySignOf(int signSource, int magnitude);
void __cdecl Battle_CameraEyeMoveTask(void);
void __cdecl Battle_CameraEyeMoveTaskStep(short taskSlot);
void __cdecl Battle_CameraFocusMoveTask(void);
void __cdecl Battle_CameraFocusMoveTaskStep(short taskSlot);
void __cdecl Battle_CameraScaleAndRotateOffset(short actorIndex, char slot, char useAltAngle);
void __cdecl Battle_SettleAutoCameraChannel(void);
int __cdecl Battle_CameraCosineEase(int endValue, int startValue, int duration, int elapsed);

/* battle_end_sequence.cpp */
void __cdecl Battle_RunEndSequence(void);

/* battle_opening_actor_setup.cpp */
void __cdecl Battle_PlaySfxPannedToActor(short sfxId, short actor, int fixedPan);
extern struct OpeningPositions162 g_BattleOpeningPartyPositions_Default;
extern struct OpeningPositions162 g_BattleOpeningPartyPositions;
extern struct OpeningPositions108 g_BattleOpeningEnemyPositions_Default;
extern struct OpeningPositions108 g_BattleOpeningEnemyPositions;
extern struct OpeningFacing54 g_BattleOpeningPartyFacing_Default;
extern struct OpeningFacing36 g_BattleOpeningEnemyFacing_Default;
extern char g_szCloudDat[10];
extern char g_szBarrettDat[12];
extern char g_szTifaDat[9];
extern char g_szEarithDat[11];
extern char g_szRed13Dat[10];
extern char g_szYufiDat[9];
extern char g_szKetcyDat[10];
extern char g_szVinsentDat[12];
extern char g_szCid1Dat[9];
extern char g_szSefirosDat[12];
extern char g_szBarrett2Dat[13];
extern char g_szBarrett3Dat[13];
extern char g_szBarrett4Dat[13];
extern char g_szVinsent2Dat[13];
extern char g_szVinsent3Dat[13];
extern char g_szHiCloudDat[12];
extern unsigned int g_pPartyModelFilenames[17];
void __cdecl Battle_RunOpeningSequence(void);
void __cdecl Battle_RunNextRoundSequence(void);
void __cdecl Battle3d_DebugDumpSceneStub(void);
void __cdecl Battle_InitOpeningUiState(void);
void __cdecl Battle3d_LoadStageAndCameraData(void);
int __cdecl Battle_GetEnemyBankSetupModelId(int bankIndex);
void __cdecl Battle_ReloadAllEnemyModelBanks(void);
void __cdecl Battle_EnemyBankLoadStep1(void);
void __cdecl Battle_EnemyBankLoadStep2(void);
void __cdecl Battle_EnemyBankLoadStep3(void);
int __cdecl Battle_LoadModelIntoBankSlot(int loadValue, int auxValue, int bankSlot, int flags);
void __cdecl Battle_LoadEnemyModelIntoBankSlot(int loadValue, int bankSlot, int flags);
void __cdecl Battle_LoadFrogModelBank(void);
void __cdecl Battle_PollEffectThreadThenLoadPartyModels(void);
void __cdecl Battle_LoadFirstTwoPartyModels(void);
void __cdecl Battle_LoadThirdPartyModel(void);
void __cdecl Battle_FinishPartyModelLoadChain(void);
void __cdecl Battle_StartAllActorScripts(void);
void __cdecl Battle_StartEnemyActorScripts(void);
void __cdecl Battle_SetupRoundActors(void);
void __cdecl Battle_ResetSequenceStateFromSetup(void);
void __cdecl Battle_BuildFormationTable(void);
int __cdecl Battle_IsEnemyTypeUsed(int enemyType);
void __cdecl Battle3d_ResetStageNodes(void);
void __cdecl Battle_PlacePartyActors(void);
void __cdecl Battle_ShiftPartyOpeningPositionZForFlag(void);
void __cdecl Battle_SetPartyFacingFromLayout(void);
void __cdecl Battle_PlaceEnemyActors(void);
void __cdecl Battle_BindEnemyActors(void);
void __cdecl Battle_TouchPartyStatBlock(int partySlot);
void __cdecl Battle_EnemyBankLoadStepNoOp(int step);
void __cdecl Battle_SetPartyActorRootParent(int actorIndex);
void __cdecl Battle_SetActorFadingFlags(short actorIndex);
void __cdecl Battle_ProcessActorFadeRequests(short actorId);
void __cdecl Battle_OverlayTaskFadeOutAndHide(void);
void __cdecl Battle_OverlayTaskFadeInActor(void);
void __cdecl Battle_ResetActorRange(int first, int end);
void __cdecl Battle_RunSequenceStep(void);
void __cdecl Battle_BindSequenceEffectToActor(void);
void __cdecl Battle3d_ClearHitDisplayTable(void);
void __cdecl Battle3d_PushHitDisplayEntry(void);
void __cdecl Battle_BuildEffectTargetMasks(void);
void __cdecl Battle_Step3dFrame(void);
void __cdecl Battle3d_PollEffectThread(void);
void __cdecl Battle3d_ProfileSectionMark(int section);
void __cdecl Battle3d_UpdateCameraNode(void);
void __cdecl Battle_Update3dScene(void);
void __cdecl Battle3d_RunPendingActorEffect(int flag);
void __cdecl Battle3d_SyncActorIdlePoses(void);
void __cdecl Battle3d_QueueAllActorDraws(void);
void __cdecl Battle3d_HandleEndOfBattleTransitions(void);
short __cdecl Battle_FindActorInEffectOrder(int actorId);
void __cdecl Battle3d_DrainSceneMessageQueue(void);
void __cdecl Battle_SpawnHitReactionTaskForActor(int actor);
void __cdecl Battle_SpawnHitReactionTaskForEffectOrderEntry(int actor, int hitRecord);
void __cdecl Battle_InitHitReactionTaskFromHitRecord(int recordIndex, int taskSlot);
void __cdecl Battle_RestoreActorAfterEffect(char actorId);
void __cdecl Battle_RunPartyActorScripts(void);
void __cdecl Battle_ServiceActorPendingFade(short actorId);
void __cdecl Battle_RunEnemyActorScripts(void);
void __cdecl Battle_LatchActorProjectedPoints(void);
void __cdecl Battle3d_UpdateAndDrawStage(void);
void __cdecl Battle3d_DrawScrollingStageLayer(char layerIndex, int scrollStep, int resetValue, int wrapLimit);
void __cdecl Battle3d_TickStageFlicker(void);
void __cdecl Battle3d_TickStageOscillator(int index);
void __cdecl Battle3d_DrawStagePart(int part, int otEntry, int depthShift, int flags);
void __cdecl Battle3d_QueuePartyActorDraws(void);
void __cdecl Battle3d_QueueEnemyActorDraws(void);
struct GfxDrawSpec;
struct GfxPolygonSet;
struct PolygonBgraColor;
void __cdecl Battle3d_ShadowBeginCallback(void);
void __cdecl Battle3d_ShadowVertexAlphaCallback(struct PolygonBgraColor *vertex, struct GfxDrawSpec *srcVertex);
void __cdecl Battle_DrawActor(int actorIndex);
void __cdecl Battle3d_PushActorNodeMatrices(int actorIndex);
void __cdecl Battle3d_ComposeNodeWithParent(struct Battle3dNode *node);
void __cdecl Gte_ScaleAndInstallMatrix(void *matrix, void *scaleVec);
void __cdecl Battle3d_SetPolygonSetHook(int pass, struct GfxPolygonSet *polySet);
void __cdecl Battle3d_DrawActorWeapon(int actorIndex);
void __cdecl Battle3d_DrawActorSubModel(int actorIndex);
void __cdecl Battle_EmptyNodeSetupStub(short actorSlot, int nodeId);
void __cdecl Battle_InitActorNodeChain(short actorIndex, int mode, int variant);
void __cdecl Battle_SetNodeParentMatrix(void *parentMatrix, struct Battle3dNode *node);
void __cdecl Battle_ApplySequenceCamera(void);
void __cdecl Battle3d_BuildNodeMatrixFromCamera(struct Battle3dNode *node, struct PsxMatrix *outMatrix, struct Vec3sPacked *eye, struct Vec3sPacked *lookAt);
void __cdecl Battle_RequestEndMusicWin(void);
void __cdecl Battle_RequestEndMusicLose(void);
void __cdecl Battle_StopMusicAndPlayPending(void);
void __cdecl Battle_RestoreSfxMasterVolume(void);
void __cdecl Battle_StopAllSfxAndRestoreVolume(void);
void __cdecl Battle_FadeOutMusic(void);
void __cdecl Battle_PreloadAndPlaySfx(int sfxId);
void __cdecl Battle_PlaySfxCentered(int sfxId);
void __cdecl Battle_ForceEffectOverrideStep(void);

/* battle_kernel_materia_support.cpp */
extern unsigned char g_MateriaDescribeModeFlag;
extern short g_MateriaEncounterModifiers[];
extern char g_MateriaCommandCount;
extern unsigned char g_MateriaMagicListCount;
extern unsigned char g_MateriaSummonCount;
extern short g_MateriaLevelUpSlotMask[3];
void __cdecl Kernel_UnpackMateriaAndGetLevel(int packed, int *apOut, char *idOut);
void __cdecl Party_RecomputeBattleModifiers(void);
void __cdecl Kernel_ApplyLinkedMateriaPair(int selectorA, int selectorB, int materiaA, int materiaB, int extraA, int extraB, int effectArg);
void __cdecl Kernel_ResolveSupportPairing(int materiaA, int materiaB, unsigned char extraA, unsigned char extraB, unsigned char effectArg);
void __cdecl Kernel_DispatchSupportEffect(int packedMateria, int partnerMateria, unsigned char unused, unsigned char mode);
void __cdecl Kernel_ApplyMateriaElement(char level, int materiaId, char mode);
void __cdecl Kernel_ApplyMateriaStatusMask(char unused, int materiaId, char mode);
void __cdecl Kernel_ApplyMagicCounterSupport(int unused, char level, int partnerWord);
void __cdecl Kernel_ApplyCommandAndMagicTriggerSupport(int unused, char level, int partnerWord);
void __cdecl Kernel_AppendTriggerListEntryFixedCode(char level, int code);
void __cdecl Kernel_AppendTriggerListEntriesForCommandMateria(char level, unsigned char materiaId, int ap);
void __cdecl Kernel_AppendTriggerListEntryFromLevelTable(unsigned char level, unsigned char materiaId, int ap);
void __cdecl Kernel_AppendTriggerListEntryFromDescriptor(unsigned char level, unsigned char materiaId, int ap);
void __cdecl Kernel_ApplyMpTurboSupport(char level, int packedMateria);
void __cdecl Kernel_AddMpTurboToSummonEntries(char amount, char materiaId, int ap);
void __cdecl Kernel_AddMpTurboToAllSummonEntries(char amount);
void __cdecl Kernel_ApplyAbilityFlagSupport(int packedMateria);
void __cdecl Kernel_OrSupportFlagIntoSummonEntry(char materiaId, int ap);
void __cdecl Kernel_OrSupportFlagIntoAllSummonEntries(void);
void __cdecl Kernel_OrSupportFlagIntoAllCommandEntries(void);
void __cdecl Kernel_OrSupportFlagIntoAllMagicEntries(void);
void __cdecl Kernel_ApplyQuadraMagicSupport(char unused, char amount, int packed);
void __cdecl Kernel_AddQuadraMagicToAllMagicEntries(char amount);
void __cdecl Kernel_AddQuadraMagicToSummonEntry(int amount, int materiaId);
void __cdecl Kernel_AddQuadraMagicToAllSummonEntries(char amount);
void __cdecl Kernel_ApplyAllSupport(int unused, int amount, int packed);
void __cdecl Kernel_AddMegaAllLevelToMagicAndCommandCounts(int packed);
void __cdecl Kernel_ApplyMateriaToActor(unsigned int packed);
void __cdecl Kernel_ApplyMateriaStatBonus(int materiaId);
void __cdecl Kernel_ApplyExtraCutMateria(char subtype, char materiaId, int packedValue);
void __cdecl Kernel_GrantExtraCutCommand(unsigned char level, unsigned char materiaId);
void __cdecl Kernel_ApplyMegaAllMateria(char materiaId, int packedValue);
void __cdecl Kernel_ApplyEnemySkillMateria(char unused, char unusedSecond, int skillMask);
void __cdecl Kernel_ApplyMasterCommandMateria(void);
void __cdecl Kernel_ApplyMasterMagicMateria(void);
void __cdecl Kernel_ApplyMasterSummonMateria(void);
void __cdecl Kernel_ApplyIndependentMateria(int selector, char materiaId, int packedValue);
void __cdecl Kernel_ApplyMateriaLongRange(char flag, char opSelector);
void __cdecl Kernel_SetLongRangeFlag(char longRange);
void __cdecl Kernel_ApplyMateriaAbilityFlag(char materiaId, int ap);
void __cdecl Kernel_ApplyMateriaStatPlus(unsigned char materiaId, int ap);
void __cdecl Kernel_AppendCounterAttackTrigger(char level, char materiaId);
void __cdecl Kernel_ApplyMateriaExpBonus(unsigned char materiaId, int ap);
void __cdecl Kernel_ApplyEncounterMateria(char selector, char materiaId, int ap);
void __cdecl Kernel_ApplyMateriaEncounterModifier(unsigned char materiaId, int ap);
int __cdecl Kernel_GetMateriaLevel(unsigned char materiaId, int ap);
void __cdecl Kernel_StoreEncounterModifiersToStatBlock(void);
void __cdecl Kernel_ApplyMateriaStatModifiersToStatBlock(void);
void __cdecl Kernel_CopySummonEntryBytesToStatBlock(void);
void __cdecl Kernel_ApplyCommandMateria(unsigned char materiaId, int ap);
void __cdecl Kernel_BuildBattleCommandMenu(void);
void __cdecl Kernel_RemoveCommand5IfCommand17Present(void);
int __cdecl Kernel_GetBattleMenuSlotForCommand(int commandId);
void __cdecl Kernel_SetBattleMenuCommand(unsigned char commandId, unsigned char slot);
void __cdecl Kernel_CopyMagicListToAbilityEntries(void);
void __cdecl Kernel_AddMagicListEntryIfAbsent(char magicId);
void __cdecl Kernel_SetAbilityEntryFromAttackTable(int attackId, int entry, int attackIndex);

/* battle_time_slice_clock.cpp */
extern int g_BattleSlipBufferIndex;
extern int g_BattleSlipRegenAccumulators[10];
extern int g_BattleSlipSharedTimeAccumulators[2];
extern int g_BattleSlipSecondTimeAccumulators[10];
extern int g_BattleSlipFirstTimeAccumulators[10];
extern int g_BattleSlipSliceBufferA[2][10];
extern int g_BattleSlipSliceBufferB[2][10];
int __cdecl Battle_GetSlipClockElapsed(int actor);
void __cdecl Battle_ApplyTimeSlices(void);
void __cdecl Battle_AccumulateTimeSlices(void);
void __cdecl Battle_UpdatePartyGaugeHud(void);

/* battle_sequence_step_records.cpp */
extern int g_BattleSeqDamageDisplayCursor;
extern int g_BattleSeqStepCursor;
extern int g_BattleSeqHitRecordCursor;
extern int g_BattleSeqStepCursorBackup;
extern int g_BattleSeqHitRecordCursorBackup;
extern int g_BattleSeqPlaybackState;
extern int g_BattleSeqWaitDoneState;
void __cdecl Battle_SnapshotSequenceCursors(void);
void __cdecl Battle_SetSequenceStepMessageTiming(int messageTiming, int animationTiming);
void __cdecl Battle_RestoreSequenceCursors(void);
void __cdecl Battle_PruneSequenceWaitFlags(void);
int __cdecl Battle_AllocDamageDisplayRecord(struct BattleHitRecord *hitRecord);
void __cdecl Battle_PushSequenceStep(int actorIdx, int actionType, int animId, int sequenceId);
void __cdecl Battle_ClearSequenceStepTargetWord(void);
void __cdecl Battle_ResetSequence(void);
void __cdecl Battle_PopSequenceStepUnused(void);
void __cdecl Battle_SignalAtbWaitState(int state);
void __cdecl Battle_PlaySequence(void);
void __cdecl Battle_WaitSequenceDone(void);
void __cdecl Battle_ClearActorAnimHoldFlags(int actorIdx);
void __cdecl Battle_RetargetActorAnimPose(int actorIdx, int oldPose, int newPose, int gateFlag);
void __cdecl Battle_CopyDamageDisplayValuesWithinLimits(short unused, short displayIdx);

/* battle_actor_effect_sprites.cpp */
extern struct BattleActorAnimState *g_BattleActor_BufferPtr;
extern int g_BattleActor_Offset00c06228;
extern struct BattleEffectPrimitive *g_BattleActor_Offset00c06328;
void __cdecl Battle_SpawnActorScaleRampTask(int actorIndex);
void __cdecl Battle_ActorScaleRampEmitterTask(void);
void __cdecl Battle_ActorScaleRampSpriteQuadTask(void);
void __cdecl Battle_SpawnWorldSpriteTask(struct Vec3sPacked *position, short width, short height);
void __cdecl Battle_WorldSpriteTask(void);
void __cdecl Battle_SpawnPositionedEffectTask(int actorId, struct Int64Pair *position, int delay, int width, int height);
void __cdecl Battle_DelayedActorSpriteTask(void);

/* battle_hit_reaction_task_slots.cpp */
void __cdecl Battle_SpawnHitReactionTask(int duration, int targetId, int strength);
void __cdecl Battle_HitReactionStaggerTask(void);
void __cdecl Battle_SpawnScriptTimerTask(int timerId, int timer);
void __cdecl Battle_ScriptTimerTask(void);
void __cdecl Battle_ResetAllTaskSlots(void);
void __cdecl Battle_ResetAuxTaskSlots(void);
void __cdecl Battle_RunMainTaskSlots(void);
void __cdecl Battle_RunOverlayTaskSlots(void);
void __cdecl Battle_RunEffectTaskSlots(void);
void __cdecl Battle_RunAuxTaskSlots(void);

/* battle_special_effect_scaling.cpp */
void Battle_SpecialEffect0F_PickSlotsAttack(void);
void __cdecl Battle_MergePartyStatsIntoActor(void);
void __cdecl Battle_ApplyActorStatMutation(void);
void __cdecl Battle_SpecialEffect0A_ScaleDamageByStatusCount(void);
void __cdecl Battle_SpecialEffect0B_ScaleDamageByBuffCount(void);
void __cdecl Battle_SpecialEffect20_PowerFromMateriaAp(void);
void __cdecl Battle_SpecialEffect0C_ScaleDamageByKoCount(void);

/* battle_target_effect_matrices.cpp */
void __cdecl BattleFx_InstallOrientedMatrixUniform(void *position, void *rotation, short scale, int pushAmount);
void __cdecl BattleFx_InstallOrientedMatrixScaled(void *position, void *rotation, void *scale, int pushAmount);

/* battle_steal_command_effects.cpp */
void __cdecl Battle_TryStealOnHit(void);
void __cdecl Battle_ApplySupportMateriaFlags(int supportBits);
void __cdecl Battle_FindAbilitySlotForCommand(int partyIndex);
void __cdecl Battle_QueueCommandEffectStep(int stepId);
int __cdecl Battle_GetDefaultHitReactionPose(void);
void __cdecl Battle_TryMorphReward(void);
void __cdecl Battle_QueueFollowUpAttack(int attackId);

/* battle_write_party_rewards.cpp */
void __cdecl Battle_OrBitsIntoEquippedMateriaType7(struct SavemapCharacter *record, int bits);
void __cdecl Battle_GrantHalfExpToNonPartyCharacters(int actorMask);
void __cdecl Battle_ResetResultRewardTotals(void);

/* lasboss3.cpp */
extern unsigned char g_SuperNova_Keyframe0VertexPositions[4992];
extern unsigned char g_SuperNova_Keyframe1VertexPositions[4992];
extern unsigned char g_SuperNova_Keyframe2VertexPositions[4992];
extern unsigned char g_SuperNova_Keyframe3VertexPositions[4992];
extern unsigned char g_SuperNova_Keyframe4VertexPositions[4992];
extern unsigned char g_SuperNova_Keyframe5VertexPositions[4992];
extern unsigned char g_SuperNova_Keyframe6VertexPositions[4992];
extern unsigned char g_SuperNova_Keyframe7VertexPositions[4992];
extern int g_SuperNova_VariantModelHandoffFlag;
extern short g_SuperNova_TimedSpriteFadeTaskParams[3];
extern short g_SuperNova_PulsingSpriteTaskParams[3];
extern short g_SuperNova_DecelSpriteTaskParams[3];
extern int g_SuperNova_FadingSpinningTaskParams;
extern short g_SuperNova_CounterRotSpriteTaskParams[3];
extern int g_SuperNova_SineOrientedTaskParams;
extern int g_SuperNova_TrailingSpriteTaskParamsA;
extern int g_SuperNova_TrailingSpriteTaskParamsB;
extern int g_SuperNova_BillboardSpriteTaskParams;
extern int g_SuperNova_SineScaledModelTaskParams;
extern short g_SuperNova_FixedPoseModelPairTaskParams[3];
extern int g_SuperNova_ShatteringModel27TaskParams;
extern short g_SuperNova_GrowingTumblingTaskParams[3];
extern short g_SuperNova_ProjectedQuadTaskParams[3];
extern int g_SuperNova_PointFieldBackdropTaskParams;
extern int g_SuperNova_InitStateTaskHandle;
void __cdecl BattleFx_WeaponLoadResSuperNova(void);
void __cdecl BattleFx_WeaponEnterSuperNova(int stateParam1, int stateParam2);
void __cdecl BattleFx_SuperNovaInitState(int stateParam1, int stateParam2);
void __cdecl BattleFx_SuperNovaWaitForAngleSpreadTask(void);
void __cdecl BattleFx_SuperNovaSpawnCameraScript(int scriptWords, int matrixPtr, int delay);
void __cdecl BattleFx_SuperNovaRunCameraScript(void);
short __cdecl BattleFx_SuperNovaReadCameraScriptOperand(void *pScriptPtr);
int __cdecl BattleFx_SuperNovaApplyCameraEasing(int value, int easingParam);
void __cdecl BattleFx_SuperNovaMainTask(void);
void __cdecl BattleFx_SuperNovaWhiteFlashFadeOutTask(void);
void __cdecl BattleFx_SuperNovaWhiteFlashPulseTask(void);
void __cdecl BattleFx_SuperNovaApproachingPointFieldTask(void);
void __cdecl BattleFx_SuperNovaClaimApproachingPointRecord(void);
void __cdecl BattleFx_SuperNovaBeginTriangleRanking(int meshSourcePtr, struct Vec3sPacked *centerPosPtr);
void __cdecl BattleFx_SuperNovaRankTrianglesByDistanceTask(void);
void __cdecl BattleFx_SuperNovaTimedSpriteFadeTask(void);
void __cdecl BattleFx_SuperNovaPulsingSpriteEmitterTask(void);
void __cdecl BattleFx_SuperNovaPulsingSpriteTask(void);
void __cdecl BattleFx_SuperNovaDeceleratingSpriteEmitterTask(void);
void __cdecl BattleFx_SuperNovaDeceleratingSpriteTask(void);
void __cdecl BattleFx_SuperNovaFadingSpinningModelTask(void);
void __cdecl BattleFx_SuperNovaCounterRotatingSpritePairTask(void);
void __cdecl BattleFx_SuperNovaFadingSpriteTask(void);
void __cdecl BattleFx_SuperNovaDampedParticleModelEmitterTask(void);
void __cdecl BattleFx_SuperNovaDampedParticleModelTask(void);
void __cdecl BattleFx_SuperNovaExpandingModelEmitterTask(void);
void __cdecl BattleFx_SuperNovaGravitySpriteParticleTask(void);
void __cdecl BattleFx_SuperNovaStaticModelTask(void);
void __cdecl BattleFx_SuperNovaShatteringModel2BTask(void);
int __cdecl BattleFx_SuperNovaDrawRankedTriangleMesh(struct BattleFxSuperNovaGeometry *geometry, int unusedArg2, int unusedArg3, int unusedArg4, struct Vec3sPacked *displacement, short *selection, void *node);
void __cdecl BattleFx_SuperNovaSeedTriangleDeltas(int meshSourcePtr, short divisor);
void __cdecl BattleFx_SuperNovaStepScatteringTriangles(void);
void __cdecl BattleFx_SuperNovaShatteringModel29Task(void);
void __cdecl BattleFx_SuperNovaFlashModelSequenceTask(void);
void __cdecl BattleFx_SuperNovaSineScaledBillboardModelTask(void);
void __cdecl BattleFx_SuperNovaSineOrientedModelTask(void);
void __cdecl BattleFx_SuperNovaFourPassModelTask(void);
void __cdecl BattleFx_SuperNovaBlendPolygonPartPositionThirds(int vertexIndex, int unusedArg2, struct BattleFxSuperNovaPosFixed8 *outPositionsPtr);
void __cdecl BattleFx_SuperNovaTwoModelPolygonPartTask(void);
void __cdecl BattleFx_SuperNovaTrailingSpritesTask(void);
void __cdecl BattleFx_SuperNovaRelocatingModelTask(void);
void __cdecl BattleFx_SuperNovaIndexedModelTask(void);
void __cdecl BattleFx_SuperNovaBillboardSpriteTask(void);
void __cdecl BattleFx_SuperNovaSineScaledModelTask(void);
void __cdecl BattleFx_SuperNovaSineScaledModelWithShortTask(void);
void __cdecl BattleFx_SuperNovaSineScaledRotatedModelTask(void);
void __cdecl BattleFx_SuperNovaBlendPolygonPartPositionFixed8(int vertexIndex, int unusedArg2, struct BattleFxSuperNovaPosFixed8 *outPositionsPtr);
void __cdecl BattleFx_SuperNovaGrowingMorphingModelTask(void);
void __cdecl BattleFx_SuperNovaVariantModelHandoffTask(void);
void __cdecl BattleFx_SuperNovaEasingBillboardModelTask(void);
void __cdecl BattleFx_SuperNovaFixedPoseModel4Task(void);
void __cdecl BattleFx_SuperNovaFixedPosePreloadedModelTask(void);
void __cdecl BattleFx_SuperNovaFixedPoseModelPairTask(void);
void __cdecl BattleFx_SuperNovaFadingSpriteBlendTask(void);
void __cdecl BattleFx_SuperNovaShatteringModel27Task(void);
void __cdecl BattleFx_SuperNovaSpinningModelPairTask(void);
void __cdecl BattleFx_SuperNovaGrowingTumblingModelTask(void);
void __cdecl BattleFx_SuperNovaRisingFadingSpriteTask(void);
void __cdecl BattleFx_SuperNovaProjectedQuadTask(void);
void __cdecl BattleFx_SuperNovaInitPointField(void);
void __cdecl BattleFx_SuperNovaDrawPointFieldTask(void);
void __cdecl BattleFx_SuperNovaPointFieldBackdropModelTask(void);
void __cdecl Battle3d_EffectSpinStageNode3Task(void);
void __cdecl BattleFx_SuperNovaStepScatteringTrianglesZOnly(void);
void __cdecl Battle3d_ComposeEffectNode(struct Battle3dNode *node);
void __cdecl Battle3d_SubmitEffectPrimitiveAtNode(int primitiveIndex, int unusedArg2, int unusedArg3, int renderFlags, void *node);
int __cdecl Battle3d_DrawShrinkingSpinningTriangles(struct BattleFxShrinkGeometry *geometry, int unusedArg2, int unusedArg3, int unusedArg4, struct Vec3sPacked *displacement, short *selection, void *node);
int __cdecl BattleFx_SuperNovaLoadStage57Dat(void);

/* coloss.cpp */
unsigned int __cdecl Battle_RunArenaNextBattleChooser(void);

/* init.cpp */
void __cdecl Kernel_ResetLoadState(void);
extern int g_BattleLoadState;
extern int g_BattleRewardExp;
extern int g_BattleRewardAp;
int __cdecl Kernel_LoadKernelBinFiles();
void __cdecl Kernel_ParseKernelBin(void);

/* inits.cpp */
void __cdecl Battle_LoadSceneRecord(int sceneId, void *callback);
int __cdecl Battle_FindSceneFileChunk(int blockIndex);
int __cdecl Battle_ScaleStatBy125Clamped(int stat);

/* yama_swirl_transition_module.cpp */
void __cdecl Swirl_WaitFrameDuration(struct GfxEngineContext *context);
void __cdecl Swirl_ModuleEnter(struct GfxEngineContext *ctx);
void __cdecl Swirl_InitGridAndPackets(void);
void __cdecl Swirl_ModuleExit(void);
void __cdecl Swirl_ModuleFrame(struct GfxEngineContext *context);
void __cdecl Swirl_SetupSpriteRecords(struct GfxEngineContext *context);
void __cdecl Swirl_UpdateAndDrawGrid(struct GfxEngineContext *context, int active);
void __cdecl Swirl_DrawPacketQuad(char *packet, int context);
void __cdecl Swirl_ModuleKeyboard(void);
void __cdecl Swirl_ModuleMouse(void);

/* deadsef.cpp */
extern unsigned char g_SaferSephirothDeath_SoundGroup[16];
extern struct SaferSephirothDeath_BlackModelDrawReq g_SaferSephirothDeathBlackModelDrawRequest;
extern unsigned char g_SaferSephirothDeathCircleRsdName[12];
extern int g_SaferSephirothDeath_SubTaskCount;
extern int g_SaferSephirothDeath_BillboardSpawnAccum;
extern int *g_SaferSephirothDeath_ModelTablePtr;
extern struct PsxMatrix g_SaferSephirothDeath_TransformMatrix;
extern struct Vec3sPacked g_SaferSephirothDeath_BoneWorldPos;
extern struct Vec3sPacked g_SaferSephirothDeath_ActorPos;
extern struct PsxMatrix g_SaferSephirothDeath_SavedMatrix;
extern int g_SaferSephirothDeath_TaskFinished;
void __cdecl BattleFx_SpecialLoadResSaferSephirothDeath(void);
void __cdecl BattleFx_SaferSephirothDeathLoadAndBegin(int targetMask, int actorIdx);
void __cdecl BattleFx_SaferSephirothDeathSpawnMainTaskAndCamera(int targetMask, int actorIdx);
void __cdecl BattleFx_SaferSephirothDeathSpawnCameraScript(short *scriptAddr, struct PsxMatrix *matrixAddr, int cameraFlag);
void __cdecl BattleFx_SaferSephirothDeathRunCameraScript(void);
short __cdecl BattleFx_SaferSephirothDeathReadCameraScriptOperand(short **pScriptPtr);
int __cdecl BattleFx_SaferSephirothDeathApplyCameraEasing(int value, int easingParam);
void __cdecl BattleFx_SaferSephirothDeathGetActorLocalPosition(int actorIdx, int boneIdx, struct BattleFxActorMatrix *pMatrix, struct Vec3sPacked *pOutPos);
void __cdecl BattleFx_SaferSephirothDeathTimelineTask(void);
void __cdecl BattleFx_SaferSephirothDeathBuildShatterFragments(void);
int __cdecl BattleFx_SaferSephirothDeathExtractModelFragments(struct SaferSephirothDeathModelPart *modelData, int fragIndex);
void __cdecl BattleFx_SaferSephirothDeathShatterDrawTask(void);
int __cdecl BattleFx_SaferSephirothDeathDrawFragmentsPsxMesh(struct BattleFxSaferSephirothDeathMeshDraw *meshDraw, int orderingTable, int otShift, int drawListHead, struct SVECTOR **vertexOutPtr, struct SVECTOR **offsetCursorPtr, short **shadeCursorPtr);
void __cdecl BattleFx_SaferSephirothDeathStepFragmentMotion(int frameCount);
void __cdecl BattleFx_SaferSephirothDeathFlashSequenceTask(void);
void BattleFx_SaferSephirothDeathSpawnFadingModelsTask(void);
void __cdecl BattleFx_SaferSephirothDeathFadingModelTask(void);
void __cdecl BattleFx_SaferSephirothDeathSubmitFadingModelDraw(int alpha);
void BattleFx_SaferSephirothDeathSpawnBillboardsTask(void);
void __cdecl BattleFx_SaferSephirothDeathBillboardTask(void);
void __cdecl BattleFx_SaferSephirothDeathWhiteFlashRampTask(void);
int __cdecl BattleFx_SaferSephirothDeathLoadEnemy331ModelTable(void);

/* battle_actor_overlay_draw.cpp */
extern struct BattleHitReactionRecord g_BattleHitReactionRecords[5];

/* Shared symbolic aliases for storage objects */
#define g_BattleItemListBlock DAT_009ac110

/* sting.cpp */
void __cdecl BattleFx_WeaponLoadResRubyTentacles(void);
void __cdecl BattleFx_WeaponEnterRubyTentaclesWithCameraScript(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponEnterRubyTentaclesNoCameraScript(int targetMask, int actorIdx);
void __cdecl BattleFx_WeaponBeginRubyTentaclesWithCameraScript(int targetMask, int actorIdx);
void __cdecl BattleFx_RubyTentaclesSpawnCameraScript(void *scriptPtr, void *matrixPtr, int value);
void __cdecl BattleFx_RubyTentaclesRunCameraScript(void);
short __cdecl BattleFx_RubyTentaclesReadCameraScriptOperand(int *cursorPtr);
int __cdecl BattleFx_RubyTentaclesApplyCameraEasing(int value, int easing);
void __cdecl BattleFx_RubyTentaclesGetActorLocalPosition(int actorIndex, int boneIndex, struct BattleFxActorMatrix *rotMatrix, struct Vec3sPacked *out);
extern struct RubyTentaclesBaseModelVertices g_RubyTentaclesBaseModelVertices;
extern unsigned char g_RubyTentaclesInitialModelVertices[9868];
extern unsigned int g_WeaponFxRubyTentaclesSmokeSpriteRecord[5];
extern unsigned char g_RubyTentaclesCameraScript[84];
extern void *g_RubyTentaclesEnemyFileData;
extern int *g_RubyTentaclesEnemyPointerTable;
extern struct GfxTableCursor *g_RubyTentaclesSpriteResourceHandle;
extern int g_RubyTentaclesRotationMatrix[8];
extern struct SVECTOR g_RubyTentaclesRotationAngles;
extern struct GfxRotationAngles2 g_RubyTentaclesModelRotationAngles;
extern int g_RubyTentaclesBurstAccumulator1;
extern int g_RubyTentaclesToggleFlag;
extern int g_RubyTentaclesParticleProgress1;
extern int g_RubyTentaclesParticleProgress2;
extern int g_RubyTentaclesBurstAccumulator2;
extern int g_RubyTentaclesBurstAccumulator3;
void __cdecl BattleFx_RubyTentaclesMainTimelineTask(void);
void __cdecl BattleFx_RubyTentaclesBeginEnvelope(int start, int end, int duration, int step);
void __cdecl BattleFx_RubyTentaclesEnvelopeTask(void);
void __cdecl BattleFx_RubyTentaclesPrepareOffscreenPair(void);
void __cdecl BattleFx_RubyTentaclesSubmitOffscreenPair(void *matrix, struct BattleFxRubyTentaclesOffscreenRecord *record);
void __cdecl BattleFx_RubyTentaclesModelTask(void);
int __cdecl BattleFx_RubyTentaclesDrawPrimitiveList(struct RubyTentaclesModelRenderDescriptor *primList, int orderingTable, int depthShift, int packetCursor);
int __cdecl BattleFx_RubyTentaclesDrawPsxModelPrimitives(struct RubyTentaclesModelRenderDescriptor *modelList, int orderingTable, int depthShift, int packetCursor);
void __cdecl BattleFx_RubyTentaclesComputeVertexDeltas(struct RubyTentaclesBaseModelVertices *srcList, struct SVECTOR *baseVerts, struct SVECTOR *deltaOut);
void __cdecl BattleFx_RubyTentaclesBlendVertexDeltas(struct RubyTentaclesBaseModelVertices *dstList, struct SVECTOR *baseVerts, struct SVECTOR *deltas, int blendWeight);
void BattleFx_RubyTentaclesEmitVertexParticles(void);
void __cdecl BattleFx_RubyTentaclesParticleSpriteTask(void);
void __cdecl BattleFx_RubyTentaclesDrawSprite(void *position, int frameIndex, int size);
int __cdecl BattleFx_RubyTentaclesPlaceSpritePosition(void *position, short stateWord, int offset);
void BattleFx_RubyTentaclesSpawnEffectRecord(short actor);
void __cdecl BattleFx_RubyTentaclesEffectRecordTask(void);
int __cdecl BattleFx_RubyTentaclesDrawRecordAttachedParts(int actorIndex, int drawMode, int shade, struct BattleFxRubyTentaclesMeshPacket *primBuf);
int __cdecl BattleFx_RubyTentaclesDrawPsxMeshPrimitives(struct BattleFxPsxMeshHeader *mesh, int mode, int index, struct BattleFxRubyTentaclesMeshPacket *scale, void *matrix, void *matrix2);
int __cdecl BattleFx_RubyTentaclesSpawnRadialParticleBurst(struct Vec3sPacked *origin, int remaining);
void __cdecl BattleFx_RubyTentaclesRadialSpriteEmitterTask(void);
void __cdecl BattleFx_RubyTentaclesSpawnTimeline(int targetMask, int actorIdx);
void __cdecl BattleFx_RubyTentaclesTimelineTask(void);
void BattleFx_RubyTentaclesSpawnRecordCallback(short actor);
void __cdecl BattleFx_RubyTentaclesRecordTask(void);
void BattleFx_RubyTentaclesSecondaryRadialEmitterTask(void);
int *__cdecl BattleFx_RubyTentaclesLoadEnemy012PointerTable(void);

/* vahamut0.cpp */
extern unsigned char g_BattleSummonBahamutZeroSfxGroup0[16];
extern unsigned char g_BattleSummonBahamutZeroSfxGroup1[16];
extern unsigned char g_BattleSummonBahamutZeroSfxGroup2[16];
extern unsigned char g_BattleSummonBahamutZeroSfxGroup3[16];
extern unsigned char g_BattleSummonBahamutZeroSfxGroup4[16];
extern struct BattleFxBzSpriteParams g_BattleSummonBahamutZeroSpriteParams;
extern struct BattleFxBzSpriteParams g_BattleSummonBahamutZeroTrailSprites[3];
extern struct BattleFxBzTrailOrigin g_BattleSummonBahamutZeroTrailOrigins[3];
extern struct BattleLimitFuryBrandModelReqA g_BattleFxBzDrawRequest0C;
extern struct BattleLimitFuryBrandModelReqA g_BattleFxBzDrawRequest0D;
void __cdecl BattleFx_SummonLoadResBahamutZero(void);
int __cdecl BattleFx_SummonEnterBahamutZero(int targetMask, int actorIdx);
void __cdecl BattleFx_BzBeginEffect(int targetMask, int actorIdx);
void __cdecl BattleFx_BzSpawnCameraScript(void *eyeTrack, void *focusTrack, int duration);
void __cdecl BattleFx_BzRunCameraScript(void);
short __cdecl BattleFx_BzFetchScriptCount(short **index);
int __cdecl BattleFx_BzEaseFixed(int value, int easeMode);
void __cdecl BattleFx_BzAttachPointToLocal(int actorIndex, int boneIndex, struct BattleFxActorMatrix *matrix, struct Vec3sPacked *localOffset);
void __cdecl BattleFx_BzMasterTimeline(void);
void __cdecl BattleFx_BzResetSubTaskTable(void);
void __cdecl BattleFx_BzRunSubTasks(void);
void __cdecl BattleFx_BzDrawSlot04And05(void);
void __cdecl BattleFx_BzDrawSlot11(void);
void __cdecl BattleFx_BzDrawSpriteTrail(void);
void *__cdecl BattleFx_BzEmitSprite(struct BattleFxBzSpriteParams *spriteParams, float blendFactor, int value);
void __cdecl BattleFx_BzSpawnSlot06And07(void);
void __cdecl BattleFx_BzDrawSlot06(void);
void __cdecl BattleFx_BzDrawSlot07(void);
void __cdecl BattleFx_BzDrawGlowSprite(void);
void __cdecl BattleFx_BzSpawnRingDebris(void);
void __cdecl BattleFx_BzDrawSlot08(void);
void __cdecl BattleFx_BzDrawSlot09(void);
void __cdecl BattleFx_BzSpawnSlot0AAnd0B(void);
void __cdecl BattleFx_BzDrawSlot0A(void);
void __cdecl BattleFx_BzDrawSlot0B(void);
void __cdecl BattleFx_BzSpawnSparkSwarm(void);
void __cdecl BattleFx_BzDrawSparkSwarm(void);
void *__cdecl BattleFx_BzEmitTilePrim(void);
void __cdecl BattleFx_BzDrawSlot0CTo10(void);
void __cdecl BattleFx_BzDrawSlot0E(void);
void __cdecl BattleFx_BzDrawSlot0F(void);
void __cdecl BattleFx_BzEmitScreenPrim(void);
void __cdecl BattleFx_BzDrawSkyDomeTask(void);
struct BattleFxBzMorphDesc;
void *__cdecl BattleFx_DrawMorphedLmdOuter(struct BattleFxBzMorphDesc *desc, void *unusedOt, int unusedZero, void *unusedPrimCursor, void *morphVertexBase, int morphT4096);
void *__cdecl BattleFx_DrawMorphedLmdInner(struct BattleFxBzMorphDesc *desc, void *unusedOt, int unusedZero, void *unusedPrimCursor, void *morphVertexBase, int morphT4096);
void __cdecl BattleFx_BzDrawSlot12(void);
void __cdecl BattleFx_BzDrawSlot10Ring(void);
void __cdecl BattleFx_BzDrawEnergyTubeTask(void);
void __cdecl BattleFx_BzBuildEnergyTube(void);
void __cdecl BattleFx_BzStepEnergyTube(void);
void __cdecl BattleFx_BzImpulseEnergyTube(int amplitude);
void __cdecl BattleFx_BzDrawSlot10Sprite(void);
void __cdecl BattleFx_BzDrawSlot13(void);
void __cdecl BattleFx_BzSpawnSlot14Swarm(void);
void __cdecl BattleFx_BzDrawSlot14(void);
void __cdecl BattleFx_BzSpawnDust(void);
int __cdecl BattleFx_BzAllocSubTask(int value);
void __cdecl BattleFx_BzDrawSlot15(struct YasuiBzSlotState *slotState);
void __cdecl BattleFx_BzSpawnGroundSparks(void);
void __cdecl BattleFx_BzDrawGroundSpark(struct BattleFxBzGroundLivewireSlot *p);
int __cdecl BattleFx_BzEmitGroundSparkPrim(struct BattleFxGroundLivewireDescriptor *descriptor, int unusedA, int unusedB, int unusedC);
void __cdecl BattleFx_BzDrawSlot10And16(void);
void __cdecl BattleFx_BzSpinWorldMatrix(int phase);
void __cdecl BattleFx_BzRunLightRig(void);

/* yasui_limit_planet_protector.cpp */
void __cdecl BattleFx_LimitLoadResPlanetProtector(void);
int __cdecl BattleFx_LimitEnterPlanetProtector(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginPlanetProtector(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitPlanetProtectorFadingSpritePoolTask(void);
void __cdecl BattleFx_LimitPlanetProtectorClearParticleRecords(void);
void __cdecl BattleFx_LimitPlanetProtectorDelayedSoundTask(void);
void __cdecl BattleFx_LimitSpawnPlanetProtectorPerTarget(int actor);
void __cdecl BattleFx_LimitPlanetProtectorTimelineTask(void);
void __cdecl BattleFx_LimitPlanetProtectorSpiralSpritePoolEmitterTask(void);
int __cdecl BattleFx_LimitPlanetProtectorClaimParticleRecord(void);
void __cdecl BattleFx_LimitPlanetProtectorDampedSpriteParticleTask(void);

/* yasui_limit_breath_of_the_earth_orbit_trails.cpp */
extern int g_YasuiLimitBreathOfTheEarthResourceHeader;
extern struct YasuiLimitBreathOfTheEarthTrailScratch *g_YasuiLimitBreathOfTheEarthTrailScratchPtr;
extern int g_YasuiLimitBreathOfTheEarthRenderPacketCursor;
extern struct Vec3sPacked *g_YasuiLimitBreathOfTheEarthOrbitScratchPtr;
extern char g_YasuiLimitBreathOfTheEarthOrbitScratchInit;
extern char g_YasuiLimitBreathOfTheEarthTrailScratchInit;
extern struct GfxTableCursor *g_YasuiLimitBreathOfTheEarthResourceHandle;
extern int g_YasuiLimitBreathOfTheEarthTrailBufferCount;
extern int g_YasuiLimitBreathOfTheEarthOrbitAnglePhase;
void __cdecl BattleFx_LimitLoadResBreathOfTheEarth(void);
int __cdecl BattleFx_LimitEnterBreathOfTheEarth(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginBreathOfTheEarth(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSpawnBreathOfTheEarthPerTarget(int actor);
void __cdecl BattleFx_LimitBreathOfTheEarthTimelineTask(void);
void __cdecl BattleFx_LimitBreathOfTheEarthOrbitTrailTask(void);
void __cdecl BattleFx_LimitBreathOfTheEarthAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitBreathOfTheEarthDrawPsxTrailRibbon(struct BattleFxPsxTrailRibbon *trail, int alpha);

/* yasui_limit_satan_slam_stage_shatter.cpp */
extern struct YasuiLimitSatanSlamWorkingMeshBlob g_YasuiLimitSatanSlamWorkingMeshBlob;
extern struct YasuiLimitSatanSlamSourceMeshAndPoseBlob g_YasuiLimitSatanSlamSourceMeshAndPoseBlob;
extern struct YasuiLimitSatanSlamSpriteRequest g_YasuiLimitSatanSlamSpriteRequest;
void __cdecl BattleFx_LimitLoadResSatanSlam(void);
int __cdecl BattleFx_LimitEnterSatanSlam(int targetMask, int actorIdx);
int __cdecl BattleFx_LimitEnterSatanSlamAlt(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginSatanSlam(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSatanSlamSetupStage(void);
void __cdecl BattleFx_LimitSatanSlamBuildFragmentRecords(struct BattleFxSatanSlamMeshSetup *state);
int __cdecl BattleFx_LimitSatanSlamBuildTriangleFragmentRecords(struct BattleFxSatanSlamMeshSetup *state, int vertexBase, struct BattleFxSatanSlamFragmentRecord *recordBlock);
int __cdecl BattleFx_LimitSatanSlamBuildQuadFragmentRecords(struct BattleFxSatanSlamMeshSetup *fxState, int vertexBase, struct BattleFxSatanSlamFragmentRecord *quadBlob);
void __cdecl BattleFx_LimitSatanSlamBuildMeshCellGrid(struct BattleFxSatanSlamMeshSetup *meshPtr);
int __cdecl BattleFx_LimitSatanSlamIndexTrianglesIntoCellGrid(struct BattleFxLimitSatanSlamMeshState *triState, char *vertexBase, int triBlob);
int __cdecl BattleFx_LimitSatanSlamIndexQuadsIntoCellGrid(struct BattleFxLimitSatanSlamMeshState *quadState, char *vertexBase, int quadBlob);
void __cdecl BattleFx_SatanSlamSpawnCameraScript(int scriptPtr, struct PsxMatrix *state, int mode);
void __cdecl BattleFx_SatanSlamRunCameraScript(void);
short __cdecl BattleFx_SatanSlamReadCameraScriptOperand(short **cursor);
int __cdecl BattleFx_SatanSlamApplyCameraEasing(int value, int amount);
void __cdecl BattleFx_LimitSatanSlamResetTrailRingBuffers(void);
void __cdecl BattleFx_LimitSatanSlamResetParticleRecords(void);
void __cdecl BattleFx_LimitSatanSlamQueueStageOffscreenRequests(struct BattleFxOffscreenCaptureReq *record);
void __cdecl BattleFx_LimitSatanSlamComputeVertexDeltas(struct SVECTOR *refPose, struct SVECTOR *workVertices, struct BattleFxShatterMeshDelta *deltas);
void __cdecl BattleFx_LimitSatanSlamTimelineTask(void);
void __cdecl BattleFx_LimitSatanSlamStageMeshTimelineTask(void);
void __cdecl BattleFx_LimitSatanSlamAddScaledVertexDeltas(struct SVECTOR *target, struct BattleFxShatterMeshDelta *deltas, int count, int factor);
int __cdecl BattleFx_LimitSatanSlamDrawPsxMesh(struct BattleFxLimitSatanSlamMeshState *meshState, int lighting);
int __cdecl BattleFx_LimitSatanSlamDrawPsxTexturedTriangles(struct BattleFxLimitSatanSlamMeshState *meshState, char *vertexBase, int triBlob, int lighting);
int __cdecl BattleFx_LimitSatanSlamDrawPsxTexturedQuads(struct BattleFxLimitSatanSlamMeshState *meshState, char *vertexBase, int quadBlob, int lighting);
int __cdecl BattleFx_LimitSatanSlamDrawPsxFragmentMesh(struct BattleFxLimitSatanSlamMeshState *meshState, int lighting);
int __cdecl BattleFx_LimitSatanSlamDrawPsxTexturedFragmentTriangles(struct BattleFxLimitSatanSlamMeshState *meshState, char *vertexBase, struct BattleFxSatanSlamFragmentPrimitive *fragBlob, int maxAge);
int __cdecl BattleFx_LimitSatanSlamDrawPsxTexturedFragmentQuads(struct BattleFxLimitSatanSlamMeshState *meshState, char *vertexBase, struct BattleFxSatanSlamFragmentPrimitive *fragmentBlock, int maxAge);
void __cdecl BattleFx_LimitSatanSlamSpawnTrailedParticlesTask(void);
void __cdecl BattleFx_LimitSatanSlamPushTrailPoint(struct BattleFxLimitSatanSlamTrailRing *ring, int point);
void __cdecl BattleFx_LimitSatanSlamTrailedParticleTask(void);
void __cdecl BattleFx_LimitSatanSlamSteerParticleTowardTarget(struct YasuiLimitSatanSlamParticle *particle);
void __cdecl BattleFx_LimitSatanSlamMirroredBurstModelsTask(void);
void __cdecl BattleFx_LimitSatanSlamCopyMeshPoseVertices(void *src);
void __cdecl BattleFx_LimitSatanSlamSetActorHeightsFromMesh(struct BattleFxLimitSatanSlamMeshState *meshState, int heightScale);
void __cdecl BattleFx_LimitSatanSlamFindMeshSurfaceNearPoint(struct BattleFxLimitSatanSlamMeshState *meshState, struct BattleFxSatanSlamActorSurface *surface);
int __cdecl BattleFx_LimitSatanSlamPointInTriangleXZ(struct SVECTOR *pointA, struct SVECTOR *pointB, struct SVECTOR *pointC, struct SVECTOR *pointD);
void __cdecl BattleFx_LimitSatanSlamMaskedRetireTask(void);
void __cdecl BattleFx_SatanSlamRootWatchdogTask(void);
void __cdecl BattleFx_LimitBeginSatanSlamAlt(int targetMask, int actorIdx);

/* yasui_limit_nightmare_effect.cpp */
extern unsigned char g_YasuiLimitNightmareMeshHeader1[1560];
extern unsigned char g_YasuiLimitNightmareMeshHeader2[1368];
extern int g_YasuiLimitNightmareTargetActor;
extern int g_YasuiLimitNightmareTargetMask;
extern int g_YasuiLimitNightmareSourceActor;
extern struct GfxTableCursor *g_YasuiLimitNightmareLoadedResTable;
void __cdecl BattleFx_LimitLoadResNightmare(void);
int __cdecl BattleFx_LimitEnterNightmare(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginNightmare(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitNightmareSpawnTimeline(void);
int __cdecl BattleFx_LimitNightmarePrepareMeshUvBlock(struct BattleFxNightmareUvScratch *block);
int __cdecl BattleFx_LimitNightmareAssignQuadUvsByAngle(struct BattleFxNightmareUvScratch *quadUv, int fragBase, int block);
void __cdecl BattleFx_LimitNightmareTimelineTask(void);
void __cdecl BattleFx_LimitNightmareTrailingModelTask(void);
void __cdecl BattleFx_LimitNightmarePushTrailPoint(struct BattleFxLimitNightmareTrailRing *trail, int point);
void __cdecl BattleFx_LimitNightmareSpinningGradientModelTask(void);
int __cdecl BattleFx_LimitNightmareDrawPsxWeightedGouraudMesh(struct BattleFxNightmareQuadState *drawState, int weight);
int __cdecl BattleFx_LimitNightmareDrawPsxWeightedGouraudQuads(struct BattleFxNightmareQuadState *p, int verts, int hdr, int weight);
void __cdecl BattleFx_LimitNightmareAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitNightmareExpandingModelTask(void);
int __cdecl BattleFx_LimitNightmareDrawPsxMesh(struct BattleFxLimitNightmarePsxMesh *mesh, int orderingTable, int depthShift, int packetCursor);

/* yasui_limit_livewire_effect.cpp */
void __cdecl BattleFx_LimitLoadResLivewire(void);
int __cdecl BattleFx_LimitEnterLivewire(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginLivewire(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitLivewireSpawnTimeline(void);
void __cdecl BattleFx_LimitLivewireTimelineTask(void);
void __cdecl BattleFx_LimitLivewireTableScaledModelTask(void);
void __cdecl BattleFx_LimitLivewireExpandingModelTask(void);
void __cdecl BattleFx_LimitLivewireStreakTask(void);
void __cdecl BattleFx_LimitLivewireOrientedSpriteTask(void);
void __cdecl BattleFx_LimitLivewireSpawnSpriteBurst(struct BattleFxLimitLivewireTimelineSlot *spriteParams, int count);
void __cdecl BattleFx_LimitLivewireAnimatedSpriteEntry2Task(void);
void __cdecl BattleFx_LimitLivewireAnimatedSpriteEntry3Task(void);

/* yasui_limit_ungarmax_impacts.cpp */
void __cdecl BattleFx_LimitLoadResUngarmax(void);
int __cdecl BattleFx_LimitEnterUngarmax(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginUngarmax(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitUngarmaxTimelineTask(void);
void __cdecl BattleFx_LimitUngarmaxSpawnImpactEffects(int targetActor);
void __cdecl BattleFx_LimitUngarmaxBillboardModelTask(void);
void __cdecl BattleFx_LimitUngarmaxAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitUngarmaxFadingPointParticleTask(void);
void __cdecl BattleFx_LimitUngarmaxActorAttachedModelsTask(void);

/* yasui_limit_climhazzard.cpp */
void __cdecl BattleFx_LimitLoadResClimhazzard(void);
int __cdecl BattleFx_LimitEnterClimhazzard(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginClimhazzard(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitClimhazzardTimelineTask(void);
void __cdecl BattleFx_LimitClimhazzardBeamTrailEmitterTask(void);
void __cdecl BattleFx_LimitClimhazzardOffsetAlongDirection(struct Vec3sPacked *startPt, struct Vec3sPacked *endPt, int distanceFixed, struct Vec3sPacked *outPt);
void __cdecl BattleFx_LimitClimhazzardShadedQuadTrailTask(void);
void __cdecl BattleFx_LimitClimhazzardAnimatedSpriteTask(void);

/* yasui_limit_hammerblow.cpp */
extern unsigned char g_YasuiLimitHammerblowBlowRsdFilename[9];
void __cdecl BattleFx_LimitLoadResHammerblow(void);
int __cdecl BattleFx_LimitEnterHammerblow(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitRegisterHammerblowPerTargetCallback(int targetActor, int sourceActor);
void __cdecl BattleFx_LimitSpawnHammerblowPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_LimitHammerblowTimelineTask(void);
void __cdecl BattleFx_LimitHammerblowGrowingBlackModelTask(void);
void __cdecl BattleFx_LimitHammerblowSpawnEasedGrowingModelsTask(void);
void __cdecl BattleFx_LimitHammerblowEasedGrowingModelTask(void);
void __cdecl BattleFx_LimitHammerblowRadialSpriteEmitterTask(void);
void __cdecl BattleFx_LimitHammerblowSpriteAnimationTask(void);
void __cdecl BattleFx_LimitHammerblowTumblingActorTask(void);

/* yasui_limit_catastrophe_beam.cpp */
void __cdecl BattleFx_LimitLoadResCatastrophe(void);
int __cdecl BattleFx_LimitEnterCatastrophe(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginCatastrophe(int targetMask, int actorIdx);
void __cdecl BattleFx_CatastropheSpawnCameraScript(int scriptPtr, int matrixPtr, int sourceActor);
void __cdecl BattleFx_CatastropheRunCameraScript(void);
short __cdecl BattleFx_CatastropheReadCameraScriptOperand(void *track);
int __cdecl BattleFx_CatastropheApplyCameraEasing(int current, int target);
void __cdecl BattleFx_LimitCatastropheActorYOffsetTask(void);
void __cdecl BattleFx_LimitCatastropheActorWorldSpriteBurstTask(void);
void __cdecl BattleFx_LimitCatastropheTimelineTask(void);
void __cdecl BattleFx_LimitCatastropheJitterTask(void);
void __cdecl BattleFx_LimitCatastropheBillboardModelTask(void);
void __cdecl BattleFx_LimitCatastropheHookDriverTask(void);
void __cdecl BattleFx_LimitCatastropheInitHookNoOp(int hookState, int *param);
void __cdecl BattleFx_LimitCatastropheUpdateHookNoOp(int hookState);
void __cdecl BattleFx_LimitCatastropheBeamModelTask(void);
int __cdecl BattleFx_LimitCatastropheBuildBeamMatrixFromEndpoints(struct Vec3sPacked *startPos, struct Vec3sPacked *endPos, struct BattleRotationMatrix *outMatrix);
void __cdecl BattleFx_LimitCatastrophePointHookNoOp(int point);
void __cdecl BattleFx_LimitCatastropheAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitCatastropheRisingSpriteTask(void);
void __cdecl BattleFx_LimitCatastropheGrowingBlackModelTask(void);
void __cdecl BattleFx_LimitCatastropheLineSegmentTask(void);
int __cdecl BattleFx_LimitCatastropheAdvanceBeamPointAlongTargets(struct Vec3sPacked *outPoint);
void __cdecl BattleFx_LimitCatastropheSpawnSpriteTrailTask(void);
void __cdecl BattleFx_LimitCatastropheDampedSpriteParticleTask(void);
void __cdecl BattleFx_LimitCatastropheScreenFlashTask(void);
void __cdecl BattleFx_LimitCatastropheWaitEffectEndTask(void);
void __cdecl BattleFx_LimitCatastropheBuildSortedSlotTable(void);

/* yasui_limit_omnislash_effect.cpp */
void __cdecl BattleFx_LimitLoadResOmnislash(void);
int __cdecl BattleFx_LimitEnterOmnislash(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginOmnislash(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitOmnislashTimelineTask(void);
void __cdecl BattleFx_LimitOmnislashScalingModelEntry0Task(void);
void __cdecl BattleFx_LimitOmnislashScalingModelEntry1Task(void);
void __cdecl BattleFx_LimitOmnislashMovingPulsingModelTask(void);
void __cdecl BattleFx_LimitOmnislashSpritePairParticleEmitterTask(void);
int __cdecl BattleFx_LimitOmnislashInstallRotatedScaledMatrix(struct BattleFxLimitOmnislashMatrix *matrix, struct SVECTOR *rotation, short scale);
void __cdecl BattleFx_LimitOmnislashBoneModelShrinkingEmitterTask(void);
void __cdecl BattleFx_LimitOmnislashSineScaledModelTask(void);
void __cdecl BattleFx_LimitOmnislashGrowingBillboardModelTask(void);
void __cdecl BattleFx_LimitOmnislashSpawnDampedSpriteParticles(short *origin, short count, short speed, short angle);
void __cdecl BattleFx_LimitOmnislashDampedSpriteParticleTask(void);
void __cdecl BattleFx_LimitOmnislashBoneSpriteEmitterTask(void);
void __cdecl BattleFx_LimitOmnislashSpriteAnimationTask(void);
void __cdecl BattleFx_LimitOmnislashSpinningCylinderPairTask(void);
void __cdecl BattleFx_LimitOmnislashDrawTexturedCylinder(short angle);
void __cdecl BattleFx_LimitOmnislashSineScaledModelParticleEmitterTask(void);
void __cdecl BattleFx_LimitOmnislashSpinningPulsingModelTask(void);
void __cdecl BattleFx_LimitOmnislashBoneModelGrowingEmitterTask(void);
void __cdecl BattleFx_LimitOmnislashSineScaledRotatingModelTask(void);
void __cdecl BattleFx_LimitOmnislashWhiteFlashPulseTask(void);
void __cdecl BattleFx_LimitOmnislashBeginParticlePool(void);
void __cdecl BattleFx_LimitOmnislashParticlePoolTask(void);
void __cdecl BattleFx_LimitOmnislashOrientedPoolParticleModel(void);
short __cdecl BattleFx_LimitOmnislashClaimParticleRecord(void);

/* yasui_limit_blade_beam.cpp */
extern unsigned char g_YasuiLimitBladeBeamCutterRsdFilename[11];
void __cdecl BattleFx_LimitLoadResBladeBeam(void);
int __cdecl BattleFx_LimitEnterBladeBeam(int targetMask, int actorIdx);
int __cdecl BattleFx_LimitEnterBladeBeamAlt(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginBladeBeam(int targetMask, int casterIdx);
void __cdecl BattleFx_LimitBladeBeamSpawnTimeline(void);
void __cdecl BattleFx_LimitBladeBeamTimelineTask(void);
void __cdecl BattleFx_LimitBladeBeamTravelingBeamModelTask(void);
void __cdecl BattleFx_LimitBladeBeamMirroredSpritePairTask(void);
void __cdecl BattleFx_LimitBladeBeamOrientedSpriteTask(void);
void __cdecl BattleFx_LimitBladeBeamTrailParticlePool100Task(void);
void __cdecl BattleFx_LimitBladeBeamUpdateTrailParticle(struct BattleFxBladeBeamParticle *targetRecord, struct BattleFxSpriteColor *colorRgb);
void __cdecl BattleFx_LimitBladeBeamFadingSpriteEmitterTask(void);
void __cdecl BattleFx_LimitBladeBeamFadingBillboardSpriteTask(void);
void __cdecl BattleFx_LimitBladeBeamDampedSpriteParticlePool50Task(void);
void __cdecl BattleFx_LimitBladeBeamUpdateDampedSpriteParticle(struct BattleFxBladeBeamParticle *particleRecord, struct BattleFxSpriteColor *colorRgb);
void __cdecl BattleFx_LimitBeginBladeBeamAlt(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSpawnBladeBeamAltPerTarget(int actor);
void __cdecl BattleFx_LimitBladeBeamAltTimelineTask(void);

/* yasui_limit_satellite_beam.cpp */
extern struct YasuiLimitSatelliteBeamModelDrawReq g_YasuiLimitSatelliteBeamModelDrawReq;
extern struct YasuiLimitSatelliteBeamTransformMatrix g_YasuiLimitSatelliteBeamTransformMatrix;
extern int g_YasuiLimitSatelliteBeamBlastRotationAngles;
void __cdecl BattleFx_LimitLoadResSatelliteBeam(void);
int __cdecl BattleFx_LimitEnterSatelliteBeam(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginSatelliteBeam(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSatelliteBeamTimelineTask(void);
void __cdecl BattleFx_LimitSatelliteBeamTargetEmitterTask(void);
void __cdecl BattleFx_LimitSatelliteBeamTargetTimelineTask(void);
void __cdecl BattleFx_LimitSatelliteBeamCrosshairLineTask(void);
void __cdecl BattleFx_LimitSatelliteBeamQuadrantModelTask(void);
void __cdecl BattleFx_LimitSatelliteBeamProjectedSpritePairTask(void);
void __cdecl BattleFx_LimitSatelliteBeamScatteredColumnEmitterTask(void);
void __cdecl BattleFx_LimitSatelliteBeamGrowingModelTask(void);
void __cdecl BattleFx_LimitSatelliteBeamScatteredColumnModelTask(void);
void __cdecl BattleFx_LimitSpawnSatelliteBeamPerTarget(int actor);
void __cdecl BattleFx_LimitSatelliteBeamImpactTimelineTask(void);
void __cdecl BattleFx_LimitSatelliteBeamImpactColumnModelTask(void);
void __cdecl BattleFx_LimitSatelliteBeamExpandingShellModelTask(void);
void __cdecl BattleFx_LimitSatelliteBeamBlackPuffEmitterTask(void);
void __cdecl BattleFx_LimitSatelliteBeamBlackPuffModelTask(void);
void __cdecl BattleFx_LimitSatelliteBeamDampedSpriteParticleTask(void);

/* yasui_limit_grenade_bomb.cpp */
void __cdecl BattleFx_LimitLoadResGrenadeBomb(void);
int __cdecl BattleFx_LimitEnterGrenadeBomb(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginGrenadeBomb(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitGrenadeBombTimelineTask(void);
void __cdecl BattleFx_LimitGrenadeBombExpandingShellModelTask(void);
void __cdecl BattleFx_LimitGrenadeBombGrowingModelTask(void);
void __cdecl BattleFx_LimitGrenadeBombTumblingSpriteEmitterTask(void);
void __cdecl BattleFx_LimitGrenadeBombTumblingSpriteTask(void);
void __cdecl BattleFx_LimitGrenadeBombRadialSpriteEmitterTask(void);
void __cdecl BattleFx_LimitGrenadeBombRadialSpriteTask(void);
void __cdecl BattleFx_LimitGrenadeBombBlackBlastModelTask(void);

/* yasui_limit_seal_evil_effect.cpp */
extern void (__cdecl *g_YasuiLimitSealEvilParticleCallbacks[170])(int);
extern int g_YasuiLimitSealEvilSpriteDrawSlot;
extern int g_YasuiLimitSealEvilTargetMask;
extern int g_YasuiLimitSealEvilBillboardCount;
extern int g_YasuiLimitSealEvilOrientedCount;
extern int g_YasuiLimitSealEvilMatrixPoolIdx;
extern struct GfxTableCursor *g_YasuiLimitSealEvilLoadedResTable;
void __cdecl BattleFx_LimitLoadResSealEvil(void);
int __cdecl BattleFx_LimitEnterSealEvil(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBeginSealEvil(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSealEvilClearParticleTable(void);
void __cdecl BattleFx_LimitSealEvilParticlePoolTask(void);
void __cdecl BattleFx_LimitSpawnSealEvilPerTarget(int actor);
void __cdecl BattleFx_LimitSealEvilPerTargetTimelineTask(void);
void __cdecl BattleFx_LimitSealEvilParticleBurstTask(void);
int __cdecl BattleFx_LimitSealEvilClaimParticleRecord(int callback);
void __cdecl BattleFx_LimitSealEvilBillboardSpriteParticle(struct YasuiSealEvilParticle *particleRecord);
void __cdecl BattleFx_LimitSealEvilTimelineTask(void);

/* yasui_limit_great_gospel.cpp */
extern int g_YasuiLimitGreatGospelColorMode;
void __cdecl BattleFx_LimitLoadResGreatGospel(void);
int __cdecl BattleFx_LimitEnterGreatGospel(int targetMask, int actorIdx);
void __cdecl BattleFx_GreatGospelCopyPolygonSetVertices(int vertexIdx, int arg2, struct BattleFxGreatGospelPolygonVertex2 *outVertices);
void __cdecl BattleFx_GreatGospelBeginEffect(int targetMask, int actorIdx);
void __cdecl BattleFx_GreatGospelSpawnCameraScript(void *scriptData, int matrixPtr, int actorIdx);
void __cdecl BattleFx_GreatGospelRunCameraScript(void);
short __cdecl BattleFx_GreatGospelReadCameraScriptOperand(short **scriptPtr);
void __cdecl BattleFx_GreatGospelClearParticleTable(void);
void __cdecl BattleFx_GreatGospelParticlePoolTask(void);
void __cdecl BattleFx_GreatGospelTimelineTask(void);
void __cdecl BattleFx_GreatGospelFallingStreakEmitterTask(void);
int __cdecl BattleFx_GreatGospelClaimParticleRecord(int callback);
void __cdecl BattleFx_GreatGospelFallingStreakTask(struct BattleFxMeshShardSlot *particleRecord);
void __cdecl BattleFx_GreatGospelStreakImpactSpriteTask(struct BattleFxGreatGospelStreakParticle *particleRecord);
void __cdecl BattleFx_GreatGospelDiscardedColorScaleRampTask(void);
void __cdecl BattleFx_GreatGospelSteppedModelTask(void);
void __cdecl BattleFx_GreatGospelSpinningModelTask(void);
void __cdecl BattleFx_GreatGospelSpiralingModelTask(void);
void __cdecl BattleFx_GreatGospelGrowingFadeModelEmitterTask(void);
void __cdecl BattleFx_GreatGospelGrowingFadeModelTask(void);
void __cdecl BattleFx_GreatGospelExpandingFadeModelTask(void);
void __cdecl BattleFx_GreatGospelWideGrowingFadeModelEmitterTask(void);
void __cdecl BattleFx_GreatGospelWideGrowingFadeModelTask(void);
void __cdecl BattleFx_GreatGospelSpawnPerTarget(int actor);
void __cdecl BattleFx_GreatGospelFallingSpriteEmitterTask(void);
void __cdecl BattleFx_GreatGospelFallingSpriteTask(void);
void __cdecl BattleFx_GreatGospelClearEffectFlagTask(void);

/* yasui_battle_fx_magic.cpp */
extern int g_YasuiMagicWallScale;
extern int g_YasuiMagicWallDrawOrder;
extern int g_YasuiMagicWallBlendFactor;
void __cdecl BattleFx_MagicLoadResWall(void);
void __cdecl BattleFx_MagicEnterWall(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginWall(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicSpawnWallPerTarget(int actor);
void __cdecl BattleFx_MagicWallTimelineTask(void);
void __cdecl BattleFx_MagicWallShrinkingModelTask(void);
void __cdecl BattleFx_MagicWallShortShrinkingModelTask(void);

/* yasui_special_boulder_throw_effect.cpp */
extern unsigned char g_BoulderThrow_MeshPayload[440];
extern struct BoulderThrow_SpriteResourcePointer g_BoulderThrowSpriteResourcePointer;
extern struct BoulderThrow_DisplacementVelocities g_BoulderThrow_DisplacementVelocities;
extern struct YasuiBoulderThrowDisplacement g_BoulderThrow_DisplacementPositions;
extern int g_BoulderThrow_MeshDrawHandle;
extern struct GfxTableCursor *g_BoulderThrow_LoadedResTable;
extern short g_BoulderThrow_PitchAngle;
extern short g_BoulderThrow_YawAngle;
void __cdecl BattleFx_SpecialLoadResBoulderThrow(void);
void __cdecl BattleFx_SpecialEnterBoulderThrow(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginBoulderThrow(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBoulderThrowInitDisplacementVectors(void);
void __cdecl BattleFx_SpecialSpawnBoulderThrowPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialBoulderThrowTimelineTask(void);
void __cdecl BattleFx_SpecialBoulderThrowDampedSpriteParticleTask(void);
void __cdecl BattleFx_SpecialBoulderThrowShatteringMeshTask(void);
int __cdecl BattleFx_SpecialBoulderThrowDrawPsxDisplacedMesh(struct BattleFxPsxDisplacedMeshDesc *pDescriptor, int reserved, int spare, struct BattleFxPsxPrimitive *spareC, struct Vec3sPacked *pDisplacements, struct PsxMatrix *pRotMatrix, struct BattleFxOffsetTriMeshDesc *pTransMatrix);
int __cdecl BattleFx_SpecialBoulderThrowDrawPsxMesh(struct BattleFxPsxMeshDescriptor *pDescriptor, int orderingTable, int depthBias, int listHead);
void __cdecl BattleFx_SpecialBoulderThrowAdvanceMeshDisplacements(void);

/* yasui_special_heartless_angel_effect.cpp */
extern int g_HeartlessAngel_ResourceHeader;
extern int g_HeartlessAngel_SpriteDrawHandle;
extern short g_HeartlessAngel_StopFlag;
extern struct GfxTableCursor *g_HeartlessAngel_LoadedResTable;
void __cdecl BattleFx_SpecialLoadResHeartlessAngel(void);
void __cdecl BattleFx_SpecialEnterHeartlessAngel(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginHeartlessAngel(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnHeartlessAngelPerTarget(int actor, int actorIdx);
void __cdecl BattleFx_SpecialHeartlessAngelEmitterTask(void);
void __cdecl BattleFx_SpecialHeartlessAngelFallingSpriteParticleTask(void);

/* yasui_special_bizarro_energy_effect.cpp */
void __cdecl BattleFx_SpecialLoadResBizarroEnergy(void);
void __cdecl BattleFx_SpecialEnterBizarroEnergy(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginBizarroEnergy(int targetMask, int actorIdx);
void __cdecl BattleFx_BizarroEnergySpawnCameraScript(short *scriptCursor, struct PsxMatrix *stateArg, int mode);
void __cdecl BattleFx_BizarroEnergyRunCameraScript(void);
short __cdecl BattleFx_BizarroEnergyReadCameraScriptOperand(short **cursor);
int __cdecl BattleFx_BizarroEnergyApplyCameraEasing(int progress, int easeRate);
void __cdecl BattleFx_SpecialBizarroEnergyCameraShakeTask(void);
void __cdecl BattleFx_SpecialBizarroEnergyTimelineTask(void);
void __cdecl BattleFx_SpecialBizarroEnergyScreenFlashTask(void);
void __cdecl BattleFx_SpecialBizarroEnergyScaledSpriteTask(void);
void __cdecl BattleFx_SpecialBizarroEnergyScalingModelEmitterTask(void);
void __cdecl BattleFx_SpecialBizarroEnergyDriftingSpriteTask(void);
void __cdecl BattleFx_SpecialBizarroEnergyAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialBizarroEnergySpiralingSpriteTask(void);
void __cdecl BattleFx_SpecialBizarroEnergyRisingSpriteTask(void);
void __cdecl BattleFx_SpecialBizarroEnergySpinningModelTask(void);
void __cdecl BattleFx_SpecialBizarroEnergyGrowingModelTask(void);
void __cdecl BattleFx_SpecialBizarroEnergySpawnRisingSpritesTask(void);
void __cdecl BattleFx_SpecialBizarroEnergySlidingModelTask(void);

/* yasui_special_rocket_punch_effect.cpp */
extern int g_YasuiSpecRocketPunchResourceHeader;
extern short g_YasuiSpecRocketPunchFlashIntensity;
extern int g_YasuiSpecRocketPunchRenderPacketCursor;
extern short g_YasuiSpecRocketPunchFlightDuration;
extern struct GfxTableCursor *g_YasuiSpecRocketPunchResourceHandle;
extern short g_YasuiSpecRocketPunchDualVariantFlag;
void __cdecl BattleFx_SpecialLoadResRocketPunch(void);
void __cdecl BattleFx_SpecialEnterDualRocketPunch(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnterRocketPunch(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialRunRocketPunch(int targetMask, int sourceActor);
void __cdecl BattleFx_SpecialSpawnRocketPunchPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_SpecialRocketPunchTimelineTask(void);
void __cdecl BattleFx_SpecialRocketPunchImpactFlashShakeTask(void);
void __cdecl BattleFx_SpecialRocketPunchTrailEmitterTask(void);
void __cdecl BattleFx_SpecialRocketPunchTrailModelTask(void);

/* yasui_summon_knights_of_round.cpp */
extern unsigned char g_BattleSummonKotrSoundGroupRecords[256];
extern struct BattleSummonKotrKnightLookAtPoints g_BattleSummonKotrKnightLookAtPoints;
extern struct BattleSummonKotrStageRequestArgs g_BattleSummonKotrStageRequestArgs;
extern struct BattleKotrFragmentData g_BattleKotrFragmentData;
extern unsigned char g_BattleSummonKotrDiscBufferStorage[65860];
void __cdecl BattleFx_SummonLoadResKnightsOfRound(void);
void __cdecl BattleFx_KotrFreeThirteenKnightModels(void);
void __cdecl BattleFx_KotrLoadThirteenKnightModels(void);
void __cdecl BattleFx_SummonEnterKnightsOfRound(int targetMask, int actorIdx);
void __cdecl BattleFx_KotrBeginEffect(int targetMask, int actorIdx);
void __cdecl BattleFx_KotrResetModuleState(void);
void __cdecl BattleFx_KotrSpawnCameraScript(int eyeTrack, int focusTrack, int duration);
void __cdecl BattleFx_KotrRunCameraScript(void);
short __cdecl BattleFx_KotrFetchScriptCount(short **index);
int __cdecl BattleFx_ApplySineEasing(int value, int easeMode);
void __cdecl BattleFx_KotrAttachPointToLocal(int recordIndex, int attachIndex, int reserved, struct SVECTOR *localOffset);
void __cdecl BattleFx_KotrTransformAttachPointToLocal(int attachIndex, struct SVECTOR *localVec);
void __cdecl BattleFx_KotrMasterTimeline(void);
void __cdecl BattleFx_KotrSpawnShakeEnvelope(int amplitude, int duration, int rate, int decay);
void __cdecl BattleFx_KotrShakeEnvelopeTask(void);
void __cdecl BattleFx_KotrRadialDiscMeshTimelineTask(void);
void __cdecl BattleFx_KotrPrepareStageOffscreenRequest(void);
void __cdecl BattleFx_KotrQueueStageOffscreenRequest(void *matrix, struct BattleFxWhirlsandStageRequestArgs *inputArgs);
void __cdecl BattleFx_KotrBuildRadialDiscMesh(void);
void __cdecl BattleFx_KotrApplyDiscMeshDisplacement(int amplitude, int shadeFrom, int shadeTo);
void __cdecl BattleFx_KotrSpinningModelEntry15Task(void);
void __cdecl BattleFx_KotrEmitFadingOffscreenQuadParticlesTask(void);
int __cdecl BattleFx_KotrGetDiscHeightAtRadius(int radius);
void __cdecl BattleFx_KotrFadingOffscreenQuadParticleTask(void);
void __cdecl BattleFx_KotrSpawnActorLiftPerTarget(int actor);
void __cdecl BattleFx_KotrActorLiftTask(void);
void __cdecl BattleFx_KotrSpawnKnight(struct BattleKotrKnight *knightContext, int callback, int knightIndex);
void __cdecl BattleFx_KotrInitKnightContextHookNoOp(int reserved, int knightContext);
void __cdecl BattleFx_KotrKnight1TimelineTask(void);
void __cdecl BattleFx_KotrSpawnKnightEndpointTrail(struct BattleKotrKnight *knightTrail, int duration, void *endpointDef);
void __cdecl BattleFx_KotrKnightEndpointTrailTask(void);
void __cdecl BattleFx_KotrDrawKnightModel(struct BattleKotrKnight *knight);
void __cdecl BattleFx_KotrPulsingTriangleParticleTask(void);
void __cdecl BattleFx_KotrBumpKnightContextCounter(struct BattleKotrKnight *knight);
void __cdecl BattleFx_KotrPostKnightImpactRecords(void);
void __cdecl BattleFx_KotrImpactScreenFlashTask(void);
void __cdecl BattleFx_KotrSpawnKnightActorBurstSprites(struct BattleKotrKnight *knight);
void __cdecl BattleFx_KotrSpawnActorBurstSprites(struct Vec3sPacked *origin);
void __cdecl BattleFx_KotrGetActorAttachLocalPosition(int actorIndex, int attachIndex, void *localPos);
void __cdecl BattleFx_KotrActorBurstSpriteTask(void);
int __cdecl BattleFx_KotrInstallBillboardMatrix(struct BattleFxSourcePoint *sourcePoint, short angle, int offset);
void __cdecl BattleFx_KotrKnight1SpinScaleModelSetupTask(void);
void __cdecl BattleFx_KotrSpawnKnight1SpinScaleModelPerTarget(int actor);
void __cdecl BattleFx_KotrSpawnEasedSpinScaleModel(void *model, void *record);
void __cdecl BattleFx_KotrEasedSpinScaleModelTask(void);
void __cdecl BattleFx_KotrKnight2TimelineTask(void);
void __cdecl BattleFx_KotrKnight2SpinScaleModelSetupTask(void);
void __cdecl BattleFx_KotrSpawnKnight2SpinScaleModelPerTarget(int actor);
void __cdecl BattleFx_KotrKnight3TimelineTask(void);
void __cdecl BattleFx_KotrImpactBurstTimelineTask(void);
void __cdecl BattleFx_KotrImpactSpriteParticleTask(void);
void __cdecl BattleFx_KotrSpinningGrowingModelTask(void);
void __cdecl BattleFx_KotrImpactSpriteRingTask(void);
void __cdecl BattleFx_KotrKnight4TimelineTask(void);
void __cdecl BattleFx_KotrSpawnKnight4SpinScaleModelPerTarget(int actor);
void __cdecl BattleFx_KotrKnight5TimelineTask(void);
void __cdecl BattleFx_KotrSpawnKnight5SpinScaleModelPerTarget(int actor);
void __cdecl BattleFx_KotrKnight6TimelineTask(void);
void __cdecl BattleFx_KotrSpawnKnight6SpinScaleModelPairPerTarget(int actor);
void __cdecl BattleFx_KotrKnight7TimelineTask(void);
void __cdecl BattleFx_KotrShrinkExpandModelsTimelineTask(void);
void __cdecl BattleFx_KotrKnight7ShrinkingBillboardModelTask(void);
void __cdecl BattleFx_KotrExpandingFadingModelTask(void);
void __cdecl BattleFx_KotrKnight8TimelineTask(void);
void __cdecl BattleFx_KotrKnight8SpinScaleModelSetupTask(void);
void __cdecl BattleFx_KotrSpawnKnight8SpinScaleModelPerTarget(int actor);
void __cdecl BattleFx_KotrKnight9TimelineTask(void);
void __cdecl BattleFx_KotrSpawnTumblingModelsAndPulsesTimelineTask(void);
void __cdecl BattleFx_KotrTumblingShrinkingModelEmitterTask(void);
void __cdecl BattleFx_KotrTumblingShrinkingModelTask(void);
void __cdecl BattleFx_KotrKnight9PulsingBillboardModelTask(void);
void __cdecl BattleFx_KotrKnight10TimelineTask(void);
void __cdecl BattleFx_KotrSpawnKnight10SpinScaleModelPerTarget(int actor);
void __cdecl BattleFx_KotrKnight11TimelineTask(void);
void __cdecl BattleFx_KotrBeginKnight11SpinScaleModels(void);
void __cdecl BattleFx_KotrSpawnKnight11SpinScaleModelPairPerTarget(int actor);
void __cdecl BattleFx_KotrKnight12TimelineTask(void);
void __cdecl BattleFx_KotrSpawnKnight12SpinScaleModelPairPerTarget(int actor);
void __cdecl BattleFx_KotrKnight13TimelineTask(void);
void __cdecl BattleFx_KotrKnight13StretchingFlashModelTask(void);
void __cdecl BattleFx_KotrSpawnKnight13SpinScaleModelPerTarget(int actor);
void __cdecl BattleFx_KotrScalingModelPairTask(void);
void __cdecl BattleFx_KotrSpawnFirstStackedModelLayerPairTask(void);
void __cdecl BattleFx_KotrSpawnStackedModelLayersTask(void *value);
void __cdecl BattleFx_KotrFadingStackedModelLayersTask(void);
void __cdecl BattleFx_KotrDrawStackedModelLayers(int matrix, int angle, int angleOffset, int count, int variant);
void __cdecl BattleFx_KotrSpawnSecondStackedModelLayerPairTask(void);
void __cdecl BattleFx_KotrSpawnConvergingStreakEmitterAndLayerTask(void);
void __cdecl BattleFx_KotrConvergingStreakEmitterTask(void);
void __cdecl BattleFx_KotrConvergingStreakModelTask(void);
void __cdecl BattleFx_KotrFadingScrollingPrimitiveSpawnTimeline(void);
void __cdecl BattleFx_KotrFadingScrollingPrimitiveTask(void);
int __cdecl BattleFx_DrawPrimitivePairReturningOtArg(struct BattleFxBzSpriteParams *record, int otArg, int packetCursor);
void __cdecl BattleFx_KotrSpawnSpinningModelsAndLayerTask(void);
void __cdecl BattleFx_KotrSpinningFadingModelTask(void);
void __cdecl BattleFx_KotrSpinningModelPairTask(void);
void __cdecl BattleFx_KotrSpawnSpinningFadingModelTask(void);
void __cdecl BattleFx_KotrSpawnRadialStreakEmitterTask(void);
void __cdecl BattleFx_KotrRadialStreakEmitterTask(void);
void __cdecl BattleFx_KotrRadialStreakModelTask(void);
void __cdecl BattleFx_KotrFragmentSpinScatterFadeTimelineTask(void);
int __cdecl BattleFx_KotrDrawPsxFragmentTriangles(void *resource, int unusedB, int unusedC, int unusedD, int matrices, int colors);
int __cdecl BattleFx_KotrReturnOtArgUnchanged(int unusedA, int unusedB, int unusedC, int otArg);
void __cdecl BattleFx_KotrInitFragmentMotionFromNormals(int resource);
void __cdecl BattleFx_KotrUpdateFragmentSpinShrinkMatrices(struct PsxMatrix *parentMatrix, int xMin);
void __cdecl BattleFx_KotrUpdateFragmentScatterMatrices(struct PsxMatrix *parentMatrix, int elapsed, int scale);
void __cdecl BattleFx_KotrSpawnDelayedSoundTask(int soundId, int delay, int firstSound, int secondSound);
void __cdecl BattleFx_KotrDelayedSoundTask(void);

/* yasui_item_smoke_bomb.cpp */
void __cdecl Battle_ItemFxLoadResSmokeBomb(void);
void __cdecl Battle_ItemFxEnterSmokeBomb(int targetMask, int actorIdx);
void __cdecl Battle_ItemFxSmokeBombSpawnEmitterWithSound(int targetMask, int actorIdx);
void __cdecl Battle_ItemFxSmokeBombSpriteEmitterTask(void);
void __cdecl Battle_ItemFxSmokeBombAnimatedSpriteTask(void);

/* yasui_item_echo_screen.cpp */
extern unsigned char g_YasuiItemEchoScreenModelFilename[11];
extern int g_YasuiItemEchoScreenSpriteDrawHandle;
extern struct GfxTableCursor *g_YasuiItemEchoScreenLoadedResTable;
void __cdecl Battle_ItemFxLoadResEchoScreen(void);
void __cdecl Battle_ItemFxEnterEchoScreen(int targetMask, int actorIdx);
void __cdecl Battle_ItemFxEchoScreenRegisterPerTargetCallback(int targetMask, int actorIdx);
void __cdecl Battle_ItemFxEchoScreenSpawnPerTarget(int actor);
void __cdecl Battle_ItemFxEchoScreenSpriteEmitterTask(void);
void __cdecl Battle_ItemFxEchoScreenAnimatedSpriteTask(void);

extern struct BattleActor g_BattleActorRecords[10];
extern struct BattleActorState g_BattleActorStates[10];
extern struct BattleOverlayTaskSlot g_BattleOverlayTaskSlotBuffer[10];
extern struct BattleAuxTaskSlot g_BattleAuxTaskSlotBuffer[16];
extern struct BattleMagicIce3Fragment g_BattleMagicIce3FragmentBuffer[256];
extern struct BattleMagicQuake3Shard g_BattleMagicQuake3ShardRecords[96];
extern struct BattleMagicQuake2Shard g_BattleMagicQuake2ShardRecords[96];
extern struct BattleCommandContext *g_pBattleCommandCtx;
extern struct BattleActorSpawnPos g_BattleActorSpawnPos[10];
extern struct BattleEffectTrailState g_BattleFxTimelineState;
extern struct BattleHitDisplayEntry g_BattleHitDisplayTable[78];
extern struct Battle3dNode g_BattleStageNodes[10];
extern struct BattleRewardItem g_BattleRewardItems[4];
extern struct BattleRewardTotals g_BattleRewardTotals;
extern struct BattleSequenceBuffer g_BattleSequenceBuffer;
extern struct BattleTaskSlotBuffer g_BattleTaskSlotBuffer;
extern struct Battle3dNode g_BattleCameraTransform;
extern struct BattleSceneState g_BattleScene;
extern struct BattleState DAT_009a8b10;
extern struct BattleItemListBlock DAT_009ac110;
#define g_BattleState DAT_009a8b10
struct BattleFxOdinZTaskSlot;
struct BattlePendingDropRecord;
struct BattleSummonAlexanderParticleBuffer;
struct BattleSummonAlexanderSubState;
struct BattleSummonAlexanderTable;
struct RubyTentaclesCombinedMatrix;

/* bdata.cpp */
char *__cdecl Battle_GetDataDirectory(void);
char *__cdecl BattleFx_BuildResourcePath(char *leaf, char *out);

/* battle_actor_anim_effect_tasks.cpp */
void __cdecl Battle_MainTaskWaitMessageWindow(void);

/* battle_actor_overlay_draw.cpp */
extern unsigned char g_BattleOverlay_Scale[16];
extern int g_BattleCurrentOrderingTable;
extern char g_BattleLoseMusicRequested;
extern char g_BattleActiveActorId;
extern char g_BattleStatusOverlayAttached;
extern char g_BattleEnemyTypeCount;
extern int g_BattleEnemyBankModelId0;
#define g_BattleEnemyBankModelIds (&g_BattleEnemyBankModelId0)
extern int g_BattleEnemyBankModelId1;
extern int g_BattleEnemyBankModelId2;
extern char g_BattleEnemyCount;
extern char g_BattleCameraScriptIndex;
extern int g_BattleOverlay_Word;
extern char g_BattleSequenceStepReady;
extern char g_BattleScriptAnimRunFlag;
extern short g_BattleOverlay_Short;
extern struct BattleCameraPosition g_BattleCameraPosition;
extern short g_BattleCameraPosZ;
extern char g_BattleLostEndState;
extern char g_BattleTargetFacingLocked;
extern short g_BattleActiveTaskSlotIdx;
extern char g_BattleHitReactionNoNumbers;
extern int g_BattleDrawFrameParity;
extern char g_BattleDebugDumpByte0;
extern char g_BattleDebugDumpByte1;
extern BattleTaskFn g_BattleMainTaskCallbacks[99];
extern char g_BattleDebugDumpByte2;
extern char g_BattleScriptBusy;
extern char g_EngineIsEffectLoading;
extern char g_BattleCameraFocusActor;
extern char g_BattleSequenceStepIndex;
extern char g_BattleEnemyBankLoadStep;
extern short g_BattleEffectActorMask;
extern short g_BattleSecondaryTaskActiveSlotIdx;
extern char g_BattleRoundSequenceState;
extern unsigned short g_BattleSceneFlags;
extern char g_BattleSequenceStepBusy;
extern int g_BattleOrderingTableA;
extern int g_BattleOrderingTableB;
extern char g_BattleSavedSpecialActorId;
extern struct BattleCameraFocalPoint g_BattleCameraFocalPoint;
extern short g_BattleCameraFocusZ;
extern short g_BattleNewTargetMask;
extern BattleTaskFn g_BattleSecondaryTaskCallbacks[60];
extern char g_BattleLimitBladeBeamTargetDiffers;
extern char g_BattleCameraIsMoving;
extern char g_BattleHitReactionInitZero;
extern short g_BattleRainActiveCount;
extern char g_BattleScriptEffectFlag;
extern short g_BattleActiveTargetMask;
void __cdecl Battle_DrawTargetedEnemyMarker(char targetIndex);

/* battle_actor_sequence_messages.cpp */
int __cdecl Battle_IsPartyFirstLayout(void);

/* battle_bin_section_decompress.cpp */
short __cdecl BinSectionPeekSize(void);

/* battle_camera_channel_scripts.cpp */
int __cdecl Battle_CameraAbsDelta(short first, short second);

/* battle_command_menu_queue.cpp */
short __cdecl Battle_GetActiveMenuActor(void);
void __cdecl Battle_ReopenQueuedCommandMenu(void);

/* battle_command_ring_atb.cpp */
void __cdecl Battle_EnqueueActionDirect(int actor, int priority, int commandId, int actionIdx, int targetMask);

/* battle_fx_alexander_summon.cpp */
extern struct BattleSummonAlexanderTable g_BattleSummonAlexanderTable;
extern struct BattleSummonAlexanderSubState g_BattleSummonAlexanderSubState;
extern struct BattleSummonAlexanderParticleBuffer g_BattleSummonAlexanderParticleBuffer;

/* battle_fx_blue_flame_thrower_effect.cpp */
int __cdecl BattleFx_BlueFlameThrowerClaimParticleRecord(void);

/* battle_fx_limit_healing_wind.cpp */
int __cdecl BattleFx_LimitHealingWindClaimParticleRecord(void);

/* battle_fx_magic_hitmark.cpp */
struct BattleFxMagicHitmarkParticle *__cdecl BattleFx_MagicHitmarkClaimParticleRecord(void);

/* battle_fx_magic_tornado.cpp */
struct BattleFxTornadoParticleRecord *__cdecl BattleFx_MagicTornadoClaimSpiralParticleRecord(void);
struct BattleFxTornadoParticleRecord *__cdecl BattleFx_MagicTornadoClaimStripParticleRecord(void);

/* battle_fx_shiva_summon.cpp */
int __cdecl BattleFx_SummonShivaUpdateParticleRecords(void);

/* battle_fx_special_vacuum.cpp */
struct BattleFxVacuumSegment;
struct BattleFxVacuumSegment *__cdecl BattleFx_SpecialVacuumClaimSegmentRecord(void);

/* battle_fx_special_spider_web.cpp */
int __cdecl BattleFx_SpecialSpiderWebClaimQuadRecord(void);

/* battle_fx_special_pollen.cpp */
int __cdecl BattleFx_SpecialPollenClaimParticleRecord(void);

/* battle_fx_special_four_laser.cpp */
int __cdecl BattleFx_SpecialFourLaserClaimQuadRecord(void);

/* battle_fx_summon_odin_zantetsuken.cpp */
extern struct BattleFxOdinZTaskSlot g_BattleSummonOdinZCameraEye;

/* battle_kernel_materia_support.cpp */
void __cdecl Kernel_AddMpTurboToAllMagicEntries(char amount);
void __cdecl Kernel_ApplyMagicMateria(char materiaId, int ap);
int __cdecl Kernel_FindOrAddCommandListSlot(unsigned char commandId);

/* battle_load_party_actors.cpp */
void __cdecl Battle_ReloadPartyActorsFromSavemap(void);

/* battle_opening_actor_setup.cpp */
void __cdecl Battle_BindEnemyActorToModel(char actorIndex);
void __cdecl Battle_ResetActorPose(char actorIndex);
void __cdecl Battle_RunEnemyActorScript(int actorId, int bankIndex, int scriptPtr);
void __cdecl Battle_PlaySfxCenteredMulti(int sfxId);

/* battle_particle_burst_effects.cpp */
void __cdecl Battle_SpawnBoneParticleBurst(int burstType, int actorIdx, int boneIdx, int zOffset, int delay, int duration);

/* battle_pending_drops.cpp */
extern struct BattlePendingDropRecord g_BattlePendingDropRecords[16];
short __cdecl Battle_PickRandomThrowableWeapon(void);

/* battle_random_source_helpers.cpp */
unsigned short __cdecl Battle_PickRandomTargetFromMask(unsigned short targetMask);

/* battle_random_stream_helpers.cpp */
unsigned char __cdecl Battle_NextRandomByte(void);
int __cdecl Battle_RandBelow(int limit);

/* battle_sequence_step_records.cpp */
int __cdecl Battle_AllocSequenceStep(void);
int __cdecl Battle_AllocHitRecord(void);
void __cdecl Battle_PushEmptyHitRecord(void);

/* battle_special_attack_effect.cpp */
int __cdecl Battle_RunSpecialAttackEffect(int effectId);

/* battle_fx_special_spade_card.cpp */
extern unsigned char g_BattleSpecialSpadeCardRightAxisFixed12[28];

/* battle_status_effect_handlers.cpp */
int __cdecl Battle_FindElementAffinityIndex(int actor, int elementMask, int statusMask);

/* battle_target_resolution.cpp */
void Battle_ResolveTargetMask(void);

/* sting.cpp */
extern struct RubyTentaclesCombinedMatrix g_RubyTentaclesCombinedMatrix;

/* vahamut0.cpp */
int __cdecl BattleFx_BzAimMatrixAlongDelta(void *srcVec, short stateValue, int scale);

struct BattleFxHellBubblesParticleRecord;
struct BattleFxBlueFlameThrowerParticle;
struct BattleFxColdBreathParticle;
struct BattleFxDemi3MultiMeshDraw;
struct BattleFxIfritPsxMesh;
struct BattleFxPollenParticleRecord;
struct BattleFxLimitHealingWindParticle;
struct BattleFxTornadoParticleRecord;
struct BattleFxHeartBreathParticleEmitter;
struct BattleFxPsxMeshDraw;
struct BattleFxShatterState;
struct BattleFxShivaParticle;
struct BattleFxSpecialWhirlpoolSpiralTrail;
struct BattleFxProceduralMesh;
struct BattleFxSandstormParticleRecord;
struct BattleFxTitanMeshInstance;
struct BattleOrthoMatrix;
struct BattleSummonLeviathanSpriteParticle;
struct MatrixParticleRecord;
struct VECTOR;
struct GzipHuffmanNode;

/* battle.cpp */
extern struct BattleHitRecord DAT_009acb98[128];
#define g_BattleEffectOrderTable DAT_009acb98

/* battle_camera_matrix_compose.cpp */
void __cdecl Battle_ComposeMatrixRelativeToCamera(short *cameraMatrix, void *matrix, struct PsxMatrix *dstMatrix);
void __cdecl Gte_OrthonormaliseFromYAxis(struct BattleOrthoMatrix *matrix);

/* battle_fx_alexander_summon.cpp */
int __cdecl BattleFx_AlexanderDrawPsxMeshDepthShaded(struct BattleFxPsxMeshDraw *meshDraw, int orderingTable, int depthShift, int packetCursor);
int __cdecl BattleFx_AlexanderDrawPsxMesh(struct BattleFxProceduralMesh *psxMesh, int orderingTable, int depthShift, int packetCursor);

/* battle_fx_blue_flame_thrower_effect.cpp */
void __cdecl BattleFx_BlueFlameThrowerDrawParticle(struct BattleFxFlameThrowerParticle *particle);

/* battle_fx_blue_pandoras_box_effect.cpp */
void __cdecl BattleFx_BluePandorasBoxDrawShatterFragments(struct BattleFxShatterState *drawState);

/* battle_fx_ifrit_summon.cpp */
int __cdecl BattleFx_SummonIfritDrawPsxMesh(struct BattleFxIfritPsxMesh *ifritMesh, int orderingTable, int depthShift, int packetCursor);

/* battle_fx_limit_healing_wind.cpp */
void __cdecl BattleFx_LimitHealingWindStepParticle(struct BattleFxLimitHealingWindParticle *particle);
void __cdecl BattleFx_LimitHealingWindDrawParticleRibbon(struct BattleFxLimitHealingWindParticle *particle);

/* battle_fx_magic_bio3.cpp */
int __cdecl BattleFx_MagicBio3DrawPsxMesh(struct BattleFxProceduralMesh *mesh, int orderingTable, int depthShift, int packetCursor);

/* battle_fx_magic_bio2.cpp */
int __cdecl BattleFx_MagicBio2DrawPsxMesh(struct BattleFxProceduralMesh *mesh, int orderingTable, int depthShift, int packetCursor);

/* battle_fx_magic_demi3_multi.cpp */
int __cdecl BattleFx_MagicDemi3MultiDrawPsxMesh(struct BattleFxDemi3MultiMeshDraw *meshDraw, int orderingTable, int depthShift, int packetCursor);

/* battle_fx_magic_demi3.cpp */
void __cdecl BattleFx_MagicDemi3DrawAndStepSegmentRecord(struct BattleFxMagicGravSegment *segment);

/* battle_fx_magic_hitmark.cpp */
void __cdecl BattleFx_MagicHitmarkDrawRadialTriangleBurst(struct Vec3sPacked *origin);

/* battle_fx_magic_quake3.cpp */
extern unsigned char g_BattleMagicQuake3FilenamesLight1[24];
extern unsigned char g_BattleMagicQuake3FilenamesLight2[24];
extern unsigned char g_BattleMagicQuake3FilenamesSmoke[24];
extern unsigned char g_BattleMagicQuake3FilenamesQueig1[24];
extern unsigned char g_BattleMagicQuake3FilenamesQueig0[24];
extern unsigned char g_BattleMagicQuake3FilenamesQueig0Alt[24];
int __cdecl BattleFx_MagicQuake3DrawPsxMesh(struct BattleFxQuake3MeshDraw *mesh, int orderingTable, int depthShift, int packetCursor);

/* battle_fx_magic_tornado.cpp */
void __cdecl BattleFx_MagicTornadoDrawAndStepSpiralParticle(struct BattleFxTornadoParticleRecord *particle);
void __cdecl BattleFx_MagicTornadoDrawAndStepStripParticle(struct BattleFxTornadoParticleRecord *stripState);

/* battle_fx_shiva_summon.cpp */
void __cdecl BattleFx_SummonShivaApplyForceFieldsToParticle(struct BattleFxShivaParticle *particle);

/* battle_fx_special_hell_bubbles.cpp */
void __cdecl BattleFx_SpecialHellBubblesDrawParticleSprites(struct BattleFxHellBubblesParticleRecord *emitter);

/* battle_fx_special_vacuum.cpp */
void __cdecl BattleFx_SpecialVacuumDrawSegmentRecord(struct BattleFxVacuumSegment *segment);

/* battle_fx_special_cold_breath.cpp */
void __cdecl BattleFx_SpecialColdBreathDrawParticleField(struct BattleFxColdBreathParticle *particle);

/* battle_fx_special_pollen.cpp */
void __cdecl BattleFx_SpecialPollenDrawPsxParticleDots(struct BattleFxPollenParticleRecord *particleRecord);

/* battle_fx_special_heart_breath_effect.cpp */
void __cdecl BattleFx_SpecialHeartBreathDrawParticleSprites(struct BattleFxHeartBreathParticleEmitter *emitterSlot);

/* battle_fx_special_sandstorm.cpp */
void __cdecl BattleFx_SpecialSandstormDrawParticleField(struct BattleFxSandstormParticleRecord *fieldRecord);

/* battle_fx_special_whirlpool.cpp */
void __cdecl BattleFx_SpecialWhirlpoolUpdateSpiralTrailParticle(struct BattleFxSpecialWhirlpoolSpiralTrail *trail);
void __cdecl BattleFx_SpecialWhirlpoolUpdateSecondaryParticle(struct BattleFxSpecialWhirlpoolSpiralTrail *particle);

/* battle_fx_summon_leviathan.cpp */
void __cdecl BattleFx_SummonLeviathanDrawAndStepSpriteParticle(struct BattleSummonLeviathanSpriteParticle *particle);

/* battle_fx_summon_ramuh.cpp */
int __cdecl BattleFx_SummonRamuhDrawPsxMesh(struct BattleFxProceduralMesh *mesh, int orderingTable, int depthShift, int packetCursor);

/* battle_fx_summon_titan.cpp */
int __cdecl BattleFx_SummonTitanDrawPsxMesh(struct BattleFxTitanMeshInstance *meshInst, int orderingTable, int depthBias, int listHead);

/* battle_gzip_inflate_decoder.cpp */
int __cdecl GzipInflate_BuildHuffmanTable(unsigned int *bitLengths, unsigned int n, unsigned int s, unsigned short *copyBases, unsigned short *extraBits, struct GzipHuffmanNode **t, unsigned int *m);
int __cdecl GzipInflate_DecodeHuffmanCodes(struct GzipHuffmanNode *tl, struct GzipHuffmanNode *td, unsigned bl, unsigned bd);

/* battle_kernel_experience_growth.cpp */
void __cdecl Kernel_InitJoiningCharacter(struct SavemapCharacter *charRecord);
void __cdecl Kernel_ApplyExpGrowthLevelUps(struct CharGrowth *growth);

/* battle_target_effect_matrices.cpp */
void __cdecl Battle_GetTargetMaskBoneCentroid(short actorMask, struct Vec3sPacked *centroid);

/* battle_vector_angle_math.cpp */
unsigned int __cdecl ComputeAngleBetweenVectors(struct VECTOR *vecA, struct VECTOR *vecB, struct VECTOR *crossOut);
void __cdecl Gte_BuildAxisAngleMatrix(int angle, struct PsxMatrix *matrix, struct VECTOR *axis);

/* yasui_limit_seal_evil_effect.cpp */
void __cdecl BattleFx_LimitSealEvilOrientedSpriteParticle(struct MatrixParticleRecord *particle);

struct BattleActorScreenPos;
struct BattleBlueFlameThrowerParticle;
struct BattleIce3Shatter;
struct BattleCommandListEntry;
struct BattleEffectStageCoords;
struct BattleFxActorOffscreenRequest;
struct BattleFxUltimaGradientDesc;
struct BattleFxBahamutBlendVertex;
struct BattleFxBahamutEulerRotation;
struct BattleFxFlareSpriteSlot;
struct BattleFxIce3MultiDebris;
struct BattleFxIce3MultiShatter;
struct BattleFxCidTrailSource;
struct BattleFxFadeRingBlock;
struct BattleFxFragmentedModelHeader;
struct BattleFxLandscaperMesh;
struct BattleFxHadesParticlePool;
struct BattleFxHadesPositionHistory;
struct BattleFxHadesPsxDrawCursor;
struct BattleFxHadesRibbon;
struct BattleFxHadesRingState;
struct BattleFxHadesSpriteParticle;
struct BattleFxHadesTube;
struct BattleFxBladeBeamTrailPoint;
struct BattleFxKjataDraw;
struct BattleFxPulseOfLifeSpriteScratch;
struct BattleFxPulseOfLifeTransformScratch;
struct BattleFxLimitMeteorainMeshRecord;
struct BattleFxLimitSledFangTrail;
struct BattleFxMagicFullCureSparkle;
struct BattleFxClearTranquilFadeSprite;
struct BattleFxClearTranquilGradientLine;
struct BattleFxClearTranquilPoolRecord;
struct BattleFxMeteorainGradientState;
struct BattleFxMoonWaveStrips;
struct BattleFxPandorasBoxFieldState;
struct BattleFxBadMouthDrawPrim;
struct BattleFxOdinDebrisSprite;
struct BattleFxOdinGungnirParticle;
struct BattleFxOdinSpriteTask;
struct BattleFxOdinZantetsukenActorPos;
struct BattleFxPhoenixOffscreenStage;
struct BattleFxProgram5Context;
struct BattleFxPsxDisplacedMeshRequest;
struct BattleFxPsxMeshResource;
struct BattleFxFreezeBreathDrawPrim;
struct BattleFxFreezeBreathDrawRecord;
struct BattleFxRibbonEmitterData;
struct BattleFxStardustRayCorner;
struct BattleFxScreenRect;
struct BattleFxPetrifyDrawPrim;
struct BattleFxShivaTriangleModelBlock;
struct BattleFxSpawnCenter;
struct BattleFxSpriteRequest;
struct BattleFxSummonTitanMeshInstance;
struct BattleFxThrowCidTrailRibbon;
struct BattleFxThrowTrailPoint;
struct BattleFxTrailState;
struct BattleFxTyphonBillboardRecord;
struct BattleFxTyphonBouncingRecord;
struct BattleFxScreenRect;
struct BattleFxZantetsukenMeshBlock;
struct BattleLimitFlashModel1MatrixTail;
struct BattleLimitGalianBeastModel;
struct BattleLimitLandscaperDrawRequest1;
struct BattleLimitLuckyGirlTrail;
struct BattleLimitSledFangTrailRing;
struct BattleLimitTifaRibbon;
struct BattleLimitTifaRingPoint;
struct BattleMagicFullParticle;
struct BattleMagicListRecord;
struct BattleFxFixedScaleVec;
struct BattlePsxDisplacedMeshPoly;
struct BattleFxEulerAngles16;
struct BattleSpecPollenSpriteReq1;
struct BattleSpecChefsKnifeAnimData;
struct BattleSpecialQuasarTable;
struct BattleSpecialScorchBasisMatrix;
struct BattleStageViewSize;
struct BattleFxMatrixBuildDesc;
struct BattleFxBzFlarePointSlot;
struct BattleSummonChocoMogEffectDescriptor;
struct BattleSummonKjataTrailRecord;
struct BattleSummonModelFile;
struct BattleSummonOdinGMatrixB168;
struct BattleSummonTitanFragment;
struct BattleTitanFragmentVertex;
struct GfxPsxShortVertexSlot;
struct MateriaStatPercentModifiers;
struct MenuPartyStats;

/* amptoanm.cpp */
int __cdecl BattleModel_ConvertPackedAnimation(void *frameCountOut, int flipFlag, int boneCount, int byteCount, struct Battle3dDescriptorFile *packedData, int dumpToFile, int animClass, char *animIndex);

/* battle_actor_overlay_draw.cpp */
extern struct BattleActorScreenPos g_BattleActorScreenPositions[10];
extern int g_BattleModelBankSlots[7];
void __cdecl Battle3d_DrawActorSpinningIndicator(char actorIndex);
void __cdecl Battle3d_UpdateActorRenderState(char actorIndex);
int __cdecl Battle3d_SelectStatusTintIndex(char actorIndex);
void __cdecl Battle3d_ApplyStatusTint(int actorIndex, int tintIndex);
void __cdecl Battle_DrawDamageNumber(short x, short y, short value, short rise, short flags);
char __cdecl Battle_SplitDamageDigits(short damage, short *digits);
void __cdecl BattleFx_DrawScreenBandWipe(short taskSlot);
int __cdecl BattleFx_DrawFullScreenPlate(unsigned char red, unsigned char green, unsigned char blue, char mode, int blendFactor);

/* battle_actor_sequence_messages.cpp */
int __cdecl Battle_GetActorSide(int actor);
short __cdecl Battle_RollEnemyDropOrSteal(void *enemy, int kind, int forceFlag, int scale);
int __cdecl Battle_FindSceneAttackIndex(int attackId);
void __cdecl Battle_InvalidateQueuedActionsForActor(int actorIndex, int threshold);

/* battle_ai_script_dispatch.cpp */
void __cdecl Battle_RunAiScriptEvent(int actor, int event, int eventParam);
void __cdecl Battle_HandleActorDeath(int attacker, int victim, int forced);

/* battle_bit_mask_helpers.cpp */
int __cdecl Battle_PickRandomBitFromMask(unsigned int mask);

/* battle_camera_channel_scripts.cpp */
void __cdecl Battle_PublishCameraForFrame(short cameraMode);
void __cdecl Battle_RunCameraEyeScript(char channel, short param, short scriptGroup);
void __cdecl Battle_CameraEyeQueueMoveToBone(short actorIndex, char channel, char *scriptPtr, int flags);
void __cdecl Battle_RunCameraFocusScript(char slot, short param, short mode);
void __cdecl Battle_CameraFocusQueueMoveToBone(short actorId, char slot, char *scriptPtr);
void __cdecl Battle_CameraEyeSnapToBone(short actorId, short boneIndex, short slot, char mode);
void __cdecl Battle_CameraFocusSnapToBone(short actorId, short boneIndex, short slot);
void __cdecl Battle_CameraEyeAddBoneOrigin(short actorId, short boneIndex, char slot, char mode);
void __cdecl Battle_CameraFocusAddBoneOrigin(short actorId, short boneIndex);
void __cdecl Battle_CameraEyeMoveTaskInit(short taskSlot);
void __cdecl Battle_CameraFocusMoveTaskInit(short taskSlot);

/* battle_effect_program_ribbons.cpp */
extern int g_BattleLimitTifaAttachPos;
extern short g_BattleLimitTifaSoundPosZ;
void __cdecl Battle_MotionOpStartEffectProgram0(int targetMask, int actorIdx);
void __cdecl Battle_MotionOpStartEffectProgram1(int targetMask, int actorIdx);
void __cdecl Battle_MotionOpStartEffectProgram2(int targetMask, int actorIdx);
void __cdecl Battle_MotionOpStartEffectProgram3(int targetMask, int actorIdx);
void __cdecl Battle_MotionOpStartEffectProgram4(int targetMask, int actorIdx);
void __cdecl Battle_MotionOpStartEffectProgram5(int targetMask, int actorIdx);
void __cdecl Battle_MotionOpStartEffectProgram6(int targetMask, int actorIdx);
void __cdecl Battle_EffectProgramResetRibbonBuffers(void);
void __cdecl Battle_EffectProgramAppendRibbonControlPoint(int ribbonIdx, struct Vec3sPacked *pointData);
void __cdecl Battle_EffectProgramAppendRibbonPathPoint(int ribbonIdx, struct Vec3sPacked *pointData);
void __cdecl Battle_EffectProgramAppendRibbonArcPoints(int ribbonIdx, struct Vec3sPacked *pt0, struct Vec3sPacked *pt1, int pt2);
void __cdecl Battle_EffectProgramDrawRibbonGradientQuads(void);
void __cdecl BattleFx_LimitTifaInitRingPointTables(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitTifaFillRingPointBuffer(int count, int radius);
void __cdecl BattleFx_LimitTifaFillRingPointsIntoBuffer(int count, int radius, struct BattleLimitTifaRingPoint *buffer);
void __cdecl Battle_SpawnActorEffectProgramTask(int targetMask, int actorIdx, int programIdx);
void __cdecl Battle_EffectProgram1Step(void);
void __cdecl Battle_ComputeAttachPointCameraOffset(int actorIdx, int attachBone, short *pOffset, short *pOut);
void __cdecl Battle_EffectProgramAnimatedSpriteEntry0Task(void);
void __cdecl Battle_EffectProgramTrailTask(void);
void __cdecl Battle_EffectProgramDrawTrailSectionQuads(struct BattleFxTrailState *context, int sectionIdx, int totalSections);
void __cdecl Battle_EffectProgramProjectTrailSectionPoints(struct BattleFxTrailState *context, int pt0, int pt1, int pt2);
void __cdecl Battle_EffectProgramPushTrailRingEntry(struct BattleLimitTifaRibbon *ringRecord, int pointData);
void __cdecl Battle_EffectProgram2Step(void);
void __cdecl Battle_EffectProgramTableScaledBillboardModelTask(void);
void __cdecl Battle_EffectProgramParticleEmitterTask(void);
void __cdecl Battle_EffectProgramDrawGradientTriangleFan(struct BattleFxRibbonEmitterData *context, int vertexCount, int angle, int color);
void __cdecl Battle_EffectProgramRandomRotatingTriangleTask(void);
void __cdecl Battle_EffectProgram2RibbonTask(void);
void __cdecl Battle_EffectProgram3Step(void);
void __cdecl Battle_EffectProgramAnimatedSpriteEntry1Task(void);
void __cdecl Battle_EffectProgram3ExpandingFadeRingTask(void);
void __cdecl Battle_EffectProgramDrawGradientQuadRing(struct BattleFxFadeRingBlock *context, int vertexCount, int angle, int color);
void __cdecl Battle_EffectProgram3RibbonTask(void);
void __cdecl Battle_EffectProgram4Step(void);
void __cdecl Battle_EffectProgramSpawnBoneWorldSpritePair(int boneIdx);
void __cdecl Battle_EffectProgram4ExpandingFadeModelTask(void);
void __cdecl Battle_EffectProgram4ActorOrbitTask(void);
void __cdecl Battle_EffectProgram5Step(void);
void __cdecl Battle_EffectProgram5RisingFadeSpriteTask(void);
void __cdecl Battle_EffectProgram5GrowingFadeModelTask(void);
void __cdecl Battle_EffectProgram5GrowingStretchFadeModelTask(void);
void __cdecl Battle_EffectProgram5AnimatedFadeModelTask(void);
int __cdecl Battle_EffectProgram5DrawPsxModel(struct BattleFxProgram5Context *modelData, int transformMatrix);
int __cdecl Battle_EffectProgram5DrawPsxGouraudTriangles(struct BattleFxProgram5Context *context, int vertices, int triangles, int blendMode);
void __cdecl Battle_EffectProgram6Step(void);
void __cdecl Battle_EffectProgram6DecayingFadeModelTask(void);
void __cdecl Battle_EffectProgram6GrowingFadeModelTask(void);
void __cdecl Battle_EffectProgram6SpinningGrowingFadeModelTask(void);
void __cdecl Battle_EffectProgram6AlignedModelTask(void);
void __cdecl Battle_EffectProgram6CarryTargetActorTask(void);
void __cdecl Battle_GetActorBoneYExtents(short actorIdx, short *pMinY, short *pMaxY);
void __cdecl Battle_EffectProgram7Step(void);
void __cdecl Battle_EffectProgram7RequestStageOffscreenRender(struct BattleEffectStageCoords *pMatrix, struct BattleStageViewSize *pAngles);
void __cdecl Battle_EffectProgram7TableScaledBillboardModelTask(void);
void __cdecl Battle_EffectProgram7RibbonTask(void);
void __cdecl Battle_EffectProgram7ActorTumbleTask(void);
void __cdecl Battle_EffectProgram7ArcingModelTask(void);
void __cdecl Battle_EffectProgram7SpinningStretchFadeModelTask(void);
void __cdecl Battle_EffectProgram7GrowingFadeModelTask(void);
void __cdecl Battle_EffectProgram7OscillatingDiscTask(void);
void __cdecl Battle_EffectProgram7ConvergingStreakTask(void);
int __cdecl Battle_FirstActorInMask(int actorMask);

/* battle_effect_summon_actors.cpp */
void __cdecl Battle_EffectAttachStatusOverlayModel(int effectParam, int unusedArg);
extern struct BattleSummonModelFile g_BattleSummonModelFileTable[18];

/* battle_fx_alexander_summon.cpp */
extern struct BattleFxMatrixBuildDesc g_BattleSummonAlexanderMatrixConfig;
int __cdecl BattleFx_SummonEnterAlexander(int targetMask, int actorIdx);

/* battle_fx_blue_beta_effect.cpp */
void __cdecl BattleFx_BlueBetaDrawParticleTrail(struct BattleFxBlueBetaParticleField *particleField, void *startPos, void *endPos);

/* battle_fx_blue_flame_thrower_attach_effect.cpp */
extern int g_BattleBlueFlameThrowerMagicId;
void __cdecl BattleFx_BlueBeginFlameThrowerAttach(int targetMask, int actorIdx);
void __cdecl BattleFx_BlueFlameThrowerAttachTimelineTask(void);
void __cdecl BattleFx_BlueFlameThrowerAttachDrawParticlesTask(void);
void __cdecl BattleFx_BlueFlameThrowerAttachDrawParticle(struct BattleBlueFlameThrowerParticle *particleRecord);
void __cdecl BattleFx_BlueFlameThrowerAttachClearParticleRecords(void);
void __cdecl BattleFx_BlueFlameThrowerAttachSpawnParticles(struct BattleFxSpawnCenter *count, struct Vec3sPacked *posX, int posY, int posZ);
int __cdecl BattleFx_BlueFlameThrowerAttachClaimParticleRecord(void);

/* battle_fx_blue_goblin_punch_effect.cpp */
void __cdecl BattleFx_BlueBeginGoblinPunch(int targetMask, int actorIdx);

/* battle_fx_blue_flame_thrower_effect.cpp */
void __cdecl BattleFx_BlueSpawnFlameThrowerPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_BlueFlameThrowerSpawnParticles(struct Vec3sPacked *origin, struct Vec3sPacked *direction, int angle, int count);
void __cdecl BattleFx_BlueSpawnFlameThrowerLeftPerTarget(int actor, int casterIndex);
void __cdecl BattleFx_BlueBeginFlameThrowerFang(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialBeginTwinBurner(int targetMask, int actorIdx);

/* battle_fx_blue_l4_suicide_effect.cpp */
void __cdecl BattleFx_BlueSpawnL4SuicidePerTarget(int actor, int sourceActor);

/* battle_fx_blue_matra_magic_effect.cpp */
void __cdecl BattleFx_BlueBeginMatraMagic(int targetMask, int actorIdx);

/* battle_fx_blue_pandoras_box_effect.cpp */
void __cdecl BattleFx_BluePandorasBoxRenderRecordPool(struct BattleFxPandorasBoxFieldState *record, int pool);

/* battle_fx_blue_laser_effect.cpp */
void __cdecl BattleFx_BlueBeginLaser(int targetMask, int actorIdx);

/* battle_fx_blue_shadow_flare_effect.cpp */
void __cdecl BattleFx_BlueShadowFlareDrawPsxFlatColorStrips(int red, int green, int blue);

/* battle_fx_chocomog_summon.cpp */
extern struct BattleSummonChocoMogEffectDescriptor g_BattleSummonChocoMogEffectDescriptor;

/* battle_limit_fx_thunks.cpp */
extern struct BattleLimitFlashModel1MatrixTail g_BattleLimitFlashModel1MatrixTail;
extern struct BattleLimitFlashModel1MatrixTail g_BattleLimitFlashModel2MatrixTail;

/* battle_fx_ifrit_summon.cpp */
int __cdecl BattleFx_SummonEnterIfrit(int targetMask, int actorIdx);

/* battle_fx_kjata_summon.cpp */
extern int g_BattleSummonKjataResourceTable;
extern int g_BattleSummonKjataScaleFixed12;
extern int g_BattleSummonKjataScale1;
extern int g_BattleSummonKjataScale2;
extern int g_BattleSummonKjataScale3;
void __cdecl BattleFx_SummonBeginKjata(int targetMask, int actorIdx);
void __cdecl BattleFx_KjataSpawnCameraScript(void *script, int matrix, int actorSlot);
void __cdecl BattleFx_KjataRunCameraScript(void);
short __cdecl BattleFx_KjataReadCameraScriptOperand(void *cursor);
int __cdecl BattleFx_KjataApplyCameraEasing(int value, int easeParam);
void __cdecl BattleFx_SummonKjataTimelineTask(void);
void __cdecl BattleFx_SummonKjataSpawnColorFade(void *valuePtr, int duration);
void __cdecl BattleFx_SummonKjataColorFadeTask(void);
void __cdecl BattleFx_SummonKjataSpawnBranchingTrailsTask(void);
void __cdecl BattleFx_SummonKjataBranchingTrailTask(void);
void __cdecl BattleFx_SummonKjataTrailRenderAndBlendTask(void);
int __cdecl BattleFx_SummonKjataDrawBlendedTriangles(struct BattleFxKjataDraw *draw, int reserved2, int reserved3, int reserved4, struct Vec3sPacked *trailSamples, struct Vec3sPacked *positions, struct BattleSummonKjataTrailRecord *trailRecords, short blend, short shade, void *worldMatrix, struct BattleFxOffsetTriMeshDesc *translation);
void __cdecl BattleFx_SummonKjataDrawBranchingTrailQuads(void);
void __cdecl BattleFx_SummonKjataTrailAnimatedSpriteTask(void);
int __cdecl BattleFx_SummonKjataInstallSpriteMatrix(void *vec, short offsetX, int scale);
void __cdecl BattleFx_SummonKjataUpdateTrailRecords(void);
void __cdecl BattleFx_SummonKjataBuildTrailTriangleArrays(void);
void __cdecl BattleFx_SummonKjataBlackFlashSpawnTask(void);
void __cdecl BattleFx_SummonKjataBlackModelFlashTask(void);
void __cdecl BattleFx_SummonKjataComputeTriangleNormalsTask(void);
void __cdecl BattleFx_SummonKjataFragmentedMeshTask(void);
int __cdecl BattleFx_SummonKjataDrawOffsetTriangles(struct BattleFxKjataDraw *draw, int reserved2, int reserved3, int reserved4, struct Vec3sPacked *positions, struct BattleSummonKjataTrailRecord *trailRecords, short scale, short shade, void *worldMatrix, struct BattleFxOffsetTriMeshDesc *translation);
int __cdecl BattleFx_SummonKjataDrawOffsetTrianglesWithSprites(struct BattleFxKjataDraw *draw, int reserved2, int reserved3, int reserved4, struct Vec3sPacked *offsets, short scale, short shade, void *worldMatrix, struct BattleFxOffsetTriMeshDesc *translation);
int __cdecl BattleFx_SummonKjataDrawPsxMesh(struct BattleFxKjataDraw *kjataDraw, int orderingTable, int depthShift, int packetCursor);
void __cdecl BattleFx_SummonKjataSpawnBlackModelsTask(void);
void __cdecl BattleFx_SummonKjataBlackModelGrowingTask(void);
void __cdecl BattleFx_SummonKjataBlackModelExpandingTask(void);
void __cdecl BattleFx_SummonKjataFragmentSpritesTask(void);
int __cdecl BattleFx_SummonKjataDrawTriangleFragmentSprites(struct BattleFxKjataDraw *kjataDraw, int unusedA, int unusedB, int drawHead, struct Vec3sPacked *offsets, short scale, int matrix, struct BattleFxOffsetTriMeshDesc *localMatrix, void *sprite);
void __cdecl BattleFx_SummonKjataBlackModel1ScalingFadeTask(void);
void __cdecl BattleFx_SummonKjataSpawnStretchingFadeModelsTask(void);
void __cdecl BattleFx_SummonKjataBlackModel2StretchingFadeTask(void);
void __cdecl BattleFx_SummonKjataClearParticleSpriteRecords(void);
void __cdecl BattleFx_SummonKjataParticleSpriteArrayTask(void);
void __cdecl BattleFx_SummonKjataSpawnSpriteParticlesTask(void);
void __cdecl BattleFx_SummonKjataHoppingSpriteTask(void);
int __cdecl BattleFx_SummonKjataClaimParticleSpriteRecord(void *payload);
void __cdecl BattleFx_SummonKjataCounterRotatingModelsTask(void);

/* battle_fx_limit_galian_beast.cpp */
void __cdecl BattleFx_LimitBeginGalianBeast(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitGalianBeastSpawnTimeline(void);
void __cdecl BattleFx_LimitGalianBeastTimelineTask(void);
void __cdecl BattleFx_LimitGalianBeastSpinningModelPairTask(void);
int __cdecl BattleFx_LimitGalianBeastDrawPsxMesh(struct BattleLimitGalianBeastModel *meshDesc);
int __cdecl BattleFx_LimitGalianBeastDrawPsxTexturedTriangles(struct BattleLimitGalianBeastModel *meshDesc, char *vertList, int *triList);

/* battle_fx_limit_boost_jump.cpp */
extern int g_BattleLimitBoostJumpResourceDef;
extern struct BattleFxSpriteRequest g_BattleLimitBoostJumpSpriteReqPool[2];
extern struct GfxTableCursor *g_BattleLimitBoostJumpLoadedResTable;
void __cdecl BattleFx_LimitRegisterBoostJumpPerTargetCallback(int targetMask, int sourceActor);
void __cdecl BattleFx_LimitSpawnBoostJumpPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_LimitBoostJumpTimelineTask(void);
void __cdecl BattleFx_LimitBoostJumpSpriteAnimationTask(void);
void __cdecl BattleFx_LimitBoostJumpMovingSpriteTask(void);

/* battle_fx_limit_cosmo_memory.cpp */
extern struct PsxMatrix g_BattleLimitCosmoMemoryLoadedResTable;
extern int g_BattleLimitCosmoMemoryCentroidModelRotation;
extern int g_BattleLimitCosmoMemoryExpandingModelRotation;
extern int g_BattleLimitCosmoMemoryConvergingRotation;
void __cdecl BattleFx_LimitBeginCosmoMemory(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitCosmoMemoryTimelineTask(void);
void __cdecl BattleFx_LimitCosmoMemoryScreenFlashTask(void);
void __cdecl BattleFx_LimitCosmoMemoryGrowingModelTask(void);
void __cdecl BattleFx_LimitCosmoMemoryCentroidGrowingModelTask(void);
void __cdecl BattleFx_LimitCosmoMemoryShrinkingCyclingModelsTask(void);
void __cdecl BattleFx_LimitCosmoMemoryExpandingModelTask(void);
void __cdecl BattleFx_LimitCosmoMemoryAnimatedBillboardSpriteTask(void);
void __cdecl BattleFx_LimitCosmoMemoryFallingSpinningModelTask(void);
void __cdecl BattleFx_LimitCosmoMemoryEmitConvergingProjectilesTask(void);
void __cdecl BattleFx_LimitCosmoMemoryProjectileTask(void);
void __cdecl BattleFx_LimitCosmoMemoryFadingBillboardSpriteTask(void);
void __cdecl BattleFx_LimitCosmoMemoryRotatingSpriteRingsTask(void);
void __cdecl BattleFx_LimitCosmoMemoryTriModelSpinTask(void);

/* battle_fx_limit_dragon_dive.cpp */
extern int g_BattleLimitDragonDiveRotationMatrix;
void __cdecl BattleFx_LimitDragonDiveBlendPolygonSetVertices(int index, int unusedArg, struct BattleFxHellBomberEndVertex *dst);
void __cdecl BattleFx_LimitBeginDragonDive(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitDragonDiveTimelineTask(void);
void __cdecl BattleFx_LimitDragonDiveImpactTask(void);
void __cdecl BattleFx_LimitDragonDiveScreenFlashTask(void);
void __cdecl BattleFx_LimitDragonDiveScalingModelTask(void);
void __cdecl BattleFx_LimitDragonDiveVerticalMovingSpriteTask(void);

/* battle_fx_limit_dice.cpp */
void __cdecl BattleFx_LimitBeginDice(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitDiceSpawnTimeline(void);
void __cdecl BattleFx_LimitDiceTimelineTask(void);
void __cdecl BattleFx_LimitDiceTumblingDieTask(void);
int __cdecl BattleFx_LimitDiceCountHopFrames(int position, int velocity, int accel, int target);

/* battle_fx_limit_dragon.cpp */
extern int g_BattleLimitDragonDrawListHead;
extern short g_BattleLimitDragonTrailFade;
extern struct GfxTableCursor *g_BattleLimitDragonLoadedResTable;
extern short g_BattleLimitDragonTrailWriteIndex;
extern int g_BattleLimitDragonPulseAngles;
void __cdecl BattleFx_LimitRegisterDragonPerTargetCallback(int targetMask, int sourceActor);
void __cdecl BattleFx_LimitSpawnDragonPerTarget(int actor, int sourceActor);
void __cdecl BattleFx_LimitDragonTimelineTask(void);
void __cdecl BattleFx_LimitDragonTrailingProjectileTask(void);
void __cdecl BattleFx_LimitDragonVariantSpriteTask(void);
void __cdecl BattleFx_LimitDragonPulsingModelTask(void);
void __cdecl BattleFx_LimitDragonPulsingRadialSpriteTask(void);
void __cdecl BattleFx_LimitDragonVariantSpriteEmitterTask(void);

/* battle_fx_limit_all_creation.cpp */
extern short g_BattleLimitAllCreationScale2;
void __cdecl BattleFx_LimitAllCreationBlendPolygonSetVertices(int index, int unusedArg, struct BattleFxHellBomberEndVertex *dst);
void __cdecl BattleFx_LimitBeginAllCreation(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitAllCreationTimelineTask(void);
void __cdecl BattleFx_LimitAllCreationShrinkingModelTask(void);
void __cdecl BattleFx_LimitAllCreationScalingModelTask(void);
void __cdecl BattleFx_LimitAllCreationScreenFlashTask(void);

/* battle_fx_limit_dynamite.cpp */
extern int g_BattleLimitDynamiteResourceDef;
extern int g_BattleLimitDynamiteScale;
extern short g_BattleLimitDynamiteStickAngles[3];
void __cdecl BattleFx_LimitDynamiteGetActorBoneRotation(int actorIdx, int boneIdx, short *rotAngles);
void __cdecl BattleFx_LimitBeginDynamite(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitDynamiteTimelineTask(void);
void __cdecl BattleFx_LimitDynamiteBallisticModelTask(void);
void __cdecl BattleFx_LimitDynamiteDrawLivewireLines(struct Vec3sPacked *position, void *rotAngle, short extraAngle);
void __cdecl BattleFx_LimitDynamiteActorBoneModelTask(void);
void __cdecl BattleFx_LimitDynamiteBouncingSpriteTask(void);
void __cdecl BattleFx_LimitDynamiteGrowingModelTask(void);

/* battle_fx_limit_earth_rave_effect.cpp */
void __cdecl BattleFx_LimitBeginEarthRave(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitEarthRaveSpawnTimeline(void);
void __cdecl BattleFx_LimitEarthRaveTimelineTask(void);
void __cdecl BattleFx_LimitEarthRaveImpactBurstTask(void);
void __cdecl BattleFx_LimitEarthRaveSpriteParticlePool100Task(void);
void __cdecl BattleFx_LimitEarthRaveDirectionalSpriteTask(void);
void __cdecl BattleFx_LimitEarthRaveSpinningShrinkingModelTask(void);
void __cdecl BattleFx_LimitEarthRaveTumblingModelTask(void);
int __cdecl BattleFx_LimitEarthRaveComputeStandPointByFacing(int sequenceIndex);
int __cdecl BattleFx_LimitEarthRaveComputeStandPointByBearing(int sequenceIndex);

/* battle_fx_limit_landscaper.cpp */
extern int g_BattleLimitLandscaperResourceDef;
extern unsigned char g_BattleLimitLandscaperBaseDistance[8];
extern struct BattleLimitLandscaperDrawRequest1 g_BattleLimitLandscaperDrawRequest1;
extern int g_BattleLimitLandscaperGrowAngles;
extern int g_BattleLimitLandscaperShrinkAngles;
void __cdecl BattleFx_LimitBeginLandscaper(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitLandscaperSaveActorPositionsFindNearestPoint(void);
void __cdecl BattleFx_LimitLandscaperInitFragmentTable(void);
void __cdecl BattleFx_LimitLandscaperTimelineTask(void);
void __cdecl BattleFx_LimitLandscaperQueueStageOffscreenRequests(short *modelPointers, struct BattleFxScreenRect *viewport);
void __cdecl BattleFx_LimitLandscaperTravelingSpriteEmitterTask(void);
void __cdecl BattleFx_LimitLandscaperBouncingSpriteTask(void);
void __cdecl BattleFx_LimitLandscaperAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitLandscaperGrowingModelTask(void);
void __cdecl BattleFx_LimitLandscaperShrinkingModelTask(void);
void __cdecl BattleFx_LimitLandscaperFragmentMorphTask(void);
int __cdecl BattleFx_LimitLandscaperDrawPsxMeshPerTriangleTransform(struct BattleFxLandscaperMesh *mesh, int arg2, int arg3, int arg4, struct Vec3sPacked *offsets, void *matrix, struct BattleGteViewMatrixScratch *translation);
int __cdecl BattleFx_LimitLandscaperDrawPsxMesh(struct BattleFxLandscaperMesh *mesh, int arg2, int arg3, int arg4);
void __cdecl BattleFx_LimitLandscaperActorFallTask(void);
void __cdecl BattleFx_LimitLandscaperUpdateFragmentPositions(void);
void __cdecl BattleFx_LimitLandscaperFlashShakeTask(void);
void __cdecl BattleFx_LimitLandscaperCameraShakeTask(void);

/* battle_fx_limit_finishing_touch.cpp */
extern int g_BattleLimitFinishingTouchResourceDef;
void __cdecl BattleFx_LimitBeginFinishingTouch(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitFinishingTouchSpawnTimeline(void);
void __cdecl BattleFx_LimitFinishingTouchTimelineTask(void);
void __cdecl BattleFx_LimitFinishingTouchRingChainTask(void);
void __cdecl BattleFx_LimitFinishingTouchFallingModelTask(void);
void __cdecl BattleFx_LimitFinishingTouchInitRingPackets(void);
void __cdecl BattleFx_LimitFinishingTouchDrawRingChain(int phase, int grayLevel);
void __cdecl BattleFx_LimitFinishingTouchUpdateRingChain(int height, int radius, int xScale, int falloff);
void __cdecl BattleFx_LimitFinishingTouchTransformAttachPointToEffectSpace(int actorId, int attachIndex, short *srcVec, short *outVec);
void __cdecl BattleFx_LimitFinishingTouchRadialSpriteEmitterTask(void);
void __cdecl BattleFx_LimitFinishingTouchActorLiftReturnTask(void);
void __cdecl BattleFx_LimitFinishingTouchActorRadialSpriteBurstTask(void);
void __cdecl BattleFx_LimitFinishingTouchScaledModelTask(void);
void __cdecl BattleFx_LimitFinishingTouchGrowShrinkModelTask(void);
void __cdecl BattleFx_LimitFinishingTouchMaskGatedExpiryTask(void);
void __cdecl BattleFx_LimitFinishingTouchSelectDrawCursorTask(void);
void __cdecl BattleFx_LimitBeginFinishingTouchAlt(int targetMask, int actorIdx);

/* battle_fx_limit_highwind.cpp */
extern int g_BattleLimitHighwindResourceDef;
extern short g_BattleLimitHighwindTargetCentroid[2];
extern short g_BattleLimitHighwindTargetCentroidZ;
extern int g_BattleLimitHighwindDrawHandle;
extern short g_BattleLimitHighwindTargetBits;
extern short g_BattleLimitHighwindFlashLevel;
extern struct GfxTableCursor *g_BattleLimitHighwindModelTable;
extern short g_BattleLimitHighwindHitIndex;
extern short g_BattleLimitHighwindShrinkRotation[3];
void __cdecl BattleFx_LimitBeginHighwind(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitHighwindTimelineTask(void);
void __cdecl BattleFx_LimitHighwindScreenFlashTask(void);
void __cdecl BattleFx_LimitHighwindSpriteAnimationEmitterTask(void);
void __cdecl BattleFx_LimitHighwindSpriteAnimationTask(void);
void __cdecl BattleFx_LimitHighwindProjectileTask(void);
void __cdecl BattleFx_LimitHighwindProjectileImpactSpriteTask(void);
void __cdecl BattleFx_LimitHighwindProjectileTrailSpriteTask(void);
void __cdecl BattleFx_LimitHighwindShrinkingModelTask(void);

/* battle_fx_limit_doom_of_the_living.cpp */
void __cdecl BattleFx_LimitBeginDoomOfTheLiving(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitDoomOfTheLivingTimelineTask(void);
void __cdecl BattleFx_LimitDoomOfTheLivingAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitDoomOfTheLivingFadingParticleSpriteTask(void);
void __cdecl BattleFx_LimitDoomOfTheLivingSpawnPulsingModelPairTask(void);
void __cdecl BattleFx_LimitDoomOfTheLivingPulsingModelPairTask(void);

/* battle_fx_limit_hyper_jump.cpp */
extern int g_BattleLimitHyperJumpZeroAnglesC;
void __cdecl BattleFx_LimitHyperJumpBlendPolygonSetVertices(int index, int unusedArg, struct BattleFxHellBomberEndVertex *dst);
void __cdecl BattleFx_LimitBeginHyperJump(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitHyperJumpInitParticleVelocities(void);
void __cdecl BattleFx_LimitHyperJumpTimelineTask(void);
void __cdecl BattleFx_LimitHyperJumpSpriteAnimationTask(void);
void __cdecl BattleFx_LimitHyperJumpMovingSpriteTask(void);
void __cdecl BattleFx_LimitHyperJumpScalingModelTask(void);
void __cdecl BattleFx_LimitHyperJumpOscillatingModelTask(void);
void __cdecl BattleFx_LimitHyperJumpGrowingModelsTask(void);
void __cdecl BattleFx_LimitHyperJumpDisplacedMeshTask(void);
int __cdecl BattleFx_LimitHyperJumpDrawPsxDisplacedMesh(struct BattleFxPsxDisplacedMeshRequest *request, int unusedArg2, int unusedArg3, struct BattlePsxDisplacedMeshPoly *unusedArg4, struct Vec3sPacked *offsets, void *rotMatrix, struct BattleGteViewMatrixScratch *transMatrix);
void __cdecl BattleFx_LimitHyperJumpStepBouncingParticles(void);
void __cdecl BattleFx_LimitHyperJumpBoneOrientedModelTask(void);

/* battle_fx_limit_howling_moon.cpp */
void __cdecl BattleFx_LimitBeginHowlingMoon(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitHowlingMoonSpawnTimeline(void);
void __cdecl BattleFx_LimitHowlingMoonTimelineTask(void);
void __cdecl BattleFx_LimitHowlingMoonFadingBillboardSpriteTask(void);
void __cdecl BattleFx_LimitHowlingMoonWaveStripTask(void);
void __cdecl BattleFx_LimitHowlingMoonDrawWaveStrips(struct BattleFxMoonWaveStrips *taskData);

/* battle_fx_limit_gauntlet.cpp */
extern int g_BattleLimitGauntletResourceDef;
extern short g_BattleLimitGauntletScaleAlpha;
extern int g_BattleLimitGauntletCentroidX;
extern short g_BattleLimitGauntletCentroidZ;
extern int g_BattleLimitGauntletGouraudLineDrawHandle;
extern short g_BattleLimitGauntletFadeStep2;
extern short g_BattleLimitGauntletTargetMask;
extern short g_BattleLimitGauntletFadeStep1;
extern struct GfxTableCursor *g_BattleLimitGauntletLoadedResTable;
extern short g_BattleLimitGauntletRotationY1[3];
extern short g_BattleLimitGauntletRotationY2[3];
void __cdecl BattleFx_LimitBeginGauntlet(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitGauntletTimelineTask(void);
void __cdecl BattleFx_LimitGauntletGrowingModelTask(void);
void __cdecl BattleFx_LimitGauntletScalingModelTask(void);
void __cdecl BattleFx_LimitGauntletGouraudLineTask(void);

/* battle_fx_limit_greased_lightning.cpp */
extern int g_BattleLimitGreasedLightningResourceDef;
extern struct GfxTableCursor *g_BattleLimitGreasedLightningLoadedResTable;
void __cdecl BattleFx_LimitRegisterGreasedLightningPerTargetCallback(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSpawnGreasedLightningPerTarget(int actor, int actorIdx);
void __cdecl BattleFx_LimitGreasedLightningTimelineTask(void);
void __cdecl BattleFx_LimitGreasedLightningSpriteAnimationTask(void);

/* battle_fx_limit_pulse_of_life.cpp */
extern int g_BattleLimitPulseOfLifeResourceKind;
extern unsigned char g_BattleLimitPulseOfLifeActiveFlag;
extern unsigned char g_BattleLimitPulseOfLifeStateFlag;
extern struct BattleFxPulseOfLifeSpriteScratch *g_BattleLimitPulseOfLifeSpriteScratch;
extern int g_BattleLimitPulseOfLifeDrawListHandle;
extern struct BattleFxPulseOfLifeTransformScratch *g_BattleLimitPulseOfLifeTransformScratch;
extern struct GfxTableCursor *g_BattleLimitPulseOfLifeLoadedRes;
void __cdecl BattleFx_LimitBeginPulseOfLife(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSpawnPulseOfLifePerTarget(int actor);
void __cdecl BattleFx_LimitPulseOfLifeTimelineTask(void);
void __cdecl BattleFx_LimitPulseOfLifePulsingModelTask(void);
void __cdecl BattleFx_LimitPulseOfLifeSpawnRisingSpriteTask(void);
void __cdecl BattleFx_LimitPulseOfLifeRisingSpinningSpriteTask(void);
void __cdecl BattleFx_LimitPulseOfLifeAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitPulseOfLifeSoundTask(void);

/* battle_fx_limit_lucky_girl.cpp */
void __cdecl BattleFx_LimitBeginLuckyGirl(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitLuckyGirlSpawnTimeline(void);
void __cdecl BattleFx_LimitLuckyGirlTimelineTask(void);
void __cdecl BattleFx_LimitLuckyGirlFadingBillboardSpriteTask(void);
void __cdecl BattleFx_LimitLuckyGirlTrailingSpriteTask(void);
void __cdecl BattleFx_LimitLuckyGirlPushTrailPosition(struct BattleLimitLuckyGirlTrail *trail, int position);
void __cdecl BattleFx_LimitLuckyGirlSineScaledSpriteTask(void);

/* battle_fx_limit_lunatic_high.cpp */
extern int g_BattleLimitLunaticHighResourceDef;
void __cdecl BattleFx_LimitBeginLunaticHigh(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitLunaticHighTimelineTask(void);
void __cdecl BattleFx_LimitLunaticHighSpawnEffectsTask(void);
void __cdecl BattleFx_LimitLunaticHighSpriteParticlePool100Task(void);
void __cdecl BattleFx_LimitLunaticHighDrawModelEntry1Task(void);
void __cdecl BattleFx_LimitLunaticHighDrawModelEntry2Task(void);

/* battle_fx_limit_bloodfest.cpp */
extern int g_BattleLimitBloodfestResourceDef;
extern int g_BattleLimitBloodfestCentroidX;
void __cdecl BattleFx_LimitBeginBloodfest(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBloodfestTimelineTask(void);
void __cdecl BattleFx_LimitBloodfestSpriteAnimationTask(void);

/* battle_fx_limit_clear_tranquil.cpp */
extern int g_BattleLimitClearTranquilModelReq[];
extern int g_BattleLimitClearTranquilPairAngles;
void __cdecl BattleFx_LimitBeginClearTranquil(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitClearTranquilTimelineTask(void);
void __cdecl BattleFx_LimitClearTranquilUpdateSpriteRecords(void);
void __cdecl BattleFx_LimitClearTranquilUpdateFadingSpriteRecord(struct BattleFxClearTranquilFadeSprite *fadeSprite);
void __cdecl BattleFx_LimitClearTranquilUpdateGradientLineRecord(struct BattleFxClearTranquilGradientLine *gradientLine);
void __cdecl BattleFx_LimitClearTranquilGrowingModelPairTask(void);
struct BattleFxClearTranquilPoolRecord *__cdecl BattleFx_LimitClearTranquilClaimEffectRecord(short tag);

/* battle_fx_limit_meteorain.cpp */
void __cdecl BattleFx_LimitBeginMeteorain(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitMeteorainSpawnTimeline(void);
int __cdecl BattleFx_LimitMeteorainAssignMeshTextureGradient(struct BattleFxMeteorainGradientState *gradState);
int __cdecl BattleFx_LimitMeteorainAssignAngularTextureGradient(struct BattleFxMeteorainGradientState *gradientState, int vertexBase, int descriptor);
void __cdecl BattleFx_LimitMeteorainTimelineTask(void);
void __cdecl BattleFx_LimitMeteorainShrinkingBlackModelTask(void);
void __cdecl BattleFx_LimitMeteorainGrowingPsxMeshTask(void);
int __cdecl BattleFx_LimitMeteorainDrawPsxMesh(struct BattleFxLimitMeteorainMeshRecord *mesh, int orderingTable, int depthShift, int packetCursor);
void __cdecl BattleFx_LimitMeteorainRisingSpriteQuadTask(void);
void __cdecl BattleFx_LimitMeteorainTargetTimelineTask(void);
void __cdecl BattleFx_LimitMeteorainProjectileTask(void);
void __cdecl BattleFx_LimitMeteorainLivewireDotTask(void);
void __cdecl BattleFx_LimitMeteorainSpriteParticlePool64Task(void);
void __cdecl BattleFx_LimitMeteorainSpinningFadingModelEntry1Task(void);
void __cdecl BattleFx_LimitMeteorainSpinningFadingModelEntry2Task(void);
void __cdecl BattleFx_LimitMeteorainGrowingModelTask(void);

/* battle_fx_limit_mindblow.cpp */
extern int g_BattleLimitMindblowResourceKind;
extern int g_BattleLimitMindblowMatrix;
extern int g_BattleLimitMindblowDrawHandle;
extern struct GfxTableCursor *g_BattleLimitMindblowLoadedResTable;
extern int g_BattleLimitMindblowModel1Rot;
extern short g_BattleLimitMindblowModel1RotZ;
extern int g_BattleLimitMindblowModel2Rot;
extern short g_BattleLimitMindblowModel2RotZ;
void __cdecl BattleFx_LimitRegisterMindblowPerTargetCallback(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSpawnMindblowPerTarget(int actor, int actorIdx);
void __cdecl BattleFx_LimitMindblowTimelineTask(void);
void __cdecl BattleFx_LimitMindblowSpinningModelPairTask(void);
void __cdecl BattleFx_LimitMindblowDriftingSpriteTask(void);
void __cdecl BattleFx_LimitMindblowDampedSpriteParticleTask(void);

/* battle_fx_limit_fury_brand.cpp */
extern int g_BattleLimitFuryBrandResourceDef;
void __cdecl BattleFx_LimitBeginFuryBrand(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitSpawnFuryBrandPerTarget(int actor);
void __cdecl BattleFx_LimitFuryBrandTimelineTask(void);
void __cdecl BattleFx_LimitFuryBrandFadingSpriteTask(void);
void __cdecl BattleFx_LimitFuryBrandAnimatedSpriteTask(void);
void __cdecl BattleFx_LimitFuryBrandSpawnFadingModelsTask(void);
void __cdecl BattleFx_LimitFuryBrandFadingModelTask(void);
void __cdecl BattleFx_LimitFuryBrandGrowingFadeModelTask(void);
void __cdecl BattleFx_LimitFuryBrandSoundTimelineTask(void);

/* battle_fx_limit_big_brawl.cpp */
extern int g_BattleLimitBigBrawlResourceDef;
extern struct BattleFxSpriteRequest g_BattleLimitBigBrawlSpriteRequest;
extern int g_BattleLimitBigBrawlCentroidX;
extern short g_BattleLimitBigBrawlCentroidY;
extern short g_BattleLimitBigBrawlTargetMask;
extern struct GfxTableCursor *g_BattleLimitBigBrawlLoadedResTable;
extern short g_BattleLimitBigBrawlTimelineFrame;
void __cdecl BattleFx_LimitBeginBigBrawl(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitBigBrawlTimelineTask(void);
void __cdecl BattleFx_LimitBigBrawlMovingSpriteTask(void);

/* battle_fx_limit_stardust_ray.cpp */
void __cdecl BattleFx_LimitBeginStardustRay(int targetMask, int actorIdx);
void __cdecl BattleFx_LimitStardustRaySelectDrawListBufferTask(void);
void __cdecl BattleFx_LimitStardustRaySpawnTimeline(void);
void __cdecl BattleFx_LimitStardustRayTimelineTask(void);
void __cdecl BattleFx_LimitStardustRaySpawnSpriteBurstTask(void);
void __cdecl BattleFx_LimitStardustRayBillboardSpriteTask(void);
void __cdecl BattleFx_LimitStardustRayDrawMovingModelTask(void);
void __cdecl BattleFx_LimitStardustRayTexturedQuadPool30Task(void);
void __cdecl BattleFx_LimitStardustRayDrawTexturedQuad(struct BattleFxStardustRayCorner *cornerPtr, int variant, int commandFlags);
void __cdecl BattleFx_LimitStardustRayDrawGrowingModelTask(void);
void __cdecl BattleFx_LimitStardustRaySpokeParticlePool180Task(void);
void __cdecl BattleFx_LimitStardustRaySpriteParticlePool120Task(void);
void __cdecl BattleFx_LimitStardustRaySpriteEntry2ParticlePool120Task(void);

/* battle_fx_limit_sled_fang.cpp */
void __cdecl BattleFx_LimitBeginSledFang(int targetMask, int actorIdx, int isAlt);
void __cdecl BattleFx_LimitSledFangSpawnTimeline(void);
void __cdecl BattleFx_LimitSledFangClearTrailRings(void);
void __cdecl BattleFx_LimitSledFangTimelineTask(void);
void __cdecl BattleFx_LimitSledFangPushTrailPoint(struct BattleLimitSledFangTrailRing *ringBuffer, void *pointValue);
void __cdecl BattleFx_LimitSledFangSlashTrailTask(void);
void __cdecl BattleFx_LimitSledFangDrawTrailSegmentQuad(struct BattleFxLimitSledFangTrail *context, int segmentIdx, int totalSegments);
void __cdecl BattleFx_LimitSledFangProjectTrailEdgePoints(struct BattleFxLimitSledFangTrail *context, void *pt0, struct GfxPsxShortVertexSlot *pt1);
void __cdecl BattleFx_LimitSledFangSpawnImpactParticlesTask(void);
void __cdecl BattleFx_LimitSledFangDrawParticlesTask(void);
void __cdecl BattleFx_LimitBloodFangSpawnTimeline(void);
void __cdecl BattleFx_LimitBloodFangTimelineTask(void);
void __cdecl BattleFx_LimitBloodFangSlashTrailTask(void);
void __cdecl BattleFx_LimitSledFangSpawnSpritesAndParticlesTask(void);
void __cdecl BattleFx_LimitSledFangFlatMovingSpriteTask(void);

/* battle_fx_limit_toy_box.cpp */
void __cdecl BattleFx_LimitBeginToyBox(int targetMask, int actorIdx, int variant);
void __cdecl BattleFx_LimitToyBoxASetupTask(void);
void __cdecl BattleFx_LimitToyBoxADrawModelTask(void);
void __cdecl BattleFx_LimitToyBoxAEmitSpriteQuadsTask(void);
void __cdecl BattleFx_LimitToyBoxASpriteQuadTask(void);
void __cdecl BattleFx_LimitToyBoxBSetupTask(void);
void __cdecl BattleFx_LimitToyBoxSharedDrawModelTask(void);
void __cdecl BattleFx_LimitToyBoxEmitSpritesTask(void);
void __cdecl BattleFx_LimitToyBoxEmittedSpriteTask(void);
void __cdecl BattleFx_LimitToyBoxCSetupTask(void);
void __cdecl BattleFx_LimitToyBoxDSetupTask(void);

/* battle_fx_magic_ultima.cpp */
int __cdecl BattleFx_MagicUltimaDrawPsxGouraudQuadGradientRows(struct BattleFxUltimaGradientDesc *effectDesc, short skipRows);

/* battle_fx_magic_ice3.cpp */
int __cdecl BattleFx_MagicIce3DrawSingleTargetShatterModel(struct BattleIce3Shatter *shatter, int threshold);
int __cdecl BattleFx_MagicIce3DrawSingleTargetShatterFragments(struct BattleIce3Shatter *shatter, int blockBase, int entryBlock, int threshold);
void __cdecl BattleFx_MagicIce3BuildScaledEulerRotationMatrix(struct BattleFxEulerAngles16 *angles, struct BattleRotationMatrix *matrix, struct BattleFxFixedScaleVec *scaleVec);
int __cdecl BattleFx_MagicIce3SineLookup(int angle);

/* battle_fx_magic_ice3_multi.cpp */
void __cdecl BattleFx_MagicLoadAndBeginIce3Multi(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicBeginIce3Multi(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicIce3MultiSelectDrawListBufferTask(void);
void __cdecl BattleFx_MagicIce3MultiSetupTask(void);
void __cdecl BattleFx_MagicIce3MultiTimelineTask(void);
void __cdecl BattleFx_MagicIce3MultiBuildDebrisFromMesh(struct BattleFxIce3MultiDebris *debris);
int __cdecl BattleFx_MagicIce3MultiBuildDebrisFromTexturedTriangles(struct BattleFxIce3MultiDebris *debris, int vertexBase, int topology);
int __cdecl BattleFx_MagicIce3MultiBuildDebrisFromTexturedQuads(struct BattleFxIce3MultiDebris *debris, int vertexBase, int topology);
int __cdecl BattleFx_MagicIce3MultiBuildDebrisFromShadedTriangles(struct BattleFxIce3MultiDebris *debris, int vertexBase, int triangles);
int __cdecl BattleFx_MagicIce3MultiBuildDebrisFromShadedQuads(struct BattleFxIce3MultiDebris *debris, int vertexBase, int quads);
void __cdecl BattleFx_MagicIce3MultiFadingSpriteLineTask(void);
void __cdecl BattleFx_MagicIce3MultiDrawSpriteLine(struct Vec3sPacked *origin, struct Vec3sPacked *target, char color);
void __cdecl BattleFx_MagicIce3MultiShatterModelTask(void);
int __cdecl BattleFx_MagicIce3MultiDrawMesh(struct BattleFxIce3MultiShatter *shatter, int shade);
int __cdecl BattleFx_MagicIce3MultiDrawMeshTexturedTriangles(struct BattleFxIce3MultiShatter *shatter, int vertexBase, int triangles, int tint);
int __cdecl BattleFx_MagicIce3MultiDrawMeshTexturedQuads(struct BattleFxIce3MultiShatter *shatter, int vertexBase, int quads, int tint);
int __cdecl BattleFx_MagicIce3MultiDrawMeshShadedTriangles(struct BattleFxIce3MultiShatter *shatter, int vertexBase, int triangles, int tint);
int __cdecl BattleFx_MagicIce3MultiDrawMeshShadedQuads(struct BattleFxIce3MultiShatter *shatter, int vertexBase, int quads, int tint);
int __cdecl BattleFx_MagicIce3MultiDrawDebris(struct BattleFxIce3MultiShatter *debris, int shade);
int __cdecl BattleFx_MagicIce3MultiDrawDebrisTexturedTriangles(struct BattleFxIce3MultiShatter *shatter, int vertexBase, int topology, int reserved);
int __cdecl BattleFx_MagicIce3MultiDrawDebrisTexturedQuads(struct BattleFxIce3MultiShatter *shatter, int vertexBase, int quadList, int reserved);
int __cdecl BattleFx_MagicIce3MultiDrawDebrisShadedTriangles(struct BattleFxIce3MultiShatter *shatter, int vertexBase, int triList, int reserved);
int __cdecl BattleFx_MagicIce3MultiDrawDebrisShadedQuads(struct BattleFxIce3MultiShatter *shatter, int vertexBase, int quadList, int reserved);
void __cdecl BattleFx_MagicIce3MultiShatterOffsetModelTask(void);
void __cdecl BattleFx_MagicIce3MultiExpandingGroundModelTask(void);
void __cdecl BattleFx_MagicIce3MultiSpawnConvergingModelsTask(void);
void __cdecl BattleFx_MagicIce3MultiConvergingModelTask(void);
void __cdecl BattleFx_MagicIce3MultiConvergingFadeModelTask(void);
void __cdecl BattleFx_MagicIce3MultiSpawnDirectedSpritesTask(void);
void __cdecl BattleFx_MagicIce3MultiDirectedSpriteTask(void);
void __cdecl BattleFx_MagicIce3MultiSpiralRibbonTask(void);

/* battle_fx_magic_debarrier.cpp */
int __cdecl BattleFx_MagicDeBarrierDrawShatterFragments(struct BattleFxDeBarrierShatterDesc *fragment, int vertexBase, int fragList, int threshold);
void __cdecl BattleFx_MagicDeBarrierBuildEulerRotationMatrix(struct BattleFxEulerAngles16 *angles, struct BattleRotationMatrix *matrix, void *scale);
int __cdecl BattleFx_MagicDeBarrierSineLookup(int angle);

/* battle_fx_magic_death.cpp */
void __cdecl BattleFx_MagicDeathUpdateDrawLinearStripTrail(int updateFlag);
void __cdecl BattleFx_MagicDeathProjectStripEdgePoints(struct BattleFxDeathStripTrail *trail, short *pointA, short *pointB);
void __cdecl BattleFx_MagicDeathTransformAttachPoint(int actorId, int attachIndex, short *inPos, short *outPos);
void __cdecl BattleFx_MagicDeathUpdateDrawCurvedStripTrail(int updateFlag);
void __cdecl BattleFx_MagicDeathSubdivideProjectTrailSegment(struct BattleFxDeathStripTrail *trail, short *endA, short *endB);

/* battle_fx_magic_despell.cpp */
int * __cdecl BattleFx_MagicDespellDrawPsxTexturedFragmentTriangles(struct BattleFxDespellFragment *fragment, unsigned char *vertexBase, int *packet, int shade);
int * __cdecl BattleFx_MagicDespellDrawPsxTexturedFragmentQuads(struct BattleFxDespellFragment *fragment, unsigned char *vertexBase, int *quadList, int fadeLevel);

/* battle_fx_magic_flare.cpp */
void __cdecl BattleFx_MagicBeginFlare(int targetMask, int actorIdx);

/* battle_fx_magic_freeze.cpp */
void __cdecl BattleFx_MagicSpawnFreezePerTarget(int actor);

/* battle_fx_magic_full_cure.cpp */
extern int g_BattleMagicFullCureMagicId;
extern unsigned char g_BattleMagicFullCureSpriteDef1[8];
extern unsigned char g_BattleMagicFullCureSpriteDef2[8];
void __cdecl BattleFx_MagicBeginFullCure(int targetMask, int actorIdx);
void __cdecl BattleFx_MagicFullCureClearParticleCallbacks(void);
void __cdecl BattleFx_MagicFullCureParticleCallbackTask(void);
void __cdecl BattleFx_MagicSpawnFullCurePerTarget(int actor);
void __cdecl BattleFx_MagicFullCureTimelineTask(void);
void __cdecl BattleFx_MagicFullCureSpawnGrowingModelsTask(void);
void __cdecl BattleFx_MagicFullCureGrowingModelTask(void);
void __cdecl BattleFx_MagicFullCureSpawnRisingSpritesTask(void);
struct BattleFxMagicFullCureSparkle *__cdecl BattleFx_MagicFullCureClaimParticleRecord(void (__cdecl *pIndex)(struct BattleFxMagicFullCureSparkle *));
void __cdecl BattleFx_MagicFullCureRisingSpriteParticle(struct BattleFxMagicFullCureSparkle *particleRecord);
void __cdecl BattleFx_MagicFullCureSpawnFadingRisingSpritesTask(void);
void __cdecl BattleFx_MagicFullCureFadingRisingSpriteTask(struct BattleFxMagicFullCureSparkle *particleRecord);
void __cdecl BattleFx_MagicFullCureShrinkingModelTask(void);
void __cdecl BattleFx_MagicFullCureNoOp(void);

/* battle_fx_magic_haste.cpp */
void __cdecl BattleFx_MagicEnterHaste(int targetMask, int actorIdx);

/* battle_fx_magic_hitmark.cpp */
void __cdecl BattleFx_MagicHitmarkSpawnImpactParticleBurst(struct Vec3sPacked *origin, void *scale);
int __cdecl BattleFx_MagicHitmarkIntersectRaySphere(short *dir, short *pointA, short *pointB);

/* battle_fx_magic_quake3.cpp */
void __cdecl BattleFx_MagicBeginQuake3(int targetMask, int actorIdx);
int __cdecl BattleFx_MagicQuake3DrawShardTriangles(struct BattleFxQuake3Shard *shard, void *vertexBase, int meshData, int threshold);
void __cdecl BattleFx_MagicQuake3BuildScaledRotationMatrix(struct BattleFxEulerAngles16 *angles, struct BattleRotationMatrix *matrix, struct BattleFxFixedScaleVec *scaleVec);
int __cdecl BattleFx_MagicQuake3LookupSine(int angle);
int __cdecl BattleFx_MagicQuake3DrawShardQuads(struct BattleFxQuake3Shard *shard, void *vertexBase, int meshData, int threshold);

/* battle_fx_magic_quake2.cpp */
int __cdecl BattleFx_MagicQuake2DrawShardTriangles(struct BattleFxQuake2Shard *shard, void *vertexBase, int triList, int threshold);
int __cdecl BattleFx_MagicQuake2BuildRotationMatrix(struct BattleFxEulerAngles16 *angles, struct BattleRotationMatrix *matrix);
int __cdecl BattleFx_MagicQuake2LookupSine(int angle);
int __cdecl BattleFx_MagicQuake2DrawShardQuads(struct BattleFxQuake2Shard *shard, void *vertexBase, int quadList, int threshold);

/* battle_fx_magic_shield.cpp */
void __cdecl BattleFx_MagicBeginShield(int targetMask, int actorIdx);

/* battle_fx_magic_slow.cpp */
void __cdecl BattleFx_MagicBeginSlow(int targetMask, int actorIdx);

/* battle_fx_magic_stop.cpp */
void __cdecl BattleFx_MagicSpawnStopPerTarget(int actor);

/* battle_fx_phoenix_summon.cpp */
void __cdecl BattleFx_SummonPhoenixQueueStageOffscreenRequest(struct Vec3sPacked *pos, struct BattleSummonPhoenixAngleData1 *shortVec);

/* battle_fx_shiva_summon.cpp */
int __cdecl BattleFx_SummonShivaSubmitPerTriangleModel(struct BattleFxShivaTriangleModelBlock *resourceBlock, int index, int param, int tint, int matrixPtr);

/* battle_fx_special_blaster.cpp */
void __cdecl BattleFx_SpecialSpawnBlasterPerTarget(int actor, int attackerId);

/* battle_fx_special_diamond_card.cpp */
void __cdecl BattleFx_SpecialBeginDiamondCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialDiamondCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialDiamondCardTimelineTask(void);
void __cdecl BattleFx_SpecialDiamondCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialDiamondCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialDiamondCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialDiamondCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialDiamondCardDelayedImpactTask(void);
void __cdecl BattleFx_SpecialDiamondCardSpawnImpactTimeline(int actorIndex, int casterIdx);
void __cdecl BattleFx_SpecialDiamondCardImpactTimelineTask(void);
void __cdecl BattleFx_SpecialDiamondCardDrawFourSpritesTask(void);
void __cdecl BattleFx_SpecialDiamondCardDrawTwoSpritesTask(void);
void __cdecl BattleFx_SpecialDiamondCardBouncingModelTask(void);

/* battle_fx_special_sun_card.cpp */
void __cdecl BattleFx_SpecialBeginSunCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSunCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialSunCardTimelineTask(void);
void __cdecl BattleFx_SpecialSunCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialSunCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialSunCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialSunCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialSunCardDelayedImpactTask(void);
void __cdecl BattleFx_SpecialSunCardSpawnTargetHitModelWithSound(int actorIndex, int extraArg);
void __cdecl BattleFx_SpecialSunCardSpawnTargetHitModel(int actorIndex, int extraArg);
void __cdecl BattleFx_SpecialSunCardTargetHitModelTask(void);

/* battle_fx_special_wave_cannon.cpp */
void __cdecl BattleFx_SpecialBeginWaveCannon(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialWaveCannonTimelineTask(void);
void __cdecl BattleFx_SpecialWaveCannonScreenFlashTask(void);
void __cdecl BattleFx_SpecialWaveCannonGrowingModelTask(void);
void __cdecl BattleFx_SpecialWaveCannonDriftingAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialWaveCannonPulsingSpriteTask(void);
void __cdecl BattleFx_SpecialWaveCannonStretchingModelsTask(void);
void __cdecl BattleFx_SpecialWaveCannonProjectileTask(void);
void __cdecl BattleFx_SpecialWaveCannonBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialWaveCannonVariantSpinModelTask(void);
void __cdecl BattleFx_SpecialWaveCannonTrailModelTask(void);

/* battle_fx_special_snowstorm.cpp */
void __cdecl BattleFx_SpecialEnterSnowstorm(int targetMask, int actorIdx);

/* battle_fx_special_joker_card.cpp */
void __cdecl BattleFx_SpecialBeginJokerCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialJokerCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialJokerCardTimelineTask(void);
void __cdecl BattleFx_SpecialJokerCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialJokerCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialJokerCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialJokerCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialJokerCardDelayedImpactTask(void);
void __cdecl BattleFx_SpecialJokerCardSpawnBeginImpactTask(int drawListHandle, int actorIdx);
void __cdecl BattleFx_SpecialJokerCardBeginImpactTask(void);
void __cdecl BattleFx_SpecialJokerCardImpactTimelineTask(void);

/* battle_fx_special_world_card.cpp */
void __cdecl BattleFx_SpecialBeginWorldCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialWorldCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialWorldCardTimelineTask(void);
void __cdecl BattleFx_SpecialWorldCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialWorldCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialWorldCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialWorldCardCaptureTargetAnchor(void);
void __cdecl BattleFx_SpecialWorldCardDelayedImpactTask(void);
void __cdecl BattleFx_SpecialWorldCardSpawnModelsWithSound(int actorIndex, int extraArg);
void __cdecl BattleFx_SpecialWorldCardSpawnModels(int actorIndex, int extraArg);
void __cdecl BattleFx_SpecialWorldCardPrimaryModelTask(void);
void __cdecl BattleFx_SpecialWorldCardSecondaryModelTask(void);
void __cdecl BattleFx_SpecialWorldCardLayeredModelsTask(void);

/* battle_fx_special_pollen.cpp */
extern struct BattleSpecPollenSpriteReq1 g_BattleSpecPollenSpriteReq1;

/* battle_fx_special_quasar.cpp */
extern struct BattleSpecialQuasarTable g_BattleSpecialQuasarTable;

/* battle_fx_special_avalanche.cpp */
int __cdecl BattleFx_SpecialAvalancheDrawPerFaceOffsetModel(struct BattleFxFreezeBreathDrawRecord *drawRecord, int flags, int value, struct BattleFxFreezeBreathDrawPrim *extraCount, struct Vec3sPacked *offsetTable, void *matrix, struct BattleFxOffsetTriMeshDesc *poseMatrix);

/* battle_fx_special_bad_mouth.cpp */
int __cdecl BattleFx_SpecialBadMouthDrawFragmentedModel(struct BattleFxFragmentedModelHeader *header, int unusedArg2, int unusedArg3, struct BattleFxBadMouthDrawPrim *unusedArg4, struct Vec3sPacked *offsetTable, void *baseMatrix, struct BattleFxOffsetTriMeshDesc *localMatrix);

/* battle_fx_special_toxic_smog.cpp */
void __cdecl BattleFx_SpecialToxicSmogDrawWavyRibbon(int amplitude, short phase, int angleStep, int color);

/* battle_fx_special_energy_orb.cpp */
void __cdecl BattleFx_SpecialBeginEnergyOrb(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEnergyOrbTimelineTask(void);
void __cdecl BattleFx_SpecialEnergyOrbScreenFlashTask(void);
void __cdecl BattleFx_SpecialEnergyOrbGrowingModelTask(void);
void __cdecl BattleFx_SpecialEnergyOrbDriftingAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyOrbPulsingSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyOrbStretchingModelsTask(void);
void __cdecl BattleFx_SpecialEnergyOrbProjectileTask(void);
void __cdecl BattleFx_SpecialEnergyOrbBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialEnergyOrbFallingFadeModelTask(void);
void __cdecl BattleFx_SpecialEnergyOrbTrailModelTask(void);

/* battle_fx_special_freeze_breath_effect.cpp */
int __cdecl BattleFx_SpecialFreezeBreathDrawPerFaceOffsetMesh(struct BattleFxFreezeBreathDrawRecord *drawRecord, int unused, int unused3, struct BattleFxFreezeBreathDrawPrim *unused4, struct Vec3sPacked *offsetCursor, void *matrix, struct BattleFxShardTransformBatch *position);

/* battle_fx_special_searchlight.cpp */
void __cdecl BattleFx_SpecialSpawnSearchlightPerTarget(int actor, int casterIndex);

/* battle_fx_special_petrify_effect.cpp */
int __cdecl BattleFx_SpecialPetrifyDrawPerFaceOffsetModel(struct BattleFxFreezeBreathDrawRecord *drawRecord, int reserved2, int reserved3, struct BattleFxPetrifyDrawPrim *reserved4, struct Vec3sPacked *faceOffsets, void *matrix, struct BattleFxOffsetTriMeshDesc *transVec);

/* battle_fx_special_shield.cpp */
extern int g_BattleSpecialShieldResourceDefs;
extern short g_BattleSpecialShieldCameraStep;
extern short g_BattleSpecialShieldVariant;
extern struct GfxTableCursor *g_BattleSpecialShieldLoadedResTable;
extern short g_BattleSpecialShieldMode;
void __cdecl BattleFx_SpecialRunShield(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpawnShieldPerTarget(int actor, int actorIdx);
void __cdecl BattleFx_SpecialShieldTimelineTask(void);
void __cdecl BattleFx_SpecialShieldFlashShakeTask(void);
void __cdecl BattleFx_SpecialShieldProjectileTask(void);
void __cdecl BattleFx_SpecialShieldTrailModelTask(void);

/* battle_fx_special_smog.cpp */
void __cdecl BattleFx_SpecialSmogDrawPsxTexturedQuadStrip(short amplitude, short phase, short angleStep, int shade);

/* battle_fx_special_evil_poison.cpp */
void __cdecl BattleFx_SpecialSpawnEvilPoisonPerTarget(int actor, int attackerId);

/* battle_fx_special_scorch.cpp */
extern struct BattleSpecialScorchBasisMatrix g_BattleSpecialScorchBasisMatrix;
void __cdecl BattleFx_SpecialBeginScorch(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialScorchTimelineTask(void);
void __cdecl BattleFx_SpecialScorchScreenFlashTask(void);
void __cdecl BattleFx_SpecialScorchGrowingModelTask(void);
void __cdecl BattleFx_SpecialScorchDriftingAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialScorchPulsingSpriteTask(void);
void __cdecl BattleFx_SpecialScorchStretchingModelsTask(void);
void __cdecl BattleFx_SpecialScorchProjectileTask(void);
void __cdecl BattleFx_SpecialScorchBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialScorchFallingFadeModelTask(void);
void __cdecl BattleFx_SpecialScorchTrailModelTask(void);

/* battle_fx_special_time_damage_warp.cpp */
void __cdecl BattleFx_SpecialTimeDamageAcquireOffscreenRect(int left, int right);
void __cdecl BattleFx_SpecialTimeDamageBuildWarpGridOffsets(int depthScale, int phase, int amplitude);

/* battle_fx_special_big_wave.cpp */
short __cdecl BattleFx_SpecialBigWaveGetWaveVertexHeight(short waveIndex, short rowIndex);

/* battle_fx_special_super_orb.cpp */
extern int g_BattleSpecialSuperOrbRotationVector;
void __cdecl BattleFx_SpecialBeginSuperOrb(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSuperOrbTimelineTask(void);
void __cdecl BattleFx_SpecialSuperOrbScreenFlashTask(void);
void __cdecl BattleFx_SpecialSuperOrbExpandingModelTask(void);
void __cdecl BattleFx_SpecialSuperOrbGrowingModelTask(void);
void __cdecl BattleFx_SpecialSuperOrbDriftingAnimatedSpriteTask(void);
void __cdecl BattleFx_SpecialSuperOrbCounterRotatingModelsTask(void);
void __cdecl BattleFx_SpecialSuperOrbStretchingModelsTask(void);
void __cdecl BattleFx_SpecialSuperOrbProjectileTask(void);
void __cdecl BattleFx_SpecialSuperOrbBouncingSpriteTask(void);
void __cdecl BattleFx_SpecialSuperOrbFallingFadeModelTask(void);
void __cdecl BattleFx_SpecialSuperOrbTrailModelTask(void);
void __cdecl BattleFx_SpecialSuperOrbDecelSpriteTask(void);

/* battle_fx_special_whirlpool.cpp */
extern struct BattleFxSpecialWhirlpoolSpiralTrail g_BattleSpecWhirlpoolParticleRecords[100];

/* battle_fx_summon_bahamut.cpp */
extern unsigned char g_BattleSummonBahamutBeamVector[16];
extern unsigned char g_BattleSummonBahamutBaseDistance1[8];
void __cdecl BattleFx_SummonBahamutBlendPolygonSetVertices(int index, struct BattleFxBahamutBlendVertex *start, struct BattleFxHellBomberEndVertex *out);
void __cdecl BattleFx_SummonBeginBahamut(int targetMask, int actorIdx);
void __cdecl BattleFx_BahamutSpawnCameraScript(void *dataA, void *dataB, int mode);
void __cdecl BattleFx_BahamutRunCameraScript(void);
short __cdecl BattleFx_BahamutReadCameraScriptOperand(short **cursor);
int __cdecl BattleFx_BahamutApplyCameraEasing(int phase, int packed);
void __cdecl BattleFx_SummonBahamutTimelineTask(void);
void __cdecl BattleFx_SummonBahamutSpawnShakeEnvelope(int amplitude, int decay, int duration, int frequency);
void __cdecl BattleFx_SummonBahamutShakeEnvelopeTask(void);
void __cdecl BattleFx_SummonBahamutSpawnFadingBillboardSpritesTask(void);
void __cdecl BattleFx_SummonBahamutFadingBillboardSpriteTask(void);
void __cdecl BattleFx_SummonBahamutCounterRotatingModelPairTask(void);
void __cdecl BattleFx_SummonBahamutBillboardGrowingModelTask(void);
int __cdecl BattleFx_SummonBahamutInstallBillboardMatrix(struct BattleFxBillboardVector *vector, short value, int distance);
void __cdecl BattleFx_SummonBahamutExpandingModelTask(void);
void __cdecl BattleFx_SummonBahamutBuildEulerRotationMatrix(struct BattleFxBahamutEulerRotation *rotation, void *matrix);
void __cdecl BattleFx_SummonBahamutSpinningExpandingModelTask(void);
void __cdecl BattleFx_SummonBahamutSpawnAnimatedSpritesTask(void);
void __cdecl BattleFx_SummonBahamutAnimatedSpriteTask(void);
void __cdecl BattleFx_SummonBahamutBlackGrowingModelTask(void);
void __cdecl BattleFx_SummonBahamutSpawnStreakLinesTask(void);
void __cdecl BattleFx_SummonBahamutStreakLineTask(void);
void __cdecl BattleFx_SummonBahamutRotatingModelPairTask(void);
void __cdecl BattleFx_SummonBahamutExtendingBeamTask(void);
void __cdecl BattleFx_SummonBahamutBeamEasedScaleModelTask(void);
void __cdecl BattleFx_SummonBahamutActorDescentTask(void);

/* battle_fx_summon_hades.cpp */
extern int g_BattleSummonHadesResourceDef;
extern unsigned char g_BattleSummonHadesPolyCount[2440];
extern int g_BattleSummonHadesVertexCount;
extern struct GfxTableCursor *g_pBattleSummonHadesTableCursor;
extern int g_BattleSummonHadesFacingMatrix;
extern struct PsxMatrix g_BattleSummonHadesRotationMatrix;
void __cdecl BattleFx_SummonBeginHades(int targetMask, int actorIdx);
void __cdecl BattleFx_SummonHadesSpawnTimeline(void);
void __cdecl BattleFx_HadesSpawnCameraScript(int arg1, int arg2, int flags);
void __cdecl BattleFx_HadesRunCameraScript(void);
short __cdecl BattleFx_HadesReadCameraScriptOperand(void *cursor);
int __cdecl BattleFx_HadesApplyCameraEasing(int value, int easingSpec);
void __cdecl BattleFx_SummonHadesTimelineTask(void);
void __cdecl BattleFx_SummonHadesSharedColorFadeInTask(void);
void __cdecl BattleFx_SummonHadesSharedColorFadeOutTask(void);
void __cdecl BattleFx_SummonHadesFadingSpriteModelPairEmitterTask(void);
void __cdecl BattleFx_SummonHadesFadingSpriteModelPairTask(void);
void __cdecl BattleFx_SummonHadesRibbonSpawnerTask(void);
void __cdecl BattleFx_SummonHadesRibbonTask(void);
void __cdecl BattleFx_SummonHadesDrawRibbonSegmentFourColorQuad(struct BattleFxHadesRibbon *ribbon, int step, int steps);
void __cdecl BattleFx_SummonHadesProjectRibbonVertices(struct BattleFxHadesRibbon *ribbon, void *matrix, struct SVECTOR *points, struct SVECTOR *index);
void __cdecl BattleFx_SummonHadesPushPositionHistory(struct BattleFxHadesPositionHistory *history, int position);
void __cdecl BattleFx_SummonHadesRadiusClampedMeshSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesGrowingRadialSpriteEmitterTask(void);
void __cdecl BattleFx_SummonHadesRadiusClampedMeshTask(void);
int __cdecl BattleFx_SummonHadesDrawPsxMeshRadiusClamped(struct BattleFxHadesRingState *object, int radius);
int __cdecl BattleFx_SummonHadesDrawPsxTexturedTrianglesRadiusClamped(struct BattleFxHadesRingState *scratch, char *vertexBase, int stream, int radius);
int __cdecl BattleFx_SummonHadesDrawPsxTexturedQuads(struct BattleFxHadesRingState *state, char *vertexBase, int stream, int radius);
void __cdecl BattleFx_SummonHadesHeightClippedMeshSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesShrinkingRadialSpriteEmitterTask(void);
void __cdecl BattleFx_SummonHadesHeightClippedMeshTask(void);
int __cdecl BattleFx_SummonHadesDrawPsxMeshHeightClipped(struct BattleFxHadesRingState *mesh, int height);
int __cdecl BattleFx_SummonHadesDrawPsxTexturedTrianglesHeightClipped(struct BattleFxHadesRingState *mesh, int vertexBase, struct BattleFxHadesPsxDrawCursor *stream, int clipHeight);
int __cdecl BattleFx_SummonHadesDrawPsxTexturedQuadsHeightClipped(struct BattleFxHadesRingState *state, char *vertexBase, void *descriptors, int threshold);
void __cdecl BattleFx_SummonHadesSpriteModelPairSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesSpriteModelPairTask(void);
void __cdecl BattleFx_SummonHadesOscillatingModelsTask(void);
void __cdecl BattleFx_SummonHadesTwoColorRibbonSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesTwoColorRibbonTask(void);
void __cdecl BattleFx_SummonHadesDrawRibbonSegmentTwoColorQuad(struct BattleFxHadesRibbon *ribbon, int amount, int divisor);
void __cdecl BattleFx_SummonHadesTubeSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesTubeTask(void);
void __cdecl BattleFx_SummonHadesDrawTubeSectionQuads(struct BattleFxHadesTube *tube, int amount, int divisor, int sectionCount);
void __cdecl BattleFx_SummonHadesProjectCirclePoints(struct BattleFxHadesTube *tube, int count);
void __cdecl BattleFx_SummonHadesLayoutCirclePoints(struct BattleFxHadesTube *tube, struct Vec3sPacked *center, int count, int radius);
void __cdecl BattleFx_SummonHadesDelayedRingSpawnTask(void);
void __cdecl BattleFx_SummonHadesRisingRingSpriteEmitterSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesRisingRingSpriteEmitterTask(void);
void __cdecl BattleFx_SummonHadesStretchingModelSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesStretchingModelTask(void);
void __cdecl BattleFx_SummonHadesColorCyclingSpriteEmitterSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesColorCyclingSpriteEmitterTask(void);
void __cdecl BattleFx_SummonHadesSpriteParticlePoolSpawnTimeline(void);
void __cdecl BattleFx_SummonHadesSpriteParticlePoolTask(void);
void __cdecl BattleFx_SummonHadesUpdateDrawSpriteParticlePool(struct BattleFxHadesSpriteParticle *particles, struct BattleFxHadesParticlePool *work);
void __cdecl BattleFx_SummonHadesDelayedPerTargetSpawnTask(void);
void __cdecl BattleFx_SummonSpawnHadesPerTarget(int actor);
void __cdecl BattleFx_SummonHadesPaletteCyclingModelEmitterTask(void);
void __cdecl BattleFx_SummonHadesPaletteCyclingModelTask(void);
int __cdecl BattleFx_SummonHadesDrawPsxMeshGouraud(struct BattleFxHadesRingState *object, int shade);
int __cdecl BattleFx_SummonHadesDrawPsxGouraudQuads(struct BattleFxHadesRingState *state, int vertexBase, int quadList, int shade);
void __cdecl BattleFx_SummonHadesTargetRecordSpawnTimeline(void);

/* battle_fx_summon_leviathan.cpp */
void __cdecl BattleFx_SummonLeviathanHalfDeltaVertexPart(int vertexIndex, float *center);
void __cdecl BattleFx_SummonBeginLeviathan(int targetMask, int actorIdx);

/* battle_fx_summon_neo_bahamut.cpp */
extern struct BattleFxShortOffsetVec *g_BattleSummonNeoBahamutRock1SpawnTaskSlot;
void __cdecl BattleFx_NeoBahamutBeginEffect(int targetMask, int actorIdx);
void __cdecl BattleFx_NeoBahamutSpawnCameraScript(void *cameraScript, int matrix, int actorIndex);
void __cdecl BattleFx_NeoBahamutRunCameraScript(void);
short __cdecl BattleFx_NeoBahamutReadCameraScriptOperand(void *cursorPtr);
int __cdecl BattleFx_NeoBahamutApplyCameraEasing(int value, int control);
void __cdecl BattleFx_NeoBahamutRunLightRig(void);
void __cdecl BattleFx_SummonNeoBahamutTimelineTask(void);
void __cdecl BattleFx_SummonBeginNeoBahamutJitterEnvelope(int value, int second, int third, int fourth);
void __cdecl BattleFx_SummonNeoBahamutJitterEnvelopeTask(void);
void __cdecl BattleFx_SummonNeoBahamutPrepareStageOffscreenRequests(void);
void __cdecl BattleFx_SummonNeoBahamutQueueStageOffscreenRequests(void *matrix, struct BattleFxScreenRect *rect);
void __cdecl BattleFx_SummonNeoBahamutGroundModelTask(void);
int __cdecl BattleFx_SummonNeoBahamutDrawPsxMesh(struct BattleFxProceduralMesh *mesh, int orderingTable, int depthShift, int packetCursor);
int __cdecl BattleFx_SummonNeoBahamutDrawPsxMeshPerFaceMatrix(struct BattleFxPsxMeshResource *resource, int arg2, int arg3, int arg4, int matrix);
void __cdecl BattleFx_SummonNeoBahamutInitFragmentTable(void);
void __cdecl BattleFx_SummonNeoBahamutUpdateFragmentMatrices(int matrix);
void __cdecl BattleFx_SummonNeoBahamutSpawnThunderSpritesTask(void);
void __cdecl BattleFx_SummonNeoBahamutThunderSpriteTask(void);
int __cdecl BattleFx_SummonNeoBahamutInstallBillboardMatrix(void *point, short scale, int offset);
void __cdecl BattleFx_SummonNeoBahamutMakeRandomLoopPoint(struct Vec3sPacked *outPoint);
void __cdecl BattleFx_SummonNeoBahamutSpawnDustSpritesTask(void);
void __cdecl BattleFx_SummonNeoBahamutDustSpriteTask(void);
void __cdecl BattleFx_SummonNeoBahamutSpawnRock1ModelsTask(void);
void __cdecl BattleFx_SummonNeoBahamutRock1ModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutCloud1ModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutCloud2ModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutSkyMaskModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutSpawnLineLight3ModelsTask(void);
void __cdecl BattleFx_SummonNeoBahamutLineLight3ModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutCloudSeaModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutBackSkyPrimitiveTask(void);
void __cdecl BattleFx_SummonNeoBahamutSpawnBallSpritesTask(void);
void __cdecl BattleFx_SummonNeoBahamutBallSpriteTask(void);
void __cdecl BattleFx_SummonNeoBahamutCircleModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutSpawnLineLight1Task(void);
void __cdecl BattleFx_SummonNeoBahamutLineLight1PrimitiveTask(void);
int __cdecl BattleFx_SummonNeoBahamutPrepareLineLightPrimitive(void);
void __cdecl BattleFx_SummonNeoBahamutBreathModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutBreathChildModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutSpawnRock2ModelsTask(void);
void __cdecl BattleFx_SummonNeoBahamutRock2ModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutFlash2ModelTask(void);
void __cdecl BattleFx_SummonNeoBahamutSpawnRock3ModelsTask(void);
void __cdecl BattleFx_SummonNeoBahamutRock3ModelTask(void);
int __cdecl BattleFx_SummonNeoBahamutHeightToXOffset(int height);
void __cdecl BattleFx_SummonSpawnNeoBahamutPerTarget(int actor);
void __cdecl BattleFx_SummonNeoBahamutActorRiseAndFallTask(void);
void __cdecl BattleFx_SummonNeoBahamutSpawnLineLight2Task(void);
void __cdecl BattleFx_SummonNeoBahamutLineLight2PrimitiveTask(void);
void __cdecl BattleFx_SummonNeoBahamutToggleActorMaskFlags(void);

/* battle_fx_summon_odin_gungnir.cpp */
extern int g_BattleSummonOdinGVertexDeltasParam1;
extern int g_BattleSummonOdinGVertexDeltasParam2;
extern int g_BattleSummonOdinGResourceDef;
extern unsigned char g_BattleSummonOdinGCameraScript1[336];
extern int g_BattleSummonOdinGAnimDataC88;
extern int g_BattleSummonOdinGAnimDataC90;
extern int g_BattleSummonOdinGAnimDataC98;
extern int g_BattleSummonOdinGVarCB0;
extern int g_BattleSummonOdinGVarCB8;
extern int g_BattleSummonOdinGAnimDataD38;
extern struct BattleSummonOdinGMatrixB168 g_BattleSummonOdinGMatrixB168;
void __cdecl BattleFx_SummonBeginOdinGungnir(int targetMask, int casterIdx);
void __cdecl BattleFx_SummonOdinGungnirBlendPolygonSetVerticesResource0(int vertexIndex, int arg2, struct BattleFxHellBomberEndVertex *outVec);
void __cdecl BattleFx_SummonOdinGungnirBlendPolygonSetVerticesResource1(int vertexIndex, int arg2, struct BattleFxHellBomberEndVertex *outVec);
void __cdecl BattleFx_SummonOdinGungnirComputeVertexDeltas(void *sourceBlock, struct Vec3sPacked *offsetTable, struct Vec3sPacked *dst);
void __cdecl BattleFx_OdinGungnirSpawnCameraScript(void *cameraScript, int matrix, int actorIndex);
void __cdecl BattleFx_OdinGungnirRunCameraScript(void);
short __cdecl BattleFx_OdinGungnirReadCameraScriptOperand(int *cursor);
int __cdecl BattleFx_OdinGungnirApplyCameraEasing(int phase, int packed);
void __cdecl BattleFx_SummonOdinGungnirTimelineTask(void);
void __cdecl BattleFx_SummonOdinGungnirPoolParticleEmitterTask(void);
void __cdecl BattleFx_SummonOdinGungnirUpdateDrawPoolParticle(struct BattleFxOdinGungnirParticle *particle);
void __cdecl BattleFx_SummonOdinGungnirSpawnShakeEnvelope(int amplitude, int decay, int frequency, int duration);
void __cdecl BattleFx_SummonOdinGungnirShakeEnvelopeTask(void);
void __cdecl BattleFx_SummonOdinGungnirSpawnPooledFadingSpritesTask(void);
void __cdecl BattleFx_SummonOdinGungnirClearPooledSpriteCallbacks(void);
int __cdecl BattleFx_SummonOdinGungnirClaimPooledSpriteRecord(int owner);
void __cdecl BattleFx_SummonOdinGungnirPooledSpriteDispatchTask(void);
void __cdecl BattleFx_SummonOdinGungnirPooledFadingSpriteTask(struct BattleFxOdinSpriteTask *task);
void __cdecl BattleFx_SummonOdinGungnirSpawnPooledFallingSpritesTask(void);
void __cdecl BattleFx_SummonOdinGungnirPooledFallingSpriteTask(struct BattleOdinFallingSprite *record);
void __cdecl BattleFx_SummonOdinGungnirSpawnPooledDampedSpritesTask(void);
void __cdecl BattleFx_SummonOdinGungnirPooledDampedSpriteTask(struct BattleFxOdinDebrisSprite *record);
void __cdecl BattleFx_SummonOdinGungnirSpinningModelPairTask(void);
void __cdecl BattleFx_SummonOdinGungnirGrowingModelTask(void);
void __cdecl BattleFx_SummonOdinGungnirPulsingModelTask(void);
void __cdecl BattleFx_SummonOdinGungnirBillboardGrowingModelTask(void);
void __cdecl BattleFx_SummonOdinGungnirRampHoldScaleModelTask(void);
void __cdecl BattleFx_SummonOdinGungnirFadingGouraudQuadTask(void);
void __cdecl BattleFx_SummonOdinGungnirFadingGouraudTriangleTask(void);
void __cdecl BattleFx_SummonOdinGungnirAnimatedBillboardSpriteTask(void);
int __cdecl BattleFx_SummonOdinGungnirInstallBillboardMatrix(void *vector, short angle, int push);
void __cdecl BattleFx_SummonOdinGungnirFlashOverlayTask(void);
void __cdecl BattleFx_SummonOdinGungnirScreenColorRampTask(void);
void __cdecl BattleFx_Odin1AltSpawnCameraScript(int arg1, int arg2, int arg3);
void __cdecl BattleFx_Odin1AltRunCameraScript(void);
void __cdecl BattleFx_SummonOdinGungnirAttachedModelPairTrailTask(void);
void __cdecl BattleFx_SummonOdinGungnirActorFadeMoveTask(void);

/* battle_fx_summon_odin_zantetsuken.cpp */
extern struct SoundGroup *g_BattleSummonOdinZSoundGroup;
extern unsigned char g_BattleSummonOdinZInitialY[8];
extern int g_BattleSummonOdinZLimitY;
void __cdecl BattleFx_SummonBeginOdinZantetsuken(int targetMask, int actorIdx);
void __cdecl BattleFx_OdinZantetsuSpawnCameraScript(short *scriptTable, struct PsxMatrix *matrix, int actorIndex);
void __cdecl BattleFx_OdinZantetsuRunCameraScript(void);
short __cdecl BattleFx_OdinZantetsuReadCameraScriptOperand(short **cursor);
void __cdecl BattleFx_SummonOdinZantetsukenActorMotionTask(void);
int __cdecl BattleFx_SummonOdinZantetsukenDrawPsxGouraudMesh(struct BattleFxPsxMeshHeader *mesh, int orderingTable, int depthShift, int packetCursor);
void __cdecl BattleFx_SummonOdinZantetsukenSpawnBoneSprites(int count);
void __cdecl BattleFx_SummonOdinZantetsukenAnimatedSpriteTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenSpawnAfterimageTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenAfterimageTask(void);
int __cdecl BattleFx_SummonOdinZantetsukenDrawPsxMesh(struct BattleFxZantetsukenMeshBlock *meshBlock, int orderingTable, int depthShift, int packetCursor);
void __cdecl BattleFx_SummonOdinZantetsukenSpawnTrailSegmentsTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenOffsetPointAlongLine(struct Vec3sPacked *startPoint, struct Vec3sPacked *endPoint, int distance, struct Vec3sPacked *outPoint);
void __cdecl BattleFx_SummonOdinZantetsukenTrailSegmentQuadTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenTimelineTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenFadeInTripleSpriteTask(void);
int __cdecl BattleFx_SummonOdinZantetsukenInstallBillboardMatrix(void *point, int value, int distance);
void __cdecl BattleFx_SummonOdinZantetsukenStaticSpriteTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenStaticModelTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenScrollingTripleSpriteTask(void);
struct PsxMatrix *__cdecl BattleFx_SummonOdinZantetsukenMakeViewSpacePointPushed(void *point, int distance, struct PsxMatrix *target);
void __cdecl BattleFx_SummonOdinZantetsukenTargetSequenceTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenSplitActorTask(void);
void __cdecl BattleFx_SummonOdinZantetsukenQueueActorOffscreenRequest(int actorIndex, struct BattleFxOdinZantetsukenActorPos *rect, struct BattleFxActorOffscreenRequest *capture);
void __cdecl BattleFx_SummonOdinZantetsukenInitSplitActorQuads(struct BattleFxActorOffscreenRequest *center, int scale);
void __cdecl BattleFx_SummonOdinZantetsukenMoveSplitQuadHalf(int extent, int scale, int row, int column);
void __cdecl BattleFx_SummonOdinZantetsukenToggleMaskedActorBit1(void);
void __cdecl BattleFx_SummonOdinZantetsukenScreenFlashPulseTask(void);
extern short g_BattleSummonOdinZBillboardMatrix[2];
extern int g_BattleSummonOdinZScrollOffset;
extern short g_BattleSummonOdinZScrollMatrix[2];

/* battle_fx_summon_ramuh.cpp */
void __cdecl BattleFx_SummonBeginRamuh(int targetMask, int actorIdx);
int __cdecl BattleFx_SummonRamuhDrawPsxSpriteRecordsShift5(struct BattleFxRamuhPsxSpriteHeader *header, int orderingTable, int depthShift, int packetCursor);

/* battle_fx_summon_titan.cpp */
void __cdecl BattleFx_SummonTitanBlendVertexPositions(short *posA, short *posB, int weightA, int weightB, short *outPos);
int __cdecl BattleFx_SummonEnterTitan(int targetMask, int actorIdx);
void __cdecl BattleFx_SummonBeginTitan(int targetMask, int actorIdx);
int __cdecl BattleFx_SummonTitanDrawPsxTexturedFragmentTriangles(struct BattleFxSummonTitanMeshInstance *meshInst, int unusedArg2, int unusedArg3, int unusedArg4, struct PsxMatrix *transformMatrixPtr, struct BattleSummonTitanFragment *fragment);
int __cdecl BattleFx_SummonTitanDrawPsxTexturedFragmentMesh(struct BattleFxTitanMeshInstance *meshInst, int unusedArg2, int unusedArg3, int unusedArg4, struct PsxMatrix *transformMatrixPtr, struct BattleTitanFragmentVertex *fragment);

/* battle_fx_throw_aeris.cpp */
void __cdecl Battle_ThrowFxSpawnWeapons62to72(int targetMask, int actorIdx);
void __cdecl BattleFx_ThrowAerisWeaponsDispatchKindTask(void);
void __cdecl BattleFx_ThrowAerisWeaponsStraightTimelineTask(void);
void __cdecl BattleFx_ThrowAerisWeaponsStraightProjectileTask(void);
void __cdecl BattleFx_ThrowAerisWeaponsArcTimelineTask(void);
void __cdecl BattleFx_ThrowAerisWeaponsArcProjectileTask(void);

/* battle_fx_throw_barret.cpp */
void __cdecl Battle_ThrowFxSpawnWeapons32to47(int targetMask, int actorIndex);
void __cdecl BattleFx_ThrowBarretWeaponsDispatchKindTask(void);
void __cdecl BattleFx_ThrowBarretWeaponsStraightTimelineTask(void);
void __cdecl BattleFx_ThrowBarretWeaponsStraightProjectileTask(void);
void __cdecl BattleFx_ThrowBarretWeaponsTumblingTimelineTask(void);
void __cdecl BattleFx_ThrowBarretWeaponsTumblingProjectileTask(void);

/* battle_fx_throw_cait_sith.cpp */
void __cdecl Battle_ThrowFxSpawnWeapons101to113(int targetMask, int actorIdx);
void __cdecl BattleFx_ThrowCaitSithWeaponsSpawnTimeline(void);
void __cdecl BattleFx_ThrowCaitSithWeaponsTimelineTask(void);
void __cdecl BattleFx_ThrowCaitSithWeaponsThrownModelTask(void);

/* battle_fx_throw_cid.cpp */
extern int g_BattleThrowCidVectorOffset[3];
extern int g_BattleThrowCidPrevPosXY;
extern int g_BattleThrowCidPrevPosZ;
void __cdecl Battle_ThrowFxSpawnWeapons73to86(int targetMask, int actorIdx);
void __cdecl BattleFx_ThrowCidWeaponsDispatchKindTask(void);
void __cdecl BattleFx_ThrowCidWeaponsClearTrailRings(void);
void __cdecl BattleFx_ThrowCidWeaponsKind0TimelineTask(void);
void __cdecl BattleFx_ThrowCidWeaponsKind0ThrownModelTask(void);
void __cdecl BattleFx_ThrowCidWeaponsKind1TimelineTask(void);
void __cdecl BattleFx_ThrowCidWeaponsKind1ThrownModelTask(void);
void __cdecl BattleFx_ThrowCidWeaponsKind2TimelineTask(void);
void __cdecl BattleFx_ThrowCidWeaponsKind2TrailRibbonTask(void);
void __cdecl BattleFx_ThrowCidWeaponsDrawGouraudQuad(struct BattleFxThrowCidTrailRibbon *quadRecord, int factor, int divisor);
void __cdecl BattleFx_ThrowCidWeaponsProjectSegmentPoints(struct BattleFxThrowCidTrailRibbon *srcMesh, struct SVECTOR *outPoints, struct SVECTOR *transformState);
void __cdecl BattleFx_ThrowCidWeaponsPushTrailPoint(struct BattleFxCidTrailSource *trailBuffer, struct BattleFxThrowTrailPoint *pNewPoint);
void __cdecl BattleFx_ThrowCidWeaponsKind2ThrownModelTask(void);

/* battle_fx_throw_cloud.cpp */
void __cdecl Battle_ThrowFxSpawnWeapons00to61(int targetMask, int actorIdx);
void __cdecl BattleFx_ThrowCloudWeaponsDispatchKindTask(void);
void __cdecl BattleFx_ThrowCloudWeaponsStraightTimelineTask(void);
void __cdecl BattleFx_ThrowCloudWeaponsStraightProjectileTask(void);

/* battle_fx_throw_vincent.cpp */
void __cdecl Battle_ThrowFxSpawnWeapons114to126(int targetMask, int actorIdx);
void __cdecl BattleFx_ThrowVincentWeaponsSpawnTimeline(void);
void __cdecl BattleFx_ThrowVincentWeaponsTimelineTask(void);
void __cdecl BattleFx_ThrowVincentWeaponsThrownModelTask(void);

/* battle_fx_throw_yuffie.cpp */
extern int g_BattleThrowYuffieActorIndex;
extern struct VECTOR g_BattleThrowYuffieTransformedOffset;
extern struct GfxTableCursor *g_BattleThrowYuffieLoadedResTable;
void __cdecl Battle_ThrowFxSpawnWeapons87to100(int targetMask, int actorIdx);
void __cdecl BattleFx_ThrowYuffieWeaponsDispatchKindTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind0TimelineTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind0ThrownModelTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind1TimelineTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind1ThrownModelTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind2TimelineTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind2ThrownModelTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind2TrailModelTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind3TimelineTask(void);
void __cdecl BattleFx_ThrowYuffieWeaponsKind3ThrownModelTask(void);

/* battle_fx_typhon_summon.cpp */
extern short g_BattleSummonTyphonActorTargets[12];
extern struct BattleMagicFullParticle g_BattleSummonTyphonRingBuffer[300];
void __cdecl BattleFx_SummonBeginTyphon(int targetMask, int actorIdx);
void __cdecl BattleFx_TyphonSpawnCameraScript(int scriptData, int basisMatrix, int actorIdx);
void __cdecl BattleFx_TyphonRunCameraScript(void);
short __cdecl BattleFx_TyphonReadCameraScriptOperand(short **cursorPtr);
void __cdecl BattleFx_SummonTyphonClearParticleCallbacks(void);
void __cdecl BattleFx_SummonTyphonParticleCallbackTask(void);
void __cdecl BattleFx_SummonTyphonTimelineTask(void);
void __cdecl BattleFx_SummonTyphonBeginFadeOverlay(int frames);
void __cdecl BattleFx_SummonTyphonFadeOverlayTask(void);
void __cdecl BattleFx_SummonTyphonSetFadeOverlayFadeOutRate(int frames);
void __cdecl BattleFx_SummonTyphonFlashColorCycleTask(void);
void __cdecl BattleFx_SummonTyphonParticleModel2EmitterTask(void);
void __cdecl BattleFx_SummonTyphonParticleModel2Task(void);
void __cdecl BattleFx_SummonTyphonParticleModel3EmitterTask(void);
void __cdecl BattleFx_SummonTyphonParticleModel3Task(void);
void __cdecl BattleFx_SummonTyphonPairedModelTask(void);
int __cdecl BattleFx_SummonTyphonInstallScaledPointMatrix(struct BattleFxSourcePoint *point, short value, int distance);
void __cdecl BattleFx_SummonTyphonOrbitSpawnTask(void);
void __cdecl BattleFx_SummonTyphonOrbitParticleTask(void);
void __cdecl BattleFx_SummonTyphonDeformingRingsTask(void);
void __cdecl BattleFx_SummonTyphonInitRingPackets(void);
void __cdecl BattleFx_SummonTyphonDrawRingQuads(int phase, int shade);
void __cdecl BattleFx_SummonTyphonUpdateRingRecords(int angle, int radius, int scale, int phaseOffset);
void __cdecl BattleFx_SummonTyphonPrepareStageOffscreenRequests(void);
void __cdecl BattleFx_SummonTyphonQueueStageOffscreenRequests(void *matrix, struct BattleFxScreenRect *params);
void __cdecl BattleFx_SummonTyphonParticleEmitterTask(void);
void __cdecl BattleFx_SummonTyphonEmittedTumblingModelTask(void);
void __cdecl BattleFx_SummonTyphonDrawIndexedModel(void *matrix, int modelIndex, short arg3);
int __cdecl BattleFx_SummonTyphonDrawPsxMesh(struct BattleFxProceduralMesh *psxMesh, int drawList, int layer, int listHead);
void __cdecl BattleFx_SummonTyphonRotatingModelTask(void);
void __cdecl BattleFx_SummonTyphonCameraRotationTask(void);
void __cdecl BattleFx_SummonTyphonHexagonQuadsTask(void);
void __cdecl BattleFx_SummonTyphonModelHexagonEmitterTask(void);
int __cdecl BattleFx_SummonTyphonClaimTaskRecord(int taskFunc);
void __cdecl BattleFx_SummonTyphonBouncingModelRecordTask(struct BattleFxTyphonBouncingRecord *bouncer);
void __cdecl BattleFx_SummonTyphonScaledPointSpriteRecordTask(struct BattleFxTyphonBillboardRecord *record);
void __cdecl BattleFx_SummonSpawnTyphonActorFallPerTarget(short actor);
void __cdecl BattleFx_SummonTyphonActorFallTask(void);
void __cdecl BattleFx_SummonTyphonBillboardSpriteRecordTask(struct BattleFxTyphonBillboardRecord *record);
void __cdecl BattleFx_SummonTyphonActorFlagFadeTask(void);
void __cdecl BattleFx_SummonTyphonWhiteFadeInTask(void);
void __cdecl BattleFx_SummonTyphonFadeOverlayPulseTask(void);

/* battle_init_exp_results.cpp */
int __cdecl Battle_UpdateExpResultRows(int forceComplete);

/* battle_item_fx_greens.cpp */
void __cdecl Battle_ItemFxGreensSpawnPerTarget(int actor, int sourceIndex);

/* battle_kernel_experience_growth.cpp */
extern unsigned int g_BattlePartyGrowthStats[3][12];

/* battle_kernel_materia_support.cpp */
extern struct BattleMagicListRecord g_MateriaMagicListRecords[56];
extern struct MateriaStatPercentModifiers g_MateriaStatPercentModifiers;
extern struct BattleCommandListEntry g_MateriaCommandListEntries[16];
void __cdecl Battle_LoadCombatantFromSavemap(char partySlot);
void __cdecl Kernel_ApplyCounterSupport(int unused, char level, int partnerWord);
void __cdecl Kernel_AddMpTurboToMagicEntries(char amount, char materiaId, int ap);
void __cdecl Kernel_OrSupportFlagIntoCommandEntry(char materiaId, int ap);
void __cdecl Kernel_OrSupportFlagIntoMagicEntries(int materiaId, int ap);
void __cdecl Kernel_AddQuadraMagicToMagicEntries(char amount, char materiaId, int packedValue);
void __cdecl Kernel_AddAllSupportToEveryMagicEntry(char amount);
void __cdecl Kernel_AddAllSupportLevelToMagicEntries(char amount, int materiaId, int packedValue);
int __cdecl Kernel_FindCommandListSlot(int commandCode);
int __cdecl Kernel_FindMagicListSlot(int magicId);
void __cdecl Kernel_ApplyDoubleCastMateria(char unused, char materiaId, int packedValue);
void __cdecl Kernel_ApplySupportMateria(char selector, char materiaId, int packedValue);
void __cdecl Kernel_DescribeSupportMateriaCount(int materiaId, int packedValue);
void __cdecl Kernel_DescribeSupportMateriaPercent(int materiaId, int packedValue);
void __cdecl Kernel_ApplyMateriaEncounterRate(char materiaId, int ap);
void __cdecl Kernel_ApplySummonMateria(char materiaId, int ap);
void __cdecl Kernel_InitActorMateriaScratchState(char actorIndex, struct MenuPartyStats *actorState);

/* battle_load_party_actors.cpp */
void __cdecl Battle_RefreshActorCommandTargeting(int actorIndex);
void __cdecl Kernel_ApplyAccessoryAutoStatus(int charIndex, int accessoryIndex);
int __cdecl Battle_ApplyBattleSquareHandicap(int actorIndex);

/* battle_opening_actor_setup.cpp */
void __cdecl Battle_BindPartyActorToModel(char actorIndex);

/* battle_pick_random_attacks.cpp */
void __cdecl Battle_RollReactionTriggers(int actorIndex, int triggerMode);
int __cdecl Battle_ResolveReactionAttackDescriptor(int unusedArg, int slotIndex, int attackId, int *descriptor);

/* battle_ring_buffer_effects.cpp */
void __cdecl BattleFx_StartScreenFlashRamp(int duration);

/* battle_fx_special_club_card.cpp */
void __cdecl BattleFx_SpecialBeginClubCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialClubCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialClubCardTimelineTask(void);
void __cdecl BattleFx_SpecialClubCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialClubCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialClubCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialClubCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialClubCardDelayedImpactTask(void);
void __cdecl BattleFx_SpecialClubCardSpawnPulsingModelWithSound(int targetActorIdx, int actorIdx);
void __cdecl BattleFx_SpecialClubCardSpawnPulsingModel(int targetActorIdx, int actorIdx);
void __cdecl BattleFx_SpecialClubCardPulsingModelTask(void);

/* battle_fx_special_star_card.cpp */
void __cdecl BattleFx_SpecialBeginStarCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialStarCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialStarCardTimelineTask(void);
void __cdecl BattleFx_SpecialStarCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialStarCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialStarCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialStarCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialStarCardStageFadeTask(void);
void __cdecl BattleFx_SpecialStarCardSpawnImpactTimeline(int attackerIndex, int targetIndex);
void __cdecl BattleFx_SpecialStarCardImpactTimelineTask(void);
void __cdecl BattleFx_SpecialStarCardFallingModelTask(void);
void __cdecl BattleFx_SpecialStarCardSpreadingModelTask(void);
void __cdecl BattleFx_SpecialStarCardStretchedSprite1Task(void);
void __cdecl BattleFx_SpecialStarCardStretchedSprite2Task(void);
void __cdecl BattleFx_SpecialStarCardDrawBillboardSpriteTask(void);
void __cdecl BattleFx_SpecialStarCardBlackModelTask(void);
void __cdecl BattleFx_SpecialStarCardCyclingModelTask(void);
void __cdecl BattleFx_SpecialStarCardPulsingModelTask(void);

/* battle_fx_special_sewer_tsunami.cpp */
short __cdecl BattleFx_SpecialSewerTsunamiSampleWaveHeight(short angle, short row);

/* battle_fx_special_heart_card.cpp */
void __cdecl BattleFx_SpecialBeginHeartCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialHeartCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialHeartCardTimelineTask(void);
void __cdecl BattleFx_SpecialHeartCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialHeartCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialHeartCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialHeartCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialHeartCardDelayedImpactTask(void);
void __cdecl BattleFx_SpecialHeartCardSpawnBoneParticleEmitterWithSound(int targetActorIdx, int actorIdx);
void __cdecl BattleFx_SpecialHeartCardSpawnBoneParticleEmitter(int targetActorIdx, int actorIdx);
void __cdecl BattleFx_SpecialHeartCardBoneParticleEmitterTask(void);
void __cdecl BattleFx_SpecialHeartCardBoneParticleSpriteTask(void);

/* battle_fx_special_hidden_one_card.cpp */
extern int g_BattleSpecialHiddenOneCardResourceDefs;
extern int g_BattleSpecialHiddenOneCardScaleFixed12;
void __cdecl BattleFx_SpecialBeginHiddenOneCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialHiddenOneCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialHiddenOneCardTimelineTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialHiddenOneCardDelayedImpactTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardBeginGatherToCenter(int targetActorIdx, int actorIdx);
void __cdecl BattleFx_SpecialHiddenOneCardGatherToCenterTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardScalingSpritePairTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardParticleBillboardTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardGrowingModelTask(void);
void __cdecl BattleFx_SpecialHiddenOneCardPullActorToCenterTask(void);

/* battle_special_spread_laser_effects.cpp */
extern struct BattleSpecMizufukiSpriteRequest2 g_BattleSpecialSpreadLaserSpriteConfig1;

/* battle_special_chefs_knife_effects.cpp */
extern struct BattleSpecChefsKnifeAnimData g_BattleSpecChefsKnifeAnimData;

/* battle_fx_special_waterball.cpp */
void __cdecl BattleFx_SpecialSpawnWaterballPerTarget(int actor, int sourceIndex);

/* battle_fx_special_spade_card.cpp */
void __cdecl BattleFx_SpecialBeginSpadeCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialSpadeCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialSpadeCardTimelineTask(void);
void __cdecl BattleFx_SpecialSpadeCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialSpadeCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialSpadeCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialSpadeCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialSpadeCardDelayedImpactTask(void);

/* battle_fx_special_emperor_card.cpp */
void __cdecl BattleFx_SpecialBeginEmperorCard(int targetMask, int actorIdx);
void __cdecl BattleFx_SpecialEmperorCardSpawnTimeline(void);
void __cdecl BattleFx_SpecialEmperorCardTimelineTask(void);
void __cdecl BattleFx_SpecialEmperorCardSpinningModelTask(void);
void __cdecl BattleFx_SpecialEmperorCardOscillatingModelTask(void);
void __cdecl BattleFx_SpecialEmperorCardAcceleratingModelTask(void);
void __cdecl BattleFx_SpecialEmperorCardBuildFacingVectors(void);
void __cdecl BattleFx_SpecialEmperorCardDelayedImpactTask(void);
void __cdecl BattleFx_SpecialEmperorCardSpawnBoneParticleEmitterWithSound(int targetActorIdx, int actorIdx);
void __cdecl BattleFx_SpecialEmperorCardSpawnBoneParticleEmitter(int targetActorIdx, int actorIdx);
void __cdecl BattleFx_SpecialEmperorCardBoneParticleEmitterTask(void);
void __cdecl BattleFx_SpecialEmperorCardBoneParticleSpriteTask(void);

/* battle_target_effect_matrices.cpp */
void __cdecl BattleFx_MakeRandomConeVector(short baseY, short spread, void *scale);

/* battle_write_party_rewards.cpp */
void __cdecl Battle_WritePartyStateBackToSavemap(int hpOverride, int mpOverride);

/* vahamut0.cpp */
extern struct BattleFxFlareSpriteSlot g_BattleFxBahamutZeroSprite;
extern struct BattleFxBzFlarePointSlot g_BattleSummonBahamutZeroSlot10And16Points;

struct BattleActorTransformRecord;
struct BattleFxGradientBandColor;
struct BattleMenuGraphicsPathBuffer;
struct BattleSummonOdinZRecordBuffer1Entry;
struct CameraScriptTrack;

/* b3ddata.cpp */
extern struct Battle3dScene *g_pBattle3dScene;

/* battle.cpp */
extern struct BattleMenuGraphicsPathBuffer g_BattleMenuGraphicsPathBuffer;
extern unsigned char g_BattleMessageWindowTextId[4];

/* battle_actor_overlay_draw.cpp */
extern int g_BattleEnemyModelIds[2];
extern int g_BattleEnemySlots;
int __cdecl BattleFx_DrawScreenGradientBand(short x, short y, short width, short height, struct BattleFxGradientBandColor *colorMask);

/* battle_bone_world_camera.cpp */
void __cdecl Battle_GetBoneWorldPos(int actorIdx, int boneIdx, short *outPos);
void __cdecl Battle_WorldPointToCameraLocalDup(struct BattleCameraWorldPointScratch *worldPoint, struct BattleCameraLocalPoint2 *localPoint);

/* battle_damage_formula_set.cpp */
extern BattleDmgFormulaFn g_BattleDamageFormulaTable[32];

/* battle_damage_hit_checks.cpp */
extern const BattleDmgOpFn g_BattleDamageOpTable[8];

/* battle_effect_summon_actors.cpp */
extern const char *g_BattleLimitModelFileTable[6];

/* battle_effect_trail_ribbon.cpp */
extern unsigned char g_BattleStageNodeState[4];
extern unsigned char g_BattleScriptFlags[8];

/* battle_fx_alexander_summon.cpp */
extern int g_BattleSummonAlexanderBoneIndices[12];
extern unsigned char g_BattleSummonAlexanderQuad[4];

/* battle_fx_blue_bad_breath_effect.cpp */
extern unsigned char g_BattleBlueBadBreathParticlePool[3544];

/* battle_fx_blue_death_force_effect.cpp */
void __cdecl BattleFx_BlueDeathForceDrawWaveStrips(int amplitude, int angleStep, int startAngle, int gray);
int __cdecl BattleFx_LookupSineFromQuarterTable(int angle);

/* battle_fx_blue_dragon_force_effect.cpp */
extern short g_BattleBlueDragonForceTriggerFrames[2];

/* battle_fx_blue_flame_thrower_effect.cpp */
extern int g_BattleBlueFlameThrowerSoundGroups[2];

/* battle_fx_blue_l5_death_effect.cpp */
extern int g_BattleBlueL5DeathResourceTable[10];

/* battle_fx_blue_magic_breath_effect.cpp */
extern int g_BattleBlueMagicBreathResourceTable[6];

/* battle_fx_blue_pandoras_box_effect.cpp */
extern int g_BattleBluePandorasBoxSoundGroup[2];

/* battle_fx_ifrit_summon.cpp */
extern struct CameraScriptTrack g_BattleSummonIfritCameraFocus;
extern struct CameraScriptTrack g_BattleSummonIfritCameraEye;

/* battle_fx_magic_ultima.cpp */
extern int g_BattleMagicUltimaSoundDefs[2];

/* battle_fx_magic_despell.cpp */
extern short g_BattleMagicDespellFrameThresholds[2];

/* battle_fx_magic_fire.cpp */
extern struct BattleActorTransformRecord g_BattleActorTransformRecords[10];

/* battle_fx_magic_hitmark.cpp */
extern struct SVECTOR g_BattleMagicHitmark_ParticleVertex0;

/* battle_fx_magic_limit_flash.cpp */
extern short g_BattleLimitFlash_SpriteDimensions[2];

/* battle_fx_magic_quake3.cpp */
extern short g_BattleMagicQuake3TransformMatrix[16];

/* battle_fx_magic_sleepel.cpp */
extern int g_BattleMagicSleepelResourceDefs[6];

/* battle_fx_special_shady_breath.cpp */
extern unsigned char g_BattleSpecShadyBreathLightRecords[3544];

/* battle_fx_special_blaster.cpp */
extern int g_BattleSpecialBlasterModelReq4[4];

/* battle_fx_special_mortar_effect.cpp */
extern short g_BattleSpecMortarShellScales[2];

/* battle_fx_special_heat_blast_effect.cpp */
extern int g_BattleSpecHeatBlastAnimData[6];

/* battle_fx_special_snowstorm.cpp */
extern int g_BattleSpecialSnowstormResourceTable[20];

/* battle_fx_special_avalanche.cpp */
extern short g_BattleSpecAvalancheFaceActiveFlags[128];
extern short g_BattleSpecAvalancheFaceOrder[128];

/* battle_fx_special_northern_cross.cpp */
extern struct BattleFxSpriteRequest g_BattleSpecialNorthernCrossSpriteReq1;

/* battle_fx_special_southern_cross.cpp */
extern struct BattleFxSpriteRequest g_BattleSpecialSouthernCrossColor1;

/* battle_fx_special_smog.cpp */
extern struct BattleFxSpriteRequest g_BattleSpecSmogSpriteReq1;

/* battle_fx_special_evil_poison.cpp */
extern short g_BattleSpecialEvilPoisonScales[2];

/* battle_fx_special_refuse_spit.cpp */
extern short g_BattleSpecialRefuseSpitBillboardScales[2];

/* battle_fx_special_war_cry.cpp */
extern int g_BattleSpecWarCryResourceModelRows[16];

/* battle_fx_special_siren_song_effect.cpp */
extern int g_BattleSpecSirenSongAnimData[6];

/* battle_fx_summon_neo_bahamut.cpp */
extern unsigned char g_BattleSummonNeoBahamutCloudModelBuffer[0x2200];

/* battle_fx_summon_odin_zantetsuken.cpp */
extern unsigned char g_BattleSummonOdinZTimelineScratchReady[0x4];
extern struct CameraScriptTrack g_BattleSummonOdinCameraFocus;
extern struct CameraScriptTrack * g_pBattleSummonOdinZCameraTrack;
extern struct CameraScriptTrack * g_pBattleSummonOdinZCameraOther;
extern struct BattleSummonOdinZRecordBuffer1Entry g_BattleSummonOdinZRecordBuffer1[0x24];
extern struct CameraScriptTrack g_BattleSummonOdinCameraEye;

/* battle_fx_summon_titan.cpp */
extern struct SVECTOR g_BattleSummonTitanRotAngles1;

/* battle_hit_reaction_task_slots.cpp */
short __cdecl Battle_AllocateOverlayTaskSlot(BattleTaskFn callback);
short __cdecl Battle_AllocateSecondaryResourceSlot(BattleTaskFn callback);
short __cdecl Battle_AllocateAuxTaskSlot(BattleTaskFn callback);

/* battle_kernel_materia_support.cpp */
extern short g_MateriaAdditiveStatBonuses[6];

/* battle_fx_special_sewer_tsunami.cpp */
extern unsigned char g_BattleSpecSewerTsunamiCoordBuffer[0x200];
extern unsigned char g_BattleSpecSewerTsunamiStateBuffer[0x1e00];

/* battle_fx_special_waterball.cpp */
extern struct BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest2;
extern struct BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest3;
extern struct BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest6;

/* battle_vector_angle_math.cpp */
extern unsigned char g_BattleFxDrawListBuffer0[0xc000];
extern unsigned char g_BattleFxDrawListBuffer1[0xc000];

/* sting.cpp */
extern struct CameraScriptTrack g_RubyTentaclesCameraFocus;
extern struct CameraScriptTrack *g_RubyTentaclesCameraTrack;
extern struct CameraScriptTrack *g_RubyTentaclesCameraOther;
extern struct CameraScriptTrack g_BattleRubyTentaclesCameraEye;

struct BattleFxLimitSpokes;
struct BattlePartyModelLoadSlot;
struct ShortQuad;

/* battle_actor_overlay_draw.cpp */
extern struct BattlePartyModelLoadSlot g_BattlePartyModelLoadSlots[2];

/* battle_fx_limit_stardust_ray.cpp */
void __cdecl BattleFx_LimitStardustRayDrawSpokes(struct BattleFxLimitSpokes *spokes, struct Int64Pair *centerPair, int count, int color);

/* battle_fx_magic_ice2.cpp */
extern unsigned char g_BattleMagicIce2_Filename[12];

/* battle_fx_magic_ice.cpp */
extern unsigned char g_BattleMagicIce_Filename[12];

/* battle_fx_magic_haste.cpp */
extern unsigned char g_BattleMagicHasteModelFilenames[24];

/* battle_fx_magic_mbarrier.cpp */
extern unsigned char g_BattleMagicMBarrier_Filename[12];

/* battle_fx_magic_reflect.cpp */
extern unsigned char g_BattleMagicReflect_Filename[12];

/* battle_fx_magic_sleepel.cpp */
extern unsigned char g_BattleMagicSleepelFilename[12];

/* battle_fx_shiva_summon.cpp */
int __cdecl BattleFx_SummonShivaClaimParticleRecord(struct Int64Pair *firstPair, struct Int64Pair *secondPair, short state);

/* battle_fx_special_silver_wheel.cpp */
int __cdecl BattleFx_SpecialSilverWheelInstallScaledSpriteMatrix(int srcVector, struct ShortQuad rotation, short scale);

/* battle_fx_blue_angel_whisper_effect.cpp */
extern unsigned char g_BattleFxBlueAngelWhisperEffectResourceNames[36];

/* battle_fx_blue_beta_effect.cpp */
extern unsigned char g_BattleFxBlueBetaEffectResourceNames[24];

/* battle_fx_blue_chocobuckle_effect.cpp */
extern unsigned char g_BattleFxBlueChocobuckleEffectResourceNames[48];

/* battle_fx_blue_flame_thrower_attach_effect.cpp */
extern unsigned char g_BattleFxBlueFlameThrowerAttachEffectResourceNames[36];

/* battle_fx_blue_frog_song_effect.cpp */
extern unsigned char g_BattleFxBlueFrogSongEffectResourceNames[56];

/* battle_fx_blue_l5_death_effect.cpp */
extern unsigned char g_BattleFxBlueL5DeathEffectResourceNames[36];

/* battle_fx_blue_magic_breath_effect.cpp */
extern unsigned char g_BattleFxBlueMagicBreathEffectResourceNames[24];

/* battle_fx_blue_big_guard_effect.cpp */
extern unsigned char g_BattleFxBlueBigGuardEffectAssetPrefix[4];

/* battle_fx_blue_shadow_flare_effect.cpp */
extern unsigned char g_BattleFxBlueShadowFlareResourceNames[116];

/* battle_fx_blue_trine_effect.cpp */
extern unsigned char g_BattleFxBlueTrineEffectResourceNames[28];

/* battle_fx_chocomog_summon.cpp */
extern unsigned char g_BattleFxChocomogSummonResourceNames[612];

/* battle_fx_ifrit_summon.cpp */
extern unsigned char g_BattleFxIfritSummonResourceNames[436];

/* battle_fx_limit_braver.cpp */
extern unsigned char g_BattleFxLimitBraverResourceNames[72];

/* battle_fx_limit_landscaper.cpp */
extern unsigned char g_BattleFxLimitLandscaperResourceNames[128];

/* battle_fx_limit_highwind.cpp */
extern unsigned char g_BattleFxLimitHighwindResourceNames[96];

/* battle_fx_limit_big_shot.cpp */
extern unsigned char g_BattleFxLimitBigShotDataPool[76];

/* battle_fx_limit_lucky_girl.cpp */
extern unsigned char g_BattleFxLimitLuckyGirlResourceNames[48];

/* battle_fx_limit_clear_tranquil.cpp */
extern unsigned char g_BattleFxLimitClearTranquilResourceNames[48];

/* battle_fx_limit_fury_brand.cpp */
extern unsigned char g_BattleFxLimitFuryBrandResourceNames[104];

/* battle_fx_limit_stardust_ray.cpp */
extern unsigned char g_BattleFxLimitStardustRayResourceNames[116];

/* battle_fx_magic_ultima.cpp */
extern unsigned char g_BattleFxMagicUltimaUltimaDataPool[52];

/* battle_fx_magic_barrier.cpp */
extern unsigned char g_BattleFxMagicBarrierResourceNames[48];

/* battle_fx_magic_bio3_multi.cpp */
extern unsigned char g_BattleFxMagicBio3MultiResourceNames[60];

/* battle_fx_magic_bio3.cpp */
extern unsigned char g_BattleFxMagicBio3ResourceNames[164];

/* battle_fx_magic_ice3.cpp */
extern unsigned char g_BattleFxMagicIce3ResourceNames[36];

/* battle_fx_magic_bolt.cpp */
extern unsigned char g_BattleFxMagicBoltResourceNames[68];

/* battle_fx_magic_bolt3.cpp */
extern unsigned char g_BattleFxMagicBolt3ResourceNames[44];
extern unsigned char g_BattleFxMagicBolt3ResourceNames2[40];
extern unsigned char g_BattleFxMagicBolt3ResourceNames3[36];

/* battle_fx_magic_remove.cpp */
extern unsigned char g_BattleFxMagicRemoveResourceNames[84];

/* battle_fx_magic_demi.cpp */
extern unsigned char g_BattleFxMagicDemiResourceNames[140];

/* battle_fx_magic_death.cpp */
extern unsigned char g_BattleFxMagicDeathResourceNames[24];

/* battle_fx_magic_fire2.cpp */
extern unsigned char g_BattleFxMagicFire2ResourceNames2[8];

/* battle_fx_magic_hitmark.cpp */
extern unsigned char g_BattleFxMagicHitmarkResourceNames[20];

/* battle_fx_magic_limit_flash.cpp */
extern unsigned char g_BattleFxMagicLimitFlashResourceNames[52];

/* battle_fx_magic_quake.cpp */
extern unsigned char g_BattleFxMagicQuakeBouldersResourceNames[24];

/* battle_fx_magic_quake2.cpp */
extern unsigned char g_BattleFxMagicQuake2ResourceNames[148];

/* battle_fx_magic_stop.cpp */
extern unsigned char g_BattleFxMagicStopClockResourceNames[12];

/* battle_fx_magic_bolt2.cpp */
extern unsigned char g_BattleFxMagicBolt2ResourceNames[124];

/* battle_fx_magic_tornado.cpp */
extern unsigned char g_BattleFxMagicTornadoResourceNames[60];

/* battle_fx_phoenix_summon.cpp */
extern unsigned char g_BattleFxPhoenixSummonResourceNames[144];

/* battle_fx_special_geyser.cpp */
extern unsigned char g_BattleFxSpecialGeyserResourceNames[24];

/* battle_fx_special_diamond_card.cpp */
extern unsigned char g_BattleFxSpecialDiamondCardResourceNames[96];

/* battle_fx_special_poison_breath.cpp */
extern unsigned char g_BattleFxSpecialPoisonBreathResourceNames[72];

/* battle_fx_special_poison_gas_effect.cpp */
extern unsigned char g_BattleFxSpecialPoisonGasEffectResourceNames[96];
extern unsigned char g_BattleFxSpecialPoisonGasEffectResourceNames2[108];

/* battle_fx_special_swamp_shoot.cpp */
extern unsigned char g_BattleFxSpecialSwampShootResourceNames[96];

/* battle_fx_special_sabotage.cpp */
extern unsigned char g_BattleFxSpecialSabotageResourceNames[60];

/* battle_fx_special_dual_mortar_effect.cpp */
extern unsigned char g_BattleFxSpecialDualMortarEffectResourceNames[128];

/* battle_fx_special_harrier.cpp */
extern unsigned char g_BattleFxSpecialHarrierResourceNames[120];

/* battle_fx_special_snowstorm.cpp */
extern unsigned char g_BattleFxSpecialSnowstormResourceNames[72];

/* battle_fx_special_jammer_effect.cpp */
extern unsigned char g_BattleFxSpecialJammerEffectResourceNames[8];

/* battle_fx_special_bomb_blast.cpp */
extern unsigned char g_BattleFxSpecialBombBlastResourceNames[276];

/* battle_fx_special_grand_spark.cpp */
extern unsigned char g_BattleFxSpecialGrandSparkResourceNames[96];

/* battle_fx_special_world_card.cpp */
extern unsigned char g_BattleFxSpecialWorldCardResourceNames[60];

/* battle_fx_special_fire_shell.cpp */
extern unsigned char g_BattleFxSpecialFireShellResourceNames[96];
extern unsigned char g_BattleFxSpecialFireShellResourceNames2[120];

/* battle_fx_special_pollen.cpp */
extern unsigned char g_BattleFxSpecialPollenResourceNames[60];

/* battle_fx_special_flying_sickle.cpp */
extern unsigned char g_BattleFxSpecialFlyingSickleResourceNames[164];

/* battle_fx_special_silver_wheel.cpp */
extern unsigned char g_BattleFxSpecialSilverWheelResourceNames[180];

/* battle_fx_special_coin_toss.cpp */
extern unsigned char g_BattleFxSpecialCoinTossResourceNames[48];

/* battle_fx_special_magma.cpp */
extern unsigned char g_BattleFxSpecialMagmaResourceNames[24];

/* battle_fx_special_beam_gun.cpp */
extern unsigned char g_BattleFxSpecialBeamGunResourceNames[60];

/* battle_fx_special_water_gun.cpp */
extern unsigned char g_BattleFxSpecialWaterGunResourceNames[48];

/* battle_fx_special_ultrasound.cpp */
extern unsigned char g_BattleFxSpecialUltrasoundResourceNames[24];

/* battle_fx_special_hot_springs.cpp */
extern unsigned char g_BattleFxSpecialHotSpringsResourceNames[24];

/* battle_fx_special_heart_breath_effect.cpp */
extern unsigned char g_BattleFxSpecialHeartBreathEffectResourceNames[120];

/* battle_fx_special_laser_cannon.cpp */
extern unsigned char g_BattleFxSpecialLaserCannonResourceNames[60];

/* battle_fx_special_search_scope.cpp */
extern unsigned char g_BattleFxSpecialSearchScopeResourceNames[76];

/* battle_fx_special_petrify_effect.cpp */
extern unsigned char g_BattleFxSpecialPetrifyEffectResourceNames[244];

/* battle_fx_special_sham_seal.cpp */
extern unsigned char g_BattleFxSpecialShamSealResourceNames[164];

/* battle_fx_special_evil_poison.cpp */
extern unsigned char g_BattleFxSpecialEvilPoisonResourceNames[172];

/* battle_fx_special_refuse_spit.cpp */
extern unsigned char g_BattleFxSpecialRefuseSpitResourceNames3[16];

/* battle_fx_special_time_damage_warp.cpp */
extern unsigned char g_BattleFxSpecialTimeDamageWarpResourceNames[56];

/* battle_fx_special_war_cry.cpp */
extern unsigned char g_BattleFxSpecialWarCryResourceNames[48];

/* battle_fx_special_icicle_drop.cpp */
extern unsigned char g_BattleFxSpecialIcicleDropResourceNames[128];

/* battle_fx_special_siren_song_effect.cpp */
extern unsigned char g_BattleFxSpecialSirenSongEffectResourceNames[24];

/* battle_fx_special_whirlpool.cpp */
extern unsigned char g_BattleFxSpecialWhirlpoolWhirlpoolResourceNames[60];

/* battle_fx_special_full_explosion_effect.cpp */
extern unsigned char g_BattleFxSpecialFullExplosionEffectResourceNames[48];

/* battle_fx_summon_hades.cpp */
extern unsigned char g_BattleFxSummonHadesResourceNames[180];

/* battle_fx_summon_leviathan.cpp */
extern unsigned char g_BattleFxSummonLeviathanResourceNames[112];

/* battle_fx_summon_neo_bahamut.cpp */
extern unsigned char g_BattleFxSummonNeoBahamutAssetPrefix[4];
extern unsigned char g_BattleFxSummonNeoBahamutResourceNames[276];

/* battle_fx_summon_odin_gungnir.cpp */
extern unsigned char g_BattleFxSummonOdinGungnirResourceNames[200];

/* battle_fx_weapon_emerald_shoot.cpp */
extern unsigned char g_BattleFxEmeraldShootProjectileResourceNames[132];

/* battle_fx_weapon_emerald_beam.cpp */
extern unsigned char g_BattleFxWeaponEmeraldBeamResourceNames[96];

/* battle_fx_weapon_revenge_stamp.cpp */
extern unsigned char g_BattleFxWeaponRevengeStampAssetPrefix[4];
extern unsigned char g_BattleFxWeaponRevengeStampResourceNames[136];

/* battle_opening_actor_setup.cpp */
extern unsigned char g_BattleMagicTypePrefix[4];
extern unsigned char g_BattleAssetAndCategoryStrings[180];

/* battle_special_frog_effects.cpp */
extern unsigned char g_BattleSpecialFrogEffectsResourceNames[108];

/* battle_fx_special_heart_card.cpp */
extern unsigned char g_BattleSpecialHeartCardResourceNames[28];

/* battle_fx_special_hidden_one_card.cpp */
extern unsigned char g_BattleSpecialHiddenOneCardResourceNames[84];

/* battle_special_chefs_knife_effects.cpp */
extern unsigned char g_BattleSpecialChefsKnifeEffectsResourceNames[44];

/* battle_fx_special_emperor_card.cpp */
extern unsigned char g_BattleSpecialEmperorCardResourceNames[16];

/* yasui_limit_ungarmax_impacts.cpp */
extern unsigned char g_YasuiLimitUngarmaxImpactsResourceNames[92];

/* yasui_limit_grenade_bomb.cpp */
extern unsigned char g_YasuiLimitGrenadeBombResourceNames[120];

/* yasui_limit_satan_slam_stage_shatter.cpp */
extern unsigned char g_YasuiLimitSatanSlamStageShatterResourceNames[72];

/* yasui_summon_knights_of_round.cpp */
extern unsigned char g_BattleSummonKotrModelWorkPool[7864];
extern unsigned char g_BattleSummonKotrSceneWorkPool[2304];
/* battle_init_exp_results.cpp */
extern unsigned int g_BattlePartyGrowthStats[3][12];

/* battle_ai_script_dispatch.cpp */
extern void (__cdecl *g_BattleAttackOpCallbacks[31])(void);
/* battle_status_effect_handlers.cpp */
extern unsigned int g_BattleEventQueue0Handlers[17];
extern unsigned int g_BattleEventQueue1Handlers[3];
extern unsigned int g_BattleEventQueue2Handlers[25];
/* battle_camera_channel_scripts.cpp */
extern void (__cdecl *g_BattleCameraScriptTaskCallback)(void);
/* battle_fx_draw_polygon_block.cpp */
struct BattleFxPsxPolygonPrim *__cdecl BattleFx_DrawPsxPolygonBlock(struct BattleFxPsxPolygonBlock *mesh, int reserved, int unusedArg3, struct BattleFxPsxPolygonPrim *fallbackValue, struct GfxPsxSpriteQuadState *quadStateA, struct GfxPsxSpriteQuadState *quadStateB, struct GfxPsxSpriteQuadState *quadStateC, struct GfxPsxSpriteQuadState *quadStateD);

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_BATTLE_H */

/* include/ff7/api/submarine.h: the functions and objects ff7/submarine defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_SUBMARINE_H
#define FF7_API_SUBMARINE_H

#ifdef __cplusplus
extern "C" {
#endif
struct SubmarineState;
struct SubmarineGridCell;
struct SubmarineFlightRecord;
struct PsxMatrix;
struct GfxEngineContext;
struct SubmarineAimRecord;
struct SubmarineAngleSet;
struct SubmarineDirectionAngles;
struct SubmarineDirectionVector;
struct SubmarineRecordMatrixRow;
struct SubmarineRotationAngles;
struct SubmarineRouteRecord;
struct SubmarineStateRecord;
struct SubmarineViewBlock;
struct SubmarineLargeGridBuffer;
struct SubmarineTriangleAnchor;
struct SubmarineRecordAngles;
struct SubmarineHudConfig;
struct SubmarineHudParams;
struct SubmarineHudRange;
struct PsxMatrix;
struct SubmarineObjectRecord;
struct SubmarineVec3Int;
struct GfxRenderObject;
struct SubmarineRadialOffset;

/* sub_bit_count_tables.cpp */
void __cdecl Bit_InitLeadingZeroCountTable(void);
void __cdecl Bit_InitLeadingOneCountTable(void);
int __cdecl Bit_CountLeadingSignBits(int value);

/* sub_grid_camera_batches.cpp */
extern struct GfxRenderObject *g_SubmarineGridBatchListA;
extern unsigned char g_SubmarineGridVertexBuffer[65536];
extern unsigned char g_SubmarineCameraBatchBuffer[12288];
extern struct GfxRenderObject *g_SubmarineModeBatchList0;
extern struct GfxRenderObject *g_SubmarineModeBatchList1;
extern struct GfxRenderObject *g_SubmarineModeBatchList2;
extern struct SubmarineLargeGridBuffer g_SubmarineLargeGridBuffer;
int __cdecl Submarine_BuildCameraGridBatches(void);
void __cdecl Submarine_EmitGridBatchesByMode(struct GfxEngineContext *frameContext);
void __cdecl Submarine_BuildGridRecordsFromCharGrid(void *charGrid);

/* sub_flagged_terrain_batches.cpp */
extern struct GfxRenderObject *g_SubmarineRadialTerrainBatchNode;
extern struct GfxRenderObject *g_SubmarineRadialTerrainScaledBatchNode;
extern int g_SubmarineFadeStep;
extern int g_SubmarineAlarmActive;
extern int g_SubmarineAccumulatedOffset;
extern struct SubmarineRadialOffset g_SubmarineOffsetTable[6];
extern unsigned char g_SubmarineIndexTable[32];
extern unsigned char g_SubmarinePointColors[32];
void __cdecl Submarine_CollectFlaggedGridCells(int maxCount);
void __cdecl Submarine_EmitRadialBandTerrainBatches(void);
void __cdecl Submarine_DrawScenePassBatches(struct GfxEngineContext *frameContext);

/* sub_record_update_and_draw.cpp */
extern int g_SubmarinePitchThresholdLow;
extern int g_SubmarinePitchThresholdHigh;
extern int g_SubmarineEffectActive[2];
extern struct GfxRenderObject *g_SubmarineLineBatchList;
extern int g_SubmarineEffectCount0;
extern int g_SubmarineEffectCount1;
extern int g_SubmarineActiveSlotCount;
extern int g_SubmarineSlotTotalWeight;
extern unsigned char g_SubmarineEffectSlotPad[8];
void __cdecl Submarine_ComputeRecordPitchYaw(void);
int __cdecl Submarine_Atan2Fixed(int x, int y);
int __cdecl Submarine_NormalizeVectorInPlace(struct SubmarineVec3Int *vector);
void __cdecl Submarine_UpdateObjectRecords(void);
void __cdecl Submarine_UpdateActiveFlagRecord(void);
void __cdecl Submarine_DrawObjectRecords(void);
void __cdecl Submarine_FillRouteRecordTable(struct SubmarineRouteRecord *outRecords, int level);
void __cdecl Submarine_CopyObjectRecordTable(struct SubmarineObjectRecord *objectRecords);
void __cdecl Submarine_DrawFlaggedRecordOutlines(void);
void __cdecl Submarine_EmitStripBatchList(struct GfxEngineContext *frameContext);

/* sub_atlas_quad_emit.cpp */
void __cdecl Submarine_DrawAtlasQuad(int originX, int originY, short srcX, short srcY, short width, short height, short alpha, int depth, int batch, int batchB, int batchC, int batchD, int color0, int color1, int color2, int color3);

/* sub_record_update_and_draw.cpp: the HUD's initialized globals */
extern int g_SubmarineHudScale;
extern int g_SubmarineHudFlag;
extern struct SubmarineHudParams g_SubmarineHudParams;
extern struct SubmarineHudRange g_SubmarineHudRange;
extern int g_SubmarineHudOffset;
extern int g_SubmarineHudOrigin;
extern struct SubmarineHudConfig g_SubmarineScreenShake;
extern unsigned char g_SubmarineHudAtlasCoords[32];

/* sub_hud_draw_and_update.cpp */
extern int g_SubmarineHudReadoutAtlasD;
extern int g_SubmarineHudReadoutAtlas;
extern int g_SubmarineHudTextAtlas;
extern int g_SubmarineHudTextAtlasB;
extern int g_SubmarineHudBarAtlasD;
extern int g_SubmarineHudTextAtlasD;
extern int g_SubmarineHudBatchTexB;
extern int g_SubmarineHudBarAtlasA;
extern int g_SubmarineHudTextAtlasC;
extern int g_SubmarineHudSingleTextAtlasC;
extern int g_SubmarineHudScreenTint;
extern int g_SubmarineHudBarAtlasC;
extern int g_SubmarineHudReadoutAtlasB;
extern int g_SubmarineHudSingleTextAtlasB;
extern int g_SubmarineHudTextAtlasA;
extern int g_SubmarineHudBatchTexC;
extern int g_SubmarineHudReadoutAtlasA;
extern int g_SubmarineHudMainAtlas;
extern int g_SubmarineHudBarAtlas;
extern int g_SubmarineHudMainAtlasA;
extern int g_SubmarineHudRenderObjectA;
extern int g_SubmarineHudRenderObjectB;
extern int g_SubmarineHudBatchTexD;
extern int g_SubmarineHudRenderObjectD;
extern int g_SubmarineHudRenderObjectE;
extern int g_SubmarineHudClearedWord;
extern int g_SubmarineHudScrollPos;
extern int g_SubmarineHudBarAnimFrame;
extern int g_SubmarineHudBarAnimIndex;
extern int g_SubmarineHudValueQuad5Timer;
extern int g_SubmarineHudValueQuad1Timer;
extern int g_SubmarineHudValueQuad3Timer;
extern int g_SubmarineUpVectorX;
extern int g_SubmarineUpVectorY;
extern int g_SubmarineUpVectorZ;
void __cdecl Submarine_DrawHud(void);
void __cdecl Submarine_DrawSingleAtlasQuadRecord(void);
void __cdecl Submarine_DrawFiveAtlasQuadRecords(void);
void __cdecl Submarine_FillFullScreenTint(void);
void __cdecl Submarine_DrawSwapFrameAtlasQuads(void);
void __cdecl Submarine_DrawFifteenAtlasQuadRecords(void);
void __cdecl Submarine_DrawSeventeenAtlasQuadRecords(void);
void __cdecl Submarine_DrawNineAtlasQuadRecords(void);
void __cdecl Submarine_DrawTargetTallyPanel(void);
void __cdecl Submarine_DrawScorePanel(void);
void __cdecl Submarine_EmitHudRenderObjects(struct GfxEngineContext *renderContext);
void __cdecl Submarine_UpdateHudGauges(void);
void __cdecl Submarine_UpdateAlertHudAndAlarm(void);
void __cdecl Submarine_UpdateTorpedoAudio(void);
void __cdecl Submarine_UpdateEnemyAudio(void);
void __cdecl Submarine_InitTargetCountDigits(void);
void __cdecl Submarine_UpdateScreenShake(void);
void __cdecl Submarine_UpdateFiveDigitHudCounter(void);
void __cdecl Submarine_UpdateTwoDigitHudCounter(void);
void __cdecl Submarine_SetSwapQuadSelection(int mode);
void __cdecl Submarine_BuildHudElementTable(void);

/* sub_object_emit_batches.cpp */
extern struct GfxRenderObject *g_SubmarineFixedBatchList;
extern struct SubmarineTriangleAnchor g_SubmarineTriangleColorBuffer;
extern short g_SubmarineRenderBatchState;
extern short g_SubmarineTriangleBatchCount;
void __cdecl Submarine_EmitObjectTriangleBatch(struct SubmarineAngleSet *angles);
void __cdecl Submarine_DrawFixedBatchPass(struct GfxEngineContext *renderContext);

/* sub_ending_tables.cpp */
extern unsigned char g_SubmarineEndingTable0[632];
extern unsigned char g_SubmarineEndingTable1[600];
extern unsigned char g_SubmarineEndingTable2[1800];
extern unsigned char g_SubmarineEndingTable3[416];
extern unsigned char g_SubmarineEndingTable4[416];
extern unsigned char g_SubmarineEndingTable5[1248];
extern unsigned char g_SubmarineEndingTable6[1248];
extern unsigned char g_SubmarineEndingTable7[1248];

/* sub_grid_init_and_step.cpp */
void __cdecl Submarine_InitGridHeightsFromBytes(int heightBytes);
void __cdecl Submarine_AllocateGridRecordSlots(short value);
void __cdecl Submarine_StepGridCellHeights(void);

/* sub_module_lifecycle.cpp */
extern int g_SubmarineViewportCenterY;
extern int g_SubmarineViewportCenterX;
extern int g_SubmarineLeadingOnesEntry252;
extern int g_SubmarineLeadingOnesEntry253;
extern int g_SubmarineLeadingOnesEntry254;
extern int g_SubmarineLeadingOnesEntry255;
extern int g_SubmarineGameState;
extern int g_SubmarineFlags;
extern unsigned char g_SubmarineMissionId;
extern int g_SubmarineResultCode;
extern int g_SubmarineRemainingTime;
extern short g_SubmarineSubState;
extern int g_SubmarineTimeRemaining;
void __cdecl Submarine_ModuleEnter(struct GfxEngineContext *ctx);
void __cdecl Submarine_ModuleExit(struct GfxEngineContext *ctx);
void __cdecl Submarine_ModuleFrame(struct GfxEngineContext *ctx);
void __cdecl Submarine_ReseedAndInstallViewMatrix(void);
void __cdecl Submarine_SelectSubmarineDataBlocks(char levelId);
void __cdecl Submarine_RequestModuleSwitchWithSelector(int selector);
void __cdecl Submarine_SendSoundSetupSequence(unsigned char mode);
void __cdecl Submarine_StartStateLerp16(void);
void __cdecl Submarine_InitStateAndTables(void);
void __cdecl Submarine_FrameRateLimit(struct GfxEngineContext *ctx);

/* sub_view_matrix_build.cpp */
extern struct PsxMatrix g_SubmarineViewMatrix;
extern int g_SubmarineViewPad;
void __cdecl Submarine_RebuildViewTransform(void);
void __cdecl Submarine_BuildRotationMatrixFromAngles(struct SubmarineViewBlock *viewBlock, struct PsxMatrix *matrix);

/* sub_record_update_and_draw.cpp: the working level block */
extern int g_SubmarineWorkingLevelParams[3];
extern int g_SubmarineWorkingLevelLimit;
extern int g_SubmarineWorkingLevelAngles[2];
extern int g_SubmarineWorkingLevelStep;
extern unsigned char g_SubmarineWorkingLevelVector[8];
extern int g_SubmarineWorkingLevelDepth;
extern int g_SubmarineWorkingLevelRouteTable[128][8];
extern short g_SubmarineModelBoundMinX;
extern short g_SubmarineModelBoundMaxX;
extern short g_SubmarineModelBoundMinY;
extern short g_SubmarineModelBoundMaxY;
extern int g_SubmarineModelColorMask;
extern int g_SubmarineModelParamA;
extern int g_SubmarineModelParamB;
extern unsigned short g_SubmarineModelIndicesA[192];
extern unsigned short g_SubmarineModelIndicesB[192];
extern unsigned char g_SubmarineModelVertexData[1344];

/* sub_terrain_data2.cpp */
extern unsigned char g_SubmarineLevel0Map[4097];
extern int g_SubmarineLevel0RouteTable[128][8];
extern unsigned char g_SubmarineLevel1Map[4097];
extern int g_SubmarineLevel1RouteTable[128][8];
extern unsigned char g_SubmarineLevel2Map[4097];
extern int g_SubmarineLevel2RouteTable[128][8];
extern unsigned char g_SubmarineLevel3Map[4097];
extern int g_SubmarineLevel3RouteTable[128][8];
extern unsigned char g_SubmarineLevel4Map[4097];
extern int g_SubmarineLevel4RouteTable[128][8];
extern unsigned char g_SubmarineLevel5Map[4097];
extern int g_SubmarineLevel5RouteTable[128][8];

/* sub_player_motion_collision.cpp */
extern short g_SubmarineRecordInterpBaseA;
extern short g_SubmarineRecordInterpBaseB;
extern short g_SubmarineRecordInterpTargetA;
extern short g_SubmarineRecordInterpTargetB;
extern short g_SubmarineRecordInterpDeltaA;
extern short g_SubmarineRecordInterpDeltaB;
extern int g_SubmarineRecordInterpBaseC;
extern int g_SubmarineRecordInterpTargetC;
extern int g_SubmarineRecordInterpDeltaC;
extern int g_SubmarineRecordInterpBaseD;
extern int g_SubmarineRecordInterpTargetD;
extern int g_SubmarineRecordInterpDeltaD;
extern int g_SubmarineRecordInterpFraction;
extern int g_SubmarineRecordInterpStep;
extern int g_SubmarineRecordInterpActive;
void __cdecl Submarine_UpdatePlayerControls(void);
void __cdecl Submarine_ArmRecordInterpolation(int targetIndex);
void __cdecl Submarine_StepRecordInterpolation(void);
void __cdecl Submarine_UpdateMotionAndCollision(void);
void __cdecl Submarine_BuildDirectionVectorFromAngles(struct SubmarineDirectionAngles *angles, struct SubmarineDirectionVector *vector);
int __cdecl Submarine_ProbeGridCellCollision(int probeX, int probeY, int probeZ);
void __cdecl Submarine_LoadStateFromRecord(struct SubmarineStateRecord *stateRecord);
void __cdecl Submarine_BuildRotationBasisFromAngles(struct SubmarineRotationAngles *rotationAngles, struct SubmarineRecordMatrixRow *matrix);

/* sub_record_flight_update.cpp */
extern unsigned char g_SubmarineConnectorColor1[4];
extern int g_SubmarineTorpedoLaunchOffsetFactor;
extern int g_SubmarineTorpedoForwardOffsetFactor;
extern int g_SubmarineTorpedoInitialSpeed;
extern struct GfxRenderObject *g_SubmarineConnectorBatchContext;
extern int g_SubmarineActiveTorpedoCount;
void __cdecl Submarine_UpdateRecordAimAngles(void);
int __cdecl Submarine_ComputeFixedAngleFromComponents(int x, int y);
int __cdecl Submarine_NormalizeVectorFixed(struct SubmarineAimRecord *vector);
void __cdecl Submarine_UpdateRecordOrientation(struct SubmarineFlightRecord *orientation);
void __cdecl Submarine_BuildRecordRotationBasis(struct SubmarineRecordAngles *rotationAngles, struct SubmarineRecordMatrixRow *matrix);
void __cdecl Submarine_ComputeRecordOffsetAndBasisDots(struct SubmarineFlightRecord *record);
void __cdecl Submarine_PushRecordPositionHistory(struct SubmarineFlightRecord *record);
void __cdecl Submarine_UpdateRecordFlightStates(void);
void __cdecl Submarine_EmitRecordConnectorQuads(void);
void __cdecl Submarine_ResetRecordTable(void);
void __cdecl Submarine_LaunchTorpedoRecord(void);
void __cdecl Submarine_DrawRecordQuadPass(struct GfxEngineContext *renderContext);

extern struct SubmarineFlightRecord g_SubmarineFlightRecords[32];
extern struct SubmarineGridCell g_SubmarineGridCellTable[4096];
extern struct SubmarineState g_SubmarineState;
struct BitCountTableEntry;

/* sub_module_lifecycle.cpp */
extern struct BitCountTableEntry g_SubmarineSceneBuffer[252];

struct SubmarineModelDescriptor;

/* sub_record_update_and_draw.cpp: the working level block */
extern struct SubmarineModelDescriptor g_SubmarineModelDescriptors[4];

struct SubmarineLevelBlock;

/* sub_module_lifecycle.cpp */
extern struct BitCountTableEntry g_SubmarineGridBuffer[256];

/* sub_terrain_data2.cpp */
extern struct SubmarineLevelBlock g_SubmarineLevel0Record;
extern struct SubmarineLevelBlock g_SubmarineLevel1Record;
extern struct SubmarineLevelBlock g_SubmarineLevel2Record;
extern struct SubmarineLevelBlock g_SubmarineLevel3Record;
extern struct SubmarineLevelBlock g_SubmarineLevel4Record;
extern struct SubmarineLevelBlock g_SubmarineLevel5Record;

struct SubmarineHudRenderState;

/* sub_hud_draw_and_update.cpp */
extern struct SubmarineHudRenderState g_SubmarineHudRenderState;
/* sub_record_flight_update.cpp */
extern unsigned char g_SubmarineConnectorColor0[4];

extern unsigned char g_SubmarineTerrainMeshBuffer[145920];
#ifdef __cplusplus
}
#endif

#endif /* FF7_API_SUBMARINE_H */

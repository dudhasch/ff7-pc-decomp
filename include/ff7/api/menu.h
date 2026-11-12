/* include/ff7/api/menu.h: the functions and objects ff7/menu defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_MENU_H
#define FF7_API_MENU_H

#ifdef __cplusplus
extern "C" {
#endif
struct Savemap;
struct GfxEngineContext;
struct BattleItemAction;
struct KernelAccessory;
struct KernelWeapon;
struct KernelArmor;
struct KernelMateria;
struct MenuPartyStats;
struct FontWindowHandles;
struct PfileLoadOptions;
struct MenuPopupBodyRect;
struct EnglishWindowFrame;
struct MenuGauge;
struct WindowData;
struct EnglishMessageWindowRect;
struct Battle3dDescriptorFile;
struct MenuInputBindingTable;
struct GfxRenderObject;
struct MenuMateriaSubRects;
struct MenuMateriaWindow;
struct GfxMenuLabelTable;
struct MenuFontBGraphicsObject;

/* battle_target_selection.cpp */
extern short g_BtlTargetFlags;
extern unsigned char g_BattleCommandMenuPhase;
extern unsigned char g_BtlTargetMode;
extern unsigned char g_BtlTargetSelectMode;
extern unsigned char g_BtlSingleTargetIndex;
extern unsigned char g_BtlPointedTarget;
void Battle_LoadTargetMaskForMode(void);
void Battle_AdvanceTargetCursorInGroup(unsigned int mode);
int Battle_FindTargetGroupOfCursor(void);
void __cdecl Battle_BeginTargetSelection(void);
short __cdecl Battle_FindNearestTargetInDirection(short current, short group, short direction);
unsigned int __cdecl Battle_ReadDirectionRepeatMask(void);
void __cdecl Battle_UpdateTargetSelection(void);

/* battle_window_panels.cpp */
extern struct BattleUiBox g_BattleWindowTable2x[];
extern int g_BattleWindowTable1x[];
extern short g_BtlMenuCommandWidths1x[];
extern unsigned char g_BtlMenuLimitListHeights1x[];
extern unsigned char g_BtlMenuSlotRecords[4];
extern unsigned char g_BtlMenuSlotTableA[];
extern unsigned char g_BtlMenuSlotTableB[4];
extern unsigned char g_BtlMenuWindowRecordsC[4];
extern unsigned char g_BtlMenuLimitCursor[];
extern int g_BtlMenuGilCursor[];
extern int *g_pBtlMenuUiTextures;
void __cdecl Battle_UiMenuHookNoOp(void);
void __cdecl Battle_DrawMenuCursorAndDimOverlay2x(int *rows);
void __cdecl Battle_DrawMenuCursorAndDimOverlay1x(int *rows);
void __cdecl Battle_BeginWindowTextDrawNoOp(int what);
void __cdecl Battle_EndWindowTextDrawNoOp(int what);
unsigned short __cdecl Battle_GetWindowState(short index);
void __cdecl Battle_OpenWindow(short x, short y, short id);
void __cdecl Battle_SetWindowStateClosing(short index);
void __cdecl Battle_ResetSelectionCursorPosition(void);
void __cdecl Battle_DrawSelectionCursorQuads2x(void *unused, short posX, short posY);
void __cdecl Battle_DrawSelectionCursorQuads1x(void *unused, short posX, short posY);
void __cdecl Battle_SelectionCursorOverlayNoOp(void *what, short x, short y);
void __cdecl Battle_EraseSelectionCursorNoOp(void *what, short x, short y);
void __cdecl Battle_PositionAndDrawTargetSelectionCursor2x(void *cursorCtx, short actorIndex);
void __cdecl Battle_PositionAndDrawTargetSelectionCursor1x(void *cursorCtx, short actorIndex);
void __cdecl Battle_DrawSelectionCursorForTarget2x(void *cursorCtx, short actorIndex);
void __cdecl Battle_DrawSelectionCursorForTarget1x(void *cursorCtx, short actorIndex);
void __cdecl Battle_DrawTargetCursorWhenMenuStateZero2x(void *what);
void __cdecl Battle_DrawTargetCursorWhenMenuStateZero1x(void *what);
void __cdecl Battle_UiFrameHookNoOp(void);
/* Unreferenced retail helpers below keep their address suffixes as names. */
void __cdecl BtlMenu_ClearPanelShortFlag(void);
int __cdecl Text_GetEncodedStringLength(unsigned char *text);
void __cdecl Menu_DrawBattleWindow(int unused1, short windowIndex, int op, int param, int unused5);
void __cdecl Battle_FormatWindowGilAmountText(int value);
void __cdecl Battle_PartyMemberAvailableHookNoOp(unsigned short actor);
void __cdecl Battle_RequestMessageWindowAt(short x, short y);
void __cdecl Battle_SetMessageWindowRequest(short textId);
void __cdecl Menu_DrawBattleWindowSet(int layer);
void __cdecl Battle_DrawWindowPanelContent2x(int *slot, short arg);
void __cdecl Battle_DrawWindowPanelContent1x(int *slot, short arg);
void __cdecl Menu_FlushAndClipToBattleWindow2x(int *rows, short id);
void __cdecl Menu_FlushAndClipToBattleWindow1x(int *rows, short id);
void __cdecl Menu_DrawBattleWindowSubRectFrames2x(int *rows, short id);
void __cdecl Menu_DrawBattleWindowSubRectFrames1x(int *rows, short id);
void __cdecl Battle_DrawWindowRow2x(int *rows, short);
void __cdecl Battle_DrawWindowRow1x(int *rows, short);
void __cdecl Battle_DrawOpenWindows2x(void);
void __cdecl Battle_DrawPauseCaption2x(void);
void __cdecl Battle_DrawOpenWindows1x(void);
void __cdecl Battle_DrawPauseCaption1x(void);
void __cdecl Battle_ConfirmSelectionAndPostCommand(void);
void __cdecl Battle_PostSavedThenCurrentCommandSelections(void);
void __cdecl Battle_ResetCommandMenuCursors(void);
void __cdecl Battle_WindowFrameControlledEnemyActionList(void);
void __cdecl Battle_WindowUnusedSlot19NoOp(void);
void __cdecl Battle_WindowOpenNoOp(void);
void __cdecl Battle_WindowUnusedNoOp(void);
void __cdecl Battle_WindowFrameNoOp(void);
void __cdecl Battle_WindowOpenSlot12And21NoOp(void);
void __cdecl Battle_WindowFrameSlot12And21NoOp(void);
void __cdecl Battle_WindowUnusedSlot12And21NoOp(void);
void __cdecl Battle_WindowOpenSlot0NoOp(void);
void __cdecl Battle_WindowFrameSlot0OpenCommandMenuForActor(void);
void __cdecl Battle_WindowUnusedSlot0NoOp(void);
void __cdecl Battle_WindowOpenSeedGilAmount(void);
void __cdecl Battle_WindowFrameGilAmountPrompt(void);
void __cdecl Battle_WindowUnusedSlot11And20NoOp(void);
void __cdecl Battle_WindowOpenCommandMenuResizeResetCursor(void);
void __cdecl Battle_WindowFrameCommandMenu(void);
void __cdecl Battle_WindowUnusedSlot1NoOp(void);
void __cdecl Battle_WindowOpenItemPanelResetCursor(void);
void __cdecl Battle_WindowFrameItemPanel(void);
void __cdecl Battle_WindowUnusedSlot5NoOp(void);
void __cdecl Battle_WindowOpenMagicPanelResetCursor(void);
void __cdecl Battle_WindowFrameMagicList(void);
void __cdecl Battle_WindowUnusedSlot6NoOp(void);
void __cdecl Battle_WindowOpenSlot2NoOp(void);
void __cdecl Battle_WindowFrameSlot2ConfirmOrCancel(void);
void __cdecl Battle_WindowUnusedSlot2NoOp(void);
void __cdecl Battle_WindowOpenSlot3NoOp(void);
void __cdecl Battle_WindowFrameSlot3ConfirmOrCancel(void);
void __cdecl Battle_WindowUnusedSlot3NoOp(void);
void __cdecl Battle_WindowOpenSummonListResetCursor(void);
void __cdecl Battle_WindowFrameSummonList(void);
void __cdecl Battle_WindowUnusedSlot7NoOp(void);
void __cdecl Battle_WindowOpenEnemySkillListResetCursor(void);
void __cdecl Battle_WindowFrameEnemySkillList(void);
void __cdecl Battle_WindowUnusedSlot4NoOp(void);
void __cdecl Battle_WindowOpenLimitListResetCursor(void);
void __cdecl Battle_WindowFrameLimitList(void);
void __cdecl Battle_WindowUnusedSlot15And24NoOp(void);
void __cdecl Battle_WindowOpenTifaReelWindow(void);
void __cdecl Battle_WindowFrameTifaReelWindow(void);
void __cdecl Battle_WindowUnusedSlot27NoOp(void);
void __cdecl Battle_WindowOpenCaitReelWindow(void);
void __cdecl Battle_WindowFrameCaitReelWindow(void);
void __cdecl Battle_WindowUnusedSlot26NoOp(void);
void __cdecl Battle_WindowOpenSlotScreen(void);
void __cdecl Battle_WindowFrameSlotScreen(void);
void __cdecl Battle_WindowUnusedSlot28NoOp(void);
void __cdecl Battle_WindowOpenWorriedChoice(void);
void __cdecl Battle_WindowFrameWorriedChoice(void);
void __cdecl Menu_BattleWindowOpenStep(short windowIndex);
void __cdecl Menu_BattleWindowCloseStep(short windowIndex);
void __cdecl Menu_ClearBattleWindowStates(void);
void __cdecl Battle_ProcessCommandMenuInput(void);
void __cdecl Battle_ResetMenuScreenState(void);
void __cdecl Battle_DrawCountdownTimer(int ignored, int posX, int y, unsigned int value);
void __cdecl Battle_FlushMenuBatches2x(void);
void __cdecl Battle_FlushMenuBatches1x(void);
void __cdecl Battle_ResetMenuUiState(void);
void Battle_SelectMenuUiTablesForScalingMode(void);

/* menu_name_entry_screen.cpp */
extern int g_CharRecordTemplates;
extern int g_MenuNameEntrySavedInputBindings;
void __cdecl Menu_DebugHotkeyPoll(void);
void __cdecl Menu_NameEntryInit(void);
void __cdecl Savemap_ResetCharRecordFromTemplate(int slot);
void __cdecl Menu_LoadDefaultNameEntryBuffer(void);
int __cdecl Menu_GetKeyGridByte(int which, int col, int row);
void __cdecl Menu_NameEntryAppendGlyph(unsigned char glyph);
void __cdecl Menu_NameEntryRemoveGlyph(void);
void __cdecl Menu_NameEntryFrame(unsigned int frameCounter);
void __cdecl Menu_NameEntryFinishJoiningCharacter(void);
void __cdecl Menu_NameEntryModuleUpdate(int arg);
void __cdecl InputConfig_BackupAndLoadKbdDefaults(void);
void __cdecl InputConfig_RestoreBackup(void);

/* scripted_input_stream.cpp */
extern short g_InputScriptCounter;
void __cdecl Input_ClearScriptedInputActive(void);
void __cdecl Input_SetScriptedInputStream(int value);
int __cdecl Input_UpdateAndConsume(void);
void __cdecl Input_ClearPopupGateIfIdle(void);
unsigned int __cdecl Input_ReadWithScriptOverride(void);
int __cdecl Input_StepScriptedInputStream(void);
int __cdecl Input_ReadScriptedInputCommand(void);
unsigned char *__cdecl Menu_ShowPopupTextAndAdvance(unsigned char *text);
int __cdecl Input_GetHeldRaw(void);
void __cdecl Input_UnusedStreamNoOp(void);
void __cdecl Input_SnapshotToFieldGlobals(void);
void __cdecl Input_ClearPressed(void);
void __cdecl Input_ClearHeld(void);

/* battle_hud_lists.cpp */
extern unsigned char g_BtlSquareGilText[4];
extern unsigned char g_BtlSquareAfterText[6];
extern unsigned char g_BtlSquareHandicapTexts[];
extern char g_BtlHandicapFlags;
extern int g_BtlSelectedSpellAllCount;
extern int g_BtlSelectedSpellQuadraLinks;
extern short g_BtlListCursorIndex;
extern unsigned char g_BtlTargetGroupIndex;
extern int g_BtlMenuOption;
extern short g_BtlTargetAvailableMask;
extern unsigned char g_BtlEnemyIndex;
extern short g_BattleMenuCursorX;
extern short g_BattleMenuCursorY;
extern unsigned char g_BtlIssuedCommandId;
extern unsigned char g_BtlSelectedCommand;
extern short g_BtlIssuedActionId;
extern unsigned char g_BtlActiveActor;
extern unsigned char g_BtlActivePartyMember;
int __cdecl Menu_GetCharZeroHpRatio(void);
void __cdecl Battle_DrawPartyMemberHpMpGauges2x(short slot);
void __cdecl Battle_DrawPartyMemberHpMpGauges1x(short slot);
void __cdecl Battle_StepPartyHpMpGaugeDisplay(void);
void __cdecl Battle_DrawStatusNameLabel2x(int y, int statusBitIndex);
void __cdecl Battle_DrawStatusNameLabel1x(int y, int statusBitIndex);
void __cdecl Battle_DrawPartyStatusOverview2x(void);
void __cdecl Battle_DrawPartyStatusOverview1x(void);
void __cdecl Battle_DrawActorCommandGrid2x(unsigned char *unused);
void __cdecl Battle_DrawActorCommandGrid1x(unsigned char *unused);
void __cdecl Menu_DrawBattleItemList2x(void);
void __cdecl Menu_DrawItemTypeIcon2x(int screenX, int screenY, int code, int unused, float z);
void __cdecl Menu_DrawBattleItemList1x(void);
void __cdecl Menu_DrawItemTypeIcon1x(int screenX, int screenY, int code, int unused, float z);
void __cdecl Menu_DrawBattleLimitList2x(void);
void __cdecl Menu_DrawBattleLimitList1x(void);
void __cdecl Menu_DrawBattleMagicList2x(void);
void __cdecl Menu_DrawBattleAbilityCostPanel2x(int x, int y, int layout);
void __cdecl Menu_DrawBattleMagicList1x(void);
void __cdecl Menu_DrawBattleAbilityCostPanel1x(int x, int y, int layout);
void __cdecl Menu_DrawBattleSummonList2x(void);
void __cdecl Menu_DrawBattleSummonList1x(void);
void __cdecl Menu_DrawBattleEnemySkillList2x(void);
void __cdecl Menu_DrawBattleEnemySkillList1x(void);
void __cdecl Battle_DrawGilAmountPrompt2x(void);
void __cdecl Battle_DrawGilAmountPrompt1x(void);
void __cdecl Battle_DrawPartyHudRows2x(void);
void __cdecl Battle_DrawPartyHudRows1x(void);
void Battle_DrawControlledEnemyActionList2x(void);
void Battle_DrawControlledEnemyActionList1x(void);
void __cdecl Menu_CaitReelArm(void);
int __cdecl Menu_CaitReelState(void);
void __cdecl Menu_DrawCaitReelPanel2x(void);
void __cdecl Menu_DrawCaitReelPanel1x(void);
void __cdecl Menu_CaitReelStopOnConfirm(void);
void __cdecl Menu_BuildCaitReelResults(void);
void __cdecl Menu_TifaReelArmAndBuildLimitList(void);
int __cdecl Menu_TifaReelState(void);
void __cdecl Menu_DrawTifaReelPanel2x(void);
void __cdecl Menu_DrawTifaReelPanel1x(void);
void __cdecl Menu_TifaReelStopOnConfirm(void);
void __cdecl Battle_BuildLimitComboResults(void);
void __cdecl Mini_SlotResetSubStateAndInputDelay(void);
void __cdecl Mini_SlotLoadReelSymbols(void);
int __cdecl Mini_SlotGetCurrentReelStopFlag(void);
void __cdecl Mini_DrawSlotScreen(void);
void __cdecl Mini_DrawSlotScreen1x(void);
void __cdecl Mini_SlotStopReelOnConfirm(void);
void __cdecl Mini_ApplySlotPenalty(void);
int __cdecl Party_GetLastFilledSlotCharId(void);
int __cdecl Party_GetLastFilledSlotIndex(void);
void __cdecl Mini_SlotPenaltyClearMatchingMateria(int recordIndex, int);
int __cdecl Mini_SlotPenaltyMateriaColorMatches(int kind, int value);
int __cdecl Battle_MeasureWorriedSecondChoice2x(void);
int __cdecl Battle_MeasureWorriedSecondChoice1x(void);
void __cdecl Battle_DrawWorriedChoiceLines2x(void);
void __cdecl Battle_DrawWorriedChoiceLines1x(void);
void __cdecl Menu_DrawCaitReelSymbol2x(int posX, int posY, int atlasX, int atlasY, int paletteSel);
void __cdecl Mini_DrawSlotReelIcon2x(int posX, int posY, int atlasX, int atlasY, int paletteSel);
void __cdecl Battle_ResetHudGaugeAndReelState(void);

/* menu_cursor_input.cpp */
int __cdecl MenuCursor_ReadHeld(unsigned int padMask, unsigned int mouseMask);
int __cdecl MenuCursor_ReadPressed(unsigned int padMask, unsigned int mouseMask);
void __cdecl MenuCursor_ForwardPadPressed(int value);
int __cdecl MenuCursor_ReadRepeat(unsigned int padMask, unsigned int mouseMask);
int __cdecl BtlMenu_GetInputRepeat(int value);
int __cdecl MenuCursor_ReadPressedOrRepeat(unsigned int padMask, unsigned int mouseMask, int repeat);

/* menu_phs_party_screen.cpp */
void __cdecl Menu_DrawPhsEquipmentPanel2x(int x, int y);
void Menu_DrawEquipMateriaSlots2x(int x, int y, unsigned char *socketBytes, int *materiaIds);
void __cdecl Menu_DrawPhsEquipmentPanel1x(int x, int y);
void Menu_DrawEquipMateriaSlots1x(int x, int y, unsigned char *socketBytes, int *materiaIds);
void __cdecl Menu_PhsCommitSwapSelection(void);
void __cdecl Menu_PhsDispatchConfirmAction(void);
void __cdecl Menu_PartyChangeInit(int mode);
int __cdecl Menu_IsPartySizeSatisfied(void);
void __cdecl Menu_PhsScreenFrame(unsigned int frameCounter);
void __cdecl Menu_DrawPhsExpPanel2x(int frameCounter);
void __cdecl Menu_DrawPhsExpPanel1x(int frameCounter);
void __cdecl Menu_PhsModuleUpdate(int mode);
void __cdecl Menu_ClearPartyChangeScreenActiveFlag(void);

/* menu_equip_screen.cpp */
extern unsigned char g_MenuEquipLabelTable[288];
extern int g_MenuEquipCursor0[];
extern int g_MenuEquipCursor1[];
void __cdecl Menu_EquipScreenInit(int arg);
void __cdecl Menu_EquipScreenFrame(unsigned int frameCounter);
void __cdecl Menu_DrawEquipMateriaSlotRow2x(int x, int y, unsigned char *slots);
void __cdecl Menu_DrawEquipMateriaSlotRow1x(int x, int y, unsigned char *slots);
void __cdecl Menu_DrawEquipStatRows(int baseX, int baseY);
void __cdecl Menu_DrawEquipStatRows1x(int baseX, int baseY);
void __cdecl Menu_BuildEquipCandidateList(int equipType);

/* menu_item_sort_screen.cpp */
extern unsigned char g_MenuItemManualText[816];
extern unsigned char g_MenuItemManualNoUseText[40];
extern unsigned char g_MateriaKindById[384];
void __cdecl Menu_ItemArrangeBySortMode(int mode);
short __cdecl Menu_ItemSortQuicksort(unsigned short *base, unsigned int count, short(__cdecl *cmp)(short, short, unsigned short **), void(__cdecl *swap)(short, short, unsigned short **));
void __cdecl Menu_ItemSortSwapRangeBounds(int *first, int *second);
short __cdecl Menu_ItemSortCompareByType(short idx0, short idx1, unsigned short **base);
int __cdecl Menu_ItemSortSign(int val);
short __cdecl Menu_ItemSortCompareByMost(short idx0, short idx1, unsigned short **base);
short __cdecl Menu_ItemSortCompareByLeast(short idx0, short idx1, unsigned short **base);
short __cdecl Menu_CompareItemsByName(short idx0, short idx1, unsigned short **base);
short __cdecl Menu_CompareItemSellability(short idx0, short idx1, unsigned short **base);
short __cdecl Menu_ItemSortCompareByBattle(short idx0, short idx1, unsigned short **base);
short __cdecl Menu_ItemSortCompareByThrow(short idx0, short idx1, unsigned short **base);
void __cdecl Menu_ItemSortSwapSlots(short idx0, short idx1, unsigned short **base);
void __cdecl Menu_SwapU16Values(unsigned short *first, unsigned short *second);
void __cdecl Menu_ItemScreenInit(int arg);
void __cdecl Menu_BuildOwnedKeyItemList(void);
int __cdecl Menu_CharacterLimitMaskMatchesAttackSlots(int charIdx);
int __cdecl Menu_GetItemUseRestrictions(int item);
void __cdecl Menu_ItemScreenFrame(unsigned int frameCounter);
void __cdecl Menu_DrawItemScreenTypeIcon2x(int x, int y, int itemId, int unused, float z);
void __cdecl Menu_DrawItemScreenTypeIcon1x(int x, int y, int itemId, int unused, float z);
int __cdecl Menu_ItemScreenIsPartyMemberHpFull(int slot);
int __cdecl Menu_ItemScreenIsPartyMemberMpFull(int slot);
void __cdecl Menu_CopyPopupTextBuffer(unsigned char *src);
void __cdecl Menu_UnusedItemSortNoOp(void);
void __cdecl Savemap_HarvestAllMateriaToStash(void);
void __cdecl Savemap_StashOneMateria(unsigned int *pMateriaEntry);
void __cdecl Savemap_ReplaceMateriaOfKind(unsigned int materia, int kind);
int __cdecl Savemap_GetLowestMateriaKind(void);
void __cdecl Savemap_CommitMateriaStash(void);
void __cdecl Savemap_EraseMateriaValueAnywhere(unsigned int materiaEntry);
void __cdecl Savemap_ReturnMateriaStash(void);
int __cdecl Savemap_EquipMateriaInAnyFreeSlot(unsigned int materiaEntry);
void __cdecl Savemap_StripCharacterEquipment(int recordIndex);
void __cdecl Savemap_BackupPartyAndEquipToStash(int recordIndex);
void __cdecl Savemap_RestorePartyAndEquipFromStash(int recordIndex);

/* menu_limit_screen.cpp */
extern struct EnglishWindowFrame g_LimitMenuFrames1x[4];
void __cdecl Menu_LimitScreenInit(int arg);
void __cdecl Menu_LimitScreenFrame(unsigned int frameCounter);
void __cdecl Menu_DrawLimitScreenHeader2x(int phase);
void __cdecl Menu_DrawLimitScreenHeader1x(int phase);
void __cdecl Menu_UnusedLimitScreenNoOp(void);
void __cdecl Menu_RebuildPartyLimitRecords(void);

/* menu_save_screen.cpp */
void __cdecl Menu_LoadPartyPortraitsSaveMenuExit(void);
unsigned short __cdecl Menu_WriteSaveSlot(int packed);
void __cdecl Menu_SaveScreenInit(int calledFromField);
void __cdecl Menu_SaveScreenShutdown(void);
void __cdecl Menu_SaveScreenTick(unsigned int frameCounter);
int __cdecl Menu_SaveScreenModuleUpdate(void);

/* menu_magic_screen.cpp */
extern unsigned char g_MenuMagicCursorCoords[];
extern struct EnglishWindowFrame g_MenuMagicWindowRects2x[6];
extern struct EnglishWindowFrame g_MenuMagicWindowRects1x[6];
void __cdecl Menu_MagicScreenInit(void);
void __cdecl Menu_MagicScreenFrame(unsigned int frameCounter);
int __cdecl Menu_MagicScreenIsPartyMemberHpFull(int slot);
void __cdecl Menu_DrawMagicScreenCharacterPanel2x(void);
void __cdecl Menu_DrawMagicScreenCharacterPanel1x(void);

/* menu_materia_screen.cpp */
extern unsigned char g_MenuMateriaLabelTable[];
extern struct MenuMateriaWindow g_MenuMateriaWindowRects2x;
extern struct MenuMateriaWindow g_MenuMateriaWindowRects1x;
extern int g_MenuMateriaState;
extern struct MenuMateriaSubRects g_MenuMateriaSubRects2x;
extern struct MenuMateriaSubRects g_MenuMateriaSubRects1x;
extern unsigned char g_MenuMateriaCursorCoords[];
void __cdecl Menu_MateriaDetailsNoOp(void);
unsigned char *__cdecl Menu_LookupEntryNameByMode(int mode, int id);
void __cdecl Menu_DrawMateriaScreenCommandList2x(void);
void __cdecl Menu_DrawMateriaScreenCommandList1x(void);
void __cdecl Menu_DrawMateriaScreenMagicList2x(unsigned char *attacks);
void __cdecl Menu_DrawMateriaActionCostPanel2x(int baseX, int baseY, int entryIndex, int partyIndex, int mode);
void __cdecl Menu_DrawMateriaScreenMagicList1x(unsigned char *attacks);
void __cdecl Menu_DrawMateriaActionCostPanel1x(int baseX, int baseY, int entryIndex, int partyIndex, int mode);
void __cdecl Menu_DrawMateriaScreenSummonList2x(unsigned char *attacks);
void __cdecl Menu_DrawMateriaScreenSummonList1x(unsigned char *attacks);
void __cdecl Menu_DrawMateriaScreenEnemySkillList2x(unsigned char *attacks);
void __cdecl Menu_DrawMateriaScreenEnemySkillList1x(unsigned char *attacks);
void __cdecl Menu_MateriaScreenInit(int arg);
void __cdecl Menu_DrawMateriaScreenCursors2x(unsigned int flags);
void __cdecl Menu_DrawMateriaScreenCursors1x(unsigned int flags);
void __cdecl Menu_DrawMateriaScreenSelectedMateriaName2x(void);
void __cdecl Menu_DrawMateriaScreenSelectedMateriaName1x(void);
void __cdecl Menu_ApplyAndDrawStockMateriaDetail2x(int cursor, int param);
void __cdecl Menu_DrawMateriaDetailPanel2x(unsigned int materiaWord, int scrollOffset);
void __cdecl Menu_DrawMateriaStatModifierRows2x(void);
int __cdecl Menu_MateriaScreenValueIsNegative(int value);
int __cdecl Menu_MateriaScreenAbsValue(int v);
void __cdecl Menu_ApplyAndDrawStockMateriaDetail1x(int cursor, int param);
void __cdecl Menu_DrawMateriaDetailPanel1x(unsigned int materiaWord, int scrollOffset);
void __cdecl Menu_DrawMateriaStatModifierRows1x(void);
void __cdecl Menu_DrawMateriaScreenStockList2x(int listIndex);
void __cdecl Menu_DrawMateriaScreenStockList1x(int listIndex);
void __cdecl Menu_DrawMateriaScreenDetailPanels2x(int scrollOffset);
void __cdecl Menu_DrawMateriaScreenDetailPanels1x(int scrollOffset);
void __cdecl Menu_MateriaScreenFrame(unsigned int frameCounter);
void __cdecl Menu_MateriaSortStock(int skip);
short __cdecl Menu_MateriaSortIterativeQuicksort(int base, unsigned int count, short(__cdecl *cmp)(short, short, int *), void(__cdecl *swap)(short, short, int *));
void __cdecl Menu_MateriaSortSwapRangeBounds(int *first, int *second);
void __cdecl Menu_MateriaSortCompareStockEntries(unsigned int indexA, unsigned int indexB, int **list);
int __cdecl Menu_MateriaSortCompareKeys(unsigned char param_1, unsigned char param_2, unsigned int param_3, unsigned int param_4);
void __cdecl Menu_MateriaSortSwapStockIndices(int i, int j, void *table);
void __cdecl Menu_MateriaSortSwapStockEntries(int *first, int *second);
void __cdecl Menu_ClearPartySingleTargetAllCounts(void);
void __cdecl Menu_DrawMateriaSlotRow(int x, int y, unsigned char *slotTypes, int *materiaIds);
void __cdecl Menu_DrawMateriaSlotRow1x(int x, int y, unsigned char *slotTypes, int *materiaIds);
void __cdecl Menu_DrawMateriaScreenCharacterPanel2x(void);
void __cdecl Menu_DrawMateriaScreenCharacterPanel1x(void);
void __cdecl Menu_MateriaExchangeInitPartyList(void);
void __cdecl Menu_DrawMateriaExchangeList2x(unsigned int flags);
void __cdecl Menu_DrawMateriaExchangeStockList2x(void);
unsigned int __cdecl Menu_DrawMateriaNameAndApStars2x(int x, int y, unsigned int item);
void __cdecl Menu_DrawMateriaExchangeList1x(unsigned int flags);
void __cdecl Menu_DrawMateriaExchangeStockList1x(void);
unsigned int __cdecl Menu_DrawMateriaNameAndApStars1x(int x, int y, unsigned int item);
void __cdecl Menu_MateriaExchangeHandleInput(void);
void __cdecl Menu_SwapEquippedMateriaWhereBothSlotsExist(unsigned char *liveA, unsigned char *liveB, int *slotsA, int *slotsB);
void __cdecl Menu_ClearMateriaScreenInitializedFlag(void);

/* menu_quit_screen.cpp */
void __cdecl Menu_QuitScreenInit(void);
void __cdecl Menu_UnusedQuitScreenNoOp(void);
void __cdecl Menu_QuitScreenFrame(void);

/* menu_status_screen.cpp */
extern unsigned char g_MenuStatusElementNames[];
extern unsigned char g_MenuStatusLabelTable[27][15];
void __cdecl Menu_StatusScreenInit(void);
void __cdecl Menu_DrawStatusCharBlockPortrait2x(void);
void __cdecl Menu_DrawStatusCharBlockPortrait1x(void);
void __cdecl Menu_ElementAbbrevStubUnused(int posX, int posY, int which);
void __cdecl Menu_DrawElementRowGlyphs(int x, int y, int row);
void __cdecl Menu_StatusScreenFrame(unsigned int frameCounter);
void __cdecl Menu_DrawElementStatusPage2x(int originX, int originY);
void __cdecl Menu_DrawElementStatusPage1x(int originX, int originY);
void __cdecl Menu_DrawStatusEquipmentPanel2x(int baseX, int baseY);
void __cdecl Menu_DrawStatusMateriaSlotRow2x(int x, int y, unsigned char *slotTypes, int *materiaIds);
void __cdecl Menu_DrawStatusEquipmentPanel1x(int baseX, int baseY);
void __cdecl Menu_DrawStatusMateriaSlotRow1x(int x, int y, unsigned char *slotTypes, int *materiaIds);
void __cdecl Menu_DrawStatusStatRows(int x, int y);
void __cdecl Menu_DrawStatusStatRows1x(int originX, int originY);
void __cdecl Menu_DrawStatusExpPanel2x(int frame);
void __cdecl Menu_DrawStatusExpPanel1x(int frame);
void __cdecl Menu_DrawStatusEntryNameGrid2x(int x, int y);
void __cdecl Menu_DrawStatusEntryNameGrid1x(int x, int y);
void __cdecl Menu_UnusedStatusScreenNoOp(void);

/* menu_savemap_materia_queries.cpp */
void __cdecl Menu_StubScreen12Init(int arg);
void __cdecl Menu_StubScreen12Frame(unsigned int frameCounter);
void __cdecl Menu_UnusedMateriaQueryNoOp(void);
int __cdecl Savemap_CountCharEquippedMateria(int charIdx);
int __cdecl Savemap_ReadUnalignedU16(unsigned char *p);
void __cdecl Savemap_WriteUnalignedU16(unsigned char *p, short value);
void __cdecl Savemap_ScaleFirstFiveHp(void);
void __cdecl Savemap_CheckMateriaSetComplete(int setIndex);
int __cdecl Savemap_HasMasteredMateriaId(unsigned int materiaId);
int __cdecl Savemap_HasMateriaId(unsigned int materiaId);
void __cdecl Savemap_ExchangeForMasterMateria(int groupId);
int __cdecl Savemap_RemoveMasteredMateria(unsigned int materiaId);

/* menu_stub_screen6.cpp */
extern unsigned char g_MenuStubScreen6Text[];
extern int g_MenuStubScreen6Cursor0[];
extern int g_MenuStubScreen6Cursor1[];
void __cdecl Menu_StubScreen6Init(int arg);
void __cdecl Menu_StubScreen6Frame(unsigned int frameCounter);
void __cdecl Menu_UnusedStubScreen6NoOp(void);

/* menu_text_primitives.cpp */
void Menu_EmptyTextHook(int what);
void __cdecl Menu_SetMaxGlyphsPerString(int value);
int __cdecl Menu_DrawFontGlyph2x(int penX, int posY, unsigned int colorIndex, unsigned short charCode, float z);
void __cdecl DrawIconGlyph16_2x(int x, int y, unsigned char code, unsigned char colorIndex, float z);
void __cdecl Menu_DrawFixedPitchString2x(int x, int y, unsigned char *text, unsigned char colorIndex, float z);
void __cdecl Menu_DrawResourceGauge2x(struct MenuGauge *gauge, float depth);
void __cdecl Menu_DrawShopPartyTileHighlight2x(int x, int y, int w, int h, int shade);
void __cdecl Menu_DrawEtchedBox2x(struct EnglishWindowFrame *rect, float depth);
void __cdecl Menu_DrawRaisedBox2x(short *rect, float z);
void __cdecl Menu_DrawScrollBar2x(short *desc, float z);
void __cdecl Menu_DrawTifaReelFadeBands2x(int x, int y, int width, int height);
void __cdecl Menu_DrawAtlasQuad2x(int screenX, int screenY, int srcU, int srcV, int width, int height, unsigned int attrByte, int useAltAtlas, int unusedArg, float z);
void __cdecl Menu_DrawDecimalNumber2x(int x, int y, unsigned int value, char digitCount, char colorIndex, float z);
void __cdecl Menu_DrawDecimalNumberZeroPad2x(int x, int y, unsigned int value, unsigned int digitCount, unsigned char colorIndex, float z);
void __cdecl Menu_FlushAndSetScissor2x(int x, int y, int w, int h);
void __cdecl Menu_FlushAndResetScissor2x(void);
int __cdecl Menu_DrawFontGlyph1x(int penX, int posY, unsigned int colorIndex, unsigned short charCode, float z);
void __cdecl DrawIconGlyph16_1x(int x, int y, unsigned char code, unsigned char colorIndex, float z);
void __cdecl Menu_DrawFixedPitchString1x(int x, int y, unsigned char *text, unsigned char colorIndex, float z);
void __cdecl Menu_DrawResourceGauge1x(struct MenuGauge *gauge, float depth);
void __cdecl Menu_DrawShopPartyTileHighlight1x(int x, int y, int width, int height, unsigned int level);
void __cdecl Menu_DrawEtchedBox1x(struct EnglishWindowFrame *rect, float depth);
void __cdecl Menu_DrawRaisedBox1x(short *rect, float z);
void __cdecl Menu_DrawScrollBar1x(short *desc, float z);
void __cdecl Menu_DrawTifaReelFadeBands1x(int originX, int originY, int width, int height);
void __cdecl Menu_DrawAtlasQuad1x(int posX, int posY, int srcX, int srcY, int width, int height, unsigned int alpha, int useBlend1, int extra, float depth);
void Menu_DrawDecimalNumber1x(int originX, int originY, unsigned int value, char digits, char palette, float depth);
void __cdecl Menu_DrawDecimalNumberZeroPad1x(int x, int y, unsigned int value, unsigned int digitCount, unsigned char colorIndex, float z);
void __cdecl Menu_FlushAndSetScissor1x(int x, int y, int w, int h);
void __cdecl Menu_FlushAndResetScissor1x(void);
void __cdecl Menu_EmitFontBatchLists(struct GfxEngineContext *ctx);

/* message_window_draw.cpp */
extern short g_BattleMenuReturnState;
extern short g_BattleWindowFrameId;
extern short g_BattleMenuState;
extern unsigned char g_MenuCornerColorTL[4];
extern unsigned char g_MenuCornerColorTR[4];
extern unsigned char g_MenuCornerColorBL[4];
extern unsigned char g_MenuCornerColorBR[4];
extern unsigned char g_FieldWindowColours[12];
extern int g_InputPressedSnapshot;
extern int g_InputHeldSnapshot;
extern unsigned char g_MenuNonTopLevelScreenFlag;
void __cdecl Menu_PackWindowColor(unsigned char *dest, unsigned char r, unsigned char g, unsigned char b);
void __cdecl BtlMenu_UnusedMessageDrawNoOpA(void);
void __cdecl Menu_DrawCharPortrait2x(int x, int y, int slot, float z);
void __cdecl BtlMenu_UnusedMessageDrawNoOpB(void);
void __cdecl Menu_DrawCharPortraitById2x(int originX, int originY, int width, int height, int charId, float depth);
short __cdecl Menu_DrawMessageText2x(short penX, short penY, short wrapWidth, unsigned char *text, float z);
void __cdecl Menu_DrawHorizontalRule2x(int x0, int x1, int y, int colour);
void __cdecl Menu_SaveWindowColors(void);
void __cdecl Menu_RestoreWindowColors(void);
void __cdecl Menu_SetWindowColors(int *colours);
void __cdecl Menu_FillRectShorts2x(short *rect, unsigned int colour, float z);
void __cdecl Menu_DrawMessageWindowFrame2x(struct EnglishMessageWindowRect *rect, float depth);
void __cdecl Menu_DrawCountdownTimer2x(int posX, int posY, unsigned int countdownValue);
void __cdecl Menu_DrawMessageWindowNumber2x(int posX, int posY, int value, int digitCount, int flags);
int __cdecl Menu_RenderMessageWindows(struct WindowData *windowArray, short windowCount, int renderPhase);
void __cdecl Menu_FlushUiBatches(void);
void __cdecl Menu_DrawCharPortrait1x(int x, int y, int slot, float z);
void __cdecl Menu_DrawCharPortraitById1x(int posX, int posY, int width, int height, int charId, float depth);
short __cdecl Menu_DrawMessageText1x(short penX, short penY, short wrapWidth, unsigned char *text, float z);
void __cdecl Menu_DrawHorizontalRule1x(int x0, int x1, int y, int colour);
void __cdecl Menu_FillRectShorts1x(short *rect, unsigned int colour, float z);
void __cdecl Menu_DrawMessageWindowFrame1x(struct EnglishMessageWindowRect *windowRect, float depth);
void __cdecl Menu_DrawCountdownTimer1x(int posX, int posY, unsigned int timerValue);
void __cdecl Menu_DrawMessageWindowNumber1x(int x, int y, int value, int digitCount, int flags);
void __cdecl Menu_ApplyUiScalingViewportForMode(int mode);
void __cdecl Menu_SetMessageWindowYOffset(int value);
void __cdecl Menu_DrawPopupMessageBody2x(struct EnglishWindowFrame *rect, int depth);
void __cdecl Menu_DrawPopupMessageBody1x(struct MenuPopupBodyRect *rect, int depth);
void __cdecl Menu_DrawTranslucentBlackRect(int param_1, int param_2, int param_3, int param_4);
void __cdecl Menu_SetDigitBackdropQuad(int arg1, char arg2);
void __cdecl Menu_SetMessageWindowBlackQuadEnabled(int value);

/* loadmenu.cpp */
extern void * g_LoadMenuPreviewLayout;
extern int g_LoadMenuSlotState;
unsigned short __cdecl Menu_RestoreSaveSlotToSavemap(int slotIndex);
int __cdecl Menu_CacheSaveSlotPreview(int unused, int slot);
void __cdecl Menu_DrawSaveSlotPreview2x(int i, int top, int slotIndex);
void __cdecl Menu_DrawSaveSlotPreview1x(int i, int top, int slotIndex);

/* menu_shop_screen.cpp */
extern int g_LoadMenuInputDelay;
int __cdecl Savemap_CountMateriaInStock(int materiaId);
void __cdecl Menu_ShopSelectShop(int index);
void __cdecl Menu_DrawShopTitle2x(void);
void __cdecl Menu_DrawShopTitle1x(void);
void __cdecl Menu_DrawShopEquipComparisonPanel2x(unsigned int frameCounter);
void __cdecl Menu_DrawShopEquipComparisonPanel1x(unsigned int frameCounter);
void __cdecl Menu_ShopScreenInit(void);
int __cdecl Menu_ShopGetMaxPurchasableQuantity(void);
int __cdecl Menu_ShopGetSelectedEntryOwnedCount(void);
int __cdecl Menu_ShopCountSelectedEntryEquipped(void);
int __cdecl Menu_ShopHasFreeMateriaStockSlot(void);
int __cdecl Menu_ShopScreenFrame(unsigned int frameCounter);
void __cdecl Menu_DrawShopItemTypeIcon2x(int x, int y, int itemId, int unused, float z);
void __cdecl Menu_DrawShopItemTypeIcon1x(int x, int y, int itemId, int unused, float z);
void __cdecl Menu_DrawShopMateriaDetailPanel2x(unsigned int materiaEntry, int frameCounter);
void __cdecl Menu_DrawShopMateriaStatModifierRows2x(void);
int __cdecl Menu_ShopValueIsNegative(int value);
int __cdecl Menu_ShopAbsValue(int v);
void __cdecl Menu_DrawShopMateriaDetailPanel1x(unsigned int materiaEntry, int frameCounter);
void __cdecl Menu_DrawShopMateriaStatModifierRows1x(void);
void __cdecl Menu_DrawShopMateriaSlotLayout2x(int x, int y, unsigned char *slots);
void __cdecl Menu_DrawShopMateriaSlotLayout1x(int x, int y, unsigned char *slots);
int __cdecl Menu_ShopGetMateriaSellValue(unsigned int packed);
int __cdecl Menu_ShopGetMateriaBasePrice(int packed);
int __cdecl Menu_ShopCountMateriaEquipped(unsigned int materiaId);
int __cdecl Menu_ShopCountGearEquipped(int gearId);
void __cdecl Menu_ShopReleaseCharacterPortraits(void);
void __cdecl Menu_ShopModuleUpdate(int shopId);

/* text_char_tables.cpp */
void __cdecl Text_BuildCharCodeTable(void);
void __cdecl Text_BuildCharCodeReverseTable(void);
void __cdecl Text_RebuildAsciiEncodeTables(void);
void __cdecl Text_EncodeAsciiWithReverseTable(unsigned char *dst, char *src);
void __cdecl Text_EncodeAsciiStringRebuildingTables(unsigned char *dst, char *src);

/* callback.cpp */
extern struct GfxRenderObject *g_BtlMenuRenderObjColoc;
extern struct GfxRenderObject *g_BtlMenuRenderObjKetcy2b;
extern struct GfxRenderObject *g_BtlMenuRenderObjKetcy2;
extern struct GfxRenderObject *g_BtlMenuRenderObjColob;
extern unsigned char g_BtlMenuDoubleBufferedCursorRecord[];
extern struct GfxRenderObject *g_BtlMenuRenderObjColo;
extern struct GfxRenderObject *g_BtlMenuRenderObjZeni;
extern struct GfxRenderObject *g_BtlMenuRenderObjColoa;
extern struct GfxRenderObject *g_BtlMenuRenderObjKetcy2a;
extern unsigned short g_BtlMenuSavedMask;
extern int g_BtlMenuFrameCounter;
extern int g_BtlMenuTextureAuxFieldA;
extern int g_BtlMenuTextureAuxFieldB;
void __cdecl Battle_SetUiModeByte(int value);
void __cdecl Battle_DebugDumpSceneNoOp(int width, int height, int value, char r, char g, char b);
void __cdecl Battle_UnusedNoOp_2(void);
void __cdecl Battle_UnusedNoOp_3(void);
void __cdecl BtlMenu_ClearUiDwordQueue(void);
void __cdecl BtlMenu_AppendUiDwordQueue(int value);
void __cdecl Battle_UnusedNoOp_4(void);
int __cdecl Battle3d_SwapOrderingTable(void);
void __cdecl Battle_Unpause(void);
void __cdecl Battle_DispatchSoundCommandSameParams(int command);
void __cdecl Battle_UpdateUiAndLogic(void);
void __cdecl Battle_LoadUiTextures(void);
void __cdecl Battle_FreeUiTextures(void);
void __cdecl Battle_DrawUiLists(void);
void __cdecl Menu_ApplyUiScalingViewport(void);
void __cdecl Battle_LoadFontAndWindowTexturesInto(struct FontWindowHandles *handles, struct PfileLoadOptions *desc, int lgp);

/* loadmenu.cpp */
extern unsigned char g_SaveLoadTextRows[37][36];
extern unsigned char g_SaveLoadDialogRows[24][48];
/* Named rows of g_SaveLoadTextRows (37 FF7-text rows of 36 bytes; rows 13..27 are "01".."15")
 * and g_SaveLoadDialogRows (24 FF7-text rows of 48 bytes). */
#define g_SaveLoadTextSelectDataFile (g_SaveLoadTextRows[1]) /* "Select a save data file." */
#define g_SaveLoadTextSelectGame (g_SaveLoadTextRows[2]) /* "Select a save game." */
#define g_SaveLoadTextLoading (g_SaveLoadTextRows[6]) /* "Loading. Please wait." */
#define g_SaveLoadTextSaving (g_SaveLoadTextRows[7]) /* "Saving. Please wait." */
#define g_SaveLoadTextEmpty (g_SaveLoadTextRows[8]) /* "EMPTY" */
#define g_SaveLoadTextGame (g_SaveLoadTextRows[9]) /* "GAME" */
#define g_SaveLoadTextContinue (g_SaveLoadTextRows[10]) /* "Continue?" */
#define g_SaveLoadTextOf15 (g_SaveLoadTextRows[11]) /* "/15" */
#define g_SaveLoadTextChecking (g_SaveLoadTextRows[12]) /* "Checking Save Data File." */
#define g_SaveLoadTextSaved (g_SaveLoadTextRows[28]) /* "Saved." */
#define g_SaveLoadTextDataRuined (g_SaveLoadTextRows[31]) /* "Data is ruined." */
#define g_SaveLoadTextNewGame (g_SaveLoadTextRows[32]) /* "NEW GAME" */
#define g_SaveLoadTextConfirmSave (g_SaveLoadTextRows[33]) /* "Are you sure you want to save?" */
#define g_SaveLoadTextYes (g_SaveLoadTextRows[34]) /* "Yes" */
#define g_SaveLoadTextNo (g_SaveLoadTextRows[35]) /* "No" */
#define g_SaveLoadTextNoSaveFile (g_SaveLoadDialogRows[7]) /* "No save data file." */
#define g_SaveLoadTextNoSaveFile2 (g_SaveLoadDialogRows[8]) /* "No save data file." */
#define g_SaveLoadTextNotEnoughSpace (g_SaveLoadDialogRows[11]) /* "Not enough space left on save data file." */
#define g_SaveLoadTextCouldNotRead (g_SaveLoadDialogRows[16]) /* "Couldn't read it." */
#define g_SaveLoadTextAllocFailed (g_SaveLoadDialogRows[22]) /* "Can not allocate save file memory buffer!" */
#define g_SaveLoadTextInvalidSaveFile (g_SaveLoadDialogRows[23]) /* "Invalid save file!" */
extern unsigned char g_LoadMenuSaveFileNames[10][12];
extern unsigned char g_LoadMenuWindowRects2x[24];
extern unsigned char g_LoadMenuWindowRects1x[24];
extern struct EnglishWindowFrame g_LoadMenuTitleFrame2x;
extern unsigned char g_LoadMenuLevelLabel[6];
extern unsigned int g_LoadMenuWindowColors[4];
extern struct SaveSlotPreviewInfo g_LoadMenuSlotPreviews[15];
extern int g_LoadMenuSubState;
extern struct Savemap *gSaveFileBuffer;
extern int g_LoadMenuErrorCode;
extern int g_LoadMenuResult;
void __cdecl Menu_SaveFileBufferFree(void);
int __cdecl Menu_SaveFileIsNewlyCreated(int slot);
int __cdecl Menu_ScanSaveDirectory(int markNew);
int __cdecl Menu_LoadSaveFileToBuffer(int mode, int slot);
int __cdecl Menu_LoadScreenTick(void);
void __cdecl Menu_LoadScreenNoOpHook(void);
void __cdecl Menu_LoadScreenShutdown(void);
int __cdecl Menu_LoadScreenModuleUpdate(void);
void __cdecl Menu_BuildLoadPreviewQuad2x(void);
void __cdecl Menu_BuildLoadPreviewQuad1x(void);
void __cdecl Menu_PickSavePreviewFrameGfx(void);

/* menu_input_config.cpp */
extern char *g_MenuKeyNamePointers[245];
extern int g_MenuKeyConfigSlotPos[2];
extern int g_MenuKeyConfigSlotMasks[42];
extern int g_MenuKeyConfigJoystickSlotMasks[40];
extern struct EnglishWindowFrame g_MenuKeyConfigWindowRects2x[2];
extern struct EnglishWindowFrame g_MenuKeyConfigWindowRects1x[2];
extern char g_MenuKeyNamesBuffer[];
extern char g_MenuKeyNameNone[];
extern struct MenuCursor g_MenuKeyConfigCursor;
extern struct MenuInputBindingTable g_MenuKeyConfigSavedInputBuffer;
extern int g_MenuKeyConfigEditHeaderWord;
extern int g_MenuKeyConfigEditBindingsRow0[25];
extern int g_MenuKeyConfigEditBindingsRow1;
#define g_MenuKeyConfigEditBindingsRow1Words (&g_MenuKeyConfigEditBindingsRow1)
extern int g_MenuKeyConfigEditState;
extern int g_MenuKeyConfigFlashTimer;
extern int g_MenuKeyConfigWaitTimer;
void __cdecl InputConfig_AsciiToFF7Text(char *dst, char *src);
void __cdecl InputConfig_BeginEdit(void);
void __cdecl InputConfig_CommitEdit(void);
int __cdecl InputConfig_CaptureSource(int source);
void __cdecl InputConfig_ScreenEnter(void);
void __cdecl InputConfig_ScreenFrame(int *exitFlag);
void __cdecl InputConfig_RevertToSaved(void);
void __cdecl InputConfig_SaveToFile(void);
void __cdecl InputConfig_LoadFromFile(void);
void __cdecl InputConfig_SyncCustomFlag(void);

/* menu_config_screen.cpp */
extern unsigned char g_MenuSoundVolumeHeaderText[56];
void __cdecl Menu_ConfigScreenInit(void);
void __cdecl Menu_ConfigScreenFrame(int flags);
void __cdecl MenuConfig_DrawRows2x(void);
void __cdecl MenuConfig_DrawRows1x(void);
void __cdecl MenuConfig_SaveVolumeAndInputConfig(void);
void __cdecl MenuConfig_RunWindowColorEditor(unsigned int frameCounter);
void __cdecl Engine_ApplyMusicAndSfxVolume(int musicVolume, int sfxVolume);

/* menu_render_objects_portraits.cpp */
void __cdecl Menu_CreateLoadScreenObjects(void);
void __cdecl Menu_UnusedDestroyAllRenderObjects(void);
void __cdecl Menu_ReleaseAllRenderObjects(void);
void __cdecl Menu_LoadFontAndWindowTextures(int hiRes);
void __cdecl Menu_LoadCharPortrait(int charId, int destSlot);
void __cdecl Menu_LoadAllCharPortraits(int loadMode);
void __cdecl Menu_LoadBusterTim(void);
void __cdecl Menu_ClearUiTextureHandles(void);
void __cdecl Menu_FreeAllCharPortraits(void);

/* menu_main_and_battle_results.cpp */
extern int g_MenuColorFormatMask;
extern void (__cdecl *g_MenuScreenInitCallbacks[22])(int arg);
extern int g_MenuScreenFrameCallbacks[22];
extern struct BattleResultScreenLayout g_MenuStatusPanelLayout;
extern int g_ScriptedInputActive;
extern unsigned char g_BattleIsPaused;
extern unsigned char g_GamePauseLatch;
extern int g_MenuField;
extern int g_MenuDialogMessageBuffer;
extern float g_MenuDepth;
extern int g_MenuQuitConfirmCursor;
extern int g_MenuSavedViewportLeft;
extern int g_MenuSavedViewportTop;
extern int g_MenuSavedViewportWidth;
extern int g_MenuSavedViewportHeight;
extern int g_MenuQuitConfirmDone;
extern int g_MenuQuitConfirmOpen;
extern struct GfxRenderObject *g_pMenuDialogBatchList;
extern struct GfxRenderObject *g_MenuWinABlend4GraphicsObject;
extern struct GfxRenderObject *g_MenuWinBBlend4GraphicsObject;
extern struct GfxRenderObject *g_MenuWinCBlend4GraphicsObject;
extern struct GfxRenderObject *g_MenuWinDBlend4GraphicsObject;
extern struct GfxRenderObject *g_MenuWinCBlend4DiffGraphicsObject;
extern struct GfxRenderObject *g_MenuWinABlend0GraphicsObject;
extern struct GfxRenderObject *g_MenuWinABlend1GraphicsObject;
extern struct GfxRenderObject *g_MenuWinBBlend1GraphicsObject;
extern struct GfxRenderObject *g_MenuWinCBlend1GraphicsObject;
extern struct GfxRenderObject *g_MenuWinDBlend1GraphicsObject;
extern struct GfxRenderObject *g_MenuWindowBgGraphicsObject;
extern struct GfxRenderObject *g_MenuBlendWindowBgGraphicsObject;
extern struct GfxRenderObject *g_MenuGaugeBarGraphicsObject;
extern struct GfxRenderObject *g_MenuFlatRectGraphicsObject;
extern struct GfxRenderObject *g_MenuBandGraphicsObject;
extern struct GfxRenderObject *g_MenuBattleLimitGraphicsObject;
extern struct GfxRenderObject *g_MenuTextBoxQuadGraphicsObject;
extern struct GfxRenderObject *g_MenuFontAGraphicsObject;
extern struct MenuFontBGraphicsObject g_MenuFontBGraphicsObject;
extern struct GfxRenderObject *g_MenuCharPortraitGraphicsObjects[9];
extern struct GfxRenderObject *g_MenuBusterTexGraphicsObject;
extern struct GfxRenderObject *g_MenuFontBlend4GraphicsObject;
extern struct GfxRenderObject *g_MenuWinBlend4GraphicsObject;
extern struct GfxRenderObject *g_MenuWinBlend0GraphicsObject;
extern struct GfxRenderObject *g_MenuWinBlend1GraphicsObject;
extern int g_MenuViewportScale;
extern int g_MenuViewportX;
extern int g_MenuViewportY;
extern int g_MenuViewportWidth;
extern int g_MenuViewportHeight;
extern int g_MenuTifaReelFadeActive;
extern int g_MenuTifaReelResetWord;
extern int g_MenuFontWindowLoadDir;
extern int g_MenuPortraitLoadDir;
extern int g_MenuAllPortraitsLoadDir;
extern int g_MenuBusterLoadDir;
extern int g_MenuConfigVolumeCursor;
extern int g_MenuConfigVolumeCursorRow;
extern struct EnglishWindowFrame *g_MenuConfigFrames;
extern unsigned char g_MenuConfigWindowColors[0x10];
extern int g_MenuConfigClearedWord;
extern int g_MenuWindowColorColumn;
extern int g_MenuConfigEditorMode;
extern int g_MenuMusicVolume;
extern int g_MenuWindowColorRow;
extern int g_MenuSfxVolume;
extern int g_MenuColorEditorCursor;
extern int g_MenuConfigCursor;
extern int g_MenuMidiVolumeControlFlag;
extern int g_MenuConfigEditorActive;
extern char g_MenuCursorPartySlot;
extern struct Timestamp64 g_MenuPrevFrameTimestamp;
extern unsigned short g_MenuLockedMask;
extern unsigned short g_SaveSlotPresentMask;
extern int g_MenuTopLevelFrameCount;
extern struct Timestamp64 g_MenuFrameTimestamp;
extern int g_MenuModuleId;
extern short g_MenuResultFadeStep;
extern int g_MenuCommandWindowHeight;
extern int g_MenuPartyMemberIndex;
extern int g_MenuMateriaVisible;
extern char g_BattleResultGilItemsFlag;
extern int g_MenuModuleActive;
extern int g_MenuTitleNewGameChosen;
extern int g_MenuOverlayActive;
extern int g_MenuTopLevelActive;
extern int g_MenuExitRequested;
extern int g_MenuIsSmallViewport320x240;
extern int g_MenuOrderSwapPicked;
extern unsigned char g_MenuMainCursorIndex;
extern int g_MenuMainScreenInitialized;
int __cdecl Menu_IsPopupActive(void);
void __cdecl Menu_SetPopupPlacement(int centered, int x, int y);
void __cdecl Menu_DrawPopupMessage2x(void);
void __cdecl Menu_DrawPopupMessage1x(void);
int __cdecl Kernel_GetEquipRestrictions(int itemId);
int __cdecl Kernel_GetEquippedAttackOrDefense(int partyIndex, int kind);
void __cdecl Savemap_AddEquipStatBonus(int slot, int which, unsigned char amount);
void __cdecl Savemap_AddElementDefense(int slot, int which, short bits);
void __cdecl Savemap_AddWeaponAttackElement(int charIndex, int elementMask);
void __cdecl Savemap_AddStatusAttackBit(int charIndex, int bit);
void __cdecl Savemap_AddStatusDefendBit(int charIndex, int bit);
void __cdecl Savemap_AddStatusDefendMask(int charIndex, int mask);
void __cdecl Savemap_BuildPartyStatsFromRecord(int partyIndex);
void __cdecl Menu_DrawCharStatBlock2x(int originX, int originY, int partySlot, float depth);
void __cdecl Menu_DrawCharStatBlock1x(int originX, int originY, int partySlot, float depth);
void __cdecl Menu_SetScreen(int value);
unsigned char *__cdecl Kernel_LookupMateriaNameText(int value);
unsigned char *__cdecl Kernel_LookupItemNameText(int value);
void __cdecl Battle_DrawResultGilAndItemsScreen2x(void);
void __cdecl Battle_DrawResultGilAndItemsScreen1x(void);
void __cdecl Battle_DrawResultExpScreen2x(void);
void __cdecl Battle_DrawResultLevelUpBox2x(int originX, int originY);
void __cdecl Battle_DrawResultMateriaLevelUpBox2x(int x, int y, int materia);
void __cdecl Battle_DrawResultNewMateriaBox2x(int x, int y);
void __cdecl Battle_DrawResultLimitPanel2x(int x, int y, int slot, int level);
void __cdecl Battle_DrawResultExpScreen1x(void);
void __cdecl Battle_DrawResultLevelUpBox1x(int originX, int originY);
void __cdecl Battle_DrawResultMateriaLevelUpBox1x(int x, int y, int materia);
void __cdecl Battle_DrawResultNewMateriaBox1x(int x, int y);
void __cdecl Battle_DrawResultLimitPanel1x(int x, int y, int slot, int level);
void __cdecl Battle_DrawResultScreenFrame(void);
void __cdecl Battle_InitResultScreenState(void);
void __cdecl Battle_AddRewardItemsToInventoryThunk(void);
void __cdecl Menu_SetPhase(int phase, int prev);
void __cdecl Menu_DrawCommandWindow2x(int mode);
void __cdecl Menu_DrawCommandWindow1x(int moduleId);
void __cdecl Menu_MainScreenInit(int mode);
int __cdecl Menu_PlaytimeToHoursDigits(int playTime);
int __cdecl Menu_PlaytimeToMinutesDigits(int tenths);
int __cdecl Menu_PlaytimeToSecondsDigits(int tenths);
void __cdecl Menu_BeginClosingTopLevelMenu(void);
void __cdecl Menu_DrawMainScreen(int flags);
int __cdecl Menu_RunTopLevelFrame(int scriptStream);
int __cdecl Savemap_GetCloudLevel(int unused);
int __cdecl Kernel_GetArmorRecord(int id);
int __cdecl Kernel_GetAccessoryRecord(int id);
struct MenuPartyStats *__cdecl Savemap_GetPartyStatBlock(int slot);
int __cdecl Savemap_GetCharacterName(int charId);
void __cdecl Savemap_DamageHp(int slot, int amount);
void __cdecl Savemap_HealHp(int slot, int amount);
void __cdecl Savemap_DamageMp(int slot, int amount);
void __cdecl Savemap_AddGil(int amount);
int __cdecl Savemap_GetGil(void);
int __cdecl Menu_GetMateriaTypeColorIndex(int materiaId);
void __cdecl Menu_RefreshPartyLimitTables(void);
void __cdecl Menu_NameEntryMenuEntryThunk(int value);
void __cdecl Menu_PhsMenuEntryThunk(int value);
void __cdecl Menu_ShopMenuEntryThunk(int value);
int __cdecl Menu_LoadMenuEntryThunk(void);
void __cdecl Menu_SaveMenuEntryThunk(void);
void __cdecl Script_SvcHarvestMateria(void);
void __cdecl Script_SvcReturnMateria(void);
void __cdecl Script_SvcStripCharacter(int value);
void __cdecl Script_SvcRestorePartyAndEquip(int value);
void __cdecl Script_SvcBackupPartyAndEquip(int value);
void __cdecl Script_SvcUnusedNoOp(void);
void __cdecl Savemap_SnapshotLevelsToBank(void);
void __cdecl Script_SvcScaleFirstFiveHp(void);
void __cdecl Script_SvcCheckMateriaSet(int value);
void __cdecl Script_SvcReplaceMateriaGroup(int value);
unsigned short __cdecl Savemap_FindItem(unsigned short itemRef);
void __cdecl Menu_PlayMateriaEquipSfx(void);
int __cdecl Savemap_AddMateria(int materia);
void __cdecl Menu_QueueAcquiredMateriaId(int materiaId);
int __cdecl Savemap_RemoveMateriaReturnZero(int materia, int count);
int __cdecl Savemap_CountMateriaReturnZero(int materia, int count);
void __cdecl Savemap_ReloadActivePartyStats(void);
void __cdecl Savemap_ResetConfigDefaults(void);
void __cdecl Party_TickAccessoryRegen(void);
void __cdecl Menu_ReleaseBatchLists(void);
void __cdecl Menu_ModuleMouse(void);
void __cdecl Menu_ModuleKeyboard(void);
void __cdecl Menu_ModuleFrame(struct GfxEngineContext *ctx);
void __cdecl Menu_SubmitDrawLists(void);
void __cdecl Menu_DebugAsciiToFF7Text(char *dst, char *src);
void __cdecl Menu_DrawDebugCounters(void);
void __cdecl Menu_LoadPartyPortraits(void);
void __cdecl Menu_FreePartyPortraits(void);
void __cdecl Menu_ResetUiBatches(void);
void __cdecl Menu_ModuleEnter(struct GfxEngineContext *ctx);
void __cdecl Menu_ModuleExit(void);
void __cdecl Menu_FillViewportQuad(unsigned char alpha, float depth);
void __cdecl Menu_EmitBatchListsAndInvalidate(void);
void __cdecl Menu_ThrottleFrameRate(struct GfxEngineContext *context);
void __cdecl Menu_DispatchScreenUpdate(void);
int __cdecl Menu_IsTextSubsystemReady(void);
void __cdecl Menu_SetTextSubsystemReady(int value);
void __cdecl Menu_SetRequestedScreen(int value);
int __cdecl Menu_GetRequestedScreen(void);

/* menu_kernel_text_pool.cpp */
extern const int g_BattleCameraFileAddressBase;
void __cdecl Kernel_ResetTextPool(void);
char *__cdecl Kernel_AllocTextBank(int size);
char *__cdecl Kernel_GetBattleMessage(int value);
char *__cdecl Kernel_GetTextBankEntry(int bank, int entry, int flags);
char *__cdecl Kernel_GetBattleMessageExpanded(int messageId);
char *__cdecl Kernel_ExpandTextBackrefs(char *text, char *source);
char *__cdecl Kernel_LookupUiText(unsigned int group, int id, int slot);
char *__cdecl Kernel_AppendEncodedString(char *out, char *text, int limit);
void __cdecl Kernel_AppendTextBankEntry(int bank, int entry, char *out);
char *__cdecl Kernel_AppendCharacterName(int charId, char *out);

/* menu_quit_confirm_dialog.cpp */
extern float g_MenuQuitBackdropTriRecord_01[9];
extern float g_MenuQuitBackdropTriRecord_02[9];
extern float g_MenuQuitBackdropTriRecord_03[9];
extern float g_MenuQuitBackdropTriRecord_04[9];
extern float g_MenuQuitBackdropTriRecord_05[9];
extern float g_MenuQuitBackdropTriRecord_06[9];
extern float g_MenuQuitBackdropTriRecord_07[9];
extern float g_MenuQuitBackdropTriRecord_08[9];
extern float g_MenuQuitBackdropTriRecord_09[9];
extern float g_MenuQuitBackdropTriRecord_10[9];
extern float g_MenuQuitBackdropTriRecord_11[9];
extern float g_MenuQuitBackdropTriRecord_12[9];
extern float g_MenuQuitBackdropTriRecord_13[9];
extern float g_MenuQuitBackdropTriRecord_14[9];
void __cdecl Menu_ScaleAndOffsetThreeVectors(float *out, float *srcVecs, float *offsetVec);
void __cdecl Menu_DrawQuitConfirmBackdropTriangles(void);
void __cdecl Menu_DialogApplyUiScalingViewport(void);
void __cdecl Menu_QuitConfirmDialogFrame(void);
void __cdecl Menu_SaveViewportRect(struct GfxEngineContext *engineContext);
void __cdecl Menu_RestoreViewportRect(struct GfxEngineContext *engineContext);
void __cdecl Menu_DialogShowCenteredMessage(int message);
void __cdecl Menu_DialogDrawConfirmBoxThunk(void);
void __cdecl Menu_DialogSubmitDrawLists(void);
void __cdecl Menu_DialogCreateTextBatchList(void);
void __cdecl Menu_DialogReleaseTextBatchList(void);
void __cdecl Menu_DialogInitSession(void);
void __cdecl Menu_QuitConfirmDialogRun(struct GfxEngineContext *engineContext);

/* menu_string_copy_savemap.cpp */
extern int g_SavemapRecordsTable;
void __cdecl Menu_CopyEncodedTextLimited(int dst, int src);
void __cdecl Savemap_BuildPreviewHeader(void);
void __cdecl Menu_SetEncodedTextCopyLimit(int limit);
short __cdecl Savemap_ReturnZero_1(void);
int __cdecl Savemap_RecordAt(int index);
short __cdecl Savemap_ReturnZero_2(void);

extern struct MenuPartyStats g_MenuPartyStats[3];
extern struct PartyBattleModifiers g_PartyBattleModifiers;
extern struct KernelMateria g_KernelMateriaDataArray[96];
extern struct KernelArmor g_KernelArmorDataArray[32];
extern struct KernelWeapon g_KernelWeaponDataArray[128];
extern struct KernelAccessory g_KernelAccessoryDataArray[32];
extern struct BattleItemAction g_MenuKernelItemBuffer[128];
extern struct Savemap gSavemap;
/* battle_window_panels.cpp */
extern short g_BattleSelectedUseCondition;
extern unsigned char g_BtlMenuSlotScreenState;
extern unsigned char g_BtlMenuWindowSets[];
extern unsigned char g_BtlMenuSlotTableC[];
extern unsigned char g_BtlMenuWindowRecordsB[];
extern unsigned char g_BtlMenuSlotTableD[];
extern unsigned char g_BtlMenuWindowRecordsD[];
extern unsigned char g_BtlMenuWindows[];
extern unsigned char g_BtlMenuCursors[];
extern unsigned char g_BtlMenuWindowRecords[];
extern int g_BtlMenuControlledActionCursor;
extern short g_BtlMenuLimitRowCount;
extern unsigned int g_BtlMenuGilAmount;
extern unsigned char g_BtlMenuSlotSeed;
extern unsigned char g_BtlMenuSlotIndex;

/* menu_main_and_battle_results.cpp */
void __cdecl Menu_DrawCharacterHpMpPanel2x(int originX, int originY, int charIndex, float depth);
void __cdecl Menu_DrawCharacterHpMpPanel1x(int originX, int originY, int charIndex, float depth);
int __cdecl Savemap_GetPartyArmourRecord(int slot);
int __cdecl Savemap_GetPartyWeaponRecord(int slot);
int __cdecl Savemap_SpendGil(int amount);
void __cdecl Savemap_ClearNewMateriaQueue(void);
void __cdecl Savemap_AddMateriaToStock(int materia);
void __cdecl Menu_ReleasePortraitsAndReloadPartyMembers(void);
void __cdecl Menu_SetFrameClearFlag(int value);

/* menu_kernel_text_pool.cpp */
int __cdecl Kernel_GetUseCondition(int category, int entry);

/* menu_load_fade.cpp */
int __cdecl Menu_LoadScreenFadeStep(int step);

/* menu_string_copy_savemap.cpp */
int __cdecl Menu_SaveChecksumCrc16(int length, void *data);

struct MenuCursor;

/* menu_cursor_input.cpp */
void __cdecl MenuCursor_Init(struct MenuCursor *cursor, int col, int row, int colCount, int rowCount, int unused4, int scrollTop, int rowStep, int totalRows, int unused8, int scrollAnim, int colWrapMode, int rowWrapMode, int scrollState, int pageScrollEnable);
void __cdecl MenuCursor_Update(struct MenuCursor *cursor);

/* menu_text_primitives.cpp */
void __cdecl Menu_DrawEncodedString2x(int startX, int posY, unsigned char *text, unsigned char colorIndex, float z);
void __cdecl Menu_DrawEncodedString1x(int startX, int posY, unsigned char *text, unsigned char colorIndex, float z);

/* message_window_draw.cpp */
void __cdecl Menu_DrawWindowFrame2x(struct EnglishWindowFrame *windowFrame, float depth);
void __cdecl Menu_DrawWindowFrame1x(struct EnglishWindowFrame *frame, float depth);

/* menu_main_and_battle_results.cpp */
extern struct GfxMenuLabelTable g_MenuMainLabels;
int __cdecl Menu_GetPhase(void);

/* menu_text_primitives.cpp */
void __cdecl Menu_DrawGaugeBar2x(int originX, int originY, int width, int height, unsigned int fillColor, float depth);
void __cdecl Menu_DrawGaugeBar1x(int x, int y, int width, int height, unsigned int color, float depth);

/* message_window_draw.cpp */
void __cdecl Menu_DrawMoreTextArrow2x(int left, int top, float depth);
void __cdecl Menu_DrawMoreTextArrow1x(int posX, int posY, float depth);

/* menu_text_primitives.cpp */
int __cdecl Menu_MeasureEncodedString2x(unsigned char *text);
int __cdecl Menu_MeasureEncodedString1x(unsigned char *text);

/* loadmenu.cpp */
void __cdecl MenuCursor_UpdateScrolling(struct MenuCursor *cursor);

/* loadmenu.cpp */
extern struct MenuCursor g_LoadMenuCursor2;
extern unsigned char g_LoadMenuSlotBuffer[0x100];

/* menu_main_and_battle_results.cpp */
extern int g_MenuColorFormatBitmasks[2];

/* loadmenu.cpp */
extern unsigned char g_LoadMenuPreviewData[0x20];
extern int g_MenuPartyLimitRefreshNeeded;
/* menu_main_and_battle_results.cpp */
void __cdecl Menu_OpenPopupMessage(const void *textPtr, int state);

/* message_window_draw.cpp */
void __cdecl Menu_SetRectShorts(short *rect, int x, int y, int w, int h);
void __cdecl Menu_CopyRectShorts(short *dst, short *src);
void __cdecl Menu_OffsetRectShorts(short *rect, int dx, int dy);
/* loadmenu.cpp */
void __cdecl Menu_LoadScreenInit(int mode);
/* menu_main_and_battle_results.cpp */
short __cdecl Savemap_RemoveItem(int itemRef);

/* battle_window_panels.cpp */
extern unsigned char g_BtlMenuWindowDataBuffer[64];

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_MENU_H */

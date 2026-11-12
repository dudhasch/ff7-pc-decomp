/* include/ff7/enums/menu.h: named codes of ff7/menu and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_MENU_H
#define FF7_ENUMS_MENU_H

/* Item IDs (low 9 bits of a savemap item slot) whose field-menu use Menu_ItemScreenFrame handles. */
enum ItemId {
    ITEM_POTION = 0x00,
    ITEM_HI_POTION = 0x01,
    ITEM_X_POTION = 0x02,
    ITEM_ETHER = 0x03,
    ITEM_TURBO_ETHER = 0x04,
    ITEM_ELIXIR = 0x05,
    ITEM_MEGALIXIR = 0x06,
    ITEM_PHOENIX_DOWN = 0x07,
    ITEM_HYPER = 0x0D,
    ITEM_TRANQUILIZER = 0x0E,
    ITEM_TENT = 0x46,
    ITEM_POWER_SOURCE = 0x47,
    ITEM_GUARD_SOURCE = 0x48,
    ITEM_MAGIC_SOURCE = 0x49,
    ITEM_MIND_SOURCE = 0x4A,
    ITEM_SPEED_SOURCE = 0x4B,
    ITEM_LUCK_SOURCE = 0x4C,
    ITEM_OMNISLASH = 0x57,
    ITEM_CATASTROPHE = 0x58,
    ITEM_FINAL_HEAVEN = 0x59,
    ITEM_GREAT_GOSPEL = 0x5A,
    ITEM_COSMO_MEMORY = 0x5B,
    ITEM_ALL_CREATION = 0x5C,
    ITEM_CHAOS = 0x5D,
    ITEM_HIGHWIND = 0x5E,
    ITEM_SAVE_CRYSTAL = 0x62,     /* sets the North Cave save-point flag */
    ITEM_EARTH_HARP = 0x67        /* handler resets Cait Sith and Vincent records */
};

/* Materia IDs special-cased by the materia detail panels (Menu_DrawMateriaDetailPanel2x/1x and shop variants)
 * and by the master-materia exchange (menu_savemap_materia_queries.cpp). */
enum MateriaId {
    MATERIA_MP_PLUS = 0x00,
    MATERIA_HP_PLUS = 0x01,
    MATERIA_SPEED_PLUS = 0x02,
    MATERIA_MAGIC_PLUS = 0x03,
    MATERIA_LUCK_PLUS = 0x04,
    MATERIA_EXP_PLUS = 0x05,
    MATERIA_GIL_PLUS = 0x06,
    MATERIA_ENEMY_AWAY = 0x07,
    MATERIA_ENEMY_LURE = 0x08,
    MATERIA_CHOCOBO_LURE = 0x09,
    MATERIA_PRE_EMPTIVE = 0x0A,
    MATERIA_LONG_RANGE = 0x0B,
    MATERIA_MEGA_ALL = 0x0C,
    MATERIA_COUNTER_ATTACK = 0x0D,
    MATERIA_SLASH_ALL = 0x0E,
    MATERIA_DOUBLE_CUT = 0x0F,
    MATERIA_COVER = 0x10,
    MATERIA_ENEMY_SKILL = 0x2C,
    MATERIA_MASTER_COMMAND = 0x30,
    MATERIA_MASTER_MAGIC = 0x49,
    MATERIA_BAHAMUT = 0x51,           /* with Neo Bahamut, the pair Savemap_CheckMateriaSetComplete(3) */
    MATERIA_NEO_BAHAMUT = 0x55,       /* requires held; both are in g_MasterSummonMateriaIds */
    MATERIA_BAHAMUT_ZERO = 0x58,      /* what Savemap_ExchangeForMasterMateria(3) adds for the pair; the
                                         three names are the game's (not confirmed in game) */
    MATERIA_MASTER_SUMMON = 0x5A
};

/* Equipment slot kind under the equip-screen cursor, dispatched in Menu_EquipScreenFrame. */
enum EquipKind {
    EQUIP_WEAPON = 0x00,
    EQUIP_ARMOR = 0x01,
    EQUIP_ACCESSORY = 0x02
};

/* Materia slot AP growth rate of a weapon or armor, shown by Menu_EquipScreenFrame and the shop panels. */
enum SlotGrowth {
    GROWTH_NONE = 0x00,       /* menu text "Nothing" */
    GROWTH_NORMAL = 0x01,
    GROWTH_DOUBLE = 0x02,
    GROWTH_TRIPLE = 0x03
};

/* Materia screen input states; Menu_MateriaScreenFrame dispatches them and the draw helpers switch on them.
 * A state is also the index of the MenuCursor in g_MenuMateriaCursorBlock that drives it. */
enum MateriaMenuState {
    MATERIA_MENU_SELECT_CHECK_ARRANGE = 0x00,
    MATERIA_MENU_SELECT_SLOT = 0x01,
    MATERIA_MENU_SELECT_STOCK_MATERIA = 0x03,
    MATERIA_MENU_CHECK_COMMANDS = 0x04,
    MATERIA_MENU_CHECK_MAGIC = 0x05,
    MATERIA_MENU_CHECK_SUMMON = 0x06,
    MATERIA_MENU_CHECK_ENEMY_SKILL = 0x07,
    MATERIA_MENU_ARRANGE_MENU = 0x08,
    MATERIA_MENU_TRASH_SELECT = 0x09,
    MATERIA_MENU_TRASH_CONFIRM = 0x0A,
    MATERIA_MENU_EXCHANGE = 0x0B
};

/* Shop screen input states; Menu_ShopScreenFrame dispatches them for drawing and input. */
enum ShopMenuState {
    SHOP_MENU_MAIN_MENU = 0x00,
    SHOP_MENU_BUY_LIST = 0x01,
    SHOP_MENU_SELL_ITEM_LIST = 0x02,
    SHOP_MENU_SELL_MATERIA_LIST = 0x03,
    SHOP_MENU_BUY_QUANTITY = 0x04,
    SHOP_MENU_SELL_QUANTITY = 0x05,
    SHOP_MENU_SELL_ITEM_OR_MATERIA = 0x06
};

/* What a shop entry sells (MenuShopEntry.kind). No shop has a kind 2 entry; the shop screen
 * counts it as owned 0 and draws neither its name nor a comparison. */
enum MenuShopEntryKind {
    MENU_SHOP_ENTRY_ITEM = 0,     /* an item id: weapons from 0x80, armour 0x100, accessories 0x120 */
    MENU_SHOP_ENTRY_MATERIA = 1,
    MENU_SHOP_ENTRY_NOTHING = 2
};

/* Columns of the shop's main menu (g_MenuShopCursorBlock[0].col). */
enum MenuShopCommand {
    MENU_SHOP_COMMAND_BUY = 0,
    MENU_SHOP_COMMAND_SELL = 1,
    MENU_SHOP_COMMAND_EXIT = 2
};

/* The shop's fade state (g_MenuShopPhase); Menu_ShopModuleUpdate runs frames until it is done. */
enum MenuShopFade {
    MENU_SHOP_FADE_IN = 0,
    MENU_SHOP_FADE_ACTIVE = 1,    /* the shop takes input */
    MENU_SHOP_FADE_OUT = 2,
    MENU_SHOP_FADE_DONE = -1
};

/* Which materia slot layout the shop's buy-quantity window shows for the selected item. */
enum MenuShopEquipType {
    MENU_SHOP_EQUIP_NONE = 0,
    MENU_SHOP_EQUIP_WEAPON = 1,
    MENU_SHOP_EQUIP_ARMOR = 2
};

/* g_MateriaDetailPanelKind: the ability list a materia's describe-mode handler wrote, which the
 * materia detail panels draw (the other values fall back to the per-materia text). */
enum MateriaDetailPanelKind {
    MATERIA_DETAIL_PANEL_MAGIC = 1,       /* Kernel_ApplyMagicMateria: magic names */
    MATERIA_DETAIL_PANEL_SUMMON = 2,      /* Kernel_ApplySummonMateria: one summon name */
    MATERIA_DETAIL_PANEL_COMMAND = 3,     /* Kernel_ApplyCommandMateria: command names */
    MATERIA_DETAIL_PANEL_STAT_PLUS = 4,          /* Kernel_ApplyMateriaStatPlus */
    MATERIA_DETAIL_PANEL_MASTER_COMMAND = 5,     /* Kernel_ApplyMasterCommandMateria */
    MATERIA_DETAIL_PANEL_MASTER_MAGIC = 6,       /* Kernel_ApplyMasterMagicMateria */
    MATERIA_DETAIL_PANEL_MASTER_SUMMON = 7,      /* Kernel_ApplyMasterSummonMateria */
    MATERIA_DETAIL_PANEL_ENEMY_SKILL = 8,        /* Kernel_ApplyEnemySkillMateria */
    MATERIA_DETAIL_PANEL_ENCOUNTER_RATE = 9,     /* Kernel_ApplyMateriaEncounterRate */
    MATERIA_DETAIL_PANEL_ENCOUNTER_MODIFIER = 0xA, /* Kernel_ApplyMateriaEncounterModifier */
    MATERIA_DETAIL_PANEL_MEGA_ALL = 0xB,         /* Kernel_ApplyMegaAllMateria */
    MATERIA_DETAIL_PANEL_EXP_BONUS = 0xD,        /* Kernel_ApplyMateriaExpBonus */
    MATERIA_DETAIL_PANEL_DOUBLE_CAST = 0xE,      /* Kernel_ApplyDoubleCastMateria */
    MATERIA_DETAIL_PANEL_SUPPORT = 0x10,         /* Kernel_DescribeSupportMateria / Kernel_DescribeSupportMateriaPercent */
    MATERIA_DETAIL_PANEL_ABILITY_FLAG = 0x11,    /* Kernel_ApplyMateriaAbilityFlag */
    MATERIA_DETAIL_PANEL_EXTRA_CUT = 0x13 /* Kernel_GrantExtraCutCommand: command names */
};

/* Where the PHS (party change) screen's cursor is (g_PhsMenuSwapState): the party column, the
 * reserve grid, or a member's details shown after picking a member and dropping it on itself.
 * g_PhsMenuHeldSource and g_PhsMenuPrevSwapState hold the same values. */
enum PhsMenuFocus {
    PHS_MENU_FOCUS_PARTY = 0,
    PHS_MENU_FOCUS_RESERVE = 1,
    PHS_MENU_FOCUS_DETAILS = 2
};

/* Whether the PHS screen holds a picked-up member (g_PhsMenuState): Circle picks one up, the
 * next Circle swaps it with the member under the cursor. */
enum PhsMenuHoldState {
    PHS_MENU_HOLD_NONE = 0,
    PHS_MENU_HOLD_MEMBER = 1
};

/* The fade of the PHS screen when a field script opens it (g_PhsMenuSelection). */
enum PhsMenuFade {
    PHS_MENU_FADE_IN = 0,
    PHS_MENU_FADE_ACTIVE = 1,     /* the screen takes input */
    PHS_MENU_FADE_OUT = 2,
    PHS_MENU_FADE_DONE = -1
};

/* How a menu cursor moves past the edge of its columns or rows (MenuCursor.colWrapMode and
 * .rowWrapMode): it stops (a row cursor scrolls the list instead), wraps around, or (columns)
 * wraps into the next or previous row. */
enum MenuCursorWrap {
    MENU_CURSOR_CLAMP = 0,
    MENU_CURSOR_WRAP = 1,
    MENU_CURSOR_WRAP_INTO_ROWS = 2
};

/* A menu cursor's list scroll in progress (MenuCursor.scrollState). */
enum MenuCursorScroll {
    MENU_SCROLL_NONE = 0,
    MENU_SCROLL_UP = 1,
    MENU_SCROLL_DOWN = 2
};

/* The Item screen's state (g_MenuItemState), also the index of the MenuCursor block that drives it. */
enum MenuItemScreenState {
    MENU_ITEM_STATE_COMMAND_BAR = 0,  /* Use / Arrange / Key Items */
    MENU_ITEM_STATE_ITEM_LIST = 1,
    MENU_ITEM_STATE_SELECT_TARGET = 2,
    MENU_ITEM_STATE_KEY_ITEMS = 3,
    MENU_ITEM_STATE_ARRANGE_CHOICE = 4,
    MENU_ITEM_STATE_SWAP_LIST = 5
};

/* Columns of the Item screen's command bar. */
enum MenuItemCommand {
    MENU_ITEM_COMMAND_USE = 0,
    MENU_ITEM_COMMAND_ARRANGE = 1,
    MENU_ITEM_COMMAND_KEY_ITEMS = 2
};

/* Where the Arrange swap list is: waiting for the first slot, or for the slot to swap it with. */
enum MenuItemSwapState {
    MENU_ITEM_SWAP_PICK_FIRST = 0,
    MENU_ITEM_SWAP_PICK_SECOND = 1
};

/* A savemap item slot is an item id in its low 9 bits and a quantity above them. */
enum MenuItemSlot {
    MENU_ITEM_QUANTITY_ONE = 0x200,
    MENU_ITEM_SLOT_EMPTY = 0xFFFF
};

/* The Arrange choice (g_MenuItemCursors[4].row, Menu_ItemArrangeBySortMode's mode): Customize opens
 * the swap list, the others sort the inventory with the comparator they name. */
enum MenuItemArrangeMode {
    MENU_ITEM_ARRANGE_CUSTOMIZE = 0,
    MENU_ITEM_ARRANGE_FIELD = 1,   /* items usable from the menu first (Menu_CompareItemSellability) */
    MENU_ITEM_ARRANGE_BATTLE = 2,  /* items usable in battle first */
    MENU_ITEM_ARRANGE_THROW = 3,   /* throwable items first */
    MENU_ITEM_ARRANGE_TYPE = 4,
    MENU_ITEM_ARRANGE_NAME = 5,
    MENU_ITEM_ARRANGE_MOST = 6,
    MENU_ITEM_ARRANGE_LEAST = 7
};

/* An item's restriction bits (Kernel_GetEquipRestrictions, Menu_GetItemUseRestrictions): a set bit
 * forbids the use. The shop sells an item only without NO_SELL; the Item screen greys an item with
 * NO_MENU_USE (also set for the Save Crystal and, with Save locked, the Tent). */
enum MenuItemRestriction {
    MENU_ITEM_RESTRICT_NO_SELL = 0x1,
    MENU_ITEM_RESTRICT_NO_BATTLE_USE = 0x2,
    MENU_ITEM_RESTRICT_NO_MENU_USE = 0x4,
    MENU_ITEM_RESTRICT_NO_THROW = 0x8
};

/* Order of the stat items (Power Source .. Luck Source) from ITEM_POWER_SOURCE. */
enum MenuItemSourceStat {
    MENU_SOURCE_STRENGTH = 0,
    MENU_SOURCE_VITALITY = 1,
    MENU_SOURCE_MAGIC = 2,
    MENU_SOURCE_SPIRIT = 3,
    MENU_SOURCE_DEXTERITY = 4,
    MENU_SOURCE_LUCK = 5
};

/* Battle menu state, equal to the id of the active battle window; per-window handlers in battle_window_panels.c. */
enum BattleMenuState {
    BATTLE_MENU_SELECT_TARGET = 0x00,
    BATTLE_MENU_COMMAND_MENU = 0x01,
    BATTLE_MENU_CHANGE_CONFIRM = 0x02,
    BATTLE_MENU_DEFEND_CONFIRM = 0x03,
    BATTLE_MENU_ENEMY_SKILL_LIST = 0x04,
    BATTLE_MENU_ITEM_LIST = 0x05,
    BATTLE_MENU_MAGIC_LIST = 0x06,
    BATTLE_MENU_SUMMON_LIST = 0x07,
    BATTLE_MENU_UNUSED_08 = 0x08,           /* freezes Wait-mode ATB; no window uses slot 8 */
    BATTLE_MENU_ARENA_CONTINUE_CHOICE = 0x09,
    BATTLE_MENU_MANIP_ACTION_LIST = 0x13,
    BATTLE_MENU_GIL_AMOUNT = 0x14,
    BATTLE_MENU_LIMIT_LIST = 0x18,
    BATTLE_MENU_CAIT_SITH_SLOTS = 0x1A,
    BATTLE_MENU_TIFA_SLOTS = 0x1B,
    BATTLE_MENU_ARENA_SLOT = 0x1C
};

/* Battle menu windows: indices of g_BattleMenuData, g_BattleWindowStates and the window handler
 * tables (Battle_OpenWindow). A list or prompt window has the id of the BattleMenuState that drives it. */
enum BattleWindowId {
    BATTLE_WINDOW_PARTY_STATUS = 0x00,      /* Battle_DrawPartyStatusOverview1x/2x */
    BATTLE_WINDOW_COMMAND_MENU = 0x01,
    BATTLE_WINDOW_CHANGE = 0x02,
    BATTLE_WINDOW_DEFEND = 0x03,
    BATTLE_WINDOW_ENEMY_SKILL_LIST = 0x04,
    BATTLE_WINDOW_ITEM_LIST = 0x05,
    BATTLE_WINDOW_MAGIC_LIST = 0x06,
    BATTLE_WINDOW_SUMMON_LIST = 0x07,
    BATTLE_WINDOW_ARENA_CONTINUE_CHOICE = 0x09,
    BATTLE_WINDOW_MANIP_ACTION_LIST = 0x13,
    BATTLE_WINDOW_GIL_AMOUNT = 0x14,
    BATTLE_WINDOW_PARTY_HUD = 0x15,         /* the party's HP/MP rows while an item picks its target */
    BATTLE_WINDOW_ACTION_NAME = 0x16,       /* the running action's name, drawn in the message window's place */
    BATTLE_WINDOW_HELP = 0x17,              /* the help text (gSavemap.options 0x4000) */
    BATTLE_WINDOW_LIMIT_LIST = 0x18,
    BATTLE_WINDOW_MESSAGE = 0x19,           /* a KERNEL_TEXT_BATTLE_MESSAGE text */
    BATTLE_WINDOW_CAIT_SITH_SLOTS = 0x1A,
    BATTLE_WINDOW_TIFA_SLOTS = 0x1B,
    BATTLE_WINDOW_ARENA_SLOT = 0x1C
};

/* A battle window's state (g_BattleWindowStates[BattleWindowId]): Battle_ProcessCommandMenuInput
 * steps the opening and closing animations and runs the open window's active handler. */
enum BattleWindowState {
    BATTLE_WINDOW_STATE_CLOSED = 0,
    BATTLE_WINDOW_STATE_OPENING = 1,
    BATTLE_WINDOW_STATE_OPEN = 2,
    BATTLE_WINDOW_STATE_CLOSING = 3
};

/* The Battle Square handicap slot screen's state (g_BtlMenuSlotScreenState), window 0x1C. */
enum BattleSlotScreenState {
    BATTLE_SLOT_SCREEN_CHOOSE = 0,          /* the cursor picks spin or not; shows the points total */
    BATTLE_SLOT_SCREEN_SPINNING = 1,        /* the reels turn; Circle stops the current one */
    BATTLE_SLOT_SCREEN_START_PROMPT = 2,    /* a flashing prompt; Circle starts the reels */
    BATTLE_SLOT_SCREEN_RESULT = 3,          /* the handicap the reels gave; Circle closes the screen */
    BATTLE_SLOT_SCREEN_DONE = 4,
    BATTLE_SLOT_SCREEN_INTRO = 5            /* the opening text, until Circle */
};

/* BtlMenuItemSlot.flags bits the battle item list tests: an item with the bit set cannot be chosen. */
enum BattleMenuItemFlag {
    BATTLE_MENU_ITEM_NO_BATTLE_USE = 0x02,  /* Item and W-Item */
    BATTLE_MENU_ITEM_NO_THROW = 0x08        /* Throw */
};

/* The save file save/saveNN.ff7: SAVE_FILE_MAGIC (a word the loader compares), a byte (the save
 * screen's cursor and slot), the slot-present mask (a word), then SAVE_FILE_SLOT_COUNT savemaps of
 * 0x10F4 bytes (gSaveFileBuffer; Menu_WriteSaveSlot writes the block, LoadMenu_ReadSaveFile reads it). */
enum SaveFileFormat {
    SAVE_FILE_MAGIC = 0x06277371,
    SAVE_FILE_SLOT_COUNT = 15
};

/* g_LoadMenuErrorCode, the screen Menu_LoadScreenTick shows (the name is the original's; it is the menu's state). */
enum LoadMenuScreen {
    LOAD_MENU_SLOT_LIST = 0, /* "Select a save data file.": the ten save files */
    LOAD_MENU_GAME_LIST = 1, /* "Select a save game.": the slots of the chosen file */
    LOAD_MENU_CHECKING = 2, /* "Checking Save Data File.": the slots are cached one by one */
    LOAD_MENU_CHECKED = 3, /* the check is over; waits g_LoadMenuInputDelay frames */
    LOAD_MENU_LOADING = 4, /* "Loading. Please wait." */
    LOAD_MENU_TITLE = 7 /* New Game / Continue */
};

/* g_LoadMenuFadePhase: the fade of the load menu. */
enum LoadMenuFade {
    LOAD_MENU_FADE_IN = 0,
    LOAD_MENU_FADE_NONE = 1, /* the menu takes input */
    LOAD_MENU_FADE_OUT = 2,
    LOAD_MENU_FADE_DONE = -1
};

/* The rows of the load menu's title choice (LOAD_MENU_TITLE, g_LoadMenuCursor2's row). */
enum LoadMenuTitleRow {
    LOAD_MENU_TITLE_NEW_GAME = 0,
    LOAD_MENU_TITLE_CONTINUE = 1  /* opens the save file list when a save file exists */
};

/* Menu_LoadSaveFileToBuffer's mode: the load menu reads an existing save file; the save menu also starts
 * an empty one when the file is not there. */
enum SaveFileLoadMode {
    SAVE_FILE_LOAD_EXISTING = 0,
    SAVE_FILE_LOAD_OR_NEW = 1
};

/* What Menu_LoadSaveFileToBuffer returns; the menus show g_SaveLoadTextAllocFailed or
 * g_SaveLoadTextInvalidSaveFile for the two failures. */
enum SaveFileLoadResult {
    SAVE_FILE_LOAD_OK = 1,
    SAVE_FILE_LOAD_NO_MEMORY = 4,     /* Menu_SaveFileBufferAlloc failed */
    SAVE_FILE_LOAD_BAD_MAGIC = 8      /* the file does not start with SAVE_FILE_MAGIC */
};

/* A save file's entry in g_LoadMenuPreviewData (SaveSlotPreview.status). */
enum SaveFileStatus {
    SAVE_FILE_STATUS_ABSENT = 0,
    SAVE_FILE_STATUS_PRESENT = 1,
    SAVE_FILE_STATUS_NEW = 2          /* not on disk; an empty buffer stands for it */
};

/* The two panes of the name entry screen (g_MenuNameEntrySelectedPane), which are also the indices of its
 * two cursors: the glyph grid and the command list. */
enum MenuNameEntryPane {
    MENU_NAME_ENTRY_PANE_GRID = 0,
    MENU_NAME_ENTRY_PANE_COMMANDS = 1
};

/* The name entry command list rows (the second cursor's row): Space, Delete, Select, Default. */
enum MenuNameEntryCommand {
    MENU_NAME_ENTRY_COMMAND_SPACE = 0,
    MENU_NAME_ENTRY_COMMAND_DELETE = 1,
    MENU_NAME_ENTRY_COMMAND_SELECT = 2,
    MENU_NAME_ENTRY_COMMAND_DEFAULT = 3
};

/* The name entry screen's fade state (g_MenuNameEntryFadePhase). */
enum MenuNameEntryFade {
    MENU_NAME_ENTRY_FADE_IN = 0,
    MENU_NAME_ENTRY_FADE_ACTIVE = 1,
    MENU_NAME_ENTRY_FADE_OUT = 2,
    MENU_NAME_ENTRY_FADE_DONE = -1
};

/* The name entry screen's glyph grid (g_MenuNameEntryTargetSlot, Menu_GetKeyGridByte's `which`):
 * g_KeyGridA, g_KeyGridB, or g_KeyGridChars (A-Z, a-z, punctuation and digits, seven rows), the
 * one Menu_NameEntryInit selects. */
enum MenuNameEntryKeyGrid {
    MENU_NAME_ENTRY_KEY_GRID_A = 0,
    MENU_NAME_ENTRY_KEY_GRID_B = 1,
    MENU_NAME_ENTRY_KEY_GRID_LATIN = 2
};

/* Entries of g_MenuEquipLabelTable, 12 bytes each, that the equipment screen indexes by name. */
enum MenuEquipLabel {
    MENU_EQUIP_LABEL_ATTACK = 10,
    MENU_EQUIP_LABEL_SLOT = 17,
    MENU_EQUIP_LABEL_GROWTH = 18,
    MENU_EQUIP_LABEL_NOTHING = 19
};

/* The Save menu's screen phase (g_SaveMenuPhase). */
enum SaveMenuPhase {
    SAVE_MENU_PHASE_SELECT_FILE = 0,
    SAVE_MENU_PHASE_SELECT_SLOT = 1,
    SAVE_MENU_PHASE_CHECKING = 2,
    SAVE_MENU_PHASE_CHECKED = 3,
    SAVE_MENU_PHASE_SAVING = 4,
    SAVE_MENU_PHASE_CONFIRM_OVERWRITE = 7
};

/* The Save menu's fade state when opened from the field (g_SaveMenuFadePhase). */
enum SaveMenuFade {
    SAVE_MENU_FADE_IN = 0,
    SAVE_MENU_FADE_ACTIVE = 1,
    SAVE_MENU_FADE_OUT = 2,
    SAVE_MENU_FADE_DONE = -1
};

/* The rows of the Save menu's overwrite confirmation (SAVE_MENU_PHASE_CONFIRM_OVERWRITE,
 * g_SaveMenuConfirmCursor's row): save over the slot, or go back to the slot list. */
enum SaveMenuConfirmRow {
    SAVE_MENU_CONFIRM_YES = 0,
    SAVE_MENU_CONFIRM_NO = 1
};

/* What a message window draws beside its text (WindowData.numDisplayType, set by the field's WSPCL). */
enum MessageWindowAddon {
    MESSAGE_WINDOW_ADDON_NONE = 0,
    MESSAGE_WINDOW_ADDON_COUNTDOWN = 1,  /* the countdown timer, from gSavemap.countdownSeconds */
    MESSAGE_WINDOW_ADDON_NUMBER = 2      /* the window's number value, digits wide */
};

/* States of the input configuration screen (g_MenuKeyConfigEditState): browsing the binding slots,
 * or waiting for a new key in the keyboard or joystick column; InputConfig_CaptureSource takes the
 * capture state as its source. */
enum MenuKeyConfigEditState {
    MENU_KEY_CONFIG_BROWSE = 0,
    MENU_KEY_CONFIG_CAPTURE_KEYBOARD = 1,
    MENU_KEY_CONFIG_CAPTURE_JOYSTICK = 2
};

/* Commands of the scripted input stream (g_pInputScriptStream) that Input_ReadScriptedInputCommand
 * runs in place of the pad: the button commands press one InputButton for a frame, then wait
 * 0x14 frames. */
enum MenuInputScriptOp {
    MENU_INPUT_SCRIPT_WAIT = 0x00,              /* 16-bit frame count follows */
    MENU_INPUT_SCRIPT_PRESS_UP = 0x02,
    MENU_INPUT_SCRIPT_PRESS_DOWN = 0x03,
    MENU_INPUT_SCRIPT_PRESS_LEFT = 0x04,
    MENU_INPUT_SCRIPT_PRESS_RIGHT = 0x05,
    MENU_INPUT_SCRIPT_PRESS_TRIANGLE = 0x06,
    MENU_INPUT_SCRIPT_PRESS_CROSS = 0x07,
    MENU_INPUT_SCRIPT_PRESS_SQUARE = 0x08,
    MENU_INPUT_SCRIPT_PRESS_CIRCLE = 0x09,
    MENU_INPUT_SCRIPT_PRESS_R1 = 0x0A,
    MENU_INPUT_SCRIPT_PRESS_R2 = 0x0B,
    MENU_INPUT_SCRIPT_PRESS_L1 = 0x0C,
    MENU_INPUT_SCRIPT_PRESS_L2 = 0x0D,
    MENU_INPUT_SCRIPT_SHOW_TEXT = 0x10,         /* inline 0xFF-terminated popup text, then 0x50 frames */
    MENU_INPUT_SCRIPT_END = 0x11,               /* ends scripted input */
    MENU_INPUT_SCRIPT_SET_POPUP_POS = 0x12      /* 16-bit x and y of the next popup */
};

/* The named 20-byte label records of g_MenuMainLabels, after the eleven main-menu commands. */
enum MenuMainLabel {
    MENU_LABEL_BEGINNER = 11,
    MENU_LABEL_TIME = 14,
    MENU_LABEL_GIL = 15,
    MENU_LABEL_NEXT_LEVEL = 16,
    MENU_LABEL_LIMIT_LEVEL = 17,
    MENU_LABEL_TUTORIAL = 18,
    MENU_LABEL_UNDER = 19,
    MENU_LABEL_LEVEL_UP = 20,
    MENU_LABEL_FURY = 21,
    MENU_LABEL_SADNESS = 22
};

/* The main menu's commands: the rows of its command cursor (g_MenuMainCursors.command.row), the bits
 * of g_MenuVisibleMask and g_MenuLockedMask (savemap menuVisible/menuLocked) and the first eleven
 * g_MenuMainLabels records. g_MenuCommandScreenIdMap maps a row to its MenuScreenId. */
enum MenuMainCommand {
    MENU_MAIN_COMMAND_ITEM = 0,
    MENU_MAIN_COMMAND_MAGIC = 1,
    MENU_MAIN_COMMAND_MATERIA = 2,
    MENU_MAIN_COMMAND_EQUIP = 3,
    MENU_MAIN_COMMAND_STATUS = 4,
    MENU_MAIN_COMMAND_ORDER = 5,
    MENU_MAIN_COMMAND_LIMIT = 6,
    MENU_MAIN_COMMAND_CONFIG = 7,
    MENU_MAIN_COMMAND_PHS = 8,
    MENU_MAIN_COMMAND_SAVE = 9,
    MENU_MAIN_COMMAND_QUIT = 10       /* always visible and never locked */
};

/* Field-menu screen ids (g_MenuCurrentScreenId, g_MenuPreviousScreenId): the index of the screen's
 * g_MenuScreenInitCallbacks and g_MenuScreenFrameCallbacks entries. */
enum MenuScreenId {
    MENU_SCREEN_MAIN = 0,
    MENU_SCREEN_ITEM = 1,
    MENU_SCREEN_MAGIC = 2,
    MENU_SCREEN_MATERIA = 3,
    MENU_SCREEN_EQUIP = 4,
    MENU_SCREEN_STATUS = 5,
    MENU_SCREEN_LIMIT = 7,
    MENU_SCREEN_CONFIG = 8,
    MENU_SCREEN_PHS = 9,
    MENU_SCREEN_SAVE = 10,
    MENU_SCREEN_QUIT = 11
};

/* The field menu's phase (g_MenuPhase, Menu_SetPhase): how Menu_DrawCommandWindow2x/1x draw the
 * command window between the main menu and a screen. */
enum MenuPhase {
    MENU_PHASE_SCREEN = 0,            /* a screen runs; the window shows its one command */
    MENU_PHASE_MAIN = 1,              /* the main menu's full command list takes input */
    MENU_PHASE_OPEN_SCREEN = 2,       /* the list collapses, then the screen's init callback runs */
    MENU_PHASE_SCREEN_FADE_IN = 3,    /* the screen fades in, then MENU_PHASE_SCREEN */
    MENU_PHASE_SWITCH_SCREEN = 4,     /* a screen hands over to another, then its init callback runs */
    MENU_PHASE_CLOSE_SCREEN = 5,      /* the screen fades out */
    MENU_PHASE_EXPAND_COMMANDS = 6    /* the command list grows back, then MENU_PHASE_MAIN */
};

/* Which of the main menu's three cursors takes input (g_MenuMainCursorIndex). */
enum MenuMainCursor {
    MENU_MAIN_CURSOR_COMMAND = 0,     /* g_MenuMainCursors.command */
    MENU_MAIN_CURSOR_PARTY = 1,       /* g_MenuMainCursors.party: the member a command applies to */
    MENU_MAIN_CURSOR_ORDER = 2        /* g_MenuMainCursors.swap: Order picks two members to swap */
};

/* The main menu's slide in and out (g_MenuTopLevelCloseState). */
enum MenuTopLevelState {
    MENU_TOP_LEVEL_OPENING = 0,       /* the windows slide in */
    MENU_TOP_LEVEL_ACTIVE = 1,
    MENU_TOP_LEVEL_CLOSING = 2,       /* the windows slide out */
    MENU_TOP_LEVEL_CLOSED = -1        /* Menu_RunTopLevelFrame ends the menu */
};

/* The menu popup message's animation (g_MenuTutorialWindowState), Menu_DrawPopupMessage2x/1x. */
enum MenuPopupState {
    MENU_POPUP_HIDDEN = 0,
    MENU_POPUP_OPENING = 1,           /* the box grows for three steps */
    MENU_POPUP_SHOWN = 2,             /* the text, until its lifetime or a key ends it */
    MENU_POPUP_CLOSING = 3            /* the box shrinks, then MENU_POPUP_HIDDEN */
};

/* How a menu is entered over a module (g_MenuEntryState). */
enum MenuEntryState {
    MENU_ENTRY_NONE = 0,
    MENU_ENTRY_BATTLE_UI = 2,         /* Battle_InitOpeningUiState: the battle module runs the battle UI */
    MENU_ENTRY_BATTLE_RESULT = 4      /* the end sequence: the menu module runs the result screens */
};

/* The battle result screens' state (g_MenuBattleEndMode), Battle_DrawResultScreenFrame. */
enum MenuBattleEndMode {
    MENU_BATTLE_END_EXP = 0,          /* the experience and AP screen */
    MENU_BATTLE_END_EXP_TO_GIL = 1,   /* a key on the EXP screen: both screens drawn while it fades */
    MENU_BATTLE_END_GIL_ITEMS = 2,    /* the gil and item screen takes input */
    MENU_BATTLE_END_GIL_ITEMS_FADE_OUT = 3,
    MENU_BATTLE_END_FADED_OUT = 4,
    MENU_BATTLE_END_DONE = 5          /* adds the chosen items to the inventory and ends the menu entry */
};

/* The rows of the result gil and item screen's cursor (g_MenuResultItemRow); rows 1..4 are the
 * reward items. */
enum MenuResultItemRow {
    MENU_RESULT_ITEM_ROW_TAKE_ALL = 0, /* selects every reward item */
    MENU_RESULT_ITEM_ROW_CONTINUE = 5  /* Start or Circle leaves the screen */
};

/* Kernel_GetEquippedAttackOrDefense's kind: the equipment value it returns (the magic ones are 0). */
enum MenuEquipValue {
    MENU_EQUIP_VALUE_ATTACK = 0,      /* the weapon's attack */
    MENU_EQUIP_VALUE_DEFENSE = 1,     /* the armour's defense */
    MENU_EQUIP_VALUE_MAGIC_ATTACK = 2,
    MENU_EQUIP_VALUE_MAGIC_DEFENSE = 3
};

/* The stat an equipment stat bonus raises (equipStatTarget), Savemap_AddEquipStatBonus. */
enum MenuEquipStat {
    MENU_EQUIP_STAT_STRENGTH = 0,
    MENU_EQUIP_STAT_VITALITY = 1,
    MENU_EQUIP_STAT_MAGIC = 2,
    MENU_EQUIP_STAT_SPIRIT = 3,
    MENU_EQUIP_STAT_DEXTERITY = 4,
    MENU_EQUIP_STAT_LUCK = 5
};

/* What an armour's or accessory's element defense does (its element byte),
 * Savemap_AddElementDefense. */
enum MenuElementDefense {
    MENU_ELEMENT_DEFENSE_ABSORB = 0,  /* absorbedElements */
    MENU_ELEMENT_DEFENSE_NULLIFY = 1, /* nullifiedElements */
    MENU_ELEMENT_DEFENSE_HALVE = 2    /* halvedElements */
};

/* The Config screen's state (g_MenuConfigState): browsing the rows, the window colour
 * editor choosing a corner or editing its colour, the controller (input binding) screen, or the
 * music/sound volume panel. */
enum MenuConfigScreenState {
    MENU_CONFIG_STATE_BROWSE = 0,
    MENU_CONFIG_STATE_WINDOW_COLOR_CORNER = 1,
    MENU_CONFIG_STATE_WINDOW_COLOR_EDIT = 2,
    MENU_CONFIG_STATE_CONTROLLER = 3,
    MENU_CONFIG_STATE_VOLUME = 0xA
};

/* The Config screen's rows (g_MenuConfigCursor), in the order of their labels. */
enum MenuConfigRow {
    MENU_CONFIG_ROW_WINDOW_COLOR = 0,
    MENU_CONFIG_ROW_SOUND = 1,
    MENU_CONFIG_ROW_CONTROLLER = 2,
    MENU_CONFIG_ROW_CURSOR = 3,
    MENU_CONFIG_ROW_ATB = 4,
    MENU_CONFIG_ROW_BATTLE_SPEED = 5,
    MENU_CONFIG_ROW_BATTLE_MESSAGE = 6,
    MENU_CONFIG_ROW_FIELD_MESSAGE = 7,
    MENU_CONFIG_ROW_CAMERA_ANGLE = 8,
    MENU_CONFIG_ROW_MAGIC_ORDER = 9,
    MENU_CONFIG_ROW_COUNT = 10
};

/* The Controller row's setting (gSavemap.options bits 2-3): "Normal" or "Customize". */
enum MenuConfigControllerMode {
    MENU_CONFIG_CONTROLLER_NORMAL = 0,
    MENU_CONFIG_CONTROLLER_CUSTOMIZE = 1
};

/* The rows of the Config screen's volume panel (g_MenuConfigVolumeCursorRow). */
enum MenuConfigVolumeRow {
    MENU_CONFIG_VOLUME_MUSIC = 0,
    MENU_CONFIG_VOLUME_SFX = 1
};

/* The window colour editor's mode (g_MenuConfigEditorMode): choosing one of the four corners,
 * editing that corner's colour, or closed (set on leaving the editor). */
enum MenuConfigColorEditorMode {
    MENU_CONFIG_COLOR_EDITOR_SELECT_CORNER = 0,
    MENU_CONFIG_COLOR_EDITOR_EDIT_COLOR = 1,
    MENU_CONFIG_COLOR_EDITOR_REENTER_EDIT = 2,
    MENU_CONFIG_COLOR_EDITOR_CLOSED = -1
};

/* The colour channel rows of the window colour editor (g_MenuColorEditorCursor); each corner colour
 * is a 0x00RRGGBB word, so red is its byte 2 and blue its byte 0. */
enum MenuConfigColorChannel {
    MENU_CONFIG_COLOR_RED = 0,
    MENU_CONFIG_COLOR_GREEN = 1,
    MENU_CONFIG_COLOR_BLUE = 2
};

/* The Magic screen's state (g_MenuMagicState), also the index of the MenuCursor block that drives
 * it. The three lists follow the command list's rows: state = row + MENU_MAGIC_STATE_SPELL_LIST. */
enum MenuMagicScreenState {
    MENU_MAGIC_STATE_COMMAND_LIST = 0,     /* Magic / Summon / Enemy-Skill */
    MENU_MAGIC_STATE_SELECT_TARGET = 1,    /* the party target of a Cure or Life spell */
    MENU_MAGIC_STATE_SPELL_LIST = 2,
    MENU_MAGIC_STATE_SUMMON_LIST = 3,
    MENU_MAGIC_STATE_ENEMY_SKILL_LIST = 4
};

/* Rows of the Magic screen's command list (g_MenuMagicListRow), which are also the first three
 * 20-byte labels of g_MenuMagicLabelTable. */
enum MenuMagicCommandRow {
    MENU_MAGIC_ROW_MAGIC = 0,
    MENU_MAGIC_ROW_SUMMON = 1,
    MENU_MAGIC_ROW_ENEMY_SKILL = 2
};

/* The other named labels of g_MenuMagicLabelTable: "Added-Ability", then the added abilities'
 * names in MenuMagicAddedAbility order from MENU_MAGIC_LABEL_FIRST_ADDED. */
enum MenuMagicLabel {
    MENU_MAGIC_LABEL_ADDED_ABILITY = 3,
    MENU_MAGIC_LABEL_FIRST_ADDED = 4
};

/* The rows of a spell's added-ability box on the Magic screen: All and Quadra Magic (their counts),
 * the support bits from MENU_MAGIC_ADDED_MP_ABSORB on (1 << (row - MENU_MAGIC_ADDED_MP_ABSORB)),
 * and the Turbo MP level (BattleAbilitySlot.support >> 5). */
enum MenuMagicAddedAbility {
    MENU_MAGIC_ADDED_ALL = 0,
    MENU_MAGIC_ADDED_QUADRA_MAGIC = 1,
    MENU_MAGIC_ADDED_MP_ABSORB = 2,
    MENU_MAGIC_ADDED_TURBO_MP = 7
};

/* The 20-byte messages of g_MenuMagicMessageTable: "Nothing." and "Not enough MP.". */
enum MenuMagicMessage {
    MENU_MAGIC_MESSAGE_NOTHING = 0,
    MENU_MAGIC_MESSAGE_NOT_ENOUGH_MP = 1
};

/* gSavemap.magicMenuFlags: the lists the Magic screen offers beside Magic. The stock sets SUMMON
 * when a summon materia enters it and ENEMY_SKILL when the Enemy Skill materia (0x2C) does. */
enum SavemapMagicMenuFlag {
    MAGIC_MENU_FLAG_SUMMON = 0x01,
    MAGIC_MENU_FLAG_ENEMY_SKILL = 0x02
};

/* The kernel magic ids (BattleAbilitySlot.attackId of a MenuPartyStats.spells slot) the Magic
 * screen casts outside battle: the Cure spells heal, the Life spells revive. */
enum MenuFieldSpellId {
    MENU_SPELL_CURE = 0x00,
    MENU_SPELL_CURE2 = 0x01,
    MENU_SPELL_CURE3 = 0x02,
    MENU_SPELL_LIFE = 0x07,
    MENU_SPELL_LIFE2 = 0x08,
    MENU_SPELL_FULL_CURE = 0x33
};

/* The Limit screen's state (g_LimitMenuActivePane), also the index of the MenuCursor block from
 * g_LimitMenuCursor1 that drives it. */
enum MenuLimitScreenState {
    MENU_LIMIT_STATE_SET_SELECT_LEVEL = 0,   /* "Set BREAK LEVEL.": the level to switch to */
    MENU_LIMIT_STATE_COMMAND_BAR = 1,        /* Set / Check */
    MENU_LIMIT_STATE_CHECK_SELECT_LEVEL = 2,
    MENU_LIMIT_STATE_CHECK_ATTACKS = 3,      /* the level's learned attacks and their descriptions */
    MENU_LIMIT_STATE_SET_CONFIRM = 4         /* "Change BREAK LEVEL?" Yes / No */
};

/* Columns of the Limit screen's command bar (g_LimitMenuCursor2.col). */
enum MenuLimitCommand {
    MENU_LIMIT_COMMAND_SET = 0,
    MENU_LIMIT_COMMAND_CHECK = 1
};

/* Rows of the Limit screen's "Change BREAK LEVEL?" choice. */
enum MenuLimitConfirm {
    MENU_LIMIT_CONFIRM_YES = 0,
    MENU_LIMIT_CONFIRM_NO = 1
};

/* The menu's UI scaling mode (GetUiScalingMode, Menu_ApplyUiScalingViewportForMode's mode): a
 * 320x240 viewport at the origin or centred at (0xA0, 0x78), or the full 640x480 one; anything
 * but the first two is drawn at 640x480. */
enum MenuUiScalingMode {
    MENU_UI_SCALE_SMALL = 0,
    MENU_UI_SCALE_SMALL_CENTERED = 1,
    MENU_UI_SCALE_FULL = 2
};

/* The ability list a battle cost panel (Menu_DrawBattleAbilityCostPanel2x/1x, and the materia screen's
 * Menu_DrawMateriaActionCostPanel2x/1x) reads: the party member's magic, summon or enemy-skill slots. */
enum BattleAbilityListLayout {
    BATTLE_ABILITY_LIST_MAGIC = 0,
    BATTLE_ABILITY_LIST_SUMMON = 1,
    BATTLE_ABILITY_LIST_ENEMY_SKILL = 2
};

/* What the battle HUD's party rows show while a target is chosen (g_BattleSelectedUseCondition, the
 * selected attack's condition from Kernel_GetUseCondition): HP, MP or the status names. */
enum BattleUseCondition {
    BATTLE_USE_CONDITION_HP = 0,
    BATTLE_USE_CONDITION_MP = 1,
    BATTLE_USE_CONDITION_STATUS = 2
};

/* A Battle Square handicap (the high byte of a slot outcome word, Mini_ApplySlotPenalty), named by the
 * text the slot screen shows for it (g_BtlSquareHandicapTexts). */
enum BattleSquareHandicap {
    BATTLE_SQUARE_HANDICAP_BREAK_MAGIC_MATERIA = 0,
    BATTLE_SQUARE_HANDICAP_BREAK_SUMMON_MATERIA = 1,
    BATTLE_SQUARE_HANDICAP_BREAK_SUPPORT_MATERIA = 2,
    BATTLE_SQUARE_HANDICAP_BREAK_INDEPENDENT_MATERIA = 3,
    BATTLE_SQUARE_HANDICAP_BREAK_COMMAND_MATERIA = 4,
    BATTLE_SQUARE_HANDICAP_BREAK_ALL_MATERIA = 5,
    BATTLE_SQUARE_HANDICAP_BREAK_ACCESSORY = 6,
    BATTLE_SQUARE_HANDICAP_SEAL_ITEM_COMMAND = 7,
    BATTLE_SQUARE_HANDICAP_BREAK_ARMOR = 8,       /* defense halved */
    BATTLE_SQUARE_HANDICAP_BREAK_WEAPON = 9,      /* attack halved */
    BATTLE_SQUARE_HANDICAP_HALF_SPEED = 10,       /* dexterity halved */
    BATTLE_SQUARE_HANDICAP_HALF_ACCURACY = 11,
    BATTLE_SQUARE_HANDICAP_MINIMUM = 12,
    BATTLE_SQUARE_HANDICAP_POISON = 13,
    BATTLE_SQUARE_HANDICAP_TOAD = 14,
    BATTLE_SQUARE_HANDICAP_TIME_X30_DAMAGE = 15,
    BATTLE_SQUARE_HANDICAP_DOWN_5_LEVELS = 16,
    BATTLE_SQUARE_HANDICAP_DOWN_10_LEVELS = 17,
    BATTLE_SQUARE_HANDICAP_HALF_HP = 18,
    BATTLE_SQUARE_HANDICAP_HALF_MP = 19,
    BATTLE_SQUARE_HANDICAP_HALF_HP_MP = 20,
    BATTLE_SQUARE_HANDICAP_ZERO_MP = 21,
    BATTLE_SQUARE_HANDICAP_NONE = 22,             /* "Yesss! No handicapp!" */
    BATTLE_SQUARE_HANDICAP_HP_RESTORED = 23
};

/* g_BtlHandicapFlags: the handicaps the battle applies; bits 1-3 index g_BattleSquareHandicapStatusMasks. */
enum BattleSquareHandicapFlag {
    BATTLE_SQUARE_HANDICAP_FLAG_MINIMUM = 0x02,
    BATTLE_SQUARE_HANDICAP_FLAG_POISON = 0x04,
    BATTLE_SQUARE_HANDICAP_FLAG_TOAD = 0x08,
    BATTLE_SQUARE_HANDICAP_FLAG_HP_RESTORED = 0x10,
    BATTLE_SQUARE_HANDICAP_FLAG_ITEM_SEALED = 0x20
};

/* The colour index Menu_GetMateriaTypeColorIndex returns for a materia type, as the Battle Square's
 * "<type> Materia is broken." handicaps match it (Mini_SlotPenaltyMateriaColorMatches). */
enum MenuMateriaColor {
    MENU_MATERIA_COLOR_MAGIC = 0x4,
    MENU_MATERIA_COLOR_SUPPORT = 0x5,
    MENU_MATERIA_COLOR_COMMAND = 0x8,
    MENU_MATERIA_COLOR_INDEPENDENT = 0x9,
    MENU_MATERIA_COLOR_SUMMON = 0xA
};

/* The kernel text Menu_LookupEntryNameByMode reads a name from: a command's or a materia's. */
enum MenuMateriaNameSource {
    MENU_MATERIA_NAME_COMMAND = 0,
    MENU_MATERIA_NAME_MATERIA = 1
};

/* The rows of the materia screen's Arrange menu (g_MenuMateriaAbilitySelected). */
enum MenuMateriaArrangeCommand {
    MENU_MATERIA_ARRANGE_SORT = 0,       /* "Arrange": Menu_MateriaSortStock */
    MENU_MATERIA_ARRANGE_EXCHANGE = 1,
    MENU_MATERIA_ARRANGE_REMOVE_ALL = 2,
    MENU_MATERIA_ARRANGE_TRASH = 3
};

/* The pane the materia Exchange screen's cursor is in (g_MenuMateriaCursorParam1; the pane of the
 * picked-up entry is g_MenuMateriaSelectedSlot): a member's materia slots, the stock list, or the row
 * labels on the left (a whole member, or a weapon or armor row, MenuMateriaExchangeRow). */
enum MenuMateriaExchangePane {
    MENU_MATERIA_EXCHANGE_PANE_SLOTS = 0,
    MENU_MATERIA_EXCHANGE_PANE_STOCK = 1,
    MENU_MATERIA_EXCHANGE_PANE_ROWS = 2
};

/* The row label the Exchange screen's left pane points at (g_MenuMateriaSortOrder, and
 * g_MenuMateriaScrollTop once picked up there): every slot of the member, or one equipment row
 * (the slot pane's EquipKind + 1). */
enum MenuMateriaExchangeRow {
    MENU_MATERIA_EXCHANGE_ROW_ALL = 0,
    MENU_MATERIA_EXCHANGE_ROW_WEAPON = 1,
    MENU_MATERIA_EXCHANGE_ROW_ARMOR = 2
};

/* The Exchange screen's member-list scroll in progress (g_MenuMateriaCursorParam5); unlike
 * MenuCursorScroll, 1 scrolls down. */
enum MenuMateriaExchangeScroll {
    MENU_MATERIA_EXCHANGE_SCROLL_NONE = 0,
    MENU_MATERIA_EXCHANGE_SCROLL_DOWN = 1,
    MENU_MATERIA_EXCHANGE_SCROLL_UP = 2
};

/* Where the Equip screen's cursor is (g_MenuEquipSelectedSlot), also the index of the MenuCursor
 * block that drives it: the weapon/armour/accessory column, or the list of items to equip there. */
enum MenuEquipFocus {
    MENU_EQUIP_FOCUS_KIND = 0,                    /* g_MenuEquipCursor0: an EquipKind row */
    MENU_EQUIP_FOCUS_LIST = 1                     /* g_MenuEquipCursor1: the candidate items */
};

/* The Status screen's page (g_MenuStatusPage); Circle steps through them in order. */
enum MenuStatusPage {
    MENU_STATUS_PAGE_STATS = 0,                   /* stats, equipment and experience */
    MENU_STATUS_PAGE_ELEMENTS = 1,                /* element attack/halve/void/absorb rows */
    MENU_STATUS_PAGE_STATUSES = 2                 /* status attack/defend rows */
};

/* The rows of the Status screen's element page: the MenuPartyStats element mask each shows. */
enum MenuStatusElementRow {
    MENU_STATUS_ELEMENT_ROW_ATTACK = 0,           /* attackElements */
    MENU_STATUS_ELEMENT_ROW_HALVED = 1,           /* halvedElements */
    MENU_STATUS_ELEMENT_ROW_NULLIFIED = 2,        /* nullifiedElements */
    MENU_STATUS_ELEMENT_ROW_ABSORBED = 3          /* absorbedElements */
};

/* The rows of the Status screen's status page: the MenuPartyStats status mask each shows. */
enum MenuStatusEffectRow {
    MENU_STATUS_EFFECT_ROW_ATTACK = 0,            /* attackStatuses */
    MENU_STATUS_EFFECT_ROW_DEFENSE = 1            /* defenseStatuses */
};

/* The materia set of Savemap_CheckMateriaSetComplete (FIELD_REQUEST_CHECK_MATERIA_SET) and
 * Savemap_ExchangeForMasterMateria (FIELD_REQUEST_REPLACE_MATERIA_GROUP). */
enum SavemapMateriaSet {
    SAVEMAP_MATERIA_SET_MAGIC = 0,                /* g_MasterMagicMateriaIds, for MATERIA_MASTER_MAGIC */
    SAVEMAP_MATERIA_SET_SUMMON = 1,               /* g_MasterSummonMateriaIds, for MATERIA_MASTER_SUMMON */
    SAVEMAP_MATERIA_SET_COMMAND = 2,              /* g_MasterCommandMateriaIds, for MATERIA_MASTER_COMMAND */
    SAVEMAP_MATERIA_SET_BAHAMUT_ZERO = 3          /* Bahamut and Neo Bahamut held, for MATERIA_BAHAMUT_ZERO */
};

/* The 256-pixel quadrant of the doubled menu atlas a quad lies in, (x >= 256) << 1 | (y >= 256)
 * (Menu_DrawAtlasQuad2x); each has its own batches g_MenuWinA..D*GraphicsObject. */
enum MenuAtlasQuadrant {
    MENU_ATLAS_QUADRANT_TOP_LEFT = 0,             /* g_MenuWinA* */
    MENU_ATLAS_QUADRANT_BOTTOM_LEFT = 1,          /* g_MenuWinC* */
    MENU_ATLAS_QUADRANT_TOP_RIGHT = 2,            /* g_MenuWinB* */
    MENU_ATLAS_QUADRANT_BOTTOM_RIGHT = 3          /* g_MenuWinD* */
};

/* How Menu_DrawAtlasQuad2x splits a quad that crosses a quadrant boundary of the atlas. */
enum MenuAtlasSplit {
    MENU_ATLAS_SPLIT_X_TOP = 1,                   /* crosses x = 256 in the top half: A and B */
    MENU_ATLAS_SPLIT_X_BOTTOM = 2,                /* crosses x = 256 in the bottom half: C and D */
    MENU_ATLAS_SPLIT_Y_LEFT = 3,                  /* crosses y = 256 in the left half: A and C */
    MENU_ATLAS_SPLIT_Y_RIGHT = 4,                 /* crosses y = 256 in the right half: B and D */
    MENU_ATLAS_SPLIT_XY = 5                       /* crosses both: four pieces */
};

/* Direction of a MenuGauge's secondary segment (MenuGauge::direction, g_BtlGaugeHpDir/MpDir). */
enum MenuGaugeDirection {
    MENU_GAUGE_DIR_STEADY = 0,                    /* current and displayed agree; one segment */
    MENU_GAUGE_DIR_RISING = 1,                    /* rising toward current; green delta segment */
    MENU_GAUGE_DIR_FALLING = 2                    /* falling toward current; red delta segment */
};


enum CaitReelSymbol {
    CAIT_REEL_SYMBOL_CAIT_SITH = 0,
    CAIT_REEL_SYMBOL_BAR = 1,
    CAIT_REEL_SYMBOL_CROWN = 2,
    CAIT_REEL_SYMBOL_HEART = 3,
    CAIT_REEL_SYMBOL_MOOGLE = 4,
    CAIT_REEL_SYMBOL_STAR = 5
};

enum CaitReelColumn {
    CAIT_REEL_COL_LEFT = 0,
    CAIT_REEL_COL_MIDDLE = 1,
    CAIT_REEL_COL_RIGHT = 2
};

enum TifaReelSymbol {
    TIFA_REEL_SYMBOL_MISS = 0,
    TIFA_REEL_SYMBOL_HIT = 1,
    TIFA_REEL_SYMBOL_YEAH = 2
};

enum CharacterLimitLevel {
    CHAR_LIMIT_LEVEL_1 = 1,
    CHAR_LIMIT_LEVEL_2 = 2,
    CHAR_LIMIT_LEVEL_3 = 3,
    CHAR_LIMIT_LEVEL_4 = 4
};

enum PhsSplitPresetOffset {
    PHS_SPLIT_OFFSET_3PARTY_A = 0,
    PHS_SPLIT_OFFSET_3PARTY_B = 1,
    PHS_SPLIT_OFFSET_3PARTY_C = 2,
    PHS_SPLIT_OFFSET_3PARTY_D = 3,
    PHS_SPLIT_OFFSET_2PARTY = 4
};
#endif

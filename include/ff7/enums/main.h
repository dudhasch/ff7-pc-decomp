/* include/ff7/enums/main.h: named codes of ff7/main and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_MAIN_H
#define FF7_ENUMS_MAIN_H

/* Game modes held in g_EngineGameMode; GameModeDispatcher_Frame switches modules on them. */
enum GameMode {
    GAME_MODE_NONE = 0x00,                  /* unset entry mode; coerced to FIELD */
    GAME_MODE_FIELD = 0x01,
    GAME_MODE_BATTLE = 0x02,
    GAME_MODE_WORLD_MAP = 0x03,
    GAME_MODE_MENU = 0x05,
    GAME_MODE_HIGHWAY = 0x06,
    GAME_MODE_CHOCOBO_RACE = 0x07,
    GAME_MODE_SNOWBOARD = 0x08,
    GAME_MODE_FORT_CONDOR = 0x09,
    GAME_MODE_SUBMARINE = 0x0A,
    GAME_MODE_JET_COASTER = 0x0B,
    GAME_MODE_DISC_CHECK = 0x0C,
    GAME_MODE_FIELD_SERVICE = 0x0D,
    GAME_MODE_SNOWBOARD_2 = 0x0E,
    GAME_MODE_REENTER_FIELD = 0x10,
    GAME_MODE_MENU_AFTER_BATTLE = 0x11,     /* battle result screens */
    GAME_MODE_QUIT = 0x13,
    GAME_MODE_TITLE_MENU = 0x14,
    GAME_MODE_CONTINUE_GAME = 0x15,
    GAME_MODE_NEW_GAME = 0x16,
    GAME_MODE_BATTLE_SWIRL = 0x17,
    GAME_MODE_UNUSED_18 = 0x18,             /* guarded in App_AllowCtrlQQuit; never entered */
    GAME_MODE_ENDING_MOVIE = 0x19,
    GAME_MODE_GAME_OVER = 0x1A,
    GAME_MODE_CREDITS = 0x1B,
    GAME_MODE_ENDING_CREDITS = 0x1C
};

/* Field requests in FieldState.requestCode posted by field script opcodes; Field_UpdateLogic maps them to game modes. */
enum FieldRequestCode {
    FIELD_REQUEST_NONE = 0x00,
    FIELD_REQUEST_MAP_JUMP = 0x01,
    FIELD_REQUEST_BATTLE = 0x02,
    FIELD_REQUEST_LOAD_MOVIE = 0x03,
    FIELD_REQUEST_PLAY_MOVIE = 0x04,
    FIELD_REQUEST_ENDING_MOVIE = 0x05,
    FIELD_REQUEST_NAME_ENTRY = 0x06,
    FIELD_REQUEST_PARTY_SELECT = 0x07,
    FIELD_REQUEST_SHOP = 0x08,
    FIELD_REQUEST_MAIN_MENU = 0x09,
    FIELD_REQUEST_WORLD_ENDING_MOVIE = 0x0A,
    FIELD_REQUEST_MINIGAME = 0x0C,
    FIELD_REQUEST_DISC_CHANGE = 0x0D,
    FIELD_REQUEST_SAVE_MENU = 0x0E,
    FIELD_REQUEST_HARVEST_MATERIA = 0x0F,
    FIELD_REQUEST_RETURN_MATERIA = 0x10,
    FIELD_REQUEST_STRIP_CHARACTER = 0x11,
    FIELD_REQUEST_BACKUP_PARTY_EQUIP = 0x12,
    FIELD_REQUEST_RESTORE_PARTY_EQUIP = 0x13,
    FIELD_REQUEST_STOP_MOVIE = 0x14,
    FIELD_REQUEST_SCALE_FIRST_FIVE_HP = 0x15,
    FIELD_REQUEST_CHECK_MATERIA_SET = 0x16,
    FIELD_REQUEST_REPLACE_MATERIA_GROUP = 0x17,
    FIELD_REQUEST_SNAPSHOT_LEVELS = 0x18,
    FIELD_REQUEST_REENTER_FIELD = 0x19,
    FIELD_REQUEST_GAME_OVER = 0x1A
};

/* FieldState.requestArg while a request is posted: the opcode posts it PENDING, the module that
 * serves it reports RUNNING and DONE (Field_ServiceMovieRequest), and the waiting opcode clears
 * the request when it reads DONE. */
enum FieldRequestProgress {
    FIELD_REQUEST_PROGRESS_PENDING = 0,
    FIELD_REQUEST_PROGRESS_RUNNING = 1,
    FIELD_REQUEST_PROGRESS_DONE = 2
};

/* The battle's flag word (g_BattleResultWord): what the field or world map asks for
 * (Battle_SetupEntryFromField ORs g_FieldEncounterState into the script's flags) and what the battle
 * returns (the phase machine sets COUNTDOWN when the countdown ran out, the end sequence tests LOST and
 * ESCAPED). */
enum FieldBattleFlag {
    FIELD_BATTLE_FLAG_LOST = 0x01,       /* the party lost: the field posts FIELD_REQUEST_GAME_OVER */
    FIELD_BATTLE_FLAG_COUNTDOWN = 0x02,  /* a timed battle (world request bit 0x20000000): ends at countdown 0 */
    FIELD_BATTLE_FLAG_PREEMPTIVE = 0x04, /* the encounter's pre-emptive roll succeeded (world request bit 31) */
    FIELD_BATTLE_FLAG_ESCAPED = 0x08,    /* the party escaped: gSavemap.escapesLo/escapesHi counts it */
    FIELD_BATTLE_FLAG_BATTLE_SQUARE = 0x40 /* a Battle Square battle: Battle_LoadSceneRecord scales the enemies,
                                              fleeing counts as a defeat, the arena chooses the next battle */
};

/* The minigame a field script's MINIGAME opcode starts: operand byte 10, kept in byte 0 of
 * FieldState.backgroundLayerVisibility (minigameKind); Field_UpdateLogic maps it to a game mode. */
enum FieldMinigame {
    FIELD_MINIGAME_HIGHWAY = 0x00,
    FIELD_MINIGAME_CHOCOBO_RACE = 0x01,
    FIELD_MINIGAME_SNOWBOARD = 0x02,
    FIELD_MINIGAME_FORT_CONDOR = 0x03,
    FIELD_MINIGAME_SUBMARINE = 0x04,
    FIELD_MINIGAME_JET_COASTER = 0x05,
    FIELD_MINIGAME_SNOWBOARD_2 = 0x06
};

/* FF7 text bytes: control codes and FE-prefixed sub-codes that Field_WindowStepTextReveal switches on. */
enum TextCode {
    TEXT_FE_COLOR_0 = 0xD2,
    TEXT_FE_COLOR_1 = 0xD3,
    TEXT_FE_COLOR_2 = 0xD4,
    TEXT_FE_COLOR_3 = 0xD5,
    TEXT_FE_COLOR_4 = 0xD6,
    TEXT_FE_COLOR_5 = 0xD7,
    TEXT_FE_COLOR_6 = 0xD8,
    TEXT_FE_COLOR_7 = 0xD9,
    TEXT_FE_BLINK = 0xDA,
    TEXT_FE_RAINBOW = 0xDB,
    TEXT_FE_PAUSE = 0xDC,
    TEXT_FE_WAIT = 0xDD,             /* 2 operand bytes: frames */
    TEXT_FE_NUM_DECIMAL = 0xDE,
    TEXT_FE_NUM_HEX = 0xDF,
    TEXT_FE_WAIT_SCROLL_PAGE = 0xE0, /* field and world message windows: wait for confirm, then scroll the page out */
    TEXT_FE_NUM_DECIMAL_PADDED = 0xE1, /* field and world message windows: five digits, leading zeros padded */
    TEXT_FE_BANK_STRING = 0xE2,      /* field and world message windows: 16-bit bank offset and length, then a string copied from the script banks */
    TEXT_FE_FIXED_PITCH = 0xE9,      /* toggles the fixed-pitch text mode (message windows) */
    TEXT_WIDE_TAB = 0xE0,            /* ten spaces */
    TEXT_TAB = 0xE1,                 /* four spaces */
    TEXT_COMMA_SPACE = 0xE2,         /* ", " */
    TEXT_PERIOD_QUOTE = 0xE3,
    TEXT_ELLIPSIS_QUOTE = 0xE4,
    TEXT_NEWLINE = 0xE7,
    TEXT_NEW_PAGE = 0xE8,
    TEXT_NEW_PAGE_ALT = 0xE9,        /* message windows end the page on it as on TEXT_NEW_PAGE */
    TEXT_NAME_CLOUD = 0xEA,
    TEXT_NAME_BARRET = 0xEB,
    TEXT_NAME_TIFA = 0xEC,
    TEXT_NAME_AERIS = 0xED,
    TEXT_NAME_RED_XIII = 0xEE,
    TEXT_NAME_YUFFIE = 0xEF,
    TEXT_NAME_CAIT_SITH = 0xF0,
    TEXT_NAME_VINCENT = 0xF1,
    TEXT_NAME_CID = 0xF2,
    TEXT_PARTY_1 = 0xF3,
    TEXT_PARTY_2 = 0xF4,
    TEXT_PARTY_3 = 0xF5,
    TEXT_WINDOW_ICON_FIRST = 0xF6,   /* message windows draw 0xF6..0xF9 as four icons of the window atlas */
    TEXT_WINDOW_ICON_LAST = 0xF9,
    TEXT_BATTLE_EMPHASIS = 0xF8,     /* battle window text; 1 operand byte */
    TEXT_BACKREF = 0xF9,             /* kernel text: 1 operand byte, a back-reference (Kernel_ExpandTextBackrefs) */
    TEXT_FONT_PAGE_1 = 0xFA,         /* next byte is the glyph */
    TEXT_FONT_PAGE_2 = 0xFB,         /* next byte is the glyph */
    TEXT_FONT_PAGE_3 = 0xFC,         /* next byte is the glyph */
    TEXT_FONT_PAGE_4 = 0xFD,         /* next byte is the glyph */
    TEXT_EXTENDED = 0xFE,
    TEXT_END = 0xFF
};

/* Battle text escapes: the byte 0xEA + this, then a big-endian 16-bit operand, which
 * Kernel_FormatBattleText replaces with the text it names. */
enum TextBattleEscape {
    TEXT_BATTLE_ESC_CHARACTER_NAME = 0,  /* the name of character id operand */
    TEXT_BATTLE_ESC_ITEM_NAME = 1,       /* KERNEL_TEXT_ITEM entry operand */
    TEXT_BATTLE_ESC_NUMBER = 2,          /* operand in decimal */
    TEXT_BATTLE_ESC_COMBATANT_NAME = 3,  /* party slot 0..2 or enemy 4.. of the battle scene */
    TEXT_BATTLE_ESC_MAGIC_NAME = 4,      /* kernel text bank 9 entry operand */
    TEXT_BATTLE_ESC_GLYPH = 5,           /* one glyph, textFormatBase + operand (below 26) */
    TEXT_BATTLE_ESC_BATTLE_MESSAGE = 6,  /* kernel text bank 0x10 (battle messages) entry operand */
    TEXT_BATTLE_ESC_BANK_ENTRY = 7       /* kernel text bank operand >> 8, entry operand & 0xFF */
};


/* The registry Graphics\Mode value GetUiScalingMode returns (2 when it is missing): the screen the
 * modules lay out for (Submarine_InitStateAndTables). */
enum MainGraphicsMode {
    MAIN_GRAPHICS_MODE_320X240 = 0,
    MAIN_GRAPHICS_MODE_320X240_CENTERED = 1,  /* 320 x 240 in the middle of a 640 x 480 screen */
    MAIN_GRAPHICS_MODE_640X480 = 2
};

/* The registry Graphics\Driver value (MainGraphicsConfig.driver): the GfxDriverType
 * ApplyConfigToContext installs. */
enum MainRegistryDriver {
    MAIN_REGISTRY_DRIVER_SOFTWARE = 0,
    MAIN_REGISTRY_DRIVER_DIRECT3D = 1,
    MAIN_REGISTRY_DRIVER_OPENGL = 2,
    MAIN_REGISTRY_DRIVER_DLL = 3              /* the driver DLL at Graphics\DriverPath */
};

/* Packed bits of g_WorldBattleRequest above the low 30-bit battle / field encounter ID
 * (bit 31, 0x80000000, marks a pre-emptive roll when entering a battle). */
enum WorldBattleRequestFlag {
    WORLD_BATTLE_REQ_COUNTDOWN = 0x20000000,   /* timed battle: sets FIELD_BATTLE_FLAG_COUNTDOWN */
    WORLD_BATTLE_REQ_ENTER_FIELD = 0x40000000, /* after battle, return to GAME_MODE_FIELD instead of world map */
    WORLD_BATTLE_REQ_ID_MASK = 0x3FFFFFFF
};


enum PartyCharacterId {
    PARTY_CHAR_CLOUD = 0,
    PARTY_CHAR_BARRET = 1,
    PARTY_CHAR_TIFA = 2,
    PARTY_CHAR_AERIS = 3,
    PARTY_CHAR_RED_XIII = 4,
    PARTY_CHAR_YUFFIE = 5,
    PARTY_CHAR_CAIT_SITH = 6,
    PARTY_CHAR_VINCENT = 7,
    PARTY_CHAR_CID = 8,
    PARTY_CHAR_YOUNG_CLOUD = 9,
    PARTY_CHAR_SEPHIROTH = 10,
    PARTY_CHAR_CHOCOBO = 11,
    PARTY_CHAR_NONE = 0xFF
};

enum BattlePartySlotIndex {
    BATTLE_PARTY_SLOT_0 = 0,
    BATTLE_PARTY_SLOT_1 = 1,
    BATTLE_PARTY_SLOT_2 = 2
};

enum TextWindowIconOffset {
    TEXT_ICON_OFFSET_CIRCLE = 0,
    TEXT_ICON_OFFSET_TRIANGLE = 1,
    TEXT_ICON_OFFSET_SQUARE = 2,
    TEXT_ICON_OFFSET_CROSS = 3
};

/* RegistryConfig::isEnabled flag in ApplyConfigToContext. */
enum RegistryModeToggle {
    REG_MODE_ENABLED = 0,
    REG_MODE_DISABLED = 1
};
#endif

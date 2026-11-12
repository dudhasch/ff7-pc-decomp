/* include/ff7/enums/field.h: named codes of ff7/field and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_FIELD_H
#define FF7_ENUMS_FIELD_H

/* Message window state machine (window +0x2C), run by Field_WindowShowMessage and WorldMsg_DispatchWindowState. */
enum MessageWindowState {
    MSG_WINDOW_CLOSED = 0x00,
    MSG_WINDOW_OPENING = 0x01,
    MSG_WINDOW_TEXT_REVEAL = 0x02,
    MSG_WINDOW_WAIT_DELAY = 0x03,
    MSG_WINDOW_WAIT_INPUT_SCROLL_ROW = 0x04,
    MSG_WINDOW_CLOSING_ALT = 0x05,                /* no writer found in src; handled exactly as 7 */
    MSG_WINDOW_TEXT_DONE = 0x06,                  /* waits confirm; choice input runs here */
    MSG_WINDOW_CLOSING = 0x07,
    MSG_WINDOW_ROW_SCROLL = 0x08,
    MSG_WINDOW_PAGE_CLEAR = 0x09,
    MSG_WINDOW_WAIT_INPUT_SCROLL_PAGE = 0x0B,
    MSG_WINDOW_PAGE_SCROLL_OUT = 0x0C,
    MSG_WINDOW_WAIT_INPUT_RESUME = 0x0D,
    MSG_WINDOW_WAIT_INPUT_CLEAR_PAGE = 0x0E
};

/* A message window's mode bits (WindowData.preventClose, set by Field_WindowSetMode): NO_PLAYER_CLOSE
 * keeps a finished window open when the player confirms, INSTANT_TEXT reveals the text at once. */
enum MessageWindowMode {
    MSG_WINDOW_MODE_NO_PLAYER_CLOSE = 0x01,
    MSG_WINDOW_MODE_INSTANT_TEXT = 0x02
};

/* Field script operand bank nibbles (literal, savemap pages, temp bank), decoded by Script_ReadOperand8/16. */
enum FieldScriptBank {
    FIELD_BANK_LITERAL = 0x00,
    FIELD_BANK_SAVE_000_BYTE = 0x01,
    FIELD_BANK_SAVE_000_WORD = 0x02,
    FIELD_BANK_SAVE_100_BYTE = 0x03,
    FIELD_BANK_SAVE_100_WORD = 0x04,
    FIELD_BANK_TEMP_BYTE = 0x05,
    FIELD_BANK_TEMP_WORD = 0x06,
    FIELD_BANK_SAVE_400_WORD = 0x07,     /* word partner of 15 */
    FIELD_BANK_SAVE_200_BYTE = 0x0B,
    FIELD_BANK_SAVE_200_WORD = 0x0C,
    FIELD_BANK_SAVE_300_BYTE = 0x0D,
    FIELD_BANK_SAVE_300_WORD = 0x0E,
    FIELD_BANK_SAVE_400_BYTE = 0x0F
};

/* Field script IF comparison/bit-test operators, evaluated by Script_EvalCompare8 and Script_EvalCompare16*. */
enum FieldCompareOp {
    FIELD_COMPARE_EQUAL = 0x00,
    FIELD_COMPARE_NOT_EQUAL = 0x01,
    FIELD_COMPARE_GREATER = 0x02,
    FIELD_COMPARE_LESS = 0x03,
    FIELD_COMPARE_GREATER_EQUAL = 0x04,
    FIELD_COMPARE_LESS_EQUAL = 0x05,
    FIELD_COMPARE_AND = 0x06,
    FIELD_COMPARE_XOR = 0x07,
    FIELD_COMPARE_OR = 0x08,
    FIELD_COMPARE_BIT_SET = 0x09,
    FIELD_COMPARE_BIT_CLEAR = 0x0A
};

/* Field script opcodes: the index of each handler in g_FieldScriptOpTable, the table
   Script_ExecuteFrame and Script_RunAllInitScripts dispatch through. A handler
   at several indices takes its index as a suffix; Script_OpUnimplementedStall runs the 13 opcodes
   the PC port leaves out. */
enum FieldOpcode {
    FIELD_OP_RETURN = 0x00,
    FIELD_OP_REQ = 0x01,
    FIELD_OP_REQ_SYNC_WAIT = 0x02,
    FIELD_OP_REQ_END_WAIT = 0x03,
    FIELD_OP_PARTY_REQ = 0x04,
    FIELD_OP_PARTY_REQ_SYNC_WAIT = 0x05,
    FIELD_OP_PARTY_REQ_END_WAIT = 0x06,
    FIELD_OP_RETURN_TO = 0x07,
    FIELD_OP_PARTY_JOIN_LEADER = 0x08,
    FIELD_OP_PARTY_SPLIT_TO = 0x09,
    FIELD_OP_SET_FIELD_PARTY = 0x0A,
    FIELD_OP_GET_FIELD_PARTY = 0x0B,
    FIELD_OP_UNIMPLEMENTED_STALL_0C = 0x0C,
    FIELD_OP_UNIMPLEMENTED_STALL_0D = 0x0D,
    FIELD_OP_REQUEST_DISC_CHANGE = 0x0E,
    FIELD_OP_SPECIAL = 0x0F,
    FIELD_OP_JUMP_FORWARD = 0x10,
    FIELD_OP_JUMP_FORWARD_LONG = 0x11,
    FIELD_OP_JUMP_BACK = 0x12,
    FIELD_OP_JUMP_BACK_LONG = 0x13,
    FIELD_OP_IF_BYTE = 0x14,
    FIELD_OP_IF_BYTE_LONG = 0x15,
    FIELD_OP_IF_WORD_SIGNED = 0x16,
    FIELD_OP_IF_WORD_SIGNED_LONG = 0x17,
    FIELD_OP_IF_WORD_UNSIGNED = 0x18,
    FIELD_OP_IF_WORD_UNSIGNED_LONG = 0x19,
    FIELD_OP_UNIMPLEMENTED_STALL_1A = 0x1A,
    FIELD_OP_UNIMPLEMENTED_STALL_1B = 0x1B,
    FIELD_OP_UNIMPLEMENTED_STALL_1C = 0x1C,
    FIELD_OP_UNIMPLEMENTED_STALL_1D = 0x1D,
    FIELD_OP_UNIMPLEMENTED_STALL_1E = 0x1E,
    FIELD_OP_UNIMPLEMENTED_STALL_1F = 0x1F,
    FIELD_OP_MINIGAME = 0x20,
    FIELD_OP_MENU = 0x21,
    FIELD_OP_SET_BATTLE_FLAGS_WORD = 0x22,
    FIELD_OP_GET_BATTLE_RESULT = 0x23,
    FIELD_OP_WAIT_FRAMES = 0x24,
    FIELD_OP_FADE_PARAMS = 0x25,
    FIELD_OP_SET_MODEL_BLINK = 0x26,
    FIELD_OP_SET_SPEED_BOOST = 0x27,
    FIELD_OP_MODEL_GFX_COMMAND = 0x28,
    FIELD_OP_WAIT_MODEL_GFX_COMMAND = 0x29,
    FIELD_OP_MOVE_TO_PARTY_MEMBER = 0x2A,
    FIELD_OP_SET_LINE_FLAG_16 = 0x2B,
    FIELD_OP_SET_LAYER_DEPTH = 0x2C,
    FIELD_OP_SET_LAYER_SCROLL = 0x2D,
    FIELD_OP_CLOSE_WINDOW_AND_WAIT = 0x2E,
    FIELD_OP_REOPEN_WINDOW = 0x2F,
    FIELD_OP_IF_BUTTON_HELD = 0x30,
    FIELD_OP_IF_BUTTON_PRESSED = 0x31,
    FIELD_OP_IF_BUTTON_RELEASED = 0x32,
    FIELD_OP_SET_DIALOGUE_LOCK = 0x33,
    FIELD_OP_FACE_PARTY_MEMBER = 0x34,
    FIELD_OP_TURN_TO_PARTY_MEMBER = 0x35,
    FIELD_OP_WINDOW_SET_NUMBER_DISPLAY = 0x36,
    FIELD_OP_WINDOW_SET_NUMBER = 0x37,
    FIELD_OP_SET_COUNTDOWN = 0x38,
    FIELD_OP_ADD_GIL = 0x39,
    FIELD_OP_SPEND_GIL = 0x3A,
    FIELD_OP_GET_GIL = 0x3B,
    FIELD_OP_FULL_HEAL_3C = 0x3C,
    FIELD_OP_FULL_HEAL_3D = 0x3D,
    FIELD_OP_HEAL_EVERYONE = 0x3E,
    FIELD_OP_FULL_HEAL_3F = 0x3F,
    FIELD_OP_SHOW_MESSAGE = 0x40,
    FIELD_OP_WINDOW_BIND_PARAM_8 = 0x41,
    FIELD_OP_WINDOW_BIND_PARAM_16 = 0x42,
    FIELD_OP_SET_LOCATION_NAME = 0x43,
    FIELD_OP_UNIMPLEMENTED_STALL_44 = 0x44,
    FIELD_OP_HEAL_MP = 0x45,
    FIELD_OP_UNIMPLEMENTED_STALL_46 = 0x46,
    FIELD_OP_DAMAGE_MP = 0x47,
    FIELD_OP_ASK_CHOICE = 0x48,
    FIELD_OP_FIELD_REQUEST = 0x49,
    FIELD_OP_SET_MENU_LOCKED = 0x4A,
    FIELD_OP_SET_ENCOUNTER_TABLE = 0x4B,
    FIELD_OP_UNIMPLEMENTED_STALL_4C = 0x4C,
    FIELD_OP_HEAL_HP = 0x4D,
    FIELD_OP_UNIMPLEMENTED_STALL_4E = 0x4E,
    FIELD_OP_DAMAGE_HP = 0x4F,
    FIELD_OP_WINDOW_CREATE = 0x50,
    FIELD_OP_WINDOW_RESIZE = 0x51,
    FIELD_OP_WINDOW_SET_MODE = 0x52,
    FIELD_OP_WINDOW_CLOSE = 0x53,
    FIELD_OP_WINDOW_WAIT_CLOSED = 0x54,
    FIELD_OP_WINDOW_SET_ROWS = 0x55,
    FIELD_OP_GET_WINDOW_COLOUR = 0x56,
    FIELD_OP_SET_WINDOW_COLOUR = 0x57,
    FIELD_OP_ADD_ITEM = 0x58,
    FIELD_OP_REMOVE_ITEM = 0x59,
    FIELD_OP_GET_ITEM_QTY = 0x5A,
    FIELD_OP_ADD_MATERIA = 0x5B,
    FIELD_OP_REMOVE_MATERIA = 0x5C,
    FIELD_OP_COUNT_MATERIA = 0x5D,
    FIELD_OP_SHAKE = 0x5E,
    FIELD_OP_YIELD_ONE_OPCODE = 0x5F,
    FIELD_OP_MAP_JUMP = 0x60,
    FIELD_OP_SET_UNUSED_FLAG_37 = 0x61,
    FIELD_OP_SCROLL_TO_PLAYER = 0x62,
    FIELD_OP_SCROLL_TO_ENTITY = 0x63,
    FIELD_OP_SCROLL_TO_POINT = 0x64,
    FIELD_OP_SCROLL_RESET = 0x65,
    FIELD_OP_SCROLL_TO_POINT_LINEAR = 0x66,
    FIELD_OP_WAIT_SCROLL = 0x67,
    FIELD_OP_SCROLL_TO_POINT_SMOOTH = 0x68,
    FIELD_OP_SET_FIELD_HIDDEN = 0x69,
    FIELD_OP_SET_VIEW_OFFSET = 0x6A,
    FIELD_OP_SCREEN_FADE = 0x6B,
    FIELD_OP_WAIT_FADE = 0x6C,
    FIELD_OP_SET_ACCESS_BLOCKED = 0x6D,
    FIELD_OP_GET_FIELD_MAP_ID = 0x6E,
    FIELD_OP_SCROLL_TO_PARTY_MEMBER = 0x6F,
    FIELD_OP_ENTER_BATTLE = 0x70,
    FIELD_OP_SET_ENCOUNTERS_DISABLED = 0x71,
    FIELD_OP_SET_BATTLE_FLAGS_BYTE = 0x72,
    FIELD_OP_GET_PARTY_DIRECTION = 0x73,
    FIELD_OP_GET_PARTY_CHAR_ID = 0x74,
    FIELD_OP_GET_PARTY_XYZI = 0x75,
    FIELD_OP_ADD_BYTE_SAT = 0x76,
    FIELD_OP_ADD_WORD_SAT = 0x77,
    FIELD_OP_SUB_BYTE_SAT = 0x78,
    FIELD_OP_SUB_WORD_SAT = 0x79,
    FIELD_OP_INC_BYTE_SAT = 0x7A,
    FIELD_OP_INC_WORD_SAT = 0x7B,
    FIELD_OP_DEC_BYTE_SAT = 0x7C,
    FIELD_OP_DEC_WORD_SAT = 0x7D,
    FIELD_OP_SET_TALK_OFF = 0x7E,
    FIELD_OP_SET_RANDOM_STRIDE = 0x7F,
    FIELD_OP_SET_BYTE = 0x80,
    FIELD_OP_SET_WORD = 0x81,
    FIELD_OP_BIT_SET = 0x82,
    FIELD_OP_BIT_CLEAR = 0x83,
    FIELD_OP_BIT_TOGGLE = 0x84,
    FIELD_OP_ADD_BYTE = 0x85,
    FIELD_OP_ADD_WORD = 0x86,
    FIELD_OP_SUB_BYTE = 0x87,
    FIELD_OP_SUB_WORD = 0x88,
    FIELD_OP_MUL_BYTE = 0x89,
    FIELD_OP_MUL_WORD = 0x8A,
    FIELD_OP_DIV_BYTE = 0x8B,
    FIELD_OP_DIV_WORD = 0x8C,
    FIELD_OP_MOD_BYTE = 0x8D,
    FIELD_OP_MOD_WORD = 0x8E,
    FIELD_OP_AND_BYTE = 0x8F,
    FIELD_OP_AND_WORD = 0x90,
    FIELD_OP_OR_BYTE = 0x91,
    FIELD_OP_OR_WORD = 0x92,
    FIELD_OP_XOR_BYTE = 0x93,
    FIELD_OP_XOR_WORD = 0x94,
    FIELD_OP_INC_BYTE = 0x95,
    FIELD_OP_INC_WORD = 0x96,
    FIELD_OP_DEC_BYTE = 0x97,
    FIELD_OP_DEC_WORD = 0x98,
    FIELD_OP_RANDOM_BYTE = 0x99,
    FIELD_OP_SET_BYTE_ALIAS = 0x9A,
    FIELD_OP_GET_HIGH_BYTE = 0x9B,
    FIELD_OP_SET_WORD_FROM_BYTES = 0x9C,
    FIELD_OP_STORE_BYTE_INDEXED = 0x9D,
    FIELD_OP_LOAD_BYTE_INDEXED = 0x9E,
    FIELD_OP_SEARCH_BANK_FOR_BYTE = 0x9F,
    FIELD_OP_BIND_CHARACTER = 0xA0,
    FIELD_OP_ATTACH_MODEL = 0xA1,
    FIELD_OP_SET_IDLE_ANIMATION = 0xA2,
    FIELD_OP_ANIMATE_AND_WAIT_A3 = 0xA3,
    FIELD_OP_SET_VISIBLE = 0xA4,
    FIELD_OP_SET_POSITION_XYZI = 0xA5,
    FIELD_OP_SET_POSITION_XYI = 0xA6,
    FIELD_OP_SET_POSITION_XYZ = 0xA7,
    FIELD_OP_MOVE_TO = 0xA8,
    FIELD_OP_MOVE_TO_STYLED = 0xA9,
    FIELD_OP_MOVE_TO_ENTITY = 0xAA,
    FIELD_OP_TURN_TO_ENTITY = 0xAB,
    FIELD_OP_WAIT_ANIMATION_DONE = 0xAC,
    FIELD_OP_MOVE_TO_DIRECT = 0xAD,
    FIELD_OP_ANIMATE_AND_WAIT_AE = 0xAE,
    FIELD_OP_ANIMATE_AND_WAIT_ALT_AF = 0xAF,
    FIELD_OP_PLAY_ANIMATION_B0 = 0xB0,
    FIELD_OP_PLAY_ANIMATION_FROM_B1 = 0xB1,
    FIELD_OP_SET_MOVE_SPEED = 0xB2,
    FIELD_OP_SET_DIRECTION = 0xB3,
    FIELD_OP_TURN_START = 0xB4,
    FIELD_OP_TURN_TO_ANGLE = 0xB5,
    FIELD_OP_FACE_ENTITY = 0xB6,
    FIELD_OP_GET_ENTITY_DIRECTION = 0xB7,
    FIELD_OP_GET_ENTITY_XY = 0xB8,
    FIELD_OP_GET_ENTITY_TRIANGLE = 0xB9,
    FIELD_OP_ANIMATE_AND_WAIT_ALT_BA = 0xBA,
    FIELD_OP_PLAY_ANIMATION_BB = 0xBB,
    FIELD_OP_PLAY_ANIMATION_FROM_BC = 0xBC,
    FIELD_OP_SET_ANIMATION_SPEED = 0xBD,
    FIELD_OP_UNIMPLEMENTED_STALL_BE = 0xBE,
    FIELD_OP_SET_PLAYER_ENTITY = 0xBF,
    FIELD_OP_MOVE_TO_3D = 0xC0,
    FIELD_OP_GET_ENTITY_XYZI = 0xC1,
    FIELD_OP_MOVE_TO_MODE = 0xC2,
    FIELD_OP_JUMP_TO = 0xC3,
    FIELD_OP_WAIT_OFFSET_DONE = 0xC4,
    FIELD_OP_SET_COLLISION_RADIUS = 0xC5,
    FIELD_OP_SET_TALK_RADIUS = 0xC6,
    FIELD_OP_SET_SOLID = 0xC7,
    FIELD_OP_PARTY_ADD = 0xC8,
    FIELD_OP_PARTY_REMOVE = 0xC9,
    FIELD_OP_PARTY_SET = 0xCA,
    FIELD_OP_IF_CHAR_IN_PARTY = 0xCB,
    FIELD_OP_IF_CHAR_AVAILABLE = 0xCC,
    FIELD_OP_SET_CHAR_AVAILABLE = 0xCD,
    FIELD_OP_MEMBER_LOCK = 0xCE,
    FIELD_OP_MEMBER_UNLOCK = 0xCF,
    FIELD_OP_DEFINE_LINE = 0xD0,
    FIELD_OP_SET_LINE_ENABLED = 0xD1,
    FIELD_OP_SET_GATEWAYS_DISABLED = 0xD2,
    FIELD_OP_SET_LINE_POINTS = 0xD3,
    FIELD_OP_SIN_SCALED = 0xD4,
    FIELD_OP_COS_SCALED = 0xD5,
    FIELD_OP_SET_COLLISION_RADIUS_W = 0xD6,
    FIELD_OP_SET_TALK_RADIUS_W = 0xD7,
    FIELD_OP_SET_PRELOAD_MAP_ID = 0xD8,
    FIELD_OP_WAIT_PRELOAD_DONE = 0xD9,
    FIELD_OP_SOUND_COMMAND_LONG = 0xDA,
    FIELD_OP_SET_MOVE_STYLE = 0xDB,
    FIELD_OP_SET_STANDARD_ANIMATION = 0xDC,
    FIELD_OP_STOP_ANIMATION = 0xDD,
    FIELD_OP_WAIT_TURN_DONE = 0xDE,
    FIELD_OP_PALETTE_MULTIPLY_RANGE = 0xDF,
    FIELD_OP_BG_ON = 0xE0,
    FIELD_OP_BG_OFF = 0xE1,
    FIELD_OP_BG_ROL = 0xE2,
    FIELD_OP_BG_ROL_2 = 0xE3,
    FIELD_OP_BG_CLR = 0xE4,
    FIELD_OP_PALETTE_STORE = 0xE5,
    FIELD_OP_PALETTE_LOAD = 0xE6,
    FIELD_OP_PALETTE_COPY = 0xE7,
    FIELD_OP_PALETTE_ROTATE = 0xE8,
    FIELD_OP_PALETTE_ADD = 0xE9,
    FIELD_OP_PALETTE_MULTIPLY = 0xEA,
    FIELD_OP_PALETTE_STORE_PARTIAL = 0xEB,
    FIELD_OP_PALETTE_LOAD_PARTIAL = 0xEC,
    FIELD_OP_PALETTE_COPY_RANGE = 0xED,
    FIELD_OP_PALETTE_ROTATE_RANGE = 0xEE,
    FIELD_OP_PALETTE_ADD_RANGE = 0xEF,
    FIELD_OP_SUBMIT_MUSIC_REQUEST_10 = 0xF0,
    FIELD_OP_SOUND_EFFECT = 0xF1,
    FIELD_OP_SOUND_COMMAND = 0xF2,
    FIELD_OP_SUBMIT_MUSIC_REQUEST_14 = 0xF3,
    FIELD_OP_SUBMIT_MUSIC_REQUEST_15 = 0xF4,
    FIELD_OP_SET_AUDIO_DISABLED = 0xF5,
    FIELD_OP_SET_MUSIC_ID = 0xF6,
    FIELD_OP_GET_FIELD_FX_PARAMS = 0xF7,
    FIELD_OP_MOVIE_OPEN = 0xF8,
    FIELD_OP_MOVIE_PLAY = 0xF9,
    FIELD_OP_GET_MOVIE_FRAME = 0xFA,
    FIELD_OP_SET_MOVIE_CAMERA_DISABLED = 0xFB,
    FIELD_OP_SET_MUSIC_ID_ALT = 0xFC,
    FIELD_OP_SUBMIT_REQUEST_WITH_ARGS = 0xFD,
    FIELD_OP_GET_MUSIC_PLAYING = 0xFE,
    FIELD_OP_FIELD_REQUEST_1A = 0xFF
};

/* The entity scripts the field script VM starts for the player's interactions
 * (Script_TriggerEntityScript's script number, from Script_ExecuteFrame; script 0 is the entity's
 * init and main loop). Each is named by the request that triggers it: a model's or a line's
 * requestTalkScript and requestPushScript, a line's across, touch (every frame it is set),
 * requestTouchOnScript and requestTouchOffScript. */
enum FieldEntityScript {
    FIELD_SCRIPT_TALK = 1,
    FIELD_SCRIPT_PUSH = 2,
    FIELD_SCRIPT_ACROSS = 3,
    FIELD_SCRIPT_TOUCH = 4,
    FIELD_SCRIPT_TOUCH_ON = 5,
    FIELD_SCRIPT_TOUCH_OFF = 6
};

/* The first two bytes of a head mesh command (Field_ApplyHeadMeshCommands): the texture of the first
 * drawn group of the model's "head" mesh and of the group after it. The blink loop shows CLOSED for
 * one frame and OPEN otherwise; any other value turns the group states off. */
enum FieldHeadMeshCommand {
    FIELD_HEAD_MESH_EYE_OPEN = 1,     /* the group's own texture */
    FIELD_HEAD_MESH_EYE_CLOSED = 2    /* the actor's alternate texture slot */
};

/* An entity's script priority levels (g_FieldScriptPriority): 0 is the most urgent, a request at a
 * level runs only when the entity is at a less urgent one. Script_ExecuteFrame starts the
 * interaction scripts at level 1; Script_ResetState puts every entity at level 7, its main script,
 * where RET waits instead of returning to a lower level (Script_OpReturn). */
enum FieldScriptPriority {
    FIELD_SCRIPT_PRIORITY_INTERACTION = 1,
    FIELD_SCRIPT_PRIORITY_MAIN = 7
};

/* FieldState.fadeType, the screen fade Field_UpdateFieldFx runs each frame (set by Script_OpScreenFade
 * and the map exit). Odd scripted types step out, even ones step in. "Scaled" fades draw the red,
 * green and blue channels scaled by the fade step on render object slot 1 or 2, "flat" fades draw
 * them unscaled; "complement" types pass 256 minus each channel. The black ramps count
 * fadeAdjust up to a fixed frame count and then end the fade. */
enum FieldFadeType {
    FIELD_FADE_NONE = 0,
    FIELD_FADE_SCALED_COMPLEMENT_OUT = 1,
    FIELD_FADE_SCALED_COMPLEMENT_IN = 2,
    FIELD_FADE_BLACK_RAMP_LONG = 3, /* 34 frames */
    FIELD_FADE_HIDE_FIELD = 4,
    FIELD_FADE_SCALED_OUT = 5,
    FIELD_FADE_SCALED_IN = 6,
    FIELD_FADE_FLAT_COMPLEMENT_OUT = 7,
    FIELD_FADE_FLAT_COMPLEMENT_IN = 8,
    FIELD_FADE_FLAT_OUT = 9,
    FIELD_FADE_FLAT_IN = 10,
    FIELD_FADE_COLOR_BLEND = 11,
    FIELD_FADE_INVERSE_COLOR_BLEND = 12,
    FIELD_FADE_BLACK_RAMP_SHORT = 13 /* 18 frames */
};

/* g_FieldLogicExitState: where a menu or map-jump exit is in its fade (Field_UpdateLogic). The fade
 * handler Field_UpdateFieldFx sets FADE_DONE when a black ramp ends on a map jump. */
enum FieldExitState {
    FIELD_EXIT_IDLE = 0,
    FIELD_EXIT_FADING = 1,
    FIELD_EXIT_FADE_DONE = 3
};

/* The commands Field_ExecuteModelCommand runs on a field model (FieldModelCommandRecord::command);
 * each is named by the function that applies it. Commands that finish at once leave
 * FIELD_MODEL_COMMAND_DONE in the result. */
enum FieldModelCommandCode {
    FIELD_MODEL_COMMAND_DONE = -1,
    FIELD_MODEL_COMMAND_NONE = 0,
    FIELD_MODEL_COMMAND_RELIGHT = 1,
    FIELD_MODEL_COMMAND_DELTA_PACKET = 2,
    FIELD_MODEL_COMMAND_MATRIX_PACKET = 3,
    FIELD_MODEL_COMMAND_SKIP = 4,
    FIELD_MODEL_COMMAND_POLYGON_RECORD_FRAME = 5,
    FIELD_MODEL_COMMAND_RAMP_DELTA = 6,
    FIELD_MODEL_COMMAND_INDEXED_MATRIX = 7,
    FIELD_MODEL_COMMAND_SPLASH_A = 8,
    FIELD_MODEL_COMMAND_SPLASH_B = 9,
    FIELD_MODEL_COMMAND_RECORD_OVERRIDES = 10,
    FIELD_MODEL_COMMAND_RAMP_CYCLE = 13
};

/* How the field palette grid and packed colour records classify one source palette word. */
enum FieldPaletteCellMode {
    FIELD_PALETTE_CELL_EMPTY = 0,
    FIELD_PALETTE_CELL_SEMI_TRANSPARENT = 1,
    FIELD_PALETTE_CELL_OPAQUE = 2,
    FIELD_PALETTE_CELL_BLACK_SEMI_TRANSPARENT = 3
};


/* packet[0] of Field_ApplyIndexedModelMatrixRecord. */
enum FieldModelMatrixRecordAction {
    FIELD_MODEL_MATRIX_RECORD_STORE = 0,     /* copy the packet into g_FieldModelRuntimeRecords[packet[1]] */
    FIELD_MODEL_MATRIX_RECORD_APPLY = 1      /* repack the record and run Field_ApplyModelMatrixPacket */
};

/* FieldPaletteGridCell::paletteIndex of a cell that has no palette row. */
#define FIELD_PALETTE_INDEX_NONE 255

/* How Script_RequestEntityScript treats the caller (its mode): REQ runs the request and goes on,
 * skipping it when the target level is busy; REQSW waits until the target accepts it; REQEW waits
 * until the requested script returns. */
enum FieldScriptRequestMode {
    FIELD_SCRIPT_REQUEST_ASYNC = 1,
    FIELD_SCRIPT_REQUEST_WAIT_START = 2,
    FIELD_SCRIPT_REQUEST_WAIT_END = 3
};

/* g_FieldScriptPriorityState[entity][priority]: a REQEW caller waiting on that level's script.
 * Script_RequestEntityScript sets WAITING; RET or RETTO at that level sets RETURNED; the caller
 * clears it back to IDLE. */
enum FieldScriptRequestState {
    FIELD_SCRIPT_REQUEST_STATE_IDLE = 0,
    FIELD_SCRIPT_REQUEST_STATE_WAITING = 1,
    FIELD_SCRIPT_REQUEST_STATE_RETURNED = 2
};

/* g_FieldModelAnimMode[model], run each frame by Field_UpdateModelAnimStates: IDLE plays the
 * model's idle animation, LOOP loops the current one, PLAY_AND_WAIT plays it once and then holds
 * at ENDED for the script waiting on it, HOLD keeps the last frame, and the two PLAY_THEN modes
 * play it once without a waiting script and fall back to IDLE or HOLD. */
enum FieldModelAnimMode {
    FIELD_ANIM_MODE_IDLE = 0,
    FIELD_ANIM_MODE_LOOP = 1,
    FIELD_ANIM_MODE_PLAY_AND_WAIT = 2,
    FIELD_ANIM_MODE_HOLD = 3,
    FIELD_ANIM_MODE_ENDED = 4,
    FIELD_ANIM_MODE_PLAY_THEN_IDLE = 5,
    FIELD_ANIM_MODE_PLAY_THEN_HOLD = 6
};

/* A field model's standard animations: the slots Script_OpSetStandardAnimation sets
 * (FieldState idleAnimId, walkAnimId, runAnimId), and the animation ids a scripted walk plays by
 * its speed. */
enum FieldStandardAnim {
    FIELD_STANDARD_ANIM_IDLE = 0,
    FIELD_STANDARD_ANIM_WALK = 1,
    FIELD_STANDARD_ANIM_RUN = 2
};

/* A field entity's scriptedMoveMode, the scripted movement the walkmesh update runs for it:
 * WALK steps it on the walkmesh to moveEndX/Y, JUMP flies it on a parabola to a point of
 * triangle moveEndI, and the two LADDER modes step it along the ladder line by the up/down or
 * left/right pad keys (the player) or on their own (other entities). */
enum FieldScriptedMove {
    FIELD_SCRIPTED_MOVE_NONE = 0,
    FIELD_SCRIPTED_MOVE_WALK = 1,
    FIELD_SCRIPTED_MOVE_JUMP = 3,
    FIELD_SCRIPTED_MOVE_LADDER_UP_DOWN = 4,
    FIELD_SCRIPTED_MOVE_LADDER_LEFT_RIGHT = 5
};

/* A field entity's actionState while a scripted move runs: the opcode starts it at START, the
 * walkmesh update sets RUNNING and then DONE, and the waiting opcode clears it. */
enum FieldMoveState {
    FIELD_MOVE_STATE_START = 0,
    FIELD_MOVE_STATE_RUNNING = 1,
    FIELD_MOVE_STATE_DONE = 2
};

/* A field entity's turnType and ofsType: how its turn or its offset move steps
 * (Field_InterpolateLinear or Field_InterpolateCosine); the update sets DONE at the last step and
 * the waiting opcode clears it to NONE. */
enum FieldInterpType {
    FIELD_INTERP_NONE = 0,
    FIELD_INTERP_LINEAR = 1,
    FIELD_INTERP_SMOOTH = 2,
    FIELD_INTERP_DONE = 3
};

/* The rotation operand of the turn opcodes: which way turnEnd is wrapped from the current
 * direction (the direction byte counts up or down to it, or takes the shorter way). */
enum FieldTurnRotation {
    FIELD_TURN_ROTATION_INCREASING = 0,
    FIELD_TURN_ROTATION_DECREASING = 1,
    FIELD_TURN_ROTATION_SHORTEST = 2
};

/* FieldState.cameraScrollMode, started by Field_BeginCameraTransitionFromRequest and stepped by
 * Field_StepCameraScrollTransition: RESET puts the camera back at the origin, FOLLOW tracks the
 * target entity, the TO_TARGET modes scroll to it, FIXED jumps to a point and the TO_POINT modes
 * scroll to one, in a straight (linear) or eased (cosine) ramp. */
enum FieldCameraScrollMode {
    FIELD_CAMERA_SCROLL_RESET = 0,
    FIELD_CAMERA_SCROLL_FOLLOW = 1,
    FIELD_CAMERA_SCROLL_TO_TARGET_LINEAR = 2,
    FIELD_CAMERA_SCROLL_TO_TARGET_SMOOTH = 3,
    FIELD_CAMERA_SCROLL_FIXED = 4,
    FIELD_CAMERA_SCROLL_TO_POINT_LINEAR = 5,
    FIELD_CAMERA_SCROLL_TO_POINT_SMOOTH = 6
};

/* FieldState.cameraScrollState: a scroll opcode posts REQUESTED, the camera starts it (RUNNING)
 * and finishes it (DONE); WSCROLL waits for DONE. */
enum FieldCameraScrollState {
    FIELD_CAMERA_SCROLL_STATE_REQUESTED = 0,
    FIELD_CAMERA_SCROLL_STATE_RUNNING = 1,
    FIELD_CAMERA_SCROLL_STATE_DONE = 2
};

/* FieldState.viewOffsetMode, which Script_OpSetViewOffset (VWOFT) sets and Field_UpdateCameraScroll
 * steps: the camera's vertical view offset ramps to its target in a straight (linear) or eased
 * (cosine) ramp and then holds (DONE); IMMEDIATE sets it at once. */
enum FieldViewOffsetMode {
    FIELD_VIEW_OFFSET_IMMEDIATE = 0,
    FIELD_VIEW_OFFSET_LINEAR = 1,
    FIELD_VIEW_OFFSET_SMOOTH = 2,
    FIELD_VIEW_OFFSET_DONE = 3
};

/* g_FieldFollowerState[entity]: where a party member is in a party join (Script_StepFollowerToLeader)
 * or split (Script_StepFollowerToPoint): it moves and turns in either order, then is DONE. */
enum FieldFollowerState {
    FIELD_FOLLOWER_START = 0,
    FIELD_FOLLOWER_MOVING = 1,
    FIELD_FOLLOWER_TURNING = 2,
    FIELD_FOLLOWER_DONE = 3
};

/* The SPECIAL opcode's sub-opcodes (operand byte 1), run by Script_OpSpecial. The item commands
 * take or give 99 of each of the 512 item ids (count 99 in the item word's top seven bits). */
enum FieldSpecialOp {
    FIELD_SPECIAL_SET_MOVIE_REQUESTED = 0xF5,
    FIELD_SPECIAL_COPY_CHARACTER_NAME = 0xF6,
    FIELD_SPECIAL_GET_MESSAGE_SPEED = 0xF7,
    FIELD_SPECIAL_SET_MESSAGE_SPEED = 0xF8,
    FIELD_SPECIAL_ADD_ALL_MATERIA = 0xF9,
    FIELD_SPECIAL_ADD_ALL_ITEMS = 0xFA,
    FIELD_SPECIAL_SET_MOVIE_PENDING = 0xFB,
    FIELD_SPECIAL_SET_MOVIE_PLAYING = 0xFC,
    FIELD_SPECIAL_SET_CHARACTER_NAME = 0xFD,
    FIELD_SPECIAL_RESET_SAVEMAP = 0xFE,
    FIELD_SPECIAL_REMOVE_ALL_ITEMS = 0xFF
};

/* A field entity's turnType and ofsType: how Field_UpdateEntities steps its turn or its offset. */
enum FieldEntityInterp {
    FIELD_ENTITY_INTERP_NONE = 0,   /* nothing to step */
    FIELD_ENTITY_INTERP_LINEAR = 1, /* Field_InterpolateLinear */
    FIELD_ENTITY_INTERP_SMOOTH = 2, /* Field_InterpolateCosine */
    FIELD_ENTITY_INTERP_DONE = 3    /* the last step is reached; the wait opcodes look for it */
};

/* FieldDoorTriggerRecord.mode, and the mode Field_SetDoorTriggerBit applies: an even mode sets the
 * record's bit of backgroundLayerVisibility when the actor reaches the segment, an odd mode clears it.
 * Field_InitDoorTriggers starts each bit in the other state. */
enum FieldDoorTriggerMode {
    FIELD_DOOR_TRIGGER_SET = 0,
    FIELD_DOOR_TRIGGER_CLEAR = 1,
    FIELD_DOOR_TRIGGER_SET_WHILE_NEAR = 2,   /* cleared again when the actor is away from the segment */
    FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR = 3, /* set again when the actor is away from the segment */
    FIELD_DOOR_TRIGGER_SET_WHILE_NEAR_SIDED = 4,  /* as 2, but kept while the actor is on the segment's positive side */
    FIELD_DOOR_TRIGGER_CLEAR_WHILE_NEAR_SIDED = 5 /* as 3, but kept while the actor is on the segment's positive side */
};

/* The step of a splash command record (FieldModelCommandRecord::command) that
 * Field_ApplyModelSplashCommand runs: SETUP stores the record's four shorts and byte in the model's
 * runtime record; DRAW applies them as an effect packet and draws the splash quads. */
enum FieldSplashCommandStep {
    FIELD_SPLASH_COMMAND_SETUP = 0,
    FIELD_SPLASH_COMMAND_DRAW = 1
};

/* The phase of a model's polygon ramp cycle (FieldModelRuntimeRecord::rampPhase, Field_RunPolygonRampCycle):
 * two pauses, each followed by a sweep of the ramp position up to 0x120. */
enum FieldPolygonRampPhase {
    FIELD_POLYGON_RAMP_WAIT = 0,     /* counts up to the record's rampLimit */
    FIELD_POLYGON_RAMP_SWEEP = 1,
    FIELD_POLYGON_RAMP_PAUSE = 2,    /* counts up to 10 */
    FIELD_POLYGON_RAMP_SWEEP_AGAIN = 3
};

/* The command byte (packet[0]) of Field_RampModelDeltaToTarget. */
enum FieldRampCommand {
    FIELD_RAMP_COMMAND_LOAD = 0,     /* the record's current, target and rate words from the packet */
    FIELD_RAMP_COMMAND_STEP = 1      /* apply the current values, then step them toward their targets */
};

/* g_FieldRenderStateMode, selected by Field_SelectRenderStateModeByMap for the six splash/water
 * fields and dispatched by Field_ApplyModelDiffuseSelectorEffect and Field_ApplyModelSplashCommand. */
enum FieldRenderStateMode {
    FIELD_RENDER_STATE_NONE = 0,
    FIELD_RENDER_STATE_MAP_1C1 = 1,
    FIELD_RENDER_STATE_MAP_058 = 2,
    FIELD_RENDER_STATE_MAP_1AD = 3,
    FIELD_RENDER_STATE_MAP_1AE = 4,
    FIELD_RENDER_STATE_MAP_1B3 = 5,
    FIELD_RENDER_STATE_MAP_281 = 6
};


enum FieldMoveToMode {
    FIELD_MOVETO_WALK_ANIM = 0,
    FIELD_MOVETO_RUN_ANIM = 1,
    FIELD_MOVETO_SLIDE_FACING = 2,
    FIELD_MOVETO_SLIDE_LOCKED = 3
};

/* Packed bank-nibble selector (1..6) passed to Script_ReadOperand8, Script_WriteOperand8,
 * Script_ReadOperand16, and Script_WriteOperand16: selects the high or low nibble of
 * opcode parameter byte 1, 2, or 3. */
enum FieldPackedBankSlot {
    FIELD_BANK_SLOT_1_HI = 1,
    FIELD_BANK_SLOT_1_LO = 2,
    FIELD_BANK_SLOT_2_HI = 3,
    FIELD_BANK_SLOT_2_LO = 4,
    FIELD_BANK_SLOT_3_HI = 5,
    FIELD_BANK_SLOT_3_LO = 6
};

enum FieldWindowSlotIndex {
    FIELD_WINDOW_SLOT_TOP = 1
};

enum FieldScriptWaitState {
    FIELD_WAIT_LOAD_OPERAND = 0,
    FIELD_WAIT_COMPLETE_NEXT_TICK = 1
};

enum FieldBgScrollLayerIndex {
    FIELD_BG_SCROLL_LAYER_2 = 2,
    FIELD_BG_SCROLL_LAYER_3 = 3
};

#endif

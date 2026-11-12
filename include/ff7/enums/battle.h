/* include/ff7/enums/battle.h: named codes of ff7/battle and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_BATTLE_H
#define FF7_ENUMS_BATTLE_H

/* Battle motion-script commands 0x8E..0xFF run by Battle_RunActorAnimScript; lower bytes are animation ids */
enum BattleMotionOp {
    BATTLE_MOTION_HIDE_TARGET_MARKER_ONCE = 0x8E,             /* latched by 00BFB710 */
    BATTLE_MOTION_CLEAR_HIDE_TARGET_MARKER = 0x8F,
    BATTLE_MOTION_SET_PALETTE_COLOR = 0x90,
    BATTLE_MOTION_DELAYED_WHITE_FLASH = 0x91,
    BATTLE_MOTION_SET_EFFECT_ACTIVE = 0x92,
    BATTLE_MOTION_SCREEN_COLOR_RAMP = 0x93,
    BATTLE_MOTION_ROTATE_YAW = 0x94,
    BATTLE_MOTION_RESET_YAW_TURN_PARTY_LAYOUT_2 = 0x95,
    BATTLE_MOTION_BONE_PARTICLES_PARTY_HEIGHT = 0x96,
    BATTLE_MOTION_STATUS_POSE = 0x97,
    BATTLE_MOTION_WAIT_MESSAGE_WINDOW_DELAYED = 0x98,
    BATTLE_MOTION_MOVE_TO_TARGET_DELAYED = 0x99,
    BATTLE_MOTION_PLACE_IN_FRONT_OF_TARGET_ALIAS = 0x9A,      /* unused by retail */
    BATTLE_MOTION_LOCK_TARGET_FACING = 0x9B,                  /* unused by retail */
    BATTLE_MOTION_RESTORE_SFX_VOLUME = 0x9C,
    BATTLE_MOTION_START_EFFECT_PROGRAM = 0x9D,
    BATTLE_MOTION_SYNC_WAIT_TARGET_IDLE = 0x9E,
    BATTLE_MOTION_UNLOCK_TARGET_FACING = 0x9F,                /* unused by retail */
    BATTLE_MOTION_ATTACH_TARGET_TO_BONE_FLIPPED = 0xA0,
    BATTLE_MOTION_SPAWN_SCRIPT_TIMER = 0xA1,
    BATTLE_MOTION_TRANSFORM_MODEL = 0xA2,
    BATTLE_MOTION_FADE_OUT_SFX = 0xA3,
    BATTLE_MOTION_FIRE_EVENT_1F0_FLASH_2 = 0xA4,              /* event id in hex */
    BATTLE_MOTION_FIRE_EVENT_1E9_FLASH_3 = 0xA5,              /* event id in hex */
    BATTLE_MOTION_SNAP_OTHER_ACTOR_TO_SPAWN = 0xA6,
    BATTLE_MOTION_PLAY_ANIM_KEEP_STEP = 0xA7,
    BATTLE_MOTION_RETURN_TO_SPAWN_POS = 0xA8,
    BATTLE_MOTION_PLAY_ANIM_STAGGERED = 0xA9,                 /* first operand byte skipped */
    BATTLE_MOTION_RESUME_CAMERA = 0xAA,
    BATTLE_MOTION_TAKE_SPAWN_PLACE_OTHER_IN_FRONT = 0xAB,
    BATTLE_MOTION_SET_TRANSFORM_MODEL = 0xAC,
    BATTLE_MOTION_BONE_PARTICLES_AT_BONE = 0xAD,
    BATTLE_MOTION_DETACH_TARGET = 0xAE,
    BATTLE_MOTION_ATTACH_TARGET_TO_BONE = 0xAF,
    BATTLE_MOTION_STEP_BACK_SHORT = 0xB0,                     /* 258 units */
    BATTLE_MOTION_STEP_FORWARD_SHORT = 0xB1,                  /* 258 units */
    BATTLE_MOTION_SKIP_BLOCK_END = 0xB2,
    BATTLE_MOTION_SKIP_BLOCK_IF_SMALL = 0xB3,
    BATTLE_MOTION_RESET_YAW_LAYOUT_2_4 = 0xB4,
    BATTLE_MOTION_SPAWN_SPRITE_AT_POINT = 0xB5,
    BATTLE_MOTION_HOLD_CAMERA_PLAY_ANIM_KEEP_STEP = 0xB6,
    BATTLE_MOTION_STATUS_POSE_FROM_HUD = 0xB7,
    BATTLE_MOTION_HIDE_PERMANENTLY = 0xB8,
    BATTLE_MOTION_SET_CAMERA_SCRIPT = 0xB9,
    BATTLE_MOTION_YAW_OFFSET_THEN_RESET = 0xBA,               /* offset lost; unused by retail */
    BATTLE_MOTION_SET_FORMATION_CAMERA = 0xBC,                /* unused by retail */
    BATTLE_MOTION_LOCK_FACING_PLACE_IN_FRONT = 0xBD,
    BATTLE_MOTION_SPAWN_HIT_REACTION_NO_NUMBERS = 0xBE,
    BATTLE_MOTION_SET_SCRIPT_FLAG = 0xBF,
    BATTLE_MOTION_RETURN_TO_IDLE_MARKER = 0xC0,               /* never dispatched */
    BATTLE_MOTION_LOOP_BACK = 0xC1,
    BATTLE_MOTION_DELAYED_EFFECT_PAIR = 0xC2,
    BATTLE_MOTION_DEATH_SINK = 0xC3,
    BATTLE_MOTION_TRANSLATE_Z_BY_LAYOUT = 0xC4,
    BATTLE_MOTION_LOAD_SHARED_WAIT_FRAMES = 0xC5,
    BATTLE_MOTION_SET_SHARED_WAIT_FRAMES = 0xC6,
    BATTLE_MOTION_START_ENEMY_SCRIPT = 0xC7,
    BATTLE_MOTION_TRANSLATE_XZ = 0xC8,
    BATTLE_MOTION_LOOP_LABEL = 0xC9,
    BATTLE_MOTION_LOOP_BACK_IF_EFFECT_LOADING = 0xCA,
    BATTLE_MOTION_SPAWN_EFFECT_TRAIL = 0xCB,
    BATTLE_MOTION_MOVE_TO_CENTROID_DOUBLED = 0xCC,
    BATTLE_MOTION_SKIP_BLOCK_IF_ENEMY = 0xCE,
    BATTLE_MOTION_MOVE_TRAVEL_THEN_DROP = 0xCF,
    BATTLE_MOTION_HOP_TO_TARGET = 0xD0,
    BATTLE_MOTION_MOVE_TO_TARGET = 0xD1,
    BATTLE_MOTION_TRANSLATE_Y = 0xD4,
    BATTLE_MOTION_MOVE_LIFT_THEN_TRAVEL = 0xD5,
    BATTLE_MOTION_DELAYED_ACTOR_EFFECT = 0xD6,
    BATTLE_MOTION_DELAYED_HIT_MARK = 0xD7,
    BATTLE_MOTION_DELAYED_EVENT = 0xD8,
    BATTLE_MOTION_SET_EFFECT_ID_CATEGORY_2 = 0xDA,            /* unused by retail */
    BATTLE_MOTION_BONE_PARTICLES = 0xDB,                      /* unused by retail */
    BATTLE_MOTION_SET_PARTICLE_OFFSET = 0xDC,
    BATTLE_MOTION_BONE_PARTICLES_SECONDARY = 0xDD,
    BATTLE_MOTION_BONE_PARTICLES_PRIMARY = 0xDE,
    BATTLE_MOTION_FACE_TARGETS_CENTROID = 0xDF,
    BATTLE_MOTION_FIRE_EVENT_24_FLASH_1 = 0xE0,               /* event id in hex */
    BATTLE_MOTION_FADE_IN = 0xE1,
    BATTLE_MOTION_FADE_OUT_AND_HIDE = 0xE2,
    BATTLE_MOTION_SET_HOME_POS = 0xE3,
    BATTLE_MOTION_STEP_BACK_LONG = 0xE4,                      /* 516 units */
    BATTLE_MOTION_RESET_YAW_BY_LAYOUT = 0xE5,
    BATTLE_MOTION_FIRE_EVENT_0D_FLASH_0 = 0xE6,               /* event id in hex */
    BATTLE_MOTION_SET_VISIBLE = 0xE7,
    BATTLE_MOTION_PRELOAD_EFFECT = 0xE8,
    BATTLE_MOTION_MOVE_TO_TARGET_ARG_FROM_ACTOR = 0xE9,
    BATTLE_MOTION_WAIT_MESSAGE_WINDOW = 0xEA,
    BATTLE_MOTION_RUN_EFFECT_IMPACT = 0xEB,
    BATTLE_MOTION_RUN_EFFECT = 0xEC,
    BATTLE_MOTION_STEP_FORWARD_LONG = 0xED,                   /* 516 units */
    BATTLE_MOTION_END_SCRIPT = 0xEE,
    BATTLE_MOTION_SPAWN_SCALE_RAMP_EMITTER = 0xF0,
    BATTLE_MOTION_HALT_IDLE = 0xF1,
    BATTLE_MOTION_NOP = 0xF2,
    BATTLE_MOTION_WAIT_FRAMES = 0xF3,
    BATTLE_MOTION_SET_WAIT_FRAMES = 0xF4,
    BATTLE_MOTION_SWAP_ENEMY_MODEL = 0xF5,                    /* unused by retail */
    BATTLE_MOTION_STATUS_POSE_FROM_HUD_IF_FLAGGED = 0xF6,
    BATTLE_MOTION_SPAWN_HIT_REACTION = 0xF7,
    BATTLE_MOTION_START_UV_SCROLL = 0xF8,
    BATTLE_MOTION_RESET_YAW = 0xF9,
    BATTLE_MOTION_SNAP_TO_SPAWN_POS = 0xFA,
    BATTLE_MOTION_PLACE_IN_FRONT_OF_TARGET = 0xFB,
    BATTLE_MOTION_FACE_TARGET = 0xFC,
    BATTLE_MOTION_SET_POSITION = 0xFD,
    BATTLE_MOTION_RETURN_TO_IDLE = 0xFE,
    BATTLE_MOTION_END_SCRIPT_ALIAS = 0xFF                     /* same handler as END_SCRIPT */
};

/* Opcodes of the battle camera eye (position) script VM, dispatched by Battle_RunCameraEyeScript. */
enum BattleCameraEyeOp {
    BATTLE_CAMERA_EYE_PLAY_SFX = 0xD5,
    BATTLE_CAMERA_EYE_FLASH_WHITE = 0xD6,                 /* also plays sound 0x10C */
    BATTLE_CAMERA_EYE_SET_VM_FLAG = 0xD7,
    BATTLE_CAMERA_EYE_MOVE_TO_BONE_FROM_TABLE = 0xD8,
    BATTLE_CAMERA_EYE_LOAD_LIVE_POS = 0xD9,
    BATTLE_CAMERA_EYE_MOVES_EASED = 0xDA,
    BATTLE_CAMERA_EYE_MOVES_LINEAR = 0xDB,
    BATTLE_CAMERA_EYE_ROTATE_OFFSETS_ON = 0xDC,
    BATTLE_CAMERA_EYE_SET_FORMATION_CAM = 0xDD,
    BATTLE_CAMERA_EYE_SET_TARGET_3 = 0xDE,
    BATTLE_CAMERA_EYE_TARGET_GROUP = 0xDF,
    BATTLE_CAMERA_EYE_MOVE_TO_TARGET_CAMREC = 0xE0,
    BATTLE_CAMERA_EYE_SNAP_FORMATION_CAM = 0xE1,
    BATTLE_CAMERA_EYE_MOVE_TO_FORMATION_CAM = 0xE2,
    BATTLE_CAMERA_EYE_MOVE_TO_BONE_ACTOR = 0xE3,
    BATTLE_CAMERA_EYE_MOVE_TO_BONE_ATTACKER = 0xE4,
    BATTLE_CAMERA_EYE_MOVE_TO_BONE_TARGET = 0xE5,
    BATTLE_CAMERA_EYE_MOVE_TO_XYZ = 0xE6,
    BATTLE_CAMERA_EYE_FOLLOW_BONE_ATTACKER = 0xE7,
    BATTLE_CAMERA_EYE_FOLLOW_BONE_TARGET = 0xE9,
    BATTLE_CAMERA_EYE_FOLLOW_BONE_ACTOR = 0xEB,
    BATTLE_CAMERA_EYE_SNAP_BONE_ACTOR = 0xEF,             /* yields */
    BATTLE_CAMERA_EYE_SNAP_BONE_ATTACKER = 0xF0,          /* yields */
    BATTLE_CAMERA_EYE_ROTATE_OFFSETS_OFF = 0xF1,
    BATTLE_CAMERA_EYE_POLAR_ATTACKER = 0xF2,
    BATTLE_CAMERA_EYE_POLAR_TARGET = 0xF3,
    BATTLE_CAMERA_EYE_WAIT = 0xF4,
    BATTLE_CAMERA_EYE_SET_WAIT = 0xF5,
    BATTLE_CAMERA_EYE_SNAP_BONE_TARGET = 0xF7,            /* yields */
    BATTLE_CAMERA_EYE_ORBIT = 0xF8,
    BATTLE_CAMERA_EYE_SET_POSITION = 0xF9,
    BATTLE_CAMERA_EYE_LOOP = 0xFE,                        /* peeks next byte; restarts on 0xC0 */
    BATTLE_CAMERA_EYE_END = 0xFF
};

/* Opcodes of the battle camera focus (look-at) script VM, dispatched by Battle_RunCameraFocusScript. */
enum BattleCameraFocusOp {
    BATTLE_CAMERA_FOCUS_MOVE_TO_BONE_FROM_TABLE = 0xD8,     /* also sets the global focus latch byte */
    BATTLE_CAMERA_FOCUS_LOAD_LIVE_POS = 0xD9,
    BATTLE_CAMERA_FOCUS_MOVES_EASED = 0xDB,
    BATTLE_CAMERA_FOCUS_MOVES_LINEAR = 0xDC,
    BATTLE_CAMERA_FOCUS_SET_FORMATION_CAM = 0xDD,
    BATTLE_CAMERA_FOCUS_SET_TARGET_3 = 0xDE,
    BATTLE_CAMERA_FOCUS_TARGET_GROUP = 0xDF,
    BATTLE_CAMERA_FOCUS_MOVE_TO_TARGET_CAMREC = 0xE0,
    BATTLE_CAMERA_FOCUS_SNAP_FORMATION_CAM = 0xE1,
    BATTLE_CAMERA_FOCUS_MOVE_TO_FORMATION_CAM = 0xE2,
    BATTLE_CAMERA_FOCUS_MOVE_TO_BONE_ACTOR = 0xE3,
    BATTLE_CAMERA_FOCUS_MOVE_TO_BONE_ATTACKER = 0xE4,
    BATTLE_CAMERA_FOCUS_MOVE_TO_BONE_TARGET = 0xE5,
    BATTLE_CAMERA_FOCUS_MOVE_TO_XYZ = 0xE6,
    BATTLE_CAMERA_FOCUS_FOLLOW_BONE_ATTACKER = 0xE8,
    BATTLE_CAMERA_FOCUS_FOLLOW_BONE_TARGET = 0xEA,
    BATTLE_CAMERA_FOCUS_FOLLOW_BONE_ACTOR = 0xEC,
    BATTLE_CAMERA_FOCUS_SNAP_BONE_ACTOR = 0xF0,             /* yields; eye 0xF0 differs */
    BATTLE_CAMERA_FOCUS_WAIT = 0xF4,
    BATTLE_CAMERA_FOCUS_SET_WAIT = 0xF5,
    BATTLE_CAMERA_FOCUS_SNAP_BONE_TARGET = 0xF8,            /* yields */
    BATTLE_CAMERA_FOCUS_SNAP_BONE_ATTACKER = 0xF9,          /* yields */
    BATTLE_CAMERA_FOCUS_SET_POSITION = 0xFA,
    BATTLE_CAMERA_FOCUS_LOOP = 0xFE,                        /* peeks next byte; restarts on 0xC0 */
    BATTLE_CAMERA_FOCUS_END = 0xFF
};

/* Camera node modes (opcode word & ~0xC0) of the per-summon BattleFx_*RunCameraScript interpreters. */
enum BattleSummonCameraMode {
    SUMMON_CAMERA_WAIT = 0x00,
    SUMMON_CAMERA_SET_POSITION = 0x01,
    SUMMON_CAMERA_MOVE_LINEAR = 0x02,
    SUMMON_CAMERA_MOVE_ORBIT = 0x03,
    SUMMON_CAMERA_MOVE_ACCEL = 0x04,
    SUMMON_CAMERA_MOVE_EASED = 0x05,
    SUMMON_CAMERA_ATTACH_BONE = 0x06,           /* KotR dialect differs (dropped) */
    SUMMON_CAMERA_DETACH = 0x07,
    SUMMON_CAMERA_SET_SCRIPT_VALUE = 0x08,      /* 10-mode dialect only: g_BattleCameraScriptValue = the next script word */
    SUMMON_CAMERA_WAIT_NO_WRITEBACK = 0x09,     /* 10-mode dialect only */
    SUMMON_CAMERA_HOLD_OUTPUT = 0x08            /* 9-mode dialect (Odin, Typhon): waits and leaves the camera output alone */
};

/* The bit of a summon camera opcode word above its BattleSummonCameraMode: set, the node drives
 * the eye (position) track, clear the focus track (BattleFx_WhirlsandRunCameraScript). */
enum BattleSummonCameraOpBit {
    SUMMON_CAMERA_OP_EYE = 0x40
};

/* The Knights of the Round camera script's dialect (BattleFxCameraOrbitState.mode, the opcode word
 * & ~0xC0, run by BattleFx_KotrRunCameraScript): modes 0-5 as BattleSummonCameraMode, then its own. */
enum BattleKotrCameraMode {
    BATTLE_KOTR_CAMERA_WAIT = 0x00,
    BATTLE_KOTR_CAMERA_SET_POSITION = 0x01,
    BATTLE_KOTR_CAMERA_MOVE_LINEAR = 0x02,
    BATTLE_KOTR_CAMERA_MOVE_ORBIT = 0x03,
    BATTLE_KOTR_CAMERA_MOVE_ACCEL = 0x04,
    BATTLE_KOTR_CAMERA_MOVE_EASED = 0x05,
    BATTLE_KOTR_CAMERA_MOVE_ARC = 0x06,                /* eased move with a sideways sine offset */
    BATTLE_KOTR_CAMERA_MOVE_ARC_NEGATED_EASE = 0x07,   /* the same, the offset eased by the negated curve */
    BATTLE_KOTR_CAMERA_ATTACH = 0x08,                  /* position becomes local to an attach point */
    BATTLE_KOTR_CAMERA_DETACH = 0x09,                  /* back to world space; attachPoint 0xFFFF */
    BATTLE_KOTR_CAMERA_SET_SCRIPT_VALUE = 0x0A,        /* g_BattleCameraScriptValue = the next script word */
    BATTLE_KOTR_CAMERA_HOLD_OUTPUT = 0x0B              /* waits and leaves the camera output alone */
};

/* Section types of kernel/kernel.bin that yama/init.c handles by name; the others index
 * g_KernelBinSectionTargets. */
enum KernelBinSection {
    KERNEL_BIN_SECTION_CHARACTER_RECORDS = 3,  /* the initial savemap character records */
    KERNEL_BIN_SECTION_TEXT = 9                /* a text bank (Kernel_AllocTextBank) */
};

/* Section types of kernel/window.bin (Kernel_ParseWindowBinSections). */
enum KernelWindowBinSection {
    KERNEL_WINDOW_BIN_SECTION_DISCARDED = 0,    /* read into a 64 KB scratch block and freed */
    KERNEL_WINDOW_BIN_SECTION_WINDOW_DATA = 1   /* g_BattleWindowData, also the text spacing table */
};

/* AI script opcodes 0x30..0x45 evaluated per lane by Battle_AiVm_EvalArithmetic and Battle_AiVm_EvalCompare. */
enum BattleAiOp {
    BATTLE_AI_OP_ADD = 0x30,
    BATTLE_AI_OP_SUB = 0x31,
    BATTLE_AI_OP_MUL = 0x32,
    BATTLE_AI_OP_DIV = 0x33,
    BATTLE_AI_OP_MOD = 0x34,
    BATTLE_AI_OP_BIT_AND = 0x35,
    BATTLE_AI_OP_BIT_OR = 0x36,
    BATTLE_AI_OP_BIT_NOT = 0x37,     /* unary */
    BATTLE_AI_OP_CMP_EQ = 0x40,
    BATTLE_AI_OP_CMP_NE = 0x41,
    BATTLE_AI_OP_CMP_GE = 0x42,      /* unsigned */
    BATTLE_AI_OP_CMP_LE = 0x43,      /* unsigned */
    BATTLE_AI_OP_CMP_GT = 0x44,      /* unsigned */
    BATTLE_AI_OP_CMP_LT = 0x45       /* unsigned */
};

/* AI script opcode high nibble (opcode >> 4) dispatched by Battle_AiVm_Run; low nibble selects within the group. */
enum BattleAiOpGroup {
    BATTLE_AI_GROUP_PUSH_VAR = 0x00,
    BATTLE_AI_GROUP_PUSH_ADDRESS = 0x01,
    BATTLE_AI_GROUP_ARITHMETIC = 0x03,
    BATTLE_AI_GROUP_COMPARE = 0x04,
    BATTLE_AI_GROUP_LOGIC = 0x05,
    BATTLE_AI_GROUP_PUSH_CONST = 0x06,
    BATTLE_AI_GROUP_FLOW = 0x07,
    BATTLE_AI_GROUP_BUILTIN = 0x08,
    BATTLE_AI_GROUP_COMMAND = 0x09,
    BATTLE_AI_GROUP_DEBUG = 0x0A             /* stripped debug print */
};

/* AI variable access width (Battle_AiVm_ReadVar/WriteVar width, the low nibble of an operand
   descriptor). On the operand stack a bit or byte takes one byte, a word two, a dword three. */
enum BattleAiVarWidth {
    BATTLE_AI_WIDTH_BIT = 0x00,
    BATTLE_AI_WIDTH_BYTE = 0x01,
    BATTLE_AI_WIDTH_WORD = 0x02,
    BATTLE_AI_WIDTH_DWORD = 0x03
};

/* AI operand kind (the high nibble of an operand descriptor, BattleScriptVm::operandType). */
enum BattleAiOperandType {
    BATTLE_AI_OPERAND_SCALAR = 0x00,         /* one value, broadcast to every lane on pop */
    BATTLE_AI_OPERAND_ADDRESS = 0x01,        /* a two-byte variable address (opcodes 0x1X) */
    BATTLE_AI_OPERAND_LANES = 0x02           /* a lane mask plus one value per selected actor */
};

/* AI_GROUP_LOGIC operations (opcode low nibble), on "any selected lane non-zero". */
enum BattleAiLogicOp {
    BATTLE_AI_LOGIC_AND = 0x00,
    BATTLE_AI_LOGIC_OR = 0x01,
    BATTLE_AI_LOGIC_NOT = 0x02
};

/* AI_GROUP_PUSH_CONST operations (opcode low nibble): the immediate's size in bytes. */
enum BattleAiConstSize {
    BATTLE_AI_CONST_8BIT = 0x00,
    BATTLE_AI_CONST_16BIT = 0x01,
    BATTLE_AI_CONST_24BIT = 0x02
};

/* AI_GROUP_FLOW operations (opcode low nibble). */
enum BattleAiFlowOp {
    BATTLE_AI_FLOW_JUMP_IF_ZERO = 0x00,      /* pops the condition */
    BATTLE_AI_FLOW_JUMP_IF_NOT_EQUAL = 0x01, /* pops one operand, peeks the other */
    BATTLE_AI_FLOW_JUMP = 0x02,
    BATTLE_AI_FLOW_END = 0x03,
    BATTLE_AI_FLOW_POP = 0x04,
    BATTLE_AI_FLOW_SET_PARTY_OVERRIDE = 0x05 /* Battle_SetPartyAiEntityOverride */
};

/* AI_GROUP_BUILTIN operations (opcode low nibble). */
enum BattleAiBuiltinOp {
    BATTLE_AI_BUILTIN_MASK_LANES = 0x00,     /* restricts a lane operand to an actor mask */
    BATTLE_AI_BUILTIN_RANDOM = 0x01,
    BATTLE_AI_BUILTIN_RANDOM_TARGET = 0x02,  /* Battle_PickRandomTargetFromMask */
    BATTLE_AI_BUILTIN_COUNT = 0x03,          /* bit count of a mask, or a scalar's width */
    BATTLE_AI_BUILTIN_MAX_MASK = 0x04,       /* mask of the lanes holding the largest value */
    BATTLE_AI_BUILTIN_MIN_MASK = 0x05,       /* mask of the lanes holding the smallest value */
    BATTLE_AI_BUILTIN_ATTACK_MP_COST = 0x06,
    BATTLE_AI_BUILTIN_BIT = 0x07             /* 1 << value */
};

/* AI_GROUP_COMMAND operations (opcode low nibble). */
enum BattleAiCommandOp {
    BATTLE_AI_COMMAND_ASSIGN = 0x00,         /* writes a variable, per lane for combatant addresses */
    BATTLE_AI_COMMAND_POP = 0x01,
    BATTLE_AI_COMMAND_POST_ACTION = 0x02,    /* Battle_AiVm_PostAction(command, attack) */
    BATTLE_AI_COMMAND_SHOW_MESSAGE = 0x03,   /* inline 0xFF-terminated text */
    BATTLE_AI_COMMAND_COPY_COMBATANT = 0x04, /* Battle_AiVm_CopyCombatantState */
    BATTLE_AI_COMMAND_SAVEMAP_BYTE = 0x05,   /* Battle_AiVm_AccessSavemapByte */
    BATTLE_AI_COMMAND_APPLY_STATUS = 0x06    /* Battle_ApplyStatusToActorMask */
};

/* AI_GROUP_DEBUG operations (opcode low nibble); both only consume their operands. */
enum BattleAiDebugOp {
    BATTLE_AI_DEBUG_PRINT = 0x00,            /* count byte, null-terminated text, popped arguments */
    BATTLE_AI_DEBUG_POP_TWO = 0x01
};

/* BattleFxBlueMatraMagicRibbon::state: the steps of one Matra missile (BattleFx_BlueMatraMagicRibbonTask). */
enum BattleBlueMatraMagicRibbonState {
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_FREE = -1,
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_LAUNCH = 0,       /* sets the launch speed and wave amplitude */
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_RISE = 1,         /* weaves out along its launch direction for 12 frames */
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_AIM = 2,          /* turns toward its target, sets speed and acceleration */
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_HOME = 3,         /* flies at the target until it arrives or overshoots */
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_IMPACT = 4,       /* spawns the hit sprite and sound */
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_DRAIN_TRAIL = 5,  /* drops one trail point a frame */
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_DONE = 6,         /* frees the task and the record */
    BATTLE_BLUE_MATRA_MAGIC_RIBBON_PAUSED = 0x63     /* the task's local copy while the battle is paused */
};

/* BattleFxBlueLaserRibbon::state: the steps of one Reazer ribbon (BattleFx_BlueLaserRibbonTask). */
enum BattleBlueLaserRibbonState {
    BATTLE_BLUE_LASER_RIBBON_LAUNCH = 0,       /* sets the launch speed and wave amplitude */
    BATTLE_BLUE_LASER_RIBBON_RISE = 1,         /* weaves out along its launch direction for 8 frames */
    BATTLE_BLUE_LASER_RIBBON_AIM = 2,          /* turns toward its target, sets speed and acceleration */
    BATTLE_BLUE_LASER_RIBBON_HOME = 3,         /* flies at the target until it arrives, overshoots or times out */
    BATTLE_BLUE_LASER_RIBBON_IMPACT = 4,       /* spawns the hit sprite and, every third ribbon, the sound */
    BATTLE_BLUE_LASER_RIBBON_DRAIN_TRAIL = 5,  /* drops one trail point a frame */
    BATTLE_BLUE_LASER_RIBBON_DONE = 6,         /* frees the task */
    BATTLE_BLUE_LASER_RIBBON_PAUSED = 0x63     /* the task's local copy while the battle is paused */
};

/* BattleSummonShivaForceField.kind: how a force field of the Shiva summon pushes a particle
 * (BattleFx_SummonShivaApplyForceFieldsToParticle); END closes the table. */
enum BattleShivaForceFieldKind {
    BATTLE_SHIVA_FORCE_END = -1,
    BATTLE_SHIVA_FORCE_NONE = 0,
    BATTLE_SHIVA_FORCE_REPEL_ALONG_DIR = 1,  /* away from the center, the more the further along dir */
    BATTLE_SHIVA_FORCE_ATTRACT = 2,          /* toward the center, scaled per axis by dir */
    BATTLE_SHIVA_FORCE_CONSTANT = 3,         /* adds dir to the velocity */
    BATTLE_SHIVA_FORCE_VORTEX = 4            /* turns around dir: the cross product of the way to the center and dir */
};

/* BattleLimitDiceTumble.state: the motion of one die of Cait Sith's Dice limit
 * (BattleFx_LimitDiceTumblingDieTask). */
enum BattleLimitDiceTumbleState {
    BATTLE_DICE_TUMBLE_FALL = 0,    /* falls to the floor, then hops or settles by how far it is from a face */
    BATTLE_DICE_TUMBLE_HOP = 1,     /* bounces once, spinning toward a face */
    BATTLE_DICE_TUMBLE_SETTLE = 2   /* finishes its spin on the floor, or hops again */
};

/* g_BattleState.statusChangeMode: how the captured status (combatant status bit 31, set with the
 * mode in its low two bits by Battle_SetStatusChange) holds its actor (Battle_RefreshActorTickRates,
 * Battle_OnStatusCapturedChanged, Battle_RecomputeActorMasks). Mode 2 has no effect. */
enum BattleCaptureMode {
    BATTLE_CAPTURE_STOP = 0,                   /* acts as Stop and incapacitates */
    BATTLE_CAPTURE_PARALYZE_SLIP = 1,          /* acts as Paralysis and drains HP (Battle_RefreshSlipHpRate) */
    BATTLE_CAPTURE_STOP_POSE = 3               /* as BATTLE_CAPTURE_STOP, with idle animation override 19 */
};

/* g_BattleBlueFlameThrowerVariantFlag: the CLUT row the Kaenn entry points give the flame particles
 * (BattleFx_BlueFlameThrowerSpawnParticles writes it into each particle's clutWord). */
enum BattleBlueFlameThrowerPalette {
    BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW0 = 0,        /* clutWord 0 */
    BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW1 = 1,        /* clutWord 0x40, the next CLUT row */
    BATTLE_BLUE_FLAME_THROWER_PALETTE_ALTERNATE = 2    /* row 0 and row 1 on alternate bursts */
};

/* g_BattleBlueMatraMagicStateFlag: BattleFx_BlueMatraMagicSpawnRibbonsTask's round of missiles. */
enum BattleBlueMatraMagicSpawnPhase {
    BATTLE_BLUE_MATRA_MAGIC_SPAWN_FIRE = 0,          /* fires one missile a frame at the next target in order */
    BATTLE_BLUE_MATRA_MAGIC_SPAWN_SHUFFLE = 1,       /* shuffles the target order for the next round */
    BATTLE_BLUE_MATRA_MAGIC_SPAWN_FINISH = 2         /* every target took 8 hits: counts down to the end */
};

/* Attack added-effect ids (record +0x12, ctx+0xBC) dispatched per stage by Battle_RunSpecialAttackEffect. */
enum BattleAddedEffect {
    BATTLE_ADDED_FX_NONE = -1,
    BATTLE_ADDED_FX_MULTI_HIT_RANDOM_TARGET = 0x00,           /* Comet2 4, Knights of Round 13 */
    BATTLE_ADDED_FX_ODIN_DEATH_ELSE_GUNGE_LANCE = 0x01,
    BATTLE_ADDED_FX_CHANCE_FAT_CHOCOBO = 0x02,
    BATTLE_ADDED_FX_TRANSFORM_USER = 0x03,                    /* Vincent's transformation limits, param 0..3 */
    BATTLE_ADDED_FX_BACK_ATTACK_IF_SIDE_MATCHES = 0x04,       /* Sewer Tsunami, Huge Tidal Wave */
    BATTLE_ADDED_FX_ESCAPE = 0x05,
    BATTLE_ADDED_FX_STEAL_GIL = 0x06,
    BATTLE_ADDED_FX_STEAL_ITEM = 0x07,
    BATTLE_ADDED_FX_TOY_BOX_RANDOM_EFFECT = 0x08,
    BATTLE_ADDED_FX_DAMAGE_X8_IF_SAME_LEVEL = 0x09,
    BATTLE_ADDED_FX_DAMAGE_BY_STATUS_COUNT = 0x0A,
    BATTLE_ADDED_FX_DAMAGE_UP_NEAR_DEATH_SENTENCE = 0x0B,
    BATTLE_ADDED_FX_DAMAGE_BY_KO_ALLY_COUNT = 0x0C,
    BATTLE_ADDED_FX_POWER_FROM_TARGET_AVG_LEVEL = 0x0D,
    BATTLE_ADDED_FX_REVIVE_PARTY_FULL_HP = 0x0E,
    BATTLE_ADDED_FX_SLOTS_PICK_ATTACK = 0x0F,
    BATTLE_ADDED_FX_MERGE_PARTY_INTO_USER = 0x10,
    BATTLE_ADDED_FX_REMOVE_PARTY_TARGET = 0x11,
    BATTLE_ADDED_FX_REMOVE_AND_EXCLUDE_TARGET = 0x12,
    BATTLE_ADDED_FX_TIFA_LIMIT_COMBO = 0x13,
    BATTLE_ADDED_FX_FILL_ALLY_LIMIT_BARS = 0x14,
    BATTLE_ADDED_FX_TARGET_ATK_MATK_DEF_MDEF_PCT = 0x15,
    BATTLE_ADDED_FX_USER_DEFENSE_PCT = 0x16,
    BATTLE_ADDED_FX_USER_ATTACK_PCT = 0x17,
    BATTLE_ADDED_FX_CHAIN_ATTACK_SAME_TARGETS = 0x18,
    BATTLE_ADDED_FX_TOGGLE_TARGET_ROW = 0x19,
    BATTLE_ADDED_FX_CHAIN_ATTACK_OTHER_TARGETS = 0x1A,
    BATTLE_ADDED_FX_EXCLUDE_ENEMY_TARGET = 0x1B,
    BATTLE_ADDED_FX_TARGET_DEF_MDEF_PCT = 0x1C,
    BATTLE_ADDED_FX_TARGET_ANIM_33 = 0x1D,                     /* attackFlag 0x1000000 on set, animId 0x33 on hit */
    BATTLE_ADDED_FX_POWER_FROM_USER_HP = 0x1E,
    BATTLE_ADDED_FX_POWER_FROM_USER_MP = 0x1F,
    BATTLE_ADDED_FX_POWER_FROM_MATERIA_AP = 0x20,
    BATTLE_ADDED_FX_POWER_FROM_KILL_COUNT = 0x21,
    BATTLE_ADDED_FX_POWER_FROM_LIMIT_BAR = 0x22,
    BATTLE_ADDED_FX_REMOVE_ENEMY_TARGET = 0x23
};

/* The stage Battle_RunSpecialAttackEffect runs an added effect at (its argument; the effect runs when
 * g_BattleSpecialAttackMaskTable has the stage's bit), named from its callers. */
enum BattleAddedEffectStage {
    BATTLE_ADDED_FX_STAGE_BEFORE_FORMULA = 0, /* Battle_RunDamageProgram, before the damage formula */
    BATTLE_ADDED_FX_STAGE_AFTER_FORMULA = 1,  /* Battle_RunDamageProgram, after the damage formula */
    BATTLE_ADDED_FX_STAGE_SET = 2,            /* Battle_SetAddedEffect, when the attack sets the effect */
    BATTLE_ADDED_FX_STAGE_AFTER_DAMAGE = 3,   /* per target, after the damage program, hit or miss */
    BATTLE_ADDED_FX_STAGE_TARGET_HIT = 4,     /* per target the attack hits */
    BATTLE_ADDED_FX_STAGE_AFTER_HITS = 5      /* after every hit of the action */
};

/* Battle status bits: the attacker's and target's status words of the battle command context and each combatant's status (g_aCombatants[].status). Names follow FF7's status order; the code proves the effect of bits 0 (KO at 0 HP), 4 (-30% damage taken), 5 (-30% hit rate), 11, 12 (damage 0), 16/17 (physical/magical damage halved), 22, 23 (damage x3/2), 26 (accuracy halved) and 30 (critical rate 255). */
enum BattleStatus {
    BATTLE_STATUS_DEATH = 0x01,
    BATTLE_STATUS_NEAR_DEATH = 0x02,
    BATTLE_STATUS_SLEEP = 0x04,
    BATTLE_STATUS_POISON = 0x08,
    BATTLE_STATUS_SADNESS = 0x10,
    BATTLE_STATUS_FURY = 0x20,
    BATTLE_STATUS_CONFUSION = 0x40,
    BATTLE_STATUS_SILENCE = 0x80,
    BATTLE_STATUS_HASTE = 0x100,
    BATTLE_STATUS_SLOW = 0x200,
    BATTLE_STATUS_STOP = 0x400,
    BATTLE_STATUS_FROG = 0x800,
    BATTLE_STATUS_SMALL = 0x1000,
    BATTLE_STATUS_SLOW_NUMB = 0x2000,
    BATTLE_STATUS_PETRIFY = 0x4000,
    BATTLE_STATUS_REGEN = 0x8000,
    BATTLE_STATUS_BARRIER = 0x10000,
    BATTLE_STATUS_MBARRIER = 0x20000,
    BATTLE_STATUS_REFLECT = 0x40000,
    BATTLE_STATUS_DUAL = 0x80000,
    BATTLE_STATUS_SHIELD = 0x100000,
    BATTLE_STATUS_DEATH_SENTENCE = 0x200000,
    BATTLE_STATUS_MANIPULATE = 0x400000,
    BATTLE_STATUS_BERSERK = 0x800000,
    BATTLE_STATUS_PEERLESS = 0x1000000,
    BATTLE_STATUS_PARALYSIS = 0x2000000,
    BATTLE_STATUS_DARKNESS = 0x4000000,
    BATTLE_STATUS_DUAL_DRAIN = 0x8000000,
    BATTLE_STATUS_DEATH_FORCE = 0x10000000,
    BATTLE_STATUS_RESIST = 0x20000000,
    BATTLE_STATUS_LUCKY_GIRL = 0x40000000,
    BATTLE_STATUS_HUD_NON_CYCLING_MASK = 0x00633803
};

/* The slots of an actor's status timers (BattleActor.statusTimers), each the countdown of the status
 * g_BattleStatusTimerSlots maps it to; when one runs out the action queue gets the slot as its event
 * code (BATTLE_ACTION_EVENT_STATUS_TIMER). Battle_ApplyTimeSlices counts STOP down on the shared
 * clock, DEATH_SENTENCE and SLOW_NUMB on the actor's tickRate slices and the others on its speedRate slices. */
enum BattleStatusTimerSlot {
    BATTLE_STATUS_TIMER_STOP = 0,
    BATTLE_STATUS_TIMER_PARALYSIS = 1,
    BATTLE_STATUS_TIMER_DEATH_SENTENCE = 2,   /* Battle_StatusTimerExpireDeathSentence */
    BATTLE_STATUS_TIMER_SLOW_NUMB = 3,        /* Battle_StatusTimerExpireSlowNumb */
    BATTLE_STATUS_TIMER_BARRIER = 4,
    BATTLE_STATUS_TIMER_MBARRIER = 5,
    BATTLE_STATUS_TIMER_POISON = 6,           /* Battle_StatusTimerExpirePoison */
    BATTLE_STATUS_TIMER_SLEEP = 7,
    BATTLE_STATUS_TIMER_REGEN = 8,
    BATTLE_STATUS_TIMER_DUAL_DRAIN = 9,
    BATTLE_STATUS_TIMER_SHIELD = 10,
    BATTLE_STATUS_TIMER_PEERLESS = 11
};

/* The command context's displayFlags, which Battle_PushDamageDisplay shows with the number:
 * HEAL marks a recovery (Battle_CommitHpMpChange adds instead of subtracting; an absorbed element
 * flips it, full recovery sets it alone), CRITICAL a critical hit (Battle_RollCriticalHit; the
 * physical formula doubles the damage), MP damage to MP instead of HP. */
enum BattleDisplayFlag {
    BATTLE_DISPLAY_HEAL = 0x1,
    BATTLE_DISPLAY_CRITICAL = 0x2,
    BATTLE_DISPLAY_MP = 0x4
};

/* Bits of the command context's resultFlags.
 * NO_DAMAGE_DISPLAY: Battle_ResolveHitOnTarget pushes the damage number only while it is clear; a
 * reflected spell and Steal set it, Attack and Mug do not. BARRIER/MBARRIER: Barrier or MBarrier
 * halved the damage (Battle_ApplyBarrierAndDamageBonus); a physical attack on a Barrier target sets
 * BARRIER, Fire on an MBarrier target sets MBARRIER, the crossed cases set neither. */
enum BattleResultFlag {
    BATTLE_RESULT_MISS = 0x1,
    BATTLE_RESULT_NO_DAMAGE_DISPLAY = 0x2,
    BATTLE_RESULT_COVERED = 0x20, /* Battle_ApplyCoverRedirect moved the hit to a covering ally; its hit record gets the cover pose */
    BATTLE_RESULT_BARRIER = 0x4000,
    BATTLE_RESULT_MBARRIER = 0x8000
};

/* Bits of the command context's specialFlags, the attack's inverted special-attack word: the
 * action starts from 0xFFFFFFFF and the attack clears the bits that apply (observed in game).
 * NOT_PHYSICAL: Battle_DecodeDamageCalcByte clears it for Attack (0xFFFB); clear, Barrier halves
 * the damage, set (Fire, 0xFDFF), MBarrier does. NOT_REFLECTABLE: Fire clears it and bounces off a
 * Reflect target, Attack keeps it and hits. NO_FORCED_CRITICAL: Deathblow clears it
 * (Battle_AttackOpMakeDeathblow) and the physical formula then sets BATTLE_DISPLAY_CRITICAL.
 * clearing one bit at a time in Fire's (or Sleepel's) kernel record in memory: without
 * NOT_MP_DAMAGE Fire takes MP, not HP (BATTLE_DISPLAY_MP); without NOT_DRAIN the caster gains the
 * damage as HP, and without NOT_DRAIN_HP_AND_MP as well as HP and MP (that bit alone does nothing);
 * without NOT_IGNORE_IMMUNITY Sleepel lands on a Resist target (the code also drops the element
 * reaction); without NOT_KO_ONLY a living target is missed (BATTLE_RESULT_MISS) and a KO'd one hit;
 * without NOT_PIERCING the target's defence is not applied (damage 296..302 -> 325..344); without
 * NOT_KO_TARGETABLE a KO'd target stays the target, where plain Fire moved to the enemy. */
enum BattleSpecialFlag {
    BATTLE_SPECIAL_NOT_MP_DAMAGE = 0x1,
    BATTLE_SPECIAL_NOT_PHYSICAL = 0x4,
    BATTLE_SPECIAL_NOT_DRAIN = 0x10,
    BATTLE_SPECIAL_NOT_DRAIN_HP_AND_MP = 0x20,
    BATTLE_SPECIAL_NOT_IGNORE_IMMUNITY = 0x80,
    BATTLE_SPECIAL_NOT_KO_ONLY = 0x100,
    BATTLE_SPECIAL_NOT_REFLECTABLE = 0x200,
    BATTLE_SPECIAL_NOT_PIERCING = 0x400,
    BATTLE_SPECIAL_NOT_KO_TARGETABLE = 0x800,
    BATTLE_SPECIAL_NO_FORCED_CRITICAL = 0x2000
};

/* A battle task slot's owner word (ownerSlot): a task writes BATTLE_TASK_FREE to give its slot back,
 * and the task loop skips free slots. */
enum BattleTaskOwner {
    BATTLE_TASK_FREE = 0xFFFF
};

/* The tag of a record in the Meikyo limit's effect pool (BattleFxClearTranquilPoolRecord): what the
 * per-frame update draws it as. */
enum BattleClearTranquilRecordTag {
    BATTLE_MEIKYO_RECORD_FREE = 0,
    BATTLE_MEIKYO_RECORD_GRADIENT_LINE = 1,
    BATTLE_MEIKYO_RECORD_FADE_SPRITE = 2
};

/* The phases of a target's Blue Lv4 Jibaku timeline task (BattleFx_BlueL4SuicideTimelineTask). */
enum BattleFxBlueJibakuPhase {
    BLUE_JIBAKU_PHASE_FADE_IN = 0,       /* the ring fades in over seven frames */
    BLUE_JIBAKU_PHASE_HOLD = 1,          /* the ring holds until frame 11 */
    BLUE_JIBAKU_PHASE_SWEEP = 2,         /* the ring is drawn up to a growing segment, with the explosion triangles */
    BLUE_JIBAKU_PHASE_EXPLODE = 3,       /* sprites burst around the target until the next-step frame */
    BLUE_JIBAKU_PHASE_AFTERGLOW = 4,     /* waits five frames */
    BLUE_JIBAKU_PHASE_ALLOC_RECORD = 5,  /* allocates the target's typed record */
    BLUE_JIBAKU_PHASE_FINISH = 6         /* waits eight frames, then frees the task */
};

/* The phases of the Blue Nanntoka timeline task (BattleFx_BluePandorasBoxTimelineTask); each spawns its
 * models at fixed frames of the task, then steps to the next. */
enum BattleFxBluePandorasBoxPhase {
    BLUE_NANNTOKA_PHASE_GROW_MODEL = 0,      /* the grow-and-shrink model */
    BLUE_NANNTOKA_PHASE_SPIN_MODELS = 1,     /* hides the stage and the actors, then two spinning models */
    BLUE_NANNTOKA_PHASE_TUMBLING_MODELS = 2, /* three tumbling models */
    BLUE_NANNTOKA_PHASE_DRIFT_MODEL = 3,     /* the spinning, drifting model */
    BLUE_NANNTOKA_PHASE_RESTORE_STAGE = 4    /* shows the stage and the actors again */
};

/* The states of the Blue Chyobo hopper's timeline task (BattleFx_BlueChocobuckleAltTimelineTask). */
enum BattleFxBlueChocobuckleState {
    BLUE_CHYOBO_STATE_START = 0,      /* allocates the ring record, plays the start sound */
    BLUE_CHYOBO_STATE_RISE = 1,       /* rises until frame 0x11, then launches the first hop */
    BLUE_CHYOBO_STATE_HOP = 2,        /* hops toward the target; when near enough, leaps at it */
    BLUE_CHYOBO_STATE_LEAP = 3,       /* the leap to the target bone's height, then the bounce off */
    BLUE_CHYOBO_STATE_BOUNCE_OFF = 4, /* 20 frames of bouncing back; the hit lands on frame 4 */
    BLUE_CHYOBO_STATE_FINISH = 5,     /* frees the task */
    BLUE_CHYOBO_STATE_DONE = 6
};

/* The stages of the Satan (Bahamut ZERO) limit's root task (BattleFx_LimitSatanSlamTimelineTask, BattleFxLimitSatanSlamState::stage). */
enum BattleFxLimitSatanSlamStage {
    SATAN_STAGE_WAIT = 0,                /* waits until timer 15 */
    SATAN_STAGE_START_MESH = 1,          /* spawns the stage mesh timeline task */
    SATAN_STAGE_RUNNING = 2              /* the mesh timeline runs; the root task only fades */
};

/* The stages of the Satan limit's stage mesh timeline task (BattleFx_LimitSatanSlamStageMeshTimelineTask,
 * BattleFxLimitSatanSlamStageSlot::stage). Stages before SHATTER draw the whole mesh, the rest its fragments. */
enum BattleFxLimitSatanSlamMeshStage {
    SATAN_MESH_STAGE_RISE = 0,           /* the mesh's vertical scale grows over 16 steps */
    SATAN_MESH_STAGE_HOLD = 1,           /* spawns the mirrored burst models; skips DEFORM when the limit's user is the only target */
    SATAN_MESH_STAGE_DEFORM = 2,         /* adds the scaled vertex deltas over 16 steps */
    SATAN_MESH_STAGE_PARTICLES = 3,      /* spawns the trailed particles, waits 70 steps */
    SATAN_MESH_STAGE_SHATTER = 4,        /* draws the fragment mesh while the actor heights sink */
    SATAN_MESH_STAGE_CLEAR = 5,          /* waits two steps with nothing left */
    SATAN_MESH_STAGE_DONE = 6            /* frees the task and flags the root task */
};

/* Target flags of a battle command (g_BtlTargetFlags). The names follow FF7's targeting byte; the
 * target cursor (battle_target_selection.cpp) tests each bit for the effect its name gives,
 * 0x20 and 0x80 aside, which it does not read. */
enum BattleTargetFlag {
    BTL_TARGETF_SELECTABLE = 0x01,
    BTL_TARGETF_START_ON_ENEMIES = 0x02,
    BTL_TARGETF_DEFAULT_MULTIPLE = 0x04,
    BTL_TARGETF_TOGGLE_MULTIPLE = 0x08,
    BTL_TARGETF_ONE_SIDE_ONLY = 0x10,
    BTL_TARGETF_SHORT_RANGE = 0x20,
    BTL_TARGETF_ALL_ROWS = 0x40,
    BTL_TARGETF_RANDOM = 0x80
};

/* Target selection modes of the battle target cursor (g_BtlTargetSelectMode): one target on
 * either side, the party side (0..3) or the enemy side (4..11); BTL_TARGET_MULTIPLE set selects a
 * whole side, or everyone. */
enum BattleTargetSelectMode {
    BTL_TARGET_ONE_ANY = 0,
    BTL_TARGET_ONE_PARTY = 1,
    BTL_TARGET_ONE_ENEMY = 2,
    BTL_TARGET_MULTIPLE = 4,
    BTL_TARGET_ALL = 4,
    BTL_TARGET_ALL_PARTY = 5,
    BTL_TARGET_ALL_ENEMIES = 6
};

/* Screen directions the battle target cursor moves in (Battle_FindNearestTargetInDirection),
 * clockwise from up. */
enum BattleTargetDirection {
    BTL_DIR_UP = 0,
    BTL_DIR_UP_RIGHT = 1,
    BTL_DIR_RIGHT = 2,
    BTL_DIR_DOWN_RIGHT = 3,
    BTL_DIR_DOWN = 4,
    BTL_DIR_DOWN_LEFT = 5,
    BTL_DIR_LEFT = 6,
    BTL_DIR_UP_LEFT = 7
};

/* Battle layout of the scene (g_BattleScene.layout, copied into g_BattleLayout and
 * gBattleAiGlobals.layout). Names follow FF7's scene layouts. The target cursor proves only that 4
 * is handled apart from 3, 5, 6 and 7; in all five, left and right move it between the three target
 * groups (g_BattleScene.targetMaskTable). The layout selects the camera data file, the opening party and
 * enemy positions and facings; g_BattleLayoutTypes maps it to g_BattleState.layoutType
 * (3, 5, 6 and 7 to 3; 8 to 5). 8 is placed like 0 and 1. */
enum BattleLayout {
    BATTLE_LAYOUT_NORMAL = 0,
    BATTLE_LAYOUT_PREEMPTIVE = 1,
    BATTLE_LAYOUT_BACK_ATTACK = 2,
    BATTLE_LAYOUT_SIDE_ATTACK_1 = 3,
    BATTLE_LAYOUT_PINCER_ATTACK = 4,
    BATTLE_LAYOUT_SIDE_ATTACK_2 = 5,
    BATTLE_LAYOUT_SIDE_ATTACK_3 = 6,
    BATTLE_LAYOUT_SIDE_ATTACK_4 = 7,
    BATTLE_LAYOUT_FRONT_ROW_LOCKED = 8
};

/* The type byte of a reaction trigger (MenuPartyStats.reactions): mode * 3 + 1 + kind, mode 0 Final
 * Attack (on death), 1 Sneak Attack (at the start of a round), 2 the counters; kind 0 an ability,
 * 1 a command, 2 a plain attack. The kernel stores the first of each
 * mode's three in g_MateriaTriggerEntryCode and adds the kind. */
enum BattleReactionType {
    BATTLE_REACTION_FINAL_ATTACK_ABILITY = 1,
    BATTLE_REACTION_FINAL_ATTACK_COMMAND = 2,
    BATTLE_REACTION_FINAL_ATTACK_ATTACK = 3,
    BATTLE_REACTION_SNEAK_ATTACK_ABILITY = 4,
    BATTLE_REACTION_SNEAK_ATTACK_COMMAND = 5,
    BATTLE_REACTION_SNEAK_ATTACK_ATTACK = 6,
    BATTLE_REACTION_MAGIC_COUNTER = 7,
    BATTLE_REACTION_COUNTER = 8,
    BATTLE_REACTION_COUNTER_ATTACK = 9
};

/* Battle_RollReactionTriggers' triggerMode, the `mode` of BattleReactionType: which reactions fire. */
enum BattleReactionTriggerMode {
    BATTLE_REACTION_MODE_FINAL_ATTACK = 0,  /* an enemy killed the actor; uses a charge and always fires */
    BATTLE_REACTION_MODE_SNEAK_ATTACK = 1,  /* a scene round loads, for each party slot */
    BATTLE_REACTION_MODE_COUNTER = 2        /* an enemy's action hit the actor */
};

/* A reaction trigger's attack index when the materia it is paired with is Master Command, Master
 * Magic or Master Summon: g_BattleReactionAttackPickFuncs[id - BATTLE_REACTION_RANDOM_COMMAND]
 * picks the command, spell or summon when it fires. */
enum BattleReactionRandomId {
    BATTLE_REACTION_RANDOM_COMMAND = 0xFD,
    BATTLE_REACTION_RANDOM_MAGIC = 0xFE,
    BATTLE_REACTION_RANDOM_SUMMON = 0xFF
};

/* The support bits of a command or ability slot (BattleCommandSlot.supportFlags, the low five bits
 * of BattleAbilitySlot.support), set by the support materia paired with it. */
enum BattleSupportFlag {
    BATTLE_SUPPORT_MP_ABSORB = 0x01,
    BATTLE_SUPPORT_HP_ABSORB = 0x02,
    BATTLE_SUPPORT_STEAL_AS_WELL = 0x08,
    BATTLE_SUPPORT_ADDED_CUT = 0x10
};

/* A materia's type, the low nibble of its kernel record's typeNibbleByte: Kernel_ApplyMateriaToActor
 * dispatches each to the handler its name follows. */
enum BattleMateriaType {
    BATTLE_MATERIA_TYPE_INDEPENDENT = 0x0,    /* Kernel_ApplyIndependentMateria */
    BATTLE_MATERIA_TYPE_ENCOUNTER = 0x1,      /* Kernel_ApplyEncounterMateria */
    BATTLE_MATERIA_TYPE_EXTRA_CUT = 0x2,      /* Kernel_ApplyExtraCutMateria */
    BATTLE_MATERIA_TYPE_DOUBLE_CAST = 0x3,    /* Kernel_ApplyDoubleCastMateria: W-Magic, W-Summon, W-Item */
    BATTLE_MATERIA_TYPE_MEGA_ALL = 0x4,
    BATTLE_MATERIA_TYPE_SUPPORT = 0x5,        /* pairs with the materia in the linked slot */
    BATTLE_MATERIA_TYPE_COMMAND = 0x6,
    BATTLE_MATERIA_TYPE_ENEMY_SKILL = 0x7,
    BATTLE_MATERIA_TYPE_MASTER_COMMAND = 0x8,
    BATTLE_MATERIA_TYPE_MAGIC = 0x9,
    BATTLE_MATERIA_TYPE_MASTER_MAGIC = 0xA,
    BATTLE_MATERIA_TYPE_SUMMON = 0xB,
    BATTLE_MATERIA_TYPE_MASTER_SUMMON = 0xC
};

/* A materia's subtype, the high nibble of typeNibbleByte, for the types whose handler switches on it;
 * each name says the type and the helper it selects. */
enum BattleMateriaSubtype {
    BATTLE_MATERIA_SUBTYPE_INDEPENDENT_ABILITY_FLAG = 0, /* Kernel_ApplyMateriaAbilityFlag */
    BATTLE_MATERIA_SUBTYPE_INDEPENDENT_STAT_PLUS = 2,    /* Kernel_ApplyMateriaStatPlus */
    BATTLE_MATERIA_SUBTYPE_INDEPENDENT_LONG_RANGE = 3,   /* Kernel_ApplyMateriaLongRange */
    BATTLE_MATERIA_SUBTYPE_INDEPENDENT_EXP_BONUS = 4,    /* Kernel_ApplyMateriaExpBonus */
    BATTLE_MATERIA_SUBTYPE_ENCOUNTER_MODIFIER = 2,       /* Kernel_ApplyMateriaEncounterModifier */
    BATTLE_MATERIA_SUBTYPE_ENCOUNTER_RATE = 4,           /* Kernel_ApplyMateriaEncounterRate */
    BATTLE_MATERIA_SUBTYPE_EXTRA_CUT_COMMAND = 1,        /* Kernel_GrantExtraCutCommand */
    BATTLE_MATERIA_SUBTYPE_SUPPORT_PERCENT = 2,          /* Kernel_DescribeSupportMateriaPercent */
    BATTLE_MATERIA_SUBTYPE_SUPPORT_COUNT = 3             /* Kernel_DescribeSupportMateriaCount */
};

/* A support materia's effect, the primaryCommandByte of its kernel record, as
 * Kernel_DispatchSupportEffect applies it to the partner materia. */
enum BattleSupportMateriaEffect {
    BATTLE_SUPPORT_EFFECT_ALL = 0x51,           /* Kernel_ApplyAllSupport */
    BATTLE_SUPPORT_EFFECT_COUNTER = 0x54,       /* a BATTLE_REACTION_COUNTER trigger */
    BATTLE_SUPPORT_EFFECT_MAGIC_COUNTER = 0x55, /* a BATTLE_REACTION_MAGIC_COUNTER trigger */
    BATTLE_SUPPORT_EFFECT_SNEAK_ATTACK = 0x56,
    BATTLE_SUPPORT_EFFECT_FINAL_ATTACK = 0x57,
    BATTLE_SUPPORT_EFFECT_MP_TURBO = 0x58,      /* Kernel_ApplyMpTurboSupport */
    BATTLE_SUPPORT_EFFECT_MP_ABSORB = 0x59,     /* BATTLE_SUPPORT_MP_ABSORB on the partner's slots */
    BATTLE_SUPPORT_EFFECT_HP_ABSORB = 0x5A,
    BATTLE_SUPPORT_EFFECT_ADDED_CUT = 0x5C,
    BATTLE_SUPPORT_EFFECT_STEAL_AS_WELL = 0x5D,
    BATTLE_SUPPORT_EFFECT_ELEMENTAL = 0x5E,     /* Kernel_ApplyMateriaElement */
    BATTLE_SUPPORT_EFFECT_ADDED_EFFECT = 0x5F,  /* Kernel_ApplyMateriaStatusMask */
    BATTLE_SUPPORT_EFFECT_QUADRA_MAGIC = 0x63,  /* Kernel_ApplyQuadraMagicSupport */
    BATTLE_SUPPORT_EFFECT_MEGA_ALL = 0x64       /* Mega All detail panel kind */
};

/* g_BattleAttackOpStateFlags bits during Battle_AttackOpExecute and reflect handling. */
enum BattleAttackOpStateFlag {
    BATTLE_ATTACK_OP_RUNNING = 0x01,      /* resolution loop stays active */
    BATTLE_ATTACK_OP_REFLECTED = 0x02,    /* current wave bounced off Reflect */
    BATTLE_ATTACK_OP_REPEAT_CAST = 0x04,  /* extra repeat cast queued after current wave */
    BATTLE_ATTACK_OP_STARTED = 0x08       /* attack op resolution entered */
};

/* A command or ability slot's menu flags (written when the actor's menu opens): GREYED when the
 * actor cannot use it (too little MP), MULTI_ARROW for the red arrow of an All or multi-hit slot. */
enum BattleMenuSlotFlag {
    BATTLE_MENU_SLOT_GREYED = 0x02,
    BATTLE_MENU_SLOT_MULTI_ARROW = 0x10
};

/* Battle command ids (BattleCommandSlot.commandId, g_BtlIssuedCommandId, the command Battle_PostCommand
 * queues; also the index of the command's name in KERNEL_TEXT_COMMAND). */
enum BattleCommandId {
    BATTLE_COMMAND_ATTACK = 0x01,       /* every member's default, in menu slot 0 (Kernel_InitActorMateriaScratchState) */
    BATTLE_COMMAND_MAGIC = 0x02,
    BATTLE_COMMAND_SUMMON = 0x03,
    BATTLE_COMMAND_ITEM = 0x04,
    BATTLE_COMMAND_STEAL = 0x05,        /* Battle_UpdateCommandAvailability pairs it with Mug */
    BATTLE_COMMAND_SENSE = 0x06,
    BATTLE_COMMAND_COIN = 0x07,
    BATTLE_COMMAND_THROW = 0x08,
    BATTLE_COMMAND_MORPH = 0x09,
    BATTLE_COMMAND_DEATHBLOW = 0x0A,
    BATTLE_COMMAND_MANIPULATE = 0x0B,
    BATTLE_COMMAND_MIME = 0x0C,         /* repeats the last actions (Battle_AttackOpReplayLastActions, which skips a Mime) */
    BATTLE_COMMAND_ENEMY_SKILL = 0x0D,
    BATTLE_COMMAND_MUG = 0x11,          /* Steal with an attack */
    BATTLE_COMMAND_CHANGE = 0x12,      /* Left on the command menu: swap rows */
    BATTLE_COMMAND_DEFEND = 0x13,       /* Right on the command menu */
    BATTLE_COMMAND_LIMIT = 0x14,
    BATTLE_COMMAND_W_MAGIC = 0x15,      /* Magic cast twice (g_BattleCommandRemapTable maps it to Magic) */
    BATTLE_COMMAND_W_SUMMON = 0x16,
    BATTLE_COMMAND_W_ITEM = 0x17,
    BATTLE_COMMAND_SLASH_ALL = 0x18,    /* the four attack-command upgrades: Battle_RefreshActorCommandTargeting gives */
    BATTLE_COMMAND_2X_CUT = 0x19,       /* them allCount 0xFF and all but 2x-Cut the multiple-target flags */
    BATTLE_COMMAND_FLASH = 0x1A,
    BATTLE_COMMAND_4X_CUT = 0x1B,
    BATTLE_COMMAND_ENEMY_ATTACK = 0x20  /* an enemy's own attack, as a controlled enemy issues it */
};

/* What choosing a battle command does (BattleCommandSlot.cursorAction, the KERNEL command's first
 * byte; copied to g_BtlSelectedCommand): Battle_WindowFrameCommandMenu opens the list or the target
 * cursor it names. The W- variants open the same list twice (g_BattleCommandMenuPhase 1). */
enum BattleCursorAction {
    BATTLE_CURSOR_ACTION_TARGET = 0,              /* the target cursor, over the scene's targetExtraMask */
    BATTLE_CURSOR_ACTION_MAGIC = 1,
    BATTLE_CURSOR_ACTION_SUMMON = 2,
    BATTLE_CURSOR_ACTION_ITEM = 3,
    BATTLE_CURSOR_ACTION_ENEMY_SKILL = 4,
    BATTLE_CURSOR_ACTION_THROW = 5,               /* the item list, of throwable items */
    BATTLE_CURSOR_ACTION_LIMIT = 6,
    BATTLE_CURSOR_ACTION_TARGET_VALID_MASK = 7,   /* the target cursor, over the scene's targetValidMask */
    BATTLE_CURSOR_ACTION_W_MAGIC = 8,
    BATTLE_CURSOR_ACTION_W_SUMMON = 9,
    BATTLE_CURSOR_ACTION_W_ITEM = 10,
    BATTLE_CURSOR_ACTION_COIN = 11                /* the gil amount prompt; on frames with bit 0x40 of
                                                   * g_BtlMenuFrameCounter clear, the Throw list */
};

/* MenuPartyStats.flags (+0x23), the bits the materia load sets (Kernel_InitActorMateriaScratchState
 * clears the byte). Bit 0x01 is set by an independent materia's ability flag; its meaning is not
 * proven here. */
enum PartyStatsFlag {
    PARTY_STATS_LONG_RANGE = 0x04,  /* Kernel_SetLongRangeFlag */
    PARTY_STATS_HP_MP_SWAP = 0x08   /* Battle_LoadCombatantFromSavemap swaps max HP and max MP */
};

/* PartyBattleModifiers.preemptiveOdds (include/ff7/types/menu.h): the odds and the mastered bit. */
enum PartyPreemptiveBits {
    PARTY_PREEMPTIVE_ODDS_MASK = 0x7F,
    PARTY_PREEMPTIVE_MASTERED = 0x80
};

/* Battle_RunOpeningSequence's states (g_BattleOpeningSequenceState). */
enum BattleOpeningSequenceState {
    BATTLE_OPENING_SEQ_INIT = 0,         /* loads the stage and camera, resets the sequence, starts the screen wipe */
    BATTLE_OPENING_SEQ_STEP_FRAME = 1,   /* steps one more frame */
    BATTLE_OPENING_SEQ_WAIT_MENU = 2,    /* waits for g_MenuEntryState to clear, then inits the opening UI */
    BATTLE_OPENING_SEQ_LOAD_ACTORS = 3   /* runs g_BattleSceneState's opening substates */
};

/* Battle_RunNextRoundSequence's states (g_BattleNextRoundSequenceState). */
enum BattleNextRoundSequenceState {
    BATTLE_NEXT_ROUND_SEQ_INIT = 0,              /* hides the enemies, rebuilds the formation, applies the camera */
    BATTLE_NEXT_ROUND_SEQ_WAIT_ENEMIES_GONE = 1, /* waits until no enemy actor has state flag 0x20 */
    BATTLE_NEXT_ROUND_SEQ_RESET_POSES = 2,       /* resets the enemy poses */
    BATTLE_NEXT_ROUND_SEQ_LOAD_ACTORS = 3        /* runs g_BattleSceneState's next-round substates */
};

/* g_BattleSceneState: what Battle_Update3dScene runs each frame. The opening and next-round
 * sequences step it through their loading substates; BATTLE_SCENE_STATE_RUNNING is the battle
 * proper (the only state Start pauses in), BATTLE_SCENE_STATE_ROUND_END follows a win, a loss, an
 * escape or the end of a round. (The tag BattleSceneState is the scene struct's.) */
enum BattleSceneStateCode {
    BATTLE_SCENE_STATE_OPENING_LOAD_ENEMIES = 0,  /* reloads the enemy model banks, starts the countdown */
    BATTLE_SCENE_STATE_OPENING_WAIT_ENEMIES = 1,  /* waits for the enemy banks, places the enemies */
    BATTLE_SCENE_STATE_OPENING_WAIT_PARTY = 2,    /* waits for the party banks, sets up the round's actors */
    BATTLE_SCENE_STATE_OPENING_COUNTDOWN = 3,     /* waits for g_BattleEnterFramesToWait */
    BATTLE_SCENE_STATE_RUNNING = 4,
    BATTLE_SCENE_STATE_ROUND_END = 5,
    BATTLE_SCENE_STATE_OPENING_START_ENEMIES = 6, /* starts the actor scripts, marks the enemies loaded */
    BATTLE_SCENE_STATE_NEXT_ROUND_LOAD_ENEMIES = 7,
    BATTLE_SCENE_STATE_NEXT_ROUND_WAIT_ENEMIES = 8,
    BATTLE_SCENE_STATE_NEXT_ROUND_START_ENEMIES = 9,
    BATTLE_SCENE_STATE_NEXT_ROUND_COUNTDOWN = 0xA
};

/* g_BattleState.layoutType values that are not a BattleLayout: g_BattleLayoutTypes maps layouts
 * 0..4 to themselves (the code compares them with BattleLayout), 3..7 to 3 and 8 to this. */
enum BattleLayoutType {
    BATTLE_LAYOUT_TYPE_FRONT_ROW_LOCKED = 5 /* BATTLE_LAYOUT_FRONT_ROW_LOCKED's type; seeds the ATB gauges as normal */
};

/* Battle_LoadSceneNextRound's state (g_BattleNextRoundLoadState); each case falls through to the
 * next until one breaks. */
enum BattleNextRoundLoadState {
    BATTLE_NEXT_ROUND_LOAD_PREPARE = 0,      /* drops queued actions, reloads the party after round one */
    BATTLE_NEXT_ROUND_LOAD_SCENE = 1,        /* Battle_LoadSceneRecord until g_BattleSceneRecordLoaded */
    BATTLE_NEXT_ROUND_LOAD_INIT_ENEMIES = 2  /* enemies, masks, init AI, ATB gauges; marks the load done */
};

/* Battle_RunPhaseMachine's step inside BATTLE_PHASE_ARENA_ROUND (g_BattleArenaRoundStep). */
enum BattleArenaRoundStep {
    BATTLE_ARENA_ROUND_STEP_CHECK = 0, /* a round break when an enemy fell in a Battle Square round */
    BATTLE_ARENA_ROUND_STEP_BREAK = 1  /* Battle_RunArenaRoundBreak until g_BattleArenaRoundBreakDone */
};

/* Battle_RunPhaseMachine's step inside BATTLE_PHASE_NEXT_BATTLE (g_BattleNextBattleStep). */
enum BattleNextBattleStep {
    BATTLE_NEXT_BATTLE_STEP_CHECK_OUTCOME = 0, /* a defeat, flight, escape or timeout exits */
    BATTLE_NEXT_BATTLE_STEP_NEXT_SCENE = 1,    /* the scene's nextScene; the Battle Square chooses */
    BATTLE_NEXT_BATTLE_STEP_ARENA_CHOOSER = 2, /* Battle_RunArenaNextBattleChooser */
    BATTLE_NEXT_BATTLE_STEP_START = 3          /* starts the next battle, or exits on 0xFFFF */
};

/* Battle_RunArenaNextBattleChooser's state (g_BattleSquareState, coloss.cpp). */
enum BattleArenaChooserState {
    BATTLE_ARENA_CHOOSER_COLLECT = 0,       /* the opponent slots that are set; none ends the chooser */
    BATTLE_ARENA_CHOOSER_PLAY_SEQUENCE = 1, /* Battle_PlaySequence until g_BattleSequencePlayDone */
    BATTLE_ARENA_CHOOSER_LOAD = 2,          /* loads CO.BIN, picks the opponent, writes HP/MP back; round 8 ends */
    BATTLE_ARENA_CHOOSER_SLOT_PENALTY = 3   /* Battle_RunArenaSlotPenaltySelection until it is done */
};

/* Battle_RunPhaseMachine's step inside BATTLE_PHASE_EXIT (g_BattleExitStep). */
enum BattleExitStep {
    BATTLE_EXIT_STEP_PLAY_SEQUENCE = 0, /* Battle_PlaySequence until g_BattleSequencePlayDone */
    BATTLE_EXIT_STEP_END_SEQUENCE = 1   /* Battle_RunEndSequence, then the module switch */
};

/* BattleStageDescriptor.stageType: how Battle3d_ResetStageNodes parents the stage nodes and how
 * Battle3d_UpdateAndDrawStage animates and draws the stage parts. */
enum BattleStageType {
    BATTLE_STAGE_TYPE_DEPTH_SCROLL = 0,   /* all ten nodes parented; eight layers scroll along Z */
    BATTLE_STAGE_TYPE_STATIC = 1,         /* part 1 and parts 5..nodeCount-1 drawn after the sorted table */
    BATTLE_STAGE_TYPE_SPLIT_SCROLL = 2,   /* the enemies stand on node 1, the party on node 0; nodes 2 and 4 scroll in Y */
    BATTLE_STAGE_TYPE_YAW_SPIN = 3,       /* nodes 1..3 turn about Y by 1, 2 and 4 a frame */
    BATTLE_STAGE_TYPE_ROLL_SPIN = 4,      /* nodes 1..3 turn about Z by 8, 6 and 4 a frame */
    BATTLE_STAGE_TYPE_STATIC_SORTED = 5   /* BATTLE_STAGE_TYPE_STATIC with those parts depth-sorted */
};

/* g_BattleStageHideFlags: the stage parts Battle3d_UpdateAndDrawStage skips. Effects set the three
 * part bits to hide the whole stage; summons set BATTLE_STAGE_HIDE_RAIN. */
enum BattleStageHideFlag {
    BATTLE_STAGE_HIDE_OTHER_PARTS = 0x1,   /* every part outside the part-1 group, parts 2..4 included */
    BATTLE_STAGE_HIDE_PART_1_GROUP = 0x2,  /* part 1 and the parts each stage type draws with it */
    BATTLE_STAGE_HIDE_PARTS_2_TO_4 = 0x4,
    BATTLE_STAGE_HIDE_RAIN = 0x8           /* the rain of location 0x3A */
};

/* BattleSeqStep.actionType: what Battle_RunSequenceStep does with a sequence step. */
enum BattleSeqStepType {
    BATTLE_SEQ_STEP_SCREEN_RAMP = 0,     /* schedules Battle_EffectTaskScreenColourRamp */
    BATTLE_SEQ_STEP_ACTION = 1,          /* the step's actor acts: camera, effect, its hit records */
    BATTLE_SEQ_STEP_MESSAGE = 2,         /* enqueues a message window */
    BATTLE_SEQ_STEP_CHAINED_ACTION = 3,  /* an action whose actor is the source of its first hit record */
    BATTLE_SEQ_STEP_SCALE_ACTOR = 4,     /* ramps the actor's scale (Battle3d_ScriptActorScaleRampTask) */
    BATTLE_SEQ_STEP_RESTORE_STATUS = 5   /* restores the actor's saved status and facing */
};

/* The codes of the scene message ring buffer (Battle_AllocSceneMessage) that
 * Battle3d_DrainSceneMessageQueue handles. */
enum BattleSceneMessage {
    BATTLE_SCENE_MSG_START_SLOT3_ACTOR = 1,  /* runs Battle3d_Slot3ActorTask at the record's position */
    BATTLE_SCENE_MSG_STOP_SLOT3_ACTOR = 2,
    BATTLE_SCENE_MSG_HIT_REACTION = 4,       /* hit reaction; the actor takes its hit-display status */
    BATTLE_SCENE_MSG_HIT_REACTION_ALT = 5,   /* handled as BATTLE_SCENE_MSG_HIT_REACTION; no caller posts 5 as a constant */
    BATTLE_SCENE_MSG_HURT = 6,               /* Battle_TriggerActorHurtSequence */
    BATTLE_SCENE_MSG_POLL_EFFECT_THREAD = 7,
    BATTLE_SCENE_MSG_HIT_REACTION_ONLY = 9,  /* hit reaction without the status */
    BATTLE_SCENE_MSG_MULTI_HIT_REACTION = 0xA /* hit reaction for one effect-order entry; counts the hit */
};

/* Battle_RunPhaseMachine's phases (g_BattlePhase, 0x009AAD64). The machine runs phase after
 * phase in one frame (each case falls through to the next) until one yields; the names are
 * from what each case runs and its debug strings. */
enum BattlePhase {
    BATTLE_PHASE_INIT = 0, /* picks the first-round or the next-round handlers */
    BATTLE_PHASE_FINISH_SEQUENCE = 1, /* a next round first plays out the last battle's sequence */
    BATTLE_PHASE_LOAD_SCENE = 2, /* the scene loader runs until g_BattleSceneLoadDone */
    BATTLE_PHASE_INIT_STATUS = 3, /* copies each combatant's status to its actor */
    BATTLE_PHASE_OPENING = 4, /* the opening or next-round sequence */
    BATTLE_PHASE_START = 5, /* idle poses, reopens the party's command menus, clears the outcome */
    BATTLE_PHASE_WAIT_OPENING = 6, /* waits for the sequence; then "Begin main battle loop" */
    BATTLE_PHASE_ACTION_QUEUE = 7, /* runs the action queue; ends the battle once an outcome is set */
    BATTLE_PHASE_PLAY_ACTION = 8, /* plays the chosen action's sequence */
    BATTLE_PHASE_APPLY_ACTION = 9, /* time slices, the action event queue, status transitions, drops */
    BATTLE_PHASE_WAIT_ACTION = 10, /* waits for the sequence those queued */
    BATTLE_PHASE_CHECK_END = 11, /* actor masks, HP/MP, sets the outcome when a side is gone or time is up */
    BATTLE_PHASE_ARENA_ROUND = 12, /* the arena's round break (g_BattleArenaRoundStep) */
    BATTLE_PHASE_NEXT_TURN = 13, /* back to BATTLE_PHASE_ACTION_QUEUE unless the party escaped */
    BATTLE_PHASE_END = 14, /* "End of current battle" */
    BATTLE_PHASE_NEXT_BATTLE = 15, /* the arena's next battle, else BATTLE_PHASE_EXIT (g_BattleNextBattleStep) */
    BATTLE_PHASE_EXIT = 16 /* "Begin end battle stuff": the end sequence, then back to the dispatcher (g_BattleExitStep) */
};

/* gBattleAiGlobals.flags: how the battle ended (Battle_RunEndSequence copies it into
 * g_BattleEndOutcomeFlags). The phase machine sets DEFEAT with OVER when no party member is left,
 * OVER alone when no enemy is left, TIMED_OUT with OVER when a countdown battle's timer ran out;
 * an Escape added effect (5) sets ESCAPED, which ends the turn loop; the arena sets FLED when the
 * player stops. DEFEAT becomes FIELD_BATTLE_FLAG_LOST, FLED and ESCAPED FIELD_BATTLE_FLAG_ESCAPED.
 * 0x01 is tested with OVER and FLED (any of the three runs the action queue in mode 1); nothing in
 * the battle code sets it. */
enum BattleOutcomeFlag {
    BATTLE_OUTCOME_DEFEAT = 0x02,
    BATTLE_OUTCOME_FLED = 0x04,
    BATTLE_OUTCOME_ESCAPED = 0x08,
    BATTLE_OUTCOME_TIMED_OUT = 0x10,
    BATTLE_OUTCOME_OVER = 0x20
};

/* Battle_PostEvent's three queues (Battle_FlushEventQueue calls the queue's handler table,
 * battle_status_effect_handlers.cpp, by the event code): the ATB queue is flushed every ATB tick
 * (Battle_TickAtbAndGauges), the turn queue by Battle_ServiceTurnQueueAndEscape, the action queue
 * by the phase machine after an action. Each code below is named by the handler it runs. */
enum BattleEventQueue {
    BATTLE_EVENT_QUEUE_ATB = 0,
    BATTLE_EVENT_QUEUE_TURN = 1,
    BATTLE_EVENT_QUEUE_ACTION = 2
};

/* BATTLE_EVENT_QUEUE_ATB's codes (g_BattleEventQueue0Handlers). */
enum BattleAtbEvent {
    BATTLE_ATB_EVENT_RESET_ATB = 0, /* Battle_EventResetActorAtb */
    BATTLE_ATB_EVENT_RESET_LIMIT_READY = 1, /* Battle_ResetLimitReadyState */
    BATTLE_ATB_EVENT_REMOVE_FROM_TURN_ORDER = 2, /* Battle_EventRemoveActorFromTurnOrder */
    BATTLE_ATB_EVENT_ADD_ITEM_DROP = 3, /* Battle_EventAddPendingItemDrop */
    BATTLE_ATB_EVENT_RELEASE_COMMAND_MENU = 4, /* Battle_EventReleaseActorCommandMenu */
    BATTLE_ATB_EVENT_CANCEL_TURN = 5, /* Battle_EventCancelActorTurn */
    BATTLE_ATB_EVENT_REOPEN_COMMAND_MENU = 6, /* Battle_EventReopenActorCommandMenu */
    BATTLE_ATB_EVENT_SET_ATB_FREEZE = 7, /* Battle_EventSetAtbFreezeFlag */
    BATTLE_ATB_EVENT_ENTER_AUTO_CONTROL = 8, /* Battle_EventEnterAutoControl */
    BATTLE_ATB_EVENT_LEAVE_AUTO_CONTROL = 9, /* Battle_EventLeaveAutoControl */
    BATTLE_ATB_EVENT_SET_MENU_SUPPRESSED = 10, /* Battle_EventSetActorMenuSuppressed */
    BATTLE_ATB_EVENT_CONSUME_RANDOM_ITEM = 11, /* Battle_ConsumeRandomInventoryItem */
    BATTLE_ATB_EVENT_SET_DIRTY_FLAGS = 12, /* Battle_EventSetActorDirtyFlags */
    BATTLE_ATB_EVENT_REBUILD_ABILITY_LISTS = 13, /* Battle_EventRebuildActorAbilityLists */
    BATTLE_ATB_EVENT_CLEAR_ENEMY_MENU_MASKS = 14, /* Battle_EventClearEnemySlotMenuMasks */
    BATTLE_ATB_EVENT_REOPEN_PENDING_MENUS = 15, /* Battle_ReopenPendingCommandMenus */
    BATTLE_ATB_EVENT_CONSUME_ITEM = 16 /* Battle_ConsumeInventoryItemById */
};

/* BATTLE_EVENT_QUEUE_TURN's codes (g_BattleEventQueue1Handlers). */
enum BattleTurnEvent {
    BATTLE_TURN_EVENT_NONE = 0, /* Battle_EventQueue1NoOp0 */
    BATTLE_TURN_EVENT_SHOW_LIMIT_BANNER = 1, /* Battle_EventShowLimitBreakBanner */
    BATTLE_TURN_EVENT_ITEM_STOLEN = 2 /* Battle_EventQueueItemStolen */
};

/* BATTLE_EVENT_QUEUE_ACTION's codes (g_BattleEventQueue2Handlers). */
enum BattleActionEvent {
    BATTLE_ACTION_EVENT_STATUS_TIMER = 0, /* 0..15: the status timer of that slot ran out (Battle_StatusTimerExpire*; the code is the slot) */
    BATTLE_ACTION_EVENT_NONE = 16, /* Battle_EventQueue2NoOp16 */
    BATTLE_ACTION_EVENT_CLEAR_LIMIT_GAUGE = 17, /* Battle_ClearLimitGauge */
    BATTLE_ACTION_EVENT_REBUILD_ABILITY_LISTS = 18, /* Battle_EventQueueRebuildAbilityLists: posts BATTLE_ATB_EVENT_REBUILD_ABILITY_LISTS */
    BATTLE_ACTION_EVENT_REMOVE_PARTY_ACTORS = 19, /* Battle_RemovePartyActorsFromBattle (the payload is the party mask) */
    BATTLE_ACTION_EVENT_ITEM_STOLEN_MESSAGE = 20, /* Battle_EventPostItemStolenMessage */
    BATTLE_ACTION_EVENT_SET_DIRTY_FLAGS = 21, /* Battle_EventQueueSetDirtyFlags: posts BATTLE_ATB_EVENT_SET_DIRTY_FLAGS */
    BATTLE_ACTION_EVENT_SET_TIMING_FLAG_40 = 22, /* Battle_SetActorTimingFlag40 (timingFlags |= 0x40) */
    BATTLE_ACTION_EVENT_PUSH_HIT_ANIM = 23, /* Battle_EventPushHitAnimStep (the payload is the attack index) */
    BATTLE_ACTION_EVENT_COPY_PARTY_STAT_WORDS = 24 /* Battle_CopyPartyStatWords */
};

/* Battle_RunActionQueue's states (the state word of the queue runner), one level of priority at a
 * time; each case falls through to the next until one breaks. */
enum BattleActionQueueStep {
    BATTLE_QUEUE_STEP_RESET = 0, /* back to level 0 */
    BATTLE_QUEUE_STEP_CHECK_LEVEL = 1, /* done when the level reaches maxPriority; skips a level with nothing queued */
    BATTLE_QUEUE_STEP_FIND_ENTRY = 2, /* moves the level's next queued action into the current action */
    BATTLE_QUEUE_STEP_RUN_ENTRY = 3, /* runs it through BattleActionQueuePhase */
    BATTLE_QUEUE_STEP_NEXT_LEVEL = 4,
    BATTLE_QUEUE_STEP_FINISH = 5 /* repeats while the AI queued something, else the sweep is complete */
};

/* The phases of BATTLE_QUEUE_STEP_RUN_ENTRY. */
enum BattleActionQueuePhase {
    BATTLE_QUEUE_PHASE_TAKE_ACTOR = 0, /* the queued actor; none ends the step */
    BATTLE_QUEUE_PHASE_CHECK_ACTOR = 1, /* skips a dead or absent actor; closes the turn of one not waiting */
    BATTLE_QUEUE_PHASE_PICK_COMMAND = 2, /* without a base command: the enemy's forced attack or its AI script */
    BATTLE_QUEUE_PHASE_EXECUTE = 3, /* BATTLE_QUEUE_SUBPHASE_*: Battle_ExecuteAction until it finishes */
    BATTLE_QUEUE_PHASE_FORMATION_AI = 4 /* the formation's AI events after the action */
};

enum BattleActionQueueSubPhase {
    BATTLE_QUEUE_SUBPHASE_PREPARE = 0, /* releases the command menus, a Berserk member gets its base command */
    BATTLE_QUEUE_SUBPHASE_RUN = 1 /* Battle_ExecuteAction; busy while it yields */
};

/* Battle_ExecuteAction's stages. */
enum BattleActionExecStage {
    BATTLE_EXEC_STAGE_SETUP = 0, /* fills the command context from the action */
    BATTLE_EXEC_STAGE_RUN_PROGRAM = 1, /* runs the command's attack program */
    BATTLE_EXEC_STAGE_FINISH = 2 /* records the action, hit reactions, AI events, Lucky 7s */
};

/* The opcodes of g_BattleAttackPrograms (g_BattleAttackOpCallbacks). */
enum BattleAttackProgramOp {
    BATTLE_ATTACK_OP_AWAIT = 0x14, /* yields until the callback sets g_BattleAttackOpContinue */
    BATTLE_ATTACK_OP_END = 0x1F
};

/* The battle actor slots: the party in 0..2, slot 3 unused by the roster, the enemies from 4 (ten
 * actors in all). */
enum BattleActorSlot {
    BATTLE_PARTY_SIZE = 3,
    BATTLE_FIRST_ENEMY_SLOT = 4,
    BATTLE_ACTOR_COUNT = 10,
    BATTLE_ENEMY_SLOT_0 = 4,
    BATTLE_ENEMY_SLOT_1 = 5,
    BATTLE_ENEMY_SLOT_2 = 6,
    BATTLE_ENEMY_SLOT_3 = 7,
    BATTLE_ENEMY_SLOT_4 = 8,
    BATTLE_ENEMY_SLOT_5 = 9
};

/* BattlePartyActor.queuedSupportCodes: an empty entry. */
enum BattleSupportCode {
    BATTLE_SUPPORT_CODE_NONE = 0xFFFF
};

/* The HP at which a party member gets the Lucky 7s free attack. */
enum BattleLucky7s {
    BATTLE_LUCKY_7S_HP = 7777
};

/* gBattleAiGlobals.arenaFlags, the bits the battle load and the savemap write-back prove. */
enum BattleArenaFlag {
    BATTLE_ARENA_BREAK_ACTIVE = 0x2, /* set while the slot penalty screen or the round break holds the command menus; Battle_RunArenaRoundBreak clears it */
    BATTLE_ARENA_KEEP_ROUND_HP = 0x4, /* Battle_LoadSceneFirstRound restores the enemies' HP from the table the previous round's end filled */
    BATTLE_ARENA_FIXED_ATB = 0x8, /* speed fixed at 0x80, no Lucky 7s check, the party's ATB gauges full and the enemies' empty */
    BATTLE_ARENA_ALL_LIMITS = 0x10 /* the write-back gives character 0 every usable limit */
};

/* The phases of the Battle Square's two question state machines, g_ArenaSlotPenaltyState
 * (Battle_RunArenaSlotPenaltySelection) and g_ArenaRoundBreakState (Battle_RunArenaRoundBreak). */
enum BattleArenaQuestionPhase {
    BATTLE_ARENA_PHASE_START = 0,       /* hold the command menus */
    BATTLE_ARENA_PHASE_PLAY_INTRO = 1,  /* play four battle sequences */
    BATTLE_ARENA_PHASE_OPEN_WINDOW = 2,
    BATTLE_ARENA_PHASE_WAIT_ANSWER = 3, /* steps through BattleArenaWaitStep */
    BATTLE_ARENA_PHASE_FINISH = 4       /* act on the answer */
};

/* The steps of BATTLE_ARENA_PHASE_WAIT_ANSWER (g_ArenaSlotPenaltySubState, g_ArenaRoundBreakSubState). */
enum BattleArenaWaitStep {
    BATTLE_ARENA_WAIT_CHECK_ANSWER = 0,
    BATTLE_ARENA_WAIT_PLAY_SEQUENCE = 1
};

/* Bits of g_ArenaSelectionResult (Battle_SetArenaSelectionResult). */
enum BattleArenaSelectionFlag {
    BATTLE_ARENA_SELECTION_ANSWERED = 1,
    BATTLE_ARENA_SELECTION_YES = 2
};

/* BattleCombatant.flags (+0x04, gBattleAiGlobals.combatants / g_aCombatants): the bits the code
 * proves, each with the code that proves it. 0x01 and 0x04 are not read or written by name. */
enum BattleCombatantFlag {
    BATTLE_COMBATANT_SECOND_GROUP = 0x2, /* Battle_AssignActorSideMasks: the combatant is in target group 2 (g_BattleScene.targetMaskTable[2]); a pincer attack puts the enemies the scene flags 2 there */
    BATTLE_COMBATANT_TARGETABLE = 0x8, /* Battle_RecomputeActorMasks collects these into g_BattleScene.targetMaskBits; an enemy gets it back when its Death is cleared and loses it when it is removed or the arena restores it dead */
    BATTLE_COMBATANT_AI_CONTROLLED = 0x10, /* its turn runs its AI script (Battle_RunActionQueue); enemies have it, a party member gets it from a stat mutation (Battle_ApplyActorStatMutation); reaction triggers roll only without it */
    BATTLE_COMBATANT_DEFENDING = 0x20, /* set when the combatant queues command 0x13, cleared by its next queued action; halves the physical damage it takes */
    BATTLE_COMBATANT_BACK_ROW = 0x40, /* halves physical damage taken, and dealt at short range; a back attack swaps the party's rows, side and pincer layouts clear it, the toggle-row added effect flips it; an enemy behind the front-most enemy row gets it */
    BATTLE_COMBATANT_FACING_FLIPPED = 0x80, /* the combatant faces the other way (Battle_AssignActorSideMasks; a hit from behind flips it, Battle_CommitFacingFlip) */
    BATTLE_COMBATANT_IMMUNE_PHYSICAL = 0x100, /* Battle_IsTargetImmuneToAttackKind for a physical attack */
    BATTLE_COMBATANT_IMMUNE_MAGIC = 0x200, /* Battle_IsTargetImmuneToAttackKind for an attack with BATTLE_SPECIAL_NOT_PHYSICAL */
    BATTLE_COMBATANT_NO_CAST_CHECK = 0x400, /* Battle_ChargeMpAndCheckCastable skips the Silence and Frog checks and charges no MP */
    BATTLE_COMBATANT_COVERED = 0x800, /* an enemy in a row in front covers it (their cover masks overlap): not in g_BattleScene.targetValidMask */
    BATTLE_COMBATANT_IMMUNE_TO_DEATH = 0x1000, /* its status immunity mask gets Death; its death posts no AI death action */
    BATTLE_COMBATANT_DEATH_HANDLED = 0x2000, /* Battle_HandleActorDeath ran for it; cleared when its Death status is cleared */
    BATTLE_COMBATANT_ALWAYS_MISSED = 0x4000 /* every hit on it misses (BATTLE_RESULT_MISS) */
};

/* KernelAccessory.autoStatusKind: the status Kernel_ApplyAccessoryAutoStatus keeps on the wearer
 * (0xFF: none). Kinds 4 and 5 do nothing. */
enum BattleAccessoryAutoStatus {
    BATTLE_ACCESSORY_AUTO_HASTE = 0,
    BATTLE_ACCESSORY_AUTO_BERSERK = 1,
    BATTLE_ACCESSORY_AUTO_DEATH_SENTENCE = 2, /* its timer is set to 0xFF: it never runs out */
    BATTLE_ACCESSORY_AUTO_REFLECT = 3,
    BATTLE_ACCESSORY_AUTO_BARRIERS = 6 /* Barrier and MBarrier; not added to the actor's immunity mask */
};

/* Byte offsets into the battle frame's PlayStation ordering table, the address that
 * g_BattleCurrentOrderingTable holds (table A or B, swapped each frame). The table starts at
 * BATTLE_OT_DEPTH_SORTED with 4,096 depth-sorted entries, 4 bytes each; a draw call passes the entry
 * and a depth shift. The PC renderer ignores the ordering-table argument of every draw call: the
 * offsets are the ones the PlayStation code passed, and they no longer order anything. */
enum BattleOrderingTableOffset {
    BATTLE_OT_DEPTH_SORTED = 0x70,      /* entry 0 of the depth-sorted table */
    BATTLE_OT_SORTED_LAST_3 = 0x4060,   /* entry 0xFFC */
    BATTLE_OT_SORTED_LAST_2 = 0x4064,   /* entry 0xFFD */
    BATTLE_OT_SORTED_LAST_1 = 0x4068,   /* entry 0xFFE */
    BATTLE_OT_SORTED_LAST = 0x406C,     /* entry 0xFFF, the last one */
    BATTLE_OT_AFTER_SORTED_0 = 0x4070,  /* the entries after the sorted table */
    BATTLE_OT_AFTER_SORTED_2 = 0x4078,
    BATTLE_OT_AFTER_SORTED_5 = 0x4084,
    BATTLE_OT_AFTER_SORTED_13 = 0x40A4,
    BATTLE_OT_AFTER_SORTED_29 = 0x40E4
};

/* Effect ids of the summon effects (BattleActor::effectId when the running action is a summon):
 * the index into g_BattleSummonModelFileTable and the case of Battle_SummonEffectTask's dispatch. */
enum BattleSummonEffect {
    BATTLE_SUMMON_CHOCOBO_MOG = 0x00,
    BATTLE_SUMMON_SHIVA = 0x01,
    BATTLE_SUMMON_IFRIT = 0x02,
    BATTLE_SUMMON_RAMUH = 0x03,
    BATTLE_SUMMON_TITAN = 0x04,
    BATTLE_SUMMON_ODIN_ZANTETSUKEN = 0x05,
    BATTLE_SUMMON_LEVIATHAN = 0x06,
    BATTLE_SUMMON_BAHAMUT = 0x07,
    BATTLE_SUMMON_KJATA = 0x08,
    BATTLE_SUMMON_ALEXANDER = 0x09,
    BATTLE_SUMMON_PHOENIX = 0x0A,
    BATTLE_SUMMON_NEO_BAHAMUT = 0x0B,
    BATTLE_SUMMON_HADES = 0x0C,
    BATTLE_SUMMON_TYPHON = 0x0D,
    BATTLE_SUMMON_BAHAMUT_ZERO = 0x0E,
    BATTLE_SUMMON_KNIGHTS_OF_ROUND = 0x0F,
    BATTLE_SUMMON_FAT_CHOCOBO = 0x10,
    BATTLE_SUMMON_ODIN_GUNGNIR = 0x11,
    BATTLE_SUMMON_COUNT = 0x12
};

/* The status overlay models Battle_EffectAttachStatusOverlayModel attaches: the index into
 * g_BattleLimitModelFileTable, named after the model file. */
enum BattleOverlayModelFile {
    BATTLE_OVERLAY_MODEL_DEATH = 0,
    BATTLE_OVERLAY_MODEL_TOY_SOLDIER = 1,
    BATTLE_OVERLAY_MODEL_MOG_DANCE = 2,
    BATTLE_OVERLAY_MODEL_L5_DEATH = 3,
    BATTLE_OVERLAY_MODEL_ANGEL = 4,
    BATTLE_OVERLAY_MODEL_CONF = 5,
    BATTLE_OVERLAY_MODEL_COUNT = 6
};

/* The flight kinds of Yuffie's thrown weapons and the Gil coin throw: each has its own timeline task. */
enum BattleYuffieThrowFlightKind {
    BATTLE_YUFFIE_THROW_STRAIGHT = 0, /* flies straight to the target */
    BATTLE_YUFFIE_THROW_CURVED = 1,   /* flies out, then back */
    BATTLE_YUFFIE_THROW_LOOPING = 2,  /* spins while it flies, with trail models */
    BATTLE_YUFFIE_THROW_MULTI_ORBIT = 3 /* one volley at every target */
};

/* The flight of Aeris's thrown weapons (g_BattleThrowAerisWeaponFlag, rand() % 2 per throw): each
 * has its own timeline task. */
enum BattleThrowAerisFlightKind {
    BATTLE_THROW_AERIS_FLIGHT_STRAIGHT = 0, /* BattleFx_ThrowAerisWeaponsStraightTimelineTask */
    BATTLE_THROW_AERIS_FLIGHT_ARC = 1       /* BattleFx_ThrowAerisWeaponsArcTimelineTask: a high spinning arc */
};

/* The Sumi ink effect's variant (g_BattleSpecialEvilPoisonMode): which entry point started it. */
enum BattleEvilPoisonMode {
    BATTLE_EVIL_POISON_MODE_DEFAULT = 0,           /* a 12-frame flight, four sprays */
    BATTLE_EVIL_POISON_MODE_ALT = 1                /* BattleFx_SpecialEnterEvilPoisonAlt: a 10-frame flight, three sprays */
};

/* The flight of Barret's thrown weapon (g_BattleThrowBarretWeaponFlag): which timeline task
 * BattleFx_ThrowBarretWeaponsDispatchKindTask starts. */
enum BattleBarretThrowFlightKind {
    BATTLE_BARRET_THROW_STRAIGHT = 0,   /* BattleFx_ThrowBarretWeaponsStraightTimelineTask */
    BATTLE_BARRET_THROW_TUMBLING = 1    /* BattleFx_ThrowBarretWeaponsTumblingTimelineTask */
};

/* The flight of Cloud's thrown weapon (g_BattleThrowCloudWeaponFlag): which timeline task
 * BattleFx_ThrowCloudWeaponsDispatchKindTask starts; every Cloud weapon sets STRAIGHT. */
enum BattleCloudThrowFlightKind {
    BATTLE_CLOUD_THROW_STRAIGHT = 0     /* BattleFx_ThrowCloudWeaponsStraightTimelineTask */
};

/* The damage program: bytes of g_abDamageStatProgramBlob that Battle_RunDamageProgram executes until it reads the end byte. */
enum BattleDamageProgramOp {
    BATTLE_DAMAGE_PROGRAM_END = 8
};

/* Bits of g_abDamageFormulaFlags, which Battle_DecodeDamageCalcByte reads for the formula chosen by the calculation byte. */
enum BattleDamageFormulaFlag {
    BATTLE_DAMAGE_FORMULA_PHYSICAL = 0x1,    /* clears BATTLE_SPECIAL_NOT_PHYSICAL */
    BATTLE_DAMAGE_FORMULA_SECOND_BANK = 0x2, /* adds 0x10 to the formula index */
    BATTLE_DAMAGE_FORMULA_EFFECT_FROM_TABLE = 0x4 /* the effect id comes from g_abDamageCalcParamTable */
};

/* Bits of g_BattleLimitFinishingTouchEventFlags, the Garyoten limit's timeline state. */
enum BattleLimitFinishingTouchEvent {
    GARYOTEN_EVENT_RELEASE_FIRST = 0x40,      /* frees the expiry task of BattleFx_LimitBeginFinishingTouch */
    GARYOTEN_EVENT_RELEASE_SECOND = 0x80,     /* frees the expiry task of BattleFx_LimitBeginFinishingTouchAlt */
    GARYOTEN_EVENT_CANCEL = 0x20000000,       /* the target mask is the source actor alone: end now */
    GARYOTEN_EVENT_HOLD = 0x40000000,         /* the timeline and the lifts wait for BattleFx_LimitBeginFinishingTouchAlt */
    GARYOTEN_EVENT_DONE = (int)0x80000000     /* the timeline has ended; frees the draw cursor task */
};

/* BattleActor::effectCategory: the kind of the running action's effect, which picks the dispatch
 * table Battle_RunActorEffect and Battle_RunActorEffectImpact call and the resource map
 * Battle_SelectEffectOverlayProgram preloads from. */
enum BattleEffectCategory {
    BATTLE_EFFECT_CATEGORY_NONE = 0,         /* unset category; action-name window falls through to MAGIC */
    BATTLE_EFFECT_CATEGORY_MAGIC = 2,        /* g_BattleFx_MagicDispatchTable */
    BATTLE_EFFECT_CATEGORY_SUMMON = 3,       /* Battle_RunSummonEffect */
    BATTLE_EFFECT_CATEGORY_GENERAL = 4,      /* g_BattleFx_GeneralDispatchTable, at impact */
    BATTLE_EFFECT_CATEGORY_COIN = 7,         /* Battle_CoinThrowEffect, at impact */
    BATTLE_EFFECT_CATEGORY_THROW = 8,        /* g_BattleFx_ThrowWeaponDispatchTable, at impact */
    BATTLE_EFFECT_CATEGORY_ENEMY_SKILL = 0xD, /* g_BattleFx_BlueMagicDispatchTable */
    BATTLE_EFFECT_CATEGORY_LIMIT = 0x14,     /* g_BattleFx_LimitDispatchTable */
    BATTLE_EFFECT_CATEGORY_SPECIAL = 0x20    /* g_BattleFx_SpecialDispatchTable */
};

/* An actor's animation speed (BattleActorState.motionState), which Battle3d_UpdateActorRenderState
 * sets from its statuses: normal, Haste, Slow, or frozen (Stop, Paralysis, Petrify, a dying fade). */
enum BattleActorMotionState {
    BATTLE_ACTOR_MOTION_NORMAL = 0,
    BATTLE_ACTOR_MOTION_HASTE = 1,
    BATTLE_ACTOR_MOTION_SLOW = 2,
    BATTLE_ACTOR_MOTION_FROZEN = 3
};

/* The status tints of g_BattleOverlay_StatusTintTable that Battle3d_SelectStatusTintIndex picks,
 * highest priority first: Slow-numb, Petrify, Poison, Berserk, Peerless, Darkness, Regen, Manipulate. */
enum BattleStatusTint {
    BATTLE_STATUS_TINT_NONE = 0,
    BATTLE_STATUS_TINT_POISON = 1,
    BATTLE_STATUS_TINT_BERSERK = 3,
    BATTLE_STATUS_TINT_PETRIFY = 5,
    BATTLE_STATUS_TINT_PEERLESS = 6,
    BATTLE_STATUS_TINT_MANIPULATE = 7,
    BATTLE_STATUS_TINT_DARKNESS = 8,
    BATTLE_STATUS_TINT_REGEN = 9,
    BATTLE_STATUS_TINT_SLOW_NUMB = 0xA
};

/* The four face blocks of a battle LMD model, in file order (Battle_BuildPolygonDataFromLmd):
 * textured triangles, textured quads, then untextured Gouraud triangles and quads. */
enum Battle3dLmdFaceBlock {
    BATTLE3D_LMD_BLOCK_TEXTURED_TRIANGLES = 0,
    BATTLE3D_LMD_BLOCK_TEXTURED_QUADS = 1,
    BATTLE3D_LMD_BLOCK_GOURAUD_TRIANGLES = 2,
    BATTLE3D_LMD_BLOCK_GOURAUD_QUADS = 3,
    BATTLE3D_LMD_BLOCK_COUNT = 4
};

/* The opcodes of the UV-scroll script Battle_EffectTaskRunUvScrollScript runs; any other byte yields. */
enum BattleUvScrollOp {
    BATTLE_UV_SCROLL_OP_SET_AND_WAIT = 0xFD, /* low byte, high byte, delay: writes the actor's scroll word, then waits */
    BATTLE_UV_SCROLL_OP_RESTART = 0xFE,      /* back to byte 0 */
    BATTLE_UV_SCROLL_OP_END = 0xFF           /* frees the task */
};

/* Battle_OverlayTaskLiftThenTravel's overlay-task state. */
enum BattleLiftTravelPhase {
    BATTLE_LIFT_TRAVEL_PHASE_INIT = 0,   /* loads the lift counter */
    BATTLE_LIFT_TRAVEL_PHASE_LIFT = 1,   /* raises the actor */
    BATTLE_LIFT_TRAVEL_PHASE_TRAVEL = 2  /* moves it in X/Z while it comes down, then frees the task */
};

/* Battle_OverlayTaskTravelThenDrop's overlay-task state. */
enum BattleTravelDropPhase {
    BATTLE_TRAVEL_DROP_PHASE_INIT = 0,   /* loads the travel counter and the Y step */
    BATTLE_TRAVEL_DROP_PHASE_TRAVEL = 1, /* moves the actor in X/Y/Z */
    BATTLE_TRAVEL_DROP_PHASE_DROP = 2    /* lowers it, then frees the task */
};

/* The magic effect ids (BattleActor::effectId under BATTLE_EFFECT_CATEGORY_MAGIC, the index into
 * g_BattleFx_MagicDispatchTable) that Battle_RunActorEffect and Battle_SelectEffectOverlayProgram
 * send to another effect while g_BattleSequenceStepBusy is set. */
enum BattleMagicEffect {
    BATTLE_MAGIC_EFFECT_REVIVE_1 = 7, /* BattleFx_BlueEnterAngelWhisperB */
    BATTLE_MAGIC_EFFECT_REVIVE_2 = 8, /* BattleFx_BlueEnterAngelWhisperC */
    BATTLE_MAGIC_EFFECT_CONFUSE = 12, /* BattleFx_BlueEnterChocobuckle */
    BATTLE_MAGIC_EFFECT_DEATH = 24,   /* BattleFx_MagicEnterDeath */
    BATTLE_MAGIC_EFFECT_ESCAPE = 0x19,
    BATTLE_MAGIC_EFFECT_FIRE3 = 0x1D, /* BattleFx_MagicEnterFire3; BattleFx_MagicEnterFire3Multi */
    BATTLE_MAGIC_EFFECT_ICE3 = 0x20,  /* BattleFx_MagicEnterIce3; BattleFx_MagicLoadAndBeginIce3Multi */
    BATTLE_MAGIC_EFFECT_BOLT3 = 0x23, /* BattleFx_MagicEnterBolt3; BattleFx_MagicEnterBolt3Multi */
    BATTLE_MAGIC_EFFECT_BIO3 = 0x29,  /* BattleFx_MagicEnterBio3; BattleFx_MagicEnterBio3Multi */
    BATTLE_MAGIC_EFFECT_DEMI3 = 0x2C  /* BattleFx_MagicEnterDemi3; BattleFx_MagicLoadAndBeginDemi3Multi */
};

/* Limit effect ids (BattleActor::effectId under BATTLE_EFFECT_CATEGORY_LIMIT, the index into
 * g_BattleFx_LimitDispatchTable) the code tests by name. */
enum BattleLimitEffect {
    BATTLE_LIMIT_EFFECT_BLADE_BEAM = 2, /* BattleFx_LimitEnterBladeBeam; sets g_BattleLimitBladeBeamTargetDiffers */
    BATTLE_LIMIT_EFFECT_EMPTY_45 = 45,
    BATTLE_LIMIT_EFFECT_EMPTY_46 = 46,
    BATTLE_LIMIT_EFFECT_EMPTY_47 = 47,
    BATTLE_LIMIT_EFFECT_EMPTY_48 = 48,
    BATTLE_LIMIT_EFFECT_GROW_SCALE = 56,
    BATTLE_LIMIT_EFFECT_MOG_DANCE = 57,
    BATTLE_LIMIT_EFFECT_TOY_SOLDIER = 58,
    BATTLE_LIMIT_EFFECT_LUCKY_GIRL = 59,
    BATTLE_LIMIT_EFFECT_THUNK_60 = 60,
    BATTLE_LIMIT_EFFECT_DEATH = 61,
    BATTLE_LIMIT_EFFECT_GALIAN_BEAST_0 = 62,
    BATTLE_LIMIT_EFFECT_GALIAN_BEAST_1 = 63,
    BATTLE_LIMIT_EFFECT_GALIAN_BEAST_2 = 64,
    BATTLE_LIMIT_EFFECT_LIVEWIRE = 65,
    BATTLE_LIMIT_EFFECT_GALIAN_BEAST_3 = 66,
    BATTLE_LIMIT_EFFECT_NIGHTMARE = 67,
    BATTLE_LIMIT_EFFECT_GALIAN_BEAST_4 = 68,
    BATTLE_LIMIT_EFFECT_SATAN_SLAM = 69,
    BATTLE_LIMIT_EFFECT_THUNK_70 = 70,
    BATTLE_LIMIT_EFFECT_THUNK_71 = 71,
    BATTLE_LIMIT_EFFECT_TOY_BOX_B = 72,
    BATTLE_LIMIT_EFFECT_TOY_BOX = 73,
    BATTLE_LIMIT_EFFECT_TOY_BOX_C = 74,
    BATTLE_LIMIT_EFFECT_TOY_BOX_D = 75,
    BATTLE_LIMIT_EFFECT_THUNK_76 = 76,
    BATTLE_LIMIT_EFFECT_FINISHING_TOUCH_ALT = 77,
    BATTLE_LIMIT_EFFECT_SATAN_SLAM_ALT = 78,
    BATTLE_LIMIT_EFFECT_BLADE_BEAM_ALT = 79
};

/* The render object of the battle scene (Battle3dScene) that Battle3d_GetSceneRenderObject
 * and Battle3d_GetSceneRenderObjectHigh return for their selector; any other selector gives 0. */
enum Battle3dSceneSlot {
    BATTLE3D_SCENE_SLOT_SEFFECT1 = 1,
    BATTLE3D_SCENE_SLOT_SEFFECT2 = 2,
    BATTLE3D_SCENE_SLOT_SEFFECT3 = 3,
    BATTLE3D_SCENE_SLOT_BATCH_LIST_A = 4,
    BATTLE3D_SCENE_SLOT_OVERLAY_QUAD = 5,
    BATTLE3D_SCENE_SLOT_BATCH_LIST_B = 6,
    BATTLE3D_SCENE_SLOT_QUAD_RECORD = 7
};

/* BattleModel_ConfigureLoadCategory's category, the index into g_battleModelLoadCategories, by
 * the loader that passes it; 0 to 2 load from the record's source, 3 to 5 from
 * g_BattleAssetSourceModeSelector. No caller passes 1. */
enum BattleModelLoadKind {
    BATTLE_MODEL_LOAD_ENEMY = 0,        /* BattleModel_LoadEnemy */
    BATTLE_MODEL_LOAD_UNUSED_1 = 1,     /* shares enemy/party archive path; unused by callers */
    BATTLE_MODEL_LOAD_PARTY = 2,        /* BattleModel_LoadPartyMember */
    BATTLE_MODEL_LOAD_LIMIT = 3,        /* the limit break's animation bank */
    BATTLE_MODEL_LOAD_EFFECT_MODEL = 4, /* battle_effect_model_fade_draw */
    BATTLE_MODEL_LOAD_SUMMON = 5        /* battle_effect_summon_actors */
};

/* Battle_AiVm_AccessSavemapByte's mode: which way the byte moves between the savemap's script
 * banks and gBattleAiGlobals.savemapByte. */
enum BattleAiSavemapAccess {
    BATTLE_AI_SAVEMAP_READ = 0,
    BATTLE_AI_SAVEMAP_WRITE = 1
};

/* The command context's attackFlags bits the code proves. Battle_ApplySupportMateriaFlags sets
 * the four support bits from the slot's BattleSupportFlag; Battle_ResolveHitOnTarget pools 1% of
 * the damage as MP under MP_ABSORB and 10% as HP under HP_ABSORB. */
enum BattleAttackFlag {
    BATTLE_ATTACK_FLAG_STEAL_AS_WELL = 0x4,
    BATTLE_ATTACK_FLAG_HP_ABSORB = 0x40,
    BATTLE_ATTACK_FLAG_MP_ABSORB = 0x80,
    BATTLE_ATTACK_FLAG_MULTIPLE_TARGETS = 0x200, /* Battle_ExecuteAction: more than one target, or a party actor's empty mask */
    BATTLE_ATTACK_FLAG_ADDED_CUT = 0x800,
    BATTLE_ATTACK_FLAG_ALT_RETURN_POSE = 0x1000,
    BATTLE_ATTACK_FLAG_POST_EFFECT_STEP = 0x10000,
    BATTLE_ATTACK_FLAG_SKIP_KO_TARGETS = 0x80000, /* Battle_AttackOpExecute drops KO'd targets; none left ends the action */
    BATTLE_ATTACK_FLAG_NO_ALL_USES = 0x100000, /* a multiple-target use with the command's All counter at 0 */
    BATTLE_ATTACK_FLAG_SUPPORT_CODE = 0x400000 /* the support bits came from a queued support code, not the command menu */
};

/* The command context's elemReaction: how the target's element affinities turn the hit
 * (Battle_ResolveElementAffinity; Battle_ApplyElementalPreDamage/PostDamage apply it). */
enum BattleElementReaction {
    BATTLE_ELEM_REACTION_DEATH = 0x01,         /* inflicts Death (hit display -2) */
    BATTLE_ELEM_REACTION_AUTO_HIT = 0x02,      /* the hit cannot miss (both hit checks, Manipulate); scene.bin's element rate 1 */
    BATTLE_ELEM_REACTION_WEAK = 0x04,          /* damage doubled */
    BATTLE_ELEM_REACTION_HALF = 0x10,          /* damage halved, rounded up */
    BATTLE_ELEM_REACTION_NULLIFY = 0x20,       /* no damage; a status or Earth attack misses */
    BATTLE_ELEM_REACTION_ABSORB = 0x40,        /* damage heals; cure and inflict masks swap */
    BATTLE_ELEM_REACTION_FULL_RECOVERY = 0x80  /* HP and MP to full (hit display -3) */
};

/* The attack's elementMask bits the code proves. */
enum BattleElementBit {
    BATTLE_ELEMENT_RESTORATIVE = 0x200  /* Battle_ResolveHitOnTarget shows the hit as a recovery (BATTLE_DISPLAY_HEAL) */
};

/* Bits of the command context's modifierFlags that Battle_ResolveAttackFacing sets.
 * BACK_ATTACK: the attacker and target are on different sides and face the same way; the physical
 * hit check always hits and the physical formula scales the damage by the target's damageScale / 8.
 * TURN_TARGET: the target turns round to face the attacker (Battle_CommitFacingFlip). */
enum BattleCommandModifierFlag {
    BATTLE_MODIFIER_BACK_ATTACK = 0x1,
    BATTLE_MODIFIER_TURN_TARGET = 0x2
};

/* BattleHitRecord.flags as Battle_ResolveHitOnTarget writes them. */
enum BattleHitRecordFlag {
    BATTLE_HIT_RECORD_HIT = 0x1,            /* the hit landed */
    BATTLE_HIT_RECORD_REFLECTED = 0x2,      /* Reflect bounced it */
    BATTLE_HIT_RECORD_TARGET_KO = 0x4,      /* the target is KO'd after it (Battle_ClearActorAnimHoldFlags clears it) */
    BATTLE_HIT_RECORD_STATUS_CHANGED = 0x8, /* the target's status changed, KO aside */
    BATTLE_HIT_RECORD_BARRIER = 0x10,       /* BATTLE_RESULT_BARRIER */
    BATTLE_HIT_RECORD_MBARRIER = 0x20       /* BATTLE_RESULT_MBARRIER */
};

/* g_BattleEndSequenceState, the steps of Battle_RunEndSequence: resolve the outcome and pick the
 * closing, play the closing for its delay, keep playing while the victory pose or the defeat music
 * is pending (g_BattleEndSubState), set the field's battle flags, release the pending item drops,
 * then hand out the rewards and end. */
enum BattleEndSequenceState {
    BATTLE_END_STATE_RESOLVE_OUTCOME = 0,
    BATTLE_END_STATE_PLAY_CLOSING = 1,
    BATTLE_END_STATE_HOLD_CLOSING = 2,
    BATTLE_END_STATE_SET_FIELD_FLAGS = 3,
    BATTLE_END_STATE_RELEASE_DROPS = 4,
    BATTLE_END_STATE_REWARDS = 5
};

/* g_BattleEndSubState inside BATTLE_END_STATE_HOLD_CLOSING: CHECK tests the victory pose and
 * defeat music flags, PLAY plays one sequence pass and returns to CHECK. */
enum BattleEndSubState {
    BATTLE_END_SUBSTATE_CHECK = 0,
    BATTLE_END_SUBSTATE_PLAY = 1
};

/* The flash BattleFx_MagicLimitFlashSpawnForActor starts (its mode, the effect record's frame
 * counter), named by the model task it runs; SINE_SCALE_WITH_SPRITES adds the radial sprite emitter. */
enum BattleLimitFlashMode {
    BATTLE_LIMIT_FLASH_SINE_SCALE = 0,
    BATTLE_LIMIT_FLASH_GROW = 1,
    BATTLE_LIMIT_FLASH_SHRINK = 2,
    BATTLE_LIMIT_FLASH_SINE_SCALE_WITH_SPRITES = 3
};

/* BattleFxModelDraw.flags bits that BattleFx_SubmitModelDraw reads. The MIRROR bits negate a row of
 * the model's rotation matrix; FADE_TRANSPARENCY makes fadeLevel a 12-bit transparency
 * (Battle3d_FadeLevelToAlpha) instead of a 7-bit opacity doubled into the alpha; SW_BLEND_COLOR
 * blends the draw colour in (GFX_DRAW_SPEC_KIND_BLEND_SAVED_OR_PALETTE) on the software driver.
 * Effects also set 0x08, which this function does not read. */
enum BattleFxModelDrawFlag {
    BATTLE_FX_DRAW_MIRROR_X = 0x01,
    BATTLE_FX_DRAW_MIRROR_Y = 0x02,
    BATTLE_FX_DRAW_MIRROR_Z = 0x04,
    BATTLE_FX_DRAW_SW_BLEND_COLOR = 0x10,
    BATTLE_FX_DRAW_FADE_TRANSPARENCY = 0x80
};

/* g_BattleThrowYuffieKind3Phase, the step of the multi-target throw (BATTLE_YUFFIE_THROW_MULTI_ORBIT)
 * timeline: SHUFFLE shuffles the order of the targets, THROW throws at the next target in that
 * order and shuffles again after the last, DONE once every target has had its share. */
enum BattleYuffieMultiThrowPhase {
    BATTLE_YUFFIE_MULTI_THROW_THROW = 0,
    BATTLE_YUFFIE_MULTI_THROW_SHUFFLE = 1,
    BATTLE_YUFFIE_MULTI_THROW_DONE = 2
};

/* Battle_PublishCameraForFrame's cameraMode (g_BattleCameraTargetIndex, a sequence step's
 * targetIndex): AUTO publishes the auto camera, or the fixed placement camera when the Camera
 * option is fixed; CHANNELS publishes the camera channel scripts. Other values are camera ids. */
enum BattleCameraMode {
    BATTLE_CAMERA_MODE_AUTO = -1,
    BATTLE_CAMERA_MODE_CHANNELS = -2
};

/* The Camera option bits of gSavemap.options as the battle camera reads them: with FIXED alone the
 * battle shows the formation camera (Battle_PublishFixedPlacementCamera). */
enum BattleCameraOption {
    BATTLE_CAMERA_OPTION_FIXED = 0x100,
    BATTLE_CAMERA_OPTION_MASK = 0x300
};

/* A camera actor (g_BattleCameraFocusActor, a camera script's actor) that is not an actor index. */
enum BattleCameraActor {
    BATTLE_CAMERA_ACTOR_TARGETS = 0xF /* the centroid of the running effect's targets (Battle_GetActorMaskCentroid) */
};

/* The menu list Battle_RefreshPendingCommandAvailability refreshes (its selector); the list's
 * dirty bit in the actor's dirtyFlags is 1 << the list. */
enum BattleCommandMenuList {
    BATTLE_MENU_LIST_COMMANDS = 0,      /* Battle_UpdateCommandAvailability */
    BATTLE_MENU_LIST_MAGIC = 1,         /* the spell slots 0x00..0x37 */
    BATTLE_MENU_LIST_SUMMON = 2,        /* the summon slots 0x38..0x47 */
    BATTLE_MENU_LIST_ENEMY_SKILL = 3    /* the enemy-skill slots 0x48..0x5F */
};

/* Bits of a party actor's dirtyFlags (g_BattleState.actors[].dirtyFlags): a menu list to refresh
 * before it is shown, and the lists Battle_UpdateSpellMenuAvailability found with no usable slot,
 * which grey their command. */
enum BattleActorDirtyFlag {
    BATTLE_ACTOR_DIRTY_COMMANDS = 0x01,
    BATTLE_ACTOR_DIRTY_MAGIC = 0x02,
    BATTLE_ACTOR_DIRTY_SUMMON = 0x04,
    BATTLE_ACTOR_DIRTY_ENEMY_SKILL = 0x08,
    BATTLE_ACTOR_MAGIC_UNAVAILABLE = 0x20,
    BATTLE_ACTOR_SUMMON_UNAVAILABLE = 0x40,
    BATTLE_ACTOR_ENEMY_SKILL_UNAVAILABLE = 0x80
};

/* g_BattleSeqPlaybackState: Battle_PlaySequence's state. START closes the step list and loads the
 * enemies' script ids; RUN runs a step a frame until the sequence ends, then goes back to START. */
enum BattleSeqPlaybackState {
    BATTLE_SEQ_PLAYBACK_START = 0,
    BATTLE_SEQ_PLAYBACK_RUN = 1
};

/* g_BattleSeqWaitDoneState: Battle_WaitSequenceDone's state. START finishes at once when no step is
 * queued; PLAY runs Battle_PlaySequence until it is done. */
enum BattleSeqWaitState {
    BATTLE_SEQ_WAIT_START = 0,
    BATTLE_SEQ_WAIT_PLAY = 1
};

/* g_BtlTargetMode: the scene mask Battle_LoadTargetMaskForMode copies into g_BtlTargetAvailableMask
 * (BATTLE_CURSOR_ACTION_TARGET_VALID_MASK selects VALID, the other target actions EXTRA). */
enum BattleTargetMaskMode {
    BTL_TARGET_MASK_VALID = 0,   /* g_BattleScene.targetValidMask */
    BTL_TARGET_MASK_EXTRA = 1    /* g_BattleScene.targetExtraMask */
};

/* The frameCounter (or overlay-task state) of a two-step battle task: the first frame sets the
 * task up and advances it to UPDATE, which then runs every frame until the task frees its slot. */
enum BattleTaskFramePhase {
    BATTLE_TASK_FRAME_FIRST = 0,
    BATTLE_TASK_FRAME_UPDATE = 1
};

/* Battle_EffectTaskScreenColourRamp's frameCounter. */
enum BattleScreenRampPhase {
    BATTLE_SCREEN_RAMP_START = 0,   /* sets the ramp up and draws its first step */
    BATTLE_SCREEN_RAMP_FADE = 1,    /* steps the level until the countdown ends, then sets it to 0xFF */
    BATTLE_SCREEN_RAMP_HOLD = 2     /* keeps drawing the full level */
};

/* Bits of g_BattleActorRecords[].stateFlags. */
enum BattleActorRecordStateFlag {
    BATTLE_ACTOR_RECORD_FADE_OUT_HIDE = 0x02,   /* BATTLE_MOTION_FADE_OUT_AND_HIDE */
    BATTLE_ACTOR_RECORD_FADE_IN = 0x04,         /* BATTLE_MOTION_FADE_IN */
    BATTLE_ACTOR_RECORD_MODEL_REFRESH = 0x20    /* BATTLE_MOTION_SET_TRANSFORM_MODEL: Battle3d_UpdateActorRenderState
                                                 * redoes the frog model, the mini scale and the status tint */
};

/* Bits of g_BattleActorRecords[].drawFlags. */
enum BattleActorRecordDrawFlag {
    BATTLE_ACTOR_DRAW_STATUS_TINT = 0x08        /* a status tint is selected; its phase pulses */
};

/* BattleFxBzMorphDesc::flags: how BattleFx_DrawMorphedLmdOuter/Inner emit a morphed LMD's faces. The
 * effect PSX-mesh draw requests (BattleFx_MagicQuake3DrawPsxMesh and its copies in the summon TUs)
 * test the same bits of their flags word the same way. */
enum BattleFxBzMorphFlag {
    BATTLE_FX_BZ_MORPH_SEMI_TRANSPARENT = 0x08, /* every packet gets the GPU semi-transparency bit */
    BATTLE_FX_BZ_MORPH_DOUBLE_SIDED = 0x20,     /* textured faces skip the front-facing test */
    BATTLE_FX_BZ_MORPH_FACE_GREY = 0x80         /* a textured face's colour is its own grey byte, shaded by desc->shade */
};

/* g_BattleSceneLoadStep: the step Battle_LoadSceneRecord runs next, once per frame. */
enum BattleSceneLoadStep {
    BATTLE_SCENE_LOAD_READ_CHUNK = 0,    /* reads the scene.bin chunk that holds the scene */
    BATTLE_SCENE_LOAD_WAIT_CALLBACK = 1, /* runs the caller's callback until g_BattleSequencePlayDone */
    BATTLE_SCENE_LOAD_INFLATE = 2        /* inflates the scene and copies the formation out */
};


/* BattleState.atbMode (from Config ATB row: Active, Recommended, Wait). */
enum BattleAtbMode {
    BATTLE_ATB_ACTIVE = 0,
    BATTLE_ATB_RECOMMENDED = 1,
    BATTLE_ATB_WAIT = 2
};

/* Enemy-skill effect indices handled in Battle_EffectAttachStatusOverlayModel. */
enum BattleBlueMagicEffect {
    BATTLE_BLUE_EFFECT_ANGEL_WHISPER = 5,
    BATTLE_BLUE_EFFECT_CHOCOBUCKLE = 18,
    BATTLE_BLUE_EFFECT_DEATH_SENTENCE = 20,
    BATTLE_BLUE_EFFECT_ROULETTE = 21
};

/* Special-attack effect indices handled in Battle_EffectAttachStatusOverlayModel. */
enum BattleSpecialEffect {
    BATTLE_SPECIAL_EFFECT_JOKER_DEATH = 50,
    BATTLE_SPECIAL_EFFECT_ANGEL_REVIVE = 144
};

/* General-action effect indices handled in Battle_EffectAttachStatusOverlayModel. */
enum BattleGeneralEffect {
    BATTLE_GENERAL_EFFECT_DEATH = 24,
    BATTLE_GENERAL_EFFECT_CONFUSE = 29
};

/* Built-in actor motion-script IDs redirected to static tables in Battle_RunActorAnimScript. */
enum BattleBuiltinAnimScriptId {
    BATTLE_BUILTIN_SCRIPT_2E = 0x2E,
    BATTLE_BUILTIN_SCRIPT_2F = 0x2F,
    BATTLE_BUILTIN_SCRIPT_30 = 0x30,
    BATTLE_BUILTIN_SCRIPT_31 = 0x31,
    BATTLE_BUILTIN_SCRIPT_32 = 0x32,
    BATTLE_BUILTIN_SCRIPT_34 = 0x34,
    BATTLE_BUILTIN_SCRIPT_35 = 0x35,
    BATTLE_BUILTIN_SCRIPT_36 = 0x36,
    BATTLE_BUILTIN_SCRIPT_37 = 0x37,
    BATTLE_BUILTIN_SCRIPT_38 = 0x38,
    BATTLE_BUILTIN_SCRIPT_39 = 0x39,
    BATTLE_BUILTIN_SCRIPT_3A = 0x3A,
    BATTLE_BUILTIN_SCRIPT_3B = 0x3B,
    BATTLE_BUILTIN_SCRIPT_33 = 0x33
};

/* Status and victory pose codes dispatched by Battle_SpawnStatusPoseTask and Battle_SpawnPoseTaskFromHud. */
enum BattlePoseKind {
    BATTLE_POSE_NORMAL = 0,
    BATTLE_POSE_DEATH_SINK = 1,
    BATTLE_POSE_HURT = 2,
    BATTLE_POSE_FADE_OUT = 3,
    BATTLE_POSE_VICTORY = 7,
    BATTLE_POSE_KNEEL = 8,
    BATTLE_POSE_DEATH_SINK_ALT = 0xA,
    BATTLE_POSE_CRITICAL_A = 0x11,
    BATTLE_POSE_CRITICAL_B = 0x12
};


enum BattleSceneActorStatusFlag {
    BATTLE_SCENE_ACTOR_KO = 0x01
};

enum BattleMotionHopKind {
    BATTLE_HOP_KIND_DEFAULT = 0,
    BATTLE_HOP_KIND_ARC_1 = 1,
    BATTLE_HOP_KIND_ARC_2 = 2,
    BATTLE_HOP_KIND_ARC_3 = 3,
    BATTLE_HOP_KIND_ARC_4 = 4,
    BATTLE_HOP_KIND_ARC_5 = 5,
    BATTLE_HOP_KIND_ARC_6 = 6,
    BATTLE_HOP_KIND_ARC_7 = 7
};

enum BattleMotionEffectProgramOp {
    BATTLE_MOTION_EFFPROG_BEAT_RUSH = 0,
    BATTLE_MOTION_EFFPROG_SOMERSAULT = 1,
    BATTLE_MOTION_EFFPROG_WATERKICK = 2,
    BATTLE_MOTION_EFFPROG_METEODRIVE = 3,
    BATTLE_MOTION_EFFPROG_DOLPHIN_BLOW = 4,
    BATTLE_MOTION_EFFPROG_METEOR_STRIKE = 5,
    BATTLE_MOTION_EFFPROG_FINAL_HEAVEN = 6
};

enum BattleEffectProgramId {
    BATTLE_EFFECT_PROGRAM_NONE = 0,
    BATTLE_EFFECT_PROGRAM_BEAT_RUSH = 1,
    BATTLE_EFFECT_PROGRAM_SOMERSAULT = 2,
    BATTLE_EFFECT_PROGRAM_WATERKICK = 3,
    BATTLE_EFFECT_PROGRAM_METEODRIVE = 4,
    BATTLE_EFFECT_PROGRAM_DOLPHIN_BLOW = 5,
    BATTLE_EFFECT_PROGRAM_METEOR_STRIKE = 6,
    BATTLE_EFFECT_PROGRAM_FINAL_HEAVEN = 7
};

enum BattleWeaponModelSlotCode {
    BATTLE_WEAPON_MODEL_PRIMARY = 0x00,
    BATTLE_WEAPON_MODEL_ALT_10 = 0x10,
    BATTLE_WEAPON_MODEL_ALT_20 = 0x20,
    BATTLE_WEAPON_MODEL_ALT_30 = 0x30
};

enum BattleStatusTintMode {
    BATTLE_TINT_MODE_NONE = 0,
    BATTLE_TINT_MODE_PULSE = 1,
    BATTLE_TINT_MODE_STATIC = 2
};

enum BattleDamageLabel {
    BATTLE_DAMAGE_LABEL_MISS = -1,
    BATTLE_DAMAGE_LABEL_DEATH = -2,
    BATTLE_DAMAGE_LABEL_FULL_RECOVERY = -3
};

enum BattleRainDropPhase {
    BATTLE_RAIN_FALLING_STREAK = 0,
    BATTLE_RAIN_SPLASH_END = 8
};

enum BattlePartyPlaceMode {
    BATTLE_PARTY_PLACE_CENTERED_X = 1,
    BATTLE_PARTY_PLACE_ENEMY_SIDE = 2
};

enum BattleLimitTransformId {
    BATTLE_LIMIT_TRANSFORM_GALIAN_GALIAN_BEAST = 0,
    BATTLE_LIMIT_TRANSFORM_DEATH_GIGAS = 1,
    BATTLE_LIMIT_TRANSFORM_HELLMASKER = 2,
    BATTLE_LIMIT_TRANSFORM_CHAOS = 3
};

enum BattleToyBoxVariant {
    BATTLE_TOYBOX_BOULDER = 0,
    BATTLE_TOYBOX_ICICLE = 1,
    BATTLE_TOYBOX_WEIGHT = 2,
    BATTLE_TOYBOX_FAT_CHOCOBO = 3
};

enum BattleHasteOrbitTiltPreset {
    BATTLE_HASTE_ORBIT_TILT_1 = 1,
    BATTLE_HASTE_ORBIT_TILT_2 = 2,
    BATTLE_HASTE_ORBIT_TILT_3 = 3,
    BATTLE_HASTE_ORBIT_TILT_4 = 4
};

enum BattleKotrKnightLookMode {
    BATTLE_KOTR_KNIGHT_LOOK_DELTA = 1,
    BATTLE_KOTR_KNIGHT_LOOK_NEG_OFFSET = 2
};

enum BattleKotrStackedLayerVariant {
    BATTLE_KOTR_STACKED_VARIANT_0 = 0,
    BATTLE_KOTR_STACKED_VARIANT_1 = 1
};

enum BattleCidThrowFlightKind {
    BATTLE_CID_THROW_STRAIGHT = 0,
    BATTLE_CID_THROW_HIGH_JUMP = 1,
    BATTLE_CID_THROW_SPINNING = 2
};

enum BattleVincentThrowFlightKind {
    BATTLE_VINCENT_THROW_STRAIGHT = 0
};

enum BattleElementalArmorLevel {
    BATTLE_ELEMENTAL_ARMOR_HALVE = 1,
    BATTLE_ELEMENTAL_ARMOR_NULLIFY = 2,
    BATTLE_ELEMENTAL_ARMOR_ABSORB = 3
};

enum Battle3dLmdLoadKind {
    BATTLE3D_LMD_LOAD_UNCOLORED = 0,
    BATTLE3D_LMD_LOAD_VERTEX_COLORED = 1
};

enum Battle3dLmdFacePass {
    BATTLE3D_LMD_PASS_COLLECT_ATLAS = 0,
    BATTLE3D_LMD_PASS_BUILD_PART = 1,
    BATTLE3D_LMD_PASS_BUILD_DIRECT = 2
};

enum KernelZeroMpMenuAbility {
    KERNEL_MENU_ABILITY_ESCAPE = 0x19,
    KERNEL_MENU_ABILITY_SUMMON_0F = 0x47
};

/* Combatant slot bitmasks across party (0..2), allies (0..3), enemies (4..9), and all 10 slots (0..9). */
enum BattleActorMask {
    BATTLE_ACTOR_MASK_PARTY = 0x007,
    BATTLE_ACTOR_MASK_ALLIES = 0x00F,
    BATTLE_ACTOR_MASK_ENEMIES = 0x3F0,
    BATTLE_ACTOR_MASK_ALL = 0x3FF
};

/* Row group index in g_BattleScene.targetMaskTable[0..2] (Battle_ResolveAttackFacing). */
enum BattleTargetRowGroup {
    BATTLE_TARGET_GROUP_SIDE_A = 0,
    BATTLE_TARGET_GROUP_CENTER = 1,
    BATTLE_TARGET_GROUP_SIDE_B = 2
};

enum BattleMagicFire3PillarTick {
    BATTLE_FIRE3_TICK_SHRINK_0 = 0,
    BATTLE_FIRE3_TICK_SHRINK_1 = 1,
    BATTLE_FIRE3_TICK_FLASH_START_1 = 2,
    BATTLE_FIRE3_TICK_FLASH_FADE_1 = 3,
    BATTLE_FIRE3_TICK_RESET_LEVEL = 4,
    BATTLE_FIRE3_TICK_SHRINK_2 = 5,
    BATTLE_FIRE3_TICK_FLASH_START_2 = 6,
    BATTLE_FIRE3_TICK_FLASH_FADE_2 = 7,
    BATTLE_FIRE3_TICK_SHRINK_3 = 8,
    BATTLE_FIRE3_TICK_SHRINK_4 = 9,
    BATTLE_FIRE3_TICK_SHRINK_5 = 0xA,
    BATTLE_FIRE3_TICK_SHRINK_6 = 0xB
};

#endif

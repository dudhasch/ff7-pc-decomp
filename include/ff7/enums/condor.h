/* include/ff7/enums/condor.h: named codes of ff7/condor (Fort Condor) and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_CONDOR_H
#define FF7_ENUMS_CONDOR_H

/* The rows of a unit's action menu (g_CondorActionRowA/B, drawn as icon row action * 0x10), which
 * Mini_CondorSetUnitMenuLayout fills by the unit's kind and Mini_CondorDispatchRecordAction runs. */
enum CondorUnitAction {
    CONDOR_ACTION_SPECIAL_SHOT = 0, /* kind 6: record state 0xA fires its one shot; refused once candidate has 0x80 */
    CONDOR_ACTION_CANCEL_1 = 1,     /* closes the action menu without acting */
    CONDOR_ACTION_RETIRE = 2,       /* Mini_CondorRetireSelectedRecord */
    CONDOR_ACTION_MOVE = 3,         /* enters CONDOR_RECORD_MODE_TARGET with CONDOR_COMMAND_MOVE */
    CONDOR_ACTION_CANCEL_4 = 4,     /* closes the action menu without acting */
    CONDOR_ACTION_TURN = 5,         /* kinds 3 and 4: the angle wheel (CONDOR_STATE_RECORD_ANGLE_COMMIT) */
    CONDOR_ACTION_NONE = 7          /* the row of a unit with no actions */
};

/* g_CondorCommandCode and CondorRecord::commandCode: what the target cursor picks for the unit, which
 * Mini_CondorApplyRecordCommand turns into the record's state (drawn as icon row code * 0x10). */
enum CondorCommandCode {
    CONDOR_COMMAND_ATTACK = 0,      /* the cursor is on an enemy unit (unitIndex above 0xF) */
    CONDOR_COMMAND_ATTACK_ALT = 1,  /* applied as CONDOR_COMMAND_ATTACK; the cursor never picks it */
    CONDOR_COMMAND_TARGET_ALLY = 2, /* a unit with unitIndex 0xC picked a unit with unitIndex up to 0xF */
    CONDOR_COMMAND_MOVE = 3,        /* to the cursor's point */
    CONDOR_COMMAND_HOLD = 4,        /* the cursor is on the unit itself: it stops */
    CONDOR_COMMAND_MOVE_STATE_6 = 5, /* begins target move in state 6 (flags 0x09) */
    CONDOR_COMMAND_MOVE_STATE_7 = 6  /* begins target move in state 7 (flags 0x11) */
};

/* The battle state g_CondorMainState, named by the handler Mini_CondorDispatchStateHandler runs
 * for it; 0 runs none and lets the board take input. */
enum CondorMainState {
    CONDOR_STATE_IDLE = 0x00,
    CONDOR_STATE_CLEAR = 0x01,                  /* Mini_CondorClearStateWord: back to IDLE */
    CONDOR_STATE_CURSOR_GLIDE_TO_ACTION = 0x02, /* Mini_CondorStepAndDispatchRequest: glide, then act */
    CONDOR_STATE_CURSOR_GLIDE = 0x03,           /* Mini_CondorStepUntilIdle: glide, then pick near the cursor */
    CONDOR_STATE_INTRO = 0x04,                  /* Mini_CondorRunPhasedMotion: fade in, banner, scroll down */
    CONDOR_STATE_UNIT_ATTACK = 0x05,            /* Mini_CondorBeginUnitAttack */
    CONDOR_STATE_UNIT_COUNTERATTACK = 0x06,     /* Mini_CondorTickUnitAttack: the target strikes back */
    CONDOR_STATE_RECORD_SELECT_WHEEL = 0x07,    /* Mini_CondorTickRecordSelectWheel */
    CONDOR_STATE_RECORD_ANGLE_INPUT = 0x08,     /* Mini_CondorTickRecordAngleInput */
    CONDOR_STATE_START_MENU = 0x09,             /* Mini_CondorProcessInputFlags, entered on Start */
    CONDOR_STATE_CONFIRM_ROWS = 0x0A,           /* Mini_CondorHandleRecordSelectInput: two-row choice */
    CONDOR_STATE_END_BANNER_EXIT_FLAGGED = 0x0B, /* Mini_CondorTickCounterOrRequestState4 */
    CONDOR_STATE_END_BANNER_EXIT = 0x0C,        /* Mini_CondorTickCounterOrRequestState5 */
    CONDOR_STATE_CURSOR_RAMP = 0x0D,            /* Mini_CondorRampStepMotion */
    CONDOR_STATE_HELP_SCREEN = 0x0E,            /* Mini_CondorBeginRecordSelect: entered on Select; shows
                                                 * the help TIM (ehelp.tim) until Select is latched again */
    CONDOR_STATE_RECORD_PANEL = 0x0F,           /* Mini_CondorMainState0FNoOp: the record panels run */
    CONDOR_STATE_RECORD_ANGLE_COMMIT = 0x10,    /* Mini_CondorTickRecordAngleCommitOrRestore */
    CONDOR_STATE_BANNER_GROW = 0x11             /* Mini_CondorAdvanceCounterToTarget */
};

/* The exit word g_CondorStage, which Mini_CondorUpdateFrameInput returns as the frame's phase and
 * Mini_CondorDispatchPhase runs: 1..3 restart the battle, 4 and 5 leave Fort Condor. */
enum CondorStage {
    CONDOR_STAGE_RUN = 0,
    CONDOR_STAGE_RESTART = 1,
    CONDOR_STAGE_SWITCH_MODE = 2,     /* toggles g_CondorPhase between 0 and 1, then restarts */
    CONDOR_STAGE_RESTART_RECORDS = 3, /* builds the active record list, then restarts in phase 2 */
    CONDOR_STAGE_EXIT_FLAGGED = 4,    /* leaves and sets the savemap flag at +0xC50 */
    CONDOR_STAGE_EXIT = 5
};

/* g_CondorRecordMode: what the board's cursor does. Mini_CondorResetGameState starts in CURSOR;
 * Mini_CondorOpenSelectedRecordActionMenu opens a unit's ACTION_MENU, whose action 3 enters
 * TARGET (a second cursor picks a point for the unit). The battle clock runs only in NONE and CURSOR. */
enum CondorRecordMode {
    CONDOR_RECORD_MODE_NONE = 0,
    CONDOR_RECORD_MODE_CURSOR = 1,
    CONDOR_RECORD_MODE_ACTION_MENU = 2,
    CONDOR_RECORD_MODE_TARGET = 3
};

/* g_CondorSelectPanelPhase: 0 when no panel is open. Mini_CondorUpdateRecordSelectPanel slides
 * the record panels in (OPEN), browses them (BROWSE) and slides them back (CLOSING); other
 * states set OPEN while they hold the board. */
enum CondorPanelPhase {
    CONDOR_PANEL_CLOSED = 0,
    CONDOR_PANEL_OPEN = 1,
    CONDOR_PANEL_BROWSE = 2,
    CONDOR_PANEL_CLOSING = 3
};

/* A unit record's attack step (CondorRecord::inContact), which Mini_CondorAdvanceUnitAttack
 * advances once per frame. */
enum CondorAttackStep {
    CONDOR_ATTACK_STEP_NONE = 0,
    CONDOR_ATTACK_STEP_BEGIN = 1,        /* starts the attack sequence, spawns the projectile */
    CONDOR_ATTACK_STEP_SWING = 2,        /* waits for the attack sequence to end */
    CONDOR_ATTACK_STEP_HIT = 3,          /* shows the damage number, starts the target's reaction */
    CONDOR_ATTACK_STEP_TARGET_REACT = 4, /* waits for the target's reaction sequence */
    CONDOR_ATTACK_STEP_RESOLVE = 5       /* subtracts the damage */
};

/* How a shot record moves (CondorRecord::operand of the records Mini_CondorSpawnMovingRecord
 * spawns), as Mini_CondorUpdateUnitByType moves it. Each motion has two codes; the second of a
 * pair moves as the first. */
enum CondorShotKind {
    CONDOR_SHOT_SPIN = 0,     /* spins, hits along its path, ends on its timer (unit kinds 5, 6) */
    CONDOR_SHOT_SPIN_ALT = 1, /* the same; a sub-shot (kind 0x18) spawned as 1 starts 0x10 right, not left */
    CONDOR_SHOT_ARC = 2,      /* rises and falls, spawns an effect where it lands (unit kind 7) */
    CONDOR_SHOT_ARC_ALT = 3,  /* the same (unit kind 8) */
    CONDOR_SHOT_FAST = 4,     /* moves three steps a tick and hits on contact (unit kinds 4, 0x13) */
    CONDOR_SHOT_FAST_ALT = 5  /* the same */
};

/* The board's fade level: g_CondorRequestedDisplayMode (0 when no change is pending, drawn as
 * FULL) and g_CondorDisplayMode, by the fade alpha Mini_CondorDispatchFadeRamp ramps to. */
enum CondorDisplayMode {
    CONDOR_DISPLAY_NONE = 0,
    CONDOR_DISPLAY_FULL = 1,   /* fade alpha 0x80 */
    CONDOR_DISPLAY_DIMMED = 2, /* fade alpha 0x50, behind the record panels */
    CONDOR_DISPLAY_DARK = 3    /* fade alpha 0x30 */
};

/* Mini_CondorAdvanceUnitAttack's result. */
enum CondorAttackResult {
    CONDOR_ATTACK_RUNNING = 0,
    CONDOR_ATTACK_DONE = 1,
    CONDOR_ATTACK_TARGET_DEFEATED = 2
};

/* Direction bits of a blocked unit (Mini_CondorResolveDirectionalBlock): the way it moves along
 * each axis, and the side Mini_CondorTestPeerBlockDirections finds free of a peer (each
 * Mini_CondorTestAdjacentPeer* returns its bit when that side is free, 0 when a peer touches it). */
enum CondorBlockDir {
    CONDOR_BLOCK_DIR_NEG_Y = 0x1,
    CONDOR_BLOCK_DIR_POS_Y = 0x2,
    CONDOR_BLOCK_DIR_NEG_X = 0x4,
    CONDOR_BLOCK_DIR_POS_X = 0x8
};

/* Mini_CondorFindFirstOpenRegionEdge's result: the first of a region's three edges whose neighbour
 * is -1 (edge0 runs from corner 0 to 1, edge1 from 1 to 2, edge2 from 2 to 0). */
enum CondorRegionOpenEdge {
    CONDOR_REGION_OPEN_EDGE_NONE = 0,
    CONDOR_REGION_OPEN_EDGE_0 = 1,
    CONDOR_REGION_OPEN_EDGE_1 = 2,
    CONDOR_REGION_OPEN_EDGE_2 = 3
};

/* CondorRecord::targetMode: how Mini_CondorResolveTargetByMode picks a unit's target. */
enum CondorTargetMode {
    CONDOR_TARGET_MODE_NONE = 0,         /* no target */
    CONDOR_TARGET_MODE_PROBE = 1,        /* the collision probe's hit */
    CONDOR_TARGET_MODE_PRIORITY = 2,     /* when the probe hits, the highest-priority unit it found */
    CONDOR_TARGET_MODE_PREFER_KIND = 3,  /* as PRIORITY, among the units of the kind its unitIndex hunts first */
    CONDOR_TARGET_MODE_PAIR = 4          /* pairs with a peer; at the last spawn drops to NONE and walks on */
};


/* CondorRecord::state values assigned when unit actions and target commands are committed. */
enum CondorUnitState {
    CONDOR_UNIT_STATE_IDLE = 0,
    CONDOR_UNIT_STATE_MOVING = 1,
    CONDOR_UNIT_STATE_ATTACK_MOVE = 3,
    CONDOR_UNIT_STATE_SUPPORT_MOVE = 5,
    CONDOR_UNIT_STATE_PATROL_6 = 6,
    CONDOR_UNIT_STATE_PATROL_7 = 7,
    CONDOR_UNIT_STATE_SPECIAL_SHOT = 0xA,
    CONDOR_UNIT_STATE_ENGAGED = 2,
    CONDOR_UNIT_STATE_WAITING_PEER = 4
};

/* 12-bit angle octant/quadrant masks in Mini_CondorAngleToStepComponents and Mini_CondorAngleMagnitudeToOffset. */
enum CondorAngleOctant {
    CONDOR_OCTANT_N_NW = 0x000,
    CONDOR_OCTANT_NW_W = 0x200,
    CONDOR_OCTANT_W_SW = 0x400,
    CONDOR_OCTANT_SW_S = 0x600,
    CONDOR_OCTANT_S_SE = 0x800,
    CONDOR_OCTANT_SE_E = 0xA00,
    CONDOR_OCTANT_E_NE = 0xC00,
    CONDOR_OCTANT_NE_N = 0xE00
};

/* CondorRecord::unitIndex unit/catapult classes switched on by sound and projectile spawners. */
enum CondorUnitKind {
    CONDOR_UNIT_SHOOTER = 4,
    CONDOR_UNIT_STONER = 5,
    CONDOR_UNIT_TRISTONER = 6,
    CONDOR_UNIT_CATAPULT = 7,
    CONDOR_UNIT_FIRE_CATAPULT = 8,
    CONDOR_UNIT_REPAIRER = 0xC,
    CONDOR_UNIT_WORKER = 0xD,
    CONDOR_UNIT_BOSS_WYVERN = 0x11,
    CONDOR_UNIT_ENEMY_BEAST = 0x12,
    CONDOR_UNIT_ENEMY_SHOOTER = 0x13,
    CONDOR_UNIT_SUB_SHOT = 0x18
};


enum CondorBehaviorKind {
    CONDOR_BEHAVIOR_STATIONARY = 0,
    CONDOR_BEHAVIOR_FIGHTER = 1,
    CONDOR_BEHAVIOR_SUPPORT = 2,
    CONDOR_BEHAVIOR_TURRET_A = 3,
    CONDOR_BEHAVIOR_TURRET_B = 4,
    CONDOR_BEHAVIOR_CATAPULT = 5,
    CONDOR_BEHAVIOR_ONESHOT = 6,
    CONDOR_BEHAVIOR_AUTONOMOUS = 7
};

enum CondorMusicSelector {
    CONDOR_MUSIC_SELECT_BATTLE = 0x0A,
    CONDOR_MUSIC_SELECT_FANFARE = 0x0B,
    CONDOR_MUSIC_SELECT_REQUIEM = 0x0C,
    CONDOR_MUSIC_SELECT_THEME = 0x14
};

enum CondorQuadrantSign {
    CONDOR_QUADRANT_POS_X_POS_Y = 0,
    CONDOR_QUADRANT_POS_X_NEG_Y = 1,
    CONDOR_QUADRANT_NEG_X_POS_Y = 2,
    CONDOR_QUADRANT_NEG_X_NEG_Y = 3
};

enum CondorChoiceRingLayout {
    CONDOR_RING_LAYOUT_TWO_WAY = 0,
    CONDOR_RING_LAYOUT_THREE_WAY = 1,
    CONDOR_RING_LAYOUT_SINGLE = 2
};

enum CondorWheelPhase {
    CONDOR_WHEEL_PHASE_SPIN = 2,
    CONDOR_WHEEL_PHASE_SETTLE = 3,
    CONDOR_WHEEL_PHASE_COMMIT = 4
};

enum CondorHudBannerSpriteIndex {
    CONDOR_HUD_BANNER_SPRITE_0 = 16,
    CONDOR_HUD_BANNER_SPRITE_1 = 17,
    CONDOR_HUD_BANNER_SPRITE_2 = 18
};

#endif

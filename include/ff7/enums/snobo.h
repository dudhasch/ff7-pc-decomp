/* include/ff7/enums/snobo.h: named codes of ff7/snobo (the Gold Saucer snowboard minigame).
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_SNOBO_H
#define FF7_ENUMS_SNOBO_H

/* SnowboardTmdRenderer::model's mode: which primitive routine draws the model's triangle records. */
enum SnowboardModelDrawMode {
    SNOWBOARD_MODEL_DRAW_FLAT = 0,      /* SnowboardTmdRenderer::triangle: vertex colors as stored */
    SNOWBOARD_MODEL_DRAW_SHADED = 1,    /* SnowboardTmdRenderer::shaded: colors scaled by the light level */
    SNOWBOARD_MODEL_DRAW_TEXTURED = 2,  /* SnowboardTmdRenderer::textured */
    SNOWBOARD_MODEL_DRAW_COLORED = 3    /* SnowboardTmdRenderer::colored: into the campaign's own batch list */
};

/* g_SnoboModeFlag: which course Mini_SnowboardInit loads. Only the Gold Saucer game has the pause
 * menu and records runs; in the story run the pause button only pauses. */
enum SnowboardMode {
    SNOWBOARD_MODE_GOLD_SAUCER = 0,     /* for_gs.tmd */
    SNOWBOARD_MODE_EVENT = 1            /* for_ev.tmd */
};

/* g_SnoboPauseMenuState, the page Mini_SnowboardRacePauseMenu shows and the renderer's
 * select0..select4 draws. */
enum SnowboardPauseMenuState {
    SNOWBOARD_PAUSE_MENU_MAIN = 0,               /* resume, retry, quit */
    SNOWBOARD_PAUSE_MENU_MAIN_WITH_OPTIONS = 1,  /* options, then the MAIN rows one lower */
    SNOWBOARD_PAUSE_MENU_OPTIONS = 2,            /* the two settings below */
    SNOWBOARD_PAUSE_MENU_REPLAY_STYLE = 3,       /* g_SnoboCourseRecords.animFrameVariantWord 0-3 (0 = no replay) */
    SNOWBOARD_PAUSE_MENU_ON_OFF = 4              /* g_SnoboActiveFlagMask 0 or 1 */
};

/* g_SnoboPauseMenuCursor on the SNOWBOARD_PAUSE_MENU_MAIN page. */
enum SnowboardPauseMainRow {
    SNOWBOARD_PAUSE_ROW_RESUME = 0,
    SNOWBOARD_PAUSE_ROW_RETRY = 1,               /* fade out, then restart the run */
    SNOWBOARD_PAUSE_ROW_QUIT = 2,
    SNOWBOARD_PAUSE_ROW_OPTIONS = 0              /* on SNOWBOARD_PAUSE_MENU_MAIN_WITH_OPTIONS only */
};

/* g_SnoboPauseMenuCursor on the SNOWBOARD_PAUSE_MENU_OPTIONS page: the page each row opens. */
enum SnowboardPauseOptionsRow {
    SNOWBOARD_PAUSE_OPTIONS_ROW_REPLAY_STYLE = 0,
    SNOWBOARD_PAUSE_OPTIONS_ROW_ON_OFF = 1
};

/* The rows of the run's result screen (SnowboardRenderer::showMenu), each with its label in
 * g_SnoboMenuTexts1 and its unit in g_SnoboMenuTexts2. */
enum SnowboardResultRow {
    SNOWBOARD_RESULT_ROW_TECHNIQUE = 0,          /* resultPartA */
    SNOWBOARD_RESULT_ROW_TIME = 1,               /* resultPartB */
    SNOWBOARD_RESULT_ROW_BALLOON = 2,            /* resultPartC */
    SNOWBOARD_RESULT_ROW_TOTAL = 3,              /* resultTotal, drawn pulsing */
    SNOWBOARD_RESULT_ROW_BEST = 4                /* the course's best score */
};

/* The rider's action (low byte of SnowboardRaceState::riderState), the index into g_SnoboActionTable;
 * named only where the table's handler tells the action apart. */
enum SnowboardRiderAction {
    SNOWBOARD_RIDER_ACTION_CONTROLS = 0x01,      /* Mini_SnowboardUpdateRiderControls */
    SNOWBOARD_RIDER_ACTION_STEER_PUSH = 0x02,    /* Mini_SnowboardActionSteerWithPush */
    SNOWBOARD_RIDER_ACTION_IMPACT_TURN = 0x08,   /* Mini_SnowboardActionImpactTurn (8 or 9) */
    SNOWBOARD_RIDER_ACTION_TURN_LEFT = 0x11,     /* Mini_SnowboardActionTurnLeft */
    SNOWBOARD_RIDER_ACTION_TURN_RIGHT = 0x12,    /* Mini_SnowboardActionTurnRight */
    SNOWBOARD_RIDER_ACTION_STOP_13 = 0x13,       /* Mini_SnowboardActionResetVelocity */
    SNOWBOARD_RIDER_ACTION_STOP_14 = 0x14,       /* Mini_SnowboardActionResetVelocity */
    SNOWBOARD_RIDER_ACTION_STOP_15 = 0x15,       /* Mini_SnowboardActionResetVelocity */
    SNOWBOARD_RIDER_ACTION_BRAKE = 0x16,         /* Mini_SnowboardActionBrake */
    SNOWBOARD_RIDER_ACTION_STOP_19 = 0x19,       /* Mini_SnowboardActionResetVelocity */
    SNOWBOARD_RIDER_ACTION_TURN_TO_TARGET = 0x1C, /* Mini_SnowboardActionTurnToTargetAngle */
    SNOWBOARD_RIDER_ACTION_HALVE_VEL_0A = 0x0A,
    SNOWBOARD_RIDER_ACTION_HALVE_VEL_0C = 0x0C,
    SNOWBOARD_RIDER_ACTION_HALVE_VEL_0E = 0x0E
};

/* The two codes of g_SnoboActionTransitions that are not a rider action
 * (Mini_SnowboardSelectActionFromTable). */
enum SnowboardActionTransition {
    SNOWBOARD_TRANSITION_DAMP = 0xFE,            /* keeps the action, plays sound 0x135, loses a quarter of the velocity */
    SNOWBOARD_TRANSITION_NONE = 0xFF             /* keeps the action */
};

/* SnowboardEffect::mode of a split-ballistic effect (g_SnoboSplitBallisticEffectDef, from its
 * descriptor's param): what Mini_SnowboardEffectOnHitSplitBallisticFadeOut spawns after the split;
 * any other value spawns nothing. */
enum SnowboardSplitSpawn {
    SNOWBOARD_SPLIT_SPAWN_LINKED_BLINK = 1,      /* Mini_SnowboardEffectSpawnLinkedBlinkGroup */
    SNOWBOARD_SPLIT_SPAWN_CHILD_COPY = 2,        /* Mini_SnowboardEffectSpawnChildCopyMotion */
    SNOWBOARD_SPLIT_SPAWN_HIT_LAUNCH_NEG = 3,    /* a landing hit-launch child at homeX -0x20 */
    SNOWBOARD_SPLIT_SPAWN_HIT_LAUNCH_POS = 4     /* the same at homeX +0x20 */
};


enum SnowboardSavemapCourseCode {
    SNOWBOARD_SAVEMAP_COURSE_B = 2,
    SNOWBOARD_SAVEMAP_TIME_ATTACK = 8
};

enum SnowboardReplayStyle {
    SNOWBOARD_REPLAY_OFF = 0,
    SNOWBOARD_REPLAY_COURSE_A = 1,
    SNOWBOARD_REPLAY_COURSE_B = 2,
    SNOWBOARD_REPLAY_COURSE_C = 3
};

enum SnowboardToggleOption {
    SNOWBOARD_TOGGLE_OFF = 0,
    SNOWBOARD_TOGGLE_ON = 1
};

enum SnowboardHeadingQuadrant {
    SNOWBOARD_HEADING_QUAD_1 = 1,
    SNOWBOARD_HEADING_QUAD_2 = 2,
    SNOWBOARD_HEADING_QUAD_3 = 3,
    SNOWBOARD_HEADING_QUAD_4 = 4
};

enum SnowboardFollowCamMode {
    SNOWBOARD_FOLLOW_CAM_STANDARD = 1,
    SNOWBOARD_FOLLOW_CAM_CLOSE = 2
};

enum SnowboardPickupKind {
    SNOWBOARD_PICKUP_BALLOON_TRIPLE = 3,
    SNOWBOARD_PICKUP_BALLOON_FIVE = 4
};
#endif

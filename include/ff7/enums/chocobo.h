/* include/ff7/enums/chocobo.h: named codes of ff7/chocobo and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_CHOCOBO_H
#define FF7_ENUMS_CHOCOBO_H

/* Kinds of chocobo course record (byte 2 of a record word), which
 * Mini_ChocoboRaceSubmitCourseSegmentRecords submits; -1 ends a record list. */
enum ChocoboCourseRecordKind {
    CHOCOBO_COURSE_RECORD_END = -1,
    CHOCOBO_COURSE_RECORD_TRIANGLE = 0,
    CHOCOBO_COURSE_RECORD_QUAD = 1,
    CHOCOBO_COURSE_RECORD_SPRITE = 0x21
};

/* The racing chocobo's colour (savemap +0xDB5, gSavemap.raceChocoboColor): the palette index of
 * Mini_ChocoboRaceInitRacerModels; the game shows 0, 1, 2 and 4, black = 3 by FF7's order. */
enum ChocoboRaceColor {
    CHOCOBO_COLOR_YELLOW = 0,
    CHOCOBO_COLOR_GREEN = 1,
    CHOCOBO_COLOR_BLUE = 2,
    CHOCOBO_COLOR_BLACK = 3,
    CHOCOBO_COLOR_GOLD = 4
};

/* ChocoboRacer::personality (savemap +0xDB6 for the player's racer). */
enum ChocoboPersonality {
    CHOCOBO_PERSONALITY_NONE = 0,
    CHOCOBO_PERSONALITY_SLOWS_WHEN_BEHIND = 1, /* eases off when not in the top two */
    CHOCOBO_PERSONALITY_DASHES_WHEN_SPURRED = 2
};

/* Course waypoint flags AND-ed with ChocoboRacer::terrainPenaltyMask: the step speed loses
 * a quarter (SLOW_1) or a half (SLOW_2). The screendumps do not show the terrain, so the
 * names give the value only. */
enum ChocoboTrackFlag {
    CHOCOBO_TRACK_FLAG_SLOW_1 = 0x01,
    CHOCOBO_TRACK_FLAG_SLOW_2 = 0x02
};

/* ChocoboRaceState::eventKind of g_ChocoboCurrentRouteEvent: the race camera view
 * (Mini_ChocoboRaceUpdateCameraTargets). Any other kind follows behind the camera racer state. */
enum ChocoboCameraEventKind {
    CHOCOBO_CAMERA_EVENT_FIXED = 1,   /* the eye at the event's position; looks at the viewed racer, or with
                                         mode 0 at the event's point */
    CHOCOBO_CAMERA_EVENT_TRAIL = 2,   /* behind a racer state moved along the route at the mode's speed */
    CHOCOBO_CAMERA_EVENT_ORBIT = 3,   /* circles the viewed racer by the event's divisors */
    CHOCOBO_CAMERA_EVENT_FINISH = 0xA /* circles the winner from above once the race is over */
};

/* ChocoboRaceState::mode of a CHOCOBO_CAMERA_EVENT_TRAIL event: the speed of the trailing racer
 * state against the viewed racer's step speed. */
enum ChocoboCameraTrailSpeed {
    CHOCOBO_CAMERA_TRAIL_STOPPED = 0,
    CHOCOBO_CAMERA_TRAIL_HALF = 1,
    CHOCOBO_CAMERA_TRAIL_SAME = 2,
    CHOCOBO_CAMERA_TRAIL_DOUBLE = 3
};

/* ChocoboRacer::mode: how a computer-run racer paces itself (Mini_ChocoboRaceAdvanceRacerAlongCourse)
 * and the tint Mini_ChocoboRaceBuildRacers gives its model. */
enum ChocoboRacerMode {
    CHOCOBO_RACER_MODE_STEADY = 0,           /* holds its speed near its base speed; no tint */
    CHOCOBO_RACER_MODE_DASH_WHEN_LAST = 1,   /* dashes at random while last, else holds 0.9..1.25 x base */
    CHOCOBO_RACER_MODE_EARLY_SPRINT = 2,     /* pushes while its stamina is above a third; no late dash */
    CHOCOBO_RACER_MODE_STEADY_HOME_LANE = 3  /* STEADY, drifting back to its lane after its dash segment */
};

/* The racer modifiers Mini_ChocoboRaceApplyRacerEventCode applies (0 does nothing). */
enum ChocoboRacerEvent {
    CHOCOBO_RACER_EVENT_LATE_DASH = 0x01,         /* dash segment 0x14 after the segment table's */
    CHOCOBO_RACER_EVENT_MORE_STAMINA = 0x02,      /* maximum stamina x 7/3, refilled */
    CHOCOBO_RACER_EVENT_EARLY_DASH = 0x03,        /* dash segment 0x14 before the segment table's */
    CHOCOBO_RACER_EVENT_DASH_WHEN_SPURRED = 0x04,
    CHOCOBO_RACER_EVENT_NO_DASH = 0x05,
    CHOCOBO_RACER_EVENT_SMART = 0x06,             /* intelligence 100, racing line 0, STEADY_HOME_LANE */
    CHOCOBO_RACER_EVENT_FLAG_9C = 0x07,           /* sets ChocoboRacer::unusedEvent7Flag to 0xFFFF */
    CHOCOBO_RACER_EVENT_HALF_STAMINA = 0x08,
    CHOCOBO_RACER_EVENT_IGNORE_SLOW_1 = 0x09,     /* only CHOCOBO_TRACK_FLAG_SLOW_2 terrain slows it */
    CHOCOBO_RACER_EVENT_IGNORE_SLOW_2 = 0x0A,     /* only CHOCOBO_TRACK_FLAG_SLOW_1 terrain slows it */
    CHOCOBO_RACER_EVENT_START_BOOST = 0x0B,       /* and half the time half its stamina */
    CHOCOBO_RACER_EVENT_LOCK_BASE_SPEED = 0x0C,
    CHOCOBO_RACER_EVENT_OUTER_RACING_LINE = 0x0D  /* sets racingLine to 3 */
};

/* g_ChocoboRaceClass (savemap raceClass): the race's class, which picks the bet prize table
 * (Mini_ChocoboRaceBuildBetGrid) and the bet costs. */
enum ChocoboRaceClass {
    CHOCOBO_RACE_CLASS_C = 0,
    CHOCOBO_RACE_CLASS_B = 1,
    CHOCOBO_RACE_CLASS_A = 2,
    CHOCOBO_RACE_CLASS_S = 3
};

/* g_ChocoboRacePhase: the state of Mini_ChocoboRunRaceResultLoop. */
enum ChocoboRacePhase {
    CHOCOBO_RACE_PHASE_SETUP = 0,          /* reset the bet screen, or skip to START */
    CHOCOBO_RACE_PHASE_BET_SCREEN = 1,     /* Mini_ChocoboRaceUpdateBetScreen each frame */
    CHOCOBO_RACE_PHASE_START = 2,          /* one-shot race start */
    CHOCOBO_RACE_PHASE_RACE = 3,
    CHOCOBO_RACE_PHASE_FINISH = 4,         /* places for the racers that did not finish */
    CHOCOBO_RACE_PHASE_RESULT_SCREEN = 5,  /* Mini_ChocoboRaceUpdateResultScreen each frame */
    CHOCOBO_RACE_PHASE_EXIT = 6
};

/* ChocoboBetCell::mode: the side Mini_ChocoboRaceUpdateBetGridCells turns a bet-grid card to.
 * The front is face 0 (drawn as quad kind 5), the back face -1 (kind 4); picking a bet turns its
 * card to the front, dropping it turns it to the back. */
enum ChocoboBetCellMode {
    CHOCOBO_BET_CELL_MODE_IDLE = -1,
    CHOCOBO_BET_CELL_MODE_TURN_TO_FRONT = 1,
    CHOCOBO_BET_CELL_MODE_TURN_TO_BACK = 2
};

/* ChocoboBetCell::state: a bet-grid card's turn. The card is edge-on at slide 0 and full width
 * at slide 16. */
enum ChocoboBetCellState {
    CHOCOBO_BET_CELL_EDGE_ON = 0,
    CHOCOBO_BET_CELL_FRONT = 1,
    CHOCOBO_BET_CELL_BACK = 2,
    CHOCOBO_BET_CELL_FRONT_CLOSING = 3,
    CHOCOBO_BET_CELL_FRONT_OPENING = 4,
    CHOCOBO_BET_CELL_BACK_CLOSING = 5,
    CHOCOBO_BET_CELL_BACK_OPENING = 6
};

/* IEEE-754 float bit patterns stored in ChocoboSpriteRecord's UV, texel-size and depth int
 * fields (chocobo_race_hud_submit.cpp has no __fltused symbol in the retail object and copies
 * every UV/size slot with integer mov instructions; see FLOAT_ONE_BITS in lib/codegen.h). */
enum ChocoboHudFloatBits {
    CHOCOBO_FLOAT_0_09_BITS = 0x3DB851EC,   /* 0.09f */
    CHOCOBO_FLOAT_0_121_BITS = 0x3DF7CED9,  /* 0.121f */
    CHOCOBO_FLOAT_0_13_BITS = 0x3E051EB8,   /* 0.13f */
    CHOCOBO_FLOAT_0_1875_BITS = 0x3E400000, /* 0.1875f */
    CHOCOBO_FLOAT_0_19_BITS = 0x3E428F5C,   /* 0.19f */
    CHOCOBO_FLOAT_0_22_BITS = 0x3E6147AE,   /* 0.22f */
    CHOCOBO_FLOAT_0_23_BITS = 0x3E6B851F,   /* 0.23f */
    CHOCOBO_FLOAT_0_25_BITS = 0x3E800000,   /* 0.25f */
    CHOCOBO_FLOAT_0_28_BITS = 0x3E8F5C29,   /* 0.28f */
    CHOCOBO_FLOAT_0_38_BITS = 0x3EC28F5C,   /* 0.38f */
    CHOCOBO_FLOAT_0_444_BITS = 0x3EE353F8,  /* 0.444f */
    CHOCOBO_FLOAT_0_47_BITS = 0x3EF0A3D7,   /* 0.47f */
    CHOCOBO_FLOAT_0_63_BITS = 0x3F2147AE,   /* 0.63f */
    CHOCOBO_FLOAT_0_94_BITS = 0x3F70A3D7,   /* 0.94f */
    CHOCOBO_FLOAT_0_97_BITS = 0x3F7851EC,   /* 0.97f */
    CHOCOBO_FLOAT_0_98_BITS = 0x3F7AE148,   /* 0.98f */
    CHOCOBO_FLOAT_16_BITS = 0x41800000,     /* 16.0f */
    CHOCOBO_FLOAT_24_BITS = 0x41C00000,     /* 24.0f */
    CHOCOBO_FLOAT_31_BITS = 0x41F80000,     /* 31.0f */
    CHOCOBO_FLOAT_32_BITS = 0x42000000,     /* 32.0f */
    CHOCOBO_FLOAT_48_BITS = 0x42400000,     /* 48.0f */
    CHOCOBO_FLOAT_63_BITS = 0x427C0000,     /* 63.0f */
    CHOCOBO_FLOAT_64_BITS = 0x42800000,     /* 64.0f */
    CHOCOBO_FLOAT_96_BITS = 0x42C00000      /* 96.0f */
};

/* ChocoboBetCell::tier odds tier (Mini_ChocoboRaceUpdateBetGridCells). */
enum ChocoboBetTicketTier {
    CHOCOBO_BET_TIER_NORMAL = 0,
    CHOCOBO_BET_TIER_HIGH = 1,
    CHOCOBO_BET_TIER_MAX = 2
};


enum ChocoboFinishPlacement {
    CHOCOBO_PLACE_1ST = 1,
    CHOCOBO_PLACE_2ND = 2,
    CHOCOBO_PLACE_6TH = 6
};

#endif

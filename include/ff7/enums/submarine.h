/* include/ff7/enums/submarine.h: named codes of the submarine minigame. Enums only. */
#ifndef FF7_ENUMS_SUBMARINE_H
#define FF7_ENUMS_SUBMARINE_H

/* SubmarineFlightRecord::state of one torpedo record (a single bit at a time). */
enum SubmarineFlightState {
    SUBMARINE_FLIGHT_IDLE = 0x01,       /* unused slot, free to launch */
    SUBMARINE_FLIGHT_HOMING = 0x02,     /* steering toward its target */
    SUBMARINE_FLIGHT_COASTING = 0x04,   /* lost the target, flying straight */
    SUBMARINE_FLIGHT_HIT_GROUND = 0x08, /* ended against the terrain */
    SUBMARINE_FLIGHT_EXPIRED = 0x10,    /* ran out of steps */
    SUBMARINE_FLIGHT_HIT_TARGET = 0x20  /* reached its target */
};

/* The state half (upper 16 bits) of a SubmarineState::cooldownTable word, one per torpedo launcher;
 * while reloading the low half counts the frames left. */
enum SubmarineLauncherState {
    SUBMARINE_LAUNCHER_FIRED = 0,         /* just fired; the reload starts at the next motion step */
    SUBMARINE_LAUNCHER_RELOADING = 0x10000,
    SUBMARINE_LAUNCHER_READY = 0x20000, /* can launch again */
    SUBMARINE_LAUNCHER_TIMER_MASK = 0xFFFF
};

/* SubmarineGridCell::flags */
enum SubmarineGridCellFlag {
    SUBMARINE_CELL_RAISED = 0x1000, /* currentHeight steps toward raisedHeight */
    SUBMARINE_CELL_FLAGGED = 0x2000, /* a marker the submarine can collect */
    SUBMARINE_CELL_TIMED = 0x8000, /* the low byte counts down */
    SUBMARINE_CELL_TIMER_INIT = 0x0080
};

/* SubmarineActiveFlag::state: an enemy's sonar contact. A contact inside the sweep window
 * (g_SubmarineState.currentRowField2 past the sweep) becomes NEW, or TRACKED when it was LOST;
 * a NEW contact outside it becomes LOST. When frameCount runs out LOCKED drops to TRACKED,
 * TRACKED to LOST and the others to 0. Submarine_UpdateActiveFlagRecord locks the nearest
 * TRACKED contact. */
enum SubmarineContactState {
    SUBMARINE_CONTACT_NONE = 0,
    SUBMARINE_CONTACT_NEW = 0x100,
    SUBMARINE_CONTACT_LOST = 0x200,
    SUBMARINE_CONTACT_TRACKED = 0x400,
    SUBMARINE_CONTACT_LOCKED = 0x800,
    SUBMARINE_CONTACT_ANY = 0xF00
};

/* g_SubmarineResultCode: how a submarine run ended (Submarine_ModuleFrame). Submarine_ModuleExit
 * saves a win only for CLEARED. */
enum SubmarineResult {
    SUBMARINE_RESULT_NONE = 0,       /* still playing */
    SUBMARINE_RESULT_CLEARED = 1,    /* the mission's goal is met */
    SUBMARINE_RESULT_DESTROYED = 2,  /* the player's playerResource ran out */
    SUBMARINE_RESULT_TIME_UP = 3,    /* g_SubmarineElapsedFrames passed g_SubmarineTimeLimit */
    SUBMARINE_RESULT_QUIT = 5        /* quit from the pause menu */
};

/* The four 256x256 atlas pages Submarine_DrawAtlasQuad draws from at 640x480, as (x >= 0x100) << 1 |
 * (y >= 0x100), each with its own batch: batch, batchC, batchB, batchD. */
enum SubmarineAtlasPage {
    SUBMARINE_ATLAS_PAGE_TOP_LEFT = 0,
    SUBMARINE_ATLAS_PAGE_BOTTOM_LEFT = 1,
    SUBMARINE_ATLAS_PAGE_TOP_RIGHT = 2,
    SUBMARINE_ATLAS_PAGE_BOTTOM_RIGHT = 3
};

/* How Submarine_DrawAtlasQuad splits a source rectangle that crosses a page edge. */
enum SubmarineAtlasSplit {
    SUBMARINE_ATLAS_SPLIT_X_TOP = 1,     /* crosses x = 0x100 in the top pages */
    SUBMARINE_ATLAS_SPLIT_X_BOTTOM = 2,  /* crosses x = 0x100 in the bottom pages */
    SUBMARINE_ATLAS_SPLIT_Y_LEFT = 3,    /* crosses y = 0x100 in the left pages */
    SUBMARINE_ATLAS_SPLIT_Y_RIGHT = 4,   /* crosses y = 0x100 in the right pages */
    SUBMARINE_ATLAS_SPLIT_XY = 5         /* crosses both: four pieces */
};


enum SubmarineMotionFlag {
    SUBMARINE_MOTION_HIT_TERRAIN = 0x0001,
    SUBMARINE_MOTION_NEAR_TERRAIN = 0x0002,
    SUBMARINE_MOTION_HIT_MINE = 0x0004,
    SUBMARINE_MOTION_HUD_ALERT_100 = 0x0100,
    SUBMARINE_MOTION_HUD_ALERT_200 = 0x0200,
    SUBMARINE_MOTION_HUD_ALERT_400 = 0x0400,
    SUBMARINE_MOTION_HUD_ALERT_800 = 0x0800
};

enum SubmarineMissionLevel {
    SUBMARINE_LEVEL_STORY = 0,
    SUBMARINE_LEVEL_1 = 1,
    SUBMARINE_LEVEL_2 = 2,
    SUBMARINE_LEVEL_3 = 3,
    SUBMARINE_LEVEL_4 = 4,
    SUBMARINE_LEVEL_5 = 5
};

enum SubmarineQuitDialogState {
    SUBMARINE_QUIT_DIALOG_CONFIRM = 4,
    SUBMARINE_QUIT_DIALOG_CANCEL = 5
};

/* g_SubmarineFlags debug and rendering toggles. */
enum SubmarineDebugFlag {
    SUBMARINE_FLAG_DEBUG_ROT_INC = 0x01,
    SUBMARINE_FLAG_DEBUG_ROT_DEC = 0x02,
    SUBMARINE_FLAG_DEBUG_STEP = 0x04,
    SUBMARINE_FLAG_HIDE_HUD_GAUGES = 0x08,
    SUBMARINE_FLAG_FREEZE_TIMER = 0x10,
    SUBMARINE_FLAG_NO_TERRAIN_SHADE = 0x20,
    SUBMARINE_FLAG_DEBUG_WIREFRAME = 0x40
};

/* g_SubmarineControlFlags pause, mission-end, and quit-dialog state bits. */
enum SubmarineControlFlag {
    SUBMARINE_CTRL_PAUSED = 0x01,
    SUBMARINE_CTRL_ENDED = 0x02,
    SUBMARINE_CTRL_QUIT_DIALOG = 0x04,
    SUBMARINE_CTRL_QUIT_CONFIRM = 0x08
};

/* g_SubmarineGameState HUD overlay bitmask dispatched by Submarine_DrawHudMain. */
enum SubmarineHudOverlayFlag {
    SUBMARINE_HUD_PAUSE_BANNER = 0x001,
    SUBMARINE_HUD_QUIT_PROMPT = 0x002,
    SUBMARINE_HUD_PAUSE_HELP = 0x004,
    SUBMARINE_HUD_MISSION_COMPLETE = 0x008,
    SUBMARINE_HUD_MISSION_FAILED = 0x010,
    SUBMARINE_HUD_TIME_UP = 0x020,
    SUBMARINE_HUD_TARGET_TALLY = 0x040,
    SUBMARINE_HUD_SCORE_PANEL = 0x080,
    SUBMARINE_HUD_SCREEN_TINT = 0x100
};
#endif

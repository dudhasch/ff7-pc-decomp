/* include/ff7/enums/highway.h: named codes of the highway minigame. Enums only. */
#ifndef FF7_ENUMS_HIGHWAY_H
#define FF7_ENUMS_HIGHWAY_H

/* Indexes of HighwayTrackFrame::info. */
enum HighwayFrameInfo {
    HIGHWAY_FRAME_INFO_SEQUENCE_ENTRY = 0, /* animation-table entry of the frame; value 4 latches the skip flag */
    HIGHWAY_FRAME_INFO_SEGMENT_BASE = 3,   /* frame index modulo 8, the drawSegment base */
    HIGHWAY_FRAME_INFO_SKIPPED = 9         /* nonzero: draw skips the frame */
};

/* The eventType of a HighwayTrackEvent that Mini_HighwayRunEventTimeline runs. */
enum HighwayTrackEventType {
    HIGHWAY_EVENT_SCENE_PRESET_05 = 0x05,    /* scene block preset for event 5 */
    HIGHWAY_EVENT_SCENE_ON = 0x06,           /* sets the scene block's sceneKind to 1 */
    HIGHWAY_EVENT_STOP_SPAWNING = 0x08,      /* flags three actor groups as flying away and stops spawning */
    HIGHWAY_EVENT_LAYER1_ON = 0x0A,
    HIGHWAY_EVENT_LAYER1_OFF = 0x0B,
    HIGHWAY_EVENT_LAYER2_ON = 0x0C,
    HIGHWAY_EVENT_LAYER2_OFF = 0x0D,
    HIGHWAY_EVENT_SHOW_PART = 0x0E,          /* silences sound 0 and 0x126 and shows an actor part */
    HIGHWAY_EVENT_LAYER3_ON = 0x10,
    HIGHWAY_EVENT_LAYER3_OFF = 0x11,
    HIGHWAY_EVENT_SCENE_INIT = 0x12,         /* scene effect init(5, 4, 0x27100) */
    HIGHWAY_EVENT_SCENE_PRESET_13 = 0x13,    /* scene block preset for event 0x13 */
    HIGHWAY_EVENT_ENDING_A = 0x14,           /* places three actors and starts ending A */
    HIGHWAY_EVENT_ENDING_B = 0x15,           /* starts ending B (once) */
    HIGHWAY_EVENT_BANNER_1 = 0x16,
    HIGHWAY_EVENT_BANNER_3 = 0x17,
    HIGHWAY_EVENT_BANNER_NONE = 0x18,
    HIGHWAY_EVENT_LAYER4_ON = 0x19,
    HIGHWAY_EVENT_LAYER4_OFF = 0x1A,
    HIGHWAY_EVENT_SET_GROUP_STATE = 0x1C     /* sets an actor group's slot0State to 6 */
};

/* Bits of the highway input button mask (g_HighwayInputButtonMask / g_HighwayInputRawMask). */
enum HighwayInputButton {
    HIGHWAY_BUTTON_CIRCLE = 0x01,
    HIGHWAY_BUTTON_SQUARE = 0x02,
    HIGHWAY_BUTTON_UP = 0x04,
    HIGHWAY_BUTTON_DOWN = 0x08,
    HIGHWAY_BUTTON_LEFT = 0x10,
    HIGHWAY_BUTTON_RIGHT = 0x20
};

/* Values of g_HighwayInputAction. */
enum HighwayInputAction {
    HIGHWAY_ACTION_NONE = 0,
    HIGHWAY_ACTION_SQUARE = 1,
    HIGHWAY_ACTION_CIRCLE = 2
};

/* Values of g_HighwayInputDirection: the digit of the numeric keypad that points the same way. */
enum HighwayInputDirection {
    HIGHWAY_DIR_NONE = 0,
    HIGHWAY_DIR_DOWN_LEFT = 1,
    HIGHWAY_DIR_DOWN = 2,
    HIGHWAY_DIR_DOWN_RIGHT = 3,
    HIGHWAY_DIR_LEFT = 4,
    HIGHWAY_DIR_RIGHT = 6,
    HIGHWAY_DIR_UP_LEFT = 7,
    HIGHWAY_DIR_UP = 8,
    HIGHWAY_DIR_UP_RIGHT = 9
};

/* HighwaySlotState.mode: the kind of a slot's actor. Slot 0 is the player (steered by the input);
 * slot 1 is the vehicle the enemies steer at; slots 2..4 are the spawned enemies, which take hits and
 * score when destroyed; slot 5 is the boss/heavy pursuer in mode 0xA. */
enum HighwaySlotMode {
    HIGHWAY_SLOT_MODE_PLAYER = 0,
    HIGHWAY_SLOT_MODE_ESCORTED = 1,
    HIGHWAY_SLOT_MODE_ENEMY = 2,
    HIGHWAY_SLOT_MODE_BOSS = 0xA,
    HIGHWAY_SLOT_MODE_BOSS_CHARGE = 0x0B,
    HIGHWAY_SLOT_MODE_BOSS_RETREAT = 0x0C
};

/* HighwaySlotState.state: a slot actor's condition. */
enum HighwaySlotCondition {
    HIGHWAY_SLOT_STATE_NORMAL = 0,
    HIGHWAY_SLOT_STATE_HIT = 1,       /* stunned while its timer runs down, then NORMAL */
    HIGHWAY_SLOT_STATE_DESTROYED = 2, /* spins out and despawns when its counter runs out */
    HIGHWAY_SLOT_STATE_INACTIVE = 5   /* despawned: not stepped or transformed, free to spawn */
};

/* HighwayScene::mode (HighwaySceneBlock2::sceneKind): how HighwayScene::update places the camera. */
enum HighwaySceneCameraMode {
    HIGHWAY_CAMERA_MODE_OFFSETS = 0,   /* eye and target at the scene's offsets from the player's car */
    HIGHWAY_CAMERA_MODE_CHASE = 1,     /* behind and above the player's car */
    HIGHWAY_CAMERA_MODE_PATH = 4,      /* CHASE moved along a sampled path (HighwayEffect::init), then CHASE */
    HIGHWAY_CAMERA_MODE_FIXED_EYE = 5  /* the eye at renderOffset beside the track, looking at the car */
};


enum HighwayEnemySlotIndex {
    HIGHWAY_ENEMY_SLOT_LEFT = 2,
    HIGHWAY_ENEMY_SLOT_CENTER = 3,
    HIGHWAY_ENEMY_SLOT_RIGHT = 4
};

enum HighwayEnemySpawnVariant {
    HIGHWAY_SPAWN_VARIANT_LIGHT = 0,
    HIGHWAY_SPAWN_VARIANT_HEAVY = 1,
    HIGHWAY_SPAWN_VARIANT_MEDIUM = 2
};

enum HighwayPoolEffectKind {
    HIGHWAY_EFFECT_SMOKE_TRAIL = 1,
    HIGHWAY_EFFECT_SPARK_A = 2,
    HIGHWAY_EFFECT_SPARK_B = 3,
    HIGHWAY_EFFECT_FLYING_DEBRIS = 4
};

/* g_HighwayControlFlags debug camera and scroll-freeze bits. */
enum HighwayControlFlag {
    HIGHWAY_CTRL_DEBUG_CAM_MODE7 = 0x01,
    HIGHWAY_CTRL_DEBUG_CAM_MODE6_FREEZE_SCROLL = 0x02
};
#endif

/* include/ff7/enums/wm.h: named codes of ff7/wm and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_WM_H
#define FF7_ENUMS_WM_H

/* World-map script opcodes run by WorldScript_ExecOperator, WorldScript_ExecControlFlow and _ExecCommand. */
enum WorldScriptOp {
    WORLD_SCRIPT_NEG = 0x15,
    WORLD_SCRIPT_NOT = 0x17,
    WORLD_SCRIPT_ZONE_DISTANCE = 0x18,
    WORLD_SCRIPT_ENTITY_DISTANCE = 0x19,
    WORLD_SCRIPT_SAVED_POSITION_DISTANCE = 0x1A,
    WORLD_SCRIPT_ZONE_ANGLE = 0x1B,
    WORLD_SCRIPT_MUL = 0x30,
    WORLD_SCRIPT_ADD = 0x40,
    WORLD_SCRIPT_SUB = 0x41,
    WORLD_SCRIPT_SHL = 0x50,
    WORLD_SCRIPT_SHR = 0x51,
    WORLD_SCRIPT_LT = 0x60,
    WORLD_SCRIPT_GT = 0x61,
    WORLD_SCRIPT_LE = 0x62,
    WORLD_SCRIPT_GE = 0x63,
    WORLD_SCRIPT_EQ = 0x70,
    WORLD_SCRIPT_NE = 0x71,
    WORLD_SCRIPT_AND = 0x80,
    WORLD_SCRIPT_OR = 0xA0,
    WORLD_SCRIPT_LOGICAL_AND = 0xB0,
    WORLD_SCRIPT_LOGICAL_OR = 0xC0,
    WORLD_SCRIPT_ASSIGN = 0xE0,
    WORLD_SCRIPT_GOTO = 0x200,
    WORLD_SCRIPT_GOTO_IF_FALSE = 0x201,
    WORLD_SCRIPT_RETURN = 0x203,
    WORLD_SCRIPT_SPAWN_ENTITY = 0x300,
    WORLD_SCRIPT_SET_PLAYER_ENTITY = 0x302,
    WORLD_SCRIPT_SET_SPEED = 0x303,
    WORLD_SCRIPT_SET_DIRECTION = 0x304,
    WORLD_SCRIPT_SET_WAIT_FRAMES = 0x305,
    WORLD_SCRIPT_STEP = 0x306,                              /* yields while waitFrames > 0 */
    WORLD_SCRIPT_SET_PLAYER_CONTROL = 0x307,
    WORLD_SCRIPT_SET_MESH_XZ = 0x308,
    WORLD_SCRIPT_SET_MESH_OFFSET_XZ = 0x309,
    WORLD_SCRIPT_SET_Y_OFFSET_SPEED = 0x30A,
    WORLD_SCRIPT_SET_Y_OFFSET = 0x30B,
    WORLD_SCRIPT_BOARD_VEHICLE = 0x30C,
    WORLD_SCRIPT_REVERT_POSITION = 0x30D,
    WORLD_SCRIPT_SET_ANIMATION = 0x30E,                     /* args anim, dir; dir 0 starts at last frame */
    WORLD_SCRIPT_ZONE_SELECT = 0x310,
    WORLD_SCRIPT_ZONE_SET_MESH_XZ = 0x311,
    WORLD_SCRIPT_ZONE_SET_OFFSET_XZ = 0x312,
    WORLD_SCRIPT_ZONE_SET_AMBIENT = 0x313,
    WORLD_SCRIPT_ZONE_SET_RADII = 0x314,
    WORLD_SCRIPT_ZONE_SET_FOG_NEAR = 0x315,
    WORLD_SCRIPT_ZONE_SET_FOG_FAR = 0x316,
    WORLD_SCRIPT_START_BATTLE = 0x317,
    WORLD_SCRIPT_GOTO_FIELD = 0x318,
    WORLD_SCRIPT_SET_CAMERA_MODE = 0x319,                   /* bits 0-1 camera mode, bits 2-3 minimap */
    WORLD_SCRIPT_FREE_ENTITY = 0x31A,
    WORLD_SCRIPT_SET_CAMERA_ZOOM_STATE = 0x31C,
    WORLD_SCRIPT_PLAY_SFX = 0x31D,
    WORLD_SCRIPT_SET_CAMERA_FRONT = 0x31E,
    WORLD_SCRIPT_SET_CAMERA_YAW_STEP = 0x31F,
    WORLD_SCRIPT_INIT_ZOLOM_TRAIL = 0x320,
    WORLD_SCRIPT_FACE_ZONE = 0x321,
    WORLD_SCRIPT_SET_WINDOW_RECT = 0x324,
    WORLD_SCRIPT_SHOW_MESSAGE = 0x325,
    WORLD_SCRIPT_ASK_CHOICE = 0x326,
    WORLD_SCRIPT_WAIT_ANSWER = 0x327,
    WORLD_SCRIPT_SET_FACING = 0x328,
    WORLD_SCRIPT_SET_CAMERA_HEIGHT_STEP = 0x329,
    WORLD_SCRIPT_SET_CAMERA_DISTANCE_STEP = 0x32A,
    WORLD_SCRIPT_SET_ENCOUNTERS_ENABLED = 0x32B,
    WORLD_SCRIPT_SET_WINDOW_MODE = 0x32C,
    WORLD_SCRIPT_CLOSE_WINDOW = 0x32D,
    WORLD_SCRIPT_WAIT_WINDOW = 0x32E,
    WORLD_SCRIPT_SET_CAMERA_FRONT_NOW = 0x32F,
    WORLD_SCRIPT_SELECT_ENTITY = 0x330,
    WORLD_SCRIPT_ARM_VEHICLE_EXIT = 0x331,
    WORLD_SCRIPT_DISABLE_SCRIPT_REQUESTS = 0x332,
    WORLD_SCRIPT_FACE_ENTITY = 0x333,
    WORLD_SCRIPT_WAIT_CALLED_SCRIPT = 0x334,
    WORLD_SCRIPT_DETACH_RIDER_AS_PLAYER = 0x335,
    WORLD_SCRIPT_SET_SPEED_X16 = 0x336,
    WORLD_SCRIPT_SET_VEHICLE_TRANSITION_ARG = 0x337,
    WORLD_SCRIPT_CLEAR_ACTOR_FLAG_8 = 0x338,
    WORLD_SCRIPT_SET_ACTOR_FLAG_8 = 0x339,
    WORLD_SCRIPT_SET_ALTITUDE_SPEED = 0x33A,
    WORLD_SCRIPT_FADE_OUT = 0x33B,
    WORLD_SCRIPT_ASCEND_EXIT = 0x33C,
    WORLD_SCRIPT_GOTO_FIELD_ENTRY = 0x33D,
    WORLD_SCRIPT_PLAY_MUSIC = 0x33E,
    WORLD_SCRIPT_START_FLOOD_FILL = 0x33F,
    WORLD_SCRIPT_PLACE_AT_FLOOD_FILL_RESULT = 0x340,
    WORLD_SCRIPT_WAIT_FLOOD_FILL = 0x341,
    WORLD_SCRIPT_WAIT_TERRAIN_EFFECT = 0x342,
    WORLD_SCRIPT_MOUNT_ENTITY = 0x343,
    WORLD_SCRIPT_SAVE_POSITION = 0x344,
    WORLD_SCRIPT_FACE_SAVED_POSITION = 0x345,
    WORLD_SCRIPT_SET_MODEL_ID = 0x346,
    WORLD_SCRIPT_COPY_POSITION_FROM = 0x347,
    WORLD_SCRIPT_FADE_IN = 0x348,
    WORLD_SCRIPT_SET_WORLD_PROGRESS = 0x349,
    WORLD_SCRIPT_SPAWN_ACTOR_EFFECT = 0x34A,
    WORLD_SCRIPT_SET_BOAT_TERRAIN_MASK = 0x34B,
    WORLD_SCRIPT_SET_MODEL13_TINT = 0x34C,
    WORLD_SCRIPT_SET_EFFECT_EMITTER = 0x34D,
    WORLD_SCRIPT_DISABLE_EFFECT_EMITTER = 0x34E,
    WORLD_SCRIPT_SET_ALTITUDE = 0x34F,
    WORLD_SCRIPT_SET_METEOR_VISIBLE = 0x350,
    WORLD_SCRIPT_SET_MIDI_VOLUME = 0x351,
    WORLD_SCRIPT_SET_PROJECTION_JITTER = 0x352,
    WORLD_SCRIPT_PLACE_PLAYER_AT_VEHICLE_RETURN = 0x353,
    WORLD_SCRIPT_SET_SCRIPT_REQUESTS_ENABLED = 0x354,
    WORLD_SCRIPT_START_COUNTDOWN_TIMER = 0x355
};

/* The kinds of a world script operand (WorldScriptOperand::kind without its two bank bits). A
 * push opcode 0x110..0x11F is the kind and bank of the operand it pushes; 0x100 empties the stack.
 * Every operator result is pushed as a constant. */
enum WorldScriptOperandKind {
    WORLD_SCRIPT_OPERAND_CLEAR_STACK = 0x100,   /* the push opcode that resets the cursor */
    WORLD_SCRIPT_OPERAND_CONSTANT = 0x110,      /* value is the operand */
    WORLD_SCRIPT_OPERAND_BIT = 0x114,           /* value is a bit offset into the bank */
    WORLD_SCRIPT_OPERAND_BYTE = 0x118,          /* value is a byte offset into the bank */
    WORLD_SCRIPT_OPERAND_WORD = 0x11C           /* value is the byte offset of a signed 16-bit word */
};

/* The special operands of the world script: an operand whose kind has both low bits set (bank 3)
 * holds one of these selectors, and WorldScript_PopStackValue returns the value it names for the
 * current actor or the world state. */
enum WorldScriptSpecial {
    WORLD_SCRIPT_SPECIAL_CELL_X = 0,                    /* pos.vx >> 13 */
    WORLD_SCRIPT_SPECIAL_CELL_Z = 1,                    /* pos.vz >> 13 */
    WORLD_SCRIPT_SPECIAL_OFFSET_X = 2,                  /* pos.vx & 0x1FFF */
    WORLD_SCRIPT_SPECIAL_OFFSET_Z = 3,                  /* pos.vz & 0x1FFF */
    WORLD_SCRIPT_SPECIAL_HEADING = 4,                   /* (direction >> 4) & 0xFF */
    WORLD_SCRIPT_SPECIAL_FIELD_TRANSITION_STATE = 5,    /* World_GetTransitionRequestState */
    WORLD_SCRIPT_SPECIAL_FIELD_TRANSITION_ARG = 6,      /* World_GetTransitionArgument */
    WORLD_SCRIPT_SPECIAL_CAMERA_MINIMAP_STATE = 7,      /* camera mode | mini-map state << 2 */
    WORLD_SCRIPT_SPECIAL_PLAYER_MODEL = 8,
    WORLD_SCRIPT_SPECIAL_CHOCOBO_ENCOUNTER = 9,         /* gSavemap.worldChocoboEncounterInfo */
    WORLD_SCRIPT_SPECIAL_BATTLE_FLAG_8 = 10,            /* g_BattleResultWord bit 3 */
    WORLD_SCRIPT_SPECIAL_SWAMP_ENTRY_X_FLAG = 11,       /* g_WorldZolomPlayerXFlag: x > 0x36000 on entering the swamp */
    WORLD_SCRIPT_SPECIAL_MESSAGE_ANSWER = 12,           /* WorldMsg_GetAnswer, -1 while the window is open */
    WORLD_SCRIPT_SPECIAL_REGION = 13,                   /* (walkmesh >> 5) & 7 */
    WORLD_SCRIPT_SPECIAL_LEADER_MODEL = 14,
    WORLD_SCRIPT_SPECIAL_ACTOR_TYPE = 15,
    WORLD_SCRIPT_SPECIAL_RANDOM_BYTE = 16,
    WORLD_SCRIPT_SPECIAL_VEHICLE_MODEL = 17,
    WORLD_SCRIPT_SPECIAL_PENDING_VEHICLE_MODEL = 18,
    WORLD_SCRIPT_SPECIAL_VEHICLE_SLOT = 19,
    WORLD_SCRIPT_SPECIAL_PENDING_VEHICLE_SLOT = 20
};

/* World-map stage ids picked by WorldMap_MainUpdate; World_LoadStageModels loads each stage's model set. */
enum WorldStage {
    WORLD_STAGE_OVERWORLD_EARLY = 0x00,                     /* buggy, tiny bronco; no highwind */
    WORLD_STAGE_OVERWORLD_DIAMOND_CANNON = 0x01,            /* highwind, canon, diaweapon */
    WORLD_STAGE_OVERWORLD_DIAMOND_MIDGUL_CANNON = 0x02,     /* highwind, midgulcanon, diaweapon */
    WORLD_STAGE_OVERWORLD_DIAMOND_CANNON_HW02 = 0x03,       /* highwind02, canon, diaweapon */
    WORLD_STAGE_OVERWORLD_DIAMOND_MIDGUL_HW02 = 0x04,       /* highwind02, midgulcanon, diaweapon */
    WORLD_STAGE_OVERWORLD_ULTIMA = 0x05,                    /* highwind, ultima weapon */
    WORLD_STAGE_OVERWORLD_ULTIMA_HW02 = 0x06,               /* highwind02, ultima weapon */
    WORLD_STAGE_OVERWORLD_RUBY = 0x07,                      /* highwind, ruby weapon */
    WORLD_STAGE_OVERWORLD_RUBY_HW02 = 0x08,                 /* highwind02, ruby weapon */
    WORLD_STAGE_WARSHIP = 0x09,                             /* story variant 0 */
    WORLD_STAGE_WARSHIP_LATE = 0x0A,                        /* story variant > 0 */
    WORLD_STAGE_UNDERWATER = 0x0B,
    WORLD_STAGE_SNOWFIELD = 0x0C
};

/* The texture set a stage loads (World_GetStageTextureSetId). Its low nibble picks the set;
   WORLD_TEXTURE_SET_LARGE_MINIMAP selects the larger mini-map and is masked away after that. */
enum WorldTextureSet {
    WORLD_TEXTURE_SET_OVERWORLD = 0x00,
    WORLD_TEXTURE_SET_UNDERWATER = 0x02,
    WORLD_TEXTURE_SET_SNOW = 0x03,
    WORLD_TEXTURE_SET_MASK = 0x0F,
    WORLD_TEXTURE_SET_LARGE_MINIMAP = 0x80
};

/* Archive id of the world map's LGP (wm/world_us.lgp) in file-open requests and FindLgpFile. */
enum WorldArchive {
    WORLD_ARCHIVE_ID = 5
};

/* World-map walkmesh triangle types: what WorldEntity_GetPlayerTerrainType returns under the
   player (each value verified against its WM0.MAP triangles and surface textures). 15, 23 and 31 do not occur. World_CanModelEnterTerrain's masks
   and g_WorldChocoboTerrainMasks are sets of these values (bit n is type n). */
enum WorldTerrain {
    WORLD_TERRAIN_GRASS = 0,                    /* fld; the only type the Highwind lands on */
    WORLD_TERRAIN_FOREST = 1,                   /* wood, lostwod */
    WORLD_TERRAIN_MOUNTAIN = 2,                 /* ggmt, snw_mt, md_mt */
    WORLD_TERRAIN_SEA = 3,                      /* the submarine dives only here */
    WORLD_TERRAIN_RIVER_CROSSING = 4,           /* a shallow ford */
    WORLD_TERRAIN_RIVER = 5,                    /* river between cliffs */
    WORLD_TERRAIN_SHALLOW_WATER = 6,            /* coastal water */
    WORLD_TERRAIN_SWAMP = 7,                    /* Midgar marsh */
    WORLD_TERRAIN_DESERT = 8,
    WORLD_TERRAIN_WASTELAND = 9,                /* around Midgar */
    WORLD_TERRAIN_SNOW = 10,                    /* snwfld */
    WORLD_TERRAIN_RIVERSIDE = 11,               /* river bank */
    WORLD_TERRAIN_CLIFF = 12,
    WORLD_TERRAIN_COREL_BRIDGE = 13,
    WORLD_TERRAIN_WUTAI_BRIDGE = 14,            /* rope bridges */
    WORLD_TERRAIN_HILLSIDE = 16,                /* slopes; encounters treat it as GRASS */
    WORLD_TERRAIN_BEACH = 17,
    WORLD_TERRAIN_SUB_PEN = 18,                 /* a dock basin */
    WORLD_TERRAIN_CANYON = 19,                  /* Cosmo Canyon */
    WORLD_TERRAIN_MOUNTAIN_PASS = 20,           /* mt_road */
    WORLD_TERRAIN_BRIDGE_RIM = 21,              /* under bridges and the crater rim; medium confidence */
    WORLD_TERRAIN_WATERFALL = 22,
    WORLD_TERRAIN_GOLD_SAUCER_DESERT = 24,      /* encounters treat it as DESERT */
    WORLD_TERRAIN_JUNGLE = 25,                  /* forest's twin in encounters and ground sink */
    WORLD_TERRAIN_SEA_2 = 26,                   /* sea that is not a dive spot; medium confidence */
    WORLD_TERRAIN_NORTHERN_CRATER = 27,         /* region 7 */
    WORLD_TERRAIN_GOLD_SAUCER_DESERT_EDGE = 28, /* walkable on foot */
    WORLD_TERRAIN_BRIDGEHEAD = 29,              /* bridge ends */
    WORLD_TERRAIN_ENTRANCE = 30                 /* small location models, a cave mouth; medium confidence */
};

/* World-map model ids (WorldActor.actorType, WorldEntity_GetPlayerModelId) seen in game;
   only these are named, where the code compares them. */
enum WorldModel {
    WORLD_MODEL_LEADER_0 = 0,                   /* on foot (the party leader's models 0-2) */
    WORLD_MODEL_LEADER_1 = 1,
    WORLD_MODEL_LEADER_2 = 2,
    WORLD_MODEL_HIGHWIND = 3,
    WORLD_MODEL_CHOCOBO_4 = 4,                  /* a chocobo model; which one is not settled */
    WORLD_MODEL_TINY_BRONCO = 5,
    WORLD_MODEL_BUGGY = 6,
    WORLD_MODEL_SHIP = 8,
    WORLD_MODEL_DIAMOND_WEAPON = 10,            /* diaweapon */
    WORLD_MODEL_ULTIMA_WEAPON = 11,             /* ultima */
    WORLD_MODEL_SUBMARINE = 13,
    WORLD_MODEL_CHOCOBO = 19,                   /* the ridden chocobo */
    WORLD_MODEL_ROCK = 25,                      /* rock */
    WORLD_MODEL_RED_SUBMARINE = 28,             /* sbmrn2 */
    WORLD_MODEL_OVERLAY_CHOCOBO_41 = 41,        /* overlay chocobo actor (>= 32) */
    WORLD_MODEL_OVERLAY_CHOCOBO_42 = 42,
    WORLD_MODEL_ZOLOM = 100                     /* Midgar Zolom swamp actor */
};

/* g_WorldMode, the world map's state (World_UpdateMapStateMachine, WorldMap_MainUpdate). The states from
   WORLD_MAP_STATE_DIVED up leave the world map module: WorldMap_MainUpdate runs the departure script and
   exports the transition; WorldMap_MainUpdate reads 6 and 7 again on the next entry (the underwater map,
   boarding the saved vehicle). */
enum WorldMapState {
    WORLD_MAP_STATE_LOADED = -15,      /* set after loading; counts up to 0, then fades in and plays */
    WORLD_MAP_STATE_CAMERA_INTRO = 0,  /* the camera distance and height ramp in before control */
    WORLD_MAP_STATE_PLAYING = 1,       /* player control; encounters run only here */
    WORLD_MAP_STATE_FADE_OUT = 2,      /* fading to black for the menu or a field (WorldMap_RequestFieldTransition) */
    WORLD_MAP_STATE_LEAVING = 3,       /* a battle, field or plain return was requested; next frame EXIT */
    WORLD_MAP_STATE_DIVING = 4,        /* the submarine dives at sea (World_BeginSubmarineDive); then DIVED */
    WORLD_MAP_STATE_SURFACING = 5,     /* the submarine surfaces under water (World_BeginSubmarineSurfacing); then SURFACED */
    WORLD_MAP_STATE_DIVED = 6,         /* leave for the underwater map */
    WORLD_MAP_STATE_SURFACED = 7,      /* leave for the surface map */
    WORLD_MAP_STATE_SCRIPT_EXIT = 8,   /* a world script picked a field entry (World_BeginScriptFieldExit) */
    WORLD_MAP_STATE_EXIT = 9           /* leave; sets g_WorldMapExitRequested */
};

/* Where the world map leaves to (g_WorldFieldTransitionState, exported into g_FieldReturnMode;
 * WorldMap_ModuleExit switches on it). */
enum WorldExitMode {
    WORLD_EXIT_FIELD = 0,         /* a field entry from g_WorldFieldTable (World_SetFieldEntryFromTable): GAME_MODE_FIELD */
    WORLD_EXIT_BATTLE = 1,        /* a queued request (World_QueueFieldTransitionRequest): GAME_MODE_BATTLE_SWIRL */
    WORLD_EXIT_PLAIN_RETURN = 2   /* World_RequestPlainReturnTransition: posts FIELD_REQUEST_WORLD_ENDING_MOVIE */
};

/* WorldEffectRecord.flags, how World_UpdateEffectNodes animates a node while the record is current.
   FADE_SLOW is compared with the whole byte; the others are the bits under WORLD_EFFECT_RECORD_MODE_MASK. */
enum WorldEffectRecordMode {
    WORLD_EFFECT_RECORD_FADE_SLOW = 0x02,      /* intensity and packed alpha fall by one a frame */
    WORLD_EFFECT_RECORD_FADE_SCROLL = 0x04,    /* fade by 12 and step the record's frame by 0x10 */
    WORLD_EFFECT_RECORD_FADE_IN_OUT = 0x08,    /* ramp up by 8 to 0x80, then fade by 8 */
    WORLD_EFFECT_RECORD_LAUNCH = 0x0C,         /* once: upward velocity 200, deceleration 30 */
    WORLD_EFFECT_RECORD_FADE_FAST = 0x10,      /* fade by 16 */
    WORLD_EFFECT_RECORD_FADE = 0x14,           /* fade by 8 */
    WORLD_EFFECT_RECORD_LOOP_FRAMES = 0x18,    /* step the frame and loop back over the record run */
    WORLD_EFFECT_RECORD_MODE_MASK = 0x1C
};

/* World_SetPrimaryLightGroupPreset's preset: the main light of g_pWorldPrimaryLightGroup. */
enum WorldLightPreset {
    WORLD_LIGHT_PRESET_DIAGONAL = 0,     /* direction (1,1,1), grey; the default map lighting */
    WORLD_LIGHT_PRESET_OVERHEAD = 1,     /* direction (0,1,0), grey; the default on the underwater map */
    WORLD_LIGHT_PRESET_ZONE_COLOR = 2,   /* colour from g_WorldAmbientColor, inside a lighting zone */
    WORLD_LIGHT_PRESET_RESET_COLOR = 3   /* grey again, directions kept; outside every zone */
};

/* The edges of a world terrain triangle as bits (the flood-fill probe's flags, low three bits): the
   edges the flood fill entered by or has visited (World_AdvanceTerrainFloodFillStep). */
enum WorldTerrainFloodEdge {
    WORLD_FLOOD_EDGES_NONE = 0,
    WORLD_FLOOD_EDGE_V0_V1 = 1,
    WORLD_FLOOD_EDGE_V1_V2 = 2,
    WORLD_FLOOD_EDGE_V2_V0 = 4,
    WORLD_FLOOD_EDGES_ALL = 7
};

/* g_WorldTerrainFloodState: the mesh border a flood fill is crossing (World_TerrainFloodVisitEdgeNeighbor
   sets it; World_PushTerrainFloodCrossingRecord finds the entry triangle on the neighbouring mesh). */
enum WorldTerrainFloodCrossState {
    WORLD_FLOOD_CROSS_NONE = 0,
    WORLD_FLOOD_CROSS_NEG_X = 1,       /* out of the low-X edge, into the mesh column before */
    WORLD_FLOOD_CROSS_NEG_Z = 2,       /* out of the low-Z edge, into the mesh row before */
    WORLD_FLOOD_CROSS_POS_X = 3,
    WORLD_FLOOD_CROSS_POS_Z = 4,
    WORLD_FLOOD_CROSS_FAILED = -1      /* no usable triangle across the border; the probe is popped */
};

/* The result of one flood-fill step (World_TerrainFloodVisitEdgeNeighbor, World_AdvanceTerrainFloodFillStep). */
enum WorldTerrainFloodVisit {
    WORLD_FLOOD_VISIT_PUSHED = 0,          /* an unvisited neighbour was pushed */
    WORLD_FLOOD_VISIT_BLOCKED = 1,         /* no neighbour, or one the fill may not enter */
    WORLD_FLOOD_VISIT_CROSSED_BORDER = 2,  /* the edge is on the mesh border; a crossing is pending */
    WORLD_FLOOD_VISIT_OVERFLOW = 3,        /* the probe stack is full */
    WORLD_FLOOD_VISIT_DONE = 4             /* the probe stack is empty: the fill is complete */
};

/* g_WorldStreamingStallFadeState: the fade to black while terrain streaming falls behind
   (World_SetStreamingStallFadeState, World_StepStreamingStallFadeState). */
enum WorldStreamStallState {
    WORLD_STREAM_STALL_NONE = 0,
    WORLD_STREAM_STALL_FADE_OUT = 1, /* over 2 block slots busy for 300 frames: control off, fade to black */
    WORLD_STREAM_STALL_BLACK = 2,    /* the fade is done; waits for a frame step of 4 or 5 clock ticks */
    WORLD_STREAM_STALL_STEADY = 3,   /* the frame step was steady */
    WORLD_STREAM_STALL_RESUME = 4    /* requested when streaming catches up: control on, fade in; stored as NONE */
};

/* World_SpawnActorEffectsByKind effect IDs for splashes, wakes, propellers, shockwaves, and dust trails. */
enum WorldActorEffectId {
    WORLD_ACTOR_EFFECT_SPLASH_RING = 1,
    WORLD_ACTOR_EFFECT_SAND_PUFF = 2,
    WORLD_ACTOR_EFFECT_WAKE_SPRAY = 3,
    WORLD_ACTOR_EFFECT_PROPELLER_A = 4,
    WORLD_ACTOR_EFFECT_PROPELLER_B = 5,
    WORLD_ACTOR_EFFECT_SHOCKWAVE = 6,
    WORLD_ACTOR_EFFECT_DUST_20 = 20,
    WORLD_ACTOR_EFFECT_DUST_21 = 21,
    WORLD_ACTOR_EFFECT_DUST_22 = 22,
    WORLD_ACTOR_EFFECT_DUST_HIGH = 23
};


enum WorldChocoboCaptureState {
    WORLD_CAPTURE_STATE_NONE = 0,
    WORLD_CAPTURE_STATE_MOUNT_OR_SCRIPT = 1,
    WORLD_CAPTURE_STATE_DISMOUNT_FREE = 2,
    WORLD_CAPTURE_STATE_CHOCOBO_4_FREE = 3
};

enum WorldFordCrossingState {
    WORLD_FORD_WAIT_RIVERSIDE = 1,
    WORLD_FORD_ENTER_DELAY = 2,
    WORLD_FORD_CROSSING = 3,
    WORLD_FORD_EXIT_DELAY = 4
};

enum WorldCameraZoomState {
    WORLD_CAMERA_ZOOM_IDLE = 0,
    WORLD_CAMERA_ZOOM_STEPPING = 1
};

enum WorldWalkmeshByteMask {
    WORLD_WALKMESH_TERRAIN_MASK = 0x1F,
    WORLD_WALKMESH_REGION_MASK = 0xE0,
    WORLD_TRIANGLE_FLOOD_VISITED = 0x40
};

enum WorldTerrainMask {
    WORLD_TERRAIN_MASK_FOOT_BRIDGES = 0x20006000,
    WORLD_TERRAIN_MASK_FOOT_OVERWORLD = 0x721B6F83,
    WORLD_TERRAIN_MASK_CHOCOBO_DEFAULT = 0x321B6F83,
    WORLD_TERRAIN_MASK_VEHICLE_LAND = 0x021B6F83,
    WORLD_TERRAIN_MASK_SHALLOW_WATER = 0x00000070,
    WORLD_TERRAIN_MASK_BRONCO_DOCK = 0x00020800,
    WORLD_TERRAIN_MASK_BUGGY_DRIVE = 0x331B6F13,
    WORLD_TERRAIN_MASK_SUBMARINE_WATER = 0x04048008,
    WORLD_TERRAIN_MASK_SHIP_WATER = 0x04040008,
    WORLD_TERRAIN_MASK_EXIT_LAND = 0x221B0F03,
    WORLD_TERRAIN_MASK_EXIT_BUGGY = 0x221B0F83,
    WORLD_TERRAIN_MASK_DUST_STEP = 0x01110701,
    WORLD_TERRAIN_MASK_SPLASH_WATER = 0x060400FA,
    WORLD_TERRAIN_MASK_SHADOW_ELIGIBLE = 0x311B6F05
};
#endif

/* include/ff7/enums/coaster.h: named codes of ff7/coaster (the Gold Saucer roller-coaster shooting
 * minigame). Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_COASTER_H
#define FF7_ENUMS_COASTER_H

/* CoasterObjectState.type, the behaviour Mini_CoasterUpdateActors runs for an actor each frame.
 * The spawn script (CoasterSpawnScriptRecord.behaviourType) picks the scripted ones; the others are
 * effects the update spawns itself. Path behaviours follow the actor's path from its pathIndex. */
enum CoasterBehaviour {
    COASTER_BEHAVIOUR_PATH_SPIN = 0x00,         /* follows its path, spinning by params[3..5] */
    COASTER_BEHAVIOUR_PATH_FACING = 0x01,       /* follows its path, facing along it */
    COASTER_BEHAVIOUR_PATH_SWAY = 0x02,         /* follows its path, pitching toward a random angle */
    COASTER_BEHAVIOUR_ABOVE_CAMERA = 0x03,      /* stays 2500 above the camera (spawned by RIDE_START) */
    COASTER_BEHAVIOUR_FALL = 0x04,              /* falls ever faster once the track passes params[3] */
    COASTER_BEHAVIOUR_PATH_TUMBLE = 0x05,       /* follows its path, turning by params[6..8] from params[3..5] */
    COASTER_BEHAVIOUR_TILT = 0x07,              /* holds its path's start, pitches by params[6] once the track passes params[5] */
    COASTER_BEHAVIOUR_ROCKET = 0x08,            /* rises, slowing, and bursts into DEBRIS at the top */
    COASTER_BEHAVIOUR_DEBRIS = 0x09,            /* flies off at a random velocity for 100 frames */
    COASTER_BEHAVIOUR_PLAYER_CAR = 0x0A,        /* the player's car, on the track ahead of the camera */
    COASTER_BEHAVIOUR_DEBRIS_BURST = 0x0B,      /* spawns params[3] FALLING_DEBRIS and goes */
    COASTER_BEHAVIOUR_FALLING_DEBRIS = 0x0C,    /* DEBRIS under gravity */
    COASTER_BEHAVIOUR_TILT_ALT = 0x0D,          /* runs as COASTER_BEHAVIOUR_TILT */
    COASTER_BEHAVIOUR_LEAP = 0x0E,              /* leaps up and falls back, throwing LEAP_SPRAY and a LEAP_TRAIL */
    COASTER_BEHAVIOUR_LEAP_SPRAY = 0x0F,        /* random drops under gravity for 200 frames */
    COASTER_BEHAVIOUR_LEAP_TRAIL = 0x10,        /* rises and falls back after 20 frames */
    COASTER_BEHAVIOUR_PATH_DELAYED = 0x11,      /* follows its path once the track passes params[3] */
    COASTER_BEHAVIOUR_HOMING = 0x64,            /* closes on the car over 0x80 frames; reaching it costs 5 points */
    COASTER_BEHAVIOUR_SPARK_EMITTER = 0xC9,     /* emits a HIT_SPARK each frame for 20 frames */
    COASTER_BEHAVIOUR_HIT_SPARK = 0xCA,         /* the spark of a beam hit */
    COASTER_BEHAVIOUR_EXPLOSION_DEBRIS = 0xCB,  /* the debris of a destroyed target */
    COASTER_BEHAVIOUR_STATIC_TARGET = 0xE6,     /* stays put until the track passes params[2] */
    COASTER_BEHAVIOUR_SCORE_CHECK = 0xFA,       /* spawns a BRAKE when the score is below params[0] */
    COASTER_BEHAVIOUR_RIDE_START = 0xFC,        /* fades in, then sets the ride going */
    COASTER_BEHAVIOUR_RIDE_END = 0xFD,          /* fades out and finishes the ride */
    COASTER_BEHAVIOUR_STOP_AND_GO = 0xFE,       /* stops the car, then speeds it up after params[0] / 60 seconds */
    COASTER_BEHAVIOUR_BRAKE = 0xFF              /* slows the car by params[0]; at a standstill spawns RIDE_END */
};

/* CoasterKillAward.killType (CoasterObjectState.params[10]): what Mini_CoasterAwardKill does when the
 * beam destroys the actor. Every kind adds its score. */
enum CoasterKillType {
    COASTER_KILL_SPARKS = 1,           /* three HIT_SPARKs, the score popup */
    COASTER_KILL_EXPLOSION = 2,        /* three EXPLOSION_DEBRIS, the score popup */
    COASTER_KILL_SCORE_ONLY = 3,       /* not destroyed: adds bonus to its own score */
    COASTER_KILL_NO_EFFECT = 4,        /* the score popup only */
    COASTER_KILL_BIG_EXPLOSION = 5     /* 100 EXPLOSION_DEBRIS, the score popup */
};

#endif

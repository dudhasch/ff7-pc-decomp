#ifndef FF7_CAMERA_SCRIPT_H
#define FF7_CAMERA_SCRIPT_H

typedef struct CameraScriptVector {
    /* Eight-byte SVECTOR copies include the otherwise unused final short. */
    union {
        struct {
            short x, y, z, pad;
        };
        struct {
            int xyWord;
            int zWord;
        };
    };
} CameraScriptVector;

typedef struct CameraScriptTrack {
    short mode, frames; /* +0x00: command and remaining frames */
    int distance, delta; /* +0x04: radial distance or fixed-point progress */
    short actor, bone; /* +0x0c: optional attachment; -1 means unattached */
    CameraScriptVector position, base, velocity; /* +0x10, +0x18, +0x20 */
    short easing, pad; /* +0x28 */
} CameraScriptTrack;

typedef struct CameraScriptTask {
    short active, initialized;
    int unused4, unused8;
    short *cursor; /* +0x0c */
    union { int matrix; void *matrixPtr; }; /* +0x10 */
    short actor;
    char tail[10];
} CameraScriptTask;

#endif

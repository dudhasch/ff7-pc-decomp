/* SPDX-License-Identifier: Apache-2.0 */
#ifndef FF7_WINDOW_H
#define FF7_WINDOW_H

/*
 * Message-window state record (0x30 bytes per slot; 4-slot table g_FieldWindows
 * at 0x00CFF5B8 shared by field, world map, and menu window rendering).
 *
 * Note: g_FieldWindows[0].state is at 0x00CFF5E4 inside the 0x00CFF5B8 array
 * extent and must not be given a separate GLOBAL annotation.
 */
typedef struct WindowData {
    unsigned char *text;            /* +0x00: active window text buffer */
    short x;                        /* +0x04: window origin X (320x240 space) */
    short y;                        /* +0x06: window origin Y (320x240 space) */
    short width;                    /* +0x08: target full width */
    short height;                   /* +0x0A: target full height */
    short currentWidth;             /* +0x0C: animated open/close width */
    short currentHeight;            /* +0x0E: animated open/close height */
    short textScrolling;            /* +0x10: vertical pixel scroll offset */
    short stringLength;             /* +0x12: revealed glyph count */
    short stringByteLength;         /* +0x14: decoded text byte cursor / last column */
    short currentRow;               /* +0x16: active text row / last rendered row */
    unsigned char isFull;           /* +0x18: non-zero when rendered text overflows height */
    unsigned char style;            /* +0x19: frame/transparency/style bitflags */
    unsigned char pointerEnabled;   /* +0x1A: choice cursor visibility flag */
    unsigned char numDisplayType;   /* +0x1B: addon widget kind (none, countdown, number) */
    unsigned char numDisplayFlags;  /* +0x1C: number widget formatting flags */
    unsigned char numDisplayLength; /* +0x1D: number widget digit count */
    short pad_1e;                   /* +0x1E: 4-byte alignment padding */
    int numDisplayValue;            /* +0x20: 32-bit numeric widget value */
    short pointerX;                 /* +0x24: choice cursor X offset */
    short pointerY;                 /* +0x26: choice cursor Y offset */
    short numDisplayX;              /* +0x28: numeric/countdown widget X offset */
    short numDisplayY;              /* +0x2A: numeric/countdown widget Y offset */
    short state;                    /* +0x2C: MSG_WINDOW_* lifecycle state */
    unsigned short preventClose;    /* +0x2E: close-lock and instant-text mode flags */
} WindowData;

#endif /* FF7_WINDOW_H */

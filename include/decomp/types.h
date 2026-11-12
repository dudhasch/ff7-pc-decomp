/* Fixed-width types and fixed-point helpers, MSVC 4.x-safe.
 *
 * No <stdint.h> -- that is C99 and this compiler predates it. No long long
 * either. Keep every declaration inside what a 1996 compiler accepts. */

#ifndef FF7_DECOMP_TYPES_H
#define FF7_DECOMP_TYPES_H

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;

typedef int bool32;

#define FF7_TRUE 1
#define FF7_FALSE 0

/* 4.12 fixed point: 4096 == 1.0. The engine is a PSX port and carries the
 * software GTE's conventions throughout.
 *
 * CAUTION: 4096 also means 360 degrees for the battle and world GTE paths, but
 * FIELD ENTITY FACING IS A BYTE ANGLE -- 256 units to a full turn, so
 * degrees = heading * 1.40625. The two units are NOT interchangeable and
 * unifying them is a documented way to get facing wrong by 90 degrees. */
typedef s32 fixed4_12;

#define FF7_ONE 4096
#define FF7_FROM_INT(n) ((fixed4_12)((n) * FF7_ONE))

/* MSVC of this era emits this exact sequence for a signed >> 12 divide.
 * Reproducing it matters for matching. */
#define FF7_TO_INT(v) (((v) + (((v) >> 31) & 0xFFF)) >> 12)

#endif /* FF7_DECOMP_TYPES_H */

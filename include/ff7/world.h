/* World-map structures.
 *
 * EVERY OFFSET AND WIDTH IN THIS FILE WAS READ OUT OF ff7.exe, by the same
 * method as include/ff7/battle.h: the base is the absolute address in the
 * encoded instruction, the member offset is the displacement, the width is the
 * operand size, and the stride is the `imul reg, reg, N` that scales an index.
 *
 * WHERE THE NAMES COME FROM
 * -------------------------
 * The field NAMES of WorldActor -- and only the names -- follow the
 * PlayStation decompilation of FF7 (see Credits in the README). They come from
 * a different binary on a different architecture; what makes them usable here
 * is that the PC image agrees about the shape. Every one of the thirty
 * distinct field accesses reached through the two world-actor pointer globals
 * lands on a PlayStation field at the same offset, with the same width, and
 * not one PC access contradicts that layout:
 *
 *     PC access                       PSX field
 *     [reg+0x00] dword  x14           WorldActor *next
 *     [reg+0x04] dword  x4            WorldActor *collide
 *     [reg+0x08] dword  x17           WorldActor *riding
 *     [reg+0x0C..0x14] dword x64      VECTOR pos
 *     [reg+0x1C..0x24] dword x20      VECTOR altPos
 *     [reg+0x40] word signed x15      s16 direction
 *     [reg+0x50] byte   x23           u8 actorType
 *     [reg+0x51] byte   x66           u8 flags1
 *     [reg+0x5C] byte signed x2       s8 verticalSpeed
 *     [reg+0xB8] dword  x4            inside the PSX shadow block (+0x90..+0xDF)
 *
 * That is agreement of a layout, not a transcription of code. Members the
 * PlayStation version leaves unnamed are named here from what the PC code does
 * with them.
 *
 * The `pad_*` members are NOT claims. They are unexamined bytes. A field
 * discovered later replaces padding; it never shifts a derived offset.
 */

#ifndef FF7_WORLD_H
#define FF7_WORLD_H

#include "ff7/window.h"
#include "lib/types/graphics.h"

#include <stddef.h>

/* The 16-byte stream-focus value copied by World_GetStreamFocus. */
typedef struct WorldStreamFocusData {
    int x, y, z, w;
} WorldStreamFocusData;

/* One saved script of a world actor: WorldScript_Request pushes the running script's position,
 * wait and priority here when a script of higher priority interrupts it, and
 * WorldScript_ExecControlFlow's return pops it. */
typedef struct WorldScriptCallFrame {
    /* +0x00 */ short scriptIdx;
    /* +0x02 */ unsigned char waitFrames;
    /* +0x03 */ unsigned char scriptPriority;
} WorldScriptCallFrame;

/* ------------------------------------------------------------------------
 * WorldActor -- an entity on the world map. 0xC0 bytes on PC: the pool stride and
 * WorldEntity_Clear use 0xC0 (the PSX record is 0xE0).
 *
 * Two pointers into this array are the world module's working set:
 *
 *   0x00E39AD8  the actor the script engine is currently running
 *   0x00E3A7D0  the player's actor
 *
 * The size is the PlayStation record's; the PC image reaches +0xB8, so the
 * tail is at least that long, and nothing here depends on the exact extent.
 * ---------------------------------------------------------------------- */
/* Triangle cache slot keyed by chunk (x, y); cleared when that terrain chunk unloads. */
typedef struct WorldTriangleCacheSlot {
    int triangle;
    short chunkX;
    short chunkY;
} WorldTriangleCacheSlot;

typedef struct WorldActor {
    /* +0x00 */ struct WorldActor *next; /* 0x0075b8f3, [eax+0]              */
    /* +0x04 */ struct WorldActor *collide;
    /* +0x08 */ struct WorldActor *riding;
    /* +0x0C */ VECTOR pos; /* 0x00761f33, add ecx,[eax+0x10]           */
    /* +0x1C */ VECTOR altPos;
    /* +0x2C */ WorldScriptCallFrame callStack[4]; /* scriptCallDepth entries in use */
    /* +0x3C */ short heading; /* 0x00761ec8, movsx ecx,[eax+0x3c] -- SIGNED,
                                where the PSX header says u16; stepped toward a
                                target facing, 0..0x1000 (WorldEntity_StepFacingToward) */
    /* +0x3E */ short headingOffset; /* eased toward the terrain probe's angle; heading +
                                        headingOffset is the direction drawn and reported */
    /* +0x40 */ short direction;
    /* +0x42 */ short groundY; /* the ground height World_SampleGroundAtEntity writes */
    /* +0x44 */ short yOffset;
    /* +0x46 */ short scriptIdx;
    /* +0x48 */ short curvedGroundY; /* groundY less the world's curvature */
    /* +0x4A */ short walkmesh;
    /* +0x4C */ short facing;
    /* +0x4E */ short unusedAttr4E;
    /* +0x50 */ unsigned char actorType; /* 0x00761749, mov cl,[eax+0x50]    */
    /* +0x51 */ unsigned char flags1; /* 0x00762944, and cl,0xfd             */
    /* +0x52 */ unsigned char scriptCallModel;
    /* +0x53 */ signed char animFrame; /* the frame of the current animation, read as unsigned */
    /* +0x54 */ unsigned char scriptCallDepth;
    /* +0x55 */ unsigned char horizontalSpeed;
    /* +0x56 */ unsigned char waitFrames;
    /* +0x57 */ unsigned char scriptPriority;
    /* +0x58 */ unsigned char animStepState;
    /* +0x59 */ signed char animStepAccum;
    /* +0x5A */ signed char pad_5a[2];
    /* +0x5C */ signed char verticalSpeed;
    /* +0x5D */ signed char animId;
    /* +0x5E */ signed char effect; /* the model's effect id, cleared every frame */
    /* +0x5F */ signed char altitudeSpeed;
    /* +0x60 */ WorldTriangleCacheSlot triangleCache[6]; /* 6 cached terrain triangles (World_SampleGroundAtEntity) */
    /* +0x90 */ unsigned char shadowPoly[0x28];          /* PSX: POLY_FT4 shadow quad packet (World_DrawActorShadow) */
    /* +0xB8 */ void *renderObject; /* 0x00766b8a, mov eax,[ecx+0xb8]; the shadow or map render object */
    /* +0xBC */ char pad_bc[0x4];
} WorldActor;

/* ------------------------------------------------------------------------
 * Compile-time offset assertions. See include/ff7/battle.h for why a negative
 * array size is the assert a 1996 compiler understands.
 * ---------------------------------------------------------------------- */
#define FF7_ASSERT_WORLD_OFFSET(tag, type, member, expect) \
    typedef char ff7_woff_##tag[(offsetof(type, member) == (expect)) ? 1 : -1]

FF7_ASSERT_WORLD_OFFSET(act_next, WorldActor, next, 0x00);
FF7_ASSERT_WORLD_OFFSET(act_collide, WorldActor, collide, 0x04);
FF7_ASSERT_WORLD_OFFSET(act_riding, WorldActor, riding, 0x08);
FF7_ASSERT_WORLD_OFFSET(act_pos, WorldActor, pos, 0x0C);
FF7_ASSERT_WORLD_OFFSET(act_altpos, WorldActor, altPos, 0x1C);
FF7_ASSERT_WORLD_OFFSET(act_calls, WorldActor, callStack, 0x2C);
FF7_ASSERT_WORLD_OFFSET(act_dir, WorldActor, direction, 0x40);
FF7_ASSERT_WORLD_OFFSET(act_yoff, WorldActor, yOffset, 0x44);
FF7_ASSERT_WORLD_OFFSET(act_script, WorldActor, scriptIdx, 0x46);
FF7_ASSERT_WORLD_OFFSET(act_walkmesh, WorldActor, walkmesh, 0x4A);
FF7_ASSERT_WORLD_OFFSET(act_facing, WorldActor, facing, 0x4C);
FF7_ASSERT_WORLD_OFFSET(act_type, WorldActor, actorType, 0x50);
FF7_ASSERT_WORLD_OFFSET(act_flags1, WorldActor, flags1, 0x51);
FF7_ASSERT_WORLD_OFFSET(act_depth, WorldActor, scriptCallDepth, 0x54);
FF7_ASSERT_WORLD_OFFSET(act_hspeed, WorldActor, horizontalSpeed, 0x55);
FF7_ASSERT_WORLD_OFFSET(act_wait, WorldActor, waitFrames, 0x56);
FF7_ASSERT_WORLD_OFFSET(act_prio, WorldActor, scriptPriority, 0x57);
FF7_ASSERT_WORLD_OFFSET(act_vspeed, WorldActor, verticalSpeed, 0x5C);
FF7_ASSERT_WORLD_OFFSET(act_anim, WorldActor, animId, 0x5D);
FF7_ASSERT_WORLD_OFFSET(act_renderobj, WorldActor, renderObject, 0xB8);
FF7_ASSERT_WORLD_OFFSET(act_size, WorldActor, pad_bc, 0xBC);

/* The world module's offsets into the message-window record at 0x00CFF5B8, which
 * it shares with the field (`WindowData`, include/ff7/window.h). */
FF7_ASSERT_WORLD_OFFSET(win_x2, WindowData, stringByteLength, 0x14);
FF7_ASSERT_WORLD_OFFSET(win_style, WindowData, style, 0x19);
FF7_ASSERT_WORLD_OFFSET(win_num, WindowData, numDisplayValue, 0x20);
FF7_ASSERT_WORLD_OFFSET(win_close, WindowData, preventClose, 0x2E);

#endif /* FF7_WORLD_H */

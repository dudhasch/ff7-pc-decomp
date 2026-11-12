// src/ff7/field/src/field_request_and_sound.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Points the field's request and effect pointers at their static blocks and posts the
//   door-trigger sound command; the remaining functions are empty placeholders.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdlib.h>
#include "ff7/types/field.h"
#include "lib/pfile.h"
#include "ff7/field.h"
#include "ff7/api/field.h"
#include "lib/api/sound.h"
#include "lib/enums/sound.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00cfff18
FieldRequest * g_pFieldRequest = 0;
unsigned char g_bssPad_00cfff1c[4] = {0};
unsigned char g_bssPad_00cfff20[24] = {0};
// clang-format off
// clang-format on
// GLOBAL: FF7 0x00cfff38
struct FieldRequest g_FieldRequestBuffer = {0};
unsigned char g_bssPad_00cfff54[4] = {0};
unsigned char g_bssPad_00cfff58[8] = {0};
// GLOBAL: FF7 0x00cfff60
int g_FieldFxBuffer = 0;
unsigned char g_bssPad_00cfff64[4] = {0};
unsigned char g_bssPad_00cfff68[168] = {0};
// clang-format on

/*
 * Points g_pFieldRequest at the static request block at g_FieldRequestBuffer.
 */
// FUNCTION: FF7 0x00646310
void __cdecl Field_BindRequestBlock(void)
{
    g_pFieldRequest = &g_FieldRequestBuffer;
}

/*
 * Points g_pFieldFx at the static block at g_FieldFxBuffer.
 */
// FUNCTION: FF7 0x0064631f
void __cdecl Field_BindFxBlock(void)
{
    *(int *)&g_pFieldFx = (int)&g_FieldFxBuffer;
}

/*
 * Empty placeholder.
 */
// FUNCTION: FF7 0x0064632e
void __cdecl Field_UnusedNoOp_6(void)
{
}

/*
 * Empty placeholder.
 */
// FUNCTION: FF7 0x00646333
void __cdecl Field_UnusedNoOp_7(void)
{
}

/*
 * Posts sound command 0xF0 with no arguments through SoundDispatchCommand, but only while
 * g_FieldReloadPending is set and g_FieldLogicUnusedB78 is clear.
 */
// FUNCTION: FF7 0x00646338
void __cdecl Field_PostSoundCommandF0(void)
{
    if (g_FieldReloadPending != 0 && g_FieldLogicUnusedB78 == 0) {
        g_pFieldRequest->code = SOUND_MUSIC_STOP;
        SoundDispatchCommand(g_pFieldRequest->code, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}

/*
 * While g_FieldReloadPending is nonzero, fills the request with code 0x20, args[0] = 0x40 and
 * args[1] = soundId, and dispatches it with six zero arguments. Does not clear the request first.
 */
// FUNCTION: FF7 0x0064637b
void __cdecl Field_PlayDoorTriggerSoundEffect(unsigned short soundId)
{
    if (g_FieldReloadPending != 0) {
        g_pFieldRequest->code = SOUND_SFX_PLAY_1;
        g_pFieldRequest->args[0] = 0x40;
        g_pFieldRequest->args[1] = soundId;
        SoundDispatchCommand(g_pFieldRequest->code, g_pFieldRequest->args[0],
                             g_pFieldRequest->args[1], 0, 0, 0, 0, 0, 0);
    }
}
} /* extern "C" */

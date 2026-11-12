// src/ff7/coaster/coaster.cpp
// Original: C:\FF7\coaster\coaster.cpp
// Contents: Roller coaster minigame data and audio: loads the archive, models, actors and starting
//   ride state, releases sound and PSX data, and runs the music and beam and engine loop sounds.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "lib/api/sound.h"
#include "lib/enums/sound.h"
#include "lib/enums/sound.h"
#include "lib/types/graphics.h"
#include "lib/crt.h"
#include <time.h>
#include "lib/api/graphics.h"
#include "ff7/enums/main.h"
#include "ff7/api/main.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"

extern "C" {
// clang-format off
// GLOBAL: FF7 0x00901410
extern "C" int g_CoasterVolumeRight = 127;
// GLOBAL: FF7 0x00901414
extern "C" int g_CoasterVolumeLeft = 127;
// The sound command buffer, g_CoasterSoundCommandBuffer (this TU's .bss fill).
// GLOBAL: FF7 0x00901418
extern "C" CoasterSoundCommand *g_pCoasterSoundBuffer = g_CoasterSoundCommandBuffer;
// GLOBAL: FF7 0x00901420
int g_CoasterSfxTable[23] = {10, 24, 45, 69, 118, 133, 142, 152, 191, 263, 269, 270, 271, 286, 358, 375, 405, 539, 555, 557, 565, 566, 673};
// GLOBAL: FF7 0x0090147c
int g_coasterSubVolumeFactor = 2;
// GLOBAL: FF7 0x00901480
int g_CoasterDisplayScaleY = 2;
// Uninitialized .bss symbols DAT_00c3f6f0..DAT_00c3f738 keep their retail names: VC5 /Tp orders .bss by symbol-name hash.
// GLOBAL: FF7 0x00c3f6f0
char DAT_00c3f6f0;
// GLOBAL: FF7 0x00c3f6f4
short DAT_00c3f6f4;
// GLOBAL: FF7 0x00c3f700
struct CoasterSoundCommand DAT_00c3f700[3];
// GLOBAL: FF7 0x00c3f724
short DAT_00c3f724;
// GLOBAL: FF7 0x00c3f728
struct CoasterScorePopupState DAT_00c3f728;
// GLOBAL: FF7 0x00c3f730
char g_coasterFreezeState;
// GLOBAL: FF7 0x00c3f74c
int g_CoasterScore = 0;
// GLOBAL: FF7 0x00c3f750
int g_CoasterFadeNearDepth = 0;
// GLOBAL: FF7 0x00c3f754
int g_CoasterFadeFarDepth = 0;
// GLOBAL: FF7 0x00c3f758
int g_CoasterSoundState = 0;
// GLOBAL: FF7 0x00c3f75c
int g_CoasterTrackCursor = 0;
// GLOBAL: FF7 0x00c3f760
char g_CoasterPaused = 0;
// GLOBAL: FF7 0x00c3f764
char g_CoasterRideActive = 0;
// GLOBAL: FF7 0x00c3f768
int g_CoasterRideSpeed = 0;
// GLOBAL: FF7 0x00c3f76c
short g_CoasterKillScore = 0;
// GLOBAL: FF7 0x00c3f770
int g_CoasterBeamLoopSoundActive = 0;
// GLOBAL: FF7 0x00c3f774
char g_CoasterRideFinished = 0;
// GLOBAL: FF7 0x00c3f778
int g_CoasterRidePosition = 0;
// GLOBAL: FF7 0x00c3f77c
int g_CoasterEngineVolume = 0;
// clang-format on


// The coaster object's file-scope globals start its default code section. VC5 emits their dynamic
// initializers here in definition order; .CRT$XCU slots 0x007ba024 and 0x007ba028 point to
// $CRT_CPP_Initializer (0x005e9150) and $CRT_CPP_Initializer (0x005e9199). The compiler-generated
// $E functions carry no FUNCTION markers. Constructed by
// 0x005e9150 -> $E1 (0x005e915f); $E3 (0x005e9178) registers at-exit thunk $E2 (0x005e918a), which
// runs ~CoasterPsxDataSet (CoasterPsxDataSet::~CoasterPsxDataSet, 0x005ee730).
// GLOBAL: FF7 0x00c3f738
CoasterPsxDataSet DAT_00c3f738;

// Crosshair size: 32 times the display scale, set by $CRT_CPP_Initializer (0x005e9199) -> $E6
// (0x005e91a3).
// GLOBAL: FF7 0x00c3f6f8
int DAT_00c3f6f8 = g_coasterSubVolumeFactor * 32;
// GLOBAL: FF7 0x00c3f780
int g_CoasterBeamLoopVolume = 0;
// GLOBAL: FF7 0x00c3f784
int g_CoasterClipLeft = 0;

/*
 * Loads the ride's data and sets the start-of-ride state; called once from `Mini_CoasterInit`.
 * Clears the pause flag and opens the data: coaster.lgp as LGP slot 9, xbinadr.bin and xbin.bin,
 * and the 23 ride sounds (g_CoasterSoundState is 0 while that runs and 0x99 after). Resets the
 * rail and scenery stream lists, the scene-node pool and the model pools, resolves the track and
 * rail-set tables (which also leaves the debug tuning mode), resets the actor pool and builds the
 * two clip planes, then builds the 100 models 0..99 from their templates into g_CoasterModelHandles.
 * Starts the ride at track position 0 with speed 0x2710 (10000) and score 0; sets the depth fade
 * range g_CoasterFadeNearDepth..2 to 10410..14300, which despite the names is the distance over
 * which `CoasterRenderer::depthFactor` fades the scenery out (14300 is the far plane); clears the
 * fade and end flags, the score popup (model, rotation, timer and blink state), the alternating
 * SFX channel, and sets both pan volumes to 0x7F and the engine and beam loop volumes to 0.
 */
// FUNCTION: FF7 0x005e91b5
void __cdecl Mini_CoasterLoadData(void)
{
    int i;

    g_CoasterSoundState = 0;
    g_CoasterPaused = 0;
    Mini_CoasterOpenArchive();
    g_CoasterSoundState = 0x99;
    Mini_CoasterResetStreamLists();
    Mini_CoasterResetSceneNodePool();
    Mini_CoasterInitModelPools();
    Mini_CoasterResolveTrackTables();
    Mini_CoasterResetActorPool();
    Mini_CoasterBuildClipPlanes();
    for (i = 0; i < 100; i++) {
        g_CoasterModelHandles[i] = Mini_CoasterBuildModelFromTemplate(i);
    }
    g_CoasterTrackCursor = 0;
    g_CoasterRidePosition = 0;
    g_CoasterRideSpeed = 10000;
    g_CoasterScore = 0;
    g_CoasterFadeNearDepth = 0x28AA;
    g_CoasterFadeFarDepth = 0x37DC;
    g_CoasterRideActive = 0;
    g_CoasterRideFinished = 0;
    g_CoasterKillScore = 0;
    g_coasterFreezeState = 0;
    g_CoasterScorePopupState.coasterScorePopupVx = 0;
    g_CoasterScorePopupState.coasterScorePopupVy = 0;
    g_CoasterScorePopupState.coasterScorePopupVz = 0;
    g_CoasterSfxAlternateToggle = 0;
    g_CoasterVolumeRight = 0x7F;
    g_CoasterVolumeLeft = 0x7F;
    g_CoasterEngineVolume = 0;
    g_CoasterBeamLoopVolume = 0;
    g_CoasterScorePopupTimer = 0;
}

/*
 * Releases cached sound effect buffers and frees coaster PSX data sets.
 */
// FUNCTION: FF7 0x005e92d8
void __cdecl Mini_CoasterReleaseSfxAndPsxData(void)
{
    int i;

    for (i = 22; i >= 0; i--) {
        Sfx_ReleaseCachedBuffer(g_CoasterSfxTable[i], 0xf);
    }
    Mini_CoasterFreePsxDataSet(&g_CoasterPsxDataSet);
}

/*
 * Opens the coaster archive file for reading.
 */
// FUNCTION: FF7 0x005e9319
void __cdecl Mini_CoasterOpenArchive(void)
{
    GfxEngineContext *engine;
    int i;
    engine = GetEngineContext();
    ChangeDirectorySaving(GetMinigameDirForCoaster(), 0);
    // STRING: FF7 0x00901484
    OpenLgpArchive("coaster.lgp", LGP_ARCHIVE_COASTER);
    // STRING: FF7 0x0090149c
    g_CoasterPsxDataSet.Mini_CoasterLoadPsxDataSet("xbinadr.bin",
                                            // STRING: FF7 0x00901490
                                            "xbin.bin");
    for (i = 22; i >= 0; i--)
        SfxLoadIntoBuffer(g_CoasterSfxTable[i], 0);
}

/*
 * Starts coaster background music and initiates engine sound loop.
 */
// FUNCTION: FF7 0x005e938d
void __cdecl Mini_CoasterStartMusic(void)
{
    MidiPlaySong(0x37, 0, 0x7F);
    g_pCoasterSoundBuffer->soundId = SOUND_SFX_CH3_VOLUME;
    g_pCoasterSoundBuffer->soundParam = 0;
    SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0, 0, 0, 0);
    g_pCoasterSoundBuffer->soundId = SOUND_SFX_PLAY_CH3;
    g_pCoasterSoundBuffer->soundParam = 0x40;
    g_pCoasterSoundBuffer->soundParam2 = 0x177;
    SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, g_pCoasterSoundBuffer->soundParam2, 0, 0, 0, 0, 0, 0);
}

/*
 * Fades and stops the coaster sound loops.
 */
// FUNCTION: FF7 0x005e9436
void __cdecl Mini_CoasterStopSoundLoops(void)
{
    g_pCoasterSoundBuffer->soundId = SOUND_MUSIC_VOLUME_TRANS;
    g_pCoasterSoundBuffer->soundParam = 0xF0;
    g_pCoasterSoundBuffer->soundParam2 = 0;
    SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, g_pCoasterSoundBuffer->soundParam2, 0, 0, 0, 0, 0, 0);
    g_pCoasterSoundBuffer->soundId = SOUND_SFX_ALL_VOLUME_TRANS;
    g_pCoasterSoundBuffer->soundParam = 0xF0;
    g_pCoasterSoundBuffer->soundParam2 = 0;
    SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, g_pCoasterSoundBuffer->soundParam2, 0, 0, 0, 0, 0, 0);
}

/*
 * Plays a coaster sound effect alternating between stereo playback channels.
 */
// FUNCTION: FF7 0x005e94e5
void __cdecl Mini_CoasterPlaySfxAlternating(short sfxId)
{
    g_CoasterSfxAlternateToggle++;
    g_CoasterSfxAlternateToggle %= 2;
    if (g_CoasterSfxAlternateToggle == 0) {
        g_pCoasterSoundBuffer->soundId = SOUND_SFX_CH1_TRANSPOSE;
        g_pCoasterSoundBuffer->soundParam = 0;
        SoundDispatchCommand(g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0, 0, 0, 0);

        g_pCoasterSoundBuffer->soundId = SOUND_SFX_PLAY_CH1;
        g_pCoasterSoundBuffer->soundParam = 0x40;
        g_pCoasterSoundBuffer->soundParam2 = sfxId;
        SoundDispatchCommand(g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, g_pCoasterSoundBuffer->soundParam2, 0, 0, 0, 0, 0, 0);

        g_pCoasterSoundBuffer->soundId = SOUND_SFX_CH1_VOLUME;
        g_pCoasterSoundBuffer->soundParam = 0x7F;
        SoundDispatchCommand(g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_CoasterSfxAlternateToggle == 1) {
        g_pCoasterSoundBuffer->soundId = SOUND_SFX_CH2_TRANSPOSE;
        g_pCoasterSoundBuffer->soundParam = 0;
        SoundDispatchCommand(g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0, 0, 0, 0);

        g_pCoasterSoundBuffer->soundId = SOUND_SFX_PLAY_CH2;
        g_pCoasterSoundBuffer->soundParam = 0x40;
        g_pCoasterSoundBuffer->soundParam2 = sfxId;
        SoundDispatchCommand(g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, g_pCoasterSoundBuffer->soundParam2, 0, 0, 0, 0, 0, 0);

        g_pCoasterSoundBuffer->soundId = SOUND_SFX_CH2_VOLUME;
        g_pCoasterSoundBuffer->soundParam = 0x7F;
        SoundDispatchCommand(g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0, 0, 0, 0);
    }
}

/*
 * Updates the beam-firing loop sound pitch and volume state.
 */
// FUNCTION: FF7 0x005e96da
void __cdecl Mini_CoasterUpdateBeamLoopSound(short volume)
{
    if (g_CoasterBeamLoopSoundActive == 0 && volume > 0) {
        g_pCoasterSoundBuffer->soundId = SOUND_SFX_PLAY_CH4;
        g_pCoasterSoundBuffer->soundParam = 0x40;
        g_pCoasterSoundBuffer->soundParam2 = 0x22B;
        SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, g_pCoasterSoundBuffer->soundParam2, 0, 0, 0, 0, 0, 0);
    }
    if (volume > 0) {
        g_CoasterBeamLoopVolume = volume;
        g_pCoasterSoundBuffer->soundId = SOUND_SFX_CH4_TRANSPOSE;
        g_pCoasterSoundBuffer->soundParam = volume;
        SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0, 0, 0, 0);
        g_CoasterBeamLoopSoundActive = volume;
    } else {
        g_pCoasterSoundBuffer->soundId = SOUND_SFX_PLAY_CH4;
        g_pCoasterSoundBuffer->soundParam = 0x40;
        g_pCoasterSoundBuffer->soundParam2 = 0;
        SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, g_pCoasterSoundBuffer->soundParam2, 0, 0, 0, 0, 0, 0);
        g_CoasterBeamLoopSoundActive = 0;
    }
}

/*
 * Pushes engine and beam loop volumes to the sound driver.
 */
// FUNCTION: FF7 0x005e9802
void __cdecl Mini_CoasterUpdateLoopVolumes(void)
{
    g_pCoasterSoundBuffer->soundId = SOUND_SFX_CH3_VOLUME;
    g_pCoasterSoundBuffer->soundParam = g_CoasterEngineVolume;
    SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0, 0, 0, 0);
    g_pCoasterSoundBuffer->soundId = SOUND_SFX_CH4_VOLUME;
    g_pCoasterSoundBuffer->soundParam = g_CoasterBeamLoopVolume;
    SoundDispatchCommand((short)g_pCoasterSoundBuffer->soundId, g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0, 0, 0, 0);
}

/*
 * Stops coaster background music playback.
 */
// FUNCTION: FF7 0x005e988b
void __cdecl Mini_CoasterStopMusic(void)
{
    MidiStop();
    g_pCoasterSoundBuffer->soundId = SOUND_SFX_ALL_VOLUME;
    g_pCoasterSoundBuffer->soundParam = 0;
    SoundDispatchCommand(g_pCoasterSoundBuffer->soundId,
                         g_pCoasterSoundBuffer->soundParam, 0, 0, 0, 0,
                         0, 0, 0);
}
} /* extern "C" */

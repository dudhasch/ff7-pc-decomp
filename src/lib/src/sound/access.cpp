// src/lib/src/sound/access.cpp
// Original: ACCESS.CPP (assert text "ACCESS.CPP::SOUND_SIDE_SWITCH"; no directory in the string,
// placed beside midi1.cpp, which it precedes in link order)

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/enums/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(abs)
#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00ddd6a8
int g_SoundMusicUsesMidi = 0;
unsigned char g_bssPad_00ddd6ac[4] = {0};
// clang-format on

/*
 * Dispatches the low sixteen command bits to wave/MIDI music and sound-effect controls: track
 * playback/crossfade, pause/resume/stop, volume/tempo changes, up to four effects, channel-5
 * preload playback and per-channel or global volume/pan/transpose transitions. Track 0 stops both
 * music backends but returns 0; valid tracks 1 through 98 select the wave or MIDI backend and
 * return 1. Most handled commands return 1, but music fade-volume and tempo commands retain return
 * 0 despite performing work; unsupported commands and the diagnostic-only side-switch command also
 * return 0. Preserves each command-specific byte truncation and parameter order. Arguments 7
 * through 9 are unused.
 */
// FUNCTION: FF7 0x00740d80
int __cdecl SoundDispatchCommand(short command, int param1, int param2, int param3, int param4, int param5, int reserved7, int reserved8, int reserved9)
{
    int ret;
    int songId;

    ret = 0;
    switch (*(int *)&command & 0xFFFF) {
    case SOUND_MUSIC_PLAY:
    case SOUND_MUSIC_PLAY_ALT:
        songId = param1;
        if (songId == 0) {
            MusicWaveStop();
            MidiStop();
        }
        if (songId <= 0 || songId > SOUND_SONG_LAST) {
            break;
        }
        g_SoundMusicUsesMidi = MusicTrackUsesMidi(songId);
        switch (g_SoundMusicUsesMidi) {
        case SOUND_MUSIC_BACKEND_NONE:
            MidiStop();
            MusicPlayWaveTrack(songId, 0);
            break;
        case SOUND_MUSIC_BACKEND_MIDI:
            MusicWaveStop();
            MidiPlaySong(songId, 0, 0);
        }
        ret = 1;
        break;
    case SOUND_SIDE_SWITCH:
        // STRING: FF7 0x00966b14
        Gfx_DebugPrint("!!!!SADNESS!!!!  ACCESS.CPP::SOUND_SIDE_SWITCH");
        break;
    case SOUND_MUSIC_CROSSFADE:
    case SOUND_MUSIC_CROSSFADE_ALT:
        songId = param1;
        if (songId == 0) {
            MusicWaveStop();
            MidiStop();
        }
        if (songId <= 0 || songId > SOUND_SONG_LAST) {
            break;
        }
        g_SoundMusicUsesMidi = MusicTrackUsesMidi(songId);
        switch (g_SoundMusicUsesMidi) {
        case SOUND_MUSIC_BACKEND_NONE:
            MidiCrossFade(0, param2);
            MusicWaveCrossPlay(songId, param2);
            break;
        case SOUND_MUSIC_BACKEND_MIDI:
            MusicWaveCrossPlay(0, param2);
            MidiCrossFade(songId, param2 & 0xFF);
        }
        ret = 1;
        break;
    case SOUND_MUSIC_PAUSE:
        MusicWavePause();
        MidiPause();
        ret = 1;
        break;
    case SOUND_MUSIC_RESUME:
        MusicWaveResume();
        MidiResume();
        ret = 1;
        break;
    case SOUND_MUSIC_VOLUME:
        MusicWaveSetVolume((char)param1);
        MidiSetVolume(param1);
        ret = 1;
        break;
    case SOUND_MUSIC_VOLUME_TRANS:
        MusicWaveSetVolumeTrans((char)param2, (char)param1);
        MidiSetVolumeTrans(param2, param1);
        ret = 1;
        break;
    case SOUND_MUSIC_VOLUME_FADE:
        /* Retail caller prototype (char, char, char) -> void preserved for /Od argument promotion on `MusicWaveFadeVolume`. */
        ((void(__cdecl *)(char, char, char))MusicWaveFadeVolume)((char)param1, (char)param2, (char)param3);
        MidiFadeVolume(param1 & 0xFF, param2 & 0xFF, param3 & 0xFF);
        break;
    case SOUND_MUSIC_STOP:
        MusicWaveStop();
        MidiStop();
        ret = 1;
        break;
    case SOUND_MIDI_TEMPO:
        MidiSetTempo((char)param1);
        break;
    case SOUND_MIDI_TEMPO_TRANS:
        /* Retail caller passes 2 arg(s) to 1-parameter `MidiSetTempoTrans` (__cdecl stack cleanup matches retail). */
        ((void(__cdecl *)(char, int))MidiSetTempoTrans)((char)param1, param2 & 0xFF);
        break;
    case SOUND_MIDI_TEMPO_FADE:
        MidiFadeTempo(param1 & 0xFF, (char)param2, (char)param3);
        break;
    case SOUND_SFX_STOP_ALL:
        SfxStopAll();
        ret = 1;
        break;
    case SOUND_SFX_PLAY_CH1:
        SfxPlayOnChannel((char)param1, param2, 1);
        ret = 1;
        break;
    case SOUND_SFX_PLAY_CH2:
        SfxPlayOnChannel((char)param1, param2, 2);
        ret = 1;
        break;
    case SOUND_SFX_PLAY_CH3:
        SfxPlayOnChannel((char)param1, param2, 3);
        ret = 1;
        break;
    case SOUND_SFX_PLAY_CH4:
        SfxPlayOnChannel((char)param1, param2, 4);
        ret = 1;
        break;
    case SOUND_SFX_PLAY_1:
    case SOUND_SFX_PLAY_1_ALT:
        /* Retail call-site prototype (char, int, int, int, int) -> void preserved for /Od caller codegen. */
        ((void(__cdecl *)(char, int, int, int, int))SfxPlayUpToFour)((char)param1, param2, 0, 0, 0);
        ret = 1;
        break;
    case SOUND_SFX_PLAY_2:
    case SOUND_SFX_PLAY_2_ALT:
        /* Retail call-site prototype (char, int, int, int, int) -> void preserved for /Od caller codegen. */
        ((void(__cdecl *)(char, int, int, int, int))SfxPlayUpToFour)((char)param1, param2, param3, 0, 0);
        ret = 1;
        break;
    case SOUND_SFX_PLAY_3:
    case SOUND_SFX_PLAY_3_ALT:
        /* Retail call-site prototype (char, int, int, int, int) -> void preserved for /Od caller codegen. */
        ((void(__cdecl *)(char, int, int, int, int))SfxPlayUpToFour)((char)param1, param2, param3, param4, 0);
        ret = 1;
        break;
    case SOUND_SFX_PLAY_4:
    case SOUND_SFX_PLAY_4_ALT:
        /* Retail call-site prototype (char, int, int, int, int) -> void preserved for /Od caller codegen. */
        ((void(__cdecl *)(char, int, int, int, int))SfxPlayUpToFour)((char)param1, param2, param3, param4, param5);
        ret = 1;
        break;
    case SOUND_SFX_CH1_VOLUME:
        SfxSetChannelVolume((char)param1, 1);
        ret = 1;
        break;
    case SOUND_SFX_CH2_VOLUME:
        SfxSetChannelVolume((char)param1, 2);
        ret = 1;
        break;
    case SOUND_SFX_CH3_VOLUME:
        SfxSetChannelVolume((char)param1, 3);
        ret = 1;
        break;
    case SOUND_SFX_CH4_VOLUME:
        SfxSetChannelVolume((char)param1, 4);
        ret = 1;
        break;
    case SOUND_SFX_ALL_VOLUME:
        SfxSetAllChannelsVolume((char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH1_VOLUME_TRANS:
        SfxSetChannelVolumeTrans((char)param2, 1, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH2_VOLUME_TRANS:
        SfxSetChannelVolumeTrans((char)param2, 2, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH3_VOLUME_TRANS:
        SfxSetChannelVolumeTrans((char)param2, 3, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH4_VOLUME_TRANS:
        SfxSetChannelVolumeTrans((char)param2, 4, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_ALL_VOLUME_TRANS:
        SfxSetAllChannelsVolumeTrans((char)param2, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH1_PAN:
        SfxSetChannelPan((char)param1, 1);
        ret = 1;
        break;
    case SOUND_SFX_CH2_PAN:
        SfxSetChannelPan((char)param1, 2);
        ret = 1;
        break;
    case SOUND_SFX_CH3_PAN:
        SfxSetChannelPan((char)param1, 3);
        ret = 1;
        break;
    case SOUND_SFX_CH4_PAN:
        SfxSetChannelPan((char)param1, 4);
        ret = 1;
        break;
    case SOUND_SFX_ALL_PAN:
        SfxSetAllChannelsPan((char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH1_PAN_TRANS:
        SfxSetChannelPanTrans((char)param2, 1, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH2_PAN_TRANS:
        SfxSetChannelPanTrans((char)param2, 2, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH3_PAN_TRANS:
        SfxSetChannelPanTrans((char)param2, 3, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH4_PAN_TRANS:
        SfxSetChannelPanTrans((char)param2, 4, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_ALL_PAN_TRANS:
        SfxSetAllChannelsPanTrans((char)param2, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH1_TRANSPOSE:
        SfxSetChannelTranspose((char)param1, 1);
        ret = 1;
        break;
    case SOUND_SFX_CH2_TRANSPOSE:
        SfxSetChannelTranspose((char)param1, 2);
        ret = 1;
        break;
    case SOUND_SFX_CH3_TRANSPOSE:
        SfxSetChannelTranspose((char)param1, 3);
        ret = 1;
        break;
    case SOUND_SFX_CH4_TRANSPOSE:
        SfxSetChannelTranspose((char)param1, 4);
        ret = 1;
        break;
    case SOUND_SFX_ALL_TRANSPOSE:
        SfxSetAllChannelsTranspose((char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH1_TRANSPOSE_TRANS:
        SfxSetChannelTransposeTrans((char)param2, 1, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH2_TRANSPOSE_TRANS:
        SfxSetChannelTransposeTrans((char)param2, 2, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH3_TRANSPOSE_TRANS:
        SfxSetChannelTransposeTrans((char)param2, 3, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_CH4_TRANSPOSE_TRANS:
        SfxSetChannelTransposeTrans((char)param2, 4, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_ALL_TRANSPOSE_TRANS:
        SfxSetAllChannelsTransposeTrans((char)param2, (char)param1);
        ret = 1;
        break;
    case SOUND_SFX_PAUSE:
        SfxPauseAll();
        ret = 1;
        break;
    case SOUND_SFX_RESUME:
        SfxResumeAll();
        ret = 1;
        break;
    case SOUND_SFX_PRELOAD_PLAY_CH5:
        SfxPreloadAndPlayOnChannel5(param1);
        ret = 1;
        break;
    case SOUND_PAUSE_ALL:
        SfxPauseAll();
        MusicWavePause();
        MidiPause();
        ret = 1;
        break;
    case SOUND_RESUME_ALL:
        SfxResumeAll();
        MusicWaveResume();
        MidiResume();
        ret = 1;
    }
    return ret;
}

/*
 * Returns zero (a wave track) for SATO, WIND, SENSUI and HEART, and for OA, OB, BAT and LB2 when
 * g_MusicWaveRegistrySetting is set. Returns one (MIDI) for all other songs, including the explicit
 * cases CINTRO, NOINTRO and MOGU.
 */
// FUNCTION: FF7 0x007416a3
int __cdecl MusicTrackUsesMidi(int track)
{
    switch (track) {
    case SOUND_SONG_OA:
    case SOUND_SONG_OB:
    case SOUND_SONG_BAT:
    case SOUND_SONG_LB2:
        if (g_MusicWaveRegistrySetting != 0) {
            return 0;
        }
        break;
    case SOUND_SONG_SATO:
    case SOUND_SONG_WIND:
    case SOUND_SONG_SENSUI:
    case SOUND_SONG_HEART:
        return 0;
    case SOUND_SONG_CINTRO:
    case SOUND_SONG_NOINTRO:
    case SOUND_SONG_MOGU:
        break;
    default:
        return 1;
    }
    return 1;
}
} /* extern "C" */

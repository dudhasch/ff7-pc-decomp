/* include/lib/enums/sound.h: named codes of lib/sound and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef LIB_ENUMS_SOUND_H
#define LIB_ENUMS_SOUND_H

/* Menu sound ids of SfxPreloadAndPlayOnChannel5, named by where the menus play them: on every
 * cursor move, when an action is refused, on cancel (Cross), and when a materia is removed. */
enum MenuSound {
    SFX_MENU_CURSOR = 1,
    SFX_MENU_CONFIRM = 2,
    SFX_MENU_INVALID = 3,
    SFX_MENU_CANCEL = 4,
    SFX_MENU_SAVE_LOAD = 0xD0,
    SFX_MENU_ITEM_EFFECT = 0x107, /* an item or spell takes effect on a party member */
    SFX_MENU_SHOP_TRADE = 0x108, /* a shop buys or sells an item or a materia */
    SFX_MENU_HEAL = 0x180,
    SFX_MENU_MATERIA_EQUIP = 0x19F,
    SFX_MENU_MATERIA_REMOVE = 0x1BF /* a materia taken off its slot and returned to the stock */
};

/* Sound command codes that SoundDispatchCommand routes to wave/MIDI music and the five SFX channels. */
enum SoundCommand {
    SOUND_MUSIC_PLAY = 0x10,
    SOUND_MUSIC_PLAY_ALT = 0x14,              /* same handler as MUSIC_PLAY */
    SOUND_SIDE_SWITCH = 0x15,                 /* no effect on PC */
    SOUND_MUSIC_CROSSFADE = 0x18,
    SOUND_MUSIC_CROSSFADE_ALT = 0x19,         /* same handler as MUSIC_CROSSFADE */
    SOUND_SFX_PLAY_1 = 0x20,
    SOUND_SFX_PLAY_2 = 0x21,
    SOUND_SFX_PLAY_3 = 0x22,
    SOUND_SFX_PLAY_4 = 0x23,
    SOUND_SFX_PLAY_1_ALT = 0x24,
    SOUND_SFX_PLAY_2_ALT = 0x25,
    SOUND_SFX_PLAY_3_ALT = 0x26,
    SOUND_SFX_PLAY_4_ALT = 0x27,
    SOUND_SFX_PLAY_CH1 = 0x28,
    SOUND_SFX_PLAY_CH2 = 0x29,
    SOUND_SFX_PLAY_CH3 = 0x2A,
    SOUND_SFX_PLAY_CH4 = 0x2B,
    SOUND_SFX_PRELOAD_PLAY_CH5 = 0x30,        /* param1 is the sound id, not a pan */
    SOUND_RESUME_ALL = 0x98,
    SOUND_PAUSE_ALL = 0x99,
    SOUND_MUSIC_RESUME = 0x9A,
    SOUND_MUSIC_PAUSE = 0x9B,
    SOUND_SFX_RESUME = 0x9C,
    SOUND_SFX_PAUSE = 0x9D,
    SOUND_SFX_CH1_VOLUME = 0xA0,
    SOUND_SFX_CH2_VOLUME = 0xA1,
    SOUND_SFX_CH3_VOLUME = 0xA2,
    SOUND_SFX_CH4_VOLUME = 0xA3,
    SOUND_SFX_CH1_VOLUME_TRANS = 0xA4,        /* param1 = steps, param2 = volume */
    SOUND_SFX_CH2_VOLUME_TRANS = 0xA5,        /* param1 = steps, param2 = volume */
    SOUND_SFX_CH3_VOLUME_TRANS = 0xA6,        /* param1 = steps, param2 = volume */
    SOUND_SFX_CH4_VOLUME_TRANS = 0xA7,        /* param1 = steps, param2 = volume */
    SOUND_SFX_CH1_PAN = 0xA8,
    SOUND_SFX_CH2_PAN = 0xA9,
    SOUND_SFX_CH3_PAN = 0xAA,
    SOUND_SFX_CH4_PAN = 0xAB,
    SOUND_SFX_CH1_PAN_TRANS = 0xAC,           /* param1 = steps, param2 = pan */
    SOUND_SFX_CH2_PAN_TRANS = 0xAD,           /* param1 = steps, param2 = pan */
    SOUND_SFX_CH3_PAN_TRANS = 0xAE,           /* param1 = steps, param2 = pan */
    SOUND_SFX_CH4_PAN_TRANS = 0xAF,           /* param1 = steps, param2 = pan */
    SOUND_SFX_CH1_TRANSPOSE = 0xB0,
    SOUND_SFX_CH2_TRANSPOSE = 0xB1,
    SOUND_SFX_CH3_TRANSPOSE = 0xB2,
    SOUND_SFX_CH4_TRANSPOSE = 0xB3,
    SOUND_SFX_CH1_TRANSPOSE_TRANS = 0xB4,     /* param1 = steps, param2 = transpose */
    SOUND_SFX_CH2_TRANSPOSE_TRANS = 0xB5,     /* param1 = steps, param2 = transpose */
    SOUND_SFX_CH3_TRANSPOSE_TRANS = 0xB6,     /* param1 = steps, param2 = transpose */
    SOUND_SFX_CH4_TRANSPOSE_TRANS = 0xB7,     /* param1 = steps, param2 = transpose */
    SOUND_SFX_ALL_VOLUME = 0xB8,
    SOUND_SFX_ALL_VOLUME_TRANS = 0xB9,        /* param1 = steps, param2 = volume */
    SOUND_SFX_ALL_PAN = 0xBA,
    SOUND_SFX_ALL_PAN_TRANS = 0xBB,           /* param1 = steps, param2 = pan */
    SOUND_SFX_ALL_TRANSPOSE = 0xBC,
    SOUND_SFX_ALL_TRANSPOSE_TRANS = 0xBD,     /* param1 = steps, param2 = transpose */
    SOUND_MUSIC_VOLUME = 0xC0,
    SOUND_MUSIC_VOLUME_TRANS = 0xC1,          /* param1 = steps, param2 = volume */
    SOUND_MUSIC_VOLUME_FADE = 0xC2,
    SOUND_MIDI_TEMPO = 0xD0,
    SOUND_MIDI_TEMPO_TRANS = 0xD1,
    SOUND_MIDI_TEMPO_FADE = 0xD2,
    SOUND_MUSIC_STOP = 0xF0,
    SOUND_SFX_STOP_ALL = 0xF1,
    SOUND_PSX_NOP_F4 = 0xF4,                  /* PSX SPU hook around battle entry; no-op on PC */
    SOUND_PSX_NOP_F5 = 0xF5                   /* PSX SPU hook after battle entry; no-op on PC */
};

/* MIDI data type g_MidiDataType, chosen from the registry mode string: the song bank file it
 * selects is midi.lgp, awe.lgp, xg.lgp or ygm.lgp. */
enum MidiDataType {
    MIDI_DATA_GENERAL_MIDI = 0,   /* "GENERAL_MIDI"; midi.lgp */
    MIDI_DATA_SOUNDFONT_MIDI = 1, /* "SOUNDFONT_MIDI"; awe.lgp */
    MIDI_DATA_YAMAHA_XG = 2,      /* "YAMAHA_XG"; xg.lgp */
    MIDI_DATA_YAMAHA_GM = 3       /* "YAMAHA_GM"; ygm.lgp */
};

/* g_MidiPlaybackState: MidiPause and MidiResume set PAUSED and PLAYING, the stream's MOM_CLOSE and
 * the last returned buffer STOPPED. MidiIsPlaying also counts a state 3, which nothing sets. */
enum MidiPlaybackState {
    MIDI_PLAYBACK_STOPPED = 0,
    MIDI_PLAYBACK_PLAYING = 1,
    MIDI_PLAYBACK_PAUSED = 2
};

/* Status bytes of a standard MIDI file's track events, as MidiConvertNextEvent parses them (channel
 * messages with the channel in the low nibble). */
enum MidiStatusByte {
    MIDI_STATUS_NOTE_OFF = 0x80,
    MIDI_STATUS_NOTE_ON = 0x90,
    MIDI_STATUS_POLY_PRESSURE = 0xA0,
    MIDI_STATUS_CONTROL_CHANGE = 0xB0,
    MIDI_STATUS_PROGRAM_CHANGE = 0xC0,   /* one data byte */
    MIDI_STATUS_CHANNEL_PRESSURE = 0xD0, /* one data byte */
    MIDI_STATUS_PITCH_BEND = 0xE0,
    MIDI_STATUS_SYSEX = 0xF0,            /* a length, then the bytes up to MIDI_STATUS_SYSEX_END */
    MIDI_STATUS_SYSEX_END = 0xF7,
    MIDI_STATUS_META = 0xFF              /* a meta event: its type byte (MidiMetaEvent) and a length */
};

/* Meta event types of a standard MIDI file that the track parser handles. */
enum MidiMetaEvent {
    MIDI_META_COPYRIGHT = 0x02,
    MIDI_META_MARKER = 0x06,      /* FF7's "loopStart" and "loopEnd" markers */
    MIDI_META_END_OF_TRACK = 0x2F,
    MIDI_META_SET_TEMPO = 0x51    /* three bytes of microseconds per quarter note */
};

/* MIDI controller numbers (the first data byte of MIDI_STATUS_CONTROL_CHANGE). */
enum MidiController {
    MIDI_CONTROLLER_ALL_NOTES_OFF = 0x7B
};

/* Music song ids that the code names, by their entry in g_SoundMidiSongNames (the MIDI file name). */
enum SoundSongId {
    SOUND_SONG_OA = 0x01,
    SOUND_SONG_OB = 0x02,
    SOUND_SONG_BAT = 0x07,
    SOUND_SONG_SATO = 0x0F,
    SOUND_SONG_CINTRO = 0x32,
    SOUND_SONG_WIND = 0x41,
    SOUND_SONG_LB2 = 0x51,
    SOUND_SONG_SENSUI = 0x58,
    SOUND_SONG_NOINTRO = 0x5B,
    SOUND_SONG_MOGU = 0x5E,
    SOUND_SONG_HEART = 0x61,
    SOUND_SONG_LAST = 0x62        /* "ROLL", the last of the 99 entries */
};

/* A wave's WAVEFORMATEX format tag, as the sound and music streams read it: PCM plays as it is,
 * ADPCM goes through an ACM stream to 16-bit PCM (WAVE_FORMAT_PCM, WAVE_FORMAT_ADPCM). */
enum SoundWaveFormatTag {
    SOUND_WAVE_FORMAT_PCM = 1,
    SOUND_WAVE_FORMAT_ADPCM = 2
};

/* The msacm32 flags acm.cpp passes (<msacm.h>'s ACM_STREAMSIZEF_DESTINATION, ACM_STREAMOPENF_ASYNC,
 * ACM_STREAMCONVERTF_BLOCKALIGN; acm.cpp declares the entry points itself, without <msacm.h>). */
enum SoundAcmFlag {
    SOUND_ACM_STREAMSIZEF_SOURCE = 0x0,         /* acmStreamSize: the destination size for a source size */
    SOUND_ACM_STREAMSIZEF_DESTINATION = 0x1,    /* acmStreamSize: the source size for a destination size */
    SOUND_ACM_STREAMOPENF_ASYNC = 0x2,
    SOUND_ACM_STREAMCONVERTF_BLOCKALIGN = 0x4
};

/* AcmStreamOpen's mode. */
enum SoundAcmOpenMode {
    SOUND_ACM_OPEN_SYNC = 0,
    SOUND_ACM_OPEN_ASYNC = 1    /* SOUND_ACM_STREAMOPENF_ASYNC */
};

/* AcmStreamConvert's mode. */
enum SoundAcmConvertMode {
    SOUND_ACM_CONVERT_WHOLE = 0,
    SOUND_ACM_CONVERT_BLOCK_ALIGN = 1   /* SOUND_ACM_STREAMCONVERTF_BLOCKALIGN */
};

/* The hundreds of a SoundFont manager status code, the class SfManErrorText picks its text table by
 * (the remainder indexes the class's table). */
enum SoundSfManErrorClass {
    SOUND_SFMAN_ERROR_SUCCESS = 0,
    SOUND_SFMAN_ERROR_INVALID = 100,    /* invalid ids, indices, files and devices */
    SOUND_SFMAN_ERROR_CONTENTION = 200, /* a busy manager or driver, insufficient resources or memory */
    SOUND_SFMAN_ERROR_MANAGER = 400,    /* loading the SoundFont Master Manager, the device, the API version */
    SOUND_SFMAN_ERROR_MISC = 900        /* text only by number */
};

/* What a DirectSound stream (SoundStreamPlayer::state) plays, and the file it reads its chunks from:
 * a sound effect from audio.dat, or the music wave file. */
enum SoundStreamKind {
    SOUND_STREAM_SFX = 0,    /* g_SoundData.sfxDataFileHandle */
    SOUND_STREAM_MUSIC = 1   /* g_SoundData.musicFileHandle */
};

/* Sound-effect indices in audio.fmt / audio.dat passed to SfxLoadIntoBuffer, SfxPlaySound*, and Battle_PlaySfx*. */
enum SoundSfxId {
    SFX_ID_CURSOR = 1,
    SFX_ID_CANCEL = 4,
    SFX_ID_PRELUDE_CHIME = 0x2B,
    SFX_ID_FANFARE_COUNT = 0x2C,
    SFX_ID_LEVEL_UP = 0x2D,
    SFX_ID_SEPHIROTH_HUM = 0x82,
    SFX_ID_CONDOR_ALLY_HIT = 0xE1,
    SFX_ID_CONDOR_UNIT_STEP = 0xE2,
    SFX_ID_CONDOR_FAST_SHOT_ALT = 0xE3,
    SFX_ID_CONDOR_FAST_SHOT = 0xE4,
    SFX_ID_CONDOR_SPIN_BURST = 0xE6,
    SFX_ID_CONDOR_ARC_SHOT = 0xE7,
    SFX_ID_CONDOR_BOSS_STEP = 0xE8,
    SFX_ID_CONDOR_SPIN_SHOT = 0xF3,
    SFX_ID_EXPLOSION_IMPACT = 0xFA,
    SFX_ID_LASBOSS_RUMBLE = 0x130,
    SFX_ID_LASBOSS_WING = 0x132,
    SFX_ID_LASBOSS_FLARE = 0x17F,
    SFX_ID_GARYOTEN_WIND = 0x182,
    SFX_ID_METEOR_STRIKE = 0x188,
    SFX_ID_LASBOSS_SHOCK = 0x189,
    SFX_ID_SUB_SONAR = 0x195,
    SFX_ID_HOWLING_MOON = 0x1AA,
    SFX_ID_LASBOSS_BEAM = 0x1B5,
    SFX_ID_SUB_TORPEDO = 0x1D2,
    SFX_ID_BATTLE_CMD_CHANGE = 0x1D3,
    SFX_ID_GIL_TICK = 0x1E1,
    SFX_ID_BATTLE_LIMIT_READY = 0x1E3,
    SFX_ID_SUB_MINE_HIT = 0x229,
    SFX_ID_SUB_COLLISION = 0x22A,
    SFX_ID_SUB_DIVE = 0x230,
    SFX_ID_SUB_ALERT = 0x238,
    SFX_ID_LASBOSS_SHATTER = 0x23C,
    SFX_ID_OMNISLASH_SWING = 0x241,
    SFX_ID_SUB_EXPLOSION = 0x243,
    SFX_ID_OMNISLASH_FINISH = 0x285,
    SFX_ID_OMNISLASH_CHARGE = 0x28E,
    SFX_ID_LASBOSS_FINALE = 0x2E7
};


enum SoundSfxChannelIndex {
    SOUND_SFX_CHANNEL_1 = 1,
    SOUND_SFX_CHANNEL_2 = 2,
    SOUND_SFX_CHANNEL_3 = 3,
    SOUND_SFX_CHANNEL_4 = 4
};

enum SoundMusicBackendMode {
    SOUND_MUSIC_BACKEND_NONE = 0,
    SOUND_MUSIC_BACKEND_MIDI = 1
};
#endif

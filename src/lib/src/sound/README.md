# Sound: music and sound effects

lib/sound plays the game's music and sound effects through DirectSound, the WinMM MIDI stream API,
ACM (ADPCM decoding) and, on Creative cards, SoundFont banks. It is a service, not a game mode:
`App_InitSubsystems` (ff7/main, [../../../ff7/src/main/README.md](../../../ff7/src/main/README.md))
starts it with `SoundInit` and `MidiInit`, the game modules send commands through
`SoundDispatchCommand` and call `SoundUpdateFades` from their frame functions, and `App_Shutdown` stops it with
`SoundShutdown` and `MidiShutdown`.

## Where the code is

| Folder | TUs | Functions |
|---|---|---|
| `src/lib/src/sound/` and `creative/sfutils.cpp`, `creative/creative_sfman_error_messages.cpp` | 12 | 203 |

- `creative/creative_sfman_error_messages.cpp` (`SfManFormatUnknownError`, `SfManErrorText`, `SfManIsAbsBelow100`) formats SFMAN32 error codes into text for `sfutils.cpp` and `sound_soundfont_open_close.cpp`; the four world-map TUs immediately following it live in [../../../ff7/src/wm/README.md](../../../ff7/src/wm/README.md).
- `sound.cpp` also holds the sound effect and music service (0x00748950..0x00749940): the external
  object map has one `sound.obj` over both, and `"wave_music"` (0x00967F80) is a local `char str[]` of
  `MusicReadWaveMusicRegistrySetting`, the first literal after all of `sound.cpp`'s.
- Six TUs here are named after what they contain: `sound_sfx_group_preload.cpp`, `sound_soundfont_open_close.cpp`,
  `sound_multimedia_error_text.cpp`, `sound_midi_song_names.cpp`, `sound_directsound_error_text.cpp`,
  `creative_sfman_error_messages.cpp`. Their bodies are sound code (SFX streams and groups, SFMAN32, WinMM and
  DirectSound error text, MIDI song names) and they sit inside the sound run `0x00740d80..0x0074ba80`.
- TU boundaries follow retail's objects.

## Entry points

| Function | Address | What it does | Called from |
|---|---|---|---|
| `SoundInit` | 0x00744400 | Copies the sound and music directories, reads `music.idx`, opens the DirectSound device, creates a 44.1 kHz 16-bit stereo primary buffer, loads `audio.fmt`, preloads 17 static sounds, resets channels 1 to 5 | `App_InitSubsystems` (failure sets game mode 19) |
| `MidiInit` | 0x00741780 | Picks the MIDI device and data type from the registry, loads a SoundFont on a Creative device, opens the MIDI LGP as archive 11 | `App_InitSubsystems` |
| `SoundDispatchCommand` | 0x00740d80 | Routes a `SoundCommand` code to wave or MIDI music and the SFX channels | 174 sites: battle, chocobo, coaster, condor, field, main, menu, wm |
| `SoundUpdateFades` | 0x00744b13 | Per-frame step: `MidiUpdateVolumeFade`, then the wave music volume and the volume, pan and transpose transitions of the five channels, timed by `GetTickCount` | `Battle_ModuleFrame`, `Field_UpdateLogic`, the world map, the menu frames, credits, coaster, highway, snowboard |
| `SfxPreloadAndPlayOnChannel5` | 0x0074580a | Loads sound id 1..751 if not cached and plays it on channel 5 at pan 0x40 | 396 sites: battle, menu, wm, lib/graphics |
| `SfxLoadIntoBuffer`, `Sfx_ReleaseCachedBuffer` | 0x00745cf3, 0x00745dbb | Decode one sound from `audio.dat` into its cached DirectSound buffer; release it | 58 and 297 sites, mostly battle |
| `SfxPlayOnChannel`, `SfxPlayUpToFour`, `SfxStopAll` | 0x00745160, 0x00745606, 0x00745b64 | Play a sound on channel 1..5; play up to four at once (one long sound is streamed); stop all | `SoundDispatchCommand`; battle, field, main, menu, wm, lib/graphics |
| `SfxRegisterSoundGroups`, `SfxPlaySoundGroup` | 0x0074938d, 0x00749404 | Copy up to 16 groups of four sound ids and preload them on a lib/thread job; wait for one group and play its four buffers | battle effects (35 and 5 sites) |
| `MidiPlaySong`, `MidiStop`, `MidiSetVolume` | 0x00742055, 0x00742e2b, 0x00742f37 | Direct MIDI control | coaster, field, menu, wm |
| `SfxSetMasterVolumePercent`, `MusicSetMasterVolumePercent` | 0x0074933d, 0x0074934a | Store the SFX and music master percentages | `src/ff7/src/menu/english/menu_config_screen.cpp` (ff7/menu) |
| `SoundShutdown`, `MidiShutdown` | 0x007446d7, 0x00741f5f | Stop timers, free buffers and tables; restore the MIDI driver volume, close the SoundFont device and archive 11 | `App_Shutdown` |
| `GetMidiLgpPath` | 0x007443e7 | The MIDI LGP path, or 0 before `MidiInit` succeeded | `BuildArchivePath` (ff7/main) |

## Main structures

The state is held in `g_SoundChannels`, `g_SoundData`, and typed stream and channel records in `include/lib/types/sound.h`.

| Type or global | Header | What it holds |
|---|---|---|
| `g_SoundChannels` | defined in `sound.cpp` | The 24,988-byte sound state block. Its first int is the "sound initialized" flag every entry tests; `sound.cpp` names members with `#define`s: the primary buffer (+0x4), 750 cached buffer handles (+0x8), the five 0x54-byte effect channels (`channels[5]`, +0xbc0), 750 0x1C-byte SFX records (`soundRecords`, +0xd64: size, `audio.dat` offset, stream flag, loop data, format pointer), the 98 wave-track name pointers (+0x5f6c) |
| `g_SoundData` | defined in `sound.cpp` | Second state block: the music and SFX stream records, file handles of `audio.dat` and the open `.wav`, the two data directories, master volume percents, pan-invert option |
| `SoundChannel` | `include/lib/types/sound.h` | One effect channel (0x54 bytes): volume, pan and transpose with their fades, the sample rate, the sound id, the buffer and its loop buffer, the loop switch flag and the saved sound id. |
| `SoundStreamPlayer`, `SoundStreamState`, `SoundWaveStream`, `SoundSfxStream` | same | Views of a stream record (DirectSound buffer, source, chunk and write positions, ACM stream) used by `SfxStreamService`, `SfxStreamPrime`, `MusicWaveStreamOpen`, `SfxStreamOpen` |
| `SoundSfxDecodeRecord` | same | View of an SFX record read by `SfxDecodeToStaticBuffer` |
| `SoundGroup` | same | Four sound ids; the 16-entry group table is `g_SoundGroups` |
| `SfMan*`, `SfManApi` | same | SFMAN32 request records and the interface table `SoundFontBindSfman32` obtains |
| `g_SoundVolumeToMillibels`, `g_SoundPanToMillibels`, `g_SoundStartupPreloadIds` | defined in `sound.cpp` | Volume 0..127 to millibels, pan 0..127 to millibels, the 17 static sound ids |
| `g_Midi*` globals | defined in `midi1.cpp` | Device id, data type, LGP path, stream handle, six 0x448-byte stream buffers (`MidiStreamBuffer midiStreamBuffers[6]`), per-track play state (`MidiTrack midiTrackState[]`) |

## Data flow

1. Startup. `SoundInit` reads `music.idx` from the music directory (`MusicLoadWaveNameIndex`,
   98 names, line n names track n), opens the device from `RegGetSoundDeviceGuid` (`DsOpenDevice`),
   and `SfxLoadFormatTable` reads 750 records from `audio.fmt` (24 bytes plus a WAVEFORMATEX, with
   extra bytes for format tag 2) and keeps `audio.dat` open. `MusicReadWaveMusicRegistrySetting`
   reads `wave_music` (ENABLED or DISABLED). `App_InitSubsystems` then preloads sound 0x2B.
2. `MidiInit` uses `RegGetMidiDeviceId` (else the MIDI mapper). If the device's manufacturer id is
   2 it binds `SFMAN32.DLL` (`SoundFontOpenDevice`) and loads `sf2\ff7.sf2`, `ff7m.sf2` or `ff7s.sf2`
   by sample RAM (at least 4 MB, 2 MB, 512 KB). `RegGetMidiDataType` (`GENERAL_MIDI`,
   `SOUNDFONT_MIDI`, `YAMAHA_XG`, `YAMAHA_GM`, or the device name `YAMAHA SXG70 Driver`) selects
   `midi.lgp`, `awe.lgp`, `xg.lgp` or `ygm.lgp`, opened with `OpenLgpArchive(path, 11)`.
3. Music. `SoundDispatchCommand` accepts tracks 1..98; track 0 stops both backends.
   `MusicTrackUsesMidi` returns wave for tracks 0x0F, 0x41, 0x58, 0x61 always and for 1, 2, 7, 0x51
   when `wave_music` is ENABLED; all others are MIDI.
   - Wave: `MusicPlayWaveTrack` opens `<music dir>\<name>.wav` from disk, `MusicWaveStreamOpen` and
     `SfxStreamPrime` set up a looping DirectSound buffer, and an 80 ms multimedia timer
     (`MusicWaveStreamServiceTimerProc`) calls `SfxStreamService` to decode the next chunks.
     `MusicWaveCrossPlay` fades out to volume 15, then `SoundUpdateFades` starts the queued track.
   - MIDI: `MidiPlaySong` calls `MidiLoadSongFromLgp`, which reads `<name>.mid` from archive 11
     (`MidiGetSongName`, at most 0x14C00 bytes). `MidiStartPlayback` parses `MThd`/`MTrk`, fills six
     stream buffers with `MidiFillStreamBuffer` and `MidiConvertNextEvent`, and `MidiStreamCallback`
     refills one per `MOM_DONE`. Text events `loopStart` and `loopEnd` save and restore the track
     state (`MidiEmitLoopRestart` sends controller 0x7B on all 16 channels). Song 81 swaps in
     `sf2\lb2.sf2` on a SoundFont device with at least 2 MB.
4. Sound effects are one-based ids; the range check passes 1..751, `audio.fmt` supplies 750
   records. `SfxLoadIntoBuffer` seeks `audio.dat` to the record's offset;
   `SfxDecodeToStaticBuffer` decodes format tag 2 through ACM (`acm.cpp`) to PCM16 into a new
   DirectSound buffer and returns 0 for any other tag. `SfxPlayOnChannel` duplicates the cached
   buffer or loads into the channel; a sound longer than 0x20000 bytes without the stream flag is
   streamed by `SfxPlayUpToFour` on an 80 ms timer (`SfxStreamServiceTimerProc`). Records with loop
   data get `SfxBuildLoopBuffer` (the channel's second buffer) and a one-shot
   `SfxLoopSwitchTimerProc` timer.
5. Per frame, `SoundUpdateFades` moves each transition by elapsed milliseconds times its rate and
   applies it with `DsSetVolumeMillibels`, `DsSetPanMillibels`, `DsSetFrequency`. Volume is
   `master percent * level / 100`, looked up in `g_SoundVolumeToMillibels`.
6. Shutdown releases the 17 static sounds, the channel and cached buffers, the format records and
   name strings, closes `audio.dat` and the device; `MidiShutdown` restores the driver volume.

## Named codes

| Enum | Header | Use |
|---|---|---|
| `SoundCommand` | `include/lib/enums/sound.h` | The 62 command codes of `SoundDispatchCommand` (music 0x10..0x19, SFX play 0x20..0x30, pause/resume 0x98..0x9D, channel volume/pan/transpose 0xA0..0xBD, music volume 0xC0..0xC2, tempo 0xD0..0xD2, stops 0xF0/0xF1); used by `access.cpp`, `main.cpp`, 18 battle TUs and five credits and submarine TUs |

## Notes

In the sound TUs:

- Table access is raw: `*(int *)((char *)&g_SoundChannels.channelFileOffset + idx * 0x1C)`,
  `*(int *)((char *)&g_SoundChannels.channelVolume + (channel - 1) * 0x54)`; the channel and SFX records have no
  struct of their own beyond the partial views above.
- Lookup tables and state globals in `sound.cpp` (`g_SoundVolumeToMillibels`, `g_SoundPanToMillibels`, `g_MusicWavHeaderBuffer`) and `midi1.cpp` (`g_MusicTrack1Requested`, `g_MidiPitchBendResetEvents`, `g_MidiExpressionResetEvents`),
  MIDI volume fade's (`g_MidiVolumeFadeRate`, `g_MidiVolumeFadeLastTime`) and the music fade's are named (`g_MusicVolume`, `g_MusicTargetVolume`, `g_MusicFadeRate`, `g_MusicFadeElapsed`,
  `g_MusicFadeStarted`, `g_MusicPlayAfterFade`, `g_MusicPendingTrack`).
- `g_SoundChannels` names a whole state block after its first member.
- `SoundDispatchCommand` casts callees to `char` parameter types to keep retail's byte truncation.
- `midi1.cpp` keeps two gotos in `MidiInit` (commented at the site).
- Eleven `Sfx_UnusedNoOp_*` / `Sfx_UnusedReturnZero_*` stubs and `AcmUnusedNoOpCallback` are
  never called by name.

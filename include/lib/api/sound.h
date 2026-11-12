/* include/lib/api/sound.h: the functions and objects lib/sound defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_SOUND_H
#define LIB_API_SOUND_H

#ifdef __cplusplus
extern "C" {
#endif
struct MidiStreamBuffer;
struct MidiTrack;
struct SoundChannel;
struct SoundSfxDecodeRecord;
struct SoundSfxStream;
struct SoundStreamState;
struct SoundWaveInfo;
struct SoundChannels;
struct SoundData;
struct SoundStreamPlayer;
struct SoundRecordEntry;

struct SoundGroup;

/* access.cpp */
int __cdecl SoundDispatchCommand(short command, int param1, int param2, int param3, int param4, int param5, int reserved7, int reserved8, int reserved9);
int __cdecl MusicTrackUsesMidi(int track);

/* acm.cpp */
void __cdecl AcmMakePcm16Format(unsigned int bytes, void *source, void *destination);
void __stdcall AcmUnusedNoOpCallback(int context, int param, int thirdParam, int fourthParam, int fifthParam);
unsigned int __cdecl AcmGetDecodedSize(int stream, unsigned int inputSize);
unsigned int __cdecl AcmGetSourceSizeForDecoded(int stream, unsigned int inputSize);
int __cdecl AcmStreamUnprepare(int stream, int header);
int __cdecl AcmStreamClose(int stream);

/* creative_sfman_error_messages.cpp */
int __cdecl SfManIsAbsBelow100(int value);
char *__cdecl SfManFormatUnknownError(const char *prefix, unsigned int error);
char *__cdecl SfManErrorText(int value);

/* sfutils.cpp */
extern unsigned short g_SfDeviceCount;
extern unsigned short g_SfSynthConfig;
int __cdecl SoundFontBindSfman32(void);
unsigned short __cdecl SoundFontProbeDevice(unsigned int *ram, unsigned short *midi, int *hardware);
int __cdecl SfManSaveSynthConfig(unsigned short device);
int __cdecl SfManRestoreSynthConfig(unsigned short device);
int __cdecl SfManOpenDevice(unsigned short device);
int __cdecl SfManCloseDevice(unsigned short device);
void __cdecl SoundFontUnbindSfman32(unsigned short device);
unsigned short __cdecl SoundFontPickBestDevice(unsigned int *ram, int *hardware);
int __cdecl SoundFontSaveLoadedBanks(unsigned short device);
int __cdecl SfManClearAllBanks(unsigned short device);
int __cdecl SoundFontRestoreLoadedBanks(unsigned short device);
int __cdecl SfManCopyPresetFromFile(unsigned short device, unsigned short sourceBank, unsigned short sourcePreset, unsigned short destinationBank, unsigned short destinationPreset, char *path);
int __cdecl SfManCopyPresetRange(unsigned short dev, int unused, char *path);
int __cdecl SfManLoadBankFromFileAlt(unsigned short device, int fileOffset, int unused, unsigned short bank);
int __cdecl SfManGetBankFileName(unsigned short device, unsigned short bank, char *name);
int __cdecl SfManGetSampleRamSize(unsigned short dev, unsigned int *total, unsigned int *free);
int __cdecl SoundFontFindMidiOutIndex(unsigned short device);
int __cdecl SfManGetDeviceCount(void);
int __cdecl SfManClearPreset(unsigned short device);

/* dx_snd.cpp */
extern int g_SoundDsDeviceCount;
int DsEnumerateDevices(void **out);
void __cdecl DsFreeEnumeratedDevices(void);
void __cdecl DsReleaseDevice(void);

/* midi1.cpp */
extern int g_MusicTrack1Requested;
int __cdecl MidiInit(char *dataDir, int unused);
void __cdecl MidiShutdown(void);
void __cdecl MidiSetDataDir(char *dataDir);
void __cdecl MidiPlaySong(int song, int unused1, int unused2);
int __cdecl MidiLoadSongFromLgp(int song);
int __cdecl MidiFileTake(int size);
int __cdecl MidiStartPlayback(void);
int __cdecl MidiSubmitStreamBuffer(struct MidiStreamBuffer *buffer);
int __cdecl MidiSelectNextTrack(void);
int __cdecl MidiReadVarLen(struct MidiTrack *track);
void __cdecl MidiCrossFade(int song, int duration);
void __cdecl MidiPause(void);
void __cdecl MidiResume(void);
void __cdecl MidiStop(void);
int __cdecl MidiIsPlaying(void);
void __cdecl MidiSetMasterVolume(unsigned int volume);
void __cdecl MidiSetVolume(int level);
void __cdecl MidiSetVolumeTrans(int volume, int step);
void __cdecl MidiFadeVolume(int duration, unsigned int start, unsigned int end);
void __cdecl MidiSetTempo(char tempoLevel);
void __cdecl MidiSetTempoTrans(int tempo);
void __cdecl MidiFadeTempo(int duration, char fadeMode, char tempo);
void __cdecl MidiSetPitch(void);
void __cdecl MidiSetPitchTrans(void);
void __cdecl MidiFadePitch(void);
void __cdecl MidiSetPan(void);
void __cdecl MidiSetPanTrans(void);
void __cdecl MidiFadePan(void);
int __cdecl MidiFillStreamBuffer(struct MidiStreamBuffer *stream);
int __cdecl MidiConvertNextEvent(struct MidiStreamBuffer *stream);
void __cdecl MidiEmitLoopRestart(struct MidiStreamBuffer *stream);
void __stdcall MidiStreamCallback(int handle, int message, int instance, int parameter1, int parameter2);
void __cdecl MidiUpdateVolumeFade(void);
int __cdecl MidiApplyDriverVolume(int value);
int __cdecl MidiStreamSetTempo(int tempo);
int __cdecl MidiStreamOpenDevice(void);
int __cdecl MidiStreamStopChecked(void);
int __cdecl MidiStreamCloseDevice(void);
void __cdecl MidiWaitForClose(void);
void __cdecl MidiWaitForStop(void);
int __cdecl MidiGetVolumeControlFlag(void);
void __cdecl MidiSendGmReset(void);
int __cdecl MidiPrimeExpressionReset(void *handle, void *header);
int __cdecl MidiPrimePitchBendReset(void *handle, void *header);
int __cdecl MidiSendEventBlock(int *handle, int *header, int data, int size);
void __cdecl MidiUnprepareBuffers(int count);
int __cdecl GetMidiLgpPath(void);

/* sound.cpp */
extern int g_MusicWaveRegistrySetting;
extern int g_SoundGroupSlots[4];
struct _RTL_CRITICAL_SECTION;
extern struct _RTL_CRITICAL_SECTION g_SoundLock;
extern struct SoundGroup g_SoundGroups[16];
extern struct SoundChannels g_SoundChannels;
extern int g_MusicWavHeaderBuffer;
extern struct SoundData g_SoundData;
extern unsigned char g_SoundCriticalSectionMusic[24];
extern int g_MusicVolume;
extern unsigned char g_SoundLoopBufferLock[24];
extern int g_SoundVolumeMillibelTable[128];
extern int g_SoundPanMillibelTable[128];
extern int g_SoundStaticPreloadIds[17];
int __cdecl SoundInit(const char *first, const char *directory, int third);
void __cdecl SoundShutdown(void);
void __cdecl MusicLoadWaveNameIndex(void);
void __cdecl SoundUpdateFades(void);
void __cdecl SfxPlayOnChannel(char pan, int soundId, int channel);
void __cdecl SfxPlayUpToFour(int pan, int soundId, int soundId2, int soundId3, int soundId4);
void __cdecl SfxPreloadAndPlayOnChannel5(int soundId);
void __cdecl SfxPauseAll(void);
void __cdecl SfxResumeAll(void);
void __cdecl SfxStopAll(void);
int __cdecl SfxLoadIntoBuffer(int soundId, int destSlot);
int __cdecl SfxDecodeToStaticBuffer(struct SoundSfxDecodeRecord *record, int size);
int __cdecl SfxLoadFormatTable(void);
int __cdecl SfxStreamStartServiceTimer(void);
void __cdecl SfxStreamStopServiceTimer(void);
void __stdcall SfxStreamServiceTimerProc(unsigned int timer, unsigned int message, unsigned long user, unsigned long one, unsigned long two);
int __cdecl MusicWaveStartServiceTimer(void);
void __cdecl MusicWaveStopServiceTimer(void);
void __stdcall MusicWaveStreamServiceTimerProc(unsigned int timer, unsigned int message, unsigned long user, unsigned long one, unsigned long two);
int __cdecl SfxStreamPrime(struct SoundStreamState *streamState, int fileHandle);
void SfxStreamClose(struct SoundStreamPlayer *stream);
int __cdecl SfxStreamOpen(int index, struct SoundSfxStream *stream);
int __cdecl MusicWaveStreamOpen(void);
int SfxStreamDecodeChunk(struct SoundStreamPlayer *stream, int fileHandle);
int __cdecl MusicWaveParseWavHeader(struct SoundWaveInfo *waveInfo);
void __cdecl MusicPlayWaveTrack(int track, int unused);
void __cdecl MusicWaveStop(void);
int __cdecl MusicWaveIsPlaying(void);
void __cdecl MusicWavePause(void);
void __cdecl MusicWaveResume(void);
void __cdecl SfxSetAllChannelsVolume(char volume);
void __cdecl SfxSetAllChannelsVolumeTrans(char volume, char transition);
void __cdecl SfxSetChannelVolume(unsigned char volume, int channel);
void __cdecl SfxSetChannelVolumeTrans(unsigned char volume, int channel, unsigned char duration);
void __cdecl SfxSetAllChannelsPan(char pan);
void __cdecl SfxSetAllChannelsPanTrans(char pan, char transition);
void __cdecl SfxSetChannelPan(unsigned char pan, int channel);
void __cdecl SfxSetChannelPanTrans(unsigned char pan, int channel, unsigned char duration);
void __cdecl SfxSetAllChannelsTranspose(char transpose);
void __cdecl SfxSetAllChannelsTransposeTrans(char transpose, char transition);
void __cdecl SfxSetChannelTranspose(char transpose, int channel);
void __cdecl SfxSetChannelTransposeTrans(char transpose, int channel, unsigned char duration);
int __cdecl SfxFrequencyFromTranspose(int transpose, int frequency);
void __cdecl SfxResetChannel(int channelNum, int pan, int sampleRate);
void __cdecl SfxSetAudioDataDirectory(char *dataDir);
void __cdecl MusicSetWaveDirectory(char *dataDir);
void __cdecl MusicWaveCrossPlay(int track, int duration);
void __cdecl MusicWaveSetVolume(unsigned char volume);
void __cdecl MusicWaveSetVolumeTrans(unsigned char targetVolume, unsigned char step);
void __cdecl MusicWaveFadeVolume(int duration, int start, int end);
void __cdecl Sfx_UnusedNoOp_1(void);
void __cdecl Sfx_UnusedNoOp_2(void);
void __cdecl Sfx_UnusedNoOp_3(void);
void __cdecl Sfx_UnusedNoOp_4(void);
void __cdecl Sfx_UnusedNoOp_5(void);
void __cdecl Sfx_UnusedNoOp_6(void);
void __cdecl Sfx_UnusedNoOp_7(void);
void __cdecl Sfx_UnusedNoOp_8(void);

/* sound_directsound_error_text.cpp */
char *__cdecl DsErrorDescription(int error);
int __cdecl DsCheckError(int error, char *file, int line);

/* sound_soundfont_open_close.cpp */
unsigned int __cdecl SoundFontOpenDevice(int midi);
void __cdecl SoundFontCloseDevice(void);
int __cdecl SoundFontLoadBank(char *path);
int __cdecl SoundFontLoadPreset(char *path, unsigned short source, unsigned short destination);
int __cdecl SoundFontClearPreset(void);

/* sound_midi_song_names.cpp */
char *__cdecl MidiGetSongName(int index);

/* sound_multimedia_error_text.cpp */
char *__cdecl MmGetErrorText(int err);
int __cdecl MmCheckError(int error, const char *file, int line);

/* sound_sfx_group_preload.cpp */
void __cdecl SfxPreloadGroupJobProc(void);
void __cdecl SfxReleaseCurrentPreloadJob(void);
void __cdecl SfxStartGroupPreload(struct SoundGroup *groups);
void __cdecl SfxPumpGroupPreload(void);
void __cdecl SfxWaitForGroupLoaded(int group);

/* sound.cpp: the sound effect and music service (0x00748950..) */
int __cdecl SfxStreamService(struct SoundStreamPlayer *player);
void __cdecl SfxPlayCachedBuffer(int id, unsigned char pan, unsigned char volume, unsigned char trans);
void __cdecl SfxRetriggerCachedBuffer(int soundId);
void __cdecl SfxStopAndRewindCachedBuffer(int soundId);
void __cdecl SfxStopCachedBuffer(int soundId);
void __cdecl SfxResumeCachedBuffer(int soundId);
void __cdecl SfxSetCachedBufferPan(int sound, unsigned char pan);
void __cdecl SfxSetCachedBufferTranspose(int soundId, int transpose);
void __cdecl SfxSetMasterVolumePercent(int value);
void __cdecl MusicSetMasterVolumePercent(int percent);
int __cdecl SfxGetMasterVolumePercent(void);
int __cdecl MusicGetMasterVolumePercent(void);
int __cdecl Sfx_UnusedReturnZero_1(void);
int __cdecl Sfx_UnusedReturnZero_2(void);
void __cdecl Sfx_UnusedNoOp_9(void);
void __cdecl SfxRegisterSoundGroups(struct SoundGroup **groups);
void __cdecl SfxPlaySoundGroup(char pan, int group);
int __cdecl SfxIsSoundPlaying(int soundId);
void __cdecl SfxStreamCloseLocked(void);
void __cdecl MusicWaveStreamCloseLocked(void);
void __cdecl MusicReadWaveMusicRegistrySetting(void);
int __cdecl SfxIsAnyChannelPlaying(void);
void __cdecl SfxBuildLoopBuffer(struct SoundRecordEntry *metadata, struct SoundChannel *channel);
void __cdecl SfxSaveChannelLoopRecords(void);
void __cdecl SfxRestoreChannelLoopRecords(void);
void __cdecl SfxClearChannelLoopRecords(void);

/* sound.cpp */
void __cdecl Sfx_ReleaseCachedBuffer(int soundId, int groupId);

struct SoundAcmStreamHeader;

/* acm.cpp */
int __cdecl AcmStreamPrepare(int stream, struct SoundAcmStreamHeader *header, int source, int sourceBytes, int destination, int destinationBytes);

/* acm.cpp */
int __cdecl AcmStreamOpen(int output, int sourceFormat, int destinationFormat, int mode);
void __cdecl AcmStreamConvert(int stream, void *header, int mode);

/* sound.cpp */
extern const char s_ff7RegistryKey[];

int __cdecl SfManLoadBankFromFile(unsigned short device, unsigned short bankIndex, char *path);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_SOUND_H */

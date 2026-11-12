/* include/lib/types/sound.h: the types lib/sound TUs share.
 * One definition per type. */
#ifndef LIB_TYPES_SOUND_H
#define LIB_TYPES_SOUND_H

typedef struct SfManPresetRef {
    unsigned short bank, preset;
} SfManPresetRef;

typedef struct SfManFileRequest {
    unsigned int size;
    char *path;
    unsigned int unused;
    unsigned short zero;
} SfManFileRequest;

typedef struct SfManFile {
    unsigned int size;
    char *path;
    unsigned int reserved;
    unsigned short reserved2;
} SfManFile;

typedef SfManPresetRef SfManBank;

typedef int(__cdecl *SfManLoadBankProc)(unsigned short, SfManBank *, SfManFile *);

typedef struct SfManApi {
    unsigned char reserved[0x3c];
    int(__cdecl *copyPresetFromFile)(unsigned short, SfManPresetRef *, SfManPresetRef *, SfManFileRequest *);
} SfManApi;

#pragma pack(push, 2)
typedef struct SfManNameRequest {
    unsigned int size;
    char *path;
    unsigned int reserved;
    unsigned short zero;
} SfManNameRequest;
#pragma pack(pop)

typedef int(__cdecl *SfManagerProc)(int version, int **api);

struct SoundGroup {
    int sound[4];
};

/* Wave stream state opened on the sound buffer; its header is parsed by MusicWaveParseWavHeader. */
typedef struct SoundWaveStream {
    int dsBuffer;
    int wavBuffer;
    int state;
    unsigned char pad_0C[0x10];
    int chunkDecoded;
    int dataSize;
    int bytesQueued;
    int writePos;
    int cursor;
    int looping;
    int loopTailDone;
    unsigned char pad_38[0x64];
    int primed;
    int bytesWritten;
    unsigned char pad_A4[0x8];
    int loopWrapped;
} SoundWaveStream;

/* Sound stream priming state: DirectSound buffer, ACM stream and chunk buffers (SfxStreamPrime). */
typedef struct SoundStreamState {
    int dsBuffer;
    union {
        struct SoundStreamSource *source;
        int filePos;
    };
    int state;
    int bufferSize;
    int chunkSize;
    unsigned char pad_14[0x8];
    int chunkDecoded;
    int readPos;
    int lastWritePos;
    int writePos;
    unsigned char pad_2C[0xc];
    int srcBuffer;
    int srcBytes;
    int pcmBuffer;
    int acmStream;
    unsigned char pad_48[0x24];
    int pcmBytesDecoded;
    unsigned char pad_70[0x2c];
    int primed;
    int bytesWritten;
} SoundStreamState;

/* Live sound stream: DirectSound buffer, write position and decode bookkeeping (SfxStreamService). */
typedef struct SoundStreamPlayer {
    int dsBuffer;
    union {
        struct SfxStreamSource *source;
        struct SoundStreamSource *streamSource;
    };
    int state;
    int bufferSize;
    int chunkSize;
    int isOpen;
    int isPaused;
    int chunkDecoded;
    int bytesToRead;
    int bytesQueued;
    int writePos;
    int cursor;
    int looping;
    int loopTailDone;
    int srcBuffer;
    int readLength;
    int pcmBuffer;
    int acmStream;
    unsigned char pad_48[0x24];
    int chunkBytes;
} SoundStreamPlayer;

/* Sound effect stream state opened from a 0x1C-byte sfx table entry. */
typedef struct SoundSfxStream {
    int dsBuffer;
    int entryPtr;
    int state;
    unsigned char pad_0C[0xc];
    int isPaused;
    int chunkDecoded;
    int bytesToRead;
    int bytesQueued;
    int writePos;
    int cursor;
    int looping;
    int loopTailDone;
    unsigned char pad_38[0x40];
} SoundSfxStream;

/* Sound effect decode record read by SfxDecodeToStaticBuffer */
typedef struct SoundSfxDecodeRecord {
    int allocSize;
    unsigned char pad_04[0x4];
    unsigned char looping;
    unsigned char pad_09[0xb];
    int endOffset;
} SoundSfxDecodeRecord;

/* One of the five sound effect channels (g_SoundChannels.channels, 0x54 bytes). Volume, pan and
 * transpose fade alike (SoundUpdateFades): a ...Trans call stores the target and a rate in steps
 * per second, the first update after it only sets the fade's started flag, and each later one adds
 * elapsed ms * rate / 1000 until the value reaches the target. Channel 1 also drives the streamed
 * effect. */
typedef struct SoundChannel {
    int volumeFadeStarted; /* +0x00 */
    int volume; /* +0x04: 0..0x7F, an index into the millibel volume table */
    int targetVolume; /* +0x08 */
    int volumeFadeRate; /* +0x0C */
    int volumeFadeElapsed; /* +0x10: ms */
    int panFadeStarted; /* +0x14 */
    int pan; /* +0x18: 0..0x7F, an index into the millibel pan table */
    int targetPan; /* +0x1C */
    int panFadeRate; /* +0x20 */
    int panFadeElapsed; /* +0x24 */
    int transposeFadeStarted; /* +0x28 */
    int transpose; /* +0x2C: SfxFrequencyFromTranspose(transpose, sampleRate) is the playback rate */
    int targetTranspose; /* +0x30 */
    int transposeFadeRate; /* +0x34 */
    int transposeFadeElapsed; /* +0x38 */
    int sampleRate; /* +0x3C: the playing sound's own rate */
    int soundId; /* +0x40: the sound the buffers hold */
    int buffer; /* +0x44: the DirectSound buffer it plays from */
    int loopBuffer; /* +0x48: its looped part, when it has one (SfxBuildLoopBuffer) */
    int loopSwitchEnabled; /* +0x4C: SfxLoopSwitchTimerProc moves to loopBuffer while it is set */
    int savedSoundId; /* +0x50: the sound SfxSaveChannelLoopRecords keeps to restart */
} SoundChannel;

/* Streamed source region: start and end offsets within the stream buffer. */
typedef struct SoundStreamSource {
    int dataBytes;
    unsigned char pad_04[0x4];
    unsigned char looping;
    unsigned char pad_09[0x3];
    int filePos;
    int startOffset;
    int endOffset;
    union {
        int sourceFormat;
        struct SoundStreamSourceInfo *formatInfo;
    };
} SoundStreamSource;

/* One of the six 0x448-byte MIDI stream buffers (midiStreamBuffers): a MIDIHDR (lpData,
 * dwBufferLength, dwBytesRecorded, dwUser, dwFlags; MidiSubmitStreamBuffer points it at `data` and
 * itself), 0x400 bytes of stream events, the buffer's index (MidiStartPlayback) and the number of
 * event bytes MidiFillStreamBuffer has written. */
typedef struct MidiStreamBuffer {
    int dataPtr;
    int dataSize;
    int bytesRecorded;
    int bufferBase;
    int flags;
    unsigned char pad_14[0x2c];
    unsigned char data[0x400];
    int index;
    int filledBytes;
} MidiStreamBuffer;

/* ACM stream header holding the source and destination buffers and their byte counts. */
typedef struct SoundAcmStreamHeader {
    int structSize; /* +0x0 */
    unsigned char pad_4[0x8];
    int source;
    int sourceBytes;
    unsigned char pad_14[0x8];
    int destination;
    int destinationBytes;
} SoundAcmStreamHeader;

/* Per-sound loop record indexed with stride 0x1C in SfxPlayOnChannel. */
typedef struct SfxLoopInfo {
    unsigned char pad_00[0x10];
    int loopInterval;
    unsigned char pad_14[0x4];
    int waveFormat;
} SfxLoopInfo;

/* Global SFX stream object whose buffer handle is resumed by SfxResumeAll. */
typedef struct SfxStreamObject {
    int bufferHandle;
    unsigned char pad_04[0x14];
    int isPaused;
} SfxStreamObject;

/* Stream source block behind SoundStreamPlayer.source, holding the file position and chunk state. */
typedef struct SfxStreamSource {
    int dataSize;
    int dataStart;
    unsigned char pad_08[0x4];
    int filePosition;
    int loopOffset;
    int decodedSize;
    int formatData;
} SfxStreamSource;

/* MIDI chunk header: the MThd or MTrk id followed by the big-endian chunk length. */
typedef struct SoundMidiChunkHeader {
    int chunkId;
    unsigned int chunkLength;
} SoundMidiChunkHeader;

/* Big-endian MThd header words read by MidiStartPlayback: format, track count, division. */
typedef struct SoundMidiHeaderWords {
    unsigned short format;
    unsigned short trackCount;
    unsigned short division;
} SoundMidiHeaderWords;

/* Stream source info: a format selector and a rate divisor used to size stream chunks. */
typedef struct SoundStreamSourceInfo {
    unsigned short format;
    unsigned char pad_02[0xa];
    unsigned short rateDivisor;
} SoundStreamSourceInfo;

/* A song track's play state (0x1C bytes; midiTrackState and the loop's saved copy). MidiStartPlayback
 * fills it from the MTrk chunk: its length, its data (MidiFileTake) and the read cursor there, the
 * tick of its first event; MidiConvertNextEvent reads the next event at the cursor, keeps the
 * running status, adds each delta to the tick and sets `ended` at the end-of-track event, and
 * MidiSelectNextTrack plays the unfinished track with the smallest tick next. */
typedef struct MidiTrack {
    int length;
    int data;
    unsigned char *cursor;
    int tick;
    unsigned char runningStatus;
    unsigned char pad_11[0x3];
    int ended;
    int index;
} MidiTrack;

/* WAV header record reached through the wave info pointer at +0x18. */
typedef struct MusicWaveInfoHeader {
    unsigned char pad_00[0x2];
    unsigned short channels;
    unsigned char pad_04[0x8];
    unsigned short blockAlign;
    unsigned char pad_0E[0x2];
    short extraValue;
} MusicWaveInfoHeader;

/* 28-byte per-sound record whose loop flag is passed to DsPlayBuffer. */
typedef struct SoundRecordStreamFlag {
    unsigned char loopFlag;
    unsigned char pad_01[0x1b];
} SoundRecordStreamFlag;

/* Sound effect data entry read by SfxStreamOpen; filePosition is a file position. */
typedef struct SoundSfxEntry {
    int dataSize;
    int filePosition;
    unsigned char looping;
} SoundSfxEntry;

/* One enumerated DirectSound device: heap copies of its GUID, description and module name, freed on release. */
typedef struct SoundDsDeviceSlot {
    void *buffer; /* the GUID copy */
    char *description;
    char *module;
} SoundDsDeviceSlot;

/* Sound record table entry (0x1C bytes), indexed by sound id minus one: the data size, the file position,
 * the loop flag DsPlayBuffer takes, the loop interval and the sound's WAVEFORMATEX. */
typedef struct SoundRecordEntry {
    int dataSize;
    int filePosition;
    unsigned char loopFlag;
    unsigned char pad_09[0x7];
    int loopStart;
    int loopEnd;
    void *waveFormat;
} SoundRecordEntry;

/* Cached sample info entry; its data pointer is freed at shutdown. */
typedef struct SoundCachedSample {
    int data;
    unsigned char pad_04[0x18];
} SoundCachedSample;

/* Wave buffer header read when a music wave stream opens. */
typedef struct SoundWavBufferHeader {
    int dataSize;
    unsigned char pad_04[0x4];
    unsigned char looping;
} SoundWavBufferHeader;

/* Parsed WAV stream info for the music file: file position, data size, scaled size and buffer. */
typedef struct SoundWaveInfo {
    int chunkSize; /* +0x0 */
    int filePos;
    char ready;
    unsigned char pad_09[0x3];
    int dataSize;
    int scaledSize;
    int rawSize;
    int buffer;
} SoundWaveInfo;

#pragma pack(push, 1)
typedef struct SoundChannels {
    int initialized; /* +0x0 */
    int primaryBuffer; /* +0x4 */
    /* +0x8: the DirectSound buffer of each sound id. The callers range-check ids to 0..0x2EE, one past
     * the 750 the release loops clear: id 0x2EE reads the first word of the first channel. */
    int bufferHandles[750];
    SoundChannel channels[5]; /* +0xbc0 */
    int soundRecords; /* +0xd64 */
    int channelFileOffset; /* +0xd68 */
    int streamFlags; /* +0xd6c */
    unsigned char pad_d70[0x4];
    int recordLoopCount; /* +0xd74 */
    unsigned char pad_d78[0x4];
    int cachedSamples; /* +0xd7c */
    unsigned char pad_d80[0x18];
    int resetFormatRecordPtr; /* +0xd98 */
    unsigned char pad_d9c[0x51cc];
    int trackNames; /* +0x5f68 */
    int trackNameBuffers; /* +0x5f6c */
    unsigned char pad_5f70[0x184];
    char wavePath; /* +0x60f4 */
    unsigned char pad_60f5[0xa3];
    int track; /* +0x6198 */
} SoundChannels;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct SoundData {
    unsigned char pad_0[0x14];
    int musicStreamBufferPtr; /* +0x14 */
    int musicStreamObject; /* +0x18 */
    unsigned char pad_1c[0x10];
    int musicPlayingFlag; /* +0x2c */
    int musicPausedFlag; /* +0x30 */
    unsigned char pad_34[0x9c];
    int currentSoundId; /* +0xd0 */
    int sfxStreamObject; /* +0xd4 */
    unsigned char pad_d8[0x10];
    int sfxStreamActiveFlag; /* +0xe8 */
    int sfxPauseAllFlag; /* +0xec */
    unsigned char pad_f0[0x9c];
    int sfxDataFileHandle; /* +0x18c */
    int musicFileHandle; /* +0x190 */
    int volumeFadeActiveFlag; /* +0x194 */
    unsigned char pad_198[0x100];
    int musicMasterVolumePercent; /* +0x298 */
    int masterVolume; /* +0x29c */
    int registryOptionsWord; /* +0x2a0 */
    int panInvertScale; /* +0x2a4 */
} SoundData;
#pragma pack(pop)

#endif /* LIB_TYPES_SOUND_H */

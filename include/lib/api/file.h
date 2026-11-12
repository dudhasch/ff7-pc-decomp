/* include/lib/api/file.h: the functions and objects lib/file defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_FILE_H
#define LIB_API_FILE_H

#ifdef __cplusplus
extern "C" {
#endif
struct Battle3dDescriptorFile;
struct GfxEngineContext;
struct FileHandle;
struct FileRecord;
struct FileDirectoryListing;
struct FileOpenRequest;
struct GfxTextureRecord;
struct InputMouseState2;
struct JoystickOutput;

/* direct.cpp */
int __cdecl IsNotDotOrDotDotEntry(char *name);
void __cdecl FreeDirectoryListing(struct FileDirectoryListing *listing);
int __cdecl AllocDirectoryListing(int entryCount);
int __cdecl CountDirectoryEntries(int mode, const char *pattern);
int __cdecl BuildDirectoryListing(int mode, const char *pattern);
void __cdecl FillDirectoryEntryPointerArray(int pointers, struct FileDirectoryListing *listing);
int __cdecl AllocDirectoryEntryPointerArray(struct FileDirectoryListing *listing);
int __cdecl CompareStringPointersCaseInsensitive(void *leftPtr, void *rightPtr);
void __cdecl SortStringPointersCaseInsensitive(int count, void *strings);
int __cdecl CompareDirectoryEntryNames(int lhs, int rhs);
void __cdecl SortDirectoryListingNames(struct FileDirectoryListing *listing);
void __cdecl DebugDumpDirectoryListing(int mode);

/* file.cpp */
int __cdecl GetFileDebugLogEnabled(void);
void __cdecl SetFileDebugLogEnabled(int enabled);
void __cdecl EnableFileDebugLog(int logPath);
void __cdecl DisableFileDebugLog(void);
struct FileHandle *__cdecl OpenFileEx(struct FileOpenRequest *request, const char *fileName);
int __cdecl WriteFileData(int size, const void *buffer, struct FileHandle *handle);
int __cdecl ReadFileIntoNewBuffer(int size, int count, struct FileHandle *file);
int __cdecl GetFileSizeEx(void *handle);
void *__cdecl LoadWholeFileEx(void *context, int extra, int *size, char *name);
void *__cdecl LoadDiskFileEx(int extra, void *sizeOut, void *fileName);
void *__cdecl LoadDiskFile(void *sizeOut, void *fileName);
int __cdecl WriteWholeFile(int size, char *fileName, void *data);
void __cdecl FileFreeRecordCallback(int value);
int __cdecl FileRecordMatchesKindNameDirectory(struct FileRecord *recordA, struct FileRecord *recordB);
void __cdecl FileFreeRecord(struct FileRecord *record);
struct FileRecord *__cdecl FileCreateRecord(int kind, char *name, char *directory);
void __cdecl FileRecordMatchesCompare(struct FileRecord *lhs, struct FileRecord *rhs);
int __cdecl IndexedEntryKeyMatches(int index, int key, void *table);
void __cdecl StripFileExtension(const char *src, char *dst);
void __cdecl MakeIndexedAssetName(char extChar, int index, const char *fileName, char *out);
void __cdecl MakeExtensionAssetName(const char *ext, const char *fileName, char *out);

/* file_input_poll_bindings.cpp */
extern int g_InputCursorX;
extern int g_InputCursorY;
extern int g_InputRepeatFirstDelay;
extern unsigned char *g_InputKeyboardStateBuffer;
extern struct GfxEngineContext *g_InputContext;
extern int g_InputHeldMask;
extern int g_InputMouseButtonFlags;
extern struct InputMouseState2 *g_InputMouseStatePointer;
extern int g_InputPressedMask;
extern int DAT_009a85e4;
#define g_InputRepeatInterval DAT_009a85e4
extern int DAT_009a85e8[3][25];
#define g_InputBindings DAT_009a85e8
extern int g_InputRepeatNextThreshold;
extern int g_InputTimeGetTimeSample;
extern struct JoystickOutput *g_InputJoystickPollTable;
extern int g_InputLastTickSample;
extern int g_InputMouseRepeatMask;
extern int g_InputRepeatActiveFlag;
extern int g_InputRepeatElapsedTicks;
extern int g_InputRepeatStateFlag;
extern int g_InputMouseDeltaAccumulator;
extern int g_InputMouseDeltaSecondAccumulator;
void __cdecl Input_Init(struct GfxEngineContext *context);
void __cdecl Input_Shutdown(void);
void __cdecl Input_SetBinding(int device, unsigned int mask, int binding);
void __cdecl Input_LoadAllDefaults(void);
void __cdecl Input_LoadKeyboardDefaults(void);
void __cdecl Input_LoadJoystickDefaults(void);
int __cdecl Input_GetHeld(int mask);
int __cdecl Input_GetPressed(int mask);
int __cdecl Input_CaptureAnySource(void);
int __cdecl Input_PollMouseBuffered(void);
int __cdecl Input_GetMouseDirMask(void);
int __cdecl Input_GetRepeat(int mask);
int __cdecl Input_GetMouseRepeatMask(void);
void __cdecl Input_SetRepeatRates(int first, int repeat);
void __cdecl Input_GetRepeatRates(int *first, int *repeat);
void __cdecl Input_TickRepeat(void);
void __cdecl Input_ResetRepeatState(void);
int __cdecl Input_IsMouseButtonDown(int button);
int __cdecl Input_IsKeyDown(unsigned int key);
int __cdecl Input_GetCursorPos(void);
void __cdecl Input_SetCursorPos(int x, int y);
void __cdecl Input_ResetRepeatClock(void);

/* is_lib.cpp */
void __cdecl SetCurrentLgpArchiveId(int archiveIndex);
int __cdecl GetCurrentLgpArchiveId(void);
struct _iobuf;
struct _iobuf *__cdecl FileOpenStdio(char *fileName, char *mode);
short __cdecl FileCloseStdio(struct _iobuf *stream);
void __cdecl FileSeekStdio(struct _iobuf *stream, int offset, int origin);
int __cdecl FileTellStdio(struct _iobuf *stream);
int __cdecl FileLengthStdio(struct _iobuf *stream);
int __cdecl FileReadU8Stdio(struct _iobuf *stream, unsigned char *dst);
int __cdecl FileReadU16Stdio(struct _iobuf *stream, unsigned short *dst);
int __cdecl FileReadU32Stdio(struct _iobuf *stream, unsigned int *dst);
short __cdecl FileReadBytesStdio(struct _iobuf *stream, void *buffer, int size);
int __cdecl FileOpenLL(char *fileName, int flags);
short __cdecl FileCloseLL(int fd);
void __cdecl FileSeekLL(int fd, int offset, int origin);
int __cdecl FileTellLL(int fd);
int __cdecl FileLengthLL(int fd);
int __cdecl FileReadU8(int fd, unsigned char *dst);
int __cdecl FileReadU16(int fd, unsigned short *dst);
int __cdecl FileReadU32(int fd, unsigned int *dst);
short __cdecl FileReadBytes(int fd, void *buffer, int size);
int __cdecl FileWriteBytesStdio(struct _iobuf *file, void *buffer, int size);
int __cdecl FileWriteU8Stdio(struct _iobuf *file, unsigned char value);
int __cdecl FileWriteU16Stdio(struct _iobuf *file, unsigned short value);
int __cdecl FileWriteU32Stdio(struct _iobuf *file, unsigned int value);
int __cdecl ReadLgpToc(int handle, int lgp);
void __cdecl ReadLgpConflictTable(int fd, int archiveIndex);
int __cdecl SetLgpLookupDirPrefix(char *name);
int __cdecl ClearLgpLookupDirPrefix(void);
char __cdecl GetLgpLookupCharCode(char character);
int __cdecl CloseLgpArchive(int archiveIndex);
void __cdecl FreeLgpConflictTable(int archiveIndex);
int __cdecl CloseAllLgpArchives(void);
void __cdecl FreeAllLgpConflictTables(void);
int __cdecl GetLgpArchiveFd(int archiveIndex);
int __cdecl SeekLgpArchive(int offset, int archiveIndex);
int __cdecl SeekLgpArchiveRelative(int offset, int archiveIndex);
int __cdecl ReadLgpArchiveStream(int archiveIndex, void *buffer, int size);

/* registry.cpp */
void __cdecl FileRegistryDestroy(int registry);
int __cdecl FileRegistryCreate(int freeValue);
int __cdecl ListFindValueByCompareInHolder(int key, int(__cdecl *compare)(int key, int value), int holder);
void __cdecl ListAppendValueToHolder(int value, int holder);
void __cdecl ListRemoveValueFromHolder(int value, int holder);
void __cdecl ListForEachEntryInHolder(int(__cdecl *visit)(int value, int arg), int arg, int holder);

/* smcdfile.cpp */
int __cdecl LoadFileBlock(int fileId, int offset, int size, void **outBuffer, void(__cdecl *callback)(void));
int __cdecl LoadFileBlockLzs(int fileId, int uncompressedSize, void **outBuffer, void(__cdecl *callback)(void));

/* direct.cpp */
int __cdecl PushFileContextDirectory(struct FileOpenRequest *context, void *dir, void *saveBuf);

/* file.cpp */
void * __cdecl LoadWholeFile(void *context, int *sizeOut, char *fileName);

/* file_input_poll_bindings.cpp */
void __cdecl Input_Update(struct GfxEngineContext *ctx);

/* file.cpp */
void __cdecl InitFileContext(int mode, void *context);

struct FileHandle;

/* file.cpp */
void *__cdecl ReadFileIntoNewBufferEx(int extra, int size, int count, struct FileHandle *file);
void __cdecl SetFilePosition(struct FileHandle *file, int position);

/* direct.cpp */
int __cdecl ChangeDirectorySaving(char *dir, char *flags);
/* file.cpp */
void __cdecl CloseFileEx(struct FileHandle *file);
int __cdecl OpenFileSimple(int mode, void *fileName);
int __cdecl ReadFileData(int size, void *buffer, struct FileHandle *file);
int __cdecl ReadFileDataPartial(int size, void *buffer, struct FileHandle *file);
int __cdecl GetFilePosition(struct FileHandle *file);
/* is_lib.cpp */
int __cdecl OpenLgpArchive(char *name, int lgp);
void *__cdecl FindLgpFile(char *name, int lgp);
int __cdecl GetLgpEntrySize(int offset, int archiveIndex);
int __cdecl ReadLgpEntry(void *file, int lgp, void *buf, int size);
/* registry.cpp */
int __cdecl ListFindPayloadInHolder(int context, void *key, int(__cdecl *arg3)(int, int, int), int holder);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_FILE_H */

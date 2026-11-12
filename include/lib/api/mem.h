/* include/lib/api/mem.h: the functions and objects lib/mem defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_MEM_H
#define LIB_API_MEM_H

#ifdef __cplusplus
extern "C" {
#endif
struct MemBlockHeap;
struct Timestamp64;
struct GfxRenderObject;
struct GfxSpriteBatch;
struct MemBlockArrayHeap;
struct MemHeapBlock;
struct MemHeapTracker;
struct GameHeapTracker;
/* Retail .bss slot 0x00d8d480 sits in the snowboard .bss run; declared here so lib/mem stays self-contained. */
extern struct GameHeapTracker g_MemHeapTrackerStorage;

/* heap.cpp */
void __cdecl DestroyBlockHeapChain(struct MemBlockHeap *heap);
int __cdecl CreateBlockHeapEx(int blockSize, int blockCount, int depth, struct GfxSpriteBatch *owner, int callback);

/* mem.cpp */
void __cdecl DestroyGameHeapTracker(struct GameHeapTracker *tracker);
struct GameHeapTracker *__cdecl CreateGameHeapTracker(void);
void __cdecl SetGameHeapTrackerEnabledOn(int value, struct GameHeapTracker *tracker);
void __cdecl FreeGameHeapTrackRecord(void *record);
void __cdecl GameHeapReportOOM(int count, int size, char *file, int line);
/* Tracked allocation. `file` and `line` are the caller's __FILE__/__LINE__, passed as
 * immediates by every caller in the shipped image. Proven by 0x00720efd..0x00720f0e
 * (Menu_SaveFileBufferAlloc). */
void *__cdecl GameHeapAlloc(int count, int size, char *file, int line);
/* Proven by 0x00720f3b..0x00720f4b (Menu_SaveFileBufferFree). */
void __cdecl GameHeapFree(void *block, char *file, int line);
/* The untracked allocator underneath GameHeapAlloc: the same __FILE__/__LINE__ tail, no
 * element count. Proven by 0x0075ab53..0x0075ab5f (World_AllocArena). */
void *__cdecl GameHeapAllocRaw(int size, char *file, int line);
void __cdecl ZeroFillMemory32(unsigned int size, void *dst);
void __cdecl DumpGameHeapBlocks(struct MemHeapTracker *tracker);
int __cdecl PrintGameHeapBlockCallback(struct MemHeapBlock *blockInfo, void *total);
void __cdecl ReportGameHeapUsage(struct MemHeapTracker *tracker);
int __cdecl SumGameHeapBlockCallback(struct MemHeapBlock *blockInfo, void *total);
void __cdecl SetGameHeapTrackingEnabled(int value);
void __cdecl DumpGameHeapLeaks(void);
void __cdecl PrintGameHeapUsage(void);
void *__cdecl GameHeapMemDup(int size, void *source);
char *__cdecl GameHeapStrDup(char *text);
int __cdecl UnusedReturnZero(void);

/* mem_block_heap_pool.cpp */
int __cdecl CreateBlockHeap(int blockSize, int blockCount, int depth);
int __cdecl CreateBlockHeapCount(int count, int size, int depth);
void __cdecl ResetBlockHeapChain(struct MemBlockHeap *heap);
void __cdecl TrimBlockHeapOverflow(struct MemBlockHeap *heap);
int __cdecl AllocBlockFromHeap(struct MemBlockHeap *heap);
int __cdecl FreeBlockToHeap(void *block, struct MemBlockHeap *heap);
int __cdecl AllocBlockArrayFromHeap(int count, struct MemBlockArrayHeap *heap);

/* mem_fill_copy_routines.cpp */
void __cdecl Mem_FillBytes(int value, int byteCount, int dst);
int __cdecl Mem_FillWordsFast(int value, int count, int destination);
int __cdecl Mem_FillBytesFast(int value, int count, int destination);
unsigned char *__cdecl Mem_FillRgbTriplets(unsigned char r, unsigned char g, unsigned char b, int count, unsigned char *destination);
void __cdecl Mem_FillDwordsByByteCount(int value, int byteCount, int dst);
void __cdecl Mem_CopyBytesFast(int count, void *dst, const void *src);
void __cdecl Mem_CopyWordsAlignedByByteCount(int byteCount, void *destination, void *source);
void __cdecl Mem_CopyDwordsByByteCount(int byteCount, void *destination, void *source);
void __cdecl Mem_CopyDwords(int dwordCount, void *destination, void *source);
void *__cdecl Mem_FillByteRows(int value, int width, int rows, int stride, void *destination);
void *__cdecl Mem_FillWordRows(int value, int width, int rows, int stride, void *destination);
int __cdecl Mem_FillRgbRows(int red, int green, int blue, int width, int height, int stride, int destination);
void *__cdecl Mem_FillDwordRows(int value, int width, int rows, int stride, void *destination);
unsigned char *__cdecl Mem_CopyByteRows(int width, int rows, int sourceStride, int destinationStride, unsigned char *source, unsigned char *destination);
unsigned short *__cdecl Mem_CopyWordRows(int width, int rows, int sourceStride, int destinationStride, unsigned short *source, unsigned short *destination);
int __cdecl Blit_MultiplyByThree(int value);
int __cdecl Blit_ReplicateByteToDword(int value);
int __cdecl Blit_ReplicateWordToDword(int value);

/* mem_timestamp_int64_math.cpp */
void __cdecl ReadTimestampCounter(struct Timestamp64 *value);
void __cdecl Add64(struct Timestamp64 *addend, struct Timestamp64 *accumulator);
void __cdecl Sub64(struct Timestamp64 *left, struct Timestamp64 *right, struct Timestamp64 *result);
int __cdecl Compare64(struct Timestamp64 *keyA, struct Timestamp64 *keyB);
double __cdecl CombineSplitValueToFloat(struct Timestamp64 *value);
void __cdecl SplitFloatToSplitValue(double value, struct Timestamp64 *parts);
double __cdecl Timer_CalibrateTscFrequency(struct Timestamp64 *result);

/* mem_fill_copy_routines.cpp */
void *__cdecl Mem_CopyDwordRows(int byteWidth, int rows, int sourceStride, int destinationStride, void *source, void *destination);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_MEM_H */

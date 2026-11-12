# Library (lib): the engine code under the game modules

`src/lib/src/` holds the TUs whose retail `__FILE__` strings start with `C:\lib\src\`, and the
TUs placed between them: rendering, sound, files, models, movies, and eight small helper libraries. None of it is a game module. The game
modules in `src/ff7/` call into it on entry, per frame and on exit. TU boundaries follow retail's
objects. A TU without a retail `__FILE__` string has a descriptive name, and its place is inferred from
its link-order neighbours.

## Subsystems with their own README

| subsystem | README | TUs | retail / descriptive names | functions |
|---|---|---:|---:|---:|
| lib/graphics | [graphics](graphics/README.md) | 67 | 29 / 35 | 1,208 |
| lib/sound | [sound](sound/README.md) | 13 | 5 / 7 | 203 |
| lib/polygon | [polygon](polygon/README.md) | 6 | 5 / 1 | 165 |
| lib/file | [file](file/README.md) | 6 | 5 / 1 | 117 |
| lib/movie | [movie](movie/README.md) | 2 | 1 / 1 | 25 |

The eight below have no README of their own.

| subsystem | TUs | retail / descriptive names |
|---|---:|---:|
| lib/mem | 5 | 2 / 3 |
| lib/input | 1 | 1 / 0 |
| lib/list | 1 | 1 / 0 |
| lib/token | 1 | 1 / 0 |
| lib/thread | 1 | 1 / 0 |
| lib/sort | 1 | 1 / 0 |
| lib/stack | 1 | 1 / 0 |
| lib/trans | 1 | 1 / 0 |


## mem

| TU | span | functions |
|---|---|---:|
| `mem/mem.cpp` | 0x0065fac0-0x00660370 | 19 |
| `mem/mem_timestamp_int64_math.cpp` | 0x00660370-0x00660540 | 7 |
| `mem/heap.cpp` | 0x006a5440-0x006a5630 | 2 |
| `mem/mem_block_heap_pool.cpp` | 0x006a5630-0x006a5a70 | 7 |
| `mem/mem_fill_copy_routines.cpp` | 0x0069bc10-0x0069bf90 | 19 |

- Game heap. `GameHeapAlloc` (0x0065ff59, count, size, file, line), `GameHeapAllocRaw` (0x0065fda1)
  and `GameHeapFree` (0x0065fb40) are called from every subsystem. Each call passes the caller's
  `__FILE__` and line. With tracking off they are `calloc`, `malloc` and `free`. With
  tracking on, an allocation gets 0x20 extra bytes, 16 guard bytes `0x4D + k` before and after the
  data, and a 0x18-byte record on a list made by `CreateListCrtHeap`; `GameHeapFree` checks both
  guards and reports `MEMORY ERROR: ...` through `Gfx_DebugPrint`.
- The tracker: `g_MemHeapTracker` (0x0090a838) points at the 8-byte object at 0x00d8d480 (enable
  word, list). `SetGameHeapTrackingEnabled` is called by `App_Bootstrap`, `Field_InitEngineContext`
  and the chocobo and condor engine setup; `DumpGameHeapLeaks` by `App_Shutdown`;
  `PrintGameHeapUsage` by `App_InitSubsystems` and `App_Shutdown`.
- Block heaps. `CreateBlockHeapEx` (0x006a5498) allocates a 0x3C-byte header, a 32-byte-aligned
  buffer and a free-index stack of bytes, shorts or ints by block count. `AllocBlockFromHeap`
  takes a freed index first, then the bump cursor, then a chained heap it creates on demand with
  depth + 1. `FreeBlockToHeap` pushes the block's index; its rewind branch compares
  offset + block size with `block - cursor`, which a block below the cursor never meets.
  Users: the graphics draw pools, sorted draw queue and primitive mesh bank, and `ThreadSystemCreate`.
- Timer. `ReadTimestampCounter` (0x00660370) is `RDTSC` in `__asm` (`_emit 0x0F, 0x31`); `Add64`
  and `Sub64` are `__asm` too. `Timer_CalibrateTscFrequency` busy-waits over 1,000 ms of
  `timeGetTime` and returns the TSC delta; `AllocEngineContext` calls it. The frame limiters of
  battle, field, menu and credits use these.
- Types (`include/lib/types/mem.h`): `MemBlockHeap` and `MemBlockArrayHeap` are two views of the
  block-heap header; `MemHeapBlock` types the tracking record.
- Still raw: `MemBlockHeap.blockCount` (+0x0C) holds a byte size (`CreateBlockHeapCount` passes
  count * size), and `MemHeapBlock.index` holds the element count. `heap.cpp` and `FreeBlockToHeap`
  read the header by raw offsets.
- Fill and copy. `mem_fill_copy_routines.cpp` holds 19 `__asm` fill and copy routines (`Mem_FillBytes`,
  `Mem_CopyBytesFast`, row fills and copies, `Blit_*`). `Submarine_InitStateAndTables` makes 39 of
  the calls; polygon, field, condor and graphics make the rest. `graphics/graphics_palette_row_fill.cpp` holds the four colour-row helpers called by `lib/graphics`.

## list

| TU | span | functions |
|---|---|---:|
| `list/list.cpp` | 0x0069afc0-0x0069b660 | 20 |

- A singly linked list. The 0x20-byte header holds an allocator word at +0x00 (nonzero: CRT
  `calloc`/`free`, zero: game heap), the count at +0x08, head and tail at +0x0C/+0x10, a value
  destructor at +0x14 and the last visit callback at +0x18. A node is 8 bytes: next, value.
- `CreateList` (0x0069b0b0) uses the game heap; `CreateListCrtHeap` uses the CRT, so the heap
  tracker's own list does not allocate from the heap it tracks. `ListAppendValue`,
  `ListPrependValue`, `RemoveFromList` (calls the destructor), `ForEachListEntry` (stops when the
  callback returns 0), `ListToArray`, `ListInsertSortedByCompare` (does not raise the count when it
  inserts before an existing node).
- Users: polygon (edit lists, skeleton parts), graphics (device and texture-format records, polygon
  set list), file, mem, battle.
- Typed through `List` and `ListNode` (`include/lib/types/list.h`: `useCrtHeap`, `initFlag`, `count`, `head`, `tail`, `freeValue`, `visit`).
- Adjacent TUs after `list.cpp` are `mem/mem_fill_copy_routines.cpp` (above) and `graphics/graphics_app_config_loader.cpp`. `Gfx_LoadAppConfigFile` (0x0069b660) tokenises a file whose
  first token names a second file, compares that file's tokens with `g_GfxConfigOptionNames`
  (`"p"`, `"sw"`, `"hw"`, `"full"`, `"d3d2"`, `"dll"`, `"gl"`, `"riva"`, ...) and writes the result
  into the graphics context (+0x834..+0xA90); `"dll"` installs a driver named by token 13 with
  `InstallGraphicsDriver`. It writes +0x988 = 0 after reading the `sw`/`hw` token. Called by
  `Field_InitEngineContext` and the chocobo and condor engine setup.

## stack

`stack/stack.cpp` (retail name `C:\lib\src\stack\stack.cpp`, 0x006a28f0-0x006a2ab0, 6 functions).

- A fixed-capacity stack of pointers: capacity, depth, entry array (`MatrixStack`,
  `include/lib/types/stack.h`). `MatrixStack_Create` allocates the slots; `GfxInitRenderResources`
  fills them with identity matrices for the stacks at context +0x2F4 and +0x2F8.
- `MatrixStack_Push` only raises the depth; `MatrixStack_Top` returns entry depth - 1;
  `MatrixStack_Pop` lowers the depth and returns that entry. Overflow and underflow print
  `ERROR: STACK ...` through `Gfx_DebugPrint`.
- Users: `Anim_DrawSkeletonFrame`, `Anim_BakeFrameMatrices` ([polygon](polygon/README.md)),
  `PushRotMatrix` and `PopRotMatrix`. `MatrixStack_PushValue`, which pushes a plain value onto the same
  record, is not called by name anywhere.

## sort

`sort/sort.cpp` (retail name `C:\lib\src\sort\sort.cpp`, 0x006a4f10-0x006a5440, 6 functions).

- A bucket sort for draw records. `Sort_CreateBucketTable` allocates a 0x20-byte header, an
  occupancy bitmap (one bit per bucket), first and last node arrays and a pool of 12-byte nodes
  (next, key, payload).
- `Sort_InsertNode` sets the bucket's bit and keeps the nodes of one bucket in ascending key order.
  `Sort_ForEachInOrder` visits the occupied buckets in index order and calls the callback with each
  payload; its second argument is a local that is never set.
- The only user is lib/graphics `dx_3d2d.cpp`: `Gfx_CreateSortedDrawQueue`,
  `Gfx_InsertSortedDrawRecord`, `Gfx_FlushSortedDrawQueue` (callback at driver object +0x90),
  `Gfx_ResetSortedDrawQueue`, `Gfx_DestroySortedDrawQueue`.
- `SortBucketTable` (`include/lib/types/sort.h`) is the one view of the 0x20-byte header: an
  occupancy bitmap, per-bucket first and last entries, and 0xC-byte entries handed out from
  `nextFree`. Still raw: `Sort_ForEachInOrder` takes the table as an `int`.

## thread

`thread/thread.cpp` (retail name `C:\lib\src\thread\thread.cpp`, 0x006892b0-0x00689820, 12 functions).

- A 0x24-byte thread record: kind at +0x00 (only 0, `_beginthread`, is handled), stack size at +0x04
  (0x10000 from `ThreadInitCallbackRecord`), callback at +0x08, name at +0x0C, state at +0x14, an
  owned flag at +0x18, the handle at +0x1C.
- `ThreadCreateRecordFromTemplate` heap-allocates a record from a 16-byte template.
  `ThreadStartRecord` sets state 1 and starts `ThreadRunEntryProc`, which sets 2, calls the
  callback, sets 3 and calls `_endthread`. `ThreadGetState` reads the state.
- Users: battle (`Battle_StartMagicEffectThread`, `Battle_PollMagicEffectThread`,
  `Battle_StopMagicEffectThread`) and sound (`SfxStartGroupPreload`, `SfxPumpGroupPreload`).
- The thread system (`ThreadSystemCreate`, `ThreadSystemAddThread`) keeps records in a block heap;
  nothing calls those two by name.
- Still raw: the record is typed as the graphics view `GfxRenderObject`, so the state is its
  `polygonSet` member; the system header is read by raw offsets.

## token

| TU | span | functions |
|---|---|---:|
| `token/token.cpp` | 0x006b35d0-0x006b3ca0 | 14 |

- `TokenizeTextFile` (0x006b37b7, callback, file context, file name) loads the whole file with
  `LoadWholeFileEx`, runs the optional callback on the text (polygon passes
  `Token_StripHashComments`), turns whitespace into NULs and builds an array of token pointers.
  `TokenizeTextFileFromDisk` does the same with a stack file context.
- `TokenAsString`, `TokenAsInt`, `TokenAsFloat` read token i with a bounds check;
  `TokenAsString` returns an empty string out of range. `FreeTokenFile` frees the record.
  `LoadBraceHexByteListFromFile` turns a `{ xx, xx }` list into bytes for
  `Gfx_SptConvertTextToBinaryFile`.
- Users: polygon (RSD, HRC, PLY/MAT, animation text), `Gfx_LoadAppConfigFile`, graphics.
- `PolygonTokenFile` (`include/lib/types/polygon.h`) types the 20-byte token record.
  `StringToInt` and `TokenAsInt` call LIBCMT's `atoi`.
- The TU after `token.cpp` is `graphics/graphics_apply_blend_record.cpp`: `Gfx_ApplyBlendRecordToHundred` copies five words of
  a blend record into a hundred record and `Gfx_GetBlendModeRecordCall` calls the driver object's
  +0x5C slot. Only `Pfile_SetHundredBlendMode` calls them.

## trans

`trans/trans.cpp` (retail name `C:\lib\src\trans\trans.cpp`, 0x0069f910-0x0069fca0, 3 functions).

- Each function builds a lookup table of `1 << bits` pixel values that maps a pixel to another in
  the same format (`UnpackPixelToArgb8888`, `PackArgb8888ToPixelFormat`):
  `Trans_BuildScaledLut` scales by a factor in [0, 1), `Trans_BuildDoubleBrightLutSaturate` doubles
  and saturates at 255, `Trans_BuildDoubleBrightLutWrapToZero` doubles and sets an overflowing
  channel to 0. Alpha is scaled or doubled only when the format word at +0x3C is positive
  (`Trans_BuildScaledLut` sets it to 255 otherwise); both doubling functions saturate alpha.
- Users: `Field_BuildBrightnessTables` (`src/ff7/field/src/ad_pal.cpp`: 0.5, 0.25 and both doubles, 16
  bits) and `Blit_OpsInitRenderState` (`graphics/sw/sw.cpp`).
- Still raw: channels are read through `*(int *)((char *)&r + n)`; the format is a `void *`.

## input

`input/input.cpp` (retail name `C:\lib\src\input\input.cpp`, 0x0041eea0-0x0041fa70, 21 functions).

- `DI_InputInit` (0x0041eea0) calls `DirectInputCreateA` with the instance at context +0x58, creates
  and acquires the keyboard (32 buffered events, cooperative level 6) and the mouse (64 events,
  level 5) on the window at context +0x5C, and calls `Joystick_Init`. `DI_InputShutdown` releases
  them.
- `DI_KeyboardGetState` returns the 256-byte key state; `DI_KeyboardGetNextEvent` one buffered
  event; the mouse has the same set. On `0x8007001E` (input lost) they reacquire the device.
- `Joystick_Init` reads joystick 0's caps and sets thresholds at 35% and 65% of the axis range.
  `Joystick_Poll` reads `joyGetPosEx`, copies 16 buttons and sets up, down, left and right; `right`
  is compared with `g_JoystickThresholdDown`, not `g_JoystickThresholdRight`.
- Callers: `Input_Init`, `Input_Shutdown`, `Input_Update` and two capture helpers in
  `src/lib/src/file/file_input_poll_bindings.cpp` (the game's input layer, see [file](file/README.md)),
  `Menu_DebugHotkeyPoll`, `InputConfig_CaptureSource`, `InputConfig_ScreenFrame`, `Menu_NameEntryInit`.
- Structures (in `input.cpp` only): `g_InputKeyboard` (0x009ada90), `g_InputMouse` (0x009adbf0),
  `g_JoystickDevData` (0x009adc60), `g_JoystickOutput` (0x009ade28), `g_pDirectInput` (0x009aded4).

# File: LGP archives, disk files and loaders

lib/file is the engine's file layer: it opens the game's LGP archives and looks files up in them
(`is_lib.cpp`), opens, reads and writes loose disk files (`file.cpp`), lists and changes directories
(`direct.cpp`) and keeps small list-backed registries (`registry.cpp`). It is a service with no game
mode of its own. `App_InitSubsystems` and `App_OpenCoreArchives` (ff7/main,
[../../../ff7/src/main/README.md](../../../ff7/src/main/README.md)) open the archives at startup;
`App_Shutdown` closes them with `CloseAllLgpArchives`. The folder also holds the game-wide input
layer (see below).

## Where the code is

| TU | Address | Functions | Content |
|---|---|---|---|
| `is_lib.cpp` | 0x006750e0 | 41 | LGP archives; stdio and file-descriptor read/write helpers |
| `file.cpp` | 0x00681ff0 | 30 | Open/read/write/close of disk or archive files, whole-file loads, file records, name helpers |
| `direct.cpp` | 0x0068f960 | 14 | Directory listings, `chdir` with save |
| `registry.cpp` | 0x006a9060 | 7 | A record holding a lib/list list, null-guarded list forwarders |
| `smcdfile.cpp` | 0x00419210 | 2 | `LoadFileBlock`, `LoadFileBlockLzs` |
| `file_input_poll_bindings.cpp` | 0x0041a1b0 | 23 | Game input layer over DirectInput (`Input_*`) |

6 TUs, 117 functions. 5 files keep their retail names from `__FILE__` strings; the sixth,
`file_input_poll_bindings.cpp`, is named after what it contains, and its place is inferred from its
link-order neighbours. TU boundaries follow retail's objects.

- `file_input_poll_bindings.cpp`: 140 call edges from ff7/menu, none from lib/file; most module frames call
  `Input_Update`. It sits on lib/input (`src/lib/src/input/`, `DI_*`, `Joystick_Poll`). It is a
  game-wide input layer.

## Entry points

| Function | Address | What it does | Called from |
|---|---|---|---|
| `OpenLgpArchive` | 0x00675511 | Opens archive slot 0..17, reads its TOC, 30x30 lookup table and conflict table | `App_InitSubsystems`, `App_OpenCoreArchives`, `MidiInit` (slot 11), battle, chocobo, coaster, field, menu |
| `FindLgpFile` | 0x006759d2 | Looks a name up in an archive; returns the entry (its data begins at +0x18) or 0 | chocobo, coaster, condor, credits, field, highway, movie, snobo, wm, lib/sound, `OpenFileEx` |
| `GetLgpEntrySize`, `ReadLgpEntry` | 0x006762ea, 0x0067633e | Size of an entry; read an entry into a buffer | the same modules (no `GetLgpEntrySize` from credits or wm); in lib/file only `GetFileSizeEx` (size) |
| `CloseLgpArchive`, `CloseAllLgpArchives` | 0x00675f1d, 0x00676064 | Close one slot or all 18 and free their tables | battle, chocobo, coaster, field, main, menu, `MidiShutdown`; `App_Shutdown` |
| `OpenFileEx`, `OpenFileSimple` | 0x006820d2, 0x006825d7 | Open a file by `FileOpenRequest` (disk or archive); the simple form builds the request from a mode | lib/graphics, lib/polygon, lib/sound, battle, menu, wm |
| `ReadFileData`, `ReadFileDataPartial`, `WriteFileData`, `CloseFileEx` | 0x00682601, 0x00682697, 0x00682725, 0x00682091 | Read, read without the length check, write, close | lib/sound (`audio.fmt`, `audio.dat`, `.wav`), graphics, polygon, battle, menu |
| `LoadDiskFile`, `LoadWholeFile`, `WriteWholeFile` | 0x00682a19, 0x006829fe, 0x00682a30 | Whole-file load into a new heap buffer; whole-file write | battle, highway, wm, lib/graphics |
| `ChangeDirectorySaving`, `PushFileContextDirectory` | 0x006900fc, 0x006901dd | `chdir` that saves the old directory; for an archive context set the LGP lookup prefix instead | battle, coaster, field, highway, menu, lib/graphics, lib/polygon |
| `LoadFileBlock`, `LoadFileBlockLzs` | 0x00419210, 0x004192d9 | Load a disk file (or a slice of it, or LZS-decompressed) and run a callback | battle `yama/init.cpp`, `yama/inits.cpp`, `yama/coloss.cpp`, `battle_opening_actor_setup.cpp` |
| `Input_Update`, `Input_GetPressed`, `Input_GetHeld` | 0x0041a21e, 0x0041ab74, 0x0041ab67 | Per-frame poll; pressed-this-frame and held masks | battle, chocobo, coaster, condor, field, menu, lib/graphics, world map (22, 143 and 71 sites) |

## Main structures

| Type or global | Header | What it holds |
|---|---|---|
| `FileOpenRequest` | `include/lib/types/file.h` | `mode` (0 read, 1 read text, 2 write, 3 create), `flags` (nonzero: from an archive), `archiveId`, `callback` (maps the name) |
| `FileHandle` | local to `file.cpp` | The 0x18-byte open file: name, fd or entry offset, then a copy of the request (`archive`, `archiveId`) |
| `FileRecord` | `include/lib/types/file.h` | Kind, name and directory strings (`FileCreateRecord`, `FileFreeRecord`) |
| `FileDirectoryListing` | `include/lib/types/file.h` | Entry count and the 32-byte entry array of `BuildDirectoryListing` |
| `FileLgpLookupDir` | same | The 18 archive descriptors of `g_LgpFileHandles` and the lookup prefix buffer that follows them |
| `g_LgpTocBlocks`, `g_LgpLookupTables`, `g_LgpFileHandles`, `g_LgpConflict*` | defined in `is_lib.cpp` | Per slot (18): TOC pointer and count, the 0xE10-byte lookup table, the file descriptor, the conflict tables (0x1F40 bytes of entries per slot) |
| `g_InputCursorX..g_InputMouseDeltaSecondAccumulator` | `include/lib/api/file.h` | Input state: cursor, held (`g_InputHeldMask`) and pressed (`g_InputPressedMask`) masks, the 3 x 25 binding table, repeat timing |

## Data flow

1. Archives. `OpenLgpArchive(path, slot)` opens the file with `FileOpenLL(path, 0x8000)`.
   `ReadLgpToc` reads a u16, ten signature bytes and the entry count, then per entry a 20-byte name,
   the u32 data offset (stored at +16 of the 24-byte entry), a byte at +20 and a u16 conflict index
   at +22. A 30 x 30 table of (first, count) pairs follows, indexed by the first two name characters
   (`GetLgpLookupCharCode`), then the conflict table (`ReadLgpConflictTable`: per name, records of a
   128-byte directory and a TOC index). Slots opened by ff7/main: 0 `field/char.lgp`,
   1 `field/flevel.lgp`, 2 `battle/battle.lgp`, 3 `battle/magic.lgp`, 4 `menu/menu_us.lgp`,
   5 `wm/world_us.lgp`, 6 `condor.lgp`, 7 `chocobo.lgp`, 14 `cd/moviecam.lgp`, 15 `cd/cr_us.lgp`;
   `BuildArchivePath` also names 8..10, 16 and 17. lib/sound opens slot 11.
2. Lookup. `FindLgpFile` splits the path, prepends the prefix from `SetLgpLookupDirPrefix` when set,
   scans the lookup range with `strcmpi`, and for a name with a conflict index matches the directory
   against the conflict table. At the returned offset an entry holds its 20-byte name and u32 size
   (`GetLgpEntrySize`) and then the data (`ReadLgpEntry` seeks offset + 0x18).
3. Open. `OpenFileEx` with `flags` set calls `SetCurrentLgpArchiveId`, `FindLgpFile` and
   `SeekLgpArchive(offset + 0x18)`; later reads go through `ReadLgpArchiveStream` from that position.
   Write modes only report "FILE SYSTEM IS READ ONLY" and still return the handle. With `flags` 0 it opens the disk file through
   LIBCMT's `_open` with the mode's flags (0x8000 read, 0x4000 text, 0x8102 write, 0x8302 create).
   With the debug log on (`EnableFileDebugLog`) every open is logged.
4. Loose data files. `LoadFileBlock` loads a whole disk file with `LoadDiskFile`, optionally copies a
   slice into a new buffer, and calls the callback; battle uses it for `kernel/window.bin`,
   `kernel/kernel.bin` (callback `Kernel_ParseKernelBin`) and 0x2000-byte chunks of `scene.bin`.
   `LoadFileBlockLzs` decompresses `kernel/kernel2.bin` with `LzsDecompress`.
5. Input. `Input_Init` calls `DI_InputInit` and `Input_LoadAllDefaults`. Each frame `Input_Update`
   polls mouse, keyboard and joystick, moves the cursor in the context (+0x840, +0x844, clamped to
   the width and height at +0x954 and +0x958), ORs 25 actions over three binding sets into the
   held mask, derives the pressed mask and ticks auto-repeat (`Input_SetRepeatRates(200, 50)` at startup).

## Named codes

| Enum | Header | Use |
|---|---|---|
| `InputButton` | `include/lib/enums/file.h` | Bits of the held and pressed masks; used by menu, field, chocobo and battle callers and by `Input_CaptureAnySource`; `Input_Update` builds the masks as `1 << j` |
| `KernelTextGroup` | same | Groups of `Kernel_LookupUiText`; used by the menu callers, not yet inside `Kernel_LookupUiText` |

## Notes

- `file_input_poll_bindings.cpp` accesses the binding table via `#define` alias `g_InputBindings` (0x009a85e4), and mirrors the cursor into the engine context at
  `(char *)g_InputContext + 0x840` once.
- `is_lib.cpp` reads the LGP tables through `FileLgpTocBlock`/`FileLgpTocEntry` (the TOC),
  `FileLgpLookupRow` (the 30 x 30 lookup cells) and the conflict banks. The lookup cells are still
  addressed as `table + row * 30 * 4 + column * 4`: a row-indexed access compiles differently.
- The low-level I/O is called by its POSIX name with the period `<io.h>` prototypes: `open`, `close`,
  `read`, `write`, `lseek`, `tell`, `getcwd`, `stricmp` and `filelength`. OLDNAMES.LIB resolves each to
  its LIBCMT name (`_open`, ...) through a member of its own; `filelength`'s member is what puts LIBCMT's
  flength.obj after thread.obj, as retail, and the others add their members to the count retail's Rich
  header shows. `_unlink`, `_chdir` and `_mkdir` keep the CRT name: as
  aliases they would move the IAT or the CRT order.
- Thin file wrappers use descriptive parameter names: `LoadDiskFile(sizeOut,
  fileName)`, `LoadDiskFileEx(extra, sizeOut, fileName)`, `ReadFileIntoNewBuffer(size, count, file)`;
  `WriteFileData` takes a `const void *` buffer. `FileRecord`'s first word is `kind`, and `FileRecordMatchesKindNameDirectory` compares two `FileRecord`s.
- `CompareDirectoryEntryNames` in `direct.cpp` is `SortDirectoryListingNames`'s `qsort` comparator.
- `FileOpenRequest` copies the 16-byte open-request block directly.

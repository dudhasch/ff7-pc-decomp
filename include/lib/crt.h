/* CRT functions the game code calls, with their addresses in ff7.exe.
 *
 * WHY THIS FILE EXISTS
 * --------------------
 * A game function that calls `sprintf` compiles to `call _sprintf`, where
 * `_sprintf` comes from the statically linked Visual C++ 5.0 LIBCMT runtime
 * (at 0x0040B4A0 in the retail executable).
 *
 * Each `LIBRARY` annotation below maps a retail runtime address to its C
 * declaration in the PDB; nothing may appear between the marker and its
 * immediately following prototype.
 *
 * By-name library annotations belong in this shared header rather than in .c
 * translation units, which is why these declarations live in
 * include/ rather than next to the call.
 *
 * THE NAME MUST BE THE DECORATED ONE
 * ---------------------------------
 * `// sprintf` does not work and reports no error -- the lookup simply
 * finds nothing and the operand stays unmasked, which is indistinguishable
 * from not having written the annotation at all. `// _sprintf` works. Our
 * PDB carries the __cdecl decoration, so that is what the lookup has to
 * spell. Nothing may follow the name on that line either: the WHOLE line
 * is the symbol being looked up, so a trailing ` -- LIBCMT.LIB` makes it
 * inert.
 *
 * The CRT functions were identified by byte identity against the period
 * LIBCMT.LIB rather than by name. An entry is here only when a game
 * function calls it.
 */

#ifndef FF7_LIB_CRT_H
#define FF7_LIB_CRT_H

// The runtime spans below come from the real VC5 archive, including local labels.
// LIBRARY: FF7 0x0040cc60 SYMBOL
// __unlock_file2

// crt0dat.obj's own exit-lock release (push 0Dh; call _unlock; add esp, 4; ret and its 90 fill).
// Retail links the whole member in place: __lockexit at 0x0040cf50, this at __unlockexit
// (0x0040cf60), __initterm at 0x0040cf70.
// LIBRARY: FF7 0x0040cf60 SYMBOL
// __unlockexit

// LIBRARY: FF7 0x0040e11d SYMBOL
// __NLG_Notify1

// LIBRARY: FF7 0x0040eb50 SYMBOL
// __atoflt

// LIBRARY: FF7 0x0040eaf0 SYMBOL
// __ld12tof

// GLOBAL: FF7 0x007bc2d4
extern unsigned int _NLG_Destination[4];

// LIBRARY: FF7 0x0040f1d0 SYMBOL
// __write

// LIBRARY: FF7 0x007ae978 SYMBOL
// _RtlUnwind@16

// LIBRARY: FF7 0x007aeb60 SYMBOL
// ?_CallMemberFunction0@@YGXPAX0@Z

// LIBRARY: FF7 0x007afe00 SYMBOL
// __CIcos

// LIBRARY: FF7 0x007afeb0 SYMBOL
// __CIsin

// LIBRARY: FF7 0x007aff60 SYMBOL
// __CItan

// LIBRARY: FF7 0x007b0020 SYMBOL
// __CIatan

// LIBRARY: FF7 0x007b0a10 SYMBOL
// __unlink

// LIBRARY: FF7 0x007b11b0 SYMBOL
// __mkdir

// LIBRARY: FF7 0x007b2663 SYMBOL
// __rtchsifneg

// LIBRARY: FF7 0x007b3520 SYMBOL
// ?_ValidateWrite@@YAHPAXI@Z

// Actual remove/atol anchors distinguish the otherwise identical unlink/atoi wrappers.
// LIBRARY: FF7 0x007b09e0
// _remove

// Native VC5 EH dispatcher, referenced by compiler-generated .text$x handlers.
// LIBRARY: FF7 0x007aebd0
// ___CxxFrameHandler

// Intrinsic runtime stack/type check, with native archive call identity.
// LIBRARY: FF7 0x007b2758
// __checkTOS_withFB

// LIBRARY: FF7 0x007b2715
// __fload_withFB

// LIBRARY: FF7 0x0040e900
// __ld12cvt

// LIBRARY: FF7 0x007afd70
// _vsprintf

// Period LIBCMT flength.obj, tell.obj and lseek.obj.
// LIBRARY: FF7 0x007b11e0
// __filelength

// LIBRARY: FF7 0x007b0570
// __tell

// Period LIBCMT read.obj: retail links the whole member, __read here and __read_lk at
// 0x007b0610.
// LIBRARY: FF7 0x007b0590
// __read

// LIBRARY: FF7 0x0040f0d0
// __lseek

// Period LIBCMT close.obj.
// LIBRARY: FF7 0x00414030
// __close

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// VC5 SP3 tolower.obj: 130 code bytes and 14 NOP bytes.
// LIBRARY: FF7 0x0040cc90
// _tolower

// LIBRARY: FF7 0x0040cc20
// __unlock_file

// Period LIBCMT free (112 bytes).
// LIBRARY: FF7 0x0040f7b0
// _free

// LIBRARY: FF7 0x0040f820
// _malloc

// LIBRARY: FF7 0x00410e30
// _calloc

// LIBRARY: FF7 0x00410860
// _strncmp

// Period narrow _open wrapper.
// LIBRARY: FF7 0x007b01b0
// __open

// LIBRARY: FF7 0x00413170
// _atoi

// LIBRARY: FF7 0x0040b610
// __strcmpi

// LIBRARY: FF7 0x0040b4a0
// _sprintf

// LIBRARY: FF7 0x00412f10
// __itoa

// The float-to-integer helper the compiler emits for `(int)floatexpr`. Written
// `_ftol()` in a body, MSVC decorates the one-underscore identifier to the
// two-underscore runtime symbol `__ftol` and links it from LIBCMT; the marker
// names that symbol so the image's `call __ftol` and ours agree. Writing
// `__ftol()` in a body instead decorates to `___ftol`, which links to nothing
// at all.
// LIBRARY: FF7 0x0040b470
// __ftol

// The std::_Lockit constructor and its separately registered atexit cleanup.
// LIBRARY: FF7 0x007ad7a0
// ??0_Lockit@std@@QAE@XZ

// LIBRARY: FF7 0x007ad830
// ?_CleanUp@std@@YAXXZ

// LIBCPMT xlock.obj's destructor, including LeaveCriticalSection and 11 NOPs.
// LIBRARY: FF7 0x007ad860
// ??1_Lockit@std@@QAE@XZ

// The 47-byte stack probe plus its CC alignment byte.
// LIBRARY: FF7 0x007ae980
// __chkstk

// Field_SeedRandomFromContext, Mini_CondorSeedRandomFromContext (0x005F5153) and
// Mini_ChocoboSeedRandomFromContext seed from the context word or, when it is zero, from `time(&t)`;
// both calls only resolve with the CRT names.
// LIBRARY: FF7 0x007ae9b0
// _srand

// LIBRARY: FF7 0x007ae9c0
// _rand

// LIBRARY: FF7 0x007aeac0
// _atexit

// LIBRARY: FF7 0x007af060
// _time

// 0x007AF850 is qsort: `num < 2 || width == 0`
// returns at once, a 0x100-byte lo/hi stack, a cutoff of 8 elements into
// shortsort, and one caller so far -- 0x00643038 pushes Field_CompareDrawOrderDepthDescending as the
// comparator with an element width of 4.
// LIBRARY: FF7 0x007af850
// _qsort

// The two-argument x87 intrinsic used by the fixed-angle helpers. Like
// __CIsqrt, it receives its operands on the x87 stack rather than through a
// normal C argument list.
// LIBRARY: FF7 0x007afdea
// __CIatan2

// The compiler's intrinsic sqrt call leaves its operand on the x87 stack and
// targets this statically linked helper directly.
// LIBRARY: FF7 0x007afa90
// __CIsqrt

// The pow intrinsic receives two operands on the x87 stack.
// LIBRARY: FF7 0x007afb50
// __CIpow

// LIBRARY: FF7 0x007aeee0
// __splitpath

// Pfile_LoadModelSet builds each RSD basename with the statically linked CRT.
// LIBRARY: FF7 0x007b0110
// __makepath

// LIBRARY: FF7 0x004134d0
// _fclose

// LIBRARY: FF7 0x007af1d0
// _fopen

// LIBRARY: FF7 0x007b00c0
// _fprintf

// LIBRARY: FF7 0x007b0840
// __getcwd

// LIBRARY: FF7 0x007af1f0
// _fseek

// LIBRARY: FF7 0x007af2d0
// _ftell

// LIBRARY: FF7 0x007af4b0
// _fread

// The 64-byte VC5 fwrite.obj wrapper.
// LIBRARY: FF7 0x007af630
// _fwrite

// LIBRARY: FF7 0x007af7c0
// _fgets

// Period thread.obj.
// LIBRARY: FF7 0x007b0a20
// __beginthread

// LIBRARY: FF7 0x007b0b50
// __endthread

// Period findfile.obj.
// LIBRARY: FF7 0x007b0ba0
// __findfirst

// LIBRARY: FF7 0x007b0cd0
// __findnext

// LIBRARY: FF7 0x007b0df0
// __findclose

// Period _ctype.obj (48 bytes).
// LIBRARY: FF7 0x007b0f80
// _isspace

// VC5 narrow rename.obj: 50 code bytes and fourteen NOPs.
// LIBRARY: FF7 0x007ae9f0
// _rename

// CRT functions the directory, UV and hex helpers call.
// LIBRARY: FF7 0x007b0eb0
// __chdir

// LIBRARY: FF7 0x007b0f50
// _isdigit

// LIBRARY: FF7 0x007b0fb0
// _atof

// LIBRARY: FF7 0x007b1020
// _toupper

#endif /* FF7_LIB_CRT_H */

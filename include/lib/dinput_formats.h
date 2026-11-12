/* The DirectX 5 import library supplies this stdcall thunk and the
 * following mouse/keyboard format objects. The inventory keeps the complete
 * 4284-byte interval. */
#ifndef FF7_LIB_DINPUT_FORMATS_H
#define FF7_LIB_DINPUT_FORMATS_H

// Native SDK import thunk forwarding to _DirectInputCreateA@16. No authored game body is present.
// The fixed inventory interval also contains the SDK mouse/keyboard format data and linker
// alignment; only the entry thunk is a procedure.
// LIBRARY: FF7 0x007ad88c SYMBOL
// _DirectInputCreateA@16

#endif

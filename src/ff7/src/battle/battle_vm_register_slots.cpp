// Original object 0x005bf9d0-0x005bfaa0: eight battle VM register
// indirection slots. Each slot is a global reference to its default storage word, so VC5 emits a
// .CRT$XCU entry, a 10-byte dynamic initializer and a 15-byte reference store per definition, in
// this order. The compiler-generated $E functions carry no FUNCTION markers.

#include "ff7/api/battle.h"
extern "C" {

// .CRT$XCU 0x007ba004: initializer $CRT_CPP_Initializer (0x005bf9d0), reference store $E1
// (0x005bf9da).
// GLOBAL: FF7 0x00c05fe8
int &g_VmSlot312_00c05fe8 = g_BattleVmSlotStorage_0;

// .CRT$XCU 0x007ba008: initializer $CRT_CPP_Initializer (0x005bf9e9), reference store
// DynamicConstruct@005bf9f3.
// GLOBAL: FF7 0x00c05fec
int &g_BattleVmReg_1 = g_BattleVmSlotStorage_1;

// .CRT$XCU 0x007ba00c: initializer $CRT_CPP_Initializer (0x005bfa02), reference store
// DynamicConstruct@005bfa0c.
// GLOBAL: FF7 0x00c05ff0
int &g_BattleVmReg_2 = g_BattleVmSlotStorage_2;

// .CRT$XCU 0x007ba010: initializer $CRT_CPP_Initializer (0x005bfa1b), reference store
// DynamicConstruct@005bfa25.
// GLOBAL: FF7 0x00c05ff4
int &g_BattleVmReg_3 = g_BattleVmSlotStorage_3;

// .CRT$XCU 0x007ba014: initializer $CRT_CPP_Initializer (0x005bfa34), reference store
// DynamicConstruct@005bfa3e.
// GLOBAL: FF7 0x00c05ff8
int &g_BattleVmReg_4 = g_BattleVmSlotStorage_4;

// .CRT$XCU 0x007ba018: initializer $CRT_CPP_Initializer (0x005bfa4d), reference store
// DynamicConstruct@005bfa57.
// GLOBAL: FF7 0x00c05ffc
int &g_BattleVmSlot_5 = g_BattleVmSlotStorage_5;

// .CRT$XCU 0x007ba01c: initializer $CRT_CPP_Initializer (0x005bfa66), reference store
// DynamicConstruct@005bfa70.
// GLOBAL: FF7 0x00c06000
int &g_BattleVmWord_6 = g_BattleVmSlotStorage_6;

// .CRT$XCU 0x007ba020: initializer $CRT_CPP_Initializer (0x005bfa7f), reference store
// DynamicConstruct@005bfa89.
// GLOBAL: FF7 0x00c06004
int &g_BattleVmVar_7 = g_BattleVmSlotStorage_7;
}

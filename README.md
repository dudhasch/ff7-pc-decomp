# Final Fantasy VII (PC, 1998): the source, rebuilt

C and C++ source for the 1998 PC release of *Final Fantasy VII*. Built with the compiler and linker
Square used in 1997, it produces an `ff7.exe` that is **byte-identical** to the retail executable:
all 5,882,880 bytes, SHA-256 `a33ccad7554f648c382a4dc0a5829c7fd4ce7ff56f6406b26fc671569875bb90`.

This is not a port, a remaster or a mod. Nothing is fixed or modernised, and the game's own bugs
are kept, because they are part of the bytes. The repository holds no game data: to build it, or
just to run what you build, you need your own copy of the game.

| | |
|---|---|
| Source files | 692, one per object of the original link |
| Functions | 10,361 game functions, each tied to its retail address |
| Data | every global at its retail address; `.data` is byte-identical too |
| Lines | about 595,000 |
| Result | `ff7.exe` 1.00, identical to retail |

## Where things are

| Folder | What it is |
|---|---|
| [`src/ff7/src/main`](src/ff7/src/main/README.md) | Start-up, the main loop, the game-mode dispatcher, registry and CD checks |
| [`src/ff7/field`](src/ff7/field/README.md) | Field maps: the script VM and its opcodes, walkmesh, models, backgrounds, encounters |
| [`src/ff7/src/battle`](src/ff7/src/battle/README.md) | Battle: ATB, commands, damage, AI scripts, every spell, summon and limit effect |
| [`src/ff7/src/menu`](src/ff7/src/menu/README.md) | The main menu, shops, the save screens and the battle menu |
| [`src/ff7/src/wm`](src/ff7/src/wm/README.md) | The world map: terrain streaming, vehicles, its script VM |
| [`src/ff7/src/movie`](src/ff7/src/movie/README.md), [`credits`](src/ff7/src/credits/README.md) | Movie playback and the ending credits |
| [`chocobo`](src/ff7/chocobo/README.md), [`coaster`](src/ff7/coaster/README.md), [`condor`](src/ff7/condor/README.md), [`highway`](src/ff7/highway/README.md), [`snobo`](src/ff7/snobo/README.md), [`snowboard`](src/ff7/snowboard/README.md), [`submarine`](src/ff7/submarine/README.md) | The minigames: chocobo racing, the Speed Square coaster, Fort Condor, the motorcycle chase, snowboarding, the submarine |
| [`src/lib/src`](src/lib/src/README.md) | The engine library underneath: [graphics](src/lib/src/graphics/README.md) (Direct3D, OpenGL, software), [sound](src/lib/src/sound/README.md), [files and LGP archives](src/lib/src/file/README.md), [polygon models](src/lib/src/polygon/README.md), [movies](src/lib/src/movie/README.md) |
| `include/` | Headers: shared types, enums, one API header per subsystem |
| `config/objects.tsv` | Every object in link order, with its library and compiler switches |
| `tools/` | The build: `build.py`, the resource step, the frozen-clock linker runner |

Each folder's README says what the code does, where it starts, which structures it uses and how
data flows through it.

## Things the code shows

- **Random encounters** ([`field_encounter_random.cpp`](src/ff7/field/src/field_encounter_random.cpp)).
  Every eighth step, the player's speed divided by the map's rate is added to a danger counter, and
  a battle starts when a random byte falls below `danger * encounterRate / 4096`. A formation equal
  to the last battle's is drawn once more, but when that second draw falls through to the last
  normal slot, the repeat check is skipped.
- **The damage overflow** ([`battle_damage_formula_set.cpp`](src/ff7/src/battle/battle_damage_formula_set.cpp)).
  The random damage variance is a 32-bit multiply. Above 524,287 damage it overflows, the result
  turns negative, and the target can end at 0 HP.
- **W-Item duplication** ([`battle_window_panels.cpp`](src/ff7/src/menu/btlmenu/english/battle_window_panels.cpp)).
  Cancelling the second W-Item pick refunds the first item to the slot it came from. If that slot
  now holds another item, that item gains one instead.
- **Armour magic defense** ([`battle_kernel_materia_support.cpp`](src/ff7/src/battle/battle_kernel_materia_support.cpp)).
  The lookup for an armour's magic defense always returns 0, so the battle stats never include it.
- **The chocobo minimap** ([`chocobo_race_hud_submit.cpp`](src/ff7/chocobo/chocobo_race_hud_submit.cpp)).
  The table the racers' marker heights come from is never set, so they are read from a fixed low
  address.

Search the tree for `BUG:` to find the others.

## Reading the code

- `// FUNCTION: FF7 0x0060b2c6` above a definition gives the function's address in the retail
  `ff7.exe`; `// GLOBAL:` and `// STRING:` do the same for data. The markers follow the
  [reccmp](https://github.com/isledecomp/reccmp) format, so reccmp and similar tools can read them.
- The game was compiled without optimisation (`/Od`). At `/Od` the compiler keeps the shape of the
  source: a nested `if` and an early `return` produce different code. So the deep nesting and the
  empty first arms of some `if` statements are the shape of Square's code, not a style choice.
  Local variable names decide stack slots at `/Od`, so some locals are named to land where the
  original's did. These spellings the old compiler needs (`UNFOLDED`, frame unions, `if (0)` blocks) are named and
  explained in [`include/lib/codegen.h`](include/lib/codegen.h).
- `// Original: C:\FF7\field\src\ad_human.CPP` at the top of a file is a path the retail executable
  still contains as a string. Those files keep their original names. The others are named after
  what they hold.
- Large tables are byte arrays where no structure has been found for them yet.

## How this was made

Each function was written to compile to the same bytes as the retail one and checked
instruction by instruction, and the linked image is checked byte for byte. That part is proven.

The names and comments are not. Square's own names did not survive in the executable, apart from
the file paths above and a few strings, so most function, variable and type names were inferred
from what the code does, some from the PlayStation decompilation. Much of the naming and
commenting was done with the help of AI tools and checked against the code; some names were
confirmed by watching memory in the running game. Some names are surely wrong. Corrections with
evidence are welcome: a debugger trace, a memory watch in the running game, a matching PlayStation
routine.

## Building

You need Windows, Python 3.10 or newer, your retail `ff7.exe`, and the period toolchain, which
this repository cannot ship. Put the toolchain into `toolchain/`:

| Folder | Contents |
|---|---|
| `toolchain/vc5/` | Visual C++ 5.0 (`bin/`, `include/`, `lib/`) with the Visual Studio 97 SP3 compiler passes (`C1XX.DLL` 11.00.7303) |
| `toolchain/link510/` | LINK 5.10.7303 as `link.exe`, with its `MSPDB50.DLL`, `MSDIS100.DLL`, `MSVCP50.DLL` and `CVTRES.EXE` |
| `toolchain/msvcrt500/` | `msvcrt.dll` 5.00.7303 from the VS97 SP3 redistributables |
| `toolchain/dx5/` | DirectX 5 SDK `inc/` and `lib/` |
| `toolchain/dxmedia/lib/` | `strmiids.lib` and `amstrmid.lib` from the DirectX Media SDK |

[`config/toolchain.sha256`](config/toolchain.sha256) lists every file the build reads, with its
hash. The build checks them first. Then, with `ff7.exe` in the repository root:

```bash
py -3.10 tools/build.py
```

It compiles the 692 objects, archives them into one library per source directory, links them with
LINK 5.10 and compares the result with your `ff7.exe`. On a current PC it takes under a minute. The
output is `build/ff7.exe`; the last line says whether it is byte-identical.

## Credits

- [reccmp](https://github.com/isledecomp/reccmp), from the LEGO Island decompilation, for the
  comparison tool and its annotation format.
- [ff7-decomp](https://github.com/Xeeynamo/ff7-decomp), the PlayStation decompilation, whose names
  for the field script VM, the world-map actor and the savemap are used here where the PC code
  agrees with them.
- The [Qhimm](https://forums.qhimm.com/) community and its wiki, whose decades of file-format and
  opcode research are the reason most of this could be named at all.

## Legal

*Final Fantasy VII* and all its code, names and assets are © Square Enix. This project is not
affiliated with or endorsed by Square Enix. It contains no game data and no Microsoft files; you
need your own copies of both.

This repository's own work is dedicated to the public domain under [CC0 1.0](LICENSE), as far as
that is possible.

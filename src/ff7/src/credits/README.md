# Credits: the opening logo sequence and the ending credits

The credits module plays two sequences. Variant 0 runs in mode 0x1B (`GAME_MODE_CREDITS`):
`App_InitSubsystems` sets that mode, so it is the first module the game shows. It plays the logo
movies, the opening name cards and the FF7 logo in a loop until a key is pressed, then returns to the
title menu (mode 0x14). Variant 1 runs in mode 0x1C (`GAME_MODE_ENDING_CREDITS`) after
`ending2.avi`: it scrolls the staff list from `people.bin`, plays `ending3.avi` and then quits the
game (mode 0x13). The dispatcher enters it through `Credits_ModuleEnter`; it leaves by handing the
dispatcher callbacks back to `RequestModuleSwitch` (see [main](../main/README.md)).

## Where the code is

`src/ff7/src/credits/`: 10 TUs, 65 functions. Four TUs hold code:

| TU | span | functions | holds |
|---|---|---:|---|
| `credits_animation_frame_loop.cpp` | 0x007a5060-0x007a8150 | 23 | `Credits_ModuleEnter`, `Credits_ModuleExit`, `Credits_ModuleFrame`, `Credits_ModuleMouse`, `Credits_ModuleKeyboard`, the sequence state machines, `Credits_SetVariant`, `Credits_SetStartSection` |
| `credits_glyph_string_draw.cpp` | 0x007a8150-0x007a98f0 | 10 | `Credits_MeasureGlyphStringWidth` and nine `Credits_DrawGlyphString*` text drawers |
| `credfile.cpp` | 0x007a98f0-0x007aa820 | 17 | render objects, `people.bin`, movie names, timing setup |
| `credits_fade_overlay_screen.cpp` | 0x007aa820-0x007ad7a0 | 15 | fade overlay, background, logo and strip quads, particles |

The other six are empty sources kept for link order: `credits_empty_tmd_primitives.cpp`,
`credits_empty_tmd_renderer.cpp` and `credits_empty_tmd_texture_release.cpp` (empty), and
`credits_empty_snobo_hud_data.cpp`, `credits_empty_tmd_texture_data.cpp` and
`credits_empty_tmd_race_data.cpp` (data-only). None holds credits code: the three empty TUs note that
the functions of retail object `0x00731AA0..0x00733340` live in `src/ff7/snobo/tmd.cpp`, and the three
data-only ones say their data lives in snowboard TUs.

The two module TUs sit right before `credfile.cpp` and hold `Credits_*` functions. TU boundaries follow
retail's object boundaries.

## Entry points

| function | address | what it does | called from |
|---|---|---|---|
| `Credits_ModuleEnter` | 0x007a776f | viewport by UI mode; variant 0 opens the first logo movie and the TIMs, variant 1 loads `people.bin` and starts music 98 | engine context, from the dispatcher (modes 0x1B, 0x1C) |
| `Credits_ModuleFrame` | 0x007a7a33 | frame pacing, scene, movie blit, fade overlay, render passes; returns to the dispatcher when the end fade is done | engine context |
| `Credits_ModuleExit` | 0x007a79be | releases movie and render objects, sets the next mode, closes archive 15 | engine context |
| `Credits_SetVariant` | 0x007a80fa | 0 or 1 | `GameModeDispatcher_Frame` |
| `Credits_SetStartSection` | 0x007a8107 | 0 after game over, else 7 | `GameModeDispatcher_Frame` |
| `Credits_OpenMovie` | 0x007aa4c5 | opens `<MoviePath>` + one of four names (index outside 0..3 becomes 1) | enter, sequence states |
| `Credits_CreateRenderObjectSets` | 0x007a9f03 | backdrop and name TIMs from archive 15 | `Credits_CreateRenderSetsAndObject` |
| `Credits_LoadPeopleBinAndCreateRenderObjects` | 0x007a9a12 | reads `people.bin`, centres its lines, font TIMs | enter, variant 1 |
| `Credits_UpdateAndDrawFadeOverlay` | 0x007aa89b | steps the overlay alpha and writes its four vertices | `Credits_ModuleFrame` |

`Credits_ClearState` and `Credits_ReleaseRenderObjectsAndCreateComesoon` (`comesoon.tim`) are not
called by name anywhere in the tree.

## Main structures

| type or global | header | what it holds |
|---|---|---|
| `g_CreditsVariant`, `g_CreditsStartSection` | `include/ff7/api/credits.h` (`credits_animation_frame_loop.cpp`) | which sequence runs, and where variant 0 starts |
| `g_CreditsSequenceState` | `include/ff7/api/credits.h` | variant 0 state, `enum CreditsSequenceState` in `include/ff7/enums/credits.h` (`Credits_TickSequenceState`) |
| `g_CreditsTextPhase` | `include/ff7/api/credits.h` | variant 1 phase 0..3 (`Credits_TickTextPhase`) |
| `g_CreditsScriptDataBase` | `include/ff7/api/credits.h` | heap copy of `people.bin`: 0x21B records of 0x40 bytes. Per record: glyph-drawer kind at +0, colour at +4, x at +8, text at +0xC, line height at +0x3C. |
| `CreditsLineRecord` | `include/ff7/types/credits.h` | the 0x40-byte record, used only by `Credits_ResetRecordDefaults` |
| `g_CreditsFadeOverlayBatch` | `include/ff7/api/credits.h` (0x1C bytes) | +0 the fade-overlay render object, +4 the six backdrop render objects |
| `g_CreditsQuadBatchListA`..`D` | `include/ff7/api/credits.h` | render objects for name cards (A, B), logo (C), font and `after_1.tim` (D) |
| `g_CreditsFadeOverlay*` from 0x00f4f5a0 (`g_CreditsFadeOverlayLeft`, ...) | `include/ff7/api/credits.h` | overlay rectangle, alpha, step, active flag |
| `CreditsFadeVertex*`, `CreditsLogo*Vertex`, `CreditsBackdropQuad*` and the other vertex structs | `include/ff7/types/credits.h` | per-quad and per-corner vertex records for the draw functions |

## Data flow

1. `GameModeDispatcher_Frame` opens `<DataPath>cd/cr_us.lgp` as LGP archive 15, sets the variant
   (and, for variant 0, the start section), and switches to the credits module.
2. `Credits_ModuleEnter` sets the viewport: 320x240 at (0,0) for UI mode 0, at (160,120) for mode 1,
   640x480 for mode 2. Every TIM is loaded from archive 15 through `Gfx_CreateRenderObject`.
3. Variant 0 (`Credits_TickSequenceState`): state 7 plays `eidoslogo.avi` and opens `sqlogo.avi`;
   state 0 plays `sqlogo.avi`; states 1 and 2 start music 0x5F and draw the `opback_*.tim` backdrops and
   `name*` cards; state 3 plays `explode.avi` and draws the FF7 logo with an alpha taken from the
   movie frame; states 4 to 6 hold the logo and strip, fade out, stop the music and go back to
   state 0. A key press (`Input_GetPressed(INPUT_ANY_BUTTON)`) starts a fade to black.
4. Variant 1: `Credits_DrawRecordWindow` draws eleven records per frame through the glyph drawers
   and scrolls; after record 0x210 the text phase moves on to the particle strips
   (`Credits_InitParticleRecords`, `Credits_UpdateFadeStrips`), a fade, and `ending3.avi`.
5. Each frame `Credits_ModuleFrame` blits the movie with `MovieBltFrameToBackBuffer` while one is
   running, updates the fade overlay and emits the render-object lists (`Credits_EmitRenderPasses`).
   It paces itself with the timestamp counter (`Credits_WaitForFrameDuration`).
6. When the end fade or the last movie is done, `Credits_ModuleFrame` hands the dispatcher
   callbacks to `RequestModuleSwitch`. `Credits_ModuleExit` then sets mode 0x14 (variant 0) or 0x13
   (variant 1) and closes archive 15.

## Named codes

[include/ff7/enums/credits.h](../../../../include/ff7/enums/credits.h) defines `CreditsSequenceState`, `CreditsVariant`, `CreditsTextPhase`, `CreditsAnimationMode`, and `CreditsGlyphCode`. Credits also uses `BLEND_*` and `RENDER_STATE_*` (`include/lib/enums/graphics.h`), `SOUND_MUSIC_*` (`include/lib/enums/sound.h`) and `INPUT_ANY_BUTTON`.

## Notes

- Scale-suffixed glyph and quad helpers (`Credits_DrawLogoStrip2x`, `Credits_DrawGlyphString32x40`, ...) pair 640x480 and 320x240 drawing paths.
- `credfile.cpp` declares stack locals as `unusedNN[]` arrays to keep VC5's frame, and fills the
  load options through loose locals in five functions; only `Credits_CreateRenderObjectSets` uses
  `PfileLoadOptions`.
- `Credits_CenterRecordTexts` and `Credits_DrawRecordWindow` read `people.bin` records as
  `*(int *)(base + (i << 6) + 0xNN)` instead of `CreditsLineRecord`.

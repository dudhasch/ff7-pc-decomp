# Battle: the turn-based (ATB) combat module

This is the battle screen: party against enemies, the ATB gauges, commands, the AI scripts,
damage, and the 3D scene with its camera and spell, summon and limit effects. The game-mode
dispatcher in [ff7/main](../main/README.md) installs `Battle_ModuleEnter`, `Battle_ModuleFrame`
and `Battle_ModuleExit` for `GAME_MODE_BATTLE` (`main.cpp`). `Battle_InitSession` reads the scene id
from `g_FieldRequestedBattleId` and the battle flags from `g_BattleResultWord`.
The swirl that comes before a battle is its own mode (`GAME_MODE_BATTLE_SWIRL`, `Swirl_Module*`
in `yama/`). The battle leaves on its own: the last phase of `Battle_RunPhaseMachine` sets
`g_EngineGameMode` to field (1), world map (3) or the result screens (0x11) and
requests a switch back to the dispatcher. The battle menus and windows are in
[ff7/menu](../menu/README.md) (`menu/btlmenu/`).

## Where the code is

373 TUs, 4,762 functions. 17 files keep their retail names from `__FILE__` strings; the other 354
are named after what they contain, and their place in the link order is inferred from their neighbours.
TU boundaries follow retail's objects; a descriptive file name such as `battle_fx_magic_fire.cpp` names the effect or subsystem slice inside its retail object.

| folder | TUs | holds |
|---|---|---|
| `battle/` (this folder) | 334 | module, phase machine, commands, damage, AI VM, camera, effects |
| `battle/battle3d/` | 8 | model, animation, LMD mesh and stage loaders (`char.cpp`, `amptoanm.cpp`, `lmd.cpp`, `stage.cpp`, `bdata.cpp`, `enemy.cpp`, `mdl.cpp`, `limitbrk.cpp`) |
| `battle/yasui/` | 26 | limit, summon, special, magic, item and weapon effects (`vahamut0.cpp`, `sting.cpp`, `deadsef.cpp`, `yasui_battle_fx_*`) |
| `battle/myoshiok/` | 1 | `lasboss3.cpp`: the Supernova effect (`BattleFx_SNova*`) |
| `battle/yama/` | 4 | kernel and scene loaders (`init.cpp`, `inits.cpp`), arena chooser (`coloss.cpp`), the swirl mode |

The 17 files with retail names: `battle.cpp`, `b3ddata.cpp`, the eight `battle3d/` files, `myoshiok/lasboss3.cpp`,
`yasui/vahamut0.cpp`, `yasui/sting.cpp`, `yasui/deadsef.cpp`, `yama/init.cpp`, `yama/inits.cpp`,
`yama/coloss.cpp`. 265 of the 334 root TUs are effect code or effect data (`battle_fx_*`).

Four pieces of battle data and helpers sit next to other folders' code in retail:
the dispatch tables in `battle_limit_fx_thunks.cpp` and `battle_camera_script_tables.cpp` (next to the field code),
`battle_bin_section_decompress.cpp` and `battle_gfx_offscreen_slots.cpp` (next to lib/file).

- The dispatch tables in `battle_limit_fx_thunks.cpp` are data: the six effect dispatch
  tables `g_BattleFx_GeneralDispatchTable`, `..._MagicDispatchTable`, `..._BlueMagicDispatchTable`,
  `..._SpecialDispatchTable`, `..._LimitDispatchTable` and `..._ThrowWeaponDispatchTable`
  (0x008fe398..0x008feba0) and battle globals from 0x00c06008 (VM slot storage, a ring buffer,
  effect state words). Only battle code
  (`Battle_RunActorEffect`, `Battle_RunActorEffectImpact`) reads the tables.
- `battle_camera_script_tables.cpp` holds the camera position and focal scripts at 0x00900198 that
  `battle_camera_channel_scripts.cpp` reads: the tables `g_BattleCameraPositionScripts` and
  `g_BattleCameraFocalScripts` (103 entries each) and the bytecode arrays they point to,
  `g_BattleCameraPositionScript000`..`g_BattleCameraPositionScript102` and `g_BattleCameraFocalScript000`..`g_BattleCameraFocalScript102` by table index (ops 0xF9,
  0xFA, 0xE2, 0xF5, 0xF4; 0xFF ends a script).
- `battle_bin_section_decompress.cpp` reads gzip-section files (`BinSection*`) and
  `battle_gfx_offscreen_slots.cpp` keeps the four battle offscreen slots (`g_GfxOffscreenSlots`); only battle
  code (`yama/init.cpp`, `yama/coloss.cpp`, `yama/yama_swirl_transition_module.cpp`) calls them.
- The battle UI (`Battle_UpdateUiAndLogic`, command menus, windows) is in `src/ff7/src/menu/btlmenu/`.

The coaster module callbacks are in [ff7/coaster](../../coaster/README.md) and the game-over mode in
[ff7/main](../main/README.md). Code here that is not battle:

- `yama/yama_swirl_transition_module.cpp` holds the swirl mode that runs before a battle (`Swirl_Module*`).
- Shared helpers sit in battle objects because retail placed them there: `LzsDecompress`
  (`battle_lzs_decompress.cpp`, used by ff7/wm and lib/file), `GzipInflate_BinSection`
  (`battle_gzip_inflate_decoder.cpp`, used by lib/file), the party-stat and materia code in
  `battle_kernel_materia_support.cpp`, which ff7/menu calls more than battle does, and
  `Kernel_InitJoiningCharacter` in `battle_kernel_experience_growth.cpp` (called from ff7/menu).

## Entry points

Called from other subsystems:

| function | address | what it does | called from |
|---|---|---|---|
| `Battle_ModuleEnter` | 0x0041b300 | Logs FRAME_INITIALIZE, runs `Battle_InitSession`, sets projection and the 320x240 or 640x480 layout from `GetUiScalingMode`, loads the UI textures | ff7/main `main.cpp` (module table) |
| `Battle_ModuleFrame` | 0x0041bab3 | One frame: timing, clear, `Battle_RunPhaseMachine` (once, or twice when the frame limiter is behind), the draw layers, camera node, UI updates, `SoundUpdateFades` | ff7/main (module table) |
| `Battle_ModuleExit` | 0x0041b87d | Waits for async loads and sound group 15, `Battle_ShutdownSession` (restores the savemap party bytes), frees UI textures, `Battle_DestroyScene` | ff7/main (module table) |
| `Kernel_LoadKernelBinFiles` | 0x00401018 | Loads `kernel/window.bin`, `kernel/kernel.bin`, `kernel/kernel2.bin` | ff7/main `main.cpp` (two sites) |
| `Battle_TickAtbAndGauges` | 0x004320ac | Advances ATB gauges, posts auto commands or requests the command menu, advances limit gauges (by its comment) | `Battle_UpdateUiAndLogic` in ff7/menu |
| `Battle_PostCommand` | 0x00432aa2 | Appends a chosen command to the 16-slot command ring and releases the actor's menu | ff7/menu (8 sites) |
| `Battle_RefreshPendingCommandAvailability` | 0x00437679 | By its name | ff7/menu (8) |
| `Battle_SetArenaSelectionResult` | 0x00431c80 | records the yes/no answer of the Battle Square continue window and the handicap slot screen | ff7/menu (4) |
| `Battle_LoadCombatantFromSavemap` | 0x005cb2cc | Rebuilds one party slot's stats from equipment and materia and writes them back (by its comment) | ff7/field, ff7/menu, lib/graphics (19) |
| `Party_RecomputeBattleModifiers` | 0x005cb127 | Recomputes the four battle modifiers in `g_PartyBattleModifiers` (gil scale, encounter rate, chocobo lure, pre-emptive odds) from the three party members' bonus bytes | ff7/field, ff7/menu, lib/graphics (13) |
| `Kernel_GetLimitAttackId` | 0x005c8684 | By its name | ff7/menu (9), lib/graphics (2) |
| `Kernel_ApplyMateriaToActor` | 0x005cd078 | By its name | ff7/menu (10) |
| `LzsDecompress` | 0x0041cf10 | LZS decoder | ff7/wm, lib/file |
| `GzipInflate_BinSection` | 0x0041d300 | Inflates one gzip section | lib/file, `Battle_LoadSceneRecord` |
| `Swirl_ModuleEnter` | 0x0040164e | Swirl mode before a battle | ff7/main (`GAME_MODE_BATTLE_SWIRL`) |
| `GameOver_ModuleEnter` | 0x00406367 | Game-over mode | ff7/main (`GAME_MODE_GAME_OVER`) |

The main loop inside the module:

| function | address | TU | what it does |
|---|---|---|---|
| `Battle_RunPhaseMachine` | 0x0041c0bb | `battle.cpp` | Phases 0..16 in `g_BattlePhase`: load scene, opening, main loop, arena rounds, end |
| `Battle_LoadSceneFirstRound` | 0x00437db0 | `battle_load_scene_rounds.cpp` | Party from savemap, scene record, enemies, AI init events, initial ATB |
| `Battle_LoadSceneNextRound` | 0x004381d5 | `battle_load_scene_rounds.cpp` | Same for the next formation of a multi-round battle |
| `Battle_LoadSceneRecord` | 0x005d1050 | `yama/inits.cpp` | Reads and inflates one record of `scene.bin` |
| `Battle_RunOpeningSequence` | 0x00429ac0 | `battle_opening_actor_setup.cpp` | Stage, camera, models; starts actor scripts |
| `Battle_RunActionQueue` | 0x00435789 | `battle_action_queue_run.cpp` | Takes queued actions by priority and runs them |
| `Battle_ExecuteAction` | 0x00435d81 | `battle_action_queue_run.cpp` | Runs the command program of one action |
| `Battle_ResolveHitOnTarget` | 0x005da5da | `battle_hit_damage_resolution.cpp` | Damage, status, reflect and HP/MP for one target |
| `Battle_RunAiScriptEvent` | 0x005c86e0 | `battle_ai_script_dispatch.cpp` | Finds an actor's AI script for an event and runs `Battle_AiVm_Run` |
| `Battle_PlaySequence` | 0x00436f6a | `battle_sequence_step_records.cpp` | Plays the queued sequence steps through `Battle_RunSequenceStep` |
| `Battle_Step3dFrame` | 0x0042d808 | `battle_opening_actor_setup.cpp` | One 3D frame: message window, `Battle_Update3dScene`, turns and escape |
| `Battle_Update3dScene` | 0x0042d992 | `battle_opening_actor_setup.cpp` | Stage, actor scripts, effects, camera channels, task slots |
| `Battle_RunActorAnimScript` | 0x0041fba4 | `battle_actor_anim_effect_tasks.cpp` | Motion script interpreter (opcodes 0x8E..0xFF) |
| `Battle_RunActorEffect` | 0x00427c22 | `battle_actor_anim_effect_tasks.cpp` | Dispatches an actor's effect through the effect tables |
| `Battle_RunCameraChannels` | 0x005c2070 | `battle_camera_channel_scripts.cpp` | Runs eye and focus camera scripts for three channels |
| `Battle_RunEndSequence` | 0x00430dd0 | `battle_end_sequence.cpp` | Victory, escape or defeat; rewards into the savemap |

## Areas

| area | where | main functions |
|---|---|---|
| Module, phase machine, scene load | `battle.cpp`, `battle_load_scene_rounds.cpp`, `yama/inits.cpp`, `battle_opening_actor_setup.cpp`, `battle_end_sequence.cpp`, `battle_arena_penalty_rounds.cpp`, `yama/coloss.cpp` | `Battle_ModuleFrame`, `Battle_RunPhaseMachine`, `Battle_LoadSceneRecord`, `Battle_RunArenaNextBattleChooser` |
| Actors and kernel data | `yama/init.cpp`, `battle_load_party_actors.cpp`, `battle_init_enemy_combatants.cpp`, `battle_kernel_materia_support.cpp`, `battle_kernel_*.cpp`, `battle_init_exp_results.cpp`, `battle_write_party_rewards.cpp` | `Kernel_LoadKernelBinFiles`, `Battle_LoadPartyActorsFromEquipment`, `Battle_InitEnemyCombatants`, `Kernel_ApplyExperienceToCharacterRecord` |
| ATB, commands, events | `battle_command_ring_atb.cpp`, `battle_command_menu_queue.cpp`, `battle_actor_event_handlers.cpp`, `battle_action_queue_run.cpp`, `battle_sequence_step_records.cpp` | `Battle_TickAtbAndGauges`, `Battle_EnqueueAction`, `Battle_PostEvent`, `Battle_RunActionQueue` |
| Actions and damage | `battle_attack_op_handlers.cpp`, `battle_damage_program_run.cpp`, `battle_damage_hit_checks.cpp`, `battle_damage_formula_set.cpp`, `battle_hit_damage_resolution.cpp`, `battle_special_attack_effect.cpp`, `battle_hp_mp_commit.cpp`, `battle_status_effect_handlers.cpp`, `battle_steal_command_effects.cpp` | `Battle_RunDamageProgram`, `Battle_DmgFormula01_Physical`, `Battle_ResolveHitOnTarget`, `Battle_RunSpecialAttackEffect` |
| AI VM | `battle_ai_vm_interpreter.cpp`, `battle_ai_script_dispatch.cpp`, `battle_hit_damage_resolution.cpp` | `Battle_AiVm_Run`, `Battle_RunAiScriptEvent`, `Battle_AiVm_PostAction` |
| Camera | `battle_camera_channel_scripts.cpp`; summon effects carry their own camera scripts (`BattleFx_*RunCameraScript`) | `Battle_RunCameraEyeScript`, `Battle_RunCameraFocusScript`, `Battle_PublishCameraForFrame` |
| Motion and task slots | `battle_actor_anim_effect_tasks.cpp`, `battle_hit_reaction_task_slots.cpp`, `battle_resource_slots.cpp`, `battle_actor_effect_sprites.cpp`, `battle_actor_overlay_draw.cpp` | `Battle_RunActorAnimScript`, `Battle_AllocateResourceSlot`, `Battle_RunMainTaskSlots` |
| Effects | `battle_fx_magic_*` (51), `battle_fx_special_*` (118, category 32 table), `battle_fx_limit_*` (35), `battle_fx_summon_*` (16), `battle_fx_blue_*` (24, enemy skills), `battle_fx_throw_*` (7), `battle_fx_weapon_*` (5), `yasui/`, `myoshiok/` | `BattleFx_MagicEnterFire`, `BattleFx_SummonEnterKnightsOfRound`, `BattleFx_LimitEnterGalianBeast` |
| 3D scene and drawing | `b3ddata.cpp`, `battle_actor_overlay_draw.cpp`, `battle_fx_model_draw.cpp`, `battle3d/*.cpp` | `Battle_DrawLayer`, `Battle3d_SweepActorDrawTable`, `BattleModel_LoadPartyMember`, `BattleModel_LoadEnemy` |

The effect TUs share one shape. `BattleFx_MagicLoadResFire` creates the resource
(`BattleFx_BuildResourcePath("MAGIC/FIRE")`), `BattleFx_MagicEnterFire` is the table entry: it
calls the loader, spawns per-target tasks with `Battle_CreateBitPatternCallbackTask` and plays the sound,
and the `...Task` functions run once per frame from a task slot.

A main task slot is 32 bytes (100 of them at `0xBFB718`, 60 secondary ones at `0xBFC3A0`). Its +0
is the allocating task's slot index (`0xFFFF` ends the task) and +2 counts the task's runs; the
rest is the effect's own. Each task reads its slot through its own struct,
`BattleFx<Effect><Task>Slot` (`ownerSlot`, `frameCounter`, then the task's fields). Both tables are in
`g_BattleTaskSlotBuffer` (`BattleTaskSlotBuffer`): `mainSlots[100]` at +0x20 and
`secondarySlots[60]` at +0xCA8, of `BattleTaskSlot`; the running task's slot is
`BATTLE_CURRENT_TASK_SLOT`.

## Main structures

| type or global | header / TU | what it holds |
|---|---|---|
| `BattleCommandContext`, `g_pBattleCommandCtx` (0x0099ce0c) | [include/ff7/battle.h](../../../../include/ff7/battle.h), storage in `yama/init.cpp` | The working set of the action being executed: attacker, targets, power, damage, result flags. 0x400 bytes at 0x0099e308; only touched fields are named |
| `BattleScriptVm`, `g_BattleAiVm` (0x00c3f330) | `include/ff7/battle.h`, `battle_ai_vm_interpreter.cpp` | AI VM: stack pointer, two lanes of ten variables, 0x200-byte stack. Overlays the command context |
| `BattleCombatant` | `include/ff7/battle.h` | 10 records of 0x68 at 0x009ab0dc (`g_aCombatants`, `gBattleAiGlobals.combatants`): status, flags, HP, MP and stats |
| `BattlePartyActor` | [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h) | 3 records of 0x34 at 0x009a8db8 (`g_BattleState.partyStats`, `g_aBattlePartyStat`) |
| `BattleAiGlobals`, `gBattleAiGlobals` (0x009ab0a0) | [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h), `battle.cpp` | 0x94c bytes: actor and target masks, flags, formation id, arena flags; the ten combatants at +0x3C and each actor's 0x80 bytes of AI variables at +0x44C |
| `g_BattleScene` (`BattleSceneState`, 0x178), `g_BattleState` (`BattleState`, 0x222c), `g_BattleSequenceBuffer` (`BattleSequenceBuffer`, 0x30C) | `battle.cpp`, [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h) | Scene header copied from `scene.bin`; per-actor timing records (stride 0x44), party stats and the scene's enemy and attack blocks; the sequence buffer, a `BattleSequenceBuffer`: `steps[64]` (`BattleSeqStep`, 12 bytes) and three words after them. Zeroed by `Battle_ClearWorkRam` |
| `Battle3dNode` | [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h) | A 0x34-byte model node: GTE matrix and translation, rotation, position, parent matrix (`Battle_SetNodeParentMatrix`). `g_BattleCameraTransform` is one, `g_BattleStageNodes` ten, `BattleEffectTrailState` holds three |
| `g_BattleRewardTotals`, `g_BattleRewardItems` | `yama/init.cpp` | The result screen's totals, `BattleRewardTotals`: `gil` and a `BattleRewardExpRow` per party member (exp to the next level, the exp bar, the level shown); and four `BattleRewardItem`s (`itemId`, `count`) |
| `BattleAction`, `BattleActionQueue`, `BattleActionQueueState` | [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h) | One queued action (type, actor, flags, target mask) and the state words of `Battle_RunActionQueue` |
| `BattleHitRecord`, `BattleSeqStep`, `BattleDamageDisplayRecord` | [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h) | What the logic hands to the 3D side: hit results, sequence steps, damage numbers |
| `BattleAttackEntry` | [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h) | One attack record copied into the command context |
| `SavemapCharacter` | [include/ff7/savemap.h](../../../../include/ff7/savemap.h) | A savemap character record (stride 0x84), used by the reward code |
| `g_BattleActorRecords` (0x00be1178) | `battle_actor_overlay_draw.cpp` | 3D actor records, 10 x `struct BattleActor` (0x1AEC, `ff7/types/battle.h`): `effectCategory`, `effectId`, root node `posX`/`posY`/`posZ` and `yaw`, `model`. Bone nodes inside it are still byte arrays |
| `g_pBattle3dScene` (0x009adfd8), `Battle3dScene` | `b3ddata.cpp`, [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h) | The scene: stage models, model-table bank, render lists drawn by `Battle_DrawLayer` |
| Task slot arrays | allocators and runners in `battle_resource_slots.cpp`; storage in `battle_actor_overlay_draw.cpp` and other TUs | 100 main (`g_BattleTaskSlotBuffer.mainSlots`), 10 overlay, 60 secondary and 16 aux (camera) slots of callback + state; the effects run in them |
| `Battle3dPartyLoadDesc` | [include/ff7/types/battle.h](../../../../include/ff7/types/battle.h) | Load descriptor for party, enemy and limit model banks |

`include/ff7/types/battle.h` holds core battle types and brings in the eight domain headers
`include/ff7/types/battle_{3d,actors,boss,fx,limit,magic,special,summon}.h` for 3D scene,
actor, boss, summon, limit, magic, and special-effect records.

## Data flow

1. Startup (not per battle): `Kernel_LoadKernelBinFiles` reads `kernel/window.bin`, then `kernel/kernel.bin`
   section by section through `Kernel_ParseKernelBin` into the nine buffers of
   `g_KernelBinSectionTargets` (two token buffers, `g_BattleKernelData`, the savemap character
   records, and item, weapon, armour, party-data and materia buffers, by their names; type 9
   sections become text banks), then the LZS file `kernel/kernel2.bin` as 18 text banks (`Kernel_LoadKernel2Bin`). The
   archives `battle/battle.lgp` and `battle/magic.lgp` are named in ff7/main, not here.
2. Enter: `Battle_InitSession` clears the work blocks, saves the savemap party bytes
   (`gSavemap`+0x4F8), reads the scene id from `g_FieldRequestedBattleId` and sets the data
   directory to `<data>BATTLE` and the effect resource root.
3. Scene load (phase 2): `Battle_LoadSceneFirstRound` builds party stats from the savemap
   (`Savemap_BuildPartyStatsFromRecord`, `Battle_LoadCombatantFromSavemap`), reads battle speed and
   ATB mode from the savemap config, and calls `Battle_LoadSceneRecord`. That reads a 0x2000-byte
   chunk of `scene.bin`, inflates the wanted entry and copies its ten blocks (scene header, the
   formation's setup, camera and actor-table blocks, enemy records, attack entries, attack ids and
   three more blocks) into the battle globals. Then enemies
   are set up, AI init events run and the initial ATB gauges are seeded.
4. Opening (phase 4): `Battle_RunOpeningSequence` calls `Battle3d_LoadStageAndCameraData`, which
   loads one of `camdat0.bin`..`camdat2.bin` (table `g_aBattleCameraDataFiles`) and the stage model;
   party and enemy model banks are loaded (`BattleModel_LoadPartyMember`, `BattleModel_LoadEnemy`,
   legacy files `ENEMY%03d.DAT`).
5. Per frame, `Battle_ModuleFrame` calls `Battle_RunPhaseMachine` and then the UI:
   `Battle_UpdateUiAndLogic` (ff7/menu) reads input, runs the command menus and calls
   `Battle_TickAtbAndGauges`. A full gauge requests a menu or posts an AI action; the player's choice
   comes back through `Battle_PostCommand`.
6. Main loop (phase 7): `Battle_RunActionQueue` takes actions by priority and calls
   `Battle_ExecuteAction`. That runs a byte program from `g_BattleAttackPrograms` through
   `g_BattleAttackOpCallbacks` (0x1F ends it). Op 9, `Battle_AttackOpExecute`, resolves targets and
   calls `Battle_ResolveHitOnTarget` per target, which runs the damage program
   (`Battle_RunDamageProgram`: check ops from `g_BattleDamageOpTable`, then one
   `Battle_DmgFormula*` from `g_BattleDamageFormulaTable`), added effects
   (`Battle_RunSpecialAttackEffect`), status changes and `Battle_CommitHpMpChange`. AI scripts run
   through `Battle_RunAiScriptEvent` -> `Battle_AiVm_Run`, called from `Battle_RunActionQueue`,
   `Battle_ExecuteAction`, the scene-load init events and a few other sites
   (`Battle_AttackOpRunAiEventOnTargets`, `Battle_HandleActorDeath`, `Battle_RunEndSequence`).
7. The logic writes `BattleSeqStep`, hit and damage-display records. `Battle_PlaySequence` ->
   `Battle_RunSequenceStep` -> `Battle_Step3dFrame` -> `Battle_Update3dScene` plays them: actor
   motion scripts (`Battle_RunPartyActorScripts`, `Battle_RunEnemyActorScripts`,
   `Battle_RunActorAnimScript`), the pending effect (`Battle_RunActorEffect` picks a table by
   category: 2 magic, 3 summon, 13 enemy skill, 20 limit, 32 enemy attack), camera
   (`Battle_RunCameraChannels`, `Battle_PublishCameraForFrame`), then `Battle_RunEffectTaskSlots` and
   `Battle_RunMainTaskSlots`.
8. Phase 9 applies time slices and status transitions; phase 11 recomputes masks, snapshots HP/MP
   and sets the end flags in `gBattleAiGlobals`+0x22 (party gone, enemies gone, countdown expired).
9. To graphics: `Battle_ModuleFrame` draws `Battle_DrawLayer(0..3)`, the actor draw table and HUD
   sprites through lib/graphics (`Gfx_*`, `GfxFlushFrameDrawList`). To sound: effects call
   `Battle_DispatchSfxOpcodeDirect` and lib/sound; the frame ends with `SoundUpdateFades`.
10. End (phases 14..16): unless end flags 0x1E are set, phase 15 takes the next scene id from the
    formation's setup block (for an arena battle `Battle_RunArenaNextBattleChooser` picks it); if
    there is one, the machine restarts at phase 0. Otherwise phase 16 plays the last sequence and
    runs `Battle_RunEndSequence`, which posts the result messages, sums gil into
    `g_BattleRewardTotals`, writes items, EXP, AP and limit progress into the savemap
    (`Kernel_ApplyExperienceToCharacterRecord`, `Battle_AwardApToEquippedMateria`,
    `Battle_WritePartyStateBackToSavemap`). Then phase 16 raises `gSavemap.escapesLo` when
    battle flag 8 is set, chooses the next game mode and `RequestModuleSwitch` hands control back
    to `GameModeDispatcher_*`.

## What the rules are

What the routines players and modders ask about do, in game terms. Each routine's header comment has
the detail, and says where a rule is the community Battle Mechanics FAQ's (well established) rather
than proven by the code alone.

### Damage

- An attack's damage calculation byte picks a hit-check program (high nibble) and a formula (low
  nibble): `Battle_DecodeDamageCalcByte`, with the sixteen programs listed above
  `g_abDamageStatProgramBlob` (0 none, 1 physical hit + critical, 2 magic hit, 8 the "Level N" check,
  9 Manipulate, 6/7 the same checks with formulas 0x10..0x1F). `Battle_RunDamageProgram` runs the
  checks, then the formula.
- `Battle_DmgFormula01_Physical`: base = Att + [(Lvl + Att) / 32] * [(Lvl * Att) / 32], times
  (512 - Def) * power / 8192; then critical x2, Berserk x3/2, back row /2 (short-range and enemy
  attacks only), Defend /2, back attack x damageScale/8 (x2 on the party), Frog /4, then the shared
  modifiers. `Battle_DmgFormula02_Magical`: base 6 * (MAt + Lvl) against Magic Defence.
- The shared modifiers: Sadness -30% (`Battle_ApplySadnessReduction`), 2/3 on each of several targets
  or 1/2 on a repeated cast (`Battle_ApplyMultiTargetSplit`), Barrier/MBarrier 1/2 and the damage
  bonus (`Battle_ApplyBarrierAndDamageBonus`), and the variance 3841..4096 / 4096 with a minimum of 1
  (`Battle_ApplyDamageVariance`).
- The other formulas: 3/4 a fraction of current/max HP or MP, 5 the cure formula
  22 * power + 6 * (MAt + Lvl), 6 fixed 20 * power, 7 power against defence alone, 8 full recovery
  (Death on an absorbing target), 0xA a fixed total shared among targets, 0x10/0x11 the attacker's HP
  left/lost, 0x18 dice, 0x19 battles escaped, 0x1A down to 1 HP, 0x1B the play clock, 0x1C ten per
  kill, 0x1D 1111 per equipped materia.

### Hit and critical

- `Battle_DmgOpPhysicalHitCheck`: accuracy + Dex/4 + the attacker's Def% - the target's Def%, -30%
  under Fury; a back attack, a helpless target (Sleep, Stop, Paralysis, ...), Cover and the deciding
  element reactions always hit; Luck/4 gives a lucky hit, and a party member's Luck/4 a lucky dodge
  against enemies. A physical hit wakes the target and ends Confusion and Manipulate.
- `Battle_DmgOpMagicalHitCheck`: misses when 0..99 is below the target's magic evade, or when 1..100
  reaches accuracy + Lvl - target Lvl/2; reflectable spells on Reflect targets and damage-only spells
  on helpless targets always hit.
- `Battle_RollCriticalHit`: (Luck + Lvl - target Lvl) / 4 + the weapon's critical bonus, every hit
  under Lucky Girl. `Battle_DmgOpLevelDivisibleCheck`: L5 Death and its kin hit levels divisible by N.
  `Battle_DmgOpManipulateCheck`: Lvl + 50 - target Lvl.
- `Battle_ResolveAttackFacing` decides back attacks and who turns round.

### How a hit resolves on a target

- `Battle_AttackOpExecute` (attack op 9) runs an action in rounds (repeats, follow-up attacks) and
  passes: the attacker's targets first, then each Reflect bounce as a new source. Multi-hit, random
  and repeat-cast spells hit random single targets (a summon never does); `repeatCasts` re-runs the
  pass on the surviving targets.
- Each hit is `Battle_ResolveHitOnTarget`. Cover may take it first (`Battle_ApplyCoverRedirect`: a
  single-target physical hit on a party member). `Battle_BeginTargetHit` loads the target, rolls the
  status and builds its immunities.
- Elements: `Battle_ResolveElementAffinity` folds the target's affinities
  (`Battle_BuildElementAffinityTable`) into `elemReaction`. `Battle_ApplyElementalPreDamage` swaps
  inflict and cure on Absorb, so a revive kills a target that absorbs it; after the damage program,
  `Battle_ApplyElementalPostDamage` doubles for Weak, halves (rounding up) for Half, turns damage into
  healing for Absorb, and applies instant Death, full recovery or Nullify.
- Reflect bounces a reflectable attack back at the caster from a foe, or to a random foe from an
  ally; it wears off on its fourth bounce (`Battle_OnStatusReflectSet` stores 3). KO-only attacks
  (revives) miss living targets.
- Damage is capped at 9999 HP / 999 MP for enemies and at the member's own caps for the party.
  Immune, Petrified and Peerless targets take 0, and an attacker at exactly 7777 HP deals 7777 (All
  Lucky 7s). HP and MP Absorb return 1/10 and 1/100 of the damage. A party member hit by an enemy
  skill may learn it (`Battle_LearnEnemySkill`).

### How a status lands

- `Battle_RollStatusChangeSuccess`: the attack's chance; 0xFC or more always lands, as does undoing
  Frog or Small, and Haste, Shield or Berserk on a party member. Otherwise the bonus percent is added,
  2/3 over several targets, 1/2 on a repeated cast, and it lands when 1..100 falls below it. A failed
  roll makes a status-only attack miss.
- `Battle_BuildStatusImmunityMask`: the actor's own immunities, plus Resist and Peerless, Haste and
  Slow coupled, and Death immunity implying Death Sentence immunity.
- On a hit, Haste and Slow cancel each other, as do Sadness and Fury (`g_BattleOpposedStatusPairs`);
  then status = ((status | inflict) & ~cure) ^ toggle, each mask without the immunities.
- `Battle_DispatchStatusTransitions` turns the changed bits into effects through the set and clear
  handler tables. A KO keeps only Death, Frog, Small, the capture bit and the statuses that cannot be
  lost. Handlers include `Battle_RemoveActorAndAwardSpoils` (KO), `Battle_OnStatusDeathCleared`
  (revive), `Battle_OnStatusIncapacitateSet`/`Cleared` (Sleep, Stop, Petrify, Paralysis),
  `Battle_RefreshSlipHpRate` (Regen and Dual Drain) and the timer starter
  (`g_BattleStatusInitialDurations`).
- Original bug: a reflector with no foe left bounces the spell onto slot 0, the first party member
  (`Battle_ResolveHitOnTarget`).

### ATB and turns

- `Battle_SetSpeedFromConfig` turns the Battle Speed setting into the clock step (273 at 0 down to
  54 at 255). `Battle_RefreshActorTickRates` gives each combatant its gauge rate: the step times
  (Dex + 50) / (the party's average Dex + 50) for the party, times Dex / the same divisor for
  enemies; normal speed is double Slow and Haste double normal; Sleep, Petrify, Paralysis, Stop and
  Death stop the gauge.
- `Battle_TickAtbAndGauges` fills the gauges each frame; a full one opens the command menu, posts
  the AI's action or the auto-control command. It also runs the escape counter (hold L1+R1; one step
  each 0x2000 clock units held for more than half of it, at 1-in-4 odds except in pre-emptive and
  side attacks) and fills the limit gauges. `Battle_IsAtbFrozen` stops the clock in Wait mode in a
  submenu and when party commands back up in Recommended and Wait mode.
- `Battle_ServiceTurnQueueAndEscape` moves posted commands into the action queue, refuses escape in a
  pincer, when the formation forbids it or when nobody can move, and picks idle poses.
  `Battle_EnqueueAction`: Defend lasts until the defender's next action.
- `Battle_ApplyTimeSlices` (after each action): Regen and Dual Drain slip by max HP / 32 per unit,
  Stop counts down on the battle clock alone, Death Sentence and Slow-numb follow Haste and Slow,
  and every other timer follows the combatant's speed.

## Named codes

From [include/ff7/enums/battle.h](../../../../include/ff7/enums/battle.h):

| enum | used in | what it names |
|---|---|---|
| `BattleMotionOp` | `battle_actor_anim_effect_tasks.cpp` | Motion-script commands 0x8E..0xFF of `Battle_RunActorAnimScript`; lower bytes are animation ids |
| `BattleCameraEyeOp` | `battle_camera_channel_scripts.cpp` | Camera eye (position) script opcodes, `Battle_RunCameraEyeScript` |
| `BattleCameraFocusOp` | `battle_camera_channel_scripts.cpp` | Camera focus (look-at) script opcodes, `Battle_RunCameraFocusScript` |
| `BattleSummonCameraMode` | `battle_fx_summon*.cpp`, `yasui/yasui_summon_knights_of_round.cpp` | Node modes of the per-summon camera script interpreters |
| `BattleAiOp` | `battle_ai_vm_interpreter.cpp` | AI arithmetic and compare opcodes 0x30..0x45 |
| `BattleAiOpGroup` | `battle_ai_vm_interpreter.cpp` | AI opcode high nibble dispatched by `Battle_AiVm_Run` |
| `BattleAddedEffect` | `battle_special_attack_effect.cpp` | Attack added-effect ids run by `Battle_RunSpecialAttackEffect` |
| `BattlePhase` | `battle.cpp` | The phases of `Battle_RunPhaseMachine` in `g_BattlePhase` |
| `BattleEffectCategory` | `battle_actor_anim_effect_tasks.cpp` | The effect categories `Battle_RunActorEffect` picks a table by |
| `BattleAttackProgramOp` | `battle_action_queue_run.cpp` | Two ops of an attack program (`g_BattleAttackOpCallbacks`): await and end; the other ops are bare numbers |
| `BattleActorMask` | battle and `btlmenu` TUs | 10-bit combatant masks (`BATTLE_ACTOR_MASK_PARTY` 0x007, `BATTLE_ACTOR_MASK_ALLIES` 0x00F, `BATTLE_ACTOR_MASK_ENEMIES` 0x3F0, `BATTLE_ACTOR_MASK_ALL` 0x3FF) |

## Notes

What that looks like in the code:

- Some `DAT_*` symbols in battle TUs keep their linker names and are accessed in function bodies via descriptive `#define` aliases
  (`g_BattleActionQueuePhase`, ...).
- The command context is read through `BattleCommandContext` members; the one raw access is the
  loop in `Battle_RunActionQueue` that clears its 0x100 words. Combatants are
  `g_aCombatants[i].member` and 3D actors `g_BattleActorRecords[actor].member`; reads into a bone
  node (`+ bone * 0x34`) still add offsets.
- `g_MenuEntryState` (0x00cc0828) is how a menu is entered over a
  module: `Battle_InitOpeningUiState` sets 2 and `Battle_ModuleFrame` runs the battle UI while it
  is 2; the end sequence sets 4 for the result screens.
- `g_BattleEffectScratch.targetMask` holds the active effect's target mask, passed as the first argument (`targetMask`) to every two-parameter `BattleFx_*Enter*` entry point alongside `actorIdx`.
- `Battle_AiVm_Run` keeps its stack frame as a union of named slots (see `lib/codegen.h`).
- Several effect TUs are long because of embedded byte arrays (meshes, textures), not code: for
  example `battle_fx_special_hidden_one_card.cpp` (about 5,700 lines, 16 functions).

## Data-only files

Data runs that their link neighbour never references, each in a file of its own:

| file | span | next to | what it holds |
|---|---|---|---|
| `battle_actor_buffer_pointers.cpp` | .data 0x008fe2ac..0x008fe2b8 | `battle_actor_effect_sprites.cpp` | the actor animation buffer and effect primitive pointers |

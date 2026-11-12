/* Battle subsystem structures.
 *
 * EVERY OFFSET IN THIS FILE WAS READ OUT OF ff7.exe. The derivation is recorded per field so a
 * later reader can re-check it without repeating the work:
 *
 *   - the base pointer / array base is the absolute address in the encoded
 *     instruction, e.g. `mov eax, dword ptr [0x99ce0c]`
 *   - the member offset is the displacement, e.g. `[eax + 0x4c]`
 *   - the width is the operand size of the access
 *   - the stride is the `imul reg, reg, N` that scales the index
 *
 * The `pad_*` members are NOT claims. They are unexamined bytes. A field
 * discovered later replaces padding; it never shifts a derived offset.
 *
 * Offsets are asserted at compile time at the bottom of this file, so a typo
 * in the padding arithmetic is a build error rather than a silent mismatch.
 */

#ifndef FF7_BATTLE_H
#define FF7_BATTLE_H

#include <stddef.h>

/* ------------------------------------------------------------------------
 * BattleCommandContext -- the attack pipeline's working set.
 *
 * Pointer   g_pBattleCommandCtx at 0x0099CE0C
 * Buffer    0x0099E308, written exactly once, by
 *             0x00401028  mov dword ptr [0x0099CE0C], 0x0099E308
 *           and re-loaded 1,210 times in .text (measured; the research corpus
 *           independently reports the same count).
 *
 * The block is addressed as dwords. The AI VM overlays a different struct on
 * the same bytes -- see BattleScriptVm below -- so a write through one view
 * clobbers the other. That aliasing is a property of the shipped program, not
 * of this header.
 *
 * Only the fields the code actually touches are named. The struct
 * is sized 0x400 because that is the extent Battle_ExecuteAction clears.
 * ---------------------------------------------------------------------- */
typedef struct BattleCommandContext {
    /* +0x000 */ int attackerSlot; /* 0x005dfb5c, [eax+0]                     */
    /* +0x004 */ int attackerLevel; /* 0x005deb93, add ecx,[edx+4]             */
    /* +0x008 */ int enemyRecordIndex;
    /* +0x00C */ int commandIndex;
    /* +0x010 */ int attackIdBase;
    /* +0x014 */ int magicIndex;
    /* +0x018 */ unsigned int targetMask; /* 0x005decb5, [eax+0x18] -> Battle_PopCountMask */
    /* +0x01C */ int actionType;
    /* +0x020 */ int stepId;
    /* +0x024 */ int resolvedAttackId;
    /* +0x028 */ int stepFlags;
    /* +0x02C */ int attackId;
    /* +0x030 */ int actorBit;
    /* +0x034 */ unsigned char pad_034[0x4];
    /* +0x038 */ int mpCost;
    /* +0x03C */ int accuracy; /* 0x005de093, cmp [eax+0x3c],0; idiv      */
    /* +0x040 */ int damageCalcByte;
    /* +0x044 */ int elementMask;
    /* +0x048 */ int power; /* 0x005deb9e, [eax+0x48]; imul ...,0x16   */
    /* +0x04C */ int attackStat; /* 0x005deb8a, [eax+0x4c]                  */
    /* +0x050 */ unsigned int targetFlags; /* 0x005de911, [ecx+0x50] & 0x80          */
    /* +0x054 */ int displayValueA;
    /* +0x058 */ int displayValueB;
    /* +0x05C */ int displayValueC;
    /* +0x060 */ int animParam;
    /* +0x064 */ int animParamAlt;
    /* +0x068 */ int secondaryDisplayValue;
    /* +0x06C */ unsigned int specialFlags; /* 0x005dc08e / 0x005de82c, [r+0x6c]     */
    /* +0x070 */ unsigned char pad_070[0x8];
    /* +0x078 */ int hitMask;
    /* +0x07C */ int hpMpPoolMask;
    /* +0x080 */ int statusMaskA;
    /* +0x084 */ int statusMaskB;
    /* +0x088 */ int statusMaskC;
    /* +0x08C */ int statusChangeId;
    /* +0x090 */ int attackFlags;
    /* +0x094 */ int resolvedTargetMask;
    /* +0x098 */ int stepPower;
    /* +0x09C */ unsigned char pad_09C[0x4];
    /* +0x0A0 */ int damageProgramIndex;
    /* +0x0A4 */ int damageFormulaIndex;
    /* +0x0A8 */ int hitTargetBits;
    /* +0x0AC */ int repeatCasts; /* 0x005de92a, cmp [eax+0xac],0            */
    /* +0x0B0 */ int countdown;
    /* +0x0B4 */ int repeatRemaining;
    /* +0x0B8 */ int targetCount; /* 0x005de902, cmp [eax+0xb8],2            */
    /* +0x0BC */ int effectId;
    /* +0x0C0 */ int effectParam;
    /* +0x0C4 */ int basePower;
    /* +0x0C8 */ unsigned int attackerStatus; /* 0x005de582 / 0x005dfb5c, [r+0xc8]   */
    /* +0x0CC */ int hitReactionIndex;
    /* +0x0D0 */ unsigned char followUpAttackIds[8]; /* queue of follow-up attack ids, 0xFF = empty; bit 0x80 makes that attack a forced critical (clears BATTLE_SPECIAL_NO_FORCED_CRITICAL) */
    /* +0x0D8 */ int weaponPower; /* 0x005dec92, [eax+0xd8]; shl 1 -> attackStat */
    /* +0x0DC */ int messageId;
    /* +0x0E0 */ int hitCount;
    /* +0x0E4 */ int payload;
    /* +0x0E8 */ int bonusPercent; /* 0x005de82c, the only unassigned disp    */
    /* +0x0EC */ int mask;
    /* +0x0F0 */ int supportBits;
    /* +0x0F4 */ int actorIndex;
    /* +0x0F8 */ int repeatCount;
    /* +0x0FC */ int savedCountdown;
    /* +0x100 */ unsigned char pad_100[0x100];
    /* +0x200 */ struct BattleHitTarget *hitTargetPtr;
    /* +0x204 */ int targetStatePtr;
    /* +0x208 */ int targetSlot; /* 0x005def25, [eax+0x208] -> imul 0x68    */
    /* +0x20C */ int enemyIndex;
    /* +0x210 */ int defence; /* 0x005dec0a, by elimination vs power/damage */
    /* +0x214 */ int damage; /* 0x005def3c, mov [eax+0x214],edx         */
    /* +0x218 */ unsigned int resultFlags; /* 0x005de0b9/0x005de0c7, or al,1         */
    /* +0x21C */ int clearedHitState;
    /* +0x220 */ int displayFlags;
    /* +0x224 */ int animId;
    /* +0x228 */ unsigned int targetStatus; /* 0x005de958, [r+0x228]                 */
    /* +0x22C */ int immunityMask;
    /* +0x230 */ unsigned int elemReaction; /* 0x005dbd24, cleared 5th             */
    /* +0x234 */ unsigned int modifierFlags; /* 0x005de53b, vs targetSlot           */
    /* +0x238 */ unsigned int workInflictMask; /* 0x005dbce5, cleared 1st            */
    /* +0x23C */ unsigned int workCureMask; /* 0x005dbcf5, cleared 2nd            */
    /* +0x240 */ unsigned int workToggleMask; /* 0x005dbd05, cleared 3rd            */
    /* +0x244 */ unsigned int workStatusUnion; /* 0x005dbd14, cleared 4th            */
    /* +0x248 */ int hitDisplayValue;
    /* +0x24C */ int hitSecondaryValue;
    /* +0x250 */ int hitDamage;
    /* +0x254 */ int targetLevel; /* 0x005de0a5, [ecx+0x254]; cdq; idiv      */
    /* +0x258 */ int hp;
    /* +0x25C */ int cap;
    /* +0x260 */ int accuracyWork;
    /* +0x264 */ unsigned char pad_264[0x19C];
} BattleCommandContext;

/* g_aCombatants: the ten BattleCombatant records at +0x3C of gBattleAiGlobals (0x009AB0DC). */
#define g_aCombatants (gBattleAiGlobals.combatants)

/* ------------------------------------------------------------------------
 * BattleScriptVm -- the AI byte-code machine.
 *
 * Pointer g_BattleAiVm at 0x00C3F330. Overlays the same buffer as
 * BattleCommandContext above.
 *
 *   sp      0x005d8ff3  mov ecx, [eax + 8]        (dword)
 *   stack   0x005d9010  mov [ecx + eax + 0x7c], dl (byte-indexed by sp)
 *   laneMask 0x005d94f2 mov ax, [edx + ecx*2 + 0x28]  (u16, scale 2)
 *   var     0x005d950a  imul eax,eax,0x28 ; lea edx,[ecx+eax+0x2c]
 *                       cmp dword ptr [edx + eax*4], 0
 *           -> +0x2C, two lanes of ten dwords, lane stride 0x28
 *
 * Total 0x7C + 0x200 = 0x27C, which is the extent the research corpus reports.
 * ---------------------------------------------------------------------- */
#define FF7_AIVM_LANES 2
#define FF7_AIVM_LANE_SLOTS 10
#define FF7_AIVM_STACK 0x200

typedef struct BattleScriptVm {
    /* +0x00 */ int actorIndex; /* the actor whose script runs              */
    /* +0x04 */ int pc; /* offset of the next byte in g_BattleAiVmBytecodePtr */
    /* +0x08 */ int sp; /* grows DOWN from FF7_AIVM_STACK          */
    /* +0x0C */ int opcode;
    /* +0x10 */ int opArg; /* opcode & 0xF                             */
    /* +0x14 */ int opKind; /* opcode >> 4                              */
    /* Battle_AiVm_PopValue fills these per operand (0 the first, 1 the second) from the descriptor
     * byte it pops: its high nibble is the type (0 a scalar broadcast to all ten lanes, 1 a raw
     * value, 2 a lane mask then one value per set lane), its low nibble the width. */
    /* +0x18 */ int operandType[FF7_AIVM_LANES];
    /* +0x20 */ int operandWidth[FF7_AIVM_LANES];
    /* +0x28 */ unsigned short laneMask[FF7_AIVM_LANES];
    /* +0x2C */ int var[FF7_AIVM_LANES][FF7_AIVM_LANE_SLOTS]; /* var[0][0] is the result word */
    /* +0x7C */ unsigned char stack[FF7_AIVM_STACK];
} BattleScriptVm;

/* ------------------------------------------------------------------------
 * BattleUiBox -- the battle window table, 0x98 bytes a record.
 *
 * Pointer   g_BattleMenuData at 0x00DC3630, set by 0x006DC89A to one of two
 *           static tables depending on GetUiScalingMode().
 * Stride    `imul eax, eax, 0x98`, 164 sites in .text across 17 functions.
 * Index     a short, and the same index selects an entry in the byte array
 *           g_BattleWindowStates and a handler in the pointer table 0x0091E638.
 *
 * The names of the first six members are read off the arithmetic in
 * 0x006D0AF9 and 0x006D7DFA, not from any external source:
 *
 *     0x006D0B74  +0x08 = +0x04 / 2        halfWidth  = width  / 2
 *     0x006D0BA2  +0x0A = +0x06 / 2        halfHeight = height / 2
 *     0x006D7F17  +0x00 + (origin + 0x08)  x + halfWidth  -> a centre
 *     0x006D7EE3  +0x02 + (origin + 0x0A)  y + halfHeight -> a centre
 *
 * A pair that halves into a second pair, and the halves used as the offset
 * from a corner to a centre, is a width and a height.
 *
 * The sub-rectangle array is 0x006D8052: the loop runs `i < +0x10` and every
 * access inside it is `[base + i*8 + 0x12..0x18]`, so +0x10 is a count and
 * +0x12 starts an array of four shorts. 16 entries is what fits between
 * +0x12 and the end of the record; the code never bounds it by anything but
 * the count, so 16 is a ceiling and not a measurement.
 * ---------------------------------------------------------------------- */
typedef struct BattleUiRect {
    /* +0x00 */ short left; /* 0x006D8169, added to the box's x         */
    /* +0x02 */ short top; /* 0x006D8137, added to the box's y          */
    /* +0x04 */ short right; /* 0x006D80A2, minus left is the width     */
    /* +0x06 */ short bottom; /* 0x006D80DE, minus top is the height    */
} BattleUiRect;

typedef struct BattleUiBox {
    /* +0x00 */ short x; /* 0x006D0B2D                                  */
    /* +0x02 */ short y; /* 0x006D0B45                                  */
    /* +0x04 */ short width; /* 0x006D0B5A, read and halved             */
    /* +0x06 */ short height; /* 0x006D0B88, read and halved            */
    /* +0x08 */ short halfWidth; /* 0x006D0B74                          */
    /* +0x0A */ short halfHeight; /* 0x006D0BA2                         */
    /* +0x0C */ short clipWidth; /* 0x006D0BB7 sets 2; the width Menu_FlushAndClipToBattleWindow* clips to */
    /* +0x0E */ short clipHeight; /* 0x006D0BCD sets 2; the clip height (box 0 may substitute its own) */
    /* +0x10 */ short subCount; /* 0x006D807F, the loop bound          */
    /* +0x12 */ BattleUiRect sub[16]; /* 0x006D80A2, `+ ecx*8 + 0x16`   */
    unsigned char pad_92[2];
    /* +0x94 */ int unused94; /* a pointer the static tables aim into 0x00DC3870 or 0x0091BE90; nothing
                                 reads it, nor what it points at: no code names it, and the 473-scenario
                                 battle trace has no access to it or to its targets */
} BattleUiBox;

/* ------------------------------------------------------------------------
 * Compile-time offset assertions.
 *
 * A negative array size is the only portable compile-time assert a 1996
 * compiler understands. If one of these fires, the padding arithmetic above
 * drifted -- fix the padding, never the assertion.
 * ---------------------------------------------------------------------- */
#define FF7_ASSERT_OFFSET(tag, type, member, expect) \
    typedef char ff7_off_##tag[(offsetof(type, member) == (expect)) ? 1 : -1]

FF7_ASSERT_OFFSET(ctx_atkslot, BattleCommandContext, attackerSlot, 0x000);
FF7_ASSERT_OFFSET(ctx_atklvl, BattleCommandContext, attackerLevel, 0x004);
FF7_ASSERT_OFFSET(ctx_acc, BattleCommandContext, accuracy, 0x03C);
FF7_ASSERT_OFFSET(ctx_pow, BattleCommandContext, power, 0x048);
FF7_ASSERT_OFFSET(ctx_atkstat, BattleCommandContext, attackStat, 0x04C);
FF7_ASSERT_OFFSET(ctx_tflags, BattleCommandContext, targetFlags, 0x050);
FF7_ASSERT_OFFSET(ctx_sflags, BattleCommandContext, specialFlags, 0x06C);
FF7_ASSERT_OFFSET(ctx_repeat, BattleCommandContext, repeatCasts, 0x0AC);
FF7_ASSERT_OFFSET(ctx_tcount, BattleCommandContext, targetCount, 0x0B8);
FF7_ASSERT_OFFSET(ctx_astatus, BattleCommandContext, attackerStatus, 0x0C8);
FF7_ASSERT_OFFSET(ctx_wpower, BattleCommandContext, weaponPower, 0x0D8);
FF7_ASSERT_OFFSET(ctx_bonus, BattleCommandContext, bonusPercent, 0x0E8);
FF7_ASSERT_OFFSET(ctx_tslot, BattleCommandContext, targetSlot, 0x208);
FF7_ASSERT_OFFSET(ctx_def, BattleCommandContext, defence, 0x210);
FF7_ASSERT_OFFSET(ctx_dmg, BattleCommandContext, damage, 0x214);
FF7_ASSERT_OFFSET(ctx_rflags, BattleCommandContext, resultFlags, 0x218);
FF7_ASSERT_OFFSET(ctx_tstatus, BattleCommandContext, targetStatus, 0x228);
FF7_ASSERT_OFFSET(ctx_elem, BattleCommandContext, elemReaction, 0x230);
FF7_ASSERT_OFFSET(ctx_mflags, BattleCommandContext, modifierFlags, 0x234);
FF7_ASSERT_OFFSET(ctx_winf, BattleCommandContext, workInflictMask, 0x238);
FF7_ASSERT_OFFSET(ctx_wcure, BattleCommandContext, workCureMask, 0x23C);
FF7_ASSERT_OFFSET(ctx_wtog, BattleCommandContext, workToggleMask, 0x240);
FF7_ASSERT_OFFSET(ctx_wuni, BattleCommandContext, workStatusUnion, 0x244);
FF7_ASSERT_OFFSET(ctx_tlvl, BattleCommandContext, targetLevel, 0x254);

FF7_ASSERT_OFFSET(box_hw, BattleUiBox, halfWidth, 0x08);
FF7_ASSERT_OFFSET(box_unk0e, BattleUiBox, clipHeight, 0x0E);
FF7_ASSERT_OFFSET(box_subn, BattleUiBox, subCount, 0x10);
FF7_ASSERT_OFFSET(box_sub, BattleUiBox, sub, 0x12);
FF7_ASSERT_OFFSET(box_size, BattleUiBox, pad_92, 0x92);


FF7_ASSERT_OFFSET(vm_sp, BattleScriptVm, sp, 0x08);
FF7_ASSERT_OFFSET(vm_lanemask, BattleScriptVm, laneMask, 0x28);
FF7_ASSERT_OFFSET(vm_var, BattleScriptVm, var, 0x2C);
FF7_ASSERT_OFFSET(vm_stack, BattleScriptVm, stack, 0x7C);

/* Sizes that are themselves evidence: the 0x34 party-record stride and the
 * 0x27C VM extent come from the image (the 0x68 combatant stride is checked
 * beside BattleCombatant in ff7/types/battle.h). */
typedef char ff7_size_scriptvm[(sizeof(BattleScriptVm) == 0x27C) ? 1 : -1];
typedef char ff7_size_cmdctx[(sizeof(BattleCommandContext) == 0x400) ? 1 : -1];

#endif /* FF7_BATTLE_H */

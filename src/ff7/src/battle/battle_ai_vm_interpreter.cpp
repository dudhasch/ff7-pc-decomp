// src/ff7/src/battle/battle_ai_vm_interpreter.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The battle AI bytecode interpreter: it reads and writes bit, byte, word and dword
//   variables, keeps an operand stack, and evaluates the arithmetic and comparison opcodes for each
//   AI run.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"

#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"

#include "lib/codegen.h"
#include "ff7/enums/main.h"
extern "C" {

/* Kept TU-local: unprototyped callers rely on 32-bit int return rather than unsigned short. */
unsigned short __cdecl Battle_AiVm_Random16(void);

// clang-format off
// GLOBAL: FF7 0x008ff1d8
int g_BattleAiVmAccessSizeTable[3] = {1, 2, 3};
// GLOBAL: FF7 0x00c3f330
BattleScriptVm * g_BattleAiVm = 0;
// GLOBAL: FF7 0x00c3f334
unsigned char *g_BattleAiVmBytecodePtr = 0;
// clang-format on

/*
 * Resolves bit address/base and writes bit, byte, word or dword for widths 0..3. Bit mode uses
 * right shift while larger widths use signed division, differing for negative addresses. Other
 * selectors no write.
 */
// FUNCTION: FF7 0x005d7cf0
void __cdecl Battle_AiVm_WriteVar(int varIndex, unsigned int width, int address, int value)
{
    char *bytePtr;
    void *baseAddr;
    short *wordPtr;
    unsigned char mask;

    address = Battle_AiVm_ResolveAddress(varIndex, address, &baseAddr);
    switch (width) {
    case BATTLE_AI_WIDTH_BIT:
        bytePtr = (char *)baseAddr;
        bytePtr += address >> 3;
        mask = 1 << (address & 7);
        *bytePtr &= (char)~mask;
        if (value != 0) {
            *bytePtr |= mask;
        }
        break;
    case BATTLE_AI_WIDTH_BYTE:
        bytePtr = (char *)baseAddr;
        bytePtr += (address) / 8;
        *bytePtr = value;
        break;
    case BATTLE_AI_WIDTH_WORD:
        wordPtr = (short *)baseAddr;
        wordPtr = (short *)((char *)wordPtr + ((address) / 16) * 2);
        *wordPtr = value;
        break;
    case BATTLE_AI_WIDTH_DWORD:
        baseAddr = (char *)baseAddr + ((address) / 32) * 4;
        *(int *)baseAddr = value;
    }
}

/*
 * Maps bit address below 0x2000 to actor vars, below 0x4000 to shared globals subtracting 0x2000,
 * otherwise combatant subtracting 0x4000. Negative addresses select actor vars unchanged.
 */
// FUNCTION: FF7 0x005d7df2
int __cdecl Battle_AiVm_ResolveAddress(int actorIndex, int bitAddress, void **outBase)
{
    int bitIndex;

    bitIndex = bitAddress;
    if (bitIndex < 0x2000) {
        *outBase = gBattleAiGlobals.actorVars[actorIndex];
    } else if (bitIndex < 0x4000) {
        *outBase = ((unsigned char *)&gBattleAiGlobals);
        bitIndex -= 0x2000;
    } else {
        *outBase = &g_aCombatants[actorIndex];
        bitIndex -= 0x4000;
    }
    return bitIndex;
}

/*
 * Resolves address/base then reads bit, byte, word or dword by width selector 0..3 with
 * bit-address alignment rounding. Other selectors return uninitialized local r.
 */
// FUNCTION: FF7 0x005d7e5c
unsigned int __cdecl Battle_AiVm_ReadVar(int varIndex, unsigned int width, int address)
{
    unsigned short *shortPtr;
    unsigned char *bytePointer;
    int res;
    void *baseAddr;

    address = Battle_AiVm_ResolveAddress(varIndex, address, &baseAddr);
    switch (width) {
    case BATTLE_AI_WIDTH_BIT:
        bytePointer = (unsigned char *)baseAddr;
        res = (*(bytePointer + (address >> 3)) & 1 << (address & 7)) != 0;
        break;
    case BATTLE_AI_WIDTH_BYTE:
        bytePointer = (unsigned char *)baseAddr;
        res = *(bytePointer + (address >> 3));
        break;
    case BATTLE_AI_WIDTH_WORD:
        shortPtr = (unsigned short *)baseAddr;
        res = *(unsigned short *)((char *)shortPtr + (address >> 4) * 2);
        break;
    case BATTLE_AI_WIDTH_DWORD:
        res = *(int *)((char *)baseAddr + (address >> 5) * 4);
    }
    return res;
}

/*
 * Only a lane operand (BATTLE_AI_OPERAND_LANES) collapses: selects first masked lane value or zero
 * if none, sets mask 0x3ff and copies value to all ten lanes. Returns one on collapse else zero.
 */
// FUNCTION: FF7 0x005d7f16
int __cdecl Battle_AiVm_CollapseToScalar(int slot)
{
    int collapsed;
    int laneIndex;
    int firstValue;

    collapsed = 0;
    if (g_BattleAiVm->operandType[slot] == BATTLE_AI_OPERAND_LANES) {
        firstValue = 0;
        for (laneIndex = 0; laneIndex < 0xA; laneIndex++) {
            if ((1 << laneIndex & g_BattleAiVm->laneMask[slot]) != 0) {
                firstValue = g_BattleAiVm->var[slot][laneIndex];
                break;
            }
        }
        g_BattleAiVm->laneMask[slot] = BATTLE_ACTOR_MASK_ALL;
        for (laneIndex = 0; laneIndex < 0xA; laneIndex++) {
            g_BattleAiVm->var[slot][laneIndex] = firstValue;
        }
        collapsed = 1;
    }
    return collapsed;
}

/*
 * Initializes shared AI VM from command context with actor actorIndex, cursor zero and stack offset
 * 0x200, then loops byte opcodes split into high class/low operation until 0x73. Classes zero/one
 * fetch variable/address words; addresses at least 0x4000 read ten lanes and push mask 0x3FF.
 * Class three pops one/two operands, evaluates arithmetic on intersected lanes and pushes widest
 * type with vector mask when needed; class four compares intersected lanes into scalar boolean or
 * actor mask. Class five applies boolean AND/OR/NOT via any-lane helpers. Class six reads
 * one/two/three-byte immediates via fallthrough. Class seven handles zero-test jump, unequal-
 * pop/peek jump, absolute jump, stop, pop and optional party override. Class eight handles mask
 * intersection, random value/target, collapse or popcount, unsigned maximum/minimum lane masks
 * initialized zero/0xFFFFFF, attack MP cost and unchecked one-bit shift. Class nine writes scalar
 * or mask-selected variables, pops, posts action, queues text and skips text bytes through 0xFF
 * with two-byte parameter skips for 0xEA..0xF1, copies source-mask actors to first destination-
 * mask actor, accesses savemap byte or applies status mask. Class ten reads count/null-terminated
 * debug text and pops collapsed arguments into local frame without output, or pops two values.
 * Unsupported classes/operations continue after opcode consumption.
 */
// FUNCTION: FF7 0x005d7fe4
void __cdecl Battle_AiVm_Run(int actorIndex, unsigned char *bytecode, int partyOverride)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[436];
        int argIndex;
        struct { unsigned char pad_to_4[0x4]; unsigned char *textPtr; };
        struct { unsigned char pad_to_8[0x8]; int argValues[17]; };
        struct { unsigned char pad_to_4C[0x4C]; int argCount; };
        struct { unsigned char pad_to_150[0x150]; int srcLane; };
        struct { unsigned char pad_to_154[0x154]; int destLane; };
        struct { unsigned char pad_to_158[0x158]; int textByte; };
        struct { unsigned char pad_to_15C[0x15C]; int targetMask; };
        struct { unsigned char pad_to_160[0x160]; int writeLane; };
        struct { unsigned char pad_to_164[0x164]; int writeOffset; };
        struct { unsigned char pad_to_168[0x168]; int writeWidth; };
        struct { unsigned char pad_to_16C[0x16C]; int minValue; };
        struct { unsigned char pad_to_170[0x170]; int minLane; };
        struct { unsigned char pad_to_174[0x174]; int minLaneMask; };
        struct { unsigned char pad_to_178[0x178]; int maxLane; };
        struct { unsigned char pad_to_17C[0x17C]; int maxValue; };
        struct { unsigned char pad_to_180[0x180]; int maxLaneMask; };
        struct { unsigned char pad_to_184[0x184]; int bitCount; };
        struct { unsigned char pad_to_188[0x188]; int poppedValue; };
        struct { unsigned char pad_to_18C[0x18C]; int altJumpTarget; };
        struct { unsigned char pad_to_190[0x190]; int jumpTarget; };
        struct { unsigned char pad_to_194[0x194]; int sizeIndex; };
        struct { unsigned char pad_to_198[0x198]; int compareLane; };
        struct { unsigned char pad_to_19C[0x19C]; int compareMask; };
        struct { unsigned char pad_to_1A0[0x1A0]; int arithLane; };
        struct { unsigned char pad_to_1A4[0x1A4]; int widestType; };
        struct { unsigned char pad_to_1A8[0x1A8]; int readLane; };
        struct { unsigned char pad_to_1AC[0x1AC]; int varAddress; };
        struct { unsigned char pad_to_1B0[0x1B0]; int stopFlag; };
    } frame;
#pragma pack(pop)

    frame.stopFlag = 0;
    g_BattleAiVmBytecodePtr = bytecode;
    g_BattleAiVm = (BattleScriptVm *)g_pBattleCommandCtx;
    g_BattleAiVm->actorIndex = actorIndex;
    g_BattleAiVm->pc = 0;
    g_BattleAiVm->sp = 0x200;
    while (frame.stopFlag == 0) {
        g_BattleAiVm->opcode = g_BattleAiVmBytecodePtr[g_BattleAiVm->pc];
        (g_BattleAiVm->pc)++;
        g_BattleAiVm->opArg = g_BattleAiVm->opcode & 0xF;
        g_BattleAiVm->opKind = g_BattleAiVm->opcode >> 4;
        switch (g_BattleAiVm->opKind) {
        case BATTLE_AI_GROUP_PUSH_VAR:
            frame.varAddress = Battle_AiVm_FetchImm16();
            if (frame.varAddress >= 0x4000) {
                for (frame.readLane = 0; frame.readLane < BATTLE_ACTOR_COUNT; frame.readLane++) {
                    g_BattleAiVm->var[0][frame.readLane] = Battle_AiVm_ReadVar(frame.readLane, g_BattleAiVm->opcode, frame.varAddress);
                }
                g_BattleAiVm->laneMask[0] = BATTLE_ACTOR_MASK_ALL;
                Battle_AiVm_PushValue(g_BattleAiVm->opArg + 0x20);
            } else {
                g_BattleAiVm->var[0][0] = Battle_AiVm_ReadVar(g_BattleAiVm->actorIndex, g_BattleAiVm->opcode, frame.varAddress);
                Battle_AiVm_PushValue(g_BattleAiVm->opcode);
            }
            break;
        case BATTLE_AI_GROUP_PUSH_ADDRESS:
            g_BattleAiVm->var[0][0] = Battle_AiVm_FetchImm16();
            Battle_AiVm_PushValue(g_BattleAiVm->opcode);
            break;
        case BATTLE_AI_GROUP_ARITHMETIC:
            g_BattleAiVm->operandWidth[1] = 0;
            g_BattleAiVm->operandType[1] = BATTLE_AI_OPERAND_SCALAR;
            g_BattleAiVm->laneMask[1] = BATTLE_ACTOR_MASK_ALL;
            if (g_BattleAiVm->opArg != 7) {
                Battle_AiVm_PopValue(1);
            }
            Battle_AiVm_PopValue(0);
            for (frame.arithLane = 0; frame.arithLane < BATTLE_ACTOR_COUNT; frame.arithLane++) {
                if ((g_BattleAiVm->laneMask[0] & g_BattleAiVm->laneMask[1] & 1 << frame.arithLane) != 0) {
                    g_BattleAiVm->var[0][frame.arithLane] = Battle_AiVm_EvalArithmetic(frame.arithLane, frame.arithLane);
                }
            }
            frame.widestType = g_BattleAiVm->operandWidth[0] > g_BattleAiVm->operandWidth[1] ? g_BattleAiVm->operandWidth[0] : g_BattleAiVm->operandWidth[1];
            if (g_BattleAiVm->operandType[0] != BATTLE_AI_OPERAND_SCALAR || g_BattleAiVm->operandType[1] != BATTLE_AI_OPERAND_SCALAR) {
                frame.widestType |= 0x20;
                *(short *)&g_BattleAiVm->laneMask[0] &= *(short *)&g_BattleAiVm->laneMask[1];
            }
            Battle_AiVm_PushValue(frame.widestType);
            break;
        case BATTLE_AI_GROUP_COMPARE:
            frame.compareMask = 0;
            Battle_AiVm_PopValue(1);
            Battle_AiVm_PopValue(0);
            for (frame.compareLane = 0; frame.compareLane < BATTLE_ACTOR_COUNT; frame.compareLane++) {
                if ((g_BattleAiVm->laneMask[0] & g_BattleAiVm->laneMask[1] & 1 << frame.compareLane) != 0) {
                    if (Battle_AiVm_EvalCompare(frame.compareLane, frame.compareLane) != 0) {
                        frame.compareMask |= 1 << frame.compareLane;
                    }
                }
            }
            if (g_BattleAiVm->operandType[0] == BATTLE_AI_OPERAND_SCALAR && g_BattleAiVm->operandType[1] == BATTLE_AI_OPERAND_SCALAR) {
                g_BattleAiVm->var[0][0] = frame.compareMask != 0;
                Battle_AiVm_PushValue(0);
            } else {
                g_BattleAiVm->var[0][0] = frame.compareMask;
                Battle_AiVm_PushValue(2);
            }
            break;
        case BATTLE_AI_GROUP_LOGIC:
            switch (g_BattleAiVm->opArg) {
            case BATTLE_AI_LOGIC_AND:
                Battle_AiVm_PopValue(1);
                Battle_AiVm_PopValue(0);
                g_BattleAiVm->var[0][0] = Battle_AiVm_AnyLaneNonZero(1) & Battle_AiVm_AnyLaneNonZero(0);
                Battle_AiVm_PushValue(0);
                break;
            case BATTLE_AI_LOGIC_OR:
                Battle_AiVm_PopValue(1);
                Battle_AiVm_PopValue(0);
                g_BattleAiVm->var[0][0] = Battle_AiVm_AnyLaneNonZero(1) | Battle_AiVm_AnyLaneNonZero(0);
                Battle_AiVm_PushValue(0);
                break;
            case BATTLE_AI_LOGIC_NOT:
                Battle_AiVm_PopValue(0);
                g_BattleAiVm->var[0][0] = Battle_AiVm_AnyLaneNonZero(0) == 0;
                Battle_AiVm_PushValue(0);
            }
            break;
        case BATTLE_AI_GROUP_PUSH_CONST:
            frame.sizeIndex = 0;
            g_BattleAiVm->var[0][0] = 0;
            switch (g_BattleAiVm->opArg) {
            case BATTLE_AI_CONST_24BIT:
                g_BattleAiVm->var[0][0] |= *(g_BattleAiVmBytecodePtr + g_BattleAiVm->pc + 2) << 0x10;
                frame.sizeIndex++;
            case BATTLE_AI_CONST_16BIT:
                g_BattleAiVm->var[0][0] |= *(g_BattleAiVmBytecodePtr + g_BattleAiVm->pc + 1) << 8;
                frame.sizeIndex++;
            case BATTLE_AI_CONST_8BIT:
                g_BattleAiVm->var[0][0] |= g_BattleAiVmBytecodePtr[g_BattleAiVm->pc];
                g_BattleAiVm->pc = g_BattleAiVm->pc + frame.sizeIndex + 1;
                Battle_AiVm_PushValue(g_BattleAiVmAccessSizeTable[frame.sizeIndex]);
            }
            break;
        case BATTLE_AI_GROUP_FLOW:
            switch (g_BattleAiVm->opArg) {
            case BATTLE_AI_FLOW_JUMP_IF_ZERO:
                frame.jumpTarget = Battle_AiVm_FetchImm16();
                Battle_AiVm_PopValue(0);
                Battle_AiVm_CollapseToScalar(0);
                if (g_BattleAiVm->var[0][0] == 0) {
                    g_BattleAiVm->pc = frame.jumpTarget;
                }
                break;
            case BATTLE_AI_FLOW_JUMP_IF_NOT_EQUAL:
                frame.altJumpTarget = Battle_AiVm_FetchImm16();
                Battle_AiVm_PopValue(1);
                Battle_AiVm_CollapseToScalar(1);
                Battle_AiVm_PeekValue(0);
                Battle_AiVm_CollapseToScalar(0);
                if (g_BattleAiVm->var[0][0] != g_BattleAiVm->var[1][0]) {
                    g_BattleAiVm->pc = frame.altJumpTarget;
                }
                break;
            case BATTLE_AI_FLOW_JUMP:
                g_BattleAiVm->pc = Battle_AiVm_FetchImm16();
                break;
            case BATTLE_AI_FLOW_END:
                frame.stopFlag = 1;
                break;
            case BATTLE_AI_FLOW_POP:
                Battle_AiVm_PopValue(0);
                break;
            case BATTLE_AI_FLOW_SET_PARTY_OVERRIDE:
                Battle_AiVm_PopValue(0);
                if (partyOverride >= 0) {
                    Battle_SetPartyAiEntityOverride(partyOverride, g_BattleAiVm->var[0][0]);
                }
            }
            break;
        case BATTLE_AI_GROUP_BUILTIN:
            switch (g_BattleAiVm->opArg) {
            case BATTLE_AI_BUILTIN_MASK_LANES:
                Battle_AiVm_PeekValue(0);
                if (g_BattleAiVm->operandType[0] != BATTLE_AI_OPERAND_ADDRESS) {
                    frame.poppedValue = Battle_AiVm_PopValue(0);
                    Battle_AiVm_PopValue(1);
                    *(short *)&g_BattleAiVm->laneMask[0] &= *(short *)&g_BattleAiVm->var[1][0];
                    Battle_AiVm_PushValue(frame.poppedValue);
                }
                break;
            case BATTLE_AI_BUILTIN_RANDOM_TARGET:
                Battle_AiVm_PopValue(0);
                g_BattleAiVm->var[0][0] = Battle_PickRandomTargetFromMask(*(short *)&g_BattleAiVm->var[0][0]);
                Battle_AiVm_PushValue(2);
                break;
            case BATTLE_AI_BUILTIN_RANDOM:
                g_BattleAiVm->var[0][0] = Battle_AiVm_Random16();
                Battle_AiVm_PushValue(2);
                break;
            case BATTLE_AI_BUILTIN_COUNT:
                frame.bitCount = 1;
                Battle_AiVm_PopValue(0);
                if (Battle_AiVm_CollapseToScalar(0) != 0) {
                    frame.bitCount = g_BattleAiVm->operandWidth[0];
                } else {
                    g_BattleAiVm->var[0][0] = Battle_PopCount16(g_BattleAiVm->var[0][0] & BATTLE_ACTOR_MASK_ALL);
                }
                Battle_AiVm_PushValue(frame.bitCount);
                break;
            case BATTLE_AI_BUILTIN_MAX_MASK:
                frame.maxValue = 0;
                *(short *)&frame.maxLaneMask = 0;
                Battle_AiVm_PopValue(0);
                for (frame.maxLane = 0; frame.maxLane < BATTLE_ACTOR_COUNT; frame.maxLane++) {
                    if ((g_BattleAiVm->laneMask[0] & 1 << frame.maxLane) != 0) {
                        if (g_BattleAiVm->var[0][frame.maxLane] == frame.maxValue) {
                            *(short *)&frame.maxLaneMask |= (short)(1 << frame.maxLane);
                        } else if ((unsigned int)g_BattleAiVm->var[0][frame.maxLane] > (unsigned int)frame.maxValue) {
                            frame.maxValue = g_BattleAiVm->var[0][frame.maxLane];
                            *(short *)&frame.maxLaneMask = 1 << frame.maxLane;
                        }
                    }
                }
                g_BattleAiVm->var[0][0] = frame.maxLaneMask & 0xFFFF;
                Battle_AiVm_PushValue(2);
                break;
            case BATTLE_AI_BUILTIN_MIN_MASK:
                frame.minValue = 0xFFFFFF;
                *(short *)&frame.minLaneMask = 0;
                Battle_AiVm_PopValue(0);
                for (frame.minLane = 0; frame.minLane < BATTLE_ACTOR_COUNT; frame.minLane++) {
                    if ((g_BattleAiVm->laneMask[0] & 1 << frame.minLane) != 0) {
                        if (g_BattleAiVm->var[0][frame.minLane] == frame.minValue) {
                            *(short *)&frame.minLaneMask |= (short)(1 << frame.minLane);
                        } else if ((unsigned int)g_BattleAiVm->var[0][frame.minLane] < (unsigned int)frame.minValue) {
                            frame.minValue = g_BattleAiVm->var[0][frame.minLane];
                            *(short *)&frame.minLaneMask = 1 << frame.minLane;
                        }
                    }
                }
                g_BattleAiVm->var[0][0] = frame.minLaneMask & 0xFFFF;
                Battle_AiVm_PushValue(2);
                break;
            case BATTLE_AI_BUILTIN_ATTACK_MP_COST:
                Battle_AiVm_PopValue(0);
                g_BattleAiVm->var[0][0] = Battle_AiVm_GetAttackMpCost(g_BattleAiVm->var[0][0]);
                Battle_AiVm_PushValue(2);
                break;
            case BATTLE_AI_BUILTIN_BIT:
                Battle_AiVm_PopValue(0);
                g_BattleAiVm->var[0][0] = 1 << g_BattleAiVm->var[0][0];
                Battle_AiVm_PushValue(2);
            }
            break;
        case BATTLE_AI_GROUP_COMMAND:
            switch (g_BattleAiVm->opArg) {
            case BATTLE_AI_COMMAND_ASSIGN:
                Battle_AiVm_PopValue(0);
                Battle_AiVm_CollapseToScalar(0);
                Battle_AiVm_PopValue(1);
                frame.writeWidth = g_BattleAiVm->operandWidth[1];
                if ((unsigned int)g_BattleAiVm->var[1][0] >= (unsigned int)0x4000) {
                    frame.writeOffset = g_BattleAiVm->var[1][0];
                    Battle_AiVm_PopValue(1);
                    frame.targetMask = Battle_AiVm_ReadVar(g_BattleAiVm->actorIndex, g_BattleAiVm->operandWidth[1], g_BattleAiVm->var[1][0]);
                    for (frame.writeLane = 0; frame.writeLane < BATTLE_ACTOR_COUNT; frame.writeLane++) {
                        if ((1 << frame.writeLane & frame.targetMask) != 0) {
                            Battle_AiVm_WriteVar(frame.writeLane, frame.writeWidth, frame.writeOffset, g_BattleAiVm->var[0][0]);
                        }
                    }
                } else {
                    Battle_AiVm_WriteVar(g_BattleAiVm->actorIndex, frame.writeWidth, g_BattleAiVm->var[1][0], g_BattleAiVm->var[0][0]);
                }
                break;
            case BATTLE_AI_COMMAND_POP:
                Battle_AiVm_PopValue(0);
                break;
            case BATTLE_AI_COMMAND_POST_ACTION:
                Battle_AiVm_PopValue(1);
                Battle_AiVm_PopValue(0);
                Battle_AiVm_PostAction(Battle_HighestSetBitIndex(gBattleAiGlobals.actorBit), *(short *)&g_BattleAiVm->var[0][0], *(short *)&g_BattleAiVm->var[1][0]);
                break;
            case BATTLE_AI_COMMAND_SHOW_MESSAGE:
                Battle_AiVm_PostAction(*(short *)&g_BattleAiVm->actorIndex, 0x21, Battle_QueueAiMessageText((char *)((int)(g_BattleAiVmBytecodePtr + g_BattleAiVm->pc)), gBattleAiGlobals.messageArgs) + 0x100);
                while (((unsigned char)(*(char *)&frame.textByte = g_BattleAiVmBytecodePtr[(g_BattleAiVm->pc)++])) != TEXT_END) {
                    if ((frame.textByte & 0xFF) >= 0xEA && (frame.textByte & 0xFF) < 0xF2) {
                        g_BattleAiVm->pc += 2;
                    }
                }
                break;
            case BATTLE_AI_COMMAND_COPY_COMBATANT:
                Battle_AiVm_PopValue(1);
                Battle_AiVm_PopValue(0);
                for (frame.srcLane = 0; frame.srcLane < BATTLE_ACTOR_COUNT; frame.srcLane++) {
                    if ((g_BattleAiVm->var[1][0] & 1 << frame.srcLane) != 0) {
                        for (frame.destLane = 0; frame.destLane < BATTLE_ACTOR_COUNT; frame.destLane++) {
                            if ((g_BattleAiVm->var[0][0] & 1 << frame.destLane) != 0) {
                                Battle_AiVm_CopyCombatantState(frame.destLane, frame.srcLane);
                            }
                        }
                        break;
                    }
                }
                break;
            case BATTLE_AI_COMMAND_SAVEMAP_BYTE:
                Battle_AiVm_PopValue(1);
                Battle_AiVm_PopValue(0);
                Battle_AiVm_AccessSavemapByte(g_BattleAiVm->var[0][0], g_BattleAiVm->var[1][0]);
                break;
            case BATTLE_AI_COMMAND_APPLY_STATUS:
                Battle_AiVm_PopValue(1);
                Battle_AiVm_PopValue(0);
                Battle_ApplyStatusToActorMask(g_BattleAiVm->var[0][0], g_BattleAiVm->var[1][0], 0);
            }
            break;
        case BATTLE_AI_GROUP_DEBUG:
            switch (g_BattleAiVm->opArg) {
            case BATTLE_AI_DEBUG_PRINT:
                frame.argCount = g_BattleAiVmBytecodePtr[g_BattleAiVm->pc];
                (g_BattleAiVm->pc)++;
                frame.textPtr = g_BattleAiVmBytecodePtr + g_BattleAiVm->pc;
                while (g_BattleAiVmBytecodePtr[(g_BattleAiVm->pc)++] != 0) {
                }
                for (frame.argIndex = 0; frame.argIndex < frame.argCount; frame.argIndex++) {
                    Battle_AiVm_PopValue(0);
                    Battle_AiVm_CollapseToScalar(0);
                    frame.argValues[frame.argIndex] = g_BattleAiVm->var[0][0];
                }
                break;
            case BATTLE_AI_DEBUG_POP_TWO:
                Battle_AiVm_PopValue(1);
                Battle_AiVm_PopValue(0);
            }
        }
    }
}

/*
 * Reads two script bytes little-endian through global VM program counter, advancing it after each,
 * and returns unsigned16 value despite intermediate short casts.
 */
// FUNCTION: FF7 0x005d8e63
unsigned short __cdecl Battle_AiVm_FetchImm16(void)
{
    unsigned short val;

    val = g_BattleAiVmBytecodePtr[g_BattleAiVm->pc];
    (g_BattleAiVm->pc)++;
    val |= (short)(g_BattleAiVmBytecodePtr[g_BattleAiVm->pc] << 8);
    (g_BattleAiVm->pc)++;
    return (short)val;
}

/*
 * Pushes scalar/raw or selected ten lanes in descending order plus mask by high type nibble, then
 * always pushes narrowed type metadata byte. Unsupported type still pushes metadata.
 */
// FUNCTION: FF7 0x005d8ecf
void __cdecl Battle_AiVm_PushValue(int descriptor)
{
    int width;
    int type;
    int lane;

    type = descriptor >> 4;
    width = descriptor & 0xF;
    switch (type) {
    case BATTLE_AI_OPERAND_SCALAR:
        Battle_AiVm_PushRaw(width, g_BattleAiVm->var[0][0]);
        break;
    case BATTLE_AI_OPERAND_ADDRESS:
        Battle_AiVm_PushRaw(BATTLE_AI_WIDTH_WORD, g_BattleAiVm->var[0][0]);
        break;
    case BATTLE_AI_OPERAND_LANES:
        for (lane = BATTLE_ACTOR_COUNT; lane > 0; lane--) {
            if ((1 << lane - 1 & g_BattleAiVm->laneMask[0]) != 0) {
                Battle_AiVm_PushRaw(width, *(int *)((char *)g_BattleAiVm + lane * 4 + 0x28));
            }
        }
        Battle_AiVm_PushRaw(BATTLE_AI_WIDTH_WORD, g_BattleAiVm->laneMask[0]);
    }
    (g_BattleAiVm->sp)--;
    g_BattleAiVm->stack[g_BattleAiVm->sp] = descriptor;
}

/*
 * Pushes low byte then successive higher bytes for sizes three/two through fallthrough, with size
 * zero/one pushing one byte. Decrements stack pointer per write; other sizes no write.
 */
// FUNCTION: FF7 0x005d8fc7
void __cdecl Battle_AiVm_PushRaw(int size, unsigned int value)
{
    /* `value & 0xFF`, not `(unsigned char)value`: the original narrows with a
       dword `and edx, 0xff` and evaluates it BEFORE decrementing sp. A cast
       loads the byte directly (`mov dl, [ebp+0xc]`) and reverses that order. */
    switch (size) {
    case BATTLE_AI_WIDTH_DWORD:
        g_BattleAiVm->stack[--g_BattleAiVm->sp] = value & 0xFF;
        value >>= 8;
    case BATTLE_AI_WIDTH_WORD:
        g_BattleAiVm->stack[--g_BattleAiVm->sp] = value & 0xFF;
        value >>= 8;
    case BATTLE_AI_WIDTH_BYTE:
    case BATTLE_AI_WIDTH_BIT:
        g_BattleAiVm->stack[--g_BattleAiVm->sp] = value & 0xFF;
    }
}

/*
 * Consumes descriptor byte from AI stream and stores high-nibble type/low-nibble width. A scalar
 * is broadcast across ten actors with mask 0x3ff; an address reads a two-byte raw value into the
 * first variable dword; a lane operand reads its mask then values only for selected actors. Other
 * types consume descriptor only; returns byte.
 */
// FUNCTION: FF7 0x005d9099
unsigned int __cdecl Battle_AiVm_PopValue(int operand)
{
    int value;
    int width;
    int descByte;
    int type;
    int lane;

    descByte = g_BattleAiVm->stack[g_BattleAiVm->sp];
    (g_BattleAiVm->sp)++;
    type = descByte >> 4;
    width = descByte & 0xF;
    g_BattleAiVm->operandType[operand] = type;
    g_BattleAiVm->operandWidth[operand] = width;
    switch (type) {
    case BATTLE_AI_OPERAND_SCALAR:
        g_BattleAiVm->laneMask[operand] = BATTLE_ACTOR_MASK_ALL;
        value = Battle_AiVm_PopRaw(width);
        for (lane = 0; lane < BATTLE_ACTOR_COUNT; lane++) {
            g_BattleAiVm->var[operand][lane] = value;
        }
        break;
    case BATTLE_AI_OPERAND_ADDRESS:
        g_BattleAiVm->var[operand][0] = Battle_AiVm_PopRaw(BATTLE_AI_WIDTH_WORD);
        break;
    case BATTLE_AI_OPERAND_LANES:
        g_BattleAiVm->laneMask[operand] = Battle_AiVm_PopRaw(BATTLE_AI_WIDTH_WORD);
        for (lane = 0; lane < BATTLE_ACTOR_COUNT; lane++) {
            if ((1 << lane & g_BattleAiVm->laneMask[operand]) != 0) {
                g_BattleAiVm->var[operand][lane] = Battle_AiVm_PopRaw(width);
            }
        }
    }
    return descByte;
}

/*
 * Pops one byte for size zero/one, two for two and three for three, shifting accumulated value
 * eight before each later byte. Advances stack pointer for every read; other sizes return zero
 * unchanged. No bounds guard.
 */
// FUNCTION: FF7 0x005d9208
unsigned int __cdecl Battle_AiVm_PopRaw(int size)
{
    unsigned int value;

    value = 0;
    switch (size) {
    case BATTLE_AI_WIDTH_DWORD:
        value |= g_BattleAiVm->stack[g_BattleAiVm->sp++];
    case BATTLE_AI_WIDTH_WORD:
        value <<= 8;
        value |= g_BattleAiVm->stack[g_BattleAiVm->sp++];
    case BATTLE_AI_WIDTH_BYTE:
    case BATTLE_AI_WIDTH_BIT:
        value <<= 8;
        value |= g_BattleAiVm->stack[g_BattleAiVm->sp++];
    }
    return value;
}

/*
 * Battle_AiVm_PopValue with the stack pointer rewound, so the operand is not consumed.
 */
// FUNCTION: FF7 0x005d92eb
unsigned int __cdecl Battle_AiVm_PeekValue(int slot)
{
    int savedSp;
    unsigned int result;

    savedSp = g_BattleAiVm->sp;
    result = Battle_AiVm_PopValue(slot);
    g_BattleAiVm->sp = savedSp;
    return result;
}

/*
 * Evaluates unsigned add/subtract/multiply/divide/remainder/AND/OR/complement by VM opcode
 * 0x30..37, defaults zero.
 */
// FUNCTION: FF7 0x005d931d
unsigned int __cdecl Battle_AiVm_EvalArithmetic(int leftLane, int rightLane)
{
    unsigned int result;
    unsigned int rightValue;
    unsigned int left;

    result = 0;
    left = g_BattleAiVm->var[0][leftLane];
    rightValue = g_BattleAiVm->var[1][rightLane];
    switch (g_BattleAiVm->opcode) {
    case BATTLE_AI_OP_ADD:
        result = left + rightValue;
        break;
    case BATTLE_AI_OP_SUB:
        result = left - rightValue;
        break;
    case BATTLE_AI_OP_MUL:
        result = left * rightValue;
        break;
    case BATTLE_AI_OP_DIV:
        result = left / rightValue;
        break;
    case BATTLE_AI_OP_MOD:
        result = left % rightValue;
        break;
    case BATTLE_AI_OP_BIT_AND:
        result = left & rightValue;
        break;
    case BATTLE_AI_OP_BIT_OR:
        result = left | rightValue;
        break;
    case BATTLE_AI_OP_BIT_NOT:
        result = ~left;
    }
    return result;
}

/*
 * Evaluates equality/inequality and unsigned >=, <=, >, < by VM opcodes 0x40..45, default false.
 */
// FUNCTION: FF7 0x005d93ef
int __cdecl Battle_AiVm_EvalCompare(int leftLane, int rightLane)
{
    int result;
    int rightValue;
    int left;

    result = 0;
    left = g_BattleAiVm->var[0][leftLane];
    rightValue = g_BattleAiVm->var[1][rightLane];
    switch (g_BattleAiVm->opcode) {
    case BATTLE_AI_OP_CMP_EQ:
        if (left == rightValue) {
            result = 1;
        }
        break;
    case BATTLE_AI_OP_CMP_NE:
        if (left != rightValue) {
            result = 1;
        }
        break;
    case BATTLE_AI_OP_CMP_GE:
        if ((unsigned int)left >= (unsigned int)rightValue) {
            result = 1;
        }
        break;
    case BATTLE_AI_OP_CMP_LE:
        if ((unsigned int)left <= (unsigned int)rightValue) {
            result = 1;
        }
        break;
    case BATTLE_AI_OP_CMP_GT:
        if ((unsigned int)left > (unsigned int)rightValue) {
            result = 1;
        }
        break;
    case BATTLE_AI_OP_CMP_LT:
        if ((unsigned int)left < (unsigned int)rightValue) {
            result = 1;
        }
    }
    return result;
}

/*
 * Tests ten selected lanes of given AI VM slot and returns whether any selected lane variable is
 * nonzero. Builds bit mask internally but returns only boolean.
 */
// FUNCTION: FF7 0x005d94c3
int __cdecl Battle_AiVm_AnyLaneNonZero(int slot)
{
    unsigned short result;
    int i;

    result = 0;
    for (i = 0; i < BATTLE_ACTOR_COUNT; i++) {
        if ((g_BattleAiVm->laneMask[slot] & (1 << i)) != 0 &&
            g_BattleAiVm->var[slot][i] != 0) {
            result |= (unsigned short)(1 << i);
        }
    }
    return result != 0;
}
} /* extern "C" */

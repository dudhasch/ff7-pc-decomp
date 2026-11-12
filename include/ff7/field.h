/* Field script virtual machine state.
 *
 * EVERY ADDRESS AND WIDTH IN THIS FILE WAS READ OUT OF ff7.exe. The NAMES
 * follow the PlayStation decompilation of FF7 (see Credits in the README).
 *
 * WHY THE NAMES ARE TRUSTWORTHY HERE
 * ----------------------------------
 * The field script bytecode is the same on both machines -- it is the data in
 * `flevel`, not code -- so both executables carry a 256-entry table of opcode
 * handlers indexed by the same byte. The PC's is at 0x009055A0 and holds 238
 * distinct targets; the PSX's is `g_FieldOpcodes[256]`.
 *
 * The correspondence comes from two handlers whose bodies are the same
 * program, not from the names:
 *
 *     PSX OpcodeFuncInc          PC 0x00619CDF Script_OpIncByte
 *     FieldEventReadMemoryU8     call 0x0060F750  (2, 2)
 *     ... + 1                    add eax, 1
 *     FieldEventWriteMemoryU8    call 0x0060FA7D  (2, 2, ...)
 *     PC_INC(3)                  g_FieldScriptPC[g_CurrentEntity] += 3
 *     return 0                   xor eax, eax
 *
 * Every operand accessor, the entity index and the per-entity program counter
 * fall out of that one pairing, and each is asserted below against the
 * instruction that proves its address and width.
 *
 * THE TAIL EVERY HANDLER SHARES
 * -----------------------------
 * `g_FieldScriptPC[g_CurrentEntity] += N;` compiles at /Od to
 *
 *     xor  eax, eax
 *     mov  al, byte ptr [g_CurrentEntity]
 *     mov  cx, word ptr [eax*2 + g_FieldScriptPC]
 *     add  cx, N
 *     xor  edx, edx
 *     mov  dl, byte ptr [g_CurrentEntity]        <- the index is RELOADED
 *     mov  word ptr [edx*2 + g_FieldScriptPC], cx
 *
 * The reload is what `+=` does at /Od; a temporary holding the index would not
 * produce it. Which registers get used varies with what the body did before,
 * so match on the shape, not on eax/ecx/edx.
 */

#ifndef FF7_FIELD_H
#define FF7_FIELD_H

#include "ff7/window.h"

/* ------------------------------------------------------------------------
 * The virtual machine's state.
 * ---------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* 0x006107F0  mov al, byte ptr [0xcc0964]   -- index of the entity whose
 * script is running. One byte: every read of it is `mov al`/`mov cl`/`mov dl`
 * off a zeroed register. */
extern unsigned char g_CurrentEntity;

/* 0x006107F5  mov cx, word ptr [eax*2 + 0xcc0cf8]   -- per-entity program
 * counter, an offset into g_FieldScripts rather than a pointer. Two bytes,
 * scaled by 2 from a byte index, so at most 256 entries. */
extern unsigned short g_FieldScriptPC[52];

/* 0x00610830  mov cx, word ptr [eax*2 + 0xcc0900]   -- per-entity wait state,
 * indexed the same way. 0x00610818 switches on it. */
extern unsigned short g_FieldScriptWait[48];

/* 0x0060F78B  mov edx, dword ptr [0xcbf5e8]   -- base of the loaded script
 * block; the operand accessors index it by g_FieldScriptPC[g_CurrentEntity].
 * 0x0075EE50 is what assigns it. */
struct FieldScriptHeader;
extern struct FieldScriptHeader *g_FieldScripts;

#ifdef __cplusplus
}
#endif

/* ------------------------------------------------------------------------
 * FieldState -- the block *g_pFieldState points at. At least 0xF3 bytes.
 *
 * Scanning every access made through 0x00CBF9D8 yields 71 distinct offsets.
 * Only the ones a function here actually reads are named and the
 * rest are padding, exactly as include/ff7/battle.h does it -- a `pad_*`
 * member is not a claim, it is unexamined bytes.
 *
 * The names come from the opcode handlers that write them: 0x0061F1EA is
 * Script_OpSetMenuLocked and it
 * writes +0x34, 0x0061A4D4 is Script_OpSetGatewaysDisabled and writes +0x36,
 * and so on down the run of one-byte script flags at 0x34..0x3D. There is no
 * PSX counterpart for this structure; the PC port keeps its field state in a
 * single allocated block where the PSX build used separate globals.
 * ---------------------------------------------------------------------- */
/* FieldShakeData -- 0xE bytes, twice over inside FieldState at +0x8A and
 * +0x98. Placed by arithmetic rather than by an access of its own: 0xE is the
 * PSX size, and putting two of them after currentMovieFrame at +0x88 is what
 * lands layer3Depth on +0xAE and layer2Depth on +0xB0, which BGPDH writes.
 * The member names are the PlayStation version's; nothing here tests them. */
typedef struct FieldShakeData {
    /* +0x00 */ unsigned char enabled;
    /* +0x01 */ unsigned char segmentActive;
    /* +0x02 */ unsigned char rngId;
    /* +0x03 */ char currentOffset;
    /* +0x04 */ short amplitude;
    /* +0x06 */ short start;
    /* +0x08 */ short target;
    /* +0x0A */ short numStepsPerSegment;
    /* +0x0C */ short currentStep;
} FieldShakeData;

typedef struct FieldState {
    /* +0x00 */ char windowPhase;
    /* +0x01 */ char requestCode; /* 0x006142BE writes it and 0x00613F7B
                                    reads it with `movsx`, so it is
                                    signed. PSX eventCmd            */
    /* +0x02 */ short eventCmdParam; /* 0x0061A305; PSX same name -- the
                                       extra word a request carries, here
                                       the movie id                       */
    /* The PSX has pcPosX/pcPosY/pcPosZ at +0x04, +0x06 and +0x08. The script
     * VM writes +0x04 and +0x06 from two operand words (field_script_read_
     * operand8.c); nothing reads +0x08, so it is padding.                 */
    /* +0x04 */ short pcTileX;
    /* +0x06 */ short pcTileY;
    /* +0x08 */ unsigned char pad_08[0x2];
    /* +0x0A */ short cameraScrollTargetX; /* 0x0061A896; PSX same name      */
    /* +0x0C */ short cameraScrollTargetY; /* 0x0061A8AC; PSX same name      */
    /* +0x0E */ char pad_0e[0x02];
    /* +0x10 */ short fieldScale; /* 0x006181E4, movsx then used
                                     as the numerator of a /512;
                                     PSX currentFieldScale, same
                                     offset                     */
    /* The view-offset block, and the two values saved when the field is
     * left. Named from the PSX at delta zero, which the four members below
     * them establish by direct evidence; nothing in this image has read any
     * of these seven yet.                                                 */
    /* +0x12 */ unsigned char viewOffsetNumSteps;
    /* +0x13 */ unsigned char viewOffsetCurrentStep;
    /* +0x14 */ char viewOffsetMode;
    /* +0x15 */ unsigned char pad_15;
    /* +0x16 */ short viewOffset;
    /* +0x18 */ short viewOffsetStart;
    /* +0x1A */ short viewOffsetTarget;
    /* +0x1C */ unsigned char pad_1c;
    /* +0x1D */ char cameraScrollMode; /* 0x0061A880 writes it and
                                         0x0061A916 switches on it with
                                         `movsx`, so it is signed. PSX
                                         same name                  */
    /* +0x1E */ char cameraScrollTargetId; /* 0x0061A5A9            */
    /* +0x1F */ unsigned char cameraScrollState; /* 0x0061A8CC; PSX same     */
    /* +0x20 */ short cameraScrollNumSteps; /* 0x0061A8C2; PSX same */
    /* +0x22 */ unsigned short pcWalkMeshId;
    /* +0x24 */ union {
        short entryId;
        struct {
            unsigned char pcDirection;
            unsigned char pad_25;
        };
    };
    /* +0x26 */ short requestArg; /* 0x006142C8, cleared with
                                   requestCode. The PSX calls it
                                   movieCommandState; here the movie,
                                   battle and disc-change handshakes
                                   all step through it, so the wider
                                   name is the one that fits       */
    /* +0x28 */ short modelCount;
    /* +0x2A */ short playerModel; /* 0x006142FF stores the model index of
                                     the entity Script_OpSetPlayerEntity
                                     names, as a WORD; 0x0061BCC9 does the
                                     same. Script_OpScrollToEntity reads
                                     only the low byte, which is why that
                                     one has a cast                       */
    /* +0x2C */ union { unsigned short idleAnimId; char idleAnimIdLo; }; /* 0x00614578; PSX same name  */
    /* +0x2E */ union { unsigned short walkAnimId; char walkAnimIdLo; }; /* 0x006145A1; PSX same name  */
    /* +0x30 */ union { unsigned short runAnimId; char runAnimIdLo; }; /* 0x006145CA; PSX same name   */
    /* +0x32 */ unsigned char characterLock; /* 0x00614169, then copied
                                              straight out to
                                              g_FieldDialogueLock; PSX
                                              same name            */
    /* +0x33 */ char entityMode;
    /* +0x34 */ unsigned char menuLocked; /* 0x0061F20D                       */
    /* +0x35 */ char unusedFlag35;
    /* +0x36 */ unsigned char gatewaysDisabled; /* 0x0061A4F7                 */
    /* +0x37 */ unsigned char unusedFlag37; /* 0x00614094                     */
    /* +0x38 */ unsigned char fieldHidden; /* 0x0061423D                      */
    /* +0x39 */ char movieCameraDisabled; /* 0x0061428B              */
    /* +0x3A */ char speedBoost; /* 0x00614046                       */
    /* +0x3B */ unsigned char encountersDisabled; /* 0x006141EF               */
    /* +0x3C */ unsigned char encounterTable; /* 0x0062008C                   */
    /* +0x3D */ unsigned char battleMode1; /* 0x0061FF0D; PSX same name       */
    /* +0x3E */ unsigned short battleMode2; /* 0x0061FEEE; PSX same name, and
                                               the same u8-then-u16 pair     */
    /* +0x40 */ short unusedWord40;
    /* +0x42 */ unsigned char pad_42[0x2];
    /* +0x44 */ int nextBattleMusic; /* 0x00613DE4; PSX same name. The music
                                       opcode resolves a track number out of
                                       the AKAO block and parks it here     */
    /* +0x48 */ int nextFieldMusic; /* 0x00613E85; PSX same name            */
    /* +0x4C */ short fadeType; /* 0x0061E0AD writes it and 0x0061E163
                                  switches on it with `movsx`, so it is
                                  signed. PSX same name              */
    /* +0x4E */ union { short fadeAdjust; char fadeAdjustLo; }; /* 0x0061E118, cleared; PSX same name     */
    /* +0x50 */ short fadeSpeed; /* 0x0061E12A; PSX same name              */
    /* +0x52 */ union { short red; char redLo; };
    /* +0x54 */ union { short green; char greenLo; };
    /* +0x56 */ union { short blue; char blueLo; };
    /* +0x58 */ short fadeRedFrom;
    /* +0x5A */ short fadeGreenFrom;
    /* +0x5C */ short fadeBlueFrom;
    /* +0x5E */ short fadeRedTarget; /* 0x0061E0C7; PSX nFadeRedTarget      */
    /* +0x60 */ short fadeGreenTarget; /* 0x0061E0E0; PSX nFadeGreenTarget  */
    /* +0x62 */ short fadeBlueTarget; /* 0x0061E0F9; PSX nFadeBlueTarget    */
    /* +0x64 */ short mapId; /* 0x0061E4F0; PSX prevFieldId               */
    /* +0x66 */ unsigned char pad_66;
    /* +0x67 */ unsigned char pad_67;
    /* The eight button masks. The PSX declares each as a u32 and this image
     * reads them 16 bits wide -- 0x0061223A is `mov cx, word ptr [eax+0x78]`
     * -- so the declaration follows the access and the upper half is padding
     * here. Same offsets either way; the names are the PSX's.            */
    /* +0x68 */ union { int activeKeys32; struct { short activeKeys; unsigned char pad_6a[2]; }; }; /* 0x0061224E */
    /* +0x6C */ union { int oldActiveKeys32; struct { short oldActiveKeys; unsigned char pad_6e[2]; }; };
    /* +0x70 */ union { int newActiveKeys32; struct { short newActiveKeys; unsigned char pad_72[2]; }; }; /* 0x0061227C */
    /* +0x74 */ union { int newInactiveKeys32; struct { short newInactiveKeys; unsigned char pad_76[2]; }; }; /* 0x006122F4 */
    /* +0x78 */ union { int activeKeys2_32; struct { short activeKeys2; unsigned char pad_7a[2]; }; }; /* 0x0061223A */
    /* +0x7C */ union { int oldActiveKeys2_32; struct { short oldActiveKeys2; unsigned char pad_7e[2]; }; };
    /* +0x80 */ union { int newActiveKeys2_32; struct { short newActiveKeys2; unsigned char pad_82[2]; }; }; /* 0x00612269 */
    /* +0x84 */ union { int newInactiveKeys2_32; struct { short newInactiveKeys2; unsigned char pad_86[2]; }; }; /* 0x006122DD */
    /* +0x88 */ short currentMovieFrame; /* 0x0061A4A2, reported when no
                                            movie is playing              */
    /* +0x8A */ FieldShakeData shakeX;
    /* +0x98 */ FieldShakeData shakeY;
    /* +0xA6 */ short layer2ScrollXSpeed;
    /* +0xA8 */ short layer2ScrollYSpeed;
    /* +0xAA */ short layer3ScrollXSpeed;
    /* +0xAC */ short layer3ScrollYSpeed;
    /* +0xAE */ unsigned short layer3Depth; /* 0x0061EDC9, BGPDH layer 3  */
    /* +0xB0 */ unsigned short layer2Depth; /* 0x0061EDAE, BGPDH layer 2  */
    /* +0xB2 */ unsigned char blockedAccesses[64];
    /* +0xF2 */ union {
        unsigned char backgroundLayerVisibility[64];
        /* Script_OpMinigame keeps the minigame to start in the first byte (enum FieldMinigame) */
        unsigned char minigameKind;
    };
    /* +0x132 */ unsigned short pad_132;
} FieldState;



/* ------------------------------------------------------------------------
 * FieldEntity -- 0x88 bytes, in the array *g_pFieldEntities points at.
 *
 * THE PLAYSTATION RECORD WITH THREE FIELDS WIDENED.
 * -------------------------------------------------
 * The head matches the PlayStation version's entity record at delta zero.
 * The tail does not: it is shifted. What settles it is 0x0060C13C in
 * Script_ResetState, which
 * clears the three offset axes in the PSX's own order:
 *
 *     mov dword ptr [edx+ecx+0x40], 0      OffsetX      PSX 0x40  s16 -> s32
 *     mov dword ptr [ecx+eax+0x48], 0      OffsetY      PSX 0x46  s16 -> s32
 *     mov dword ptr [eax+edx+0x50], 0      OffsetZ      PSX 0x4C  s16 -> s32
 *     mov word  ptr [edx+ecx+0x44], 0      OffsetStartX PSX 0x42
 *     mov word  ptr [ecx+eax+0x4c], 0      OffsetStartY PSX 0x48
 *     mov word  ptr [eax+edx+0x54], 0      OffsetStartZ PSX 0x4E
 *     mov word  ptr [edx+ecx+0x46], 0      OffsetEndX   PSX 0x44
 *     mov word  ptr [ecx+eax+0x4e], 0      OffsetEndY   PSX 0x4A
 *     mov word  ptr [eax+edx+0x56], 0      OffsetEndZ   PSX 0x50
 *
 * Three axes, two extra bytes each. So the delta is
 *
 *     +0x00 .. +0x3F   0    the head, unchanged
 *     +0x40 .. +0x7A  +6    everything after the widened offsets
 *     +0x7C .. +0x87  +4    the PSX's Pad76 is gone; on x86 the u16 pair at
 *                           +0x78/+0x7A already leaves +0x7C 4-aligned
 *
 * and 0x84 + 6 - 2 = 0x88 exactly. Every one of the 50 offsets the stride-0x88
 * scan reports agrees with that map, in width as well as position, and the
 * size closes on the nose. A constant delta could never have done both.
 *
 * Four opcodes confirm it semantically, each against its named PSX twin:
 *
 *     0x00614353  CHAR    writes +0x6C, +0x62, +0x5D; OpcodeFuncChar writes
 *                         charId (0x66), visible (0x5C), entityId (0x57)
 *     0x00615271  ANIMB   +0x6A = +0x68 >> 4; OpcodeFuncAnimb does
 *                         animLastFrame = animCurrentFrame >> 4
 *     0x00618A01  VISI    writes +0x62; OpcodeFuncVisi writes visible
 *     0x00618A80  TLKON   writes +0x61; OpcodeFuncTlkon writes TalkOff
 *
 * Note: +0x66 is `animSpeed` (set by opcode DFANM, six bytes below `charId`),
 * and +0x72..+0x76 (`solidRange`, `talkRange`, `moveSpeed`) are initialised as
 * `currentFieldScale * k`.
 *
 * Positions below are derived from that map, so the whole record is named.
 * A trailing address is an instruction in this image that touches the member;
 * a member without one is placed by the map and by nothing else.
 * ---------------------------------------------------------------------- */
typedef struct FieldEntity {
    /* +0x00 */ short kawaiOp1;
    /* +0x02 */ unsigned short kawaiOp0;
    /* +0x04 */ unsigned char *kawaiDataOffset;
    /* +0x08 */ unsigned char blinkOn; /* 0x0062010A                        */
    /* +0x09 */ unsigned char kawaiA;
    /* +0x0A */ unsigned char kawaiB;
    /* +0x0B */ unsigned char kawaiC;
    /* +0x0C */ int posX; /* 0x006186D6                                     */
    /* +0x10 */ int posY; /* 0x006186FF                                     */
    /* +0x14 */ int posZ;
    /* +0x18 */ int moveStartX;
    /* +0x1C */ int moveStartY;
    /* +0x20 */ int moveStartZ;
    /* +0x24 */ unsigned char pad_24[8];
    /* +0x2C */ int moveB;
    /* +0x30 */ short moveSteps; /* 0x00615EB8                              */
    /* +0x32 */ short moveStep;
    /* +0x34 */ unsigned char pad_34;
    /* +0x35 */ unsigned char moveDirAdd;
    /* +0x36 */ unsigned char moveDir;
    /* +0x37 */ unsigned char dirLock; /* 0x00615C77                        */
    /* +0x38 */ unsigned char dir; /* 0x006184E9                            */
    /* +0x39 */ unsigned char turnSteps;
    /* +0x3A */ unsigned char turnStep;
    /* +0x3B */ unsigned char turnType;
    /* +0x3C */ short turnStart;
    /* +0x3E */ short turnEnd;
    /* +0x40 */ int offsetX; /* 0x0060C13C; s16 on the PSX                  */
    /* +0x44 */ short offsetStartX; /* 0x0060C180                          */
    /* +0x46 */ short offsetEndX; /* 0x0060C1C1                            */
    /* +0x48 */ int offsetY; /* 0x0060C153; s16 on the PSX                  */
    /* +0x4C */ short offsetStartY; /* 0x0060C196                          */
    /* +0x4E */ short offsetEndY; /* 0x0060C1D7                            */
    /* +0x50 */ int offsetZ; /* 0x0060C169; s16 on the PSX                  */
    /* +0x54 */ short offsetStartZ; /* 0x0060C1AB                          */
    /* +0x56 */ short offsetEndZ; /* 0x0060C1EC                            */
    /* +0x58 */ unsigned short offsetSteps;
    /* +0x5A */ unsigned short offsetStep;
    /* +0x5C */ unsigned char ofsType;
    /* +0x5D */ unsigned char entityId; /* 0x006143E9, set by CHAR          */
    /* +0x5E */ unsigned char requestPushScript;
    /* +0x5F */ unsigned char solidOff; /* 0x0061CA30, set by SOLID         */
    /* +0x60 */ unsigned char requestTalkScript;
    /* +0x61 */ unsigned char talkOff; /* 0x00618AD3, set by TLKON          */
    /* +0x62 */ unsigned char visible; /* 0x00618A54, set by VISI           */
    /* +0x63 */ unsigned char scriptedMoveMode;
    /* +0x64 */ unsigned char activeAnimId;
    /* +0x65 */ char pad_65;
    /* +0x66 */ short animSpeed; /* 0x00618418, set by DFANM                */
    /* +0x68 */ short animCurrentFrame; /* 0x0061528D                       */
    /* +0x6A */ short animLastFrame; /* 0x0061529E                          */
    /* +0x6C */ unsigned short charId; /* 0x006143B1, set by CHAR           */
    /* +0x6E */ short actionArg;
    /* +0x70 */ short actionState;
    /* +0x72 */ unsigned short solidRange; /* 0x00618227, fieldScale * k    */
    /* +0x74 */ unsigned short talkRange; /* 0x0061833D, likewise           */
    /* +0x76 */ unsigned short moveSpeed; /* 0x006183C7, likewise           */
    /* +0x78 */ unsigned short posI; /* 0x0061866B, read by GETAI           */
    /* +0x7A */ short moveEndI;
    /* +0x7C */ int moveEndX;
    /* +0x80 */ int moveEndY;
    /* +0x84 */ int moveEndZ;
} FieldEntity;

/* ------------------------------------------------------------------------
 * LinePos / FieldLine -- 0x18 bytes, at 0x00CC1F70 (g_FieldLines[0x20]).
 *
 * Matches the PSX 0x18-byte trigger-line layout byte for byte: six 16-bit
 * endpoint coordinates (`x1..z2`, written from six consecutive operand pairs
 * by `Script_OpSetLinePoints` at 0x006114D0), active/entity/touch/across state bytes,
 * script request flags, proximity angle, on-line state, `slipDisabled` at
 * +0x16 (0x00CC1F86, 22 bytes from base), and `pad_17` at +0x17 (PSX `approaching`).
 * ---------------------------------------------------------------------- */
typedef struct LinePos {
    /* +0x00 */ short x1;
    /* +0x02 */ short y1;
    /* +0x04 */ short z1;
    /* +0x06 */ short x2;
    /* +0x08 */ short y2;
    /* +0x0A */ short z2;
} LinePos;
typedef struct FieldLine {
    /* +0x00 */ LinePos pos; /* 0x006114EF..0x00611591                       */
    /* +0x0C */ unsigned char isActive; /* 0x006115CB, set by LINON          */
    /* +0x0D */ unsigned char entityId;
    /* +0x0E */ unsigned char touch; /* 0x00611630, cleared when deactivated */
    /* +0x0F */ unsigned char across;
    /* +0x10 */ unsigned char requestPushScript;
    /* +0x11 */ unsigned char requestTalkScript;
    /* +0x12 */ unsigned char requestTouchOnScript;
    /* +0x13 */ unsigned char requestTouchOffScript;
    /* +0x14 */ unsigned char proximityAngle;
    /* +0x15 */ unsigned char isOnLine;
    /* +0x16 */ unsigned char slipDisabled; /* 0x00611666                    */
    /* +0x17 */ unsigned char pad_17;
} FieldLine;


/* ------------------------------------------------------------------------
 * FieldRequest -- the block *g_pFieldRequest points at.
 *
 * 0x006137BD clears a word at +0x00 and then six dwords at +0x04, which sizes
 * it and gives the argument array its extent. The three request opcodes at
 * 0x00613AC8, 0x00613AE1 and 0x00613AFA write a code into +0x00 between a call
 * that clears the block and a call that submits it.
 * ---------------------------------------------------------------------- */
typedef struct FieldRequest {
    /* +0x00 */ short code;
    /* +0x02 */ short pad_02;
    /* +0x04 */ int args[6]; /* 0x006137EB, [eax + edx*4 + 4]                 */
} FieldRequest;



/* The script stream itself. `SCRIPT_PARAM_U8(1)` is the byte after the
 * opcode, and compiles at /Od to
 *
 *     xor  eax, eax
 *     mov  al, byte ptr [g_CurrentEntity]
 *     xor  ecx, ecx
 *     mov  cx, word ptr [eax*2 + g_FieldScriptPC]
 *     mov  edx, dword ptr [g_FieldScripts]
 *     mov  al, byte ptr [edx + ecx + 1]
 *
 * -- the three terms fold into one addressing mode. This is the PSX
 * GET_PARAM_U8 macro from src/field/field_private.h under another name; the
 * spelling below is what reproduces the PC instruction stream. */
#define SCRIPT_PARAM_U8(offset) \
    (*((unsigned char *)g_FieldScripts + g_FieldScriptPC[g_CurrentEntity] + (offset)))

/* The same byte read SIGNED. 0x00619247 is `movsx cx, byte ptr [...]` where
 * every other operand fetch in the VM is `movzx`, so the distinction is real
 * and belongs in the source rather than in a cast at one call site. */
#define SCRIPT_PARAM_S8(offset) \
    (*((char *)g_FieldScripts + g_FieldScriptPC[g_CurrentEntity] + (offset)))

/* The two bytes of entity `entity`'s script `script` entry, a little-endian offset into the
 * script image: the entries follow the 0x20-byte header, an 8-byte name per entity and the AKAO
 * block offsets (`akaoBytes` is akaoBlockCount << 2), 32 per entity. The original reads the two
 * bytes in separate expressions whose terms are summed in different orders; each macro keeps its
 * order (Script_RequestEntityScript, Script_OpReturnTo, Script_TriggerEntityScript). */
#define FIELD_SCRIPT_ENTRY_LO(entity, script, akaoBytes) \
    (*(unsigned char *)(g_FieldScripts->entityCount * 8 + ((entity) << 6) + (script) * 2 + 0x20 + \
                        (akaoBytes) + (char *)g_FieldScripts))
#define FIELD_SCRIPT_ENTRY_HI(entity, script, akaoBytes) \
    (*(unsigned char *)(g_FieldScripts->entityCount * 8 + 0x20 + (((entity) << 6) + (int)g_FieldScripts) + \
                        (script) * 2 + (akaoBytes) + 1))

/* ------------------------------------------------------------------------
 * The operand accessors.
 *
 * `bank` is 1-based: both readers subtract one and switch on the result with
 * a six-way jump table, so banks 1..6 exist. The PSX header gives these the
 * signature (s16, s16), and 0x0060F756 `movsx eax, word ptr [ebp+8]` agrees.
 *
 * Script_ReadOperand8 returns a byte, and a caller that promotes it emits
 * `and eax, 0xff` rather than `movzx` -- that mask is the return type, not an
 * explicit mask in the source. Script_ReadOperand16 returns a signed short and
 * its callers emit `movsx eax, ax`.
 * ---------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif
unsigned char __cdecl Script_ReadOperand8(short nibbleSelector, short instrOffset);
void __cdecl Script_WriteOperand8(short nibbleSelector, short instrOffset, unsigned char value);
short __cdecl Script_ReadOperand16(short bank, short addr);
void __cdecl Script_WriteOperand16(short nibbleSelector, short instrOffset, short value);
#ifdef __cplusplus
}
#endif

/* hub FieldActorState begin */
/* The field state g_pFieldState points at (Script_BeginMap), then 4 bytes nothing reads. */
#pragma pack(push, 1)
struct FieldActorState {
    FieldState state; /* +0x0 */
    unsigned char pad_134[0x4];
};
#pragma pack(pop)
/* hub FieldActorState end */

#endif /* FF7_FIELD_H */

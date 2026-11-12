// Contents: Starts field maps and runs the field script state each frame: begins map scripts,
//   updates the clock, resets state for a new game, triggers entity scripts, and maintains model
//   animation and the hand cursor.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/codegen.h"
#include "ff7/enums/main.h"
#include "ff7/enums/field.h"
#include "ff7/api/field.h"
#include "ff7/api/main.h"
#include "ff7/field.h"
#include "ff7/savemap.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include <stdlib.h>
#include <string.h>
#include "lib/enums/file.h"
#include "ff7/types/condor.h"
#include "ff7/enums/condor.h"
#include "lib/types/graphics.h"
#include "ff7/enums/battle.h"
#include "ff7/types/menu.h"
#include "lib/enums/sound.h"
extern "C" {

// GLOBAL: FF7 0x009055a0
FieldOpcodeHandler g_FieldScriptOpTable[256] = {
    Script_OpReturn,                                       /* 0x00 FIELD_OP_RETURN */
    Script_OpReq,                                          /* 0x01 FIELD_OP_REQ */
    Script_OpReqSyncWait,                                  /* 0x02 FIELD_OP_REQ_SYNC_WAIT */
    Script_OpReqEndWait,                                   /* 0x03 FIELD_OP_REQ_END_WAIT */
    Script_OpPartyReq,                                     /* 0x04 FIELD_OP_PARTY_REQ */
    Script_OpPartyReqSyncWait,                             /* 0x05 FIELD_OP_PARTY_REQ_SYNC_WAIT */
    Script_OpPartyReqEndWait,                              /* 0x06 FIELD_OP_PARTY_REQ_END_WAIT */
    Script_OpReturnTo,                                     /* 0x07 FIELD_OP_RETURN_TO */
    Script_OpPartyJoinLeader,                              /* 0x08 FIELD_OP_PARTY_JOIN_LEADER */
    Script_OpPartySplitTo,                                 /* 0x09 FIELD_OP_PARTY_SPLIT_TO */
    Script_OpSetFieldParty,                                /* 0x0A FIELD_OP_SET_FIELD_PARTY */
    Script_OpGetFieldParty,                                /* 0x0B FIELD_OP_GET_FIELD_PARTY */
    Script_OpUnimplementedStall,                           /* 0x0C FIELD_OP_UNIMPLEMENTED_STALL_0C */
    Script_OpUnimplementedStall,                           /* 0x0D FIELD_OP_UNIMPLEMENTED_STALL_0D */
    Script_OpRequestDiscChange,                            /* 0x0E FIELD_OP_REQUEST_DISC_CHANGE */
    Script_OpSpecial,                                      /* 0x0F FIELD_OP_SPECIAL */
    Script_OpJumpForward,                                  /* 0x10 FIELD_OP_JUMP_FORWARD */
    Script_OpJumpForwardLong,                              /* 0x11 FIELD_OP_JUMP_FORWARD_LONG */
    Script_OpJumpBack,                                     /* 0x12 FIELD_OP_JUMP_BACK */
    Script_OpJumpBackLong,                                 /* 0x13 FIELD_OP_JUMP_BACK_LONG */
    Script_OpIfByte,                                       /* 0x14 FIELD_OP_IF_BYTE */
    Script_OpIfByteLong,                                   /* 0x15 FIELD_OP_IF_BYTE_LONG */
    Script_OpIfWordSigned,                                 /* 0x16 FIELD_OP_IF_WORD_SIGNED */
    Script_OpIfWordSignedLong,                             /* 0x17 FIELD_OP_IF_WORD_SIGNED_LONG */
    Script_OpIfWordUnsigned,                               /* 0x18 FIELD_OP_IF_WORD_UNSIGNED */
    Script_OpIfWordUnsignedLong,                           /* 0x19 FIELD_OP_IF_WORD_UNSIGNED_LONG */
    Script_OpUnimplementedStall,                           /* 0x1A FIELD_OP_UNIMPLEMENTED_STALL_1A */
    Script_OpUnimplementedStall,                           /* 0x1B FIELD_OP_UNIMPLEMENTED_STALL_1B */
    Script_OpUnimplementedStall,                           /* 0x1C FIELD_OP_UNIMPLEMENTED_STALL_1C */
    Script_OpUnimplementedStall,                           /* 0x1D FIELD_OP_UNIMPLEMENTED_STALL_1D */
    Script_OpUnimplementedStall,                           /* 0x1E FIELD_OP_UNIMPLEMENTED_STALL_1E */
    Script_OpUnimplementedStall,                           /* 0x1F FIELD_OP_UNIMPLEMENTED_STALL_1F */
    Script_OpMinigame,                                     /* 0x20 FIELD_OP_MINIGAME */
    Script_OpMenu,                                         /* 0x21 FIELD_OP_MENU */
    Script_OpSetBattleFlagsWord,                           /* 0x22 FIELD_OP_SET_BATTLE_FLAGS_WORD */
    Script_OpGetBattleResult,                              /* 0x23 FIELD_OP_GET_BATTLE_RESULT */
    Script_OpWaitFrames,                                   /* 0x24 FIELD_OP_WAIT_FRAMES */
    Script_OpFadeParams,                                   /* 0x25 FIELD_OP_FADE_PARAMS */
    Script_OpSetModelBlink,                                /* 0x26 FIELD_OP_SET_MODEL_BLINK */
    Script_OpSetSpeedBoost,                                /* 0x27 FIELD_OP_SET_SPEED_BOOST */
    Script_OpModelGfxCommand,                              /* 0x28 FIELD_OP_MODEL_GFX_COMMAND */
    Script_OpWaitModelGfxCommand,                          /* 0x29 FIELD_OP_WAIT_MODEL_GFX_COMMAND */
    Script_OpMoveToPartyMember,                            /* 0x2A FIELD_OP_MOVE_TO_PARTY_MEMBER */
    Script_OpSetLineSlipDisabled,                          /* 0x2B FIELD_OP_SET_LINE_FLAG_16 */
    Script_OpSetLayerDepth,                                /* 0x2C FIELD_OP_SET_LAYER_DEPTH */
    Script_OpSetLayerScroll,                               /* 0x2D FIELD_OP_SET_LAYER_SCROLL */
    Script_OpCloseWindowAndWait,                           /* 0x2E FIELD_OP_CLOSE_WINDOW_AND_WAIT */
    Script_OpReopenWindow,                                 /* 0x2F FIELD_OP_REOPEN_WINDOW */
    Script_OpIfButtonHeld,                                 /* 0x30 FIELD_OP_IF_BUTTON_HELD */
    Script_OpIfButtonPressed,                              /* 0x31 FIELD_OP_IF_BUTTON_PRESSED */
    Script_OpIfButtonReleased,                             /* 0x32 FIELD_OP_IF_BUTTON_RELEASED */
    Script_OpSetDialogueLock,                              /* 0x33 FIELD_OP_SET_DIALOGUE_LOCK */
    Script_OpFacePartyMember,                              /* 0x34 FIELD_OP_FACE_PARTY_MEMBER */
    Script_OpTurnToPartyMember,                            /* 0x35 FIELD_OP_TURN_TO_PARTY_MEMBER */
    Script_OpWindowSetNumberDisplay,                       /* 0x36 FIELD_OP_WINDOW_SET_NUMBER_DISPLAY */
    Script_OpWindowSetNumber,                              /* 0x37 FIELD_OP_WINDOW_SET_NUMBER */
    Script_OpSetCountdown,                                 /* 0x38 FIELD_OP_SET_COUNTDOWN */
    Script_OpAddGil,                                       /* 0x39 FIELD_OP_ADD_GIL */
    Script_OpSpendGil,                                     /* 0x3A FIELD_OP_SPEND_GIL */
    Script_OpGetGil,                                       /* 0x3B FIELD_OP_GET_GIL */
    Script_OpFullHeal,                                     /* 0x3C FIELD_OP_FULL_HEAL_3C */
    Script_OpFullHeal,                                     /* 0x3D FIELD_OP_FULL_HEAL_3D */
    Script_OpHealEveryone,                                 /* 0x3E FIELD_OP_HEAL_EVERYONE */
    Script_OpFullHeal,                                     /* 0x3F FIELD_OP_FULL_HEAL_3F */
    Script_OpShowMessage,                                  /* 0x40 FIELD_OP_SHOW_MESSAGE */
    Script_OpWindowBindParam8,                             /* 0x41 FIELD_OP_WINDOW_BIND_PARAM_8 */
    Script_OpWindowBindParam16,                            /* 0x42 FIELD_OP_WINDOW_BIND_PARAM_16 */
    Script_OpSetLocationName,                              /* 0x43 FIELD_OP_SET_LOCATION_NAME */
    Script_OpUnimplementedStall,                           /* 0x44 FIELD_OP_UNIMPLEMENTED_STALL_44 */
    Script_OpHealMp,                                       /* 0x45 FIELD_OP_HEAL_MP */
    Script_OpUnimplementedStall,                           /* 0x46 FIELD_OP_UNIMPLEMENTED_STALL_46 */
    Script_OpDamageMp,                                     /* 0x47 FIELD_OP_DAMAGE_MP */
    Script_OpAskChoice,                                    /* 0x48 FIELD_OP_ASK_CHOICE */
    Script_OpFieldRequest,                                 /* 0x49 FIELD_OP_FIELD_REQUEST */
    Script_OpSetMenuLocked,                                /* 0x4A FIELD_OP_SET_MENU_LOCKED */
    Script_OpSetEncounterTable,                            /* 0x4B FIELD_OP_SET_ENCOUNTER_TABLE */
    Script_OpUnimplementedStall,                           /* 0x4C FIELD_OP_UNIMPLEMENTED_STALL_4C */
    Script_OpHealHp,                                       /* 0x4D FIELD_OP_HEAL_HP */
    Script_OpUnimplementedStall,                           /* 0x4E FIELD_OP_UNIMPLEMENTED_STALL_4E */
    Script_OpDamageHp,                                     /* 0x4F FIELD_OP_DAMAGE_HP */
    Script_OpWindowCreate,                                 /* 0x50 FIELD_OP_WINDOW_CREATE */
    Script_OpWindowResize,                                 /* 0x51 FIELD_OP_WINDOW_RESIZE */
    Script_OpWindowSetMode,                                /* 0x52 FIELD_OP_WINDOW_SET_MODE */
    Script_OpWindowClose,                                  /* 0x53 FIELD_OP_WINDOW_CLOSE */
    Script_OpWindowWaitClosed,                             /* 0x54 FIELD_OP_WINDOW_WAIT_CLOSED */
    Script_OpWindowSetRows,                                /* 0x55 FIELD_OP_WINDOW_SET_ROWS */
    Script_OpGetWindowColour,                              /* 0x56 FIELD_OP_GET_WINDOW_COLOUR */
    Script_OpSetWindowColour,                              /* 0x57 FIELD_OP_SET_WINDOW_COLOUR */
    Script_OpAddItem,                                      /* 0x58 FIELD_OP_ADD_ITEM */
    Script_OpRemoveItem,                                   /* 0x59 FIELD_OP_REMOVE_ITEM */
    Script_OpGetItemQty,                                   /* 0x5A FIELD_OP_GET_ITEM_QTY */
    Script_OpAddMateria,                                   /* 0x5B FIELD_OP_ADD_MATERIA */
    Script_OpRemoveMateria,                                /* 0x5C FIELD_OP_REMOVE_MATERIA */
    Script_OpCountMateria,                                 /* 0x5D FIELD_OP_COUNT_MATERIA */
    Script_OpShake,                                        /* 0x5E FIELD_OP_SHAKE */
    Script_OpYieldOneOpcode,                               /* 0x5F FIELD_OP_YIELD_ONE_OPCODE */
    Script_OpMapJump,                                      /* 0x60 FIELD_OP_MAP_JUMP */
    Script_OpSetUnusedFlag37,                              /* 0x61 FIELD_OP_SET_UNUSED_FLAG_37 */
    Script_OpScrollToPlayer,                               /* 0x62 FIELD_OP_SCROLL_TO_PLAYER */
    Script_OpScrollToEntity,                               /* 0x63 FIELD_OP_SCROLL_TO_ENTITY */
    Script_OpScrollToPoint,                                /* 0x64 FIELD_OP_SCROLL_TO_POINT */
    Script_OpScrollReset,                                  /* 0x65 FIELD_OP_SCROLL_RESET */
    Script_OpScrollToPointLinear,                          /* 0x66 FIELD_OP_SCROLL_TO_POINT_LINEAR */
    Script_OpWaitScroll,                                   /* 0x67 FIELD_OP_WAIT_SCROLL */
    Script_OpScrollToPointSmooth,                          /* 0x68 FIELD_OP_SCROLL_TO_POINT_SMOOTH */
    Script_OpSetFieldHidden,                               /* 0x69 FIELD_OP_SET_FIELD_HIDDEN */
    Script_OpSetViewOffset,                                /* 0x6A FIELD_OP_SET_VIEW_OFFSET */
    Script_OpScreenFade,                                   /* 0x6B FIELD_OP_SCREEN_FADE */
    Script_OpWaitFade,                                     /* 0x6C FIELD_OP_WAIT_FADE */
    Script_OpSetAccessBlocked,                             /* 0x6D FIELD_OP_SET_ACCESS_BLOCKED */
    Script_OpGetFieldMapId,                                /* 0x6E FIELD_OP_GET_FIELD_MAP_ID */
    Script_OpScrollToPartyMember,                          /* 0x6F FIELD_OP_SCROLL_TO_PARTY_MEMBER */
    Script_OpEnterBattle,                                  /* 0x70 FIELD_OP_ENTER_BATTLE */
    Script_OpSetEncountersDisabled,                        /* 0x71 FIELD_OP_SET_ENCOUNTERS_DISABLED */
    Script_OpSetBattleFlagsByte,                           /* 0x72 FIELD_OP_SET_BATTLE_FLAGS_BYTE */
    Script_OpGetPartyDirection,                            /* 0x73 FIELD_OP_GET_PARTY_DIRECTION */
    Script_OpGetPartyCharId,                               /* 0x74 FIELD_OP_GET_PARTY_CHAR_ID */
    Script_OpGetPartyXYZI,                                 /* 0x75 FIELD_OP_GET_PARTY_XYZI */
    Script_OpAddByteSat,                                   /* 0x76 FIELD_OP_ADD_BYTE_SAT */
    Script_OpAddWordSat,                                   /* 0x77 FIELD_OP_ADD_WORD_SAT */
    Script_OpSubByteSat,                                   /* 0x78 FIELD_OP_SUB_BYTE_SAT */
    Script_OpSubWordSat,                                   /* 0x79 FIELD_OP_SUB_WORD_SAT */
    Script_OpIncByteSat,                                   /* 0x7A FIELD_OP_INC_BYTE_SAT */
    Script_OpIncWordSat,                                   /* 0x7B FIELD_OP_INC_WORD_SAT */
    Script_OpDecByteSat,                                   /* 0x7C FIELD_OP_DEC_BYTE_SAT */
    Script_OpDecWordSat,                                   /* 0x7D FIELD_OP_DEC_WORD_SAT */
    Script_OpSetTalkOff,                                   /* 0x7E FIELD_OP_SET_TALK_OFF */
    Script_OpSetRandomStride,                              /* 0x7F FIELD_OP_SET_RANDOM_STRIDE */
    Script_OpSetByte,                                      /* 0x80 FIELD_OP_SET_BYTE */
    Script_OpSetWord,                                      /* 0x81 FIELD_OP_SET_WORD */
    Script_OpBitSet,                                       /* 0x82 FIELD_OP_BIT_SET */
    Script_OpBitClear,                                     /* 0x83 FIELD_OP_BIT_CLEAR */
    Script_OpBitToggle,                                    /* 0x84 FIELD_OP_BIT_TOGGLE */
    Script_OpAddByte,                                      /* 0x85 FIELD_OP_ADD_BYTE */
    Script_OpAddWord,                                      /* 0x86 FIELD_OP_ADD_WORD */
    Script_OpSubByte,                                      /* 0x87 FIELD_OP_SUB_BYTE */
    Script_OpSubWord,                                      /* 0x88 FIELD_OP_SUB_WORD */
    Script_OpMulByte,                                      /* 0x89 FIELD_OP_MUL_BYTE */
    Script_OpMulWord,                                      /* 0x8A FIELD_OP_MUL_WORD */
    Script_OpDivByte,                                      /* 0x8B FIELD_OP_DIV_BYTE */
    Script_OpDivWord,                                      /* 0x8C FIELD_OP_DIV_WORD */
    Script_OpModByte,                                      /* 0x8D FIELD_OP_MOD_BYTE */
    Script_OpModWord,                                      /* 0x8E FIELD_OP_MOD_WORD */
    Script_OpAndByte,                                      /* 0x8F FIELD_OP_AND_BYTE */
    Script_OpAndWord,                                      /* 0x90 FIELD_OP_AND_WORD */
    Script_OpOrByte,                                       /* 0x91 FIELD_OP_OR_BYTE */
    Script_OpOrWord,                                       /* 0x92 FIELD_OP_OR_WORD */
    Script_OpXorByte,                                      /* 0x93 FIELD_OP_XOR_BYTE */
    Script_OpXorWord,                                      /* 0x94 FIELD_OP_XOR_WORD */
    Script_OpIncByte,                                      /* 0x95 FIELD_OP_INC_BYTE */
    Script_OpIncWord,                                      /* 0x96 FIELD_OP_INC_WORD */
    Script_OpDecByte,                                      /* 0x97 FIELD_OP_DEC_BYTE */
    Script_OpDecWord,                                      /* 0x98 FIELD_OP_DEC_WORD */
    Script_OpRandomByte,                                   /* 0x99 FIELD_OP_RANDOM_BYTE */
    Script_OpSetByteAlias,                                 /* 0x9A FIELD_OP_SET_BYTE_ALIAS */
    Script_OpGetHighByte,                                  /* 0x9B FIELD_OP_GET_HIGH_BYTE */
    Script_OpSetWordFromBytes,                             /* 0x9C FIELD_OP_SET_WORD_FROM_BYTES */
    Script_OpStoreByteIndexed,                             /* 0x9D FIELD_OP_STORE_BYTE_INDEXED */
    Script_OpLoadByteIndexed,                              /* 0x9E FIELD_OP_LOAD_BYTE_INDEXED */
    Script_OpSearchBankForByte,                            /* 0x9F FIELD_OP_SEARCH_BANK_FOR_BYTE */
    Script_OpBindCharacter,                                /* 0xA0 FIELD_OP_BIND_CHARACTER */
    Script_OpAttachModel,                                  /* 0xA1 FIELD_OP_ATTACH_MODEL */
    Script_OpSetIdleAnimation,                             /* 0xA2 FIELD_OP_SET_IDLE_ANIMATION */
    Script_OpAnimateAndWait,                               /* 0xA3 FIELD_OP_ANIMATE_AND_WAIT_A3 */
    Script_OpSetVisible,                                   /* 0xA4 FIELD_OP_SET_VISIBLE */
    Script_OpSetPositionXYZI,                              /* 0xA5 FIELD_OP_SET_POSITION_XYZI */
    Script_OpSetPositionXYI,                               /* 0xA6 FIELD_OP_SET_POSITION_XYI */
    Script_OpSetPositionXYZ,                               /* 0xA7 FIELD_OP_SET_POSITION_XYZ */
    Script_OpMoveTo,                                       /* 0xA8 FIELD_OP_MOVE_TO */
    Script_OpMoveToStyled,                                 /* 0xA9 FIELD_OP_MOVE_TO_STYLED */
    Script_OpMoveToEntity,                                 /* 0xAA FIELD_OP_MOVE_TO_ENTITY */
    Script_OpTurnToEntity,                                 /* 0xAB FIELD_OP_TURN_TO_ENTITY */
    Script_OpWaitAnimationDone,                            /* 0xAC FIELD_OP_WAIT_ANIMATION_DONE */
    Script_OpMoveToDirect,                                 /* 0xAD FIELD_OP_MOVE_TO_DIRECT */
    Script_OpAnimateAndWait,                               /* 0xAE FIELD_OP_ANIMATE_AND_WAIT_AE */
    Script_OpAnimateAndWaitAlt,                            /* 0xAF FIELD_OP_ANIMATE_AND_WAIT_ALT_AF */
    Script_OpPlayAnimation,                                /* 0xB0 FIELD_OP_PLAY_ANIMATION_B0 */
    Script_OpPlayAnimationFrom,                            /* 0xB1 FIELD_OP_PLAY_ANIMATION_FROM_B1 */
    Script_OpSetMoveSpeed,                                 /* 0xB2 FIELD_OP_SET_MOVE_SPEED */
    Script_OpSetDirection,                                 /* 0xB3 FIELD_OP_SET_DIRECTION */
    Script_OpTurnStart,                                    /* 0xB4 FIELD_OP_TURN_START */
    Script_OpTurnToAngle,                                  /* 0xB5 FIELD_OP_TURN_TO_ANGLE */
    Script_OpFaceEntity,                                   /* 0xB6 FIELD_OP_FACE_ENTITY */
    Script_OpGetEntityDirection,                           /* 0xB7 FIELD_OP_GET_ENTITY_DIRECTION */
    Script_OpGetEntityXY,                                  /* 0xB8 FIELD_OP_GET_ENTITY_XY */
    Script_OpGetEntityTriangle,                            /* 0xB9 FIELD_OP_GET_ENTITY_TRIANGLE */
    Script_OpAnimateAndWaitAlt,                            /* 0xBA FIELD_OP_ANIMATE_AND_WAIT_ALT_BA */
    Script_OpPlayAnimation,                                /* 0xBB FIELD_OP_PLAY_ANIMATION_BB */
    Script_OpPlayAnimationFrom,                            /* 0xBC FIELD_OP_PLAY_ANIMATION_FROM_BC */
    Script_OpSetAnimationSpeed,                            /* 0xBD FIELD_OP_SET_ANIMATION_SPEED */
    Script_OpUnimplementedStall,                           /* 0xBE FIELD_OP_UNIMPLEMENTED_STALL_BE */
    Script_OpSetPlayerEntity,                              /* 0xBF FIELD_OP_SET_PLAYER_ENTITY */
    Script_OpMoveTo3D,                                     /* 0xC0 FIELD_OP_MOVE_TO_3D */
    Script_OpGetEntityXYZI,                                /* 0xC1 FIELD_OP_GET_ENTITY_XYZI */
    Script_OpMoveToMode,                                   /* 0xC2 FIELD_OP_MOVE_TO_MODE */
    Script_OpJumpTo,                                       /* 0xC3 FIELD_OP_JUMP_TO */
    Script_OpWaitOffsetDone,                               /* 0xC4 FIELD_OP_WAIT_OFFSET_DONE */
    Script_OpSetCollisionRadius,                           /* 0xC5 FIELD_OP_SET_COLLISION_RADIUS */
    Script_OpSetTalkRadius,                                /* 0xC6 FIELD_OP_SET_TALK_RADIUS */
    Script_OpSetSolid,                                     /* 0xC7 FIELD_OP_SET_SOLID */
    Script_OpPartyAdd,                                     /* 0xC8 FIELD_OP_PARTY_ADD */
    Script_OpPartyRemove,                                  /* 0xC9 FIELD_OP_PARTY_REMOVE */
    Script_OpPartySet,                                     /* 0xCA FIELD_OP_PARTY_SET */
    Script_OpIfCharInParty,                                /* 0xCB FIELD_OP_IF_CHAR_IN_PARTY */
    Script_OpIfCharAvailable,                              /* 0xCC FIELD_OP_IF_CHAR_AVAILABLE */
    Script_OpSetCharAvailable,                             /* 0xCD FIELD_OP_SET_CHAR_AVAILABLE */
    Script_OpMemberLock,                                   /* 0xCE FIELD_OP_MEMBER_LOCK */
    Script_OpMemberUnlock,                                 /* 0xCF FIELD_OP_MEMBER_UNLOCK */
    Script_OpDefineLine,                                   /* 0xD0 FIELD_OP_DEFINE_LINE */
    Script_OpSetLineEnabled,                               /* 0xD1 FIELD_OP_SET_LINE_ENABLED */
    Script_OpSetGatewaysDisabled,                          /* 0xD2 FIELD_OP_SET_GATEWAYS_DISABLED */
    Script_OpSetLinePoints,                                /* 0xD3 FIELD_OP_SET_LINE_POINTS */
    Script_OpSinScaled,                                    /* 0xD4 FIELD_OP_SIN_SCALED */
    Script_OpCosScaled,                                    /* 0xD5 FIELD_OP_COS_SCALED */
    Script_OpSetCollisionRadiusW,                          /* 0xD6 FIELD_OP_SET_COLLISION_RADIUS_W */
    Script_OpSetTalkRadiusW,                               /* 0xD7 FIELD_OP_SET_TALK_RADIUS_W */
    Script_OpSetPreloadMapId,                              /* 0xD8 FIELD_OP_SET_PRELOAD_MAP_ID */
    Script_OpWaitPreloadDone,                              /* 0xD9 FIELD_OP_WAIT_PRELOAD_DONE */
    Script_OpSoundCommandLong,                             /* 0xDA FIELD_OP_SOUND_COMMAND_LONG */
    Script_OpSetMoveStyle,                                 /* 0xDB FIELD_OP_SET_MOVE_STYLE */
    Script_OpSetStandardAnimation,                         /* 0xDC FIELD_OP_SET_STANDARD_ANIMATION */
    Script_OpStopAnimation,                                /* 0xDD FIELD_OP_STOP_ANIMATION */
    Script_OpWaitTurnDone,                                 /* 0xDE FIELD_OP_WAIT_TURN_DONE */
    Script_OpPaletteMultiplyRange,                         /* 0xDF FIELD_OP_PALETTE_MULTIPLY_RANGE */
    Script_OpBgOn,                                         /* 0xE0 FIELD_OP_BG_ON */
    Script_OpBgOff,                                        /* 0xE1 FIELD_OP_BG_OFF */
    Script_OpBgRol,                                        /* 0xE2 FIELD_OP_BG_ROL */
    Script_OpBgRol2,                                       /* 0xE3 FIELD_OP_BG_ROL_2 */
    Script_OpBgClr,                                        /* 0xE4 FIELD_OP_BG_CLR */
    Script_OpPaletteStore,                                 /* 0xE5 FIELD_OP_PALETTE_STORE */
    Script_OpPaletteLoad,                                  /* 0xE6 FIELD_OP_PALETTE_LOAD */
    Script_OpPaletteCopy,                                  /* 0xE7 FIELD_OP_PALETTE_COPY */
    Script_OpPaletteRotate,                                /* 0xE8 FIELD_OP_PALETTE_ROTATE */
    Script_OpPaletteAdd,                                   /* 0xE9 FIELD_OP_PALETTE_ADD */
    Script_OpPaletteMultiply,                              /* 0xEA FIELD_OP_PALETTE_MULTIPLY */
    Script_OpPaletteStorePartial,                          /* 0xEB FIELD_OP_PALETTE_STORE_PARTIAL */
    Script_OpPaletteLoadPartial,                           /* 0xEC FIELD_OP_PALETTE_LOAD_PARTIAL */
    Script_OpPaletteCopyRange,                             /* 0xED FIELD_OP_PALETTE_COPY_RANGE */
    Script_OpPaletteRotateRange,                           /* 0xEE FIELD_OP_PALETTE_ROTATE_RANGE */
    Script_OpPaletteAddRange,                              /* 0xEF FIELD_OP_PALETTE_ADD_RANGE */
    (int (__cdecl *)(void))&Script_OpSubmitMusicRequest10, /* 0xF0 FIELD_OP_SUBMIT_MUSIC_REQUEST_10 */
    Script_OpSoundEffect,                                  /* 0xF1 FIELD_OP_SOUND_EFFECT */
    Script_OpSoundCommand,                                 /* 0xF2 FIELD_OP_SOUND_COMMAND */
    (int (__cdecl *)(void))&Script_OpSubmitMusicRequest14, /* 0xF3 FIELD_OP_SUBMIT_MUSIC_REQUEST_14 */
    (int (__cdecl *)(void))&Script_OpSubmitMusicRequest15, /* 0xF4 FIELD_OP_SUBMIT_MUSIC_REQUEST_15 */
    Script_OpSetAudioDisabled,                             /* 0xF5 FIELD_OP_SET_AUDIO_DISABLED */
    Script_OpSetMusicId,                                   /* 0xF6 FIELD_OP_SET_MUSIC_ID */
    Script_OpGetFieldFxParams,                             /* 0xF7 FIELD_OP_GET_FIELD_FX_PARAMS */
    Script_OpMovieOpen,                                    /* 0xF8 FIELD_OP_MOVIE_OPEN */
    Script_OpMoviePlay,                                    /* 0xF9 FIELD_OP_MOVIE_PLAY */
    Script_OpGetMovieFrame,                                /* 0xFA FIELD_OP_GET_MOVIE_FRAME */
    Script_OpSetMovieCameraDisabled,                       /* 0xFB FIELD_OP_SET_MOVIE_CAMERA_DISABLED */
    Script_OpSetMusicIdAlt,                                /* 0xFC FIELD_OP_SET_MUSIC_ID_ALT */
    Script_OpSubmitRequestWithArgs,                        /* 0xFD FIELD_OP_SUBMIT_REQUEST_WITH_ARGS */
    Script_OpGetMusicPlaying,                              /* 0xFE FIELD_OP_GET_MUSIC_PLAYING */
    Script_OpFieldRequest1A                                /* 0xFF FIELD_OP_FIELD_REQUEST_1A */
};
// GLOBAL: FF7 0x009059a0
unsigned char g_FieldRandomTable[0x100] = {0xb1, 0xca, 0xee, 0x6c, 0x5a, 0x71, 0x2e, 0x55, 0xd6, 0x00, 0xcc, 0x99, 0x90, 0x6b, 0x7d, 0xeb, 0x4f, 0xa0, 0x07, 0xac, 0xdf, 0x8a, 0x56, 0x9e, 0xf1, 0x9a, 0x63, 0x75, 0x11, 0x91, 0xa3, 0xb8, 0x94, 0x73, 0xf7, 0x54, 0xd9, 0x6e, 0x72, 0xc0, 0xf4, 0x80, 0xde, 0xb9, 0xbb, 0x8d, 0x66, 0x26, 0xd0, 0x36, 0xe1, 0xe9, 0x70, 0xdc, 0xcd, 0x2f, 0x4a, 0x67, 0x5d, 0xd2, 0x60, 0xb5, 0x9d, 0x7f, 0x45, 0x37, 0x50, 0x44, 0x78, 0x04, 0x19, 0x2c, 0xef, 0xfd, 0x64, 0x81, 0x03, 0xda, 0x95, 0x4c, 0x7a, 0x0b, 0xad, 0x1f, 0xba, 0xdd, 0x3e, 0xf9, 0xd7, 0x1a, 0x29, 0xf8, 0x18, 0xb3, 0x20, 0xf6, 0xd1, 0x5e, 0x34, 0x92, 0x7b, 0x24, 0x43, 0x88, 0x97, 0xd4, 0x0f, 0x35, 0xaa, 0x83, 0x68, 0x27, 0xa8, 0xd5, 0xbe, 0xfa, 0x14, 0x31, 0xaf, 0x10, 0x0d, 0xd8, 0x6a, 0xce, 0x23, 0x61, 0xf3, 0x3d, 0xa4, 0x08, 0x33, 0xe3, 0xa9, 0x38, 0xe6, 0x93, 0x1d, 0x1c, 0xf0, 0x0e, 0x87, 0x59, 0x65, 0x82, 0xbc, 0xff, 0xfe, 0x7e, 0x8f, 0xc1, 0x1e, 0xf5, 0xcb, 0x49, 0x02, 0x32, 0x09, 0xc4, 0x8e, 0xc6, 0x2b, 0x40, 0xa7, 0x17, 0x76, 0x3b, 0x16, 0x2a, 0xc8, 0xfb, 0xb2, 0x58, 0xa5, 0x15, 0xae, 0x25, 0xcf, 0x46, 0xc7, 0x48, 0xb4, 0x0a, 0x3f, 0xc9, 0x06, 0x85, 0x51, 0x89, 0x62, 0x4d, 0x12, 0x8c, 0xea, 0xa2, 0x98, 0x4b, 0x79, 0x6f, 0x5c, 0x47, 0x30, 0x1b, 0xe7, 0xc5, 0x22, 0x9c, 0xe8, 0x96, 0x3a, 0xe4, 0x7c, 0xe0, 0x69, 0xa1, 0xb7, 0x05, 0x39, 0x74, 0x01, 0x9f, 0xbd, 0xc3, 0x84, 0xfc, 0x77, 0x86, 0x13, 0x4e, 0xbf, 0xf2, 0x53, 0x5b, 0xed, 0x21, 0x8b, 0x6d, 0xc2, 0x41, 0xb6, 0xdb, 0x3c, 0xd3, 0x28, 0xec, 0x2d, 0xe2, 0x9b, 0xa6, 0x42, 0x52, 0x57, 0x5f, 0xe5, 0xab, 0xb0, 0x0c};
// GLOBAL: FF7 0x00905aa0
char g_FieldDigitMapping[16] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26};
// clang-format off
// GLOBAL: FF7 0x00905ab0
int g_FieldInitialLoad = 1;
unsigned char g_rawData_00905ab4[4] = {0x00, 0x00, 0x00, 0x3e};
unsigned char g_rawData_00905ab8[4] = {0x62, 0x10, 0x78, 0x3f};
// GLOBAL: FF7 0x00cc1670
struct FieldEntity g_FieldEntityState[16] = {0};
// GLOBAL: FF7 0x00cc1ef0
char g_FieldCurrentMapName[0x80] = {0};
// GLOBAL: FF7 0x00cc1f70
FieldLine g_FieldLines[0x20] = {0};
// GLOBAL: FF7 0x00cc2270
float g_FieldCursorScaleX = 0.0f;
// GLOBAL: FF7 0x00cc2274
float g_FieldCursorScaleY = 0.0f;
// clang-format on


#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

#pragma intrinsic(memset)

#pragma intrinsic(abs)

// Copied map name; all callers pass a char[0x80] local.

/*
 * Returns zero for a NULL name. Builds the data directory plus name into a local path that is never
 * used, copies the name to g_FieldCurrentMapName, applies the engine map configuration, strips the
 * last four characters, loads the field file and stores section zero in g_FieldScripts. A name
 * shorter than four characters writes before the buffer (original defect).
 */
// FUNCTION: FF7 0x0060b9d0
int __cdecl Field_LoadMapByName(char *name)
{
    char path[256];
    int len;
    int m; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (name == 0) {
        return 0;
    }
    strcpy(path, g_FieldDataDir);
    strcat(path, name);
    strcpy(g_FieldCurrentMapName, name);
    Field_ApplyMapConfig(GetEngineContext());
    len = strlen(g_FieldCurrentMapName);
    // name[len - 4]: the folded displacement lands at g_FieldEntityState[15].moveEndZ + len.
    // BUG: the extension is cut at strlen - 4 with no length check; a map name shorter than
    // four characters writes the 0 before g_FieldCurrentMapName, into g_FieldEntityState.
    ((char *)&g_FieldEntityState[15].moveEndZ)[len] = 0;
    if (Field_LoadFieldFile(g_FieldCurrentMapName) == 0) {
        return 0;
    }
    g_FieldScripts = (FieldScriptHeader *)Field_GetSectionPtr(0);
    return 1;
}

/*
 * Does nothing.
 */
// FUNCTION: FF7 0x0060baca
void __cdecl Script_UnusedNoOp(void)
{
}

/*
 * Returns when script pointer null; otherwise assigns field state/entities, posts request, resets
 * helpers/state and conditionally resets savemap for new game. Logs Init Event! then runs all init
 * scripts.
 */
// FUNCTION: FF7 0x0060bacf
void __cdecl Script_BeginMap(struct FieldEntity *entities)
{
    int shiftedValue;

    shiftedValue = g_FieldActorState.state.activeKeys32 >> 0x10;
    if (g_FieldScripts == 0) {
        return;
    }
    g_pFieldState = &g_FieldActorState.state;
    g_pFieldEntities = entities;
    g_FieldRequestPosted = 1;
    Field_ResetMessageWindowsForField();
    Script_ResetState();
    if (g_FieldFileLoadPending != 0 || (g_FieldFileLoadParam != 0 || g_FieldReloadPending != 0)) {
        g_FieldInitialLoad = 0;
        if (g_FieldModuleEntered == 0) {
            Savemap_ResetForNewGame();
        }
    }
    // STRING: FF7 0x00905abc
    Field_NoOp("Init Event!", 0);
    Script_RunAllInitScripts();
}

/*
 * Reads shifted field-state values into unused locals, services nonzero request flag through
 * Field_ResetMessageWindowState then clears flag, and executes script frame. New requests posted by helper can be
 * cleared.
 */
// FUNCTION: FF7 0x0060bb58
void __cdecl Script_UpdateFrame(void)
{
    int activeKeysHi;
    int newKeysHi;

    activeKeysHi = *(int *)&g_pFieldState->activeKeys >> 0x10;
    newKeysHi = *(int *)&g_pFieldState->newActiveKeys >> 0x10;
    if (*(char *)&g_FieldRequestPosted != 0) {
        Field_ResetMessageWindowState();
        g_FieldRequestPosted = 0;
    }
    Script_ExecuteFrame();
}

/*
 * When the restart flag is set, clears only three field shorts and a field byte, then clears the
 * flag. Otherwise resets movie/field globals, clears the 0x500-byte save bank, initializes both
 * party arrays to 0xFF with member zero selected, sets PHS visibility to one, clears clocks and
 * field counters and restores several 0xFF/0x7FFF sentinels. It does not reset unrelated save
 * storage.
 */
// FUNCTION: FF7 0x0060bb9b
void __cdecl Savemap_ResetForNewGame(void)
{
    int padSlot[64]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int index;

    if (g_Main_FieldEntryPointActive != 0) {
        g_FieldActorState.state.fadeRedFrom = 0;
        g_FieldActorState.state.fadeGreenFrom = 0;
        g_FieldActorState.state.fadeBlueFrom = 0;
        g_FieldActorState.state.movieCameraDisabled = 0;
        g_Main_FieldEntryPointActive = 0;
    } else {
        g_FieldAudioDisabled = 0;
        g_FieldMoviePlaying = 0;
        g_FieldMoviePending = 0;
        for (index = 0; index < 0x500; index++) {
            gSavemap.scriptBanks[index] = 0;
        }
        for (index = 0; index < 3; index++) {
            gSavemap.party[index] = PARTY_CHAR_NONE;
            gSavemap.fieldParty[index] = PARTY_CHAR_NONE;
        }
        gSavemap.party[0] = PARTY_CHAR_CLOUD;
        gSavemap.phsVisibilityMask = 1;
        gSavemap.fieldParty[0] = PARTY_CHAR_CLOUD;
        gSavemap.location[0] = (char)0xFF;
        gSavemap.menuVisible = 0xFF;
        gSavemap.menuVisibleHigh = 0xFF;
        gSavemap.seconds = 0;
        gSavemap.countdownSeconds = 0;
        g_FieldActorState.state.fadeRedFrom = 0;
        g_FieldActorState.state.fadeGreenFrom = 0;
        g_FieldActorState.state.fadeBlueFrom = 0;
        g_FieldActorState.state.movieCameraDisabled = 0;
        g_PartyStatsDirty = 0;
        g_FieldActorState.state.pcTileX = 0x7FFF;
        g_FieldActorState.state.pcTileY = 0x7FFF;
        g_FieldActorState.state.pcWalkMeshId = 0;
        g_FieldActorState.state.entryId = 0;
    }
}

/*
 * Initializes field script state from the script header, clears the temporary bank and eight
 * priority tracks per entity, and resets entity model/line/follower/wait mappings. Initializes
 * each loaded model record with zero motion/animation state and field-scale-derived movement
 * parameters, clears palette and line state, resets party mappings and movie request and sets
 * shared flag bits. Counts come directly from the header.
 */
// FUNCTION: FF7 0x0060bcfa
void __cdecl Script_ResetState(void)
{
    int i;
    int j;

    g_pFieldState->requestCode = FIELD_REQUEST_NONE;
    g_pFieldState->eventCmdParam = 0;
    g_pFieldState->requestArg = 0;
    g_pFieldState->modelCount = g_FieldScripts->modelCount;
    g_pFieldState->playerModel = 0;
    g_pFieldState->idleAnimId = FIELD_STANDARD_ANIM_IDLE;
    g_pFieldState->walkAnimId = FIELD_STANDARD_ANIM_WALK;
    g_pFieldState->runAnimId = FIELD_STANDARD_ANIM_RUN;
    g_pFieldState->characterLock = 0;
    g_FieldDialogueLock = 0;
    g_pFieldState->entityMode = 0;
    g_pFieldState->menuLocked = 0;
    g_pFieldState->unusedFlag35 = 0;
    g_pFieldState->encountersDisabled = 0;
    g_pFieldState->gatewaysDisabled = 0;
    g_pFieldState->unusedFlag37 = 0;
    g_pFieldState->fieldHidden = 0;
    g_pFieldState->nextFieldMusic = 0x157C0;
    g_pFieldState->nextBattleMusic = 0;
    g_pFieldState->unusedWord40 = 0;
    g_pFieldState->battleMode2 = 0;
    g_pFieldState->battleMode1 = 0;
    g_pFieldState->encounterTable = 0;
    g_pFieldState->viewOffsetNumSteps = 0;
    g_pFieldState->viewOffsetCurrentStep = 0;
    g_pFieldState->viewOffsetMode = 0;
    g_pFieldState->viewOffsetStart = 0;
    g_pFieldState->viewOffsetTarget = 0;
    g_pFieldState->shakeX.enabled = 0;
    g_pFieldState->shakeY.enabled = 0;
    g_pFieldState->shakeX.segmentActive = 0;
    g_pFieldState->shakeY.segmentActive = 0;
    g_pFieldState->shakeX.amplitude = 0;
    g_pFieldState->shakeY.amplitude = 0;
    g_pFieldState->shakeX.numStepsPerSegment = 0;
    g_pFieldState->shakeY.numStepsPerSegment = 0;
    g_pFieldState->shakeX.currentStep = 0;
    g_pFieldState->shakeY.currentStep = 0;
    *(int *)&g_pFieldState->activeKeys = 0;
    *(int *)&g_pFieldState->activeKeys2 = 0;
    g_pFieldState->speedBoost = 0;
    g_pFieldState->cameraScrollMode = FIELD_CAMERA_SCROLL_RESET;
    g_pFieldState->fieldScale = g_FieldScripts->fieldScale;
    for (i = 0; i < 0x100; i++) {
        g_FieldTempBank[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < g_FieldScripts->entityCount; j++) {
            g_FieldScriptPriorityPC[j][i] = 0;
            g_FieldScriptPriorityState[j][i] = FIELD_SCRIPT_REQUEST_STATE_IDLE;
            g_FieldCharEntity.scriptCaller[j][i] = 0xFF;
            g_FieldEntityScriptTrack[i + j * 8] = 0;
        }
    }
    for (i = 0; i < g_FieldScripts->entityCount; i++) {
        g_FieldScriptPriority[i] = FIELD_SCRIPT_PRIORITY_MAIN;
        g_FieldEntityModel[i] = 0xFF;
        g_FieldScriptWait[i] = 0;
        g_FieldFollowerState[i] = 0;
        g_FieldEntityLine[i] = 0xFF;
    }
    for (i = 0; i < g_FieldScripts->modelCount; i++) {
        g_pFieldEntities[i].charId = 0;
        g_pFieldEntities[i].posX = 0;
        g_pFieldEntities[i].posY = 0;
        g_pFieldEntities[i].posZ = 0;
        g_pFieldEntities[i].posI = 0;
        g_pFieldEntities[i].moveEndI = 0;
        g_pFieldEntities[i].moveDir = 0;
        g_pFieldEntities[i].dir = 0;
        g_pFieldEntities[i].turnType = 0;
        g_pFieldEntities[i].turnSteps = 0;
        g_pFieldEntities[i].turnStep = 0;
        g_pFieldEntities[i].turnStart = 0;
        g_pFieldEntities[i].turnEnd = 0;
        g_pFieldEntities[i].offsetX = 0;
        g_pFieldEntities[i].offsetY = 0;
        g_pFieldEntities[i].offsetZ = 0;
        g_pFieldEntities[i].offsetStartX = 0;
        g_pFieldEntities[i].offsetStartY = 0;
        g_pFieldEntities[i].offsetStartZ = 0;
        g_pFieldEntities[i].offsetEndX = 0;
        g_pFieldEntities[i].offsetEndY = 0;
        g_pFieldEntities[i].offsetEndZ = 0;
        g_pFieldEntities[i].offsetSteps = 0;
        g_pFieldEntities[i].offsetStep = 0;
        g_pFieldEntities[i].ofsType = 0;
        g_pFieldEntities[i].activeAnimId = 0;
        g_pFieldEntities[i].animSpeed = 0x10;
        g_pFieldEntities[i].moveEndX = 0;
        g_pFieldEntities[i].moveEndY = 0;
        g_pFieldEntities[i].moveEndZ = 0;
        g_pFieldEntities[i].animCurrentFrame = 0;
        g_pFieldEntities[i].animLastFrame = 0;
        g_pFieldEntities[i].visible = 0;
        g_pFieldEntities[i].moveSpeed = (g_pFieldState->fieldScale << 0xA) / 512;
        g_pFieldEntities[i].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
        g_pFieldEntities[i].actionArg = 0;
        g_pFieldEntities[i].actionState = 0;
        g_pFieldEntities[i].requestTalkScript = 0;
        g_pFieldEntities[i].requestPushScript = 0;
        g_pFieldEntities[i].solidOff = 0;
        g_pFieldEntities[i].talkOff = 0;
        g_pFieldEntities[i].dirLock = 0;
        g_pFieldEntities[i].solidRange = (g_pFieldState->fieldScale * 0x1E) / 512;
        g_pFieldEntities[i].talkRange = (g_pFieldState->fieldScale * 0x50) / 512;
        g_FieldModelIdleAnimId[i] = 0;
        g_FieldModelAnimMode[i] = FIELD_ANIM_MODE_IDLE;
        g_FieldModelAnimSpeed[i] = 0x10;
        g_FieldModelIdleAnimSpeed[i] = 0x10;
        g_pFieldEntities[i].kawaiOp1 = 0;
        g_pFieldEntities[i].kawaiOp0 = 0;
        g_pFieldEntities[i].kawaiDataOffset = 0;
        g_pFieldEntities[i].blinkOn = 0;
        *(short *)&g_pFieldEntities[i].kawaiB = 0;
    }
    for (i = 0; i < 0x40; i++) {
        g_pFieldState->backgroundLayerVisibility[i] = 0;
    }
    for (i = 0; i < 0x40; i++) {
        g_pFieldState->blockedAccesses[i] = 0;
    }
    for (i = 0; i < 0x40; i++) {
        for (j = 0; j < 0x10; j++) {
            g_FieldPalettes[i][j] = 0;
        }
    }
    for (i = 0; i < 0x20; i++) {
        g_FieldLines[i].requestTalkScript = 0;
        g_FieldLines[i].requestPushScript = 0;
        g_FieldLines[i].across = 0;
        g_FieldLines[i].touch = 0;
        g_FieldLines[i].requestTouchOnScript = 0;
        g_FieldLines[i].requestTouchOffScript = 0;
        g_FieldLines[i].isActive = 0;
        g_FieldLines[i].entityId = 0;
        g_FieldLines[i].slipDisabled = 0;
        g_FieldLines[i].pos.x1 = 0;
        g_FieldLines[i].pos.y1 = 0;
        g_FieldLines[i].pos.z1 = 0;
        g_FieldLines[i].pos.x2 = 0;
        g_FieldLines[i].pos.y2 = 0;
        g_FieldLines[i].pos.z2 = 0;
    }
    g_FieldLineCount = 0;
    for (i = 0; i < 9; i++) {
        g_FieldCharEntity.charEntity[i] = 0xFF;
    }
    g_FieldDialogTextState[0] = 0xFF;
    gSavemap.menuFlags |= 3;
    g_FieldMovieRequested = 0;
}

/*
 * Clears the field model count and visits each entity indicated by the script header byte. Reads
 * that entity initial script offset from the header table, dispatches opcodes through
 * g_FieldScriptOpTable until a zero opcode, advances past that zero and finally resets
 * g_CurrentEntity to zero. The header offset intermediate narrows to short.
 */
// FUNCTION: FF7 0x0060c683
void __cdecl Script_RunAllInitScripts(void)
{
    short val;
    g_FieldModelCount = 0;
    for (g_CurrentEntity = 0; g_CurrentEntity < g_FieldScripts->entityCount; g_CurrentEntity++) {
        val = g_FieldScripts->akaoBlockCount << 2;
        g_FieldScriptPC[g_CurrentEntity] = (int)*(unsigned char *)(((int)g_FieldScripts + (((g_FieldScripts->entityCount * 8) + (g_CurrentEntity << 6)) + val)) + 0x20);
        g_FieldScriptPC[g_CurrentEntity] |= (short)(*(unsigned char *)(UNFOLDED(((g_FieldScripts->entityCount * 8) + 0x20)) + ((g_CurrentEntity << 6) + (int)g_FieldScripts) + val + 1) << 8);
        while ((g_FieldScriptOpcode = SCRIPT_PARAM_U8(0)) != 0) {
            g_FieldScriptOpTable[g_FieldScriptOpcode]();
        }
        g_FieldScriptPC[g_CurrentEntity]++;
    }
    g_CurrentEntity = 0;
}

/*
 * Marks valid party models retained via character/entity mappings, then scans model records marked
 * zero and clears every entity-model mapping equal to model index. Entity flags are written using
 * model index s rather than matching entity r. Later scan does not guard model pointer null; no
 * retention marks cleared here.
 */
// FUNCTION: FF7 0x0060c7cb
void __cdecl Field_UnmapUnretainedEntityModels(void)
{
    short partyChar;
    short modelIdx;
    short entityIndex;
    short i;

    for (i = 0; i < 3; i++) {
        partyChar = gSavemap.fieldParty[i];
        if (partyChar != PARTY_CHAR_NONE) {
            entityIndex = g_FieldCharEntity.charEntity[partyChar];
            if (entityIndex != 0xFF) {
                modelIdx = g_FieldEntityModel[entityIndex];
                if (modelIdx != 0xFF && modelIdx < g_FieldHumanAnimState.modelListCountCopy && g_pFieldModelSlots != 0) {
                    g_pFieldModelSlots[modelIdx].unmapFlag = 1;
                }
            }
        }
    }
    for (i = 0; i < g_FieldHumanAnimState.modelListCountCopy; i++) {
        if (*(unsigned char *)&g_pFieldModelSlots[i].unmapFlag == 0) {
            for (entityIndex = 0; entityIndex < g_FieldScripts->entityCount; entityIndex++) {
                if (g_FieldEntityModel[entityIndex] == i) {
                    g_FieldEntityModel[entityIndex] = 0xFF;
                    g_pFieldEntities[i].visible = 0;
                    g_pFieldEntities[i].solidOff = 1;
                    g_pFieldEntities[i].talkOff = 1;
                }
            }
        }
    }
}

/*
 * Publishes elapsed/countdown clock bytes, narrowing hours before capping them, and maintains
 * per-second subcounters. Dispatches and clears entity/line trigger flags, except one persistent
 * line flag (touch), then round-robins script entities with up to eight opcodes each and skips inactive
 * followers. A model's or line's talk request runs only while the field's characterLock is clear. Updates animation modes and clears a shared completion flag after optional
 * special-map handling. Zero entity count still enters the do-while loop.
 */
// FUNCTION: FF7 0x0060c94d
void __cdecl Script_ExecuteFrame(void)
{
    unsigned short sum;
    unsigned short secs;
    unsigned short mins;
    int cnt;
    int talkFlag;
    int i;
    int opcodeBudget;

    talkFlag = 0;
    sum = gSavemap.seconds / 0xE10;
    if (sum > 0xFF) {
        sum = 0xFF;
    }
    gSavemap.playClockHours = sum;
    sum = gSavemap.seconds % 0xE10;
    mins = sum / 0x3C;
    gSavemap.playClockMinutes = mins;
    secs = sum % 0x3C;
    if (gSavemap.playClockSeconds != secs) {
        gSavemap.playClockSeconds = secs;
        gSavemap.playClockFrames = 0;
    } else {
        gSavemap.playClockFrames += 1;
    }
    sum = (unsigned int)gSavemap.countdownSeconds / 0xE10;
    if (sum > 0xFF) {
        sum = 0xFF;
    }
    gSavemap.countdownClockHours = sum;
    sum = (unsigned int)gSavemap.countdownSeconds % 0xE10;
    mins = sum / 0x3C;
    gSavemap.countdownClockMinutes = mins;
    secs = sum % 0x3C;
    if (gSavemap.countdownClockSeconds != secs) {
        gSavemap.countdownClockSeconds = secs;
        gSavemap.countdownClockFrames = 0x1E;
    } else if (*(unsigned char *)&gSavemap.countdownClockFrames != 0) {
        gSavemap.countdownClockFrames -= 1;
    }
    cnt = g_FieldScripts->modelCount;
    for (i = 0; i < cnt; i++) {
        if (*(char *)&g_pFieldEntities[i].requestTalkScript != 0) {
            if (*(char *)&g_pFieldState->characterLock == 0 && talkFlag == 0) {
                Script_TriggerEntityScript(g_pFieldEntities[i].entityId, FIELD_SCRIPT_PRIORITY_INTERACTION, FIELD_SCRIPT_TALK);
                talkFlag = 1;
            }
            g_pFieldEntities[i].requestTalkScript = 0;
        }
        if (*(char *)&g_pFieldEntities[i].requestPushScript != 0) {
            Script_TriggerEntityScript(g_pFieldEntities[i].entityId, FIELD_SCRIPT_PRIORITY_INTERACTION, FIELD_SCRIPT_PUSH);
            g_pFieldEntities[i].requestPushScript = 0;
        }
    }
    for (i = 0; i < g_FieldLineCount; i++) {
        if (g_FieldLines[i].requestTalkScript != 0) {
            if (*(char *)&g_pFieldState->characterLock == 0) {
                Script_TriggerEntityScript(*(char *)&g_FieldLines[i].entityId, FIELD_SCRIPT_PRIORITY_INTERACTION, FIELD_SCRIPT_TALK);
            }
            g_FieldLines[i].requestTalkScript = 0;
        }
        if (g_FieldLines[i].requestPushScript != 0) {
            Script_TriggerEntityScript(*(char *)&g_FieldLines[i].entityId, FIELD_SCRIPT_PRIORITY_INTERACTION, FIELD_SCRIPT_PUSH);
            g_FieldLines[i].requestPushScript = 0;
        }
        if (g_FieldLines[i].across != 0) {
            Script_TriggerEntityScript(*(char *)&g_FieldLines[i].entityId, FIELD_SCRIPT_PRIORITY_INTERACTION, FIELD_SCRIPT_ACROSS);
            g_FieldLines[i].across = 0;
        }
        if (g_FieldLines[i].requestTouchOnScript != 0) {
            Script_TriggerEntityScript(*(char *)&g_FieldLines[i].entityId, FIELD_SCRIPT_PRIORITY_INTERACTION, FIELD_SCRIPT_TOUCH_ON);
            g_FieldLines[i].requestTouchOnScript = 0;
        }
        if (g_FieldLines[i].requestTouchOffScript != 0) {
            Script_TriggerEntityScript(*(char *)&g_FieldLines[i].entityId, FIELD_SCRIPT_PRIORITY_INTERACTION, FIELD_SCRIPT_TOUCH_OFF);
            g_FieldLines[i].requestTouchOffScript = 0;
        }
        if (g_FieldLines[i].touch != 0) {
            Script_TriggerEntityScript(*(char *)&g_FieldLines[i].entityId, FIELD_SCRIPT_PRIORITY_INTERACTION, FIELD_SCRIPT_TOUCH);
        }
    }
    cnt = g_FieldScripts->entityCount;
    do {
        if (g_CurrentEntity >= g_FieldScripts->entityCount) {
            g_CurrentEntity = 0;
        }
        if (g_FieldFollowerState[g_CurrentEntity] != 0 && g_FieldDialogTextState[0] != g_CurrentEntity) {
            goto nextEntity;
        }
        opcodeBudget = 8;
        do {
            g_FieldScriptOpcode = SCRIPT_PARAM_U8(0) & 0xFF;
            if (g_FieldScriptOpTable[g_FieldScriptOpcode]() != 0) {
                goto nextEntity;
            }
            opcodeBudget -= 1;
        } while (opcodeBudget != 0);
    nextEntity:
        g_CurrentEntity += 1;
        cnt -= 1;
    } while (cnt != 0);
    Field_UpdateModelAnimStates();
    if (g_FieldDisplayModeDirty != 0) {
        /* On map 0xA9 (169) a display-mode change (Field_SetDisplayMode) also releases the
           character lock; which map that is, the code does not say. */
        if (g_FieldCurrentMapId == 0xA9) {
            g_FieldActorState.state.characterLock = 0;
        }
    }
    g_FieldDisplayModeDirty = 0;
    return;
}

/*
 * Visits mapped field models except the selected dialogue-held model when unlocked, and steps each
 * by its g_FieldModelAnimMode: IDLE restores the idle animation and speed and loops it (the frame
 * counter has four fraction bits), LOOP loops the current animation, PLAY_AND_WAIT holds the last
 * frame and moves to ENDED, HOLD and ENDED keep the last frame, and PLAY_THEN_IDLE/PLAY_THEN_HOLD
 * fall back to IDLE/HOLD when the animation ends. Refreshes the last frame from the model
 * animation when available.
 */
// FUNCTION: FF7 0x0060ce26
void __cdecl Field_UpdateModelAnimStates(void)
{
    int dummy;
    int entityIndex;

    dummy = 0;
    for (entityIndex = 0; entityIndex < g_FieldScripts->entityCount; entityIndex++) {
        if (g_FieldEntityModel[entityIndex] != 0xFF && (g_pFieldState->playerModel != g_FieldEntityModel[entityIndex] || *(char *)&g_pFieldState->characterLock != 0)) {
            switch (g_FieldModelAnimMode[g_FieldEntityModel[entityIndex]]) {
            case FIELD_ANIM_MODE_IDLE:
                if (*(char *)&g_pFieldEntities[g_FieldEntityModel[entityIndex]].activeAnimId != g_FieldModelIdleAnimId[g_FieldEntityModel[entityIndex]]) {
                    g_pFieldEntities[g_FieldEntityModel[entityIndex]].activeAnimId = *(char *)&g_FieldModelIdleAnimId[g_FieldEntityModel[entityIndex]];
                    g_pFieldEntities[g_FieldEntityModel[entityIndex]].animSpeed = g_FieldModelIdleAnimSpeed[g_FieldEntityModel[entityIndex]];
                    g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame = 0;
                    if (g_pFieldModelSlots != 0) {
                        Field_SetModelAnimation((FieldModel *)&g_pFieldModelSlots[g_FieldEntityModel[entityIndex]], *(char *)&g_pFieldEntities[g_FieldEntityModel[entityIndex]].activeAnimId);
                        g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame = g_pFieldModelSlots[g_FieldEntityModel[entityIndex]].model->lastFrame - 1;
                    }
                } else {
                    if (g_pFieldModelSlots != 0) {
                        g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame = g_pFieldModelSlots[g_FieldEntityModel[entityIndex]].model->lastFrame - 1;
                    }
                    if (g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame >> 4 >= g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame) {
                        g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame = 0;
                    }
                }
                break;
            case FIELD_ANIM_MODE_LOOP:
                if (g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame >> 4 >= g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame) {
                    g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame = 0;
                }
                break;
            case FIELD_ANIM_MODE_PLAY_AND_WAIT:
                if (g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame >> 4 >= g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame) {
                    g_FieldModelAnimMode[g_FieldEntityModel[entityIndex]] = FIELD_ANIM_MODE_ENDED;
                    g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame = g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame << 4;
                }
                break;
            case FIELD_ANIM_MODE_HOLD:
            case FIELD_ANIM_MODE_ENDED:
                g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame = g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame << 4;
                break;
            case FIELD_ANIM_MODE_PLAY_THEN_IDLE:
                if (g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame >> 4 >= g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame) {
                    g_FieldModelAnimMode[g_FieldEntityModel[entityIndex]] = FIELD_ANIM_MODE_IDLE;
                }
                break;
            case FIELD_ANIM_MODE_PLAY_THEN_HOLD:
                if (g_pFieldEntities[g_FieldEntityModel[entityIndex]].animCurrentFrame >> 4 >= g_pFieldEntities[g_FieldEntityModel[entityIndex]].animLastFrame) {
                    g_FieldModelAnimMode[g_FieldEntityModel[entityIndex]] = FIELD_ANIM_MODE_HOLD;
                }
            }
        }
    }
}

/*
 * Asks entity `entity` to run script `script` at priority level `priority`. Returns 0 when the
 * entity already runs at that level or a more urgent one (a lower number). When the level is free,
 * it reads the script's start offset from the field script header (entity table at
 * 0x20 + entityCount * 8 + akaoCount * 4, 64 bytes per entity, read as two unaligned bytes). A
 * nonzero first opcode saves the current PC for the old level, switches the entity to the new
 * script and level, stops a scripted move (and a walk's animation) and clears the entity's wait counter. Either
 * way it returns 1. The script/level byte goes to g_FieldEntityScriptTrack (eight bytes per entity).
 * The retail frame reserves 128 unreferenced bytes between the offset and the PC (`q`); locals
 * retain the short names whose VC5 /Od slot hash reproduces the binary stack layout.
 */
// FUNCTION: FF7 0x0060d29b
int __cdecl Script_TriggerEntityScript(short entity, short priority, short script)
{
    short akaoOffset;
    char q[128]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    unsigned short scriptOffset;
    unsigned char opcode;

    if (g_FieldScriptPriority[entity] > priority) {
        if (g_FieldScriptPriorityState[entity][priority] == FIELD_SCRIPT_REQUEST_STATE_IDLE) {
            akaoOffset = g_FieldScripts->akaoBlockCount << 2;
            scriptOffset = FIELD_SCRIPT_ENTRY_LO(entity, script, akaoOffset);
            scriptOffset |= FIELD_SCRIPT_ENTRY_HI(entity, script, akaoOffset) << 8;
            opcode = *((unsigned char *)g_FieldScripts + scriptOffset) & 0xFF;
            if (opcode != 0) {
                g_FieldEntityScriptTrack[entity * 8 + priority] = (unsigned char)script;
                g_FieldScriptPriorityPC[entity][g_FieldScriptPriority[entity]] = g_FieldScriptPC[entity];
                g_FieldScriptPC[entity] = scriptOffset;
                g_FieldScriptPriority[entity] = (unsigned char)priority;
                if (g_FieldEntityModel[entity] != 0xFF) {
                    if (*(char *)&g_pFieldEntities[g_FieldEntityModel[entity]].scriptedMoveMode == FIELD_SCRIPTED_MOVE_WALK) {
                        g_pFieldEntities[g_FieldEntityModel[entity]].activeAnimId = 0;
                        g_pFieldEntities[g_FieldEntityModel[entity]].animCurrentFrame = 0;
                        g_pFieldEntities[g_FieldEntityModel[entity]].animLastFrame = 0;
                    }
                    g_pFieldEntities[g_FieldEntityModel[entity]].scriptedMoveMode = FIELD_SCRIPTED_MOVE_NONE;
                }
                g_FieldScriptWait[entity] = 0;
                return 1;
            }
            return 1;
        }
    } else {
        return 0;
    }
    return 1;
}

/*
 * Sets g_FieldHandCursorX and g_FieldHandCursorY to 0x7FFF, clears the byte at
 * g_FieldHandCursorState, and passes the context and the load options to
 * Field_LoadHandCursorImage, which loads the hand cursor picture.
 */
// FUNCTION: FF7 0x0060d4c5
void __cdecl Field_InitHandCursor(struct GfxEngineContext *context, struct PfileLoadOptions *opts)
{
    g_FieldHandCursorX = 0x7FFF;
    g_FieldHandCursorY = 0x7FFF;
    g_FieldHandCursorState = 0;
    Field_LoadHandCursorImage(context, opts);
}

/*
 * Toggles bit 0 of gSavemap.fieldHelp when newActiveKeys2 has bit 0x100 set. Draws the hand cursor
 * when fieldHelp equals one and characterLock is zero, or when fieldHelp bit 1 is set.
 */
// FUNCTION: FF7 0x0060d4f3
void __cdecl Field_UpdateHandCursor(void)
{
    if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_SELECT) != 0) {
        gSavemap.fieldHelp ^= 1;
    }
    if ((gSavemap.fieldHelp == 1 && *(char *)&g_pFieldState->characterLock == 0) ||
        (gSavemap.fieldHelp & 2) != 0) {
        Field_DrawHandCursor();
    }
}

/*
 * Toggles bit 0 of gSavemap.fieldHelp when newActiveKeys2 has bit 0x100 set.
 */
// FUNCTION: FF7 0x0060d549
void __cdecl Field_ToggleHandCursorBlink(void)
{
    if ((*(int *)&g_pFieldState->newActiveKeys2 & INPUT_SELECT) != 0) {
        gSavemap.fieldHelp ^= 1;
    }
}

/*
 * Skips cursor drawing while a movie is requested, both coordinates equal 32767, or either is
 * below -100. Otherwise clamps the global coordinates against configured horizontal and vertical
 * bounds, chooses flipped texture ranges and vertex positions according to two thresholds, toggles
 * a global byte, initializes the draw object and constant, invokes the quad helper through &z, and
 * marks its draw flag. The helper receives separately declared locals rather than an explicit full
 * descriptor; the intended vertex/metadata stack layout needs native confirmation.
 * The locals `x` and `y` hold the other axis (`y` is computed from g_FieldHandCursorX), but they
 * keep their names: VC5 lays out this frame by the locals' names, and swapping them changes it.
 */
// FUNCTION: FF7 0x0060d572
void __cdecl Field_DrawHandCursor(void)
{
    FieldQuadRecord imageBuffer;

    if (*(char *)&g_FieldMovieRequested != 0) {
        return;
    }
    if (g_FieldHandCursorX == 0x7FFF && g_FieldHandCursorY == 0x7FFF) { /* empty first arm: lib/codegen.h */
    } else if (g_FieldHandCursorX < -0x64 || g_FieldHandCursorY < -0x64) { /* empty first arm: lib/codegen.h */
    } else {
        if (g_FieldHandCursorX > g_FieldDisplayScaleFactor * 0x140 + g_FieldDisplayOffsetX) {
            g_FieldHandCursorX = g_FieldDisplayScaleFactor * 0x140 + g_FieldDisplayOffsetX;
        }
        if (g_FieldHandCursorX < g_FieldDisplayOffsetX) {
            g_FieldHandCursorX = *(short *)&g_FieldDisplayOffsetX;
        }
        if (g_FieldHandCursorY > g_FieldDisplayScaleFactor * 0xE0 + g_FieldDisplayOffsetX) {
            g_FieldHandCursorY = g_FieldDisplayScaleFactor * 0xE0 + g_FieldDisplayOffsetX;
        }
        if (g_FieldHandCursorY < g_FieldDisplayOffsetX - 0x20) {
            g_FieldHandCursorY = g_FieldDisplayOffsetX - 0x20;
        }
        if (g_FieldHandCursorX > g_FieldDisplayClipWidth) {
            imageBuffer.vertex0TuFloat = g_FieldCursorScaleX + g_FieldDisplayScaleY;
            imageBuffer.vertex1TuFloat = imageBuffer.vertex0TuFloat;
            imageBuffer.vertex2TuFloat = g_FieldCursorScaleX;
            imageBuffer.vertex3TuFloat = imageBuffer.vertex2TuFloat;
            imageBuffer.vertex0XFloat = (float)g_FieldHandCursorX - g_FieldDisplayCenterXFloat;
            imageBuffer.vertex1XFloat = imageBuffer.vertex0XFloat;
            imageBuffer.vertex2XFloat = imageBuffer.vertex0XFloat + g_FieldDisplayCenterXFloat;
            imageBuffer.vertex3XFloat = imageBuffer.vertex2XFloat;
        } else {
            imageBuffer.vertex0TuFloat = g_FieldCursorScaleX;
            imageBuffer.vertex1TuFloat = imageBuffer.vertex0TuFloat;
            imageBuffer.vertex2TuFloat = g_FieldCursorScaleX + g_FieldDisplayScaleY;
            imageBuffer.vertex3TuFloat = imageBuffer.vertex2TuFloat;
            imageBuffer.vertex0XFloat = (float)g_FieldHandCursorX;
            imageBuffer.vertex1XFloat = imageBuffer.vertex0XFloat;
            imageBuffer.vertex2XFloat = imageBuffer.vertex0XFloat + g_FieldDisplayCenterXFloat;
            imageBuffer.vertex3XFloat = imageBuffer.vertex2XFloat;
        }
        if (g_FieldHandCursorY < g_FieldDisplayClipHeight) {
            imageBuffer.vertex0TvFloat = g_FieldCursorScaleY + g_FieldDisplayAspectScale;
            imageBuffer.vertex1TvFloat = g_FieldCursorScaleY;
            imageBuffer.vertex2TvFloat = imageBuffer.vertex0TvFloat;
            imageBuffer.vertex3TvFloat = imageBuffer.vertex1TvFloat;
            imageBuffer.vertex1YFloat = (imageBuffer.vertex0YFloat = (float)g_FieldHandCursorY + g_FieldDisplayScaleX) + g_FieldDisplayCenterXFloat;
            imageBuffer.vertex2YFloat = imageBuffer.vertex0YFloat;
            imageBuffer.vertex3YFloat = imageBuffer.vertex1YFloat;
        } else {
            imageBuffer.vertex0TvFloat = g_FieldCursorScaleY;
            imageBuffer.vertex1TvFloat = g_FieldCursorScaleY + g_FieldDisplayAspectScale;
            imageBuffer.vertex2TvFloat = imageBuffer.vertex0TvFloat;
            imageBuffer.vertex3TvFloat = imageBuffer.vertex1TvFloat;
            imageBuffer.vertex1YFloat = (imageBuffer.vertex0YFloat = (float)g_FieldHandCursorY - g_FieldDisplayCenterXFloat + g_FieldDisplayScaleX) + g_FieldDisplayCenterXFloat;
            imageBuffer.vertex2YFloat = imageBuffer.vertex0YFloat;
            imageBuffer.vertex3YFloat = imageBuffer.vertex1YFloat;
        }
        g_FieldHandCursorState ^= 1;
        imageBuffer.batchHandle = (GfxRenderObject *)(g_pFieldHandImageBuffer);
        imageBuffer.quadZ = 0x3727C5AC;
        imageBuffer.flagsWord = 1;
        Field_EmitQuadFromRecord(&imageBuffer);
        g_FieldHandImageType = 1;
    }
}
} /* extern "C" */

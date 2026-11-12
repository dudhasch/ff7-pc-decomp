/* The savemap: `gSavemap`, 0x10F4 bytes at 0x00DBFD38 (defined in menu_main_and_battle_results.cpp),
 * the size of one PSX save block.
 *
 * The PC savemap is laid out like the PlayStation one: every PlayStation savemap offset maps to
 * a PC address by adding 0x00DBFD38. Ten points spanning 0xBAA bytes of the record agree (the
 * PlayStation names follow its decompilation; see Credits in the README):
 *
 *     PC address    subtract base   PlayStation field
 *     0x00DC0234    0x04FC          inventory[]         at 0x4FC
 *     0x00DC08B4    0x0B7C          gil                 at 0xB7C
 *     0x00DC08B8    0x0B80          time                at 0xB80
 *     0x00DC0DDC    0x10A4          phs_locking_mask    at 0x10A4
 *     0x00DC0DDE    0x10A6          phs_visibility_mask at 0x10A6
 *     0x00DC08BC    0x0B84          countdown_timer_... at 0xB84
 *     0x00DC0BDC    0x0EA4          memory_bank_4       at 0xEA4
 *     0x00DC09E5    0x0CAD          memory_bank_2[9]    at 0xCA4+9
 *     0x00DBFDAB    0x0073          party[0].status_flags at 0x54+0x1F
 *     0x00DC08DC    0x0BA4          memory_bank_1       at 0xBA4,
 *                                   and the four after it
 *
 * The last covers a whole region: the two indexed bank opcodes at 0x00610AFE and 0x00610C63
 * clamp their offset to 0x500 before touching 0x00DC08DC, and 0x500 is memory_bank_1 through
 * memory_bank_5, 0xBA4 to 0x10A3, ending where phs_locking_mask begins. Script_OpMemberLock
 * (0x0061C907) is the PlayStation MMBLK handler, `phs_locking_mask |= 1 << member`.
 *
 * The PlayStation layout gives a hypothesis for a field, not a name for the source: `struct Savemap` below has a
 * field only where the PC code reads one, with the offset and width of those reads. Bytes nothing
 * reads are pads. The field-script banks from +0xBA4 are fields where code reads a byte by name,
 * in a union with `scriptBanks[0x500]`, which the script VM's indexed reads use
 * (`(char *)&gSavemap.scriptBanks + index`).
 */

#ifndef FF7_SAVEMAP_H
#define FF7_SAVEMAP_H

/* Subtract from a PC address to get a PSX game.h offset, or add to go the
 * other way: the address of gSavemap. */
#define FF7_SAVEMAP_BASE 0x00DBFD38

/* One character's record in the savemap: 0x84 bytes, nine of them from +0x54
 * (`gSavemap.chars[i]`). Every field is read by the PC field, battle, and menu subsystems. */
typedef struct SavemapCharacter {
    unsigned char id;             /* +0x00 */
    unsigned char level;          /* +0x01 */
    unsigned char strength;       /* +0x02 */
    unsigned char vitality;       /* +0x03 */
    unsigned char magic;          /* +0x04 */
    unsigned char spirit;         /* +0x05 */
    unsigned char dexterity;      /* +0x06 */
    unsigned char luck;           /* +0x07 */
    unsigned char strengthBonus;  /* +0x08 */
    unsigned char vitalityBonus;  /* +0x09 */
    unsigned char magicBonus;     /* +0x0A */
    unsigned char spiritBonus;    /* +0x0B */
    unsigned char dexterityBonus; /* +0x0C */
    unsigned char luckBonus;      /* +0x0D */
    unsigned char limitLevel;     /* +0x0E */
    unsigned char limitBar;       /* +0x0F */
    unsigned char name[12];       /* +0x10 */
    unsigned char weapon;         /* +0x1C */
    unsigned char armor;          /* +0x1D */
    unsigned char accessory;      /* +0x1E */
    unsigned char status;         /* +0x1F */
    unsigned char row;            /* +0x20 */
    char levelProgress;           /* +0x21 */
    unsigned short limits;        /* +0x22: one bit per learned limit */
    unsigned short killCount;     /* +0x24 */
    unsigned short limitUses[3];  /* +0x26: uses of the limit at each limit level 1..3 */
    unsigned short hp;            /* +0x2C */
    unsigned short baseHp;        /* +0x2E: what hp is reset to when the character joins */
    short mp;                     /* +0x30 */
    unsigned short baseMp;        /* +0x32: what mp is reset to when the character joins */
    unsigned char pad_34[0x4];
    unsigned short maxHp;         /* +0x38 */
    unsigned short maxMp;         /* +0x3A */
    int exp;                      /* +0x3C */
    int weaponMateria[8];         /* +0x40 */
    int armorMateria[8];          /* +0x60 */
    int expToNext;                /* +0x80 */
} SavemapCharacter;

/* hub Savemap begin */
/* The savemap at 0x00DBFD38: 0x10F4 bytes, the PC object's extent (data layout) and the size of one PSX save block. Fields are the offsets the PC code reads. */
#pragma pack(push, 1)
struct Savemap {
    int checksum; /* +0x0 */
    char descLevel; /* +0x4 */
    unsigned char descParty[0x3]; /* +0x5 */
    unsigned char descName[0x10]; /* +0x8 */
    short descHp; /* +0x18 */
    short descMaxHp; /* +0x1a */
    short descMp; /* +0x1c */
    short descMaxMp; /* +0x1e */
    int descGil; /* +0x20 */
    int descTime; /* +0x24 */
    unsigned char descLocation[0x20]; /* +0x28 */
    unsigned char windowColors[0xc]; /* +0x48 */
    SavemapCharacter chars[9]; /* +0x54 */
    unsigned char party[0x3]; /* +0x4f8 */
    unsigned char pad_4fb[0x1];
    unsigned short items[0x140]; /* +0x4fc */
    int materia[0xc8]; /* +0x77c */
    int stolenMateria[0x30]; /* +0xa9c */
    unsigned char pad_b5c[0x20];
    int gil; /* +0xb7c */
    unsigned int seconds; /* +0xb80 */
    int countdownSeconds; /* +0xb84 */
    int millisecondCounter; /* +0xb88 */
    int clockAccumulator; /* +0xb8c */
    int fieldReturnMode; /* +0xb90 */
    short gameMode; /* +0xb94: the game mode a load enters (GAME_MODE_FIELD or GAME_MODE_WORLD_MAP) */
    short locationId; /* +0xb96 */
    unsigned char pad_b98[0x2];
    short coordX; /* +0xb9a */
    short coordY; /* +0xb9c */
    short coordT; /* +0xb9e */
    char coordD; /* +0xba0 */
    char fieldRandomIndex1; /* +0xba1 */
    char fieldRandomIndex2; /* +0xba2 */
    unsigned char pad_ba3[0x1];
    union {
        /* +0xba4: the five 256-byte field-script banks, which the script VM indexes flat
         * (bank * 0x100 + offset; Script_ReadOperand8/16 and the writers). The fields
         * below are the bytes code reads by name. */
        unsigned char scriptBanks[0x500];
        struct {
            union {
                unsigned short storyProgress; /* +0xba4: the story progress word */
                struct {
                    unsigned char storyProgressLo; /* +0xba4: the story progress word, low byte */
                    unsigned char storyProgressHi; /* +0xba5 */
                };
            };
            unsigned char joinLevel; /* +0xba6: the level Kernel_InitJoiningCharacter gives a character
                                       whose kernel growth byte has bit 0 set */
            unsigned char pad_ba7[0xd];
            char playClockHours; /* +0xbb4: Script_ExecuteFrame publishes the play time (`seconds`)
                                    as hours (capped at 255), minutes, seconds and frames */
            char playClockMinutes; /* +0xbb5 */
            unsigned char playClockSeconds; /* +0xbb6 */
            char playClockFrames; /* +0xbb7: frames since playClockSeconds last changed */
            char countdownClockHours; /* +0xbb8: the same four for `countdownSeconds` */
            char countdownClockMinutes; /* +0xbb9 */
            unsigned char countdownClockSeconds; /* +0xbba */
            char countdownClockFrames; /* +0xbbb: 30 when countdownClockSeconds changes, then counts down */
            unsigned char battlesLo; /* +0xbbc: battles fought, low byte (main.cpp carries into battlesHi) */
            unsigned char battlesHi; /* +0xbbd */
            unsigned char escapesLo; /* +0xbbe: battles escaped, low byte (battle.cpp carries into escapesHi) */
            unsigned char escapesHi; /* +0xbbf */
            unsigned char menuVisible; /* +0xbc0 */
            unsigned char menuVisibleHigh; /* +0xbc1 */
            union {
                unsigned short menuLockedWord; /* +0xbc2: menuLocked and menuFlags as one word */
                struct {
                    unsigned char menuLocked; /* +0xbc2 */
                    char menuFlags; /* +0xbc3 */
                };
            };
            unsigned char pad_bc4[0x20];
            unsigned char keyItems[8]; /* +0xbe4: one bit per key item (Menu_BuildOwnedKeyItemList tests 0x40) */
            unsigned char pad_bec[0x3];
            unsigned char magicMenuFlags; /* +0xbef */
            unsigned char pad_bf0[0x13];
            unsigned char countdownFlags; /* +0xc03: bit 0 the countdown runs (World_StartCountdownTimer),
                                            bit 1 it counts up instead of down (the clock tick) */
            unsigned char pad_c04[0x10];
            unsigned char slotTotalByte0; /* +0xc14 */
            unsigned char slotTotalByte1; /* +0xc15 */
            unsigned char slotTotalByte2; /* +0xc16 */
            unsigned char slotTotalByte3; /* +0xc17 */
            unsigned char slotIndex; /* +0xc18 */
            unsigned char pad_c19[0x5];
            unsigned char tutSub; /* +0xc1e */
            unsigned char pad_c1f[0x2e];
            char condorOutcome; /* +0xc4d */
            char condorPlayerUnitCount; /* +0xc4e */
            char condorSpawnsLeft; /* +0xc4f */
            char condorFlag; /* +0xc50 */
            unsigned char pad_c51[0x2];
            char condorSpawnCount; /* +0xc53 */
            unsigned char pad_c54[0x3];
            unsigned char condorUnitChoiceTier; /* +0xc57 */
            unsigned char pad_c58[0x55];
            unsigned char fieldParty[0x3]; /* +0xcad */
            unsigned char pad_cb0[0x56];
            char coasterScoreLowByte; /* +0xd06 */
            char coasterScoreHighByte; /* +0xd07 */
            unsigned char pad_d08[0xf];
            unsigned char highwayGameMode; /* +0xd17 */
            unsigned short highwayHpFractions[5]; /* +0xd18: the first five characters' HP over 0xFFFF;
                                                    Mini_Mode6ExitStoreScores writes them from the highway's
                                                    g_HighwayHpBarTable, Savemap_ScaleFirstFiveHp
                                                    scales each HP by its fraction */
            unsigned char pad_d22[0x7];
            unsigned char yuffieForest; /* +0xd29 */
            unsigned char pad_d2a[0x3e];
            unsigned char levelSnapshot[8]; /* +0xd68: Savemap_SnapshotLevelsToBank copies each recruited
                                              character's level here, by g_MenuCharSlotIndexMapping slot */
            unsigned char pad_d70[0x2];
            char snowboardCourseResult; /* +0xd72 */
            unsigned char pad_d73[0x31];
            unsigned char chocoboName; /* +0xda4: the first of the six glyphs of the chocobo's name (name
                                         entry 9); the race copies +0xda4..+0xda7 and chocoboNameTail */
            unsigned char pad_da5[0x3];
            short chocoboNameTail; /* +0xda8: glyphs 4 and 5 */
            unsigned char playerRacerRow; /* +0xdaa: the player's racer is row 6 + this of the racer table;
                                            the name entry's 0xFF after the six name glyphs lands here */
            unsigned char chocoboCourse; /* +0xdab */
            unsigned char chocoboRaceNoBetting; /* +0xdac: the race skips the bet and result screens and
                                                  sets chocoboPlayerRides */
            unsigned char chocoboPlayerRides; /* +0xdad: the player rides racer 0 (g_ChocoboPlayerRides;
                                                observed in game) */
            unsigned char topSpeedLow; /* +0xdae, the player's racing chocobo */
            unsigned char topSpeedHigh; /* +0xdaf */
            unsigned char baseSpeedLow; /* +0xdb0 */
            unsigned char baseSpeedHigh; /* +0xdb1 */
            unsigned char raceAcceleration; /* +0xdb2, copied to ChocoboRacer::acceleration (observed in game) */
            unsigned char raceCooperation; /* +0xdb3, copied to ChocoboRacer::cooperation */
            unsigned char raceIntelligence; /* +0xdb4, copied to ChocoboRacer::intelligence */
            unsigned char raceChocoboColor; /* +0xdb5, enum ChocoboRaceColor; picks the racer's terrainPenaltyMask */
            unsigned char racePersonality; /* +0xdb6, enum ChocoboPersonality */
            unsigned char pad_db7[0x3];
            unsigned char racerEnabled; /* +0xdba */
            unsigned char raceClass; /* +0xdbb, 0..3: the bet screen shows C, B, A, S */
            unsigned char pad_dbc[0x1];
            unsigned char chocoboRacePlace; /* +0xdbd */
            unsigned char staminaLow; /* +0xdbe */
            unsigned char staminaHigh; /* +0xdbf */
            unsigned char chocoboBetPrize; /* +0xdc0: the prize of the won bet (the bet grid cell's value
                                              through g_ChocoboBetPrizeSaveIds) */
            unsigned char pad_dc1[0x4e];
            char currentScoreLow; /* +0xe0f */
            char currentScoreHigh; /* +0xe10 */
            unsigned char highScoreLow; /* +0xe11 */
            unsigned char highScoreHigh; /* +0xe12 */
            unsigned char snowUnused; /* +0xe13 */
            unsigned char snowBegFastTime[0xf]; /* +0xe14 */
            char snowTempVar; /* +0xe23 */
            unsigned char pad_e24[0x80];
            unsigned char discChangeRequest; /* +0xea4 */
            unsigned char pad_ea5[0x1];
            unsigned char apTotal[4]; /* +0xea6: the AP of every battle won, an unaligned int the battle end adds
                                         g_BattleRewardAp to byte by byte (Battle_WritePartyStateBackToSavemap) */
            char encounterStepCountLow; /* +0xeaa */
            unsigned char encounterStepCountHigh; /* +0xeab */
            unsigned char pad_eac[0x16];
            unsigned char fieldHelp; /* +0xec2 */
            unsigned char materiaResult; /* +0xec3 */
            unsigned char pad_ec4[0x3d];
            unsigned char submarineMissionId; /* +0xf01 */
            unsigned char submarineAudioStateLowByte; /* +0xf02 */
            unsigned char submarineAudioStateHighByte; /* +0xf03 */
            unsigned char saveCrystalFlags; /* +0xf04: bit 0 set by using the Save Crystal; with bit 1
                                              clear the item menu restricts it */
            unsigned char soundMuteFlags; /* +0xf05: bits 0x30 mute App_PlayCachedSfxUnlessMuted */
            unsigned char pad_f06[0x6];
            char location[0x18]; /* +0xf0c */
            unsigned char pad_f24[0x6];
            unsigned char worldMapVariantFlags; /* +0xf2a: from story progress 0x62C, bit 0x10 picks
                                                   the later pair of world maps
                                                   (World_SelectMapVariantFromStoryProgress) */
            unsigned char pad_f2b[0x2d];
            unsigned short worldLastEncounter; /* +0xf58 */
            unsigned short worldTerrainCountdown; /* +0xf5a */
            unsigned char lWorld[0x30]; /* +0xf5c */
            unsigned int polePositions[3]; /* +0xf8c: x in the low and z in the high half, of the poles that world models 0x15 to 0x17 stand for */
            int worldTerrainBytes; /* +0xf98 */
            unsigned short worldCameraState; /* +0xf9c */
            unsigned char pad_f9e[0x1];
            unsigned char worldSteeringSpawnIndex; /* +0xf9f */
            unsigned char worldChocoboEncounterInfo; /* +0xfa0 */
            unsigned char worldPlayerModelId; /* +0xfa1 */
            unsigned char worldMapVariant; /* +0xfa2 */
            unsigned char worldChecksum; /* +0xfa3 */
            unsigned char pad_fa4[0x6f];
            unsigned char materiaSetComplete; /* +0x1013: Savemap_CheckMateriaSetComplete's result */
            unsigned char pad_1014[0x13];
            unsigned char fieldLineDrawFlags; /* +0x1027: with bit 0x80 set Field_StepLineDrawRecords
                                                 ramps g_FieldLineDrawIntensity up, clear down; no other
                                                 bit is read */
            unsigned char pad_1028[0x7c];
        };
    };
    unsigned short phsLockingMask; /* +0x10a4 */
    unsigned short phsVisibilityMask; /* +0x10a6 */
    unsigned char pad_10a8[0x30];
    char battleSpeed; /* +0x10d8 */
    char battleMessageSpeed; /* +0x10d9 */
    unsigned short options; /* +0x10da */
    unsigned char controllerMap[0x10]; /* +0x10dc */
    unsigned char fieldMessageSpeed; /* +0x10ec */
    unsigned char pad_10ed[0x7];
};
#pragma pack(pop)
/* hub Savemap end */

#ifdef __cplusplus
extern "C" {
#endif
extern struct Savemap gSavemap;
#ifdef __cplusplus
}
#endif

/* The field-script banks read a 16-bit variable as two bytes: the low byte at
 * `(char *)&gSavemap.scriptBanks + i` and the high byte one on, `(char *)&gSavemapBankHi + i`, the
 * banks' second byte (+0xBA5). */
#define gSavemapBankHi (gSavemap.storyProgressHi)
#define g_SavemapScriptBanksHi (&gSavemap.storyProgressHi)

#endif /* FF7_SAVEMAP_H */

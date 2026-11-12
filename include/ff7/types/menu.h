/* include/ff7/types/menu.h: the types ff7/menu TUs share.
 * One definition per type. */
#ifndef FF7_TYPES_MENU_H
#define FF7_TYPES_MENU_H

/* Three rows of 25 input action bindings (300 bytes) copied between live, edit and saved buffers. */
typedef struct MenuInputBindingTable {
    int bindings[3][25];
} MenuInputBindingTable;

/* Window frame rectangle (x, y, width, height as shorts) for the 1x and 2x frame drawers. */
typedef struct EnglishWindowFrame {
    short x;
    short y;
    short width;
    short height;
} EnglishWindowFrame;

/* Popup message body offsets, added to the menu viewport origin. */
typedef struct MenuPopupBodyRect {
    short x;
    short y;
    short extentX;
    short extentY;
} MenuPopupBodyRect;

/* A gauge the menu draws (Menu_DrawResourceGauge2x/1x): a bar of `width` by `height` at (x, y),
 * filled to width * low / max, with a second segment to width * high / max when `direction` is
 * nonzero. Battle_StepPartyHpMpGaugeDisplay fills low/high/direction from the displayed and the
 * current value: direction 2 when the value falls, 1 when it rises, 0 when they agree (one
 * segment). The battle HUD and the result screens build one on the stack and pass its address. */
typedef struct MenuGauge {
    short x;
    short y;
    short width;
    short height;
    short low; /* +0x08 */
    short max; /* +0x0A */
    short direction; /* +0x0C */
    short high; /* +0x0E */
    unsigned int colour; /* +0x10 */
} MenuGauge;

/* Render-object handles for the font and window textures loaded by Battle_LoadFontAndWindowTexturesInto. */
typedef struct FontWindowHandles {
    int renderHandle0;
    int renderHandle1;
    int renderHandle2;
    int renderHandle3;
    int renderHandle4;
    int renderHandle5;
    int renderHandle6;
    int renderHandle7;
} FontWindowHandles;

/* Message window frame rectangle (x, y, width, height) drawn by Menu_DrawMessageWindowFrame2x. */
typedef struct EnglishMessageWindowRect {
    short posX;
    short posY;
    short width;
    short height;
} EnglishMessageWindowRect;

/* Equipment screen state: scroll-bar block, candidate count and list top. */
typedef struct MenuEquipScreenState {
    short visibleRows;
    short candidateCount;
    short listTop;
    short frameWidth;
    short frameHeight;
    short rowHeight;
    short listWidth;
} MenuEquipScreenState;

/* The materia menu's window frames (g_MenuMateriaWindowRects2x or 1x): 11 rectangles. [1] holds the
 * selected-name row, [5] the exchange list origin (x, y), [6] its anchor, [9] the stock list origin
 * and [10] (x only) the cursor base. */
typedef struct MenuMateriaWindow {
    EnglishWindowFrame frames[11];
} MenuMateriaWindow;

/* Packed 32-bit BGRA vertex color word with per-channel byte access. */
typedef union MenuColorWord {
    unsigned int word;
    unsigned char bytes[4];
} MenuColorWord;

/* One of the party stat block's eight reaction triggers (+0x24), which the materia kernel builds
 * from Counter, Magic Counter, Sneak Attack and Final Attack pairs and Battle_RollReactionTriggers
 * reads. `typeId` is mode * 3 + 1 + kind (BattleReactionType); `attackIndex` an attack, command or
 * BATTLE_REACTION_RANDOM_* id; `chance` the materia level's percentage (for Final Attack: the
 * charges left, one used per trigger). */
typedef struct BattleReactionTrigger {
    unsigned char typeId;
    unsigned char attackIndex;
    unsigned char chance;
} BattleReactionTrigger;

/* One of the sixteen battle menu commands (+0x4C, four per column). `cursorAction` is the KERNEL
 * command's first byte; `menuFlags` BattleMenuSlotFlag; `allCount` Mega All's level (or the
 * W-Magic/W-Summon level, 0xFF for the multi-hit commands); `supportFlags` BattleSupportFlag. */
typedef struct BattleCommandSlot {
    unsigned char commandId;
    unsigned char cursorAction;
    unsigned char targetFlags;
    unsigned char menuFlags;
    unsigned char allCount;
    unsigned char supportFlags;
} BattleCommandSlot;

/* One magic, summon or enemy skill entry (+0x108): the attack's id in its list (0xFF: empty), its MP
 * cost after MP Turbo, the All count (summons: casts per battle), the linked Quadra Magic count and
 * casts, the attack's target flags, BattleMenuSlotFlag, and `support`: BattleSupportFlag in the
 * low five bits and the MP Turbo level in bits 5-7. */
typedef struct BattleAbilitySlot {
    unsigned char attackId;
    unsigned char mpCost;
    unsigned char allCount;
    unsigned char quadraLinks;
    unsigned char quadraCount;
    unsigned char targetFlags;
    unsigned char menuFlags;
    unsigned char support;
} BattleAbilitySlot;

/* hub MenuPartyStats begin */
#pragma pack(push, 1)
struct MenuPartyStats {
    unsigned char statBlock[0x1]; /* +0x0 */
    unsigned char coverChance; /* +0x1 */
    /* +0x02..+0x0F: the battle stats (strength to luck with their bonuses, then attack, defense,
       magic attack and magic defense) */
    unsigned char strength; /* +0x2 */
    unsigned char vitality; /* +0x3 */
    unsigned char magic; /* +0x4 */
    unsigned char spirit; /* +0x5 */
    unsigned char dexterity; /* +0x6 */
    unsigned char luck; /* +0x7 */
    unsigned short attack; /* +0x8 */
    unsigned short defense; /* +0xa */
    unsigned short magicAttack; /* +0xc */
    unsigned short magicDefense; /* +0xe */
    short hp; /* +0x10 */
    short maxHp; /* +0x12 */
    short mp; /* +0x14 */
    short maxMp; /* +0x16 */
    short atbGauge; /* +0x18 */
    unsigned short limitGauge; /* +0x1a */
    short barrierGauge; /* +0x1c */
    short mBarrierGauge; /* +0x1e */
    unsigned char limitLevel; /* +0x20 */
    unsigned char commandColumns; /* +0x21, columns of four commands: (placed - 1) / 4 + 1 */
    char pad_22; /* +0x22 */
    unsigned char flags; /* +0x23 */
    BattleReactionTrigger reactions[8]; /* +0x24 */
    unsigned short attackElements; /* +0x3c */
    unsigned short halvedElements; /* +0x3e */
    unsigned short nullifiedElements; /* +0x40 */
    unsigned short absorbedElements; /* +0x42 */
    int attackStatuses; /* +0x44 */
    int defenseStatuses; /* +0x48 */
    BattleCommandSlot commands[16]; /* +0x4c */
    unsigned char limitAttacks[3]; /* +0xac: the limit attacks the battle menu lists */
    unsigned char pad_af[0x5];
    unsigned char limitRecords[3][28]; /* +0xb4: the three limit attacks' 28-byte KERNEL records */
    /* +0x108: 96 entries, 56 magic, 16 summons from +0x2C8 and 24 enemy skills from +0x348; the
       kernel's loops run over all of them as `abilities` (MP Turbo over magic and summons) */
    union {
        BattleAbilitySlot abilities[96];
        struct {
            BattleAbilitySlot spells[56];
            BattleAbilitySlot summons[16];
            BattleAbilitySlot enemySkills[24];
        };
    };
    /* +0x408: the equipped weapon's 0x2C-byte KERNEL record is copied here (to +0x434); the members
       from statusAttackBits to elementMask are its fields */
    unsigned char weapon[0x5];
    unsigned char statusAttackBits; /* +0x40d */
    char weaponGrowthRate; /* +0x40e */
    unsigned char pad_40f[0x1];
    unsigned char slotPenaltyBase; /* +0x410 */
    unsigned char paramIndex; /* +0x411, high nibble: the model variant battle loads */
    unsigned char pad_412[0x6];
    unsigned short elementMask; /* +0x418 */
    unsigned char pad_41a[0x2];
    unsigned char equipStatTarget[4]; /* +0x41c: the weapon's stat bonuses, Savemap_AddEquipStatBonus slots */
    unsigned char equipStatAmount[4]; /* +0x420 */
    char weaponMateriaSlots[8]; /* +0x424: the weapon's materia slot kinds, linked in pairs (0,1) (2,3) (4,5) (6,7) */
    unsigned char pad_42c[0xb];
    char encounterRateDown; /* +0x437: subtracted from the party's encounterRate */
    unsigned char pad_438[0x4];
    /* +0x43C..+0x43F: added to the party's PartyBattleModifiers */
    char gilScaleBonus; /* +0x43c */
    char encounterRateUp; /* +0x43d */
    char chocoboLureBonus; /* +0x43e */
    char preemptiveBonus; /* +0x43f */
};
#pragma pack(pop)
/* hub MenuPartyStats end */

/* hub KernelMateria begin */
#pragma pack(push, 1)
struct KernelMateria {
    union {
        unsigned char headerBytes[0x8]; /* +0x0 */
        unsigned short apLevels[4]; /* +0x0: the AP needed (in hundreds) to reach levels 2 to 5; 0xFFFF where the materia has no such level */
    };
    unsigned char statSelectorByte; /* +0x8 */
    unsigned char statusMaskLowByte; /* +0x9 */
    unsigned char statusMaskMidByte; /* +0xa */
    unsigned char statusMaskHighByte; /* +0xb */
    unsigned char elementId; /* +0xc */
    unsigned char typeNibbleByte; /* +0xd */
    union {
        unsigned char attributes[6]; /* +0xe: the materia's per-level parameters */
        struct {
            unsigned char primaryCommandByte; /* +0xe */
            char secondaryCommandByte; /* +0xf */
            unsigned char pad_10[0x4];
        };
    };
};
#pragma pack(pop)
/* hub KernelMateria end */

/* hub KernelArmor begin */
#pragma pack(push, 1)
struct KernelArmor {
    unsigned char headerBytes[0x2]; /* +0x0 */
    unsigned char physicalDefense; /* +0x2 */
    unsigned char magicDefense; /* +0x3 */
    unsigned char dfpo; /* +0x4 */
    unsigned char magicEvade; /* +0x5 */
    unsigned char pad_6[0x3];
    unsigned char materiaSlotBytes[0x9]; /* +0x9 */
    unsigned short equipMask; /* +0x12 */
    unsigned char pad_14[0xc];
    unsigned short flagWord; /* +0x20 */
    unsigned char pad_22[0x2];
};
#pragma pack(pop)
/* hub KernelArmor end */

/* hub KernelWeapon begin */
#pragma pack(push, 1)
struct KernelWeapon {
    char typeByte; /* +0x0 */
    unsigned char pad_1[0x3];
    unsigned char attackBonusByte; /* +0x4 */
    unsigned char pad_5[0x1];
    unsigned char materiaGrowth[0x2]; /* +0x6 */
    unsigned char hitRate; /* +0x8 */
    unsigned char pad_9[0x5];
    unsigned short equipMask; /* +0xe */
    unsigned char pad_10[0xc];
    unsigned char materiaSlotTable[0xb]; /* +0x1c */
    unsigned char secondaryDisplayValue; /* +0x27 */
    unsigned char pad_28[0x2];
    unsigned short flagWord; /* +0x2a */
};
#pragma pack(pop)
/* hub KernelWeapon end */

/* hub KernelAccessory begin */
/* One of the 32 accessory records of the kernel, 16 bytes. */
#pragma pack(push, 1)
struct KernelAccessory {
    unsigned char pad_0[0x5];
    char autoStatusKind; /* +0x5: the status the battle applies for as long as it is worn, BattleAccessoryAutoStatus */
    unsigned char pad_6[0x6];
    unsigned short equipMask; /* +0xc */
    unsigned short restrictions; /* +0xe: Kernel_BuildBattleItemList masks it with 0xB, as Kernel_GetEquipRestrictions' result */
};
#pragma pack(pop)
/* hub KernelAccessory end */

/* Battle menu magic-list window record, addressed by a gauge index with stride 0x700. */
typedef struct BattleMagicListWindow {
    int col;
    int row;
    unsigned char pad_08[0xc];
    int scrollTop;
    unsigned char pad_18[0x18];
    int scrollState;
} BattleMagicListWindow;

/* Armor record read by Savemap_BuildPartyStatsFromRecord for element, status-defend bit and element defense. */
typedef struct KernelArmorRecord {
    unsigned char pad_00[0x1];
    unsigned char element;
    unsigned char pad_02[0x4];
    unsigned char statusDefendBit;
    unsigned char pad_07[0xa];
    char growthRate;
    unsigned char pad_12[0x2];
    unsigned short elementDefense;
    unsigned char pad_16[0x2];
    unsigned char equipStatTarget[4]; /* +0x18: the armor's stat bonuses */
    unsigned char equipStatAmount[4]; /* +0x1c */
} KernelArmorRecord;

/* Accessory record read by Savemap_BuildPartyStatsFromRecord for element, element defense and status mask. */
typedef struct KernelAccessoryRecord {
    unsigned char equipStatTarget[2];
    unsigned char equipStatAmount[2];
    unsigned char element;
    unsigned char pad_05[0x1];
    unsigned short elementDefense;
    int statusDefendMask;
} KernelAccessoryRecord;

/* Per-party-member result screen slot: displayed level, bit indices and timers. */
typedef struct EnglishExpPartySlot {
    unsigned char displayedLevel;
    unsigned char materiaBitIndex;
    unsigned char timer;
    unsigned char countdown;
    unsigned char limitBitIndex;
    unsigned char holdFrames;
} EnglishExpPartySlot;

/* One ability row of the materia detail panel: the attack's id in its list (0xFF: empty) and whether it is usable. */
typedef struct MenuMateriaDetailEntry {
    unsigned char id;
    unsigned char enabled;
} MenuMateriaDetailEntry;

/* The detail Kernel_ApplyMateriaToActor builds for the selected materia (g_MenuMateriaBuffer, 0x80
 * bytes): its level and level count, the AP to the next level, the eight stat modifiers and the abilities listed. */
typedef struct MenuMateriaDetail {
    unsigned char level;
    unsigned char maxLevel;
    unsigned char pad_02[0x2];
    int nextLevelAp;
    unsigned char pad_08[0x2];
    short statModifier[8];
    MenuMateriaDetailEntry entries[51];
} MenuMateriaDetail;

/* The materia screen's sub-rectangles (g_MenuMateriaSubRects2x or 1x): the command list origin and
 * height (its width comes from g_MenuMateriaTextPtr), the list frame and the cost box size. */
typedef struct MenuMateriaSubRects {
    short listX;
    short listY;
    unsigned char pad_04[0x2];
    short listHeight;
    EnglishWindowFrame frame;
    unsigned char pad_10[0x4];
    short costWidth;
    short costHeight;
} MenuMateriaSubRects;

/* Save slot preview status of 3 bytes per slot, set by Menu_ScanSaveDirectory. */
typedef struct SaveSlotPreview {
    char status;
    unsigned char pad_01[0x2];
} SaveSlotPreview;

/* One 0x700-byte battle menu window-set record, indexed by the menu cursor entry. */
typedef struct BattleMenuWindowSet {
    int cursorIndex;
    unsigned char pad_04[0x6fc];
} BattleMenuWindowSet;

/* Maps a character id to its save-party slot index. */
typedef struct MenuCharSaveSlot {
    char saveSlot;
    unsigned char pad_01[0x3];
} MenuCharSaveSlot;

/* Battle menu window record (0x700 bytes); entryIndex is an index added to a frame entry. */
typedef struct BattleMenuWindow {
    int entryIndex;
    unsigned char pad_04[0x6fc];
} BattleMenuWindow;

/* Battle menu cursor record (0x700 bytes); row is a row, scaled to pixels for the cursor. */
typedef struct BattleMenuCursor {
    int row;
    unsigned char pad_04[0x6fc];
} BattleMenuCursor;

/* Layout table of the battle result gil and items screen, indexed by menu state. */
typedef struct BattleResultScreenLayout {
    short row0ArrowTop2x;
    unsigned short rowBase2x;
    short row5ArrowTop2x;
    unsigned char pad_06[0x2];
    unsigned char row0ArrowTop1x;
    unsigned char rowBase1x;
    unsigned char row5ArrowTop1x;
    unsigned char pad_0b[0x5];
} BattleResultScreenLayout;

/* A menu cursor block: the 14 dwords MenuCursor_Init stores, in its argument order (0x38 bytes).
 * scrollTop is the first list row on screen; scrollAnim counts a scroll animation back to 0. */
typedef struct MenuCursor {
    int col;
    int row;
    int colCount;
    int rowCount;
    int unused4;
    int scrollTop;
    int rowStep;
    int totalRows;
    int unused8;
    int scrollAnim;
    int colWrapMode; /* enum MenuCursorWrap */
    int rowWrapMode; /* enum MenuCursorWrap */
    int scrollState; /* enum MenuCursorScroll */
    int pageScrollEnable; /* L1/R1 move scrollTop a page */
} MenuCursor;

/* Per-party-actor 0x700-byte stride starting at MenuCursor.row (+0x04) for g_BtlMenuSlotTableA. */
typedef struct BtlMenuActorListCursorStride {
    int row;
    unsigned char pad_04[0xc];
    int scrollTop;
    unsigned char pad_14[0x6ec];
} BtlMenuActorListCursorStride;

/* Per-party-actor 0x700-byte stride for a single dword field inside the battle menu state. */
typedef struct BtlMenuActorFieldStride {
    int value;
    unsigned char pad_04[0x6fc];
} BtlMenuActorFieldStride;

/* Materia screen sub-cursor record used to size the stock list. */
typedef struct MenuMateriaSubCursor {
    int rowBonus;
    unsigned char pad_04[0x34];
} MenuMateriaSubCursor;

/* A name entry key: the grid cell a keyboard scan code selects, and which shift states accept it
 * (bit 0: unshifted, bit 1: shifted; zero: the key does nothing). */
typedef struct MenuNameEntryLayout {
    int gridCol;
    int gridRow;
    int shiftFlags;
} MenuNameEntryLayout;

/* Save slot preview record (0x54 bytes, the head of a save slot): what the load screen draws for
 * a slot. The window colours are four RGB corners in the savemap's order, top-left, bottom-left,
 * top-right, bottom-right. */
typedef struct SaveSlotPreviewInfo {
    unsigned char pad_00[0x4];
    unsigned char leaderLevel;
    unsigned char partyPortraits[3]; /* 0xFF: empty */
    unsigned char leaderName[0x10];
    unsigned char pad_18[0x8];
    int gil;
    int playtime; /* shown as hours and minutes */
    unsigned char location[0x20];
    unsigned char windowColors[12];
} SaveSlotPreviewInfo;

/* 0x440-byte (1088-byte) computed party-member stat block swapped when reordering party slots. */
typedef struct MenuPartyStatBlock {
    int w[272];
} MenuPartyStatBlock;

/* The main menu's three cursors (MenuCursor_Init), 0x38 bytes each: the command list, the party list and the
 * swap cursor; their row members are the selected command, party member and swap target. */
typedef struct MenuMainCursorSet {
    union {
        struct {
            MenuCursor command;
            MenuCursor party;
            MenuCursor swap;
        };
        MenuCursor list[3];
    };
} MenuMainCursorSet;

/* Menu main label table of encoded text strings drawn by the menu and battle result screens. */
typedef struct GfxMenuLabelTable {
    unsigned char pad_00[0x118];
    unsigned char timeText;
    unsigned char pad_119[0x13];
    unsigned char gilText;
    unsigned char pad_12D[0x13];
    unsigned char mainScreenText;
    unsigned char pad_141[0x13];
    unsigned char headerText;
    unsigned char pad_155[0x3b];
    unsigned char materiaLevelUpText;
    unsigned char pad_191[0x3b];
} GfxMenuLabelTable;

#pragma pack(push, 1)
typedef struct CaitReelSymbolState {
    unsigned char reelSymbol; /* +0x0 */
    unsigned char targetSymbol; /* +0x1 */
    unsigned char lastSymbol; /* +0x2 */
    unsigned char caitReelByte3; /* +0x3 */
} CaitReelSymbolState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BtlGaugeReelState {
    unsigned char reelCounter; /* +0x0 */
    unsigned char countdown; /* +0x1 */
    unsigned char pad_2[0x2];
} BtlGaugeReelState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct InputScriptButtonState {
    unsigned char buttonHighByte; /* +0x0 */
    unsigned char buttonLowByte;  /* +0x1 */
    unsigned char pad_2[0x2];
} InputScriptButtonState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MenuFontBGraphicsObject {
    struct GfxRenderObject *batchList; /* +0x0 */
    struct GfxRenderObject *portraitRenderObjects[3]; /* +0x4: the three party portrait render objects, loaded by Menu_LoadCharPortrait */
} MenuFontBGraphicsObject;
#pragma pack(pop)

/* Pair slices of the 7-short Menu_DrawScrollBar2x/1x descriptor when each dword has its own GLOBAL marker. */
#pragma pack(push, 1)
typedef struct MenuScrollBarRows {
    short visibleRows;
    short totalRows;
} MenuScrollBarRows;

typedef struct MenuScrollBarPos {
    short scrollTop;
    short x;
} MenuScrollBarPos;

typedef struct MenuScrollBarSize {
    short y;
    short width;
} MenuScrollBarSize;
#pragma pack(pop)

/* g_MenuMateriaCursorBlock (src/ff7/src/menu/english/menu_materia_screen.cpp): the first 0x30 bytes
 * of a MenuCursor; the rest is g_MenuMateriaSubCursorBlock. */
#pragma pack(push, 1)
typedef struct MenuMateriaCursorBlock {
    int col;
    int row;
    unsigned char pad_8[0xc];
    int scrollTop;
    unsigned char pad_18[0xc];
    int scrollAnim;
    unsigned char pad_28[0x8];
} MenuMateriaCursorBlock;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MenuShopScrollBar {
    short visibleRows; /* the rows the list shows at once */
    short totalRows;
    short scrollTop; /* the first row shown */
    short x;
    short y;
    short width;
    short height;
    unsigned char pad_e[0x2];
} MenuShopScrollBar;
#pragma pack(pop)

/* The party's battle modifiers, g_PartyBattleModifiers (0x00DBCAD8, defined in
 * menu_main_and_battle_results.cpp), from the members' materia. Party_RecomputeBattleModifiers resets
 * them to 0x10, 0x10, 0 and 0x10, adds each present member's bonus bytes (MenuPartyStats
 * +0x43C..+0x43F; the encounter rate also loses encounterRateDown, with a floor of 2) and clamps
 * them to 0x20, 0x20, 0x20 and 0x55.
 * - gilScale: the battle's gil is multiplied by it in 16ths (Battle_RunEndSequence; 0 leaves it).
 * - encounterRate: the field's and the world map's encounter rate, in 16ths.
 * - chocoboLure: nonzero lets the world map draw a chocobo formation; the roll is divided by it.
 * - preemptiveOdds: its low 7 bits are the odds of a pre-emptive battle out of 256;
 *   PARTY_PREEMPTIVE_MASTERED (0x80, set when Kernel_ApplyMateriaEncounterModifier saw materia 0x0A at
 *   level 5) halves the odds of field special formations 0, 1 and 3 and of the world map's rows
 *   7, 8 and 10. */
#pragma pack(push, 1)
typedef struct PartyBattleModifiers {
    unsigned char gilScale; /* +0x0 */
    unsigned char encounterRate; /* +0x1 */
    unsigned char chocoboLure; /* +0x2 */
    unsigned char preemptiveOdds; /* +0x3 */
} PartyBattleModifiers;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BtlLimitMenuTables {
    unsigned char pad_0[0xf];
    unsigned char limitRowYTableLarge[4]; /* +0xf: the y of each limit row at 2x */
    unsigned char limitLevelAtlasTable[4]; /* +0x13: the atlas u of each limit level's icon */
    unsigned char limitRowYTableSmall[4]; /* +0x17: the y of each limit row at 1x */
    unsigned char pad_1b[0x1];
} BtlLimitMenuTables;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MenuStubScreen6WindowRects {
    unsigned char pad_0[0xa];
    short window1Y; /* +0xa */
} MenuStubScreen6WindowRects;
#pragma pack(pop)

/* A shop's entry: what it sells, an item (MENU_SHOP_ENTRY_ITEM) or a materia (MENU_SHOP_ENTRY_MATERIA). */
typedef struct MenuShopEntry {
    short kind; /* +0x0: enum MenuShopEntryKind; an item id has weapons from 0x80, armour 0x100, accessories 0x120 */
    unsigned short pad_02; /* +0x2: 0 in every entry */
    int id; /* +0x4 */
} MenuShopEntry;

/* One shop (0x54 bytes), the one Menu_ShopSelectShop selects through g_MenuShopIndex. */
typedef struct MenuShopRecord {
    char titleIndex; /* +0x00: the index into g_MenuCaptions of the centred title */
    char textSet; /* +0x01: the 460-byte block of shopkeeper lines in g_MenuCaptions */
    short count; /* +0x02: the entries on sale */
    MenuShopEntry entries[10]; /* +0x04: the entries past count are 0 */
} MenuShopRecord;
typedef char menu_shop_record_size[(sizeof(MenuShopRecord) == 0x54) ? 1 : -1];

#endif /* FF7_TYPES_MENU_H */

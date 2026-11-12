/* include/ff7/types/battle_actors.h: Battle actor, HUD, menu, camera, AI and core runtime record types. */
#ifndef FF7_TYPES_BATTLE_ACTORS_H
#define FF7_TYPES_BATTLE_ACTORS_H

#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/savemap.h"

/* --- amptoanm --- */
/* One 0x28-byte entry of a decoded animation frame: per axis the bit-read delta and the 0..0xFFF
 * angle derived from it. Entry 0 also holds the frame's three accumulated 16-bit root channels
 * (accumA..C) and their float copies (decodedA..C). */
typedef struct Battle3dAnimAngles {
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_06[0x2];
    short accumA;
    short accumB;
    short accumC;
    unsigned char pad_0E[0x2];
    int angleX;
    int angleY;
    int angleZ;
    float decodedA;
    float decodedB;
    float decodedC;
} Battle3dAnimAngles;

/* --- battle_action_queue_run --- */
/* State words of the battle action queue runner, Battle_RunActionQueue. */
typedef struct BattleActionQueueState {
    int executeStage; /* BattleActionExecStage of Battle_ExecuteAction */
    int state;
    int busy;
    int phase;
    int subPhase;
} BattleActionQueueState;

/* Battle action queue state: active flag and current priority level. */
typedef struct BattleActionQueue {
    int actionDone; /* Battle_ExecuteAction sets it once the action has run */
    int active;
    int priority;
} BattleActionQueue;

/* Per-actor battle queue timing record, stride 0x44, with a queue value and flags. */
typedef struct BattleActorQueueRecord {
    unsigned char pad_00[0x94];
    unsigned short fallbackAttackId;
    unsigned char pad_96[0x4];
    unsigned short flags;
} BattleActorQueueRecord;

/* Six-byte battle action entry holding target flags, support bits and a counter. */
typedef struct BattleActionEntry {
    unsigned char commandIndex;
    unsigned char pad_01[0x1];
    unsigned char targetFlags;
    unsigned char pad_03[0x1];
    unsigned char counter;
    unsigned char supportBits;
} BattleActionEntry;

/* One command's row of g_BattleCommandProperties: the step id, the damage-calculation byte and
 * the attack flags Battle_ExecuteAction loads into the command context. */
typedef struct BattleCommandProperty {
    unsigned char stepId;
    unsigned char damageCalcByte;
    unsigned short attackFlags;
} BattleCommandProperty;

/* A command whose step flags are replaced by another (g_BattleCommandRemapTable). */
typedef struct BattleCommandRemap {
    int commandIndex;
    int stepFlags;
} BattleCommandRemap;

#pragma pack(push, 1)
typedef struct BattleActionSlot {
    unsigned char type; /* the actor's turnCount when it was stored */
    char actor;
    char commandId;
    short param;
    unsigned short extra;
    unsigned char pad_07[0x1];
} BattleActionSlot;
#pragma pack(pop)

/* --- battle_actor_anim_effect_tasks --- */
/* Animation set header of a battle actor model, giving bone count, clip tables and animation count. */
typedef struct BattleActorAnimSet {
    unsigned char pad_00[0x4];
    struct BattleAnimSkeleton *skeleton;
    int animCount;
    struct BattleAnimClip **clipTable;
    unsigned char pad_10[0x4];
    struct BattleAnimClip **weaponClipTable;
} BattleActorAnimSet;

/* The skeleton header of an actor's animation set; only the bone count is read. */
typedef struct BattleAnimSkeleton {
    unsigned char pad_00[0x10];
    short boneCount;
} BattleAnimSkeleton;

/* One animation clip: the number of frames (banks) it plays. */
typedef struct BattleAnimClip {
    unsigned char pad_00[0x4];
    int frameCount;
} BattleAnimClip;

/* The payload of a delayed effect task (a secondary-slot record): it counts delay down, then acts on
 * the actor and the value it was spawned with. */
typedef struct BattleEffectDelaySlot {
    short ownerSlot;
    short frameCounter;
    short delay;
    short value;
    short actor;
} BattleEffectDelaySlot;

/* One 0x64-byte record of a skeleton's texture table; handle is nonzero when it has a texture set. */
typedef struct BattleSkeletonTextureRecord {
    unsigned char pad_00[0x14];
    int handle;
    unsigned char pad_18[0x4c];
} BattleSkeletonTextureRecord;

/* A delayed effect task that runs an actor's effect: the spawner stores the actor as a word, the
 * task reads its low byte. */
typedef struct BattleEffectActorEffectSlot {
    short ownerSlot;
    short frameCounter;
    short delay;
    union {
        short actorWord;
        char actor;
    };
} BattleEffectActorEffectSlot;

/* A secondary task that fires one event when it is run. */
typedef struct BattleEffectEventSlot {
    short ownerSlot;
    short frameCounter;
    short event;
} BattleEffectEventSlot;

/* The main task that waits for the battle message window: a pre-delay, then the display time. */
typedef struct BattleMessageWaitSlot {
    short ownerSlot;
    short frameCounter;
    short displayTime;
    short preDelay;
} BattleMessageWaitSlot;

/* A status pose task spawned for an actor, with the flag that enables its panned sound. */
typedef struct BattlePoseTaskSlot {
    short ownerSlot;
    short frameCounter;
    short timer;
    short sfxEnabled;
    short actor;
} BattlePoseTaskSlot;

/* The secondary task that interprets a UV scroll script for an actor. */
typedef struct BattleUvScrollSlot {
    short ownerSlot;
    short frameCounter;
    short delay;
    short actor;
    unsigned char pad_08[0x8];
    unsigned char *script;
    unsigned char pad_14[0x4];
    char scriptIndex;
} BattleUvScrollSlot;

/* The main task of a hit reaction: delay, the hit actor and the event it plays when it completes. */
typedef struct BattleHitReactionSlot {
    short ownerSlot;
    short frameCounter;
    short delay;
    short actorWord;
    union {
        short actor;
        char actorByte; /* its low byte */
    };
    short targetParam;
    short otherActor;
    short damage;
    union {
        int event;
        short eventSfx; /* its low word: the sound the task plays when it completes */
    };
    int packed;
    char mode;
    char commitByte;
} BattleHitReactionSlot;

/* The secondary task that shows a hit's damage number. */
typedef struct BattleDamageNumberSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0xA];
    short value;
    int actor;
    int flags;
} BattleDamageNumberSlot;

/* The secondary task that commits a hit reaction's damage display after a short delay. */
typedef struct BattleHitCommitSlot {
    short ownerSlot;
    short frameCounter;
    short delay;
    unsigned char pad_06[0x2];
    short actor;
    short value;
} BattleHitCommitSlot;

/* The secondary task that pulses the HUD for a hit actor with the hit's packed flags. */
typedef struct BattleHitMarkSlot {
    short ownerSlot;
    short frameCounter;
    unsigned char pad_04[0x4];
    short actor;
    unsigned char pad_0A[0xA];
    int flags;
} BattleHitMarkSlot;

/* --- battle_actor_effect_sprites --- */
/* Sprite quad whose size ramps each frame from two base values. */
typedef struct BattleFxScaleRampSprite {
    short ownerSlot;
    short step;
    short posX; /* +0x04: the bone position RotTransPoint reads */
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short scaleX;
    short scaleY;
} BattleFxScaleRampSprite;

/* Scale-ramp emitter record with an actor index and a step counter. */
typedef struct BattleActorScaleRamp {
    short actorIndex;
    short step;
    unsigned char pad_04[0xa];
    short currentScale;
    short actorScale;
} BattleActorScaleRamp;

/* Battle world-sprite secondary task slot: frame counter, world position and sprite size. */
typedef struct BattleWorldSpriteRecord {
    short ownerSlot;
    short frameCount;
    union {
        struct {
            short posX; /* +0x04: the world position the sprite is drawn at */
            short posY;
            short posZ;
            short posPad;
        };
        Int64Pair position;
    };
    unsigned char pad_0C[0x2];
    short width;
    short height;
} BattleWorldSpriteRecord;

/* Secondary task slot that spawns a world sprite for a battle actor after a delay. */
typedef struct BattleDelayedActorSprite {
    short ownerSlot; /* +0x0 */
    unsigned char pad_2[0x2];
    union {
        struct {
            short offsetX; /* +0x4: the offset from the actor, rotated by its rotation */
            short offsetY;
            short offsetZ;
            short offsetPad;
        };
        Int64Pair position;
    };
    short delay;
    short width;
    short height;
    short actorIndex;
} BattleDelayedActorSprite;

#pragma pack(push, 1)
typedef struct BattleWorldSprite_DrawReq {
    unsigned char pad_0[0x4];
    char stepByte; /* +0x4 */
    unsigned char pad_5[0xb];
    int objectDword; /* +0x10 */
} BattleWorldSprite_DrawReq;
#pragma pack(pop)

/* --- battle_actor_event_handlers --- */
/* Four-byte battle event queue entry: actor (0xFF when free), opcode and payload. */
typedef struct BattleEventEntry {
    unsigned char actor;
    char opcode;
    short payload;
} BattleEventEntry;

/* One of the 320 battle inventory slots in g_BattleItemListBlock (+0x244, 6 bytes each): the
 * item id (0xFFFF when empty), the count and flag bits. The random pick skips a slot with bit 0
 * set, a use by item id one with bit 0 or 3; a slot that runs out gets flags 0xA. */
typedef struct BattleInventorySlot {
    union {
        unsigned short itemId;
        unsigned short id;
    };
    unsigned char count;
    unsigned char targetFlags;
    unsigned char flags;
    unsigned char pad_05[0x1];
} BattleInventorySlot;

/* --- battle_attack_op_handlers --- */
/* Attack list entry applied by Kernel_ApplyAttackToAction, with repeat-cast and countdown bytes. */
typedef struct BattleMagicMenuEntry {
    unsigned char pad_00[0x1];
    unsigned char mpCost;
    unsigned char countdown;
    unsigned char repeatCasts;
    unsigned char repeatsLeft;
    unsigned char pad_05[0x2];
    unsigned char supportBits;
} BattleMagicMenuEntry;

/* --- battle_camera_channel_scripts --- */
/* Auxiliary task slot for an eased camera-eye move, holding per-axis start, target and step values. */
typedef struct BattleCameraMoveTask {
    short ownerSlot;   /* +0x00: 0xFFFF frees the aux slot when the move ends */
    short phase;       /* +0x02: the aux slot's timerWord: 0 runs the init and a step, 1 a step */
    short framesLeft;  /* +0x04: the move's length from the script; the step counts it down to 1 */
    short targetX;     /* +0x06: where the channel ends; the last step stores it */
    short targetY;
    short targetZ;
    short startX;      /* +0x0C: the channel's position when the move was queued */
    short startY;
    short startZ;
    short channel;     /* +0x12: the eye or focus channel the move drives */
    short stepX;       /* +0x14: a linear move's step per frame, in 1/16 units */
    short stepY;
    short stepZ;
    short totalFrames; /* +0x1A: an eased move's length (Battle_CameraCosineEase) */
    unsigned char pad_1C[0x2];
    short elapsed;     /* +0x1E: an eased move's frames so far */
} BattleCameraMoveTask;

/* Root of the battle camera global scripts with file-relative script addresses. */
typedef struct BattleCameraScriptRoot {
    int eyeScriptAddr;
    int focusScriptAddr;
    int eyeScriptOffset;
    int focusScriptOffset;
} BattleCameraScriptRoot;

/* --- battle_command_menu_queue --- */
/* Base command record per actor (also the Frog substitute): command id, sub command, target flags and parameter. */
typedef struct {
    unsigned char commandId;
    char subCommand;
    char targetFlags;
    unsigned char pad_03[0x2];
    char param;
} BattleActorBaseCommand;

/* --- battle_command_ring_atb --- */
/* Per-actor 0x44-byte battle record that the ATB and gauge tick reads and updates. */
typedef struct BattleAtbRecord {
    unsigned char pad_00[0x2];
    unsigned short atbTick;
    unsigned short gauge;
    unsigned char pad_06[0x2];
    unsigned short flags;
} BattleAtbRecord;

/* One 8-byte entry in the battle command ring: actor, command id, target and extra value. */
typedef struct BattleCommandRingEntry {
    char kind;
    unsigned char pad_01[0x1];
    char actor;
    char commandId;
    short target;
    short extra;
} BattleCommandRingEntry;

/* Party stat record whose count and limit fields gate the turn queue escape check. */
typedef struct BattleTurnQueueEntry {
    unsigned char pad_00[0x10];
    short count;
    short limit;
    unsigned char pad_14[0x6];
    unsigned short limitGauge; /* the limit gauge in its high byte */
} BattleTurnQueueEntry;

/* Per-entry 0x44-byte record indexed by queue slot, holding a clock value for slip timing. */
typedef struct BattleSlipEntry {
    unsigned char pad_00[0x12];
    unsigned char deathSentenceTimer;
    unsigned char slowNumbTimer;
} BattleSlipEntry;

/* --- battle_end_sequence --- */
/* Six-byte end-of-battle item record: item id and quantity. */
typedef struct BattleEndItemRecord {
    unsigned short itemId;
    unsigned char quantity;
} BattleEndItemRecord;

/* --- battle_gzip_inflate_decoder --- */
/* Gzip dynamic-block table entry; a byte and a short are copied to the frame value. */
typedef struct BattleGzipEntry {
    unsigned char pad_00[0x1];
    unsigned char codeBits;
    unsigned char pad_02[0x2];
    unsigned short codeValue;
} BattleGzipEntry;

/* --- battle_hit_damage_resolution --- */
/* Hit target record with a hit counter byte and a flags word. */
typedef struct BattleHitTarget {
    unsigned char pad_00[0x28];
    unsigned char hitCounter;
    unsigned char pad_29[0xb];
    int flags;
} BattleHitTarget;

/* Pairs a source animation id with the pose id it translates to. */
typedef struct BattleAnimPoseTranslation {
    unsigned char fromAnimId;
    char toAnimId;
} BattleAnimPoseTranslation;

/* --- battle_hit_reaction_task_slots --- */
/* Effect100 slot record for the hit-reaction stagger: bit cursor, frame timing and pending-bit mask. */
typedef struct BattleHitStaggerRecord {
    short bitIndex;
    short frame;
    short startFrame;
    short interval;
    short pendingMask;
} BattleHitStaggerRecord;

/* Script timer slot: sequence index, a tick counter, a start tick and an interval. */
typedef struct BattleScriptTimer {
    short sequenceIndex;
    short timer;
    short startTime;
    short interval;
} BattleScriptTimer;

/* Main-slot entry for a hit-reaction stagger: target, strength and duration. */
typedef struct BattleHitReactionEntry {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short targetId;
    short strength;
    short duration;
} BattleHitReactionEntry;

/* Script timer task record: timer id and timer value. */
typedef struct BattleScriptTimerTask {
    short ownerSlot;
    unsigned char pad_02[0x2];
    short timerId;
    short timer;
} BattleScriptTimerTask;

/* --- battle_init_enemy_combatants --- */
/* Six-byte entry in an enemy's action table (sixteen per enemy, 0x60 bytes): the first three are the
 * enemy's attacks, the rest are reset to 0xFF/0/0/3 at battle init. */
typedef struct BattleEnemyCombatantSlot {
    char attackIndex;
    char actionKind;
    char targetFlags;
    char state;
    unsigned char pad_04[0x2];
} BattleEnemyCombatantSlot;

/* Battle staging block (g_BattleItemListBlock / DAT_009ac110, 0xA48 = 2632 bytes):
 * item row limit, inventory-dirty byte, six 16-slot enemy action tables, 320 6-byte
 * battle inventory slots, and the Battle Square character snapshot (+0x9C4). */
typedef struct BattleItemListBlock {
    union {
        unsigned char itemRowCount;
        unsigned char itemListLimit;
    };
    unsigned char inventoryChanged;
    unsigned char pad_02[0x2];
    union {
        BattleEnemyCombatantSlot enemyActions[6][16];
        unsigned char enemyActionBytes[576];
    };
    union {
        BattleInventorySlot slots[320];
        unsigned char itemSlots[1920];
    };
    SavemapCharacter savedCharacter; /* SavemapCharacter snapshot (+0x9C4..+0xA48) */
} BattleItemListBlock;
typedef char CheckBattleItemListBlock[(sizeof(BattleItemListBlock) == 2632) ? 1 : -1];

/* --- battle_item_fx_greens --- */
/* The Greens projectile task slot. The spawner fills it with the target actor, the start
 * position, the per-frame velocity and the gravity; the task frees it by writing 0xFFFF to
 * actorIndex. */
typedef struct BattleItemFxGreensProjectile {
    short actorIndex;
    short frame;
    SVECTOR pos;
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    SVECTOR angles;
    short accelerationY;
} BattleItemFxGreensProjectile;

/* Scratch block from data block zero: a position and a model matrix. */
#pragma pack(push, 2)
typedef struct BattleItemGreensScratch {
    Vec3sPacked pos;
    unsigned char pad_06[0x2];
    PsxMatrix matrix;
} BattleItemGreensScratch;
#pragma pack(pop)

/* --- battle_kernel_materia_support --- */
/* The materia description that the describe mode (g_MateriaDescribeModeFlag) fills for the
 * materia and shop menus through g_MateriaCurrentAbilityMask, instead of applying the materia.
 * Each kind of materia fills its own members. */
typedef struct BattleMateriaStatPlusAbility {
    char level;                     /* +0x00: the materia's level (Kernel_GetMateriaLevel) */
    unsigned char masteredCount;    /* +0x01: the level count; the level that equals it is mastered */
    unsigned char pad_02[0x2];
    int lookupLevel;                /* +0x04: the AP still needed for the next level, 0 at the last */
    unsigned char pad_08[0x2];
    union {                         /* +0x0A: the stat bonus row of g_MateriaStatBonusTable */
        short bonuses[8];
        struct {
            short strengthBonus;
            short vitalityBonus;
            short magicBonus;
            short spiritBonus;
            short dexterityBonus;
            short luckBonus;
            short maxHpPercentBonus;
            short maxMpPercentBonus;
        };
    };
    union {
        struct {                    /* +0x1A: a support, command, summon or encounter materia */
            char primaryCommandByte;
            char value;             /* the number shown with it: a level, a count, a percent or a table value */
        };
        struct {                    /* +0x1A: the five commands or spells of a list materia */
            char id;
            char marked;            /* 1 on the entry of the current level */
        } entries[5];
    };
} BattleMateriaStatPlusAbility;

#pragma pack(push, 1)
typedef struct MateriaTriggerDescriptor {
    short pendingTrigger; /* +0x0 */
    unsigned char pad_2[0x5];
    char levelParamBase; /* +0x7: the five level parameters below are read as [level] from here, level 1..5 */
    char levelParams[5]; /* +0x8 */
    unsigned char pad_d[0x3];
} MateriaTriggerDescriptor;
#pragma pack(pop)

/* --- battle_load_party_actors --- */
/* The weapon block of a party stat record (MenuPartyStats.weapon, +0x408), which the actor loaders
   read the weapon row from. */
typedef struct BattlePartyEquipSetup {
    char flags;
    char resolvedAttackId;
    char damageCalcByte;
    unsigned char pad_03[0x4];
    char criticalBonus;
    char accuracy;
    unsigned char pad_09[0x2];
    unsigned char slotBits; /* bit n: the nth swing's slot word gets 0x100 */
    short animParam;
    unsigned char pad_0E[0x2];
    unsigned short elementMask;
    unsigned char pad_12[0x12];
    unsigned char slotBytes[3]; /* +0x24: the low byte of each swing's slot word */
    char displayValue;
    short specialFlags;
} BattlePartyEquipSetup;

/* Per-slot auto-status record that accessory effects set and clear. */
typedef struct BattleAutoStatusRecord {
    unsigned char pad_00[0xd];
    char accessoryKind;
    unsigned char pad_0E[0x4];
    char deathSentenceTimer;
    unsigned char pad_13[0x21];
    int statusFlags;
} BattleAutoStatusRecord;

/* Armor data sub-record whose two byte fields are copied into the battle actor's equipment fields. */
typedef struct BattleArmorDataRecord {
    unsigned char pad_00[0x4];
    char defensePercent;
    char magicDefensePercent;
} BattleArmorDataRecord;

/* --- battle_magic_screen_shatter --- */
/* Offscreen request record for the magic screen-shatter effect, filled from the frame locals. */
typedef struct BattleMagicScreenShatterRequest {
    int drawKind;
    unsigned char pad_04[0xc4];
    int colorKeyEnabled;
    int renderKind;
    int blendMode;
    unsigned char pad_D4[0x8];
    int viewportX;
    int viewportY;
    int viewportW;
    int viewportH;
    unsigned char pad_EC[0x20];
    int sampleStepX;
    int sampleStepY;
    int offscreenSlot;
} BattleMagicScreenShatterRequest;

/* One 12-byte vertex of the 41-by-22 shatter grid: its screen position and its distance from the screen centre. */
typedef struct BattleMagicScreenShatterGridVertex {
    short x;
    short y;
    int radius;
    unsigned char pad_08[0x4];
} BattleMagicScreenShatterGridVertex;

/* One row of 41 vertices (0x1EC bytes). */
typedef BattleMagicScreenShatterGridVertex BattleMagicScreenShatterGridRow[0x29];

/* The motion part of one 0x80-byte shatter tile: the four grid vertices of its corners, its
 * displacement, velocity, rotation and spin. It is followed by the next tile's two quads. */
typedef struct BattleMagicScreenShatterGridTile {
    BattleMagicScreenShatterGridVertex *cornerPtr;
    BattleMagicScreenShatterGridVertex *rightNeighbor;
    BattleMagicScreenShatterGridVertex *downNeighbor;
    BattleMagicScreenShatterGridVertex *diagNeighbor;
    SVECTOR pos;
    SVECTOR vel;
    SVECTOR angle;
    SVECTOR spin;
    unsigned char pad_30[0x50];
} BattleMagicScreenShatterGridTile;

/* Header word of one of a shatter tile's two quads, read with the 0x80-byte tile stride. */
typedef struct BattleMagicScreenShatterTileHeader {
    int header;
    unsigned char pad_04[0x7c];
} BattleMagicScreenShatterTileHeader;

/* Colour and code bytes of a shatter quad, read with the 0x80-byte tile stride. */
typedef struct BattleMagicScreenShatterTileColor {
    char r;
    char g;
    char b;
    char code;
    unsigned char pad_04[0x7c];
} BattleMagicScreenShatterTileColor;

/* Texture coordinate bytes of one corner of a shatter quad, read with the 0x80-byte tile stride. */
typedef struct BattleMagicScreenShatterTileUv {
    char u;
    char v;
    unsigned char pad_02[0x7e];
} BattleMagicScreenShatterTileUv;

/* Colour and command bytes of the first tile's quad. */
typedef struct BattleMagicScreenShatterQuadColor {
    char r;
    char g;
    char b;
    char code;
} BattleMagicScreenShatterQuadColor;

/* Texture coordinates of one corner of the first tile's quad. */
typedef struct BattleMagicScreenShatterQuadUv {
    char u;
    char v;
} BattleMagicScreenShatterQuadUv;

#pragma pack(push, 1)
typedef struct BattleMagicScreenShatter_ScreenGrid {
    unsigned char pad_0[0x2];
    short shatterOutputY; /* +0x2 */
    int shatterOutputDistance; /* +0x4 */
    unsigned char pad_8[0x1e0];
} BattleMagicScreenShatter_ScreenGrid;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleMagicScreenShatter_GridRecords {
    unsigned char pad_0[0x2];
    short screenY; /* +0x2 */
    int radius; /* +0x4 */
    unsigned char pad_8[0x2a40];
} BattleMagicScreenShatter_GridRecords;
#pragma pack(pop)

/* The motion records of the 840 tiles. The last record runs 0x20 bytes past the object into the
 * TU's padding after it, as in the original. */
typedef struct BattleMagicScreenShatter_TileGrid {
    BattleMagicScreenShatterGridTile tiles[839];
    unsigned char tail[0x10];
} BattleMagicScreenShatter_TileGrid;

/* --- battle_opening_actor_setup --- */
/* Party actor binding record: actor record words and overlay words copied in one call. */
typedef struct BattleBindPartyRecord {
    short modelId;
    char modelFlags;
    unsigned char pad_03[0x1];
    short radius;
    short scaleX;
    short scaleY;
    short modelSlot;
    short posY;
    short bindIndex;
    short spriteY;
    char boneTable[0x10];       /* +0x12: copied to the actor's bone ids */
    unsigned char pad_22[0x22];
    short ratios[4];            /* +0x44: copied to ratioX, ratioY, ratioZ and ratioExtra of the actor state */
    short statusOffsetsA[6];    /* +0x4C: the first six status offsets of the actor state */
    short statusOffsetsB[6];    /* +0x58: the next six */
    short heightBias;
} BattleBindPartyRecord;

/* Enemy actor binding record: the same actor and overlay words as the party binding. */
typedef struct BattleBindEnemyRecord {
    short modelId;
    char modelFlags;
    unsigned char pad_03[0x1];
    short radius;
    short scaleX;
    short scaleY;
    short modelSlot;
    short posY;
    short bindIndex;
    unsigned char pad_10[0x2];
    char boneTable[0x10];       /* +0x12: copied to the actor's bone ids */
    unsigned char pad_22[0x22];
    short ratios[4];            /* +0x44: copied to ratioX, ratioY, ratioZ and ratioExtra of the actor state */
    short statusOffsetsA[6];    /* +0x4C: the first six status offsets of the actor state */
    short statusOffsetsB[6];    /* +0x58: the next six */
    short heightBias;
} BattleBindEnemyRecord;

/* The tables of a loaded model bank file that loading turns from file offsets into pointers by adding
 * the file's base address. */
typedef struct BattleModelFileOffsets {
    unsigned char pad_00[0x24];
    int partOffsets[8];  /* +0x24 */
    unsigned char pad_44[0x24];
    int animOffsets[0x4A]; /* +0x68: 0x4A entries in the party models, 0x20 in the enemy models */
} BattleModelFileOffsets;

/* The scratch Battle3d_PushActorNodeMatrices and Battle3d_ComposeNodeWithParent build at
 * g_BattleActor_BufferPtr: a node's parent matrix pointer, then a copy of that matrix. */
typedef struct BattleNodeMatrixScratch {
    int parentMatrix;
    struct PsxMatrix matrix;
} BattleNodeMatrixScratch;

/* --- battle_particle_burst_effects --- */
/* Effect-60 particle slot for a bursting bouncing model: position, velocity, bounce count. */
typedef struct BattleParticleBounceSlot {
    short bounces;
    unsigned char pad_02[0x6];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0E[0x2];
    short velX;
    short velY;
    short velZ;
    unsigned char pad_16[0x2];
    short yaw;
} BattleParticleBounceSlot;

/* Particle burst task record: tick, bone index, duration and flags with a 0x80 bit. */
typedef struct BattleParticleBurst {
    short endFlag;
    short tick;
    short burstType;
    short actorIndex;
    short delay;
    short duration;
    unsigned char pad_0C[0xe];
    short zOffset;
    int attachBoneIndex;
} BattleParticleBurst;

/* GPU textured-quad primitive written by the particle burst task. */
typedef struct BattleParticleQuad {
    int tag;
    int colorCode;
    unsigned char pad_08[0x4];
    short uv0;
    short clut;
    unsigned char pad_10[0x4];
    short uv1;
    short tpage;
    unsigned char pad_18[0x4];
    short uv2;
    unsigned char pad_1E[0x6];
    short uv3;
} BattleParticleQuad;

/* Effect slot of an attached particle burst: timer, actor attachment and bone index. */
typedef struct BattleFxParticleBurst {
    short ownerSlot;
    short timer;
    short burstType;
    short actorId;
    unsigned char pad_08[0x10];
    short yaw;
    short offsetZ;
    int attachIndex;
} BattleFxParticleBurst;

/* Bone particle burst task record: burst type, actor, timing and bone index. */
typedef struct BattleBoneParticleBurst {
    unsigned char pad_00[0x4];
    short burstType;
    short actorIndex;
    short delay;
    short duration;
    unsigned char pad_0C[0xe];
    short zOffset;
    int boneIndex;
} BattleBoneParticleBurst;

/* Task slot for a particle burst quad; its translation shorts feed the quad translation globals. */
typedef struct BattleParticleBurstQuad {
    short status;
    unsigned char pad_02[0x6];
    short translationX;
    unsigned char pad_0A[0x2];
    short translationZ;
} BattleParticleBurstQuad;

/* Task slot for a particle burst attached to a model; stores positions relative to the trail translation. */
typedef struct BattleParticleAttachedModel {
    short status;
    unsigned char pad_02[0x6];
    short relX;
    short relY;
    short relZ;
} BattleParticleAttachedModel;

/* --- battle_pending_drops --- */
/* One collected drop: item id, count, and two bytes from the item's data table. */
typedef struct BattleDropEntry {
    short itemId;
    char count;
    char dropType;
    char dropFlags;
} BattleDropEntry;

/* --- battle_ring_buffer_effects --- */
/* Battle effect ramp task: 16.16 fixed-point value, step and countdown timer. */
typedef struct BattleFxRampTask {
    short ownerSlot;
    unsigned char pad_02[0x2];
    int value;
    int step;
    int timer;
} BattleFxRampTask;

/* The function a bit-pattern task calls for each set bit: the bit's index (an actor: the masks are
   actor masks) and the task's argument. */
typedef void (__cdecl *BattleBitPatternCallback)(int actor, int callbackArg);

/* The battle task slot of Battle_CreateBitPatternCallbackTask: Battle_BitPatternCallbackTaskHandler walks the set
   bits of mask and calls callback for each, all in one frame when interval is 0, else one every
   interval frames. */
typedef struct BattleBitPatternTask {
    short bitIndex; /* +0x00: the next bit to test; the slot's first word, which the spawner clears */
    short counter; /* +0x02: frames since the last call */
    short mask; /* +0x04 */
    short callbackArg; /* +0x06 */
    short interval; /* +0x08 */
    unsigned char pad_0A[0x2];
    BattleBitPatternCallback callback; /* +0x0C */
} BattleBitPatternTask;

/* --- battle_special_attack_effect --- */
/* Per-actor party stat block of 0x34 bytes, indexed by actor index. */
typedef struct BattlePartyStatBlock {
    struct SavemapCharacter *partyMember;
    unsigned char pad_04[0x1];
    unsigned char scaleStep;
    unsigned char pad_06[0x4];
    unsigned short statValue;
} BattlePartyStatBlock;

/* --- battle_special_bless_effects --- */
/* Per-slot state of the Bless special effect timeline: random X/Z offsets and a variant flag. */
typedef struct BattleFxRadiantBreathTimelineState {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0xc];
    short variant;
} BattleFxRadiantBreathTimelineState;

/* The battle task slot of BattleFx_SpecialRadiantBreathGrowingModelSparksTask. */
typedef struct BattleFxRadiantBreathGrowingModelSparksSlot {
    short ownerSlot;
    short frameCounter;
    short growthX;
    short growthY;
    short growthZ;
} BattleFxRadiantBreathGrowingModelSparksSlot;

/* The battle task slot of BattleFx_SpecialRadiantBreathExpandingShellsTask. */
typedef struct BattleFxRadiantBreathExpandingShellsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxRadiantBreathExpandingShellsSlot;

/* Slots BattleFx_SpecialRadiantBreathExpandingShellsTask allocates for BattleFx_SpecialBless{LargeAnimatedSprite, 2 more}Task. */
typedef struct BattleFxRadiantBreathExpandingShellsSpawnSlot {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_0A[0xa];
    short angle;
    short variant;
} BattleFxRadiantBreathExpandingShellsSpawnSlot;

/* Timeline slot for the Bless ground burst, placed at an actor offset. */
typedef struct BattleFxRadiantBreathGroundSlot {
    unsigned char pad_00[0x4];
    short offsetX;
    short offsetY;
    short offsetZ;
} BattleFxRadiantBreathGroundSlot;

#pragma pack(push, 1)
typedef struct BattleSpecRadiantBreathAnimData4 {
    unsigned char pad_0[0x18];
    int record24; /* +0x18 */
    int drawObj; /* +0x1c */
} BattleSpecRadiantBreathAnimData4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecRadiantBreathAnimData7 {
    unsigned char pad_0[0x8];
    int risingFadeRecord; /* +0x8 */
    int drawObj; /* +0xc */
} BattleSpecRadiantBreathAnimData7;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecRadiantBreathSpriteRequest1 {
    int spriteEntry; /* +0x0 */
    int req1Resource; /* +0x4 */
    unsigned char pad_8[0x4];
    short req1Word12; /* +0xc */
    unsigned char pad_e[0x2];
} BattleSpecRadiantBreathSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecRadiantBreathSpriteRequest2 {
    char red; /* +0x0 */
    char req2Byte1; /* +0x1 */
    char req2Byte2; /* +0x2 */
    unsigned char pad_3[0x5];
} BattleSpecRadiantBreathSpriteRequest2;
#pragma pack(pop)

/* The large animated sprite's request, then the head of the rising spin model's draw request (its
 * state flags are the word after this object). */
typedef struct {
    BattleFxSpriteRequest sprite; /* +0x0 */
    int modelGroup; /* +0x10 */
    int drawFlags; /* +0x14 */
    short uvScroll; /* +0x18 */
    short fadeLevel; /* +0x1a */
} BattleSpecRadiantBreathSpriteRequest3;

/* The rising fade sprite's colour and frame, then the head of the expanding shells' draw request. */
#pragma pack(push, 1)
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int modelGroup; /* +0x8 */
    unsigned char pad_c[0x6];
    short fadeLevel; /* +0x12 */
} BattleSpecRadiantBreathSpriteRequest4;
#pragma pack(pop)

/* The ground burst's model draw request head and sprite request, then the frame-table sprite's request. */
typedef struct {
    BattleFxModelDrawHead groundModel; /* +0x0 */
    BattleFxSpriteRequest groundSprite; /* +0x10 */
    BattleFxSpriteRequest frameSprite; /* +0x20 */
} BattleSpecRadiantBreathTransformData1;

#pragma pack(push, 1)
typedef struct BattleSpecRadiantBreathTransformData2 {
    int resEntry; /* +0x0 */
    int flags; /* +0x4 */
    unsigned char pad_8[0x2];
    short alpha; /* +0xa */
    unsigned char pad_c[0x4];
} BattleSpecRadiantBreathTransformData2;
#pragma pack(pop)

/* --- battle_special_club_second_effects --- */
/* Effect-100 slot for one Special Club2 impact particle: position and frame count. */
typedef struct BattleFxStarCardImpactParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    short frameCount;
    short targetX;
    short targetY;
    short targetZ;
    short variant;
    short angle;
    short velocityY;
    short velocityX;
    short size;
    short scale;
    short scaleRate;
} BattleFxStarCardImpactParticle;

/* Special Club2 falling model record: position, velocity, tilt and model selector. */
typedef struct BattleFxStarCardFallRecord {
    short ownerSlot;
    short phase;
    short posX;
    short posY;
    short posZ;
    short endPhase;
    short velocityX;
    short velocityY;
    short velocityZ;
    short modelIndex;
    short angle;
    short angleVelocity;
    short tilt;
    short tiltVelocity;
    short scale;
    short scaleRate;
} BattleFxStarCardFallRecord;

/* Club2 impact timeline record: step counter and X/Z positions compared to the camera and target. */
typedef struct BattleFxStarCardImpact {
    short ownerSlot;
    short step;
    short posX;
    unsigned char pad_06[0x2];
    short posZ;
    unsigned char pad_0A[0x12];
    short angle;
} BattleFxStarCardImpact;

/* Club2 spreading model record: base positions, angle with angular velocity. */
typedef struct BattleFxStarCardSpread {
    short ownerSlot;
    short step;
    short baseA;
    short baseB;
    short baseC;
    unsigned char pad_0A[0xa];
    short angle;
    short angleVelocity;
    unsigned char pad_18[0x4];
    short scaleXZ;
    short scaleY;
} BattleFxStarCardSpread;

/* Club2 cycling model record: base positions, angle, and a second phase with its velocity. */
typedef struct BattleFxStarCardCycle {
    short ownerSlot;
    short step;
    short baseA;
    short baseB;
    short baseC;
    unsigned char pad_0A[0xa];
    short angle;
    short angleVelocity;
    short phase;
    short phaseVelocity;
    short scale;
} BattleFxStarCardCycle;

/* Special Club2 stretched sprite 1: sprite frame plus start and end positions for interpolation. */
typedef struct BattleFxStarCardStretch {
    short ownerSlot;
    short spriteFrame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
} BattleFxStarCardStretch;

/* Per-frame state of the Special Club2 black-model effect task, stored in a BattleFx effect slot. */
typedef struct BattleFxStarCardBlackModelState {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short accumA;
    short stepA;
    short accumB;
    short stepB;
    short scale;
} BattleFxStarCardBlackModelState;

/* Club2 pulsing model slot: timer, two accumulating angles with their per-tick steps. */
typedef struct BattleFxStarCardPulsingModel {
    short ownerSlot;
    short timer;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xa];
    short angle;
    short angleStep;
    short pulseAngle;
    short pulseAngleStep;
    short scale;
} BattleFxStarCardPulsingModel;

/* Special Club2 spinning model effect record: step counter, Y position and Y velocity. */
typedef struct BattleFxStarCardSpinRecord {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    short spinRadius;
    unsigned char pad_0C[0x2];
    short velocityY;
    unsigned char pad_10[0x8];
    short scale;
} BattleFxStarCardSpinRecord;

/* Club2 billboard sprite record; its counter is the age of the sprite. */
typedef struct BattleFxStarCardBillboardSprite {
    short ownerSlot;
    short age;
    unsigned char pad_04[0x18];
    short scale;
} BattleFxStarCardBillboardSprite;

/* Main task slot of the Club 2 falling model, seeded from the spreading model's angle. */
typedef struct BattleStarCardFallingModel {
    unsigned char pad_00[0x6];
    short posY;
    unsigned char pad_08[0xc];
    short angle;
    short angleVelocity;
    unsigned char pad_18[0x4];
    short scaleXZ;
    short scaleY;
} BattleStarCardFallingModel;

/* Impact timeline record for the Club special effect, placed at a bone position. */
typedef struct BattleFxStarCardImpactTimeline {
    unsigned char pad_00[0x4];
    short bonePos;
    unsigned char pad_06[0x16];
    short offsetY;
} BattleFxStarCardImpactTimeline;

/* --- battle_special_daionpa_effects --- */
/* The battle task slot of BattleFx_SpecialGiantSonicWaveTimelineTask. */
typedef struct BattleFxGiantSonicWaveTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
} BattleFxGiantSonicWaveTimelineSlot;

/* The battle task slot of BattleFx_SpecialGiantSonicWaveSpinFadeSpriteTask. */
typedef struct BattleFxGiantSonicWaveSpinFadeSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short recordIndex;
} BattleFxGiantSonicWaveSpinFadeSpriteSlot;

#pragma pack(push, 1)
typedef struct BattleSpecGiantSonicWaveAnimData1 {
    int modelGroup; /* +0x0 */
    unsigned char pad_4[0x6];
    short alpha; /* +0xa */
} BattleSpecGiantSonicWaveAnimData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecGiantSonicWaveAnimData2 {
    unsigned char pad_0[0x8];
    int modelRecord; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
    unsigned char pad_14[0x4];
    int spriteRecord; /* +0x18 */
    int drawObj; /* +0x1c */
} BattleSpecGiantSonicWaveAnimData2;
#pragma pack(pop)

/* --- battle_special_effect_scaling --- */
/* Three-byte row of the slots combination table. */
typedef struct BattleSlotsCombinationEntry {
    unsigned char requiredSlot0;
    unsigned char requiredSlot1;
    unsigned char requiredSlot2;
} BattleSlotsCombinationEntry;

/* --- battle_special_energy_effects --- */
/* Energy per-target timeline record: start and end points and the target actor index. */
typedef struct BattleFxEnergyTimeline {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short targetActorIdx;
} BattleFxEnergyTimeline;

/* Slots BattleFx_SpecialEnergyTimelineTask allocates for BattleFx_SpecialEnergy{MainSlotAnimatedSpriteTask, 6 more}. */
typedef struct BattleFxEnergyTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short variant;
    short startValue;
    short clearedParam;
} BattleFxEnergyTimelineSpawnSlot;

/* A sprite task slot of the energy effect: frame counter and position. */
typedef struct BattleFxEnergySpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxEnergySpriteSlot;

/* The battle task slot of BattleFx_SpecialEnergyStretchingModelsTask. */
typedef struct BattleFxEnergyStretchingModelsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short stretchScaleY;
} BattleFxEnergyStretchingModelsSlot;

/* The battle task slot of BattleFx_SpecialEnergyBeamModelsTask. */
typedef struct BattleFxEnergyBeamModelsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short pitchRate;
} BattleFxEnergyBeamModelsSlot;

/* The bouncing sprite's colour and frame, the expanding and spin models' draw request heads, then
 * the animated and shrinking sprites' handles; each sprite request runs on into the colour of the
 * object after it. */
#pragma pack(push, 1)
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    char primCode; /* +0x3 */
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int expandModelGroup; /* +0x8 */
    unsigned char pad_c[0x6];
    short expandFadeLevel; /* +0x12 */
    unsigned char pad_14[0x4];
    int spinModelGroup; /* +0x18 */
    int spinDrawFlags; /* +0x1c */
    unsigned char pad_20[0x2];
    short spinFadeLevel; /* +0x22 */
    unsigned char pad_24[0x4];
    int animSpriteGroup; /* +0x28 */
    int animDrawObj; /* +0x2c */
    unsigned char pad_30[0x4];
    short animFrame; /* +0x34 */
    unsigned char pad_36[0x2];
    int shrinkSpriteGroup; /* +0x38 */
    int shrinkDrawObj; /* +0x3c */
} BattleSpecialEnergyModelDescriptors;
#pragma pack(pop)

/* The secondary-slot animated sprite's request, then the bouncing sprite's handles. */
#pragma pack(push, 1)
typedef struct {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int bounceSpriteGroup; /* +0x10 */
    int bounceDrawObj; /* +0x14 */
} BattleSpecialEnergySpriteConfig1;
#pragma pack(pop)

/* The shrinking sprite's colour and frame, then the stretching models' draw request head. */
#pragma pack(push, 1)
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int modelGroup; /* +0x8 */
    unsigned char pad_c[0x6];
    short fadeLevel; /* +0x12 */
} BattleSpecialEnergySpriteRequest1;
#pragma pack(pop)

/* --- battle_special_frog_effects --- */
/* Per-target frog spawn record: start and end positions per axis and the target index. */
typedef struct BattleFxFrogSpawnRecord {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0x2];
    short randomRange;
    unsigned char pad_16[0x6];
    short targetIndex;
} BattleFxFrogSpawnRecord;

/* The battle task slot of BattleFx_SpecialFrogTimelineTask. */
typedef struct BattleFxFrogTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0x2];
    short randomRange;
    unsigned char pad_16[0x6];
    short actorIndex;
} BattleFxFrogTimelineSlot;

/* The battle task slot of BattleFx_SpecialFrogSpiralModelTask. */
typedef struct BattleFxFrogSpiralModelSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x4];
    short yaw;
    unsigned char pad_10[0x4];
    short scaleY;
    unsigned char pad_16[0x2];
    short distance;
    short spiralIndex;
} BattleFxFrogSpiralModelSlot;

/* Slots BattleFx_SpecialFrogTimelineTask allocates for BattleFx_SpecialFrog{AnimatedSprite, GrowingModel, 1 more}Task. */
typedef struct BattleFxFrogTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x4];
    short angle;
    unsigned char pad_10[0x4];
    short scaleY;
    unsigned char pad_16[0x2];
    short distance;
    short spiralIndex;
} BattleFxFrogTimelineSpawnSlot;

/* Ring-buffer record for the Frog special effect timeline. */
typedef struct BattleFxFrogTimeline {
    short actorIndex;
    unsigned char pad_02[0xe];
    short effectOrder;
} BattleFxFrogTimeline;

/* Frog sprite definition holding the current animation frame table pointer. */
typedef struct BattleSpecFrogSpriteDef {
    unsigned char pad_00[0x70];
    int frameTable;
    int drawObj;
} BattleSpecFrogSpriteDef;

#pragma pack(push, 1)
typedef struct BattleSpecFrogBuffer {
    short spanX; /* +0x0 */
    short spanY; /* +0x2 */
} BattleSpecFrogBuffer;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecFrogSpriteRequest {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int reqResource; /* +0x4 */
    unsigned char pad_8[0x4];
    short reqWord12; /* +0xc */
    unsigned char pad_e[0x2];
    int modelRecord; /* +0x10 */
    unsigned char pad_14[0x6];
    short reqWord26; /* +0x1a */
} BattleSpecFrogSpriteRequest;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecFrogTransformData {
    unsigned char pad_0[0x8];
    int resource8; /* +0x8 */
    unsigned char pad_c[0x6];
    short alpha; /* +0x12 */
    unsigned char pad_14[0x4];
} BattleSpecFrogTransformData;
#pragma pack(pop)

/* --- battle_special_gesui_effects --- */
/* Gesui1 timeline task record with randomly jittered values and a toggle. */
typedef struct BattleFxSewerTsunamiTask {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x10];
    short isActive;
} BattleFxSewerTsunamiTask;

/* What BattleFx_SpecialSewerTsunamiWaveSurfaceTask reads through Gfx_GetPsxPacketScratchPtr(). */
typedef struct BattleFxSewerTsunamiSurfaceRenderStateBlock {
    int packetTag;
    union {
        int rgbCmd;
        struct { char rgbR, rgbG, rgbB, primCode; };
    };
    unsigned char pad_08[0x4];
    char vertex0U;
    char vertex0V;
    short clut;
    unsigned char pad_10[0x4];
    char vertex1U;
    char vertex1V;
    short tpage;
    unsigned char pad_18[0x4];
    char vertex2U;
    char vertex2V;
    unsigned char pad_1E[0x6];
    char vertex3U;
    char vertex3V;
} BattleFxSewerTsunamiSurfaceRenderStateBlock;

/* The Gesui1 wave direction (1 or -1), the sprite request of the splash sprites, and the sprite-group
 * index each splash variant draws. */
#pragma pack(push, 1)
typedef struct BattleSpecSewerTsunamiSpriteRequest {
    short direction;
    unsigned char pad_2[0x6];
    BattleFxSpriteRequest sprite; /* +0x8 */
    int variantTable; /* +0x18: the first of two entries; the second is g_rawData_0089a0b4 */
} BattleSpecSewerTsunamiSpriteRequest;
#pragma pack(pop)

/* --- battle_special_hado_effects --- */
/* Hado per-target timeline record: start and end points and the target mask. */
typedef struct BattleFxEnergyWaveTimeline {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short targetMask;
} BattleFxEnergyWaveTimeline;

/* Slots BattleFx_SpecialEnergyWaveTimelineTask allocates for BattleFx_SpecialEnergyWave{MainSlotAnimatedSpriteTask, 6 more}. */
typedef struct BattleFxEnergyWaveTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short variant;
    short scale;
    short clearedParam;
} BattleFxEnergyWaveTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialEnergyWaveStretchingModelsTask. */
typedef struct BattleFxEnergyWaveStretchingModelsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short stretchRate;
} BattleFxEnergyWaveStretchingModelsSlot;

/* The battle task slot of BattleFx_SpecialEnergyWaveBeamModelsTask. */
typedef struct BattleFxEnergyWaveBeamModelsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short pitchStep;
} BattleFxEnergyWaveBeamModelsSlot;

/* The bouncing sprite's colour and frame, the expanding and spin models' draw request heads, then
 * the animated and shrinking sprites' handles; each sprite request runs on into the colour of the
 * object after it. */
#pragma pack(push, 1)
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    char primCode; /* +0x3 */
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int expandModelGroup; /* +0x8 */
    unsigned char pad_c[0x6];
    short expandFadeLevel; /* +0x12 */
    unsigned char pad_14[0x4];
    int spinModelGroup; /* +0x18 */
    int spinDrawFlags; /* +0x1c */
    unsigned char pad_20[0x2];
    short spinFadeLevel; /* +0x22 */
    unsigned char pad_24[0x4];
    int animSpriteGroup; /* +0x28 */
    int animDrawObj; /* +0x2c */
    unsigned char pad_30[0x4];
    short animFrame; /* +0x34 */
    unsigned char pad_36[0x2];
    int shrinkSpriteGroup; /* +0x38 */
    int shrinkDrawObj; /* +0x3c */
} BattleSpecialEnergyWaveModelDescriptors;
#pragma pack(pop)

/* The secondary-slot animated sprite's request, then the bouncing sprite's handles. */
#pragma pack(push, 1)
typedef struct {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int bounceSpriteGroup; /* +0x10 */
    int bounceDrawObj; /* +0x14 */
} BattleSpecialEnergyWaveSpriteConfig1;
#pragma pack(pop)

/* The shrinking sprite's colour and frame, then the stretching models' draw request head. */
#pragma pack(push, 1)
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int modelGroup; /* +0x8 */
    unsigned char pad_c[0x6];
    short fadeLevel; /* +0x12 */
} BattleSpecialEnergyWaveSpriteRequest1;
#pragma pack(pop)

/* --- battle_special_heart_first_effects --- */
/* Heart1 bone particle sprite: steps through an alpha table as its index counts up. */
typedef struct BattleFxHeartCardBoneParticleSprite {
    short ownerSlot;
    short alphaIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x3];
    char cellRow;
    short cellWord;
    short scale;
    unsigned char pad_12[0x4];
    short flags;
} BattleFxHeartCardBoneParticleSprite;

/* World position of an actor's bone, with the actor's heading in the fourth short. */
typedef struct BattleSpecialHeartCardBonePos {
    short x;
    short y;
    short z;
    short heading;
} BattleSpecialHeartCardBonePos;

/* --- battle_special_heart_second_effects --- */
/* Heart 2 gather particle slot: start position and velocity per axis converging on the centre. */
typedef struct BattleFxHiddenOneCardGatherParticle {
    unsigned char pad_00[0x4];
    union {
        Int64Pair pos;
        struct {
            short posX;
            short posY;
            short posZ;
            unsigned char pad_0A[0x2];
        };
    };
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short scale; /* a particle's billboard size; a pulled actor's starting scale */
    short actorIndex; /* the pulled actor */
} BattleFxHiddenOneCardGatherParticle;

/* The second Heart special's actor bone position with the actor's heading beside it. */
typedef struct {
    union {
        Int64Pair pos;
        struct {
            short posX;
            short posY;
            short posZ;
            short heading;
        };
    };
} BattleSpecialHeart2BonePos;

/* --- battle_special_hoden_effects --- */
/* HoDen fading model task record with a step counter driving a fade value. */
typedef struct BattleFxEnergyBallFadeTask {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_12[0x4];
    short modelIndex;
} BattleFxEnergyBallFadeTask;

/* HoDen per-target timeline record: start and end points. */
typedef struct BattleFxEnergyBallTimeline {
    unsigned char pad_00[0x4];
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short targetIdx;
} BattleFxEnergyBallTimeline;

/* Slots BattleFx_SpecialEnergyBallTimelineTask allocates for BattleFx_SpecialHoDen{AnimatedSprite, BeamModels, 4 more}Task. */
typedef struct BattleFxEnergyBallTimelineSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleX;
    short angleY;
    short angleZ;
    unsigned char pad_12[0x4];
    short modelIndex;
    short scale;
    short clearedParam;
} BattleFxEnergyBallTimelineSpawnSlot;

/* The battle task slot of BattleFx_SpecialEnergyBallCounterSpinModelsTask. */
typedef struct BattleFxEnergyBallCounterSpinModelsSlot {
    short ownerSlot;
    short frameCounter;
    short rotX;
    short rotY;
    short rotZ;
} BattleFxEnergyBallCounterSpinModelsSlot;

/* The battle task slot of BattleFx_SpecialEnergyBallBeamModelsTask. */
typedef struct BattleFxEnergyBallBeamModelsSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0xe];
    short stepRate;
} BattleFxEnergyBallBeamModelsSlot;

#pragma pack(push, 1)
typedef struct BattleSpecEnergyBallStateBuffer {
    unsigned char pad_0[0x12];
    int x; /* +0x12 */
    int y; /* +0x16 */
    int z; /* +0x1a */
    unsigned char pad_1e[0x2];
} BattleSpecEnergyBallStateBuffer;
#pragma pack(pop)

/* --- battle_special_kakusan_effects --- */
/* Kakusan timeline slot: timer and position, aimed at the battle target centroid. */
typedef struct BattleFxSpecialSpreadLaserTimeline {
    short step;
    short timer;
    short posX;
    short posY;
    short posZ;
} BattleFxSpecialSpreadLaserTimeline;

/* Kakusan effect record: start position, travel angle, variant and travel length. */
typedef struct BattleFxSpreadLaserRecord {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short angleJitter;
    short angle;
    short lift;
    unsigned char pad_12[0x2];
    short variant;
    short side;
    short travelLength;
} BattleFxSpreadLaserRecord;

/* The battle task slot of BattleFx_SpecialSpreadLaserStreakSpriteTask. */
typedef struct BattleFxSpreadLaserStreakSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short rotX;
    short rotY;
    short rotZ;
    unsigned char pad_12[0x2];
    short stepCount;
    short direction;
    short scale;
} BattleFxSpreadLaserStreakSpriteSlot;

#pragma pack(push, 1)
typedef struct BattleSpecialSpreadLaserEffectConfig {
    int modelResField; /* +0x0 */
    unsigned char pad_4[0x6];
    short flag2Shifted; /* +0xa */
} BattleSpecialSpreadLaserEffectConfig;
#pragma pack(pop)

/* --- battle_special_korosi_effects --- */
/* The battle task slot of BattleFx_SpecialChefsKnifeTimelineTask, which BattleFx_SpecialChefsKnifeSpawnTimelineTask
 * fills: the target bone's position and the effect's radius, and the index of the next sprite. */
typedef struct BattleFxChefsKnifeTimelineSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short effectRadius;
    short doubleRadius;
    short velocity;
    short doubledVelocity;
    short spawnIndex;
} BattleFxChefsKnifeTimelineSlot;

/* The task slot of a Korosi sprite (the animated and the pulse sprite): its frame, position and scale. */
typedef struct BattleFxChefsKnifeAnimatedSprite {
    short ownerSlot;
    short frameIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short scale;
} BattleFxChefsKnifeAnimatedSprite;

/* g_BattleSpecChefsKnifeSpriteRequest: the tail of the pulse sprite's BattleFxSpriteRequest, whose
 * sprite group and draw object end g_BattleSpecChefsKnifeAnimData, then the animated sprite's whole
 * request. */
#pragma pack(push, 1)
typedef struct {
    char pulseColorR; /* +0x0 */
    char pulseColorG;
    char pulseColorB;
    unsigned char pulseCode; /* the PSX primitive code; not read */
    unsigned short pulseFrame;
    unsigned short pulseUvPacked;
    BattleFxSpriteRequest animated; /* +0x8 */
} BattleSpecChefsKnifeSpriteRequest;
#pragma pack(pop)

/* --- battle_special_mizufuki_effects --- */
/* Mizufuki track record: start position, per-step velocity, frame counter and bone index. */
typedef struct BattleFxWaterballTrack {
    short ownerSlot;
    short frame;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0xa];
    short boneIndex;
} BattleFxWaterballTrack;

/* Mizufuki sprite and model particle: position, drift deltas, phase, timer and stage. */
typedef struct BattleFxWaterballParticle {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short deltaX;
    short deltaY;
    short deltaZ;
    unsigned char pad_12[0x2];
    short phase;
    short timer;
    short variant;
    short stage;
    short step;
} BattleFxWaterballParticle;

/* Mizufuki projectile sprite: step, position with velocity, billboard angle and scale. */
typedef struct BattleFxWaterballProjectileRecord {
    short ownerSlot;
    short step;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x4];
    short scale;
    short spawnIndex;
} BattleFxWaterballProjectileRecord;

/* Slots BattleFx_SpecialWaterballSpinModelTask allocates for BattleFx_SpecialMizufuki{DriftingAnimatedSprite, 2 more}Task. */
typedef struct BattleFxWaterballSpinModelSpawnSlot {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short offsetX;
    short offsetY;
    short offsetZ;
    unsigned char pad_12[0x2];
    short startAngle;
    short startRadius;
    unsigned char pad_18[0x2];
    short spinDirection;
} BattleFxWaterballSpinModelSpawnSlot;

/* The battle task slot of BattleFx_SpecialWaterballSpiralSpriteTask. */
typedef struct BattleFxWaterballSpiralSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short centerX;
    short posY;
    short centerZ;
    unsigned char pad_0A[0xa];
    short angle;
    short radius;
    unsigned char pad_18[0x2];
    short spriteIndex;
} BattleFxWaterballSpiralSpriteSlot;

#pragma pack(push, 1)
typedef struct BattleSpecWaterballTransformData {
    unsigned char pad_0[0x10];
    int record; /* +0x10 */
    unsigned char pad_14[0x6];
    short alpha; /* +0x1a */
    unsigned char pad_1c[0x4];
} BattleSpecWaterballTransformData;
#pragma pack(pop)

/* --- battle_special_renmisa_effects --- */
/* Renmisa track record: start and end positions of the traveling set, its frame counter and bone index. */
typedef struct BattleFxMissileSalvoTrack {
    short ownerSlot;
    short frame;
    short startX;
    short startY;
    short startZ;
    unsigned char pad_0A[0x2];
    short endX;
    short endY;
    short endZ;
    unsigned char pad_12[0xa];
    short targetIndex;
    short sourceIndex;
} BattleFxMissileSalvoTrack;

/* Renmisa effect slot: start position, per-axis step, travel angle, variant and frame count. */
typedef struct BattleFxMissileSalvoSegment {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short stepX;
    short stepY;
    short stepZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x2];
    short variant;
    short frameCount;
} BattleFxMissileSalvoSegment;

/* The battle task slot of BattleFx_SpecialMissileSalvoProjectileTask. */
typedef struct BattleFxMissileSalvoProjectileSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x2];
    short angle;
    unsigned char pad_16[0x4];
    short arcVelocity;
} BattleFxMissileSalvoProjectileSlot;

/* The battle task slot of BattleFx_SpecialMissileSalvoBouncingSpriteTask. */
typedef struct BattleFxMissileSalvoBouncingSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short angle;
    short variant;
} BattleFxMissileSalvoBouncingSpriteSlot;

/* What BattleFx_SpecialMissileSalvoProjectileTask reads through g_BattleSpecMissileSalvoDrawListHead. */
typedef struct BattleFxMissileSalvoProjectileState {
    int commandWord;
    char colorR;
    char colorG;
    char colorB;
    char primCode;
} BattleFxMissileSalvoProjectileState;

/* Renmisa growing model task state: growth step and translation. */
typedef struct BattleFxMissileSalvoGrowingRecord {
    short ownerSlot;
    short growStep;
    short posX;
    short posY;
    short posZ;
} BattleFxMissileSalvoGrowingRecord;

#pragma pack(push, 1)
typedef struct BattleSpecMissileSalvoSpriteRequest1 {
    int spriteResource; /* +0x0 */
    int req1Object; /* +0x4 */
    unsigned char pad_8[0x4];
    short req1Word12; /* +0xc */
    unsigned char pad_e[0x2];
    int record; /* +0x10 */
    int drawObj; /* +0x14 */
} BattleSpecMissileSalvoSpriteRequest1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecMissileSalvoSpriteRequest2 {
    char red; /* +0x0 */
    char req2Byte1; /* +0x1 */
    char req2Byte2; /* +0x2 */
    char req2Byte3; /* +0x3 */
    short req2Word4; /* +0x4 */
    unsigned char pad_6[0x2];
    int growingModelRecord; /* +0x8 */
    unsigned char pad_c[0x6];
    short req2Word18; /* +0x12 */
    unsigned char pad_14[0x4];
    int modelRecord; /* +0x18 */
    int req2Int28; /* +0x1c */
    unsigned char pad_20[0x2];
    short req2Word34; /* +0x22 */
} BattleSpecMissileSalvoSpriteRequest2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecMissileSalvoTransformMatrix {
    unsigned char pad_0[0x10];
    int modelRecord; /* +0x10 */
    unsigned char pad_14[0xc];
    int resource; /* +0x20 */
    int res36; /* +0x24 */
    unsigned char pad_28[0x4];
    short firstFrame; /* +0x2c */
    unsigned char pad_2e[0x2];
    int spriteRecord; /* +0x30 */
    int secondDrawObj; /* +0x34 */
    unsigned char pad_38[0x4];
    short secondFrame; /* +0x3c */
    unsigned char pad_3e[0x2];
} BattleSpecMissileSalvoTransformMatrix;
#pragma pack(pop)

/* --- battle_special_saikyo_effects --- */
/* Timeline record of the Saikyo special, allocated from the effect slot array. */
typedef struct BattleFxUltimateBlastTimeline2 {
    unsigned char pad_00[0x4];
    short posX;
    short initialOffset;
    short posZ;
} BattleFxUltimateBlastTimeline2;

/* Task slot of BattleFx_SpecialUltimateBlastTimelineTask as filled by BattleFx_SpecialBeginUltimateBlast: the caster bone position and the target centroid. */
typedef struct BattleFxUltimateBlastTimelineStart {
    unsigned char pad_00[0x4];
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short targetX;
    short targetY;
    short targetZ;
} BattleFxUltimateBlastTimelineStart;

/* Task slot of BattleFx_SpecialUltimateBlastAnimatedSpriteTask: frame counter and position. */
typedef struct BattleFxUltimateBlastAnimatedSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
} BattleFxUltimateBlastAnimatedSpriteSlot;

/* The battle task slot of BattleFx_SpecialUltimateBlastDriftingSpriteTask. */
typedef struct BattleFxUltimateBlastDriftingSpriteSlot {
    short ownerSlot;
    short frameCounter;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x2];
    short velocityX;
    short velocityY;
    short velocityZ;
    unsigned char pad_12[0x4];
    short animIndex;
} BattleFxUltimateBlastDriftingSpriteSlot;

#pragma pack(push, 1)
typedef struct BattleSpecUltimateBlastBuffer {
    short rotX; /* +0x0 */
    short scaledShort; /* +0x2 */
} BattleSpecUltimateBlastBuffer;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecUltimateBlastAnimData4 {
    unsigned char pad_0[0x18];
    int spriteField; /* +0x18 */
    int spriteAlt; /* +0x1c */
} BattleSpecUltimateBlastAnimData4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecUltimateBlastAnimData6 {
    unsigned char pad_0[0x8];
    int spriteField; /* +0x8 */
    int spriteAlt; /* +0xc */
} BattleSpecUltimateBlastAnimData6;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BattleSpecUltimateBlastSpriteRequest2 {
    char red; /* +0x0 */
    char request2Byte1; /* +0x1 */
    char request2Byte2; /* +0x2 */
    unsigned char pad_3[0x1];
} BattleSpecUltimateBlastSpriteRequest2;
#pragma pack(pop)

/* The rising fade sprite's colour and frame, then the head of the expanding shells' draw request. */
#pragma pack(push, 1)
typedef struct {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
    short frameIndex; /* +0x4 */
    unsigned char pad_6[0x2];
    int modelGroup; /* +0x8 */
    unsigned char pad_c[0x6];
    short fadeLevel; /* +0x12 */
} BattleSpecUltimateBlastSpriteRequest3;
#pragma pack(pop)

/* The animated sprite's request (its colour in pad_8), then the head of the rising spin model's
 * draw request. */
#pragma pack(push, 1)
typedef struct {
    struct BattleFxSpriteGroup *spriteGroup; /* +0x0 */
    int drawObj; /* +0x4 */
    unsigned char pad_8[0x4];
    short frame; /* +0xc */
    unsigned char pad_e[0x2];
    int modelGroup; /* +0x10 */
    unsigned char pad_14[0x6];
    short fadeLevel; /* +0x1a */
} BattleSpecUltimateBlastTransformData1;
#pragma pack(pop)

/* --- battle_special_spade_second_effects --- */
/* Spade2 bone particle sprite: steps through an alpha table as its index counts up. */
typedef struct BattleFxEmperorCardBoneParticleSprite {
    short ownerSlot;
    short alphaIndex;
    short posX;
    short posY;
    short posZ;
    unsigned char pad_0A[0x3];
    char cellY;
    short cellWord;
    short scale;
    unsigned char pad_12[0x4];
    short flags;
} BattleFxEmperorCardBoneParticleSprite;

/* --- battle_status_effect_handlers --- */
/* Per-actor marker byte set to 0xFF when the actor resumes after status. */
typedef struct BattleActorResumeMarker {
    unsigned char marker;
    unsigned char pad_01[0x7];
} BattleActorResumeMarker;

/* --- enemy --- */
/* Enemy model bank: a header pointer and a per-part pointer table from the enemy DAT. */
typedef struct Battle3dEnemyModelBank {
    unsigned char pad_00[0x4];
    void *header;
    unsigned char pad_08[0x4];
    void *partTable;
    unsigned char pad_10[0xc];
    void *weaponSkeletons;
} Battle3dEnemyModelBank;

/* --- mdl --- */
/* One bone's state in a battle3d model skeleton. */
typedef struct Battle3dSkeletonState {
    int name;
    int parentName;
    int lastIndex;
    float boneOffset;
    int active;
    int childTable;
    struct Battle3dSkeletonPartBuffer *buffer;
} Battle3dSkeletonState;

/* The 8-byte block allocated for a bone that has a part: the registered part follows a reserved word. */
typedef struct Battle3dSkeletonPartBuffer {
    int reserved;
    void *part;
} Battle3dSkeletonPartBuffer;

/* Skeleton built by BattleModel_BuildSkeletonFromLmd; one Battle3dSkeletonState per bone. */
typedef struct Battle3dSkeletonRoot {
    unsigned char pad_00[0x10];
    int boneCount;
    Battle3dSkeletonState *skeletonState;
} Battle3dSkeletonRoot;

/* Damage number bone-space X/Y coordinates paired with g_BattleDamageNumberPosZ. */
typedef struct BattleDamageNumberPos {
    short x;
    short y;
} BattleDamageNumberPos;

#endif /* FF7_TYPES_BATTLE_ACTORS_H */

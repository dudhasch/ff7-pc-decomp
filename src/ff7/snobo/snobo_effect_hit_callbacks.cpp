// src/ff7/snobo/snobo_effect_hit_callbacks.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The snowboard effect callbacks that run after a hit: they pick the next update callback
//   from the record's flags, play the hit sound, and drive sine sways, fade-outs, ballistic arcs,
//   spin cycles and drops. It also holds the free-record search and the animation phase helpers.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: Trigger handlers for the snowboard's alternate event slots: when a trigger is set, each
//   one spawns one or more effect nodes tagged to a rider's position or velocity. Some also enter
//   the rider's alternate state and play a sound, and shared helpers stage the effect definitions
//   these nodes use.
// Also: The alternate snowboard event-slot handlers, which spawn effect nodes and emit sprite
//   primitives when their enable flag is set. They share one sprite-effect template and keep the
//   alternate event visit flags and effect records.
#include "ff7/types/snobo.h"
#include "ff7/api/snobo.h"
#include "lib/api/graphics.h"
#include <stdlib.h>
#include "ff7/enums/snobo.h"
#include "lib/types/graphics.h"
extern "C" {

// clang-format off
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00964ce8
SnowboardEffectTemplate g_SnoboEffectTemplateOnHit = {0, 0, 0, 0, 0x0, 0x0, 96, 256, 192, {0x00, 0x00, 0x80, 0x80}, 0, 0, 0, 0, 0x0, 0x0, 0x0, 0x0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964d30
SnowboardEffectTemplate g_SnoboEffectTemplatePlain = {0, 0, 1, 0, 0, 0, 152, 512, 448, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectSelectHitCallbackAndSway, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964d78
SnowboardEffectTemplate g_SnoboEffectDefDescriptor = {0, 0, 0, 0, 0, 0, 160, 64, 0, {0x00, 0x00, 0x0A, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectArmOnHitThenFadeOut, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964dc0
SnowboardEffectTemplate g_SnoboTaggedPositionEffectDef = {0xF4, 0, 0, 0, 0, 0, 416, 224, 192, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectOnHitSplitFallFadeOut, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964e08
SnowboardEffectTemplate g_SnoboSelectorEffectDef = {0, 0, 0, 0, 1, 0, 64, 320, 256, {0x00, 0x00, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectOnHitSplitTwoChildren, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964e50
SnowboardEffectTemplate g_SnoboTemplateIdEffectDef = {0, 0, 0, 0, 1, 0, 40, 320, 256, {0x00, 0x00, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectOnHitSelectBallisticOrFadeOut, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964e98
SnowboardEffectTemplate g_SnoboSelectedRecordEffectDef = {0, 0, 0, 0, 0, 0, 208, 128, 64, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectHitSfxSelectFadeOut, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964ee0
SnowboardEffectTemplate g_SnoboFixedTagEffectDef = {0x103, 0, 0, 0, 0, 0, 192, 448, 416, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectHitSfxSelectFadeOut, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964f28
SnowboardEffectTemplate g_SnoboVelocityEffectDef = {0xF0, 0, 0, 0, 2, 0, 128, 192, 0, {0xFF, 0xFF, 0xFF, 0xFF}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardPickupBobOrCollect, 0, 0, 0, 0};
// g_SnoboPickupPointsByKind holds the points a collected pickup adds to pickupTotal, indexed by
// its pickupKind (Mini_SnowboardPickupBobOrCollect).
// GLOBAL: FF7 0x00964f70
short g_SnoboPickupPointsByKind[8] = {1, 3, 5, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964f80
SnowboardEffectTemplate g_SnoboKindAngleEffectDef = {0, 0, 0, 0, 2, 0, 64, 512, 448, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectOnHitSelectGrowOrFade, 0, 0, 0, 0};
// GLOBAL: FF7 0x00964fc8
SnowboardEffectTemplate g_SnoboTagVelocityFieldEffectDef = {0x104, 0x107, 0, 0, 0, 0, 80, 128, 0, {0x00, 0x00, 0x80, 0x80}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateSweepTargetReverse, 0, 0, 0, 0};
// g_SnoboEffectCycleFrames holds the six model2 frames a sweeping effect cycles through
// (Mini_SnowboardGetEffectCycleFrame).
// GLOBAL: FF7 0x00965010
short g_SnoboEffectCycleFrames[8] = {262, 263, 264, 262, 265, 266, 0, 0};
// GLOBAL: FF7 0x00965020
SnowboardEffectTemplate g_SnoboTagVelocityEffectDef = {0x104, 0x106, 0, 0, 0, 0, 80, 128, 0, {0x00, 0x00, 0x80, 0x80}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectSelectApproachOrFade, 0, 0, 0, 0};
// GLOBAL: FF7 0x00965068
SnowboardEffectTemplate g_SnoboPositionAngleEffectDef = {0x105, 0x10B, 0, 0, 0, 0, 0, 0, 0, {0x00, 0x00, 0x00, 0x00}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectFallFadeOut, 0, 0, 0, 0};
// GLOBAL: FF7 0x009650b0
SnowboardEffectTemplate g_SnoboSpritePrimEffectDef = {0, 0, 0, 0, 0, 0, 0, 0, 0, {0x00, 0x00, 0x0C, 0x0C}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectStartDropFadeInWhenNear, 0, 0, 0, 0};
// GLOBAL: FF7 0x009650f8
SnowboardEffectTemplate g_SnoboSelectorOffsetEffectDef = {0x10E, 0, 0, 0, 0, 0, 0, 128, 128, {0x00, 0x00, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectWaitThenStartBounce, 0, 0, 0, 0};
// g_SnoboSelectorEffectSpeeds holds the side and forward speed of a selector offset effect,
// indexed by its selector (Mini_SnowboardSpawnSelectorOffsetEffectNode).
// GLOBAL: FF7 0x00965140
short g_SnoboSelectorEffectSpeeds[4] = {-16, 16, -20, 20};
// GLOBAL: FF7 0x00965148
SnowboardEffectTemplate g_SnoboIndexedEffectDef = {0x10E, 0, 0, 0, 0, 0, 0, 128, 128, {0x0C, 0x0C, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectWaitThenStartSpin, 0, 0, 0, 0};
// g_SnoboIndexedEffectVelocities holds the initial side, fall and forward velocities of the indexed effects.
// GLOBAL: FF7 0x00965190
struct SnowboardVec g_SnoboIndexedEffectVelocities[4] = {{-0x114C0, 0x1FA0, 0x50C0}, {0x2080, 0x1FA0, -0x11F40}, {-0x14E50, 0x310D, -0x12F2}, {-0x4C35, 0x3143, -0x14779}};

// The indexed effects' tables: their rotY angle, their spin lifetime (by index >> 1) and their
// rotX step per frame (Mini_SnowboardSpawnIndexedEffectNode and its update callbacks).
// GLOBAL: FF7 0x009651c0
short g_SnoboIndexedEffectAngles[4] = {1144, -8, 888, 248};
// GLOBAL: FF7 0x009651c8
short g_SnoboIndexedEffectLifetimes[4] = {128, 152, 0, 0};
// GLOBAL: FF7 0x009651d0
short g_SnoboIndexedEffectSpinSteps[4] = {72, 72, 96, 96};
// GLOBAL: FF7 0x009651d8
SnowboardEffectTemplate g_SnoboEffectTemplateTyped = {0, 0, 0, 0, 0x2, 0x0, 0, 0, 128, {0xFF, 0xFF, 0xFF, 0xFF}, 0, 0, 0, 0, 0x0, 0x0, 0x0, 0x0, 0, 0, 0, 0, 0, 0};
// g_SnoboTypedEffectModels holds the models of the typed table effects.
// GLOBAL: FF7 0x00965220
short g_SnoboTypedEffectModels[4] = {271, 0, 0, 0};
// GLOBAL: FF7 0x00965228
struct SnowboardSpawnPos g_SnoboAltSlot008EffectPos0 = {0x300000, 0xF74000, -0x308F000};
// GLOBAL: FF7 0x00965238
struct SnowboardVec g_SnoboAltSlot015EffectPos0 = {-0x1191000, 0x2dbe000, -0x5939000};
// GLOBAL: FF7 0x00965248
struct SnowboardSpawnPos g_SnoboAltSlot016EffectPos0 = {-0x18CC000, 0x3270000, -0x59EF000};
// GLOBAL: FF7 0x00965258
struct SnowboardSpawnPos g_SnoboAltSlot020EffectPos0 = {-0x34B1000, 0x426A000, -0x5B1A000};
// GLOBAL: FF7 0x00965268
struct SnowboardSpawnPos g_SnoboAltSlot026EffectPos0 = {-0x437C000, 0x5470000, -0x74E5000};
// GLOBAL: FF7 0x00965278
struct SnowboardVec g_SnoboAltSlot031EffectPos0[2] = {{-0x31ac000, 0x6126000, -0x83fb000}, {-0x31f4000, 0x6126000, -0x8705000}};
// GLOBAL: FF7 0x00965290
struct SnowboardSpawnPos g_SnoboAltSlot032EffectPos0[3] = {{-0x2F0D000, 0x61BF000, -0x859F000}, {-0x2D57000, 0x61EE000, -0x86C3000}, {-0x2D14000, 0x61FC000, -0x84CA000}};
// GLOBAL: FF7 0x009652b8
struct SnowboardSpawnPos g_SnoboAltSlot040EffectPos0[2] = {{-0xEBA000, 0x679D000, -0x9C4E000}, {-0xECB000, 0x679A000, -0x9DDD000}};
// GLOBAL: FF7 0x009652d0
struct SnowboardVec g_SnoboAltSlot040EffectPos1 = {-0xea4000, 0x6798000, -0x9ace000};
// GLOBAL: FF7 0x009652e0
struct SnowboardVec g_SnoboAltSlot048EffectPos0[2] = {{0xc55000, 0x705a000, -0x8c9d000}, {0xd33000, 0x7065000, -0x88ad000}};
// GLOBAL: FF7 0x009652f8
struct SnowboardSpawnPos g_SnoboAltSlot048EffectPos1 = {0xCC7000, 0x705C000, -0x8AA2000};
// GLOBAL: FF7 0x00965308
struct SnowboardSpawnPos g_SnoboAltSlot050EffectPos0 = {0x1585000, 0x718C000, -0x926C000};
// GLOBAL: FF7 0x00965318
struct SnowboardSpawnPos g_SnoboAltSlot056EffectPos0 = {0x1C4F000, 0x7E4D000, -0xAF92000};
// GLOBAL: FF7 0x00965328
struct SnowboardSpawnPos g_SnoboAltSlot058EffectPos0 = {0x134B000, 0x8218000, -0xB434000};
// GLOBAL: FF7 0x00965338
struct SnowboardVec g_SnoboAltSlot061EffectPos0 = {0x818000, 0x85f5000, -0xaf33000};
// GLOBAL: FF7 0x00965348
struct SnowboardVec g_SnoboAltSlot063EffectPos0 = {0x7000, 0x8965000, -0xa69b000};
// GLOBAL: FF7 0x00965358
struct SnowboardSpawnPos g_SnoboAltSlot066EffectPos0 = {-0x57A000, 0x8DA2000, -0xB06A000};
// GLOBAL: FF7 0x00965368
struct SnowboardVec g_SnoboAltSlot073EffectPos0 = {-0x17f4000, 0x9883000, -0xb35f000};
// GLOBAL: FF7 0x00965378
struct SnowboardVec g_SnoboAltSlot074EffectPos0 = {-0x1c8f000, 0x9a14000, -0xb1bb000};
// GLOBAL: FF7 0x00965388
struct SnowboardVec g_SnoboAltSlot075EffectPos0 = {-0x20cc000, 0x9b75000, -0xb246000};
// GLOBAL: FF7 0x00965398
struct SnowboardVec g_SnoboAltSlot076EffectPos0 = {-0x23c2000, 0x9cd7000, -0xb46b000};
// GLOBAL: FF7 0x009653a8
struct SnowboardSpawnPos g_SnoboAltSlot079EffectPos0 = {-0x2938000, 0xA1AA000, -0xC037000};
// GLOBAL: FF7 0x009653b8
struct SnowboardSpawnPos g_SnoboAltSlot085EffectPos0 = {-0x3B62000, 0xAA14000, -0xC04D000};
// GLOBAL: FF7 0x009653c8
struct SnowboardSpawnPos g_SnoboAltSlot115EffectPos0 = {-0x20D1000, 0xD70C000, -0xF12B000};
// GLOBAL: FF7 0x009653d8
struct SnowboardSpawnPos g_SnoboAltSlot132EffectPos0[2] = {{-0x32DC000, 0xEBC0000, -0x10D13000}, {-0x350C000, 0xEC80000, -0x10F3D000}};
// GLOBAL: FF7 0x009653f0
struct SnowboardSpawnPos g_SnoboAltSlot133EffectPos0 = {-0x36A0000, 0xEE10000, -0x110D7000};
// GLOBAL: FF7 0x00965400
struct SnowboardSpawnPos g_SnoboAltSlot138EffectPos0 = {-0x4DC0000, 0xF8F6000, -0x127FA000};
// GLOBAL: FF7 0x00965410
struct SnowboardSpawnPos g_SnoboAltSlot146EffectPos0 = {-0x6EF7000, 0x10CFD000, -0x1492E000};
// GLOBAL: FF7 0x00965420
struct SnowboardSpawnPos g_SnoboAltSlot148EffectPos0 = {-0x797E000, 0x10F30000, -0x153B5000};
// GLOBAL: FF7 0x00965430
struct SnowboardSpawnPos g_SnoboAltSlot156EffectPos0 = {-0x9B8A000, 0x128AA000, -0x175C2000};
// GLOBAL: FF7 0x00965440
struct SnowboardVec g_SnoboAltSlot161EffectPos0[2] = {{-0xaa72000, 0x12ed8000, -0x1898a000}, {-0xaf56000, 0x12ed8000, -0x184b1000}};
// GLOBAL: FF7 0x00965458
struct SnowboardSpawnPos g_SnoboAltSlot163EffectPos0 = {-0xB73C000, 0x13061000, -0x19174000};
// GLOBAL: FF7 0x00965468
struct SnowboardCyclingEffectOffset g_SnoboCyclingQuadOffsets[16] = {{712, 64, 370}, {2150, 196, 1105}, {1680, 296, 104}, {1426, 108, 844}, {1457, 45, 1214}, {603, 111, 10}, {2866, 367, 915}, {1971, 197, 922}, {915, 66, 554}, {1786, 248, 464}, {2354, 200, 1285}, {1705, 35, 1561}, {2069, 32, 1889}, {966, 297, -605}, {1058, 126, 385}, {2203, 306, 577}};
// GLOBAL: FF7 0x009654c8
short g_SnoboCyclingQuadDisplacements[5] = {0, 24, -24, 48, -48};
// GLOBAL: FF7 0x009654d8
struct SnowboardSpawnPos g_SnoboAltSlot236EffectPos0 = {-0x194E0000, 0x1A1FA000, -0x2CF96000};
// GLOBAL: FF7 0x009654e8
struct SnowboardSpawnPos g_SnoboAltSlot239EffectPos0 = {-0x1815E000, 0x1A7DD000, -0x2DB24000};
// GLOBAL: FF7 0x009654f8
struct SnowboardSpawnPos g_SnoboAltSlot243EffectPos0 = {-0x17619000, 0x1ACE2000, -0x2EA62000};
// GLOBAL: FF7 0x00965508
struct SnowboardSpawnPos g_SnoboAltSlot248EffectPos0 = {-0x162B9000, 0x1B488000, -0x2FF71000};
// GLOBAL: FF7 0x00965518
struct SnowboardSpawnPos g_SnoboAltSlot252EffectPos0 = {-0x14E27000, 0x1BB3E000, -0x30E5B000};
// GLOBAL: FF7 0x00965528
struct SnowboardSpawnPos g_SnoboAltSlot256EffectPos0 = {-0x13EE2000, 0x1C258000, -0x3249B000};
// GLOBAL: FF7 0x00965538
struct SnowboardVec g_SnoboAltSlot265EffectPos0[2] = {{-0x12c2e000, 0x1cda7000, -0x346e8000}, {-0x12ac4000, 0x1cda7000, -0x34859000}};
// GLOBAL: FF7 0x00965550
struct SnowboardSpawnPos g_SnoboAltSlot265EffectPos1 = {-0x12B71000, 0x1CDA7000, -0x3479F000};
// GLOBAL: FF7 0x00965560
struct SnowboardVec g_SnoboAltSlot269EffectPos0 = {-0x13c8f000, 0x1d1fd000, -0x34d28000};
// GLOBAL: FF7 0x00965570
struct SnowboardVec g_SnoboAltSlot270EffectPos0[2] = {{-0x13fc1000, 0x1d286000, -0x34bd6000}, {-0x13d72000, 0x1d26f000, -0x349f8000}};
// GLOBAL: FF7 0x00965588
struct SnowboardSpawnPos g_SnoboAltSlot270EffectPos1 = {-0x13E00000, 0x1D25C000, -0x34BAB000};
// GLOBAL: FF7 0x00965598
struct SnowboardSpawnPos g_SnoboAltSlot277EffectPos0 = {-0x14D2D000, 0x1D9AF000, -0x325CD000};
// GLOBAL: FF7 0x009655a8
struct SnowboardSpawnPos g_SnoboAltSlot301EffectPos0 = {-0x1710A000, 0x1EAF0000, -0x2E0F8000};
// GLOBAL: FF7 0x009655b8
struct SnowboardSpawnPos g_SnoboAltSlot316EffectPos0[2] = {{-0x1c4c8000, 0x2004f000, -0x2da74000}, {-0x1c2ce000, 0x2004f000, -0x2dcc7000}};
// GLOBAL: FF7 0x009655d0
struct SnowboardSpawnPos g_SnoboAltSlot317EffectPos0 = {-0x1c675000, 0x20136000, -0x2e1c8000};
// GLOBAL: FF7 0x009655e0
struct SnowboardSpawnPos g_SnoboAltSlot318EffectPos1 = {-0x1cca4000, 0x2018c000, -0x2de42000};
// GLOBAL: FF7 0x009655f0
struct SnowboardVec g_SnoboAltSlot318EffectPos0 = {-0x1c72c000, 0x20163000, -0x2e347000};
// GLOBAL: FF7 0x00965600
struct SnowboardSpawnPos g_SnoboAltSlot319EffectPos0 = {-0x1cf6f000, 0x20330000, -0x2e9b0000};
// GLOBAL: FF7 0x00965610
struct SnowboardVec g_SnoboAltSlot319EffectPos1 = {-0x1cd7d000, 0x201b7000, -0x2ded3000};
// GLOBAL: FF7 0x00965620
struct SnowboardSpawnPos g_SnoboAltSlot320EffectPos0 = {-0x1d5d5000, 0x2039f000, -0x2e658000};
// GLOBAL: FF7 0x00965630
struct SnowboardVec g_SnoboAltSlot320EffectPos1 = {-0x1d047000, 0x20361000, -0x2eafe000};
// GLOBAL: FF7 0x00965640
struct SnowboardSpawnPos g_SnoboAltSlot321EffectPos0 = {-0x1d8cb000, 0x20536000, -0x2f130000};
// GLOBAL: FF7 0x00965650
struct SnowboardVec g_SnoboAltSlot321EffectPos1 = {-0x1d764000, 0x203dd000, -0x2e724000};
// GLOBAL: FF7 0x00965660
struct SnowboardSpawnPos g_SnoboAltSlot322EffectPos0 = {-0x1e084000, 0x205e5000, -0x2ef08000};
// GLOBAL: FF7 0x00965670
struct SnowboardVec g_SnoboAltSlot322EffectPos1 = {-0x1d97c000, 0x20562000, -0x2f2ac000};
// GLOBAL: FF7 0x00965680
struct SnowboardSpawnPos g_SnoboAltSlot323EffectPos0 = {-0x1deda000, 0x20688000, -0x2f65b000};
// GLOBAL: FF7 0x00965690
struct SnowboardVec g_SnoboAltSlot323EffectPos1 = {-0x1e238000, 0x2063e000, -0x2f052000};
// GLOBAL: FF7 0x009656a0
struct SnowboardVec g_SnoboAltSlot324EffectPos0 = {-0x1de73000, 0x20677000, -0x2f6e0000};
// GLOBAL: FF7 0x009656b0
struct SnowboardSpawnPos g_SnoboAltSlot325EffectPos0 = {-0x1eb7d000, 0x208fe000, -0x2fd0f000};
// GLOBAL: FF7 0x009656c0
struct SnowboardSpawnPos g_SnoboAltSlot326EffectPos0[2] = {{-0x1ee2d000, 0x209ca000, -0x301ad000}, {-0x1f077000, 0x209cb000, -0x2fef4000}};
// GLOBAL: FF7 0x009656d8
struct SnowboardSpawnPos g_SnoboAltSlot327EffectPos0[2] = {{-0x1f10a000, 0x20a83000, -0x30615000}, {-0x1f559000, 0x20a7f000, -0x300e0000}};
// GLOBAL: FF7 0x009656f0
struct SnowboardSpawnPos g_SnoboAltSlot329EffectPos0[2] = {{-0x1fa6b000, 0x20c8b000, -0x30d88000}, {-0x1ffbf000, 0x20ce0000, -0x30a3c000}};
// GLOBAL: FF7 0x00965708
struct SnowboardVec g_SnoboAltSlot330EffectPos0 = {-0x1fa4a000, 0x20c8b000, -0x30e5d000};
// GLOBAL: FF7 0x00965718
struct SnowboardSpawnPos g_SnoboAltSlot331EffectPos1[2] = {{-0x20770000, 0x20e8a000, -0x3109d000}, {-0x206a0000, 0x20f35000, -0x317d0000}};
// GLOBAL: FF7 0x00965730
struct SnowboardVec g_SnoboAltSlot331EffectPos0 = {-0x2016a000, 0x20d0a000, -0x30a88000};
// GLOBAL: FF7 0x00965740
struct SnowboardVec g_SnoboAltSlot332EffectPos0[2] = {{-0x208bd000, 0x20ea6000, -0x310c4000}, {-0x20760000, 0x20f62000, -0x3194c000}};
// GLOBAL: FF7 0x00965758
struct SnowboardSpawnPos g_SnoboAltSlot333EffectPos0 = {-0x210a1000, 0x2108d000, -0x31864000};
// GLOBAL: FF7 0x00965768
struct SnowboardSpawnPos g_SnoboAltSlot334EffectPos1 = {-0x212a1000, 0x211d6000, -0x32264000};
// GLOBAL: FF7 0x00965778
struct SnowboardVec g_SnoboAltSlot334EffectPos0 = {-0x21242000, 0x210ba000, -0x318c8000};
// GLOBAL: FF7 0x00965788
struct SnowboardVec g_SnoboAltSlot335EffectPos1 = {-0x21352000, 0x21201000, -0x3236b000};
// GLOBAL: FF7 0x00965798
struct SnowboardSpawnPos g_SnoboAltSlot335EffectPos0 = {-0x21c87000, 0x21322000, -0x3225a000};
// GLOBAL: FF7 0x009657a8
struct SnowboardVec g_SnoboAltSlot336EffectPos0[2] = {{-0x21455000, 0x21238000, -0x32429000}, {-0x21db4000, 0x21337000, -0x32268000}};
// GLOBAL: FF7 0x009657c0
struct SnowboardSpawnPos g_SnoboAltSlot337EffectPos0 = {-0x2241c000, 0x21556000, -0x32ca9000};
// GLOBAL: FF7 0x009657d0
struct SnowboardSpawnPos g_SnoboAltSlot338EffectPos0[2] = {{-0x227d5000, 0x215c0000, -0x32cd3000}, {-0x224eb000, 0x215c1000, -0x3304b000}};
// GLOBAL: FF7 0x009657e8
struct SnowboardSpawnPos g_SnoboAltSlot340EffectPos0[2] = {{-0x2328a000, 0x21816000, -0x335d3000}, {-0x22fa1000, 0x21816000, -0x3394b000}};
// GLOBAL: FF7 0x00965800
struct SnowboardSpawnPos g_SnoboAltSlot342EffectPos0 = {-0x2397c000, 0x219ff000, -0x33ea7000};
// GLOBAL: FF7 0x00965810
struct SnowboardVec g_SnoboAltSlot342EffectPos1[2] = {{-0x233c9000, 0x21832000, -0x335e3000}, {-0x22fe4000, 0x21833000, -0x33a87000}};
// GLOBAL: FF7 0x00965828
struct SnowboardSpawnPos g_SnoboAltSlot344EffectPos0[2] = {{-0x24033000, 0x21ba6000, -0x3463f000}, {-0x24220000, 0x21ba5000, -0x343f3000}};
// GLOBAL: FF7 0x00965840
struct SnowboardSpawnPos g_SnoboAltSlot346EffectPos0[2] = {{-0x248c5000, 0x21d83000, -0x34ea7000}, {-0x24be3000, 0x21d83000, -0x34af2000}};
// GLOBAL: FF7 0x00965858
struct SnowboardEffectSpawn g_SnoboAltSlot358EffectPos0 = {-0x283A0000, 0x22913000, -0x346D8000, -0x283A0000, -0x27FD3000, 0xC3E0, 0xA4D0, 1};
// GLOBAL: FF7 0x00965878
struct SnowboardEffectSpawn g_SnoboAltSlot359EffectPos0 = {-0x283A9000, 0x22994000, -0x33F0C000, -0x2877C000, -0x283A9000, -0xC3E0, -0xA4D0, 0};
// GLOBAL: FF7 0x00965898
struct SnowboardEffectSpawn g_SnoboAltSlot360EffectPos0 = {-0x28AFA000, 0x22A49000, -0x33D57000, -0x28AFA000, -0x288AE000, 0xC3E0, 0xA4D0, 1};
// GLOBAL: FF7 0x009658b8
struct SnowboardEffectSpawn g_SnoboAltSlot361EffectPos0 = {-0x28BCE000, 0x22B93000, -0x33632000, -0x28E13000, -0x28BCE000, -0xC3E0, -0xA4D0, 0};
// GLOBAL: FF7 0x009658d8
struct SnowboardEffectSpawn g_SnoboAltSlot362EffectPos0 = {-0x290A4000, 0x22C90000, -0x33517000, -0x290A4000, -0x28F1C000, 0xC3E0, 0xA4D0, 1};
// GLOBAL: FF7 0x009658f8
struct SnowboardEffectSpawn g_SnoboAltSlot363EffectPos0 = {-0x29360000, 0x22DFA000, -0x32F7C000, -0x29360000, -0x2923A000, 0xC3E0, 0xA4D0, 1};
// GLOBAL: FF7 0x00965918
struct SnowboardEffectSpawn g_SnoboAltSlot363EffectPos1 = {-0x29423000, 0x22DFA000, -0x33021000, -0x2954F000, -0x29423000, -0xC3E0, -0xA4D0, 0};
// GLOBAL: FF7 0x00ddc1a0
short g_SnoboEffectNodeCursor = 0;
unsigned char g_bssPad_00ddc1a4[4] = {0};
// GLOBAL: FF7 0x00ddc1a8
char g_SnoboAltEventVisitFlags[604] = {0};
unsigned char g_bssPad_00ddc404[4] = {0};
// GLOBAL: FF7 0x00ddc408
struct SnowboardSpawnPos g_SnoboAltPairSpawnPos1 = {0};
unsigned char g_bssPad_00ddc414[4] = {0};
// GLOBAL: FF7 0x00ddc418
struct SnowboardSpawnPos g_SnoboAltTemplateF0Pos1 = {0};
unsigned char g_bssPad_00ddc424[4] = {0};
// GLOBAL: FF7 0x00ddc428
struct SnowboardSpawnPos g_SnoboModelE6Pos = {0};
unsigned char g_bssPad_00ddc434[4] = {0};
// GLOBAL: FF7 0x00ddc438
struct SnowboardSpawnPos g_SnoboAltPairSpawnPos2 = {0};
unsigned char g_bssPad_00ddc444[4] = {0};
// GLOBAL: FF7 0x00ddc448
struct SnowboardVec g_SnoboAltTableTypedPos1 = {0};
unsigned char g_bssPad_00ddc454[4] = {0};
// GLOBAL: FF7 0x00ddc458
struct SnowboardSpawnPos g_SnoboAltTemplateF0Pos2 = {0};
unsigned char g_bssPad_00ddc464[4] = {0};
// GLOBAL: FF7 0x00ddc468
struct SnowboardSpawnPos g_SnoboAltTemplateF0Pos3 = {0};
unsigned char g_bssPad_00ddc474[4] = {0};
// GLOBAL: FF7 0x00ddc478
SnowboardEffect g_SnoboAltEffectRecords[32] = {0};
// GLOBAL: FF7 0x00ddd678
struct SnowboardSpawnPos g_SnoboAltTemplateF0Pos4 = {0};
// GLOBAL: FF7 0x00ddd684
short g_SnoboLastSignalValue = 0;
// GLOBAL: FF7 0x00ddd688
struct SnowboardVec g_SnoboAltTableTypedPos2 = {0};
// GLOBAL: FF7 0x00ddd694
short g_SnoboEffectRecordCursor = 0;
// GLOBAL: FF7 0x00ddd698
short g_SnoboSfx1ABLastTick = 0;
// GLOBAL: FF7 0x00ddd69c
short g_SnoboSfx1ABLastStrength = 0;
unsigned char g_bssPad_00ddd6a0[4] = {0};
// GLOBAL: FF7 0x00ddd6a4
short g_SnoboCyclingQuadIndex = 0;
// clang-format on

#pragma intrinsic(abs)

/*
 * For a nonzero contact, bit 2 clears contactRadius, plays channel-3 sound 0xF0 and selects Mini_SnowboardEffectSineSwayThenRetire;
 * otherwise selects Mini_SnowboardEffectFadeOutWithSway. Calls Mini_SnowboardEffectSetSineSway when model equals 0xEC.
 */
// FUNCTION: FF7 0x0073bb60
void __cdecl Mini_SnowboardEffectSelectHitCallbackAndSway(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(3, 0xF0, 0);
            effect->update = Mini_SnowboardEffectSineSwayThenRetire;
        } else {
            effect->update = Mini_SnowboardEffectFadeOutWithSway;
        }
    }
    if (effect->model - 0xEA == 2) {
        Mini_SnowboardEffectSetSineSway(effect);
    }
}

/*
 * Writes a nested fixed-point sine of the signed age to rotX and clears matrixValid. Then post-
 * increments age; only after its consumed value exceeds 120 does it post-increment unsigned fade
 * and clear the active byte if the consumed fade exceeds 30.
 */
// FUNCTION: FF7 0x0073bbce
void __cdecl Mini_SnowboardEffectSineSwayThenRetire(SnowboardEffect *effect)
{
    effect->rotX = -SinFixed(SinFixed(effect->age << 5) >> 4) >> 3;
    effect->matrixValid = 0;
    if ((effect->age)++ > 120 && (effect->fade)++ > 30)
        effect->active = 0;
}

/*
 * Post-increments signed age and, only after its consumed value exceeds 60, unsigned fade; clears
 * the active byte when consumed fade exceeds 30. Regardless of retirement, calls
 * Mini_SnowboardEffectSetSineSway(effect) when model equals 0xec.
 */
// FUNCTION: FF7 0x0073bc60
void __cdecl Mini_SnowboardEffectFadeOutWithSway(SnowboardEffect *effect)
{
    if ((effect->age)++ > 60 && (effect->fade)++ > 30)
        effect->active = 0;
    if (effect->model - 0xea == 2)
        Mini_SnowboardEffectSetSineSway(effect);
}

// Sets swayOffset from nested SinFixed calls. phase is post-incremented, SinFixed of its old value
// shifted left 5 is shifted right 4 and passed to a second SinFixed, and that result shifted right
// 3 becomes swayOffset. matrixValid is cleared.
// FUNCTION: FF7 0x0073bcd9
void __cdecl Mini_SnowboardEffectSetSineSway(SnowboardEffect *effect)
{

    effect->rotZ = SinFixed(SinFixed(((effect->timer)++ << 5)) >> 4) >> 3;
    effect->matrixValid = 0;
}

/*
 * When signed age is nonzero, post-increments it and conditionally post-increments
 * unsigned fade after consumed age exceeds 60, clearing active when consumed fade exceeds 30. At
 * age zero, a nonzero contact arms age to 1; flag bit 1 additionally clears contactRadius and plays
 * sound 0x10c on channel 2.
 */
// FUNCTION: FF7 0x0073bd34
void __cdecl Mini_SnowboardEffectArmOnHitThenFadeOut(SnowboardEffect *effect)
{
    if (effect->age != 0) {
        if ((effect->age)++ > 60 && (effect->fade)++ > 30)
            effect->active = 0;
    } else if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(2, 0x10c, 0);
        }
        effect->age = 1;
    }
}

/*
 * On collision bit two changes velocities, mesh and callback and plays a sound, then allocates up
 * to two full 144-byte copies with rotated velocity pairs and incremented mesh IDs, returning
 * early on either allocation failure. Retains vertical-velocity writes to the original between
 * copies and optionally spawns another effect from a timer nibble. Other collision selects a
 * different callback.
 */
// FUNCTION: FF7 0x0073bde0
void __cdecl Mini_SnowboardEffectOnHitSplitFallFadeOut(SnowboardEffect *effect)
{
    SnowboardEffect *newRecord;

    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(3, 0xF0, 0);
            effect->contactRadius = 0;
            effect->sideVelocity = 0xFFFFDB40;
            effect->fallVelocity = 0xFFFDC000;
            effect->forwardVelocity = 0xFFFFCB80;
            (effect->model)++;
            effect->update = Mini_SnowboardEffectFallFadeOut;
            newRecord = Mini_SnowboardFindFreeEffectRecord();
            if (newRecord == 0) {
                return;
            }
            *newRecord = *effect;
            newRecord->sideVelocity = -effect->forwardVelocity;
            effect->fallVelocity = 0xFFFDB800;
            newRecord->forwardVelocity = effect->sideVelocity;
            newRecord->model = effect->model + 1;
            newRecord = Mini_SnowboardFindFreeEffectRecord();
            if (newRecord == 0) {
                return;
            }
            *newRecord = *effect;
            newRecord->sideVelocity = effect->forwardVelocity;
            effect->fallVelocity = 0xFFFDC800;
            newRecord->forwardVelocity = -effect->sideVelocity;
            newRecord->model = effect->model + 2;
            if ((*(unsigned int *)&g_SnoboStatePtr->courseTime >> 0x10 & 0xF) == 7) {
                Mini_SnowboardSpawnEffectNodeWithVelocity((short)effect->section, (int)(g_SnoboStatePtr->velocity * 651.8986206054688f) + 0x800, (SnowboardVec *)&effect->sidePos, (SnowboardVec *)&effect->sideVelocity);
            }
        } else {
            effect->update = Mini_SnowboardEffectAgeThenFadeOut_1;
        }
    }
}

/*
 * Searches all 32 0x90-byte records at g_SnoboAltEffectRecords cyclically from g_SnoboEffectRecordCursor for a zero first
 * byte; updates the cursor to the next slot and returns the record address, or 0 when none is
 * free.
 */
// FUNCTION: FF7 0x0073bf70
SnowboardEffect *__cdecl Mini_SnowboardFindFreeEffectRecord(void)
{
    int slot;
    SnowboardEffect *record;
    int remaining;
    remaining = 32;
    for (slot = g_SnoboEffectRecordCursor; remaining--; slot = (slot + 1) & 0x1F) {
        record = &g_SnoboAltEffectRecords[slot];
        if (*(unsigned char *)record == 0) {
            g_SnoboEffectRecordCursor = (slot + 1) & 0x1F;
            return record;
        }
    }
    return 0;
}

/*
 * Post-increments signed age and, once the consumed age exceeds 40, unsigned fade. Clears the
 * active byte when consumed fade exceeds 30; otherwise advances all three position dwords by their
 * velocities, then adds 0x1000 to y velocity.
 */
// FUNCTION: FF7 0x0073bfdf
void __cdecl Mini_SnowboardEffectFallFadeOut(struct SnowboardEffect *effect)
{
    if ((effect->age)++ > 40 && (effect->fade)++ > 30) {
        effect->active = 0;
    } else {
        effect->sidePos += effect->sideVelocity;
        effect->height += effect->fallVelocity;
        effect->forward += effect->forwardVelocity;
        effect->fallVelocity += 4096;
    }
}

/*
 * Post-increments the signed age. Only when its consumed value exceeds 60 does it post-
 * increment the unsigned fade counter; clears the active byte when that consumed value
 * exceeds 30.
 */
// FUNCTION: FF7 0x0073c084
void __cdecl Mini_SnowboardEffectAgeThenFadeOut_1(SnowboardEffect *effect)
{
    if ((effect->age)++ > 60 && (effect->fade)++ > 30)
        effect->active = 0;
}

/*
 * Runs the preliminary effect helper. Collision bit two clears size, plays a sound, halves saved
 * horizontal/forward velocity and sets an upward impulse/new callback. Copies the full effect into
 * shared scratch and invokes two split-effect helpers with differing angle/offset values. Other
 * collision selects the alternate callback.
 */
// FUNCTION: FF7 0x0073c0df
void __cdecl Mini_SnowboardEffectOnHitSplitTwoChildren(SnowboardEffect *effect)
{
    SnowboardEffect *copy;

    Mini_SnowboardEffectStepTrianglePhase(effect);
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(3, 0x189, 0);
            effect->sideVelocity = effect->hitVelocityX >> 1;
            effect->fallVelocity = 0xFFFE0000;
            effect->forwardVelocity = effect->hitVelocityZ >> 1;
            (effect->model)++;
            effect->update = Mini_SnowboardEffectUpdateBallisticArcRetire;
            copy = (SnowboardEffect *)Gfx_GetPsxScratchpadPtr(0x27);
            *copy = *effect;
            (copy->model)++;
            Mini_SnowboardSpawnRotatedEffectCopy(copy, 0, 0xFFFFFD00, (SnowboardEffectTransform *)(copy + 1));
            Mini_SnowboardSpawnRotatedEffectCopy(copy, 0x800, 0x300, (SnowboardEffectTransform *)(copy + 1));
        } else {
            effect->update = Mini_SnowboardEffectUpdateAgeFadeOutStepPhase;
        }
    }
}

/*
 * Advances one record's 32-step animation phase and turns it into a triangle wave. Takes the low
 * five bits of timer, post-increments it, folds any value above 0x10 to 0x20
 * minus it so the sequence ramps 0 to 0x10 and back down, then stores the result shifted left by 7
 * into renderArg1.
 */
// FUNCTION: FF7 0x0073c1ee
void __cdecl Mini_SnowboardEffectStepTrianglePhase(SnowboardEffect *effect)
{
    int phase;

    phase = effect->timer & 0x1F;
    (effect->timer)++;
    if (phase > 16) {
        phase = 0x20 - phase;
    }
    effect->renderArg1 = phase << 7;
}

// Steps the phase in timer with Mini_SnowboardEffectStepTrianglePhase, then adds fallVelocity to height. If
// height then exceeds heightLimit, active is cleared to retire the record. Otherwise fallVelocity gains gravity of
// 0x1000, sideVelocity and forwardVelocity are added to sidePos and forward, rotX is spun by 0x20 and matrixValid is cleared.
// Once fallVelocity exceeds 0x8000, retireTimer is incremented.
// FUNCTION: FF7 0x0073c23b
void __cdecl Mini_SnowboardEffectUpdateBallisticArcRetire(SnowboardEffect *effect)
{
    Mini_SnowboardEffectStepTrianglePhase(effect);
    effect->height += effect->fallVelocity;
    if (effect->height > effect->heightLimit) {
        effect->active = 0;
    } else {
        effect->fallVelocity += 4096;
        effect->sidePos += effect->sideVelocity;
        effect->forward += effect->forwardVelocity;
        effect->rotX += 0x20;
        effect->matrixValid = 0;
        if (effect->fallVelocity > 32768) {
            (effect->fade)++;
        }
    }
}

/*
 * Post-increments signed age and, only after consumed age exceeds 60, unsigned fade. Clears the
 * active byte when consumed fade exceeds 30; otherwise advances the triangle-wave animation
 * through Mini_SnowboardEffectStepTrianglePhase.
 */
// FUNCTION: FF7 0x0073c2e9
void __cdecl Mini_SnowboardEffectUpdateAgeFadeOutStepPhase(SnowboardEffect *effect)
{
    if ((effect->age)++ > 60 && (effect->fade)++ > 30)
        effect->active = 0;
    else
        Mini_SnowboardEffectStepTrianglePhase(effect);
}

// Obtains a free record from Mini_SnowboardFindFreeEffectRecord and returns when there is none.
// Otherwise it copies the whole src effect into the record, adds angleOffset to rotY and clears
// matrixValid. A Y rotation by angle is written into transform and applied to src's velocity vector,
// and the resulting sideVelocity and forwardVelocity are stored in the record.
// FUNCTION: FF7 0x0073c352
void __cdecl Mini_SnowboardSpawnRotatedEffectCopy(SnowboardEffect *src, int angleOffset, int angle, SnowboardEffectTransform *transform)
{
    SnowboardEffect *newRecord;

    newRecord = Mini_SnowboardFindFreeEffectRecord();
    if (newRecord == 0) {
        return;
    }
    *newRecord = *src;
    newRecord->rotY += angleOffset;
    newRecord->matrixValid = 0;
    Matrix30SetRotationY((short)angle, (char *)transform + 0x40);
    PsxApplyMatrix33ToIntVector((SnoboMatrix *)((char *)transform + 0x40), (SnowboardSpawnPos *)&src->sideVelocity, (SnowboardVec *)((char *)transform + 0x20));
    newRecord->sideVelocity = transform->velocityX;
    newRecord->forwardVelocity = transform->velocityZ;
}

// Steps the phase in timer with Mini_SnowboardEffectStepTrianglePhase, then acts on a non-zero
// flags byte. With flags bit 2, contactRadius is cleared, sound 0x189 plays on channel 3, savedVelocityX and
// savedVelocityZ are each halved into sideVelocity and forwardVelocity, fallVelocity is set to 0xFFFE0000, and update
// becomes Mini_SnowboardEffectUpdateBallisticArcRetire. Otherwise update becomes
// Mini_SnowboardEffectUpdateAgeFadeOutStepPhase.
// FUNCTION: FF7 0x0073c3df
void __cdecl Mini_SnowboardEffectOnHitSelectBallisticOrFadeOut(SnowboardEffect *effect)
{
    Mini_SnowboardEffectStepTrianglePhase(effect);
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(3, 0x189, 0);
            effect->sideVelocity = effect->hitVelocityX >> 1;
            effect->fallVelocity = 0xFFFE0000;
            effect->forwardVelocity = effect->hitVelocityZ >> 1;
            effect->update = Mini_SnowboardEffectUpdateBallisticArcRetire;
        } else {
            effect->update = Mini_SnowboardEffectUpdateAgeFadeOutStepPhase;
        }
    }
}

/*
 * For a nonzero contact, bit 2 clears contactRadius and plays channel-2 sound 0x10C; always selects
 * Mini_SnowboardEffectAgeThenFadeOut_2 for that nonzero byte.
 */
// FUNCTION: FF7 0x0073c464
void __cdecl Mini_SnowboardEffectHitSfxSelectFadeOut(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(2, 0x10C, 0);
        }
        effect->update = Mini_SnowboardEffectAgeThenFadeOut_2;
    }
}

/*
 * Post-increments the signed age. Only when its consumed value exceeds 60 does it post-
 * increment the unsigned fade counter; clears the active byte when that consumed value
 * exceeds 30.
 */
// FUNCTION: FF7 0x0073c4a8
void __cdecl Mini_SnowboardEffectAgeThenFadeOut_2(SnowboardEffect *effect)
{
    if ((effect->age)++ > 60 && (effect->fade)++ > 30)
        effect->active = 0;
}

// Collects the pickup when collected is non-zero. active is cleared, sound 0xF9 plays on channel 3,
// and the g_SnoboPickupPointsByKind entry at pickupKind is added to g_SnoboStatePtr->pickupTotal.
// Otherwise age is incremented, frame becomes 0xF0 plus bits 3 and 4 of the prior age, height gains
// twice SinFixed of the prior age shifted left 5, and update becomes
// Mini_SnowboardPickupRiseThenFadeOut once trackX is within one of g_SnoboLastSignalValue.
// FUNCTION: FF7 0x0073c503
void __cdecl Mini_SnowboardPickupBobOrCollect(SnowboardEffect *effect)
{
    int oldAge;

    if (effect->contact != 0) {
        effect->active = 0;
        Mini_SnowboardPlaySfxOnChannel(3, 0xF9, 0);
        g_SnoboStatePtr->pickupTotal += g_SnoboPickupPointsByKind[effect->pickupKind];
    } else {
        oldAge = effect->age;
        (effect->age)++;
        effect->model = (oldAge >> 3 & 3) + 0xF0;
        effect->height += SinFixed(oldAge << 5) * 2;
        if (abs(g_SnoboLastSignalValue - effect->section) <= 1) {
            effect->update = Mini_SnowboardPickupRiseThenFadeOut;
        }
    }
}

/*
 * Post-increments signed counter and, only after its consumed value exceeds 120, unsigned
 * fade. Clears active when consumed fade exceeds 30; otherwise post-increments age, derives a
 * four-step model from its consumed value, advances y by velocity, and then subtracts 0x800
 * from that velocity.
 */
// FUNCTION: FF7 0x0073c5cf
void __cdecl Mini_SnowboardPickupRiseThenFadeOut(SnowboardEffect *node)
{
    int phase;
    if ((node->counter)++ > 120 && (node->fade)++ > 30) {
        node->active = 0;
    } else {
        phase = (node->age)++;
        node->model = ((phase >> 3) & 3) + 0xf0;
        node->height += node->fallVelocity;
        node->fallVelocity -= 2048;
    }
}

/*
 * For a nonzero contact, bit 2 clears contactRadius, selects Mini_SnowboardEffectTiltSineThenFade and plays channel-1 sound 0x10C
 * at 0x7F; otherwise selects Mini_SnowboardEffectTimerThenFadeOut.
 */
// FUNCTION: FF7 0x0073c68d
void __cdecl Mini_SnowboardEffectOnHitSelectGrowOrFade(SnowboardEffect *node)
{
    if (node->contact != 0) {
        if ((node->contact & 2) != 0) {
            node->contactRadius = 0;
            node->update = Mini_SnowboardEffectTiltSineThenFade;
            Mini_SnowboardPlaySfxOnChannel(1, 0x10C, 0x7F);
        } else {
            node->update = Mini_SnowboardEffectTimerThenFadeOut;
        }
    }
}

// Adds a quarter of SinFixed of the old tiltPhase shifted right 1 to tilt, post-incrementing
// tiltPhase, and clears matrixValid. When tilt exceeds 0x480, the callback becomes
// Mini_SnowboardEffectTimerThenFadeOut.
// FUNCTION: FF7 0x0073c6dd
void __cdecl Mini_SnowboardEffectTiltSineThenFade(SnowboardEffect *node)
{

    node->rotX += (short)(SinFixed(((node->age)++ >> 1)) >> 2);
    node->matrixValid = 0;
    if (node->rotX > 0x480) {
        node->update = Mini_SnowboardEffectTimerThenFadeOut;
    }
}

/*
 * Post-increments the signed timer. Only when its consumed value exceeds 60 does it post-
 * increment the unsigned fade counter; clears the active byte when that consumed value
 * exceeds 30.
 */
// FUNCTION: FF7 0x0073c74d
void __cdecl Mini_SnowboardEffectTimerThenFadeOut(SnowboardEffect *node)
{
    if ((node->timer)++ > 60 && (node->fade)++ > 30)
        node->active = 0;
}

// Calls Mini_SnowboardEffectOnHitSelectLaunchOrFade, advances sidePos and forward by sideVelocity and forwardVelocity,
// and sets model2 from Mini_SnowboardGetEffectCycleFrame of the cycleCounter value before its
// increment, shifted right 1. When sidePos is within 0x8000 of the homeX or homeY target chosen by
// reversed as the selector, the selector toggles, both increments are negated, countdown is set to
// 0x10 and update becomes Mini_SnowboardEffectUpdateSpinCycleThenResumeSweep.
// FUNCTION: FF7 0x0073c7a8
void __cdecl Mini_SnowboardEffectUpdateSweepTargetReverse(SnowboardEffect *node)
{

    Mini_SnowboardEffectOnHitSelectLaunchOrFade(node);
    node->sidePos += node->sideVelocity;
    node->forward += node->forwardVelocity;
    node->model2 = Mini_SnowboardGetEffectCycleFrame(((node->cycleCounter)++ >> 1));
    if (abs(*(int *)((char *)node + node->reversed * 4 + 0x3C) - node->sidePos) < 32768) {
        node->reversed ^= 1;
        node->sideVelocity = -node->sideVelocity;
        node->forwardVelocity = -node->forwardVelocity;
        node->countdown = 16;
        node->update = Mini_SnowboardEffectUpdateSpinCycleThenResumeSweep;
    }
}

/*
 * Mechanical table lookup: returns the short at index index % 6 of the array at g_SnoboEffectCycleFrames.
 * Nothing else is read or written.
 */
// FUNCTION: FF7 0x0073c86b
short __cdecl Mini_SnowboardGetEffectCycleFrame(int index)
{
    return (short)g_SnoboEffectCycleFrames[index % 6];
}

/*
 * For a nonzero contact, bit 2 clears contactRadius and selects Mini_SnowboardEffectHitLaunchSpinFall; otherwise selects
 * Mini_SnowboardEffectAgeThenFadeOut_3.
 */
// FUNCTION: FF7 0x0073c883
void __cdecl Mini_SnowboardEffectOnHitSelectLaunchOrFade(SnowboardEffect *node)
{
    if (node->contact != 0) {
        if ((node->contact & 2) != 0) {
            node->contactRadius = 0;
            node->update = Mini_SnowboardEffectHitLaunchSpinFall;
        } else {
            node->update = Mini_SnowboardEffectAgeThenFadeOut_3;
        }
    }
}

// Calls Mini_SnowboardEffectOnHitSelectLaunchOrFade, then spinStep gains 0x80, or -0x80 when the
// reversed is non-zero. matrixValid is cleared, model2 is set from
// Mini_SnowboardGetEffectCycleFrame of the cycleCounter value before its increment, shifted right
// 1, and countdown is decremented. When countdown reaches zero, update becomes
// Mini_SnowboardEffectUpdateSweepTargetReverse.
// FUNCTION: FF7 0x0073c8c2
void __cdecl Mini_SnowboardEffectUpdateSpinCycleThenResumeSweep(SnowboardEffect *node)
{

    Mini_SnowboardEffectOnHitSelectLaunchOrFade(node);
    node->rotY += (short)((node->reversed != 0 ? -0x100 : 0) + 0x80);
    node->matrixValid = 0;
    node->model2 = Mini_SnowboardGetEffectCycleFrame(((node->cycleCounter)++ >> 1));
    (node->countdown)--;
    if (node->countdown == 0) {
        node->update = Mini_SnowboardEffectUpdateSweepTargetReverse;
    }
}

// Per-entity update handler that Mini_SnowboardEffectOnHitSelectLaunchOrFade installs in the
// update. On the first tick, with bit 0x2 of contact set, it clears contact
// and launches the entity: the horizontal velocities are seeded from hitVelocityX and hitVelocityZ, the
// vertical velocity becomes -2.0 (0xFFFE0000 in 16.16 fixed point), sound 0x135 plays on channel 3,
// and section is copied to g_SnoboLastSignalValue. Each tick it adds the vertical velocity
// to the height. Once that passes heightLimit it clears active;
// otherwise it applies gravity, advances position and rotation, and refreshes section from
// g_SnoboStatePtr->section.
// FUNCTION: FF7 0x0073c952
void __cdecl Mini_SnowboardEffectHitLaunchSpinFall(SnowboardEffect *node)
{
    if ((node->contact & 2) != 0) {
        node->contact = 0;
        node->sideVelocity = node->hitVelocityX;
        node->fallVelocity = 0xFFFE0000;
        node->forwardVelocity = node->hitVelocityZ;
        Mini_SnowboardPlaySfxOnChannel(3, 0x135, 0);
        node->model = 0x105;
        node->model2 = 0x10B;
        g_SnoboLastSignalValue = node->section;
    }
    node->height += node->fallVelocity;
    if (node->height > node->heightLimit) {
        node->active = 0;
    } else {
        node->fallVelocity += 4096;
        node->sidePos += node->sideVelocity;
        node->forward += node->forwardVelocity;
        node->rotY += 0x100 /* 22.5 deg */;
        node->matrixValid = 0;
        node->section = g_SnoboStatePtr->section;
    }
}

/*
 * Post-increments the signed age. Only when its consumed value exceeds 60 does it post-
 * increment the unsigned fade counter; clears the active byte when that consumed value
 * exceeds 30.
 */
// FUNCTION: FF7 0x0073ca51
void __cdecl Mini_SnowboardEffectAgeThenFadeOut_3(SnowboardEffect *node)
{
    if ((node->age)++ > 60 && (node->fade)++ > 30)
        node->active = 0;
}

// Chooses the next update callback once the integer-converted riderPos (a float in the state) falls
// below node->triggerPos. It then picks Mini_SnowboardEffectUpdateCycleApproachTargetX
// when followAngle exceeds 0x200, and Mini_SnowboardEffectOnHitSelectLaunchOrFadeThunk otherwise.
// FUNCTION: FF7 0x0073caac
void __cdecl Mini_SnowboardEffectSelectApproachOrFade(SnowboardEffect *node)
{
    if ((int)g_SnoboStatePtr->riderPos < node->triggerPos) {
        if (g_SnoboStatePtr->followAngle > 0x200) {
            node->update = Mini_SnowboardEffectUpdateCycleApproachTargetX;
        } else {
            node->update = Mini_SnowboardEffectOnHitSelectLaunchOrFadeThunk;
        }
    }
}

// Calls Mini_SnowboardEffectOnHitSelectLaunchOrFade, then adds sideVelocity and forwardVelocity to
// sidePos and forward. It sets model2 from Mini_SnowboardGetEffectCycleFrame applied to the
// post-incremented cycleCounter. Once sidePos is within 0x10000 of targetX, it sets model2 to
// 0x106, selects Mini_SnowboardEffectUpdateSpinCycleThenBounce as update and sets
// countdown to 8.
// FUNCTION: FF7 0x0073caf3
void __cdecl Mini_SnowboardEffectUpdateCycleApproachTargetX(SnowboardEffect *node)
{

    Mini_SnowboardEffectOnHitSelectLaunchOrFade(node);
    node->sidePos += node->sideVelocity;
    node->forward += node->forwardVelocity;
    node->model2 = Mini_SnowboardGetEffectCycleFrame(((node->cycleCounter)++ >> 1));
    if (abs(node->targetX - node->sidePos) < 65536) {
        node->model2 = 0x106;
        node->update = Mini_SnowboardEffectUpdateSpinCycleThenBounce;
        node->countdown = 8;
    }
}

// Calls Mini_SnowboardEffectOnHitSelectLaunchOrFade, then moves spinStep by -0x80 when the
// sideVelocity is positive and by 0x80 otherwise, and clears matrixValid. It sets
// model2 from Mini_SnowboardGetEffectCycleFrame of the post-incremented cycleCounter and counts
// countdown down. At zero it selects Mini_SnowboardEffectUpdateBounceAwaitSignal as
// update, reloads countdown from height and sets cycleCounter to 8.
// FUNCTION: FF7 0x0073cb8f
void __cdecl Mini_SnowboardEffectUpdateSpinCycleThenBounce(SnowboardEffect *node)
{

    Mini_SnowboardEffectOnHitSelectLaunchOrFade(node);
    node->rotY += (short)(node->sideVelocity > 0 ? -0x80 : 0x80);
    node->matrixValid = 0;
    node->model2 = Mini_SnowboardGetEffectCycleFrame(((node->cycleCounter)++ >> 1));
    (node->countdown)--;
    if (node->countdown == 0) {
        node->update = Mini_SnowboardEffectUpdateBounceAwaitSignal;
        node->countdown = node->height;
        node->cycleCounter = 8;
    }
}

/*
 * Collision bit two clears size and switches callback. Otherwise positive countdown selects mesh
 * 0x106 and can return through collision bit one; negative countdown applies airborne motion until
 * stored ground height, resetting countdown to 0x10. Zero countdown either consumes a matching
 * shared segment signal and changes callback or sets upward velocity. Decrements the countdown
 * after retained non-return branches.
 */
// FUNCTION: FF7 0x0073cc38
void __cdecl Mini_SnowboardEffectUpdateBounceAwaitSignal(SnowboardEffect *node)
{
    if ((node->contact & 2) != 0) {
        node->contactRadius = 0;
        node->update = Mini_SnowboardEffectHitLaunchSpinFall;
    } else {
        if (node->direction > 0) {
            node->model2 = 0x106;
            if ((node->contact & 1) != 0) {
                node->update = Mini_SnowboardEffectAgeThenFadeOut_3;
                return;
            }
        } else if (node->direction < 0) {
            node->model2 = 0x10B;
            node->height += node->fallVelocity;
            node->fallVelocity += 4096;
            if (node->fallVelocity > 0 && node->height >= node->limit) {
                node->height = node->limit;
                node->direction = 0x10;
            }
        } else if (g_SnoboLastSignalValue == node->section) {
            node->update = Mini_SnowboardEffectUpdateSpinCycleHoldRestart;
            node->limit = 14;
            node->direction = 0;
            g_SnoboLastSignalValue = 0;
        } else {
            node->fallVelocity = 0xFFFF8000;
        }
        (node->direction)--;
    }
}

/*
 * A thunk: forwards its only argument to Mini_SnowboardEffectOnHitSelectLaunchOrFade and returns. It adds nothing of its own.
 */
// FUNCTION: FF7 0x0073cd52
void __cdecl Mini_SnowboardEffectOnHitSelectLaunchOrFadeThunk(SnowboardEffect *node)
{
    Mini_SnowboardEffectOnHitSelectLaunchOrFade(node);
}

/*
 * Runs the motion helper and postdecrements its long counter. Positive old values rotate according
 * to lateral velocity and advance indexed mesh animation; values down through greater than -0xB4
 * hold mesh 0x106. At or below -0xB4 resets mesh/countdown and changes callback.
 */
// FUNCTION: FF7 0x0073cd63
void __cdecl Mini_SnowboardEffectUpdateSpinCycleHoldRestart(SnowboardEffect *node)
{
    int timer;

    Mini_SnowboardEffectOnHitSelectLaunchOrFade(node);
    timer = node->holdTimer;
    (node->holdTimer)--;
    if (timer > 0) {
        node->rotY += (short)(node->sideVelocity > 0 ? 0x80 : -0x80);
        node->matrixValid = 0;
        node->model2 = Mini_SnowboardGetEffectCycleFrame(((node->cycleCounter)++ >> 1));
    } else if (timer > -180) {
        node->model2 = 0x106;
    } else {
        node->model = 0x105;
        node->holdTimer = 12;
        node->update = Mini_SnowboardEffectUpdateSpinCycleThenBounce;
    }
}

// Spawns an effect node of the given kind from the shared template g_SnoboPositionAngleEffectDef,
// after copying angle into rotY and position x, y and z into sidePos, height
// and forward. The node's sideVelocity and forwardVelocity come from velocity, and its
// fallVelocity from velocity->y minus 0x1000.
// FUNCTION: FF7 0x0073ce27
void __cdecl Mini_SnowboardSpawnEffectNodeWithVelocity(short kind, int angle, SnowboardVec *position, SnowboardVec *velocity)
{
    struct SnowboardEffect *node;

    g_SnoboPositionAngleEffectDef.rotY = angle;
    g_SnoboPositionAngleEffectDef.sidePos = position->x;
    g_SnoboPositionAngleEffectDef.height = position->y;
    g_SnoboPositionAngleEffectDef.forward = position->z;
    node = (struct SnowboardEffect *)Mini_SnowboardAllocEffectNodeFromTemplate((short)kind, &g_SnoboPositionAngleEffectDef);
    node->sideVelocity = velocity->x;
    node->fallVelocity = velocity->y - 4096;
    node->forwardVelocity = velocity->z;
}

/*
 * Allocates a secondary-list effect node and, on success, sets active/type fields, copies a
 * seventy-two-byte template and clears four velocity/state dwords and five short fields. Returns
 * the node or zero without accessing the template on allocation failure.
 */
// FUNCTION: FF7 0x0073ce98
int __cdecl Mini_SnowboardAllocEffectNodeFromTemplate(short kind, SnowboardEffectTemplate *def)
{
    SnowboardEffect *node;

    node = Mini_SnowboardFindFreeEffectRecord();
    if (node != 0) {
        node->active = 1;
        node->contact = 0;
        node->section = kind;
        *(SnowboardEffectTemplate *)&node->model = *def;
        node->sideVelocity = 0;
        node->fallVelocity = 0;
        node->forwardVelocity = 0;
        node->reversed = 0;
        node->age = 0;
        node->timer = 0;
        node->counter = 0;
        node->fade = 0;
        node->blinkCounter = 0;
    }
    return (int)node;
}

// Sets segmentDistance of node[1] to 0xFFFF when segmentIndex is more than two ahead of the section
// of g_SnoboStatePtr, and to 0 otherwise. When Mini_SnowboardXZDistance from the rider position in the state to
// the node position is below followAngle plus 0x400, it selects Mini_SnowboardEffectUpdateDropFadeIn as
// update. It then sets fadeRate to 0x60 or 0x40 according to bit 0x1 of flags, and sets
// segmentDistance and fadeLimit to 0x100.
// FUNCTION: FF7 0x0073cf45
void __cdecl Mini_SnowboardEffectStartDropFadeInWhenNear(SnowboardEffect *node)
{
    node->fade = node->section - g_SnoboStatePtr->section > 2 ? 0xffff : 0;
    if (Mini_SnowboardXZDistance(((int)g_SnoboStatePtr) + 0x16c, &node->sidePos) < g_SnoboStatePtr->followAngle + 0x400) {
        node->update = Mini_SnowboardEffectUpdateDropFadeIn;
        if ((node->variant & 1) != 0) {
            node->contactRadius = 96;
            node->contactTop = 256;
            node->contactSplit = 0x100;
        } else {
            node->contactRadius = 64;
            node->contactTop = 256;
            node->contactSplit = 0x100;
        }
    }
}

/*
 * Converts the first and third float coordinates at pointA to integers, subtracts the corresponding
 * integer coordinates at pointB, then shifts each difference right 12. Returns SqrtToInt of the sum
 * of their squares. Conversion and multiplication retain the native arithmetic limits.
 */
// FUNCTION: FF7 0x0073cffb
int __cdecl Mini_SnowboardXZDistance(int pointA, void *pointB)
{
    int deltaZ;
    int xDelta;
    xDelta = ((int)*(float *)pointA - *(int *)pointB) >> 12;
    deltaZ = ((int)*(float *)((char *)pointA + 8) - *(int *)((char *)pointB + 8)) >> 12;
    return SqrtToInt(xDelta * xDelta + deltaZ * deltaZ);
}

/*
 * Animates an eight-frame opacity field, accelerates vertical motion and selects the next callback
 * after passing stored height. Plays a one-shot approach sound within 0x20000 of height; collision
 * bit two plays another sound and clears collision/size fields.
 */
// FUNCTION: FF7 0x0073d04a
void __cdecl Mini_SnowboardEffectUpdateDropFadeIn(SnowboardEffect *node)
{
    if ((node->age)++ < 8) {
        node->isFadingIn = 1;
        node->fadeAmount = 8 - node->age << 7;
    } else {
        node->isFadingIn = 0;
    }
    node->fallVelocity += 4096;
    node->height += node->fallVelocity;
    if (node->height > node->heightLimit) {
        node->update = Mini_SnowboardEffectUpdateLandedHitFadeRetire;
        node->age = 60;
    }
    if (node->landed == 0 && node->height > node->heightLimit - 0x20000) {
        Mini_SnowboardPlaySfxOnChannel(3, 0x1AB, 0);
        node->landed = 1;
    }
    if ((node->contact & 2) != 0) {
        Mini_SnowboardPlaySfxOnChannel(2, 0xF0, 0);
        node->contact = 0;
        node->contactRadius = 0;
    }
}

// While delayTimer is nonzero it counts it down; if bit 0x2 of flags is then set, it plays sound
// 0xF0 on channel 2, clears the whole flags byte and clears contactRadius. Once delayTimer is zero, it
// clears contactRadius and post-increments the unsigned fadeTimer, clearing the active byte when the
// consumed value exceeds 30.
// FUNCTION: FF7 0x0073d15c
void __cdecl Mini_SnowboardEffectUpdateLandedHitFadeRetire(SnowboardEffect *node)
{
    if (node->age != 0) {
        (node->age)--;
        if ((node->contact & 2) != 0) {
            Mini_SnowboardPlaySfxOnChannel(2, 0xf0, 0);
            node->contact = 0;
            node->contactRadius = 0;
        }
    } else {
        node->contactRadius = 0;
        if ((node->fade)++ > 30)
            node->active = 0;
    }
}

/*
 * Returns while its segment is more than three ahead. Otherwise postdecrements the delay, keeping
 * fade field-1 while old delay is nonzero. At zero initializes size/vertical velocity, reloads
 * lifetime, resets bounce count and copies base position with randomized Y offset, then changes to
 * the active callback.
 */
// FUNCTION: FF7 0x0073d1f1
void __cdecl Mini_SnowboardEffectWaitThenStartBounce(SnowboardEffect *node)
{
    if (node->section - g_SnoboStatePtr->section > 3) {
        return;
    }
    if ((node->age)-- != 0) {
        node->waitState = 0xffff;
    } else {
        node->waitState = 0;
        node->contactRadius = 160;
        node->fallVelocity = 0x8000;
        node->age = node->timerReload;
        node->counter = 0;
        node->sidePos = node->homeX;
        node->height = node->homeY + ((rand() & 15) << 16);
        node->forward = node->homeZ;
        node->update = Mini_SnowboardEffectUpdateBounceGravity;
    }
}

/*
 * Postdecrements lifetime; at old zero deactivates a previously hit effect or hides/rearms it with
 * a random delay and returns. Near expiry increases its fade field, clears collision on hit and
 * marks it hit with sound, then advances accelerated position/rotation. Ground crossing clamps
 * height, reverses and halves vertical velocity and invokes the bounce helper for the first three
 * old bounce counts.
 */
// FUNCTION: FF7 0x0073d2c1
void __cdecl Mini_SnowboardEffectUpdateBounceGravity(SnowboardEffect *effect)
{
    int lifetime;

    lifetime = effect->age;
    (effect->age)--;
    if (lifetime == 0) {
        if (effect->started != 0) {
            effect->active = 0;
        } else {
            effect->contactRadius = 0;
            effect->age = (rand() & 0x1F) + 8;
            effect->update = Mini_SnowboardEffectWaitThenStartBounce;
            effect->stepIndex = 0xFFFF;
        }
        return;
    } else if (lifetime < 16) {
        (effect->stepIndex)++;
    }
    if (effect->contact != 0) {
        effect->contact = 0;
        effect->contactRadius = 0;
        effect->started = 1;
        Mini_SnowboardPlaySfxOnChannel(2, 0xF0, 0);
    }
    effect->sidePos += effect->sideVelocity;
    effect->forward += effect->forwardVelocity;
    effect->height += effect->fallVelocity;
    effect->fallVelocity += 4096;
    effect->rotZ += (short)(-effect->sideVelocity >> 11);
    effect->matrixValid = 0;
    if (effect->height > effect->heightLimit) {
        effect->height = effect->heightLimit;
        effect->fallVelocity = -effect->fallVelocity >> 1;
        if ((effect->bounceCount)++ < 3) {
            Mini_SnowboardPlaySfx1ABThrottled(7 - (effect->section - g_SnoboStatePtr->section) << 4);
        }
    }
}

/*
 * When narrowed strength is at least g_SnoboSfx1ABLastStrength or more than ten context ticks have elapsed, records
 * the tick and argument in g_SnoboSfx1ABLastTick/g_SnoboSfx1ABLastStrength and plays channel-4 sound 0x1AB with strength
 * narrowed to short.
 */
// FUNCTION: FF7 0x0073d481
void __cdecl Mini_SnowboardPlaySfx1ABThrottled(int strength)
{
    if ((short)strength >= g_SnoboSfx1ABLastStrength || g_SnoboStatePtr->tick - g_SnoboSfx1ABLastTick > 10) {
        g_SnoboSfx1ABLastTick = *(short *)&g_SnoboStatePtr->tick;
        g_SnoboSfx1ABLastStrength = strength;
        Mini_SnowboardPlaySfxOnChannel(4, 0x1AB, (short)strength);
    }
}

// Returns while startSection is more than three ahead of the section of g_SnoboStatePtr. While the
// old value of timer is nonzero, it sets startState to -1. Otherwise it sets waitFrames to 0xA0,
// copies homeX, homeY and homeZ to sidePos, height and forward, reloads timer from g_SnoboIndexedEffectLifetimes
// indexed by tableIndex >> 1, clears startState and selects Mini_SnowboardEffectUpdateSpin.
// FUNCTION: FF7 0x0073d4de
void __cdecl Mini_SnowboardEffectWaitThenStartSpin(SnowboardEffect *effect)
{
    if (effect->section - g_SnoboStatePtr->section > 3) {
        return;
    }
    if ((effect->age)-- != 0) {
        effect->startState = 0xffff;
    } else {
        effect->contactRadius = 160;
        effect->sidePos = effect->homeX;
        effect->height = effect->homeY;
        effect->forward = effect->homeZ;
        effect->age = g_SnoboIndexedEffectLifetimes[effect->spinIndex >> 1];
        effect->startState = 0;
        effect->update = Mini_SnowboardEffectUpdateSpin;
    }
}

/*
 * Clears collision and size and marks a hit with sound when flagged. Postdecrements lifetime, then
 * deactivates a hit effect or rearms an unhit one with a random delay at old zero; near expiry
 * increases fade. Continues position/rotation updates even after those transitions, using the
 * indexed rotation table without bounds checks.
 */
// FUNCTION: FF7 0x0073d594
void __cdecl Mini_SnowboardEffectUpdateSpin(SnowboardEffect *effect)
{
    int lifetime;

    if (effect->contact != 0) {
        effect->contact = 0;
        effect->contactRadius = 0;
        effect->spinning = 1;
        Mini_SnowboardPlaySfxOnChannel(2, 0xF0, 0);
    }
    lifetime = effect->age;
    (effect->age)--;
    if (lifetime == 0) {
        if (effect->spinning != 0) {
            effect->active = 0;
        } else {
            effect->contact = 0;
            effect->update = Mini_SnowboardEffectWaitThenStartSpin;
            effect->age = (rand() & 0x1F) + 8;
        }
    } else if (lifetime < 16) {
        (effect->fade)++;
    }
    effect->sidePos += effect->sideVelocity;
    effect->height += effect->fallVelocity;
    effect->forward += effect->forwardVelocity;
    effect->rotX += g_SnoboIndexedEffectSpinSteps[effect->spinIndex];
    effect->matrixValid = 0;
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0x800, 0, and the address of the descriptor record g_SnoboAltSlot008EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073d6b7
void __cdecl Mini_SnowboardAltSlot008SpawnEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x800, 0, &g_SnoboAltSlot008EffectPos0);
    }
}

/*
 * Prepares g_SnoboVelocityEffectDef with ID 0xF0, narrowed variant fields, effectKind and the three words at descriptor with
 * the middle word reduced by 0x80000, then calls Mini_SnowboardAllocEffectNodeFromTemplate for slot.
 */
// FUNCTION: FF7 0x0073d6db
void __cdecl Mini_SnowboardSpawnEffectNodeTemplateF0(short slot, int effectKind, int variant, SnowboardSpawnPos *descriptor)
{
    g_SnoboVelocityEffectDef.variant = (short)variant;
    g_SnoboVelocityEffectDef.model = 0xF0;
    g_SnoboVelocityEffectDef.renderArg1 = (short)variant;
    g_SnoboVelocityEffectDef.rotY = effectKind;
    g_SnoboVelocityEffectDef.sidePos = descriptor->vecX;
    g_SnoboVelocityEffectDef.height = descriptor->vecY - 0x80000;
    g_SnoboVelocityEffectDef.forward = descriptor->vecZ;
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboVelocityEffectDef);
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateED with slot narrowed to short, the
 * constant 0xADC, 0, and the address of the descriptor record g_SnoboAltSlot015EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073d740
void __cdecl Mini_SnowboardAltSlot015SpawnEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateED((short)slot, 0xADC, 0, &g_SnoboAltSlot015EffectPos0);
    }
}

/*
 * Prepares g_SnoboEffectDefDescriptor with narrowed variant plus 0xED, effectKind and the three words at descriptor, then calls
 * Mini_SnowboardAllocEffectNodeFromTemplate for slot.
 */
// FUNCTION: FF7 0x0073d764
void __cdecl Mini_SnowboardSpawnEffectNodeTemplateED(short slot, int effectKind, int variant, SnowboardVec *descriptor)
{
    g_SnoboEffectDefDescriptor.model = (short)variant + 0xED;
    g_SnoboEffectDefDescriptor.rotY = effectKind;
    g_SnoboEffectDefDescriptor.sidePos = descriptor->x;
    g_SnoboEffectDefDescriptor.height = descriptor->y;
    g_SnoboEffectDefDescriptor.forward = descriptor->z;
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboEffectDefDescriptor);
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0xB42, 0, and the address of the descriptor record g_SnoboAltSlot016EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073d7b7
void __cdecl Mini_SnowboardAltSlot016SpawnEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xB42, 0, &g_SnoboAltSlot016EffectPos0);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0xB73, 0, and the address of the descriptor record g_SnoboAltSlot020EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073d7db
void __cdecl Mini_SnowboardAltSlot020SpawnEffectAbove(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xB73, 0, &g_SnoboAltSlot020EffectPos0);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0x772, 0, and the address of the descriptor record g_SnoboAltSlot026EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073d7ff
void __cdecl Mini_SnowboardAltSlot026SpawnEffectAbove(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x772, 0, &g_SnoboAltSlot026EffectPos0);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnKindEAEffectOnHit twice with slot narrowed to short and
 * the constant 0x43A, once for the descriptor at g_SnoboAltSlot031EffectPos0 and once for the one at
 * g_SnoboAltSlot031EffectPos0[1], twelve bytes further on. Does nothing when enabled is zero.
 */
// FUNCTION: FF7 0x0073d823
void __cdecl Mini_SnowboardAltSlot031SpawnTwoKindEAEffects(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0x43A, 0, g_SnoboAltSlot031EffectPos0);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0x43A, 0, &g_SnoboAltSlot031EffectPos0[1]);
    }
}

/*
 * Prepares g_SnoboEffectTemplateOnHit with narrowed variant plus 0xEA, effectKind and the three words at descriptor, retaining
 * the middle word twice. Selects callback Mini_SnowboardEffectUpdateHopOrOnHit for narrowed variant==2 or Mini_SnowboardEffectOnHitSignalThenLaunchOrFade
 * otherwise, then calls Mini_SnowboardAllocEffectNodeFromTemplate.
 */
// FUNCTION: FF7 0x0073d860
void __cdecl Mini_SnowboardSpawnKindEAEffectOnHit(short slot, int effectKind, int variant, SnowboardVec *descriptor)
{
    g_SnoboEffectTemplateOnHit.model = (short)variant + 0xEA;
    g_SnoboEffectTemplateOnHit.rotY = effectKind;
    g_SnoboEffectTemplateOnHit.sidePos = descriptor->x;
    g_SnoboEffectTemplateOnHit.heightLimit = descriptor->y;
    g_SnoboEffectTemplateOnHit.height = g_SnoboEffectTemplateOnHit.heightLimit;
    g_SnoboEffectTemplateOnHit.forward = descriptor->z;
    if ((short)variant == 2) {
        g_SnoboEffectTemplateOnHit.update = (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateHopOrOnHit;
    } else {
        g_SnoboEffectTemplateOnHit.update = (SnowboardEffectUpdate)Mini_SnowboardEffectOnHitSignalThenLaunchOrFade;
    }
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboEffectTemplateOnHit);
}

// When flags is nonzero, it copies signalValue to g_SnoboLastSignalValue. If bit 0x2 of flags is
// set, it seeds sideVelocity and forwardVelocity from savedVelocityX and savedVelocityZ, sets the
// fallVelocity to 0xFFFD8000, plays sound 0xF0 on channel 3, clears contactRadius and selects
// Mini_SnowboardEffectUpdateLaunchedArcRetire as update. Otherwise it selects
// Mini_SnowboardEffectAgeThenFadeOut_4.
// FUNCTION: FF7 0x0073d8dc
void __cdecl Mini_SnowboardEffectOnHitSignalThenLaunchOrFade(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        g_SnoboLastSignalValue = effect->section;
        if ((effect->contact & 2) != 0) {
            effect->sideVelocity = effect->hitVelocityX;
            effect->fallVelocity = 0xFFFD8000;
            effect->forwardVelocity = effect->hitVelocityZ;
            Mini_SnowboardPlaySfxOnChannel(3, 0xF0, 0);
            effect->contactRadius = 0;
            effect->update = Mini_SnowboardEffectUpdateLaunchedArcRetire;
        } else {
            effect->update = Mini_SnowboardEffectAgeThenFadeOut_4;
        }
    }
}

// Per-entity update handler of the same family as Mini_SnowboardEffectHitLaunchSpinFall, called
// through update. If height already exceeds heightLimit it clears the active
// byte active and stops. Otherwise it refreshes section from g_SnoboStatePtr, integrates sidePos,
// height and forward by sideVelocity, fallVelocity and forwardVelocity, adds 0x1000 of gravity to
// fallVelocity, subtracts 0x20 from rotX, zeroes matrixValid, and once fallVelocity is positive after that
// (the entity is falling) increments age.
// FUNCTION: FF7 0x0073d95e
void __cdecl Mini_SnowboardEffectUpdateLaunchedArcRetire(struct SnowboardEffect *effect)
{
    if (effect->height > effect->heightLimit) {
        effect->active = 0;
    } else {
        effect->section = g_SnoboStatePtr->section;
        effect->sidePos += effect->sideVelocity;
        effect->height += effect->fallVelocity;
        effect->forward += effect->forwardVelocity;
        effect->fallVelocity += 4096;
        effect->rotX -= 0x20;
        effect->matrixValid = 0;
        if (effect->fallVelocity > 0) {
            (effect->fade)++;
        }
    }
}

/*
 * Post-increments the signed age. Only when its consumed value exceeds 60 does it post-
 * increment the unsigned fade counter; clears the active byte when that consumed value
 * exceeds 30.
 */
// FUNCTION: FF7 0x0073da14
void __cdecl Mini_SnowboardEffectAgeThenFadeOut_4(SnowboardEffect *effect)
{
    if ((effect->age)++ > 60 && (effect->fade)++ > 30)
        effect->active = 0;
}

/*
 * Calls Mini_SnowboardEffectOnHitSignalThenLaunchOrFade when contact is nonzero. Otherwise post-decrements signed countdown
 * age; if its consumed value is negative, resets y velocity to -0x8000 and countdown to a random
 * value from 32 through 63. Advances y, adds 0x1000 to velocity, then clamps y to heightLimit and
 * zeroes velocity if y has passed the limit.
 */
// FUNCTION: FF7 0x0073da6f
void __cdecl Mini_SnowboardEffectUpdateHopOrOnHit(struct SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        Mini_SnowboardEffectOnHitSignalThenLaunchOrFade(effect);
    } else {
        if ((effect->age)-- < 0) {
            effect->fallVelocity = -0x8000;
            effect->age = (rand() & 0x1f) + 32;
        }
        effect->height += effect->fallVelocity;
        effect->fallVelocity += 4096;
        if (effect->height > effect->heightLimit) {
            effect->height = effect->heightLimit;
            effect->fallVelocity = 0;
        }
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0x41C, 0, and the address of the descriptor record g_SnoboAltSlot032EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073db1e
void __cdecl Mini_SnowboardAltSlot032SpawnEffectAbove(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x41C, 0, &g_SnoboAltSlot032EffectPos0[0]);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it issues three calls with slot narrowed to short and the
 * constant 0x41D, one to Mini_SnowboardSpawnEffectNodeTemplateF0 for the descriptor at g_SnoboAltSlot040EffectPos0 and two to Mini_SnowboardSpawnKindEAEffectOnHit
 * for the descriptors at g_SnoboAltSlot040EffectPos0 + 0xC and g_SnoboAltSlot040EffectPos1, twelve bytes apart. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073db42
void __cdecl Mini_SnowboardAltSlot040SpawnThreeEffects(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x41D, 0, &g_SnoboAltSlot040EffectPos0[0]);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0x41D, 0, (SnowboardVec *)&g_SnoboAltSlot040EffectPos0[1]);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0x41D, 0, &g_SnoboAltSlot040EffectPos1);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it issues three calls with slot narrowed to short and the
 * constant 0x48D, two to Mini_SnowboardSpawnKindEAEffectOnHit for the descriptors at g_SnoboAltSlot048EffectPos0 and g_SnoboAltSlot048EffectPos0[1] and one
 * to Mini_SnowboardSpawnEffectNodeTemplateF0 for g_SnoboAltSlot048EffectPos1, twelve bytes apart. Does nothing when enabled is zero.
 */
// FUNCTION: FF7 0x0073db98
void __cdecl Mini_SnowboardAltSlot048SpawnThreeEffects(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0x48D, 0, g_SnoboAltSlot048EffectPos0);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0x48D, 0, &g_SnoboAltSlot048EffectPos0[1]);
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x48D, 0, &g_SnoboAltSlot048EffectPos1);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnKindEAEffectPlain with slot narrowed to short, the
 * constant 0x726, 0, and the address of the descriptor record g_SnoboAltSlot050EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073dbee
void __cdecl Mini_SnowboardAltSlot050SpawnKindEAEffect(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectPlain((short)slot, 0x726, 0, &g_SnoboAltSlot050EffectPos0);
    }
}

/*
 * Prepares g_SnoboEffectTemplatePlain with narrowed variant plus 0xEA, effectKind and the three words at descriptor, then calls
 * Mini_SnowboardAllocEffectNodeFromTemplate for slot.
 */
// FUNCTION: FF7 0x0073dc12
void __cdecl Mini_SnowboardSpawnKindEAEffectPlain(short slot, int effectKind, int variant, SnowboardSpawnPos *descriptor)
{
    g_SnoboEffectTemplatePlain.model = (short)variant + 0xEA;
    g_SnoboEffectTemplatePlain.rotY = effectKind;
    g_SnoboEffectTemplatePlain.sidePos = descriptor->vecX;
    g_SnoboEffectTemplatePlain.height = descriptor->vecY;
    g_SnoboEffectTemplatePlain.forward = descriptor->vecZ;
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboEffectTemplatePlain);
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0x8C4, 0, and the address of the descriptor record g_SnoboAltSlot056EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073dc65
void __cdecl Mini_SnowboardAltSlot056SpawnEffectAbove(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x8C4, 0, &g_SnoboAltSlot056EffectPos0);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0xC00, 0, and the address of the descriptor record g_SnoboAltSlot058EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073dc89
void __cdecl Mini_SnowboardAltSlot058SpawnEffectAbove(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xC00, 0, &g_SnoboAltSlot058EffectPos0);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateED with slot narrowed to short, the
 * constant 0xF1E, 0, and the address of the descriptor record g_SnoboAltSlot061EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073dcad
void __cdecl Mini_SnowboardAltSlot061SpawnKindEDEffect(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateED((short)slot, 0xF1E, 0, &g_SnoboAltSlot061EffectPos0);
    }
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073dcd1
void __cdecl Mini_SnowboardAltSlot062NoOp(void)
{
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateED with slot narrowed to short, the
 * constant 0xB90, the third argument 2 rather than the usual 0, and the address of the descriptor
 * record g_SnoboAltSlot063EffectPos0. Does nothing when enabled is zero.
 */
// FUNCTION: FF7 0x0073dcd6
void __cdecl Mini_SnowboardAltSlot063SpawnKindEFEffect(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateED((short)slot, 0xB90, 2, &g_SnoboAltSlot063EffectPos0);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0x858, 0, and the address of the descriptor record g_SnoboAltSlot066EffectPos0. Does nothing when enabled
 * is zero.
 */
// FUNCTION: FF7 0x0073dcfa
void __cdecl Mini_SnowboardAltSlot066SpawnEffectAbove(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x858, 0, &g_SnoboAltSlot066EffectPos0);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnKindEAEffectOnHit with slot narrowed to short, the
 * constant 0xE3A, the third argument 1, and the address of the descriptor record g_SnoboAltSlot073EffectPos0.
 * Does nothing when enabled is zero.
 */
// FUNCTION: FF7 0x0073dd1e
void __cdecl Mini_SnowboardAltSlot073SpawnKindEBEffect(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xE3A, 1, &g_SnoboAltSlot073EffectPos0);
    }
}

/*
 * Guard wrapper: when enabled is nonzero it calls Mini_SnowboardSpawnKindEAEffectOnHit with slot narrowed to short, the
 * constant 0xD1C, the third argument 1, and the address of the descriptor record g_SnoboAltSlot074EffectPos0.
 * Does nothing when enabled is zero.
 */
// FUNCTION: FF7 0x0073dd42
void __cdecl Mini_SnowboardAltSlot074SpawnKindEBEffect(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xD1C, 1, &g_SnoboAltSlot074EffectPos0);
    }
}

/*
 * Guard wrapper: when gate is nonzero it calls Mini_SnowboardSpawnKindEAEffectOnHit with slot narrowed to short, the
 * constant 0xC00, the third argument 1, and the address of the descriptor record g_SnoboAltSlot075EffectPos0.
 * Does nothing when gate is zero.
 */
// FUNCTION: FF7 0x0073dd66
void __cdecl Mini_SnowboardAltSlot075SpawnKindEBEffect(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xC00, 1, &g_SnoboAltSlot075EffectPos0);
    }
}

/*
 * Guard wrapper: when gate is nonzero it calls Mini_SnowboardSpawnKindEAEffectOnHit with slot narrowed to short, the
 * constant 0xAE5, the third argument 1, and the address of the descriptor record g_SnoboAltSlot076EffectPos0.
 * Does nothing when gate is zero.
 */
// FUNCTION: FF7 0x0073dd8a
void __cdecl Mini_SnowboardAltSlot076SpawnKindEBEffect(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xAE5, 1, &g_SnoboAltSlot076EffectPos0);
    }
}

/*
 * Guard wrapper: when gate is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0x83A, 0, and the address of the descriptor record g_SnoboAltSlot079EffectPos0. Does nothing when gate
 * is zero.
 */
// FUNCTION: FF7 0x0073ddae
void __cdecl Mini_SnowboardAltSlot079SpawnEffectAbove(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x83A, 0, &g_SnoboAltSlot079EffectPos0);
    }
}

/*
 * Guard wrapper: when gate is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF0 with slot narrowed to short, the
 * constant 0xEE5, 0, and the address of the descriptor record g_SnoboAltSlot085EffectPos0. Does nothing when gate
 * is zero.
 */
// FUNCTION: FF7 0x0073ddd2
void __cdecl Mini_SnowboardAltSlot085SpawnEffectAbove(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xEE5, 0, &g_SnoboAltSlot085EffectPos0);
    }
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073ddf6
void __cdecl Mini_SnowboardAltSlot091NoOp(void)
{
}

/*
 * One of the one-line emitter shims in this file: does nothing unless gate is nonzero, then calls
 * Mini_SnowboardSpawnKindEAEffectPlain with slot narrowed to short, the constant 0xE00, sub-index 0 and the address of the
 * static record g_SnoboAltSlot115EffectPos0.
 */
// FUNCTION: FF7 0x0073ddfb
void __cdecl Mini_SnowboardAltSlot115SpawnPlainEffect(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnKindEAEffectPlain((short)slot, 0xE00, 0, &g_SnoboAltSlot115EffectPos0);
    }
}

/*
 * Gated on gate being nonzero; calls Mini_SnowboardSpawnEffectNodeTemplateF0 twice with (short)slot and the constant 0xA00,
 * once with sub-index 0 and the record at g_SnoboAltSlot132EffectPos0 and once with sub-index 1 and the record
 * twelve bytes later at g_SnoboAltSlot132EffectPos0 + 0xC.
 */
// FUNCTION: FF7 0x0073de1f
void __cdecl Mini_SnowboardAltSlot132SpawnTwoEffectsAbove(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltSlot132EffectPos0[0]);
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 1, &g_SnoboAltSlot132EffectPos0[1]);
    }
}

/*
 * Does nothing unless gate is nonzero, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 2,
 * &g_SnoboAltSlot133EffectPos0).
 */
// FUNCTION: FF7 0x0073de5c
void __cdecl Mini_SnowboardAltSlot133SpawnEffectAbove(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 2, &g_SnoboAltSlot133EffectPos0);
    }
}

/*
 * Does nothing unless gate is nonzero, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0,
 * &g_SnoboAltSlot138EffectPos0).
 */
// FUNCTION: FF7 0x0073de80
void __cdecl Mini_SnowboardAltSlot138SpawnEffectAbove(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltSlot138EffectPos0);
    }
}

/*
 * Does nothing unless gate is nonzero, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0,
 * &g_SnoboAltSlot146EffectPos0).
 */
// FUNCTION: FF7 0x0073dea4
void __cdecl Mini_SnowboardAltSlot146SpawnEffectAbove(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltSlot146EffectPos0);
    }
}

/*
 * Does nothing unless gate is nonzero, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 1,
 * &g_SnoboAltSlot148EffectPos0).
 */
// FUNCTION: FF7 0x0073dec8
void __cdecl Mini_SnowboardAltSlot148SpawnEffectAbove(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 1, &g_SnoboAltSlot148EffectPos0);
    }
}

/*
 * Does nothing unless gate is nonzero, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0,
 * &g_SnoboAltSlot156EffectPos0).
 */
// FUNCTION: FF7 0x0073deec
void __cdecl Mini_SnowboardAltSlot156SpawnEffectAbove(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltSlot156EffectPos0);
    }
}

/*
 * Gated on gate being nonzero; calls Mini_SnowboardEmitTableTypedEffect twice with (short)slot, the constant 0xA00 and
 * sub-index 0, passing the two consecutive twelve-byte records at g_SnoboAltSlot161EffectPos0 and g_SnoboAltSlot161EffectPos0[1].
 */
// FUNCTION: FF7 0x0073df10
void __cdecl Mini_SnowboardAltSlot161SpawnTwoEffects(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardEmitTableTypedEffect((short)slot, 0xA00, 0, g_SnoboAltSlot161EffectPos0);
        Mini_SnowboardEmitTableTypedEffect((short)slot, 0xA00, 0, &g_SnoboAltSlot161EffectPos0[1]);
    }
}

/*
 * Prepares g_SnoboEffectTemplateTyped with the g_SnoboTypedEffectModels word indexed by narrowed
 * subIndex, param and the three words at record, then calls
 * Mini_SnowboardAllocEffectNodeFromTemplate for slot.
 */
// FUNCTION: FF7 0x0073df4d
void __cdecl Mini_SnowboardEmitTableTypedEffect(short slot, int param, int subIndex, SnowboardVec *record)
{
    g_SnoboEffectTemplateTyped.model = g_SnoboTypedEffectModels[(short)subIndex];
    g_SnoboEffectTemplateTyped.rotY = param;
    g_SnoboEffectTemplateTyped.sidePos = record->x;
    g_SnoboEffectTemplateTyped.height = record->y;
    g_SnoboEffectTemplateTyped.forward = record->z;
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboEffectTemplateTyped);
}

/*
 * Optionally creates the course effect, then for the matching segment and rows 0x18 through 0x2F
 * outside action ten/twelve tests floating height difference divided by 4096. Selects action
 * twelve for 0x50 through 0x1BF or ten for 0x1C0 through 0x1C7 with sound; other ranges leave
 * action unchanged.
 */
// FUNCTION: FF7 0x0073dfa4
void __cdecl Mini_SnowboardAltSlot163SpawnEffectAboveSetRiderAnim(int slot, int gate)
{
    int heightDelta;

    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 2, &g_SnoboAltSlot163EffectPos0);
    }
    if (slot == g_SnoboStatePtr->section && g_SnoboStatePtr->triangle >= 24 && g_SnoboStatePtr->triangle <= 47) {
        switch (*(char *)&g_SnoboStatePtr->riderState) {
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0A:
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0C:
            return;
        }
        heightDelta = (int)((g_SnoboStatePtr->terrainHeight - g_SnoboStatePtr->relPosY) / 4096.0);
        if (heightDelta < 80) {
            return;
        }
        if (heightDelta < 448) {
            g_SnoboStatePtr->riderState = 0xC;
            Mini_SnowboardPlaySfxOnChannel(2, 0xF0, 0);
        } else if (heightDelta < 456) {
            g_SnoboStatePtr->riderState = 0xA;
            Mini_SnowboardPlaySfxOnChannel(2, 0xF0, 0);
        }
    }
}

/*
 * Gated on gate; indexes the twelve-byte record table g_SnoboStatePtr->effectPosTable by slot, biases
 * its three ints by -0x25A, +2 and -0x1BA, shifts each left by 12 into the 20.12 fixed-point
 * scratch triple g_SnoboAltTableTypedPos2, then calls Mini_SnowboardEmitTableTypedEffect((short)slot,
 * 0xA00, 0, &g_SnoboAltTableTypedPos2).
 */
// FUNCTION: FF7 0x0073e0b0
void __cdecl Mini_SnowboardAltMultiSlotSpawnOffsetEffect_1(int slot, int gate)
{
    SnowboardEffectPos *recordPtr;

    if (gate != 0) {
        recordPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
        g_SnoboAltTableTypedPos2.x = recordPtr->posX - 602 << 12;
        g_SnoboAltTableTypedPos2.y = recordPtr->posY + 2 << 12;
        g_SnoboAltTableTypedPos2.z = recordPtr->posZ - 442 << 12;
        Mini_SnowboardEmitTableTypedEffect((short)slot, 0xA00, 0, &g_SnoboAltTableTypedPos2);
    }
}

/*
 * Same shape as Mini_SnowboardAltMultiSlotSpawnOffsetEffect_1 with different biases: reads the twelve-byte record
 * g_SnoboStatePtr->effectPosTable + slot * 0xC, writes (x + 0x6C1) << 12, (y + 2) << 12 and (z - 0xADB) << 12 into
 * g_SnoboAltTableTypedPos1, then calls Mini_SnowboardEmitTableTypedEffect((short)slot, 0xA00, 0,
 * &g_SnoboAltTableTypedPos1). Runs only when gate is nonzero.
 */
// FUNCTION: FF7 0x0073e128
void __cdecl Mini_SnowboardAltMultiSlotEmitOffsetTypedEffect(int slot, int gate)
{
    SnowboardEffectPos *recordPtr;

    if (gate != 0) {
        recordPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
        g_SnoboAltTableTypedPos1.x = recordPtr->posX + 1729 << 12;
        g_SnoboAltTableTypedPos1.y = recordPtr->posY + 2 << 12;
        g_SnoboAltTableTypedPos1.z = recordPtr->posZ - 2779 << 12;
        Mini_SnowboardEmitTableTypedEffect((short)slot, 0xA00, 0, &g_SnoboAltTableTypedPos1);
    }
}

/*
 * Gated on gate; takes the twelve-byte record g_SnoboStatePtr->effectPosTable + slot * 0xC, writes (x +
 * 0xF) << 12, (y - 0x13D) << 12 and (z - 0x2C8) << 12 into the fixed-point triple g_SnoboAltTemplateF0Pos3, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltTemplateF0Pos3).
 */
// FUNCTION: FF7 0x0073e1a0
void __cdecl Mini_SnowboardAltMultiSlotSpawnOffsetEffectAbove(int slot, int gate)
{
    SnowboardEffectPos *recordPtr;

    if (gate != 0) {
        recordPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
        g_SnoboAltTemplateF0Pos3.vecX = recordPtr->posX + 15 << 12;
        g_SnoboAltTemplateF0Pos3.vecY = recordPtr->posY - 317 << 12;
        g_SnoboAltTemplateF0Pos3.vecZ = recordPtr->posZ - 712 << 12;
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltTemplateF0Pos3);
    }
}

/*
 * Gated on gate; takes the twelve-byte record g_SnoboStatePtr->effectPosTable + slot * 0xC, writes (x +
 * 0x5B7) << 12, (y - 0x13D) << 12 and (z - 0x870) << 12 into g_SnoboAltTemplateF0Pos1, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltTemplateF0Pos1).
 */
// FUNCTION: FF7 0x0073e217
void __cdecl Mini_SnowboardAltMultiSlotSpawnOffsetEffect_2(int slot, int gate)
{
    SnowboardEffectPos *recordPtr;

    if (gate != 0) {
        recordPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
        g_SnoboAltTemplateF0Pos1.vecX = recordPtr->posX + 1463 << 12;
        g_SnoboAltTemplateF0Pos1.vecY = recordPtr->posY - 317 << 12;
        g_SnoboAltTemplateF0Pos1.vecZ = recordPtr->posZ - 2160 << 12;
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltTemplateF0Pos1);
    }
}

/*
 * Gated on gate; takes the twelve-byte record g_SnoboStatePtr->effectPosTable + slot * 0xC, writes (x -
 * 0xA1) << 12, (y - 0x17A) << 12 and (z - 0x377) << 12 into g_SnoboAltTemplateF0Pos2, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 1, &g_SnoboAltTemplateF0Pos2).
 */
// FUNCTION: FF7 0x0073e291
void __cdecl Mini_SnowboardAltMultiSlotSpawnOffsetEffect_3(int slot, int gate)
{
    SnowboardEffectPos *recordPtr;

    if (gate != 0) {
        recordPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
        g_SnoboAltTemplateF0Pos2.vecX = recordPtr->posX - 161 << 12;
        g_SnoboAltTemplateF0Pos2.vecY = recordPtr->posY - 378 << 12;
        g_SnoboAltTemplateF0Pos2.vecZ = recordPtr->posZ - 887 << 12;
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 1, &g_SnoboAltTemplateF0Pos2);
    }
}

/*
 * Gated on gate; takes the twelve-byte record g_SnoboStatePtr->effectPosTable + slot * 0xC, writes (x +
 * 0x507) << 12, (y - 0x17A) << 12 and (z - 0x91F) << 12 into g_SnoboAltTemplateF0Pos4, then calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 1, &g_SnoboAltTemplateF0Pos4).
 */
// FUNCTION: FF7 0x0073e30b
void __cdecl Mini_SnowboardAltMultiSlotSpawnOffsetEffect_4(int slot, int gate)
{
    SnowboardEffectPos *recordPtr;

    if (gate != 0) {
        recordPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
        g_SnoboAltTemplateF0Pos4.vecX = recordPtr->posX + 1287 << 12;
        g_SnoboAltTemplateF0Pos4.vecY = recordPtr->posY - 378 << 12;
        g_SnoboAltTemplateF0Pos4.vecZ = recordPtr->posZ - 2335 << 12;
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 1, &g_SnoboAltTemplateF0Pos4);
    }
}

// Does nothing unless gate is nonzero. Otherwise it reads the twelve-byte record at
// g_SnoboStatePtr->effectPosTable + slot * 0xC and builds the fixed-point triple
// g_SnoboAltPairSpawnPos1 as (x + 0xD0) << 12, (y - 0x133) << 12 and (z - 0x7AC) << 12. It
// calls Mini_SnowboardSpawnEffectNodeTemplateF0 on that triple, then rewrites only the middle
// component to (y + 0x14D) << 12 and calls Mini_SnowboardSpawnKindEAEffectPlain on it. The two
// emissions share x and z and sit 0x280 apart on the middle axis.
// FUNCTION: FF7 0x0073e385
void __cdecl Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant1(int slot, int gate)
{
    SnowboardEffectPos *recordPtr;

    if (gate != 0) {
        recordPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
        g_SnoboAltPairSpawnPos1.vecX = recordPtr->posX + 208 << 12;
        g_SnoboAltPairSpawnPos1.vecY = recordPtr->posY - 307 << 12;
        g_SnoboAltPairSpawnPos1.vecZ = recordPtr->posZ - 1964 << 12;
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 1, &g_SnoboAltPairSpawnPos1);
        g_SnoboAltPairSpawnPos1.vecY = recordPtr->posY + 333 << 12;
        Mini_SnowboardSpawnKindEAEffectPlain((short)slot, 0xA00, 1, &g_SnoboAltPairSpawnPos1);
    }
}

// Does nothing unless gate is nonzero. Otherwise it reads the twelve-byte record at
// g_SnoboStatePtr->effectPosTable + slot * 0xC and builds g_SnoboAltPairSpawnPos2 as (x +
// 0xD0) << 12, (y - 0x173) << 12 and (z - 0x7AC) << 12. It calls
// Mini_SnowboardSpawnEffectNodeTemplateF0 with (slot, 0xA00, 2) on that triple, then rewrites the
// middle component to (y + 0x14D) << 12 and calls Mini_SnowboardSpawnKindEAEffectPlain with (slot,
// 0xA00, 1).
// FUNCTION: FF7 0x0073e431
void __cdecl Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant2(int slot, int gate)
{
    SnowboardEffectPos *recordPtr;

    if (gate != 0) {
        recordPtr = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
        g_SnoboAltPairSpawnPos2.vecX = recordPtr->posX + 208 << 12;
        g_SnoboAltPairSpawnPos2.vecY = recordPtr->posY - 371 << 12;
        g_SnoboAltPairSpawnPos2.vecZ = recordPtr->posZ - 1964 << 12;
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 2, &g_SnoboAltPairSpawnPos2);
        g_SnoboAltPairSpawnPos2.vecY = recordPtr->posY + 333 << 12;
        Mini_SnowboardSpawnKindEAEffectPlain((short)slot, 0xA00, 1, &g_SnoboAltPairSpawnPos2);
    }
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073e4dd
void __cdecl Mini_SnowboardAltSlot230NoOp(void)
{
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073e4e2
void __cdecl Mini_SnowboardAltSlot231NoOp(void)
{
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnCyclingEffectQuad with slot; otherwise performs no source-visible work.
 */
// FUNCTION: FF7 0x0073e4e7
void __cdecl Mini_SnowboardAltMultiSlotGatedSpawnFourEffects(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnCyclingEffectQuad(slot);
    }
}

/*
 * Creates four effects around the indexed course position using one of four template offset groups
 * and a cycling four-entry displacement table. Converts coordinates to fixed point by shifting
 * twelve, alternates the supplied angle by 0x100 and wraps the displacement index after each
 * spawn.
 */
// FUNCTION: FF7 0x0073e4fe
void __cdecl Mini_SnowboardSpawnCyclingEffectQuad(int slot)
{
    SnowboardCyclingEffectOffset *templatePtr;
    int count;
    SnowboardEffectPos *basePosition;
    int displacement;

    basePosition = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + slot * 0xC);
    templatePtr = (SnowboardCyclingEffectOffset *)(((short *)g_SnoboCyclingQuadOffsets) + (slot & 3) * 0xC);
    for (count = 4; count-- != 0; templatePtr++) {
        displacement = g_SnoboCyclingQuadDisplacements[g_SnoboCyclingQuadIndex];
        g_SnoboModelE6Pos.vecX = basePosition->posX + templatePtr->offsetX + displacement << 12;
        g_SnoboModelE6Pos.vecY = basePosition->posY + templatePtr->offsetY << 12;
        g_SnoboModelE6Pos.vecZ = basePosition->posZ + templatePtr->offsetZ + displacement << 12;
        Mini_SnowboardSpawnEffectNodeIdPlusE6((short)slot, ((count & 1) != 0 ? -0x100 : 0) + 0xe80, &g_SnoboModelE6Pos, 1);
        g_SnoboCyclingQuadIndex++;
        if (g_SnoboCyclingQuadIndex >= 4) {
            g_SnoboCyclingQuadIndex = 0;
        }
    }
}

/*
 * Prepares g_SnoboKindAngleEffectDef with kind plus 0xE6, angle and the three words at record, then calls
 * Mini_SnowboardAllocEffectNodeFromTemplate for slot.
 */
// FUNCTION: FF7 0x0073e607
void __cdecl Mini_SnowboardSpawnEffectNodeIdPlusE6(short slot, int angle, SnowboardSpawnPos *record, int kind)
{
    g_SnoboKindAngleEffectDef.model = kind + 0xE6;
    g_SnoboKindAngleEffectDef.rotY = angle;
    g_SnoboKindAngleEffectDef.sidePos = record->vecX;
    g_SnoboKindAngleEffectDef.height = record->vecY;
    g_SnoboKindAngleEffectDef.forward = record->vecZ;
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboKindAngleEffectDef);
}

/*
 * Gated on gate; calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot236EffectPos0) and then Mini_SnowboardSpawnCyclingEffectQuad
 * with slot.
 */
// FUNCTION: FF7 0x0073e659
void __cdecl Mini_SnowboardAltSlot236SpawnTemplateF0AndQuad(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot236EffectPos0);
        Mini_SnowboardSpawnCyclingEffectQuad(slot);
    }
}

/*
 * Gated on gate; calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot239EffectPos0) and then Mini_SnowboardSpawnCyclingEffectQuad
 * with slot. Identical to Mini_SnowboardAltSlot236SpawnTemplateF0AndQuad apart from the record address.
 */
// FUNCTION: FF7 0x0073e689
void __cdecl Mini_SnowboardAltSlot239SpawnTemplateF0AndQuad(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot239EffectPos0);
        Mini_SnowboardSpawnCyclingEffectQuad(slot);
    }
}

/*
 * Gated on gate; calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot243EffectPos0) and then Mini_SnowboardSpawnCyclingEffectQuad
 * with slot. Identical to Mini_SnowboardAltSlot236SpawnTemplateF0AndQuad apart from the record address.
 */
// FUNCTION: FF7 0x0073e6b9
void __cdecl Mini_SnowboardAltSlot243SpawnTemplateF0AndQuad(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot243EffectPos0);
        Mini_SnowboardSpawnCyclingEffectQuad(slot);
    }
}

/*
 * Gated on gate; calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot248EffectPos0) and then Mini_SnowboardSpawnCyclingEffectQuad
 * with slot. Identical to Mini_SnowboardAltSlot236SpawnTemplateF0AndQuad apart from the record address.
 */
// FUNCTION: FF7 0x0073e6e9
void __cdecl Mini_SnowboardAltSlot248SpawnTemplateF0AndQuad(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot248EffectPos0);
        Mini_SnowboardSpawnCyclingEffectQuad(slot);
    }
}

/*
 * Gated on gate; calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot252EffectPos0) and then Mini_SnowboardSpawnCyclingEffectQuad
 * with slot. Identical to Mini_SnowboardAltSlot236SpawnTemplateF0AndQuad apart from the record address.
 */
// FUNCTION: FF7 0x0073e719
void __cdecl Mini_SnowboardAltSlot252SpawnTemplateF0AndQuad(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot252EffectPos0);
        Mini_SnowboardSpawnCyclingEffectQuad(slot);
    }
}

/*
 * Gated on gate; calls Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot256EffectPos0) and then Mini_SnowboardSpawnCyclingEffectQuad
 * with slot. Identical to Mini_SnowboardAltSlot236SpawnTemplateF0AndQuad apart from the record address.
 */
// FUNCTION: FF7 0x0073e749
void __cdecl Mini_SnowboardAltSlot256SpawnTemplateF0AndQuad(int slot, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x600, 0, &g_SnoboAltSlot256EffectPos0);
        Mini_SnowboardSpawnCyclingEffectQuad(slot);
    }
}

/*
 * Gated on enabled; calls Mini_SnowboardSpawnKindEAEffectOnHit twice with (short)slot, 0xA00 and sub-index 2 for the
 * consecutive twelve-byte records at g_SnoboAltSlot265EffectPos0 and g_SnoboAltSlot265EffectPos0[1], then Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot,
 * 0xA00, 0, &g_SnoboAltSlot265EffectPos1).
 */
// FUNCTION: FF7 0x0073e779
void __cdecl Mini_SnowboardAltSlot265SpawnTwoEAHitAndF0(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xA00, 2, g_SnoboAltSlot265EffectPos0);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xA00, 2, &g_SnoboAltSlot265EffectPos0[1]);
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xA00, 0, &g_SnoboAltSlot265EffectPos1);
    }
}

/*
 * Does nothing unless enabled is nonzero, then calls Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xDC5, 2,
 * &g_SnoboAltSlot269EffectPos0).
 */
// FUNCTION: FF7 0x0073e7cf
void __cdecl Mini_SnowboardAltSlot269SpawnEAHit(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xDC5, 2, &g_SnoboAltSlot269EffectPos0);
    }
}

/*
 * When enabled is nonzero, narrows slot to short and issues Mini_SnowboardSpawnKindEAEffectOnHit calls with constant 0xe45
 * and mode 2 for g_SnoboAltSlot270EffectPos0 and g_SnoboAltSlot270EffectPos0[1], followed by Mini_SnowboardSpawnEffectNodeTemplateF0 with constant 0xe28 and
 * mode 1 for g_SnoboAltSlot270EffectPos1.
 */
// FUNCTION: FF7 0x0073e7f3
void __cdecl Mini_SnowboardAltSlot270SpawnTwoEAHitAndF0Mode1(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xE45, 2, g_SnoboAltSlot270EffectPos0);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)slot, 0xE45, 2, &g_SnoboAltSlot270EffectPos0[1]);
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xE28, 1, &g_SnoboAltSlot270EffectPos1);
    }
}

/*
 * When enabled is nonzero, calls Mini_SnowboardSpawnKindEAEffectPlain with slot narrowed to short, constant 0xf55, mode 2 and
 * record g_SnoboAltSlot277EffectPos0.
 */
// FUNCTION: FF7 0x0073e849
void __cdecl Mini_SnowboardAltSlot277SpawnEAPlain(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnKindEAEffectPlain((short)slot, 0xF55, 2, &g_SnoboAltSlot277EffectPos0);
    }
}

/*
 * When enabled is nonzero and the signed short in the module state exceeds 60, calls Mini_SnowboardSpawnEffectNodeTemplateF0
 * with slot narrowed to short, constant 0xbe3, mode 3 and record g_SnoboAltSlot301EffectPos0.
 */
// FUNCTION: FF7 0x0073e86d
void __cdecl Mini_SnowboardAltSlot301SpawnF0IfStateAbove60(int slot, int enabled)
{
    if (enabled != 0 && g_SnoboStatePtr->resultTotal > 60) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0xBE3, 3, &g_SnoboAltSlot301EffectPos0);
    }
}

/*
 * Guarded by enabled, submits the two adjacent 12-byte records at g_SnoboAltSlot316EffectPos0 and g_SnoboAltSlot316EffectPos0[1]
 * through Mini_SnowboardSpawnEffectNodeTag238 with slot narrowed to short.
 */
// FUNCTION: FF7 0x0073e89f
void __cdecl Mini_SnowboardAltSlot316SpawnTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot316EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot316EffectPos0[1]);
    }
}

// Spawns an effect node for the given slot from the shared template g_SnoboFixedTagEffectDef, with
// tag 0x238 written to rotY. The three dwords of record (vecX, vecY, vecZ) are copied
// to the template's sidePos, height and forward before
// Mini_SnowboardAllocEffectNodeFromTemplate allocates the node.
// FUNCTION: FF7 0x0073e8ce
void __cdecl Mini_SnowboardSpawnEffectNodeTag238(short slot, SnowboardSpawnPos *record)
{
    g_SnoboFixedTagEffectDef.rotY = 0x238;
    g_SnoboFixedTagEffectDef.sidePos = record->vecX;
    g_SnoboFixedTagEffectDef.height = record->vecY;
    g_SnoboFixedTagEffectDef.forward = record->vecZ;
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboFixedTagEffectDef);
}

/*
 * Guarded by enabled, passes the single 12-byte record at g_SnoboAltSlot317EffectPos0 to Mini_SnowboardSpawnEffectNodeTag238 with slot as
 * short.
 */
// FUNCTION: FF7 0x0073e910
void __cdecl Mini_SnowboardAltSlot317SpawnEffectTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot317EffectPos0);
    }
}

/*
 * Guarded by enabled, calls the Mini_SnowboardSpawnIndexedEffectNode with selector 0 on the record at g_SnoboAltSlot318EffectPos0, then
 * Mini_SnowboardSpawnEffectNodeTag238 on the record at g_SnoboAltSlot318EffectPos1, both with slot narrowed to short.
 */
// FUNCTION: FF7 0x0073e92d
void __cdecl Mini_SnowboardAltSlot318SpawnLinkedAndTag238Effect(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 0, &g_SnoboAltSlot318EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot318EffectPos1);
    }
}

// Fills g_SnoboIndexedEffectDef for the index, narrowed to a short: rotY comes from
// g_SnoboIndexedEffectAngles at that index, posX, posY and posZ from record with 0x80000 subtracted from
// posY, and index is stored. It then calls Mini_SnowboardAllocEffectNodeFromTemplate, loads the
// node's sideVelocity, fallVelocity and forwardVelocity from the 12-byte entries of g_SnoboIndexedEffectVelocities at
// that index, and sets fade to -1.
// FUNCTION: FF7 0x0073e95e
void __cdecl Mini_SnowboardSpawnIndexedEffectNode(short slot, int index, SnowboardVec *record)
{
    struct SnowboardEffect *node;

    g_SnoboIndexedEffectDef.rotY = g_SnoboIndexedEffectAngles[(short)index];
    g_SnoboIndexedEffectDef.homeX = record->x;
    g_SnoboIndexedEffectDef.homeY = record->y - 0x80000;
    g_SnoboIndexedEffectDef.homeZ = record->z;
    g_SnoboIndexedEffectDef.spinIndex = (short)index;
    node = (struct SnowboardEffect *)Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboIndexedEffectDef);
    node->sideVelocity = g_SnoboIndexedEffectVelocities[(short)index].x;
    node->fallVelocity = g_SnoboIndexedEffectVelocities[(short)index].y;
    node->forwardVelocity = g_SnoboIndexedEffectVelocities[(short)index].z;
    node->fade = 0xFFFF;
}

/*
 * When enabled is nonzero, narrows slot to short and submits g_SnoboAltSlot319EffectPos0 through Mini_SnowboardSpawnEffectNodeTag238, then
 * calls Mini_SnowboardSpawnIndexedEffectNode with selector 1 and g_SnoboAltSlot319EffectPos1.
 */
// FUNCTION: FF7 0x0073ea05
void __cdecl Mini_SnowboardAltSlot319SpawnEffectTag238AndIndexedNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot319EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 1, &g_SnoboAltSlot319EffectPos1);
    }
}

/*
 * Guarded by enabled, submits g_SnoboAltSlot320EffectPos0 through Mini_SnowboardSpawnEffectNodeTag238, then calls Mini_SnowboardSpawnIndexedEffectNode with
 * selector 0 on g_SnoboAltSlot320EffectPos1. Identical in shape to its neighbours, only the records and the
 * selector differ.
 */
// FUNCTION: FF7 0x0073ea36
void __cdecl Mini_SnowboardAltSlot320SpawnEffectTag238AndIndexedNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot320EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 0, &g_SnoboAltSlot320EffectPos1);
    }
}

/*
 * Guarded by enabled, submits g_SnoboAltSlot321EffectPos0 through Mini_SnowboardSpawnEffectNodeTag238, then calls Mini_SnowboardSpawnIndexedEffectNode with
 * selector 1 on g_SnoboAltSlot321EffectPos1.
 */
// FUNCTION: FF7 0x0073ea67
void __cdecl Mini_SnowboardAltSlot321SpawnEffectTag238AndIndexedNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot321EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 1, &g_SnoboAltSlot321EffectPos1);
    }
}

/*
 * Guarded by enabled, submits g_SnoboAltSlot322EffectPos0 through Mini_SnowboardSpawnEffectNodeTag238, then calls Mini_SnowboardSpawnIndexedEffectNode with
 * selector 0 on g_SnoboAltSlot322EffectPos1.
 */
// FUNCTION: FF7 0x0073ea98
void __cdecl Mini_SnowboardAltSlot322SpawnEffectTag238AndIndexedNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot322EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 0, &g_SnoboAltSlot322EffectPos1);
    }
}

/*
 * Guarded by enabled, submits g_SnoboAltSlot323EffectPos0 through Mini_SnowboardSpawnEffectNodeTag238, then calls Mini_SnowboardSpawnIndexedEffectNode with
 * selector 1 on g_SnoboAltSlot323EffectPos1.
 */
// FUNCTION: FF7 0x0073eac9
void __cdecl Mini_SnowboardAltSlot323SpawnEffectTag238AndIndexedNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot323EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 1, &g_SnoboAltSlot323EffectPos1);
    }
}

/*
 * Guarded by enabled, its only action is Mini_SnowboardSpawnIndexedEffectNode with selector 0 on the record at g_SnoboAltSlot324EffectPos0,
 * slot narrowed to short.
 */
// FUNCTION: FF7 0x0073eafa
void __cdecl Mini_SnowboardAltSlot324SpawnIndexedEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 0, &g_SnoboAltSlot324EffectPos0);
    }
}

/*
 * Guarded by enabled, its only action is passing the record at g_SnoboAltSlot325EffectPos0 to Mini_SnowboardSpawnEffectNodeTag238 with slot
 * as short.
 */
// FUNCTION: FF7 0x0073eb19
void __cdecl Mini_SnowboardAltSlot325SpawnEffectTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot325EffectPos0);
    }
}

/*
 * Guarded by enabled, submits the two adjacent 12-byte records at g_SnoboAltSlot326EffectPos0 and g_SnoboAltSlot326EffectPos0[1]
 * through Mini_SnowboardSpawnEffectNodeTag238.
 */
// FUNCTION: FF7 0x0073eb36
void __cdecl Mini_SnowboardAltSlot326SpawnTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot326EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot326EffectPos0[1]);
    }
}

/*
 * Guarded by enabled, submits the two adjacent 12-byte records at g_SnoboAltSlot327EffectPos0 and g_SnoboAltSlot327EffectPos0[1]
 * through Mini_SnowboardSpawnEffectNodeTag238.
 */
// FUNCTION: FF7 0x0073eb65
void __cdecl Mini_SnowboardAltSlot327SpawnTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot327EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot327EffectPos0[1]);
    }
}

/*
 * Guarded by enabled, submits the two adjacent 12-byte records at g_SnoboAltSlot329EffectPos0 and g_SnoboAltSlot329EffectPos0[1]
 * through Mini_SnowboardSpawnEffectNodeTag238.
 */
// FUNCTION: FF7 0x0073eb94
void __cdecl Mini_SnowboardAltSlot329SpawnTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot329EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot329EffectPos0[1]);
    }
}

/*
 * Guarded by enabled, its only action is Mini_SnowboardSpawnIndexedEffectNode with selector 2 on the record at g_SnoboAltSlot330EffectPos0.
 */
// FUNCTION: FF7 0x0073ebc3
void __cdecl Mini_SnowboardAltSlot330SpawnIndexedEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 2, &g_SnoboAltSlot330EffectPos0);
    }
}

/*
 * Guarded by enabled, calls Mini_SnowboardSpawnIndexedEffectNode with selector 3 on g_SnoboAltSlot331EffectPos0, then submits the two
 * adjacent records at g_SnoboAltSlot331EffectPos1 and g_SnoboAltSlot331EffectPos1[1] through Mini_SnowboardSpawnEffectNodeTag238. Note the out-of-order
 * record addresses: the Mini_SnowboardSpawnIndexedEffectNode argument sits after the two Mini_SnowboardSpawnEffectNodeTag238 ones in memory but
 * is used first.
 */
// FUNCTION: FF7 0x0073ebe2
void __cdecl Mini_SnowboardAltSlot331SpawnIndexedNodeAndTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 3, &g_SnoboAltSlot331EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot331EffectPos1);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot331EffectPos1[1]);
    }
}

/*
 * Guarded by enabled, makes two Mini_SnowboardSpawnIndexedEffectNode calls with slot as short: selector 3 on g_SnoboAltSlot332EffectPos0 and
 * selector 0 on the adjacent g_SnoboAltSlot332EffectPos0[1].
 */
// FUNCTION: FF7 0x0073ec25
void __cdecl Mini_SnowboardAltSlot332SpawnTwoIndexedEffectNodes(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 3, g_SnoboAltSlot332EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 0, &g_SnoboAltSlot332EffectPos0[1]);
    }
}

/*
 * Guarded by enabled, its only action is passing the record at g_SnoboAltSlot333EffectPos0 to Mini_SnowboardSpawnEffectNodeTag238 with slot
 * as short.
 */
// FUNCTION: FF7 0x0073ec58
void __cdecl Mini_SnowboardAltSlot333SpawnEffectTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot333EffectPos0);
    }
}

/*
 * Guarded by enabled, calls Mini_SnowboardSpawnIndexedEffectNode with selector 1 on g_SnoboAltSlot334EffectPos0, then submits g_SnoboAltSlot334EffectPos1
 * through Mini_SnowboardSpawnEffectNodeTag238 -- again the later record is used first.
 */
// FUNCTION: FF7 0x0073ec75
void __cdecl Mini_SnowboardAltSlot334SpawnIndexedNodeAndEffectTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 1, &g_SnoboAltSlot334EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot334EffectPos1);
    }
}

/*
 * Guarded by enabled, submits g_SnoboAltSlot335EffectPos0 through Mini_SnowboardSpawnEffectNodeTag238, then calls Mini_SnowboardSpawnIndexedEffectNode with
 * selector 0 on the earlier record g_SnoboAltSlot335EffectPos1.
 */
// FUNCTION: FF7 0x0073eca6
void __cdecl Mini_SnowboardAltSlot335SpawnEffectTag238AndIndexedNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot335EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 0, &g_SnoboAltSlot335EffectPos1);
    }
}

/*
 * Guarded by enabled, makes two Mini_SnowboardSpawnIndexedEffectNode calls: selector 2 on g_SnoboAltSlot336EffectPos0 and selector 1 on the
 * adjacent g_SnoboAltSlot336EffectPos0[1].
 */
// FUNCTION: FF7 0x0073ecd7
void __cdecl Mini_SnowboardAltSlot336SpawnTwoIndexedEffectNodes(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 2, g_SnoboAltSlot336EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 1, &g_SnoboAltSlot336EffectPos0[1]);
    }
}

/*
 * Guarded by enabled, its only action is passing the record at g_SnoboAltSlot337EffectPos0 to Mini_SnowboardSpawnEffectNodeTag238 with slot
 * as short.
 */
// FUNCTION: FF7 0x0073ed0a
void __cdecl Mini_SnowboardAltSlot337SpawnEffectTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot337EffectPos0);
    }
}

/*
 * Guarded entirely on enabled being nonzero; then narrows slot to short and hands the two 12-byte
 * records at g_SnoboAltSlot338EffectPos0 and g_SnoboAltSlot338EffectPos0[1] to Mini_SnowboardSpawnEffectNodeTag238, which stages each into the block at
 * g_SnoboFixedTagEffectDef under the fixed tag 0x238 and calls Mini_SnowboardAllocEffectNodeFromTemplate. Mechanical: one of a long run of
 * near-identical entries in this file that differ only in the record addresses.
 */
// FUNCTION: FF7 0x0073ed27
void __cdecl Mini_SnowboardAltSlot338SpawnTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot338EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot338EffectPos0[1]);
    }
}

/*
 * When enabled is nonzero, passes slot as a short plus the 12-byte records at g_SnoboAltSlot340EffectPos0 and
 * g_SnoboAltSlot340EffectPos0[1] to Mini_SnowboardSpawnEffectNodeTag238, which stages each into g_SnoboFixedTagEffectDef with tag 0x238 and calls
 * Mini_SnowboardAllocEffectNodeFromTemplate; does nothing at all when enabled is zero. Mechanical table entry, identical in shape
 * to its neighbours but for the two record addresses.
 */
// FUNCTION: FF7 0x0073ed56
void __cdecl Mini_SnowboardAltSlot340SpawnTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot340EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot340EffectPos0[1]);
    }
}

/*
 * When enabled is nonzero, narrows slot to short and submits g_SnoboAltSlot342EffectPos0 through Mini_SnowboardSpawnEffectNodeTag238, then
 * calls Mini_SnowboardSpawnIndexedEffectNode with selector 3 and g_SnoboAltSlot342EffectPos1, followed by selector 2 and g_SnoboAltSlot342EffectPos1[1].
 */
// FUNCTION: FF7 0x0073ed85
void __cdecl Mini_SnowboardAltSlot342SpawnEffectTag238AndTwoIndexedNodes(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot342EffectPos0);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 3, g_SnoboAltSlot342EffectPos1);
        Mini_SnowboardSpawnIndexedEffectNode((short)slot, 2, &g_SnoboAltSlot342EffectPos1[1]);
    }
}

/*
 * When enabled is nonzero, submits the 12-byte records at g_SnoboAltSlot344EffectPos0 and g_SnoboAltSlot344EffectPos0[1] for slot (as
 * short) through Mini_SnowboardSpawnEffectNodeTag238, which stages each into g_SnoboFixedTagEffectDef with tag 0x238 and calls
 * Mini_SnowboardAllocEffectNodeFromTemplate. Mechanical; the record addresses are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x0073edca
void __cdecl Mini_SnowboardAltSlot344SpawnTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot344EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot344EffectPos0[1]);
    }
}

/*
 * When enabled is nonzero, submits the 12-byte records at g_SnoboAltSlot346EffectPos0 and g_SnoboAltSlot346EffectPos0[1] for slot (as
 * short) through Mini_SnowboardSpawnEffectNodeTag238, which stages each into g_SnoboFixedTagEffectDef with tag 0x238 and calls
 * Mini_SnowboardAllocEffectNodeFromTemplate. Mechanical, and the last of the plain two-record Mini_SnowboardSpawnEffectNodeTag238 entries before the
 * run switches to Mini_SnowboardSpawnDirectedEffectNode.
 */
// FUNCTION: FF7 0x0073edf9
void __cdecl Mini_SnowboardAltSlot346SpawnTwoEffectsTag238(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, g_SnoboAltSlot346EffectPos0);
        Mini_SnowboardSpawnEffectNodeTag238((short)slot, &g_SnoboAltSlot346EffectPos0[1]);
    }
}

/*
 * When enabled is nonzero, calls Mini_SnowboardSpawnDirectedEffectNode with slot narrowed to short, tag 0xa38 and descriptor
 * g_SnoboAltSlot358EffectPos0.
 */
// FUNCTION: FF7 0x0073ee28
void __cdecl Mini_SnowboardAltSlot358SpawnTaggedEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnDirectedEffectNode((short)slot, 0xA38, &g_SnoboAltSlot358EffectPos0);
    }
}

// Stages tag, the spawn's position (sidePos, height, forward), a second copy of the height raised
// by 0x600000 and the two home words into the shared record g_SnoboTagVelocityFieldEffectDef, then
// calls Mini_SnowboardAllocEffectNodeFromTemplate with objectIndex. The returned node takes the
// spawn's side and forward velocity and its reversed flag.
// FUNCTION: FF7 0x0073ee4a
void __cdecl Mini_SnowboardSpawnDirectedEffectNode(short objectIndex, int tag, SnowboardEffectSpawn *spawn)
{
    struct SnowboardEffect *node;

    g_SnoboTagVelocityFieldEffectDef.rotY = tag;
    g_SnoboTagVelocityFieldEffectDef.sidePos = spawn->sidePos;
    g_SnoboTagVelocityFieldEffectDef.height = spawn->height;
    g_SnoboTagVelocityFieldEffectDef.heightLimit = spawn->height + 0x600000;
    g_SnoboTagVelocityFieldEffectDef.forward = spawn->forward;
    g_SnoboTagVelocityFieldEffectDef.homeX = spawn->homeX;
    g_SnoboTagVelocityFieldEffectDef.homeY = spawn->homeY;
    node = (struct SnowboardEffect *)Mini_SnowboardAllocEffectNodeFromTemplate((short)objectIndex, &g_SnoboTagVelocityFieldEffectDef);
    node->sideVelocity = spawn->sideVelocity;
    node->forwardVelocity = spawn->forwardVelocity;
    node->reversed = spawn->reversed;
}

/*
 * When spawnFlag is nonzero, calls Mini_SnowboardSpawnDirectedEffectNode with objectIndex as a short, the constant 0x1238 and the
 * record at g_SnoboAltSlot359EffectPos0; nothing otherwise.
 */
// FUNCTION: FF7 0x0073eedf
void __cdecl Mini_SnowboardAltSlot359SpawnEffectTag1238(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnDirectedEffectNode((short)objectIndex, 0x1238, &g_SnoboAltSlot359EffectPos0);
    }
}

/*
 * When spawnFlag is nonzero, calls Mini_SnowboardSpawnDirectedEffectNode with objectIndex as a short, the constant 0xA38 and the record
 * at g_SnoboAltSlot360EffectPos0. Mechanical dispatch entry.
 */
// FUNCTION: FF7 0x0073ef01
void __cdecl Mini_SnowboardAltSlot360SpawnEffectTagA38(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnDirectedEffectNode((short)objectIndex, 0xA38, &g_SnoboAltSlot360EffectPos0);
    }
}

/*
 * When spawnFlag is nonzero, calls Mini_SnowboardSpawnDirectedEffectNode with objectIndex as a short, the constant 0x1238 and the
 * record at g_SnoboAltSlot361EffectPos0. Mechanical dispatch entry.
 */
// FUNCTION: FF7 0x0073ef23
void __cdecl Mini_SnowboardAltSlot361SpawnEffectTag1238(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnDirectedEffectNode((short)objectIndex, 0x1238, &g_SnoboAltSlot361EffectPos0);
    }
}

/*
 * When spawnFlag is nonzero, calls Mini_SnowboardSpawnDirectedEffectNode with objectIndex as a short, the constant 0xA38 and the record
 * at g_SnoboAltSlot362EffectPos0. Mechanical dispatch entry.
 */
// FUNCTION: FF7 0x0073ef45
void __cdecl Mini_SnowboardAltSlot362SpawnEffectTagA38(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnDirectedEffectNode((short)objectIndex, 0xA38, &g_SnoboAltSlot362EffectPos0);
    }
}

/*
 * When spawnFlag is nonzero, makes two Mini_SnowboardSpawnDirectedEffectNode calls for objectIndex (as short): the record at
 * g_SnoboAltSlot363EffectPos0 with the constant 0xA38, then g_SnoboAltSlot363EffectPos1 with 0x1238. The only two-record entry
 * in the Mini_SnowboardSpawnDirectedEffectNode run, and the pair is 0x20 apart like the rest of that table.
 */
// FUNCTION: FF7 0x0073ef67
void __cdecl Mini_SnowboardAltSlot363SpawnEffectsTagA38AndTag1238(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnDirectedEffectNode((short)objectIndex, 0xA38, &g_SnoboAltSlot363EffectPos0);
        Mini_SnowboardSpawnDirectedEffectNode((short)objectIndex, 0x1238, &g_SnoboAltSlot363EffectPos1);
    }
}
} /* extern "C" */

extern "C" {

// clang-format off
// GLOBAL: FF7 0x00965938
struct SnowboardVec g_SnoboAltEffectPos_Slot371 = {-0x2bb0f000, 0x23a9f000, -0x3316c000};
// GLOBAL: FF7 0x00965948
struct SnowboardVec g_SnoboAltEffectPos_Slot373 = {-0x2c33d000, 0x23dbb000, -0x339d7000};
// GLOBAL: FF7 0x00965958
struct SnowboardEffectSpawn g_SnoboAltEffectSpawn_Slot374 = {-0x2C453000, 0x23E3D000, -0x33B67000, -0x2C13D000, -0x33680000, 0x13AA0, -0xDC20, 0};
// GLOBAL: FF7 0x00965978
struct SnowboardVec g_SnoboAltEffectPos_Slot375 = {-0x2c5f1000, 0x240e5000, -0x34610000};
// GLOBAL: FF7 0x00965988
struct SnowboardEffectSpawn g_SnoboAltEffectSpawn_Slot376 = {-0x2C6CF000, 0x24158000, -0x347D1000, -0x2CA40000, -0x33E00000, -0x13AA0, 0xDC20, 0};
// GLOBAL: FF7 0x009659a8
struct SnowboardVec g_SnoboAltEffectPos_Slot377 = {-0x2cd78000, 0x243ee000, -0x34efa000};
// GLOBAL: FF7 0x009659b8
struct SnowboardVec g_SnoboAltEffectPos_Slot379 = {-0x2d8a9000, 0x24727000, -0x3553b000};
// GLOBAL: FF7 0x009659c8
struct SnowboardEffectSpawn g_SnoboAltEffectSpawn_Slot380A = {-0x2D9BF000, 0x247AC000, -0x356D6000, -0x2D600000, -0x35000000, 0x13AA0, -0xDC20, 0};
// GLOBAL: FF7 0x009659e8
struct SnowboardEffectSpawn g_SnoboAltEffectSpawn_Slot380B = {-0x2DA16000, 0x247C6000, -0x3573B000, -0x2D7D0000, -0x35300000, 0x13AA0, -0xDC20, 0};
// GLOBAL: FF7 0x00965a08
struct SnowboardVec g_SnoboAltEffectPos_Slot381 = {-0x2d9c9000, 0x24a07000, -0x361f3000};
// GLOBAL: FF7 0x00965a18
struct SnowboardVec g_SnoboAltEffectPos_Slot383[2] = {{-0x2e62b000, 0x24d8b000, -0x36949000}, {-0x2e0c4000, 0x24d8b000, -0x36d11000}};
// GLOBAL: FF7 0x00965a30
struct SnowboardEffectSpawn g_SnoboAltEffectSpawn_Slot384A = {-0x2E1F1000, 0x24E00000, -0x36E8B000, -0x2E400000, -0x36380000, -0x13AA0, 0xDC20, 0};
// GLOBAL: FF7 0x00965a50
struct SnowboardEffectSpawn g_SnoboAltEffectSpawn_Slot384B = {-0x2E76D000, 0x24E28000, -0x36B51000, -0x2E554000, -0x36380000, 0x13AA0, -0xDC20, 0};
// GLOBAL: FF7 0x00965a70
struct SnowboardVec g_SnoboAltEffectPos_Slot385 = {-0x2e987000, 0x25054000, -0x373dd000};
// GLOBAL: FF7 0x00965a80
struct SnowboardVec g_SnoboAltEffectPos_Slot387 = {-0x2eda0000, 0x253c1000, -0x3804d000};
// GLOBAL: FF7 0x00965a90
struct SnowboardVec g_SnoboAltEffectPos_Slot389 = {-0x2fa8c000, 0x256dd000, -0x3856a000};
// GLOBAL: FF7 0x00965aa0
struct SnowboardVec g_SnoboAltEffectPos_Slot391 = {-0x2fd72000, 0x259a3000, -0x39055000};
// GLOBAL: FF7 0x00965ab0
struct SnowboardVec g_SnoboAltEffectPos_Slot393[2] = {{-0x307ba000, 0x25d10000, -0x3986a000}, {-0x3016a000, 0x25d10000, -0x39cdc000}};
// GLOBAL: FF7 0x00965ac8
struct SnowboardEffectSpawn g_SnoboAltEffectSpawn_Slot394A = {-0x30856000, 0x25D80000, -0x39A69000, -0x30630000, -0x39200000, 0x13AA0, -0xDC20, 0};
// GLOBAL: FF7 0x00965ae8
struct SnowboardEffectSpawn g_SnoboAltEffectSpawn_Slot394B = {-0x3032C000, 0x25D80000, -0x39E07000, -0x30560000, -0x39300000, -0x13AA0, 0xDC20, 0};
// GLOBAL: FF7 0x00965b08
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot401 = {-0x25eca000, 0x2211f000, -0x3621f000};
// GLOBAL: FF7 0x00965b18
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot415 = {-0x25dd3000, 0x24f43000, -0x3a188000};
// GLOBAL: FF7 0x00965b28
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot416[2] = {{-0x263bd000, 0x252cc000, -0x39da4000}, {-0x26498000, 0x252cc000, -0x39f7b000}};
// GLOBAL: FF7 0x00965b40
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot417A[2] = {{-0x268aa000, 0x2553f000, -0x39c73000}, {-0x2696b000, 0x2563f000, -0x39955000}};
// GLOBAL: FF7 0x00965b58
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot417B = {-0x26b84000, 0x2563f000, -0x39dd5000};
// GLOBAL: FF7 0x00965b68
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot424[2] = {{-0x2626c000, 0x263d9000, -0x38c9b000}, {-0x25fc2000, 0x264cb000, -0x38ba9000}};
// GLOBAL: FF7 0x00965b80
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot425 = {-0x25ba6000, 0x26743000, -0x38ead000};
// GLOBAL: FF7 0x00965b90
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot426[2] = {{-0x256b1000, 0x269db000, -0x38fe2000}, {-0x254d8000, 0x26b48000, -0x392f3000}};
// GLOBAL: FF7 0x00965ba8
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot427A[2] = {{-0x251ad000, 0x26ccd000, -0x39352000}, {-0x25030000, 0x26e45000, -0x39726000}};
// GLOBAL: FF7 0x00965bc0
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot427B = {-0x24dbe000, 0x26e49000, -0x391f6000};
// GLOBAL: FF7 0x00965bd0
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot428[2] = {{-0x24ca9000, 0x26fdc000, -0x396c3000}, {-0x24923000, 0x27147000, -0x39631000}};
// GLOBAL: FF7 0x00965be8
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot429A[2] = {{-0x245e8000, 0x27342000, -0x398cd000}, {-0x24895000, 0x2723e000, -0x399be000}};
// GLOBAL: FF7 0x00965c00
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot429B = {-0x246da000, 0x27243000, -0x3961a000};
// GLOBAL: FF7 0x00965c10
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot430[2] = {{-0x241cc000, 0x275da000, -0x39bd2000}, {-0x23f21000, 0x276cd000, -0x39adb000}};
// GLOBAL: FF7 0x00965c28
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot431A[2] = {{-0x23b04000, 0x27944000, -0x39ddf000}, {-0x23dc0000, 0x2783f000, -0x39ee9000}};
// GLOBAL: FF7 0x00965c40
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot431B = {-0x23bfb000, 0x27843000, -0x39b2d000};
// GLOBAL: FF7 0x00965c50
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot432A[2] = {{-0x236e8000, 0x27bdc000, -0x3a0e4000}, {-0x23440000, 0x27cce000, -0x39fec000}};
// GLOBAL: FF7 0x00965c68
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot432B[2] = {{-0x236a8000, 0x27c41000, -0x3a29c000}, {-0x2348c000, 0x27c42000, -0x39e14000}};
// GLOBAL: FF7 0x00965c80
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot439 = {-0x22e02000, 0x28b40000, -0x39018000};
// GLOBAL: FF7 0x00965c90
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot440[2] = {{-0x232fe000, 0x28dda000, -0x38ee4000}, {-0x233eb000, 0x28ec9000, -0x38c38000}};
// GLOBAL: FF7 0x00965ca8
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot441A[2] = {{-0x238e7000, 0x29142000, -0x38b06000}, {-0x239ed000, 0x29143000, -0x38d46000}};
// GLOBAL: FF7 0x00965cc0
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot441B[2] = {{-0x23962000, 0x29145000, -0x38c1c000}, {-0x2385b000, 0x29144000, -0x389e7000}};
// GLOBAL: FF7 0x00965cd8
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot441C = {-0x237d0000, 0x29141000, -0x388be000};
// GLOBAL: FF7 0x00965ce8
struct SnowboardSpawnPos g_SnoboAltEffectPos_Slot442[2] = {{-0x23cee000, 0x29348000, -0x38a41000}, {-0x23def000, 0x2944d000, -0x38796000}};
// GLOBAL: FF7 0x00965d00
struct SnowboardVec g_SnoboAltEffectPos_Slot465[2] = {{-0x13a4e000, 0x20821000, -0x324a1000}, {-0x1394f000, 0x20821000, -0x323a3000}};
// GLOBAL: FF7 0x00965d18
struct SnowboardVec g_SnoboAltEffectPos_Slot467[2] = {{-0x13231000, 0x20a33000, -0x32779000}, {-0x13664000, 0x20a33000, -0x32baa000}};
// GLOBAL: FF7 0x00965d30
struct SnowboardSelectorEffect g_SnoboAltEffectPos_Slot468[2] = {{-0x12932000, 0x20ccc000, -0x32c2a000}, {-0x13476000, 0x20bbd000, -0x331da000}};
// GLOBAL: FF7 0x00965d48
struct SnowboardVec g_SnoboAltEffectPos_Slot469[2] = {{-0x1277c000, 0x20dcf000, -0x32fd3000}, {-0x12e06000, 0x20dcf000, -0x3365b000}};
// GLOBAL: FF7 0x00965d60
struct SnowboardSelectorEffect g_SnoboAltEffectPos_Slot470[2] = {{-0x120ae000, 0x21005000, -0x334ae000}, {-0x12c8a000, 0x20ebc000, -0x339c6000}};
// GLOBAL: FF7 0x00965d78
struct SnowboardVec g_SnoboAltEffectPos_Slot473[2] = {{-0x11888000, 0x2135e000, -0x33e42000}, {-0x117e3000, 0x21426000, -0x341ba000}};
// GLOBAL: FF7 0x00965d90
struct SnowboardVec g_SnoboAltEffectPos_Slot474[2] = {{-0x11742000, 0x214ec000, -0x34531000}, {-0x1169f000, 0x215b3000, -0x348a9000}};
// GLOBAL: FF7 0x00965da8
struct SnowboardVec g_SnoboAltEffectPos_Slot475[2] = {{-0x115f6000, 0x2167a000, -0x34c1b000}, {-0x11552000, 0x2173e000, -0x34f87000}};
// GLOBAL: FF7 0x00965dc0
struct SnowboardSelectorEffect g_SnoboAltEffectPos_Slot477 = {-0x10ed7000, 0x219f8000, -0x35778000};
// GLOBAL: FF7 0x00965dd0
struct SnowboardSelectorEffect g_SnoboAltEffectPos_Slot478 = {-0xffdf000, 0x21c70000, -0x3557e000};
// GLOBAL: FF7 0x00965de0
struct SnowboardVec g_SnoboAltEffectPos_Slot479[2] = {{-0x10115000, 0x21d50000, -0x35b64000}, {-0x1027f000, 0x21d50000, -0x35cce000}};
// clang-format on

#pragma intrinsic(abs)

/*
 * When enabled is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition with slotIndex as a short, the tag 0x98E and the 12-byte
 * record at g_SnoboAltEffectPos_Slot371; does nothing otherwise. First of the Mini_SnowboardSpawnEffectNodeTaggedPosition run, which stages into
 * the block at g_SnoboTaggedPositionEffectDef rather than the g_SnoboFixedTagEffectDef one used by the earlier entries.
 */
// FUNCTION: FF7 0x0073efa0
void __cdecl Mini_SnowboardAltSlot371SpawnEffectTag98E(int slotIndex, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)slotIndex, 0x98E, &g_SnoboAltEffectPos_Slot371);
    }
}

// Stages a position effect in the shared block g_SnoboTaggedPositionEffectDef. The tag goes into
// m_1c, and the three ints of position are written at +0x22, +0x26 and +0x2a, which are 4 bytes
// apart at odd offsets from the block base. It then calls Mini_SnowboardAllocEffectNodeFromTemplate
// with slotIndex and the block. This is the same shape as Mini_SnowboardSpawnEffectNodeTag238,
// which fills the parallel block g_SnoboFixedTagEffectDef and hardcodes its tag to 0x238. Here the
// tag is a parameter, and every caller in this file passes 0x98E.
// FUNCTION: FF7 0x0073efc2
void __cdecl Mini_SnowboardSpawnEffectNodeTaggedPosition(short slotIndex, int tag, SnowboardVec *position)
{
    g_SnoboTaggedPositionEffectDef.rotY = tag;
    g_SnoboTaggedPositionEffectDef.sidePos = position->x;
    g_SnoboTaggedPositionEffectDef.height = position->y;
    g_SnoboTaggedPositionEffectDef.forward = position->z;
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slotIndex, &g_SnoboTaggedPositionEffectDef);
}

/*
 * When enabled is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition with slotIndex as a short, the tag 0x98E and the 12-byte
 * record at g_SnoboAltEffectPos_Slot373, which Mini_SnowboardSpawnEffectNodeTaggedPosition copies into the block at g_SnoboTaggedPositionEffectDef before calling
 * Mini_SnowboardAllocEffectNodeFromTemplate. Mechanical; only the record address varies from its neighbours.
 */
// FUNCTION: FF7 0x0073f005
void __cdecl Mini_SnowboardAltSlot373SpawnEffectTag98E(int slotIndex, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)slotIndex, 0x98E, &g_SnoboAltEffectPos_Slot373);
    }
}

/*
 * When enabled is nonzero, calls Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ with slotIndex as a short, the constant 0xD8E and the record
 * at g_SnoboAltEffectSpawn_Slot374.
 */
// FUNCTION: FF7 0x0073f027
void __cdecl Mini_SnowboardAltSlot374SpawnEffectTagD8E(int slotIndex, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ((short)slotIndex, 0xD8E, &g_SnoboAltEffectSpawn_Slot374);
    }
}

// Stages the tag in g_SnoboTagVelocityEffectDef.rotY, the spawn's sideVelocity, y and forwardVelocity at
// +0x22, +0x26 and +0x2a, y plus 0x600000 at +0x2e, and tagVelX and tagVelZ in m_38 and m_3c. It
// calls Mini_SnowboardAllocEffectNodeFromTemplate with slotIndex and the record, then writes
// nodeVelX and nodeVelZ into the returned node at +0x6A and +0x72 without checking that the node is
// non-null.
// FUNCTION: FF7 0x0073f049
void __cdecl Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ(short slotIndex, int tag, SnowboardEffectSpawn *spawn)
{
    SnowboardEffect *node;

    g_SnoboTagVelocityEffectDef.rotY = tag;
    g_SnoboTagVelocityEffectDef.sidePos = spawn->sidePos;
    g_SnoboTagVelocityEffectDef.height = spawn->height;
    g_SnoboTagVelocityEffectDef.heightLimit = spawn->height + 0x600000;
    g_SnoboTagVelocityEffectDef.forward = spawn->forward;
    g_SnoboTagVelocityEffectDef.homeX = spawn->homeX;
    g_SnoboTagVelocityEffectDef.triggerPos = spawn->homeY;
    node = (SnowboardEffect *)Mini_SnowboardAllocEffectNodeFromTemplate((short)slotIndex, &g_SnoboTagVelocityEffectDef);
    node->sideVelocity = spawn->sideVelocity;
    node->forwardVelocity = spawn->forwardVelocity;
}

/*
 * When enabled is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition with slotIndex as a short, the tag 0x98E and the 12-byte
 * record at g_SnoboAltEffectPos_Slot375, which is staged into g_SnoboTaggedPositionEffectDef and handed to Mini_SnowboardAllocEffectNodeFromTemplate. Mechanical
 * table entry.
 */
// FUNCTION: FF7 0x0073f0d2
void __cdecl Mini_SnowboardAltSlot375SpawnEffectTag98E(int slotIndex, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)slotIndex, 0x98E, &g_SnoboAltEffectPos_Slot375);
    }
}

/*
 * When trigger is nonzero, calls Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ with riderId as a short, the constant 0x58E and the record
 * at g_SnoboAltEffectSpawn_Slot376; nothing otherwise.
 */
// FUNCTION: FF7 0x0073f0f4
void __cdecl Mini_SnowboardAltSlot376SpawnEffectTag58E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ((short)riderId, 0x58E, &g_SnoboAltEffectSpawn_Slot376);
    }
}

/*
 * When trigger is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition with riderId as a short, the tag 0x98E and the 12-byte
 * record at g_SnoboAltEffectPos_Slot377. Mechanical dispatch entry.
 */
// FUNCTION: FF7 0x0073f116
void __cdecl Mini_SnowboardAltSlot377SpawnEffectTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, &g_SnoboAltEffectPos_Slot377);
    }
}

/*
 * When trigger is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition with riderId as a short, the tag 0x98E and the 12-byte
 * record at g_SnoboAltEffectPos_Slot379. Mechanical dispatch entry, differing from Mini_SnowboardAltSlot377SpawnEffectTag98E only in the
 * record address.
 */
// FUNCTION: FF7 0x0073f138
void __cdecl Mini_SnowboardAltSlot379SpawnEffectTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, &g_SnoboAltEffectPos_Slot379);
    }
}

/*
 * When trigger is nonzero, makes two Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ calls for riderId (as short), both with the constant
 * 0xD8E, for the records at g_SnoboAltEffectSpawn_Slot380A and g_SnoboAltEffectSpawn_Slot380B, which are 0x20 apart.
 */
// FUNCTION: FF7 0x0073f15a
void __cdecl Mini_SnowboardAltSlot380SpawnTwoEffectsTagD8E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ((short)riderId, 0xD8E, &g_SnoboAltEffectSpawn_Slot380A);
        Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ((short)riderId, 0xD8E, &g_SnoboAltEffectSpawn_Slot380B);
    }
}

/*
 * When trigger is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition with riderId as a short, the tag 0x98E and the 12-byte
 * record at g_SnoboAltEffectPos_Slot381. Mechanical dispatch entry.
 */
// FUNCTION: FF7 0x0073f193
void __cdecl Mini_SnowboardAltSlot381SpawnEffectTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, &g_SnoboAltEffectPos_Slot381);
    }
}

/*
 * When trigger is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition twice for riderId (as short), both with the tag 0x98E, for
 * the adjacent 12-byte records at g_SnoboAltEffectPos_Slot383 and g_SnoboAltEffectPos_Slot383[1]. Mechanical; the pair is
 * contiguous, which is what the 12-byte gap says about the record stride here.
 */
// FUNCTION: FF7 0x0073f1b5
void __cdecl Mini_SnowboardAltSlot383SpawnTwoEffectsTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, g_SnoboAltEffectPos_Slot383);
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot383 + 0xc)));
    }
}

/*
 * When trigger is nonzero, makes two Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ calls for riderId (as short): the record at
 * g_SnoboAltEffectSpawn_Slot384A with the constant 0x58E, then g_SnoboAltEffectSpawn_Slot384B with 0xD8E, the two records 0x20 apart.
 * The only entry that passes both constants;
 */
// FUNCTION: FF7 0x0073f1ee
void __cdecl Mini_SnowboardAltSlot384SpawnEffectsTag58EAndTagD8E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ((short)riderId, 0x58E, &g_SnoboAltEffectSpawn_Slot384A);
        Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ((short)riderId, 0xD8E, &g_SnoboAltEffectSpawn_Slot384B);
    }
}

/*
 * When trigger is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition with riderId as a short, the tag 0x98E and the 12-byte
 * record at g_SnoboAltEffectPos_Slot385. Mechanical dispatch entry.
 */
// FUNCTION: FF7 0x0073f227
void __cdecl Mini_SnowboardAltSlot385SpawnEffectTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, &g_SnoboAltEffectPos_Slot385);
    }
}

/*
 * When trigger is nonzero, calls Mini_SnowboardSpawnEffectNodeTaggedPosition with riderId as a short, the tag 0x98E and the 12-byte
 * record at g_SnoboAltEffectPos_Slot387, which is staged into g_SnoboTaggedPositionEffectDef and passed to Mini_SnowboardAllocEffectNodeFromTemplate.
 * Mechanical, and the last entry of this run.
 */
// FUNCTION: FF7 0x0073f249
void __cdecl Mini_SnowboardAltSlot387SpawnEffectTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, &g_SnoboAltEffectPos_Slot387);
    }
}

// One of the file's uniform two-argument trigger handlers. A nonzero trigger makes one call to
// Mini_SnowboardSpawnEffectNodeTaggedPosition with riderId, tag 0x98E and the 12-byte record at
// g_SnoboAltEffectPos_Slot389, whose three ints become the effect's position. A zero trigger does
// nothing.
// FUNCTION: FF7 0x0073f26b
void __cdecl Mini_SnowboardAltSlot389SpawnEffectTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, &g_SnoboAltEffectPos_Slot389);
    }
}

/*
 * Identical in shape to Mini_SnowboardAltSlot389SpawnEffectTag98E one entry earlier, differing only in the record address: when
 * trigger is nonzero it calls Mini_SnowboardSpawnEffectNodeTaggedPosition(riderId, 0x98E, &g_SnoboAltEffectPos_Slot391), which copies that 12-byte
 * record into the g_SnoboTaggedPositionEffectDef submission block and passes it to Mini_SnowboardAllocEffectNodeFromTemplate.
 */
// FUNCTION: FF7 0x0073f28d
void __cdecl Mini_SnowboardAltSlot391SpawnEffectTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, &g_SnoboAltEffectPos_Slot391);
    }
}

/*
 * When trigger is nonzero it emits two records back to back through Mini_SnowboardSpawnEffectNodeTaggedPosition, both with code
 * 0x98E: first &g_SnoboAltEffectPos_Slot393, then g_SnoboAltEffectPos_Slot393[1]. The two addresses are 12 bytes apart, which is
 * the record stride Mini_SnowboardSpawnEffectNodeTaggedPosition reads (three ints at +0, +4, +8).
 */
// FUNCTION: FF7 0x0073f2af
void __cdecl Mini_SnowboardAltSlot393SpawnTwoEffectsTag98E(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, g_SnoboAltEffectPos_Slot393);
        Mini_SnowboardSpawnEffectNodeTaggedPosition((short)riderId, 0x98E, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot393 + 0xc)));
    }
}

/*
 * When trigger is nonzero, narrows riderId to short and calls Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ with code 0xd8e and
 * g_SnoboAltEffectSpawn_Slot394A, then code 0x58e and g_SnoboAltEffectSpawn_Slot394B.
 */
// FUNCTION: FF7 0x0073f2e8
void __cdecl Mini_SnowboardAltSlot394SpawnTwoVelocityEffects(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ((short)riderId, 0xD8E, &g_SnoboAltEffectSpawn_Slot394A);
        Mini_SnowboardSpawnEffectNodeTagWithVelocityXZ((short)riderId, 0x58E, &g_SnoboAltEffectSpawn_Slot394B);
    }
}

/*
 * Fires only when trigger is nonzero and the short at offset 0x78 of the structure pointed to by
 * g_SnoboStatePtr is greater than 0x50; in that case it calls Mini_SnowboardSpawnEffectNodeTemplateF0(riderId, 0x955, 3,
 * &g_SnoboAltEffectPos_Slot401). The 0x78 field is used as the same threshold gate in two other handlers in this
 * file, one of them against 0x3C instead.
 */
// FUNCTION: FF7 0x0073f321
void __cdecl Mini_SnowboardAltSlot401SpawnEffectTemplateF0Gated(int riderId, int trigger)
{
    if (trigger != 0 && g_SnoboStatePtr->resultTotal > 80) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)riderId, 0x955, 3, &g_SnoboAltEffectPos_Slot401);
    }
}

/*
 * When trigger is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0xD1C, 0,
 * &g_SnoboAltEffectPos_Slot415); either way it then calls
 * Mini_SnowboardCheckAltRiderState0BTrigger(riderId). This
 * guarded-emit-then-always-Mini_SnowboardCheckAltRiderState0BTrigger shape is shared by the whole
 * run of handlers from here to Mini_SnowboardAltSlot442SpawnTwoEffectsAndCheckState0B (0x0073f9e2).
 */
// FUNCTION: FF7 0x0073f353
void __cdecl Mini_SnowboardAltSlot415SpawnEffectAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0xD1C, 0, &g_SnoboAltEffectPos_Slot415);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * Prepares g_SnoboSelectorEffectDef with narrowed selector times three plus 0xF8, effectCode and the three words at
 * record, also retaining the middle word plus 0x400000, and returns Mini_SnowboardAllocEffectNodeFromTemplate(ownerId,template).
 */
// FUNCTION: FF7 0x0073f383
int __cdecl Mini_SnowboardSpawnEffectNodeTemplateF8(short ownerId, int effectCode, int selector, SnowboardSpawnPos *record)
{
    g_SnoboSelectorEffectDef.model = (short)selector * 3 + 0xF8;
    g_SnoboSelectorEffectDef.rotY = effectCode;
    g_SnoboSelectorEffectDef.sidePos = record->vecX;
    g_SnoboSelectorEffectDef.forward = record->vecZ;
    g_SnoboSelectorEffectDef.height = record->vecY;
    g_SnoboSelectorEffectDef.heightLimit = record->vecY + 0x400000;
    return Mini_SnowboardAllocEffectNodeFromTemplate((short)ownerId, &g_SnoboSelectorEffectDef);
}

// Calls Mini_SnowboardEnterAltRiderState0BPlaySfx when the rider is in section riderId and
// riderState is not 0xB, and the integer-converted difference between terrainHeight and relPosY,
// shifted right twelve bits, exceeds 0x240. Returns early otherwise.
// FUNCTION: FF7 0x0073f3e9
void __cdecl Mini_SnowboardCheckAltRiderState0BTrigger(int riderId)
{
    if (g_SnoboStatePtr->section != riderId) {
        return;
    }
    if (*(unsigned char *)&g_SnoboStatePtr->riderState == 0xB) {
        return;
    }
    if ((int)(g_SnoboStatePtr->terrainHeight - g_SnoboStatePtr->relPosY) >> 12 > 576) {
        Mini_SnowboardEnterAltRiderState0BPlaySfx();
    }
}

/*
 * Sets context+0x154 to 0xB, replaces its +0x17C float with the absolute integer-converted value
 * clamped to at least 0x40000 and plays channel-2 sound 0x10C.
 */
// FUNCTION: FF7 0x0073f442
void __cdecl Mini_SnowboardEnterAltRiderState0BPlaySfx(void)
{
    int val;

    g_SnoboStatePtr->riderState = 0xB;
    val = (int)g_SnoboStatePtr->fadeLevel;
    if (val < 0) {
        val = -val;
    }
    if (val < 262144) {
        val = 0x40000;
    }
    g_SnoboStatePtr->fadeLevel = (float)val;
    Mini_SnowboardPlaySfxOnChannel(2, 0x10C, 0);
}

/*
 * When trigger is nonzero it emits two records through Mini_SnowboardSpawnEffectNodeTemplateF8 with code 0xD1C and selectors 1
 * and 2: &g_SnoboAltEffectPos_Slot416 and g_SnoboAltEffectPos_Slot416[1], 12 bytes apart. It then always calls Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f4a9
void __cdecl Mini_SnowboardAltSlot416SpawnTwoEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0xD1C, 1, g_SnoboAltEffectPos_Slot416);
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0xD1C, 2, &g_SnoboAltEffectPos_Slot416[1]);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * When trigger is nonzero it emits three records: Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0xD1C, 0, &g_SnoboAltEffectPos_Slot417A), then
 * Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0xD1C, 0xF9, g_SnoboAltEffectPos_Slot417A[1]) and Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0x151C, 0xFC,
 * &g_SnoboAltEffectPos_Slot417B). The three record addresses run consecutively at a 12-byte stride. It then always
 * calls Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f4f2
void __cdecl Mini_SnowboardAltSlot417SpawnThreeEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0xD1C, 0, g_SnoboAltEffectPos_Slot417A);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0xD1C, 0xF9, &g_SnoboAltEffectPos_Slot417A[1]);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0x151C, 0xFC, &g_SnoboAltEffectPos_Slot417B);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * Prepares g_SnoboTemplateIdEffectDef with templateId, effectCode and the three words at record, also retaining the middle
 * word plus 0x400000, and returns Mini_SnowboardAllocEffectNodeFromTemplate(ownerId,template).
 */
// FUNCTION: FF7 0x0073f55a
int __cdecl Mini_SnowboardSpawnEffectNodeTemplateWithId(short ownerId, int effectCode, int templateId, SnowboardSpawnPos *record)
{
    g_SnoboTemplateIdEffectDef.model = templateId;
    g_SnoboTemplateIdEffectDef.rotY = effectCode;
    g_SnoboTemplateIdEffectDef.sidePos = record->vecX;
    g_SnoboTemplateIdEffectDef.height = record->vecY;
    g_SnoboTemplateIdEffectDef.forward = record->vecZ;
    g_SnoboTemplateIdEffectDef.heightLimit = record->vecY + 0x400000;
    return Mini_SnowboardAllocEffectNodeFromTemplate((short)ownerId, &g_SnoboTemplateIdEffectDef);
}

/*
 * When trigger is nonzero it calls Mini_SnowboardSpawnEffectNodeTemplateF8 twice with code 0x51C and selectors 1 and 2, on the
 * adjacent 12-byte records &g_SnoboAltEffectPos_Slot424 and g_SnoboAltEffectPos_Slot424[1]. It then always calls
 * Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f5b8
void __cdecl Mini_SnowboardAltSlot424SpawnTwoEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 1, g_SnoboAltEffectPos_Slot424);
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 2, &g_SnoboAltEffectPos_Slot424[1]);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * When trigger is nonzero it makes the single call Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 0, &g_SnoboAltEffectPos_Slot425);
 * either way it then calls Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f601
void __cdecl Mini_SnowboardAltSlot425SpawnEffectAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 0, &g_SnoboAltEffectPos_Slot425);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * Same shape as Mini_SnowboardAltSlot424SpawnTwoEffectsAndCheckState0B: on nonzero trigger it calls Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 1, &g_SnoboAltEffectPos_Slot426)
 * and Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 2, g_SnoboAltEffectPos_Slot426[1]), then always Mini_SnowboardCheckAltRiderState0BTrigger(riderId). Only the two
 * record addresses differ.
 */
// FUNCTION: FF7 0x0073f631
void __cdecl Mini_SnowboardAltSlot426SpawnTwoEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 1, g_SnoboAltEffectPos_Slot426);
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 2, &g_SnoboAltEffectPos_Slot426[1]);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * When trigger is nonzero it emits three records: Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 0, &g_SnoboAltEffectPos_Slot427A),
 * Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0x51C, 0xFF, g_SnoboAltEffectPos_Slot427A[1]) and Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0xD1C, 0xF9,
 * &g_SnoboAltEffectPos_Slot427B), consecutive at the 12-byte record stride. It then always calls
 * Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f67a
void __cdecl Mini_SnowboardAltSlot427SpawnThreeEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 0, g_SnoboAltEffectPos_Slot427A);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0x51C, 0xFF, &g_SnoboAltEffectPos_Slot427A[1]);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0xD1C, 0xF9, &g_SnoboAltEffectPos_Slot427B);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * Another selector-1-then-selector-2 pair: on nonzero trigger it calls Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 1,
 * &g_SnoboAltEffectPos_Slot428) and Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 2, g_SnoboAltEffectPos_Slot428[1]), then always Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f6e2
void __cdecl Mini_SnowboardAltSlot428SpawnTwoEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 1, g_SnoboAltEffectPos_Slot428);
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 2, &g_SnoboAltEffectPos_Slot428[1]);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * When trigger is nonzero it emits Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 0, &g_SnoboAltEffectPos_Slot429A) followed by
 * Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0x51C, 0xFC, g_SnoboAltEffectPos_Slot429A[1]) and Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0xD1C, 0xFF,
 * &g_SnoboAltEffectPos_Slot429B). It then always calls Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f72b
void __cdecl Mini_SnowboardAltSlot429SpawnThreeEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 0, g_SnoboAltEffectPos_Slot429A);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0x51C, 0xFC, &g_SnoboAltEffectPos_Slot429A[1]);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0xD1C, 0xFF, &g_SnoboAltEffectPos_Slot429B);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * On nonzero trigger it calls Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 1, &g_SnoboAltEffectPos_Slot430) and Mini_SnowboardSpawnEffectNodeTemplateF8(riderId,
 * 0x51C, 2, g_SnoboAltEffectPos_Slot430[1]), then always Mini_SnowboardCheckAltRiderState0BTrigger(riderId). Mechanically identical to Mini_SnowboardAltSlot428SpawnTwoEffectsAndCheckState0B
 * apart from the record addresses.
 */
// FUNCTION: FF7 0x0073f793
void __cdecl Mini_SnowboardAltSlot430SpawnTwoEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 1, g_SnoboAltEffectPos_Slot430);
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 2, &g_SnoboAltEffectPos_Slot430[1]);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * On nonzero trigger it emits Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 0, &g_SnoboAltEffectPos_Slot431A), Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0x51C,
 * 0xFC, g_SnoboAltEffectPos_Slot431A[1]) and Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0xD1C, 0xFF, &g_SnoboAltEffectPos_Slot431B) - the same code and
 * selector triple as Mini_SnowboardAltSlot429SpawnThreeEffectsAndCheckState0B on a later run of records. It then always calls
 * Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f7dc
void __cdecl Mini_SnowboardAltSlot431SpawnThreeEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 0, g_SnoboAltEffectPos_Slot431A);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0x51C, 0xFC, &g_SnoboAltEffectPos_Slot431A[1]);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0xD1C, 0xFF, &g_SnoboAltEffectPos_Slot431B);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * The longest of this run: on nonzero trigger it emits four records, Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 1,
 * &g_SnoboAltEffectPos_Slot432A), Mini_SnowboardSpawnEffectNodeTemplateF8(riderId, 0x51C, 2, g_SnoboAltEffectPos_Slot432A[1]), Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0x51C, 0xF9,
 * &g_SnoboAltEffectPos_Slot432B) and Mini_SnowboardSpawnEffectNodeTemplateWithId(riderId, 0xD1C, 0xFC, g_SnoboAltEffectPos_Slot432B[1]), consecutive at the 12-byte
 * stride. It then always calls Mini_SnowboardCheckAltRiderState0BTrigger(riderId).
 */
// FUNCTION: FF7 0x0073f844
void __cdecl Mini_SnowboardAltSlot432SpawnFourEffectsAndCheckState0B(int riderId, int trigger)
{
    if (trigger != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 1, g_SnoboAltEffectPos_Slot432A);
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)riderId, 0x51C, 2, &g_SnoboAltEffectPos_Slot432A[1]);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0x51C, 0xF9, g_SnoboAltEffectPos_Slot432B);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)riderId, 0xD1C, 0xFC, &g_SnoboAltEffectPos_Slot432B[1]);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(riderId);
}

/*
 * When enabled is nonzero it makes the single call Mini_SnowboardSpawnEffectNodeTemplateF8(slot, 0xD1C, 0, &g_SnoboAltEffectPos_Slot439);
 * either way it then calls Mini_SnowboardCheckAltRiderState0BTrigger(slot).
 */
// FUNCTION: FF7 0x0073f8c5
void __cdecl Mini_SnowboardAltSlot439SpawnEffectAndCheckState0B(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)slot, 0xD1C, 0, &g_SnoboAltEffectPos_Slot439);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(slot);
}

/*
 * On nonzero enabled it calls Mini_SnowboardSpawnEffectNodeTemplateF8 twice with code 0xD1C and selectors 1 and 2, on
 * &g_SnoboAltEffectPos_Slot440 and g_SnoboAltEffectPos_Slot440[1], then always Mini_SnowboardCheckAltRiderState0BTrigger(slot).
 */
// FUNCTION: FF7 0x0073f8f5
void __cdecl Mini_SnowboardAltSlot440SpawnTwoEffectsAndCheckState0B(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)slot, 0xD1C, 1, g_SnoboAltEffectPos_Slot440);
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)slot, 0xD1C, 2, &g_SnoboAltEffectPos_Slot440[1]);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(slot);
}

/*
 * The widest emit in this group: on nonzero enabled it calls Mini_SnowboardSpawnEffectNodeTemplateF8(slot, 0xD1C, 0,
 * &g_SnoboAltEffectPos_Slot441A) and then four Mini_SnowboardSpawnEffectNodeTemplateWithId calls - (0x151C, 0xFF, g_SnoboAltEffectPos_Slot441A[1]), (0x151C, 0xFC,
 * &g_SnoboAltEffectPos_Slot441B), (0xD1C, 0xFC, g_SnoboAltEffectPos_Slot441B[1]) and (0xD1C, 0xFF, &g_SnoboAltEffectPos_Slot441C) - five consecutive
 * 12-byte records in all. It then always calls Mini_SnowboardCheckAltRiderState0BTrigger(slot).
 */
// FUNCTION: FF7 0x0073f93e
void __cdecl Mini_SnowboardAltSlot441SpawnFiveEffectsAndCheckState0B(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)slot, 0xD1C, 0, g_SnoboAltEffectPos_Slot441A);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)slot, 0x151C, 0xFF, &g_SnoboAltEffectPos_Slot441A[1]);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)slot, 0x151C, 0xFC, g_SnoboAltEffectPos_Slot441B);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)slot, 0xD1C, 0xFC, &g_SnoboAltEffectPos_Slot441B[1]);
        Mini_SnowboardSpawnEffectNodeTemplateWithId((short)slot, 0xD1C, 0xFF, &g_SnoboAltEffectPos_Slot441C);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(slot);
}

/*
 * The last of the Mini_SnowboardSpawnEffectNodeTemplateF8 run: on nonzero enabled it emits code 0xD1C with selectors 1 and 2 on
 * &g_SnoboAltEffectPos_Slot442 and g_SnoboAltEffectPos_Slot442[1], then always calls Mini_SnowboardCheckAltRiderState0BTrigger(slot).
 */
// FUNCTION: FF7 0x0073f9e2
void __cdecl Mini_SnowboardAltSlot442SpawnTwoEffectsAndCheckState0B(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)slot, 0xD1C, 1, g_SnoboAltEffectPos_Slot442);
        Mini_SnowboardSpawnEffectNodeTemplateF8((short)slot, 0xD1C, 2, &g_SnoboAltEffectPos_Slot442[1]);
    }
    Mini_SnowboardCheckAltRiderState0BTrigger(slot);
}

/*
 * When enabled is nonzero, narrows slot to short and calls Mini_SnowboardSpawnE00EffectNode with selector 1 and
 * g_SnoboAltEffectPos_Slot465, then selector 0 and g_SnoboAltEffectPos_Slot465[1].
 */
// FUNCTION: FF7 0x0073fa2b
void __cdecl Mini_SnowboardAltSlot465SpawnTwoEffectsTagE00(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, g_SnoboAltEffectPos_Slot465);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot465 + 0xc)));
    }
}

/*
 * Prepares g_SnoboSelectedRecordEffectDef with narrowed selector plus 0x101, constant 0xE00 and the three words at record,
 * then calls Mini_SnowboardAllocEffectNodeFromTemplate for slot.
 */
// FUNCTION: FF7 0x0073fa5e
void __cdecl Mini_SnowboardSpawnE00EffectNode(short slot, int selector, SnowboardVec *record)
{
    g_SnoboSelectedRecordEffectDef.model = (short)selector + 0x101;
    g_SnoboSelectedRecordEffectDef.rotY = 0xE00 /* 315 deg */;
    g_SnoboSelectedRecordEffectDef.sidePos = record->x;
    g_SnoboSelectedRecordEffectDef.height = record->y;
    g_SnoboSelectedRecordEffectDef.forward = record->z;
    Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboSelectedRecordEffectDef);
}

/*
 * When the full int enabled is nonzero, narrows slot to short and calls Mini_SnowboardSpawnE00EffectNode with selector 0
 * and g_SnoboAltEffectPos_Slot467, then selector 1 and g_SnoboAltEffectPos_Slot467[1].
 */
// FUNCTION: FF7 0x0073faaf
void __cdecl Mini_SnowboardAltSlot467SpawnTwoEffectsTagE00(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, g_SnoboAltEffectPos_Slot467);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot467 + 0xc)));
    }
}

/*
 * When enabled is nonzero it calls Mini_SnowboardSpawnSelectorOffsetEffectNode(slot, 0x600, 0, &g_SnoboAltEffectPos_Slot468) then
 * Mini_SnowboardSpawnSelectorOffsetEffectNode(slot, 0x600, 1, g_SnoboAltEffectPos_Slot468[1]) - the only two uses of the four-argument
 * Mini_SnowboardSpawnSelectorOffsetEffectNode in this segment, both with code 0x600 and selectors 0 and 1.
 */
// FUNCTION: FF7 0x0073fae2
void __cdecl Mini_SnowboardAltSlot468SpawnTwoEffectsTag600(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 0, g_SnoboAltEffectPos_Slot468);
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 1, (SnowboardSelectorEffect *)(((char *)&g_SnoboAltEffectPos_Slot468 + 0xc)));
    }
}

// Fills the g_SnoboSelectorOffsetEffectDef template from the code, the record's baseValueA,
// selectorOffset and baseValueB, and offsets selected by the low two bits of the narrowed selector,
// then calls Mini_SnowboardAllocEffectNodeFromTemplate. The node's sideVelocity and forwardVelocity
// are set to the g_SnoboSelectorEffectSpeeds word at index selector, shifted left twelve bits, and fade is
// set to -1.
// FUNCTION: FF7 0x0073fb1f
void __cdecl Mini_SnowboardSpawnSelectorOffsetEffectNode(short slot, int code, int selector, SnowboardSelectorEffect *record)
{
    struct SnowboardEffect *node;
    int offset;

    offset = (((short)selector & 1) != 0 ? 0xFFFFFE80 : 0) + 0xC0 << 12;
    g_SnoboSelectorOffsetEffectDef.rotY = code;
    g_SnoboSelectorOffsetEffectDef.homeX = record->baseValueA + offset;
    g_SnoboSelectorOffsetEffectDef.homeY = record->selectorOffset - 0x400000;
    g_SnoboSelectorOffsetEffectDef.homeZ = record->baseValueB + offset;
    g_SnoboSelectorOffsetEffectDef.heightLimit = record->selectorOffset - 0x80000;
    g_SnoboSelectorOffsetEffectDef.cycleCounter = (((short)selector & 2) != 0 ? 0xFFFFFFD8 : 0) + 144;
    node = (struct SnowboardEffect *)Mini_SnowboardAllocEffectNodeFromTemplate((short)slot, &g_SnoboSelectorOffsetEffectDef);
    offset = g_SnoboSelectorEffectSpeeds[(short)selector] << 12;
    node->sideVelocity = offset;
    node->forwardVelocity = offset;
    node->fade = 0xFFFF;
}

/*
 * Byte-for-byte the same shape as Mini_SnowboardAltSlot467SpawnTwoEffectsTagE00 on a different pair of records: when enabled is
 * nonzero it calls Mini_SnowboardSpawnE00EffectNode(slot, 0, &g_SnoboAltEffectPos_Slot469) then Mini_SnowboardSpawnE00EffectNode(slot, 1, g_SnoboAltEffectPos_Slot469[1]).
 */
// FUNCTION: FF7 0x0073fbec
void __cdecl Mini_SnowboardAltSlot469SpawnTwoEffectsTagE00(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, g_SnoboAltEffectPos_Slot469);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot469 + 0xc)));
    }
}

/*
 * Does nothing unless enabled is non-zero. Then calls Mini_SnowboardSpawnSelectorOffsetEffectNode twice with slot narrowed to short,
 * the constant 0x600 that every call site in this file passes to that helper, selector 0 and then
 * selector 1, and the addresses of the two static records g_SnoboAltEffectPos_Slot470 and g_SnoboAltEffectPos_Slot470[1], which are
 * 12 bytes apart.
 */
// FUNCTION: FF7 0x0073fc1f
void __cdecl Mini_SnowboardAltSlot470SpawnTwoEffectsTag600(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 0, g_SnoboAltEffectPos_Slot470);
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 1, (SnowboardSelectorEffect *)(((char *)&g_SnoboAltEffectPos_Slot470 + 0xc)));
    }
}

/*
 * Gated on enabled being non-zero. Calls Mini_SnowboardSpawnE00EffectNode twice with (short)slot, selector 0 both times,
 * and the records g_SnoboAltEffectPos_Slot473 and g_SnoboAltEffectPos_Slot473[1].
 */
// FUNCTION: FF7 0x0073fc5c
void __cdecl Mini_SnowboardAltSlot473SpawnTwoEffectsTagE00(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, g_SnoboAltEffectPos_Slot473);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot473 + 0xc)));
    }
}

/*
 * Gated on enabled being non-zero. Calls Mini_SnowboardSpawnE00EffectNode with (short)slot and selector 0 for record
 * g_SnoboAltEffectPos_Slot474, then again with selector 1 for record g_SnoboAltEffectPos_Slot474[1].
 */
// FUNCTION: FF7 0x0073fc8f
void __cdecl Mini_SnowboardAltSlot474SpawnTwoEffectsTagE00(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, g_SnoboAltEffectPos_Slot474);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot474 + 0xc)));
    }
}

/*
 * Gated on enabled being non-zero. Calls Mini_SnowboardSpawnE00EffectNode twice with (short)slot, selector 1 both times,
 * and the records g_SnoboAltEffectPos_Slot475 and g_SnoboAltEffectPos_Slot475[1].
 */
// FUNCTION: FF7 0x0073fcc2
void __cdecl Mini_SnowboardAltSlot475SpawnTwoEffectsTagE00(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, g_SnoboAltEffectPos_Slot475);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot475 + 0xc)));
    }
}

/*
 * Gated on enabled being non-zero. Makes a single call to Mini_SnowboardSpawnSelectorOffsetEffectNode with (short)slot, the usual
 * 0x600, selector 3 and the record g_SnoboAltEffectPos_Slot477.
 */
// FUNCTION: FF7 0x0073fcf5
void __cdecl Mini_SnowboardAltSlot477SpawnEffectTag600(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 3, &g_SnoboAltEffectPos_Slot477);
    }
}

/*
 * Gated on enabled being non-zero. Makes a single call to Mini_SnowboardSpawnSelectorOffsetEffectNode with (short)slot, the usual
 * 0x600, selector 2 and the record g_SnoboAltEffectPos_Slot478.
 */
// FUNCTION: FF7 0x0073fd19
void __cdecl Mini_SnowboardAltSlot478SpawnEffectTag600(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 2, &g_SnoboAltEffectPos_Slot478);
    }
}

/*
 * Gated on enabled being non-zero. Calls Mini_SnowboardSpawnE00EffectNode with (short)slot and selector 0 for record
 * g_SnoboAltEffectPos_Slot479, then with selector 1 for record g_SnoboAltEffectPos_Slot479[1].
 */
// FUNCTION: FF7 0x0073fd3d
void __cdecl Mini_SnowboardAltSlot479SpawnTwoEffectNodes(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, g_SnoboAltEffectPos_Slot479);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, (SnowboardVec *)(((char *)&g_SnoboAltEffectPos_Slot479 + 0xc)));
    }
}
} /* extern "C" */

extern "C" {


#pragma intrinsic(abs)
struct SnowboardMatrixRecord;
#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00965df8
struct SnowboardVec g_SnoboAltSlot480EffectPos0[2] = {{-0xfacf000, 0x21ee0000, -0x35d5b000}, {-0x10077000, 0x21ee0000, -0x36301000}};
// GLOBAL: FF7 0x00965e10
struct SnowboardSelectorEffect g_SnoboAltSlot483EffectPos0[2] = {{-0xf741000, 0x222e6000, -0x36f0b000}, {-0xeb5a000, 0x22433000, -0x369ff000}};
// GLOBAL: FF7 0x00965e28
struct SnowboardSelectorEffect g_SnoboAltSlot484EffectPos0 = {-0xf015000, 0x2259c000, -0x37635000};
// GLOBAL: FF7 0x00965e38
struct SnowboardSelectorEffect g_SnoboAltSlot485EffectPos0 = {-0xe313000, 0x22756000, -0x3724f000};
// GLOBAL: FF7 0x00965e48
struct SnowboardVec g_SnoboAltSlot485EffectPos1[2] = {{-0xec01000, 0x226e2000, -0x378e0000}, {-0xe4f4000, 0x226e2000, -0x371d7000}};
// GLOBAL: FF7 0x00965e60
struct SnowboardSelectorEffect g_SnoboAltSlot486EffectPos0 = {-0xe73b000, 0x228b6000, -0x37dbe000};
// GLOBAL: FF7 0x00965e70
struct SnowboardSelectorEffect g_SnoboAltSlot487EffectPos0 = {-0xdc1a000, 0x22a45000, -0x37ad6000};
// GLOBAL: FF7 0x00965e80
struct SnowboardSelectorEffect g_SnoboAltSlot488EffectPos0 = {-0xe06b000, 0x22b8a000, -0x385e1000};
// GLOBAL: FF7 0x00965e90
struct SnowboardVec g_SnoboAltSlot489EffectPos0[2] = {{-0xd9e9000, 0x22c92000, -0x384d5000}, {-0xd901000, 0x22c92000, -0x383ed000}};
// GLOBAL: FF7 0x00965ea8
short g_SnoboAltSlot491EffectPos0[12] = {384, 512, 544, 544, 512, 384, 384, 512, 544, 544, 512, 384};
// GLOBAL: FF7 0x00965ec0
struct SnowboardVec g_SnoboAltSlot490EffectPos0[2] = {{-0xd237000, 0x22e85000, -0x3876a000}, {-0xd675000, 0x22e85000, -0x38ba6000}};
// GLOBAL: FF7 0x00965ed8
struct SnowboardSpawnPos g_SnoboAltSlot501EffectPos0 = {-0xb0b4000, 0x2436a000, -0x3bb5e000};
// GLOBAL: FF7 0x00965ee8
struct SnowboardSpriteRecord g_SnoboAltSlot519SpriteRecord0 = {-0xd45a000, 0x258a1000, -0x3f0ba000, 0x220000};
// GLOBAL: FF7 0x00965ef8
struct SnowboardSpriteRecord g_SnoboAltSlot519SpriteRecord1 = {-0xd326000, 0x2589e000, -0x3ef4b000, 0x240000};
// GLOBAL: FF7 0x00965f08
struct SnowboardSpriteRecord g_SnoboAltSlot519SpriteRecord2 = {-0xd1ab000, 0x25922000, -0x3f236000, 0x250000};
// GLOBAL: FF7 0x00965f18
struct SnowboardSpriteRecord g_SnoboAltSlot520SpriteRecord0 = {-0xcec5000, 0x259ac000, -0x3f3cf000, 0x220000};
// GLOBAL: FF7 0x00965f28
struct SnowboardSpriteRecord g_SnoboAltSlot520SpriteRecord1 = {-0xcffe000, 0x259aa000, -0x3f537000, 0x260000};
// GLOBAL: FF7 0x00965f38
struct SnowboardSpriteRecord g_SnoboAltSlot525SpriteRecord0 = {-0xd2ed000, 0x25ea8000, -0x404c8000, 0x240000};
// GLOBAL: FF7 0x00965f48
struct SnowboardSpriteRecord g_SnoboAltSlot525SpriteRecord1 = {-0xd61a000, 0x25f2a000, -0x404c2000, 0x260000};
// GLOBAL: FF7 0x00965f58
struct SnowboardSpriteRecord g_SnoboAltSlot525SpriteRecord2 = {-0xd196000, 0x25e54000, -0x402dc000, 0x230000};
// GLOBAL: FF7 0x00965f68
struct SnowboardSpriteRecord g_SnoboAltSlot526SpriteRecord0 = {-0xd93d000, 0x25fa4000, -0x404a0000, 0x280000};
// GLOBAL: FF7 0x00965f78
struct SnowboardSpriteRecord g_SnoboAltSlot526SpriteRecord1 = {-0xd9f5000, 0x25fdf000, -0x4069f000, 0x230000};
// GLOBAL: FF7 0x00965f88
struct SnowboardSpriteRecord g_SnoboAltSlot527SpriteRecord0 = {-0xde55000, 0x2608c000, -0x405fa000, 0x250000};
// GLOBAL: FF7 0x00965f98
struct SnowboardSpriteRecord g_SnoboAltSlot527SpriteRecord1 = {-0xdf70000, 0x260d0000, -0x40805000, 0x240000};
// GLOBAL: FF7 0x00965fa8
struct SnowboardSpriteRecord g_SnoboAltSlot528SpriteRecord0 = {-0xe390000, 0x26179000, -0x4085a000, 0x220000};
// GLOBAL: FF7 0x00965fb8
unsigned char g_SnoboAltSlot528SpriteRecord1[8] = {0x00, 0xc0, 0x9e, 0xf1, 0x00, 0xa0, 0x1f, 0x26};
// GLOBAL: FF7 0x00965fc0
unsigned char g_SnoboAltSlot528SpriteRecord2[8] = {0x00, 0x40, 0x55, 0xbf, 0x00, 0x00, 0x26, 0x00};
// GLOBAL: FF7 0x00965fc8
struct SnowboardSpriteRecord g_SnoboAltSlot528SpriteRecord3[2] = {{-0xE6D2000, 0x261FD000, -0x407C4000, 0x240000}, {-0xE6AF000, 0x261FE000, -0x40936000, 0x250000}};
// GLOBAL: FF7 0x00965fe8
struct SnowboardSpriteRecord g_SnoboAltSlot529SpriteRecord0 = {-0xea0d000, 0x262a9000, -0x40bdb000, 0x230000};
// GLOBAL: FF7 0x00965ff8
struct SnowboardSpriteRecord g_SnoboAltSlot529SpriteRecord1 = {-0xea25000, 0x2628d000, -0x4085b000, 0x220000};
// GLOBAL: FF7 0x00966008
struct SnowboardSpriteRecord g_SnoboAltSlot529SpriteRecord2 = {-0xec45000, 0x262f3000, -0x40a53000, 0x280000};
// GLOBAL: FF7 0x00966018
struct SnowboardSpriteRecord g_SnoboAltSlot530SpriteRecord0 = {-0xef0c000, 0x263b6000, -0x40c34000, 0x230000};
// GLOBAL: FF7 0x00966028
struct SnowboardSpriteRecord g_SnoboAltSlot530SpriteRecord1 = {-0xf0ee000, 0x2640a000, -0x40bcf000, 0x260000};
// GLOBAL: FF7 0x00966038
struct SnowboardSpriteRecord g_SnoboAltSlot531SpriteRecord0 = {-0xf214000, 0x26489000, -0x40da9000, 0x2c0000};
// GLOBAL: FF7 0x00966048
struct SnowboardSpriteRecord g_SnoboAltSlot531SpriteRecord1 = {-0xf1c7000, 0x26509000, -0x40ff9000, 0x270000};
// GLOBAL: FF7 0x00966058
struct SnowboardSpriteRecord g_SnoboAltSlot532SpriteRecord0 = {-0xf302000, 0x265b7000, -0x4124a000, 0x240000};
// GLOBAL: FF7 0x00966068
struct SnowboardSpriteRecord g_SnoboAltSlot532SpriteRecord1 = {-0xf394000, 0x26618000, -0x413ef000, 0x270000};
// GLOBAL: FF7 0x00966078
struct SnowboardSpriteRecord g_SnoboAltSlot533SpriteRecord0 = {-0xf284000, 0x26666000, -0x414ed000, 0x250000};
// GLOBAL: FF7 0x00966088
struct SnowboardSpriteRecord g_SnoboAltSlot533SpriteRecord1 = {-0xf1ef000, 0x266e5000, -0x416de000, 0x280000};
// GLOBAL: FF7 0x00966098
struct SnowboardSpriteRecord g_SnoboAltSlot534SpriteRecord0 = {-0xf019000, 0x26758000, -0x4178c000, 0x240000};
// GLOBAL: FF7 0x009660a8
struct SnowboardSpriteRecord g_SnoboAltSlot534SpriteRecord1 = {-0xee94000, 0x267e1000, -0x418de000, 0x260000};
// GLOBAL: FF7 0x009660b8
struct SnowboardSpriteRecord g_SnoboAltSlot535SpriteRecord0 = {-0xecc7000, 0x26850000, -0x41a25000, 0x270000};
// GLOBAL: FF7 0x009660c8
struct SnowboardSpriteRecord g_SnoboAltSlot535SpriteRecord1 = {-0xe7ca000, 0x2692a000, -0x41a78000, 0x210000};
// GLOBAL: FF7 0x009660d8
struct SnowboardSpriteRecord g_SnoboAltSlot535SpriteRecord2 = {-0xeb03000, 0x26895000, -0x41967000, 0x250000};
// GLOBAL: FF7 0x009660e8
struct SnowboardSpriteRecord g_SnoboAltSlot535SpriteRecord3 = {-0xe9ac000, 0x268d6000, -0x41acc000, 0x240000};
// GLOBAL: FF7 0x009660f8
struct SnowboardSpriteRecord g_SnoboAltSlot536SpriteRecord0 = {-0xe2d4000, 0x26a03000, -0x41c5b000, 0x250000};
// GLOBAL: FF7 0x00966108
struct SnowboardSpriteRecord g_SnoboAltSlot536SpriteRecord1 = {-0xe5cf000, 0x26987000, -0x41bd6000, 0x240000};
// GLOBAL: FF7 0x00966118
struct SnowboardVec g_SnoboAltSlot557EffectPos0[2] = {{-0x8cf0000, 0x24977000, -0x3afff000}, {-0x8ad6000, 0x24a37000, -0x3ada2000}};
// GLOBAL: FF7 0x00966130
unsigned char g_SnoboAltSlot557EffectPos1[16] = {0x00, 0x50, 0x62, 0xf7, 0x00, 0x60, 0xa3, 0x24, 0x00, 0x90, 0x0a, 0xc5, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00966140
struct SnowboardVec g_SnoboAltSlot558EffectPos0[2] = {{-0x88c7000, 0x24b2d000, -0x3ab43000}, {-0x86c5000, 0x24b2b000, -0x3aec1000}};
// GLOBAL: FF7 0x00966158
struct SnowboardVec g_SnoboAltSlot559EffectPos0[2] = {{-0x83c4000, 0x24ded000, -0x3a8f4000}, {-0x8245000, 0x24ded000, -0x3ab8d000}};
// GLOBAL: FF7 0x00966170
unsigned char g_SnoboAltSlot559EffectPos1[16] = {0x00, 0x80, 0x00, 0xf8, 0x00, 0x70, 0xfb, 0x24, 0x00, 0x10, 0x78, 0xc5, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00966180
struct SnowboardVec g_SnoboAltSlot560EffectPos0[2] = {{-0x7d64000, 0x251eb000, -0x3a410000}, {-0x7ae6000, 0x251ea000, -0x3a865000}};
// GLOBAL: FF7 0x00966198
struct SnowboardSpawnPos g_SnoboAltSlot561EffectPos0[2] = {{-0x77af000, 0x25481000, -0x3a1ec000}, {-0x7625000, 0x25483000, -0x3a488000}};
// clang-format on

// Alternate snowboard event callbacks indexed by descriptor id:
// 601 slots at 0x009661B0..0x00966B14, forming the final .data table
// of this translation unit.
// GLOBAL: FF7 0x009661b0
SnowboardCallback g_SnoboAltEffectCallbacks[601] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot008SpawnEffectNode,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot015SpawnEffectNode,
    (SnowboardCallback)Mini_SnowboardAltSlot016SpawnEffectNode,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot020SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot026SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot031SpawnTwoKindEAEffects,
    (SnowboardCallback)Mini_SnowboardAltSlot032SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot040SpawnThreeEffects,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot048SpawnThreeEffects,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot050SpawnKindEAEffect,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot056SpawnEffectAbove,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot058SpawnEffectAbove,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot061SpawnKindEDEffect,
    (SnowboardCallback)Mini_SnowboardAltSlot062NoOp,
    (SnowboardCallback)Mini_SnowboardAltSlot063SpawnKindEFEffect,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot066SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot073SpawnKindEBEffect,
    (SnowboardCallback)Mini_SnowboardAltSlot074SpawnKindEBEffect,
    (SnowboardCallback)Mini_SnowboardAltSlot075SpawnKindEBEffect,
    (SnowboardCallback)Mini_SnowboardAltSlot076SpawnKindEBEffect,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot079SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot085SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot091NoOp,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot115SpawnPlainEffect,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot132SpawnTwoEffectsAbove,
    (SnowboardCallback)Mini_SnowboardAltSlot133SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot138SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot146SpawnEffectAbove,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot148SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot156SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot161SpawnTwoEffects,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot163SpawnEffectAboveSetRiderAnim,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_1,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffectAbove,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant1,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEmitOffsetTypedEffect,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_2,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant1,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_1,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffectAbove,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant1,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEmitOffsetTypedEffect,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_2,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant1,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_1,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffectAbove,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant1,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEmitOffsetTypedEffect,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_2,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant1,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_1,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffectAbove,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant2,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEmitOffsetTypedEffect,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_2,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant2,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_1,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_3,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant2,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEmitOffsetTypedEffect,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_4,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant2,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_1,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_3,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant2,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEmitOffsetTypedEffect,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnOffsetEffect_4,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotSpawnTwoOffsetEffectsVariant2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot230NoOp,
    (SnowboardCallback)Mini_SnowboardAltSlot231NoOp,
    0,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltSlot236SpawnTemplateF0AndQuad,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltSlot239SpawnTemplateF0AndQuad,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltSlot243SpawnTemplateF0AndQuad,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltSlot248SpawnTemplateF0AndQuad,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltSlot252SpawnTemplateF0AndQuad,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltSlot256SpawnTemplateF0AndQuad,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotGatedSpawnFourEffects,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot265SpawnTwoEAHitAndF0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot269SpawnEAHit,
    (SnowboardCallback)Mini_SnowboardAltSlot270SpawnTwoEAHitAndF0Mode1,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot277SpawnEAPlain,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot301SpawnF0IfStateAbove60,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot316SpawnTwoEffectsTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot317SpawnEffectTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot318SpawnLinkedAndTag238Effect,
    (SnowboardCallback)Mini_SnowboardAltSlot319SpawnEffectTag238AndIndexedNode,
    (SnowboardCallback)Mini_SnowboardAltSlot320SpawnEffectTag238AndIndexedNode,
    (SnowboardCallback)Mini_SnowboardAltSlot321SpawnEffectTag238AndIndexedNode,
    (SnowboardCallback)Mini_SnowboardAltSlot322SpawnEffectTag238AndIndexedNode,
    (SnowboardCallback)Mini_SnowboardAltSlot323SpawnEffectTag238AndIndexedNode,
    (SnowboardCallback)Mini_SnowboardAltSlot324SpawnIndexedEffectNode,
    (SnowboardCallback)Mini_SnowboardAltSlot325SpawnEffectTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot326SpawnTwoEffectsTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot327SpawnTwoEffectsTag238,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot329SpawnTwoEffectsTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot330SpawnIndexedEffectNode,
    (SnowboardCallback)Mini_SnowboardAltSlot331SpawnIndexedNodeAndTwoEffectsTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot332SpawnTwoIndexedEffectNodes,
    (SnowboardCallback)Mini_SnowboardAltSlot333SpawnEffectTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot334SpawnIndexedNodeAndEffectTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot335SpawnEffectTag238AndIndexedNode,
    (SnowboardCallback)Mini_SnowboardAltSlot336SpawnTwoIndexedEffectNodes,
    (SnowboardCallback)Mini_SnowboardAltSlot337SpawnEffectTag238,
    (SnowboardCallback)Mini_SnowboardAltSlot338SpawnTwoEffectsTag238,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot340SpawnTwoEffectsTag238,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot342SpawnEffectTag238AndTwoIndexedNodes,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot344SpawnTwoEffectsTag238,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot346SpawnTwoEffectsTag238,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot358SpawnTaggedEffectNode,
    (SnowboardCallback)Mini_SnowboardAltSlot359SpawnEffectTag1238,
    (SnowboardCallback)Mini_SnowboardAltSlot360SpawnEffectTagA38,
    (SnowboardCallback)Mini_SnowboardAltSlot361SpawnEffectTag1238,
    (SnowboardCallback)Mini_SnowboardAltSlot362SpawnEffectTagA38,
    (SnowboardCallback)Mini_SnowboardAltSlot363SpawnEffectsTagA38AndTag1238,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot371SpawnEffectTag98E,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot373SpawnEffectTag98E,
    (SnowboardCallback)Mini_SnowboardAltSlot374SpawnEffectTagD8E,
    (SnowboardCallback)Mini_SnowboardAltSlot375SpawnEffectTag98E,
    (SnowboardCallback)Mini_SnowboardAltSlot376SpawnEffectTag58E,
    (SnowboardCallback)Mini_SnowboardAltSlot377SpawnEffectTag98E,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot379SpawnEffectTag98E,
    (SnowboardCallback)Mini_SnowboardAltSlot380SpawnTwoEffectsTagD8E,
    (SnowboardCallback)Mini_SnowboardAltSlot381SpawnEffectTag98E,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot383SpawnTwoEffectsTag98E,
    (SnowboardCallback)Mini_SnowboardAltSlot384SpawnEffectsTag58EAndTagD8E,
    (SnowboardCallback)Mini_SnowboardAltSlot385SpawnEffectTag98E,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot387SpawnEffectTag98E,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot389SpawnEffectTag98E,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot391SpawnEffectTag98E,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot393SpawnTwoEffectsTag98E,
    (SnowboardCallback)Mini_SnowboardAltSlot394SpawnTwoVelocityEffects,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot401SpawnEffectTemplateF0Gated,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot415SpawnEffectAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot416SpawnTwoEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot417SpawnThreeEffectsAndCheckState0B,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot424SpawnTwoEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot425SpawnEffectAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot426SpawnTwoEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot427SpawnThreeEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot428SpawnTwoEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot429SpawnThreeEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot430SpawnTwoEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot431SpawnThreeEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot432SpawnFourEffectsAndCheckState0B,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot439SpawnEffectAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot440SpawnTwoEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot441SpawnFiveEffectsAndCheckState0B,
    (SnowboardCallback)Mini_SnowboardAltSlot442SpawnTwoEffectsAndCheckState0B,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot465SpawnTwoEffectsTagE00,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot467SpawnTwoEffectsTagE00,
    (SnowboardCallback)Mini_SnowboardAltSlot468SpawnTwoEffectsTag600,
    (SnowboardCallback)Mini_SnowboardAltSlot469SpawnTwoEffectsTagE00,
    (SnowboardCallback)Mini_SnowboardAltSlot470SpawnTwoEffectsTag600,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot473SpawnTwoEffectsTagE00,
    (SnowboardCallback)Mini_SnowboardAltSlot474SpawnTwoEffectsTagE00,
    (SnowboardCallback)Mini_SnowboardAltSlot475SpawnTwoEffectsTagE00,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot477SpawnEffectTag600,
    (SnowboardCallback)Mini_SnowboardAltSlot478SpawnEffectTag600,
    (SnowboardCallback)Mini_SnowboardAltSlot479SpawnTwoEffectNodes,
    (SnowboardCallback)Mini_SnowboardAltSlot480SpawnTwoEffectNodes,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot483SpawnTwoOffsetEffectNodes,
    (SnowboardCallback)Mini_SnowboardAltSlot484SpawnOffsetEffectNode,
    (SnowboardCallback)Mini_SnowboardAltSlot485SpawnOffsetEffectAndTwoEffectsTagE00,
    (SnowboardCallback)Mini_SnowboardAltSlot486SpawnOffsetEffectNode,
    (SnowboardCallback)Mini_SnowboardAltSlot487SpawnOffsetEffectNode,
    (SnowboardCallback)Mini_SnowboardAltSlot488SpawnOffsetEffectNode,
    (SnowboardCallback)Mini_SnowboardAltSlot489SpawnTwoEffectNodes,
    (SnowboardCallback)Mini_SnowboardAltSlot490SpawnTwoEffectNodes,
    (SnowboardCallback)Mini_SnowboardAltSlot491EnterRiderState0BIfOffsetExceeded,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEnterRiderState0B,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEnterRiderState0B,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEnterRiderState0B,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEnterRiderState0B,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEnterRiderState0B,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEnterRiderState0B,
    (SnowboardCallback)Mini_SnowboardAltMultiSlotEnterRiderState0B,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot501SpawnEffectAboveIfResultAbove80,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot518PlaySfx00CD,
    (SnowboardCallback)Mini_SnowboardAltSlot519EmitThreeSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot520EmitTwoSpritePrims,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot525EmitThreeSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot526EmitTwoSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot527EmitTwoSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot528EmitFourSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot529EmitThreeSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot530EmitTwoSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot531EmitTwoSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot532EmitTwoSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot533EmitTwoSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot534EmitTwoSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot535EmitFourSpritePrims,
    (SnowboardCallback)Mini_SnowboardAltSlot536EmitTwoSpritePrims,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardAltSlot557SpawnThreeKindEAEffectsOnHit,
    (SnowboardCallback)Mini_SnowboardAltSlot558SpawnTwoKindEAEffectsOnHit,
    (SnowboardCallback)Mini_SnowboardAltSlot559SpawnThreeKindEAEffectsOnHit,
    (SnowboardCallback)Mini_SnowboardAltSlot560SpawnTwoKindEAEffectsOnHit,
    (SnowboardCallback)Mini_SnowboardAltSlot561SpawnTwoKindEAEffectsPlain,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

/*
 * Gated on enabled being non-zero. Calls Mini_SnowboardSpawnE00EffectNode with (short)slot and selector 0 for record
 * g_SnoboAltSlot480EffectPos0, then with selector 1 for record g_SnoboAltSlot480EffectPos0[1]. Identical in shape to Mini_SnowboardAltSlot479SpawnTwoEffectNodes,
 * differing only in the two record addresses.
 */
// FUNCTION: FF7 0x0073fd70
void __cdecl Mini_SnowboardAltSlot480SpawnTwoEffectNodes(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, g_SnoboAltSlot480EffectPos0);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, &g_SnoboAltSlot480EffectPos0[1]);
    }
}

/*
 * Gated on enabled being non-zero. Calls Mini_SnowboardSpawnSelectorOffsetEffectNode with (short)slot, 0x600, selector 1 and record
 * g_SnoboAltSlot483EffectPos0, then with selector 0 and record g_SnoboAltSlot483EffectPos0[1]; the selector order is the reverse of
 * Mini_SnowboardAltSlot470SpawnTwoEffectsTag600's.
 */
// FUNCTION: FF7 0x0073fda3
void __cdecl Mini_SnowboardAltSlot483SpawnTwoOffsetEffectNodes(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 1, g_SnoboAltSlot483EffectPos0);
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 0, &g_SnoboAltSlot483EffectPos0[1]);
    }
}

/*
 * Gated on enabled being non-zero. Makes a single call to Mini_SnowboardSpawnSelectorOffsetEffectNode with (short)slot, 0x600,
 * selector 3 and the record g_SnoboAltSlot484EffectPos0.
 */
// FUNCTION: FF7 0x0073fde0
void __cdecl Mini_SnowboardAltSlot484SpawnOffsetEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 3, &g_SnoboAltSlot484EffectPos0);
    }
}

/*
 * Gated on enabled being non-zero. Unlike its neighbours it drives both helpers: first Mini_SnowboardSpawnSelectorOffsetEffectNode
 * with (short)slot, 0x600, selector 2 and record g_SnoboAltSlot485EffectPos0, then Mini_SnowboardSpawnE00EffectNode with selector 1
 * for g_SnoboAltSlot485EffectPos1 and selector 0 for g_SnoboAltSlot485EffectPos1[1].
 */
// FUNCTION: FF7 0x0073fe04
void __cdecl Mini_SnowboardAltSlot485SpawnOffsetEffectAndTwoEffectsTagE00(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 2, &g_SnoboAltSlot485EffectPos0);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, g_SnoboAltSlot485EffectPos1);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, &g_SnoboAltSlot485EffectPos1[1]);
    }
}

/*
 * Gated on enabled being non-zero. Makes a single call to Mini_SnowboardSpawnSelectorOffsetEffectNode with (short)slot, 0x600,
 * selector 3 and the record g_SnoboAltSlot486EffectPos0.
 */
// FUNCTION: FF7 0x0073fe50
void __cdecl Mini_SnowboardAltSlot486SpawnOffsetEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 3, &g_SnoboAltSlot486EffectPos0);
    }
}

/*
 * Gated on enabled being non-zero. Makes a single call to Mini_SnowboardSpawnSelectorOffsetEffectNode with (short)slot, 0x600,
 * selector 2 and the record g_SnoboAltSlot487EffectPos0.
 */
// FUNCTION: FF7 0x0073fe74
void __cdecl Mini_SnowboardAltSlot487SpawnOffsetEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 2, &g_SnoboAltSlot487EffectPos0);
    }
}

/*
 * Gated on enabled being non-zero. Makes a single call to Mini_SnowboardSpawnSelectorOffsetEffectNode with (short)slot, 0x600,
 * selector 1 and the record g_SnoboAltSlot488EffectPos0.
 */
// FUNCTION: FF7 0x0073fe98
void __cdecl Mini_SnowboardAltSlot488SpawnOffsetEffectNode(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnSelectorOffsetEffectNode((short)slot, 0x600, 1, &g_SnoboAltSlot488EffectPos0);
    }
}

/*
 * Gated on enabled being non-zero. Calls Mini_SnowboardSpawnE00EffectNode with (short)slot and selector 1 for record
 * g_SnoboAltSlot489EffectPos0, then with selector 0 for record g_SnoboAltSlot489EffectPos0[1].
 */
// FUNCTION: FF7 0x0073febc
void __cdecl Mini_SnowboardAltSlot489SpawnTwoEffectNodes(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, g_SnoboAltSlot489EffectPos0);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, &g_SnoboAltSlot489EffectPos0[1]);
    }
}

// Calls Mini_SnowboardEnterAltRiderState0BPlaySfx when entityId matches the rider's section, the
// phase ((triangle - 0x3C) >> 1) is nonnegative and riderState is not 0xB, and the
// integer-converted difference between terrainHeight and relPosY, shifted right twelve bits,
// exceeds the g_SnoboAltSlot491EffectPos0 word at that phase.
// FUNCTION: FF7 0x0073feef
void __cdecl Mini_SnowboardAltSlot491EnterRiderState0BIfOffsetExceeded(int entityId)
{
    short phase;

    if (entityId != g_SnoboStatePtr->section) {
        return;
    }
    phase = g_SnoboStatePtr->triangle - 0x3C >> 1;
    if (phase < 0) {
        return;
    }
    if (*(char *)&g_SnoboStatePtr->riderState == 0xB) {
        return;
    }
    if ((int)(g_SnoboStatePtr->terrainHeight - g_SnoboStatePtr->relPosY) >> 12 > g_SnoboAltSlot491EffectPos0[phase]) {
        Mini_SnowboardEnterAltRiderState0BPlaySfx();
    }
}

// Calls Mini_SnowboardEnterAltRiderState0BPlaySfx when entityId matches the rider's section, the
// phase ((triangle - 0x30) >> 1) is nonnegative and riderState is not 0xB, and the
// integer-converted difference between terrainHeight and relPosY, shifted right twelve bits,
// exceeds the g_SnoboAltSlot491EffectPos0 word at that phase.
// FUNCTION: FF7 0x0073ff73
void __cdecl Mini_SnowboardAltMultiSlotEnterRiderState0B(int entityId)
{
    short phase;

    if (entityId != g_SnoboStatePtr->section) {
        return;
    }
    phase = g_SnoboStatePtr->triangle - 0x30 >> 1;
    if (phase < 0) {
        return;
    }
    if (*(char *)&g_SnoboStatePtr->riderState == 0xB) {
        return;
    }
    if ((int)(g_SnoboStatePtr->terrainHeight - g_SnoboStatePtr->relPosY) >> 12 > g_SnoboAltSlot491EffectPos0[phase]) {
        Mini_SnowboardEnterAltRiderState0BPlaySfx();
    }
}

/*
 * Gated on enabled being non-zero. Calls Mini_SnowboardSpawnE00EffectNode with (short)slot and selector 0 for record
 * g_SnoboAltSlot490EffectPos0, then with selector 1 for record g_SnoboAltSlot490EffectPos0[1].
 */
// FUNCTION: FF7 0x0073fff7
void __cdecl Mini_SnowboardAltSlot490SpawnTwoEffectNodes(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardSpawnE00EffectNode((short)slot, 0, g_SnoboAltSlot490EffectPos0);
        Mini_SnowboardSpawnE00EffectNode((short)slot, 1, &g_SnoboAltSlot490EffectPos0[1]);
    }
}

/*
 * Carries a second gate on top of the usual enabled test: the run's resultTotal (+0x78) in the structure
 * g_SnoboStatePtr points at must be greater than 0x50. Only then does it call Mini_SnowboardSpawnEffectNodeTemplateF0 with
 * (short)slot, 0x785, selector 3 and the record g_SnoboAltSlot501EffectPos0. Two other functions in this file
 * test the same 0x78 field, one of them against 0x3C.
 */
// FUNCTION: FF7 0x0074002a
void __cdecl Mini_SnowboardAltSlot501SpawnEffectAboveIfResultAbove80(int slot, int enabled)
{
    if (enabled != 0 && g_SnoboStatePtr->resultTotal > 80) {
        Mini_SnowboardSpawnEffectNodeTemplateF0((short)slot, 0x785, 3, &g_SnoboAltSlot501EffectPos0);
    }
}

/*
 * Ignores unused entirely. When enabled is non-zero it calls Mini_SnowboardPlaySfxOnChannel(4, 0xCD,
 * 0), playing sound 0xCD on channel 4, and discards the return value.
 */
// FUNCTION: FF7 0x0074005c
void __cdecl Mini_SnowboardAltSlot518PlaySfx00CD(int unused, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardPlaySfxOnChannel(4, 0xCD, 0);
    }
}

/*
 * When enabled is nonzero, narrows slot to short and calls Mini_SnowboardEmitSpritePrim with
 * parameter pairs (0,0), (0xa00,2), and (0x400,3), using g_SnoboAltSlot519SpriteRecord0, g_SnoboAltSlot519SpriteRecord1, and
 * g_SnoboAltSlot519SpriteRecord2 respectively.
 */
// FUNCTION: FF7 0x00740078
void __cdecl Mini_SnowboardAltSlot519EmitThreeSpritePrims(int slot, int enabled)
{
    if (enabled != 0) {
        Mini_SnowboardEmitSpritePrim((short)slot, 0, 0, &g_SnoboAltSlot519SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)slot, 0xA00, 2, &g_SnoboAltSlot519SpriteRecord1);
        Mini_SnowboardEmitSpritePrim((short)slot, 0x400, 3, &g_SnoboAltSlot519SpriteRecord2);
    }
}

/*
 * Configures a shared sprite-effect template from the supplied position, angle and low flag bits.
 * Bit two clears callback/render fields, otherwise installs its callback; bit one selects one of
 * two mesh IDs. Stores the height limits and flags and allocates a secondary effect, ignoring the
 * allocation return.
 */
// FUNCTION: FF7 0x007400cb
void __cdecl Mini_SnowboardEmitSpritePrim(short objectId, int param, int flags, SnowboardSpriteRecord *record)
{
    int node;

    if (((short)flags & 2) != 0) {
        g_SnoboSpritePrimEffectDef.update = 0;
        g_SnoboSpritePrimEffectDef.contactRadius = 0;
    } else {
        g_SnoboSpritePrimEffectDef.update = (SnowboardEffectUpdate)Mini_SnowboardEffectStartDropFadeInWhenNear;
    }
    g_SnoboSpritePrimEffectDef.model = ((short)flags & 1) + 0x10C;
    g_SnoboSpritePrimEffectDef.rotY = param;
    g_SnoboSpritePrimEffectDef.sidePos = record->sidePos;
    g_SnoboSpritePrimEffectDef.height = record->anchorPos - record->anchorOffset;
    g_SnoboSpritePrimEffectDef.heightLimit = record->anchorPos + 0x20000;
    g_SnoboSpritePrimEffectDef.forward = record->forward;
    g_SnoboSpritePrimEffectDef.variant = (short)flags;
    node = Mini_SnowboardAllocEffectNodeFromTemplate((short)objectId, &g_SnoboSpritePrimEffectDef);
}

/*
 * Gated on enable being non-zero. Calls Mini_SnowboardEmitSpritePrim twice with (short)objectId: 0x800
 * with selector 0 and record g_SnoboAltSlot520SpriteRecord0, then 0x300 with selector 2 and g_SnoboAltSlot520SpriteRecord1.
 */
// FUNCTION: FF7 0x00740173
void __cdecl Mini_SnowboardAltSlot520EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x800, 0, &g_SnoboAltSlot520SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x300, 2, &g_SnoboAltSlot520SpriteRecord1);
    }
}

/*
 * Gated on enable being non-zero. Calls Mini_SnowboardEmitSpritePrim three times with (short)objectId:
 * 0xC00 selector 2 and record g_SnoboAltSlot525SpriteRecord0, 0 selector 1 and g_SnoboAltSlot525SpriteRecord1, 0x700 selector 2 and
 * g_SnoboAltSlot525SpriteRecord2.
 */
// FUNCTION: FF7 0x007401b0
void __cdecl Mini_SnowboardAltSlot525EmitThreeSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0xC00, 2, &g_SnoboAltSlot525SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 1, &g_SnoboAltSlot525SpriteRecord1);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x700, 2, &g_SnoboAltSlot525SpriteRecord2);
    }
}

/*
 * Gated on enable being non-zero. Calls Mini_SnowboardEmitSpritePrim twice with (short)objectId and
 * selector 2 both times: 0x600 with record g_SnoboAltSlot526SpriteRecord0 and 0xC00 with record g_SnoboAltSlot526SpriteRecord1.
 */
// FUNCTION: FF7 0x00740203
void __cdecl Mini_SnowboardAltSlot526EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x600, 2, &g_SnoboAltSlot526SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0xC00, 2, &g_SnoboAltSlot526SpriteRecord1);
    }
}

/*
 * Gated on enable being non-zero. Byte-for-byte the same shape as Mini_SnowboardAltSlot526EmitTwoSpritePrims --
 * Mini_SnowboardEmitSpritePrim at 0x600 and 0xC00, selector 2 both times -- only the records
 * differ, here g_SnoboAltSlot527SpriteRecord0 and g_SnoboAltSlot527SpriteRecord1.
 * For nonzero enable, emits two snowboard sprite primitives for narrowed objectId with constants 0x600
 * and 0xC00, mode 2 and templates g_SnoboAltSlot527SpriteRecord0/g_SnoboAltSlot527SpriteRecord1.
 */
// FUNCTION: FF7 0x00740240
void __cdecl Mini_SnowboardAltSlot527EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x600, 2, &g_SnoboAltSlot527SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0xC00, 2, &g_SnoboAltSlot527SpriteRecord1);
    }
}

/*
 * Gated on enable being non-zero. Emits four sprite primitives through
 * Mini_SnowboardEmitSpritePrim((short)objectId, ...), all with selector 0: 0xE00 with record
 * g_SnoboAltSlot528SpriteRecord0, 0x700 with g_SnoboAltSlot528SpriteRecord1, 0x100 with g_SnoboAltSlot528SpriteRecord2 and 0x300 with g_SnoboAltSlot528SpriteRecord3. The
 * first two records are 16 bytes apart, the last three only 8, so this run of data is not
 * uniformly strided.
 */
// FUNCTION: FF7 0x0074027d
void __cdecl Mini_SnowboardAltSlot528EmitFourSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0xE00, 0, &g_SnoboAltSlot528SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x700, 0, (SnowboardSpriteRecord *)&g_SnoboAltSlot528SpriteRecord1);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x100, 0, (SnowboardSpriteRecord *)&g_SnoboAltSlot528SpriteRecord2);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x300, 0, &g_SnoboAltSlot528SpriteRecord3[0]);
    }
}

/*
 * Gated on enable being non-zero. Calls Mini_SnowboardEmitSpritePrim three times with (short)objectId: 0
 * with selector 2 and record g_SnoboAltSlot529SpriteRecord0, 0x800 with selector 2 and g_SnoboAltSlot529SpriteRecord1, 0x400 with
 * selector 3 and g_SnoboAltSlot529SpriteRecord2.
 */
// FUNCTION: FF7 0x007402ec
void __cdecl Mini_SnowboardAltSlot529EmitThreeSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 2, &g_SnoboAltSlot529SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x800, 2, &g_SnoboAltSlot529SpriteRecord1);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x400, 3, &g_SnoboAltSlot529SpriteRecord2);
    }
}

/*
 * Does nothing unless enable is non-zero; then emits two sprite primitives for object objectId (narrowed
 * to short) through Mini_SnowboardEmitSpritePrim, one with parameters (0, 0) from the 16-byte
 * record at g_SnoboAltSlot530SpriteRecord0 and one with (0x100, 2) from g_SnoboAltSlot530SpriteRecord1. One of a long run of near-
 * identical per-object emit routines in this file, each differing only in its constants and record
 * addresses.
 */
// FUNCTION: FF7 0x0074033f
void __cdecl Mini_SnowboardAltSlot530EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 0, &g_SnoboAltSlot530SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x100, 2, &g_SnoboAltSlot530SpriteRecord1);
    }
}

/*
 * Same shape as Mini_SnowboardAltSlot530EmitTwoSpritePrims: when enable is non-zero it calls Mini_SnowboardEmitSpritePrim twice
 * for object objectId, with (0, 0) on the 16-byte record at g_SnoboAltSlot531SpriteRecord0 and (0x500, 2) on
 * g_SnoboAltSlot531SpriteRecord1. Nothing else; enable is purely an enable flag.
 */
// FUNCTION: FF7 0x00740379
void __cdecl Mini_SnowboardAltSlot531EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 0, &g_SnoboAltSlot531SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x500, 2, &g_SnoboAltSlot531SpriteRecord1);
    }
}

/*
 * When enable is non-zero, emits two sprite primitives for object objectId via
 * Mini_SnowboardEmitSpritePrim: (0, 0) from the record at g_SnoboAltSlot532SpriteRecord0 and (0, 3) from
 * g_SnoboAltSlot532SpriteRecord1, the records being 16 bytes apart. Mechanical, like its neighbours.
 */
// FUNCTION: FF7 0x007403b3
void __cdecl Mini_SnowboardAltSlot532EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 0, &g_SnoboAltSlot532SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 3, &g_SnoboAltSlot532SpriteRecord1);
    }
}

/*
 * When enable is non-zero, emits two sprite primitives for object objectId via
 * Mini_SnowboardEmitSpritePrim: (0, 2) from the 16-byte record at g_SnoboAltSlot533SpriteRecord0 and (0xC00, 0) from
 * g_SnoboAltSlot533SpriteRecord1.
 */
// FUNCTION: FF7 0x007403ea
void __cdecl Mini_SnowboardAltSlot533EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 2, &g_SnoboAltSlot533SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0xC00, 0, &g_SnoboAltSlot533SpriteRecord1);
    }
}

/*
 * When enable is non-zero, emits two sprite primitives for object objectId via
 * Mini_SnowboardEmitSpritePrim: (0x300, 2) from the 16-byte record at g_SnoboAltSlot534SpriteRecord0 and (0xF00, 3)
 * from g_SnoboAltSlot534SpriteRecord1. The second argument is one of the recurring multiples of 0x100 used across
 * this whole family of emitters.
 */
// FUNCTION: FF7 0x00740424
void __cdecl Mini_SnowboardAltSlot534EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x300, 2, &g_SnoboAltSlot534SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0xF00, 3, &g_SnoboAltSlot534SpriteRecord1);
    }
}

/*
 * The longest of the family: when enable is non-zero it calls Mini_SnowboardEmitSpritePrim four
 * times for object objectId, with (0, 0), (0x400, 0), (0x800, 2) and (0xC00, 2) on the four
 * consecutive 16-byte records at g_SnoboAltSlot535SpriteRecord0, g_SnoboAltSlot535SpriteRecord1, g_SnoboAltSlot535SpriteRecord2 and g_SnoboAltSlot535SpriteRecord3.
 */
// FUNCTION: FF7 0x00740461
void __cdecl Mini_SnowboardAltSlot535EmitFourSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 0, &g_SnoboAltSlot535SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x400, 0, &g_SnoboAltSlot535SpriteRecord1);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x800, 2, &g_SnoboAltSlot535SpriteRecord2);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0xC00, 2, &g_SnoboAltSlot535SpriteRecord3);
    }
}

/*
 * When enable is non-zero, emits two sprite primitives for object objectId via
 * Mini_SnowboardEmitSpritePrim: (0, 1) from the 16-byte record at g_SnoboAltSlot536SpriteRecord0 and (0x200, 2) from
 * g_SnoboAltSlot536SpriteRecord1.
 */
// FUNCTION: FF7 0x007404cd
void __cdecl Mini_SnowboardAltSlot536EmitTwoSpritePrims(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardEmitSpritePrim((short)objectId, 0, 1, &g_SnoboAltSlot536SpriteRecord0);
        Mini_SnowboardEmitSpritePrim((short)objectId, 0x200, 2, &g_SnoboAltSlot536SpriteRecord1);
    }
}

/*
 * When enable is non-zero, makes three identical-parameter calls to Mini_SnowboardSpawnKindEAEffectOnHit for object objectId,
 * each with (0x12AB, 2), on the three consecutive 12-byte records at g_SnoboAltSlot557EffectPos0, g_SnoboAltSlot557EffectPos0[1]
 * and g_SnoboAltSlot557EffectPos1. From here on the family switches emitter from Mini_SnowboardEmitSpritePrim to
 * Mini_SnowboardSpawnKindEAEffectOnHit and the record stride drops from 16 to 12 bytes.
 */
// FUNCTION: FF7 0x00740507
void __cdecl Mini_SnowboardAltSlot557SpawnThreeKindEAEffectsOnHit(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, g_SnoboAltSlot557EffectPos0);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, &g_SnoboAltSlot557EffectPos0[1]);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, (SnowboardVec *)&g_SnoboAltSlot557EffectPos1);
    }
}

/*
 * When enable is non-zero, calls Mini_SnowboardSpawnKindEAEffectOnHit twice for object objectId with (0x12AB, 2) on the 12-byte
 * records at g_SnoboAltSlot558EffectPos0 and g_SnoboAltSlot558EffectPos0[1]. Mechanical; enable is only an enable flag.
 */
// FUNCTION: FF7 0x0074055d
void __cdecl Mini_SnowboardAltSlot558SpawnTwoKindEAEffectsOnHit(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, g_SnoboAltSlot558EffectPos0);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, &g_SnoboAltSlot558EffectPos0[1]);
    }
}

/*
 * When enable is non-zero, calls Mini_SnowboardSpawnKindEAEffectOnHit three times for object objectId with (0x12AB, 2) on the
 * consecutive 12-byte records at g_SnoboAltSlot559EffectPos0, g_SnoboAltSlot559EffectPos0[1] and g_SnoboAltSlot559EffectPos1.
 */
// FUNCTION: FF7 0x0074059a
void __cdecl Mini_SnowboardAltSlot559SpawnThreeKindEAEffectsOnHit(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, g_SnoboAltSlot559EffectPos0);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, &g_SnoboAltSlot559EffectPos0[1]);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, (SnowboardVec *)&g_SnoboAltSlot559EffectPos1);
    }
}

/*
 * When enable is non-zero, calls Mini_SnowboardSpawnKindEAEffectOnHit twice for object objectId with (0x12AB, 2) on the 12-byte
 * records at g_SnoboAltSlot560EffectPos0 and g_SnoboAltSlot560EffectPos0[1].
 */
// FUNCTION: FF7 0x007405f0
void __cdecl Mini_SnowboardAltSlot560SpawnTwoKindEAEffectsOnHit(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, g_SnoboAltSlot560EffectPos0);
        Mini_SnowboardSpawnKindEAEffectOnHit((short)objectId, 0x12AB, 2, &g_SnoboAltSlot560EffectPos0[1]);
    }
}

/*
 * When enable is non-zero, calls Mini_SnowboardSpawnKindEAEffectPlain (not Mini_SnowboardSpawnKindEAEffectOnHit, unlike the four routines just
 * above) twice for object objectId with (0x2AB, 2) on the 12-byte records at g_SnoboAltSlot561EffectPos0 and
 * g_SnoboAltSlot561EffectPos0 + 0xC.
 */
// FUNCTION: FF7 0x0074062d
void __cdecl Mini_SnowboardAltSlot561SpawnTwoKindEAEffectsPlain(int objectId, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnKindEAEffectPlain((short)objectId, 0x2AB, 2, g_SnoboAltSlot561EffectPos0);
        Mini_SnowboardSpawnKindEAEffectPlain((short)objectId, 0x2AB, 2, &g_SnoboAltSlot561EffectPos0[1]);
    }
}

/*
 * Clears 151 consecutive dwords beginning at g_SnoboAltEventVisitFlags and clears g_SnoboLastSignalValue.
 */
// FUNCTION: FF7 0x0074066a
void __cdecl Mini_SnowboardClearAltEventVisitFlags(void)
{
    int cursor, remaining;
    remaining = 151;
    cursor = (int)&g_SnoboAltEventVisitFlags;
    while (remaining-- != 0) {
        *(int *)cursor = 0;
        cursor += 4;
    }
    g_SnoboLastSignalValue = 0;
}

/*
 * Marks byte slot in g_SnoboAltEventVisitFlags on its first visit and invokes the optional
 * g_SnoboAltEffectCallbacks[slot] callback with slot and a flag indicating that first visit.
 */
// FUNCTION: FF7 0x007406af
void __cdecl Mini_SnowboardVisitAltEventSlot(short slot)
{
    int firstVisit;
    firstVisit = 0;
    if (g_SnoboAltEventVisitFlags[slot] == 0) {
        g_SnoboAltEventVisitFlags[slot] = 1;
        firstVisit = 1;
    }
    if (g_SnoboAltEffectCallbacks[slot] != 0)
        g_SnoboAltEffectCallbacks[slot](slot, firstVisit);
}

/*
 * Clears the first byte of all 32 0x90-byte records at g_SnoboAltEffectRecords and clears g_SnoboEffectRecordCursor.
 */
// FUNCTION: FF7 0x00740704
void __cdecl Mini_SnowboardClearAltEffectRecords(void)
{
    int recordPtr, remaining;
    remaining = 32;
    for (recordPtr = (int)&g_SnoboAltEffectRecords; remaining-- != 0; recordPtr += 0x90)
        *(char *)recordPtr = 0;
    g_SnoboEffectRecordCursor = 0;
}

/*
 * Updates 32 active 0x90-byte records against the current minigame state. Expires records older
 * than the current short state, conditionally invokes their callbacks and skips disabled or
 * alternating-frame records. Builds or reuses each cached rotation matrix, applies relative
 * translation, converts it to float and optionally appends one or two render-list records without
 * a capacity check. For same-state records with nonzero radius, applies vertical and radial
 * proximity tests, marks contact and selects response bytes according to vertical position and the
 * current +0x15C value. Special response 0x80 maps to 8 or 12; response 8 can become 9 by frame
 * modulo 12 and halves two motion floats. Zero response clears both motion values. Processing
 * continues through all records, so later contacts can overwrite the response.
 */
// FUNCTION: FF7 0x0074074a
void __cdecl Mini_SnowboardUpdateAltEffectRecords(SnowboardCourseNodeState *state)
{
    SnowboardEffect *item;
    int rangeLimit;
    int *position;
    int ofs, offsetAmt, itemH, i, elem;
    SnowboardMatrixRecord *mtxRecord, *secondRecord;
    state->nodeIndex = g_SnoboStatePtr->section;
    state->raceRunning = *(char *)&g_SnoboStatePtr->running;
    position = state->colors;
    position[0] = (int)g_SnoboStatePtr->relPosX >> 12;
    position[1] = (int)g_SnoboStatePtr->relPosY >> 12;
    position[2] = (int)g_SnoboStatePtr->riderPos >> 12;
    for (i = 0, item = g_SnoboAltEffectRecords; i < 32; i++, item++) {
        if ((unsigned char)item->active == 0)
            continue;
        if (item->section < state->nodeIndex) {
            item->active = 0;
            g_SnoboEffectRecordCursor = (short)i;
            continue;
        }
        if (state->raceRunning != 0 && item->update != 0) {
            item->update(item);
            if ((unsigned char)item->active == 0)
                continue;
        }
        if (item->fade == 0xffff)
            continue;
        if (g_SnoboStatePtr->renderFrame != 0 && item->fade != 0) {
            item->blinkCounter
            ++;
            if ((item->blinkCounter & 1) != 0)
                continue;
        }
        if (item->matrixValid == 0) {
            RotMatrix((SVECTOR *)&item->rotX, state->rotation);
            itemH = item->matrixShift;
            if (itemH != 0) {
                elem = (int)state->rotation;
                offsetAmt = 9;
                while (offsetAmt-- != 0) {
                    *(short *)elem <<= itemH;
                    elem += 2;
                }
            }
            item->matrix = *(Copy30 *)state->rotation;
            item->matrixValid = 1;
        } else {
            *(Copy30 *)state->rotation = item->matrix;
        }
        state->translateX = (item->sidePos >> 12) - state->baseX;
        state->translateY = (item->height >> 12) - state->baseY;
        state->translateZ = (item->forward >> 12) - state->baseZ;
        GtePsxMatrixToFloat4x4((PsxMatrix *)state->rotation, state->worldMatrix);
        if (g_SnoboStatePtr->renderFrame != 0) {
            mtxRecord = (SnowboardMatrixRecord *)(g_SnoboMatrixRecordCount * 0x4c + (char *)&g_SnoboMatrixRecords);
            g_SnoboMatrixRecordCount++;
            mtxRecord->matrix = *(Copy64 *)state->worldMatrix;
            mtxRecord->kind = 0;
            mtxRecord->actionCode = item->model;
            mtxRecord->animFrame = item->renderArg0;
            mtxRecord->drawFlags = item->renderArg1;
            if (item->model2 != 0) {
                secondRecord = (SnowboardMatrixRecord *)(g_SnoboMatrixRecordCount * 0x4c + (char *)&g_SnoboMatrixRecords);
                g_SnoboMatrixRecordCount++;
                secondRecord->kind = 2;
                secondRecord->actionCode = item->model2;
                secondRecord->animFrame = item->renderArg0;
                secondRecord->drawFlags = item->renderArg1;
            }
        }
        if (item->contactRadius == 0)
            continue;
        if (item->section != state->nodeIndex)
            continue;
        switch (*(char *)&g_SnoboStatePtr->riderState) {
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0A:
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0C:
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0E:
            if (g_SnoboStatePtr->frameCount < 8)
                continue;
        }
        itemH = item->height >> 12;
        offsetAmt = itemH - item->contactTop;
        if (position[1] < offsetAmt)
            continue;
        offsetAmt = itemH + 0x100;
        if (position[1] > offsetAmt)
            continue;
        offsetAmt = itemH - item->contactSplit;
        ofs = position[1] < offsetAmt;
        rangeLimit = item->contactRadius;
        offsetAmt = abs(position[0] - (item->sidePos >> 12));
        if (offsetAmt > rangeLimit)
            continue;
        itemH = abs(position[2] - (item->forward >> 12));
        if (itemH > rangeLimit)
            continue;
        offsetAmt = SqrtToInt(offsetAmt * offsetAmt + itemH * itemH);
        if (offsetAmt > rangeLimit)
            continue;
        item->contact |= 1;
        if (item->contactResponse[0] == 0xff)
            continue;
        Mini_SnowboardRestoreRiderSnapshot();
        if (g_SnoboStatePtr->followAngle > 0x100) {
            ofs += 2;
            item->contact |= 2;
            item->hitVelocityX = (int)g_SnoboStatePtr->velocityX;
            item->hitVelocityZ = (int)g_SnoboStatePtr->velocityZ;
        }
        if (item->contactResponse[ofs] != 0) {
            offsetAmt = item->contactResponse[ofs];
            if (offsetAmt == 0x80) {
                if (*(char *)&g_SnoboStatePtr->riderState == 11)
                    offsetAmt = 12;
                else
                    offsetAmt = g_SnoboStatePtr->followAngle > 0x280 ? 8 : 12;
            }
            if (offsetAmt == 8) {
                if (g_SnoboStatePtr->triangle % 12 >= 6)
                    offsetAmt++;
                g_SnoboStatePtr->velocityX -= g_SnoboStatePtr->velocityX * 0.5f;
                g_SnoboStatePtr->velocityZ -= g_SnoboStatePtr->velocityZ * 0.5f;
            }
            g_SnoboStatePtr->riderState = (short)offsetAmt;
        } else {
            *(int *)&g_SnoboStatePtr->velocityZ = 0;
            *(int *)&g_SnoboStatePtr->velocityX = 0;
        }
    }
}
} /* extern "C" */

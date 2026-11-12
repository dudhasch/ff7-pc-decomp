// src/ff7/src/battle/battle_actor_anim_effect_tasks.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Resets battle animation state and runs actor animation scripts, pose and scale changes,
//   and the effect and overlay tasks that move, pulse or flash actors and fire delayed events
//   during a battle.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "lib/pfile.h"
#include "ff7/types/battle_actors.h"
#include "ff7/types/menu.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/enums/sound.h"
#include "lib/types/polygon.h"
#include "lib/types/thread.h"
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/graphics.h"
#include "lib/api/thread.h"
#include "lib/api/polygon.h"
#include "lib/api/sound.h"
#include <stdio.h>
extern "C" {

/* The slot of the secondary task that runs now. */
#define BATTLE_CURRENT_SECONDARY_SLOT (&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx])
#define BATTLE_SECONDARY_VIEW(type) ((type *)BATTLE_CURRENT_SECONDARY_SLOT)
#define BATTLE_MAIN_VIEW(type) ((type *)BATTLE_CURRENT_TASK_SLOT)
#define BATTLE_SLOT_VIEW(type, slot) ((type *)(slot))


__declspec(dllimport) void __stdcall OutputDebugStringA(const char *);

// clang-format off
// GLOBAL: FF7 0x007c1028
char g_VincentGallDat[9] = "GALL.DAT";
// GLOBAL: FF7 0x007c1038
char g_VincentDeathgigDat[13] = "DEATHGIG.DAT";
// GLOBAL: FF7 0x007c1048
char g_VincentHellmaskDat[13] = "HELLMASK.DAT";
// GLOBAL: FF7 0x007c1058
char g_VincentChaosDat[10] = "CHAOS.DAT";
// GLOBAL: FF7 0x007c1068
char g_VincentDat[12] = "VINSENT.DAT";
// GLOBAL: FF7 0x007c1078
char g_Vincent2Dat[13] = "VINSENT2.DAT";
// GLOBAL: FF7 0x007c1088
char g_Vincent3Dat[13] = "VINSENT3.DAT";
// clang-format on
// GLOBAL: FF7 0x007c1098
int g_VincentModelFilenames[7] = {(int)&g_VincentGallDat, (int)&g_VincentDeathgigDat, (int)&g_VincentHellmaskDat, (int)&g_VincentChaosDat, (int)&g_VincentDat, (int)&g_Vincent2Dat, (int)&g_Vincent3Dat};
// GLOBAL: FF7 0x007c10b8
short g_BattleBoneParticleBurstParam[16] = {200, 320, 320, 320, 320, 320, 320, 320, 320, 320, 320, 320, 320, 320, 320, 320};
// GLOBAL: FF7 0x007c10d8
int g_BattleSkipFreeEffectGroupTag0 = 1;
// GLOBAL: FF7 0x007c10dc
int g_BattleAssetSourceModeSelector = 1;
// GLOBAL: FF7 0x007c10e0
unsigned char g_BattleBuiltinAnimScript2E[15] = {0xce, 0xc2, 0x00, 0xf4, 0x01, 0xf3, 0xee, 0xcd, 0xc2, 0x00, 0xf6, 0xf4, 0x01, 0xf3, 0xee};
// GLOBAL: FF7 0x007c10f0
unsigned char g_BattleBuiltinAnimScript2F[5] = {0xb7, 0xf4, 0x01, 0xf3, 0xee};
// GLOBAL: FF7 0x007c10f8
unsigned char g_BattleBuiltinAnimScript30[16] = {0xce, 0xe5, 0xf2, 0xe5, 0xf4, 0x01, 0xf3, 0xee, 0xcd, 0xe5, 0xf6, 0xe5, 0xf4, 0x01, 0xf3, 0xee};
// GLOBAL: FF7 0x007c1108
unsigned char g_BattleBuiltinAnimScript3A[1] = {0xee};
// GLOBAL: FF7 0x007c110c
unsigned char g_BattleBuiltinAnimScript3B[2] = {0x00, 0xee};
// GLOBAL: FF7 0x007c1110
unsigned char g_BattleBuiltinAnimScript39[7] = {0xe7, 0x00, 0xe1, 0xc5, 0xf3, 0x00, 0xee};
// GLOBAL: FF7 0x007c1118
unsigned char g_BattleBuiltinAnimScript34[8] = {0xe8, 0xc9, 0xf4, 0x01, 0xf3, 0xca, 0xec, 0xee};
// GLOBAL: FF7 0x007c1120
unsigned char g_BattleBuiltinAnimScript31[12] = {0xe5, 0xc8, 0x00, 0x00, 0x56, 0x00, 0x06, 0xf4, 0x06, 0xf3, 0xe3, 0xee};
// GLOBAL: FF7 0x007c1130
unsigned char g_BattleBuiltinAnimScript32[12] = {0xe5, 0xc8, 0x00, 0x00, 0xaa, 0xff, 0x06, 0xf4, 0x06, 0xf3, 0xe3, 0xee};
// GLOBAL: FF7 0x007c1140
unsigned char g_BattleBuiltinAnimScript38[12] = {0xfc, 0x03, 0xed, 0xe6, 0xea, 0x0c, 0x0d, 0x0e, 0x04, 0xfa, 0xe5, 0xee};
// GLOBAL: FF7 0x007c1150
unsigned char g_BattleBuiltinAnimScript37[12] = {0xfc, 0x03, 0xed, 0xa4, 0xea, 0x0c, 0x0d, 0x0e, 0x04, 0xfa, 0xe5, 0xee};
// GLOBAL: FF7 0x007c1160
unsigned char g_BattleBuiltinAnimScript36[12] = {0xfc, 0x03, 0xed, 0xa5, 0xea, 0x0c, 0x0d, 0x0e, 0x04, 0xfa, 0xe5, 0xee};
// GLOBAL: FF7 0x007c1170
unsigned char g_BattleBuiltinAnimScript35[8] = {0x00, 0xe0, 0xf4, 0x19, 0xf3, 0xfa, 0xe5, 0xee};
// GLOBAL: FF7 0x007c1178
int g_BattleRestingYArrayData = (int)0xFF9CFFCE;
unsigned char g_rawData_007c117c[4] = {0x00, 0x00, 0x64, 0x00};
unsigned char g_rawData_007c1180[136] = {
    0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0xff, 0x38, 0xff, 0x00, 0x00, 0xc8, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x38, 0xff, 0xd4, 0xfe, 0x00, 0x00, 0x2c, 0x01, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd4, 0xfe, 0x0c, 0xfe, 0x00, 0x00, 0xf4, 0x01,
    0x2c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xce, 0xff, 0xce, 0xff, 0xce, 0xff, 0xe7, 0xff, 0x19, 0x00, 0x32, 0x00, 0x32, 0x00, 0x32, 0x00,
    0x9c, 0xff, 0x9c, 0xff, 0x9c, 0xff, 0xce, 0xff, 0x32, 0x00, 0x64, 0x00, 0x64, 0x00, 0x64, 0x00, 0x6a, 0xff, 0x6a, 0xff, 0x6a, 0xff, 0xb5, 0xff,
    0x4b, 0x00, 0x96, 0x00, 0x96, 0x00, 0x96, 0x00, 0x38, 0xff, 0x38, 0xff, 0x38, 0xff, 0x9c, 0xff, 0x64, 0x00, 0xc8, 0x00, 0xc8, 0x00, 0xc8, 0x00,
    0x18, 0xfc, 0x30, 0xf8, 0xd0, 0x07, 0xe8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x007c1208
void (__cdecl *g_BattleFxOverlayCallbacks[322])(void) = {0, BattleFx_MagicLoadResFire, BattleFx_MagicLoadResFire2, BattleFx_MagicLoadResBolt, BattleFx_MagicLoadResBolt2, BattleFx_MagicLoadResBolt3, BattleFx_MagicLoadResBolt3Multi, BattleFx_MagicLoadResIce, BattleFx_MagicLoadResIce2, BattleFx_MagicLoadResDemi, BattleFx_MagicLoadResSlow, BattleFx_MagicLoadResBarrier, BattleFx_MagicLoadResMBarrier, BattleFx_MagicLoadResReflect, BattleFx_MagicLoadResBerserk, BattleFx_SummonLoadResLeviathan, BattleFx_SpecialLoadResSmokeShot, BattleFx_LimitLoadResHealingWind, BattleFx_LimitLoadResBraver, BattleFx_LimitLoadResCrossSlash, BattleFx_LimitLoadResBigShot, BattleFx_SpecialLoadResLaserCannon, BattleFx_MagicLoadResRemove, BattleFx_MagicLoadResTornado, BattleFx_MagicLoadResDemi2, BattleFx_MagicLoadResDemi3, BattleFx_MagicLoadResDemi3Multi, BattleFx_MagicLoadResComet, BattleFx_MagicLoadResBio, BattleFx_MagicLoadResBio2, BattleFx_MagicLoadResComet2, BattleFx_MagicLoadResBio3, BattleFx_MagicLoadResBio3Multi, BattleFx_MagicLoadResBreak, 0, BattleFx_MagicLoadResEsuna, BattleFx_MagicLoadResResist, BattleFx_BlueLoadResWhiteWind, BattleFx_MagicLoadResHitmark, BattleFx_SummonLoadResTitan, BattleFx_SummonLoadResRamuh, BattleFx_SummonLoadResIfrit, BattleFx_SummonLoadResShiva, BattleFx_MagicLoadResFreeze, BattleFx_BlueLoadResFlameThrower, 0, BattleFx_SpecialLoadResWhirlpool, 0, BattleFx_SpecialLoadResTimeDamage, BattleFx_MagicLoadResSleepel, BattleFx_MagicLoadResHaste, BattleFx_MagicLoadResQuake2, BattleFx_MagicLoadResQuake3, BattleFx_MagicLoadResQuake, BattleFx_MagicLoadResSilence, BattleFx_MagicLoadResStop, BattleFx_MagicLoadResFlare, BattleFx_MagicLoadResFire3, BattleFx_MagicLoadResIce3, BattleFx_BlueLoadResBeta, BattleFx_MagicLoadResUltima, BattleFx_MagicLoadResDeBarrier, BattleFx_BlueLoadResShadowFlare, BattleFx_MagicLoadResDespell, BattleFx_MagicLoadResMini, BattleFx_MagicLoadResToad, BattleFx_BlueLoadResTrine, BattleFx_BlueLoadResMagicBreath, BattleFx_BlueLoadResAqualung, BattleFx_BlueLoadResGoblinPunch, BattleFx_BlueLoadResQuestionMarks, BattleFx_BlueLoadResBadBreath, BattleFx_BlueLoadResMagicHammer, BattleFx_BlueLoadResDragonForce, BattleFx_BlueLoadResFrogSong, BattleFx_BlueLoadResDeathForce, BattleFx_BlueLoadResL5Death, BattleFx_MagicLoadResFire3Multi, BattleFx_BlueLoadResLaser, BattleFx_BlueLoadResMatraMagic, BattleFx_BlueLoadResL4Suicide, BattleFx_BlueLoadResBigGuard, BattleFx_BlueLoadResPandorasBox, BattleFx_BlueLoadResRoulette, BattleFx_BlueLoadResAngelWhisper, BattleFx_MagicLoadResDeath, BattleFx_BlueLoadResChocobuckle, BattleFx_SpecialLoadResUltrasound, BattleFx_SpecialLoadResVacuum, BattleFx_SpecialLoadResSearchlight, BattleFx_SpecialLoadResThousandNeedles, BattleFx_SpecialLoadResSeedShot, BattleFx_SpecialLoadResBeamGun, BattleFx_SpecialLoadResRefuseSpit, BattleFx_SpecialLoadResSilverWheel, BattleFx_SpecialLoadResAutumnLeaves, BattleFx_SpecialLoadResSandstorm, BattleFx_SpecialLoadResSpiderWeb, BattleFx_SpecialLoadResStareDown, BattleFx_SpecialLoadResFourLaser, BattleFx_SpecialLoadResFascination, BattleFx_SpecialLoadResBlaster, BattleFx_SpecialLoadResRocketLauncher, BattleFx_SpecialLoadResFlyingSickle, BattleFx_SpecialLoadResDrain, BattleFx_SpecialLoadResEvilPoison, BattleFx_SpecialLoadResShamSeal, BattleFx_SpecialLoadResSelfDestruct, BattleFx_SpecialLoadResEnergySiphon, BattleFx_SpecialLoadResToxicSmog, BattleFx_SpecialLoadResTriangleAttack, BattleFx_SpecialLoadResWaterGun, BattleFx_SpecialLoadResHarrier, BattleFx_SpecialLoadResFallingRocks, BattleFx_SpecialLoadResColdBreath, BattleFx_SpecialLoadResAtomicRay, BattleFx_SpecialLoadResBadMouth, BattleFx_SpecialLoadResOil, BattleFx_SpecialLoadResPoisonBreath, BattleFx_SpecialLoadResLiquidPoison, BattleFx_SpecialLoadResSnowstorm, BattleFx_SpecialLoadResQuasar, BattleFx_SpecialLoadResSouthernCross, BattleFx_SpecialLoadResNorthernCross, BattleFx_SpecialLoadResSearchScope, BattleFx_SpecialLoadResWarCry, BattleFx_SpecialLoadResPollen, BattleFx_SpecialLoadResBigWave, BattleFx_SpecialLoadResSmog, BattleFx_SpecialLoadResCoinToss, BattleFx_SpecialLoadResGeyser, BattleFx_SpecialLoadResHellBubbles, BattleFx_SpecialLoadResSwiftBolt, BattleFx_SpecialLoadResHotSprings, BattleFx_SpecialLoadResMagma, BattleFx_SpecialLoadResHeatWave, BattleFx_SpecialLoadResSwampShoot, BattleFx_SpecialLoadResSephirothShock, 0, BattleFx_SpecialLoadResSabotage, BattleFx_SpecialLoadResDoubleLaser, BattleFx_SpecialLoadResIcicleDrop, BattleFx_SpecialLoadResBombBlast, BattleFx_SpecialLoadResFireball, BattleFx_SpecialLoadResFireShell, BattleFx_SpecialLoadResColdAir, BattleFx_SpecialLoadResLightBlast2, BattleFx_SpecialLoadResLightBlast3, BattleFx_SpecialLoadResMagmaEruption, BattleFx_MagicLoadResShield, BattleFx_SpecialLoadResHeatBlast, BattleFx_SpecialLoadResShadyBreath, BattleFx_SpecialLoadResWhirlwind, BattleFx_SpecialLoadResSirenSong, BattleFx_SpecialLoadResFullExplosion, BattleFx_SpecialLoadResChainExplosion, BattleFx_SpecialLoadResDarkWave, BattleFx_SpecialLoadResFreezeBreath, BattleFx_SpecialLoadResLightShell, BattleFx_SpecialLoadResRiderBreath, BattleFx_SpecialLoadResHellBomber, BattleFx_SpecialLoadResAvalanche, BattleFx_SpecialLoadResElectricShock, BattleFx_SpecialLoadResTailLaser, BattleFx_SpecialLoadResGrandSpark, BattleFx_SpecialLoadResAcidShower, BattleFx_SpecialLoadResPoisonGas, BattleFx_SpecialLoadResDualMortar, BattleFx_SpecialLoadResMortar, BattleFx_SpecialLoadResJammer, BattleFx_SpecialLoadResMultiJammer, BattleFx_SpecialLoadResHeartBreath, BattleFx_SpecialLoadResWingScales, BattleFx_SpecialLoadResNapalm, BattleFx_SpecialLoadResPetrify, BattleFx_SpecialLoadResMissileSalvo, BattleFx_SpecialLoadResGiantSonicWave, BattleFx_SpecialLoadResFrog, BattleFx_SpecialLoadResChefsKnife, BattleFx_SummonLoadResPhoenix, Battle_ItemFxGreensInit, BattleFx_SpecialLoadResWaterball, BattleFx_SpecialLoadResSewerTsunami, BattleFx_SpecialLoadResRadiantBreath, BattleFx_SpecialLoadResEnergyBall, BattleFx_SpecialLoadResUltimateBlast, BattleFx_SpecialLoadResSpreadLaser, BattleFx_SpecialLoadResEnergy, BattleFx_SpecialLoadResEnergyWave, BattleFx_SummonLoadResChocoMog, BattleFx_SummonLoadResFatChocobo, BattleFx_SummonLoadResAlexander, 0, BattleFx_SummonLoadResKjata, 0, BattleFx_SpecialLoadResSpadeCard, BattleFx_SpecialLoadResEmperorCard, BattleFx_SpecialLoadResHeartCard, BattleFx_SpecialLoadResHiddenOneCard, BattleFx_SpecialLoadResClubCard, BattleFx_SpecialLoadResStarCard, BattleFx_SpecialLoadResDiamondCard, BattleFx_SpecialLoadResSunCard, BattleFx_SpecialLoadResJokerCard, BattleFx_SpecialLoadResWorldCard, BattleFx_LimitLoadResSledFang, BattleFx_LimitLoadResTifa1, 0, BattleFx_SpecialLoadResShield, BattleFx_SpecialLoadResWaveCannon, BattleFx_SpecialLoadResEnergyOrb, BattleFx_SpecialLoadResScorch, BattleFx_SpecialLoadResSuperOrb, BattleFx_SummonLoadResTyphon, 0, BattleFx_MagicLoadResFullCure, BattleFx_BlueLoadResFlameThrowerAttach, BattleFx_ThrowLoadResCloudWeapons, BattleFx_ThrowLoadResBarretWeapons, BattleFx_ThrowLoadResAerisWeapons, BattleFx_ThrowLoadResCidWeapons, BattleFx_ThrowLoadResYuffieWeapons, BattleFx_ThrowLoadResCaitSithWeapons, BattleFx_ThrowLoadResVincentWeapons, BattleFx_LimitLoadResToyBox, BattleFx_LimitLoadResHowlingMoon, BattleFx_LimitLoadResStardustRay, BattleFx_LimitLoadResLunaticHigh, BattleFx_LimitLoadResEarthRave, BattleFx_LimitLoadResCosmoMemory, 0, 0, BattleFx_LimitLoadResLuckyGirl, BattleFx_LimitLoadResMeteorain, BattleFx_LimitLoadResFinishingTouch, BattleFx_SummonLoadResHades, BattleFx_LimitLoadResDynamite, BattleFx_LimitLoadResClearTranquil, BattleFx_LimitLoadResHighwind, BattleFx_LimitLoadResAllCreation, BattleFx_LimitLoadResBoostJump, BattleFx_LimitLoadResDragon, BattleFx_LimitLoadResHyperJump, BattleFx_LimitLoadResDragonDive, BattleFx_LimitLoadResBigBrawl, BattleFx_LimitLoadResGreasedLightning, BattleFx_LimitLoadResBloodfest, BattleFx_LimitLoadResGauntlet, BattleFx_LimitLoadResDoomOfTheLiving, BattleFx_LimitLoadResMindblow, BattleFx_LimitLoadResPulseOfLife, BattleFx_LimitLoadResGalianBeast, BattleFx_SummonLoadResOdinZantetsuken, BattleFx_SummonLoadResOdinGungnir, BattleFx_LimitLoadResDice, BattleFx_MagicLoadResIce3Multi, 0, BattleFx_SummonLoadResBahamut, 0, BattleFx_LimitLoadResFuryBrand, BattleFx_LimitLoadResLandscaper, 0, BattleFx_SummonLoadResNeoBahamut, 0, 0, BattleFx_SummonLoadResBahamutZero, 0, 0, 0, 0, 0, BattleFx_SummonLoadResKnightsOfRound, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, BattleFx_LimitLoadResGreatGospel, BattleFx_LimitLoadResSealEvil, BattleFx_LimitLoadResGrenadeBomb, BattleFx_LimitLoadResSatelliteBeam, BattleFx_LimitLoadResBladeBeam, BattleFx_LimitLoadResOmnislash, 0, 0, BattleFx_LimitLoadResCatastrophe, BattleFx_LimitLoadResHammerblow, BattleFx_LimitLoadResClimhazzard, BattleFx_LimitLoadResUngarmax, Battle_ItemFxLoadResEchoScreen, Battle_ItemFxLoadResSmokeBomb, BattleFx_LimitLoadResLivewire, BattleFx_LimitLoadResNightmare, BattleFx_LimitLoadResSatanSlam, BattleFx_SpecialLoadResRocketPunch, BattleFx_LimitLoadResBreathOfTheEarth, BattleFx_LimitLoadResPlanetProtector, BattleFx_SpecialLoadResBizarroEnergy, BattleFx_MagicLoadResWall, BattleFx_SpecialLoadResHeartlessAngel, BattleFx_SpecialLoadResBoulderThrow, BattleFx_SpecialLoadResSaferSephirothDeath, BattleFx_WeaponLoadResWhirlsand, BattleFx_WeaponLoadResRubyTentacles, BattleFx_WeaponLoadResEmeraldShoot, BattleFx_WeaponLoadResEmeraldBeam, BattleFx_WeaponLoadResRevengeStamp, BattleFx_WeaponLoadResDiamondFlash, BattleFx_WeaponLoadResSuperNova, Battle_EnsureParticleResource, BattleFx_EffectLoadResSBaria, BattleFx_EffectLoadResHitMark};
// GLOBAL: FF7 0x007c1710
short g_BattleFxOverlayResourceMap1[70] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 65, 64, 299, 139, 47, 45, 300, 50, 81, 36, 1, 155, 154, 55, 85, 10, 49, 54, 14, 86, 2, 77, 8, 255, 4, 6, 51, 52, 29, 32, 30, 104, 108, 33, 11, 12, 13, 63, 152, 68, 65, 64, 35, 1, 168, 173, 9, 24, 105, 90, 183, 71, 180, 180, 180, 180, 180, 180, 180, 180};
// GLOBAL: FF7 0x007c17a0
short g_BattleFxOverlayResourceMap2[54] = {0, 0, 0, 0, 0, 35, 36, 84, 84, 64, 65, 49, 86, 54, 14, 11, 12, 13, 308, 50, 10, 55, 61, 63, 85, 34, 22, 1, 2, 57, 7, 8, 58, 3, 4, 5, 53, 51, 52, 28, 29, 31, 9, 24, 25, 27, 30, 43, 33, 23, 56, 215, 60, 149};
// GLOBAL: FF7 0x007c1810
short g_BattleFxOverlayResourceMap3[24] = {74, 80, 72, 37, 81, 84, 73, 75, 44, 78, 79, 71, 59, 68, 66, 67, 70, 69, 86, 76, 83, 83, 62, 82};
// GLOBAL: FF7 0x007c1840
short g_BattleFxOverlayResourceMap4[18] = {189, 42, 41, 40, 39, 252, 15, 257, 193, 191, 179, 262, 235, 213, 265, 271, 190, 253};
// GLOBAL: FF7 0x007c1868
short g_BattleFxOverlayResourceMap5[158] = {158, 143, 97, 93, 21, 99, 111, 91, 114, 118, 116, 87, 96, 132, 94, 117, 98, 89, 135, 102, 109, 105, 92, 104, 136, 137, 127, 88, 107, 122, 1, 90, 121, 161, 103, 95, 113, 100, 101, 110, 115, 1, 188, 184, 112, 1, 195, 197, 199, 201, 203, 196, 198, 200, 202, 204, 106, 159, 144, 120, 119, 125, 126, 160, 16, 108, 164, 142, 123, 209, 216, 44, 182, 182, 181, 124, 163, 151, 141, 131, 165, 130, 133, 134, 129, 152, 152, 174, 167, 168, 139, 140, 138, 154, 0, 157, 44, 44, 145, 147, 146, 187, 150, 46, 156, 175, 183, 173, 166, 211, 210, 172, 171, 169, 47, 162, 153, 185, 148, 48, 178, 155, 118, 126, 128, 38, 177, 208, 208, 304, 304, 44, 176, 53, 51, 52, 4, 170, 44, 44, 186, 212, 307, 318, 84, 309, 310, 311, 0, 312, 313, 313, 314, 315, 316, 317, 315, 0};
// GLOBAL: FF7 0x007c19a8
short g_BattleFxOverlayResourceMap6[80] = {18, 19, 291, 297, 233, 234, 292, 20, 289, 249, 296, 290, 298, 295, 17, 288, 305, 259, 306, 250, 287, 206, 206, 206, 206, 206, 206, 206, 240, 241, 242, 236, 243, 244, 238, 205, 225, 205, 226, 227, 228, 229, 254, 1, 1, 1, 1, 1, 1, 245, 237, 260, 246, 247, 248, 239, 261, 230, 231, 232, 76, 85, 251, 251, 251, 301, 251, 302, 251, 303, 113, 141, 224, 224, 224, 224, 30, 234, 303, 291};
// GLOBAL: FF7 0x007c1a48
short g_BattleFxOverlayResourceMap7[128] = {0, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 0};
// GLOBAL: FF7 0x007c1b48
unsigned char g_BattleLimitAnimBraverDat[11] = "BLAVER.DAT";
// The six code literals precede the table's strings in retail .data (0x7C1C98..0x7C1D93, then 0x7C1D94): a
// literal returned by a static __inline function is a parse-time unit at the function's definition, and the
// call expands to the plain push under /Ob1. Battle_AdvanceActorAnimation,
// Battle_StartMagicEffectThread and Battle_PollMagicEffectThread use them.
static __inline char *Battle_AnmErrorNoAnimationFormat(void)
{
    // STRING: FF7 0x007c1c98
    return "ANM ERROR: ANIMATION == 0: TOTAL ANIMATIONS %d CURRENT ANIMATION %d \n";
}
static __inline char *Battle_AnmErrorAnimationRangeFormat(void)
{
    // STRING: FF7 0x007c1ce0
    return "ANM ERROR: TOTAL ANIMATIONS %d CURRENT ANIMATION %d \n";
}
static __inline char *Battle_MagicThreadStartText(void)
{
    // STRING: FF7 0x007c1d18
    return "MAGIC THREAD START \n";
}
static __inline char *Battle_MagicThreadCreateErrorText(void)
{
    // STRING: FF7 0x007c1d30
    return "ERROR: COULD NOT START MAGIC THREAD \n";
}
static __inline char *Battle_MagicThreadBusyErrorText(void)
{
    // STRING: FF7 0x007c1d58
    return "ERROR: MAGIC THREAD ALREADY IN USE \n";
}
static __inline char *Battle_MagicThreadEndText(void)
{
    // STRING: FF7 0x007c1d80
    return "MAGIC THREAD END \n";
}
// GLOBAL: FF7 0x007c1b58
unsigned int g_BattleLimitAnimBankPointers[80] = {
    (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)"KYOU.DAT", (unsigned int)"LIMCL2.DAT", (unsigned int)"LIMCL6.DAT",
    (unsigned int)"LIMCL3.DAT", (unsigned int)"LIMCL7.DAT", (unsigned int)"LIMCL4.DAT", (unsigned int)"HVSHOT.DAT",
    (unsigned int)"LIMBR5.DAT", (unsigned int)"LIMBR2.DAT", (unsigned int)"LIMBR6.DAT", (unsigned int)"LIMBR3.DAT",
    (unsigned int)"LIMBR7.DAT", (unsigned int)"LIMBR4.DAT", (unsigned int)"IYASH.DAT", (unsigned int)"LIMEA5.DAT",
    (unsigned int)"LIMEA2.DAT", (unsigned int)"LIMEA6.DAT", (unsigned int)"LIMEA3.DAT", (unsigned int)"KODO.DAT",
    (unsigned int)"LIMEA4.DAT", (unsigned int)"LIMFAST.DAT", (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)g_BattleLimitAnimBraverDat,
    (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)g_BattleLimitAnimBraverDat,
    (unsigned int)"LIMCD1.DAT", (unsigned int)"LIMCD5.DAT", (unsigned int)"LIMCD2.DAT", (unsigned int)"LIMCD6.DAT",
    (unsigned int)"LIMCD3.DAT", (unsigned int)"LIMCD7.DAT", (unsigned int)"LIMCD4.DAT", (unsigned int)"LIMSLED.DAT",
    (unsigned int)"LIMRD5.DAT", (unsigned int)"LIMSLED.DAT", (unsigned int)"LIMRD6.DAT", (unsigned int)"LIMRD3.DAT",
    (unsigned int)"LIMRD7.DAT", (unsigned int)"LIMRD4.DAT", (unsigned int)"DICE.DAT", (unsigned int)g_BattleLimitAnimBraverDat,
    (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)g_BattleLimitAnimBraverDat,
    (unsigned int)g_BattleLimitAnimBraverDat, (unsigned int)"LIMYF1.DAT", (unsigned int)"LIMYF5.DAT", (unsigned int)"LIMYF2.DAT",
    (unsigned int)"LIMYF6.DAT", (unsigned int)"LIMYF3.DAT", (unsigned int)"LIMYF7.DAT", (unsigned int)"LIMYF4.DAT",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, (unsigned int)"LIMCL7.DAT", 0, (unsigned int)"LIMCL2.DAT"
};
// clang-format on


/* The pending hit reactions, one per hit display entry. */



// clang-format off
// GLOBAL: FF7 0x009aded8
int g_VincentLimitModelIndex = 0;
unsigned char g_bssPad_009adedc[4] = {0};
// GLOBAL: FF7 0x009adee0
int g_BattleAnimTextureSetHandles[2] = {0};
// GLOBAL: FF7 0x009adee8
int g_BattleAnimTextureSetCount = 0;
unsigned char g_bssPad_009adeec[4] = {0};
// GLOBAL: FF7 0x009adef0
struct BattleSkeletonTextureRecord *g_BattleAnimTextureSetRecords[2] = {0};
// GLOBAL: FF7 0x009adef8
char g_BattleAnimTransformActive = 0;
// GLOBAL: FF7 0x009adefc
char g_BattleAnimPreloadReady = 0;
// GLOBAL: FF7 0x009adf00
char g_BattleAnimPreloadActor = 0;
// GLOBAL: FF7 0x009adf04
char g_BattleEffectOpStalled = 0;
// GLOBAL: FF7 0x009adf08
char g_BattleAnimTextureSetLoaded = 0;
// GLOBAL: FF7 0x009adf0c
struct ThreadRecord *g_BattleMagicEffectThread = 0;
// clang-format on

/*
 * Clears the four battle animation debug flags g_BattleAnimTransformActive..g_BattleEffectOpStalled.
 */
// FUNCTION: FF7 0x0041fa70
void __cdecl Battle_ResetAnimDebugFlags(void)
{
    g_BattleAnimTransformActive = 0;
    g_BattleAnimPreloadReady = 0;
    g_BattleAnimPreloadActor = 0;
    g_BattleEffectOpStalled = 0;
}

/*
 * Zeroes actor actorIndex's node translation (bone0PosX, bone0PosY and bone0PosZ) and its
 * fadeFlags; unused and unusedB are unused.
 */
// FUNCTION: FF7 0x0041fa91
void __cdecl Battle_ClearActorNodeTranslation(void *unused, int unusedB, short actorIndex)
{
    g_BattleActorRecords[actorIndex].bone0PosZ = 0;
    g_BattleActorRecords[actorIndex].bone0PosY = 0;
    g_BattleActorRecords[actorIndex].bone0PosX = 0;
    g_BattleActorRecords[actorIndex].fadeFlags = 0;
}

/*
 * Zeroes the auxiliary node translation (auxPosX, auxPosY and auxPosZ) of slot slotIndex
 * in g_BattleActorTransformRecords.
 */
// FUNCTION: FF7 0x0041fae0
void __cdecl Battle_ClearAuxNodeTranslation(void *unused, int unusedB, short slotIndex)
{
    g_BattleActorTransformRecords[slotIndex].auxPosZ = 0;
    g_BattleActorTransformRecords[slotIndex].auxPosY = 0;
    g_BattleActorTransformRecords[slotIndex].auxPosX = 0;
}

/*
 * Always 1.
 */
// FUNCTION: FF7 0x0041fb15
char __cdecl Battle_ReturnOne(void)
{
    return 1;
}

// Collects up to two texture sets for the battle animation, while fewer than two have been
// collected. It scans the skeleton's records, recordCount of them with a stride of 0x64. Each
// record whose handle is nonzero is copied, with its record pointer, into the global
// handle and record arrays, and the shared count advances. The scan stops once the count reaches
// two.
// FUNCTION: FF7 0x0041fb1c
void __cdecl Battle_CollectSkeletonTextureSets(int unused, BattleSkeletonTextureHeader *skeleton)
{
    BattleSkeletonTextureRecord *record;
    int index;

    if (g_BattleAnimTextureSetCount < 2) {
        record = (BattleSkeletonTextureRecord *)skeleton->records;
        for (index = 0; index < skeleton->recordCount; index++) {
            if (record->handle != 0) {
                g_BattleAnimTextureSetHandles[g_BattleAnimTextureSetCount] = record->handle;
                g_BattleAnimTextureSetRecords[g_BattleAnimTextureSetCount] = record;
                g_BattleAnimTextureSetCount++;
                if (g_BattleAnimTextureSetCount >= 2) {
                    return;
                }
            }
            record++;
        }
    }
}

/*
 * Runs the actor animation script unless battle animation is paused, selecting the actor script
 * table entry or special fixed scripts and restarting pending scripts. When animation completion
 * permits, consumes byte opcodes and signed word operands through the actor byte index to update
 * poses, position, yaw, flags, models and node parents, dispatch sound/camera/effect programs, and
 * allocate movement, particle, timer and message tasks. Wait opcodes rewind the byte index and
 * return or stop this invocation; terminators restore the default script, while ordinary animation
 * bytes reset the frame counters and stop opcode execution. Afterwards advances animation according
 * to the actor mode, including twice, alternate invocations, or the special reset path, unless
 * actor flag 2 suppresses it. Some opcode selectors retain shared scratch values outside their
 * handled cases. Opcode 0xBA falls through to 0xB4, replacing its just-added angle with the
 * base-angle path. The four arguments select an actor, its script table, and animation state; the
 * interpreter state lives in g_BattleActor_BufferPtr. Empty byte scans keep their post-incremented
 * indices. The volatile byte reads keep the original's order of widening the byte before the
 * destination address is computed.
 */
// FUNCTION: FF7 0x0041fba4
void __cdecl Battle_RunActorAnimScript(short actor, int scriptTable, int animMode, int animFlags)
{
    int winEntry;
    int animationIdx;
    int paletteIdx;
    BattleActorAnimState *animStatus;
    GfxDriverOps *graphics;
    short opWord;
    BattleRgbaColor *palettePtr;
    int zero;
    int entSlot;
    int rgba;
    GfxTextureSurface *texHandle;
    int windowBase;
    int loopIndex;
    struct MenuPartyStats *statBlockPtr;

    if (g_BattleIsPaused != 0) {
        return;
    }
    animStatus = g_BattleActor_BufferPtr;
    g_BattleAnimTransformActive = 0;
    animStatus->running = 1;
    animStatus->scriptPtr = ((char **)scriptTable)[g_BattleActorRecords[actor].scriptId];
    switch (g_BattleActorRecords[actor].scriptId) {
    case BATTLE_BUILTIN_SCRIPT_2E:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript2E;
        break;
    case BATTLE_BUILTIN_SCRIPT_2F:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript2F;
        break;
    case BATTLE_BUILTIN_SCRIPT_30:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript30;
        break;
    case BATTLE_BUILTIN_SCRIPT_31:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript31;
        break;
    case BATTLE_BUILTIN_SCRIPT_32:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript32;
        break;
    case BATTLE_BUILTIN_SCRIPT_34:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript34;
        break;
    case BATTLE_BUILTIN_SCRIPT_35:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript35;
        break;
    case BATTLE_BUILTIN_SCRIPT_36:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript36;
        break;
    case BATTLE_BUILTIN_SCRIPT_37:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript37;
        break;
    case BATTLE_BUILTIN_SCRIPT_38:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript38;
        break;
    case BATTLE_BUILTIN_SCRIPT_39:
        g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags | 0x80;
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript39;
        break;
    case BATTLE_BUILTIN_SCRIPT_3A:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript3A;
        break;
    case BATTLE_BUILTIN_SCRIPT_3B:
        animStatus->scriptPtr = (char *)g_BattleBuiltinAnimScript3B;
    }
    if ((g_BattleActorRecords[actor].stateFlags & 1) != 0) {
        Battle_RestartActorScript((short)actor);
        g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags & 0xFFFFFFFE;
    }
    if (g_BattleActorRecords[actor].animRunning != 0) {
        g_BattleActorRecords[actor].nextAnimBank = 0;
        while (animStatus->running != 0) {
            animStatus->opcode = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
            (g_BattleActorRecords[actor].scriptPos)++;
            switch (*(unsigned char *)&animStatus->opcode) {
            case BATTLE_MOTION_RETURN_TO_IDLE:
                if (g_BattleActorRecords[actor].waitFrames == 0) {
                    animStatus->opcode = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
                    if (*(unsigned char *)&animStatus->opcode == BATTLE_MOTION_RETURN_TO_IDLE_MARKER) {
                        g_BattleActorRecords[actor].scriptPos = 0;
                        g_BattleActorTransformRecords[actor].animState = 0;
                        g_BattleActorStates[actor].timer = 0;
                        g_BattleActorRecords[actor].animTicks = 0;
                        g_BattleActorRecords[actor].waitFrames = 0;
                        g_BattleActorRecords[actor].animId = (unsigned short)*(volatile unsigned char *)animStatus->scriptPtr;
                        g_BattleActorRecords[actor].scriptId = (short)*(volatile char *)&g_BattleActorIdleScripts[actor];
                        animStatus->scriptPtr = ((char **)scriptTable)[g_BattleActorRecords[actor].scriptId];
                        Battle_RestartActorScript((short)actor);
                    }
                }
                break;
            case BATTLE_MOTION_DELAYED_WHITE_FLASH:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->taskSlot = Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskDelayedWhiteFlash);
                BATTLE_SLOT_VIEW(BattleEffectDelaySlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->taskSlot])->delay = *(short *)&g_BattleVmReg_0;
                break;
            case BATTLE_MOTION_RESTORE_SFX_VOLUME:
                Battle_RestoreSfxMasterVolume();
                break;
            case BATTLE_MOTION_START_UV_SCROLL:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->secondarySlot = Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskRunUvScrollScript);
                if (actor < 4) {
                    g_BattleVmReg_2 = actor;
                    winEntry = *(int *)g_BattleModelBankSlots[g_BattleVmReg_2];
                } else {
                    g_BattleVmReg_2 = ((struct BattleFormationEntry *)&g_BattleEnemySlots)[actor - 4].slotId;
                    winEntry = *(int *)g_BattleModelBankSlots[g_BattleVmReg_2];
                }
                BATTLE_SLOT_VIEW(BattleUvScrollSlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->script = (unsigned char *)*(int *)((char *)winEntry + g_BattleVmReg_0 * 4 + 0x24);
                BATTLE_SLOT_VIEW(BattleUvScrollSlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->actor = actor;
                BATTLE_SLOT_VIEW(BattleUvScrollSlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->scriptIndex = 0;
                BATTLE_SLOT_VIEW(BattleUvScrollSlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->delay = 0;
                (g_BattleActorStates[actor].frameCounter)++;
                g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot].frameCounter = (unsigned short)*(volatile unsigned char *)&g_BattleActorStates[actor].frameCounter;
                break;
            case BATTLE_MOTION_PLAY_ANIM_STAGGERED:
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleActorRecords[actor].animId = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleActorRecords[actor].animTicks = 0;
                g_BattleActorStates[actor].timer = 0;
                g_BattleActorTransformRecords[actor].animState = 0;
                g_BattleAnimStaggerCounter += 3;
                g_BattleVmReg_0 = g_BattleAnimStaggerCounter & 0xF;
                for (loopIndex = 0; loopIndex < g_BattleVmReg_0; loopIndex++) {
                    (g_BattleActorStates[actor].timer)++;
                    Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
                }
                animStatus->running = 0;
                break;
            case BATTLE_MOTION_CLEAR_HIDE_TARGET_MARKER:
                g_BattleScriptAnimRunFlag = 0;
                break;
            case BATTLE_MOTION_HIDE_TARGET_MARKER_ONCE:
                if (g_BattleTaskSlotBuffer.animRunning == 0) {
                    g_BattleScriptAnimRunFlag = 1;
                    g_BattleTaskSlotBuffer.animRunning = 1;
                }
                break;
            case BATTLE_MOTION_SET_EFFECT_ACTIVE:
                g_BattleScriptEffectFlag = 1;
                break;
            case BATTLE_MOTION_SCREEN_COLOR_RAMP:
                Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskScreenColourRamp);
                break;
            case BATTLE_MOTION_SET_CAMERA_SCRIPT:
                g_BattleCameraScriptIndex = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].targetIndex = 0xFFFE;
                Battle_ApplySequenceCamera();
                break;
            case BATTLE_MOTION_SET_FORMATION_CAMERA:
                g_BattleCurrFormationCameraIdx = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
                (g_BattleActorRecords[actor].scriptPos)++;
                break;
            case BATTLE_MOTION_SET_POSITION:
                g_BattleActorRecords[actor].posX = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleActorRecords[actor].posY = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleActorRecords[actor].posZ = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                break;
            case BATTLE_MOTION_ROTATE_YAW:
                g_BattleVmReg_0 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_1 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_2 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->taskSlot = Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskRotateYaw);
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].actorId = actor;
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].battleActorIndex = *(short *)&g_BattleVmReg_0;
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].velocityX = *(short *)&g_BattleVmReg_1;
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].velocityZ = (g_BattleVmReg_1 - g_BattleVmReg_0) / g_BattleVmReg_2;
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].sinkCountdown = *(short *)&g_BattleVmReg_2;
                break;
            case BATTLE_MOTION_YAW_OFFSET_THEN_RESET:
                g_BattleActorRecords[actor].yaw = g_BattleActorRecords[actor].facing;
                g_BattleActorRecords[actor].yaw += (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
            case BATTLE_MOTION_RESET_YAW_LAYOUT_2_4:
                switch (*(char *)&g_BattleLayout) {
                case BATTLE_LAYOUT_BACK_ATTACK:
                    g_BattleActorRecords[actor].facing = 0x800;
                    break;
                case BATTLE_LAYOUT_PINCER_ATTACK:
                    g_BattleActorRecords[actor].facing = 0;
                }
                g_BattleActorRecords[actor].yaw = g_BattleActorRecords[actor].facing;
                break;
            case BATTLE_MOTION_RESET_YAW_TURN_PARTY_LAYOUT_2:
                switch (*(char *)&g_BattleLayout) {
                case BATTLE_LAYOUT_PREEMPTIVE:
                    break;
                case BATTLE_LAYOUT_BACK_ATTACK:
                    if (actor < 4) {
                        g_BattleActorRecords[actor].facing = 0x800;
                    }
                }
                g_BattleActorRecords[actor].yaw = g_BattleActorRecords[actor].facing;
                break;
            case BATTLE_MOTION_RESET_YAW_BY_LAYOUT:
                switch (g_BattleLayout) {
                case BATTLE_LAYOUT_PINCER_ATTACK:
                    if (actor < 4 && (actor != g_BattleSpecialActorId || actor != g_BattleActiveActorId)) {
                        if (g_BattleSequenceStepBusy == 0) {
                            if (actor == g_BattleActiveActorId) {
                                if (g_BattleActorRecords[g_BattleSpecialActorId].facing == 0) {
                                    g_BattleActorRecords[actor].facing = 0x800;
                                } else {
                                    g_BattleActorRecords[actor].facing = 0;
                                }
                            } else if (g_BattleActorRecords[g_BattleActiveActorId].facing == 0) {
                                g_BattleActorRecords[actor].facing = 0x800;
                            } else {
                                g_BattleActorRecords[actor].facing = 0;
                            }
                        }
                    }
                    break;
                case BATTLE_LAYOUT_SIDE_ATTACK_1:
                case BATTLE_LAYOUT_SIDE_ATTACK_2:
                case BATTLE_LAYOUT_SIDE_ATTACK_3:
                    if (actor >= 4) {
                        if (actor == g_BattleActiveActorId) {
                            if (g_BattleActorRecords[g_BattleSpecialActorId].facing == 0) {
                                g_BattleActorRecords[actor].facing = 0x800;
                            } else {
                                g_BattleActorRecords[actor].facing = 0;
                            }
                        } else if (g_BattleActorRecords[g_BattleActiveActorId].facing == 0) {
                            g_BattleActorRecords[actor].facing = 0x800;
                        } else {
                            g_BattleActorRecords[actor].facing = 0;
                        }
                    }
                    break;
                case BATTLE_LAYOUT_PREEMPTIVE:
                    if (actor >= 4) {
                        g_BattleActorRecords[actor].facing = 0x800;
                    }
                    break;
                case BATTLE_LAYOUT_BACK_ATTACK:
                    if (actor < 4) {
                        g_BattleActorRecords[actor].facing = 0x800;
                    }
                }
                g_BattleActorRecords[actor].yaw = g_BattleActorRecords[actor].facing;
                break;
            case BATTLE_MOTION_START_ENEMY_SCRIPT:
                animStatus->targetActor = actor + (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->divisor = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                if (animStatus->targetActor < 0xA && animStatus->targetActor >= 4) {
                    if ((g_BattleActorStates[actor].flags & 4) == 0) {
                        if ((*(unsigned char *)&g_BattleActorRecords[animStatus->targetActor].drawFlags & 0x80) != 0) {
                            g_BattleActorRecords[animStatus->targetActor].facing = g_BattleActorRecords[actor].facing;
                            g_BattleActorRecords[animStatus->targetActor].yaw = g_BattleActorRecords[animStatus->targetActor].facing;
                            g_BattleActorRecords[animStatus->targetActor].scriptId = animStatus->divisor;
                            g_BattleActorRecords[animStatus->targetActor].stateFlags = g_BattleActorRecords[animStatus->targetActor].stateFlags | 1;
                            g_BattleActorRecords[animStatus->targetActor].isDirty = 0;
                            g_BattleActorStates[animStatus->targetActor].flags = g_BattleActorStates[animStatus->targetActor].flags | 4;
                        }
                    } else if ((g_BattleActorStates[actor].flags & 8) != 0) {
                        if (g_BattleSequenceStepBusy != 0) {
                            if (g_BattleHitReactionActor == actor && (*(unsigned char *)&g_BattleActorRecords[animStatus->targetActor].drawFlags & 0x80) != 0) {
                                g_BattleActorRecords[animStatus->targetActor].scriptId = animStatus->divisor;
                                g_BattleActorRecords[animStatus->targetActor].stateFlags = g_BattleActorRecords[animStatus->targetActor].stateFlags | 1;
                                g_BattleActorRecords[animStatus->targetActor].isDirty = 0;
                                g_BattleActorStates[animStatus->targetActor].flags = g_BattleActorStates[animStatus->targetActor].flags | 4;
                            }
                        } else if ((*(unsigned char *)&g_BattleActorRecords[animStatus->targetActor].drawFlags & 0x80) != 0) {
                            g_BattleActorRecords[animStatus->targetActor].scriptId = animStatus->divisor;
                            g_BattleActorRecords[animStatus->targetActor].stateFlags = g_BattleActorRecords[animStatus->targetActor].stateFlags | 1;
                            g_BattleActorRecords[animStatus->targetActor].isDirty = 0;
                            g_BattleActorStates[animStatus->targetActor].flags = g_BattleActorStates[animStatus->targetActor].flags | 4;
                        }
                    }
                }
                break;
            case BATTLE_MOTION_RESET_YAW:
                g_BattleActorRecords[actor].yaw = g_BattleActorRecords[actor].facing;
                break;
            case BATTLE_MOTION_SET_HOME_POS:
                g_BattleActorSpawnPos[g_BattleActiveActorId].posX = g_BattleActorRecords[g_BattleActiveActorId].posX;
                g_BattleActorSpawnPos[g_BattleActiveActorId].posZ = g_BattleActorRecords[g_BattleActiveActorId].posZ;
                break;
            case BATTLE_MOTION_TRANSLATE_Y:
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->divisor = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_0 = animStatus->distance / animStatus->divisor;
                animStatus->taskSlot = Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskTranslateY);
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].actorId = actor;
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].battleActorIndex = *(short *)&g_BattleVmReg_0;
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].sinkCountdown = animStatus->divisor;
                break;
            case BATTLE_MOTION_FACE_TARGETS_CENTROID:
                Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, &animStatus->targetX);
                g_BattleActorRecords[actor].yaw = ArcTangentFixed(animStatus->targetX - g_BattleActorRecords[actor].posX, animStatus->targetZ - g_BattleActorRecords[actor].posZ) + 0x800;
                break;
            case BATTLE_MOTION_FACE_TARGET:
                if (g_BattleSequenceStepBusy != 0) {
                    switch (g_BattleLayout) {
                    case BATTLE_LAYOUT_SIDE_ATTACK_1:
                    case BATTLE_LAYOUT_SIDE_ATTACK_2:
                    case BATTLE_LAYOUT_SIDE_ATTACK_3:
                    case BATTLE_LAYOUT_SIDE_ATTACK_4:
                        for (loopIndex = 4; loopIndex < 10; loopIndex++) {
                            if (g_BattleActorRecords[actor].facing == 0) {
                                g_BattleActorRecords[loopIndex].facing = 0x800;
                            } else {
                                g_BattleActorRecords[loopIndex].facing = 0;
                            }
                        }
                        break;
                    case BATTLE_LAYOUT_PINCER_ATTACK:
                        if ((g_BattleScene.targetMaskTable[0] & g_BattleEffectScratch.targetMask) != 0) {
                            g_BattleActorRecords[actor].yaw = 0;
                        }
                        if ((g_BattleScene.targetMaskTable[2] & g_BattleEffectScratch.targetMask) != 0) {
                            g_BattleActorRecords[actor].yaw = 0x800 /* 180 deg */;
                        }
                    }
                } else if (g_BattleActorRecords[actor].effectId != 0x15 || g_BattleActorRecords[actor].effectCategory != BATTLE_EFFECT_CATEGORY_ENEMY_SKILL) {
                    if (g_BattleActiveActorId != g_BattleSavedSpecialActorId) {
                        g_BattleActorRecords[actor].yaw = ArcTangentFixed(g_BattleActorRecords[g_BattleSavedSpecialActorId].posX - g_BattleActorRecords[actor].posX, g_BattleActorRecords[g_BattleSavedSpecialActorId].posZ - g_BattleActorRecords[actor].posZ) + 0x800;
                        g_BattleActorStates[g_BattleSavedSpecialActorId].yaw = ArcTangentFixed(g_BattleActorRecords[g_BattleSavedSpecialActorId].posX - g_BattleActorRecords[actor].posX, g_BattleActorRecords[g_BattleSavedSpecialActorId].posZ - g_BattleActorRecords[actor].posZ);
                    }
                }
                break;
            case BATTLE_MOTION_PLACE_IN_FRONT_OF_TARGET_ALIAS:
            case BATTLE_MOTION_PLACE_IN_FRONT_OF_TARGET:
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->posY = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->angle = 0x1000 - (g_BattleActorStates[g_BattleSavedSpecialActorId].yaw + 0x800);
                animStatus->secondarySlot = (animStatus->distance * g_BattleActorRecords[actor].scale >> 12) + (g_BattleActorRecords[g_BattleSavedSpecialActorId].radius * g_BattleActorRecords[g_BattleSavedSpecialActorId].scale >> 12);
                g_BattleActorRecords[actor].posX = g_BattleActorRecords[g_BattleSavedSpecialActorId].posX - (SinFixed(animStatus->angle) * animStatus->secondarySlot >> 12);
                g_BattleActorRecords[actor].posZ = g_BattleActorRecords[g_BattleSavedSpecialActorId].posZ + (CosFixed(animStatus->angle) * animStatus->secondarySlot >> 12);
                g_BattleActorRecords[actor].posY = animStatus->posY;
                break;
            case BATTLE_MOTION_RETURN_TO_SPAWN_POS:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_1 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SpawnReturnToSpawnPosTask((short)actor, *(short *)&g_BattleVmReg_1, *(short *)&g_BattleVmReg_0);
                break;
            case BATTLE_MOTION_MOVE_TO_TARGET_ARG_FROM_ACTOR:
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->divisor = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_0 = g_BattleActorStates[actor].bindIndex;
                Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskApproachTarget, (short)animStatus->divisor, &g_BattleActorRecords[g_BattleSavedSpecialActorId].posX);
                break;
            case BATTLE_MOTION_MOVE_TO_TARGET_DELAYED:
                g_BattleApproachHoldFrames = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_0 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->divisor = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                if (g_BattleSequenceStepBusy == 0) {
                    Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskApproachTarget, (short)animStatus->divisor, &g_BattleActorRecords[g_BattleSavedSpecialActorId].posX);
                } else {
                    Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, &animStatus->targetX);
                    Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskApproachTarget, (short)animStatus->divisor, &animStatus->targetX);
                }
                break;
            case BATTLE_MOTION_MOVE_TO_TARGET:
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_0 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->divisor = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                if (g_BattleSequenceStepBusy == 0) {
                    Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskApproachTarget, (short)animStatus->divisor, &g_BattleActorRecords[g_BattleSavedSpecialActorId].posX);
                } else {
                    Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, &animStatus->targetX);
                    Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskApproachTarget, (short)animStatus->divisor, &animStatus->targetX);
                }
                break;
            case BATTLE_MOTION_TRANSLATE_XZ:
                g_BattleVmReg_0 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_1 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_2 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_3 = (short)Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskTranslateXZ);
                g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_3].actorId = actor;
                g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_3].battleActorIndex = *(short *)&g_BattleVmReg_0;
                g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_3].velocityX = *(short *)&g_BattleVmReg_1;
                g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_3].sinkCountdown = *(short *)&g_BattleVmReg_2;
                break;
            case BATTLE_MOTION_DEATH_SINK:
                g_BattleActorRecords[actor].isDirty = 1;
                Battle_ResetActorScaleAxes((short)actor);
                animStatus->taskSlot = Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskDeathSink);
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].actorId = actor;
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].battleActorIndex = 0;
                g_BattleOverlayTaskSlotBuffer[animStatus->taskSlot].sfxPan = 1;
                break;
            case BATTLE_MOTION_HIDE_PERMANENTLY:
                g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags & ~0x80;
                g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags | 4;
                break;
            case BATTLE_MOTION_STATUS_POSE_FROM_HUD:
                Battle_SpawnPoseTaskFromHud((short)actor, 1);
                break;
            case BATTLE_MOTION_TRANSLATE_Z_BY_LAYOUT:
                if (g_BattleAnimTextureSetLoaded == 0) {
                    Battle_PlaySfxPannedToActor(0x185, -1, 0x40);
                    g_BattleAnimTextureSetLoaded = 1;
                }
                g_BattleVmReg_1 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_2 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_3 = (short)Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskTranslateXZ);
                g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_3].actorId = actor;
                g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_3].battleActorIndex = 0;
                g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_3].sinkCountdown = *(short *)&g_BattleVmReg_2;
                switch (g_BattleLayout) {
                case BATTLE_LAYOUT_NORMAL:
                case BATTLE_LAYOUT_PREEMPTIVE:
                case BATTLE_LAYOUT_FRONT_ROW_LOCKED:
                    g_BattleVmReg_4 = g_BattleVmReg_1;
                    break;
                case BATTLE_LAYOUT_BACK_ATTACK:
                    g_BattleVmReg_4 = -g_BattleVmReg_1;
                    break;
                case BATTLE_LAYOUT_SIDE_ATTACK_1:
                case BATTLE_LAYOUT_SIDE_ATTACK_2:
                case BATTLE_LAYOUT_SIDE_ATTACK_3:
                case BATTLE_LAYOUT_SIDE_ATTACK_4:
                    if (g_BattleActorRecords[actor].facing == 0) {
                        g_BattleVmReg_4 = g_BattleVmReg_1;
                    } else {
                        g_BattleVmReg_4 = -g_BattleVmReg_1;
                    }
                    break;
                case BATTLE_LAYOUT_PINCER_ATTACK:
                    if (g_BattleScene.occupiedEnemySideCount != 0) {
                        g_BattleActorRecords[actor].yaw = 0;
                        g_BattleVmReg_4 = g_BattleVmReg_1;
                    } else {
                        g_BattleActorRecords[actor].yaw = 0x800 /* 180 deg */;
                        g_BattleVmReg_4 = -g_BattleVmReg_1;
                    }
                }
                g_BattleOverlayTaskSlotBuffer[g_BattleVmReg_3].velocityX = *(short *)&g_BattleVmReg_4;
                break;
            case BATTLE_MOTION_LOCK_FACING_PLACE_IN_FRONT:
                g_BattleTargetFacingLocked = 1;
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->posY = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                Battle_PlaceActorInFrontOfActor((char)actor, g_BattleSavedSpecialActorId, (short)animStatus->distance, (short)animStatus->posY);
                break;
            case BATTLE_MOTION_SNAP_OTHER_ACTOR_TO_SPAWN:
                g_BattleActorRecords[g_BattleSequenceActorIndex].posX = g_BattleActorSpawnPos[g_BattleSequenceActorIndex].posX;
                g_BattleActorRecords[g_BattleSequenceActorIndex].posY = g_BattleActorSpawnPos[g_BattleSequenceActorIndex].posY;
                g_BattleActorRecords[g_BattleSequenceActorIndex].posZ = g_BattleActorSpawnPos[g_BattleSequenceActorIndex].posZ;
                break;
            case BATTLE_MOTION_TAKE_SPAWN_PLACE_OTHER_IN_FRONT:
                g_BattleVmReg_0 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->distance = 0;
                animStatus->posY = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleActorRecords[actor].posX = g_BattleActorSpawnPos[g_BattleSequenceActorIndex].posX;
                g_BattleActorRecords[actor].posY = g_BattleActorSpawnPos[g_BattleSequenceActorIndex].posY;
                g_BattleActorRecords[actor].posZ = g_BattleActorSpawnPos[g_BattleSequenceActorIndex].posZ;
                g_BattleActorRecords[actor].yaw = g_BattleActorRecords[g_BattleSequenceActorIndex].facing;
                animStatus->distance = *(short *)&g_BattleVmReg_0;
                animStatus->posY = 0;
                animStatus->angle = 0x1000 - (g_BattleActorRecords[g_BattleActiveActorId].yaw + 0x800);
                animStatus->secondarySlot = (animStatus->distance * g_BattleActorRecords[g_BattleSequenceActorIndex].scale >> 12) + (g_BattleActorRecords[actor].radius * g_BattleActorRecords[actor].scale >> 12);
                g_BattleActorRecords[g_BattleSequenceActorIndex].posX = g_BattleActorRecords[actor].posX - (SinFixed(animStatus->angle) * animStatus->secondarySlot >> 12);
                g_BattleActorRecords[g_BattleSequenceActorIndex].posZ = g_BattleActorRecords[actor].posZ + (CosFixed(animStatus->angle) * animStatus->secondarySlot >> 12);
                g_BattleActorRecords[g_BattleSequenceActorIndex].posY = animStatus->posY;
                break;
            case BATTLE_MOTION_MOVE_TO_CENTROID_DOUBLED:
                animStatus->distance = 0;
                g_BattleVmReg_0 = 0;
                animStatus->divisor = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, &animStatus->targetX);
                animStatus->targetX = animStatus->targetX << 1;
                animStatus->targetZ = animStatus->targetZ << 1;
                Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskApproachTarget, (short)animStatus->divisor, &animStatus->targetX);
                break;
            case BATTLE_MOTION_FADE_OUT_SFX:
                SoundDispatchCommand(SOUND_SFX_CH1_VOLUME_TRANS, *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos)++), 0, 0, 0, 0, 0, 0, 0);
                break;
            case BATTLE_MOTION_SPAWN_EFFECT_TRAIL:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_1 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_2 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_6 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_7 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_3 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_4 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_5 = (g_BattleVmReg_2 & 0xFF) << 0xA | (g_BattleVmReg_6 & 0xFF) << 5 | g_BattleVmReg_7 & 0xFF;
                if (g_BattleVmReg_0 == 0xFF) {
                    Battle_CreateEffectTrailTask(actor, 0, g_BattleVmReg_1, g_BattleVmReg_5, g_BattleVmReg_3, g_BattleVmReg_4);
                } else {
                    Battle_CreateEffectTrailTask(actor, g_BattleVmReg_0, g_BattleVmReg_1, g_BattleVmReg_5, g_BattleVmReg_3, g_BattleVmReg_4);
                }
                break;
            case BATTLE_MOTION_MOVE_LIFT_THEN_TRAVEL:
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_1 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_0 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_2 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_3 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->divisor = *(short *)&g_BattleVmReg_3;
                Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskLiftThenTravel, (short)animStatus->divisor, &g_BattleActorRecords[g_BattleSavedSpecialActorId].posX);
                break;
            case BATTLE_MOTION_MOVE_TRAVEL_THEN_DROP:
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_1 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_0 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_2 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_3 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->divisor = *(short *)&g_BattleVmReg_3;
                Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskTravelThenDrop, (short)animStatus->divisor, &g_BattleActorRecords[g_BattleSavedSpecialActorId].posX);
                break;
            case BATTLE_MOTION_SET_VISIBLE:
                g_BattleVmReg_2 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                if (g_BattleVmReg_2 == 0) {
                    g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags | 4;
                } else {
                    g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags & 0xFFFFFFFB;
                }
                break;
            case BATTLE_MOTION_SWAP_ENEMY_MODEL:
                g_BattleVmReg_3 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                ((struct BattleFormationEntry *)&g_BattleEnemySlots)[actor - 4].slotId = g_BattleVmReg_3 + 3;
                Battle_BindEnemyActorToModel((char)actor);
                g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 1;
                break;
            case BATTLE_MOTION_HOP_TO_TARGET:
                animStatus->distance = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                switch (g_BattleVmReg_0) {
                case BATTLE_HOP_KIND_DEFAULT:
                case BATTLE_HOP_KIND_ARC_1:
                case BATTLE_HOP_KIND_ARC_2:
                case BATTLE_HOP_KIND_ARC_3:
                    animStatus->divisor = 5;
                    break;
                case BATTLE_HOP_KIND_ARC_4:
                case BATTLE_HOP_KIND_ARC_5:
                case BATTLE_HOP_KIND_ARC_6:
                case BATTLE_HOP_KIND_ARC_7:
                    animStatus->divisor = 8;
                }
                if (g_BattleSequenceStepBusy == 0) {
                    Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskHopArc, (short)animStatus->divisor, &g_BattleActorRecords[g_BattleSavedSpecialActorId].posX);
                } else {
                    Battle_GetActorMaskCentroid(g_BattleEffectScratch.targetMask, &animStatus->targetX);
                    Battle_SpawnActorMoveToTargetTask((short)actor, animStatus->scriptPtr, (short)animStatus->distance, Battle_OverlayTaskHopArc, (short)animStatus->divisor, &animStatus->targetX);
                }
                break;
            case BATTLE_MOTION_WAIT_FRAMES:
                if (g_BattleActorRecords[actor].waitFrames != 0) {
                    (g_BattleActorRecords[actor].waitFrames)--;
                    (g_BattleActorRecords[actor].scriptPos)--;
                    animStatus->running = 0;
                    return;
                }
                break;
            case BATTLE_MOTION_SET_WAIT_FRAMES:
                g_BattleActorRecords[actor].waitFrames = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
                (g_BattleActorRecords[actor].scriptPos)++;
                break;
            case BATTLE_MOTION_LOAD_SHARED_WAIT_FRAMES:
                g_BattleActorRecords[actor].waitFrames = g_BattleScriptWaitFrames;
                break;
            case BATTLE_MOTION_SNAP_TO_SPAWN_POS:
                Battle_SnapActorToSpawnPos((char)actor);
                break;
            case BATTLE_MOTION_DELAYED_EFFECT_PAIR:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_1 = (short)Battle_AllocateResourceSlot(Battle_MainTaskSpawnDelayedEffectPair);
                BATTLE_SLOT_VIEW(BattleHitReactionSlot, &g_BattleTaskSlotBuffer.mainSlots[g_BattleVmReg_1])->actor = (short)*(volatile char *)&g_BattleSpecialActorId;
                loopIndex = (short)Battle_FindActorInEffectOrder(g_BattleSpecialActorId);
                BATTLE_SLOT_VIEW(BattleHitReactionSlot, &g_BattleTaskSlotBuffer.mainSlots[g_BattleVmReg_1])->targetParam = g_BattleHitReactionRecords[loopIndex].targetParam;
                BATTLE_SLOT_VIEW(BattleHitReactionSlot, &g_BattleTaskSlotBuffer.mainSlots[g_BattleVmReg_1])->damage = g_BattleHitReactionRecords[loopIndex].damage;
                BATTLE_SLOT_VIEW(BattleHitReactionSlot, &g_BattleTaskSlotBuffer.mainSlots[g_BattleVmReg_1])->actorWord = g_BattleHitReactionRecords[loopIndex].reactionParam;
                BATTLE_SLOT_VIEW(BattleHitReactionSlot, &g_BattleTaskSlotBuffer.mainSlots[g_BattleVmReg_1])->commitByte = g_BattleHitReactionRecords[loopIndex].hitKind;
                BATTLE_SLOT_VIEW(BattleHitReactionSlot, &g_BattleTaskSlotBuffer.mainSlots[g_BattleVmReg_1])->delay = *(short *)&g_BattleVmReg_0;
                break;
            case BATTLE_MOTION_SPAWN_HIT_REACTION:
                g_BattleHitReactionNoNumbers = 0;
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SpawnHitReactionTask(g_BattleEffectScratch.targetMask, g_BattleVmReg_0, 1);
                break;
            case BATTLE_MOTION_SPAWN_SCRIPT_TIMER:
                g_BattleHitReactionNoNumbers = 0;
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_1 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SpawnScriptTimerTask(g_BattleVmReg_0, g_BattleVmReg_1);
                break;
            case BATTLE_MOTION_SPAWN_HIT_REACTION_NO_NUMBERS:
                g_BattleHitReactionNoNumbers = 1;
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                if (g_BattleScene.actors[g_BattleActiveActorId].unitIndex != 2 || (g_BattleVmReg_0 != 8 || (g_BattleScene.actors[g_BattleSpecialActorId].savedStatus & BATTLE_STATUS_FROG) == 0)) {
                    Battle_SpawnHitReactionTask(g_BattleEffectScratch.targetMask, g_BattleVmReg_0, 1);
                }
                break;
            case BATTLE_MOTION_STATUS_POSE_FROM_HUD_IF_FLAGGED:
                if (g_BattleTaskSlotBuffer.isBusy == 0) {
                    if ((g_BattleActiveTargetMask & 1 << actor) != 0) {
                        Battle_SpawnPoseTaskFromHud((short)actor, 1);
                    }
                } else if ((g_BattleActorStates[actor].flags & 0x10) != 0) {
                    Battle_SpawnPoseTaskFromHud((short)actor, 1);
                }
                break;
            case BATTLE_MOTION_SKIP_BLOCK_IF_ENEMY:
                if (actor >= 4) {
                    while (*(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos)++) != 0xCD) {
                    }
                }
                break;
            case BATTLE_MOTION_SKIP_BLOCK_IF_SMALL:
                if ((g_BattleActorStates[actor].status & BATTLE_STATUS_SMALL) != 0) {
                    while (*(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos)++) != 0xB2) {
                    }
                }
                break;
            case BATTLE_MOTION_SET_SCRIPT_FLAG:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_1 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                *(char *)((char *)&g_BattleScriptFlags + g_BattleVmReg_0) = *(char *)&g_BattleVmReg_1;
                Battle_ApplyVmFlagArrayBit0();
                break;
            case BATTLE_MOTION_LOOP_BACK:
                g_BattleActorRecords[actor].scriptPos = 0;
                while (*(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos)++) != 0xC9) {
                }
                break;
            case BATTLE_MOTION_LOOP_BACK_IF_EFFECT_LOADING:
                if (g_EngineIsEffectLoading != 0) {
                    g_BattleActorRecords[actor].scriptPos = 0;
                    while (*(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos)++) != 0xC9) {
                    }
                }
                break;
            case BATTLE_MOTION_STEP_FORWARD_LONG:
                g_BattleVmReg_0 = g_BattleActorRecords[actor].yaw;
                g_BattleActorRecords[actor].posX -= (short)(SinFixed(g_BattleVmReg_0) * 0x204 >> 12);
                g_BattleActorRecords[actor].posZ -= (short)(CosFixed(g_BattleVmReg_0) * 0x204 >> 12);
                break;
            case BATTLE_MOTION_STEP_BACK_LONG:
                g_BattleVmReg_0 = g_BattleActorRecords[actor].yaw + 0x800;
                g_BattleActorRecords[actor].posX -= (short)(SinFixed(g_BattleVmReg_0) * 0x204 >> 12);
                g_BattleActorRecords[actor].posZ -= (short)(CosFixed(g_BattleVmReg_0) * 0x204 >> 12);
                break;
            case BATTLE_MOTION_STEP_FORWARD_SHORT:
                g_BattleVmReg_0 = g_BattleActorRecords[actor].yaw;
                g_BattleActorRecords[actor].posX -= (short)(SinFixed(g_BattleVmReg_0) * 0x102 >> 12);
                g_BattleActorRecords[actor].posZ -= (short)(CosFixed(g_BattleVmReg_0) * 0x102 >> 12);
                break;
            case BATTLE_MOTION_STEP_BACK_SHORT:
                g_BattleVmReg_0 = g_BattleActorRecords[actor].yaw + 0x800;
                g_BattleActorRecords[actor].posX -= (short)(SinFixed(g_BattleVmReg_0) * 0x102 >> 12);
                g_BattleActorRecords[actor].posZ -= (short)(CosFixed(g_BattleVmReg_0) * 0x102 >> 12);
                break;
            case BATTLE_MOTION_DELAYED_HIT_MARK:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_1 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->secondarySlot = Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskDelayedHudPulse);
                BATTLE_SLOT_VIEW(BattleEffectDelaySlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->delay = *(short *)&g_BattleVmReg_0;
                BATTLE_SLOT_VIEW(BattleEffectDelaySlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->value = *(short *)&g_BattleVmReg_1;
                BATTLE_SLOT_VIEW(BattleEffectDelaySlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->actor = actor;
                break;
            case BATTLE_MOTION_DELAYED_EVENT:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_1 = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->secondarySlot = Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskDelayedEvent);
                BATTLE_SLOT_VIEW(BattleEffectDelaySlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->delay = *(short *)&g_BattleVmReg_0;
                BATTLE_SLOT_VIEW(BattleEffectDelaySlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->secondarySlot])->value = *(short *)&g_BattleVmReg_1;
                break;
            case BATTLE_MOTION_NOP:
                break;
            case BATTLE_MOTION_SYNC_WAIT_TARGET_IDLE:
                if (actor == g_BattleSpecialActorId) {
                    if (g_BattleSecondaryTaskActiveCount == 0) {
                        g_BattleActorRecords[actor].isDirty = 1;
                        return;
                    }
                    (g_BattleActorRecords[actor].scriptPos)--;
                    return;
                }
                if (*(unsigned char *)&g_BattleActorRecords[g_BattleSpecialActorId].isDirty == 1 && g_BattleSecondaryTaskActiveCount == 0) {
                    g_BattleActorRecords[actor].isDirty = 1;
                    return;
                }
                (g_BattleActorRecords[actor].scriptPos)--;
                return;
            case BATTLE_MOTION_END_SCRIPT:
            case BATTLE_MOTION_END_SCRIPT_ALIAS:
                g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & 0xFFFFFFFB;
                g_BattleActorRecords[actor].isDirty = 1;
                g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & 0xFFFFFFF7;
                *(char *)&g_BattleActorIdleScripts[actor] = g_BattleScene.actors[actor].scriptId;
                g_BattleActorRecords[actor].scriptId = (short)*(volatile char *)&g_BattleActorIdleScripts[actor];
                animStatus->scriptPtr = ((char **)scriptTable)[g_BattleActorRecords[actor].scriptId];
                Battle_RestartActorScript((short)actor);
                break;
            case BATTLE_MOTION_HALT_IDLE:
                g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & 0xFFFFFFFB;
                g_BattleActorRecords[actor].isDirty = 1;
                g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags & ~8;
                (g_BattleActorRecords[actor].scriptPos)--;
                return;
            case BATTLE_MOTION_ATTACH_TARGET_TO_BONE_FLIPPED:
                g_BattleActorStates[g_BattleSavedSpecialActorId].flags = g_BattleActorStates[g_BattleSavedSpecialActorId].flags | 1;
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SetNodeParentMatrix(&g_BattleActorRecords[actor].nodes[g_BattleVmReg_0], &g_BattleActorRecords[g_BattleSavedSpecialActorId].root);
                g_BattleActorRecords[g_BattleSavedSpecialActorId].yaw = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].rotX = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posZ = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posY = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posX = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].rotZ = 0x800 /* 180 deg */;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].drawFlags = *(unsigned char *)&g_BattleActorRecords[g_BattleSavedSpecialActorId].drawFlags | 0x10;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].attachActor = (int)*(volatile short *)&actor;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].attachBone = g_BattleVmReg_0;
                break;
            case BATTLE_MOTION_ATTACH_TARGET_TO_BONE:
                g_BattleActorStates[g_BattleSavedSpecialActorId].flags = g_BattleActorStates[g_BattleSavedSpecialActorId].flags | 1;
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SetNodeParentMatrix(&g_BattleActorRecords[actor].nodes[g_BattleVmReg_0], &g_BattleActorRecords[g_BattleSavedSpecialActorId].root);
                g_BattleActorRecords[g_BattleSavedSpecialActorId].rotZ = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].yaw = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].rotX = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posZ = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posY = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posX = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].drawFlags = *(unsigned char *)&g_BattleActorRecords[g_BattleSavedSpecialActorId].drawFlags | 0x10;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].attachActor = (int)*(volatile short *)&actor;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].attachBone = g_BattleVmReg_0;
                break;
            case BATTLE_MOTION_DETACH_TARGET:
                g_BattleActorStates[g_BattleSavedSpecialActorId].flags = g_BattleActorStates[g_BattleSavedSpecialActorId].flags & 0xFFFFFFFE;
                Battle_SetNodeParentMatrix(&g_BattleStageNodes[0], &g_BattleActorRecords[g_BattleSavedSpecialActorId].root);
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posX = g_BattleActorSpawnPos[g_BattleSavedSpecialActorId].posX;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posY = g_BattleActorSpawnPos[g_BattleSavedSpecialActorId].posY;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].posZ = g_BattleActorSpawnPos[g_BattleSavedSpecialActorId].posZ;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].rotZ = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].yaw = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].rotX = 0;
                g_BattleActorRecords[g_BattleSavedSpecialActorId].drawFlags = *(unsigned char *)&g_BattleActorRecords[g_BattleSavedSpecialActorId].drawFlags & 0xFFFFFFEF;
                break;
            case BATTLE_MOTION_START_EFFECT_PROGRAM:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                switch (g_BattleVmReg_0) {
                case BATTLE_MOTION_EFFPROG_BEAT_RUSH:
                    Battle_MotionOpStartEffectProgram0(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                    break;
                case BATTLE_MOTION_EFFPROG_SOMERSAULT:
                    Battle_MotionOpStartEffectProgram1(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                    break;
                case BATTLE_MOTION_EFFPROG_WATERKICK:
                    Battle_MotionOpStartEffectProgram2(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                    break;
                case BATTLE_MOTION_EFFPROG_METEODRIVE:
                    Battle_MotionOpStartEffectProgram3(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                    break;
                case BATTLE_MOTION_EFFPROG_DOLPHIN_BLOW:
                    Battle_MotionOpStartEffectProgram4(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                    break;
                case BATTLE_MOTION_EFFPROG_METEOR_STRIKE:
                    Battle_MotionOpStartEffectProgram5(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                    break;
                case BATTLE_MOTION_EFFPROG_FINAL_HEAVEN:
                    Battle_MotionOpStartEffectProgram6(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                }
                break;
            case BATTLE_MOTION_SET_EFFECT_ID_CATEGORY_2:
                g_BattleActorRecords[actor].effectId = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleActorRecords[actor].effectCategory = BATTLE_EFFECT_CATEGORY_MAGIC;
                break;
            case BATTLE_MOTION_BONE_PARTICLES_PARTY_HEIGHT:
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->posY = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                statBlockPtr = Savemap_GetPartyStatBlock(actor);
                if (statBlockPtr != 0) {
                    animStatus->targetActor = g_BattleBoneParticleBurstParam[statBlockPtr->paramIndex & 0xF];
                    Battle_SpawnBoneParticleBurst(g_BattleSpecialActorId, actor, -1, animStatus->targetActor, animStatus->secondarySlot, animStatus->posY);
                }
                break;
            case BATTLE_MOTION_SET_PARTICLE_OFFSET:
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                (&g_BattleActorRecords[actor].burstOffsetA)[animStatus->secondarySlot] = (short)Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr) * g_BattleActorRecords[actor].scale >> 12;
                break;
            case BATTLE_MOTION_BONE_PARTICLES_PRIMARY:
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->posY = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SpawnBoneParticleBurstA(g_BattleSpecialActorId, actor, animStatus->secondarySlot, animStatus->posY);
                break;
            case BATTLE_MOTION_BONE_PARTICLES_SECONDARY:
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->posY = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SpawnBoneParticleBurstB(g_BattleSpecialActorId, actor, animStatus->secondarySlot, animStatus->posY);
                break;
            case BATTLE_MOTION_BONE_PARTICLES_AT_BONE:
                animStatus->divisor = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->targetActor = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->posY = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SpawnBoneParticleBurst(g_BattleSpecialActorId, actor, (&g_BattleActorRecords[actor].targetBone)[animStatus->divisor], animStatus->targetActor, animStatus->secondarySlot, animStatus->posY);
                break;
            case BATTLE_MOTION_BONE_PARTICLES:
                animStatus->targetActor = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->posY = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SpawnBoneParticleBurst(g_BattleSpecialActorId, actor, -1, animStatus->targetActor, animStatus->secondarySlot, animStatus->posY);
                break;
            case BATTLE_MOTION_SPAWN_SCALE_RAMP_EMITTER:
                Battle_SpawnActorScaleRampTask(actor);
                break;
            case BATTLE_MOTION_SPAWN_SPRITE_AT_POINT:
                animStatus->targetX = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->targetY = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->targetZ = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->posY = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                animStatus->targetActor = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                /* Retail caller prototype (int, int, short, short, short) -> void preserved for /Od argument promotion on `Battle_SpawnPositionedEffectTask`. */
                ((void(__cdecl *)(int, int, short, short, short))Battle_SpawnPositionedEffectTask)(actor, (int)&animStatus->targetX, (short)animStatus->secondarySlot, (short)animStatus->posY, (short)animStatus->targetActor);
                break;
            case BATTLE_MOTION_FIRE_EVENT_1F0_FLASH_2:
                loopIndex = (short)Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskFireEventAndRetire);
                g_BattleTaskSlotBuffer.secondarySlots[loopIndex].frameCounter = 2;
                BATTLE_SLOT_VIEW(BattleEffectEventSlot, &g_BattleTaskSlotBuffer.secondarySlots[loopIndex])->event = 0x1F0;
                break;
            case BATTLE_MOTION_FIRE_EVENT_1E9_FLASH_3:
                loopIndex = (short)Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskFireEventAndRetire);
                g_BattleTaskSlotBuffer.secondarySlots[loopIndex].frameCounter = 3;
                BATTLE_SLOT_VIEW(BattleEffectEventSlot, &g_BattleTaskSlotBuffer.secondarySlots[loopIndex])->event = 0x1E9;
                break;
            case BATTLE_MOTION_FIRE_EVENT_0D_FLASH_0:
                loopIndex = (short)Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskFireEventAndRetire);
                g_BattleTaskSlotBuffer.secondarySlots[loopIndex].frameCounter = 0;
                BATTLE_SLOT_VIEW(BattleEffectEventSlot, &g_BattleTaskSlotBuffer.secondarySlots[loopIndex])->event = 0xD;
                break;
            case BATTLE_MOTION_FIRE_EVENT_24_FLASH_1:
                loopIndex = (short)Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskFireEventAndRetire);
                g_BattleTaskSlotBuffer.secondarySlots[loopIndex].frameCounter = 1;
                BATTLE_SLOT_VIEW(BattleEffectEventSlot, &g_BattleTaskSlotBuffer.secondarySlots[loopIndex])->event = 0x24;
                break;
            case BATTLE_MOTION_SET_PALETTE_COLOR:
                animStatus->scriptArg = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
                (g_BattleActorRecords[actor].scriptPos)++;
                switch (actor) {
                case BATTLE_ENEMY_SLOT_2:
                    g_BattleVmReg_0 = 0;
                    g_BattleVmReg_1 = animStatus->scriptArg;
                    break;
                case BATTLE_ENEMY_SLOT_3:
                    g_BattleVmReg_0 = 0;
                    g_BattleVmReg_1 = animStatus->scriptArg + 8;
                    break;
                case BATTLE_ENEMY_SLOT_4:
                    g_BattleVmReg_0 = 1;
                    g_BattleVmReg_1 = animStatus->scriptArg;
                    break;
                case BATTLE_ENEMY_SLOT_5:
                    g_BattleVmReg_0 = 1;
                    g_BattleVmReg_1 = animStatus->scriptArg + 8;
                }
                graphics = GetGraphicsDriverObject(GetEngineContext());
                entSlot = 4;
                opWord = Battle_ScriptFetchWord((short)actor, animStatus->scriptPtr);
                palettePtr = (BattleRgbaColor *)&rgba;
                palettePtr->colorB = (opWord & 0x1F) << 3;
                palettePtr->colorG = (opWord & 0x3E0) >> 2;
                palettePtr->colorR = (opWord & 0x7C00) >> 7;
                palettePtr->colorA = 0xFF;
                paletteIdx = g_BattleVmReg_1;
                zero = 0;
                g_BattleAnimTextureSetCount = 0;
                g_BattleAnimTextureSetHandles[0] = 0;
                g_BattleAnimTextureSetHandles[1] = 0;
                g_BattleAnimTextureSetRecords[0] = 0;
                g_BattleAnimTextureSetRecords[1] = 0;
                Gfx_ForEachModelPolygonSet(zero, (GfxPolygonSetFn)Battle_CollectSkeletonTextureSets, (PolygonModel *)(g_BattleActorRecords[entSlot].model->modelParts));
                if (actor < 8) {
                    texHandle = (GfxTextureSurface *)(g_BattleAnimTextureSetHandles[0]);
                } else {
                    texHandle = (GfxTextureSurface *)(g_BattleAnimTextureSetHandles[1]);
                }
                if (texHandle != 0) {
                    Gfx_UploadTexturePalette(0, 1, (int)palettePtr, paletteIdx, (int)texHandle);
                    graphics->paletteChanged(paletteIdx, 1, paletteIdx, (void *)texHandle->palette, texHandle);
                }
                break;
            case BATTLE_MOTION_LOCK_TARGET_FACING:
                g_BattleTargetFacingLocked = 1;
                break;
            case BATTLE_MOTION_UNLOCK_TARGET_FACING:
                g_BattleTargetFacingLocked = 0;
                break;
            case BATTLE_MOTION_SET_SHARED_WAIT_FRAMES:
                g_BattleScriptWaitFrames = animStatus->scriptPtr[g_BattleActorRecords[actor].scriptPos];
                (g_BattleActorRecords[actor].scriptPos)++;
                break;
            case BATTLE_MOTION_FADE_OUT_AND_HIDE:
                g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 2;
                break;
            case BATTLE_MOTION_FADE_IN:
                g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 4;
                break;
            case BATTLE_MOTION_PRELOAD_EFFECT:
                Battle_SelectEffectOverlayProgram((char)actor);
                break;
            case BATTLE_MOTION_SET_TRANSFORM_MODEL:
                g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 0x20;
                g_BattleActorStates[actor].status = g_BattleScene.actors[actor].savedStatus;
                g_BattleActorRecords[actor].yaw = g_BattleActorRecords[actor].facing;
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                if (animStatus->secondarySlot == 0xA) {
                    statBlockPtr = Savemap_GetPartyStatBlock(actor);
                    if (statBlockPtr != 0) {
                        animStatus->posY = (unsigned short)statBlockPtr->paramIndex & 0xF0;
                        switch (animStatus->posY) {
                        case BATTLE_WEAPON_MODEL_PRIMARY:
                            animStatus->targetActor = 4;
                            break;
                        case BATTLE_WEAPON_MODEL_ALT_10:
                            animStatus->targetActor = 5;
                            break;
                        case BATTLE_WEAPON_MODEL_ALT_20:
                            animStatus->targetActor = 6;
                            break;
                        default:
                            animStatus->targetActor = 4;
                        }
                    } else {
                        animStatus->targetActor = 4;
                    }
                    animationIdx = animStatus->targetActor;
                } else {
                    animationIdx = animStatus->secondarySlot;
                }
                g_VincentLimitModelIndex = animationIdx;
                break;
            case BATTLE_MOTION_TRANSFORM_MODEL:
                animStatus->targetActor = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                if (g_VincentLimitModelIndex >= 4) {
                    Battle_LoadModelIntoBankSlot(0, 0, actor, g_VincentModelFilenames[g_VincentLimitModelIndex]);
                } else {
                    Battle_LoadModelIntoBankSlot(0, 1, actor, g_VincentModelFilenames[g_VincentLimitModelIndex]);
                }
                if (g_BattleModelBankSlots[actor] != 0) {
                    g_BattleActorRecords[actor].model = (BattleActorModel *)g_BattleModelBankSlots[actor];
                }
                g_BattleActorRecords[actor].animId = 0;
                g_BattleActorRecords[actor].nextAnimBank = 0;
                g_BattleActorRecords[actor].animBank = 0;
                g_BattleActorRecords[actor].animFrame = 0;
                g_BattleActorRecords[actor].useSecondAngleTable = 0;
                Battle_SetPartyActorRootParent(actor);
                Battle_TouchPartyStatBlock(actor);
                windowBase = *(int *)g_BattleModelBankSlots[actor];
                for (loopIndex = 0; loopIndex < 74; loopIndex++) {
                    *(int *)((char *)windowBase + loopIndex * 4 + 0x68) += *(int *)g_BattleModelBankSlots[actor];
                }
                Battle_BindPartyActorToModel((char)actor);
                g_BattleActorRecords[actor].scriptId = animStatus->targetActor;
                g_BattleScene.actors[actor].scriptId = *(char *)&g_BattleActorRecords[actor].scriptId;
                *(char *)&g_BattleActorIdleScripts[actor] = g_BattleScene.actors[actor].scriptId;
                g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 1;
                g_BattleActorRecords[actor].stateFlags = g_BattleActorRecords[actor].stateFlags | 4;
                g_BattleActorRecords[actor].drawFlags = *(unsigned char *)&g_BattleActorRecords[actor].drawFlags & ~2;
                Battle_PlaySfxPannedToActor(0x2D9, (short)actor, 0);
                return;
            case BATTLE_MOTION_DELAYED_ACTOR_EFFECT:
                animStatus->secondarySlot = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->posY = Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskDelayedActorEffect);
                BATTLE_SLOT_VIEW(BattleEffectActorEffectSlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->posY])->actorWord = actor;
                BATTLE_SLOT_VIEW(BattleEffectActorEffectSlot, &g_BattleTaskSlotBuffer.secondarySlots[animStatus->posY])->delay = animStatus->secondarySlot;
                break;
            case BATTLE_MOTION_RUN_EFFECT:
                if (g_EngineIsEffectLoading == 0) {
                    Battle_RunActorEffect((char)actor);
                    g_BattleEffectOpStalled = 0;
                } else {
                    (g_BattleActorRecords[actor].scriptPos)--;
                    animStatus->running = 0;
                    g_BattleEffectOpStalled = 1;
                }
                break;
            case BATTLE_MOTION_STATUS_POSE:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleVmReg_1 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                Battle_SpawnStatusPoseTask((short)actor, *(char *)&g_BattleVmReg_0, *(char *)&g_BattleVmReg_1);
                break;
            case BATTLE_MOTION_WAIT_MESSAGE_WINDOW_DELAYED:
                g_BattleVmReg_0 = *(unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                animStatus->taskSlot = Battle_AllocateResourceSlot(Battle_MainTaskWaitMessageWindow);
                BATTLE_SLOT_VIEW(BattleMessageWaitSlot, &g_BattleTaskSlotBuffer.mainSlots[animStatus->taskSlot])->displayTime = Battle_GetTextSpeedDelay();
                BATTLE_SLOT_VIEW(BattleMessageWaitSlot, &g_BattleTaskSlotBuffer.mainSlots[animStatus->taskSlot])->preDelay = *(short *)&g_BattleVmReg_0;
                break;
            case BATTLE_MOTION_WAIT_MESSAGE_WINDOW:
                animStatus->taskSlot = Battle_AllocateResourceSlot(Battle_MainTaskWaitMessageWindow);
                BATTLE_SLOT_VIEW(BattleMessageWaitSlot, &g_BattleTaskSlotBuffer.mainSlots[animStatus->taskSlot])->displayTime = Battle_GetTextSpeedDelay();
                BATTLE_SLOT_VIEW(BattleMessageWaitSlot, &g_BattleTaskSlotBuffer.mainSlots[animStatus->taskSlot])->preDelay = 0;
                break;
            case BATTLE_MOTION_RUN_EFFECT_IMPACT:
                if (g_EngineIsEffectLoading == 0) {
                    Battle_RunActorEffectImpact((char)actor);
                } else {
                    (g_BattleActorRecords[actor].scriptPos)--;
                    animStatus->running = 0;
                }
                break;
            case BATTLE_MOTION_SKIP_BLOCK_END:
            case BATTLE_MOTION_LOOP_LABEL:
                break;
            case BATTLE_MOTION_RESUME_CAMERA:
                g_BattleScriptBusy = 0;
                break;
            case BATTLE_MOTION_PLAY_ANIM_KEEP_STEP:
                g_BattleActorRecords[actor].animId = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleActorRecords[actor].animTicks = 0;
                g_BattleActorTransformRecords[actor].animState = 0;
                if (*(unsigned char *)&g_BattleActorStates[actor].timer != 0) {
                    for (loopIndex = 0; loopIndex < *(unsigned char *)&g_BattleActorStates[actor].timer; loopIndex++) {
                        Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
                    }
                }
                animStatus->running = 0;
                break;
            case BATTLE_MOTION_HOLD_CAMERA_PLAY_ANIM_KEEP_STEP:
                Battle_AllocateSecondaryResourceSlot(Battle_EffectTaskRaiseScriptBusy);
                g_BattleActorRecords[actor].animId = (unsigned short)*(volatile unsigned char *)(animStatus->scriptPtr + (g_BattleActorRecords[actor].scriptPos));
                (g_BattleActorRecords[actor].scriptPos)++;
                g_BattleActorRecords[actor].animTicks = 0;
                g_BattleActorTransformRecords[actor].animState = 0;
                if (*(unsigned char *)&g_BattleActorStates[actor].timer != 0) {
                    for (loopIndex = 0; loopIndex < *(unsigned char *)&g_BattleActorStates[actor].timer; loopIndex++) {
                        Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
                    }
                }
                animStatus->running = 0;
                break;
            default:
                g_BattleActorRecords[actor].animId = (unsigned short)*(volatile unsigned char *)&animStatus->opcode;
                g_BattleActorRecords[actor].animTicks = 0;
                g_BattleActorStates[actor].timer = 0;
                g_BattleActorTransformRecords[actor].animState = 0;
                animStatus->running = 0;
            }
        }
    }
    if ((g_BattleActorStates[actor].flags & 2) != 0) {
        return;
    }
    (g_BattleActorStates[actor].timer)++;
    if (g_BattleActorRecords[actor].scriptId != *(char *)&g_BattleActorIdleScripts[actor]) {
        Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
    } else {
        switch ((char)g_BattleActorStates[actor].motionState) {
        default:
            Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
            break;
        case BATTLE_ACTOR_MOTION_HASTE:
            Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
            Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
            break;
        case BATTLE_ACTOR_MOTION_SLOW:
            if ((g_Battle3dFrameTick & 1) != 0) {
                Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
            }
            break;
        case BATTLE_ACTOR_MOTION_FROZEN:
            g_BattleActorRecords[actor].animTicks = 0;
            g_BattleActorTransformRecords[actor].animState = 0;
            g_BattleAnimTransformActive = 1;
            Battle_AdvanceActorAnimation((short)actor, animMode, animFlags);
        }
    }
}

/*
 * Whether animation index animIndex (0x2C..0x33) on party slot partySlot (0..2) is one of the weapon
 * animations.
 */
// FUNCTION: FF7 0x00424b1a
int __cdecl Battle_IsWeaponAnimIndex(int partySlot, int animIndex)
{
    int result;

    result = 0;
    if (partySlot >= 0 && partySlot < 3 && animIndex >= 44 && animIndex < 52) {
        result = 1;
    }
    return result;
}

/*
 * With nonnull actor model copies its header short and clears the selected animation index. For
 * nonnegative animation selects a weapon animation when applicable or a checked normal table
 * entry, preserving previous frame and advancing/clamping to count minus one while setting
 * completion. Missing weapon data marks complete without logging; missing/out-of-range normal data
 * logs errors. Shared freeze/actor controls can suppress normal advancement or force the last
 * frame; weapon advancement ignores those controls. Null model or negative animation leaves later
 * state untouched; no zero-frame-count guard appears.
 */
// FUNCTION: FF7 0x00424b4b
void __cdecl Battle_AdvanceActorAnimation(short actor, int animMode, int animFlags)
{
    BattleActorAnimSet *animSet;
    char buf[0xC8];
    int animId;
    BattleAnimClip *clip;
    int advance;
    int weaponIdx;

    animSet = (BattleActorAnimSet *)(g_BattleActorRecords[actor].model);
    if (animSet != 0) {
        g_BattleActorRecords[actor].boneCount = animSet->skeleton->boneCount;
        g_BattleActorRecords[actor].animFrame = -1;
        animId = g_BattleActorRecords[actor].animId;
        if (animId >= 0) {
            if (Battle_IsWeaponAnimIndex(actor, animId) != 0) {
                if (animSet->weaponClipTable != 0) {
                    g_BattleActorRecords[actor].useSecondAngleTable = 1;
                    weaponIdx = animId - 44;
                    clip = animSet->weaponClipTable[weaponIdx];
                    if (clip != 0) {
                        g_BattleActorRecords[actor].animFrame = weaponIdx;
                        g_BattleActorRecords[actor].animBank = g_BattleActorRecords[actor].nextAnimBank;
                        g_BattleActorRecords[actor].nextAnimBank = g_BattleActorRecords[actor].nextAnimBank + 1;
                        if (g_BattleActorRecords[actor].nextAnimBank >= clip->frameCount) {
                            g_BattleActorRecords[actor].nextAnimBank = clip->frameCount - 1;
                            g_BattleActorRecords[actor].animRunning = 1;
                        } else {
                            g_BattleActorRecords[actor].animRunning = 0;
                        }
                    } else {
                        g_BattleActorRecords[actor].animRunning = 1;
                    }
                } else {
                    g_BattleActorRecords[actor].animRunning = 1;
                }
            } else {
                g_BattleActorRecords[actor].useSecondAngleTable = 0;
                if (animId < animSet->animCount) {
                    g_BattleActorRecords[actor].animFrame = animId;
                    clip = animSet->clipTable[animId];
                    if (clip != 0) {
                        g_BattleActorRecords[actor].animBank = g_BattleActorRecords[actor].nextAnimBank;
                        if ((unsigned char)g_BattleAnimTransformActive == 0) {
                            advance = 1;
                            if ((unsigned char)g_BattleAnimPreloadReady != 0 && (unsigned char)g_BattleEffectOpStalled != 0 && actor == (unsigned char)g_BattleAnimPreloadActor) {
                                if (g_EngineIsEffectLoading != 0) {
                                    g_BattleActorRecords[actor].nextAnimBank = clip->frameCount - 1;
                                    g_BattleActorRecords[actor].animBank = g_BattleActorRecords[actor].nextAnimBank;
                                    advance = 0;
                                } else {
                                    g_BattleEffectOpStalled = 0;
                                    g_BattleAnimPreloadReady = 0;
                                }
                            }
                            if (advance != 0) {
                                g_BattleActorRecords[actor].nextAnimBank = g_BattleActorRecords[actor].nextAnimBank + 1;
                            }
                        }
                        if (g_BattleActorRecords[actor].nextAnimBank >= clip->frameCount) {
                            g_BattleActorRecords[actor].nextAnimBank = clip->frameCount - 1;
                            g_BattleActorRecords[actor].animRunning = 1;
                        } else {
                            g_BattleActorRecords[actor].animRunning = 0;
                        }
                    } else {
                        g_BattleActorRecords[actor].animRunning = 1;
                        sprintf(buf, Battle_AnmErrorNoAnimationFormat(), animSet->animCount, animId);
                        Gfx_DebugPrint(buf);
                    }
                } else {
                    g_BattleActorRecords[actor].animRunning = 1;
                    sprintf(buf, Battle_AnmErrorAnimationRangeFormat(), animSet->animCount, animId);
                    Gfx_DebugPrint(buf);
                }
            }
        }
    }
}

/*
 * Fetches a little-endian 16-bit word from actor actor's animation script at script, advancing its
 * script position g_BattleActorRecords[actor].scriptPos twice.
 */
// FUNCTION: FF7 0x00424f5f
int __cdecl Battle_ScriptFetchWord(short actor, char *script)
{
    int highByte;
    int lowByte;

    lowByte = *(unsigned char *)(script + (g_BattleActorRecords[actor].scriptPos));
    (g_BattleActorRecords[actor].scriptPos)++;
    highByte = *(unsigned char *)(script + (g_BattleActorRecords[actor].scriptPos));
    (g_BattleActorRecords[actor].scriptPos)++;
    return (highByte << 8) + lowByte;
}

/*
 * Derives an angle from the second actor's facing and a distance from both actors
 * scale/position words, then writes first-actor X/Z relative to the second actor through
 * SinFixed/CosFixed with 12-bit shifts, Y from height, and the derived angle to the actor's yaw.
 */
// FUNCTION: FF7 0x00424ff1
void __cdecl Battle_PlaceActorInFrontOfActor(char actor, char anchor, short distance, short height)
{
    g_BattleVmReg_0 = 0x1000 - (g_BattleActorRecords[anchor].facing + 0x800);
    g_BattleVmReg_1 = (distance * g_BattleActorRecords[actor].scale >> 12) + (g_BattleActorRecords[anchor].radius * g_BattleActorRecords[anchor].scale >> 12);
    g_BattleActorRecords[actor].posX = g_BattleActorRecords[anchor].posX - (SinFixed(g_BattleVmReg_0) * g_BattleVmReg_1 >> 12);
    g_BattleActorRecords[actor].posZ = g_BattleActorRecords[anchor].posZ + (CosFixed(g_BattleVmReg_0) * g_BattleVmReg_1 >> 12);
    g_BattleActorRecords[actor].posY = height;
    g_BattleActorRecords[actor].yaw = *(short *)&g_BattleVmReg_0;
}

/*
 * Snaps actor actor's position (posX, posY and posZ) to its spawn position in the table at
 * g_BattleActorSpawnPos (six bytes per actor).
 */
// FUNCTION: FF7 0x00425119
void __cdecl Battle_SnapActorToSpawnPos(char actor)
{
    g_BattleActorRecords[actor].posX = g_BattleActorSpawnPos[actor].posX;
    g_BattleActorRecords[actor].posY = g_BattleActorSpawnPos[actor].posY;
    g_BattleActorRecords[actor].posZ = g_BattleActorSpawnPos[actor].posZ;
}

/*
 * Effect task that raises the script-busy flag g_BattleScriptBusy and retires its own task record.
 */
// FUNCTION: FF7 0x0042517b
void __cdecl Battle_EffectTaskRaiseScriptBusy(void)
{
    g_BattleScriptBusy = 1;
    g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Computes movement angle/radius from the selected actor or from averages over the low ten shared
 * target-mask bits, accumulating size/scales in shorts and count in char. The average branch
 * divides by count without an empty-mask guard. Derives X/Z target around targetPos and allocates the
 * supplied overlay callback, saving actor/target IDs, duration and shared script parameters.
 * Movement steps are zero for self-target, otherwise displacement divided by duration without a zero-
 * duration guard. The `unused` parameter is ignored; slot indexing uses the returned char directly.
 */
// FUNCTION: FF7 0x0042519a
void __cdecl Battle_SpawnActorMoveToTargetTask(short actor, char *unused, short distance, BattleTaskFn callback, short duration, short *targetPos)
{
    short heading;
    short dist;
    short posX;
    int angleSum;
    int loopIndex;
    short posZ;
    char taskIdx;

    if (g_BattleSequenceStepBusy == 0) {
        heading = 0x1000 - (g_BattleActorStates[g_BattleSavedSpecialActorId].yaw + 0x800);
        dist = (distance * g_BattleActorRecords[actor].scale >> 12) + (g_BattleActorRecords[g_BattleSavedSpecialActorId].radius * g_BattleActorRecords[g_BattleSavedSpecialActorId].scale >> 12);
    } else {
        angleSum = 0;
        taskIdx = angleSum;
        posZ = taskIdx;
        posX = posZ;
        for (loopIndex = 0; loopIndex < 10; loopIndex++) {
            if ((g_BattleEffectScratch.targetMask & (1 << loopIndex)) != 0) {
                angleSum += g_BattleActorRecords[loopIndex].facing;
                posX += g_BattleActorRecords[loopIndex].radius;
                posZ += g_BattleActorRecords[loopIndex].scale;
                taskIdx++;
            }
        }
        angleSum /= taskIdx;
        posX /= taskIdx;
        posZ /= taskIdx;
        heading = 0x1000 - (angleSum + 0x800);
        dist = (distance * g_BattleActorRecords[actor].scale >> 12) + (posX * posZ >> 12);
    }
    posX = targetPos[0] - (SinFixed(heading) * dist >> 12);
    posZ = targetPos[2] + (CosFixed(heading) * dist >> 12);
    taskIdx = Battle_AllocateOverlayTaskSlot((BattleTaskFn)(callback));
    g_BattleOverlayTaskSlotBuffer[taskIdx].actorId = actor;
    g_BattleOverlayTaskSlotBuffer[taskIdx].battleActorIndex = (short)g_BattleSavedSpecialActorId;
    if (g_BattleSavedSpecialActorId == actor) {
        g_BattleOverlayTaskSlotBuffer[taskIdx].velocityX = 0;
        g_BattleOverlayTaskSlotBuffer[taskIdx].velocityZ = 0;
    } else {
        g_BattleOverlayTaskSlotBuffer[taskIdx].velocityX = (posX - g_BattleActorRecords[actor].posX) / duration;
        g_BattleOverlayTaskSlotBuffer[taskIdx].velocityZ = (posZ - g_BattleActorRecords[actor].posZ) / duration;
    }
    g_BattleOverlayTaskSlotBuffer[taskIdx].sinkCountdown = duration;
    g_BattleOverlayTaskSlotBuffer[taskIdx].duration = g_BattleVmReg_0;
    g_BattleOverlayTaskSlotBuffer[taskIdx].velocityY = g_BattleVmReg_1;
    g_BattleOverlayTaskSlotBuffer[taskIdx].spawnArg = *(char *)&g_BattleVmReg_2;
    g_BattleOverlayTaskSlotBuffer[taskIdx].targetRef = *(char *)&g_BattleVmReg_3;
    g_BattleOverlayTaskSlotBuffer[taskIdx].timer = g_BattleApproachHoldFrames;
}

/*
 * Restarts actor actor's animation script: sets animRunning and clears scriptPos and waitFrames.
 */
// FUNCTION: FF7 0x004254b0
void __cdecl Battle_RestartActorScript(short actor)
{
    g_BattleActorRecords[actor].animRunning = 1;
    g_BattleActorRecords[actor].scriptPos = 0;
    g_BattleActorRecords[actor].waitFrames = 0;
}

/*
 * Copies the flag g_BattleScriptFlags into bit 0 of the short g_BattleSceneFlags.
 */
// FUNCTION: FF7 0x004254e8
void __cdecl Battle_ApplyVmFlagArrayBit0(void)
{
    if ((*(char *)&g_BattleScriptFlags) != 0) {
        g_BattleSceneFlags = g_BattleSceneFlags | 1;
    } else {
        g_BattleSceneFlags = g_BattleSceneFlags & 0xFFFFFFFE;
    }
}

/*
 * Dispatches the current 0x20-byte effect record event, calls BattleFx_MagicLimitFlashSpawnForActor with the actor and
 * the record's frameCounter, then frees the slot.
 */
// FUNCTION: FF7 0x00425520
void __cdecl Battle_EffectTaskFireEventAndRetire(void)
{
    Battle_PlaySfxPannedToActor(BATTLE_SECONDARY_VIEW(BattleEffectEventSlot)->event, g_BattleActiveActorId, 0);
    BattleFx_MagicLimitFlashSpawnForActor(g_BattleActiveActorId, g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter);
    g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Posts the level-derived dispatch 0x14 for actor actor, spawns a hit-reaction task for target and
 * pulses the HUD (BattleFx_HitMarkSpawnAtActorBone) for g_BattleSpecialActorId with 0x2C.
 */
// FUNCTION: FF7 0x0042557f
void __cdecl BattleFx_EnterBuiltinHitReaction(int targetMask, int actorIdx)
{
    Battle_PlaySfxPannedToActor(0x14, (short)actorIdx, 0);
    Battle_SpawnHitReactionTask(targetMask, 1, 1);
    BattleFx_HitMarkSpawnAtActorBone(g_BattleSpecialActorId, 0x2C);
}

/*
 * Effect task that counts its delay (+4 of the record) down and then flashes the screen white
 * (0xFA, 0xFA, 0xFA), storing the flash handle in g_BattleOverlayDrawListHead and retiring itself.
 */
// FUNCTION: FF7 0x004255b7
void __cdecl Battle_EffectTaskDelayedWhiteFlash(void)
{
    if (BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->delay == 0) {
        g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0xFA, 0xFA, 0xFA);
        g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->delay--;
    }
}

/*
 * Sets g_BattleActorStates[actor].poseAngleA, poseAngleB and poseAngleC to 0x1000, then sets bit
 * 0x20 in its flags.
 */
// FUNCTION: FF7 0x00425628
void __cdecl Battle_ResetActorScaleAxes(short actor)
{
    g_BattleActorStates[actor].poseAngleC = 0x1000;
    g_BattleActorStates[actor].poseAngleB = 0x1000 /* 360 deg */;
    g_BattleActorStates[actor].poseAngleA = 0x1000 /* 360 deg */;
    g_BattleActorStates[actor].flags = g_BattleActorStates[actor].flags | 0x20;
}

/*
 * Resets the actor scale axes and selects a pose task from poseKind: 0, 8, 2, 3, 0x11/0x12 and 7
 * choose their respective main-task callbacks and store actor plus signed char param. Values 1 and
 * 0xA allocate the death-sink overlay, store actor and param, and set its extra short to 0xF8.
 * Other values only reset scale.
 */
// FUNCTION: FF7 0x0042567e
void __cdecl Battle_SpawnStatusPoseTask(short actor, char poseKind, char param)
{
    int slot;

    Battle_ResetActorScaleAxes((short)actor);
    switch (poseKind) {
    case BATTLE_POSE_NORMAL:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode0Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)param;
        break;
    case BATTLE_POSE_KNEEL:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode8Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)param;
        break;
    case BATTLE_POSE_DEATH_SINK:
    case BATTLE_POSE_DEATH_SINK_ALT:
        slot = (short)Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskDeathSink);
        g_BattleOverlayTaskSlotBuffer[slot].actorId = actor;
        g_BattleOverlayTaskSlotBuffer[slot].sfxPan = (short)param;
        g_BattleOverlayTaskSlotBuffer[slot].battleActorIndex = 248;
        break;
    case BATTLE_POSE_HURT:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode2Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)param;
        break;
    case BATTLE_POSE_FADE_OUT:
        slot = (short)Battle_AllocateResourceSlot(Battle_ActorFadeOutEffectTask);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)param;
        break;
    case BATTLE_POSE_CRITICAL_A:
    case BATTLE_POSE_CRITICAL_B:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode17Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)param;
        break;
    case BATTLE_POSE_VICTORY:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode7Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)param;
    }
}

/*
 * Resets the actor scale axes and selects a pose task from the actor's
 * g_BattleScene.actors[actor].state: 0, 8, 2, 3, 0x11/0x12 and 7 choose their respective main-task
 * callbacks and store actor plus signed char poseArg. Values 1 and 0xA allocate the death-sink
 * overlay, store actor and poseArg, and set its extra short to 0xF8. Other values only reset scale.
 */
// FUNCTION: FF7 0x00425890
void __cdecl Battle_SpawnPoseTaskFromHud(short actor, char poseArg)
{
    int slot;

    Battle_ResetActorScaleAxes((short)actor);
    switch (g_BattleScene.actors[actor].state) {
    case BATTLE_POSE_NORMAL:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode0Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)poseArg;
        break;
    case BATTLE_POSE_KNEEL:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode8Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)poseArg;
        break;
    case BATTLE_POSE_DEATH_SINK:
    case BATTLE_POSE_DEATH_SINK_ALT:
        slot = (short)Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskDeathSink);
        g_BattleOverlayTaskSlotBuffer[slot].actorId = actor;
        g_BattleOverlayTaskSlotBuffer[slot].sfxPan = (short)poseArg;
        g_BattleOverlayTaskSlotBuffer[slot].battleActorIndex = 248;
        break;
    case BATTLE_POSE_HURT:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode2Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)poseArg;
        break;
    case BATTLE_POSE_FADE_OUT:
        slot = (short)Battle_AllocateResourceSlot(Battle_ActorFadeOutEffectTask);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)poseArg;
        break;
    case BATTLE_POSE_CRITICAL_A:
    case BATTLE_POSE_CRITICAL_B:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode17Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)poseArg;
        break;
    case BATTLE_POSE_VICTORY:
        slot = (short)Battle_AllocateResourceSlot(Battle_StatusPoseMode7Task);
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->actor = actor;
        BATTLE_SLOT_VIEW(BattlePoseTaskSlot, &g_BattleTaskSlotBuffer.mainSlots[slot])->sfxEnabled = (short)poseArg;
    }
}

/*
 * Runs the current secondary-task UV script while its saved actor counter matches the actor
 * counter; mismatch retires the task, and a nonzero delay is decremented instead of interpreting.
 * Opcode 0xFD consumes low/high bytes and a delay, writes the combined actor short, sets its
 * update byte and yields. Opcode 0xFE restarts at byte zero within the same invocation; 0xFF
 * retires the task, and other bytes yield. The plain char script index, script extent and restart
 * loop have no checks.
 */
// FUNCTION: FF7 0x00425aad
void __cdecl Battle_EffectTaskRunUvScrollScript(void)
{
    unsigned short upperShort;
    unsigned short firstWord;
    unsigned char lowByte;
    unsigned char highByte;
    unsigned char *script;
    unsigned char opCode;
    unsigned char running;
    if (*(unsigned char *)&g_BattleActorStates[BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->actor].frameCounter != (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter)) {
        g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else if (BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->delay) {
        BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->delay--;
    } else {
        running = 1;
        while (running) {
            script = BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->script;
            opCode = script[BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex];
            BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex++;
            switch (opCode) {
            case BATTLE_UV_SCROLL_OP_SET_AND_WAIT:
                lowByte = script[BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex];
                BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex++;
                highByte = script[BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex];
                BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex++;
                BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->delay = (unsigned short)script[BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex];
                BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex++;
                firstWord = lowByte;
                upperShort = highByte;
                g_BattleActorStates[BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->actor].mirrorWord = firstWord | (upperShort << 8);
                g_BattleActorStates[BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->actor].drawEnabled = 1;
                running = 0;
                break;
            case BATTLE_UV_SCROLL_OP_RESTART:
                BATTLE_SECONDARY_VIEW(BattleUvScrollSlot)->scriptIndex = 0;
                break;
            case BATTLE_UV_SCROLL_OP_END:
                g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
                running = 0;
                break;
            default:
                running = 0;
                break;
            }
        }
    }
}

/*
 * Decrements the current-slot delay until zero; then, unless its actor word is -1, spawns an
 * effect pair and two secondary tasks populated from the current-slot fields, and retires the
 * current slot in either actor case.
 */
// FUNCTION: FF7 0x00425d29
void __cdecl Battle_MainTaskSpawnDelayedEffectPair(void)
{
    int task;

    if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->delay == 0) {
        if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actorWord != -1) {
            Battle_SpawnEffectPairForActor();
            task = (short)Battle_AllocateSecondaryResourceSlot(Battle_DamageNumberTask);
            BATTLE_SLOT_VIEW(BattleDamageNumberSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->flags = (int)BATTLE_MAIN_VIEW(BattleHitReactionSlot)->damage;
            BATTLE_SLOT_VIEW(BattleDamageNumberSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->value = BATTLE_MAIN_VIEW(BattleHitReactionSlot)->targetParam;
            BATTLE_SLOT_VIEW(BattleDamageNumberSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->actor = (int)BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor;
            task = (short)Battle_AllocateSecondaryResourceSlot(Battle_HitReactionDelayedCommitTask);
            BATTLE_SLOT_VIEW(BattleHitCommitSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->value = (short)BATTLE_MAIN_VIEW(BattleHitReactionSlot)->commitByte;
            BATTLE_SLOT_VIEW(BattleHitCommitSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->actor = BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actorWord;
        }
        g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        BATTLE_MAIN_VIEW(BattleHitReactionSlot)->delay--;
    }
}

/*
 * State 0 sets the current secondary-slot delay to 3 and increments frameCounter, then falls
 * through to state 1. State 1 retires the slot and calls Battle_CopyDamageDisplayValuesWithinLimits with value and actor when
 * delay is zero, then decrements delay even on that final call.
 */
// FUNCTION: FF7 0x00425e5f
void __cdecl Battle_HitReactionDelayedCommitTask(void)
{
    switch (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter) {
    case BATTLE_TASK_FRAME_FIRST:
        BATTLE_SECONDARY_VIEW(BattleHitCommitSlot)->delay = 3;
        (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter)++;
    case BATTLE_TASK_FRAME_UPDATE:
        if (!BATTLE_SECONDARY_VIEW(BattleHitCommitSlot)->delay) {
            g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
            Battle_CopyDamageDisplayValuesWithinLimits(BATTLE_SECONDARY_VIEW(BattleHitCommitSlot)->value, BATTLE_SECONDARY_VIEW(BattleHitCommitSlot)->actor);
        }
        BATTLE_SECONDARY_VIEW(BattleHitCommitSlot)->delay--;
    }
}

/*
 * Main task that spawns the paired effect for the current record's actor: BattleFx_SBariaBeginActorVariant1 when the
 * actor's bit is in g_BattleEffectActorMask, otherwise BattleFx_SBariaBeginActorVariant0 when it is in g_BattleEffectTargetMask.
 */
// FUNCTION: FF7 0x00425f3f
void __cdecl Battle_SpawnEffectPairForActor(void)
{
    if ((g_BattleEffectActorMask & 1 << BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor) != 0) {
        BattleFx_SBariaBeginActorVariant1(BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor);
    } else if ((g_BattleEffectTargetMask & 1 << BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor) != 0) {
        BattleFx_SBariaBeginActorVariant0(BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor);
    }
}

/*
 * Decrements the current-slot delay until zero; then flashes white when flag bit two is set,
 * conditionally spawns an effect pair and two secondary tasks when actorWord is not -1 and mode
 * is not one, and calls Battle_CompleteCurrentMainTask.
 */
// FUNCTION: FF7 0x00425fc4
void __cdecl Battle_HitReactionTask(void)
{
    int task;

    if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->delay == 0) {
        if ((BATTLE_MAIN_VIEW(BattleHitReactionSlot)->damage & 2) != 0) {
            g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0xFA, 0xFA, 0xFA);
        }
        if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actorWord != -1 && BATTLE_MAIN_VIEW(BattleHitReactionSlot)->mode != 1) {
            Battle_SpawnEffectPairForActor();
            task = (short)Battle_AllocateSecondaryResourceSlot(Battle_DamageNumberTask);
            BATTLE_SLOT_VIEW(BattleDamageNumberSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->flags = (int)BATTLE_MAIN_VIEW(BattleHitReactionSlot)->damage;
            BATTLE_SLOT_VIEW(BattleDamageNumberSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->value = BATTLE_MAIN_VIEW(BattleHitReactionSlot)->targetParam;
            BATTLE_SLOT_VIEW(BattleDamageNumberSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->actor = (int)BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor;
            task = (short)Battle_AllocateSecondaryResourceSlot(Battle_HitReactionDelayedCommitTask);
            BATTLE_SLOT_VIEW(BattleHitCommitSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->value = (short)BATTLE_MAIN_VIEW(BattleHitReactionSlot)->commitByte;
            BATTLE_SLOT_VIEW(BattleHitCommitSlot, &g_BattleTaskSlotBuffer.secondarySlots[task])->actor = BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actorWord;
        }
        Battle_CompleteCurrentMainTask();
    } else {
        BATTLE_MAIN_VIEW(BattleHitReactionSlot)->delay--;
    }
}

/*
 * Completes the current main task: optionally dispatches its stored event (substituting event 5
 * when the task short is -1 and the other actor is at least 4), then schedules
 * Battle_OverlayTaskSpawnHitMark when the packed value low byte is not 0xFF. Updates the task actor
 * completion, script and restart flags according to whether the actor differs from the saved actor,
 * its saved script 0x33, active masks and battle state. The normal differing-actor path includes
 * the g_BattleScene.locationId value 0x4E special script-1 override and conditionally restores
 * the initiating actor yaw. Equal actors restore enemy scripts when masked, otherwise set
 * completion for the specified battle-record type. Finally marks the main task slot -1.
 */
// FUNCTION: FF7 0x0042613a
void __cdecl Battle_CompleteCurrentMainTask(void)
{
    if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->event != -1) {
        if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->targetParam == -1 && BATTLE_MAIN_VIEW(BattleHitReactionSlot)->otherActor >= 4) {
            Battle_PlaySfxPannedToActor(5, BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor, 0);
        } else {
            Battle_PlaySfxPannedToActor(BATTLE_MAIN_VIEW(BattleHitReactionSlot)->eventSfx, BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor, 0);
        }
    }
    if ((BATTLE_MAIN_VIEW(BattleHitReactionSlot)->packed & 0xFF) != 0xFF) {
        g_BattleVmReg_0 = (short)Battle_AllocateSecondaryResourceSlot(Battle_OverlayTaskSpawnHitMark);
        BATTLE_SLOT_VIEW(BattleHitMarkSlot, &g_BattleTaskSlotBuffer.secondarySlots[g_BattleVmReg_0])->actor = BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor;
        BATTLE_SLOT_VIEW(BattleHitMarkSlot, &g_BattleTaskSlotBuffer.secondarySlots[g_BattleVmReg_0])->flags = BATTLE_MAIN_VIEW(BattleHitReactionSlot)->packed;
    }
    if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->otherActor != BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor) {
        switch (g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].state) {
        case BATTLE_BUILTIN_SCRIPT_33:
            if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor >= 4) {
                if (g_BattleTaskSlotBuffer.isBusy == 0) {
                    if ((g_BattleActiveTargetMask & 1 << BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor) != 0) {
                        Battle_SpawnPoseTaskFromHud(BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor, 1);
                    }
                } else if ((g_BattleActorStates[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].flags & 0x10) != 0) {
                    Battle_SpawnPoseTaskFromHud(BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor, 1);
                }
            } else {
                g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].isDirty = 1;
            }
            break;
        default:
            g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].isDirty = 0;
            if (g_BattleScene.locationId == 0x4E) {
                if (g_BattleHitReactionCount == 15 && g_BattleActorRecords[g_BattleActiveActorId].effectId == 6 && g_BattleActorRecords[g_BattleActiveActorId].effectCategory == BATTLE_EFFECT_CATEGORY_LIMIT) {
                    g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].scriptId = 1;
                    g_BattleScene.actors[4].scriptId = 5;
                } else if (g_BattleSpecialActorId == 4) {
                    g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].scriptId = 1;
                    g_BattleScene.actors[4].scriptId = 5;
                } else {
                    g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].scriptId = g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].state;
                }
            } else {
                g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].scriptId = g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].state;
            }
            g_BattleActorStates[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].flags = g_BattleActorStates[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].flags | 8;
            g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].stateFlags = g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].stateFlags | 1;
            g_BattleHitReactionActor = BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actorByte;
            if (g_BattleActorRecords[g_BattleActiveActorId].effectId != 0x15 || g_BattleActorRecords[g_BattleActiveActorId].effectCategory != BATTLE_EFFECT_CATEGORY_ENEMY_SKILL) {
                if ((g_BattleActorRecords[g_BattleSpecialActorId].modelFlags & 0x40) == 0 && g_BattleSequenceStepBusy == 0 && (g_BattleActorStates[g_BattleSpecialActorId].status & BATTLE_STATUS_PETRIFY) == 0 && (g_BattleActorStates[g_BattleSpecialActorId].status & BATTLE_STATUS_STOP) == 0 && g_BattleTargetFacingLocked == 0) {
                    g_BattleActorRecords[g_BattleSpecialActorId].yaw = g_BattleActorStates[g_BattleSpecialActorId].yaw;
                }
            }
        }
    } else if ((g_BattleActiveTargetMask & 1 << BATTLE_MAIN_VIEW(BattleHitReactionSlot)->otherActor) != 0) {
        if (BATTLE_MAIN_VIEW(BattleHitReactionSlot)->otherActor >= 4) {
            g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].scriptId = g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].state;
            g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].stateFlags = g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].stateFlags | 1;
            g_BattleActorStates[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].flags = g_BattleActorStates[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].flags | 8;
            g_BattleHitReactionActor = BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actorByte;
        }
    } else if (g_BattleSequenceBuffer.steps[g_BattleSequenceStepIndex].actionType == 3) {
        g_BattleActorRecords[BATTLE_MAIN_VIEW(BattleHitReactionSlot)->actor].isDirty = 1;
    }
    g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Effect task that raises flag 0x100 in the record's parameter when the current actor's row
 * (g_BattleActorScreenPositions table) is below the target's, pulses the HUD with the parameter, and retires
 * itself.
 */
// FUNCTION: FF7 0x0042675f
void __cdecl Battle_OverlayTaskSpawnHitMark(void)
{
    if (g_BattleActorScreenPositions[g_BattleActiveActorId].prevX < g_BattleActorScreenPositions[BATTLE_SECONDARY_VIEW(BattleHitMarkSlot)->actor].prevX) {
        BATTLE_SECONDARY_VIEW(BattleHitMarkSlot)->flags |= 0x100;
    }
    BattleFx_HitMarkSpawnAtActorBone(BATTLE_SECONDARY_VIEW(BattleHitMarkSlot)->actor, BATTLE_SECONDARY_VIEW(BattleHitMarkSlot)->flags);
    g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
}

/*
 * Overlay task that moves its actor's y position by the record's step (battleActorIndex) once per frame until
 * the counter (sinkCountdown) runs out, then retires.
 */
// FUNCTION: FF7 0x004267f1
void __cdecl Battle_OverlayTaskTranslateY(void)
{
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        g_BattleVmReg_3 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
        g_BattleActorRecords[g_BattleVmReg_3].posY += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
    }
}

/*
 * Overlay task that turns its actor's yaw by the record's step (velocityZ) once per frame until the
 * counter runs out, then retires.
 */
// FUNCTION: FF7 0x00426899
void __cdecl Battle_OverlayTaskRotateYaw(void)
{
    g_BattleVmReg_3 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        g_BattleActorRecords[g_BattleVmReg_3].yaw += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityZ;
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
    }
}

/*
 * Overlay task that moves its actor's x and z by the record's two steps (held in battleActorIndex
 * and velocityX) once per frame until sinkCountdown runs out, then retires.
 */
// FUNCTION: FF7 0x00426941
void __cdecl Battle_OverlayTaskTranslateXZ(void)
{
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        g_BattleVmReg_3 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
        g_BattleActorRecords[g_BattleVmReg_3].posX += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
        g_BattleActorRecords[g_BattleVmReg_3].posZ += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityX;
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
    }
}

/*
 * Overlay task that walks an actor toward a target actor: on its first frame it derives the y step
 * (held in sfxPan) from the height difference over the counter (only for enemies, and only when
 * g_BattleSequenceStepBusy is clear), then each frame after the hold count (timer) expires it moves x,
 * z and y by the record's steps until the counter runs out and retires; g_BattleApproachHoldFrames is
 * cleared while moving.
 */
// FUNCTION: FF7 0x00426a26
void __cdecl Battle_OverlayTaskApproachTarget(void)
{
    g_BattleVmReg_3 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
    g_BattleVmReg_2 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state == 0) {
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state = 1;
        if (g_BattleVmReg_3 < 4) {
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan = 0;
        } else if (g_BattleSequenceStepBusy == 0) {
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan = ((g_BattleActorRecords[g_BattleVmReg_2].posY * g_BattleActorRecords[g_BattleVmReg_2].scale >> 12) - (g_BattleActorRecords[g_BattleVmReg_3].posY * g_BattleActorRecords[g_BattleVmReg_3].scale >> 12)) / g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown;
        } else {
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan = 0;
        }
    }
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].timer == 0) {
        g_BattleApproachHoldFrames = 0;
        if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        } else {
            g_BattleActorRecords[g_BattleVmReg_3].posX += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityX;
            g_BattleActorRecords[g_BattleVmReg_3].posZ += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityZ;
            g_BattleActorRecords[g_BattleVmReg_3].posY += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan;
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
        }
    } else {
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].timer)--;
    }
}

/*
 * Allocates a return-to-spawn overlay task with actor actor, current actor, duration duration and
 * narrowed hold hold. Clears all steps when actor is the current actor; otherwise divides each
 * spawn-minus-current coordinate by duration to initialize movement, without a zero-divisor guard.
 */
// FUNCTION: FF7 0x00426c9b
void __cdecl Battle_SpawnReturnToSpawnPosTask(short actor, short duration, short hold)
{
    char slot;

    slot = Battle_AllocateOverlayTaskSlot(Battle_OverlayTaskReturnToSpawnPos);
    g_BattleOverlayTaskSlotBuffer[slot].actorId = actor;
    g_BattleOverlayTaskSlotBuffer[slot].battleActorIndex = (short)g_BattleSavedSpecialActorId;
    g_BattleOverlayTaskSlotBuffer[slot].sinkCountdown = duration;
    g_BattleOverlayTaskSlotBuffer[slot].timer = hold;
    if (g_BattleSavedSpecialActorId == actor) {
        g_BattleOverlayTaskSlotBuffer[slot].velocityX = 0;
        g_BattleOverlayTaskSlotBuffer[slot].velocityZ = 0;
        g_BattleOverlayTaskSlotBuffer[slot].sfxPan = 0;
    } else {
        g_BattleOverlayTaskSlotBuffer[slot].velocityX = (g_BattleActorSpawnPos[actor].posX - g_BattleActorRecords[actor].posX) / duration;
        g_BattleOverlayTaskSlotBuffer[slot].velocityZ = (g_BattleActorSpawnPos[actor].posZ - g_BattleActorRecords[actor].posZ) / duration;
        g_BattleOverlayTaskSlotBuffer[slot].sfxPan = (g_BattleActorSpawnPos[actor].posY - g_BattleActorRecords[actor].posY) / duration;
    }
}

/*
 * Overlay task that returns an actor to its spawn position: after the hold count expires it moves
 * x, z and y by the record's steps each frame until the counter runs out, then retires.
 */
// FUNCTION: FF7 0x00426de3
void __cdecl Battle_OverlayTaskReturnToSpawnPos(void)
{
    g_BattleVmReg_3 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
    g_BattleVmReg_2 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].timer == 0) {
        if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        } else {
            g_BattleActorRecords[g_BattleVmReg_3].posX += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityX;
            g_BattleActorRecords[g_BattleVmReg_3].posZ += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityZ;
            g_BattleActorRecords[g_BattleVmReg_3].posY += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan;
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
        }
    } else {
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].timer)--;
    }
}

/*
 * Overlay task that hops an actor along an arc: each frame moves x and z by the steps and y by the
 * arc table g_BattleRestingYArrayData (eight steps per row, the row from duration) entry for the hop's
 * phase (timer, advanced every frame), until the counter runs out and it retires.
 */
// FUNCTION: FF7 0x00426f58
void __cdecl Battle_OverlayTaskHopArc(void)
{
    if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        g_BattleVmReg_3 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
        g_BattleVmReg_2 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
        g_BattleVmReg_4 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].duration;
        g_BattleActorRecords[g_BattleVmReg_3].posX += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityX;
        g_BattleActorRecords[g_BattleVmReg_3].posZ += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityZ;
        g_BattleActorRecords[g_BattleVmReg_3].posY += *(short *)((char *)&g_BattleRestingYArrayData + ((g_BattleVmReg_4 << 4) + g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].timer * 2));
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].timer)++;
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
    }
}

/*
 * Loads the two overlay actor indices, initializes a lift-duration counter in state zero, moves Y
 * by the low word of velocityY in state one, then computes a scaled Y step and transitions to state
 * two; state two moves X/Z and subtracts the Y step until its counter is zero, then retires the task.
 */
// FUNCTION: FF7 0x004270de
void __cdecl Battle_OverlayTaskLiftThenTravel(void)
{
    g_BattleVmReg_3 = (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId);
    g_BattleVmReg_2 = (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex);
    switch (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state) {
    case BATTLE_LIFT_TRAVEL_PHASE_INIT:
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown) = (short)(g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].spawnArg);
        (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state) = BATTLE_LIFT_TRAVEL_PHASE_LIFT;
        break;
    case BATTLE_LIFT_TRAVEL_PHASE_LIFT:
        if (!(g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)) {
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown) = (short)(g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].targetRef);
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state) = BATTLE_LIFT_TRAVEL_PHASE_TRAVEL;
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan) = ((((g_BattleActorRecords[g_BattleVmReg_3].posY) - (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].duration)) * (g_BattleActorRecords[g_BattleVmReg_2].scale)) >> 12) / (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].targetRef);
        } else {
            (g_BattleActorRecords[g_BattleVmReg_3].posY) += (*(short *)&g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityY);
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
        }
        break;
    case BATTLE_LIFT_TRAVEL_PHASE_TRAVEL:
        if (!(g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)) {
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        } else {
            (g_BattleActorRecords[g_BattleVmReg_3].posX) += (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityX);
            (g_BattleActorRecords[g_BattleVmReg_3].posZ) += (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityZ);
            (g_BattleActorRecords[g_BattleVmReg_3].posY) -= (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan);
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
        }
        break;
    }
}

/*
 * Loads the overlay actor indices and initializes a travel counter and Y step toward the height in
 * velocityY in state zero. State one advances X/Z/Y, then computes a scaled drop step and changes to state
 * two; state two subtracts that Y step until its counter is zero, then retires the task.
 */
// FUNCTION: FF7 0x0042739d
void __cdecl Battle_OverlayTaskTravelThenDrop(void)
{
    g_BattleVmReg_3 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].actorId;
    g_BattleVmReg_2 = g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex;
    switch (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state) {
    case BATTLE_TRAVEL_DROP_PHASE_INIT:
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown = (short)g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].targetRef;
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan = (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityY - g_BattleActorRecords[g_BattleVmReg_3].posY) / g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].targetRef;
        g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state = BATTLE_TRAVEL_DROP_PHASE_TRAVEL;
        break;
    case BATTLE_TRAVEL_DROP_PHASE_TRAVEL:
        if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown = (short)g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].spawnArg;
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan = ((g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityY - g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].duration) * g_BattleActorRecords[g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].battleActorIndex].scale >> 12) / g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].spawnArg;
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state = BATTLE_TRAVEL_DROP_PHASE_DROP;
        } else {
            g_BattleActorRecords[g_BattleVmReg_3].posX += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityX;
            g_BattleActorRecords[g_BattleVmReg_3].posZ += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].velocityZ;
            g_BattleActorRecords[g_BattleVmReg_3].posY += g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan;
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
        }
        break;
    case BATTLE_TRAVEL_DROP_PHASE_DROP:
        if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown == 0) {
            g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        } else {
            g_BattleActorRecords[g_BattleVmReg_3].posY -= g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sfxPan;
            (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].sinkCountdown)--;
        }
        break;
    }
}

/*
 * Effect task that counts its delay down and then pulses the HUD for the record's actor with its
 * parameter, retiring itself.
 */
// FUNCTION: FF7 0x004276b6
void __cdecl Battle_EffectTaskDelayedHudPulse(void)
{
    if (BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->delay == 0) {
        BattleFx_HitMarkSpawnAtActorBone(BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->actor, BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->value);
        g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->delay--;
    }
}

/*
 * Decrements the current effect record's delay when nonzero; when it is zero, dispatches
 * the event stored in its value and frees the slot.
 */
// FUNCTION: FF7 0x00427737
void __cdecl Battle_EffectTaskDelayedEvent(void)
{
    if (BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->delay == 0) {
        Battle_PlaySfxPannedToActor(BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->value, g_BattleActiveActorId, 0);
        g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
    } else {
        BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->delay--;
    }
}

/*
 * Effect task that counts the delay of the current effect record
 * (g_BattleSecondaryTaskActiveSlotIdx into g_BattleTaskSlotBuffer.secondarySlots) down to zero,
 * then, unless g_EngineIsEffectLoading blocks it, runs the actor effect whose id is effectId
 * and frees the slot.
 */
// FUNCTION: FF7 0x004277b1
void __cdecl Battle_EffectTaskDelayedActorEffect(void)
{
    if (BATTLE_SECONDARY_VIEW(BattleEffectActorEffectSlot)->delay == 0) {
        if (g_EngineIsEffectLoading == 0) {
            Battle_RunActorEffect(BATTLE_SECONDARY_VIEW(BattleEffectActorEffectSlot)->actor);
            g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        }
    } else {
        BATTLE_SECONDARY_VIEW(BattleEffectActorEffectSlot)->delay--;
    }
}

/*
 * Main task that waits for the battle message window: after its preDelay it calls
 * Battle_RequestMessageWindowAt for the current actor's window each frame while drawing is enabled, counting the
 * displayTime down only when g_BattleIsRunning says the window is live, and retires when the time
 * is up.
 */
// FUNCTION: FF7 0x0042782a
void __cdecl Battle_MainTaskWaitMessageWindow(void)
{
    if (BATTLE_MAIN_VIEW(BattleMessageWaitSlot)->preDelay == 0) {
        if (BATTLE_MAIN_VIEW(BattleMessageWaitSlot)->displayTime == 0) {
            g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        } else if (Gfx_IsDrawEnabled() != 0) {
            Battle_RequestMessageWindowAt(g_BattleActorRecords[g_BattleActiveActorId].effectCategory, (short)g_BattleActorStates[g_BattleActiveActorId].power);
            if (g_BattleIsRunning != 0) {
                BATTLE_MAIN_VIEW(BattleMessageWaitSlot)->displayTime--;
            }
        }
    } else if (g_BattleIsRunning != 0) {
        BATTLE_MAIN_VIEW(BattleMessageWaitSlot)->preDelay--;
    }
}

/*
 * Stops the magic effect loader thread g_BattleMagicEffectThread and clears the handle.
 */
// FUNCTION: FF7 0x0042790b
void __cdecl Battle_StopMagicEffectThread(void)
{
    ThreadFreeOwnedRecord(g_BattleMagicEffectThread);
    g_BattleMagicEffectThread = 0;
}

/*
 * When the magic-thread handle is empty prepares its frame from effect, frees effect group zero,
 * creates and initializes the handle, and sets g_EngineIsEffectLoading to one on success; logs creation
 * failure or an already-in-use handle, with the success log disabled.
 */
// FUNCTION: FF7 0x00427928
void __cdecl Battle_StartMagicEffectThread(void (__cdecl *effect)(void))
{
    int frame[4];
    if (!g_BattleMagicEffectThread) {
        ThreadInitCallbackRecord((int)effect, (ThreadCallbackRecord *)frame);
        Battle_EffectFreeGroupTag0();
        g_BattleMagicEffectThread = (ThreadRecord *)ThreadCreateRecordFromTemplate((int)frame);
        if (g_BattleMagicEffectThread) {
            ThreadStartRecord(g_BattleMagicEffectThread);
            if (DEBUG_TRACE) {
                OutputDebugStringA(Battle_MagicThreadStartText());
            }
            g_EngineIsEffectLoading = 1;
        } else {
            if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                OutputDebugStringA(Battle_MagicThreadCreateErrorText());
            }
        }
    } else {
        if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
            OutputDebugStringA(Battle_MagicThreadBusyErrorText());
        }
    }
}

/*
 * When g_BattleMagicEffectThread is nonzero, polls it with ThreadGetState; result 3 stops the thread through
 * Battle_StopMagicEffectThread and clears g_EngineIsEffectLoading. The debug-string branch is disabled.
 */
// FUNCTION: FF7 0x004279bc
void __cdecl Battle_PollMagicEffectThread(void)
{
    int result;
    if (g_BattleMagicEffectThread != 0) {
        result = ThreadGetState(g_BattleMagicEffectThread);
        if (result == 3) {
            if (DEBUG_TRACE) {
                OutputDebugStringA(Battle_MagicThreadEndText());
            }
            Battle_StopMagicEffectThread();
            g_EngineIsEffectLoading = 0;
        }
    }
}

/*
 * Calls Battle_PollMagicEffectThread repeatedly while g_BattleMagicEffectThread is nonzero.
 */
// FUNCTION: FF7 0x00427a01
void __cdecl Battle_WaitAsyncLoadsIdle(void)
{
    ThreadRecord *thread;

    while ((thread = g_BattleMagicEffectThread) != 0) {
        Battle_PollMagicEffectThread();
    }
}

/*
 * Polls the magic effect loader thread with the default arguments.
 */
// FUNCTION: FF7 0x00427a1e
void __cdecl Battle_PollMagicEffectThreadDefault(void)
{
    Battle_PollMagicEffectThread();
}

/*
 * A loop of eight iterations that does nothing: the debug body was compiled out.
 */
// FUNCTION: FF7 0x00427a28
void __cdecl Battle_EmptyDebugLoop(void)
{
    int i;

    for (i = 0; i < 8; i++) {
    }
}

/*
 * A loop of eight iterations that does nothing: the unfrog-party stub's body was compiled out.
 */
// FUNCTION: FF7 0x00427a4a
void __cdecl Battle_UnfrogPartyActorStub(char actor)
{
    int i;

    for (i = 0; i < 8; i++) {
    }
}

/*
 * Enters the last-boss stage effect: sets status bit 1 on the three party actors, runs
 * BattleFx_SaferSephirothDeathLoadAndBegin for summonId and the current actor, allocates a secondary task running Battle_DelayedFadeOutMusicTask
 * (its slot in g_BattleVmReg_0) with a 2-frame delay.
 */
// FUNCTION: FF7 0x00427a6c
void __cdecl BattleFx_SpecialEnterLastBossStage(int summonId)
{
    int i;

    for (i = 0; i < 3; i++) {
        g_BattleActorRecords[i].drawFlags = *(unsigned char *)&g_BattleActorRecords[i].drawFlags | 1;
    }
    BattleFx_SaferSephirothDeathLoadAndBegin(summonId, g_BattleActiveActorId);
    g_BattleVmReg_0 = (short)Battle_AllocateSecondaryResourceSlot(Battle_DelayedFadeOutMusicTask);
    BATTLE_SLOT_VIEW(BattleEffectDelaySlot, &g_BattleTaskSlotBuffer.secondarySlots[g_BattleVmReg_0])->delay = 2;
}

/*
 * Secondary task that counts its delay down, then retires itself and fades the music out.
 */
// FUNCTION: FF7 0x00427af1
void __cdecl Battle_DelayedFadeOutMusicTask(void)
{
    if (BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->delay == 0) {
        g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = BATTLE_TASK_FREE;
        Battle_FadeOutMusic();
    } else {
        BATTLE_SECONDARY_VIEW(BattleEffectDelaySlot)->delay--;
    }
}

/*
 * Frees effect group tag 0 when g_BattleSkipFreeEffectGroupTag0 is zero, clears
 * g_BattleHitReactionNoNumbers, then dispatches on the actor's effectCategory: 4 calls the nonnull
 * g_BattleFx_GeneralDispatchTable entry of its effectId, 7 calls Battle_CoinThrowEffect and 8 the
 * nonnull g_BattleFx_ThrowWeaponDispatchTable entry. Each call passes the target mask in
 * g_BattleEffectScratch and g_BattleActiveActorId. Other categories return without dispatch.
 */
// FUNCTION: FF7 0x00427b4b
void __cdecl Battle_RunActorEffectImpact(char actor)
{
    int (*q)(int, int);

    if (g_BattleSkipFreeEffectGroupTag0 != 0) { /* empty first arm: lib/codegen.h */
    } else {
        Battle_EffectFreeGroupTag0();
    }
    g_BattleHitReactionNoNumbers = 0;
    switch (g_BattleActorRecords[actor].effectCategory) {
    case BATTLE_EFFECT_CATEGORY_GENERAL:
        q = (int (*)(int, int))g_BattleFx_GeneralDispatchTable[g_BattleActorRecords[actor].effectId];
        if (q != 0) {
            q(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
        }
        break;
    case BATTLE_EFFECT_CATEGORY_COIN:
        Battle_CoinThrowEffect(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
        break;
    case BATTLE_EFFECT_CATEGORY_THROW:
        q = (int (*)(int, int))g_BattleFx_ThrowWeaponDispatchTable[g_BattleActorRecords[actor].effectId];
        if (q != 0) {
            q(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
        }
        break;
    }
}

/*
 * Frees effect group tag 0 when g_BattleSkipFreeEffectGroupTag0 is zero, clears g_BattleHitReactionNoNumbers, then dispatches the
 * actor effect category and ID. Category 2 uses the magic callback table with five g_BattleSequenceStepBusy
 * alternatives and the ID 0x19 completion writes; categories 13 and 32 use their callback tables
 * and category 3 runs the summon effect. Category 20 optionally configures and converts/loads the
 * limit animation bank, calls the limit effect table and passes its result to
 * Battle_LimitEffectResultStub, forcing that result to zero for the listed limit IDs. Null
 * callback entries are skipped; category and ID indices and model pointers have no bounds checks.
 * The diagnostic sprintf branch is disabled by if (0).
 */
// FUNCTION: FF7 0x00427c22
void __cdecl Battle_RunActorEffect(char actor)
{
    int (*p)(int, int);
    int (*q)(int, int);
    int bankPointer;
    char buf[0xC8];
    Battle3dPartyLoadDesc *partyLoadDesc;
    PfileLoadOptions opts;
    PfileLoadOptions *loadOptions;
    Battle3dPartyLoadDesc dsc;

    if (g_BattleSkipFreeEffectGroupTag0 != 0) { /* empty first arm: lib/codegen.h */
    } else {
        Battle_EffectFreeGroupTag0();
    }
    g_BattleHitReactionNoNumbers = 0;
    switch (g_BattleActorRecords[actor].effectCategory) {
    case BATTLE_EFFECT_CATEGORY_MAGIC:
        if (g_BattleSequenceStepBusy == 0) {
            switch (g_BattleActorRecords[actor].effectId) {
            case BATTLE_MAGIC_EFFECT_ESCAPE:
                g_BattleActorRecords[0].isDirty = 1;
                g_BattleActorRecords[1].isDirty = 1;
                g_BattleActorRecords[2].isDirty = 1;
            }
            q = (int (*)(int, int))g_BattleFx_MagicDispatchTable[g_BattleActorRecords[actor].effectId];
            if (q != 0) {
                q(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
            }
        } else {
            switch (g_BattleActorRecords[actor].effectId) {
            case BATTLE_MAGIC_EFFECT_BIO3:
                BattleFx_MagicEnterBio3Multi(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_MAGIC_EFFECT_DEMI3:
                BattleFx_MagicLoadAndBeginDemi3Multi(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_MAGIC_EFFECT_BOLT3:
                BattleFx_MagicEnterBolt3Multi(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_MAGIC_EFFECT_ICE3:
                BattleFx_MagicLoadAndBeginIce3Multi(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            case BATTLE_MAGIC_EFFECT_FIRE3:
                BattleFx_MagicEnterFire3Multi(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                break;
            default:
                q = (int (*)(int, int))g_BattleFx_MagicDispatchTable[g_BattleActorRecords[actor].effectId];
                if (q != 0) {
                    q(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
                }
            }
        }
        break;
    case BATTLE_EFFECT_CATEGORY_ENEMY_SKILL:
        q = (int (*)(int, int))g_BattleFx_BlueMagicDispatchTable[g_BattleActorRecords[actor].effectId];
        if (q != 0) {
            q(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
        }
        break;
    case BATTLE_EFFECT_CATEGORY_LIMIT:
        switch (g_BattleActorRecords[actor].effectId) {
        case BATTLE_LIMIT_EFFECT_BLADE_BEAM:
            if (g_BattleActiveActorId == (g_BattleEffectOrderTable[2].targetActor)) {
                g_BattleLimitBladeBeamTargetDiffers = 0;
            } else {
                g_BattleLimitBladeBeamTargetDiffers = 1;
            }
        }
        if (DEBUG_TRACE) {
            // STRING: FF7 0x007c1fb0
            sprintf(buf, "LIMIT BREAK ID %d \n", g_BattleActorRecords[actor].effectId);
            Gfx_DebugPrint(buf);
        }
        bankPointer = g_BattleLimitAnimBankPointers[g_BattleActorRecords[actor].effectId];
        if (bankPointer != 0) {
            partyLoadDesc = &dsc;
            Battle_InitLmdLoadDesc((int *)partyLoadDesc);
            loadOptions = &opts;
            Pfile_InitLoadOptions(loadOptions);
            BattleModel_ConfigureLoadCategory(0, 3, partyLoadDesc, loadOptions);
            Battle_FreeLimitAnims((BattleLimitAnimBank *)(g_BattleActorRecords[actor].model));
            if (partyLoadDesc->loadCategory != 0) {
                if (partyLoadDesc->secondCount != 0) {
                    Battle_ConvertLimitAnimsLegacy(0, partyLoadDesc, (BattleLimitAnimBank *)(g_BattleActorRecords[actor].model), (char *)bankPointer);
                    Battle_FreeLimitAnims((BattleLimitAnimBank *)(g_BattleActorRecords[actor].model));
                }
                Battle_LoadLimitAnimBank((Battle3dLimitAnimBank *)(g_BattleActorRecords[actor].model), partyLoadDesc, loadOptions, (char *)bankPointer);
            } else {
                Battle_ConvertLimitAnimsLegacy(0, partyLoadDesc, (BattleLimitAnimBank *)(g_BattleActorRecords[actor].model), (char *)bankPointer);
            }
        }
        p = (int (*)(int, int))g_BattleFx_LimitDispatchTable[g_BattleActorRecords[actor].effectId];
        if (p != 0) {
            g_BattleVmReg_0 = p(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
            switch (g_BattleActorRecords[actor].effectId) {
            case BATTLE_LIMIT_EFFECT_EMPTY_45:
            case BATTLE_LIMIT_EFFECT_EMPTY_46:
            case BATTLE_LIMIT_EFFECT_EMPTY_47:
            case BATTLE_LIMIT_EFFECT_EMPTY_48:
            case BATTLE_LIMIT_EFFECT_GROW_SCALE:
            case BATTLE_LIMIT_EFFECT_MOG_DANCE:
            case BATTLE_LIMIT_EFFECT_TOY_SOLDIER:
            case BATTLE_LIMIT_EFFECT_LUCKY_GIRL:
            case BATTLE_LIMIT_EFFECT_THUNK_60:
            case BATTLE_LIMIT_EFFECT_DEATH:
            case BATTLE_LIMIT_EFFECT_GALIAN_BEAST_0:
            case BATTLE_LIMIT_EFFECT_GALIAN_BEAST_1:
            case BATTLE_LIMIT_EFFECT_GALIAN_BEAST_2:
            case BATTLE_LIMIT_EFFECT_LIVEWIRE:
            case BATTLE_LIMIT_EFFECT_GALIAN_BEAST_3:
            case BATTLE_LIMIT_EFFECT_NIGHTMARE:
            case BATTLE_LIMIT_EFFECT_GALIAN_BEAST_4:
            case BATTLE_LIMIT_EFFECT_SATAN_SLAM:
            case BATTLE_LIMIT_EFFECT_THUNK_70:
            case BATTLE_LIMIT_EFFECT_THUNK_71:
            case BATTLE_LIMIT_EFFECT_TOY_BOX_B:
            case BATTLE_LIMIT_EFFECT_TOY_BOX:
            case BATTLE_LIMIT_EFFECT_TOY_BOX_C:
            case BATTLE_LIMIT_EFFECT_TOY_BOX_D:
            case BATTLE_LIMIT_EFFECT_THUNK_76:
            case BATTLE_LIMIT_EFFECT_FINISHING_TOUCH_ALT:
            case BATTLE_LIMIT_EFFECT_BLADE_BEAM_ALT:
                g_BattleVmReg_0 = 0;
            }
            Battle_LimitEffectResultStub(actor, g_BattleVmReg_0);
        }
        break;
    case BATTLE_EFFECT_CATEGORY_SPECIAL:
        q = (int (*)(int, int))g_BattleFx_SpecialDispatchTable[g_BattleActorRecords[actor].effectId];
        if (q != 0) {
            q(g_BattleEffectScratch.targetMask, g_BattleActiveActorId);
        }
        break;
    case BATTLE_EFFECT_CATEGORY_SUMMON:
        Battle_RunSummonEffect();
    }
}

/*
 * Limit-effect result stub: copies limitResult into a local and does nothing else.
 */
// FUNCTION: FF7 0x0042819b
void __cdecl Battle_LimitEffectResultStub(char actor, int limitResult)
{
    int copy;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (limitResult != 0) {
        copy = limitResult;
    }
}

/*
 * When g_BattleSkipFreeEffectGroupTag0 is nonzero, saves the actor and clears
 * g_BattleAnimPreloadReady, then preloads the resource selected by its effect category and byte ID
 * through the corresponding short-index table. Category 2 applies five
 * g_BattleSequenceStepBusy-specific resource overrides; category 7 uses resource entry 221;
 * category 3 sets g_BattleAnimPreloadReady to 1 after preloading. Unhandled categories only update
 * the globals. When the enable global is zero only g_BattleAnimPreloadReady is cleared.
 */
// FUNCTION: FF7 0x004281b1
void __cdecl Battle_SelectEffectOverlayProgram(char actor)
{
    if (g_BattleSkipFreeEffectGroupTag0 != 0) {
        g_BattleAnimPreloadActor = actor;
        g_BattleAnimPreloadReady = 0;
        switch (g_BattleActorRecords[actor].effectCategory) {
        case BATTLE_EFFECT_CATEGORY_MAGIC:
            if (g_BattleSequenceStepBusy == 0) {
                Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[g_BattleFxOverlayResourceMap2[g_BattleActorRecords[actor].effectId]]);
            } else {
                switch (g_BattleActorRecords[actor].effectId) {
                case BATTLE_MAGIC_EFFECT_FIRE3:
                    Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[77]);
                    break;
                case BATTLE_MAGIC_EFFECT_BIO3:
                    Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[32]);
                    break;
                case BATTLE_MAGIC_EFFECT_DEMI3:
                    Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[26]);
                    break;
                case BATTLE_MAGIC_EFFECT_ICE3:
                    Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[255]);
                    break;
                case BATTLE_MAGIC_EFFECT_BOLT3:
                    Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[6]);
                    break;
                default:
                    Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[g_BattleFxOverlayResourceMap2[g_BattleActorRecords[actor].effectId]]);
                }
                break;
            }
            break;
        case BATTLE_EFFECT_CATEGORY_COIN:
            Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[221]);
            break;
        case BATTLE_EFFECT_CATEGORY_THROW:
            Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[g_BattleFxOverlayResourceMap7[g_BattleActorRecords[actor].effectId]]);
            break;
        case BATTLE_EFFECT_CATEGORY_ENEMY_SKILL:
            Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[g_BattleFxOverlayResourceMap3[g_BattleActorRecords[actor].effectId]]);
            break;
        case BATTLE_EFFECT_CATEGORY_LIMIT:
            Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[g_BattleFxOverlayResourceMap6[g_BattleActorRecords[actor].effectId]]);
            break;
        case BATTLE_EFFECT_CATEGORY_GENERAL:
            Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[g_BattleFxOverlayResourceMap1[g_BattleActorRecords[actor].effectId]]);
            break;
        case BATTLE_EFFECT_CATEGORY_SPECIAL:
            Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[g_BattleFxOverlayResourceMap5[g_BattleActorRecords[actor].effectId]]);
            break;
        case BATTLE_EFFECT_CATEGORY_SUMMON:
            Battle_PreloadEffectResources(g_BattleFxOverlayCallbacks[g_BattleFxOverlayResourceMap4[g_BattleActorRecords[actor].effectId]]);
            g_BattleAnimPreloadReady = 1;
        }
    } else {
        g_BattleAnimPreloadReady = 0;
    }
}

/*
 * Preloads an effect's resources through the magic effect thread; for
 * BattleFx_WeaponLoadResSuperNova it first destroys the seffect render objects and
 * reloads slots 6 and 7 through BattleStage_FreePartMesh.
 */
// FUNCTION: FF7 0x004284a7
void __cdecl Battle_PreloadEffectResources(void (__cdecl *resource)(void))
{
    if (resource != 0) {
        if (resource == BattleFx_WeaponLoadResSuperNova) {
            Battle3d_DestroySeffectRenderObjects((int)g_pBattle3dScene);
            BattleStage_FreePartMesh(6, g_pBattle3dScene->stageId);
            BattleStage_FreePartMesh(7, g_pBattle3dScene->stageId);
        }
        Battle_StartMagicEffectThread(resource);
        Battle_PollMagicEffectThreadDefault();
    }
}
} /* extern "C" */

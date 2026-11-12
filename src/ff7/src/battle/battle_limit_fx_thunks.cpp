// src/ff7/src/battle/battle_limit_fx_thunks.cpp
// Limit Break Effect Thunks [57, 58, 60, 61, 70, 71, 76]: Cait Sith Slots & Status Overlay Forwarding Thunks
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Thin entry points for limit break effects, each forwarding to a magic, special or
//   status effect routine.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include "ff7/api/battle.h"
#include "lib/types/graphics.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Limit break effect thunk forwarding to Cometeo magic effect.
 */
// FUNCTION: FF7 0x005c0c20
int __cdecl Battle_LimitFxThunk76(int context, int param)
{
    BattleFx_MagicEnterComet2(context, param);
    return 0;
}

/*
 * Limit break effect thunk forwarding to Turara special effect.
 */
// FUNCTION: FF7 0x005c0c37
int __cdecl Battle_LimitFxThunk71(int context, int param)
{
    BattleFx_SpecialEnterIcicleDrop(context, param);
    return 0;
}

/*
 * Limit break effect thunk forwarding to status overlay model attachment.
 */
// FUNCTION: FF7 0x005c0c4e
int __cdecl Battle_LimitFxStatusOverlayThunk(int context, int param)
{
    Battle_EffectAttachStatusOverlayModel(context, param);
    return 0;
}

/*
 * Limit break effect thunk forwarding to Lv5 Death enemy skill effect.
 */
// FUNCTION: FF7 0x005c0c65
int __cdecl Battle_LimitFxThunk60(int context, int param)
{
    BattleFx_BlueEnterL5Death(context, param);
    return 0;
}

/*
 * Limit break effect thunk forwarding to Raku special effect.
 */
// FUNCTION: FF7 0x005c0c7c
int __cdecl Battle_LimitFxThunk70(int context, int param)
{
    BattleFx_SpecialEnterFallingRocks(context, param);
    return 0;
}
} /* extern "C" */

extern "C" {
// GLOBAL: FF7 0x008fe398
int (*g_BattleFx_GeneralDispatchTable[70])() = {(int (*)())&BattleFx_BuiltinFlashSnd06, (int (*)())&BattleFx_BuiltinFlashSnd07, (int (*)())&BattleFx_BuiltinFlashSnd08, (int (*)())&BattleFx_BuiltinFlashSnd1Fa, (int (*)())&BattleFx_BuiltinFlashSnd1Fb, (int (*)())&BattleFx_BuiltinFlashSnd45, (int (*)())&BattleFx_BuiltinFlashSnd45, (int (*)())&BattleFx_BuiltinFlashSnd17E, (int (*)())&BattleFx_BuiltinFlashSnd5C, (int (*)())&BattleFx_SpecialEnterSmokeShot, (int (*)())&BattleFx_MagicEnterToad, (int (*)())&BattleFx_MagicEnterMini, (int (*)())&Battle_ItemFxEnterEchoScreen, (int (*)())&BattleFx_SpecialEnterSabotage, (int (*)())&BattleFx_EnterBuiltinPuff, (int (*)())&BattleFx_EnterBuiltinRingBurst, (int (*)())&Battle_ItemFxEnterSmokeBomb, (int (*)())&BattleFx_MagicEnterHaste, (int (*)())&BattleFx_BlueEnterBigGuard, (int (*)())&BattleFx_MagicEnterResist, (int (*)())&BattleFx_EnterBuiltinHitReaction, (int (*)())&BattleFx_SpecialEnterChainExplosion, (int (*)())&BattleFx_SpecialEnterFullExplosion, (int (*)())&BattleFx_MagicEnterStop, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_MagicEnterSlow, (int (*)())&BattleFx_MagicEnterSleepel, (int (*)())&BattleFx_MagicEnterSilence, (int (*)())&BattleFx_MagicEnterBerserk, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_MagicEnterFire2, (int (*)())&BattleFx_MagicEnterFire3Multi, (int (*)())&BattleFx_MagicEnterIce2, (int (*)())&BattleFx_MagicEnterIce3, (int (*)())&BattleFx_MagicEnterBolt2, (int (*)())&BattleFx_MagicEnterBolt3Multi, (int (*)())&BattleFx_MagicEnterQuake2, (int (*)())&BattleFx_MagicEnterQuake3, (int (*)())&BattleFx_MagicEnterBio2, (int (*)())&BattleFx_MagicEnterBio3Multi, (int (*)())&BattleFx_MagicEnterComet2, (int (*)())&BattleFx_SpecialEnterDrain, (int (*)())&BattleFx_SpecialEnterEnergySiphon, (int (*)())&BattleFx_MagicEnterBreak, (int (*)())&BattleFx_MagicEnterBarrier, (int (*)())&BattleFx_MagicEnterMBarrier, (int (*)())&BattleFx_MagicEnterReflect, (int (*)())&BattleFx_MagicEnterDespell, (int (*)())&BattleFx_SpecialEnterWhirlwindVar0, (int (*)())&BattleFx_BlueEnterAqualung, (int (*)())&BattleFx_MagicEnterToad, (int (*)())&BattleFx_MagicEnterMini, (int (*)())&BattleFx_MagicEnterEsuna, (int (*)())&BattleFx_MagicEnterFire, (int (*)())&BattleFx_SpecialEnterMortar, (int (*)())&BattleFx_SpecialEnterNapalm, (int (*)())&BattleFx_MagicEnterDemi, (int (*)())&BattleFx_MagicEnterDemi2, (int (*)())&BattleFx_SpecialEnterEvilPoison, (int (*)())&BattleFx_SpecialEnterThousandNeedles, (int (*)())&BattleFx_SpecialEnterRadiantBreath, (int (*)())&BattleFx_BlueEnterBadBreath, (int (*)())&Battle_ItemFxGreens0, (int (*)())&Battle_ItemFxGreens1, (int (*)())&Battle_ItemFxGreens2, (int (*)())&Battle_ItemFxGreens3, (int (*)())&Battle_ItemFxGreens4, (int (*)())&Battle_ItemFxGreens5, (int (*)())&Battle_ItemFxGreens6, (int (*)())&Battle_ItemFxGreens7};
// GLOBAL: FF7 0x008fe4b0
int (*g_BattleFx_MagicDispatchTable[54])() = {(int (*)())&BattleFx_BuiltinFlashSnd06, (int (*)())&BattleFx_BuiltinFlashSnd08, (int (*)())&BattleFx_BuiltinFlashSnd45, (int (*)())&BattleFx_BuiltinFlashSnd5B, (int (*)())&BattleFx_BuiltinFlashSnd5C, (int (*)())&BattleFx_MagicEnterEsuna, (int (*)())&BattleFx_MagicEnterResist, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_MagicEnterMini, (int (*)())&BattleFx_MagicEnterToad, (int (*)())&BattleFx_MagicEnterSleepel, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_MagicEnterSilence, (int (*)())&BattleFx_MagicEnterBerserk, (int (*)())&BattleFx_MagicEnterBarrier, (int (*)())&BattleFx_MagicEnterMBarrier, (int (*)())&BattleFx_MagicEnterReflect, (int (*)())&BattleFx_MagicEnterWall, (int (*)())&BattleFx_MagicEnterHaste, (int (*)())&BattleFx_MagicEnterSlow, (int (*)())&BattleFx_MagicEnterStop, (int (*)())&BattleFx_MagicEnterDeBarrier, (int (*)())&BattleFx_MagicEnterDespell, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_MagicEnterEscape, (int (*)())&BattleFx_MagicEnterRemove, (int (*)())&BattleFx_MagicEnterFire, (int (*)())&BattleFx_MagicEnterFire2, (int (*)())&BattleFx_MagicEnterFire3, (int (*)())&BattleFx_MagicEnterIce, (int (*)())&BattleFx_MagicEnterIce2, (int (*)())&BattleFx_MagicEnterIce3, (int (*)())&BattleFx_MagicEnterBolt, (int (*)())&BattleFx_MagicEnterBolt2, (int (*)())&BattleFx_MagicEnterBolt3, (int (*)())&BattleFx_MagicEnterQuake, (int (*)())&BattleFx_MagicEnterQuake2, (int (*)())&BattleFx_MagicEnterQuake3, (int (*)())&BattleFx_MagicEnterBio, (int (*)())&BattleFx_MagicEnterBio2, (int (*)())&BattleFx_MagicEnterBio3, (int (*)())&BattleFx_MagicEnterDemi, (int (*)())&BattleFx_MagicEnterDemi2, (int (*)())&BattleFx_MagicEnterDemi3, (int (*)())&BattleFx_MagicEnterComet, (int (*)())&BattleFx_MagicEnterComet2, (int (*)())&BattleFx_MagicEnterFreeze, (int (*)())&BattleFx_MagicEnterBreak, (int (*)())&BattleFx_MagicEnterTornado, (int (*)())&BattleFx_MagicEnterFlare, (int (*)())&BattleFx_MagicEnterFullCure, (int (*)())&BattleFx_MagicEnterUltima, (int (*)())&BattleFx_MagicEnterShield};
// GLOBAL: FF7 0x008fe588
int (*g_BattleFx_BlueMagicDispatchTable[24])() = {(int (*)())&BattleFx_BlueEnterFrogSong, (int (*)())&BattleFx_BlueEnterL4Suicide, (int (*)())&BattleFx_BlueEnterMagicHammer, (int (*)())&BattleFx_BlueEnterWhiteWind, (int (*)())&BattleFx_BlueEnterBigGuard, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_BlueEnterDragonForce, (int (*)())&BattleFx_BlueEnterDeathForce, (int (*)())&BattleFx_BlueEnterFlameThrower, (int (*)())&BattleFx_BlueEnterLaser, (int (*)())&BattleFx_BlueEnterMatraMagic, (int (*)())&BattleFx_BlueEnterBadBreath, (int (*)())&BattleFx_BlueEnterBeta, (int (*)())&BattleFx_BlueEnterAqualung, (int (*)())&BattleFx_BlueEnterTrine, (int (*)())&BattleFx_BlueEnterMagicBreath, (int (*)())&BattleFx_BlueEnterQuestionMarks, (int (*)())&BattleFx_BlueEnterGoblinPunch, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_BlueEnterL5Death, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_BlueEnterShadowFlare, (int (*)())&BattleFx_BlueEnterPandorasBox};
// GLOBAL: FF7 0x008fe5e8
int (*g_BattleFx_SpecialDispatchTable[158])() = {(int (*)())&BattleFx_SpecialEnterLightShell, (int (*)())&BattleFx_SpecialEnterFireball, (int (*)())&BattleFx_SpecialEnterSpiderWeb, (int (*)())&BattleFx_SpecialEnterRefuseSpit, (int (*)())&BattleFx_SpecialEnterLaserCannon, (int (*)())&BattleFx_SpecialEnterFourLaser, (int (*)())&BattleFx_SpecialEnterWaterGun, (int (*)())&BattleFx_SpecialEnterSeedShot, (int (*)())&BattleFx_SpecialEnterColdBreath, (int (*)())&BattleFx_SpecialEnterPoisonBreath, (int (*)())&BattleFx_SpecialEnterBadMouth, (int (*)())&BattleFx_SpecialEnterUltrasound, (int (*)())&BattleFx_SpecialEnterSandstorm, (int (*)())&BattleFx_SpecialEnterSwiftBolt, (int (*)())&BattleFx_SpecialEnterSilverWheel, (int (*)())&BattleFx_SpecialEnterOil, (int (*)())&BattleFx_SpecialEnterStareDown, (int (*)())&BattleFx_SpecialEnterSearchlight, (int (*)())&BattleFx_SpecialEnterHeatWave, (int (*)())&BattleFx_SpecialEnterRocketLauncher, (int (*)())&BattleFx_SpecialEnterToxicSmog, (int (*)())&BattleFx_SpecialEnterEvilPoison, (int (*)())&BattleFx_SpecialEnterBeamGun, (int (*)())&BattleFx_SpecialEnterDrain, (int (*)())&BattleFx_SpecialEnterSwampShoot, (int (*)())&BattleFx_SpecialEnterSephirothShock, (int (*)())&BattleFx_SpecialEnterBigWave, (int (*)())&BattleFx_SpecialEnterVacuum, (int (*)())&BattleFx_SpecialEnterSelfDestruct, (int (*)())&BattleFx_SpecialEnterSouthernCross, (int (*)())&BattleFx_MagicEnterFire, (int (*)())&BattleFx_SpecialEnterThousandNeedles, (int (*)())&BattleFx_SpecialEnterQuasar, (int (*)())&BattleFx_SpecialEnterAvalanche, (int (*)())&BattleFx_SpecialEnterFlyingSickle, (int (*)())&BattleFx_SpecialEnterAutumnLeaves, (int (*)())&BattleFx_SpecialEnterFallingRocks, (int (*)())&BattleFx_SpecialEnterFascination, (int (*)())&BattleFx_SpecialEnterBlaster, (int (*)())&BattleFx_SpecialEnterTriangleAttack, (int (*)())&BattleFx_SpecialEnterAtomicRay, (int (*)())&BattleFx_MagicEnterFire, (int (*)())&BattleFx_SpecialEnterEnergyWave, (int (*)())&BattleFx_SpecialEnterEnergyBall, (int (*)())&BattleFx_SpecialEnterHarrier, (int (*)())&BattleFx_MagicEnterFire, (int (*)())&BattleFx_SpecialEnterSpadeCard, (int (*)())&BattleFx_SpecialEnterHeartCard, (int (*)())&BattleFx_SpecialEnterClubCard, (int (*)())&BattleFx_SpecialEnterDiamondCard, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_SpecialEnterEmperorCard, (int (*)())&BattleFx_SpecialEnterHiddenOneCard, (int (*)())&BattleFx_SpecialEnterStarCard, (int (*)())&BattleFx_SpecialEnterSunCard, (int (*)())&BattleFx_SpecialEnterWorldCard, (int (*)())&BattleFx_SpecialEnterShamSeal, (int (*)())&BattleFx_SpecialEnterRiderBreath, (int (*)())&BattleFx_SpecialEnterFireShell, (int (*)())&BattleFx_SpecialEnterSnowstorm, (int (*)())&BattleFx_SpecialEnterLiquidPoison, (int (*)())&BattleFx_SpecialEnterWarCry, (int (*)())&BattleFx_SpecialEnterPollen, (int (*)())&BattleFx_SpecialEnterHellBomber, (int (*)())&BattleFx_SpecialEnterSmokeShot, (int (*)())&BattleFx_SpecialEnterEnergySiphon, (int (*)())&BattleFx_SpecialEnterGrandSpark, (int (*)())&BattleFx_SpecialEnterBombBlast, (int (*)())&BattleFx_SpecialEnterNorthernCross, (int (*)())&BattleFx_SpecialEnterWaveCannon, (int (*)())&BattleFx_BlueEnterFlameThrowerAttach, (int (*)())&BattleFx_SpecialEnterTwinBurner, (int (*)())&BattleFx_SpecialEnterSewerTsunami, (int (*)())&BattleFx_SpecialEnterSewerTsunamiAlt, (int (*)())&BattleFx_SpecialEnterWaterball, (int (*)())&BattleFx_SpecialEnterSearchScope, (int (*)())&BattleFx_SpecialEnterTailLaser, (int (*)())&BattleFx_SpecialEnterShadyBreath, (int (*)())&BattleFx_SpecialEnterIcicleDrop, (int (*)())&BattleFx_SpecialEnterHellBubbles, (int (*)())&BattleFx_SpecialEnterAcidShower, (int (*)())&BattleFx_SpecialEnterGeyser, (int (*)())&BattleFx_SpecialEnterHotSprings, (int (*)())&BattleFx_SpecialEnterMagma, (int (*)())&BattleFx_SpecialEnterCoinToss, (int (*)())&BattleFx_SpecialEnterWhirlwindVar1, (int (*)())&BattleFx_SpecialEnterWhirlwindVar0, (int (*)())&BattleFx_SpecialEnterPetrify, (int (*)())&BattleFx_SpecialEnterDualMortar, (int (*)())&BattleFx_SpecialEnterMortar, (int (*)())&BattleFx_SpecialEnterSabotage, (int (*)())&BattleFx_SpecialEnterDoubleLaser, (int (*)())&BattleFx_SpecialEnterBuiltinGust, (int (*)())&BattleFx_SpecialEnterFullExplosion, (int (*)())&BattleFx_BuiltinFlashSnd166, (int (*)())&BattleFx_SpecialEnterFreezeBreath, (int (*)())&BattleFx_SpecialEnterFlameThrowerVar1, (int (*)())&BattleFx_SpecialEnterFlameThrowerVar2, (int (*)())&BattleFx_SpecialEnterColdAir, (int (*)())&BattleFx_SpecialEnterLightBlast3, (int (*)())&BattleFx_SpecialEnterLightBlast2, (int (*)())&BattleFx_SpecialEnterEnergy, (int (*)())&BattleFx_SpecialEnterHeatBlast, (int (*)())&BattleFx_SpecialEnterWhirlpool, (int (*)())&BattleFx_SpecialEnterDarkWave, (int (*)())&BattleFx_SpecialEnterMissileSalvo, (int (*)())&BattleFx_SpecialEnterRadiantBreath, (int (*)())&BattleFx_SpecialEnterNapalm, (int (*)())&BattleFx_SpecialEnterPoisonGas, (int (*)())&BattleFx_SpecialEnterScorch, (int (*)())&BattleFx_SpecialEnterEnergyOrb, (int (*)())&BattleFx_SpecialEnterWingScales, (int (*)())&BattleFx_SpecialEnterHeartBreath, (int (*)())&BattleFx_SpecialEnterJammer, (int (*)())&BattleFx_EnterBuiltinPuff, (int (*)())&BattleFx_SpecialEnterElectricShock, (int (*)())&BattleFx_SpecialEnterSirenSong, (int (*)())&BattleFx_SpecialEnterUltimateBlast, (int (*)())&BattleFx_SpecialEnterMagmaEruption, (int (*)())&BattleFx_SpecialEnterTimeDamage, (int (*)())&BattleFx_SpecialEnterChefsKnife, (int (*)())&BattleFx_SpecialEnterChainExplosion, (int (*)())&BattleFx_SpecialEnterPoisonBreathLoud, (int (*)())&BattleFx_SpecialEnterPollenAlt, (int (*)())&BattleFx_SpecialEnterSmog, (int (*)())&BattleFx_MagicEnterHitmark, (int (*)())&BattleFx_SpecialEnterFrog, (int (*)())&BattleFx_SpecialEnterShield, (int (*)())&BattleFx_SpecialEnterDualShield, (int (*)())&BattleFx_SpecialEnterRocketPunch, (int (*)())&BattleFx_SpecialEnterDualRocketPunch, (int (*)())&BattleFx_SpecialEnterFlameThrowerLeft, (int (*)())&BattleFx_SpecialEnterGiantSonicWave, (int (*)())&BattleFx_MagicEnterQuake, (int (*)())&BattleFx_MagicEnterQuake2, (int (*)())&BattleFx_MagicEnterQuake3, (int (*)())&BattleFx_MagicEnterBolt2, (int (*)())&BattleFx_SpecialEnterMultiJammer, (int (*)())&BattleFx_BlueEnterFlameThrower, (int (*)())&BattleFx_SpecialEnterFlameThrowerFang, (int (*)())&BattleFx_SpecialEnterSpreadLaser, (int (*)())&BattleFx_SpecialEnterSuperOrb, (int (*)())&BattleFx_SpecialEnterBizarroEnergy, (int (*)())&BattleFx_WeaponEnterSuperNova, (int (*)())&Battle_EffectAttachStatusOverlayModel, (int (*)())&BattleFx_SpecialEnterHeartlessAngel, (int (*)())&BattleFx_SpecialEnterBoulderThrow, (int (*)())&BattleFx_SpecialEnterLastBossStage, (int (*)())&BattleFx_BuiltinFlashSnd45, (int (*)())&BattleFx_WeaponEnterWhirlsand, (int (*)())&BattleFx_WeaponEnterRubyTentaclesWithCameraScript, (int (*)())&BattleFx_WeaponEnterRubyTentaclesNoCameraScript, (int (*)())&BattleFx_WeaponEnterEmeraldShoot, (int (*)())&BattleFx_WeaponEnterEmeraldBeamVariant0, (int (*)())&BattleFx_WeaponEnterRevengeStamp, (int (*)())&BattleFx_WeaponEnterDiamondFlash, (int (*)())&BattleFx_WeaponEnterEmeraldBeamVariant1, 0};
// GLOBAL: FF7 0x008fe860
int (*g_BattleFx_LimitDispatchTable[80])() = {(int (*)())&Battle_LimitFx00_Braver, (int (*)())&Battle_LimitFx01_CrossSlash, (int (*)())&BattleFx_LimitEnterBladeBeam, (int (*)())&BattleFx_LimitEnterClimhazzard, (int (*)())&BattleFx_LimitEnterMeteorain, (int (*)())&BattleFx_LimitEnterFinishingTouch, (int (*)())&BattleFx_LimitEnterOmnislash, (int (*)())&Battle_LimitFx07_BigShot, (int (*)())&BattleFx_LimitEnterGrenadeBomb, (int (*)())&BattleFx_LimitEnterMindblow, (int (*)())&BattleFx_LimitEnterHammerblow, (int (*)())&BattleFx_LimitEnterSatelliteBeam, (int (*)())&BattleFx_LimitEnterUngarmax, (int (*)())&BattleFx_LimitEnterCatastrophe, (int (*)())&Battle_LimitFx14_HealingWind, (int (*)())&BattleFx_LimitEnterSealEvil, (int (*)())&BattleFx_LimitEnterBreathOfTheEarth, (int (*)())&BattleFx_LimitEnterFuryBrand, (int (*)())&BattleFx_LimitEnterPlanetProtector, (int (*)())&Battle_LimitFx19_PulseOfLife, (int (*)())&BattleFx_LimitEnterGreatGospel, (int (*)())&Battle_LimitFx21_LimFast, 0, 0, 0, 0, 0, 0, (int (*)())&BattleFx_LimitEnterBoostJump, (int (*)())&BattleFx_LimitEnterDragon, (int (*)())&BattleFx_LimitEnterHyperJump, (int (*)())&BattleFx_LimitEnterDynamite, (int (*)())&BattleFx_LimitEnterDragonDive, (int (*)())&BattleFx_LimitEnterBigBrawl, (int (*)())&BattleFx_LimitEnterHighwind, (int (*)())&BattleFx_LimitEnterSledFang, (int (*)())&BattleFx_LimitEnterHowlingMoon, (int (*)())&BattleFx_LimitEnterBloodFang, (int (*)())&BattleFx_LimitEnterStardustRay, (int (*)())&BattleFx_LimitEnterLunaticHigh, (int (*)())&BattleFx_LimitEnterEarthRave, (int (*)())&BattleFx_LimitEnterCosmoMemory, (int (*)())&Battle_LimitFx42_Dice, 0, 0, 0, 0, 0, 0, (int (*)())&BattleFx_LimitEnterGreasedLightning, (int (*)())&BattleFx_LimitEnterClearTranquil, (int (*)())&BattleFx_LimitEnterLandscaper, (int (*)())&BattleFx_LimitEnterBloodfest, (int (*)())&BattleFx_LimitEnterGauntlet, (int (*)())&BattleFx_LimitEnterDoomOfTheLiving, (int (*)())&BattleFx_LimitEnterAllCreation, (int (*)())&BattleFx_EnterGrowActorScale, (int (*)())&Battle_LimitFxStatusOverlayThunk, (int (*)())&Battle_LimitFxStatusOverlayThunk, (int (*)())&BattleFx_LimitEnterLuckyGirl, (int (*)())&Battle_LimitFxThunk60, (int (*)())&Battle_LimitFxStatusOverlayThunk, (int (*)())&BattleFx_LimitEnterGalianBeast, (int (*)())&BattleFx_LimitEnterGalianBeast, (int (*)())&BattleFx_LimitEnterGalianBeast, (int (*)())&BattleFx_LimitEnterLivewire, (int (*)())&BattleFx_LimitEnterGalianBeast, (int (*)())&BattleFx_LimitEnterNightmare, (int (*)())&BattleFx_LimitEnterGalianBeast, (int (*)())&BattleFx_LimitEnterSatanSlam, (int (*)())&Battle_LimitFxThunk70, (int (*)())&Battle_LimitFxThunk71, (int (*)())&BattleFx_LimitEnterToyBoxB, (int (*)())&BattleFx_LimitEnterToyBox, (int (*)())&BattleFx_LimitEnterToyBoxC, (int (*)())&BattleFx_LimitEnterToyBoxD, (int (*)())&Battle_LimitFxThunk76, (int (*)())&BattleFx_LimitEnterFinishingTouchAlt, (int (*)())&BattleFx_LimitEnterSatanSlamAlt, (int (*)())&BattleFx_LimitEnterBladeBeamAlt};
// GLOBAL: FF7 0x008fe9a0
int (*g_BattleFx_ThrowWeaponDispatchTable[128])() = {(int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00Index1, (int (*)())&Battle_ThrowFxWeaponBlock00Index2, (int (*)())&Battle_ThrowFxWeaponBlock00Index3, (int (*)())&Battle_ThrowFxWeaponBlock00Index4, (int (*)())&Battle_ThrowFxWeaponBlock00Index5, (int (*)())&Battle_ThrowFxWeaponBlock00Index6, (int (*)())&Battle_ThrowFxWeaponBlock00Index7, (int (*)())&Battle_ThrowFxWeaponBlock00Index8, (int (*)())&Battle_ThrowFxWeaponBlock00Index9, (int (*)())&Battle_ThrowFxWeaponBlock00Index10, (int (*)())&Battle_ThrowFxWeaponBlock00Index11, (int (*)())&Battle_ThrowFxWeaponBlock00Index12, (int (*)())&Battle_ThrowFxWeaponBlock00Index13, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock16, (int (*)())&Battle_ThrowFxWeaponBlock32, (int (*)())&Battle_ThrowFxWeaponBlock32Index1, (int (*)())&Battle_ThrowFxWeaponBlock32, (int (*)())&Battle_ThrowFxWeaponBlock32Index2, (int (*)())&Battle_ThrowFxWeaponBlock32Index3, (int (*)())&Battle_ThrowFxWeaponBlock32Index4, (int (*)())&Battle_ThrowFxWeaponBlock32Index5, (int (*)())&Battle_ThrowFxWeaponBlock32Index6, (int (*)())&Battle_ThrowFxWeaponBlock32Index7, (int (*)())&Battle_ThrowFxWeaponBlock32Index8, (int (*)())&Battle_ThrowFxWeaponBlock32Index9, (int (*)())&Battle_ThrowFxWeaponBlock32Index10, (int (*)())&Battle_ThrowFxWeaponBlock32Index11, (int (*)())&Battle_ThrowFxWeaponBlock32Index12, (int (*)())&Battle_ThrowFxWeaponBlock32Index13, (int (*)())&Battle_ThrowFxWeaponBlock32Index14, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock00, (int (*)())&Battle_ThrowFxWeaponBlock62, (int (*)())&Battle_ThrowFxWeaponBlock62Index1, (int (*)())&Battle_ThrowFxWeaponBlock62Index2, (int (*)())&Battle_ThrowFxWeaponBlock62Index3, (int (*)())&Battle_ThrowFxWeaponBlock62Index4, (int (*)())&Battle_ThrowFxWeaponBlock62Index5, (int (*)())&Battle_ThrowFxWeaponBlock62Index6, (int (*)())&Battle_ThrowFxWeaponBlock62Index7, (int (*)())&Battle_ThrowFxWeaponBlock62Index8, (int (*)())&Battle_ThrowFxWeaponBlock62Index9, (int (*)())&Battle_ThrowFxWeaponBlock62Index10, (int (*)())&Battle_ThrowFxWeaponBlock73, (int (*)())&Battle_ThrowFxWeaponBlock73Index1, (int (*)())&Battle_ThrowFxWeaponBlock73Index2, (int (*)())&Battle_ThrowFxWeaponBlock73Index3, (int (*)())&Battle_ThrowFxWeaponBlock73Index4, (int (*)())&Battle_ThrowFxWeaponBlock73Index5, (int (*)())&Battle_ThrowFxWeaponBlock73Index6, (int (*)())&Battle_ThrowFxWeaponBlock73Index7, (int (*)())&Battle_ThrowFxWeaponBlock73Index8, (int (*)())&Battle_ThrowFxWeaponBlock73Index9, (int (*)())&Battle_ThrowFxWeaponBlock73Index10, (int (*)())&Battle_ThrowFxWeaponBlock73Index11, (int (*)())&Battle_ThrowFxWeaponBlock73Index12, (int (*)())&Battle_ThrowFxWeaponBlock73Index13, (int (*)())&Battle_ThrowFxWeaponBlock87, (int (*)())&Battle_ThrowFxWeaponBlock87Index1, (int (*)())&Battle_ThrowFxWeaponBlock87Index2, (int (*)())&Battle_ThrowFxWeaponBlock87Index3, (int (*)())&Battle_ThrowFxWeaponBlock87Index4, (int (*)())&Battle_ThrowFxWeaponBlock87Index5, (int (*)())&Battle_ThrowFxWeaponBlock87Index6, (int (*)())&Battle_ThrowFxWeaponBlock87Index7, (int (*)())&Battle_ThrowFxWeaponBlock87Index8, (int (*)())&Battle_ThrowFxWeaponBlock87Index9, (int (*)())&Battle_ThrowFxWeaponBlock87Index10, (int (*)())&Battle_ThrowFxWeaponBlock87Index11, (int (*)())&Battle_ThrowFxWeaponBlock87Index12, (int (*)())&Battle_ThrowFxWeaponBlock87Index13, (int (*)())&Battle_ThrowFxWeaponBlock101, (int (*)())&Battle_ThrowFxWeaponBlock101Index1, (int (*)())&Battle_ThrowFxWeaponBlock101Index2, (int (*)())&Battle_ThrowFxWeaponBlock101Index3, (int (*)())&Battle_ThrowFxWeaponBlock101Index4, (int (*)())&Battle_ThrowFxWeaponBlock101Index5, (int (*)())&Battle_ThrowFxWeaponBlock101Index6, (int (*)())&Battle_ThrowFxWeaponBlock101Index7, (int (*)())&Battle_ThrowFxWeaponBlock101Index8, (int (*)())&Battle_ThrowFxWeaponBlock101Index9, (int (*)())&Battle_ThrowFxWeaponBlock101Index10, (int (*)())&Battle_ThrowFxWeaponBlock101Index11, (int (*)())&Battle_ThrowFxWeaponBlock101Index12, (int (*)())&Battle_ThrowFxWeaponBlock114, (int (*)())&Battle_ThrowFxWeaponBlock114Index1, (int (*)())&Battle_ThrowFxWeaponBlock114Index2, (int (*)())&Battle_ThrowFxWeaponBlock114Index3, (int (*)())&Battle_ThrowFxWeaponBlock114Index4, (int (*)())&Battle_ThrowFxWeaponBlock114Index5, (int (*)())&Battle_ThrowFxWeaponBlock114Index6, (int (*)())&Battle_ThrowFxWeaponBlock114Index7, (int (*)())&Battle_ThrowFxWeaponBlock114Index8, (int (*)())&Battle_ThrowFxWeaponBlock114Index9, (int (*)())&Battle_ThrowFxWeaponBlock114Index10, (int (*)())&Battle_ThrowFxWeaponBlock114Index11, (int (*)())&Battle_ThrowFxWeaponBlock114Index12, 0};
// GLOBAL: FF7 0x00c06008
int g_BattleVmSlotStorage_0 = 0;
// GLOBAL: FF7 0x00c0600c
int g_BattleVmSlotStorage_1 = 0;
// GLOBAL: FF7 0x00c06010
int g_BattleVmSlotStorage_2 = 0;
// GLOBAL: FF7 0x00c06014
int g_BattleVmSlotStorage_3 = 0;
// GLOBAL: FF7 0x00c06018
int g_BattleVmSlotStorage_4 = 0;
// GLOBAL: FF7 0x00c0601c
int g_BattleVmSlotStorage_5 = 0;
// GLOBAL: FF7 0x00c06020
int g_BattleVmSlotStorage_6 = 0;
// GLOBAL: FF7 0x00c06024
int g_BattleVmSlotStorage_7 = 0;
// GLOBAL: FF7 0x00c06028
unsigned char g_BattleActorAnimStateBuffer[34] = {0};
unsigned char g_bssPad_00c0604c[4] = {0};
unsigned char g_bssPad_00c06050[984] = {0};
// GLOBAL: FF7 0x00c06428
unsigned char g_BattleRingBufferTags[64] = {0};
// GLOBAL: FF7 0x00c06468
unsigned char g_BattleRingBufferRecords[320] = {0};
// GLOBAL: FF7 0x00c065a8
int g_BattleRingBufferWriteIndex = 0;
// GLOBAL: FF7 0x00c065ac
int g_BattleRingBufferReadIndex = 0;
// GLOBAL: FF7 0x00c065b0
struct BattleScreenFlashRamp *g_BattleScreenFlashRampTaskHandle = 0;
// GLOBAL: FF7 0x00c065b4
struct BattleFixedPointRamp *g_BattleFixedPointRampTaskHandle = 0;
// GLOBAL: FF7 0x00c065b8
char g_BattleLimitFlashModel2Started = 0;
// GLOBAL: FF7 0x00c065bc
int g_BattleLimitFlashModel0Data = 0;
// GLOBAL: FF7 0x00c065c0
char g_BattleLimitFlashModel0Started = 0;
// GLOBAL: FF7 0x00c065c4
int g_BattleLimitFlashModel1Data = 0;
// GLOBAL: FF7 0x00c065c8
int g_BattleLimitFlashModel2Data = 0;
// GLOBAL: FF7 0x00c065cc
char g_BattleLimitFlashModel1Started = 0;
// GLOBAL: FF7 0x00c065d0
struct GfxTableCursor *g_pBattleLimitFlashTable = 0;
unsigned char g_bssPad_00c065d4[4] = {0};
// GLOBAL: FF7 0x00c065d8
short g_BattleLimitFlashModel1Matrix = 0;
unsigned char g_bssPad_00c065dc[4] = {0};
// GLOBAL: FF7 0x00c065e0
short g_BattleLimitFlashModel1Scale = 0;
unsigned char g_bssPad_00c065e4[4] = {0};
// GLOBAL: FF7 0x00c065e8
struct BattleLimitFlashModel1MatrixTail g_BattleLimitFlashModel1MatrixTail = {0};
// GLOBAL: FF7 0x00c065f8
short g_BattleLimitFlashModel2Matrix = 0;
unsigned char g_bssPad_00c065fc[4] = {0};
// GLOBAL: FF7 0x00c06600
short g_BattleLimitFlashModel2Scale = 0;
unsigned char g_bssPad_00c06604[4] = {0};
// GLOBAL: FF7 0x00c06608
struct BattleLimitFlashModel1MatrixTail g_BattleLimitFlashModel2MatrixTail = {0};
unsigned char g_bssPad_00c06618[4] = {0};
// GLOBAL: FF7 0x00c0661c
char g_BattleCameraFocusScriptActive = 0;
// clang-format on
// Data only: the battle effect dispatch tables, the battle VM slot words, the ring buffer and the limit flash state.
} /* extern "C" */

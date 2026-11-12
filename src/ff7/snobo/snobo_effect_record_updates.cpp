// src/ff7/snobo/snobo_effect_record_updates.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The per-frame update handlers for the snowboard effect records, covering sway, fall,
//   ballistic arcs, hit launches, pickups, linked blinks and child spawns. Each handler ages its
//   record, moves it, and retires it when it falls behind the rider or its fade completes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: The snowboard effect trigger handlers, one per script slot. Each spawns or queues one
//   or more effects for an object when its spawn flag or gate is set, with a few that also set the
//   rider animation or a flag.
#include "ff7/types/snobo.h"
#include <stdlib.h>
#include "ff7/api/snobo.h"
#include "lib/api/graphics.h"
#include "ff7/enums/snobo.h"
#include "ff7/snobo_course.h"
#include "lib/types/graphics.h"

#pragma intrinsic(abs)
extern "C" {
// clang-format off
// The snowboard effect templates: thirteen 72-byte records, each the argument of its spawn sites to
// Mini_SnowboardAllocEffectNode, which copies the template whole into the node, and between the
// fifth and the sixth the frame table of Mini_SnowboardEffectUpdateFrameCycle. A template's update
// member is the node's update callback; the spawn sites stage the position (sidePos, height,
// forward), the callback or the effect id (homeX) before the copy.
// GLOBAL: FF7 0x00962b60
SnowboardEffectTemplate g_SnoboEffectDef = {0, 0, 0, 0, 0, 0, 0, 0, 0, {0xFF, 0xFF, 0xFF, 0xFF}, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962ba8
SnowboardEffectTemplate g_SnoboEffectDefSmall = {0, 0, 0, 0, 0, 0, 96, 256, 192, {0x00, 0x00, 0x80, 0x80}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectHitLaunchWithSignal, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962bf0
SnowboardEffectTemplate g_SnoboEffectDefLarge = {0, 0, 1, 0, 0, 0, 152, 512, 448, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateSwayOnHit, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962c38
SnowboardEffectTemplate g_SnoboLateralDriftEffectDef = {0xD1, 0xD2, 0, 0, 0, 0, 0, 128, 96, {0x00, 0x00, 0x80, 0x0A}, -170, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectStartLateralDrift, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962c80
SnowboardEffectTemplate g_SnoboTumbleBounceEffectDef = {0xD5, 0, 0, 0, 0, 0, 64, 64, 64, {0x00, 0x00, 0x0A, 0x0A}, -170, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateFrameCycle, 0, 0, 0, 0};
// The four frames Mini_SnowboardEffectUpdateFrameCycle cycles through, indexed by (age >> 2) & 3.
// GLOBAL: FF7 0x00962cc8
short g_SnoboEffectAnimFrames[4] = {213, 214, 213, 215};
// GLOBAL: FF7 0x00962cd0
SnowboardEffectTemplate g_SnoboApproachFollowEffectDef = {0xD1, 0xD2, 0, 0, 0, 0, 0, 128, 96, {0x00, 0x00, 0x80, 0x0A}, -170, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectStartApproachFollow, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962d18
SnowboardEffectTemplate g_SnoboLateralDriftSpawnChildEffectDef = {0xDF, 0xE0, 0, 0, 0, 0, 0, 128, 96, {0x00, 0x00, 0x80, 0x0A}, -170, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectStartLateralDriftSpawnChild, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962d60
SnowboardEffectTemplate g_SnoboHitTumbleChildEffectDef = {0xE1, 0, 0, 0, 0, 0, 0, 0, 0, {0x00, 0x00, 0x00, 0x00}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectFallFadeRetireNoSkip, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962da8
SnowboardEffectTemplate g_SnoboApproachFollowSpawnChildEffectDef = {0xDF, 0xE0, 0, 0, 0, 0, 0, 128, 96, {0x00, 0x00, 0x80, 0x0A}, -170, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectStartApproachFollowSpawnChild, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962df0
SnowboardEffectTemplate g_SnoboSplitBallisticEffectDef = {0xBC, 0, 0, 0, 0, 0, 256, 128, 96, {0x00, 0x00, 0x81, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectOnHitSplitBallisticFadeOut, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962e38
SnowboardEffectTemplate g_SnoboLinkedBlinkEffectDef = {0, 0, 0, 0, 0, 0, 0, 0, 0, {0x00, 0x00, 0x80, 0x80}, -170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962e80
SnowboardEffectTemplate g_SnoboLandingFrameCycleEffectDef = {0xD1, 0xD0, 0, 0, 0, 0, 64, 128, 96, {0x00, 0x00, 0x0C, 0x0A}, -170, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateAirborneLandingFrameCycle, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962ec8
SnowboardEffectTemplate g_SnoboLandingHitLaunchEffectDef = {0xD1, 0xD0, 0, 0, 0, 0, 64, 128, 96, {0x00, 0x00, 0x80, 0x0A}, -170, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateAirborneLandingHitLaunch, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962f10
SnowboardEffectTemplate g_SnoboLinkedEffectDef = {0xE9, 0, 0, 0, 0, 0, 96, 192, 160, {0x00, 0x00, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdatePairedLaunchOrSwing, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962f58
SnowboardEffectTemplate g_SnoboLinkedEffectTopDef = {0xC9, 0xCB, 0, 0, 0, 0, 0, 0, 0, {0x00, 0x00, 0x00, 0x00}, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962fa0
unsigned char g_SnoboCycleFrameIds[16] = {0xcb, 0x00, 0xcc, 0x00, 0xcd, 0x00, 0xcb, 0x00, 0xce, 0x00, 0xcf, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00962fb0
SnowboardEffectTemplate g_SnoboHopWaitEffectDef = {0, 0xE5, 0, 0, 0, 0, 64, 320, 256, {0x00, 0x00, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateHopWait, 0, 0, 0, 0};
// GLOBAL: FF7 0x00962ff8
SnowboardEffectTemplate g_SnoboAboveObjectEffectDef = {0xB8, 0, 0, 0, 2, 0, 136, 192, 0, {0xFF, 0xFF, 0xFF, 0xFF}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdatePickup, 0, 0, 0, 0};
// The score Mini_SnowboardEffectUpdatePickup adds to pickupTotal, indexed by the pickup's kind
// (kinds 3 and 4 take other paths). The span runs to the next named definition of the TU.
// GLOBAL: FF7 0x00963040
short g_SnoboPickupScoreByKind[8] = {1, 3, 5, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00963050
SnowboardEffectTemplate g_SnoboHitTiltNegEffectDef = {0, 0, 0, 0, 2, 0, 64, 512, 448, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectHitStartTiltNeg, 0, 0, 0, 0};
// GLOBAL: FF7 0x00963098
SnowboardEffectTemplate g_SnoboHitTiltPosEffectDef = {0, 0, 0, 0, 2, 0, 64, 512, 448, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectHitStartTiltPos, 0, 0, 0, 0};
// GLOBAL: FF7 0x009630e0
SnowboardEffectTemplate g_SnoboEffectTemplateAtObject = {0, 0, 0, 0, 2, 0, 0, 0, 0, {0x00, 0x00, 0x00, 0x00}, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00963128
SnowboardEffectTemplate g_SnoboPlainEffectDef = {0xF0, 0, 0, 0, 0, 0, 128, 368, 320, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectHitPlaySoundFade, 0, 0, 0, 0};
// The 72-byte tile effect template whose header words Mini_SnowboardEffect* populates
// before spawning an effect instance.
// GLOBAL: FF7 0x00963170
SnowboardEffectTemplate g_SnoboTileEffectDef = {0, 0, 0, 0, 0, 0, 0, 512, 480, {0x00, 0x00, 0x0C, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectHitPlaySoundFade, 0, 0, 0, 0};
// GLOBAL: FF7 0x009631b8
SnowboardEffectTemplate g_SnoboStepLaunchEffectDef = {0xF4, 0, 0, 0, 0, 0, 64, 192, 160, {0x00, 0x00, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectStepOnHitSelectLaunch, 0, 0, 0, 0};
// GLOBAL: FF7 0x00963200
SnowboardEffectTemplate g_SnoboRaisedEffectDef = {0, 0, 0, 0, 3, 0, 0, 0, 0, {0x00, 0x00, 0x0C, 0x0C}, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// GLOBAL: FF7 0x00963248
SnowboardEffectTemplate g_SnoboCircularOrbitEffectDef = {0xF5, 0, 0, 0, 3, 0, 96, 192, 160, {0x00, 0x00, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateCircularOrbit, 0, 0, 0, 32};
// GLOBAL: FF7 0x00963290
SnowboardEffectTemplate g_SnoboVerticalBobEffectDef = {0xF5, 0, 0, 0, 3, 0, 96, 192, 160, {0x00, 0x00, 0x80, 0x0A}, 0, 0, 0, 0, 0, 0, 0, 0, 0, (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateVerticalBob, 0, 0, 0, 0};
// GLOBAL: FF7 0x009632d8
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot003 = {{0x800, 767, 119, -254}, {3}};
// GLOBAL: FF7 0x009632e8
struct SnowboardEffectDesc g_SnoboNodeOffsetsDesc_Slot012 = {{0x400, 0, 0, 0}, {235, 247}};
// GLOBAL: FF7 0x009632f8
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot012 = {{0xC00, 0, 0, -768}, {4}};
// GLOBAL: FF7 0x00963308
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot018 = {{0xC00, -1025, -693, -768}, {1}};
// GLOBAL: FF7 0x00963318
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot022[2] = {{3044, -235, 155, -768}, {0}};
// GLOBAL: FF7 0x00963328
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot029[2] = {{2133, 720, 179, -701}, {0}};
// GLOBAL: FF7 0x00963338
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot035[2] = {{3158, -703, 178, -723}, {0}};
// GLOBAL: FF7 0x00963348
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot040[2] = {{4011, -881, 179, 494}, {0}};
// GLOBAL: FF7 0x00963358
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot048[2] = {{4049, -776, 67, 223}, {0}};
// clang-format on

// Prototypes of callees the C sources used implicitly (definitions elsewhere), required in C++.

// If the flags byte of record is nonzero, signal is copied to g_SnoboSignalValue and a callback is
// selected. With bit 2 set, sideVelocity, fallVelocity and forwardVelocity are set from hitVelocityX,
// 0xFFFD8000 and hitVelocityZ, sound 0xF0 plays on channel 3, contactRadius is cleared, and update
// selects Mini_SnowboardEffectUpdateLaunchArc. Without bit 2, update selects
// Mini_SnowboardEffectRetireAfterDelay.
// FUNCTION: FF7 0x00735400
void __cdecl Mini_SnowboardEffectHitLaunchWithSignal(SnowboardEffect *record)
{
    if (record->contact != 0) {
        g_SnoboSignalValue = record->section;
        if ((record->contact & 2) != 0) {
            record->sideVelocity = record->hitVelocityX;
            record->fallVelocity = 0xFFFD8000;
            record->forwardVelocity = record->hitVelocityZ;
            Mini_SnowboardPlaySfxOnChannel(3, 0xF0, 0);
            record->contactRadius = 0;
            record->update = Mini_SnowboardEffectUpdateLaunchArc;
        } else {
            record->update = Mini_SnowboardEffectRetireAfterDelay;
        }
    }
}

/*
 * Post-increments the signed age. Only when its consumed value exceeds 60 does it post-
 * increment the unsigned fade counter; clears the active byte when that consumed value
 * exceeds 30.
 */
// FUNCTION: FF7 0x00735482
void __cdecl Mini_SnowboardEffectRetireAfterDelay(SnowboardEffect *record)
{
    if ((record->age)++ > 60 && (record->fade)++ > 30)
        record->active = 0;
}

// Per-frame update for one 0x8C-byte effect record. When height has passed heightLimit, active is
// cleared to retire the record. Otherwise section is copied from g_SnoboStatePtr, sidePos, height
// and forward advance by sideVelocity, fallVelocity and forwardVelocity, fallVelocity gains a
// constant 0x1000 of gravity, rotX drops by 0x20, matrixValid is zeroed, and age increments once
// fallVelocity is positive.
// FUNCTION: FF7 0x007354dd
void __cdecl Mini_SnowboardEffectUpdateLaunchArc(struct SnowboardEffect *record)
{
    if (record->height > record->heightLimit) {
        record->active = 0;
    } else {
        record->section = g_SnoboStatePtr->section;
        record->sidePos += record->sideVelocity;
        record->height += record->fallVelocity;
        record->forward += record->forwardVelocity;
        record->fallVelocity += 4096;
        record->rotX -= 0x20;
        record->matrixValid = 0;
        if (record->fallVelocity > 0) {
            (record->fade)++;
        }
    }
}

/*
 * For a nonzero contact byte, bit 2 clears contactRadius, plays channel-3 sound 0xF0 and selects
 * Mini_SnowboardEffectUpdateSwayFade; otherwise selects Mini_SnowboardEffectRetireAfterDelay. Independently calls Mini_SnowboardEffectStepSwayRotation when mode
 * equals 2.
 */
// FUNCTION: FF7 0x00735593
void __cdecl Mini_SnowboardEffectUpdateSwayOnHit(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(3, 0xF0, 0);
            effect->update = Mini_SnowboardEffectUpdateSwayFade;
        } else {
            effect->update = Mini_SnowboardEffectRetireAfterDelay;
        }
    }
    if (effect->state == 2) {
        Mini_SnowboardEffectStepSwayRotation(effect);
    }
}

/*
 * Writes a nested fixed-point sine of the signed age to rotX and clears matrixValid. Then post-
 * increments age; only after its consumed value exceeds 120 does it post-increment unsigned fade
 * and clear the active byte if the consumed fade exceeds 30.
 */
// FUNCTION: FF7 0x007355f8
void __cdecl Mini_SnowboardEffectUpdateSwayFade(SnowboardEffect *effect)
{
    effect->rotX = -SinFixed(SinFixed(effect->age << 5) >> 4) >> 3;
    effect->matrixValid = 0;
    if ((effect->age)++ > 120 && (effect->fade)++ > 30)
        effect->active = 0;
}

// Advances the sway of a snowboard effect. swayAngle is set by SinFixed applied to SinFixed of
// swayCounter, which is post-incremented and shifted left five bits, with the inner result shifted
// right four and the outer result shifted right three. matrixValid is then cleared. This is a mechanical
// periodic field update.
// FUNCTION: FF7 0x0073568a
void __cdecl Mini_SnowboardEffectStepSwayRotation(SnowboardEffect *effect)
{

    effect->rotZ = SinFixed(SinFixed(((effect->timer)++ << 5)) >> 4) >> 3;
    effect->matrixValid = 0;
}

/*
 * Leaves an effect unchanged while its forward distance exceeds 0x700000. Otherwise chooses half
 * rider forward speed with minimum 0x8000, sets signed lateral speed according to its side flag,
 * resets timer/size fields and installs Mini_SnowboardEffectUpdateLateralDrift as callback.
 */
// FUNCTION: FF7 0x007356e5
void __cdecl Mini_SnowboardEffectStartLateralDrift(SnowboardEffect *effect)
{
    int speed;
    int lateralSpeed;

    if (effect->forward - (int)g_SnoboStatePtr->riderPos > 7340032) {
        return;
    }
    speed = (int)g_SnoboStatePtr->velocityZ >> 1;
    if (speed < 32768) {
        speed = 0x8000;
    }
    effect->forwardVelocity = speed;
    if (effect->sideFlag != 0) {
        lateralSpeed = -speed;
    } else {
        lateralSpeed = speed;
    }
    effect->sideVelocity = lateralSpeed;
    effect->fade = 0;
    effect->contactRadius = 64;
    effect->update = Mini_SnowboardEffectUpdateLateralDrift;
}

/*
 * Dispatches flagged effects to the collision helper. Otherwise removes effects more than 0x200
 * units behind, advances horizontal/forward coordinates and samples course height. Beyond side-
 * dependent lateral thresholds applies rotation and negative vertical velocity, changing to the
 * falling callback past the outer threshold.
 */
// FUNCTION: FF7 0x00735785
void __cdecl Mini_SnowboardEffectUpdateLateralDrift(SnowboardEffect *effect)
{
    int distance;

    if (effect->contact != 0) {
        Mini_SnowboardEffectBeginHitTumble(effect);
    } else {
        distance = (int)g_SnoboStatePtr->riderPos - effect->forward >> 12;
        if (distance > 512) {
            effect->active = 0;
        } else {
            effect->sidePos += effect->sideVelocity;
            effect->forward += effect->forwardVelocity;
            effect->height = Mini_SnowboardSlopeHeightFromForward(effect->forward);
            distance = effect->sidePos >> 0xC & 0xFFFFF;
            if (effect->age != 0) {
                if (distance < 983908) {
                    effect->rotY += 0x80;
                    effect->matrixValid = 0;
                    effect->fallVelocity = 0xFFFF0000;
                    if (distance < 982502) {
                        effect->update = Mini_SnowboardEffectUpdateFallingSpinRetireAtGround;
                    }
                }
            } else if (distance > 983908) {
                effect->rotY -= 0x80;
                effect->matrixValid = 0;
                effect->fallVelocity = 0xFFFF0000;
                if (distance > 985317) {
                    effect->update = Mini_SnowboardEffectUpdateFallingSpinRetireAtGround;
                }
            }
        }
    }
}

/*
 * Linear remap of one 20.12 fixed-point coordinate: shifts forward right 12, subtracts 0xB042, scales
 * by 0x5865/0x14A33, adds 0x10A36 and shifts the sum back left 12, so the result is fixed-point
 * again.
 */
// FUNCTION: FF7 0x007358b8
int __cdecl Mini_SnowboardSlopeHeightFromForward(int forward)
{
    forward = (forward >> 12) - 0xB042;
    return forward * 22629 / 84531 + 0x10A36 << 12;
}

// Starts the tumble after a hit. tumbleFrames is cleared, sideVelocity and forwardVelocity are
// set to three quarters of hitVelocityX and hitVelocityZ, fallVelocity is set to 0xFFFE0000, and
// update selects Mini_SnowboardEffectUpdateTumbleFallBounce. Sound 0x135 plays on channel 3.
// FUNCTION: FF7 0x007358e4
void __cdecl Mini_SnowboardEffectBeginHitTumble(SnowboardEffect *effect)
{
    effect->contactRadius = 0;
    effect->sideVelocity = (effect->hitVelocityX * 3) / 4;
    effect->fallVelocity = 0xFFFE0000;
    effect->forwardVelocity = (effect->hitVelocityZ * 3) / 4;
    effect->update = Mini_SnowboardEffectUpdateTumbleFallBounce;
    Mini_SnowboardPlaySfxOnChannel(3, 0x135, 0);
}

/*
 * Removes effects more than 512 units behind, otherwise advances coordinates and vertical
 * velocity with 0x1000 acceleration. On descending ground approach changes mesh/state, snaps
 * height, halves horizontal/forward speed and creates a secondary effect, with distance-scaled
 * sound when ahead within 0x1000. Updates vertical position and applies early rotation while the
 * old timer is below 0x40.
 */
// FUNCTION: FF7 0x0073594a
void __cdecl Mini_SnowboardEffectUpdateTumbleFallBounce(struct SnowboardEffect *effect)
{
    int slopeHeight;
    int distance;
    int h;
    int child;

    distance = (int)g_SnoboStatePtr->riderPos - effect->forward >> 12;
    if (distance > 512) {
        effect->active = 0;
    } else {
        effect->model = 0xD3;
        effect->model2 = 0xD4;
        effect->sidePos += effect->sideVelocity;
        effect->forward += effect->forwardVelocity;
        slopeHeight = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        effect->fallVelocity += 4096;
        h = effect->height + effect->fallVelocity;
        if (effect->fallVelocity > 0 && h > slopeHeight - 0x40000) {
            h = slopeHeight;
            effect->model = 0xE2;
            effect->model2 = 0;
            effect->rotY = 0x200 /* 45 deg */;
            effect->update = Mini_SnowboardEffectFallFadeRetire;
            effect->fallVelocity = 0xFFFE0000;
            effect->sideVelocity >>= 1;
            effect->forwardVelocity >>= 1;
            effect->age = 0;
            g_SnoboTumbleBounceEffectDef.sidePos = effect->sidePos;
            g_SnoboTumbleBounceEffectDef.height = h;
            g_SnoboTumbleBounceEffectDef.forward = effect->forward;
            child = Mini_SnowboardAllocEffectNode(Mini_SnowboardForwardToSectionIndex(effect->forward), &g_SnoboTumbleBounceEffectDef, 1);
            distance = effect->forward - (int)g_SnoboStatePtr->riderPos >> 12;
            if (distance > 0 && distance < 4096) {
                Mini_SnowboardPlaySfxOnChannel(4, 0x1AB, 0x7F - (distance >> 5));
            }
        }
        effect->height = h;
        if ((effect->timer)++ < 64) {
            effect->rotX += -0x20;
            effect->matrixValid = 0;
        }
    }
}

/*
 * Allocates an effect node and, on success, sets active/type fields, copies a seventy-two-byte
 * template and clears four velocity/state dwords and five short state fields. Returns the node or
 * zero without template access on allocation failure.
 */
// FUNCTION: FF7 0x00735b42
int __cdecl Mini_SnowboardAllocEffectNode(short section, void *src, int active)
{
    SnowboardEffect *node;

    node = (SnowboardEffect *)Mini_SnowboardFindFreeEffectNode();
    if (node != 0) {
        node->active = active;
        node->contact = 0;
        node->section = section;
        *(SnowboardEffectTemplate *)&node->model = *(SnowboardEffectTemplate *)src;
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

/*
 * Searches all 32 0x90-byte records at g_SnoboEffectRecords cyclically from g_SnoboEffectNodeCursor for a zero first
 * byte. Updates the cursor to the next slot and returns the free record address, or returns 0 if
 * none is free.
 */
// FUNCTION: FF7 0x00735bf1
int __cdecl Mini_SnowboardFindFreeEffectNode(void)
{
    int slot;
    SnowboardEffect *pNode;
    int remaining;
    remaining = 32;
    for (slot = g_SnoboEffectNodeCursor; remaining--; slot = (slot + 1) & 0x1F) {
        pNode = (SnowboardEffect *)(slot * 0x90 + ((char *)&g_SnoboEffectRecords));
        if (*(unsigned char *)pNode == 0) {
            g_SnoboEffectNodeCursor = (slot + 1) & 0x1F;
            return (int)pNode;
        }
    }
    return 0;
}

/*
 * The same remap as Mini_SnowboardSlopeHeightFromForward but with a different scale and an integer result: (forward >> 12) -
 * 0xB042, times 0x39 divided by 0x14A33, plus 0x8E. No shift back, so the value is a plain
 * integer.
 */
// FUNCTION: FF7 0x00735c60
int __cdecl Mini_SnowboardForwardToSectionIndex(int forward)
{
    forward = (forward >> 12) - 0xB042;
    return forward * 57 / 84531 + 0x8E;
}

/*
 * Post-increments signed age, and post-increments unsigned fade only after the consumed age
 * exceeds 60. If both consumed counters exceed 60 and 30, clears the active byte; otherwise
 * advances x and z by velocity, adds 0x1000 to y velocity before advancing y, decreases rotX by
 * 0x200 and clears matrixValid.
 */
// FUNCTION: FF7 0x00735c86
void __cdecl Mini_SnowboardEffectFallFadeRetire(struct SnowboardEffect *effect)
{
    if ((effect->age)++ > 60 && (effect->fade)++ > 30) {
        effect->active = 0;
    } else {
        effect->sidePos += effect->sideVelocity;
        effect->fallVelocity += 4096;
        effect->height += effect->fallVelocity;
        effect->forward += effect->forwardVelocity;
        effect->rotX += -0x200 /* 45 deg */;
        effect->matrixValid = 0;
    }
}

// Advances the frame cycle of a snowboard effect. For a nonzero flags byte, bit 2 also clears
// contactRadius and plays sound 0x135 on channel 3. Then age is reset and
// update selects Mini_SnowboardEffectRetireAfterDelay. animFrame is set from the
// g_SnoboEffectAnimFrames word at index (age >> 2) & 3, and age
// is incremented.
// FUNCTION: FF7 0x00735d46
void __cdecl Mini_SnowboardEffectUpdateFrameCycle(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(3, 0x135, 0);
        }
        effect->age = 0;
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
    effect->model = g_SnoboEffectAnimFrames[effect->age >> 2 & 3];
    (effect->age)++;
}

/*
 * Removes effects more than 512 units behind. Otherwise advances lateral/forward coordinates,
 * accelerates vertical velocity by 0x1000 and removes the effect on descending ground approach.
 * Before ground contact stores the new height, rotates by negative 0x40 and clears the other
 * rotation field.
 */
// FUNCTION: FF7 0x00735dcd
void __cdecl Mini_SnowboardEffectUpdateFallingSpinRetireAtGround(struct SnowboardEffect *effect)
{
    int groundH;
    int gap;
    int height;

    gap = (int)g_SnoboStatePtr->riderPos - effect->forward >> 12;
    if (gap > 512) {
        effect->active = 0;
    } else {
        effect->model = 0xD3;
        effect->model2 = 0xD4;
        effect->sidePos += effect->sideVelocity;
        effect->forward += effect->forwardVelocity;
        groundH = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        effect->fallVelocity += 4096;
        height = effect->height + effect->fallVelocity;
        if (effect->fallVelocity > 0 && height > groundH - 0x40000) {
            effect->active = 0;
        } else {
            effect->height = height;
            effect->rotX += 0xFFC0;
            effect->matrixValid = 0;
        }
    }
}

// Starts the approach-follow phase unless forward, read as an int, exceeds the
// integer-converted riderPos by more than 0x200000, in which case it returns. Otherwise fade is
// cleared, forwardVelocity takes the integer-converted headingY, contactRadius is set to 0x40, and
// callback selects Mini_SnowboardEffectUpdateFollowRiderSway.
// FUNCTION: FF7 0x00735eb3
void __cdecl Mini_SnowboardEffectStartApproachFollow(SnowboardEffect *effect)
{
    if (effect->forward - (int)g_SnoboStatePtr->riderPos > 2097152) {
        return;
    }
    effect->fade = 0;
    effect->forwardVelocity = (int)g_SnoboStatePtr->velocityZ;
    effect->contactRadius = 64;
    effect->update = Mini_SnowboardEffectUpdateFollowRiderSway;
}

/*
 * Dispatches flagged effects to the collision helper. Otherwise adjusts forward velocity according
 * to relative rider position with minimum 0x4000, bounds relative distance for rider speed above
 * 0x200 and can raise effect speed to rider speed. Updates sampled height and cosine lateral
 * motion, switching callback after passing its stored forward limit.
 */
// FUNCTION: FF7 0x00735f10
void __cdecl Mini_SnowboardEffectUpdateFollowRiderSway(SnowboardEffect *effect)
{
    int offset;
    int forwardPos;
    int riderPos;

    if (effect->contact != 0) {
        Mini_SnowboardEffectBeginHitTumble(effect);
    } else {
        riderPos = (int)g_SnoboStatePtr->riderPos;
        forwardPos = effect->forwardVelocity + (effect->forward > riderPos ? 0xFFFFFC00 : 2048);
        if (forwardPos < 16384) {
            forwardPos = 16384;
        }
        effect->forwardVelocity = forwardPos;
        forwardPos = effect->forward + effect->forwardVelocity;
        offset = riderPos - forwardPos;
        if (g_SnoboStatePtr->followAngle > 0x200) {
            if (offset > 0x80000) {
                forwardPos = riderPos - 0x80000;
                if (effect->forwardVelocity < (int)g_SnoboStatePtr->velocityZ) {
                    effect->forwardVelocity = (int)g_SnoboStatePtr->velocityZ;
                }
            } else if (offset < -0x1000000) {
                forwardPos = riderPos + 0x1000000;
            }
        }
        effect->forward = forwardPos;
        effect->height = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        effect->age += 16;
        effect->sidePos += CosFixed(effect->age) << 4;
        if (forwardPos > effect->forwardLimit) {
            effect->timer = 0;
            effect->update = Mini_SnowboardEffectUpdateSwaySpinLaunch;
        }
    }
}

/*
 * Dispatches flagged effects to the collision helper. Otherwise advances cosine lateral motion,
 * forward coordinate and sampled height, accelerates rotation and clears its other axis. After the
 * old timer exceeds 0x1E, clears size, sets negative vertical velocity and switches to
 * Mini_SnowboardEffectUpdateTumbleFallBounce.
 */
// FUNCTION: FF7 0x00736089
void __cdecl Mini_SnowboardEffectUpdateSwaySpinLaunch(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        Mini_SnowboardEffectBeginHitTumble(effect);
    } else {
        effect->age += 16;
        effect->sidePos += CosFixed(effect->age) * 8;
        effect->forward += effect->forwardVelocity;
        effect->height = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        effect->spinVelocity += 8;
        effect->rotY += effect->spinVelocity;
        effect->matrixValid = 0;
        if ((effect->timer)++ > 30) {
            effect->contactRadius = 0;
            effect->fallVelocity = 0xFFFE0000;
            effect->update = Mini_SnowboardEffectUpdateTumbleFallBounce;
        }
    }
}

/*
 * Leaves an effect unchanged while its forward distance exceeds 0x700000. Otherwise chooses half
 * rider forward speed with minimum 0x8000, sets signed lateral speed according to its side flag,
 * resets timer/size fields and installs Mini_SnowboardEffectUpdateLateralDriftSpawnChild as callback.
 */
// FUNCTION: FF7 0x00736189
void __cdecl Mini_SnowboardEffectStartLateralDriftSpawnChild(SnowboardEffect *effect)
{
    int speed;
    int lateralSpeed;

    if (effect->forward - (int)g_SnoboStatePtr->riderPos > 7340032) {
        return;
    }
    speed = (int)g_SnoboStatePtr->velocityZ >> 1;
    if (speed < 32768) {
        speed = 0x8000;
    }
    effect->forwardVelocity = speed;
    if (effect->sideFlag != 0) {
        lateralSpeed = -speed;
    } else {
        lateralSpeed = speed;
    }
    effect->sideVelocity = lateralSpeed;
    effect->fade = 0;
    effect->contactRadius = 64;
    effect->update = Mini_SnowboardEffectUpdateLateralDriftSpawnChild;
}

/*
 * Dispatches flagged effects to Mini_SnowboardEffectBeginHitTumbleSpawnChild. Otherwise removes effects more than 512 units
 * behind, advances lateral/forward coordinates and samples height. At side-dependent lateral
 * thresholds applies rotation and negative vertical velocity and changes to the falling callback
 * beyond the outer threshold.
 */
// FUNCTION: FF7 0x00736229
void __cdecl Mini_SnowboardEffectUpdateLateralDriftSpawnChild(SnowboardEffect *effect)
{
    int distance;

    if (effect->contact != 0) {
        Mini_SnowboardEffectBeginHitTumbleSpawnChild(effect);
    } else {
        distance = (int)g_SnoboStatePtr->riderPos - effect->forward >> 12;
        if (distance > 512) {
            effect->active = 0;
        } else {
            effect->sidePos += effect->sideVelocity;
            effect->forward += effect->forwardVelocity;
            effect->height = Mini_SnowboardSlopeHeightFromForward(effect->forward);
            distance = effect->sidePos >> 0xC & 0xFFFFF;
            if (effect->age != 0) {
                if (distance < 983908) {
                    effect->rotY += 0x80;
                    effect->matrixValid = 0;
                    effect->fallVelocity = 0xFFFF0000;
                    if (distance < 982502) {
                        effect->update = Mini_SnowboardEffectUpdateFallingSpinRetireAtGround;
                    }
                }
            } else if (distance > 983908) {
                effect->rotY -= 0x80;
                effect->matrixValid = 0;
                effect->fallVelocity = 0xFFFF0000;
                if (distance > 985317) {
                    effect->update = Mini_SnowboardEffectUpdateFallingSpinRetireAtGround;
                }
            }
        }
    }
}

/*
 * Copies effect position/angles into the secondary template and allocates a node, then sets its
 * velocities to four-fifths of the original and a fixed upward impulse without checking allocation
 * success. Clears the original collision flag, applies three-quarter saved velocities, changes
 * mesh/size/callback fields and plays the collision sound.
 */
// FUNCTION: FF7 0x0073635c
void __cdecl Mini_SnowboardEffectBeginHitTumbleSpawnChild(SnowboardEffect *effect)
{
    struct SnowboardEffect *node;

    g_SnoboHitTumbleChildEffectDef.sidePos = effect->sidePos;
    g_SnoboHitTumbleChildEffectDef.height = effect->height;
    g_SnoboHitTumbleChildEffectDef.forward = effect->forward;
    g_SnoboHitTumbleChildEffectDef.rotX = effect->rotX;
    g_SnoboHitTumbleChildEffectDef.rotY = effect->rotY;
    node = (struct SnowboardEffect *)Mini_SnowboardAllocEffectNode((short)g_SnoboStatePtr->section, &g_SnoboHitTumbleChildEffectDef, 1);
    node->sideVelocity = (effect->sideVelocity << 2) / 5;
    node->fallVelocity = 0xFFFDE000;
    node->forwardVelocity = (effect->forwardVelocity << 2) / 5;
    effect->contact = 0;
    effect->sideVelocity = (effect->hitVelocityX * 3) / 4;
    effect->fallVelocity = 0xFFFE0000;
    effect->forwardVelocity = (effect->hitVelocityZ * 3) / 4;
    effect->model = 0xC9;
    effect->model2 = 0xD0;
    effect->contactRadius = 0;
    effect->update = Mini_SnowboardEffectUpdateAirborneLandingFrameCycle;
    Mini_SnowboardPlaySfxOnChannel(3, 0x135, 0);
}

/*
 * Removes effects more than 512 units behind or advances position under vertical acceleration.
 * On descending ground approach snaps to terrain and initializes its mesh, dimensions, color/state
 * and ground-effect callback, playing distance-scaled sound when ahead within 0x1000. Otherwise
 * stores the airborne height and applies early rotation while the old timer is below 0x40.
 */
// FUNCTION: FF7 0x00736474
void __cdecl Mini_SnowboardEffectUpdateAirborneLandingFrameCycle(SnowboardEffect *effect)
{
    int groundH;
    int gap;
    int height;

    gap = (int)g_SnoboStatePtr->riderPos - effect->forward >> 12;
    if (gap > 512) {
        effect->active = 0;
    } else {
        effect->sidePos += effect->sideVelocity;
        effect->forward += effect->forwardVelocity;
        groundH = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        effect->fallVelocity += 4096;
        height = effect->height + effect->fallVelocity;
        if (effect->fallVelocity > 0 && height > groundH - 0x40000) {
            effect->active = 1;
            effect->section = Mini_SnowboardForwardToSectionIndex(effect->forward);
            effect->model = 0xD5;
            effect->model2 = 0;
            effect->contactRadius = 64;
            effect->height = groundH;
            effect->rotX = 0xFF56;
            effect->rotY = 0;
            effect->matrixValid = 0;
            effect->update = Mini_SnowboardEffectUpdateFrameCycle;
            effect->contactSplit = 0x40;
            effect->contactTop = 64;
            effect->contactResponse[3] = 0xA;
            effect->contactResponse[2] = 0xA;
            gap = effect->forward - (int)g_SnoboStatePtr->riderPos >> 12;
            if (gap > 0 && gap < 4096) {
                Mini_SnowboardPlaySfxOnChannel(4, 0x1AB, 0x7F - (gap >> 5));
            }
        } else {
            effect->height = height;
            if ((effect->timer)++ < 64) {
                effect->rotX += -0x20;
                effect->matrixValid = 0;
            }
        }
    }
}

/*
 * Post-increments signed age, and post-increments unsigned fade only after the consumed age
 * exceeds 60; clears the active byte if the consumed fade exceeds 30. Even after clearing it,
 * advances x and z, adds 0x1000 to y velocity before advancing y, decreases rotX by 0x200 and
 * clears matrixValid.
 */
// FUNCTION: FF7 0x00736640
void __cdecl Mini_SnowboardEffectFallFadeRetireNoSkip(struct SnowboardEffect *effect)
{
    if ((effect->age)++ > 60 && (effect->fade)++ > 30) {
        effect->active = 0;
    }
    effect->sidePos += effect->sideVelocity;
    effect->fallVelocity += 4096;
    effect->height += effect->fallVelocity;
    effect->forward += effect->forwardVelocity;
    effect->rotX += -0x200 /* 45 deg */;
    effect->matrixValid = 0;
}

// Starts the approach-follow phase unless forward, read as an int, exceeds the
// integer-converted riderPos by more than 0x200000, in which case it returns. Otherwise fade is
// cleared, forwardVelocity takes the integer-converted headingY, contactRadius is set to 0x40, and
// callback selects Mini_SnowboardEffectUpdateFollowRiderSwaySpawnChild.
// FUNCTION: FF7 0x007366fe
void __cdecl Mini_SnowboardEffectStartApproachFollowSpawnChild(SnowboardEffect *effect)
{
    if (effect->forward - (int)g_SnoboStatePtr->riderPos > 2097152) {
        return;
    }
    effect->fade = 0;
    effect->forwardVelocity = (int)g_SnoboStatePtr->velocityZ;
    effect->contactRadius = 64;
    effect->update = Mini_SnowboardEffectUpdateFollowRiderSwaySpawnChild;
}

/*
 * Dispatches flagged effects to the secondary-effect collision helper. Otherwise adjusts forward
 * velocity with a minimum, bounds distance from the rider at high speed, samples course height and
 * advances cosine lateral motion with phase step 0x20/amplitude shift five. After passing the
 * stored limit resets its timer and switches callback.
 */
// FUNCTION: FF7 0x0073675b
void __cdecl Mini_SnowboardEffectUpdateFollowRiderSwaySpawnChild(struct SnowboardEffect *effect)
{
    int distance;
    int forwardPos;
    int riderPos;

    if (effect->contact != 0) {
        Mini_SnowboardEffectBeginHitTumbleSpawnChild(effect);
    } else {
        riderPos = (int)g_SnoboStatePtr->riderPos;
        forwardPos = effect->forwardVelocity + (effect->forward > riderPos ? 0xFFFFFC00 : 2048);
        if (forwardPos < 16384) {
            forwardPos = 16384;
        }
        effect->forwardVelocity = forwardPos;
        forwardPos = effect->forward + effect->forwardVelocity;
        distance = riderPos - forwardPos;
        if (g_SnoboStatePtr->followAngle > 0x200) {
            if (distance > 524288) {
                forwardPos = riderPos - 0x80000;
                if (effect->forwardVelocity < (int)g_SnoboStatePtr->velocityZ) {
                    effect->forwardVelocity = (int)g_SnoboStatePtr->velocityZ;
                }
            } else if (distance < -16777216) {
                forwardPos = riderPos + 0x1000000;
            }
        }
        effect->forward = forwardPos;
        effect->height = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        effect->angle += 0x20;
        effect->sidePos += CosFixed(effect->angle) << 5;
        if (forwardPos > effect->forwardLimit) {
            effect->timer = 0;
            effect->update = Mini_SnowboardEffectUpdateSwaySpinThenDrop;
        }
    }
}

/*
 * Dispatches flagged effects to the collision helper. Otherwise advances cosine lateral motion,
 * forward position, sampled height and accelerating rotation. After the old timer exceeds 0x1E
 * clears size, sets negative vertical velocity and changes to Mini_SnowboardEffectUpdateAirborneLandingFrameCycle.
 */
// FUNCTION: FF7 0x007368d4
void __cdecl Mini_SnowboardEffectUpdateSwaySpinThenDrop(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        Mini_SnowboardEffectBeginHitTumble(effect);
    } else {
        effect->age += 16;
        effect->sidePos += CosFixed(effect->age) * 8;
        effect->forward += effect->forwardVelocity;
        effect->height = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        effect->spinVelocity += 8;
        effect->rotY += effect->spinVelocity;
        effect->matrixValid = 0;
        if ((effect->timer)++ > 30) {
            effect->contactRadius = 0;
            effect->fallVelocity = 0xFFFE0000;
            effect->update = Mini_SnowboardEffectUpdateAirborneLandingFrameCycle;
        }
    }
}

/*
 * Returns when the contact byte is zero. When its 0x02 bit is set, clears contactRadius, plays snowboard sound
 * 0xF0 on channel 3, initializes sideVelocity, fallVelocity and forwardVelocity, increments the
 * short at +4 and selects callback Mini_SnowboardEffectUpdateBallisticFadeOut. Allocates and copies two 144-byte records with
 * rotated/sign-changed motion and successive short values; each allocation failure returns
 * immediately. The intermediate fallVelocity adjustments affect the original record. After both
 * allocations spawns what mode names (enum SnowboardSplitSpawn). Without the 0x02 contact bit selects
 * Mini_SnowboardEffectRetireAfterDelay.
 */
// FUNCTION: FF7 0x007369d4
void __cdecl Mini_SnowboardEffectOnHitSplitBallisticFadeOut(SnowboardEffect *effect)
{
    SnowboardEffect *child;

    if (effect->contact == 0) {
        return;
    }
    if ((effect->contact & 2) != 0) {
        effect->contactRadius = 0;
        Mini_SnowboardPlaySfxOnChannel(3, 0xF0, 0);
        effect->contactRadius = 0;
        effect->sideVelocity = 0xFFFFEDA0;
        effect->fallVelocity = 0xFFFDC000;
        effect->forwardVelocity = 0xFFFFE5C0;
        (effect->model)++;
        effect->update = Mini_SnowboardEffectUpdateBallisticFadeOut;
        child = (SnowboardEffect *)Mini_SnowboardFindFreeEffectNode();
        if (child == 0) {
            return;
        }
        *child = *effect;
        child->sideVelocity = -effect->forwardVelocity;
        effect->fallVelocity = 0xFFFDB800;
        child->forwardVelocity = effect->sideVelocity;
        child->model = effect->model + 1;
        child = (SnowboardEffect *)Mini_SnowboardFindFreeEffectNode();
        if (child == 0) {
            return;
        }
        *child = *effect;
        child->sideVelocity = effect->forwardVelocity;
        effect->fallVelocity = 0xFFFDC800;
        child->forwardVelocity = -effect->sideVelocity;
        child->model = effect->model + 2;
        switch (effect->mode) {
        case SNOWBOARD_SPLIT_SPAWN_LINKED_BLINK:
            Mini_SnowboardEffectSpawnLinkedBlinkGroup(effect);
            break;
        case SNOWBOARD_SPLIT_SPAWN_CHILD_COPY:
            Mini_SnowboardEffectSpawnChildCopyMotion(effect);
            break;
        case SNOWBOARD_SPLIT_SPAWN_HIT_LAUNCH_NEG:
            Mini_SnowboardEffectSpawnSidedChildCopyMotion(effect, 0);
            break;
        case SNOWBOARD_SPLIT_SPAWN_HIT_LAUNCH_POS:
            Mini_SnowboardEffectSpawnSidedChildCopyMotion(effect, 1);
        }
    } else {
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
}

/*
 * Post-increments signed age and, once the consumed age exceeds 40, unsigned fade. Clears the
 * active byte when consumed fade exceeds 30; otherwise advances all three position dwords by their
 * velocities, then adds 0x1000 to y velocity.
 */
// FUNCTION: FF7 0x00736b85
void __cdecl Mini_SnowboardEffectUpdateBallisticFadeOut(struct SnowboardEffect *effect)
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

// Stages the shared g_SnoboLinkedBlinkEffectDef template from the effect's sidePos, height and forward
// sets its heightLimit to the new height plus 0x600000 (the target height), and allocates
// the main node with afterDelayCallback Mini_SnowboardEffectActivateLinkedEffectsAfterDelay. The
// main node's linked child is allocated with afterDelayCallback
// Mini_SnowboardEffectUpdateBlinkAfterDelay. linkedA and linkedB are then allocated with
// template ids 0xDC and 0xDD and lowered Y and Z, and their fallVelocity values are set to 0xFFFE8000 and
// 0xFFFE2000. All four allocations are used without null checks, and the template is left in its
// final staged state.
// FUNCTION: FF7 0x00736c2a
void __cdecl Mini_SnowboardEffectSpawnLinkedBlinkGroup(struct SnowboardEffect *effect)
{
    SnowboardEffect *childNode;
    SnowboardEffect *parentNode;

    g_SnoboLinkedBlinkEffectDef.model = 0xD8;
    g_SnoboLinkedBlinkEffectDef.model2 = 0xD9;
    g_SnoboLinkedBlinkEffectDef.sidePos = effect->sidePos;
    g_SnoboLinkedBlinkEffectDef.height = effect->height;
    g_SnoboLinkedBlinkEffectDef.forward = effect->forward;
    g_SnoboLinkedBlinkEffectDef.heightLimit = g_SnoboLinkedBlinkEffectDef.height + 0x600000;
    g_SnoboLinkedBlinkEffectDef.update = (SnowboardEffectUpdate)Mini_SnowboardEffectActivateLinkedEffectsAfterDelay;
    parentNode = (SnowboardEffect *)Mini_SnowboardAllocEffectNode((short)effect->section, &g_SnoboLinkedBlinkEffectDef, 1);
    g_SnoboLinkedBlinkEffectDef.model = 0;
    g_SnoboLinkedBlinkEffectDef.model2 = 0;
    g_SnoboLinkedBlinkEffectDef.update = (SnowboardEffectUpdate)Mini_SnowboardEffectUpdateBlinkAfterDelay;
    parentNode->linked = Mini_SnowboardAllocEffectNode((short)effect->section, &g_SnoboLinkedBlinkEffectDef, 1);
    g_SnoboLinkedBlinkEffectDef.model = 0xDC;
    g_SnoboLinkedBlinkEffectDef.model2 = 0;
    g_SnoboLinkedBlinkEffectDef.sidePos = effect->sidePos;
    g_SnoboLinkedBlinkEffectDef.height = effect->height - 0x22000;
    g_SnoboLinkedBlinkEffectDef.forward = effect->forward - 0x80000;
    g_SnoboLinkedBlinkEffectDef.update = 0;
    childNode = (SnowboardEffect *)Mini_SnowboardAllocEffectNode((short)effect->section, &g_SnoboLinkedBlinkEffectDef, 1);
    parentNode->linkedA = (int)childNode;
    childNode->fallVelocity = 0xFFFE8000;
    g_SnoboLinkedBlinkEffectDef.model = 0xDD;
    g_SnoboLinkedBlinkEffectDef.model2 = 0;
    g_SnoboLinkedBlinkEffectDef.update = 0;
    childNode = (SnowboardEffect *)Mini_SnowboardAllocEffectNode((short)effect->section, &g_SnoboLinkedBlinkEffectDef, 1);
    parentNode->linkedB = (int)childNode;
    childNode->fallVelocity = 0xFFFE2000;
}

/*
 * Post-increments the signed age. When its consumed value exceeds 60, writes 0xde or
 * zero to model according to bit 2 of that consumed value.
 */
// FUNCTION: FF7 0x00736d95
void __cdecl Mini_SnowboardEffectUpdateBlinkAfterDelay(SnowboardEffect *effect)
{
    int oldAge;
    if ((oldAge = (effect->age)++) > 60) {
        effect->model = (oldAge & 4) ? 0xde : 0;
    }
}

/*
 * After the old short counter exceeds 0x78, resets it and initializes this effect and its two
 * linked effects with new callbacks/mesh/size/color fields. Sets the first linked effect velocity
 * toward rider position divided by sixteen and a randomized lateral velocity for the second.
 */
// FUNCTION: FF7 0x00736de1
void __cdecl Mini_SnowboardEffectActivateLinkedEffectsAfterDelay(SnowboardEffect *linkedEffect)
{
    SnowboardEffect *linked;
    if ((linkedEffect->age)++ > 120) {
        linkedEffect->age = 0;
        linkedEffect->model = 0xda;
        linkedEffect->model2 = 0xdb;
        linkedEffect->update = Mini_SnowboardEffectUpdateLinkedHit;
        linkedEffect->contactRadius = 64;
        linkedEffect->contactTop = 128;
        linked = (SnowboardEffect *)(linkedEffect->linkedA);
        linked->update = Mini_SnowboardEffectUpdateThrownArcToGround;
        linked->contactRadius = 192;
        linked->contactTop = 64;
        linked->contactResponse[3] = 12;
        linked->contactResponse[2] = 12;
        linked->contactResponse[1] = 12;
        linked->contactResponse[0] = 12;
        linked->sideVelocity = ((int)g_SnoboStatePtr->relPosX - linked->sidePos) >> 4;
        linked->forwardVelocity = ((int)g_SnoboStatePtr->riderPos - linked->forward) >> 4;
        linked = (SnowboardEffect *)(linkedEffect->linkedB);
        linked->update = Mini_SnowboardEffectUpdateFallToGroundThenRetire;
        linked->sideVelocity = (16 - (rand() & 31)) << 11;
    }
}

/*
 * For collision bit two, clears the flag, restores saved horizontal/forward velocities, sets
 * upward impulse and falling mesh/state, deactivates the linked effect and plays a sound. Other
 * nonzero collision flags reset the counter and select a different callback. Without collision,
 * changes the mesh pair when the old timer equals 0x3C.
 */
// FUNCTION: FF7 0x00736ef8
void __cdecl Mini_SnowboardEffectUpdateLinkedHit(SnowboardEffect *effect)
{
    SnowboardEffect *linkedRecord;

    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contact = 0;
            effect->contactRadius = 0;
            effect->sideVelocity = effect->hitVelocityX;
            effect->fallVelocity = 0xFFFE0000;
            effect->forwardVelocity = effect->hitVelocityZ;
            effect->model = 0xCA;
            effect->model2 = 0xD0;
            linkedRecord = (SnowboardEffect *)effect->linked;
            linkedRecord->active = 0;
            effect->update = Mini_SnowboardEffectUpdateAirborneLandingFrameCycle;
            Mini_SnowboardPlaySfxOnChannel(3, 0x135, 0);
        } else {
            effect->age = 0;
            effect->update = Mini_SnowboardEffectFadeOutLinkedRecord;
        }
    } else if ((effect->age)++ == 0x3C) {
        effect->model = 0xD8;
        effect->model2 = 0xD9;
    }
}

/*
 * Post-increments signed age. Once its consumed value exceeds 60, reads the linked record
 * pointer (linked) and post-increments unsigned fade. If the consumed fade exceeds 30, clears both
 * active bytes; otherwise copies the newly incremented fade value to the linked record's fade.
 */
// FUNCTION: FF7 0x00736fe3
void __cdecl Mini_SnowboardEffectFadeOutLinkedRecord(SnowboardEffect *effect)
{
    SnowboardEffect *linkedRecord;
    if ((effect->age)++ > 60) {
        linkedRecord = (SnowboardEffect *)effect->linked;
        if ((effect->fade)++ > 30) {
            effect->active = 0;
            linkedRecord->active = 0;
        } else {
            linkedRecord->fade = effect->fade;
        }
    }
}

/*
 * Advances horizontal/forward position and accelerated vertical motion, clamping to course height
 * and changing callback on ground contact. Applies early rotation while the old timer is below
 * 0x15. A collision clears the flag/size and plays a sound independently of that ground update.
 */
// FUNCTION: FF7 0x00737066
void __cdecl Mini_SnowboardEffectUpdateThrownArcToGround(SnowboardEffect *effect)
{
    int groundHeight;
    int newY;

    effect->sidePos += effect->sideVelocity;
    effect->forward += effect->forwardVelocity;
    effect->fallVelocity += 4096;
    newY = effect->height + effect->fallVelocity;
    groundHeight = Mini_SnowboardSlopeHeightFromForward(effect->forward);
    if (newY > groundHeight) {
        newY = groundHeight;
        effect->contactRadius = 0;
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
    effect->height = newY;
    if ((effect->timer)++ < 21) {
        effect->rotX += 0x60;
        effect->matrixValid = 0;
    }
    if (effect->contact != 0) {
        effect->contactRadius = 0;
        effect->contact = 0;
        Mini_SnowboardPlaySfxOnChannel(3, 0xF0, 0);
    }
}

// Moves the effect one frame while it falls to the ground. sidePos gains sideVelocity, forward
// loses 0x4000, and fallVelocity gains a 0x1000 step. height advances by fallVelocity and is
// clamped to the slope height at forward plus 0x20. When the clamp applies, update selects
// Mini_SnowboardEffectRetireAfterDelay.
// FUNCTION: FF7 0x0073715f
void __cdecl Mini_SnowboardEffectUpdateFallToGroundThenRetire(struct SnowboardEffect *effect)
{
    int groundLimit;
    int newY;

    effect->sidePos += effect->sideVelocity;
    effect->forward -= 0x4000;
    effect->fallVelocity += 4096;
    newY = effect->height + effect->fallVelocity;
    groundLimit = Mini_SnowboardSlopeHeightFromForward(effect->forward) + 32;
    if (newY > groundLimit) {
        newY = groundLimit;
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
    effect->height = newY;
}

// Copies the effect's position (sidePos, height, forward, heightLimit) into the
// g_SnoboLandingFrameCycleEffectDef template, then allocates an effect node from that template for the
// rider's section. The effect's velocities (sideVelocity, fallVelocity, forwardVelocity, reversed)
// are copied into the new node without a null check.
// FUNCTION: FF7 0x007371e4
void __cdecl Mini_SnowboardEffectSpawnChildCopyMotion(SnowboardEffect *effect)
{
    SnowboardEffect *childNode;
    *(Copy16 *)&g_SnoboLandingFrameCycleEffectDef.sidePos = *(Copy16 *)&effect->sidePos;
    childNode = (SnowboardEffect *)Mini_SnowboardAllocEffectNode(g_SnoboStatePtr->section, &g_SnoboLandingFrameCycleEffectDef, 1);
    *(Copy16 *)&childNode->sideVelocity = *(Copy16 *)&effect->sideVelocity;
}

// Sets the g_SnoboLandingHitLaunchEffectDef template's homeX to 0x20 when side is nonzero and to
// -0x20 otherwise. The effect's position is copied into the template, a node is allocated from it
// for the rider's section, and the effect's velocities are copied into the node without a null check.
// FUNCTION: FF7 0x00737256
void __cdecl Mini_SnowboardEffectSpawnSidedChildCopyMotion(SnowboardEffect *effect, int side)
{
    SnowboardEffect *childNode;
    g_SnoboLandingHitLaunchEffectDef.homeX = side ? 32 : -32;
    *(Copy16 *)&g_SnoboLandingHitLaunchEffectDef.sidePos = *(Copy16 *)&effect->sidePos;
    childNode = (SnowboardEffect *)Mini_SnowboardAllocEffectNode(g_SnoboStatePtr->section, &g_SnoboLandingHitLaunchEffectDef, 1);
    *(Copy16 *)&childNode->sideVelocity = *(Copy16 *)&effect->sideVelocity;
}

/*
 * Removes effects more than 512 units behind. Otherwise advances position with vertical
 * acceleration; on descending ground approach snaps height and initializes mesh, size, color and a
 * new callback with sound. Before contact stores height and applies rotation while the old timer
 * is below 0x40.
 */
// FUNCTION: FF7 0x007372d9
void __cdecl Mini_SnowboardEffectUpdateAirborneLandingHitLaunch(SnowboardEffect *effect)
{
    int slopeHeight;
    int distance;
    int newY;

    distance = (int)g_SnoboStatePtr->riderPos - effect->forward >> 12;
    if (distance > 512) {
        effect->active = 0;
    } else {
        effect->sidePos += effect->sideVelocity;
        effect->forward += effect->forwardVelocity;
        slopeHeight = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        effect->fallVelocity += 4096;
        newY = effect->height + effect->fallVelocity;
        if (effect->fallVelocity > 0 && newY > slopeHeight - 0x40000) {
            effect->active = 1;
            effect->model = 0xDB;
            effect->model2 = 0xDA;
            effect->contactRadius = 64;
            effect->height = slopeHeight;
            effect->rotX = 0;
            effect->rotY = 0;
            effect->matrixValid = 0;
            effect->contactSplit = 0x40;
            effect->contactTop = 64;
            effect->contactResponse[3] = 0xA;
            effect->contactResponse[2] = 0xA;
            effect->timer = 0;
            effect->age = 0;
            effect->update = Mini_SnowboardEffectUpdateHitLaunchOrDrift;
            Mini_SnowboardPlaySfxOnChannel(3, 0x38, 0);
        } else {
            effect->height = newY;
            if ((effect->timer)++ < 64) {
                effect->rotX += -0x40;
                effect->matrixValid = 0;
            }
        }
    }
}

/*
 * Updates the snowboard object through activity and motion phases. A nonzero activity byte with
 * bit 2 clears activity, sets motion from hitVelocityX/hitVelocityZ with vertical component -0x20000, selects
 * frames 0xCA/0xD0 and callback Mini_SnowboardEffectUpdateAirborneLandingFrameCycle, and plays sound 0x135; activity without bit 2 clears
 * the timer and selects Mini_SnowboardEffectRetireAfterDelay. In the inactive terminal phase increments rotX by 0x80
 * until exactly 0x400. Otherwise waits until the postincrement timer reaches 15, rotates a shared
 * data-block vector, advances coordinates and derives height. An out-of-range X test sets terminal
 * flag timer and frames 0xE3/0xE4, but the final stores overwrite those frames with 0xC9 and
 * Mini_SnowboardGetCycleFrameId(timer>>1).
 */
// FUNCTION: FF7 0x0073746b
void __cdecl Mini_SnowboardEffectUpdateHitLaunchOrDrift(SnowboardEffect *effect)
{
    int value;
    SnowboardEffectScratch *ptr;

    ptr = (SnowboardEffectScratch *)Gfx_GetPsxScratchpadPtr(0);
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contact = 0;
            effect->contactRadius = 0;
            effect->sideVelocity = effect->hitVelocityX;
            effect->fallVelocity = -0x20000;
            effect->forwardVelocity = effect->hitVelocityZ;
            effect->model = 0xCA;
            effect->model2 = 0xD0;
            effect->update = Mini_SnowboardEffectUpdateAirborneLandingFrameCycle;
            Mini_SnowboardPlaySfxOnChannel(3, 0x135, 0);
        } else {
            effect->age = 0;
            effect->update = Mini_SnowboardEffectRetireAfterDelay;
        }
    } else if (effect->launched != 0) {
        if (effect->rotX != 0x400) {
            effect->rotX += 0x80;
            effect->matrixValid = 0;
        }
    } else {
        if ((effect->age)++ < 15) {
            return;
        }
        effect->rotY += effect->angleStep;
        value = effect->rotY;
        effect->rotY = (short)value;
        effect->matrixValid = 0;
        /* Retail caller prototype (int, short) -> void preserved for /Od argument promotion on `Mini_SnowboardBuildYRotationMatrix`. */
        ((void(__cdecl *)(int, short))Mini_SnowboardBuildYRotationMatrix)((int)&ptr->yawMatrix, value);
        ptr->step.x = 0;
        ptr->step.z = -3072;
        PsxApplyMatrix33ToLongVector(&ptr->yawMatrix, &ptr->step, &ptr->rotatedX);
        effect->sidePos += ptr->rotatedX << 4;
        effect->forward += ptr->rotatedForward << 4;
        effect->height = Mini_SnowboardSlopeHeightFromForward(effect->forward);
        value = effect->sidePos;
        value = (value >> 12) & 0xFFFFF;
        if (value > 985344 || value < 982432) {
            effect->launched = 1;
            effect->model = 0xE3;
            effect->model2 = 0xE4;
        }
        effect->model = 0xC9;
        effect->model2 = Mini_SnowboardGetCycleFrameId(effect->age >> 1);
    }
}

/*
 * Mechanical forwarder: calls Matrix30SetRotationY with the arguments swapped and angle narrowed to short,
 * i.e. Matrix30SetRotationY((short)angle, matrix).
 */
// FUNCTION: FF7 0x007376af
void __cdecl Mini_SnowboardBuildYRotationMatrix(void *matrix, int angle)
{
    Matrix30SetRotationY((short)angle, matrix);
}

/*
 * Returns the short at index index % 6 of the six-entry table at g_SnoboCycleFrameIds.
 */
// FUNCTION: FF7 0x007376c5
short __cdecl Mini_SnowboardGetCycleFrameId(int index)
{
    return (short)((short *)&g_SnoboCycleFrameIds)[index % 6];
}

/*
 * Updates the paired snowboard records referenced by effect and effect->linked. In the armed phase
 * postincrements timer, and after its old value exceeds 60 increments both fade counters,
 * clearing both state bytes when the first exceeds 30. Activity bit 2 launches both records using
 * oppositely rotated motion vectors, vertical speed -0x28000 and callback Mini_SnowboardEffectUpdateFallSpinFadeOutCopyIndex, and plays
 * sound 0x135; other activity arms isLatched. The common tail postincrements the first timer again for
 * cosine, uses the incremented value for sine, rotates the displacement and mirrors X/Z into the
 * partner. Updates the partner frame from its separately postincremented timer. Thus the armed
 * timer normally advances twice per call.
 */
// FUNCTION: FF7 0x007376dd
void __cdecl Mini_SnowboardEffectUpdatePairedLaunchOrSwing(SnowboardEffect *effect)
{
    SnowboardEffectScratch *ptr;
    SnowboardEffect *other;
    int sh;

    ptr = (SnowboardEffectScratch *)Gfx_GetPsxScratchpadPtr(0);
    other = (SnowboardEffect *)(effect->linked);
    if (effect->isLatched != 0) {
        if ((effect->timer)++ > 60) {
            effect->fade += 1;
            other->fade += 1;
            if (effect->fade > 30) {
                effect->active = 0;
                other->active = 0;
                return;
            }
        }
    } else if (effect->contact != 0) {
        g_SnoboSignalValue = effect->section;
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            ptr->step.x = (short)(effect->hitVelocityX >> 12);
            ptr->step.z = (short)(effect->hitVelocityZ >> 12);
            Mini_SnowboardBuildYRotationMatrix(&ptr->yawMatrix, 0x100);
            PsxApplyMatrix33ToLongVector(&ptr->yawMatrix, &ptr->step, &ptr->rotatedX);
            effect->sideVelocity = ptr->rotatedX << 12;
            effect->fallVelocity = -0x28000;
            effect->forwardVelocity = ptr->rotatedForward << 12;
            effect->update = Mini_SnowboardEffectUpdateFallSpinFadeOutCopyIndex;
            Mini_SnowboardBuildYRotationMatrix(&ptr->yawMatrix, 0xF00);
            PsxApplyMatrix33ToLongVector(&ptr->yawMatrix, &ptr->step, &ptr->rotatedX);
            other->sideVelocity = ptr->rotatedX << 12;
            other->fallVelocity = -0x28000;
            other->forwardVelocity = ptr->rotatedForward << 12;
            other->update = Mini_SnowboardEffectUpdateFallSpinFadeOutCopyIndex;
            other->model = 0xCA;
            other->model2 = 0xD0;
            Mini_SnowboardPlaySfxOnChannel(3, 0x135, 0);
        } else {
            effect->isLatched = 1;
        }
    }
    ptr->step.x = 0;
    ptr->step.y = 0;
    sh = effect->swingShift;
    ptr->step.z =
        (short)CosFixed((effect->timer)++ << sh);
    effect->rotZ = (short)SinFixed(effect->timer << sh);
    effect->matrixValid = 0;
    Mini_SnowboardBuildYRotationMatrix(&ptr->yawMatrix, effect->rotY + 0x400);
    PsxApplyMatrix33ToShortVector(&ptr->yawMatrix, &ptr->step, &ptr->step);
    sh = effect->stepShift;
    effect->sidePos += ptr->step.x << sh;
    other->sidePos = effect->sidePos;
    effect->forward += ptr->step.z << sh;
    other->forward = effect->forward;
    other->model2 = Mini_SnowboardGetCycleFrameId((other->timer)++ >> 2);
}

// Post-increments age and, only once its old value exceeds 0x3C, post-increments fade. When both
// old values exceed their thresholds (0x3C and 0x1E), active is cleared. Otherwise section is copied
// from the state's section, sidePos advances by sideVelocity, fallVelocity gains 0x1000 and
// height advances by the new fallVelocity, forward advances by forwardVelocity, rotX gains
// 0x80, and matrixValid is cleared.
// FUNCTION: FF7 0x00737a27
void __cdecl Mini_SnowboardEffectUpdateFallSpinFadeOutCopyIndex(struct SnowboardEffect *context)
{
    if ((context->age)++ > 60 && (context->fade)++ > 30) {
        context->active = 0;
    } else {
        context->section = g_SnoboStatePtr->section;
        context->sidePos += context->sideVelocity;
        context->fallVelocity += 4096;
        context->height += context->fallVelocity;
        context->forward += context->forwardVelocity;
        context->rotX += 0x80;
        context->matrixValid = 0;
    }
}

/*
 * A nonzero +1 byte with bit 2 calls Mini_SnowboardEffectBeginHitFallFade and returns; otherwise a nonzero byte selects
 * Mini_SnowboardEffectRetireAfterDelay. When the old postincremented age exceeds 0xF, sets fallVelocity to 0xFFFF0000 and
 * selects Mini_SnowboardEffectUpdateHopArc.
 */
// FUNCTION: FF7 0x00737afb
void __cdecl Mini_SnowboardEffectUpdateHopWait(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            Mini_SnowboardEffectBeginHitFallFade(effect);
            return;
        }
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
    if ((effect->age)++ > 15) {
        effect->fallVelocity = 0xFFFF0000;
        effect->update = Mini_SnowboardEffectUpdateHopArc;
    }
}

// Calls Mini_SnowboardEffectBeginHitFallFade when flags has bit 2. Otherwise fallVelocity gains 0x1000 and
// height advances by it. When height exceeds heightLimit, it is clamped to heightLimit, timer is cleared, hopArc
// becomes 0xE5 and the callback becomes Mini_SnowboardEffectUpdateHopWait. Otherwise hopArc becomes
// 0xD0.
// FUNCTION: FF7 0x00737b6e
void __cdecl Mini_SnowboardEffectUpdateHopArc(SnowboardEffect *effect)
{
    if ((effect->contact & 2) != 0) {
        Mini_SnowboardEffectBeginHitFallFade(effect);
    } else {
        effect->fallVelocity += 4096;
        effect->height += effect->fallVelocity;
        if (effect->height > effect->heightLimit) {
            effect->age = 0;
            effect->height = effect->heightLimit;
            effect->model2 = 0xE5;
            effect->update = Mini_SnowboardEffectUpdateHopWait;
        } else {
            effect->model2 = 0xD0;
        }
    }
}

// Starts the fall after a hit. state becomes 2, timer and contactRadius are cleared, and sideVelocity and forwardVelocity are
// loaded from savedHorizontalVelocity and savedForwardVelocity. fallVelocity is set to 0xFFFE0000, the
// callback becomes Mini_SnowboardEffectUpdateFallSpinFadeOut, and sound 0x135 plays on channel 3.
// FUNCTION: FF7 0x00737bf8
void __cdecl Mini_SnowboardEffectBeginHitFallFade(SnowboardEffect *effect)
{
    effect->active = 2;
    effect->age = 0;
    effect->contactRadius = 0;
    effect->sideVelocity = effect->hitVelocityX;
    effect->fallVelocity = 0xFFFE0000;
    effect->forwardVelocity = effect->hitVelocityZ;
    effect->update = Mini_SnowboardEffectUpdateFallSpinFadeOut;
    Mini_SnowboardPlaySfxOnChannel(3, 0x135, 0);
}

/*
 * Post-increments signed age and, only after consumed age exceeds 60, unsigned fade. Clears the
 * active byte when consumed fade exceeds 30; otherwise advances x and z, adds 0x1000 to y velocity
 * before advancing y, adds 0x80 to rotX and clears matrixValid.
 */
// FUNCTION: FF7 0x00737c58
void __cdecl Mini_SnowboardEffectUpdateFallSpinFadeOut(struct SnowboardEffect *effect)
{
    if ((effect->age)++ > 60 && (effect->fade)++ > 30) {
        effect->active = 0;
    } else {
        effect->sidePos += effect->sideVelocity;
        effect->fallVelocity += 4096;
        effect->height += effect->fallVelocity;
        effect->forward += effect->forwardVelocity;
        effect->rotX += 0x80;
        effect->matrixValid = 0;
    }
}

/*
 * On collision deactivates the effect and plays a sound, then sets mode flags for types three/four
 * or adds its indexed score otherwise. Without collision cycles its mesh/palette phase and
 * oscillates height; a nearby shared segment signal changes its callback and clears that signal.
 */
// FUNCTION: FF7 0x00737d18
void __cdecl Mini_SnowboardEffectUpdatePickup(SnowboardEffect *pickup)
{
    int phase;

    if (pickup->contact != 0) {
        pickup->active = 0;
        Mini_SnowboardPlaySfxOnChannel(3, 0xF9, 0);
        switch (pickup->pickupKind) {
        case SNOWBOARD_PICKUP_BALLOON_TRIPLE:
            /* Retail caller passes 2 arg(s) to 1-parameter `Mini_SnowboardSpawnPickupEffectType3` (__cdecl stack cleanup matches retail). */
            ((void(__cdecl *)(int, int))Mini_SnowboardSpawnPickupEffectType3)(0, 0);
            g_SnoboStatePtr->activeFlags = 1;
            break;
        case SNOWBOARD_PICKUP_BALLOON_FIVE:
            /* Retail caller passes 2 arg(s) to 1-parameter `Mini_SnowboardSpawnPickupEffectType4` (__cdecl stack cleanup matches retail). */
            ((void(__cdecl *)(int, int))Mini_SnowboardSpawnPickupEffectType4)(0, 0);
            g_SnoboStatePtr->pickupFlag = 1;
            break;
        default:
            g_SnoboStatePtr->pickupTotal += g_SnoboPickupScoreByKind[pickup->pickupKind];
        }
    } else {
        phase = pickup->age;
        (pickup->age)++;
        pickup->model = (phase >> 3 & 3) + 0xB8;
        pickup->packedPhase = pickup->packedPhase & 0xF | (phase >> 3 & 3) << 4;
        pickup->height += SinFixed(phase << 5) * 2;
        if (g_SnoboSignalValue != 0 && abs(g_SnoboSignalValue - pickup->section) <= 1) {
            pickup->update = Mini_SnowboardEffectUpdatePickupRiseFadeOut;
            g_SnoboSignalValue = 0;
        }
    }
}

// Post-increments timer, a signed counter, and once its old value exceeds 60 post-increments
// fadeTimer, an unsigned one. When that old value exceeds 30 as well, the active byte is
// cleared. Otherwise phase is post-incremented. Bits 3 and 4 of its old value give a frame of 0xB8
// plus that step, and the high nibble of packedFlags is set to the step while the low nibble is
// kept. height then advances by fallVelocity, and fallVelocity drops by 0x800.
// FUNCTION: FF7 0x00737e6d
void __cdecl Mini_SnowboardEffectUpdatePickupRiseFadeOut(SnowboardEffect *pickup)
{
    int phase;
    if ((pickup->counter)++ > 60 && (pickup->fade)++ > 30) {
        pickup->active = 0;
    } else {
        phase = (pickup->age)++;
        pickup->model = ((phase >> 3) & 3) + 0xb8;
        pickup->packedFlags = (pickup->packedFlags & 0xf) | (((phase >> 3) & 3) << 4);
        pickup->height += pickup->fallVelocity;
        pickup->fallVelocity -= 2048;
    }
}

/*
 * Selects the first snowboard effect callback and allocates its effect node.
 */
// FUNCTION: FF7 0x00737f48
void __cdecl Mini_SnowboardSpawnPickupEffectType3(int unused)
{
    g_SnoboEffectDef.update = (SnowboardEffectUpdate)Mini_SnowboardEffectRenderFamily2Expire;
    Mini_SnowboardAllocEffectNode((short)unused, &g_SnoboEffectDef, 2);
}
/*
 * Calls family2 on the renderer g_pSnoboRenderer and clears the first byte when the old
 * postincremented age exceeds 60.
 */
// FUNCTION: FF7 0x00737f6b
void __cdecl Mini_SnowboardEffectRenderFamily2Expire(SnowboardEffect *effect)
{
    g_pSnoboRenderer->family2();
    if ((effect->age)++ > 60)
        effect->active = 0;
}

/*
 * Selects the second snowboard effect callback and allocates its effect node.
 */
// FUNCTION: FF7 0x00737fa8
void __cdecl Mini_SnowboardSpawnPickupEffectType4(int unused)
{
    g_SnoboEffectDef.update = (SnowboardEffectUpdate)Mini_SnowboardEffectRenderFamily0Expire;
    Mini_SnowboardAllocEffectNode((short)unused, &g_SnoboEffectDef, 2);
}
/*
 * Calls family0 on the renderer g_pSnoboRenderer and clears the first byte when the old
 * postincremented age exceeds 60.
 */
// FUNCTION: FF7 0x00737fcb
void __cdecl Mini_SnowboardEffectRenderFamily0Expire(SnowboardEffect *effect)
{
    g_pSnoboRenderer->family0();
    if ((effect->age)++ > 60)
        effect->active = 0;
}

/*
 * For a nonzero contact byte, bit 2 clears contactRadius, selects Mini_SnowboardEffectUpdateTiltNeg and plays channel-1 sound 0x10C
 * at 0x7F; otherwise selects Mini_SnowboardEffectRetireAfterDelay.
 */
// FUNCTION: FF7 0x00738008
void __cdecl Mini_SnowboardEffectHitStartTiltNeg(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            effect->update = Mini_SnowboardEffectUpdateTiltNeg;
            Mini_SnowboardPlaySfxOnChannel(1, 0x10C, 0x7F);
        } else {
            effect->update = Mini_SnowboardEffectRetireAfterDelay;
        }
    }
}

// Subtracts a quarter of SinFixed of the old tiltPhase shifted right 1 from tilt, post-incrementing
// tiltPhase, and clears matrixValid. When tilt falls below -0x480, the callback becomes
// Mini_SnowboardEffectRetireAfterDelay.
// FUNCTION: FF7 0x00738058
void __cdecl Mini_SnowboardEffectUpdateTiltNeg(SnowboardEffect *effect)
{

    effect->rotX -= (short)(SinFixed(((effect->timer)++ >> 1)) >> 2);
    effect->matrixValid = 0;
    if (effect->rotX < -0x480) {
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
}

/*
 * For a nonzero contact byte, bit 2 clears contactRadius, selects Mini_SnowboardEffectUpdateTiltPos and plays channel-1 sound 0x10C
 * at 0x7F; otherwise selects Mini_SnowboardEffectRetireAfterDelay.
 */
// FUNCTION: FF7 0x007380c8
void __cdecl Mini_SnowboardEffectHitStartTiltPos(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            effect->update = Mini_SnowboardEffectUpdateTiltPos;
            Mini_SnowboardPlaySfxOnChannel(1, 0x10C, 0x7F);
        } else {
            effect->update = Mini_SnowboardEffectRetireAfterDelay;
        }
    }
}

// Adds a quarter of SinFixed of the old tiltPhase shifted right 1 to tilt, post-incrementing
// tiltPhase, and clears matrixValid. When tilt exceeds 0x480, the callback becomes
// Mini_SnowboardEffectRetireAfterDelay.
// FUNCTION: FF7 0x00738118
void __cdecl Mini_SnowboardEffectUpdateTiltPos(SnowboardEffect *effect)
{

    effect->rotX += (short)(SinFixed(((effect->timer)++ >> 1)) >> 2);
    effect->matrixValid = 0;
    if (effect->rotX > 0x480) {
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
}

/*
 * For a nonzero contact byte, optionally clears contactRadius and plays channel-2 sound 0x10C for bit 2, then
 * selects Mini_SnowboardEffectRetireAfterDelay.
 */
// FUNCTION: FF7 0x00738188
void __cdecl Mini_SnowboardEffectHitPlaySoundFade(SnowboardEffect *effect)
{
    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(2, 0x10C, 0);
        }
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
}

/*
 * For a nonzero collision flag, bit two clears size, plays a sound and selects one of two
 * callbacks by the rider facing half-turn bit. Other nonzero flags select the default callback. A
 * zero collision flag leaves the effect unchanged.
 */
// FUNCTION: FF7 0x007381cc
void __cdecl Mini_SnowboardEffectStepOnHitSelectLaunch(SnowboardEffect *effect)
{
    SnowboardEffectUpdate callback;

    if (effect->contact != 0) {
        if ((effect->contact & 2) != 0) {
            effect->contactRadius = 0;
            Mini_SnowboardPlaySfxOnChannel(2, 0x10C, 0);
            if ((g_SnoboStatePtr->flags & 0x800) != 0) {
                callback = Mini_SnowboardEffectUpdateLaunchAccelerateNeg;
            } else {
                callback = Mini_SnowboardEffectUpdateLaunchAccelerate;
            }
            effect->update = callback;
        } else {
            effect->update = Mini_SnowboardEffectRetireAfterDelay;
        }
    }
}

// Adds 8 to accel and subtracts that value from speed, then clears accel. When speed
// falls below -0x500, accel is cleared and update becomes
// Mini_SnowboardEffectRetireAfterDelay.
// FUNCTION: FF7 0x00738244
void __cdecl Mini_SnowboardEffectUpdateLaunchAccelerateNeg(SnowboardEffect *effect)
{
    effect->accel += 8;
    effect->rotX -= effect->accel;
    effect->matrixValid = 0;
    if (effect->rotX < -0x500) {
        effect->accel = 0;
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
}

// Adds 8 to accel and adds that value to speed, then clears accel. When speed exceeds
// 0x500, accel is cleared and update becomes Mini_SnowboardEffectRetireAfterDelay.
// FUNCTION: FF7 0x007382a6
void __cdecl Mini_SnowboardEffectUpdateLaunchAccelerate(SnowboardEffect *effect)
{
    effect->accel += 8;
    effect->rotX += effect->accel;
    effect->matrixValid = 0;
    if (effect->rotX > 0x500) {
        effect->accel = 0;
        effect->update = Mini_SnowboardEffectRetireAfterDelay;
    }
}

// Returns when Mini_SnowboardEffectTickHitOrExpire reports a nonzero result. Otherwise sidePos and
// forward are set to baseX and baseZ plus SinFixed and CosFixed of tick << 5 plus phaseOffset, each
// times 0x1C0, and height is cleared. spinAngle gains 0x20 and matrixValid is cleared. height is then set to
// baseY plus SinFixed of tick << 6 plus phaseOffset, shifted left 6. Finally tick is incremented.
// FUNCTION: FF7 0x00738308
void __cdecl Mini_SnowboardEffectUpdateCircularOrbit(SnowboardEffect *effect)
{
    int phase;

    if (Mini_SnowboardEffectTickHitOrExpire(effect) != 0) {
        return;
    }
    phase = (effect->timer << 5) + effect->phaseOffset;
    effect->sidePos = effect->orbitX + SinFixed(phase) * 448;
    effect->forward = effect->orbitZ + CosFixed(phase) * 448;
    effect->height = 0;
    effect->rotY += 0x20;
    effect->matrixValid = 0;
    phase = (effect->timer << 6) + effect->phaseOffset;
    effect->height = effect->orbitY + (SinFixed(phase) << 6);
    (effect->timer)++;
}

/*
 * For a running timer, advances its short counter and after old counter 0x3C advances the unsigned
 * fade counter, deactivating and returning one after old fade 0x1E. Otherwise collision bit two
 * restores saved velocities, changes callback and plays a sound, returning one; other collision
 * begins the timer. Returns zero without a terminal transition.
 */
// FUNCTION: FF7 0x007383f7
int __cdecl Mini_SnowboardEffectTickHitOrExpire(SnowboardEffect *effect)
{
    if (effect->age != 0) {
        if ((effect->age)++ > 60) {
            if ((effect->fade)++ > 30) {
                effect->active = 0;
                return 1;
            }
        }
    } else {
        if (effect->contact != 0) {
            if ((effect->contact & 2) != 0) {
                effect->age = 0;
                effect->contactRadius = 0;
                effect->sideVelocity = effect->hitVelocityX;
                effect->forwardVelocity = effect->hitVelocityZ;
                effect->update = Mini_SnowboardEffectUpdateKnockedArcSpinFadeOut;
                Mini_SnowboardPlaySfxOnChannel(2, 0x10C, 0);
                return 1;
            } else {
                effect->age = 1;
            }
        }
    }
    return 0;
}

// Calls Mini_SnowboardEffectRetireAfterDelay, then advances sidePos by sideVelocity and forward by forwardVelocity,
// lowers fallVelocity by 0xC00 and advances height by fallVelocity. rotY gains spinRate, matrixValid is
// cleared, and cycleCounter changes by -4 when negative and by +4 otherwise.
// FUNCTION: FF7 0x007384e4
void __cdecl Mini_SnowboardEffectUpdateKnockedArcSpinFadeOut(struct SnowboardEffect *effect)
{
    Mini_SnowboardEffectRetireAfterDelay(effect);
    effect->sidePos += effect->sideVelocity;
    effect->forward += effect->forwardVelocity;
    effect->fallVelocity -= 3072;
    effect->height += effect->fallVelocity;
    effect->rotY += effect->spinRate;
    effect->matrixValid = 0;
    effect->cycleCounter += effect->cycleCounter < 0 ? -4 : 4;
}

// Returns when Mini_SnowboardEffectTickHitOrExpire reports a nonzero result. Otherwise phaseTicks
// is incremented, and height is set to baseY plus SinFixed of the phase from before this increment,
// shifted left 5. rotY then advances by spinRate and matrixValid is cleared.
// FUNCTION: FF7 0x0073857c
void __cdecl Mini_SnowboardEffectUpdateVerticalBob(SnowboardEffect *effect)
{
    int phase;

    if (Mini_SnowboardEffectTickHitOrExpire(effect) != 0) {
        return;
    }
    phase = (effect->timer << 5) + effect->phaseOffset;
    (effect->timer)++;
    effect->height = effect->restY + (SinFixed(phase) << 5);
    effect->rotY += effect->spinRate;
    effect->matrixValid = 0;
}

/*
 * Event hook for one object: when enabled is non-zero and countC has
 * reached 0x46 or more, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot003 above
 * object object through Mini_SnowboardSpawnEffectAboveObject. Does nothing otherwise.
 */
// FUNCTION: FF7 0x00738605
void __cdecl Mini_SnowboardSlot003SpawnEffectAboveGated(int object, int enabled)
{
    if (enabled != 0 && g_SnoboStatePtr->countC >= 0x46) {
        Mini_SnowboardSpawnEffectAboveObject((short)object, &g_SnoboAboveObjectDesc_Slot003.pos);
    }
}

// Spawns one effect above the object from the 16-byte descriptor desc. It returns without doing
// anything when the short at desc+8 is not 4 and activeFlags is non-zero. Otherwise the short at
// desc+8 is copied into g_SnoboAboveObjectEffectDef.pickupKind and .packedPhase, the staging record is filled by
// Mini_SnowboardFillEffectPosFromObject, its sidePos coordinate is lifted by subtracting 0x80000 (128
// in 20.12 fixed point), and the record goes to Mini_SnowboardAllocEffectNode with kind 1.
// FUNCTION: FF7 0x00738631
void __cdecl Mini_SnowboardSpawnEffectAboveObject(short object, struct SnowboardEffectPosOffset *desc)
{
    if (((SnowboardCompactEffectDesc *)desc)->param != 4 && g_SnoboStatePtr->activeFlags != 0) { /* empty first arm: lib/codegen.h */
    } else {
        g_SnoboAboveObjectEffectDef.pickupKind = ((SnowboardCompactEffectDesc *)desc)->param;
        g_SnoboAboveObjectEffectDef.packedPhase = ((SnowboardCompactEffectDesc *)desc)->param;
        Mini_SnowboardFillEffectPosFromObject((short)object, &g_SnoboAboveObjectEffectDef, desc);
        g_SnoboAboveObjectEffectDef.height -= 0x80000;
        Mini_SnowboardAllocEffectNode((short)object, &g_SnoboAboveObjectEffectDef, 1);
    }
}

// Fills the position fields of effect for object objIndex and descriptor desc. The twelve-byte
// entry for objIndex in g_SnoboStatePtr->effectPosTable supplies the object's coordinates.
// effect->rotY is copied from desc->tag, and sidePos, height and forward each become the descriptor
// offset (offsetX, offsetY or offsetZ) plus the entry coordinate, the sum shifted left 12 into 20.12 fixed
// point.
// FUNCTION: FF7 0x007386ab
void __cdecl Mini_SnowboardFillEffectPosFromObject(short objIndex, struct SnowboardEffectTemplate *effect, SnowboardEffectPosOffset *desc)
{
    SnowboardEffectPos *posEntry;

    posEntry = (SnowboardEffectPos *)(g_SnoboStatePtr->effectPosTable + objIndex * 0xC);
    effect->rotY = desc->tag;
    effect->sidePos = desc->offsetX + posEntry->posX << 12;
    effect->height = desc->offsetY + posEntry->posY << 12;
    effect->forward = desc->offsetZ + posEntry->posZ << 12;
}

// Event hook for one object. When spawnFlag is non-zero it spawns descriptor g_SnoboNodeOffsetsDesc_Slot012
// at the object's offsets through Mini_SnowboardSpawnEffectNodeAtObjectOffsets. It also spawns
// g_SnoboAboveObjectDesc_Slot012 above the object when activeFlags is non-zero and slotGate012 is non-zero
// and below 0x1050000, compared as an unsigned value. Independently of spawnFlag, when
// triggerLatched is still 0 and section equals object, triggerStage is set to 1 and triggerLatched
// to 1.
// FUNCTION: FF7 0x00738719
void __cdecl Mini_SnowboardSlot012SpawnEffectsAndLatchTrigger(int object, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectNodeAtObjectOffsets((short)object, &g_SnoboNodeOffsetsDesc_Slot012);
        if (g_SnoboStatePtr->activeFlags != 0 && g_SnoboStatePtr->slotGate012 != 0 && (unsigned int)g_SnoboStatePtr->slotGate012 < (unsigned int)0x1050000) {
            Mini_SnowboardSpawnEffectAboveObject((short)object, &g_SnoboAboveObjectDesc_Slot012.pos);
        }
    }
    if (g_SnoboStatePtr->triggerLatched == 0 && g_SnoboStatePtr->section == object) {
        g_SnoboStatePtr->triggerStage = 1;
        g_SnoboStatePtr->triggerLatched = 1;
    }
}

/*
 * Copies the spawn descriptor's offsetA and offsetB into g_SnoboEffectTemplateAtObject/g_SnoboEffectTemplateAtObject.model2, fills that template position from the
 * object and allocates one effect node with object.
 */
// FUNCTION: FF7 0x007387a8
void __cdecl Mini_SnowboardSpawnEffectNodeAtObjectOffsets(short object, void *desc)
{
    g_SnoboEffectTemplateAtObject.model = ((SnowboardEffectSpawnDesc *)desc)->offsetA;
    g_SnoboEffectTemplateAtObject.model2 = ((SnowboardEffectSpawnDesc *)desc)->offsetB;
    Mini_SnowboardFillEffectPosFromObject((short)object, &g_SnoboEffectTemplateAtObject, (struct SnowboardEffectPosOffset *)desc);
    Mini_SnowboardAllocEffectNode((short)object, &g_SnoboEffectTemplateAtObject, 1);
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot018 above
 * object object via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. One of sixteen
 * identical wrappers at (int)Mini_SnowboardSlot018SpawnEffectAbove through (int)Mini_SnowboardSlot106SpawnEffectAbove, each differing only in which 16-byte
 * descriptor of the 0x10-stride table it names.
 */
// FUNCTION: FF7 0x007387f2
void __cdecl Mini_SnowboardSlot018SpawnEffectAbove(int object, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)object, &g_SnoboAboveObjectDesc_Slot018.pos);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot022 above
 * object object via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x0073880f
void __cdecl Mini_SnowboardSlot022SpawnEffectAbove(int object, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)object, g_SnoboAboveObjectDesc_Slot022);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot029 above
 * object object via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x0073882c
void __cdecl Mini_SnowboardSlot029SpawnEffectAbove(int object, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)object, g_SnoboAboveObjectDesc_Slot029);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot035 above
 * object object via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x00738849
void __cdecl Mini_SnowboardSlot035SpawnEffectAbove(int object, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)object, g_SnoboAboveObjectDesc_Slot035);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot040 above
 * object object via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x00738866
void __cdecl Mini_SnowboardSlot040SpawnEffectAbove(int object, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)object, g_SnoboAboveObjectDesc_Slot040);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot048 above
 * object object via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x00738883
void __cdecl Mini_SnowboardSlot048SpawnEffectAbove(int object, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)object, g_SnoboAboveObjectDesc_Slot048);
    }
}
} /* extern "C" */

extern "C" {
#pragma intrinsic(abs)
// GLOBAL: FF7 0x00963368
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot053[2] = {{2720, 252, 55, -745}, {0}};
// GLOBAL: FF7 0x00963378
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot056[2] = {{3480, -672, 67, -454}, {0}};
// GLOBAL: FF7 0x00963388
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot059[2] = {{2911, -568, 324, -1084}, {0}};
// GLOBAL: FF7 0x00963398
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot066[2] = {{3906, -1394, 243, 733}, {0}};
// GLOBAL: FF7 0x009633a8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot070[2] = {{0x102f, -501, 58, 200}, {0}};
// GLOBAL: FF7 0x009633b8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot076[2] = {{3527, -1273, 511, 72}, {0}};
// GLOBAL: FF7 0x009633c8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot082[2] = {{2531, -441, 670, -1404}, {0}};
// GLOBAL: FF7 0x009633d8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot090[2] = {{0x153a, 1050, 192, 219}, {0}};
// GLOBAL: FF7 0x009633e8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot099[2] = {{0x11e5, 58, 190, 1068}, {0}};
// GLOBAL: FF7 0x009633f8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot106[2] = {{0x138f, 626, 193, 889}, {0}};
// GLOBAL: FF7 0x00963408
int g_SnoboHopWaitDesc_Slot108[4] = {0x0437128f, 0x067f0152, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963418
struct SnowboardEffectDesc g_SnoboHitLaunchDesc_Slot112 = {{0xF8F, -939, 513, 878}, {1}};
// GLOBAL: FF7 0x00963428
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot113[2] = {{{0xF8F, -1232, 264, 305}, 1}, {{0xF8F, -460, 264, 439}, 1}};
// GLOBAL: FF7 0x00963440
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot113 = {{0xF8F, -875, 257, 373}, {1}};
// GLOBAL: FF7 0x00963450
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot119[2] = {{0x10db, -37, 326, 1463}, {0}};
// GLOBAL: FF7 0x00963460
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot125[2] = {{3225, -509, 128, -691}, {0}};
// GLOBAL: FF7 0x00963470
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot137[2] = {{3840, -974, 179, 248}, {0}};
// GLOBAL: FF7 0x00963480
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot140 = {{0x1000, -768, 92, 1020}, {1}};
// GLOBAL: FF7 0x00963490
struct SnowboardEffectPosOffset g_SnoboLateralDriftDesc_Slot142Group = {0xA00, -2943, 16, 62};
// GLOBAL: FF7 0x00963498
int g_SnoboGatedModelAEDesc_Slot142Group[4] = {(-192870912), 0, 0, 0};
// GLOBAL: FF7 0x009634a8
struct SnowboardEffectPosOffset g_SnoboLateralDriftDesc_Slot143Group = {0x600, -128, 15, 54};
// GLOBAL: FF7 0x009634b0
int g_SnoboGatedModelAEDesc_Slot143Group[4] = {(-8386048), 0, 0, 0};
// GLOBAL: FF7 0x009634c0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot148[2] = {{0x1000, -1536, 199, 746}, {0}};
// GLOBAL: FF7 0x009634d0
int g_SnoboSplitBallisticDesc_Slot149[4] = {0xf6fc0000, 0x00f70042, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x009634e0
struct SnowboardEffectDesc g_SnoboApproachFollowDesc_Slot149 = {{0x800, -2308, 66, 247}, {0x800, 0x3800}};
// GLOBAL: FF7 0x009634f0
int g_SnoboSplitBallisticDesc_Slot150[4] = {0xfd040000, 0x00fc0043, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963500
struct SnowboardEffectDesc g_SnoboApproachFollowDesc_Slot150 = {{0x800, -764, 67, 252}, {0, 0x3800}};
// GLOBAL: FF7 0x00963510
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot154[2] = {{0x1000, -2049, 132, 494}, {0}};
// GLOBAL: FF7 0x00963520
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot159[2] = {{0x1000, -1025, 199, 746}, {0}};
// GLOBAL: FF7 0x00963530
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot167[2] = {{0x1000, -1536, 132, 496}, {0}};
// GLOBAL: FF7 0x00963540
short g_SnoboGatedModelAEDesc_Slot168[25] = {2560, -529, 332, 1240, 0, 5632, -1040, 332, 1240, 0, 2560, -1552, 332, 1240, 0, 5632, -2048, 332, 1240, 0, 2560, -2568, 332, 1240, 0};
// GLOBAL: FF7 0x00963578
short g_SnoboGatedModelAEDesc_Slot169[32] = {2560, -262, 65, 240, 0, 5632, -774, 65, 240, 0, 2560, -1296, 65, 240, 0, 2560, -1798, 65, 240, 0, 5632, -2302, 65, 240, 0, 2560, -2806, 65, 240, 0, 0, 0};
// GLOBAL: FF7 0x009635b8
struct SnowboardEffectPosOffset g_SnoboLateralDriftChildDesc_Slot172Group = {0xA00, -2943, 16, 62};
// GLOBAL: FF7 0x009635c0
int g_SnoboGatedModelAEDesc_Slot172Group[4] = {(-192870912), 0, 0, 0};
// GLOBAL: FF7 0x009635d0
struct SnowboardEffectPosOffset g_SnoboLateralDriftChildDesc_Slot173Group = {0x600, -128, 15, 54};
// GLOBAL: FF7 0x009635d8
int g_SnoboGatedModelAEDesc_Slot173Group[4] = {(-8386048), 0, 0, 0};
// GLOBAL: FF7 0x009635e8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot174[2] = {{0x1000, -2312, 331, 1235}, {0}};
// GLOBAL: FF7 0x009635f8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot178[2] = {{0x1000, -1536, 265, 990}, {0}};
// GLOBAL: FF7 0x00963608
int g_SnoboSplitBallisticDesc_Slot179[4] = {0xf6f80000, 0x00f60042, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963618
struct SnowboardEffectDesc g_SnoboKind2Desc_Slot179 = {{0x800, -2312, 66, 246}, {0x800, 0x2800}};
// GLOBAL: FF7 0x00963628
int g_SnoboSplitBallisticDesc_Slot180[4] = {0xfd000000, 0x00f30041, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963638
struct SnowboardEffectDesc g_SnoboKind2Desc_Slot180 = {{0x800, -768, 65, 243}, {0, 0x1800}};
// GLOBAL: FF7 0x00963648
int g_SnoboSplitBallisticDesc_Slot181[4] = {0xf9ff0000, 0x00f80042, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963658
struct SnowboardEffectDesc g_SnoboKind2Desc_Slot181 = {{0x800, -1537, 66, 248}, {0x800, 0x2800}};
// GLOBAL: FF7 0x00963668
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot184[2] = {{0x1000, -776, 331, 1237}, {0}};
// GLOBAL: FF7 0x00963678
struct SnowboardEffectDesc g_SnoboSplitBallisticDesc_Slot194 = {{0, -1536, 198, 743}, {4}};
// GLOBAL: FF7 0x00963688
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot195[2] = {{{0x1000, -2056, 198, 741}, 2}, {{0x1000, -1040, 198, 741}, 3}};
// GLOBAL: FF7 0x009636a0
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot196[2] = {{{0x1000, -2584, 198, 741}, 4}, {{0x1000, -512, 198, 741}, 2}};
// GLOBAL: FF7 0x009636b8
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot197[2] = {{{0x1000, -2047, 200, 747}, 1}, {{0x1000, -1023, 200, 747}, 3}};
// GLOBAL: FF7 0x009636d0
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot200 = {{0x1000, -768, -500, 764}, {2}};
// GLOBAL: FF7 0x009636e0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot204[2] = {{0x1000, -768, 75, 509}, {0}};
// GLOBAL: FF7 0x009636f0
struct SnowboardEffectDesc g_SnoboNodeOffsetsDesc_Slot205 = {{0x800, 0, 0, 0}, {239, 247}};
// GLOBAL: FF7 0x00963700
SnowboardEffectDesc g_SnoboNodeEffectPair_Slot206[2] = {{{0xF3F, 40, 9, -126}, {238, 248}}, {{0, 0, 0, 0}, {236, 247}}};
// GLOBAL: FF7 0x00963720
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot219A = {{0x800, 772, 140, -1278}, {1}};
// GLOBAL: FF7 0x00963730
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot219 = {0, 768, 513, -1025};
// GLOBAL: FF7 0x00963738
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot219B = {{0x800, 768, 0, 0}, {4}};
// GLOBAL: FF7 0x00963748
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot221 = {0, 384, 380, -769};
// GLOBAL: FF7 0x00963750
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot221[2] = {{2048, 769, 119, -257}, {0}};
// GLOBAL: FF7 0x00963760
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot222 = {0, 1160, 380, -769};
// GLOBAL: FF7 0x00963768
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot224 = {0, 640, 380, -769};
// GLOBAL: FF7 0x00963770
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot225 = {0, 896, 380, -769};
// GLOBAL: FF7 0x00963778
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot226 = {0, 128, 628, -1273};
// GLOBAL: FF7 0x00963780
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot226[2] = {{2048, 769, 513, -1025}, {0}};
// GLOBAL: FF7 0x00963790
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot227 = {0, 1407, 628, -1273};
// GLOBAL: FF7 0x00963798
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot228 = {0, 769, 116, -249};
// GLOBAL: FF7 0x009637a0
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot229A = {0, 431, 267, -513};
// GLOBAL: FF7 0x009637a8
struct SnowboardEffectPosOffset g_SnoboPlainDesc_Slot229B = {0, 1088, 269, -513};
// GLOBAL: FF7 0x009637b0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot233[2] = {{3013, -563, 412, -966}, {0}};
// GLOBAL: FF7 0x009637c0
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot237 = {0, -1023, 386, 771};
// GLOBAL: FF7 0x009637c8
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot238 = {0, -511, 381, 763};
// GLOBAL: FF7 0x009637d0
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot239 = {0, -1279, 513, 1027};
// GLOBAL: FF7 0x009637d8
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot240 = {0, -767, 372, 763};
// GLOBAL: FF7 0x009637e0
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot241 = {0, -255, 513, 1027};
// GLOBAL: FF7 0x009637e8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot242[2] = {{0x1000, -773, 513, 1024}, {0}};
// GLOBAL: FF7 0x009637f8
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot242 = {0, -951, 269, 515};
// GLOBAL: FF7 0x00963800
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot243 = {0, -591, 268, 515};
// GLOBAL: FF7 0x00963808
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot244 = {0, -1151, 265, 515};
// GLOBAL: FF7 0x00963810
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot245 = {0, -375, 260, 507};
// GLOBAL: FF7 0x00963818
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot248[2] = {{0x13c7, 569, 415, 966}, {0}};
// GLOBAL: FF7 0x00963828
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot252 = {0x800, 768, 376, -770};
// GLOBAL: FF7 0x00963830
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot253A = {0x800, 1152, 265, -514};
// GLOBAL: FF7 0x00963838
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot253B = {0x800, 376, 264, -514};
// GLOBAL: FF7 0x00963840
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot254[2] = {{2048, 768, 376, -770}, {0}};
// GLOBAL: FF7 0x00963850
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot255A = {0x800, 1400, 380, -770};
// GLOBAL: FF7 0x00963858
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot255B = {0x800, 136, 380, -770};
// GLOBAL: FF7 0x00963860
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot256 = {0x800, 768, 513, -1026};
// GLOBAL: FF7 0x00963868
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot257A = {0x800, 1024, 385, -770};
// GLOBAL: FF7 0x00963870
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot257B = {0x800, 512, 385, -770};
// GLOBAL: FF7 0x00963878
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot258A = {0x800, 1280, 376, -770};
// GLOBAL: FF7 0x00963880
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot258B = {0x800, 256, 376, -770};
// GLOBAL: FF7 0x00963888
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot259 = {0x800, 768, 376, -770};
// GLOBAL: FF7 0x00963890
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot260A = {0x800, 1160, 7, 2};
// GLOBAL: FF7 0x00963898
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot260B = {0x800, 376, 7, 2};
// GLOBAL: FF7 0x009638a0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot260[2] = {{2048, 768, 513, -1026}, {0}};
// GLOBAL: FF7 0x009638b0
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot261 = {0x800, 515, 637, -1276};
// GLOBAL: FF7 0x009638b8
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot262 = {0x800, 1027, 382, -764};
// GLOBAL: FF7 0x009638c0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot265[2] = {{3015, -567, 413, -967}, {0}};
// GLOBAL: FF7 0x009638d0
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot269A = {0, -509, 637, 1276};
// GLOBAL: FF7 0x009638d8
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot269B = {0, -766, 751, 1533};
// GLOBAL: FF7 0x009638e0
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot269C = {0, -261, 751, 1532};
// GLOBAL: FF7 0x009638e8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot271[2] = {{0x1000, -773, 252, 506}, {0}};
// GLOBAL: FF7 0x009638f8
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot272A = {0, -1029, 382, 764};
// GLOBAL: FF7 0x00963900
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot272B = {0, -1277, 513, 1028};
// GLOBAL: FF7 0x00963908
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot272C = {0, -773, 514, 1028};
// GLOBAL: FF7 0x00963910
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot274[2] = {{0x1000, -767, -14, 2}, {0}};
// GLOBAL: FF7 0x00963920
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot275A = {0, -773, 255, 509};
// GLOBAL: FF7 0x00963928
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot275B = {0, -1029, 382, 764};
// GLOBAL: FF7 0x00963930
struct SnowboardEffectPosOffset g_SnoboAtObjectDesc_Slot275C = {0, -517, 382, 764};
// GLOBAL: FF7 0x00963938
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot281[2] = {{0x1600, 580, 168, 20}, {0}};
// GLOBAL: FF7 0x00963948
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot286[2] = {{0x1391, 303, 361, 1312}, {0}};
// GLOBAL: FF7 0x00963958
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot295[2] = {{0x17c7, 790, 131, -471}, {0}};
// GLOBAL: FF7 0x00963968
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot301 = {{0x14AB, 930, 62, 543}, {1}};
// GLOBAL: FF7 0x00963978
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot302[2] = {{{0x14AB, 930, 62, 543}, 0}, {{0xCAB, 930, 62, 543}, 0x800}};
// GLOBAL: FF7 0x00963990
SnowboardEffectOffsetDesc g_SnoboOffsetEffect_Slot303[2] = {{{0, 697, 49, 894}, 64, -32, 0}, {{0, 614, 57, 389}, 64, 32, 0x800}};
// GLOBAL: FF7 0x009639b0
SnowboardEffectOffsetDesc g_SnoboOffsetEffect_Slot304[2] = {{{0, 629, 68, 1323}, 64, -32, 0}, {{0, 767, 80, 308}, 64, 32, 0x800}};
// GLOBAL: FF7 0x009639d0
SnowboardEffectOffsetDesc g_SnoboOffsetEffect_Slot305[2] = {{{0, 801, 89, 1395}, 64, -32, 0}, {{0, 993, 93, 644}, 64, 32, 0x800}};
// GLOBAL: FF7 0x009639f0
SnowboardEffectOffsetDesc g_SnoboOffsetEffect_Slot306[2] = {{{0, 376, 63, 1560}, 64, -32, 0}, {{0, 707, 65, 315}, 64, 32, 0x800}};
// GLOBAL: FF7 0x00963a10
struct SnowboardEffectDesc g_SnoboOffsetsDesc = {{0, 245, 52, 879}, {64, 32}};
// GLOBAL: FF7 0x00963a20
SnowboardEffectOffsetDesc g_SnoboOffsetEffect_Slot308[2] = {{{0, -207, 46, 1233}, 64, -32, 0}, {{0, 237, 58, 597}, 64, 32, 0x800}};
// GLOBAL: FF7 0x00963a40
struct SnowboardEffectDesc g_SnoboHopWaitDesc_Slot309 = {{0x1100, 3, 203, 1803}, {1}};
// GLOBAL: FF7 0x00963a50
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot309[2] = {{0x1200, -365, 42, 725}, {0}};
// GLOBAL: FF7 0x00963a60
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot310[2] = {{{0x1200, -92, 37, 447}, 2}, {{0x1200, -629, 36, 984}, 2}};
// GLOBAL: FF7 0x00963a78
struct SnowboardEffectDesc g_SnoboModelB2Desc_Slot314 = {{0xA00, 2, 375, -1085}, {2}};
// GLOBAL: FF7 0x00963a88
struct SnowboardEffectDesc g_SnoboHitLaunchDesc_Slot317 = {{0xA25, -225, 877, -1718}, {2}};
// GLOBAL: FF7 0x00963a98
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot318[2] = {{2673, -198, 496, -1116}, {0}};
// GLOBAL: FF7 0x00963aa8
struct SnowboardEffectDesc g_SnoboHitLaunchDesc_Slot319 = {{0xAF6, -755, 714, -950}, {2}};
// GLOBAL: FF7 0x00963ab8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot323[2] = {{3280, -1539, 881, -404}, {0}};
// GLOBAL: FF7 0x00963ac8
struct SnowboardEffectDesc g_SnoboHitLaunchDesc_Slot323 = {{0xC97, -585, 376, -141}, {2}};
// GLOBAL: FF7 0x00963ad8
struct SnowboardEffectDesc g_SnoboHitLaunchDesc_Slot326 = {{0xDC7, -1219, 275, -652}, {2}};
// GLOBAL: FF7 0x00963ae8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot328[2] = {{3243, -976, 191, -441}, {0}};
// GLOBAL: FF7 0x00963af8
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot329[2] = {{{0xCC6, -1059, 189, -690}, 2}, {{0xCC6, -904, 188, -194}, 2}};
// GLOBAL: FF7 0x00963b10
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot332 = {{0x5C7, -931, 327, -1111}, {1}};
// GLOBAL: FF7 0x00963b20
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot333 = {{0x5C7, -974, 512, -598}, {1}};
// GLOBAL: FF7 0x00963b30
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot334 = {{0x5C7, -941, 334, -986}, {1}};
// GLOBAL: FF7 0x00963b40
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot335 = {{0x5C7, -486, 255, -295}, {1}};
// GLOBAL: FF7 0x00963b50
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot336 = {{0x5C7, -408, 161, -1201}, {1}};
// GLOBAL: FF7 0x00963b60
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot337 = {{0x11C7, -475, 261, -418}, {1}};
// GLOBAL: FF7 0x00963b70
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot338 = {{0x5C7, -960, 344, -850}, {1}};
// GLOBAL: FF7 0x00963b80
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot339[2] = {{{0x5C7, -409, 264, -1136}, 1}, {{0x11C7, -985, 511, -465}, 1}};
// GLOBAL: FF7 0x00963b98
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot340[2] = {{{0x5C7, -929, 327, -1108}, 1}, {{0x11C7, -979, 335, -589}, 1}};
// GLOBAL: FF7 0x00963bb0
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot340 = {{0xBC7, -961, 344, -854}, {1}};
// GLOBAL: FF7 0x00963bc0
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot341 = {{0x5C7, -995, 510, -340}, {1}};
// GLOBAL: FF7 0x00963bd0
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot342 = {{0x5C7, -429, 167, -940}, {1}};
// GLOBAL: FF7 0x00963be0
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot343 = {{0x5C7, -1217, 631, -740}, {1}};
// GLOBAL: FF7 0x00963bf0
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot344 = {{0x5C7, -926, 327, -1234}, {1}};
// GLOBAL: FF7 0x00963c00
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot345 = {{0x11C7, -991, 513, -467}, {1}};
// GLOBAL: FF7 0x00963c10
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot346[2] = {{{0x5C7, -897, 329, -1468}, 1}, {{0x11C7, -495, 161, -165}, 1}};
// GLOBAL: FF7 0x00963c28
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot347 = {{0x5C7, -171, 131, -1047}, {1}};
// GLOBAL: FF7 0x00963c38
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot348 = {{0x5C7, -481, 157, -416}, {1}};
// GLOBAL: FF7 0x00963c48
struct SnowboardEffectDesc g_SnoboModelAEDesc_Slot349 = {{0x5C7, -415, 267, -1187}, {1}};
// GLOBAL: FF7 0x00963c58
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot350 = {{0xBC7, -958, -320, -855}, {2}};
// GLOBAL: FF7 0x00963c68
struct SnowboardEffectDesc g_SnoboModelB2Desc_Slot350 = {{0xBC7, -958, 343, -855}, {2}};
// GLOBAL: FF7 0x00963c78
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot355[2] = {{3754, -882, 112, -48}, {0}};
// GLOBAL: FF7 0x00963c88
int g_SnoboHopWaitDesc_Slot356[4] = {0xf9980eaa, 0x031a0135, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963c98
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot360[2] = {{2787, -168, 130, -901}, {0}};
// GLOBAL: FF7 0x00963ca8
struct SnowboardEffectDesc g_SnoboHopWaitDesc_Slot361 = {{0xAE3, -816, 318, -1057}, {1}};
// GLOBAL: FF7 0x00963cb8
struct SnowboardEffectDesc g_SnoboModelB2Desc_Slot364 = {{0xD39, -1464, 325, -57}, {2}};
// GLOBAL: FF7 0x00963cc8
int g_SnoboHopWaitDesc_Slot366[4] = {0xfb050ae3, 0xfb5c013f, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963cd8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot371[2] = {{2361, 590, 49, -538}, {0}};
// GLOBAL: FF7 0x00963ce8
int g_SnoboHopWaitDesc_Slot373[4] = {0x009908c7, 0xf8f00142, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963cf8
struct SnowboardEffectDesc g_SnoboModelB2Desc_Slot378 = {{0x1439, 599, 131, 691}, {2}};
// GLOBAL: FF7 0x00963d08
struct SnowboardEffectDesc g_SnoboHopWaitDesc_Slot380 = {{0x14C8, 1252, 319, 360}, {1}};
// GLOBAL: FF7 0x00963d18
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot384[2] = {{2191, 719, 48, -365}, {0}};
// GLOBAL: FF7 0x00963d28
int g_SnoboHopWaitDesc_Slot385[4] = {0x005a09aa, 0xfad60135, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963d38
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot390[2] = {{0x150b, 1263, 387, -208}, {0}};
// GLOBAL: FF7 0x00963d48
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot393[2] = {{0x13c7, 870, 394, 808}, {0}};
// GLOBAL: FF7 0x00963d58
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot396[2] = {{0x125f, 361, 467, 1607}, {0}};
// GLOBAL: FF7 0x00963d68
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot402[2] = {{0x12ff, 242, 176, 976}, {0}};
// GLOBAL: FF7 0x00963d78
int g_SnoboHopWaitDesc_Slot407[4] = {0x05d61556, 0x001701aa, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963d88
struct SnowboardEffectDesc g_SnoboHitLaunchDesc_Slot410 = {{0x1172, -538, 49, 586}, {2}};
// GLOBAL: FF7 0x00963d98
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot411[2] = {{{0x1156, -354, 208, 1679}, 2}, {{0x1156, 319, 206, 1284}, 2}};
// GLOBAL: FF7 0x00963db0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot418[2] = {{0x1156, -281, 117, 1044}, {0}};
// GLOBAL: FF7 0x00963dc0
struct SnowboardEffectDesc g_SnoboNodeOffsetsDesc_Slot420 = {{0x956, 0, 0, 0}, {239, 247}};
// GLOBAL: FF7 0x00963dd0
struct SnowboardEffectDesc g_SnoboNodeOffsetsDesc_Slot421 = {{0xB3E, 136, 10, 43}, {238, 248}};
// GLOBAL: FF7 0x00963de0
struct SnowboardEffectDesc g_SnoboNodeOffsetsDesc_Slot425 = {{0x800, 0, 0, 0}, {237, 247}};
// GLOBAL: FF7 0x00963df0
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot425 = {{0x1000, -768, 0, 0}, {4}};
// GLOBAL: FF7 0x00963e00
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot426[2] = {{{0x1239, -299, 119, 752}, 0}, {{0x1239, 290, 511, 1245}, 0}};
// GLOBAL: FF7 0x00963e18
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot427[2] = {{{0x1239, -299, 84, 751}, 0}, {{0x1239, 290, 340, 1244}, 0}};
// GLOBAL: FF7 0x00963e30
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot428[2] = {{{0x1239, -299, 41, 749}, 0}, {{0x1239, 295, 161, 1247}, 0}};
// GLOBAL: FF7 0x00963e48
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot429[2] = {{{0x1239, -294, 1, 755}, 0}, {{0x1239, 294, 7, 1247}, 0}};
// GLOBAL: FF7 0x00963e60
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot437 = {{0x1239, 480, -650, 1404}, {1}};
// GLOBAL: FF7 0x00963e70
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot454[2] = {{0x1639, 1479, 375, 234}, {0}};
// GLOBAL: FF7 0x00963e80
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot459[2] = {{2616, 1, 246, -736}, {0}};
// GLOBAL: FF7 0x00963e90
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot463[2] = {{0x1639, 261, 17, 142}, {0}};
// GLOBAL: FF7 0x00963ea0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot467[2] = {{0x1239, -215, 126, 659}, {0}};
// GLOBAL: FF7 0x00963eb0
int g_SnoboHitLaunchDesc_Slot473[4] = {0xfe171239, 0x024dfff3, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00963ec0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot488[2] = {{0x12a7, 17, 82, 481}, {0}};
// GLOBAL: FF7 0x00963ed0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot493 = {{0x1356, 130, 42, 292}, {0x220, 0, 1}};
// GLOBAL: FF7 0x00963ee0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot494 = {{0x11C9, -111, 84, 610}, {0x240, 0, 1}};
// GLOBAL: FF7 0x00963ef0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot495 = {{0x10A8, -310, 44, 299}, {0x230, 0, 1}};
// GLOBAL: FF7 0x00963f00
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot496 = {{0xEE4, -415, 128, 465}, {0x240, 0, 1}};
// GLOBAL: FF7 0x00963f10
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot497 = {{0xEAB, -711, 85, -101}, {0x230, 0, 1}};
// GLOBAL: FF7 0x00963f20
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot498 = {{0x1004, -274, 88, 275}, {0x220, 0, 1}};
// GLOBAL: FF7 0x00963f30
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot499 = {{0x1158, -320, 87, 501}, {0x240, 0, 1}};
// GLOBAL: FF7 0x00963f40
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot500 = {{0x12A0, 36, 88, 487}, {0x240}};
// GLOBAL: FF7 0x00963f50
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot501 = {{0x143B, 452, 130, 525}, {0x230, 0, 1}};
// GLOBAL: FF7 0x00963f60
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot502 = {{0x1557, 564, 87, 438}, {0x240}};
// GLOBAL: FF7 0x00963f70
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot503 = {{0x16E4, 443, 129, -233}, {0x250}};
// GLOBAL: FF7 0x00963f80
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot504 = {{0x16C9, 740, 147, -394}, {0x220, 0, 1}};
// GLOBAL: FF7 0x00963f90
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot505 = {{0x15AF, 546, 104, -87}, {0x240}};
// GLOBAL: FF7 0x00963fa0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot506 = {{0x141F, 702, 148, 318}, {0x220, 0, 1}};
// GLOBAL: FF7 0x00963fb0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot507 = {{0x131B, 353, 87, 225}, {0x200, 0, 1}};
// GLOBAL: FF7 0x00963fc0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot508 = {{0x118D, 46, 129, 703}, {0x200, 0, 1}};
// GLOBAL: FF7 0x00963fd0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot509 = {{0x1039, -377, 128, 631}, {0x240}};
// GLOBAL: FF7 0x00963fe0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot510 = {{0xE71, -905, 214, 562}, {0x220, 0, 1}};
// GLOBAL: FF7 0x00963ff0
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot513 = {{0xCE4, -398, 1, -1080}, {0x380, 1}};
// GLOBAL: FF7 0x00964000
struct SnowboardEffectPosOffset g_SnoboEffectAboveOffset[2] = {{3300, -1494, 203, -372}, {0}};
// GLOBAL: FF7 0x00964010
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot514 = {{0xE05, -931, 217, 6}, {0x240, 0, 1}};
// GLOBAL: FF7 0x00964020
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot515 = {{0xE77, -298, 46, -30}, {0x220}};
// GLOBAL: FF7 0x00964030
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot516 = {{0xFCA, -400, 44, 105}, {0x230}};
// GLOBAL: FF7 0x00964040
struct SnowboardEffectDesc g_SnoboRaisedDesc_Slot519 = {{0x1239, -367, 1, 448}, {0x380, 1}};
// GLOBAL: FF7 0x00964050
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot519[2] = {{0x1239, 421, 203, 1484}, {0}};
// GLOBAL: FF7 0x00964060
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot532 = {{0x1239, 532, 240, 952}, {1}};
// GLOBAL: FF7 0x00964070
int g_SnoboTileDesc_Slot537[4] = {0xfe581239, 0x051b0081, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00964080
struct SnowboardEffectDesc g_SnoboTileDesc_Slot540 = {{0x12E4, 159, 193, 1486}, {1}};
// GLOBAL: FF7 0x00964090
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot540[2] = {{0x12e4, 475, 191, 781}, {0}};
// GLOBAL: FF7 0x009640a0
int g_SnoboTileDesc_Slot543[4] = {0x0144138f, 0x0447007f, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x009640b0
struct SnowboardEffectDesc g_SnoboTileDesc_Slot544 = {{0x138F, 727, 189, 259}, {1}};
// GLOBAL: FF7 0x009640c0
int g_SnoboTileDesc_Slot545[4] = {0x04af1439, 0x00380100, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x009640d0
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot546[2] = {{0x1439, 808, 185, 711}, {0}};
// GLOBAL: FF7 0x009640e0
struct SnowboardEffectDesc g_SnoboTileDesc_Slot549 = {{0x1556, 1113, 155, 320}, {1}};
// GLOBAL: FF7 0x009640f0
int g_SnoboTileDesc_Slot552[4] = {0x02f414e4, 0xff6800be, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00964100
struct SnowboardEffectDesc g_SnoboTileDesc_Slot553 = {{0x143A, 1649, 250, 669}, {1}};
// GLOBAL: FF7 0x00964110
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot553[2] = {{0x1446, 1489, 236, 45}, {0}};
// GLOBAL: FF7 0x00964120
int g_SnoboTileDesc_Slot560[4] = {0x010a1272, 0x05c6009d, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00964130
int g_SnoboTileDesc_Slot561[4] = {0x00911239, 0x0260007d, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00964140
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot561[2] = {{0x1239, -494, 5, 587}, {0}};
// GLOBAL: FF7 0x00964150
SnowboardCompactEffectDesc g_SnoboCompactEffect_Slot562[2] = {{{0x1239, -430, 127, 1306}, 0}, {{0x1239, 221, 125, 516}, 0}};
// GLOBAL: FF7 0x00964168
int g_SnoboHitLaunchDesc_Slot574[4] = {0x03e71439, 0x027f02e2, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00964178
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot575[2] = {{0x13c7, 870, 736, 804}, {0}};
// GLOBAL: FF7 0x00964188
int g_SnoboHitLaunchDesc_Slot576[4] = {0x030f1356, 0x02b802e0, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x00964198
int g_SnoboHitLaunchDesc_Slot577[4] = {0x01b112e4, 0x050402f8, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x009641a8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot578[2] = {{0x1272, 351, 735, 1128}, {0}};
// GLOBAL: FF7 0x009641b8
int g_SnoboHitLaunchDesc_Slot579[4] = {0xff251200, 0x06000328, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x009641c8
int g_SnoboHitLaunchDesc_Slot580[4] = {0x0171128f, 0x037d02a4, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x009641d8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot581[2] = {{0x111d, -258, 740, 1161}, {0}};
// GLOBAL: FF7 0x009641e8
int g_SnoboHitLaunchDesc_Slot582[4] = {0xfe3510ab, 0x044b02e5, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x009641f8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot589[2] = {{0x1072, -834, 383, 926}, {0}};
// GLOBAL: FF7 0x00964208
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot603 = {{0x1072, -623, 64, 885}, {0, 4, 4, 1}};
// GLOBAL: FF7 0x00964218
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot603[2] = {{0x1072, -623, 64, 885}, {0}};
// GLOBAL: FF7 0x00964228
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot607 = {{0x1472, 886, 64, 624}, {0, 4, 4}};
// GLOBAL: FF7 0x00964238
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot611A = {{0x1072, -370, 64, 841}, {0, 5, 4}};
// GLOBAL: FF7 0x00964248
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot611B = {{0x1072, -876, 55, 931}, {128, 5, 4}};
// GLOBAL: FF7 0x00964258
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot611 = {{0x1072, -580, 75, 1130}, {1}};
// GLOBAL: FF7 0x00964268
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot616 = {{0x1472, 880, 62, 626}, {0, 5, 5, 1}};
// GLOBAL: FF7 0x00964278
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot620A = {{0x871, 670, 33, -634}, {0, 6, 4}};
// GLOBAL: FF7 0x00964288
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot620B = {{0x871, 583, 75, -1130}, {64, 5, 4}};
// GLOBAL: FF7 0x00964298
struct SnowboardEffectDesc g_SnoboAboveObjectDesc_Slot620 = {{0x871, 625, 64, -884}, {2}};
// GLOBAL: FF7 0x009642a8
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot627 = {{0x1072, -622, 62, 879}, {0, 5, 4}};
// GLOBAL: FF7 0x009642b8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot627[2] = {{0x1072, -665, 33, 630}, {0}};
// GLOBAL: FF7 0x009642c8
struct SnowboardEffectDesc g_SnoboLinkedPairDesc_Slot631 = {{0x1472, 882, 62, 627}, {0, 5, 5, 1}};
// GLOBAL: FF7 0x009642d8
struct SnowboardEffectPosOffset g_SnoboAboveObjectDesc_Slot634[2] = {{0x11c8, -259, 77, 886}, {0}};
// GLOBAL: FF7 0x009642e8
struct SnowboardEffectDesc g_SnoboNodeOffsetsDesc_Slot635 = {{0x9C8, 0, 0, 0}, {239, 247}};
// clang-format on

// GLOBAL: FF7 0x00ddad18
unsigned char g_SnoboEventVisitFlags[640] = {0};
// GLOBAL: FF7 0x00ddaf98
SnowboardEffect g_SnoboEffectRecords[32] = {0};
// GLOBAL: FF7 0x00ddc198
short g_SnoboValue = 0;
// GLOBAL: FF7 0x00ddc19c
short g_SnoboSignalValue = 0;
// clang-format on
struct SnowboardMatrixRecord;
#pragma intrinsic(abs)

// Primary snowboard event callbacks indexed by descriptor id:
// 636 slots at 0x009642F8..0x00964CE8, forming the final .data table
// of this translation unit.
// GLOBAL: FF7 0x009642f8
SnowboardCallback g_SnoboEffectCallbacks[636] = {
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot003SpawnEffectAboveGated,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot012SpawnEffectsAndLatchTrigger,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot018SpawnEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot022SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot029SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot035SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot040SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot048SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot053SpawnEffectAbove,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot056SpawnEffectAbove,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot059SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot066SpawnEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot070SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot076SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot082SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot090SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot099SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot106SpawnEffectAbove,
    0,
    (SnowboardCallback)Mini_SnowboardSlot108QueueGatedEffect,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot112QueueGatedEffect,
    (SnowboardCallback)Mini_SnowboardSlot113SpawnEffectAndQueueTwoGated,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot119SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot125SpawnEffectAbove,
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
    (SnowboardCallback)Mini_SnowboardSlot137SpawnEffectAbove,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot140SpawnEffectAboveSetRiderAnim,
    0,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_1,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_2,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_1,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_2,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_1,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_2,
    (SnowboardCallback)Mini_SnowboardSlot148SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot149QueueGatedAndOffsetEffect,
    (SnowboardCallback)Mini_SnowboardSlot150QueueGatedAndOffsetEffect,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot154SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_1,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_2,
    (SnowboardCallback)Mini_SnowboardSlot159SpawnEffectAbove,
    0,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_1,
    0,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_2,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot167SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot168SpawnFiveGatedEffectNodes,
    (SnowboardCallback)Mini_SnowboardSlot169SpawnSixGatedEffectNodes,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_3,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_4,
    (SnowboardCallback)Mini_SnowboardSlot174SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_3,
    (SnowboardCallback)Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_4,
    0,
    (SnowboardCallback)Mini_SnowboardSlot178SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot179QueueGatedAndKind2Effect,
    (SnowboardCallback)Mini_SnowboardSlot180QueueGatedAndKind2Effect,
    (SnowboardCallback)Mini_SnowboardSlot181QueueGatedAndKind2Effect,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot184SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot194QueueGatedEffect,
    (SnowboardCallback)Mini_SnowboardSlot195QueueTwoGatedEffects,
    (SnowboardCallback)Mini_SnowboardSlot196QueueTwoGatedEffects,
    (SnowboardCallback)Mini_SnowboardSlot197QueueTwoGatedEffects,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot200SpawnEffectAboveSetRiderAnim,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot204SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot205SpawnEffect,
    (SnowboardCallback)Mini_SnowboardSlot206SpawnEffectIfRecordFlag,
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
    (SnowboardCallback)Mini_SnowboardSlot219SpawnEffectsAndLatchTrigger2,
    0,
    (SnowboardCallback)Mini_SnowboardSlot221SpawnPlainThenAboveEffect,
    (SnowboardCallback)Mini_SnowboardSlot222SpawnPlainEffect,
    0,
    (SnowboardCallback)Mini_SnowboardSlot224SpawnPlainEffect,
    (SnowboardCallback)Mini_SnowboardSlot225SpawnPlainEffect,
    (SnowboardCallback)Mini_SnowboardSlot226SpawnPlainThenAboveEffect,
    (SnowboardCallback)Mini_SnowboardSlot227SpawnPlainEffect,
    (SnowboardCallback)Mini_SnowboardSlot228SpawnPlainEffect,
    (SnowboardCallback)Mini_SnowboardSlot229SpawnTwoPlainEffects,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot233SpawnEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot237SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot238SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot239SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot240SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot241SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot242SpawnEffectAtAndAboveThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot243SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot244SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot245SpawnEffectAtThenCheckRider,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot248SpawnEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot252SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot253SpawnTwoEffectsAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot254SpawnEffectAboveThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot255SpawnTwoEffectsAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot256SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot257SpawnTwoEffectsAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot258SpawnTwoEffectsAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot259SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot260SpawnTwoEffectsAtAndAboveThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot261SpawnEffectAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot262SpawnEffectAtThenCheckRider,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot265SpawnEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot269SpawnThreeEffectsAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot270CheckRider,
    (SnowboardCallback)Mini_SnowboardSlot271SpawnEffectAboveThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot272SpawnThreeEffectsAtThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot273CheckRider,
    (SnowboardCallback)Mini_SnowboardSlot274SpawnEffectAboveThenCheckRider,
    (SnowboardCallback)Mini_SnowboardSlot275SpawnThreeEffectsAtThenCheckRider,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot281SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot286SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot295SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot301SpawnAboveAndTwoTargetEffects,
    (SnowboardCallback)Mini_SnowboardSlot302SpawnTwoOffsetEffects,
    (SnowboardCallback)Mini_SnowboardSlot303SpawnTwoOffsetEffects,
    (SnowboardCallback)Mini_SnowboardSlot304SpawnTwoOffsetEffects,
    (SnowboardCallback)Mini_SnowboardSlot305SpawnTwoOffsetEffects,
    (SnowboardCallback)Mini_SnowboardSlot306SpawnEffectWithOffsets,
    (SnowboardCallback)Mini_SnowboardSlot307SpawnTwoEffectsWithOffsets,
    0,
    (SnowboardCallback)Mini_SnowboardSlot309SpawnFourEffects,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot314SpawnEffectIdPlusB2,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot317QueueGatedEffect,
    (SnowboardCallback)Mini_SnowboardSlot318SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot319QueueGatedEffect,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot323SpawnEffectAboveThenGated,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot326QueueGatedEffect,
    0,
    (SnowboardCallback)Mini_SnowboardSlot328SpawnEffectAboveThenTwoGated,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot332SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot333SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot334SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot335SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot336SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot337SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot338SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot339SpawnTwoEffectsIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot340SpawnTwoIdPlusAEThenAbove,
    (SnowboardCallback)Mini_SnowboardSlot341SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot342SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot343SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot344SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot345SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot346SpawnTwoEffectsIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot347SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot348SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot349SpawnEffectIdPlusAE,
    (SnowboardCallback)Mini_SnowboardSlot350SpawnEffectAboveAndIdPlusB2,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot355SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot356QueueGatedEffect,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot360SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot361QueueGatedEffect,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot364SpawnEffectIdPlusB2,
    0,
    (SnowboardCallback)Mini_SnowboardSlot366QueueGatedEffect,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot371SpawnEffectAbove,
    0,
    (SnowboardCallback)Mini_SnowboardSlot373QueueGatedEffect,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot378SpawnEffectIdPlusB2,
    0,
    (SnowboardCallback)Mini_SnowboardSlot380QueueGatedEffectE6,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot384SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot385QueueGatedEffectE6,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot390SpawnEffectAbove,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot393SpawnEffectAbove,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot396SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot402SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot407QueueGatedEffectE6,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot410QueueGatedEffectB2,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot416SpawnTwoEffectsIdPlusB2,
    0,
    (SnowboardCallback)Mini_SnowboardSlot418SpawnEffectAbove,
    0,
    (SnowboardCallback)Mini_SnowboardSlot420SpawnEffect,
    (SnowboardCallback)Mini_SnowboardSlot421SpawnEffectGated,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot425SpawnEffectsAndLatchTrigger3,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot431SpawnTwoEffectsAbove,
    (SnowboardCallback)Mini_SnowboardSlot432SpawnTwoEffectsAbove,
    (SnowboardCallback)Mini_SnowboardSlot433SpawnTwoEffectsAbove,
    (SnowboardCallback)Mini_SnowboardSlot434SpawnTwoEffectsAbove,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot437SpawnEffectAbove,
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
    (SnowboardCallback)Mini_SnowboardSlot454SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot459SpawnEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot463SpawnEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot467SpawnEffectAbove,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot473QueueGatedEffectB2,
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
    (SnowboardCallback)Mini_SnowboardSlot488SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot493SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot494SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot495SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot496SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot497SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot498SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot499SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot500SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot501SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot502SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot503SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot504SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot505SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot506SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot507SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot508SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot509SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot510SpawnEffectRaisedForFall,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot513SpawnEffectRaisedForFallThenAbove,
    (SnowboardCallback)Mini_SnowboardSlot514SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot515SpawnEffectRaisedForFall,
    (SnowboardCallback)Mini_SnowboardSlot516SpawnEffectRaisedForFall,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot519SpawnEffectRaisedForFallThenAbove,
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
    (SnowboardCallback)Mini_SnowboardSlot532SpawnEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot537SpawnEffectTileF2OrF3,
    0,
    (SnowboardCallback)Mini_SnowboardSlot539NoOp,
    (SnowboardCallback)Mini_SnowboardSlot540SpawnTileF2OrF3ThenEffectAbove,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot543SpawnEffectTileF2OrF3,
    (SnowboardCallback)Mini_SnowboardSlot544SpawnEffectTileF2OrF3,
    (SnowboardCallback)Mini_SnowboardSlot545SpawnEffectTileF2OrF3,
    (SnowboardCallback)Mini_SnowboardSlot546SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot547NoOp,
    (SnowboardCallback)Mini_SnowboardSlot548NoOp,
    (SnowboardCallback)Mini_SnowboardSlot549SpawnEffectTileF2OrF3,
    (SnowboardCallback)Mini_SnowboardSlot550NoOp,
    (SnowboardCallback)Mini_SnowboardSlot551NoOp,
    (SnowboardCallback)Mini_SnowboardSlot552SpawnEffectTileF2OrF3,
    (SnowboardCallback)Mini_SnowboardSlot553SpawnTileF2OrF3ThenEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot554NoOp,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot557NoOp,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot560SpawnEffectTileF2OrF3,
    (SnowboardCallback)Mini_SnowboardSlot561SpawnTileEffectThenEffectAbove,
    0,
    (SnowboardCallback)Mini_SnowboardSlot563SpawnTwoTileEffects,
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
    (SnowboardCallback)Mini_SnowboardSlot574QueueGatedEffect,
    (SnowboardCallback)Mini_SnowboardSlot575SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot576QueueGatedEffect,
    (SnowboardCallback)Mini_SnowboardSlot577QueueGatedEffect,
    (SnowboardCallback)Mini_SnowboardSlot578SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot579QueueGatedEffect,
    (SnowboardCallback)Mini_SnowboardSlot580QueueGatedEffect,
    (SnowboardCallback)Mini_SnowboardSlot581SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot582QueueGatedEffect,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot589SpawnEffectAbove,
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
    (SnowboardCallback)Mini_SnowboardSlot603SpawnLinkedPairThenEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot607SpawnLinkedEffectPair,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot611SpawnTwoLinkedPairsThenEffectAbove,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot616SpawnLinkedEffectPair,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot620SpawnTwoLinkedPairsThenEffectAbove,
    0,
    0,
    0,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot627SpawnLinkedPairThenEffectAbove,
    0,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot631SpawnLinkedEffectPair,
    0,
    0,
    (SnowboardCallback)Mini_SnowboardSlot634SpawnEffectAbove,
    (SnowboardCallback)Mini_SnowboardSlot635SpawnEffectNode,
};

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot053 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x007388a0
void __cdecl Mini_SnowboardSlot053SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot053);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot056 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x007388bd
void __cdecl Mini_SnowboardSlot056SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot056);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot059 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x007388da
void __cdecl Mini_SnowboardSlot059SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot059);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot066 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x007388f7
void __cdecl Mini_SnowboardSlot066SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot066);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot070 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x00738914
void __cdecl Mini_SnowboardSlot070SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot070);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot076 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x00738931
void __cdecl Mini_SnowboardSlot076SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot076);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot082 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x0073894e
void __cdecl Mini_SnowboardSlot082SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot082);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot090 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x0073896b
void __cdecl Mini_SnowboardSlot090SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot090);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot099 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x00738988
void __cdecl Mini_SnowboardSlot099SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot099);
    }
}

/*
 * If spawnFlag is non-zero, spawns the effect described by the 16-byte record at g_SnoboAboveObjectDesc_Slot106 above
 * object objectIndex via Mini_SnowboardSpawnEffectAboveObject; otherwise does nothing. Mechanically
 * identical to its sixteen siblings from 0x007387f2 to 0x007389a5 apart from the descriptor
 * address.
 */
// FUNCTION: FF7 0x007389a5
void __cdecl Mini_SnowboardSlot106SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot106);
    }
}

/*
 * Snowboard minigame effect trigger: when spawnFlag is nonzero it calls Mini_SnowboardQueueHopWaitEffect with objectIndex
 * truncated to short and the effect descriptor at g_SnoboHopWaitDesc_Slot108. Mechanical -- the descriptor
 * address is the only thing that distinguishes it from its neighbours.
 */
// FUNCTION: FF7 0x007389c2
void __cdecl Mini_SnowboardSlot108QueueGatedEffect(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objectIndex, &g_SnoboHopWaitDesc_Slot108);
    }
}

/*
 * Returns when g_SnoboStatePtr->activeFlags is nonzero. Otherwise sets g_SnoboHopWaitEffectDef from
 * the descriptor's param plus 0xE6, fills its effect position, copies g_SnoboHopWaitEffectDef[9] + 2 to
 * g_SnoboHopWaitEffectDef[11] + 2 and allocates one effect node.
 */
// FUNCTION: FF7 0x007389df
void __cdecl Mini_SnowboardQueueHopWaitEffect(short objectIndex, void *descriptor)
{
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    g_SnoboHopWaitEffectDef.model = ((SnowboardCompactEffectDesc *)descriptor)->param + 0xE6;
    Mini_SnowboardFillEffectPosFromObject((short)objectIndex, &g_SnoboHopWaitEffectDef, (struct SnowboardEffectPosOffset *)descriptor);
    g_SnoboHopWaitEffectDef.heightLimit = g_SnoboHopWaitEffectDef.height;
    Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboHopWaitEffectDef, 1);
}

/*
 * Snowboard minigame effect trigger: when spawnFlag is nonzero it calls Mini_SnowboardQueueHitLaunchWithSignalEffect with objectIndex
 * truncated to short and the effect descriptor at g_SnoboHitLaunchDesc_Slot112. Mechanical -- the descriptor
 * address is the only thing that distinguishes it from its neighbours.
 */
// FUNCTION: FF7 0x00738a3c
void __cdecl Mini_SnowboardSlot112QueueGatedEffect(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboHitLaunchDesc_Slot112);
    }
}

/*
 * Returns when g_SnoboStatePtr->activeFlags is nonzero. Otherwise fills the g_SnoboEffectDefSmall
 * effect position, copies g_SnoboEffectDefSmall[9] + 2 to g_SnoboEffectDefSmall[11] + 2,
 * sets g_SnoboEffectDefSmall from the descriptor's param plus 0xB2 and allocates one effect node.
 */
// FUNCTION: FF7 0x00738a59
void __cdecl Mini_SnowboardQueueHitLaunchWithSignalEffect(short objectIndex, void *descriptor)
{
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objectIndex, &g_SnoboEffectDefSmall, (struct SnowboardEffectPosOffset *)descriptor);
    g_SnoboEffectDefSmall.heightLimit = g_SnoboEffectDefSmall.height;
    g_SnoboEffectDefSmall.model = ((SnowboardCompactEffectDesc *)descriptor)->param + 0xB2;
    Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboEffectDefSmall, 1);
}

/*
 * Snowboard minigame effect trigger that fires three effects when spawnFlag is nonzero:
 * Mini_SnowboardSpawnEffectAboveObject with the descriptor at g_SnoboAboveObjectDesc_Slot113, then
 * Mini_SnowboardQueueHitLaunchWithSignalEffect twice with the descriptors at g_SnoboCompactEffect_Slot113 and
 * g_SnoboCompactEffect_Slot113[1], which are the two halves of one adjacent pair (the
 * descriptor stride here is 10 bytes). objectIndex is the object index, truncated to short at every
 * call.
 */
// FUNCTION: FF7 0x00738ab8
void __cdecl Mini_SnowboardSlot113SpawnEffectAndQueueTwoGated(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, &g_SnoboAboveObjectDesc_Slot113.pos);
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboCompactEffect_Slot113[0]);
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboCompactEffect_Slot113[1]);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when spawnFlag is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objectIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot119. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00738af9
void __cdecl Mini_SnowboardSlot119SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot119);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when spawnFlag is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objectIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot125. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00738b16
void __cdecl Mini_SnowboardSlot125SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot125);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when spawnFlag is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objectIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot137. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00738b33
void __cdecl Mini_SnowboardSlot137SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot137);
    }
}

/*
 * Snowboard minigame effect trigger: when spawnFlag is nonzero it spawns the effect described at
 * g_SnoboAboveObjectDesc_Slot140 above object objectIndex via Mini_SnowboardSpawnEffectAboveObject. Then, regardless of
 * spawnFlag, it compares objectIndex against g_SnoboStatePtr->section and
 * calls Mini_SnowboardSetRiderAnimByHeightDiff when they are equal, so the extra work only happens for one particular
 * object.
 */
// FUNCTION: FF7 0x00738b50
void __cdecl Mini_SnowboardSlot140SpawnEffectAboveSetRiderAnim(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, &g_SnoboAboveObjectDesc_Slot140.pos);
    }
    if (objectIndex == g_SnoboStatePtr->section) {
        Mini_SnowboardSetRiderAnimByHeightDiff();
    }
}

/*
 * Within course rows 24 through 47 and outside animation codes ten/twelve, computes rider height
 * difference shifted by twelve. For differences 0x100 through 0x1BF selects animation twelve, or
 * 0x1C0 through 0x1C7 selects ten, playing the same sound. Other ranges leave state unchanged.
 */
// FUNCTION: FF7 0x00738b84
void __cdecl Mini_SnowboardSetRiderAnimByHeightDiff(void)
{
    int val;
    if (g_SnoboStatePtr->triangle >= 24 && g_SnoboStatePtr->triangle <= 47) {
        switch ((char)g_SnoboStatePtr->riderState) {
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0A:
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0C:
            return;
        }
        val = ((int)g_SnoboStatePtr->terrainHeight - (int)g_SnoboStatePtr->relPosY) >> 12;
        if (val < 256) {
            return;
        }
        if (val < 448) {
            g_SnoboStatePtr->riderState = 12;
            Mini_SnowboardPlaySfxOnChannel(2, 0xf0, 0);
        } else if (val < 456) {
            g_SnoboStatePtr->riderState = 10;
            Mini_SnowboardPlaySfxOnChannel(2, 0xf0, 0);
        }
    }
}

/*
 * Snowboard minigame effect trigger: when spawnFlag is nonzero it calls Mini_SnowboardQueueLateralDriftEffect with objectIndex
 * truncated to short (the object index) and the descriptor at g_SnoboLateralDriftDesc_Slot142Group, then Mini_SnowboardSpawnEffectNodeGatedPlusAE with
 * the descriptor at g_SnoboGatedModelAEDesc_Slot142Group. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x00738c66
void __cdecl Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_1(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardQueueLateralDriftEffect((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLateralDriftDesc_Slot142Group);
        Mini_SnowboardSpawnEffectNodeGatedPlusAE((short)objectIndex, &g_SnoboGatedModelAEDesc_Slot142Group);
    }
}

// Queues a mode-2 flagged effect for objectIndex, and does nothing while
// g_SnoboStatePtr->activeFlags is nonzero. It fills the template g_SnoboLateralDriftEffectDef from the
// object and spec, then allocates the node, sets fade to 0xFFFF (-1), and sets enabled when
// spec->flags lacks bit 0x800.
// FUNCTION: FF7 0x00738c95
void __cdecl Mini_SnowboardQueueLateralDriftEffect(short objectIndex, SnoboLinkedEffectSpec *spec)
{
    SnowboardEffect *node;

    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objectIndex, &g_SnoboLateralDriftEffectDef, (struct SnowboardEffectPosOffset *)spec);
    node = (SnowboardEffect *)(Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboLateralDriftEffectDef, 2));
    node->fade = 0xFFFF;
    node->enabled = (spec->flags & 0x800) == 0;
}

/*
 * When g_SnoboStatePtr->activeFlags is zero, sets g_SnoboHitTiltNegEffectDef from the descriptor's param plus 0xAE, fills its position from the
 * object and allocates one effect node.
 */
// FUNCTION: FF7 0x00738d03
void __cdecl Mini_SnowboardSpawnEffectNodeGatedPlusAE(short objectIndex, void *descriptor)
{
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    g_SnoboHitTiltNegEffectDef.model = ((SnowboardCompactEffectDesc *)descriptor)->param + 0xAE;
    Mini_SnowboardFillEffectPosFromObject((short)objectIndex, &g_SnoboHitTiltNegEffectDef, (struct SnowboardEffectPosOffset *)descriptor);
    Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboHitTiltNegEffectDef, 1);
}

/*
 * Snowboard minigame effect trigger: when spawnFlag is nonzero it calls Mini_SnowboardQueueLateralDriftEffect with objectIndex
 * truncated to short (the object index) and the descriptor at g_SnoboLateralDriftDesc_Slot143Group, then Mini_SnowboardSpawnEffectNodeGatedPlusAE with
 * the descriptor at g_SnoboGatedModelAEDesc_Slot143Group. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x00738d56
void __cdecl Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_2(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardQueueLateralDriftEffect((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLateralDriftDesc_Slot143Group);
        Mini_SnowboardSpawnEffectNodeGatedPlusAE((short)objectIndex, &g_SnoboGatedModelAEDesc_Slot143Group);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when spawnFlag is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objectIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot148. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00738d85
void __cdecl Mini_SnowboardSlot148SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot148);
    }
}

/*
 * Snowboard minigame effect trigger: when spawnFlag is nonzero it calls Mini_SnowboardQueueSplitBallisticEffect with objectIndex
 * truncated to short (the object index) and the descriptor at g_SnoboSplitBallisticDesc_Slot149, then Mini_SnowboardQueueApproachFollowEffect with
 * the descriptor at g_SnoboApproachFollowDesc_Slot149. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x00738da2
void __cdecl Mini_SnowboardSlot149QueueGatedAndOffsetEffect(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objectIndex, &g_SnoboSplitBallisticDesc_Slot149);
        Mini_SnowboardQueueApproachFollowEffect((short)objectIndex, (SnowboardKind2EffectDesc *)&g_SnoboApproachFollowDesc_Slot149);
    }
}

// Queues a mode-2 offset effect for objectIndex, and does nothing while
// g_SnoboStatePtr->activeFlags is nonzero. It fills the template g_SnoboApproachFollowEffectDef from the
// object and descriptor, then allocates the node. The node's age takes descriptor->swayPhase, its
// offsetPos is the node's forward plus fixedOffset shifted left 12 bits, and fade is set to
// 0xFFFF.
// FUNCTION: FF7 0x00738dd1
void __cdecl Mini_SnowboardQueueApproachFollowEffect(short objectIndex, SnowboardKind2EffectDesc *descriptor)
{
    SnowboardEffect *node;

    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objectIndex, &g_SnoboApproachFollowEffectDef, (struct SnowboardEffectPosOffset *)descriptor);
    node = (SnowboardEffect *)(Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboApproachFollowEffectDef, 2));
    node->age = descriptor->swayPhase;
    node->offsetPos = node->forward + (descriptor->fixedOffset << 12);
    node->fade = 0xFFFF;
}

/*
 * Queues one snowboard effect for object objIndex from the descriptor at descriptor. It copies the
 * short at descriptor offset 8 (the effect id) into g_SnoboSplitBallisticEffectDef[14], then bails out
 * if g_SnoboStatePtr->activeFlags is nonzero; otherwise it fills
 * the shared scratch node g_SnoboSplitBallisticEffectDef with Mini_SnowboardFillEffectPosFromObject and
 * hands it to Mini_SnowboardAllocEffectNode with kind 1. It is the plain sibling of
 * Mini_SnowboardSpawnEffectAboveObject, which does the same thing but also biases the node's height
 * field by -0x80000 and mirrors the id into g_SnoboAboveObjectEffectDef[14] and
 * g_SnoboAboveObjectEffectDef[3].
 */
// FUNCTION: FF7 0x00738e4d
void __cdecl Mini_SnowboardQueueSplitBallisticEffect(short objIndex, void *descriptor)
{
    g_SnoboSplitBallisticEffectDef.mode = ((SnowboardCompactEffectDesc *)descriptor)->param;
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objIndex, &g_SnoboSplitBallisticEffectDef, (struct SnowboardEffectPosOffset *)descriptor);
    Mini_SnowboardAllocEffectNode((short)objIndex, &g_SnoboSplitBallisticEffectDef, 1);
}

/*
 * Snowboard minigame effect trigger: when gate is nonzero it calls Mini_SnowboardQueueSplitBallisticEffect with objIndex
 * truncated to short (the object index) and the descriptor at g_SnoboSplitBallisticDesc_Slot150, then Mini_SnowboardQueueApproachFollowEffect with
 * the descriptor at g_SnoboApproachFollowDesc_Slot150. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x00738e9c
void __cdecl Mini_SnowboardSlot150QueueGatedAndOffsetEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboSplitBallisticDesc_Slot150);
        Mini_SnowboardQueueApproachFollowEffect((short)objIndex, (SnowboardKind2EffectDesc *)&g_SnoboApproachFollowDesc_Slot150);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when gate is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot154. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00738ecb
void __cdecl Mini_SnowboardSlot154SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot154);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when gate is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot159. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00738ee8
void __cdecl Mini_SnowboardSlot159SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot159);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when gate is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot167. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00738f05
void __cdecl Mini_SnowboardSlot167SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot167);
    }
}

/*
 * For nonzero gate, calls Mini_SnowboardSpawnEffectNodeGatedPlusAE five times with objIndex narrowed to short and entries spaced
 * five elements apart in g_SnoboGatedModelAEDesc_Slot168.
 */
// FUNCTION: FF7 0x00738f22
void __cdecl Mini_SnowboardSlot168SpawnFiveGatedEffectNodes(int objIndex, int gate)
{
    int i;

    if (gate != 0) {
        for (i = 0; i < 5; i++) {
            Mini_SnowboardSpawnEffectNodeGatedPlusAE((short)objIndex, &g_SnoboGatedModelAEDesc_Slot168[i * 5]);
        }
    }
}

/*
 * For nonzero gate, calls Mini_SnowboardSpawnEffectNodeGatedPlusAE six times with objIndex narrowed to short and entries spaced
 * five elements apart in g_SnoboGatedModelAEDesc_Slot169.
 */
// FUNCTION: FF7 0x00738f65
void __cdecl Mini_SnowboardSlot169SpawnSixGatedEffectNodes(int objIndex, int gate)
{
    int i;

    if (gate != 0) {
        for (i = 0; i < 6; i++) {
            Mini_SnowboardSpawnEffectNodeGatedPlusAE((short)objIndex, &g_SnoboGatedModelAEDesc_Slot169[i * 5]);
        }
    }
}

/*
 * Snowboard minigame effect trigger: when gate is nonzero it calls Mini_SnowboardQueueLateralDriftSpawnChildEffect with objIndex
 * truncated to short (the object index) and the descriptor at g_SnoboLateralDriftChildDesc_Slot172Group, then Mini_SnowboardSpawnEffectNodeGatedPlusAE with
 * the descriptor at g_SnoboGatedModelAEDesc_Slot172Group. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x00738fa8
void __cdecl Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_3(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueLateralDriftSpawnChildEffect((short)objIndex, (SnoboLinkedEffectSpec *)&g_SnoboLateralDriftChildDesc_Slot172Group);
        Mini_SnowboardSpawnEffectNodeGatedPlusAE((short)objIndex, &g_SnoboGatedModelAEDesc_Slot172Group);
    }
}

// Queues a mode-2 flagged effect like Mini_SnowboardQueueLateralDriftEffect, but with the
// template g_SnoboLateralDriftSpawnChildEffectDef. It does nothing while g_SnoboStatePtr->activeFlags is
// nonzero, and otherwise fills the template from the object and spec, allocates the node, sets
// fade to 0xFFFF (-1), and sets enabled when spec->flags lacks bit 0x800.
// FUNCTION: FF7 0x00738fd7
void __cdecl Mini_SnowboardQueueLateralDriftSpawnChildEffect(short objIndex, SnoboLinkedEffectSpec *spec)
{
    SnowboardEffect *nodePtr;

    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objIndex, &g_SnoboLateralDriftSpawnChildEffectDef, (struct SnowboardEffectPosOffset *)spec);
    nodePtr = (SnowboardEffect *)(Mini_SnowboardAllocEffectNode((short)objIndex, &g_SnoboLateralDriftSpawnChildEffectDef, 2));
    nodePtr->fade = 0xFFFF;
    nodePtr->enabled = (spec->flags & 0x800) == 0;
}

/*
 * Snowboard minigame effect trigger: when gate is nonzero it calls Mini_SnowboardQueueLateralDriftSpawnChildEffect with objIndex
 * truncated to short (the object index) and the descriptor at g_SnoboLateralDriftChildDesc_Slot173Group, then Mini_SnowboardSpawnEffectNodeGatedPlusAE with
 * the descriptor at g_SnoboGatedModelAEDesc_Slot173Group. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x00739045
void __cdecl Mini_SnowboardMultiSlotQueueFlaggedAndIdPlusAEEffect_4(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueLateralDriftSpawnChildEffect((short)objIndex, (SnoboLinkedEffectSpec *)&g_SnoboLateralDriftChildDesc_Slot173Group);
        Mini_SnowboardSpawnEffectNodeGatedPlusAE((short)objIndex, &g_SnoboGatedModelAEDesc_Slot173Group);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when gate is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot174. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00739074
void __cdecl Mini_SnowboardSlot174SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot174);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when gate is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot178. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x00739091
void __cdecl Mini_SnowboardSlot178SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot178);
    }
}

/*
 * Snowboard minigame effect trigger: when gate is nonzero it calls Mini_SnowboardQueueSplitBallisticEffect with objIndex
 * truncated to short (the object index) and the descriptor at g_SnoboSplitBallisticDesc_Slot179, then Mini_SnowboardSpawnEffectKind2AtObject with
 * the descriptor at g_SnoboKind2Desc_Slot179. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x007390ae
void __cdecl Mini_SnowboardSlot179QueueGatedAndKind2Effect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboSplitBallisticDesc_Slot179);
        Mini_SnowboardSpawnEffectKind2AtObject((short)objIndex, (SnowboardKind2EffectDesc *)&g_SnoboKind2Desc_Slot179);
    }
}

// Spawns a kind-2 effect at objIndex from a SnowboardKind2EffectDesc, and does nothing while
// g_SnoboStatePtr->activeFlags is nonzero. It fills the template g_SnoboApproachFollowSpawnChildEffectDef from the
// object and descriptor, then allocates a mode-2 node. The node's age takes descriptor->swayPhase, its
// offsetPos is the node's forward plus fixedOffset shifted left 12 bits, and fade is set to
// 0xFFFF.
// FUNCTION: FF7 0x007390dd
void __cdecl Mini_SnowboardSpawnEffectKind2AtObject(short objIndex, SnowboardKind2EffectDesc *descriptor)
{
    SnowboardEffect *nodePtr;

    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objIndex, &g_SnoboApproachFollowSpawnChildEffectDef, (struct SnowboardEffectPosOffset *)descriptor);
    nodePtr = (SnowboardEffect *)(Mini_SnowboardAllocEffectNode((short)objIndex, &g_SnoboApproachFollowSpawnChildEffectDef, 2));
    nodePtr->age = descriptor->swayPhase;
    nodePtr->offsetPos = nodePtr->forward + (descriptor->fixedOffset << 12);
    nodePtr->fade = 0xFFFF;
}

/*
 * Snowboard minigame effect trigger: when gate is nonzero it calls Mini_SnowboardQueueSplitBallisticEffect with objIndex
 * truncated to short (the object index) and the descriptor at g_SnoboSplitBallisticDesc_Slot180, then Mini_SnowboardSpawnEffectKind2AtObject with
 * the descriptor at g_SnoboKind2Desc_Slot180. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x00739159
void __cdecl Mini_SnowboardSlot180QueueGatedAndKind2Effect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboSplitBallisticDesc_Slot180);
        Mini_SnowboardSpawnEffectKind2AtObject((short)objIndex, (SnowboardKind2EffectDesc *)&g_SnoboKind2Desc_Slot180);
    }
}

/*
 * Snowboard minigame effect trigger: when gate is nonzero it calls Mini_SnowboardQueueSplitBallisticEffect with objIndex
 * truncated to short (the object index) and the descriptor at g_SnoboSplitBallisticDesc_Slot181, then Mini_SnowboardSpawnEffectKind2AtObject with
 * the descriptor at g_SnoboKind2Desc_Slot181. Mechanical -- the two callees and the two descriptor addresses
 * are all that distinguish it from its neighbours.
 */
// FUNCTION: FF7 0x00739188
void __cdecl Mini_SnowboardSlot181QueueGatedAndKind2Effect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboSplitBallisticDesc_Slot181);
        Mini_SnowboardSpawnEffectKind2AtObject((short)objIndex, (SnowboardKind2EffectDesc *)&g_SnoboKind2Desc_Slot181);
    }
}

/*
 * One of the snowboard minigame's one-line effect triggers: when gate is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objIndex truncated to short (the object index) and the
 * effect descriptor at g_SnoboAboveObjectDesc_Slot184. Purely mechanical -- the only thing that distinguishes it
 * from its neighbours is the descriptor address.
 */
// FUNCTION: FF7 0x007391b7
void __cdecl Mini_SnowboardSlot184SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot184);
    }
}

/*
 * Snowboard minigame effect trigger: when gate is nonzero it calls Mini_SnowboardQueueSplitBallisticEffect with objIndex
 * truncated to short and the effect descriptor at g_SnoboSplitBallisticDesc_Slot194, which queues that effect at the
 * object's own position. Mechanical -- the descriptor address is the only thing that distinguishes
 * it from its neighbours.
 */
// FUNCTION: FF7 0x007391d4
void __cdecl Mini_SnowboardSlot194QueueGatedEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboSplitBallisticDesc_Slot194);
    }
}

/*
 * When gate is nonzero, invokes Mini_SnowboardQueueSplitBallisticEffect twice with objIndex
 * narrowed to short and descriptors at g_SnoboCompactEffect_Slot195 and
 * g_SnoboCompactEffect_Slot195[1], ten bytes apart. The position helper reads
 * descriptor coordinate offsets at +2, +4 and +6; descriptor +0 supplies a separate short and +8
 * supplies the effect ID.
 */
// FUNCTION: FF7 0x007391f1
void __cdecl Mini_SnowboardSlot195QueueTwoGatedEffects(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboCompactEffect_Slot195[0]);
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboCompactEffect_Slot195[1]);
    }
}

/*
 * Same shape as Mini_SnowboardSlot195QueueTwoGatedEffects: if gate is nonzero, spawns two effects
 * for object objIndex through Mini_SnowboardQueueSplitBallisticEffect from the descriptors at
 * g_SnoboCompactEffect_Slot196 and g_SnoboCompactEffect_Slot196[1], again ten bytes apart.
 * Mechanical dispatch shim with no other side effects.
 */
// FUNCTION: FF7 0x00739220
void __cdecl Mini_SnowboardSlot196QueueTwoGatedEffects(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboCompactEffect_Slot196[0]);
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboCompactEffect_Slot196[1]);
    }
}

/*
 * If gate is nonzero, calls Mini_SnowboardQueueSplitBallisticEffect for object objIndex with the
 * descriptors at g_SnoboCompactEffect_Slot197 and g_SnoboCompactEffect_Slot197[1] (ten bytes
 * apart). Another entry in the same family of two-effect dispatch shims; it touches nothing else.
 */
// FUNCTION: FF7 0x0073924f
void __cdecl Mini_SnowboardSlot197QueueTwoGatedEffects(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboCompactEffect_Slot197[0]);
        Mini_SnowboardQueueSplitBallisticEffect((short)objIndex, &g_SnoboCompactEffect_Slot197[1]);
    }
}

/*
 * When gate is nonzero, spawns the effect described at g_SnoboAboveObjectDesc_Slot200 over object objIndex via
 * Mini_SnowboardSpawnEffectAboveObject. Then, unconditionally, if objIndex equals the short at
 * g_SnoboStatePtr+0xEC (the object the minigame state treats as the player), it calls Mini_SnowboardSetRiderAnimByHeightDiff.
 */
// FUNCTION: FF7 0x0073927e
void __cdecl Mini_SnowboardSlot200SpawnEffectAboveSetRiderAnim(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboAboveObjectDesc_Slot200.pos);
    }
    if (objIndex == g_SnoboStatePtr->section) {
        Mini_SnowboardSetRiderAnimByHeightDiff();
    }
}

/*
 * One-line dispatch shim: if gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for
 * object objIndex with the static effect descriptor at g_SnoboAboveObjectDesc_Slot204. Nothing else.
 */
// FUNCTION: FF7 0x007392b2
void __cdecl Mini_SnowboardSlot204SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot204);
    }
}

/*
 * If gate is nonzero, calls Mini_SnowboardSpawnEffectNodeAtObjectOffsets for object objIndex with the effect descriptor at
 * g_SnoboNodeOffsetsDesc_Slot205. Mechanical dispatch shim.
 */
// FUNCTION: FF7 0x007392cf
void __cdecl Mini_SnowboardSlot205SpawnEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeAtObjectOffsets((short)objIndex, &g_SnoboNodeOffsetsDesc_Slot205);
    }
}

/*
 * Like Mini_SnowboardSlot205SpawnEffect but with a second gate: it fires only when gate is nonzero and the
 * recordFlag of the race effect state at g_SnoboStatePtr->segmentTable is nonzero, in which case
 * it calls Mini_SnowboardSpawnEffectNodeAtObjectOffsets for object objIndex with the descriptor at g_SnoboNodeEffectPair_Slot206.
 */
// FUNCTION: FF7 0x007392ec
void __cdecl Mini_SnowboardSlot206SpawnEffectIfRecordFlag(int objIndex, int gate)
{
    if (gate != 0 && ((SnowboardRaceEffectState *)(g_SnoboStatePtr->segmentTable))->recordFlag != 0) {
        Mini_SnowboardSpawnEffectNodeAtObjectOffsets((short)objIndex, &g_SnoboNodeEffectPair_Slot206[0]);
    }
}

// Fires the slot-219 effects for objIndex when gate is nonzero: an effect node at the object
// offsets of slot219OffsetsEffectDesc, an effect above the object from g_SnoboAboveObjectDesc_Slot219A, and a
// plain effect at the object from g_SnoboPlainDesc_Slot219. An effect above the object from
// g_SnoboAboveObjectDesc_Slot219B also fires when activeFlags is nonzero and slotGate219 is nonzero and below
// 0x1200000 unsigned. Regardless of gate, if triggerLatched is zero and section (the player object)
// equals objIndex, it sets triggerStage to 2 and triggerLatched to 1. This is the latch that
// Mini_SnowboardSlot012SpawnEffectsAndLatchTrigger sets with stage 1.
// FUNCTION: FF7 0x0073931c
void __cdecl Mini_SnowboardSlot219SpawnEffectsAndLatchTrigger2(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeAtObjectOffsets((short)objIndex, &g_SnoboNodeEffectPair_Slot206[1]);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboAboveObjectDesc_Slot219A.pos);
        Mini_SnowboardSpawnPlainEffectAtObject((short)objIndex, &g_SnoboPlainDesc_Slot219);
        if (g_SnoboStatePtr->activeFlags != 0 && g_SnoboStatePtr->slotGate219 != 0 && (unsigned int)g_SnoboStatePtr->slotGate219 < (unsigned int)0x1200000) {
            Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboAboveObjectDesc_Slot219B.pos);
        }
    }
    if (g_SnoboStatePtr->triggerLatched == 0 && g_SnoboStatePtr->section == objIndex) {
        g_SnoboStatePtr->triggerStage = 2;
        g_SnoboStatePtr->triggerLatched = 1;
    }
}

/*
 * Shared helper for the handlers below it. It does nothing at all when the short at
 * g_SnoboStatePtr->activeFlags is nonzero; otherwise it fills the static scratch effect record g_SnoboPlainEffectDef
 * from object objIndex and descriptor descriptor via Mini_SnowboardFillEffectPosFromObject and hands that
 * record to Mini_SnowboardAllocEffectNode with kind 1. Unlike Mini_SnowboardSpawnEffectAboveObject
 * it applies no vertical offset and does not copy the descriptor's effect id.
 */
// FUNCTION: FF7 0x007393ce
void __cdecl Mini_SnowboardSpawnPlainEffectAtObject(short objIndex, void *descriptor)
{
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objIndex, &g_SnoboPlainEffectDef, (struct SnowboardEffectPosOffset *)descriptor);
    Mini_SnowboardAllocEffectNode((short)objIndex, &g_SnoboPlainEffectDef, 1);
}

/*
 * If gate is nonzero, spawns two effects on object objIndex: Mini_SnowboardSpawnPlainEffectAtObject with the descriptor at
 * g_SnoboPlainDesc_Slot221 and Mini_SnowboardSpawnEffectAboveObject with the one at g_SnoboAboveObjectDesc_Slot221.
 */
// FUNCTION: FF7 0x0073940f
void __cdecl Mini_SnowboardSlot221SpawnPlainThenAboveEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnPlainEffectAtObject((short)objIndex, &g_SnoboPlainDesc_Slot221);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot221);
    }
}

/*
 * Dispatch shim: if gate is nonzero, calls Mini_SnowboardSpawnPlainEffectAtObject for object objIndex with the effect descriptor
 * at g_SnoboPlainDesc_Slot222. Nothing else.
 */
// FUNCTION: FF7 0x0073943e
void __cdecl Mini_SnowboardSlot222SpawnPlainEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnPlainEffectAtObject((short)objIndex, &g_SnoboPlainDesc_Slot222);
    }
}

/*
 * Dispatch shim: if gate is nonzero, calls Mini_SnowboardSpawnPlainEffectAtObject for object objIndex with the effect descriptor
 * at g_SnoboPlainDesc_Slot224, the next eight-byte record after the one Mini_SnowboardSlot222SpawnPlainEffect uses.
 */
// FUNCTION: FF7 0x0073945b
void __cdecl Mini_SnowboardSlot224SpawnPlainEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnPlainEffectAtObject((short)objIndex, &g_SnoboPlainDesc_Slot224);
    }
}

/*
 * Dispatch shim: if gate is nonzero, calls Mini_SnowboardSpawnPlainEffectAtObject for object objIndex with the effect descriptor
 * at g_SnoboPlainDesc_Slot225. Identical in shape to its two neighbours, differing only in the descriptor
 * address.
 */
// FUNCTION: FF7 0x00739478
void __cdecl Mini_SnowboardSlot225SpawnPlainEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnPlainEffectAtObject((short)objIndex, &g_SnoboPlainDesc_Slot225);
    }
}

/*
 * If gate is nonzero, calls Mini_SnowboardSpawnPlainEffectAtObject for object objIndex with the descriptor at g_SnoboPlainDesc_Slot226 and
 * then Mini_SnowboardSpawnEffectAboveObject with the one at g_SnoboAboveObjectDesc_Slot226.
 */
// FUNCTION: FF7 0x00739495
void __cdecl Mini_SnowboardSlot226SpawnPlainThenAboveEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnPlainEffectAtObject((short)objIndex, &g_SnoboPlainDesc_Slot226);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot226);
    }
}

/*
 * Dispatch shim: if spawnFlag is nonzero, calls Mini_SnowboardSpawnPlainEffectAtObject for object objectIndex with the effect descriptor
 * at g_SnoboPlainDesc_Slot227.
 */
// FUNCTION: FF7 0x007394c4
void __cdecl Mini_SnowboardSlot227SpawnPlainEffect(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnPlainEffectAtObject((short)objectIndex, &g_SnoboPlainDesc_Slot227);
    }
}

/*
 * Dispatch shim: if spawnFlag is nonzero, calls Mini_SnowboardSpawnPlainEffectAtObject for object objectIndex with the effect descriptor
 * at g_SnoboPlainDesc_Slot228.
 */
// FUNCTION: FF7 0x007394e1
void __cdecl Mini_SnowboardSlot228SpawnPlainEffect(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnPlainEffectAtObject((short)objectIndex, &g_SnoboPlainDesc_Slot228);
    }
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnPlainEffectAtObject twice for object objectIndex, with the two adjacent eight-byte
 * descriptors at g_SnoboPlainDesc_Slot229A and g_SnoboPlainDesc_Slot229B.
 */
// FUNCTION: FF7 0x007394fe
void __cdecl Mini_SnowboardSlot229SpawnTwoPlainEffects(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnPlainEffectAtObject((short)objectIndex, &g_SnoboPlainDesc_Slot229A);
        Mini_SnowboardSpawnPlainEffectAtObject((short)objectIndex, &g_SnoboPlainDesc_Slot229B);
    }
}

/*
 * Dispatch shim: if spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for object objectIndex
 * with the effect descriptor at g_SnoboAboveObjectDesc_Slot233.
 */
// FUNCTION: FF7 0x0073952d
void __cdecl Mini_SnowboardSlot233SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot233);
    }
}

/*
 * When spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAtObject with objectIndex narrowed to short and
 * descriptor g_SnoboAtObjectDesc_Slot237. Then calls Mini_SnowboardCheckRiderDropEnterState0B(objectIndex) regardless of spawnFlag.
 */
// FUNCTION: FF7 0x0073954a
void __cdecl Mini_SnowboardSlot237SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot237);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * Returns when g_SnoboStatePtr->activeFlags is nonzero; otherwise fills the g_SnoboStepLaunchEffectDef effect position from
 * offset and allocates one effect node for objectIndex, leaving the allocation result unused.
 */
// FUNCTION: FF7 0x00739573
void __cdecl Mini_SnowboardSpawnEffectAtObject(short objectIndex, void *offset)
{
    int effectNode;

    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objectIndex, &g_SnoboStepLaunchEffectDef, (struct SnowboardEffectPosOffset *)offset);
    effectNode = Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboStepLaunchEffectDef, 1);
}

// Checks for the state-0B entry. It returns unless objectIndex equals section, and returns when
// riderState is 11. Otherwise it calls Mini_SnowboardEnterRiderState0BPlaySfx when terrainHeight
// minus relPosY, taken as ints and shifted right by 12 bits, exceeds 0x240.
// FUNCTION: FF7 0x007395ba
void __cdecl Mini_SnowboardCheckRiderDropEnterState0B(int objectIndex)
{
    if (g_SnoboStatePtr->section != objectIndex) {
        return;
    }
    if ((unsigned char)g_SnoboStatePtr->riderState == 11) {
        return;
    }
    if (((int)g_SnoboStatePtr->terrainHeight - (int)g_SnoboStatePtr->relPosY) >> 12 > 576) {
        Mini_SnowboardEnterRiderState0BPlaySfx();
    }
}

/*
 * Sets the rider state to 0xB, replaces the fadeLevel float with the absolute integer-converted value
 * clamped to at least 0x40000 and plays channel-2 sound 0x10C.
 */
// FUNCTION: FF7 0x0073961f
void __cdecl Mini_SnowboardEnterRiderState0BPlaySfx(void)
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
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAtObject for object objectIndex with the descriptor
 * at g_SnoboAtObjectDesc_Slot238, then unconditionally calls Mini_SnowboardCheckRiderDropEnterState0B(objectIndex). Same shape as Mini_SnowboardSlot237SpawnEffectAtThenCheckRider with
 * a different descriptor.
 */
// FUNCTION: FF7 0x00739686
void __cdecl Mini_SnowboardSlot238SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot238);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAtObject for object objectIndex with the descriptor
 * at g_SnoboAtObjectDesc_Slot239, then unconditionally calls Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x007396af
void __cdecl Mini_SnowboardSlot239SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot239);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAtObject for object objectIndex with the descriptor
 * at g_SnoboAtObjectDesc_Slot240, then unconditionally calls Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x007396d8
void __cdecl Mini_SnowboardSlot240SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot240);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAtObject for object objectIndex with the descriptor
 * at g_SnoboAtObjectDesc_Slot241, then unconditionally calls Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739701
void __cdecl Mini_SnowboardSlot241SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot241);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns two effects on object objectIndex -- Mini_SnowboardSpawnEffectAtObject with
 * the descriptor at g_SnoboAtObjectDesc_Slot242 and Mini_SnowboardSpawnEffectAboveObject with the earlier one at
 * g_SnoboAboveObjectDesc_Slot242, in that order, so the descriptors are used out of address order. Then it calls
 * Mini_SnowboardCheckRiderDropEnterState0B(objectIndex) unconditionally.
 */
// FUNCTION: FF7 0x0073972a
void __cdecl Mini_SnowboardSlot242SpawnEffectAtAndAboveThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot242);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot242);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAtObject for object objectIndex with the descriptor
 * at g_SnoboAtObjectDesc_Slot243, then unconditionally calls Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739765
void __cdecl Mini_SnowboardSlot243SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot243);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * When spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAtObject with objectIndex narrowed to short and
 * descriptor g_SnoboAtObjectDesc_Slot244. Then calls Mini_SnowboardCheckRiderDropEnterState0B(objectIndex) regardless of spawnFlag.
 */
// FUNCTION: FF7 0x0073978e
void __cdecl Mini_SnowboardSlot244SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot244);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * Same shape as its neighbours: if spawnFlag is nonzero, spawns the effect described by the record at
 * g_SnoboAtObjectDesc_Slot245 on object objectIndex via Mini_SnowboardSpawnEffectAtObject, then always calls the
 * Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x007397b7
void __cdecl Mini_SnowboardSlot245SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot245);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * When spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject with objectIndex narrowed to short and
 * descriptor g_SnoboAboveObjectDesc_Slot248. Performs no additional source-visible operation.
 */
// FUNCTION: FF7 0x007397e0
void __cdecl Mini_SnowboardSlot248SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot248);
    }
}

/*
 * If spawnFlag is nonzero, spawns the effect record at g_SnoboAtObjectDesc_Slot252 on object objectIndex through
 * Mini_SnowboardSpawnEffectAtObject, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x007397fd
void __cdecl Mini_SnowboardSlot252SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot252);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns two effects on object objectIndex through Mini_SnowboardSpawnEffectAtObject,
 * from the records at g_SnoboAtObjectDesc_Slot253A and g_SnoboAtObjectDesc_Slot253B in that order, then always calls the
 * Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739826
void __cdecl Mini_SnowboardSlot253SpawnTwoEffectsAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot253A);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot253B);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for object objectIndex with the record
 * at g_SnoboAboveObjectDesc_Slot254, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739861
void __cdecl Mini_SnowboardSlot254SpawnEffectAboveThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot254);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns two effects on object objectIndex through Mini_SnowboardSpawnEffectAtObject,
 * from the records at g_SnoboAtObjectDesc_Slot255A and g_SnoboAtObjectDesc_Slot255B, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x0073988a
void __cdecl Mini_SnowboardSlot255SpawnTwoEffectsAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot255A);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot255B);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns the effect record at g_SnoboAtObjectDesc_Slot256 on object objectIndex through
 * Mini_SnowboardSpawnEffectAtObject, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x007398c5
void __cdecl Mini_SnowboardSlot256SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot256);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns two effects on object objectIndex through Mini_SnowboardSpawnEffectAtObject,
 * from the records at g_SnoboAtObjectDesc_Slot257A and g_SnoboAtObjectDesc_Slot257B, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x007398ee
void __cdecl Mini_SnowboardSlot257SpawnTwoEffectsAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot257A);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot257B);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns two effects on object objectIndex through Mini_SnowboardSpawnEffectAtObject,
 * from the records at g_SnoboAtObjectDesc_Slot258A and g_SnoboAtObjectDesc_Slot258B, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739929
void __cdecl Mini_SnowboardSlot258SpawnTwoEffectsAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot258A);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot258B);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns the effect record at g_SnoboAtObjectDesc_Slot259 on object objectIndex through
 * Mini_SnowboardSpawnEffectAtObject, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739964
void __cdecl Mini_SnowboardSlot259SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot259);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns three effects on object objectIndex: the records at g_SnoboAtObjectDesc_Slot260A and
 * g_SnoboAtObjectDesc_Slot260B through Mini_SnowboardSpawnEffectAtObject, then the record at g_SnoboAboveObjectDesc_Slot260 through
 * Mini_SnowboardSpawnEffectAboveObject, which raises the spawn point. Always calls the
 * Mini_SnowboardCheckRiderDropEnterState0B(objectIndex) afterwards.
 */
// FUNCTION: FF7 0x0073998d
void __cdecl Mini_SnowboardSlot260SpawnTwoEffectsAtAndAboveThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot260A);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot260B);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot260);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns the effect record at g_SnoboAtObjectDesc_Slot261 on object objectIndex through
 * Mini_SnowboardSpawnEffectAtObject, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x007399da
void __cdecl Mini_SnowboardSlot261SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot261);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns the effect record at g_SnoboAtObjectDesc_Slot262 on object objectIndex through
 * Mini_SnowboardSpawnEffectAtObject, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739a03
void __cdecl Mini_SnowboardSlot262SpawnEffectAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot262);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for object objectIndex with the record
 * at g_SnoboAboveObjectDesc_Slot265. That is the whole body; unlike most of this family it does not call
 * Mini_SnowboardCheckRiderDropEnterState0B afterwards.
 */
// FUNCTION: FF7 0x00739a2c
void __cdecl Mini_SnowboardSlot265SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot265);
    }
}

/*
 * If spawnFlag is nonzero, spawns three effects on object objectIndex through
 * Mini_SnowboardSpawnEffectAtObject, from the records at g_SnoboAtObjectDesc_Slot269A, g_SnoboAtObjectDesc_Slot269B and
 * g_SnoboAtObjectDesc_Slot269C in that order, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739a49
void __cdecl Mini_SnowboardSlot269SpawnThreeEffectsAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot269A);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot269B);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot269C);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * Takes only an object index and forwards it to the Mini_SnowboardCheckRiderDropEnterState0B. The degenerate member of this
 * handler family: no effect record, no second argument, nothing else in the body.
 */
// FUNCTION: FF7 0x00739a96
void __cdecl Mini_SnowboardSlot270CheckRider(int objectIndex)
{
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for object objectIndex with the record
 * at g_SnoboAboveObjectDesc_Slot271, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739aa7
void __cdecl Mini_SnowboardSlot271SpawnEffectAboveThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot271);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns three effects on object objectIndex through
 * Mini_SnowboardSpawnEffectAtObject, from the records at g_SnoboAtObjectDesc_Slot272A, g_SnoboAtObjectDesc_Slot272B and
 * g_SnoboAtObjectDesc_Slot272C, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739ad0
void __cdecl Mini_SnowboardSlot272SpawnThreeEffectsAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot272A);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot272B);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot272C);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * Takes only an object index and forwards it to the Mini_SnowboardCheckRiderDropEnterState0B. Byte-for-byte the same shape as
 * Mini_SnowboardSlot270CheckRider at 0x00739a96; the two are separate table entries, not one function.
 */
// FUNCTION: FF7 0x00739b1d
void __cdecl Mini_SnowboardSlot273CheckRider(int objectIndex)
{
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for object objectIndex with the record
 * at g_SnoboAboveObjectDesc_Slot274, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739b2e
void __cdecl Mini_SnowboardSlot274SpawnEffectAboveThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot274);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, spawns three effects on object objectIndex through
 * Mini_SnowboardSpawnEffectAtObject, from the records at g_SnoboAtObjectDesc_Slot275A, g_SnoboAtObjectDesc_Slot275B and
 * g_SnoboAtObjectDesc_Slot275C, then always calls the Mini_SnowboardCheckRiderDropEnterState0B(objectIndex).
 */
// FUNCTION: FF7 0x00739b57
void __cdecl Mini_SnowboardSlot275SpawnThreeEffectsAtThenCheckRider(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot275A);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot275B);
        Mini_SnowboardSpawnEffectAtObject((short)objectIndex, &g_SnoboAtObjectDesc_Slot275C);
    }
    Mini_SnowboardCheckRiderDropEnterState0B(objectIndex);
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for object objectIndex with the record
 * at g_SnoboAboveObjectDesc_Slot281. No trailing Mini_SnowboardCheckRiderDropEnterState0B call.
 */
// FUNCTION: FF7 0x00739ba4
void __cdecl Mini_SnowboardSlot281SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot281);
    }
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for object objectIndex with the record
 * at g_SnoboAboveObjectDesc_Slot286. No trailing Mini_SnowboardCheckRiderDropEnterState0B call.
 */
// FUNCTION: FF7 0x00739bc1
void __cdecl Mini_SnowboardSlot286SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot286);
    }
}

/*
 * If spawnFlag is nonzero, calls Mini_SnowboardSpawnEffectAboveObject for object objectIndex with the record
 * at g_SnoboAboveObjectDesc_Slot295. Last of this run of handlers and, like its two immediate neighbours, it has no
 * trailing Mini_SnowboardCheckRiderDropEnterState0B call.
 */
// FUNCTION: FF7 0x00739bde
void __cdecl Mini_SnowboardSlot295SpawnEffectAbove(int objectIndex, int spawnFlag)
{
    if (spawnFlag != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot295);
    }
}

/*
 * One of this TU's two-argument effect-spawn wrappers: does nothing unless enable is nonzero,
 * otherwise passes the object index objIndex (narrowed to short) to
 * Mini_SnowboardSpawnEffectAboveObject with the descriptor at g_SnoboAboveObjectDesc_Slot301, then to
 * Mini_SnowboardSpawnCircularOrbitEffect twice with the descriptors at g_SnoboCompactEffect_Slot302
 * and g_SnoboCompactEffect_Slot302[1]. The descriptors are the fixed ten-byte records
 * those helpers read (id at +0, x/y/z offsets at +2/+4/+6, kind at +8), which is why the last two
 * addresses are 0xA apart.
 */
// FUNCTION: FF7 0x00739bfb
void __cdecl Mini_SnowboardSlot301SpawnAboveAndTwoTargetEffects(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboAboveObjectDesc_Slot301.pos);
        Mini_SnowboardSpawnCircularOrbitEffect((short)objIndex, &g_SnoboCompactEffect_Slot302[0]);
        Mini_SnowboardSpawnCircularOrbitEffect((short)objIndex, &g_SnoboCompactEffect_Slot302[1]);
    }
}

// Spawns a mode-1 effect with a target at objIndex, and does nothing while
// g_SnoboStatePtr->activeFlags is nonzero. It fills the template g_SnoboCircularOrbitEffectDef from the
// object and desc, then allocates the node. The node's counter takes the descriptor's param, orbitX
// takes the node's sidePos, orbitZ takes its forward, and orbitY takes its height minus 0x60000.
// FUNCTION: FF7 0x00739c3c
void __cdecl Mini_SnowboardSpawnCircularOrbitEffect(short objIndex, void *desc)
{
    SnowboardEffect *node;

    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objIndex, &g_SnoboCircularOrbitEffectDef, (struct SnowboardEffectPosOffset *)desc);
    node = (SnowboardEffect *)(Mini_SnowboardAllocEffectNode((short)objIndex, &g_SnoboCircularOrbitEffectDef, 1));
    node->phaseOffset = ((SnowboardCompactEffectDesc *)desc)->param;
    node->orbitX = node->sidePos;
    node->orbitZ = node->forward;
    node->orbitY = node->height - 0x60000;
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnVerticalBobEffect
 * twice for object (short)objIndex, with the descriptor records at g_SnoboOffsetEffect_Slot303 and
 * g_SnoboOffsetEffect_Slot303[1]; otherwise it does nothing.
 */
// FUNCTION: FF7 0x00739cbd
void __cdecl Mini_SnowboardSlot302SpawnTwoOffsetEffects(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot303[0]);
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot303[1]);
    }
}

// Spawns a mode-1 effect at objIndex from a SnowboardEffectOffsetDesc, and does nothing while
// g_SnoboStatePtr->activeFlags is nonzero. It fills the template g_SnoboVerticalBobEffectDef from the
// object and desc, then allocates the node. The node's counter takes desc->phaseOffset, its spinRate takes
// desc->spinRate, and its offsetPos is the node's height minus heightUnits shifted left 12 bits.
// FUNCTION: FF7 0x00739cec
void __cdecl Mini_SnowboardSpawnVerticalBobEffect(short objIndex, SnowboardEffectOffsetDesc *desc)
{
    SnowboardOffsetsEffectNode *node;

    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objIndex, &g_SnoboVerticalBobEffectDef, (struct SnowboardEffectPosOffset *)desc);
    node = (SnowboardOffsetsEffectNode *)(Mini_SnowboardAllocEffectNode((short)objIndex, &g_SnoboVerticalBobEffectDef, 1));
    node->phaseOffset = desc->phaseOffset;
    node->spinRate = desc->spinRate;
    node->offsetPos = ((SnowboardEffect *)node)->height - (desc->heightUnits << 12);
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnVerticalBobEffect
 * twice for object (short)objIndex, with the descriptors at g_SnoboOffsetEffect_Slot304 and
 * g_SnoboOffsetEffect_Slot304[1]. Same shape as its neighbours, differing only in which two records it
 * names.
 */
// FUNCTION: FF7 0x00739d69
void __cdecl Mini_SnowboardSlot303SpawnTwoOffsetEffects(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot304[0]);
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot304[1]);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnVerticalBobEffect twice for object (short)objIndex,
 * with the descriptors at g_SnoboOffsetEffect_Slot305 and g_SnoboOffsetEffect_Slot305[1].
 */
// FUNCTION: FF7 0x00739d98
void __cdecl Mini_SnowboardSlot304SpawnTwoOffsetEffects(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot305[0]);
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot305[1]);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnVerticalBobEffect twice for object (short)objIndex,
 * with the descriptors at g_SnoboOffsetEffect_Slot306 and g_SnoboOffsetEffect_Slot306[1].
 */
// FUNCTION: FF7 0x00739dc7
void __cdecl Mini_SnowboardSlot305SpawnTwoOffsetEffects(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot306[0]);
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot306[1]);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnVerticalBobEffect once for object (short)objIndex with
 * the descriptor at g_SnoboOffsetsDesc; otherwise it does nothing.
 */
// FUNCTION: FF7 0x00739df6
void __cdecl Mini_SnowboardSlot306SpawnEffectWithOffsets(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, (SnowboardEffectOffsetDesc *)&g_SnoboOffsetsDesc);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnVerticalBobEffect twice for object (short)objIndex,
 * with the descriptors at g_SnoboOffsetEffect_Slot308 and g_SnoboOffsetEffect_Slot308[1].
 */
// FUNCTION: FF7 0x00739e13
void __cdecl Mini_SnowboardSlot307SpawnTwoEffectsWithOffsets(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot308[0]);
        Mini_SnowboardSpawnVerticalBobEffect((short)objIndex, &g_SnoboOffsetEffect_Slot308[1]);
    }
}

/*
 * When enable is nonzero, narrows objIndex to short and calls Mini_SnowboardQueueHopWaitEffect with g_SnoboHopWaitDesc_Slot309,
 * Mini_SnowboardSpawnEffectAboveObject with g_SnoboAboveObjectDesc_Slot309, then Mini_SnowboardQueueHitLaunchWithSignalEffect with g_SnoboCompactEffect_Slot310 and
 * g_SnoboCompactEffect_Slot310[1], in that order.
 */
// FUNCTION: FF7 0x00739e42
void __cdecl Mini_SnowboardSlot309SpawnFourEffects(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objIndex, &g_SnoboHopWaitDesc_Slot309);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot309);
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboCompactEffect_Slot310[0]);
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboCompactEffect_Slot310[1]);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectIdPlusB2 with objIndex narrowed to short and descriptor
 * g_SnoboModelB2Desc_Slot314.
 */
// FUNCTION: FF7 0x00739e95
void __cdecl Mini_SnowboardSlot314SpawnEffectIdPlusB2(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusB2((short)objIndex, &g_SnoboModelB2Desc_Slot314);
    }
}

/*
 * When g_SnoboStatePtr->activeFlags is zero, copies the descriptor's param to g_SnoboEffectDefLarge[14] and that word plus
 * 0xB2 to g_SnoboEffectDefLarge, fills the latter template position and allocates one effect
 * node.
 */
// FUNCTION: FF7 0x00739eb2
void __cdecl Mini_SnowboardSpawnEffectIdPlusB2(short objIndex, void *desc)
{
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    g_SnoboEffectDefLarge.state = ((SnowboardCompactEffectDesc *)desc)->param;
    g_SnoboEffectDefLarge.model = ((SnowboardCompactEffectDesc *)desc)->param + 0xB2;
    Mini_SnowboardFillEffectPosFromObject((short)objIndex, &g_SnoboEffectDefLarge, (struct SnowboardEffectPosOffset *)desc);
    Mini_SnowboardAllocEffectNode((short)objIndex, &g_SnoboEffectDefLarge, 1);
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardQueueHitLaunchWithSignalEffect once for object (short)objIndex with
 * the descriptor at g_SnoboHitLaunchDesc_Slot317.
 */
// FUNCTION: FF7 0x00739f13
void __cdecl Mini_SnowboardSlot317QueueGatedEffect(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboHitLaunchDesc_Slot317);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectAboveObject once for
 * object (short)objIndex with the descriptor at g_SnoboAboveObjectDesc_Slot318, which fills the shared staging record
 * g_SnoboAboveObjectEffectDef and allocates an effect node.
 */
// FUNCTION: FF7 0x00739f30
void __cdecl Mini_SnowboardSlot318SpawnEffectAbove(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot318);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardQueueHitLaunchWithSignalEffect once for object (short)objIndex with
 * the descriptor at g_SnoboHitLaunchDesc_Slot319.
 */
// FUNCTION: FF7 0x00739f4d
void __cdecl Mini_SnowboardSlot319QueueGatedEffect(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboHitLaunchDesc_Slot319);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectAboveObject with the
 * descriptor at g_SnoboAboveObjectDesc_Slot323 and then Mini_SnowboardQueueHitLaunchWithSignalEffect with the one at g_SnoboHitLaunchDesc_Slot323, both for object
 * (short)objIndex.
 */
// FUNCTION: FF7 0x00739f6a
void __cdecl Mini_SnowboardSlot323SpawnEffectAboveThenGated(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot323);
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboHitLaunchDesc_Slot323);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardQueueHitLaunchWithSignalEffect once for object (short)objIndex with
 * the descriptor at g_SnoboHitLaunchDesc_Slot326.
 */
// FUNCTION: FF7 0x00739f99
void __cdecl Mini_SnowboardSlot326QueueGatedEffect(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboHitLaunchDesc_Slot326);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectAboveObject with the
 * descriptor at g_SnoboAboveObjectDesc_Slot328, then Mini_SnowboardQueueHitLaunchWithSignalEffect with
 * g_SnoboCompactEffect_Slot329 and g_SnoboCompactEffect_Slot329[1], all for object
 * (short)objIndex. The last two records are 0xA apart, the descriptor stride.
 */
// FUNCTION: FF7 0x00739fb6
void __cdecl Mini_SnowboardSlot328SpawnEffectAboveThenTwoGated(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot328);
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboCompactEffect_Slot329[0]);
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboCompactEffect_Slot329[1]);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectIdPlusAE with objIndex narrowed to short and descriptor
 * g_SnoboModelAEDesc_Slot332.
 */
// FUNCTION: FF7 0x00739ff7
void __cdecl Mini_SnowboardSlot332SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot332);
    }
}

/*
 * When g_SnoboStatePtr->activeFlags is zero, sets g_SnoboHitTiltPosEffectDef from the descriptor's param plus 0xAE, fills its position and
 * allocates one effect node.
 */
// FUNCTION: FF7 0x0073a014
void __cdecl Mini_SnowboardSpawnEffectIdPlusAE(short objIndex, void *desc)
{
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    g_SnoboHitTiltPosEffectDef.model = ((SnowboardCompactEffectDesc *)desc)->param + 0xAE;
    Mini_SnowboardFillEffectPosFromObject((short)objIndex, &g_SnoboHitTiltPosEffectDef, (struct SnowboardEffectPosOffset *)desc);
    Mini_SnowboardAllocEffectNode((short)objIndex, &g_SnoboHitTiltPosEffectDef, 1);
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE once for object (short)objIndex with
 * the descriptor at g_SnoboModelAEDesc_Slot333; otherwise it does nothing.
 */
// FUNCTION: FF7 0x0073a067
void __cdecl Mini_SnowboardSlot333SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot333);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE once for object (short)objIndex with
 * the descriptor at g_SnoboModelAEDesc_Slot334.
 */
// FUNCTION: FF7 0x0073a084
void __cdecl Mini_SnowboardSlot334SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot334);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE once for object (short)objIndex with
 * the descriptor at g_SnoboModelAEDesc_Slot335.
 */
// FUNCTION: FF7 0x0073a0a1
void __cdecl Mini_SnowboardSlot335SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot335);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE once for object (short)objIndex with
 * the descriptor at g_SnoboModelAEDesc_Slot336.
 */
// FUNCTION: FF7 0x0073a0be
void __cdecl Mini_SnowboardSlot336SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot336);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE once for object (short)objIndex with
 * the descriptor at g_SnoboModelAEDesc_Slot337.
 */
// FUNCTION: FF7 0x0073a0db
void __cdecl Mini_SnowboardSlot337SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot337);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE once for object (short)objIndex with
 * the descriptor at g_SnoboModelAEDesc_Slot338. One of eight consecutive functions in this file that differ only
 * in the descriptor address, which advances by 0x10 each time.
 */
// FUNCTION: FF7 0x0073a0f8
void __cdecl Mini_SnowboardSlot338SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot338);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE twice for object (short)objIndex,
 * with the descriptors at g_SnoboCompactEffect_Slot339 and g_SnoboCompactEffect_Slot339[1].
 */
// FUNCTION: FF7 0x0073a115
void __cdecl Mini_SnowboardSlot339SpawnTwoEffectsIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboCompactEffect_Slot339[0]);
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboCompactEffect_Slot339[1]);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE with the
 * descriptors at g_SnoboCompactEffect_Slot340 and g_SnoboCompactEffect_Slot340[1], then
 * Mini_SnowboardSpawnEffectAboveObject with the one at g_SnoboAboveObjectDesc_Slot340, all for object
 * (short)objIndex.
 */
// FUNCTION: FF7 0x0073a144
void __cdecl Mini_SnowboardSlot340SpawnTwoIdPlusAEThenAbove(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboCompactEffect_Slot340[0]);
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboCompactEffect_Slot340[1]);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboAboveObjectDesc_Slot340.pos);
    }
}

/*
 * Effect-spawn wrapper: if enable is nonzero it calls Mini_SnowboardSpawnEffectIdPlusAE once for object (short)objIndex with
 * the descriptor at g_SnoboModelAEDesc_Slot341; the last of this run of wrappers.
 */
// FUNCTION: FF7 0x0073a185
void __cdecl Mini_SnowboardSlot341SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot341);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless enable is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusAE with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelAEDesc_Slot342.
 */
// FUNCTION: FF7 0x0073a1a2
void __cdecl Mini_SnowboardSlot342SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot342);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless enable is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusAE with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelAEDesc_Slot343.
 */
// FUNCTION: FF7 0x0073a1bf
void __cdecl Mini_SnowboardSlot343SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot343);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless enable is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusAE with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelAEDesc_Slot344.
 */
// FUNCTION: FF7 0x0073a1dc
void __cdecl Mini_SnowboardSlot344SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot344);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless enable is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusAE with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelAEDesc_Slot345.
 */
// FUNCTION: FF7 0x0073a1f9
void __cdecl Mini_SnowboardSlot345SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot345);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectIdPlusAE twice with objIndex narrowed to
 * short, first using g_SnoboCompactEffect_Slot346 and then g_SnoboCompactEffect_Slot346[1]. The
 * descriptor addresses are ten bytes apart; address spacing alone does not establish an array
 * record extent.
 */
// FUNCTION: FF7 0x0073a216
void __cdecl Mini_SnowboardSlot346SpawnTwoEffectsIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboCompactEffect_Slot346[0]);
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboCompactEffect_Slot346[1]);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless enable is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusAE with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelAEDesc_Slot347.
 */
// FUNCTION: FF7 0x0073a245
void __cdecl Mini_SnowboardSlot347SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot347);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless enable is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusAE with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelAEDesc_Slot348.
 */
// FUNCTION: FF7 0x0073a262
void __cdecl Mini_SnowboardSlot348SpawnEffectIdPlusAE(int objIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot348);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusAE with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelAEDesc_Slot349.
 */
// FUNCTION: FF7 0x0073a27f
void __cdecl Mini_SnowboardSlot349SpawnEffectIdPlusAE(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectIdPlusAE((short)objIndex, &g_SnoboModelAEDesc_Slot349);
    }
}

/*
 * Snowboard effect hook that fires two different helpers: when gate is nonzero it calls
 * Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short and the descriptor
 * g_SnoboAboveObjectDesc_Slot350, then Mini_SnowboardSpawnEffectIdPlusB2 with the same object index and the descriptor g_SnoboModelB2Desc_Slot350.
 */
// FUNCTION: FF7 0x0073a29c
void __cdecl Mini_SnowboardSlot350SpawnEffectAboveAndIdPlusB2(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboAboveObjectDesc_Slot350.pos);
        Mini_SnowboardSpawnEffectIdPlusB2((short)objIndex, &g_SnoboModelB2Desc_Slot350);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short (the
 * object index the helpers use to index the state's object array) and
 * the address of the fixed effect descriptor g_SnoboAboveObjectDesc_Slot355.
 */
// FUNCTION: FF7 0x0073a2cb
void __cdecl Mini_SnowboardSlot355SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot355);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardQueueHopWaitEffect with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboHopWaitDesc_Slot356.
 */
// FUNCTION: FF7 0x0073a2e8
void __cdecl Mini_SnowboardSlot356QueueGatedEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objIndex, &g_SnoboHopWaitDesc_Slot356);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short (the
 * object index the helpers use to index the state's object array) and
 * the address of the fixed effect descriptor g_SnoboAboveObjectDesc_Slot360.
 */
// FUNCTION: FF7 0x0073a305
void __cdecl Mini_SnowboardSlot360SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot360);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardQueueHopWaitEffect with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboHopWaitDesc_Slot361.
 */
// FUNCTION: FF7 0x0073a322
void __cdecl Mini_SnowboardSlot361QueueGatedEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objIndex, &g_SnoboHopWaitDesc_Slot361);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusB2 with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelB2Desc_Slot364.
 */
// FUNCTION: FF7 0x0073a33f
void __cdecl Mini_SnowboardSlot364SpawnEffectIdPlusB2(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectIdPlusB2((short)objIndex, &g_SnoboModelB2Desc_Slot364);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardQueueHopWaitEffect with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboHopWaitDesc_Slot366.
 */
// FUNCTION: FF7 0x0073a35c
void __cdecl Mini_SnowboardSlot366QueueGatedEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objIndex, &g_SnoboHopWaitDesc_Slot366);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short (the
 * object index the helpers use to index the state's object array) and
 * the address of the fixed effect descriptor g_SnoboAboveObjectDesc_Slot371.
 */
// FUNCTION: FF7 0x0073a379
void __cdecl Mini_SnowboardSlot371SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot371);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardQueueHopWaitEffect with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboHopWaitDesc_Slot373.
 */
// FUNCTION: FF7 0x0073a396
void __cdecl Mini_SnowboardSlot373QueueGatedEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objIndex, &g_SnoboHopWaitDesc_Slot373);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectIdPlusB2 with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboModelB2Desc_Slot378.
 */
// FUNCTION: FF7 0x0073a3b3
void __cdecl Mini_SnowboardSlot378SpawnEffectIdPlusB2(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectIdPlusB2((short)objIndex, &g_SnoboModelB2Desc_Slot378);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardQueueHopWaitEffect with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboHopWaitDesc_Slot380.
 */
// FUNCTION: FF7 0x0073a3d0
void __cdecl Mini_SnowboardSlot380QueueGatedEffectE6(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objIndex, &g_SnoboHopWaitDesc_Slot380);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short (the
 * object index the helpers use to index the state's object array) and
 * the address of the fixed effect descriptor g_SnoboAboveObjectDesc_Slot384.
 */
// FUNCTION: FF7 0x0073a3ed
void __cdecl Mini_SnowboardSlot384SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot384);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardQueueHopWaitEffect with objIndex narrowed to short (the object index the helpers
 * use to index the state's object array) and the address of the fixed
 * effect descriptor g_SnoboHopWaitDesc_Slot385.
 */
// FUNCTION: FF7 0x0073a40a
void __cdecl Mini_SnowboardSlot385QueueGatedEffectE6(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objIndex, &g_SnoboHopWaitDesc_Slot385);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short (the
 * object index the helpers use to index the state's object array) and
 * the address of the fixed effect descriptor g_SnoboAboveObjectDesc_Slot390.
 */
// FUNCTION: FF7 0x0073a427
void __cdecl Mini_SnowboardSlot390SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot390);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short (the
 * object index the helpers use to index the state's object array) and
 * the address of the fixed effect descriptor g_SnoboAboveObjectDesc_Slot393.
 */
// FUNCTION: FF7 0x0073a444
void __cdecl Mini_SnowboardSlot393SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot393);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short (the
 * object index the helpers use to index the state's object array) and
 * the address of the fixed effect descriptor g_SnoboAboveObjectDesc_Slot396.
 */
// FUNCTION: FF7 0x0073a461
void __cdecl Mini_SnowboardSlot396SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot396);
    }
}

/*
 * One of the snowboard minigame's one-line effect hooks: it does nothing unless gate is nonzero,
 * in which case it calls Mini_SnowboardSpawnEffectAboveObject with objIndex narrowed to short (the
 * object index the helpers use to index the state's object array) and
 * the address of the fixed effect descriptor g_SnoboAboveObjectDesc_Slot402.
 */
// FUNCTION: FF7 0x0073a47e
void __cdecl Mini_SnowboardSlot402SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot402);
    }
}

/*
 * Gate-and-spawn thunk: does nothing unless gate is nonzero, then calls Mini_SnowboardQueueHopWaitEffect with the
 * object index objIndex narrowed to short and the address of the static effect record g_SnoboHopWaitDesc_Slot407.
 * Mechanical - one of a long run of near-identical callbacks in this file that differ only in
 * which spawner they call and which record they name.
 */
// FUNCTION: FF7 0x0073a49b
void __cdecl Mini_SnowboardSlot407QueueGatedEffectE6(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHopWaitEffect((short)objIndex, &g_SnoboHopWaitDesc_Slot407);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardQueueHitLaunchWithSignalEffect with (short)objIndex and the address of the effect record
 * g_SnoboHitLaunchDesc_Slot410. Otherwise does nothing. Mechanical, identical in shape to its neighbours.
 */
// FUNCTION: FF7 0x0073a4b8
void __cdecl Mini_SnowboardSlot410QueueGatedEffectB2(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboHitLaunchDesc_Slot410);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectIdPlusB2 twice with (short)objIndex, first
 * for the effect record at g_SnoboCompactEffect_Slot411 and then for the one at
 * g_SnoboCompactEffect_Slot411[1]. The two records are 0xA bytes apart, which is the
 * stride of these effect descriptors: a short id at +0, x/y/z offsets at +2/+4/+6 and a type at +8,
 * as read by Mini_SnowboardFillEffectPosFromObject and Mini_SnowboardSpawnEffectAboveObject.
 */
// FUNCTION: FF7 0x0073a4d5
void __cdecl Mini_SnowboardSlot416SpawnTwoEffectsIdPlusB2(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectIdPlusB2((short)objIndex, &g_SnoboCompactEffect_Slot411[0]);
        Mini_SnowboardSpawnEffectIdPlusB2((short)objIndex, &g_SnoboCompactEffect_Slot411[1]);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject with (short)objIndex and the effect
 * record at g_SnoboAboveObjectDesc_Slot418. Mechanical single-record spawn callback.
 */
// FUNCTION: FF7 0x0073a504
void __cdecl Mini_SnowboardSlot418SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot418);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectNodeAtObjectOffsets with (short)objIndex and the effect record at g_SnoboNodeOffsetsDesc_Slot420.
 * Mechanical.
 */
// FUNCTION: FF7 0x0073a521
void __cdecl Mini_SnowboardSlot420SpawnEffect(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeAtObjectOffsets((short)objIndex, &g_SnoboNodeOffsetsDesc_Slot420);
    }
}

/*
 * Same shape as its neighbours but with an extra guard: it spawns via Mini_SnowboardSpawnEffectNodeAtObjectOffsets with
 * (short)objIndex and the record at g_SnoboNodeOffsetsDesc_Slot421 only when gate is nonzero and the effectGate of
 * the race effect state at g_SnoboStatePtr->segmentTable is nonzero.
 */
// FUNCTION: FF7 0x0073a53e
void __cdecl Mini_SnowboardSlot421SpawnEffectGated(int objIndex, int gate)
{
    if (gate != 0 && ((SnowboardRaceEffectState *)(g_SnoboStatePtr->segmentTable))->effectGate != 0) {
        Mini_SnowboardSpawnEffectNodeAtObjectOffsets((short)objIndex, &g_SnoboNodeOffsetsDesc_Slot421);
    }
}

// Fires the slot-425 effects for objIndex when gate is nonzero: an effect node at object offsets
// from the record g_SnoboNodeOffsetsDesc_Slot425, and, when activeFlags is nonzero and slotGate425 is
// nonzero and below 0x1350000 unsigned, an effect above the object from g_SnoboAboveObjectDesc_Slot425.
// Regardless of gate, if triggerLatched is zero and section equals objIndex, it sets triggerStage
// to 3 and triggerLatched to 1, the same one-shot latch that
// Mini_SnowboardSlot012SpawnEffectsAndLatchTrigger sets with stage 1.
// FUNCTION: FF7 0x0073a571
void __cdecl Mini_SnowboardSlot425SpawnEffectsAndLatchTrigger3(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeAtObjectOffsets((short)objIndex, &g_SnoboNodeOffsetsDesc_Slot425);
        if (g_SnoboStatePtr->activeFlags != 0 && g_SnoboStatePtr->slotGate425 != 0 && (unsigned int)g_SnoboStatePtr->slotGate425 < (unsigned int)0x1350000) {
            Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboAboveObjectDesc_Slot425.pos);
        }
    }
    if (g_SnoboStatePtr->triggerLatched == 0 && g_SnoboStatePtr->section == objIndex) {
        g_SnoboStatePtr->triggerStage = 3;
        g_SnoboStatePtr->triggerLatched = 1;
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject twice with (short)objIndex, for
 * the effect records at g_SnoboCompactEffect_Slot426 and g_SnoboCompactEffect_Slot426[1] - the usual
 * 0xA-byte record stride. Mechanical.
 */
// FUNCTION: FF7 0x0073a600
void __cdecl Mini_SnowboardSlot431SpawnTwoEffectsAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboCompactEffect_Slot426[0].pos);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboCompactEffect_Slot426[1].pos);
    }
}

/*
 * When gate is nonzero, spawns two effects through Mini_SnowboardSpawnEffectAboveObject with
 * (short)objIndex, from the adjacent records g_SnoboCompactEffect_Slot427 and
 * g_SnoboCompactEffect_Slot427[1]. Mechanical.
 */
// FUNCTION: FF7 0x0073a62f
void __cdecl Mini_SnowboardSlot432SpawnTwoEffectsAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboCompactEffect_Slot427[0].pos);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboCompactEffect_Slot427[1].pos);
    }
}

/*
 * When gate is nonzero, spawns two effects through Mini_SnowboardSpawnEffectAboveObject with
 * (short)objIndex, from the adjacent records g_SnoboCompactEffect_Slot428 and
 * g_SnoboCompactEffect_Slot428[1]. Mechanical.
 */
// FUNCTION: FF7 0x0073a65e
void __cdecl Mini_SnowboardSlot433SpawnTwoEffectsAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboCompactEffect_Slot428[0].pos);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboCompactEffect_Slot428[1].pos);
    }
}

/*
 * When gate is nonzero, spawns two effects through Mini_SnowboardSpawnEffectAboveObject with
 * (short)objIndex, from the adjacent records g_SnoboCompactEffect_Slot429 and
 * g_SnoboCompactEffect_Slot429[1]. Mechanical.
 */
// FUNCTION: FF7 0x0073a68d
void __cdecl Mini_SnowboardSlot434SpawnTwoEffectsAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboCompactEffect_Slot429[0].pos);
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboCompactEffect_Slot429[1].pos);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject with (short)objIndex and the effect
 * record at g_SnoboAboveObjectDesc_Slot437. Mechanical single-record spawn callback.
 */
// FUNCTION: FF7 0x0073a6bc
void __cdecl Mini_SnowboardSlot437SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, &g_SnoboAboveObjectDesc_Slot437.pos);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject with (short)objIndex and the effect
 * record at g_SnoboAboveObjectDesc_Slot454. Mechanical; differs from its neighbours only in the record address.
 */
// FUNCTION: FF7 0x0073a6d9
void __cdecl Mini_SnowboardSlot454SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot454);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject with (short)objIndex and the effect
 * record at g_SnoboAboveObjectDesc_Slot459. Mechanical.
 */
// FUNCTION: FF7 0x0073a6f6
void __cdecl Mini_SnowboardSlot459SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot459);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject with (short)objIndex and the effect
 * record at g_SnoboAboveObjectDesc_Slot463. Mechanical.
 */
// FUNCTION: FF7 0x0073a713
void __cdecl Mini_SnowboardSlot463SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot463);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject with (short)objIndex and the effect
 * record at g_SnoboAboveObjectDesc_Slot467. Mechanical.
 */
// FUNCTION: FF7 0x0073a730
void __cdecl Mini_SnowboardSlot467SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot467);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardQueueHitLaunchWithSignalEffect with (short)objIndex and the effect record at g_SnoboHitLaunchDesc_Slot473.
 * Mechanical.
 */
// FUNCTION: FF7 0x0073a74d
void __cdecl Mini_SnowboardSlot473QueueGatedEffectB2(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objIndex, &g_SnoboHitLaunchDesc_Slot473);
    }
}

/*
 * When gate is nonzero, calls Mini_SnowboardSpawnEffectAboveObject with (short)objIndex and the effect
 * record at g_SnoboAboveObjectDesc_Slot488. Mechanical.
 */
// FUNCTION: FF7 0x0073a76a
void __cdecl Mini_SnowboardSlot488SpawnEffectAbove(int objIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objIndex, g_SnoboAboveObjectDesc_Slot488);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectRaisedForFall with (short)objectIndex and the effect record at g_SnoboRaisedDesc_Slot493.
 * First of the seven callbacks in this file that route through that spawner.
 */
// FUNCTION: FF7 0x0073a787
void __cdecl Mini_SnowboardSlot493SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot493);
    }
}

/*
 * When g_SnoboStatePtr->activeFlags is zero, prepares g_SnoboRaisedEffectDef with a callback selected by the descriptor's selector, an ID
 * selected by its selector and its filled position offset by its fallHeight shifted twelve, then allocates one
 * effect node; also retains the selector in g_SnoboRaisedEffectDef.variant.
 */
// FUNCTION: FF7 0x0073a7a4
void __cdecl Mini_SnowboardSpawnEffectRaisedForFall(short objectIndex, SnowboardRaisedEffect *descriptor)
{
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    g_SnoboRaisedEffectDef.update = descriptor->noFallWhenNear != 0 ? 0 : (SnowboardEffectUpdate)Mini_SnowboardEffectStartFallWhenPlayerNear;
    Mini_SnowboardFillEffectPosFromObject(objectIndex, &g_SnoboRaisedEffectDef, (struct SnowboardEffectPosOffset *)descriptor);
    g_SnoboRaisedEffectDef.variant = descriptor->selector;
    g_SnoboRaisedEffectDef.model = descriptor->selector != 0 ? 0xf1 : 0xf6;
    g_SnoboRaisedEffectDef.heightLimit = g_SnoboRaisedEffectDef.height;
    g_SnoboRaisedEffectDef.height -= descriptor->fallHeight << 12;
    Mini_SnowboardAllocEffectNode(objectIndex, &g_SnoboRaisedEffectDef, 1);
}

// Starts a falling effect once the rider is near. It sets effect[1].segmentDistance to 0xFFFF when
// segmentIndex is more than two ahead of section, otherwise to 0. When the XZ distance from
// g_SnoboStatePtr->relPosX to effect->sidePos, as computed by Mini_SnowboardGetXZDistanceFloatToFixed, is
// below followAngle + 0x400, the update becomes Mini_SnowboardUpdateFallingEffect. fadeRate becomes
// 0x180 if flags is nonzero and 0x40 otherwise, and segmentDistance and fadeLimit both become
// 0x100.
// FUNCTION: FF7 0x0073a848
void __cdecl Mini_SnowboardEffectStartFallWhenPlayerNear(SnowboardEffect *effect)
{
    effect->fade = effect->section - g_SnoboStatePtr->section > 2 ? 0xffff : 0;
    if (Mini_SnowboardGetXZDistanceFloatToFixed(&g_SnoboStatePtr->relPosX, &effect->sidePos) < g_SnoboStatePtr->followAngle + 0x400) {
        effect->update = Mini_SnowboardUpdateFallingEffect;
        if (effect->variant != 0) {
            effect->contactRadius = 384;
            effect->contactTop = 256;
            effect->contactSplit = 0x100;
        } else {
            effect->contactRadius = 64;
            effect->contactTop = 256;
            effect->contactSplit = 0x100;
        }
    }
}

/*
 * Converts the first and third float coordinates at floatPos to integers, subtracts the corresponding
 * integer coordinates at fixedPos, then shifts each difference right 12. Returns SqrtToInt of the sum
 * of their squares. Conversion and multiplication retain the native arithmetic limits.
 */
// FUNCTION: FF7 0x0073a8fa
int __cdecl Mini_SnowboardGetXZDistanceFloatToFixed(float *floatPos, int *fixedPos)
{
    int deltaZ;
    int xDelta;
    xDelta = ((int)floatPos[0] - fixedPos[0]) >> 12;
    deltaZ = ((int)floatPos[2] - fixedPos[2]) >> 12;
    return SqrtToInt(xDelta * xDelta + deltaZ * deltaZ);
}

/*
 * In alternate mode animates an eight-frame opacity field, then accelerates vertical motion and
 * switches to its next callback after exceeding stored height. Plays a one-shot approach sound
 * within 0x20000 of that height. Collision bit two plays another sound and clears collision/size
 * fields.
 */
// FUNCTION: FF7 0x0073a949
void __cdecl Mini_SnowboardUpdateFallingEffect(SnowboardEffect *effect)
{
    if (g_SnoboModeFlag == 1) {
        if ((effect->age)++ < 8) {
            effect->isFalling = 1;
            effect->fadeAmount = 8 - effect->age << 7;
        } else {
            effect->isFalling = 0;
        }
    }
    effect->fallVelocity += 4096;
    effect->height += effect->fallVelocity;
    if (effect->height > effect->heightLimit) {
        effect->update = Mini_SnowboardEffectCountdownThenFadeOut;
        effect->age = 60;
    }
    if (effect->landed == 0 && effect->height > effect->heightLimit - 0x20000) {
        Mini_SnowboardPlaySfxOnChannel(3, 0x1AB, 0);
        effect->landed = 1;
    }
    if ((effect->contact & 2) != 0) {
        Mini_SnowboardPlaySfxOnChannel(2, 0xF0, 0);
        effect->contact = 0;
        effect->contactRadius = 0;
    }
}

// Counts down the effect's delay, then fades it out. While delayTimer is nonzero it decrements it,
// and if flags has bit 1 (0x02) set it plays sound 0xF0 on channel 2, clears the flag byte and sets
// contactRadius to 0. Once delayTimer is zero it clears contactRadius and post-increments fadeTimer, clearing active
// (the active byte) when the value before the increment exceeds 30.
// FUNCTION: FF7 0x0073aa64
void __cdecl Mini_SnowboardEffectCountdownThenFadeOut(SnowboardEffect *effect)
{
    if (effect->age != 0) {
        (effect->age)--;
        if ((effect->contact & 2) != 0) {
            Mini_SnowboardPlaySfxOnChannel(2, 0xf0, 0);
            effect->contact = 0;
            effect->contactRadius = 0;
        }
    } else {
        effect->contactRadius = 0;
        if ((effect->fade)++ > 30)
            effect->active = 0;
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectRaisedForFall with (short)objectIndex and the effect record at g_SnoboRaisedDesc_Slot494.
 * Mechanical.
 */
// FUNCTION: FF7 0x0073aaf9
void __cdecl Mini_SnowboardSlot494SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot494);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectRaisedForFall with (short)objectIndex and the effect record at g_SnoboRaisedDesc_Slot495.
 * Mechanical.
 */
// FUNCTION: FF7 0x0073ab16
void __cdecl Mini_SnowboardSlot495SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot495);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectRaisedForFall with (short)objectIndex and the effect record at g_SnoboRaisedDesc_Slot496.
 * Mechanical.
 */
// FUNCTION: FF7 0x0073ab33
void __cdecl Mini_SnowboardSlot496SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot496);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectRaisedForFall with (short)objectIndex and the effect record at g_SnoboRaisedDesc_Slot497.
 * Mechanical.
 */
// FUNCTION: FF7 0x0073ab50
void __cdecl Mini_SnowboardSlot497SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot497);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectRaisedForFall with (short)objectIndex and the effect record at g_SnoboRaisedDesc_Slot498.
 * Mechanical.
 */
// FUNCTION: FF7 0x0073ab6d
void __cdecl Mini_SnowboardSlot498SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot498);
    }
}

/*
 * When enable is nonzero, calls Mini_SnowboardSpawnEffectRaisedForFall with (short)objectIndex and the effect record at g_SnoboRaisedDesc_Slot499.
 * Mechanical; last of the run of these callbacks whose records lie at 0x00963E00..0x00963F30.
 */
// FUNCTION: FF7 0x0073ab8a
void __cdecl Mini_SnowboardSlot499SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot499);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot500; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073aba7
void __cdecl Mini_SnowboardSlot500SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot500);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot501; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073abc4
void __cdecl Mini_SnowboardSlot501SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot501);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot502; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073abe1
void __cdecl Mini_SnowboardSlot502SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot502);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot503; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073abfe
void __cdecl Mini_SnowboardSlot503SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot503);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot504; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ac1b
void __cdecl Mini_SnowboardSlot504SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot504);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot505; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ac38
void __cdecl Mini_SnowboardSlot505SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot505);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot506; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ac55
void __cdecl Mini_SnowboardSlot506SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot506);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot507; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ac72
void __cdecl Mini_SnowboardSlot507SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot507);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot508; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ac8f
void __cdecl Mini_SnowboardSlot508SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot508);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot509; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073acac
void __cdecl Mini_SnowboardSlot509SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot509);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot510; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073acc9
void __cdecl Mini_SnowboardSlot510SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot510);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the 16-byte effect descriptor at g_SnoboRaisedDesc_Slot513, then
 * Mini_SnowboardSpawnEffectAboveObject with the next record at g_SnoboEffectAboveOffset; with enable zero it
 * does nothing. Mechanical -- same shape as its neighbours, only the descriptor addresses differ.
 */
// FUNCTION: FF7 0x0073ace6
void __cdecl Mini_SnowboardSlot513SpawnEffectRaisedForFallThenAbove(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot513);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboEffectAboveOffset);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot514; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ad15
void __cdecl Mini_SnowboardSlot514SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot514);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot515; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ad32
void __cdecl Mini_SnowboardSlot515SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot515);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the address of the 16-byte effect descriptor at
 * g_SnoboRaisedDesc_Slot516; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ad4f
void __cdecl Mini_SnowboardSlot516SpawnEffectRaisedForFall(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot516);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectRaisedForFall with the 16-byte effect descriptor at g_SnoboRaisedDesc_Slot519, then
 * Mini_SnowboardSpawnEffectAboveObject with the next record at g_SnoboAboveObjectDesc_Slot519; with enable zero it
 * does nothing. Mechanical -- same shape as its neighbours, only the descriptor addresses differ.
 */
// FUNCTION: FF7 0x0073ad6c
void __cdecl Mini_SnowboardSlot519SpawnEffectRaisedForFallThenAbove(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectRaisedForFall((short)objectIndex, (SnowboardRaisedEffect *)&g_SnoboRaisedDesc_Slot519);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot519);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectAboveObject with the 16-byte effect descriptor
 * at g_SnoboAboveObjectDesc_Slot532; with enable zero it does nothing. Mechanical -- one of the run of near-identical
 * triggers over the 0x10-strided descriptor table.
 */
// FUNCTION: FF7 0x0073ad9b
void __cdecl Mini_SnowboardSlot532SpawnEffectAbove(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, &g_SnoboAboveObjectDesc_Slot532.pos);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectTileF2OrF3 with the address of the 16-byte effect descriptor at
 * g_SnoboTileDesc_Slot537; with enable zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073adb8
void __cdecl Mini_SnowboardSlot537SpawnEffectTileF2OrF3(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot537);
    }
}

/*
 * When g_SnoboStatePtr->activeFlags is zero, selects g_SnoboTileEffectDef/g_SnoboTileEffectDef[4] as 0xF2/0x100 or
 * 0xF3/0xC0 according to the descriptor's param, fills the template position and allocates one effect node.
 */
// FUNCTION: FF7 0x0073add5
void __cdecl Mini_SnowboardSpawnEffectTileF2OrF3(short objectIndex, void *descriptor)
{
    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    if (((SnowboardCompactEffectDesc *)descriptor)->param != 0) {
        g_SnoboTileEffectDef.model = 0xF2;
        g_SnoboTileEffectDef.contactRadius = 256;
    } else {
        g_SnoboTileEffectDef.model = 0xF3;
        g_SnoboTileEffectDef.contactRadius = 192;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objectIndex, &g_SnoboTileEffectDef, (struct SnowboardEffectPosOffset *)descriptor);
    Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboTileEffectDef, 1);
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073ae47
void __cdecl Mini_SnowboardSlot539NoOp(void)
{
}

/*
 * Snowboard minigame per-object effect trigger: when enable is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectTileF2OrF3 with the 16-byte effect descriptor at g_SnoboTileDesc_Slot540, then
 * Mini_SnowboardSpawnEffectAboveObject with the next record at g_SnoboAboveObjectDesc_Slot540; with enable zero it
 * does nothing. Mechanical -- same shape as its neighbours, only the descriptor addresses differ.
 */
// FUNCTION: FF7 0x0073ae4c
void __cdecl Mini_SnowboardSlot540SpawnTileF2OrF3ThenEffectAbove(int objectIndex, int enable)
{
    if (enable != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot540);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot540);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when gate is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectTileF2OrF3 with the address of the 16-byte effect descriptor at
 * g_SnoboTileDesc_Slot543; with gate zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ae7b
void __cdecl Mini_SnowboardSlot543SpawnEffectTileF2OrF3(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot543);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when gate is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectTileF2OrF3 with the address of the 16-byte effect descriptor at
 * g_SnoboTileDesc_Slot544; with gate zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073ae98
void __cdecl Mini_SnowboardSlot544SpawnEffectTileF2OrF3(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot544);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when gate is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectTileF2OrF3 with the address of the 16-byte effect descriptor at
 * g_SnoboTileDesc_Slot545; with gate zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073aeb5
void __cdecl Mini_SnowboardSlot545SpawnEffectTileF2OrF3(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot545);
    }
}

/*
 * Snowboard minigame per-object effect trigger: when gate is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectAboveObject with the 16-byte effect descriptor
 * at g_SnoboAboveObjectDesc_Slot546; with gate zero it does nothing. Mechanical -- one of the run of near-identical
 * triggers over the 0x10-strided descriptor table.
 */
// FUNCTION: FF7 0x0073aed2
void __cdecl Mini_SnowboardSlot546SpawnEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot546);
    }
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073aeef
void __cdecl Mini_SnowboardSlot547NoOp(void)
{
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073aef4
void __cdecl Mini_SnowboardSlot548NoOp(void)
{
}

/*
 * Snowboard minigame per-object effect trigger: when gate is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectTileF2OrF3 with the address of the 16-byte effect descriptor at
 * g_SnoboTileDesc_Slot549; with gate zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073aef9
void __cdecl Mini_SnowboardSlot549SpawnEffectTileF2OrF3(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot549);
    }
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073af16
void __cdecl Mini_SnowboardSlot550NoOp(void)
{
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073af1b
void __cdecl Mini_SnowboardSlot551NoOp(void)
{
}

/*
 * Snowboard minigame per-object effect trigger: when gate is nonzero it narrows objectIndex to a short
 * object index and calls Mini_SnowboardSpawnEffectTileF2OrF3 with the address of the 16-byte effect descriptor at
 * g_SnoboTileDesc_Slot552; with gate zero it does nothing. Mechanical -- one of a long run of near-identical
 * triggers that differ only in which record of the 0x10-strided descriptor table they name.
 */
// FUNCTION: FF7 0x0073af20
void __cdecl Mini_SnowboardSlot552SpawnEffectTileF2OrF3(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot552);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectTileF2OrF3 with the effect record at
 * &g_SnoboTileDesc_Slot553 and then Mini_SnowboardSpawnEffectAboveObject with
 * &g_SnoboAboveObjectDesc_Slot553. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073af3d
void __cdecl Mini_SnowboardSlot553SpawnTileF2OrF3ThenEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot553);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot553);
    }
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073af6c
void __cdecl Mini_SnowboardSlot554NoOp(void)
{
}

/*
 * Empty function; reads no state and makes no calls.
 */
// FUNCTION: FF7 0x0073af71
void __cdecl Mini_SnowboardSlot557NoOp(void)
{
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectTileF2OrF3 with the effect record at
 * &g_SnoboTileDesc_Slot560. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073af76
void __cdecl Mini_SnowboardSlot560SpawnEffectTileF2OrF3(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot560);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectTileF2OrF3 with the effect record at
 * &g_SnoboTileDesc_Slot561 and then Mini_SnowboardSpawnEffectAboveObject with
 * &g_SnoboAboveObjectDesc_Slot561. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073af93
void __cdecl Mini_SnowboardSlot561SpawnTileEffectThenEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboTileDesc_Slot561);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot561);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectTileF2OrF3 twice, with the records at
 * &g_SnoboCompactEffect_Slot562[0] and &g_SnoboCompactEffect_Slot562[1]. objectIndex is narrowed to short
 * and passed as the object index. Note the 10-byte gap between the two records here, where the rest
 * of this table steps 16 bytes at a time.
 */
// FUNCTION: FF7 0x0073afc2
void __cdecl Mini_SnowboardSlot563SpawnTwoTileEffects(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboCompactEffect_Slot562[0]);
        Mini_SnowboardSpawnEffectTileF2OrF3((short)objectIndex, &g_SnoboCompactEffect_Slot562[1]);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardQueueHitLaunchWithSignalEffect with the effect record at
 * &g_SnoboHitLaunchDesc_Slot574. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073aff1
void __cdecl Mini_SnowboardSlot574QueueGatedEffect(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboHitLaunchDesc_Slot574);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectAboveObject with the effect record at
 * &g_SnoboAboveObjectDesc_Slot575. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b00e
void __cdecl Mini_SnowboardSlot575SpawnEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot575);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardQueueHitLaunchWithSignalEffect with the effect record at
 * &g_SnoboHitLaunchDesc_Slot576. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b02b
void __cdecl Mini_SnowboardSlot576QueueGatedEffect(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboHitLaunchDesc_Slot576);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardQueueHitLaunchWithSignalEffect with the effect record at
 * &g_SnoboHitLaunchDesc_Slot577. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b048
void __cdecl Mini_SnowboardSlot577QueueGatedEffect(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboHitLaunchDesc_Slot577);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectAboveObject with the effect record at
 * &g_SnoboAboveObjectDesc_Slot578. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b065
void __cdecl Mini_SnowboardSlot578SpawnEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot578);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardQueueHitLaunchWithSignalEffect with the effect record at
 * &g_SnoboHitLaunchDesc_Slot579. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b082
void __cdecl Mini_SnowboardSlot579QueueGatedEffect(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboHitLaunchDesc_Slot579);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardQueueHitLaunchWithSignalEffect with the effect record at
 * &g_SnoboHitLaunchDesc_Slot580. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b09f
void __cdecl Mini_SnowboardSlot580QueueGatedEffect(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboHitLaunchDesc_Slot580);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectAboveObject with the effect record at
 * &g_SnoboAboveObjectDesc_Slot581. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b0bc
void __cdecl Mini_SnowboardSlot581SpawnEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot581);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardQueueHitLaunchWithSignalEffect with the effect record at
 * &g_SnoboHitLaunchDesc_Slot582. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b0d9
void __cdecl Mini_SnowboardSlot582QueueGatedEffect(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardQueueHitLaunchWithSignalEffect((short)objectIndex, &g_SnoboHitLaunchDesc_Slot582);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectAboveObject with the effect record at
 * &g_SnoboAboveObjectDesc_Slot589. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b0f6
void __cdecl Mini_SnowboardSlot589SpawnEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot589);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnLinkedEffectPair with the effect record at
 * &g_SnoboLinkedPairDesc_Slot603 and then Mini_SnowboardSpawnEffectAboveObject with
 * &g_SnoboAboveObjectDesc_Slot603. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b113
void __cdecl Mini_SnowboardSlot603SpawnLinkedPairThenEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot603);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot603);
    }
}

/*
 * Returns in race mode. Otherwise selects the effect template by the supplied object flag, fills
 * its position and allocates two linked effect nodes. Offsets their height and copies object
 * parameters into the primary node. Allocation results are unchecked before primary-node access.
 */
// FUNCTION: FF7 0x0073b142
void __cdecl Mini_SnowboardSpawnLinkedEffectPair(short objectIndex, SnoboLinkedEffectSpec *spec)
{
    int linkedNode;
    SnowboardEffect *node;

    if (g_SnoboStatePtr->activeFlags != 0) {
        return;
    }
    if (spec->flag != 0) {
        g_SnoboLinkedEffectDef.model = 0xE8;
        g_SnoboLinkedEffectDef.contactRadius = 160;
        g_SnoboLinkedEffectDef.contactTop = 256;
        g_SnoboLinkedEffectDef.contactSplit = 0xE0;
    } else {
        g_SnoboLinkedEffectDef.model = 0xE9;
        g_SnoboLinkedEffectDef.contactRadius = 96;
        g_SnoboLinkedEffectDef.contactTop = 192;
        g_SnoboLinkedEffectDef.contactSplit = 0xA0;
    }
    Mini_SnowboardFillEffectPosFromObject((short)objectIndex, &g_SnoboLinkedEffectDef, (struct SnowboardEffectPosOffset *)spec);
    node = (SnowboardEffect *)(Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboLinkedEffectDef, 1));
    node->height -= (spec->flag != 0 ? 64 : 0) + 64 << 12;
    g_SnoboLinkedEffectTopDef.height = node->height - ((spec->flag != 0 ? 64 : 0) + 64 << 12);
    g_SnoboLinkedEffectTopDef.rotY = spec->flags;
    linkedNode = Mini_SnowboardAllocEffectNode((short)objectIndex, &g_SnoboLinkedEffectTopDef, 1);
    node->linked = linkedNode;
    node->swingShift = spec->swingShift;
    node->stepShift = spec->stepShift;
    node->timer = spec->startTimer;
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnLinkedEffectPair with the effect record at
 * &g_SnoboLinkedPairDesc_Slot607. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b27e
void __cdecl Mini_SnowboardSlot607SpawnLinkedEffectPair(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot607);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnLinkedEffectPair with the records at &g_SnoboLinkedPairDesc_Slot611A
 * and &g_SnoboLinkedPairDesc_Slot611B, then Mini_SnowboardSpawnEffectAboveObject with
 * &g_SnoboAboveObjectDesc_Slot611. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b29b
void __cdecl Mini_SnowboardSlot611SpawnTwoLinkedPairsThenEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot611A);
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot611B);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, &g_SnoboAboveObjectDesc_Slot611.pos);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnLinkedEffectPair with the effect record at
 * &g_SnoboLinkedPairDesc_Slot616. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b2dc
void __cdecl Mini_SnowboardSlot616SpawnLinkedEffectPair(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot616);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnLinkedEffectPair with the records at &g_SnoboLinkedPairDesc_Slot620A
 * and &g_SnoboLinkedPairDesc_Slot620B, then Mini_SnowboardSpawnEffectAboveObject with
 * &g_SnoboAboveObjectDesc_Slot620. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b2f9
void __cdecl Mini_SnowboardSlot620SpawnTwoLinkedPairsThenEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot620A);
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot620B);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, &g_SnoboAboveObjectDesc_Slot620.pos);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnLinkedEffectPair with the effect record at
 * &g_SnoboLinkedPairDesc_Slot627 and then Mini_SnowboardSpawnEffectAboveObject with
 * &g_SnoboAboveObjectDesc_Slot627. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b33a
void __cdecl Mini_SnowboardSlot627SpawnLinkedPairThenEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot627);
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot627);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnLinkedEffectPair with the effect record at
 * &g_SnoboLinkedPairDesc_Slot631. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b369
void __cdecl Mini_SnowboardSlot631SpawnLinkedEffectPair(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnLinkedEffectPair((short)objectIndex, (SnoboLinkedEffectSpec *)&g_SnoboLinkedPairDesc_Slot631);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectAboveObject with the effect record at
 * &g_SnoboAboveObjectDesc_Slot634. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b386
void __cdecl Mini_SnowboardSlot634SpawnEffectAbove(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectAboveObject((short)objectIndex, g_SnoboAboveObjectDesc_Slot634);
    }
}

/*
 * One of the snowboard minigame's per-effect entry points: does nothing unless gate is nonzero,
 * otherwise calls Mini_SnowboardSpawnEffectNodeAtObjectOffsets with the effect record at
 * &g_SnoboNodeOffsetsDesc_Slot635. objectIndex is narrowed to short and passed as the object index; the
 * record addresses are consecutive 16-byte entries in the effect-descriptor table at
 * g_SnoboEffectAboveOffset.
 */
// FUNCTION: FF7 0x0073b3a3
void __cdecl Mini_SnowboardSlot635SpawnEffectNode(int objectIndex, int gate)
{
    if (gate != 0) {
        Mini_SnowboardSpawnEffectNodeAtObjectOffsets((short)objectIndex, &g_SnoboNodeOffsetsDesc_Slot635);
    }
}

/*
 * Clears 160 consecutive dwords beginning at g_SnoboEventVisitFlags and clears g_SnoboSignalValue.
 */
// FUNCTION: FF7 0x0073b3c0
void __cdecl Mini_SnowboardClearEventVisitFlags(void)
{
    int *cursor, remaining;
    remaining = 160;
    cursor = (int *)g_SnoboEventVisitFlags;
    while (remaining-- != 0) {
        *cursor = 0;
        cursor++;
    }
    g_SnoboSignalValue = 0;
}

/*
 * Marks byte slotIndex in g_SnoboEventVisitFlags on its first visit and invokes the optional
 * g_SnoboEffectCallbacks[slotIndex] callback with slotIndex and a flag indicating that first visit.
 */
// FUNCTION: FF7 0x0073b405
void __cdecl Mini_SnowboardVisitEventSlot(short slotIndex)
{
    int firstVisit;
    firstVisit = 0;
    if (((char *)g_SnoboEventVisitFlags)[slotIndex] == 0) {
        ((char *)g_SnoboEventVisitFlags)[slotIndex] = 1;
        firstVisit = 1;
    }
    if (g_SnoboEffectCallbacks[slotIndex] != 0)
        g_SnoboEffectCallbacks[slotIndex](slotIndex, firstVisit);
}

/*
 * Clears the first byte of all 32 0x90-byte records at g_SnoboEffectRecords and clears
 * g_SnoboEffectNodeCursor/g_SnoboValue.
 */
// FUNCTION: FF7 0x0073b45a
void __cdecl Mini_SnowboardClearEffectRecords(void)
{
    SnowboardEffect *recordPtr;
    int remaining;
    remaining = 32;
    for (recordPtr = g_SnoboEffectRecords; remaining-- != 0; recordPtr++)
        recordPtr->active = 0;
    g_SnoboEffectNodeCursor = 0;
    g_SnoboValue = 0;
}

/*
 * Updates 32 active 0x90-byte minigame records. Activity bit 2 bypasses both age expiration and
 * same-state contact filtering. Conditionally invokes callbacks with an activity recheck, skips
 * disabled/alternating records, builds or reuses cached XYZ rotation matrices and optionally
 * appends render records when render mode and primary model are nonzero, without capacity checks.
 * Applies vertical/radial proximity tests, marks contact and selects one of four response bytes
 * from height and followAngle. Response 0x80 maps to 8 or 12, response 0x81 maps to 12 and quarters two
 * motion floats, and response 8 can become 9 by frame modulo 12 and halves those floats. Zero
 * response clears motion. Later records can overwrite the current response.
 */
// FUNCTION: FF7 0x0073b4a9
void __cdecl Mini_SnowboardUpdateEffectRecords(SnowboardCourseNodeState *pose)
{
    SnowboardEffect *ent;
    int limitVal, ofs, deltaValue, hgt, i;
    int *position;
    short *matCursor;
    SnowboardMatrixRecord *mtxRecord, *secondRecord;
    pose->nodeIndex = g_SnoboStatePtr->section;
    pose->raceRunning = (char)g_SnoboStatePtr->running;
    position = pose->colors;
    position[0] = (int)g_SnoboStatePtr->relPosX >> 12;
    position[1] = (int)g_SnoboStatePtr->relPosY >> 12;
    position[2] = (int)g_SnoboStatePtr->riderPos >> 12;
    for (i = 0, ent = g_SnoboEffectRecords; i < 32; i++, ent++) {
        if ((unsigned char)ent->active == 0)
            continue;
        if (((unsigned char)ent->active & 2) == 0 && ent->section < pose->nodeIndex) {
            ent->active = 0;
            g_SnoboEffectNodeCursor = (short)i;
            continue;
        }
        if (pose->raceRunning != 0 && ent->update != 0) {
            ent->update(ent);
            if ((unsigned char)ent->active == 0)
                continue;
        }
        if (ent->fade == 0xffff)
            continue;
        if (g_SnoboStatePtr->renderFrame != 0 && ent->fade != 0) {
            ent->blinkCounter
            ++;
            if ((ent->blinkCounter & 1) != 0)
                continue;
        }
        if (ent->matrixValid == 0) {
            RotMatrixXYZ(&ent->rotX, (void *)pose->rotation);
            hgt = ent->matrixShift;
            if (hgt != 0) {
                matCursor = pose->rotation;
                deltaValue = 9;
                while (deltaValue-- != 0) {
                    *matCursor <<= hgt;
                    matCursor++;
                }
            }
            ent->matrix = *(Copy30 *)pose->rotation;
            ent->matrixValid = 1;
        } else {
            *(Copy30 *)pose->rotation = ent->matrix;
        }
        pose->translateX = (ent->sidePos >> 12) - pose->baseX;
        pose->translateY = (ent->height >> 12) - pose->baseY;
        pose->translateZ = (ent->forward >> 12) - pose->baseZ;
        GtePsxMatrixToFloat4x4((PsxMatrix *)pose->rotation, pose->worldMatrix);
        if (g_SnoboStatePtr->renderFrame != 0 && ent->model != 0) {
            mtxRecord = (SnowboardMatrixRecord *)(g_SnoboMatrixRecordCount * 0x4c + (char *)&g_SnoboMatrixRecords);
            g_SnoboMatrixRecordCount++;
            mtxRecord->matrix = *(Copy64 *)pose->worldMatrix;
            mtxRecord->kind = 0;
            mtxRecord->actionCode = ent->model;
            mtxRecord->animFrame = ent->renderArg0;
            mtxRecord->drawFlags = ent->renderArg1;
            if (ent->model2 != 0) {
                secondRecord = (SnowboardMatrixRecord *)(g_SnoboMatrixRecordCount * 0x4c + (char *)&g_SnoboMatrixRecords);
                g_SnoboMatrixRecordCount++;
                secondRecord->kind = 2;
                secondRecord->actionCode = ent->model2;
                secondRecord->animFrame = ent->renderArg0;
                secondRecord->drawFlags = ent->renderArg1;
            }
        }
        if (ent->contactRadius == 0)
            continue;
        if (((unsigned char)ent->active & 2) == 0 && ent->section != pose->nodeIndex)
            continue;
        switch ((char)g_SnoboStatePtr->riderState) {
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0A:
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0C:
        case SNOWBOARD_RIDER_ACTION_HALVE_VEL_0E:
            if (g_SnoboStatePtr->frameCount < 8)
                continue;
        }
        hgt = ent->height >> 12;
        deltaValue = hgt - ent->contactTop;
        if (position[1] < deltaValue)
            continue;
        deltaValue = hgt + 0x100;
        if (position[1] > deltaValue)
            continue;
        deltaValue = hgt - ent->contactSplit;
        ofs = position[1] < deltaValue;
        limitVal = ent->contactRadius;
        deltaValue = abs(position[0] - (ent->sidePos >> 12));
        if (deltaValue > limitVal)
            continue;
        hgt = abs(position[2] - (ent->forward >> 12));
        if (hgt > limitVal)
            continue;
        deltaValue = SqrtToInt(deltaValue * deltaValue + hgt * hgt);
        if (deltaValue > limitVal)
            continue;
        ent->contact |= 1;
        if (ent->contactResponse[0] == 0xff)
            continue;
        Mini_SnowboardRestoreRiderSnapshot();
        if (g_SnoboStatePtr->followAngle > 0x100) {
            ofs += 2;
            ent->contact |= 2;
            ent->hitVelocityX = (int)g_SnoboStatePtr->velocityX;
            ent->hitVelocityZ = (int)g_SnoboStatePtr->velocityZ;
        }
        if (ent->contactResponse[ofs] != 0) {
            deltaValue = ent->contactResponse[ofs];
            if (deltaValue == 0x80) {
                if ((char)g_SnoboStatePtr->riderState == 11)
                    deltaValue = 12;
                else
                    deltaValue = g_SnoboStatePtr->followAngle > 0x280 ? 8 : 12;
            } else if (deltaValue == 0x81) {
                deltaValue = 12;
                g_SnoboStatePtr->velocityX *= 0.25f;
                g_SnoboStatePtr->velocityZ *= 0.25f;
            }
            if (deltaValue == 8) {
                if (g_SnoboStatePtr->triangle % 12 >= 6)
                    deltaValue++;
                g_SnoboStatePtr->velocityX -= g_SnoboStatePtr->velocityX * 0.5f;
                g_SnoboStatePtr->velocityZ -= g_SnoboStatePtr->velocityZ * 0.5f;
            }
            g_SnoboStatePtr->riderState = (short)deltaValue;
        } else {
            *(int *)&g_SnoboStatePtr->velocityZ = 0;
            *(int *)&g_SnoboStatePtr->velocityX = 0;
        }
    }
}
} /* extern "C" */

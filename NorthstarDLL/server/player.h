#pragma once

#include "basecombatcharacter.h"

struct fogparams_t
{
	char gap[8];
	float botAlt; // 0x8 ( Size: 4 )
	float topAlt; // 0xc ( Size: 4 )
	float halfDistBot; // 0x10 ( Size: 4 )
	float halfDistTop; // 0x14 ( Size: 4 )
	float distColorStr; // 0x18 ( Size: 4 )
	float dirColorStr; // 0x1c ( Size: 4 )
	float distOffset; // 0x20 ( Size: 4 )
	float densityScale; // 0x24 ( Size: 4 )
	float halfAngleDeg; // 0x28 ( Size: 4 )
	float HDRColorScale; // 0x2c ( Size: 4 )
	Color distColor; // 0x30 ( Size: 4 )
	Color dirColor; // 0x34 ( Size: 4 )
	Vector3 direction; // 0x38 ( Size: 12 )
	float minFadeTime; // 0x44 ( Size: 4 )
	bool forceOntoSky; // 0x48 ( Size: 1 )
	bool enable; // 0x49 ( Size: 1 )
	char gap_4a[2];
	int id; // 0x4c ( Size: 4 )
};
static_assert(sizeof(fogparams_t) == 80);

struct sky3dparams_t
{
	char gap[8];
	int scale; // 0x8 ( Size: 4 )
	int cellNum; // 0xc ( Size: 4 )
	bool useWorldFog; // 0x10 ( Size: 1 )
	char gap_11[7];
	fogparams_t fog; // 0x18 ( Size: 80 )
};
static_assert(sizeof(sky3dparams_t) == 104);

struct fogplayerparamsstate_t
{
	char gap[8];
	bool enable; // 0x8 ( Size: 1 )
	char gap_9[3];
	float botAlt; // 0xc ( Size: 4 )
	float topAlt; // 0x10 ( Size: 4 )
	float halfDistBot; // 0x14 ( Size: 4 )
	float halfDistTop; // 0x18 ( Size: 4 )
	float distOffset; // 0x1c ( Size: 4 )
	float densityScale; // 0x20 ( Size: 4 )
	float halfAngleDeg; // 0x24 ( Size: 4 )
	float distColorStr; // 0x28 ( Size: 4 )
	float dirColorStr; // 0x2c ( Size: 4 )
	float HDRColorScale; // 0x30 ( Size: 4 )
	float minFadeTime; // 0x34 ( Size: 4 )
	bool forceOntoSky; // 0x38 ( Size: 1 )
	Color distColor; // 0x39 ( Size: 4 )
	Color dirColor; // 0x3d ( Size: 4 )
	Vector3 direction; // 0x44 ( Size: 12 )
	int id; // 0x50 ( Size: 4 )
	char gap_54[4];
};
static_assert(sizeof(fogplayerparamsstate_t) == 88);

struct fogplayerparams_t
{
	char gap[8];
	int /*EHANDLE*/ m_hCtrl; // 0x8 ( Size: 4 )
	float m_flTransitionStartTime; // 0xc ( Size: 4 )
	fogplayerparamsstate_t m_Old; // 0x10 ( Size: 88 )
	fogplayerparamsstate_t m_New; // 0x68 ( Size: 88 )
	fogplayerparamsstate_t m_OldSky; // 0xc0 ( Size: 88 )
	fogplayerparamsstate_t m_NewSky; // 0x118 ( Size: 88 )
};
static_assert(sizeof(fogplayerparams_t) == 368);

struct audioparams_t
{
	char gap_0[8];
	Vector3 localSound[8]; // 0x8 ( Size: 96 )
	int soundscapeIndex; // 0x68 ( Size: 4 )
	int localBits; // 0x6c ( Size: 4 )
	int entIndex; // 0x70 ( Size: 4 )
	char gap_74[4];
};
static_assert(sizeof(audioparams_t) == 120);

class CPlayerState
{
	void* vftable;

  public:
	int currentClass; // 0x8 ( Size: 4 )
	int requestedClass; // 0xc ( Size: 4 )
	int onDeathClass; // 0x10 ( Size: 4 )
	int oldClass; // 0x14 ( Size: 4 )
	Vector3 a_angle; // 0x18 ( Size: 12 )
	char gap_24[4];
	__int64 netname; // 0x28 ( Size: 8 )
	int fixangle; // 0x30 ( Size: 4 )
	Vector3 anglechange; // 0x34 ( Size: 12 )
	int index; // 0x40 ( Size: 4 )
	bool replay; // 0x44 ( Size: 1 )
	char gap_45[3];
	int lastPlayerView_tickcount; // 0x48 ( Size: 4 )
	Vector3 lastPlayerView_origin; // 0x4c ( Size: 12 )
	Vector3 lastPlayerView_angle; // 0x58 ( Size: 12 )
	bool deadflag; // 0x64 ( Size: 1 )
	char gap_65[3];
	Vector3 localViewAngles; // 0x68 ( Size: 12 )
	Vector3 worldViewAngles; // 0x74 ( Size: 12 )
};
static_assert(sizeof(CPlayerState) == 128);

class Rodeo_PlayerData
{
	void* vftable;

  public:
	int stage; // 0x8 ( Size: 4 )
	bool canRodeo; // 0xc ( Size: 1 )
	char gap_d[3];
	int rodeoCountParity; // 0x10 ( Size: 4 )
	float startTime; // 0x14 ( Size: 4 )
	float endTime; // 0x18 ( Size: 4 )
	int /*EHANDLE*/ targetEnt; // 0x1c ( Size: 4 )
	int /*EHANDLE*/ prevEnt; // 0x20 ( Size: 4 )
	float prevEntCooldown; // 0x24 ( Size: 4 )
	int pilot1pSequenceIndex; // 0x28 ( Size: 4 )
	int pilot3pSequenceIndex; // 0x2c ( Size: 4 )
	int targetAttachmentIndex; // 0x30 ( Size: 4 )
	float rodeoStabilizeStrength; // 0x34 ( Size: 4 )
	bool rodeoStabilizeViewFirstFrame; // 0x38 ( Size: 1 )
	char gap_39[3];
	matrix3x4_t lastPlayerToWorld; // 0x3c ( Size: 48 )
	Quaternion initialCameraCorrection; // 0x6c ( Size: 16 )
};
static_assert(sizeof(Rodeo_PlayerData) == 128);

class ClassModValues
{
  public:
	float health; // 0x0 ( Size: 4 )
	float healthShield; // 0x4 ( Size: 4 )
	float healthDoomed; // 0x8 ( Size: 4 )
	float healthPerSegment; // 0xc ( Size: 4 )
	float powerRegenRate; // 0x10 ( Size: 4 )
	float dodgeDuration; // 0x14 ( Size: 4 )
	float dodgeSpeed; // 0x18 ( Size: 4 )
	float dodgePowerDrain; // 0x1c ( Size: 4 )
	float smartAmmoLockTimeModifier; // 0x20 ( Size: 4 )
	float wallrunAccelerateVertical; // 0x24 ( Size: 4 )
	float wallrunAccelerateHorizontal; // 0x28 ( Size: 4 )
	float wallrunMaxSpeedHorizontal; // 0x2c ( Size: 4 )
	float wallrun_timeLimit; // 0x30 ( Size: 4 )
	float wallrun_hangTimeLimit; // 0x34 ( Size: 4 )
	bool wallrunAllowed; // 0x38 ( Size: 1 )
	char gap_39[3];
	int wallrunAdsType; // 0x3c ( Size: 4 )
	bool doubleJumpAllowed; // 0x40 ( Size: 1 )
	char gap_41[3];
	float pitchMaxUp; // 0x44 ( Size: 4 )
	float pitchMaxDown; // 0x48 ( Size: 4 )
	bool mantlePitchLeveling; // 0x4c ( Size: 1 )
	bool dodgeAllowed; // 0x4d ( Size: 1 )
	bool sprintAllowed; // 0x4e ( Size: 1 )
	bool stealthSounds; // 0x4f ( Size: 1 )
	bool hoverEnabled; // 0x50 ( Size: 1 )
	char gap_51[3];
	float grapple_power_regen_delay; // 0x54 ( Size: 4 )
	float grapple_power_regen_rate; // 0x58 ( Size: 4 )
	float slideFOVScale; // 0x5c ( Size: 4 )
	float slideFOVLerpInTime; // 0x60 ( Size: 4 )
	float slideFOVLerpOutTime; // 0x64 ( Size: 4 )
	float airSlowMoSpeed; // 0x68 ( Size: 4 )
	int sharedEnergyTotal; // 0x6c ( Size: 4 )
	float sharedEnergyRegenRate; // 0x70 ( Size: 4 )
};
static_assert(sizeof(ClassModValues) == 116);

class PerPosClassModValues
{
  public:
	float speed_; // 0x0 ( Size: 4 )
	float sprintSpeed_; // 0x4 ( Size: 4 )
};
static_assert(sizeof(PerPosClassModValues) == 8);

class ThirdPersonViewData
{
	void* vftable;

  public:
	Vector3 m_thirdPersonEntViewOffset; // 0x8 ( Size: 12 )
	bool m_thirdPersonEntPitchIsFreelook; // 0x14 ( Size: 1 )
	bool m_thirdPersonEntYawIsFreelook; // 0x15 ( Size: 1 )
	bool m_thirdPersonEntUseFixedDist; // 0x16 ( Size: 1 )
	bool m_thirdPersonEntPushedInByGeo; // 0x17 ( Size: 1 )
	bool m_thirdPersonEntDrawViewmodel; // 0x18 ( Size: 1 )
	char gap_19[3];
	float m_thirdPersonEntBlendTotalDuration; // 0x1c ( Size: 4 )
	float m_thirdPersonEntBlendEaseInDuration; // 0x20 ( Size: 4 )
	float m_thirdPersonEntBlendEaseOutDuration; // 0x24 ( Size: 4 )
	float m_thirdPersonEntFixedPitch; // 0x28 ( Size: 4 )
	float m_thirdPersonEntFixedYaw; // 0x2c ( Size: 4 )
	float m_thirdPersonEntFixedDist; // 0x30 ( Size: 4 )
	float m_thirdPersonEntMinYaw; // 0x34 ( Size: 4 )
	float m_thirdPersonEntMaxYaw; // 0x38 ( Size: 4 )
	float m_thirdPersonEntMinPitch; // 0x3c ( Size: 4 )
	float m_thirdPersonEntMaxPitch; // 0x40 ( Size: 4 )
	float m_thirdPersonEntSpringToCenterRate; // 0x44 ( Size: 4 )
	float m_thirdPersonEntLookaheadLowerEntSpeed; // 0x48 ( Size: 4 )
	float m_thirdPersonEntLookaheadUpperEntSpeed; // 0x4c ( Size: 4 )
	float m_thirdPersonEntLookaheadMaxAngle; // 0x50 ( Size: 4 )
	float m_thirdPersonEntLookaheadLerpAheadRate; // 0x54 ( Size: 4 )
	float m_thirdPersonEntLookaheadLerpToCenterRate; // 0x58 ( Size: 4 )
};
static_assert(sizeof(ThirdPersonViewData) == 96);

class GrappleData
{
	void* vftable;

  public:
	Vector3 m_grappleVel; // 0x8 ( Size: 12 )
	Vector3 m_grapplePoints[4]; // 0x14 ( Size: 48 )
	int m_grapplePointCount; // 0x44 ( Size: 4 )
	bool m_grappleAttached; // 0x48 ( Size: 1 )
	bool m_grapplePulling; // 0x49 ( Size: 1 )
	bool m_grappleRetracting; // 0x4a ( Size: 1 )
	bool m_grappleForcedRetracting; // 0x4b ( Size: 1 )
	float m_grappleUsedPower; // 0x4c ( Size: 4 )
	float m_grapplePullTime; // 0x50 ( Size: 4 )
	float m_grappleAttachTime; // 0x54 ( Size: 4 )
	float m_grappleDetachTime; // 0x58 ( Size: 4 )
	int /*EHANDLE*/ m_grappleMeleeTarget; // 0x5c ( Size: 4 )
	bool m_grappleHasGoodVelocity; // 0x60 ( Size: 1 )
	char gap_61[3];
	float m_grappleLastGoodVelocityTime; // 0x64 ( Size: 4 )
};
static_assert(sizeof(GrappleData) == 104);

class PlayerZiplineData
{
	void* vftable;

  public:
	bool m_ziplineReenableWeapons; // 0x8 ( Size: 1 )
	char gap_9[3];
	float m_mountingZiplineDuration; // 0xc ( Size: 4 )
	float m_mountingZiplineAlpha; // 0x10 ( Size: 4 )
	float m_ziplineStartTime; // 0x14 ( Size: 4 )
	float m_ziplineEndTime; // 0x18 ( Size: 4 )
	Vector3 m_mountingZiplineSourcePosition; // 0x1c ( Size: 12 )
	Vector3 m_mountingZiplineSourceVelocity; // 0x28 ( Size: 12 )
	Vector3 m_mountingZiplineTargetPosition; // 0x34 ( Size: 12 )
	Vector3 m_ziplineUsePosition; // 0x40 ( Size: 12 )
};
static_assert(sizeof(PlayerZiplineData) == 80);

class Player_OperatorData
{
	void* vftable;

  public:
	bool diving; // 0x8 ( Size: 1 )
	bool cameraEnabled; // 0x9 ( Size: 1 )
	char gap_a[2];
	float minPitch; // 0xc ( Size: 4 )
	float maxPitch; // 0x10 ( Size: 4 )
	float followDistance; // 0x14 ( Size: 4 )
	float followHeight; // 0x18 ( Size: 4 )
	bool shootFromPlayer; // 0x1c ( Size: 1 )
	char gap_1d[3];
	float smoothDuration; // 0x20 ( Size: 4 )
	float smoothFollowDistanceStartTime; // 0x24 ( Size: 4 )
	float smoothFollowDistanceStartValue; // 0x28 ( Size: 4 )
	float smoothFollowDistanceEndValue; // 0x2c ( Size: 4 )
	float smoothFollowHeightStartTime; // 0x30 ( Size: 4 )
	float smoothFollowHeightStartValue; // 0x34 ( Size: 4 )
	float smoothFollowHeightEndValue; // 0x38 ( Size: 4 )
	float smoothMinPitchStartTime; // 0x3c ( Size: 4 )
	float smoothMinPitchStartValue; // 0x40 ( Size: 4 )
	float smoothMinPitchEndValue; // 0x44 ( Size: 4 )
	float smoothMaxPitchStartTime; // 0x48 ( Size: 4 )
	float smoothMaxPitchStartValue; // 0x4c ( Size: 4 )
	float smoothMaxPitchEndValue; // 0x50 ( Size: 4 )
	bool forceDefaultFloorHeight; // 0x54 ( Size: 1 )
	char gap_55[3];
	float defaultFloorHeight; // 0x58 ( Size: 4 )
	bool ignoreWorldForMovement; // 0x5c ( Size: 1 )
	bool ignoreWorldForFloorTrace; // 0x5d ( Size: 1 )
	char gap_5e[2];
	float moveGridSizeScale; // 0x60 ( Size: 4 )
	float moveFloorHeightOffset; // 0x64 ( Size: 4 )
	bool jumpIsDodge; // 0x68 ( Size: 1 )
	char gap_69[3];
	float jumpAcceleration; // 0x6c ( Size: 4 )
	float jumpMaxSpeed; // 0x70 ( Size: 4 )
	float hoverAcceleration; // 0x74 ( Size: 4 )
	float hoverMaxSpeed; // 0x78 ( Size: 4 )
};
static_assert(sizeof(Player_OperatorData) == 128);

class Player_ViewOffsetEntityData
{
	void* vftable;

  public:
	int /*EHANDLE*/ viewOffsetEntityHandle; // 0x8 ( Size: 4 )
	float lerpInDuration; // 0xc ( Size: 4 )
	float lerpOutDuration; // 0x10 ( Size: 4 )
	bool stabilizePlayerEyeAngles; // 0x14 ( Size: 1 )
};
static_assert(sizeof(Player_ViewOffsetEntityData) == 24);

class Player_AnimViewEntityData
{
	void* vftable;

  public:
	int /*EHANDLE*/ animViewEntityHandle; // 0x8 ( Size: 4 )
	float animViewEntityAngleLerpInDuration; // 0xc ( Size: 4 )
	float animViewEntityOriginLerpInDuration; // 0x10 ( Size: 4 )
	float animViewEntityLerpOutDuration; // 0x14 ( Size: 4 )
	bool animViewEntityStabilizePlayerEyeAngles; // 0x18 ( Size: 1 )
	char gap_19[3];
	int animViewEntityThirdPersonCameraParity; // 0x1c ( Size: 4 )
	int animViewEntityThirdPersonCameraAttachment[6]; // 0x20 ( Size: 24 )
	int animViewEntityNumThirdPersonCameraAttachments; // 0x38 ( Size: 4 )
	bool animViewEntityThirdPersonCameraVisibilityChecks; // 0x3c ( Size: 1 )
	char gap_3d[3];
	int animViewEntityParity; // 0x40 ( Size: 4 )
	int lastAnimViewEntityParity; // 0x44 ( Size: 4 )
	Vector3 animViewEntityCameraPosition; // 0x48 ( Size: 12 )
	Vector3 animViewEntityCameraAngles; // 0x54 ( Size: 12 )
	float animViewEntityBlendStartTime; // 0x60 ( Size: 4 )
	Vector3 animViewEntityBlendStartEyePosition; // 0x64 ( Size: 12 )
	Vector3 animViewEntityBlendStartEyeAngles; // 0x70 ( Size: 12 )
};
static_assert(sizeof(Player_AnimViewEntityData) == 128);

class CurrentData_Player
{
	void* vftable;

  public:
	float m_flHullHeight; // 0x8 ( Size: 4 )
	float m_traversalAnimProgress; // 0xc ( Size: 4 )
	float m_sprintTiltFrac; // 0x10 ( Size: 4 )
	Vector3 m_angEyeAngles; // 0x14 ( Size: 12 )
};
static_assert(sizeof(CurrentData_Player) == 32);

class CurrentData_LocalPlayer
{
	void* vftable;

  public:
	Vector3 m_viewConeAngleMin; // 0x8 ( Size: 12 )
	Vector3 m_viewConeAngleMax; // 0x14 ( Size: 12 )
	Vector3 m_stepSmoothingOffset; // 0x20 ( Size: 12 )
	Vector3 m_vecPunchBase_Angle; // 0x2c ( Size: 12 )
	Vector3 m_vecPunchBase_AngleVel; // 0x38 ( Size: 12 )
	Vector3 m_vecPunchWeapon_Angle; // 0x44 ( Size: 12 )
	Vector3 m_vecPunchWeapon_AngleVel; // 0x50 ( Size: 12 )
};
static_assert(sizeof(CurrentData_LocalPlayer) == 96);

class ScriptOriginatedDamageInfo
{
  public:
	int m_scriptDamageType; // 0x0 ( Size: 4 )
	int m_damageSourceIdentifier; // 0x4 ( Size: 4 )
	int m_scriptAttackerClass; // 0x8 ( Size: 4 )
};
static_assert(sizeof(ScriptOriginatedDamageInfo) == 12);

class CTakeDamageInfo
{
  public:
	Vector3 m_vecDamageForce; // 0x0 ( Size: 12 )
	Vector3 m_vecDamagePosition; // 0xc ( Size: 12 )
	Vector3 m_vecReportedPosition; // 0x18 ( Size: 12 )
	int /*EHANDLE*/ m_hInflictor; // 0x24 ( Size: 4 )
	int /*EHANDLE*/ m_hAttacker; // 0x28 ( Size: 4 )
	int /*EHANDLE*/ m_hWeapon; // 0x2c ( Size: 4 )
	short m_hWeaponFileInfo; // 0x30 ( Size: 2 )
	bool m_forceKill; // 0x32 ( Size: 1 )
	char gap_33[1];
	float m_flDamage; // 0x34 ( Size: 4 )
	float m_damageCriticalScale; // 0x38 ( Size: 4 )
	float m_flMaxDamage; // 0x3c ( Size: 4 )
	float m_flHeavyArmorDamageScale; // 0x40 ( Size: 4 )
	int m_bitsDamageType; // 0x44 ( Size: 4 )
	float m_flRadius; // 0x48 ( Size: 4 )
	int m_hitGroup; // 0x4c ( Size: 4 )
	int m_hitBox; // 0x50 ( Size: 4 )
	ScriptOriginatedDamageInfo m_scriptDamageInfo; // 0x54 ( Size: 12 )
	int m_deathPackage; // 0x60 ( Size: 4 )
	float m_distanceFromAttackOrigin; // 0x64 ( Size: 4 )
	float m_distanceFromExplosionCenter; // 0x68 ( Size: 4 )
	bool m_doDeathForce; // 0x6c ( Size: 1 )
	char gap_6d[3];
	int m_damageFlags; // 0x70 ( Size: 4 )
	int m_flinchDirection; // 0x74 ( Size: 4 )
};
static_assert(sizeof(CTakeDamageInfo) == 120);

class PlayerMelee_PlayerData
{
	void* vftable;

  public:
	bool attackActive; // 0x8 ( Size: 1 )
	bool attackRecoveryShouldBeQuick; // 0x9 ( Size: 1 )
	char gap_a[2];
	float attackStartTime; // 0xc ( Size: 4 )
	int /*EHANDLE*/ attackHitEntity; // 0x10 ( Size: 4 )
	float attackHitEntityTime; // 0x14 ( Size: 4 )
	float attackLastHitNonWorldEntity; // 0x18 ( Size: 4 )
	int scriptedState; // 0x1c ( Size: 4 )
	bool pendingMeleePress; // 0x20 ( Size: 1 )
};
static_assert(sizeof(PlayerMelee_PlayerData) == 40);

class CPlayerShared
{
	void* vftable;

  public:
	int m_nPlayerCond; // 0x8 ( Size: 4 )
	bool m_bLoadoutUnavailable; // 0xc ( Size: 1 )
	char gap_d[3];
	float m_flCondExpireTimeLeft[2]; // 0x10 ( Size: 8 )
	void* m_pOuter; // 0x18 ( Size: 8 )
	float m_flNextCritUpdate; // 0x20 ( Size: 4 )
	float m_flTauntRemoveTime; // 0x24 ( Size: 4 )
	CTakeDamageInfo m_damageInfo; // 0x28 ( Size: 120 )
};
static_assert(sizeof(CPlayerShared) == 160);

class StatusEffectTimedData
{
	void* vftable;

  public:
	int seComboVars; // 0x8 ( Size: 4 )
	float seTimeEnd; // 0xc ( Size: 4 )
	float seEaseOut; // 0x10 ( Size: 4 )
};
static_assert(sizeof(StatusEffectTimedData) == 24);

class StatusEffectEndlessData
{
	void* vftable;

  public:
	int seComboVars; // 0x8 ( Size: 4 )
};
static_assert(sizeof(StatusEffectEndlessData) == 16);

class PushHistoryEntry
{
  public:
	float time; // 0x0 ( Size: 4 )
	Vector3 pushed; // 0x4 ( Size: 12 )
};
static_assert(sizeof(PushHistoryEntry) == 16);

class PredictableServerEvent
{
  public:
	int type; // 0x0 ( Size: 4 )
	float deadlineTime; // 0x4 ( Size: 4 )
	int fullSizeOfUnion[4]; // 0x8 ( Size: 16 )
};
static_assert(sizeof(PredictableServerEvent) == 24);

class CPlayerLocalData
{
	void* vftable;

  public:
	int m_iHideHUD; // 0x8 ( Size: 4 )
	Vector3 m_vecOverViewpoint; // 0xc ( Size: 12 )
	bool m_duckToggleOn; // 0x18 ( Size: 1 )
	char gap_19[3];
	int m_forceStance; // 0x1c ( Size: 4 )
	int m_nDuckTransitionTimeMsecs; // 0x20 ( Size: 4 )
	int m_superJumpsUsed; // 0x24 ( Size: 4 )
	bool m_jumpedOffRodeo; // 0x28 ( Size: 1 )
	char gap_29[3];
	float m_flSuitPower; // 0x2c ( Size: 4 )
	float m_flSuitJumpPower; // 0x30 ( Size: 4 )
	float m_flSuitGrapplePower; // 0x34 ( Size: 4 )
	int m_nStepside; // 0x38 ( Size: 4 )
	float m_flFallVelocity; // 0x3c ( Size: 4 )
	int m_nOldButtons; // 0x40 ( Size: 4 )
	float m_oldForwardMove; // 0x44 ( Size: 4 )
	CBaseEntity* m_pOldSkyCamera; // 0x48 ( Size: 8 )
	float m_accelScale; // 0x50 ( Size: 4 )
	float m_powerRegenRateScale; // 0x54 ( Size: 4 )
	float m_dodgePowerDelayScale; // 0x58 ( Size: 4 )
	bool m_bDrawViewmodel; // 0x5c ( Size: 1 )
	char gap_5d[3];
	float m_flStepSize; // 0x60 ( Size: 4 )
	bool m_bAllowAutoMovement; // 0x64 ( Size: 1 )
	char gap_65[3];
	float m_airSlowMoFrac; // 0x68 ( Size: 4 )
	int predictableFlags; // 0x6c ( Size: 4 )
	int m_bitsActiveDevices; // 0x70 ( Size: 4 )
	int /*EHANDLE*/ m_hSkyCamera; // 0x74 ( Size: 4 )
	sky3dparams_t m_skybox3d; // 0x78 ( Size: 104 )
	fogplayerparams_t m_PlayerFog; // 0xe0 ( Size: 368 )
	fogparams_t m_fog; // 0x250 ( Size: 80 )
	audioparams_t m_audio; // 0x2a0 ( Size: 120 )
	float m_animNearZ; // 0x318 ( Size: 4 )
	Vector3 m_airMoveBlockPlanes[2]; // 0x31c ( Size: 24 )
	float m_airMoveBlockPlaneTime; // 0x334 ( Size: 4 )
	int m_airMoveBlockPlaneCount; // 0x338 ( Size: 4 )
	float m_queuedMeleePressTime; // 0x33c ( Size: 4 )
	float m_queuedGrappleMeleeTime; // 0x340 ( Size: 4 )
	bool m_queuedMeleeAttackAnimEvent; // 0x344 ( Size: 1 )
	bool m_disableMeleeUntilRelease; // 0x345 ( Size: 1 )
	char gap_346[2];
	float m_meleePressTime; // 0x348 ( Size: 4 )
	int m_meleeDisabledCounter; // 0x34c ( Size: 4 )
	int /*EHANDLE*/ lastAttacker; // 0x350 ( Size: 4 )
	int attackedCount; // 0x354 ( Size: 4 )
	int m_trackedChildProjectileCount; // 0x358 ( Size: 4 )
	bool m_oneHandedWeaponUsage; // 0x35c ( Size: 1 )
	bool m_prevOneHandedWeaponUsage; // 0x35d ( Size: 1 )
	char gap_35e[2];
	float m_flCockpitEntryTime; // 0x360 ( Size: 4 )
	float m_ejectStartTime; // 0x364 ( Size: 4 )
	float m_disembarkStartTime; // 0x368 ( Size: 4 )
	float m_hotDropImpactTime; // 0x36c ( Size: 4 )
	float m_outOfBoundsDeadTime; // 0x370 ( Size: 4 )
	int m_objectiveIndex; // 0x374 ( Size: 4 )
	int /*EHANDLE*/ m_objectiveEntity; // 0x378 ( Size: 4 )
	float m_objectiveEndTime; // 0x37c ( Size: 4 )
	int m_cinematicEventFlags; // 0x380 ( Size: 4 )
	bool m_forcedDialogueOnly; // 0x384 ( Size: 1 )
	char gap_385[3];
	float m_titanBuildTime; // 0x388 ( Size: 4 )
	float m_titanBubbleShieldTime; // 0x38c ( Size: 4 )
	bool m_titanEmbarkEnabled; // 0x390 ( Size: 1 )
	bool m_titanDisembarkEnabled; // 0x391 ( Size: 1 )
	char gap_392[2];
	int m_voicePackIndex; // 0x394 ( Size: 4 )
	float m_playerAnimUpdateTime; // 0x398 ( Size: 4 )
	float m_playerAnimLastAimTurnTime; // 0x39c ( Size: 4 )
	float m_playerAnimCurrentFeetYaw; // 0x3a0 ( Size: 4 )
	float m_playerAnimEstimateYaw; // 0x3a4 ( Size: 4 )
	float m_playerAnimGoalFeetYaw; // 0x3a8 ( Size: 4 )
	bool m_playerAnimJumping; // 0x3ac ( Size: 1 )
	char gap_3ad[3];
	float m_playerAnimJumpStartTime; // 0x3b0 ( Size: 4 )
	bool m_playerAnimFirstJumpFrame; // 0x3b4 ( Size: 1 )
	bool m_playerAnimDodging; // 0x3b5 ( Size: 1 )
	char gap_3b6[2];
	float m_playerLandStartTime; // 0x3b8 ( Size: 4 )
	int m_playerAnimJumpActivity; // 0x3bc ( Size: 4 )
	Vector3 m_playerAnimLastWallRunNormal; // 0x3c0 ( Size: 12 )
	bool m_playerAnimLanding; // 0x3cc ( Size: 1 )
	bool m_playerAnimShouldLand; // 0x3cd ( Size: 1 )
	char gap_3ce[2];
	float m_playerAnimLandStartTime; // 0x3d0 ( Size: 4 )
	bool m_playerAnimInAirWalk; // 0x3d4 ( Size: 1 )
	char gap_3d5[3];
	float m_playerAnimPrevFrameSequenceMotionYaw; // 0x3d8 ( Size: 4 )
	float m_playerAnimMovementPlaybackRate; // 0x3dc ( Size: 4 )
	float m_fake_playerAnimUpdateTime; // 0x3e0 ( Size: 4 )
	float m_fake_playerAnimLastAimTurnTime; // 0x3e4 ( Size: 4 )
	float m_fake_playerAnimCurrentFeetYaw; // 0x3e8 ( Size: 4 )
	float m_fake_playerAnimEstimateYaw; // 0x3ec ( Size: 4 )
	float m_fake_playerAnimGoalFeetYaw; // 0x3f0 ( Size: 4 )
	bool m_fake_playerAnimJumping; // 0x3f4 ( Size: 1 )
	char gap_3f5[3];
	float m_fake_playerAnimJumpStartTime; // 0x3f8 ( Size: 4 )
	bool m_fake_playerAnimFirstJumpFrame; // 0x3fc ( Size: 1 )
	bool m_fake_playerAnimDodging; // 0x3fd ( Size: 1 )
	char gap_3fe[2];
	float m_fake_playerLandStartTime; // 0x400 ( Size: 4 )
	int m_fake_playerAnimJumpActivity; // 0x404 ( Size: 4 )
	Vector3 m_fake_playerAnimLastWallRunNormal; // 0x408 ( Size: 12 )
	bool m_fake_playerAnimLanding; // 0x414 ( Size: 1 )
	bool m_fake_playerAnimShouldLand; // 0x415 ( Size: 1 )
	char gap_416[2];
	float m_fake_playerAnimLandStartTime; // 0x418 ( Size: 4 )
	bool m_fake_playerAnimInAirWalk; // 0x41c ( Size: 1 )
	char gap_41d[3];
	float m_fake_playerAnimPrevFrameSequenceMotionYaw; // 0x420 ( Size: 4 )
	float m_fake_playerAnimMovementPlaybackRate; // 0x424 ( Size: 4 )
};
static_assert(sizeof(CPlayerLocalData) == 1064);

namespace R2
{
    class CPlayer : public CBaseCombatCharacter
    {
      public:
        char m_szNetname[256]; // 0x1498 ( Size: 256 )
        bool m_bZooming; // 0x1598 ( Size: 1 )
        bool m_zoomToggleOn; // 0x1599 ( Size: 1 )
        char gap_159a[2];
        float m_zoomBaseFrac; // 0x159c ( Size: 4 )
        float m_zoomBaseTime; // 0x15a0 ( Size: 4 )
        float m_zoomFullStartTime; // 0x15a4 ( Size: 4 )
        int m_physicsSolidMask; // 0x15a8 ( Size: 4 )
        int m_StuckLast; // 0x15ac ( Size: 4 )
        CPlayerLocalData m_Local; // 0x15b0 ( Size: 1064 )
        fogplayerparams_t m_PlayerFog; // 0x19d8 ( Size: 368 )
        char m_hTriggerTonemapList[32]; // 0x1b48 ( Size: 32 ) // custom
        int /*EHANDLE*/ m_hColorCorrectionCtrl; // 0x1b68 ( Size: 4 )
        char gap_1b6c[4];
        char m_hTriggerSoundscapeList[32]; // 0x1b70 ( Size: 32 ) // custom
        CPlayerState pl; // 0x1b90 ( Size: 128 )
        Rodeo_PlayerData m_rodeo; // 0x1c10 ( Size: 128 )
        bool m_hasBadReputation; // 0x1c90 ( Size: 1 )
        char m_communityName[64]; // 0x1c91 ( Size: 64 )
        char m_communityClanTag[16]; // 0x1cd1 ( Size: 16 )
        char m_factionName[16]; // 0x1ce1 ( Size: 16 )
        char m_hardwareIcon[16]; // 0x1cf1 ( Size: 16 )
        bool m_happyHourActive; // 0x1d01 ( Size: 1 )
        char gap_1d02[6];
        __int64 m_platformUserId; // 0x1d08 ( Size: 8 )
        int m_classModsActive; // 0x1d10 ( Size: 4 )
        int m_classModsActiveOld; // 0x1d14 ( Size: 4 )
        ClassModValues m_classModValues; // 0x1d18 ( Size: 116 )
        int m_posClassModsActive[4]; // 0x1d8c ( Size: 16 )
        int m_posClassModsActiveOld[4]; // 0x1d9c ( Size: 16 )
        PerPosClassModValues m_perPosValues[4]; // 0x1dac ( Size: 8 )
        bool m_passives[128]; // 0x1dcc ( Size: 128 )
        int m_communityId; // 0x1e4c ( Size: 4 )
        int m_nButtons; // 0x1e50 ( Size: 4 )
        int m_afButtonPressed; // 0x1e54 ( Size: 4 )
        int m_afButtonReleased; // 0x1e58 ( Size: 4 )
        int m_afButtonLast; // 0x1e5c ( Size: 4 )
        int m_afButtonDisabled; // 0x1e60 ( Size: 4 )
        int m_afButtonForced; // 0x1e64 ( Size: 4 )
        float m_forwardMove; // 0x1e68 ( Size: 4 )
        float m_sideMove; // 0x1e6c ( Size: 4 )
        float m_prevForwardMove; // 0x1e70 ( Size: 4 )
        float m_prevSideMove; // 0x1e74 ( Size: 4 )
        bool m_bLagCompensation; // 0x1e78 ( Size: 1 )
        bool m_bPredictWeapons; // 0x1e79 ( Size: 1 )
        bool m_bPredictionEnabled; // 0x1e7a ( Size: 1 )
        bool m_wantedToMatchmake; // 0x1e7b ( Size: 1 )
        int /*EHANDLE*/ m_skyCamera; // 0x1e7c ( Size: 4 )
        int /*EHANDLE*/ m_titanSoulBeingRodeoed; // 0x1e80 ( Size: 4 )
        int /*EHANDLE*/ m_entitySyncingWithMe; // 0x1e84 ( Size: 4 )
        int m_playerFlags; // 0x1e88 ( Size: 4 )
        bool m_hasMic; // 0x1e8c ( Size: 1 )
        bool m_inPartyChat; // 0x1e8d ( Size: 1 )
        char gap_1e8e[2];
        float m_playerMoveSpeedScale; // 0x1e90 ( Size: 4 )
        int m_gestureSequences[4]; // 0x1e94 ( Size: 16 )
        float m_gestureStartTimes[4]; // 0x1ea4 ( Size: 16 )
        float m_gestureBlendInDuration[4]; // 0x1eb4 ( Size: 16 )
        float m_gestureBlendOutDuration[4]; // 0x1ec4 ( Size: 16 )
        float m_gestureFadeOutStartTime[4]; // 0x1ed4 ( Size: 16 )
        float m_gestureFadeOutDuration[4]; // 0x1ee4 ( Size: 16 )
        int m_gestureAutoKillBitfield; // 0x1ef4 ( Size: 4 )
        bool m_bDropEnabled; // 0x1ef8 ( Size: 1 )
        bool m_bDuckEnabled; // 0x1ef9 ( Size: 1 )
        char gap_1efa[2];
        int m_iRespawnFrames; // 0x1efc ( Size: 4 )
        int m_afPhysicsFlags; // 0x1f00 ( Size: 4 )
        int /*EHANDLE*/ m_remoteTurret; // 0x1f04 ( Size: 4 )
        float m_flTimeLastTouchedGround; // 0x1f08 ( Size: 4 )
        float m_flTimeLastJumped; // 0x1f0c ( Size: 4 )
        float m_flTimeLastLanded; // 0x1f10 ( Size: 4 )
        Vector3 m_upDirWhenLastTouchedGround; // 0x1f14 ( Size: 12 )
        bool m_bHasJumpedSinceTouchedGround; // 0x1f20 ( Size: 1 )
        char gap_1f21[3];
        float m_holdToUseTimeLeft; // 0x1f24 ( Size: 4 )
        float m_fTimeLastHurt; // 0x1f28 ( Size: 4 )
        float m_fLastAimBotCheckTime; // 0x1f2c ( Size: 4 )
        Vector3 m_accumDamageImpulseVel; // 0x1f30 ( Size: 12 )
        float m_accumDamageImpulseTime; // 0x1f3c ( Size: 4 )
        float m_damageImpulseNoDecelEndTime; // 0x1f40 ( Size: 4 )
        int /*EHANDLE*/ m_hDmgEntity; // 0x1f44 ( Size: 4 )
        float m_DmgTake; // 0x1f48 ( Size: 4 )
        int m_bitsDamageType; // 0x1f4c ( Size: 4 )
        int m_bitsHUDDamage; // 0x1f50 ( Size: 4 )
        float m_xpRate; // 0x1f54 ( Size: 4 )
        float m_flDeathTime; // 0x1f58 ( Size: 4 )
        float m_flDeathAnimTime; // 0x1f5c ( Size: 4 )
        bool m_frozen; // 0x1f60 ( Size: 1 )
        bool m_stressAnimation; // 0x1f61 ( Size: 1 )
        char gap_1f62[2];
        int m_iObserverMode; // 0x1f64 ( Size: 4 )
        int m_iObserverLastMode; // 0x1f68 ( Size: 4 )
        int /*EHANDLE*/ m_hObserverTarget; // 0x1f6c ( Size: 4 )
        Vector3 m_observerModeStaticPosition; // 0x1f70 ( Size: 12 )
        Vector3 m_observerModeStaticAngles; // 0x1f7c ( Size: 12 )
        bool m_isValidChaseObserverTarget; // 0x1f88 ( Size: 1 )
        char gap_1f89[3];
        int m_vphysicsCollisionState; // 0x1f8c ( Size: 4 )
        bool m_bHasVPhysicsCollision; // 0x1f90 ( Size: 1 )
        char gap_1f91[3];
        float m_fNextSuicideTime; // 0x1f94 ( Size: 4 )
        int m_iSuicideCustomKillFlags; // 0x1f98 ( Size: 4 )
        int m_preNoClipPhysicsFlags; // 0x1f9c ( Size: 4 )
        int /*EHANDLE*/ m_hTonemapController; // 0x1fa0 ( Size: 4 )
        int m_activeBurnCardIndex; // 0x1fa4 ( Size: 4 )
        char m_CommandContext[32]; // 0x1fa8 ( Size: 32 ) // void
        void* m_pPhysicsController; // 0x1fc8 ( Size: 8 )
        void* m_pShadowStand; // 0x1fd0 ( Size: 8 )
        void* m_pShadowCrouch; // 0x1fd8 ( Size: 8 )
        Vector3 m_oldOrigin; // 0x1fe0 ( Size: 12 )
        Vector3 m_vecSmoothedVelocity; // 0x1fec ( Size: 12 )
        bool m_bTouchedPhysObject; // 0x1ff8 ( Size: 1 )
        bool m_bPhysicsWasFrozen; // 0x1ff9 ( Size: 1 )
        char gap_1ffa[2];
        int m_iTargetVolume; // 0x1ffc ( Size: 4 )
        float m_flDuckTime; // 0x2000 ( Size: 4 )
        float m_flDuckJumpTime; // 0x2004 ( Size: 4 )
        bool m_VDU; // 0x2008 ( Size: 1 )
        bool m_fInitHUD; // 0x2009 ( Size: 1 )
        bool m_fGameHUDInitialized; // 0x200a ( Size: 1 )
        bool m_fWeapon; // 0x200b ( Size: 1 )
        int m_iUpdateTime; // 0x200c ( Size: 4 )
        int m_iConnected; // 0x2010 ( Size: 4 )
        int m_iPlayerLocked; // 0x2014 ( Size: 4 )
        int m_gameStats[12]; // 0x2018 ( Size: 48 )
        int /*EHANDLE*/ m_firstPersonProxy; // 0x2048 ( Size: 4 )
        int /*EHANDLE*/ m_predictedFirstPersonProxy; // 0x204c ( Size: 4 )
        int /*EHANDLE*/ m_grappleHook; // 0x2050 ( Size: 4 )
        int /*EHANDLE*/ m_petTitan; // 0x2054 ( Size: 4 )
        int m_petTitanMode; // 0x2058 ( Size: 4 )
        int m_xp; // 0x205c ( Size: 4 )
        int m_generation; // 0x2060 ( Size: 4 )
        int m_rank; // 0x2064 ( Size: 4 )
        int m_serverForceIncreasePlayerListGenerationParity; // 0x2068 ( Size: 4 )
        bool m_isPlayingRanked; // 0x206c ( Size: 1 )
        char gap_206d[3];
        float m_skill_mu; // 0x2070 ( Size: 4 )
        int /*EHANDLE*/ m_hardpointEntity; // 0x2074 ( Size: 4 )
        float m_nextTitanRespawnAvailable; // 0x2078 ( Size: 4 )
        bool m_activeViewmodelModifiers[25]; // 0x207c ( Size: 25 )
        bool m_activeViewmodelModifiersChanged; // 0x2095 ( Size: 1 )
        char gap_2096[2];
        int /*EHANDLE*/ m_hViewModel; // 0x2098 ( Size: 4 )
        char gap_209c[4];
        char m_LastCmd[312]; // 0x20a0 ( Size: 312 ) // void
        void* m_pCurrentCommand; // 0x21d8 ( Size: 8 )
        float m_flStepSoundTime; // 0x21e0 ( Size: 4 )
        float m_flStepSoundReduceTime; // 0x21e4 ( Size: 4 )
        int /*EHANDLE*/ m_hThirdPersonEnt; // 0x21e8 ( Size: 4 )
        char gap_21ec[4];
        ThirdPersonViewData m_thirdPerson; // 0x21f0 ( Size: 96 )
        int m_duckState; // 0x2250 ( Size: 4 )
        Vector3 m_StandHullMin; // 0x2254 ( Size: 12 )
        Vector3 m_StandHullMax; // 0x2260 ( Size: 12 )
        Vector3 m_DuckHullMin; // 0x226c ( Size: 12 )
        Vector3 m_DuckHullMax; // 0x2278 ( Size: 12 )
        Vector3 m_upDir; // 0x2284 ( Size: 12 )
        Vector3 m_upDirPredicted; // 0x2290 ( Size: 12 )
        Vector3 m_lastWallRunStartPos; // 0x229c ( Size: 12 )
        float m_wallRunStartTime; // 0x22a8 ( Size: 4 )
        float m_wallRunClearTime; // 0x22ac ( Size: 4 )
        int m_wallRunCount; // 0x22b0 ( Size: 4 )
        bool m_wallRunWeak; // 0x22b4 ( Size: 1 )
        char gap_22b5[3];
        float m_wallRunPushAwayTime; // 0x22b8 ( Size: 4 )
        float m_wallrunFrictionScale; // 0x22bc ( Size: 4 )
        float m_groundFrictionScale; // 0x22c0 ( Size: 4 )
        float m_wallrunRetryTime; // 0x22c4 ( Size: 4 )
        Vector3 m_wallrunRetryPos; // 0x22c8 ( Size: 12 )
        Vector3 m_wallrunRetryNormal; // 0x22d4 ( Size: 12 )
        bool m_wallHanging; // 0x22e0 ( Size: 1 )
        char gap_22e1[3];
        float m_wallHangStartTime; // 0x22e4 ( Size: 4 )
        float m_wallHangTime; // 0x22e8 ( Size: 4 )
        int m_traversalType; // 0x22ec ( Size: 4 )
        int m_traversalState; // 0x22f0 ( Size: 4 )
        Vector3 m_traversalBegin; // 0x22f4 ( Size: 12 )
        Vector3 m_traversalMid; // 0x2300 ( Size: 12 )
        Vector3 m_traversalEnd; // 0x230c ( Size: 12 )
        float m_traversalMidFrac; // 0x2318 ( Size: 4 )
        Vector3 m_traversalForwardDir; // 0x231c ( Size: 12 )
        Vector3 m_traversalRefPos; // 0x2328 ( Size: 12 )
        float m_traversalProgress; // 0x2334 ( Size: 4 )
        float m_traversalStartTime; // 0x2338 ( Size: 4 )
        float m_traversalHandAppearTime; // 0x233c ( Size: 4 )
        float m_traversalReleaseTime; // 0x2340 ( Size: 4 )
        float m_traversalBlendOutStartTime; // 0x2344 ( Size: 4 )
        Vector3 m_traversalBlendOutStartOffset; // 0x2348 ( Size: 12 )
        float m_traversalYawDelta; // 0x2354 ( Size: 4 )
        int m_traversalYawPoseParameter; // 0x2358 ( Size: 4 )
        float m_wallDangleJumpOffTime; // 0x235c ( Size: 4 )
        bool m_wallDangleMayHangHere; // 0x2360 ( Size: 1 )
        bool m_wallDangleForceFallOff; // 0x2361 ( Size: 1 )
        bool m_wallDangleLastPushedForward; // 0x2362 ( Size: 1 )
        char gap_2363[1];
        int m_wallDangleDisableWeapon; // 0x2364 ( Size: 4 )
        float m_wallDangleClimbProgressFloor; // 0x2368 ( Size: 4 )
        float m_prevMoveYaw; // 0x236c ( Size: 4 )
        float m_sprintTiltVel; // 0x2370 ( Size: 4 )
        int m_sprintTiltPoseParameter; // 0x2374 ( Size: 4 )
        int m_sprintFracPoseParameter; // 0x2378 ( Size: 4 )
        char gap_237c[4];
        GrappleData m_grapple; // 0x2380 ( Size: 104 )
        bool m_grappleActive; // 0x23e8 ( Size: 1 )
        bool m_grappleNeedWindowCheck; // 0x23e9 ( Size: 1 )
        char gap_23ea[2];
        int /*EHANDLE*/ m_grappleNextWindowHint; // 0x23ec ( Size: 4 )
        bool m_sliding; // 0x23f0 ( Size: 1 )
        bool m_slideLongJumpAllowed; // 0x23f1 ( Size: 1 )
        char gap_23f2[2];
        float m_lastSlideTime; // 0x23f4 ( Size: 4 )
        float m_lastSlideBoost; // 0x23f8 ( Size: 4 )
        int /*EHANDLE*/ m_activeZipline; // 0x23fc ( Size: 4 )
        bool m_ziplineReverse; // 0x2400 ( Size: 1 )
        char gap_2401[3];
        int /*EHANDLE*/ m_lastZipline; // 0x2404 ( Size: 4 )
        float m_useLastZiplineCooldown; // 0x2408 ( Size: 4 )
        bool m_ziplineValid3pWeaponLayerAnim; // 0x240c ( Size: 1 )
        char gap_240d[3];
        int m_ziplineState; // 0x2410 ( Size: 4 )
        char gap_2414[4];
        PlayerZiplineData m_zipline; // 0x2418 ( Size: 80 )
        Player_OperatorData m_operator; // 0x2468 ( Size: 128 )
        Player_ViewOffsetEntityData m_viewOffsetEntity; // 0x24e8 ( Size: 24 )
        Player_AnimViewEntityData m_animViewEntity; // 0x2500 ( Size: 128 )
        bool m_highSpeedViewmodelAnims; // 0x2580 ( Size: 1 )
        char gap_2581[3];
        int m_gravityGrenadeStatusEffect; // 0x2584 ( Size: 4 )
        float m_onSlopeTime; // 0x2588 ( Size: 4 )
        Vector3 m_lastWallNormal; // 0x258c ( Size: 12 )
        bool m_dodgingInAir; // 0x2598 ( Size: 1 )
        bool m_dodging; // 0x2599 ( Size: 1 )
        char gap_259a[2];
        float m_lastDodgeTime; // 0x259c ( Size: 4 )
        float m_airSpeed; // 0x25a0 ( Size: 4 )
        float m_airAcceleration; // 0x25a4 ( Size: 4 )
        bool m_iSpawnParity; // 0x25a8 ( Size: 1 )
        bool m_boosting; // 0x25a9 ( Size: 1 )
        bool m_repeatedBoost; // 0x25aa ( Size: 1 )
        char gap_25ab[1];
        float m_boostMeter; // 0x25ac ( Size: 4 )
        bool m_jetpack; // 0x25b0 ( Size: 1 )
        bool m_gliding; // 0x25b1 ( Size: 1 )
        char gap_25b2[2];
        float m_glideMeter; // 0x25b4 ( Size: 4 )
        float m_glideRechargeDelayAccumulator; // 0x25b8 ( Size: 4 )
        bool m_hovering; // 0x25bc ( Size: 1 )
        bool m_climbing; // 0x25bd ( Size: 1 )
        bool m_isPerformingBoostAction; // 0x25be ( Size: 1 )
        char gap_25bf[1];
        float m_lastJumpHeight; // 0x25c0 ( Size: 4 )
        int m_numPingsUsed; // 0x25c4 ( Size: 4 )
        int m_numPingsAvailable; // 0x25c8 ( Size: 4 )
        float m_lastPingTime; // 0x25cc ( Size: 4 )
        float m_pingGroupStartTime; // 0x25d0 ( Size: 4 )
        int m_pingGroupAccumulator; // 0x25d4 ( Size: 4 )
        __int64 m_lastBodySound1p; // 0x25d8 ( Size: 8 )
        __int64 m_lastBodySound3p; // 0x25e0 ( Size: 8 )
        __int64 m_lastFinishSound1p; // 0x25e8 ( Size: 8 )
        __int64 m_lastFinishSound3p; // 0x25f0 ( Size: 8 )
        __int64 m_primedSound1p; // 0x25f8 ( Size: 8 )
        __int64 m_primedSound3p; // 0x2600 ( Size: 8 )
        CurrentData_Player m_currentFramePlayer; // 0x2608 ( Size: 32 )
        CurrentData_LocalPlayer m_currentFrameLocalPlayer; // 0x2628 ( Size: 96 )
        int m_nImpulse; // 0x2688 ( Size: 4 )
        float m_flFlashTime; // 0x268c ( Size: 4 )
        float m_flForwardMove; // 0x2690 ( Size: 4 )
        float m_flSideMove; // 0x2694 ( Size: 4 )
        int m_nNumCrateHudHints; // 0x2698 ( Size: 4 )
        bool m_needStuckCheck; // 0x269c ( Size: 1 )
        char gap_269d[3];
        float m_totalFrameTime; // 0x26a0 ( Size: 4 )
        float m_joinFrameTime; // 0x26a4 ( Size: 4 )
        int m_lastUCmdSimulationTicks; // 0x26a8 ( Size: 4 )
        float m_lastUCmdSimulationRemainderTime; // 0x26ac ( Size: 4 )
        float m_totalExtraClientCmdTimeAttempted; // 0x26b0 ( Size: 4 )
        bool m_bGamePaused; // 0x26b4 ( Size: 1 )
        bool m_bPlayerUnderwater; // 0x26b5 ( Size: 1 )
        char gap_26b6[2];
        int /*EHANDLE*/ m_hPlayerViewEntity; // 0x26b8 ( Size: 4 )
        bool m_bShouldDrawPlayerWhileUsingViewEntity; // 0x26bc ( Size: 1 )
        char gap_26bd[3];
        int /*EHANDLE*/ m_hConstraintEntity; // 0x26c0 ( Size: 4 )
        Vector3 m_vecConstraintCenter; // 0x26c4 ( Size: 12 )
        float m_flConstraintRadius; // 0x26d0 ( Size: 4 )
        float m_flConstraintWidth; // 0x26d4 ( Size: 4 )
        float m_flConstraintSpeedFactor; // 0x26d8 ( Size: 4 )
        bool m_bConstraintPastRadius; // 0x26dc ( Size: 1 )
        char gap_26dd[3];
        float m_lastActiveTime; // 0x26e0 ( Size: 4 )
        float m_flLaggedMovementValue; // 0x26e4 ( Size: 4 )
        float m_lastMoveInputTime; // 0x26e8 ( Size: 4 )
        Vector3 m_vNewVPhysicsPosition; // 0x26ec ( Size: 12 )
        Vector3 m_vNewVPhysicsVelocity; // 0x26f8 ( Size: 12 )
        Vector3 m_vNewVPhysicsWishVel; // 0x2704 ( Size: 12 )
        Vector3 m_vecPreviouslyPredictedOrigin; // 0x2710 ( Size: 12 )
        int m_nBodyPitchPoseParam; // 0x271c ( Size: 4 )
        __int64 m_lastNavArea; // 0x2720 ( Size: 8 ) // void
        char m_szNetworkIDString[64]; // 0x2728 ( Size: 64 )
        __int64 m_squad; // 0x2768 ( Size: 8 )
        __int64 m_SquadName; // 0x2770 ( Size: 8 )
        char m_gameMovementUtil[56]; // 0x2778 ( Size: 56 ) // void
        float m_flTimeAllSuitDevicesOff; // 0x27b0 ( Size: 4 )
        bool m_bIsStickySprinting; // 0x27b4 ( Size: 1 )
        char gap_27b5[3];
        float m_fStickySprintMinTime; // 0x27b8 ( Size: 4 )
        bool m_bPlayedSprintStartEffects; // 0x27bc ( Size: 1 )
        char gap_27bd[3];
        int m_autoSprintForced; // 0x27c0 ( Size: 4 )
        bool m_fIsSprinting; // 0x27c4 ( Size: 1 )
        bool m_fIsWalking; // 0x27c5 ( Size: 1 )
        char gap_27c6[2];
        float m_useHeldTime; // 0x27c8 ( Size: 4 )
        float m_sprintStartedTime; // 0x27cc ( Size: 4 )
        float m_sprintStartedFrac; // 0x27d0 ( Size: 4 )
        float m_sprintEndedTime; // 0x27d4 ( Size: 4 )
        float m_sprintEndedFrac; // 0x27d8 ( Size: 4 )
        float m_stickySprintStartTime; // 0x27dc ( Size: 4 )
        bool m_bSinglePlayerGameEnding; // 0x27e0 ( Size: 1 )
        char gap_27e1[3];
        int m_ubEFNoInterpParity; // 0x27e4 ( Size: 4 )
        bool m_viewConeActive; // 0x27e8 ( Size: 1 )
        bool m_viewConeParented; // 0x27e9 ( Size: 1 )
        char gap_27ea[2];
        int m_viewConeParity; // 0x27ec ( Size: 4 )
        int m_lastViewConeParityTick; // 0x27f0 ( Size: 4 )
        float m_viewConeLerpTime; // 0x27f4 ( Size: 4 )
        bool m_viewConeSpecificEnabled; // 0x27f8 ( Size: 1 )
        char gap_27f9[3];
        Vector3 m_viewConeSpecific; // 0x27fc ( Size: 12 )
        Vector3 m_viewConeRelativeAngleMin; // 0x2808 ( Size: 12 )
        Vector3 m_viewConeRelativeAngleMax; // 0x2814 ( Size: 12 )
        int /*EHANDLE*/ m_hReservedSpawnPoint; // 0x2820 ( Size: 4 )
        int /*EHANDLE*/ m_hLastSpawnPoint; // 0x2824 ( Size: 4 )
        bool m_autoKickDisabled; // 0x2828 ( Size: 1 )
        char gap_2829[3];
        Vector3 m_movementCollisionNormal; // 0x282c ( Size: 12 )
        Vector3 m_groundNormal; // 0x2838 ( Size: 12 )
        int /*EHANDLE*/ m_stuckCharacter; // 0x2844 ( Size: 4 )
        char m_title[32]; // 0x2848 ( Size: 32 )
        bool sentHUDScriptChecksum; // 0x2868 ( Size: 1 )
        bool m_bIsFullyConnected; // 0x2869 ( Size: 1 )
        char gap_286a[2];
        CTakeDamageInfo m_lastDeathInfo; // 0x286c ( Size: 120 )
        char gap_28e4[4];
        PlayerMelee_PlayerData m_melee; // 0x28e8 ( Size: 40 )
        int /*EHANDLE*/ m_lungeTargetEntity; // 0x2910 ( Size: 4 )
        bool m_isLungingToPosition; // 0x2914 ( Size: 1 )
        char gap_2915[3];
        Vector3 m_lungeTargetPosition; // 0x2918 ( Size: 12 )
        Vector3 m_lungeStartPositionOffset; // 0x2924 ( Size: 12 )
        Vector3 m_lungeStartPositionOffset_notLagCompensated; // 0x2930 ( Size: 12 )
        Vector3 m_lungeEndPositionOffset; // 0x293c ( Size: 12 )
        float m_lungeStartTime; // 0x2948 ( Size: 4 )
        float m_lungeEndTime; // 0x294c ( Size: 4 )
        bool m_lungeCanFly; // 0x2950 ( Size: 1 )
        bool m_lungeLockPitch; // 0x2951 ( Size: 1 )
        char gap_2952[2];
        float m_lungeStartPitch; // 0x2954 ( Size: 4 )
        float m_lungeSmoothTime; // 0x2958 ( Size: 4 )
        float m_lungeMaxTime; // 0x295c ( Size: 4 )
        float m_lungeMaxEndSpeed; // 0x2960 ( Size: 4 )
        bool m_useCredit; // 0x2964 ( Size: 1 )
        char gap_2965[3];
        __int64 m_smartAmmoNextTarget; // 0x2968 ( Size: 8 )
        __int64 m_smartAmmoPrevTarget; // 0x2970 ( Size: 8 )
        float m_smartAmmoHighestLocksOnMeFractionValues[4]; // 0x2978 ( Size: 16 )
        int /*EHANDLE*/ m_smartAmmoHighestLocksOnMeEntities[4]; // 0x2988 ( Size: 16 )
        float m_smartAmmoPreviousHighestLockOnMeFractionValue; // 0x2998 ( Size: 4 )
        float m_smartAmmoPendingHighestLocksOnMeFractionValues[4]; // 0x299c ( Size: 16 )
        char gap_29ac[4];
        CBaseEntity* m_smartAmmoPendingHighestLocksOnMeEntities[4]; // 0x29b0 ( Size: 32 )
        bool m_smartAmmoRemoveFromTargetList; // 0x29d0 ( Size: 1 )
        char gap_29d1[3];
        int m_delayedFlinchEvents; // 0x29d4 ( Size: 4 )
        __int64 m_delayedFlinchEventCount; // 0x29d8 ( Size: 8 )
        char m_extraWeaponModNames[512]; // 0x29e0 ( Size: 512 )
        char m_extraWeaponModNamesArray[64]; // 0x2be0 ( Size: 64 ) // void
        __int64 m_extraWeaponModNameCount; // 0x2c20 ( Size: 8 )
        __int64 m_pPlayerAISquad; // 0x2c28 ( Size: 8 ) // void
        float m_flAreaCaptureScoreAccumulator; // 0x2c30 ( Size: 4 )
        float m_flCapPointScoreRate; // 0x2c34 ( Size: 4 )
        float m_flConnectionTime; // 0x2c38 ( Size: 4 )
        float m_fullyConnectedTime; // 0x2c3c ( Size: 4 )
        float m_connectedForDurationCallback_duration; // 0x2c40 ( Size: 4 )
        float m_flLastForcedChangeTeamTime; // 0x2c44 ( Size: 4 )
        int m_iBalanceScore; // 0x2c48 ( Size: 4 )
        char gap_2c4c[4];
        __int64 m_PlayerAnimState; // 0x2c50 ( Size: 8 )
        Vector3 m_vWorldSpaceCenterHolder; // 0x2c58 ( Size: 12 )
        Vector3 m_vPrevGroundNormal; // 0x2c64 ( Size: 12 )
        int m_threadedPostProcessJob; // 0x2c70 ( Size: 4 )
        char gap_2c74[4];
        CPlayerShared m_Shared; // 0x2c78 ( Size: 160 )
        StatusEffectTimedData m_statusEffectsTimedPlayerNV[10]; // 0x2d18 ( Size: 24 )
        StatusEffectEndlessData m_statusEffectsEndlessPlayerNV[10]; // 0x2e08 ( Size: 16 )
        int m_pilotClassIndex; // 0x2ea8 ( Size: 4 )
        int m_latestCommandRun; // 0x2eac ( Size: 4 )
        char m_nearbyPushers[480]; // 0x2eb0 ( Size: 480 ) // void
        int m_nearbyPusherCount; // 0x3090 ( Size: 4 )
        PushHistoryEntry m_pushHistory[16]; // 0x3094 ( Size: 16 )
        int m_pushHistoryEntryIndex; // 0x3194 ( Size: 4 )
        float m_baseVelocityLastServerTime; // 0x3198 ( Size: 4 )
        Vector3 m_pushedThisFrame; // 0x319c ( Size: 12 )
        Vector3 m_pushedThisSnapshotAccum; // 0x31a8 ( Size: 12 )
        int m_pushedFixedPointOffset[3]; // 0x31b4 ( Size: 12 )
        float m_lastCommandContextWarnTime; // 0x31c0 ( Size: 4 )
        Vector3 m_pushAwayFromTopAcceleration; // 0x31c4 ( Size: 12 )
        float m_trackedState[52]; // 0x31d0 ( Size: 208 )
        int m_prevTrackedState; // 0x32a0 ( Size: 4 )
        Vector3 m_prevTrackedStatePos; // 0x32a4 ( Size: 12 )
        __int64 m_recordingAnim; // 0x32b0 ( Size: 8 ) // void
        __int64 m_animRecordFile; // 0x32b8 ( Size: 8 ) // void
        int m_animRecordButtons; // 0x32c0 ( Size: 4 ) //void
        Vector3 m_prevAbsOrigin; // 0x32c4 ( Size: 12 )
        bool m_sendMovementCallbacks; // 0x32d0 ( Size: 1 )
        bool m_sendInputCallbacks; // 0x32d1 ( Size: 1 )
        char gap_32d2[2];
        PredictableServerEvent m_predictableServerEvents[16]; // 0x32d4 ( Size: 24 )
        int m_predictableServerEventCount; // 0x3454 ( Size: 4 )
        int m_predictableServerEventAcked; // 0x3458 ( Size: 4 )
        int /*EHANDLE*/ m_playerScriptNetDataGlobal; // 0x345c ( Size: 4 )
        int /*EHANDLE*/ m_playerScriptNetDataExclusive; // 0x3460 ( Size: 4 )
    };
    static_assert(sizeof(CPlayer) == 0x3468);
} // namespace R2
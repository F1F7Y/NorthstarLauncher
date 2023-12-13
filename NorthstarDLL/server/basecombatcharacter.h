#pragma once

#include "baseanimatingoverlay.h"

class WeaponDropInfo
{
  public:
	Vector3 weaponPosition; // 0x0 ( Size: 12 )
	char prevDropFrameCounter; // 0xc ( Size: 1 )
	char dropFrameCounter; // 0xd ( Size: 1 )
	char gap_e[2];
	Vector3 weaponAngles; // 0x10 ( Size: 12 )
	float weaponPositionTime; // 0x1c ( Size: 4 )
};
static_assert(sizeof(WeaponDropInfo) == 32);

class WeaponInventory
{
	void* vftable;

  public:
	int /*EHANDLE*/ weapons[4]; // 0x8 ( Size: 16 )
	int /*EHANDLE*/ activeWeapon; // 0x18 ( Size: 4 )
	int /*EHANDLE*/ offhandWeapons[6]; // 0x1c ( Size: 24 )
};
static_assert(sizeof(WeaponInventory) == 56);

class CTether
{
	void* vftable;

  public:
	Vector3 pos; // 0x8 ( Size: 12 )
	float health; // 0x14 ( Size: 4 )
	float nextSoundTime; // 0x18 ( Size: 4 )
	float creationTime; // 0x1c ( Size: 4 )
	int scriptID; // 0x20 ( Size: 4 )
};
static_assert(sizeof(CTether) == 40);

class CBaseCombatCharacter : public CBaseAnimatingOverlay
{
	bool m_bPreventWeaponPickup; // 0x1210 ( Size: 1 )
	char gap_1211[3];
	float m_phaseShiftTimeStart; // 0x1214 ( Size: 4 )
	float m_phaseShiftTimeEnd; // 0x1218 ( Size: 4 )
	float m_flNextAttack; // 0x121c ( Size: 4 )
	float m_lastFiredTime; // 0x1220 ( Size: 4 )
	float m_raiseFromMeleeEndTime; // 0x1224 ( Size: 4 )
	int m_sharedEnergyCount; // 0x1228 ( Size: 4 )
	int m_sharedEnergyTotal; // 0x122c ( Size: 4 )
	float m_lastSharedEnergyRegenTime; // 0x1230 ( Size: 4 )
	float m_sharedEnergyRegenRate; // 0x1234 ( Size: 4 )
	float m_sharedEnergyRegenDelay; // 0x1238 ( Size: 4 )
	float m_lastSharedEnergyTakeTime; // 0x123c ( Size: 4 )
	int m_eHull; // 0x1240 ( Size: 4 )
	float m_fieldOfViewCos; // 0x1244 ( Size: 4 )
	Vector3 m_HackedGunPos; // 0x1248 ( Size: 12 )
	float m_impactEnergyScale; // 0x1254 ( Size: 4 )
	WeaponDropInfo m_weaponDropInfo; // 0x1258 ( Size: 32 )
	float m_physicsMass; // 0x1278 ( Size: 4 )
	float m_flDamageAccumulator; // 0x127c ( Size: 4 )
	int m_prevHealth; // 0x1280 ( Size: 4 )
	float m_healthChangeRate; // 0x1284 ( Size: 4 )
	float m_healthChangeAmount; // 0x1288 ( Size: 4 )
	float m_healthChangeStartTime; // 0x128c ( Size: 4 )
	float m_lastHealthChangeTime; // 0x1290 ( Size: 4 )
	int m_lastHitGroup; // 0x1294 ( Size: 4 )
	Vector3 m_lastDamageDir; // 0x1298 ( Size: 12 )
	Vector3 m_lastDamageForce; // 0x12a4 ( Size: 12 )
	int m_deathPackage; // 0x12b0 ( Size: 4 )
	Vector3 m_deathDirection2DInverse; // 0x12b4 ( Size: 12 )
	int m_CurrentWeaponProficiency; // 0x12c0 ( Size: 4 )
	float m_flEnemyAccurcyMultiplier; // 0x12c4 ( Size: 4 )
	int m_npcPriorityOverride; // 0x12c8 ( Size: 4 )
	int m_healthPerSegment; // 0x12cc ( Size: 4 )
	char m_hTriggerFogList[32]; // 0x12d0 ( Size: 32 ) // custom
	int /*EHANDLE*/ m_hLastFogTrigger; // 0x12f0 ( Size: 4 )
	char gap_12f4[4];
	WeaponInventory m_inventory; // 0x12f8 ( Size: 56 )
	int /*EHANDLE*/ m_selectedWeapon; // 0x1330 ( Size: 4 )
	int /*EHANDLE*/ m_latestPrimaryWeapon; // 0x1334 ( Size: 4 )
	int /*EHANDLE*/ m_latestNonOffhandWeapon; // 0x1338 ( Size: 4 )
	int m_lastCycleSlot; // 0x133c ( Size: 4 )
	int /*EHANDLE*/ m_removeWeaponOnSelectSwitch; // 0x1340 ( Size: 4 )
	int /*EHANDLE*/ m_weaponGettingSwitchedOut; // 0x1344 ( Size: 4 )
	bool m_showNewWeapon3p; // 0x1348 ( Size: 1 )
	char gap_1349[3];
	int m_weaponPermission; // 0x134c ( Size: 4 )
	bool m_weaponDisabled; // 0x1350 ( Size: 1 )
	bool m_hudInfo_visibilityTestAlwaysPasses; // 0x1351 ( Size: 1 )
	char gap_1352[2];
	int /*EHANDLE*/ m_selectedOffhand; // 0x1354 ( Size: 4 )
	int m_selectedOffhandPendingHybridAction; // 0x1358 ( Size: 4 )
	bool m_doOffhandAnim; // 0x135c ( Size: 1 )
	bool m_wantInventoryChangedScriptCall; // 0x135d ( Size: 1 )
	bool m_doInventoryChangedScriptCall; // 0x135e ( Size: 1 )
	char gap_135f[1];
	float m_cloakReactEndTime; // 0x1360 ( Size: 4 )
	char gap_1364[4];
	CTether m_tethers[2]; // 0x1368 ( Size: 40 )
	int /*EHANDLE*/ m_titanSoul; // 0x13b8 ( Size: 4 )
	Vector3 m_lastFootstepDamagePos; // 0x13bc ( Size: 12 )
	int m_muzzleAttachment[2]; // 0x13c8 ( Size: 8 )
	int m_prevNearestNode; // 0x13d0 ( Size: 4 )
	int m_nearestNode; // 0x13d4 ( Size: 4 )
	float m_nearestNodeCheckTime; // 0x13d8 ( Size: 4 )
	Vector3 m_nearestNodeCheckPos; // 0x13dc ( Size: 12 )
	int m_nearestPolyRef[4]; // 0x13e8 ( Size: 16 )
	Vector3 m_nearestPolyCheckPos[4]; // 0x13f8 ( Size: 48 )
	Vector3 m_meleeExecutionUnstuckPosition; // 0x1428 ( Size: 12 )
	int /*EHANDLE*/ m_meleeExecutionEntityBlocker; // 0x1434 ( Size: 4 )
	int m_contextAction; // 0x1438 ( Size: 4 )
	char m_targetInfoIconName[64]; // 0x143c ( Size: 64 )
	int /*EHANDLE*/ m_rodeoRiders[5]; // 0x147c ( Size: 20 )
	int m_numRodeoSlots; // 0x1490 ( Size: 4 )
};
static_assert(sizeof(CBaseCombatCharacter) == 0x1498);

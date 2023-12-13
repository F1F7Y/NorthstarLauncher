#pragma once

#include "core/math/color.h"
#include "core/math/vector.h"

typedef uint16_t edict_t;

class CBaseEntity;

class IServerNetworkable
{
	void* vftable;
};

class CServerNetworkProperty : IServerNetworkable
{
  public:
	CBaseEntity* m_pOuter;
	void* m_pServerClass;
	edict_t m_edict;
	int /*EHANDLE*/ m_hParent;
	char unk[8];
};
static_assert(sizeof(CServerNetworkProperty) == 40);

class CCollisionProperty
{
	void* vftable;

  public:
	CBaseEntity* m_pOuter; // 0x8 ( Size: 8 )
	Vector3 m_vecMins; // 0x10 ( Size: 12 )
	Vector3 m_vecMaxs; // 0x1c ( Size: 12 )
	int m_usSolidFlags; // 0x28 ( Size: 4 )
	char m_nSolidType; // 0x2c ( Size: 1 )
	char m_triggerBloat; // 0x2d ( Size: 1 )
	char gap_2e[2];
	float m_flRadius; // 0x30 ( Size: 4 )
	short m_PartitionHandle; // 0x34 ( Size: 2 )
	char m_nSurroundType; // 0x36 ( Size: 1 )
	bool m_bRemovedFromPartition; // 0x37 ( Size: 1 )
	int m_spatialPartitionFlags; // 0x38 ( Size: 4 )
	Vector3 m_vecSpecifiedSurroundingMins; // 0x3c ( Size: 12 )
	Vector3 m_vecSpecifiedSurroundingMaxs; // 0x48 ( Size: 12 )
	Vector3 m_vecSurroundingMins; // 0x54 ( Size: 12 )
	Vector3 m_vecSurroundingMaxs; // 0x60 ( Size: 12 )
	float m_hitboxTestRadius; // 0x6c ( Size: 4 )
};
static_assert(sizeof(CCollisionProperty) == 112);

class CBaseEntity // : public IServerEntity
{
  public:
	void* vftable;

	uint32_t m_RefEHandle; // 0x8 ( Size: 4 ) // handle
	char gap_c[4];
	void* m_collideable; // 0x10 ( Size: 8 )
	void* m_networkable; // 0x18 ( Size: 8 )
	int genericKeyValueCount; // 0x20 ( Size: 4 )
	char gap_24[4];
	void* genericKeyValues; // 0x28 ( Size: 8 )
	void* m_pfnMoveDone; // 0x30 ( Size: 8 )
	void* m_pfnThink; // 0x38 ( Size: 8 )
	CServerNetworkProperty m_Network; // 0x40 ( Size: 40 )
	int m_entIndex; // 0x68 ( Size: 4 )
	char gap_6c[4];
	__int64 m_iClassname; // 0x70 ( Size: 8 )
	float m_flAnimTime; // 0x78 ( Size: 4 )
	float m_flSimulationTime; // 0x7c ( Size: 4 )
	int m_creationTick; // 0x80 ( Size: 4 )
	int m_nLastThinkTick; // 0x84 ( Size: 4 )
	int m_PredictableID; // 0x88 ( Size: 4 )
	int touchStamp; // 0x8c ( Size: 4 )
	char /*CUtlVector*/ m_aThinkFunctions[32]; // 0x90 ( Size: 32 ) CUtlVector
	float m_entitySpawnTime; // 0xb0 ( Size: 4 )
	int /*EHANDLE*/ m_spawner; // 0xb4 ( Size: 4 )
	int m_scriptClass; // 0xb8 ( Size: 4 )
	bool m_wantsDamageCallbacks; // 0xbc ( Size: 1 )
	bool m_wantsDeathCallbacks; // 0xbd ( Size: 1 )
	char gap_be[2];
	int m_nNextThinkTick; // 0xc0 ( Size: 4 )
	int m_fEffects; // 0xc4 ( Size: 4 )
	__int64 m_ModelName; // 0xc8 ( Size: 8 )
	__int64 m_target; // 0xd0 ( Size: 8 )
	int m_networkedFlags; // 0xd8 ( Size: 4 )
	char m_nRenderFX; // 0xdc ( Size: 1 )
	char m_nRenderMode; // 0xdd ( Size: 1 )
	short m_nModelIndex; // 0xde ( Size: 2 )
	Color m_clrRender; // 0xe0 ( Size: 4 )
	int m_desiredHibernationType; // 0xe4 ( Size: 4 )
	int m_scriptMinHibernationType; // 0xe8 ( Size: 4 )
	int m_minSelfAndDescendantHibernationType; // 0xec ( Size: 4 )
	int m_actualHibernationType; // 0xf0 ( Size: 4 )
	int m_hibernationQueueIndex; // 0xf4 ( Size: 4 )
	bool m_bRenderWithViewModels; // 0xf8 ( Size: 1 )
	char gap_f9[3];
	int m_nameVisibilityFlags; // 0xfc ( Size: 4 )
	float m_cloakEndTime; // 0x100 ( Size: 4 )
	float m_cloakFadeInEndTime; // 0x104 ( Size: 4 )
	float m_cloakFadeOutStartTime; // 0x108 ( Size: 4 )
	float m_cloakFadeInDuration; // 0x10c ( Size: 4 )
	float m_cloakFlickerAmount; // 0x110 ( Size: 4 )
	float m_cloakFlickerEndTime; // 0x114 ( Size: 4 )
	float m_cloakFadeOutDuration; // 0x118 ( Size: 4 )
	bool m_highlightIsNetworked; // 0x11c ( Size: 1 )
	char gap_11d[3];
	Vector3 m_highlightParams[16]; // 0x120 ( Size: 192 )
	int m_highlightFunctionBits[8]; // 0x1e0 ( Size: 32 )
	int m_highlightPlayerVisibilityBits[8]; // 0x200 ( Size: 32 )
	float m_highlightServerFadeBases[2]; // 0x220 ( Size: 8 )
	float m_highlightServerFadeStartTimes[2]; // 0x228 ( Size: 8 )
	float m_highlightServerFadeEndTimes[2]; // 0x230 ( Size: 8 )
	int m_highlightServerContextID; // 0x238 ( Size: 4 )
	int m_highlightTeamBits; // 0x23c ( Size: 4 )
	float m_nextGrenadeTargetTime; // 0x240 ( Size: 4 )
	float m_grenadeTargetDebounce; // 0x244 ( Size: 4 )
	int m_nSimulationTick; // 0x248 ( Size: 4 )
	int m_fDataObjectTypes; // 0x24c ( Size: 4 )
	float m_nextVelocitySample; // 0x250 ( Size: 4 )
	Vector3 m_velocitySamples[5]; // 0x254 ( Size: 60 )
	__int64 m_iEFlags; // 0x290 ( Size: 8 )
	int m_fFlags; // 0x298 ( Size: 4 )
	char gap_29c[4];
	__int64 m_iName; // 0x2a0 ( Size: 8 )
	int m_scriptNameIndex; // 0x2a8 ( Size: 4 )
	int m_instanceNameIndex; // 0x2ac ( Size: 4 )
	char m_scriptName[64]; // 0x2b0 ( Size: 64 )
	char m_instanceName[64]; // 0x2f0 ( Size: 64 )
	__int64 m_holdUsePrompt; // 0x330 ( Size: 8 )
	__int64 m_pressUsePrompt; // 0x338 ( Size: 8 )
	float m_attachmentLerpStartTime; // 0x340 ( Size: 4 )
	float m_attachmentLerpEndTime; // 0x344 ( Size: 4 )
	Vector3 m_attachmentLerpStartOrigin; // 0x348 ( Size: 12 )
	Vector3 m_attachmentLerpStartAngles; // 0x354 ( Size: 12 )
	int m_parentAttachmentType; // 0x360 ( Size: 4 )
	int m_parentAttachmentIndex; // 0x364 ( Size: 4 )
	int m_parentAttachmentHitbox; // 0x368 ( Size: 4 )
	int m_parentAttachmentModel; // 0x36c ( Size: 4 )
	char m_MoveType; // 0x370 ( Size: 1 )
	char m_MoveCollide; // 0x371 ( Size: 1 )
	char gap_372[2];
	int m_RestoreMoveTypeOnDetach; // 0x374 ( Size: 4 )
	int /*EHANDLE*/ m_hMoveParent; // 0x378 ( Size: 4 )
	int /*EHANDLE*/ m_hMoveChild; // 0x37c ( Size: 4 )
	int /*EHANDLE*/ m_hMovePeer; // 0x380 ( Size: 4 )
	bool m_bIsActiveChild; // 0x384 ( Size: 1 )
	bool m_bPrevAbsOriginValid; // 0x385 ( Size: 1 )
	char gap_386[2];
	CCollisionProperty m_Collision; // 0x388 ( Size: 112 )
	int /*EHANDLE*/ m_hOwnerEntity; // 0x3f8 ( Size: 4 )
	int m_CollisionGroup; // 0x3fc ( Size: 4 )
	int m_contents; // 0x400 ( Size: 4 )
	int m_baseSolidType; // 0x404 ( Size: 4 )
	void* m_pPhysicsObject; // 0x408 ( Size: 8 )
	float m_flNavIgnoreUntilTime; // 0x410 ( Size: 4 )
	int /*EHANDLE*/ m_hGroundEntity; // 0x414 ( Size: 4 )
	float m_flGroundChangeTime; // 0x418 ( Size: 4 )
	Vector3 m_vecBaseVelocity; // 0x41c ( Size: 12 )
	Vector3 m_vecAbsVelocity; // 0x428 ( Size: 12 )
	Vector3 m_vecAngVelocity; // 0x434 ( Size: 12 )
	float m_rgflCoordinateFrame[12]; // 0x440 ( Size: 48 )
	float m_flFriction; // 0x470 ( Size: 4 )
	float m_flLocalTime; // 0x474 ( Size: 4 )
	float m_flVPhysicsUpdateLocalTime; // 0x478 ( Size: 4 )
	float m_flMoveDoneTime; // 0x47c ( Size: 4 )
	int m_nPushEnumCount; // 0x480 ( Size: 4 )
	Vector3 m_vecPrevAbsOrigin; // 0x484 ( Size: 12 )
	Vector3 m_vecAbsOrigin; // 0x490 ( Size: 12 )
	Vector3 m_angAbsRotation; // 0x49c ( Size: 12 )
	Vector3 m_vecVelocity; // 0x4a8 ( Size: 12 )
	int /*EHANDLE*/ m_pBlocker; // 0x4b4 ( Size: 4 )
	__int64 m_iGlobalname; // 0x4b8 ( Size: 8 )
	__int64 m_iParent; // 0x4c0 ( Size: 8 )
	int m_iHammerID; // 0x4c8 ( Size: 4 )
	float m_flSpeed; // 0x4cc ( Size: 4 )
	int m_iMaxHealth; // 0x4d0 ( Size: 4 )
	int m_iHealth; // 0x4d4 ( Size: 4 )
	void* m_pfnTouch; // 0x4d8 ( Size: 8 )
	void* m_pfnUse; // 0x4e0 ( Size: 8 )
	void* m_pfnBlocked; // 0x4e8 ( Size: 8 )
	bool m_bClientSideRagdoll; // 0x4f0 ( Size: 1 )
	char m_lifeState; // 0x4f1 ( Size: 1 )
	bool m_forceVisibleInPhaseShift; // 0x4f2 ( Size: 1 )
	char m_baseTakeDamage; // 0x4f3 ( Size: 1 )
	int m_invulnerableToDamageCount; // 0x4f4 ( Size: 4 )
	char m_passDamageToParent; // 0x4f8 ( Size: 1 )
	char gap_4f9[3];
	Vector3 m_deathVelocity; // 0x4fc ( Size: 12 )
	float m_lastTitanFootstepDamageTime; // 0x508 ( Size: 4 )
	float m_flMaxspeed; // 0x50c ( Size: 4 )
	int m_visibilityFlags; // 0x510 ( Size: 4 )
	bool m_scriptVisible; // 0x514 ( Size: 1 )
	char gap_515[3];
	char m_OnUser1[40]; // 0x518 ( Size: 40 ) // custom
	char m_OnDeath[40]; // 0x540 ( Size: 40 ) // custom
	char m_OnDestroy[40]; // 0x568 ( Size: 40 ) // custom
	int m_cellwidth; // 0x590 ( Size: 4 )
	int m_cellbits; // 0x594 ( Size: 4 )
	int m_cellX; // 0x598 ( Size: 4 )
	int m_cellY; // 0x59c ( Size: 4 )
	int m_cellZ; // 0x5a0 ( Size: 4 )
	Vector3 m_localOrigin; // 0x5a4 ( Size: 12 )
	Vector3 m_localAngles; // 0x5b0 ( Size: 12 )
	Vector3 m_vecViewOffset; // 0x5bc ( Size: 12 )
	int m_ListByClass; // 0x5c8 ( Size: 4 )
	char gap_5cc[4];
	void* m_pPrevByClass; // 0x5d0 ( Size: 8 )
	void* m_pNextByClass; // 0x5d8 ( Size: 8 )
	int m_iInitialTeamNum; // 0x5e0 ( Size: 4 )
	int m_iTeamNum; // 0x5e4 ( Size: 4 )
	int m_passThroughFlags; // 0x5e8 ( Size: 4 )
	int m_passThroughThickness; // 0x5ec ( Size: 4 )
	float m_passThroughDirection; // 0x5f0 ( Size: 4 )
	int m_spawnflags; // 0x5f4 ( Size: 4 )
	__int64 m_AIAddOn; // 0x5f8 ( Size: 8 )
	float m_flGravity; // 0x600 ( Size: 4 )
	float m_entityFadeDist; // 0x604 ( Size: 4 )
	int /*EHANDLE*/ m_dissolveEffectEntityHandle; // 0x608 ( Size: 4 )
	float m_fadeDist; // 0x60c ( Size: 4 )
	__int64 m_iSignifierName; // 0x610 ( Size: 8 )
	int m_collectedInvalidateFlags; // 0x618 ( Size: 4 )
	bool m_collectingInvalidateFlags; // 0x61c ( Size: 1 )
	char gap_61d[3];
	int m_lagCompensationCounter; // 0x620 ( Size: 4 )
	bool m_bLagCompensate; // 0x624 ( Size: 1 )
	bool m_bNetworkQuantizeOriginAndAngles; // 0x625 ( Size: 1 )
	bool m_bForcePurgeFixedupStrings; // 0x626 ( Size: 1 )
	char gap_627[1];
	void* m_pEvent; // 0x628 ( Size: 8 )
	int m_debugOverlays; // 0x630 ( Size: 4 )
	char gap_634[4];
	void* m_pTimedOverlay; // 0x638 ( Size: 8 )
	char m_ScriptScope[32]; // 0x640 ( Size: 32 ) // void
	__int64 m_hScriptInstance; // 0x660 ( Size: 8 ) // void
	__int64 m_iszScriptId; // 0x668 ( Size: 8 )
	int /*EHANDLE*/ m_bossPlayer; // 0x670 ( Size: 4 )
	int m_usableType; // 0x674 ( Size: 4 )
	int m_usablePriority; // 0x678 ( Size: 4 )
	float m_usableRadius; // 0x67c ( Size: 4 )
	float m_usableFOV; // 0x680 ( Size: 4 )
	float m_usePromptSize; // 0x684 ( Size: 4 )
	bool m_hasDispatchedSpawn; // 0x688 ( Size: 1 )
	bool m_bDoDestroyCallback; // 0x689 ( Size: 1 )
	bool m_bDoPreSpawnCallback; // 0x68a ( Size: 1 )
	bool m_bDoOnSpawnedCallback; // 0x68b ( Size: 1 )
	float m_spottedBeginTimes[31]; // 0x68c ( Size: 124 )
	float m_spottedLatestTimes[31]; // 0x708 ( Size: 124 )
	int m_spottedByTeams; // 0x784 ( Size: 4 )
	char m_minimapData[40]; // 0x788 ( Size: 40 ) // void
	int m_shieldHealth; // 0x7b0 ( Size: 4 )
	int m_shieldHealthMax; // 0x7b4 ( Size: 4 )
	bool m_areEntityLinksNetworked; // 0x7b8 ( Size: 1 )
	char gap_7b9[3];
	int m_entitiesLinkedFromMeCount; // 0x7bc ( Size: 4 )
	int m_entitiesLinkedToMeCount; // 0x7c0 ( Size: 4 )
	int /*EHANDLE*/ m_entitiesLinkedFromMe[64]; // 0x7c4 ( Size: 256 )
	int /*EHANDLE*/ m_entitiesLinkedToMe[64]; // 0x8c4 ( Size: 256 )
	float m_pusherWithChildrenRadius; // 0x9c4 ( Size: 4 )
	int m_childPusherMoveHandlerCount; // 0x9c8 ( Size: 4 )
	char gap_9cc[4];
	CBaseEntity* m_statusEffectPlugin; // 0x9d0 ( Size: 8 )
	char gap_9d8[1];
	bool m_physDummyMotionEnabled; // 0x9d9 ( Size: 1 )
};
static_assert(sizeof(CBaseEntity) == 0x9E0);

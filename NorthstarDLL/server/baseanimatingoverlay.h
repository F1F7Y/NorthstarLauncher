#pragma once

#include "baseanimating.h"

class CAnimationLayer
{
  public:
	bool m_bSequenceFinished; // 0x0 ( Size: 1 )
	char gap_1[3];
	int m_fFlags; // 0x4 ( Size: 4 )
	int m_layerIndex; // 0x8 ( Size: 4 )
	int m_modelIndex; // 0xc ( Size: 4 )
	float m_flKillRate; // 0x10 ( Size: 4 )
	float m_flKillDelay; // 0x14 ( Size: 4 )
	int m_nActivity; // 0x18 ( Size: 4 )
	int m_nPriority; // 0x1c ( Size: 4 )
	float m_flLastEventCheck; // 0x20 ( Size: 4 )
	char gap_24[4];
	CBaseEntity* m_animationLayerOwner; // 0x28 ( Size: 8 )
};
static_assert(sizeof(CAnimationLayer) == 48);

class CBaseAnimatingOverlay : public CBaseAnimating
{
  public:
	int m_maxOverlays; // 0xeb8 ( Size: 4 )
	char gap_ebc[4];
	CAnimationLayer m_AnimOverlay[8]; // 0xec0 ( Size: 48 )
	int m_AnimOverlayCount; // 0x1040 ( Size: 4 )
	bool m_animOverlayIsActive[8]; // 0x1044 ( Size: 8 )
	int m_animOverlayModelIndex[8]; // 0x104c ( Size: 32 )
	int m_animOverlaySequence[8]; // 0x106c ( Size: 32 )
	float m_animOverlayCycle[8]; // 0x108c ( Size: 32 )
	float m_animOverlayStartTime[8]; // 0x10ac ( Size: 32 )
	float m_animOverlayStartCycle[8]; // 0x10cc ( Size: 32 )
	float m_animOverlayPlaybackRate[8]; // 0x10ec ( Size: 32 )
	float m_animOverlayWeight[8]; // 0x110c ( Size: 32 )
	int m_animOverlayOrder[8]; // 0x112c ( Size: 32 )
	float m_animOverlayAnimTime[8]; // 0x114c ( Size: 32 )
	float m_animOverlayFadeInDuration[8]; // 0x116c ( Size: 32 )
	float m_animOverlayFadeOutDuration[8]; // 0x118c ( Size: 32 )
	bool m_localAnimOverlayIsActive[4]; // 0x11ac ( Size: 4 )
	int m_localAnimOverlayModelIndex[4]; // 0x11b0 ( Size: 16 )
	int m_localAnimOverlaySequence[4]; // 0x11c0 ( Size: 16 )
	float m_localAnimOverlayStartTime[4]; // 0x11d0 ( Size: 16 )
	float m_localAnimOverlayWeight[4]; // 0x11e0 ( Size: 16 )
	float m_localAnimOverlayFadeInDuration[4]; // 0x11f0 ( Size: 16 )
	float m_localAnimOverlayFadeOutDuration[4]; // 0x1200 ( Size: 16 )
};
static_assert(sizeof(CBaseAnimatingOverlay) == 0x1210);
// 1360

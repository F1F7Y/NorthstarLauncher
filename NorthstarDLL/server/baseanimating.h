#pragma once

#include "baseentity.h"
#include "core/math/matrix.h"
#include "core/math/quaternion.h"

namespace R2
{
    class AnimRelativeData
    {
        void* vftable;

    public:
        Vector3 m_animInitialPos; // 0x8 ( Size: 12 )
        Vector3 m_animInitialVel; // 0x14 ( Size: 12 )
        Quaternion m_animInitialRot; // 0x20 ( Size: 16 )
        Vector3 m_animInitialCorrectPos; // 0x30 ( Size: 12 )
        Quaternion m_animInitialCorrectRot; // 0x3c ( Size: 16 )
        Vector3 m_animEntityToRefOffset; // 0x4c ( Size: 12 )
        Quaternion m_animEntityToRefRotation; // 0x58 ( Size: 16 )
        float m_animBeginTime; // 0x68 ( Size: 4 )
        float m_animEndTime; // 0x6c ( Size: 4 )
        int m_animScriptSequence; // 0x70 ( Size: 4 )
        int m_animScriptModel; // 0x74 ( Size: 4 )
        bool m_animIgnoreParentRot; // 0x78 ( Size: 1 )
        char gap_79[3];
        int m_animMotionMode; // 0x7c ( Size: 4 )
    };
    static_assert(sizeof(AnimRelativeData) == 128);

    class PredictedAnimEventData
    {
        void* vftable;

    public:
        float m_predictedAnimEventTimes[8]; // 0x8 ( Size: 32 )
        int m_predictedAnimEventIndices[8]; // 0x28 ( Size: 32 )
        int m_predictedAnimEventCount; // 0x48 ( Size: 4 )
        int /*EHANDLE*/ m_predictedAnimEventTarget; // 0x4c ( Size: 4 )
        int m_predictedAnimEventSequence; // 0x50 ( Size: 4 )
        int m_predictedAnimEventModel; // 0x54 ( Size: 4 )
        float m_predictedAnimEventsReadyToFireTime; // 0x58 ( Size: 4 )
    };
    static_assert(sizeof(PredictedAnimEventData) == 96);

    class CBaseAnimating : public CBaseEntity
    {
    public:
        bool m_bCanUseFastPathFromServer; // 0x9e0 ( Size: 1 )
        char gap_9e1[3];
        float m_flGroundSpeed; // 0x9e4 ( Size: 4 )
        float m_flLastEventCheck; // 0x9e8 ( Size: 4 )
        int m_nForceBone; // 0x9ec ( Size: 4 )
        Vector3 m_vecForce; // 0x9f0 ( Size: 12 )
        int m_nSkin; // 0x9fc ( Size: 4 )
        int m_nBody; // 0xa00 ( Size: 4 )
        int m_camoIndex; // 0xa04 ( Size: 4 )
        int m_decalIndex; // 0xa08 ( Size: 4 )
        int m_nHitboxSet; // 0xa0c ( Size: 4 )
        float m_flModelScale; // 0xa10 ( Size: 4 )
        int m_nRagdollImpactFXTableId; // 0xa14 ( Size: 4 )
        float m_flSkyScaleStartTime; // 0xa18 ( Size: 4 )
        float m_flSkyScaleEndTime; // 0xa1c ( Size: 4 )
        float m_flSkyScaleStartValue; // 0xa20 ( Size: 4 )
        float m_flSkyScaleEndValue; // 0xa24 ( Size: 4 )
        char m_SequenceTransitioner[376]; // 0xa28 ( Size: 376 ) // void
        float m_flIKGroundContactTime; // 0xba0 ( Size: 4 )
        float m_flIKGroundMinHeight; // 0xba4 ( Size: 4 )
        float m_flIKGroundMaxHeight; // 0xba8 ( Size: 4 )
        float m_flEstIkFloor; // 0xbac ( Size: 4 )
        float m_flEstIkOffset; // 0xbb0 ( Size: 4 )
        char gap_bb4[4];
        void* m_pIk; // 0xbb8 ( Size: 8 )
        int m_ikPrevSequence; // 0xbc0 ( Size: 4 )
        bool m_bSequenceFinished; // 0xbc4 ( Size: 1 )
        bool m_bSequenceLooped; // 0xbc5 ( Size: 1 )
        bool m_bSequenceLoops; // 0xbc6 ( Size: 1 )
        char gap_bc7[1];
        int m_numSequenceLoops; // 0xbc8 ( Size: 4 )
        bool m_continueAnimatingAfterRagdoll; // 0xbcc ( Size: 1 )
        bool m_useLockedAnimDeltaYaw; // 0xbcd ( Size: 1 )
        char gap_bce[2];
        float m_lockedAnimDeltaYaw; // 0xbd0 ( Size: 4 )
        bool m_threadedBoneSetup; // 0xbd4 ( Size: 1 )
        bool m_settingUpBones; // 0xbd5 ( Size: 1 )
        char gap_bd6[2];
        float m_flDissolveStartTime; // 0xbd8 ( Size: 4 )
        int m_baseAnimatingActivity; // 0xbdc ( Size: 4 )
        float m_flPoseParameter[11]; // 0xbe0 ( Size: 44 )
        bool m_poseParameterOverTimeActive; // 0xc0c ( Size: 1 )
        char gap_c0d[3];
        float m_poseParameterGoalValue[11]; // 0xc10 ( Size: 44 )
        float m_poseParameterEndTime[11]; // 0xc3c ( Size: 44 )
        float m_lastTimeSetPoseParametersSameAs; // 0xc68 ( Size: 4 )
        bool m_bClientSideAnimation; // 0xc6c ( Size: 1 )
        bool m_bReallyClientSideAnimation; // 0xc6d ( Size: 1 )
        char gap_c6e[2];
        int m_nNewSequenceParity; // 0xc70 ( Size: 4 )
        int m_nResetEventsParity; // 0xc74 ( Size: 4 )
        __int64 m_boneCacheHandle; // 0xc78 ( Size: 8 )
        short m_fBoneCacheFlags; // 0xc80 ( Size: 2 )
        char gap_c82[2];
        int m_animNetworkFlags; // 0xc84 ( Size: 4 )
        bool m_animActive; // 0xc88 ( Size: 1 )
        bool m_animCollisionEnabled; // 0xc89 ( Size: 1 )
        bool m_animInitialCorrection; // 0xc8a ( Size: 1 )
        bool m_animWaitingForCleanup; // 0xc8b ( Size: 1 )
        int m_animWaitingForCleanupTime; // 0xc8c ( Size: 4 )
        __int64 m_recordedAnim; // 0xc90 ( Size: 8 )
        int m_recordedAnimIndex; // 0xc98 ( Size: 4 )
        int m_recordedAnimCachedFrameIndex; // 0xc9c ( Size: 4 )
        float m_recordedAnimPlaybackRate; // 0xca0 ( Size: 4 )
        float m_recordedAnimPlaybackTime; // 0xca4 ( Size: 4 )
        matrix3x4_t m_recordedAnimTransform; // 0xca8 ( Size: 48 )
        int /*EHANDLE*/ m_recordedAnimPlaybackEnt; // 0xcd8 ( Size: 4 )
        float m_recordedAnimBlendTime; // 0xcdc ( Size: 4 )
        Vector3 m_recordedAnimBlendOffset; // 0xce0 ( Size: 12 )
        Vector3 m_recordedAnimBlendAngles; // 0xcec ( Size: 12 )
        AnimRelativeData m_animRelativeData; // 0xcf8 ( Size: 128 )
        int /*EHANDLE*/ m_syncingWithEntity; // 0xd78 ( Size: 4 )
        char gap_d7c[4];
        PredictedAnimEventData m_predictedAnimEventData; // 0xd80 ( Size: 96 )
        int m_animRefEntityAttachmentIndex; // 0xde0 ( Size: 4 )
        int m_fireAttachmentSmartAmmoIndex; // 0xde4 ( Size: 4 )
        int m_fireAttachmentChestFocusIndex; // 0xde8 ( Size: 4 )
        int m_fireAttachmentModelIndex; // 0xdec ( Size: 4 )
        char m_keyHitboxes[160]; // 0xdf0 ( Size: 160 ) // void
        void* m_pStudioHdr; // 0xe90 ( Size: 8 )
        int m_animSequence; // 0xe98 ( Size: 4 )
        float m_animCycle; // 0xe9c ( Size: 4 )
        int m_animModelIndex; // 0xea0 ( Size: 4 )
        float m_animStartTime; // 0xea4 ( Size: 4 )
        float m_animStartCycle; // 0xea8 ( Size: 4 )
        float m_animPlaybackRate; // 0xeac ( Size: 4 )
        bool m_animFrozen; // 0xeb0 ( Size: 1 )
    };
    static_assert(sizeof(CBaseAnimating) == 0xeb8);
} // namespace R2

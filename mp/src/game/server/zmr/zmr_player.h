#pragma once


#include "hl2/hl2_player.h"

#include "zmr/zmr_player_shared.h"
#include "zmr/zmr_entities.h"
#include "zmr/weapons/zmr_base.h"
#include "zmr/zmr_shareddefs.h"
#include "zmr/zmr_playerlocaldata.h"
#include "zmr/zmr_playeranimstate.h"


/*
    NOTE: You have to:
    
    Remove LINK_ENTITY_TO_CLASS in hl2mp_player.cpp

    Override CanSprint manually


*/

extern ConVar zm_sv_antiafk;


struct ZMFireBulletsInfo_t;
class CZMPlayerAttackTraceFilter;

class CZMBaseZombie;
class CZMBaseWeapon;
class CZMRagdoll;
class CZMPlayerModelData;

enum ZMPlayerState_t
{
    // Happily running around in the game.
    ZMSTATE_ACTIVE = 0,
    ZMSTATE_OBSERVER_MODE, // Noclipping around, watching players, etc.

    NUM_ZMSTATE_PLAYER
};

class CZMPlayerStateInfo
{
public:
    ZMPlayerState_t m_iPlayerState;
    const char* m_pStateName;

    void (CZMPlayer::*pfnEnterState)();	// Init and deinit the state.
    void (CZMPlayer::*pfnLeaveState)();

    void (CZMPlayer::*pfnPreThink)(); // Do a PreThink() in this state.
};

struct ZMServerWepData_t
{
    void Reset()
    {
        iAmmoType = -1;
        iLastFireCommandNumber = 0;
        flDamage = 0.0f;
        hWeapon.Set( nullptr );
        vecShootPos = vec3_origin;
        bIsMelee = false;
    }

    int iLastFireCommandNumber;
    int iAmmoType;
    float flDamage;
    CHandle<CZMBaseWeapon> hWeapon;
    Vector vecShootPos;
    bool bIsMelee;
};

class CZMPlayer : public CHL2_Player
{
public:
    DECLARE_CLASS( CZMPlayer, CHL2_Player )
    DECLARE_SERVERCLASS()
    //DECLARE_PREDICTABLE()
    DECLARE_DATADESC()
    
    CZMPlayer();
    ~CZMPlayer();

    
    static CZMPlayer* CreatePlayer( const char* className, edict_t* ed )
    {
        CZMPlayer::s_PlayerEdict = ed;
        return static_cast<CZMPlayer*>( CreateEntityByName( className ) );
    }


    virtual void    Precache() OVERRIDE;

    virtual void    InitialSpawn() OVERRIDE;
    virtual void    Spawn() OVERRIDE;
    virtual void    UpdateOnRemove() OVERRIDE;
    virtual void    PreThink() OVERRIDE;
    virtual void    PostThink() OVERRIDE;
    virtual void    PlayerDeathThink() OVERRIDE;
    virtual bool    ClientCommand( const CCommand& args ) OVERRIDE;
    void            PickDefaultSpawnTeam();
    virtual void    ChangeTeam( int iTeam ) OVERRIDE;
    bool            ShouldSpawn();

    virtual bool            IsValidObserverTarget( CBaseEntity* pEnt ) OVERRIDE;
    virtual CBaseEntity*    FindNextObserverTarget( bool bReverse ) OVERRIDE;
    virtual void            CheckObserverSettings() OVERRIDE;

    virtual int     ShouldTransmit( const CCheckTransmitInfo* ) OVERRIDE;

    virtual void    SetAnimation( PLAYER_ANIM playerAnim ) OVERRIDE;
    bool            SetPlayerModel();
    void            UpdatePlayerFOV();
    void            SetHandsModel( const char* model );
    void            SetHandsData( CZMPlayerModelData* pData );
    virtual void    CreateViewModel( int index = 0 ) OVERRIDE;


    virtual void    FlashlightTurnOn() OVERRIDE;
    virtual void    FlashlightTurnOff() OVERRIDE;

    virtual bool    BecomeRagdollOnClient( const Vector& force ) OVERRIDE;
    void            CreateRagdollEntity();

    virtual bool Weapon_Lower() OVERRIDE { return false; };
    
    virtual void PickupObject( CBaseEntity *pObject, bool bLimitMassAndSize ) OVERRIDE;
    virtual	bool IsHoldingEntity( CBaseEntity *pEnt ) OVERRIDE;
    virtual float GetHeldObjectMass( IPhysicsObject *pHeldObject ) OVERRIDE;

    virtual bool BumpWeapon( CBaseCombatWeapon *pWeapon ) OVERRIDE;

    // Lag compensation stuff...
    bool WantsLagCompensationOnNPC( const CZMBaseZombie* pZombie, const CUserCmd* pCmd, const CBitVec<MAX_EDICTS>* pEntityTransmitBits ) const;
    void NoteWeaponFired();

    virtual void    DeathSound( const CTakeDamageInfo& info ) OVERRIDE;
    virtual void    Event_Killed( const CTakeDamageInfo& info ) OVERRIDE;
    virtual int     OnTakeDamage( const CTakeDamageInfo& info ) OVERRIDE;
	virtual void    CommitSuicide( bool bExplode = false, bool bForce = false ) OVERRIDE;
	virtual void    CommitSuicide( const Vector &vecForce, bool bExplode = false, bool bForce = false ) OVERRIDE;
    virtual void    Ignite( float flFlameLifetime, bool bNPCOnly, float flSize, bool bCalledByLevelDesigner ) OVERRIDE;

    virtual CBaseEntity* EntSelectSpawnPoint( void ) OVERRIDE;

    virtual void    ImpulseCommands() OVERRIDE;
    virtual void    CheatImpulseCommands( int ) OVERRIDE;

    virtual int     GiveAmmo( int nCount, int nAmmoIndex, bool bSuppressSound ) OVERRIDE;
    void            GiveDefaultItems( void );
    virtual void    EquipSuit( bool bPlayEffects = false ) OVERRIDE;
    virtual void    RemoveAllItems( bool removeSuit ) OVERRIDE;

    virtual void PlayerUse( void ) OVERRIDE;
    //virtual void PlayUseDenySound() OVERRIDE;


    void State_Transition( ZMPlayerState_t newState );
    void State_Enter( ZMPlayerState_t newState );
    void State_Leave();
    void State_PreThink();
    CZMPlayerStateInfo* State_LookupInfo( ZMPlayerState_t state );

    void State_Enter_ACTIVE();
    void State_PreThink_ACTIVE();
    void State_Enter_OBSERVER_MODE();
    void State_PreThink_OBSERVER_MODE();


    virtual bool StartObserverMode( int mode ) OVERRIDE;
    virtual void StopObserverMode( void ) OVERRIDE;

    
    inline bool IsZM() const { return GetTeamNumber() == ZMTEAM_ZM; }
    inline bool IsHuman() const { return GetTeamNumber() == ZMTEAM_HUMAN; }

    float m_flNextResourceInc;


    void SetBuildSpawn( CZMEntZombieSpawn* pSpawn )
    {
        int index = 0;
        if ( pSpawn )
            index = pSpawn->entindex();


        m_iBuildSpawnIndex = index;
    }

    void SetBuildSpawn( int index )
    {
        m_iBuildSpawnIndex = index;
    }

    inline CZMEntZombieSpawn* GetBuildSpawn() { return dynamic_cast<CZMEntZombieSpawn*>( UTIL_EntityByIndex( m_iBuildSpawnIndex ) ); };
    inline int GetBuildSpawnIndex() { return m_iBuildSpawnIndex; };

    void DeselectAllZombies();


    void SetTeamSpecificProps();
    void PushAway( const Vector& pos, float force );


    // Implemented in zm_player_shared
    bool                HasEnoughResToSpawn( ZombieClass_t zclass );
    bool                HasEnoughRes( int cost );
    int                 GetWeaponSlotFlags();
    int                 GetResources();
    void                IncResources( int res, bool bLimit = false );
    void                SetResources( int res );
    float               GetFlashlightBattery();
    void                SetFlashlightBattery( float battery );
    bool                Weapon_CanSwitchTo( CBaseCombatWeapon* pWeapon ) OVERRIDE;
    Participation_t     GetParticipation();
    virtual void        PlayStepSound( Vector& vecOrigin, surfacedata_t* psurface, float fvol, bool force ) OVERRIDE;
    CBaseCombatWeapon*  GetWeaponForAmmo( int iAmmoType );
    virtual Vector      GetAttackSpread( CBaseCombatWeapon* pWeapon, CBaseEntity* pTarget = nullptr ) OVERRIDE;
    virtual Vector      GetAutoaimVector( float flScale ) OVERRIDE;
    void                DoAnimationEvent( PlayerAnimEvent_t playerAnim, int nData = 0 );
    float               GetAccuracyRatio() const;
    void                UpdateAccuracyRatio();
    void                GetZMMovementVars( float& maxspd, float& accel, float& decel ) const;
    virtual void        FireBullets( const FireBulletsInfo_t& info ) OVERRIDE;
    void                SimulateBullet( ZMFireBulletsInfo_t& bulletinfo );
    bool                HandleBulletPenetration( trace_t& tr, const ZMFireBulletsInfo_t& bulletinfo, Vector& vecNextSrc, float& flDistance );
    bool                HandleShotImpactingWater( const FireBulletsInfo_t& info, const Vector& vecEnd, CTraceFilter* pFilter );
    virtual void        DoMuzzleFlash() OVERRIDE;
    int                 GetTotalAmmoAmount( int iValidAmmoIndex ) const;
    int                 GetAmmoRoom( int iValidAmmoIndex ) const;


    CZMBaseWeapon*  GetWeaponOfHighestSlot();
    CZMBaseWeapon*  GetWeaponOfSlot( const char* szSlotName );
    CZMBaseWeapon*  GetWeaponOfSlot( int slot );
    void            SetWeaponSlotFlags( int flags ) { m_ZMLocal.m_fWeaponSlotFlags = flags; }
    void            AddWeaponSlotFlag( int flag ) { m_ZMLocal.m_fWeaponSlotFlags |= flag; }
    void            RemoveWeaponSlotFlag( int flag ) { m_ZMLocal.m_fWeaponSlotFlags &= ~flag; }

    inline int  GetPickPriority() const { return m_nPickPriority; }
    inline void SetPickPriority( int i ) { m_nPickPriority = i; }
    
    inline float    GetLastActivity() const { return m_flLastActivity; }
    inline bool     IsCloseToAFK() const { return zm_sv_antiafk.GetInt() > 0 && (gpGlobals->curtime - GetLastActivity()) > (zm_sv_antiafk.GetFloat() * 0.8f); }
    inline bool     IsAFK() const { return zm_sv_antiafk.GetInt() > 0 && (gpGlobals->curtime - GetLastActivity()) > zm_sv_antiafk.GetFloat(); }

    inline float    GetNextModelChangeTime() const { return m_flNextModelChangeTime; }
    inline float    GetNextVoiceLineTime() const { return m_flNextVoiceLineTime; }
    inline void     SetNextVoiceLineTime( float t ) { m_flNextVoiceLineTime = t; }

    int             GetZMCommandInterruptFlags() const;

    float           GetInterpNPCTime() const { return m_flInterpNPCTime; }
    void            UpdatePlayerInterpNPC();

    void            UpdatePlayerZMVars();

    void            GetMyRecipientFilter( CRecipientFilter& filter );

    void InitZMFog();


    void CopyWeaponDamage( CZMBaseWeapon* pWeapon, const FireBulletsInfo_t& info );
    void CopyMeleeDamage( CZMBaseWeapon* pWeapon, const Vector& vecSrc, float flDamage );
private:
    void HandleDamagesFromUserCmd();
    ZMServerWepData_t m_ServerWepData;


    // Since I didn't get this at first either, this is only sent to THIS player.
    CNetworkVarEmbedded( CZMPlayerLocalData, m_ZMLocal );

    CNetworkQAngle( m_angEyeAngles );
    CNetworkVar( int, m_iSpawnInterpCounter );
    CNetworkHandle( CZMRagdoll, m_hRagdoll );

    int                 m_iLastWeaponFireUsercmd;
    Vector              m_vecTotalBulletForce; // Accumulator for bullet force in a single frame
	ZMPlayerState_t     m_iPlayerState;
	CZMPlayerStateInfo* m_pCurStateInfo;
    bool                m_bEnterObserver;
    float               m_flNextModelChangeTime;
    float               m_flNextVoiceLineTime;

    CZMPlayerAnimState* m_pPlayerAnimState;

    int m_iBuildSpawnIndex; // To update build menu.
    //Participation_t m_iParticipation;
    int m_nPickPriority;
    float m_flLastActivity;
    float m_flLastActivityWarning;

    // So we don't lag compensate more than once.
    bool m_bIsFireBulletsRecursive;

    // Store all the ammo indices denied for this frame.
    CUtlVector<int> m_vAmmoDenied;

    // The cl_interp_npcs value
    float m_flInterpNPCTime;

    // Cache of client's ZM movement values
    void SafelyClampZMValue( float& value, float min, float max );
    float m_flZMMoveSpeed;
    float m_flZMMoveAccel;
    float m_flZMMoveDecel;
};

inline CZMPlayer* ToZMPlayer( CBaseEntity* pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return nullptr;

	return static_cast<CZMPlayer*>( pEntity );
}

inline CZMPlayer* ToZMPlayer( CBasePlayer* pPlayer )
{
	return static_cast<CZMPlayer*>( pPlayer );
}

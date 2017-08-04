#include "cbase.h"

#include "npcevent.h"
#include "eventlist.h"


#include "zmr/zmr_shareddefs.h"
#include "zmr/zmr_player_shared.h"

#include "zmr_basemelee.h"


#ifdef CLIENT_DLL
#define CZMWeaponImprovised C_ZMWeaponImprovised
#endif

class CZMWeaponImprovised : public CZMBaseMeleeWeapon
{
public:
	DECLARE_CLASS( CZMWeaponImprovised, CZMBaseMeleeWeapon );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

    CZMWeaponImprovised();

    float GetRange() OVERRIDE { return 50.0f; };
    float GetFireRate() OVERRIDE { return 1.0f; };
    float GetDamageForActivity( Activity hitActivity ) { return 20.0f; };

    void AddViewKick() OVERRIDE
    {
        CZMPlayer* pPlayer = GetPlayerOwner();
        if ( !pPlayer ) return;
	    pPlayer->ViewPunch( QAngle( random->RandomFloat( 1.0f, 2.0f ), random->RandomFloat( -2.0f, -1.0f ), 0.0f ) );
    };


#ifndef CLIENT_DLL
    void HandleAnimEvent( animevent_t* ) OVERRIDE;
#else
    bool OnFireEvent( C_BaseViewModel*, const Vector&, const QAngle&, int event, const char* ) OVERRIDE;
#endif
};

IMPLEMENT_NETWORKCLASS_ALIASED( ZMWeaponImprovised, DT_ZM_WeaponImprovised )

BEGIN_NETWORK_TABLE( CZMWeaponImprovised, DT_ZM_WeaponImprovised )
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CZMWeaponImprovised )
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_zm_improvised, CZMWeaponImprovised );
PRECACHE_WEAPON_REGISTER( weapon_zm_improvised );

#ifndef CLIENT_DLL
acttable_t	CZMWeaponImprovised::m_acttable[] = 
{
    { ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_SLAM,                  true },
    { ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_MELEE,					false },
    { ACT_HL2MP_RUN,					ACT_HL2MP_RUN_MELEE,					false },
    { ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_MELEE,			false },
    { ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_MELEE,			false },
    { ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },
    { ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },
    { ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_MELEE,					false },
};
IMPLEMENT_ACTTABLE( CZMWeaponImprovised );
#endif

CZMWeaponImprovised::CZMWeaponImprovised()
{
#ifndef CLIENT_DLL
    SetSlotFlag( ZMWEAPONSLOT_MELEE );
#endif
}

#ifndef CLIENT_DLL
void CZMWeaponImprovised::HandleAnimEvent( animevent_t *pEvent )
{
    Msg( "Handling event: %i\n", pEvent->event );
	switch( pEvent->event )
	{
	case EVENT_WEAPON_MELEE_HIT:
		HandleAnimEventMeleeHit( GetPlayerOwner() );
		break;

	default:
		BaseClass::HandleAnimEvent( pEvent );
		break;
	}
}
#else
bool CZMWeaponImprovised::OnFireEvent( C_BaseViewModel* pViewModel, const Vector& origin, const QAngle& angles, int event, const char* options )
{
    if ( event == EVENT_WEAPON_MELEE_HIT )
    {
        HandleAnimEventMeleeHit( GetOwner() );
        return true;
    }

    return false;
}
#endif
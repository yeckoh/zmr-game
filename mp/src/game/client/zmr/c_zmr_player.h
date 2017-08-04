#pragma once

//#include "c_baseplayer.h"
#include "hl2mp/c_hl2mp_player.h"


#include "zmr/zmr_shareddefs.h"

#include "zmr/zmr_player_shared.h"


class C_ZMPlayer : public CHL2MP_Player
{
public:
    DECLARE_CLASS( C_ZMPlayer, CHL2MP_Player );
    DECLARE_CLIENTCLASS();
    //DECLARE_PREDICTABLE(); 
    DECLARE_INTERPOLATION();

    
    virtual void TeamChange( int ) OVERRIDE;

    // Custom...
    inline bool IsZM() { return GetTeamNumber() == ZMTEAM_ZM; };
    inline bool IsHuman() { return GetTeamNumber() == ZMTEAM_HUMAN; };


    // Implemented in zm_player_shared
    bool HasEnoughResToSpawn( ZombieClass_t );
    bool HasEnoughRes( int );
    int GetResources();
    void SetResources( int );

private:
    CNetworkVar( int, m_nResources );
};

inline CZMPlayer* ToZMPlayer( CBaseEntity* pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<CZMPlayer*>( pEntity );
}

inline CZMPlayer* ToZMPlayer( CBasePlayer* pPlayer )
{
	if ( !pPlayer || !pPlayer->IsPlayer() )
		return NULL;

	return static_cast<CZMPlayer*>( pPlayer );
}
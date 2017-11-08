#include "cbase.h"
#include "gamestringpool.h"
#include "cmodel.h"

#include "c_clientsprite.h"
//#include "c_baseclientent.h"

static CUtlVector<C_ClientSprite*> g_vSprites;

CON_COMMAND( cl_showsprites, "" )
{
    Msg( "Client sprites: %i\n", g_vSprites.Count() );
}


C_ClientSprite::C_ClientSprite()
{
    g_vSprites.AddToTail( this );
}

C_ClientSprite::~C_ClientSprite()
{
    g_vSprites.FindAndRemove( this );
}

C_BaseClientEnt* C_ClientSprite::CreateNew()
{
    return new C_ClientSprite();
}

bool C_ClientSprite::Initialize()
{
    if ( m_iNetworkStatus != ENS_CLIENTSIDE )
    {
        DevMsg( "Sprite has a parent/targetname and cannot be spawned as a client ent!\n" );
        return false;
    }


	if ( InitializeAsClientEntity( STRING( GetModelName() ), RENDER_GROUP_TRANSLUCENT_ENTITY ) == false )
	{
		return false;
	}

    Spawn();

    DevMsg( "Successfully initialized sprite!\n" );

    return true;
}

bool C_ClientSprite::KeyValue( const char* szKeyName, const char* szValue )
{
	if ( FStrEq( szKeyName, "model" ) )
	{
		SetModelName( AllocPooledString( szValue ) );
        return true;
	}
    else if ( FStrEq( szKeyName, "rendermode" ) )
    {
        //SetRenderMode( (RenderMode_t)atoi( szValue ) );
        return true;
    }
    else if ( FStrEq( szKeyName, "parentname" ) )
    {
        m_iNetworkStatus = ENS_SHARED;
    }
    else if ( FStrEq( szKeyName, "targetname" ) )
    {
        m_iNetworkStatus = ENS_SHARED;
    }


	return BaseClass::KeyValue( szKeyName, szValue );
}

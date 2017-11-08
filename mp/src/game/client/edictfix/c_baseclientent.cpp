#include "cbase.h"
#include "mapentities_shared.h"


#include "c_baseclientent.h"

#include "c_clientsprite.h"



CUtlVector<C_BaseClientEnt*> g_vClientEnts;


C_BaseClientEnt::C_BaseClientEnt()
{
    m_iNetworkStatus = ENS_CLIENTSIDE;


    g_vClientEnts.AddToTail( this );
}

C_BaseClientEnt::~C_BaseClientEnt()
{
    g_vClientEnts.FindAndRemove( this );
}

void C_BaseClientEnt::RecreateAll()
{
    DestroyAll();

    ParseAllEntities( engine->GetMapEntitiesString() );
}

void C_BaseClientEnt::DestroyAll()
{
    while ( g_vClientEnts.Count() > 0 )
    {
        g_vClientEnts[0]->Release();
    }
}

void C_BaseClientEnt::ParseAllEntities( const char* pMapData )
{
	int nEntities = 0;

	char szTokenBuffer[MAPKEY_MAXLENGTH];

	//
	//  Loop through all entities in the map data, creating each.
	//
	for ( ; true; pMapData = MapEntity_SkipToNextEntity(pMapData, szTokenBuffer) )
	{
		//
		// Parse the opening brace.
		//
		char token[MAPKEY_MAXLENGTH];
		pMapData = MapEntity_ParseToken( pMapData, token );

		//
		// Check to see if we've finished or not.
		//
		if (!pMapData)
			break;

		if (token[0] != '{')
		{
			Error( "MapEntity_ParseAllEntities: found %s when expecting {", token);
			continue;
		}

		//
		// Parse the entity and add it to the spawn list.
		//

		pMapData = ParseEntity( pMapData );

		nEntities++;
	}
}

const char* C_BaseClientEnt::ParseEntity( const char* pEntData )
{
	CEntityMapData entData( (char*)pEntData );
	char className[MAPKEY_MAXLENGTH];
	
	MDLCACHE_CRITICAL_SECTION();

	if ( !entData.ExtractValue( "classname", className ) )
	{
		Error( "classname missing from entity!\n" );
	}


    int len = g_vClientEntRegister.Count();
    for ( int i = 0; i < len; i++ )
    {
        if ( Q_strcmp( className, g_vClientEntRegister[i].classname ) == 0 )
        {
            C_BaseClientEnt* pEnt = (*g_vClientEntRegister[i].pfn)();

		    if ( pEnt )
		    {	// Set up keyvalues.
			    pEnt->ParseMapData( &entData );
			
			    if ( !pEnt->Initialize() )
				    pEnt->Release();
		
			    return entData.CurrentBufferPosition();
		    }
        }
    }
	

	// Just skip past all the keys.
	char keyName[MAPKEY_MAXLENGTH];
	char value[MAPKEY_MAXLENGTH];
	if ( entData.GetFirstKey(keyName, value) )
	{
		do 
		{
		} 
		while ( entData.GetNextKey(keyName, value) );
	}

	//
	// Return the current parser position in the data block
	//
	return entData.CurrentBufferPosition();
}



void RegisterClientEnts()
{
    REGISTER_CLIENTENT( env_sprite, C_ClientSprite::CreateNew );
    REGISTER_CLIENTENT( env_sprite_clientside, C_ClientSprite::CreateNew );
}


static CUtlVector<ClientEntRegister_t> g_vClientEntRegister;
CUtlVector<ClientEntRegister_t>* g_pClientEntRegister = &(g_vClientEntRegister);

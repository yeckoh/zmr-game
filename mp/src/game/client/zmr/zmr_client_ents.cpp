#include "cbase.h"
#include "mapentities_shared.h"



class C_ClientMapEnt : public C_BaseEntity
{
public:
    static C_ClientMapEnt* CreateClientMapEntity()
    {
        AssertAlways( "You forgot to implement CreateClientMapEntity()" );

        return nullptr;
    }
};

class CClientMapEnts : public CAutoGameSystem
{
public:
    CClientMapEnts() : CAutoGameSystem( "ClientMapEnts" )
    {
    }

    virtual void LevelInitPostEntity() OVERRIDE;


protected:
    void ParseAllEntities( const char* );
    const char* ParseEntity( const char* );
};

void CClientMapEnts::LevelInitPostEntity()
{
    ParseAllEntities( engine->GetMapEntitiesString() );
}


void CClientMapEnts::ParseAllEntities( const char* pMapData )
{
	int nEntities = 0;

	char szTokenBuffer[MAPKEY_MAXLENGTH];

	//
	//  Loop through all entities in the map data, creating each.
	//
	for ( ; true; pMapData = MapEntity_SkipToNextEntity( pMapData, szTokenBuffer ) )
	{
		//
		// Parse the opening brace.
		//
		char token[MAPKEY_MAXLENGTH];
		pMapData = MapEntity_ParseToken( pMapData, token );

		//
		// Check to see if we've finished or not.
		//
		if ( !pMapData )
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


const char* CClientMapEnts::ParseEntity( const char* pEntData )
{
	CEntityMapData entData( (char*)pEntData );
	char className[MAPKEY_MAXLENGTH];
	
	MDLCACHE_CRITICAL_SECTION();

	if (!entData.ExtractValue("classname", className))
	{
		Error( "classname missing from entity!\n" );
	}

	if ( Q_strcmp( className, "prop_physics_multiplayer" ) == 0 )
	{
		// always force clientside entitis placed in maps
		C_ClientMapEnt* pEntity = C_ClientMapEnt::CreateClientMapEntity(); 

		if ( pEntity )
		{	// Set up keyvalues.
			pEntity->ParseMapData( &entData );
			
			if ( !pEntity->Initialize() )
				pEntity->Release();
		
			return entData.CurrentBufferPosition();
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

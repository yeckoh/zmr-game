#pragma once


enum EntNetworkStatus_t
{
    ENS_CLIENTSIDE = 0,
    ENS_SERVERSIDE,

    ENS_SHARED,
};

class C_BaseClientEnt;

typedef C_BaseClientEnt* (*CLIENTENTCREATENEW)();

struct ClientEntRegister_t
{
    ClientEntRegister_t( const char* pszClassname, CLIENTENTCREATENEW fn )
    {
        Q_strncpy( classname, pszClassname, sizeof( classname ) );
        pfn = fn;
    }


    char classname[32];
    CLIENTENTCREATENEW pfn;
};

abstract_class C_BaseClientEnt
{
public:
    C_BaseClientEnt();
    ~C_BaseClientEnt();


    virtual bool            Initialize() = 0;
    virtual void            ParseMapData( CEntityMapData* mapData ) = 0;
    virtual void            Release() = 0;


    static void             RecreateAll(); // recreate all clientside ents in map
    static void             DestroyAll();  // clear all clientside created ents

protected:
	static void             ParseAllEntities( const char* pMapData );
	static const char*      ParseEntity( const char* pEntData );

    EntNetworkStatus_t m_iNetworkStatus;
};

void RegisterClientEnts();

CUtlVector<ClientEntRegister_t>* g_pClientEntRegister;


#define REGISTER_CLIENTENT(classname,fn) g_pClientEntRegister->AddToTail( ClientEntRegister_t( #classname, &fn ) );

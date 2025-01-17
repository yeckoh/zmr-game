#pragma once


#include "zmr/zmr_obj_manager.h"
#include "zmr/zmr_shareddefs.h"


class CObjLine
{
public:
    inline void Reset()
    {
        m_szTexts[0] = NULL;
        m_iTextArgType = OBJARGTYPE_NONE;
        m_flTextArgNum = 0.0f;
        m_szTextArg[0] = NULL;
        m_bComplete = false;
        m_flTimerEnd = 0.0f;
    }


    static void WriteEmptyDisplayUserMsg();

    void WriteArg();
    void WriteDisplayUserMsg();
    void WriteUpdateUserMsg();
    void ParseArg( const char* psz );
    void SetText( const char* psz );


    char m_szTexts[MAX_OBJ_LINESIZE];
    ObjArgType_t m_iTextArgType;

    float m_flTimerEnd;
    float m_flTextArgNum;
    char m_szTextArg[32];

    bool m_bComplete;
};

class CZMEntObjectives : public CServerOnlyPointEntity
{
public:
    DECLARE_CLASS( CZMEntObjectives, CServerOnlyPointEntity )
    DECLARE_DATADESC()


    CZMEntObjectives();
    ~CZMEntObjectives();

    void Spawn( void ) OVERRIDE;
    

    void InputDisplay( inputdata_t &inputData );
    void InputDisplayActivator( inputdata_t &inputData );
    void InputUpdate( inputdata_t &inputData );
    void InputReset( inputdata_t &inputData );

    void InputSetMainText( inputdata_t &inputData );
    void InputSetChild1Text( inputdata_t &inputData );
    void InputSetChild2Text( inputdata_t &inputData );
    void InputSetChild3Text( inputdata_t &inputData );
    void InputSetChild4Text( inputdata_t &inputData );

    void InputSetMainTextArg( inputdata_t &inputData );
    void InputSetChild1TextArg( inputdata_t &inputData );
    void InputSetChild2TextArg( inputdata_t &inputData );
    void InputSetChild3TextArg( inputdata_t &inputData );
    void InputSetChild4TextArg( inputdata_t &inputData );

    /*void InputSetMainTextArgType( inputdata_t &inputData );
    void InputSetChild1TextArgType( inputdata_t &inputData );
    void InputSetChild2TextArgType( inputdata_t &inputData );
    void InputSetChild3TextArgType( inputdata_t &inputData );
    void InputSetChild4TextArgType( inputdata_t &inputData );*/

    void InputIncMainTextArg( inputdata_t &inputData );
    void InputIncChild1TextArg( inputdata_t &inputData );
    void InputIncChild2TextArg( inputdata_t &inputData );
    void InputIncChild3TextArg( inputdata_t &inputData );
    void InputIncChild4TextArg( inputdata_t &inputData );

    void InputCompleteMain( inputdata_t &inputData );
    void InputCompleteChild1( inputdata_t &inputData );
    void InputCompleteChild2( inputdata_t &inputData );
    void InputCompleteChild3( inputdata_t &inputData );
    void InputCompleteChild4( inputdata_t &inputData );

    void InputInCompleteMain( inputdata_t &inputData );
    void InputInCompleteChild1( inputdata_t &inputData );
    void InputInCompleteChild2( inputdata_t &inputData );
    void InputInCompleteChild3( inputdata_t &inputData );
    void InputInCompleteChild4( inputdata_t &inputData );

    void InputResetMain( inputdata_t &inputData );
    void InputResetChild1( inputdata_t &inputData );
    void InputResetChild2( inputdata_t &inputData );
    void InputResetChild3( inputdata_t &inputData );
    void InputResetChild4( inputdata_t &inputData );

    void InputUpdateMainText( inputdata_t &inputData );
    void InputUpdateChild1Text( inputdata_t &inputData );
    void InputUpdateChild2Text( inputdata_t &inputData );
    void InputUpdateChild3Text( inputdata_t &inputData );
    void InputUpdateChild4Text( inputdata_t &inputData );

    void Reset();

    inline ObjRecipient_t GetRecipient() { return m_iRecipient; };
    void GetRecipientFilter( CBaseEntity* pActivator, CRecipientFilter& filter, ObjRecipient_t = OBJRECIPIENT_INVALID );
    static void RecipientToFilter( CBaseEntity* pActivator, CRecipientFilter& filter, ObjRecipient_t = OBJRECIPIENT_INVALID );
    inline void WriteDisplayUserMsg( int i ) { m_Lines[i].WriteDisplayUserMsg(); };
    inline void WriteUpdateUserMsg( int i ) { m_Lines[i].WriteUpdateUserMsg(); };

private:
    CObjLine m_Lines[NUM_OBJ_LINES];
    ObjRecipient_t m_iRecipient;
};


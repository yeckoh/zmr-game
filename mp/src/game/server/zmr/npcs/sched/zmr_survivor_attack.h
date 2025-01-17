#pragma once

class CZMSurvivorAttackInt
{
public:
    CBaseEntity*    GetAttackTarget() const { return m_pEnemy.Get(); }
    void            SetAttackTarget( CBaseEntity* pEnt ) { m_pEnemy.Set( pEnt ); }


private:
    CHandle<CBaseEntity> m_pEnemy;
};

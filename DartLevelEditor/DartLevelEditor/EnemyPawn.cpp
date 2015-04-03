#include "EnemyPawn.h"


EnemyPawn::EnemyPawn(const PawnType type, D3DXVECTOR3& initPos/* = D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	m_ePawnType = type;
	m_vScale = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
	m_vPos = initPos;

	switch (m_ePawnType) {
	case PawnType::PT_EnemyMeleeSpawnLocale: m_vDiffuseColor = D3DXVECTOR4(0.41f, 0.2f, 1.0f, 1.0f); break;
	case PawnType::PT_EnemyFireSpawnLocale: m_vDiffuseColor = D3DXVECTOR4(0.5f, 1.0f, 0.60f, 1.0f); break;
	case  PawnType::PT_EnemySeedSpawnLocale: m_vDiffuseColor = D3DXVECTOR4(0.91f, 0.502f, 0.30f, 1.0f); break;
	default: m_vDiffuseColor = D3DXVECTOR4(1.0f, 1.02f, 1.0f, 1.0f); break;
	}
}


EnemyPawn::~EnemyPawn()
{
}

#include "PickUpPawn.h"


PickUpPawn::PickUpPawn(const PawnType type, D3DXVECTOR3& initPos /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	m_ePawnType = type;
	m_vScale = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
	m_vPos = initPos;

	switch (m_ePawnType) {
	case PawnType::PT_Pickup_Heal: m_vDiffuseColor = D3DXVECTOR4(0.1f, 0.2f, 1.0f, 1.0f); break; 
	case PawnType::PT_Pickup_Bonus: m_vDiffuseColor = D3DXVECTOR4(0.5f, 1.0f, 0.0f, 1.0f); break;
	case  PawnType::PT_Pickup_Ammo_Seed: m_vDiffuseColor = D3DXVECTOR4(0.91f, 0.02f, 0.30f, 1.0f); break;
	case  PawnType::PT_Pickup_Ammo_Fire: m_vDiffuseColor = D3DXVECTOR4(0.91f, 0.802f, 0.30f, 1.0f); break;
	default: m_vDiffuseColor = D3DXVECTOR4(1.0f, 1.02f, 1.0f, 1.0f); break;
	}
}


PickUpPawn::~PickUpPawn()
{
}

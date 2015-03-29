#include "FleePointPawn.h"


FleePointPawn::FleePointPawn()
{
	m_ePawnType = PawnType::PT_FleePoint;
	m_vDiffuseColor = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	m_vScale = D3DXVECTOR3(50.f, 50.f, 50.f);
}


FleePointPawn::~FleePointPawn()
{
}

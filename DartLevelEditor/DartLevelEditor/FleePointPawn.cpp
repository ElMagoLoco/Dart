#include "FleePointPawn.h"


FleePointPawn::FleePointPawn()
{
	m_ePawnType = PawnType::PT_FleePoint;
	m_vDiffuseColor = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
}


FleePointPawn::~FleePointPawn()
{
}

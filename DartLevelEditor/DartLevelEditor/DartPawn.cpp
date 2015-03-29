#include "DartPawn.h"


DartPawn::DartPawn()
{
	m_ePawnType = PawnType::PT_DartSpawnLocale;
	m_vDiffuseColor = D3DXVECTOR4(0.0f, 0.0f, 0.65f, 1.0f);
	m_vScale = D3DXVECTOR3(50.f, 50.f, 50.f);
}


DartPawn::~DartPawn()
{
}

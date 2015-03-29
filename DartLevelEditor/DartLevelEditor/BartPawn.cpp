#include "BartPawn.h"


BartPawn::BartPawn()
{
	m_ePawnType = PawnType::PT_BartSpawnLocale;
	m_vDiffuseColor = D3DXVECTOR4(0.35f, 0.35f, 1.0f, 1.0f);
	m_vScale = D3DXVECTOR3(50.f, 50.f, 50.f);
}


BartPawn::~BartPawn()
{
}

#include "GoalPawn.h"


GoalPawn::GoalPawn()
{
	m_ePawnType = PawnType::PT_PointOfLevelLocale;
	m_vDiffuseColor = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_vScale = D3DXVECTOR3(50.f, 50.f, 50.f);
}


GoalPawn::~GoalPawn()
{
}

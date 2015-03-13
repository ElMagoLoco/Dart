#include "WallPawn.h"


WallPawn::WallPawn()
{
	m_bSolid = true;
	m_vDiffuseColor = D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f);
	m_ePawnType = PawnType::PT_Wall;
}

WallPawn::WallPawn(float width, float length, float heigth)
{
	m_bSolid = true;
	m_vDiffuseColor = D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f);
	m_ePawnType = PawnType::PT_Wall;
	m_vScale = { width, heigth, length };
}


WallPawn::~WallPawn()
{
}

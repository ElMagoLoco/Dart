#include "EditorPawn.h"


EditorPawn::EditorPawn() :
m_ePawnType(PT_Undefined),
m_vPos(0.0f, 0.0f, 0.0f),
m_vScale(1.0f, 1.0f, 1.0f),
m_vDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f),
m_nMeshID(-1),
m_bSolid(false)
{
	D3DXMatrixIdentity(&m_matWorld);
}

EditorPawn::EditorPawn(PawnType type, D3DXVECTOR3& initPos, D3DXVECTOR4& color /*= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)*/, D3DXVECTOR3& initScale /*= D3DXVECTOR3(1.0f, 1.0f, 1.0f)*/) :
m_ePawnType(type),
m_vPos(initPos),
m_vDiffuseColor(color),
m_vScale(initScale),
m_nMeshID(-1),
m_bSolid(false)
{
	D3DXMATRIX scaling;
	D3DXMATRIX trans;

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixScaling(&scaling, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&trans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&m_matWorld, &scaling, &trans);
}


EditorPawn::~EditorPawn()
{
}

void EditorPawn::update(float dt, D3DXVECTOR3& velo /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	D3DXMATRIX trans;
	D3DXMATRIX scale;
	m_vPos += velo * dt;

	D3DXMatrixTranslation(&trans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixScaling(&scale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixMultiply(&m_matWorld, &scale, &trans);
}

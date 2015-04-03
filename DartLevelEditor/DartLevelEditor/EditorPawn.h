#pragma once
#include "D3DUtils.h"

class EditorPawn
{
public:
	enum PawnType {
		PT_Undefined = -1,
		PT_EnemyMeleeSpawnLocale,
		PT_EnemyFireSpawnLocale,
		PT_EnemySeedSpawnLocale,
		PT_DartSpawnLocale,
		PT_BartSpawnLocale,
		PT_PointOfLevelLocale,
		PT_RockLocale,
		PT_Wall,
		PT_Pickup_Heal,
		PT_Pickup_Bonus,
		PT_Pickup_Ammo_Seed,
		PT_Pickup_Ammo_Fire,
		PT_FleePoint,
		PT_End
	};

	D3DXMATRIX& getMatWorld() { return m_matWorld; }
	D3DXVECTOR4& getDiffuseColor() { return m_vDiffuseColor; }
	PawnType getPawnType() const { return m_ePawnType; }
	D3DXVECTOR3& getScale() { return m_vScale; }
	D3DXVECTOR3& getPos() { return m_vPos; }
	int getMeshID() const { return m_nMeshID; }
	bool getSolid() const { return m_bSolid; }

	bool operator==(const EditorPawn& other)		{ return ((this == &other) ? true : false); }

	void setMeshID(int val) { m_nMeshID = val; }
	void setMatWorld(D3DXMATRIX& val) { m_matWorld = val; }
	void setDiffuseColor(D3DXVECTOR4& val) { m_vDiffuseColor = val; }
	void setPawnType(EditorPawn::PawnType val) { m_ePawnType = val; }	
	void setPos(D3DXVECTOR3& val) { m_vPos = val; }
	void setSolid(bool val) { m_bSolid = val; }
	void setScale(D3DXVECTOR3& val) { m_vScale = val; }

	virtual void update(float dt, D3DXVECTOR3& velo = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	EditorPawn(PawnType type, D3DXVECTOR3& initPos, D3DXVECTOR4& color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3& initScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	EditorPawn();
	virtual ~EditorPawn();

protected:
	D3DXMATRIX		m_matWorld;

	D3DXVECTOR4	m_vDiffuseColor;

	D3DXVECTOR3	m_vPos;
	D3DXVECTOR3	m_vScale;
	//D3DXVECTOR3	m_vRot;

	PawnType		m_ePawnType;
	
	int		m_nMeshID;

	bool m_bSolid;
};


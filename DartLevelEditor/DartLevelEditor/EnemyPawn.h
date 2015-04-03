#pragma once
#include "EditorPawn.h"
class EnemyPawn :	public EditorPawn
{
public:
	EnemyPawn(const PawnType type, D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	virtual ~EnemyPawn();
};


#pragma once
#include "EditorPawn.h"
class PickUpPawn :	public EditorPawn
{
public:
	PickUpPawn(const PawnType type, D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	virtual ~PickUpPawn();
};


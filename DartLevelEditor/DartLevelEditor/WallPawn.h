#pragma once
#include "EditorPawn.h"
class WallPawn :	public EditorPawn
{
public:
	WallPawn(float width, float length, float heigth);
	WallPawn();
	virtual ~WallPawn();
};


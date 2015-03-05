//=============================================================================
// FrameStats.h
//
// Class used for keeping track of and displaying the frames rendered per second,
// milliseconds per frame, and vertex and triangle counts.
//=============================================================================
#pragma once

#include <d3dx9.h>
#include "D3DUtils.h"
//singleton class so anything can add debug info
class FrameStats
{
public:
	FrameStats();
	~FrameStats();

	void onLostDevice();
	void onResetDevice();

	void update(float _dt);
	void display();
private:
	// Prevent copying
	FrameStats(const FrameStats& _rhs);
	FrameStats& operator=(const FrameStats& _rhs);

	ID3DXFont* mFont;
	float mFPS;
	float mMilliSecPerFrame;
};

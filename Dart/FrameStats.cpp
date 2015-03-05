//=============================================================================
// GfxStats.cpp
//=============================================================================

#include "D3DUtils.h"
#include "D3DApp.h"
#include "FrameStats.h"
#include <sstream>
#include <tchar.h>

//constructor
FrameStats::FrameStats()
	: mFont(0), mFPS(0.0f), mMilliSecPerFrame(0.0f)
{
	//set up font
	D3DXFONT_DESC fontDesc;
	fontDesc.Height = 18;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	//create font
	HR(D3DXCreateFontIndirect(gD3DDevice, &fontDesc, &mFont));
}
//destructor
FrameStats::~FrameStats()
{
	SAFE_RELEASE(mFont);
}

void FrameStats::onLostDevice()
{
	HR(mFont->OnLostDevice());
}

void FrameStats::onResetDevice()
{
	HR(mFont->OnResetDevice());
}

void FrameStats::update(float _dt)
{
	// Make static so that their values persist accross function calls.
	static float numFrames = 0.0f;
	static float timeElapsed = 0.0f;

	// Increment the frame count.
	numFrames += 1.0f;

	// Accumulate how much time has passed.
	timeElapsed += _dt;

	// Has one second passed?--we compute the frame statistics once 
	// per second.  Note that the time between frames can vary so 
	// these stats are averages over a second.
	if (timeElapsed >= 1.0f)
	{
		// Frames Per Second = numFrames / timeElapsed,
		// but timeElapsed approx. equals 1.0, so 
		// frames per second = numFrames.

		mFPS = numFrames;

		// Average time, in miliseconds, it took to render a single frame.
		mMilliSecPerFrame = 1000.0f / mFPS;

		// Reset time counter and frame count to prepare for computing
		// the average stats over the next second.
		// Reseting by subtracting 1 instead of just setting to 0 is 
		// more accurate 
		timeElapsed -= 1.0f;
		numFrames = 0.0f;
	}
}

void FrameStats::display()
{
	// Make static so memory is not allocated every frame.
	static wchar_t buffer[256];

	//display frames and any additional debug info needed
	swprintf(buffer, 
		L"Frames Per Second = %.0f\n"
		L"Milliseconds Per Frame = %.4f\n", 
		mFPS, mMilliSecPerFrame);
	RECT R = { 5, 5, 0, 0 };
	HR(mFont->DrawText(0, buffer, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(255, 0, 255)));
}


#pragma once

#include "D3DApp.h"
#include <crtdbg.h>
#include <list>
#include <string>
#if defined(DEBUG) | defined(_DEBUG)
#include "FrameStats.h"

#endif

class ProjectApp : public D3DApp
{
public:
	//constructor/destructer//cleanup
	ProjectApp(HINSTANCE _hInstance, LPCWSTR _winCaption, D3DDEVTYPE _devType, DWORD _requestedVP);
	~ProjectApp();
	void CleanUp();
	//override proper functions from D3DApp
	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float _dt);
	void drawScene();
	//initialize any game components that aren't covered by state events
	//this should run before initStateMachine
	void initGameComponents();
	void initStateMachine();//initialize state machine
private:
#if defined(DEBUG) | defined(_DEBUG)
	FrameStats		mFrameStats;
#endif
};







#pragma once
#include "D3DApp.h"

class RenderWindow :	public D3DApp
{
public:
	RenderWindow(HINSTANCE _hInstance, LPCWSTR _winCaption, D3DDEVTYPE _devType, DWORD _requestedVP);
	RenderWindow();
	virtual ~RenderWindow();

	virtual void CleanUp();
	virtual bool checkDeviceCaps();
	virtual void onLostDevice();
	virtual void onResetDevice();
	virtual void updateScene(float _dt);
	virtual void drawScene();
	virtual void initMainWindow(HINSTANCE _hInstance);
	virtual void initDirect3D();
	virtual int run();

	virtual LRESULT msgProc(UINT _msg, WPARAM _wParam, LPARAM _lParam);

};


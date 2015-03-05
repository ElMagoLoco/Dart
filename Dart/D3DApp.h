//D3DGame.h
//This is the base Direct3D game class with provides the framework interface for sample applications.
//Clients derive from this, override the framework methods, and instantiate a single instance of derived class.
//Client should also set global application pointer (gD3DGame) to point to the one instance e.g. gD3DApp = new GameApp(hInstance);

#pragma once

#include "D3DUtils.h"

class D3DApp
{
public:
	//app creates window and starts system
	D3DApp(HINSTANCE _hInstance, LPCWSTR _winCaption, D3DDEVTYPE _devType, DWORD _requestedVP);
	virtual					~D3DApp();//destructor
	virtual void			CleanUp();//clean up before exiting

	HINSTANCE				getAppInst();//get app instance
	HWND					getMainWnd();//handle for main window
	//framework methods: derived class overrides these to implement game requirements
	virtual bool			checkDeviceCaps() { return true; }//check abilities of hardware
	virtual void			onLostDevice() {}//usually when the screen size changes or goes full screen/windowed
	virtual void			onResetDevice() {}//reset after window change
	virtual void			updateScene(float _dt) { UNREFERENCED_PARAMETER(_dt); }//used by child object
	virtual void			drawScene() {}//used by child object
	//these don't need to be overridden unless you want to change default window creation, 
	//direct3D device creation, window procedure, or message loop. 
	virtual void			initMainWindow(HINSTANCE _hInstance);
	virtual void			initDirect3D();
	virtual int				run();//run program
	virtual LRESULT			msgProc(UINT _msg, WPARAM _wParam, LPARAM _lParam);//message procedure

	void					enableFullScreenMode(bool _bEnable);
	void					changeWindowResolution(UINT _width, UINT _height);
	bool					isDeviceLost();
protected:
	//derived class can modify these to customize
	LPCWSTR					mMainWndCaption;
	D3DDEVTYPE				mDevType;
	DWORD					mRequestedVP;
	//game, windows, direct3d data members
	HINSTANCE				mhAppInst;
	HWND					mhMainWnd;
	IDirect3D9*				mD3DObject;
	bool					mAppPaused;
	D3DPRESENT_PARAMETERS	mD3DPP;
};
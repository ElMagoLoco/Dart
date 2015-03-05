//D3DGame.h
//This is the base Direct3D game class with provides the framework interface for sample applications.
//Clients derive from this, override the framework methods, and instantiate a single instance of derived class.
//Client should also set global application pointer (gD3DGame) to point to the one instance e.g. gD3DApp = new GameApp(hInstance);

// this is a test
#include <assert.h>
#include "D3DApp.h"
//global window size
UINT				gWindowWidth = 1024;
UINT				gWindowHeight = 768;
//other globals for convenient access
D3DApp*				gD3DApp = NULL;//global D3DApp
IDirect3DDevice9*	gD3DDevice = NULL;//global D3D Device

//main window procedure
LRESULT CALLBACK MainWndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (gD3DApp != NULL)
		return gD3DApp->msgProc(_msg, _wParam, _lParam);
	else
		return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}
//constructor, set up window and app options
D3DApp::D3DApp(HINSTANCE _hInstance, LPCWSTR _winCaption, D3DDEVTYPE _devType, DWORD _requestedVP)
{
	mMainWndCaption = _winCaption;
	mDevType = _devType;
	mRequestedVP = _requestedVP;
	mhAppInst = _hInstance;
	mhMainWnd = NULL;
	mD3DObject = NULL;
	mAppPaused = false;
	ZeroMemory(&mD3DPP, sizeof(mD3DPP));
	//init window and D3D
	initMainWindow(_hInstance);
	initDirect3D();
}
//destructor
D3DApp::~D3DApp()
{
	CleanUp();
}
//release D3DApp options; also call this from cleanup function of the child app
void D3DApp::CleanUp()
{
	SAFE_RELEASE(mD3DObject);
	SAFE_RELEASE(gD3DDevice);
}
//get app instance
HINSTANCE D3DApp::getAppInst()
{
	return mhAppInst;
}
//handle for main window
HWND D3DApp::getMainWnd()
{
	return mhMainWnd;
}
//init main window
void D3DApp::initMainWindow(HINSTANCE _hInstance)
{
	WNDCLASSEX			wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClassEx(&wc))
	{//failed to register
		MessageBox(NULL, L"RegisterClassEx FAILED", L"D3DApp::initMainWindow", MB_ICONERROR);
		PostQuitMessage(0);
	}

	//default to initial window size
	//start with windowed window of global size
	RECT R = { 0, 0, gWindowWidth, gWindowHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	mhMainWnd = CreateWindow(L"D3DWndClassName", mMainWndCaption, WS_OVERLAPPEDWINDOW, 5, 5,
		R.right - R.left, R.bottom - R.top, NULL, NULL, mhAppInst, NULL);

	if (!mhMainWnd)
	{
		MessageBox(NULL, L"CreateWindow FAILED", L"D3DApp::initMainWindow", MB_ICONERROR);
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
}

//initialize direct 3d
void D3DApp::initDirect3D()
{
	// Step 1: Create the IDirect3D9 object.

	mD3DObject = Direct3DCreate9(D3D_SDK_VERSION);
	if (mD3DObject == NULL)
	{
		MessageBox(NULL, L"Direct3DCreate9 FAILED", L"D3DApp::initDirect3D", MB_ICONERROR);
		PostQuitMessage(0);
	}


	// Step 2: Verify hardware support for specified formats in windowed and full screen modes.

	D3DDISPLAYMODE mode;
	mD3DObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	HR(mD3DObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, mode.Format, mode.Format, true));
	HR(mD3DObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));

	// Step 3: Check for requested vertex processing and pure device.

	D3DCAPS9 caps;
	HR(mD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, mDevType, &caps));

	DWORD devBehaviorFlags = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		devBehaviorFlags |= mRequestedVP;
	}
	else
	{
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// If pure device and HW T&L supported
	if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
		devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;
	}

	// Step 4: Fill out the D3DPRESENT_PARAMETERS structure.

	mD3DPP.BackBufferWidth = 0;
	mD3DPP.BackBufferHeight = 0;
	mD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	mD3DPP.BackBufferCount = 1;
	mD3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	mD3DPP.MultiSampleQuality = 0;
	mD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mD3DPP.hDeviceWindow = mhMainWnd;
	mD3DPP.Windowed = true;
	mD3DPP.EnableAutoDepthStencil = true;
	mD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	mD3DPP.Flags = 0;
	mD3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//only use default for presentation interval if you want it to stick to 60 FPS
	mD3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//	mD3DPP.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;



	// Step 5: Create the device.

	HR(mD3DObject->CreateDevice(D3DADAPTER_DEFAULT, // primary adapter
		mDevType,           // device type
		mhMainWnd,          // window associated with device
		devBehaviorFlags,   // vertex processing
		&mD3DPP,            // present parameters
		&gD3DDevice));      // return created device
}

int D3DApp::run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Use precision timer, helps with deltatime and framerate
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	// Message loop
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			// If the application is paused then free some CPU cycles to other 
			// applications and then continue on to the next frame.
			if (mAppPaused)
			{
				Sleep(20);
				continue;
			}

			if (!isDeviceLost())
			{
				__int64 currTimeStamp = 0;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
				float dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;

				updateScene(dt);
				drawScene();

				// Prepare for next iteration: The current time stamp becomes
				// the previous time stamp for the next iteration.
				prevTimeStamp = currTimeStamp;
			}
		}
	}
	return (int)msg.wParam;
}

LRESULT D3DApp::msgProc(UINT _msg, WPARAM _wParam, LPARAM _lParam)
{


	// Is the application in a minimized or maximized state?
	static bool minOrMaxed = false;

	RECT clientRect = { 0, 0, 0, 0 };
	switch (_msg)
	{

		// WM_ACTIVE is sent when the window is activated or deactivated.
		// We pause the game when the main window is deactivated and 
		// unpause it when it becomes active.
	case WM_ACTIVATE:
		if (LOWORD(_wParam) == WA_INACTIVE)
			mAppPaused = true;
		else
			mAppPaused = false;
		return 0;


		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		if (gD3DDevice)
		{
			mD3DPP.BackBufferWidth = gWindowWidth = LOWORD(_lParam);
			mD3DPP.BackBufferHeight = gWindowHeight = HIWORD(_lParam);

			if (_wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				minOrMaxed = true;
			}
			else if (_wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				minOrMaxed = true;
				onLostDevice();
				HR(gD3DDevice->Reset(&mD3DPP));
				onResetDevice();
			}
			// Restored is any resize that is not a minimize or maximize.
			// For example, restoring the window to its default size
			// after a minimize or maximize, or from dragging the resize
			// bars.
			else if (_wParam == SIZE_RESTORED)
			{
				mAppPaused = false;

				// Are we restoring from a mimimized or maximized state, 
				// and are in windowed mode?  Do not execute this code if 
				// we are restoring to full screen mode.
				if (minOrMaxed && mD3DPP.Windowed)
				{
					onLostDevice();
					HR(gD3DDevice->Reset(&mD3DPP));
					onResetDevice();
				}
				else
				{
					// No, which implies the user is resizing by dragging
					// the resize bars.  However, we do not reset the device
					// here because as the user continuously drags the resize
					// bars, a stream of WM_SIZE messages is sent to the window,
					// and it would be pointless (and slow) to reset for each
					// WM_SIZE message received from dragging the resize bars.
					// So instead, we reset after the user is done resizing the
					// window and releases the resize bars, which sends a
					// WM_EXITSIZEMOVE message.
				}
				minOrMaxed = false;
			}
		}
		GetClientRect(mhMainWnd, &clientRect);

		return 0;


		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		GetClientRect(mhMainWnd, &clientRect);
		mD3DPP.BackBufferWidth = clientRect.right;
		mD3DPP.BackBufferHeight = clientRect.bottom;
		onLostDevice();
		HR(gD3DDevice->Reset(&mD3DPP));
		onResetDevice();
		return 0;

		// WM_CLOSE is sent when the user presses the 'X' button in the title line.
	case WM_CLOSE:
		DestroyWindow(mhMainWnd);
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// Keys pressed...
	case WM_KEYDOWN:
		switch (_wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		case VK_PRIOR:
			enableFullScreenMode(true);
			return 0;

		case VK_NEXT:
			enableFullScreenMode(false);
			return 0;
		}
	}

	return DefWindowProc(mhMainWnd, _msg, _wParam, _lParam);
}

void D3DApp::enableFullScreenMode(bool _bEnable)
{
	//keep track of old width/height for when leaving fullscreen mode
	static UINT oldWidth = gWindowWidth;
	static UINT oldHeight = gWindowHeight;
	// Switch to fullscreen mode.
	if (_bEnable)
	{
		// Are we already in fullscreen mode?
		if (!mD3DPP.Windowed)
			return;
		//keep track of old width and height for when you leave fullscreen mode
		oldWidth = gWindowWidth;
		oldHeight = gWindowHeight;

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		mD3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		mD3DPP.BackBufferWidth = width;
		mD3DPP.BackBufferHeight = height;
		mD3DPP.Windowed = false;

		// Change the window style to a more fullscreen friendly style.
		SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_POPUP);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(mhMainWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	// Switch to windowed mode.
	else
	{
		// Are we already in windowed mode?
		if (mD3DPP.Windowed)
			return;

		gWindowWidth = oldWidth;
		gWindowHeight = oldHeight;

		RECT R = { 0, 0, gWindowWidth, gWindowHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		mD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
		mD3DPP.BackBufferWidth = R.right - R.left;
		mD3DPP.BackBufferHeight = R.bottom - R.top;
		mD3DPP.Windowed = true;

		// Change the window style to a more windowed friendly style.
		SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(mhMainWnd, HWND_TOP, 0, 0, R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	// Reset the device with the changes.
	onLostDevice();
	HR(gD3DDevice->Reset(&mD3DPP));

	onResetDevice();
}

void D3DApp::changeWindowResolution(UINT _width, UINT _height)
{
	if (_width == gWindowWidth && _height == gWindowHeight)
		return;
	gWindowWidth = _width;
	gWindowHeight = _height;

	RECT R = { 0, 0, gWindowWidth, gWindowHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	mD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	mD3DPP.BackBufferWidth = R.right - R.left;
	mD3DPP.BackBufferHeight = R.bottom - R.top;
	mD3DPP.Windowed = true;

	// Change the window style to a more windowed friendly style.
	SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

	// If we call SetWindowLongPtr, MSDN states that we need to call
	// SetWindowPos for the change to take effect.  In addition, we 
	// need to call this function anyway to update the window dimensions.
	SetWindowPos(mhMainWnd, HWND_TOP, 0, 0, R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
}

bool D3DApp::isDeviceLost()
{
	// Get the state of the graphics device.
	HRESULT hr = gD3DDevice->TestCooperativeLevel();

	// If the device is lost and cannot be reset yet then
	// sleep for a bit and we'll try again on the next 
	// message loop cycle.
	if (hr == D3DERR_DEVICELOST)
	{
		Sleep(20);
		return true;
	}
	// Driver error, exit.
	else if (hr == D3DERR_DRIVERINTERNALERROR)
	{
		MessageBox(0, L"Internal Driver Error...Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	// The device is lost but we can reset and restore it.
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		onLostDevice();
		HR(gD3DDevice->Reset(&mD3DPP));


		onResetDevice();
		return false;
	}
	else
		return false;
}

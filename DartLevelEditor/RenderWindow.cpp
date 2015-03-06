#include "stdafx.h"
#include "RenderWindow.h"
#include "DartLevelEditor.h"


RenderWindow::RenderWindow()
{
}


RenderWindow::~RenderWindow()
{
}

void RenderWindow::CleanUp()
{
	SAFE_RELEASE(gD3DDevice);
	SAFE_RELEASE(mD3DObject);
}

bool RenderWindow::checkDeviceCaps()
{
	return false;
}

void RenderWindow::onLostDevice()
{
}

void RenderWindow::onResetDevice()
{
}

void RenderWindow::updateScene(float _dt)
{
}

void RenderWindow::drawScene()
{
	HR(gD3DDevice->BeginScene());
	HR(gD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xff020202, 1.0f, 0));

	HR(gD3DDevice->EndScene());
	HR(gD3DDevice->Present(0, 0, 0, 0));

}

void RenderWindow::initMainWindow(HINSTANCE _hInstance)
{
}

void RenderWindow::initDirect3D()
{
	// Step 1: Create the IDirect3D9 object.

	mhMainWnd = theApp.GetMainWnd()->GetSafeHwnd();
	mDevType = D3DDEVTYPE_HAL;
	mRequestedVP = D3DCREATE_HARDWARE_VERTEXPROCESSING;

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
	//GetDlgItem(theApp.GetMainWnd()->GetSafeHwnd(), IDC_DXWINDOW);
	HR(mD3DObject->CreateDevice(D3DADAPTER_DEFAULT, // primary adapter
		mDevType,           // device type
		/*GetDlgItem(mhMainWnd, IDC_DXWINDOW)*/mhMainWnd,          // window associated with device
		devBehaviorFlags,   // vertex processing
		&mD3DPP,            // present parameters
		&gD3DDevice));      // return created device
}

int RenderWindow::run()
{
	return 0;
}

LRESULT RenderWindow::msgProc(UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	return 0;
}

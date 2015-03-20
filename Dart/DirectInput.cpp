//=============================================================================
// DirectInput.cpp
//=============================================================================

#include "D3DUtils.h"
#include "DirectInput.h"
#include "D3DApp.h"
#include "Camera.h"

DirectInput* gDInput = 0;
//constructor
DirectInput::DirectInput(DWORD _keyboardCoopFlags, DWORD _mouseCoopFlags)
{
	//reset mouse and keyboard states
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));
	//create direct3d object
	HR(DirectInput8Create(gD3DApp->getAppInst(), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&mDInput, 0));
	//create keyboard device
	HR(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	HR(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyboard->SetCooperativeLevel(gD3DApp->getMainWnd(), _keyboardCoopFlags));
	HR(mKeyboard->Acquire());
	//create mouse device
	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(gD3DApp->getMainWnd(), _mouseCoopFlags));
	HR(mMouse->Acquire());

	onResetDevice();

	ShowCursor(false);//hide default cursor in window
	//used to keep mouse from clicking too fast
	mMouseClickPause = 0.0f;
}

DirectInput::~DirectInput()
{
	//release  components
	SAFE_RELEASE(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	SAFE_RELEASE(mKeyboard);
	SAFE_RELEASE(mMouse);
	onLostDevice();
}

void DirectInput::onLostDevice()
{
	SAFE_RELEASE(mCursorTex);
}

void DirectInput::onResetDevice()
{
	//mouse cursor
	HR(D3DXCreateTextureFromFileEx(gD3DDevice, L"Content/Textures/cursor.png", 16, 16, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0, 0, 0),
		0, 0, &mCursorTex));
}

void DirectInput::poll()
{
	// Poll keyboard.
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState);
	if (FAILED(hr))
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		// Try to acquire for next time we poll.
		hr = mKeyboard->Acquire();
	}

	// Poll mouse.
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState);
	if (FAILED(hr))
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to acquire for next time we poll.
		hr = mMouse->Acquire();
	}
}
//a key is pressed
bool DirectInput::keyDown(unsigned char _key)
{
	return (mKeyboardState[_key] & 0x80) != 0;
}
//a mouse button is clicked
bool DirectInput::mouseButtonDown(int _button)
{
		return (mMouseState.rgbButtons[_button] & 0x80) != 0;
}
//the amount the mouse moved in the x direction this frame
float DirectInput::mouseDX()
{
	return (float)mMouseState.lX;
}
//the amount the mouse moved in the y direction this frame
float DirectInput::mouseDY()
{
	return (float)mMouseState.lY;
}
//the amount the mouse moved in the z direction this frame
float DirectInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}

void DirectInput::updateMouse(float _dt)
{
	//mouse click pause
	mMouseClickPause -= _dt;
	//update 2D position
	RECT R;
	GetWindowRect(GetForegroundWindow(), &R);
	SetCursorPos(R.left + 100, R.top + 100);//keep system cursor hidden behind window
	mCursorPos2D.x += mMouseState.lX;
	mCursorPos2D.y += mMouseState.lY;
	if (mCursorPos2D.x < 0)					mCursorPos2D.x = 0;
	else if (mCursorPos2D.x >(float)gWindowWidth)	mCursorPos2D.x = (float)gWindowWidth;
	if (mCursorPos2D.y < 0)					mCursorPos2D.y = 0;
	else if (mCursorPos2D.y >(float)gWindowHeight)	mCursorPos2D.y = (float)gWindowHeight;
}
//draw the mouse cursor
void DirectInput::drawMouse(ID3DXSprite* _D3DSprite)
{
	D3DXMATRIX transMat;
	D3DXMatrixIdentity(&transMat);
	D3DXMatrixTranslation(&transMat, mCursorPos2D.x, mCursorPos2D.y, 0);
	HR(_D3DSprite->SetTransform(&transMat));
	HR(_D3DSprite->Draw(mCursorTex, 0, &D3DXVECTOR3(8, 8, 0), 0, D3DCOLOR_ARGB(255, 255, 255, 255)));
}

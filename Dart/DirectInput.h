// Wraps initialization of immediate mode Direct Input, and provides 
// information for querying the state of the keyboard and mouse.
#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtils.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class DirectInput
{
public:
	//constructor
	DirectInput(DWORD _keyboardCoopFlags, DWORD _mouseCoopFlags);
	~DirectInput();//destructor

	void onLostDevice();
	void onResetDevice();

	void poll();//poll for any keys or mouse activated
	bool keyDown(unsigned char key);//a key has been pressed
	bool mouseButtonDown(int button);//a mouse button was clicked
	//the amount that the mouse moves this frame
	float mouseDX();
	float mouseDY();
	float mouseDZ();

	void updateMouse(float _dt);
	//draw the mouse cursor and keep track of position
	void drawMouse();
	D3DXVECTOR2				mCursorPos2D;

private:
	// Make private to prevent copying of members of this class.
	DirectInput(const DirectInput& _rhs);
	DirectInput& operator=(const DirectInput& _rhs);
	//D3D devices needed	
	IDirectInput8*			mDInput;
	//for keyboard
	IDirectInputDevice8*	mKeyboard;
	char					mKeyboardState[256];
	//for mouse
	IDirectInputDevice8*	mMouse;
	DIMOUSESTATE2			mMouseState;
	//mouse cursor
	IDirect3DTexture9*		mCursorTex;
	//keeps mouse from clicking too fast when held down
	float					mMouseClickPause;
};
//global for ease of use
extern DirectInput* gDInput;

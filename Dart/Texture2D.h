#pragma once

#include "D3DUtils.h"
//basic texture class, parent to things like menus, buttons, UI
class Texture2D
{
public:
	//constructor
	Texture2D(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX, int _sizeOrigY,
		float _sizeScreenX, float _sizeScreenY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	Texture2D(){ bInitialized = false;  }
	void init(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX, int _sizeOrigY,
		float _sizeScreenX, float _sizeScreenY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	~Texture2D();
	//draw with global sprite object
	virtual void draw(ID3DXSprite* _sprite);
	virtual void onLostDevice();
	virtual void onResetDevice();
	//textures will only appear when processing, not all buttons will use this
	void setProcessing(bool _process) { bProcess = _process; }
	bool getProcessing()			  { return bProcess; }
	void setPosition(float _x, float _y){ mPositionX = _x; mPositionY = _y; }
	D3DXVECTOR2 getPosition() { return D3DXVECTOR2(mPositionX, mPositionY); }
	bool isInitialized()		{ return bInitialized; }

protected:
	bool				bProcess;
	bool				bInitialized;
	//location of file
	LPCWSTR				mName;
	D3DCOLOR			mColor;
	IDirect3DTexture9*	mTexture;
	//position relative to edges of screen; between 0.0 for left/up and 1.0 for right/down
	float				mPositionX, mPositionY;
	//size of actual texture in pixels
	int					mSizeOrigX, mSizeOrigY;
	//size of texture on screen relative to screen size
	//between 0.0 and 1.0 with 1.0 being that it covers the whole screen
	float				mSizeScreenX, mSizeScreenY;
};
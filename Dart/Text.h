#pragma once

#include "D3DUtils.h"

#include <sstream>
class Text
{
public:
	Text(LPCWSTR _faceName, UINT _height, float _x, float _y,
		D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	~Text();
	void onLostDevice();
	void onResetDevice();
	void draw();
	LPCWSTR getText() { return mText; }
	void setText(LPCWSTR _text) { mText = _text; }
	void setText(UINT _text);
private:
	LPCWSTR mText;
	ID3DXFont*	mFont;
	//these are from 0 to 1, proportionate to the screen
	float		mX;
	float		mY;
	//default white
	D3DCOLOR	mColor;
};
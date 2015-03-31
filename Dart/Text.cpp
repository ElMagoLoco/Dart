#include "Text.h"
#include <tchar.h>
//constructor
Text::Text(LPCWSTR _faceName, UINT _height, float _x, float _y, D3DCOLOR _color)
{
	//set up font
	D3DXFONT_DESC fontDesc;
	fontDesc.Height = _height;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy(fontDesc.FaceName, _faceName);
//	strcpy_s(fontDesc.FaceName, sizeof(fontDesc.FaceName),_T(_faceName));
	
	//create font
	HR(D3DXCreateFontIndirect(gD3DDevice, &fontDesc, &mFont));
	mX = _x;
	mY = _y;
	mColor = _color;
	mText = L"0";
}
//destructor
Text::~Text()
{
	SAFE_RELEASE(mFont);
}

void Text::onLostDevice()
{
	HR(mFont->OnLostDevice());
}

void Text::onResetDevice()
{
	HR(mFont->OnResetDevice());
}

void Text::draw()
{
	RECT* rect = new RECT();
	rect->left = UINT((float)gWindowWidth * mX);
	rect->right = 0;
	rect->top = UINT((float)gWindowHeight * mY);
	rect->bottom = 0;
	HR(mFont->DrawText(0, mText, -1, rect, DT_NOCLIP, mColor));
	delete rect;
}


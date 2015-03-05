#pragma once

#include "D3DUtils.h"
#include "DirectInput.h"
#include "Texture2D.h"

class Button : public Texture2D
{
public:
	Button(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
		float _sizeSX, float _sizeSY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	//this is what happens when the button is clicked; it is overridden for each type of button
	virtual void onClick();
	//this is ran when a click is registered, it will check each active button to see
	//if it's clicked
	bool tryClick();
};
//button to go from main menu -> play game
class ButtonNewGame : public Button
{
public:
	ButtonNewGame(LPCWSTR _texture, float p_ositionX, float _positionY, int _sizeOX, int _sizeOY,
		float _sizeSX, float _sizeSY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void onClick();
private:
};
//exit game
class ButtonExitGame : public Button
{
public:
	ButtonExitGame(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
		float _sizeSX, float _sizeSY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void onClick();
private:
};
//button to go from main menu to options menu
class ButtonOptions : public Button
{
public:
	ButtonOptions(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
		float _sizeSX, float _sizeSY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void onClick();
private:
};

//button to set the resolution to windowed
class Button1024x768 : public Button
{
public:
	Button1024x768(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
		float _sizeSX, float _sizeSY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void onClick();
private:
};
//button to set the resolution to full screen
class ButtonFullScreen : public Button
{
public:
	ButtonFullScreen(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
		float _sizeSX, float _sizeSY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void onClick();
private:
};
//button to go from options screen to main menu
class ButtonGoBack : public Button
{
public:
	ButtonGoBack(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
		float _sizeSX, float _sizeSY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void onClick();
private:

};
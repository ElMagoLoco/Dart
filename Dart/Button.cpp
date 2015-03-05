#include "Button.h"
#include "StateMachine.h"

//for constructor, just call the Texture2D constructor, just passes the data to the 
//parent texture2d class
Button::Button(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Texture2D(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}

//virtual function that is overridden by children
//is ran when the button is clicked
void Button::onClick()
{
	
}

//see if the mouse is in the right spot and run the onClick if it is
bool Button::tryClick()
{
	if (bProcess)
	{
		//min position is upper left of texture2d
		float minX = (float)gWindowWidth  * mPositionX;
		float minY = (float)gWindowHeight * mPositionY;
		//max position is lower right of texture2d
		float maxX = minX + ((float)gWindowWidth * mSizeScreenX);
		float maxY = minY + ((float)gWindowHeight * mSizeScreenY);
		//check to see if the click is within that position
		if (gDInput->mCursorPos2D.x >= minX && gDInput->mCursorPos2D.x <= maxX &&
			gDInput->mCursorPos2D.y >= minY && gDInput->mCursorPos2D.y <= maxY)
		{
			onClick();
			return true;
		}
		//if it returns true, no other buttons will be checked
		return false;
	}
	return false;
}
/***********************************************************
Start New Game
***********************************************************/
//constructors for other buttons just call parent constructor
ButtonNewGame::ButtonNewGame(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}
//go to state play
void ButtonNewGame::onClick()
{
	Button::onClick();
	gStateMachine->transitionState(STATE_PLAY);
}
/***********************************************************
Exit Game
***********************************************************/
//constructors for other buttons just call parent
ButtonExitGame::ButtonExitGame(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}
//exit the game
void ButtonExitGame::onClick()
{
	PostQuitMessage(0);
}
/***********************************************************
Go To Options Menu
***********************************************************/
//call parent constructor
ButtonOptions::ButtonOptions(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}
//go to the options menu
void ButtonOptions::onClick()
{
	Button::onClick();
	gStateMachine->transitionState(STATE_OPTIONSMENU);
}
/***********************************************************
Set Game To Windowed
***********************************************************/
//parent constructor
Button1024x768::Button1024x768(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}
//set windowed mode and size
void Button1024x768::onClick()
{
	Button::onClick();
	gD3DApp->enableFullScreenMode(false);
	gD3DApp->changeWindowResolution(1024, 768);
}
/***********************************************************
Set Game To Full Screen
***********************************************************/
//parent constructor
ButtonFullScreen::ButtonFullScreen(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}
//go to full screen mode
void ButtonFullScreen::onClick()
{
	Button::onClick();
	gD3DApp->enableFullScreenMode(true);
}
/***********************************************************
Go To Main Menu
***********************************************************/
//parent constructor
ButtonGoBack::ButtonGoBack(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}
//go to main menu
void ButtonGoBack::onClick()
{
	Button::onClick();
	bProcess = false;
	gStateMachine->transitionState(STATE_MAINMENU);
}
#include "Button.h"
#include "StateMachine.h"

//for constructor, just call the Texture2D constructor, just passes the data to the 
//parent texture2d class
Button::Button(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Texture2D(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}

//see if the mouse is in the right spot and run the onClick if it is
bool Button::tryClick(bool _left)
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
			if (_left)
				onClickLeft();
			else
				onClickRight();
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
void ButtonNewGame::onClickLeft()
{
	Button::onClickLeft();
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
void ButtonExitGame::onClickLeft()
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
void ButtonOptions::onClickLeft()
{
	Button::onClickLeft();
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
void Button1024x768::onClickLeft()
{
	Button::onClickLeft();
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
void ButtonFullScreen::onClickLeft()
{
	Button::onClickLeft();
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
void ButtonGoBack::onClickLeft()
{
	Button::onClickLeft();
	bProcess = false;
	gStateMachine->transitionState(STATE_MAINMENU);
}

/***********************************************************
Change Attack to Melee
***********************************************************/
//parent constructor
ButtonSetAttackMelee::ButtonSetAttackMelee(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}

void ButtonSetAttackMelee::onClickLeft()
{
	gPlayer->setLeftAttack(A_MELEE);
}

void ButtonSetAttackMelee::onClickRight()
{
	gPlayer->setRightAttack(A_MELEE);
}

/***********************************************************
Change Attack to Seeds
***********************************************************/
//parent constructor
ButtonSetAttackSeed::ButtonSetAttackSeed(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}

void ButtonSetAttackSeed::onClickLeft()
{
	gPlayer->setLeftAttack(A_SEED);
}

void ButtonSetAttackSeed::onClickRight()
{
	gPlayer->setRightAttack(A_SEED);
}

/***********************************************************
Change Attack to Fire
***********************************************************/
//parent constructor
ButtonSetAttackFire::ButtonSetAttackFire(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}

void ButtonSetAttackFire::onClickLeft()
{
	gPlayer->setLeftAttack(A_FIRE);
}

void ButtonSetAttackFire::onClickRight()
{
	gPlayer->setRightAttack(A_FIRE);
}

/***********************************************************
View Story
***********************************************************/
//call parent constructor
ButtonViewStory::ButtonViewStory(LPCWSTR _texture, float _positionX, float _positionY, int _sizeOX, int _sizeOY,
	float _sizeSX, float _sizeSY, D3DCOLOR _color) :
	Button(_texture, _positionX, _positionY, _sizeOX, _sizeOY, _sizeSX, _sizeSY, _color)
{
}
//go to the options menu
void ButtonViewStory::onClickLeft()
{
	Button::onClickLeft();
	gStateMachine->transitionState(STATE_INTRO);
}
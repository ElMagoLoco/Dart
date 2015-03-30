#pragma once

#include <vector>
#include "D3DUtils.h"
#include "Button.h"
#include "Sound.h"

using std::vector;

class Menu : public Texture2D
{
public:
	//the parameters inherited from Texture2D refer to the background image
	Menu(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX, int _sizeOrigY,
		float _sizeScreenX, float _sizeScreenY, D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255));
	~Menu();
	void update(float _dt);//check if any buttons are clicked
	void draw(ID3DXSprite* _sprite);//draw background and buttons
	void drawText();//draw text
	void onLostDevice();
	void onResetDevice();
	void addButton(Button* _newButton);//add a new button
	void addTexture(Texture2D* _newTexture);//add a non-button texture
protected:
	vector<Button*> mButtons;
	vector<Texture2D*> mTextures;
	//Sound* menuKeyPress; //(for later use)
	Sound* menuMouseClick;
};

#pragma once

#include <vector>
using std::vector;
#include "D3DUtils.h"
#include "Menu.h"

//types of stat bars
enum eStatBar{
	STAT_HEALTH_PLAYER = 0,
	STAT_HEALTH_FOLLOWER,
	STAT_AMMO_SEED,
	STAT_AMMO_FIRE
};
//a stat bar like health or ammo
class StatBar : public Texture2D 
{
public:
	StatBar(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX, int _sizeOrigY,
		float _sizeScreenX, float _sizeScreenY, eStatBar _type, 
		D3DCOLOR _color = D3DCOLOR_ARGB(255, 255, 255, 255)) :
		Texture2D(_name, _positionX, _positionY, _sizeOrigX, _sizeOrigY, _sizeScreenX, _sizeScreenY,
		_color), mType(_type), mOrigScreenSizeX(_sizeScreenX)
		{}
	void update(float _dt);
	void draw(ID3DXSprite* _sprite);
private:
	eStatBar mType;
	float mPercentFull;
	//the initial size screen x, keep track of this since the primary one will change
	//when the stat changes
	float mOrigScreenSizeX;
};
//user interface displayed while playing the game
class UserInterface : public Menu
{
public:
	UserInterface(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX, int _sizeOrigY,
		float _sizeScreenX, float _sizeScreenY, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void draw(ID3DXSprite* _sprite);
	void update(float _dt);
	void addStatBar(StatBar* _stat) { mStatBars.push_back(_stat); }
private:
	vector<StatBar*> mStatBars;
	Text* mScoreText;
};
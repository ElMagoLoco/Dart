#include "UserInterface.h"

#include "Player.h"

void StatBar::update(float _dt)
{
	float percentFullPrev = mPercentFull;
	switch (mType)
	{
	case STAT_HEALTH_PLAYER:
		mPercentFull = gPlayer->getHealth() / gPlayer->getHealthMax();
		break;
	case STAT_HEALTH_FOLLOWER:
		mPercentFull = gFollower->getHealth() / gFollower->getHealthMax();
		break;
	case STAT_AMMO_SEED:
		mPercentFull = gPlayer->getAmmoSeeds() / gPlayer->getAmmoSeedsMax();
		break;
	case STAT_AMMO_FIRE:
		mPercentFull = gPlayer->getAmmoFire() / gPlayer->getAmmoFireMax();
		break;
	}
	//if it has changed, figure the texture again
	if (percentFullPrev != mPercentFull)
	{
		mSizeScreenX = mOrigSizeX * mPercentFull;
	}
}

void StatBar::draw(ID3DXSprite* _sprite)
{
	Texture2D::draw(_sprite);
}

UserInterface::UserInterface(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX, int _sizeOrigY,
	float _sizeScreenX, float _sizeScreenY, D3DCOLOR _color) :
	Menu(_name, _positionX, _positionY, _sizeOrigX, _sizeScreenY, _sizeScreenX, _sizeScreenY,
	_color)
{

}

void UserInterface::draw(ID3DXSprite* _sprite)
{
	for (StatBar* S : mStatBars)
		S->draw(_sprite);
	Menu::draw(_sprite);
}
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
		mPercentFull = (float)gPlayer->getAmmoSeeds() / (float)gPlayer->getAmmoSeedsMax();
		break;
	case STAT_AMMO_FIRE:
		mPercentFull = (float)gPlayer->getAmmoFire() / (float)gPlayer->getAmmoFireMax();
		break;
	}
	//if it has changed, figure the texture again
	//if (percentFullPrev != mPercentFull)
	{
		mSizeScreenX = mOrigScreenSizeX * mPercentFull;
	}
}

void StatBar::draw(ID3DXSprite* _sprite)
{
	Texture2D::draw(_sprite);
}

UserInterface::UserInterface(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX,
	int _sizeOrigY, float _sizeScreenX, float _sizeScreenY, D3DCOLOR _color) :
	Menu(_name, _positionX, _positionY, _sizeOrigX, _sizeOrigY, _sizeScreenX, _sizeScreenY,
	_color)
{
	mScoreText = new Text(L"Times New Roman", 30, 0.002f, 0.0f,
		D3DCOLOR_ARGB(255, 255, 0, 0));
	mScoreText->setText(L"0");
}

void UserInterface::update(float _dt)
{
	for (StatBar* SB : mStatBars)
		SB->update(_dt);

	mScoreText->setText(gPlayer->getScore());

	Menu::update(_dt);
}

void UserInterface::draw(ID3DXSprite* _sprite)
{
	Menu::draw(_sprite);
	for (StatBar* S : mStatBars)
		S->draw(_sprite);
	mScoreText->draw();
}
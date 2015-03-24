#include "PickUp.h"

#include "Player.h"
//for constructor just set 1the members
PickUp::PickUp(LPCWSTR _mesh, LPCWSTR _textureName, LPCWSTR _normalName, ePickUpType _type, 
	D3DXVECTOR3 _position, D3DXVECTOR3 _scale, float _amount, float _radius) :
mType(_type), mAmount(_amount), mRadius(_radius), bUsed(false)
{
	mMesh = new Mesh(_mesh, _position, _scale);
	mMesh->addTexture(_textureName, _normalName);
	mMesh->setPosRot(_position, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

PickUp::~PickUp()
{
	if (mMesh) {
		delete mMesh;
		mMesh = NULL;
	}
}

void PickUp::draw()
{
	if (!bUsed)
	{
		mMesh->draw();
	}
}

void PickUp::onTouch(bool _player)
{
	//if already used this one, do nothing
	if (bUsed)
		return;
	//action depends on type
	switch (mType)
	{
	case PICKUP_HEAL:
		if (_player)
			gPlayer->addHealth(mAmount);
		else
			gFollower->addHealth(mAmount);
		bUsed = true;
		break;
	case PICKUP_BONUS:
		//only player can use this one
		if (_player)
		{
			gPlayer->addScore((UINT)mAmount);
			bUsed = true;
		}
		break;
	case PICKUP_AMMO_SEED:
		//only player can use this one
		if (_player)
		{
			gPlayer->addAmmoSeeds((UINT)mAmount);
			bUsed = true;
		}
		break;
	case PICKUP_AMMO_FIRE:
		//only player can use this one
		if (_player)
		{
			gPlayer->addAmmoFire((UINT)mAmount);
			bUsed = true;
		}
		break;
	}
}

void PickUpManager::update(float _dt)
{
	//check each attack and either run its update, or if it is finished then remove it
	list<PickUp*>::iterator it = mPickUps.begin();
	while (it != mPickUps.end())
	{
		bool used = (*it)->getIsUsed();
		if (used)
		{
			mPickUps.erase(it++);
			break; // need to break because erasing can invalidate the iterator or set it back to the beginning
		}

		++it;
	}
}

void PickUpManager::draw()
{
	for (PickUp* P : mPickUps)
	{
		P->draw();
	}
}
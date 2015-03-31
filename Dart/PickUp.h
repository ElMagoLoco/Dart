#pragma once

#include <list>
using std::list;

#include "D3DUtils.h"

#include "Mesh.h"
#include "Sound.h"

//this represents items that either drop from enemies or are otherwise on the ground waiting
//to be picked up
//enum determines what the drop does when  used 
enum ePickUpType{
	PICKUP_HEAL = 0,
	PICKUP_BONUS,
	PICKUP_AMMO_SEED,
	PICKUP_AMMO_FIRE,
	PICKUP_GOAL
};
//this first is the base class for the actual drops
class PickUp
{
public:
	PickUp(LPCWSTR _mesh, LPCWSTR _textureName, LPCWSTR _normalName, char* _pickUpSound,
		ePickUpType _type, 	D3DXVECTOR3 _position, D3DXVECTOR3 _scale, float _amount, 
		float _radius = 32.0f);
	//when the item is touched. _player determines if it was the player or follower that touched
	~PickUp();
	void draw();
	void onTouch(bool _player);
	bool getIsUsed() { return bUsed; }
	Mesh* getMesh() { return mMesh; }
	float getRadius() { return mRadius; }
protected:
	Mesh* mMesh;//mesh used to display
	ePickUpType mType;//type of pickup
	//amount associated with the type of pickup
	//this could be amount of health healed, amount of bonus points received, or amount
	//of ammo received
	float mAmount;
	//how close the player or follower has to be to pick it up
	float mRadius;
	//if it has been picked up, this will tell the pickup organizer when to remove it from the list
	bool bUsed;
	//sound played when picked up
	Sound* sndUsed;
};

//manages a list of pickups for the level
class PickUpManager
{
public:
	list<PickUp*>& getPickUps() { return mPickUps; }
	void addPickUp(PickUp* _pickup) { mPickUps.push_back(_pickup); }
	void update(float _dt);
	void draw();
private:
	list<PickUp*> mPickUps;
};
#pragma once

#include "D3DUtils.h"
#include "Mesh.h"
#include "PathNode.h"

//pawn is base class for player, follower, and enemies, so it has the things that are common 
//to all of them. 
class Pawn
{
public:
	//constructor
	Pawn(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName, 
		D3DXVECTOR3 _startPosition, float _healthMax, float _radius,
		D3DXVECTOR3 _meshScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	~Pawn();

	//get functions
	float getHealth()		{ return mHealth; }
	float getHealthMax()	{ return mHealthMax; }
	float getIsDead()		{ return bIsDead; }
	float getRadius()		{ return mRadius; }
	bool getJustAttacked()	{ return mAttackTime < mAttackDelay; }//did player just attack
	D3DXVECTOR3 getPosition()	{ return mPosition; }
	//add health, add negative to remove health
	void addHealth(float _amount);
	//draw the pawn
	void draw();
	//update, also call this from child class update functions
	void update(float _dt);
protected:
	AnimMesh* mMesh;
	//health
	float mHealth;
	float mHealthMax;
	bool bIsDead;
	//movement
	D3DXVECTOR3 mPosition;//where it is
	D3DXVECTOR3 mLastPosition;//where it was
	D3DXVECTOR3 mVelocity;//direction of movement
	float mSpeed;//speed in velocity direction, multiplied by normalized velocity to get actual movement
	D3DXVECTOR3 mRotation;//rotation
	//attacks
	float mAttackTime; //time since they last attacked
	float mAttackDelay;//how often can attack
	//collision radius
	float mRadius;
	///is moving
	bool bIsMoving;
};

//attacks that the player can use
enum ePAttack{
	A_NONE,
	A_MELEE,
	A_SEED,
	A_FIRE
};


class Player : public Pawn
{
public:
	Player(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName, 
		D3DXVECTOR3 _startPosition, float _healthMax, float _radius,
		D3DXVECTOR3 _meshScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void update(float _dt);
	void addScore(UINT _add) { mScore += _add; }
	void addAmmoSeeds(UINT _amount){ mAmmoSeeds = min(mAmmoSeedsMax, mAmmoSeeds + _amount); }
	void addAmmoFire(UINT _amount){ mAmmoSeeds = min(mAmmoFireMax, mAmmoFire + _amount); }
	UINT getAmmoSeeds() { return mAmmoSeeds; }
	UINT getAmmoSeedsMax() { return mAmmoSeedsMax; }
	UINT getAmmoFire() { return mAmmoFire; }
	UINT getAmmoFireMax() { return mAmmoFireMax; }
	ePAttack getLeftAttack() { return mLeftAttack; }
	ePAttack getRightAttack() { return mRightAttack; }
	void setLeftAttack(ePAttack attack) { mLeftAttack = attack; }
	void setRightAttack(ePAttack attack) { mRightAttack = attack; }
private:
	//ammo
	UINT mAmmoSeeds;
	UINT mAmmoSeedsMax;
	UINT mAmmoFire;
	UINT mAmmoFireMax;
	//score
	UINT mScore;
	//attacks that are equipped to left and right mouse buttons
	ePAttack mLeftAttack;
	ePAttack mRightAttack;
};
extern Player* gPlayer;
//follower states, also used for enemy.h
enum pState{
	PSTATE_PURSUE = 0,//follow or pursue the player/follower 
	PSTATE_INJURED,//unit is searching for a source of healing
	PSTATE_AFRAID,//panicked, fleeing
	PSTATE_WANDER//wandering aimlessly, not following anything
};
//closest it gets to player using path finding, any closer just uses direction
//this is squared to make math easier
const static float FOLLOW_DISTANCE_SQ = 40000.0f;//actual distance 200.0f
//distance from player in which the follower stops
const static float FOLLOW_DISTANCE_STOP_SQ = 22500.0f;//150.0f
//the minimum time between making new paths
const static float MIN_PATH_TIME = 1.5f;
//how often does AI run; increases framerate
const static float RUN_AI = 0.02f;//~50 per second

class Follower : public Pawn
{
public:
	Follower(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
		D3DXVECTOR3 _startPosition, float _healthMax, float _radius,
		D3DXVECTOR3 _meshScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void update(float _dt);
	//set path finding to player
	void setPathPlayer();
	//set path to random flee point if panicked
	void setPathFlee();
	//see mStartNode below
	void resetStartNode();
	//follow the presently built path. _speed is the percent of top speed to use, like .75 for 3/4 speed
	//follow the path, return false if you are out of path to follow
	bool followPath(float _speed = 1.0f);
	//stop all movement
	void stop();
	//face the direction it is going
	void pointForward(float _dt);
	//is afraid or panicked
	bool getIsAfraid() { return bAfraid; }
	void setIsAfraid(bool _fear) { bAfraid = _fear;  }
	//add health, add negative to remove health, additionally will check if becomes afraid
	void addHealth(float _amount);
protected:
	pState mState;//state it is in
	vector<D3DXVECTOR3> mPath;//path it is currently following
	//temporary node, this is placed at the beginning of their path so that they have a place
	//to start from without jerking over to the nearest existing node
	PathNode* mStartNode;
	//last path found, the delta time since the last path finding action
	float mLastPathFound;
	//is follower afraid or panicking
	bool bAfraid;
	//time they have spent being afraid
	float mAfraidTime;
	//max time follower will remain afraid
	float mAfraidMax;
	//the health level where we start checking to see if it is afraid, also used for enemies
	float mFearHealth;
};
extern Follower* gFollower;
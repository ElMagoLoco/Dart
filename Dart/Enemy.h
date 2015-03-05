#pragma once
#include <list>
using std::list;

#include "Attack.h"
#include "Player.h"//pawn and follower

//enemy is a child of follower instead of pawn because follower contains a number of things
//that will be duplicated in enemy, mostly involving path finding
//this is the parent class to each individual type of enemy

const static float LOSE_SIGHT = 1.0f;//time it takes for enemy to lose sight of player/follower

class Enemy : public Follower
{
public:
	Enemy(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
		D3DXVECTOR3 _startPosition, float _healthMax, float _radius, 
		D3DXVECTOR3 _meshScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	~Enemy();
	void update(float _dt);
	float getPoints() { return mPoints; }
	//attack toward a position.
	void attack(D3DXVECTOR3 _position);
	//can the enemy see or hear the player? if seeing, is there line of sight to the player?
	//we pass the distance to the player, so it only has to be figured once during this frame
	bool noticePlayer();
	bool noticeFollower();
	//additional path finding
	void setPathRandom();
	void setPathFollower();
protected:
	//has the enemy seen the player. When it sees the player, it's point value 
	//starts to decrease
	bool bSeenPlayer;
	//does the enemy presently see the player
	//represents if they could see the player the previous frame
	bool bSeesPlayer;
	//can fire on player/follower if true
	bool bAttackPlayer;
	bool bAttackFollower;
	//who are they currently attacking? true for player, false for follower
	bool bPursuingPlayer;
	//present point value
	float mPoints;
	//how fast points drain once it notices the player
	float mPointDrain;
	//the minimum that the point value will go down to
	float mMinPoints;
	//attack
	bool mRanged; //is their attack ranged
	float mAttackDistanceSq;//how close they must be to attack
	float mClosestDistanceSq;//the closest they will come before stopping
	//ranges
	float mSightRangeSq;//how far the enemy can see (squared)
	float mHearAttackRangeSq;//how far away they can hear the player attacking
	float mHearRangeSq;//how far away they can hear the player when player isn't attacking
	//time it takes to lose sight of the player when they are no longer noticed
	float mLoseSightPlayer;
	float mLoseSightFollower;
	//the attack that the enemy uses. Copy this when they make an attack
	Attack* mAttack;
};



//sub classes of enemy will mostly just be constructors with variable setting. . All else will be in the main
//enemy class

//ants are a basic enemy with a melee attack
class EnemyAnt : public Enemy
{
public:
	EnemyAnt(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
		D3DXVECTOR3 _startPosition, float _healthMax, float _radius,
		D3DXVECTOR3 _meshScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void attack(D3DXVECTOR3 _position);//attack towards a position
private:
};


#include <vector>
using std::vector;

//a group of enemies that travel together
class EnemyGroup
{
public:
	void update(float _dt);
	void draw();
	list<Enemy*> getEnemies()		{ return mEnemies; }
private:
	list<Enemy*> mEnemies;
};

//enemy spawner handles all the enemies for the current level
//it will keep track of individual enemies and enemy groups
class EnemySpawner
{
public:
	void update(float _dt);
	void draw();
	list<EnemyGroup*> getGroups()	{ return mGroups; }
	list<Enemy*> getEnemies()		{ return mEnemies; }
	void addGroup(EnemyGroup* _group) { mGroups.push_back(_group); }
	void addEnemy(Enemy* _enemy, UINT _numToAdd = 1)
	{
		for (UINT i = 0; i < _numToAdd; ++i)
		{
			Enemy* e = new Enemy(*_enemy);
			mEnemies.push_back(e);
		}
	}
private:
	list<EnemyGroup*> mGroups;
	list<Enemy*> mEnemies;
};
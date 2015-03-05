#pragma once

#include <list>
using std::list;

#include "D3DUtils.h"

#include "Mesh.h"

//these are for melee and projectile attacks, whether used by a player or enemy
//to make it a melee attack, just do not pass it a mesh and make it's lifetime short

class Attack
{
public:
	//leave _mesh as "" if attack is not visible (like a melee attack might be)
	Attack(LPCWSTR _mesh, float _damage, float _speed, float _lifetime, bool _isPlayers,
		float _radius = 10.0f,	D3DXVECTOR3 _scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	~Attack();
	//set textures
	void setTextures(LPCWSTR _texture, LPCWSTR _normalTex){
		mMesh->addTexture(_texture, _normalTex);
	}
	//initial movement
	void moveFacingDirection();//move in facing direction of player
	void moveTowardPlayer(D3DXVECTOR3& _origin);//move towards the player
	void moveTowardFollower(D3DXVECTOR3& _origin);//move toward follower
	void moveTowardPoint(D3DXVECTOR3& _origin, D3DXVECTOR3& _target);//to specific point
	//update movement/collision
	void update(float _dt);
	//draw if applicable
	void draw();
	bool getIsAlive()	{ return bIsAlive; }
private:
	Mesh* mMesh;
	//position
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mVelocity;
	D3DXVECTOR3 mRotation;
	float mSpeed;
	//collision radius
	float mRadius;
	//if it is the player's or not, determines who it collides with
	bool bIsPlayers;
	//if it is still active or not
	bool bIsAlive;
	float mDamage;//damage it causes
	float mLifetime;//how long it 'lives' if it doesn't collide
	float mLived;//how long it has lived so far
};
//handles all the projectiles and attacks that are currently in process
class AttackManager
{
public:
	void addAttack(Attack* _attack) { mAttacks.push_back(_attack); }
	void update(float _dt);
	void draw();
private:
	list<Attack*> mAttacks;
};
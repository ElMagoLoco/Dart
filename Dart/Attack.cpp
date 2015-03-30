#include "Attack.h"

#include "Bounds.h"
#include "Level.h"
#include "Player.h"

Attack::Attack(LPCWSTR _mesh, float _damage, float _speed, float _lifetime, bool _isPlayers, float _radius, D3DXVECTOR3 _scale) :
	mDamage(_damage), mSpeed(_speed), mLifetime(_lifetime), mLived(0.0f),
	bIsPlayers(_isPlayers), mRadius(_radius), mRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), 
	bIsAlive(true)
{
	if (_mesh != L"")
		mMesh = new Mesh(_mesh, D3DXVECTOR3(0.0f, 0.0f, 0.0f), _scale);
	else
		mMesh = NULL;
	//make sure to set initial speed and direction using one of the movement functions

	FR(gSound->getSystem()->createSound("Content/Audio/sndHitRock", FMOD_DEFAULT, 0, &HitRock);
	FR(gSound->getSystem()->createSound("Content/Audio/sndHitWallMelee", FMOD_DEFAULT, 0, &HitWallMelee);
	FR(gSound->getSystem()->createSound("Content/Audio/sndHitWallSeed", FMOD_DEFAULT, 0, &HitWallSeed);
	FR(gSound->getSystem()->createSound("Content/Audio/sndHitWood", FMOD_DEFAULT, 0, &HitWood);


}

Attack::~Attack()
{
	if (mMesh != NULL)
		delete mMesh;
}

//move in the facing direction of the player
void Attack::moveFacingDirection()
{
	mPosition = gPlayer->getPosition();
	//assumes player remains in middle of screen
	D3DXVECTOR3 direction = D3DXVECTOR3(
		(gDInput->mCursorPos2D.x) - ((float)gWindowWidth / 2.0f), 
		0.0f,
		((float)gWindowHeight / 2.0f) - gDInput->mCursorPos2D.y);
	D3DXVec3Normalize(&direction, &direction);
	mVelocity = direction * mSpeed;
}
//move towards player
void Attack::moveTowardPlayer(D3DXVECTOR3& _origin)
{
	mPosition = _origin;
	D3DXVECTOR3 direction = (gPlayer->getPosition() - _origin);
	D3DXVec3Normalize(&direction, &direction);
	mVelocity = direction * mSpeed;
}

//move towards follower
void Attack::moveTowardFollower(D3DXVECTOR3& _origin)
{
	mPosition = _origin;
	D3DXVECTOR3 direction = (gFollower->getPosition() - _origin);
	D3DXVec3Normalize(&direction, &direction);
	mVelocity = direction * mSpeed;
}

void Attack::moveTowardPoint(D3DXVECTOR3& _origin, D3DXVECTOR3& _target)
{
	mPosition = _origin;
	D3DXVECTOR3 direction = _target - _origin;
	D3DXVec3Normalize(&direction, &direction);
	mVelocity = direction * mSpeed;
}

void Attack::update(float _dt)
{
	if (!bIsAlive)
		return;
	//update movement
	mPosition += (mVelocity * _dt);
	//if there is a mesh, spin the mesh
	if (mMesh != NULL)
	{
		mRotation += (D3DXVECTOR3(10.0f, 10.0f, 10.0f) * _dt);
		//set mesh to position
		mMesh->setPosRot(mPosition, mRotation);
	}
	//if its life runs out, destroy
	mLived += _dt;
	if (mLived >= mLifetime)
	{
		bIsAlive = false;
		return;
	}
	//if it collides with the environment, destroy
	bool colliding = false;
	//for each mesh in current level
	for (Mesh* M : gCurrentLevel->getWorldGeometry())
	{
		//for each AABB in mesh
		for (AxisAlignedBoundingBox AABB : M->getBoundsBoxList())
		{
			//if the AABB blocks projectiles
			if (AABB.bBlocksProjectiles)
			{
				//see if it collides
				BoundingSphere circle = BoundingSphere(mPosition, mRadius);
				colliding = collides(AABB, circle);
				//if it collided, break
				if (colliding) break;
			}
		}
		if (colliding) break;
	}
	//kill it if it collided
	if (colliding)
	{
		bIsAlive = false;
		return;
	}
	if (!bIsPlayers)
	{
		//if it doesn't belong to the player and it hits the player
		if (collides(BoundingSphere2D(D3DXVECTOR2(mPosition.x, mPosition.z), mRadius),
			BoundingSphere2D(D3DXVECTOR2(gPlayer->getPosition().x, gPlayer->getPosition().z), gPlayer->getRadius())))
		{
		gPlayer->addHealth(-mDamage);
		bIsAlive = false;
		return;
		}
		//if it doesn't belong to the player and it hits the follower
		if (collides(BoundingSphere2D(D3DXVECTOR2(mPosition.x, mPosition.z), mRadius),
			BoundingSphere2D(D3DXVECTOR2(gFollower->getPosition().x, gFollower->getPosition().z), gFollower->getRadius())))
		{
			gFollower->addHealth(-mDamage);
			bIsAlive = false;
			return;
		}
	}
	//if it belongs to the player and hits an enemy
	else
	{
		for (Enemy* E : gCurrentLevel->getEnemies())
		{
			if (collides(BoundingSphere2D(D3DXVECTOR2(mPosition.x, mPosition.z), mRadius),
				BoundingSphere2D(D3DXVECTOR2(E->getPosition().x, E->getPosition().z), E->getRadius())))
			{
				E->addHealth(-mDamage);
				bIsAlive = false;
				return;
			}
		}
		for (EnemyGroup* EG : gCurrentLevel->getGroups())
		{
			for (Enemy* E : EG->getEnemies())
			{
				if (collides(BoundingSphere2D(D3DXVECTOR2(mPosition.x, mPosition.z), mRadius),
					BoundingSphere2D(D3DXVECTOR2(E->getPosition().x, E->getPosition().z), E->getRadius())))
				{
					E->addHealth(-mDamage);
					bIsAlive = false;
					return;
				}
			}
		}
	}
}

void Attack::draw()
{
	//lived check prevents strange graphics glitch when projectiles start to fly
	if (mLived > 0.1f && mMesh != NULL)
		mMesh->draw();
}

void AttackManager::update(float _dt)
{
	//check each attack and either run its update, or if it is finished then remove it
	list<Attack*>::iterator it = mAttacks.begin();
	while (it != mAttacks.end())
	{
		bool alive = (*it)->getIsAlive();
		if (alive)
		{
			(*it)->update(_dt);
			++it;
		}
		else
		{
			mAttacks.erase(it++);
		}
	}
}

void AttackManager::draw()
{
	for (Attack* A : mAttacks)
	{
		A->draw();
	}
}
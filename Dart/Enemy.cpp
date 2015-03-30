#include "Enemy.h"

#include "Level.h"

Enemy::Enemy(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
	D3DXVECTOR3 _startPosition, float _healthMax, float _radius, D3DXVECTOR3 _meshScale) :
	Follower(_meshName, _textureName, _normalTexName, _startPosition, _healthMax,
	_radius, _meshScale), mLoseSightPlayer(10.0f), mLoseSightFollower(10.0f)
{
	mState = PSTATE_WANDER;

	FR(gSound->getSystem()->createSound("Content/Audio/sndEnemy1Attack", FMOD_DEFAULT, 0, &enemyAttack));
	FR(gSound->getSystem()->createSound("Content/Audio/sndEnemy1Death", FMOD_DEFAULT, 0, &enemyDeath));
	FR(gSound->getSystem()->createSound("Content/Audio/sndEnemy1GetHit", FMOD_DEFAULT, 0, &enemyGetHit));
}

Enemy::~Enemy()
{
	Pawn::~Pawn();
	if (mAttack != NULL)
		delete mAttack;
}

void Enemy::update(float _dt)
{
	if (bIsDead == true)
		return;
	Pawn::update(_dt);
	//if it has seen the player/follower, decrease its point value over time
	if (bSeenPlayer && mPoints > mMinPoints)
	{
		mPoints -= (mPointDrain * _dt);
		mPoints = max(mMinPoints, mPoints);//if it dropped below min value, push it back up
	}


	mLastPathFound += _dt;//when last path was found
	mAttackTime += _dt;//when last attack was made
	mLoseSightPlayer += _dt;
	mLoseSightFollower += _dt;
	D3DXVECTOR3 targetPosition;
/*	static float aiCheck = 0.0f;
	aiCheck += _dt;
	if (aiCheck >= RUN_AI)//only run the AI every so many frames*/
		//inside this conditional you should use aiCheck where you'd normally use _dt
	{
		switch (mState)
		{
		case PSTATE_WANDER:
			//if they have recently seen player/follower or see them now
			if (mLoseSightPlayer < LOSE_SIGHT || mLoseSightFollower < LOSE_SIGHT ||
				noticeFollower() || noticePlayer())
			{
				if (bAfraid)//if afraid, run
				{
					mState = PSTATE_AFRAID;
					break;
				}
				else
				{
					chooseTarget();
					mState = PSTATE_PURSUE;
					gSound->getSystem()->playSound(FMOD_CHANNEL_FREE, enemyAttack, false, 0);
					break;
				}
			}
			else //if they can't see the player, just wander
				//unless low on health, in which case seek a heal spot
			{
				if (mHealth == mHealthMax)
				{
					if (!followPath(0.75f) && mLastPathFound > MIN_PATH_TIME)
					{
						setPathRandom();
						break;
					}
				}
				else
				{
					mState = PSTATE_INJURED;
					bHealing = false;
					gSound->getSystem()->playSound(FMOD_CHANNEL_FREE, enemyGetHit, false, 0);
					break;
				}
			}
			break;
		case PSTATE_PURSUE:
			//if they no longer see the target, go back to wandering state
			// if pursuing player and doesn't see player and hasn't seen them for a while
			if ((bPursuingPlayer && !noticePlayer() && mLoseSightPlayer >= LOSE_SIGHT) ||
				//if pursuing follower and doesn't see them and hasn't seen them for a while
				(!bPursuingPlayer && !noticeFollower() && mLoseSightFollower >= LOSE_SIGHT))
			{
				mState = PSTATE_WANDER;
				break;
			}
			//if can see their target, but is afraid
			if (bAfraid)
			{
				mState = PSTATE_AFRAID;
				break;
			}
			//otherwise they can see and aren't afraid

			//stop if they are too close to the target, otherwise try to get closer
			if (bPursuingPlayer) targetPosition = gPlayer->getPosition();
			else                 targetPosition = gFollower->getPosition();

			if ((D3DXVec3LengthSq(&(mPosition - targetPosition))) <= mClosestDistanceSq)
			{
				mVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			else
			{
				if (mLastPathFound >= MIN_PATH_TIME || !followPath(1.0f))
				{
					if (bPursuingPlayer) setPathPlayer();
					else                setPathFollower();
				}
			}
			//attack if it can
			if (mAttackTime > mAttackDelay)
			{
				if (bPursuingPlayer)
				{
					if (bAttackPlayer)
					{
						attack(gPlayer->getPosition());
						mAttackTime = 0.0f;
						gSound->getSystem()->playSound(FMOD_CHANNEL_FREE, enemyAttack, false, 0);
					}
				}
				else
				{
					if (bAttackFollower)
					{
						attack(gFollower->getPosition());
						mAttackTime = 0.0f;
						gSound->getSystem()->playSound(FMOD_CHANNEL_FREE, enemyAttack, false, 0);
					}
				}
			}
			break;
		case PSTATE_AFRAID:
			//if they reached good health again, they are no longer afraid
			//the add health function will take away fear state when they reach proper hp
			if (!bAfraid)
			{
				mState = PSTATE_WANDER;
				break;
			}
			//if they no longer see the target, go back to wandering state
			// if pursuing player and doesn't see player and hasn't seen them for a while
			if ((bPursuingPlayer && !noticePlayer() && mLoseSightPlayer >= LOSE_SIGHT) ||
				//if pursuing follower and doesn't see them and hasn't seen them for a while
				(!bPursuingPlayer && !noticeFollower() && mLoseSightFollower >= LOSE_SIGHT))
			{
				mState = PSTATE_INJURED;
				break;
			}
			if (mLastPathFound >= MIN_PATH_TIME || !followPath(1.0f))
			{
				setPathFlee();
			}
			//still attack if they are close enough
			if (mAttackTime >= mAttackDelay)
			{
				if (bPursuingPlayer)
				{
					if (bAttackPlayer)
					{
						attack(gPlayer->getPosition());
						mAttackTime = 0.0f;
						gSound->getSystem()->playSound(FMOD_CHANNEL_FREE, enemyAttack, false, 0);
					}
				}
				else
				{
					if (bAttackFollower)
					{
						attack(gFollower->getPosition());
						mAttackTime = 0.0f;
						gSound->getSystem()->playSound(FMOD_CHANNEL_FREE, enemyAttack, false, 0);
					}
				}
			}
			break;
		case PSTATE_INJURED://enemy is injured and cannot presently see player/wanderer
			//if they have recently seen player/follower or see them now
			if (mLoseSightPlayer < LOSE_SIGHT || mLoseSightFollower < LOSE_SIGHT ||
				noticeFollower() || noticePlayer())
			{
				//if they are afraid, flee. If not, pursue
				if (bAfraid)
				{
					mState = PSTATE_AFRAID;
					break;
				}
				else
				{
					chooseTarget();
					mState = PSTATE_PURSUE;
					gSound->getSystem()->playSound(FMOD_CHANNEL_FREE,enemyAttack, false, 0);
					break;
				}
			}
			else//if they don't see the player or follower, find a healing spot to recover
			{
				if (mHealth == mHealthMax)
				{//when fully healed, go back to wandering
					mState = PSTATE_WANDER;
					break;
				}
				if (bHealing)
				{
					stop();
					if (mHealth == mHealthMax)
					{
						bHealing = false;
						mState = PSTATE_WANDER;
						break;
					}
				}
				else
				{
					if (mLastPathFound >= MIN_PATH_TIME || !followPath(1.0f))
					{
						setPathHealing();
					}
				}
			}
			break;
		}
		//rotate to the direction you're going
		pointForward(_dt);
		//update position
		mPosition += mVelocity * _dt;
		mMesh->setPosRot(mPosition, mRotation);
	}
}

bool Enemy::noticePlayer()
{
	bool returnBool = false;
	D3DXVECTOR3 playerPos = gPlayer->getPosition();
	float distanceSq = D3DXVec3LengthSq(&(mPosition - playerPos));
	if (distanceSq > mSightRangeSq)//if not within range, they can't see player
	{
		bAttackPlayer = false;
		return false;
	}
	//if player has fired within hearing range of enemy
	if (distanceSq < mHearAttackRangeSq && gPlayer->getJustAttacked())
		returnBool = true;
	// they are close enough to hear the player through a wall or behind them
	if (distanceSq < mHearRangeSq)
		returnBool = true;
	//if the player would be behind the enemy, they don't get a chance to see them
	//current enemy rotation is mRotation.y
	//find rotation towards player
	D3DXVECTOR3 toPlayer = gPlayer->getPosition() - mPosition;
	float playerDirection = atan2(toPlayer.x, toPlayer.z);
	//if not within VISION_RANGE degrees/PI radians of each other
	//this only applies if the player has not injured them. If player has, they are more 
	//watchful
	if (mHealth == mHealthMax && returnBool == false)
	{
		//1.57 ~ 1/2 * PI      6.28 ~ 2 * PI
		if ((fabs(playerDirection - mRotation.y) > 1.57) &&
			((fabs(playerDirection - 6.28)) + mRotation.y > 1.57) &&
			((fabs(mRotation.y - 6.28)) + playerDirection > 1.57))
		{
			bAttackPlayer = false;
			return false;
		}
	}
	//if they are between max hearing and max sight range, see if they are line of sight to player
	//make a line segment between enemy and player location
	//only take into account x and z here since map is flat, will save time
	/*LineSegment3D line(mPosition, playerPos);*/
	// SAM
	LineSegment line3D(mPosition, playerPos);
	for (Mesh* M : gCurrentLevel->getWorldGeometry())
	{
		for (AxisAlignedBoundingBox AABB : M->getBoundsBoxList())
		{
			//if further out than sight range, no point in checking it
			if (D3DXVec3LengthSq(&(AABB.mMin - mPosition)) > mSightRangeSq)
				continue;
			//see if it collides with the line
			if (collides(AABB, line3D))
			{
				bAttackPlayer = false;
				return returnBool;
			}
		}

		// SAM
		/*for (UINT i = 0; i < M->getBoundsBoxList().size(); ++i)	{
			//if further out than sight range, no point in checking it
			if (D3DXVec3LengthSq(&(M->getBoundsBoxList()[i].mMin - D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z))) > mSightRangeSq)
				continue;
			//see if it collides with the line
			if (collides(M->getBoundsBoxList()[i], line3D))
			{
				bAttackPlayer = false;
				return returnBool;
			}
		}*/
	}
	//if it gets through the above checks, then it can see the player
	mLoseSightPlayer = 0.0f;
	bAttackPlayer = true;
	bSeenPlayer = true;
	return true;
}

bool Enemy::noticeFollower()
{
	bool returnBool = false;
	D3DXVECTOR3 followerPos = gFollower->getPosition();
	float distanceSq = D3DXVec3LengthSq(&(mPosition - followerPos));
	if (distanceSq > mSightRangeSq)//if not within range, they can't see follower
	{
		bAttackFollower = false;
		return false;
	}
	// they are close enough to hear the follower through a wall or behind them
	if (distanceSq < mHearRangeSq)
		returnBool = true;
	//if the follower would be behind the enemy, they don't get a chance to see them
	//current enemy rotation is mRotation.y
	//find rotation towards follower
	D3DXVECTOR3 toFollower = gFollower->getPosition() - mPosition;
	float followerDirection = atan2(toFollower.x, toFollower.z);
	//if not within VISION_RANGE degrees/PI radians of each other
	//this only applies if the player has not injured them. If player has, they are more 
	//watchful
	if (mHealth == mHealthMax && returnBool == false)
	{
		//1.57 ~ 1/2 * PI      6.28 ~ 2 * PI
		if ((fabs(followerDirection - mRotation.y) > 1.57) &&
			((fabs(followerDirection - 6.28)) + mRotation.y > 1.57) &&
			((fabs(mRotation.y - 6.28)) + followerDirection > 1.57))
		{
			bAttackFollower = false;
			return false;
		}
	}
	//if they are between max hearing and max sight range, see if they are line of sight to player
	//make a line segment between enemy and player location
	//only take into account x and z here since map is flat, will save time
	LineSegment line3D(mPosition, followerPos);
	for (Mesh* M : gCurrentLevel->getWorldGeometry())
	{
		for (AxisAlignedBoundingBox AABB : M->getBoundsBoxList())
		{
			//if further out than sight range, no point in checking it
			if (D3DXVec3LengthSq(&(AABB.mMin - mPosition)) > mSightRangeSq)
				continue;
			//see if it collides with the line
			if (collides(AABB, line3D))
			{
				bAttackPlayer = false;
				return returnBool;
			}
		}
	}
	//if it gets through the above checks, then it can see the player
	mLoseSightFollower = 0.0f;
	bAttackFollower = true;
	bSeenPlayer = true;
	return true;
}

//set present path towards a random node
void Enemy::setPathRandom()
{
	vector<D3DXVECTOR3>().swap(mPath);//clean out old vector
	//get random node as a goal
	vector<PathNode*> nodeList = gCurrentLevel->getPaths()->getPathNodes();
	PathNode* randomNode = nodeList[rand() % nodeList.size()];
	//find new path to goal
	resetStartNode();
	mPath = gCurrentLevel->getPaths()->findPath(mPosition, randomNode);
	//path just found
	mLastPathFound = 0.0f;
}

void Enemy::setPathFollower()
{
	vector<D3DXVECTOR3>().swap(mPath);//clean out vector
	//find new path to player position
	resetStartNode();
	mPath = gCurrentLevel->getPaths()->findPath(mStartNode, gFollower->getPosition());
	//path just found
	mLastPathFound = 0.0f;
}

void Enemy::setPathHealing()
{
	vector<D3DXVECTOR3>().swap(mPath);//clean out vector
	//find new path to nearest healing spot
	float nearestSq = 1000000000000.0f;
	D3DXVECTOR2 nearestV = D3DXVECTOR2(0.0f, 0.0f);
	for (D3DXVECTOR2 V : gCurrentLevel->getHealPoints())
	{
		float distance = D3DXVec2LengthSq(&(V - D3DXVECTOR2(mPosition.x, mPosition.z)));
		if (distance < nearestSq)
		{
			nearestSq = distance;
			nearestV = V;
		}
	}
	if (nearestSq < 400)
	{
		bHealing = true;
		stop();
		return;
	}
	resetStartNode();
	mPath = gCurrentLevel->getPaths()->findPath(mStartNode, D3DXVECTOR3(nearestV.x, 50.0f, nearestV.y));
	//path just found
	mLastPathFound = 0.0f;
}

void Enemy::attack(D3DXVECTOR3 _position)
{
	Attack* attack = new Attack(*mAttack);
	attack->moveTowardPoint(mPosition, _position);
	gCurrentLevel->getAttackManager()->addAttack(attack);
}

void Enemy::chooseTarget()
{
	if (!bAttackPlayer)
	{//if only see the follower, attack them
		bPursuingPlayer = false;
	}
	else if (!bAttackFollower)
	{//if only sees the player, attack them
		bPursuingPlayer = true;
	}
	else//else choose randomly between them
	{
		if (getRandomFloat(0.0f, 1.0f) < 0.5f)
			bPursuingPlayer = true;
		else
			bPursuingPlayer = false;
	}
}


EnemyMelee::EnemyMelee(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
	D3DXVECTOR3 _startPosition, float _healthMax, float _radius, D3DXVECTOR3 _meshScale) :
	Enemy(_meshName, _textureName, _normalTexName, _startPosition, _healthMax,
	_radius, _meshScale)
{
	mRanged = false;
	mAttackDistanceSq = 10000.0f;//100
	mClosestDistanceSq = 6500.0f;//80
	mAttackDelay = 1.0f;
	mFearHealth = mHealthMax * .667f;
	mPoints = 100.0f;
	mPointDrain = 1.0f;
	mMinPoints = 50.0f;
	mSightRangeSq = 1000000.0f;//1000
	mHearAttackRangeSq = 62500.0f;//250
	mHearRangeSq = 10000.0f;//100
	mLoseSightPlayer = 1.0f;
	mLoseSightFollower = 1.0f;
	mSpeed = 300.0f;
	mAttack = new Attack(L"Content/Models/ball.x", 10.0f, 400.0f, 0.1f, false);
	mAttack->setTextures(L"Content/Textures/tex_fire.dds", L"Content/Textures/tex_fire_n.dds");
}

EnemySeeds::EnemySeeds(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
	D3DXVECTOR3 _startPosition, float _healthMax, float _radius, D3DXVECTOR3 _meshScale) :
	Enemy(_meshName, _textureName, _normalTexName, _startPosition, _healthMax,
	_radius, _meshScale)
{
	mRanged = true;
	mAttackDistanceSq = 4000000.0f;//2000
	mClosestDistanceSq = 6500.0f;//80
	mAttackDelay = 1.0f;
	mFearHealth = mHealthMax * .667f;
	mPoints = 200.0f;
	mPointDrain = 1.5f;
	mMinPoints = 75.0f;
	mSightRangeSq = 1000000.0f;//1000
	mHearAttackRangeSq = 62500.0f;//250
	mHearRangeSq = 10000.0f;//100
	mLoseSightPlayer = 1.0f;
	mLoseSightFollower = 1.0f;
	mSpeed = 250.0f;
	mAttack = new Attack(L"Content/Models/ball.x", 15.0f, 400.0f, 0.1f, false);
	mAttack->setTextures(L"Content/Textures/tex_seed.dds", L"Content/Textures/tex_seed_n.dds");
}

EnemyFire::EnemyFire(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
	D3DXVECTOR3 _startPosition, float _healthMax, float _radius, D3DXVECTOR3 _meshScale) :
	Enemy(_meshName, _textureName, _normalTexName, _startPosition, _healthMax,
	_radius, _meshScale)
{
	mRanged = true;
	mAttackDistanceSq = 4000000.0f;//2000
	mClosestDistanceSq = 6500.0f;//80
	mAttackDelay = 1.0f;
	mFearHealth = mHealthMax * .667f;
	mPoints = 300.0f;
	mPointDrain = 2.0f;
	mMinPoints = 100.0f;
	mSightRangeSq = 1000000.0f;//1000
	mHearAttackRangeSq = 62500.0f;//250
	mHearRangeSq = 10000.0f;//100
	mLoseSightPlayer = 1.0f;
	mLoseSightFollower = 1.0f;
	mSpeed = 300.0f;
	mAttack = new Attack(L"Content/Models/ball.x", 20.0f, 400.0f, 0.1f, false);
	mAttack->setTextures(L"Content/Textures/tex_fire.dds", L"Content/Textures/tex_fire_n.dds");
}

void EnemyGroup::update(float _dt)
{
	for (Enemy* E : mEnemies)
		E->update(_dt);
}

void EnemyGroup::draw()
{
	for (Enemy* E : mEnemies)
		E->draw();
}

void EnemySpawner::update(float _dt)
{
	for (EnemyGroup* EG : mGroups)
		EG->update(_dt);
	for (Enemy* E : mEnemies)
		E->update(_dt);
}

void EnemySpawner::draw()
{
	for (EnemyGroup* EG : mGroups)
		EG->draw();
	for (Enemy* E : mEnemies)
		E->draw();
}
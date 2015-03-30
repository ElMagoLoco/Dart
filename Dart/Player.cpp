#include "Player.h"

#include "DirectInput.h"
#include "Level.h"
#include "StateMachine.h"
#include "Sound.h"

Player* gPlayer = 0;
Follower* gFollower = 0;

Pawn::Pawn(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
	D3DXVECTOR3 _startPosition, float _healthMax, float _radius, D3DXVECTOR3 _meshScale) :
	mHealth(_healthMax), mHealthMax(_healthMax), bIsDead(false), mPosition(_startPosition),
	mVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), mSpeed(0.0f), mRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	mRadius(_radius), mAttackTime(0.0f), mLastPosition(_startPosition), bIsMoving(false)
{
	//mMesh = new Mesh(_meshName, _startPosition, _meshScale, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	//mMesh->addTexture(_textureName, _normalTexName);
	mMesh = new AnimMesh(_meshName, _textureName, _normalTexName,_startPosition, _meshScale, 
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
}

Pawn::~Pawn()
{
	delete mMesh;
}

bool Pawn::addHealth(float _health)
{
	//can't add any if already maxed
	if (_health > 0 && mHealth == mHealthMax)
		return false;
	//set to min of max health or added health
	mHealth = min(mHealthMax, mHealth + _health);
	//if health is 0 then set to dead
	if (mHealth <= 0)
	{
		bIsDead = true;
	}
	return true;
}

void Pawn::update(float _dt)
{
	
	if (bIsMoving)
		mMesh->setCurrentAnimation(mMesh->getAnimationSetCount() - 1);
	else
		mMesh->setCurrentAnimation(-1);
	
	mMesh->update(_dt);
}

void Pawn::draw()
{
	mMesh->draw();
}

Player::Player(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName, 
	D3DXVECTOR3 _startPosition, float _healthMax, float _radius, D3DXVECTOR3 _meshScale) :
	Pawn(_meshName, _textureName, _normalTexName, _startPosition, _healthMax, _radius,
	_meshScale), 
	//starting with full ammo here, but that won't be the case in the actual game
	mAmmoSeeds(100), mAmmoSeedsMax(100), mAmmoFire(100), mAmmoFireMax(100), mScore(0),
	mLeftAttack(A_MELEE), mRightAttack(A_SEED)
{
	mSpeed = 400.0f;
	mAttackDelay = 1.0f;

	FR(gSound->getSystem()->createSound("Content/Audio/sndPlayerAttackFire", FMOD_DEFAULT, 0, &playerAttackFire));
	FR(gSound->getSystem()->createSound("Content/Audio/sndPlayerAttackMelee", FMOD_DEFAULT, 0, &playerAttackMelee));
	FR(gSound->getSystem()->createSound("Content/Audio/sndPlayerAttackSeed", FMOD_DEFAULT, 0, &playerAttackSeed));
	FR(gSound->getSystem()->createSound("Content/Audio/sndPlayerDeath", FMOD_DEFAULT, 0, &playerDeath)); //(for later use)
}

void Player::update(float _dt)
{
	//do nothing if dead
	if (bIsDead)
	{
		gStateMachine->transitionState(STATE_LOSE);
		return;
	}

	Pawn::update(_dt);

	//direction controls
	bIsMoving = false;
	mVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (gDInput->keyDown(DIK_W))
	{
		mVelocity += D3DXVECTOR3(0.0f, 0.0f, mSpeed);
		bIsMoving = true;
	}
	if (gDInput->keyDown(DIK_S))
	{
		mVelocity += D3DXVECTOR3(0.0f, 0.0f, -mSpeed);
		bIsMoving = true;
	}
	if (gDInput->keyDown(DIK_A))
	{
		mVelocity += D3DXVECTOR3(-mSpeed, 0.0f, 0.0f);
		bIsMoving = true;
	}
	if (gDInput->keyDown(DIK_D))
	{
		mVelocity += D3DXVECTOR3(mSpeed, 0.0f, 0.0f);
		bIsMoving = true;
	}
	if (mVelocity != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		D3DXVec3Normalize(&mVelocity, &mVelocity);
	mVelocity *= mSpeed;
	//face towards mouse position
	float a = (float)gWindowWidth / 2.0f - gDInput->mCursorPos2D.x;
	float b = (float)gWindowHeight / 2.0f - gDInput->mCursorPos2D.y;
	float angle = atan2f(b, a) + D3DX_PI / 2.0f;
	mRotation.y = angle;
	//did we collide with any level geometry?
	bool colliding = false;
	//for each colliding mesh in level
	// SAM: TODO: Hack, this is for testing
// 	mLastPosition = mPosition;
// 	mPosition += (mVelocity * _dt);
	for (Mesh* M : gCurrentLevel->getWorldGeometry())
	{
		//for each bounding box in mesh
/*		for (AxisAlignedBoundingBox2D* AABB : M->getAABBs())
		{
			colliding = collides(*AABB, BoundingSphere2D(D3DXVECTOR2(mPosition.x, mPosition.z), mRadius));
			if (colliding)
				break;
		}
		if (colliding) break;*/

		// SAM
		for (UINT i = 0; i < M->getBoundsBoxList().size(); ++i) {
			colliding = collides(AxisAlignedBoundingBox(M->getBoundsBoxList()[i]), BoundingSphere(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z), mRadius));
			if (colliding)
				break;
		}
		if (colliding)
			break;
	}
	//move
	if (!colliding)
	{//if not colliding, update position and rotation as normal
		mLastPosition = mPosition;
		mPosition += (mVelocity * _dt);
	}
	else//if colliding, only update rotation, push position back
	{
		mPosition = mLastPosition;
	}
	mMesh->setPosRot(mPosition, mRotation);
	
	//attacks
	mAttackTime += _dt;
	//if time to attack and if didn't just click the upper right UI buttons this frame
	if (mAttackTime > mAttackDelay &&
		(
		(gDInput->mCursorPos2D.x < (0.9f * (float)gWindowWidth)) ||
		(gDInput->mCursorPos2D.y > (0.3f * (float)gWindowHeight))
		)
		)
	{
		ePAttack attackType;
		//check if button is pressed
		if (gDInput->mouseButtonDown(0))
			attackType = mLeftAttack;
		else if (gDInput->mouseButtonDown(1))
			attackType = mRightAttack;
		else
			attackType = A_NONE;

		if (attackType != A_NONE)
		{
			Attack* attack;
			bool canAttack = true;
			switch (attackType)
			{
			case A_MELEE:
				attack = new Attack(L"", 20.0f, 400.0f, 0.1f, true);
				gSound->getSystem()->playSound(FMOD_CHANNEL_FREE,playerAttackMelee, false, 0);
				break;
			case A_SEED:
				if (mAmmoSeeds > 0)
				{
					--mAmmoSeeds;
					attack = new Attack(L"Content/Models/ball.x", 15.0f, 600.0f, 8.0f, true, 10.0f,
						D3DXVECTOR3(4.0f, 4.0f, 4.0f));
					attack->setTextures(L"Content/Textures/tex_seed.dds", L"Content/Textures/tex_seed_n.dds");
					gSound->getSystem()->playSound(FMOD_CHANNEL_FREE,playerAttackSeed, false, 0);
				}
				break;
			case A_FIRE:
				if (mAmmoFire > 0)
				{
					--mAmmoFire;
					attack = new Attack(L"Content/Models/ball.x", 30.0f, 500.0f, 8.0f, true, 10.0f,
						D3DXVECTOR3(4.0f, 4.0f, 4.0f));
					attack->setTextures(L"Content/Textures/tex_fire.dds", L"Content/Textures/tex_fire_n.dds");
					gSound->getSystem()->playSound(FMOD_CHANNEL_FREE,playerAttackFire, false, 0);
				}
				break;
			}
			attack->moveFacingDirection();
			gCurrentLevel->getAttackManager()->addAttack(attack);
			mAttackTime = 0.0f;
		}
	}
}

bool Player::addAmmoSeeds(UINT _amount)
{
	//can't add past max
	if (_amount > 0 && mAmmoSeeds == mAmmoSeedsMax)
		return false;
	//can't subtract past bottom
	if (_amount == 0)
		return false;
	mAmmoSeeds = min(mAmmoSeedsMax, mAmmoSeeds + _amount);
	return true;
}

bool Player::addAmmoFire(UINT _amount)
{
	//can't add past max
	if (_amount > 0 && mAmmoFire == mAmmoFireMax)
		return false;
	//can't subtract past bottom
	if (_amount == 0)
		return false;
	mAmmoFire = min(mAmmoFireMax, mAmmoFire + _amount);
	return true;
}

Follower::Follower(LPCWSTR _meshName, LPCWSTR _textureName, LPCWSTR _normalTexName,
	D3DXVECTOR3 _startPosition, float _healthMax, float _radius, D3DXVECTOR3 _meshScale):
	Pawn(_meshName, _textureName, _normalTexName, _startPosition, _healthMax,
	_radius, _meshScale), 
	mState(PSTATE_PURSUE), mLastPathFound(0.0f), bAfraid(false), mFearHealth(50.0f)
{
#ifdef DEBUG_PATHS
	mStartNode = new PathNode(_meshName, _startPosition);
#else
	mStartNode = new PathNode(_startPosition);
#endif
	mSpeed = 375.0f;//slightly less than player's

	//FR(gSound->getSystem()->createSound("Content/Audio/sndFollowerCryForHelp", FMOD_DEFAULT, 0, &followerCry);
}

void Follower::update(float _dt)
{
	//do nothing if dead
	if (bIsDead)
	{//will add a more elaborate lose screen later
		gStateMachine->transitionState(STATE_LOSE);
		return;
	}

	Pawn::update(_dt);

	mLastPathFound += _dt;
	static float aiCheck = 0.0f;
	aiCheck += _dt;
	if (aiCheck >= RUN_AI)//only run the AI every so many frames
	//inside this conditional you should use aiCheck where you'd normally use _dt
	{
		float distanceToPlayerSq;
		bIsMoving = true;
		switch (mState)
		{
		case PSTATE_PURSUE:
			distanceToPlayerSq = D3DXVec3LengthSq(&(gPlayer->getPosition() - mPosition));
			if (distanceToPlayerSq > FOLLOW_DISTANCE_SQ)
			{
				if (!followPath() || mLastPathFound > MIN_PATH_TIME)
					setPathPlayer();
			}
			else if (distanceToPlayerSq > FOLLOW_DISTANCE_STOP_SQ)
			{
				mVelocity = gPlayer->getPosition() - mPosition;
				D3DXVec3Normalize(&mVelocity, &mVelocity);//normalize to 1 unit length vector
				mVelocity *= mSpeed;//multiply by speed
			}
			else
			{
				stop();
				bIsMoving = false;
			}
			break;
		case PSTATE_AFRAID:
			//how long has it been afraid
			mAfraidTime += _dt;
			if (mAfraidTime >= mAfraidMax)
			{//don't let the follower be afraid indefinitely, else the task of getting them
				//to the goal might become impossible
				bAfraid = false;
				mAfraidTime = 0.0f;
				mState = PSTATE_PURSUE;
				break;
			}
			//if no longer afraid
			if (!bAfraid)
			{
				mState = PSTATE_PURSUE;
				break;
			}
			else//flee randomly in panic
			{
				if (mLastPathFound >= MIN_PATH_TIME || !followPath(1.0f))
				{
					setPathFlee();
				}
				//gSound->getSystem()->playSound(NULL, followerCry, false, 0);
			}
			break;
		}
		mPosition += (mVelocity * aiCheck);//set position based on speed
		pointForward(aiCheck);//set rotation based on direction
		mMesh->setPosRot(mPosition, mRotation);//set position and rotation of mesh
		aiCheck = 0.0f;
	}
}

void Follower::stop()
{
	mVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//swap to force reallocation
	vector<D3DXVECTOR3>().swap(mPath);
}

void Follower::resetStartNode()
{
	mStartNode->setPosition(mPosition);
	//find closest existing node and use the same links as it has
	PathNode* close;
	gCurrentLevel->getPaths()->getNodeNearPosition(close, mPosition);
	mStartNode->setLinks(close->getLinks());
}

void Follower::setPathPlayer()
{
	vector<D3DXVECTOR3>().swap(mPath);//clean out vector
	//find new path to player position
	resetStartNode();
	mPath = gCurrentLevel->getPaths()->findPath(mStartNode, gPlayer->getPosition());
	//path just found
	mLastPathFound = 0.0f;
}
//follow the path, return false if you are out of path to follow
bool Follower::followPath(float _speed)
{
	if (mPath.size() > (UINT)2)
	{
		//to make it a bit smoother, it goes towards the next to last node instead
		//of the last one, then pops the last node off once it's closer to the next to last 
		//node than the last one
		int lastIndex = mPath.size() - 1;
		float distLast = D3DXVec3LengthSq(&(mPosition - mPath[lastIndex]));
		float distNext = D3DXVec3LengthSq(&(mPosition - mPath[lastIndex - 1]));
		if (distNext < distLast)
		{
			mPath.pop_back();
			--lastIndex;
		}
		mVelocity = mPath[lastIndex - 1] - mPosition;
		D3DXVec3Normalize(&mVelocity, &mVelocity);//normalize to 1 unit length vector
		mVelocity *= (mSpeed * _speed);//multiply by speed, wandering is slower
		return true;
	}
	//if the path is getting short, go ahead to the next/last node
	else if (mPath.size() > (UINT)1)
	{
		//if you are near the next node, take it off and go to the next one
		int lastIndex = mPath.size() - 1;
		float distLast = D3DXVec3LengthSq(&(mPosition - mPath[lastIndex]));
		if (distLast < 100)//within 10 units
		{
			mPath.pop_back();
			--lastIndex;
		}
		//go towards the next node
		mVelocity = mPath[lastIndex] - mPosition;//find vector between the two points
		D3DXVec3Normalize(&mVelocity, &mVelocity);//normalize to 1 unit length vector
		mVelocity *= (mSpeed * _speed);//multiply by speed, wandering is slower
		return true;
	}
	else
		//if shorter than that, we need to redo it
		return false;
}

void Follower::pointForward(float _dt)
{
	//if they aren't moving, do nothing
	if (D3DXVec3LengthSq(&mVelocity) == 0.0f)
		return;
	//face the direction it is going
	float targetDir = atan2(mVelocity.x, mVelocity.z) + D3DX_PI;//target direction
	float angleDiff;//difference between angle we have and angle we want
	float pi2 = 2 * D3DX_PI;//2 * PI (full circle in radians)
	float facingMinusTarget = mRotation.y - targetDir;//vector difference between present rotation and target

	if (fabs(facingMinusTarget) > D3DX_PI)//if the angle we get is > half a circle
	{//figure angle difference to make it smaller
		if (mRotation.y > targetDir)
			angleDiff = -(pi2 - mRotation.y + targetDir);
		else
			angleDiff = pi2 - targetDir + mRotation.y;
	}
	else//else the angle difference we have is ok
		angleDiff = facingMinusTarget;

	if (angleDiff > 0.0f)
		mRotation.y -= 3.0f * _dt;
	else if (angleDiff < 0.0f)
		mRotation.y += 3.0f * _dt;
}

//when an enemy loses health, check if they are afraid
bool Follower::addHealth(float _health)
{
	bool boolReturn = Pawn::addHealth(_health);
	//if health was removed and they are below fear health
	if (boolReturn &&_health > 0 && mHealth < mFearHealth)
	{
		float chance = mHealth / mFearHealth;
		if (getRandomFloat(0.0f, 1.0f) > chance)
			bAfraid = true;
	}
	return boolReturn;
}

//find a random flee point and flee there
void Follower::setPathFlee()
{
	//find all the valid flee points
	vector<D3DXVECTOR2> validPoints;
	D3DXVECTOR2 choice;
	vector<D3DXVECTOR2> fleePoints = gCurrentLevel->getFleePoints();
	for (D3DXVECTOR2 FP : fleePoints)
	{
		//find distance of unit to flee point
		float enemyDistanceSq = D3DXVec2LengthSq(&(FP - D3DXVECTOR2(mPosition.x, mPosition.z)));
		//if it is too close, it is not ideal to flee to
		if (enemyDistanceSq < 90000)//<300
			continue;
		//find distance of player to flee point
		D3DXVECTOR3 playerPos = gPlayer->getPosition();
		float playerDistanceSq = D3DXVec2LengthSq(&(FP - D3DXVECTOR2(playerPos.x, playerPos.z)));
		//if distance from point is < 300, it is too close to flee to
		//if point is further from enemy than player, it is not ideal to flee to
		if (enemyDistanceSq < 90000 || enemyDistanceSq > playerDistanceSq)
			continue;
		validPoints.push_back(FP);
	}
	UINT size = validPoints.size();
	//if their is more than one valid point, choose one at random
	if (size > 1)
		choice = validPoints[rand() % size];
	//if there is one point, choose it
	else if (size == 1)
		choice = validPoints[0];
	//if there are no valid points, just choose one at random from all points
	else
		choice = fleePoints[rand() % fleePoints.size()];
	resetStartNode();
	mPath = gCurrentLevel->getPaths()->findPath(mPosition, D3DXVECTOR3(choice.x, 0.0f, choice.y));
	mLastPathFound = 0.0f;
}
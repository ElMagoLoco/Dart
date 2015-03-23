#pragma once

#include <list>
#include <vector>

#include "D3DUtils.h"
#include "Attack.h"
#include "Enemy.h"
#include "PathNode.h"
#include "PickUp.h"
#include "Mesh.h"

//the level itself
//to make some things easier, level will always be square x/z

class Level
{
public:
	Level(D3DXVECTOR2 _negCorner, UINT _squareSize);
	~Level();
	void update(float dt);
	void draw();
	void onLostDevice();
	void onResetDevice();
	//ground objects are never treated as obstacles
	void addGround(Mesh* _mesh)		{ mGround.push_back(_mesh); }
	//world geometry obstacles
	void addObstacle(Mesh* _mesh)	{ mObstacle.push_back(_mesh); }
	//add a point to flee to
	void addFleePoint(D3DXVECTOR2 _point) { mFleePoints.push_back(_point); }
	//add a point for enemies to heal at
	void addHealPoint(D3DXVECTOR2 _point) { mHealPoints.push_back(_point); }
	//delete all meshes
	void clearMeshes();
	//access world geometry for collisions
	std::vector<Mesh*> getWorldGeometry()  { return mObstacle; }
	//maps in this case are square on x/z and axis aligned
	UINT getSquareSize()		{ return mSquareSize; }
	D3DXVECTOR2 getNegCorner()	{ return mNegCorner; }
	//get paths
	AStar* getPaths()			{ return mPaths; }
	//get enemies
	EnemySpawner* getSpawner()  { return mSpawner; }
	std::list<Enemy*> getEnemies()	{ return mSpawner->getEnemies(); }//get individuals
	std::list<EnemyGroup*> getGroups() { return mSpawner->getGroups(); }//get groups
	//get flee points
	std::vector<D3DXVECTOR2>& getFleePoints() { return mFleePoints; }
	//get enemy heal points
	vector<D3DXVECTOR2>& getHealPoints() { return mHealPoints; }
	//get attack manager
	AttackManager* getAttackManager() { return mAttacks;  }
	PickUpManager* getPickUps() const { return mPickUps; }

private:
	//world geometry
	vector<Mesh*> mGround;
	vector<Mesh*> mObstacle;
	//locations a panicked unit might flee towards
	vector<D3DXVECTOR2> mFleePoints;
	//northwest corner of square level
	D3DXVECTOR2 mNegCorner;
	//size squared
	UINT mSquareSize;
	//path finding within the level
	AStar* mPaths;
	//enemy spawning
	EnemySpawner* mSpawner;
	//keep track of projectiles
	AttackManager* mAttacks;
	//points on the map where enemies can be healed
	vector<D3DXVECTOR2> mHealPoints;
	//keep track of pickups
	PickUpManager* mPickUps;
};

extern Level* gCurrentLevel;
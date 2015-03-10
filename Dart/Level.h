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
	//get attack manager
	AttackManager* getAttackManager() { return mAttacks;  }
	//get flee points
	std::vector<D3DXVECTOR2> getFleePoints() { return mFleePoints; }
private:
	//world geometry
	std::vector<Mesh*> mGround;
	std::vector<Mesh*> mObstacle;
	//locations a panicked unit might flee towards
	std::vector<D3DXVECTOR2> mFleePoints;
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
	//keep track of pickups
	PickUpManager* mPickUps;
};

extern Level* gCurrentLevel;
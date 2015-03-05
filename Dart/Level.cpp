#include "Level.h"

Level* gCurrentLevel = 0;

Level::Level(D3DXVECTOR2 _negCorner, UINT _squareSize):
mNegCorner(_negCorner), mSquareSize(_squareSize)
{
	mPaths = new AStar();
	mSpawner = new EnemySpawner();
	mAttacks = new AttackManager();
}

Level::~Level()
{
	delete mPaths;
	delete mSpawner;
	delete mAttacks;
}

void Level::update(float _dt)
{
	mSpawner->update(_dt);
	mAttacks->update(_dt);
}
void Level::draw()
{//draw in order ground, obstacles, enemies, attacks
	for (Mesh* M : mGround)
		M->draw();
	for (Mesh* M : mObstacle)
		M->draw();
	mPaths->draw();
	mSpawner->draw();
	mAttacks->draw();
}
void Level::onLostDevice()
{

}
void Level::onResetDevice()
{

}

void Level::clearMeshes()
{
	for (Mesh* M : mGround)
		delete M;
	vector<Mesh*>().swap(mGround);

	for (Mesh* M : mObstacle)
		delete M;
	vector<Mesh*>().swap(mObstacle);
}
#include "Level.h"

using std::list;
using std::vector;

Level* gCurrentLevel = 0;

Level::Level(D3DXVECTOR3 _negCorner, D3DXVECTOR3 _squareSize):
mNegCorner(_negCorner), mSize(_squareSize)
{
	mPaths = new AStar();
	mSpawner = new EnemySpawner();
	mAttacks = new AttackManager();
	mPickUps = new PickUpManager();
}

Level::~Level()
{
	delete mPaths;
	delete mSpawner;
	delete mAttacks;
	delete mPickUps;
}

void Level::update(float _dt)
{
	mSpawner->update(_dt);
	mAttacks->update(_dt);
	mPickUps->update(_dt);
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
	mPickUps->draw();
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
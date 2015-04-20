#include <string>
#include <fstream>
#include "LevelImporter.h"

#include "Level.h"

using std::wstring;
using std::wifstream;

LevelImporter imp;

LevelImporter* g_levelImp = &imp;

LevelImporter::LevelImporter() :
m_nLevelId(0),
m_nNumPawns(0),
m_nNumWalls(0),
m_nNumPickups(0),
m_nNumEnemies(0),
m_nNumFleePoints(0),
m_bLevelLoaded(false)
{
}


LevelImporter::~LevelImporter()
{
}

int LevelImporter::loadLevel(wchar_t* fileName)
{
	wifstream ifs(fileName, std::ios::in);

	if (ifs.fail())
		return -1;

	// need to clear the vectors and reset the counters if a level has already been loaded
	if (m_bLevelLoaded)
		clearData();
// 	if (m_enemyList.size() > 0 || m_wallList.size() > 0 || m_pickupList.size() > 0) {
// 		m_enemyList.clear();
// 		m_wallList.clear();
// 		m_pickupList.clear();
// 
// 		m_nNumPawns = 0;
// 		m_nNumWalls = 0;
// 		m_nNumPickups = 0;
// 		m_nNumEnemies = 0;
// 	}

	wchar_t token[64];
	ifs.getline(token, 64, L':');
	ifs.getline(token, 64);

	int numPawnTypes = _wtoi(token);

	// skip the descriptions and enums
	for (int i = 0; i < numPawnTypes; ++i) {
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);
	}

	ifs.getline(token, 64, L':');
	ifs.getline(token, 64);

	int numPawns = _wtoi(token);
	int numAntz = 0;
	int numWalls = 0;

	for (int i = 0; i < numPawns; ++i) {
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);

		int type = _wtoi(token);

		ifs.getline(token, 64, L':');
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);

		int solid = _wtoi(token);

		switch (type)
		{
		case EditorPawn::PawnType::PT_EnemyMeleeSpawnLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_EnemyMeleeSpawnLocale, D3DXVECTOR3(x, y, z));
			newPawn.setSolid(solid);
			m_enemyList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumEnemies;
		}
		break;
		case EditorPawn::PawnType::PT_EnemyFireSpawnLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_EnemyFireSpawnLocale, D3DXVECTOR3(x, y, z));
			newPawn.setSolid(solid);
			m_enemyList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumEnemies;
		}
		break;
		case EditorPawn::PawnType::PT_EnemySeedSpawnLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_EnemySeedSpawnLocale, D3DXVECTOR3(x, y, z));
			newPawn.setSolid(solid);
			m_enemyList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumEnemies;
		}
		break;
		case EditorPawn::PawnType::PT_DartSpawnLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			m_dart = EditorPawn(EditorPawn::PawnType::PT_DartSpawnLocale, D3DXVECTOR3(x, y, z));
			m_dart.setSolid(solid);
			++m_nNumPawns;
		}
		break;
		case EditorPawn::PawnType::PT_BartSpawnLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			m_bart = EditorPawn(EditorPawn::PawnType::PT_BartSpawnLocale, D3DXVECTOR3(x, y, z));
			m_bart.setSolid(solid);
			++m_nNumPawns;
		}
		break;
		case EditorPawn::PawnType::PT_PointOfLevelLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			m_goal = EditorPawn(EditorPawn::PawnType::PT_PointOfLevelLocale, D3DXVECTOR3(x, y, z));
			m_goal.setSolid(solid);
			++m_nNumPawns;
		}
		break;
		case EditorPawn::PawnType::PT_RockLocale:
		{

		}
		break;
		case EditorPawn::PawnType::PT_Wall:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;
			float dx, dy, dz;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			dx = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			dy = (float)_wtof(token);
			ifs.getline(token, 64);
			dz = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_Wall, D3DXVECTOR3(x, y, z), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(dx, dy, dz));
			newPawn.setSolid(solid);
			m_wallList.push_back(newPawn);

			++m_nNumPawns;
			++m_nNumWalls;
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Heal:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_Pickup_Heal, D3DXVECTOR3(x, y, z));
			newPawn.setSolid(solid);
			m_pickupList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumPickups;
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Bonus:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_Pickup_Bonus, D3DXVECTOR3(x, y, z));
			newPawn.setSolid(solid);
			m_pickupList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumPickups;
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Ammo_Seed:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_Pickup_Ammo_Seed, D3DXVECTOR3(x, y, z));
			newPawn.setSolid(solid);
			m_pickupList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumPickups;
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Ammo_Fire:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_Pickup_Ammo_Fire, D3DXVECTOR3(x, y, z));
			newPawn.setSolid(solid);
			m_pickupList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumPickups;
		}
		break;
		case EditorPawn::PawnType::PT_FleePoint:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			EditorPawn newPawn(EditorPawn::PawnType::PT_FleePoint, D3DXVECTOR3(x, y, z));
			newPawn.setSolid(solid);
			m_fleePointList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumFleePoints;
		}
		break;
		} // end switch
	} // end for

	ifs.getline(token, 64, L':');
	ifs.getline(token, 64);
	int floorCreated = _wtoi(token);
	if (floorCreated) {
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);
	}

	m_bLevelLoaded = true;

	return m_nLevelId++;
}

void LevelImporter::clearData()
{
	m_enemyList.clear();
	m_wallList.clear();
	m_pickupList.clear();
	m_fleePointList.clear();

	m_dart = EditorPawn();
	m_bart = EditorPawn();
	m_nNumPawns = 0;
	m_nNumWalls = 0;
	m_nNumPickups = 0;
	m_nNumEnemies = 0;
	m_nNumFleePoints = 0;

	m_bLevelLoaded = false;
}
//run after load level
void LevelImporter::makeLevel()
{
	//make level
	gCurrentLevel = new Level(D3DXVECTOR3(-3000.0f, 0.0f, -3000.0f),
		D3DXVECTOR3(6000.0f, 0.0f, 6000.0f));
	//add ground
	Mesh* meshGround = new Mesh(L"Content/Models/ground.X", D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	meshGround->addTexture(L"Content/Textures/tex_grass.dds", L"Content/Textures/tex_grass_n.dds");
	gCurrentLevel->addGround(meshGround);
	//add obstacles
	for (int i = 0; i < g_levelImp->getNumWalls(); ++i) {
		Mesh* meshObstacle = new Mesh(L"Content/Models/MyBoxUnscaled.x", g_levelImp->getWallList()[i].getPos(), g_levelImp->getWallList()[i].getScale());
		meshObstacle->addTexture(L"Content/Textures/tex_rock.dds", L"Content/Textures/tex_rock_n.dds");
		gCurrentLevel->addObstacle(meshObstacle);
		// SAM
		// TODO: most figure out the dimensions of the AABB using the position and scale of the current wall
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 		float minx = g_levelImp->getWallList()[i].getPos().x - (g_levelImp->getWallList()[i].getScale().x * 0.5f);
		// 		float minz = g_levelImp->getWallList()[i].getPos().z - (g_levelImp->getWallList()[i].getScale().z * 0.5f);
		// 		float maxx = g_levelImp->getWallList()[i].getPos().x + (g_levelImp->getWallList()[i].getScale().x * 0.5f);
		// 		float maxz = g_levelImp->getWallList()[i].getPos().z + (g_levelImp->getWallList()[i].getScale().z * 0.5f);
		float minx = g_levelImp->getWallList()[i].getPos().x - (g_levelImp->getWallList()[i].getScale().x * 0.5f);
		float miny = 0.0f;// g_levelImp->getWallList()[i].getPos().y - (g_levelImp->getWallList()[i].getScale().y * 0.5f);
		float minz = g_levelImp->getWallList()[i].getPos().z - (g_levelImp->getWallList()[i].getScale().z * 0.5f);
		float maxx = g_levelImp->getWallList()[i].getPos().x + (g_levelImp->getWallList()[i].getScale().x * 0.5f);
		float maxy = /*g_levelImp->getWallList()[i].getPos().y + (*/g_levelImp->getWallList()[i].getScale().y;// *0.5f);
		float maxz = g_levelImp->getWallList()[i].getPos().z + (g_levelImp->getWallList()[i].getScale().z * 0.5f);
		AxisAlignedBoundingBox aabbObstacle(D3DXVECTOR3(minx, miny, minz/*50.0f, 50.0f*/), D3DXVECTOR3(maxx, maxy, maxz/*150.0f, 150.0f*/));
		meshObstacle->addBoundsBox(aabbObstacle);
		gCurrentLevel->addObstacle(meshObstacle);
	}

	// 	Mesh* meshObstacle = new Mesh(L"Content/Models/box.x", D3DXVECTOR3(100.0f, 50.0f, 100.0f));
	// 	meshObstacle->addTexture(L"Content/Textures/tex_rock.dds", L"Content/Textures/tex_rock_n.dds");
	// 	AxisAlignedBoundingBox2D* aabbObstacle = new AxisAlignedBoundingBox2D(
	// 		D3DXVECTOR2(50.0f, 50.0f), D3DXVECTOR2(150.0f, 150.0f));
	// 	meshObstacle->addAABB(aabbObstacle);
	// 	gCurrentLevel->addObstacle(meshObstacle);

	//add enemies to level
	for (int i = 0; i < g_levelImp->getNumEnemies(); ++i) {
		switch (g_levelImp->getEnemyList()[i].getPawnType())
		{
		case EditorPawn::PawnType::PT_EnemyMeleeSpawnLocale:
		{
			gCurrentLevel->getSpawner()->addEnemy(new EnemyMelee(
				L"Content/Models/BeeZFinal.X", L"Content/Textures/tex_snail.dds",
				L"Content/Textures/tex_snail_n.dds", "Content/Audio/sndEnemy1Attack.wav",
				"Content/Audio/sndEnemy1Death.wav", "Content/Audio/sndEnemy1GetHit.wav",
				g_levelImp->getEnemyList()[i].getPos(), 30.0f, 40.0f, D3DXVECTOR3(1.50f, 1.50f, 1.50f)));
			break;
		}
		case EditorPawn::PawnType::PT_EnemySeedSpawnLocale:
		{
			gCurrentLevel->getSpawner()->addEnemy(new EnemySeeds(
				L"Content/Models/AntZFinal.X", L"Content/Textures/tex_bee.dds",
				L"Content/Textures/tex_bee_n.dds", "Content/Audio/sndEnemy1Attack.wav",
				"Content/Audio/sndEnemy1Death.wav", "Content/Audio/sndEnemy1GetHit.wav",
				g_levelImp->getEnemyList()[i].getPos(), 30.0f, 40.0f, D3DXVECTOR3(2.50f, 2.50f, 2.50f)));
			break;
		}
		case EditorPawn::PawnType::PT_EnemyFireSpawnLocale:
		{
			gCurrentLevel->getSpawner()->addEnemy(new EnemyFire(
				L"Content/Models/BeeZFinal.X", L"Content/Textures/tex_ant.dds",
				L"Content/Textures/tex_ant_n.dds", "Content/Audio/sndEnemy1Attack.wav",
				"Content/Audio/sndEnemy1Death.wav", "Content/Audio/sndEnemy1GetHit.wav",
				g_levelImp->getEnemyList()[i].getPos(), 30.0f, 40.0f, D3DXVECTOR3(1.50f, 1.50f, 1.50f)));
			break;
		}
		}
	}
	// add pickups
	for (int i = 0; i < g_levelImp->getNumPickups(); ++i) {
		switch (g_levelImp->getPickupList()[i].getPawnType())
		{
		case EditorPawn::PawnType::PT_Pickup_Heal:
		{
			//g_levelImp->getPickupList()[i].getPos().y += 350.0f;
			PickUp* newPickup = new PickUp(L"Content\\Models\\ball.x",
				L"Content\\Textures\\tex_heal_berry.dds",
				L"Content\\Textures\\tex_heal_berry_n.dds",
				"Content\\Audio\\sndMenuKeyPress.wav",
				ePickUpType::PICKUP_HEAL,
				g_levelImp->getPickupList()[i].getPos(),
				D3DXVECTOR3(5.0f, 5.0f, 5.0f),//g_levelImp->getPickupList()[i].getScale(), 
				25.0f);

			gCurrentLevel->getPickUps()->addPickUp(newPickup);
		}
			break;
		case EditorPawn::PawnType::PT_Pickup_Bonus:
		{
			//	g_levelImp->getPickupList()[i].getPos().y += 50.0f;
			PickUp* newPickup = new PickUp(L"Content\\Models\\ball.x",
				L"Content\\Textures\\tex_bonus.dds",
				L"Content\\Textures\\tex_bonus_n.dds",
				"Content\\Audio\\sndMenuKeyPress.wav",
				ePickUpType::PICKUP_BONUS,
				g_levelImp->getPickupList()[i].getPos(),
				D3DXVECTOR3(5.0f, 5.0f, 5.0f),//g_levelImp->getPickupList()[i].getScale(),
				50.0f);

			gCurrentLevel->getPickUps()->addPickUp(newPickup);
		}
			break;
		case EditorPawn::PawnType::PT_Pickup_Ammo_Seed:
		{
			//	g_levelImp->getPickupList()[i].getPos().y += 50.0f;
			PickUp* newPickup = new PickUp(L"Content\\Models\\ball.x",
				L"Content\\Textures\\tex_seed.dds",
				L"Content\\Textures\\tex_seed_n.dds",
				"Content\\Audio\\sndMenuKeyPress.wav",
				ePickUpType::PICKUP_AMMO_SEED,
				g_levelImp->getPickupList()[i].getPos(),
				D3DXVECTOR3(5.0f, 5.0f, 5.0f),//g_levelImp->getPickupList()[i].getScale(),
				20.0f);

			gCurrentLevel->getPickUps()->addPickUp(newPickup);
		}
			break;
		case EditorPawn::PawnType::PT_Pickup_Ammo_Fire:
		{
			//	g_levelImp->getPickupList()[i].getPos().y += 50.0f;
			PickUp* newPickup = new PickUp(L"Content\\Models\\ball.x",
				L"Content\\Textures\\tex_fire.dds",
				L"Content\\Textures\\tex_fire_n.dds",
				"Content\\Audio\\sndMenuKeyPress.wav",
				ePickUpType::PICKUP_AMMO_FIRE,
				g_levelImp->getPickupList()[i].getPos(),
				D3DXVECTOR3(5.0f, 5.0f, 5.0f),//g_levelImp->getPickupList()[i].getScale(),
				20.0f);

			gCurrentLevel->getPickUps()->addPickUp(newPickup);
		}
			break;
		} // end switch
	} // end for

	//add goal
	//goal must be added after other pickups
	PickUp* goalPickup = new PickUp(L"Content\\Models\\ball.x",
		L"Content\\Textures\\tex_bart.dds",
		L"Content\\Textures\\tex_bart_n.dds",
		"Content\\Audio\\sndMenuKeyPress.wav",
		ePickUpType::PICKUP_GOAL,
		g_levelImp->getGoal().getPos(),
		D3DXVECTOR3(5.0f, 5.0f, 5.0f),//g_levelImp->getPickupList()[i].getScale(), 
		20.0f);
	gCurrentLevel->getPickUps()->addPickUp(goalPickup);

	//always load obstacles before running initialize path finding
	gCurrentLevel->getPaths()->initPathfinding();
	//specify points for units to flee to
	for (int i = 0; i < g_levelImp->getNumFleePoints(); ++i) {
		gCurrentLevel->addFleePoint(D3DXVECTOR2(g_levelImp->getFleePointList()[i].getPos().x, g_levelImp->getFleePointList()[i].getPos().z));
	}
}
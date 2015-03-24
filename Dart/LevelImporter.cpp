#include <string>
#include <fstream>
#include "LevelImporter.h"

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
		case EditorPawn::PawnType::PT_EnemyType1SpawnLocale:
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

			EditorPawn newPawn(EditorPawn::PawnType::PT_EnemyType1SpawnLocale, D3DXVECTOR3(x, y, z));
			newPawn.setSolid((bool)solid);
			m_enemyList.push_back(newPawn);
			++m_nNumPawns;
			++m_nNumEnemies;
		}
		break;
		case EditorPawn::PawnType::PT_EnemyType2SpawnLocale:
		{

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
			m_dart.setSolid((bool)solid);
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
			m_bart.setSolid((bool)solid);
			++m_nNumPawns;
		}
		break;
		case EditorPawn::PawnType::PT_PointOfLevelLocale:
		{

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
			newPawn.setSolid((bool)solid);
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
			newPawn.setSolid((bool)solid);
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
			newPawn.setSolid((bool)solid);
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
			newPawn.setSolid((bool)solid);
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
			newPawn.setSolid((bool)solid);
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
			newPawn.setSolid((bool)solid);
			m_pickupList.push_back(newPawn);
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

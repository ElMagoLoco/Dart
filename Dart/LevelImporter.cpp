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
m_nNumWalls(0)
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

// 	if (m_bFloorCreated) {
// 		SAFE_RELEASE(m_pFloorIB);
// 		SAFE_RELEASE(m_pFloorVB);
// 	}

	//m_pawnList.clear();
	//m_pawnList.resize(0);

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

//	m_pawnList.resize(numPawns);

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
			m_nNumPawns++;
// 			createAntzSpawn(D3DXVECTOR3(x, y, z));
// 			wstring temp = L"Antz Spawn Point";
// 			wchar_t temp1[32];
// 			_itow_s(numAntz++, temp1, 10);
// 			temp += temp1;
// 			m_dlg.insertToPawnList(temp.GetString());
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
		//	m_pawnList.push_back(newPawn);
// 			createDartSpawn(D3DXVECTOR3(x, y, z));
// 			m_dlg.insertToPawnList(L"Dart Spawn Point");
// 			m_dlg.setDartCreated(true);
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

			m_bart = EditorPawn(EditorPawn::PawnType::PT_DartSpawnLocale, D3DXVECTOR3(x, y, z));
			m_bart.setSolid((bool)solid);

// 			createBartSpawn(D3DXVECTOR3(x, y, z));
// 			m_dlg.insertToPawnList(L"Bart Spawn Point");
// 			m_dlg.setBartCreated(true);
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
// 			createWall(D3DXVECTOR3(dx, dy, dz), D3DXVECTOR3(x, y, z));
// 
// 			CString temp = L"Wall";
// 			wchar_t temp1[32];
// 			_itow_s(numWalls++, temp1, 10);
// 			temp += temp1;
// 			m_dlg.insertToPawnList(temp.GetString());
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
	//	m_dlg.setFloorWidth(_wtoi(token));
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);
// 		m_dlg.setFloorLength(_wtoi(token));
// 		createFloor();
	}

	return m_nLevelId++;
}

void LevelImporter::clearData()
{

}

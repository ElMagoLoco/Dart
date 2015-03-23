#pragma once
#include <vector>
#include "D3DUtils.h"
#include "EditorPawn.h"

class LevelImporter
{
public:
	int loadLevel(wchar_t* fileName);
	void clearData();

	std::vector<EditorPawn>& getEnemyList() { return m_enemyList; }
	std::vector<EditorPawn>& getWallList() { return m_wallList; }
	std::vector<EditorPawn>& getPickupList() { return m_pickupList; }
	EditorPawn getDart() const { return m_dart; }
	//void setDart(EditorPawn val) { m_dart = val; }
	EditorPawn getBart() const { return m_bart; }
	//void setBart(EditorPawn val) { m_bart = val; }
	int getNumPawns() const { return m_nNumPawns; }
	//void setNumPawns(int val) { m_nNumPawns = val; }
	int getNumWalls() const { return m_nNumWalls; }
	//void setNumWalls(int val) { m_nNumWalls = val; }
	int getNumPickups() const { return m_nNumPickups; }
	int getNumEnemies() const { return m_nNumEnemies; }

	LevelImporter();
	~LevelImporter();

protected:
	std::vector<EditorPawn>		m_enemyList;
	std::vector<EditorPawn>		m_wallList;
	std::vector<EditorPawn>		m_pickupList;

	EditorPawn		m_dart;
	EditorPawn		m_bart;

	int		m_nNumPawns;
	int		m_nNumWalls;
	int		m_nNumEnemies;
	int		m_nNumPickups;
	int		m_nLevelId;

};

extern LevelImporter* g_levelImp;

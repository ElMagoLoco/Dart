#pragma once
#include <vector>
#include "D3DUtils.h"
#include "EditorPawn.h"

class LevelImporter
{
public:
	int loadLevel(wchar_t* fileName);
	void makeLevel();
	void clearData();

	std::vector<EditorPawn>& getEnemyList() { return m_enemyList; }
	std::vector<EditorPawn>& getWallList() { return m_wallList; }
	std::vector<EditorPawn>& getPickupList() { return m_pickupList; }
	std::vector<EditorPawn>& getFleePointList() { return m_fleePointList; }

	EditorPawn getDart() const { return m_dart; }
	//void setDart(EditorPawn val) { m_dart = val; }
	EditorPawn getBart() const { return m_bart; }
	EditorPawn getGoal() const { return m_goal; }
	//void setBart(EditorPawn val) { m_bart = val; }
	int getNumPawns() const { return m_nNumPawns; }
	//void setNumPawns(int val) { m_nNumPawns = val; }
	int getNumWalls() const { return m_nNumWalls; }
	//void setNumWalls(int val) { m_nNumWalls = val; }
	int getNumPickups() const { return m_nNumPickups; }
	int getNumEnemies() const { return m_nNumEnemies; }
	int getNumFleePoints() const { return m_nNumFleePoints; }

	LevelImporter();
	~LevelImporter();

protected:
	std::vector<EditorPawn>		m_enemyList;
	std::vector<EditorPawn>		m_wallList;
	std::vector<EditorPawn>		m_pickupList;
	std::vector<EditorPawn>		m_fleePointList;

	EditorPawn		m_dart;
	EditorPawn		m_bart;
	EditorPawn		m_goal;

	int		m_nNumPawns;
	int		m_nNumWalls;
	int		m_nNumEnemies;
	int		m_nNumPickups;
	int		m_nNumFleePoints;
	int		m_nLevelId;

	bool	m_bLevelLoaded;
};

extern LevelImporter* g_levelImp;

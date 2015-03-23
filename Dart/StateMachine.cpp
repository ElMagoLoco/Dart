#include "StateMachine.h"

StateMachine* gStateMachine = 0;

//constructor
StateMachine::StateMachine()
{
	bReset = false;
}
//destructor
StateMachine::~StateMachine()
{
	for (State* S : mStates)
		delete S;
	vector<State*>().swap(mStates);
}
//don't turn it on until you've added at least one state
void StateMachine::turnOn()
{
	if (mStates.size() < 1)
	{
		OutputDebugString(L"Cannot start state machine without at least one state added.\n");
		PostQuitMessage(0);
	}
	else
	{
		mStates[0]->setTransitioning(false);
		mStates[0]->beginState();
		mCurrentState = eState(0);
	}
}

//these methods run appropriate state functions for the current state
void StateMachine::beginState()
{
	mStates[mCurrentState]->beginState();
}

void StateMachine::endState()
{
	mStates[mCurrentState]->endState();
}

void StateMachine::updateState(float _dt)
{
	mStates[mCurrentState]->updateState(_dt);
}

void StateMachine::drawState3D()
{
	mStates[mCurrentState]->drawState3D();
}

void StateMachine::drawState2D(ID3DXSprite* _sprite)
{
	mStates[mCurrentState]->drawState2D(_sprite);
}

void StateMachine::drawStateText()
{
	mStates[mCurrentState]->drawStateText();
}

void StateMachine::onLostDevice()
{
	mStates[mCurrentState]->onLostDevice();
}

void StateMachine::onResetDevice()
{
	mStates[mCurrentState]->onResetDevice();
}

void StateMachine::addState(State* _newState)
{
	mStates.push_back(_newState);
}
//transition from one state to another
//end the first state, set the state, then begin the new state
void StateMachine::transitionState(eState _newState)
{
	mStates[mCurrentState]->endState();
	//if it transitioned in the middle of a state update, you'll need
	//to let the other events know not to execute
	mStates[mCurrentState]->setTransitioning(true);
	mCurrentState = _newState;
	mStates[mCurrentState]->setTransitioning(false);
	mStates[mCurrentState]->beginState();
}
//base state constructor
State::State()
{
}
//base state destructor, delete events
State::~State()
{
	for (Event* E : mEvents)
		delete E;
	vector<Event*>().swap(mEvents);
}
//following functions cycle through events and run each of them
void State::beginState()
{
	for (Event* E : mEvents)
		E->beginEvent();
}

void State::endState()
{
	for (Event* E : mEvents)
		E->endEvent();
}

void State::updateState(float dt)
{
	for (Event* E : mEvents)
	{
		E->updateEvent(dt);
		if (bTransitioning)
			break;
	}
}

void State::drawState3D()
{
	if (bTransitioning)
		return;
	for (Event* E : mEvents)
		E->drawEvent3D();
}

void State::drawState2D(ID3DXSprite* sprite)
{
	if (bTransitioning)
		return;
	for (Event* E : mEvents)
		E->drawEvent2D(sprite);
}

void State::drawStateText()
{
	if (bTransitioning)
		return;
	for (Event* E : mEvents)
		E->drawEventText();
}

void State::onLostDevice()
{
	for (Event* E : mEvents)
		E->onLostDevice();
}

void State::onResetDevice()
{
	for (Event* E : mEvents)
		E->onResetDevice();
}

void State::addEvent(Event* _newEvent)
{
	mEvents.push_back(_newEvent);
}

/*******************************************************************
Main Menu
*******************************************************************/
void EventDisplayMenuMain::beginEvent()
{
	//main menu background
	mMenu = new Menu(L"Content/Textures/background_menu.png", 0.0f, 0.0f, 1024, 1024, 1.0f, 1.0f);
	//button to start game
	ButtonNewGame* bNewGame = new ButtonNewGame(L"Content/Textures/button_new_game.png",
		0.4f, 0.4f, 128, 64, 0.2f, 0.1f);
	mMenu->addButton(bNewGame);
	//button to exit game
	ButtonExitGame* bExit = new ButtonExitGame(L"Content/Textures/button_exit_game.png",
		0.4f, 0.8f, 128, 64, 0.2f, 0.1f);
	mMenu->addButton(bExit);
	//button to go to options menu
	ButtonOptions* bOptions = new ButtonOptions(L"Content/Textures/button_options.png",
		0.4f, 0.6f, 128, 64, 0.2f, 0.1f);
	mMenu->addButton(bOptions);
}

/*******************************************************************
Options Menu
*******************************************************************/
void EventDisplayMenuOptions::beginEvent()
{
	//background
	mMenu = new Menu(L"Content/Textures/background_menu.png", 0.0f, 0.0f, 1024, 1024,
		1.0f, 1.0f);
	//button to go to windowed mode
	Button1024x768* b1024x768 = new Button1024x768(L"Content/Textures/button_windowed.png",
		0.4f, 0.4f, 128, 64, 0.2f, 0.1f);
	mMenu->addButton(b1024x768);
	//button to go to full screen mode
	ButtonFullScreen* bFullScreen = new ButtonFullScreen(L"Content/Textures/button_full_screen.png",
		0.4f, 0.6f, 128, 64, 0.2f, 0.1f);
	mMenu->addButton(bFullScreen);
	//button to return to main menu
	ButtonGoBack* bGoBack = new ButtonGoBack(L"Content/Textures/button_return.png",
		0.4f, 0.8f, 128, 64, 0.2f, 0.1f);
	mMenu->addButton(bGoBack);
}

/*******************************************************************
User Interface
*******************************************************************/
void EventDisplayUserInterface::beginEvent()
{
	//overlay
	mInterface = new UserInterface(L"Content/Textures/ui_overlay.png", 0.0f, 0.0f, 1024, 1024,
		1.0f, 1.0f);
	StatBar* sbHealthDart = new StatBar(L"Content/Textures/ui_bar_health_dart.png", 0.0f, 0.85f,
		64, 16, 0.1f, 0.05f, STAT_HEALTH_PLAYER);
	mInterface->addStatBar(sbHealthDart);
	StatBar* sbHealthBart = new StatBar(L"Content/Textures/ui_bar_health_bart.png", 0.0f, 0.9f,
		64, 16, 0.1f, 0.05f, STAT_HEALTH_FOLLOWER);
	mInterface->addStatBar(sbHealthBart);
	StatBar* sbAmmoSeeds = new StatBar(L"Content/Textures/ui_bar_ammo_seed.png", 0.9f, 0.85f,
		64, 16, 0.1f, 0.05f, STAT_AMMO_SEED);
	mInterface->addStatBar(sbAmmoSeeds);
	StatBar* sbAmmoFire = new StatBar(L"Content/Textures/ui_bar_ammo_fire.png", 0.9f, 0.9f,
		64, 16, 0.1f, 0.05f, STAT_AMMO_SEED);
	mInterface->addStatBar(sbAmmoFire);
	ButtonSetAttackMelee* bMelee = new ButtonSetAttackMelee(L"Content/Textures/ui_icon_beak.png",
		0.9f, 0.0f, 64, 64, 0.1f, 0.1f);
	mInterface->addButton(bMelee);
	ButtonSetAttackSeed* bSeed = new ButtonSetAttackSeed(L"Content/Textures/ui_icon_seed.png",
		0.9f, 0.1f, 64, 64, 0.1f, 0.1f);
	mInterface->addButton(bSeed);
	ButtonSetAttackFire* bFire = new ButtonSetAttackFire(L"Content/Textures/ui_icon_fire.png",
		0.9f, 0.2f, 64, 64, 0.1f, 0.1f);
	mInterface->addButton(bFire);
}


/*******************************************************************
Level 1
*******************************************************************/
void EventProcessLevel1::beginEvent()
{
	// load the level info from the file
	g_levelImp->loadLevel(L"Content\\Levels\\TestLevel4.dlvl");
	//make level
	gCurrentLevel = new Level(D3DXVECTOR2(-1500.0f, -1500.0f), 1000);
	//add ground
	Mesh* meshGround = new Mesh(L"Content/Models/ground.X", D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	meshGround->addTexture(L"Content/Textures/tex_grass.dds", L"Content/Textures/tex_grass_n.dds");
	gCurrentLevel->addGround(meshGround);
	//add obstacles
	for (int i = 0; i < g_levelImp->getNumWalls(); ++i) {
		Mesh* meshObstacle = new Mesh(L"Content/Models/box.x", g_levelImp->getWallList()[i].getPos() * 10.0f, g_levelImp->getWallList()[i].getScale());
		meshObstacle->addTexture(L"Content/Textures/tex_rock.dds", L"Content/Textures/tex_rock_n.dds");
		gCurrentLevel->addObstacle(meshObstacle);
		// SAM
		// TODO: most figure out the dimensions of the AABB using the position and scale of the current wall
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 		float minx = g_levelImp->getWallList()[i].getPos().x - (g_levelImp->getWallList()[i].getScale().x * 0.5f);
		// 		float minz = g_levelImp->getWallList()[i].getPos().z - (g_levelImp->getWallList()[i].getScale().z * 0.5f);
		// 		float maxx = g_levelImp->getWallList()[i].getPos().x + (g_levelImp->getWallList()[i].getScale().x * 0.5f);
		// 		float maxz = g_levelImp->getWallList()[i].getPos().z + (g_levelImp->getWallList()[i].getScale().z * 0.5f);
		float minx = (g_levelImp->getWallList()[i].getScale().x * 0.5f) + g_levelImp->getWallList()[i].getPos().x;
		float minz = (g_levelImp->getWallList()[i].getScale().x * 0.5f) + g_levelImp->getWallList()[i].getPos().x;
		float maxx = (g_levelImp->getWallList()[i].getScale().z * 0.5f) + g_levelImp->getWallList()[i].getPos().z;
		float maxz = (g_levelImp->getWallList()[i].getScale().z * 0.5f) + g_levelImp->getWallList()[i].getPos().z;
		AxisAlignedBoundingBox aabbObstacle(D3DXVECTOR3(minx, minz, minx/*50.0f, 50.0f*/), D3DXVECTOR3(maxx, maxz, maxz/*150.0f, 150.0f*/));
		meshObstacle->addBoundsBox(aabbObstacle);
		gCurrentLevel->addObstacle(meshObstacle);
	}
	
// 	Mesh* meshObstacle = new Mesh(L"Content/Models/box.x", D3DXVECTOR3(100.0f, 50.0f, 100.0f));
// 	meshObstacle->addTexture(L"Content/Textures/tex_rock.dds", L"Content/Textures/tex_rock_n.dds");
// 	AxisAlignedBoundingBox2D* aabbObstacle = new AxisAlignedBoundingBox2D(
// 		D3DXVECTOR2(50.0f, 50.0f), D3DXVECTOR2(150.0f, 150.0f));
// 	meshObstacle->addAABB(aabbObstacle);
// 	gCurrentLevel->addObstacle(meshObstacle);
	//always load obstacles before running initialize path finding
	gCurrentLevel->getPaths()->initPathfinding();
	//specify points for units to flee to
	gCurrentLevel->addFleePoint(D3DXVECTOR2(-1400, -1400));
	gCurrentLevel->addFleePoint(D3DXVECTOR2(1400, -1400));
	gCurrentLevel->addFleePoint(D3DXVECTOR2(-1400, 1400));
	gCurrentLevel->addFleePoint(D3DXVECTOR2(1400, 1400));
	//add enemies to level
	for (UINT i = 0; i < g_levelImp->getNumEnemies(); ++i) {
		gCurrentLevel->getSpawner()->addEnemy(new EnemyMelee(L"Content/Models/tiny.x", L"Content/Textures/Tiny_skin.dds",
			L"Content/Textures/Tiny_skin_n.dds",
			g_levelImp->getEnemyList()[i].getPos() * 10.0f, 30.0f, 40.0f, D3DXVECTOR3(0.50f, 0.50f, 0.50f)));
	}

	// add pickups
// 	for (UINT i = 0; i < g_levelImp->getNumPickups(); ++i) {
// 		PickUp newPickup;
// 
// 	}
}
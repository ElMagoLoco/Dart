#include "StateMachine.h"

StateMachine* gStateMachine = 0;

//constructor
StateMachine::StateMachine()
{
	bReset = false;
	mLoadingScreen = new Texture2D(L"Content/Textures/background_loading.png", 0.0f, 0.0f,
		1024, 1024, 1.0f, 1.0f);
}
//destructor
StateMachine::~StateMachine()
{
	for (State* S : mStates)
		delete S;
	vector<State*>().swap(mStates);
	delete mLoadingScreen;
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

void StateMachine::drawState2D()
{
	mStates[mCurrentState]->drawState2D();
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

void State::drawState2D()
{
	if (bTransitioning)
		return;
	for (Event* E : mEvents)
		E->drawEvent2D();
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
Loading Menu
*******************************************************************/
void EventDisplayMenuLoading::beginEvent()
{
	mMenu = new Menu(L"Content/Textures/background_menu.png", 0.0f, 0.0f, 1024, 1024, 1.0f, 1.0f);
	ButtonNewGame* bStartGame = new ButtonNewGame(L"Content/Textures/background_loading.png",
		0.0f, 0.0f, 1024, 1024, 1.0f, 1.0f);
	mMenu->addButton(bStartGame);
}

/*******************************************************************
Loss Menu
*******************************************************************/
void EventDisplayMenuLoss::beginEvent()
{
	//main menu background
	mMenu = new Menu(L"Content/Textures/background_menu.png", 0.0f, 0.0f, 1024, 1024, 1.0f, 1.0f);
	//button to return to main menu
	ButtonGoBack* bGoBack = new ButtonGoBack(L"Content/Textures/button_return.png",
		0.4f, 0.8f, 128, 64, 0.2f, 0.1f);
	mMenu->addButton(bGoBack);
	//graphic stating that you lost
	Texture2D* tLost = new Texture2D(L"Content/Textures/button_loss.png", 0.3f, 0.3f, 256, 128,
		0.4f, 0.2f);
	mMenu->addTexture(tLost);
}
/*******************************************************************
Main Menu
*******************************************************************/
void EventDisplayMenuMain::beginEvent()
{
	//main menu background
	mMenu = new Menu(L"Content/Textures/background_menu.png", 0.0f, 0.0f, 1024, 1024, 1.0f, 1.0f);
	//button to start game
	ButtonViewStory* bNewGame = new ButtonViewStory(L"Content/Textures/button_new_game.png",
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
Story Screen
*******************************************************************/
void EventDisplayMenuStory::beginEvent()
{
	mMenu = new Menu(L"Content/Textures/background_menu.png", 0.0f, 0.0f, 1024, 1024, 1.0f, 1.0f);
	ButtonViewLoading* bStartGame = new ButtonViewLoading(L"Content/Textures/background_story.png",
		0.0f, 0.0f, 1024, 1024, 1.0f, 1.0f);
	mMenu->addButton(bStartGame);
}

/*******************************************************************
Win Menu
*******************************************************************/
void EventDisplayMenuWin::beginEvent()
{
	//main menu background
	mMenu = new Menu(L"Content/Textures/background_menu.png", 0.0f, 0.0f, 1024, 1024, 1.0f, 1.0f);
	//button to return to main menu
	ButtonGoBack* bGoBack = new ButtonGoBack(L"Content/Textures/button_return.png",
		0.4f, 0.8f, 128, 64, 0.2f, 0.1f);
	mMenu->addButton(bGoBack);
	//graphic stating that you won
	Texture2D* tLost = new Texture2D(L"Content/Textures/button_win.png", 0.3f, 0.3f, 256, 128,
		0.4f, 0.2f);
	mMenu->addTexture(tLost);
}

/*******************************************************************
User Interface
*******************************************************************/
void EventDisplayUserInterface::beginEvent()
{
	//overlay
	mInterface = new UserInterface(L"Content/Textures/ui_overlay.png", 0.0f, 0.0f, 1024, 1024,
		0.99f, 1.0f);
	StatBar* sbHealthDart = new StatBar(L"Content/Textures/ui_bar_health_dart.png", 0.0f, 0.85f,
		64, 16, 0.1f, 0.05f, STAT_HEALTH_PLAYER);
	mInterface->addStatBar(sbHealthDart);
	StatBar* sbHealthBart = new StatBar(L"Content/Textures/ui_bar_health_bart.png", 0.0f, 0.9f,
		64, 16, 0.1f, 0.05f, STAT_HEALTH_FOLLOWER);
	mInterface->addStatBar(sbHealthBart);
	StatBar* sbAmmoSeeds = new StatBar(L"Content/Textures/ui_bar_ammo_seed.png", 0.88f, 0.85f,
		64, 16, 0.1f, 0.05f, STAT_AMMO_SEED);
	mInterface->addStatBar(sbAmmoSeeds);
	StatBar* sbAmmoFire = new StatBar(L"Content/Textures/ui_bar_ammo_fire.png", 0.88f, 0.9f,
		64, 16, 0.1f, 0.05f, STAT_AMMO_FIRE);
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
	Texture2D* tLeftChoice = new Texture2D(L"Content/Textures/ui_icon_left_mouse_choice.png",
		0.9f, 0.0f, 64, 64, 0.1f, 0.1f);
	mInterface->addTexture(tLeftChoice);
	Texture2D* tRightChoice = new Texture2D(L"Content/Textures/ui_icon_right_mouse_choice.png",
		0.9f, 0.0f, 64, 64, 0.1f, 0.1f);
	mInterface->addTexture(tRightChoice);
	Texture2D* tHealthDart = new Texture2D(L"Content/Textures/ui_bar_health_dart_text.png", 0.0f, 0.85f,
		64, 16, 0.1f, 0.05f);
	mInterface->addTexture(tHealthDart);
	Texture2D* tHealthBart = new Texture2D(L"Content/Textures/ui_bar_health_bart_text.png", 0.0f, 0.9f,
		64, 16, 0.1f, 0.05f);
	mInterface->addTexture(tHealthBart);
	Texture2D* tSeedAmmo = new Texture2D(L"Content/Textures/ui_bar_ammo_seed_text.png",
		0.88f, 0.85f, 64, 16, 0.1f, 0.05f);
	mInterface->addTexture(tSeedAmmo);
	Texture2D* tFireAmmo = new Texture2D(L"Content/Textures/ui_bar_ammo_fire_text.png",
		0.88f, 0.9f, 64, 16, 0.1f, 0.05f);
	mInterface->addTexture(tFireAmmo);
}

void EventDisplayUserInterface::updateEvent(float _dt)
{
	switch (gPlayer->getLeftAttack())
	{
	case A_NONE:	mInterface->moveTexture(0, 0.9f, -0.1f); break;
	case A_MELEE:	mInterface->moveTexture(0, 0.9f, 0.0f);  break;
	case A_SEED:	mInterface->moveTexture(0, 0.9f, 0.1f);  break;
	case A_FIRE:	mInterface->moveTexture(0, 0.9f, 0.2f);  break;
	}
	switch (gPlayer->getRightAttack())
	{
	case A_NONE:	mInterface->moveTexture(1, 0.9f, -0.1f); break;
	case A_MELEE:	mInterface->moveTexture(1, 0.9f, 0.0f);  break;
	case A_SEED:	mInterface->moveTexture(1, 0.9f, 0.1f);  break;
	case A_FIRE:	mInterface->moveTexture(1, 0.9f, 0.2f);  break;
	}
	mInterface->update(_dt);
}

/*******************************************************************
Start Music, stop old music
*******************************************************************/

EventMusicStart::EventMusicStart(char* _music)
{
	mFile = _music;
}

void EventMusicStart::beginEvent()
{
	FR(gSound->getSystem()->createStream(mFile, FMOD_DEFAULT, 0, &mMusic));
	gSound->mChannel->stop();
	FR(gSound->mSystem->playSound(FMOD_CHANNEL_FREE, mMusic, false, &gSound->mChannel))
	FR(gSound->mChannel->setMode(FMOD_LOOP_NORMAL));	
	FR(gSound->mChannel->setLoopCount(-1));
	FR(gSound->mChannel->setVolume(0.4f));
}

void EventMusicStart::endEvent()
{
	FR(mMusic->release());
}

/*******************************************************************
Levels
*******************************************************************/
void EventProcessLevel::beginEvent()
{
	g_levelImp->clearData();
	if (gCurrentLevel) delete gCurrentLevel;
	// load the level info from the file
	g_levelImp->loadLevel(mLevelName);
	g_levelImp->makeLevel();
	gCurrentLevel->setLevelNumber(mLevelNumber);
}
//changes loaded level
//the file name is the level AFTER the one that's loading, the event
//already has the one we're loading name
//use "" if there is no level after
void StateMachine::changeLevel(wchar_t* _newNextFile)
{
	HR(gD3DDevice->BeginScene());
	//clear screen with blue
	HR(gD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0x222255CC, 1.0f, 0));
	HR(gD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK));
	mLoadingScreen->draw();
	HR(gD3DSprite->End());
	HR(gD3DDevice->EndScene());
	// Present the back buffer.
	HR(gD3DDevice->Present(0, 0, 0, 0));
	mStates[STATE_PLAY]->getEvent(LEVEL_INDEX)->setLevel(_newNextFile);
	mStates[STATE_PLAY]->getEvent(LEVEL_INDEX)->beginEvent();
}

wchar_t* StateMachine::getNextLevelFile()
{
	return mStates[STATE_PLAY]->getEvent(LEVEL_INDEX)->getNextLevel();
}
#include "ProjectApp.h"
#include <ctime>//time
#include <cstdlib>//rand, srand
#include "Button.h"
#include "DirectInput.h"
#include "Light.h"
#include "Menu.h"
#include "Mesh.h"
#include "Player.h"
#include "Sound.h"
#include "StateMachine.h"
#include "Vertex.h"

using std::rand;
using std::srand;
using std::time;
//initialize program, create window and app, and run app
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _prevInstance, PSTR _cmdLine, int _showCmd)
{
	UNREFERENCED_PARAMETER(_prevInstance);
	UNREFERENCED_PARAMETER(_cmdLine);
	UNREFERENCED_PARAMETER(_showCmd);

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ProjectApp app(_hInstance, L"GSP361 ProjectApp", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gD3DApp = &app;

	//direct input
	DirectInput DI(DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	gDInput = &DI;
	//seed random number generator
	srand(UINT(time(0)));
	//start program run
	return gD3DApp->run();
}

ProjectApp::ProjectApp(HINSTANCE _hInstance, LPCWSTR _winCaption, D3DDEVTYPE _devType, DWORD _requestedVP)
	: D3DApp(_hInstance, _winCaption, _devType, _requestedVP)
{
	//check device caps
	if (!checkDeviceCaps())
	{
		MessageBox(0, L"checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}
	//create sprite to be used for draw2d events
	D3DXCreateSprite(gD3DDevice, &mD3DSprite);
	//create vertex that can be used
	InitAllVertexDeclarations();
	//init game components not covered by state machine beginEvents, run before initStateMachine
	initGameComponents();
	//init state machines, add events and states
	initStateMachine();
}

ProjectApp::~ProjectApp()
{
	CleanUp();
}

void ProjectApp::CleanUp()
{
	DestroyAllVertexDeclarations();//destroy vertex info
	gStateMachine->endState();//play end state of final state
	DestroyAllVertexDeclarations();//destroy vertex info
	delete gStateMachine;//delete state machine
	SAFE_RELEASE(mD3DSprite);//delete sprite
	D3DApp::CleanUp();//run D3DApp's
}

bool ProjectApp::checkDeviceCaps()
{
	D3DCAPS9 caps;
	HR(gD3DDevice->GetDeviceCaps(&caps));

	// Check for vertex shader version 2.0 support.
	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
	{
		return false;
	}

	// Check for pixel shader version 2.0 support.
	if (caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
		return false;
	}

	return true;
}

void ProjectApp::onLostDevice()
{
	mD3DSprite->OnLostDevice();
	gDInput->onLostDevice();
#if defined(DEBUG) | defined(_DEBUG)
	mFrameStats.onLostDevice();
#endif
	gStateMachine->onLostDevice();
}

void ProjectApp::onResetDevice()
{
	mD3DSprite->OnResetDevice();
	gDInput->onResetDevice();
#if defined(DEBUG) | defined(_DEBUG)
	mFrameStats.onResetDevice();
#endif
	gStateMachine->onResetDevice();
}

void ProjectApp::updateScene(float _dt)
{
	// Get snapshot of input devices
	gDInput->poll();
	//update cursor position
	gDInput->updateMouse(_dt);
#if defined(DEBUG) | defined(_DEBUG)
	mFrameStats.update(_dt);
#endif
	//update current state
	gStateMachine->updateState(_dt);
}


void ProjectApp::drawScene()
{
	HR(gD3DDevice->BeginScene());
	//clear screen with blue
	HR(gD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0x222255CC, 1.0f, 0));

	//set things that are the same for the room and models
	HR(gD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE));
	HR(gD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE));
	
	//call 3ddrawing events for the current state
	gStateMachine->drawState3D();


	//process 2d sprites as well
	HR(mD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK));
	//call 2ddrawing events for current state
	gStateMachine->drawState2D(mD3DSprite);

	//draw mouse cursor
	gDInput->drawMouse(mD3DSprite);

	HR(mD3DSprite->End());

	//display text; if it's not drawn after sprites it will be covered up
	gStateMachine->drawStateText();

#if defined(DEBUG) | defined(_DEBUG)
	mFrameStats.display();
#endif


	HR(gD3DDevice->EndScene());
	// Present the back buffer.
	HR(gD3DDevice->Present(0, 0, 0, 0));
}

void ProjectApp::initStateMachine()
{
	//create events and states
	//must be done in the order they appear in enum eStates in StateMachine.h
	gStateMachine = new StateMachine();
	/*******************************************************************
	Main Menu
	*******************************************************************/
	State* sMainMenu = new State();
	//create menu event
	EventDisplayMenuMain* eMainMenu = new EventDisplayMenuMain();
	//add event to state
	sMainMenu->addEvent(eMainMenu);
	// music
	EventMusicStart* musMain = new EventMusicStart("Content/Audio/musicMenu.mp3");
	sMainMenu->addEvent(musMain);
	//add state
	gStateMachine->addState(sMainMenu);
	/*******************************************************************
	Options Menu
	*******************************************************************/
	State* sOptions = new State();
	//create menu event
	EventDisplayMenuOptions* eOptionsMenu = new EventDisplayMenuOptions();
	//add event to state
	sOptions->addEvent(eOptionsMenu);
	// music
	EventMusicStart* musOpt = new EventMusicStart("Content/Audio/musicMenu.mp3");
	sOptions->addEvent(musOpt);
	//add state
	gStateMachine->addState(sOptions);
	/*******************************************************************
	GamePlay Level 1
	*******************************************************************/
	State* sPlay = new State();
	//Always process level first!!
	//level////////////////////////////////////////////////////////////
	EventProcessLevel* eLevel = new EventProcessLevel(L"Content/Levels/NewGoalTest.dlvl",
		L"Content/Levels/FinalLevelV1.dlvl", 1);
	sPlay->addEvent(eLevel);
	//user interface///////////////////////////////////////////////////
	//Always process user interface before player so we'll know if they
	//clicked a button during that frame
	EventDisplayUserInterface* eInterface = new EventDisplayUserInterface();
	sPlay->addEvent(eInterface);
	//camera///////////////////////////////////////////////////////////
	EventProcessCamera* eCam = new EventProcessCamera();
	sPlay->addEvent(eCam);
	//player///////////////////////////////////////////////////////////
	EventProcessPlayer* ePlayer = new EventProcessPlayer();
	sPlay->addEvent(ePlayer);
	//follower/////////////////////////////////////////////////////////
	EventProcessFollower* eFollower = new EventProcessFollower();
	sPlay->addEvent(eFollower);
	// music
	EventMusicStart* musPlay = new EventMusicStart("Content/Audio/musicPlay.mp3");
	sPlay->addEvent(musPlay);
	//add state
	gStateMachine->addState(sPlay);
	/*******************************************************************
	Loss Menu
	*******************************************************************/
	State* sLoss = new State();
	//create menu event
	EventDisplayMenuLoss* eLossMenu = new EventDisplayMenuLoss();
	//add event to state
	sLoss->addEvent(eLossMenu);
	//music
	EventMusicStart* musLoss = new EventMusicStart("Content/Audio/musicLose.mp3");
	sLoss->addEvent(musLoss);
	//add state
	gStateMachine->addState(sLoss);
	/*******************************************************************
	Win Menu
	*******************************************************************/
	State* sWin = new State();
	//create menu event
	EventDisplayMenuWin* eWinMenu = new EventDisplayMenuWin();
	//add event to state
	sWin->addEvent(eWinMenu);
	//music
	EventMusicStart* musWin = new EventMusicStart("Content/Audio/musicWin.mp3");
	sWin->addEvent(musWin);
	//add state
	gStateMachine->addState(sWin);
	/*******************************************************************
	Game Intro
	*******************************************************************/
	State* sIntro = new State();
	EventDisplayMenuStory* eStory = new EventDisplayMenuStory();
	sIntro->addEvent(eStory);
	gStateMachine->addState(sIntro);

	//start state machine
	gStateMachine->turnOn();
}

void ProjectApp::initGameComponents()
{
	//sounds
	gSound = new SoundSystem();

	//load main effect file for levels
	gEffectPrimary = new Effect(L"Content/FX/BumpMapping.fx");
	gEffectAnim = new Effect(L"Content/FX/MultiAnimation.fx");
	//create light for levels
	//light setting uses D3DXVECTOR instead of XMFLOAT because the functions they are called for
	//usually require the former
	LightSetting lights;
	lights.setAmbient(2.0f, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	lights.setDirectional(2.0f, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	lights.setSpecular(2.0f, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 5.0f);
	//set lights to effect file
	gEffectPrimary->setLighting(lights);
	gEffectAnim->setLighting(lights);
}


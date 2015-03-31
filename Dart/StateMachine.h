//This is the base class for the game states. Sub classes will override this.
//this contains the state machine, the base state class, and the base event class

#pragma once
#include <vector>
#include "Camera.h"
#include "D3DUtils.h"
#include "Level.h"
#include "LevelImporter.h"
#include "Menu.h"
#include "Player.h"
#include "Sound.h"
#include "UserInterface.h"
using std::vector;

//you have to be careful in what order you add the states in. you should add them so that
//their vector index lines up with the following enum
//state machine will start on eState 0
enum eState
{
	STATE_MAINMENU = 0,
	STATE_OPTIONSMENU,
	STATE_PLAY,
	STATE_LOSE,
	STATE_WIN,
	STATE_HIGHSCORE,
	STATE_INPUTNAME
};

class Event
{
public:
	Event() {}
	~Event(){}
	//override whichever functions are used in the child event class
	virtual void init()								{}//add parameters, this is to give needed info to event, like a menu to display
	virtual void beginEvent()						{}//do this as you enter the state
	virtual void endEvent()							{}//do this as you leave the state
	virtual void updateEvent(float _dt)				{}//do this during state update
	virtual void drawEvent3D()						{}//do this during 3d drawing
	virtual void drawEvent2D(ID3DXSprite* _sprite)	{}//do this during sprite drawing
	virtual void drawEventText()					{}//draw text on screen
	virtual void onLostDevice()						{}//do this on lost device
	virtual void onResetDevice()					{}//do this on reset device
};

//base state class to be overridden
class State
{
public:
	State();
	~State();

	void	beginState();						//run all event.beginState
	void	endState();							//run all event.endState
	void	updateState(float _dt);				//etc
	void	drawState3D();
	void	drawState2D(ID3DXSprite* _sprite);
	void    drawStateText();
	void	onLostDevice();
	void	onResetDevice();

	void	addEvent(Event* newEvent);
	void	setTransitioning(bool _bTrans) { bTransitioning = _bTrans; }
private:
	vector<Event*>	mEvents;
	//once this is set, it is going to a new state, cease processing events and go to end state
	bool			bTransitioning;
};
//the state machine
//keeps track of current state and runs all the events for the current state
class StateMachine
{
public:
	StateMachine();
	~StateMachine();
	void	beginState();						//run all state.beginState
	void	endState();							//run all state.endState
	void	updateState(float _dt);				//etc
	void	drawState3D();
	void	drawState2D(ID3DXSprite* _sprite);
	void	drawStateText();
	void	onLostDevice();
	void	onResetDevice();
	//add state or get current state
	void	addState(State* newState);
	eState	getCurrentState() { return mCurrentState; }
	void	transitionState(eState newState);//transition to new state
	void	turnOn();//initialize
	bool	getReset() { return bReset; }
	void	setReset(bool _reset) { bReset = _reset; }
private:
	vector<State*>	mStates;
	eState			mCurrentState;
	bool			bReset;
};

extern StateMachine* gStateMachine;

class EventDisplayMenuLoss : public Event
{
public:
	void beginEvent();
	void endEvent()							{ delete mMenu; }
	void updateEvent(float _dt)				{ mMenu->update(_dt); }
	void drawEvent2D(ID3DXSprite* _sprite)	{ mMenu->draw(_sprite); }
	void drawEventText()					{ mMenu->drawText(); }
	void onLostDevice()						{ mMenu->onLostDevice(); }
	void onResetDevice()					{ mMenu->onResetDevice(); }
private:
	Menu* mMenu;
};

//event to display main menu
class EventDisplayMenuMain : public Event
{
public:
	void beginEvent();
	void endEvent()							{ delete mMenu; }
	void updateEvent(float _dt)				{ mMenu->update(_dt); }
	void drawEvent2D(ID3DXSprite* _sprite)	{ mMenu->draw(_sprite); }
	void drawEventText()					{ mMenu->drawText(); }
	void onLostDevice()						{ mMenu->onLostDevice(); }
	void onResetDevice()					{ mMenu->onResetDevice(); }
private:
	Menu* mMenu;
};

//event to display options menu
class EventDisplayMenuOptions : public Event
{
public:
	void beginEvent();
	void endEvent()							{ delete mMenu; }
	void updateEvent(float _dt)				{ mMenu->update(_dt); }
	void drawEvent2D(ID3DXSprite* _sprite)	{ mMenu->draw(_sprite); }
	void drawEventText()					{ mMenu->drawText(); }
	void onLostDevice()						{ mMenu->onLostDevice(); }
	void onResetDevice()					{ mMenu->onResetDevice(); }
private:
	Menu* mMenu;
};

class EventDisplayMenuWin : public Event
{
public:
	void beginEvent();
	void endEvent()							{ delete mMenu; }
	void updateEvent(float _dt)				{ mMenu->update(_dt); }
	void drawEvent2D(ID3DXSprite* _sprite)	{ mMenu->draw(_sprite); }
	void drawEventText()					{ mMenu->drawText(); }
	void onLostDevice()						{ mMenu->onLostDevice(); }
	void onResetDevice()					{ mMenu->onResetDevice(); }
private:
	Menu* mMenu;
};


class EventDisplayUserInterface : public Event
{
public:
	void beginEvent();
	void endEvent()							{ delete mInterface; }
	void updateEvent(float _dt)				{ mInterface->update(_dt); }
	void drawEvent2D(ID3DXSprite* _sprite)	{ mInterface->draw(_sprite); }
	void drawEventText()					{ mInterface->drawText(); }
	void onLostDevice()						{ mInterface->onLostDevice(); }
	void onResetDevice()					{ mInterface->onResetDevice(); }
private:
	UserInterface* mInterface;
};

//an event to start music
class EventMusicStart : public Event
{
public:
	EventMusicStart(char* _music);
	void beginEvent();
	void updateEvent(float _dt)				{ gSound->mSystem->update(); }
	void endEvent();
private:
	char* mFile;
	Sound* mMusic;
};

//event to process the game camera
class EventProcessCamera : public Event
{
public:
	void beginEvent()
	{
		if (gCameraMain == NULL)
			gCameraMain = new Camera(D3DXVECTOR3(0.0f, 1000.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		else
			gCameraMain->init(D3DXVECTOR3(0.0f, 100.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	void updateEvent(float _dt){ gCameraMain->update(); }
	//the full screen change will crash if we delete the camera, so just keep it and reinitialize it next time
	void endEvent(){}
};
 
//event to process follower/protected AI unit
class EventProcessFollower : public Event
{
public:
	void beginEvent(){
		gFollower = new Follower(L"Content/Models/tiny.x", L"Content/Textures/Tiny_skin.dds",
			L"Content/Textures/Tiny_skin_n.dds", g_levelImp->getBart().getPos()/*D3DXVECTOR3(1000.0f, 50.0f, 0.0f)*/, 100, 32.0f, D3DXVECTOR3(0.25, 0.25f, 0.25f));
	}
	void endEvent()				{ delete gFollower;  }
	void updateEvent(float _dt)	{ gFollower->update(_dt); }
	void drawEvent3D()			{ gFollower->draw(); }
};

//event to display a given game level
class EventProcessLevel1 : public Event
{
public:
	void beginEvent();
	void updateEvent(float _dt)				{ gCurrentLevel->update(_dt); }
	void drawEvent3D()						{ gCurrentLevel->draw(); }
	void onLostDevice()						{ gCurrentLevel->onLostDevice(); }
	void onResetDevice()					{ gCurrentLevel->onResetDevice(); }
};

class EventProcessPlayer : public Event
{
	void beginEvent(){
		gPlayer = new Player(L"Content/Models/tiny.x", L"Content/Textures/Tiny_skin.dds",
			L"Content/Textures/Tiny_skin_n.dds", g_levelImp->getDart().getPos()/*D3DXVECTOR3(0.0f, 50.0f, 0.0f)*/,
			100, 40.0f, D3DXVECTOR3(0.25, 0.25f, 0.25f));
	}
	void updateEvent(float _dt)	{ gPlayer->update(_dt); }
	void drawEvent3D()			{ gPlayer->draw(); }
	void endEvent()
	{
		delete gPlayer;
	}
};
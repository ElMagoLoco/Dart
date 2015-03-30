#pragma once

#include "D3DUtils.h"
#include "Mesh.h"
//camera class to keep track of camera, view, projection, etc
//there will be a global camera for the main game view
class Camera
{
public:
	//setup camera and face it towards initial target with offset from target
	Camera(D3DXVECTOR3 _offset, D3DXVECTOR3 _target);
	void init(D3DXVECTOR3 _offset, D3DXVECTOR3 _target);
	//gets and sets
	//return position of camera 
	D3DXVECTOR3 getPosition()	{ return mPosition; }
	//return projection matrix
	D3DXMATRIX	getProjection()	{ return mProjection; }
	//return view matrix
	D3DXMATRIX	getView()		{ return mView; }
	//update to follow the player if mFollowPlayer is true
	void update();
	//initially build the projection matrix
	void buildProjection();
	//initially build the view matrix
	void buildView();
	//follow the player
	void followPlayer(bool follow)					{ bFollowPlayer = follow; }
	//change offset angle
	void setAngleOffset(float offset)	{ mOffsetAngle += offset; }
	float getAngleOffset()				{ return mOffsetAngle; }
private:
	D3DXVECTOR3 mOffset;//offset from target
	float		mOffsetAngle;//angle offset from standard north facing
	D3DXVECTOR3 mTarget;//target we are looking at
	D3DXVECTOR3 mPosition;//present position
	D3DXMATRIX	mProjection;//projection matrix
	D3DXMATRIX	mView;//view matrix
	bool		bFollowPlayer;//if we follow the player
};

extern Camera* gCameraMain;
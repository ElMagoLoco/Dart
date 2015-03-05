#include "Camera.h"

#include "Player.h"

Camera* gCameraMain = 0;
//constructor
//choose a target and put the camera at an offset to that target
//generally target is player's initial position
Camera::Camera(D3DXVECTOR3 _offset, D3DXVECTOR3 _target)
{
	init(_offset, _target);
}
void Camera::init(D3DXVECTOR3 _offset, D3DXVECTOR3 _target)
{
	mOffset = _offset;
	mTarget = _target;
	bFollowPlayer = true;
	buildView();
	buildProjection();
}
//follow the player around by changing to follow them
//offset remains the same unless changed
void Camera::update()
{
	if (bFollowPlayer)
	{
		mTarget = gPlayer->getPosition();
	}
	buildView();
	buildProjection();
}
//build projection matrix for other components to use
void Camera::buildProjection()
{
	D3DXMatrixPerspectiveFovLH(&mProjection, 0.785f, (float)gWindowWidth / (float)gWindowHeight,
		1.0f, 5000.0f);
}
//build view matrix for other components to use
void Camera::buildView()
{
	mPosition = mTarget + mOffset;
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &mPosition, &mTarget, &up);
}
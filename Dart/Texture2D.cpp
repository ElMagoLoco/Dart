#include "Texture2D.h"

//constructor, sets up basic texture variables
Texture2D::Texture2D(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX, int _sizeOrigY,
	float _sizeScreenX, float _sizeScreenY, D3DCOLOR _color) :
	mName(_name),
	mPositionX(_positionX),
	mPositionY(_positionY),
	mSizeOrigX(_sizeOrigX),
	mSizeOrigY(_sizeOrigY),
	mSizeScreenX(_sizeScreenX),
	mSizeScreenY(_sizeScreenY),
	mColor(_color),
	bProcess(true),
	bInitialized(true)
{
	// init(_name, _positionX, _positionY, _sizeOrigX, _sizeOrigY, _sizeScreenX, _sizeScreenY, _color);
	onResetDevice();
}

void Texture2D::init(LPCWSTR _name, float _positionX, float _positionY, int _sizeOrigX, int _sizeOrigY,
	float _sizeScreenX, float _sizeScreenY, D3DCOLOR _color)
{
	//name of the file itself
	mName = _name;
	mPositionX = _positionX;
	mPositionY = _positionY;
	mSizeOrigX = _sizeOrigX;
	mSizeOrigY = _sizeOrigY;
	mSizeScreenX = _sizeScreenX;
	mSizeScreenY = _sizeScreenY;
	mColor = _color;
	bProcess = true;
	bInitialized = true;
	onResetDevice();
}
//destructor
Texture2D::~Texture2D()
{
	onLostDevice();
}
void Texture2D::draw(ID3DXSprite* _sprite)
{
	if (bProcess)
	{
		//this draws the texture at a size proportionate to the screen rather than a defined
		//pixel size; allows the textures to stretch with the window
		//matrices
		D3DXMATRIX transMat, scaleMat;
		//move to proper point on window, proportionally
		D3DXMatrixTranslation(&transMat, (float)gWindowWidth * mPositionX, (float)gWindowHeight * mPositionY, 0);
		//scale proportionally to window size
		D3DXMatrixScaling(&scaleMat, (float)gWindowWidth * mSizeScreenX / (float)mSizeOrigX,
			(float)gWindowHeight * mSizeScreenY / (float)mSizeOrigY, 1.0f);
		//must multiply in this order
		D3DXMatrixMultiply(&transMat, &scaleMat, &transMat);
		//set transform and draw
		HR(_sprite->SetTransform(&transMat));
		HR(_sprite->Draw(mTexture, 0, &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			&D3DXVECTOR3(0.0f, 0.0f, 0.0f), mColor));
	}
}

void Texture2D::onLostDevice()
{
	SAFE_RELEASE(mTexture);
}

void Texture2D::onResetDevice()
{
	(D3DXCreateTextureFromFileEx(gD3DDevice, mName, mSizeOrigX,
		mSizeOrigY, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, 0, 0, &mTexture));
}
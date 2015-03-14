#pragma once
//the effect file for 3d rendering

#include "D3DUtils.h"
#include "Light.h"

class Effect
{
public:
	Effect(LPCWSTR _fileName);
	~Effect();
	void onLostDevice();
	void onResetDevice();
	void setLighting(LightSetting _settings);
	void setTextures(IDirect3DTexture9* _texture, IDirect3DTexture9* _normal);
	void setEyePos(D3DXVECTOR3 _eyePos);
	//get
	ID3DXEffect* getFX()						{ return mFX; }
	D3DXHANDLE getTechniqueHandle()				{ return mhTechnique; }
	D3DXHANDLE getWorldHandle()					{ return mhWorld; }
	D3DXHANDLE getWorldInverseTransHandle()		{ return mhWorldInverseTranspose; }
	D3DXHANDLE getWorldViewProjectionHandle()	{ return mhWorldViewProjection; }
	D3DXHANDLE getTextureHandle()				{ return mhTexture; }
	D3DXHANDLE getNormalTextureHandle()			{ return mhNormalTexture; }
	D3DXHANDLE getAmbientIntensityHandle()		{ return mhAmbientIntensity; }
	D3DXHANDLE getAmbientColorHandle()			{ return mhAmbientColor; }
	D3DXHANDLE getSpecularIntensityHandle()		{ return mhSpecularIntensity; }
	D3DXHANDLE getSpecularColorHandle()			{ return mhSpecularColor; }
	D3DXHANDLE getSpecularPowerHandle()			{ return mhSpecularPower; }
	D3DXHANDLE getDirLightIntensityHandle()		{ return mhDirLightIntensity; }
	D3DXHANDLE getDirLightColorHandle()			{ return mhDirLightColor; }
	D3DXHANDLE getDirLightDirectionHandle()		{ return mhDirLightDirection; }
	D3DXHANDLE getEyePositionHandle()			{ return mhEyePosition; }
private:
	ID3DXEffect* mFX;
	//handles to values in effect file
	//technique
	D3DXHANDLE mhTechnique;
	//world matrices
	D3DXHANDLE mhWorld;
	D3DXHANDLE mhWorldInverseTranspose;
	D3DXHANDLE mhWorldViewProjection;
	//textures
	D3DXHANDLE mhTexture;
	D3DXHANDLE mhNormalTexture;
	//ambient light
	D3DXHANDLE mhAmbientIntensity;
	D3DXHANDLE mhAmbientColor;
	//specular light
	D3DXHANDLE mhSpecularIntensity;
	D3DXHANDLE mhSpecularColor;
	D3DXHANDLE mhSpecularPower;
	//directional light
	D3DXHANDLE mhDirLightDirection;
	D3DXHANDLE mhDirLightColor;
	D3DXHANDLE mhDirLightIntensity;
	//other
	D3DXHANDLE mhEyePosition;
};

extern Effect* gEffectPrimary;
extern Effect* gEffectAnim;
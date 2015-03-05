#pragma once
//I've used D3DXVECTORs here instead of XMFLOATs because these values get used in set float array
//a lot which requires D3DXVECTORs
#include "D3DUtils.h"
//struct to hold light settings
struct LightSetting
{
	//set ambient intensity or color
	void setAmbient(float _intensity, D3DXVECTOR4 _color)
	{
		mAmbientIntensity = _intensity; mAmbientColor = _color;
	}
	//set directional intensity, color, direction
	void setDirectional(float _intensity, D3DXVECTOR4 _color, D3DXVECTOR3 _direction)
	{
		mDirectionalIntensity = _intensity; mDirectionalColor = _color;
		mDirectionalDirection = _direction;
	}
	//set specular intensity, color, power
	void setSpecular(float _intensity, D3DXVECTOR4 _color, float _power)
	{
		mSpecularIntensity = _intensity; mSpecularColor = _color;
		mSpecularPower = _power;
	}
	//variables
	//ambient
	float mAmbientIntensity;
	D3DXVECTOR4 mAmbientColor;
	//directional
	float mDirectionalIntensity;
	D3DXVECTOR4 mDirectionalColor;
	D3DXVECTOR3 mDirectionalDirection;
	//specular
	float mSpecularIntensity;
	D3DXVECTOR4 mSpecularColor;
	float mSpecularPower;
};
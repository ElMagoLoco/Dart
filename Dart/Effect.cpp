#include "Effect.h"

Effect* gEffectPrimary = 0;
Effect* gEffectAnim = 0;
//create effect file
//so far we are only using the one effect file for everything shown
Effect::Effect(LPCWSTR _fileName)
{
	//create effect
	ID3DXBuffer* errors = NULL;
	D3DXCreateEffectFromFile(gD3DDevice, _fileName, NULL, NULL, D3DXSHADER_DEBUG, 0,
		&mFX, &errors);
	//get and show any errors that were made in the effect file
	if (errors != NULL)
	{
		MessageBox(NULL, (LPCWSTR)errors->GetBufferPointer(), L"D3DXCreateEffectFromFIle",
			MB_ICONERROR);
		DebugBreak();
	}
	//get handles to values in effect file
	//if those semantics aren't used in the file, these will be made Null
	mhTechnique = mFX->GetTechniqueByName("Main");
	mhWorld = mFX->GetParameterBySemantic(0, "WORLD");
	mhWorldInverseTranspose = mFX->GetParameterBySemantic(0, "WORLDINVTRANS");
	mhWorldViewProjection = mFX->GetParameterBySemantic(0, "WORLDVIEWPROJ");
	mhTexture = mFX->GetParameterBySemantic(0, "TEXTURE");
	mhNormalTexture = mFX->GetParameterBySemantic(0, "NORMALTEX");
	mhAmbientIntensity = mFX->GetParameterBySemantic(0, "AMBIENTINTENSITY");
	mhAmbientColor = mFX->GetParameterBySemantic(0, "AMBIENTCOLOR");
	mhSpecularIntensity = mFX->GetParameterBySemantic(0, "SPECULARINTENSITY");
	mhSpecularColor = mFX->GetParameterBySemantic(0, "SPECULARCOLOR");
	mhSpecularPower = mFX->GetParameterBySemantic(0, "SPECULARPOWER");
	mhDirLightDirection = mFX->GetParameterBySemantic(0, "LIGHTDIRECTIONDIR");
	mhDirLightColor = mFX->GetParameterBySemantic(0, "LIGHTCOLORDIR");
	mhDirLightIntensity = mFX->GetParameterBySemantic(0, "LIGHTINTENSITYDIR");
	mhEyePosition = mFX->GetParameterBySemantic(0, "EYEPOS");
}
//destructor
Effect::~Effect()
{
	delete mFX;
}

void Effect::onLostDevice()
{
	mFX->OnLostDevice();
}

void Effect::onResetDevice()
{
	mFX->OnResetDevice();
}
//set up lighting for the effects
void Effect::setLighting(LightSetting _settings)
{
	//for each lighting variable, check to make sure it's not null and set
	//it if it isn't
	if (mhAmbientIntensity)
		HR(mFX->SetFloat(mhAmbientIntensity, _settings.mAmbientIntensity));
	if (mhAmbientColor)
		HR(mFX->SetFloatArray(mhAmbientColor, _settings.mAmbientColor, 4));
	if (mhDirLightIntensity)
		HR(mFX->SetFloat(mhDirLightIntensity, _settings.mDirectionalIntensity));
	if (mhDirLightColor)
		HR(mFX->SetFloatArray(mhDirLightIntensity, _settings.mDirectionalColor, 4));
	if (mhDirLightDirection)
		HR(mFX->SetFloatArray(mhDirLightDirection, _settings.mDirectionalDirection, 3));
	if (mhSpecularIntensity)
		HR(mFX->SetFloat(mhSpecularIntensity, _settings.mSpecularIntensity));
	if (mhSpecularColor)
		HR(mFX->SetFloatArray(mhSpecularColor, _settings.mSpecularColor, 4));
	if (mhSpecularPower)
		HR(mFX->SetFloat(mhSpecularPower, _settings.mSpecularPower));
}
//set the diffuse and normal texture during mesh drawing
void Effect::setTextures(IDirect3DTexture9* _texture, IDirect3DTexture9* _normal)
{
	if (mhTexture && mhNormalTexture)
	{
		HR(mFX->SetTexture(mhTexture, _texture));
		HR(mFX->SetTexture(mhNormalTexture, _normal));
	}
	else
	{
		OutputDebugString(L"Invalid handles for effect textures.\n");
		PostQuitMessage(0);
	}
}

void Effect::setEyePos(D3DXVECTOR3 _eyePos)
{   
	float eyePos[3] = { _eyePos.x, _eyePos.y, _eyePos.z };
	HR(mFX->SetFloatArray(mhEyePosition, eyePos, 3));
}
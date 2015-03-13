#include "FxManager.h"

using std::wstring;
using std::vector;

FxManager::FxManager()
{
}


FxManager::~FxManager()
{
	for (UINT i = 0; i < m_fxDataList.size(); ++i)
		SAFE_RELEASE(m_fxDataList[i].pFx);
	SAFE_RELEASE(m_pFxPool);
}

int FxManager::loadFx(const wstring& fileName, const wstring& techName, const wstring& worldName)
{
	FxData9		data;
	ID3DXBuffer*	errs;

	HR(D3DXCreateEffectFromFile(gD3DDevice, fileName.c_str(), NULL,
											NULL, D3DXFX_NOT_CLONEABLE | D3DXFX_LARGEADDRESSAWARE,
											m_pFxPool, &data.pFx, &errs));

	if (errs) {
		MessageBox(0, (wchar_t*)errs->GetBufferPointer(), 0, 0);
		HR(E_FAIL);
		SAFE_RELEASE(errs);
		return -1;
	}
	SAFE_RELEASE(errs);

	char temp[256];
	size_t numChange;
	wcstombs_s(&numChange, temp, techName.c_str(), 256);
	data.hTech = data.pFx->GetTechniqueByName(temp);
	wcstombs_s(&numChange, temp, worldName.c_str(), 256);
	data.hWorld = data.pFx->GetParameterByName(0, temp);

	// if we are doing this for the first time, grab the shared variable handles
	if (m_fxDataList.size() == 0) {
		m_hLightDiffuse = data.pFx->GetParameterByName(0, "gLightDiffuse");
		m_hLightAmbient = data.pFx->GetParameterByName(0, "gLightAmbient");
		m_hLightDir = data.pFx->GetParameterByName(0, "gLightDir");
		m_hViewProj = data.pFx->GetParameterByName(0, "gViewProj");
		m_hTexDiffuse = data.pFx->GetParameterByName(0, "gTexDiffuse");
		m_hMtrlAmbient = data.pFx->GetParameterByName(0, "gMtrlAmbientColor");
		m_hMtrlDiffuse = data.pFx->GetParameterByName(0, "gMtrlDiffuseColor");
		m_hMtrlSpecular = data.pFx->GetParameterByName(0, "gMtrlSpecularColor");
		m_hSpecularExp = data.pFx->GetParameterByName(0, "gSpecularExp");
		m_hView = data.pFx->GetParameterByName(0, "gView");
		m_hWorld = data.pFx->GetParameterByName(0, "gWorld");
		m_hWVP = data.pFx->GetParameterByName(0, "gWVP");
		m_hEye = data.pFx->GetParameterByName(0, "gEye");
		m_hLightPos = data.pFx->GetParameterByName(0, "gLightPos");
		m_hLightSpecular = data.pFx->GetParameterByName(0, "gLightSpecular");
		m_hWIT = data.pFx->GetParameterByName(0, "gWIT");
	}

	m_fxDataList.push_back(data);	

	return m_fxDataList.size() - 1;
}

void FxManager::setFxVarWorld(const int id, const D3DXMATRIX& world)
{
	HR(m_fxDataList[id].pFx->SetMatrix(m_fxDataList[id].hWorld, &world));
}

FxManager::FxData9& FxManager::getFx(int id)
{
	return m_fxDataList[id];
}

UINT FxManager::getNumEffectsLoaded()
{
	return m_fxDataList.size();
}

void FxManager::createFxPool()
{
	HR(D3DXCreateEffectPool(&m_pFxPool));
}

void FxManager::setMtrlAmbient(D3DXVECTOR4& c)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hMtrlAmbient, &c));
}

void FxManager::setMtrlDiffuse(D3DXVECTOR4& c)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hMtrlDiffuse, &c));
}

void FxManager::setMtrlSpecular(D3DXVECTOR4& c)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hMtrlSpecular, &c));

}

void FxManager::setLightDir(D3DXVECTOR4& v)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hLightDir, &v));
}

void FxManager::setLightDiffuse(D3DXVECTOR4& c)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hLightDiffuse, &c));

}

void FxManager::setLightAmbient(D3DXVECTOR4& c)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hLightAmbient, &c));
}

void FxManager::setSpecularExp(float s)
{
	HR(m_fxDataList[0].pFx->SetFloat(m_hSpecularExp, s));
}

void FxManager::setViewProj(const D3DXMATRIX& vp)
{
	HR(m_fxDataList[0].pFx->SetMatrix(m_hViewProj, &vp));
}

void FxManager::setEye(const D3DXVECTOR4& pos)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hEye, &pos));
}

void FxManager::setWVP(const D3DXMATRIX& wvp)
{
	HR(m_fxDataList[0].pFx->SetMatrix(m_hWVP, &wvp));
}

void FxManager::setView(const D3DXMATRIX& view)
{
	HR(m_fxDataList[0].pFx->SetMatrix(m_hView, &view));
}

void FxManager::setWorld(const D3DXMATRIX& world)
{
	HR(m_fxDataList[0].pFx->SetMatrix(m_hWorld, &world));
}

void FxManager::setLightPos(D3DXVECTOR4& v)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hLightPos, &v));
}

void FxManager::setLightSpecular(D3DXVECTOR4& c)
{
	HR(m_fxDataList[0].pFx->SetVector(m_hLightSpecular, &c));
}

void FxManager::setWIT(D3DXMATRIX& wit)
{
	HR(m_fxDataList[0].pFx->SetMatrix(m_hWIT, &wit));
}

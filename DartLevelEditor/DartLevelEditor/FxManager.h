/*
 * FILE: FxManager.h
 * DATE: 2015/03/09 20:41
 
 * AUTHOR: Sam
 * CONTACT: theebola8me@gmail.com
 
 * BRIEF: can create and store multiple shader FX files in one vector for easy access.
	Also has the ability to use shared variable names with the different shaders to ease 
	in shader programming.
 
 * LONG NOTES: To begin call createFXPool() before doing anything with the manager.
	Make sure that you capture the int that is returned in loadFx so that you can access the 
	shader at will from the vector. Use getFX() with the captured int to access the shader.
	There are multiple set functions for all the shared variables.
*/
#pragma once
#include <vector>
#include <string>
#include "d3dUtils.h"

class FxManager
{
public:
	// this holds specific effect variables
	struct FxData9 {
		ID3DXEffect*	pFx;		// the effect pointer
		D3DXHANDLE	hTech;	// handle to the technique
		D3DXHANDLE	hWorld; // this may be a bit of overkill
	};

	//************************************
	// Method:    loadFx
	// FullName:  FxManager::loadFx
	// Access:    public 
	// Returns:   int
	// - you most capture this int in order to access the fx
	// - returns -1 if anything fails
	// Qualifier:
	// Parameter: const std::wstring & fileName
	//	- the file name of the fx file
	// Parameter: const std::wstring & techName
	// - the technique name
	// Parameter: const std::wstring & worldName
	// - the name of the world variable in the shader
	// NOTES: It is important that you capture the into returned so that you can 
	//	properly access the shader from the vector list at will
	//************************************
	int loadFx(const std::wstring& fileName, const std::wstring& techName, const std::wstring& worldName);

	//************************************
	// Method:    setFxVarWorld
	// FullName:  FxManager::setFxVarWorld
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: const int id
	// - the id of the shader
	// Parameter: const D3DXMATRIX & world
	// - the world matrix you are going to set
	//************************************
	void setFxVarWorld(const int id, const D3DXMATRIX& world);


	//************************************
	// Method:    getNumEffectsLoaded
	// FullName:  FxManager::getNumEffectsLoaded
	// Access:    public 
	// Returns:   UINT
	//	- the number of effects currently loaded
	// Qualifier:
	// NOTES: TODO - I think this is irrelevant and not needed
	//************************************
	UINT getNumEffectsLoaded();

	//************************************
	// Method:    getFx
	// FullName:  FxManager::getFx
	// Access:    public 
	// Returns:   FxData9&
	// - the effect referenced from the ID sent in
	// Qualifier:
	// Parameter: int id
	// - the ID previously captured when created
	// Notes: NONE
	//************************************
	FxData9& getFx(int id);


	//************************************
	// Method:    createFxPool
	// FullName:  FxManager::createFxPool
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Notes: IMPORTANT - You MOST call this function first
	// before doing anything else with the manager
	//************************************
	void createFxPool();

	/************************************************************************/
	/* functions that set shared handles                                                                     */
	/************************************************************************/
	void setMtrlAmbient(D3DXVECTOR4& c);
	void setMtrlDiffuse(D3DXVECTOR4& c);
	void setMtrlSpecular(D3DXVECTOR4& c);
	void setLightDir(D3DXVECTOR4& v);
	void setLightDiffuse(D3DXVECTOR4& c);
	void setLightAmbient(D3DXVECTOR4& c);
	void setLightSpecular(D3DXVECTOR4& c);
	void setLightPos(D3DXVECTOR4& v);
	void setSpecularExp(float s);
	void setViewProj(const D3DXMATRIX& vp);
	void setEye(const D3DXVECTOR4& pos);
	void setWVP(const D3DXMATRIX& wvp);
	void setView(const D3DXMATRIX& view);
	void setWorld(const D3DXMATRIX& world);
	void setWIT(D3DXMATRIX& wit);
	
	FxManager();
	~FxManager();

private:
	ID3DXEffectPool*		m_pFxPool;		// the pool so that we can share variables between shaders
	std::vector<FxData9>		m_fxDataList; // the list of effects loaded

	// the shared fx variable handles
	D3DXHANDLE		// matrix handles
							m_hViewProj, // view projection
							m_hView,
							m_hWorld,
							m_hWVP,		// world view projection
							m_hWIT,		// world inverse transpose
							m_hEye,			// the position of the camera
							// light handles
							m_hLightDir,
							m_hLightDiffuse,
							m_hLightAmbient,
							m_hLightSpecular,
							m_hLightPos,
							// material handles
							m_hMtrlDiffuse,
							m_hMtrlAmbient,
							m_hMtrlSpecular,
							m_hSpecularExp,
							// texture handles
							m_hTexDiffuse;
};


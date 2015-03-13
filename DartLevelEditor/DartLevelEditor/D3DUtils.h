//D3DUtils.h
//Contains includes, debugging, globals, and other utility functions
#pragma once

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//type of vertex to use
#define VERTEX VertexPNTaT
//enable extra D3D debugging
#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif


//d3d includes and libraries
#include <afxwin.h>
#include <afxdlgs.h>
//#include <afxcmn.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>

#pragma comment(lib, "d3d9.lib")
#if defined(DEBUG) | defined(_DEBUG)
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif
#pragma comment(lib, "dxerr.lib")//used for HR macro, DxTraceW located here

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
//clean up
#define SAFE_RELEASE(x)    { if (x) { x->Release(); x = 0; } }

#include <vector>


//globals for convenient access

class D3DApp;

extern D3DApp*				gD3DApp;
extern IDirect3DDevice9*	gD3DDevice;
extern UINT					gWindowWidth;
extern UINT					gWindowHeight;

#include "D3DApp.h"
#include "DXInput.h"

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if (FAILED(hr))                                \
				{                                              \
			MessageBox(NULL/*gD3DApp->getMainWnd()*/, DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK);\
				}                                              \
	}
#endif
//			DXTrace(__FILE__, __LINE__, hr, DXGetErrorString(hr), TRUE);
#else
#ifndef HR
#define HR(x) x;
#endif
#endif 

//some utility functions
//////////////////////////////////////////////////////////////////////////

//===============================================================
// Geometry generation.
#define	TRIGRID_FVF		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE

void genTriGrid(int numVertRows,
					int numVertCols,
					float dx,
					float dz,
					const D3DXVECTOR3& center,
					std::vector<D3DXVECTOR3>& verts,
					std::vector<DWORD>& indices);

float getRandomFloat(float _low, float _high);

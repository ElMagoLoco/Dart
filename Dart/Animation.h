#pragma once
/////////////////////////////////////////////////
//
// Code from animation tutorial
//
// Author: Jason Jurecka
//
// Creation Date: June 9, 2003
//
//http://www.gamedev.net/page/resources/_/technical/game-programming/working-with-the-directx-x-file-format-and-animation-in-directx-90-r2079
//
/////////////////////////////////////////////////
#include "D3DUtils.h"


//derived animation frame, builds on default frame
typedef struct _D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIX matCombined;//combined transformation matrix
}FRAME, *LPFRAME;

//derived mesh container, builds on default container
typedef struct _D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	//mesh variables
	LPDIRECT3DTEXTURE9* ppTextures;//textures of the mesh
	D3DMATERIAL9* pMaterials9;//use direct x 9 material type
	//skinned mesh variables
	LPD3DXMESH pSkinMesh;//skin mesh
	//	LPD3DXMESH pOrigMesh;//original mesh
	//	D3DXMATRIX* pBoneMatrices;//bones for the mesh
	LPD3DXMATRIX pBoneOffsets;//bone matrix offsets
	LPD3DXMATRIX* ppFrameMatrices;//the frame matrix
	//attribute table stuff
	LPD3DXATTRIBUTERANGE pAttributeTable;//attribute table
	DWORD NumAttributeGroups;//number of attribute groups
}MESHCONTAINER, *LPMESHCONTAINER;


// Purpose: This is an Allocation class that is
//		used with the D3DXLoadMeshHierarchyFromX
//		function. It handles the Creation and Deletion
//		of Frames and Mesh Containers. The overloaded
//		functions are callbacks so there is no need
//		to call any of the functions in written code
//		just pass an Object of this class to the function

class AnimAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	//create a frame 1) name of frame 2) output new frame
	STDMETHOD(CreateFrame)(THIS_ LPCTSTR Name, LPD3DXFRAME *ppNewFrame);
	//create mesh container
	//1) name of mesh 2) mesh data 3) materials of mesh 4) effects on the mesh
	//5) number of materials in mesh 6) adjacency array for mesh
	//7)skin info for mesh 8) output mesh container
	STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR Name, LPD3DXMESHDATA pMeshData,
		LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials,
		DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer);
	//destroy frame, the frame to destroy
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	//destroy mesh container, the container to destroy
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

};


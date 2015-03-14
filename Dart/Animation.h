#pragma once
/////////////////////////////////////////////////
//

#define MAX_NUM_BONES_SUPPORTED 35

#include "D3DUtils.h"
struct FrameEx : public D3DXFRAME
{
	D3DXMATRIX	toRoot;
};

class AllocMeshHierarchy : public ID3DXAllocateHierarchy
{
public:
	HRESULT STDMETHODCALLTYPE CreateFrame(
		THIS_ PCSTR Name,
		D3DXFRAME** ppNewFrame);
	HRESULT STDMETHODCALLTYPE CreateMeshContainer(
		PCSTR Name,
		const D3DXMESHDATA* pMeshData,
		const D3DXMATERIAL* pMaterials,
		const D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		const DWORD* pAdjacency,
		ID3DXSkinInfo* pSkinInfo,
		D3DXMESHCONTAINER** ppNewMeshContainer);
	HRESULT STDMETHODCALLTYPE DestroyFrame(
		THIS_ D3DXFRAME* pFrameToFree);
	HRESULT STDMETHODCALLTYPE DestroyMeshContainer(
		THIS_ D3DXMESHCONTAINER* pMeshContainerBase);
};
/*
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
*/

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
/*
	HRESULT STDMETHODCALLTYPE CreateFrame(
		THIS_ LPCWSTR Name,
		D3DXFRAME** ppNewFrame);
	HRESULT STDMETHODCALLTYPE CreateMeshContainer(
		LPCWSTR Name,
		const D3DXMESHDATA* pMeshData,
		const D3DXMATERIAL* pMaterials,
		const D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		const DWORD* pAdjacency,
		ID3DXSkinInfo* pSkinInfo,
		D3DXMESHCONTAINER** ppNewMeshContainer);
*/
	HRESULT STDMETHODCALLTYPE CreateFrame(
		THIS_ PCSTR Name,
		D3DXFRAME** ppNewFrame);
	HRESULT STDMETHODCALLTYPE CreateMeshContainer(
		PCSTR Name,
		const D3DXMESHDATA* pMeshData,
		const D3DXMATERIAL* pMaterials,
		const D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		const DWORD* pAdjacency,
		ID3DXSkinInfo* pSkinInfo,
		D3DXMESHCONTAINER** ppNewMeshContainer);
	HRESULT STDMETHODCALLTYPE DestroyFrame(
		THIS_ D3DXFRAME* pFrameToFree);
	HRESULT STDMETHODCALLTYPE DestroyMeshContainer(
		THIS_ D3DXMESHCONTAINER* pMeshContainerBase);
};


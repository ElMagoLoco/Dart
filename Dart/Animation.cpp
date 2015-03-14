#include "Animation.h"

HRESULT STDMETHODCALLTYPE AllocMeshHierarchy::CreateFrame(
	THIS_ PCSTR Name,
	D3DXFRAME** ppNewFrame)
{
	// Remark: CreateFrame must return a non-null value for ppNewFrame,
	// otherwise D3DXLoadMeshHierarchyFromX will interpret it as 
	// a failed operation.


	FrameEx* frameEx = new FrameEx();

	if (Name)	CopyString(Name, &frameEx->Name);
	else		CopyString("<no name>", &frameEx->Name);

	frameEx->pMeshContainer = 0;
	frameEx->pFrameSibling = 0;
	frameEx->pFrameFirstChild = 0;
	D3DXMatrixIdentity(&frameEx->TransformationMatrix);
	D3DXMatrixIdentity(&frameEx->toRoot);

	*ppNewFrame = frameEx;

	return D3D_OK;
/*	//create frame using derived struct
	LPFRAME pFrame = new FRAME;
	ZeroMemory(pFrame, sizeof(FRAME));
	//initialize passed in frame
	*ppNewFrame = NULL;
	//put the name in the frame
	if (Name)
	{
		int nNameSize = lstrlen(Name) + 1;
		pFrame->Name = new char[nNameSize];
		memcpy(pFrame->Name, Name, nNameSize * sizeof(char));
	}
	else
		pFrame->Name = NULL;
	//initialize rest of frame
	pFrame->pFrameFirstChild = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pMeshContainer = NULL;
	D3DXMatrixIdentity(&pFrame->matCombined);
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	//set output frame to one we have
	*ppNewFrame = (LPD3DXFRAME)pFrame;
	//no longer points to frame
	pFrame = NULL;
	//return HRESULT
	return S_OK;*/
}

HRESULT STDMETHODCALLTYPE AllocMeshHierarchy::CreateMeshContainer(
	PCSTR Name,
	const D3DXMESHDATA* pMeshData,
	const D3DXMATERIAL* pMaterials,
	const D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials,
	const DWORD* pAdjacency,
	ID3DXSkinInfo* pSkinInfo,
	D3DXMESHCONTAINER** ppNewMeshContainer)
{
	// Remark: CreateMeshContainer should always return a non-null value
	// for ppNewMeshContainer, even if we are not interested in the mesh 
	// (i.e., it contains no skin info), otherwise D3DXLoadMeshHierarchyFromX
	// will interpret it as  a failed operation.


	//===============================================================
	// Allocate a new D3DXMESHCONTAINER, and set its name.  

	D3DXMESHCONTAINER* meshContainer = new D3DXMESHCONTAINER();
	::ZeroMemory(meshContainer, sizeof(D3DXMESHCONTAINER));	
	if( Name )	CopyString(Name, &meshContainer->Name);
	else        CopyString("<no name>", &meshContainer->Name);


	//===============================================================
	// Save our created mesh container now because we might return 
	// early, and we must _always_ return an allocated container.

	*ppNewMeshContainer = meshContainer;


	//===============================================================
	// Only interested in meshes with skin info and regular meshes.
	// If a given mesh does not satisfy these requirements we do
	// not return an error; rather we simply skip loading anything
	// for this mesh container.

	if( pSkinInfo == 0 || pMeshData->Type != D3DXMESHTYPE_MESH)
		return D3D_OK;


	//===============================================================
	// Copy material data, and allocate memory for texture file names.

	meshContainer->NumMaterials = NumMaterials;
	meshContainer->pMaterials   = new D3DXMATERIAL[NumMaterials];
	for(DWORD i = 0; i < NumMaterials; ++i)
	{
		D3DXMATERIAL* mtrls = meshContainer->pMaterials;
		mtrls[i].MatD3D = pMaterials[i].MatD3D;
		mtrls[i].MatD3D.Ambient = pMaterials[i].MatD3D.Diffuse;

		CopyString(pMaterials[i].pTextureFilename, 
			&mtrls[i].pTextureFilename);
	}


	//===============================================================
	// Ignore effect instances and adjacency info for this demo.

	meshContainer->pEffects   = 0;
	meshContainer->pAdjacency = 0;


	//===============================================================
	// Save mesh and skininfo.

	meshContainer->MeshData.Type  = D3DXMESHTYPE_MESH;
	meshContainer->MeshData.pMesh = pMeshData->pMesh; 
	meshContainer->pSkinInfo      = pSkinInfo;
	pMeshData->pMesh->AddRef();
	pSkinInfo->AddRef();

	return D3D_OK;


/*	//create temp mesh container using derived struct
	LPMESHCONTAINER pMeshContainer = new MESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(MESHCONTAINER));
	//initialize passed in container
	*ppNewMeshContainer = NULL;
	if (Name)
	{
		//put in then name
		int nNameSize = lstrlen(Name) + 1;
		pMeshContainer->Name = new char(nNameSize);
		memcpy(pMeshContainer->Name, Name, nNameSize * sizeof(char));
	}
	else
		pMeshContainer->Name = NULL;

	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//get number of faces for adjacency
	DWORD dwFaces = pMeshData->pMesh->GetNumFaces();
	//get initialize other data
	pMeshContainer->NumMaterials = NumMaterials;
	//create arrays for materials and textures
	pMeshContainer->pMaterials9 = new D3DMATERIAL9[pMeshContainer->NumMaterials];
	//multiply by 3 because there are 3 adjacent triangles
	pMeshContainer->pAdjacency = new DWORD[dwFaces * 3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * dwFaces * 3);
	//get device to use
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;//direct3d rendering device
	pMeshData->pMesh->GetDevice(&pd3dDevice);

	pMeshData->pMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshData->pMesh->GetFVF(), pd3dDevice,
		&pMeshContainer->MeshData.pMesh);

	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[NumMaterials];
	for (DWORD dw = 0; dw < NumMaterials; ++dw)
	{
		pMeshContainer->ppTextures[dw] = NULL;
		if (pMaterials[dw].pTextureFilename && strlen(pMaterials[dw].pTextureFilename) > 0)
		{
			//convert filename from LPSTR to LPCWSTR for create function
			WCHAR w[256];
			MultiByteToWideChar(CP_ACP, 0, pMaterials[dw].pTextureFilename, -1,
				w, 256);
			//create if possible
			if (FAILED(D3DXCreateTextureFromFile(pd3dDevice, w,
				&pMeshContainer->ppTextures[dw])))
			{
				pMeshContainer->ppTextures[dw] = NULL;
			}
		}
	}
	//release device
	SAFE_RELEASE(pd3dDevice);

	if (pSkinInfo)
	{
		//save off skin info and original mesh data
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		//need an array of offset matrices to move vertices from figure space to bone's space
		UINT uBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsets = new D3DXMATRIX[uBones];
		//create arrays for bones and frame matrices
		pMeshContainer->ppFrameMatrices = new D3DXMATRIX*[uBones];
		//get each of bone offset matrices so we don't need to get them later
		for (UINT i = 0; i < uBones; ++i)
			pMeshContainer->pBoneOffsets[i] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
	}
	else
	{
		pMeshContainer->pSkinInfo = NULL;
		pMeshContainer->pBoneOffsets = NULL;
		pMeshContainer->pSkinMesh = NULL;
		pMeshContainer->ppFrameMatrices = NULL;
	}

	pMeshContainer->pMaterials = NULL;
	pMeshContainer->pEffects = NULL;
	//set output mesh container to temp one
	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;
	//return
	return S_OK;*/
}

HRESULT STDMETHODCALLTYPE AllocMeshHierarchy::DestroyFrame(
	THIS_ D3DXFRAME* pFrameToFree)
{
	delete[] pFrameToFree->Name;
	delete pFrameToFree;

	return D3D_OK;
	/*
	//convert frame
	LPFRAME pFrame = (LPFRAME)pFrameToFree;
	//delete name
	SAFE_DELETE_ARRAY(pFrame->Name);
	//delete frame
	SAFE_DELETE(pFrame);
	//return
	return S_OK;*/
}

HRESULT STDMETHODCALLTYPE AllocMeshHierarchy::DestroyMeshContainer(
	THIS_ D3DXMESHCONTAINER* pMeshContainerBase)
{
	delete[] pMeshContainerBase->Name;
	delete[] pMeshContainerBase->pAdjacency;
	delete[] pMeshContainerBase->pEffects;

	for (DWORD i = 0; i < pMeshContainerBase->NumMaterials; ++i)
	{
		delete[] pMeshContainerBase->pMaterials[i].pTextureFilename;
	}

	delete[] pMeshContainerBase->pMaterials;

	SAFE_RELEASE(pMeshContainerBase->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainerBase->pSkinInfo);

	delete pMeshContainerBase;

	return D3D_OK;
	/*
	//convert to derived struct
	LPMESHCONTAINER pMeshContainer = (LPMESHCONTAINER)pMeshContainerBase;
	//if there is name
	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	//if there are materials
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials9);
	//release textures
	if (pMeshContainer->ppTextures)
		for (UINT i = 0; i < pMeshContainer->NumMaterials; ++i)
			SAFE_RELEASE(pMeshContainer->ppTextures[i]);
	//if there are textures
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	//if there is adjacency data
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	//if there are bone parts
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsets);
	//if there are frame matrices
	SAFE_DELETE_ARRAY(pMeshContainer->ppFrameMatrices);

	SAFE_DELETE_ARRAY(pMeshContainer->pAttributeTable);
	//if copy of the mesh here
	SAFE_RELEASE(pMeshContainer->pSkinMesh);
	//if there is mesh
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	//if there is skin info
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	//delete mesh container
	SAFE_DELETE(pMeshContainer);

	return S_OK;*/
}
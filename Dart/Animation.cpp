#include "Animation.h"

HRESULT AnimAllocateHierarchy::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
	//create frame using derived struct
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
	return S_OK;
}

HRESULT AnimAllocateHierarchy::CreateMeshContainer(LPCTSTR Name, LPD3DXMESHDATA pMeshData,
	LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials,
	DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	//create temp mesh container using derived struct
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
	return S_OK;
}

HRESULT AnimAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	//convert frame
	LPFRAME pFrame = (LPFRAME)pFrameToFree;
	//delete name
	SAFE_DELETE_ARRAY(pFrame->Name);
	//delete frame
	SAFE_DELETE(pFrame);
	//return
	return S_OK;
}

HRESULT AnimAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
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

	return S_OK;
}
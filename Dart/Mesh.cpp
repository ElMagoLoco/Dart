#include "Mesh.h"

//base mesh constructor, sets all the basics
Mesh::Mesh(LPCWSTR _mesh, D3DXVECTOR3 _position, D3DXVECTOR3 _scale, D3DXVECTOR3 _rotation,
	UINT _subsets) :
	mPosition(_position), mScale(_scale), mRotation(_rotation), mNumSubsets(_subsets),
	bVisible(true), bTextured(false)
{
	//some classes may only load meshes if in debug mode, like path finding nodes
	if (_mesh != L"")
		loadMesh(_mesh);
	//set the world matrix
	doWorld();
}

void Mesh::loadMesh(LPCWSTR _mesh)
{
	//mesh
	mMesh = NULL;
	//load and optimize mesh
	ID3DXBuffer* pAdjacency = NULL; //required for optimize-in-place
	HR(D3DXLoadMeshFromX(_mesh, D3DXMESH_MANAGED, gD3DDevice, &pAdjacency, 0, 0, 0, &mMesh));

	HR(mMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(), 0, 0, 0));

	//Done with adjacency buffer 
	pAdjacency->Release();
}
//destructor
Mesh::~Mesh()
{
	clearTextures();
	SAFE_RELEASE(mMesh);
}
//when a change occurs with the mesh, it will sometimes need to redo the world matrix
void Mesh::doWorld()
{
	//matrices
	D3DXMATRIX Rotation;
	D3DXMATRIX Scaling;
	D3DXMATRIX Translation;
	//rotation
	D3DXMatrixRotationYawPitchRoll(&Rotation, mRotation.y, mRotation.x, mRotation.z);
	//scale
	D3DXMatrixScaling(&Scaling, mScale.x, mScale.y, mScale.z);
	//movement
	D3DXMatrixTranslation(&Translation, mPosition.x, mPosition.y, mPosition.z);
	// must multiply in this order to work correctly
	mWorld = Rotation * Scaling * Translation;
}
//add textures to mesh, add in diffuse/normal groups
void Mesh::addTexture(LPCWSTR _texture, LPCWSTR _normal)
{
	IDirect3DTexture9* textureTex;
	IDirect3DTexture9* normalTex;
	//create textures
	HR(D3DXCreateTextureFromFile(gD3DDevice, _texture, &textureTex));
	HR(D3DXCreateTextureFromFile(gD3DDevice, _normal, &normalTex));
	//add textures to vectors
	mTextures.push_back(textureTex);
	mNormals.push_back(normalTex);
	bTextured = true;
}
//reset textures
void Mesh::clearTextures()
{
	if (bTextured)
	{
		for (UINT i = 0; i < mTextures.size(); ++i)
		{
			SAFE_RELEASE(mTextures[i]);
			SAFE_RELEASE(mNormals[i]);
		}
		if (mTextures.size() > 0)
			vector<IDirect3DTexture9*>().swap(mTextures);
		if (mNormals.size() > 0)
			vector<IDirect3DTexture9*>().swap(mNormals);
		bTextured = false;
	}
}
//draw the mesh. This method will almost never change with descendant classes as they are all drawn
//using basically the same method
//draw using a particular texture index, if needed
void Mesh::draw(UINT _texIndex)
{
	//draw if presently visible
	if (bVisible && bTextured)
	{
		UINT index = _texIndex + mNumSubsets;
		if (index > mTextures.size() || index < 0)
		{
			OutputDebugString(L"ERROR: Draw is using a texture index outside of range.\n");
			PostQuitMessage(0);
		}
		//set technique
		HR(gEffectPrimary->getFX()->SetTechnique(gEffectPrimary->getTechniqueHandle()));
		//set eye position
		HR(gEffectPrimary->getFX()->SetFloatArray(gEffectPrimary->getEyePositionHandle(), gCameraMain->getPosition(), 3));
		//figure world matrix if one is found, or have error
		if (gEffectPrimary->getWorldHandle())
		{
			HR(gEffectPrimary->getFX()->SetMatrix(gEffectPrimary->getWorldHandle(), (D3DXMATRIX*)&mWorld));
		}
		else
		{
			OutputDebugString(L"Error: No world handle found in Mesh::draw");
		}
		//set up world view projection if one is found, or give error
		if (gEffectPrimary->getWorldViewProjectionHandle())
		{
			D3DXMATRIX WVP = mWorld * gCameraMain->getView() * gCameraMain->getProjection();
			HR(gEffectPrimary->getFX()->SetMatrix(gEffectPrimary->getWorldViewProjectionHandle(),
				&WVP));
		}
		else
		{
			OutputDebugString(L"Error: No worldViewProj handle found in Mesh::draw");
		}
		//start passes, one pass for our purposes
		UINT numPasses = 1;
		HR(gEffectPrimary->getFX()->Begin(&numPasses, 0));
		HR(gEffectPrimary->getFX()->BeginPass(0));
		//do this for each subset if multiple subsets
		for (UINT i = 0; i < mNumSubsets; ++i)
		{
			//set textures here as they might be different between subsets
			gEffectPrimary->setTextures(mTextures[i + _texIndex], mNormals[i + _texIndex]);
			//commit after all changes are done
			HR(gEffectPrimary->getFX()->CommitChanges());
			//draw subset
			mMesh->DrawSubset(i);
		}
		HR(gEffectPrimary->getFX()->EndPass());
		HR(gEffectPrimary->getFX()->End());
	}
}
//check if it collides with any of the AABB for this mesh
const bool Mesh::collideWith(const BoundingSphere2D& _sphere)
{
	for (AxisAlignedBoundingBox2D* AABB : mBoundsAABB)
	{
		if (collides(*AABB, _sphere))
			return true;
	}
	return false;
}

AnimMesh::AnimMesh(LPDIRECT3DDEVICE9 _d3dDevice) :
mpFrameRoot(NULL), mpBoneMatrices(NULL), mVecCenter(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
mfRadius(0.0f), mdwCurrentAnimation(-1), mdwAnimationSetCount(0), muMaxBones(0),
mpAnimController(NULL), mpFirstMesh(NULL), mpD3DDevice(_d3dDevice)
{
}

AnimMesh::~AnimMesh()
{
	//delete controller
	SAFE_RELEASE(mpAnimController);
	//if there is frame hierarchy...
	if (mpFrameRoot)
	{
		//allocation class
		AnimAllocateHierarchy* alloc = NULL;
		D3DXFrameDestroy(mpFrameRoot, alloc);
		mpFrameRoot = NULL;
	}
	//delete bones
	SAFE_DELETE_ARRAY(mpBoneMatrices);
	//clear device
	mpD3DDevice = NULL;
}

void AnimMesh::loadXFile(LPCWSTR _filename)
{
	//allocation class
	AnimAllocateHierarchy* alloc = NULL;
	//load mesh
	HR(D3DXLoadMeshHierarchyFromX(_filename, D3DXMESH_MANAGED, mpD3DDevice, alloc, NULL,
		&mpFrameRoot, &mpAnimController));
	if (mpAnimController)
		mdwAnimationSetCount = mpAnimController->GetMaxNumAnimationSets();
	if (mpFrameRoot)
	{
		//set up bones
		setupBoneMatrices((LPFRAME)mpFrameRoot, NULL);
		//set up bone matrices array
		mpBoneMatrices = new D3DXMATRIX[muMaxBones];
		ZeroMemory(mpBoneMatrices, sizeof(D3DXMATRIX) *muMaxBones);
		//calculate the bounding sphere
		D3DXFrameCalculateBoundingSphere(mpFrameRoot, &mVecCenter, &mfRadius);
	}
}

void AnimMesh::setupBoneMatrices(LPFRAME _frame, LPD3DXMATRIX _parentMatrix)
{
	LPMESHCONTAINER pMesh = (LPMESHCONTAINER)_frame->pMeshContainer;
	//setup bones on mesh
	if (pMesh)
	{
		if (!mpFirstMesh)
			mpFirstMesh = pMesh;
		//if there is skin mesh, then setup the bone matrices
		if (pMesh->pSkinInfo)
		{
			//create copy of mesh
			pMesh->MeshData.pMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMesh->MeshData.pMesh->GetFVF(),
				mpD3DDevice, &pMesh->pSkinMesh);
			if (muMaxBones < pMesh->pSkinInfo->GetNumBones())
			{
				//get number of bones
				muMaxBones = pMesh->pSkinInfo->GetNumBones();
			}
			LPFRAME pTempFrame = NULL;
			//for each bone
			for (UINT i = 0; i < pMesh->pSkinInfo->GetNumBones(); ++i)
			{
				//find frame
				pTempFrame = (LPFRAME)D3DXFrameFind(mpFrameRoot, pMesh->pSkinInfo->GetBoneName(i));
				//set the bone part
				pMesh->ppFrameMatrices[i] = &pTempFrame->matCombined;
			}
		}
	}
	//check sibling
	if (_frame->pFrameSibling)
		setupBoneMatrices((LPFRAME)_frame->pFrameSibling, _parentMatrix);
	//check child
	if (_frame->pFrameFirstChild)
		setupBoneMatrices((LPFRAME)_frame->pFrameFirstChild, &_frame->matCombined);
}

void AnimMesh::setCurrentAnimation(DWORD _animationFlag)
{
	//if animation is not one that we are already using, and passed in flag
	//is not bigger than the number of animations
	if (_animationFlag != mdwCurrentAnimation && _animationFlag < mdwAnimationSetCount)
	{
		mdwCurrentAnimation = _animationFlag;
		LPD3DXANIMATIONSET AnimSet = NULL;
		mpAnimController->GetAnimationSet(mdwCurrentAnimation, &AnimSet);
		mpAnimController->SetTrackAnimationSet(0, AnimSet);
		SAFE_RELEASE(AnimSet);
	}
}

void AnimMesh::draw()
{
	LPMESHCONTAINER pMesh = mpFirstMesh;
	//while there is a mesh, try to draw it
	while (pMesh)
	{
		//select mesh to draw
		LPD3DXMESH pDrawMesh = (pMesh->pSkinInfo) ? pMesh->pSkinMesh : pMesh->MeshData.pMesh;
		//draw each mesh subset with correct materials and texture
		for (DWORD i = 0; i < pMesh->NumMaterials; ++i)
		{
			mpD3DDevice->SetMaterial(&pMesh->pMaterials9[i]);
			mpD3DDevice->SetTexture(0, pMesh->ppTextures[i]);
			pDrawMesh->DrawSubset(i);
		}
		//go to next
		pMesh = (LPMESHCONTAINER)pMesh->pNextMeshContainer;
	}
}

void AnimMesh::drawFrame(LPFRAME _frame)
{
	LPMESHCONTAINER pMesh = (LPMESHCONTAINER)_frame->pMeshContainer;
	//try to draw a mesh while there are meshes to draw
	while (pMesh)
	{
		//select mesh
		LPD3DXMESH pDrawMesh = (pMesh->pSkinInfo)
			? pMesh->pSkinMesh : pMesh->MeshData.pMesh;

		//draw subsets with materials and textures
		for (DWORD i = 0; i < pMesh->NumMaterials; ++i)
		{
			mpD3DDevice->SetMaterial(&pMesh->pMaterials9[i]);
			mpD3DDevice->SetTexture(0, pMesh->ppTextures[i]);
			pDrawMesh->DrawSubset(i);
		}

		//go to next
		pMesh = (LPMESHCONTAINER)pMesh->pNextMeshContainer;
	}

	//Check your Sister
	if (_frame->pFrameSibling)
		drawFrame((LPFRAME)_frame->pFrameSibling);

	//Check your Son
	if (_frame->pFrameFirstChild)
		drawFrame((LPFRAME)_frame->pFrameFirstChild);
}

void AnimMesh::update(float _dt)
{
	//Set the time for animation
	if (mpAnimController && mdwCurrentAnimation != -1)
		//mpAnimController->SetTime(mpAnimController->GetTime() + _dt);
		mpAnimController->AdvanceTime(_dt, NULL);

	//Update the frame hierarchy
	if (mpFrameRoot)
	{
		updateFrameMatrices((LPFRAME)mpFrameRoot, NULL);

		LPMESHCONTAINER pMesh = mpFirstMesh;
		if (pMesh)
		{
			if (pMesh->pSkinInfo)
			{
				UINT Bones = pMesh->pSkinInfo->GetNumBones();
				for (UINT i = 0; i < Bones; ++i)
				{
					D3DXMatrixMultiply
						(
						&mpBoneMatrices[i],//out
						&pMesh->pBoneOffsets[i],
						pMesh->ppFrameMatrices[i]
						);
				}

				// Lock the meshes' vertex buffers
				void *SrcPtr, *DestPtr;
				pMesh->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&SrcPtr);
				pMesh->pSkinMesh->LockVertexBuffer(0, (void**)&DestPtr);

				// Update the skinned mesh using provided transformations
				pMesh->pSkinInfo->UpdateSkinnedMesh(mpBoneMatrices, NULL, SrcPtr, DestPtr);

				// Unlock the meshes vertex buffers
				pMesh->pSkinMesh->UnlockVertexBuffer();
				pMesh->MeshData.pMesh->UnlockVertexBuffer();
			}
		}
	}
}

void AnimMesh::updateFrameMatrices(LPFRAME _frame, LPD3DXMATRIX _parentMatrix)
{
	//Parent check
	if (_parentMatrix)
	{
		D3DXMatrixMultiply(&_frame->matCombined,
			&_frame->TransformationMatrix,
			_parentMatrix);
	}
	else
		_frame->matCombined = _frame->TransformationMatrix;

	//Do the kid too
	if (_frame->pFrameSibling)
	{
		updateFrameMatrices((LPFRAME)_frame->pFrameSibling, _parentMatrix);
	}

	//make sure you get the first kid
	if (_frame->pFrameFirstChild)
	{
		updateFrameMatrices((LPFRAME)_frame->pFrameFirstChild,
			&_frame->matCombined);
	}
}

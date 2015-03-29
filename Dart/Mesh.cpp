#include "DirectInput.h"

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

Mesh::Mesh(LPCWSTR _mesh, LPCWSTR _texture, LPCWSTR _normal, D3DXVECTOR3 _position,
	D3DXVECTOR3  _scale, D3DXVECTOR3 _rotation, UINT _subsets):
	Mesh(_mesh, _position, _scale, _rotation, _subsets)
{
	addTexture(_texture, _normal);
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
		HR(gD3DDevice->SetVertexDeclaration(VERTEX_STATIC::Decl));
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
/*const bool Mesh::collideWith(const BoundingSphere2D& _sphere)
{
	for (AxisAlignedBoundingBox2D* AABB : mBoundsAABB)
	{
		if (collides(*AABB, _sphere))
			return true;
	}
	return false;
}*/


AnimMesh::AnimMesh(LPCWSTR _mesh, LPCWSTR _texture, LPCWSTR _normal, D3DXVECTOR3 _position,
	D3DXVECTOR3  _scale, D3DXVECTOR3 _rotation, UINT _subsets):
	Mesh(L"", _position, _scale, _rotation, _subsets), mCurrentAnimation(-1)
{
	// Create frame hierarchy
	AllocMeshHierarchy allocMeshHierarchy;
	D3DXLoadMeshHierarchyFromX(_mesh, D3DXMESH_SYSTEMMEM, gD3DDevice, &allocMeshHierarchy,
		0, &mRoot, &mAnimController);


	// we assume that the input .x file contains only one mesh.  
	// So search for that one and only mesh.
	D3DXFRAME* f = findNodeWithMesh(mRoot);
	D3DXMESHCONTAINER* meshContainer = f->pMeshContainer;
	mSkinInfo = meshContainer->pSkinInfo;
	mSkinInfo->AddRef();
	//add texture that is passed into function
	addTexture(_texture, _normal);
	//bones and matrix transforms for them
	mNumBones = meshContainer->pSkinInfo->GetNumBones();
	mFinalXForms.resize(mNumBones);
	mToRootXFormPtrs.resize(mNumBones, 0);
	buildSkinnedMesh(meshContainer->MeshData.pMesh);
	buildToRootXFormPtrArray();
	//animations
	mAnimationSetCount = mAnimController->GetMaxNumAnimationSets();
	//setCurrentAnimation(0);//this doesn't seem to do anything yet
}

void AnimMesh::setCurrentAnimation(DWORD _dwAnimationFlag)
{
	if (_dwAnimationFlag != mCurrentAnimation)//&& _dwAnimationFlag < mAnimationSetCount)
	{
		mCurrentAnimation = _dwAnimationFlag;
		LPD3DXANIMATIONSET AnimSet = NULL;
		mAnimController->GetAnimationSet(mCurrentAnimation, &AnimSet);
		mAnimController->SetTrackAnimationSet(0, AnimSet);
		SAFE_RELEASE(AnimSet);
	}
}

//we assume 1 mesh per file and search for the node that represents it
D3DXFRAME* AnimMesh::findNodeWithMesh(D3DXFRAME* _frame)
{
	if (_frame->pMeshContainer)
		if (_frame->pMeshContainer->MeshData.pMesh != 0)
			return _frame;

	D3DXFRAME* f = 0;
	if (_frame->pFrameSibling)
		if (f = findNodeWithMesh(_frame->pFrameSibling))
			return f;

	if (_frame->pFrameFirstChild)
		if (f = findNodeWithMesh(_frame->pFrameFirstChild))
			return f;

	return 0;
}

void AnimMesh::buildSkinnedMesh(ID3DXMesh* mesh)
{
	// First add a normal component and 2D texture coordinate component
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VERTEX_ANIM::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* tempMesh = 0;
	mesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gD3DDevice, &tempMesh);

	DWORD* adj = new DWORD[tempMesh->GetNumFaces() * 3];
	ID3DXBuffer* remap = 0;
	tempMesh->GenerateAdjacency(0.001f, adj);
	ID3DXMesh* optimizedTempMesh = 0;
	tempMesh->Optimize(D3DXMESH_SYSTEMMEM | D3DXMESHOPT_VERTEXCACHE | D3DXMESHOPT_ATTRSORT,
		adj, 0, 0, &remap, &optimizedTempMesh);
	SAFE_RELEASE(tempMesh);
	delete[] adj;

	mSkinInfo->Remap(optimizedTempMesh->GetNumVertices(), (DWORD*)remap->GetBufferPointer());
	SAFE_RELEASE(remap);

	DWORD numBoneComboEntries = 0;
	ID3DXBuffer* boneComboTable = 0;

	HRESULT hr = mSkinInfo->ConvertToIndexedBlendedMesh(optimizedTempMesh, D3DXMESH_MANAGED |
		D3DXMESH_WRITEONLY, MAX_NUM_BONES_SUPPORTED, 0, 0, 0, 0, &mMaxVertInfluences,
		&numBoneComboEntries, &boneComboTable, &mMesh);

	SAFE_RELEASE(optimizedTempMesh);
	SAFE_RELEASE(boneComboTable);
}

void AnimMesh::buildToRootXFormPtrArray()
{
	for (UINT i = 0; i < mNumBones; ++i)
	{
		// Find the frame that corresponds with the ith bone offset matrix
		const char* boneName = mSkinInfo->GetBoneName(i);
		D3DXFRAME* frame = D3DXFrameFind(mRoot, boneName);

		if (frame)
		{
			FrameEx* frameEx = static_cast<FrameEx*>(frame);
			mToRootXFormPtrs[i] = &frameEx->toRoot;
		}
	}
}

void AnimMesh::update(float _dt)
{

	// Animate the mesh.  The animation controller has pointer to
	// the hierarchy frame transform matrices.  The animation controller
	// updates these matrices to reflect the given pose at the current
	// time by interpolating between animation frames.
	if (mCurrentAnimation != -1)
		mAnimController->AdvanceTime(_dt, 0);

	// Recurse down the tree and generate a frame's toRoot xform from the updated pose
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	buildToRootXForms((FrameEx*)mRoot, identity);

	// Build the final transforms for each bone
	D3DXMATRIX offsetTemp, toRootTemp;
	for (UINT i = 0; i < mNumBones; ++i)
	{
		offsetTemp = *mSkinInfo->GetBoneOffsetMatrix(i);
		toRootTemp = *mToRootXFormPtrs[i];
		mFinalXForms[i] = offsetTemp * toRootTemp;
	}
}

void AnimMesh::buildToRootXForms(FrameEx* frame, D3DXMATRIX& parentsToRoot)
{
	// Save some references to economize line space.
	D3DXMATRIX& toParent = frame->TransformationMatrix;
	D3DXMATRIX& toRoot = frame->toRoot;

	toRoot = toParent * parentsToRoot;

	FrameEx* sibling = (FrameEx*)frame->pFrameSibling;
	FrameEx* firstChild = (FrameEx*)frame->pFrameFirstChild;

	// Recurse down siblings
	if (sibling)
		buildToRootXForms(sibling, parentsToRoot);

	// Recurse to first child
	if (firstChild)
		buildToRootXForms(firstChild, toRoot);
}

void AnimMesh::draw()
{
	//draw if presently visible
	if (bVisible && bTextured)
	{
		HR(gD3DDevice->SetVertexDeclaration(VERTEX_ANIM::Decl));

		if (mNumSubsets > mTextures.size() || mNumSubsets < 1)
		{
			OutputDebugString(L"ERROR: Draw is using a texture index outside of range.\n");
			PostQuitMessage(0);
		}
		//set technique
		HR(gEffectAnim->getFX()->SetTechnique(gEffectAnim->getTechniqueHandle()));
		//set eye position
		HR(gEffectAnim->getFX()->SetFloatArray(gEffectAnim->getEyePositionHandle(), gCameraMain->getPosition(), 3));
		//figure world matrix if one is found, or have error
		if (gEffectAnim->getWorldHandle())
		{
			HR(gEffectAnim->getFX()->SetMatrix(gEffectAnim->getWorldHandle(), (D3DXMATRIX*)&mWorld));
		}
		else
		{
			OutputDebugString(L"Error: No world handle found in Mesh::draw");
		}
		//set up world inverse transpose
		if (gEffectAnim->getWorldInverseTransHandle())
		{
			D3DXMATRIX M = mWorld;
			D3DXMatrixInverse(&M, NULL, &M);
			D3DXMatrixTranspose(&M, &M);
			HR(gEffectAnim->getFX()->SetMatrix(gEffectAnim->getWorldInverseTransHandle(), &M));
		}
		//set up world view projection if one is found, or give error
		if (gEffectAnim->getWorldViewProjectionHandle())
		{
			D3DXMATRIX WVP = mWorld * gCameraMain->getView() * gCameraMain->getProjection();
			HR(gEffectAnim->getFX()->SetMatrix(gEffectAnim->getWorldViewProjectionHandle(),
				&WVP));
		}
		else
		{
			OutputDebugString(L"Error: No worldViewProj handle found in Mesh::draw");
		}
		//pass in bone info
		gEffectAnim->getFX()->SetMatrixArray("finalXForms", getFinalXFormArray(), mNumBones);
		//start passes, one pass for our purposes
		UINT numPasses = 1;
		HR(gEffectAnim->getFX()->Begin(&numPasses, 0));
		HR(gEffectAnim->getFX()->BeginPass(0));
		//do this for each subset if multiple subsets
		for (UINT i = 0; i < mNumSubsets; ++i)
		{
			//set textures here as they might be different between subsets
			gEffectAnim->setTextures(mTextures[i], mNormals[i]);
			//commit after all changes are done
			HR(gEffectAnim->getFX()->CommitChanges());
			//draw subset
			mMesh->DrawSubset(i);
		}
		HR(gEffectAnim->getFX()->EndPass());
		HR(gEffectAnim->getFX()->End());
	}
}
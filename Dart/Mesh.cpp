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
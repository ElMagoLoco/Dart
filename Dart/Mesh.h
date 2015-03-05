//meshes are what many objects descend from, including player, enemies, and projectiles
#pragma once

#include <vector>
#include "Camera.h"
#include "Bounds.h"
#include "D3DUtils.h"
#include "DirectInput.h"
#include "Effect.h"
#include "Light.h"
#include "Vertex.h"

using std::vector;
//basic mesh has things that are common to all descendants
class Mesh
{
public:
	//constructor s
	Mesh(LPCWSTR _mesh, D3DXVECTOR3 _position, D3DXVECTOR3  _scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		D3DXVECTOR3 _rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f), UINT _subsets = 1);
	Mesh(){}
	//destructor
	~Mesh();
	//get/set
	//get functions
	D3DXVECTOR3 getPosition()	{ return mPosition; }
	D3DXVECTOR3 getScale()		{ return mScale; }
	D3DXVECTOR3 getRotation()	{ return mRotation; }
	D3DXMATRIX	getWorld()		{ return mWorld; }
	ID3DXMesh*  getMesh()		{ return mMesh; }
	//set functions
	void setPosition(D3DXVECTOR3 _position)		{ mPosition = _position; doWorld(); }
	void setRotation(D3DXVECTOR3 _rotation)		{ mRotation = _rotation; doWorld(); }
	void setPosRot(D3DXVECTOR3 _position, D3DXVECTOR3 _rotation)
		{ mPosition = _position; mRotation = _rotation; doWorld(); }
	//set the world matrix for the mesh
	void setWorldMatrix(D3DXMATRIX _world)	{ mWorld = _world; }
	//add a texture to the mesh
	void addTexture(LPCWSTR _texture, LPCWSTR _normal);
	void clearTextures();//reset textures
	void draw(UINT _texIndex = 0);//draw the mesh; this changes very little if any for descendant classes
	void loadMesh(LPCWSTR _mesh);//load a mesh to be displayed
	void doWorld();//figure world matrices
	//can turn off visibility
	void setVisibility(bool _bVisible) { bVisible = _bVisible; }
	//adding AABB to the mesh for collisions
	void addAABB(AxisAlignedBoundingBox2D* AABB) { mBoundsAABB.push_back(AABB); }
	//get collision boxes
	vector<AxisAlignedBoundingBox2D*> getAABBs(){ return mBoundsAABB; }
	//collisions with bounding spheres
	const bool collideWith(const BoundingSphere2D& _sphere);
protected:
	ID3DXMesh*					mMesh;//mesh that we are showing
	D3DXVECTOR3					mPosition;//position
	D3DXVECTOR3					mScale;//how big compared to base scale of mesh
	D3DXVECTOR3					mRotation;//rotation of mesh
	D3DXMATRIX					mWorld;//world matrix
	//number of subsets in this mesh
	UINT						mNumSubsets;
	//textures and normal textures for each subset
	//should be placed in the vector in the same order as the subsets
	vector<IDirect3DTexture9*>	mTextures;
	vector<IDirect3DTexture9*>	mNormals;
	//is presently visible
	bool						bVisible;
	//has textures
	bool						bTextured;
	//collision boxes
	vector<AxisAlignedBoundingBox2D*> mBoundsAABB;
};


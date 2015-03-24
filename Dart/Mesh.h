//meshes are what many objects descend from, including player, enemies, and projectiles
//http://www.gamedev.net/page/resources/_/technical/game-programming/working-with-the-directx-x-file-format-and-animation-in-directx-90-r2079
//used as a tutorial when making animated mesh structures
#pragma once

#include <vector>
#include "Animation.h"
#include "Bounds.h"
#include "Camera.h"
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
	Mesh(LPCWSTR _mesh, LPCWSTR _texture, LPCWSTR _normal, D3DXVECTOR3 _position, 
		D3DXVECTOR3  _scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
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
	{
		mPosition = _position; mRotation = _rotation; doWorld();
	}
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
	//void addAABB(AxisAlignedBoundingBox2D* AABB) { mBoundsAABB.push_back(AABB); }
	// SAM
	void addBoundsBox(const AxisAlignedBoundingBox& box) { mBoundsBoxList.push_back(box); }
	//get collision boxes
	//vector<AxisAlignedBoundingBox2D*> getAABBs(){ return mBoundsAABB; }
	// SAM
	vector<AxisAlignedBoundingBox>& getBoundsBoxList() { return mBoundsBoxList; }
	//collisions with bounding spheres
	/*const bool collideWith(const BoundingSphere2D& _sphere);*/
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
	//vector<AxisAlignedBoundingBox2D*> mBoundsAABB;
	// SAM
	vector<AxisAlignedBoundingBox>		mBoundsBoxList;
};

class AnimMesh: public Mesh
{
public:
	AnimMesh(LPCWSTR _mesh, LPCWSTR _texture, LPCWSTR _normal, D3DXVECTOR3 _position,
		D3DXVECTOR3  _scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		D3DXVECTOR3 _rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f), UINT _subsets = 1);
	void update(float _dt);
	void draw();
	void setCurrentAnimation(DWORD _dwAnimationFlag);
	DWORD getAnimationSetCount() { return mAnimationSetCount; }
private:
	D3DXFRAME* findNodeWithMesh(D3DXFRAME* _frame);
	void buildSkinnedMesh(ID3DXMesh* mesh);
	void buildToRootXFormPtrArray();
	void buildToRootXForms(FrameEx* _frame, D3DXMATRIX& _parentsToRoot);
	const D3DXMATRIX* getFinalXFormArray() { return &mFinalXForms[0]; }
	D3DXFRAME* mRoot;
	ID3DXAnimationController* mAnimController;
	ID3DXSkinInfo* mSkinInfo;
	DWORD mNumBones;
	vector<D3DXMATRIX> mFinalXForms;
	vector<D3DXMATRIX*> mToRootXFormPtrs;
	DWORD mMaxVertInfluences;
	DWORD mCurrentAnimation;
	DWORD mAnimationSetCount;
};
/*
//animmesh is a mesh with animations attached
//set animation.h for credit info
class AnimMesh
{
public:
	AnimMesh(LPDIRECT3DDEVICE9 _d3dDevice, D3DXVECTOR3 _position, D3DXVECTOR3  scale,
		D3DXVECTOR3 _rotation);
	~AnimMesh();
	AnimMesh &operator=(const AnimMesh&){}
	AnimMesh(const AnimMesh&){}
	//return center and radius of bounding sphere
	inline LPD3DXVECTOR3 getBoundingSphereCenter(){ return &mVecCenter; }
	inline float getBoundingSphereRadius(){ return mfRadius; }
	//return animation being used
	inline DWORD getCurrentAnimation(){ return mdwCurrentAnimation; }
	//set current animation
	void setCurrentAnimation(DWORD dwAnimationFlag);
	void draw(UINT _texIndex);
	void loadXFile(LPCWSTR _fileName);
	void update(float _dt);

	//add a texture to the mesh
	void addTexture(LPCWSTR _texture, LPCWSTR _normal);
	void clearTextures();//reset textures
	//set world matrix based on positions
	void doWorld();

private:
	vector<IDirect3DTexture9*>	mTextures;
	vector<IDirect3DTexture9*>	mNormals;
	bool bTextured;//is it textured
	bool bVisible;//is it visible
	UINT mNumSubsets;//how many subsets
	//position and world matrix
	D3DXVECTOR3					mPosition;//position
	D3DXVECTOR3					mScale;//how big compared to base scale of mesh
	D3DXVECTOR3					mRotation;//rotation of mesh
	D3DXMATRIX					mWorld;//world matrix

	LPDIRECT3DDEVICE9 mpD3DDevice;//d3d device to use
	//model
	LPMESHCONTAINER mpFirstMesh;//first mesh in hierarchy
	LPD3DXFRAME mpFrameRoot;//frame hierarchy
	LPD3DXMATRIX mpBoneMatrices;//used when calculating bone positions
	D3DXVECTOR3 mVecCenter;//center of bounding sphere of object
	float mfRadius;//radius of bounding sphere of object
	UINT muMaxBones;//max number of bones for model
	//animation
	DWORD mdwCurrentAnimation;//current animation
	DWORD mdwAnimationSetCount;//number of animation sets
	LPD3DXANIMATIONCONTROLLER mpAnimController;//controller for animations


	//void drawMesh(LPMESHCONTAINER _meshContainer, LPD3DXMATRIX _matrix);
	//go through each frame and draw non-null frames
	void drawFrame(LPFRAME _frame);
	//set up bone matrices
	void setupBoneMatrices(LPFRAME _frame, LPD3DXMATRIX _parentMatrix);
	//update frame matrices
	void updateFrameMatrices(LPFRAME _frame, LPD3DXMATRIX _parentMatrix);
	//void setupBoneMatricesOnMesh(LPMESHCONTAINER _meshContainer);
};

*/
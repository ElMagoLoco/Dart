//=============================================================================
// Vertex.h
//=============================================================================
#pragma once

#include <d3dx9.h>
#include "D3DUtils.h"

// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();


//===============================================================
struct VertexPos
{
	VertexPos()
		: mPosition(0.0f, 0.0f, 0.0f)
	{}
	VertexPos(float _x, float _y, float _z)
		: mPosition(_x, _y, _z)
	{}
	VertexPos(const D3DXVECTOR3& _v)
		: mPosition(_v)
	{}

	D3DXVECTOR3 mPosition;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexCol
{
	VertexCol()
		: mPosition(0.0f, 0.0f, 0.0f), mColor(0x00000000)
	{}
	VertexCol(float _x, float _y, float _z, D3DCOLOR _c)
		: mPosition(_x, _y, _z), mColor(_c)
	{}
	VertexCol(const D3DXVECTOR3& _v, D3DCOLOR _c)
		: mPosition(_v), mColor(_c)
	{}

	D3DXVECTOR3 mPosition;
	D3DCOLOR    mColor;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPN
{
	VertexPN()
		: mPosition(0.0f, 0.0f, 0.0f), mNormal(0.0f, 0.0f, 0.0f)
	{}
	VertexPN(float _x, float _y, float _z, float _nx, float _ny, float _nz)
		: mPosition(_x, _y, _z), mNormal(_nx, _ny, _nz)
	{}
	VertexPN(const D3DXVECTOR3& _v, const D3DXVECTOR3& _n)
		: mPosition(_v), mNormal(_n)
	{}

	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mNormal;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPNT
{
	VertexPNT()
		: mPosition(0.0f, 0.0f, 0.0f), mNormal(0.0f, 0.0f, 0.0f), mTex(0.0f, 0.0f)
	{}
	VertexPNT(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v)
		: mPosition(_x, _y, _z), mNormal(_nx, _ny, _nz), mTex(_u, _v)
	{}
	VertexPNT(const D3DXVECTOR3& _v, const D3DXVECTOR3& _n, const D3DXVECTOR2& _uv)
		: mPosition(_v), mNormal(_n), mTex(_uv)
	{}

	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mNormal;
	D3DXVECTOR2 mTex;

	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPNTaT
{
	VertexPNTaT()
		: mPosition(0.0f, 0.0f, 0.0f), mNormal(0.0f, 0.0f, 0.0f), mTangent(0.0f, 0.0f, 0.0f),
		mTex(0.0f, 0.0f)
	{}
	VertexPNTaT(float _x, float _y, float _z, float _nx, float _ny, float _nz,
		float _tx, float _ty, float _tz, float _u, float _v)
		: mPosition(_x, _y, _z), mNormal(_nx, _ny, _nz), mTangent(_tx, _ty, _tz), mTex(_u, _v)
	{}
	VertexPNTaT(const D3DXVECTOR3& _v, const D3DXVECTOR3& _n,
		const D3DXVECTOR3& _ta, const D3DXVECTOR2& _uv)
		: mPosition(_v), mNormal(_n), mTangent(_ta), mTex(_uv)
	{}

	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mNormal;
	D3DXVECTOR3 mTangent;
	D3DXVECTOR2 mTex;

	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
//particles for attacks or magic
struct Particle
{
	D3DXVECTOR3 mInitialPos;
	D3DXVECTOR3 mInitialVelocity;
	float mInitialSize;//in pixels
	float mInitialTime;
	float mLifeTime;
	float mMass;
	D3DCOLOR mInitialColor;
	static IDirect3DVertexDeclaration9* Decl;
};
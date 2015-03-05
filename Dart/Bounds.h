//bounding shapes and collision functions between them
//some simple collision structures and functions for finding collisions
#pragma once
#include <vector>
using std::vector;
#include "D3DUtils.h"

//parent for all bounding shapes
struct BoundingShape
{
};

//axis aligned bounding box
struct AxisAlignedBoundingBox
{
	//constructor, just sets the three variables
	AxisAlignedBoundingBox(D3DXVECTOR3 _min, D3DXVECTOR3 _max, bool _bBlocksProjectiles = true) :
		mMin(_min), mMax(_max), bBlocksProjectiles(_bBlocksProjectiles)
	{
		if (_min.x > _max.x || _min.y > _max.y || _min.z > _max.z)
			OutputDebugString(L"ERROR: AABB min point should be less than max point or collision detection will be inaccurate.");
	}
	AxisAlignedBoundingBox() : mMin(D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX)),
		mMax(D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX)), bBlocksProjectiles(true) {};
	D3DXVECTOR3	mMin;//min point
	D3DXVECTOR3	mMax;//max point
	bool		bBlocksProjectiles;//does it block projectiles?
};
//bounding sphere
struct BoundingSphere
{
	//constructor, just sets the three variables
	BoundingSphere(D3DXVECTOR3 _center, float _radius, bool _bBlocksProjectiles = true)
	{
		mCenter = _center; mRadius = _radius; bBlocksProjectiles = _bBlocksProjectiles;
	}

	D3DXVECTOR3	mCenter;//center of sphere
	float		mRadius;//sphere radius
	bool		bBlocksProjectiles;//does it block projectiles?
};

struct LineSegment
{
	LineSegment(D3DXVECTOR3 _begin, D3DXVECTOR3 _end)
	{
		mBegin = _begin; mEnd = _end;
	}

	D3DXVECTOR3 mBegin;
	D3DXVECTOR3 mEnd;
};
//I ended up making a 2d version of these to save processing power since the map is flat
//and we only really need to check in the x and z dimensions
struct AxisAlignedBoundingBox2D
{
	//constructor
	AxisAlignedBoundingBox2D(D3DXVECTOR2 _min, D3DXVECTOR2 _max, bool _blocksProjectiles = true) :
		mMin(_min), mMax(_max)
	{
		if (_max.x < _min.x || _max.y < _min.y)
		{
			OutputDebugString(L"ERROR: Box max point should be > box min point or collision detection will fail.");
		}
	}
	D3DXVECTOR2 mMin;//minimum point of box
	D3DXVECTOR2 mMax;//size of box starting from min point
	bool		bBlocksProjectiles;//does it block projectiles?
};
struct AxisAlignedBoundingSquare2D
{
	AxisAlignedBoundingSquare2D(D3DXVECTOR2 _min, float _size) :
		mMin(_min), mSize(_size) {}
	D3DXVECTOR2 mMin;
	float mSize;
};

struct BoundingSphere2D
{
	BoundingSphere2D(D3DXVECTOR2 _center, float _radius) :
		mCenter(_center), mRadius(_radius) {}
	D3DXVECTOR2 mCenter;
	float mRadius;
};

struct LineSegment2D
{
	LineSegment2D(D3DXVECTOR2 _start, D3DXVECTOR2 _end) :
		mStart(_start), mEnd(_end) {}
	D3DXVECTOR2 mStart, mEnd;//start and end points
};

const bool collides(const AxisAlignedBoundingBox2D _box, const BoundingSphere2D _circle);
const bool collides(const BoundingSphere2D _circle1, const BoundingSphere2D _circle2);
const bool collides(const AxisAlignedBoundingBox2D _box, const LineSegment2D _line);
const bool collides(const LineSegment2D _line1, const LineSegment2D _line2);
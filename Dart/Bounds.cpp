#include "Bounds.h"

//will put collision functions here as they are needed


//perpendicular dot product for assistance in line collision
float perpDot(D3DXVECTOR2 _a, D3DXVECTOR2 _b)
{
	return ((_a.x * _b.y) - (_a.y * _b.x));
}

//collision between 2d box and circle, does not return penetration depth or other info
const bool collides(const AxisAlignedBoundingBox2D _box, const BoundingSphere2D _circle)
{
	float distance = D3DXVec2LengthSq(&(_circle.mCenter - _box.mMin));
	if (distance < 62500.0f)
	{
		//if we extend the box edges by the circle radius, we can then check and see if the
		//circle's center point falls within that box to know if they collide
		D3DXVECTOR2 minBox(_box.mMin.x - _circle.mRadius, _box.mMin.y - _circle.mRadius);
		D3DXVECTOR2 maxBox(_box.mMax.x + _circle.mRadius, _box.mMax.y + _circle.mRadius);
		return (_circle.mCenter.x < maxBox.x && _circle.mCenter.x > minBox.x &&
			_circle.mCenter.y < maxBox.y && _circle.mCenter.y > minBox.y);
	}
	return false;
}

const bool collides(const BoundingSphere2D _circle1, const BoundingSphere2D _circle2)
{
	float distance = D3DXVec2LengthSq(&(_circle1.mCenter - _circle2.mCenter));
	if (distance < 62500.0f)
	{
		float totalRadius = _circle1.mRadius + _circle2.mRadius;
		return (distance < (totalRadius * totalRadius));
	}
	return false;
}

const bool collides(const AxisAlignedBoundingBox2D _box, const LineSegment2D _line)
{
	//to find if it collides with the box, check the line segment against each side
	//of the box as if the box were four line segments
	//D3DXVECTOR2 boxPoint1 = _box.mMin;
	D3DXVECTOR2 boxPoint2(_box.mMin.x, _box.mMax.y);
	//D3DXVECTOR2 boxPoint3 = _box.mMax;
	D3DXVECTOR2 boxPoint4(_box.mMax.x, _box.mMin.y);
	LineSegment2D line1(_box.mMin, boxPoint2);
	LineSegment2D line2(boxPoint2, _box.mMax);
	LineSegment2D line3(_box.mMax, boxPoint4);
	LineSegment2D line4(boxPoint4, _box.mMin);
	return
		(collides(_line, line1) || collides(_line, line2) ||
		collides(_line, line3) || collides(_line, line4));
}

const bool collides(const LineSegment2D _line1, const LineSegment2D _line2)
{
	D3DXVECTOR2 u = _line1.mEnd - _line1.mStart;//first line length
	D3DXVECTOR2 v = _line2.mEnd - _line2.mStart;//second line length
	float d = perpDot(u, v);//perpendicular dot product
	//are they parallel?
	if (fabs(d) < 0.0000001f)
		return false;
	D3DXVECTOR2 w = _line1.mStart - _line2.mStart;//difference between line starts
	float s = perpDot(v, w) / d;
	if (s < 0 || s > 1)
		return false;
	float t = perpDot(u, w) / d;
	if (t < 0 || t > 1)
		return false;
	//if it makes it through the above checks, then they collide
	return true;
}

// SAM
const bool collides(const AxisAlignedBoundingBox& box, const BoundingSphere& circle)
{
	// TODO: Implement
	return false;
}
// SAM
const bool collides(const AxisAlignedBoundingBox& box, const LineSegment& line)
{
	// TODO: Implement
	return false;
}

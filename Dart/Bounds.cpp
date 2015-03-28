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

const bool collides(const AxisAlignedBoundingBox& _box, const BoundingSphere& _sphere)
{
	//find closest point on box to sphere
	D3DXVECTOR3 closest = _sphere.mCenter;
	if (_sphere.mCenter.x < _box.mMin.x) closest.x = _box.mMin.x;
	else if (_sphere.mCenter.x > _box.mMax.x) closest.x = _box.mMax.x;
	if (_sphere.mCenter.y < _box.mMin.y) closest.y = _box.mMin.y;
	else if (_sphere.mCenter.y > _box.mMax.y) closest.y = _box.mMax.y;
	if (_sphere.mCenter.z < _box.mMin.z) closest.z = _box.mMin.z;
	else if (_sphere.mCenter.z > _box.mMax.z) closest.z = _box.mMax.z;
	//distance from sphere center to closest point squared
	float distSq = D3DXVec3LengthSq(&(closest - _sphere.mCenter));
	//radius squared
	float radiusSq = _sphere.mRadius * _sphere.mRadius;
	return (distSq < radiusSq);//if there is a collision
	/*
	//variables for square root and difference
	float sqRt = 0.0f;
	float dist = 0.0f;//initialize distance
	//if the sphere center x is less than the box position x
	if (_sphere.mCenter.x < _box.mMin.x)
		//then set sqrt to difference between x points
		sqRt = _sphere.mCenter.x - _box.mMin.x;
	//else if the center x is greater
	else if (_sphere.mCenter.x > (_box.mMax.x))
		//then set sqrt between center.x and the box side opposite the x position
		sqRt = _sphere.mCenter.x - (_box.mMax.x);
	//figure square, the distance
	dist += sqRt * sqRt;
	//we then do the above for the y and z dimensions and add their squared
	//distance to the total distance
	if (_sphere.mCenter.y < _box.mMin.y)
		sqRt = _sphere.mCenter.y - _box.mMin.y;
	else if (_sphere.mCenter.y > _box.mMax.y)
		sqRt = _sphere.mCenter.y - _box.mMax.y;
	dist += sqRt * sqRt;

	if (_sphere.mCenter.z < _box.mMin.z)
		sqRt = _sphere.mCenter.z - _box.mMin.z;
	else if (_sphere.mCenter.z > _box.mMax.z)
		sqRt = _sphere.mCenter.z - _box.mMax.z;
	dist += sqRt * sqRt;
	//if total distance is <= square of the radius, then we have
	//a collision
	return dist <= (_sphere.mRadius * _sphere.mRadius);
	*/
}

const bool collides(const AxisAlignedBoundingBox& _box, const BoundingSphere& _sphere,
	D3DXVECTOR3 sphereMovement)
{
	//find closest point on box to sphere
	D3DXVECTOR3 closest = _sphere.mCenter;
	if		(_sphere.mCenter.x < _box.mMin.x) closest.x = _box.mMin.x;
	else if (_sphere.mCenter.x > _box.mMax.x) closest.x = _box.mMax.x;
	if (_sphere.mCenter.y < _box.mMin.y) closest.y = _box.mMin.y;
	else if (_sphere.mCenter.y > _box.mMax.y) closest.y = _box.mMax.y;
	if (_sphere.mCenter.z < _box.mMin.z) closest.z = _box.mMin.z;
	else if (_sphere.mCenter.z > _box.mMax.z) closest.z = _box.mMax.z;
	//distance from sphere center to closest point squared
	float distSq = D3DXVec3LengthSq(&(closest - _sphere.mCenter));
	//radius squared
	float radiusSq = _sphere.mRadius * _sphere.mRadius;
	if (distSq < radiusSq)//if there is a collision
	{
		//penetration depth
		float deep = _sphere.mRadius - sqrt(distSq);
		//normal (direction to move sphere)
		D3DXVECTOR3 normal = closest - _sphere.mCenter;
		D3DXVec3Normalize(&normal, &normal);
		sphereMovement = normal * deep;
		return true;
	}
	else
		return false;
}

const bool collides(const AxisAlignedBoundingBox& box, const LineSegment& line)
{
	//get line midpoint,extent
	D3DXVECTOR3 lineMid = (line.mBegin + line.mEnd) * 0.5f;
	D3DXVECTOR3 lineHalf = (line.mBegin - lineMid);
	D3DXVECTOR3 lineExtent = D3DXVECTOR3(fabs(lineHalf.x), fabs(lineHalf.y), fabs(lineHalf.z));
	//get box extent
	D3DXVECTOR3 boxExtent = (box.mMax - box.mMin) * 0.5f;
	//separating axis test
	//separation vector from box center to line center is lineMid
	if (fabs(lineMid.x) > boxExtent.x + lineExtent.x) return false;
	if (fabs(lineMid.y) > boxExtent.y + lineExtent.y) return false;
	if (fabs(lineMid.z) > boxExtent.z + lineExtent.z) return false;
	//crossproducts of lines and each axis
	if (fabs(lineMid.y * lineHalf.z - lineMid.z * lineHalf.y) > (boxExtent.y * lineExtent.z + boxExtent.z * lineExtent.y)) return false;
	if (fabs(lineMid.x * lineHalf.z - lineMid.z * lineHalf.x) > (boxExtent.x * lineExtent.z + boxExtent.z * lineExtent.x)) return false;
	if (fabs(lineMid.x * lineHalf.y - lineMid.y * lineHalf.x) > (boxExtent.x * lineExtent.y + boxExtent.y * lineExtent.x)) return false;
	//no separating axis, it intersects
	return true;
}

const bool collides(const BoundingSphere& one, const BoundingSphere& two)
{
	float totalRadius = one.mRadius + two.mRadius;
	float distanceSq = D3DXVec3LengthSq(&(one.mCenter - two.mCenter));
	return (distanceSq < totalRadius * totalRadius);
}

const bool collides(const BoundingSphere& one, const BoundingSphere& two,
	D3DXVECTOR3 oneMovement)
{
	float totalRadius = one.mRadius + two.mRadius;
	float distanceSq = D3DXVec3LengthSq(&(one.mCenter - two.mCenter));
	if (distanceSq < totalRadius * totalRadius)
	{
		//penetration depth
		float deep = totalRadius - sqrt(distanceSq);
		//normal
		D3DXVECTOR3 normal = one.mCenter - two.mCenter;
		D3DXVec3Normalize(&normal, &normal);
		oneMovement = normal * deep;
		return true;
	}
	else
		return false;
}
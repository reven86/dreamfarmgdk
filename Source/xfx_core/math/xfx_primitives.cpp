//  File xfx_primitives.cpp                                                   |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_primitives.h"

_XFX_BEGIN




//
// Ray
//

void Ray::Transform (const Mat4& m)
{
	mOrigin		= m.TransformCoord (mOrigin);
	mDirection	= m.TransformNormal (mDirection);
}



//
// Sphere
//

void Primitives::Sphere::Projection (float& a, float& b, const Vec3& axis) const
{
	float dot = Vec3::Dot (Position (), axis);

	a = dot - Radius ();
	b = dot + Radius ();
}



//
// Triangle
//

void Primitives::Triangle::Transform (const Mat4& m)
{
	for (int i = 0; i < 3; i++)
		vertices[i] = m.TransformCoord( vertices[i] );
}

void Primitives::Triangle::Projection (float& a, float& b, const Vec3& axis) const
{
	a = b = Vec3::Dot( vertices[0], axis );

	for (int i = 1; i < 3; i++)
	{
		float dot = Vec3::Dot( vertices[i], axis );

		if (dot < a) a = dot;
		else if (dot > b) b = dot;
	}
}



//
// AABB
//

void Primitives::AABB::From (const Vec3& v0, const Vec3& v1)
{
	for (int j = 0; j < 3; j++)
	{
		mMin[j] = std::min (v0[j], v1[j]);
		mMax[j] = std::max (v0[j], v1[j]);
	}
}

void Primitives::AABB::From (const Primitives::Triangle& tri)
{
	mMin = tri.vertices[0];
	mMax = tri.vertices[0];

	for (int i = 1; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (tri.vertices[i][j] < mMin[j]) mMin[j] = tri.vertices[i][j];
			else if (tri.vertices[i][j] > mMax[j]) mMax[j] = tri.vertices[i][j];
		}
}

Vec3 Primitives::AABB::RandomPoint () const
{
	return Vec3 (
		mMin.x + (mMax - mMin).x * static_cast<float> (rand ()) / RAND_MAX,
		mMin.y + (mMax - mMin).y * static_cast<float> (rand ()) / RAND_MAX,
		mMin.z + (mMax - mMin).z * static_cast<float> (rand ()) / RAND_MAX
		);
}

void Primitives::AABB::Enlarge (const Primitives::Triangle& tri)
{
	for (int i = 0; i < 3; i++)
		Enlarge (tri.vertices[i]);
}

void Primitives::AABB::Enlarge (const Vec3& point)
{
	for (int j = 0; j < 3; j++)
	{
		if (point[j] < mMin[j]) mMin[j] = point[j];
		/*else */if (point[j] > mMax[j]) mMax[j] = point[j];
	}
}

void Primitives::AABB::Projection (float& a, float& b, const Vec3& axis) const
{
	Vec3 verts[] = {
		Vec3 (mMin.x, mMin.y, mMin.z),
		Vec3 (mMin.x, mMin.y, mMax.z),
		Vec3 (mMin.x, mMax.y, mMin.z),
		Vec3 (mMin.x, mMax.y, mMax.z),
		Vec3 (mMax.x, mMin.y, mMin.z),
		Vec3 (mMax.x, mMin.y, mMax.z),
		Vec3 (mMax.x, mMax.y, mMin.z),
		Vec3 (mMax.x, mMax.y, mMax.z)};

	a = b = Vec3::Dot (verts[0], axis);
	
	for (int i = 1; i < 8; i++)
	{
		float dot = Vec3::Dot (verts[i], axis);

		if (dot < a) a = dot;
		else if (dot > b) b = dot;
	}
}



//
// OBB
//

void Primitives::OBB::From (const Vec3& v0, const Vec3& v1)
{
	mLocation	= v0;
	mE1			= Vec3 (v1.x - v0.x, 0.0f, 0.0f);
	mE2			= Vec3 (0.0f, v1.y - v0.y, 0.0f);
	mE3			= Vec3 (0.0f, 0.0f, v1.z - v0.z);
}

void Primitives::OBB::From (const Primitives::Triangle& tri)
{
	Vec3 min = tri.vertices[0];
	Vec3 max = tri.vertices[0];

	for (int i = 1; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (tri.vertices[i][j] < min[j]) min[j] = tri.vertices[i][j];
			else if (tri.vertices[i][j] > max[j]) max[j] = tri.vertices[i][j];
		}

	mLocation	= min;
	mE1			= Vec3 (max.x - min.x, 0.0f, 0.0f);
	mE2			= Vec3 (0.0f, max.y - min.y, 0.0f);
	mE3			= Vec3 (0.0f, 0.0f, max.z - min.z);
}

void Primitives::OBB::From (const Primitives::AABB& aabb)
{
	mLocation	= aabb.Min ();
	mE1			= Vec3 (aabb.Max ().x - aabb.Min ().x, 0.0f, 0.0f);
	mE2			= Vec3 (0.0f, aabb.Max ().y - aabb.Min ().y, 0.0f);
	mE3			= Vec3 (0.0f, 0.0f, aabb.Max ().z - aabb.Min ().z);
}

void Primitives::OBB::Transform (const Mat4& m)
{
	mLocation	= m.TransformCoord (mLocation);
	mE1			= m.TransformNormal (mE1);
	mE2			= m.TransformNormal (mE2);
	mE3			= m.TransformNormal (mE3);
}

void Primitives::OBB::Projection (float& a, float& b, const Vec3& axis) const
{
	Vec3 verts[] = {
		mLocation,
		mLocation + mE1,
		mLocation + mE2,
		mLocation + mE3,
		mLocation + mE1 + mE2,
		mLocation + mE1 + mE3,
		mLocation + mE2 + mE3,
		mLocation + mE1 + mE2 + mE3
	};

	a = b = Vec3::Dot (verts[0], axis);
	
	for (int i = 1; i < 8; i++)
	{
		float dot = Vec3::Dot (verts[i], axis);

		if (dot < a) a = dot;
		else if (dot > b) b = dot;
	}
}



//
// Plane
//

void Primitives::Plane::Projection (float& a, float& b, const Vec3& axis) const
{
	if (Vec3::Dot (axis.GetNormalized (), Normal ().GetNormalized ()) > 0.998f)
	{
		a = b = 0.0f;
	}
	else
	{
		a = -FLT_MAX;
		b = FLT_MAX;
	}
}



//
// Functions
//

bool Primitives::TestIntersection (const Primitive& p1, const Primitive& p2, const Vec3& axis)
{
	float a1, b1, a2, b2;
	p1.Projection (a1, b1, axis);
	p2.Projection (a2, b2, axis);

	return !((b1 < a2) || (a1 > b2));
}

bool Primitives::TestIntersection (const Primitives::Triangle& p1, const Primitives::Triangle& p2)
{
	Vec3 edges[2][3] = {
		{p1.vertices[1] - p1.vertices[0], p1.vertices[2] - p1.vertices[0], p1.vertices[2] - p1.vertices[1]},
		{p2.vertices[1] - p2.vertices[0], p2.vertices[2] - p2.vertices[0], p2.vertices[2] - p2.vertices[1]}
	};

	Vec3 axis;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{			
			axis = Vec3::Cross (edges[0][i], edges[1][j]);

			if (!TestIntersection (p1, p2, axis)) return false;
		}

	Vec3 norm1 = Vec3::Cross (edges[0][0], edges[0][1]);
	Vec3 norm2 = Vec3::Cross (edges[1][0], edges[1][1]);

	return TestIntersection (p1, p2, norm1) && TestIntersection (p1, p2, norm2);
}

bool Primitives::TestIntersection (const Primitives::AABB& p1, const Primitives::AABB& p2)
{
	const Vec3& p1min = p1.Min ();
	const Vec3& p1max = p1.Max ();
	const Vec3& p2min = p2.Min ();
	const Vec3& p2max = p2.Max ();

	return !((p1max.x < p2min.x) || (p1min.x > p2max.x) ||
		(p1max.y < p2min.y) || (p1min.y > p2max.y) ||
		(p1max.z < p2min.z) || (p1min.z > p2max.z));
}

bool Primitives::TestIntersection (const Primitives::OBB& p1, const Primitives::OBB& p2)
{
	Vec3 edges[2][3] = {
		{p1.E1 (), p1.E2 (), p1.E3 ()},
		{p2.E1 (), p2.E2 (), p2.E3 ()}
	};

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 3; j++)
			if (!TestIntersection (p1, p2, edges[i][j])) return false;

	Vec3 axis;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			axis = Vec3::Cross (edges[0][i], edges[1][j]);

			if (!TestIntersection (p1, p2, axis)) return false;
		}

	return true;
}

bool Primitives::TestIntersection (const Primitives::AABB& p1, const Primitives::Triangle& p2)
{
	Vec3 edges[2][3] = {
		{Vec3 (1.0f, 0.0f, 0.0f), Vec3 (0.0f, 1.0f, 0.0f), Vec3 (0.0f, 0.0f, 1.0f)},
		{p2.vertices[1] - p2.vertices[0], p2.vertices[2] - p2.vertices[0], p2.vertices[2] - p2.vertices[1]}
	};

	Vec3 axis;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			axis = Vec3::Cross (edges[0][i], edges[1][j]);

			if (!TestIntersection (p1, p2, axis)) return false;
		}

	return TestIntersection (p1, p2, Vec3 (1.0f, 0.0f, 0.0f)) &&
		TestIntersection (p1, p2, Vec3 (0.0f, 1.0f, 0.0f)) &&
		TestIntersection (p1, p2, Vec3 (0.0f, 0.0f, 1.0f));
}

bool Primitives::TestIntersection (const Primitives::AABB& p1, const Primitives::OBB& p2)
{
	Vec3 edges[2][3] = {
		{Vec3 (1.0f, 0.0f, 0.0f), Vec3 (0.0f, 1.0f, 0.0f), Vec3 (0.0f, 0.0f, 1.0f)},
		{p2.E1 (), p2.E2 (), p2.E3 ()}
	};

	Vec3 axis;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			axis = Vec3::Cross (edges[0][i], edges[1][j]);

			if (!TestIntersection (p1, p2, axis)) return false;
		}

	return TestIntersection (p1, p2, Vec3 (1.0f, 0.0f, 0.0f)) &&
		TestIntersection (p1, p2, Vec3 (0.0f, 1.0f, 0.0f)) &&
		TestIntersection (p1, p2, Vec3 (0.0f, 0.0f, 1.0f)) &&
		TestIntersection (p1, p2, p2.E1 ()) &&
		TestIntersection (p1, p2, p2.E2 ()) &&
		TestIntersection (p1, p2, p2.E3 ());
}

bool Primitives::TestIntersection (float& t, Vec3& normal, const Primitives::Triangle& p, const Ray& r, const float& max_t)
{
	Vec3 ku, kv;

	ku = p.vertices[1] - p.vertices[0];
	kv = p.vertices[2] - p.vertices[0];

	normal = Vec3::Cross (ku, kv).GetNormalized ();

	float vd = Vec3::Dot (normal, r.Direction ());

	t = Vec3::Dot (p.vertices[0] - r.Origin (), normal) / vd;

	if ((t < 0.0f) || (t > max_t)) return false;

	Vec3 loc = r.Point (t) - p.vertices[0];

	float s11	= ku.LenSq ();
	float s12	= Vec3::Dot (ku, kv);
	float s22	= kv.LenSq ();
	float d		= s11 * s22 - s12 * s12;

	float u = Vec3::Dot (loc, ku * s22 - kv * s12);
	float v = Vec3::Dot (loc, kv * s11 - ku * s12);

	//float u = Vec3::Dot (loc, ku);
	//float v = Vec3::Dot (loc, kv);

	return (u > 0) && (v > 0) && (u + v < d);
}

bool Primitives::TestIntersection (float& t, Vec3& normal, const Primitives::AABB& p, const Ray& r, const float& max_t)
{
	int nearnormind = 0;
	int farnormind = 0;

	float invlx = 1.0f / r.Direction ().x;
	float invly = 1.0f / r.Direction ().y;

	float t1x = (p.Min ().x - r.Origin ().x) * invlx;
	float t2x = (p.Max ().x - r.Origin ().x) * invlx;

	float tnear	= std::min (t1x, t2x);
	float tfar	= std::max (t1x, t2x);

	float t1y = (p.Min ().y - r.Origin ().y) * invly;
	float t2y = (p.Max ().y - r.Origin ().y) * invly;

	if (t1y > t2y) std::swap (t1y, t2y);

	if (t1y > tnear)
	{
		tnear = t1y;
		nearnormind = 1;
	}
	if (t2y < tfar)
	{
		tfar = t2y;
		farnormind = 1;
	}

	if ((tnear > tfar) || (tfar < 0)) return false;

	float invlz = 1.0f / r.Direction ().z;
	float t1z = (p.Min ().z - r.Origin ().z) * invlz;
	float t2z = (p.Max ().z - r.Origin ().z) * invlz;

	if (t1z > t2z) std::swap (t1z, t2z);

	if (t1z > tnear)
	{
		tnear = t1z;
		nearnormind = 2;
	}
	if (t2z < tfar)
	{
		tfar = t2z;
		farnormind = 2;
	}

	if ((tfar > 0) && (tnear < tfar))
	{
		normal.xyz (0.0f, 0.0f, 0.0f);

		if (tnear > 0)
		{
			t = tnear;
			if (t > max_t)
				return false;

			normal[nearnormind] = -1;
		}
		else
		{
			t = tfar;
			normal[farnormind] = 1;
		}

		return true;
	}

	return false;
}

bool Primitives::TestIntersection (float& t, Vec3& normal, const Primitives::Sphere& p, const Ray& r, const float& max_t)
{
	Vec3 l	= p.Position () - r.Origin ();
	float l2	= l.LenSq ();
	float tca	= Vec3::Dot (l, r.Direction ());
	float t2hc	= p.Radius () * p.Radius () - l2 + tca * tca;

	if( t2hc <= 0.0f )
		return false;

	t2hc = math_sqrt( t2hc );

	t = tca < t2hc ? tca + t2hc : tca - t2hc;
	if( t > max_t )
		return false;

	normal = (r.Point (t) - p.Position ()).GetNormalized ();

	return true;
}

bool Primitives::TestIntersection (float& t, Vec3& normal, const Primitives::Plane& p, const Ray& r, const float& max_t)
{
	float vd = Vec3::Dot( p.Normal( ), r.Direction( ) );

	t = -( Vec3::Dot( r.Origin( ), p.Normal( ) ) + p.D( ) ) / vd;

	return t > 0.0f && t < max_t ? normal = p.Normal( ), true : false;
}



_XFX_END
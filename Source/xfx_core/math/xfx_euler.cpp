//  File xfx_euler.cpp                                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_euler.h"

_XFX_BEGIN





//
// Euler
//

Mat4 Euler::ToMat4 () const
{
	const float cosy = Math::Instance ().Cos (yaw);
	const float siny = Math::Instance ().Sin (yaw);
	const float cosr = Math::Instance ().Cos (roll);
	const float sinr = Math::Instance ().Sin (roll);
	const float cosp = Math::Instance ().Cos (pitch);
	const float sinp = Math::Instance ().Sin (pitch);

	return Mat4 (
		sinp * siny * sinr + cosy * cosr,	cosp * sinr,	sinp * cosy * sinr - cosr * siny,	0,
		sinp * siny * cosr - cosy * sinr,	cosp * cosr,	sinp * cosy * cosr + siny * sinr,	0,
		cosp * siny,						-sinp,			cosp * cosy,						0,
		0,									0,				0,									1
		);
}

Euler& Euler::FromMat4 (const Mat4& m)
{
	//FIXME: works only for non-normalized matrix

	float sy = m._12;

	if (sy > 1.0f)
		sy = 1.0f;
	if (sy < -1.0f)
		sy = -1.0f;

	Math::Angle theta = math_asin( sy );
	float cy = -math_cos( theta );

	if (fabs (cy) > 0.001f)
	{
		yaw		= math_rad2angle( -atan2f( m._13, m._11 ) );
		pitch	= theta;
		roll	= math_rad2angle( -atan2f( m._32, m._22 ) );
	}
	else
	{
		yaw		= math_rad2angle( -atan2f( m._31, m._33 ) );
		pitch	= theta;
		roll	= 0;
	}

	return *this;
}

Euler& Euler::FromVec3 (const Vec3& v)
{
	if (fabs (v.x) < 0.00001f && fabs (v.y) < 0.000001f)
		yaw = pitch = 0;
	else
	{
		yaw = math_rad2angle( atan2f( v.x, v.z ) );

		float f = math_sqrt( v.x * v.x + v.z * v.z );
		pitch = math_rad2angle( atan2f( v.y, f ) );
	}

	roll = 0;
	return *this;
}

void Euler::AngleDirections (Vec3& f, Vec3& u, Vec3& r) const
{
	const float cosy = Math::Instance ().Cos (yaw);
	const float siny = Math::Instance ().Sin (yaw);
	const float cosr = Math::Instance ().Cos (roll);
	const float sinr = Math::Instance ().Sin (roll);
	const float cosp = Math::Instance ().Cos (pitch);
	const float sinp = Math::Instance ().Sin (pitch);

	f.xyz (cosp * siny, -sinp, cosp * cosy);
	u.xyz (sinp * siny * cosr - cosy * sinr, cosp * cosr, sinp * cosy * cosr + siny * sinr);
	r.xyz (sinp * siny * sinr + cosy * cosr, cosp * sinr, sinp * cosy * sinr - cosr * siny);
}

Vec3 Euler::Forward () const
{
	const float cosy = Math::Instance ().Cos (yaw);
	const float siny = Math::Instance ().Sin (yaw);
	const float cosp = Math::Instance ().Cos (pitch);
	const float sinp = Math::Instance ().Sin (pitch);

	return Vec3 (cosp * siny, -sinp, cosp * cosy);
}

Vec3 Euler::Up () const
{
	const float cosy = Math::Instance ().Cos (yaw);
	const float siny = Math::Instance ().Sin (yaw);
	const float cosr = Math::Instance ().Cos (roll);
	const float sinr = Math::Instance ().Sin (roll);
	const float cosp = Math::Instance ().Cos (pitch);
	const float sinp = Math::Instance ().Sin (pitch);

	return Vec3 (sinp * siny * cosr - cosy * sinr, cosp * cosr, sinp * cosy * cosr + siny * sinr);
}

Vec3 Euler::Right () const
{
	const float cosy = Math::Instance ().Cos (yaw);
	const float siny = Math::Instance ().Sin (yaw);
	const float cosr = Math::Instance ().Cos (roll);
	const float sinr = Math::Instance ().Sin (roll);
	const float cosp = Math::Instance ().Cos (pitch);
	const float sinp = Math::Instance ().Sin (pitch);

	return Vec3 (sinp * siny * sinr + cosy * cosr, cosp * sinr, sinp * cosy * sinr - cosr * siny);
}

void Euler::Rotate (const Vec3& axis, const Math::BigAngle& ang)
{
	Mat4 m;
	m.RotationAxisByAngle (axis, ang);

	FromMat4 (ToMat4 () * m);
}



_XFX_END
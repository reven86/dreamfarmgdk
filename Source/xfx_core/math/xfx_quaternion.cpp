#include "xfx.h"
#include "xfx_quaternion.h"

_XFX_BEGIN




//
// Quaternion
//

Quaternion::Quaternion( const Vec3& axis, const Math::Angle& angle )
{
	FromAxisAngle( axis, angle );
}

Quaternion& Quaternion::operator *= ( const Quaternion& v )
{
	xyzw(
		w * v.x + x * v.w + y * v.z - z * v.y,
		w * v.y + y * v.w + z * v.x - x * v.z,
		w * v.z + z * v.w + x * v.y - y * v.x,
		w * v.w - x * v.x - y * v.y - z * v.z
		);

	return *this;
}

Quaternion& Quaternion::FromAxisAngle( const Vec3& axis, const Math::Angle& angle )
{
	Vec3 axis_norm = axis.GetNormalized( );

	Math::Angle half_angle = angle / 2;
    float sine = math_sin( half_angle );
    w = math_cos( half_angle );
    x = axis_norm.x * sine;
    y = axis_norm.y * sine;
    z = axis_norm.z * sine;

	return *this;
}

boost::tuple< Vec3, Math::Angle > Quaternion::ToAxisAngle( )
{
	float len_sq = x * x + y * y + z * z;

	if( len_sq < 0.0001f )
		return boost::make_tuple( Vec3( 0.0f ), Math::Angle( 0 ) );

	float inv_len = math_invsqrt( len_sq );

	return boost::make_tuple( Vec3( x, y, z ) * inv_len, math_normalize_angle( math_acos( w ) * 2 ) );
}

Mat4 Quaternion::ToMat4( )
{
    float tx  = 2.0f * x;
    float ty  = 2.0f * y;
    float tz  = 2.0f * z;
    float twx = tx * w;
    float twy = ty * w;
    float twz = tz * w;
    float txx = tx * x;
    float txy = ty * x;
    float txz = tz * x;
    float tyy = ty * y;
    float tyz = tz * y;
    float tzz = tz * z;

	return Mat4(
		1.0f - ( tyy + tzz ), txy + twz, txz - twy, 0.0f,
		txy - twz, 1.0f - ( txx + tzz ), tyz + twx, 0.0f,
		txz + twy, tyz - twx, 1.0f - ( txx + tyy ), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

// CounterWarp: A helper function used by Slerp.
inline float CounterWarp(float t, float fCos) 
{
    const float ATTENUATION = 0.82279687f;
    const float WORST_CASE_SLOPE = 0.58549219f;

    float fFactor = 1.0f - ATTENUATION * fCos;
    fFactor *= fFactor;
    float fK = WORST_CASE_SLOPE * fFactor;

    return t*(fK*t*(2.0f*t - 3.0f) + 1.0f + fK);
}

Quaternion Quaternion::Slerp( const Quaternion& u, const Quaternion& v, float t )
{
    // This algorithm is Copyright (c) 2002 Jonathan Blow, from his article 
    // "Hacking Quaternions" in Game Developer Magazine, March 2002.
    
    float cos = Dot( u, v );

    float prime;
    if( t <= 0.5f )
        prime = CounterWarp( t, cos );
	else
        prime = 1.0f - CounterWarp( 1.0f - t, cos );

	return Quaternion(
		u.x + ( v.x - u.x ) * prime,
		u.y + ( v.y - u.y ) * prime,
		u.z + ( v.z - u.z ) * prime,
		u.w + ( v.w - u.w ) * prime
		).GetNormalized( );
}

Quaternion Quaternion::GetInversed( ) const
{
	return Quaternion( -x, -y, -z, w ) * math_invsqrt( LenSq( ) );
}



_XFX_END
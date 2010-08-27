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
	const float cosy = Math::Instance( ).Cos( yaw );
	const float siny = Math::Instance( ).Sin( yaw );
	const float cosr = Math::Instance( ).Cos( roll );
	const float sinr = Math::Instance( ).Sin( roll );
	const float cosp = Math::Instance( ).Cos( pitch );
	const float sinp = Math::Instance( ).Sin( pitch );

	return Mat4 (
		sinp * siny * sinr + cosy * cosr,	cosp * sinr,	sinp * cosy * sinr - cosr * siny,	0,
		sinp * siny * cosr - cosy * sinr,	cosp * cosr,	sinp * cosy * cosr + siny * sinr,	0,
		cosp * siny,						-sinp,			cosp * cosy,						0,
		0,									0,				0,									1
		);
}

Euler& Euler::FromMat4( const Mat4& m )
{
	float kx = 1.0f / Vec3( m._11, m._12, m._13 ).Len( );
	float ky = 1.0f / Vec3( m._21, m._22, m._23 ).Len( );
	float kz = 1.0f / Vec3( m._31, m._32, m._33 ).Len( );

	float sy = -m._32 * kz;

	if( sy > 1.0f )
		sy = 1.0f;
	if( sy < -1.0f )
		sy = -1.0f;

	pitch = math_asin( sy );

	if( fabs( sy ) > 0.99f )
	{
		// pitch is about pi/2, extremal case

		// sine of pitch is about 1, then
		// m._21 is sine of yaw minus roll and
		// m._11 is cosine of yaw minus roll
		// if sine of pitch is about -1, then
		// m._21 become a negative sine of yaw plus roll and
		// m._11 become a cosine of yaw plus roll

		Math::BigAngle yaw_minus_roll =
			sy > 0.0f ? math_rad2angle( atan2f( m._21 * ky, m._11 * kx ) ) :
			math_rad2angle( atan2f( -m._21 * ky, m._11 * kx ) );

		yaw = yaw_minus_roll;
		roll = 0;
	}
	else
	{
		yaw = math_rad2angle( atan2f( m._31, m._33 ) );
		roll = math_rad2angle( atan2f( m._12 * kx, m._22 * ky ) );
	}

	return *this;
}

Euler& Euler::FromVec3( const Vec3& v_not_norm )
{
	Vec3 v = v_not_norm.GetNormalized( );

	if( fabs( v.x ) < 0.00001f && fabs( v.y ) < 0.000001f )
		yaw = pitch = 0;
	else
	{
		yaw = math_rad2angle( atan2f( v.x, v.z ) );

		float f = math_sqrt( v.x * v.x + v.z * v.z );
		pitch = math_rad2angle( -atan2f( v.y, f ) );
	}

	roll = 0;
	return *this;
}

Quaternion Euler::ToQuaternion( ) const
{
	Quaternion q;

    // Algorithm in Ken Shoemake's article in 1987 SIGGraPH course notes
    // article "Quaternion Calculus and Fast Animation".
	
	Vec3 f[ 3 ];
	AngleDirections( f[ 2 ], f[ 1 ], f[ 0 ] );

    float fTrace = f[ 0 ][ 0 ] + f[ 1 ][ 1 ] + f[ 2 ][ 2 ];
    float fRoot;

    if ( fTrace > 0.0f )
    {
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = math_sqrt( fTrace+1.0f );  // 2w
        q.w = 0.5f * fRoot;
        fRoot = 0.5f / fRoot;  // 1/(4w)

        q.x = ( f[ 2 ][ 1 ] - f[ 1 ][ 2 ] ) * fRoot;
        q.y = ( f[ 0 ][ 2 ] - f[ 2 ][ 0 ] ) * fRoot;
        q.z = ( f[ 1 ][ 0 ] - f[ 0 ][ 1 ] ) * fRoot;
    }
    else
    {
        // |w| <= 1/2
        static int next[3] = { 1, 2, 0 };
        int i = 0;
        if ( f[ 1 ][ 1 ] > f[ 0 ][ 0 ] )
            i = 1;
        if ( f[ 2 ][ 2 ] > f[ i ][ i ] ) 
            i = 2;
        int j = next[i];
        int k = next[j];

        fRoot = math_sqrt( f[ i ][ i ] - f[ j ][ j ] - f[ k ][ k ] + 1.0f );
        float* quat[3] = { &q.x, &q.y, &q.z };
        *quat[ i ] = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;
        q.w = ( f[ j ][ k ] - f[ k ][ j ] ) * fRoot;
        *quat[j] = ( f[ j ][ i ] + f[ i ][ j ] ) * fRoot;
        *quat[k] = ( f[ k ][ i ] + f[ i ][ k ] ) * fRoot;
    }

	return q;
}

Euler& Euler::FromQuaternion( const Quaternion& q )
{
    float tx  = 2.0f * q.x;
    float ty  = 2.0f * q.y;
    float tz  = 2.0f * q.z;
    float twx = tx * q.w;
    float twy = ty * q.w;
    float twz = tz * q.w;
    float txx = tx * q.x;
    float txy = ty * q.x;
    float txz = tz * q.x;
    float tyy = ty * q.y;
    float tyz = tz * q.y;
    float tzz = tz * q.z;

	float sy = twx - tyz;

	pitch = math_asin( sy );

	if( fabs( sy ) > 0.99f )
	{
		// pitch is about pi/2, extremal case

		// sine of pitch is about 1, then
		// m._21 is sine of yaw minus roll and
		// m._11 is cosine of yaw minus roll
		// if sine of pitch is about -1, then
		// m._21 become a negative sine of yaw plus roll and
		// m._11 become a cosine of yaw plus roll

		Math::BigAngle yaw_minus_roll =
			sy > 0.0f ? math_rad2angle( atan2f( txy - twz, 1.0f - ( tyy + tzz ) ) ) :
			math_rad2angle( atan2f( twz - txy, 1.0f - ( tyy + tzz ) ) );

		yaw = yaw_minus_roll;
		roll = 0;
	}
	else
	{
		yaw = math_rad2angle( atan2f( txz + twy, 1.0f - ( txx + tyy ) ) );
		roll = math_rad2angle( atan2f( txy + twz, 1.0f - ( txx + tzz ) ) );
	}

	return *this;
}

void Euler::AngleDirections( Vec3& f, Vec3& u, Vec3& r ) const
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
	m.MakeRotationAxisByAngle (axis, ang);

	FromMat4 (ToMat4 () * m);
}



_XFX_END
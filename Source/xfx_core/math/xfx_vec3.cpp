//  File xfx_vec3.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_vec3.h"

_XFX_BEGIN





//
// Vec3
//

void Vec3::Normalize( )
{
	float invlsq = math_invsqrt( LenSq( ) );

	x *= invlsq;
	y *= invlsq;
	z *= invlsq;
}

Vec3 Vec3::GetNormalized( ) const
{
	float invlsq = math_invsqrt( LenSq( ) );

	return Vec3( x * invlsq, y * invlsq, z * invlsq );
}

void Vec3::Clip( Vec3& v )
{
	if( v.x < 0.0f ) v.x = 0.0f;
	else if( v.x > 1.0f ) v.x = 1.0f;
	if( v.y < 0.0f ) v.y = 0.0f;
	else if( v.y > 1.0f ) v.y = 1.0f;
	if( v.z < 0.0f ) v.z = 0.0f;
	else if( v.z > 1.0f ) v.z = 1.0f;
}




_XFX_END
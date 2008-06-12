//  File xfx_vec4.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_vec4.h"

_XFX_BEGIN





//
// Vec4
//

void Vec4::Normalize( )
{
	float invlsq = math_invsqrt( LenSq( ) );

	x *= invlsq;
	y *= invlsq;
	z *= invlsq;
	w *= invlsq;
}

Vec4 Vec4::GetNormalized( ) const
{
	float invlsq = math_invsqrt( LenSq( ) );

	return Vec4( x * invlsq, y * invlsq, z * invlsq, w * invlsq );
}

void Vec4::Clip( Vec4& v )
{
	if( v.x < 0.0f ) v.x = 0.0f;
	else if( v.x > 1.0f ) v.x = 1.0f;
	if( v.y < 0.0f ) v.y = 0.0f;
	else if( v.y > 1.0f ) v.y = 1.0f;
	if( v.z < 0.0f ) v.z = 0.0f;
	else if( v.z > 1.0f ) v.z = 1.0f;
	if( v.w < 0.0f ) v.w = 0.0f;
	else if( v.w > 1.0f ) v.w = 1.0f;
}




_XFX_END
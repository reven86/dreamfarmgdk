//  File xfx_vec2.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_vec2.h"

_XFX_BEGIN





//
// Vec2
//

void Vec2::Normalize( )
{
	float invlsq = math_invsqrt( LenSq( ) );

	x *= invlsq;
	y *= invlsq;
}

Vec2 Vec2::GetNormalized( ) const
{
	float invlsq = math_invsqrt( LenSq( ) );

	return Vec2( x * invlsq, y * invlsq );
}

void Vec2::Clip( Vec2& v )
{
	if( v.x < 0.0f )
		v.x = 0.0f;
	else if( v.x > 1.0f )
		v.x = 1.0f;

	if( v.y < 0.0f )
		v.y = 0.0f;
	else if( v.y > 1.0f )
		v.y = 1.0f;
}




_XFX_END
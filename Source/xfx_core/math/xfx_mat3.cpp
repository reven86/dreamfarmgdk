//  File xfx_mat3.cpp                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_mat3.h"

_XFX_BEGIN





//
// Mat3
//

Mat3::Mat3( float v )
{
	x[ 0 ][ 0 ] = v; x[ 0 ][ 1 ] = 0.0f; x[ 0][ 2 ] = 0.0f;
	x[ 1 ][ 0 ] = 0.0f; x[ 1 ][ 1 ] = v; x[ 1 ][ 2 ] = 0.0f;
	x[ 2 ][ 0 ] = 0.0f; x[ 2 ][ 1 ] = 0.0f; x[ 2 ][ 2 ] = 1.0f;
}

Mat3::Mat3( float a_11, float a_12, float a_13,
			float a_21, float a_22, float a_23,
			float a_31, float a_32, float a_33 )
{
	x[ 0 ][ 0 ] = a_11; x[ 0 ][ 1 ] = a_12; x[ 0 ][ 2 ] = a_13;
	x[ 1 ][ 0 ] = a_21; x[ 1 ][ 1 ] = a_22; x[ 1 ][ 2 ] = a_23;
	x[ 2 ][ 0 ] = a_31; x[ 2 ][ 1 ] = a_32; x[ 2 ][ 2 ] = a_33;
}

Mat3::Mat3( const Vec2& pos, const Math::Angle& orientation, const Vec2& scale )
{
	Reset( pos, orientation, scale );
}

Mat4 Mat3::ExpandToMat4( ) const
{
	Mat4 res;

	res._11 = _11;	res._12 = _12;	res._13 = _13;	res._14 = 0;
	res._21 = _21;	res._22 = _22;	res._23 = _23;	res._24 = 0;
	res._31 = _31;	res._32 = _32;	res._33 = _33;	res._34 = 0;
	res._41 = 0.0f;	res._42 = 0.0f;	res._43 = 0.0f;	res._44 = 1.0f;

	return res;
}

Mat3& Mat3::operator *= ( const Mat3& A )
{
	Mat3 res = *this;
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
		{
			float sum = 0;
			for( int k = 0; k < 3; k++ )
				sum += res.x[ i ][ k ] * A.x[ k ][  j ];
			x[ i ][ j ] = sum;
		}

	return *this;
}

Mat3 operator * ( const Mat3& A, const Mat3& B )
{
	Mat3 out;
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
		{
			float sum = 0;
			for( int k = 0; k < 3; k++ )
				sum += A.x[ i ][ k ] * B.x[ k ][ j ];
			out.x[ i ][ j ] = sum;
		}

	return out;
}

Mat3& Mat3::Identity( )
{
	x[ 0 ][ 0 ] = 1; x[ 0 ][ 1 ] = 0; x[ 0 ][ 2 ] = 0;
	x[ 1 ][ 0 ] = 0; x[ 1 ][ 1 ] = 1; x[ 1 ][ 2 ] = 0;
	x[ 2 ][ 0 ] = 0; x[ 2 ][ 1 ] = 0; x[ 2 ][ 2 ] = 1;

	return *this;
}

bool Mat3::IsIdentity( ) const
{
	const float eps = 0.000001f;

	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			if( i == j && fabs( x[ i ][ j ] - 1.0f ) > eps ||
				i != j && fabs( x[ i ][ j ] ) > eps )
			{
				return false;
			}

	return true;
}

void Mat3::Translate( const Vec2& loc )
{
	x[ 0 ][ 0 ] = 1; x[ 0 ][ 1 ] = 0; x[ 0 ][ 2 ] = 0;
	x[ 1 ][ 0 ] = 0; x[ 1 ][ 1 ] = 1; x[ 1 ][ 2 ] = 0;
	x[ 2 ][ 0 ] = loc.x; x[ 2 ][ 1 ] = loc.y; x[ 2 ][ 2 ] = 1;
}

void Mat3::Scale( const Vec2& v )
{
	x[ 0 ][ 0 ] = v.x; x[ 0 ][ 1 ] = 0; x[ 0 ][ 2 ] = 0;
	x[ 1 ][ 0 ] = 0; x[ 1 ][ 1 ] = v.y; x[ 1 ][ 2 ] = 0;
	x[ 2 ][ 0 ] = 0; x[ 2 ][ 1 ] = 0; x[ 2 ][ 2 ] = 1.0f;
}

void Mat3::Reset( const Vec2& pos, const Math::Angle& orientation, const Vec2& scale )
{
	Rotation( orientation );
	SetScale( scale );
	SetTranslation( pos );
}

void Mat3::Rotation( Math::BigAngle angle )
{
	float Cosine	= math_cos( angle );
	float Sine		= math_sin( angle );

	x[ 0 ][ 0 ] = Cosine; x[ 0 ][ 1 ] = Sine; x[ 0 ][ 2 ] = 0;
	x[ 1 ][ 0 ] = -Sine; x[ 1 ][ 1 ] = Cosine; x[ 1 ][ 2 ] = 0;
	x[ 2 ][ 0 ] = 0; x[ 2 ][ 1 ] = 0; x[ 2 ][ 2 ] = 1;
}

Mat3 Mat3::GetTransposed( ) const
{
	Mat3 res;

	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			res.x[ i ][ j ] = x[ j ][ i ];

	return res;
}

Mat3 Mat3::GetInversed( ) const
{
	Mat3 A( *this );
	Mat3 out( 1 );

	for( int i = 0; i < 3; i++ )
	{
		float d = A.x[ i ][ i ];
		if( d != 1.0f )
		{
			d = 1.0f / d;

			for( int j = 0; j < 3; j++ )
			{
				out.x[ i ][ j ]	*= d;
				A.x[ i ][ j ]	*= d;
			}
		}

		for( int j = 0; j < 3; j++ )
			if( j != i )
				if( A.x[ j ][ i ] != 0.0f )
				{
					float mulby = A.x[ j ][ i ];
					for( int k = 0; k < 3; k++ )
					{
						A.x[ j ][ k ]	-= mulby * A.x[ i ][ k ];
						out.x[ j ][ k ]	-= mulby * out.x[ i ][ k ];
					}
				}
	}

	return out;
}

Mat3 Mat3::GetInversedAffine( ) const
{
	Mat3 res;

	float kx = 1.0f / ( Vec2( x[ 0 ][ 0 ], x[ 1 ][ 0 ] ).LenSq( ) );
	float ky = 1.0f / ( Vec2( x[ 0 ][ 1 ], x[ 1 ][ 1 ] ).LenSq( ) );

	res.x[ 0 ][ 0 ] = x[ 0 ][ 0 ] * kx;
	res.x[ 1 ][ 0 ] = x[ 0 ][ 1 ] * ky;
	res.x[ 0 ][ 1 ] = x[ 1 ][ 0 ] * kx;
	res.x[ 1 ][ 1 ] = x[ 1 ][ 1 ] * ky;

	res.x[ 0 ][ 2 ] = res.x[ 1 ][ 2 ] = 0.0f;
	res.x[ 2 ][ 2 ] = 1.0f;

	res.x[ 2 ][ 0 ] = -( x[ 0 ][ 0 ] * x[ 2 ][ 0 ] + x[ 0 ][ 1 ] * x[ 2 ][ 1 ] ) * kx;
	res.x[ 2 ][ 1 ] = -( x[ 1 ][ 0 ] * x[ 2 ][ 0 ] + x[ 1 ][ 1 ] * x[ 2 ][ 1 ] ) * ky;

	return res;
}

Vec2 Mat3::TransformCoord( const Vec2& v ) const
{
	Vec2 out;

	out.x = v.x * x[ 0 ][ 0 ] + v.y * x[ 1 ][ 0 ] + x[ 2 ][ 0 ];
	out.y = v.x * x[ 0 ][ 1 ] + v.y * x[ 1 ][ 1 ] + x[ 2 ][ 1 ];

	if( x[ 2 ][ 2 ] != 1.0f )
		if( x[ 2 ][ 2 ] != 0.0f )
			out /= x[ 2 ][ 2 ];

	return out;
}

Vec2 Mat3::TransformNormal( const Vec2& v ) const
{
	Vec2 out;

	out.x = v.x * x[ 0 ][ 0 ] + v.y * x[ 1 ][ 0 ];
	out.y = v.x * x[ 0 ][ 1 ] + v.y * x[ 1 ][ 1 ];

	return out;
}

void Mat3::SetScale( const Vec2& v )
{
	float kx = v.x / ( Vec2( _11, _21 ).Len( ) );
	float ky = v.y / ( Vec2( _12, _22 ).Len( ) );

	_11 *= kx; _12 *= ky;
	_21 *= kx; _22 *= ky;
}



_XFX_END
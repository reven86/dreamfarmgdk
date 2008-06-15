//  File xfx_vec2.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_math.h"

/*!	\file xfx_vec2.h 
 *	\brief Two dimension vector.
 */

_XFX_BEGIN





/*! \class Vec2 xfx_vec2.h "math/xfx_vec2.h"
 *	\brief 2D vector class.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Vec2
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	float x, y;

	//! Constructs uninitialized vector.
	Vec2								( ) { };

	//! Constructs vector, initialized by value.
	explicit Vec2						( float v ) : x( v ), y( v ) { };

	//! Copy constructor.
	Vec2								( const Vec2& v ) : x( v.x ), y( v.y ) { };

	//! Constructs vector from two components.
	Vec2								( float vx, float vy ) : x( vx ), y( vy ) { };

	//! Assignment operator.
	Vec2& operator =					( const Vec2& v ) { x = v.x; y = v.y; return *this; };

	Vec2 operator -						( ) const { return Vec2( -x, -y ); };
	Vec2& operator +=					( const Vec2& v ) { x += v.x; y += v.y; return *this; };
	Vec2& operator -=					( const Vec2& v ) { x -= v.x; y -= v.y; return *this; };
	Vec2& operator *=					( float v ) { x *= v; y *= v; return *this; };
	Vec2& operator *=					( const Vec2& v ) { x *= v.x; y *= v.y; return *this; };
	Vec2& operator /=					( float v ) { float invv = 1.0f / v; x *= invv; y *= invv; return *this; };

	//! Get component.
	float& operator [ ]					( int n ) { _ASSERTE( n < 2); return *( &x + n ); };

	//! Get component.
	const float& operator [ ]			( int n ) const { _ASSERTE( n < 2 ); return *( &x + n ); };

	friend inline Vec2 operator +		( const Vec2& u, const Vec2& v ) { return Vec2( u.x + v.x, u.y + v.y ); };
	friend inline Vec2 operator -		( const Vec2& u, const Vec2& v ) { return Vec2( u.x - v.x, u.y - v.y ); };
	friend inline Vec2 operator *		( float f, const Vec2& v ) { return Vec2( f * v.x, f * v.y ); };
	friend inline Vec2 operator *		( const Vec2& v, float f ) { return Vec2( v.x * f, v.y * f ); };
	friend inline Vec2 operator /		( const Vec2& v, float f ) { float invf = 1.0f / f; return Vec2( v.x * invf, v.y * invf ); };
	friend inline bool operator ==		( const Vec2& u, const Vec2& v ) { return( u.x == v.x ) && ( u.y == v.y ); };
	friend inline bool operator !=		( const Vec2& u, const Vec2& v ) { return( u.x != v.x ) || ( u.y != v.y ); };

	//! Set X, Y components.
	void xy								( float _x, float _y ) { x = _x; y = _y; };

	//! Normalize vector.
	void Normalize						( );

	//! Get normalized vector.
	Vec2 GetNormalized					( ) const;

	//! Get square of length.
	float LenSq							( ) const { return x * x + y * y; };
	
	//! Get length.
	float Len							( ) const { return math_sqrt( LenSq( ) ); };

	//! Lerp vector.
	static inline Vec2 Lerp				( const Vec2& u, const Vec2& v, const float& t ) { return v * t + u * (1.0f - t); };

	//! Get dot product.
	static inline float	Dot				( const Vec2& u, const Vec2& v ) { return u.x * v.x + u.y * v.y; };

	//! Get random not normalized vector.
	static inline Vec2 Rand				( ) { return Vec2( rand( ) - 0.5f * RAND_MAX, rand( ) - 0.5f * RAND_MAX ); };

	//! Clips coords in range [0.0f, 1.0f]
	static void Clip					( Vec2& v );
};




//! Parse Vec2 variable.
template< class _String >
HRESULT ParseVariable( Vec2& var, typename _String::size_type& pos, const _String& str )
{
	skip_comments( str, pos );
	Script::ParseVariable< float >( var.x, pos, str );
	Script::ParseVariable< float >( var.y, pos, str );
	return S_OK;
};


_XFX_END

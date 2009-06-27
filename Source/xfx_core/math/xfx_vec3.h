//  File xfx_vec3.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_math.h"

/*!	\file xfx_vec3.h 
 *	\brief Three dimension vector.
 */

_XFX_BEGIN





/*! \class Vec3 xfx_vec3.h "math/xfx_vec3.h"
 *	\brief 3D vector class.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Vec3
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive>
	void serialize( _Archive& ar, const unsigned int version );

public:
	float x, y, z;

	//! Constructs uninitialized vector.
	Vec3								() {};

	//! Constructs vector, initialized by value.
	explicit Vec3						(float v) : x (v), y (v), z (v) {};

	//! Copy constructor.
	Vec3								(const Vec3& v) : x (v.x), y (v.y), z (v.z) {};

	//! Constructs vector from three components.
	Vec3								(float vx, float vy, float vz) : x (vx), y (vy), z (vz) {};

	//! Assignment operator.
	Vec3& operator =					(const Vec3& v) {x = v.x; y = v.y; z = v.z; return *this;};

	Vec3 operator -						() const {return Vec3 (-x, -y, -z);};
	Vec3& operator +=					(const Vec3& v) {x += v.x; y += v.y; z += v.z; return *this;};
	Vec3& operator -=					(const Vec3& v) {x -= v.x; y -= v.y; z -= v.z; return *this;};
	Vec3& operator *=					(float v) {x *= v; y *= v; z *= v; return *this;};
	Vec3& operator *=					(const Vec3& v) {x *= v.x; y *= v.y; z *= v.z; return *this;};
	Vec3& operator /=					(float v) {float invv = 1.0f / v; x *= invv; y *= invv; z *= invv; return *this;};

	//! Get component.
	float& operator []					(int n) {_ASSERTE (n < 3); return *(&x + n);};

	//! Get component.
	const float& operator []			(int n) const {_ASSERTE (n < 3); return *(&x + n);};

	friend inline Vec3 operator +		( const Vec3& u, const Vec3& v ) { return Vec3 (u.x + v.x, u.y + v.y, u.z + v.z);};
	friend inline Vec3 operator -		( const Vec3& u, const Vec3& v ) { return Vec3 (u.x - v.x, u.y - v.y, u.z - v.z);};
	friend inline Vec3 operator *		( float f, const Vec3& v ) { return Vec3 (f * v.x , f * v.y, f * v.z);};
	friend inline Vec3 operator *		( const Vec3& v, float f ) { return Vec3 (v.x * f, v.y * f, v.z * f);};
	friend inline Vec3 operator /		( const Vec3& v, float f ) { float invf = 1.0f / f; return Vec3 (v.x * invf, v.y * invf, v.z * invf);};
	friend inline bool operator ==		( const Vec3& u, const Vec3& v ) { return (u.x == v.x) && (u.y == v.y) && (u.z == v.z);};
	friend inline bool operator !=		( const Vec3& u, const Vec3& v ) { return (u.x != v.x) || (u.y != v.y) || (u.z != v.z);};
	friend inline bool operator <		( const Vec3& u, const Vec3& v ) { return ( u.x < v.x ) && ( u.y < v.y ) && ( u.z < v.z ); };

	//! Set X, Y, Z components.
	void xyz							( float _x, float _y, float _z ) { x = _x; y = _y; z = _z; };

	//! Normalize vector.
	void Normalize						( );

	//! Get normalized vector.
	Vec3 GetNormalized					( ) const;

	//! Get square of length.
	float LenSq							( ) const { return x * x + y * y + z * z; };
	
	//! Get length.
	float Len							( ) const { return math_sqrt( LenSq( ) ); };

	//! Lerp vector.
	static inline Vec3 Lerp				(const Vec3& u, const Vec3& v, const float& t) {return v * t + u * (1.0f - t);};

	//! Get dot product.
	static inline float	Dot				(const Vec3& u, const Vec3& v) {return u.x * v.x + u.y * v.y + u.z * v.z;};

	//! Get cross product.
	static inline Vec3 Cross			(const Vec3& u, const Vec3& v) {return Vec3 (u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);};

	//! Get random not normalized vector.
	static inline Vec3 Rand				() {return Vec3 (rand () - 0.5f * RAND_MAX, rand () - 0.5f * RAND_MAX, rand () - 0.5f * RAND_MAX);};

	//! Clips coords in range [0.0f, 1.0f]
	static void Clip					(Vec3& v);
};



//! Parse Vec3 variable.
template< class _String >
HRESULT ParseVariable( Vec3& var, typename _String::size_type& pos, const _String& str )
{
	skip_comments( str, pos );
	Script< _String >::ParseVariable< float >( var.x, pos, str );
	Script< _String >::ParseVariable< float >( var.y, pos, str );
	Script< _String >::ParseVariable< float >( var.z, pos, str );
	return S_OK;
};



_XFX_END

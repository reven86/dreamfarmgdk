//  File xfx_vec4.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_math.h"

/*!	\file xfx_vec4.h 
 *	\brief Four dimension vector.
 */

_XFX_BEGIN





/*! \class Vec4 xfx_vec4.h "math/xfx_vec4.h"
 *	\brief 4D vector class.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Vec4
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	float x, y, z, w;

	//! Constructs uninitialized vector.
	Vec4								() {};

	//! Constructs vector, initialized by value.
	explicit Vec4						(float v) : x (v), y (v), z (v), w (v) {};

	//! Copy constructor.
	Vec4								(const Vec4& v) : x (v.x), y (v.y), z (v.z), w (v.w) {};

	//! Constructs vector from four components.
	Vec4								(float vx, float vy, float vz, float vw) : x (vx), y (vy), z (vz), w (vw) {};

	//! Assignment operator.
	Vec4& operator =					(const Vec4& v) {x = v.x; y = v.y; z = v.z; w = v.w; return *this;};

	Vec4 operator -						() const {return Vec4 (-x, -y, -z, -w);};
	Vec4& operator +=					(const Vec4& v) {x += v.x; y += v.y; z += v.z; w += v.w; return *this;};
	Vec4& operator -=					(const Vec4& v) {x -= v.x; y -= v.y; z -= v.z; w += v.w; return *this;};
	Vec4& operator *=					(float v) {x *= v; y *= v; z *= v; w *= v; return *this;};
	Vec4& operator *=					(const Vec4& v) {x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this;};
	Vec4& operator /=					(float v) {float invv = 1.0f / v; x *= invv; y *= invv; z *= invv; w *= invv; return *this;};

	//! Get component.
	float& operator []					(int n) {_ASSERTE (n < 4); return *(&x + n);};

	//! Get component.
	const float& operator []			(int n) const {_ASSERTE (n < 4); return *(&x + n);};

	//operator	D3DXVECTOR4*			() {return reinterpret_cast<D3DXVECTOR4 *> (&x);};
	//operator	const D3DXVECTOR4*		() const {return reinterpret_cast<const D3DXVECTOR4*> (&x);};

	friend inline Vec4 operator +		(const Vec4& u, const Vec4& v) {return Vec4 (u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);};
	friend inline Vec4 operator -		(const Vec4& u, const Vec4& v) {return Vec4 (u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);};
	friend inline Vec4 operator *		(float f, const Vec4& v) {return Vec4 (f * v.x , f * v.y, f * v.z, f * v.w);};
	friend inline Vec4 operator *		(const Vec4& v, float f) {return Vec4 (v.x * f, v.y * f, v.z * f, v.w * f);};
	friend inline Vec4 operator /		(const Vec4& v, float f) {float invf = 1.0f / f; return Vec4 (v.x * invf, v.y * invf, v.z * invf, v.w * invf);};
	friend inline bool operator ==		(const Vec4& u, const Vec4& v) {return (u.x == v.x) && (u.y == v.y) && (u.z == v.z) && (u.w == v.w);};
	friend inline bool operator !=		(const Vec4& u, const Vec4& v) {return (u.x != v.x) || (u.y != v.y) || (u.z != v.z) || (u.w != v.w);};

	//! Set X, Y, Z, W components.
	void xyzw							( float _x, float _y, float _z, float _w ) { x = _x; y = _y; z = _z; w = _w; };

	//! Normalize vector.
	void Normalize						( );

	//! Get normalized vector.
	Vec4 GetNormalized					( ) const;

	//! Get square of length.
	float LenSq							( ) const { return x * x + y * y + z * z + w * w; };

	//! Get length.
	float Len							( ) const { return math_sqrt( LenSq( ) ); };

	//! Lerp vector.
	static inline Vec4 Lerp				(const Vec4& u, const Vec4& v, const float& t) {return v * t + u * (1.0f - t);};

	//! Get dot product.
	static inline float	Dot				(const Vec4& u, const Vec4& v) {return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;};

	//! Get random not normalized vector.
	static inline Vec4 Rand				() {return Vec4 (rand () - 0.5f * RAND_MAX, rand () - 0.5f * RAND_MAX, rand () - 0.5f * RAND_MAX, rand () - 0.5f * RAND_MAX);};

	//! Clips coords in range [0.0f, 1.0f]
	static void Clip					(Vec4& v);

	//! String representation.
	String ToString						( ) const { return boost::str( StringFormat( "Vec4(%f, %f, %f, %f)" ) % x % y % z % w ); };
};



_XFX_END

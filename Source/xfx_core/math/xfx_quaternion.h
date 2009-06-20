//  File xfx_quaternion.h                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once



/*!	\file xfx_quaternion.h 
 *	\brief Quaternions.
 */

#include "xfx_vec3.h"
#include "xfx_mat4.h"


_XFX_BEGIN





/*! \class Quaternion xfx_quaternion.h "math/xfx_quaternion.h"
 *	\brief %Quaternions support.
 *	\ingroup MathGroup
 *	
 *	\author Andrew "RevEn" Karpushin
 */

class Quaternion
{
public:
	float x, y, z, w;

	//! Empty constructor.
	Quaternion																	( ) { };

	//! Constructs quaternion with specific values.
	Quaternion																	( float _x, float _y, float _z, float _w ) : x( _x ), y( _y ), z( _z ), w( _w ) { };

	//! Constructs quaternion from axis and angle.
	Quaternion																	( const Vec3& axis, const Math::Angle& angle );

	//! Sets values.
	void								xyzw									( float _x, float _y, float _z, float _w ) { x = _x; y = _y; z = _z; w = _w; };

	//
	// Operators.
	//

	//! Get a negative quaternion.
	Quaternion							operator -								( ) const { return Quaternion( -x, -y, -z, -w ); };

	Quaternion&							operator +=								( const Quaternion& v ) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; };
	Quaternion&							operator -=								( const Quaternion& v ) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; };
	Quaternion&							operator *=								( float v ) { x *= v; y *= v; z *= v; w *= v; return *this; };
	Quaternion&							operator /=								( float v ) { float invv = 1.0f / v; x *= invv; y *= invv; z *= invv; w *= invv; return *this; };
	Quaternion&							operator *=								( const Quaternion& v );

	friend inline Quaternion			operator +								( const Quaternion & u, const Quaternion & v ) { return Quaternion( u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w ); };
	friend inline Quaternion			operator -								( const Quaternion& u, const Quaternion& v ) { return Quaternion( u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w); };
	friend inline Quaternion			operator *								( float f, const Quaternion& v ) { return Quaternion( f * v.x , f * v.y, f * v.z, f * v.w ); };
	friend inline Quaternion			operator *								( const Quaternion& v, float f ) { return Quaternion( v.x * f, v.y * f, v.z * f, v.w * f ); };
	friend inline Quaternion			operator /								( const Quaternion& v, float f ) { float invf = 1.0f / f; return Quaternion( v.x * invf, v.y * invf, v.z * invf, v.w * invf ); };
	friend inline bool					operator ==								( const Quaternion& u, const Quaternion& v ) { return ( u.x == v.x ) && ( u.y == v.y ) && ( u.z == v.z ) && ( u.w == v.w ); };
	friend inline bool					operator !=								( const Quaternion& u, const Quaternion& v ) { return ( u.x != v.x ) || ( u.y != v.y ) || ( u.z != v.z ) || ( u.w != v.w ); };
	friend inline Quaternion			operator *								( const Quaternion& u, const Quaternion& v ) { Quaternion q( u ); q *= v; return q; };

	//! Get square length.
	float								LenSq									( ) const { return x * x + y * y + z * z + w * w; };

	//! Get length.
	float								Len										( ) const { return math_sqrt( LenSq( ) ); };

	//! Normalize (use sqrtf instead of math_invsqrt to get higher accuracy).
	void								Normalize								( ) { *this *= 1.0f / sqrtf( LenSq( ) ); };

	//! Get normlized (use sqrtf instead of math_invsqrt to get higher accuracy)..
	Quaternion							GetNormalized							( ) const { return Quaternion( *this ) * ( 1.0f / sqrtf( LenSq( ) ) ); };

	//! Inverse quaternion.
	void								Inverse									( ) { *this = GetInversed( ); };

	//! Get inversed quaternion.
	Quaternion							GetInversed								( ) const;

	//! Convert from axis and angle.
	Quaternion&							FromAxisAngle							( const Vec3& axis, const Math::Angle& angle );

	//! Convert to axis and angle.
	boost::tuple< Vec3, Math::Angle >	ToAxisAngle								( );

	//! Convert to Mat4.
	Mat4								ToMat4									( );

	//! Get dot product.
	static inline float					Dot										( const Quaternion& u, const Quaternion& v ) { return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w; };

	//! Slerp.
	static Quaternion					Slerp									( const Quaternion& u, const Quaternion& v, float t );
};




/*! \struct QuaternionLerpFn xfx_quaternion.h "utility/xfx_quaternion.h"
 *	\brief Quaternion lerp functor. Performs slerp.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

struct QuaternionLerpFn
{
	Quaternion lerp( const Quaternion& a, const Quaternion& b, const float& t ) { return Quaternion::Slerp( a, b, t ); };
};


_XFX_END
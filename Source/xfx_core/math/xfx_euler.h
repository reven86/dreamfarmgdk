//  File xfx_euler.h                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_math.h"
#include "xfx_mat4.h"
#include "xfx_quaternion.h"

/*!	\file xfx_euler.h 
 *	\brief %Euler angles.
 */

_XFX_BEGIN





/*! \class Euler xfx_euler.h "math/xfx_euler.h"
 *	\brief %Euler angles.
 *	\ingroup MathGroup
 *	
 *	\author Andrew "RevEn" Karpushin
 */

class Euler
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	Math::BigAngle		yaw;		//!< Angle for y axis - 3
	Math::BigAngle		pitch;		//!< Angle for x axis - 2
	Math::BigAngle		roll;		//!< Angle for z axis - 1

	//! Constructs uninitialized Euler.
	Euler														( ) { };

	//! Constructs initialized by one angle Euler.
	explicit Euler												( Math::BigAngle a ) : yaw( a ), pitch( a ), roll( a ) { };

	//! Constructs initialized by three angle Euler.
	Euler														( Math::BigAngle _y, Math::BigAngle _p, Math::BigAngle _r ) : yaw( _y ), pitch( _p ), roll( _r ) {};

	//Compiler-generated copy constructor and assignment operator are fine

	Euler&							operator +=					(const Euler& v) {yaw += v.yaw; pitch += v.pitch; roll += v.roll; return *this;};
	Euler&							operator -=					(const Euler& v) {yaw -= v.yaw; pitch -= v.pitch; roll -= v.roll; return *this;};
	Euler							operator -					() const {return Euler (-yaw, -pitch, -roll);};

	friend inline Euler				operator +					(const Euler& u, const Euler& v) {return Euler (u.yaw + v.yaw, u.pitch + v.pitch, u.roll + v.roll);};
	friend inline Euler				operator -					(const Euler& u, const Euler& v) {return Euler (u.yaw - v.yaw, u.pitch - v.pitch, u.roll - v.roll);};
	friend inline Euler				operator *					(const Euler& u, float t) {return Euler (static_cast<Math::BigAngle> (u.yaw * t), static_cast<Math::BigAngle> (u.pitch * t), static_cast<Math::BigAngle> (u.roll * t));};
	friend inline Euler				operator *					(float t, const Euler& u) {return Euler (static_cast<Math::BigAngle> (u.yaw * t), static_cast<Math::BigAngle> (u.pitch * t), static_cast<Math::BigAngle> (u.roll * t));};
	friend inline bool				operator ==					(const Euler& u, const Euler& v) {return (u.yaw == v.yaw) && (u.pitch == v.pitch) && (u.roll == v.roll);};
	friend inline bool				operator !=					(const Euler& u, const Euler& v) {return (u.yaw != v.yaw) || (u.pitch != v.pitch) || (u.roll != v.roll);};

	//! Set yaw, pitch, roll.
	void							ypr							(const Math::BigAngle& y, const Math::BigAngle& p, const Math::BigAngle& r) {yaw = y; pitch = p; roll = r;};

	//! Convert to Mat4
	Mat4							ToMat4						() const;

	//! Convert from Mat4
	Euler&							FromMat4					(const Mat4& m);

	//! Convert from Vec3. \note Roll will always be 0.
	Euler&							FromVec3					(const Vec3& v);

	//! Convert to Vec3. \note Roll ignored.
	Vec3							ToVec3						( ) const { return Forward( ); };

	//! Convert to Quaternion.
	Quaternion						ToQuaternion				( ) const;

	//! Convert from Quaternion.
	Euler&							FromQuaternion				( const Quaternion& q );

	//! Get angle directions.
	void							AngleDirections				( Vec3& f, Vec3& u, Vec3& r ) const;

	//! Get forward direction.
	Vec3							Forward						( ) const;

	//! Get up direction.
	Vec3							Up							( ) const;

	//! Get right direction.
	Vec3							Right						( ) const;

	//! Rotate angles by axis and axis angle.
	void							Rotate						( const Vec3& axis, const Math::BigAngle& ang );

	//! Linear interpolation of Euler angles.
	static inline Euler				Lerp						( const Euler& u, const Euler& v, const float& t ) { return v * t + u * ( 1.0f - t ); };
};





//! Parse Euler variable.
template< class _String >
HRESULT ParseVariable( Euler& var, typename _String::size_type& pos, const _String& str )
{
	skip_comments( str, pos );
	ParseBigAngleVariable( var.yaw, pos, str );
	ParseBigAngleVariable( var.pitch, pos, str );
	ParseBigAngleVariable( var.roll, pos, str );

	return S_OK;
};



_XFX_END

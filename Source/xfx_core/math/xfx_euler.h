//  File xfx_euler.h                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_math.h"
#include "xfx_mat4.h"

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
#ifdef __XFX_USE_BOOST_SERIALIZATION__
	friend class boost::serialization::access;

	//! Serialization.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version )
	{
		ar & BOOST_SERIALIZATION_NVP( yaw );
		ar & BOOST_SERIALIZATION_NVP( pitch );
		ar & BOOST_SERIALIZATION_NVP( roll );
	};
#endif

public:
	Math::BigAngle		yaw;		//!< Angle for y axis - 3
	Math::BigAngle		pitch;		//!< Angle for x axis - 2
	Math::BigAngle		roll;		//!< Angle for z axis - 1

	//! Constructs uninitialized Euler.
	Euler														() {};

	//! Constructs initialized by one angle Euler.
	explicit Euler												(Math::BigAngle a) : yaw (a), pitch (a), roll (a) {};

	//! Constructs initialized by three angle Euler.
	Euler														(Math::BigAngle _y, Math::BigAngle _p, Math::BigAngle _r) : yaw (_y), pitch (_p), roll (_r) {};

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
	Vec3							ToVec3						() const {return Forward ();};

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

	//! Lerp Euler.
	static inline Euler				Lerp						( const Euler& u, const Euler& v, const float& t ) { return v * t + u * ( 1.0f - t ); };

	//! Slerp Euler.
	static inline Euler				Slerp						( const Euler& u, const Euler& v, const float& t )
	{ 
		return Euler( 
			math_slerp_angle( 
				math_normalize_angle( u.yaw ),
				math_normalize_angle( v.yaw ),
				t
			),
			math_slerp_angle( 
				math_normalize_angle( u.pitch ),
				math_normalize_angle( v.pitch ),
				t
			), 
			math_slerp_angle( 
				math_normalize_angle( u.roll ),
				math_normalize_angle( v.roll ),
				t 
			) );
	};
};



/*! \struct EulerLerpFn xfx_utils.h "utility/xfx_utils.h"
 *	\brief Euler lerp functor. Performs slerp.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

struct EulerLerpFn
{
	Euler lerp( const Euler& a, const Euler& b, const float& t ) { return Euler::Slerp( a, b, t ); };
};



//! Parse Euler variable.
template< class _String >
HRESULT ParseVariable( Euler& var, typename _String::size_type& pos, const _String& str )
{
	skip_comments( str, pos );
	Script::ParseBigAngleVariable( var.yaw, pos, str );
	Script::ParseBigAngleVariable( var.pitch, pos, str );
	Script::ParseBigAngleVariable( var.roll, pos, str );

	return S_OK;
};



_XFX_END



#ifdef __XFX_USE_BOOST_SERIALIZATION__
BOOST_CLASS_IMPLEMENTATION( xfx::Euler, boost::serialization::object_serializable );
BOOST_CLASS_TRACKING( xfx::Euler, boost::serialization::track_never )
#endif
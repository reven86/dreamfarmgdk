//  File xfx_mat4.h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_vec3.h"
#include "xfx_math.h"

/*!	\file xfx_mat4.h 
 *	\brief Matrix 4x4.
 */

_XFX_BEGIN





class Euler;

#pragma warning (push)
#pragma warning (disable:4201) // nameless struct/union

/*! \class Mat4 xfx_mat4.h "math/xfx_mat4.h"
 *	\brief Matrix 4x4.
 *	\ingroup MathGroup
 *	
 *	\author Andrew "RevEn" Karpushin
 */

class Mat4
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
#ifndef __GCCXML__
	union
	{
		float x[4][4];				//!< Array access.

		//! Component access.
		struct
		{
#endif // __GCCXML__
			float _11; float _12; float _13; float _14;
			float _21; float _22; float _23; float _24;
			float _31; float _32; float _33; float _34;
			float _41; float _42; float _43; float _44;
#ifndef __GCCXML__
		};
	};
#endif // __GCCXML__

	//! Constructs uninitialized matrix.
	Mat4															() {};

	//! Constructs matrix by one value.
	explicit Mat4													(float v);

	//! Constructs matrix by set of values.
	Mat4															(float _11, float _12, float _13, float _14,
																	 float _21, float _22, float _23, float _24,
																	 float _31, float _32, float _33, float _34,
																	 float _41, float _42, float _43, float _44);

	//! Constructs matrix from position, rotation and scale.
	Mat4															( const Vec3& pos, const Euler& orientation, const Vec3& scale );

	Mat4&							operator *=						(const Mat4& A);

	friend Mat4						operator *						(const Mat4& A, const Mat4& B);

	//! Set identity matrix.
	Mat4&							MakeIdentity					( );

	//! Tests if matrix is identity.
	bool							IsIdentity						( ) const;

	//! Set translate matrix.
	void							Translate						( const Vec3& l );

	//! Set scale matrix.
	void							MakeScale						( const Vec3& v );

	//! Reset matrix by position, rotation and scale.
	void							Reset							( const Vec3& pos, const Euler& orientation, const Vec3& scale );

	//! Set rotation matrix around X axis.
	void							MakeRotationX					( Math::BigAngle angle );

	//! Set rotation matrix around Y axis.
	void							MakeRotationY					( Math::BigAngle angle );

	//! Set rotation matrix around Z axis.
	void							MakeRotationZ					( Math::BigAngle angle );

	//! Set rotation matrix around custom axis and sine and cosine for angle.
	void							MakeRotationAxis				( const Vec3& axis, float cosine, float sine );

	//! Set rotation matrix around custom axis.
	void							MakeRotationAxisByAngle			( const Vec3& axis, Math::BigAngle Angle ) { MakeRotationAxis( axis, Math::Instance( ).Cos( Angle ), Math::Instance( ).Sin( Angle ) ); };

	//! Transpose matrix.
	void							Transpose						( ) { *this = GetTransposed( ); };

	//! Get transposed matrix.
	Mat4							GetTransposed					( ) const;

	//! Set left-handed perspective matrix.
	void							PerspectiveFovLH				( float fov, float aspect, float znear, float zfar );

	//! Set right-handed perspective matrix.
	void							PerspectiveFovRH				( float fov, float aspect, float znear, float zfar );

	//! Set left-handed "look at" matrix.
	void							LookAtLH						(const Vec3& cam, const Vec3& dir, const Vec3& vy);

	//! Set right-handed "look at" matrix.
	void							LookAtRH						(const Vec3& cam, const Vec3& dir, const Vec3& vy);

	//! Inverse matrix. For affine matrices use InverseAffine.
	void							Inverse							() {*this = GetInversed ();};
	//! Get inversed matrix. For affine matrices use GetInversedAffine.
	Mat4							GetInversed						() const;

	//! Inverse affine matrix. Fast.
	void							InverseAffine					() {*this = GetInversedAffine ();};
	//! Get inversed affine matrix. Fast.
	Mat4							GetInversedAffine				() const;

	//! Multiply current matrix by translation transform.
	void							SetTranslation					(const Vec3& v) { _41 = v.x; _42 = v.y; _43 = v.z;};

	//! Multiply current matrix by scale transform.
	void							SetScale						(const Vec3& v);

	//! Get translation from current transform.
	Vec3							GetTranslation					() const {return Vec3 (_41, _42, _43);};

	//! Get scale from current transform.
	Vec3							GetScale						() const {return Vec3 (Vec3 (_11, _21, _31).Len (), Vec3 (_12, _22, _32).Len (), Vec3 (_13, _23, _33).Len ());};

	//! Transform coordinate by matrix.
	Vec3							TransformCoord					(const Vec3& v) const;

	//! Transform normal by matrix.
	Vec3							TransformNormal					(const Vec3& v) const;
};

#pragma warning (pop)



_XFX_END

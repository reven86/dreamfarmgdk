//  File xfx_mat3.h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_vec2.h"
#include "xfx_math.h"
#include "xfx_mat4.h"

/*!	\file xfx_mat3.h 
 *	\brief Matrix 3x3.
 */

_XFX_BEGIN





/*! \class Mat3 xfx_mat3.h "math/xfx_mat3.h"
 *	\brief Matrix 3x3 for 2D transformations.
 *	\ingroup MathGroup
 *	
 *	\author Andrew "RevEn" Karpushin
 */

class Mat3
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	union
	{
		float x[ 3 ][ 3 ];				//!< Array access.

		//! Component access.
		struct
		{
			float _11; float _12; float _13;
			float _21; float _22; float _23;
			float _31; float _32; float _33;
		};
	};

	//! Constructs uninitialized matrix.
	Mat3															( ) { };

	//! Constructs matrix by one value.
	explicit Mat3													( float v );

	//! Constructs matrix by set of values.
	Mat3															( float _11, float _12, float _13,
																	  float _21, float _22, float _23,
																	  float _31, float _32, float _33 );

	//! Constructs matrix from position, rotation and scale.
	Mat3															( const Vec2& pos, const Math::Angle& orientation, const Vec2& scale );

	Mat3&							operator *=						( const Mat3& A );

	friend Mat3						operator *						( const Mat3& A, const Mat3& B );

	//! Expand to matrix 4x4
	Mat4							ExpandToMat4					( ) const;

	//! Set identity matrix.
	Mat3&							Identity						( );

	//! Tests if matrix is identity.
	bool							IsIdentity						( ) const;

	//! Set translate matrix.
	void							Translate						( const Vec2& l );

	//! Set scale matrix.
	void							Scale							( const Vec2& v );

	//! Reset matrix by position, rotation and scale.
	void							Reset							( const Vec2& pos, const Math::Angle& orientation, const Vec2& scale );

	//! Set rotation matrix.
	void							Rotation						( Math::BigAngle angle );

	//! Transpose matrix.
	void							Transpose						( ) { *this = GetTransposed( ); };

	//! Get transposed matrix.
	Mat3							GetTransposed					( ) const;

	//! Inverse matrix. For affine matrices use InverseAffine.
	void							Inverse							( ) { *this = GetInversed( ); };

	//! Get inversed matrix. For affine matrices use GetInversedAffine.
	Mat3							GetInversed						( ) const;

	//! Inverse affine matrix. Fast.
	void							InverseAffine					( ) { *this = GetInversedAffine( ); };

	//! Get inversed affine matrix. Fast.
	Mat3							GetInversedAffine				( ) const;

	//! Multiply current matrix by translation transform.
	void							SetTranslation					( const Vec2& v ) { x[ 2 ][ 0 ] = v.x; x[ 2 ][ 1 ] = v.y; };

	//! Multiply current matrix by scale transform.
	void							SetScale						( const Vec2& v );

	//! Get translation from current transform.
	Vec2							GetTranslation					( ) const { return Vec2( x[ 2 ][ 0 ], x[ 2 ][ 1 ] ); };

	//! Get scale from current transform.
	Vec2							GetScale						( ) const { return Vec2( Vec2( _11, _21 ).Len( ), Vec2( _12, _22 ).Len( ) ); };

	//! Transform coordinate by matrix.
	Vec2							TransformCoord					( const Vec2& v ) const;

	//! Transform normal by matrix.
	Vec2							TransformNormal					( const Vec2& v ) const;
};



_XFX_END

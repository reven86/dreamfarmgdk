//  File xfx_transformable.h                                                  |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_vec2.h"
#include "xfx_vec3.h"
#include "xfx_euler.h"
#include "xfx_mat3.h"
#include "xfx_mat4.h"

/*!	\file xfx_transformable.h 
 *	\brief %Transformable classes.
 */

_XFX_BEGIN





/*! \class Transformable3D xfx_transformable.h "utility/xfx_transformable.h"
 *	\brief %Transformable base class for transforms in 3D space.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Transformable3D
{
	friend class boost::serialization::access;

	Vec3							mPosition;
	Euler							mRotation;
	Vec3							mScale;
	Mat4							mTransformation;

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	//! Constructs base transformable object with identity transformation matrix.
	Transformable3D												( ) : mTransformation( 1 ), mPosition( 0 ), mRotation( 0 ), mScale( 1 ) { };
	virtual ~Transformable3D									( ) { };

	//Compiler-generated copy constructor and assignment operator are fine

	//! Get position.
	const Vec3&						GetPosition					( ) const { return mPosition; };

	//! Get rotation.
	const Euler&					GetRotation					( ) const { return mRotation; };

	//! Get scale.
	const Vec3&						GetScale					( ) const { return mScale; };

	//! Get transformation matrix.
	const Mat4&						GetTransformation			( ) const { return mTransformation; };

	//! Set position.
	void							SetPosition					( const Vec3& pos );

	//! Set rotation.
	void							SetRotation					( const Euler& rot );

	//! Set scale.
	void							SetScale					( const Vec3& scale );

	//! Reset all transform: position, rotation and scale.
	void							ResetTransform				( const Vec3& pos, const Euler& rot, const Vec3& scale );
};





/*! \class Transformable2D xfx_transformable.h "utility/xfx_transformable.h"
 *	\brief %Transformable base class for transforms in 2D space.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Transformable2D
{
	friend class boost::serialization::access;

	Vec2							mPosition;
	Math::BigAngle					mRotation;
	Vec2							mScale;
	Mat3							mTransformation;

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	//! Constructs base transformable object with identity transformation matrix.
	Transformable2D												( ) : mTransformation( 1 ), mPosition( 0 ), mRotation( 0 ), mScale( 1 ) { };
	virtual ~Transformable2D									( ) { };

	//Compiler-generated copy constructor and assignment operator are fine

	//! Get position.
	const Vec2&						GetPosition					( ) const { return mPosition; };

	//! Get rotation.
	const Math::BigAngle&			GetRotation					( ) const { return mRotation; };

	//! Get scale.
	const Vec2&						GetScale					( ) const { return mScale; };

	//! Get transformation matrix.
	const Mat3&						GetTransformation			( ) const { return mTransformation; };

	//! Set position.
	void							SetPosition					( const Vec2& pos );

	//! Set rotation.
	void							SetRotation					( const Math::BigAngle& rot );

	//! Set scale.
	void							SetScale					( const Vec2& scale );

	//! Reset all transform: position, rotation and scale.
	void							ResetTransform				( const Vec2& pos, const Math::BigAngle& rot, const Vec2& scale );
};




_XFX_END

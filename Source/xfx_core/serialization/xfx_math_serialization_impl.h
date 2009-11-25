//  File xfx_math_serialization_impl.h                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_math_serialization_impl.h 
 *	\brief Serialization implementation for Math library.
 */





_XFX_BEGIN






//
// Euler
//

template< class _Archive >
void Euler::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( yaw );
	ar & BOOST_SERIALIZATION_NVP( pitch );
	ar & BOOST_SERIALIZATION_NVP( roll );
};

BOOST_CLASS_IMPLEMENTATION( xfx::Euler, boost::serialization::object_serializable );
BOOST_CLASS_TRACKING( xfx::Euler, boost::serialization::track_never )



//
// Mat3
//

template< class _Archive >
void Mat3::serialize( _Archive& ar, const unsigned int version )
{
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			ar & boost::serialization::make_nvp( "x", x[ i ][ j ] );
};

BOOST_CLASS_IMPLEMENTATION( xfx::Mat3, boost::serialization::object_serializable );
BOOST_CLASS_TRACKING( xfx::Mat3, boost::serialization::track_never )



//
// Mat4
//

template< class _Archive >
void Mat4::serialize( _Archive& ar, const unsigned int version )
{
	for( int i = 0; i < 4; i++ )
		for( int j = 0; j < 4; j++ )
			ar & boost::serialization::make_nvp( "x", x[ i ][ j ] );
};

BOOST_CLASS_IMPLEMENTATION( xfx::Mat4, boost::serialization::object_serializable );
BOOST_CLASS_TRACKING( xfx::Mat4, boost::serialization::track_never )




//
// Ray
//

template< class _Archive >
void Ray::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mOrigin );
	ar & BOOST_SERIALIZATION_NVP( mDirection );
};



//
// Sphere
//

template< class _Archive >
void Primitives::Sphere::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mPosition );
	ar & BOOST_SERIALIZATION_NVP( mRadius );
};



//
// Triangle
//

template< class _Archive >
void Primitives::Triangle::serialize( _Archive& ar, const unsigned int version )
{
	for( int i = 0; i < 3; i++ )
		ar & boost::serialization::make_nvp( "vertex", vertices[ i ] );
};




//
// AABB
//

template< class _Archive >
void Primitives::AABB::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mMin );
	ar & BOOST_SERIALIZATION_NVP( mMax );
};



//
// OBB
//

template< class _Archive >
void Primitives::OBB::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mLocation );
	ar & BOOST_SERIALIZATION_NVP( mE1 );
	ar & BOOST_SERIALIZATION_NVP( mE2 );
	ar & BOOST_SERIALIZATION_NVP( mE3 );
};



//
// Plane
//

template< class _Archive >
void Primitives::Plane::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mA );
	ar & BOOST_SERIALIZATION_NVP( mB );
	ar & BOOST_SERIALIZATION_NVP( mC );
	ar & BOOST_SERIALIZATION_NVP( mD );
};




//
// Transformable3D
//

template< class _Archive >
void Transformable3D::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mPosition );
	ar & BOOST_SERIALIZATION_NVP( mRotation );
	ar & BOOST_SERIALIZATION_NVP( mScale );
	ar & BOOST_SERIALIZATION_NVP( mTransformation );
};



//
// Transformable2D
//

template< class _Archive >
void Transformable2D::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mPosition );
	ar & BOOST_SERIALIZATION_NVP( mRotation );
	ar & BOOST_SERIALIZATION_NVP( mScale );
	ar & BOOST_SERIALIZATION_NVP( mTransformation );
};



//
// Vec2
//

template< class _Archive >
void Vec2::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( x );
	ar & BOOST_SERIALIZATION_NVP( y );
};

BOOST_CLASS_IMPLEMENTATION( xfx::Vec2, boost::serialization::object_serializable );
BOOST_CLASS_TRACKING( xfx::Vec2, boost::serialization::track_never )



//
// Vec3
//

template< class _Archive >
void Vec3::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( x );
	ar & BOOST_SERIALIZATION_NVP( y );
	ar & BOOST_SERIALIZATION_NVP( z );
};

BOOST_CLASS_IMPLEMENTATION( xfx::Vec3, boost::serialization::object_serializable );
BOOST_CLASS_TRACKING( xfx::Vec3, boost::serialization::track_never )



//
// Vec4
//

template< class _Archive >
void Vec4::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( x );
	ar & BOOST_SERIALIZATION_NVP( y );
	ar & BOOST_SERIALIZATION_NVP( z );
	ar & BOOST_SERIALIZATION_NVP( w );
};

BOOST_CLASS_IMPLEMENTATION( xfx::Vec4, boost::serialization::object_serializable );
BOOST_CLASS_TRACKING( xfx::Vec4, boost::serialization::track_never )




_XFX_END
//  File xfx_transformable.cpp                                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_transformable.h"

_XFX_BEGIN





//
// Transformable3D
//

void Transformable3D::SetPosition( const Vec3& pos )
{
	mPosition = pos;

	mTransformation.SetTranslation( pos );
}

void Transformable3D::SetRotation( const Euler& rot )
{
	mRotation = rot;

	mTransformation = mRotation.ToMat4( );
	mTransformation.SetScale( mScale );
	mTransformation.SetTranslation( mPosition );
}

void Transformable3D::SetScale( const Vec3& scale )
{
	mScale = scale;
	mTransformation.SetScale( mScale );
}

void Transformable3D::ResetTransform( const Vec3& pos, const Euler& rot, const Vec3& scale )
{
	mPosition	= pos;
	mRotation	= rot;
	mScale		= scale;

	mTransformation.Reset( pos, rot, scale );
}





//
// Transformable2D
//

void Transformable2D::SetPosition( const Vec2& pos )
{
	mPosition = pos;

	mTransformation.SetTranslation( pos );
}

void Transformable2D::SetRotation( const Math::BigAngle& rot )
{
	mRotation = rot;

	mTransformation.Reset( mPosition, math_normalize_angle( mRotation ), mScale );
}

void Transformable2D::SetScale( const Vec2& scale )
{
	mScale = scale;
	mTransformation.SetScale( mScale );
}

void Transformable2D::ResetTransform( const Vec2& pos, const Math::BigAngle& rot, const Vec2& scale )
{
	mPosition	= pos;
	mRotation	= rot;
	mScale		= scale;

	mTransformation.Reset( pos, math_normalize_angle( rot ), scale );
}



_XFX_END
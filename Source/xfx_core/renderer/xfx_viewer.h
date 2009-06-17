//  File xfx_viewer->h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_viewer.h 
 *	\brief Defines a camera parameters.
 */

_XFX_BEGIN





/*! \class Viewer xfx_viewer.h "engine/xfx_viewer.h"
 *	\brief %Viewer attributes class.
 *	\ingroup RenderGroup
 *	
 *	Defines a camera representation in system. %Viewer can be created using following attributes:
 *	- Position as Vec3;
 *	- Rotation as Euler;
 *	- Field Of View (FOV);
 *	- Distance to near plane;
 *	- Distance to far plane;
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Viewer
{
	Vec3				mPosition;
	Euler				mRotation;

	float				mFOV;		//In radians!

	float				mNear;
	float				mFar;

	// Frustum planes
	// Note: Calculated when SetVPMatrices executed
	enum FrustumPlanes
	{
		FP_LEFT = 0,
		FP_RIGHT,
		FP_TOP,
		FP_BOTTOM,
		FP_NEAR,
		FP_FAR,

		MAX_PLANES
	};

	mutable Primitives::Plane	mFrustum[ MAX_PLANES ];

	//! Use left-handed projection matrix.
	bool				mUseLH;

public:
	//! Constructs default viewer whose view matrix is identity.
	Viewer														( ) : mPosition( 0, 0, -1 ), mRotation( 0 ), mFOV( Math::real_pi * 0.5f ), mNear( 0.1f ), mFar( 100.0f ), mUseLH( true ) { };
	~Viewer														( ) { };

	//! Get viewer position.
	const Vec3&							Position				( ) const { return mPosition; };
	//! Get viewer rotation.
	const Euler&						Rotation				( ) const { return mRotation; };
	//! Get viewer FOV.
	const float&						FOV						( ) const { return mFOV; };
	//! Get viewer near plane.
	const float&						NearPlane				( ) const { return mNear; };
	//! Get viewer far plane.
	const float&						FarPlane				( ) const { return mFar; };
	//! Get use of left-handed projection matrix.
	const bool&							IsLH					( ) const { return mUseLH; };

	//! Get/Set viewer position.
	Vec3&								Position				( ) { return mPosition; };
	//! Get/Set viewer rotation.
	Euler&								Rotation				( ) { return mRotation; };
	//! Get/Set viewer FOV.
	float&								FOV						( ) { return mFOV; };
	//! Get/Set viewer near plane.
	float&								NearPlane				( ) { return mNear; };
	//! Get/Set viewer far plane.
	float&								FarPlane				( ) { return mFar; };
	//! Get/Set use o left-handed projection matrix.
	bool&								IsLH					( ) { return mUseLH; };

	/*! /brief Calculate and cache view and projection matrices, based on attributes.
	 *
	 *	Calculate and cache view and projection matrices, based on attributes.
	 *	This should be done before DIP call in order to setup Direct3D viewer->
	 *
	 *	/param set_vpmatrices		Set prepared matrices directly by invoking Renderer::SetTransform.
	 */
	void								SetVPMatrices			( bool set_vpmatrices = true ) const;

	//! Setups frustum from current view and proj matrices
	void								SetupFrustum			( );

	//! Performs frustum culling in view space with sphere.
	bool								TestFrustumCulling		( const Primitives::Sphere& sphere ) const;

	//! Performs frustum culling in view space with AABB.
	bool								TestFrustumCulling		( const Primitives::AABB& box ) const;

	//! Performs frustum culling in view space with OBB.
	bool								TestFrustumCulling		( const Primitives::OBB& box ) const;
};




_XFX_END

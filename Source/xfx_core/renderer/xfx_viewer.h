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





/*! \class Viewer xfx_viewer.h "main/xfx_viewer.h"
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
	//! Viewer position.
	Vec3				mPosition;

	//! Viewer rotation.
	Euler				mRotation;

	//! Viewer FOV in radians.
	float				mFOV;

	//! Near clipping plane.
	float				mNear;

	//! Far clipping plane.
	float				mFar;

	// Frustum planes
	// Note: Calculated in SetVPMatrices
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

	Mat4				mView;
	Mat4				mProj;
	Primitives::Plane	mFrustum[ MAX_PLANES ];

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

	//! Set viewer position.
	void								SetPosition				( const Vec3& p ) { mPosition = p; };
	//! Set viewer rotation.
	void								SetRotation				( const Euler& r ) { mRotation = r; };
	//! Set viewer FOV.
	void								SetFOV					( const float& f ) { mFOV = f; };
	//! Set viewer near plane.
	void								SetNearPlane			( const float& n ) { mNear = n; };
	//! Set viewer far plane.
	void								SetFarPlane				( const float& f ) { mFar = f; };
	//! Set use o left-handed projection matrix.
	void								SetLH					( const bool& lh ) { mUseLH = lh; };

	/*! /brief Calculate and cache view and projection matrices, based on attributes.
	 *
	 *	Calculate and cache view and projection matrices, based on attributes.
	 *	This should be done before DIP call in order to setup Direct3D viewer.
	 *
	 *	/param set_vpmatrices Set prepared matrices directly by invoking Renderer::SetTransform.
	 */
	void								SetVPMatrices			( bool set_vpmatrices = true );

	//! Setups frustum from view and proj matrices.
	void								SetupFrustum			( const Mat4& view_proj );

	//! Get view matrix.
	const Mat4&							GetView					( ) const { return mView; };
	
	//! Get projection matrix.
	const Mat4&							GetProjection			( ) const { return mProj; };

	//! Performs frustum culling in view space with sphere.
	bool								TestFrustumCulling		( const Primitives::Sphere& sphere ) const;

	//! Performs frustum culling in view space with AABB.
	bool								TestFrustumCulling		( const Primitives::AABB& box ) const;

	//! Performs frustum culling in view space with OBB.
	bool								TestFrustumCulling		( const Primitives::OBB& box ) const;
};




_XFX_END

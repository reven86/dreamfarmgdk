//  File xfx_viewer.cpp                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_viewer.h"
#include "xfx_renderer.h"
#include "xfx_renderer_cvars.h"

_XFX_BEGIN





//
// Viewer
//

void Viewer::SetVPMatrices( bool set_vpmatrices )
{
#if( __XFX_DIRECTX_VER__ < 9 )
	D3DVIEWPORT8 viewport;
#else
	D3DVIEWPORT9 viewport;
#endif
	Renderer::Instance( ).GetViewport( viewport );

	Vec3 dir, up, right;
	Rotation( ).AngleDirections( dir, up, right );

	if( mUseLH )
	{
		mProj.PerspectiveFovLH( FOV( ), static_cast< float >( viewport.Width ) / viewport.Height, NearPlane( ), FarPlane( ) );
		mView.LookAtLH( Position( ), dir, up );
	}
	else
	{
		mProj.PerspectiveFovRH( FOV( ), static_cast< float >( viewport.Width ) / viewport.Height, NearPlane( ), FarPlane( ) );
		mView.LookAtRH( Position( ), dir, up );
	}

	if( set_vpmatrices )
	{
		Renderer::Instance( ).SetTransform( D3DTS_PROJECTION, mProj );
		Renderer::Instance( ).SetTransform( D3DTS_VIEW, mView );
	}

	Mat4 f = mView * mProj;

	mFrustum[ FP_LEFT ].ABCD		(f.x[0][3] + f.x[0][0], f.x[1][3] + f.x[1][0], f.x[2][3] + f.x[2][0], f.x[3][3] + f.x[3][0]);
	mFrustum[ FP_RIGHT ].ABCD		(f.x[0][3] - f.x[0][0], f.x[1][3] - f.x[1][0], f.x[2][3] - f.x[2][0], f.x[3][3] - f.x[3][0]);
	mFrustum[ FP_BOTTOM ].ABCD		(f.x[0][3] + f.x[0][1], f.x[1][3] + f.x[1][1], f.x[2][3] + f.x[2][1], f.x[3][3] + f.x[3][1]);
	mFrustum[ FP_TOP ].ABCD			(f.x[0][3] - f.x[0][1], f.x[1][3] - f.x[1][1], f.x[2][3] - f.x[2][1], f.x[3][3] - f.x[3][1]);
	mFrustum[ FP_NEAR ].ABCD		(f.x[0][2], f.x[1][2], f.x[2][2], f.x[3][2]);
	mFrustum[ FP_FAR ].ABCD			(f.x[0][3] - f.x[0][2], f.x[1][3] - f.x[1][2], f.x[2][3] - f.x[2][2], f.x[3][3] - f.x[3][2]);
}

void Viewer::SetupFrustum( const Mat4& view_proj )
{
	const Mat4& f = view_proj;

	mFrustum[ FP_LEFT ].ABCD		(f.x[0][3] + f.x[0][0], f.x[1][3] + f.x[1][0], f.x[2][3] + f.x[2][0], f.x[3][3] + f.x[3][0]);
	mFrustum[ FP_RIGHT ].ABCD		(f.x[0][3] - f.x[0][0], f.x[1][3] - f.x[1][0], f.x[2][3] - f.x[2][0], f.x[3][3] - f.x[3][0]);
	mFrustum[ FP_BOTTOM ].ABCD		(f.x[0][3] + f.x[0][1], f.x[1][3] + f.x[1][1], f.x[2][3] + f.x[2][1], f.x[3][3] + f.x[3][1]);
	mFrustum[ FP_TOP ].ABCD			(f.x[0][3] - f.x[0][1], f.x[1][3] - f.x[1][1], f.x[2][3] - f.x[2][1], f.x[3][3] - f.x[3][1]);
	mFrustum[ FP_NEAR ].ABCD		(f.x[0][2], f.x[1][2], f.x[2][2], f.x[3][2]);
	mFrustum[ FP_FAR ].ABCD			(f.x[0][3] - f.x[0][2], f.x[1][3] - f.x[1][2], f.x[2][3] - f.x[2][2], f.x[3][3] - f.x[3][2]);
}

bool Viewer::TestFrustumCulling( const Primitives::Sphere& sphere ) const
{
	if( r_debug_frustumculling->AsInt( ) )
		Renderer::Instance( ).DrawTools( ).PushDrawSphere( sphere.Position( ), sphere.Radius( ), 0xff00ffff );

	for( int i = 0; i < 6; i++ )
	{
		float dist = Vec3::Dot( mFrustum[ i ].Normal( ), sphere.Position( ) ) + mFrustum[ i ].D( );

		if( dist < -sphere.Radius( ) * mFrustum[ i ].Normal( ).Len( ) )
			return false;
	}

	return true;
}

bool Viewer::TestFrustumCulling( const Primitives::AABB& box ) const
{
	if( r_debug_frustumculling->AsInt( ) )
		Renderer::Instance( ).DrawTools( ).PushDrawBox( box, 0xff00ffff );

	const Vec3 verts[ 8 ] =
	{
		box.Min( ),
		Vec3( box.Min( ).x, box.Min( ).y, box.Max( ).z ),
		Vec3( box.Min( ).x, box.Max( ).y, box.Min( ).z ),
		Vec3( box.Max( ).x, box.Min( ).y, box.Min( ).z ),
		Vec3( box.Min( ).x, box.Max( ).y, box.Max( ).z ),
		Vec3( box.Max( ).x, box.Max( ).y, box.Min( ).z ),
		Vec3( box.Max( ).x, box.Min( ).y, box.Max( ).z ),
		box.Max( )
	};

	for( int i = 0; i < 6; i++ )
	{
		int incount = 8;

		for( int v = 0; v < 8; v++ )
			if( Vec3::Dot( mFrustum[ i ].Normal( ), verts[ v ] ) + mFrustum[ i ].D( ) < 0.0f )
				--incount;
			else
				break;

		if( incount == 0 )
			return false;
	}

	return true;
}

bool Viewer::TestFrustumCulling( const Primitives::OBB& box ) const
{
	if( r_debug_frustumculling->AsInt( ) )
		Renderer::Instance( ).DrawTools( ).PushDrawBox( box, 0xff00ffff );

	const Vec3 verts[ 8 ] =
	{
		box.Location( ),
		box.Location( ) + box.E1( ),
		box.Location( ) + box.E2( ),
		box.Location( ) + box.E3( ),
		box.Location( ) + box.E1( ) + box.E2( ),
		box.Location( ) + box.E2( ) + box.E3( ),
		box.Location( ) + box.E3( ) + box.E1( ),
		box.Location( ) + box.E3( ) + box.E2( ) + box.E1( ),
	};

	for( int i = 0; i < 6; i++ )
	{
		int incount = 8;

		for( int v = 0; v < 8; v++ )
			if( Vec3::Dot( mFrustum[ i ].Normal( ), verts[ v ] ) + mFrustum[ i ].D( ) < 0.0f )
				--incount;
			else
				break;

		if( incount == 0 )
			return false;
	}

	return true;
}



_XFX_END
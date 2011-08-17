//  File xfx_viewport.cpp                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_viewport.h"
#include "xfx_renderer.h"
#include "xfx_renderer_cvars.h"

_XFX_BEGIN






//
// Viewport
//

Viewport::Viewport( )
{
	Init( 800, 600 );
}

Viewport::~Viewport( )
{
}

void Viewport::Init( unsigned width, unsigned height )
{
	mWidth		= width;
	mHeight		= height;
	mInvWidth	= 1.0f / mWidth;
	mInvHeight	= 1.0f / mHeight;

	mViewportUpdateSignals( );
}

void Viewport::SetWidth( const unsigned& width )
{
	mWidth		= width;
	mInvWidth	= 1.0f / width;

	mViewportUpdateSignals( );
}

void Viewport::SetHeight( const unsigned& height )
{
	mHeight		= height;
	mInvHeight	= 1.0f / height;

	mViewportUpdateSignals( );
}

void Viewport::Render( ) const
{
	PROFILE( __FUNCTION__, "Render" );

	XFX_PLACE_DEVICE_LOCK;

	Renderer::Instance( ).BeginFrame( );

	DWORD oldwr = 0;
	if( r_wireframe->AsInt( ) )
	{
		oldwr = Renderer::Instance( ).State( Renderer::SM_FILLMODE );
		Renderer::Instance( ).State( Renderer::SM_FILLMODE, D3DFILL_WIREFRAME );
	}

	gGetApplication( ).Render( );

	if( r_wireframe->AsInt( ) )
	{
		Renderer::Instance( ).State( Renderer::SM_FILLMODE, oldwr );
	}

	Renderer::Instance( ).EndFrame( );
}

void Viewport::Project( float& x, float& y, const Vec3& p ) const
{
	Mat4 world;
	Renderer::Instance( ).GetTransform( D3DTS_WORLD, world );

	Vec3 out;

	out = ( world * Renderer::Instance( ).GetVP( ) ).TransformCoord( p );

	float k = 1.0f / out.z;

	x = out.x * k;
	y = out.y * k;
}

void Viewport::Unproject( Vec3& out, float x, float y ) const
{
	Mat4 view;
	Mat4 proj;
	Mat4 world;

	Renderer::Instance( ).GetTransform( D3DTS_PROJECTION, proj );
	Renderer::Instance( ).GetTransform( D3DTS_VIEW, view );
	Renderer::Instance( ).GetTransform( D3DTS_WORLD, world );

	out = ( view * world ).GetInversedAffine( ).TransformNormal(
		Vec3( x / proj.x[ 0 ][ 0 ], y / proj.x[ 1 ][ 1 ], 1.0f ) );
}

boost::tuple< float, float > Viewport::MapViewportToRenderer( float x, float y ) const
{
	return boost::make_tuple( x * mInvWidth * Renderer::Instance( ).D3DPP( ).BackBufferWidth, y * mInvHeight * Renderer::Instance( ).D3DPP( ).BackBufferHeight );
}

boost::tuple< float, float > Viewport::MapRendererToViewport( float x, float y ) const
{
	return boost::make_tuple( x * mWidth / Renderer::Instance( ).D3DPP( ).BackBufferWidth, y * mHeight / Renderer::Instance( ).D3DPP( ).BackBufferHeight );
}


_XFX_END
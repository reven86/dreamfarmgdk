//  File xfx_rendertargetgroup.cpp                                            |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_rendertargetgroup.h"
#include "xfx_texture.h"
#include "xfx_depthstencil.h"
#include "xfx_renderer.h"





_XFX_BEGIN

RenderTargetGroup::RenderTargetGroup( ) : 
	mClearFlags( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ),
	mClearColor( 0x00000000 ),
	mClearDepth( 1.0f ),
	mClearStencil( 0 )
{
}

RenderTargetGroup::~RenderTargetGroup( )
{
}

HRESULT RenderTargetGroup::SetRenderedTexture( const boost::shared_ptr< RenderedTexture >& tex )
{
	HRESULT hr;
	if( FAILED( hr = ValidateDepthStencilMatch( tex.get( ), mDepthStencilPtr.get( ) ) ) )
		return hr;

	mRenderedTexturePtr = tex;
	return S_OK;
}

HRESULT RenderTargetGroup::SetDepthStencil( const boost::shared_ptr< DepthStencil >& ds )
{
	HRESULT hr;
	if( FAILED( hr = ValidateDepthStencilMatch( mRenderedTexturePtr.get( ), ds.get( ) ) ) )
		return hr;

	mDepthStencilPtr = ds;
	return S_OK;
}

HRESULT RenderTargetGroup::CreateCompatibleDepthStencil( )
{
	if( !mRenderedTexturePtr || mRenderedTexturePtr->IsEmpty( ) )
		return XFXERR_INVALIDCALL;

	if( mDepthStencilPtr &&
		SUCCEEDED( ValidateDepthStencilMatch( mRenderedTexturePtr.get( ), mDepthStencilPtr.get( ) ) ) )
		return S_OK;

	HRESULT hr;

	D3DFORMAT fmts[ ] = { D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D24X8, D3DFMT_D16 };

	D3DSURFACE_DESC sd;
#if __XFX_DIRECTX_VER__ < 9
	if( FAILED( hr = ( ( LPDIRECT3DTEXTURE8 ) mRenderedTexturePtr->GetD3DTex( ) )->GetLevelDesc( 0, &sd ) ) )
#else
	if( FAILED( hr = ( ( LPDIRECT3DTEXTURE9 ) mRenderedTexturePtr->GetD3DTex( ) )->GetLevelDesc( 0, &sd ) ) )
#endif
		return hr;

	D3DDEVICE_CREATION_PARAMETERS dcp;
	xfx::Renderer::Instance( ).pD3DDevice( )->GetCreationParameters( &dcp );

	for( int i = 0, max_i = sizeof( fmts ) / sizeof( D3DFORMAT ); i < max_i; i++ )
	{
		if( SUCCEEDED( hr = xfx::Renderer::Instance( ).pD3D( )->CheckDeviceFormat(
				D3DADAPTER_DEFAULT,
				dcp.DeviceType,
				xfx::Renderer::Instance( ).D3DPP( ).BackBufferFormat,
				D3DUSAGE_DEPTHSTENCIL,
				D3DRTYPE_SURFACE,
				fmts[ i ] ) ) &&
			SUCCEEDED( hr = xfx::Renderer::Instance( ).pD3D( )->CheckDepthStencilMatch (
				D3DADAPTER_DEFAULT,
				dcp.DeviceType,
				xfx::Renderer::Instance( ).D3DPP( ).BackBufferFormat,
				sd.Format,
				fmts[ i ] ) ) )
		{
			mDepthStencilPtr.reset( new DepthStencil( ) );
			return mDepthStencilPtr->Create(
				mRenderedTexturePtr->GetWidth( ),
				mRenderedTexturePtr->GetHeight( ),
				fmts[ i ] );
		}
	}

	return hr;
}

HRESULT RenderTargetGroup::ValidateDepthStencilMatch( RenderedTexture * tex, DepthStencil * ds )
{
	if( !tex || tex->IsEmpty( ) )
		return XFXERR_INVALIDCALL;

	if( !ds || ds->IsEmpty( ) )
		return S_OK;

	HRESULT hr;

	D3DSURFACE_DESC sd;
#if __XFX_DIRECTX_VER__ < 9
	if( FAILED( hr = ( ( LPDIRECT3DTEXTURE8 ) tex->GetD3DTex( ) )->GetLevelDesc( 0, &sd ) ) )
#else
	if( FAILED( hr = ( ( LPDIRECT3DTEXTURE9 ) tex->GetD3DTex( ) )->GetLevelDesc( 0, &sd ) ) )
#endif
		return hr;

	D3DSURFACE_DESC ds_sd;
	if( FAILED( hr = ds->GetD3DSurface( )->GetDesc( &ds_sd ) ) )
		return hr;

	D3DDEVICE_CREATION_PARAMETERS dcp;
	xfx::Renderer::Instance( ).pD3DDevice( )->GetCreationParameters( &dcp );

	if( FAILED( hr = xfx::Renderer::Instance( ).pD3D( )->CheckDeviceFormat(
			D3DADAPTER_DEFAULT,
			dcp.DeviceType,
			xfx::Renderer::Instance( ).D3DPP( ).BackBufferFormat,
			D3DUSAGE_DEPTHSTENCIL,
			D3DRTYPE_SURFACE,
			ds_sd.Format ) ) ||
		FAILED( hr = xfx::Renderer::Instance( ).pD3D( )->CheckDepthStencilMatch (
			D3DADAPTER_DEFAULT,
			dcp.DeviceType,
			xfx::Renderer::Instance( ).D3DPP( ).BackBufferFormat,
			sd.Format,
			ds_sd.Format ) ) )
	{
		return hr;
	}

	return S_OK;
}

HRESULT RenderTargetGroup::BeginUsing( bool push_state )
{
	if( !mRenderedTexturePtr || mRenderedTexturePtr->IsEmpty( ) )
		return XFXERR_INVALIDCALL;

	HRESULT hr;

	if( push_state )
	{
#if (__XFX_DIRECTX_VER__ < 9)
		LPDIRECT3DSURFACE8 saved_color;
		LPDIRECT3DSURFACE8 saved_depth;
#else
		LPDIRECT3DSURFACE9 saved_color;
		LPDIRECT3DSURFACE9 saved_depth;
#endif

#if (__XFX_DIRECTX_VER__ < 9)
		if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->GetRenderTarget( &saved_color ) ) )
#else
		if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->GetRenderTarget( 0, &saved_color ) ) )
#endif
			return hr;

		mpSavedColor.reset( saved_color, IUnknownDeleter( ) );

		if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->GetDepthStencilSurface( &saved_depth ) ) )
			return hr;

		mpSavedDepth.reset( saved_depth, IUnknownDeleter( ) );
	}

	xfx::Renderer::Instance( ).GetDrawTools( ).FlushAll( );

#if (__XFX_DIRECTX_VER__ < 9)
	if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->SetRenderTarget( mRenderedTexturePtr->GetD3DSurface( ), mDepthStencilPtr ? mDepthStencilPtr->GetD3DSurface( ) : NULL ) ) )
		return hr;
#else
	if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->SetRenderTarget( 0, mRenderedTexturePtr->GetD3DSurface( ) ) ) )
		return hr;

	if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->SetDepthStencilSurface( mDepthStencilPtr ? mDepthStencilPtr->GetD3DSurface( ) : NULL ) ) )
		return hr;
#endif

	if( GetClearFlags( ) )
		xfx::Renderer::Instance( ).pD3DDevice( )->Clear( 0, NULL, mDepthStencilPtr && !mDepthStencilPtr->IsEmpty( ) ? GetClearFlags( ) : GetClearFlags( ) & ~D3DCLEAR_ZBUFFER, GetClearColor( ).dword, GetClearDepth( ), GetClearStencil( ) );

	return S_OK;
}

void RenderTargetGroup::EndUsing( bool pop_state )
{
	_ASSERTE( IsActive( ) );
	if( !IsActive( ) )
		return;

	HRESULT hr;

	xfx::Renderer::Instance( ).GetDrawTools( ).FlushAll( );

	if( pop_state )
	{
#if (__XFX_DIRECTX_VER__ < 9)
		if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->SetRenderTarget( mpSavedColor.get( ), mpSavedDepth.get( ) ) ) )
			return;
#else
		if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->SetRenderTarget( 0, mpSavedColor.get( ) ) ) )
			return;

		if( FAILED( hr = xfx::Renderer::Instance( ).pD3DDevice( )->SetDepthStencilSurface( mpSavedDepth.get( ) ) ) )
			return;
#endif

		mpSavedColor.reset( );
		mpSavedDepth.reset( );
	}
}

bool RenderTargetGroup::IsActive( ) const
{
	return mpSavedColor || mpSavedDepth;
}



_XFX_END
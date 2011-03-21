//  File xfx_depthstencil.cpp                                                 |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_depthstencil.h"
#include "xfx_renderer.h"





_XFX_BEGIN



DepthStencil::DepthStencil( ) :
	mWidth( 1 ), mHeight( 1 ), mpSurface( )
{
}

DepthStencil::~DepthStencil( )
{
}

HRESULT DepthStencil::Create( unsigned width, unsigned height, const D3DFORMAT& fmt )
{
	Free ();

	mWidth		= width;
	mHeight		= height;

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DSURFACE8 tex;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateDepthStencilSurface(
		mWidth, mHeight, fmt, D3DMULTISAMPLE_NONE, &tex);
#else
	LPDIRECT3DSURFACE9 tex;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateDepthStencilSurface(
		mWidth, mHeight, fmt, D3DMULTISAMPLE_NONE, 0, true, &tex, NULL);
#endif

	if (SUCCEEDED (hr))
		mpSurface.reset (tex, IUnknownDeleter ());

	return S_OK;
}

void DepthStencil::Free( )
{
	mWidth = mHeight = 1;
	mpSurface.reset( );
}




_XFX_END
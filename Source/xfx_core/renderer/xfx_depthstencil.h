//  File xfx_depthstencil.h                                                   |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_depthstencil.h 
 *	\brief Depth stencil surface.
 */

_XFX_BEGIN




/*! \class DepthStencil xfx_depthstencil.h "render/xfx_depthstencil.h"
 *	\brief Depth/stencil surface.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class DepthStencil : boost::noncopyable
{
#if (__XFX_DIRECTX_VER__ < 9)
	boost::shared_ptr< IDirect3DSurface8 >		mpSurface;
#else
	boost::shared_ptr< IDirect3DSurface9 >		mpSurface;
#endif

	unsigned				mWidth;				//surface width
	unsigned				mHeight;			//surface height

public:
	DepthStencil				( );
	virtual ~DepthStencil		( );

	//! Get width.
	unsigned						GetWidth					( ) const { return mWidth; };

	//! Get height.
	unsigned						GetHeight					( ) const { return mHeight; };

	/*! \brief Create depth\stencil surface.
	 *
	 *	\param[in]	width		Surface width.
	 *	\param[in]	height		Surface height.
	 *	\param[in]	fmt			Surface format (one of D32, D24S8, D24X8, D16).
	 *
	 *	\return S_OK if succeeded.
	 */
	HRESULT							Create						( unsigned width, unsigned height, const D3DFORMAT& fmt = D3DFMT_D24S8 );

	//! Destroy all allocated resources for texture.
	void							Free						( );

	//! Is texture empty.
	bool							IsEmpty						( ) const { return !mpSurface; };

#if (__XFX_DIRECTX_VER__ < 9)
	//! Get ID3DSurface.
	LPDIRECT3DSURFACE8				GetD3DSurface				( ) const { return mpSurface.get( ); };
#else
	//! Get ID3DSurface.
	LPDIRECT3DSURFACE9				GetD3DSurface				( ) const { return mpSurface.get( ); };
#endif
};



_XFX_END
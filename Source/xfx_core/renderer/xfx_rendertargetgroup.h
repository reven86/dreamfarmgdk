//  File xfx_rendertargetgroup.h                                              |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_rendertargetgroup.h 
 *	\brief Render target group representation.
 */

_XFX_BEGIN




/*! \class RenderTargetGroup xfx_rendertargetgroup.h "render/xfx_rendertargetgroup.h"
 *	\brief Render target group.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class RenderTargetGroup : boost::noncopyable
{
	boost::shared_ptr< class RenderedTexture >	mRenderedTexturePtr;
	boost::shared_ptr< class DepthStencil >		mDepthStencilPtr;

#if (__XFX_DIRECTX_VER__ < 9)
	boost::shared_ptr< IDirect3DSurface8 >		mpSavedColor;
	boost::shared_ptr< IDirect3DSurface8 >		mpSavedDepth;
#else
	boost::shared_ptr< IDirect3DSurface9 >		mpSavedColor;
	boost::shared_ptr< IDirect3DSurface9 >		mpSavedDepth;
#endif

	DWORD	mClearFlags;
	ARGB	mClearColor;
	float	mClearDepth;
	DWORD	mClearStencil;

public:
	//! Creates empty render target group.
	RenderTargetGroup ( );

	virtual ~RenderTargetGroup ( );

	//
	// Clear parameters.
	//

	//! Get clear flags.
	const DWORD& GetClearFlags( ) const { return mClearFlags; };

	//! Set clear flags.
	void SetClearFlags( const DWORD& fl ) { mClearFlags = fl; };

	//! Get clear color.
	const ARGB& GetClearColor( ) const { return mClearColor; };

	//! Set clear color.
	void SetClearColor( const ARGB& c ) { mClearColor = c; };

	//! Get clear depth value.
	const float& GetClearDepth( ) const { return mClearDepth; };

	//! Set clear depth value.
	void SetClearDepth( const float& d ) { mClearDepth = d; };

	//! Get clear stencil value.
	const DWORD& GetClearStencil( ) const { return mClearStencil; };

	//! Set clear stencil value.
	void SetClearStencil( const DWORD& v ) { mClearStencil = v; };

	//
	// Activation/Deactivation methods.
	//

	/*! \brief Begin using render target group.
	 *
	 *	The current active render target group is saved and replaced by this one,
	 *	then the Device::Clear method is called with proper clear parameters.
	 *
	 *	\return S_OK if render target group was successfully set.
	 */
	HRESULT BeginUsing( );

	//! End using render target group.
	void EndUsing( );

	//! Is render target group active.
	bool IsActive( ) const;

	//
	// Texture, Depth/Stencil methods.
	//

	//! Assign rendered texture and validate with depth-stencil.
	HRESULT SetRenderedTexture( const boost::shared_ptr< RenderedTexture >& tex );

	//! Assign depth-stencil surface and validate it with rendered texture.
	HRESULT SetDepthStencil( const boost::shared_ptr< DepthStencil >& ds );

	//! Create compatible depth-stencil surface and assign it.
	HRESULT CreateCompatibleDepthStencil( );

	//! Get rendered texture.
	const boost::shared_ptr< RenderedTexture >& GetRenderedTexture( ) const { return mRenderedTexturePtr; };

	//! Get depth-stencil surface.
	const boost::shared_ptr< DepthStencil >& GetDepthStencil( ) const { return mDepthStencilPtr; };

protected:
	//! Validate rendered texture and depth-stencil surface.
	HRESULT ValidateDepthStencilMatch( RenderedTexture * tex, DepthStencil * ds );
};




_XFX_END

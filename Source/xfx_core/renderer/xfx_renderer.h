//  File xfx_renderer.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_drawtools.h"

/*!	\file xfx_renderer.h 
 *	\brief %Renderer class.
 */


_XFX_BEGIN





/*! \defgroup RenderGroup Render-related classes.
 */

interface ITexture;

/*! \class Renderer xfx_renderer.h "renderer/xfx_renderer.h"
 *	\brief Common renderer class, based on D3D.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Renderer : public Singleton< Renderer >
{
	friend class Singleton< Renderer >;

public:

	//! %Render states (same as D3DRENDERSTATES).
	enum STATES
	{
		SM_ZENABLE = 0,			//Don't add anything before
		SM_FILLMODE,
		SM_SHADEMODE,
		SM_ZWRITEENABLE,
		SM_ALPHATESTENABLE,
		SM_LASTPIXEL,
		SM_SRCBLEND,
		SM_DESTBLEND,
		SM_CULLMODE,
		SM_ZFUNC,
		SM_ALPHAREF,
		SM_ALPHAFUNC,
		SM_DITHERENABLE,
		SM_ALPHABLENDENABLE,
		SM_FOGENABLE,
		SM_SPECULARENABLE,
		SM_FOGCOLOR,
		SM_FOGTABLEMODE,
		SM_FOGSTART,
		SM_FOGEND,
		SM_FOGDENSITY,
		SM_RANGEFOGENABLE,
		SM_STENCILENABLE,
		SM_STENCILFAIL,
		SM_STENCILZFAIL,
		SM_STENCILPASS,
		SM_STENCILFUNC,
		SM_STENCILREF,
		SM_STENCILMASK,
		SM_STENCILWRITEMASK,
		SM_TEXTUREFACTOR,
		SM_WRAP0,
		SM_WRAP1,
		SM_WRAP2,
		SM_WRAP3,
		SM_WRAP4,
		SM_WRAP5,
		SM_WRAP6,
		SM_WRAP7,
		SM_CLIPPING,
		SM_LIGHTING,
		SM_AMBIENT,
		SM_FOGVERTEXMODE,
		SM_COLORVERTEX,
		SM_LOCALVIEWER,
		SM_NORMALIZENORMALS,
		SM_DIFFUSEMATERIALSOURCE,
		SM_SPECULARMATERIALSOURCE,
		SM_AMBIENTMATERIALSOURCE,
		SM_EMISSIVEMATERIALSOURCE,
		SM_VERTEXBLEND,
		SM_CLIPPLANEENABLE,
		SM_POINTSIZE,
		SM_POINTSIZE_MIN,
		SM_POINTSPRITEENABLE,
		SM_POINTSCALEENABLE,
		SM_POINTSCALE_A,
		SM_POINTSCALE_B,
		SM_POINTSCALE_C,
		SM_MULTISAMPLEANTIALIAS,
		SM_MULTISAMPLEMASK,
		SM_PATHEDEGESTYLE,
		SM_DEBigMONITORTOKEN,
		SM_POINTSIZE_MAX,
		SM_INDEXEDVERTEXBLENDENABLE,
		SM_COLORWRITEENABLE,
		SM_TWEENFACTOR,
		SM_BLENDOP,
		SM_POSITIONDEGREE,
		SM_NORMALDEGREE,
		SM_SCISSORTESTENABLE,
		SM_SLOPESCALEDEPTHBIAS,
		SM_ANTIALIASEDLINEENABLE,
		SM_MINTESSELLATIONLEVEL,
		SM_MAXTESSELLATIONLEVEL,
		SM_ADAPTIVETESS_X,
		SM_ADAPTIVETESS_Y,
		SM_ADAPTIVETESS_Z,
		SM_ADAPTIVETESS_W,
		SM_ENABLEADAPTIVETESSELLATION,
		SM_TWOSIDEDSTENCILMODE,
		SM_CCW_STENCILFAIL,
		SM_CCW_STENCILZFAIL,
		SM_CCW_STENCILPASS,
		SM_CCW_STENCILFUNC,
		SM_COLORWRITEENABLE1,
		SM_COLORWRITEENABLE2,
		SM_COLORWRITEENABLE3,
		SM_BLENDFACTOR,
		SM_SRGBWRITEENABLE,
		SM_DEPTHBIAS,
		SM_WRAP8,
		SM_WRAP9,
		SM_WRAP10,
		SM_WRAP11,
		SM_WRAP12,
		SM_WRAP13,
		SM_WRAP14,
		SM_WRAP15,
		SM_SEPARATEALPHABLENDENABLE,
		SM_SRCBLENDALPHA,
		SM_DESTBLENDALPHA,
		SM_BLENDOPALPHA,

		SM_MAX_STATES,

		SM_FORCE_DWORD = 0x7fffffff
	};

	//! %Texture states (same as D3DTEXTURESTATE).
	enum TEXTURE_STATES
	{
		SM_TEX_COLOROP = 0,			//Don't add anything before
		SM_TEX_COLORARG1,
		SM_TEX_COLORARG2,
		SM_TEX_ALPHAOP,
		SM_TEX_ALPHAARG1,
		SM_TEX_ALPHAARG2,
		SM_TEX_BUMPENVMAT00,
		SM_TEX_BUMPENVMAT01,
		SM_TEX_BUMPENVMAT10,
		SM_TEX_BUMPENVMAT11,
		SM_TEX_TEXCOORDINDEX,
		SM_TEX_BUMPENVLSCALE,
		SM_TEX_BUMPENVLOFFSET,
		SM_TEX_TEXTURETRANSFORMFLAGS,
		SM_TEX_COLORARG0,
		SM_TEX_ALPHAARG0,
		SM_TEX_RESULTARG,
		SM_TEX_CONSTANT,

		SM_TEX_MAX_STATES,

		SM_TEX_FORCE_DWORD = 0x7fffffff
	};

	//! %Sampler states (same as D3DSAMPLERSTATE).
	enum SAMPLER_STATES
	{
		SM_SAMP_ADDRESSU = 0,			//Don't add anything before
		SM_SAMP_ADDRESSV,
		SM_SAMP_ADDRESSW,
		SM_SAMP_BORDERCOLOR,
		SM_SAMP_MAGFILTER,
		SM_SAMP_MINFILTER,
		SM_SAMP_MIPFILTER,
		SM_SAMP_MIPMAPLODBIAS,
		SM_SAMP_MAXMIPLEVEL,
		SM_SAMP_MAXANISOTROPY,
		SM_SAMP_SRGBTEXTURE,
		SM_SAMP_ELEMENTINDEX,
		SM_SAMP_DMAPOFFSET,

		SM_SAMP_MAX_STATES,

		SM_SAMP_FORCE_DWORD = 0x7fffffff
	};

	//! Max supported texture stages.
	static const int MAX_TEXTURE_STAGES = 8;

	//! Frame statistics.
	struct FrameStatistics
	{
		unsigned	num_batches;			//!< Number of batches per frame.
		unsigned	num_tris;				//!< Number of triangles per frame.
		unsigned	num_lines;				//!< Number of lines per frame.
		unsigned	num_sprites;			//!< Number of sprites per frame.
		unsigned	num_usertris;			//!< Number of user triangles per frame.

		float		avg_lines;				//!< Average lines flush count.
		float		avg_sprites;			//!< Average sprites flush count.
		float		avg_usertris;			//!< Averate user tris flush count.

		float		avg_lines_disp;			//!< Average lines flush count dispersion.
		float		avg_sprites_disp;		//!< Average sprites flush count dispersion.
		float		avg_usertris_disp;		//!< Averate user tris flush count dispersion.

		//! Clear statistic.
		void		clear( )	{ ZeroMemory( this, sizeof( *this ) ); };
	};

private:

#if( __XFX_DIRECTX_VER__ < 9 )
	boost::shared_ptr< IDirect3D8 >			mpD3D;
	boost::shared_ptr< IDirect3DDevice8 >	mpD3DDevice;
	D3DCAPS8								mD3DCaps;
#else
	boost::shared_ptr< IDirect3D9 >			mpD3D;
	boost::shared_ptr< IDirect3DDevice9 >	mpD3DDevice;
	D3DCAPS9								mD3DCaps;
#endif

	D3DPRESENT_PARAMETERS			mD3DPP;
	DWORD							mClearFlags;

	FrameStatistics					mFrameStat;

	boost::shared_ptr< const ITexture >	mActiveTexturesPtr[ MAX_TEXTURE_STAGES ];

	static D3DRENDERSTATETYPE		stateD3D[ SM_MAX_STATES ];
	static DWORD					stateValue[ SM_MAX_STATES ];

	static D3DTEXTURESTAGESTATETYPE	stateTexD3D[ SM_TEX_MAX_STATES ];
	static DWORD					stateTexValue[ MAX_TEXTURE_STAGES ][ SM_TEX_MAX_STATES ];

#if( __XFX_DIRECTX_VER__ < 9 )
	static D3DTEXTURESTAGESTATETYPE	stateSampD3D[ SM_SAMP_MAX_STATES ];
#else
	static D3DSAMPLERSTATETYPE		stateSampD3D[ SM_SAMP_MAX_STATES ];
#endif
	static DWORD					stateSampValue[ MAX_TEXTURE_STAGES ][ SM_SAMP_MAX_STATES ];

	mutable bool							mIsVPCached;
	mutable Mat4							mCachedVP;

	// Draw tools
	DrawTools								mDrawTools;

	int											mCurrentFVF;
	boost::shared_ptr< const class Shader >		mLastShaderPtr;

#ifdef __XFX_ENABLE_MULTITHREADING__
	/*! \brief Device mutex. 
	 *
	 *	Applications must this mutex in any device operation in multithreading environment.
	 *	Recursive mutex is used to allow multiple locks by one thread.
	 */
	boost::recursive_try_mutex				mDeviceMutex;
#endif

	//! Fullscreen change events.
	boost::signal3< void, bool, unsigned, unsigned > mFullscreenChangeEvents;

	//! Invalidate callbacks.
	boost::signal1< void, bool >			mInvalidateCallbacks;

	//! Saved window style.
	LONG									mSavedWindowStyle;

	//! Saved window ex style.
	LONG									mSavedWindowExStyle;

	/*! \brief Default shader.
	 *
	 *	This shader is used primary as "default" for textures loaded without shader (e.g. for gui).
	 */
	boost::shared_ptr< const Shader >		mDefaultShader;

protected:
	Renderer														( );
	~Renderer														( );

public:
#if( __XFX_DIRECTX_VER__ < 9 )
	//! Get D3D Object.
	LPDIRECT3D8							pD3D						( ) const { return mpD3D.get( ); };

	//! Get D3D device.
	LPDIRECT3DDEVICE8					pD3DDevice					( ) const { return mpD3DDevice.get( ); };

	//! Get D3D caps.
	const D3DCAPS8&						D3DCaps						( ) const { return mD3DCaps; };
#else
	//! Get D3D Object.
	LPDIRECT3D9							pD3D						( ) const { return mpD3D.get( ); };

	//! Get D3D device.
	LPDIRECT3DDEVICE9					pD3DDevice					( ) const { return mpD3DDevice.get( );};

	//! Get D3D caps.
	const D3DCAPS9&						D3DCaps						( ) const { return mD3DCaps; };
#endif

	//! Get D3DPRESENT_PARAMETERS.
	const D3DPRESENT_PARAMETERS&		D3DPP						( ) const { return mD3DPP; };

#ifdef __XFX_ENABLE_MULTITHREADING__
	boost::recursive_try_mutex&			GetDeviceMutex				( ) { return mDeviceMutex; };
#endif

	//! Create device.
	HRESULT								CreateDevice				( HWND hwnd );

	//! Destroy device and free all allocated resources.
	void								Shutdown					( );

	//! Get/Set frame statistics.
	struct FrameStatistics&				FrameStatistics				( ) { return mFrameStat; };

	//! Get frame statistics.
	const struct FrameStatistics&		FrameStatistics				( ) const { return mFrameStat; };

	//! Draw per-frame statistics.
	void								DrawFrameStatistics			( const class Font& fnt, const boost::shared_ptr< const class Shader >& white_shader ) const;

	//! Start render frame ( automatically clear back buffer ). \see ClearFlags.
	void								BeginFrame					( );

	//! End render frame.
	void								EndFrame					( );

	//! Clear frame.
	void								Clear						( const DWORD& flags );

	//! Set clear flags for automatic clear.
	void								SetClearFlags				( DWORD fl ) { mClearFlags = fl; };

	//! Get clear flags for automatic clear.
	const DWORD&						ClearFlags					( ) const { return mClearFlags; };

#if( __XFX_DIRECTX_VER__ < 9 )
	//! Draw indexed primitive.
	void								DrawIndexedPrimitive		( D3DPRIMITIVETYPE type, UINT min_index, UINT num_verts, UINT start_index, UINT p_count ) { mpD3DDevice->DrawIndexedPrimitive (type, min_index, num_verts, start_index, p_count); mFrameStat.num_tris += p_count; mFrameStat.num_batches++; };
#else
	//! Draw indexed primitive.
	void								DrawIndexedPrimitive		( D3DPRIMITIVETYPE type, INT base_vertex_index, UINT min_index, UINT num_verts, UINT start_index, UINT p_count ) { mpD3DDevice->DrawIndexedPrimitive (type, base_vertex_index, min_index, num_verts, start_index, p_count); mFrameStat.num_tris += p_count; mFrameStat.num_batches++; };
#endif
	//! Draw primitive.
	void								DrawPrimitive				( D3DPRIMITIVETYPE type, UINT offset, UINT p_count ) { mpD3DDevice->DrawPrimitive( type, offset, p_count ); mFrameStat.num_tris += p_count; mFrameStat.num_batches++; };

	//! Get draw tools.
	xfx::DrawTools&						GetDrawTools				( ) { return mDrawTools; };

	//! Get draw tools.
	const xfx::DrawTools&				GetDrawTools				( ) const { return mDrawTools; };

	//
	//! \name State manager
	//

	//! @{

	//! Restore all states.
	void								RestoreStates				( );

	//! Get render state.
	const DWORD&						State						( STATES state ) const { _ASSERTE( state < SM_MAX_STATES ); return stateValue[ state ]; };

	//! Set render state.
	void								State						( STATES state, DWORD value );

	//! Get texture state.
	const DWORD&						TextureState				( unsigned stage, TEXTURE_STATES state ) const { _ASSERTE( stage < MAX_TEXTURE_STAGES ); _ASSERTE( state < SM_TEX_MAX_STATES ); return stateTexValue[ stage ][ state ]; };

	//! Set texture state.
	void								TextureState				( unsigned stage, TEXTURE_STATES state, DWORD value );	

	//! Get sampler state.
	const DWORD&						SamplerState				( unsigned stage, SAMPLER_STATES state ) const { _ASSERTE( stage < MAX_TEXTURE_STAGES ); _ASSERTE( state < SM_SAMP_MAX_STATES ); return stateSampValue[ stage ][ state ]; };

	//! Set sampler state.
	void								SamplerState				( unsigned stage, SAMPLER_STATES state, DWORD value );	

	//! Get FVF.
	const int&							FVF							( ) const { return mCurrentFVF; };

	//! Set FVF.
	void								FVF							( const int& fvf );

	//! @}

	//
	//! \name Shader system
	//

	//! @{

	//! Apply texture to stage.
	void								ApplyTexture				( const boost::shared_ptr< const ITexture >& tex, unsigned stage = 0 );

	//! Get current texture on stage.
	const boost::shared_ptr< const ITexture >& ActiveTexture		( unsigned stage = 0 ) const { _ASSERTE( stage < MAX_TEXTURE_STAGES ); return mActiveTexturesPtr[ stage ]; };

	//! %Render primitive.
	void								RenderPrimitive				( const boost::shared_ptr< const Shader >& shd, const class ShaderParams& consts, const boost::function0< void >& render_fn );

	/*! \brief Set fullscreen mode.
	 *
	 *	\param[in]	isfullscreen	Is fullscreen mode.
	 *	\param[in]	width			Window/Desktop width.
	 *	\param[in]	height			Window/Desktop height.
	 *	\param[in]	refreshrate		Refresh rate (only if \a isfullscree is \b true).
	 *	\param[in]	format			Back buffer format (only if \a isfullscree is \b true).
	 */
	HRESULT								Fullscreen					( bool isfullscreen, unsigned width, unsigned height, const unsigned * refreshrate = NULL, const D3DFORMAT * format = NULL );

	/*!	\brief Take and save screen shot.
	 *
	 *	Typically used to save one screen in high quality tga or frame sequences as jpegs.
	 *
	 *	\param[in]	filename		Screenshot file name.
	 *	\param[in]	save_jpeg		Save screenshot as jpeg.
	 *
	 *	\return S_OK if succeeded.
	 */
	HRESULT								TakeScreenshot				( const String& filename, bool save_jpeg = false ) const;

	//! Reset device with specific width and height.
	HRESULT								ResetDevice					( unsigned width, unsigned height );

	//! @}

	//
	//! \name Callbacks, events, subscribers
	//

	//! @{

	//! Add invalidate callbacks.
	template< class _CallbackType >
	boost::signals::connection			AddInvalidateCallback		( const _CallbackType& callback ) { return mInvalidateCallbacks.connect( callback ); };

	//! Add fullscreen change callback.
	template< class _CallbackType >
	boost::signals::connection			AddFullscreenChangeCallback	( const _CallbackType& callback ) { return mFullscreenChangeEvents.connect( callback ); };

	//! @}

	//
	//! \name Transform and viewport
	//

	//! @{

	//! Set and cache transform.
	void								SetTransform				( const D3DTRANSFORMSTATETYPE& transform, const Mat4& matr );

	//! Get transform.
	void								GetTransform				( const D3DTRANSFORMSTATETYPE& transform, Mat4& matr ) const { _ASSERTE( mpD3DDevice ); mpD3DDevice->GetTransform( transform, reinterpret_cast< D3DMATRIX * >( &matr ) ); };

#if( __XFX_DIRECTX_VER__ < 9 )
	//! Set viewport.
	void								SetViewport					( const D3DVIEWPORT8& viewport ) { /*DrawTools ().FlushAll (); */_ASSERTE( mpD3DDevice ); mpD3DDevice->SetViewport( &viewport ); };

	//! Get viewport.
	void								GetViewport					( D3DVIEWPORT8& viewport ) { _ASSERTE( mpD3DDevice ); mpD3DDevice->GetViewport( &viewport ); };
#else
	//! Set viewport.
	void								SetViewport					( const D3DVIEWPORT9& viewport ) { /*DrawTools ().FlushAll (); */_ASSERTE( mpD3DDevice ); mpD3DDevice->SetViewport( &viewport ); };

	//! Get viewport.
	void								GetViewport					( D3DVIEWPORT9& viewport ) { _ASSERTE( mpD3DDevice ); mpD3DDevice->GetViewport( &viewport ); };
#endif

	//! Get ViewProjection matrix.
	const Mat4&							GetVP						( ) const { if( !mIsVPCached ) CacheVP( ); return mCachedVP; };

	//! @}

	//! Get default shader.
	const boost::shared_ptr< const Shader >&	GetDefaultShader	( ) const { return mDefaultShader; };

	//! Set default shader.
	void								SetDefaultShader			( const boost::shared_ptr< const Shader >& shd ) { mDefaultShader = shd; };

private:
	HRESULT								TryToCreateD3DDevice		( HWND hwnd );
	HRESULT								ResetDevice					( );

	void								CacheVP						( ) const;

	//! Print out caps warnings.
	void								TestDeviceCaps				( ) const;

	//! Full screen change event.
	void								OnFullscreenChange			( bool isfullscreen, unsigned width, unsigned height );
};




_XFX_END

#ifdef __XFX_ENABLE_MULTITHREADING__
#define XFX_PLACE_DEVICE_LOCK boost::recursive_try_mutex::scoped_lock _xfx_device_lock( Render::Instance( ).GetDeviceMutex( ) );
#else
#define XFX_PLACE_DEVICE_LOCK
#endif


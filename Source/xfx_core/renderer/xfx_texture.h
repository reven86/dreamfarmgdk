//  File xfx_texture.h                                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_texture.h 
 *	\brief %Texture classes.
 */

_XFX_BEGIN





/*! \interface ITexture xfx_texture.h "render/xfx_texture.h"
 *	\brief Base interface for all texture classes.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

interface ITexture
{
public:
	virtual ~ITexture													( ) { };

	//! Get texture width.
	virtual unsigned						Width						( ) const = 0;

	//! Get texture height.
	virtual unsigned						Height						( ) const = 0;

	//! Get texture matrix.
	virtual const Mat4&						TextureMatrix				( ) const = 0;

	//! Get texture transformation.
	virtual const Mat4&						Transformation				( ) const = 0;

	//! Is texture matrix * texture transform identity matrix.
	virtual bool							IsIdentityResultTransform	( ) const = 0;

#if (__XFX_DIRECTX_VER__ < 9)
	//! Get ID3DBaseTexture.
	virtual LPDIRECT3DBASETEXTURE8			D3DTex						( ) const = 0;
#else
	//! Get ID3DBaseTexture.
	virtual LPDIRECT3DBASETEXTURE9			D3DTex						( ) const = 0;
#endif
};




/*! \class Texture xfx_texture.h "render/xfx_texture.h"
 *	\brief 2D texture.
 *	\ingroup RenderGroup
 *
 *	If g_texture_cache_enable is 1, all textures will be converted to right format and 
 *	stored in cache folder at g_texture_cache_folder. This leads to slow loading on
 *	first time, but very fast loading on the following runs.
 *
 *	\remark You should always use UpdateTransformation after update Transformable2D matrix.
 *	\author Andrew "RevEn" Karpushin
 */

class Texture : public ITexture, public Resource, public Transformable2D
{
#if (__XFX_DIRECTX_VER__ < 9)
	boost::shared_ptr< IDirect3DTexture8 >		mpTex;
#else
	boost::shared_ptr< IDirect3DTexture9 >		mpTex;
#endif

	unsigned				mWidth;				//bitmap width
	unsigned				mHeight;			//bitmap height
	unsigned				mSurfaceWidth;		//surface width
	unsigned				mSurfaceHeight;		//surface height
	unsigned				mNumMips;

	float					mKWidth;
	float					mKHeight;

	Mat4					mTextureMatrix;
	mutable Mat4			mTransformation;

	bool					mIsIdentityTransform;

	static Cache<Texture>	msCache;

	struct LoadInfo
	{
		D3DFORMAT			forced_format;
		int					num_mips;
	};

	boost::scoped_ptr< LoadInfo >	mLoadInfoPtr;

public:
	//! Construct an empty 1x1 texture.
	Texture													( );

	//! Copy constuctor.
	Texture													( const Texture& tex );

	//! Destructor.
	virtual ~Texture										( ) { Free( ); };

	//! Assignment operator.
	Texture&						operator =				( const Texture& tex );

	/*! \brief Create new texture surface.
	 *
	 *	\param[in]	width		Surface width.
	 *	\param[in]	height		Surface height.
	 *	\param[in]	nummips		Mipmaps count (0 - maximum).
	 *	\param[in]	fmt			Texture format.
	 *
	 *	If width or height is more than maximum dimension in D3DCaps, then texture
	 *	is created with maximum available dimension and the entire image is
	 *	streched to new dimensions.
	 *
	 *	\return S_OK if succeeded.
	 */
	HRESULT							Create					( unsigned width, unsigned height, unsigned nummips, const D3DFORMAT& fmt = D3DFMT_A8R8G8B8 );

	//! Destroy all allocated resources for texture.
	void							Free					( );

	//! Get width.
	virtual unsigned				Width					( ) const { return mWidth; };

	//! Get height.
	virtual unsigned				Height					( ) const { return mHeight; };

	//! Get texture matrix.
	virtual const Mat4&				TextureMatrix			( ) const { return mTextureMatrix; };

	//! Get texture transformation.
	virtual const Mat4&				Transformation			( ) const { return mTransformation; };

	//! Is texture matrix * texture transform identity matrix.
	virtual bool					IsIdentityResultTransform	( ) const { return mIsIdentityTransform; };

	//! Update texture matrix transformation.
	void							UpdateTransformation	( );

	//! Copy average RGB value from other texture into alpha channel of this texture.
	HRESULT							CopyAlphaFromRGB		( const Texture& tex );

#if (__XFX_DIRECTX_VER__ < 9)
	//! Get ID3DXBaseTexture.
	virtual LPDIRECT3DBASETEXTURE8	D3DTex					( ) const { return mpTex.get( ); };
#else
	//! Get ID3DXBaseTexture.
	virtual LPDIRECT3DBASETEXTURE9	D3DTex					( ) const { return mpTex.get( ); };
#endif

	//! Get mipmaps count.
	const unsigned&					NumMips					( ) const { return mNumMips; };

	//! Is texture empty.
	bool							IsEmpty					( ) const { return !mpTex; };

	/*! \brief Load from file.
	 *	
	 *	\param[in]	filename		File name.
	 *
	 *	\note If file extension absent, newly extension added by priorities: dds, tga, png, jpg, bmp.
	 *	\note To load separate rgb and alpha channels use ':' separator. Ex. foo.jpg:foo_a.jpg
	 *	\note Mipmaps count can be specified by adding "*mipmaps.3" at the filename end.
	 *	\note To force texture format, add "*fmt.dxt1". Supported formats are: x8r8g8b8, a8r8g8b8, dxt1, dxt2, dxt3, dxt4, dxt5
	 */
	virtual HRESULT					LoadFile				( const String& filename );

	//! Load from memory.
	virtual HRESULT					LoadMemory				( const void * pmemory, unsigned long filelen );

	//! Get texture data in ARGB format. \note Very expensive call.
	HRESULT							GetSurfaceData			( DWORD level, ARGB * data ) const;

	//! Set texture data from ARGB format. \note Very expensive call.
	HRESULT							SetSurfaceData			( DWORD level, ARGB * data );

	//! Get texture Cache.
	static Cache< Texture >&		Cache					( ) { return msCache; };

	/*! \brief Get texture coords for one animation frame.
	 *
	 *	Utility function. Get a texture coordinates for specified animation frame, based on animation parameters.
	 *
	 *	\param[out]			u0					Result first U coordinate.
	 *	\param[out]			v0					Result first V coordinate.
	 *	\param[out]			u1					Result second U coordinate.
	 *	\param[out]			v1					Result second V coordinate.
	 *	\param[in]			frame_num			Frame number.
	 *	\param[in]			frame_width_rel		Frame width relative to texture width.
	 *	\param[in]			frame_height_rel	Frame height relative to texture width.
	 */

	static void						GetFrameTexCoords		( float& u0, float& v0, float& u1, float& v1, int frame_num, float frame_width_rel, float frame_height_rel );

private:
#if (__XFX_DIRECTX_VER__ < 9)
	HRESULT							CopyTexture				( LPDIRECT3DTEXTURE8 ptex, unsigned width, unsigned height );
#else
	HRESULT							CopyTexture				( LPDIRECT3DTEXTURE9 ptex, unsigned width, unsigned height );
#endif

	void							SaveTextureToCacheFile	( const String& file ) const;
};



/*! \class CubemapTexture xfx_texture.h "render/xfx_texture.h"
 *	\brief Cubemap texture.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class CubemapTexture : public ITexture, public Transformable3D, public Resource
{
#if (__XFX_DIRECTX_VER__ < 9)
	boost::shared_ptr< IDirect3DCubeTexture8 >			mpTex;
#else
	boost::shared_ptr< IDirect3DCubeTexture9 >			mpTex;
#endif

	unsigned						mWidth;
	unsigned						mSurfaceWidth;
	unsigned						mNumMips;

	float							mKWidth;

	static Cache< CubemapTexture >	msCache;

	Mat4							mTextureMatrix;

public:
	//! Construct an empty cubemap texture.
	CubemapTexture														( );

	//! Destructor
	virtual ~CubemapTexture												( );

	//! Copy constructor.
	CubemapTexture														( const CubemapTexture& tex );

	//! Assignment operator.
	CubemapTexture&							operator =					(const CubemapTexture& tex);

	/*! \brief Create new cubemap texture surface.
	 *
	 *	\param[in]	width		Surface width.
	 *	\param[in]	nummips		Mipmaps count (0 - maximum).
	 *	\param[in]	fmt			Texture format.
	 *
	 *	\return S_OK if succeeded.
	 */
	HRESULT									Create						( unsigned width, unsigned nummips, const D3DFORMAT& fmt = D3DFMT_A8R8G8B8 );

	//! Destroy all allocated resources for texture.
	void									Free						();

	//! Get texture width.
	virtual unsigned						Width						() const {return mWidth;};

	//! Get texture height.
	virtual unsigned						Height						() const {return mWidth;};

	//! Get texture matrix.
	virtual const Mat4&						TextureMatrix				() const {return mTextureMatrix;};

	//! Get texture transformation.
	virtual const Mat4&						Transformation				() const {return Transformable3D::Transformation ();};

	//! Is texture matrix * texture transform identity matrix.
	virtual bool							IsIdentityResultTransform	( ) const { return mWidth == mSurfaceWidth && Transformable3D::Transformation( ).IsIdentity( ); };

#if (__XFX_DIRECTX_VER__ < 9)
	//! Get ID3DXBaseTexture.
	virtual LPDIRECT3DBASETEXTURE8			D3DTex						() const {return mpTex.get ();};
#else
	//! Get ID3DXBaseTexture.
	virtual LPDIRECT3DBASETEXTURE9			D3DTex						() const {return mpTex.get ();};
#endif

	//! Get mipmaps count.
	const unsigned&							NumMips						() const {return mNumMips;};

	//! Is texture empty.
	bool									IsEmpty						() const {return !mpTex;};

	/*! \brief Load from file.
	 *	
	 *	\param[in]	filename		File name.
	 *
	 *	\note File extension stripped and newly added by priorities: dds, tga, png, jpg, bmp.
	 *	\note To load cubemap faces from separate 2D textures use '|'. Ex: neg_x|pos_x|neg_y|pos_y|neg_z|pos_z
	 */
	virtual HRESULT							LoadFile					(const String& filename);

	//! Load from memory.
	virtual HRESULT							LoadMemory					(const void * pmemory, unsigned long filelen);

	//! Load one face from plain texture.
	HRESULT									LoadFace					(const D3DCUBEMAP_FACES& face, const Texture& tex);

	//! Get texture data in ARGB format. \note Very expensive call.
	HRESULT									GetSurfaceData				(const D3DCUBEMAP_FACES& face, DWORD level, ARGB * data) const;

	//! Set texture data from ARGB format. \note Very expensive call.
	HRESULT									SetSurfaceData				(const D3DCUBEMAP_FACES& face, DWORD level, ARGB * data);

	//! Get cubemap texture Cache.
	static Cache<CubemapTexture>&			Cache						() {return msCache;};

private:
#if (__XFX_DIRECTX_VER__ < 9)
	HRESULT									CopyTexture					(LPDIRECT3DCUBETEXTURE8 ptex, unsigned width);
#else
	HRESULT									CopyTexture					(LPDIRECT3DCUBETEXTURE9 ptex, unsigned width);
#endif
};



_XFX_END

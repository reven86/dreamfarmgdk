//  File xfx_texture.cpp                                                      |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_texture.h"
#include "xfx_renderer.h"
#include "xfx_renderer_cvars.h"




_XFX_BEGIN


#pragma message ("FIXME: implement a choice, where to store textures: managed or default pool")



static const char * sExtensions[] = {".dds", ".tga", ".png", ".jpg", ".bmp", ""};

HRESULT sFindTexture (const String& in, String& out)
{
	const char ** ext = sExtensions;

	String::size_type dot_pos = in.find_last_of ('.');
	String::size_type slash_pos = in.find_last_of ('\\');
	if( dot_pos < in.size( ) && ( slash_pos == String::npos || dot_pos > slash_pos ) )
	{
		out = in;
		return S_OK;
	}

	HRESULT hr = XFXERR_INVALIDCALL;
	while( **ext )
	{
		if( SUCCEEDED( hr = FileSystem::Instance( ).FindFile( out = in + *ext ) ) )
			return hr;

		ext++;
	}

	out = in;

	return hr;
};

String sGetTextureCacheFilename( const String& file )
{
	DWORD name = boost::hash_value( file );

	return boost::str( StringFormat( "%X.dds" ) % name );
}





//
// Texture
//

Cache<Texture> Texture::msCache;

Texture::Texture () : Resource ("Texture"), mpTex (), mWidth (1),
	mHeight (1), mSurfaceWidth (1), mSurfaceHeight (1), mNumMips (0),
	mKWidth (1.0f), mKHeight (1.0f), Transformable2D( ),
	mIsIdentityTransform( true )
{
}

Texture::Texture (const Texture& tex) : Resource ("Texture"), mpTex (),
	mWidth (tex.mWidth), mHeight (tex.mHeight), mSurfaceWidth (tex.mSurfaceWidth),
	mSurfaceHeight (tex.mSurfaceHeight), mKWidth (tex.mKWidth),
	mKHeight (tex.mKHeight), mNumMips (tex.mNumMips), Transformable2D( tex ),
	mIsIdentityTransform( tex.mIsIdentityTransform )
{
	CopyTexture( tex.mpTex.get( ), Width( ), Height( ) );
}

Texture& Texture::operator = (const Texture& tex)
{
	Free ();

	mWidth					= tex.mWidth;
	mHeight					= tex.mHeight;
	mSurfaceWidth			= tex.mSurfaceWidth;
	mSurfaceHeight			= tex.mSurfaceHeight;
	mKWidth					= tex.mKWidth;
	mKHeight				= tex.mKHeight;
	mNumMips				= tex.mNumMips;
	mIsIdentityTransform	= tex.mIsIdentityTransform;

	Transformable2D::operator = ( tex );

	CopyTexture (tex.mpTex.get (), Width (), Height ());

	return *this;
}

void Texture::Free ()
{
	mpTex.reset ();

	mWidth				= mHeight = mSurfaceWidth = mSurfaceHeight = mNumMips = 1;
	mKWidth				= mKHeight = 1.0f;

	mIsIdentityTransform = true;

	Transformable2D::ResetTransform( Vec2( 0.0f ), 0, Vec2( 1.0f ) );
}

void Texture::UpdateTransformation( )
{
	mIsIdentityTransform = mWidth == mSurfaceWidth && 
		mHeight == mSurfaceHeight && 
		Transformable2D::Transformation( ).IsIdentity( );

	mTransformation = Transformable2D::Transformation( ).ExpandToMat4( );
}

HRESULT Texture::Create (unsigned width, unsigned height, unsigned nummips, const D3DFORMAT& fmt)
{
	bool capped_size = false;

	if( width > Renderer::Instance( ).D3DCaps( ).MaxTextureWidth )
	{
		capped_size = true;
		width = Renderer::Instance( ).D3DCaps( ).MaxTextureWidth;
	}

	if( height > Renderer::Instance( ).D3DCaps( ).MaxTextureHeight )
	{
		capped_size = true;
		height = Renderer::Instance( ).D3DCaps( ).MaxTextureHeight;
	}

	Free ();

	unsigned i = 0, j = 0;

#pragma message ( "FIXME: if non power of 2 textures are supported, no need to extend texture surface" )

	for (mSurfaceWidth = 1; mSurfaceWidth < width; mSurfaceWidth <<= 1, i++)
		;

	for (mSurfaceHeight = 1; mSurfaceHeight < height; mSurfaceHeight <<= 1, j++)
		;

	mNumMips	= ( nummips ) ? nummips : std::max( i, j );

	if( capped_size )
	{
		mWidth = mSurfaceWidth;
		mHeight = mSurfaceHeight;
		mKWidth = mKHeight = 1.0f;
	}
	else
	{
		mWidth		= width;
		mHeight		= height;
		mKWidth		= static_cast<float> (mWidth) / mSurfaceWidth;
		mKHeight	= static_cast<float> (mHeight) / mSurfaceHeight;
	}

	mTextureMatrix = Mat4 (
		mKWidth, 0.0f, 0.0f, 0.0f,
		0.0f, mKHeight, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DTEXTURE8 tex;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateTexture (mSurfaceWidth, mSurfaceHeight, nummips, 0, fmt, D3DPOOL_MANAGED, &tex);
#else
	LPDIRECT3DTEXTURE9 tex;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateTexture (mSurfaceWidth, mSurfaceHeight, nummips, 0, fmt, D3DPOOL_MANAGED, &tex, NULL);
#endif

	if (SUCCEEDED (hr))
		mpTex.reset (tex, IUnknownDeleter ());

	UpdateTransformation( );

	return hr;
}

void Texture::GetFrameTexCoords( float& u0, float& v0, float& u1, float& v1,
	int frame_num, float frame_width_rel, float frame_height_rel )
{
	u0 = 0.0f;
	v0 = 0.0f;
	u1 = 1.0f;
	v1 = 1.0f;

	_ASSERTE( fabs( frame_width_rel ) > 0.01f && fabs( frame_height_rel ) > 0.01f );
	if( fabs( frame_width_rel ) <= 0.01f || fabs( frame_height_rel ) <= 0.01f )
		return;

	int frames_in_a_row = static_cast< int >( 1.0f / frame_width_rel );

	_ASSERTE( frames_in_a_row != 0 );
	if( frames_in_a_row == 0 )
		return;

	int frames_in_a_col = static_cast< int >( 1.0f / frame_height_rel );

	_ASSERTE( frames_in_a_col != 0 );
	if( frames_in_a_col == 0 )
		return;

	u0 = frame_width_rel * ( frame_num % frames_in_a_row );
	v0 = frame_height_rel * ( frame_num / frames_in_a_row % frames_in_a_col );

	u1 = u0 + frame_width_rel;
	v1 = v0 + frame_height_rel;
}

HRESULT Texture::GetSurfaceData (DWORD level, ARGB * data) const
{
	if (IsEmpty ())
		return XFXERR_INVALIDCALL;

	HRESULT hr;

	unsigned w = Width () >> level;
	unsigned h = Height () >> level;
	if (w < 1) w = 1;
	if (h < 1) h = 1;

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DSURFACE8 surf;

	if (FAILED (hr = Renderer::Instance ().pD3DDevice ()->CreateImageSurface (w, h, D3DFMT_A8R8G8B8, &surf)))
		return hr;

	LPDIRECT3DSURFACE8 s;
#else
	LPDIRECT3DSURFACE9 surf;

	if (FAILED (hr = Renderer::Instance ().pD3DDevice ()->CreateOffscreenPlainSurface (w, h, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surf, NULL)))
		return hr;

	LPDIRECT3DSURFACE9 s;
#endif

	boost::shared_ptr<IUnknown> surf_ptr (surf, IUnknownDeleter ());

	if (FAILED (hr = mpTex->GetSurfaceLevel (level, &s)))
		return hr;

	boost::shared_ptr<IUnknown> s_ptr (s, IUnknownDeleter ());

	if (FAILED (hr = D3DXLoadSurfaceFromSurface (surf, NULL, NULL, s, NULL, NULL, D3DX_FILTER_NONE, 0)))
		return hr;

	//Lock
	D3DLOCKED_RECT lr;
	if (FAILED (hr = surf->LockRect (&lr, NULL, D3DLOCK_READONLY)))
		return hr;

	char * in = reinterpret_cast<char *> (lr.pBits);
	for (unsigned y = 0; y < h; y++, data += w, in += lr.Pitch)
		memcpy (data, in, sizeof (ARGB) * w);

	surf->UnlockRect ();

	return hr;
}

HRESULT Texture::SetSurfaceData (DWORD level, ARGB * data)
{
	if (IsEmpty ())
		return XFXERR_INVALIDCALL;

	HRESULT hr;

	unsigned w = Width () >> level;
	unsigned h = Height () >> level;
	if (w < 1) w = 1;
	if (h < 1) h = 1;

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DSURFACE8 surf;

	if (FAILED (hr = Renderer::Instance ().pD3DDevice ()->CreateImageSurface (w, h, D3DFMT_A8R8G8B8, &surf)))
		return hr;

	LPDIRECT3DSURFACE8 s;
#else
	LPDIRECT3DSURFACE9 surf;

	if (FAILED (hr = Renderer::Instance ().pD3DDevice ()->CreateOffscreenPlainSurface (w, h, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surf, NULL)))
		return hr;

	LPDIRECT3DSURFACE9 s;
#endif

	boost::shared_ptr<IUnknown> surf_ptr (surf, IUnknownDeleter ());

	//Lock
	D3DLOCKED_RECT lr;
	if (FAILED(hr = surf->LockRect (&lr, NULL, D3DLOCK_NOSYSLOCK)))
		return hr;

	char * in = reinterpret_cast<char *> (lr.pBits);
	for (unsigned y = 0; y < h; y++, data += w, in += lr.Pitch)
		memcpy (in, data, sizeof (ARGB) * w);

	surf->UnlockRect ();

	if (FAILED (hr = mpTex->GetSurfaceLevel (level, &s)))
		return hr;

	boost::shared_ptr<IUnknown> s_ptr (s, IUnknownDeleter ());

	hr = D3DXLoadSurfaceFromSurface (s, NULL, NULL, surf, NULL, NULL, D3DX_FILTER_NONE, 0);
	
	return hr;
}

HRESULT Texture::LoadFile( const String& file )
{
	HRESULT hr;

	if( g_texture_cache_enable->AsInt( ) )
	{
		// try to load texture from cache
#if (__XFX_DIRECTX_VER__ < 9)
		LPDIRECT3DTEXTURE8 tex;
#else
		LPDIRECT3DTEXTURE9 tex;
#endif
		String tex_file = sGetTextureCacheFilename( file );
		String info_file = tex_file + ".info";

		unsigned long filesize;
		HRESULT hr;
		if( SUCCEEDED( hr = FileSystem::Instance( ).GetFileSize( info_file, filesize ) ) )
		{
			boost::scoped_array< BYTE > pbuf( new BYTE[ filesize ] );

			FileSystem::Instance( ).ReadFile( info_file, pbuf.get( ) );

			const BYTE * buf = pbuf.get( );

			mWidth = *reinterpret_cast< const DWORD * >( buf );
			buf += sizeof( mWidth );

			mHeight = *reinterpret_cast< const DWORD * >( buf );
			buf += sizeof( mHeight );

			mSurfaceWidth = *reinterpret_cast< const DWORD * >( buf );
			buf += sizeof( mSurfaceWidth );

			mSurfaceHeight = *reinterpret_cast< const DWORD * >( buf );
			buf += sizeof( mSurfaceHeight );

			mNumMips = *reinterpret_cast< const DWORD * >( buf );
			buf += sizeof( mNumMips );

			D3DFORMAT fmt = *reinterpret_cast< const D3DFORMAT * >( buf );
			buf += sizeof( fmt );

			mKWidth		= static_cast<float> (mWidth) / mSurfaceWidth;
			mKHeight	= static_cast<float> (mHeight) / mSurfaceHeight;

			mTextureMatrix = Mat4 (
				mKWidth, 0.0f, 0.0f, 0.0f,
				0.0f, mKHeight, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);

			UpdateTransformation( );

			if( SUCCEEDED( hr = FileSystem::Instance( ).GetFileSize( tex_file, filesize ) ) )
			{
				boost::scoped_array< BYTE > pbuf( new BYTE[ filesize ] );

				FileSystem::Instance( ).ReadFile( tex_file, pbuf.get( ) );

				XFX_PLACE_DEVICE_LOCK;

				hr = D3DXCreateTextureFromFileInMemoryEx( 
					Renderer::Instance( ).pD3DDevice( ), pbuf.get( ), filesize, mSurfaceWidth, mSurfaceHeight, 
					mNumMips, 0, fmt, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL, &tex );

				if( SUCCEEDED( hr ) )
				{
					mpTex.reset( tex, IUnknownDeleter( ) );
					return hr;
				}
			}
		}
	}

	String::size_type dots_pos = file.find_first_of( ':' );

	mLoadInfoPtr.reset( new LoadInfo );
	mLoadInfoPtr->forced_format = D3DFMT_UNKNOWN;
	mLoadInfoPtr->num_mips = 0;

	// reset mLoadInfoPtr at function end.

	struct loadinfo_reset
	{
		void operator ( )				( boost::scoped_ptr< LoadInfo > * l ) const { l->reset( ); };
	};

	boost::shared_ptr< boost::scoped_ptr< LoadInfo > > load_del( &mLoadInfoPtr, loadinfo_reset( ) );

	// get options

	String::size_type option_pos = file.find_first_of( '*' );
	String::size_type filenames_end = option_pos;

	while( option_pos != file.npos )
	{
		++option_pos;

		// get option
		String::size_type option_end = file.find_first_of( '*', option_pos );

		if( option_end == file.npos )
			option_end = file.size( );

		String full_option = file.substr( option_pos, option_end - option_pos );

		if( !full_option.empty( ) )
		{
			String::size_type option_dot_pos = full_option.find_first_of( '.' );

			if( option_dot_pos == full_option.npos )
				option_dot_pos = full_option.size( );

			String option = full_option.substr( 0, option_dot_pos );
			String params = full_option.substr( option_dot_pos + 1, full_option.size( ) - option_dot_pos );

			if( option == "mipmaps" )
			{
				// handle mip maps option
				mLoadInfoPtr->num_mips = atoi( params.c_str( ) );
			}
			else if( option == "fmt" )
			{
				D3DFORMAT fmt = D3DFMT_UNKNOWN;

				if( params == "a8r8g8b8" )
					fmt = D3DFMT_A8R8G8B8;
				else if( params == "x8r8g8b8" )
					fmt = D3DFMT_X8R8G8B8;
				else if( params == "dxt1" )
					fmt = D3DFMT_DXT1;
				else if( params == "dxt2" )
					fmt = D3DFMT_DXT2;
				else if( params == "dxt3" )
					fmt = D3DFMT_DXT3;
				else if( params == "dxt4" )
					fmt = D3DFMT_DXT4;
				else if( params == "dxt5" )
					fmt = D3DFMT_DXT5;

				D3DDEVICE_CREATION_PARAMETERS params;

				if( SUCCEEDED( Renderer::Instance( ).pD3DDevice( )->GetCreationParameters( &params ) ) &&
					SUCCEEDED( Renderer::Instance( ).pD3D( )->CheckDeviceFormat( 
						params.AdapterOrdinal, params.DeviceType, Renderer::Instance( ).D3DPP( ).BackBufferFormat,
						0, D3DRTYPE_TEXTURE, fmt ) )
						)
				{
					mLoadInfoPtr->forced_format = fmt;
				}
			}
		}

		option_pos = file.find_first_of( '*', option_pos );
	}

	if( dots_pos != file.npos )
	{
		String newrgbfile;
		if( FAILED( hr = sFindTexture( file.substr( 0, dots_pos ), newrgbfile ) ) )
		{
			gError( "Can't find image file for a name %s", file.substr( 0, dots_pos ).c_str( ) );
			return hr;
		}

		String newalphafile;
		if( FAILED( hr = sFindTexture( file.substr( dots_pos + 1, filenames_end - dots_pos - 1 ), newalphafile ) ) )
		{
			gError( "Can't find image file for a name %s", file.substr( dots_pos + 1, filenames_end - dots_pos - 1 ).c_str( ) );
			return hr;
		}

		//try to load alpha
		Texture tex;
		if (FAILED (tex.LoadFile (newalphafile)))
		{
			gWarn ("Alpha file \"%s\" not loaded!", newalphafile.c_str ());
			return hr;
		}
		else
		{
			if( mLoadInfoPtr->forced_format == D3DFMT_UNKNOWN )
				mLoadInfoPtr->forced_format = D3DFMT_A8R8G8B8;
		}

		//try to load rgb first
		if (FAILED (hr = Resource::LoadFile (newrgbfile)))
			return hr;

		rFilename () = file;

		//alpha loaded
		hr = CopyAlphaFromRGB (tex);

		if( g_texture_cache_enable->AsInt( ) && SUCCEEDED( hr ) )
			SaveTextureToCacheFile( file );

		return hr;
	}

	String newfile;
	if (FAILED (hr = sFindTexture( file.substr( 0, filenames_end ), newfile ) ) )
	{
		gError( "Can't find image file for a name %s", file.substr( 0, filenames_end ).c_str( ) );
		return hr;
	}

	hr = Resource::LoadFile( newfile );

	if( g_texture_cache_enable->AsInt( ) && SUCCEEDED( hr ) )
		SaveTextureToCacheFile( file );

	return hr;
}

void Texture::SaveTextureToCacheFile( const String& file ) const
{
	String tex_file = g_texture_cache_folder->Value( ) + sGetTextureCacheFilename( file );
	String info_file = tex_file + ".info";

	FILE * f = fopen( info_file.c_str( ), "wb" );

	if( f )
	{
		D3DFORMAT fmt = D3DFMT_UNKNOWN;

		if( mpTex )
		{
			D3DSURFACE_DESC sd;
			if( SUCCEEDED( mpTex->GetLevelDesc( 0, &sd ) ) )
			{
				fmt = sd.Format;
			}
		}

		fwrite( &mWidth, sizeof( mWidth ), 1, f );
		fwrite( &mHeight, sizeof( mHeight ), 1, f );
		fwrite( &mSurfaceWidth, sizeof( mSurfaceWidth ), 1, f );
		fwrite( &mSurfaceHeight, sizeof( mSurfaceHeight ), 1, f );
		fwrite( &mNumMips, sizeof( mNumMips ), 1, f );
		fwrite( &fmt, sizeof( fmt ), 1, f );
		fclose( f );
	}

	D3DXSaveTextureToFileA( tex_file.c_str( ), D3DXIFF_DDS, mpTex.get( ), NULL );
}

HRESULT Texture::LoadMemory (const void * p, unsigned long filelen)
{
	_ASSERTE (p);

	if (!p)
		return XFXERR_INVALIDCALL;

	Free ();

	HRESULT hr;
#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DTEXTURE8 tex;
	LPDIRECT3DSURFACE8 s1;
#else
	LPDIRECT3DTEXTURE9 tex;
	LPDIRECT3DSURFACE9 s1;
#endif
	D3DXIMAGE_INFO info;

	D3DFORMAT fmt = mLoadInfoPtr ? mLoadInfoPtr->forced_format : D3DFMT_UNKNOWN;
	int num_mips = mLoadInfoPtr ? mLoadInfoPtr->num_mips : 0;

	{
	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	if (FAILED (hr = D3DXCreateTextureFromFileInMemoryEx (Renderer::Instance ().pD3DDevice (), p, filelen, D3DX_DEFAULT, D3DX_DEFAULT, num_mips, 0, fmt, D3DPOOL_SYSTEMMEM, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &tex)))
		return hr;
#else
	if (FAILED (hr = D3DXCreateTextureFromFileInMemoryEx (Renderer::Instance ().pD3DDevice (), p, filelen, D3DX_DEFAULT, D3DX_DEFAULT, num_mips, 0, fmt, D3DPOOL_SYSTEMMEM, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &tex)))
		return hr;
#endif
	}

	boost::shared_ptr<IUnknown> tex_lock (tex, IUnknownDeleter ());

	if (FAILED (hr = tex->GetSurfaceLevel (0, &s1)))
		return hr;

	boost::shared_ptr<IUnknown> s1_lock (s1, IUnknownDeleter ());

	D3DSURFACE_DESC d1;

	if( FAILED( hr = s1->GetDesc( &d1 ) ) )
		return hr;

	if( info.Width > d1.Width || info.Height > d1.Height )
	{
		XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
		if (FAILED (hr = D3DXCreateTextureFromFileInMemoryEx (Renderer::Instance ().pD3DDevice (), p, filelen, D3DX_DEFAULT, D3DX_DEFAULT, num_mips, 0, fmt, D3DPOOL_SYSTEMMEM, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &tex)))
			return hr;
#else
		if (FAILED (hr = D3DXCreateTextureFromFileInMemoryEx (Renderer::Instance ().pD3DDevice (), p, filelen, D3DX_DEFAULT, D3DX_DEFAULT, num_mips, 0, fmt, D3DPOOL_SYSTEMMEM, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &tex)))
			return hr;
#endif
	}

	CopyTexture (tex, info.Width, info.Height);

	return hr;
}

#if (__XFX_DIRECTX_VER__ < 9)
HRESULT Texture::CopyTexture (LPDIRECT3DTEXTURE8 tex, unsigned width, unsigned height)
#else
HRESULT Texture::CopyTexture (LPDIRECT3DTEXTURE9 tex, unsigned width, unsigned height)
#endif
{
	HRESULT hr = XFXERR_INVALIDCALL;

	if (tex)
	{
		D3DSURFACE_DESC sd;

		DWORD nummips = tex->GetLevelCount ();
		tex->GetLevelDesc (0, &sd);

		if (FAILED (hr = Create (width, height, nummips, sd.Format)))
			return hr;

		nummips = mpTex->GetLevelCount( );

		for (DWORD level = 0; (level < nummips) && SUCCEEDED (hr); level++)
		{
#if (__XFX_DIRECTX_VER__ < 9)
			LPDIRECT3DSURFACE8 s1, s2;
#else
			LPDIRECT3DSURFACE9 s1, s2;
#endif

			if (FAILED (hr = tex->GetSurfaceLevel (level, &s1)))
				return hr;

			boost::shared_ptr<IUnknown> s1_lock (s1, IUnknownDeleter ());

			if (FAILED (hr = mpTex->GetSurfaceLevel (level, &s2)))
				return hr;		

			boost::shared_ptr<IUnknown> s2_lock (s2, IUnknownDeleter ());

			if( width > mWidth || height > mHeight )
				hr = D3DXLoadSurfaceFromSurface (s2, NULL, NULL, s1, NULL, NULL, D3DX_DEFAULT, 0);
			else
				hr = D3DXLoadSurfaceFromSurface (s2, NULL, NULL, s1, NULL, NULL, D3DX_FILTER_NONE, 0);
		}
	}

	return hr;
}

HRESULT Texture::CopyAlphaFromRGB( const Texture& tex )
{
	if( IsEmpty( ) || tex.IsEmpty( ) )
		return XFXERR_INVALIDCALL;

	boost::scoped_array< ARGB > data1( new ARGB[ Width( ) * Height( ) ] );
	boost::scoped_array< ARGB > data2( new ARGB[ tex.Width( ) * tex.Height( ) ] );

	float kx = static_cast< float >( tex.Width( ) ) / Width( );
	float ky = static_cast< float >( tex.Height( ) ) / Height( );

	int w = Width( );
	int h = Height( );
	DWORD max_level = std::min( NumMips( ), tex.NumMips( ) );
	for( DWORD level = 0; level < max_level; level++, w >>= 1, h >>= 1 )
	{
		if( w < 1 ) w = 1;
		if( h < 1 ) h = 1;

		GetSurfaceData( level, data1.get( ) );
		tex.GetSurfaceData( level, data2.get( ) );

		for( int y = 0; y < h; y++ )
			for( int x = 0; x < w; x++ )
			{
				const ARGB& col = data2[ static_cast< int >( y * w * ky + x * kx ) ];

				data1[ y * w + x ].a = static_cast< BYTE >( static_cast< unsigned >( col.r + col.g + col.b ) / 3 );
			}

		SetSurfaceData( level, data1.get( ) );
	}

	return S_OK;
}



//CubemapTexture
Cache< CubemapTexture > CubemapTexture::msCache;

CubemapTexture::CubemapTexture( ) : Resource( "Cubemap" ), mpTex ( ), mWidth( 1 ),
	mSurfaceWidth( 1 ), mNumMips( 0 ), mKWidth( 1.0f )
{
}

CubemapTexture::~CubemapTexture( )
{
	Free( );
}

CubemapTexture::CubemapTexture( const CubemapTexture& tex ) : Resource( "Cubemap" ),
	mpTex( ), mWidth( tex.mWidth ), mSurfaceWidth( tex.mSurfaceWidth ),
	mKWidth( tex.mKWidth ), mNumMips( tex.mNumMips ), Transformable3D( tex )
{
	CopyTexture( tex.mpTex.get( ), Width( ) );
}

CubemapTexture& CubemapTexture::operator = ( const CubemapTexture& tex )
{
	Free( );

	mWidth			= tex.mWidth;
	mSurfaceWidth	= tex.mSurfaceWidth;
	mKWidth			= tex.mKWidth;
	mNumMips		= tex.mNumMips;

	Transformable3D::operator = ( tex );

	CopyTexture( tex.mpTex.get( ), Width( ) );

	return *this;
}

void CubemapTexture::Free( )
{
	mpTex.reset( );
	mWidth = mSurfaceWidth = mNumMips = 1;
	mKWidth = 1.0f;

	ResetTransform( Vec3( 0.0f ), Euler( 0 ), Vec3( 1.0f ) );
}

HRESULT CubemapTexture::Create( unsigned width, unsigned nummips, const D3DFORMAT& fmt )
{
	Free( );

	bool capped_size = false;

	if( width > Renderer::Instance( ).D3DCaps( ).MaxTextureWidth )
	{
		capped_size = true;
		width = Renderer::Instance( ).D3DCaps( ).MaxTextureWidth;
	}

	unsigned i = 0;
	for( mSurfaceWidth = 1; mSurfaceWidth < width; mSurfaceWidth <<= 1, i++ )
		;

	if( capped_size )
	{
		mWidth = mSurfaceWidth;
		mKWidth = 1.0f;
	}
	else
	{
		mWidth = width;
		mKWidth = static_cast< float >( mWidth ) / mSurfaceWidth;
	}

	mNumMips = ( nummips ) ? nummips : i;

	mTextureMatrix = Mat4 (
		mKWidth, 0.0f, 0.0f, 0.0f,
		0.0f, mKWidth, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DCUBETEXTURE8 tex;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateCubeTexture (mSurfaceWidth, nummips, 0, fmt, D3DPOOL_MANAGED, &tex);
#else
	LPDIRECT3DCUBETEXTURE9 tex;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateCubeTexture (mSurfaceWidth, nummips, 0, fmt, D3DPOOL_MANAGED, &tex, NULL);
#endif

	if (SUCCEEDED (hr))
		mpTex.reset (tex, IUnknownDeleter ());

	return hr;
}

HRESULT CubemapTexture::GetSurfaceData (const D3DCUBEMAP_FACES& face, DWORD level, ARGB * data) const
{
	if (IsEmpty ())
		return XFXERR_INVALIDCALL;

	HRESULT hr;

	unsigned w = Width () >> level;
	if (w < 1) w = 1;

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DSURFACE8 surf;

	if (FAILED (hr = Renderer::Instance ().pD3DDevice ()->CreateImageSurface (w, w, D3DFMT_A8R8G8B8, &surf)))
		return hr;

	LPDIRECT3DSURFACE8 s;
#else
	LPDIRECT3DSURFACE9 surf;

	if (FAILED (hr = Renderer::Instance ().pD3DDevice ()->CreateOffscreenPlainSurface (w, w, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surf, NULL)))
		return hr;

	LPDIRECT3DSURFACE9 s;
#endif

	boost::shared_ptr<IUnknown> surf_lock (surf, IUnknownDeleter ());

	if (FAILED (hr = mpTex->GetCubeMapSurface (face, level, &s)))
		return hr;

	boost::shared_ptr<IUnknown> s_lock (s, IUnknownDeleter ());

	if (FAILED (hr = D3DXLoadSurfaceFromSurface (surf, NULL, NULL, s, NULL, NULL, D3DX_FILTER_NONE, 0)))
		return hr;

	//Lock
	D3DLOCKED_RECT lr;
	if (FAILED(hr = surf->LockRect (&lr, NULL, D3DLOCK_READONLY)))
		return hr;

	char * in = reinterpret_cast<char *> (lr.pBits);
	for (unsigned y = 0; y < w; y++, data += w, in += lr.Pitch)
		memcpy (data, in, sizeof (ARGB) * w);

	surf->UnlockRect ();

	return hr;
}

HRESULT CubemapTexture::SetSurfaceData (const D3DCUBEMAP_FACES& face, DWORD level, ARGB * data)
{
	if (IsEmpty ())
		return XFXERR_INVALIDCALL;

	HRESULT hr;

	unsigned w = Width () >> level;
	if (w < 1) w = 1;

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DSURFACE8 surf;

	if (FAILED (hr = Renderer::Instance ().pD3DDevice ()->CreateImageSurface (w, w, D3DFMT_A8R8G8B8, &surf)))
		return hr;

	LPDIRECT3DSURFACE8 s;
#else
	LPDIRECT3DSURFACE9 surf;

	if (FAILED (hr = Renderer::Instance ().pD3DDevice ()->CreateOffscreenPlainSurface (w, w, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surf, NULL)))
		return hr;

	LPDIRECT3DSURFACE9 s;
#endif

	boost::shared_ptr<IUnknown> surf_lock (surf, IUnknownDeleter ());

	//Lock
	D3DLOCKED_RECT lr;
	if (FAILED(hr = surf->LockRect (&lr, NULL, D3DLOCK_NOSYSLOCK)))
		return hr;

	char * in = reinterpret_cast<char *> (lr.pBits);
	for (unsigned y = 0; y < w; y++, data += w, in += lr.Pitch)
		memcpy (in, data, sizeof (ARGB) * w);

	surf->UnlockRect ();

	if (FAILED (hr = mpTex->GetCubeMapSurface (face, level, &s)))
		return hr;

	boost::shared_ptr<IUnknown> s_lock (s, IUnknownDeleter ());

	hr = D3DXLoadSurfaceFromSurface (s, NULL, NULL, surf, NULL, NULL, D3DX_FILTER_NONE, 0);
	
	return hr;
}

HRESULT CubemapTexture::LoadFile( const String& file )
{
	HRESULT hr;

	String::size_type d_pos = file.find_first_of ('|');
	String pos_x, neg_x, pos_y, neg_y, pos_z, neg_z;

	if( d_pos == file.size( ) )
	{
		String newfile;
		if (FAILED (hr = sFindTexture (file, newfile)))
			return hr;

		return Resource::LoadFile (newfile);
	}

	String::size_type start = 0;

	neg_x = file.substr (0, d_pos);
	start = d_pos + 1;

	d_pos = file.find_first_of ('|', start);
	if (d_pos != file.size ())
	{
		pos_x = file.substr (start, d_pos - start);
		start = d_pos + 1;
	}

	d_pos = file.find_first_of ('|', start);
	if (d_pos != file.size ())
	{
		neg_y = file.substr (start, d_pos - start);
		start = d_pos + 1;
	}

	d_pos = file.find_first_of ('|', start);
	if (d_pos != file.size ())
	{
		pos_y = file.substr (start, d_pos - start);
		start = d_pos + 1;
	}

	d_pos = file.find_first_of ('|', start);
	if (d_pos != file.size ())
	{
		neg_z = file.substr (start, d_pos - start);
		start = d_pos + 1;
	}

	pos_z = file.substr (start);

	Texture tex_pos_x, tex_neg_x, tex_pos_y, tex_neg_y, tex_pos_z, tex_neg_z;
	Free ();

	if (SUCCEEDED (tex_neg_x.LoadFile (neg_x)))
		LoadFace (D3DCUBEMAP_FACE_NEGATIVE_X, tex_neg_x);

	if (SUCCEEDED (tex_pos_x.LoadFile (pos_x)))
		LoadFace (D3DCUBEMAP_FACE_POSITIVE_X, tex_pos_x);

	if (SUCCEEDED (tex_neg_y.LoadFile (neg_y)))
		LoadFace (D3DCUBEMAP_FACE_NEGATIVE_Y, tex_neg_y);

	if (SUCCEEDED (tex_pos_y.LoadFile (pos_y)))
		LoadFace (D3DCUBEMAP_FACE_POSITIVE_Y, tex_pos_y);

	if (SUCCEEDED (tex_neg_z.LoadFile (neg_z)))
		LoadFace (D3DCUBEMAP_FACE_NEGATIVE_Z, tex_neg_z);

	if (SUCCEEDED (tex_pos_z.LoadFile (pos_z)))
		LoadFace (D3DCUBEMAP_FACE_POSITIVE_Z, tex_pos_z);

	rFilename () = file;

	return S_OK;
}

HRESULT CubemapTexture::LoadFace (const D3DCUBEMAP_FACES& face, const Texture& tex)
{
	_ASSERTE (tex.Width () == tex.Height () && (IsEmpty () || Width () == tex.Width ()));
	if (tex.Width () != tex.Height () || (!IsEmpty () && Width () != tex.Width ()))
		return XFXERR_INVALIDCALL;

	HRESULT hr;

	if (IsEmpty ())
		if (FAILED (hr = Create (tex.Width (), tex.NumMips ())))
			return hr;

	boost::scoped_array<ARGB> src (new ARGB[Width () * Width ()]);

	int w = Width ();
	for (DWORD level = 0; level < NumMips (); level++, w >>= 1)
	{
		if (w < 1) w = 1;

		tex.GetSurfaceData (level, src.get ());
		SetSurfaceData (face, level, src.get ());
	}

	return S_OK;
}

HRESULT CubemapTexture::LoadMemory (const void * p, unsigned long filelen)
{
	_ASSERTE (p);

	if (!p)
		return XFXERR_INVALIDCALL;

	Free ();

	HRESULT hr;
	D3DXIMAGE_INFO info;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DCUBETEXTURE8 tex;
#else
	LPDIRECT3DCUBETEXTURE9 tex;
#endif

	{
	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	if (FAILED (hr = D3DXCreateCubeTextureFromFileInMemoryEx (Renderer::Instance ().pD3DDevice (), p, filelen, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_SYSTEMMEM, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &info, NULL, &tex)))
		return hr;
#else
	if (FAILED (hr = D3DXCreateCubeTextureFromFileInMemoryEx (Renderer::Instance ().pD3DDevice (), p, filelen, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_SYSTEMMEM, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &info, NULL, &tex)))
		return hr;
#endif
	}

	CopyTexture (tex, info.Width);

	tex->Release ();

	return hr;
}

#if (__XFX_DIRECTX_VER__ < 9)
HRESULT CubemapTexture::CopyTexture (LPDIRECT3DCUBETEXTURE8 ptex, unsigned width)
#else
HRESULT CubemapTexture::CopyTexture (LPDIRECT3DCUBETEXTURE9 ptex, unsigned width)
#endif
{
	HRESULT hr = XFXERR_INVALIDCALL;

	if (ptex)
	{
		D3DSURFACE_DESC sdesc;
		ptex->GetLevelDesc (0, &sdesc);

		DWORD nummips = ptex->GetLevelCount ();
		if (FAILED (hr = Create (width, nummips, sdesc.Format)))
			return hr;

		for (int face = 0; face < 6; face++)
			for (DWORD level = 0; (level < nummips) && SUCCEEDED (hr); level++)
			{
#if (__XFX_DIRECTX_VER__ < 9)
				LPDIRECT3DSURFACE8 s1, s2;
#else
				LPDIRECT3DSURFACE9 s1, s2;
#endif

				if (FAILED (hr = ptex->GetCubeMapSurface (static_cast<D3DCUBEMAP_FACES> (face), level, &s1)))
					return hr;

				boost::shared_ptr<IUnknown> s1_lock (s1, IUnknownDeleter ());

				if (FAILED (hr = mpTex->GetCubeMapSurface (static_cast<D3DCUBEMAP_FACES> (face), level, &s2)))
					return hr;

				boost::shared_ptr<IUnknown> s2_lock (s2, IUnknownDeleter ());

				hr = D3DXLoadSurfaceFromSurface (s2, NULL, NULL, s1, NULL, NULL, D3DX_FILTER_NONE, 0);
			}
	}

	return hr;
}



_XFX_END
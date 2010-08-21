//  File xfx_shader.cpp                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_shader.h"
#include "xfx_texture.h"
#include "xfx_renderer.h"

_XFX_BEGIN






//
// Effect
//

Cache< Effect > Effect::msCache;

Effect::Effect( ) :
	Resource( "Effect" )
{
#if( __XFX_DIRECTX_VER__ >= 9 )
	mKnownSemantics.assign( NULL );
#endif
}

Effect::~Effect( )
{
	Cleanup( );
}

void Effect::Cleanup( )
{
#if( __XFX_DIRECTX_VER__ < 9 )
	mValidTechnique = "";
#else
	mValidTechnique = NULL;
#endif

	mDXEffectPtr.reset( reinterpret_cast< ID3DXEffect * >( NULL ) );

	mInvalidateConnection.disconnect( );
}

void Effect::InvalidateCallback( bool invalidate )
{
	if( mDXEffectPtr )
	{
		if( invalidate )
		{
			mDXEffectPtr->OnLostDevice( );
		}
		else
		{
			mDXEffectPtr->OnResetDevice( );
		}
	}
}

void Effect::SetupEffectData( ) const
{
#if( __XFX_DIRECTX_VER__ >= 9 )
	Mat4 world, view, proj;

	if( mKnownSemantics[ ES_WORLD ] )
	{
		Renderer::Instance( ).GetTransform( D3DTS_WORLD, world );
		mDXEffectPtr->SetMatrix( mKnownSemantics[ ES_WORLD ], ( const D3DXMATRIX * )( &world ) );
	}

	if( mKnownSemantics[ ES_WORLDVIEW ] )
	{
		Renderer::Instance( ).GetTransform( D3DTS_WORLD, world );
		Renderer::Instance( ).GetTransform( D3DTS_VIEW, view );

		xfx::Mat4 m( world * view );
		mDXEffectPtr->SetMatrix (mKnownSemantics[ES_WORLDVIEW], ( const D3DXMATRIX * )( &m ) );
	}

	if (mKnownSemantics[ES_WORLDVIEWPROJECTION])
	{
		Renderer::Instance ().GetTransform (D3DTS_WORLD, world);

		xfx::Mat4 m( world * Renderer::Instance ().GetVP () );
		mDXEffectPtr->SetMatrix (mKnownSemantics[ES_WORLDVIEWPROJECTION], ( const D3DXMATRIX * )( &m ) );
	}

	if (mKnownSemantics[ES_VIEW])
	{
		Renderer::Instance ().GetTransform (D3DTS_VIEW, view);
		mDXEffectPtr->SetMatrix (mKnownSemantics[ES_VIEW], ( const D3DXMATRIX * )( &view ));
	}

	if (mKnownSemantics[ES_VIEWPROJECTION])
	{
		mDXEffectPtr->SetMatrix (mKnownSemantics[ES_VIEWPROJECTION], ( const D3DXMATRIX * )( &Renderer::Instance ().GetVP ()));
	}

	if (mKnownSemantics[ES_PROJECTION])
	{
		Renderer::Instance ().GetTransform (D3DTS_PROJECTION, proj);
		mDXEffectPtr->SetMatrix (mKnownSemantics[ES_PROJECTION], ( const D3DXMATRIX * )( &proj ));
	}
#endif
}

void Effect::RenderEffect( const boost::function0< void >& render_fn, DWORD flags ) const
{
	if( mDXEffectPtr )
	{
		SetupEffectData( );


		//
		// Render
		//

		UINT num_passes = 0;

		mDXEffectPtr->Begin( &num_passes, flags );

		for( UINT i = 0; i < num_passes; i++ )
		{
#if (__XFX_DIRECTX_VER__ < 9)
			mDXEffectPtr->Pass( i );
#else
			mDXEffectPtr->BeginPass (i);
#endif
			render_fn( );
#if (__XFX_DIRECTX_VER__ >= 9)
			mDXEffectPtr->EndPass ();
#endif
		}

		mDXEffectPtr->End( );
	}
}

HRESULT Effect::LoadFile( const String& filename )
{
	static const char * sExtensions[ ] = { ".fx", ".fxo", "" };

	const char ** ext = sExtensions;

	String new_filename( filename );
	bool found = false;

	String::size_type dot_pos = filename.find_last_of( '.' );
	if( ( dot_pos < filename.size( ) ) && ( dot_pos > filename.find_last_of( '\\' ) ) )
	{
		new_filename = filename;
		found = true;
	}

	while( **ext && !found )
	{
		if( SUCCEEDED( FileSystem::Instance( ).FindFile( new_filename = filename + *ext ) ) )
			found = true;

		ext++;
	}

	return Resource::LoadFile( new_filename );
}

HRESULT Effect::LoadMemory( const void *pmemory, unsigned long filelen )
{
	Cleanup( );

	HRESULT hr;

	ID3DXEffect * eff = NULL;

	XFX_PLACE_DEVICE_LOCK;

	LPD3DXBUFFER err_buf = NULL;

#if( __XFX_DIRECTX_VER__ < 9 )
	if( FAILED( hr = D3DXCreateEffect( Renderer::Instance( ).pD3DDevice( ), pmemory, filelen, &eff, &err_buf ) ) )
#else
	if( FAILED( hr = D3DXCreateEffect( Renderer::Instance( ).pD3DDevice( ), pmemory, filelen, NULL, NULL, 0, NULL, &eff, &err_buf ) ) )
#endif
	{
		boost::shared_ptr< ID3DXBuffer > err_buf_ptr( err_buf, IUnknownDeleter( ) );
		if( err_buf )
			gError( "Effect creation error: %s", err_buf->GetBufferPointer( ) );

		return hr;
	}

	mDXEffectPtr.reset( eff, IUnknownDeleter( ) );

#if (__XFX_DIRECTX_VER__ < 9)
	D3DXTECHNIQUE_DESC desc;

	if (FAILED (hr = eff->FindNextValidTechnique (NULL, &desc)))
	{
		gError( "Can't find any valid technique for effect!" );
		return hr;
	}

	mValidTechnique = desc.Name;
	eff->SetTechnique (mValidTechnique.c_str ());

#else
	if (FAILED (hr = eff->FindNextValidTechnique (NULL, &mValidTechnique)))
	{
		gError( "Can't find any valid technique for effect!" );
		return hr;
	}

	eff->SetTechnique (mValidTechnique);
#endif

#if (__XFX_DIRECTX_VER >= 9)
	int sem = 0;

	for (boost::array<D3DXHANDLE, ES_TOTAL>::iterator it = mKnownSemantics.begin (); it != mKnownSemantics.end (); it++)
		*it = eff->GetParameterBySemantic (NULL, GetSemanticName (sem++));
#endif

	mInvalidateConnection = Renderer::Instance( ).AddInvalidateCallback( boost::bind( &Effect::InvalidateCallback, this, _1 ) );
	return S_OK;
}

const char * Effect::GetSemanticName( const int& sem ) const
{
	static boost::array< const char *, ES_TOTAL > sem_names =
	{
		"WORLD",
		"WORLDVIEW",
		"WORLDVIEWPROJECTION",

		"VIEW",
		"VIEWPROJECTION",

		"PROJECTION"
	};

	_ASSERTE( sem >= 0 || sem < ES_TOTAL );
	return sem_names[ sem ];
};





//
// Shader
//

Cache< Shader > Shader::msCache;
int Shader::msID( 0 );

Shader::Shader( ) : ScriptResource( "Shader" ), mID( msID++ ), 
	mFlags( ESF_USE_TEXTURE_TRANSFORM )
{
	AddToken( "shader",		&ParseUselessToken );
	AddToken( "{",			&ParseUselessToken );
	AddToken( "map",		boost::bind( &Shader::ParseMap, this, _1, _2 ) );
	AddToken( "cubemap",	boost::bind( &Shader::ParseCubeMap, this, _1, _2 ) );
	AddToken( "animmap",	boost::bind( &Shader::ParseAnimMap, this, _1, _2 ) );
	AddToken( "effect",		boost::bind( &Shader::ParseEffect, this, _1, _2 ) );
	AddToken( "save_rs",	boost::bind( &Shader::ParseSaveRS, this, _1, _2 ) );
	AddToken( "}",			boost::bind( &Shader::ParseEndToken, this, _1, _2 ) );
}

Shader::~Shader( )
{
}

Shader::Shader( const Shader& shd ) : ScriptResource( "Shader" ), mID( msID++ )
{
	CopyShader( shd );
}

Shader& Shader::operator = ( const Shader& shd )
{
	CopyShader( shd );
	return *this;
}

void Shader::CopyShader( const Shader& shd )
{
	mNamedTextureMaps = shd.mNamedTextureMaps;
	mEffectPtr = shd.mEffectPtr;
	mTextureMaps = shd.mTextureMaps;
	mFlags = shd.mFlags;

	FindTextureStagesNum( );
}

void Shader::RemoveTextureMap( const String& name )
{
	mNamedTextureMaps.erase( name );

	TextureMapsType::iterator tex_it = 
		std::find_if( 
			mTextureMaps.begin( ),
			mTextureMaps.end( ),
			boost::bind( std::equal_to< String >( ), boost::bind( &TextureMapsType::value_type::get< 0 >, _1 ), boost::cref( name ) )
			);

	if( tex_it != mTextureMaps.end( ) )
		mTextureMaps.erase( tex_it );

	FindTextureStagesNum( );
}

int Shader::AddTextureMap( const TextureInfo& tex_info )
{
	mNamedTextureMaps[ tex_info.stage_name ] = tex_info.texture;

#if (__XFX_DIRECTX_VER__ < 9)
	TextureMapsType::iterator tex_it = 
		std::find_if( 
			mTextureMaps.begin( ),
			mTextureMaps.end( ),
			boost::bind( std::equal_to< String >( ), boost::bind( &TextureMapsType::value_type::get< 0 >, _1 ), boost::cref( tex_info.stage_name ) )
			);
#else
	D3DXHANDLE th = mEffectPtr ? mEffectPtr->DXEffectPtr( )->GetParameterByName( NULL, tex_info.stage_name.c_str( ) ) : NULL;

	TextureMapsType::iterator tex_it = 
		std::find_if( 
			mTextureMaps.begin( ),
			mTextureMaps.end( ),
			boost::bind( std::equal_to< D3DXHANDLE >( ), boost::bind( &TextureMapsType::value_type::get< 0 >, _1 ), boost::cref( th ) )
			);
#endif

	if( tex_it == mTextureMaps.end( ) )
	{
#if (__XFX_DIRECTX_VER__ < 9)
		mTextureMaps.push_back( boost::make_tuple( tex_info.stage_name, -1, tex_info ) );
#else
		mTextureMaps.push_back( boost::make_tuple( th, -1, tex_info ) );
#endif
		FindTextureStagesNum( );

		return mTextureMaps.back( ).get< 1 >( );
	}

	( *tex_it ).get< 2 >( ) = tex_info;

	if( ( *tex_it ).get< 1 >( ) >= 0 )
		mTextureStages[ ( *tex_it ).get< 1 >( ) ] = &( *tex_it ).get< 2 >( );

	return ( *tex_it ).get< 1 >( );
}

void Shader::SetEffectPtr( const boost::shared_ptr< const Effect >& eff )
{
	mEffectPtr = eff;
	FindTextureStagesNum( );
}

void Shader::FindTextureStagesNum( )
{
	//
	// setup texture stages
	//

	if( mEffectPtr )
	{
		XFX_PLACE_DEVICE_LOCK;

		SetEffectTextures( mEffectPtr );

		UINT num_passes = 0;

		mEffectPtr->DXEffectPtr( )->Begin( &num_passes, 0 );

		for( UINT i = 0; i < num_passes; i++ )
		{
			DWORD fvf = 0;

#if (__XFX_DIRECTX_VER__ < 9)
			mEffectPtr->DXEffectPtr( )->Pass( i );
#else
			mEffectPtr->DXEffectPtr( )->BeginPass (i);
#endif

			// don't apply texture transform for TnL vertices
#if (__XFX_DIRECTX_VER__ < 9)
			if( SUCCEEDED( Renderer::Instance( ).pD3DDevice( )->GetVertexShader( &fvf ) ) )
#else
			if( SUCCEEDED( Renderer::Instance( ).pD3DDevice( )->GetFVF( &fvf ) ) )
#endif
				if( fvf & D3DFVF_XYZRHW )
				{
					mFlags &= ~ESF_USE_TEXTURE_TRANSFORM;
				}

			//render_fn ();
#if (__XFX_DIRECTX_VER__ >= 9)
			mEffectPtr->DXEffectPtr( )->EndPass ();
#endif

			// find texture on texture stage and apply texture matrix
			BOOST_FOREACH( TextureMapsType::value_type& tex_ptr, mTextureMaps )
			{
				for( int stage = 0; stage < Renderer::MAX_TEXTURE_STAGES; stage++ )
				{
#if( __XFX_DIRECTX_VER__ < 9 )
					IDirect3DBaseTexture8 * stage_tex = NULL;
#else
					IDirect3DBaseTexture9 * stage_tex = NULL;
#endif

					if( SUCCEEDED( Renderer::Instance( ).pD3DDevice( )->GetTexture( stage, &stage_tex ) ) )
					{
						if( stage_tex )
						{
							boost::shared_ptr< IUnknown > stage_tex_ptr( stage_tex, IUnknownDeleter( ) );

							// Is this equality valid?
							if( stage_tex == tex_ptr.get< 2 >( ).texture->D3DTex( ) )
							{
								tex_ptr.get< 1 >( ) = stage;
								mTextureStages[ stage ] = &tex_ptr.get< 2 >( );

								break;
							}
						}
					}
				}
			}
		}

		mEffectPtr->DXEffectPtr( )->End ();
	}
}

void Shader::SetEffectTextures( const boost::shared_ptr< const Effect >& eff_ptr ) const
{
	for( TextureMapsType::const_iterator it = mTextureMaps.begin( );
		it != mTextureMaps.end( );
		it++ )
	{
		if( (*it).get< 2 >( ).texture )
		{
#if ( __XFX_DIRECTX_VER__ < 9 )
			eff_ptr->DXEffectPtr( )->SetTexture( ( *it ).get< 0 >( ).c_str( ), ( *it ).get< 2 >( ).texture->D3DTex( ) );
#else
			eff_ptr->DXEffectPtr( )->SetTexture( ( *it ).get< 0 >( ), ( *it ).get< 2 >( ).texture->D3DTex( ) );
#endif
		}

		if( mFlags & ESF_USE_TEXTURE_TRANSFORM )
		{
			int stage = ( *it ).get< 1 >( );
			if( stage != -1 )
			{
				// Apply texture matrix
				Renderer::Instance( ).pD3DDevice( )->SetTransform( D3DTRANSFORMSTATETYPE( D3DTS_TEXTURE0 + stage ), ( D3DMATRIX * )( &( *it ).get< 2 >( ).texture->TextureMatrix( ) ) );
				Renderer::Instance( ).pD3DDevice( )->MultiplyTransform( D3DTRANSFORMSTATETYPE( D3DTS_TEXTURE0 + stage ), ( D3DMATRIX * )( &( *it ).get< 2 >( ).texture->Transformation( ) ) );

#pragma message ( "FIXME: apply animation texture matrix" )
			}
		}
	}
}

void Shader::RenderEffect( const ShaderParams& consts, const boost::function0< void >& render_fn ) const
{
	if( mEffectPtr )
	{
		//
		// Set textures
		//

		SetEffectTextures( mEffectPtr );

		consts.ApplyValues( *mEffectPtr );

		mEffectPtr->RenderEffect( render_fn, ( mFlags & ESF_RESTORE_STATES ) ? 0 : D3DXFX_DONOTSAVESTATE );
	}
}

boost::shared_ptr< const ITexture > Shader::TextureMap( const String& name ) const
{
	NameTextureMapsType::const_iterator it = mNamedTextureMaps.find( name );

	if( it == mNamedTextureMaps.end( ) )
		return boost::shared_ptr< const ITexture >( );

	return ( *it ).second;
}

HRESULT Shader::PrepareParsing( )
{
	mLoadingInfoPtr.reset( new LoadingMapsContainer( ) );

	mNamedTextureMaps.clear ();
	mTextureMaps.clear ();
	mEffectPtr.reset ();
	mFlags = ESF_USE_TEXTURE_TRANSFORM;

	mTextureStages.assign( NULL );

	return S_OK;
}

HRESULT Shader::AfterParsing ()
{
	_ASSERTE( mLoadingInfoPtr );

#if ( __XFX_DIRECTX_VER__ < 9 )

	mTextureMaps.reserve( mLoadingInfoPtr->size( ) );

#endif

	BOOST_FOREACH( LoadingMapsContainer::value_type& tli, *mLoadingInfoPtr )
	{
		mNamedTextureMaps.insert( std::make_pair( tli.stage_name, tli.texture ) );

#if ( __XFX_DIRECTX_VER__ < 9 )

		mTextureMaps.push_back( boost::make_tuple( tli.stage_name, -1, tli ) );

#else

		if( mEffectPtr )
		{
			D3DXHANDLE h = mEffectPtr->DXEffectPtr( )->GetParameterByName( NULL, tli.stage_name.c_str( ) );

			if (h != NULL)
				mTextureMaps.push_back ( boost::make_tuple( h, -1, tli ) );
			else
				gWarn ("Invalid texture name \"%s\" in shader file \"%s\"!", tli.stage_name.c_str (), Filename ().c_str ());
		}

#endif
	}

	FindTextureStagesNum( );

	mLoadingInfoPtr.reset( );

	return S_OK;
}

HRESULT Shader::ParseMap( String::size_type& pos, const String& str )
{
	String stage_name;
	ParseVariable( stage_name, pos, str );

	const boost::shared_ptr< const ITexture >& tex = Texture::Cache( ).Register( next_token( str, pos ) );

	if( tex )
	{
		TextureInfo tli;
		tli.stage_name = stage_name;
		tli.texture = tex;
		tli.frame_width = 1.0f;
		tli.frame_height = 1.0f;
		tli.frame_count = 1;
		tli.fps = 1;

		mLoadingInfoPtr->push_back( tli );
		return S_OK;
	}

	return XFXERR_IO;
}

HRESULT Shader::ParseCubeMap( String::size_type& pos, const String& str )
{
	String stage_name;
	ParseVariable( stage_name, pos, str );

	const boost::shared_ptr< const ITexture >& tex = CubemapTexture::Cache( ).Register( next_token( str, pos ) );

	if( tex )
	{
		TextureInfo tli;
		tli.stage_name = stage_name;
		tli.texture = tex;
		tli.frame_width = 1.0f;
		tli.frame_height = 1.0f;
		tli.frame_count = 1;
		tli.fps = 1;

		mLoadingInfoPtr->push_back( tli );
		return S_OK;
	}

	return XFXERR_IO;
}

HRESULT Shader::ParseAnimMap( String::size_type& pos, const String& str )
{
	String stage_name;
	ParseVariable( stage_name, pos, str );

	const boost::shared_ptr< const ITexture >& tex = Texture::Cache( ).Register( next_token( str, pos ) );

	if( tex )
	{
		float frame_width = 1.0f, frame_height = 1.0f;
		int frame_count = 1, fps = 1;

		ParseVariable( frame_width, pos, str );
		ParseVariable( frame_height, pos, str );
		ParseVariable( frame_count, pos, str );
		ParseVariable( fps, pos, str );

		TextureInfo tli;
		tli.stage_name = stage_name;
		tli.texture = tex;
		tli.frame_width = frame_width;
		tli.frame_height = frame_height;
		tli.frame_count = frame_count;
		tli.fps = fps;

		mLoadingInfoPtr->push_back( tli );
		return S_OK;
	}

	return XFXERR_IO;
}

HRESULT Shader::ParseEffect (String::size_type& pos, const String& str)
{
	mEffectPtr = Effect::Cache( ).Register( next_token( str, pos ) );

	return mEffectPtr ? S_OK : XFXERR_IO;
}

HRESULT Shader::ParseSaveRS( String::size_type& pos, const String& str )
{
	int val;
	ParseVariable( val, pos, str );

	if( val )
		mFlags |= ESF_RESTORE_STATES;
	else
		mFlags &= ~ESF_RESTORE_STATES;

	return S_OK;
}





//
// ShaderParams
//

void ShaderParams::ApplyValues( const Effect& eff ) const
{
	if( !eff.DXEffectPtr( ) )
		return;

	for( stdext::hash_map< String, boost::shared_ptr< const ITexture >, HashCompare< String > >::const_iterator t_it = mTextures.begin( ); t_it != mTextures.end( ); t_it++ )
	{
		const boost::shared_ptr< const ITexture >& tex_ptr = ( *t_it ).second;

		eff.DXEffectPtr( )->SetTexture( eff.GetHandle( ( *t_it ).first ), tex_ptr ? tex_ptr->D3DTex( ) : NULL );
	}

	for( stdext::hash_map< String, float, HashCompare< String > >::const_iterator f_it = mFloats.begin( ); f_it != mFloats.end( ); f_it++ )
	{
		eff.DXEffectPtr( )->SetFloat( eff.GetHandle( ( *f_it ).first ), ( *f_it ).second );
	}

	for( stdext::hash_map< String, int, HashCompare< String > >::const_iterator i_it = mInts.begin( ); i_it != mInts.end( ); i_it++ )
	{
#if ( __XFX_DIRECTX_VER__ < 9 )
		eff.DXEffectPtr( )->SetDword( eff.GetHandle( ( *i_it ).first ), static_cast< DWORD >( ( *i_it ).second ) );
#else
		eff.DXEffectPtr( )->SetInt( eff.GetHandle( ( *i_it ).first ), ( *i_it ).second );
#endif
	}

	for( stdext::hash_map< String, Vec4, HashCompare< String > >::const_iterator v_it = mVectors.begin( ); v_it != mVectors.end( ); v_it++ )
	{
		eff.DXEffectPtr( )->SetVector( eff.GetHandle( ( *v_it ).first ), ( D3DXVECTOR4 * )( &( *v_it ).second ) );
	}

	for( stdext::hash_map< String, Mat4, HashCompare< String > >::const_iterator m_it = mMatrices.begin( ); m_it != mMatrices.end( ); m_it++ )
	{
		eff.DXEffectPtr( )->SetMatrix( eff.GetHandle( ( *m_it ).first ), ( D3DXMATRIX * )( &( *m_it ).second ) );
	}
}




_XFX_END
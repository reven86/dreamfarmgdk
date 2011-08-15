//  File xfx_font.cpp                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_font.h"
#include "xfx_texture.h"
#include "xfx_shader.h"

_XFX_BEGIN






//
// Fond class
//

Cache< Font > Font::msCache;

//-----------------------------------------------------------------------------
//                           Methods
//-----------------------------------------------------------------------------
Font::Font () :
	ScriptResource( "Font" ),
	mCharHeight( 0 ),
	mCharMapString( ),
	mShaderPtr( ),
	mTextureWidth( 1 ),
	mTextureHeight( 1 )
{
	AddToken( "font",			&ParseUselessToken );
	AddToken( "{",				&ParseUselessToken );
	AddToken( "height",			boost::bind( &Font::ParseHeight, this, _1, _2 ) );
	AddToken( "shader",			boost::bind( &Font::ParseShader, this, _1, _2 ) );
	AddToken( "charmap",		boost::bind( &Font::ParseCharMap, this, _1, _2 ) );
	AddToken( "}",				boost::bind( &Font::ParseEndToken, this, _1, _2 ) );
}

Font::~Font( )
{
}

HRESULT Font::Create( unsigned height, const boost::shared_ptr< const Shader >& shd, const WString& charmapstr )
{
	mShaderPtr = shd;

	const boost::shared_ptr< const Shader >& shd_ptr		= mShaderPtr;
	const boost::shared_ptr< const ITexture >& tex_ptr		= shd_ptr ? shd_ptr->GetTextureMap( "FontTex" ) : boost::shared_ptr< const ITexture >( );

	const Texture * tex = tex_ptr ? boost::polymorphic_downcast< const Texture *, const ITexture >( tex_ptr.get( ) ) : NULL;
	if( tex && !charmapstr.empty( ) )
	{
		mTextureWidth	= tex->GetWidth ();
		mTextureHeight	= tex->GetHeight ();

		mCharHeight		= height;
		mCharMapString	= charmapstr;

		unsigned x = 0;
		unsigned y = 0;

		HRESULT hr;

		boost::scoped_array<ARGB> buf (new ARGB[tex->GetWidth () * tex->GetHeight ()]);

		hr = tex->GetSurfaceData (0, buf.get ());

		if (FAILED (hr))
			return hr;

		//Line above chars describes chars width
		ARGB * pscanline	= buf.get ();
		ARGB * poldscanline	= pscanline;
		ARGB breakcolor		= *pscanline;
		breakcolor.dword	&= 0x00ffffff;

		for (WString::const_iterator cmit = charmapstr.begin (); cmit != charmapstr.end (); cmit++)
			if (*cmit == '\n')
			{
				pscanline += tex->GetWidth () * height;
				x = 0;
				y += height;

				if (y >= tex->GetHeight ())
					return XFXERR_INVALIDCALL;

				poldscanline = pscanline;
			}
			else if (*cmit != '\r')
			{
				ARGB * pnewscanline = poldscanline + 1;
				unsigned width = 1;

				while ((pnewscanline->dword & 0x00ffffff) != breakcolor.dword && x + width < tex->GetWidth ())
				{
					pnewscanline++;
					width++;
				}

				poldscanline = pnewscanline;

				CharRectType cr;
				cr.x1 = x;
				cr.y1 = y + 1;
				cr.x2 = x + width;
				cr.y2 = y + height + 1;

				mCharMap[*cmit] = cr;

				x += width;
			}

		return S_OK;
	}

	return XFXERR_INVALIDCALL;
}

HRESULT Font::Create( const boost::shared_ptr< const Shader >& shd, const CharMapType& char_map )
{
	mShaderPtr = shd;
	mCharMap = char_map;
	mCharMapString.clear( );

	mCharHeight = 0;

	BOOST_FOREACH( const CharMapType::value_type& c, char_map )
	{
		mCharMapString.push_back( c.first );

		unsigned h = c.second.y2 - c.second.y1;

		if( h > mCharHeight )
			mCharHeight = h;
	}

	return S_OK;
}

HRESULT Font::PrepareParsing( )
{
	mLoadInfoPtr.reset( new LoadInfo );
	mLoadInfoPtr->height = 0;

	return S_OK;
}

HRESULT Font::AfterParsing( )
{
	if( !mLoadInfoPtr )
		return XFXERR_INVALIDCALL;

	// get font folder and temporary add it as search path
	String fname = PhysicalPath( );
	String font_folder = fname.substr( 0, fname.find_last_of( "\\/" ) + 1 );
	bool revert = FileSystem::Instance( ).AddSearchPath( font_folder, FileSystem::ESPP_LOW );

	HRESULT hr = Create( mLoadInfoPtr->height, Shader::Cache( ).Register( mLoadInfoPtr->shader ), fromUTF8( mLoadInfoPtr->char_map ) );

	if( revert )
		FileSystem::Instance( ).RemoveSearchPath( font_folder );

	mLoadInfoPtr.reset( );
	return hr;
}

HRESULT Font::ParseHeight( String::size_type& pos, const String& str )
{
	if( !mLoadInfoPtr )
		return XFXERR_INVALIDCALL;

	return ParseVariable( mLoadInfoPtr->height, pos, str );
}

HRESULT Font::ParseShader( String::size_type& pos, const String& str )
{
	if( !mLoadInfoPtr )
		return XFXERR_INVALIDCALL;

	return ParseVariable( mLoadInfoPtr->shader, pos, str );
}

HRESULT Font::ParseCharMap( String::size_type& pos, const String& str )
{
	if( !mLoadInfoPtr )
		return XFXERR_INVALIDCALL;

	return ParseVariable( mLoadInfoPtr->char_map, pos, str );
}




_XFX_END
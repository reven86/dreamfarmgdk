//  File xfx_font.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_font.h 
 *	\brief %Font class (supports unicode).
 */

_XFX_BEGIN





/*! \class Font xfx_font.h "main/xfx_font.h"
 *	\brief %Font class.
 *	\ingroup RenderGroup
 *	
 *	%Font is loaded from special script file. This script file defines:
 *	- texture where all characters must have equal height, all characters located in rows. Top-left pixel on texture
 *	  is a "break color". Width of each character is calculated from pixel line above each row where "break color" set
 *	  before each character in a row;
 *	- height of each character;
 *	- character map that describes character location on texture
 *
 *	Texture in shader, which stores "break color" pixels must have name "FontTex".
 *
 *	%Font file syntax can be found here: \ref font_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section font_script Font file syntax
 *
 *	Example:
 *
 *	\verbatim
font
{
	height 14

	charmap 
" !\"#$%&'()*+,-./
0123456789:;<=>?
@ABCDEFGHIJKLMNO
PQRSTUVWXYZ[\\]^_
`abcdefghijklmno
pqrstuvwxyz{|}~"

	shader "font\system.font.shader"
}
	\endverbatim
 *	
 *	Description:
 *
 *	\b height		-	Height of all charecters in the font. \n
 *	\b charmap		-	Character map. \n
 *	\b shader		-	%Font shader file name.
 *
 *	\see Font, Shader
 *
 *	@}
 */

class Font : public ScriptResource
{
public:
	//! Character position on texture
	struct CharRectType
	{
		int x1;
		int y1;
		int x2;
		int y2;

		CharRectType( ) : x1( 0 ), x2( 0 ), y1( 0 ), y2( 0 ) { };
	};

	//! Type of character map
	typedef stdext::hash_map< wchar_t, CharRectType > CharMapType;

private:
	boost::shared_ptr< const class Shader >	mShaderPtr;

	CharMapType								mCharMap;
	WString									mCharMapString;
	unsigned								mCharHeight;

	unsigned								mTextureWidth;
	unsigned								mTextureHeight;

	//
	// Temporary loading data
	//

	struct LoadInfo
	{
		unsigned							height;
		String								shader;
		String								char_map;
	};

	boost::scoped_ptr< LoadInfo >			mLoadInfoPtr;

	static Cache< Font >					msCache;

public:
	Font																	( );
	virtual ~Font															( );

	//copy constructor and assignment operators are fine

	/*!	\brief %Font creation.
	 *	
	 *	Creates font by height, Shader and charactes.
	 *	FontTex in shader should be prepared as described in Font description. 
	 *
	 *	\param[in] height		Height of each character.
	 *	\param[in] shd			Shader with texture on stage "FontTex".
	 *	\param[in] charmapstr	Characters on texture.
	 *
	 *	\return Returns S_OK if font has been created sucessfully.
	 */
	virtual HRESULT						Create								( unsigned height, const boost::shared_ptr< const Shader >& shd, const WString& charmapstr );

	/*! \brief Create %Font Shader and character map
	 *
	 *	Creates font by shader and character map.
	 *
	 *	\param[in]	shd			Shader with texture on stage "FontTex".
	 *	\param[in]	char_map	Character map.
	 *
	 *	\return Returns S_OK of fond has been created sucessfully.
	 */
	virtual HRESULT						Create								( const boost::shared_ptr< const Shader >& shd, const CharMapType& char_map );

	//! Returns fonts cache.
	static Cache< Font >&				Cache								( ) { return msCache; };

	//! Returns font shader.
	const boost::shared_ptr< const class Shader >&	ShaderPtr				( ) const { return mShaderPtr; };
	//! Returns character map.
	const WString&						CharMapString						( ) const { return mCharMapString; };
	//! Returns character height.
	const unsigned&						CharHeight							( ) const { return mCharHeight; };

	//! Returns information about each character on texture.
	const CharMapType&					CharMap								( ) const { return mCharMap; };

	//! Returns texture width.
	const unsigned&						TextureWidth						( ) const { return mTextureWidth; };
	//! Returns texture height.
	const unsigned&						TextureHeight						( ) const { return mTextureHeight; };

	/*!	\brief Format string by special width.
	 *	
	 *	For each char in incoming string applies visitor \a 'v' which takes:
	 *	- x-center of char sprite;
	 *	- y-center of char sprite;
	 *	- width of char sprite;
	 *	- height of char sprite;
	 *	- u1, v1, u2, v2 - texture coordinates;
	 *
	 *	\param[in] str			Incoming string.
	 *	\param[in] len			Length of incoming string (for partial processing).
	 *	\param[in] v			Visitor object.
	 *	\param[in] max_width	Max width of formatted text (if > 0)
	 *
	 *	\return Always returns S_OK.
	 */
	template< class _Visitor >
	HRESULT								FormatString						( const WString& str, size_t len, _Visitor v, float max_width = -1.0f ) const;

private:
	virtual HRESULT						PrepareParsing						( );
	virtual HRESULT						AfterParsing						( );

	HRESULT								ParseHeight							( String::size_type& pos, const String& str );
	HRESULT								ParseShader							( String::size_type& pos, const String& str );
	HRESULT								ParseCharMap						( String::size_type& pos, const String& str );
};






//
// Template methods
//

template< class _Visitor >
HRESULT Font::FormatString( const WString& text, size_t len, _Visitor v, float max_width ) const
{
	PROFILE( "xfx::Font::FormatString", "General" );

	const boost::shared_ptr< const Shader >& shd_ptr = ShaderPtr( );

	if( !shd_ptr )
		return XFXERR_INVALIDCALL;

	const wchar_t * end		= text.c_str( ) + len;
	const wchar_t * p		= text.c_str( );
	int temp;
	float dx = 0.0f;
	float dy = 0.0f;

	float _1_by_texw = 1.0f / TextureWidth( );
	float _1_by_texh = 1.0f / TextureHeight( );

	while( p < end )
	{
		temp = 0;
		while( ( p < end ) && ( *p == '\n' ) )
			p++, temp++;

		if( temp )
		{
			dy += static_cast< float >( temp ) * CharHeight( );
			dx = 0.0f;
		}

		while( ( p < end ) && ( *p != '\n' ) )
		{
			float w = 0.0f;
			float h = 0.0f;
			float u0 = 0.0f;
			float v0 = 0.0f;
			float u1 = 0.0f;
			float v1 = 0.0f;

			if( max_width > 0.0f )
			{
				// get next word width and test with max_width
				if( p > text.c_str( ) && *( p - 1 ) == ' ' && *p != ' ' )
				{
					const wchar_t * word_end = p;

					float word_w = 0.0f;
					while( word_end < end && *word_end != ' ' && *word_end != '\n' && *word_end != '\r' && *word_end != '\t' )
					{
						word_end++;

						Font::CharMapType::const_iterator it = CharMap( ).find( *word_end );

						if( it != CharMap( ).end( ) )
							word_w += static_cast< float >( ( *it ).second.x2 - ( *it ).second.x1 );
					}

					if( dx + word_w > max_width )
					{
						dy += CharHeight( );
						dx = 0.0f;
					}
				}
			}

			if( *p == '\t' )
			{
				// tab is equal to four spaces

				Font::CharMapType::const_iterator it = CharMap( ).find( ' ' );

				if( it != CharMap( ).end( ) )
				{
					w = 4.0f * static_cast< float >( ( *it ).second.x2 - ( *it ).second.x1 );
					h = static_cast< float >( ( *it ).second.y2 - ( *it ).second.y1 );
				}
			}
			else
			{
				Font::CharMapType::const_iterator it = CharMap( ).find( *p );

				if( it != CharMap( ).end( ) )
				{
					w = static_cast< float >( ( *it ).second.x2 - ( *it ).second.x1 );
					h = static_cast< float >( ( *it ).second.y2 - ( *it ).second.y1 );

					u0 = ( *it ).second.x1 * _1_by_texw;
					v0 = ( *it ).second.y1 * _1_by_texh;
					u1 = ( *it ).second.x2 * _1_by_texw;
					v1 = ( *it ).second.y2 * _1_by_texh;
				}
			}

			v( dx + w * 0.5f, dy + h * 0.5f, w, h, u0, v0, u1, v1 );

			dx += w;

			p++;
		}
	}

	return S_OK;
};



_XFX_END

//  File xfx_uilabel.cpp                                                      |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_uilabel.h"
#include "renderer\xfx_font.h"
#include "renderer\xfx_renderer.h"
#include "renderer\xfx_viewport.h"
#include "main\xfx_messagesbook.h"

_XFX_BEGIN





//
// UILabel
//

UILabel::UILabel( ) :
	mLx( 0.0f ),
	mLy( 0.0f ),
	mIsTrisCached( false ),
	mIsRealSize( false ),
	mShaderConstsPtr( ),
	mMaxWidth( -1 ),
	mXScale( 1.0f ),
	mYScale( 1.0f ),
	mAlign( EAM_LEFT )
{
	AddToken( "label.font",			boost::bind( &UILabel::ParseFont, this, _1, _2 ) );
	AddToken( "label.text",			boost::bind( &UILabel::ParseText, this, _1, _2 ) );
	AddToken( "label.max_width",	boost::bind( &ParseVariable< float >, boost::ref( mMaxWidth ), _1, _2 ) );
	AddToken( "label.real_size",	boost::bind( &ParseVariable< bool >, boost::ref( mIsRealSize ), _1, _2 ) );
	AddToken( "label.x_scale",		boost::bind( &ParseVariable< float >, boost::ref( mXScale ), _1, _2 ) );
	AddToken( "label.y_scale",		boost::bind( &ParseVariable< float >, boost::ref( mYScale ), _1, _2 ) );
	AddToken( "label.align",		boost::bind( &UILabel::ParseAlign, this, _1, _2 ) );
}

UILabel::~UILabel( )
{
}

void UILabel::Text( const WString& str )
{
	if( mCurText == str )
		return;

	mIsTrisCached = false;
	mCurText = str;

	CacheTris( );
}

void UILabel::CacheTris( )
{
	float kw = mIsRealSize ? static_cast< float >( Viewport::Instance( ).Width( ) ) / Renderer::Instance( ).D3DPP( ).BackBufferWidth : 1.0f;
	float kh = mIsRealSize ? static_cast< float >( Viewport::Instance( ).Height( ) ) / Renderer::Instance( ).D3DPP( ).BackBufferHeight : 1.0f;

	_ASSERTE( !mIsTrisCached );
	if( mCurText.empty( ) )
	{
		UIObject::Width		( 0.0f );
		UIObject::Height	( mFontPtr ? mFontPtr->CharHeight( ) * 0.5f * kh * mYScale : 0.0f );
		UIObject::X			( mLx );
		UIObject::Y			( mLy - UIObject::Height( ) );
		return;
	}

	if( !mFontPtr )
		return;

	size_t numtris = mCurText.size( );

	mCachedTris.resize( 0 );
	if( numtris > mCachedTris.capacity( ) )
		mCachedTris.reserve( numtris );

	float max_w = 0.0f;
	float max_h = 0.0f;

	struct visitor
	{
		std::vector< SpriteInfo >&			cont;
		float&								max_w;
		float&								max_h;
		float								kw;
		float								kh;

		visitor								( std::vector< SpriteInfo >& _c, float& _w, float& _h, float w, float h ) : cont( _c ), max_w( _w ), max_h( _h ), kw( w ), kh( h ) { };

		void operator ( )					( const float& ch_x, const float& ch_y, const float& w, const float& h, const float& u1, const float& v1, const float& u2, const float& v2 ) const
		{
			SpriteInfo i;
			i.x = ch_x * kw;
			i.y = -ch_y * kh;
			i.w = w * kw;
			i.h = h * kh;
			i.u1 = u1;
			i.v1 = v1;
			i.u2 = u2;
			i.v2 = v2;

			cont.push_back( i );

			if( ch_x + w * 0.5f > max_w )
				max_w = ch_x + w * 0.5f;

			if( ch_y + h * 0.5f > max_h )
				max_h = ch_y + h * 0.5f;
		};
	};

	mFontPtr->FormatString( mCurText, mCurText.length( ), visitor( mCachedTris, max_w, max_h, kw, kh ), mMaxWidth / mXScale );

	float old_w = Width( );

	UIObject::Width		( max_w * 0.5f * kw * mXScale );
	UIObject::Height	( max_h * 0.5f * kh * mYScale );

	switch( mAlign )
	{
	case EAM_LEFT:
		UIObject::X( mLx + Width( ) );
		break;

	case EAM_CENTER:
		mLx = X( ) - Width( );
		break;

	case EAM_RIGHT:
		PosX( X( ) + old_w - 2 * Width( ) );
		break;

	default:
		_ASSERTE( !"Invalid align mode" );
	}

	UIObject::Y( mLy - Height( ) );
}

void UILabel::Render( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const
{
	if( mCurText.empty( ) )
		return;

	if( !mFontPtr )
		return;

	ARGB newcolor = color * Color( );
	if( newcolor.a == 0 )
		return;

	float iw		= Viewport::Instance( ).Width( ) * 0.5f;
	float ih		= Viewport::Instance( ).Height( ) * 0.5f;

	float kw		= Viewport::Instance( ).InvWidth( ) * Renderer::Instance( ).D3DPP( ).BackBufferWidth;
	float kh		= Viewport::Instance( ).InvHeight( ) * Renderer::Instance( ).D3DPP( ).BackBufferHeight;

	for( std::vector< SpriteInfo >::const_iterator it = mCachedTris.begin( ); it != mCachedTris.end( ); it++ )
	{
		Renderer::Instance( ).DrawTools( ).PushDraw2DSprite(
			( ( ( *it ).x * mXScale + mLx ) * scale.x + iw + ofs.x ) * kw, 
			( ih - ( ( *it ).y * mYScale + mLy ) * scale.y - ofs.y ) * kh, 
			( *it ).w * mXScale * scale.x * kw, 
			( *it ).h * mYScale * scale.y * kh,
			0, newcolor, mFontPtr->ShaderPtr( ), mShaderConstsPtr, ( *it ).u1, ( *it ).v1, ( *it ).u2, ( *it ).v2
			);
	}
}

HRESULT UILabel::PrepareParsing( )
{
	mCurText.clear( );
	mCachedTris.clear( );
	mIsTrisCached = false;
	mXScale = 1.0f;
	mYScale = 1.0f;
	mAlign = EAM_LEFT;

	return UIObject::PrepareParsing( );
}

HRESULT UILabel::AfterParsing( )
{
	HRESULT hr = UIObject::AfterParsing( );

	X( UIObject::X( ) );
	Y( UIObject::Y( ) );

	CacheTris( );

	return hr;
}

HRESULT UILabel::ParseFont( String::size_type& pos, const String& str )
{
	mFontPtr = Font::Cache( ).Register( next_token( str, pos ) );

	return mFontPtr ? S_OK : XFXERR_IO;
}

HRESULT UILabel::ParseText( String::size_type& pos, const String& str )
{
	mCurText = BooksManager::Instance( ).GetEntry( next_token( str, pos ) ).text;

	return S_OK;
}

HRESULT UILabel::ParseAlign( String::size_type& pos, const String& str )
{
	String mode;
	HRESULT hr;

	if( FAILED( hr = ParseVariable( mode, pos, str ) ) )
		return hr;

	if( mode == "left" )
		mAlign = EAM_LEFT;
	else if ( mode == "center" )
		mAlign = EAM_CENTER;
	else if ( mode == "right" )
		mAlign = EAM_RIGHT;
	else if ( mode == "width" )
		mAlign = EAM_WIDTH;
	else
	{
		_ASSERTE( !"Invalid align mode for UILabel" );
		return XFXERR_INVALIDCALL;
	}

	return S_OK;
}

void UILabel::LuaRegister( lua_State * L )
{
	luabind::module( L )
	[
		luabind::class_< UILabel, UIObject >( "UILabel" )
			.enum_( "AlignMode" )
			[
				luabind::value( "EAM_LEFT", EAM_LEFT ),
				luabind::value( "EAM_CENTER", EAM_CENTER ),
				luabind::value( "EAM_RIGHT", EAM_RIGHT ),
				luabind::value( "EAM_WIDTH", EAM_WIDTH ),
				luabind::value( "EAM_TOTAL_MODES", EAM_TOTAL_MODES )
			]
			.property( "text",
				( const WString& ( UILabel::* ) ( ) const ) &UILabel::Text,
				( void ( UILabel::* ) ( const WString& ) ) &UILabel::Text
				)
			.property( "pos_x",
				( const float& ( UILabel::* ) ( ) const ) &UILabel::PosX,
				( void ( UILabel::* ) ( float ) ) &UILabel::PosX
				)
			.property( "pos_y",
				( const float& ( UILabel::* ) ( ) const ) &UILabel::PosY,
				( void ( UILabel::* ) ( float ) ) &UILabel::PosY
				)
			.property( "font",
				( const boost::shared_ptr< const Font >& ( UILabel::* ) ( ) const ) &UILabel::FontPtr,
				( void ( UILabel::* ) ( const boost::shared_ptr< const Font >& ) ) &UILabel::FontPtr
				)
			.property( "x_scale",
				( const float& ( UILabel::* ) ( ) const ) &UILabel::XScale,
				( void ( UILabel::* ) ( float ) ) &UILabel::XScale
				)
			.property( "y_scale",
				( const float& ( UILabel::* ) ( ) const ) &UILabel::YScale,
				( void ( UILabel::* ) ( float ) ) &UILabel::YScale
				)
			.property( "align_mode",
				( const enum AlignMode& ( UILabel::* ) ( ) const ) &UILabel::AlignMode,
				( void ( UILabel::* ) ( const enum AlignMode& ) ) &UILabel::AlignMode
				)
	];
}





_XFX_END
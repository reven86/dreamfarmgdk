//  File xfx_uiobject.cpp                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_uiedit.h"
#include "main\xfx_application.h"
#include "renderer\xfx_font.h"
#include "lua\xfx_luascript.h"

_XFX_BEGIN






//
// UIEdit
//

UIEdit::UIEdit( ) :
	mLocalTime( 0 ),
	mMaxChars( 65535 )
{
	AddToken( "edit.font",			boost::bind( &UIEdit::ParseFont, this, _1, _2 ) );
	AddToken( "edit.text",			boost::bind( &UIEdit::ParseText, this, _1, _2 ) );
	AddToken( "edit.real_size",		boost::bind( &UIEdit::ParseRealSize, this, _1, _2 ) );
	AddToken( "edit.x_scale",		boost::bind( &UIEdit::ParseXScale, this, _1, _2 ) );
	AddToken( "edit.y_scale",		boost::bind( &UIEdit::ParseYScale, this, _1, _2 ) );
	AddToken( "edit.max_chars",		boost::bind( &ParseVariable< DWORD >, boost::ref( mMaxChars ), _1, _2 ) );
}

UIEdit::~UIEdit( )
{
}

void UIEdit::Text( const WString& str )
{
	mText = str;
	PrepareText( );
}

void UIEdit::OnSetFocus( )
{
	UIObject::OnSetFocus( );
	PrepareText( );
}

void UIEdit::OnKillFocus( )
{
	UIObject::OnKillFocus( );
	PrepareText( );
}

void UIEdit::MaxChars( DWORD count )
{
	mMaxChars = count;
	PrepareText( );
}

void UIEdit::PrepareText( )
{
	if( mText.size( ) > mMaxChars )
		mText.resize( mMaxChars );

	mCursor.Text( HasFocus( ) ? L"|" : L"" );
	mLabel.Text( mText );

	float width = mLabel.Width( ) + mCursor.Width( );

	UIObject::Width( width );
	UIObject::Height( mLabel.Height( ) );

	UIObject::X( mLabel.PosX( ) + width );
	UIObject::Y( mLabel.Y( ) );

	mCursor.X( mLabel.X( ) + mLabel.Width( ) + mCursor.Width( ) );
	mCursor.Y( mLabel.Y( ) );
}

void UIEdit::ProcessKey( wchar_t key )
{
	switch( key )
	{
		/*case 0x03:	//Ctrl + C
			OpenClipboard		(gEngine.RenderWnd ());
			SetClipboardData	(CF_TEXT, (HANDLE) mText.c_str ());
			CloseClipboard		();
			break;*/

		case 0x08:	//Backspace
			if( mText.length( ) )
				mText.resize( mText.size( ) - 1 );
			break;

		case 0x09:	//Tab
			break;

		case 0x0a:	//Ctrl + Enter
		case 0x0d:	//Enter
			if( IsNamed( ) )
			{
				String event_str ( boost::str( StringFormat( "OnTextEntered_%s" ) % Name( ) ) );

#ifdef __XFX_USE_LUA_LUABIND__
				if( SUCCEEDED( LuaScript::Instance( ).IsFunction( event_str.c_str( ) ) ) )
				{
					try
					{
						luabind::call_function< void >(
							LuaScript::Instance( ).GetLuaVM( ),
							event_str.c_str( ),
							this );
					}
					catch( std::exception& e )
					{
						gError( "Call to lua function %s failed, reason: %s", event_str.c_str( ), e.what( ) );
					}
					catch( ... )
					{
						gError( "Call to lua function %s failed, reason: unknown", event_str.c_str( ) );
					}
				}
#endif
			}
			return;

		case 0x16:	//Ctrl + V
			{
				OpenClipboard( gGetApplication( ).hWnd( ) );
				wchar_t * pclipboard = reinterpret_cast< wchar_t * >( GetClipboardData( CF_UNICODETEXT ) );
				CloseClipboard( );

				if( pclipboard != NULL )
				{
					WString clipboard_text( pclipboard );

					size_t len = clipboard_text.size( );

					for( size_t i = 0; i < len; i++)
					{
						if( !HasFocus( ) )
							break;

						if( clipboard_text[ i ] != 0x0d )
							ProcessKey( clipboard_text[ i ] );
					}
				}
			}
			break;

		/*case 0x24:	//Ctrl + X
			OpenClipboard		(gEngine.RenderWnd ());
			SetClipboardData	(CF_TEXT, (HANDLE) mText.c_str ());
			CloseClipboard		();

			mText.clear ();
			break;*/

		default:
			mText.push_back( key );
	}

	if( IsNamed( ) )
	{
		String event_str ( boost::str( StringFormat( "OnTextChanged_%s" ) % Name( ) ) );

#ifdef __XFX_USE_LUA_LUABIND__
		if( SUCCEEDED( LuaScript::Instance( ).IsFunction( event_str.c_str( ) ) ) )
		{
			try
			{
				luabind::call_function< void >(
					LuaScript::Instance( ).GetLuaVM( ),
					event_str.c_str( ),
					this );
			}
			catch( std::exception& e )
			{
				gError( "Call to lua function %s failed, reason: %s", event_str.c_str( ), e.what( ) );
			}
			catch( ... )
			{
				gError( "Call to lua function %s failed, reason: unknown", event_str.c_str( ) );
			}
		}
#endif
	}

	PrepareText( );
}

void UIEdit::Render( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const
{
	ARGB newcolor = color * Color( );
	if( newcolor.a == 0 )
		return;

	mLabel.Render( newcolor, ofs, scale );
	mCursor.Render( newcolor, ofs, scale );
}

void UIEdit::Update( DWORD mspf )
{
	mLocalTime += mspf;

	mLabel.Update( mspf );
	mCursor.Update( mspf );

	if( HasFocus( ) )
	{
		Math::BigAngle angle = static_cast< Math::BigAngle >( Math::pi * mLocalTime * 0.0004f );

		mCursor.Color( ARGB( static_cast< unsigned char >( 255.0f * ( 0.5f + 0.5f * math_sin( angle ) ) ), 255, 255, 255 ) );
	}
}

void UIEdit::X( float x )
{
	float delta_x = x - UIObject::X( );

	UIObject::X( x );
	mLabel.X( mLabel.X( ) + delta_x );
	mCursor.X( mCursor.X( ) + delta_x );
}

void UIEdit::Y( float y )
{
	float delta_y = y - UIObject::Y( );

	UIObject::Y( y );
	mLabel.Y( mLabel.Y( ) + delta_y );
	mCursor.Y( mCursor.Y( ) + delta_y );
}

void UIEdit::PosX( float lx )
{
	float delta_x = lx - mLabel.PosX( );

	mLabel.PosX( lx );

	UIObject::X( UIObject::X( ) + delta_x );
	mCursor.X( mCursor.X( ) + delta_x );
}

void UIEdit::PosY( float ly )
{
	float delta_y = ly - mLabel.PosY( );

	mLabel.PosY( ly );

	UIObject::Y( UIObject::Y( ) + delta_y );
	mCursor.Y( mCursor.Y( ) + delta_y );
}

HRESULT UIEdit::AfterParsing( )
{
	UIObject::AfterParsing( );

	return S_OK;
}

HRESULT UIEdit::ParseFont( String::size_type& pos, const String& str )
{
	boost::shared_ptr< const Font > fnt = Font::Cache( ).Register( next_token( str, pos ) );

	mLabel.FontPtr( fnt );
	mCursor.FontPtr( fnt );

	return fnt ? S_OK : XFXERR_IO;
}

HRESULT UIEdit::ParseText( String::size_type& pos, const String& str )
{
	Text( fromUTF8( next_token( str, pos ) ) );

	return S_OK;
}

HRESULT UIEdit::ParseRealSize( String::size_type& pos, const String& str )
{
	bool rs = false;

	HRESULT hr;

	if( FAILED( hr = ParseVariable< bool >( rs, pos, str ) ) )
		return hr;

	IsRealSize( rs );

	return S_OK;
}

HRESULT UIEdit::ParseXScale( String::size_type& pos, const String& str )
{
	HRESULT hr;
	float scale;

	hr = ParseVariable< float >( scale, pos, str );
	if( FAILED( hr ) )
		return hr;

	XScale( scale );

	return hr;
}

HRESULT UIEdit::ParseYScale( String::size_type& pos, const String& str )
{
	HRESULT hr;
	float scale;

	hr = ParseVariable< float >( scale, pos, str );
	if( FAILED( hr ) )
		return hr;

	YScale( scale );

	return hr;
}

void UIEdit::LuaRegister( lua_State * L )
{
#ifdef __XFX_USE_LUA_LUABIND__
	luabind::module( L )
	[
		luabind::class_< UIEdit, UIObject >( "UIEdit" )
			.property( "text",
				( const WString& ( UIEdit::* ) ( ) const ) &UIEdit::Text,
				( void ( UIEdit::* ) ( const WString& ) ) &UIEdit::Text
				)
			.property( "pos_x",
				( const float& ( UIEdit::* ) ( ) const ) &UIEdit::PosX,
				( void ( UIEdit::* ) ( float ) ) &UIEdit::PosX
				)
			.property( "pos_y",
				( const float& ( UIEdit::* ) ( ) const ) &UIEdit::PosY,
				( void ( UIEdit::* ) ( float ) ) &UIEdit::PosY
				)
			.property( "font",
				( const boost::shared_ptr< const Font >& ( UIEdit::* ) ( ) const ) &UIEdit::FontPtr,
				( void ( UIEdit::* ) ( const boost::shared_ptr< const Font >& ) ) &UIEdit::FontPtr
				)
			.property( "x_scale",
				( const float& ( UIEdit::* ) ( ) const ) &UIEdit::XScale,
				( void ( UIEdit::* ) ( float ) ) &UIEdit::XScale
				)
			.property( "y_scale",
				( const float& ( UIEdit::* ) ( ) const ) &UIEdit::YScale,
				( void ( UIEdit::* ) ( float ) ) &UIEdit::YScale
				)
			.property( "max_chars",
				( const DWORD& ( UIEdit::* ) ( ) const ) &UIEdit::MaxChars,
				( void ( UIEdit::* ) ( DWORD ) ) &UIEdit::MaxChars
				)
	];
#endif
}





_XFX_END
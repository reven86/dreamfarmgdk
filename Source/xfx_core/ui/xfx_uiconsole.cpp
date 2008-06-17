//  File xfx_uiconsole.cpp                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_uiconsole.h"
#include "xfx_interface.h"
#include "main\xfx_application.h"
#include "main\xfx_cmd.h"
#include "renderer\xfx_font.h"
#include "renderer\xfx_shader.h"
#include "renderer\xfx_viewport.h"
#include "lua\xfx_luascript.h"

_XFX_BEGIN





//
// UIStandardConsole
//

UIStandardConsole::UIStandardConsole( ) :
	mMessagesCount( 0 ),
	mMessagesStartIndex( 0 ),
	mFadeDistance( 0.5f ),
	mFadeTime( 250 ),
	mCurrentState( ST_HIDE ),
	mMoveTime( 0 ),
	mTimeToHideMiniConsole( 0 ),
	mMiniConsoleLinesNumber( 3 )
{
	AddToken( "standard_console.fade_distance",			boost::bind( &ParseVariable< float >, boost::ref( mFadeDistance ), _1, _2 ) );
	AddToken( "standard_console.fade_time",				boost::bind( &ParseVariable< DWORD >, boost::ref( mFadeTime ), _1, _2 ) );
	AddToken( "standard_console.background_shader",		boost::bind( &UIStandardConsole::ParseBackground, this, _1, _2 ) );
	AddToken( "standard_console.font",					boost::bind( &UIStandardConsole::ParseFont, this, _1, _2 ) );
	AddToken( "standard_console.miniconsole_lines",		boost::bind( &ParseVariable< int >, boost::ref( mMiniConsoleLinesNumber ), _1, _2 ) );

	mPromptEditPtr.reset( &mPromptEdit, NullDeleter< UIEdit >( ) ); 
}

UIStandardConsole::~UIStandardConsole( )
{
}

void UIStandardConsole::PrepareToRender( )
{
	if( mCurrentState == ST_HIDE )
	{
		if( mTimeToHideMiniConsole < 0 )
			return;

		// render miniconsole
		mMiniConsoleLabel.PosX( -0.5f * Viewport::Instance( ).Width( ) );
		mMiniConsoleLabel.PosY( 0.5f * Viewport::Instance( ).Height( ) );

		mMiniConsoleLabel.Text( L"" );
		for( int i = static_cast< int >( mMessagesStartIndex + mMessagesCount - 3 );
			i < static_cast< int >( mMessagesStartIndex + mMessagesCount ); i++ )
			mMiniConsoleLabel.Text( mMiniConsoleLabel.Text( ) + L'\n' + mMessages[ i % mMessages.max_size( ) ] );

		return;
	}

	switch( mCurrentState )
	{
	case ST_HIDE:
		break;

	case ST_SHOW:
	case ST_MOVE_DOWN:
	case ST_MOVE_UP:
		{
			float avail_height = Viewport::Instance( ).Height( ) * mFadeDistance - 5.0f - mPromptEdit.Height( ) * 4.0f;

			mMessagesLabel.Text( L"" );
			for( 
				int i = static_cast< int >( mMessagesStartIndex + mMessagesCount ) - 1;
				i >= static_cast< int >( mMessagesStartIndex );
				i--
				)
			{
				mMessagesLabel.Text( mMessages[ i % mMessages.max_size( ) ] + L'\n' + mMessagesLabel.Text( ) );

				if( mMessagesLabel.Height( ) * 2.0f >= avail_height )
					break;
			}
		}
		break;
	}
}

void UIStandardConsole::ChangeState( const StateType& new_state )
{
	switch( new_state )
	{
	case ST_HIDE:
		break;

	case ST_SHOW:
		break;

	case ST_MOVE_DOWN:
		if( mCurrentState == ST_MOVE_UP )
			mMoveTime = mFadeTime - mMoveTime;
		else
			mMoveTime = 0;
		break;

	case ST_MOVE_UP:
		if( mCurrentState == ST_MOVE_DOWN )
			mMoveTime = mFadeTime - mMoveTime;
		else
			mMoveTime = 0;
		break;
	}

	mCurrentState = new_state;

	if( gGetApplication( ).pInterface( ) )
		if( mCurrentState == ST_SHOW )
		{
			gGetApplication( ).pInterface( )->FocusObject( mPromptEditPtr );
		}
		else
		{
			gGetApplication( ).pInterface( )->FocusObject( boost::shared_ptr< UIFocusable >( ) );
		}

	PrepareToRender( );

	// Force position changing
	Update( 0 );
}

void UIStandardConsole::Clear( )
{
	mMessagesStartIndex	= 0;
	mMessagesCount		= 0;

	PrepareToRender( );
}

void UIStandardConsole::Print( const Log::EMessageType& type, const WString& str )
{
	WString prefix;

	switch( type )
	{
	case Log::EMT_CRITICAL:
		prefix = L"Critical: ";
		break;

	case Log::EMT_ERROR:
		prefix = L"Error: ";
		break;

	case Log::EMT_WARNING:
		prefix = L"Warning: ";
		break;

	case Log::EMT_DEFAULT:
		prefix = L"";
		break;
	}

	mMessages[ ( mMessagesStartIndex + mMessagesCount ) % mMessages.max_size( ) ] = prefix + str;

	if( mMessagesCount < mMessages.max_size( ) )
	{
		mMessagesCount++;
	}
	else
	{
		mMessagesStartIndex++;
	}

	// show last message for 5 seconds.
	mTimeToHideMiniConsole = 50000;

	PrepareToRender( );
}

void UIStandardConsole::Render( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const
{
	ARGB newcolor = color * Color( );
	if( newcolor.a == 0 )
		return;

	if( mCurrentState == ST_HIDE )
	{
		// render miniconsole

		if( mTimeToHideMiniConsole > 0 )
			mMiniConsoleLabel.Render( newcolor, ofs, scale );
		return;
	}

	mBackground.Render( newcolor, ofs, scale );
	mMessagesLabel.Render( newcolor, ofs, scale );
	mPromptLabel.Render( newcolor, ofs, scale );
	mPromptEdit.Render( newcolor, ofs, scale );
}

void UIStandardConsole::Update( DWORD mspf )
{
	UIObject::X( 0.0f );
	UIObject::Width( Viewport::Instance( ).Width( ) * 0.5f );
	UIObject::Height( Viewport::Instance( ).Height( ) * 0.5f );

	switch( mCurrentState )
	{
	case ST_HIDE:
		UIObject::Y( static_cast< float >( Viewport::Instance( ).Height( ) ) );
		break;

	case ST_SHOW:
		UIObject::Y( Viewport::Instance( ).Height( ) * ( 1.0f - mFadeDistance ) );
		break;

	case ST_MOVE_DOWN:
		mMoveTime += mspf;

		if( mMoveTime > mFadeTime )
			ChangeState( ST_SHOW );
		else
			UIObject::Y( Viewport::Instance( ).Height( ) * ( 1.0f - mFadeDistance * mMoveTime / mFadeTime ) );
		break;

	case ST_MOVE_UP:
		mMoveTime += mspf;

		if( mMoveTime > mFadeTime )
			ChangeState( ST_HIDE );
		else
			UIObject::Y( Viewport::Instance( ).Height( ) * ( 1.0f - mFadeDistance * ( 1.0f - static_cast< float >( mMoveTime ) / mFadeTime ) ) );
		break;
	}

	if( mCurrentState != ST_HIDE )
	{
		mBackground.X( UIObject::X( ) );
		mBackground.Y( UIObject::Y( ) );
		mBackground.Width( UIObject::Width( ) );
		mBackground.Height( UIObject::Height( ) );

		mPromptLabel.PosX( mBackground.X( ) - mBackground.Width( ) + 5.0f );
		mPromptLabel.PosY( mBackground.Y( ) - mBackground.Height( ) + 15.0f );

		mPromptEdit.PosX( mPromptLabel.X( ) + mPromptLabel.Width( ) );
		mPromptEdit.PosY( mPromptLabel.PosY( ) );

		mMessagesLabel.PosX( mPromptLabel.PosX( ) );
		mMessagesLabel.Y( mBackground.Y( ) - mBackground.Height( ) + 15.0f + mPromptLabel.Height( ) * 3.0f + mMessagesLabel.Height( ) );

		mBackground.Update( mspf );
		mMessagesLabel.Update( mspf );
		mPromptLabel.Update( mspf );
		mPromptEdit.Update( mspf );
	}
	else
	{
		mTimeToHideMiniConsole -= mspf;

		if( mTimeToHideMiniConsole > 0 )
		{
			float t = 1.0f - 0.00002f * mTimeToHideMiniConsole;
			mMiniConsoleLabel.Update( mspf );
			mMiniConsoleLabel.PosY( 0.5f * Viewport::Instance( ).Height( ) + t * mMiniConsoleLabel.Height( ) * 2.0f );
		}
	}
}

void UIStandardConsole::Toggle( )
{
	StateType new_state;

	switch( mCurrentState )
	{
	case ST_HIDE:
		new_state = ST_MOVE_DOWN;
		break;

	case ST_MOVE_DOWN:
		new_state = ST_MOVE_UP;
		break;

	case ST_MOVE_UP:
		new_state = ST_MOVE_DOWN;
		break;

	case ST_SHOW:
		new_state = ST_MOVE_UP;
		break;
	}

	ChangeState( new_state );
}

HRESULT UIStandardConsole::PrepareParsing( )
{
	Clear( );
	mFadeDistance = 0.5f;
	mFadeTime = 250;
	mMiniConsoleLinesNumber = 3;

	ChangeState( ST_HIDE );

	return UIObject::PrepareParsing( );
}

HRESULT UIStandardConsole::AfterParsing( )
{
	mMessagesLabel.IsRealSize( true );
	mPromptLabel.IsRealSize( true );
	mPromptEdit.IsRealSize( true );
	mMiniConsoleLabel.IsRealSize( true );

	mPromptLabel.Text( L">" );
	ChangeState( ST_HIDE );
	PrepareToRender( );

	mPromptEdit.Name( boost::str( StringFormat( "%s_prompt_edit" ) % Name( ) ) );

	// register lua text entered event
#ifdef __XFX_USE_LUA_LUABIND__
	luabind::module( LuaScript::Instance( ).GetLuaVM( ) )
	[
		luabind::def(
			boost::str( StringFormat( "OnTextEntered_%s" ) % mPromptEdit.Name( ) ).c_str( ),
			&UIStandardConsole::OnTextEntered,
			luabind::discard_result
			)
	];
#endif

	return UIObject::AfterParsing( );
}

void UIStandardConsole::OnTextEntered( UIEdit * ed )
{
	if( !ed )
		return;

	Cmd::Instance( ).SetExecutionContext( Cmd::EEC_USER );
	Cmd::Instance( ).Execute( toMBCS( ed->Text( ) ) );
	Cmd::Instance( ).SetExecutionContext( Cmd::EEC_CODE );

	ed->Text( L"" );
}

HRESULT UIStandardConsole::ParseBackground( String::size_type& pos, const String& str )
{
	boost::shared_ptr< const Shader > shd = Shader::Cache( ).Register( next_token( str, pos ) );

	mBackground.AddShader( shd );

	ARGB col;

	ParseVariable< ARGB >( col, pos, str );

	mBackground.Color( col );

	return shd ? S_OK : XFXERR_IO;
}

HRESULT UIStandardConsole::ParseFont( String::size_type& pos, const String& str )
{
	boost::shared_ptr< const Font > fnt = Font::Cache( ).Register( next_token( str, pos ) );

	ARGB col;

	ParseVariable< ARGB >( col, pos, str );

	mMessagesLabel.FontPtr( fnt );
	mMessagesLabel.Color( col );
	mPromptLabel.FontPtr( fnt );
	mPromptLabel.Color( col );
	mPromptEdit.FontPtr( fnt );
	mPromptEdit.Color( col );
	mMiniConsoleLabel.FontPtr( fnt );
	mMiniConsoleLabel.Color( 0xffffffff );

	return fnt ? S_OK : XFXERR_IO;
}



_XFX_END
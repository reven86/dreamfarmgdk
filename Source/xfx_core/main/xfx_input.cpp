//  File xfx_input.cpp                                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_input.h"
#include "xfx_engine.h"
#include "xfx_cmd.h"
#include "xfx_application.h"

_XFX_BEGIN




#define DECLARE_KEY( k, d ) \
	std::pair< const char *, unsigned >( #k, d )

std::pair< const char *, unsigned > sKeyDWORD[ ] = 
{
    DECLARE_KEY( 0, DIK_0 ),
    DECLARE_KEY( 1, DIK_1 ),
    DECLARE_KEY( 2, DIK_2 ),
    DECLARE_KEY( 3, DIK_3 ),
    DECLARE_KEY( 4, DIK_4 ),
    DECLARE_KEY( 5, DIK_5 ),
    DECLARE_KEY( 6, DIK_6 ),
    DECLARE_KEY( 7, DIK_7 ),
    DECLARE_KEY( 8, DIK_8 ),
    DECLARE_KEY( 9, DIK_9 ),
    DECLARE_KEY( a, DIK_A ),
    DECLARE_KEY( brz_c1, DIK_ABNT_C1 ),
    DECLARE_KEY( brz_c2, DIK_ABNT_C2 ),
    DECLARE_KEY( num_plus, DIK_ADD ),
    DECLARE_KEY( apostr, DIK_APOSTROPHE ),
    DECLARE_KEY( apps, DIK_APPS ),
    DECLARE_KEY( jp_at, DIK_AT ),
    DECLARE_KEY( jp_ax, DIK_AX ),
    DECLARE_KEY( b, DIK_B ),
    DECLARE_KEY( back, DIK_BACK ),
    DECLARE_KEY( bslash, DIK_BACKSLASH ),
    DECLARE_KEY( c, DIK_C ),
    DECLARE_KEY( calc, DIK_CALCULATOR ),
    DECLARE_KEY( caps_lock, DIK_CAPITAL ),
    DECLARE_KEY( jp_colon, DIK_COLON ),
    DECLARE_KEY( comma, DIK_COMMA ),
    DECLARE_KEY( jp_convert, DIK_CONVERT ),
    DECLARE_KEY( d, DIK_D ),
    DECLARE_KEY( num_period, DIK_DECIMAL ),
    DECLARE_KEY( del, DIK_DELETE ),
    DECLARE_KEY( num_divide, DIK_DIVIDE ),
    DECLARE_KEY( down, DIK_DOWN ),
    DECLARE_KEY( e, DIK_E ),
    DECLARE_KEY( end, DIK_END ),
    DECLARE_KEY( equals, DIK_EQUALS ),
    DECLARE_KEY( escape, DIK_ESCAPE ),
    DECLARE_KEY( f, DIK_F ),
    DECLARE_KEY( f1, DIK_F1 ),
    DECLARE_KEY( f2, DIK_F2 ),
    DECLARE_KEY( f3, DIK_F3 ),
    DECLARE_KEY( f4, DIK_F4 ),
    DECLARE_KEY( f5, DIK_F5 ),
    DECLARE_KEY( f6, DIK_F6 ),
    DECLARE_KEY( f7, DIK_F7 ),
    DECLARE_KEY( f8, DIK_F8 ),
    DECLARE_KEY( f9, DIK_F9 ),
    DECLARE_KEY( f10, DIK_F10 ),
    DECLARE_KEY( f11, DIK_F11 ),
    DECLARE_KEY( f12, DIK_F12 ),
    DECLARE_KEY( f13, DIK_F13 ),
    DECLARE_KEY( f14, DIK_F14 ),
    DECLARE_KEY( f15, DIK_F15 ),
    DECLARE_KEY( g, DIK_G ),
    DECLARE_KEY( grave, DIK_GRAVE ),
    DECLARE_KEY( h, DIK_H ),
    DECLARE_KEY( home, DIK_HOME ),
    DECLARE_KEY( i, DIK_I ),
    DECLARE_KEY( ins, DIK_INSERT ),
    DECLARE_KEY( j, DIK_J ),
    DECLARE_KEY( k, DIK_K ),
    DECLARE_KEY( jp_kana, DIK_KANA ),
    DECLARE_KEY( jp_kanji, DIK_KANJI ),
    DECLARE_KEY( l, DIK_L ),
    DECLARE_KEY( lbracket, DIK_LBRACKET ),
    DECLARE_KEY( lctrl, DIK_LCONTROL ),
    DECLARE_KEY( left, DIK_LEFT ),
    DECLARE_KEY( lalt, DIK_LMENU ),
    DECLARE_KEY( lshift, DIK_LSHIFT ),
    DECLARE_KEY( lwin, DIK_LWIN ),
    DECLARE_KEY( m, DIK_M ),
    DECLARE_KEY( mail, DIK_MAIL ),
    DECLARE_KEY( media_select, DIK_MEDIASELECT ),
    DECLARE_KEY( media_stop, DIK_MEDIASTOP ),
    DECLARE_KEY( minus, DIK_MINUS ),
    DECLARE_KEY( num_mult, DIK_MULTIPLY ),
    DECLARE_KEY( mute, DIK_MUTE ),
    DECLARE_KEY( my_computer, DIK_MYCOMPUTER ),
    DECLARE_KEY( n, DIK_N ),
    DECLARE_KEY( pgdn, DIK_NEXT ),
    DECLARE_KEY( next_track, DIK_NEXTTRACK ),
    DECLARE_KEY( jp_noconvert, DIK_NOCONVERT ),
    DECLARE_KEY( num_lock, DIK_NUMLOCK ),
    DECLARE_KEY( num_0, DIK_NUMPAD0 ),
    DECLARE_KEY( num_1, DIK_NUMPAD1 ),
    DECLARE_KEY( num_2, DIK_NUMPAD2 ),
    DECLARE_KEY( num_3, DIK_NUMPAD3 ),
    DECLARE_KEY( num_4, DIK_NUMPAD4 ),
    DECLARE_KEY( num_5, DIK_NUMPAD5 ),
    DECLARE_KEY( num_6, DIK_NUMPAD6 ),
    DECLARE_KEY( num_7, DIK_NUMPAD7 ),
    DECLARE_KEY( num_8, DIK_NUMPAD8 ),
    DECLARE_KEY( num_9, DIK_NUMPAD9 ),
    DECLARE_KEY( num_comma, DIK_NUMPADCOMMA ),
    DECLARE_KEY( num_enter, DIK_NUMPADENTER ),
    DECLARE_KEY( num_equals, DIK_NUMPADEQUALS ),
    DECLARE_KEY( o, DIK_O ),
    DECLARE_KEY( oem_102, DIK_OEM_102 ),
    DECLARE_KEY( p, DIK_P ),
    DECLARE_KEY( pause, DIK_PAUSE ),
    DECLARE_KEY( period, DIK_PERIOD ),
    DECLARE_KEY( play_pause, DIK_PLAYPAUSE ),
    DECLARE_KEY( power, DIK_POWER ),
    DECLARE_KEY( prev_track, DIK_PREVTRACK ),
    DECLARE_KEY( pgup, DIK_PRIOR ),
    DECLARE_KEY( q, DIK_Q ),
    DECLARE_KEY( r, DIK_R ),
    DECLARE_KEY( rbracket, DIK_RBRACKET ),
    DECLARE_KEY( rctrl, DIK_RCONTROL ),
    DECLARE_KEY( enter, DIK_RETURN ),
    DECLARE_KEY( right, DIK_RIGHT ),
    DECLARE_KEY( ralt, DIK_RMENU ),
    DECLARE_KEY( rshift, DIK_RSHIFT ),
    DECLARE_KEY( rwin, DIK_RWIN ),
    DECLARE_KEY( s, DIK_S ),
    DECLARE_KEY( scroll_lock, DIK_SCROLL ),
    DECLARE_KEY( semicolon, DIK_SEMICOLON ),
    DECLARE_KEY( slash, DIK_SLASH ),
    DECLARE_KEY( sleep, DIK_SLEEP ),
    DECLARE_KEY( space, DIK_SPACE ),
    DECLARE_KEY( stop, DIK_STOP ),
    DECLARE_KEY( num_minus, DIK_SUBTRACT ),
    DECLARE_KEY( sysrq, DIK_SYSRQ ),
    DECLARE_KEY( t, DIK_T ),
    DECLARE_KEY( tab, DIK_TAB ),
    DECLARE_KEY( u, DIK_U ),
    DECLARE_KEY( jp_underline, DIK_UNDERLINE ),
    DECLARE_KEY( jp_unlabeled, DIK_UNLABELED ),
    DECLARE_KEY( up, DIK_UP ),
    DECLARE_KEY( v, DIK_V ),
    DECLARE_KEY( voldn, DIK_VOLUMEDOWN ),
    DECLARE_KEY( volup, DIK_VOLUMEUP ),
    DECLARE_KEY( w, DIK_W ),
    DECLARE_KEY( wake, DIK_WAKE ),
    DECLARE_KEY( webback, DIK_WEBBACK ),
    DECLARE_KEY( webfav, DIK_WEBFAVORITES ),
    DECLARE_KEY( webforw, DIK_WEBFORWARD ),
    DECLARE_KEY( webhome, DIK_WEBHOME ),
    DECLARE_KEY( webrefr, DIK_WEBREFRESH ),
    DECLARE_KEY( websearch, DIK_WEBSEARCH ),
    DECLARE_KEY( webstop, DIK_WEBSTOP ),
    DECLARE_KEY( x, DIK_X ),
    DECLARE_KEY( y, DIK_Y ),
    DECLARE_KEY( jp_yen, DIK_YEN ),
    DECLARE_KEY( z, DIK_Z ),

	DECLARE_KEY( mouse_l, Input::MOUSE_LBUTTON ),
    DECLARE_KEY( mouse_r, Input::MOUSE_RBUTTON ),
    DECLARE_KEY( mouse_m, Input::MOUSE_MBUTTON ),
    DECLARE_KEY( mouse_4, Input::MOUSE_4BUTTON ),

	DECLARE_KEY( mouse_moveleft, Input::MOUSE_MOVELEFT ),
	DECLARE_KEY( mouse_moveright, Input::MOUSE_MOVERIGHT ),
	DECLARE_KEY( mouse_movedown, Input::MOUSE_MOVEDOWN ),
	DECLARE_KEY( mouse_moveup, Input::MOUSE_MOVEUP ),
	DECLARE_KEY( mouse_wheelup, Input::MOUSE_WHEELUP ),
	DECLARE_KEY( mouse_wheeldown, Input::MOUSE_WHEELDOWN )
};

#undef DECLARE_KEY
const int sKeyDWORDCount = sizeof( sKeyDWORD ) / sizeof( std::pair< const char *, unsigned > );





//
// Input class
//

Input::Input( ) :
	mpInput( ),
	mpKeyboard( ),
	mpMouse( ),
	mUseKeyMapping( false )
{
}

Input::~Input( )
{
}

HRESULT Input::Init( )
{
	//Initialization input system
	HRESULT res;

	gMess ("");
	gMess ("Initialization input system:");

	IDirectInput8 * dev;
	res = DirectInput8Create (GetModuleHandle (NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &dev, NULL);

	if (SUCCEEDED (res))
	{
		mpInput.reset (dev, IUnknownDeleter ());

		gMess ("...creating DirectInput object: OK");

		//Creating keyboard device
		IDirectInputDevice8 * k_dev;
		res = mpInput->CreateDevice (GUID_SysKeyboard, &k_dev, NULL);

		if( SUCCEEDED( res ) )
		{
			mpKeyboard.reset( k_dev, IUnknownDeleter( ) );

			gMess( "...creating keyboard input device: OK" );

			mpKeyboard->SetCooperativeLevel	( gGetApplication( ).hWnd( ), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
			mpKeyboard->SetDataFormat		( &c_dfDIKeyboard );
			mpKeyboard->Acquire				( );

			//Creating mouse device
			IDirectInputDevice8 * m_dev;
			res = mpInput->CreateDevice (GUID_SysMouse, &m_dev, NULL);

			if (SUCCEEDED (res))
			{
				mpMouse.reset (m_dev, IUnknownDeleter ());

				gMess ("...creating mouse input device: OK");

				DIPROPDWORD hd;
				hd.diph.dwHeaderSize	= sizeof (hd.diph);
				hd.diph.dwHow			= DIPH_DEVICE;
				hd.diph.dwObj			= 0;
				hd.diph.dwSize			= sizeof (hd);
				hd.dwData				= 256;

				mpMouse->SetCooperativeLevel	( gGetApplication( ).hWnd( ), DISCL_EXCLUSIVE | DISCL_FOREGROUND );
				mpMouse->SetDataFormat			( &c_dfDIMouse );
				mpMouse->SetProperty			( DIPROP_BUFFERSIZE, &hd.diph );
				mpMouse->Acquire				( );
			}
			else
				gError ("Creating mouse input device failed!");
		}
		else
			gError ("Creating keyboard input device failed!");
	}
	else
		gError ("Creating DirectInput object failed!");

	gMess ("");

	return res;
}

HRESULT Input::Shutdown( )
{
	HRESULT res = S_OK;

	gMess ("");
	gMess ("Shutting down input system:");

	if (mpMouse)
	{
		gMess ("...releasing mouse input device: OK");

		mpMouse->Unacquire	();
		mpMouse.reset ();
	}
	else
	{
		gMess ("Releasing %s input device failed!", "mouse");
		res = XFXERR_INVALIDCALL;
	}

	if (mpKeyboard)
	{
		gMess ("...releasing keyboard input device: OK");

		mpKeyboard->Unacquire	();
		mpKeyboard.reset ();
	}
	else
	{
		gMess ("Releasing %s input device failed!", "keyboard");
		res = XFXERR_INVALIDCALL;
	}

	if (mpInput)
	{
		gMess ("...releasing DirectInput object: OK");
	}
	else
	{
		gMess ("Releasing DirectInput object failed!");
		res = XFXERR_INVALIDCALL;
	}

	mpInput.reset ();

	return res;
}

void Input::RetrieveData( )
{
	PROFILE( __FUNCTION__, "General" );

	//Retrieving keyboard data
	HRESULT hr;
	char keys[ 256 ];
	
	bool console_active = Engine::Instance( ).IsConsoleActive( );

	hr = mpKeyboard->GetDeviceState( sizeof( keys ), keys );

	if( SUCCEEDED( hr ) )
	{
		char * curkey = keys;
		BYTE * keymap = mKeyMap.begin( );

		for( unsigned i = 0; i < 256; i++, curkey++, keymap++ )
		{
			bool allow_key = !console_active || ( ( *keymap & EKF_IN_CONSOLE ) != 0 );
			bool oldvalue( *keymap & EKF_PUSHED );

			if( ( ( *curkey ) & 0x80 ) != 0 )
				*keymap |= EKF_PUSHED;
			else
				*keymap &= ~EKF_PUSHED;

			if( mUseKeyMapping && allow_key )
			{
				if( *keymap & EKF_PUSHED )
				{
					Cmd::Instance( ).Execute( mCommands[ i ][ PUSHED ] );

					if( !oldvalue )
						Cmd::Instance( ).Execute( mCommands[ i ][ FIRST_PUSHED ] );
				}
				else if( oldvalue )
					Cmd::Instance( ).Execute( mCommands[ i ][ POPED ] );
			}
		}
	}
	else
		mpKeyboard->Acquire ();

	//Retrieving mouse data
	DIMOUSESTATE ms;

	hr = mpMouse->GetDeviceState (sizeof (ms), &ms);

	if (SUCCEEDED (hr))
	{
		BYTE * keymap = mKeyMap.c_array( ) + 256;

		for( unsigned i = 0; i < 4; i++, keymap++ )
		{
			bool allow_key = !console_active || ( ( *keymap & EKF_IN_CONSOLE ) != 0 );
			bool oldvalue( *keymap & EKF_PUSHED );

			if( ( ms.rgbButtons[ i ] & 0x80 ) != 0 )
				*keymap |= EKF_PUSHED;
			else
				*keymap &= ~EKF_PUSHED;

			if( mUseKeyMapping && allow_key )
			{
				if( *keymap )
				{
					Cmd::Instance( ).Execute( mCommands[ i + 256 ][ PUSHED ]);

					if( !oldvalue )
						Cmd::Instance( ).Execute( mCommands[ i + 256 ][ FIRST_PUSHED ] );
				}
				else if( oldvalue )
					Cmd::Instance( ).Execute( mCommands[ i + 256 ][ POPED ] );
			}
		}
	}
	else
		mpMouse->Acquire( );

	TestMouseMove( );
}

void Input::TestMouseMove ()
{
	static DIDEVICEOBJECTDATA m_buffer[256];
	DWORD read_num = 256;

	if (FAILED (mpMouse->GetDeviceData (sizeof (DIDEVICEOBJECTDATA), m_buffer, &read_num, 0)))
	{
		mpMouse->Acquire ();
		return;
	}

	int x_move = 0;
	int y_move = 0;
	int z_move = 0;

	while (read_num--)
	{
		switch (m_buffer[read_num].dwOfs)
		{
		case DIMOFS_X:
			x_move += static_cast<int> (m_buffer[read_num].dwData);
			break;

		case DIMOFS_Y:
			y_move += static_cast<int> (m_buffer[read_num].dwData);
			break;

		case DIMOFS_Z:
			z_move += static_cast<int> (m_buffer[read_num].dwData);
			break;
		}
	}

	WriteMouseMoveCommand (MOUSE_MOVERIGHT, x_move, x_move > 0);
	WriteMouseMoveCommand (MOUSE_MOVELEFT, -x_move, x_move < 0);
	WriteMouseMoveCommand (MOUSE_MOVEUP, y_move, y_move > 0);
	WriteMouseMoveCommand (MOUSE_MOVEDOWN, -y_move, y_move < 0);
	WriteMouseMoveCommand (MOUSE_WHEELUP, z_move, z_move > 0);
	WriteMouseMoveCommand (MOUSE_WHEELDOWN, -z_move, z_move < 0);
}

void Input::WriteMouseMoveCommand( DWORD key, int n, bool condition )
{
	bool allow_key = !Engine::Instance( ).IsConsoleActive( ) || ( ( mKeyMap[ key ] & EKF_IN_CONSOLE ) != 0 );
	bool oldvalue = ( ( mKeyMap[ key ] & EKF_PUSHED ) != 0 );

	if( condition )
		mKeyMap[ key ] |= EKF_PUSHED;
	else
		mKeyMap[ key ] &= ~EKF_PUSHED;

	if( mUseKeyMapping && allow_key )
	{
		char num[ 10 ];
		_snprintf( num, 10, " %d", n );

		if( ( mKeyMap[ key ] & EKF_PUSHED ) != 0 )
		{
			if( !mCommands[ key ][ PUSHED ].empty( ) )
				Cmd::Instance( ).Execute( mCommands[ key ][ PUSHED ] + num );

			if( !oldvalue && !mCommands[ key ][ FIRST_PUSHED ].empty( ) )
				Cmd::Instance( ).Execute( mCommands[ key ][ FIRST_PUSHED ] + num );
		}
		else if( oldvalue && !mCommands[ key ][ POPED ].empty( ) )
			Cmd::Instance( ).Execute( mCommands[ key ][ POPED ] + num );
	}
}

void Input::SetMouseMode( const bool& isexclusive )
{
	if( mpMouse )
	{
		mpMouse->Unacquire( );
		mpMouse->SetCooperativeLevel( gGetApplication( ).hWnd( ), ( isexclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE ) | DISCL_FOREGROUND );
		mpMouse->Acquire( );
	}
}

const char * Input::KeyName( unsigned key ) const
{
	_ASSERTE( key < MAX_KEYS );

	for( int i = 0; i < sKeyDWORDCount; i++ )
		if( sKeyDWORD[i].second == key )
			return sKeyDWORD[i].first;

	gWarn( "Unknown key value: %u", key );

	return sKeyDWORD[ 0 ].first;
}

void Input::Command( unsigned key, STATES state, const String& cmd )
{
	_ASSERTE( key < MAX_KEYS );
	_ASSERTE( state < MAX_STATES );
	
	if( cmd.empty( ) )
	{
		mBindedKeysMap[ state ].erase( mCommands[ key ][ state ] );
	}
	else
	{
		mBindedKeysMap[ state ].insert( std::make_pair( cmd, key ) );
	}

	mCommands[ key ][ state ] = cmd;
}

unsigned Input::GetKeyForCommand( const String& command ) const
{
	for( int state = 0; state < MAX_STATES; state++ )
	{
		stdext::hash_map< String, unsigned, HashCompare< String > >::const_iterator it = mBindedKeysMap[ state ].find( command );

		if( it != mBindedKeysMap[ state ].end( ) )
			return ( *it ).second;
	}

	return MAX_KEYS;
}

void Input::Cmd_bind( bool in_console, const String &str )
{
	String::size_type cmdstart = 0;
	String token;

	token = next_token( str, cmdstart );

	if( token.empty( ) )
	{
		gToConsole( "Use: bind {+|-}<key> \"[<cmds>;]\"" );
	}
	else
	{
		String key( boost::to_lower_copy( token ) );

		String oldkey( key );

		Input::STATES state;
		switch( key[ 0 ] )
		{
		case '+':
			state = Input::FIRST_PUSHED;
			key.erase( 0, 1 );
			break;

		case '-':
			state = Input::POPED;
			key.erase( 0, 1 );
			break;

		default:
			state = Input::PUSHED;
		};

		unsigned i;
		for( i = 0; i < sKeyDWORDCount; i++ )
			if( sKeyDWORD[ i ].first == key )
				break;

		if( i == sKeyDWORDCount )
		{
			gToConsole( "Unknown key: %s", key.c_str( ) );
		}
		else
		{
			token = next_token( str, cmdstart );
			unsigned keydw = sKeyDWORD[ i ].second;

			if( token.empty( ) )
			{
				gToConsole( "%s is bound to \"%s\"", oldkey.c_str( ), Command( keydw, state ).c_str( ) );
			}
			else
			{
				Command( keydw, state, token );

				if( in_console )
					mKeyMap[ keydw ] |= EKF_IN_CONSOLE;
				else
					mKeyMap[ keydw ] &= ~EKF_IN_CONSOLE;

				gToConsole( "%s bound to \"%s\"", oldkey.c_str( ), token.c_str( ) );
			}
		}
	}
}





_XFX_END
//  File xfx_engine.cpp                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_engine.h"

_XFX_BEGIN





//
// Variables
//

const String Engine::msVersion( "1.0.$WCREV$" );





//
// Engine
//

Engine::Engine( )
{
}

Engine::~Engine( )
{
}

bool Engine::Initialize( HWND hwnd )
{
	mRenderWnd = hwnd;

	RECT rect;
	GetClientRect( mRenderWnd, &rect );

	//Viewport::Instance( ).Init( rect.right - rect.left, rect.bottom - rect.top );

	HRESULT hr;

	//mEngineLogPtr.reset( new class Log( g_logfile->Value( ) ) );

	gMess( "Initializing xfx:" );
	gMess( "...engine version: %s", msVersion.c_str( ) );
	gMess( "...window handle: 0x%X", mRenderWnd );

	//Getting OS information
	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof( osinfo );

	GetVersionEx( &osinfo );

	gMess( "" );
	gMess( "OS information:" );
	gMess( "...platform: %s", ( osinfo.dwPlatformId == 1 ) ? "Win32" : "WinNT" );
	gMess( "...version number: %u.%u.%u", osinfo.dwMajorVersion, osinfo.dwMinorVersion, LOWORD( osinfo.dwBuildNumber ) );
	gMess( "...additional info: %s", osinfo.szCSDVersion );

	//Getting processor info
	char			cpu_vendorID[ 13 ];
	unsigned		cpu_family;
	unsigned		cpu_model;
    
	cpu_vendorID[12] = '\0';
	__asm {
		xor		eax, eax;
		cpuid;
		mov		dword ptr [cpu_vendorID], ebx;
		mov		dword ptr [cpu_vendorID + 4], edx;
		mov		dword ptr [cpu_vendorID + 8], ecx;
		mov		eax, 1;
		cpuid;
		mov		ebx, eax;
		and		eax, 0xf00;
		shr		eax, 8;
		mov		cpu_family, eax;
		mov		eax, ebx;
		and		eax, 0xf0;
		shr		eax, 4;
		mov		cpu_model, eax;
	}

	gMess( "" );
	gMess( "CPU information:" );
	gMess( "...vendorID: %s", cpu_vendorID );
	gMess( "...family: %u", cpu_family );
	gMess( "...model: %u", cpu_model );

	/*
	if( !Input::Instance( ).Init( ) )
		return false;
		*/

#pragma message ( "FIXME: fix this" )
	/*
#ifdef __XFX_USE_LUA_LUABIND__
	if( FAILED( hr = LuaScript::Instance( ).Open( ) ) )
		return hr;
#endif
		*/

	gMess( "Initialization xfx was successfully completed!" );
	gMess( "" );

	mRenderWndStyle		= GetWindowLong( mRenderWnd, GWL_STYLE );
	mRenderWndExStyle	= GetWindowLong( mRenderWnd, GWL_EXSTYLE );

	return true;
}

bool Engine::Free( )
{
	//Caches::Instance( ).FlushAll( );

#pragma message ( "FIXME: fix this" )
	/*
#ifdef __XFX_USE_LUA_LUABIND__
	LuaScript::Instance( ).Close( );
#endif
	*/

	//Input::Instance( ).Free( );

	gMess( "" );
	gMess( "Shutting down xfx." );

	//Render::Instance( ).Free( );

	return true;
}

LRESULT Engine::ProcessMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	/*
	switch( msg )
	{
		case WM_PAINT:
			{
				//Viewport::Instance ().Render ();
			}
			break;

		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_MOUSEWHEEL:
			if( gGetApplication( ).pInterface( ) )
			{
#pragma message ( "FIXME: use aux func" )
				ViewportType viewport;
				Render::Instance( ).GetViewport( viewport );

				float x = static_cast< float >( LOWORD( lParam ) ) / viewport.Width * Viewport::Instance( ).Width( );
				float y = static_cast< float >( HIWORD( lParam ) ) / viewport.Height * Viewport::Instance( ).Height( );

				gGetApplication( ).pInterface( )->ProcessMouse( x, y, msg, wParam );
			}
			break;

		case WM_SIZE:
			{
				RECT wndrect;
				GetClientRect( hWnd, &wndrect );

				Render::Instance( ).ResetDevice( wndrect.right, wndrect.bottom );
			}
			break;

		case WM_KEYDOWN:
			if( gGetApplication( ).pInterface( ) )
			{
				BYTE key_state[ 256 ];

				if( GetKeyboardState( key_state ) )
				{
					wchar_t unicode_char;
					if( ToUnicode(
						static_cast< UINT >( wParam ),
						( static_cast< UINT >( lParam ) >> 0x10 ) & 0xff,
						key_state,
						&unicode_char,
						1,
						0 ) > 0
						)
					{
						unsigned console_key = Input::Instance( ).GetKeyForCommand( "console_toggle" );

						if( !IsConsoleActive( ) || console_key == Input::MAX_KEYS || !Input::Instance( ).TestKey( console_key ) )
							gGetApplication( ).pInterface( )->ProcessKey( static_cast< wchar_t >( unicode_char ) );
					}
				}
			}
			break;

		default:
			return DefWindowProc (hWnd, msg, wParam, lParam);
	}
	*/

	return 0;
}

/*
void Engine::PrintToConsole( const Log::EMessageType& type, const WString& str )
{
	if( mConsoleControlPtr )
	{
		mConsoleControlPtr->Print( type, str );
	}
}
*/

void Engine::ToggleConsole( )
{
	if( mConsoleControlPtr )
	{
		//mConsoleControlPtr->Toggle( );
	}
}

bool Engine::IsConsoleActive( ) const
{
	if( mConsoleControlPtr )
	{
		//return mConsoleControlPtr->IsVisible( );
	}

	return false;
}

/*
void Engine::Log( const Log::EMessageType& type, const String& msg )
{
	if( type > g_log_level->AsInt( ) )
		return;

	if( mEngineLogPtr )
		mEngineLogPtr->Print( type, msg );

	// duplicate in console
	PrintToConsole( type, fromMBCS( msg ) );
}
*/




_XFX_END
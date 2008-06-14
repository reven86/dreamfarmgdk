//  File xfx_application.cpp                                                  |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_application.h"
#include "xfx_engine.h"
#include "xfx_input.h"
#include "xfx_main_cvars.h"
#include "xfx_cmd.h"
#include <direct.h>

_XFX_BEGIN







//
// Application methods
//

HRESULT Application::InitEngine( HINSTANCE inst, const String& cmdline, const WString& wndclass, const WString& caption )
{
	mhInstance			= inst;
	mCommandLine		= cmdline;
	mWndClass			= wndclass;

	WNDCLASS wc;

	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= &sWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= mhInstance;
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= HBRUSH( RGB( 0, 0, 0 ) );
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= mWndClass.c_str( );
	
	RegisterClass( &wc );

    mWnd = CreateWindow( mWndClass.c_str( ), caption.c_str( ), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, mhInstance, NULL );

	static char dir[ 2500 ];
	_getcwd( dir, 2500 );

	return Engine::Instance( ).Init( ) ? S_OK : XFXERR_UNKNOWN;
}

/*
HRESULT Application::InitRenderer( )
{
	return Render::Instance( ).CreateDevice( );
}
*/

/*
HRESULT Application::InitAudio( )
{
	return Audio::Instance( ).Init( );
}
*/

void Application::Shutdown( )
{
	Engine::Instance( ).Shutdown( );
	//Audio::Instance ().Free ();
	UnregisterClass( mWndClass.c_str( ), mhInstance );
}

void Application::MainLoop( )
{
	MSG msg;
	msg.message = static_cast< unsigned >( ~WM_QUIT );

	while( msg.message != WM_QUIT )
	{
		{
			PROFILE( __FUNCTION__, "General" );

			Input::Instance( ).RetrieveData( );
			Update( );

			/*
#ifdef __XFX_USE_LUA_LUABIND__
			LuaSMManager::Instance( ).Update( mTimer.MSPF100( ) );
#endif
			Audio::Instance( ).Update( mTimer.MSPF100( ) );
			*/

			/*
			if( hWnd( ) )
				Viewport::Instance( ).Render( );
				*/

			{
				PROFILE( "xfx::Application::MessageLoop", "General" );
				if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
				{
					TranslateMessage	( &msg );
					DispatchMessageW	( &msg );
				}
			}
		}

#ifndef __XFX_DISABLE_PROFILER__
		ProfilingManager::Instance( ).SaveProfilerResults( );
		ProfilingManager::Instance( ).SetFrameTime( g_profiler_frame_rate->AsInt( ) > 0 ? 10000 / g_profiler_frame_rate->AsInt( ) : mTimer.MSPF100( ) );
#endif
	}
}

LRESULT CALLBACK Application::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
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
	case 0x020A://WM_MOUSEWHEEL:
		/*
		if( gGetApplication( ).pInterface( ) )
		{
			ViewportType viewport;
			Render::Instance ().GetViewport (viewport);

			float x = ((float) LOWORD (lParam) / viewport.Width - 0.5f) * Viewport::Instance ().Width ();
			float y = (0.5f - (float) HIWORD (lParam) / viewport.Height) * Viewport::Instance ().Height ();

			gGetApplication ().pInterface ()->ProcessMouse (x, y, msg, wParam);
		}
		*/
		break;

	case WM_SIZE:
		/*
		{
			RECT wndrect;
			GetClientRect( hWnd, &wndrect );

			Render::Instance( ).ResetDevice( wndrect.right, wndrect.bottom );
		}
		*/
		break;

	case WM_KEYDOWN:
		/*
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
		*/
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		mWnd = 0;
		break;

	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	return 0;
}

LRESULT CALLBACK Application::sWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return gGetApplication( ).WndProc( hWnd, msg, wParam, lParam );
}




_XFX_END
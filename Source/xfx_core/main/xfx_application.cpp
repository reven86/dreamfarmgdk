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

Application::Application( ) :
	mWnd( 0 ),
	mhInstance( 0 )
{
}

HRESULT Application::InitEngine( HINSTANCE inst, const String& cmdline, const WString& wndclass, const WString& caption )
{
	if( mhInstance != 0 )
	{
		// mhInstance must be real instance, and if it not null
		// then InitEngine is called more than once, or
		// Shutdown was not called
		return XFXERR_INVALIDCALL;
	}

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
	wc.hbrBackground	= HBRUSH( 0 );//COLOR_BACKGROUND );
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= mWndClass.c_str( );
	
	RegisterClass( &wc );

    mWnd = CreateWindow( mWndClass.c_str( ), caption.c_str( ), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, mhInstance, NULL );

	return Engine::Instance( ).Init( ) ? S_OK : XFXERR_UNKNOWN;
}

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

	mhInstance = 0;
	mWnd = 0;
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

			{
				PROFILE( "xfx::Application::MessageLoop", "General" );
				if( PeekMessage( &msg, HWnd( ), 0, 0, PM_REMOVE ) )
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
	case WM_SIZE:
		/*
		{
			RECT wndrect;
			GetClientRect( hWnd, &wndrect );

			Render::Instance( ).ResetDevice( wndrect.right, wndrect.bottom );
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
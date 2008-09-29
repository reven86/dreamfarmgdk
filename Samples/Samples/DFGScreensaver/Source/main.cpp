#include "pch.h"
#include "application.h"
#include "resource.h"
#include <Scrnsave.h>

#pragma comment (lib, "Scrnsavw.lib")
#pragma comment (lib, "comctl32.lib")



// Pointer to screensaver class.
boost::scoped_ptr< Screensaver >			gScreensaverPtr;
boost::shared_ptr< xfx::Pack >				gResourcePackPtr;



// Init engine.
HRESULT InitEngine( HWND hwnd, const xfx::String& cmdline )
{
	// init engine without input system
	if( !xfx::Engine::Instance( ).Init( false ) )
		return XFXERR_UNKNOWN;

	// Initialize renderer.
	HRESULT res = xfx::Renderer::Instance( ).CreateDevice( hwnd );

	if( FAILED( res ) )
		return res;

	// Load from resources pack file with data
	HGLOBAL pack_res = LoadResource( NULL, FindResource( NULL, MAKEINTRESOURCE( IDR_PACK1 ), L"PACK" ) );
	unsigned long pack_file_len = 173433;

	gResourcePackPtr.reset( new xfx::Pack( ) );
	gResourcePackPtr->LoadMemory( LockResource( pack_res ), pack_file_len );

	xfx::FileSystem::Instance( ).AddPack( gResourcePackPtr );

	return S_OK;
}

void ShutdownEngine( )
{
	gResourcePackPtr.reset( );

	// firstly shutdown engine, to flush caches and all other resources
	// and then shutdown renderer.

	// shutdown engine
	xfx::Engine::Instance( ).Shutdown( );

	// shutdown renderer
	xfx::Renderer::Instance( ).Shutdown( );
}




//
// ScreenSaverProc
//

LRESULT WINAPI ScreenSaverProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam)
{
	static UINT_PTR sTimer;   // timer identifier 

    // Handles screen saver messages
    switch(message)
    {
    case WM_CREATE:
        // Creation of the screen saver window

		if( SUCCEEDED( InitEngine( hwnd, "" ) ) )
		{
			gScreensaverPtr.reset( new Screensaver( ) );
			gScreensaverPtr->Init( );
		}

		// Set timer to update-render screensaver
		sTimer = SetTimer( hwnd, 1, 0, NULL );
        return 0;
    case WM_ERASEBKGND:
        // Erases the screen saver background
        return 0;
	case WM_TIMER:
		if( gScreensaverPtr )
		{
			gScreensaverPtr->Update( );
			gScreensaverPtr->Render( );
		}
		return 0;
	case WM_PAINT:
		if( gScreensaverPtr )
		{
			gScreensaverPtr->Update( );
			gScreensaverPtr->Render( );
		}
		return 0;
    case WM_DESTROY:
        // Cleans up the screen saver window
		if( sTimer )
			KillTimer( hwnd, sTimer );

		if( gScreensaverPtr )
			gScreensaverPtr.reset( NULL );

		ShutdownEngine( );

        PostQuitMessage(0);
        return 0;
    }
    return DefScreenSaverProc(hwnd,message,wparam,lparam);
}


BOOL WINAPI ScreenSaverConfigureDialog(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam)
{
    return true;
}


BOOL WINAPI RegisterDialogClasses(HANDLE hmodule)
{
    return true;
}
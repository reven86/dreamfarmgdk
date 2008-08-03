#include "pch.h"
#include "application.h"






//
// gGetApplication implementation
//

xfx::Application& xfx::gGetApplication( )
{
	return ::Application::Instance( );
}




//
// Application
//

HRESULT Application::InitEngine( HINSTANCE inst, const xfx::String &cmdline, const xfx::WString &wndclass, const xfx::WString &caption )
{
	HRESULT res = xfx::Application::InitEngine( inst, cmdline, wndclass, caption );

	if( FAILED( res ) )
		return res;

	// Initialize renderer.
	res = xfx::Renderer::Instance( ).CreateDevice( );

	if( FAILED( res ) )
		return res;

	// add search path with media content to file system
	// after doing this, we can refer to media content without using full path
	// note: the path should be end up with '\' 
	xfx::FileSystem::Instance( ).AddSearchPath( "data\\" );

	mGUI.reset( new xfx::GUI( ) );

	return res;
}

void Application::Shutdown( )
{
	mGUI.reset( );

	// shutdown renderer
	xfx::Renderer::Instance( ).Shutdown( );

	// call base method
	xfx::Application::Shutdown( );
}

void Application::Update( )
{
	// that means we are in shutdown process, no update-render then
	if( !hWnd( ) )
		return;

	// Update timer.
	mTimer.Update( );

	// Update GUI.
	if( mGUI )
		mGUI->Update( mTimer.MicroSeconds100( ) );

	// prepare scene to rendering
	// this function do all preparation for begin and end scene rendering and
	// then call xfx::Application::Render
	xfx::Viewport::Instance( ).Render( );
}

void Application::Render( ) const
{
	// render GUI
	if( mGUI )
		mGUI->Render( );
}

LRESULT CALLBACK Application::WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	if( mGUI )
		mGUI->ProcessWndMessage( hwnd, msg, wparam, lparam );

	return xfx::Application::WndProc( hwnd, msg, wparam, lparam );
}
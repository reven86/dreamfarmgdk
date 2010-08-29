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

HRESULT Application::InitEngine( const xfx::String &cmdline, const xfx::WString &wndclass, const xfx::WString &caption )
{
	HRESULT res = xfx::Application::InitEngine( cmdline, wndclass, caption );

	if( FAILED( res ) )
		return res;

	// Initialize renderer.
	res = xfx::Renderer::Instance( ).CreateDevice( HWnd( ) );

	if( FAILED( res ) )
		return res;

	// add search path with media content to file system
	// after doing this, we can refer to media content without using full path
	// note: the path should be end up with '\' 
	xfx::FileSystem::Instance( ).AddSearchPath( "data\\" );

	// init lua script system
	xfx::LuaScript::Instance( ).Open( );

	// register default shader that will be used if another one won't provided
	xfx::Renderer::Instance( ).SetDefaultShader( xfx::Shader::Cache( ).Register( "default.shader" ) );

	mGUI.reset( new xfx::GUI( ) );

	if( !mGUI )
		return XFXERR_OUTOFMEMORY;

	// init gui system
	mGUI->Init( );

	// setup resource folders for gui system
	mGUI->GetResourceProviderPtr( )->setResourceGroupDirectory( "schemes", "schemes/" );
	mGUI->GetResourceProviderPtr( )->setResourceGroupDirectory( "imagesets", "imagesets/" );
	mGUI->GetResourceProviderPtr( )->setResourceGroupDirectory( "fonts", "fonts/" );
	mGUI->GetResourceProviderPtr( )->setResourceGroupDirectory( "layouts", "layouts/" );
	mGUI->GetResourceProviderPtr( )->setResourceGroupDirectory( "looknfeel", "looknfeel/" );
	mGUI->GetResourceProviderPtr( )->setResourceGroupDirectory( "lua_scripts", "lua_scripts/" );

	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	// load one of default schemes
	// for more information about gui schemes concept visit http://www.cegui.org.uk/wiki/index.php/Scheme_files
	CEGUI::SchemeManager::getSingleton().loadScheme( "TaharezLook.scheme" );

	// set mouse cursor
	CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" );

	// load default gui layout
	CEGUI::Window* myRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout( "editordefault.layout" );
	CEGUI::System::getSingleton().setGUISheet( myRoot );

	// hide mouse cursor
	ShowCursor( false );

	return res;
}

void Application::Shutdown( )
{
	if( mGUI )
		mGUI->Shutdown( );

	mGUI.reset( );

	// destroy lua state
	xfx::LuaScript::Instance( ).Close( );

	// shutdown renderer
	xfx::Renderer::Instance( ).Shutdown( );

	// call base method
	xfx::Application::Shutdown( );
}

void Application::Update( )
{
	// that means we are in shutdown process, no update-render then
	if( !HWnd( ) )
		return;

	// Update timer.
	mTimer.Update( );

	// Update GUI.
	if( mGUI )
	{
		if( mTimer.MSPF100( ) > 0 )
		{
			char fps[ 10 ];
			_snprintf( fps, 10, "FPS: %d", 10000 / mTimer.MSPF100( ) );

			CEGUI::System::getSingleton( ).getGUISheet( )->getChildAtIdx( 0 )->setText( fps );
		}

		mGUI->Update( mTimer.MicroSeconds100( ) );
	}

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
	switch( msg )
	{
	case WM_SIZE:
		{
			// resize gui and renderer back buffer
			RECT wndrect;
			GetClientRect( hwnd, &wndrect );

			xfx::Renderer::Instance( ).ResetDevice( wndrect.right, wndrect.bottom );
			xfx::Viewport::Instance( ).Init( wndrect.right, wndrect.bottom );

			if( mGUI )
			{
				CEGUI::Window * wnd = CEGUI::System::getSingleton().getGUISheet( );
				if( wnd )
					wnd->setArea( CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 1.0f, 0.0f ), CEGUI::UDim( 1.0f, 0.0f ) );
			}
		}
		break;

	case WM_PAINT:
		// redraw window
		xfx::Viewport::Instance( ).Render( );
		return TRUE;
	}

	if( mGUI )
		mGUI->ProcessWndMessage( hwnd, msg, wparam, lparam );

	return xfx::Application::WndProc( hwnd, msg, wparam, lparam );
}
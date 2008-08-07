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

	// init lua script system
	xfx::LuaScript::Instance( ).Open( );

	// register default shader that will be used if another one won't provided
	xfx::Renderer::Instance( ).SetDefaultShader( xfx::Shader::Cache( ).Register( "default.shader" ) );

	mGUI.reset( new xfx::GUI( ) );

	if( !mGUI )
		return XFXERR_OUTOFMEMORY;

	mGUI->Init( );

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

	CEGUI::SchemeManager::getSingleton().loadScheme( "TaharezLook.scheme" );

	if(! CEGUI::FontManager::getSingleton().isFontPresent( "Commonwealth-10" ) )
		CEGUI::FontManager::getSingleton().createFont( "Commonwealth-10.font" );

	CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" );

	CEGUI::Window* myRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout( "editordefault.layout" );
	CEGUI::System::getSingleton().setGUISheet( myRoot );

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
	switch( msg )
	{
	case WM_SIZE:
		{
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
		xfx::Viewport::Instance( ).Render( );
		return TRUE;
	}

	if( mGUI )
		mGUI->ProcessWndMessage( hwnd, msg, wparam, lparam );

	return xfx::Application::WndProc( hwnd, msg, wparam, lparam );
}
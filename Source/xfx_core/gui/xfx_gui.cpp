//  File xfx_gui.cpp                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_gui.h"
#include "xfx_cegui_renderer.h"
#include "xfx_cegui_resourceprovider.h"
#include "xfx_cegui_luabind_scriptmodule.h"

_XFX_BEGIN







//
// GUI
//

GUI::GUI( )
{
}

GUI::~GUI( )
{
}

HRESULT GUI::Init( )
{
	mResourceProviderPtr.reset( new CEGUIResourceProvider( ) );

	if( mResourceProviderPtr == NULL )
		return XFXERR_OUTOFMEMORY;

	mRendererPtr.reset( new CEGUIRenderer( ) );

	if( mRendererPtr == NULL )
		return XFXERR_OUTOFMEMORY;

	mScriptModulePtr.reset( new CEGUI::LuaScriptModule( LuaScript::Instance( ).GetLuaVM( ) ) );

	if( mScriptModulePtr == NULL )
		return XFXERR_OUTOFMEMORY;

	mSystemPtr.reset( new CEGUI::System( mRendererPtr.get( ), mResourceProviderPtr.get( ), 0, mScriptModulePtr.get( ) ) );

	if( mSystemPtr == NULL )
		return XFXERR_OUTOFMEMORY;

	return S_OK;
}

void GUI::Shutdown( )
{
	mSystemPtr.reset( );
	mScriptModulePtr.reset( );
	mRendererPtr.reset( );
	mResourceProviderPtr.reset( );
}

bool GUI::ProcessWndMessage( HWND /*hwnd*/, UINT msg, WPARAM wparam, LPARAM lparam )
{
	if( !mSystemPtr )
		return false;

	switch( msg )
	{
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
		{
			float x = static_cast< float >( LOWORD( lparam ) ) / Renderer::Instance( ).D3DPP( ).BackBufferWidth * Viewport::Instance( ).Width( );
			float y = static_cast< float >( HIWORD( lparam ) ) / Renderer::Instance( ).D3DPP( ).BackBufferHeight * Viewport::Instance( ).Height( );

			mSystemPtr->injectMousePosition( x, y );

			switch( msg )
			{
			case WM_LBUTTONDOWN:
				mSystemPtr->injectMouseButtonDown( CEGUI::LeftButton );
				break;

			case WM_LBUTTONUP:
				mSystemPtr->injectMouseButtonUp( CEGUI::LeftButton );
				break;

			case WM_RBUTTONDOWN:
				mSystemPtr->injectMouseButtonDown( CEGUI::RightButton );
				break;

			case WM_RBUTTONUP:
				mSystemPtr->injectMouseButtonUp( CEGUI::RightButton );
				break;

			case WM_MBUTTONDOWN:
				mSystemPtr->injectMouseButtonDown( CEGUI::MiddleButton );
				break;

			case WM_MBUTTONUP:
				mSystemPtr->injectMouseButtonUp( CEGUI::MiddleButton );
				break;
			}
		}
		return true;

	case WM_CHAR:
		{
			BYTE key_state[ 256 ];

			if( GetKeyboardState( key_state ) )
			{
				wchar_t unicode_char;
				if( ToUnicode(
					static_cast< UINT >( wparam ),
					( static_cast< UINT >( lparam ) >> 0x10 ) & 0xff,
					key_state,
					&unicode_char,
					1,
					0 ) > 0
					)
				{
					mSystemPtr->injectChar( unicode_char );
				}
			}
		}
		return true;
	}

	return false;
}

void GUI::Render( ) const
{
	mSystemPtr->renderGUI( );
}

void GUI::Update( DWORD mspf )
{
	mSystemPtr->injectTimePulse( 0.0001f * mspf );
}




_XFX_END
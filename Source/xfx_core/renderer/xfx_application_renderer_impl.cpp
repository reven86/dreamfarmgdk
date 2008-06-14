//  File xfx_application_renderer_impl.cpp                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_renderer.h"
#include "xfx_viewport.h"




_XFX_BEGIN









//
// Application renderer-specific methods
//

HRESULT Application::InitRenderer( )
{
	HRESULT hr = Renderer::Instance( ).CreateDevice( );

	if( SUCCEEDED( hr ) )
	{
		mFullscreenChangeConnection = Renderer::Instance( ).AddFullscreenChangeCallback( boost::bind( &Application::OnFullscreenChange, this, _1, _2, _3 ) );

		mSavedWindowStyle	= GetWindowLong( mWnd, GWL_STYLE );
		mSavedWindowExStyle	= GetWindowLong( mWnd, GWL_EXSTYLE );

		RECT rect;
		GetClientRect( mWnd, &rect );

		Viewport::Instance( ).Init( rect.right - rect.left, rect.bottom - rect.top );
	}

	return hr;
}

void Application::OnFullscreenChange( bool isfullscreen, unsigned width, unsigned height )
{
	if( !isfullscreen )
	{
		SetWindowLong ( mWnd, GWL_STYLE, mSavedWindowStyle );
		SetWindowLong ( mWnd, GWL_EXSTYLE, mSavedWindowExStyle );
	}
	else
	{
		SetWindowLong ( mWnd, GWL_STYLE, WS_VISIBLE );
	}

	SetWindowPos ( mWnd, isfullscreen ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, width, height, SWP_SHOWWINDOW );
}



_XFX_END
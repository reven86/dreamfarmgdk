//  File xfx_gui.cpp                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_gui.h"
#include "xfx_cegui_renderer.h"

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
	mRendererPtr.reset( new CEGUIRenderer( ) );

	if( mRendererPtr == NULL )
		return XFXERR_OUTOFMEMORY;

	mSystemPtr.reset( new CEGUI::System( mRendererPtr.get( ) ) );

	if( mSystemPtr == NULL )
		return XFXERR_OUTOFMEMORY;

	return S_OK;
}

void GUI::Shutdown( )
{
	mSystemPtr.reset( );
	mRendererPtr.reset( );
}

void GUI::Render( ) const
{
	mSystemPtr->renderGUI( );
}




_XFX_END
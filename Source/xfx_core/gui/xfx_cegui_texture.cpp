//  File xfx_cegui_texture.cpp                                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_cegui_texture.h"
#include "xfx_cegui_renderer.h"

_XFX_BEGIN






//
// CEGUITexture
//

CEGUITexture::CEGUITexture( class CEGUIRenderer * renderer ) :
	CEGUI::Texture( renderer )
{
}

CEGUITexture::~CEGUITexture( )
{
}

void CEGUITexture::SetSize( unsigned size )
{
}

CEGUI::ushort CEGUITexture::getWidth( ) const
{
	return 0;
}

CEGUI::ushort CEGUITexture::getHeight( ) const
{
	return 0;
}

void CEGUITexture::loadFromFile( const CEGUI::String& filename, const CEGUI::String& resourceGroup )
{
}

void CEGUITexture::loadFromMemory(const void *buffPtr, CEGUI::uint buffWidth, CEGUI::uint buffHeight, CEGUI::Texture::PixelFormat pixelFormat)
{
}



_XFX_END
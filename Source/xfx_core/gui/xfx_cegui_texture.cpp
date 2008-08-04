//  File xfx_cegui_texture.cpp                                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_cegui_texture.h"
#include "xfx_cegui_renderer.h"
#include "xfx_cegui_resourceprovider.h"

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

void CEGUITexture::SetSize( unsigned /*size*/ )
{
	_ASSERTE( !"CEGUITexture::SetSize not implemented yet." );
}

CEGUI::ushort CEGUITexture::getWidth( ) const
{
	if( !mShaderPtr )
		return 0;

	const Shader::TextureInfo * texinfo = mShaderPtr->GetTextureInfoOnStage( 0 );

	if( !texinfo || !texinfo->texture )
		return 0;

	return static_cast< CEGUI::ushort >( texinfo->frame_width * texinfo->texture->Width( ) );
}

CEGUI::ushort CEGUITexture::getHeight( ) const
{
	if( !mShaderPtr )
		return 0;

	const Shader::TextureInfo * texinfo = mShaderPtr->GetTextureInfoOnStage( 0 );

	if( !texinfo || !texinfo->texture )
		return 0;

	return static_cast< CEGUI::ushort >( texinfo->frame_height * texinfo->texture->Height( ) );
}

void CEGUITexture::loadFromFile( const CEGUI::String& filename, const CEGUI::String& resourceGroup )
{
	mShaderPtr.reset( );

	CEGUIResourceProvider * rp = static_cast< CEGUIResourceProvider * >( CEGUI::System::getSingleton( ).getResourceProvider( ) );
	CEGUI::String final_filename = rp->getFinalFilename( filename, resourceGroup );

	mShaderPtr = Shader::Cache( ).Register( final_filename.c_str( ) );
}

void CEGUITexture::loadFromMemory(const void * /*buffPtr*/, CEGUI::uint /*buffWidth*/, CEGUI::uint /*buffHeight*/, CEGUI::Texture::PixelFormat /*pixelFormat*/)
{
	_ASSERTE( !"CEGUITexture::loadFromMemory not implemented yet." );
}



_XFX_END
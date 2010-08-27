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

	return static_cast< CEGUI::ushort >( texinfo->frame_width * texinfo->texture->GetWidth( ) );
}

CEGUI::ushort CEGUITexture::getHeight( ) const
{
	if( !mShaderPtr )
		return 0;

	const Shader::TextureInfo * texinfo = mShaderPtr->GetTextureInfoOnStage( 0 );

	if( !texinfo || !texinfo->texture )
		return 0;

	return static_cast< CEGUI::ushort >( texinfo->frame_height * texinfo->texture->GetHeight( ) );
}

void CEGUITexture::loadFromFile( const CEGUI::String& filename, const CEGUI::String& resourceGroup )
{
	mShaderPtr.reset( );

	CEGUIResourceProvider * rp = static_cast< CEGUIResourceProvider * >( CEGUI::System::getSingleton( ).getResourceProvider( ) );
	CEGUI::String final_filename = rp->getFinalFilename( filename, resourceGroup );

	mShaderPtr = Shader::Cache( ).Register( final_filename.c_str( ) );

	const boost::shared_ptr< const Shader >& def_shd = Renderer::Instance( ).GetDefaultShader( );

	if( !mShaderPtr && def_shd )
	{
		// shader not loaded
		// try to load texture
		boost::shared_ptr< const xfx::Texture > tex = xfx::Texture::Cache( ).Register( final_filename.c_str( ) );

		if( tex )
		{
			// texture has been loaded
			// create default shader for it
			boost::shared_ptr< Shader > new_shd( new Shader( *def_shd ) );

			mShaderPtr = new_shd;

			Shader::TextureInfo tex_info;
			tex_info.stage_name = "DiffuseTex";
			tex_info.fps = 1;
			tex_info.frame_count = 1;
			tex_info.frame_height = 1.0f;
			tex_info.frame_width = 1.0f;
			tex_info.texture = tex;

			new_shd->AddTextureMap( tex_info );
		}
	}
}

void CEGUITexture::loadFromMemory(const void * buffPtr, CEGUI::uint buffWidth, CEGUI::uint buffHeight, CEGUI::Texture::PixelFormat pixelFormat )
{
	const boost::shared_ptr< const Shader >& def_shd = Renderer::Instance( ).GetDefaultShader( );
	if( !def_shd )
	{
		// print something?
		return;
	}

	// create texture
	boost::shared_ptr< xfx::Texture > tex( new xfx::Texture( ) );
	if( FAILED( tex->Create( buffWidth, buffHeight, 1, D3DFMT_A8R8G8B8 ) ) )
	{
		// print something?
		return;
	}

	boost::shared_ptr< Shader > new_shd( new Shader( *def_shd ) );
	mShaderPtr = new_shd;

	ARGB * new_buff = reinterpret_cast< ARGB * >( alloca( buffWidth * buffHeight * sizeof( DWORD ) ) );
	const BYTE * bb = reinterpret_cast< const BYTE * >( buffPtr );
	size_t step = pixelFormat == PF_RGB ? 3 : 4;
	for( unsigned y = 0; y < buffHeight; y++ )
		for( unsigned x = 0; x < buffWidth; x++, bb += step )
			new_buff[ y * buffWidth + x ] = ARGB( pixelFormat == PF_RGB ? 0xff : *( bb + 3 ), *( bb + 0 ), *( bb + 1 ), *( bb + 2 ) );

	tex->SetSurfaceData( 0, new_buff );

	Shader::TextureInfo tex_info;
	tex_info.stage_name = "DiffuseTex";
	tex_info.fps = 1;
	tex_info.frame_count = 1;
	tex_info.frame_height = 1.0f;
	tex_info.frame_width = 1.0f;
	tex_info.texture = tex;

	new_shd->AddTextureMap( tex_info );
}



_XFX_END
//  File xfx_cegui_renderer.cpp                                               |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_cegui_renderer.h"
#include "xfx_cegui_texture.h"

_XFX_BEGIN





//
// CEGUIRenderer
//


CEGUIRenderer::CEGUIRenderer( ) :
	mEnableQueueing( true )
{
	mQuadArray.reserve( 256 );
	d_identifierString = "XFX::CEGUIRenderer - renderer to CEGUI for Dream Farm GDK";
}

CEGUIRenderer::~CEGUIRenderer( )
{
}

void CEGUIRenderer::addQuad(const CEGUI::Rect &dest_rect, float z, const CEGUI::Texture *tex, const CEGUI::Rect &texture_rect, const CEGUI::ColourRect &colours, CEGUI::QuadSplitMode /*quad_split_mode*/)
{
	QuadElem el;
	el.x = dest_rect.d_left;
	el.y = dest_rect.d_top;
	el.w = dest_rect.d_right - dest_rect.d_left;
	el.h = dest_rect.d_bottom - dest_rect.d_top;
	el.z = z;
	el.tex = static_cast< const CEGUITexture * >( tex );
	el.u0 = texture_rect.d_left;
	el.v0 = texture_rect.d_top;
	el.u1 = texture_rect.d_right;
	el.v1 = texture_rect.d_bottom;
	el.c = colours.d_top_left.getARGB( );

	if( mEnableQueueing )
	{
		mQuadArray.push_back( el );
	}
	else
	{
		// map viewport coordinates to screen-space
		float v2sx = Viewport::Instance( ).InvWidth( ) * xfx::Renderer::Instance( ).D3DPP( ).BackBufferWidth;
		float v2sy = Viewport::Instance( ).InvHeight( ) * xfx::Renderer::Instance( ).D3DPP( ).BackBufferHeight;

		DrawTools& dt = xfx::Renderer::Instance( ).GetDrawTools( );

		dt.PushDraw2DSprite(
			( el.x + el.w * 0.5f ) * v2sx,
			( el.y + el.h * 0.5f ) * v2sy,
			el.w * v2sx, el.h * v2sy,
			0, el.c,
			el.tex->GetShaderPtr( ),
			el.tex->GetShaderParamsPtr( ),
			el.u0, el.v0, el.u1, el.v1,
			el.z
			);
	}
}

void CEGUIRenderer::doRender()
{
	// map viewport coordinates to screen-space
	float v2sx = Viewport::Instance( ).InvWidth( ) * xfx::Renderer::Instance( ).D3DPP( ).BackBufferWidth;
	float v2sy = Viewport::Instance( ).InvHeight( ) * xfx::Renderer::Instance( ).D3DPP( ).BackBufferHeight;

	DrawTools& dt = xfx::Renderer::Instance( ).GetDrawTools( );

	BOOST_FOREACH( const QuadElem& el, mQuadArray )
	{
		dt.PushDraw2DSprite(
			( el.x + el.w * 0.5f ) * v2sx,
			( el.y + el.h * 0.5f ) * v2sy,
			el.w * v2sx, el.h * v2sy,
			0, el.c,
			el.tex->GetShaderPtr( ),
			el.tex->GetShaderParamsPtr( ),
			el.u0, el.v0, el.u1, el.v1,
			el.z
			);
	}
}

void CEGUIRenderer::clearRenderList()
{
	mQuadArray.clear( );
}

void CEGUIRenderer::setQueueingEnabled( bool setting )
{
	mEnableQueueing = setting;
}

CEGUI::Texture * CEGUIRenderer::createTexture()
{
	CEGUITexture * res = new CEGUITexture( this );

	mTextures.push_back( res );

	return res;
}

CEGUI::Texture * CEGUIRenderer::createTexture(const CEGUI::String &filename, const CEGUI::String &resourceGroup)
{
	CEGUITexture * res = new CEGUITexture( this );

	if( !res )
		return res;

	res->loadFromFile( filename, resourceGroup );
	mTextures.push_back( res );

	return res;
}

CEGUI::Texture * CEGUIRenderer::createTexture(float size)
{
	CEGUITexture * res = new CEGUITexture( this );

	if( !res )
		return res;

	res->SetSize( static_cast< unsigned >( size ) );
	mTextures.push_back( res );

	return NULL;
}

void CEGUIRenderer::destroyTexture(CEGUI::Texture *texture)
{
	std::vector< CEGUITexture * >::iterator e = std::find( mTextures.begin( ), mTextures.end( ), texture );

	if( e != mTextures.end( ) )
		mTextures.erase( e );

	CEGUITexture * tex = static_cast< CEGUITexture * >( texture );

	delete tex;
}

void CEGUIRenderer::destroyAllTextures()
{
	BOOST_FOREACH( CEGUITexture * tex, mTextures )
	{
		delete tex;
	}

	std::vector< CEGUITexture * >( ).swap( mTextures );
}

bool CEGUIRenderer::isQueueingEnabled() const
{
	return mEnableQueueing;
}

float CEGUIRenderer::getWidth() const
{
	return static_cast< float >( Viewport::Instance( ).GetWidth( ) );
}

float CEGUIRenderer::getHeight() const
{
	return static_cast< float >( Viewport::Instance( ).GetHeight( ) );
}

CEGUI::Size CEGUIRenderer::getSize() const
{
	return CEGUI::Size( getWidth( ), getHeight( ) );
}

CEGUI::Rect CEGUIRenderer::getRect() const
{
	return CEGUI::Rect( 0, 0, getWidth( ), getHeight( ) );
}

CEGUI::uint CEGUIRenderer::getMaxTextureSize() const
{
	return xfx::Renderer::Instance( ).D3DCaps( ).MaxTextureWidth;
}

CEGUI::uint CEGUIRenderer::getHorzScreenDPI() const
{
	//_ASSERTE( !"CEGUIRenderer::getHorzScreenDPI not implemented." );
	return 96;
}

CEGUI::uint CEGUIRenderer::getVertScreenDPI() const
{
	//_ASSERTE( !"CEGUIRenderer::getVertScreenDPI not implemented." );
	return 96;
}





_XFX_END
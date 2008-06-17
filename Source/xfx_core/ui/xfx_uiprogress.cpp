//  File xfx_uiprogress.cpp                                                   |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_uiprogress.h"
#include "renderer/xfx_renderer.h"
#include "renderer/xfx_shader.h"
#include "renderer/xfx_viewport.h"

_XFX_BEGIN





//
// UIProgress
//

UIProgress::UIProgress( ) :
	mShaderPtr( ),
	mShaderConstsPtr( ),
	mProgressValue( 1.0f ),
	mIsVertical( false )
{
	AddToken( "progress.shader",			boost::bind( &UIProgress::ParseShader, this, _1, _2 ) );
	AddToken( "progress.value",				boost::bind( &ParseVariable< float >, boost::ref( mProgressValue ), _1, _2 ) );
	AddToken( "progress.vertical",			boost::bind( &ParseVariable< bool >, boost::ref( mIsVertical ), _1, _2 ) );
}

UIProgress::~UIProgress( )
{
}

void UIProgress::ShaderPtr( const boost::shared_ptr< const Shader >& shd )
{
	mShaderPtr = shd;
}

void UIProgress::Render( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const
{
	ARGB newcolor = Color( ) * color;
	if( newcolor.a == 0 )
		return;

	float iw		= Viewport::Instance( ).Width( ) * 0.5f;
	float ih		= Viewport::Instance( ).Height( ) * 0.5f;

	float kw		= Viewport::Instance( ).InvWidth( ) * Renderer::Instance( ).D3DPP( ).BackBufferWidth;
	float kh		= Viewport::Instance( ).InvHeight( ) * Renderer::Instance( ).D3DPP( ).BackBufferHeight;

	if( mIsVertical )
	{
		float new_height = Height( ) * mProgressValue;
		float new_y = Y( ) - Height( ) + new_height;

		Renderer::Instance( ).DrawTools( ).PushDraw2DSprite(
			( X( ) * scale.x + iw + ofs.x ) * kw, ( ih - new_y * scale.y - ofs.x ) * kh, 
			Width( ) * scale.x * 2.0f * kw, new_height * scale.y * 2.0f * kh,
			0, newcolor, mShaderPtr, mShaderConstsPtr, 0.0f, 1.0f - mProgressValue, 1.0f, 1.0f );
	}
	else
	{
		float new_width = Width( ) * mProgressValue;
		float new_x = X( ) - Width( ) + new_width;

		Renderer::Instance( ).DrawTools( ).PushDraw2DSprite(
			( new_x * scale.x + iw + ofs.x ) * kw, ( ih - Y( ) * scale.y - ofs.x ) * kh, 
			new_width * scale.x * 2.0f * kw, Height( ) * scale.y * 2.0f * kh,
			0, newcolor, mShaderPtr, mShaderConstsPtr, 0.0f, 0.0f, mProgressValue, 1.0f );
	}
}

void UIProgress::Update( DWORD )
{
}

HRESULT UIProgress::PrepareParsing( )
{
	mShaderPtr.reset( );
	mProgressValue = 0.5f;

	return UIObject::PrepareParsing( );
}

HRESULT UIProgress::AfterParsing( )
{
	return UIObject::AfterParsing( );
}

HRESULT UIProgress::ParseShader( String::size_type& pos, const String& str )
{
	return ( mShaderPtr = Shader::Cache( ).Register( next_token( str, pos ) ) ) ? S_OK : XFXERR_IO;
}

void UIProgress::LuaRegister( lua_State * L )
{
#ifdef __XFX_USE_LUA_LUABIND__
	luabind::module( L )
	[
		luabind::class_< UIProgress, UIObject >( "UIProgress" )
			.property( "shader",
				( const boost::shared_ptr< const Shader >& ( UIProgress::* ) ( ) const ) &UIProgress::ShaderPtr,
				( void ( UIProgress::* ) ( const boost::shared_ptr< const Shader >& ) ) &UIProgress::ShaderPtr
				)
			.property( "progress_value",
				( const float& ( UIProgress::* ) ( ) const ) &UIProgress::ProgressValue,
				( void ( UIProgress::* ) ( const float& ) ) &UIProgress::ProgressValue
				)
			.property( "vertical",
				&UIProgress::IsVertical,
				&UIProgress::SetVertical
				)
	];
#endif
}





_XFX_END
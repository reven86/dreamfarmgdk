//  File xfx_uisprite.cpp                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_uisprite.h"
#include "renderer/xfx_renderer.h"
#include "renderer/xfx_shader.h"
#include "renderer/xfx_viewport.h"

_XFX_BEGIN





//-----------------------------------------------------------------------------
//                           Methods
//-----------------------------------------------------------------------------
//UISprite
UISprite::UISprite( ) :
	mShaders( ),
	mShaderParamsPtr( ),
	mCurrentShaderIndex( 0 ),
	mRoll( 0 )
{
	AddToken( "sprite.shader",			boost::bind( &UISprite::ParseShader, this, _1, _2 ) );
	AddToken( "sprite.roll",			boost::bind( &ParseAngleVariable, boost::ref( mRoll ), _1, _2 ) );
}

UISprite::~UISprite ()
{
}

void UISprite::ShaderPtr( unsigned ind, const boost::shared_ptr< const Shader >& shd )
{
	_ASSERTE( ind >=0 && ind < ShadersCount( ) );

	if( ind >= 0 && ind < ShadersCount( ) )
		mShaders[ ind ] = shd;
}

void UISprite::Render( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const
{
	if( mCurrentShaderIndex >= ShadersCount( ) )
		return;

	ARGB newcolor = Color( ) * color;
	if( newcolor.a == 0 )
		return;

	float iw		= Viewport::Instance( ).Width( ) * 0.5f;
	float ih		= Viewport::Instance( ).Height( ) * 0.5f;

	float kw		= Viewport::Instance( ).InvWidth( ) * Renderer::Instance( ).D3DPP( ).BackBufferWidth;
	float kh		= Viewport::Instance( ).InvHeight( ) * Renderer::Instance( ).D3DPP( ).BackBufferHeight;

	Renderer::Instance ().DrawTools ().PushDraw2DSprite(
		( X( ) * scale.x + iw + ofs.x ) * kw, ( ih - Y( ) * scale.y - ofs.y ) * kh,
		Width( ) * scale.x * 2.0f * kw, Height( ) * scale.y * 2.0f * kh,
		mRoll, newcolor, mShaders[ mCurrentShaderIndex ], mShaderParamsPtr, 0.0f, 0.0f, 1.0f, 1.0f );
}

void UISprite::Update( DWORD )
{
}

HRESULT UISprite::PrepareParsing ()
{
	std::vector< boost::shared_ptr< const class Shader > >( ).swap( mShaders );
	mCurrentShaderIndex = 0;
	mRoll = 0;

	return UIObject::PrepareParsing ();
}

HRESULT UISprite::AfterParsing ()
{
	return UIObject::AfterParsing ();
}

HRESULT UISprite::ParseShader( String::size_type& pos, const String& str )
{
	boost::shared_ptr< const Shader > shd = Shader::Cache( ).Register( next_token( str, pos ) );

	if( shd )
	{
		mShaders.push_back( shd );
		return S_OK;
	}

	return XFXERR_IO;
}

void UISprite::LuaRegister( lua_State * L )
{
	luabind::module( L )
	[
		luabind::class_< UISprite, UIObject >( "UISprite" )
			.property( "cur_shader",
				&UISprite::GetCurrentShaderIndex,
				&UISprite::SetCurrentShaderIndex
				)
			.def( "get_shader",
				( const boost::shared_ptr< const Shader >& ( UISprite::* ) ( unsigned ) const ) &UISprite::ShaderPtr
				)
			.def( "set_shader",
				( void ( UISprite::* ) ( unsigned, const boost::shared_ptr< const Shader >& ) ) &UISprite::ShaderPtr
				)
			.def( "roll",
				&UISprite::GetRollAngle,
				&UISprite::SetRollAngle
				)
	];
}



_XFX_END
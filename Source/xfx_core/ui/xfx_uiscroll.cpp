//  File xfx_uiscroll.cpp                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_uiscroll.h"
#include "renderer/xfx_renderer.h"
#include "renderer/xfx_shader.h"
#include "renderer/xfx_texture.h"
#include "renderer/xfx_viewport.h"

_XFX_BEGIN




//
// UIScroll
//

UIScroll::UIScroll( ) : 
	mScrollPos( 0.0f ),
	mArrowStep( 0.1f ),
	mScrollFlags( 0 )
{
	AddToken( "scroll.arrow1_shader",				boost::bind( &UIScroll::ParseShader, this, EST_FIRST_ARROW, _1, _2 ) );
	AddToken( "scroll.arrow2_shader",				boost::bind( &UIScroll::ParseShader, this, EST_SECOND_ARROW, _1, _2 ) );
	AddToken( "scroll.fill_shader",					boost::bind( &UIScroll::ParseShader, this, EST_FILL, _1, _2 ) );
	AddToken( "scroll.pos_shader",					boost::bind( &UIScroll::ParseShader, this, EST_SCROLL, _1, _2 ) );
	AddToken( "scroll.arrow_step",					boost::bind( &UIScroll::ParseVariable< float >, boost::ref( mArrowStep ), _1, _2 ) );
	AddToken( "scroll.position",					boost::bind( &UIScroll::ParseVariable< float >, boost::ref( mScrollPos ), _1, _2 ) );
}

UIScroll::~UIScroll( )
{
}

const boost::shared_ptr< const Shader >& UIScroll::GetScrollShader( const EShaderType shd_type ) const
{
	if( shd_type >= 0 && shd_type < EST_MAX_SHADERS )
		return mShaders[ shd_type ].get< 0 >( );

	_ASSERTE( !"Invalid shader type passed in UIScroll::GetScrollShader" );

	static boost::shared_ptr< const Shader > error_shd;
	return error_shd;
}

void UIScroll::SetScrollShader( const EShaderType shd_type, const boost::shared_ptr< const Shader >& shd )
{
	mShaders[ shd_type ].get< 0 >( ) = shd;

	PrepareScrollToRender( );
}

void UIScroll::SetScrollPosition( float pos )
{
	if( pos < 0.0f )
		pos = 0.0f;
	else if( pos > 1.0f )
		pos = 1.0f;

	mScrollPos = pos;
	PrepareScrollToRender( );

	// call lua event.
	if( ( GetFlags( ) & FL_NAMED ) != 0 )
	{
		String event_str ( boost::str( StringFormat( "OnChangeScrollPos_%s" ) % Name( ) ) );

		if( SUCCEEDED( LuaScript::Instance( ).IsFunction( event_str.c_str( ) ) ) )
		{
			try
			{
				luabind::call_function< void >(
					LuaScript::Instance( ).GetLuaVM( ),
					event_str.c_str( ),
					this );
			}
			catch( std::exception& e )
			{
				gError( "Call to lua function %s failed, reason: %s", event_str.c_str( ), e.what( ) );
			}
			catch( ... )
			{
				gError( "Call to lua function %s failed, reason: unknown", event_str.c_str( ) );
			}
		}
	}
}

void UIScroll::Update( DWORD mspf )
{
	if( mScrollFlags & ESF_PUSH_ARROW1 )
		SetScrollPosition( GetScrollPosition( ) - mArrowStep * 0.0001f * mspf );

	if( mScrollFlags & ESF_PUSH_ARROW2 )
		SetScrollPosition( GetScrollPosition( ) + mArrowStep * 0.0001f * mspf );
}

bool UIScroll::ProcessMouse( float x, float y, UINT mousemsg, WPARAM wParam )
{
	bool res = UIObject::ProcessMouse( x, y, mousemsg, wParam );

	if( IsActive( ) && IsInside( x, y ) && 
		( mousemsg == WM_LBUTTONDOWN || mousemsg == WM_MOUSEMOVE && ( wParam & MK_LBUTTON ) != 0 ) )
	{
		if( CheckOnFirstArrow( x, y ) )
			mScrollFlags |= ESF_PUSH_ARROW1;
		else
			mScrollFlags &= ~ESF_PUSH_ARROW1;

		if( CheckOnSecondArrow( x, y ) )
			mScrollFlags |= ESF_PUSH_ARROW2;
		else
			mScrollFlags &= ~ESF_PUSH_ARROW2;

		if( mScrollFlags == 0 )
		{
			// noting happens. check for cursor move.
			SetScrollPosition( GetScrollPosFromXY( x, y ) );
		}
	}
	else
	{
		mScrollFlags = 0;
	}

	return res;
}

void UIScroll::ResetState( )
{
	UIObject::ResetState( );
	mScrollFlags = 0;
}

void UIScroll::PrepareScrollToRender( )
{
	for( int i = 0; i < EST_MAX_SHADERS; i++ )
	{
		if( !mShaders[ i ].get< 0 >( ) )
		{
			mShaders[ i ].get< 1 >( ).xy( 1.0f, 1.0f );
			continue;
		}

		const Shader::TextureInfo * tex_info = mShaders[ i ].get< 0 >( )->GetTextureInfoOnStage( 0 );

		_ASSERTE( tex_info );
		if( !tex_info || !tex_info->texture )
		{
			mShaders[ i ].get< 1 >( ).xy( 1.0f, 1.0f );
			continue;
		}

		mShaders[ i ].get< 1 >( ).xy(
			tex_info->frame_width * tex_info->texture->Width( ),
			tex_info->frame_height * tex_info->texture->Height( )
			);
	}
}

HRESULT UIScroll::PrepareParsing( )
{
	return UIObject::PrepareParsing( );
}

HRESULT UIScroll::AfterParsing( )
{
	// clamp scroll pos and prepare to render
	SetScrollPosition( mScrollPos );
	mScrollFlags = 0;

	return UIObject::AfterParsing( );
}

HRESULT UIScroll::ParseShader( const EShaderType& type, String::size_type& pos, const String& str )
{
	boost::shared_ptr< const Shader > shd = Shader::Cache( ).Register( next_token( str, pos ) );

	if( shd )
	{
		SetScrollShader( type, shd );
		return S_OK;
	}

	return XFXERR_IO;
}

void UIScroll::LuaRegister( lua_State * L )
{
	luabind::module( L )
	[
		luabind::class_< UIScroll, UIObject >( "UIScroll" )
			.property( "scroll_position",
				&UIScroll::GetScrollPosition,
				&UIScroll::SetScrollPosition
				)
			.property( "scroll_step",
				&UIScroll::GetScrollStep,
				&UIScroll::SetScrollStep
				)
			.def( "get_shader", &UIScroll::GetScrollShader )
			.def( "set_shader", &UIScroll::SetScrollShader )
	];

	luabind::object uiscroll		= luabind::globals( L );
	uiscroll						= uiscroll[ "UIScroll" ];
	uiscroll[ "EST_FIRST_ARROW" ]	= EST_FIRST_ARROW;
	uiscroll[ "EST_SECOND_ARROW" ]	= EST_SECOND_ARROW;
	uiscroll[ "EST_FILL" ]			= EST_FILL;
	uiscroll[ "EST_SCROLL" ]		= EST_SCROLL;
	uiscroll[ "EST_MAX_SHADERS" ]	= EST_MAX_SHADERS;
}





//
// UIHScroll
//

UIHScroll::UIHScroll( )
{
}

UIHScroll::~UIHScroll( )
{
}

bool UIHScroll::CheckOnFirstArrow( float x, float y ) const
{
	float height	= 2.0f * Height( );
	float arrow1w	= height * mShaders[ EST_FIRST_ARROW ].get< 1 >( ).x / mShaders[ EST_FIRST_ARROW ].get< 1 >( ).y;
	float arrow1x	= X( ) - Width( ) + 0.5f * arrow1w;

	return fabs( x - arrow1x ) < 0.5f * arrow1w && fabs( y - Y( ) ) < Height( );
}

bool UIHScroll::CheckOnSecondArrow( float x, float y ) const
{
	float height	= 2.0f * Height( );
	float arrow2w	= height * mShaders[ EST_SECOND_ARROW ].get< 1 >( ).x / mShaders[ EST_SECOND_ARROW ].get< 1 >( ).y;
	float arrow2x	= X( ) + Width( ) - 0.5f * arrow2w;

	return fabs( x - arrow2x ) < 0.5f * arrow2w && fabs( y - Y( ) ) < Height( );
}

float UIHScroll::GetScrollPosFromXY( float x, float ) const
{
	float height	= 2.0f * Height( );
	float arrow1w	= height * mShaders[ EST_FIRST_ARROW ].get< 1 >( ).x / mShaders[ EST_FIRST_ARROW ].get< 1 >( ).y;
	float arrow1x	= X( ) - Width( ) + 0.5f * arrow1w;
	float arrow2w	= height * mShaders[ EST_SECOND_ARROW ].get< 1 >( ).x / mShaders[ EST_SECOND_ARROW ].get< 1 >( ).y;
	float arrow2x	= X( ) + Width( ) - 0.5f * arrow2w;
	float fillw		= 2.0f * Width( ) - arrow1w - arrow2w;
	float fillx		= 0.5f * ( arrow1x + arrow2x );
	float scrollw	= height * mShaders[ EST_SCROLL ].get< 1 >( ).x / mShaders[ EST_SCROLL ].get< 1 >( ).y;

	return ( x - ( fillx - fillw * 0.5f + scrollw * 0.5f ) ) / ( fillw - scrollw );
}

void UIHScroll::Render( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const
{
	ARGB newcolor = Color( ) * color;
	if( newcolor.a == 0 )
		return;

	float iw		= Viewport::Instance( ).Width( ) * 0.5f;
	float ih		= Viewport::Instance( ).Height( ) * 0.5f;

	float kw		= Viewport::Instance( ).InvWidth( ) * Renderer::Instance( ).D3DPP( ).BackBufferWidth;
	float kh		= Viewport::Instance( ).InvHeight( ) * Renderer::Instance( ).D3DPP( ).BackBufferHeight;

	float height	= 2.0f * Height( );
	float arrow1w	= height * mShaders[ EST_FIRST_ARROW ].get< 1 >( ).x / mShaders[ EST_FIRST_ARROW ].get< 1 >( ).y;
	float arrow1x	= X( ) - Width( ) + 0.5f * arrow1w;
	float arrow2w	= height * mShaders[ EST_SECOND_ARROW ].get< 1 >( ).x / mShaders[ EST_SECOND_ARROW ].get< 1 >( ).y;
	float arrow2x	= X( ) + Width( ) - 0.5f * arrow2w;
	float fillw		= 2.0f * Width( ) - arrow1w - arrow2w;
	float fillx		= 0.5f * ( arrow1x + arrow2x );
	float fillrep	= fillw / ( height * mShaders[ EST_FILL ].get< 1 >( ).x ) * mShaders[ EST_FILL ].get< 1 >( ).y;
	float scrollw	= height * mShaders[ EST_SCROLL ].get< 1 >( ).x / mShaders[ EST_SCROLL ].get< 1 >( ).y;
	float scrollx	= arrow1x + 0.5f * arrow1w + scrollw * 0.5f + ( fillw - scrollw ) * mScrollPos;

	boost::shared_ptr< ShaderConsts > empty_consts;

	// render fill sprite
	Renderer::Instance( ).DrawTools( ).PushDraw2DSprite(
		( fillx * scale.x + iw + ofs.x ) * kw, ( ih - Y( ) * scale.y - ofs.y ) * kh,
		fillw * scale.x * kw, Height( ) * scale.y * 2.0f * kh,
		0, newcolor, mShaders[ EST_FILL ].get< 0 >( ), 
		empty_consts, 0.0f, 0.0f, fillrep, 1.0f );

	// render left arrow sprite
	Renderer::Instance( ).DrawTools( ).PushDraw2DSprite(
		( arrow1x * scale.x + iw + ofs.x ) * kw, ( ih - Y( ) * scale.y - ofs.y ) * kh,
		arrow1w * scale.x * kw, Height( ) * scale.y * 2.0f * kh,
		0, newcolor, mShaders[ EST_FIRST_ARROW ].get< 0 >( ), 
		empty_consts, 0.0f, 0.0f, 1.0f, 1.0f );

	// render right arrow sprite
	Renderer::Instance( ).DrawTools( ).PushDraw2DSprite(
		( arrow2x * scale.x + iw + ofs.x ) * kw, ( ih - Y( ) * scale.y - ofs.y ) * kh,
		arrow2w * scale.x * kw, Height( ) * scale.y * 2.0f * kh,
		0, newcolor, mShaders[ EST_SECOND_ARROW ].get< 0 >( ), 
		empty_consts, 0.0f, 0.0f, 1.0f, 1.0f );

	// render scroll position sprite
	Renderer::Instance( ).DrawTools( ).PushDraw2DSprite(
		( scrollx * scale.x + iw + ofs.x ) * kw, ( ih - Y( ) * scale.y - ofs.y ) * kh,
		scrollw * scale.x * kw, Height( ) * scale.y * 2.0f * kh,
		0, newcolor, mShaders[ EST_SCROLL ].get< 0 >( ), 
		empty_consts, 0.0f, 0.0f, 1.0f, 1.0f );
}



_XFX_END
#include "pch.h"
#include "application.h"






//
// gGetApplication implementation
//

xfx::Application& xfx::gGetApplication( )
{
	_ASSERTE( !"This method MUST not be called in this application" );

	return *reinterpret_cast< xfx::Application * >( NULL );
}




//
// Screensaver
//

Screensaver::Screensaver( ) :
	mBlackMaskShader( ),
	mBackgroundMaskShader( ),
	mBackgroundShader( ),
	mSheepShader( ),
	mStateTime( 0 ),
	mLocalTime( 0 ),
	mCurrentState( ELG_EMPTY_SCREEN ),
	mLogoCenterX( 0.0f ),
	mLogoCenterY( 0.0f ),
	mLogoWidth( 0.0f ),
	mLogoHeight( 0.0f )
{
}

HRESULT Screensaver::Init( )
{
	// register shaders
	mBlackMaskShader		= xfx::Shader::Cache( ).Register( "black_mask.shader" );
	mBackgroundMaskShader	= xfx::Shader::Cache( ).Register( "dfg_background_mask.shader" );
	mBackgroundShader		= xfx::Shader::Cache( ).Register( "dfg_background.shader" );
	mSheepShader			= xfx::Shader::Cache( ).Register( "dfg_sheep.shader" );

	// set sheep animation time to 0
	mSheepShaderParams.reset( new xfx::ShaderParams( ) );
	mSheepShaderParams->SetValue( "xfx_DiffuseTex_time", 0.0f );

	// fill in information about each state.
	mLogoStateInfos[ ELG_EMPTY_SCREEN ]		= LogoStateInfo( 3000, boost::bind( &Screensaver::RenderEmptyScreen, this, _1 ) );
	mLogoStateInfos[ ELG_SHOW_FARM ]		= LogoStateInfo( 30000, boost::bind( &Screensaver::RenderShowFarm, this, _1 ) );
	mLogoStateInfos[ ELG_FARM_ANIMATION ]	= LogoStateInfo( std::numeric_limits< DWORD >::max( ), boost::bind( &Screensaver::RenderFarmAnimation, this, _1 ) );

	//
	// Initialize sheep paths
	//

#define PSHOP2VEC( x, y ) xfx::Vec2( x / 256 - 1.0f, y / 256 - 1.0f )

	mSheepPathKeys.AddKey( 256 * 0, PSHOP2VEC( 500.0f, 446.0f ) );
	mSheepPathKeys.AddKey( 256 * 30, PSHOP2VEC( 440.0f, 348.0f ) );
	mSheepPathKeys.AddKey( 256 * 60, PSHOP2VEC( 366.0f, 275.0f ) );
	mSheepPathKeys.AddKey( 256 * 88, PSHOP2VEC( 294.0f, 236.0f ) );
	//mSheepPathKeys.AddKey( 256 * 93, PSHOP2VEC( 279.0f, 150.0f ) );
	//mSheepPathKeys.AddKey( 256 * 94, PSHOP2VEC( 268.0f, 110.0f ) );
	//mSheepPathKeys.AddKey( 256 * 95, PSHOP2VEC( 238.0f, 90.0f ) );
	//mSheepPathKeys.AddKey( 256 * 96, PSHOP2VEC( 204.0f, 104.0f ) );
	//mSheepPathKeys.AddKey( 256 * 97, PSHOP2VEC( 179.0f, 143.0f ) );
	mSheepPathKeys.AddKey( 256 * 95, PSHOP2VEC( 220.0f, 143.0f ) );
	mSheepPathKeys.AddKey( 256 * 100, PSHOP2VEC( 177.0f, 219.0f ) );
	mSheepPathKeys.AddKey( 256 * 150, PSHOP2VEC( 106.0f, 231.0f ) );
	mSheepPathKeys.AddKey( 256 * 200, PSHOP2VEC( 66.0f, 253.0f ) );
	mSheepPathKeys.AddKey( 256 * 255, PSHOP2VEC( 37.0f, 270.0f ) );

	mSheepScaleKeys.AddKey( 0, xfx::Vec2( 0.17f, 0.17f ) );
	mSheepScaleKeys.AddKey( 90, xfx::Vec2( 0.13f, 0.13f ) );
	mSheepScaleKeys.AddKey( 95, xfx::Vec2( 0.11f, 0.11f ) );
	mSheepScaleKeys.AddKey( 100, xfx::Vec2( 0.08f, 0.08f ) );
	mSheepScaleKeys.AddKey( 138, xfx::Vec2( 0.06f, 0.06f ) );
	mSheepScaleKeys.AddKey( 181, xfx::Vec2( 0.04f, 0.04f ) );
	mSheepScaleKeys.AddKey( 255, xfx::Vec2( 0.0f, 0.0f ) );

	mSheepRotationKeys.AddKey( 0, xfx::Math::pi / 4 );
	mSheepRotationKeys.AddKey( 88, 0 );
	mSheepRotationKeys.AddKey( 90, xfx::Math::pi / 8 );
	mSheepRotationKeys.AddKey( 93, 0 );
	mSheepRotationKeys.AddKey( 98, -xfx::Math::pi / 8 );
	mSheepRotationKeys.AddKey( 100, -xfx::Math::pi / 12 );
	mSheepRotationKeys.AddKey( 255, -xfx::Math::pi / 4 );

#undef PSHOP2VEC

	// make ELG_EMPTY_SCREEN state as current state
	mLocalTime = 0;
	mStateTime = 0;
	mCurrentState = ELG_EMPTY_SCREEN;

	return S_OK;
}

void Screensaver::Update( )
{
	// Update timer.
	mTimer.Update( );

	//gMess( "Update: %d .%3f %.3f %.3f %.3f", mCurrentState, mLogoCenterX, mLogoCenterY, mLogoWidth, mLogoHeight );

	if( mCurrentState == ELG_MAX_STATES )
		return;

	DWORD mspf = mTimer.MSPF100( );

	float iw		= xfx::Viewport::Instance( ).Width( ) * 0.5f;
	float ih		= xfx::Viewport::Instance( ).Height( ) * 0.5f;

	float kw		= xfx::Viewport::Instance( ).InvWidth( ) * xfx::Renderer::Instance( ).D3DPP( ).BackBufferWidth;
	float kh		= xfx::Viewport::Instance( ).InvHeight( ) * xfx::Renderer::Instance( ).D3DPP( ).BackBufferHeight;

	mLogoCenterX	= iw * kw;
	mLogoCenterY	= ih * kh;
	mLogoWidth		= iw * 0.9f;
	mLogoHeight		= mLogoWidth;

	mLogoWidth *= kw;
	mLogoHeight *= kh;

	mLocalTime += mspf;
	mStateTime += mspf;

	if( mStateTime > mLogoStateInfos[ mCurrentState ].duration )
	{
		mStateTime -= mLogoStateInfos[ mCurrentState ].duration;
		mCurrentState = static_cast< ELogoState >( mCurrentState + 1 );

		if( mCurrentState > ELG_FARM_ANIMATION )
		{
			mCurrentState = ELG_FARM_ANIMATION;
		}
	}
}

void Screensaver::Render( ) const
{
	// prepare scene to rendering
	xfx::Renderer::Instance( ).BeginFrame( );

	// select state render function for current state
	_ASSERTE( mCurrentState < ELG_MAX_STATES );
	if( mCurrentState < ELG_MAX_STATES )
		mLogoStateInfos[ mCurrentState ].render_fn( static_cast< float>( mStateTime ) / mLogoStateInfos[ mCurrentState ].duration );

	xfx::Renderer::Instance( ).EndFrame( );
}

void Screensaver::RenderEmptyScreen( float state_normal_time ) const
{
}

void Screensaver::RenderShowFarm( float state_normal_time ) const
{
	float cur_width	= mLogoWidth * xfx::math_sin( static_cast< xfx::Math::Angle >( 0.5f * xfx::Math::pi * state_normal_time ) );
	float cur_height = mLogoHeight * xfx::math_sin( static_cast< xfx::Math::Angle >( 0.5f * xfx::Math::pi * state_normal_time ) );

	xfx::ARGB color( 0xffffffff );

	xfx::Renderer::Instance( ).DrawTools( ).PushDraw2DSprite( mLogoCenterX, mLogoCenterY, mLogoWidth, mLogoHeight, 0, color, mBackgroundShader );

	RenderSheeps( );

	RenderMask( cur_width, cur_height );
}

void Screensaver::RenderFarmAnimation( float state_normal_time ) const
{
	xfx::ARGB color( 0xffffffff );

	xfx::Renderer::Instance( ).DrawTools( ).PushDraw2DSprite( mLogoCenterX, mLogoCenterY, mLogoWidth, mLogoHeight, 0, color, mBackgroundShader );

	RenderSheeps( );

	RenderMask( mLogoWidth, mLogoHeight );
}

void Screensaver::RenderMask( float cur_width, float cur_height ) const
{
	xfx::ARGB color( 0xffffffff );
	xfx::Renderer::Instance( ).DrawTools( ).PushDraw2DSprite( mLogoCenterX, mLogoCenterY, cur_width, cur_height, 0, color, mBackgroundMaskShader );

	float mask_width = mLogoWidth * 1.25f;
	float mask_height = mLogoHeight * 1.25f;

	xfx::Renderer::Instance( ).DrawTools( ).PushDraw2DSprite( mLogoCenterX, mLogoCenterY + mask_height * 0.5f - ( mask_height - cur_height ) * 0.25f, mask_width, ( mask_height - cur_height ) * 0.5f, 0, color, mBlackMaskShader );
	xfx::Renderer::Instance( ).DrawTools( ).PushDraw2DSprite( mLogoCenterX, mLogoCenterY - mask_height * 0.5f + ( mask_height - cur_height ) * 0.25f, mask_width, ( mask_height - cur_height ) * 0.5f, 0, color, mBlackMaskShader );
	xfx::Renderer::Instance( ).DrawTools( ).PushDraw2DSprite( mLogoCenterX - mask_width * 0.5f + ( mask_width - cur_width ) * 0.25f, mLogoCenterY, ( mask_width - cur_width ) * 0.5f, cur_height, 0, color, mBlackMaskShader );
	xfx::Renderer::Instance( ).DrawTools( ).PushDraw2DSprite( mLogoCenterX + mask_width * 0.5f - ( mask_width - cur_width ) * 0.25f, mLogoCenterY, ( mask_width - cur_width ) * 0.5f, cur_height, 0, color, mBlackMaskShader );
}

void Screensaver::RenderSheeps( ) const
{
	xfx::ARGB color( 0xffffffff );

	const DWORD total_time = 150000;
	const int sheeps_count = 9;
	const DWORD sheep_delta = total_time / sheeps_count;

	for( int i = 0; i < sheeps_count; i++ )
	{
		DWORD sheep_time = ( mLocalTime % total_time ) + sheep_delta * i;
		float rel_time = static_cast< float >( sheep_time ) / total_time;

		WORD pos_key = static_cast< WORD >( rel_time * 65535.0f );
		BYTE scl_key = static_cast< BYTE >( rel_time * 255.0f );

		xfx::Vec2 pos = mSheepPathKeys.Key( pos_key );
		xfx::Math::BigAngle rot = mSheepRotationKeys.Key( scl_key );
		xfx::Vec2 scale = mSheepScaleKeys.Key( scl_key );

		pos.x = pos.x * mLogoWidth * 0.5f + mLogoCenterX;
		pos.y = pos.y * mLogoHeight * 0.5f + mLogoCenterY;

		scale.x *= mLogoWidth;
		scale.y *= mLogoHeight;

		mSheepShaderParams->SetValue( "xfx_DiffuseTex_time", static_cast< float >( sheep_time * 0.0001f ) );

		xfx::Renderer::Instance( ).DrawTools( ).PushDraw2DSprite( pos.x, pos.y, scale.x, scale.y, rot, color, mSheepShader, mSheepShaderParams );
	}
}
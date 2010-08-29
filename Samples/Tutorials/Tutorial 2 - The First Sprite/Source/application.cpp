#include "pch.h"
#include "application.h"






//
// gGetApplication implementation
//

xfx::Application& xfx::gGetApplication( )
{
	return ::Application::Instance( );
}




//
// Application
//

HRESULT Application::InitEngine( const xfx::String &cmdline, const xfx::WString &wndclass, const xfx::WString &caption )
{
	HRESULT res = xfx::Application::InitEngine( cmdline, wndclass, caption );

	if( FAILED( res ) )
		return res;

	// Initialize renderer.
	res = xfx::Renderer::Instance( ).CreateDevice( HWnd( ) );

	if( FAILED( res ) )
		return res;

	// add search path to media content to file system
	// after doing this, we can refer to media content without using full path
	// note: the path should be ended with '\' 
	xfx::FileSystem::Instance( ).AddSearchPath( "data\\" );

	// try to register shader for sprite
	// we use Shader::Cache( ) to cache shaders with equal filenames
	// other way to load shader is simply call xfx::Shader::LoadFile
	// but in this way shader will be loaded always from disk, even if
	// the copy of one has already been loaded
	mSpriteShader = xfx::Shader::Cache( ).Register( "dfg_sheep.shader" );

	mShaderParamsConsts.reset( new xfx::ShaderParams( ) );

	return res;
}

void Application::Shutdown( )
{
	// shutdown renderer
	xfx::Renderer::Instance( ).Shutdown( );

	// call base method
	xfx::Application::Shutdown( );
}

void Application::Update( )
{
	// that means we are in shutdown process, no update-render then
	if( !HWnd( ) )
		return;

	// Update timer.
	mTimer.Update( );

	// convert time to seconds 
	float seconds = 0.0001f * mTimer.MicroSeconds100( );

	// Set time for DiffuseTex texture map in shader.
	mShaderParamsConsts->SetValue( "xfx_DiffuseTex_time", seconds );

	// prepare scene to rendering
	// this function do all preparation for begin and end scene rendering and
	// then call xfx::Application::Render
	xfx::Viewport::Instance( ).Render( );
}

void Application::Render( ) const
{
	// render sprite
	xfx::Renderer::Instance( ).GetDrawTools( ).PushDraw2DSprite(
		256, 256, 128, 128, 0, 0xffffffff, mSpriteShader, mShaderParamsConsts
		);
}
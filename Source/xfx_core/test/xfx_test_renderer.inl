struct AppFixture
{
	AppFixture( )
	{
		::Application::Instance( ).InitEngine( static_cast< HINSTANCE >( GetCurrentProcess( ) ), "", L"TestRendererWndClass", L"TestRendererWndCaption" );
	}

	~AppFixture( )
	{
		::Application::Instance( ).Shutdown( );
	}
};

BOOST_FIXTURE_TEST_SUITE( xfx_renderer_test_suite, AppFixture )

BOOST_AUTO_TEST_CASE( renderer )
{
	BOOST_CHECK_EQUAL( Renderer::Instance( ).CreateDevice( gGetApplication( ).hWnd( ) ), S_OK );
	BOOST_CHECK_NE( Renderer::Instance( ).CreateDevice( gGetApplication( ).hWnd( ) ), S_OK );

	BOOST_CHECK_NE( Renderer::Instance( ).pD3D( ), ( void * )NULL );
	BOOST_CHECK_NE( Renderer::Instance( ).pD3DDevice( ), ( void * )NULL );

	Renderer::Instance( ).BeginFrame( );
	Renderer::Instance( ).Clear( 0 );
	Renderer::Instance( ).EndFrame( );

	BOOST_CHECK_EQUAL( Renderer::Instance( ).ResetDevice( 10, 10 ), S_OK );

	// hard to test all functionality of renderer
	// the tests will be added when new bugs will be found.

	Renderer::Instance( ).Shutdown( );
}

BOOST_AUTO_TEST_SUITE_END( )

struct AppFixture
{
	AppFixture( )
	{
		::Application::Instance( ).InitEngine( "", L"TestRendererWndClass", L"TestRendererWndCaption" );
	}

	~AppFixture( )
	{
		::Application::Instance( ).Shutdown( );
	}
};

struct RendererFixture
{
	RendererFixture( )
	{
		::Application::Instance( ).InitEngine( "", L"TestRendererWndClass2", L"TestRendererWndCaption2" );
		Renderer::Instance( ).CreateDevice( gGetApplication( ).HWnd( ) );
	}

	~RendererFixture( )
	{
		Renderer::Instance( ).Shutdown( );
		::Application::Instance( ).Shutdown( );
	}
};

BOOST_FIXTURE_TEST_SUITE( xfx_renderer_test_suite, AppFixture )

BOOST_AUTO_TEST_CASE( renderer )
{
	BOOST_CHECK_EQUAL( Renderer::Instance( ).CreateDevice( gGetApplication( ).HWnd( ) ), S_OK );
	BOOST_CHECK_NE( Renderer::Instance( ).CreateDevice( gGetApplication( ).HWnd( ) ), S_OK );

	BOOST_CHECK_NE( Renderer::Instance( ).pD3D( ), ( void * )NULL );
	BOOST_CHECK_NE( Renderer::Instance( ).pD3DDevice( ), ( void * )NULL );

	Renderer::Instance( ).BeginFrame( );
	Renderer::Instance( ).Clear( D3DCLEAR_TARGET );
	Renderer::Instance( ).EndFrame( );

	BOOST_CHECK_EQUAL( Renderer::Instance( ).ResetDevice( 10, 10 ), S_OK );

	// hard to test all functionality of renderer
	// the tests will be added when new bugs will be found.

	Renderer::Instance( ).Shutdown( );
}

BOOST_AUTO_TEST_SUITE_END( )

BOOST_FIXTURE_TEST_SUITE( xfx_renderer_test_suite_renderer_fixture, RendererFixture )

BOOST_AUTO_TEST_CASE( indexbuffer )
{
	IndexBuffer ib;
	BOOST_CHECK_EQUAL( ib.Create( true, 10000, true ), S_OK );
	BOOST_CHECK_EQUAL( ib.Create( true, 10000, false ), S_OK );
	BOOST_CHECK_EQUAL( ib.Create( false, 10000, true ), S_OK );
	BOOST_CHECK_EQUAL( ib.Create( false, 10000, false ), S_OK );

	ib.SetIndices( );

	ib.Destroy( );
}

BOOST_AUTO_TEST_CASE( vertexbuffer )
{
	VertexBuffer vb;
	BOOST_CHECK_EQUAL( vb.Create( 100, 10000, D3DFVF_XYZ, true, D3DUSAGE_WRITEONLY ), S_OK );
	BOOST_CHECK_EQUAL( vb.Create( 100, 10000, D3DFVF_XYZ, false, 0 ), S_OK );
	BOOST_CHECK_EQUAL( vb.Create( 100, 10000, D3DFVF_XYZ, true, D3DUSAGE_DYNAMIC ), S_OK );
	BOOST_CHECK_EQUAL( vb.Create( 100, 10000, D3DFVF_XYZ, false, D3DUSAGE_WRITEONLY ), S_OK );
	BOOST_CHECK_EQUAL( vb.Create( 100, 10000, D3DFVF_XYZ, true, 0 ), S_OK );

	vb.SetStream( 0 );
	vb.SetStream( 1 );
	vb.SetStream( 2 );
	vb.SetStream( 3 );

	vb.Destroy( );
}

BOOST_AUTO_TEST_CASE( viewer )
{
	Viewer v;
	BOOST_CHECK_LT( ( v.Position( ) - Vec3( 0.0f, 0.0f, -1.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK( v.Rotation( ) == Euler( 0 ) );
	BOOST_CHECK_EQUAL( v.FOV( ), Math::real_pi * 0.5f );
	BOOST_CHECK_EQUAL( v.NearPlane( ), 0.1f );
	BOOST_CHECK_EQUAL( v.FarPlane( ), 100.0f );
	BOOST_CHECK_EQUAL( v.IsLH( ), true );

	v.SetPosition( Vec3( -1.0f, 1.0f, 0.0f ) );
	v.SetRotation( Euler( Math::pi / 2, Math::pi / 4, 0 ) );

	// Viewer uses aspect ration from current viewport, so we have to setup
	// new viewport in order to use square frustum.
	Viewport::Instance( ).Init( 512, 512 );

	v.SetVPMatrices( false );

	Mat4 view;
	view.LookAtLH( v.Position( ), Vec3( 1.0f, -1.0f, 0.0f ), Vec3( 1.0f, 1.0f, 0.0f ) );
	BOOST_CHECK( ( view.GetInversedAffine( ) * v.GetView( ) ).IsIdentity( ) );

	BOOST_CHECK( v.TestFrustumCulling( Primitives::Sphere( Vec3( 0.0f ), 1.0f ) ) );
	BOOST_CHECK( v.TestFrustumCulling( Primitives::Sphere( Vec3( 0.0f, 1.99999f, 0.0f ), 1.0f ) ) );
	BOOST_CHECK( !v.TestFrustumCulling( Primitives::Sphere( Vec3( 0.0f, 2.00001f, 0.0f ), 1.0f ) ) );

	BOOST_CHECK( v.TestFrustumCulling( Primitives::AABB( Vec3( -0.5f ), Vec3( 0.5f ) ) ) );
	BOOST_CHECK( v.TestFrustumCulling( Primitives::AABB( Vec3( -1.0f, 0.999999f, -1.0f ), Vec3( 0.0f, 4.0f, 0.0f ) ) ) );
	BOOST_CHECK( !v.TestFrustumCulling( Primitives::AABB( Vec3( -1.0f, 1.000001f, -1.0f ), Vec3( 0.0f, 4.0f, 0.0f ) ) ) );

	BOOST_CHECK( v.TestFrustumCulling( Primitives::OBB( Vec3( 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ), Vec3( 0.0f, 0.0f, 1.0f ) ) ) );
	BOOST_CHECK( v.TestFrustumCulling( Primitives::OBB( Vec3( 0.0f, 0.999999f, 0.0f ), Vec3( -1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ), Vec3( 0.0f, 0.0f, -1.0f ) ) ) );
	BOOST_CHECK( !v.TestFrustumCulling( Primitives::OBB( Vec3( 0.0f, 1.000001f, 0.0f ), Vec3( -1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ), Vec3( 0.0f, 0.0f, -1.0f ) ) ) );
}

BOOST_AUTO_TEST_CASE( texture )
{
	Texture tex;
	BOOST_CHECK( tex.IsEmpty( ) );

	BOOST_CHECK_EQUAL( tex.Create( 256, 256, 3 ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetWidth( ), ( unsigned )256 );
	BOOST_CHECK_EQUAL( tex.GetHeight( ), ( unsigned )256 );
	BOOST_CHECK_EQUAL( tex.NumMips( ), ( unsigned )3 );
	BOOST_CHECK( tex.IsIdentityResultTransform( ) );
	BOOST_CHECK_EQUAL( tex.Create( 33, 44, 0 ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetWidth( ), ( unsigned )33 );
	BOOST_CHECK_EQUAL( tex.GetHeight( ), ( unsigned )44 );
	BOOST_CHECK_EQUAL( tex.NumMips( ), ( unsigned )6 );
	BOOST_CHECK( !tex.IsEmpty( ) );
	BOOST_CHECK( !tex.IsIdentityResultTransform( ) );
	tex.Free( );
	BOOST_CHECK( tex.IsEmpty( ) );
	BOOST_CHECK_EQUAL( tex.Create( Renderer::Instance( ).D3DCaps( ).MaxTextureWidth * 2, Renderer::Instance( ).D3DCaps( ).MaxTextureHeight * 2, 0 ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetWidth( ), Renderer::Instance( ).D3DCaps( ).MaxTextureWidth );
	BOOST_CHECK_EQUAL( tex.GetHeight( ), Renderer::Instance( ).D3DCaps( ).MaxTextureHeight );

	BOOST_CHECK_EQUAL( tex.LoadFile( "FileData\\texture1" ), S_OK );
	BOOST_CHECK_EQUAL( tex.LoadFile( "FileData\\texture1.png*fmt.a8r8g8b8" ), S_OK );

	Texture tex2( tex );
	ARGB * tex2_data = reinterpret_cast< ARGB * >( alloca( sizeof( ARGB ) * tex2.GetWidth( ) * tex2.GetHeight( ) ) );
	BOOST_CHECK_EQUAL( tex2.GetWidth( ), ( unsigned )100 );
	BOOST_CHECK_EQUAL( tex2.GetHeight( ), ( unsigned )50 );
	BOOST_CHECK_EQUAL( tex2.GetSurfaceData( 0, tex2_data ), S_OK );
	BOOST_CHECK( tex2_data[ 0 ] == ARGB( 255, 0, 0, 255 ) );
	BOOST_CHECK( tex2_data[ tex2.GetWidth( ) - 1 ] == ARGB( 255, 252, 0, 255 ) );
	BOOST_CHECK( tex2_data[ tex2.GetWidth( ) * ( tex2.GetHeight( ) - 1 ) ] == ARGB( 255, 0, 250, 5 ) );
	BOOST_CHECK( tex2_data[ tex2.GetWidth( ) * ( tex2.GetHeight( ) - 1 ) + tex2.GetWidth( ) - 1 ] == ARGB( 255, 252, 250, 5 ) );

	BOOST_CHECK_EQUAL( tex.CopyAlphaFromRGB( tex2 ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetSurfaceData( 0, tex2_data ), S_OK );
	BOOST_CHECK( tex2_data[ 0 ] == ARGB( 85, 0, 0, 255 ) );
	BOOST_CHECK( tex2_data[ tex2.GetWidth( ) - 1 ] == ARGB( 169, 252, 0, 255 ) );
	BOOST_CHECK( tex2_data[ tex2.GetWidth( ) * ( tex2.GetHeight( ) - 1 ) ] == ARGB( 85, 0, 250, 5 ) );
	BOOST_CHECK( tex2_data[ tex2.GetWidth( ) * ( tex2.GetHeight( ) - 1 ) + tex2.GetWidth( ) - 1 ] == ARGB( 169, 252, 250, 5 ) );

	tex2_data[ 100 ] = ARGB( 0, 0, 0, 0 );
	BOOST_CHECK_EQUAL( tex.SetSurfaceData( 0, tex2_data ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetSurfaceData( 0, tex2_data ), S_OK );
	BOOST_CHECK( tex2_data[ 100 ] == ARGB( 0, 0, 0, 0 ) );
}

BOOST_AUTO_TEST_CASE( cubemap )
{
	CubemapTexture tex;
	BOOST_CHECK( tex.IsEmpty( ) );

	BOOST_CHECK_EQUAL( tex.Create( 256, 3 ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetWidth( ), ( unsigned )256 );
	BOOST_CHECK_EQUAL( tex.GetHeight( ), ( unsigned )256 );
	BOOST_CHECK_EQUAL( tex.NumMips( ), ( unsigned )3 );
	BOOST_CHECK( tex.IsIdentityResultTransform( ) );
	BOOST_CHECK_EQUAL( tex.Create( 33, 0 ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetWidth( ), ( unsigned )33 );
	BOOST_CHECK_EQUAL( tex.GetHeight( ), ( unsigned )33 );
	BOOST_CHECK_EQUAL( tex.NumMips( ), ( unsigned )6 );
	BOOST_CHECK( !tex.IsEmpty( ) );
	BOOST_CHECK( !tex.IsIdentityResultTransform( ) );
	tex.Free( );
	BOOST_CHECK( tex.IsEmpty( ) );
	BOOST_CHECK_EQUAL( tex.Create( Renderer::Instance( ).D3DCaps( ).MaxTextureWidth * 2, 0 ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetWidth( ), Renderer::Instance( ).D3DCaps( ).MaxTextureWidth );
	BOOST_CHECK_EQUAL( tex.GetHeight( ), Renderer::Instance( ).D3DCaps( ).MaxTextureWidth );

	/*
	BOOST_CHECK_EQUAL( tex.LoadFile( "FileData\\texture1" ), S_OK );
	BOOST_CHECK_EQUAL( tex.LoadFile( "FileData\\texture1.png*fmt.a8r8g8b8" ), S_OK );

	CubemapTexture tex2( tex );
	ARGB * tex2_data = reinterpret_cast< ARGB * >( alloca( sizeof( ARGB ) * tex2.Width( ) * tex2.Height( ) ) );
	BOOST_CHECK_EQUAL( tex2.Width( ), 100 );
	BOOST_CHECK_EQUAL( tex2.Height( ), 50 );
	BOOST_CHECK_EQUAL( tex2.GetSurfaceData( 0, tex2_data ), S_OK );
	BOOST_CHECK( tex2_data[ 0 ] == ARGB( 255, 0, 0, 255 ) );
	BOOST_CHECK( tex2_data[ tex2.Width( ) - 1 ] == ARGB( 255, 252, 0, 255 ) );
	BOOST_CHECK( tex2_data[ tex2.Width( ) * ( tex2.Height( ) - 1 ) ] == ARGB( 255, 0, 250, 5 ) );
	BOOST_CHECK( tex2_data[ tex2.Width( ) * ( tex2.Height( ) - 1 ) + tex2.Width( ) - 1 ] == ARGB( 255, 252, 250, 5 ) );

	BOOST_CHECK_EQUAL( tex.CopyAlphaFromRGB( tex2 ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetSurfaceData( 0, tex2_data ), S_OK );
	BOOST_CHECK( tex2_data[ 0 ] == ARGB( 85, 0, 0, 255 ) );
	BOOST_CHECK( tex2_data[ tex2.Width( ) - 1 ] == ARGB( 169, 252, 0, 255 ) );
	BOOST_CHECK( tex2_data[ tex2.Width( ) * ( tex2.Height( ) - 1 ) ] == ARGB( 85, 0, 250, 5 ) );
	BOOST_CHECK( tex2_data[ tex2.Width( ) * ( tex2.Height( ) - 1 ) + tex2.Width( ) - 1 ] == ARGB( 169, 252, 250, 5 ) );

	tex2_data[ 100 ] = ARGB( 0, 0, 0, 0 );
	BOOST_CHECK_EQUAL( tex.SetSurfaceData( 0, tex2_data ), S_OK );
	BOOST_CHECK_EQUAL( tex.GetSurfaceData( 0, tex2_data ), S_OK );
	BOOST_CHECK( tex2_data[ 100 ] == ARGB( 0, 0, 0, 0 ) );
	*/
}

BOOST_AUTO_TEST_CASE( drawtools )
{
}

BOOST_AUTO_TEST_SUITE_END( )

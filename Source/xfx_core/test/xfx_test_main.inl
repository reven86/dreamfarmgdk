BOOST_AUTO_TEST_SUITE( xfx_math_test_suite )

void app_quit( )
{
	Sleep( 1000 );
	SendMessage( gGetApplication( ).hWnd( ), WM_DESTROY, 0, 0 );
}

BOOST_AUTO_TEST_CASE( application )
{
	BOOST_CHECK_EQUAL( ::Application::Instance( ).InitEngine( static_cast< HINSTANCE >( GetCurrentProcess( ) ), "-?", L"TestWndClass", L"TestWndCaption" ), S_OK );
	BOOST_CHECK_NE( ::Application::Instance( ).InitEngine( static_cast< HINSTANCE >( GetCurrentProcess( ) ), "-?", L"TestWndClass2", L"TestWndCaption2" ), S_OK );
	::Application::Instance( ).Update( );
	::Application::Instance( ).Render( );
	boost::thread thr( app_quit );
	gGetApplication( ).MainLoop( );
	thr.join( );
	BOOST_CHECK_EQUAL( ::Application::Instance( ).CommandLine( ), "-?" );
	::Application::Instance( ).Shutdown( );
	::Application::Instance( ).Shutdown( );
}

BOOST_AUTO_TEST_CASE( cmd )
{
	Var * test_var_def = Cmd::Instance( ).RegisterVar( "test_var_def", "def", Cmd::EVF_DEFAULT );
	Var * test_var_dev = Cmd::Instance( ).RegisterVar( "test_var_dev", "def", Cmd::EVF_DEVELOPER );
	Var * test_var_cheat = Cmd::Instance( ).RegisterVar( "test_var_cheat", "def", Cmd::EVF_CHEAT );
	Var * test_var_autoreset = Cmd::Instance( ).RegisterVar( "test_var_autoreset", "def", Cmd::EVF_AUTORESET );
	//Var * test_var_server = Cmd::Instance( ).RegisterVar( "test_var_server", "def", Cmd::EVF_SERVER_VAR );
	Var * test_var_const = Cmd::Instance( ).RegisterVar( "test_var_const", "def", Cmd::EVF_CONST );

	test_var_def->Change( "10" );
	BOOST_CHECK_EQUAL( test_var_def->AsFloat( ), 10.0f );
	BOOST_CHECK_EQUAL( test_var_def->AsInt( ), 10 );
	BOOST_CHECK_EQUAL( test_var_def->Value( ), "10" );
	BOOST_CHECK_EQUAL( test_var_def->Default( ), "def" );

	BOOST_CHECK_EQUAL( g_developer->AsInt( ), 0 );
	BOOST_CHECK_EQUAL( g_cheats->AsInt( ), 0 );
	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetExecutionContext( ), Cmd::EEC_CODE );
	Cmd::Instance( ).Execute( "test_var_dev 1;test_var_cheat \"1\"; test_var_autoreset \"\\\";res\\\"\"; test_var_const 22" );
	BOOST_CHECK_NE( test_var_dev->Value( ), "1" );
	BOOST_CHECK_NE( test_var_cheat->Value( ), "1" );
	BOOST_CHECK_EQUAL( test_var_autoreset->Value( ), "\";res\"" );
	BOOST_CHECK_EQUAL( test_var_const->AsInt( ), 22 );
	Cmd::Instance( ).SetExecutionContext( Cmd::EEC_USER );
	g_developer->Change( "1" );
	g_cheats->Change( "1" );
	Cmd::Instance( ).Execute( "test_var_dev 1;test_var_cheat \"1\"; test_var_autoreset \"\\\";res\\\"\"; test_var_const 33" );
	BOOST_CHECK_EQUAL( test_var_dev->Value( ), "1" );
	BOOST_CHECK_EQUAL( test_var_cheat->Value( ), "1" );
	BOOST_CHECK_EQUAL( test_var_const->Value( ), "22" );
	Cmd::Instance( ).ResetVariables( );
	BOOST_CHECK_EQUAL( test_var_dev->Value( ), "1" );
	BOOST_CHECK_EQUAL( test_var_cheat->Value( ), "1" );
	BOOST_CHECK_EQUAL( test_var_autoreset->Value( ), "def" );

	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetVarFlags( "test_var_def" ), ( DWORD )Cmd::EVF_DEFAULT );
	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetVarFlags( "test_var_dev" ), ( DWORD )Cmd::EVF_DEVELOPER );
	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetVarFlags( "test_var_cheat" ), ( DWORD )Cmd::EVF_CHEAT );
	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetVarFlags( "test_var_autoreset" ), ( DWORD )Cmd::EVF_AUTORESET );
	//BOOST_CHECK_EQUAL( Cmd::Instance( ).GetVarFlags( "test_var_server" ), ( DWORD )Cmd::EVF_SERVER );
	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetVarFlags( "test_var_const" ), ( DWORD )Cmd::EVF_CONST );

	static int test = 0;

	struct test_fn : public std::binary_function< String, int, void >
	{
		int * t;
		test_fn( int * _t ) : t( _t ) { };

		void operator( ) ( const String& str, int mult ) const
		{
			String::size_type s = 0;
			String param = next_token( str, s );

			*t += atoi( param.c_str( ) ) * mult;
		};
	};

	Cmd::Instance( ).RegisterCmd( "test_cmd_def", boost::bind( test_fn( &test ), _1, 1 ), Cmd::ECF_DEFAULT );
	Cmd::Instance( ).RegisterCmd( "test_cmd_dev", boost::bind( test_fn( &test ), _1, 2 ), Cmd::ECF_DEVELOPER );
	Cmd::Instance( ).RegisterCmd( "test_cmd_cheat", boost::bind( test_fn( &test ), _1, 3 ), Cmd::ECF_CHEAT );
	Cmd::Instance( ).RegisterCmd( "test_cmd_server", boost::bind( test_fn( &test ), _1, 4 ), Cmd::ECF_SERVER_CMD );

	Cmd::Instance( ).ResetVariables( true );
	BOOST_CHECK_EQUAL( test, 0 );
	BOOST_CHECK_EQUAL( g_developer->AsInt( ), 0 );
	BOOST_CHECK_EQUAL( g_cheats->AsInt( ), 0 );
	Cmd::Instance( ).Execute( "test_cmd_def 1; test_cmd_dev 2; test_cmd_cheat 4; test_cmd_server 6" );
	BOOST_CHECK_EQUAL( test, 1 );
	g_developer->Change( "1" );
	g_cheats->Change( "1" );
	Cmd::Instance( ).Execute( "test_cmd_def 1; test_cmd_dev 2; test_cmd_cheat 4; test_cmd_server 6" );
	BOOST_CHECK_EQUAL( test, 18 );

	test = 0;

	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetCmdFlags( "test_cmd_def" ), ( DWORD )Cmd::ECF_DEFAULT );
	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetCmdFlags( "test_cmd_dev" ), ( DWORD )Cmd::ECF_DEVELOPER );
	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetCmdFlags( "test_cmd_cheat" ), ( DWORD )Cmd::ECF_CHEAT );
	BOOST_CHECK_EQUAL( Cmd::Instance( ).GetCmdFlags( "test_cmd_server" ), ( DWORD )Cmd::ECF_SERVER_CMD );
}

BOOST_AUTO_TEST_CASE( engine )
{
	// init/shutdown has been tested in application
	// nothing to test here
}

void test_pack( Pack& p )
{
	BOOST_CHECK( p.GetFirstFile( ) != p.GetLastFile( ) );
	BOOST_CHECK_EQUAL( ( DWORD )std::distance( p.GetFirstFile( ), p.GetLastFile( ) ), ( DWORD )2 );

	Pack::FileIterator f1_iter = p.FindFile( "dir1\\file1.txt" );

	BOOST_CHECK( p.FindFile( "file2.txt" ) != p.GetLastFile( ) );
	BOOST_CHECK( f1_iter != p.GetLastFile( ) );

	String name;
	BOOST_CHECK_EQUAL( p.GetFileName( f1_iter, name ), S_OK );
	BOOST_CHECK_EQUAL( name, "dir1\\file1.txt" );

	unsigned long len = 0;
	BOOST_CHECK_EQUAL( p.GetFileSize( f1_iter, len ), S_OK );
	BOOST_CHECK_EQUAL( len, ( DWORD )9 );

	boost::uint8_t * mem = reinterpret_cast< boost::uint8_t * >( _alloca( sizeof( boost::uint8_t ) * len ) );
	BOOST_CHECK_EQUAL( p.ReadFile( f1_iter, mem ), S_OK );
	BOOST_CHECK( !memcmp( mem, "packfile1", len ) );

	unsigned long ofs = 0;
	BOOST_CHECK_EQUAL( p.GetFileOffset( f1_iter, ofs ), S_OK );

	FILE * pfile = fopen( "pack1.pack", "rb" );
	if( pfile )
	{
		boost::shared_ptr< FILE > p_guard( pfile, boost::bind( &fclose, _1 ) );

		fseek( pfile, ofs, SEEK_SET );
		fread( mem, len, 1, pfile );

		BOOST_CHECK( !memcmp( mem, "packfile1", len ) );
	}

	p.ClosePack( );

	BOOST_CHECK( p.GetFirstFile( ) == p.GetLastFile( ) );
}

BOOST_AUTO_TEST_CASE( pack )
{
	Pack p[ 3 ];
	BOOST_CHECK( p[ 0 ].GetFirstFile( ) == p[ 0 ].GetLastFile( ) );

	// create this pack from folder
	BOOST_CHECK_EQUAL( p[ 0 ].CreateFromDirectory( "PackData", "pack1.pack" ), S_OK );
	test_pack( p[ 0 ] );
	p[ 0 ].ClosePack( );

	BOOST_CHECK_EQUAL( p[ 1 ].LoadFile( "pack1.pack" ), S_OK );

	FILE * pfile = fopen( "pack1.pack", "rb" );
	BOOST_CHECK_NE( pfile, ( FILE * )NULL );
	if( pfile )
	{
		boost::shared_ptr< FILE > p_guard( pfile, boost::bind( &fclose, _1 ) );

		fseek( pfile, 0, SEEK_END );
		long pack_size = ftell( pfile );
		fseek( pfile, 0, SEEK_SET );

		boost::uint8_t * mem = reinterpret_cast< boost::uint8_t * >( _alloca( pack_size ) );
		fread( mem, pack_size, 1, pfile );

		BOOST_CHECK_EQUAL( p[ 2 ].LoadMemory( mem, pack_size ), S_OK );
	}
	//this->add( BOOST_PARAM_TEST_CASE( &test_pack, &p[ 0 ], &p[ 3 ] ) );
	std::for_each(
		&p[ 1 ],
		&p[ 3 ],
		boost::bind( &test_pack, _1 )
		);
}

BOOST_AUTO_TEST_CASE( fs )
{
	BOOST_CHECK_NE( xfx::FileSystem::Instance( ).FindFile( "file1.txt" ), S_OK );
	BOOST_CHECK_NE( xfx::FileSystem::Instance( ).FindFile( "file2.txt" ), S_OK );

	xfx::FileSystem::Instance( ).AddSearchPath( "FileData\\" );
	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).FindFile( "file1.txt" ), S_OK );
	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).FindFile( "FileData\\dir1\\file2.txt" ), S_OK );

	xfx::FileSystem::Instance( ).AddSearchPath( "FileData\\dir1\\" );
	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).FindFile( "file1.txt" ), S_OK );

	// now the file1.txt should be found in dir1
	// read its content and check
	unsigned long len = 0;
	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).GetFileSize( "file1.txt", len ), S_OK );
	BOOST_CHECK_EQUAL( len, ( unsigned long )8 );

	boost::uint8_t * mem = reinterpret_cast< boost::uint8_t * >( _alloca( sizeof( boost::uint8_t ) * len ) );
	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).ReadFile( "file1.txt", mem ), S_OK );

	BOOST_CHECK( !memcmp( mem, "dirfile1", len ) );

	// remove last search path and try to find file1.txt again
	xfx::FileSystem::Instance( ).RemoveSearchPath( "FileData\\dir1\\" );

	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).GetFileSize( "file1.txt", len ), S_OK );
	BOOST_CHECK_EQUAL( len, ( DWORD )5 );

	mem = reinterpret_cast< boost::uint8_t * >( _alloca( sizeof( boost::uint8_t ) * len ) );
	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).ReadFile( "file1.txt", mem ), S_OK );

	BOOST_CHECK( !memcmp( mem, "file1", len ) );

	xfx::FileSystem::Instance( ).RemoveAllSearchPaths( );
	BOOST_CHECK_NE( xfx::FileSystem::Instance( ).FindFile( "file1.txt" ), S_OK );

	// test packs
	boost::shared_ptr< Pack > p( new Pack( ) ), p_test;
	p->LoadFile( "pack1.pack" );
	xfx::FileSystem::Instance( ).AddPack( p );

	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).FindFile( "file2.txt", &p_test ), S_OK );
	BOOST_CHECK_EQUAL( p, p_test );
	xfx::FileSystem::Instance( ).AddSearchPath( "FileData\\" );
	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).FindFile( "dir1\\file1.txt", &p_test ), S_OK );
	BOOST_CHECK( p_test == NULL );

	BOOST_CHECK_EQUAL( xfx::FileSystem::Instance( ).WriteFile( "dir1\\file3.txt", "test", 4 ), S_OK );

	xfx::FileSystem::Instance( ).RemoveAllSearchPaths( );
	xfx::FileSystem::Instance( ).RemoveAllPacks( );
}

BOOST_AUTO_TEST_CASE( input )
{
	BOOST_CHECK_EQUAL( Input::Instance( ).Init( ), S_OK );
	BOOST_CHECK_NE( Input::Instance( ).Init( ), S_OK );
	BOOST_CHECK_EQUAL( Input::Instance( ).Shutdown( ), S_OK );
	BOOST_CHECK_NE( Input::Instance( ).Shutdown( ), S_OK );

	BOOST_CHECK_EQUAL( ::Application::Instance( ).InitEngine( static_cast< HINSTANCE >( GetCurrentProcess( ) ), "-?", L"TestWndClass", L"TestWndCaption" ), S_OK );
	Input::Instance( ).RetrieveData( );
	BOOST_CHECK_EQUAL( Input::Instance( ).TestKey( DIK_Q ), false );
	// don't know how to check key pressing and command execution
	/*
	SendMessage( gGetApplication( ).hWnd( ), WM_KEYDOWN, 'Q', 0x00110000 );
	Input::Instance( ).RetrieveData( );
	BOOST_CHECK_EQUAL( Input::Instance( ).TestKey( DIK_Q ), true );
	*/

	Input::Instance( ).Command( DIK_W, Input::FIRST_PUSHED, "cmd1" );
	Input::Instance( ).Command( DIK_W, Input::PUSHED, "cmd2" );
	Input::Instance( ).Command( DIK_W, Input::POPED, "cmd3" );
	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_W, Input::FIRST_PUSHED ), "cmd1" );
	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_W, Input::PUSHED ), "cmd2" );
	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_W, Input::POPED ), "cmd3" );

	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_Q, Input::FIRST_PUSHED ), "" );
	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_Q, Input::PUSHED ), "" );
	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_Q, Input::POPED ), "" );
	Cmd::Instance( ).Execute( "bind +q qcmd1" );
	Cmd::Instance( ).Execute( "bind q qcmd2" );
	Cmd::Instance( ).Execute( "bind -q qcmd3" );
	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_Q, Input::FIRST_PUSHED ), "qcmd1" );
	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_Q, Input::PUSHED ), "qcmd2" );
	BOOST_CHECK_EQUAL( Input::Instance( ).Command( DIK_Q, Input::POPED ), "qcmd3" );

	BOOST_CHECK_EQUAL( Input::Instance( ).Shutdown( ), S_OK );
	::Application::Instance( ).Shutdown( );
}

BOOST_AUTO_TEST_SUITE_END( )

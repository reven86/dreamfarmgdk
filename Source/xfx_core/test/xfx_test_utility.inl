BOOST_AUTO_TEST_SUITE( xfx_utility_test_suite )

BOOST_AUTO_TEST_CASE( string )
{
	String test_text(
		"  0123 dasd\t ddd\n"
		"line2     last_token\n"
		"\"line3 in quotes\"\n\n"
		"\"line4, \\\"quotes\\\" in quotes with slash \\\\\"\n"
		"1234"
		);

	String::size_type s = 0;
	String token;

	token = next_token( test_text, s );
	BOOST_CHECK_EQUAL( token, "0123" );

	token = next_token( test_text, s );
	BOOST_CHECK_EQUAL( token, "dasd" );

	token = next_token( test_text, s );
	BOOST_CHECK_EQUAL( token, "ddd" );

	token = next_token( test_text, s );
	BOOST_CHECK_EQUAL( token, "line2" );

	token = next_token( test_text, s );
	BOOST_CHECK_EQUAL( token, "last_token" );

	token = next_token( test_text, s );
	BOOST_CHECK_EQUAL( token, "line3 in quotes" );

	token = next_token( test_text, s );
	BOOST_CHECK_EQUAL( token, "line4, \"quotes\" in quotes with slash \\" );

	BOOST_CHECK( test_token( test_text, s, "1234" ) );

	token = next_token( test_text, s );
	BOOST_CHECK( token.empty( ) );

	BOOST_CHECK( fromUTF8( "\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82" ) == L"привет" );
	BOOST_CHECK( fromMBCS( "привет" ) == L"привет" );
	BOOST_CHECK( toMBCS( L"привет" ) == "привет" );
}

BOOST_AUTO_TEST_CASE( argb )
{
	ARGB a( 0xff7f4f2f );
	BOOST_CHECK_EQUAL( sizeof( a.dword ), sizeof( boost::uint32_t ) );

	ARGB b( 0xff, 0x7f, 0x4f, 0x2f );

	BOOST_CHECK_EQUAL( a.dword, b.dword );
	BOOST_CHECK_EQUAL( b.r, a.g + 0x30 );
	BOOST_CHECK_EQUAL( b.b, 0x2f );

	b.dword = 0x11223344;
	
	ARGB c;

	c = ARGB::Interpolate( a, b, 0.0f );
	BOOST_CHECK_EQUAL( c.dword, a.dword );
	c = ARGB::Interpolate( a, b, 1.0f );
	BOOST_CHECK_EQUAL( c.dword, b.dword );
	c = ARGB::Interpolate( a, b, 0.2f );
	BOOST_CHECK_EQUAL( c.dword, 3479980339 );

	BOOST_CHECK( !( a == c ) );
	BOOST_CHECK( b != c );
	BOOST_CHECK( a > c );
	BOOST_CHECK( b < c );

	ARGB one( 0xffffffff );
	ARGB one_half( 0x7f7f7f7f );

	BOOST_CHECK_EQUAL( b.dword, ( b * one ).dword );
	BOOST_CHECK_EQUAL( ( b * 0.5f ).dword, ( b * one_half ).dword );
	BOOST_CHECK_EQUAL( ( a * 0.5f ).dword, ( a * one_half ).dword );

	unsigned pos = 0;
	BOOST_CHECK( ParseVariable< String >( c, pos, "0x44332211" ) == S_OK );
	BOOST_CHECK_EQUAL( c.dword, static_cast< boost::uint32_t >( 0x44332211 ) );

	BOOST_CHECK_EQUAL( SimpleLerpFn< ARGB >( ).lerp( a, b, 0.9f ).dword, ARGB::Interpolate( a, b, 0.9f ).dword );
}

BOOST_AUTO_TEST_SUITE_END( )

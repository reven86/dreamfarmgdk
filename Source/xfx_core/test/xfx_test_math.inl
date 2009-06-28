static const float gMathEps = 0.01f;
static const float gMathEpsSq = gMathEps * gMathEps;

BOOST_AUTO_TEST_SUITE( xfx_math_test_suite )

BOOST_AUTO_TEST_CASE( math )
{
	srand( static_cast< unsigned >( time( NULL ) ) );

	BOOST_CHECK_LT( fabs( math_cos( Math::pi / 3 ) - 0.5f ), gMathEps );
	BOOST_CHECK_LT( fabs( math_sin( Math::pi / 2 ) - 1.0f ), gMathEps );

	for( Math::BigAngle ang = -Math::pi; ang < Math::pi; ang += ( rand( ) % 10 ) )
		BOOST_CHECK_LT( fabs( math_cos( ang ) * math_cos( ang ) + math_sin( ang ) * math_sin( ang ) - 1.0f ), gMathEpsSq );

	BOOST_CHECK_LT( fabs( math_acos( 0.5f ) / ( Math::pi / 3 ) - 1.00f ), gMathEps );
	BOOST_CHECK_LT( fabs( math_asin( 0.5f ) / ( Math::pi / 6 ) - 1.00f ), gMathEps );
	BOOST_CHECK_LT( fabs( math_invsqrt( 10.0f ) - 1.0f / sqrtf( 10.0f ) ), gMathEps );
	BOOST_CHECK_LT( fabs( math_sqrt( 10.0f ) - sqrtf( 10.0f ) ), gMathEps );

	BOOST_CHECK_LT( fabs( math_sqrt( 10.0f ) - sqrtf( 10.0f ) ), gMathEps );

	Math::Angle a = 0;
	Math::BigAngle ba = 0;
	String::size_type s = 0;
	BOOST_CHECK_EQUAL( ParseAngleVariable( a, s, "/*comment*/ 30" ), S_OK );
	BOOST_CHECK_EQUAL( a, Math::pi / 6 );

	s = 0;
	BOOST_CHECK_EQUAL( ParseBigAngleVariable( ba, s, "/*comment*/ 810" ), S_OK );
	BOOST_CHECK_EQUAL( ba, 4 * Math::pi + Math::pi / 2 );
}

BOOST_AUTO_TEST_CASE( vec2 )
{
	Vec2 v1( 1.1f );
	BOOST_CHECK_EQUAL( v1.x, v1.y );
	BOOST_CHECK_EQUAL( v1.x, 1.1f );

	Vec2 v2( 1.1f, 2.2f );
	BOOST_CHECK_EQUAL( v2.y, v1.x * 2.0f );

	v1 = -v1;
	v1 += v2 - Vec2( 0.3f, 1.5f );
	v1 -= Vec2( 1.2f, 2.2f ) * 0.1f;
	v1 *= 5.0f;
	v1 *= v2 / 2.0f;
	v1 /= 10.0f;
	v1[ 0 ] += v1[ 1 ];

	BOOST_CHECK_LT( fabs( ( v1 - Vec2( -0.4565f, -0.341f ) ).LenSq( ) ), gMathEpsSq );
	BOOST_CHECK( v2 == Vec2( 1.1f, 2.2f ) );
	BOOST_CHECK( v2 != Vec2( 1.1f, 2.20001f ) );
	
	v1.Normalize( );
	BOOST_CHECK_LT( fabs( v1.LenSq( ) - 1.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( v2.GetNormalized( ).Len( ) - 1.0f ), gMathEps );
	
	BOOST_CHECK_LT( fabs( Vec2::Lerp( Vec2( 1.2f ), Vec2( 0.5f, 1.5f ), 0.0f ).LenSq( ) - 2.88f ), gMathEpsSq );
	BOOST_CHECK_LT( fabs( Vec2::Lerp( Vec2( 1.2f ), Vec2( 0.5f, 1.5f ), 1.0f ).LenSq( ) - 2.5f ), gMathEpsSq );
	BOOST_CHECK_LT( fabs( Vec2::Lerp( Vec2( 1.2f ), Vec2( 0.5f, 1.5f ), 0.2f ).x - 1.06f ), gMathEps );
	BOOST_CHECK_EQUAL( Vec2::Dot( Vec2( 1.1f, 1.3f ), Vec2( -1.3f, 1.1f ) ), 0.0f );
	BOOST_CHECK_EQUAL( Vec2::Dot( Vec2( 1.1f, 1.3f ), Vec2( 1.3f, 1.1f ) ), 1.1f * 1.3f + 1.3f * 1.1f );

	Vec2::Clip( v1 );
	Vec2::Clip( v2 );
	BOOST_CHECK_EQUAL( v1.x, 0.0f );
	BOOST_CHECK_EQUAL( v1.y, 0.0f );
	BOOST_CHECK_EQUAL( v2.x, 1.0f );
	BOOST_CHECK_EQUAL( v2.y, 1.0f );

	String::size_type s = 0;
	BOOST_CHECK_EQUAL( ParseVariable< String >( v1, s, " 0.44 0.66" ), S_OK );
	BOOST_CHECK_EQUAL( v1.x, 0.44f );
	BOOST_CHECK_EQUAL( v1.y, 0.66f );
}

BOOST_AUTO_TEST_CASE( vec3 )
{
	Vec3 v1( 1.1f );
	BOOST_CHECK_EQUAL( v1.x, v1.y );
	BOOST_CHECK_EQUAL( v1.x, 1.1f );
	BOOST_CHECK_EQUAL( v1.z, v1.y );

	Vec3 v2( 1.1f, 2.2f, 3.3f );
	BOOST_CHECK_EQUAL( v2.z, 3.3f );

	v1 = -v1;
	v1 += v2 - Vec3( 0.3f, 1.5f, 0.1f );
	v1 -= Vec3( 1.2f, 2.2f, 0.5f ) * 0.1f;
	v1 *= 5.0f;
	v1 *= v2 / 2.0f;
	v1 /= 10.0f;
	v1[ 0 ] += v1[ 1 ];

	BOOST_CHECK_LT( fabs( ( v1 - Vec3( -0.4565f, -0.341f, 1.69125f ) ).LenSq( ) ), gMathEps );
	BOOST_CHECK( v2 == Vec3( 1.1f, 2.2f, 3.3f ) );
	BOOST_CHECK( v2 != Vec3( 1.1f, 2.20001f, 3.3f ) );
	
	v1.Normalize( );
	BOOST_CHECK_LT( fabs( v1.LenSq( ) - 1.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( v2.GetNormalized( ).Len( ) - 1.0f ), gMathEps );
	
	BOOST_CHECK_LT( fabs( Vec3::Lerp( Vec3( 1.2f ), Vec3( 0.5f, 1.5f, 1.7f ), 0.0f ).LenSq( ) - 4.32f ), gMathEpsSq );
	BOOST_CHECK_LT( fabs( Vec3::Lerp( Vec3( 1.2f ), Vec3( 0.5f, 1.5f, 1.7f ), 1.0f ).LenSq( ) - 5.39f ), gMathEpsSq );
	BOOST_CHECK_LT( fabs( Vec3::Lerp( Vec3( 1.2f ), Vec3( 0.5f, 1.5f, 1.7f ), 0.2f ).z - 1.3f ), gMathEps );
	BOOST_CHECK_LT( fabs( Vec3::Dot( Vec3( 1.1f, 1.3f, 1.2f ), Vec3( 1.3f, 1.1f, 3.4f ) ) - 1.1f * 1.3f - 1.3f * 1.1f - 1.2f * 3.4f ), gMathEpsSq );

	Vec3::Clip( v1 );
	Vec3::Clip( v2 );
	BOOST_CHECK_EQUAL( v1.x, 0.0f );
	BOOST_CHECK_EQUAL( v1.y, 0.0f );
	BOOST_CHECK_NE( v1.z, 0.0f );
	BOOST_CHECK_EQUAL( v2.x, 1.0f );
	BOOST_CHECK_EQUAL( v2.y, 1.0f );
	BOOST_CHECK_EQUAL( v2.z, 1.0f );

	Vec3 v3 = Vec3::Cross( v1, Vec3( -v1.x, v1.z, v1.y ) );
	BOOST_CHECK_EQUAL( Vec3::Dot( v3, v1 ), 0.0f );
	BOOST_CHECK_EQUAL( Vec3::Dot( v3, Vec3( -v1.x, v1.z, v1.y ) ), 0.0f );

	String::size_type s = 0;
	BOOST_CHECK_EQUAL( ParseVariable< String >( v1, s, " 0.44 0.66 \n0.11" ), S_OK );
	BOOST_CHECK_EQUAL( v1.x, 0.44f );
	BOOST_CHECK_EQUAL( v1.y, 0.66f );
	BOOST_CHECK_EQUAL( v1.z, 0.11f );
}

BOOST_AUTO_TEST_CASE( vec4 )
{
	Vec4 v1( 1.1f );
	BOOST_CHECK_EQUAL( v1.x, v1.y );
	BOOST_CHECK_EQUAL( v1.x, 1.1f );
	BOOST_CHECK_EQUAL( v1.z, v1.y );
	BOOST_CHECK_EQUAL( v1.w, v1.y );

	Vec4 v2( 1.1f, 2.2f, 3.3f, 4.4f );
	BOOST_CHECK_EQUAL( v2.z, 3.3f );
	BOOST_CHECK_EQUAL( v2.w, 4.4f );

	v1 = -v1;
	v1 += v2 - Vec4( 0.3f, 1.5f, 0.1f, 0.3f );
	v1 -= Vec4( 1.2f, 2.2f, 0.5f, 0.7f ) * 0.1f;
	v1 *= 5.0f;
	v1 *= v2 / 2.0f;
	v1 /= 10.0f;
	v1[ 0 ] += v1[ 1 ];

	BOOST_CHECK_LT( fabs( ( v1 - Vec4( -0.4565f, -0.341f, 1.69125f, 3.377f ) ).LenSq( ) ), gMathEpsSq );
	BOOST_CHECK( v2 == Vec4( 1.1f, 2.2f, 3.3f, 4.4f ) );
	BOOST_CHECK( v2 != Vec4( 1.1f, 2.20001f, 3.3f, 4.4f ) );
	
	v1.Normalize( );
	BOOST_CHECK_LT( fabs( v1.LenSq( ) - 1.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( v2.GetNormalized( ).Len( ) - 1.0f ), gMathEps );
	
	BOOST_CHECK_LT( fabs( Vec4::Lerp( Vec4( 1.2f ), Vec4( 0.5f, 1.5f, 1.7f, 1.3f ), 0.0f ).LenSq( ) - 5.76f ), gMathEpsSq );
	BOOST_CHECK_LT( fabs( Vec4::Lerp( Vec4( 1.2f ), Vec4( 0.5f, 1.5f, 1.7f, 1.3f ), 1.0f ).LenSq( ) - 7.08f ), gMathEpsSq );
	BOOST_CHECK_LT( fabs( Vec4::Lerp( Vec4( 1.2f ), Vec4( 0.5f, 1.5f, 1.7f, 1.3f ), 0.2f ).w - 1.22f ), gMathEps );
	BOOST_CHECK_LT( fabs( Vec4::Dot( Vec4( 1.1f, 1.3f, 1.2f, 1.4f ), Vec4( 1.3f, 1.1f, 3.4f, 2.3f ) ) - 1.1f * 1.3f - 1.3f * 1.1f - 1.2f * 3.4f - 1.4f * 2.3f ), gMathEpsSq );

	Vec4::Clip( v1 );
	Vec4::Clip( v2 );
	BOOST_CHECK_EQUAL( v1.x, 0.0f );
	BOOST_CHECK_EQUAL( v1.y, 0.0f );
	BOOST_CHECK_NE( v1.z, 0.0f );
	BOOST_CHECK_LT( v1.w, 1.0f );
	BOOST_CHECK_EQUAL( v2.x, 1.0f );
	BOOST_CHECK_EQUAL( v2.y, 1.0f );
	BOOST_CHECK_EQUAL( v2.z, 1.0f );
	BOOST_CHECK_EQUAL( v2.w, 1.0f );
}

BOOST_AUTO_TEST_CASE( mat3 )
{
	Mat3 m1( 3.0f );

	int i, j;
	for( i = 0; i < 3; i++ )
		for( j = 0; j < 3; j++ )
			if( i != j )
				BOOST_CHECK_EQUAL( m1.x[ i ][ j ], 0.0f );
			else
				BOOST_CHECK_EQUAL( m1.x[ i ][ j ], i == 2 ? 1.0f : 3.0f );

	Mat3 m2(
		1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f,
		7.0f, 8.0f, 9.0f
		);

	BOOST_CHECK_EQUAL( m2.x[ 1 ][ 2 ], 6.0f );
	BOOST_CHECK_EQUAL( m2._23, 6.0f );

	Mat3 m3( Vec2( 1.0f, 2.0f ), Math::pi / 3, Vec2( 0.5f, 2.0f ) );
	Vec2 p( 2.0f );

	Vec2 t = m3.TransformCoord( p );
	BOOST_CHECK_LT( fabs( t.x - ( 1.0f - 0.25881904510252076234889883762405f * 2.0f * sqrtf( 2.0f ) * 0.5f ) ), gMathEps );
	BOOST_CHECK_LT( fabs( t.y - ( 2.0f + 0.9659258262890682867497431997289f * 2.0f * sqrtf( 2.0f ) * 2.0f ) ), gMathEps );

	m1.Identity( );
	for( i = 0; i < 3; i++ )
		for( j = 0; j < 3; j++ )
			if( i != j )
				BOOST_CHECK_EQUAL( m1.x[ i ][ j ], 0.0f );
			else
				BOOST_CHECK_EQUAL( m1.x[ i ][ j ], 1.0f );

	BOOST_CHECK( m1.IsIdentity( ) );

	m1.Rotation( Math::pi / 3 );
	m1.SetTranslation( Vec2( 1.0f, 2.0f ) );
	m1.SetScale( Vec2( 0.5f, 2.0f ) );

	BOOST_CHECK_LT( fabs( m1.GetTranslation( ).y - 2.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( m1.GetScale( ).x - 0.5f ), gMathEps );

	BOOST_CHECK( !memcmp( &m1.x, &m3.x, sizeof( m1.x ) ) );
	m3.Inverse( );
	BOOST_CHECK( ( m1 * m3 ).IsIdentity( ) );

	p = m3.TransformCoord( t );
	BOOST_CHECK_LT( fabs( p.x - 2.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( p.y - 2.0f ), gMathEps );

	m1 = m3.GetInversedAffine( );
	BOOST_CHECK( ( m1 * m3 ).IsIdentity( ) );
}

BOOST_AUTO_TEST_CASE( mat4 )
{
	Mat4 m1( 3.0f );

	int i, j;
	for( i = 0; i < 4; i++ )
		for( j = 0; j < 4; j++ )
			if( i != j )
				BOOST_CHECK_EQUAL( m1.x[ i ][ j ], 0.0f );
			else
				BOOST_CHECK_EQUAL( m1.x[ i ][ j ], i == 3 ? 1.0f : 3.0f );

	Mat4 m2(
		1.0f, 2.0f, 3.0f, 10.0f,
		4.0f, 5.0f, 6.0f, 11.0f,
		7.0f, 8.0f, 9.0f, 12.0f,
		13.0f, 14.0f, 15.0f, 16.0f
		);

	BOOST_CHECK_EQUAL( m2.x[ 2 ][ 3 ], 12.0f );
	BOOST_CHECK_EQUAL( m2._34, 12.0f );

	Mat4 m3( Vec3( 1.0f, 2.0f, 3.0f ), Euler( Math::pi / 6, Math::pi / 3, Math::pi / 2 ), Vec3( 0.5f, 2.0f, 3.0f ) );
	Vec3 p( 2.0f );

	Vec3 t = m3.TransformCoord( p );
	BOOST_CHECK_LT( fabs( t.x - ( 1.0f - 0.366025404f * 0.5f ) ), gMathEps );
	BOOST_CHECK_LT( fabs( t.y - ( 2.0f - 0.732050807f * 2.0f ) ), gMathEps );
	BOOST_CHECK_LT( fabs( t.z - ( 3.0f + 3.36602540329f * 3.0f ) ), gMathEps );

	m1.Identity( );
	for( i = 0; i < 4; i++ )
		for( j = 0; j < 4; j++ )
			if( i != j )
				BOOST_CHECK_EQUAL( m1.x[ i ][ j ], 0.0f );
			else
				BOOST_CHECK_EQUAL( m1.x[ i ][ j ], 1.0f );

	BOOST_CHECK( m1.IsIdentity( ) );

	m2.RotationZ( Math::pi / 2 );
	m1 *= m2;
	m2.RotationX( Math::pi / 3 );
	m1 = m1 * m2;
	m2.RotationY( Math::pi / 6 );
	m1 = ( m2.GetTransposed( ) * m1.GetTransposed( ) ).GetTransposed( );

	m1.SetTranslation( Vec3( 1.0f, 2.0f, 3.0f ) );
	m1.SetScale( Vec3( 0.5f, 2.0f, 3.0f ) );

	BOOST_CHECK_LT( fabs( m1.GetTranslation( ).z - 3.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( m1.GetScale( ).z - 3.0f ), gMathEps );

	BOOST_CHECK( !memcmp( &m1.x, &m3.x, sizeof( m1.x ) ) );
	m3.Inverse( );
	BOOST_CHECK( ( m1 * m3 ).IsIdentity( ) );

	p = m3.TransformCoord( t );
	BOOST_CHECK_LT( fabs( p.x - 2.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( p.y - 2.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( p.z - 2.0f ), gMathEps );

	m1 = m3.GetInversedAffine( );
	BOOST_CHECK( ( m1 * m3 ).IsIdentity( ) );

	m2.RotationAxisByAngle( Vec3( 3.0f, 0.0f, 0.0f ), Math::pi / 6 );
	m1.RotationX( -Math::pi / 6 );

	BOOST_CHECK( ( m2 * m1 ).IsIdentity( ) );
}

BOOST_AUTO_TEST_CASE( quaternion )
{
	Quaternion q1( 1.0f, 2.0f, 3.0f, 4.0f );
	BOOST_CHECK_LT( fabs( q1.x - 1.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( q1.y - 2.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( q1.z - 3.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( q1.w - 4.0f ), gMathEps );

	q1 = Quaternion( Vec3( 1.0f, 2.0f, 3.0f ), Math::pi / 3 );
	Mat4 m1 = q1.ToMat4( );
	Mat4 m2;

	m2.RotationAxisByAngle( Vec3( 1.0f, 2.0f, 3.0f ), Math::pi / 3 );
	BOOST_CHECK( ( m2.GetInversedAffine( ) * m1 ).IsIdentity( ) );
	BOOST_CHECK( ( m2 * q1.GetInversed( ).ToMat4( ) ).IsIdentity( ) );

	q1.xyzw( 1.0f, 2.0f, 3.0f, 4.0f );
	BOOST_CHECK_LT( fabs( q1.LenSq( ) - 30.0f ), gMathEpsSq );

	Quaternion q2( 3.0f, 1.0f, 2.0f, 4.0f );
	q1 += ( -q2 + 2.0f * Quaternion( 1.0f, 0.0f, 0.0f, 0.0f ) ) * 0.5f;
	q1 -= ( q1 - Quaternion( 0.0f, 1.0f, 0.0f, 0.0f ) * 0.1f ) * 3.0f;
	q1 *= 2.0f;
	q1 /= 4.0f;

	BOOST_CHECK( q1 == Quaternion( -0.5f, -1.3499999f, -2.0f, -2.0f ) );
	BOOST_CHECK( !( q1 != Quaternion( -0.5f, -1.3499999f, -2.0f, -2.0f ) ) );

	BOOST_CHECK_LT( fabs( q1.GetNormalized( ).LenSq( ) - 1.0f ), gMathEpsSq );
	q1.Normalize( );
	BOOST_CHECK_LT( fabs( q1.Len( ) - 1.0f ), gMathEps );

	q1.FromAxisAngle( Vec3( 3.0f, 2.0f, 1.0f ), Math::pi / 6 );
	BOOST_CHECK( ( q1 * q1.GetInversed( ) ).ToMat4( ).IsIdentity( ) );

	Vec3 axis;
	Math::Angle angle;
	boost::tie( axis, angle ) = q1.ToAxisAngle( );
	BOOST_CHECK_LT( ( axis - Vec3( 3.0f, 2.0f, 1.0f ).GetNormalized( ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( abs( angle - Math::pi / 6 ), 3 );

	// check slerp
	Vec3 v1, v2;
	Mat4 m;

	q1.FromAxisAngle( Vec3( 0.0f, 1.0f, 0.0f ), -Math::pi / 2 );
	m = q1.ToMat4( );
	v1.xyz( m._31, m._32, m._33 );

	q2 = Quaternion( Vec3( 0.0f, 0.0f, 1.0f ), -Math::pi / 2 ) * q1;
	m = q2.ToMat4( );
	v2.xyz( m._31, m._32, m._33 );

	Quaternion q = Quaternion::Slerp( q1, q2, 0.33333f );
	m = q.ToMat4( );
	BOOST_CHECK_LT( fabs( m._33 ), gMathEps );
	q = Quaternion::Slerp( q1, q2, 0.66666f );
	m = q.ToMat4( );
	BOOST_CHECK_LT( fabs( m._33 ), gMathEps );

	//*
	q1.FromAxisAngle( Vec3::Rand( ), math_normalize_angle( static_cast< Math::BigAngle >( math_frand( ) * Math::pi * 30.0f ) ) );
	m = q1.ToMat4( );
	v1.xyz( m._31, m._32, m._33 );

	// make second vector as first, rotate about local x axis no more than 0.9 * pi
	q2 = q1 * Quaternion( Vec3( 1.0f, 0.0f, 0.0f ), static_cast< Math::Angle >( math_frand( ) * Math::pi * 0.9f ) );
	m = q2.ToMat4( );
	v2.xyz( m._31, m._32, m._33 );

	float cosv1v2 = Vec3::Dot( v1, v2 );
	Math::Angle alpha = math_acos( cosv1v2 );
	
	q = Quaternion::Slerp( q1, q2, 0.33333f );
	m = q.ToMat4( );
	BOOST_CHECK_LT( ( Vec3( m._31, m._32, m._33 ) - Vec3::Lerp( v1, v2, math_sin( alpha / 3 ) / ( math_cos( alpha / 6 ) * math_sqrt( 2.0f - 2.0f * cosv1v2 ) ) ).GetNormalized( ) ).LenSq( ), gMathEps );
	q = Quaternion::Slerp( q1, q2, 0.66666f );
	m = q.ToMat4( );
	BOOST_CHECK_LT( ( Vec3( m._31, m._32, m._33 ) - Vec3::Lerp( v1, v2, math_sin( 2 * alpha / 3 ) / ( math_cos( alpha / 6 ) * math_sqrt( 2.0f - 2.0f * cosv1v2 ) ) ).GetNormalized( ) ).LenSq( ), gMathEps );
}

BOOST_AUTO_TEST_CASE( euler )
{
	Euler e1( 3 * Math::pi / 2 );
	BOOST_CHECK_EQUAL( e1.yaw, 3 * Math::pi / 2 );

	Euler e2( Math::pi * 4, Math::pi * 5 / 2, Math::pi * 6 / 7 );
	BOOST_CHECK_EQUAL( e2.roll, Math::pi * 6 / 7 );

	e1 += e2 * 2.0f;
	e2 -= -e2 + e1 - 3.0f * ( e1 - e2 );
	BOOST_CHECK_EQUAL( e2.yaw, 491520 );
	BOOST_CHECK_EQUAL( e2.pitch, 344064 );
	BOOST_CHECK_EQUAL( e2.roll, 182562 );
	BOOST_CHECK( e2 == Euler( 491520, 344064, 182562 ) );

	Mat4 m = e2.ToMat4( );
	e1 = Euler( ).FromMat4( m * Mat4( 3.0f ) );

	Vec3 f1, u1, r1;
	Vec3 f2, u2, r2;
	e1.AngleDirections( f1, u1, r1 );
	e2.AngleDirections( f2, u2, r2 );

	BOOST_CHECK_LT( ( f1 - f2 ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( u1 - u2 ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( r1 - r2 ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( f2 - Vec3( m._31, m._32, m._33 ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( u2 - Vec3( m._21, m._22, m._23 ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( r2 - Vec3( m._11, m._12, m._13 ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( f2 - e1.Forward( ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( u2 - e1.Up( ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( r2 - e1.Right( ) ).LenSq( ), gMathEpsSq );

	// check quaternion conversation
	BOOST_CHECK( ( e2.ToQuaternion( ).ToMat4( ) * m.GetInversedAffine( ) ).IsIdentity( ) );

	Mat4 mm;
	mm.RotationAxisByAngle( Vec3( 1.0f, 2.0f, 3.0f ), Math::pi / 4 );

	Quaternion q( Vec3( 1.0f, 2.0f, 3.0f ), Math::pi / 4 );
	e2.FromQuaternion( q );

	BOOST_CHECK( ( ( m * mm ).GetInversedAffine( ) * ( m * e2.ToMat4( ) ) ).IsIdentity( ) );

	// check Vec3 conversation

	e2.FromVec3( u1 * 3.0f );
	BOOST_CHECK_LT( fabs( Vec3::Dot( e2.Forward( ), u1 ) - 1.0f ), gMathEpsSq );

	e1 = Euler( 0 );
	e1.Rotate( Vec3( 1.0f ), 2 * Math::pi / 3 );
	BOOST_CHECK_LT( ( e1.Forward( ) - Vec3( 1.0f, 0.0f, 0.0f ) ).LenSq( ), gMathEpsSq ); 
	BOOST_CHECK_LT( ( e1.Up( ) - Vec3( 0.0f, 0.0f, 1.0f ) ).LenSq( ), gMathEpsSq ); 
	BOOST_CHECK_LT( ( e1.Right( ) - Vec3( 0.0f, 1.0f, 0.0f ) ).LenSq( ), gMathEpsSq  ); 

	e1.FromVec3( Vec3( 0.5f, 0.5f, 0.0f ) );
	f1 = e1.Forward( );
	BOOST_CHECK_LT( fabs( f1.x - math_invsqrt( 2.0f ) ), gMathEps );
	BOOST_CHECK_LT( fabs( f1.y - math_invsqrt( 2.0f ) ), gMathEps );
	BOOST_CHECK_LT( fabs( f1.z ), gMathEps );

	// check slerp, aslo checks slerp on quaternions
	/*

	Vec3 v1, v2;
	Euler e;

	v1 = Vec3( 1.0f, 0.0f, 0.0f );
	v2 = Vec3( 0.01f, 1.0f, 0.0f );
	e1.FromVec3( v1 );
	e2.FromVec3( v2 );
	v1.Normalize( );
	v2.Normalize( );

	e = Euler::Slerp( e1, e2, 0.3f );
	BOOST_CHECK_LT( fabs( e.Forward( ).z ), gMathEps );
	e = Euler::Slerp( e1, e2, 0.6f );
	BOOST_CHECK_LT( fabs( e.Forward( ).z ), gMathEps );

	// check Euler::Slerp extremal case (pitch is about pi/2)

	m.RotationY( -Math::pi / 2 );
	v1.xyz( m._31, m._32, m._33 );
	e1.FromMat4( m );

	Mat4 m2;
	m2.RotationZ( -Math::pi / 2 );

	m *= m2;
	v2.xyz( m._31, m._32, m._33 );
	e2.FromMat4( m );

	float cosv1v2 = Vec3::Dot( v1, v2 );
	Math::Angle alpha = math_acos( cosv1v2 );
	
	e = Euler::Slerp( e1, e2, 0.33333f );
	BOOST_CHECK_LT( ( e.Forward( ) - Vec3::Lerp( v1, v2, math_sin( alpha / 3 ) / ( math_cos( alpha / 6 ) * math_sqrt( 2.0f - 2.0f * cosv1v2 ) ) ).GetNormalized( ) ).LenSq( ), gMathEpsSq );
	e = Euler::Slerp( e1, e2, 0.66666f );
	BOOST_CHECK_LT( ( e.Forward( ) - Vec3::Lerp( v1, v2, math_sin( 2 * alpha / 3 ) / ( math_cos( alpha / 6 ) * math_sqrt( 2.0f - 2.0f * cosv1v2 ) ) ).GetNormalized( ) ).LenSq( ), gMathEpsSq );

	// check usual case for Euler::Slerp

	m.Identity( );
	v1.xyz( m._31, m._32, m._33 );
	e1.FromMat4( m );

	m2.RotationX( 2 * Math::pi / 3 );

	m *= m2;
	v2.xyz( m._31, m._32, m._33 );
	e2.FromMat4( m );

	cosv1v2 = Vec3::Dot( v1, v2 );
	alpha = math_acos( cosv1v2 );
	
	e = Euler::Slerp( e1, e2, 0.33333f );
	BOOST_CHECK_LT( fabs( e.Forward( ).x ), gMathEps );
	BOOST_CHECK_LT( ( e.Forward( ) - Vec3::Lerp( v1, v2, math_sin( alpha / 3 ) / ( math_cos( alpha / 6 ) * math_sqrt( 2.0f - 2.0f * cosv1v2 ) ) ).GetNormalized( ) ).LenSq( ), gMathEpsSq );
	e = Euler::Slerp( e1, e2, 0.66666f );
	BOOST_CHECK_LT( fabs( e.Forward( ).x ), gMathEps );
	BOOST_CHECK_LT( ( e.Forward( ) - Vec3::Lerp( v1, v2, math_sin( 2 * alpha / 3 ) / ( math_cos( alpha / 6 ) * math_sqrt( 2.0f - 2.0f * cosv1v2 ) ) ).GetNormalized( ) ).LenSq( ), gMathEpsSq  );

	// check usual case for Euler::Slerp

	m.RotationAxisByAngle( Vec3::Rand( ), static_cast< Math::BigAngle >( math_frand( ) * Math::pi * 30.0f ) );
	v1.xyz( m._31, m._32, m._33 );
	e1.FromMat4( m );

	m.RotationAxisByAngle( Vec3::Rand( ), static_cast< Math::BigAngle >( math_frand( ) * Math::pi * 30.0f ) );
	v2.xyz( m._31, m._32, m._33 );
	e2.FromMat4( m );

	cosv1v2 = Vec3::Dot( v1, v2 );
	alpha = math_acos( cosv1v2 );
	
	e = Euler::Slerp( e1, e2, 0.33333f );
	BOOST_CHECK_LT( ( e.Forward( ) - Vec3::Lerp( v1, v2, math_sin( alpha / 3 ) / ( math_cos( alpha / 6 ) * math_sqrt( 2.0f - 2.0f * cosv1v2 ) ) ).GetNormalized( ) ).LenSq( ), gMathEpsSq );
	e = Euler::Slerp( e1, e2, 0.66666f );
	BOOST_CHECK_LT( ( e.Forward( ) - Vec3::Lerp( v1, v2, math_sin( 2 * alpha / 3 ) / ( math_cos( alpha / 6 ) * math_sqrt( 2.0f - 2.0f * cosv1v2 ) ) ).GetNormalized( ) ).LenSq( ), gMathEpsSq  );
	*/

	Euler e;
	String::size_type s = 0;
	BOOST_CHECK_EQUAL( ParseVariable< String >( e, s, "810 /*ccc*/ 3600 -1" ), S_OK );
	BOOST_CHECK_EQUAL( e.yaw, Math::pi * 4 + Math::pi / 2 );
	BOOST_CHECK_EQUAL( e.pitch, Math::pi * 20 );
	BOOST_CHECK_EQUAL( e.roll, -Math::pi / 180 );
}

BOOST_AUTO_TEST_CASE( primitives )
{
	// check Ray
	Ray r;
	BOOST_CHECK( r.Origin( ) == Vec3( 0.0f ) );
	BOOST_CHECK( r.Direction( ) == Vec3( 0.0f, 0.0f, 1.0f ) );

	r = Ray( Vec3( 3.0f, 2.0f, 1.0f ), Vec3( 1.0f, 2.0f, 3.0f ) );
	BOOST_CHECK_LT( ( r.Point( 2.0f ) - Vec3( 5.0f, 6.0f, 7.0f ) ).LenSq( ), gMathEpsSq );

	Mat4 m( Vec3( -2.0f, -1.0f, 0.0f ), Euler( Math::pi, 0, 0 ), Vec3( 2.0f ) );
	r.Transform( m );
	BOOST_CHECK_LT( ( r.Point( -1.0f ) - Vec3( -6.0f, -1.0f, 4.0f ) ).LenSq( ), gMathEpsSq );

	// check sphere
	Primitives::Sphere s;
	BOOST_CHECK( s.Position( ) == Vec3( 0.0f ) );
	BOOST_CHECK_EQUAL( s.Radius( ), 1.0f );

	s.Position( ).xyz( 1.0f, 2.0f, 3.0f );
	s.Radius( ) = 2.0f;

	float a, b;
	s.Projection( a, b, Vec3( 1.0f, 0.0f, 0.0f ) );
	BOOST_CHECK_EQUAL( a, -1.0f );
	BOOST_CHECK_EQUAL( b, 3.0f );

	s.Projection( a, b, Vec3( 1.0f ) );
	BOOST_CHECK_EQUAL( a, 4.0f );
	BOOST_CHECK_EQUAL( b, 8.0f );

	// check triangle
	Primitives::Triangle tri( Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ), Vec3( 0.0f, 0.0f, 1.0f ) );
	m.Reset( Vec3( 1.0f, 2.0f, 3.0f ), Euler( 0, Math::pi / 2, 0 ), Vec3( 1.0f, 2.0f, 3.0f ) );
	tri.Transform( m );
	BOOST_CHECK_LT( ( tri.vertices[ 0 ] - Vec3( 2.0f, 2.0f, 3.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( tri.vertices[ 1 ] - Vec3( 1.0f, 2.0f, 6.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( tri.vertices[ 2 ] - Vec3( 1.0f, 0.0f, 3.0f ) ).LenSq( ), gMathEpsSq );

	tri.Projection( a, b, Vec3( 6.0f, -3.0f, 2.0f ) );
	BOOST_CHECK_EQUAL( a, b );
	tri.Projection( a, b, Vec3( 1.0f, 0.0f, 0.0f ) );
	BOOST_CHECK_EQUAL( a, 1.0f );
	BOOST_CHECK_EQUAL( b, 2.0f );

	// check AABB
	Primitives::AABB aabb( tri );
	BOOST_CHECK_LT( ( aabb.Min( ) - Vec3( 1.0f, 0.0f, 3.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( aabb.Max( ) - Vec3( 2.0f, 2.0f, 6.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( aabb.Center( ) - Vec3( 1.5f, 1.0f, 4.5f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( aabb.Size( ) - Vec3( 1.0f, 2.0f, 3.0f ) ).LenSq( ), gMathEpsSq );

	Vec3 v = aabb.RandomPoint( );
	BOOST_CHECK_GT( v.x, aabb.Min( ).x );
	BOOST_CHECK_LT( v.x, aabb.Max( ).x );
	BOOST_CHECK_GT( v.y, aabb.Min( ).y );
	BOOST_CHECK_LT( v.y, aabb.Max( ).y );
	BOOST_CHECK_GT( v.z, aabb.Min( ).z );
	BOOST_CHECK_LT( v.z, aabb.Max( ).z );

	aabb.Shift( Vec3( -1.0f, -2.0f, -3.0f ) );
	BOOST_CHECK_LT( ( aabb.Min( ) - Vec3( 0.0f, -2.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( aabb.Max( ) - Vec3( 1.0f, 0.0f, 3.0f ) ).LenSq( ), gMathEpsSq );
	aabb.Enlarge( tri );
	BOOST_CHECK_LT( ( aabb.Min( ) - Vec3( 0.0f, -2.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( aabb.Max( ) - Vec3( 2.0f, 2.0f, 6.0f ) ).LenSq( ), gMathEpsSq );
	aabb.Enlarge( Vec3( -1.0f ) );
	BOOST_CHECK_LT( ( aabb.Min( ) - Vec3( -1.0f, -2.0f, -1.0f ) ).LenSq( ), gMathEpsSq );
	aabb.EnlargeSize( Vec3( 1.0f ) );
	BOOST_CHECK_LT( ( aabb.Min( ) - Vec3( -2.0f, -3.0f, -2.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( aabb.Max( ) - Vec3( 3.0f, 3.0f, 7.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK( !aabb.IsInside( aabb.Min( ) ) );
	BOOST_CHECK( aabb.IsInside( Vec3( 0.0f ) ) );
	BOOST_CHECK( aabb.IsInside( Vec3( 1.0f, 2.0f, 6.999f ) ) );
	Primitives::AABB aabb2( aabb );

	aabb2.Projection( a, b, Vec3( 1.0f, 0.0f, 0.0 ) );
	BOOST_CHECK_LT( fabs( a + 2.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( b - 3.0f ), gMathEps );
	aabb2.Projection( a, b, Vec3( 1.0f ) );
	BOOST_CHECK_LT( fabs( a + 7.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( b - 13.0f ), gMathEps );
	aabb2 = Primitives::AABB::Null( );
	BOOST_CHECK( !aabb2.IsInside( Vec3( 0.0f ) ) );
	BOOST_CHECK( !aabb2.IsInside( Vec3::Rand( ) ) );

	// check OBB
	Primitives::OBB obb;
	obb = Primitives::OBB( Vec3( -1.0f, -2.0f, -3.0f ), Vec3( 3.0f, 2.0f, 1.0f ) );
	BOOST_CHECK_LT( ( obb.Location( ) - Vec3( -1.0f, -2.0f, -3.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E1( ) - Vec3( 4.0f, 0.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E2( ) - Vec3( 0.0f, 4.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E3( ) - Vec3( 0.0f, 0.0f, 4.0f ) ).LenSq( ), gMathEpsSq );
	obb.From( tri );
	BOOST_CHECK_LT( ( obb.Location( ) - Vec3( 1.0f, 0.0f, 3.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E1( ) - Vec3( 1.0f, 0.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E2( ) - Vec3( 0.0f, 2.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E3( ) - Vec3( 0.0f, 0.0f, 3.0f ) ).LenSq( ), gMathEpsSq );
	obb.From( aabb );
	BOOST_CHECK_LT( ( obb.Location( ) - Vec3( -2.0f, -3.0f, -2.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E1( ) - Vec3( 5.0f, 0.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E2( ) - Vec3( 0.0f, 6.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.E3( ) - Vec3( 0.0f, 0.0f, 9.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( obb.Center( ) - Vec3( 0.5f, 0.0f, 2.5f ) ).LenSq( ), gMathEpsSq );

	obb.Transform( Mat4( Vec3( 1.0f, 2.0f, 3.0f ), Euler( 0, 0, Math::pi / 2 ), Vec3( 1.0f, 2.0f, 3.0f ) ) );
	BOOST_CHECK_LT( ( obb.Center( ) - Vec3( 1.0f, 3.0f, 10.5f ) ).LenSq( ), gMathEpsSq );
	aabb = obb.EnlargeToAABB( );
	BOOST_CHECK_LT( ( aabb.Min( ) - Vec3( -2.0f, -2.0f, -3.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK_LT( ( aabb.Max( ) - Vec3( 4.0f, 8.0f, 24.0f ) ).LenSq( ), gMathEpsSq );
	obb.Projection( a, b, Vec3( 1.0f ) );
	BOOST_CHECK_LT( fabs( a + 7.0f ), gMathEps );
	BOOST_CHECK_LT( fabs( b - 36.0f ), gMathEps );

	// check Plane
	Primitives::Plane plane( 1.0f, 2.0f, 3.0f, 4.0f );
	// plane origin is (-4/14, -8/14, -12/14)
	BOOST_CHECK_LT( ( plane.Normal( ) - Vec3( 1.0f, 2.0f, 3.0f ) ).LenSq( ), gMathEpsSq );
	plane.Transform( Mat4( 1.0f ) );
	BOOST_CHECK_GT( Vec3::Dot( plane.Normal( ), Vec3( 1.0f, 2.0f, 3.0f ) ), 0.0f );
	m.Reset( Vec3( 1.0f, 2.0f, 3.0f ), Euler( 0, 0, Math::pi / 2 ), Vec3( 1.0f, 2.0f, 3.0f ) );
	plane.Transform( m );
	// plane origin now should be (22/14, 20/14, 6/14)
	BOOST_CHECK_GT( Vec3::Dot( plane.Normal( ), m.TransformNormal( Vec3( 1.0f, 2.0f, 3.0f ) ) ), 0.0f );
	BOOST_CHECK_LT( fabs( plane.D( ) - 0.87123173f ), gMathEps );
	plane.Projection( a, b, Vec3( -4.0f, 1.0f, 2.0f ) );
	BOOST_CHECK_EQUAL( a, b );
	BOOST_CHECK_LT( fabs( a + plane.D( ) * Vec3::Dot( Vec3( -4.0f, 1.0f, 2.0f ), plane.Normal( ) ) ) * 0.5f, gMathEps );
	plane.Projection( a, b, Vec3( 1.0f ) );
	BOOST_CHECK_EQUAL( a, -FLT_MAX );
	BOOST_CHECK_EQUAL( b, FLT_MAX );

	// check triangles TestIntersection
	Primitives::Triangle tri1( Vec3( 0.0f, 0.0f, 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ) );
	Primitives::Triangle tri2( Vec3( 1.0f, 1.0f, 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ) );

	BOOST_CHECK( Primitives::TestIntersection( tri1, tri2 ) );
	tri2.vertices[ 0 ].xyz( 0.0f, 0.5f, -1.0f );
	tri2.vertices[ 2 ].z = 1.0f;
	BOOST_CHECK( Primitives::TestIntersection( tri1, tri2 ) );
	tri2.Transform( Mat4( Vec3( 1.0f, 0.0f, 0.0f ), Euler( 0 ), Vec3( 1.0f ) ) );
	BOOST_CHECK( !Primitives::TestIntersection( tri1, tri2 ) );

	// check AABB-AABB intersection
	Primitives::AABB aabb1( Vec3( 0.0f ), Vec3( 1.0f ) );
	aabb2.From( Vec3( 1.0f ), Vec3( 2.0f ) );
	BOOST_CHECK( Primitives::TestIntersection( aabb1, aabb2 ) );
	aabb2.Shift( Vec3( -0.5f ) );
	BOOST_CHECK( Primitives::TestIntersection( aabb1, aabb2 ) );
	aabb2.Shift( Vec3( 1.5f, 0.0f, 0.0f ) );
	BOOST_CHECK( !Primitives::TestIntersection( aabb1, aabb2 ) );
	aabb2 = Primitives::AABB::Null( );
	BOOST_CHECK( !Primitives::TestIntersection( aabb1, aabb2 ) );

	// check OBB-OBB intersection
	Primitives::OBB obb1( Vec3( 0.0f ), Vec3( 1.0f, 1.0f, 0.0f ), Vec3( 0.0f, 1.0f, 1.0f ), Vec3( 1.0f, 0.0f, 1.0f ) );
	Primitives::OBB obb2( Vec3( 0.5f ), Vec3( 1.0f ) );
	BOOST_CHECK( Primitives::TestIntersection( obb1, obb2 ) );
	obb2.From( Vec3( 0.0f ), Vec3( -1.0f ) );
	BOOST_CHECK( Primitives::TestIntersection( obb1, obb2 ) );
	obb2.Location( ) -= Vec3( 0.001f );
	BOOST_CHECK( !Primitives::TestIntersection( obb1, obb2 ) );

	// check AABB-triangle intersection
	BOOST_CHECK( Primitives::TestIntersection( aabb1, tri2 ) );
	//BOOST_CHECK( !Primitives::TestIntersection( aabb2, tri2 ) );

	// check AABB-OBB intersection
	BOOST_CHECK( Primitives::TestIntersection( aabb1, obb1 ) );

	// check Ray intersections
	float t;
	Vec3 norm;
	BOOST_CHECK( Primitives::TestIntersection( t, norm, tri1, Ray( Vec3( 0.25f, 0.25f, -0.5f ), Vec3( 0.0f, 0.0f, 1.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 0.5f ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( 0.0f, 0.0f, 1.0f ) ).LenSq( ), gMathEps );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, tri1, Ray( Vec3( 0.5f, 0.5f, -0.5f ), Vec3( 0.0f, 0.0f, 1.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 0.5f ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( 0.0f, 0.0f, 1.0f ) ).LenSq( ), gMathEps );
	BOOST_CHECK( !Primitives::TestIntersection( t, norm, tri1, Ray( Vec3( -0.5f, 0.0f, 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );
	BOOST_CHECK( !Primitives::TestIntersection( t, norm, tri1, Ray( Vec3( -0.5f, 0.0f, 0.01f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );

	BOOST_CHECK( Primitives::TestIntersection( t, norm, aabb1, Ray( Vec3( 0.25f, 0.25f, -0.5f ), Vec3( 0.0f, 0.0f, 1.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 0.5f ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( 0.0f, 0.0f, -1.0f ) ).LenSq( ), gMathEps );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, aabb1, Ray( Vec3( -0.25f, 0.25f, 0.5f ), Vec3( 1.0f, 1.0f, 0.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 0.25f ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( -1.0f, 0.0f, 0.0f ) ).LenSq( ), gMathEps );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, aabb1, Ray( Vec3( -0.25f, 0.25f, 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 0.25f ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( -1.0f, 0.0f, 0.0f ) ).LenSq( ), gMathEps );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, aabb1, Ray( Vec3( 0.25f, 0.25f, 0.5f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 0.75f ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( 1.0f, 0.0f, 0.0f ) ).LenSq( ), gMathEps );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, aabb1, Ray( Vec3( -0.5f, 0.0f, 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 0.5f ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( -1.0f, 0.0f, 0.0f ) ).LenSq( ), gMathEps );
	BOOST_CHECK( !Primitives::TestIntersection( t, norm, aabb1, Ray( Vec3( 1.25f, 0.25f, 0.5f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );

	r.Origin( ).xyz( 0.0f, 0.0f, 0.0f );
	r.Direction( Vec3( 1.0f, 1.0f, 1.0f ) );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, s, r ) );
	BOOST_CHECK_LT( fabs( t - 1.0f / sqrtf( 3.0f ) * ( 6.0f - sqrtf( 6.0f ) ) ), gMathEps * 3.0f );
	Vec3 p = r.Point( 1.0f / sqrtf( 3.0f ) * ( 6.0f - sqrtf( 6.0f ) ) );
	BOOST_CHECK_LT( ( norm - ( p - Vec3( 1.0f, 2.0f, 3.0f ) ).GetNormalized( ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, s, Ray( Vec3( 0.0f, 0.0f, 3.0f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 1.0f ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( 0.0f, -1.0f, 0.0f ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, s, Ray( Vec3( 1.0f, 1.0f, 3.0f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - math_sqrt( 3.0f ) ), gMathEps );
	BOOST_CHECK_LT( ( norm - Vec3( math_sqrt( 3.0f ) * 0.5f, -0.5f, 0.0f ).GetNormalized( ) ).LenSq( ), gMathEpsSq );
	BOOST_CHECK( !Primitives::TestIntersection( t, norm, s, Ray( Vec3( 0.0f, 0.0f, 3.0f ), Vec3( 1.0f, -0.001f, 0.0f ) ) ) );

	plane.From( Vec3( 1.0f, 2.0f, 3.0f ), Vec3( -1.0f, 2.0f, 3.0f ) );
	BOOST_CHECK( !Primitives::TestIntersection( t, norm, plane, Ray( Vec3( 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ) ) ) );
	BOOST_CHECK( Primitives::TestIntersection( t, norm, plane, Ray( Vec3( 0.0f ), Vec3( -1.0f, 0.0f, 0.0f ) ) ) );
	BOOST_CHECK_LT( fabs( t - 12.0f ), gMathEps );
	BOOST_CHECK( !Primitives::TestIntersection( t, norm, plane, Ray( Vec3( 0.0f ), Vec3( 1.0f, 2.0f, -1.0f ) ) ) );
}

BOOST_AUTO_TEST_CASE( aabbtree )
{
	AABBTree aabbtree;
	BOOST_CHECK( aabbtree.RootAABB( ).Min( ) == Vec3( 0.0f ) );
	BOOST_CHECK( aabbtree.RootAABB( ).Max( ) == Vec3( 0.0f ) );

	Primitives::Triangle tris[ ] =
	{
		Primitives::Triangle( Vec3( 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ) ),
		Primitives::Triangle( Vec3( 1.0f, 1.0f, 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ) ),
		Primitives::Triangle( Vec3( 0.0f, 0.0f, 0.0f ), Vec3( 0.0f, 0.0f, 1.0f ), Vec3( 0.0f, 1.0f, 0.0f ) ),
		Primitives::Triangle( Vec3( 0.0f, 1.0f, 1.0f ), Vec3( 0.0f, 0.0f, 1.0f ), Vec3( 0.0f, 1.0f, 0.0f ) ),
		Primitives::Triangle( Vec3( 0.0f, 1.0f, 0.0f ), Vec3( 1.0f, 1.0f, 0.0f ), Vec3( 0.0f, 1.0f, 1.0f ) ),
		Primitives::Triangle( Vec3( 1.0f, 1.0f, 1.0f ), Vec3( 1.0f, 1.0f, 0.0f ), Vec3( 0.0f, 1.0f, 1.0f ) ),
		Primitives::Triangle( Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 1.0f, 1.0f, 0.0f ), Vec3( 1.0f, 1.0f, 1.0f ) ),
		Primitives::Triangle( Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 1.0f, 0.0f, 1.0f ), Vec3( 1.0f, 1.0f, 1.0f ) ),
		Primitives::Triangle( Vec3( 0.0f, 0.0f, 1.0f ), Vec3( 1.0f, 0.0f, 1.0f ), Vec3( 0.0f, 1.0f, 1.0f ) ),
		Primitives::Triangle( Vec3( 1.0f, 1.0f, 1.0f ), Vec3( 1.0f, 0.0f, 1.0f ), Vec3( 0.0f, 1.0f, 1.0f ) ),
		Primitives::Triangle( Vec3( 0.0f, 0.0f, 0.0f ), Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, 0.0f, 1.0f ) ),
		Primitives::Triangle( Vec3( 1.0f, 0.0f, 1.0f ), Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 1.0f, 0.0f, 1.0f ) )
	};

	aabbtree.Build( &tris[ 0 ], &tris[ sizeof( tris ) / sizeof( Primitives::Triangle ) ] );

	Primitives::Triangle tri( Vec3( -1.0f, 0.5f, 0.5f ), Vec3( 0.5f, 0.25f, 0.5f ), Vec3( 0.25f, 0.5f, 0.75f ) );
	BOOST_CHECK( aabbtree.TestIntersection( tri ) );
}

BOOST_AUTO_TEST_SUITE_END( )

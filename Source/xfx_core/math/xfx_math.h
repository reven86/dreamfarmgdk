//  File xfx_math.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include <math.h>
#include <stdlib.h>

/*!	\file xfx_math.h 
 *	\brief %Math functions.
 */

_XFX_BEGIN





/*! \class Math xfx_math.h "math/xfx_math.h"
 *	\brief %Math support.
 *	\ingroup MathGroup
 *	
 *	\author Andrew "RevEn" Karpushin
 */

class Math : public Singleton< Math >
{
	friend class Singleton< Math >;

public:
	//! %Angle type. Support values [-PI; PI].
	typedef signed short	Angle;

	//! Big angle type. Support values [-65536 * PI; 65536 * PI].
	typedef signed int		BigAngle;

	//! PI constant.
	static const Angle pi = 32767;

	//! 1.0 / PI.
	static const float inv_pi;

	//! 1 rad to Angle.
	static const float rad;

	//! 1.0 / rad.
	static const float inv_rad;

	//! 1 deg to Angle.
	static const float deg;

	//! 1.0 / deg.
	static const float inv_deg;

	//! 3.1415...
	static const float real_pi;

	//! 1.0 / 3.1415...
	static const float real_inv_pi;

private:
	float mCos[ 65536 ];
	float mSin[ 65536 ];

protected:
	inline Math					();
	~Math						() {};

public:
	//! Constant time cosine.
	float			Cos					( BigAngle ang ) const { return mCos[ static_cast< unsigned short >( static_cast< Angle >( ang ) + 32768 ) ]; };

	//! Constant time sine.
	float			Sin					( BigAngle ang ) const { return mSin[ static_cast< unsigned short >( static_cast< Angle >( ang ) + 32768 ) ]; };
};




//
// Functions
//

//! Calculate table cosine.
inline float math_cos( Math::BigAngle ang ) { return Math::Instance( ).Cos( ang ); };

//! Calculate table sine.
inline float math_sin( Math::BigAngle ang ) { return Math::Instance( ).Sin( ang ); };

//! Normalize angle, transform it to range [-Pi; Pi].
inline Math::Angle math_normalize_angle( Math::BigAngle ang ) { return static_cast< Math::Angle >( ang ); };

//! Lerp angle.
inline Math::BigAngle math_lerp_angle( const Math::BigAngle& u, const Math::BigAngle& v, const float& t ) { return static_cast< Math::BigAngle >( t * v + ( 1.0f - t ) * u ); };

//! Slerp angle.
inline Math::Angle math_slerp_angle( const Math::Angle& u, const Math::Angle& v, const float& t ) { return static_cast< Math::Angle >( u + math_normalize_angle( v - u ) * t ); };

//! Degree to angle.
inline Math::BigAngle math_deg2angle( const float& d ) { return static_cast< Math::BigAngle >( Math::deg * d ); };

//! Radians to angle.
inline Math::BigAngle math_rad2angle( const float& r ) { return static_cast< Math::BigAngle >( Math::rad * r ); };

//! Angle to degree.
inline float math_angle2deg( const Math::BigAngle& ang ) { return Math::inv_deg * ang; };

//! Angle to radians.
inline float math_angle2rad( const Math::BigAngle& ang ) { return Math::inv_rad * ang; };

//! Logarithm time arccosine.
inline Math::Angle math_acos( float c )
{
	Math::Angle a = 0;
	Math::Angle b = Math::pi;

	while( b - a > 1 )
	{
		Math::Angle m = ( a + b ) >> 1;

		if( math_cos( m ) == c )
			return m;

		if( math_cos( m ) < c )
			b = m;
		else
			a = m;
	}

	return a;
}

//! Logarithm time arcsine.
inline Math::Angle math_asin( float s )
{
	Math::Angle a = -Math::pi / 2;
	Math::Angle b = Math::pi / 2;

	while( b - a > 1 )
	{
		Math::Angle m = ( a + b ) >> 1;

		if( math_sin( m ) == s )
			return m;

		if( math_sin( m ) > s )
			b = m;
		else
			a = m;
	}

	return b;
}

//! Fast \f$1/\sqrt{x}\f$.
inline float math_invsqrt( float x )
{
	const float _0_47 = 0.47f;
	const float _1_47 = 1.47f;
	unsigned long y;
	_asm
	{
		mov     eax, 07F000000h+03F800000h // (ONE_AS_INTEGER<<1) + ONE_AS_INTEGER
		sub     eax, x
		sar     eax, 1

		mov     y, eax                      // y
		fld		_0_47						// 0.47
		fmul    dword ptr x                 // x*0.47

		fld     dword ptr y
		fld     st(0)                       // y y x*0.47
		fmul    st(0), st(1)                // y*y y x*0.47

		fld		_1_47						// 1.47 y*y y x*0.47
		fxch    st(3)                       // x*0.47 y*y y 1.47
		fmulp   st(1), st(0)                // x*0.47*y*y y 1.47
		fsubp   st(2), st(0)                // y 1.47-x*0.47*y*y
		fmulp   st(1), st(0)                // result
		fstp    y
		and     y, 07FFFFFFFh

	}
	return *( float * )&y;
}

//! Fast \f$\sqrt{x}\f$.
inline float math_sqrt( float x )
{
	float r = math_invsqrt(x);
	return x * ( 3.0f - x * ( r * r ) ) * r * 0.5f; // remove for low accuracy
}

//! Get random float value [0, 1).
inline float math_frand( ) { return static_cast< float >( rand( ) ) / RAND_MAX; };





//! Parse Math::Angle variable (converts from degree to Math::Angle).
HRESULT	ParseAngleVariable ( Math::Angle& var, String::size_type& pos, const String& str );

//! Parse Math::BigAngle variable (converts from degree to Math::BigAngle).
HRESULT ParseBigAngleVariable ( Math::BigAngle& var, String::size_type& pos, const String& str );





//-----------------------------------------------------------------------------
//                           Methods
//-----------------------------------------------------------------------------
inline Math::Math ()
{
	for (int i = -32768; i < 32768; i++)
	{
		mCos[i + 32768] = cosf ((float) i * real_pi * inv_pi);
		mSin[i + 32768] = sinf ((float) i * real_pi * inv_pi);
	}
}



_XFX_END
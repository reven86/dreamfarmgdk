//  File xfx_argb.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_argb.h 
 *	\brief Defines class for color representation without any overhead from DWORD type.
 */

_XFX_BEGIN





#pragma warning (push)
#pragma warning (disable:4201) // nameless struct/union

/*! \union ARGB xfx_argb.h "utility/xfx_argb.h"
 *	\brief Color class.
 *	\ingroup UtilityGroup
 *	
 *	Represent color as dword value. Each color component can be accessed by members a, r, g, b
 *	provided as union with dword. Also this class has methods and operator for color 
 *	processing like color interpolation.
 *
 *	\author Andrew "RevEn" Karpushin
 */

union ARGB
{
	//! DWORD rerpesentation.
	boost::uint32_t dword;

	struct
	{
		boost::uint8_t b;			//!< Blue component.
		boost::uint8_t g;			//!< Green component.
		boost::uint8_t r;			//!< Red component.
		boost::uint8_t a;			//!< Alpha component.
	};

	//! Constructs an uninitialized color.
	ARGB					( ) { };

	//! Constructs color by dword.
	ARGB					( boost::uint32_t dw ) : dword( dw ) { };

	//! Constructs color by each color component.
	ARGB					( unsigned char a0, unsigned char r0, unsigned char g0, unsigned char b0 ) : r( r0 ), g( g0 ), b( b0 ), a( a0 ) { };

	/*!	\brief Color interpolation.
	 *	\param[in] col1		First color.
	 *	\param[in] col2		Second color.
	 *	\param[in] t		Interpolation value.
	 */
	static inline ARGB Interpolate		( const ARGB& col1, const ARGB& col2, float t )
	{
		// avoiding floats
		int it = static_cast< int >( 256 * t );

		return ARGB(
			static_cast< boost::uint8_t >( ( it * ( col2.a - col1.a ) + 256 * col1.a ) >> 0x08 ),
			static_cast< boost::uint8_t >( ( it * ( col2.r - col1.r ) + 256 * col1.r ) >> 0x08 ),
			static_cast< boost::uint8_t >( ( it * ( col2.g - col1.g ) + 256 * col1.g ) >> 0x08 ),
			static_cast< boost::uint8_t >( ( it * ( col2.b - col1.b ) + 256 * col1.b ) >> 0x08 )
			);		
	};
};

#pragma warning (pop)

//! Parse ARGB variable.
template< class _String >
HRESULT ParseVariable( ARGB& var, typename _String::size_type& pos, const _String& str )
{
	skip_comments( str, pos );
	unsigned fvar;
	Script< _String >::ParseVariable< unsigned >( fvar, pos, str );

	var = ARGB( fvar );
	return S_OK;
};




// operators

//! Equality color operator.
inline bool operator ==		( const ARGB& u, const ARGB& v ) { return u.dword == v.dword; };

//! Inequality color operator.
inline bool operator !=		( const ARGB& u, const ARGB& v ) { return u.dword != v.dword; };

//! Less color operator.
inline bool operator <		( const ARGB& u, const ARGB& v ) { return u.dword < v.dword; };

//! Greater color operator.
inline bool operator >		( const ARGB& u, const ARGB& v ) { return u.dword > v.dword; };

//! Color multiply operator.
inline ARGB operator *		( const ARGB& u, const ARGB& v )
{
	return ARGB (
		static_cast< unsigned char >( static_cast< boost::uint32_t >( u.a * ( v.a + 1 ) ) >> 0x08 ),
		static_cast< unsigned char >( static_cast< boost::uint32_t >( u.r * ( v.r + 1 ) ) >> 0x08 ),
		static_cast< unsigned char >( static_cast< boost::uint32_t >( u.g * ( v.g + 1 ) ) >> 0x08 ),
		static_cast< unsigned char >( static_cast< boost::uint32_t >( u.b * ( v.b + 1 ) ) >> 0x08 )
		);
};

//! Color addition operator.
inline ARGB operator +		( const ARGB& u, const ARGB& v )
{
	return ARGB(
		static_cast< unsigned char >( std::min< int >( u.a + v.a, 255 ) ),
		static_cast< unsigned char >( std::min< int >( u.r + v.r, 255 ) ),
		static_cast< unsigned char >( std::min< int >( u.g + v.g, 255 ) ),
		static_cast< unsigned char >( std::min< int >( u.b + v.b, 255 ) )
		);
};

//! Color substraction operator.
inline ARGB operator -		( const ARGB& u, const ARGB& v )
{
	return ARGB(
		static_cast< unsigned char >( std::max< int >( u.a - v.a, 0 ) ),
		static_cast< unsigned char >( std::max< int >( u.r - v.r, 0 ) ),
		static_cast< unsigned char >( std::max< int >( u.g - v.g, 0 ) ),
		static_cast< unsigned char >( std::max< int >( u.b - v.b, 0 ) )
		);
};

//! Color multiply operator by value.
inline ARGB operator *		( const ARGB& u, const float& t )
{
	int it = static_cast< int >( 256 * t );

	return ( it < 0 ) ? ARGB( 0x00000000 ) : ARGB(
		static_cast< boost::uint8_t >( std::min< int >( u.a * it, 255 * 256 ) >> 0x08 ),
		static_cast< boost::uint8_t >( std::min< int >( u.r * it, 255 * 256 ) >> 0x08 ),
		static_cast< boost::uint8_t >( std::min< int >( u.g * it, 255 * 256 ) >> 0x08 ),
		static_cast< boost::uint8_t >( std::min< int >( u.b * it, 255 * 256 ) >> 0x08 )
		);
};



/*!	\brief Simple lerp functor for ARGB.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

template< >
struct SimpleLerpFn< ARGB >
{
	ARGB lerp ( const ARGB& a, const ARGB& b, const float& t ) { return ARGB::Interpolate( a, b, t ); };
};



_XFX_END

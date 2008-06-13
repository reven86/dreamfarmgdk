//  File xfx_utils.h                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_utils.h 
 *	\brief Utility functions.
 */

_XFX_BEGIN





//
// IUnknown deleter
//

/*! \defgroup UtilityGroup Utility classes and functions.
 */

/*! \struct IUnknownDeleter xfx_utils.h "utility/xfx_utils.h"
 *	\brief Helper functor to release IUnknown objects. Passed to smart pointers.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

struct IUnknownDeleter : public std::unary_function< interface IUnknown *, void >
{
	void operator ( )			( IUnknown * u ) { _ASSERTE( u ); if( u ) u->Release( ); };
};



//
// Null deleter
//

/*! \struct NullDeleter xfx_utils.h "utility/xfx_utils.h"
 *	\brief Helper functor to pass to smart pointers.
 *	\ingroup UtilityGroup
 *
 *	Allow construct smart pointers for local variables.
 *
 *	\author Andrew "RevEn" Karpushin
 */

template< class _Type >
struct NullDeleter : public std::unary_function< _Type *, void>
{
	void operator ( )			( _Type * u ) { _ASSERTE( u ); };
};



//
// Hash compare
//

/*! \struct HashCompare xfx_utils.h "utility/xfx_utils.h"
 *	\brief Helper functor to compare two objects by hash value.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

template< class _Type >
struct HashCompare : public stdext::hash_compare< _Type, std::less< _Type > >
{
	using stdext::hash_compare< _Type, std::less< _Type > >::operator ();
	size_t operator ( )					( const _Type& a ) const { return boost::hash_value( a ); };
};




//
// Vertex/Index buffer locker
//

/*! \class BufferLocker xfx_utils.h "utility/xfx_utils.h"
 *	\brief Helper class, provides support for locking/unlocking vertex and index buffer.
 *	\ingroup UtilityGroup
 *
 *	\see VertexBuffer, IndexBuffer.
 *	\author Andrew "RevEn" Karpushin
 */

template< class _Buffer >
class BufferLocker
{
private:
	_Buffer& owner;
	void * buf;

public:
	//! Lock buffer in constructor.
	BufferLocker			( _Buffer& b, const unsigned& offset, const unsigned& count, const bool& discard_if_dynamic = true ) : owner( b ) { buf = owner.Lock( offset, count, discard_if_dynamic ); };

	//! Unlock buffer in destructor.
	~BufferLocker			( ) { owner.Unlock( ); };

	//! Get buffer data.
	template< class T >
	T * data				( ) { return reinterpret_cast< T * >( buf ); };
};




//
// Functions
//

/*! \brief Simple factory function.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

template< class _Base, class _Derived >
inline boost::shared_ptr< _Base > SimpleFactoryFn( ) { return boost::shared_ptr< _Base >( new _Derived( ) ); };




/*! \struct SimpleLerpFn xfx_utils.h "utility/xfx_utils.h"
 *	\brief Simple lerp functor.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

template< class _Type >
struct SimpleLerpFn
{
	_Type lerp( const _Type& a, const _Type& b, const float& t ) { return static_cast< _Type >( a + ( b - a ) * t ); };
};



/*!	\brief Simple lerp functor for ARGB.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*
template< >
struct SimpleLerpFn< ARGB >
{
	ARGB lerp ( const ARGB& a, const ARGB& b, const float& t ) { return ARGB::Interpolate( a, b, t ); };
};
*/




_XFX_END



#ifdef __XFX_USE_BOOST_SERIALIZATION__

namespace boost
{
namespace serialization
{

/*! \brief Serialization helpful function to save std::bitset.
 */
template< class _Archive, int _N >
void serialize( _Archive & ar, std::bitset< _N >& t, const unsigned int version )
{
	if( _Archive::is_saving::value )
	{
		int bit = 0;
		unsigned char byte = 0;
		for( std::size_t i = 0; i < t.size( ); i++ )
		{
			if( t.test( i ) )
				byte |= 1 << bit;

			if( bit == 7 || i == t.size( ) - 1 )
			{
				ar & BOOST_SERIALIZATION_NVP( byte );
				byte = 0;
				bit = 0;
			}
			else
				bit++;
		}
	}
	else
	{
		std::size_t byte_count = ( t.size( ) >> 3 );

		if( ( t.size( ) & 7 ) != 0 )
			byte_count++;

		std::size_t set_bit = 0;
		for( std::size_t i = 0; i < byte_count; i++ )
		{
			unsigned char byte;
			ar & BOOST_SERIALIZATION_NVP( byte );

			for( int bit = 0; bit < 8 && set_bit < t.size( ); bit++, set_bit++ )
			{
				t.set( set_bit, byte & ( 1 << bit ) );
			}
		}
	}
};



/*! \brief Serialization helpful function to save boost::array.
 */
template< class _Archive, class _T, int _N >
void serialize( _Archive & ar, boost::array< _T, _N >& t, const unsigned int version )
{
	for( int i = 0; i < _N; i++ )
		ar & boost::serialization::make_nvp( "item", t[ i ] );
};


/*! \brief Serialization helpful function to save boost::tuple< T0 >.
 */
template< class _Archive, class _T0 >
void serialize( _Archive & ar, boost::tuple< _T0 >& t, const unsigned int version )
{
	ar & boost::serialization::make_nvp( "t0", t.get< 0 >( ) );
};


/*! \brief Serialization helpful function to save boost::tuple< T0, T1 >.
 */
template< class _Archive, class _T0, class _T1 >
void serialize( _Archive & ar, boost::tuple< _T0, _T1 >& t, const unsigned int version )
{
	ar & boost::serialization::make_nvp( "t0", t.get< 0 >( ) );
	ar & boost::serialization::make_nvp( "t1", t.get< 1 >( ) );
};


/*! \brief Serialization helpful function to save boost::tuple< T0, T1, T2 >.
 */
template< class _Archive, class _T0, class _T1, class _T2 >
void serialize( _Archive & ar, boost::tuple< _T0, _T1, _T2 >& t, const unsigned int version )
{
	ar & boost::serialization::make_nvp( "t0", t.get< 0 >( ) );
	ar & boost::serialization::make_nvp( "t1", t.get< 1 >( ) );
	ar & boost::serialization::make_nvp( "t2", t.get< 2 >( ) );
};


/*! \brief Serialization helpful function to save boost::tuple< T0, T1, T2, T3 >.
 */
template< class _Archive, class _T0, class _T1, class _T2, class _T3 >
void serialize( _Archive & ar, boost::tuple< _T0, _T1, _T2, _T3 >& t, const unsigned int version )
{
	ar & boost::serialization::make_nvp( "t0", t.get< 0 >( ) );
	ar & boost::serialization::make_nvp( "t1", t.get< 1 >( ) );
	ar & boost::serialization::make_nvp( "t2", t.get< 2 >( ) );
	ar & boost::serialization::make_nvp( "t3", t.get< 3 >( ) );
};


/*! \brief Serialization helpful function to save boost::tuple< T0, T1, T2, T3, T4 >.
 */
template< class _Archive, class _T0, class _T1, class _T2, class _T3, class _T4 >
void serialize( _Archive & ar, boost::tuple< _T0, _T1, _T2, _T3, _T4 >& t, const unsigned int version )
{
	ar & boost::serialization::make_nvp( "t0", t.get< 0 >( ) );
	ar & boost::serialization::make_nvp( "t1", t.get< 1 >( ) );
	ar & boost::serialization::make_nvp( "t2", t.get< 2 >( ) );
	ar & boost::serialization::make_nvp( "t3", t.get< 3 >( ) );
	ar & boost::serialization::make_nvp( "t4", t.get< 4 >( ) );
};


/*! \brief Serialization helpful function to save boost::tuple< T0, T1, T2, T3, T4, T5 >.
 */
template< class _Archive, class _T0, class _T1, class _T2, class _T3, class _T4, class _T5 >
void serialize( _Archive & ar, boost::tuple< _T0, _T1, _T2, _T3, _T4, _T5 >& t, const unsigned int version )
{
	ar & boost::serialization::make_nvp( "t0", t.get< 0 >( ) );
	ar & boost::serialization::make_nvp( "t1", t.get< 1 >( ) );
	ar & boost::serialization::make_nvp( "t2", t.get< 2 >( ) );
	ar & boost::serialization::make_nvp( "t3", t.get< 3 >( ) );
	ar & boost::serialization::make_nvp( "t4", t.get< 4 >( ) );
	ar & boost::serialization::make_nvp( "t5", t.get< 5 >( ) );
};


/*! \brief Serialization helpful function to save boost::tuple< T0, T1, T2, T3, T4, T5, T6 >.
 */
template< class _Archive, class _T0, class _T1, class _T2, class _T3, class _T4, class _T5, class _T6 >
void serialize( _Archive & ar, boost::tuple< _T0, _T1, _T2, _T3, _T4, _T5, _T6 >& t, const unsigned int version )
{
	ar & boost::serialization::make_nvp( "t0", t.get< 0 >( ) );
	ar & boost::serialization::make_nvp( "t1", t.get< 1 >( ) );
	ar & boost::serialization::make_nvp( "t2", t.get< 2 >( ) );
	ar & boost::serialization::make_nvp( "t3", t.get< 3 >( ) );
	ar & boost::serialization::make_nvp( "t4", t.get< 4 >( ) );
	ar & boost::serialization::make_nvp( "t5", t.get< 5 >( ) );
	ar & boost::serialization::make_nvp( "t6", t.get< 6 >( ) );
};


/*! \brief Serialization helpful function to save boost::tuple< T0, T1, T2, T3, T4, T5, T6, T7 >.
 */
template< class _Archive, class _T0, class _T1, class _T2, class _T3, class _T4, class _T5, class _T6, class _T7 >
void serialize( _Archive & ar, boost::tuple< _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7 >& t, const unsigned int version )
{
	ar & boost::serialization::make_nvp( "t0", t.get< 0 >( ) );
	ar & boost::serialization::make_nvp( "t1", t.get< 1 >( ) );
	ar & boost::serialization::make_nvp( "t2", t.get< 2 >( ) );
	ar & boost::serialization::make_nvp( "t3", t.get< 3 >( ) );
	ar & boost::serialization::make_nvp( "t4", t.get< 4 >( ) );
	ar & boost::serialization::make_nvp( "t5", t.get< 5 >( ) );
	ar & boost::serialization::make_nvp( "t6", t.get< 6 >( ) );
	ar & boost::serialization::make_nvp( "t7", t.get< 7 >( ) );
};




/*! \brief Serialization helpful function to save resources from Cache, like boost::shared_ptr< const Shader >.
 */
template< class _Archive, class _T >
void serialize( _Archive & ar, boost::shared_ptr< const _T >& t, const unsigned int version )
{
	if( _Archive::is_saving::value )
	{
		xfx::String file = t ? t->Filename( ) : "";
		ar & BOOST_SERIALIZATION_NVP( file );
	}
	else
	{
		xfx::String file;
		ar & BOOST_SERIALIZATION_NVP( file );

		t = _T::Cache( ).Register( file );
	}
}



} // serialization
} // boost


#endif



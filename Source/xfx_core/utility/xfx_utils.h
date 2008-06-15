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

struct IUnknownDeleter : public std::unary_function< IUnknown *, void >
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





_XFX_END

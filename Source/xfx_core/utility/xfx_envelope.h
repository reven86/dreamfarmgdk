//  File xfx_envelope.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_envelope.h 
 *	\brief %Envelope template class.
 */

_XFX_BEGIN





/*! \class Envelope xfx_envelope.h "utility/xfx_envelope.h"
 *	\brief %Envelope class.
 *	\ingroup UtilityGroup
 *	
 *	%Envelope class is a container that assign custom values with 
 *	decimal keys. %Envelope provides methods to get a value between keys.
 *	Is this case value is linear interpolated.
 *
 *	Template class is specialezed by value type T, key type _KT and 
 *	lerp functor _LerpFn which by default is SimpleLerpFn< T >
 *
 *	%Envelope file syntax can be found here: \ref envelope_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section envelope_script Envelope script syntax
 *
 *	Example:
 *
 *	\verbatim
{
	0			:		0
	32			:		10
	255			:		0
}
	\endverbatim
 *
 *	Description:
 *
 *	First column	-	keys.\n
 *	Second column	-	values.
 *
 *	@}
 */

template< class T, class _KT = boost::uint8_t, class _LerpFn = SimpleLerpFn< T > >
class Envelope
{
	friend class boost::serialization::access;

	typedef std::pair< _KT, T >		KeyType;
	typedef std::vector< KeyType/*, boost::pool_allocator<KeyType> */> KeysType;

	KeysType						mKeys;
	T								mEmptyKey;

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	//! Constructs an empty envelope.
	Envelope					( ) { };

	//! Destructs envelope.
	~Envelope					( ) { };

	/*! \brief Add new key and value associated with this key.
	 *
	 *	\note Keys should be added in ascending order.
	 *	
	 *	\param[in] t		Key.
	 *	\param[in] key		Value.
	 */
	void AddKey					( const _KT& t, const T& key )
	{
		_ASSERTE( mKeys.empty () || ( t > mKeys.back( ).first ) );
		if( mKeys.empty( ) || ( t > mKeys.back( ).first ) )
			mKeys.push_back( KeyType( t, key ) );
	};

	//! Gets a value, associated with key.
	T Key						( const _KT& t ) const
	{
		if( mKeys.empty( ) )
			return mEmptyKey;

		struct key_comp : public std::binary_function< bool, KeyType, KeyType >
		{
			bool operator( )		( const KeyType& a, const KeyType& b ) const
			{
				return a.first < b.first;
			};
		};

		KeysType::const_iterator it2 = std::lower_bound(
			mKeys.begin( ),
			mKeys.end( ),
			KeyType( t, mEmptyKey ),
			key_comp( )
			);

		if( it2 != mKeys.end( ) )
		{
			if( it2 == mKeys.begin( ) )
				return (*it2).second;

			KeysType::const_iterator it = boost::prior( it2 );

			float lerp_frac = static_cast< float >( t - ( *it ).first ) / ( ( *it2 ).first - ( *it ).first );

			return _LerpFn( ).lerp( ( *it ).second, ( *it2 ).second, lerp_frac );
		}

		return mKeys.back( ).second;
	};

	//! Makes an envelope empty.
	void Clear					( ) { KeysType( ).swap( mKeys ); };

	/*! \brief Parse envelope from string.
	 *
	 *	\param[in,out]	env			Result envelope.
	 *	\param[in,out]	pos			Start search position.s
	 *	\param[in]		str			Parsed string.
	 *
	 *	\return S_OK if parsing has been succeeded.
	 */

	static HRESULT					ParseEnvelope				( Envelope< T, _KT, _LerpFn >& env, String::size_type& pos, const String& str )
	{
		env.Clear( );

		String token;
		Script< String >::ParseVariable( token, pos, str );

		if( token != "{" )
			return XFXERR_INVALIDCALL;

		while( !token.empty( ) && !test_token( str, pos, "}" ) )
		{
			_KT key;
			HRESULT hr = Script< String >::ParseVariable< _KT >( key, pos, str );

			if( FAILED( hr ) )
				return hr;

			Script< String >::ParseVariable( token, pos, str );

			if( token != ":" )
				return XFXERR_INVALIDCALL;

			T value;
			hr = Script< String >::ParseVariable< T >( value, pos, str );

			if( FAILED( hr ) )
				return hr;

			env.AddKey( key, value );
		}

		return S_OK;
	};
};





_XFX_END

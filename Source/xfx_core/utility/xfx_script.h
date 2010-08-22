//  File xfx_script.h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_script.h 
 *	\brief %Script, basic lexical parser.
 */

_XFX_BEGIN




template< class _Type >
struct HashCompare;



//! Script base delimiters.
extern const char _ScriptBaseDelimiters[ ];

/*! \class Script xfx_script.h "utility/xfx_script.h"
 *	\brief Lexical parser.
 *	\ingroup UtilityGroup
 *
 *	%Script class support word parsing and invokes user-defined functors on known tokens.
 *
 *	\author Andrew "RevEn" Karpushin
 */

template< class _String = String, const char * _Delimiters = _ScriptBaseDelimiters >
class Script
{
	typedef boost::function2< HRESULT, typename _String::size_type&, const _String& >	TokenFnType;
	typedef stdext::hash_map< _String, TokenFnType, HashCompare< _String > >		TokenMapType;

	TokenMapType				mTokenMap;

	bool						mIsParsingStopped;

public:
	Script												( ) { };
	virtual ~Script										( ) { };

	//! Parse string.
	HRESULT						Parse					( const _String& str ) { typename _String::size_type pos = 0; return ParseAt( pos, str ); };

	//! Parse string at specified position.
	HRESULT						ParseAt					( typename _String::size_type& pos, const _String& str )
	{
		PROFILE( "xfx::Script::ParseAt", "General" );

		HRESULT hr;

		mIsParsingStopped = false;
		if( FAILED( hr = PrepareParsing( ) ) )
			return hr;

		while( pos < str.size( ) && !mIsParsingStopped )
			if( FAILED( hr = ParseToken( pos, str ) ) )
			{
				ParsingFailed( pos, str );
				return hr;
			}

		return AfterParsing( );
	};

	//
	// Useful functions
	//

	//! Parse useless token.
	static HRESULT				ParseUselessToken		( typename _String::size_type&, const _String& )
	{
		return S_OK;
	};

	//! Parse custom type variable.
	template< class _Type >
	static HRESULT				ParseVariable			( _Type&, typename _String::size_type&, const _String& )
	{
		_ASSERTE( !"invalid type passed to specialize ParseVariable" );
		return XFXERR_INVALIDCALL;
	};

#ifndef __GCCXML__

	//! Parse float variable.
	template< >
	static HRESULT				ParseVariable			( float& var, typename _String::size_type& pos, const _String& str )
	{
		skip_comments( str, pos );
		var = static_cast< float >( atof( next_token( str, pos ).c_str( ) ) );
		return S_OK;
	};

	//! Parse int variable.
	template< >
	static HRESULT				ParseVariable			( int& var, typename _String::size_type& pos, const _String& str )
	{
		skip_comments( str, pos );
		_String::value_type * ch;
		var = static_cast< int >( strtol( next_token( str, pos ).c_str( ), &ch, 0 ) );
		return S_OK;
	};

	//! Parse bool variable.
	template< >
	static HRESULT				ParseVariable			( bool& var, typename _String::size_type& pos, const _String& str )
	{
		skip_comments( str, pos );
		_String::value_type * ch;
		var = static_cast< bool >( strtol( next_token( str, pos ).c_str( ), &ch, 0 ) );
		return S_OK;
	};

	//! Parse unsigned char variable.
	template< >
	static HRESULT				ParseVariable			( unsigned char& var, typename _String::size_type& pos, const _String& str )
	{
		skip_comments( str, pos );
		_String::value_type * ch;
		var = static_cast< unsigned char >( strtol( next_token( str, pos ).c_str( ), &ch, 0 ) );
		return S_OK;
	};

	//! Parse DWORD variable.
	template< >
	static HRESULT				ParseVariable			( boost::uint32_t& var, typename _String::size_type& pos, const _String& str )
	{
		skip_comments( str, pos );
		_String::value_type * ch;
		var = strtoul( next_token( str, pos ).c_str( ), &ch, 0 );
		return S_OK;
	};

	//! Parse unsigned variable.
	template< >
	static HRESULT				ParseVariable			( unsigned& var, typename _String::size_type& pos, const _String& str )
	{
		skip_comments( str, pos );
		_String::value_type * ch;
		var = static_cast< unsigned >( strtoul( next_token( str, pos ).c_str( ), &ch, 0 ) );
		return S_OK;
	};

	//! Parse String or WString variable.
	template< >
	static HRESULT				ParseVariable			( _String& var, typename _String::size_type& pos, const _String& str )
	{
		skip_comments( str, pos );
		var = next_token( str, pos );
		return S_OK;
	};

#endif

protected:
	//! Parse end token.
	HRESULT						ParseEndToken			( typename _String::size_type&, const _String& )
	{
		StopParsing( );
		return S_OK;
	};

	/*! \brief Register new token and associated functor.
	 *
	 *	\param	tok		Token.
	 *	\param	fn		Functor.
	 */
	void						AddToken				( const _String& tok, const TokenFnType& fn ) { mTokenMap.insert( TokenMapType::value_type( tok, fn ) ); };

	//! Prepare parsing event.
	virtual HRESULT				PrepareParsing			( ) { return S_OK; };	//before parsing

	//! After succeeded parsing event.
	virtual	HRESULT				AfterParsing			( ) { return S_OK; };	//after succeeded parsing

	//! Failed parsing event.
	virtual void				ParsingFailed			( typename _String::size_type& pos, const _String& str )
	{
		gError( "Parsing script failed near token %s, line %d", next_token( str, pos ).c_str( ), std::count( str.begin( ), str.begin( ) + pos, '\n' ) + 1 );
	};

	//! Is parsing stopped.
	bool						IsParsingStopped		( ) const { return mIsParsingStopped; };

	//! Stop parsing.
	void						StopParsing				( ) { mIsParsingStopped = true; };

	//! Resume parsing.
	void						ResumeParsing			( ) { mIsParsingStopped = false; };

private:
	HRESULT						ParseToken				( typename _String::size_type& pos, const _String& str )
	{
		skip_comments( str, pos, _Delimiters );

		_String tok = next_token( str, pos, _Delimiters );

		typename TokenMapType::const_iterator it = mTokenMap.find( tok );

		return ( it != mTokenMap.end( ) ) ? ( *it ).second( boost::ref( pos ), boost::cref( str ) ) : XFXERR_NOTFOUND;
	};
};




_XFX_END

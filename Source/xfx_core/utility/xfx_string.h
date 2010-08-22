//  File xfx_string.h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#define NOMINMAX
#include <windows.h>

/*!	\file xfx_string.h 
 *	\brief %String functions.
 */

_XFX_BEGIN





//! String typedef.
typedef std::basic_string<char/*, std::char_traits<char>, boost::pool_allocator<char> */> String;

//! WString typedef.
typedef std::basic_string<wchar_t/*, std::char_traits<wchar_t>, boost::pool_allocator<wchar_t> */> WString;

//! String format typedef.
typedef boost::basic_format<char/*, std::char_traits<char>, boost::pool_allocator<char> */> StringFormat;

//! WString format typedef.
typedef boost::basic_format<wchar_t/*, std::char_traits<wchar_t>, boost::pool_allocator<wchar_t> */> WStringFormat;

/*! \brief Get next token in string.
 *	\ingroup Globals
 *	
 *	Search next token in string, starting from specified position.
 *	Can search through strings in '"'. Support '\\\\' and '\\"' escape symbols.
 *
 *	\param[in]		str			Incoming string.
 *	\param[in,out]	s			Start search position.
 *	\param[in]		delimiters	Default delimiters.
 *
 *	\return Next token found or empty string.
 */
template< class _String >
_String next_token( const _String& str, typename _String::size_type& s, const char * delimiters = " \n\t\r" )
{
	typename _String::size_type e;
	s = str.find_first_not_of( delimiters, s );

	if( s != _String::npos )
	{
		if( str[ s ] == '\"' )
		{
			s++;

			_String result;

			// search for escape symbols: '\\' '\"'

			e = s;

			typename _String::size_type str_size = str.size( );

			while( e < str_size && str[ e ] != '\"' )
			{
				if( str[ e ] == '\\' && e + 1 < str_size )
				{
					// found escape symbol, recognize it
					switch( str[ e + 1 ] )
					{
					default:
						break;

					case '\\':
					case '\"':
						e++;
						break;
					}
				}

				result += str[ e ];

				e++;
			}

			s = ( e == _String::npos ) ? str.size( ) : e + 1;

			return result;
		}

		e = str.find_first_of( delimiters, s );

		typename _String::size_type olds = s;
		s = ( e == _String::npos ) ? str.size( ) : e + 1;

		return ( e == _String::npos ) ? str.substr( olds ) : str.substr( olds, e - olds );
	}

	s = str.size( );

	return _String( "" );
};




/*! \brief Skip white spaces.
 *	\ingroup Globals
 *	
 *	Skip white spaces in string.
 *
 *	\param[in]		str			Incoming string.
 *	\param[in,out]	pos			Start search position.
 *	\param[in]		delimiters	Default delimiters.
 *
 *	\return Next found non-whitespace symbol position in s.
 */

template< class _String >
inline void skip_whitespaces( const _String& str, typename _String::size_type& pos, const char * delimiters = " \n\t\r" )
{
	pos = str.find_first_not_of( delimiters, pos );

	if( pos == _String::npos )
		pos = str.size( );
};



/*! \brief Skip C++ style comments.
 *	\ingroup Globals
 *	
 *	Skip comments like '//' and '/*'.
 *
 *	\param[in]		str			Incoming string.
 *	\param[in,out]	pos			Start search position.
 *	\param[in]		delimiters	Default delimiters.
 *
 *	\return Next found uncommented symbol is pos.
 */

template< class _String >
void skip_comments( const _String& str, typename _String::size_type& pos, const char * delimiters = " \n\t\r" )
{
	for( ; ; )
	{
		skip_whitespaces( str, pos, delimiters );

		if( pos > str.size () - 2 )
			break;

		if( str[ pos ] == '/' )
		{
			if( str[ pos + 1 ] == '/' )
			{
				pos = str.find_first_of( "\n\r", pos + 2 );

				if( pos == _String::npos )
					pos = str.size( );
				else
					pos++;

				continue;
			}
			else if( str[ pos + 1 ] == '*' )
			{
				pos = str.find( "*/", pos + 2 );

				if( pos == _String::npos )
					pos = str.size( );
				else
					pos += 2;

				continue;
			}
		}

		break;
	}
};


/*! \brief Test next token.
 *	\ingroup Globals
 *	
 *	Test next token in string by equality for specified string.
 *
 *	\param[in]		str			Incoming string.
 *	\param[in,out]	pos			Start search position.
 *	\param[in]		test		Test string.
 *	\param[in]		delimiters	Default delimiters.
 *
 *	\return If test succeeded returns \b true and modify pos.
 */

template< class _String, class _TestString >
bool test_token( const _String& str, typename _String::size_type& pos, const _TestString& test, const char * delimiters = " \n\t\r" )
{
	skip_comments( str, pos, delimiters );

	if( boost::algorithm::starts_with( boost::make_iterator_range( str.begin( ) + pos, str.end( ) ), test ) )
	{
		pos += test.size( );
		return true;
	}

	return false;
};


/*! \brief Test next token.
 *	\ingroup Globals
 *	
 *	Test next token in string by equality for specified string.
 *
 *	\param[in]		str			Incoming string.
 *	\param[in,out]	pos			Start search position.
 *	\param[in]		test		Test string.
 *	\param[in]		delimiters	Default delimiters.
 *
 *	\return If test succeeded returns \b true and modify pos.
 */

template< class _String >
bool test_token( const _String& str, typename _String::size_type& pos, const char * test, const char * delimiters = " \n\t\r" )
{
	skip_comments( str, pos, delimiters );

	if( boost::algorithm::starts_with( boost::make_iterator_range( str.begin( ) + pos, str.end( ) ), test ) )
	{
		pos += strlen( test );
		return true;
	}

	return false;
};



/*! \brief Convert UTF8 string to WString.
 *	\ingroup Globals
 *	
 *	\param[in]		str			Incoming string.
 *
 *	\return Converted WString.
 */

inline WString fromUTF8( const String& str )
{
	boost::scoped_array< wchar_t > out( new wchar_t[ str.size( ) + 1 ] );

	MultiByteToWideChar( CP_UTF8, 0, str.c_str( ), -1, out.get( ), static_cast< int >( str.size( ) + 1 ) );
	return WString( out.get( ) );
};


/*! \brief Convert MBCS string to WString.
 *	\ingroup Globals
 *	
 *	\param[in]		str			Incoming string.
 *
 *	\return Converted WString.
 */

inline WString fromMBCS( const String& str )
{
	boost::scoped_array< wchar_t > out( new wchar_t[ str.size( ) + 1 ] );

	MultiByteToWideChar( CP_ACP, 0, str.c_str( ), -1, out.get( ), static_cast< int >( str.size( ) + 1 ) );
	return WString( out.get( ) );
};



/*! \brief Convert WString string to MBCS.
 *	\ingroup Globals
 *	
 *	\param[in]		str			Incoming string.
 *
 *	\return Converted MBCS string.
 */

inline String toMBCS( const WString& str )
{
	boost::scoped_array< char > out( new char[ str.size( ) + 1 ] );

	WideCharToMultiByte( CP_ACP, 0, str.c_str( ), -1, out.get( ), static_cast< int >( str.size( ) + 1 ), NULL, NULL );
	return String( out.get( ) );
};




_XFX_END

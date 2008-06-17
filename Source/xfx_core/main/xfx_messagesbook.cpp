//  File xfx_messagesbook.cpp                                                 |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_messagesbook.h"

_XFX_BEGIN





//
// MessagesBook
//

MessagesBook::MessagesBook( ) : ScriptResource( "MessagesBook" )
{
	AddToken( "messages_book",				&ParseUselessToken );
	AddToken( "{",							&ParseUselessToken );
	AddToken( "entry",						boost::bind( &MessagesBook::ParseEntry, this, _1, _2 ) );
	AddToken( "}",							boost::bind( &MessagesBook::ParseEndToken, this, _1, _2 ) );
}

MessagesBook::~MessagesBook( )
{
}

const MessagesBook::Entry& MessagesBook::GetEntry( const String& id ) const
{
	static Entry empty_entry( L"no_entry" );

	EntriesType::const_iterator it = mEntries.find( id );

	return( it == mEntries.end( ) ) ? empty_entry : ( *it ).second;
}

HRESULT MessagesBook::PrepareParsing( )
{
	Clear( );

	return S_OK;
}

HRESULT MessagesBook::ParseEntry( String::size_type& pos, const String& str )
{
	HRESULT hr;

	String id;

	if( FAILED( hr = ParseVariable( id, pos, str ) ) )
		return hr;

	String tok;

	for( ; ; )
	{
		if( FAILED( hr = ParseVariable( tok, pos, str ) ) )
			return hr;

		if( tok == "}" )
			break;

		if( tok == "{" )
			continue;

		if( tok == "text" )
		{
			if( FAILED( hr = ParseVariable( tok, pos, str ) ) )
				return hr;

			mEntries.insert( std::make_pair( id, Entry( fromUTF8( tok ) ) ) );
		}
	}

	return S_OK;
}




//
// BooksManager
//

void BooksManager::Clear( )
{
	std::for_each(
		mBooks.begin( ),
		mBooks.end( ),
		std::mem_fun_ref( &MessagesBook::Clear )
		);
}

bool BooksManager::IsEntryExists( const String& id ) const
{
	BOOST_FOREACH( const MessagesBook& b, mBooks )
	{
		if( b.IsEntryExists( id ) )
			return true;
	}

	return false;
}

const MessagesBook::Entry& BooksManager::GetEntry( const String& id ) const
{
	static MessagesBook::Entry empty_entry( L"no_entry" );

	BOOST_FOREACH( const MessagesBook& b, mBooks )
	{
		if( b.IsEntryExists( id ) )
			return b.GetEntry( id );
	}

	return empty_entry;
}

const MessagesBook::Entry& BooksManager::GetEntry( const char * id ) const
{
	return GetEntry( String( id ) );
}



_XFX_END
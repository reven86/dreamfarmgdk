//  File xfx_messagesbook.h                                                   |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_fs.h"

/*!	\file xfx_messagesbook.h 
 *	\brief Language-specific text resources.
 *
 *	Provides text resources storage.
 */


_XFX_BEGIN





/*! \class MessagesBook xfx_messagesbook.h "main/xfx_messagesbook.h"
 *	\brief Text resource storage.
 *	\ingroup MainGroup
 *
 *	Stores localizable text resources in one logical storage.
 *	Book stores resouces in hash map and each language-specific resources contains its tech name.
 *
 *	MessagesBook file syntax can be found at section \ref messagesbook_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section messagesbook_script MessagesBook file syntax
 *
 *	Example:
 *
 *	\verbatim
messages_book
{
	entry SYS_gameover
	{
		text		"Here is Game Over messege"
	}

	// other entries...
}
	\endverbatim
 *	
 *	Description:
 *
 *	\b entry		-	Describes a book entry. Following the entry keywork there is tech name. \n
 *	\b text			-	Entry text in UTF8 format.
 *
 *	\see MessagesBook, BooksManager.
 *
 *	@}
 */

class MessagesBook : public ScriptResource
{
public:
	//! Entry type.
	struct Entry
	{
		WString		text;

		Entry		( ) { };
		Entry		( const WString& t ) : text( t ) { };
	};

private:
	typedef stdext::hash_map< String, Entry, HashCompare< String > >	EntriesType;
	EntriesType															mEntries;

public:
	//! Constructs empty book.
	MessagesBook													( );

	//! Destructor.
	virtual ~MessagesBook											( );

	//! Clears all entries in book.
	void							Clear							( ) { mEntries.clear( ); };

	/*! \brief Test is entry exists.
	 *
	 *	\param[in]			id		Entry tech name.
	 *
	 *	\return \b true if entry with \a id found.
	 */
	bool							IsEntryExists					( const String& id ) const { return( mEntries.find( id ) != mEntries.end( ) ); };

	/*! \brief Get book entry by its tech name.
	 *
	 *	\param[in]			id		Entry tech name.
	 *
	 *	\return Book entry if entry with \a id found or empty entry otherwise.
	 */
	const Entry&					GetEntry						( const String& id ) const;

private:
	virtual HRESULT					PrepareParsing					( );

	HRESULT							ParseEntry						( String::size_type& pos, const String& str );
};





/*! \class BooksManager xfx_messagesbook.h "utility/xfx_messagesbook.h"
 *	\brief Holds collection of MessagesBooks.
 *	\ingroup UtilityGroup
 *
 *	General interface of collection of MessagesBook.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class BooksManager : public Singleton< BooksManager >
{
	friend class Singleton< BooksManager >;

	typedef std::list< MessagesBook >			BooksType;

public:
	typedef BooksType::iterator					BookID;

private:
	BooksType									mBooks;

private:
	BooksManager												( ) { };

public:
	virtual ~BooksManager										( ) { };

	/*! \brief Add a \b copy of book to BooksManager.
	 *
	 *	\param[in]			book		Book to add.
	 *
	 *	\return BookID that can be used to access or remove book from BooksManager.
	 */
	BookID						AddBook							( const MessagesBook& book ) { return mBooks.insert( mBooks.end( ), book ); };

	/*! \brief Remove book from BooksManager.
	 *
	 *	\param[in]			book_id		Book id returned by AddBook.
	 *
	 *	\see AddBook.
	 */
	void						RemoveBook						( const BookID& book_id ) { mBooks.erase( book_id ); };

	/*! \brief Return reference to MessagesBook by id.
	 *
	 *	\param[in]			book_id		Book id returned by AddBook.
	 *
	 *	\remark book_id must be valid before calling this method. No validation take place in this method.
	 *
	 *	\see AddBook.
	 */
	MessagesBook&				GetBook							( const BookID& book_id ) { return *book_id; };

	/*! \brief Clears all entries in registered books.
	 */
	void						Clear							( );

	/*! \brief Test is entry exists.
	 *
	 *	\param[in]			id		Entry tech name.
	 *
	 *	\return \b true if entry with \a id found.
	 */
	bool						IsEntryExists					( const String& id ) const;

	/*! \brief Get book entry by its tech name.
	 *
	 *	\param[in]			id		Entry tech name.
	 *
	 *	\return Book entry if entry with \a id found or empty entry otherwise.
	 */
	const MessagesBook::Entry&	GetEntry						( const String& id ) const;

	/*! \brief Get book entry by its tech name.
	 *
	 *	\param[in]			id		Entry tech name.
	 *
	 *	\return Book entry if entry with \a id found or empty entry otherwise.
	 */
	const MessagesBook::Entry&	GetEntry						( const char * id ) const;
};



_XFX_END

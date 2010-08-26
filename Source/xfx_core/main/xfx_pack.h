//  File xfx_pack.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_fs.h"
#include <io.h>

/*!	\file xfx_pack.h 
 *	\brief Pack files interface.
 */

_XFX_BEGIN






/*! \class Pack xfx_pack.h "fs/xfx_pack.h"
 *	\brief Pack files class.
 *	\ingroup FSGroup
 *
 *	Pack stores group of files with directory hierarchy in one packed or unpacked file.
 *	Noncopyable to avoid error-prone copy code.
 *	
 *	\author Andrew "RevEn" Karpushin
 */

class Pack : boost::noncopyable, public Resource
{
public:
	//! File iterator type.
	struct FileIterator_t{ };

	typedef const FileIterator_t * FileIterator;

private:
	struct FileInfo;

	//! Files are stored in lexicographical order.
	boost::scoped_array< FileInfo >			mFileStore;

	//! Total files count.
	unsigned								mFilesCount;

	//! Is pack opened from memory pointer.
	bool									mIsMemory;

	//! Pack file handle.
	FILE									* mPackFile;

	//! Memory pointer.
	const BYTE								* mpPackMemory;

	enum { VERSION = 1 };

public:
	/*! \brief Constuctor. Creates empty files storage.
	 */
	Pack															( );

	//! Destructor. Releases all allocated resources.
	virtual ~Pack													( );

	//! Get first file.
	FileIterator						GetFirstFile				( ) const;

	//! Get file iterator behind last.
	FileIterator						GetLastFile					( ) const;

	//! Get total files count.
	unsigned							GetFilesCount				( ) const;

	/*! \brief Search file in pack by name.
	 *
	 *	\param[in] file		File name.
	 *
	 *	\note Wildcards are not supported.
	 *	\note Complexity is O( log n ).
	 *
	 *	\return FileIterator corrensponding to found file or GetLastFile( ) if file was not found.
	 */
	FileIterator						FindFile					( const String& file ) const;

	/*! \brief Get file name.
	 *
	 *	\param[in]	file	File iterator.
	 *	\param[out]	name	Filename.
	 *
	 *	\return
	 *	- S_OK if file was found.
	 *	- XFXERR_NOTFOUND otherwise.
	 */
	HRESULT								GetFileName					( const FileIterator& file, String& name ) const;

	/*! \brief Read file contents.
	 *
	 *	\param[in] file		File iterator.
	 *	\param[out] buf		Buffer where to hold file data.
	 *
	 *	\note Buffer must be large enough to hold data.
	 *
	 *	\return
	 *	- S_OK if file was found.
	 *	- XFXERR_NOTFOUND otherwise.
	 */
	HRESULT								ReadFile					( const FileIterator& file, void * buf ) const;

	/*! \brief Get file size in bytes.
	 *
	 *	\param[in] file		File iterator.
	 *	\param[out] len		File length in bytes.
	 *
	 *	\return
	 *	- S_OK if file was found.
	 *	- XFXERR_NOTFOUND otherwise.
	 */
	HRESULT								GetFileSize					( const FileIterator& file, unsigned long& len ) const;

	/*! \brief Get file offset from pack file beginning.
	 *
	 *	\param[in] file		File iterator.
	 *	\param[out] ofs		File offset.
	 *
	 *	\return
	 *	- S_OK if file was found.
	 *	- XFXERR_NOTFOUND otherwise.
	 */
	HRESULT								GetFileOffset				( const FileIterator& file, unsigned long& ofs ) const;

	/*! \brief Write file.
	 *
	 *	\param[in] file		File name.
	 *	\param[in] buf		Buffer to write.
	 *	\param[in] len		Length of the buffer, in bytes.
	 *
	 *	\note Currently not implemented.
	 *
	 *	\return
	 *	- XFXERR_IO file can't be opened for writing.
	 */
	HRESULT								WriteFile					( const String& file, const void * buf, unsigned long len ) const { file; buf; len; return XFXERR_IO; };

	/*! \brief Create pack file from directory structure.
	 *
	 *	Creates new pack file named \a pack_file_name from \a directory_path and assign it with \b this.
	 *
	 *	\param[in]	directory_path		Directory path on disk.
	 *	\param[in]	pack_file_name		Pack file name.
	 *
	 *	\note If \a pack_file_name is exists, it will be rewritten.
	 *	\note Skips hidden files and folders.
	 *
	 *	\return
	 *	- S_OK if pack has been created successfully.
	 *	- XFXERR_OUTOFMEMORY if not enough memory.
	 *	- XFXERR_IO otherwise.
	 */
	HRESULT								CreateFromDirectory			( const String& directory_path, const String& pack_file_name );

	/*! \brief Open and assign pack file.
	 *
	 *	Open and read pack file header.
	 *
	 *	\param[in]	pack_file_name		Pack file name.
	 *
	 *	\return
	 *	- S_OK if operation was successful.
	 *	- XFXERR_OUTOFMEMORY if not enough memory.
	 *	- XFXERR_NOTFOUND if \a pack_file_name was not found.
	 */
	virtual HRESULT						LoadFile					( const String& pack_file_name );

	/*! \brief Load pack from memory.
	 *
	 *	\note \a pmemory should be valid memory address for all operations on memory pack.
	 */
	virtual HRESULT						LoadMemory					( const void * pmemory, unsigned long filelen );

	//! Close opened pack file.
	void								ClosePack					( );

private:
	template< class _OutputIterator >
	void								RecursiveScanDirectory		( const String& dir_name_with_slash, _OutputIterator out_it ) const;
};






//
// Template methods
//

template< class _OutputIterator >
void Pack::RecursiveScanDirectory( const String& dir_name_with_slash, _OutputIterator out_it ) const
{
	_finddata_t file_info;

	intptr_t hfile = _findfirst( ( dir_name_with_slash + "*.*" ).c_str( ), &file_info );

	if( hfile == -1 )
		return;

	do
	{
		if( file_info.attrib & _A_HIDDEN )
			continue;

		if( file_info.attrib & _A_SUBDIR )
		{
			if( !strcmp( file_info.name, "." ) || !strcmp( file_info.name, ".." ) )
				continue;

			RecursiveScanDirectory( dir_name_with_slash + file_info.name + "\\", out_it );
		}
		else
		{
			*out_it++ = std::make_pair( boost::algorithm::to_lower_copy( dir_name_with_slash + file_info.name ), file_info.size );
		}
	}
	while( _findnext( hfile, &file_info ) == 0 );

	_findclose( hfile );
}



_XFX_END

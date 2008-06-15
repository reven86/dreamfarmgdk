//  File xfx_fs.h                                                             |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_errorcodes.h"

/*!	\file xfx_fs.h 
 *	\brief File system.
 */

_XFX_BEGIN





/*! \class Resource xfx_fs.h "fs/xfx_fs.h"
 *	\brief Base resource class.
 *	\ingroup FSGroup
 *
 *	Base resources object which can be loaded.
 *	
 *	\author Andrew "RevEn" Karpushin
 */

class Resource
{
	String							mName;
	String							mFilename;

	friend class boost::serialization::access;

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	//! Constructs an empty resource by name.
	Resource													( const char * name ) : mName( name ) {};

	//! Destructs resource.
	virtual ~Resource											( ) { };

	//! Get resource name.
	const String&					Name						( ) const { return mName; };

	//! Get resource file name.
	const String&					Filename					( ) const { return mFilename; };

	//! Load resource from file.
	virtual HRESULT					LoadFile					( const String& filename );

	//! Load resource from memory.
	virtual HRESULT					LoadMemory					( const void *, unsigned long ) { return XFXERR_INVALIDCALL; };

protected:
	//! Get/Set resource file name.
	String&							rFilename					( ) { return mFilename; };
};





/*! \class ScriptResource xfx_script.h "utility/xfx_script.h"
 *	\brief %Resource that can be loaded from text script file.
 *	\ingroup FSGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \defgroup ScriptResourceGroup Scripted resources index.
 *	
 *	List of supported scipted resources.
 */

class ScriptResource : public Script< String >, public Resource
{
	friend class boost::serialization::access;

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	//! Constructs a resource with specified name.
	ScriptResource												( const char * name ) : Resource( name ), Script( ) { };

	//! Destructor.
	virtual ~ScriptResource										( ) { };

	//! Load script from memory.
	virtual HRESULT					LoadMemory					( const void * pmemory, unsigned long filelen )
	{
		const char * cmem = reinterpret_cast< const char * >( pmemory );

		// skip UTF-8
		if( filelen >= 3 &&
			*reinterpret_cast< const unsigned char * >( cmem + 0 ) == 0xef &&
			*reinterpret_cast< const unsigned char * >( cmem + 1 ) == 0xbb &&
			*reinterpret_cast< const unsigned char * >( cmem + 2 ) == 0xbf )
		{
			cmem += 3;
			filelen -= 3;
		}

		String file( cmem, filelen );

		return Parse( file );
	};
};




/*! \class FileSystem xfx_fs.h "fs/xfx_fs.h"
 *	\brief File system class.
 *	\ingroup FSGroup
 *
 *	File system class. Provides methods for file search, read and write operations.
 *	
 *	\note File system doesn't support Wide Char file names.
 *	\todo Implement wide char file names in FileSystem class.
 *	\author Andrew "RevEn" Karpushin
 */

class FileSystem : public Singleton< FileSystem >
{
	friend class Singleton< FileSystem >;

public:
	//! Seach path priority value.
	enum ESearchPathPriority
	{
		ESPP_LOW,						//!< Low search path priority
		ESPP_HIGH,						//!< High search path priority
	};

private:
	//! Bridge pattern implementation.
	class FileSystem_impl;

	std::auto_ptr< FileSystem_impl >	mImpl;

protected:
	FileSystem														( );
	virtual ~FileSystem												( );

public:
	/*! \brief Add new search path.
	 *
	 *	Add new search path.
	 *
	 *	\note By default one search path added on FileSystem initialization.
	 *	This search path is "", it cannot be deleted and has low priority.
	 *
	 *	\param[in]	path		New search path.
	 *	\param[in]	priority	Search path priority.
	 */
	void								AddSearchPath				( const String& path, const ESearchPathPriority& priority = ESPP_HIGH );

	/*! \brief Remove search path.
	 *
	 *	\note By default one search path added on FileSystem initialization.
	 *	This search path is "", it cannot be deleted and has low priority.
	 *	
	 *	\param[in]	path		New search path.
	 */
	void								RemoveSearchPath			( const String& path );

	/*! \brief Remove all search paths.
	 *
	 *	\note By default one search path added on FileSystem initialization.
	 *	This search path is "", it cannot be deleted and has low priority.
	 */
	void								RemoveAllSearchPaths		( );

	/*! \brief Add pack to file system
	 *
	 *	\param[in]		pack	Pack file shared pointer.
	 */
	void								AddPack						( const boost::shared_ptr< class Pack >& pack );

	/*! \brief Search file by name.
	 *
	 *	\param[in]	file		File name.
	 *	\param[out]	pack		Pack file where file was found, if not NULL.
	 *	\note Wildcards are not supported.
	 *
	 *	\return
	 *	- S_OK if file was found;
	 *	- XFXERR_NOTFOUND otherwise
	 */
	HRESULT								FindFile					( const String& file, boost::shared_ptr< class Pack > * pack = NULL ) const;

	/*! \brief Read file contents.
	 *
	 *	\param[in] file		File name.
	 *	\param[out] buf		Buffer where to hold file data.
	 *	\note Buffer must be large enough to hold data.
	 *
	 *	\return
	 *	- S_OK if file was found;
	 *	- XFXERR_NOTFOUND otherwise
	 */
	HRESULT								ReadFile					( const String& file, void * buf ) const;

	/*! \brief Get file size in bytes.
	 *
	 *	\param[in] file		File name.
	 *	\param[out] len		File length in bytes.
	 *
	 *	\return
	 *	- S_OK if file was found;
	 *	- XFXERR_NOTFOUND otherwise
	 */
	HRESULT								GetFileSize					( const String& file, unsigned long& len ) const;

	/*! \brief Write file.
	 *
	 *	\param[in] file		File name.
	 *	\param[in] buf		Buffer to write.
	 *	\param[in] len		Length of the buffer, in bytes.
	 *
	 *	\return
	 *	- S_OK file was found;
	 *	- XFXERR_NOTFOUND file not found;
	 *	- XFXERR_IO file can't be opened for writing.
	 */
	HRESULT								WriteFile					( const String& file, const void * buf, unsigned long len ) const;
};




_XFX_END

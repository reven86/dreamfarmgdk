//  File xfx_fs.cpp                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_fs.h"
#include "xfx_pack.h"
#include "utility\xfx_profiler.h"

_XFX_BEGIN





//
// Resource
//

HRESULT Resource::LoadFile( const String& file )
{
	PROFILE( __FUNCTION__, "General" );

	mFilename = file;
	gMess( "Loading %s from file \"%s\"...", mName.c_str( ), file.c_str( ) );

	unsigned long filesize;
	HRESULT hr;
	if( FAILED( hr = FileSystem::Instance( ).GetFileSize( file, filesize ) ) )
	{
		gError( "Can't open file \"%s\"", file.c_str( ) );
		return hr;
	}

	boost::scoped_array< BYTE > pbuf( new BYTE[ filesize ] );

	FileSystem::Instance( ).ReadFile( file, pbuf.get( ) );

	if( FAILED( hr = LoadMemory( pbuf.get( ), filesize ) ) )
		gError( "Loading %s from file \"%s\" failed!", mName.c_str( ), file.c_str( ) );

	return hr;
}





//
// File system implementation class definition.
//

class FileSystem::FileSystem_impl
{
	std::deque< String >						mSearchPaths;
	std::vector< boost::shared_ptr< Pack > >	mPacks;

	enum EOpenMode
	{
		EOM_READ		= ( 1 << 0 ),
		EOM_WRITE		= ( 1 << 1 ),
		EOM_READWRITE	= EOM_READ | EOM_WRITE,
	};

	class FileObjectProxyBase
	{
	public:
		FileObjectProxyBase											( ) { };
		virtual ~FileObjectProxyBase								( ) { };

		virtual boost::shared_ptr< Pack > GetPack					( ) const { return boost::shared_ptr< Pack >( ); };
		virtual HRESULT					ReadFile					( void * buf ) const = 0;
		virtual HRESULT					GetFileSize					( unsigned long& len ) const = 0;
		virtual HRESULT					WriteFile					( const void * buf, unsigned long len ) const = 0;
	};

	class DiskFileObjectProxy : public FileObjectProxyBase
	{
		boost::shared_ptr< FILE >		mFilePtr;

	public:
		DiskFileObjectProxy											( FILE * f ) : mFilePtr( f, boost::bind( &fclose, _1 ) ) { };
		virtual ~DiskFileObjectProxy								( ) { };

		virtual HRESULT					ReadFile					( void * buf ) const
		{
			fseek( mFilePtr.get( ), 0, SEEK_END );
			unsigned long len = ftell( mFilePtr.get( ) );

			fseek( mFilePtr.get( ), 0, SEEK_SET );
			fread( buf, len, 1, mFilePtr.get( ) );

			return S_OK;
		};

		virtual HRESULT					GetFileSize					( unsigned long& len ) const
		{
			fseek( mFilePtr.get( ), 0, SEEK_END );
			len = ftell( mFilePtr.get( ) );

			return S_OK;
		};

		virtual HRESULT					WriteFile					( const void * buf, unsigned long len ) const
		{
			fwrite( buf, len, 1, mFilePtr.get( ) );
			fclose( mFilePtr.get( ) );

			return S_OK;
		};
	};

	class PackFileObjectProxy : public FileObjectProxyBase
	{
		boost::shared_ptr< Pack >		mPackPtr;
		Pack::FileIterator				mFileIterator;

	public:
		PackFileObjectProxy											( const boost::shared_ptr< Pack >& p, const Pack::FileIterator& f ) : mPackPtr( p ), mFileIterator( f ) { _ASSERTE( mPackPtr ); };
		virtual ~PackFileObjectProxy								( ) { };

		virtual boost::shared_ptr< Pack > GetPack					( ) const { return mPackPtr; };

		virtual HRESULT					ReadFile					( void * buf ) const
		{
			return mPackPtr->ReadFile( mFileIterator, buf );
		};

		virtual HRESULT					GetFileSize					( unsigned long& len ) const
		{
			return mPackPtr->GetFileSize( mFileIterator, len );
		};

		virtual HRESULT					WriteFile					( const void *, unsigned long ) const
		{
			return XFXERR_INVALIDCALL;
		};
	};

public:
	FileSystem_impl													( )
	{
		mSearchPaths.push_back( "" );
	}

	~FileSystem_impl												( ) { };

	void								AddSearchPath				( const String& path, const ESearchPathPriority& priority = ESPP_HIGH );
	void								RemoveSearchPath			( const String& path );
	void								RemoveAllSearchPaths		( );

	void								AddPack						( const boost::shared_ptr< class Pack >& pack );
	void								RemoveAllPacks				( );

	HRESULT								FindFile					( const String& file, boost::shared_ptr< class Pack > * pack ) const;
	HRESULT								ReadFile					( const String& file, void * buf ) const;
	HRESULT								GetFileSize					( const String& file, unsigned long& len ) const;
	HRESULT								WriteFile					( const String& file, const void * buf, unsigned long len ) const;

private:
	boost::shared_ptr< FileObjectProxyBase >	GetFileObject		( const String& filename, const EOpenMode& mode ) const;
};




//
// File system implementation details
//

void FileSystem::FileSystem_impl::AddSearchPath( const String& path, const ESearchPathPriority& priority )
{
	if( priority == ESPP_HIGH )
	{
		mSearchPaths.push_front( path );
		gMess( "High priority search path has been added: %s", path.c_str( ) );
	}
	else
	{
		mSearchPaths.push_back( path );
		gMess( "Low priority search path has been added: %s", path.c_str( ) );
	}
}

void FileSystem::FileSystem_impl::RemoveSearchPath( const String& path )
{
	if( path.empty( ) )
		return;

	mSearchPaths.erase( std::remove( mSearchPaths.begin( ), mSearchPaths.end( ), path ), mSearchPaths.end( ) );
}

void FileSystem::FileSystem_impl::RemoveAllSearchPaths( )
{
	mSearchPaths.clear( );
	mSearchPaths.push_back( "" );
}

void FileSystem::FileSystem_impl::AddPack( const boost::shared_ptr< class Pack >& pack )
{
	mPacks.push_back( pack );
}

void FileSystem::FileSystem_impl::RemoveAllPacks( )
{
	mPacks.clear( );
}

boost::shared_ptr< FileSystem::FileSystem_impl::FileObjectProxyBase > FileSystem::FileSystem_impl::GetFileObject( const String& filename, const EOpenMode& mode ) const
{
	boost::shared_ptr< FileObjectProxyBase > res;

	const char * openmode;
	switch( mode )
	{
	case EOM_READ:
		openmode = "rb";
		break;

	case EOM_WRITE:
		openmode = "w+b";
		break;

	case EOM_READWRITE:
		openmode = "rw+b";
		break;

	default:
		_ASSERTE( !"Invalid open mode passed." );
		return res;
	}

	std::deque< String >::const_iterator sp_it;

	boost::shared_ptr< Pack > found_pack;
	Pack::FileIterator pack_file_it;

	for( sp_it = mSearchPaths.begin( );
		 sp_it != mSearchPaths.end( );
		 ++sp_it )
	{
		String new_file_name( *sp_it + filename );
		boost::algorithm::to_lower( new_file_name );

		FILE * f = fopen( new_file_name.c_str( ), openmode );

		if( f )
		{
			res.reset( new DiskFileObjectProxy( f ) );
			break;
		}
	}

	// if file was not found on disk, look for it in packs
	if( sp_it == mSearchPaths.end( ) )
		for( sp_it = mSearchPaths.begin( );
			 sp_it != mSearchPaths.end( ) && !found_pack;
			 ++sp_it )
		{
			String new_file_name( *sp_it + filename );
			boost::algorithm::to_lower( new_file_name );

			// search in packs
			for( std::vector< boost::shared_ptr< Pack > >::const_iterator p_it = mPacks.begin( );
				 p_it != mPacks.end( );
				 ++p_it )
			{
				Pack::FileIterator f_it = ( *p_it )->FindFile( new_file_name );

				if( ( *p_it ) && f_it != ( *p_it )->GetLastFile( ) )
				{
					found_pack = *p_it;
					pack_file_it = f_it;
					break;
				}
			}
		}

	if( !res && found_pack )
		res.reset( new PackFileObjectProxy( found_pack, pack_file_it ) );

	return res;
}

HRESULT FileSystem::FileSystem_impl::FindFile( const String& file, boost::shared_ptr< class Pack > * pack ) const
{
	boost::shared_ptr< FileObjectProxyBase > fileobj = GetFileObject( file, EOM_READ );

	if( fileobj.get( ) )
	{
		if( pack )
			*pack = fileobj->GetPack( );

		return S_OK;
	}

	return XFXERR_NOTFOUND;
}

HRESULT FileSystem::FileSystem_impl::ReadFile( const String& file, void * buf ) const
{
	boost::shared_ptr< FileObjectProxyBase > fileobj = GetFileObject( file, EOM_READ );

	if( fileobj )
	{
		return fileobj->ReadFile( buf );
	}

	return XFXERR_NOTFOUND;
}

HRESULT FileSystem::FileSystem_impl::GetFileSize( const String& file, unsigned long& len ) const
{
	boost::shared_ptr< FileObjectProxyBase > fileobj = GetFileObject( file, EOM_READ );

	if( fileobj )
	{
		return fileobj->GetFileSize( len );
	}

	return XFXERR_NOTFOUND;
}

HRESULT FileSystem::FileSystem_impl::WriteFile( const String& file, const void * buf, unsigned long len ) const
{
	boost::shared_ptr< FileObjectProxyBase > fileobj = GetFileObject( file, EOM_WRITE );

	if( fileobj )
	{
		return fileobj->WriteFile( buf, len );
	}

	return XFXERR_NOTFOUND;
}



//
// File system
//

FileSystem::FileSystem( )
{
	mImpl.reset( new FileSystem_impl( ) );
}

FileSystem::~FileSystem( )
{
}

void FileSystem::AddSearchPath( const String& path, const ESearchPathPriority& priority )
{
	mImpl->AddSearchPath( path, priority );
}

void FileSystem::RemoveSearchPath( const String& path )
{
	mImpl->RemoveSearchPath( path );
}

void FileSystem::RemoveAllSearchPaths( )
{
	mImpl->RemoveAllSearchPaths( );
}

void FileSystem::AddPack( const boost::shared_ptr< class Pack >& pack )
{
	mImpl->AddPack( pack );
}

void FileSystem::RemoveAllPacks( )
{
	mImpl->RemoveAllPacks( );
}

HRESULT FileSystem::FindFile( const String& file, boost::shared_ptr< class Pack > * pack ) const
{
	return mImpl->FindFile( file, pack );
}

HRESULT FileSystem::ReadFile (const String& file, void * buf) const
{
	return mImpl->ReadFile( file, buf );
}

HRESULT FileSystem::GetFileSize (const String& file, unsigned long& len) const
{
	return mImpl->GetFileSize( file, len );
}

HRESULT FileSystem::WriteFile (const String& file, const void * buf, unsigned long len) const
{
	_ASSERTE (buf);

	return mImpl->WriteFile( file, buf, len );
}




_XFX_END
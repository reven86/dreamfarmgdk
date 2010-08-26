//  File xfx_pack.cpp                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_pack.h"

_XFX_BEGIN






//
// Pack
//

struct Pack::FileInfo : Pack::FileIterator_t
{
	DWORD			offset;
	DWORD			length;
	String			filename;
};

#pragma message( "FIXME: use log for filesystem" )

Pack::Pack( ) :
	Resource( "Pack" ),
	mFilesCount( 0 ),
	mIsMemory( false ),
	mpPackMemory( NULL ),
	mPackFile( NULL )
{
}

Pack::~Pack( )
{
	if( mPackFile )
		fclose( mPackFile );
}

Pack::FileIterator Pack::GetFirstFile( ) const
{
	return mFileStore.get( );
}

Pack::FileIterator Pack::GetLastFile( ) const
{
	return mFileStore ? GetFirstFile( ) + mFilesCount : GetFirstFile( );
}

unsigned Pack::GetFilesCount( ) const
{
	return ( unsigned )std::distance( GetFirstFile( ), GetLastFile( ) );
}

Pack::FileIterator Pack::FindFile( const String& file ) const
{
	if( !mFileStore || !mFilesCount )
		return GetLastFile( );

	FileInfo wrap;
	wrap.filename = boost::algorithm::to_lower_copy( file );

	const FileInfo * it = std::lower_bound(
		( const FileInfo * ) GetFirstFile( ),
		( const FileInfo * ) GetLastFile( ),
		wrap,
		boost::bind( std::less< String >( ), boost::bind( &FileInfo::filename, _1 ), boost::bind( &FileInfo::filename, _2 ) )
		);

	return it != GetLastFile( ) && ( *it ).filename == wrap.filename ? it : GetLastFile( );
}

HRESULT Pack::GetFileName( const FileIterator& file, String& name ) const
{
	size_t dist = std::distance( GetFirstFile( ), file );

	if( dist >= mFilesCount ) 
		return XFXERR_NOTFOUND;

	name = ( ( const FileInfo& ) *file ).filename;

	return S_OK;
}

HRESULT Pack::ReadFile( const FileIterator& file, void * buf ) const
{
	size_t dist = std::distance( GetFirstFile( ), file );

	if( dist >= mFilesCount ) 
		return XFXERR_NOTFOUND;

	if( !mIsMemory )
	{
		if( mPackFile )
		{
			fseek( mPackFile, ( ( const FileInfo& ) *file ).offset, SEEK_SET );
			fread( buf, ( ( const FileInfo& ) *file ).length, 1, mPackFile );
		}
	}
	else
	{
		if( mpPackMemory )
		{
			memcpy( buf, mpPackMemory + ( ( const FileInfo& ) *file ).offset, ( ( const FileInfo& ) *file ).length );
		}
	}

	return S_OK;
}

HRESULT Pack::GetFileSize( const FileIterator& file, unsigned long& len ) const
{
	size_t dist = std::distance( GetFirstFile( ), file );

	if( dist >= mFilesCount ) 
		return XFXERR_NOTFOUND;

	len = ( ( const FileInfo& ) *file ).length;

	return S_OK;
}

HRESULT Pack::GetFileOffset( const FileIterator& file, unsigned long& ofs ) const
{
	size_t dist = std::distance( GetFirstFile( ), file );

	if( dist >= mFilesCount ) 
		return XFXERR_NOTFOUND;

	ofs = ( ( const FileInfo& ) *file ).offset;

	return S_OK;
}

HRESULT Pack::CreateFromDirectory( const String& directory_path, const String& pack_file_name )
{
	ClosePack( );

	mPackFile = fopen( pack_file_name.c_str( ), "w+b" );

	if( !mPackFile )
		return XFXERR_IO;

	String dir_with_slash( directory_path );

	if( !dir_with_slash.empty( ) && dir_with_slash[ dir_with_slash.size( ) - 1 ] != '\\' )
		dir_with_slash.push_back( '\\' );

	std::vector< std::pair< String, _fsize_t > > found_files;

	RecursiveScanDirectory( dir_with_slash, std::back_inserter( found_files ) );

	std::sort(
		found_files.begin( ),
		found_files.end( ),
		boost::bind( std::less< String >( ),
			boost::bind( &std::pair< String, _fsize_t >::first, _1 ),
			boost::bind( &std::pair< String, _fsize_t >::first, _2 ) )
		);

	// make relative paths from dir_with_slash
	std::vector< std::pair< String, _fsize_t > >::iterator f_it;
	for( f_it = found_files.begin( ); f_it != found_files.end( ); ++f_it )
		( *f_it ).first.erase( 0, dir_with_slash.size( ) );

	// compute start offset
	DWORD start_offset = 0;
	start_offset += sizeof( DWORD );	// magic
	start_offset += sizeof( DWORD );	// version
	start_offset += sizeof( DWORD );	// files count
	start_offset += static_cast< DWORD >( found_files.size( ) * ( sizeof( DWORD ) + sizeof( DWORD ) + sizeof( DWORD ) ) );

	for( f_it = found_files.begin( ); f_it != found_files.end( ); ++f_it )
		start_offset += static_cast< DWORD >( ( *f_it ).first.size( ) );

	struct add_fn : public std::unary_function< std::pair< String, _fsize_t >, FileInfo >
	{
		DWORD start_offset;

		add_fn			( DWORD ss ) : start_offset( ss ) { };

		FileInfo operator ( )				( const std::pair< String, _fsize_t >& val )
		{
			FileInfo res;
			res.filename = val.first;
			res.length = val.second;
			res.offset = start_offset;

			start_offset += res.length;

			return res;
		};
	};

	mFilesCount = static_cast< unsigned >( found_files.size( ) );
	mFileStore.reset( new FileInfo[ mFilesCount ] );

	if( !mFileStore )
		return XFXERR_OUTOFMEMORY;

	std::transform(
		found_files.begin( ),
		found_files.end( ),
		mFileStore.get( ),
		add_fn( start_offset )
		);

	//
	// Write pack file
	//

	// magic
	DWORD magic = MAKEFOURCC( 'P', 'A', 'C', 'K' );
	fwrite( &magic, sizeof( magic ), 1, mPackFile );

	// version
	DWORD version = Pack::VERSION;
	fwrite( &version, sizeof( version ), 1, mPackFile );

	// files count
	DWORD files_count = mFilesCount;
	fwrite( &files_count, sizeof( files_count ), 1, mPackFile );

	// files info
	unsigned i;
	for( i = 0; i < mFilesCount; i++ )
	{
		DWORD file_name_length = static_cast< DWORD >( mFileStore[ i ].filename.size( ) );

		fwrite( &file_name_length, sizeof( file_name_length ), 1, mPackFile );
		fwrite( mFileStore[ i ].filename.c_str( ), mFileStore[ i ].filename.size( ), 1, mPackFile );
		fwrite( &mFileStore[ i ].length, sizeof( mFileStore[ i ].length ), 1, mPackFile );
		fwrite( &mFileStore[ i ].offset, sizeof( mFileStore[ i ].offset ), 1, mPackFile );
	}

	// files
	for( i = 0; i < mFilesCount; i++ )
	{
		boost::shared_ptr< FILE > file;

		file.reset( fopen( ( dir_with_slash + mFileStore[ i ].filename ).c_str( ), "rb" ), boost::bind( &fclose, _1 ) );

		boost::scoped_array< BYTE > buf ( new BYTE[ mFileStore[ i ].length ] );

		if( mFileStore[ i ].length != fread( buf.get( ), 1, mFileStore[ i ].length, file.get( ) ) )
		{
			gError( "Can't read %d bytes from file %s", mFileStore[ i ].length, mFileStore[ i ].filename.c_str( ) );
			return XFXERR_IO;
		}

		fwrite( buf.get( ), 1, mFileStore[ i ].length, mPackFile );
	}

	rFilename( ) = pack_file_name;

	return S_OK;
}

HRESULT Pack::LoadFile( const String& pack_file_name )
{
	ClosePack( );

	mPackFile = fopen( pack_file_name.c_str( ), "rb" );

	if( !mPackFile )
		return XFXERR_IO;

	// magic
	DWORD magic;
	fread( &magic, sizeof( magic ), 1, mPackFile );

	if( magic != MAKEFOURCC( 'P', 'A', 'C', 'K' ) )
		return XFXERR_IO;

	// version
	DWORD version;
	fread( &version, sizeof( version ), 1, mPackFile );

	if( version > Pack::VERSION )
		return XFXERR_IO;

	// files count
	fread( &mFilesCount, sizeof( mFilesCount ), 1, mPackFile );

	mFileStore.reset( new FileInfo[ mFilesCount ] );

	if( !mFileStore )
		return XFXERR_OUTOFMEMORY;

	unsigned i;

	for( i = 0; i < mFilesCount; i++ )
	{
		DWORD file_name_length;
		fread( &file_name_length, sizeof( file_name_length ), 1, mPackFile );

		boost::scoped_array< char > filename_buf ( new char[ file_name_length + 1 ] );

		if( !filename_buf )
			return XFXERR_OUTOFMEMORY;

		filename_buf[ file_name_length ] = '\0';

		fread( filename_buf.get( ), file_name_length, 1, mPackFile );

		mFileStore[ i ].filename = filename_buf.get( );

		fread( &mFileStore[ i ].length, sizeof( mFileStore[ i ].length ), 1, mPackFile );
		fread( &mFileStore[ i ].offset, sizeof( mFileStore[ i ].offset ), 1, mPackFile );
	}

	rFilename( ) = pack_file_name;

	return S_OK;
}


HRESULT Pack::LoadMemory( const void *pmemory, unsigned long )
{
	ClosePack( );

	mIsMemory = true;
	mpPackMemory = reinterpret_cast< const BYTE *>( pmemory );

	const BYTE * p = mpPackMemory;

	// magic
	DWORD magic = *reinterpret_cast< const DWORD * >( p );
	p += sizeof( magic );

	if( magic != MAKEFOURCC( 'P', 'A', 'C', 'K' ) )
		return XFXERR_IO;

	// version
	DWORD version = *reinterpret_cast< const DWORD * >( p );
	p += sizeof( version );

	if( version > Pack::VERSION )
		return XFXERR_IO;

	// files count
	mFilesCount = *reinterpret_cast< const DWORD * >( p );
	p += sizeof( mFilesCount );

	mFileStore.reset( new FileInfo[ mFilesCount ] );

	if( !mFileStore )
		return XFXERR_OUTOFMEMORY;

	unsigned i;

	for( i = 0; i < mFilesCount; i++ )
	{
		DWORD file_name_length = *reinterpret_cast< const DWORD * >( p );
		p += sizeof( file_name_length );

		boost::scoped_array< char > filename_buf ( new char[ file_name_length + 1 ] );

		if( !filename_buf )
			return XFXERR_OUTOFMEMORY;

		filename_buf[ file_name_length ] = '\0';

		memcpy( filename_buf.get( ), p, file_name_length );
		p += file_name_length;

		mFileStore[ i ].filename = filename_buf.get( );

		mFileStore[ i ].length = *reinterpret_cast< const DWORD * >( p );
		p += sizeof( mFileStore[ i ].length );

		mFileStore[ i ].offset = *reinterpret_cast< const DWORD * >( p );
		p += sizeof( mFileStore[ i ].offset );
	}

	return S_OK;
}

void Pack::ClosePack( )
{
	mFilesCount = 0;
	mFileStore.reset( NULL );
	mIsMemory = false; 
	mpPackMemory = NULL; 

	if( mPackFile )
	{
		fclose( mPackFile );
		mPackFile = NULL;
	}
}





_XFX_END
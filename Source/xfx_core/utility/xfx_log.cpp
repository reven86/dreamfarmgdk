//  File xfx_log.cpp                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_log.h"
#include "xfx_profiler.h"
#include <time.h>
#include <sys/timeb.h>

_XFX_BEGIN





//
// Log
//

Log::Log( const String& log_file ) :
	mLogFile( log_file )
{
	//Write time information
	char date[ 9 ], time[ 9 ];
	_strdate( date );
	_strtime( time );

	FILE * file = fopen( mLogFile.c_str( ), "w+t" );
	
	if( file )
	{
		fprintf( file, "Log started on %s at %s\n\n", time, date );
		fclose( file );
	}
}

Log::~Log( )
{
	FILE * file = fopen( mLogFile.c_str( ), "a+t" );
	
	if( file )
	{
		fprintf( file, "\nEnd of log." );

		fclose( file );
	}
}

void Log::Print( const EMessageType& type, const String& msg, const String& pr )
{
	PROFILE( __FUNCTION__, "General" )

	String prefix;
	switch( type )
	{
		case EMT_CRITICAL:
			prefix = "[Critical]  ";
			break;

		case EMT_ERROR:
			prefix = "[Error]     ";
			break;

		case EMT_WARNING:
			prefix = "[Warning]   ";
			break;

		default:
			prefix = "            ";
	}

	//Write time information
	struct __timeb64 timebuffer;
	_ftime64( &timebuffer );

	time_t seconds;
	time( &seconds );
	tm * t = localtime( &seconds );

	char time_string[ 128 ];
	_snprintf( time_string, 128, "[%02d/%02d/%02d %02d:%02d:%02d.%03d]  ", t->tm_mon + 1, t->tm_mday, t->tm_year % 100, t->tm_hour, t->tm_min, t->tm_sec, timebuffer.millitm );

	prefix = String( time_string ) + prefix + pr;

	FILE * file = fopen( mLogFile.c_str (), "a+t" );
	
	if( file )
	{
		std::vector< String > lines;
		boost::split( lines, msg, boost::is_any_of( "\n" ) );

		if( lines.empty( ) )
		{
			fprintf( file, "%s\n", prefix.c_str( ) );
		}
		else
		{
			BOOST_FOREACH( const String& l, lines )
			{
				fprintf( file, "%s%s\n", prefix.c_str( ), l.c_str( ) );
			}
		}

		fclose( file );
	}
}



_XFX_END
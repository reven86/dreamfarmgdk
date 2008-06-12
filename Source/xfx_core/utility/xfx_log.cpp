//  File xfx_log.cpp                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_log.h"
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

void Log::Print( const EMessageType& type, const String& msg )
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

	String all_msg( prefix + msg + '\n' );

	FILE * file = fopen( mLogFile.c_str (), "a+t" );
	
	if( file )
	{
		//Write time information
		struct __timeb64 timebuffer;
		_ftime64( &timebuffer );

		time_t seconds;
		time( &seconds );
		tm * t = localtime( &seconds );

		fprintf( file, "[%02d/%02d/%02d %02d:%02d:%02d.%03d]  %s", t->tm_mon + 1, t->tm_mday, t->tm_year % 100, t->tm_hour, t->tm_min, t->tm_sec, timebuffer.millitm, all_msg.c_str( ) );

		fclose( file );
	}
}



_XFX_END
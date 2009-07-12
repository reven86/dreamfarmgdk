//  File xfx_main_cvars.cpp                                                   |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_main_cvars.h"
#include "xfx_engine.h"
#include "xfx_cmd.h"
#include "xfx_fs.h"
#include "xfx_input.h"





//
// CVars
//

#define DEFINE_VAR( v ) \
	xfx::Var * v( NULL )

DEFINE_VAR( g_developer );
DEFINE_VAR( g_cheats );
DEFINE_VAR( g_logfile );
DEFINE_VAR( g_debug_cmd );
DEFINE_VAR( g_debug_temp_var );
DEFINE_VAR( g_log_level );
DEFINE_VAR( g_profiler_frame_rate );

#undef DEFINE_VAR







//
// Register cvars
//

_XFX_BEGIN







//
// Commands
//

#define DECLARE_CMD( cmd_name )\
	void cmd_##cmd_name( const String& str )

#define DECLARE_CMD_NOPARAM( cmd_name )\
	void cmd_##cmd_name( const String& )

DECLARE_CMD_NOPARAM( console_toggle )
{
	Engine::Instance( ).ToggleConsole( );
}

DECLARE_CMD( exec )
{
	String::size_type cmdstart = 0;
	String token;
	
	token = next_token( str, cmdstart );

	if( token.empty( ) )
	{
		gToConsole( "Use: exec <filename>" );
	}
	else
	{
		gMess( "Executing %s...", token.c_str( ) );

		unsigned long filesize;
		HRESULT hr;
		if( FAILED( hr = FileSystem::Instance( ).GetFileSize( token, filesize ) ) )
		{
			gError( "Can't open file \"%s\"", token.c_str( ) );
		}
		else
		{
			boost::scoped_array< BYTE > pbuf( new BYTE[ filesize ] );
			if( !pbuf )
				return;

			FileSystem::Instance( ).ReadFile( token, pbuf.get( ) );

			String file( reinterpret_cast< char * >( pbuf.get( ) ), filesize );

			String::size_type s = 0;
			String::size_type e = s;

			while( e != file.length( ) )
			{
				if( file.at( e ) == '\n' )
					s++;
				else if( file.at( e ) == '\r' )
				{
					Cmd::Instance( ).Execute( file.substr( s, e - s ) );

					s = e + 1;
				}
				e++;
			}
			Cmd::Instance( ).Execute( file.substr( s, e - s ) );
		}
	}
}

DECLARE_CMD_NOPARAM( profiler_pause )
{
	ProfilingManager::Instance( ).PauseResumeExternalProfiler( true );
}

DECLARE_CMD_NOPARAM( profiler_resume )
{
	ProfilingManager::Instance( ).PauseResumeExternalProfiler( false );
}

#undef DECLARE_CMD





//
// Make a simple class that registers all cvars and commands
// and then create new static object of that class
//

class Main_CVarsRegistrant
{
public:
	Main_CVarsRegistrant				( )
	{
#define REGISTER_VAR( var_name, def_value, flags ) \
		var_name = Cmd::Instance( ).RegisterVar( #var_name, String( def_value ), flags );

		//
		// Vars
		//

		REGISTER_VAR	( g_developer,					"0",					Cmd::EVF_HIDDEN );
		REGISTER_VAR	( g_cheats,						"0",					Cmd::EVF_SERVER_VAR );
		REGISTER_VAR	( g_logfile,					"xfx.log",				Cmd::EVF_CONST );
		REGISTER_VAR	( g_debug_cmd,					"0",					Cmd::EVF_DEVELOPER );
		REGISTER_VAR	( g_debug_temp_var,				"0",					Cmd::EVF_DEVELOPER );
		REGISTER_VAR	( g_log_level,					"3",					Cmd::EVF_DEFAULT );
		REGISTER_VAR	( g_profiler_frame_rate,		"-1",					Cmd::EVF_DEVELOPER );

#undef REGISTER_VAR

		//
		// Commands
		//

#define REGISTER_CMD( cmd_name, flags ) \
		Cmd::Instance( ).RegisterCmd( #cmd_name, cmd_##cmd_name, flags );

		Cmd::Instance( ).RegisterCmd(
			"bind",					boost::bind( &Input::Cmd_bind, boost::ref( Input::Instance( ) ), false, _1 ),	Cmd::ECF_DEFAULT
			);

		Cmd::Instance( ).RegisterCmd(
			"bindc",				boost::bind( &Input::Cmd_bind, boost::ref( Input::Instance( ) ), true, _1 ),	Cmd::ECF_DEFAULT
			);

		REGISTER_CMD( console_toggle,					Cmd::ECF_DEFAULT );
		REGISTER_CMD( exec,								Cmd::ECF_DEFAULT );
		REGISTER_CMD( profiler_pause,					Cmd::ECF_DEFAULT );
		REGISTER_CMD( profiler_resume,					Cmd::ECF_DEFAULT );

#undef REGISTER_CMD
	};
};

static Main_CVarsRegistrant _main_cvars_reg;

_XFX_END
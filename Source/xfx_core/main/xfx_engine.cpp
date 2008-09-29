//  File xfx_engine.cpp                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_engine.h"
#include "xfx_main_cvars.h"
#include "xfx_console.h"
#include "xfx_input.h"
#include "xfx_cmd.h"

_XFX_BEGIN





//
// Variables
//

const String Engine::msVersion( "1.0.$WCREV$" );





//
// Engine
//

Engine::Engine( )
{
}

Engine::~Engine( )
{
}

bool Engine::Init( bool init_input )
{
	mEngineLogPtr.reset( new class Log( g_logfile->Value( ) ) );

	gMess( "Initializing xfx:" );
	gMess( "...engine version: %s", msVersion.c_str( ) );

	//Getting OS information
	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof( osinfo );

	GetVersionEx( &osinfo );

	gMess( "" );
	gMess( "OS information:" );
	gMess( "...platform: %s", ( osinfo.dwPlatformId == 1 ) ? "Win32" : "WinNT" );
	gMess( "...version number: %u.%u.%u", osinfo.dwMajorVersion, osinfo.dwMinorVersion, LOWORD( osinfo.dwBuildNumber ) );
	gMess( "...additional info: %s", osinfo.szCSDVersion );

	//Getting processor info
	char			cpu_vendorID[ 13 ];
	unsigned		cpu_family;
	unsigned		cpu_model;
    
	cpu_vendorID[12] = '\0';
	__asm {
		xor		eax, eax;
		cpuid;
		mov		dword ptr [cpu_vendorID], ebx;
		mov		dword ptr [cpu_vendorID + 4], edx;
		mov		dword ptr [cpu_vendorID + 8], ecx;
		mov		eax, 1;
		cpuid;
		mov		ebx, eax;
		and		eax, 0xf00;
		shr		eax, 8;
		mov		cpu_family, eax;
		mov		eax, ebx;
		and		eax, 0xf0;
		shr		eax, 4;
		mov		cpu_model, eax;
	}

	gMess( "" );
	gMess( "CPU information:" );
	gMess( "...vendorID: %s", cpu_vendorID );
	gMess( "...family: %u", cpu_family );
	gMess( "...model: %u", cpu_model );

	if( init_input )
	{
		HRESULT hr;
		if( FAILED( hr = Input::Instance( ).Init( ) ) )
			return false;
	}

	gMess( "Initialization xfx was successfully completed!" );
	gMess( "" );

	return true;
}

bool Engine::Shutdown( )
{
	Caches::Instance( ).FlushAll( );

	Input::Instance( ).Shutdown( );

	gMess( "" );
	gMess( "Shutting down xfx." );

	return true;
}

void Engine::PrintToConsole( const Log::EMessageType& type, const WString& str )
{
	if( mConsoleControlPtr )
	{
		mConsoleControlPtr->Print( type, str );
	}
}

void Engine::ToggleConsole( )
{
	if( mConsoleControlPtr )
	{
		mConsoleControlPtr->Toggle( );
	}
}

bool Engine::IsConsoleActive( ) const
{
	if( mConsoleControlPtr )
	{
		return mConsoleControlPtr->IsVisible( );
	}

	return false;
}

void Engine::Log( const Log::EMessageType& type, const String& msg )
{
	if( type > g_log_level->AsInt( ) )
		return;

	if( mEngineLogPtr )
		mEngineLogPtr->Print( type, msg );

	// duplicate in console
	PrintToConsole( type, fromMBCS( msg ) );
}





_XFX_END




void gToConsole (const char * fmt, ...)
{
	static char message[ 1024 ];

	va_list va;
	va_start (va, fmt);
	_vsnprintf (message, 1024, fmt, va);

	xfx::Engine::Instance( ).PrintToConsole( xfx::Log::EMT_DEFAULT, xfx::fromMBCS( message ) );
}

void gMess (const char * fmt, ...)
{
	static char message[ 1024 ];

	va_list va;
	va_start (va, fmt);
	_vsnprintf (message, 1024, fmt, va);

	xfx::Engine::Instance ().Log (xfx::Log::EMT_DEFAULT, message);
}

void gWarn (const char * fmt, ...)
{
	static char message[ 1024 ];

	va_list va;
	va_start (va, fmt);
	_vsnprintf (message, 1024, fmt, va);

	xfx::Engine::Instance ().Log (xfx::Log::EMT_WARNING, message);
}

void gError (const char * fmt, ...)
{
	static char message[ 1024 ];

	va_list va;
	va_start (va, fmt);
	_vsnprintf (message, 1024, fmt, va);

	xfx::Engine::Instance ().Log (xfx::Log::EMT_ERROR, message);
}

void gCError (const char * fmt, ...)
{
	static char message[ 1024 ];

	va_list va;
	va_start (va, fmt);
	_vsnprintf (message, 1024, fmt, va);

	xfx::Engine::Instance ().Log (xfx::Log::EMT_CRITICAL, message);
}


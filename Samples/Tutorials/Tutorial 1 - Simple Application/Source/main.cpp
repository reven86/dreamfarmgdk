#include "pch.h"
#include "application.h"






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






//
// Main
//

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	if( SUCCEEDED( Application::Instance( ).InitEngine( hInstance, lpCmdLine, L"Tutorial1WndClass", L"Tutorial 1 - Simple Application" ) ) )
	{
		Application::Instance( ).MainLoop( );
		Application::Instance( ).Shutdown( );
	}

	return 0;
}
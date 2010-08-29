#include "pch.h"
#include "application.h"







//
// Main
//

int APIENTRY WinMain(HINSTANCE /*hInstance*/,
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR     lpCmdLine,
                     int       /*nCmdShow*/)
{
	if( SUCCEEDED( Application::Instance( ).InitEngine( lpCmdLine, L"Tutorial1WndClass", L"Tutorial 1 - Simple Application" ) ) )
	{
		Application::Instance( ).MainLoop( );
		Application::Instance( ).Shutdown( );
	}

	return 0;
}
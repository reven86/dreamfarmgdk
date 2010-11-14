#include "xfx.h"


namespace bp = boost::python;



// BOOST_PYTHON_MODULE(X) macro exports 'initX' function
// Since we statically linked to xfx_python, we need to call init function explicitly
extern "C" __declspec(dllexport) void initxfx( );

extern "C" __declspec(dllexport) void initxfx_Debug( )
{
	initxfx( );
}


//
// gGetApplication implementation
//


xfx::Application& xfx::gGetApplication( )
{
	// Get Application object from Python
	bp::object gGlobals( bp::handle< >( bp::borrowed( PyImport_AddModule("__main__") ) ) );
	bp::object app = gGlobals.attr("Application");
	return *( bp::extract< xfx::Application * >( app.attr( "instance" ) ) );
}





BOOL WINAPI DllMain(
  HANDLE /*hinstDLL*/, 
  DWORD dwReason, 
  LPVOID /*lpvReserved*/
  )
{
	switch( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		//initxfx( );
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
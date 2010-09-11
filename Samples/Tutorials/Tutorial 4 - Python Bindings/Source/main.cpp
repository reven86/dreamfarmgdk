#include "pch.h"
#include <stdio.h>
#include <omp.h>

namespace bp = boost::python;




// Python globals. We use them to access Application wrapper
bp::object gGlobals;


//
// gGetApplication implementation
//

xfx::Application& xfx::gGetApplication( )
{
	// Get Application object from Python
	bp::object app = gGlobals["Application"];
	return *( bp::extract< xfx::Application * >( app.attr( "instance" ) ) );
}


// BOOST_PYTHON_MODULE(X) macro exports 'initX' function
// Since we statically linked to xfx_python, we need to call init function explicitly
extern "C" void initxfx( );


//
// Main
//

int APIENTRY WinMain(HINSTANCE /*hInstance*/,
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR     lpCmdLine,
                     int       /*nCmdShow*/)
{
	// Setting PYTHONPATH to point to PyDev installation
	// Note: modify this path to your installation of PyDev
	putenv( "PYTHONPATH=C:\\eclipse\\plugins\\org.python.pydev.debug_1.5.7.2010042100\\pysrc;.\\Data\\python_libs.zip;.\\Data\\DLLs" );

	// Disable importing of 'site' module during initialization (we don't have that module)
	//Py_NoSiteFlag = 1;
	Py_Initialize( );

	// Explicitly initialize Python bindings to XFX
	initxfx( );

	// Get and store Python globals
	bp::object main = bp::import("__main__"); 
	gGlobals = main.attr("__dict__");

	// Execute script
	bp::handle_exception( boost::bind( &bp::exec_file, "Data\\app.py", boost::ref( gGlobals ), boost::ref( gGlobals ) ) );
	//bp::handle_exception( boost::bind( &bp::exec_file, "C:\\Users\\RevEn\\workspace\\PyNN\\app.py", boost::ref( gGlobals ), boost::ref( gGlobals ) ) );

	// Print errors, if any
	if (PyErr_Occurred() != NULL)
		PyErr_Print();

	// Finalize python
	Py_Finalize( );

	return 0;
}
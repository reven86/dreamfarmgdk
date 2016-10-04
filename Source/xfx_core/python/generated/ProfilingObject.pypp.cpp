// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "profilingobject.pypp.hpp"

namespace bp = boost::python;

void register_ProfilingObject_class(){

    { //::xfx::ProfilingObject
        typedef bp::class_< xfx::ProfilingObject, boost::noncopyable > ProfilingObject_exposer_t;
        ProfilingObject_exposer_t ProfilingObject_exposer = ProfilingObject_exposer_t( "ProfilingObject", bp::init< DWORD const & >(( bp::arg("id") )) );
        bp::scope ProfilingObject_scope( ProfilingObject_exposer );
        bp::implicitly_convertible< DWORD const &, xfx::ProfilingObject >();
        bp::register_ptr_to_python< boost::shared_ptr< xfx::ProfilingObject const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::ProfilingObject >, boost::shared_ptr< xfx::ProfilingObject const > >( );
    }

}
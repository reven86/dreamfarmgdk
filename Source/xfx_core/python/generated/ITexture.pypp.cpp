// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "itexture.pypp.hpp"

namespace bp = boost::python;

void register_ITexture_class(){

    { //::xfx::ITexture
        typedef bp::class_< xfx::ITexture, boost::noncopyable > ITexture_exposer_t;
        ITexture_exposer_t ITexture_exposer = ITexture_exposer_t( "ITexture", bp::no_init );
        bp::scope ITexture_scope( ITexture_exposer );
        bp::register_ptr_to_python< boost::shared_ptr< xfx::ITexture const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::ITexture >, boost::shared_ptr< xfx::ITexture const > >( );
    }

}

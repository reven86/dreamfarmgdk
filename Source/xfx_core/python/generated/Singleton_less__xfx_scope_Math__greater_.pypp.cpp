// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "singleton_less__xfx_scope_math__greater_.pypp.hpp"

namespace bp = boost::python;

void register_Singleton_less__xfx_scope_Math__greater__class(){

    { //::xfx::Singleton< xfx::Math >
        typedef bp::class_< xfx::Singleton< xfx::Math >, boost::noncopyable > Singleton_less__xfx_scope_Math__greater__exposer_t;
        Singleton_less__xfx_scope_Math__greater__exposer_t Singleton_less__xfx_scope_Math__greater__exposer = Singleton_less__xfx_scope_Math__greater__exposer_t( "Singleton_less__xfx_scope_Math__greater_", bp::no_init );
        bp::scope Singleton_less__xfx_scope_Math__greater__scope( Singleton_less__xfx_scope_Math__greater__exposer );
        { //property "instance"[fget=::xfx::Singleton<xfx::Math>::Instance]
        
            typedef xfx::Singleton<xfx::Math> fget_class_t;
            
            typedef ::xfx::Math & ( *fget )(  );
            
            Singleton_less__xfx_scope_Math__greater__exposer.add_static_property( 
                "instance"
                , bp::make_function( 
                      fget( &::xfx::Singleton<xfx::Math>::Instance )
                    , bp::return_value_policy< bp::reference_existing_object >() )  );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::Singleton<xfx::Math> const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::Singleton<xfx::Math> >, boost::shared_ptr< xfx::Singleton<xfx::Math> const > >( );
    }

}

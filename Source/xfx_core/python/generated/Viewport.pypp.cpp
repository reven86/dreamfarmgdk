// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "__convenience.pypp.hpp"
#include "__call_policies.pypp.hpp"
#include "xfx_py_bindings.h"
#include "viewport.pypp.hpp"

namespace bp = boost::python;

static boost::python::tuple Project_0e7cc8a4c43927e22abefe74af1e3c97( ::xfx::Viewport const & inst, ::xfx::Vec3 const & p ){
    float x2;
    float y2;
    inst.Project(x2, y2, p);
    return bp::make_tuple( x2, y2 );
}

void register_Viewport_class(){

    { //::xfx::Viewport
        typedef bp::class_< xfx::Viewport, bp::bases< xfx::Singleton< xfx::Viewport > >, boost::noncopyable > Viewport_exposer_t;
        Viewport_exposer_t Viewport_exposer = Viewport_exposer_t( "Viewport", bp::no_init );
        bp::scope Viewport_scope( Viewport_exposer );
        { //::xfx::Viewport::Init
        
            typedef void ( ::xfx::Viewport::*init_function_type )( unsigned int,unsigned int ) ;
            
            Viewport_exposer.def( 
                "init"
                , init_function_type( &::xfx::Viewport::Init )
                , ( bp::arg("width"), bp::arg("height") ) );
        
        }
        { //::xfx::Viewport::MapRendererToViewport
        
            typedef ::boost::tuples::tuple< float, float, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::xfx::Viewport::*map_renderer_to_viewport_function_type )( float,float ) const;
            
            Viewport_exposer.def( 
                "map_renderer_to_viewport"
                , map_renderer_to_viewport_function_type( &::xfx::Viewport::MapRendererToViewport )
                , ( bp::arg("x"), bp::arg("y") ) );
        
        }
        { //::xfx::Viewport::MapViewportToRenderer
        
            typedef ::boost::tuples::tuple< float, float, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::xfx::Viewport::*map_viewport_to_renderer_function_type )( float,float ) const;
            
            Viewport_exposer.def( 
                "map_viewport_to_renderer"
                , map_viewport_to_renderer_function_type( &::xfx::Viewport::MapViewportToRenderer )
                , ( bp::arg("x"), bp::arg("y") ) );
        
        }
        { //::xfx::Viewport::Project
        
            typedef boost::python::tuple ( *project_function_type )( ::xfx::Viewport const &,::xfx::Vec3 const & );
            
            Viewport_exposer.def( 
                "project"
                , project_function_type( &Project_0e7cc8a4c43927e22abefe74af1e3c97 )
                , ( bp::arg("inst"), bp::arg("p") ) );
        
        }
        { //::xfx::Viewport::Render
        
            typedef void ( ::xfx::Viewport::*render_function_type )(  ) const;
            
            Viewport_exposer.def( 
                "render"
                , render_function_type( &::xfx::Viewport::Render ) );
        
        }
        { //::xfx::Viewport::Unproject
        
            typedef void ( ::xfx::Viewport::*unproject_function_type )( ::xfx::Vec3 &,float,float ) const;
            
            Viewport_exposer.def( 
                "unproject"
                , unproject_function_type( &::xfx::Viewport::Unproject )
                , ( bp::arg("out"), bp::arg("x"), bp::arg("y") ) );
        
        }
        { //property "width"[fget=::xfx::Viewport::GetWidth, fset=::xfx::Viewport::SetWidth]
        
            typedef unsigned int const & ( ::xfx::Viewport::*fget )(  ) const;
            typedef void ( ::xfx::Viewport::*fset )( unsigned int const & ) ;
            
            Viewport_exposer.add_property( 
                "width"
                , bp::make_function( 
                      fget( &::xfx::Viewport::GetWidth )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::xfx::Viewport::SetWidth )
                , "get\\set property, built on top of \"unsigned int const & xfx::Viewport::GetWidth() const [member function]\" and \"void xfx::Viewport::SetWidth(unsigned int const & width) [member function]\"" );
        
        }
        { //property "height"[fget=::xfx::Viewport::GetHeight, fset=::xfx::Viewport::SetHeight]
        
            typedef unsigned int const & ( ::xfx::Viewport::*fget )(  ) const;
            typedef void ( ::xfx::Viewport::*fset )( unsigned int const & ) ;
            
            Viewport_exposer.add_property( 
                "height"
                , bp::make_function( 
                      fget( &::xfx::Viewport::GetHeight )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::xfx::Viewport::SetHeight )
                , "get\\set property, built on top of \"unsigned int const & xfx::Viewport::GetHeight() const [member function]\" and \"void xfx::Viewport::SetHeight(unsigned int const & height) [member function]\"" );
        
        }
        { //property "inv_width"[fget=::xfx::Viewport::InvWidth]
        
            typedef float const & ( ::xfx::Viewport::*fget )(  ) const;
            
            Viewport_exposer.add_property( 
                "inv_width"
                , bp::make_function( 
                      fget( &::xfx::Viewport::InvWidth )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , "get property, built on top of \"float const & xfx::Viewport::InvWidth() const [member function]\"" );
        
        }
        { //property "inv_height"[fget=::xfx::Viewport::InvHeight]
        
            typedef float const & ( ::xfx::Viewport::*fget )(  ) const;
            
            Viewport_exposer.add_property( 
                "inv_height"
                , bp::make_function( 
                      fget( &::xfx::Viewport::InvHeight )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , "get property, built on top of \"float const & xfx::Viewport::InvHeight() const [member function]\"" );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::Viewport const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::Viewport >, boost::shared_ptr< xfx::Viewport const > >( );
    }

}

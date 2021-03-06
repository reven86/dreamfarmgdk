// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "__convenience.pypp.hpp"
#include "__call_policies.pypp.hpp"
#include "xfx_py_bindings.h"
#include "sphere.pypp.hpp"

namespace bp = boost::python;

struct Sphere_wrapper : xfx::Primitives::Sphere, bp::wrapper< xfx::Primitives::Sphere > {

    Sphere_wrapper(xfx::Primitives::Sphere const & arg )
    : xfx::Primitives::Sphere( arg )
      , bp::wrapper< xfx::Primitives::Sphere >(){
        // copy constructor
        
    }

    Sphere_wrapper( )
    : xfx::Primitives::Sphere( )
      , bp::wrapper< xfx::Primitives::Sphere >(){
        // null constructor
    
    }

    Sphere_wrapper(::xfx::Vec3 const & pos, float r )
    : xfx::Primitives::Sphere( boost::ref(pos), r )
      , bp::wrapper< xfx::Primitives::Sphere >(){
        // constructor
    
    }

    virtual void Projection( float & a, float & b, ::xfx::Vec3 const & axis ) const  {
        namespace bpl = boost::python;
        if( bpl::override func_projection = this->get_override( "projection" ) ){
            bpl::object py_result = bpl::call<bpl::object>( func_projection.ptr(), axis );
            a = boost::python::extract< float >( pyplus_conv::get_out_argument( py_result, "a" ) );
            b = boost::python::extract< float >( pyplus_conv::get_out_argument( py_result, "b" ) );
        }
        else{
            xfx::Primitives::Sphere::Projection( a, b, boost::ref(axis) );
        }
    }
    
    static boost::python::tuple default_projection( ::xfx::Primitives::Sphere const & inst, ::xfx::Vec3 const & axis ){
        float a2;
        float b2;
        if( dynamic_cast< Sphere_wrapper const* >( boost::addressof( inst ) ) ){
            inst.::xfx::Primitives::Sphere::Projection(a2, b2, axis);
        }
        else{
            inst.Projection(a2, b2, axis);
        }
        return bp::make_tuple( a2, b2 );
    }

};

void register_Sphere_class(){

    { //::xfx::Primitives::Sphere
        typedef bp::class_< Sphere_wrapper, bp::bases< xfx::Primitive > > Sphere_exposer_t;
        Sphere_exposer_t Sphere_exposer = Sphere_exposer_t( "Sphere", bp::init< >() );
        bp::scope Sphere_scope( Sphere_exposer );
        Sphere_exposer.def( bp::init< xfx::Vec3 const &, float >(( bp::arg("pos"), bp::arg("r") )) );
        { //::xfx::Primitives::Sphere::Projection
        
            typedef boost::python::tuple ( *default_projection_function_type )( ::xfx::Primitives::Sphere const &,::xfx::Vec3 const & );
            
            Sphere_exposer.def( 
                "projection"
                , default_projection_function_type( &Sphere_wrapper::default_projection )
                , ( bp::arg("inst"), bp::arg("axis") ) );
        
        }
        { //property "position"[fget=::xfx::Primitives::Sphere::Position, fset=::xfx::Primitives::Sphere::SetPosition]
        
            typedef ::xfx::Vec3 const & ( ::xfx::Primitives::Sphere::*fget )(  ) const;
            typedef void ( ::xfx::Primitives::Sphere::*fset )( ::xfx::Vec3 const & ) ;
            
            Sphere_exposer.add_property( 
                "position"
                , bp::make_function( 
                      fget( &::xfx::Primitives::Sphere::Position )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::xfx::Primitives::Sphere::SetPosition )
                , "get\\set property, built on top of \"xfx::Vec3 const & xfx::Primitives::Sphere::Position() const [member function]\" and \"void xfx::Primitives::Sphere::SetPosition(xfx::Vec3 const & p) [member function]\"" );
        
        }
        { //property "radius"[fget=::xfx::Primitives::Sphere::Radius, fset=::xfx::Primitives::Sphere::SetRadius]
        
            typedef float const & ( ::xfx::Primitives::Sphere::*fget )(  ) const;
            typedef void ( ::xfx::Primitives::Sphere::*fset )( float const & ) ;
            
            Sphere_exposer.add_property( 
                "radius"
                , bp::make_function( 
                      fget( &::xfx::Primitives::Sphere::Radius )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::xfx::Primitives::Sphere::SetRadius )
                , "get\\set property, built on top of \"float const & xfx::Primitives::Sphere::Radius() const [member function]\" and \"void xfx::Primitives::Sphere::SetRadius(float const & r) [member function]\"" );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::Primitives::Sphere const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::Primitives::Sphere >, boost::shared_ptr< xfx::Primitives::Sphere const > >( );
    }

}

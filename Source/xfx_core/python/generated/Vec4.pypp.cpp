// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "vec4.pypp.hpp"

namespace bp = boost::python;

void register_Vec4_class(){

    { //::xfx::Vec4
        typedef bp::class_< xfx::Vec4 > Vec4_exposer_t;
        Vec4_exposer_t Vec4_exposer = Vec4_exposer_t( "Vec4", bp::init< >() );
        bp::scope Vec4_scope( Vec4_exposer );
        Vec4_exposer.def( bp::init< float >(( bp::arg("v") )) );
        bp::implicitly_convertible< float, xfx::Vec4 >();
        Vec4_exposer.def( bp::init< xfx::Vec4 const & >(( bp::arg("v") )) );
        Vec4_exposer.def( bp::init< float, float, float, float >(( bp::arg("vx"), bp::arg("vy"), bp::arg("vz"), bp::arg("vw") )) );
        { //::xfx::Vec4::Clip
        
            typedef void ( *clip_function_type )( ::xfx::Vec4 & );
            
            Vec4_exposer.def( 
                "clip"
                , clip_function_type( &::xfx::Vec4::Clip )
                , ( bp::arg("v") ) );
        
        }
        { //::xfx::Vec4::Dot
        
            typedef float ( *dot_function_type )( ::xfx::Vec4 const &,::xfx::Vec4 const & );
            
            Vec4_exposer.def( 
                "dot"
                , dot_function_type( &::xfx::Vec4::Dot )
                , ( bp::arg("u"), bp::arg("v") ) );
        
        }
        { //::xfx::Vec4::Lerp
        
            typedef ::xfx::Vec4 ( *lerp_function_type )( ::xfx::Vec4 const &,::xfx::Vec4 const &,float const & );
            
            Vec4_exposer.def( 
                "lerp"
                , lerp_function_type( &::xfx::Vec4::Lerp )
                , ( bp::arg("u"), bp::arg("v"), bp::arg("t") ) );
        
        }
        { //::xfx::Vec4::Normalize
        
            typedef void ( ::xfx::Vec4::*normalize_function_type )(  ) ;
            
            Vec4_exposer.def( 
                "normalize"
                , normalize_function_type( &::xfx::Vec4::Normalize ) );
        
        }
        { //::xfx::Vec4::Rand
        
            typedef ::xfx::Vec4 ( *rand_function_type )(  );
            
            Vec4_exposer.def( 
                "rand"
                , rand_function_type( &::xfx::Vec4::Rand ) );
        
        }
        { //::xfx::Vec4::ToString
        
            typedef ::xfx::String ( ::xfx::Vec4::*__repr___function_type )(  ) const;
            
            Vec4_exposer.def( 
                "__repr__"
                , __repr___function_type( &::xfx::Vec4::ToString ) );
        
        }
        Vec4_exposer.def( bp::self *= bp::other< float >() );
        Vec4_exposer.def( bp::self *= bp::self );
        Vec4_exposer.def( bp::self += bp::self );
        Vec4_exposer.def( -bp::self );
        Vec4_exposer.def( bp::self -= bp::self );
        Vec4_exposer.def( bp::self /= bp::other< float >() );
        { //::xfx::Vec4::operator=
        
            typedef ::xfx::Vec4 & ( ::xfx::Vec4::*assign_function_type )( ::xfx::Vec4 const & ) ;
            
            Vec4_exposer.def( 
                "assign"
                , assign_function_type( &::xfx::Vec4::operator= )
                , ( bp::arg("v") )
                , bp::return_self< >() );
        
        }
        { //::xfx::Vec4::xyzw
        
            typedef void ( ::xfx::Vec4::*xyzw_function_type )( float,float,float,float ) ;
            
            Vec4_exposer.def( 
                "xyzw"
                , xyzw_function_type( &::xfx::Vec4::xyzw )
                , ( bp::arg("_x"), bp::arg("_y"), bp::arg("_z"), bp::arg("_w") ) );
        
        }
        Vec4_exposer.def_readwrite( "w", &xfx::Vec4::w );
        Vec4_exposer.def_readwrite( "x", &xfx::Vec4::x );
        Vec4_exposer.def_readwrite( "y", &xfx::Vec4::y );
        Vec4_exposer.def_readwrite( "z", &xfx::Vec4::z );
        Vec4_exposer.staticmethod( "clip" );
        Vec4_exposer.staticmethod( "dot" );
        Vec4_exposer.staticmethod( "lerp" );
        Vec4_exposer.staticmethod( "rand" );
        { //property "normalized"[fget=::xfx::Vec4::GetNormalized]
        
            typedef ::xfx::Vec4 ( ::xfx::Vec4::*fget )(  ) const;
            
            Vec4_exposer.add_property( 
                "normalized"
                , fget( &::xfx::Vec4::GetNormalized )
                , "get property, built on top of \"xfx::Vec4 xfx::Vec4::GetNormalized() const [member function]\"" );
        
        }
        { //property "len_sq"[fget=::xfx::Vec4::LenSq]
        
            typedef float ( ::xfx::Vec4::*fget )(  ) const;
            
            Vec4_exposer.add_property( 
                "len_sq"
                , fget( &::xfx::Vec4::LenSq )
                , "get property, built on top of \"float xfx::Vec4::LenSq() const [member function]\"" );
        
        }
        { //property "len"[fget=::xfx::Vec4::Len]
        
            typedef float ( ::xfx::Vec4::*fget )(  ) const;
            
            Vec4_exposer.add_property( 
                "len"
                , fget( &::xfx::Vec4::Len )
                , "get property, built on top of \"float xfx::Vec4::Len() const [member function]\"" );
        
        }
        Vec4_exposer.def( bp::self != bp::self );
        Vec4_exposer.def( bp::self * bp::other< float >() );
        Vec4_exposer.def( bp::other< float >() * bp::self );
        Vec4_exposer.def( bp::self + bp::self );
        Vec4_exposer.def( bp::self - bp::self );
        Vec4_exposer.def( bp::self / bp::other< float >() );
        Vec4_exposer.def( bp::self == bp::self );
        bp::register_ptr_to_python< boost::shared_ptr< xfx::Vec4 const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::Vec4 >, boost::shared_ptr< xfx::Vec4 const > >( );
    }

}
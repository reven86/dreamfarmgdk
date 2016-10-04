// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "renderedtexture.pypp.hpp"

namespace bp = boost::python;

void register_RenderedTexture_class(){

    { //::xfx::RenderedTexture
        typedef bp::class_< xfx::RenderedTexture, bp::bases< xfx::ITexture, xfx::Transformable2D >, boost::noncopyable > RenderedTexture_exposer_t;
        RenderedTexture_exposer_t RenderedTexture_exposer = RenderedTexture_exposer_t( "RenderedTexture", bp::init< >() );
        bp::scope RenderedTexture_scope( RenderedTexture_exposer );
        { //::xfx::RenderedTexture::Create
        
            typedef ::HRESULT ( ::xfx::RenderedTexture::*create_function_type )( unsigned int,unsigned int,::D3DFORMAT const & ) ;
            
            RenderedTexture_exposer.def( 
                "create"
                , create_function_type( &::xfx::RenderedTexture::Create )
                , ( bp::arg("width"), bp::arg("height"), bp::arg("fmt")=::D3DFMT_A8R8G8B8 ) );
        
        }
        { //::xfx::RenderedTexture::Free
        
            typedef void ( ::xfx::RenderedTexture::*free_function_type )(  ) ;
            
            RenderedTexture_exposer.def( 
                "free"
                , free_function_type( &::xfx::RenderedTexture::Free ) );
        
        }
        { //::xfx::RenderedTexture::UpdateTransformation
        
            typedef void ( ::xfx::RenderedTexture::*update_transformation_function_type )(  ) ;
            
            RenderedTexture_exposer.def( 
                "update_transformation"
                , update_transformation_function_type( &::xfx::RenderedTexture::UpdateTransformation ) );
        
        }
        { //property "width"[fget=::xfx::RenderedTexture::GetWidth]
        
            typedef unsigned int ( ::xfx::RenderedTexture::*fget )(  ) const;
            
            RenderedTexture_exposer.add_property( 
                "width"
                , fget( &::xfx::RenderedTexture::GetWidth )
                , "get property, built on top of \"unsigned int xfx::RenderedTexture::GetWidth() const [member function]\"" );
        
        }
        { //property "height"[fget=::xfx::RenderedTexture::GetHeight]
        
            typedef unsigned int ( ::xfx::RenderedTexture::*fget )(  ) const;
            
            RenderedTexture_exposer.add_property( 
                "height"
                , fget( &::xfx::RenderedTexture::GetHeight )
                , "get property, built on top of \"unsigned int xfx::RenderedTexture::GetHeight() const [member function]\"" );
        
        }
        { //property "texture_matrix"[fget=::xfx::RenderedTexture::GetTextureMatrix]
        
            typedef ::xfx::Mat4 const & ( ::xfx::RenderedTexture::*fget )(  ) const;
            
            RenderedTexture_exposer.add_property( 
                "texture_matrix"
                , bp::make_function( 
                      fget( &::xfx::RenderedTexture::GetTextureMatrix )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , "get property, built on top of \"xfx::Mat4 const & xfx::RenderedTexture::GetTextureMatrix() const [member function]\"" );
        
        }
        { //property "transformation"[fget=::xfx::RenderedTexture::GetTransformation]
        
            typedef ::xfx::Mat4 const & ( ::xfx::RenderedTexture::*fget )(  ) const;
            
            RenderedTexture_exposer.add_property( 
                "transformation"
                , bp::make_function( 
                      fget( &::xfx::RenderedTexture::GetTransformation )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , "get property, built on top of \"xfx::Mat4 const & xfx::RenderedTexture::GetTransformation() const [member function]\"" );
        
        }
        { //property "identity_result_transform"[fget=::xfx::RenderedTexture::IsIdentityResultTransform]
        
            typedef bool ( ::xfx::RenderedTexture::*fget )(  ) const;
            
            RenderedTexture_exposer.add_property( 
                "identity_result_transform"
                , fget( &::xfx::RenderedTexture::IsIdentityResultTransform )
                , "get property, built on top of \"bool xfx::RenderedTexture::IsIdentityResultTransform() const [member function]\"" );
        
        }
        { //property "empty"[fget=::xfx::RenderedTexture::IsEmpty]
        
            typedef bool ( ::xfx::RenderedTexture::*fget )(  ) const;
            
            RenderedTexture_exposer.add_property( 
                "empty"
                , fget( &::xfx::RenderedTexture::IsEmpty )
                , "get property, built on top of \"bool xfx::RenderedTexture::IsEmpty() const [member function]\"" );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::RenderedTexture > >();
        bp::implicitly_convertible< boost::shared_ptr< xfx::RenderedTexture >, boost::shared_ptr< boost::noncopyable_::noncopyable > >();
        bp::implicitly_convertible< boost::shared_ptr< xfx::RenderedTexture >, boost::shared_ptr< xfx::Transformable2D > >();
        bp::implicitly_convertible< boost::shared_ptr< xfx::RenderedTexture >, boost::shared_ptr< xfx::ITexture > >();
        bp::register_ptr_to_python< boost::shared_ptr< xfx::RenderedTexture const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::RenderedTexture >, boost::shared_ptr< xfx::RenderedTexture const > >( );
    }

}
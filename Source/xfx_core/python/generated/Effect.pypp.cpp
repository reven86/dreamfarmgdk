// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "effect.pypp.hpp"

namespace bp = boost::python;

struct Effect_wrapper : xfx::Effect, bp::wrapper< xfx::Effect > {

    Effect_wrapper(xfx::Effect const & arg )
    : xfx::Effect( arg )
      , bp::wrapper< xfx::Effect >(){
        // copy constructor
        
    }

    Effect_wrapper( )
    : xfx::Effect( )
      , bp::wrapper< xfx::Effect >(){
        // null constructor
    
    }

    virtual ::HRESULT LoadFile( ::xfx::String const & filename ) {
        if( bp::override func_load_file = this->get_override( "load_file" ) )
            return func_load_file( filename );
        else
            return this->xfx::Effect::LoadFile( filename );
    }
    
    
    ::HRESULT default_load_file( ::xfx::String const & filename ) {
        return xfx::Effect::LoadFile( filename );
    }

    virtual ::HRESULT LoadMemory( void const * pmemory, long unsigned int filelen ) {
        if( bp::override func_load_memory = this->get_override( "load_memory" ) )
            return func_load_memory( pmemory, filelen );
        else
            return this->xfx::Effect::LoadMemory( pmemory, filelen );
    }
    
    
    ::HRESULT default_load_memory( void const * pmemory, long unsigned int filelen ) {
        return xfx::Effect::LoadMemory( pmemory, filelen );
    }

};

void register_Effect_class(){

    { //::xfx::Effect
        typedef bp::class_< Effect_wrapper, bp::bases< xfx::Resource > > Effect_exposer_t;
        Effect_exposer_t Effect_exposer = Effect_exposer_t( "Effect", bp::init< >() );
        bp::scope Effect_scope( Effect_exposer );
        { //::xfx::Effect::GetHandle
        
            typedef ::D3DXHANDLE ( ::xfx::Effect::*get_handle_function_type )( ::xfx::String const & ) const;
            
            Effect_exposer.def( 
                "get_handle"
                , get_handle_function_type( &::xfx::Effect::GetHandle )
                , ( bp::arg("parameter") ) );
        
        }
        { //::xfx::Effect::LoadFile
        
            typedef ::HRESULT ( ::xfx::Effect::*load_file_function_type )( ::xfx::String const & ) ;
            typedef ::HRESULT ( Effect_wrapper::*default_load_file_function_type )( ::xfx::String const & ) ;
            
            Effect_exposer.def( 
                "load_file"
                , load_file_function_type(&::xfx::Effect::LoadFile)
                , default_load_file_function_type(&Effect_wrapper::default_load_file)
                , ( bp::arg("filename") ) );
        
        }
        { //::xfx::Effect::LoadMemory
        
            typedef ::HRESULT ( ::xfx::Effect::*load_memory_function_type )( void const *,long unsigned int ) ;
            typedef ::HRESULT ( Effect_wrapper::*default_load_memory_function_type )( void const *,long unsigned int ) ;
            
            Effect_exposer.def( 
                "load_memory"
                , load_memory_function_type(&::xfx::Effect::LoadMemory)
                , default_load_memory_function_type(&Effect_wrapper::default_load_memory)
                , ( bp::arg("pmemory"), bp::arg("filelen") ) );
        
        }
        { //::xfx::Effect::RenderEffect
        
            typedef void ( ::xfx::Effect::*render_effect_function_type )( ::boost::function0< void > const &,::DWORD ) const;
            
            Effect_exposer.def( 
                "render_effect"
                , render_effect_function_type( &::xfx::Effect::RenderEffect )
                , ( bp::arg("render_fn"), bp::arg("flags")=(long unsigned int)(0) ) );
        
        }
        { //property "dx_effect_ptr"[fget=::xfx::Effect::DXEffectPtr]
        
            typedef ::boost::shared_ptr<ID3DXEffect> const & ( ::xfx::Effect::*fget )(  ) const;
            
            Effect_exposer.add_property( 
                "dx_effect_ptr"
                , bp::make_function( 
                      fget( &::xfx::Effect::DXEffectPtr )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , "get property, built on top of \"boost::shared_ptr<ID3DXEffect> const & xfx::Effect::DXEffectPtr() const [member function]\"" );
        
        }
        { //property "cache"[fget=::xfx::Effect::Cache]
        
            typedef ::xfx::Cache<xfx::Effect> & ( *fget )(  );
            
            Effect_exposer.add_static_property( 
                "cache"
                , bp::make_function( 
                      fget( &::xfx::Effect::Cache )
                    , bp::return_value_policy< bp::reference_existing_object >() )  );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::Effect const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::Effect >, boost::shared_ptr< xfx::Effect const > >( );
    }

}
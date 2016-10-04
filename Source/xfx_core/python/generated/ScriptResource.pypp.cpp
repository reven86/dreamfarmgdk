// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "scriptresource.pypp.hpp"

namespace bp = boost::python;

struct ScriptResource_wrapper : xfx::ScriptResource, bp::wrapper< xfx::ScriptResource > {

    ScriptResource_wrapper(xfx::ScriptResource const & arg )
    : xfx::ScriptResource( arg )
      , bp::wrapper< xfx::ScriptResource >(){
        // copy constructor
        
    }

    ScriptResource_wrapper(char const * name )
    : xfx::ScriptResource( name )
      , bp::wrapper< xfx::ScriptResource >(){
        // constructor
    
    }

    virtual ::HRESULT LoadMemory( void const * pmemory, long unsigned int filelen ) {
        if( bp::override func_load_memory = this->get_override( "load_memory" ) )
            return func_load_memory( pmemory, filelen );
        else
            return this->xfx::ScriptResource::LoadMemory( pmemory, filelen );
    }
    
    
    ::HRESULT default_load_memory( void const * pmemory, long unsigned int filelen ) {
        return xfx::ScriptResource::LoadMemory( pmemory, filelen );
    }

    virtual ::HRESULT LoadFile( ::xfx::String const & filename ) {
        if( bp::override func_load_file = this->get_override( "load_file" ) )
            return func_load_file( filename );
        else
            return this->xfx::Resource::LoadFile( filename );
    }
    
    
    ::HRESULT default_load_file( ::xfx::String const & filename ) {
        return xfx::Resource::LoadFile( filename );
    }

};

void register_ScriptResource_class(){

    { //::xfx::ScriptResource
        typedef bp::class_< ScriptResource_wrapper, bp::bases< xfx::Script< std::string >, xfx::Resource > > ScriptResource_exposer_t;
        ScriptResource_exposer_t ScriptResource_exposer = ScriptResource_exposer_t( "ScriptResource", bp::init< char const * >(( bp::arg("name") )) );
        bp::scope ScriptResource_scope( ScriptResource_exposer );
        bp::implicitly_convertible< char const *, xfx::ScriptResource >();
        { //::xfx::ScriptResource::LoadMemory
        
            typedef ::HRESULT ( ::xfx::ScriptResource::*load_memory_function_type )( void const *,long unsigned int ) ;
            typedef ::HRESULT ( ScriptResource_wrapper::*default_load_memory_function_type )( void const *,long unsigned int ) ;
            
            ScriptResource_exposer.def( 
                "load_memory"
                , load_memory_function_type(&::xfx::ScriptResource::LoadMemory)
                , default_load_memory_function_type(&ScriptResource_wrapper::default_load_memory)
                , ( bp::arg("pmemory"), bp::arg("filelen") ) );
        
        }
        { //::xfx::Resource::LoadFile
        
            typedef ::HRESULT ( ::xfx::Resource::*load_file_function_type )( ::xfx::String const & ) ;
            typedef ::HRESULT ( ScriptResource_wrapper::*default_load_file_function_type )( ::xfx::String const & ) ;
            
            ScriptResource_exposer.def( 
                "load_file"
                , load_file_function_type(&::xfx::Resource::LoadFile)
                , default_load_file_function_type(&ScriptResource_wrapper::default_load_file)
                , ( bp::arg("filename") ) );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::ScriptResource const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::ScriptResource >, boost::shared_ptr< xfx::ScriptResource const > >( );
    }

}
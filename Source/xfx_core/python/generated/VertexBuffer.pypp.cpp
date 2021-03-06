// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "vertexbuffer.pypp.hpp"

namespace bp = boost::python;

void register_VertexBuffer_class(){

    { //::xfx::VertexBuffer
        typedef bp::class_< xfx::VertexBuffer, boost::noncopyable > VertexBuffer_exposer_t;
        VertexBuffer_exposer_t VertexBuffer_exposer = VertexBuffer_exposer_t( "VertexBuffer", bp::init< >() );
        bp::scope VertexBuffer_scope( VertexBuffer_exposer );
        { //::xfx::VertexBuffer::Create
        
            typedef ::HRESULT ( ::xfx::VertexBuffer::*create_function_type )( unsigned int const &,unsigned int const &,int const &,bool const &,int const & ) ;
            
            VertexBuffer_exposer.def( 
                "create"
                , create_function_type( &::xfx::VertexBuffer::Create )
                , ( bp::arg("vertexsize"), bp::arg("numvertices"), bp::arg("fvf"), bp::arg("dynamic"), bp::arg("additional_usage")=0 ) );
        
        }
        { //::xfx::VertexBuffer::Destroy
        
            typedef void ( ::xfx::VertexBuffer::*destroy_function_type )(  ) ;
            
            VertexBuffer_exposer.def( 
                "destroy"
                , destroy_function_type( &::xfx::VertexBuffer::Destroy ) );
        
        }
        { //::xfx::VertexBuffer::InvalidateCallback
        
            typedef void ( ::xfx::VertexBuffer::*invalidate_callback_function_type )( bool ) ;
            
            VertexBuffer_exposer.def( 
                "invalidate_callback"
                , invalidate_callback_function_type( &::xfx::VertexBuffer::InvalidateCallback )
                , ( bp::arg("invalidate") ) );
        
        }
        { //::xfx::VertexBuffer::SetStream
        
            typedef void ( ::xfx::VertexBuffer::*set_stream_function_type )( int const & ) const;
            
            VertexBuffer_exposer.def( 
                "set_stream"
                , set_stream_function_type( &::xfx::VertexBuffer::SetStream )
                , ( bp::arg("stream") ) );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::VertexBuffer const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::VertexBuffer >, boost::shared_ptr< xfx::VertexBuffer const > >( );
    }

}

// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "baseparticlesystem.pypp.hpp"

namespace bp = boost::python;

struct BaseParticleSystem_wrapper : xfx::BaseParticleSystem, bp::wrapper< xfx::BaseParticleSystem > {

    BaseParticleSystem_wrapper( )
    : xfx::BaseParticleSystem( )
      , bp::wrapper< xfx::BaseParticleSystem >(){
        // null constructor
    
    }

    virtual void Render( ::xfx::ARGB const & col ) const {
        bp::override func_render = this->get_override( "render" );
        func_render( boost::ref(col) );
    }

    virtual void Update( ::DWORD mspf ){
        bp::override func_update = this->get_override( "update" );
        func_update( mspf );
    }

};

void register_BaseParticleSystem_class(){

    { //::xfx::BaseParticleSystem
        typedef bp::class_< BaseParticleSystem_wrapper, bp::bases< xfx::Transformable3D >, boost::noncopyable > BaseParticleSystem_exposer_t;
        BaseParticleSystem_exposer_t BaseParticleSystem_exposer = BaseParticleSystem_exposer_t( "BaseParticleSystem", bp::init< >() );
        bp::scope BaseParticleSystem_scope( BaseParticleSystem_exposer );
        { //::xfx::BaseParticleSystem::EmitterTransformation
        
            typedef ::xfx::Mat4 & ( ::xfx::BaseParticleSystem::*emitter_transformation_function_type )(  ) ;
            
            BaseParticleSystem_exposer.def( 
                "emitter_transformation"
                , emitter_transformation_function_type( &::xfx::BaseParticleSystem::EmitterTransformation )
                , bp::return_internal_reference< >() );
        
        }
        { //::xfx::BaseParticleSystem::EmitterTransformation
        
            typedef ::xfx::Mat4 const & ( ::xfx::BaseParticleSystem::*emitter_transformation_function_type )(  ) const;
            
            BaseParticleSystem_exposer.def( 
                "emitter_transformation"
                , emitter_transformation_function_type( &::xfx::BaseParticleSystem::EmitterTransformation )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::xfx::BaseParticleSystem::Render
        
            typedef void ( ::xfx::BaseParticleSystem::*render_function_type )( ::xfx::ARGB const & ) const;
            
            BaseParticleSystem_exposer.def( 
                "render"
                , bp::pure_virtual( render_function_type(&::xfx::BaseParticleSystem::Render) )
                , ( bp::arg("col") ) );
        
        }
        { //::xfx::BaseParticleSystem::StopSpawn
        
            typedef void ( ::xfx::BaseParticleSystem::*stop_spawn_function_type )(  ) ;
            
            BaseParticleSystem_exposer.def( 
                "stop_spawn"
                , stop_spawn_function_type( &::xfx::BaseParticleSystem::StopSpawn ) );
        
        }
        { //::xfx::BaseParticleSystem::Update
        
            typedef void ( ::xfx::BaseParticleSystem::*update_function_type )( ::DWORD ) ;
            
            BaseParticleSystem_exposer.def( 
                "update"
                , bp::pure_virtual( update_function_type(&::xfx::BaseParticleSystem::Update) )
                , ( bp::arg("mspf") ) );
        
        }
        { //property "scaler"[fget=::xfx::BaseParticleSystem::GetScaler, fset=::xfx::BaseParticleSystem::SetScaler]
        
            typedef float const & ( ::xfx::BaseParticleSystem::*fget )(  ) const;
            typedef void ( ::xfx::BaseParticleSystem::*fset )( float const & ) ;
            
            BaseParticleSystem_exposer.add_property( 
                "scaler"
                , bp::make_function( 
                      fget( &::xfx::BaseParticleSystem::GetScaler )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::xfx::BaseParticleSystem::SetScaler )
                , "get\\set property, built on top of \"float const & xfx::BaseParticleSystem::GetScaler() const [member function]\" and \"void xfx::BaseParticleSystem::SetScaler(float const & scale) [member function]\"" );
        
        }
        { //property "max_particles_count"[fget=::xfx::BaseParticleSystem::GetMaxParticlesCount]
        
            typedef unsigned int ( ::xfx::BaseParticleSystem::*fget )(  ) const;
            
            BaseParticleSystem_exposer.add_property( 
                "max_particles_count"
                , fget( &::xfx::BaseParticleSystem::GetMaxParticlesCount )
                , "get property, built on top of \"unsigned int xfx::BaseParticleSystem::GetMaxParticlesCount() const [member function]\"" );
        
        }
        { //property "alive_count"[fget=::xfx::BaseParticleSystem::AliveCount]
        
            typedef unsigned int ( ::xfx::BaseParticleSystem::*fget )(  ) const;
            
            BaseParticleSystem_exposer.add_property( 
                "alive_count"
                , fget( &::xfx::BaseParticleSystem::AliveCount )
                , "get property, built on top of \"unsigned int xfx::BaseParticleSystem::AliveCount() const [member function]\"" );
        
        }
        { //property "alive"[fget=::xfx::BaseParticleSystem::IsAlive]
        
            typedef bool ( ::xfx::BaseParticleSystem::*fget )(  ) const;
            
            BaseParticleSystem_exposer.add_property( 
                "alive"
                , fget( &::xfx::BaseParticleSystem::IsAlive )
                , "get property, built on top of \"bool xfx::BaseParticleSystem::IsAlive() const [member function]\"" );
        
        }
        { //property "stopped"[fget=::xfx::BaseParticleSystem::IsStopped]
        
            typedef bool ( ::xfx::BaseParticleSystem::*fget )(  ) const;
            
            BaseParticleSystem_exposer.add_property( 
                "stopped"
                , fget( &::xfx::BaseParticleSystem::IsStopped )
                , "get property, built on top of \"bool xfx::BaseParticleSystem::IsStopped() const [member function]\"" );
        
        }
        { //property "aabb"[fget=::xfx::BaseParticleSystem::AABB]
        
            typedef ::xfx::Primitives::AABB const & ( ::xfx::BaseParticleSystem::*fget )(  ) const;
            
            BaseParticleSystem_exposer.add_property( 
                "aabb"
                , bp::make_function( 
                      fget( &::xfx::BaseParticleSystem::AABB )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , "get property, built on top of \"xfx::Primitives::AABB const & xfx::BaseParticleSystem::AABB() const [member function]\"" );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::BaseParticleSystem const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::BaseParticleSystem >, boost::shared_ptr< xfx::BaseParticleSystem const > >( );
    }

}

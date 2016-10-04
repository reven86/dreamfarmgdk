// This file has been generated by Py++.

#include "xfx.h"
#include "boost/python.hpp"
#include "xfx_py_bindings.h"
#include "particlesystem.pypp.hpp"

namespace bp = boost::python;

struct ParticleSystem_wrapper : xfx::ParticleSystem, bp::wrapper< xfx::ParticleSystem > {

    ParticleSystem_wrapper(xfx::ParticleSystem const & arg )
    : xfx::ParticleSystem( arg )
      , bp::wrapper< xfx::ParticleSystem >(){
        // copy constructor
        
    }

    ParticleSystem_wrapper( )
    : xfx::ParticleSystem( )
      , bp::wrapper< xfx::ParticleSystem >(){
        // null constructor
    
    }

    virtual void Render( ::xfx::ARGB const & modulator=xfx::ARGB(255, 255, 255, 255) ) const  {
        if( bp::override func_render = this->get_override( "render" ) )
            func_render( boost::ref(modulator) );
        else
            this->xfx::ParticleSystem::Render( boost::ref(modulator) );
    }
    
    
    void default_render( ::xfx::ARGB const & modulator=xfx::ARGB(255, 255, 255, 255) ) const  {
        xfx::ParticleSystem::Render( boost::ref(modulator) );
    }

    virtual void Update( ::DWORD mspf ) {
        if( bp::override func_update = this->get_override( "update" ) )
            func_update( mspf );
        else
            this->xfx::ParticleSystem::Update( mspf );
    }
    
    
    void default_update( ::DWORD mspf ) {
        xfx::ParticleSystem::Update( mspf );
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

    virtual ::HRESULT LoadMemory( void const * pmemory, long unsigned int filelen ) {
        if( bp::override func_load_memory = this->get_override( "load_memory" ) )
            return func_load_memory( pmemory, filelen );
        else
            return this->xfx::ScriptResource::LoadMemory( pmemory, filelen );
    }
    
    
    ::HRESULT default_load_memory( void const * pmemory, long unsigned int filelen ) {
        return xfx::ScriptResource::LoadMemory( pmemory, filelen );
    }

};

void register_ParticleSystem_class(){

    { //::xfx::ParticleSystem
        typedef bp::class_< ParticleSystem_wrapper, bp::bases< xfx::BaseParticleSystem, xfx::ScriptResource > > ParticleSystem_exposer_t;
        ParticleSystem_exposer_t ParticleSystem_exposer = ParticleSystem_exposer_t( "ParticleSystem", bp::init< >() );
        bp::scope ParticleSystem_scope( ParticleSystem_exposer );
        { //::xfx::ParticleSystem::AddSubSystem
        
            typedef void ( ::xfx::ParticleSystem::*add_sub_system_function_type )( ::xfx::ParticleSubSystem const & ) ;
            
            ParticleSystem_exposer.def( 
                "add_sub_system"
                , add_sub_system_function_type( &::xfx::ParticleSystem::AddSubSystem )
                , ( bp::arg("ps") ) );
        
        }
        { //::xfx::ParticleSystem::GetParticle
        
            typedef ::xfx::Particle * ( ::xfx::ParticleSystem::*get_particle_function_type )( unsigned int ) ;
            
            ParticleSystem_exposer.def( 
                "get_particle"
                , get_particle_function_type( &::xfx::ParticleSystem::GetParticle )
                , ( bp::arg("particle_index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::xfx::ParticleSystem::GetParticle
        
            typedef ::xfx::Particle const * ( ::xfx::ParticleSystem::*get_particle_function_type )( unsigned int ) const;
            
            ParticleSystem_exposer.def( 
                "get_particle"
                , get_particle_function_type( &::xfx::ParticleSystem::GetParticle )
                , ( bp::arg("particle_index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::xfx::ParticleSystem::GetSubSystem
        
            typedef ::xfx::ParticleSubSystem * ( ::xfx::ParticleSystem::*get_sub_system_function_type )( unsigned int ) ;
            
            ParticleSystem_exposer.def( 
                "get_sub_system"
                , get_sub_system_function_type( &::xfx::ParticleSystem::GetSubSystem )
                , ( bp::arg("index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::xfx::ParticleSystem::GetSubSystem
        
            typedef ::xfx::ParticleSubSystem const * ( ::xfx::ParticleSystem::*get_sub_system_function_type )( unsigned int ) const;
            
            ParticleSystem_exposer.def( 
                "get_sub_system"
                , get_sub_system_function_type( &::xfx::ParticleSystem::GetSubSystem )
                , ( bp::arg("index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::xfx::ParticleSystem::RemoveSubSystem
        
            typedef void ( ::xfx::ParticleSystem::*remove_sub_system_function_type )( unsigned int ) ;
            
            ParticleSystem_exposer.def( 
                "remove_sub_system"
                , remove_sub_system_function_type( &::xfx::ParticleSystem::RemoveSubSystem )
                , ( bp::arg("index") ) );
        
        }
        { //::xfx::ParticleSystem::Render
        
            typedef void ( ::xfx::ParticleSystem::*render_function_type )( ::xfx::ARGB const & ) const;
            typedef void ( ParticleSystem_wrapper::*default_render_function_type )( ::xfx::ARGB const & ) const;
            
            ParticleSystem_exposer.def( 
                "render"
                , render_function_type(&::xfx::ParticleSystem::Render)
                , default_render_function_type(&ParticleSystem_wrapper::default_render)
                , ( bp::arg("modulator")=xfx::ARGB(255, 255, 255, 255) ) );
        
        }
        { //::xfx::ParticleSystem::Reset
        
            typedef void ( ::xfx::ParticleSystem::*reset_function_type )(  ) ;
            
            ParticleSystem_exposer.def( 
                "reset"
                , reset_function_type( &::xfx::ParticleSystem::Reset ) );
        
        }
        { //::xfx::ParticleSystem::Update
        
            typedef void ( ::xfx::ParticleSystem::*update_function_type )( ::DWORD ) ;
            typedef void ( ParticleSystem_wrapper::*default_update_function_type )( ::DWORD ) ;
            
            ParticleSystem_exposer.def( 
                "update"
                , update_function_type(&::xfx::ParticleSystem::Update)
                , default_update_function_type(&ParticleSystem_wrapper::default_update)
                , ( bp::arg("mspf") ) );
        
        }
        { //::xfx::Resource::LoadFile
        
            typedef ::HRESULT ( ::xfx::Resource::*load_file_function_type )( ::xfx::String const & ) ;
            typedef ::HRESULT ( ParticleSystem_wrapper::*default_load_file_function_type )( ::xfx::String const & ) ;
            
            ParticleSystem_exposer.def( 
                "load_file"
                , load_file_function_type(&::xfx::Resource::LoadFile)
                , default_load_file_function_type(&ParticleSystem_wrapper::default_load_file)
                , ( bp::arg("filename") ) );
        
        }
        { //::xfx::ScriptResource::LoadMemory
        
            typedef ::HRESULT ( ::xfx::ScriptResource::*load_memory_function_type )( void const *,long unsigned int ) ;
            typedef ::HRESULT ( ParticleSystem_wrapper::*default_load_memory_function_type )( void const *,long unsigned int ) ;
            
            ParticleSystem_exposer.def( 
                "load_memory"
                , load_memory_function_type(&::xfx::ScriptResource::LoadMemory)
                , default_load_memory_function_type(&ParticleSystem_wrapper::default_load_memory)
                , ( bp::arg("pmemory"), bp::arg("filelen") ) );
        
        }
        { //property "flat"[fget=::xfx::ParticleSystem::IsFlat, fset=::xfx::ParticleSystem::SetFlat]
        
            typedef bool ( ::xfx::ParticleSystem::*fget )(  ) const;
            typedef void ( ::xfx::ParticleSystem::*fset )( bool ) ;
            
            ParticleSystem_exposer.add_property( 
                "flat"
                , fget( &::xfx::ParticleSystem::IsFlat )
                , fset( &::xfx::ParticleSystem::SetFlat )
                , "get\\set property, built on top of \"bool xfx::ParticleSystem::IsFlat() const [member function]\" and \"void xfx::ParticleSystem::SetFlat(bool flat) [member function]\"" );
        
        }
        { //property "high_precision_update"[fget=::xfx::ParticleSystem::IsHighPrecisionUpdate, fset=::xfx::ParticleSystem::SetHighPrecisionUpdate]
        
            typedef bool ( ::xfx::ParticleSystem::*fget )(  ) const;
            typedef void ( ::xfx::ParticleSystem::*fset )( bool ) ;
            
            ParticleSystem_exposer.add_property( 
                "high_precision_update"
                , fget( &::xfx::ParticleSystem::IsHighPrecisionUpdate )
                , fset( &::xfx::ParticleSystem::SetHighPrecisionUpdate )
                , "get\\set property, built on top of \"bool xfx::ParticleSystem::IsHighPrecisionUpdate() const [member function]\" and \"void xfx::ParticleSystem::SetHighPrecisionUpdate(bool hi_prec) [member function]\"" );
        
        }
        { //property "update_period"[fget=::xfx::ParticleSystem::GetUpdatePeriod, fset=::xfx::ParticleSystem::SetUpdatePeriod]
        
            typedef int ( ::xfx::ParticleSystem::*fget )(  ) const;
            typedef void ( ::xfx::ParticleSystem::*fset )( int ) ;
            
            ParticleSystem_exposer.add_property( 
                "update_period"
                , fget( &::xfx::ParticleSystem::GetUpdatePeriod )
                , fset( &::xfx::ParticleSystem::SetUpdatePeriod )
                , "get\\set property, built on top of \"int xfx::ParticleSystem::GetUpdatePeriod() const [member function]\" and \"void xfx::ParticleSystem::SetUpdatePeriod(int period) [member function]\"" );
        
        }
        { //property "sub_systems_count"[fget=::xfx::ParticleSystem::GetSubSystemsCount]
        
            typedef unsigned int ( ::xfx::ParticleSystem::*fget )(  ) const;
            
            ParticleSystem_exposer.add_property( 
                "sub_systems_count"
                , fget( &::xfx::ParticleSystem::GetSubSystemsCount )
                , "get property, built on top of \"unsigned int xfx::ParticleSystem::GetSubSystemsCount() const [member function]\"" );
        
        }
        { //property "cache"[fget=::xfx::ParticleSystem::Cache]
        
            typedef ::xfx::Cache<xfx::ParticleSystem> & ( *fget )(  );
            
            ParticleSystem_exposer.add_static_property( 
                "cache"
                , bp::make_function( 
                      fget( &::xfx::ParticleSystem::Cache )
                    , bp::return_value_policy< bp::reference_existing_object >() )  );
        
        }
        bp::register_ptr_to_python< boost::shared_ptr< xfx::ParticleSystem const > >( );
        bp::implicitly_convertible< boost::shared_ptr< xfx::ParticleSystem >, boost::shared_ptr< xfx::ParticleSystem const > >( );
    }

}
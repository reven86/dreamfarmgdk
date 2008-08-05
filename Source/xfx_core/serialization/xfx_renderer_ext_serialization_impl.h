//  File xfx_renderer_ext_serialization_impl.h                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_renderer_ext_serialization_impl.h 
 *	\brief Serialization implementation for Extensions for Renderer library.
 */




_XFX_BEGIN




//
// Particle
//

template< class _Archive >
void Particle::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( position );
	ar & BOOST_SERIALIZATION_NVP( velocity );
	ar & BOOST_SERIALIZATION_NVP( size );
	ar & BOOST_SERIALIZATION_NVP( acceleration );
	ar & BOOST_SERIALIZATION_NVP( angle );
	ar & BOOST_SERIALIZATION_NVP( spin );
	ar & BOOST_SERIALIZATION_NVP( motionrand );
	ar & BOOST_SERIALIZATION_NVP( color.dword );
	ar & BOOST_SERIALIZATION_NVP( spawn_count );

	ar & BOOST_SERIALIZATION_NVP( start_size );

	ar & BOOST_SERIALIZATION_NVP( lifetime );
	ar & BOOST_SERIALIZATION_NVP( localtime );

}



//
// ParticleSubSystem
//

template< class _Archive >
void ParticleSubSystem::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( emitter_pos );
	ar & BOOST_SERIALIZATION_NVP( accelerate_dir );
	ar & BOOST_SERIALIZATION_NVP( velocity_dir );
	ar & BOOST_SERIALIZATION_NVP( volume );
	ar & BOOST_SERIALIZATION_NVP( max_particles );
	ar & BOOST_SERIALIZATION_NVP( spawn_count );

	ar & BOOST_SERIALIZATION_NVP( lifetime );
	ar & BOOST_SERIALIZATION_NVP( size );
	ar & BOOST_SERIALIZATION_NVP( velocity );
	ar & BOOST_SERIALIZATION_NVP( acceleration );
	ar & BOOST_SERIALIZATION_NVP( emissionrange );
	ar & BOOST_SERIALIZATION_NVP( spin );
	ar & BOOST_SERIALIZATION_NVP( motionrand );
	ar & BOOST_SERIALIZATION_NVP( particleangle );
	ar & BOOST_SERIALIZATION_NVP( align_to_motion );

	ar & BOOST_SERIALIZATION_NVP( life_variation );
	ar & BOOST_SERIALIZATION_NVP( size_variation );
	ar & BOOST_SERIALIZATION_NVP( velocity_variation );
	ar & BOOST_SERIALIZATION_NVP( accelerate_variation );
	ar & BOOST_SERIALIZATION_NVP( spin_variation );
	ar & BOOST_SERIALIZATION_NVP( motionrand_variation );
	ar & BOOST_SERIALIZATION_NVP( particleangle_variation );
	ar & BOOST_SERIALIZATION_NVP( starttime_variation );

	ar & BOOST_SERIALIZATION_NVP( size_envelope );
	ar & BOOST_SERIALIZATION_NVP( velocity_envelope );
	ar & BOOST_SERIALIZATION_NVP( acceleration_envelope );
	ar & BOOST_SERIALIZATION_NVP( spin_envelope );
	ar & BOOST_SERIALIZATION_NVP( motionrand_envelope );
	ar & BOOST_SERIALIZATION_NVP( colors_envelope );

	ar & BOOST_SERIALIZATION_NVP( shader_ptr );
};




//
// BaseParticleSystem
//

template< class _Archive >
void BaseParticleSystem::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Transformable3D );

	ar & BOOST_SERIALIZATION_NVP( mMaxParticles );
	ar & BOOST_SERIALIZATION_NVP( mEmitterTransformation );
	ar & BOOST_SERIALIZATION_NVP( mIsStopped );
	ar & BOOST_SERIALIZATION_NVP( mAliveCount );
	ar & BOOST_SERIALIZATION_NVP( mMaxParticleSize );
	ar & BOOST_SERIALIZATION_NVP( mAABB );
	ar & BOOST_SERIALIZATION_NVP( mScaler );
};



//
// ParticleSystem
//

template< class _Archive >
void ParticleSystem::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( BaseParticleSystem );
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( ScriptResource );

	ar & BOOST_SERIALIZATION_NVP( mSystems );
	ar & BOOST_SERIALIZATION_NVP( mParticles );
	ar & BOOST_SERIALIZATION_NVP( mInvisibleTimer );
	ar & BOOST_SERIALIZATION_NVP( mFramesToUpdate );
	ar & BOOST_SERIALIZATION_NVP( mFlags );
	ar & BOOST_SERIALIZATION_NVP( mUpdatePeriod );
	ar & BOOST_SERIALIZATION_NVP( mUpdateTimer );
};



_XFX_END
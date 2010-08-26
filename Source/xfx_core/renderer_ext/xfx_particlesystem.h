//  File xfx_particlesystem.h                                                 |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_particlesystem.h 
 *	\brief Describe particle system related classes and structs.
 */

_XFX_BEGIN








/*! \struct Particle xfx_particlesystem.h "renderer_ext/xfx_particlesystem.h"
 *	\brief %Particle structure.
 *	\ingroup RenderExtGroup
 *
 *	Simple 3D particle structure.
 *	
 *	\see ParticleSystem, ParticleSubSystem
 *	\author Andrew "RevEn" Karpushin
 */

struct Particle
{
	Vec3						position;						//!< Position.
	Vec3						velocity;						//!< Velocity.
	float						size;							//!< Size.
	float						acceleration;					//!< Acceleration.
	Math::BigAngle				angle;							//!< Roll angle.
	Math::BigAngle				spin;							//!< Angle speed.
	float						motionrand;						//!< Motion randomness.
	ARGB						color;							//!< Particle color.
	int							spawn_count;					//!< Current spawn count.

	float						start_size;						//!< Start size.

	DWORD						lifetime;						//!< Total life time.
	DWORD						localtime;						//!< Local non-normalized time.

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );
};




/*! \struct ParticleSubSystem xfx_particlesystem.h "renderer_ext/xfx_particlesystem.h"
 *	\brief %Particle subsystem structure.
 *	\ingroup RenderExtGroup
 *
 *	Stores particles with same type.
 *	
 *	\see ParticleSystem, Particle
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section particlesubsystem_script ParticleSubSystem syntax
 *
 *	Example:
 *
 *	\verbatim
subsystem
{
	shader				"textures\sfx\explosion.shader"

	emitter_pos			0.0 0.0 0.0
	accelerate_dir			0.0 0.0 0.0
	velocity_dir			0.0 0.0 1.0
	volume				5.0 5.0 5.0
	max_particles			20
	spawn_count			1

	lifetime			3000
	size				4.5
	velocity			3
	acceleration			0
	emissionrange			180
	spin				0
	motionrand			3
	particleangle			0

	life_variation			500
	size_variation			0.75
	velocity_variation		0.75
	accelerate_variation		0
	spin_variation			90
	motionrand_variation		2
	particleangle_variation		360
	starttime_variation		0.0

	size_envelope
	{
		0	:	1.0
		255	:	1.5
	}

	velocity_envelope
	{
		0	:	15.0
		255	:	0.5
	}

	acceleration_envelope
	{
		0	:	1.0
		255	:	1.0
	}

	spin_envelope
	{
		0	:	1.0
		255	:	1.0
	}

	motionrand_envelope
	{
		0	:	1.0
		255	:	1.0
	}

	colors_envelope
	{
		0	:	0xff000000
		25	:	0xffffffff
		127	:	0xffff6060
		255	:	0xff000000
	}
}
	\endverbatim
 *	
 *	Description:
 *
 *	Keywords has same meaning as in ParticleSubSystem members.
 *
 *	@}
 */

struct ParticleSubSystem : public Script< String >
{
	Vec3						emitter_pos;					//!< Emitter position.
	Vec3						accelerate_dir;					//!< \b Normalized accelerate direction.
	Vec3						velocity_dir;					//!< \b Normalized velocity direction.
	Vec3						volume;							//!< Spawn volume.
	unsigned					max_particles;					//!< Max particles.
	int							spawn_count;					//!< Maximum spawn count for each particle.

	DWORD						lifetime;						//!< Base particle lifetime.
	float						size;							//!< Base particle size.
	float						velocity;						//!< Base particle speed.
	float						acceleration;					//!< Base particle acceleration.
	Math::Angle					emissionrange;					//!< Emitter emission range from 0 to pi.
	Math::BigAngle				spin;							//!< Base particle spin.
	float						motionrand;						//!< Base particle motion randomness.
	Math::BigAngle				particleangle;					//!< Base particle angle.
	bool						align_to_motion;				//!< Will particle be aligned to motion?

	DWORD						life_variation;					//!< Particle lifetime variation.
	float						size_variation;					//!< Particle size variation.
	float						velocity_variation;				//!< Particle speed variation.
	float						accelerate_variation;			//!< Particle acceleration variation.
	Math::BigAngle				spin_variation;					//!< Particle spin variation.
	float						motionrand_variation;			//!< Particle motion randomness variation.
	Math::BigAngle				particleangle_variation;		//!< Particle angle variation.
	float						starttime_variation;			//!< Particle start time variation.

	Envelope< float >			size_envelope;					//!< Size envelope.
	Envelope< float >			velocity_envelope;				//!< Speed envelope.
	Envelope< float >			acceleration_envelope;			//!< Acceleration envelope.
	Envelope< float >			spin_envelope;					//!< Spin envelope.
	Envelope< float >			motionrand_envelope;			//!< Motion randomness envelope.
	Envelope< ARGB >			colors_envelope;				//!< Color envelope.

	boost::shared_ptr< const class Shader >	shader_ptr;			//!< Shader pointer.

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	ParticleSubSystem									( );

	// copy constructor and assignment operator are fine.

	//! Update particle by small amount of time.
	void						UpdateParticle			( Particle& p, DWORD mspf ) const;

	//! Spawn new particle.
	void						SpawnParticle			( Particle& p, const Mat4& transform ) const;

protected:
	virtual HRESULT				PrepareParsing			( );
	virtual HRESULT				AfterParsing			( );

	HRESULT						ParseShader				( String::size_type& pos, const String& str );

	template< class T >
	T RandValue											( const T& v1, const T& v2 ) const
	{
		return v1 + ( v2 - v1 ) * math_frand( );
	};

	HRESULT						ParseShader				( );
};





/*! \struct BaseParticleSystem xfx_particlesystem.h "renderer_ext/xfx_particlesystem.h"
 *	\brief Base particle system class.
 *	\ingroup RenderExtGroup
 *
 *	\see ParticleSystem, ParticleSubSystem
 *	\author Andrew "RevEn" Karpushin
 */

class BaseParticleSystem : public Transformable3D
{
	friend class boost::serialization::access;

protected:
	unsigned								mMaxParticles;						//!< Total particles count.
	Mat4									mEmitterTransformation;				//!< Emitter transformation.

	bool									mIsStopped;							//!< Is spawn stopped?
	unsigned								mAliveCount;						//!< Alive particles count.
	float									mMaxParticleSize;					//!< Max particle size.
	float									mScaler;							//!< Particle system scale factor.

	Primitives::AABB						mAABB;								//!< Closest AABB.

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	BaseParticleSystem														( );
	virtual ~BaseParticleSystem												( ) { };

	// compiler-generated copy constructor and assignment operator are fine

	//! Get/Set emitter transformation.
	Mat4&							EmitterTransformation					( ) { return mEmitterTransformation; };

	//! Get emitter transformation.
	const Mat4&						EmitterTransformation					( ) const { return mEmitterTransformation; };

	//! Render particle system.
	virtual void					Render									( const ARGB& col ) const = 0;

	//! Update particle system.
	virtual void					Update									( DWORD mspf ) = 0;

	//! Stop spawn particles.
	void							StopSpawn								( ) { mIsStopped = true; };

	//! Get max particles count.
	unsigned						GetMaxParticlesCount					( ) const { return mMaxParticles; };

	//! Get particles alive count.
	unsigned						AliveCount								( ) const { return mAliveCount; };

	//! Is any particle alive?
	bool							IsAlive									( ) const { return mAliveCount != 0; };

	//! Is emitter stopped?
	bool							IsStopped								( ) const { return mIsStopped ;};

	//! Get a local closest AABB.
	const Primitives::AABB&			AABB									( ) const { return mAABB; };

	//! Get scale factor.
	const float&					GetScaler								( ) const { return mScaler; };

	//! Set scale factor.
	void							SetScaler								( const float& scale ) { mScaler = scale; };

protected:
	//! Test frustum culling.
	bool							TestFrustumCulling						( const class Viewer * viewer, const Mat4& transform ) const;
};




/*! \struct ParticleSystem xfx_particlesystem.h "renderer_ext/xfx_particlesystem.h"
 *	\brief Particle system class.
 *	\ingroup RenderExtGroup
 *
 *	Particle system based on texture sprites.
 *
 *	\see ParticleSystem, ParticleSubSystem
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section particlesystem_script ParticleSystem file syntax
 *
 *	Example:
 *
 *	\verbatim
particle_system
{
	is_2d					1
	update_period			250
	hi_percision_update		1

	subsystem
	{
		< subsystem parameters >
	}

	< more subsystems >
}
	\endverbatim
 *	
 *	Description:
 *
 *	\b is_2d				- Is 2D space particle system ( default: false ).\n
 *	\b update_period		- Update period, how often particle system will be updates ( default : 250 means update no more than 40 times per second ). \n
 *	\b hi_percision_update	- High precision update. If frame delta time will be more than update_period then update will be devided into few steps ( default: false ). \n
 *	\b subsystem			- ParticleSubSystem description.
 *
 *	@}
 */

class ParticleSystem : public BaseParticleSystem, public ScriptResource
{
	friend class boost::serialization::access;

	// list here is more better than vector, since ParticleSubSystem is large and their count is small.
	typedef std::list< const ParticleSubSystem >	SystemsType;
	boost::shared_ptr< SystemsType >				mSystems;			// they are same for all clones

	typedef std::vector< Particle >					ParticlesType;
	ParticlesType									mParticles;

	static Cache< ParticleSystem >					msCache;

	//! Time since last Render method was called.
	mutable DWORD									mInvisibleTimer;
	int												mFramesToUpdate;

	enum EFlags
	{
		EFL_2D						= ( 1 << 0 ),
		EFL_HIGH_PRECISION			= ( 1 << 1 )
	};

	DWORD											mFlags;

	int												mUpdatePeriod;
	int												mUpdateTimer;

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	ParticleSystem															( );
	virtual ~ParticleSystem													( );

	// compiler-generated copy constructor and assignment operator are fine

	//
	// Subsystem management.
	// Note: subsystem management reflects on ALL clones, made from this one.
	//

	//! Get subsystems count.
	unsigned						GetSubSystemsCount						( ) const { return mSystems->size( ); };

	//! Add new subsystem to particle system (add as last subsystem).
	void							AddSubSystem							( const ParticleSubSystem& ps );

	/*! \brief Get subsystem (nonconst version).
	 *
	 *	\note Use it carefully because some members (like max_particles) should not be altered
	 *
	 *	\return NULL if index is invalid.
	 */
	ParticleSubSystem *				GetSubSystem							( unsigned index );

	/*! \brief Get subsystem (const version).
	 *
	 *	\note Use it carefully because some members (like max_particles) should not be altered
	 *
	 *	\return NULL if index is invalid.
	 */
	const ParticleSubSystem *		GetSubSystem							( unsigned index ) const;

	//! Remove subsystem by index.
	void							RemoveSubSystem							( unsigned index );

	//! Get particle (nonconst version).
	Particle *						GetParticle								( unsigned particle_index );

	//! Get particle (const version).
	const Particle *				GetParticle								( unsigned particle_index ) const;


	//
	// Behavior
	//

	//! Is particle system 'flat'?
	bool							IsFlat									( ) const { return ( mFlags & EFL_2D ) != 0; };

	//! Set particle system 'flat' mode.
	void							SetFlat									( bool flat ) { if( flat ) mFlags |= EFL_2D; else mFlags &= ~EFL_2D; };

	//! Is high precision update used?
	bool							IsHighPrecisionUpdate					( ) const { return ( mFlags & EFL_HIGH_PRECISION ) != 0; };

	//! Set high precision update.
	void							SetHighPrecisionUpdate					( bool hi_prec ) { if( hi_prec ) mFlags |= EFL_HIGH_PRECISION; else mFlags &= ~EFL_HIGH_PRECISION; };

	//! Get update period.
	int								GetUpdatePeriod							( ) const { return mUpdatePeriod; };

	//! Set update period.
	void							SetUpdatePeriod							( int period ) { mUpdatePeriod = period; };



	//
	// General methods.
	//

	//! Kill all particles.
	void							Reset									( );

	//! Render particle system.
	virtual void					Render									( const ARGB& modulator = xfx::ARGB( 0xff, 0xff, 0xff, 0xff ) ) const;

	//! Update particle system.
	virtual void					Update									( DWORD mspf );

	//! Get particle system Cache.
	static Cache< ParticleSystem >&	Cache									( ) { return msCache; };

private:
	void							PureUpdate								( DWORD mspf );

	virtual HRESULT					PrepareParsing							( );
	HRESULT							Parse2D									( String::size_type& pos, const String& str );
	HRESULT							ParseHiPrecisionUpdate					( String::size_type& pos, const String& str );
	HRESULT							ParseSubSystem							( String::size_type& pos, const String& str );
};




_XFX_END

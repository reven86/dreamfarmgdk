//  File xfx_particlesystem.cpp                                               |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_particlesystem.h"
#include "main\xfx_cmd.h"
#include "renderer\xfx_viewer.h"
#include "renderer\xfx_viewport.h"

_XFX_BEGIN







//
// ParticleSubSystem
//

ParticleSubSystem::ParticleSubSystem( ) :
	Script( )
{
	AddToken( "subsystem",					&ParseUselessToken );
	AddToken( "{",							&ParseUselessToken );

	AddToken( "shader",						boost::bind( &ParticleSubSystem::ParseShader, this, _1, _2 ) );
	AddToken( "emitter_pos",				boost::bind( &ParseVariable< Vec3 >, boost::ref( emitter_pos ), _1, _2 ) );
	AddToken( "accelerate_dir",				boost::bind( &ParseVariable< Vec3 >, boost::ref( accelerate_dir ), _1, _2 ) );
	AddToken( "velocity_dir",				boost::bind( &ParseVariable< Vec3 >, boost::ref( velocity_dir ), _1, _2 ) );
	AddToken( "volume",						boost::bind( &ParseVariable< Vec3 >, boost::ref( volume ), _1, _2 ) );
	AddToken( "max_particles",				boost::bind( &ParseVariable< unsigned >, boost::ref( max_particles ), _1, _2 ) );
	AddToken( "spawn_count",				boost::bind( &ParseVariable< int >, boost::ref( spawn_count ), _1, _2 ) );

	AddToken( "lifetime",					boost::bind( &ParseVariable< DWORD >, boost::ref( lifetime ), _1, _2 ) );
	AddToken( "size",						boost::bind( &ParseVariable< float >, boost::ref( size ), _1, _2 ) );
	AddToken( "velocity",					boost::bind( &ParseVariable< float >, boost::ref( velocity ), _1, _2 ) );
	AddToken( "acceleration",				boost::bind( &ParseVariable< float >, boost::ref( acceleration ), _1, _2 ) );
	AddToken( "emissionrange",				boost::bind( &ParseAngleVariable, boost::ref( emissionrange ), _1, _2 ) );
	AddToken( "spin",						boost::bind( &ParseBigAngleVariable, boost::ref( spin ), _1, _2 ) );
	AddToken( "motionrand",					boost::bind( &ParseVariable< float >, boost::ref( motionrand ), _1, _2 ) );
	AddToken( "particleangle",				boost::bind( &ParseBigAngleVariable, boost::ref( particleangle ), _1, _2 ) );
	AddToken( "align_to_motion",			boost::bind( &ParseVariable< bool >, boost::ref( align_to_motion ), _1, _2 ) );

	AddToken( "life_variation",				boost::bind( &ParseVariable< DWORD >, boost::ref( life_variation ), _1, _2 ) );
	AddToken( "size_variation",				boost::bind( &ParseVariable< float >, boost::ref( size_variation ), _1, _2 ) );
	AddToken( "velocity_variation",			boost::bind( &ParseVariable< float >, boost::ref( velocity_variation ), _1, _2 ) );
	AddToken( "accelerate_variation",		boost::bind( &ParseVariable< float >, boost::ref( accelerate_variation ), _1, _2 ) );
	AddToken( "spin_variation",				boost::bind( &ParseBigAngleVariable, boost::ref( spin_variation ), _1, _2 ) );
	AddToken( "motionrand_variation",		boost::bind( &ParseVariable< float >, boost::ref( motionrand_variation ), _1, _2 ) );
	AddToken( "particleangle_variation",	boost::bind( &ParseBigAngleVariable, boost::ref( particleangle_variation ), _1, _2 ) );
	AddToken( "starttime_variation",		boost::bind( &ParseVariable< float >, boost::ref( starttime_variation ), _1, _2 ) );

	AddToken( "size_envelope",				boost::bind( &Envelope< float >::ParseEnvelope, boost::ref( size_envelope ), _1, _2 ) );
	AddToken( "velocity_envelope",			boost::bind( &Envelope< float >::ParseEnvelope, boost::ref( velocity_envelope ), _1, _2 ) );
	AddToken( "acceleration_envelope",		boost::bind( &Envelope< float >::ParseEnvelope, boost::ref( acceleration_envelope ), _1, _2 ) );
	AddToken( "spin_envelope",				boost::bind( &Envelope< float >::ParseEnvelope, boost::ref( spin_envelope ), _1, _2 ) );
	AddToken( "motionrand_envelope",		boost::bind( &Envelope< float >::ParseEnvelope, boost::ref( motionrand_envelope ), _1, _2 ) );
	AddToken( "colors_envelope",			boost::bind( &Envelope< ARGB >::ParseEnvelope, boost::ref( colors_envelope ), _1, _2 ) );

	AddToken( "}",							boost::bind( &ParticleSubSystem::ParseEndToken, this, _1, _2 ) );
}

void ParticleSubSystem::UpdateParticle( Particle& p, DWORD mspf ) const
{
	BYTE normaltime = static_cast< BYTE >( ( p.localtime << 0x08 ) / p.lifetime );

	float spf = 0.0001f * mspf;

	p.position	+= velocity_envelope.Key( normaltime ) * spf * p.velocity;
	p.velocity	+= spf * ( p.acceleration * acceleration_envelope.Key( normaltime ) * accelerate_dir + p.motionrand * motionrand_envelope.Key( normaltime ) * Vec3::Rand( ).GetNormalized( ) );
	p.angle		+= static_cast< Math::BigAngle >( p.spin * spin_envelope.Key( normaltime ) * spf );
	p.size		= p.start_size * size_envelope.Key( normaltime );
	p.color		= colors_envelope.Key( normaltime );
}

void ParticleSubSystem::SpawnParticle( Particle& p, const Mat4& transform ) const
{
	Math::Angle angle	= static_cast< Math::Angle >( emissionrange * 2.0f * ( math_frand( ) - 0.5f ) );
	float cos			= math_cos( angle );
	float sin			= math_sin( angle );

	Vec3 o( Vec3::Cross( velocity_dir, Vec3::Rand( ) ) );

	p.position		= emitter_pos + Vec3(
		volume.x * ( 0.5f - math_frand( ) ),
		volume.y * ( 0.5f - math_frand( ) ),
		volume.z * ( 0.5f - math_frand( ) )
		);

	p.position		= transform.TransformCoord( p.position );

	p.velocity		= ( velocity_dir * cos + o.GetNormalized( ) * sin ).GetNormalized( ) *
		( velocity + 2.0f * velocity_variation * ( math_frand( ) - 0.5f ) );

	p.velocity		= transform.TransformNormal( p.velocity );

	p.start_size	= size + 2.0f * size_variation * ( math_frand( ) - 0.5f );
	p.angle			= ( align_to_motion ) ? angle : particleangle + static_cast< Math::BigAngle >( 2.0f * static_cast< float >( particleangle_variation ) * ( math_frand( ) - 0.5f ) );
	p.spin			= spin + static_cast< Math::BigAngle >( 2.0f * static_cast< float >( spin_variation ) * ( math_frand( ) - 0.5f ) );
	p.motionrand	= motionrand + 2.0f * motionrand_variation * ( math_frand( ) - 0.5f );
	p.color			= colors_envelope.Key( 0 );
	p.size			= p.start_size * size_envelope.Key( 0 );
	p.acceleration	= acceleration + 2.0f * accelerate_variation * ( math_frand( ) - 0.5f );

	if( p.lifetime & 0x80000000 )
	{
		p.lifetime	= static_cast< DWORD >( lifetime + 2.0f * life_variation * ( math_frand( ) - 0.5f ) );
	}
	else
	{
		if ( p.lifetime == 0 )
		{
			p.lifetime	= static_cast< DWORD >( lifetime + 2.0f * life_variation * ( math_frand( ) - 0.5f ) );
			p.localtime	= p.localtime % p.lifetime;
		}
		else
		{
			p.localtime	= p.localtime % p.lifetime;
			p.lifetime	= static_cast< DWORD >( lifetime + 2.0f * life_variation * ( math_frand( ) - 0.5f ) );
		}
	}

	UpdateParticle( p, p.localtime );
}

HRESULT ParticleSubSystem::PrepareParsing( )
{
	shader_ptr.reset( );
	spawn_count			= 0;
	align_to_motion		= false;

	return S_OK;
}

HRESULT ParticleSubSystem::AfterParsing( )
{
	accelerate_dir.Normalize( );
	velocity_dir.Normalize( );

	return S_OK;
}

HRESULT ParticleSubSystem::ParseShader( String::size_type& pos, const String& str )
{
	String shader_name;
	ParseVariable( shader_name, pos, str );

	shader_ptr = Shader::Cache( ).Register( shader_name );

	return shader_ptr ? S_OK : XFXERR_IO;
}




//
// BaseParticleSystem
//
BaseParticleSystem::BaseParticleSystem( ) :
	mIsStopped( false ), mAliveCount( 0 ), mMaxParticles( 0 ),
	mEmitterTransformation( 1.0f ), mMaxParticleSize( 0.0f ),
	mScaler( 1.0f )
{
}

bool BaseParticleSystem::TestFrustumCulling( const Viewer * viewer, const Mat4& transform ) const
{
	Primitives::OBB obb( mAABB );
	obb.Transform( transform );

	_ASSERTE( viewer );

	if( r_debug_particles->AsInt( ) )
		Renderer::Instance( ).GetDrawTools( ).PushDrawBox( obb, 0xffffff00 );

	return viewer && viewer->TestFrustumCulling( obb );
}




//
// ParticleSystem
//
Cache< ParticleSystem > ParticleSystem::msCache;

ParticleSystem::ParticleSystem( ) : ScriptResource( "Particle system" ),
	mSystems( new std::list< const ParticleSubSystem >( ) ),
	mInvisibleTimer( 0 ),
	mFramesToUpdate( 0 ),
	mFlags( 0 ),
	mUpdatePeriod( 250 ),
	mUpdateTimer( 0 )
{
	AddToken( "particle_system",				&ParseUselessToken );
	AddToken( "{",								&ParseUselessToken );
	AddToken( "is_2d",							boost::bind( &ParticleSystem::Parse2D, this, _1, _2 ) );
	AddToken( "hi_precision_update",			boost::bind( &ParticleSystem::ParseHiPrecisionUpdate, this, _1, _2 ) );
	AddToken( "update_period",					boost::bind( &ParseVariable< int >, boost::ref( mUpdatePeriod ), _1, _2 ) );
	AddToken( "subsystem",						boost::bind( &ParticleSystem::ParseSubSystem, this, _1, _2 ) );
	AddToken( "}",								boost::bind( &ParticleSystem::ParseEndToken, this, _1, _2 ) );
}

ParticleSystem::~ParticleSystem( )
{
}

void ParticleSystem::AddSubSystem( const ParticleSubSystem& ps )
{
	mMaxParticles += ps.max_particles;

	mSystems->push_back( ps );
	mParticles.resize( mMaxParticles, Particle( ) );
	
	Reset( );
}

void ParticleSystem::RemoveSubSystem( unsigned index )
{
	if( index >= mSystems->size( ) )
		return;

	SystemsType::iterator it = boost::next( mSystems->begin( ), index );

	mMaxParticles -= ( *it ).max_particles;
	mSystems->erase( it );

	mParticles.resize( mMaxParticles, Particle( ) );

	Reset( );
}

ParticleSubSystem * ParticleSystem::GetSubSystem( unsigned index )
{
	if( index >= mSystems->size( ) )
		return NULL;

	return &( *boost::next( mSystems->begin( ), index ) );
}

const ParticleSubSystem * ParticleSystem::GetSubSystem( unsigned index ) const
{
	if( index >= mSystems->size( ) )
		return NULL;

	return &( *boost::next( mSystems->begin( ), index ) );
}

Particle * ParticleSystem::GetParticle( unsigned particle_index )
{
	if( particle_index >= mParticles.size( ) )
		return NULL;

	return &( *boost::next( mParticles.begin( ), particle_index ) );
}

const Particle * ParticleSystem::GetParticle( unsigned particle_index ) const
{
	if( particle_index >= mParticles.size( ) )
		return NULL;

	return &( *boost::next( mParticles.begin( ), particle_index ) );
}

void ParticleSystem::Reset( )
{
	ParticlesType::iterator pit = mParticles.begin( );

	for( SystemsType::iterator sit = mSystems->begin( ); sit != mSystems->end( ); sit++ )
	{
		DWORD dt = static_cast< DWORD >( ( *sit ).lifetime / ( *sit ).max_particles * ( *sit ).starttime_variation );
		DWORD t = 0;
		for( unsigned i = 0; i < ( *sit ).max_particles; i++, pit++, t -= dt )
		{
			(*pit).localtime	= t;
			(*pit).lifetime		= 0;
			(*pit).spawn_count	= 0;
		}
	}

	mIsStopped = false;
}

#pragma message( "FIXME: think about shader consts for particle systems" )
void ParticleSystem::Render( const ARGB& modulator ) const
{
	PROFILE( __FUNCTION__, "Render" );

	if( modulator.a == 0 )
		return;

	const Mat4 * transform( &Transformation( ) );

	float kw = 1.0f, kh = 1.0f;

	if( mFlags & EFL_2D )
	{
		kw = Viewport::Instance( ).InvWidth( ) * Renderer::Instance( ).D3DPP( ).BackBufferWidth;
		kh = Viewport::Instance( ).InvHeight( ) * Renderer::Instance( ).D3DPP( ).BackBufferHeight;
	}
	else
	{
#pragma message ( "FIXME: fix this" )
		/*
		if( World::Instance( ).CurrentViewer( ) &&
			!TestFrustumCulling( World::Instance( ).CurrentViewer( ).get( ), *transform )
			)
			return;
			*/
	}

	mInvisibleTimer = 0;

	float scale_kw = GetScaler( ) * kw;
	float scale_kh = GetScaler( ) * kh;

	boost::shared_ptr< const xfx::ShaderParams > def_shader_consts;

	ParticlesType::const_iterator pit = mParticles.begin( );
	for( SystemsType::const_iterator it = mSystems->begin( ); it != mSystems->end( ); it++ )
	{
		unsigned max_particles = ( *it ).max_particles;

		for( unsigned i = 0; i < max_particles; i++, pit++ )
		{
			const Particle& par = *pit;

			if( !( par.localtime & 0x80000000 ) && ( par.localtime < par.lifetime ) )
			{
				Vec3 pos = transform->TransformCoord( par.position );

				xfx::ARGB col( modulator * par.color );

				if( col.a != 0 )
					if( mFlags & EFL_2D )
					{
						Renderer::Instance( ).GetDrawTools( ).PushDraw2DSprite( 
							pos.x * kw, pos.y * kh, 
							par.size * scale_kw, par.size * scale_kh, 
							par.angle, col, ( *it ).shader_ptr,
							def_shader_consts, 0.0f, 0.0f, 1.0f, 1.0f,
							0.0f, 1.0f, true
							);
					}
					else
					{
						if( r_debug_particles->AsInt( ) )
						{
							Renderer::Instance( ).GetDrawTools( ).PushDrawBox( Primitives::AABB( pos - Vec3( par.size * 0.5f ), pos + Vec3( par.size * 0.5f ) ), 0x40ffffff );
						}
						else
						{
							Renderer::Instance( ).GetDrawTools( ).PushDraw3DSprite( pos, par.size, par.angle, col, ( *it ).shader_ptr );
						}
					}
			}
		}
	}
}

void ParticleSystem::Update( DWORD mspf )
{
	PROFILE( __FUNCTION__, "General" );

	mUpdateTimer -= static_cast< int >( mspf );
	if( mUpdateTimer > 0 )
		return;

	if( ( mFlags & EFL_HIGH_PRECISION ) != 0 && -mUpdateTimer > mUpdatePeriod )
	{
		int delta = -mUpdateTimer;

		while( delta > mUpdatePeriod )
		{
			PureUpdate( mUpdatePeriod );
			delta -= mUpdatePeriod;
		}

		PureUpdate( mUpdatePeriod + delta );
		mUpdateTimer = mUpdatePeriod;
		return;
	}

	mspf = static_cast< DWORD >( mUpdatePeriod - mUpdateTimer );
	mUpdateTimer = mUpdatePeriod;

	mInvisibleTimer += mspf;
	if( mInvisibleTimer > 5000 )
	{
		if( --mFramesToUpdate > 0 )
			return;

		mFramesToUpdate = 3;
		mspf *= 3;
	}

	PureUpdate( mspf );
}

void ParticleSystem::PureUpdate( DWORD mspf )
{
	mAliveCount			= 0;
	mMaxParticleSize	= 0.0f;

	mAABB =	Primitives::AABB::Null( );

	ParticlesType::iterator pit = mParticles.begin( );
	for( SystemsType::const_iterator it = mSystems->begin( ); it != mSystems->end( ); it++ )
	{
		int max_particles = static_cast< int >( ( *it ).max_particles );

#pragma omp parallel
		{

#pragma omp for
		for( int i = 0; i < max_particles; i++ )
		{
			Particle& p = *( &( *pit ) + i );

			p.localtime += mspf;

			bool alive_particle = false;

			if( p.localtime & 0x80000000 )
			{
			}
			else if( ( ( p.localtime > p.lifetime ) ) &&
				!mIsStopped &&
				( ( ( *it ).spawn_count <= 0 ) || ( p.spawn_count++ < ( *it ).spawn_count ) ) )
			{
				( *it ).SpawnParticle( p, mEmitterTransformation );
				alive_particle = true;
			}
			else if( p.localtime < p.lifetime )
			{
				( *it ).UpdateParticle( p, mspf );
				alive_particle = true;
			}

			if( alive_particle )
			{
#pragma omp atomic
				mAliveCount++;

				if( !mAABB.IsInside( p.position ) )
				{
#pragma omp critical ( cs_aabbtest )
					mAABB.Enlarge( p.position );
				}

				if( p.size > mMaxParticleSize )
				{
#pragma omp critical ( cs_sizetest )
					if( p.size > mMaxParticleSize )
						mMaxParticleSize = p.size;
				}
			}
		}
		}

		std::advance( pit, max_particles );
	}

	mAABB.EnlargeSize( Vec3( mMaxParticleSize * GetScaler( ) ) );
}

HRESULT ParticleSystem::PrepareParsing( )
{
	//_ASSERTE (mSystems->empty ());

	mSystems->clear ();
	ParticlesType( ).swap( mParticles );

	mMaxParticles = 0;
	mFlags = 0;
	mUpdatePeriod = 250;
	mUpdateTimer = 0;

	return S_OK;
}

HRESULT ParticleSystem::ParseSubSystem( String::size_type& pos, const String& str )
{
	static ParticleSubSystem ps;

	HRESULT hr;
	if( SUCCEEDED( hr = ps.ParseAt( pos, str ) ) )
	{
		if( ps.max_particles > 0 )
			AddSubSystem( ps );
	}

	return hr;
}

HRESULT ParticleSystem::Parse2D( String::size_type& pos, const String& str )
{
	int value = 0;

	HRESULT hr = ParseVariable< int >( value, pos, str );
	if( FAILED( hr ) )
		return hr;

	SetFlat( value != 0 );

	return S_OK;
}

HRESULT ParticleSystem::ParseHiPrecisionUpdate( String::size_type& pos, const String& str )
{
	int value = 0;

	HRESULT hr = ParseVariable< int >( value, pos, str );
	if( FAILED( hr ) )
		return hr;

	SetHighPrecisionUpdate( value != 0 );

	return S_OK;
}




_XFX_END
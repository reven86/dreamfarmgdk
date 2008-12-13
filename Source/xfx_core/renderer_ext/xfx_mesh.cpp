//  File xfx_mesh.cpp                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_mesh.h"
#include "main\xfx_cmd.h"

_XFX_BEGIN






//
// Mesh
//

AABBTree					Mesh::msEmptyTree;
Cache< Mesh >				Mesh::msCache;

Mesh::Mesh( ) : Resource( "Mesh" )
{
}

void Mesh::Cleanup( )
{
	mAnimations.clear( );
	mPosKeys.Clear( );
	mRotKeys.Clear( );
	mScaleKeys.Clear( );

	mVB.Destroy( );
	mIB.Destroy( );

	mAABBTree.reset( reinterpret_cast< class AABBTree * >( NULL ) );

	mTotalVertices = mTotalFaces = 0;
}

void Mesh::Render( const MeshState& state ) const
{
	//do frustum culling
	Mat4 matr = state.Transformation ();// * MatrixStack::Instance ().Top ();

	Primitives::OBB obb( RootAABB( ) );
	obb.Transform( matr );

#pragma message ( "FIXME: fix this" )
	/*
	if( World::Instance( ).CurrentViewer( ) &&
		!World::Instance( ).CurrentViewer( )->TestFrustumCulling( obb ) )
		return;
		*/

	if( r_drawbboxes->AsInt( ) )
	{
		Renderer::Instance( ).DrawTools( ).PushDrawBox( RootAABB( ), 0xffffffff, matr );
	}

	if( r_debug_aabbtree->AsInt( ) >= 0 )
	{
		AABBTree( ).DebugDraw( matr );
	}

	Renderer::Instance( ).SetTransform( D3DTS_WORLD, matr );

	mVB.SetStream		( 0 );
	mIB.SetIndices		( );

	Renderer::Instance( ).RenderPrimitive(
		state.ShaderPtr( ),
		state.ShaderParams( ),
#if( __XFX_DIRECTX_VER__ < 9 )
		boost::bind( &Renderer::DrawIndexedPrimitive, boost::ref( Renderer::Instance( ) ), D3DPT_TRIANGLELIST, 0, mTotalVertices, 0, mTotalFaces )
#else
		boost::bind( &Renderer::DrawIndexedPrimitive, boost::ref( Renderer::Instance( ) ), D3DPT_TRIANGLELIST, 0, 0, mTotalVertices, 0, mTotalFaces )
#endif
	);
}

HRESULT Mesh::LoadMemory( const void * memory, unsigned long )
{
	_ASSERTE( memory );
	if( !memory )
		return XFXERR_INVALIDCALL;

	if( *( ( unsigned long * ) memory ) != MAKEFOURCC( 'M', 'E', 'S', 'H' ) )
		return XFXERR_INVALIDCALL;

	char * curp = ( char * ) memory + sizeof( unsigned long );

	if( *( ( DWORD * ) curp ) != MODEL_FILE_VER )
		return XFXERR_INVALIDCALL;

	curp += sizeof( DWORD );

	DWORD fvf		= *( ( DWORD * ) curp );
	curp += sizeof( fvf );

	DWORD vert_size	= *( ( DWORD * ) curp );
	curp += sizeof( vert_size );

	DWORD flags		= *( ( DWORD * ) curp );
	curp += sizeof( flags );

	//Loading vertex and index buffers
	mTotalVertices = *( ( unsigned * ) curp );
	curp += sizeof( mTotalVertices );
	mTotalFaces = *( ( unsigned * ) curp );
	curp += sizeof( mTotalFaces );

	HRESULT hr = mVB.Create( vert_size, mTotalVertices, fvf, false );
	if( FAILED( hr ) )
		return hr;

	hr = mIB.Create( false, mTotalFaces * 3, false );
	if( FAILED( hr ) )
		return hr;

	{
		BufferLocker< VertexBuffer > vb_lock( mVB, 0, 0 );
		BufferLocker< IndexBuffer > ib_lock( mIB, 0, 0 );

		BYTE * vbuf = vb_lock.data< BYTE >( );
		unsigned short * ibuf = ib_lock.data< unsigned short >( );
		_ASSERTE( vbuf && ibuf );

		if( !vbuf || !ibuf )
			return XFXERR_UNKNOWN;

		memcpy( vbuf, curp, mTotalVertices * vert_size );
		curp += mTotalVertices * vert_size;

		memcpy( ibuf, curp, ( mTotalFaces * 3 ) << ( ( flags & 1 ) ? 1 : 2 ) );
		curp += ( mTotalFaces * 3 ) << ( ( flags & 1 ) ? 1 : 2 );

		//Building AABB tree
		std::vector< Primitives::Triangle > tris( 16384 );

		unsigned short * ind = ibuf;
		for( unsigned face = 0; face < mTotalFaces; face++, ind += 3 )
		{
			Vec3 * v1 = reinterpret_cast< Vec3 * >( vbuf + *ind * vert_size );
			Vec3 * v2 = reinterpret_cast< Vec3 * >( vbuf + *( ind + 1 ) * vert_size );
			Vec3 * v3 = reinterpret_cast< Vec3 * >( vbuf + *( ind + 2 ) * vert_size );

			tris.push_back( Primitives::Triangle( *v1, *v2, *v3 ) );
		}

		mAABBTree.reset( new class AABBTree( ) );
		mAABBTree->Build( tris.begin( ), tris.end( ) );
	}

	//
	// Load animations
	//

	/*

	DWORD anim_count = *( ( DWORD * ) curp );
	curp += sizeof( anim_count );

	for( DWORD i = 0; i < anim_count; ++i )
	{
		String str( curp );
		curp += str.size( ) + 1;

		Animation anim;

		anim.start_frame = *( ( DWORD * ) curp );
		curp += sizeof( anim.start_frame );

		anim.end_frame = *( ( DWORD * ) curp );
		curp += sizeof( anim.end_frame );

		anim.fps = *( ( float * ) curp );
		curp += sizeof( anim.fps );

		mAnimations.insert( AnimationsType::value_type( str, anim ) );
	}

	DWORD pos_keys_count = *( ( DWORD * ) curp );
	curp += sizeof( pos_keys_count );

	DWORD rot_keys_count = *( ( DWORD * ) curp );
	curp += sizeof( rot_keys_count );

	DWORD scale_keys_count = *( ( DWORD * ) curp );
	curp += sizeof( scale_keys_count );

	for( DWORD pk = 0; pk < pos_keys_count; ++pk )
	{
		DWORD time = *( ( DWORD * ) curp );
		curp += sizeof( time );

		Vec3 v;
		v.x = *( ( float * ) curp );
		curp += sizeof( v.x );

		v.y = *( ( float * ) curp );
		curp += sizeof( v.y );

		v.z = *( ( float * ) curp );
		curp += sizeof( v.z );

		mPosKeys.AddKey( static_cast< float >( time ), v );
	}

	for( DWORD rk = 0; rk < rot_keys_count; ++rk )
	{
		DWORD time = *( ( DWORD * ) curp );
		curp += sizeof( time );

		Euler v;
		v.yaw = *( ( Math::BigAngle * ) curp );
		curp += sizeof( v.yaw );

		v.pitch = *( ( Math::BigAngle * ) curp );
		curp += sizeof( v.pitch );

		v.roll = *( ( Math::BigAngle * ) curp );
		curp += sizeof( v.roll );

		mRotKeys.AddKey( static_cast< float >( time ), v );
	}

	for( DWORD sk = 0; sk < scale_keys_count; ++sk )
	{
		DWORD time = *( ( DWORD * ) curp );
		curp += sizeof( time );

		Vec3 v;
		v.x = *( ( float * ) curp );
		curp += sizeof( v.x );

		v.y = *( ( float * ) curp );
		curp += sizeof( v.y );

		v.z = *( ( float * ) curp );
		curp += sizeof( v.z );

		mScaleKeys.AddKey( static_cast< float >( time ), v );
	}

	*/

	return hr;
}





//
// MeshState
//

MeshState::MeshState( ) :
	mShaderParamsPtr( new class ShaderParams( ) )
{
}

void MeshState::PlayAnimation( const String& str )
{
	mAnimationState.current_animation = mMeshPtr->Animations( ).find( str );

	mAnimationState.cur_frame	= 0;
	mAnimationState.local_time	= 0;

	if( mAnimationState.current_animation == mMeshPtr->Animations( ).end( ) )
	{
		mAnimationState.total_frames		= 0;
		mAnimationState.one_loop_was_played	= true;
		return;
	}

	mAnimationState.total_frames		= ( *mAnimationState.current_animation ).second.end_frame - ( *mAnimationState.current_animation ).second.start_frame;
	mAnimationState.one_loop_was_played	= false;

	if( !mAnimationState.total_frames )
		mAnimationState.total_frames = 1;
}

void MeshState::Update( DWORD mspf )
{
	PROFILE( __FUNCTION__, "General" );

	if( mMeshPtr && mAnimationState.current_animation != mMeshPtr->Animations( ).end( ) && mAnimationState.total_frames != 0 )
	{
		mAnimationState.local_time += mspf;

		float f_frame = r_debug_animationframe->AsFloat( ) >= 0.0f ? r_debug_animationframe->AsFloat( ) : mAnimationState.local_time * ( *mAnimationState.current_animation ).second.fps * 0.1f;
		DWORD frame = static_cast< DWORD >( f_frame );
		mAnimationState.cur_frame = frame % mAnimationState.total_frames;
		mAnimationState.one_loop_was_played = ( frame >= mAnimationState.total_frames );

		f_frame = fmodf( f_frame, static_cast< float >( mAnimationState.total_frames ) ) + ( *mAnimationState.current_animation ).second.start_frame;

		mAnimationState.anim_transform.Reset( mMeshPtr->PosKeys( ).Key( f_frame ), mMeshPtr->RotKeys( ).Key( f_frame ), mMeshPtr->ScaleKeys( ).Key( f_frame ) );
	}
}





_XFX_END
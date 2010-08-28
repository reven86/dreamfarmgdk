//  File xfx_mesh.h                                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "..\math\xfx_aabbtree.h"

/*!	\file xfx_mesh.h 
 *	\brief Simple geometry object with key frame animations.
 */

_XFX_BEGIN







/*!	\defgroup RenderExtGroup Renderer extension classes.
 */

/*! \class Mesh xfx_mesh.h "renderer_ext/xfx_mesh.h"
 *	\brief Polygonal model class with keyframe animations.
 *	\ingroup RenderExtGroup
 *	
 *	Simple geometry object with position, rotation and scale key frame animations.
 *	Holds only geomtry data and provides no update functions, so most of all methods are const.
 *	Model also have AABB tree representation. This provides support methods for collision and ray tracing.
 *
 *	\see MeshState.
 *	\author Andrew "RevEn" Karpushin
 */

class Mesh : public Resource
{
public:
	//! %Animation information.
	struct Animation
	{
		DWORD			start_frame;	//!< Start frame number.
		DWORD			end_frame;		//!< End frame number.
		float			fps;			//!< Frames Per Second.
	};

	//! Animations container.
	typedef stdext::hash_map< String, Animation >	AnimationsType;

private:
	//! It is same for all clones.
	struct SharedData
	{
		unsigned total_vertices;
		unsigned total_faces;

		boost::shared_ptr< AABBTree > aabb_tree;

		VertexBuffer vb;
		IndexBuffer ib;

		AnimationsType animations;
		Envelope< Vec3, float > pos_keys;
		Envelope< Quaternion, float, QuaternionLerpFn >	rot_keys;
		Envelope< Vec3, float > scale_keys;

		~SharedData( )
		{
			animations.clear( );
			pos_keys.Clear( );
			rot_keys.Clear( );
			scale_keys.Clear( );

			vb.Destroy( );
			ib.Destroy( );

			aabb_tree.reset( reinterpret_cast< class AABBTree * >( NULL ) );

			total_vertices = total_faces = 0;
		}
	};

	boost::shared_ptr< SharedData >					mData;

public:
	//! File version.
	enum { MODEL_FILE_VER = 100 };

private:
	static AABBTree									msEmptyTree;
	static Cache< Mesh >							msCache;

public:
	//! Contructs an empty object without animations.
	Mesh																	( );

	//! Destructs object.
	virtual ~Mesh															( ) { Cleanup( ); };

	//! Get all animations.
	const AnimationsType&					Animations						( ) const { return mData->animations; };

	//! Get position keys.
	const Envelope< Vec3, float >&			PosKeys							( ) const { return mData->pos_keys; };

	//! Get rotation keys.
	const Envelope< Quaternion, float, QuaternionLerpFn >& RotKeys			( ) const { return mData->rot_keys; };

	//! Get scale keys.
	const Envelope< Vec3, float >&			ScaleKeys						( ) const { return mData->scale_keys; };

	//! Get AABB tree.
	const AABBTree&							GetAABBTree						( ) const { return mData->aabb_tree ? *mData->aabb_tree : msEmptyTree; };

	//! Get root AABB.
	const Primitives::AABB&					RootAABB						( ) const { return GetAABBTree( ).RootAABB( ); };

	//! Load from memory.
	virtual HRESULT							LoadMemory						( const void * memory, unsigned long filelen );

	//! %Render model. MeshState object (holds rendering state) needed.
	void									Render							( const class MeshState& state ) const;

	//! Get cache of %Mesh.
	static Cache< Mesh >&					Cache							( ) { return msCache; };

protected:
	//! Cleanup all data.
	void									Cleanup							( );
};





/*! \class MeshState xfx_mesh.h "renderer_ext/xfx_mesh.h"
 *	\brief Solid model state class.
 *	\ingroup RenderExtGroup
 *	
 *	Holds an animation and rendering states for SolidModel.
 *
 *	\see Mesh.
 *	\author Andrew "RevEn" Karpushin
 */

class MeshState : public Transformable3D
{
	boost::shared_ptr< const Mesh >							mMeshPtr;

	//
	// Render state
	//

	boost::shared_ptr< const class Shader >					mShaderPtr;
	boost::scoped_ptr< class ShaderParams >					mShaderParamsPtr;


	//
	// Animation state
	//

	struct AnimationState
	{
		Mesh::AnimationsType::const_iterator				current_animation;
		DWORD												cur_frame;
		DWORD												total_frames;
		float												local_time;				//!< Local time in 100 microseconds.
		bool												one_loop_was_played;

		Mat4												anim_transform;
	};

	AnimationState											mAnimationState;

public:
	//! Constructs an empty, default MeshState.
	MeshState																( );

	//! Destructs MeshState.
	~MeshState																( ) { };

	//! Set associated Mesh object.
	void										SetMeshPtr					( const boost::shared_ptr< const Mesh >& mdl )
	{
		mMeshPtr = mdl;
		if( mMeshPtr )
			mAnimationState.current_animation = mMeshPtr->Animations ().end ();
	};

	//! Get associated Mesh object.
	const boost::shared_ptr< const Mesh >&		MeshPtr						( ) const { return mMeshPtr; };

	//! \name Render-specific methods
	//! @{

	//! Set Shader.
	void										SetShaderPtr				( const boost::shared_ptr< const Shader >& shd ) { mShaderPtr = shd;};

	//! Get Shader.
	const boost::shared_ptr< const Shader >&	ShaderPtr					( ) const { return mShaderPtr; };

	//! Get/Set ShaderConsts.
	class ShaderParams&							GetShaderParams				( ) { return *mShaderParamsPtr; };

	//! Get ShaderConsts.
	const class ShaderParams&					GetShaderParams				( ) const { return *mShaderParamsPtr; };

	//! %Render Mesh with current state.
	void										Render						( ) const
	{
		mMeshPtr->Render( *this );
	};

	//! @}


	//! \name Animation methods

	//! @{

	//! Play new animation by name.
	void										PlayAnimation				( const String& str );

	//! Update current state.
	void										Update						( DWORD mspf );

	//! @}
};





_XFX_END

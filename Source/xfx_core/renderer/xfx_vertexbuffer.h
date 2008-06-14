//  File xfx_vertexbuffer.h                                                   |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_vertexbuffer.h 
 *	\brief Vertex buffer wrapper.
 */

_XFX_BEGIN





/*! \class VertexBuffer xfx_vertexbuffer.h "render/xfx_vertexbuffer.h"
 *	\brief Wrapper around D3D vertex buffer.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class VertexBuffer : boost::noncopyable
{
	friend class BufferLocker< VertexBuffer >;

#if( __XFX_DIRECTX_VER__ < 9 )
	boost::shared_ptr< IDirect3DVertexBuffer8 >		mpVB;
#else
	boost::shared_ptr< IDirect3DVertexBuffer9 >		mpVB;
#endif

	bool						mIsDynamic;
	unsigned					mVertexSize;
	unsigned					mNumVertices;
	int							mFVF;
	int							mAdditionalUsage;

	boost::signals::connection	mInvalidateConnection;

public:
	//! Constructs an empty vertex buffer.
	VertexBuffer				( );

	//! Destructor.
	~VertexBuffer				( );

	/*! \brief Create vertex buffer.
	 *	
	 *	\param[in]	vertexsize			Size of one vertex.
	 *	\param[in]	numvertices			Number of vertices.
	 *	\param[in]	fvf					FVF.
	 *	\param[in]	dynamic				Is dynamic.
	 *	\param[in]	additional_usage	Additional usage flags, D3DUSAGE_*.
	 */
	HRESULT Create				( const unsigned& vertexsize, const unsigned& numvertices, const int& fvf, const bool& dynamic, const int& additional_usage = 0 );

	//! Destroy vertex buffer and free its resources.
	void Destroy				( );

	//! Invalidate callback.
	void InvalidateCallback		( bool invalidate );

	//! Set vertex buffer to Renderer in specified stream and prepare for rendering.
	void SetStream				( const int& stream ) const;

private:
	void * Lock					( const unsigned& offset, const unsigned& count, const bool& discard_if_dynamic = true );
	void Unlock					( );
};




_XFX_END

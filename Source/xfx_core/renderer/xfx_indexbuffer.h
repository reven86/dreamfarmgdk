//  File xfx_indexbuffer.h                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_indexbuffer.h 
 *	\brief Index buffer wrapper.
 */

_XFX_BEGIN





/*! \class IndexBuffer xfx_indexbuffer.h "render/xfx_indexbuffer.h"
 *	\brief Wrapper around D3D index buffer.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class IndexBuffer : boost::noncopyable
{
	friend class BufferLocker< IndexBuffer >;

#if (__XFX_DIRECTX_VER__ < 9)
	boost::shared_ptr< IDirect3DIndexBuffer8 >		mpIB;
#else
	boost::shared_ptr< IDirect3DIndexBuffer9 >		mpIB;
#endif

	bool						mIsDynamic;
	bool						mIs32BitIndex;
	unsigned					mNumIndices;

	boost::signals::connection	mInvalidateConnection;

public:
	//! Constructs an empty index buffer.
	IndexBuffer					( );

	//! Destructor.
	~IndexBuffer				( );

	/*! \brief Create index buffer.
	 *	
	 *	\param[in]	is32bit		Bits per index ( \b true if 32 ).
	 *	\param[in]	numindices	Number of indices.
	 *	\param[in]	dynamic		Is dynamic.
	 */
	HRESULT Create				( const bool& is32bit, const unsigned& numindices, const bool& dynamic );

	//! Destroy index buffer and free its resources.
	void Destroy				( );

	//! Set indices to Render and prepare for rendering.
	void SetIndices				( ) const;

	//! Invalidate callback.
	void InvalidateCallback		( bool invalidate );

private:
	void * Lock					( const unsigned& offset, const unsigned& count, const bool& discard_if_dynamic = true );
	void Unlock					( );
};




_XFX_END

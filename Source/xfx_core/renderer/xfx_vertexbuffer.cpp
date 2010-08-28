//  File xfx_vertexbuffer.cpp                                                 |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_vertexbuffer.h"
#include "xfx_renderer.h"

_XFX_BEGIN





//
// Vertex buffer
//

VertexBuffer::VertexBuffer () : mpVB (), mIsDynamic (false),
	mVertexSize (0), mNumVertices (0), mFVF (0)
{
}

VertexBuffer::~VertexBuffer ()
{
	Destroy ();
}

HRESULT VertexBuffer::Create (const unsigned& vertexsize, const unsigned& numvertices, const int& fvf, const bool& dynamic, const int& additional_usage)
{
	Destroy ();

	mIsDynamic			= dynamic;
	mVertexSize			= vertexsize;
	mNumVertices		= numvertices;
	mFVF				= fvf;
	mAdditionalUsage	= additional_usage;

	unsigned length = vertexsize * numvertices;

	int usage;
	D3DPOOL pool;

	if (dynamic)
	{
		usage = D3DUSAGE_DYNAMIC;
		pool = D3DPOOL_DEFAULT;

		mInvalidateConnection = Renderer::Instance( ).AddInvalidateCallback( boost::bind( &VertexBuffer::InvalidateCallback, this, _1 ) );
	}
	else
	{
		usage = 0;
		pool = D3DPOOL_MANAGED;
	}

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DVERTEXBUFFER8 vb;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateVertexBuffer (length, usage | D3DUSAGE_WRITEONLY | additional_usage, fvf, pool, &vb);
#else
	LPDIRECT3DVERTEXBUFFER9 vb;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateVertexBuffer (length, usage | D3DUSAGE_WRITEONLY | additional_usage, fvf, pool, &vb, NULL);
#endif

	if (SUCCEEDED (hr))
		mpVB.reset (vb, IUnknownDeleter ());

	return hr;
}

void VertexBuffer::Destroy ()
{
	if (!mpVB)
		return;

	if (mIsDynamic)
		mInvalidateConnection.disconnect ();

	InvalidateCallback (true);
}

void * VertexBuffer::Lock (const unsigned& offset, const unsigned& count, const bool& discard_if_dynamic)
{
	PROFILE( __FUNCTION__, "Render" );

	if (!mpVB)
		return NULL;

    void * res;

#if (__XFX_DIRECTX_VER__ < 9)
	if (FAILED (mpVB->Lock (offset * mVertexSize, count * mVertexSize, reinterpret_cast<BYTE **> (&res), (mIsDynamic) ? ((discard_if_dynamic) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE) | D3DLOCK_NOSYSLOCK : 0)))
#else
	if (FAILED (mpVB->Lock (offset * mVertexSize, count * mVertexSize, &res, (mIsDynamic) ? ((discard_if_dynamic) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE) | D3DLOCK_NOSYSLOCK : 0)))
#endif
		return NULL;

	return res;
}

void VertexBuffer::Unlock ()
{
	if (mpVB)
		mpVB->Unlock ();
}

void VertexBuffer::SetStream (const int& stream) const
{
	if (mpVB)
	{
#if (__XFX_DIRECTX_VER__ < 9)
		Renderer::Instance ().pD3DDevice ()->SetStreamSource (stream, mpVB.get (), mVertexSize);
#else
		Renderer::Instance ().pD3DDevice ()->SetStreamSource (stream, mpVB.get (), 0, mVertexSize);
#endif
		Renderer::Instance ().SetFVF (mFVF);
	}
}

void VertexBuffer::InvalidateCallback (bool invalidate)
{
	//gMess( "%s %d", __FUNCTION__, invalidate );	

	if (invalidate)
	{
		mpVB.reset ();
		return;
	}

	unsigned length = mVertexSize * mNumVertices;

	int usage;
	D3DPOOL pool;

	if (mIsDynamic)
	{
		usage = D3DUSAGE_DYNAMIC;
		pool = D3DPOOL_DEFAULT;
	}
	else
	{
		usage = 0;
		pool = D3DPOOL_MANAGED;
	}

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DVERTEXBUFFER8 vb;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateVertexBuffer (length, usage | D3DUSAGE_WRITEONLY | mAdditionalUsage, mFVF, pool, &vb);
#else
	LPDIRECT3DVERTEXBUFFER9 vb;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateVertexBuffer (length, usage | D3DUSAGE_WRITEONLY | mAdditionalUsage, mFVF, pool, &vb, NULL);
#endif

	_ASSERT (SUCCEEDED (hr));

	if (SUCCEEDED (hr))
		mpVB.reset (vb, IUnknownDeleter ());
	else
		gError ("Can't restore vertex buffer surface!");
}



_XFX_END
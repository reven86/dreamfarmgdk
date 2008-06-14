//  File xfx_indexbuffer.cpp                                                  |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_indexbuffer.h"
#include "xfx_renderer.h"

_XFX_BEGIN





//
// Index buffer
//

IndexBuffer::IndexBuffer( ) : mpIB( ), mIsDynamic( false ),
	mIs32BitIndex( false ), mNumIndices( 0 )
{
}

IndexBuffer::~IndexBuffer( )
{
	Destroy( );
}

HRESULT IndexBuffer::Create (const bool& is32bit, const unsigned& numindices, const bool& dynamic)
{
	Destroy ();

	mIsDynamic		= dynamic;
	mIs32BitIndex	= is32bit;
	mNumIndices		= numindices;

	unsigned length = ((is32bit) ? sizeof (DWORD) : sizeof (WORD)) * numindices;

	int usage;
	D3DPOOL pool;

	if (dynamic)
	{
		usage = D3DUSAGE_DYNAMIC;
		pool = D3DPOOL_DEFAULT;

		mInvalidateConnection = Renderer::Instance ().InvalidateCallbacks ().connect (boost::bind (&IndexBuffer::InvalidateCallback, this, _1));
	}
	else
	{
		usage = 0;
		pool = D3DPOOL_MANAGED;
	}

	XFX_PLACE_DEVICE_LOCK;

#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DINDEXBUFFER8 ib;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateIndexBuffer (length, usage | D3DUSAGE_WRITEONLY, (is32bit) ? D3DFMT_INDEX32 : D3DFMT_INDEX16, pool, &ib);
#else
	LPDIRECT3DINDEXBUFFER9 ib;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateIndexBuffer (length, usage | D3DUSAGE_WRITEONLY, (is32bit) ? D3DFMT_INDEX32 : D3DFMT_INDEX16, pool, &ib, NULL);
#endif

	if (SUCCEEDED (hr))
		mpIB.reset (ib, IUnknownDeleter ());

	return hr;
}

void IndexBuffer::Destroy ()
{
	if (!mpIB)
		return;

	if (mIsDynamic)
		mInvalidateConnection.disconnect ();

	InvalidateCallback (true);
}

void * IndexBuffer::Lock (const unsigned& offset, const unsigned& count, const bool& discard_if_dynamic)
{
	PROFILE( __FUNCTION__, "Render" );

	if (!mpIB)
		return NULL;

    void * res;

#if (__XFX_DIRECTX_VER__ < 9)
	if (FAILED (mpIB->Lock (offset, (mIs32BitIndex) ? (count << 2) : (count << 1), reinterpret_cast<BYTE **> (&res), (mIsDynamic) ? ((discard_if_dynamic) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE) | D3DLOCK_NOSYSLOCK : 0)))
#else
	if (FAILED (mpIB->Lock (offset, (mIs32BitIndex) ? (count << 2) : (count << 1), &res, (mIsDynamic) ? ((discard_if_dynamic) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE) | D3DLOCK_NOSYSLOCK : 0)))
#endif
		return NULL;

	return res;
}

void IndexBuffer::Unlock ()
{
	if (mpIB)
		mpIB->Unlock ();
}

void IndexBuffer::SetIndices () const
{
	if (mpIB)
	{
#if (__XFX_DIRECTX_VER__ < 9)
		Renderer::Instance ().pD3DDevice ()->SetIndices (mpIB.get (), 0);
#else
		Renderer::Instance ().pD3DDevice ()->SetIndices (mpIB.get ());
#endif
	}
}

void IndexBuffer::InvalidateCallback (bool invalidate)
{
	if (invalidate)
	{
		mpIB.reset ();
		return;
	}

	unsigned length = ((mIs32BitIndex) ? sizeof (DWORD) : sizeof (WORD)) * mNumIndices;

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
	LPDIRECT3DINDEXBUFFER8 ib;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateIndexBuffer (length, usage | D3DUSAGE_WRITEONLY, (mIs32BitIndex) ? D3DFMT_INDEX32 : D3DFMT_INDEX16, pool, &ib);
#else
	LPDIRECT3DINDEXBUFFER9 ib;
	HRESULT hr = Renderer::Instance ().pD3DDevice ()->CreateIndexBuffer (length, usage | D3DUSAGE_WRITEONLY, (mIs32BitIndex) ? D3DFMT_INDEX32 : D3DFMT_INDEX16, pool, &ib, NULL);
#endif

	_ASSERTE (SUCCEEDED (hr));

	if (SUCCEEDED (hr))
		mpIB.reset (ib, IUnknownDeleter ());
	else
		gError ("Can't restore index buffer surface!");
}



_XFX_END
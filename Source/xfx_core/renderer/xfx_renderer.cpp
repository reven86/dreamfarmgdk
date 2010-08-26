//  File xfx_renderer.cpp                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_renderer.h"
#include "xfx_texture.h"
#include "xfx_shader.h"
#include "xfx_renderer_cvars.h"
#include "xfx_viewport.h"

extern "C"
{
#include <jpeglib.h>
}

_XFX_BEGIN





//
// Variables
//

D3DRENDERSTATETYPE Renderer::stateD3D[Renderer::SM_MAX_STATES] = 
{
	D3DRS_ZENABLE,
	D3DRS_FILLMODE,
	D3DRS_SHADEMODE,
	D3DRS_ZWRITEENABLE,
	D3DRS_ALPHATESTENABLE,
	D3DRS_LASTPIXEL,
	D3DRS_SRCBLEND,
	D3DRS_DESTBLEND,
	D3DRS_CULLMODE,
	D3DRS_ZFUNC,
	D3DRS_ALPHAREF,
	D3DRS_ALPHAFUNC,
	D3DRS_DITHERENABLE,
	D3DRS_ALPHABLENDENABLE,
	D3DRS_FOGENABLE,
	D3DRS_SPECULARENABLE,
	D3DRS_FOGCOLOR,
	D3DRS_FOGTABLEMODE,
	D3DRS_FOGSTART,
	D3DRS_FOGEND,
	D3DRS_FOGDENSITY,
	D3DRS_RANGEFOGENABLE,
	D3DRS_STENCILENABLE,
	D3DRS_STENCILFAIL,
	D3DRS_STENCILZFAIL,
	D3DRS_STENCILPASS,
	D3DRS_STENCILFUNC,
	D3DRS_STENCILFAIL,
	D3DRS_STENCILMASK,
	D3DRS_STENCILWRITEMASK,
	D3DRS_TEXTUREFACTOR,
	D3DRS_WRAP0,
	D3DRS_WRAP1,
	D3DRS_WRAP2,
	D3DRS_WRAP3,
	D3DRS_WRAP4,
	D3DRS_WRAP5,
	D3DRS_WRAP6,
	D3DRS_WRAP7,
	D3DRS_CLIPPING,
	D3DRS_LIGHTING,
	D3DRS_AMBIENT,
	D3DRS_FOGVERTEXMODE,
	D3DRS_COLORVERTEX,
	D3DRS_LOCALVIEWER,
	D3DRS_NORMALIZENORMALS,
	D3DRS_DIFFUSEMATERIALSOURCE,
	D3DRS_SPECULARMATERIALSOURCE,
	D3DRS_AMBIENTMATERIALSOURCE,
	D3DRS_EMISSIVEMATERIALSOURCE,
	D3DRS_VERTEXBLEND,
	D3DRS_CLIPPLANEENABLE,
	D3DRS_POINTSIZE,
	D3DRS_POINTSIZE_MIN,
	D3DRS_POINTSPRITEENABLE,
	D3DRS_POINTSCALEENABLE,
	D3DRS_POINTSCALE_A,
	D3DRS_POINTSCALE_B,
	D3DRS_POINTSCALE_C,
	D3DRS_MULTISAMPLEANTIALIAS,
	D3DRS_MULTISAMPLEMASK,
	D3DRS_PATCHEDGESTYLE,
	D3DRS_DEBUGMONITORTOKEN,
	D3DRS_POINTSIZE_MAX,
	D3DRS_INDEXEDVERTEXBLENDENABLE,
	D3DRS_COLORWRITEENABLE,
	D3DRS_TWEENFACTOR,
	D3DRS_BLENDOP,
#if !(__XFX_DIRECTX_VER__ < 9)
	D3DRS_POSITIONDEGREE,
	D3DRS_NORMALDEGREE,
	D3DRS_SCISSORTESTENABLE,
	D3DRS_SLOPESCALEDEPTHBIAS,
	D3DRS_ANTIALIASEDLINEENABLE,
	D3DRS_MINTESSELLATIONLEVEL,
	D3DRS_MAXTESSELLATIONLEVEL,
	D3DRS_ADAPTIVETESS_X,
	D3DRS_ADAPTIVETESS_Y,
	D3DRS_ADAPTIVETESS_Z,
	D3DRS_ADAPTIVETESS_W,
	D3DRS_ENABLEADAPTIVETESSELLATION,
	D3DRS_TWOSIDEDSTENCILMODE,
	D3DRS_CCW_STENCILFAIL,
	D3DRS_CCW_STENCILZFAIL,
	D3DRS_CCW_STENCILPASS,
	D3DRS_CCW_STENCILFUNC,
	D3DRS_COLORWRITEENABLE1,
	D3DRS_COLORWRITEENABLE2,
	D3DRS_COLORWRITEENABLE3,
	D3DRS_BLENDFACTOR,
	D3DRS_SRGBWRITEENABLE,
	D3DRS_DEPTHBIAS,
	D3DRS_WRAP8,
	D3DRS_WRAP9,
	D3DRS_WRAP10,
	D3DRS_WRAP11,
	D3DRS_WRAP12,
	D3DRS_WRAP13,
	D3DRS_WRAP14,
	D3DRS_WRAP15,
	D3DRS_SEPARATEALPHABLENDENABLE,
	D3DRS_SRCBLENDALPHA,
	D3DRS_DESTBLENDALPHA,
	D3DRS_BLENDOPALPHA
#else
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD,
	D3DRS_FORCE_DWORD
#endif
};

DWORD Renderer::stateValue[Renderer::SM_MAX_STATES] = 
{
	D3DZB_TRUE,
	D3DFILL_SOLID,
	D3DSHADE_GOURAUD,
	TRUE,
	FALSE,
	TRUE,
	D3DBLEND_ONE,
	D3DBLEND_ZERO,
	D3DCULL_CCW,
	D3DCMP_LESSEQUAL,
	0,
	D3DCMP_ALWAYS,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	0,
	D3DFOG_NONE,
	0,
	0x3f800000, //1.0f
	0x3f800000, //1.0f
	FALSE,
	FALSE,
	D3DSTENCILOP_KEEP,
	D3DSTENCILOP_KEEP,
	D3DSTENCILOP_KEEP,
	D3DCMP_ALWAYS,
	0,
	0xffffffff,
	0xffffffff,
	0xffffffff,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	TRUE,
	TRUE,
	0,
	D3DFOG_NONE,
	TRUE,
	TRUE,
	FALSE,
	D3DMCS_COLOR1,
	D3DMCS_COLOR2,
	D3DMCS_MATERIAL,
	D3DMCS_MATERIAL,
	D3DVBF_DISABLE,
	0,
	0x3f800000, //1.0f
	0x3f800000, //1.0f
	FALSE,
	FALSE,
	0x3f800000, //1.0f
	0,
	0,
	TRUE,
	0xffffffff,
	D3DPATCHEDGE_DISCRETE,
	D3DDMT_ENABLE,
	0x42800000, //64.0f
	FALSE,
	0x0000000f,
	0,
	D3DBLENDOP_ADD,
#if !(__XFX_DIRECTX_VER__ < 9)
	D3DDEGREE_CUBIC,
	D3DDEGREE_LINEAR,
#else
	0,
	0,
#endif
	FALSE,
	0,
	FALSE,
	0x3f800000, //1.0f
	0x3f800000, //1.0f
	0,
	0,
	0x3f800000, //1.0f
	0,
	FALSE,
	FALSE,
	0x00000001,
	0x00000001,
	0x00000001,
	0x00000008,
	0x0000000f,
	0x0000000f,
	0x0000000f,
	0xffffffff,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	FALSE,
	D3DBLEND_ONE,
	D3DBLEND_ZERO,
	D3DBLENDOP_ADD
};

D3DTEXTURESTAGESTATETYPE Renderer::stateTexD3D[Renderer::SM_TEX_MAX_STATES] = 
{
	D3DTSS_COLOROP,
    D3DTSS_COLORARG1,
    D3DTSS_COLORARG2,
    D3DTSS_ALPHAOP,
    D3DTSS_ALPHAARG1,
    D3DTSS_ALPHAARG2,
    D3DTSS_BUMPENVMAT00,
    D3DTSS_BUMPENVMAT01,
    D3DTSS_BUMPENVMAT10,
    D3DTSS_BUMPENVMAT11,
    D3DTSS_TEXCOORDINDEX,
    D3DTSS_BUMPENVLSCALE,
    D3DTSS_BUMPENVLOFFSET,
    D3DTSS_TEXTURETRANSFORMFLAGS,
    D3DTSS_COLORARG0,
    D3DTSS_ALPHAARG0,
    D3DTSS_RESULTARG,
#if !(__XFX_DIRECTX_VER__ < 9)
    D3DTSS_CONSTANT
#else
	D3DTSS_FORCE_DWORD
#endif
};

DWORD Renderer::stateTexValue[Renderer::MAX_TEXTURE_STAGES][Renderer::SM_TEX_MAX_STATES] = 
{
	{
		D3DTOP_MODULATE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		D3DTOP_SELECTARG1,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		0,
		0,
		0,
		0,
		D3DTSS_TCI_PASSTHRU,
		0,
		0,
		D3DTTFF_DISABLE,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		0
	},
	{
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		0,
		0,
		0,
		0,
		D3DTSS_TCI_PASSTHRU,
		0,
		0,
		D3DTTFF_DISABLE,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		0
	},
	{
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		0,
		0,
		0,
		0,
		D3DTSS_TCI_PASSTHRU,
		0,
		0,
		D3DTTFF_DISABLE,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		0
	},
	{
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		0,
		0,
		0,
		0,
		D3DTSS_TCI_PASSTHRU,
		0,
		0,
		D3DTTFF_DISABLE,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		0
	},
	{
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		0,
		0,
		0,
		0,
		D3DTSS_TCI_PASSTHRU,
		0,
		0,
		D3DTTFF_DISABLE,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		0
	},
	{
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		0,
		0,
		0,
		0,
		D3DTSS_TCI_PASSTHRU,
		0,
		0,
		D3DTTFF_DISABLE,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		0
	},
	{
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		0,
		0,
		0,
		0,
		D3DTSS_TCI_PASSTHRU,
		0,
		0,
		D3DTTFF_DISABLE,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		0
	},
	{
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		D3DTOP_DISABLE,
		D3DTA_TEXTURE,
		D3DTA_CURRENT,
		0,
		0,
		0,
		0,
		D3DTSS_TCI_PASSTHRU,
		0,
		0,
		D3DTTFF_DISABLE,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		D3DTA_CURRENT,
		0
	}
};

#if !(__XFX_DIRECTX_VER__ < 9)
D3DSAMPLERSTATETYPE Renderer::stateSampD3D[Renderer::SM_SAMP_MAX_STATES] = 
{
    D3DSAMP_ADDRESSU,
    D3DSAMP_ADDRESSV,
    D3DSAMP_ADDRESSW,
    D3DSAMP_BORDERCOLOR,
    D3DSAMP_MAGFILTER,
    D3DSAMP_MINFILTER,
    D3DSAMP_MIPFILTER,
    D3DSAMP_MIPMAPLODBIAS,
    D3DSAMP_MAXMIPLEVEL,
    D3DSAMP_MAXANISOTROPY,
    D3DSAMP_SRGBTEXTURE,
    D3DSAMP_ELEMENTINDEX,
    D3DSAMP_DMAPOFFSET
};
#else
D3DTEXTURESTAGESTATETYPE Renderer::stateSampD3D[Renderer::SM_SAMP_MAX_STATES] = 
{
    D3DTSS_ADDRESSU,
    D3DTSS_ADDRESSV,
    D3DTSS_ADDRESSW,
    D3DTSS_BORDERCOLOR,
    D3DTSS_MAGFILTER,
    D3DTSS_MINFILTER,
    D3DTSS_MIPFILTER,
    D3DTSS_MIPMAPLODBIAS,
    D3DTSS_MAXMIPLEVEL,
    D3DTSS_MAXANISOTROPY,
    D3DTSS_FORCE_DWORD,
    D3DTSS_FORCE_DWORD,
    D3DTSS_FORCE_DWORD
};
#endif

DWORD Renderer::stateSampValue[Renderer::MAX_TEXTURE_STAGES][Renderer::SM_SAMP_MAX_STATES] = 
{
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		0x00000000,
		D3DTEXF_POINT,
		D3DTEXF_POINT,
		D3DTEXF_NONE,
		0,
		0,
		1,
		0x3f800000, //1.0f
		0,
		256
	},
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		0x00000000,
		D3DTEXF_POINT,
		D3DTEXF_POINT,
		D3DTEXF_NONE,
		0,
		0,
		1,
		0x3f800000, //1.0f
		0,
		256
	},
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		0x00000000,
		D3DTEXF_POINT,
		D3DTEXF_POINT,
		D3DTEXF_NONE,
		0,
		0,
		1,
		0x3f800000, //1.0f
		0,
		256
	},
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		0x00000000,
		D3DTEXF_POINT,
		D3DTEXF_POINT,
		D3DTEXF_NONE,
		0,
		0,
		1,
		0x3f800000, //1.0f
		0,
		256
	},
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		0x00000000,
		D3DTEXF_POINT,
		D3DTEXF_POINT,
		D3DTEXF_NONE,
		0,
		0,
		1,
		0x3f800000, //1.0f
		0,
		256
	},
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		0x00000000,
		D3DTEXF_POINT,
		D3DTEXF_POINT,
		D3DTEXF_NONE,
		0,
		0,
		1,
		0x3f800000, //1.0f
		0,
		256
	},
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		0x00000000,
		D3DTEXF_POINT,
		D3DTEXF_POINT,
		D3DTEXF_NONE,
		0,
		0,
		1,
		0x3f800000, //1.0f
		0,
		256
	},
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_WRAP,
		0x00000000,
		D3DTEXF_POINT,
		D3DTEXF_POINT,
		D3DTEXF_NONE,
		0,
		0,
		1,
		0x3f800000, //1.0f
		0,
		256
	}
};





//
// Render
//

Renderer::Renderer() :
	mCurrentFVF( D3DFVF_XYZ ),
	mClearFlags( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ),
	mSavedWindowStyle( 0 ),
	mSavedWindowExStyle( 0 )
{
}

Renderer::~Renderer ()
{
}

HRESULT Renderer::CreateDevice( HWND hwnd )
{
	if( mpD3D || mpD3DDevice )
	{
		gError( "You have to call Shutdown prior to call CreateDevice second time" );
		return XFXERR_INVALIDCALL;
	}

	//Creating device
#if (__XFX_DIRECTX_VER__ < 9)
	HMODULE d3d8lib = LoadLibraryA( "d3d8.dll" );

	IDirect3D8 * (WINAPI * pDirect3DCreate8) (UINT) = (IDirect3D8 * (WINAPI *) (UINT)) GetProcAddress (d3d8lib, "Direct3DCreate8");
	if (!pDirect3DCreate8)
	{
		gCError( "Incorrect d3d8.dll! Please reinstall DirectX!" );
		return XFXERR_UNKNOWN;
	}

	IDirect3D8 * pd3d = pDirect3DCreate8( D3D_SDK_VERSION );
	if( !pd3d )
	{
		gCError( "Creating D3D object failed!" );
		return XFXERR_UNKNOWN;
	}

#else

	HMODULE d3d9lib = LoadLibraryA( "d3d9.dll" );

	IDirect3D9 * (WINAPI * pDirect3DCreate9) (UINT) = (IDirect3D9 * (WINAPI *) (UINT)) GetProcAddress (d3d9lib, "Direct3DCreate9");
	if( !pDirect3DCreate9 )
	{
		gCError( "Incorrect d3d9.dll! Please reinstall DirectX!" );
		return XFXERR_UNKNOWN;
	}

	IDirect3D9 * pd3d = pDirect3DCreate9( D3D_SDK_VERSION );
	if( !pd3d )
	{
		gCError( "Creating D3D object failed!" );
		return XFXERR_UNKNOWN;
	}

#endif

	mpD3D.reset( pd3d, IUnknownDeleter( ) );

	gMess ("Creating D3D Device:");
	gMess ("...creating D3D object: OK");

	HRESULT hr;
	if( FAILED( hr = TryToCreateD3DDevice( hwnd ) ) )
		return hr;

#if ( __XFX_DIRECTX_VER__ >= 9 )
	mpD3DDevice->SetVertexShader (NULL);
#endif

	TextureState	(0, SM_TEX_ALPHAOP, D3DTOP_MODULATE);
	TextureState	(0, SM_TEX_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);

	SamplerState	(0, SM_SAMP_MAGFILTER, D3DTEXF_LINEAR);
	SamplerState	(0, SM_SAMP_MINFILTER, D3DTEXF_LINEAR);
	SamplerState	(0, SM_SAMP_MIPFILTER, D3DTEXF_LINEAR);

	//Getting adapter information
#if (__XFX_DIRECTX_VER__ < 9)
	D3DADAPTER_IDENTIFIER8 adident;
#else
	D3DADAPTER_IDENTIFIER9 adident;
#endif
	mpD3D->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &adident );

	gMess( "" );
	gMess( "Adapter information:" );
	gMess( "...driver: %s", adident.Driver );
	gMess( "...description: %s", adident.Description );
	gMess( "...driver version: %d.%d.%d.%d", HIWORD( adident.DriverVersion.HighPart ), LOWORD( adident.DriverVersion.HighPart ), HIWORD( adident.DriverVersion.LowPart ), LOWORD( adident.DriverVersion.LowPart ) );

	//Getting memory status
	MEMORYSTATUS ms;

	GlobalMemoryStatus	(&ms);
	gMess ("");
	gMess ("Memory status:");
	gMess ("...memory use: %u%%", ms.dwMemoryLoad);
	gMess ("...total physical memory: %u MB", ms.dwTotalPhys >> 20);
	gMess ("...available physical memory: %u MB", ms.dwAvailPhys >> 20);
	gMess ("...total page file: %u MB", ms.dwTotalPageFile >> 20);
	gMess ("...available page file: %u MB", ms.dwAvailPageFile >> 20);
	gMess ("...available texture memory: %u MB", mpD3DDevice->GetAvailableTextureMem () >> 20);
	gMess ("");

	State	( SM_NORMALIZENORMALS, true );
	State	( SM_ALPHATESTENABLE, true );
	State	( SM_ALPHAFUNC, D3DCMP_GREATER );
	State	( SM_ALPHAREF, 0x00000005 );
	State	( SM_LIGHTING, false );

	mDrawTools.Init( );

	mSavedWindowStyle	= GetWindowLong( hwnd, GWL_STYLE );
	mSavedWindowExStyle	= GetWindowLong( hwnd, GWL_EXSTYLE );

	RECT rect;
	GetClientRect( hwnd, &rect );

	Viewport::Instance( ).Init( rect.right - rect.left, rect.bottom - rect.top );

	return S_OK;
}

void Renderer::Shutdown( )
{
	mDrawTools.Shutdown( );

	gMess( "Releasing D3D device..." );
	if( !mpD3D || !mpD3DDevice )
	{
		gMess( "D3D device was not successfuly created." );
	}

	if( mpD3DDevice )
	{
		for( int i = 0; i < 7; i++ )
            mpD3DDevice->SetTexture( i, NULL );

		mpD3DDevice.reset( );
	}

	mpD3D.reset( );

	gMess( "D3D device has been released." );
}

void Renderer::BeginFrame( )
{
	Clear( ClearFlags( ) );

	mpD3DDevice->BeginScene( );

	mFrameStat.clear( );
}

void Renderer::EndFrame( )
{
	DrawTools ().FlushAll( );

	mpD3DDevice->EndScene( );

	if( mpD3DDevice->Present( NULL, NULL, NULL, NULL ) == D3DERR_DEVICELOST )
		if( mpD3DDevice->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
		{
			gMess( "Device lost" );

			ResetDevice( );
		}
}

void Renderer::Clear( const DWORD& flags )
{
	mpD3DDevice->Clear( 0, NULL, flags, r_background->AsInt( ), 1.0f, 0 );
}

void Renderer::ApplyTexture( const boost::shared_ptr< const ITexture >& tex, unsigned stage )
{
	_ASSERTE (stage < 8);

	const boost::shared_ptr<const ITexture>& stage_tex_ptr		= mActiveTexturesPtr[ stage ];
	const boost::shared_ptr<const ITexture>& tex_ptr			= tex;

	if( stage_tex_ptr.get( ) != tex_ptr.get( ) )
	{
		mActiveTexturesPtr[ stage ] = tex;

		mpD3DDevice->SetTexture( stage, ( tex_ptr ) ? tex_ptr->D3DTex( ) : NULL );
	}

	if( tex_ptr )
	{
		mpD3DDevice->SetTransform( D3DTRANSFORMSTATETYPE( D3DTS_TEXTURE0 + stage ), ( D3DMATRIX * )( &tex_ptr->TextureMatrix( ) ) );
		mpD3DDevice->MultiplyTransform( D3DTRANSFORMSTATETYPE( D3DTS_TEXTURE0 + stage ), ( D3DMATRIX * )( &tex_ptr->Transformation( ) ) );
	}
}

void Renderer::RestoreStates ()
{
	for (int i = 0; i < SM_MAX_STATES; i++)
#if (__XFX_DIRECTX_VER__ < 9)
		if (stateD3D[i] != D3DRS_FORCE_DWORD)
#endif
		mpD3DDevice->SetRenderState (stateD3D[i], stateValue[i]);

	for (unsigned stage = 0; stage < MAX_TEXTURE_STAGES; stage++)
	{
		for (int j = 0; j < SM_TEX_MAX_STATES; j++)
#if (__XFX_DIRECTX_VER__ < 9)
			if (stateTexD3D[j] != D3DTSS_FORCE_DWORD)
#endif
			mpD3DDevice->SetTextureStageState (stage, stateTexD3D[j], stateTexValue[stage][j]);

		for (int k = 0; k < SM_SAMP_MAX_STATES; k++)
#if (__XFX_DIRECTX_VER__ < 9)
			if (stateSampD3D[k] != D3DTSS_FORCE_DWORD)
				mpD3DDevice->SetTextureStageState (stage, stateSampD3D[k], stateSampValue[stage][k]);
#else
			mpD3DDevice->SetSamplerState (stage, stateSampD3D[k], stateSampValue[stage][k]);
#endif
	}

	for (int t = 0; t < MAX_TEXTURE_STAGES; t++)
		mActiveTexturesPtr[t].reset ();
}

void Renderer::State (STATES state, DWORD value)
{
	_ASSERTE (state < SM_MAX_STATES);

	if (stateValue[state] != value)
	{
		stateValue[state] = value;

#if (__XFX_DIRECTX_VER__ < 9)
		if (stateD3D[state] != D3DRS_FORCE_DWORD)
#endif
		mpD3DDevice->SetRenderState (stateD3D[state], value);
	}
}

void Renderer::TextureState (unsigned stage, TEXTURE_STATES state, DWORD value)
{
	_ASSERTE (stage < MAX_TEXTURE_STAGES);
	_ASSERTE (state < SM_TEX_MAX_STATES);

	if (stateTexValue[stage][state] != value)
	{
		stateTexValue[stage][state] = value;

#if (__XFX_DIRECTX_VER__ < 9)
		if (stateTexD3D[stage] != D3DTSS_FORCE_DWORD)
#endif
		mpD3DDevice->SetTextureStageState (stage, stateTexD3D[state], value);
	}
}

void Renderer::SamplerState (unsigned stage, SAMPLER_STATES state, DWORD value)
{
	_ASSERTE (stage < MAX_TEXTURE_STAGES);
	_ASSERTE (state < SM_SAMP_MAX_STATES);

	if (stateSampValue[stage][state] != value)
	{
		stateSampValue[stage][state] = value;

#if (__XFX_DIRECTX_VER__ < 9)
		if (stateSampD3D[state] != D3DTSS_FORCE_DWORD)
			mpD3DDevice->SetTextureStageState (stage, stateSampD3D[state], value);
#else
		mpD3DDevice->SetSamplerState (stage, stateSampD3D[state], value);
#endif
	}
}

void Renderer::FVF (const int& fvf)
{
	if (mCurrentFVF != fvf)
	{
		mCurrentFVF = fvf;

#if (__XFX_DIRECTX_VER__ < 9)
		mpD3DDevice->SetVertexShader (fvf);
#else
		mpD3DDevice->SetFVF (fvf);
#endif
	}
}

void Renderer::RenderPrimitive( const boost::shared_ptr< const Shader> & shd, const class ShaderParams& consts, const boost::function0< void >& render_fn )
{
	if( shd )
	{
		shd->RenderEffect( consts, render_fn );
	}
}

HRESULT Renderer::ResetDevice (unsigned width, unsigned height)
{
	if (!mpD3DDevice)
		return S_OK;

	if (mD3DPP.Windowed)
	{
		if (width && height)
		{
			mD3DPP.BackBufferWidth = width;
			mD3DPP.BackBufferHeight = height;

			HRESULT hr = ResetDevice ();
			if (FAILED (hr))
				return hr;
		}
	}

	return S_OK;
}

HRESULT Renderer::ResetDevice ()
{
	Mat4 w, v, p;
	GetTransform( D3DTS_WORLD, w );
	GetTransform( D3DTS_VIEW, v );
	GetTransform( D3DTS_PROJECTION, p );

	XFX_PLACE_DEVICE_LOCK;

	mInvalidateCallbacks( true );

	HRESULT hr = mpD3DDevice->Reset (&mD3DPP);
	if (FAILED (hr))
		return hr;

	mInvalidateCallbacks( false );

	SetTransform( D3DTS_WORLD, w );
	SetTransform( D3DTS_VIEW, v );
	SetTransform( D3DTS_PROJECTION, p );

	RestoreStates ();

	DWORD fvf = mCurrentFVF;
	mCurrentFVF = 0;
	FVF( fvf );

	return hr;
}

void Renderer::SetTransform( const D3DTRANSFORMSTATETYPE& transform, const Mat4& matr )
{
	if( transform == D3DTS_PROJECTION || transform == D3DTS_VIEW )
	{
		//DrawTools ().FlushAll ();

		mIsVPCached = false;
	}

	_ASSERTE( mpD3DDevice );
	mpD3DDevice->SetTransform( transform, ( D3DMATRIX * )( &matr ) );
}

void Renderer::CacheVP () const
{
	Mat4 proj;
	Mat4 view;

	GetTransform (D3DTS_PROJECTION, proj);
	GetTransform (D3DTS_VIEW, view);

	mIsVPCached	= true;
	mCachedVP	= view * proj;
}

HRESULT Renderer::TryToCreateD3DDevice( HWND hwnd )
{
	HRESULT hr = XFXERR_UNKNOWN;

	D3DDISPLAYMODE d3ddm;
	mpD3D->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm);

	gMess ("...creating D3D device...");

	ZeroMemory (&mD3DPP, sizeof (mD3DPP));
	mD3DPP.Windowed						= true;
	mD3DPP.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	mD3DPP.BackBufferFormat				= d3ddm.Format;
	mD3DPP.EnableAutoDepthStencil		= true;

	if( r_debug_perfhud->AsInt( ) )
	{
		gWarn ("...creating NVPerfHUD device (only for debugging)");

		mD3DPP.AutoDepthStencilFormat	= D3DFMT_D24X8;

#if (__XFX_DIRECTX_VER__ < 9)
		IDirect3DDevice8 * dev;
		mD3DPP.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
#else
		IDirect3DDevice9 * dev;
		mD3DPP.PresentationInterval		= (mD3DCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE) ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_ONE;
#endif

		HRESULT hr = mpD3D->CreateDevice (
			mpD3D->GetAdapterCount () - 1,
			D3DDEVTYPE_REF,
			hwnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&mD3DPP,
			&dev );

		if( SUCCEEDED( hr ) )
			mpD3DDevice.reset (dev, IUnknownDeleter ());

		return hr;
	}

	boost::array< std::pair< D3DDEVTYPE, const char * >, 3 > devicetypes;
	devicetypes[ 1 ] = std::pair< D3DDEVTYPE, const char * >( D3DDEVTYPE_HAL, "hal" );
	devicetypes[ 2 ] = std::pair< D3DDEVTYPE, const char * >( D3DDEVTYPE_REF, "ref" );
	devicetypes[ 0 ] = devicetypes[ r_device_type->Value( ) == "hal" ? 1 : 2 ];

	boost::array< std::pair< D3DFORMAT, const char * >, 5 > dsformats;
	dsformats[ 1 ] = std::pair< D3DFORMAT, const char * >( D3DFMT_D32, "d32" );
	dsformats[ 2 ] = std::pair< D3DFORMAT, const char * >( D3DFMT_D24S8, "d24s8" );
	dsformats[ 3 ] = std::pair< D3DFORMAT, const char * >( D3DFMT_D24X8, "d24x8" );
	dsformats[ 4 ] = std::pair< D3DFORMAT, const char * >( D3DFMT_D16, "d16" );

	if( r_device_depth_stencil->Value( ) == "d32" )
		dsformats[ 0 ] = dsformats[ 1 ];
	else if( r_device_depth_stencil->Value( ) == "d24s8" )
		dsformats[ 0 ] = dsformats[ 2 ];
	else if( r_device_depth_stencil->Value( ) == "d24x8" )
		dsformats[ 0 ] = dsformats[ 3 ];
	else
		dsformats[ 0 ] = dsformats[ 4 ];

	boost::array< std::pair< DWORD, const char * >, 4 > vptypes;
	vptypes[ 1 ] = std::pair< DWORD, const char * >( D3DCREATE_HARDWARE_VERTEXPROCESSING, "hardware" );
	vptypes[ 2 ] = std::pair< DWORD, const char * >( D3DCREATE_MIXED_VERTEXPROCESSING, "mixed" );
	vptypes[ 3 ] = std::pair< DWORD, const char * >( D3DCREATE_SOFTWARE_VERTEXPROCESSING, "software" );

	if( r_device_vp->Value( ) == "hardware" )
		vptypes[ 0 ] = vptypes[ 1 ];
	else if( r_device_vp->Value( ) == "mixed" )
		vptypes[ 0 ] = vptypes[ 2 ];
	else
		vptypes[ 0 ] = vptypes[ 3 ];

	for(
		boost::array<std::pair<D3DDEVTYPE, const char *>, 3>::const_iterator devit = devicetypes.begin ();
		devit != devicetypes.end ();
		devit++
		)
	{
		if( r_device_type->Value( ).empty( ) && devit == devicetypes.begin( ) )
			continue;

		mpD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, ( *devit ).first, &mD3DCaps );

		//Checking depth/stencil formats
		boost::array< std::pair< D3DFORMAT, const char * >, 4 >::const_iterator dsfmt;

		for( dsfmt = dsformats.begin (); dsfmt != dsformats.end (); dsfmt++)
		{
			if( r_device_depth_stencil->Value( ).empty( ) && dsfmt == dsformats.begin( ) )
				continue;

			String check_device_str( boost::str( StringFormat( "...checking device format: %s %s ... " ) % ( *devit ).second % ( *dsfmt ).second ) );

			if( SUCCEEDED ( mpD3D->CheckDeviceFormat(
					D3DADAPTER_DEFAULT,
					( *devit ).first,
					d3ddm.Format,
					D3DUSAGE_DEPTHSTENCIL,
					D3DRTYPE_SURFACE,
					( *dsfmt ).first ) )
				)
			{
				check_device_str += "passed";
				gMess( "%s", check_device_str.c_str( ) );

				String check_depth_stencil( "...checking depth stencil match... " );

				if( SUCCEEDED( mpD3D->CheckDepthStencilMatch (
						D3DADAPTER_DEFAULT,
						( *devit ).first,
						d3ddm.Format,
						d3ddm.Format,
						( *dsfmt ).first ) ) )
				{
					check_depth_stencil += "passed";
					gMess( "%s", check_depth_stencil.c_str( ) );

					break;
				}
				else
				{
					check_depth_stencil += "failed";
					gMess( "%s", check_depth_stencil.c_str( ) );
				}
			}
			else
			{
				check_device_str += "failed";
				gMess( "%s", check_device_str.c_str( ) );
			}
		}

		if( dsfmt == dsformats.end( ) )
			continue;

		mD3DPP.AutoDepthStencilFormat	= (*dsfmt).first;
#if (__XFX_DIRECTX_VER__ < 9)
		IDirect3DDevice8 * dev;
		mD3DPP.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
#else
		IDirect3DDevice9 * dev;
		mD3DPP.PresentationInterval		= (mD3DCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE) ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_ONE;
#endif

		for( boost::array< std::pair< DWORD, const char * >, 4 >::const_iterator vpit = vptypes.begin( ); vpit != vptypes.end( ); vpit++ )
		{
			if( r_device_vp->Value( ).empty( ) && vpit == vptypes.begin( ) )
				continue;

#pragma message( "FIXME: D3DCREATE_MULTITHREADED" )
			if( SUCCEEDED( hr = mpD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				( *devit ).first,
				hwnd,
				//( *vpit ).first,
				( *vpit ).first | D3DCREATE_MULTITHREADED,		// hack for stability
				&mD3DPP,
				&dev ) ) )
			{
				mpD3DDevice.reset (dev, IUnknownDeleter ());

				gMess ("...device type: %s", (*devit).second);
				gMess ("...vertex processing: %s", (*vpit).second);
				gMess ("...depth\\stencil format: %s", (*dsfmt).second);

				//if (!(caps.DevCaps2 & D3DDEVCAPS2_STREAMOFFSET))
					//Console::Instance ().Output (Console::MessageType::WARNING, "Device doesn't support stream offsets!\n");

				RECT client_rect;
				GetClientRect( hwnd, &client_rect );

				mD3DPP.BackBufferWidth	= client_rect.right - client_rect.left;
				mD3DPP.BackBufferHeight	= client_rect.bottom - client_rect.top;

				TestDeviceCaps( );

				return S_OK;
			}
		}
	}
	gCError ("Failed to create device! (hr == 0x%X)", hr);

	return hr;
}

HRESULT Renderer::Fullscreen (bool isfullscreen, unsigned width, unsigned height, const unsigned * refreshrate, const D3DFORMAT * format)
{
	HRESULT hr = XFXERR_INVALIDCALL;

	if (mpD3DDevice)
	{
		if ((width) && (height))
		{
			mD3DPP.BackBufferWidth	= width;
			mD3DPP.BackBufferHeight	= height;
			mD3DPP.Windowed			= !isfullscreen;

			if (format)
			{
				//HACK
				//if (isfullscreen)
#pragma message( "FIXME: provide user choice between depth\\stencil formats" )
					mD3DPP.AutoDepthStencilFormat = D3DFMT_D16;//(*format != D3DFMT_X8R8G8B8) ? D3DFMT_D16 : D3DFMT_D24X8;

				mD3DPP.BackBufferFormat = *format;
			}

			if (refreshrate)
				mD3DPP.FullScreen_RefreshRateInHz = *refreshrate;
			else
				mD3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

			if (!isfullscreen)
			{
				mD3DPP.FullScreen_RefreshRateInHz = 0;

#if( __XFX_DIRECTX_VER__ < 9 )
				mD3DPP.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
#endif
			}
			else
			{
#if( __XFX_DIRECTX_VER__ < 9 )
				mD3DPP.FullScreen_PresentationInterval = ( mD3DCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE ) ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_DEFAULT;
#endif
			}

			OnFullscreenChange( isfullscreen, width, height );
			mFullscreenChangeEvents( isfullscreen, width, height );

			gMess( 
				"Setting videomode: %s, %dx%d@%d, 0x%X", 
				isfullscreen ? "fullscreen" : "windowed", 
				mD3DPP.BackBufferWidth, mD3DPP.BackBufferHeight, 
				mD3DPP.FullScreen_RefreshRateInHz,
				mD3DPP.BackBufferFormat
				);

			hr = ResetDevice ();
		} 
	}

	return hr;
}

HRESULT Renderer::TakeScreenshot( const String& filename, bool save_jpeg ) const
{
	//try to get front buffer
#if (__XFX_DIRECTX_VER__ < 9)
	LPDIRECT3DSURFACE8 surf;
#else
	LPDIRECT3DSURFACE9 surf;
#endif

	unsigned w, h;

	RECT r;
	GetWindowRect (GetDesktopWindow (), &r);

	w = r.right - r.left;
	h = r.bottom - r.top;

	// use try lock since TakeScreenshot can be called from other thread and we don't want to wait for
	// device when it get ready to take screenshot.
#ifdef __XFX_ENABLE_MULTITHREADING__
	boost::recursive_try_mutex::scoped_try_lock _xfx_device_lock( Renderer::Instance( ).GetDeviceMutex( ) );
	//XFX_PLACE_DEVICE_LOCK;
#endif

	HRESULT hr;
#if (__XFX_DIRECTX_VER__ < 9)
	if (FAILED (hr = mpD3DDevice->CreateImageSurface (w, h, D3DFMT_A8R8G8B8, &surf)))
		return hr;
#else
	if (FAILED (hr = mpD3DDevice->CreateOffscreenPlainSurface (w, h, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surf, NULL)))
		return hr;
#endif

	boost::shared_ptr<IUnknown> surf_ptr (surf, IUnknownDeleter ());

#if (__XFX_DIRECTX_VER__ < 9)
	if (FAILED (hr = mpD3DDevice->GetFrontBuffer (surf)))
#else
	if (FAILED (hr = mpD3DDevice->GetFrontBufferData (0, surf)))
#endif
		return hr;

	//try to create file
	FILE * file = fopen (filename.c_str (), "wb");
	if (!file)
	{
		gWarn ("Cannot create file \"%s\" to save screenshot!", filename.c_str ());
		return XFXERR_IO;
	}

	D3DLOCKED_RECT rect;
	if (FAILED (hr = surf->LockRect (&rect, NULL, D3DLOCK_READONLY)))
	{
		fclose (file);

		return hr;
	}

	if( save_jpeg )
	{
		jpeg_compress_struct cinfo;
		jpeg_error_mgr jerr;

		cinfo.err = jpeg_std_error( &jerr );
		jpeg_create_compress( &cinfo );

		jpeg_stdio_dest( &cinfo, file );

		cinfo.image_width = w;
		cinfo.image_height = h;
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;

		jpeg_set_defaults( &cinfo );

		jpeg_start_compress( &cinfo, TRUE );

		JSAMPROW row_pointer[ 1 ];

		boost::scoped_array< unsigned char > scanline_rgb( new unsigned char[ w * 3 ] );

		unsigned char * buf = reinterpret_cast< unsigned char * >( rect.pBits );
		while( cinfo.next_scanline < cinfo.image_height )
		{
			for( unsigned x = 0; x < w; x++ )
			{
				scanline_rgb[ x * 3 + 0 ] = *( buf + cinfo.next_scanline * rect.Pitch + x * 4 + 2 );
				scanline_rgb[ x * 3 + 1 ] = *( buf + cinfo.next_scanline * rect.Pitch + x * 4 + 1 );
				scanline_rgb[ x * 3 + 2 ] = *( buf + cinfo.next_scanline * rect.Pitch + x * 4 + 0 );
			}

			row_pointer[ 0 ] = scanline_rgb.get( );
			jpeg_write_scanlines( &cinfo, row_pointer, 1 );
		}

		jpeg_finish_compress( &cinfo );
		jpeg_destroy_compress( &cinfo );
	}
	else
	{

#pragma pack (push, 1)
		struct TGAHeader
		{
			char		idlength;
			char		colormaptype;
			char		datatypecode;
			short int	colormaporigin;
			short int	colormaplength;
			char		colormapdepth;
			short int	x_origin;
			short int	y_origin;
			short		width;
			short		height;
			char		bitsperpixel;
			char		imagedescriptor;
		};
	#pragma pack (pop)

		TGAHeader header;
		header.idlength			= 0;
		header.colormaptype		= 0;
		header.datatypecode		= 2;
		header.colormaporigin	= 0;
		header.colormaplength	= 0;
		header.colormapdepth	= 0;
		header.x_origin			= 0;
		header.y_origin			= 0;
		header.width			= static_cast< short >( w );
		header.height			= static_cast< short >( h );
		header.bitsperpixel		= 24;
		header.imagedescriptor	= 0;

		fwrite (&header, 1, sizeof (TGAHeader), file);

		char * buf = reinterpret_cast<char *> (rect.pBits) + (h - 1) * rect.Pitch;
		for (unsigned y = 0; y < h; y++, buf -= rect.Pitch)
			for (unsigned x = 0; x < w; x++)
			{
				fputc (*(buf + x * 4 + 0), file);
				fputc (*(buf + x * 4 + 1), file);
				fputc (*(buf + x * 4 + 2), file);
			}
	}

	surf->UnlockRect ();

	fclose (file);

	gMess ("Screenshot \"%s\" saved!", filename.c_str ());

	return hr;
}

void Renderer::TestDeviceCaps( ) const
{
	gMess( "" );
	gMess( "Testing device caps..." );

	if( !( D3DCaps( ).TextureCaps & D3DPTEXTURECAPS_ALPHA ) )
		gWarn( "Device doesn't support alpha channel in textures!" );

	if( !( D3DCaps( ).TextureCaps & D3DPTEXTURECAPS_CUBEMAP ) )
		gWarn( "Device doesn't support cubemap textures!" );

	if( !( D3DCaps( ).TextureCaps & D3DPTEXTURECAPS_MIPCUBEMAP ) )
		gWarn( "Device doesn't support mipmapped cube textures!" );

	if( !( D3DCaps( ).TextureCaps & D3DPTEXTURECAPS_MIPMAP ) )
		gWarn( "Device doesn't support mipmapped textures!" );

	if( !( D3DCaps( ).TextureCaps & D3DPTEXTURECAPS_MIPVOLUMEMAP ) )
		gWarn( "Device doesn't support mipmapped volume textures!" );

	if( ( D3DCaps( ).TextureCaps & D3DPTEXTURECAPS_SQUAREONLY ) )
		gWarn( "Device doesn't support non square textures!" );

	if( !( D3DCaps( ).TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP ) )
		gWarn( "Device doesn't support volume textures!" );

	gMess( "Max texture width: %d", D3DCaps( ).MaxTextureWidth );
	gMess( "Max texture height: %d", D3DCaps( ).MaxTextureHeight );
	gMess( "Max texture repeat: %d", D3DCaps( ).MaxTextureRepeat );
	gMess( "Max texture aspect ratio: %d", D3DCaps( ).MaxTextureAspectRatio );
	gMess( "Max texture blend stages: %d", D3DCaps( ).MaxTextureBlendStages );
	gMess( "Max simultaneous textures: %d", D3DCaps( ).MaxSimultaneousTextures );
	gMess( "Max volume extent: %d", D3DCaps( ).MaxVolumeExtent );
	gMess( "Max anisotropy: %d", D3DCaps( ).MaxAnisotropy );
	gMess( "Max active lights: %d", D3DCaps( ).MaxActiveLights );
	gMess( "Max primitive count: %d", D3DCaps( ).MaxPrimitiveCount );
	gMess( "Max vertex index: %d", D3DCaps( ).MaxVertexIndex );
	gMess( "Max streams: %d", D3DCaps( ).MaxStreams );
	gMess( "Max stream stride: %d", D3DCaps( ).MaxStreamStride );
	gMess( "Vertex shader version: %X", D3DCaps( ).VertexShaderVersion );
	gMess( "Max vertex shader const: %d", D3DCaps( ).MaxVertexShaderConst );
	gMess( "Pixel shader version: %X", D3DCaps( ).PixelShaderVersion );

	gMess( "" );
}

void Renderer::DrawFrameStatistics( const Font& fnt, const boost::shared_ptr< const class Shader >& white_shader ) const
{
	float cur_x = 100.0f;
	float cur_y = 100.0f;

	WString text = fromMBCS( boost::str( xfx::StringFormat( 
		"Num batches:     %d\n"
		"Num tris:        %d\n"
		"Num lines:       %d\n"
		"Num sprites:     %d\n"
		"Num user tris:   %d" ) % 
		mFrameStat.num_batches % mFrameStat.num_tris % mFrameStat.num_lines % 
		mFrameStat.num_sprites % mFrameStat.num_usertris ) );

	Renderer::Instance( ).GetDrawTools( ).PushDraw2DSprite(
		cur_x + 80.0f, cur_y + 40.0f,
		180.0f, 100.0f, 0, 0xa0000000, white_shader );

	Renderer::Instance( ).GetDrawTools( ).PushDraw2DText(
		fnt,
		cur_x, cur_y,
		1.0f, 1.0f,
		text
		);
}

void Renderer::OnFullscreenChange( bool isfullscreen, unsigned width, unsigned height )
{
	HWND wnd = gGetApplication( ).hWnd( );

	if( !isfullscreen )
	{
		SetWindowLong ( wnd, GWL_STYLE, mSavedWindowStyle );
		SetWindowLong ( wnd, GWL_EXSTYLE, mSavedWindowExStyle );
	}
	else
	{
		SetWindowLong ( wnd, GWL_STYLE, WS_VISIBLE );
	}

	SetWindowPos ( wnd, isfullscreen ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, width, height, SWP_SHOWWINDOW );
}




_XFX_END
//  File xfx_renderer_include.h                                               |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_renderer_include.h 
 *	\brief Base file to include for Renderer library.
 */





#include "xfx.h"
#include "xfx_drawtools.h"
#include "xfx_font.h"
#include "xfx_indexbuffer.h"
#include "xfx_renderer.h"
#include "xfx_renderer_cvars.h"
#include "xfx_shader.h"
#include "xfx_texture.h"
#include "xfx_vertexbuffer.h"
#include "xfx_viewer.h"
#include "xfx_viewport.h"



// include libraries
#ifdef _DLL
#pragma comment ( lib, "jpeg-6brd.lib" )

#ifdef _DEBUG
#pragma comment ( lib, "xfx_mathdd.lib" )
#else
#pragma comment ( lib, "xfx_mathrd.lib" )
#endif

#else
#pragma comment ( lib, "jpeg-6br.lib" )

#ifdef _DEBUG
#pragma comment ( lib, "xfx_mathd.lib" )
#else
#pragma comment ( lib, "xfx_mathr.lib" )
#endif

#endif

#if( __XFX_DIRECTX_VER__ < 9 )
#pragma comment ( lib, "d3dx8.lib" )
#else
#pragma comment ( lib, "d3dx9.lib" )
#endif
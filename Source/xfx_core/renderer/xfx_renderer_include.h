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



//
// Linker options
//

#pragma comment ( lib, "xfx_renderer" LIB_SUFFIX )

#pragma comment ( lib, "jpeg-6b" LIB_3RD_SUFFIX )


#if( __XFX_DIRECTX_VER__ < 9 )
#pragma comment ( lib, "d3dx8.lib" )
#else
#pragma comment ( lib, "d3dx9.lib" )
#endif
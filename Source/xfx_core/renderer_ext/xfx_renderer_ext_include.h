//  File xfx_renderer_ext_include.h                                           |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_renderer_ext_include.h 
 *	\brief Base file to include for Renderer Extensions library.
 */





#include "xfx.h"
#include "xfx_mesh.h"
#include "xfx_particlesystem.h"




#if ( defined _DEBUG )
#pragma comment ( lib, "xfx_renderer_ext_debug.lib" )
#else 
#if ( defined __XFX_SHIPPING__ )
#pragma comment ( lib, "xfx_renderer_ext_shipping.lib" )
#else
#pragma comment ( lib, "xfx_renderer_ext_release.lib" )
#endif
#endif

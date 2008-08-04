//  File xfx_serialization_include.h                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_serialization_include.h 
 *	\brief Base file to include for Serialization library.
 */






#include "xfx.h"
#include "xfx_main_serialization_impl.h"
#include "xfx_math_serialization_impl.h"
#include "xfx_renderer_ext_serialization_impl.h"
#include "xfx_renderer_serialization_impl.h"
#include "xfx_ui_serialization_impl.h"
#include "xfx_utility_serialization_impl.h"



#if ( defined _DEBUG )
#pragma comment ( lib, "xfx_serialization_debug.lib" )
#else 
#if ( defined __XFX_SHIPPING__ )
#pragma comment ( lib, "xfx_serialization_shipping.lib" )
#else
#pragma comment ( lib, "xfx_serialization_release.lib" )
#endif
#endif

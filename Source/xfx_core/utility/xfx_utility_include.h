//  File xfx_utility_include.h                                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_utility_include.h 
 *	\brief Base file to include for UI library.
 */





#include "xfx.h"
#include "xfx_singleton.h"
#include "xfx_string.h"
#include "xfx_utils.h"
#include "xfx_argb.h"
#include "xfx_cache.h"
#include "xfx_envelope.h"
#include "xfx_log.h"
#include "xfx_profiler.h"
#include "xfx_script.h"





#if ( defined _DEBUG )
#pragma comment ( lib, "xfx_utility_debug.lib" )
#else 
#if ( defined __XFX_SHIPPING__ )
#pragma comment ( lib, "xfx_utility_shipping.lib" )
#else
#pragma comment ( lib, "xfx_utility_release.lib" )
#endif
#endif

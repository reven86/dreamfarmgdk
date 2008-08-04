//  File xfx_lua_include.h                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_lua_include.h 
 *	\brief Base file to include for Lua library.
 */





#include "xfx.h"
#include "xfx_lua_sm.h"
#include "xfx_luascript.h"





#if ( defined _DEBUG )
#pragma comment ( lib, "xfx_lua_debug.lib" )
#else 
#if ( defined __XFX_SHIPPING__ )
#pragma comment ( lib, "xfx_lua_shipping.lib" )
#else
#pragma comment ( lib, "xfx_lua_release.lib" )
#endif
#endif

#pragma comment ( lib, "lualib_release.lib" )
#pragma comment ( lib, "luabind_release.lib" )
#pragma comment ( lib, "lauxlib_release.lib" )

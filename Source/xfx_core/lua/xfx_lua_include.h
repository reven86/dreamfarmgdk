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





//
// Linker options
//

#pragma comment ( lib, "xfx_lua" LIB_SUFFIX )


#pragma comment ( lib, "lualib" LIB_3RD_SUFFIX )
#pragma comment ( lib, "luabind" LIB_3RD_SUFFIX )
#pragma comment ( lib, "lauxlib" LIB_3RD_SUFFIX )

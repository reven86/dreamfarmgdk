//  File xfx_gui_include.h                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_gui_include.h 
 *	\brief Base file to include for GUI library.
 */





#include "xfx.h"
#include "xfx_gui.h"
#include "xfx_cegui_renderer.h"
#include "xfx_cegui_texture.h"
#include "xfx_cegui_resourceprovider.h"
#include "xfx_cegui_luabind_scriptmodule.h"




// include libraries
#ifdef _DEBUG
#pragma comment ( lib, "xfx_gui_debug.lib" )
#else
#if ( defined __XFX_SHIPPING__ )
#pragma comment ( lib, "xfx_gui_shipping.lib" )
#else
#pragma comment ( lib, "xfx_gui_release.lib" )
#endif

#endif

#pragma comment ( lib, "ceguibase_release.lib" )
#pragma comment ( lib, "ceguilua_release.lib" )
#pragma comment ( lib, "ceguiexpatparser_release.lib" )
#pragma comment ( lib, "ceguifalagardwrbase_release.lib" )
#pragma comment ( lib, "pcre_release.lib" )
#pragma comment ( lib, "freetype237_release.lib" )
#pragma comment ( lib, "expatw_release.lib" )
#pragma comment ( lib, "tolua++_release.lib" )
#pragma comment ( lib, "winmm.lib" )

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



//
// Linker options
//

#pragma comment ( lib, "xfx_gui" LIB_SUFFIX )

#pragma comment ( lib, "ceguibase" LIB_3RD_SUFFIX )
#pragma comment ( lib, "ceguilua" LIB_3RD_SUFFIX )
#pragma comment ( lib, "ceguiexpatparser" LIB_3RD_SUFFIX )
#pragma comment ( lib, "ceguifalagardwrbase" LIB_3RD_SUFFIX )
#pragma comment ( lib, "pcre" LIB_3RD_SUFFIX )
#pragma comment ( lib, "freetype237" LIB_3RD_SUFFIX )
#pragma comment ( lib, "expat" LIB_3RD_SUFFIX )
#pragma comment ( lib, "tolua++" LIB_3RD_SUFFIX )

#pragma comment ( lib, "winmm.lib" )

//  File pch.h                                                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file pch.h 
 *	\brief Precompiled header.
 */


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define NOMINMAX


// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#pragma warning (disable:4996) // ...was declared deprecated


//
// Boost
//

#include <boost/signals.hpp>



//
// XFX headers
//

#include "utility\xfx_utility_include.h"
#include "main\xfx_main_include.h"
#include "math\xfx_math_include.h"
#include "renderer\xfx_renderer_include.h"
#include "lua\xfx_lua_include.h"
#include "gui\xfx_gui_include.h"


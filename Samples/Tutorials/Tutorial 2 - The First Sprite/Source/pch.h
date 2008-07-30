//  File pch.h                                                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file pch.h 
 *	\brief Precompiled header.
 */


#define _CRT_SECURE_NO_DEPRECATE

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


//
// Boost
//

#include <boost/signals.hpp>



//
// XFX headers
//

#include "utility\xfx_utility_include.h"
#include "main\xfx_main_include.h"
#include "renderer\xfx_renderer_include.h"


//
// Linker options
//

#ifdef _DEBUG
#pragma comment ( lib, "xfx_utilityd.lib" )
#pragma comment ( lib, "xfx_maind.lib" )
#pragma comment ( lib, "xfx_rendererd.lib" )
#else
#pragma comment ( lib, "xfx_utilityr.lib" )
#pragma comment ( lib, "xfx_mainr.lib" )
#pragma comment ( lib, "xfx_rendererr.lib" )
#endif
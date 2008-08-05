//  File shared.h                                                             |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file shared.h 
 *	\brief Defines macros and other preprocessor stuff included in all libraries.
 */




#ifdef _DEBUG
#define LIB_SUFFIX "_debug.lib"
#define LIB_3RD_SUFFIX "_debug.lib"
#else
#define LIB_3RD_SUFFIX "_release.lib"
#ifdef __XFX_SHIPPING__
#define LIB_SUFFIX "_shipping.lib"
#else
#define LIB_SUFFIX "_release.lib"
#endif
#endif


#define NOMINMAX


#pragma warning (disable:4786)	//identifier was truncated to '255' characters...
#pragma warning (disable:4800)	//identifier was truncated to '255' characters...
#pragma warning (disable:4996)	//stdext::hash_map
#pragma warning (disable:4267)	//convert warning
#pragma warning (disable:4512)	//cannot generate assignment operator
#pragma warning (disable:4201)	//nameless struct/union



#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined (MAKEFOURCC) */


//Namespace
#define _XFX_BEGIN namespace xfx {
#define _XFX_END }

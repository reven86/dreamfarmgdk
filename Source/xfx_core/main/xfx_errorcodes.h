//  File xfx_errorcodes.h                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_errorcodes.h 
 *	\brief Library error codes definitions.
 */


#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;

#endif // !_HRESULT_DEFINED

#ifndef MAKE_HRESULT
#define MAKE_HRESULT(sev,fac,code) \
    ((HRESULT) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )
#endif


//! \hideinitializer
#define _FAC_XFX  0x777
//! \hideinitializer
#define MAKE_XFXHRESULT( code )  MAKE_HRESULT( 1, _FAC_XFX, code )

/*!	\brief Error codes definition.
 *	\ingroup MainGroup
 */
enum _XFXERR {
    XFXERR_INVALIDCALL				= MAKE_XFXHRESULT(1),		//!< Invalid call. One of parameters is wrong.
    XFXERR_OUTOFMEMORY				= MAKE_XFXHRESULT(2),		//!< Not enough memory.
    XFXERR_IO						= MAKE_XFXHRESULT(3),		//!< IO error (primary, with file system). 
    XFXERR_NOTFOUND					= MAKE_XFXHRESULT(4),		//!< Something was not found.
    XFXERR_UNKNOWN					= MAKE_XFXHRESULT(5),		//!< Unknown error.
    XFXERR_OVERFLOW					= MAKE_XFXHRESULT(6),		//!< Overflow.
};




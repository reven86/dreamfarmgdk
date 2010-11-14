//  File xfx.h                                                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx.h 
 *	\brief Base configuration file for library.
 */





#include "../shared.h"







//
// Additional libraries
//

#include <crtdbg.h>		//search for memory leaks

// OpenMP
#if _MSC_VER < 1500
#include <omp.h>
#endif

#pragma warning( push )
#pragma warning( disable : 4244 )	// possible loss of data in conversion
#pragma warning( disable : 4201 )	// nameless struct/union
#pragma warning( disable : 4510 )	// can't generate default constructor for ParticleSystem::SystemsType
#pragma warning( disable : 4610 )	// type cannot be instatiaated, user-defined constructor required
#pragma warning( disable : 4127 )	// conditional expression is constant
#pragma warning( disable : 4100 )	// unreferenced formal parameter
#pragma warning( disable : 4121 )	// alignment of a member was sensitive to packing

// Boost
#define BOOST_PYTHON_MAX_ARITY 25
#define BOOST_PYTHON_STATIC_LIB
//#define Py_NO_ENABLE_SHARED
#ifdef _DEBUG
//#define BOOST_DEBUG_PYTHON
//#define BOOST_LINKING_PYTHON
#endif
#include <boost/python.hpp>
#ifdef _DEBUG
//#undef BOOST_DEBUG_PYTHON
//#undef BOOST_LINKING_PYTHON
#endif

// forward definition to use serialization
namespace boost { namespace serialization {
	class access;
} };


// D3D
#if( __XFX_DIRECTX_VER__ < 9 )
#include <d3d8.h>
#include <d3dx8.h>
#else
#include <d3d9.h>
#include <d3dx9.h>
#endif



// XFX
#include "xfx_py_bindings.h"

#pragma warning( pop )

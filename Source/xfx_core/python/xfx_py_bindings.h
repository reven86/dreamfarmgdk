//  File xfx_py_bindings.h                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_py_bindings.h 
 *	\brief File contains Python bindings (ready to process by Py++) to all modules.
 */



#include "../shared.h"
#include "../main/xfx_errorcodes.h"
#include <string>
#include <hash_map>
#include <boost/cstdint.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/signal.hpp>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

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



#include "../utility/xfx_utility_include.h"
#include "../main/xfx_main_include.h"
#include "../math/xfx_math_include.h"
#include "../renderer/xfx_renderer_include.h"
#include "../renderer_ext/xfx_renderer_ext_include.h"
//#include "../serialization/xfx_serialization_include.h"
//#include "../gui/xfx_gui_include.h"


#ifdef __GCCXML__
void f( )
{
	using namespace xfx;

	sizeof( Cache< ParticleSystem > );
	sizeof( Cache< Mesh > );
	sizeof( Cache< CubemapTexture > );
	sizeof( Cache< Texture > );
	sizeof( Cache< Shader > );
	sizeof( Cache< Effect > );
	sizeof( Cache< Font > );
}

#endif // __GCCXML__
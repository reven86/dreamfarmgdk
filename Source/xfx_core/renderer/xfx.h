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

// STL
//#include <map>
//#include <list>
#include <numeric>
//#include <algorithm>
#include <vector>
//#include <deque>
//#include <memory>
//#include <string>
//#include <stack>
//#include <iterator>
//#include <functional>
//#include <bitset>
//#include <set>
//#include <iostream>
//#include <fstream>
#include <hash_map>
#include <limits>

// Boost
#pragma warning( push )
#pragma warning( disable: 4244 ) // possible loss of data in conversion

#include <boost/array.hpp>
#include <boost/scoped_ptr.hpp>
//#include <boost/weak_ptr.hpp>
//#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>
//#include <boost/any.hpp>
#include <boost/cast.hpp>
//#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
//#include <boost/mem_fn.hpp>
//#include <boost/shared_container_iterator.hpp>
//#include <boost/function_output_iterator.hpp>
//#include <boost/iterator/filter_iterator.hpp>
//#include <boost/iterator/permutation_iterator.hpp>
//#include <boost/iterator/indirect_iterator.hpp>
//#include <boost/utility.hpp>
//#include <boost/pool/pool_alloc.hpp>
#include <boost/functional/hash.hpp>
//#include <boost/function.hpp>
#include <boost/signals.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/format.hpp>

//#pragma managed(push, off)
//extern "C" void tss_cleanup_implemented(void) {}
//#include <boost/thread/recursive_mutex.hpp>
//#pragma managed(pop)

#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <boost/circular_buffer.hpp>

#pragma warning( pop )

// Boost.MPL
//#include <boost/mpl/vector.hpp>
//#include <boost/mpl/vector_c.hpp>
//#include <boost/mpl/min_max.hpp>

// forward definition to use serialization
namespace boost { namespace serialization {
	class access;
} };


// XFX
#include "utility\xfx_singleton.h"
#include "utility\xfx_profiler.h"
#include "utility\xfx_utils.h"
#include "utility\xfx_cache.h"
#include "utility\xfx_script.h"
#include "utility\xfx_log.h"
#include "utility\xfx_argb.h"
#include "math\xfx_aabbtree.h"
#include "math\xfx_math.h"
#include "math\xfx_vec2.h"
#include "math\xfx_vec3.h"
#include "math\xfx_vec4.h"
#include "math\xfx_euler.h"
#include "math\xfx_primitives.h"
#include "math\xfx_math.h"
#include "math\xfx_transformable.h"
#include "main\xfx_fs.h"
#include "main\xfx_application.h"
#include "main\xfx_cmd.h"

#ifdef _DEBUG_NEW
#pragma push_macro("new")
#undef new
#endif

// D3D
#if( __XFX_DIRECTX_VER__ < 9 )
#include <d3d8.h>
#include <d3dx8.h>
#else
#include <d3d9.h>
#include <d3dx9.h>
#endif

#ifdef _DEBUG_NEW
#pragma pop_macro("new")
#endif

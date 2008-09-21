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
//#include <numeric>
//#include <algorithm>
//#include <vector>
//#include <deque>
//#include <memory>
#include <string>
//#include <stack>
//#include <iterator>
//#include <functional>
//#include <bitset>
//#include <set>
//#include <iostream>
//#include <fstream>
#include <hash_map>

// Boost
#include <boost/array.hpp>
#include <boost/scoped_ptr.hpp>
//#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>
//#include <boost/any.hpp>
//#include <boost/cast.hpp>
//#include <boost/static_assert.hpp>
//#include <boost/bind.hpp>
//#include <boost/mem_fn.hpp>
//#include <boost/shared_container_iterator.hpp>
//#include <boost/function_output_iterator.hpp>
//#include <boost/iterator/filter_iterator.hpp>
//#include <boost/iterator/permutation_iterator.hpp>
//#include <boost/iterator/indirect_iterator.hpp>
//#include <boost/utility.hpp>
//#include <boost/pool/pool_alloc.hpp>
#include <boost/functional/hash.hpp>
#include <boost/function.hpp>
//#include <boost/signals.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/tuple/tuple.hpp>
#include <boost/format.hpp>
//#include <boost/thread.hpp>
//#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>


// Boost.MPL
//#include <boost/mpl/vector.hpp>
//#include <boost/mpl/vector_c.hpp>
//#include <boost/mpl/min_max.hpp>


// LUA
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}


// LuaBind
#include <luabind/luabind.hpp>
#include <luabind/discard_result_policy.hpp>
#include <luabind/out_value_policy.hpp>
#include <luabind/raw_policy.hpp>



// forward definition to use serialization
namespace boost { namespace serialization {
	class access;
} };



// XFX
#include "utility\xfx_singleton.h"
#include "utility\xfx_string.h"
#include "utility\xfx_profiler.h"
#include "utility\xfx_log.h"
#include "utility\xfx_script.h"
#include "utility\xfx_utils.h"
#include "main\xfx_errorcodes.h"
#include "main\xfx_fs.h"

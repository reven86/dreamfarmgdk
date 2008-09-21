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
//#include <limits>

// Boost
#include <boost/array.hpp>
//#include <boost/scoped_ptr.hpp>
//#include <boost/weak_ptr.hpp>
//#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
//#include <boost/noncopyable.hpp>
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
#include <boost/signals.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/tuple/tuple.hpp>
#include <boost/format.hpp>
//#include <boost/thread/recursive_mutex.hpp>
//#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
//#include <boost/type_traits.hpp>


// Boost.MPL
//#include <boost/mpl/vector.hpp>
//#include <boost/mpl/vector_c.hpp>
//#include <boost/mpl/min_max.hpp>
#include <boost/mpl/and.hpp>


// Boost.Serialization
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/hash_map.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/export.hpp>



// XFX
/*
#include "utility\xfx_string.h"
#include "utility\xfx_profiler.h"
#include "utility\xfx_script.h"
#include "utility\xfx_utils.h"
#include "utility\xfx_singleton.h"
#include "utility\xfx_log.h"
#include "utility\xfx_argb.h"
#include "utility\xfx_envelope.h"
#include "utility\xfx_cache.h"
#include "main\xfx_fs.h"
#include "math\xfx_euler.h"
#include "math\xfx_mat3.h"
#include "math\xfx_mat4.h"
#include "math\xfx_primitives.h"
#include "math\xfx_transformable.h"
#include "renderer_ext\xfx_particlesystem.h"
*/
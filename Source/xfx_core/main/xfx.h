//  File xfx.h                                                                |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx.h 
 *	\brief Base configuration file for library.
 */



#pragma warning (disable:4786)	//identifier was truncated to '255' characters...
#pragma warning (disable:4800)	//identifier was truncated to '255' characters...
#pragma warning (disable:4996)	//stdext::hash_map
#pragma warning (disable:4267)	//convert warning
#pragma warning (disable:4512)	//cannot generate assignment operator


#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined (MAKEFOURCC) */

//Namespace
#define _XFX_BEGIN namespace xfx {
#define _XFX_END }



//
// Additional libraries
//

#include <crtdbg.h>		//search for memory leaks

// OpenMP
#include <omp.h>

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
#include <boost/function.hpp>
//#include <boost/signals.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/tuple/tuple.hpp>
#include <boost/format.hpp>
//#include <boost/thread.hpp>
//#include <boost/foreach.hpp>


// Boost.MPL
//#include <boost/mpl/vector.hpp>
//#include <boost/mpl/vector_c.hpp>
//#include <boost/mpl/min_max.hpp>

// Other XFX libraries
#include "xfx_errorcodes.h"
#include "utility/xfx_string.h"
#include "utility/xfx_singleton.h"
#include "utility/xfx_log.h"
#include "utility/xfx_utils.h"
#include "utility/xfx_profiler.h"
#include "utility/xfx_script.h"
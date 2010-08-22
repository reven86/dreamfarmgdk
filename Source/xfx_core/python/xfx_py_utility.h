//  File xfx_py_utility.h                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_py_utility.h 
 *	\brief File contains Python bindings (ready to process by Py++) to Utility module.
 */



// Check whether or not we are parsed by GCCXML library
#ifdef __GCCXML__

#define _HAS_TR1 0
#define BOOST_RANGE_ENABLE_CONCEPT_ASSERT 0
#define BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES 1
#define __XFX_SHIPPING__ 1
#define __XFX_DISABLE_PROFILER__ 1

#include "../shared.h"
#include "../main/xfx_errorcodes.h"
#include <string>
#include <boost/cstdint.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/scoped_array.hpp>
#include <boost/unordered_map.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#endif




//#include "../utility/xfx_utils.h"
//#include "../utility/xfx_string.h"
#include "../utility/xfx_profiler.h"
//#include "../utility/xfx_script.h"
//#include "../utility/xfx_argb.h"
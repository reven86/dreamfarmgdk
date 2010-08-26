//  File xfx_py_main.h                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_py_utility.h 
 *	\brief File contains Python bindings (ready to process by Py++) to Main module.
 */



// Check whether or not we are parsed by GCCXML library
#ifdef __GCCXML__

#include "../shared.h"


#endif


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

// forward definition to use serialization
namespace boost { namespace serialization {
	class access;
} };


#include "../utility/xfx_string.h"
#include "../utility/xfx_singleton.h"
#include "../utility/xfx_log.h"
#include "../utility/xfx_utils.h"
#include "../utility/xfx_profiler.h"
#include "../utility/xfx_script.h"
#include "../utility/xfx_cache.h"


#include "../main/xfx_errorcodes.h"
#include "../main/xfx_timer.h"
#include "../main/xfx_fs.h"
#include "../main/xfx_pack.h"
#include "../main/xfx_messagesbook.h"
#include "../main/xfx_cmd.h"
#include "../main/xfx_console.h"
#include "../main/xfx_input.h"
#include "../main/xfx_main_cvars.h"
#include "../main/xfx_engine.h"
#include "../main/xfx_application.h"

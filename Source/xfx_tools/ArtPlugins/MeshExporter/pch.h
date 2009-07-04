// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// TODO: reference additional headers your program requires here


#include <crtdbg.h>		//search for memory leaks

//3DSMAX
#pragma warning( push )
#pragma warning( disable: 4005 )
#pragma warning( disable: 4100 )
#pragma warning( disable: 4201 )
#pragma warning( disable: 4238 )
#pragma warning( disable: 4239 )
#pragma warning( disable: 4245 )
#pragma warning( disable: 4512 )

#include "Max.h"

#pragma warning( pop )


#undef min
#undef max

#include "utility/xfx_utility_include.h"
#include "math/xfx_math_include.h"

#pragma comment (lib, "core.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "mesh.lib")
#pragma comment (lib, "geom.lib")
#pragma comment (lib, "maxutil.lib")

inline xfx::Vec3 ToVector (const Point3& b)
{
	xfx::Vec3 r;

	r.x = b.x;
	r.y = b.y;
	r.z = b.z;

	return r;
}
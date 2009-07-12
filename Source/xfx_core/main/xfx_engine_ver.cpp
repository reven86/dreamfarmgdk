#include "xfx.h"
#include "xfx_engine.h"

_XFX_BEGIN

const int Engine::msVersionMajor = 2;
const int Engine::msVersionMinor = 0;
const int Engine::msVersionBuild = 151;
const String Engine::msVersionBuildDate( "2009/07/12 15:56:28" );
const String Engine::msVersion( boost::str( xfx::StringFormat( "%d.%d, Build %d - Date %s%s." ) % Engine::msVersionMajor % Engine::msVersionMinor % Engine::msVersionBuild % Engine::msVersionBuildDate % " (local modifications)" ) );


_XFX_END
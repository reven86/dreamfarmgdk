//  File xfx_luascript.h                                                      |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_luascript.h 
 *	\brief Lua script binding by luabind library.
 */

_XFX_BEGIN







/*!	\defgroup LuaGroup Lua classes.
 */

/*! \class LuaScript xfx_luascript.h "lua/xfx_luascript.h"
 *	\brief Lua singleton wrapper.
 *	\ingroup LuaGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class LuaScript : public Singleton< LuaScript >
{
	friend class Singleton< LuaScript >;

	struct lua_State			* mpLuaVM;

private:
	LuaScript											( ) : mpLuaVM( NULL ) { };
	virtual ~LuaScript									( ) { };

public:
	//! Open lua state.
	HRESULT						Open					( );

	//! Close lua state.
	HRESULT						Close					( );

	//! Get lua virtual machine.
	lua_State *					GetLuaVM				( ) { return mpLuaVM; };

	/*! \brief Is function exists?
	 *
	 *	\param[in]		name		Function name.
	 *
	 *	\return \b S_OK if function found, XFXERR_NOTFOUND otherwise.
	 */
	HRESULT						IsFunction				( const char * name ) const;
};




_XFX_END

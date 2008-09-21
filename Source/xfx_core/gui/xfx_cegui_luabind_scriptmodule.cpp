//  File xfx_cegui_luabind_scriptmodule.cpp                                   |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_cegui_luabind_scriptmodule.h"

_XFX_BEGIN






//
// CEGUILuaBindScriptModule
//

CEGUILuaBindScriptModule::CEGUILuaBindScriptModule( )
{
	mIdentifierString = "CEGUI LuaBind scripting module (Dream Farm GDK implementation)";
	mLanguageString = "Lua 5.0.2";
}

CEGUILuaBindScriptModule::~CEGUILuaBindScriptModule( )
{
}

void CEGUILuaBindScriptModule::executeScriptFile(const CEGUI::String &filename, const CEGUI::String &resourceGroup)
{
	// load file
	CEGUI::RawDataContainer raw;
	CEGUI::System::getSingleton().getResourceProvider()->loadRawDataContainer(filename,
        raw, resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);

	lua_State * lua_vm = LuaScript::Instance( ).GetLuaVM( );

	// load code into lua
	int top = lua_gettop( lua_vm );
	int loaderr = luaL_loadbuffer( lua_vm, (char*)raw.getDataPtr(), raw.getSize(), filename.c_str());
	CEGUI::System::getSingleton().getResourceProvider()->unloadRawDataContainer( raw );
	if (loaderr)
	{
		CEGUI::String errMsg = lua_tostring(lua_vm,-1);
		lua_settop(lua_vm,top);
		throw CEGUI::ScriptException("Unable to execute Lua script file: '"+filename+"'\n\n"+errMsg+"\n");
	}

    // call it
	if (lua_pcall(lua_vm,0,0,0))
	{
	    CEGUI::String errMsg = lua_tostring(lua_vm,-1);
		lua_settop(lua_vm,top);
		throw CEGUI::ScriptException("Unable to execute Lua script file: '"+filename+"'\n\n"+errMsg+"\n");
	}

	lua_settop(lua_vm,top); // just in case :P
}

int CEGUILuaBindScriptModule::executeScriptGlobal(const CEGUI::String &function_name)
{
	lua_State * d_state = LuaScript::Instance( ).GetLuaVM( );

	int top = lua_gettop(d_state);

    // get the function from lua
    lua_getglobal(d_state, function_name.c_str());

    // is it a function
    if (!lua_isfunction(d_state,-1))
    {
        lua_settop(d_state,top);
		throw CEGUI::ScriptException("Unable to get Lua global: '"+function_name+"' as name not represent a global Lua function" );
    }

    // call it
    int error = lua_pcall(d_state,0,1,0);

    // handle errors
    if (error)
    {
		CEGUI::String errMsg = lua_tostring(d_state,-1);
        lua_pop(d_state,1);
		throw CEGUI::ScriptException("Unable to evaluate Lua global: '"+function_name+"\n\n"+errMsg+"\n");
    }

    // get return value
    if (!lua_isnumber(d_state,-1))
    {
        // log that return value is invalid. return -1 and move on.
        lua_settop(d_state,top);
		CEGUI::ScriptException("Unable to get Lua global : '"+function_name+"' return value as it's not a number" );
        return -1;
    }

    int ret = (int)lua_tonumber(d_state,-1);
    lua_pop(d_state,1);

    // return it
    return ret;
}

bool CEGUILuaBindScriptModule::executeScriptedEventHandler( const CEGUI::String& handler_name, const CEGUI::EventArgs& e )
{
	/*
	lua_State * d_state = LuaScript::Instance( ).GetLuaVM( );

	LuaFunctor::pushNamedFunction(d_state, handler_name);

	ScriptWindowHelper* helper = 0;
	//If this is an event that was triggered by a window then make a "this" pointer to the window for the script.
	if(e.d_hasWindow)
	{
		WindowEventArgs& we = (WindowEventArgs&)e;
		helper = new ScriptWindowHelper(we.window);
		lua_pushlightuserdata(d_state,(void*)helper);
		lua_setglobal(d_state,"this");
	} // if(e.d_hasWindow)

    // push EventArgs as the first parameter
    tolua_pushusertype(d_state,(void*)&e,"const CEGUI::EventArgs");

    // call it
    int error = lua_pcall(d_state,1,0,0);

    // handle errors
    if (error)
    {
        String errStr(lua_tostring(d_state,-1));
        lua_pop(d_state,1);
		//cleanup the helper object if any
		if(helper)
		{
			delete helper;
			helper = 0;
		}
        throw ScriptException("Unable to evaluate the Lua event handler: '"+handler_name+"'\n\n"+errStr+"\n");
    } // if (error)

	if(helper)
	{
		delete helper;
		helper = 0;
	}
	*/

	_ASSERTE( !"Not implemented yet." );

	return false;
}

void CEGUILuaBindScriptModule::executeString(const CEGUI::String &str)
{
	lua_State * d_state = LuaScript::Instance( ).GetLuaVM( );

	int top = lua_gettop(d_state);

    // load code into lua and call it
    int error =	luaL_loadbuffer(d_state, str.c_str(), str.length(), str.c_str()) || lua_pcall(d_state,0,0,0);

    // handle errors
    if (error)
    {
		CEGUI::String errMsg = lua_tostring(d_state,-1);
        lua_settop(d_state,top);
		throw CEGUI::ScriptException("Unable to execute Lua script string: '"+str+"'\n\n"+errMsg+"\n");
    }
}

void CEGUILuaBindScriptModule::createBindings( )
{
	CEGUI::Logger::getSingleton().logEvent( "---- Creating Lua bindings ----" );

	// register xfx classes
	luabind::module( LuaScript::Instance( ).GetLuaVM( ) )
	[
		luabind::namespace_( "CEGUI" )
		[
			luabind::class_< CEGUI::Vector2 >( "Vector2" )
				.def( luabind::constructor< >( ) )
				.def( luabind::constructor< float, float >( ) )
				.def_readwrite( "x", &CEGUI::Vector2::d_x )
				.def_readwrite( "y", &CEGUI::Vector2::d_y )
		]
	];
}

void CEGUILuaBindScriptModule::destroyBindings()
{
	CEGUI::Logger::getSingleton().logEvent( "---- Destroying Lua bindings ----" );

	lua_State * d_state = LuaScript::Instance( ).GetLuaVM( );

	// is this ok ?
	lua_pushnil(d_state);
	lua_setglobal(d_state,"CEGUI");
}

const CEGUI::String& CEGUILuaBindScriptModule::getIdentifierString() const
{
	return mIdentifierString;
}

const CEGUI::String& CEGUILuaBindScriptModule::getLanguage() const
{
	return mLanguageString;
}

CEGUI::Event::Connection CEGUILuaBindScriptModule::subscribeEvent( CEGUI::EventSet * target, const CEGUI::String& name, const CEGUI::String& subscriber_name )
{
	_ASSERTE( !"Not implemented yet." );

	/*
    // do the real subscription
    LuaFunctor functor(d_state,subscriber_name,LUA_NOREF);
	CEGUI::Event::Connection con = target->subscribeEvent(event_name, Event::Subscriber(functor));
    // make sure we don't release the reference we just made when this call returns
    functor.index = LUA_NOREF;
	*/

    // return the event connection
	return CEGUI::Event::Connection( );
}

CEGUI::Event::Connection CEGUILuaBindScriptModule::subscribeEvent( CEGUI::EventSet * target, const CEGUI::String& name, CEGUI::Event::Group event_group, const CEGUI::String& subscriber_name )
{
	_ASSERTE( !"Not implemented yet." );
	/*
    // do the real subscription
    LuaFunctor functor(d_state,subscriber_name,LUA_NOREF);
    Event::Connection con = target->subscribeEvent(event_name, group, Event::Subscriber(functor));

    // return the event connection
    return con;
	*/

	return CEGUI::Event::Connection( );
}




_XFX_END
//  File xfx_luascript.cpp                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_luascript.h"
#include "xfx_lua_sm.h"
#include "math\xfx_vec2.h"
#include "math\xfx_vec3.h"
#include "math\xfx_vec4.h"
#include "math\xfx_euler.h"
#include "main\xfx_messagesbook.h"
#include "utility\xfx_argb.h"
#include <sstream>

_XFX_BEGIN






//
// Error callback
//

int pcall_error_callback( lua_State * L )
{
	lua_Debug d;
	lua_getstack( L, 1	, &d );
	lua_getinfo( L, "Sln", &d );
	std::string err = lua_tostring( L, -1 );
	lua_pop( L, 1 );
	std::stringstream msg;
	msg << d.short_src << ":" << d.currentline;

	if( d.name != 0 )
	{
	  msg << "(" << d.namewhat << " " << d.name << ")";
	}

	msg << " " << err;
	lua_pushstring( L, msg.str( ).c_str( ) );

	gError( "%s", msg.str( ).c_str( ) );

	return 1;
}



//
// 'dofile function that support pack files
//

void lua_dofile_with_packs( const char * filename )
{
	//const char * filename = luaL_optstring( L, 1, NULL );

	gMess( "Executing file \"%s\"...", filename );

	unsigned long filesize;
	HRESULT hr;
	if( FAILED( hr = FileSystem::Instance( ).GetFileSize( filename, filesize ) ) )
	{
		gError( "Can't open file \"%s\"", filename );
		return;
	}

	boost::scoped_array< char > pbuf( new char[ filesize ] );

	FileSystem::Instance( ).ReadFile( filename, pbuf.get( ) );

	int res = lua_dobuffer( LuaScript::Instance( ).GetLuaVM( ), pbuf.get( ), filesize, filename );
	if( res != 0 )
		gError( "Executing file \"%s\" failed!", filename );
}





//
// LuaScript
//

HRESULT LuaScript::Open( )
{
	Close( );

	mpLuaVM = lua_open( );

	//open standard libraries
	lua_baselibopen		( mpLuaVM );
	lua_iolibopen		( mpLuaVM );
	lua_strlibopen		( mpLuaVM );
	lua_mathlibopen		( mpLuaVM );
	lua_tablibopen		( mpLuaVM );

#ifdef _DEBUG
	lua_dblibopen		( mpLuaVM );
#endif

	luabind::open				( mpLuaVM );
	luabind::set_pcall_callback	( &pcall_error_callback );

	LuaSMManager::LuaRegister( mpLuaVM );

	// register xfx classes
	luabind::module( mpLuaVM )
	[
		luabind::def( "dofile", &lua_dofile_with_packs ),
		luabind::class_< Vec2 >( "Vec2" )
			.def( luabind::constructor< float >( ) )
			.def( luabind::constructor< float, float >( ) )
			.def_readwrite( "x", &Vec2::x )
			.def_readwrite( "y", &Vec2::y ),
		luabind::class_< Vec3 >( "Vec3" )
			.def( luabind::constructor< float >( ) )
			.def( luabind::constructor< float, float, float >( ) )
			.def_readwrite( "x", &Vec3::x )
			.def_readwrite( "y", &Vec3::y )
			.def_readwrite( "z", &Vec3::z ),
		luabind::class_< Vec4 >( "Vec4" )
			.def( luabind::constructor< float >( ) )
			.def( luabind::constructor< float, float, float, float >( ) )
			.def_readwrite( "x", &Vec4::x )
			.def_readwrite( "y", &Vec4::y )
			.def_readwrite( "z", &Vec4::z )
			.def_readwrite( "w", &Vec4::w ),
		luabind::class_< Euler >( "Euler" )
			.def( luabind::constructor< Math::BigAngle >( ) )
			.def( luabind::constructor< Math::BigAngle, Math::BigAngle, Math::BigAngle >( ) )
			.def_readwrite( "yaw", &Euler::yaw )
			.def_readwrite( "pitch", &Euler::pitch )
			.def_readwrite( "roll", &Euler::roll ),
		luabind::class_< ARGB >( "ARGB" )
			.def( luabind::constructor< DWORD >( ) )
			.def( luabind::constructor< UCHAR, UCHAR, UCHAR, UCHAR >( ) )
			.def_readwrite( "a", &ARGB::a )
			.def_readwrite( "r", &ARGB::r )
			.def_readwrite( "g", &ARGB::g )
			.def_readwrite( "b", &ARGB::b ),
		luabind::class_< String >( "String" ),
		luabind::class_< WString >( "WString" ),
		luabind::class_< MessagesBook >( "MessagesBook" )
			//.def( constructor< >( )
			.scope
			[
				luabind::class_< MessagesBook::Entry >( "MessagesBook::Entry" )
					.def_readonly( "text", &MessagesBook::Entry::text )
			],
		luabind::class_< BooksManager >( "BooksManager" )
			.def( "get_entry", ( const MessagesBook::Entry& ( BooksManager::* ) ( const char * ) const ) &BooksManager::GetEntry )
	];

	luabind::object glob = luabind::globals( mpLuaVM );
	glob[ "gBookManager" ] = &BooksManager::Instance( );

	return S_OK;
}

HRESULT LuaScript::Close( )
{
	LuaSMManager::Instance( ).Free( );

	if( mpLuaVM )
	{
		lua_close( mpLuaVM );
		mpLuaVM = NULL;
	}

	return S_OK;
}

HRESULT LuaScript::IsFunction( const char * name ) const
{
	_ASSERTE( mpLuaVM && name );

	lua_pushstring( mpLuaVM, name );
	lua_gettable( mpLuaVM, LUA_GLOBALSINDEX );

	if( !lua_isfunction( mpLuaVM, -1 ) )
	{
		lua_pop( mpLuaVM, 1 );
		return XFXERR_NOTFOUND;
	}

	return S_OK;
}






_XFX_END

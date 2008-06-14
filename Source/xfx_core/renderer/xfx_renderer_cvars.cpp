//  File xfx_renderer_cvars.cpp                                               |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_renderer_cvars.h"





//
// CVars
//

#define DEFINE_VAR( v ) \
	xfx::Var * v( NULL )

DEFINE_VAR( g_texture_cache_folder );
DEFINE_VAR( g_texture_cache_enable );

DEFINE_VAR( r_device_type );
DEFINE_VAR( r_device_depth_stencil );
DEFINE_VAR( r_device_vp );
DEFINE_VAR( r_background );
DEFINE_VAR( r_wireframe );
DEFINE_VAR( r_drawbboxes );
DEFINE_VAR( r_maxsprites );
DEFINE_VAR( r_maxtris );
DEFINE_VAR( r_maxlines );
DEFINE_VAR( r_debug_frustumculling );
DEFINE_VAR( r_debug_aabbtree );
DEFINE_VAR( r_debug_animationframe );
DEFINE_VAR( r_debug_particles );
DEFINE_VAR( r_debug_perfhud );

#undef DEFINE_VAR







//
// Register cvars
//

_XFX_BEGIN







//
// Commands
//






//
// Make a simple class that registers all cvars and commands
// and then create new static object of that class
//

class Renderer_CVarsRegistrant
{
public:
	Renderer_CVarsRegistrant				( )
	{
#define REGISTER_VAR( var_name, def_value, flags ) \
		var_name = Cmd::Instance( ).Register( #var_name, String( def_value ), flags );

		//
		// Vars
		//

		REGISTER_VAR	( g_texture_cache_folder,		"TextureCache\\",		Cmd::EVF_DEFAULT );
		REGISTER_VAR	( g_texture_cache_enable,		"0",					Cmd::EVF_DEFAULT );

		REGISTER_VAR	( r_device_type,				"",						Cmd::EVF_DEFAULT );
		REGISTER_VAR	( r_device_depth_stencil,		"",						Cmd::EVF_DEFAULT );
		REGISTER_VAR	( r_device_vp,					"",						Cmd::EVF_DEFAULT );
		REGISTER_VAR	( r_background,					"0",					Cmd::EVF_DEFAULT );
		REGISTER_VAR	( r_wireframe,					"0",					Cmd::EVF_CHEAT );
		REGISTER_VAR	( r_drawbboxes,					"0",					Cmd::EVF_DEFAULT );
		REGISTER_VAR	( r_maxsprites,					"1024",					Cmd::EVF_DEFAULT );
		REGISTER_VAR	( r_maxtris,					"1024",					Cmd::EVF_DEFAULT );
		REGISTER_VAR	( r_maxlines,					"1024",					Cmd::EVF_DEFAULT );
		REGISTER_VAR	( r_debug_frustumculling,		"0",					Cmd::EVF_DEVELOPER );
		REGISTER_VAR	( r_debug_aabbtree,				"-1",					Cmd::EVF_DEVELOPER );
		REGISTER_VAR	( r_debug_animationframe,		"-1",					Cmd::EVF_DEVELOPER );
		REGISTER_VAR	( r_debug_particles,			"0",					Cmd::EVF_DEVELOPER );
		REGISTER_VAR	( r_debug_perfhud,				"0",					Cmd::EVF_DEVELOPER );//| Cmd::EVF_CONST );

#undef REGISTER_VAR
	};
};

static Renderer_CVarsRegistrant _renderer_cvars_reg;

_XFX_END
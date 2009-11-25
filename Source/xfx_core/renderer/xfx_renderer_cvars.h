//  File xfx_renderer_cvars.h                                                 |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_renderer_cvars.h
 *	\brief Defines a global pre-registered variables for Renderer library. \see Cmd.
 */








_XFX_BEGIN
class Var;
_XFX_END


#define DECLARE_VAR(x) \
	extern xfx::Var * x

/*! \addtogroup CommonVariables.
 */

//! @{

//! Set texture cache folder (with slash at the end).
DECLARE_VAR( g_texture_cache_folder );
//! Enable texture cache save/load.
DECLARE_VAR( g_texture_cache_enable );

//! Preffered device type.
DECLARE_VAR( r_device_type );
//! Preffered device depth/stencil buffer.
DECLARE_VAR( r_device_depth_stencil );
//! Preffered device vertex processing mode.
DECLARE_VAR( r_device_vp );
//! Set background color.
DECLARE_VAR( r_background );
//! Set wireframe mode.
DECLARE_VAR( r_wireframe );
//! Draw bound-boxes.
DECLARE_VAR( r_drawbboxes );
//! Maximum sprites count (default 1024).
DECLARE_VAR( r_maxsprites );
//! Maximum user tris count (default 1024).
DECLARE_VAR( r_maxtris );
//! Maximum lines count (default 1024).
DECLARE_VAR( r_maxlines );
//! Debug frustum culling.
DECLARE_VAR( r_debug_frustumculling );
//! Debug AABBTree.
DECLARE_VAR( r_debug_aabbtree );
//! Set animation frame to debug.
DECLARE_VAR( r_debug_animationframe );
//! Debug particles.
DECLARE_VAR( r_debug_particles );
//! Enable NVPerfHUD.
DECLARE_VAR( r_debug_perfhud );

//! @}

#undef DECLARE_VAR

//  File xfx_main_cvars.h                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_main_cvars.h
 *	\brief Defines a global pre-registered variables for Main library. \see Cmd.
 */








_XFX_BEGIN
class Var;
_XFX_END

#define DECLARE_VAR(x) \
	extern xfx::Var * x

/*! \defgroup CommonVariables Common cvars.
 */

//! @{

//! Set developer mode.
DECLARE_VAR( g_developer );
//! Allow cheating.
DECLARE_VAR( g_cheats );
//! Set log file.
DECLARE_VAR( g_logfile );
//! Set debug mode for command processor.
DECLARE_VAR( g_debug_cmd );
//! Temp variable, used only in debugging to switch between several algorithms.
DECLARE_VAR( g_debug_temp_var );
//! Log level ( 0 - critical, 3 - default )
DECLARE_VAR( g_log_level );
//! Set profile frame rate or -1 to automatically adjusted frame rate
DECLARE_VAR( g_profiler_frame_rate );

//! @}

#undef DECLARE_VAR

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

/*! \class Var xfx_main_cvars.h "engine/xfx_main_cvars.h"
 *	\brief Variable class.
 *	\ingroup EngineGroup
 *	
 *	%Variable have value as String, default value as String and holds float and int representation.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Var
{
	String					mValue;
	String					mDefault;
	float					mAsFloat;
	int						mAsInt;

public:
	Var						( const String& def = "" ) : mDefault( def) { Change( mDefault ); };
	~Var					( ) { };

	const String& Default	( ) const { return mDefault; };
	const String& Value		( ) const { return mValue; };
	const float& AsFloat	( ) const { return mAsFloat; };
	const int& AsInt		( ) const { return mAsInt; };

	void Change				( const String& val ) { mValue = val; mAsInt = GetInt( val ); mAsFloat = GetFloat( val ); };
	void Reset				( ) { Change( mDefault ); };

protected:
	float GetFloat			( const String& val ) const { return static_cast< float >( atof( val.c_str( ) ) ); };
	int GetInt				( const String& val ) const { char * ch; return static_cast< int >( strtoul( val.c_str( ), &ch, 0 ) ); };
};

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

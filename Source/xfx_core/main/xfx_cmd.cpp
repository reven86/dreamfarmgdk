//  File xfx_cmd.cpp                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_cmd.h"
#include "xfx_main_cvars.h"

_XFX_BEGIN





//
// Cmd
//

Cmd::Cmd( ) :
	mExecContext( EEC_CODE )
{
}

boost::shared_ptr< Var > Cmd::RegisterVar( const String& var, const String& def, const DWORD& flags )
{
	String var_copy( boost::to_lower_copy( var ) );

	boost::shared_ptr< Var > res ( new Var( def ) );

	VariablesType::iterator it = mVariables.find( var_copy );
	if( it != mVariables.end( ) )
		( *it ).second = std::make_pair( res, flags );
	else
		mVariables.insert( VariablesType::value_type( var_copy, std::make_pair( res, flags ) ) );

	return res;
}

void Cmd::RegisterCmd( const String& cmd, const boost::function1< void, String >& fn, const DWORD& flags )
{
	String cmd_copy( boost::to_lower_copy( cmd ) );

	CommandsType::iterator it = mCommands.find( cmd_copy );
	if( it != mCommands.end( ) )
		( *it ).second.first = fn;
	else
		mCommands.insert( CommandsType::value_type( cmd_copy, std::make_pair( fn, flags ) ) );
}

DWORD Cmd::GetVarFlags( const String& var ) const
{
	String var_copy( boost::to_lower_copy( var ) );
	_ASSERTE( mVariables.find( var_copy ) != mVariables.end( ) );

	VariablesType::const_iterator it = mVariables.find( var_copy );
	if( it == mVariables.end( ) )
	{
		gError( "Variable %s not exists", var_copy.c_str( ) );
		return 0;
	}

	return ( *it ).second.second;
}

DWORD Cmd::GetCmdFlags( const String& cmd ) const
{
	String cmd_copy( boost::to_lower_copy( cmd ) );
	_ASSERTE( mCommands.find( cmd_copy ) != mCommands.end( ) );

	CommandsType::const_iterator it = mCommands.find( cmd_copy );
	if( it == mCommands.end( ) )
	{
		gError( "Command %s not exits", cmd_copy.c_str( ) );
		return 0;
	}

	return ( *it ).second.second;
}

void Cmd::Execute( const String& name )
{
	if( name.empty( ) )
		return;

	String::size_type s = 0;

	String cur_cmd;
	while( s < name.size( ) )
	{
		skip_comments( name, s );
		String tok = next_token( name, s );

		if( tok.empty( ) )
			break;

		if( tok[ 0 ] == ';' )
		{
			ExecOneCommand( cur_cmd );

			String::size_type s2 = tok.find_first_not_of( ';' );
			if( s2 != String::npos )
			{
				cur_cmd = '\"';
				cur_cmd += tok.substr( s2 ) + "\" ";
			}
			else
				cur_cmd.clear( );
		}
		else
		{
			String::size_type s2 = 0;
			String new_tok = next_token( tok, s2, ";" );

			if( !tok.empty( ) && tok[ 0 ] == '\"' && tok[ tok.size( ) - 1 ] == '\"' && new_tok.size( ) == tok.size( ) - 2 )
			{
				tok = String( "\\\"" ) + new_tok + "\\\"";
			}
			else if( new_tok.size( ) != tok.size( ) )
			{
				s = s - tok.size( ) + new_tok.size( ) - 1;
				tok = new_tok;
			}

			cur_cmd += '\"';
			cur_cmd += tok + "\" ";
		}
	}

	ExecOneCommand( cur_cmd );
}

void Cmd::ExecOneCommand( const String& name )
{
	String newname( name );

	String::size_type cmdstart = 0;
	String cmd = next_token( newname, cmdstart );

	if( cmd.empty( ) )
		return;

	boost::to_lower( cmd );

	CommandsType::const_iterator cmd_it = mCommands.find( cmd );

	if( cmd_it != mCommands.end( ) )
	{
		String params( ( cmdstart >= newname.size( ) ) ? String( "" ) : newname.substr( cmdstart ) );

		// test command flags
		if( ( ( *cmd_it ).second.second & ECF_DEVELOPER ) && !g_developer->AsInt( ) )
		{
			gToConsole( "Command %s is for developers only.", cmd.c_str( ) );
			return;
		}

		if( ( ( *cmd_it ).second.second & ECF_CHEAT) && !g_cheats->AsInt( ) && !g_developer->AsInt( ) )
		{
			gToConsole( "Command %s is a cheat command. Enable cheats to use this command.", cmd.c_str( ) );
			return;
		}

#pragma message ("FIXME: redirect command to server")
		if( ( *cmd_it ).second.second & ECF_SERVER_CMD )
			return;

		if( g_debug_cmd->AsInt( ) )
			gMess( "Executing %s...", name.c_str( ) );
		(*cmd_it).second.first( boost::cref( params ) );

		return;
	}

	const String& alias = FindAlias( cmd );
	if( !alias.empty( ) )
	{
		Execute( alias );
		return;
	}

	VariablesType::const_iterator var_it = mVariables.find( cmd );
	if( var_it != mVariables.end( ) )
	{
		String w2 = next_token( newname, cmdstart );

		if( w2.empty( ) )
		{
			gToConsole( "%s is \"%s\" (default: \"%s\")", cmd.c_str( ), ( *var_it ).second.first->Value( ).c_str( ), ( *var_it ).second.first->Default( ).c_str( ) );
		}
		else
		{
			// test variable flags
			if( ( ( *var_it ).second.second & EVF_DEVELOPER ) && !g_developer->AsInt( ) )
			{
				gToConsole( "Variable %s is for developers only.", cmd.c_str( ) );
				return;
			}

			if( ( ( *var_it ).second.second & EVF_CHEAT ) && !g_cheats->AsInt( ) && !g_developer->AsInt( ) )
			{
				gToConsole( "Variable %s is a cheat variable. Enable cheats to use this variable.", cmd.c_str( ) );
				return;
			}

#pragma message( "FIXME: redirect variable to server" )
			if( ( *var_it ).second.second & EVF_SERVER_VAR )
				return;

			if( ( ( *var_it ).second.second & EVF_CONST ) != 0 && mExecContext == EEC_USER )
			{
				gToConsole( "Variable %s can't be modified.", cmd.c_str( ) );
				return;
			}

			gMess( "%s changed to \"%s\"", cmd.c_str( ), w2.c_str( ) );
			( *var_it ).second.first->Change( w2 );
		}
		return;
	}

	gMess( "Unknown command: %s", cmd.c_str( ) );
}

const String& Cmd::FindAlias( const String& cmd ) const
{
	AliasesType::const_iterator it = mAliases.find( cmd );

	if( it == mAliases.end( ) )
	{
		static String res( "" );
		return res;
	}

	return( *it ).second;
}

void Cmd::ResetVariables( bool reset_all )
{
	BOOST_FOREACH( VariablesType::value_type& v, mVariables )
	{
		if( reset_all || ( v.second.second & EVF_AUTORESET ) != 0 )
			v.second.first->Reset( );		
	}
}




_XFX_END
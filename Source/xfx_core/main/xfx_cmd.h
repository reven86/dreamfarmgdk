//  File xfx_cmd.h                                                            |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_cmd.h
 *	\brief Defines a command process class.
 */

_XFX_BEGIN






/*! \class Cmd xfx_cmd.h "engine/xfx_cmd.h"
 *	\brief Command processor class.
 *	\ingroup EngineGroup
 *	
 *	%Command processor stores available variables and commands.
 *
 *	\todo Implement network interaction with command processor.
 *	\author Andrew "RevEn" Karpushin
 */

class Cmd : public Singleton< Cmd >
{
	friend class Singleton< Cmd >;

	typedef stdext::hash_map< String, std::pair< boost::function1< void, String >, DWORD >, HashCompare< String > >	CommandsType;
	typedef stdext::hash_map< String, std::pair< boost::shared_ptr< class Var >, DWORD >, HashCompare< String > >	VariablesType;
	typedef stdext::hash_map< String, String, HashCompare< String> >												AliasesType;

public:

	//! Command flags.
	enum ECommandFlags
	{
		ECF_DEFAULT				= 0x00000000,		//!< Default flag for usual command
		ECF_HIDDEN				= 0x00000001,		//!< Command is hidden in console, but can be executed
		ECF_DEVELOPER			= 0x00000002,		//!< Command can be executed only if g_developer is not 0
		ECF_CHEAT				= 0x00000004,		//!< Command can be executed if g_developer or g_cheats are not 0
		ECF_SERVER_CMD			= 0x00000008,		//!< Command can be executed only on server, i.e. server command
	};

	//! Variable flags.
	enum EVariableFlags
	{
		EVF_DEFAULT				= 0x00000000,		//!< Default flag for usual variable
		EVF_HIDDEN				= 0x00000001,		//!< Variable hidden in console, but can be executed
		EVF_DEVELOPER			= 0x00000002,		//!< Variable can be executed only if g_developer is not 0
		EVF_CHEAT				= 0x00000004,		//!< Variable can be executed if g_developer or g_cheats are not 0
		EVF_AUTORESET			= 0x00000008,		//!< Variable resets own state when ResetVariables is called
		EVF_SERVER_VAR			= 0x00000010,		//!< Variable state send from server, i.e. server variable
		EVF_CONST				= 0x00000020,		//!< Variable can't be modified in runtime
	};

	//! Execution context. Const variables can be changed from code.
	enum EExecutionContext
	{
		EEC_CODE,									//!< Command executed from code.
		EEC_USER,									//!< Command executed from user input.
	};

private:
	CommandsType					mCommands;
	VariablesType					mVariables;
	AliasesType						mAliases;

	EExecutionContext				mExecContext;

private:
	Cmd																	( );
	~Cmd																( ) { };

public:
	/*! \brief Register new variable.
	 *	
	 *	\param[in] var		Variable name.
	 *	\param[in] def		Default value.
	 *	\param[in] flags	Variable flags. Combination of EVariableFlags.
	 *
	 *	\return New variable.
	 */
	Var *							Register							( const String& var, const String& def, const DWORD& flags = EVF_DEFAULT );

	/*! \brief Register new command.
	 *	
	 *	\param[in] cmd		Command name.
	 *	\param[in] fn		A function object that will be called.
	 *	\param[in] flags	Command flags. Combination of ECommandFlags.
	 */
	void							Register							( const String& cmd, const boost::function1< void, String >& fn, const DWORD& flags = ECF_DEFAULT );

	//! Retrieves variable flags.
	DWORD							GetVarFlags							( const String& var ) const;

	//! Retrieves command flags.
	DWORD							GetCmdFlags							( const String& cmd ) const;

	/*! \brief Executes one line.
	 *	
	 *
	 *	\note Commands may be separated by ';'. Strings can be places in '"'.
	 */
	void							Execute								( const String& cmd );

	//! Set execution context.
	void							SetExecutionContext					( const EExecutionContext& cont ) { mExecContext = cont; };

	//! Get execution context.
	const EExecutionContext&		GetExecutionContext					( ) const { return mExecContext; };

private:
	void							ExecOneCommand						( const String& cmd );

	const String&					FindAlias							( const String& cmd ) const;
};



_XFX_END

//  File xfx_lua_sm.h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_lua_sm.h 
 *	\brief Defines lua state machine manager and subclasses.
 */

_XFX_BEGIN





/*! \class LuaSMManager xfx_lua_sm.h "lua/xfx_lua_sm.h"
 *	\brief Lua state machine manager.
 *	\ingroup LuaGroup
 *
 *	This class stores all running Lua State Machines.
 *
 *	Lua State Machine is defined as two lua tables: one for algorithm and one for data.
 *	The first lua table must have one specific function "start", 
 *	which is called when state machine starts.
 *	Each function in first table should return next state function name.
 *	If function with returned name is not exists in table, lua call its panic function.
 *	To finish state machine, return "" (empty) state.
 *	To setup timer, call SM_SetTimer function with current state machine id, state name, time value in 
 *	100 microseconds and return "time_wait".
 *	The second lua table consist of data. This table is passed to each state in state machine.
 *
 *	Each function in algorithmic table takes two arguments.
 *	The first one is current executed state machine id. This id should only be used in SM_SetTimer function to
 *	represent current state machine. This id can be different at different time slices, but is not changed during 
 *	state execution time.
 *	The second argument is data table.
 *
 *	Lua State Machine Example:
 *
 *	\verbatim
 -- declare algorith table.
 example_sm =
 {
	start = function( sm_id, data )
		data.test_member = "test string";

		-- set one second timer to invoke state "time".
		SM_SetTimer( sm_id, "time", 10000 );

		-- note: "wait" state is absent, but we set timer.
		return "time_wait";
	end;

	time = function( sm_id, data )
		
		-- note: "end" state is absent and not timer set, this considered to be finish state.
		return "end";
	end;
 };

 -- run state machine with data object.
 SM_Run( example_sm, data_obj );
	\endverbatim
 *
 *	\author Andrew "RevEn" Karpushin
 */

class LuaSMManager : public Singleton< LuaSMManager >
{
	friend class Singleton< LuaSMManager >;

	//! State machine structure.
	struct StateMachine
	{
		//! ID
		int						id;

		//! Algorithm table.
		luabind::object			algorithm;

		//! Data table.
		luabind::object			data;

		//! Current state name.
		String					state;

		//! Local time, started from "start" state.
		DWORD					local_time;

		//! Defaut constructor.
		StateMachine			( ) : state( "start" ), local_time( 0 ) { };

		//! Another constructor.
		StateMachine			( int i, luabind::object& alg, luabind::object& d ) : local_time( 0 ), id( i ), algorithm( alg ), data( d ), state( "start" ) { };
	};

	//! State machine timer structure.
	struct SMTimer
	{
		//! State machine id.
		int						sm_id;

		//! New state.
		String					state;

		//! Time to start new state.
		int						time;
	};

	//! Pending execution list.
	std::vector< StateMachine >						mPendingExecutionList;

	//! Executed state machines.
	std::vector< StateMachine >						mExecutedList;

	//! Active timers.
	std::vector< SMTimer >							mActiveTimers;

	//! Last SM ID.
	int												mLastSMID;

private:
	LuaSMManager													( );
	~LuaSMManager													( );

public:
	//! Free all state machines.
	void							Free							( );

	/*!	\brief Run State Machine.
	 *
	 *	This function used from lua code, but may be invoked in C++ code.
	 *
	 *	\param[in]		alg			Algorithmic table.
	 *	\param[in]		data		Data table.
	 *
	 *	\note State Machine is not executed immediatly. It is added to pending execution list.
	 *
	 *	\return State Machine ID.
	 */
	int								RunSM							( luabind::object& alg, luabind::object& data );

	/*! \brief Set State Machine timer.
	 *
	 *	\param[in]		sm_id		State Machine index.
	 *	\param[in]		state		State name.
	 *	\param[in]		time		Time in 100 microseconds.
	 */
	void							SetTimer						( int sm_id, const String& state, int time );

	/*! \brief Find state machine and change state.
	 *
	 *	Try to find state machine with specified algorith and data and change it state.
	 *
	 *	\param[in]		sm_id		State Machine ID.
	 *	\param[in]		new_state	New state name.
	 */
	void							ChangeState						( int sm_id, const String& new_state );

	/*! \brief Get State Machine local time.
	 *
	 *	\param[in]		sm_id		State Machine ID.
	 *
	 *	\return State Machine local time.
	 */
	DWORD							GetSMLocalTime					( int sm_id ) const;

	/*! \brief Set State Machine local time.
	 *
	 *	\param[in]		sm_id		State Machine ID.
	 *	\param[in]		time		New time in 100 microseconds.
	 */
	void							SetSMLocalTime					( int sm_id, DWORD time );

	//! Update all state machines.
	void							Update							( DWORD mspf );

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void						LuaRegister						( lua_State * L );

private:
	/*! \brief Lua function SM_Run.
	 *
	 *	Lua wrapper to RunSM.
	 */
	static int						Lua_SM_Run						( luabind::object alg, luabind::object data );

	/*!	\brief Lua function SM_SetTimer.
	 *
	 *	Lua wrapper to SetTimer.
	 */
	static void						Lua_SM_SetTimer					( int sm_id, const char * state, int time );

	/*! \brief Lua function SM_ChangeState.
	 *
	 *	Lua wrapper to ChangeState.
	 */
	static void						Lua_SM_ChangeState				( int sm_id, const String& new_state );

	/*! \brief Lua function SM_GetLocalTime.
	 *
	 *	Lua wrapper to GetSMLocalTime.
	 */
	static DWORD					Lua_SM_GetLocalTime				( int sm_id );

	/*! \brief Lua function SM_SetLocalTime.
	 *
	 *	Lua wrapper to SetSMLocalTime.
	 */
	static void						Lua_SM_SetLocalTime				( int sm_id, DWORD seconds );
};




_XFX_END

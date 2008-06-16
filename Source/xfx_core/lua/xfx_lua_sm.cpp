//  File xfx_lua_sm.cpp                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_lua_sm.h"

_XFX_BEGIN




//
// LuaSMManager
//

LuaSMManager::LuaSMManager( ) :
	mLastSMID( 0 )
{
	mPendingExecutionList.reserve( 64 );
	mExecutedList.reserve( 256 );
	mActiveTimers.reserve( 256 );
}

LuaSMManager::~LuaSMManager( )
{
}

void LuaSMManager::Free( )
{
	mPendingExecutionList.clear( );
	mExecutedList.clear( );
	mActiveTimers.clear( );
}

int LuaSMManager::RunSM( luabind::object& alg, luabind::object& data )
{
	mPendingExecutionList.push_back( StateMachine( mLastSMID++, alg, data ) );

	return mLastSMID - 1;
}

void LuaSMManager::Update( DWORD mspf )
{
	PROFILE( __FUNCTION__, "General" );

	// create new state machines
	std::copy(
		mPendingExecutionList.begin( ),
		mPendingExecutionList.end( ),
		std::back_inserter( mExecutedList )
		);

	mPendingExecutionList.clear( );

	// process state machines
	for( std::vector< StateMachine >::iterator it = mExecutedList.begin( ); it != mExecutedList.end( ); )
	{
		( *it ).local_time += mspf;

		// execute one state machine
		if( ( *it ).state == "" )
		{
			it = mExecutedList.erase( it );
		}
		else if ( ( *it ).state == "time_wait" )
		{
			// do nothing
			++it;
		}
		else
		{
			try
			{
				( *it ).state = luabind::call_function< const char * >(
					( *it ).algorithm[ ( *it ).state ],
					( *it ).id, ( *it ).data );
			}
			catch( std::exception& e )
			{
				gError( "Call to state machine function failed, reason: %s, last SM state: %s", e.what( ), ( *it ).state.c_str( ) );
				( *it ).state = "";
			}
			catch( ... )
			{
				gError( "Call to state machine function failed, reason: %s, last SM state: %s", "unknown", ( *it ).state.c_str( ) );
				( *it ).state = "";
			}

			++it;
		}
	}

	// process timers
	for( std::vector< SMTimer >::iterator time_it = mActiveTimers.begin( ); time_it != mActiveTimers.end( ); )
	{
		( *time_it ).time -= mspf;

		if( ( *time_it ).time < 0 )
		{
			// time's up
			std::vector< StateMachine >::iterator sm_it = std::find_if(
				mExecutedList.begin( ),
				mExecutedList.end( ),
				boost::bind( std::equal_to< int >( ), boost::bind( &StateMachine::id, _1 ), ( *time_it ).sm_id )
				);

			if( sm_it != mExecutedList.end( ) )
			{
				( *sm_it ).state = ( *time_it ).state;
			}
			else
			{
				gWarn( "State machine with id %d finished before time event occured!", ( *time_it ).sm_id );
			}

			time_it = mActiveTimers.erase( time_it );
		}
		else
			++time_it;
	}
}

void LuaSMManager::SetTimer( int sm_id, const String& state, int time )
{
	for( std::vector< SMTimer >::iterator time_it = mActiveTimers.begin( ); time_it != mActiveTimers.end( ); ++time_it )
	{
		if( ( *time_it ).sm_id == sm_id )
		{
			( *time_it ).state = state;
			( *time_it ).time = time;
			return;
		}
	}

	SMTimer timer;
	timer.sm_id = sm_id;
	timer.state = state;
	timer.time = time;

	mActiveTimers.push_back( timer );
}

void LuaSMManager::ChangeState( int sm_id, const String& new_state )
{
	// try to find sm
	for( std::vector< StateMachine >::iterator it = mExecutedList.begin( ); it != mExecutedList.end( ); ++it )
	{
		if( ( *it ).id == sm_id )
		{
			( *it ).state = new_state;

			mActiveTimers.erase( std::remove_if( 
				mActiveTimers.begin( ),
				mActiveTimers.end( ),
				boost::bind( std::equal_to< int >( ), boost::bind( &SMTimer::sm_id, _1 ), sm_id )
				),
				mActiveTimers.end( )
			);

			return;
		}
	}
}

DWORD LuaSMManager::GetSMLocalTime( int sm_id ) const
{
	for( std::vector< StateMachine >::const_iterator it = mExecutedList.begin( ); it != mExecutedList.end( ); ++it )
	{
		if( ( *it ).id == sm_id )
			return ( *it ).local_time;
	}

	return 0;
}

void LuaSMManager::SetSMLocalTime( int sm_id, DWORD time )
{
	for( std::vector< StateMachine >::iterator it = mExecutedList.begin( ); it != mExecutedList.end( ); ++it )
	{
		if( ( *it ).id == sm_id )
		{
			( *it ).local_time = time;
			return;
		}
	}
}

void LuaSMManager::LuaRegister( lua_State * L )
{
	luabind::module( L )
	[
		luabind::def(
			"SM_Run",
			&LuaSMManager::Lua_SM_Run
			),
		luabind::def(
			"SM_SetTimer",
			&LuaSMManager::Lua_SM_SetTimer,
			luabind::discard_result
			),
		luabind::def(
			"SM_ChangeState",
			&LuaSMManager::Lua_SM_ChangeState,
			luabind::discard_result
			),
		luabind::def(
			"SM_GetLocalTime",
			&LuaSMManager::Lua_SM_GetLocalTime
			),
		luabind::def(
			"SM_SetLocalTime",
			&LuaSMManager::Lua_SM_SetLocalTime,
			luabind::discard_result
			)
	];
}

int LuaSMManager::Lua_SM_Run( luabind::object alg, luabind::object data )
{
	return LuaSMManager::Instance( ).RunSM( alg, data );
}

void LuaSMManager::Lua_SM_SetTimer( int sm_id, const char * state, int time )
{
	LuaSMManager::Instance( ).SetTimer( sm_id, state, time );
}

void LuaSMManager::Lua_SM_ChangeState( int sm_id, const String& new_state )
{
	LuaSMManager::Instance( ).ChangeState( sm_id, new_state );
}

DWORD LuaSMManager::Lua_SM_GetLocalTime( int sm_id )
{
	return LuaSMManager::Instance( ).GetSMLocalTime( sm_id );
}

void LuaSMManager::Lua_SM_SetLocalTime( int sm_id, DWORD time )
{
	LuaSMManager::Instance( ).SetSMLocalTime( sm_id, time );
}



_XFX_END

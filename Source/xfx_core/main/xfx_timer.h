//  File xfx_timer.h                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_timer.h 
 *	\brief %Timer, based on QueryPerformanceCounter.
 */

_XFX_BEGIN





/*! \class Timer xfx_timer.h "main/xfx_timer.h"
 *	\brief %Timer class.
 *	\ingroup MainGroup
 *	
 *	Provides following time methods:
 *	- Get current time.
 *	- Get time between two polls (in 100 microsecondes).
 *	- Pause/Resume timer.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Timer
{
	//! Bridge pattern implementation (platform-dependent implementation).
	class Timer_impl;
	boost::scoped_ptr< Timer_impl >	mImpl;

	boost::uint32_t				m100MSPF;
	boost::uint32_t				m100MicroSeconds;
	float						mSpeed;

	bool						mIsPaused;

public:
	Timer											( );
	~Timer											( );

	Timer											( const Timer& t );
	Timer&						operator =			( const Timer& t );

	//! Get current time between to pools in 100 microseconds.
	const boost::uint32_t&		GetMSPF100			( ) const { return m100MSPF; };

	//! Get elapsed time in 100 microseconds.
	const boost::uint32_t&		GetMicroSeconds100	( ) const { return m100MicroSeconds; };

	//! Set elapsed time.
	void						SetMicroSeconds100	( const boost::uint32_t& time );

	//! Get 'paused' flag.
	const bool&					IsPaused			( ) const { return mIsPaused; };

	//! Pause timer.
	void						Pause				( );

	//! Resume timer.
	void						Resume				( );

	//! Poll timer.
	void						Update				( );
};



_XFX_END

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





/*! \class Timer xfx_timer.h "engine/xfx_timer.h"
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
	LARGE_INTEGER				mslasttime;

	DWORD						m100MSPF;
	DWORD						m100MicroSeconds;
	float						mSpeed;
	LARGE_INTEGER				mTicksPerSecond;

	LARGE_INTEGER				mstarttime;
	LARGE_INTEGER				moldtime;

	bool						mIsPaused;

public:
	Timer											( );
	~Timer											( ) { };

	//! Get current time between to pools in 100 microseconds.
	const DWORD&				MSPF100				( ) const { return m100MSPF; };

	//! Get elapsed time in 100 microseconds.
	const DWORD&				MicroSeconds100		( ) const { return m100MicroSeconds; };

	//! Set elapsed time.
	void						MicroSeconds100		( DWORD time ) { mstarttime.QuadPart += ( __int64( m100MicroSeconds ) - time ) * mTicksPerSecond.QuadPart / 10000; m100MicroSeconds = time; };

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

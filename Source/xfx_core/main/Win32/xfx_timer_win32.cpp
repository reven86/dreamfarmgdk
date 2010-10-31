//  File xfx_timer_win32.cpp                                                  |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "..\xfx_timer.h"

_XFX_BEGIN



class Timer::Timer_impl
{
	LARGE_INTEGER				mslasttime;
	LARGE_INTEGER				mTicksPerSecond;
	LARGE_INTEGER				mstarttime;
	LARGE_INTEGER				moldtime;
	bool						mIsPaused;

public:
	Timer_impl( ) :
		mIsPaused( false )
	{
		QueryPerformanceFrequency	( &mTicksPerSecond );
		QueryPerformanceCounter		( &mstarttime );
		mslasttime = mstarttime;
	};

	~Timer_impl( ) { };

	void Pause( )
	{
		mIsPaused = true;
		QueryPerformanceCounter( &moldtime );
	}

	void Resume( )
	{
		mIsPaused = false;
		QueryPerformanceCounter( &mslasttime );
		mstarttime.QuadPart += mslasttime.QuadPart - moldtime.QuadPart;
	}

	void Update( boost::uint32_t& mspf, boost::uint32_t& sec )
	{
		LARGE_INTEGER currenttime;
		
		QueryPerformanceCounter( &currenttime );

		mspf = static_cast< DWORD >( ( currenttime.QuadPart - mslasttime.QuadPart ) * 10000 / mTicksPerSecond.QuadPart );// * mSpeed;

		if( !mIsPaused )
			sec = static_cast< DWORD >( ( currenttime.QuadPart - mstarttime.QuadPart ) * 10000 / mTicksPerSecond.QuadPart );// * mSpeed;

		// sometimes on AMD Athlon X2 delta between two frames can be negative
		if( static_cast< int >( mspf ) < 0 )
			mspf = 0;

		mslasttime = currenttime;
	}

	void SetMicroSeconds100( boost::uint32_t oldtime, boost::uint32_t time )
	{
		mstarttime.QuadPart += ( __int64( oldtime ) - time ) * mTicksPerSecond.QuadPart / 10000;
	}
};



_XFX_END
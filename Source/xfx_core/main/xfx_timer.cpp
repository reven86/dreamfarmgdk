//  File xfx_timer.cpp                                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_timer.h"

_XFX_BEGIN





//
// Timer
//

Timer::Timer( ) :
	mSpeed( 1.0f ),
	mIsPaused( false ),
	m100MicroSeconds( 0 ),
	m100MSPF( 0 )
{
	QueryPerformanceFrequency	( &mTicksPerSecond );
	QueryPerformanceCounter		( &mstarttime );
}

void Timer::Pause( )
{
	mIsPaused = true;
	QueryPerformanceCounter( &moldtime );
}

void Timer::Resume( )
{
	if( mIsPaused )
	{
		mIsPaused = false;

		QueryPerformanceCounter( &mslasttime );
		mstarttime.QuadPart += mslasttime.QuadPart - moldtime.QuadPart;
	}
}

void Timer::Update( )
{
	LARGE_INTEGER currenttime;
	
	QueryPerformanceCounter( &currenttime );

	m100MSPF = static_cast< DWORD >( ( currenttime.QuadPart - mslasttime.QuadPart ) * 10000 / mTicksPerSecond.QuadPart );// * mSpeed;
	m100MicroSeconds = static_cast< DWORD >( ( currenttime.QuadPart - mstarttime.QuadPart ) * 10000 / mTicksPerSecond.QuadPart );// * mSpeed;

	// sometimes on AMD Athlon X2 delta between two frames can be negative
	if( static_cast< int >( m100MSPF ) < 0 )
		m100MSPF = 0;

	mslasttime = currenttime;
}




_XFX_END
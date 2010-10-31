//  File xfx_timer.cpp                                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_timer.h"

#ifdef _WIN32
#include "win32/xfx_timer_win32.cpp"
#endif

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
	mImpl.reset( new Timer_impl( ) );
}

Timer::~Timer( )
{
}

Timer::Timer( const Timer& t )
{
	*this = t;
}

Timer& Timer::operator = ( const Timer& t )
{
	mSpeed = t.mSpeed;
	mIsPaused = t.mIsPaused;
	m100MicroSeconds = t.m100MicroSeconds;
	m100MSPF = t.m100MSPF;

	mImpl.reset( new Timer_impl( *t.mImpl ) );

	return *this;
}

void Timer::Pause( )
{
	if( !mIsPaused )
	{
		mIsPaused = true;
		mImpl->Pause( );
	}
}

void Timer::Resume( )
{
	if( mIsPaused )
	{
		mIsPaused = false;
		mImpl->Resume( );
	}
}

void Timer::Update( )
{
	mImpl->Update( m100MSPF, m100MicroSeconds );
}

void Timer::SetMicroSeconds100( const boost::uint32_t& time )
{
	mImpl->SetMicroSeconds100( m100MicroSeconds, time );
	m100MicroSeconds = time;
}




_XFX_END
//  File xfx_profiler.cpp                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_profiler.h"
#include "xfx_log.h"

_XFX_BEGIN







//
// Profiling Manager
//

ProfilingManager::ProfilingManager( ) :
	mCurrentParentsCount( 0 ),
	mRegisteredGroupsCount( 0 ),
	mRegisteredObjectsCount( 0 ),
	mCurrentMSPF100( 100 ),
	mExternalProfilerPauseFn( NULL ),
	mExternalProfilerResumeFn( NULL )
{
	HMODULE profile_module;

	profile_module = LoadLibraryA( "caprofapi32.dll" );

	if( profile_module )
	{
		mExternalProfilerPauseFn = ( void ( * ) ( void ) ) GetProcAddress( profile_module, "CAProfPause" );
		mExternalProfilerResumeFn = ( void ( * ) ( void ) ) GetProcAddress( profile_module, "CAProfResume" );
	}
	else
	{
		profile_module = LoadLibraryA( "vtuneapi.dll" );

		if( profile_module )
		{
			mExternalProfilerPauseFn = ( void ( * ) ( void ) ) GetProcAddress( profile_module, "VTPause" );
			mExternalProfilerResumeFn = ( void ( * ) ( void ) ) GetProcAddress( profile_module, "VTResume" );
		}
	}
}

ProfilingManager::~ProfilingManager( )
{
}

DWORD ProfilingManager::RegisterProfilerObject( const char * name, const char * group )
{
	_ASSERTE( mRegisteredObjectsCount < MAX_OBJECTS );
	_ASSERTE( mRegisteredGroupsCount < MAX_GROUPS );

	unsigned g;
	for( g = 0; g < mRegisteredGroupsCount; g++ )
		if( mRegisteredGroups[ g ].first == group )
			break;

	LARGE_INTEGER null_int;
	null_int.QuadPart = 0;

	if( g == mRegisteredGroupsCount )
	{
		if( mRegisteredGroupsCount == MAX_GROUPS )
			return INVALID_OBJECT_ID;

		mRegisteredGroups[ mRegisteredGroupsCount++ ] = std::make_pair( String( group ), null_int );
	}

	unsigned o;
	for( o = 0; o < mRegisteredObjectsCount; o++ )
		if( mRegisteredObjects[ o ].first == name )
			break;

	// this can be if PROFILE macro set into template function
	//_ASSERTE( o == mRegisteredObjectsCount );

	if( o == mRegisteredObjectsCount )
	{
		if( mRegisteredObjectsCount == MAX_OBJECTS )
			return INVALID_OBJECT_ID;

		mRegisteredObjects[ mRegisteredObjectsCount++ ] = std::make_pair( String( name ), null_int );
	}

	return g * MAX_OBJECTS + o;
}

void ProfilingManager::StartProfiler( const DWORD& profile_object_id )
{
	_ASSERTE( profile_object_id != INVALID_OBJECT_ID );
	_ASSERTE( mCurrentParentsCount < MAX_OBJECTS );

	if( profile_object_id == INVALID_OBJECT_ID )
		return;

	if( mCurrentParentsCount >= MAX_OBJECTS )
		return;

	LARGE_INTEGER time;

	QueryPerformanceCounter( &time );

	mCurrentParents[ mCurrentParentsCount++ ] = std::make_pair( profile_object_id, time );
}

void ProfilingManager::StopProfiler( )
{
	LARGE_INTEGER diff;
	QueryPerformanceCounter( &diff );

	_ASSERTE( mCurrentParentsCount > 0 );

	if( mCurrentParentsCount == 0 )
		return;

	mCurrentParentsCount--;
	diff.QuadPart -= mCurrentParents[ mCurrentParentsCount ].second.QuadPart;

	int g, o;

	g = mCurrentParents[ mCurrentParentsCount ].first / MAX_OBJECTS;
	o = mCurrentParents[ mCurrentParentsCount ].first % MAX_OBJECTS;

	mRegisteredGroups[ g ].second.QuadPart += diff.QuadPart * mCurrentMSPF100 / 10000;
	mRegisteredObjects[ o ].second.QuadPart += diff.QuadPart * mCurrentMSPF100 / 10000;

	//_ASSERTE( mRegisteredGroups[ g ].second.QuadPart >= 0 );
	//_ASSERTE( mRegisteredObjects[ o ].second.QuadPart >= 0 );

	for( unsigned p = 0; p < mCurrentParentsCount; p++ )
		mCurrentParents[ p ].second.QuadPart += diff.QuadPart;
}

void ProfilingManager::SaveProfilerResults( )
{
	std::copy( mRegisteredGroups.begin( ), mRegisteredGroups.begin( ) + mRegisteredGroupsCount, mProfileGroupsResult.begin( ) );
	std::copy( mRegisteredObjects.begin( ), mRegisteredObjects.begin( ) + mRegisteredObjectsCount, mProfileObjectsResult.begin( ) );

	DWORD mspf = mCurrentMSPF100 > 10000 ? 10000 : mCurrentMSPF100;

	// reset 
	for( unsigned g = 0; g < mRegisteredGroupsCount; g++ )
		mRegisteredGroups[ g ].second.QuadPart -= mRegisteredGroups[ g ].second.QuadPart * mspf / 10000;

	for( unsigned o = 0; o < mRegisteredObjectsCount; o++ )
		mRegisteredObjects[ o ].second.QuadPart -= mRegisteredObjects[ o ].second.QuadPart * mspf / 10000;
}

void ProfilingManager::PauseResumeExternalProfiler( bool pause ) const
{
	if( pause )
	{
		if( mExternalProfilerPauseFn )
		{
			mExternalProfilerPauseFn( );
			gMess( "Profiler paused" );
		}
	}
	else
	{
		if( mExternalProfilerResumeFn )
		{
			mExternalProfilerResumeFn( );
			gMess( "Profiler resumed" );
		}
	}
}




_XFX_END
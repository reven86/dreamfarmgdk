//  File xfx_profiler.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_profiler.h 
 *	\brief Simple lightweight easy-to-use profiler.
 */

_XFX_BEGIN




/*! \class ProfilingManager xfx_profiler.h "utility/xfx_profiler.h"
 *	\brief %Profiling manager.
 *	\ingroup UtilityGroup
 *
 *	Collect and accumulate all time data from different profiler objects.
 *	Calculates average self time per second or per specified frame count.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class ProfilingManager : public Singleton< ProfilingManager >
{
	friend class Singleton< ProfilingManager >;
	
	typedef std::pair< String, LARGE_INTEGER >	ProfileInfo;

	enum { MAX_GROUPS = 256 };
	enum { MAX_OBJECTS = 256 };

	enum { INVALID_OBJECT_ID = MAX_GROUPS * MAX_OBJECTS };

	boost::array< ProfileInfo, MAX_GROUPS >		mRegisteredGroups;
	boost::array< ProfileInfo, MAX_GROUPS >		mProfileGroupsResult;
	unsigned									mRegisteredGroupsCount;

	boost::array< ProfileInfo, MAX_OBJECTS >	mRegisteredObjects;
	boost::array< ProfileInfo, MAX_OBJECTS >	mProfileObjectsResult;
	unsigned									mRegisteredObjectsCount;

	boost::array< std::pair< DWORD, LARGE_INTEGER >, MAX_OBJECTS >		mCurrentParents;
	unsigned															mCurrentParentsCount;

	DWORD										mCurrentMSPF100;

	void ( * mExternalProfilerPauseFn ) ( void );
	void ( * mExternalProfilerResumeFn ) ( void );

private:
	ProfilingManager													( );
	~ProfilingManager													( );

public:
	/*! \brief Register new profile object.
	 *
	 *	Registration occured once in runtime for every profile object.
	 *
	 *	\param[in]		name		Profile object name.
	 *	\param[in]		group		Profile object group.
	 *
	 *	\return Profile object unique ID.
	 */
	DWORD					RegisterProfilerObject						( const char * name, const char * group );

	/*! \brief Start profile.
	 *
	 *	Stores current time and push parent object onto stack
	 *
	 *	\param[in]		profile_object_id		Profile object id;
	 */
	void					StartProfiler								( const DWORD& profile_object_id );

	/*! \brief Stop profile.
	 *
	 *	Stop profile timer for current object and calculate it self time and all parents self time.
	 */
	void					StopProfiler								( );

	/*! \brief Save current results and reset profile data for new frame.
	 *
	 *	Saved results will be used in GetGroupStatistics and GetObjectStatistics.
	 */
	void					SaveProfilerResults							( );

	/*! \brief Set calculation frame rate.
	 *
	 *	\note Use this method each frame with current frame time to calculate results within 1 second.
	 *
	 *	\param[in]		mspf100			Frame time in 100 microseconds.
	 */
	void					SetFrameTime								( const DWORD& mspf100 ) { mCurrentMSPF100 = mspf100; };

	/*! \brief Get group statistics.
	 *
	 *	\param[in]		v			Visitor that accept group name as first argument and group self time.
	 */
	template< class _Visitor >
	void					GetGroupStatistics							( _Visitor v ) const;

	/*! \brief Get object statistics.
	 *
	 *	\param[in]		v			Visitor that accept object name as first argument and group self time.
	 */
	template< class _Visitor >
	void					GetObjectStatistics							( _Visitor v ) const;

	/*! \brief Draw performance information by objects and groups.
	 *
	 *	\param[in]		fnt			Font object.
	 *	\param[in]		white_shd	White shader.
	 */
	//void					DrawPerformanceInfo							( const class Font& fnt, const boost::shared_ptr< const class Shader >& white_shd ) const;

	/*! \brief Pause or resume external profiler ( Intel VTune or AMD Catalyst )
	 *
	 *	\param[in]		pause		\b true to pause profiler.
	 */
	void					PauseResumeExternalProfiler					( bool pause ) const;
};






/*! \class ProfilingObject xfx_profiler.h "utility/xfx_profiler.h"
 *	\brief %Profiling object.
 *	\ingroup UtilityGroup
 *
 *	Start profiler timing in constructor and stop it in destructor.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class ProfilingObject : boost::noncopyable
{
public:
	ProfilingObject														( const DWORD& id ) { ProfilingManager::Instance( ).StartProfiler( id ); };
	~ProfilingObject													( ) { ProfilingManager::Instance( ).StopProfiler( ); };
};




//! Helper macro to easy to use profile objects
#ifndef __XFX_DISABLE_PROFILER__
#define PROFILE( name, group ) \
	static DWORD __xfx_profile_id = xfx::ProfilingManager::Instance( ).RegisterProfilerObject( name, group ); \
	xfx::ProfilingObject __xfx_profile_obj( __xfx_profile_id );
#else
#define PROFILE( name, group )
#endif






//
// Template methods
//

template< class _Visitor >
void ProfilingManager::GetGroupStatistics( _Visitor v ) const
{
	std::for_each(
		mProfileGroupsResult.begin( ),
		mProfileGroupsResult.begin( ) + mRegisteredGroupsCount,
		boost::bind( v, boost::bind( &ProfileInfo::first, _1 ), boost::bind( &ProfileInfo::second, _1 ) )
		);
};

template< class _Visitor >
void ProfilingManager::GetObjectStatistics( _Visitor v ) const
{
	std::for_each(
		mProfileObjectsResult.begin( ),
		mProfileObjectsResult.begin( ) + mRegisteredObjectsCount,
		boost::bind( v, boost::bind( &ProfileInfo::first, _1 ), boost::bind( &ProfileInfo::second, _1 ) )
		);
};



_XFX_END
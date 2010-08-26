//  File xfx_cache.h                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_cache.h 
 *	\brief %Cache system, holds frequently used resources in one instance.
 */

_XFX_BEGIN






/*! \class Caches xfx_cache.h "utility/xfx_cache.h"
 *	\brief %Caches holder.
 *	\ingroup UtilityGroup
 *	
 *	Holds all caches and control for clearing them.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Caches : public Singleton< Caches >
{
	friend class Singleton< Caches >;

public:
	//! Clear callback function.
	typedef boost::signal0< void >		ClearCallbacksType;

private:
	ClearCallbacksType					mClearCallbacks;

private:
	Caches													( ) { };
	~Caches													( ) { };

public:
	//! Get/Set clear callbacks.
	ClearCallbacksType&				ClearCallbacks			( ) { return mClearCallbacks; };

	//! Get clear callbacks.
	const ClearCallbacksType&		ClearCallbacks			( ) const { return mClearCallbacks; };

public:
	//! Flush all caches by invoking registered clear callbacks.
	void							FlushAll				( ) { mClearCallbacks( ); };
};





/*! \class Cache xfx_cache.h "utility/xfx_cache.h"
 *	\brief %Cache class.
 *	\ingroup UtilityGroup
 *	
 *	Holds registered resources in one instance and provides access to newly loaded resources.
 *
 *	\author Andrew "RevEn" Karpushin
 */

template< class T >
class Cache
{
	typedef stdext::hash_map< String, boost::shared_ptr< T >, HashCompare< String > >	ResourcesType;
	ResourcesType					mResources;

	boost::signals::connection		mCachesConnection;

public:
	//! Constructs empty cache.
	Cache													( ) { };

	//! Destructs cache object and unload all loaded resources.
	~Cache													( ) { Clear( ); mCachesConnection.disconnect( ); };

	/*!	\brief Register new resource.
	 *	
	 *	Register new resource from file or clone existing one.
	 *
	 *	\param	filename	Resource filename.
	 *	\return	Returns shared pointer to resource.
	 */
	boost::shared_ptr< typename boost::add_const< T >::type >	Register	( const String& filename )
	{
		if( !mCachesConnection.connected( ) )
			mCachesConnection = Caches::Instance( ).ClearCallbacks( ).connect(
				boost::bind( &Cache< T >::Clear, this ) );

		String lower_name = boost::algorithm::to_lower_copy( filename );
		typename ResourcesType::iterator it = mResources.find( lower_name );

		T * res;
		if( it == mResources.end( ) )
		{
			gMess( "Registering resource: %s", lower_name.c_str( ) );

			boost::shared_ptr< T > r( res = new T );

			if( FAILED( res->LoadFile( lower_name ) ) )
			{
				gError( "Can't load resource: %s", lower_name.c_str( ) );
				return boost::shared_ptr< typename boost::add_const< T >::type >( );
			}

			mResources.insert( ResourcesType::value_type( lower_name, r ) );
			return r;
		}

		return ( *it ).second;
	};

	/*!	\brief Register new resource.
	 *	
	 *	Register new resource from file or clone existing one.
	 *
	 *	\note Makes a copy of resource.
	 *
	 *	\param	object		Resource.
	 *	\param	filename	Resource filename.
	 *
	 *	\return S_OK if object was successfully registered.
	 */
	HRESULT							Register					( T& object, const String& filename )
	{
		boost::shared_ptr< typename boost::add_const< T >::type > o = Register( filename );
		if( o )
		{
			object = *o;
			return S_OK;
		}

		return XFXERR_INVALIDCALL;
	};

	/*!	\brief Unregister resource.
	 *	
	 *	Unregister previously registered resource.
	 *
	 *	\param	object	Resource pointer object.
	 */
	void							Unregister					( const boost::shared_ptr< typename boost::add_const< T >::type >& object )
	{
		gMess( "Unregistering resource: %s", object->Filename( ).c_str( ) );

		// Linear search used due to rarely function using.
		typename ResourcesType::iterator it = mResources.begin( );

		while( it != mResources.end( ) )
		{
			if( ( *it ).second == object )
			{
				mResources.erase( it );
				break;
			}

			++it;
		}
	};

	//! Unregister all registered resources.
	void							Clear						( )
	{
		for( typename ResourcesType::iterator it = mResources.begin( ); it != mResources.end( ); it++ )
			gMess( "Unregistering resource: %s", ( *it ).first.c_str( ) );

		mResources.clear( );
	};

	//! Reload all resources.
	void							ReloadAll					( )
	{
		BOOST_FOREACH( typename ResourcesType::value_type& res, mResources )
		{
			gMess( "Reloading resource: %s", res.first.c_str( ) );

			// hack
			const_cast< T& >( *res.second ).LoadFile( res.first );
		}
	}
};





_XFX_END

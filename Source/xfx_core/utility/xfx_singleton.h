//  File xfx_singleton.h                                                      |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_singleton.h 
 *	\brief Base singleton class.
 */

_XFX_BEGIN





/*! \class Singleton xfx_singleton.h "utility/xfx_singleton.h"
 *	\brief %Singleton base class.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

template< class T >
class Singleton : public boost::noncopyable
{
protected:
	Singleton					( ) { };
	virtual ~Singleton			( ) { };

public:
	//! Get an instance.
	static T& Instance			( )
	{
		static T instance;

		return instance;
	};
};



_XFX_END

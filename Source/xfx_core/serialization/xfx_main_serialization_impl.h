//  File xfx_main_serialization_impl.h                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_main_serialization_impl.h 
 *	\brief Serialization implementation for Main library.
 */




_XFX_BEGIN




//
// Resource
//

template< class _Archive >
void Resource::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mFilename );
}



//
// Script resource
//
template< class _Archive >
void ScriptResource::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Resource );
}





_XFX_END
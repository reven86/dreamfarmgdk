//  File xfx_uifocusable.h                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_uifocusable.h 
 *	\brief Base class for user controls whose can have focus.
 */

_XFX_BEGIN






/*! \class UIFocusable xfx_uifocusable.h "ui/xfx_uifocusable.h"
 *	\brief Base class for user controls whose can have focus.
 *	\ingroup InterfaceGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class UIFocusable
{
	friend class UIInterface;

	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UIFocusable													( ) { };
	virtual ~UIFocusable										( ) { };

	//! Process pushed keys.
	virtual void					ProcessKey					( wchar_t ) { };

protected:
	//! Set focus callback.
	virtual void					OnSetFocus					( ) { };

	//! Kill focus callback.
	virtual void					OnKillFocus					( ) { };
};





_XFX_END

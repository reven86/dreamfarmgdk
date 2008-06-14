//  File xfx_console.h                                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_console.h 
 *	\brief Defines base console class.
 */

_XFX_BEGIN




/*!	\class Console xfx_console.h "ui/xfx_console.h"
 *	\brief Base console class
 *	\ingroup MainGroup
 *
 *	Console is a user defined control that
 *	accept messages and executes user commands through command processor.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Console
{
public:
	Console												( ) { };
	virtual ~Console									( ) { };

	//! Clear console messages.
	virtual void				Clear					( ) = 0;

	/*! \brief Print message to console.
	 *
	 *	\param[in]	type	Message type.
	 *	\param[in]	msg		Message to print.
	 */
	virtual void				Print					( const Log::EMessageType& type, const WString& msg ) = 0;

	/*! \brief Show/hide console.
	 */
	virtual void				Toggle					( ) = 0;

	//! Get 'visible' flag.
	virtual bool				IsVisible				( ) const = 0;
};




_XFX_END
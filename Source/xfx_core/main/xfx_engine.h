//  File xfx_engine.h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_engine.h 
 *	\brief Defines engine base class.
 */

_XFX_BEGIN




/*!	\defgroup MainGroup Main classes.
 */

/*! \class Engine xfx_engine.h "main/xfx_engine.h"
 *	\brief %Engine facade class.
 *	\ingroup MainGroup
 *	
 *	%Engine is a kernel for the library. Provides logging, version handler, user terminal
 *	via command processor, etc.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Engine : public Singleton< Engine >
{
	friend class Singleton< Engine >;

	//! Version string. Generated through control version system.
	static const String						msVersion;

	boost::scoped_ptr< class Log >			mEngineLogPtr;
	boost::shared_ptr< class Console >		mConsoleControlPtr;

protected:
	Engine																( );
	~Engine																( );

public:
	/*!	\brief %Engine initialization.
	 *	
	 *	Initialize engine by window handle. Performs next tasks:
	 *	- gather OS information
	 *	- gather CPU information
	 *	- initialize input system (calls Input::Init)
	 *
	 *	\return Returns \b true if engine has been initialized sucessfully.
	 */
	bool									Init						( );

	/*!	\brief %Engine deinitialization.
	 *
	 *	Flush all Caches, deinitialize Input, deinitialize Render.
	 *
	 *	\return Always return \b true.
	 */
	bool									Shutdown					( );

	const String&							Version						( ) const { return msVersion; };

	/*! \brief Write message to common log file.
	 *
	 *	\param[in]	type	Message type.
	 *	\param[in]	msg		Message.
	 */
	void									Log							( const Log::EMessageType& type, const String& msg );

	/*! \brief Prints message to console.
	 *
	 *	\param[in]	type	Message type.
	 *	\param[in]	str		Message.
	 */
	void									PrintToConsole				( const Log::EMessageType& type, const WString& str );

	/*! \brief Sets console control.
	 *
	 *	Associate console control. Console is a user defined interface control that
	 *	accept messages and executes user commands through command processor.
	 *
	 *	\param[in]	ctrl	Console UI control.
	 *	\see Console, Cmd.
	 */
	void									AssociateConsoleControl		( const boost::shared_ptr< Console >& ctrl ) { mConsoleControlPtr = ctrl; };

	/*! \brief Toggle visibility for console object.
	 */
	void									ToggleConsole				( );

	/*! \brief Get console 'visible' flag.
	 */
	bool									IsConsoleActive				( ) const;
};




_XFX_END





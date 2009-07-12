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

	//! Version string. Generated through SVN.
	static const int						msVersionMajor;
	static const int						msVersionMinor;
	static const int						msVersionBuild;
	static const String						msVersionBuildDate;
	static const String						msVersion;

	boost::scoped_ptr< class Log >			mEngineLogPtr;
	boost::shared_ptr< class Console >		mConsoleControlPtr;

protected:
	Engine																( );
	~Engine																( );

public:
	/*!	\brief %Engine initialization.
	 *	
	 *	Initializes engine by window handle. Performs next tasks:
	 *	- gather OS information
	 *	- gather CPU information
	 *	- initialize input system (calls Input::Init)
	 *
	 *	\param[in]	init_input		Init input system (gGetApplication must provide valid instance).
	 *
	 *	\return Returns \b true if engine has been initialized sucessfully.
	 */
	bool									Init						( bool init_input = true );

	/*!	\brief %Engine deinitialization.
	 *
	 *	Flushes all Caches, deinitializes Input.
	 *
	 *	\return Always return \b true.
	 */
	bool									Shutdown					( );

	//! Gets engine major version number.
	const int&								GetVersionMajor				( ) const { return msVersionMajor; };

	//! Get engine minor version number.
	const int&								GetVersionMinor				( ) const { return msVersionMinor; };

	//! Get engine build number.
	const int&								GetVersionBuild				( ) const { return msVersionBuild; };

	//! Get engine build date as string.
	const String&							GetVersionBuildDate			( ) const { return msVersionBuildDate; };

	//! Gets engine full version as string.
	const String&							GetVersionFull				( ) const { return msVersion; };

	/*! \brief Writes message to common log file.
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





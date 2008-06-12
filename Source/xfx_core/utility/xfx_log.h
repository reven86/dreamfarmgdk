//  File xfx_log.h                                                            |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_string.h"

/*!	\file xfx_log.h 
 *	\brief %Logging.
 */

_XFX_BEGIN






/*! \class Log xfx_log.h "utility/xfx_log.h"
 *	\brief %Logging class.
 *	\ingroup UtilityGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Log : boost::noncopyable
{
public:
	//! Message type.
	enum EMessageType
	{
		EMT_CRITICAL,				//!< Critical message.
		EMT_ERROR,					//!< Error.
		EMT_WARNING,				//!< Warning.
		EMT_DEFAULT					//!< Default.
	};

private:
	//! Log file name.
	String							mLogFile;

public:
	//! Open specified log file and write out date and time stamp.
	Log															( const String& log_file );

	//! Close log file.
	~Log														( );

	/*!	\brief Print message to log file.
	 *
	 *	\param[in]	type	Message type.
	 *	\param[in]	msg		Message.
	 */
	void							Print						( const EMessageType& type, const String& msg );
};



_XFX_END





//
// Additional global functions
//

/*! \brief Print message to console.
 *	\ingroup Globals
 *
 *	Print message only to console and no other logs.
 *
 */
void gToConsole	( const char * fmt, ... );

/*! \brief Print usual message to primary log.
 *	\ingroup Globals
 *
 */
void gMess		( const char * fmt, ... );

/*! \brief Print warning message to primary log.
 *	\ingroup Globals
 *
 *	Insert warning tag prior message in log.
 */
void gWarn		( const char * fmt, ... );

/*! \brief Print error message to primary log.
 *	\ingroup Globals
 *
 *	Insert error tag prior message in log.
 */
void gError		( const char * fmt, ... );

/*! \brief Print critical error message to primary log.
 *	\ingroup Globals
 *
 *	Insert critical error tag prior message in log.
 */
void gCError	( const char * fmt, ... );
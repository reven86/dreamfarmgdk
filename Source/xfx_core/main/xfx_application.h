//  File xfx_application.h                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_timer.h"

/*!	\file xfx_application.h 
 *	\brief Defines a base class for application.
 */


_XFX_BEGIN






/*! \class Application xfx_application.h "engine/xfx_application.h"
 *	\brief Defines a base class for application.
 *	\ingroup MainGroup
 *	
 *	%Application facade class that handles windows messages and makes common initialization 
 *	tasks such as InitEngine, InitRender, InitAudio
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Application
{
protected:

	//! Window handle.
	HWND						mWnd;

	//! Instance handle.
	HINSTANCE					mhInstance;

	//! Command line.
	String						mCommandLine;

	//! Window class name.
	WString						mWndClass;

	//! Application timer.
	Timer						mTimer;

protected:
	Application														( ) : mWnd( 0 ), mhInstance( 0 ) { };
	virtual ~Application											( ) { };

public:
	/*!	\name Initialization/Deinitialization.
	 *
	 *	@{
	 */

	/*!	\brief %Engine initialization.
	 *	
	 *	Creates a window and calls Engine::Initialize on that window.
	 *
	 *	\param[in] inst		HINSTANCE of execution process
	 *	\param[in] cmdline	Command line
	 *	\param[in] wndclass	Window Class of created window
	 *	\param[in] caption	Caption of created window
	 *
	 *	\return Returns S_OK if engine has been initialized sucessfully.
	 */
	virtual HRESULT					InitEngine						( HINSTANCE inst, const String& cmdline, const WString& wndclass, const WString& caption );

	/*! \brief Render initialization.
	 *	
	 *	Simply calls Render::CreateDevice.
	 *
	 *	\return Returns S_OK if device has been created sucessfully.
	 */
	virtual HRESULT					InitRenderer					( );

	/*! \brief Audio initialization.
	 *	
	 *	\note Not implemented yet.
	 *	\todo Implement audio initialization.
	 *
	 *	\return Returns S_OK anyway.
	 */
	virtual HRESULT					InitAudio						( );

	/*! \brief Deinitialization.
	 *	
	 *	Deinitialize Engine, Audio, Render.
	 *
	 *	\note Facade method. Simply calls Engine::Free, Audio::Free and unregisters window class.
	 */
	virtual void					Shutdown						( );

	//! @}

	/*! \brief Main loop method.
	 *	
	 *	This method updates scene then renders it and processes window messages.
	 */
	virtual void					MainLoop						( );

	//! %Render the scene.
	virtual void					Render							( ) const { };

	//! %Update scene.
	virtual void					Update							( ) { };

	//! Retrieves an interface pointer.
	virtual class UIInterface *		pInterface						( ) { return NULL; };

	//! Windows message handler.
	virtual LRESULT CALLBACK		WndProc							( HWND hWnd, UINT msp, WPARAM wParam, LPARAM lParam );

	const HWND&						hWnd							( ) const { return mWnd; };
	const HINSTANCE&				hInstance						( ) const { return mhInstance; };
	const String&					CommandLine						( ) const { return mCommandLine; };
	const WString&					WndClass						( ) const { return mWndClass; };

	const Timer&					GetTimer						( ) const { return mTimer; };

private:
	static LRESULT CALLBACK			sWndProc						(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};



/*! \defgroup Globals Global functions and variables.
 */

//! @{
/*! \brief Get the application class.
 *
 *	Returns application class derived from Application.
 *	\note Library user should define its own implementation of this function
 */
extern Application& gGetApplication		();
//! @}





_XFX_END
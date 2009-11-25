//  File xfx_gui.h                                                            |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_gui.h 
 *	\brief Adapter for CEGUI library.
 */

_XFX_BEGIN





/*!	\defgroup GUIGroup Graphics user interface classes.
 */

/*! \class GUI xfx_gui.h "gui/xfx_gui.h"
 *	\brief Adapter for CEGUI library.
 *	\ingroup GUIGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class GUI : boost::noncopyable
{
	//! Pointer to renderer.
	boost::scoped_ptr< class CEGUIRenderer >			mRendererPtr;

	//! Pointer to CEGUI system object.
	boost::scoped_ptr< CEGUI::System >					mSystemPtr;

	//! Pointer to resource provider.
	boost::scoped_ptr< class CEGUIResourceProvider >	mResourceProviderPtr;

	//! Pointer to scripting module.
	boost::scoped_ptr< CEGUI::LuaScriptModule >			mScriptModulePtr;

public:
	GUI															( );
	~GUI														( );


	//
	// System methods
	//

	/*! \brief Initialize GUI system.
	 */
	HRESULT							Init						( );

	/*! \brief Shutdown GUI system.
	 */
	void							Shutdown					( );

	/*! \brief Handle window messages.
	 *
	 *	Handle window messages and inject input into CEGUI.
	 */
	bool							ProcessWndMessage			( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );


	//
	// Accessors
	//

	//! Get renderer (const version).
	const boost::scoped_ptr< CEGUIRenderer >&	GetRendererPtr	( ) const { return mRendererPtr; };

	//! Get renderer (non-const version).
	CEGUIRenderer *								GetRendererPtr	( ) { return mRendererPtr.get( ); };

	//! Get resource provider (const version).
	const boost::scoped_ptr< CEGUIResourceProvider >& GetResourceProviderPtr ( ) const { return mResourceProviderPtr; };

	//! Get resource provider (nonconst version).
	CEGUIResourceProvider *						GetResourceProviderPtr ( ) { return mResourceProviderPtr.get( ); };

	//! Get script module (const version).
	const boost::scoped_ptr< CEGUI::LuaScriptModule >& GetScriptModulePtr( ) const { return mScriptModulePtr; };

	//! Get script module (nonconst version).
	CEGUI::LuaScriptModule *					GetScriptModulePtr( ) { return mScriptModulePtr.get( ); };



	//
	// Misc
	//

	//! Render GUI.
	void							Render						( ) const;

	//! Update GUI.
	void							Update						( DWORD mspf );
};



_XFX_END
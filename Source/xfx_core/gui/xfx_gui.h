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





/*!	\defgroup GUIGroup Graphic user interface classes.
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
	boost::scoped_ptr< class CEGUIRenderer >		mRendererPtr;

	//! Pointer to CEGUI system object.
	boost::scoped_ptr< CEGUI::System >				mSystemPtr;

public:
	GUI															( );
	~GUI														( );

	/*! \brief Initialize GUI system.
	 */
	HRESULT							Init						( );

	/*! \brief Shutdown GUI system.
	 */
	void							Shutdown					( );

	//! Render GUI.
	void							Render						( ) const;
};



_XFX_END
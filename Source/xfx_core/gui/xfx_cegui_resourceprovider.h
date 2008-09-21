//  File xfx_cegui_resourceprovider.h                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_cegui_resourceprovider.h 
 *	\brief ResourceProvider implementation for CEGUI library.
 */

#include <ceguidefaultresourceprovider.h>

_XFX_BEGIN







/*! \class CEGUIResourceProvider xfx_cegui_resourceprovider.h "gui/xfx_cegui_resourceprovider.h"
 *	\brief ResourceProvider for CEGUI library.
 *	\ingroup GUIGroup
 *
 *	This resource provider can read resources in pack files.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class CEGUIResourceProvider : public CEGUI::DefaultResourceProvider, boost::noncopyable
{
public:
	CEGUIResourceProvider											( );
	virtual ~CEGUIResourceProvider									( );

	//
	// CEGUI interface
	//

    /*!
    \brief
        Load raw binary data.

    \param filename
        String containing a filename of the resource to be loaded.

	\param output
        Reference to a RawDataContainer object to load the data into.

    \param resourceGroup
        Optional String that may be used by implementations to identify the group from
        which the resource should be loaded.
    */
	virtual void				loadRawDataContainer				( const CEGUI::String& filename, CEGUI::RawDataContainer& output, const CEGUI::String& resourceGroup );

	using DefaultResourceProvider::getFinalFilename;
};



_XFX_END
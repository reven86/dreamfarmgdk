//  File xfx_cegui_resourceprovider.cpp                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_cegui_resourceprovider.h"

_XFX_BEGIN





//
// CEGUIResourceProvider
//

CEGUIResourceProvider::CEGUIResourceProvider( )
{
}

CEGUIResourceProvider::~CEGUIResourceProvider( )
{
}

void CEGUIResourceProvider::loadRawDataContainer( 
	const CEGUI::String& filename, 
	CEGUI::RawDataContainer& output, 
	const CEGUI::String& resourceGroup
	)
{
	CEGUI::String final_filename( getFinalFilename( filename, resourceGroup ) );

	unsigned long filesize;
	HRESULT hr;
	if( FAILED( hr = FileSystem::Instance( ).GetFileSize( final_filename.c_str( ), filesize ) ) )
	{
		CEGUI::String sMsg("CEGUIResourceProvider::loadRawDataContainer - Filename supplied for loading must be valid");
		sMsg+=" ["+final_filename+"]";
		throw CEGUI::InvalidRequestException(sMsg);
	}
	else
	{
		BYTE * pbuf = new BYTE[ filesize ];

		FileSystem::Instance( ).ReadFile( final_filename.c_str( ), pbuf );

		output.setData(pbuf);
		output.setSize(filesize);
	}
}




_XFX_END
//  File xfx_cegui_texture.h                                                  |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_cegui_texture.h 
 *	\brief Texture implementation for CEGUI library.
 */

_XFX_BEGIN




 /*! \class CEGUITexture xfx_cegui_texture.h "gui/xfx_cegui_texture.h"
 *	\brief Texture interface for CEGUI library.
 *	\ingroup GUIGroup
 *
 *	Textures are always created with a size that is a power of 2. If you specify a size 
 *	that is not a power of two, the final size will be rounded up. So if you specify a 
 *	size of 1024, the texture will be (1024 x 1024), however, if you specify a size of 
 *	1025, the texture will be (2048 x 2048). You can check the ultimate size by querying 
 *	the texture after creation.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class CEGUITexture : public CEGUI::Texture
{
	boost::shared_ptr< const Shader >				mShaderPtr;
	boost::shared_ptr< ShaderParams >				mShaderParamsPtr;

public:
	CEGUITexture													( class CEGUIRenderer * renderer );
	virtual ~CEGUITexture											( );

	//
	// Own methods
	//

	//! Rescale texture to specific size.
	void								SetSize						( unsigned new_size );

	//! Return shader associated with texture.
	const boost::shared_ptr< const Shader >& GetShaderPtr			( ) const { return mShaderPtr; };

	//! Return shader params (non-const version).
	boost::shared_ptr< ShaderParams >	GetShaderParamsPtr			( ) { return mShaderParamsPtr; };

	//! Return shader params (const version).
	const boost::shared_ptr< ShaderParams >& GetShaderParamsPtr		( ) const { return mShaderParamsPtr; };

	//
	// CEGUI interface
	//

	/*!
	\brief
		Returns the current pixel width of the texture

	\return
		ushort value that is the current width of the texture in pixels
	*/
	virtual	CEGUI::ushort				getWidth					( void ) const;

	/*!
	\brief
		Returns the current pixel height of the texture

	\return
		ushort value that is the current height of the texture in pixels
	*/
	virtual	CEGUI::ushort				getHeight					( void ) const;

	/*!
	\brief
		Loads the specified image file into the texture.  The texture is resized as required to hold the image.

	\param filename
		The filename of the image file that is to be loaded into the texture

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when loading the image file.

	\return
		Nothing.
	*/
	virtual void						loadFromFile				( const CEGUI::String& filename, const CEGUI::String& resourceGroup );

	/*!
	\brief
		Loads (copies) an image in memory into the texture.  The texture is resized as required to hold the image.

	\param buffPtr
		Pointer to the buffer containing the image data

	\param buffWidth
		Width of the buffer (in pixels as specified by \a pixelFormat )

	\param buffHeight
		Height of the buffer (in pixels as specified by \a pixelFormat )

    \param pixelFormat
        PixelFormat value describing the format contained in \a buffPtr

	\return
		Nothing.
	*/
	virtual void						loadFromMemory				( const void* buffPtr, CEGUI::uint buffWidth, CEGUI::uint buffHeight, PixelFormat pixelFormat );
};



_XFX_END
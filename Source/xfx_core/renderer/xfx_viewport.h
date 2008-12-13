//  File xfx_viewport.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_viewport.h 
 *	\brief %Viewport class.
 */

_XFX_BEGIN






/*! \class Viewport xfx_viewport.h "render/xfx_viewport.h"
 *	\brief %Viewport class.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Viewport : public Singleton< Viewport >
{
	friend class Singleton< Viewport >;

	unsigned												mWidth;
	unsigned												mHeight;
	float													mInvWidth;
	float													mInvHeight;

	//! Viewport update event subscribers.
	boost::signal0< void >									mViewportUpdateSignals;

protected:
	Viewport													( );
	virtual ~Viewport											( );

public:
	//! Initialize viewport with width and height.
	void							Init						( unsigned width, unsigned height );

	//! Get viewport width.
	const unsigned&					Width						( ) const { return mWidth; };

	//! Get viewport height.
	const unsigned&					Height						( ) const { return mHeight; };

	//! Get viewport 1.0 / width.
	const float&					InvWidth					( ) const { return mInvWidth; };

	//! Get viewport 1.0 / height.
	const float&					InvHeight					( ) const { return mInvHeight; };

	//! Set viewport width.
	void							Width						( unsigned width );

	//! Set viewport height.
	void							Height						( unsigned height );

	//! Viewport to render surface coordinate mapping.
	boost::tuple< float, float >	MapViewportToRenderer		( float x, float y ) const;

	//! Render surface to viewport coordinate mapping.
	boost::tuple< float, float >	MapRendererToViewport		( float x, float y ) const;

	//! Add viewport update signal subscriber.
	template< class _Type >
	boost::signals::connection		AddOnViewportUpdateSubscriber( const _Type& fn ) { return mViewportUpdateSignals.connect( fn ); };

	//! %Render viewport. Calls Render::BeginFrame, Application::Render, Render::EndFrame and do some related tasks.
	void							Render						( ) const;

	/*! \brief Project 3D coordinates to 2D screen coordinates.
	 *
	 *	Gets WorldViewProjection matrix and transform incoming vector p.
	 *
	 *	\param[out]	x	Screen X coordinate.
	 *	\param[out]	y	Screen Y coordinate.
	 *	\param[in]	p	Incoming point.
	 */
	void							Project						( float& x, float& y, const Vec3& p ) const;

	/*! \brief Unproject 2D screen coordinates to 3D coordinates.
	 *
	 *	Gets WorldViewProjection matrix, inverse it and transform incoming screen vector ( x, y, 1 ).
	 *
	 *	\param[out]	out	Output vector.
	 *	\param[in]	x	Screen X coordinate.
	 *	\param[in]	y	Screen Y coordinate.
	 */
	void							Unproject					( class Vec3& out, float x, float y ) const;
};



_XFX_END
//  File xfx_uiscroll.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_uiobject.h"

/*!	\file xfx_uiscroll.h 
 *	\brief UIScroll base class, and hscroll/vscroll user controls. 
 */

_XFX_BEGIN






/*! \class UIScroll xfx_uiscroll.h "ui/xfx_uiscroll.h"
 *	\brief UI scroll base class.
 *	\ingroup InterfaceGroup
 *
 *	%UIScroll file syntax can be found at section \ref uiscroll_script.
 *
 *	Scroll position is limited in range from 0 to 1.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uiscroll_script UIScroll file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	< UIObject properties >

	scroll.arrow1_shader		"textures\interface\scroll_arrow_left.shader"
	scroll.arrow2_shader		"textures\interface\scroll_arrow_right.shader"
	scroll.fill_shader			"textures\interface\scroll_fill_shader.shader"
	scroll.pos_shader			"textures\interface\scroll_pos_shader.shader"
	scroll.arrow_step			0.2
	scroll.position				0.5
}
	\endverbatim
 *	
 *	Description:
 *
 *	\note Keyword \a scroll is omitted.
 *
 *	\b arrow1_shader		-	First arrow shader file name. First arrow is a left (for horizontal scroll) or top (for vertical scroll).\n
 *	\b arrow2_shader		-	Second arrow shader file name. Second arrow is a right (for horizontal scroll) or bottom (for vertical scroll).\n
 *	\b fill_shader			-	Center part shader file name. This shader is repeated from first to second arrow.\n
 *	\b pos_shader			-	Scroll position shader file name.\n
 *	\b arrow_step			-	Step by which scroll position will be adjusted when user click on arrow. Default: 0.1.\n
 *	\b position				-	Start scroll position. \n
 *	
 *
 *	\see UIObject, UIScroll, UIHScroll, Shader
 *
 *	@}
 */

class UIScroll : public UIObject
{
	friend class boost::serialization::access;

public:
	//! Scrolls shaders enumeration.
	enum EShaderType
	{
		EST_FIRST_ARROW,			//!< Shader for first arrow.
		EST_SECOND_ARROW,			//!< Shader for second arrow.
		EST_FILL,					//!< Fill shader.
		EST_SCROLL,					//!< Scroll position shader.

		EST_MAX_SHADERS,			//!< Max shader types.
	};

protected:
	//! Shaders collection. Stores shader and shader texture size in texels for 
	//each dimension (horizontal and vertical). This is needed to properly define
	//rendering offsets for arrows and scroll position sprites.
	boost::array< boost::tuple< 
		boost::shared_ptr< const Shader >, xfx::Vec2 >, EST_MAX_SHADERS >	mShaders;

	//! Current scroll position.
	float																	mScrollPos;

	//! Arrow step.
	float																	mArrowStep;

	//! Internal flags.
	enum EScrollFlags
	{
		ESF_PUSH_ARROW1			= ( 1 << 0 ),
		ESF_PUSH_ARROW2			= ( 1 << 1 ),
	};

	//! Scroll flags.
	DWORD																	mScrollFlags;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UIScroll															( );
	virtual ~UIScroll													( );

	//! Get shader.
	const boost::shared_ptr< const Shader >&	GetScrollShader			( const EShaderType shd_type ) const;

	//! Set shader.
	void										SetScrollShader			( const EShaderType shd_type, const boost::shared_ptr< const Shader >& shd );

	//! Get scroll position.
	const float&								GetScrollPosition		( ) const { return mScrollPos; };

	//! Set scroll position.
	void										SetScrollPosition		( float pos );

	//! Get scroll step.
	const float&								GetScrollStep			( ) const { return mArrowStep; };

	//! Set scroll step.
	void										SetScrollStep			( const float& step ) { mArrowStep = step; };

	//! Update scroll.
	virtual void								Update					( DWORD mspf );

	//! Process mouse window messages. \note Calls lua events.
	virtual bool								ProcessMouse			( float x, float y, UINT mousemsg, WPARAM wParam );

	/*! \brief Reset control state.
	 *
	 *	Function that reset object state (ex. when cursor moved out of window all child
	 *	objects must be reseted.
	 */
	virtual void								ResetState				( );

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void									LuaRegister				( lua_State * L );

protected:
	//! Prepare scroll to render.
	virtual void								PrepareScrollToRender	( );

	//! Check if mouse is on first arrow.
	virtual bool								CheckOnFirstArrow		( float x, float y ) const = 0;

	//! Check if mouse is on second arrow.
	virtual bool								CheckOnSecondArrow		( float x, float y ) const = 0;

	//! Get a scroll position under mouse.
	virtual float								GetScrollPosFromXY		( float x, float y ) const = 0;

	virtual HRESULT								PrepareParsing			( );
	virtual HRESULT								AfterParsing			( );

	HRESULT										ParseShader				( const EShaderType& type, String::size_type& pos, const String& str );

	//! Get lua object for this class.
	virtual luabind::object						GetLuaObject			( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };
};





/*! \class UIHScroll xfx_uiscroll.h "ui/xfx_uiscroll.h"
 *	\brief Horizontal scroll control.
 *	\ingroup InterfaceGroup
 *
 *	%UIHScroll file syntax can be found at section \ref uiscroll_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class UIHScroll : public UIScroll
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UIHScroll															( );
	virtual ~UIHScroll													( );

	/*! \brief %Render control.
	 *
	 *	\param[in]	color	Color modulator.
	 *	\param[in]	ofs		X/Y offset.
	 *	\param[in]	scale	X/Y scale.
	 */
	virtual void							Render						( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const;	

protected:
	//! Check if mouse is on first arrow.
	virtual bool							CheckOnFirstArrow			( float x, float y ) const;

	//! Check if mouse is on second arrow.
	virtual bool							CheckOnSecondArrow			( float x, float y ) const;

	//! Get a scroll position under mouse.
	virtual float							GetScrollPosFromXY			( float x, float y ) const;
};







_XFX_END

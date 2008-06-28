//  File xfx_uilabel.h                                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_uiobject.h"
#include "renderer\xfx_shader.h"
#include "renderer\xfx_font.h"


/*!	\file xfx_uilabel.h 
 *	\brief Label user control.
 */

_XFX_BEGIN






/*! \class UILabel xfx_uilabel.h "ui/xfx_uilabel.h"
 *	\brief Label user control.
 *	\ingroup InterfaceGroup
 *
 *	%UILabel file syntax can be found at section \ref uilabel_script.
 *
 *	\todo Make color commands in text.
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uilabel_script UILabel file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	< UIObject properties >

	label.font			"font\system.font"
	label.text			"test text"
	label.max_width		50
	label.real_size		1
	label.x_scale		0.5
	label.y_scale		0.5
	label.align			center
}
	\endverbatim
 *	
 *	Description:
 *
 *	\note Keyword \a label is omitted.
 *
 *	\b font			-	Font file name. \n
 *	\b text			-	Text in label control. \n
 *	\b max_width	-	Max width of one string in label control (could be -1, if no formating required).
 *	\b real_size	-	Text size will be drawn in one to one dimensions, not depend on current Viewport size.
 *	\b x_scale		-	X scale for font characters.
 *	\b y_scale		-	Y scale for font characters.
 *	\b align		-	Align mode. How text position would be changed after text changing. Supported modes: left (default), center, right, width.
 *
 *	\see UIObject, UILabel, Font
 *
 *	@}
 */

#pragma message( "FIXME: Make color commands in text" )

class UILabel : public UIObject
{
	friend class boost::serialization::access;

public:
	enum AlignMode
	{
		EAM_LEFT,
		EAM_CENTER,
		EAM_RIGHT,
		EAM_WIDTH,

		EAM_TOTAL_MODES
	};

private:
	struct SpriteInfo
	{
		float x, y;
		float w, h;
		float u1, v1;
		float u2, v2;
	};

	boost::shared_ptr< const class Font >			mFontPtr;
	boost::shared_ptr< const class ShaderConsts >	mShaderConstsPtr;

	WString										mCurText;
	std::vector< SpriteInfo >					mCachedTris;
	bool										mIsTrisCached;
	bool										mIsRealSize;

	float										mLx, mLy;
	float										mMaxWidth;
	float										mXScale;
	float										mYScale;
	AlignMode									mAlign;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UILabel																( );
	virtual ~UILabel													( );

	//! Get font.
	const boost::shared_ptr< const Font >& FontPtr						( ) const { return mFontPtr; };

	//! Set font.
	void								FontPtr							( const boost::shared_ptr< const Font >& fnt ) { mFontPtr = fnt; mIsTrisCached = false; CacheTris( );};

	//! Get shader consts for font shader.
	const boost::shared_ptr< const ShaderConsts >& ShaderConstsPtr		( ) const { return mShaderConstsPtr; };

	//! Set shader consts for font shader.
	void								ShaderConstsPtr					( const boost::shared_ptr< const ShaderConsts >& consts ) { mShaderConstsPtr = consts; };

	using UIObject::X;
	using UIObject::Y;
	using UIObject::Width;
	using UIObject::Height;
	virtual void						X								( float x ) { UIObject::X( x ); mLx = x - UIObject::Width( ); };
	virtual void						Y								( float y ) { UIObject::Y( y ); mLy = y + UIObject::Height( ); };
	virtual	void						Width							( float ) { };
	virtual void						Height							( float ) { };

	//! Get x scale factor.
	const float&						XScale							( ) const { return mXScale; };

	//! Set x scale factor.
	void								XScale							( float x ) { mXScale = x; mIsTrisCached = false; CacheTris( ); };

	//! Get y scale factor.
	const float&						YScale							( ) const { return mYScale; };

	//! Set y scale factor.
	void								YScale							( float y ) { mYScale = y; mIsTrisCached = false; CacheTris( ); };

	//! Get left text position.
	const float&						PosX							( ) const { return mLx; };

	//! Get bottom text position.
	const float&						PosY							( ) const { return mLy; };

	//! Set left text position.
	void								PosX							( float lx ) { mLx = lx; UIObject::X( mLx + UIObject::Width( ) ); };

	//! Set bottom text position.
	void								PosY							( float ly ) { mLy = ly; UIObject::Y( mLy - UIObject::Height( ) ); };

	//! Get text.
	const WString&						Text							( ) const { return mCurText; };

	//! Set text.
	void								Text							( const WString& str );

	//! Get align mode.
	const enum AlignMode&				AlignMode						( ) const { return mAlign; };

	//! Set align mode.
	void								AlignMode						( const enum AlignMode& mode ) { mAlign = mode; };

	//! Get real size flag.
	const bool&							IsRealSize						( ) const { return mIsRealSize; };

	//! Set real size flag.
	void								IsRealSize						( bool fl ) { mIsRealSize = fl; };

	/*! \brief %Render control.
	 *
	 *	\param[in]	color	Color modulator.
	 *	\param[in]	ofs		X/Y offset.
	 *	\param[in]	scale	X/Y scale.
	 */
	virtual void						Render							( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const;

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void							LuaRegister						( struct lua_State * L );

private:
	void								CacheTris						( );

protected:
	virtual HRESULT						PrepareParsing					( );
	virtual HRESULT						AfterParsing					( );

	//! Get lua object for this class.
	virtual luabind::object				GetLuaObject					( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };

private:
	HRESULT								ParseFont						( String::size_type& pos, const String& str );
	HRESULT								ParseText						( String::size_type& pos, const String& str );
	HRESULT								ParseAlign						( String::size_type& pos, const String& str );
};




_XFX_END

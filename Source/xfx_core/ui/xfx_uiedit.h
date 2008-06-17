//  File xfx_uiedit.h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_uilabel.h"

/*!	\file xfx_uiedit.h 
 *	\brief Edit user control.
 */

_XFX_BEGIN






#pragma message( "FIXME: allow multiline edit" )

/*! \class UIEdit xfx_uiedit.h "ui/xfx_uiedit.h"
 *	\brief Edit user control.
 *	\ingroup InterfaceGroup
 *
 *	One line edit control. Support:
 *	- Insertion/Deletion chars at the end of the text.
 *	- Clipboard operation: Copy, Paste, Cut.
 *
 *	%UIEdit file syntax can be found at section \ref uiedit_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uiedit_script UIEdit file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	< UIObject properties >

	edit.font		"font\system.font"
	edit.text		"test text"
	edit.real_size	1
	edit.x_scale	0.5
	edit.y_scale	0.5
	edit.max_chars	100
}
	\endverbatim
 *	
 *	Description:
 *
 *	\note Keyword \a edit is omitted.
 *
 *	\b font			-	Font file name. \n
 *	\b text			-	Text in edit control.
 *	\b real_size	-	Text size will be drawn in one to one dimensions, not depend on current Viewport size.
 *	\b x_scale		-	X scale for font characters.
 *	\b y_scale		-	Y scale for font characters.
 *	\b max_chars	-	Max characters count. Default 65535.
 *
 *	\see UIObject, UIEdit, Font
 *
 *	@}
 */

class UIEdit : public UIObject
{
	friend class boost::serialization::access;

	// text not prepared, UILabel stores prepared text
	WString								mText;

	UILabel								mLabel;
	UILabel								mCursor;
	DWORD								mMaxChars;

	DWORD								mLocalTime;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UIEdit																	( );
	virtual ~UIEdit															( );

	//! Get font.
	const boost::shared_ptr< const Font >& FontPtr							( ) const { return mLabel.FontPtr( ); };

	//! Set font.
	void								FontPtr								( const boost::shared_ptr< const Font >& fnt ) { mLabel.FontPtr( fnt ); mCursor.FontPtr( fnt ); };

	//! Get center X coordinate.
	const float&						X									( ) const { return UIObject::X( ); };

	//! Get center Y coordinate.
	const float&						Y									( ) const { return UIObject::Y( ); };

	//! Get half width.
	const float&						Width								( ) const { return UIObject::Width( ); };

	//! Get half height.
	const float&						Height								( ) const { return UIObject::Height( ); };
	
	//! Set center X coordinate.
	virtual void						X									( float x );

	//! Get center Y coordinate.
	virtual void						Y									( float y );

	//! Get half width.
	virtual void						Width								( float ) { };

	//! Get half height. 
	virtual void						Height								( float ) { };

	//! Get left text position.
	const float&						PosX								( ) const { return mLabel.PosX( ); };

	//! Get bottom text position.
	const float&						PosY								( ) const { return mLabel.PosY( ); };

	//! Set left text position.
	void								PosX								( float lx );

	//! Set bottom text position.
	void								PosY								( float ly );

	//! Get x scale factor.
	const float&						XScale								( ) const { return mLabel.XScale( ); };

	//! Set x scale factor.
	void								XScale								( float x ) { mLabel.XScale( x ); mCursor.XScale( x ); };

	//! Get y scale factor.
	const float&						YScale								( ) const { return mLabel.YScale( ); };

	//! Set y scale factor.
	void								YScale								( float y ) { mLabel.YScale( y ); mCursor.YScale( y ); };

	//! Get max characters count.
	const DWORD&						MaxChars							( ) const { return mMaxChars; };

	//! Set max characters count.
	void								MaxChars							( DWORD count );

	/*! \brief %Render control.
	 *
	 *	\param[in]	color	Color modulator.
	 *	\param[in]	ofs		X/Y offset.
	 *	\param[in]	scale	X/Y scale.
	 */
	virtual void						Render								( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const;

	//! %Update edit.
	virtual void						Update								( DWORD mspf );

	//! Get edited text.
	const WString&						Text								( ) const { return mText; };

	//! Set text to edit.
	void								Text								( const WString& str );

	//! Get real size flag.
	const bool&							IsRealSize							( ) const { return mLabel.IsRealSize( ); };

	//! Set real size flag.
	void								IsRealSize							( bool fl ) { mLabel.IsRealSize( fl ); mCursor.IsRealSize( fl ); };

	//! Process pushed keys.
	virtual void						ProcessKey							( wchar_t key );

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void							LuaRegister							( struct lua_State * L );

protected:
	//! Set focus callback.
	virtual void						OnSetFocus							( );

	//! Kill focus callback.
	virtual void						OnKillFocus							( );

	//! Get lua object for this class.
#ifdef __XFX_USE_LUA_LUABIND__
	virtual luabind::object				GetLuaObject						( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };
#endif

private:
	void								PrepareText							( );

	virtual HRESULT						AfterParsing						( );

	HRESULT								ParseFont							( String::size_type& pos, const String& str );
	HRESULT								ParseText							( String::size_type& pos, const String& str );
	HRESULT								ParseRealSize						( String::size_type& pos, const String& str );
	HRESULT								ParseXScale							( String::size_type& pos, const String& str );
	HRESULT								ParseYScale							( String::size_type& pos, const String& str );
};



_XFX_END

//  File xfx_uiconsole.h                                                      |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_uiobject.h"
#include "xfx_uilabel.h"
#include "xfx_uisprite.h"
#include "xfx_uiedit.h"
#include "main\xfx_console.h"

/*!	\file xfx_uiconsole.h 
 *	\brief Defines standard console class.
 */

_XFX_BEGIN





/*!	\class UIStandardConsole xfx_uiconsole.h "ui/xfx_uiconsole.h"
 *	\brief Basic standard console.
 *	\ingroup InterfaceGroup
 *
 *	Standard moved up-down console control.
 *
 *	%Console file syntax can be found here: \ref uistandardconsole_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uistandardconsole_script UIStandardConsole file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	< UIObject properties >

	standard_console.fade_distance		0.5
	standard_console.fade_time			2500
	standard_console.background_shader	"interface\test.shader"		0xffffffff
	standard_console.font				"font\system.font"			0xffffffff
}
	\endverbatim
 *	
 *	Description:
 *
 *	\note Keyword \a standard_console is omitted.
 *
 *	\b fade_distance		-	Relative fade distance (1.0 - fullscreen). \n
 *	\b fade_time			-	Fade time in 100 microseconds. \n
 *	\b background_shader	-	Background shader and background shader modulator. \n
 *	\b font					-	Font file name and font color modulator.
 *	\b miniconsole_lines	-	Miniconsole lines number. Default is 3.
 *
 *	\see UIObject, UIStandardConsole, Shader, Font
 *
 *	@}
 */

class UIStandardConsole : public Console, public UIObject
{
	friend class boost::serialization::access;

	UISprite								mBackground;
	UILabel									mMessagesLabel;
	UILabel									mPromptLabel;
	UIEdit									mPromptEdit;
	UIEdit									mMiniConsoleLabel;

	boost::shared_ptr< UIEdit >				mPromptEditPtr;

	enum{ MAX_MESSAGES = 127 };

	boost::array< WString, MAX_MESSAGES >	mMessages;
	std::size_t								mMessagesCount;
	std::size_t								mMessagesStartIndex;

	//
	// Attributes
	//

	// Relative fade distance ( 0 - nothing, 1 - full screen )
	float									mFadeDistance;

	// Fade time in 100 microseconds
	DWORD									mFadeTime;

	//
	// States and behavior
	//

	enum StateType
	{
		ST_HIDE,							//!< Miniconsole visible.
		ST_MOVE_DOWN,
		ST_SHOW,
		ST_MOVE_UP,
	};

	StateType								mCurrentState;

	// Move time in 100 microseconds
	DWORD									mMoveTime;

	//! Time to hide miniconsole.
	int										mTimeToHideMiniConsole;

	//! Miniconsole lines number.
	int										mMiniConsoleLinesNumber;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UIStandardConsole											( );
	virtual ~UIStandardConsole									( );
	

	//
	// UIObject methods
	//

	//! Get center X coordinate.
	const float&				X								( ) const { return UIObject::X( ); };

	//! Get center Y coordinate.
	const float&				Y								( ) const { return UIObject::Y( ); };

	//! Get half width.
	const float&				Width							( ) const { return UIObject::Width( ); };

	//! Get half height.
	const float&				Height							( ) const { return UIObject::Height( ); };
	
	//! Set center X coordinate.
	virtual void				X								( float ) { };

	//! Set center Y coordinate.
	virtual void				Y								( float ) { };

	//! Set half width.
	virtual void				Width							( float ) { };

	//! Set half height.
	virtual void				Height							( float ) { };

	/*! \brief %Render control.
	 *
	 *	\param[in]	color	Color modulator.
	 *	\param[in]	ofs		X/Y offset.
	 *	\param[in]	scale	X/Y scale.
	 */
	virtual void				Render							( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const;

	//! %Update console control and set position to all child controls.
	virtual void				Update							( DWORD mspf );


	//
	// UIConsole methods
	//

	//! Clear console messages.
	virtual void				Clear							( );

	/*! \brief Print message to console.
	 *
	 *	\param[in]	type	Message type.
	 *	\param[in]	msg		Message to print.
	 */
	virtual void				Print							( const Log::EMessageType& type, const WString& msg );

	/*! \brief Show/hide console.
	 */
	virtual void				Toggle							( );

	//! Get 'shown' flag.
	virtual bool				IsVisible						( ) const { return mCurrentState == ST_SHOW; };

private:
	//! Set text on all labels.
	void						PrepareToRender					( );
	void						ChangeState						( const StateType& new_state );

	//! Text entered event.
	static void					OnTextEntered					( UIEdit * ed );

protected:
	virtual HRESULT				PrepareParsing					( );
	virtual HRESULT				AfterParsing					( );

	HRESULT						ParseBackground					( String::size_type& pos, const String& str );
	HRESULT						ParseFont						( String::size_type& pos, const String& str );

	//! Get lua object for this class.
	virtual luabind::object		GetLuaObject					( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };
};





_XFX_END

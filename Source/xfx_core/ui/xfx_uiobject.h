//  File xfx_uiobject.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_uifocusable.h"

/*!	\file xfx_uiobject.h 
 *	\brief Base class for user controls.
 */

_XFX_BEGIN






/*! \class UIObject xfx_uiobject.h "ui/xfx_uiobject.h"
 *	\brief Base class for user controls.
 *	\ingroup InterfaceGroup
 *
 *	%UIObject base file syntax can be found here: \ref uiobject_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

#pragma message( "FIXME: document lua events." )

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uiobject_script UIObject base file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	base.name		"test_control"
	base.color		0xffffffff
	base.render		1
	base.active		1
	base.x			0
	base.y			0
	base.width		100
	base.height		100
	base.anchors		0 1 0 1

	< more control-specific properties >
}
	\endverbatim
 *	
 *	Description:
 *
 *	\b name			-	Control name. \n
 *	\b color		-	Color modulator. \n
 *	\b render		-	%Render flag. \n
 *	\b active		-	Active flag. \n
 *	\b x			-	X coordinate. \n
 *	\b y			-	Y coordinate. \n
 *	\b width		-	Half width. \n
 *	\b height		-	Half height. \n
 *	\b anchors		-	Control anchors. \n
 *
 *	\see UIObject
 *
 *	@}
 */

class UIObject : boost::noncopyable, public ScriptResource, public UIFocusable
{
	friend class UIComposite;
	friend class UIRepository;

	friend class boost::serialization::access;

public:
	//! Anchors type.
	enum AnchorsType
	{
		NONE	= 0x00000000,
		LEFT	= 0x00000001,
		TOP		= 0x00000002,
		RIGHT	= 0x00000004,
		BOTTOM	= 0x00000008,
		ALL		= LEFT | TOP | RIGHT | BOTTOM
	};

private:
	UIObject					* mpParent;
	UIObject					* mpNextInGroup;

	//Position in pixels ((0, 0) - screen center) and half diminsions in pixels
	float						mX, mY;
	float						mWidth, mHeight;

protected:
	enum FlagsType
	{
		FL_RENDER		= ( 1 << 0 ),
		FL_ACTIVE		= ( 1 << 1 ),

		FL_PREVINSIDE	= ( 1 << 2 ),
		FL_HASFOCUS		= ( 1 << 3 ),
		FL_NAMED		= ( 1 << 4 ),

		FL_FORCE_DWORD	= 0x7fffffff
	};

private:
	DWORD						mFlags;

	//Anchors
	float						mAnchorLeft;
	float						mAnchorTop;
	float						mAnchorRight;
	float						mAnchorBottom;

	DWORD						mAnchors;

	ARGB						mColor;
	String						mName;

	class UIRepository			* mpRepository;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

protected:
	//! Constructs an unnamed user control.
	UIObject														( );

public:
	virtual ~UIObject												( );

	//! Get parent for this object.
	const UIObject *					pParent						( ) const { return mpParent; };


	//
	// Iteration methods.
	//

	//! Get first child object.
	virtual const UIObject *			pFirstChild					( ) const { return NULL; };

	//! Get next object in group.
	const UIObject *					pNextInGroup				( ) const { return mpNextInGroup; };

	//
	// Accessors
	//

	//! Set user control name. \note Use it carefully and only for user-defined controls, not from repository.
	void								Name						( const String& name ) { mName = name; mFlags |= FL_NAMED; };

	//! Get user control name.
	const String&						Name						( ) const { return mName; };

	//! Get repository. \todo Think more about pointers.
	const UIRepository *				pRepository					( ) const { return mpRepository; };

	//! Get center X coordinate.
	const float&						X							( ) const { return mX; };

	//! Get center Y coordinate.
	const float&						Y							( ) const { return mY; };

	//! Get half width.
	const float&						Width						( ) const { return mWidth; };

	//! Get half height.
	const float&						Height						( ) const { return mHeight; };
	
	//! Set center X coordinate.
	virtual void						X							( float x );

	//! Set center Y coordinate.
	virtual void						Y							( float y );

	//! Set half width.
	virtual void						Width						( float width );

	//! Set half height.
	virtual void						Height						( float height );

	//! Get 'active' flag.
	bool								IsActive					( ) const { return ( mFlags & FL_ACTIVE ) != 0; };

	//! Get 'render' flag.
	bool								IsRender					( ) const { return ( mFlags & FL_RENDER ) != 0; };

	//! Set 'active' flag.
	void								IsActive					( bool isactive ) {if( IsActive( ) != isactive ) mFlags ^= FL_ACTIVE; };

	//! Set 'render' flag.
	void								IsRender					( bool isrender ) {if( IsRender( ) != isrender ) mFlags ^= FL_RENDER; };

	//! Get 'focus' flag.
	bool								HasFocus					( ) const { return ( mFlags & FL_HASFOCUS ) != 0; } ;

	//! Is named object.
	bool								IsNamed						( ) const { return ( mFlags & FL_NAMED ) != 0; };

	//! Get anchors.
	const DWORD&						Anchors						( ) const { return mAnchors; };

	//! Set anchors.
	void								Anchors						( const DWORD& anchors );

	//! Add new child.
	virtual void						AddUIObject					( UIObject * ) { _ASSERT (false); };

	//! Remove child.
	virtual void						RemoveUIObject				( UIObject * ) { _ASSERT (false); };

	//! Remove all children.
	virtual void						RemoveAllUIObjects			( ) { _ASSERTE (false); };

	//! Get children count.
	virtual unsigned					GetChildrenCount			( ) const { return 0; };

	//! Get color.
	const ARGB&							Color						( ) const { return mColor; };

	//! Set color.
	virtual void						Color						( const ARGB& col ) { mColor = col; };

	/*! \brief %Render control.
	 *
	 *	\param[in]	color	Color modulator.
	 *	\param[in]	ofs		X/Y offset.
	 *	\param[in]	scale	X/Y scale.
	 */
	virtual void						Render						( const ARGB& color, const Vec2& ofs, const Vec2& scale ) const = 0;

	//! %Update control.
	virtual void						Update						( DWORD ) { };

	//! Viewport update callback.
	virtual void						OnViewportUpdate			( );

	//! Process mouse window messages. \note Calls lua events.
	virtual bool						ProcessMouse				( float x, float y, UINT mousemsg, WPARAM wParam );

	//! Is point inside control.
	virtual bool						IsInside					( float x, float y ) const;

	/*! \brief Reset control state.
	 *
	 *	Function that reset object state (ex. when cursor moved out of window all child
	 *	objects must be reseted.
	 */
	virtual void						ResetState					( ) { mFlags &= ~FL_PREVINSIDE; };

	//! Process pushed keys.
	virtual void						ProcessKey					( wchar_t key );

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void							LuaRegister					( struct lua_State * L );

protected:
	//! Child update callback.
	virtual void						OnChildUpdate				( ) { };

	//! New parent callback.
	virtual void						OnNewParent					( UIObject * parent );

	//! Set focus callback.
	virtual void						OnSetFocus					( ) { mFlags |= FL_HASFOCUS; };

	//! Kill focus callback.
	virtual void						OnKillFocus					( ) { mFlags &= ~FL_HASFOCUS; };

	//! Get control flags.
	DWORD								GetFlags					( ) const { return mFlags; };

protected:
	HRESULT								ParseX						( String::size_type& pos, const String& str );
	HRESULT								ParseY						( String::size_type& pos, const String& str );
	HRESULT								ParseWidth					( String::size_type& pos, const String& str );
	HRESULT								ParseHeight					( String::size_type& pos, const String& str );
	HRESULT								ParseAnchors				( String::size_type& pos, const String& str );
	HRESULT								ParseColor					( String::size_type& pos, const String& str );
	HRESULT								ParseRender					( String::size_type& pos, const String& str );
	HRESULT								ParseActive					( String::size_type& pos, const String& str );

	virtual HRESULT						AfterParsing				( );

	//! Get lua object for this class.
#ifdef __XFX_USE_LUA_LUABIND__
	virtual luabind::object				GetLuaObject				( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };
#endif
};





_XFX_END

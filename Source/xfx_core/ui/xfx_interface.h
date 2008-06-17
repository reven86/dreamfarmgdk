//  File xfx_interface.h                                                      |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_interface.h 
 *	\brief %Interface base classes.
 *	\todo Implement interface callback in LUA.
 */

_XFX_BEGIN




/*! \defgroup InterfaceGroup Interface classes.
 */

/*! \class UIFactory xfx_interface.h "ui/xfx_interface.h"
 *	\brief Factory class, creates UIObject.
 *	\ingroup InterfaceGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class UIFactory : public Singleton< UIFactory >
{
	friend class Singleton< UIFactory >;

public:
	//! Factory functor.
	typedef boost::function0< boost::shared_ptr< class UIObject > >			CreateFnType;

private:
	typedef stdext::hash_map< String, CreateFnType, HashCompare< String > >	ClassesType;

	ClassesType							mClasses;

private:
	UIFactory															( );
	~UIFactory															( ) { };

public:
	/*! \brief Register new UI class.
	 *
	 *	Supported pre-defined classes:
	 *	- UISprite
	 *	- UILabel
	 *	- UIEdit
	 *	- UIStandardConsole
	 *	- UIComposite
	 *	- UIDummy
	 *	- UIProgress
	 *
	 *	\param[in]	type		Control keyword.
	 *	\param[in]	fn			Factory functor.
	 *
	 */
	void								RegisterClass					( const String& type, const CreateFnType& fn ) { mClasses.insert( ClassesType::value_type( type, fn ) ); };

	/*! \brief Unregister UI class.
	 *
	 *	Unregister previously register control.
	 *
	 *	\param[in]	type		Control keyword.
	 */
	void								UnregisterClass					( const String& type );

	/*! \brief Create UIObject by name.
	 *
	 *	Create new instance of user interface control by keyword.
	 *
	 *	\param[in]	type		Control keyword.
	 */
	boost::shared_ptr< UIObject >		CreateUIObject					( const String& type ) const;
};







/*! \class UIInterface xfx_interface.h "ui/xfx_interface.h"
 *	\brief User interface class. Holds a collection of UIObject.
 *	\ingroup InterfaceGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class UIInterface : boost::noncopyable
{
	boost::shared_ptr< UIObject >				mInterfaceObjectPtr;
	boost::shared_ptr< UIObject >				mMouseCursorPtr;

	boost::shared_ptr< class UIFocusable >		mFocusObjectPtr;

	// used during rendering
	mutable D3DRECT								mClipRect;

	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

	//! Viewport update event signals connection.
	boost::signals::connection					mViewportUpdateConnection;

public:
	//! Constructs empty user interface.
	UIInterface														( );

	//! Destructor.
	~UIInterface													( );

	//! Set clip region.
	void								ClipRect					( const D3DRECT& rect ) const;

	//! Get clip region.
	const D3DRECT&						ClipRect					( ) const { return mClipRect; };

	//! Get/Set primary interface object.
	boost::shared_ptr< UIObject >&		InterfaceObject				( ) { return mInterfaceObjectPtr; };

	//! Get primary interface object.
	const boost::shared_ptr< UIObject >& InterfaceObject			( ) const { return mInterfaceObjectPtr; };

	//! Get/Set mouse cursor interface object.
	boost::shared_ptr< UIObject >&		MouseCursor					( ) { return mMouseCursorPtr; };

	//! Get mouse cursor interface object.
	const boost::shared_ptr< UIObject >& MouseCursor				( ) const { return mMouseCursorPtr; };

	//! Set focus interface object.
	void								FocusObject					( const boost::shared_ptr< UIFocusable >& obj );

	//! Get focus interface object.
	const boost::shared_ptr< UIFocusable >& FocusObject				( ) const { return mFocusObjectPtr; };

	//! Process mouse window messages.
	bool								ProcessMouse				( float x, float y, UINT mousemsg, WPARAM wParam );

	//! Process pushed keys.
	void								ProcessKey					( wchar_t key );

	/*! \brief %Render interface.
	 *
	 *	\param[in]		ofs			Interface offset.
	 *	\param[in]		scale		Interface scale.
	 */
	void								Render						( const Vec2& ofs, const Vec2& scale ) const;

	//! Update interface state and all object.
	void								Update						( DWORD mspf );

	//! Viewport update callback.
	void								OnViewportUpdate			( );

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void							LuaRegister					( struct lua_State * L );

private:
	//! Lua wrapper for get focus object.
	UIObject *							Lua_GetFocusObject			( ) const;

	//! Set focus object.
	void								Lua_SetFocusObject			( UIObject * obj );
};



/*! \class UIRepository xfx_interface.h "ui/xfx_interface.h"
 *	\brief Collection of UIObject, loaded from file.
 *	\ingroup InterfaceGroup
 *
 *	%Interface file syntax can be found here: \ref uirepository_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uirepository_script Interface file syntax
 *
 *	Example:
 *
 *	\verbatim
interface
{
	width		800
	height		600
	background	0x00000000

	control sprite
	{...}

	< more controls here >
}
	\endverbatim
 *	
 *	Description:
 *
 *	\b width		-	%Interface width. \n
 *	\b height		-	%Interface height. \n
 *	\b background	-	%Interface background color.
 *
 *	\see UIInterface, UIRepository
 *
 *	@}
 */

class UIRepository : public ScriptResource
{
	friend class boost::serialization::access;

	typedef std::map< String, boost::shared_ptr< UIObject > >	ObjectsType;
	
	ObjectsType							mObjects;

	unsigned							mInterfaceWidth;
	unsigned							mInterfaceHeight;
	ARGB								mBackground;

	//
	//! Serialization
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version );

public:
	//! Constructs an empty repository.
	UIRepository													( );

	//! Destructor.
	virtual ~UIRepository											( );

	//! Add new UIObject to repository by unique name.
	void								AddUIObject					( const String& id, const boost::shared_ptr< UIObject >& obj );

	//! Find UIObject by name.
	const boost::shared_ptr< UIObject >& FindUIObject				( const String& id ) const;

	//! Get interface width.
	const unsigned&						InterfaceWidth				( ) const { return mInterfaceWidth; };

	//! Get interface height.
	const unsigned&						InterfaceHeight				( ) const { return mInterfaceHeight; };

	//! Get background color.
	const ARGB&							Background					( ) const { return mBackground; };

	//! Clear repository.
	void								Clear						( );

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void							LuaRegister					( struct lua_State * L );

protected:
	//! Prepare repository parsing. Clear repository.
	virtual HRESULT						PrepareParsing				( ) { Clear( ); return S_OK; };

	//! Try to parse one object.
	HRESULT								TryParseObject				( String::size_type& pos, const String& str );

	//! Lua FindUIObject alternative.
	UIObject *							Lua_FindUIObject			( const char * name ) const;
};




_XFX_END

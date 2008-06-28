//  File xfx_uicomposite.h                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#ifndef __XFX_UICOMPOSITE__
#define __XFX_UICOMPOSITE__

#include "xfx_uiobject.h"

/*!	\file xfx_uicomposite.h 
 *	\brief Defines composite control class.
 */

_XFX_BEGIN





/*!	\class UIComposite xfx_uicomposite.h "ui/xfx_uicomposite.h"
 *	\brief Composite control class.
 *	\ingroup InterfaceGroup
 *
 *	Composite control aggregates other ui controls and delegates messages to these controls.
 *
 *	%Composite control file syntax can be found here: \ref uicomposite_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uicomposite_script UIComposite file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	< UIObject properties >

	composite.child			<child control name>
}
	\endverbatim
 *	
 *	Description:
 *
 *	\note Keyword \a composite is omitted.
 *
 *	\b child		-	Child control name.
 *
 *	\see UIObject, UIComposite
 *
 *	@}
 */

class UIComposite : public UIObject
{
#ifdef __XFX_USE_BOOST_SERIALIZATION__
	friend class boost::serialization::access;
#endif

	typedef std::vector< UIObject * >						ObjectsType;

	ObjectsType												mObjects;

	Vec2													mChildrenScale;

#ifdef __XFX_USE_BOOST_SERIALIZATION__

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version )
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIObject );

		ar & BOOST_SERIALIZATION_NVP( mChildrenScale );

		if( _Archive::is_saving::value )
		{
			unsigned int count = mObjects.size( );
			ar & BOOST_SERIALIZATION_NVP( count );

			for( unsigned i = 0; i < count; i++ )
			{
				ar & boost::serialization::make_nvp( "object", mObjects[ i ] );
			}
		}
		else
		{
			mObjects.clear( );

			unsigned int count = 0;
			ar & BOOST_SERIALIZATION_NVP( count );

			mObjects.reserve( count );

			for( unsigned i = 0; i < count; i++ )
			{
				UIObject * obj;
				ar & boost::serialization::make_nvp( "object", obj );

				AddUIObject( obj );
			}
		}
	};

#endif

public:
	UIComposite													( );
	virtual ~UIComposite										( );
	

	//
	// UIObject methods
	//

	//! Get first child object. \todo Think more about pointers.
	virtual const UIObject *	pFirstChild						( ) const { return mObjects.empty( ) ? NULL : mObjects.front( ); };

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
	virtual void				Render							( const ARGB& color, const Vec2& ofs, const Vec2& scale ) const;

	//! %Update console control and set position to all child controls.
	virtual void				Update							( DWORD mspf );

	//! Add new child.
	virtual void				AddUIObject						( UIObject * obj );

	//! Remove child.
	virtual void				RemoveUIObject					( UIObject * obj );

	//! Remove all children.
	virtual void				RemoveAllUIObjects				( );

	//! Get children count.
	virtual unsigned			GetChildrenCount				( ) const { return static_cast< unsigned >( mObjects.size( ) ); };

	//! Viewport update callback.
	virtual void				OnViewportUpdate				( );

	//! Process mouse window messages.
	virtual bool				ProcessMouse					( float x, float y, UINT mousemsg, WPARAM wParam );

	/*! \brief Reset control state.
	 *	Function that reset object state (ex. when cursor moved out of window all child
	 *	objects must be reseted.
	 */
	virtual void				ResetState						( );


	//
	// UIComposite methods
	//

	//! Get children scale factor
	const Vec2&					ChildrenScale					( ) const { return mChildrenScale; };

	//! Set children scale factor.
	void						ChildrenScale					( const Vec2& scale ) { mChildrenScale = scale; };

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void					LuaRegister						( struct lua_State * L );

protected:
	//! Child update callback.
	virtual void				OnChildUpdate					( );

	virtual HRESULT				PrepareParsing					( );
	virtual HRESULT				AfterParsing					( );

	//! Get lua object for this class.
	virtual luabind::object		GetLuaObject					( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };

private:
	void						ComputePositionSize				( );

	HRESULT						ParseChild						( String::size_type& pos, const String& str );
};





_XFX_END



#ifdef __XFX_USE_BOOST_SERIALIZATION__
BOOST_CLASS_EXPORT( xfx::UIComposite );
#endif



#endif // __XFX_UICOMPOSITE__
//  File xfx_interface.cpp                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_interface.h"
#include "renderer\xfx_renderer.h"
#include "renderer\xfx_viewport.h"

#include "xfx_uiobject.h"
#include "xfx_uisprite.h"
#include "xfx_uilabel.h"
#include "xfx_uiedit.h"
#include "xfx_uiconsole.h"
#include "xfx_uicomposite.h"
#include "xfx_uidummy.h"
#include "xfx_uiprogress.h"
#include "xfx_uiscroll.h"

_XFX_BEGIN





//
// UIFactory
//

UIFactory::UIFactory ()
{
	RegisterClass( "sprite",		&SimpleFactoryFn< UIObject, UISprite > );
	RegisterClass( "label",			&SimpleFactoryFn< UIObject, UILabel > );
	RegisterClass( "edit",			&SimpleFactoryFn< UIObject, UIEdit > );
	RegisterClass( "console",		&SimpleFactoryFn< UIObject, UIStandardConsole > );
	RegisterClass( "composite",		&SimpleFactoryFn< UIObject, UIComposite > );
	RegisterClass( "dummy",			&SimpleFactoryFn< UIObject, UIDummy > );
	RegisterClass( "progress",		&SimpleFactoryFn< UIObject, UIProgress > );
	RegisterClass( "hscroll",		&SimpleFactoryFn< UIObject, UIHScroll > );
	//RegisterClass( "vscroll",		&SimpleFactoryFn< UIObject, UIVScroll > );

	/*
	//Register standard UIObjects
	RegisterClass ("composition",	SimpleFactoryFn<UIObject, UIComposition>);
	RegisterClass ("label",			SimpleFactoryFn<UIObject, UILabel>);
	RegisterClass ("button",		SimpleFactoryFn<UIObject, UIButton>);
	RegisterClass ("vscroll",		SimpleFactoryFn<UIObject, UIVScroll>);
	RegisterClass ("hscroll",		SimpleFactoryFn<UIObject, UIHScroll>);
	RegisterClass ("window",		SimpleFactoryFn<UIObject, UIWindow>);
	RegisterClass ("edit",			SimpleFactoryFn<UIObject, UIEdit>);
	RegisterClass ("list",			SimpleFactoryFn<UIObject, UIList>);
	RegisterClass ("gobject",		SimpleFactoryFn<UIObject, UIGObject>);
	RegisterClass ("progress",		SimpleFactoryFn<UIObject, UIProgress>);
	RegisterClass ("dummy",			SimpleFactoryFn<UIObject, UIDummy>);
	RegisterClass ("checkbox",		SimpleFactoryFn<UIObject, UICheckbox>);
	RegisterClass ("simple_edit",	SimpleFactoryFn<UIObject, UISimpleEdit>);
	RegisterClass ("label_list",	SimpleFactoryFn<UIObject, UILabelList>);
	*/
}

void UIFactory::UnregisterClass (const String& type)
{
	ClassesType::iterator it = mClasses.find (type);

	if (it != mClasses.end ())
		mClasses.erase (it);
}

boost::shared_ptr<UIObject> UIFactory::CreateUIObject( const String& type ) const
{
	ClassesType::const_iterator it = mClasses.find( type );

	if( it != mClasses.end( ) )
	{
		boost::shared_ptr< UIObject > obj = ( *it ).second( );

		_ASSERTE( obj );
		return obj;
	}

	_ASSERTE( false );
	return boost::shared_ptr< UIObject >( );
}





//
// UIInterface
//

UIInterface::UIInterface( )
{
	mViewportUpdateConnection = Viewport::Instance( ).AddOnViewportUpdateSubscriber( boost::bind( &UIInterface::OnViewportUpdate, this ) );
}

UIInterface::~UIInterface( )
{
	mViewportUpdateConnection.disconnect( );
}

void UIInterface::FocusObject( const boost::shared_ptr< UIFocusable >& obj )
{
	if( mFocusObjectPtr )
		mFocusObjectPtr->OnKillFocus( );

	mFocusObjectPtr = obj;

	if( mFocusObjectPtr )
		mFocusObjectPtr->OnSetFocus( );
}

bool UIInterface::ProcessMouse( float x, float y, UINT mousemsg, WPARAM wParam )
{
	PROFILE( __FUNCTION__, "Interface" );

	if( mMouseCursorPtr )
	{
		mMouseCursorPtr->X( x + mMouseCursorPtr->Width( ) );
		mMouseCursorPtr->Y( y - mMouseCursorPtr->Height( ) );
	}

	if( mInterfaceObjectPtr && mInterfaceObjectPtr->IsActive( ) )
		return mInterfaceObjectPtr->ProcessMouse( x, y, mousemsg, wParam );

	return false;
}

void UIInterface::ProcessKey( wchar_t key )
{
	if( mFocusObjectPtr )
	{
		mFocusObjectPtr->ProcessKey( key );
	}
}

void UIInterface::Render( const Vec2& ofs, const Vec2& scale ) const
{
	PROFILE( __FUNCTION__, "Interface" );

	D3DRECT rect;
	rect.x1		= 0;
	rect.y1		= 0;
	rect.x2		= Renderer::Instance( ).D3DPP( ).BackBufferWidth;
	rect.y2		= Renderer::Instance( ).D3DPP( ).BackBufferHeight;
	ClipRect( rect );

	bool iseqsize = (
		rect.x2 == static_cast< int >( Viewport::Instance( ).Width( ) ) && 
		rect.y2 == static_cast< int >( Viewport::Instance( ).Height( ) )
		);

	if( iseqsize )
	{
		Renderer::Instance( ).SamplerState( 0, Renderer::SM_SAMP_MAGFILTER, D3DTEXF_NONE );
		Renderer::Instance( ).SamplerState( 0, Renderer::SM_SAMP_MINFILTER, D3DTEXF_NONE );
	}

	if( mInterfaceObjectPtr )
		if( mInterfaceObjectPtr->IsRender( ) )
			mInterfaceObjectPtr->Render( 0xffffffff, ofs, scale );

	if( mMouseCursorPtr )
		if( mMouseCursorPtr->IsRender( ) )
			mMouseCursorPtr->Render( 0xffffffff, ofs, scale );

	//Renderer::Instance( ).DrawTools( ).FlushAll( );

	if( iseqsize )
	{
		Renderer::Instance( ).SamplerState( 0, Renderer::SM_SAMP_MAGFILTER, D3DTEXF_LINEAR );
		Renderer::Instance( ).SamplerState( 0, Renderer::SM_SAMP_MINFILTER, D3DTEXF_LINEAR );
	}
}

void UIInterface::Update( DWORD mspf )
{
	PROFILE( __FUNCTION__, "Interface" );

	if( mInterfaceObjectPtr )
		if( mInterfaceObjectPtr->IsRender( ) )
			mInterfaceObjectPtr->Update( mspf );

	if( mMouseCursorPtr )
		if( mMouseCursorPtr->IsRender( ) )
			mMouseCursorPtr->Update( mspf );
}

void UIInterface::OnViewportUpdate( )
{
	if( mInterfaceObjectPtr )
		mInterfaceObjectPtr->OnViewportUpdate( );
}

void UIInterface::ClipRect( const D3DRECT& rect ) const
{
	mClipRect = rect;

	//Renderer::Instance( ).DrawTools( ).FlushAll( );
}

UIObject * UIInterface::Lua_GetFocusObject( ) const
{
	return dynamic_cast< UIObject * >( FocusObject( ).get( ) );
}

void UIInterface::Lua_SetFocusObject( UIObject * obj )
{
	boost::shared_ptr< UIFocusable > s_obj;
	s_obj.reset( obj, NullDeleter< UIFocusable >( ) );

	FocusObject( s_obj );
}

void UIInterface::LuaRegister( lua_State * L )
{
	UIRepository::LuaRegister( L );

	UIObject::LuaRegister( L );
	UIEdit::LuaRegister( L );
	UILabel::LuaRegister( L );
	UIProgress::LuaRegister( L );
	UISprite::LuaRegister( L );
	UIComposite::LuaRegister( L );
	UIHScroll::LuaRegister( L );

	luabind::module( L )
	[
		luabind::class_< UIInterface >( "UIInterface" )
			.property( "focus_object",
				&UIInterface::Lua_GetFocusObject,
				&UIInterface::Lua_SetFocusObject
				)
	];
}




//
// UIRepository
//

UIRepository::UIRepository () : ScriptResource ("Interface")
{
	AddToken ("{",				&ParseUselessToken);
	AddToken ("interface",		&ParseUselessToken);
	AddToken ("width",			boost::bind (&ParseVariable<unsigned>, boost::ref (mInterfaceWidth), _1, _2));
	AddToken ("height",			boost::bind (&ParseVariable<unsigned>, boost::ref (mInterfaceHeight), _1, _2));
	AddToken ("background",		boost::bind (&ParseVariable<ARGB>, boost::ref (mBackground), _1, _2));
	AddToken ("control",		boost::bind (&UIRepository::TryParseObject, this, _1, _2));
	AddToken ("}",				boost::bind (&UIRepository::ParseEndToken, this, _1, _2));
}

UIRepository::~UIRepository ()
{
	Clear ();
}

void UIRepository::Clear ()
{
	struct null_rep : public std::unary_function<ObjectsType::value_type, void>
	{
		void operator ()			(ObjectsType::value_type& val) const
		{
			_ASSERTE (val.second);

			if (val.second)
				val.second->mpRepository = NULL;
		};
	};

	std::for_each (
		mObjects.begin (),
		mObjects.end (),
		null_rep ()
		);

	mObjects.clear ();
}

void UIRepository::AddUIObject (const String& id, const boost::shared_ptr<UIObject>& obj)
{
	mObjects.insert (ObjectsType::value_type (id, obj));

	gMess ("Control %s has been registered", id.c_str ());
}
 
const boost::shared_ptr<UIObject>& UIRepository::FindUIObject (const String& id) const
{
	ObjectsType::const_iterator it = mObjects.find (id);

	if (it == mObjects.end ())
	{
		if (!id.empty ())
			gWarn ("Control %s not found!", id.c_str ());

		static boost::shared_ptr<UIObject> res;

		return res;
	}

	return (*it).second;
}

UIObject * UIRepository::Lua_FindUIObject( const char * name ) const
{
	return FindUIObject( String( name ) ).get( );
}

HRESULT UIRepository::TryParseObject (String::size_type& pos, const String& str)
{
	boost::shared_ptr<UIObject> obj = UIFactory::Instance ().CreateUIObject (next_token (str, pos));
	if (obj)
	{
		HRESULT hr;
		obj->mpRepository = this;

		if (FAILED (hr = obj->ParseAt (pos, str)))
			return hr;

		AddUIObject (obj->Name (), obj);
		return S_OK;
	}
	
	return XFXERR_INVALIDCALL;
}

void UIRepository::LuaRegister( lua_State * L )
{
	luabind::module( L )
	[
		luabind::class_< UIRepository >( "UIRepository" )
			.def( "get_control", &UIRepository::Lua_FindUIObject )
	];
}



_XFX_END
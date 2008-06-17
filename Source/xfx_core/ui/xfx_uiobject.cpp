//  File xfx_uiobject.cpp                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_uiobject.h"
#include "xfx_interface.h"
#include "renderer\xfx_viewport.h"
#include "lua\xfx_luascript.h"

_XFX_BEGIN







//
// UIObject
//

UIObject::UIObject( ) : 
	ScriptResource( "UIObject" ),
	mName( "unnamed_uiobject" ),
	mpRepository( NULL ),
	mX( 0.0f ), mY( 0.0f ), mWidth( 0.0f ),
	mHeight( 0.0f ), mFlags( FL_RENDER | FL_ACTIVE ),
	mpParent( NULL ), mpNextInGroup( NULL ), mAnchors( 0 ),
	mColor( 0xffffffff ),
	mAnchorLeft( 0.0f ), mAnchorTop( 0.0f ), mAnchorRight( 0.0f ), mAnchorBottom( 0.0f )
{
	AddToken( "{",				&ParseUselessToken );
	AddToken( "base.name",		boost::bind( &ParseVariable< String >, boost::ref( mName ), _1, _2 ) );
	AddToken( "base.color",		boost::bind( &UIObject::ParseColor, this, _1, _2 ) );
	AddToken( "base.render",	boost::bind( &UIObject::ParseRender, this, _1, _2 ) );
	AddToken( "base.active",	boost::bind( &UIObject::ParseActive, this, _1, _2 ) );
	AddToken( "base.x",			boost::bind( &UIObject::ParseX, this, _1, _2 ) );
	AddToken( "base.y",			boost::bind( &UIObject::ParseY, this, _1, _2 ) );
	AddToken( "base.width",		boost::bind( &UIObject::ParseWidth, this, _1, _2 ) );
	AddToken( "base.height",	boost::bind( &UIObject::ParseHeight, this, _1, _2 ) );
	AddToken( "base.anchors",	boost::bind( &UIObject::ParseAnchors, this, _1, _2 ) );
	AddToken( "}",				boost::bind( &UIObject::ParseEndToken, this, _1, _2 ) );
}

UIObject::~UIObject ()
{
	if (mpParent)
		mpParent->RemoveUIObject (this);
}

void UIObject::X (float x)
{
	mX = x;

	if (mpParent)
		mpParent->OnChildUpdate ();

	if (mAnchors & LEFT)
		mAnchorLeft = Viewport::Instance ().Width () * 0.5f + X () - Width ();

	if (mAnchors & RIGHT)
		mAnchorRight = Viewport::Instance ().Width () * 0.5f - X () - Width ();
}

void UIObject::Y (float y)
{
	mY = y;

	if (mpParent)
		mpParent->OnChildUpdate ();

	if (mAnchors & TOP)
		mAnchorTop = Viewport::Instance ().Height () * 0.5f - Y () - Height ();

	if (mAnchors & BOTTOM)
		mAnchorBottom = Viewport::Instance ().Height () * 0.5f + Y () - Height ();
}

void UIObject::Width (float width)
{
	mWidth = width;

	if (mpParent)
		mpParent->OnChildUpdate ();

	if (mAnchors & LEFT)
		mAnchorLeft = Viewport::Instance ().Width () * 0.5f + X () - Width ();

	if (mAnchors & RIGHT)
		mAnchorRight = Viewport::Instance ().Width () * 0.5f - X () - Width ();
}

void UIObject::Height (float height)
{
	mHeight = height;

	if (mpParent)
		mpParent->OnChildUpdate ();

	if (mAnchors & TOP)
		mAnchorTop = Viewport::Instance ().Height () * 0.5f - Y () - Height ();

	if (mAnchors & BOTTOM)
		mAnchorBottom = Viewport::Instance ().Height () * 0.5f + Y () - Height ();
}

void UIObject::Anchors (const DWORD& anchors)
{
	mAnchors = anchors;

	if (mAnchors & LEFT)
		mAnchorLeft = Viewport::Instance ().Width () * 0.5f + X () - Width ();

	if (mAnchors & RIGHT)
		mAnchorRight = Viewport::Instance ().Width () * 0.5f - X () - Width ();

	if (mAnchors & TOP)
		mAnchorTop = Viewport::Instance ().Height () * 0.5f - Y () - Height ();

	if (mAnchors & BOTTOM)
		mAnchorBottom = Viewport::Instance ().Height () * 0.5f + Y () - Height ();
}

HRESULT UIObject::AfterParsing( )
{
	if (mAnchors & LEFT)
		mAnchorLeft = pRepository( )->InterfaceWidth( ) * 0.5f + X () - Width ();

	if (mAnchors & RIGHT)
		mAnchorRight = pRepository( )->InterfaceWidth( ) * 0.5f - X () - Width ();

	if (mAnchors & TOP)
		mAnchorTop = pRepository( )->InterfaceHeight( ) * 0.5f - Y () - Height ();

	if (mAnchors & BOTTOM)
		mAnchorBottom = pRepository( )->InterfaceHeight( ) * 0.5f + Y () - Height ();

	mFlags |= FL_NAMED;

	return S_OK;
}

void UIObject::OnNewParent (UIObject * parent)
{
	UIObject * oldpar (mpParent);

	if (oldpar)
		oldpar->OnChildUpdate ();

	mpParent = parent;

	if (parent)
		parent->OnChildUpdate ();
}

void UIObject::OnViewportUpdate ()
{
	if (mAnchors & LEFT)
	{
		if (mAnchors & RIGHT)
		{
			Width	((Viewport::Instance ().Width () - mAnchorLeft - mAnchorRight) * 0.5f);
			X		((mAnchorLeft - mAnchorRight) * 0.5f);
		}
		else
		{
			X		(mAnchorLeft + Width () - Viewport::Instance ().Width () * 0.5f);
		}
	}
	else if (mAnchors & RIGHT)
	{
			X		(Viewport::Instance ().Width () * 0.5f - mAnchorRight - Width ());
	}

	if (mAnchors & BOTTOM)
	{
		if (mAnchors & TOP)
		{
			Height	((Viewport::Instance ().Height () - mAnchorBottom - mAnchorTop) * 0.5f);
			Y		((mAnchorBottom - mAnchorTop) * 0.5f);
		}
		else
		{
			Y		(mAnchorBottom + Height () - Viewport::Instance ().Height () * 0.5f);
		}
	}
	else if (mAnchors & TOP)
	{
			Y		(Viewport::Instance ().Height () * 0.5f - mAnchorTop - Height ());
	}
}

void UIObject::ProcessKey( wchar_t )
{
	if( !IsNamed( ) )
		return;

	String event_str ( "OnPushedKey_" );
	
	event_str += Name( );

#ifdef __XFX_USE_LUA_LUABIND__
	if( SUCCEEDED( LuaScript::Instance( ).IsFunction( event_str.c_str( ) ) ) )
	{
		try
		{
			luabind::call_function< void >(
				LuaScript::Instance( ).GetLuaVM( ),
				event_str.c_str( ),
				this, static_cast< int >( key )
				);
		}
		catch( std::exception& e )
		{
			gError( "Call to lua function %s failed, reason: %s", event_str.c_str( ), e.what( ) );
		}
		catch( ... )
		{
			gError( "Call to lua function %s failed, reason: unknown", event_str.c_str( ) );
		}
	}
#endif
}

bool UIObject::ProcessMouse (float x, float y, UINT mousemsg, WPARAM )
{
	if( !IsNamed( ) )
		return false;

	bool isinside = IsInside( x, y );

	const char * event_name = NULL;

	if( isinside != ( ( mFlags & FL_PREVINSIDE ) != 0 ) )
	{
		mFlags ^= FL_PREVINSIDE;

		if( isinside )
			event_name = "OnMouseOn";
		else
			event_name = "OnMouseOut";

		if( event_name && ( mFlags & FL_NAMED ) != 0 )
		{
			String event_str ( boost::str( StringFormat( "%s_%s" ) % event_name % Name( ) ) );

#ifdef __XFX_USE_LUA_LUABIND__
			if( SUCCEEDED( LuaScript::Instance( ).IsFunction( event_str.c_str( ) ) ) )
			{
				try
				{
					luabind::call_function< void >(
						LuaScript::Instance( ).GetLuaVM( ),
						event_str.c_str( ),
						this );
				}
				catch( std::exception& e )
				{
					gError( "Call to lua function %s failed, reason: %s", event_str.c_str( ), e.what( ) );
				}
				catch( ... )
				{
					gError( "Call to lua function %s failed, reason: unknown", event_str.c_str( ) );
				}

				return true;
			}
#endif
		}

		return false;
	}

	if( isinside )
	{
		switch( mousemsg )
		{
			case WM_MOUSEMOVE:
				event_name = "OnMouseMove";
				break;

			case WM_LBUTTONDOWN:
				event_name = "OnMouseLButtonDown";
				break;

			case WM_LBUTTONUP:
				event_name = "OnMouseLButtonUp";
				break;

			case WM_LBUTTONDBLCLK:
				event_name = "OnMouseLButtonDoubleClick";
				break;

			case WM_RBUTTONDOWN:
				event_name = "OnMouseRButtonDown";
				break;

			case WM_RBUTTONUP:
				event_name = "OnMouseRButtonUp";
				break;

			case WM_RBUTTONDBLCLK:
				event_name = "OnMouseRButtonDoubleClick";
				break;

			case WM_MBUTTONDOWN:
				event_name = "OnMouseMButtonDown";
				break;

			case WM_MBUTTONUP:
				event_name = "OnMouseMButtonUp";
				break;

			case WM_MBUTTONDBLCLK:
				event_name = "OnMouseMButtonDoubleClick";
				break;

			case 0x020a://WM_MOUSEWHEEL:
				event_name = "OnMouseWheel";
				break;

			default:
				_ASSERTE( false );
		}

		if( event_name && ( mFlags & FL_NAMED ) != 0 )
		{
			String event_str ( boost::str( StringFormat( "%s_%s" ) % event_name % Name( ) ) );

#ifdef __XFX_USE_LUA_LUABIND__
			if( SUCCEEDED( LuaScript::Instance( ).IsFunction( event_str.c_str( ) ) ) )
			{
				try
				{
					luabind::call_function< void >(
						LuaScript::Instance( ).GetLuaVM( ),
						event_str.c_str( ),
						this );
				}
				catch( std::exception& e )
				{
					gError( "Call to lua function %s failed, reason: %s", event_str.c_str( ), e.what( ) );
				}
				catch( ... )
				{
					gError( "Call to lua function %s failed, reason: unknown", event_str.c_str( ) );
				}
	
				return true;
			}
#endif
		}

		return false;
	}

	return false;
}

bool UIObject::IsInside( float x, float y ) const
{
#pragma message( "FIXME: implement shape bitmask!!!" )
	return( fabs( x - mX ) < mWidth ) && ( fabs( y - mY ) < mHeight );
}

HRESULT UIObject::ParseAnchors (String::size_type& pos, const String& str)
{
	HRESULT hr;
	int lv, rv, tv, bv;

	if (FAILED (hr = ParseVariable<int> (lv, pos, str)))
		return hr;

	if (FAILED (hr = ParseVariable<int> (rv, pos, str)))
		return hr;

	if (FAILED (hr = ParseVariable<int> (tv, pos, str)))
		return hr;

	if (FAILED (hr = ParseVariable<int> (bv, pos, str)))
		return hr;

	bool l = (lv != 0);
	bool r = (rv != 0);
	bool t = (tv != 0);
	bool b = (bv != 0);

	mAnchors = l ? LEFT : NONE;
	mAnchors |= r ? RIGHT : NONE;
	mAnchors |= t ? TOP : NONE;
	mAnchors |= b ? BOTTOM : NONE;

	return S_OK;
}

HRESULT UIObject::ParseColor (String::size_type& pos, const String& str)
{
	HRESULT hr;
	ARGB col;

	if (FAILED (hr = ParseVariable<ARGB> (col, pos, str)))
		return hr;

	Color (col);

	return S_OK;
}

HRESULT UIObject::ParseRender (String::size_type& pos, const String& str)
{
	HRESULT hr;
	int val;

	if (FAILED (hr = ParseVariable<int> (val, pos, str)))
		return hr;

	IsRender (val != 0);

	return S_OK;
}

HRESULT UIObject::ParseX( String::size_type& pos, const String& str )
{
	HRESULT hr;
	float val;

	if( FAILED( hr = ParseVariable< float >( val, pos, str ) ) )
		return hr;

	X( val );

	return hr;
}

HRESULT UIObject::ParseY( String::size_type& pos, const String& str )
{
	HRESULT hr;
	float val;

	if( FAILED( hr = ParseVariable< float >( val, pos, str ) ) )
		return hr;

	Y( val );

	return hr;
}

HRESULT UIObject::ParseWidth( String::size_type& pos, const String& str )
{
	HRESULT hr;
	float val;

	if( FAILED( hr = ParseVariable< float >( val, pos, str ) ) )
		return hr;

	Width( val );

	return hr;
}

HRESULT UIObject::ParseHeight( String::size_type& pos, const String& str )
{
	HRESULT hr;
	float val;

	if( FAILED( hr = ParseVariable< float >( val, pos, str ) ) )
		return hr;

	Height( val );

	return hr;
}

HRESULT UIObject::ParseActive (String::size_type& pos, const String& str)
{
	HRESULT hr;
	int val;

	if (FAILED (hr = ParseVariable<int> (val, pos, str)))
		return hr;

	IsActive (val != 0);

	return S_OK;
}

void UIObject::LuaRegister( lua_State * L )
{
#ifdef __XFX_USE_LUA_LUABIND__
	luabind::module( L )
	[
		luabind::class_< UIObject >( "UIObject" )
			.def( "downcast", &UIObject::GetLuaObject )
			.property( "name",
				( const String& ( UIObject::* ) ( ) const ) &UIObject::Name, 
				( void ( UIObject::* ) ( const String& ) ) &UIObject::Name
				)
			.property( "x",
				( const float& ( UIObject::* ) ( ) const ) &UIObject::X, 
				( void ( UIObject::* ) ( float ) ) &UIObject::X
				)
			.property( "y",
				( const float& ( UIObject::* ) ( ) const ) &UIObject::Y, 
				( void ( UIObject::* ) ( float ) ) &UIObject::Y
				)
			.property( "width",
				( const float& ( UIObject::* ) ( ) const ) &UIObject::Width, 
				( void ( UIObject::* ) ( float ) ) &UIObject::Width
				)
			.property( "height",
				( const float& ( UIObject::* ) ( ) const ) &UIObject::Height, 
				( void ( UIObject::* ) ( float ) ) &UIObject::Height
				)
			.property( "active",
				( bool ( UIObject::* ) ( ) const ) &UIObject::IsActive, 
				( void ( UIObject::* ) ( bool ) ) &UIObject::IsActive
				)
			.property( "render",
				( bool ( UIObject::* ) ( ) const ) &UIObject::IsRender, 
				( void ( UIObject::* ) ( bool ) ) &UIObject::IsRender
				)
			.property( "has_focus", 
				&UIObject::HasFocus
				)
			.property( "color",
				( const ARGB& ( UIObject::* ) ( ) const ) &UIObject::Color, 
				( void ( UIObject::* ) ( const ARGB& ) ) &UIObject::Color
				)
			.property( "repository", 
				&UIObject::pRepository
				)
	];
#endif
}




_XFX_END
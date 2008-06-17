//  File xfx_uicomposite.cpp                                                  |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_uicomposite.h"
#include "xfx_interface.h"

_XFX_BEGIN






//
// UIComposite
//

UIComposite::UIComposite( ) :
	mChildrenScale( 1.0f )
{
	AddToken( "composite.child",			boost::bind( &UIComposite::ParseChild, this, _1, _2 ) );
}

UIComposite::~UIComposite( )
{
	RemoveAllUIObjects( );
}

void UIComposite::AddUIObject( UIObject * obj )
{
	if( obj == NULL )
		return;

	ObjectsType::iterator it = std::find( mObjects.begin( ), mObjects.end( ), obj );

	_ASSERTE( it == mObjects.end( ) );

	if( it == mObjects.end( ) )
	{
		obj->OnNewParent( this );
		obj->mpNextInGroup = NULL;

		if( !mObjects.empty( ) )
		{
			mObjects.back( )->mpNextInGroup = obj;
		}

		mObjects.push_back( obj );
	}
}

void UIComposite::RemoveUIObject( UIObject * obj )
{
	ObjectsType::iterator it = std::find( mObjects.begin( ), mObjects.end( ), obj );

	if( it != mObjects.end( ) )
	{
		if( it != mObjects.begin( ) )
		{
			ObjectsType::iterator prev = boost::prior( it );
			ObjectsType::iterator next = boost::next( it );

			( *prev )->mpNextInGroup = next == mObjects.end( ) ? NULL : ( *next )->mpNextInGroup;
		}

		mObjects.erase( it );

		obj->OnNewParent( NULL );
		obj->mpNextInGroup = NULL;
	}
}

void UIComposite::RemoveAllUIObjects( )
{
	BOOST_FOREACH( UIObject * obj, mObjects )
	{
		obj->mpNextInGroup = NULL;
		obj->OnNewParent( NULL );
	}

	mObjects.clear( );
	OnChildUpdate( );
}

void UIComposite::Render( const xfx::ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const
{
	ARGB newcolor = color * Color( );
	if( newcolor.a == 0 )
		return;

	BOOST_FOREACH( ObjectsType::value_type obj, mObjects )
	{
		if( obj->IsRender( ) )
			obj->Render( newcolor, ofs, Vec2( scale.x * mChildrenScale.x, scale.y * mChildrenScale.y ) );
	}
}

void UIComposite::Update( DWORD mspf )
{
	BOOST_FOREACH( ObjectsType::value_type& obj, mObjects )
	{
		obj->Update( mspf );
	}
}

void UIComposite::OnViewportUpdate( )
{
	UIObject::OnViewportUpdate( );

	BOOST_FOREACH( UIObject * obj, mObjects )
	{
		obj->OnViewportUpdate( );
	}
}

bool UIComposite::ProcessMouse( float x, float y, UINT mousemsg, WPARAM wParam )
{
	for( ObjectsType::reverse_iterator it = mObjects.rbegin( ); it != mObjects.rend( ); it++ )
		if( ( *it )->IsActive( ) && ( *it )->ProcessMouse( x, y, mousemsg, wParam ) )
			return true;

	return false;
}

void UIComposite::ResetState( )
{
	UIObject::ResetState( );

	BOOST_FOREACH( UIObject * obj, mObjects )
	{
		obj->ResetState( );
	}
}

void UIComposite::OnChildUpdate( )
{
	ComputePositionSize( );
}

void UIComposite::ComputePositionSize( )
{
	if( mObjects.empty( ) )
	{
		UIObject::X( 0.0f );
		UIObject::Y( 0.0f );
		UIObject::Width( 1.0f );
		UIObject::Height( 1.0f );
		return;
	}

	float minx, maxx, miny, maxy;

	minx = miny = std::numeric_limits< float >::max( );
	maxx = maxy = -std::numeric_limits< float >::max( );

	BOOST_FOREACH( UIObject * obj, mObjects )
	{
		float obj_minx = obj->X( ) - obj->Width( );
		float obj_maxx = obj->X( ) + obj->Width( );

		float obj_miny = obj->Y( ) - obj->Height( );
		float obj_maxy = obj->Y( ) + obj->Height( );

		if( obj_minx < minx )
			minx = obj_minx;

		if( obj_maxx > maxx )
			maxx = obj_maxx;

		if( obj_miny < miny )
			miny = obj_miny;

		if( obj_maxy > maxy )
			maxy = obj_maxy;
	}

	UIObject::X( 0.5f * ( minx + maxx ) );
	UIObject::Y( 0.5f * ( miny + maxy ) );
	UIObject::Width( 0.5f * ( maxx - minx ) );
	UIObject::Height( 0.5f * ( maxy - miny ) );
}

HRESULT UIComposite::PrepareParsing( )
{
	RemoveAllUIObjects( );

	return UIObject::PrepareParsing( );
}

HRESULT UIComposite::AfterParsing( )
{
	return UIObject::AfterParsing( );
}

HRESULT UIComposite::ParseChild( String::size_type& pos, const String& str )
{
	String child_name = next_token( str, pos );

	_ASSERTE( pRepository( ) );
	if( !pRepository( ) )
		return XFXERR_INVALIDCALL;

	UIObject * obj = pRepository( )->FindUIObject( child_name ).get( );

	if( obj == NULL )
		return XFXERR_NOTFOUND;

	AddUIObject( obj );

	return S_OK;
}

void UIComposite::LuaRegister( lua_State * L )
{
#ifdef __XFX_USE_LUA_LUABIND__
	luabind::module( L )
	[
		luabind::class_< UIComposite, UIObject >( "UIComposite" )
			.property( "children_scale",
				( const Vec2& ( UIComposite::* ) ( ) const ) &UIComposite::ChildrenScale,
				( void ( UIComposite::* ) ( const Vec2& ) ) &UIComposite::ChildrenScale
				)
	];
#endif
}



_XFX_END
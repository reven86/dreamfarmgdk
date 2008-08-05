//  File xfx_ui_serialization_impl.h                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_ui_serialization_impl.h 
 *	\brief Serialization implementation for Utility library.
 */




_XFX_BEGIN




//
// UIInterface
//

template< class _Archive >
void UIInterface::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_NVP( mInterfaceObjectPtr );
	ar & BOOST_SERIALIZATION_NVP( mMouseCursorPtr );
	ar & BOOST_SERIALIZATION_NVP( mFocusObjectPtr );
};



//
// UIRepository
//

template< class _Archive >
void UIRepository::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( ScriptResource );

	ar & BOOST_SERIALIZATION_NVP( mObjects );
	ar & BOOST_SERIALIZATION_NVP( mInterfaceWidth );
	ar & BOOST_SERIALIZATION_NVP( mInterfaceHeight );
	ar & BOOST_SERIALIZATION_NVP( mBackground );

	if( _Archive::is_loading::value )
	{
		// set mpRepository link in all objects
		BOOST_FOREACH( ObjectsType::value_type& pair, mObjects )
		{
			pair.second->mpRepository = this;
		}
	}
};



//
// UIConsole
//

template< class _Archive >
void UIStandardConsole::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIObject );

	ar & BOOST_SERIALIZATION_NVP( mBackground );
	ar & BOOST_SERIALIZATION_NVP( mMessagesLabel );
	ar & BOOST_SERIALIZATION_NVP( mPromptLabel );
	ar & BOOST_SERIALIZATION_NVP( mPromptEdit );
	ar & BOOST_SERIALIZATION_NVP( mMiniConsoleLabel );

	if( _Archive::is_loading::value )
		mPromptEditPtr.reset( &mPromptEdit, NullDeleter< UIEdit >( ) ); 

	ar & BOOST_SERIALIZATION_NVP( mMessages );
	ar & BOOST_SERIALIZATION_NVP( mMessagesCount );
	ar & BOOST_SERIALIZATION_NVP( mMessagesStartIndex );

	ar & BOOST_SERIALIZATION_NVP( mFadeDistance );
	ar & BOOST_SERIALIZATION_NVP( mFadeTime );
	ar & BOOST_SERIALIZATION_NVP( mCurrentState );
	ar & BOOST_SERIALIZATION_NVP( mMoveTime );
	ar & BOOST_SERIALIZATION_NVP( mTimeToHideMiniConsole );
	ar & BOOST_SERIALIZATION_NVP( mMiniConsoleLinesNumber );
};

BOOST_CLASS_EXPORT( xfx::UIStandardConsole );



//
// UIDummy
//

template< class _Archive >
void UIDummy::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIObject );
};

BOOST_CLASS_EXPORT( xfx::UIDummy );



//
// UIEdit
//

template< class _Archive >
void UIEdit::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIObject );

	ar & BOOST_SERIALIZATION_NVP( mText );
	ar & BOOST_SERIALIZATION_NVP( mLabel );
	ar & BOOST_SERIALIZATION_NVP( mCursor );
	ar & BOOST_SERIALIZATION_NVP( mLocalTime );
	ar & BOOST_SERIALIZATION_NVP( mMaxChars );
};

BOOST_CLASS_EXPORT( xfx::UIEdit );




//
// UIFocusable
//

template< class _Archive >
void UIFocusable::serialize( _Archive& ar, const unsigned int version )
{
};



//
// UILabel
//

template< class _Archive >
void UILabel::serialize( _Archive& ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIObject );

	if( _Archive::is_saving::value )
	{
		boost::shared_ptr< ShaderParams > shader_consts = boost::const_pointer_cast< ShaderConsts >( mShaderConstsPtr );

		ar & BOOST_SERIALIZATION_NVP( shader_consts );
	}
	else
	{
		boost::shared_ptr< ShaderParams > shader_consts;
		ar & BOOST_SERIALIZATION_NVP( shader_consts );

		mShaderParamsPtr = shader_consts;
	}

	ar & BOOST_SERIALIZATION_NVP( mFontPtr );

	ar & BOOST_SERIALIZATION_NVP( mCurText );
	ar & BOOST_SERIALIZATION_NVP( mIsRealSize );

	ar & BOOST_SERIALIZATION_NVP( mLx );
	ar & BOOST_SERIALIZATION_NVP( mLy );
	ar & BOOST_SERIALIZATION_NVP( mMaxWidth );
	ar & BOOST_SERIALIZATION_NVP( mXScale );
	ar & BOOST_SERIALIZATION_NVP( mYScale );
	ar & BOOST_SERIALIZATION_NVP( mAlign );

	if( _Archive::is_loading::value )
	{
		mIsTrisCached = false;
		CacheTris( );
	}
};

BOOST_CLASS_EXPORT( xfx::UILabel );



//
// UIObject
//

template< class _Archive >
void UIObject::serialize( _Archive& ar, const unsigned int version )
{
	// this links will be restored in UIComposite::serialize
	//ar & BOOST_SERIALIZATION_NVP( mpParent );
	//ar & BOOST_SERIALIZATION_NVP( mpNextInGroup );

	ar & BOOST_SERIALIZATION_NVP( mX );
	ar & BOOST_SERIALIZATION_NVP( mY );
	ar & BOOST_SERIALIZATION_NVP( mWidth );
	ar & BOOST_SERIALIZATION_NVP( mHeight );

	ar & BOOST_SERIALIZATION_NVP( mFlags );

	ar & BOOST_SERIALIZATION_NVP( mAnchorLeft );
	ar & BOOST_SERIALIZATION_NVP( mAnchorTop );
	ar & BOOST_SERIALIZATION_NVP( mAnchorRight );
	ar & BOOST_SERIALIZATION_NVP( mAnchorBottom );

	ar & BOOST_SERIALIZATION_NVP( mAnchors );

	ar & BOOST_SERIALIZATION_NVP( mColor );
	ar & BOOST_SERIALIZATION_NVP( mName );

	// this link will be restored in UIRepository::serialize
	//ar & BOOST_SERIALIZATION_NVP( mpRepository );
};

BOOST_IS_ABSTRACT( xfx::UIObject );





//
// UIProgress
//

template< class _Archive >
void UIProgress::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIObject );

	ar & BOOST_SERIALIZATION_NVP( mShaderPtr );
	ar & BOOST_SERIALIZATION_NVP( mProgressValue );
	ar & BOOST_SERIALIZATION_NVP( mIsVertical );

	if( _Archive::is_saving::value )
	{
		boost::shared_ptr< ShaderParams > shader_consts = boost::const_pointer_cast< ShaderConsts >( mShaderConstsPtr );

		ar & BOOST_SERIALIZATION_NVP( shader_consts );
	}
	else
	{
		boost::shared_ptr< ShaderParams > shader_consts;
		ar & BOOST_SERIALIZATION_NVP( shader_consts );

		mShaderParamsPtr = shader_consts;
	}
};

BOOST_CLASS_EXPORT( xfx::UIProgress );



//
// UIScroll
//

template< class _Archive >
void UIScroll::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIObject );

	ar & BOOST_SERIALIZATION_NVP( mShaders );
	ar & BOOST_SERIALIZATION_NVP( mScrollPos );
	ar & BOOST_SERIALIZATION_NVP( mArrowStep );
	ar & BOOST_SERUALIZATION_NVP( mScrollFlags );
};



//
// UIHScroll
//

template< class _Archive >
void UIHScroll::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIScroll );
}

BOOST_CLASS_EXPORT( xfx::UIScroll );



//
// UISprite
//

template< class _Archive >
void UISprite::serialize( _Archive & ar, const unsigned int version )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( UIObject );

	ar & BOOST_SERIALIZATION_NVP( mShaders );
	ar & BOOST_SERIALIZATION_NVP( mRoll );

	ar & BOOST_SERIALIZATION_NVP( mCurrentShaderIndex );

	if( _Archive::is_saving::value )
	{
		boost::shared_ptr< ShaderParams > shader_consts = boost::const_pointer_cast< ShaderConsts >( mShaderConstsPtr );

		ar & BOOST_SERIALIZATION_NVP( shader_consts );
	}
	else
	{
		boost::shared_ptr< ShaderParams > shader_consts;
		ar & BOOST_SERIALIZATION_NVP( shader_consts );

		mShaderParamsPtr = shader_consts;
	}
};

BOOST_CLASS_EXPORT( xfx::UISprite );





_XFX_END
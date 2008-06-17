//  File xfx_uisprite.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_uiobject.h"

/*!	\file xfx_uisprite.h 
 *	\brief Sprite user control. 
 */

_XFX_BEGIN






/*! \class UISprite xfx_uisprite.h "ui/xfx_uisprite.h"
 *	\brief Sprite user control.
 *	\ingroup InterfaceGroup
 *
 *	%UISprite file syntax can be found at section \ref uisprite_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uisprite_script UISprite file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	< UIObject properties >

	sprite.shader		"textures\interface\mouse_cursor1.shader"
	sprite.shader		"textures\interface\mouse_cursor2.shader"
}
	\endverbatim
 *	
 *	Description:
 *
 *	\note Keyword \a sprite is omitted.
 *
 *	\b shader		-	Sprite shader file name. \n
 *	\b roll			-	Rotation angle in degrees. \n
 *
 *	\see UIObject, UISprite, Shader
 *
 *	@}
 */

class UISprite : public UIObject
{
	friend class boost::serialization::access;

	std::vector< boost::shared_ptr< const class Shader > >		mShaders;
	boost::shared_ptr< const class ShaderConsts >				mShaderConstsPtr;
	
	xfx::Math::Angle											mRoll;

	unsigned													mCurrentShaderIndex;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UISprite															( );
	virtual ~UISprite													( );

	//! Get roll angle.
	const xfx::Math::Angle&					GetRollAngle				( ) const { return mRoll; };

	//! Set roll angle.
	void									SetRollAngle				( const xfx::Math::Angle& ang ) { mRoll = ang; };

	//! Add new shader.
	void									AddShader					( const boost::shared_ptr< const Shader >& shd ) { mShaders.push_back( shd ); };

	//! Get shaders count.
	unsigned								ShadersCount				( ) const { return static_cast< unsigned >( mShaders.size( ) ); };

	//! Get current shader index.
	unsigned								GetCurrentShaderIndex		( ) const { return mCurrentShaderIndex; };

	//! Set current shader index.
	void									SetCurrentShaderIndex		( unsigned ind ) { _ASSERTE( ind < ShadersCount( ) ); if( ind < ShadersCount( ) ) mCurrentShaderIndex = ind; };

	//! Set shader.
	void									ShaderPtr					( unsigned ind, const boost::shared_ptr< const Shader >& shd );

	//! Get shader.
	const boost::shared_ptr< const Shader >& ShaderPtr					( unsigned ind ) const { _ASSERTE( ind >= 0 && ind < ShadersCount( ) ); return mShaders[ ind ]; };

	//! Set shader consts for sprite shader.
	void									ShaderConstsPtr				( const boost::shared_ptr< const ShaderConsts >& shd_const ) { mShaderConstsPtr = shd_const; };

	//! Get shader consts for sprite shader.
	const boost::shared_ptr< const ShaderConsts >& ShaderConstsPtr		( ) const { return mShaderConstsPtr; };

	/*! \brief %Render control.
	 *
	 *	\param[in]	color	Color modulator.
	 *	\param[in]	ofs		X/Y offset.
	 *	\param[in]	scale	X/Y scale.
	 */
	virtual void							Render						( const ARGB& color, const xfx::Vec2& ofs, const xfx::Vec2& scale ) const;
	
	//! Update sprite.
	virtual void							Update						( DWORD mspf );

	/*! \brief Register lua representation.
	 *
	 *	This function is called by LuaScript::Open.
	 *
	 *	\param[in]	L		Lua state.
	 */
	static void								LuaRegister					( struct lua_State * L );

protected:
	virtual HRESULT							PrepareParsing				();
	virtual HRESULT							AfterParsing				();

	HRESULT									ParseShader					(String::size_type& pos, const String& str);

	//! Get lua object for this class.
#ifdef __XFX_USE_LUA_LUABIND__
	virtual luabind::object					GetLuaObject				( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };
#endif
};




_XFX_END

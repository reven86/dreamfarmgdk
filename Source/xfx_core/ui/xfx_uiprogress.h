//  File xfx_uiprogress.h                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_uiobject.h"

/*!	\file xfx_uiprogress.h 
 *	\brief Progress user control. 
 */

_XFX_BEGIN






/*! \class UIProgress xfx_uiprogress.h "ui/xfx_uiprogress.h"
 *	\brief Progress user control.
 *	\ingroup InterfaceGroup
 *
 *	%UIProgress file syntax can be found at section \ref uiprogress_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uiprogress_script UIProgress file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	< UIObject properties >

	progress.shader		"textures\interface\progress.shader"
	progress.value		0.5
	progress.vertical	1
}
	\endverbatim
 *	
 *	Description:
 *
 *	\note Keyword \a progress is omitted.
 *
 *	\b shader		-	Progress shader file name. \n
 *	\b value		-	Progress value from 0 to 1. \n
 *	\b vertical		-	Progress raise from bottom to top. \n
 *
 *	\see UIObject, UIProgress, Shader
 *
 *	@}
 */

class UIProgress : public UIObject
{
	friend class boost::serialization::access;

	boost::shared_ptr< const class Shader >			mShaderPtr;
	boost::shared_ptr< const class ShaderConsts >	mShaderConstsPtr;

	float											mProgressValue;
	bool											mIsVertical;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UIProgress															( );
	virtual ~UIProgress													( );

	//! Set shader.
	void									ShaderPtr					( const boost::shared_ptr< const Shader >& shd );

	//! Get shader.
	const boost::shared_ptr<const Shader>&	ShaderPtr					( ) const { return mShaderPtr; };

	//! Set shader consts for sprite shader.
	void									ShaderConstsPtr				( const boost::shared_ptr< const ShaderConsts >& shd_const ) { mShaderConstsPtr = shd_const; };

	//! Get shader consts for sprite shader.
	const boost::shared_ptr< const ShaderConsts >& ShaderConstsPtr		( ) const { return mShaderConstsPtr; };

	//! Set progress value.
	void									ProgressValue				( const float& new_pos ) { mProgressValue = new_pos; };

	//! Get progress value.
	const float&							ProgressValue				( ) const { return mProgressValue; };

	//! Set 'vertical' flag.
	void									SetVertical					( bool vertical ) { mIsVertical = vertical; };

	//! Get 'vertical' flag.
	const bool&								IsVertical					( ) const { return mIsVertical; };

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
	static void								LuaRegister					( lua_State * L );

protected:
	virtual HRESULT							PrepareParsing				( );
	virtual HRESULT							AfterParsing				( );

	HRESULT									ParseShader					( String::size_type& pos, const String& str );

	//! Get lua object for this class.
	virtual luabind::object					GetLuaObject				( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };
};




_XFX_END

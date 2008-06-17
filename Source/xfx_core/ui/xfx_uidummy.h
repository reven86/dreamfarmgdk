//  File xfx_uidummy.h                                                        |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_uiobject.h"

/*!	\file xfx_uidummy.h 
 *	\brief Dummy interface control.
 */

_XFX_BEGIN






/*! \class UIDummy xfx_uidummy.h "ui/xfx_uidummy.h"
 *	\brief Dummy control.
 *	\ingroup InterfaceGroup
 *
 *	%UIDummy file syntax can be found at section \ref uidummy_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section uidummy_script UIDummy file syntax
 *
 *	Example:
 *
 *	\verbatim
control < control class >
{
	< UIObject properties >
}
	\endverbatim
 *	
 *	\see UIObject, UIDummy
 *
 *	@}
 */

class UIDummy : public UIObject
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	UIDummy														( ) { };
	virtual ~UIDummy											( ) { };

	/*! \brief %Render control.
	 *
	 *	\param[in]	color	Color modulator.
	 *	\param[in]	ofs		X/Y offset.
	 *	\param[in]	scale	X/Y scale.
	 */
	virtual void						Render					( const ARGB&, const Vec2&, const Vec2& ) const { };

protected:
	//! Get lua object for this class.
#ifdef __XFX_USE_LUA_LUABIND__
	virtual luabind::object				GetLuaObject			( ) { return luabind::object( LuaScript::Instance( ).GetLuaVM( ), this ); };
#endif
};





_XFX_END

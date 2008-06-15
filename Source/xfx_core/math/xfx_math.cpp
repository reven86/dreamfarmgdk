//  File xfx_math.cpp                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_math.h"

_XFX_BEGIN





//
// Constants
//

const float Math::inv_pi		= 0.000030517578125f;
const float Math::rad			= 10430.3783504704527f;
const float Math::inv_rad		= 0.000095873799242853f;
const float Math::deg			= 182.04444444444444444444444444444f;
const float Math::inv_deg		= 0.0054931640625f;
const float Math::real_pi		= 3.141592654f;
const float Math::real_inv_pi	= 0.318309886f;




//
// Functions
//

//! Parse Math::Angle variable (converts from degree to Math::Angle).
HRESULT	ParseAngleVariable ( Math::Angle& var, String::size_type& pos, const String& str )
{
	float fvar;
	skip_comments( str, pos );
	fvar = static_cast< float >( atof( next_token( str, pos ).c_str( ) ) );

	var = static_cast< Math::Angle >( math_deg2angle( fvar ) );
	return S_OK;
};

//! Parse Math::BigAngle variable (converts from degree to Math::BigAngle).
HRESULT ParseBigAngleVariable ( Math::BigAngle& var, String::size_type& pos, const String& str )
{
	float fvar;
	skip_comments( str, pos );
	fvar = static_cast< float >( atof( next_token( str, pos ).c_str( ) ) );

	var = math_deg2angle( fvar );
	return S_OK;
};





_XFX_END
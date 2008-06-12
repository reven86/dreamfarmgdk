//  File xfx_mathstat.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_mathstat.h 
 *	\brief %Statistics functions.
 */

_XFX_BEGIN





/*! \class Statistics xfx_mathstat.h "math/xfx_mathstat.h"
 *	\brief %Statistics support.
 *	\ingroup MathGroup
 *
 *	%Statistics holds some value and calculates average and dispertion for those values.
 *	Can be specialized by _T - value type and _Period - values count.
 *	
 *	\author Andrew "RevEn" Karpushin
 */

template< class _T, int _Period >
class Statistics
{
	boost::array< _T, _Period >	mValues;
	int							mCurIndex;

	_T							mAverage;
	_T							mDispersion;

public:
	//! Constructs statistics with specified start value.
	Statistics												( const _T& start_val ) :
		mAverage( start_val ),
		mDispersion( start_val - start_val ),
		mCurIndex( 0 )
	{
		mValues.assign( start_val );
	};

	//! Destructor.
	~Statistics												( ) { };

	//! Get an average.
	const _T&				Average							( ) const { return mAverage; };

	//! Get a dispertion.
	const _T&				Dispersion						( ) const { return mDispersion; };

	//! Add new value to statistics.
	void					AddValue						( const _T& value )
	{
		_T average_delta	= static_cast< _T >( ( value - mValues[ mCurIndex ] ) / _Period );
		_T dispersion_delta	= static_cast< _T >( (
			( value - mAverage - average_delta ) * ( value - mAverage - average_delta ) - 
			( mValues[ mCurIndex ] - mAverage ) * ( mValues[ mCurIndex ] - mAverage )
			) / _Period );

		mValues[ mCurIndex ] = value;
		mCurIndex = ( mCurIndex + 1 ) % _Period;

		mAverage		+= average_delta;
		mDispersion		+= dispersion_delta;
	};
};





_XFX_END

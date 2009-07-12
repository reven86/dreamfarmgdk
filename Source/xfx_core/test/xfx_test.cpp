#include "xfx.h"


/*! \class Application
 *	\brief Defines a base class for application.
 *	
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Application : public xfx::Singleton< Application >, public xfx::Application
{
	friend class xfx::Singleton< Application >;
};




//
// gGetApplication implementation
//

xfx::Application& xfx::gGetApplication( )
{
	return ::Application::Instance( );
}


using namespace xfx;

// repeat all tests two times

BOOST_AUTO_TEST_SUITE( xfx_test1 )

#include "xfx_test_utility.inl"
#include "xfx_test_math.inl"
#include "xfx_test_main.inl"

BOOST_AUTO_TEST_SUITE_END( )

BOOST_AUTO_TEST_SUITE( xfx_test2 )

#include "xfx_test_utility.inl"
#include "xfx_test_math.inl"
#include "xfx_test_main.inl"

BOOST_AUTO_TEST_SUITE_END( )

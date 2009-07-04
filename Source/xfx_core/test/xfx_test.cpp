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

#include "xfx_test_utility.inl"
#include "xfx_test_math.inl"
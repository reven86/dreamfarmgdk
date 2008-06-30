/*! \file application.h
 *
 *	Application class, inherited from xfx::Application
 */






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
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

	/*! \brief Timer used in scene update method.
	 */
	xfx::Timer										mTimer;

	//! GUI object.
	boost::scoped_ptr< xfx::GUI >					mGUI;

public:

	/*! \brief Init engine and renderer.
	 *
	 *	In order to properly renderer out first sprite we have to 
	 *	initialize renderer. The best place to so is here.
	 *	We override default InitEngine method and place renderer 
	 *	initialization code there.
	 *
	 *	\param[in] inst		HINSTANCE of execution process
	 *	\param[in] cmdline	Command line
	 *	\param[in] wndclass	Window Class of created window
	 *	\param[in] caption	Caption of created window
	 *
	 *	\return Returns S_OK if engine and renderer has been initialized sucessfully.
	 */
	virtual HRESULT					InitEngine						( HINSTANCE inst, const xfx::String& cmdline, const xfx::WString& wndclass, const xfx::WString& caption );

	/*! \brief Deinitialization.
	 *	
	 *	Deinitialize renderer and then call xfx::Application::Shutdown.
	 */
	virtual void					Shutdown						( );

	/*! \brief Update scene.
	 *
	 *	Override Update method to perform scene updating.
	 *	In this call we update timer and at the end prepare scene to render.
	 */
	virtual void					Update							( );

	/*! \brief Render scene.
	 *
	 *	Override Render method to perform rendering.
	 */
	virtual void					Render							( ) const;

protected:
	/*! \brief Windows message handler.
	 */
	virtual LRESULT CALLBACK		WndProc							( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};
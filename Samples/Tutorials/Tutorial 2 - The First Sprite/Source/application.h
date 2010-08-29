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

	/*! \brief Define a Shader resource that will be used in sprite rendering.
	 *
	 *	Shared is a set of textures and algorithm (effect) that are used during rendering.
	 *	Resources with equal filenames are shared and should be constant.
	 *	That is why we use smart pointer to constant object to store shader.
	 */
	boost::shared_ptr< const xfx::Shader >			mSpriteShader;

	//! User-defined parameters such as current frame used together with shader.
	boost::shared_ptr< xfx::ShaderParams >			mShaderParamsConsts;

	/*! \brief Timer used in scene update method.
	 */
	xfx::Timer										mTimer;

public:

	/*! \brief Init engine and renderer.
	 *
	 *	In order to properly renderer out first sprite we have to 
	 *	initialize renderer. The best place to so is here.
	 *	We override default InitEngine method and place renderer 
	 *	initialization code there.
	 *
	 *	\param[in] cmdline	Command line
	 *	\param[in] wndclass	Window Class of created window
	 *	\param[in] caption	Caption of created window
	 *
	 *	\return Returns S_OK if engine and renderer has been initialized sucessfully.
	 */
	virtual HRESULT					InitEngine						( const xfx::String& cmdline, const xfx::WString& wndclass, const xfx::WString& caption );

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
};
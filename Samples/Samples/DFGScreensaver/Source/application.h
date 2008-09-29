/*! \file application.h
 *
 *	Screensaver class.
 */






/*! \class Screensaver
 *	\brief Defines a base class for Screensaver.
 *	
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Screensaver
{
	// Shaders
	boost::shared_ptr< const xfx::Shader >					mBlackMaskShader;
	boost::shared_ptr< const xfx::Shader >					mBackgroundMaskShader;
	boost::shared_ptr< const xfx::Shader >					mBackgroundShader;
	boost::shared_ptr< const xfx::Shader >					mSheepShader;

	//! Sheep shader parameters.
	boost::shared_ptr< xfx::ShaderParams >					mSheepShaderParams;

	/*! \brief Screensaver states.
	 */
	enum ELogoState
	{
		ELG_EMPTY_SCREEN,							//!< Just started - empty screen.
		ELG_SHOW_FARM,								//!< Show farm from point to full round.
		ELG_FARM_ANIMATION,							//!< Play farm animation.

		ELG_MAX_STATES,
	};

	//! Information about each state.
	struct LogoStateInfo
	{
		//! State duration in 100 microseconds
		DWORD								duration;

		//! Functor used to render this state.
		boost::function1< void, float >		render_fn;

		LogoStateInfo						( ) { };
		LogoStateInfo						( DWORD _d, const boost::function1< void, float >& fn ) : duration( _d ), render_fn( fn ) { };
	};

	//! Information about all states.
	boost::array< LogoStateInfo, ELG_MAX_STATES >			mLogoStateInfos;

	//! Current state.
	ELogoState												mCurrentState;

	//! Time in current state.
	DWORD													mStateTime;

	//! Normalized time in current state.
	DWORD													mLocalTime;

	// Relative sheep transformation keys.
	xfx::Envelope< xfx::Vec2, WORD >						mSheepPathKeys;
	xfx::Envelope< xfx::Math::BigAngle >					mSheepRotationKeys;
	xfx::Envelope< xfx::Vec2 >								mSheepScaleKeys;

	// Screen center coordinates.
	float													mLogoCenterX;
	float													mLogoCenterY;
	float													mLogoWidth;
	float													mLogoHeight;

	/*! \brief Timer used in scene update method.
	 */
	xfx::Timer										mTimer;

public:
	Screensaver												( );

	/*! \brief Init engine and renderer.
	 *
	 *	\return Always S_OK.
	 */
	HRESULT							Init							( );

	/*! \brief Update scene.
	 *
	 *	Override Update method to perform scene updating.
	 *	In this call we update timer and at the end prepare scene to render.
	 */
	void							Update							( );

	/*! \brief Render scene.
	 *
	 *	Override Render method to perform rendering.
	 */
	void							Render							( ) const;

private:
	//! Render empty state.
	void									RenderEmptyScreen					( float state_normal_time ) const;

	//! Render farm.
	void									RenderShowFarm						( float state_normal_time ) const;

	//! Render sheeps on farm.
	void									RenderFarmAnimation					( float state_normal_time ) const;

	//! Render mask.
	void									RenderMask							( float cur_width, float cur_height ) const;

	//! Render only sheeps.
	void									RenderSheeps						( ) const;
};
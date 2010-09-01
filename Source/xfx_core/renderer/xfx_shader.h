//  File xfx_shader.h                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_shader.h 
 *	\brief %Shader system.
 */

_XFX_BEGIN






/*! \class Effect xfx_shader.h "render/xfx_shader.h"
 *	\brief Wrapper for ID3DXEffect.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Effect : public Resource
{
	enum SemanticType
	{
		ES_WORLD,
		ES_WORLDVIEW,
		ES_WORLDVIEWPROJECTION,

		ES_VIEW,
		ES_VIEWPROJECTION,

		ES_PROJECTION,

		ES_TOTAL
	};

	boost::shared_ptr< struct ID3DXEffect >				mDXEffectPtr;

#if( __XFX_DIRECTX_VER__ < 9 )
	String												mValidTechnique;
#else
	D3DXHANDLE											mValidTechnique;

	boost::array< D3DXHANDLE, ES_TOTAL >				mKnownSemantics;

	mutable stdext::hash_map< String, D3DXHANDLE, HashCompare< String > >		mCachedParameters;
#endif

	boost::signals::connection							mInvalidateConnection;
	static Cache< Effect >								msCache;

public:
	Effect																	( );
	virtual ~Effect															( );

	//! Get ID3DXEffect.
	const boost::shared_ptr< ID3DXEffect >&			DXEffectPtr				( ) const { return mDXEffectPtr; };

#if( __XFX_DIRECTX_VER__ < 9 )
	//! Get a handle for parameter.
	const char *									GetHandle				( const String& parameter ) const { return parameter.c_str( ); };
#else
	//! Get a handle for parameter.
	D3DXHANDLE										GetHandle				( const String& parameter ) const
	{
		stdext::hash_map< String, D3DXHANDLE>::const_iterator it = mCachedParameters.find( parameter );

		if( it == mCachedParameters.end( ) )
		{
			return mCachedParameters[ parameter ] = mDXEffectPtr ? mDXEffectPtr->GetParameterByName( NULL, parameter.c_str( ) ) : NULL;
		}

		return ( *it ).second;
	};
#endif

	/*! \brief %Render effect.
	 *
	 *	\param[in]	render_fn	%Render functor.
	 *	\param[in]	flags		Flags for ID3DXEffect::Begin method.
	 */
	void											RenderEffect			( const boost::function0< void >& render_fn, DWORD flags = 0 ) const;

	//! Load effect from memory. Calls D3DXCreateEffect.
	virtual HRESULT									LoadMemory				( const void * pmemory, unsigned long filelen );

	/*! \brief Load effect from file.
	 *	
	 *	\param[in]	filename		File name.
	 *
	 *	\note If file extension absent, newly extensions added by priorities: fx, fxo.
	 */
	virtual HRESULT									LoadFile				( const String& filename );

	//! Get effects Cache.
	static Cache< Effect >&							Cache					( ) { return msCache; };

private:
	const char *									GetSemanticName			( const int& sem ) const;

	void											SetupEffectData			( ) const;

	void											Cleanup					( );
	void											InvalidateCallback		( bool invalidate );
};





/*! \class Shader xfx_shader.h "render/xfx_shader.h"
 *	\brief %Shader class.
 *	\ingroup RenderGroup
 *
 *	%Shader class provides information about how object will look.
 *	Holds an Effect pointer and texture maps. Used together with ShaderParams.
 *
 *	%Shader file syntax can be found here: \ref shader_script.
 *
 *	\author Andrew "RevEn" Karpushin
 */

/*! \addtogroup ScriptResourceGroup
 *	@{
 *	\section shader_script Shader file syntax
 *
 *	Example:
 *
 *	\verbatim
shader
{
	map		FontTex		"font\system_font:font\system_font_a"
	animmap	DiffuseTex	"textures\sfx\caustic"	0.2 0.2 30 24

	save_rs	0

	effect	"font\system.font.fx"
}
	\endverbatim
 *	
 *	Description:
 *
 *	\b map			-	Associate texture name in effect file (*.fx) with texture file. \n
 *	\b cubemap		-	Associate texture name in effect file (*.fx) with cubemap texture file. \n
 *	\b animmap		-	Associate texture name in effect file (*.fx) with animated texture file.
 *						Next to filename are frame width, frame height, FPS and total frames number. After applying \b animmap user can 
 *						use const xfx_<TexName>_time to set animation frame. (e.g. xfx_DiffuseTex_time 5.0).\n
 *	\b save_rs		-	Save render states and restore them after primitive draw call (Default: 0). \n
 *	\b effect		-	%Effect file name.
 *
 *	\see Shader, Texture, CubemapTexture, Effect
 *
 *	@}
 */

class Shader : public ScriptResource
{
	//
	// Temporary loading data
	//

	typedef stdext::hash_map< String, boost::shared_ptr< const interface ITexture >, HashCompare< String > > NameTextureMapsType;

	NameTextureMapsType									mNamedTextureMaps;

public:
	struct TextureInfo
	{
		String											stage_name;
		boost::shared_ptr< const ITexture >				texture;
		float											frame_width;
		float											frame_height;
		int												frame_count;
		int												fps;
	};

private:
#if( __XFX_DIRECTX_VER__ < 9 )
	typedef std::vector< boost::tuple< String, int, TextureInfo > >			TextureMapsType;
#else
	typedef std::vector< boost::tuple< D3DXHANDLE, int, TextureInfo > >		TextureMapsType;
#endif

	boost::shared_ptr< const Effect >					mEffectPtr;
	TextureMapsType										mTextureMaps;

	// there should be Renderer::MAX_TEXTURE_STAGES
	boost::array< TextureInfo *, 8 >					mTextureStages;

	static Cache< Shader >								msCache;

	static int											msID;
	int													mID;

	enum ShaderFlags
	{
		ESF_USE_TEXTURE_TRANSFORM		= ( 1 << 0 ),
		ESF_RESTORE_STATES				= ( 1 << 1 ),
	};

	DWORD												mFlags;

	//
	// Information used during shader loading process.
	//

	typedef std::vector< TextureInfo >					LoadingMapsContainer;
	boost::scoped_ptr< LoadingMapsContainer >			mLoadingInfoPtr;

public:
	//! Constructs empty shader.
	Shader															( );

	//! Destuctor.
	virtual ~Shader													( );

	//! Copy constructor.
	Shader															( const Shader& shd );

	//! Assignment operator.
	Shader&									operator =				( const Shader& shd );

	//! Equality operator.
	bool									operator ==				( const Shader& shd ) const { return mID == shd.mID; };

	//! Get an ID.
	const int&								ID						( ) const { return mID; };

	//! Get shader Cache.
	static Cache< Shader >&					Cache					( ) { return msCache; };

	//! Get texture by name.
	boost::shared_ptr< const ITexture >		TextureMap				( const String& name ) const;

	/*! \brief Get texture on specific stage.
	 *
	 *	\param[in]	stage		Stage number.
	 */
	const TextureInfo *						GetTextureInfoOnStage	( const int& stage ) const { _ASSERTE( stage >= 0 && stage < 8 ); return mTextureStages[ stage ]; };

	/*! \brief Remove texture map from shader.
	 *
	 *	\param[in]	name		Stage name.
	 */
	void									RemoveTextureMap		( const String& name );

	/*! \brief Add new texture map to shader.
	 *
	 *	\param[in]	tex_info	Texture map info.
	 *
	 *	\return New stage number if texture is used in current effect or -1.
	 */
	int										AddTextureMap			( const TextureInfo& tex_info );

	//! Get effect pointer (const version).
	const boost::shared_ptr< const Effect>&	GetEffectPtr			( ) const { return mEffectPtr; };

	/*! Set effect for shader.
	 *
	 *	\param[in]	eff			New effect.
	 *
	 *	\note Textures stage numbers can be changed, according to new effect.
	 */
	void									SetEffectPtr			( const boost::shared_ptr< const Effect >& eff );

	/*! \brief %Render function.
	 *
	 *	\param[in]	consts		Shader constants (defined by user).
	 *	\param[in]	render_fn	%Render functor.
	 */
	void									RenderEffect			( const class ShaderParams& consts, const boost::function0< void >& render_fn ) const;

private:
	virtual HRESULT							PrepareParsing			( );
	virtual HRESULT							AfterParsing			( );

	HRESULT 								ParseMap				( String::size_type& pos, const String& str );
	HRESULT 								ParseCubeMap			( String::size_type& pos, const String& str );
	HRESULT									ParseAnimMap			( String::size_type& pos, const String& str );
	HRESULT									ParseEffect				( String::size_type& pos, const String& str );
	HRESULT									ParseSaveRS				( String::size_type& pos, const String& str );

	void									SetEffectTextures		( const boost::shared_ptr< const Effect >& eff_ptr ) const;
	void									FindTextureStagesNum	( );
	void									CopyShader				( const Shader& from );
};





/*! \class ShaderParams xfx_shader.h "render/xfx_shader.h"
 *	\brief %Shader constants class.
 *	\ingroup RenderGroup
 *
 *	User-defined constants for shader. The following types supported:
 *	- ITexture
 *	- float
 *	- int
 *	- Vec4
 *	- Mat4
 *
 *	\author Andrew "RevEn" Karpushin
 */

class ShaderParams
{
	friend class boost::serialization::access;

	stdext::hash_map< String, boost::shared_ptr< const ITexture >, HashCompare< String > >		mTextures;
	stdext::hash_map< String, float, HashCompare< String > >									mFloats;
	stdext::hash_map< String, int, HashCompare< String > >										mInts;
	stdext::hash_map< String, Vec4, HashCompare< String > >										mVectors;
	stdext::hash_map< String, Mat4, HashCompare< String > >										mMatrices;

#ifdef __XFX_USE_BOOST_SERIALIZATION__

	//
	//! Serialization
	//

	template< class _Archive >
	void serialize( _Archive & ar, const unsigned int version )
	{
		//ar & BOOST_SERIALIZATION_NVP( mTextures );
		//ar & BOOST_SERIALIZATION_NVP( mFloats );
		//ar & BOOST_SERIALIZATION_NVP( mInts );
		//ar & BOOST_SERIALIZATION_NVP( mVectors );
		//ar & BOOST_SERIALIZATION_NVP( mMatrices );
	};

#endif

public:
	//! Constructs empty shader constants container.
	ShaderParams										( ) { };

	//! Destructor.
	~ShaderParams										( ) { };

	//! Set a custom value by name.
	template<class T>
	void						SetValue				( const String& str, const T& value ) { _ASSERTE ( false ); };

	//! Get a custom value by name. Return false if value not exists.
	template< class T >
	bool						GetValue				( const String& str, T& value ) const { _ASSERTE( false ); return false; };

	//! Apply values on Effect. Needed just prior rendering.
	void						ApplyValues				( const Effect& eff ) const;

private:
	template< class _Container >
	bool						GetValueCommon			( const String& str, typename _Container::mapped_type& val, const _Container& cont ) const;
};




//
// ShaderParams specializations
//

//! Set a texture by name.
template<>
inline void ShaderParams::SetValue	( const String& str, const boost::shared_ptr< const ITexture >& value ) { mTextures[str] = value; };

//! Set a float value by name.
template<>
inline void ShaderParams::SetValue	( const String& str, const float& value ) { mFloats[ str ] = value; };

//! Set an int value by name.
template<>
inline void ShaderParams::SetValue	( const String& str, const int& value ) { mInts[ str ] = value; };

//! Set a Vec4 value by name.
template<>
inline void ShaderParams::SetValue	( const String& str, const Vec4& value ) { mVectors[ str ] = value; };

//! Set a Mat4 value by name.
template<>
inline void ShaderParams::SetValue	( const String& str, const Mat4& value ) { mMatrices[ str ] = value; };

//! Get a texture by name.
template< >
inline bool ShaderParams::GetValue ( const String& str, boost::shared_ptr< const ITexture >& value ) const { return GetValueCommon( str, value, mTextures ); };

//! Get a float value by name.
template< >
inline bool ShaderParams::GetValue ( const String& str, float& value ) const { return GetValueCommon( str, value, mFloats ); };

//! Get a int value by name.
template< >
inline bool ShaderParams::GetValue ( const String& str, int& value ) const { return GetValueCommon( str, value, mInts ); };;

//! Get a Vec4 value by name.
template< >
inline bool ShaderParams::GetValue ( const String& str, Vec4& value ) const { return GetValueCommon( str, value, mVectors ); };;

//! Get a Mat4 value by name.
template< >
inline bool ShaderParams::GetValue ( const String& str, Mat4& value ) const { return GetValueCommon( str, value, mMatrices ); };;






//
// Template methods
//

template< class _Container >
bool ShaderParams::GetValueCommon( const String& str, typename _Container::mapped_type& tex, const _Container& cont ) const
{
	typename _Container::const_iterator it = cont.find( str );

	if( it != cont.end( ) )
	{
		tex = ( *it ).second;
		return true;
	}

	return false;
}



_XFX_END

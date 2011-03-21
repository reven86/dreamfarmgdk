//  File xfx_drawtools.h                                                      |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_vertexbuffer.h"
#include "xfx_indexbuffer.h"

/*!	\file xfx_drawtools.h 
 *	\brief Drawing lines, sprites.
 */

_XFX_BEGIN






/*! \class DrawTools xfx_drawtools.h "render/xfx_drawtools.h"
 *	\brief Drawing class.
 *	\ingroup RenderGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class DrawTools
{
	struct LineVertex
	{
		Vec3	pos;
		ARGB	color;

		LineVertex		( ) { };
		LineVertex		( const Vec3& v, const ARGB& c ) : pos( v ), color( c ) { };
	};

public:
	//! Triangle vertex type.
	struct TriVertex
	{
		Vec3	pos;			//!< Position.
		Vec3	norm;			//!< Normal.
		ARGB	color;			//!< Color.
		float	tu;				//!< %Texture coordinate u.
		float	tv;				//!< %Texture coordinate v.

		TriVertex		( ) { };
		TriVertex		( const Vec3& ver, const Vec3& n, const ARGB& c, const float& u, const float& v ) :
			pos( ver ), norm( n ), color( c ), tu( u ), tv( v ) { };
	};

private:
	struct SpriteVertex : public Vec3
	{
		float	rhw;
		ARGB	color;
		float	tu, tv;

		SpriteVertex	( ) { };
		SpriteVertex	( const Vec3& v, const float& _rhw, const ARGB& c, const float& _tu, const float& _tv ) : Vec3( v ), color( c ), rhw( _rhw ), tu( _tu ), tv( _tv ) { };
	};

	struct TriBuffer
	{
		boost::shared_ptr< const class Shader >			shader;
		boost::shared_ptr< const class ShaderParams >	shader_params;
		int												offset;		//vertices offset
		int												count;		//tris count

		TriBuffer		( ) { };
		TriBuffer		( const boost::shared_ptr< const Shader >& shd, const boost::shared_ptr< const ShaderParams >& consts, const int& ofs, const int& c ) : shader( shd ), shader_params( consts ), offset( ofs ), count( c ) { };
	};

	typedef TriBuffer SpriteBuffer;			//it's same

	static const int FVF_LINEVERTEX		= D3DFVF_XYZ | D3DFVF_DIFFUSE;
	static const int FVF_TRIVERTEX		= D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const int FVF_SPRITEVERTEX	= D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	struct DrawChunkInfo
	{
		unsigned short	chunk_count : 15;
		bool			buffer_type : 1;		// true for tri, false for sprite

		DrawChunkInfo	( ) { };
		DrawChunkInfo	( const unsigned short& c, const bool& b ) : chunk_count( c ), buffer_type( b ) { };
	};

	std::vector< LineVertex >				mLines;
	std::vector< TriVertex >				mTris;
	std::vector< TriBuffer >				mTrisBuffer;
	std::vector< SpriteVertex >				mSpritesVerts;
	std::vector< SpriteBuffer >				mSpritesBuffer;
	std::vector< DrawChunkInfo >			mDrawChunks;

	unsigned								mLinesCount;
	unsigned								mTrisCount;
	unsigned								mTrisBufferCount;
	unsigned								mSpritesVertsCount;
	unsigned								mSpritesBufferCount;
	unsigned								mDrawChunksCount;

	VertexBuffer							mTrisVB;
	VertexBuffer							mSpritesVB;
	IndexBuffer								mSpritesIB;

public:
	//! Constructor.
	DrawTools														( );

	//! Destructor.
	~DrawTools														( );

	//! Allocate memory for buffers.
	void								Init						( );

	//! Free all allocated resources.
	void								Shutdown					( );

	//! \name Line drawing methods
	//! @{

	/*! \brief Draw solid color line.
	 *
	 *	\param[in]	v0		First vertex.
	 *	\param[in]	v1		Second vertex.
	 *	\param[in]	color	Color.
	 */
	void								PushDrawLine				( const Vec3& v0, const Vec3& v1, const ARGB& color );

	/*! \brief Draw gradient color line.
	 *
	 *	\param[in]	v0		First vertex.
	 *	\param[in]	v1		Second vertex.
	 *	\param[in]	color0	Color for first vertex.
	 *	\param[in]	color1	Color for second vertex.
	 */
	void								PushDrawLine				( const Vec3& v0, const Vec3& v1, const ARGB& color0, const ARGB& color1 );

	/*! \brief Draw axises, corresponding to matrix.
	 *
	 *	X - Red, Y - Green, Z - Blue.
	 *
	 *	\param[in]	m		Matrix.
	 *	\param[in]	len		Axis length.
	 */
	void								PushDrawAxises				( const Mat4& m, const float& len );

	/*! \brief Draw sphere.
	 *
	 *	\param[in]	origin	Origin.
	 *	\param[in]	radius	Radius.
	 *	\param[in]	color	Color.
	 */
	void								PushDrawSphere				( const Vec3& origin, const float& radius, const ARGB& color = ARGB( 0xff, 0xff, 0xff, 0xff ) );

	/*! \brief Draw AABB, transformed by matrix.
	 *
	 *	\param[in]	box		AABB.
	 *	\param[in]	color	Color.
	 *	\param[in]	matr	Transfromation.
	 */
	void								PushDrawBox					( const Primitives::AABB& box, const ARGB& color, const Mat4& matr = Mat4( 1 ) );

	/*! \brief Draw OBB.
	 *
	 *	\param[in]	box		OBB.
	 *	\param[in]	color	Color.
	 */
	void								PushDrawBox					( const Primitives::OBB& box, const ARGB& color );

	//! @}

	//! \name Triangle drawing in camera view space
	//! @{

	/*! \brief Draw a set of triangles.
	 *
	 *	\param[in]	numtris			Number of triangle.
	 *	\param[in]	vertices		TriVertex array.
	 *	\param[in]	shader			Shader.
	 *	\param[in]	shader_params	Shader user-define parameters.
	 */
	void								PushDrawTris				( int numtris, const TriVertex * vertices, const boost::shared_ptr< const class Shader >& shader, const boost::shared_ptr< const ShaderParams >& shader_params = boost::shared_ptr< const ShaderParams >( ) );

	/*! \brief Draw a 2D sprite.
	 *
	 *	\param[in]	x				X coordinate.
	 *	\param[in]	y				Y coordinate.
	 *	\param[in]	scalex			Scale for X.
	 *	\param[in]	scaley			Scale for Y.
	 *	\param[in]	roll			Roll.
	 *	\param[in]	color			Color.
	 *	\param[in]	shader			Shader.
	 *	\param[in]	shader_params	Shader user-defined parameters.
	 *	\param[in]	u1				First texture coordinate u.
	 *	\param[in]	v1				First texture coordinate v.
	 *	\param[in]	u2				Second texture coordinate u.
	 *	\param[in]	v2				Second texture coordinate v.
	 *	\param[in]	z				Output z coordinate.
	 *	\param[in]	rhw				Output RHW.
	 *	\param[in]	fixed_size		If roll is present, rotate sprite in fixed size, used in ParticleSystem.
	 *	\param[in]	vp				If \b true, coordinates are in viewport-space, not in render surface space.
	 */
	void								PushDraw2DSprite			( const float& x, const float& y, const float& scalex, const float& scaley, const Math::BigAngle& roll, const ARGB& color, const boost::shared_ptr< const class Shader >& shader, const boost::shared_ptr< const ShaderParams >& shader_params = boost::shared_ptr< const ShaderParams >( ), const float& u1 = 0.0f, const float& v1 = 0.0f, const float& u2 = 1.0f, const float& v2 = 1.0f, const float& z = 0.0f, const float& rhw = 1.0f, const bool& fixed_size = false, const bool& vp = false );	

	/*! \brief Draw a 3D sprite.
	 *
	 *	\param[in]	position		3D position.
	 *	\param[in]	scale			Scale both for X and Y.
	 *	\param[in]	roll			Roll.
	 *	\param[in]	color			Color.
	 *	\param[in]	shader			Shader.
	 *	\param[in]	shader_params	Shader user-defined parameters.
	 *	\param[in]	u1				First texture coordinate u.
	 *	\param[in]	v1				First texture coordinate v.
	 *	\param[in]	u2				Second texture coordinate u.
	 *	\param[in]	v2				Second texture coordinate v.
	 */
	void								PushDraw3DSprite			( const Vec3& position, const float& scale, const Math::BigAngle& roll, const ARGB& color, const boost::shared_ptr< const class Shader >& shader, const boost::shared_ptr< const ShaderParams >& shader_params = boost::shared_ptr< const ShaderParams >( ), const float& u1 = 0.0f, const float& v1 = 0.0f, const float& u2 = 1.0f, const float& v2 = 1.0f );

	/*! \brief Draw a 3D sprite.
	 *
	 *	\param[in]	position		3D position.
	 *	\param[in]	scalex			Scale for X.
	 *	\param[in]	scaley			Scale for Y.
	 *	\param[in]	roll			Roll.
	 *	\param[in]	color			Color.
	 *	\param[in]	shader			Shader.
	 *	\param[in]	shader_params	Shader user-defined parameters.
	 *	\param[in]	u1				First texture coordinate u.
	 *	\param[in]	v1				First texture coordinate v.
	 *	\param[in]	u2				Second texture coordinate u.
	 *	\param[in]	v2				Second texture coordinate v.
	 */
	void								PushDraw3DSprite			( const Vec3& position, const float& scalex, const float& scaley, const Math::BigAngle& roll, const ARGB& color, const boost::shared_ptr< const class Shader >& shader, const boost::shared_ptr< const ShaderParams >& shader_params = boost::shared_ptr< const ShaderParams >( ), const float& u1 = 0.0f, const float& v1 = 0.0f, const float& u2 = 1.0f, const float& v2 = 1.0f);

	//! @}

	//! \name Text drawing
	//! @{

	/*! \brief Draw a 2D text.
	 *
	 *	\param[in]	fnt				Font.
	 *	\param[in]	x				X coordinate.
	 *	\param[in]	y				Y coordinate.
	 *	\param[in]	xscale			X scale.
	 *	\param[in]	yscale			Y scale.
	 *	\param[in]	text			Text.
	 *	\param[in]	color			Color.
	 *	\param[in]	shader_params	Shader user-defined parameters.
	 *	\param[in]	z				Output z coordinate.
	 *	\param[in]	rhw				Output RHW.
	 *	\param[in]	vp				If \b true, coordinates are in viewport-space, not in render surface space.
	 */
	void								PushDraw2DText				( const class Font& fnt, const float& x, const float& y, const float& xscale, const float& yscale, const WString& text, const ARGB& color = ARGB( 0xff, 0xff, 0xff, 0xff ), const boost::shared_ptr< const ShaderParams >& shader_params = boost::shared_ptr< const ShaderParams >( ), const float& z = 0.0f, const float& rhw = 1.0f, const bool& vp = false );

	/*! \brief Draw a 2D text in 3D space.
	 *
	 *	\param[in]	fnt				Font.
	 *	\param[in]	pos				Position.
	 *	\param[in]	xscale			X scale.
	 *	\param[in]	yscale			Y scale.
	 *	\param[in]	text			Text.
	 *	\param[in]	color			Color.
	 *	\param[in]	shader_params	Shader user-defined parameters.
	 */
	void								PushDraw3DText				( const class Font& fnt, const Vec3& pos, const float& xscale, const float& yscale, const WString& text, const ARGB& color = ARGB( 0xff, 0xff, 0xff, 0xff ), const boost::shared_ptr< const ShaderParams >& shader_params = boost::shared_ptr< const ShaderParams >( ) );

	//! @}

	//! Flush lines.
	void								FlushLines					( );

	//! Flush tris and sprites in order they were pushed.
	void								FlushTrisAndSprites			( );

	//! Flush all.
	void								FlushAll					( ) { FlushLines( ); FlushTrisAndSprites( ); };
};





_XFX_END

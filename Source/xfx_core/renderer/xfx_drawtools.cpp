//  File xfx_drawtools.cpp                                                    |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_drawtools.h"
#include "xfx_renderer.h"
#include "xfx_shader.h"
#include "xfx_texture.h"
#include "xfx_font.h"
#include "xfx_renderer_cvars.h"

_XFX_BEGIN






//
// Draw tools
//

DrawTools::DrawTools( )
{
}

DrawTools::~DrawTools( )
{
}

void DrawTools::Init( )
{
	mLines.reset( new LineVertex[ r_maxlines->AsInt( ) ] );
	mTris.reset( new TriVertex[ r_maxtris->AsInt( ) * 4 ] );
	mTrisBuffer.reset( new TriBuffer[ r_maxtris->AsInt( ) ] );
	mSpritesVerts.reset( new SpriteVertex[ r_maxsprites->AsInt( ) * 4 ] );
	mSpritesBuffer.reset( new SpriteBuffer[ r_maxsprites->AsInt( ) ] );
	mDrawChunks.reset( new DrawChunkInfo[ r_maxsprites->AsInt( ) + r_maxtris->AsInt( ) ] );

	mLinesCount = 0;
	mTrisCount = 0;
	mTrisBufferCount = 0;
	mSpritesVertsCount = 0;
	mSpritesBufferCount = 0;
	mDrawChunksCount = 0;

	mLinesCapacity = r_maxlines->AsInt( );
	mTrisCapacity = r_maxtris->AsInt( ) * 4;
	mTrisBufferCapacity = r_maxtris->AsInt( );
	mSpritesVertsCapacity = r_maxsprites->AsInt( ) * 4;
	mSpritesBufferCapacity = r_maxsprites->AsInt( );
	mDrawChunksCapacity = r_maxsprites->AsInt( ) + r_maxtris->AsInt( );

	mTrisVB.Create (sizeof (TriVertex), r_maxtris->AsInt( ) * 6, FVF_TRIVERTEX, true);
	mSpritesVB.Create (sizeof (SpriteVertex), r_maxsprites->AsInt( ) * 4, FVF_SPRITEVERTEX, true);

	mSpritesIB.Create (false, r_maxsprites->AsInt( )* 6, false);

	//create indices for sprites
	{
		BufferLocker< IndexBuffer > bl( mSpritesIB, 0, r_maxsprites->AsInt( ) * 6 );
		WORD * ind = bl.data< WORD >( );
		_ASSERTE( ind );

		if( ind )
		{
			WORD vert = 0;
			for( int i = 0; i < r_maxsprites->AsInt( ); i++, vert += 4 )
			{
				*ind++ = vert + 0;
				*ind++ = vert + 1;
				*ind++ = vert + 2;
				*ind++ = vert + 2;
				*ind++ = vert + 1;
				*ind++ = vert + 3;
			}
		}
	}
}

void DrawTools::Shutdown( )
{
	mTrisVB.Destroy( );
	mSpritesVB.Destroy( );
	mSpritesIB.Destroy( );
}

void DrawTools::PushDrawLine( const Vec3& v0, const Vec3& v1, const ARGB& color )
{
	PushDrawLine( v0, v1, color, color );
}

void DrawTools::PushDrawLine (const Vec3& v0, const Vec3& v1, const ARGB& color0, const ARGB& color1)
{
	PROFILE( __FUNCTION__, "Render" );

#pragma message ( "FIXME: allocate buffer" )
	if( mLinesCount >= mLinesCapacity - 2 )
		return;

	mLines[ mLinesCount ++ ] = LineVertex( v0, color0 );
	mLines[ mLinesCount ++ ] = LineVertex( v1, color1 );
}

void DrawTools::PushDrawAxises (const Mat4& m, const float& len)
{
	Vec3 x (m.x[0][0], m.x[0][1], m.x[0][2]);
	Vec3 y (m.x[1][0], m.x[1][1], m.x[1][2]);
	Vec3 z (m.x[2][0], m.x[2][1], m.x[2][2]);
	Vec3 p (m.x[3][0], m.x[3][1], m.x[3][2]);

	PushDrawLine (p, p + x * len, 0xffff0000);
	PushDrawLine (p, p + y * len, 0xff00ff00);
	PushDrawLine (p, p + z * len, 0xff0000ff);
}

void DrawTools::PushDrawSphere (const Vec3& origin, const float& radius, const ARGB& color)
{
	struct point
	{
		float a, b;

		point (float _1, float _2) : a (_1), b (_2) {};
	};

	static point points[12] = {
		point (1.0f, 0.0f),
		point (0.866f, 0.5f),
		point (0.5f, 0.866f),
		point (0.0f, 1.0f),
		point (-0.5f, 0.866f),
		point (-0.866f, 0.5f),
		point (-1.0f, 0.0f),
		point (-0.866f, -0.5f),
		point (-0.5f, -0.866f),
		point (0.0f, -1.0f),
		point (0.5f, -0.866f),
		point (0.866f, -0.5f)
	};

	int i;

	const int num = sizeof (points) / sizeof (point);

	for (i = 0; i < num - 1; i++)
		PushDrawLine (Vec3 (points[i].a, points[i].b, 0.0f) * radius + origin, Vec3 (points[i + 1].a, points[i + 1].b, 0.0f) * radius + origin, color);
	PushDrawLine (Vec3 (points[num - 1].a, points[num - 1].b, 0.0f) * radius + origin, Vec3 (points[0].a, points[0].b, 0.0f) * radius + origin, color);

	for (i = 0; i < num - 1; i++)
		PushDrawLine (Vec3 (points[i].a, 0.0f, points[i].b) * radius + origin, Vec3 (points[i + 1].a, 0.0f, points[i + 1].b) * radius + origin, color);
	PushDrawLine (Vec3 (points[num - 1].a, 0.0f, points[num - 1].b) * radius + origin, Vec3 (points[0].a, 0.0f, points[0].b) * radius + origin, color);

	for (i = 0; i < num - 1; i++)
		PushDrawLine (Vec3 (0.0f, points[i].a, points[i].b) * radius + origin, Vec3 (0.0f, points[i + 1].a, points[i + 1].b) * radius + origin, color);
	PushDrawLine (Vec3 (0.0f, points[num - 1].a, points[num - 1].b) * radius + origin, Vec3 (0.0f, points[0].a, points[0].b) * radius + origin, color);
}

void DrawTools::PushDrawBox (const Primitives::AABB& box, const ARGB& color, const Mat4& m)
{
	Primitives::OBB obb (box);
	obb.Transform (m);

	PushDrawBox (obb, color);
}

void DrawTools::PushDrawBox (const Primitives::OBB& box, const ARGB& color)
{
	PushDrawLine (box.Location (), box.Location () + box.E1 (), color);
	PushDrawLine (box.Location (), box.Location () + box.E2 (), color);
	PushDrawLine (box.Location (), box.Location () + box.E3 (), color);
	PushDrawLine (box.Location () + box.E1 (), box.Location () + box.E1 () + box.E2 (), color);
	PushDrawLine (box.Location () + box.E1 (), box.Location () + box.E1 () + box.E3 (), color);
	PushDrawLine (box.Location () + box.E2 (), box.Location () + box.E2 () + box.E1 (), color);
	PushDrawLine (box.Location () + box.E2 (), box.Location () + box.E2 () + box.E3 (), color);
	PushDrawLine (box.Location () + box.E3 (), box.Location () + box.E3 () + box.E1 (), color);
	PushDrawLine (box.Location () + box.E3 (), box.Location () + box.E3 () + box.E2 (), color);
	PushDrawLine (box.Location () + box.E1 () + box.E2 () + box.E3 (), box.Location () + box.E1 () + box.E2 (), color);
	PushDrawLine (box.Location () + box.E1 () + box.E2 () + box.E3 (), box.Location () + box.E2 () + box.E3 (), color);
	PushDrawLine (box.Location () + box.E1 () + box.E2 () + box.E3 (), box.Location () + box.E3 () + box.E1 (), color);
}

void DrawTools::FlushLines ()
{
	PROFILE( __FUNCTION__, "Render" );

	if( mLinesCount > 0 )
	{
		DWORD blend		= Renderer::Instance( ).State( Renderer::SM_ALPHABLENDENABLE );
		DWORD srcblend	= Renderer::Instance( ).State( Renderer::SM_SRCBLEND );
		DWORD destblend	= Renderer::Instance( ).State( Renderer::SM_DESTBLEND );
		DWORD light		= Renderer::Instance( ).State( Renderer::SM_LIGHTING );

		Renderer::Instance( ).State( Renderer::SM_ALPHABLENDENABLE, true );
		Renderer::Instance( ).State( Renderer::SM_SRCBLEND, D3DBLEND_SRCALPHA );
		Renderer::Instance( ).State( Renderer::SM_DESTBLEND, D3DBLEND_INVSRCALPHA );
		Renderer::Instance( ).State( Renderer::SM_LIGHTING, false );

		Renderer::Instance( ).ApplyTexture( boost::shared_ptr< const ITexture >( ) );

		Renderer::Instance( ).pD3DDevice( )->SetTransform( D3DTS_WORLD, ( D3DMATRIX * )( &Mat4( 1 ) ) );

		Renderer::Instance( ).FVF( FVF_LINEVERTEX );
		Renderer::Instance( ).pD3DDevice( )->DrawPrimitiveUP( D3DPT_LINELIST, static_cast< UINT >( mLinesCount >> 1 ), mLines.get( ), sizeof( LineVertex ) );

		Renderer::Instance( ).State( Renderer::SM_ALPHABLENDENABLE, blend );
		Renderer::Instance( ).State( Renderer::SM_SRCBLEND, srcblend );
		Renderer::Instance( ).State( Renderer::SM_DESTBLEND, destblend );
		Renderer::Instance( ).State( Renderer::SM_LIGHTING, light );

		Renderer::Instance( ).FrameStatistics( ).num_lines += static_cast< unsigned >( mLinesCount >> 1 );

		mLinesCount = 0;
	}
}

void DrawTools::PushDrawTris( int numtris, const TriVertex * vertices, const boost::shared_ptr<const class Shader>& shader, const boost::shared_ptr< const ShaderConsts >& shader_consts )
{
	PROFILE( __FUNCTION__, "Render" );

	if( numtris * 3 + mTrisCount > mTrisCapacity )
	{
#pragma message ("FIXME: reallocate buffer")
		numtris = static_cast< int >( ( mTrisCapacity - mTrisCount ) / 3 );
		return;
	}

	if (numtris <= 0)
		return;

	const Shader * tri_ptr = mTrisBufferCount == 0 ? NULL : ( mTrisBuffer.get( ) + mTrisBufferCount - 1 )->shader.get( );

	const ShaderConsts * shd_consts_ptr = mTrisBufferCount == 0 ? NULL : ( mTrisBuffer.get( ) + mTrisBufferCount - 1 )->shader_consts.get( );

	if( ( mDrawChunksCount == 0 || mDrawChunks[ mDrawChunksCount - 1 ].buffer_type ) && 
		shader && tri_ptr && *shader == *tri_ptr && shd_consts_ptr == shader_consts.get( ) )
	{
		( mTrisBuffer.get( ) + mTrisBufferCount - 1 )->count += numtris;
	}
	else
	{
		if( mDrawChunksCount == 0 )
		{
			mDrawChunks[ mDrawChunksCount ].chunk_count		= 1;
			mDrawChunks[ mDrawChunksCount ].buffer_type		= true;
			mDrawChunksCount++;
		}
		else
		{
			if( mDrawChunks[ mDrawChunksCount - 1 ].buffer_type )
			{
				mDrawChunks[ mDrawChunksCount - 1 ].chunk_count++;
			}
			else
			{
				mDrawChunks[ mDrawChunksCount ].chunk_count	= 1;
				mDrawChunks[ mDrawChunksCount ].buffer_type	= true;
				mDrawChunksCount++;
			}
		}

		mTrisBuffer[ mTrisBufferCount ++ ] = TriBuffer( shader, shader_consts, static_cast< int >( mTrisCount ), numtris );
	}

	std::copy( vertices, vertices + 3 * numtris, mTris.get( ) + mTrisCount );

	if( shader )
	{
		const Shader::TextureInfo * tex_info = shader->GetTextureInfoOnStage( 0 );

		if( tex_info )
		{
			const ITexture * tex_ptr = tex_info->texture.get( );

			// this float comparison is ok
			if( tex_info->frame_width != 1.0f || tex_info->frame_height != 1.0f )
			{
				// sprite is animated
				float time = 0;
				int frame = 0;

				if ( tex_info->frame_count > 0 && shader_consts && 
					 shader_consts->GetValue( boost::str( StringFormat( "xfx_%s_time" ) % tex_info->stage_name.c_str( ) ), time )
					 )
				{
					frame = static_cast< int >( time * tex_info->fps ) % tex_info->frame_count;
				}

				float au0, av0, au1, av1;
				Texture::GetFrameTexCoords( au0, av0, au1, av1, frame, tex_info->frame_width, tex_info->frame_height );

				for( int i = 0; i < numtris * 3; i++ )
				{
					float& nu1 = ( mTris.get( ) + mTrisCount + i )->tu;
					float& nv1 = ( mTris.get( ) + mTrisCount + i )->tv;

					nu1 *= au1 - au0;
					nv1 *= av1 - av0;

					nu1 += au0;
					nv1 += av0;
				}
			}

			if( tex_ptr && !tex_ptr->IsIdentityResultTransform( ) )
			{
				Mat4 tex_mat = tex_ptr->TextureMatrix( ) * tex_ptr->Transformation( );

				for( int i = 0; i < numtris * 3; i++ )
				{
					float& nu1 = ( mTris.get( ) + mTrisCount + i )->tu;
					float& nv1 = ( mTris.get( ) + mTrisCount + i )->tv;

					float u1 = nu1;
					float v1 = nv1;

					nu1 = u1 * tex_mat._11 + v1 * tex_mat._21 + tex_mat._31;
					nv1 = u1 * tex_mat._12 + v1 * tex_mat._22 + tex_mat._33;
				}
			}
		}
	}

	mTrisCount += 3 * numtris;
}

void DrawTools::PushDraw2DSprite(
	const float& x, const float& y, const float& scalex, const float& scaley, const Math::BigAngle& roll, const ARGB& color,
	const boost::shared_ptr< const class Shader >& shader, const boost::shared_ptr< const ShaderConsts >& shader_consts,
	const float& u1, const float& v1, const float& u2, const float& v2, const float& z, const float& rhw,
	const bool& fixed_size
	)
{
	PROFILE( __FUNCTION__, "Render" );

#pragma message ("FIXME: reallocate buffer")
	if( mSpritesVertsCount + 4 >= mSpritesVertsCapacity )
	{
		return;
	}

	float dx = scalex * 0.5f;
	float dy = scaley * 0.5f;

	const Shader * spr_ptr = mSpritesBufferCount == 0 ? NULL : ( mSpritesBuffer.get( ) + mSpritesBufferCount - 1 )->shader.get( );
	const ShaderConsts * shd_consts_ptr = mSpritesBufferCount == 0 ? NULL : ( mSpritesBuffer.get( ) + mSpritesBufferCount - 1 )->shader_consts.get( );

	if( ( mDrawChunksCount == 0 || !mDrawChunks[ mDrawChunksCount - 1 ].buffer_type ) &&
		shader && spr_ptr && *shader == *spr_ptr && shd_consts_ptr == shader_consts.get( ) )
	{
		( mSpritesBuffer.get( ) + mSpritesBufferCount - 1 )->count++;
	}
	else
	{
		if( mDrawChunksCount == 0 )
		{
			mDrawChunks[ mDrawChunksCount ].chunk_count		= 1;
			mDrawChunks[ mDrawChunksCount ].buffer_type		= false;
			mDrawChunksCount++;
		}
		else
		{
			if( !mDrawChunks[ mDrawChunksCount - 1 ].buffer_type )
			{
				mDrawChunks[ mDrawChunksCount - 1 ].chunk_count++;
			}
			else
			{
				mDrawChunks[ mDrawChunksCount ].chunk_count	= 1;
				mDrawChunks[ mDrawChunksCount ].buffer_type	= false;
				mDrawChunksCount++;
			}
		}

		mSpritesBuffer[ mSpritesBufferCount ].shader		= shader;
		mSpritesBuffer[ mSpritesBufferCount ].shader_consts	= shader_consts;
		mSpritesBuffer[ mSpritesBufferCount ].offset		= static_cast< int >( mSpritesVertsCount );
		mSpritesBuffer[ mSpritesBufferCount ].count			= 1;
		++mSpritesBufferCount;
	}

	SpriteVertex * vtx[4];

	vtx[ 0 ] = &mSpritesVerts[ mSpritesVertsCount++ ];
	vtx[ 1 ] = &mSpritesVerts[ mSpritesVertsCount++ ];
	vtx[ 2 ] = &mSpritesVerts[ mSpritesVertsCount++ ];
	vtx[ 3 ] = &mSpritesVerts[ mSpritesVertsCount++ ];

	vtx[ 0 ]->z = vtx[ 1 ]->z = vtx[ 2 ]->z = vtx[ 3 ]->z = z;
	vtx[ 0 ]->color = vtx[ 1 ]->color = vtx[ 2 ]->color = vtx[ 3 ]->color = color;
	vtx[ 0 ]->rhw = vtx[ 1 ]->rhw = vtx[ 2 ]->rhw = vtx[ 3 ]->rhw = rhw;

	float nu1 = u1, nu2 = u2, nv1 = v1, nv2 = v2;

	if( shader )
	{
		const Shader::TextureInfo * tex_info = shader->GetTextureInfoOnStage( 0 );

		if( tex_info )
		{
			const ITexture * tex_ptr = tex_info->texture.get( );

			// this float comparison is ok
			if( tex_info->frame_width != 1.0f || tex_info->frame_height != 1.0f )
			{
				// sprite is animated
				float time = 0;
				int frame = 0;

				if ( tex_info->frame_count > 0 && shader_consts && 
					 shader_consts->GetValue( boost::str( StringFormat( "xfx_%s_time" ) % tex_info->stage_name.c_str( ) ), time )
					 )
				{
					frame = static_cast< int >( time * tex_info->fps ) % tex_info->frame_count;
				}

				float au0, av0, au1, av1;
				Texture::GetFrameTexCoords( au0, av0, au1, av1, frame, tex_info->frame_width, tex_info->frame_height );

				nu1 *= au1 - au0;
				nu2 *= au1 - au0;
				nv1 *= av1 - av0;
				nv2 *= av1 - av0;

				nu1 += au0;
				nu2 += au0;
				nv1 += av0;
				nv2 += av0;
			}

			if( tex_ptr && !tex_ptr->IsIdentityResultTransform( ) )
			{
				Mat4 tex_mat = tex_ptr->TextureMatrix( ) * tex_ptr->Transformation( );

				float u1 = nu1;
				float v1 = nv1;
				float u2 = nu2;
				float v2 = nv2;

				nu1 = u1 * tex_mat._11 + v1 * tex_mat._21 + tex_mat._31;
				nv1 = u1 * tex_mat._12 + v1 * tex_mat._22 + tex_mat._32;

				nu2 = u2 * tex_mat._11 + v2 * tex_mat._21 + tex_mat._31;
				nv2 = u2 * tex_mat._12 + v2 * tex_mat._22 + tex_mat._32;
			}
		}
	}

	vtx[0]->tu = vtx[2]->tu = nu1;
	vtx[1]->tu = vtx[3]->tu = nu2;
	vtx[0]->tv = vtx[1]->tv = nv1;
	vtx[2]->tv = vtx[3]->tv = nv2;

	if (roll == 0)
	{
#pragma message ( "FIXME: use different flag" )
		float x1 = fixed_size ? x - dx : static_cast< float >( static_cast< int >( x - dx ) - 0.5f );
		float x2 = fixed_size ? x + dx : static_cast< float >( static_cast< int >( x + dx ) - 0.5f );
		float y1 = fixed_size ? y - dy : static_cast< float >( static_cast< int >( y - dy ) - 0.5f );
		float y2 = fixed_size ? y + dy : static_cast< float >( static_cast< int >( y + dy ) - 0.5f );

		vtx[0]->x = x1;
		vtx[0]->y = y1;
		vtx[1]->x = x2;
		vtx[1]->y = y1;
		vtx[2]->x = x1;
		vtx[2]->y = y2;
		vtx[3]->x = x2;
		vtx[3]->y = y2;
	}
	else
	{
		float dx_x, dx_y, dy_x, dy_y;

		float cs = math_cos( roll );
		float sn = math_sin( roll );

		if( fixed_size )
		{
			dx_x = dx * cs;
			dx_y = dy * sn;
			dy_x = -dx * sn;
			dy_y = dy * cs;
		}
		else
		{
			dx_x = dx * cs;
			dx_y = dx * sn;
			dy_x = -dy * sn;
			dy_y = dy * cs;
		}

		vtx[0]->x = static_cast< float >( static_cast< int >( x - dx_x - dy_x ) + 0.5f );
		vtx[0]->y = static_cast< float >( static_cast< int >( y - dx_y - dy_y ) + 0.5f );

		vtx[1]->x = static_cast< float >( static_cast< int >( x + dx_x - dy_x ) + 0.5f );
		vtx[1]->y = static_cast< float >( static_cast< int >( y + dx_y - dy_y ) + 0.5f );

		vtx[2]->x = static_cast< float >( static_cast< int >( x - dx_x + dy_x ) + 0.5f );
		vtx[2]->y = static_cast< float >( static_cast< int >( y - dx_y + dy_y ) + 0.5f );

		vtx[3]->x = static_cast< float >( static_cast< int >( x + dx_x + dy_x ) + 0.5f );
		vtx[3]->y = static_cast< float >( static_cast< int >( y + dx_y + dy_y ) + 0.5f );
	}
}

void DrawTools::PushDraw3DSprite( 
	const Vec3& position, const float& scale, const Math::BigAngle& roll, const ARGB& color,
	const boost::shared_ptr< const class Shader >& shader,	const boost::shared_ptr< const ShaderConsts >& shader_consts,
	const float& u1, const float& v1, const float& u2, const float& v2
	)
{
	PushDraw3DSprite (position, scale, scale, roll, color, shader, shader_consts, u1, v1, u2, v2);
}

void DrawTools::PushDraw3DSprite(
	const Vec3& position, const float& scalex, const float& scaley, const Math::BigAngle& roll, const ARGB& color,
	const boost::shared_ptr< const class Shader >& shader, const boost::shared_ptr< const ShaderConsts >& shader_consts,
	const float& u1, const float& v1, const float& u2, const float& v2
	)
{
	float sx, sy, sz;
	float rhw;

	const Mat4& t = Renderer::Instance ().GetVP ();

	rhw = 1.0f / (position.x * t._14 + position.y * t._24 + position.z * t._34 + t._44);
	sz = rhw * (position.x * t._13 + position.y * t._23 + position.z * t._33 + t._43);

	if ((sz >= 1.0f) || (sz < 0.0f))
		return;

	D3DVIEWPORT8 vp;
	Renderer::Instance( ).GetViewport (vp);

	sx = rhw * (position.x * t._11 + position.y * t._21 + position.z * t._31 + t._41);
	sy = rhw * (position.x * t._12 + position.y * t._22 + position.z * t._32 + t._42);
	sx = vp.X + (sx + 1.0f) * vp.Width * 0.5f;
	sy = vp.Y + (1.0f - sy) * vp.Height * 0.5f;

	Mat4 proj;
	Renderer::Instance ().GetTransform (D3DTS_PROJECTION, proj);

	float sx2 = vp.Width * 0.5f * rhw * (proj._11 + proj._41);
	float newscalex = scalex * sx2;
	float newscaley = scaley * sx2;

	PushDraw2DSprite( sx, sy, newscalex * 2.0f, newscaley * 2.0f, roll, color, shader, shader_consts, u1, v1, u2, v2, sz, rhw );
}

void DrawTools::PushDraw2DText(
	const Font& fnt, const float& x, const float& y, const float& xscale, const float& yscale,
	const WString& text, const ARGB& color, const boost::shared_ptr< const ShaderConsts >& shader_consts, 
	const float& z, const float& rhw
	)
{
	PROFILE( __FUNCTION__, "Render" );

	struct visitor
	{
		DrawTools										* obj;
		float											x;
		float											y;
		ARGB											color;
		const boost::shared_ptr< const Shader >&			shader_ptr;
		const boost::shared_ptr< const ShaderConsts >&	shader_consts;
		const float&									z;
		const float&									rhw;
		const float&									xscale;
		const float&									yscale;

		visitor								(
			DrawTools * _o, const float& _x, const float& _y, const float& xs, 
			const float& ys, const ARGB& _color,
			const boost::shared_ptr< const Shader >& _shd, const boost::shared_ptr< const ShaderConsts >& _consts,
			const float& _z, const float& _rhw
			) : obj( _o ), x( _x ), y( _y ), xscale( xs ), yscale( ys ), color( _color ), shader_ptr( _shd ), shader_consts( _consts ), z( _z ), rhw( _rhw ) { };

		void operator ( )					( const float& ch_x, const float& ch_y, const float& w, const float& h, const float& u1, const float& v1, const float& u2, const float& v2 ) const
		{
			obj->PushDraw2DSprite( x + ch_x * xscale, y + ch_y * yscale, w * xscale, h * yscale, 0, color, shader_ptr, shader_consts, u1, v1, u2, v2, z, rhw );
		};
	};

	fnt.FormatString( text, text.length( ),
		visitor( this, x, y, xscale, yscale, color, fnt.ShaderPtr( ), shader_consts, z, rhw )
		);
}

void DrawTools::PushDraw3DText(
	const Font& fnt, const Vec3& position, const float& xscale, const float& yscale,
	const WString& text, const ARGB& color, const boost::shared_ptr< const ShaderConsts >& shader_consts )
{
	float sx, sy, sz;
	float rhw;

	const Mat4& t = Renderer::Instance( ).GetVP( );

	rhw = 1.0f / ( position.x * t._14 + position.y * t._24 + position.z * t._34 + t._44 );
	sz = rhw * ( position.x * t._13 + position.y * t._23 + position.z * t._33 + t._43 );

	if( ( sz >= 1.0f ) || ( sz < 0.0f ) )
		return;

	D3DVIEWPORT8 vp;
	Renderer::Instance( ).GetViewport( vp );

	sx = rhw * (position.x * t._11 + position.y * t._21 + position.z * t._31 + t._41);
	sy = rhw * (position.x * t._12 + position.y * t._22 + position.z * t._32 + t._42);
	sx = vp.X + (sx + 1.0f) * vp.Width * 0.5f;
	sy = vp.Y + (1.0f - sy) * vp.Height * 0.5f;

	PushDraw2DText( fnt, sx, sy, xscale, yscale, text, color, shader_consts, sz, rhw );
}

void DrawTools::FlushTrisAndSprites( )
{
	PROFILE( __FUNCTION__, "Render" );

	if( mTrisCount != 0 )
	{
		BufferLocker< VertexBuffer > bl( mTrisVB, 0, static_cast< unsigned >( mTrisCount ) );

		if( bl.data< void >( ) )
			memcpy( bl.data< void >( ), mTris.get( ), sizeof( TriVertex ) * mTrisCount );

		Renderer::Instance( ).pD3DDevice( )->SetTransform( D3DTS_WORLD, ( D3DMATRIX * )( &Mat4( 1 ) ) );
	}

	if( mSpritesVertsCount != 0 )
	{
		BufferLocker< VertexBuffer > bl( mSpritesVB, 0, static_cast< unsigned >( mSpritesVertsCount ) );

		if( bl.data< void >( ) )
			memcpy( bl.data< void >( ), mSpritesVerts.get( ), sizeof( SpriteVertex ) * mSpritesVertsCount );

		mSpritesIB.SetIndices ();
	}

	int sprites_buffer_index = 0;
	int tris_buffer_index = 0;

	ShaderConsts def_consts;

	struct tri_render_fn
	{
		UINT offset;
		UINT p_count;

		void operator( )			( ) const
		{
			Renderer::Instance( ).DrawPrimitive( D3DPT_TRIANGLELIST, offset, p_count );
		};
	};

	struct sprite_render_fn
	{
		UINT min_index;
		UINT num_verts;
		UINT start_index;
		UINT p_count;

		void operator( )			( ) const
		{
#if (__XFX_DIRECTX_VER__ < 9)
			Renderer::Instance( ).DrawIndexedPrimitive( D3DPT_TRIANGLELIST, min_index, num_verts, start_index, p_count );
#else
			Renderer::Instance( ).DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, min_index, num_verts, start_index, p_count );
#endif
		};
	};

	sprite_render_fn srfn;
	tri_render_fn trfn;

	for( int i = 0; i < mDrawChunksCount; i++ )
	{
		if( mDrawChunks[ i ].buffer_type )
		{
			mTrisVB.SetStream( 0 );

			for( int j = 0; j < mDrawChunks[ i ].chunk_count; j++ )
			{
				trfn.offset = mTrisBuffer[ tris_buffer_index + j ].offset;
				trfn.p_count = mTrisBuffer[ tris_buffer_index + j ].count;

				Renderer::Instance( ).RenderPrimitive(
					mTrisBuffer[ tris_buffer_index + j ].shader,
					mTrisBuffer[ tris_buffer_index + j ].shader_consts ? *mTrisBuffer[ tris_buffer_index + j ].shader_consts : def_consts,
					trfn
					);

				Renderer::Instance ().FrameStatistics ().num_usertris += mTrisBuffer[ tris_buffer_index + j ].count;
			}

			tris_buffer_index += mDrawChunks[ i ].chunk_count;
		}
		else
		{
			mSpritesVB.SetStream( 0 );

			for( int j = 0; j < mDrawChunks[ i ].chunk_count; j++ )
			{
				srfn.min_index = mSpritesBuffer[ sprites_buffer_index + j ].offset;
				srfn.num_verts = mSpritesBuffer[ sprites_buffer_index + j ].count << 2;
				srfn.start_index = ( mSpritesBuffer[ sprites_buffer_index + j ].offset >> 1 ) * 3;
				srfn.p_count = mSpritesBuffer[ sprites_buffer_index + j ].count << 1;

				Renderer::Instance( ).RenderPrimitive(
					mSpritesBuffer[ sprites_buffer_index + j ].shader,
					mSpritesBuffer[ sprites_buffer_index + j ].shader_consts ? *mSpritesBuffer[ sprites_buffer_index + j ].shader_consts : def_consts,
					srfn
					);

				Renderer::Instance ().FrameStatistics ().num_sprites += mSpritesBuffer[ sprites_buffer_index + j ].count;
			}

			sprites_buffer_index += mDrawChunks[ i ].chunk_count;
		}
	}

	mDrawChunksCount = 0;
	mTrisCount = 0;
	mTrisBufferCount = 0;
	mSpritesVertsCount = 0;
	mSpritesBufferCount = 0;
}






_XFX_END
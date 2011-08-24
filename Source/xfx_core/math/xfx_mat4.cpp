//  File xfx_mat4.cpp                                                         |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_mat4.h"
#include "xfx_euler.h"

_XFX_BEGIN





//
// Mat4
//

Mat4::Mat4 (float v)
{
	x[0][0] = v; x[0][1] = 0.0f; x[0][2] = 0.0f; x[0][3] = 0.0f;
	x[1][0] = 0.0f; x[1][1] = v; x[1][2] = 0.0f; x[1][3] = 0.0f;
	x[2][0] = 0.0f; x[2][1] = 0.0f; x[2][2] = v; x[2][3] = 0.0f;
	x[3][0] = 0.0f; x[3][1] = 0.0f; x[3][2] = 0.0f; x[3][3] = 1.0f;
}

Mat4::Mat4 (float a_11, float a_12, float a_13, float a_14,
				float a_21, float a_22, float a_23, float a_24,
				float a_31, float a_32, float a_33, float a_34,
				float a_41, float a_42, float a_43, float a_44)
{
	x[0][0] = a_11; x[0][1] = a_12; x[0][2] = a_13; x[0][3] = a_14;
	x[1][0] = a_21; x[1][1] = a_22; x[1][2] = a_23; x[1][3] = a_24;
	x[2][0] = a_31; x[2][1] = a_32; x[2][2] = a_33; x[2][3] = a_34;
	x[3][0] = a_41; x[3][1] = a_42; x[3][2] = a_43; x[3][3] = a_44;
}

Mat4::Mat4 (const Vec3& pos, const Euler& orientation, const Vec3& scale)
{
	Reset (pos, orientation, scale);
}

Mat4& Mat4::operator *= (const Mat4& A)
{
	Mat4 res = *this;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			float sum = 0;
			for (int k = 0; k < 4; k++)
				sum += res.x[i][k] * A.x[k][j];
			x[i][j] = sum;
		}

	return *this;
}

Mat4 operator * (const Mat4& A, const Mat4 & B)
{
	Mat4 out;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			float sum = 0;
			for (int k = 0; k < 4; k++)
				sum += A.x[i][k] * B.x[k][j];
			out.x[i][j] = sum;
		}

	return out;
}

Mat4& Mat4::MakeIdentity ()
{
	x[0][0] = 1; x[0][1] = 0; x[0][2] = 0; x[0][3] = 0;
	x[1][0] = 0; x[1][1] = 1; x[1][2] = 0; x[1][3] = 0;
	x[2][0] = 0; x[2][1] = 0; x[2][2] = 1; x[2][3] = 0;
	x[3][0] = 0; x[3][1] = 0; x[3][2] = 0; x[3][3] = 1;

	return *this;
}

bool Mat4::IsIdentity( ) const
{
	const float eps = 0.01f;

	for( int i = 0; i < 4; i++ )
		for( int j = 0; j < 4; j++ )
			if( i == j && fabs( x[ i ][ j ] - 1.0f ) > eps ||
				i != j && fabs( x[ i ][ j ] ) > eps )
			{
				return false;
			}

	return true;
}

Mat4& Mat4::Translate (const Vec3& loc)
{
	x[0][0] = 1; x[0][1] = 0; x[0][2] = 0; x[0][3] = 0;
	x[1][0] = 0; x[1][1] = 1; x[1][2] = 0; x[1][3] = 0;
	x[2][0] = 0; x[2][1] = 0; x[2][2] = 1; x[2][3] = 0;
	x[3][0] = loc.x; x[3][1] = loc.y; x[3][2] = loc.z; x[3][3] = 1;

	return *this;
}

Mat4& Mat4::MakeScale (const Vec3& v)
{
	x[0][0] = v.x; x[0][1] = 0; x[0][2] = 0; x[0][3] = 0;
	x[1][0] = 0; x[1][1] = v.y; x[1][2] = 0; x[1][3] = 0;
	x[2][0] = 0; x[2][1] = 0; x[2][2] = v.z; x[2][3] = 0;
	x[3][0] = 0; x[3][1] = 0; x[3][2] = 0; x[3][3] = 1;

	return *this;
}

Mat4& Mat4::Reset (const Vec3& pos, const Euler& orientation, const Vec3& scale)
{
	*this = orientation.ToMat4 ();
	SetScale (scale);
	SetTranslation (pos);

	return *this;;
}

Mat4& Mat4::MakeRotationX (Math::BigAngle angle)
{
	float Cosine	= Math::Instance ().Cos (angle);
	float Sine		= Math::Instance ().Sin (angle);

	x[0][0] = 1; x[0][1] = 0; x[0][2] = 0; x[0][3] = 0;
	x[1][0] = 0; x[1][1] = Cosine; x[1][2] = Sine; x[1][3] = 0;
	x[2][0] = 0; x[2][1] = -Sine; x[2][2] = Cosine; x[2][3] = 0;
	x[3][0] = 0; x[3][1] = 0; x[3][2] = 0; x[3][3] = 1;

	return *this;
}

Mat4& Mat4::MakeRotationY (Math::BigAngle angle)
{
	float Cosine	= Math::Instance ().Cos (angle);
	float Sine		= Math::Instance ().Sin (angle);

	x[0][0] = Cosine; x[0][1] = 0; x[0][2] = -Sine; x[0][3] = 0;
	x[1][0] = 0; x[1][1] = 1; x[1][2] = 0; x[1][3] = 0;
	x[2][0] = Sine; x[2][1] = 0; x[2][2] = Cosine; x[2][3] = 0;
	x[3][0] = 0; x[3][1] = 0; x[3][2] = 0; x[3][3] = 1;

	return *this;
}

Mat4& Mat4::MakeRotationZ (Math::BigAngle angle)
{
	float Cosine	= Math::Instance ().Cos (angle);
	float Sine		= Math::Instance ().Sin (angle);

	x[0][0] = Cosine; x[0][1] = Sine; x[0][2] = 0; x[0][3] = 0;
	x[1][0] = -Sine; x[1][1] = Cosine; x[1][2] = 0; x[1][3] = 0;
	x[2][0] = 0; x[2][1] = 0; x[2][2] = 1; x[2][3] = 0;
	x[3][0] = 0; x[3][1] = 0; x[3][2] = 0; x[3][3] = 1;

	return *this;
}

Mat4& Mat4::MakeRotationAxis (const Vec3& Axis, float Cosine, float Sine)
{
	Vec3 axis (Axis.GetNormalized ());

	x[0][0] = axis.x * axis.x + (1 - axis.x * axis.x) * Cosine;
	x[0][1] = axis.x * axis.y * (1 - Cosine) + axis.z * Sine;
	x[0][2] = axis.x * axis.z * (1 - Cosine) - axis.y * Sine;
	x[0][3] = 0;
	x[1][0] = axis.x * axis.y * (1 - Cosine) - axis.z * Sine;
	x[1][1] = axis.y * axis.y + (1 - axis.y * axis.y) * Cosine;
	x[1][2] = axis.y * axis.z * (1 - Cosine) + axis.x * Sine;
	x[1][3] = 0;
	x[2][0] = axis.x * axis.z * (1 - Cosine) + axis.y * Sine;
	x[2][1] = axis.y * axis.z * (1 - Cosine) - axis.x * Sine;
	x[2][2] = axis.z * axis.z + (1 - axis.z * axis.z) * Cosine;
	x[2][3] = 0;
	x[3][0] = 0;
	x[3][1] = 0;
	x[3][2] = 0;
	x[3][3] = 1;

	return *this;
}

Mat4 Mat4::GetTransposed () const
{
	Mat4 res;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			res.x[i][j] = x[j][i];

	return res;
}

Mat4& Mat4::PerspectiveFovLH (float fov, float aspect, float znear, float zfar)
{
	float t		= 1.0f / tanf (fov * 0.5f);
	float kz	= zfar / (zfar - znear);

	x[0][0] = t / aspect;
	x[0][1] = 0.0f;
	x[0][2] = 0.0f;
	x[0][3] = 0.0f;
	x[1][0] = 0.0f;
	x[1][1] = t;
	x[1][2] = 0.0f;
	x[1][3] = 0.0f;
	x[2][0] = 0.0f;
	x[2][1] = 0.0f;
	x[2][2] = kz;
	x[2][3] = 1.0f;
	x[3][0] = 0.0f;
	x[3][1] = 0.0f;
	x[3][2] = -znear * kz;
	x[3][3] = 0.0f;

	return *this;
}

Mat4& Mat4::PerspectiveFovRH (float fov, float aspect, float znear, float zfar)
{
	float t		= 1.0f / tanf (fov * 0.5f);
	float kz	= zfar / (zfar - znear);

	x[0][0] = t / aspect;
	x[0][1] = 0.0f;
	x[0][2] = 0.0f;
	x[0][3] = 0.0f;
	x[1][0] = 0.0f;
	x[1][1] = t;
	x[1][2] = 0.0f;
	x[1][3] = 0.0f;
	x[2][0] = 0.0f;
	x[2][1] = 0.0f;
	x[2][2] = -kz;
	x[2][3] = -1.0f;
	x[3][0] = 0.0f;
	x[3][1] = 0.0f;
	x[3][2] = -znear * kz;
	x[3][3] = 0.0f;

	return *this;
}

Mat4& Mat4::LookAtLH (const Vec3& cam, const Vec3& dir, const Vec3& vy)
{
	Vec3 normdir	(dir.GetNormalized ());
	Vec3 normvy		(vy.GetNormalized ());
	Vec3 normvx		(Vec3::Cross (normvy, normdir));

	x[0][0] = normvx.x;
	x[0][1] = normvy.x;
	x[0][2] = normdir.x;
	x[0][3] = 0.0f;
	x[1][0] = normvx.y;
	x[1][1] = normvy.y;
	x[1][2] = normdir.y;
	x[1][3] = 0.0f;
	x[2][0] = normvx.z;
	x[2][1] = normvy.z;
	x[2][2] = normdir.z;
	x[2][3] = 0.0f;
	x[3][0] = -Vec3::Dot (normvx, cam);
	x[3][1] = -Vec3::Dot (normvy, cam);
	x[3][2] = -Vec3::Dot (normdir, cam);
	x[3][3] = 1.0f;

	return *this;
}

Mat4& Mat4::LookAtRH (const Vec3& cam, const Vec3& dir, const Vec3& vy)
{
	Vec3 normdir	(-dir.GetNormalized ());
	Vec3 normvx		(Vec3::Cross (vy, normdir).GetNormalized( ));
	Vec3 normvy		(Vec3::Cross (normdir, normvx));

	x[0][0] = normvx.x;
	x[0][1] = normvy.x;
	x[0][2] = normdir.x;
	x[0][3] = 0.0f;
	x[1][0] = normvx.y;
	x[1][1] = normvy.y;
	x[1][2] = normdir.y;
	x[1][3] = 0.0f;
	x[2][0] = normvx.z;
	x[2][1] = normvy.z;
	x[2][2] = normdir.z;
	x[2][3] = 0.0f;
	x[3][0] = -Vec3::Dot (normvx, cam);
	x[3][1] = -Vec3::Dot (normvy, cam);
	x[3][2] = -Vec3::Dot (normdir, cam);
	x[3][3] = 1.0f;

	return *this;
}

Mat4 Mat4::GetInversed () const
{
	Mat4 A (*this);
	Mat4 out (1);

	for (int i = 0; i < 4; i++)
	{
		float d = A.x[i][i];
		if (d != 1.0f)
		{
			d = 1.0f / d;

			for (int j = 0; j < 4; j++)
			{
				out.x[i][j]	*= d;
				A.x[i][j]	*= d;
			}
		}

		for (int j = 0; j < 4; j++)
			if (j != i)
				if (A.x[j][i] != 0.0f)
				{
					float mulby = A.x[j][i];
					for (int k = 0; k < 4; k++)
					{
						A.x[j][k]	-= mulby * A.x[i][k];
						out.x[j][k]	-= mulby * out.x[i][k];
					}
				}
	}

	return out;
}

Mat4 Mat4::GetInversedAffine () const
{
	Mat4 res;

	float kx = 1.0f / ( Vec3( x[ 0 ][ 0 ], x[ 0 ][ 1 ], x[ 0 ][ 2 ] ).LenSq( ) );
	float ky = 1.0f / ( Vec3( x[ 1 ][ 0 ], x[ 1 ][ 1 ], x[ 1 ][ 2 ] ).LenSq( ) );
	float kz = 1.0f / ( Vec3( x[ 2 ][ 0 ], x[ 2 ][ 1 ], x[ 2 ][ 2 ] ).LenSq( ) );

	res.x[ 0 ][ 0 ] = x[ 0 ][ 0 ] * kx;
	res.x[ 1 ][ 0 ] = x[ 0 ][ 1 ] * kx;
	res.x[ 2 ][ 0 ] = x[ 0 ][ 2 ] * kx;
	res.x[ 0 ][ 1 ] = x[ 1 ][ 0 ] * ky;
	res.x[ 1 ][ 1 ] = x[ 1 ][ 1 ] * ky;
	res.x[ 2 ][ 1 ] = x[ 1 ][ 2 ] * ky;
	res.x[ 0 ][ 2 ] = x[ 2 ][ 0 ] * kz;
	res.x[ 1 ][ 2 ] = x[ 2 ][ 1 ] * kz;
	res.x[ 2 ][ 2 ] = x[ 2 ][ 2 ] * kz;

	res.x[ 0 ][ 3 ] = res.x[ 1 ][ 3 ] = res.x[ 2 ][ 3 ] = 0.0f;
	res.x[ 3 ][ 3 ] = 1.0f;

	res.x[ 3 ][ 0 ] = -( x[ 0 ][ 0 ] * x[ 3 ][ 0 ] + x[ 0 ][ 1 ] * x[ 3 ][ 1 ] + x[ 0 ][ 2 ] * x[ 3 ][ 2 ]) * kx;
	res.x[ 3 ][ 1 ] = -( x[ 1 ][ 0 ] * x[ 3 ][ 0 ] + x[ 1 ][ 1 ] * x[ 3 ][ 1 ] + x[ 1 ][ 2 ] * x[ 3 ][ 2 ]) * ky;
	res.x[ 3 ][ 2 ] = -( x[ 2 ][ 0 ] * x[ 3 ][ 0 ] + x[ 2 ][ 1 ] * x[ 3 ][ 1 ] + x[ 2 ][ 2 ] * x[ 3 ][ 2 ]) * kz;

	return res;
}

Vec3 Mat4::TransformCoord (const Vec3& v) const
{
	Vec3 out;

	out.x = v.x * x[0][0] + v.y * x[1][0] + v.z * x[2][0] + x[3][0];
	out.y = v.x * x[0][1] + v.y * x[1][1] + v.z * x[2][1] + x[3][1];
	out.z = v.x * x[0][2] + v.y * x[1][2] + v.z * x[2][2] + x[3][2];

	if (x[3][3] != 1.0f)
		if (x[3][3] != 0.0f)
			out /= x[3][3];

	return out;
}

Vec3 Mat4::TransformNormal (const Vec3& v) const
{
	Vec3 out;

	out.x = v.x * x[0][0] + v.y * x[1][0] + v.z * x[2][0];
	out.y = v.x * x[0][1] + v.y * x[1][1] + v.z * x[2][1];
	out.z = v.x * x[0][2] + v.y * x[1][2] + v.z * x[2][2];

	return out;
}

Mat4& Mat4::SetScale (const Vec3& v)
{
	float kx = v.x / (Vec3 (_11, _21, _31).Len ());
	float ky = v.y / (Vec3 (_12, _22, _32).Len ());
	float kz = v.z / (Vec3 (_13, _23, _33).Len ());

	_11 *= kx; _12 *= ky; _13 *= kz;
	_21 *= kx; _22 *= ky; _23 *= kz;
	_31 *= kx; _32 *= ky; _33 *= kz;

	return *this;
}



_XFX_END
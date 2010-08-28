//  File xfx_primitives.h                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_vec3.h"
#include "xfx_mat4.h"

/*!	\file xfx_primitives.h 
 *	\brief Geometry primitives.
 */

_XFX_BEGIN






/*! \class Ray xfx_primitives.h "math/xfx_primitives.h"
 *	\brief %Ray class.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Ray
{
	Vec3		mOrigin;
	Vec3		mDirection;		//not neccessary normalized

	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	//! Constructs a ray with origin (0.0, 0.0, 0.0) and direction (0.0, 0.0, 1.0).
	Ray													( ) : mOrigin ( 0.0f ), mDirection ( 0.0f, 0.0f, 1.0f ) { };

	//! Constructs a ray with custom origin and direction.
	Ray													( const Vec3& origin, const Vec3& direction ) : mOrigin ( origin ), mDirection ( direction ) { };

	//! Set origin.
	void						SetOrigin				( const Vec3& origin ) { mOrigin = origin; };

	//! Get origin.
	const Vec3&					Origin					( ) const { return mOrigin; };

	//! Get direction.
	const Vec3&					Direction				( ) const { return mDirection; };

	//! Set direction.
	void						SetDirection			( const Vec3& dir ) { mDirection = dir.GetNormalized( ); };

	//! Get a point on ray with distance t.
	Vec3						Point					( float t ) const { return mOrigin + t * mDirection; };

	//! Transform ray by matrix.
	void						Transform				( const Mat4& m );
};




/*! \class Primitive xfx_primitives.h "math/xfx_primitives.h"
 *	\brief Base primitive class.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Primitive
{
public:
	/*! \brief Project primitive to axis.

	 *	\param[out]	a	First project point.
	 *	\param[out] b	Second project point.
	 *	\param[in] axis	Axis.
	 */
	virtual void Projection		( float& a, float& b, const Vec3& axis ) const = 0;
};



//Primitives
namespace Primitives
{



/*! \class Sphere xfx_primitives.h "math/xfx_primitives.h"
 *	\brief %Sphere class.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Sphere : public Primitive
{
	Vec3		mPosition;
	float		mRadius;

	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	//! Constructs identity sphere.
	Sphere						( ) : mPosition( 0.0f ), mRadius( 1.0f ) { };

	//! Constructs custom sphere.
	Sphere						( const Vec3& pos, float r ) : mPosition( pos ), mRadius( r ) { };

	//! Set origin.
	void SetPosition			( const Vec3& p ) { mPosition = p; };

	//! Get origin.
	const Vec3& Position		( ) const { return mPosition; };

	//! Set radius.
	void SetRadius				( float r ) { mRadius = r; };

	//! Get radius.
	const float& Radius			( ) const { return mRadius; };

	/*! \brief Project primitive to axis.

	 *	\param[out]	a	First project point.
	 *	\param[out] b	Second project point.
	 *	\param[in] axis	Axis.
	 */
	virtual void Projection		( float& a, float& b, const Vec3& axis ) const;
};



/*! \struct Triangle xfx_primitives.h "math/xfx_primitives.h"
 *	\brief %Triangle class.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

struct Triangle : public Primitive
{
	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

	//! Triangle vertices array.
	Vec3		vertices[3];


	//! Constructs empty triangle.
	Triangle							( ) { vertices[ 0 ].xyz( 0, 0, 0 ); vertices[ 1 ].xyz( 0, 0, 0 ); vertices[ 2 ].xyz( 0, 0, 0 ); };

	//! Constructs custom triangle.
	Triangle							( const Vec3& v1, const Vec3& v2, const Vec3& v3 ) { vertices[0] = v1, vertices[1] = v2, vertices[2] = v3; };

	//! Transform triangle by matrix.
	void Transform						( const Mat4& m );

	/*! \brief Project primitive to axis.

	 *	\param[out]	a	First project point.
	 *	\param[out] b	Second project point.
	 *	\param[in] axis	Axis.
	 */
	virtual void Projection				( float& a, float& b, const Vec3& axis ) const;
};





/*! \class AABB xfx_primitives.h "math/xfx_primitives.h"
 *	\brief Axis-aligned bounding box.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class AABB : public Primitive
{
	Vec3		mMin;
	Vec3		mMax;

	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	//! Constructs empty AABB.
	AABB								( ) : mMin ( 0.0f, 0.0f, 0.0f ), mMax ( 0.0f, 0.0f, 0.0f ) { };

	//! Constructs AABB from two vertices.
	AABB								( const Vec3& v0, const Vec3& v1 ) { From ( v0, v1 ); };

	//! Constructs AABB from Triangle.
	AABB								(const Triangle& tri) {From (tri);};

	//! Reconstructs AABB from two vertices.
	void				From			(const Vec3& v0, const Vec3& v1);

	//! Reconstructs AABB from Triangle.
	void				From			(const Triangle& tri);

	//! Get min point.
	const Vec3&			Min				() const {return mMin;};

	//! Get max point.
	const Vec3&			Max				() const {return mMax;};

	//! Get center.
	Vec3				Center			() const {return (mMax + mMin) * 0.5f;};

	//! Get size.
	Vec3				Size			() const {return mMax - mMin;};

	//! Get random point inside.
	Vec3				RandomPoint		() const;

	//! Shift AABB by vector.
	void				Shift			(const Vec3& v) {mMin += v; mMax += v;};

	//! Enlarge AABB by triangle.
	void				Enlarge			(const Triangle& tri);

	//! Enlarge AABB by point.
	void				Enlarge			(const Vec3& point);

	//! Enlarge AABB by size.
	void				EnlargeSize		(const Vec3& ofs) {mMin -= ofs; mMax += ofs;};

	//! Check whether point inside AABB
	bool				IsInside		( const Vec3& point ) const { return mMin < point && point < mMax; };

	/*! \brief Project primitive to axis.

	 *	\param[out]	a	First project point.
	 *	\param[out] b	Second project point.
	 *	\param[in] axis	Axis.
	 */
	virtual void		Projection		(float& a, float& b, const Vec3& axis) const;

	//! Returns null AABB.
	static AABB			Null			( )
	{
		AABB res; 
		res.mMin.xyz( 
			std::numeric_limits< float >::max( ), 
			std::numeric_limits< float >::max( ), 
			std::numeric_limits< float >::max( ) 
		); 
		res.mMax.xyz(
			-std::numeric_limits< float >::max( ),
			-std::numeric_limits< float >::max( ),
			-std::numeric_limits< float >::max( ) 
		); 
		return res;
	};
};




/*! \class OBB xfx_primitives.h "math/xfx_primitives.h"
 *	\brief Oriented bounding box.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class OBB : public Primitive
{
	Vec3		mLocation;
	Vec3		mE1;
	Vec3		mE2;
	Vec3		mE3;

	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	//! Constructs empty OBB.
	OBB							( ) : mLocation( 0.0f ), mE1( 0.0f ), mE2( 0.0f ), mE3( 0.0f ) { };
	
	//! Constructs OBB from location and three edge vectors.
	OBB							( const Vec3& loc, const Vec3& e1, const Vec3& e2, const Vec3& e3 ) : mLocation( loc ), mE1( e1 ), mE2( e2 ), mE3( e3 ) { };

	//! Constructs OBB from two vertices.
	OBB							( const Vec3& v0, const Vec3& v1 ) { From ( v0, v1 ); };

	//! Constructs OBB from triangle.
	OBB							( const Triangle& tri ) { From( tri ); };

	//! Constructs OBB from AABB.
	OBB							( const AABB& aabb ) { From( aabb ); };

	//! Reconstructs OBB from two vertice.
	void From					( const Vec3& v0, const Vec3& v1 );

	//! Reconstructs OBB from triangle.
	void From					( const Triangle& tri );

	//! Reconstructs OBB from AABB.
	void From					( const AABB& aabb );

	//! Return closest AABB.
	AABB EnlargeToAABB			( ) const { AABB res( mE1, mE2 ); res.Enlarge( mE3 ); res.Enlarge( mE1 + mE2 ); res.Enlarge( mE1 + mE3 ); res.Enlarge( mE2 + mE3 ); res.Enlarge( mE1 + mE2 + mE3 ); res.Shift( mLocation ); return res; };

	//! Get center.
	Vec3 Center					() const {return mLocation + (mE1 + mE2 + mE3) * 0.5;};

	//! Get size * size.
	float SizeSq				() const {return (mE1 + mE2 + mE3).LenSq ();};

	//! Get size.
	float Size					() const {return (mE1 + mE2 + mE3).Len ();};

	//! Get/Set origin.
	Vec3& Location				() {return mLocation;};

	//! Get origin.
	const Vec3& Location		() const {return mLocation;};

	//! Get first edge.
	const Vec3& E1				() const {return mE1;};

	//! Get second edge.
	const Vec3& E2				() const {return mE2;};

	//! Get third edge.
	const Vec3& E3				() const {return mE3;};

	//! Transform OBB by matrix.
	void Transform				(const Mat4& m);

	/*! \brief Project primitive to axis.

	 *	\param[out]	a	First project point.
	 *	\param[out] b	Second project point.
	 *	\param[in] axis	Axis.
	 */
	virtual void Projection		(float& a, float& b, const Vec3& axis) const;
};




/*! \class Plane xfx_primitives.h "math/xfx_primitives.h"
 *	\brief %Plane.
 *	\ingroup MathGroup
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Plane : public Primitive
{
	float		mA;
	float		mB;
	float		mC;
	float		mD;

	friend class boost::serialization::access;

	//
	//! Serialization.
	//

	// Method is implemented in Serialization library.
	template< class _Archive >
	void serialize( _Archive& ar, const unsigned int version );

public:
	//! Constructs uninitialized plane.
	Plane														( ) { };

	//! Constructs custom plane.
	Plane														( const float& a, const float& b, const float& c, const float& d ) : mA( a ), mB( b ), mC( c ), mD( d ) { };

	//! Create plane from A, B, C, D values.
	void						ABCD							( const float& a, const float& b, const float& c, const float& d ) { mA = a; mB = b; mC = c; mD = d; };

	void						SetA							( float a ) { mA = a; };
	void						SetB							( float b ) { mB = b; };
	void						SetC							( float c ) { mC = c; };
	void						SetD							( float d ) { mD = d; };

	const float&				A								( ) const { return mA; };
	const float&				B								( ) const { return mB; };
	const float&				C								( ) const { return mC; };
	const float&				D								( ) const { return mD; };

	//! Construct plane from point and normal.
	void						From							( const Vec3& origin, const Vec3& normal );

	//! Return non-normalized normal.
	Vec3						Normal							( ) const { return Vec3( mA, mB, mC ); };

	/*! \brief Transform Plane by matrix.

		Transform acts in the following way. Three points on plane are transformed
		by matrix, and then new plane is created from transformed points. This
		method perfectly deals with situations when matrix contains scale and
		even different scales for different axises.

		\param[in] m	Transformation matrix.
	*/
	void						Transform						( const Mat4& m );

	/*! \brief Project primitive to axis.

	 *	\param[out]	a	First project point.
	 *	\param[out] b	Second project point.
	 *	\param[in] axis	Axis.
	 */
	virtual void				Projection						( float& a, float& b, const Vec3& axis ) const;
};



//Intersection functions
bool TestIntersection	(const Primitive& p1, const Primitive& p2, const Vec3& axis);

bool TestIntersection	(const Triangle& p1, const Triangle& p2);
bool TestIntersection	(const AABB& p1, const AABB& p2);
bool TestIntersection	(const OBB& p1, const OBB& p2);
bool TestIntersection	(const AABB& p1, const Triangle& p2);
bool TestIntersection	(const AABB& p1, const OBB& p2);

//Ray intersection
bool TestIntersection	(float& t, Vec3& norm, const Triangle& p, const Ray& r, const float& max_t = FLT_MAX);
bool TestIntersection	(float& t, Vec3& norm, const AABB& p, const Ray& r, const float& max_t = FLT_MAX);
bool TestIntersection	(float& t, Vec3& norm, const Sphere& p, const Ray& r, const float& max_t = FLT_MAX);
bool TestIntersection	(float& t, Vec3& norm, const Plane& p, const Ray& r, const float& max_t = FLT_MAX);



}; //namespace Primitives



_XFX_END

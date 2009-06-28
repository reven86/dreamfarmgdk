//  File xfx_aabbtree.h                                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#include "xfx_primitives.h"

/*!	\file xfx_aabbtree.h 
 *	\brief AABB collision tree.
 */

_XFX_BEGIN







/*!	\defgroup MathGroup Math classes.
 */

/*! \class AABBTree xfx_aabbtree.h "math/xfx_aabbtree.h"
 *	\brief AABB collision tree.
 *	\ingroup MathGroup
 *	
 *	Collision representation for geometric objects.
 *	AABBTree is collision tree where, for all nodes,
 *	all triangles of geometry are divided into two sets,
 *	based on length of longest axis of entire AABB for node.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class AABBTree : boost::noncopyable
{
	struct Node
	{
		Primitives::AABB					box;

		boost::scoped_array< Primitives::Triangle > triangles; //usually only one triangle
		unsigned							triangles_count;

		boost::scoped_ptr< Node >			positive;
		boost::scoped_ptr< Node >			negative;

		Node								( ) : triangles_count( 0 ) { };

		bool IsLeaf							( ) const
		{
			_ASSERTE ( ( !positive.get( ) && !negative.get( ) ) || ( positive.get( ) && negative.get( ) ) );
			return !positive.get( );
		}
	};

	boost::scoped_ptr< Node >				mRootNodePtr;

	static Primitives::AABB					msEmptyAABB;

public:
	//! Constructs an empty tree.
	AABBTree																	( ) : mRootNodePtr( NULL ) {};

	//! Destructor.
	~AABBTree																	( ) { };

	//! Get root AABB.
	const Primitives::AABB&						RootAABB						( ) const { return mRootNodePtr ? mRootNodePtr->box : msEmptyAABB; };

	//! Recursive build AABB tree from triangle input iterators.
	template< class _InputIterator >
	void										Build							( _InputIterator begin, _InputIterator end )
	{
		if( begin != end )
		{
			mRootNodePtr.reset( new Node );
			RecursiveBuild( mRootNodePtr.get (), begin, end );
		}
		else
			mRootNodePtr.reset( NULL );
	};

	//! Debug draw AABB tree.
	void										DebugDraw						( const Mat4& matr ) const { RecursiveDebugDraw( matr, mRootNodePtr.get( ), 0 ); };

	/*! \brief Test intersection with primitive.
	 *
	 *	\param[in] p		Primitive.
	 *	\param[in] deep		Maximum recursion deep.
	 *
	 *	\note If deep == 0 then intersection will be tested only for root AABB.
	 *	\note The function xfx::Primitives::TestPrimitive ( Primitives::Triangle, _Primitive ) must be implemented.
	 *	\todo Provide functor instead or Primitives::TestPrimitive.
	 *
	 *	\return \b true if intersection found.
	 */
	template< class _Primitive >
	bool										TestIntersection				( const _Primitive& p, unsigned deep = -1 ) const { return mRootNodePtr ? RecursiveTestIntersection( mRootNodePtr.get( ), p, deep ) : false; };
	
	/*! \brief Test intersection with another AABBTree.
	 *
	 *	\param[in] tree		Another AABB tree.
	 *	\param[in] matr		Matrix that transform other tree coordinate system into this tree coordinate system.
	 *	\param[in] deep		Recursive deep.
	 *
	 *	\note If deep == 0 then intersection will be tested only for root AABB.
	 *
	 *	\return \b true if intersection found.
	 */
	bool										TestIntersection				( const AABBTree& tree, const Mat4& matr, unsigned deep = -1 ) const;

	/*! \brief Test intersection with Ray.
	 *
	 *	\param[out]	t			Result ray distance.
	 *	\param[out]	normal		Result normal.
	 *	\param[in]	ray			Test ray.
	 *	\param[in]	max_t		Maximal allowed ray distance.
	 *
	 *	\return \b true if intersection found.
	 */
	bool										TestIntersection				( float& t, Vec3& normal, const Ray& ray, const float& max_t = FLT_MAX ) const { PROFILE( __FUNCTION__, "Math" ); return mRootNodePtr ? t = max_t, RecursiveTestIntersection( mRootNodePtr.get( ), t, normal, ray ) : false; };

private:
	bool										IsPositiveTriangle				( const Primitives::Triangle& t, const Primitives::AABB& b, int indofmaxboxdim ) const;

	template< class _InputIterator >
	void										RecursiveBuild					( Node * pnode, _InputIterator begin, _InputIterator end );

	void										RecursiveDebugDraw				( const Mat4& matr, Node * node, int level ) const;

	template< class _Primitive >
	bool										RecursiveTestIntersection		( const Node * node, const _Primitive& p, unsigned deep = -1 ) const;

	bool										RecursiveTestIntersection		( const Node * my, const Node * outside, const Mat4& m, unsigned deep = -1 ) const;
	bool										RecursiveTestIntersection		( const Node * node, float& t, Vec3& normal, const Ray& ray ) const;
};





//
// Template methods
//

template< class _Primitive >
bool AABBTree::RecursiveTestIntersection( const Node * node, const _Primitive& p, unsigned deep ) const
{
	_ASSERTE( node );

	if( node->IsLeaf( ) )
	{
		bool ( *pfn ) ( const Primitives::Triangle&, const _Primitive& );
		pfn = &Primitives::TestIntersection;

		return std::find_if (
			node->triangles.get( ),
			node->triangles.get( ) + node->triangles_count,
			boost::bind( pfn, _1, boost::cref( p ) )
			) != node->triangles.get( ) + node->triangles_count;
	}
	else if( Primitives::TestIntersection( node->box, p ) )
		return( deep == 0 ) ||
			RecursiveTestIntersection( node->negative.get( ), p, deep - 1 ) ||
			RecursiveTestIntersection( node->positive.get( ), p, deep - 1 );

	return false;
}



template< class _InputIterator >
void AABBTree::RecursiveBuild( Node * node, _InputIterator begin, _InputIterator end )
{
	if( boost::next( begin ) == end )
	{
		node->triangles_count = 1;
		node->triangles.reset( new Primitives::Triangle[ 1 ] );
		node->triangles[ 0 ] = *begin;

		node->box.From( *begin );
	}
	else
	{
		node->box.From( *begin );

		void ( Primitives::AABB::*pfn ) ( const Primitives::Triangle& );
		pfn = &Primitives::AABB::Enlarge;

		std::for_each (
			boost::next( begin ),
			end,
			boost::bind( pfn, boost::ref( node->box ), _1 )
			);

		int indofmaxboxdim = 0;
		const Vec3& size( node->box.Size( ) );
		float maxboxdim = size[ 0 ];

		for( int i = 1; i < 3; i++ )
			if( size[ i ] > maxboxdim )
			{
				maxboxdim = size[ i ];
				indofmaxboxdim = i;
			}

		_InputIterator endneg( begin );
		_InputIterator begpos( end );
		_InputIterator maxtriangle( begin );

		while( endneg != begpos )
			if( IsPositiveTriangle( *endneg, node->box, indofmaxboxdim ) )
				std::swap( *endneg, *--begpos );
			else
				endneg++;

		//! Warning! In some rarely cases one of sets can be empty.
		//! So mark this node as leaf
		if( ( begin == endneg ) || ( begpos == end ) )
		{
			node->triangles_count = std::distance( begin, end );
			node->triangles.reset( new Primitives::Triangle[ node->triangles_count ] );

			std::copy( begin, end, node->triangles.get( ) );
		}
		else
		{
			node->negative.reset( new Node );
			node->positive.reset( new Node );

			RecursiveBuild( node->negative.get( ), begin, endneg );
			RecursiveBuild( node->positive.get( ), begpos, end );
		}
	}
}




_XFX_END

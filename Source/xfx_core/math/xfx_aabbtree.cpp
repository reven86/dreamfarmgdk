//  File xfx_aabbtree.cpp                                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_aabbtree.h"

_XFX_BEGIN







//
// AABBTree
//

Primitives::AABB	AABBTree::msEmptyAABB;

bool AABBTree::IsPositiveTriangle( const Primitives::Triangle& t, const Primitives::AABB& b, int indofmaxboxdim ) const
{
	float c0 = t.vertices[0][indofmaxboxdim];
	float c1 = t.vertices[1][indofmaxboxdim];
	float c2 = t.vertices[2][indofmaxboxdim];

	float maxc = std::max (std::max (c0, c1), c2);
	float minc = std::min (std::min (c0, c1), c2);

	float boxmiddle = b.Max ()[indofmaxboxdim] + b.Min ()[indofmaxboxdim];
	float trimiddle = maxc + minc;

	return trimiddle > boxmiddle;
}

bool AABBTree::TestIntersection( const AABBTree& tree, const Mat4& m, unsigned deep ) const
{
	PROFILE( __FUNCTION__, "Math" );

	if (mRootNodePtr && tree.mRootNodePtr)
		return RecursiveTestIntersection (mRootNodePtr.get (), tree.mRootNodePtr.get (), m, deep);

	return false;
}

bool AABBTree::RecursiveTestIntersection (const Node * my, const Node * outside, const Mat4& m, unsigned deep) const
{
	_ASSERTE (my);
	_ASSERTE (outside);

	Primitives::OBB obb (outside->box);
	obb.Transform (m);

	if (!Primitives::TestIntersection (my->box, obb))
		return false;

	if (my->IsLeaf ())
	{
		Mat4 m_inv (m.GetInversedAffine ());

		BOOST_FOREACH( const Primitives::Triangle& it, my->triangles )
		{
			Primitives::Triangle tri( it );
			tri.Transform( m_inv );

			if( RecursiveTestIntersection( outside, tri, deep - 1 ) )
				return true;
		}
	}
	else
	{
		if (outside->IsLeaf ())
		{
			BOOST_FOREACH( const Primitives::Triangle& it, my->triangles )
			{
				Primitives::Triangle tri( it );
				tri.Transform (m);

				if (RecursiveTestIntersection (my, tri, deep - 1))
					return true;
			}
		}
		else
		{
			if (deep == 0)
				return true;

			if (my->box.Size ().LenSq () > outside->box.Size ().LenSq ())
				return RecursiveTestIntersection (my->negative.get (), outside, m, deep - 1) ||
					RecursiveTestIntersection (my->positive.get (), outside, m, deep - 1);
			else
				return RecursiveTestIntersection (my, outside->negative.get (), m, deep - 1) ||
					RecursiveTestIntersection (my, outside->positive.get (), m, deep - 1);
		}
	}

	return false;
}

bool AABBTree::RecursiveTestIntersection (const Node * node, float& t, Vec3& normal, const Ray& ray) const
{
	_ASSERTE (node);

	if (node->IsLeaf ())
	{
		float curt = FLT_MAX;
		Vec3 curnorm;

		bool res = false;
		BOOST_FOREACH( const Primitives::Triangle& it, node->triangles )
		{
			if (Primitives::TestIntersection (curt, curnorm, it, ray, t))
			{
				if( curt < t )
				{
#pragma omp critical
					if( curt < t )
					{
						t = curt;
						normal = curnorm;
						res = true;
					}
				}
			}
		}

		return res;
	}
	else
	{
		float t1;
		Vec3 n;

		if (Primitives::TestIntersection (t1, n, node->box, ray, t))
		{
			bool is1, is2;

#pragma omp parallel sections
			{
#pragma omp section
				is1 = RecursiveTestIntersection( node->negative.get( ), t, normal, ray );

#pragma omp section
				is2 = RecursiveTestIntersection( node->positive.get( ), t, normal, ray );
			}

			return is1 || is2;
		}
	}

	return false;
}




_XFX_END
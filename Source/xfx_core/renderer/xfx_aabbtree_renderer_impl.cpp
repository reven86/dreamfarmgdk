//  File xfx_aabbtree_renderer_impl.cpp                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_renderer.h"
#include "xfx_renderer_cvars.h"



_XFX_BEGIN




//
// AABBTree renderer-specific methods
//

void AABBTree::RecursiveDebugDraw( const Mat4& matr, Node * node, int level ) const
{
	if( node )
	{
		if( level == r_debug_aabbtree->AsInt( ) )
		{
			Renderer::Instance( ).DrawTools( ).PushDrawBox( node->box, node->IsLeaf( ) ? 0xffffff00 : 0xff00ff00, matr );
		}

		if( !node->IsLeaf( ) )
		{
			RecursiveDebugDraw( matr, node->negative.get( ), level + 1 );
			RecursiveDebugDraw( matr, node->positive.get( ), level + 1 );
		}
	}
}


_XFX_END
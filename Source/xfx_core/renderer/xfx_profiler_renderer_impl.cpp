//  File xfx_profiler_renderer_impl.cpp                                       |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#include "xfx.h"
#include "xfx_renderer.h"
#include "xfx_viewport.h"
#include "xfx_font.h"
#include "xfx_shader.h"




_XFX_BEGIN






//
// ProfilingManager renderer-specific methods
//

void ProfilingManager::DrawPerformanceInfo( const Font& fnt, const boost::shared_ptr< const Shader >& white_shd ) const
{
	// draw objects
	LONGLONG total_obj_time;
	total_obj_time = 0;

	total_obj_time = std::accumulate(
		mProfileObjectsResult.begin( ),
		mProfileObjectsResult.end( ),
		total_obj_time,
		boost::bind( 
			std::plus< LONGLONG >( ),
			boost::bind( &LARGE_INTEGER::QuadPart, boost::bind( &ProfileInfo::second, _2 ) ),
			_1 )
		);

	float cur_x = 100.0f;
	float cur_y = 100.0f;
	float bar_w = 200.0f;
	float bar_h	= static_cast< float >( fnt.CharHeight( ) ) + 8.0f;
	float ms = gGetApplication( ).GetTimer( ).GetMSPF100( ) * 0.1f * 0.01f;

	BOOST_FOREACH( const ProfileInfo& pi, mProfileObjectsResult )
	{
		float percent = total_obj_time == 0 ? 0 : static_cast< float >( pi.second.QuadPart * 1000 / total_obj_time ) * 0.1f;

		if( percent < 3.0f )
			continue;

		Renderer::Instance( ).GetDrawTools( ).PushDraw2DSprite(
			cur_x + bar_w * percent * 0.01f * 0.5f, cur_y + bar_h * 0.5f - 4.0f,
			bar_w * percent * 0.01f,
			bar_h,
			0, 0xff008000, white_shd );

		Renderer::Instance( ).GetDrawTools( ).PushDraw2DText(
			fnt,
			cur_x, cur_y,
			1.0f, 1.0f,
			xfx::fromMBCS( boost::str( xfx::StringFormat( "%s: %.1fms %.1f%%" ) % pi.first % ( percent * ms ) % percent ) )
			);

		cur_y += bar_h + 8.0f;
	}

	// draw groups
	LONGLONG total_group_time;
	total_group_time = 0;

	total_group_time = std::accumulate(
		mProfileGroupsResult.begin( ),
		mProfileGroupsResult.end( ),
		total_group_time,
		boost::bind( 
			std::plus< LONGLONG >( ),
			boost::bind( &LARGE_INTEGER::QuadPart, boost::bind( &ProfileInfo::second, _2 ) ),
			_1 )
		);

	cur_x = 500.0f;
	cur_y = 100.0f;
	bar_w = 200.0f;
	bar_h = static_cast< float >( fnt.CharHeight( ) ) + 8.0f;

	BOOST_FOREACH( const ProfileInfo& pi, mProfileGroupsResult )
	{
		float percent = total_group_time == 0 ? 0 : static_cast< float >( pi.second.QuadPart * 1000 / total_group_time ) * 0.1f;

		if( percent < 3.0f )
			continue;

		Renderer::Instance( ).GetDrawTools( ).PushDraw2DSprite(
			cur_x + bar_w * percent * 0.01f * 0.5f, cur_y + bar_h * 0.5f - 4.0f,
			bar_w * percent * 0.01f,
			bar_h,
			0, 0xff008000, white_shd );

		Renderer::Instance( ).GetDrawTools( ).PushDraw2DText(
			fnt,
			cur_x, cur_y,
			1.0f, 1.0f,
			xfx::fromMBCS( boost::str( xfx::StringFormat( "%s: %.1fms %.1f%%" ) % pi.first % ( percent * ms ) % percent ) )
			);

		cur_y += bar_h + 8.0f;
	}
}


_XFX_END
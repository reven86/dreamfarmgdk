#import xfx module (package is exposed in xfx_python library)
import xfx

# declare general Application class
class Application( xfx.Application ):

	instance = None

	def __init__( self ):
		Application.instance = self
		super( Application, self ).__init__( )
	
	def init_engine( self, command_line, wnd_class, wnd_caption ):
		super( Application, self ).init_engine( command_line, wnd_class, wnd_caption )
		
		xfx.Renderer.instance( ).create_device( self.h_wnd )		
		xfx.FileSystem.instance( ).add_search_path( "data\\" )
		
		self._sprite_shader = xfx.Shader.cache( ).register( "dfg_sheep.shader" )
		self._sprite_shader_params = xfx.ShaderParams( )
		self._timer = xfx.Timer( )
		
	def shutdown( self ):
		xfx.Renderer.instance( ).shutdown( )
		super( Application, self ).shutdown( )
		
	def update( self ):
		if not self.h_wnd:
			return
		
		self._timer.update( )
		sec = 0.0001 * self._timer.micro_seconds100( )
		self._sprite_shader_params.set_value( "xfx_DiffuseTex_time", sec )
		xfx.Viewport.instance( ).render( )
		
	def render( self ):
		xfx.Renderer.instance( ).get_draw_tools( ).push_draw_2d_sprite(
			256.0, 256.0, 128.0, 128.0, 0, xfx.ARGB( 255, 255, 255, 255 ), self._sprite_shader, self._sprite_shader_params
			)
	
Application( )
Application.instance.init_engine( "", u"Tutorial4WndClass", u"Tutorial 4 - Python Bindings" )
Application.instance.main_loop( )
Application.instance.shutdown( )
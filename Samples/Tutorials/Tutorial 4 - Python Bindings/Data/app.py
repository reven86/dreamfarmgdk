import sys

#try to import PyDev module
try:
	import pydevd
	pydevd.settrace( stdoutToServer = True, stderrToServer = True )
except:
	print "Can't import PyDev module: ", sys.exc_info()

#import xfx module (package is exposed in xfx_python library)
import xfx



def failed( hr ):
	"""Small function to check whether HRESULT is failed."""
	return int( hr ) < 0

def succeeded( hr ):
	"""Small function to check whether HRESULT is succeeded."""
	return not failed( hr )



class Application( xfx.Application ):
	"""
	Application - base class for entire application. Provides methods for
	initialization, deinitialization, update-render loop.
	"""

	instance = None

	def __init__( self ):
		Application.instance = self
		super( Application, self ).__init__()

	def init_engine( self, command_line, wnd_class, wnd_caption ):
		hr = super( Application, self ).init_engine( command_line, wnd_class, wnd_caption )
		if failed( hr ):
			return hr;

		hr = xfx.Renderer.instance.create_device( self.h_wnd )
		if failed( hr ):
			return hr;

		xfx.FileSystem.instance.add_search_path( "data\\" )

		self._sprite_shader = xfx.Shader.cache.register( "dfg_sheep.shader" )
		self._sprite_shader_params = xfx.ShaderParams()

		self._frames = 0

		return hr

	def shutdown( self ):
		xfx.Renderer.instance.shutdown()
		super( Application, self ).shutdown()

	def update( self ):
		if not self.h_wnd:
			return

		self.timer.update()
		sec = 0.0001 * self.timer.micro_seconds100()
		self._sprite_shader_params.set_value( "xfx_DiffuseTex_time", sec )
		xfx.Viewport.instance.render()

		self._frames += 1
		if ( self.timer.micro_seconds100() - self.timer.mspf100 ) // 10000 != self.timer.micro_seconds100() // 10000:
			print self._frames
			self._frames = 0

	def render( self ):
		xfx.Renderer.instance.draw_tools.push_draw_2d_sprite( 
			256.0, 256.0, 128.0, 128.0, 0, xfx.ARGB( 255, 255, 255, 255 ), self._sprite_shader, self._sprite_shader_params
			)

#print sys.path

Application()
Application.instance.init_engine( "", u"Tutorial4WndClass", u"Tutorial 4 - Python Bindings" )
Application.instance.main_loop()
Application.instance.shutdown()

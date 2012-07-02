
import os
import re
import sys

sys.path.append( os.path.join( '..', '..', 'third_party', 'py++-1.0.0' ) )
sys.path.append( os.path.join( '..', '..', 'third_party', 'pygccxml-1.0.0' ) )

from pyplusplus import module_builder, code_creators, messages
from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pygccxml.declarations.matchers import access_type_matcher_t 
from pyplusplus.decl_wrappers.properties import name_based_recognizer_t

class PropertyRecognizer( name_based_recognizer_t ):
	def create_read_only_property( self, fget ):
		if fget.name == 'ToString':
			return None
		return name_based_recognizer_t.create_read_only_property( self, fget )

def camel_convert(name):
    if name == "D3DPP":
        return "d3dpp"
    if name == "D3DCaps":
        return "d3dcaps"
    if name == "gGetApplication":
		return "application"
		
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [ r"xfx_py_bindings.h", ]
                                      , gccxml_path=r"../../third_party/gccxml-0.9/bin/gccxml.exe" 
                                      , working_directory=r"."
                                      , include_paths=[u'../../third_party/boost_1_44_0', u'../../third_party/dxsdk-9.0c-sum04/include', '.', ]
                                      , define_symbols=['WIN32=1', 'NDEBUG=1', '_LIB=1', 'UNICODE=1', '_UNICODE=1',
														'_HAS_TR1=0', 'BOOST_RANGE_ENABLE_CONCEPT_ASSERT=0',
														'BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES=1' ] )
														
mb.BOOST_PYTHON_MAX_ARITY = 16

mb.namespace( 'xfx' ).include( )
mb.calldefs( access_type_matcher_t( 'protected' ) ).exclude()
mb.variables( access_type_matcher_t( 'protected' ) ).exclude()
mb.calldefs( access_type_matcher_t( 'private' ) ).exclude()
mb.variables( access_type_matcher_t( 'private' ) ).exclude()
mb.decls( lambda x: x.name.startswith( '_' ) ).exclude()
mb.variables( type = '::boost::shared_ptr<xfx::Var>' ).include()

mb.decls( lambda x: x.name.startswith( '_D3DTRANSFORMSTATETYPE' ) ).include( )
mb.decls( lambda x: x.name.startswith( '_D3DCUBEMAP_FACES' ) ).include( )
mb.decls( lambda x: x.name.startswith( '_D3DFORMAT' ) ).include( )
mb.decls( lambda x: x.name.startswith( '_D3DPRIMITIVETYPE' ) ).include( )
mb.decls( lambda x: x.name.startswith( '_D3DVIEWPORT' ) ).include( )
mb.decls( lambda x: x.name.startswith( '_D3DPRESENT_PARAMETERS' ) ).include( )
mb.decls( lambda x: x.name.startswith( '_D3DCMPFUNC' ) ).include( )

mb.class_( "ITexture" ).member_functions( ).exclude( )
mb.class_( "Texture" ).member_functions( "GetD3DTex" ).exclude( )
mb.class_( "CubemapTexture" ).member_functions( "GetD3DTex" ).exclude( )
mb.class_( "RenderedTexture" ).member_functions( "GetD3DTex" ).exclude( )
mb.class_( "RenderedTexture" ).member_functions( "GetD3DSurface" ).exclude( )
mb.class_( "DepthStencil" ).member_functions( "GetD3DSurface" ).exclude( )
mb.class_( "Renderer" ).member_functions( lambda x: x.name in ( 'pD3D', 'pD3DDevice' ) ).exclude( )
mb.class_( "Renderer" ).member_functions( "GetDrawTools" ).exclude( )
mb.class_( "Renderer" ).add_property( "draw_tools", fget = mb.class_( "Renderer" ).member_function( "GetDrawTools", return_type = '::xfx::DrawTools &' ) )
mb.class_( "Application" ).member_function( "GetTimer" ).exclude( )
mb.class_( "Application" ).add_property( "timer", fget = mb.class_( "Application" ).member_function( "GetTimer" ) )
mb.member_functions( "Instance" ).exclude( )

mb.classes( ).add_properties( recognizer = PropertyRecognizer( ), exclude_accessors=True )

for c in mb.member_functions( ):
	c.rename( camel_convert( c.alias ) if c.alias != 'ToString' else '__repr__' )
for c in mb.free_functions( ):
	c.rename( camel_convert( c.alias ) )
for c in mb.variables( ):
	c.rename( camel_convert( c.alias ) )
for c in mb.classes( ):
	for p in c.properties:
		p.set_name( camel_convert( p.name ) )
		
mb.class_( "Caches" ).member_functions( "ClearCallbacks" ).exclude( )
mb.class_( "Cmd" ).member_function( "RegisterCmd" ).exclude( )
mb.class_( "Cmd" ).member_function( "RegisterVar" ).use_overload_macro = True #exclude( )
mb.member_functions( "ParseAt" ).exclude()
mb.member_operators( "operator[]" ).exclude( )

#mb.free_function( "gToConsole" ).include( )
#mb.free_function( "gMess" ).include( )
#mb.free_function( "gWarn" ).include( )
#mb.free_function( "gError" ).include( )
#mb.free_function( "gCError" ).include( )

for c in mb.namespace( "xfx" ).classes( ):
	c.add_registration_code( ''.join( ( 'bp::register_ptr_to_python< boost::shared_ptr< ', c.demangled, ' const > >( );' ) ), False )
	c.add_registration_code( ''.join( ( 'bp::implicitly_convertible< boost::shared_ptr< ', c.demangled, ' >, boost::shared_ptr< ', c.demangled, ' const > >( );' ) ), False )

mb.class_( "DrawTools" ).member_functions( "PushDraw2DSprite" ).rename( "push_draw_2d_sprite" )
mb.class_( "DrawTools" ).member_functions( "PushDraw2DText" ).rename( "push_draw_2d_text" )
mb.class_( "DrawTools" ).member_functions( "PushDraw3DSprite" ).rename( "push_draw_3d_sprite" )
mb.class_( "DrawTools" ).member_functions( "PushDraw3DText" ).rename( "push_draw_3d_text" )

mb.member_functions( "Instance" ).call_policies = call_policies.return_value_policy( call_policies.reference_existing_object )
mb.free_function( "gGetApplication" ).call_policies = call_policies.return_value_policy( call_policies.reference_existing_object )
mb.class_( "BooksManager" ).member_function( "GetBook" ).call_policies = call_policies.return_internal_reference( )
mb.class_( "Shader" ).member_function( "GetTextureInfoOnStage" ).call_policies = call_policies.return_internal_reference( )
mb.class_( "Pack" ).member_function( "FindFile" ).call_policies = call_policies.return_internal_reference( )
mb.class_( "Pack" ).member_function( "GetFirstFile" ).call_policies = call_policies.return_internal_reference( )
mb.class_( "Pack" ).member_function( "GetLastFile" ).call_policies = call_policies.return_internal_reference( )
mb.class_( "ParticleSystem" ).member_functions( "GetSubSystem" ).call_policies = call_policies.return_internal_reference( )
mb.class_( "ParticleSystem" ).member_functions( "GetParticle" ).call_policies = call_policies.return_internal_reference( )
mb.class_( "BaseParticleSystem" ).member_function( "EmitterTransformation", return_type = '::xfx::Mat4 &' ).call_policies = call_policies.return_internal_reference( )
mb.class_( "Quaternion" ).member_function( "FromAxisAngle" ).call_policies = call_policies.return_self( )
mb.class_( "Mat3" ).member_functions( return_type = '::xfx::Mat3 &' ).call_policies = call_policies.return_self( )
mb.class_( "Mat4" ).member_functions( return_type = '::xfx::Mat4 &' ).call_policies = call_policies.return_self( )
mb.class_( "Euler" ).member_functions( lambda x: x.name in ( "FromMat4", "FromVec3", "FromQuaternion" ) ).call_policies = call_policies.return_self( )
mb.class_( "Renderer" ).member_function( "GetFrameStatistics", return_type = '::xfx::Renderer::FrameStatistics &' ).call_policies = call_policies.return_internal_reference( )
mb.class_( "Renderer" ).member_function( "GetDrawTools", return_type = '::xfx::DrawTools &' ).call_policies = call_policies.return_internal_reference( )
mb.class_( "MeshState" ).member_function( "GetShaderParams", return_type = '::xfx::ShaderParams &' ).call_policies = call_policies.return_internal_reference( )
mb.class_( "Application" ).member_function( "HWnd" ).call_policies = call_policies.return_value_policy( call_policies.return_opaque_pointer )
mb.class_( "Application" ).member_function( "GetTimer" ).call_policies = call_policies.return_internal_reference( )
mb.member_functions( "Cache", arg_types = [] ).call_policies = call_policies.return_value_policy( call_policies.reference_existing_object )

for c in mb.classes( lambda x: x.name.startswith( "Singleton" ) ):
	c.add_static_property( 'instance', fget = c.member_function( "Instance" ) )

for c in mb.classes( ):
	for f in c.member_functions( "Cache", arg_types = [], allow_empty = True ):
		f.exclude( )
		c.add_static_property( 'cache', fget = f )

#mb.class_( "FileIterator_t" ).opaque = True
#mb.class_( "TextureInfo" ).opaque = True
mb.class_( lambda x: x.name.startswith( 'HWND' ) ).opaque = True

mb.class_( "FileSystem" ).member_function( "GetFileSize" ).add_transformation( ft.output( "len" ) )
mb.class_( "Pack" ).member_function( "GetFileSize" ).add_transformation( ft.output( "len" ) )
mb.class_( "Pack" ).member_function( "GetFileOffset" ).add_transformation( ft.output( "ofs" ) )
mb.class_( "Viewport" ).member_functions( "Project" ).add_transformation( ft.output( "x" ), ft.output( "y" ) )
mb.class_( "AABBTree" ).member_functions( "TestIntersection", arg_types = [ 'float &', None, None, None ] ).add_transformation( ft.output( "t" ), ft.output( "normal" ), alias = 'test_intersection_ray' )
mb.class_( "Texture" ).member_functions( "GetFrameTexCoords" ).add_transformation( ft.output( "u0" ), ft.output( "v0" ), ft.output( "u1" ), ft.output( "v1" ) )
mb.member_functions( "Projection" ).add_transformation( ft.output( "a" ), ft.output( "b" ) )
mb.member_functions( "ParseEnvelope" ).add_transformation( ft.output( "env" ), ft.inout( "pos" ) )
mb.free_function( "ParseAngleVariable" ).add_transformation( ft.output( "var" ), ft.inout( "pos" ) )
mb.free_function( "ParseBigAngleVariable" ).add_transformation( ft.output( "var" ), ft.inout( "pos" ) )

l = ( 'Triangle', 'AABB', 'Sphere', 'Plane' )
for s in l:
	f = mb.namespace( "Primitives" ).free_function( name = "TestIntersection", arg_types = [ 'float &', None, ''.join( ( '::xfx::Primitives::', s, ' const &' ) ), None, None ] )
	f.add_transformation( ft.output( "t" ), ft.output( "norm" ), alias = ''.join( ( f.alias, '_', camel_convert( s ) ) ) )

l = ( 'Mat4', 'Vec4' )
for s in l:
	f = mb.class_( "ShaderParams" ).member_function( name = "GetValue", arg_types = [ None, ''.join( ( '::xfx::', s, ' &' ) ) ] )
	f.add_transformation( ft.output( "value" ), alias = ''.join( ( f.alias, '_', camel_convert( s ) ) ) )

l = ( 'int', 'float' )
for s in l:
	f = mb.class_( "ShaderParams" ).member_function( name = "GetValue", arg_types = [ None, ''.join( ( s, ' &' ) ) ] )
	f.add_transformation( ft.output( "value" ), alias = ''.join( ( f.alias, '_', camel_convert( s ) ) ) )

f = mb.class_( "ShaderParams" ).member_function( name = "GetValue", arg_types = [ None, '::boost::shared_ptr<xfx::ITexture const> &' ] )
f.add_transformation( ft.output( "value" ), alias = ''.join( ( f.alias, '_texture' ) ) )

#mb.print_declarations( mb.namespace( lambda x: x.name.startswith( 'xfx' ) ) )

mb.namespace( "Primitives" ).class_( "Triangle" ).disable_warnings( messages.W1027 ) #Py++ will generate class wrapper - array member variable
mb.class_( "Shader" ).disable_warnings( messages.W1023 ) #Py++ will generate class wrapper - few functions that should be redefined in class wrapper
mb.class_( "ScriptResource" ).disable_warnings( messages.W1023 ) #Py++ will generate class wrapper - few functions that should be redefined in class wrapper
mb.class_( "Font" ).disable_warnings( messages.W1023 ) #Py++ will generate class wrapper - few functions that should be redefined in class wrapper
mb.class_( "ParticleSystem" ).disable_warnings( messages.W1023 ) #Py++ will generate class wrapper - few functions that should be redefined in class wrapper
mb.class_( "MessagesBook" ).disable_warnings( messages.W1023 ) #Py++ will generate class wrapper - few functions that should be redefined in class wrapper
mb.class_( "Mesh" ).disable_warnings( messages.W1023 ) #Py++ will generate class wrapper - few functions that should be redefined in class wrapper
mb.class_( "DrawTools" ).class_( "DrawChunkInfo" ).disable_warnings( messages.W1024 ) #Py++ will generate class wrapper - bit field member variable
mb.class_( "ITexture" ).member_functions( lambda x: x.name in ( "GetTextureMatrix", "GetTransformation", ) ).disable_warnings( messages.W1049 ) #This method could not be overriden in Python
mb.namespace( "boost" ).decls( lambda x: x.name.startswith( "shared_ptr" ) ).disable_warnings( messages.W1040 ) #The declaration is unexposed
mb.namespace( "boost" ).decls( lambda x: x.name.startswith( "function" ) ).disable_warnings( messages.W1040 ) #The declaration is unexposed
mb.namespace( "tuples" ).decls( lambda x: x.name.startswith( "tuple" ) ).disable_warnings( messages.W1040 ) #The declaration is unexposed
mb.namespace( "stdext" ).decls( lambda x: x.name.startswith( "hash_map" ) ).disable_warnings( messages.W1040 ) #The declaration is unexposed

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='xfx' )
mb.code_creator.adopt_creator( code_creators.include_t( 'xfx.h' ), 0 )

#Writing code to file.
mb.split_module( 'generated' )

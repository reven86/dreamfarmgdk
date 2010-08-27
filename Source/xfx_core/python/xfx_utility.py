
import os
import re
from pyplusplus import module_builder
from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pygccxml.declarations.matchers import access_type_matcher_t 

def camel_convert(name):
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


mb.namespace( lambda x: x.name.startswith( 'xfx' ) ).include( )
mb.class_( lambda x: x.name.startswith( 'HINSTANCE' ) ).include( )
mb.class_( lambda x: x.name.startswith( 'HWND' ) ).include( )
mb.calldefs( access_type_matcher_t( 'protected' ) ).exclude()
mb.calldefs( access_type_matcher_t( 'private' ) ).exclude()
mb.variables( access_type_matcher_t( 'protected' ) ).exclude()
mb.variables( access_type_matcher_t( 'private' ) ).exclude()
mb.member_functions( lambda x: x.name.startswith( '_' ) ).exclude()
mb.variables( lambda x: x.name.startswith( '_' ) ).exclude()

mb.classes( ).add_properties( exclude_accessors=True )

for c in mb.member_functions( ):
	c.rename( camel_convert( c.alias ) )
for c in mb.free_functions( ):
	c.rename( camel_convert( c.alias ) )
for c in mb.variables( ):
	c.rename( camel_convert( c.alias ) )
for c in mb.classes( ):
	for p in c.properties:
		p.set_name( camel_convert( p.name ) )
		#c_same = c.member_functions( p.name, allow_empty = True )
		#if c_same:
		#	c_same.exclude( )

mb.class_( "Caches" ).member_functions( "ClearCallbacks" ).exclude( )
mb.class_( "Cmd" ).member_function( "RegisterCmd" ).exclude( )
mb.class_( "Cmd" ).member_function( "RegisterVar" ).exclude( )
mb.member_functions( "ParseAt" ).exclude()

#mb.free_function( "gToConsole" ).include( )
#mb.free_function( "gMess" ).include( )
#mb.free_function( "gWarn" ).include( )
#mb.free_function( "gError" ).include( )
#mb.free_function( "gCError" ).include( )

mb.member_functions( "Instance" ).call_policies = call_policies.return_internal_reference( )
mb.free_function( "gGetApplication" ).call_policies = call_policies.return_internal_reference( )
mb.class_( "BooksManager" ).member_function( "GetBook" ).call_policies = call_policies.return_internal_reference( )

mb.class_( "FileSystem" ).member_function( "GetFileSize" ).add_transformation( ft.output( "len" ) )
mb.class_( "Pack" ).member_function( "GetFileSize" ).add_transformation( ft.output( "len" ) )
mb.class_( "Pack" ).member_function( "GetFileOffset" ).add_transformation( ft.output( "ofs" ) )
mb.class_( "Viewport" ).member_functions( "Project" ).add_transformation( ft.output( "x" ), ft.output( "y" ) )
mb.class_( "ShaderParams" ).member_functions( "GetValue" ).add_transformation( ft.output( "value" ) )
mb.member_functions( "ParseEnvelope" ).add_transformation( ft.inout( "env" ), ft.inout( "pos" ) )
mb.member_functions( "Projection" ).add_transformation( ft.output( "a" ), ft.output( "b" ) )

mb.class_( "FileIterator_t" ).opaque = True
mb.class_( "TextureInfo" ).opaque = True

#mb.print_declarations( mb.namespace( lambda x: x.name.startswith( 'xfx' ) ) )

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='xfx' )
mb.code_creator.replace_included_headers( ["xfx.h"], False )

#Writing code to file.
mb.split_module( 'generated' )

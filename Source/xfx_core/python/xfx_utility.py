
import os
from pyplusplus import module_builder
from pyplusplus import function_transformers as ft
from pygccxml.declarations.matchers import access_type_matcher_t 

def camel_convert(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [ r"xfx_py_utility.h", r"xfx_py_main.h", ]
                                      , gccxml_path=r"../../third_party/gccxml-0.9/bin/gccxml.exe" 
                                      , working_directory=r"."
                                      , include_paths=[u'../../third_party/boost_1_44_0', u'../../third_party/dxsdk-9.0c-sum04/include', '.']
                                      , define_symbols=['WIN32=1', 'NDEBUG=1', '_LIB=1', 'UNICODE=1', '_UNICODE=1',
														'_HAS_TR1=0', 'BOOST_RANGE_ENABLE_CONCEPT_ASSERT=0',
														'BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES=1' ] )


mb.namespace( lambda x: x.name.startswith( 'xfx' ) ).include( )
#mb.classes( lambda x: x.name.startswith( 'basic_string' ) ).include( )
#mb.classes( lambda x: x.name.startswith( 'shared_ptr' ) ).include( )
#mb.classes( lambda x: x.name.startswith( 'scoped_ptr' ) ).include( )
#mb.classes( lambda x: x.name.startswith( 'Singleton' ) ).exclude( )
mb.calldefs( access_type_matcher_t( 'protected' ) ).exclude()
mb.calldefs( access_type_matcher_t( 'private' ) ).exclude()
mb.variables( access_type_matcher_t( 'protected' ) ).exclude()
mb.variables( access_type_matcher_t( 'private' ) ).exclude()
mb.member_functions( lambda x: x.name.startswith( '_' ) ).exclude()
mb.variables( lambda x: x.name.startswith( '_' ) ).exclude()

mb.class_( "Caches" ).member_functions( "ClearCallbacks" ).exclude( )
mb.classes( lambda x: x.name.startswith( 'Singleton' ) ).member_functions( lambda x: x.name == "Instance" ).exclude( )
#mb.free_function( "gToConsole" ).include( )
#mb.free_function( "gMess" ).include( )
#mb.free_function( "gWarn" ).include( )
#mb.free_function( "gError" ).include( )
#mb.free_function( "gCError" ).include( )

mb.member_functions( lambda x: x.name == 'ParseAt' ).exclude()

mb.class_( "FileSystem" ).member_function( "GetFileSize" ).add_transformation( ft.output( "len" ) )
mb.class_( "Pack" ).member_function( "GetFileSize" ).add_transformation( ft.output( "len" ) )
mb.class_( "Pack" ).member_function( "GetFileOffset" ).add_transformation( ft.output( "ofs" ) )

mb.print_declarations( mb.namespace( lambda x: x.name.startswith( 'xfx' ) ) )
mb.namespace( lambda x: x.name.startswith( 'xfx' ) ).include_files.insert( 0, "xfx.h" )

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='xfx' )
mb.code_creator.replace_included_headers( ["xfx.h"], False )

#Writing code to file.
mb.split_module( 'generated' )

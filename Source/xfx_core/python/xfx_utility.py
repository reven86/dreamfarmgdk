
import os
from pyplusplus import module_builder
from pygccxml.declarations.matchers import access_type_matcher_t 

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [ r"xfx_py_utility.h" ]
                                      , gccxml_path=r"../../third_party/gccxml-0.9/bin/gccxml.exe" 
                                      , working_directory=r"."
                                      , include_paths=[u'../../third_party/boost_1_44_0', '.']
                                      , define_symbols=['WIN32=1', 'NDEBUG=1', '_LIB=1', 'UNICODE=1', '_UNICODE=1'] )


mb.namespace( lambda x: x.name.startswith( 'xfx' ) ).include( )
#mb.classes( lambda x: x.name.startswith( 'basic_string' ) ).include( )
mb.classes( lambda x: x.name.startswith( 'Singleton' ) ).exclude( )
mb.calldefs( access_type_matcher_t( 'protected' ) ).exclude()
mb.calldefs( access_type_matcher_t( 'private' ) ).exclude()
mb.variables( access_type_matcher_t( 'protected' ) ).exclude()
mb.variables( access_type_matcher_t( 'private' ) ).exclude()
mb.member_functions( lambda x: x.name.startswith( '_' ) ).exclude()
mb.variables( lambda x: x.name.startswith( '_' ) ).exclude()

mb.print_declarations( mb.namespace( lambda x: x.name.startswith( 'xfx' ) ) )

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='xfx' )

#Writing code to file.
mb.split_module( './generated' )

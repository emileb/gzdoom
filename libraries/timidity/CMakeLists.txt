cmake_minimum_required( VERSION 3.1.0 )

use_fast_math()
require_stricmp()

if( ZD_CMAKE_COMPILER_IS_GNUC_COMPATIBLE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-unused-function -Wno-unused-variable" )
endif()

include_directories( timidity )

file( GLOB HEADER_FILES
	timidity/*.h
	)
add_library( timidity STATIC
	common.cpp
	instrum.cpp
	instrum_dls.cpp
	instrum_font.cpp
	instrum_sf2.cpp
	mix.cpp
	playmidi.cpp
	resample.cpp
	timidity.cpp
	)
target_link_libraries( timidity )

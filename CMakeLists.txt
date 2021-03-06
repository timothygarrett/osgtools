#	osgtools Library
#	CMake file for the osgtools library
#
#	Tim Garrett (garrettt@iastate.edu)
#	2015.07.22

cmake_minimum_required(VERSION 3.2)

# Main project
project( osgtools )

# Set the output directories depending on the architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	# Compiling for x64	
	set(OSGTOOLS_ARCH_32 FALSE)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/x64)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/x64)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/x64)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/x64)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/x64)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/x64)
else ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	# Compiling for x32
	set(OSGTOOLS_ARCH_32 TRUE)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/x86)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/x86)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/x86)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/x86)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/x86)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/x86)
endif ( CMAKE_SIZEOF_VOID_P EQUAL 8 )

# Copy output files to bin
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Set the runtime path
set( RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Add the projects
add_subdirectory(src)

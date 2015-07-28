#	osgtools Library
#	CMake file for the osgtools library
#
#	Tim Garrett (garrettt@iastate.edu)
#	2015.07.28


# Include subdirectories
add_subdirectory(osgtools)

# Optional GTest unit testing
option(BUILD_TESTS "Build GTest unit tests" OFF)
if(BUILD_TESTS)

	# Download and build GTest
	configure_file(test_osgtools/gtest-CMakeLists.txt ${CMAKE_BINARY_DIR}/gtest-download/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-download )
	execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-download )
	
	# Force use of the shared DLL
	SET(gtest_force_shared_crt on CACHE BOOL "Use shared (DLL) run-time lib even when Google test is built as static lib.") 

	# Add the GTest Projects
	add_subdirectory(${CMAKE_BINARY_DIR}/gtest-src
		${CMAKE_BINARY_DIR}/gtest-build
		EXCLUDE_FROM_ALL)
	
	enable_testing()
	# Add the GTest include directory
	target_include_directories(gtest INTERFACE "${CMAKE_BINARY_DIR}/gtest-src/include" )
	
	# Add the test project subdirectory
	add_subdirectory(test_osgtools)
	  
ENDIF(BUILD_TESTS)
# test_osgtools CMAKE file
# Provides testing of the osgtools library
#
# Tim Garrett (garrettt@iastate.edu)
# 2015.07.28

project(test_osgtools)

# Set the project files
set(TEST_SRC
	main.h
	main.cpp
	OneTest.h
	OneTest.cpp
)


# Add the files
add_executable(test_osgtools
	${TEST_SRC}
)

# Add libraries
target_link_libraries(test_osgtools 
	gtest
	gtest_main
)

# Set the properties
set_target_properties (test_osgtools PROPERTIES DEBUG_POSTFIX "_d")

# Add the dependencies
add_dependencies(test_osgtools
	gtest
	gtest_main
	osgtools
)
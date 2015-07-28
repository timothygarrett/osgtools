/*
	main.h
	Entry point for osgtools test project
	
	Tim Garrett (garrettt@iastate.edu)
	2015.07.28
*/

#include "main.h"



GTEST_API_ int main( int argc, char* argv[] )
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
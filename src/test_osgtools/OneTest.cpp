/*
	main.cpp
	Sample unit test
	
	Tim Garrett (garrettt@iastate.edu)
	2015.05.15
*/

#include "OneTest.h"

TEST_F(OneTest, Vector) {
	ASSERT_TRUE(_nodes.size() == 1) << "Non zero node vector";
}



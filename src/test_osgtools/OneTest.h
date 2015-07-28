/*
	OneTest.h
	Google test class template
	
	Tim Garrett (garrettt@iastate.edu)
	2015.05.15
*/

#pragma once

// Local
#include <vector>

// GTest
#include <gtest/gtest.h>


class OneTest : public ::testing::Test {
	protected:
		std::vector<int> _nodes;
		
public:
	OneTest() {
		_nodes.push_back(1);
	}

	virtual ~OneTest() {}
};

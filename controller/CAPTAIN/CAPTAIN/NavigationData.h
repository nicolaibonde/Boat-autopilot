#pragma once
#include "Coordinate.h"
#include <vector>

class NavigationData
{
public:
	NavigationData(const std::vector<Coordinate> line); 
	~NavigationData();

public:
	std::vector<Coordinate> Line_; //Consider if vector is the best list for this
	int Ete_;
};


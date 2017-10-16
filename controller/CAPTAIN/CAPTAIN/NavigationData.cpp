#include "NavigationData.h"


NavigationData::NavigationData(const std::vector<Coordinate> line)
{
	Line_ = line;
	Ete_ = 0; //Does this make sense?
}

NavigationData::~NavigationData()
{
}

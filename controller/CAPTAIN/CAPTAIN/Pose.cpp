#include "Pose.h"


Pose::Pose(const Coordinate coordinate, const double orientation) : Coordinate_(coordinate)
{
	Orientation_ = orientation;
}

Pose::~Pose()
{
}

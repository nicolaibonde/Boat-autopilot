#pragma once
#include "Coordinate.h"

class Pose
{
public:
	Pose(const Coordinate coordinate, const double orientation);
	~Pose();

public:
	Coordinate Coordinate_;
	double Orientation_;
};


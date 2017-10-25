#pragma once
#include "Coordinate.h"

class TargetPosition
{
public:
	TargetPosition(); //Only used to signify an empty object, used for errors
	TargetPosition(const Coordinate);
	~TargetPosition();
	Coordinate get_target_postion() const;

private:
	Coordinate target_position_;
};


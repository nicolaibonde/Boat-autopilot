#include "TargetPosition.h"


TargetPosition::TargetPosition() : target_position_(Coordinate(-1, -1))
{
}

TargetPosition::TargetPosition(const Coordinate coordinate) : target_position_(coordinate)
{
}

TargetPosition::~TargetPosition()
{
}

Coordinate TargetPosition::get_target_postion() const
{
	return target_position_;
}

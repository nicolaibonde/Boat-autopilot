#include "TargetPosition.h"


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

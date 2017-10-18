#pragma once
#include "TaskData.h"
#include "Coordinate.h"

class TargetPosition :
	public TaskData
{
public:
	TargetPosition(const Coordinate);
	~TargetPosition();
	Coordinate get_target_postion() const;

private:
	Coordinate target_position_;
};


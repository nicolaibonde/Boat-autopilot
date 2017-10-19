#pragma once
#include "MotorStatus.h"

class IMotorStatusGetter
{
public:
	virtual ~IMotorStatusGetter() {};
	virtual MotorStatus GetStatus() = 0;
};

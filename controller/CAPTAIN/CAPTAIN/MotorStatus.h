#pragma once
#include "IStatus.h"
#include "MotorType.h"

class MotorStatus:
	public IStatus
{
public:
	MotorStatus(const double percentage, MotorType motor_type);
	~MotorStatus();
	std::string GetString() override;

private:
	MotorType motor_type_;
	double percentage_;
};

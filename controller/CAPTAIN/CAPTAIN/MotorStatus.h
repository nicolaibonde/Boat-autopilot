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

private: //comment out this line to give boost access and run tests!!
	MotorType motor_type_;
	double percentage_;
};

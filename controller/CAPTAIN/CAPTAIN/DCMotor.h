#pragma once
#include "ISpeedMotor.h"
#include "IMotorStatusGetter.h"
#include "MotorStatus.h"

class DCMotor :
	public ISpeedMotor, public IMotorStatusGetter
{
public:
	DCMotor();
	~DCMotor();
	void SetSpeed(double speed);
	MotorStatus GetStatus() override;

private:
	double speed_;
};

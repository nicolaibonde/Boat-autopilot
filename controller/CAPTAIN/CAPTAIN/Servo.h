#pragma once
#include "IPositionMotor.h"
#include "IMotorStatusGetter.h"
#include "MotorStatus.h"

class Servo :
	public IPositionMotor, public IMotorStatusGetter
{
public:
	Servo();
	~Servo();
	void SetPosition(const double position) override;
	MotorStatus GetStatus() override;

private:
	double position_;
};

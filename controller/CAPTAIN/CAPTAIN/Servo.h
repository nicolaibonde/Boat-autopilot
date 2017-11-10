#pragma once
#include "IPositionMotor.h"
#include "IMotorStatusGetter.h"
#include "MotorStatus.h"
#include "IGPIO.h"

class Servo :
	public IPositionMotor, public IMotorStatusGetter
{
public:
	Servo(IGPIO & gpio);;
	~Servo() override;
	void SetPosition(const double position) override;
	MotorStatus GetStatus() override;

private:
	unsigned percentageToServo(double percentage) const;

private:
	double position_;
	IGPIO & gpio_;
	unsigned pin_;
};

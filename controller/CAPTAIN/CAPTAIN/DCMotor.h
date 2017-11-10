#pragma once
#include "ISpeedMotor.h"
#include "IMotorStatusGetter.h"
#include "MotorStatus.h"
#include "IGPIO.h"

class DCMotor :
	public ISpeedMotor, public IMotorStatusGetter
{
public:
	DCMotor(IGPIO & gpio);
	~DCMotor();
	void SetSpeed(const double speed) override;
	MotorStatus GetStatus() override;

private:
	double speed_;
	IGPIO & gpio_;
	unsigned pin_;
	int frequency_;
};

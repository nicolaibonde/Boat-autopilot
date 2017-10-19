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
	void SetSpeed(const double speed);
	MotorStatus GetStatus() override;

//private: //comment out this line to give boost access adn run tests!!
	double speed_;
};

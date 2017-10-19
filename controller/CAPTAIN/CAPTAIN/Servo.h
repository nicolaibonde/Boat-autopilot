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
	void SetPosition(const double position);
	MotorStatus GetStatus() override;

//private: //comment out this line to give boost access adn run tests!!
	double position_;
};

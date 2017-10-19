#include "DCMotor.h"

DCMotor::DCMotor()
{
	speed_ = 0.0; //Don't start the speedmotor when object is created..!
};

DCMotor::~DCMotor()
{
	
};

void DCMotor::SetSpeed(const double speed)
{
	speed_ = speed;
}

MotorStatus DCMotor::GetStatus()
{
	return MotorStatus(speed_, SPEED);
}

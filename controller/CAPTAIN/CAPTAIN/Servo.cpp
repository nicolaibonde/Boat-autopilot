#include "Servo.h"

Servo::Servo()
{
	position_ = 0.0; //Don't start the speedmotor when object is created..!
};

Servo::~Servo()
{

};

void Servo::SetPosition(const double position)
{
	position_ = position;
}

MotorStatus Servo::GetStatus()
{
	return MotorStatus(position_, POSITION);
}

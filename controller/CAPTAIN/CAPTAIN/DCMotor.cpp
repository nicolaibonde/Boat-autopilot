//#include <Python.h>
#include "DCMotor.h"
#include <iostream>

DCMotor::DCMotor(IGPIO& gpio) : gpio_(gpio)
{
	pin_ = 9; //Maybe choose a better one later
	frequency_ = 30000; //Setting frequency to 30kHz for noiseless operation

	//mode is 1 for output and 0 for input
	gpio_.GpioSetMode(pin_, 1);

	//setting the pwm frequency
	frequency_ = gpio_.GpioSetPWMfrequency(pin_, frequency_);

	//Setting the default speed to 0% so that the boat doesent move in the beginning
	DCMotor::SetSpeed(0); 
}

DCMotor::~DCMotor()
{
	
};

void DCMotor::SetSpeed(const double speed)
{

	if (speed < 0)
	{
		speed_ = 0;
	}
	else if (speed > 100)
	{
		speed_ = 100;
	}
	else
	{
		//Save position set
		speed_ = speed;
	}

	gpio_.GpioPWM(pin_, static_cast<unsigned>(speed_));
}

MotorStatus DCMotor::GetStatus()
{
	return MotorStatus(speed_, SPEED);
}

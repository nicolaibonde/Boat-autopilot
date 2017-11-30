//#include <Python.h>
#include "DCMotor.h"
#include <iostream>

DCMotor::DCMotor(IGPIO& gpio) : gpio_(gpio)
{
	pin_ = 18; //Dedicated hardware pwm pin on Raspberry pi 3b. called pwm0, and is the physical pin 12
	frequency_ = 30000; //Setting frequency to 30kHz for noiseless operation

	//mode is 1 for output and 0 for input
	gpio_.GpioSetMode(pin_, 1);

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

	gpio_.GpioHardwarePWM(pin_,frequency_, static_cast<unsigned>(speed_* 10000));
}

MotorStatus DCMotor::GetStatus()
{
	return MotorStatus(speed_, SPEED);
}

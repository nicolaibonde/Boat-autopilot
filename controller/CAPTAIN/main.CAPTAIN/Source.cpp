#include "CAPTAIN/PiGpio.h"
#include "CAPTAIN/Servo.h"
#include "CAPTAIN/DCMotor.h"
#include <chrono>
#include <thread>


int main(int argc, char* argv[])
{
	PiGpio servoGPIO = PiGpio();
	Servo rudder = Servo(servoGPIO);

	PiGpio motorGPIO = PiGpio();
	DCMotor thruster = DCMotor(motorGPIO);

	float i = 0.0;
	
	while (i < 48.0)
	{
		if (i > 50.0)
		{
			i = 0;
		}

		rudder.SetPosition(i);
		thruster.SetSpeed(i);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		i += 0.1;
	}
}

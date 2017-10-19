#include <iostream>
#include <string>
#include "MotorStatus.h"
#include "DCMotor.h"
#include "Servo.h"

int main()
{
	DCMotor DCM1;
	Servo Ser1;
	MotorStatus MS1 = DCM1.GetStatus();
	MotorStatus MS2 = Ser1.GetStatus();

	std::string MS_string = MS1.GetString();
	std::cout << MS_string << std::endl;
	std::string test_string( R"({"items_":[{"color_","progress-bar-warning","data_":0.0,"title_":DC Motor,"unit_":"%"}],"title_":"Motor")");
	if (MS_string.compare(test_string))
	{
		std::cout << "Equal!" << std::endl;
	}
}

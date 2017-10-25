//#include <Python.h>
#include "DCMotor.h"

DCMotor::DCMotor()
{
	speed_ = 0.0; //Don't start the speedmotor when object is created..!
};

DCMotor::~DCMotor()
{
	
};

void DCMotor::SetSpeed(double speed)
{
	speed_ = speed;

	//Py_Initialize();
	//PyRun_SimpleString("import sys");
	//PyRun_SimpleString('sys.path.append("path/to/my/module/")');
	//Py_Finalize();
}

MotorStatus DCMotor::GetStatus()
{
	return MotorStatus(speed_, SPEED);
}

//#include <Python.h>
#include "Servo.h"

Servo::Servo()
{
	position_ = 0.0; //Don't start the speedmotor when object is created..!

	//Py_Initialize();
	//PyRun_SimpleString("import sys");
	//PyRun_SimpleString('sys.path.append("path/to/my/module/")');
	//Py_Finalize();
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


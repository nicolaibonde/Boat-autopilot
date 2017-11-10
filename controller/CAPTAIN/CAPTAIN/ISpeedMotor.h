#pragma once

class ISpeedMotor
{
public:
	virtual ~ISpeedMotor(){};
	virtual void SetSpeed(const double speed) {};
};


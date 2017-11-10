#pragma once

class IPositionMotor
{
public:
	virtual ~IPositionMotor(){};
	virtual void SetPosition(const double position) = 0;
};


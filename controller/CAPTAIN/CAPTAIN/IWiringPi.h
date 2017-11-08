#pragma once
#pragma once

class IWiringPi
{
public:
	virtual ~IWiringPi() {};
	virtual int WiringPiSetup() = 0;
	virtual int SoftPwmCreate(int pin, int initialValue, int pwmRange) = 0;
	virtual void SoftPwmWrite(int pin, int value) = 0;

};


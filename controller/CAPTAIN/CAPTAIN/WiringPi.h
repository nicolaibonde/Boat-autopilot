#pragma once
#include "IWiringPi.h"
#include <wiringPi.h>
#include <softPwm.h>

class WiringPi :
	public IWiringPi
{
public:
	WiringPi();
	~WiringPi();
	int WiringPiSetup() override;
	int SoftPwmCreate(int pin, int initialValue, int pwmRange) override;
	void SoftPwmWrite(int pin, int value) override;
};


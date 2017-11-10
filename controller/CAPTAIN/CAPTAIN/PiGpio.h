#pragma once
#include "IGPIO.h"
#include <pigpio.h>

class PiGpio :
	public IGPIO
{
public:
	PiGpio();
	~PiGpio() override;

	int GpioSetMode(unsigned gpio, unsigned mode) override;
	int GpioPWM(unsigned user_gpio, unsigned dutycycle) override;
	int GpioSetPWMfrequency(unsigned user_gpio, unsigned frequency) override;
	int GpioServo(unsigned user_gpio, unsigned pulsewidth) override;
};
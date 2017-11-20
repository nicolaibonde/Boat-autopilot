#pragma once

class IGPIO
{
public:
	virtual ~IGPIO() {};
	virtual int GpioSetMode(unsigned gpio, unsigned mode) = 0;
	virtual int GpioPWM(unsigned user_gpio, unsigned dutycycle) = 0;
	virtual int GpioSetPWMfrequency(unsigned user_gpio, unsigned frequency) = 0;
	virtual int GpioServo(unsigned user_gpio, unsigned pulsewidth) = 0;
};


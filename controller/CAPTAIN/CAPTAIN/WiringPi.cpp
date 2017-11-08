#include "WiringPi.h"



WiringPi::~WiringPi()
{
}

int WiringPi::WiringPiSetup()
{
	return wiringPiSetup();
}

int WiringPi::SoftPwmCreate(int pin, int initialValue, int pwmRange)
{
	return softPwmCreate(pin, initialValue, pwmRange);
}

void WiringPi::SoftPwmWrite(int pin, int value)
{
	return softPwmWrite(pin, value);
}

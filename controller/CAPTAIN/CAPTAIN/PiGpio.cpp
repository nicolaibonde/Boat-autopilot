#include "PiGpio.h"

PiGpio::PiGpio()
{
	gpioInitialise();
}

PiGpio::~PiGpio()
{
	gpioTerminate();
}

int PiGpio::GpioSetMode(unsigned gpio, unsigned mode)
{
	return gpioSetMode(gpio, mode);
}

int PiGpio::GpioPWM(unsigned user_gpio, unsigned dutycycle)
{
	gpioPWM(user_gpio, dutycycle);
}

int PiGpio::GpioSetPWMfrequency(unsigned user_gpio, unsigned frequency)
{
	gpioSetPWMfrequency(user_gpio, frequency);
}

int PiGpio::GpioServo(unsigned user_gpio, unsigned pulsewidth)
{
	gpioServo(user_gpio, pulsewidth);
}

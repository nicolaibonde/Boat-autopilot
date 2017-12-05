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
	return gpioPWM(user_gpio, dutycycle);
}

int PiGpio::GpioSetPWMfrequency(unsigned user_gpio, unsigned frequency)
{
	return gpioSetPWMfrequency(user_gpio, frequency);
}

int PiGpio::GpioServo(unsigned user_gpio, unsigned pulsewidth)
{
	return gpioServo(user_gpio, pulsewidth);
}

int PiGpio::GpioHardwarePWM(unsigned user_gpio, unsigned frequency, unsigned dutycycle) {
	return gpioHardwarePWM(user_gpio, frequency, dutycycle);

}

uint32_t PiGpio::GpioTick()
{
	return gpioTick();
}
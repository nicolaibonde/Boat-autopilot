#pragma once
#include "IGPIO.h"
#include <pigpio.h>

/**
 * \brief An implementation of the IGPIO interface for the Raspberry Pi
 * \note This is just a wrapper for the pigpio library
 * \see http://abyz.me.uk/rpi/pigpio/cif.html
 */
class PiGpio :
	public IGPIO
{
public:
	/**
	 * \brief Default constructor
	 */
	PiGpio();
	/**
	 * \brief Destructor
	 */
	~PiGpio() override;

	int GpioSetMode(unsigned gpio, unsigned mode) override;
	int GpioPWM(unsigned user_gpio, unsigned dutycycle) override;
	int GpioSetPWMfrequency(unsigned user_gpio, unsigned frequency) override;
	int GpioServo(unsigned user_gpio, unsigned pulsewidth) override;
	int GpioHardwarePWM(unsigned user_gpio, unsigned frequency, unsigned dutycycle) override;
};
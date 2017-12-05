#pragma once

/**
 * \brief An interface towards hardware general purpose I/O
 * \details The interface is create to use pigpio, so that we are able to 
 * test the functionallity of classes depending on it with out having GPIO hardware.
 * \note This interface could be expanded to add more GPIO functionallity, 
 * but for this first implementation of CAPTAIN, only pwm and servo were needed
 */
class IGPIO
{
public:
	/**
	 * \brief destructor
	 */
	virtual ~IGPIO() {};
	/**
	 * \brief Set if what mode a given gpio pin should be 
	 * \param gpio what pin should the mode be set on
	 * \param mode what mode should be set; 1: output, 0: input
	 * \return return Returns 0 if OK
	 */
	virtual int GpioSetMode(unsigned gpio, unsigned mode) = 0;
	/**
	 * \brief Sets PWM on the GPIO, dutycycle between 0 (off) and range (fully on). Range defaults to 255. 
	 * \param user_gpio what gpio should the dutycycle be set on
	 * \param dutycycle what dutycycle should be set
	 * \return Returns 0 if OK
	 */
	virtual int GpioPWM(unsigned user_gpio, unsigned dutycycle) = 0;
	/**
	 * \brief Sets the pwm frequency of a gpio pin
	 * \param user_gpio What gpio should the frequency be set on
	 * \param frequency What frequency should be set
	 * \return Returns the numerically closest frequency if OK
	 */
	virtual int GpioSetPWMfrequency(unsigned user_gpio, unsigned frequency) = 0;
	/**
	 * \brief Starts servo pulses on the GPIO, 0 (off), 500 (most anti-clockwise) to 2500 (most clockwise). 
	 * \param user_gpio What gpio pin should the pulsewidth be set on
	 * \param pulsewidth What pulse width should be set
	 * \return Returns 0 if OK
	 */
	virtual int GpioServo(unsigned user_gpio, unsigned pulsewidth) = 0;

	/**
	 * \brief Use the dedicated hardware gpios for the pwm
	 * \param user_gpio What gpio should the hardware pwm run on
	 * \param frequency What frequency should it run 0 25000000
	 * \param dutycycle What dutycycle between 0 and 1000000
	 * \return 0 if OK
	 */
	virtual int GpioHardwarePWM(unsigned user_gpio, unsigned frequency, unsigned dutycycle) = 0;

	/**
	* \brief Get the number of microseconds since system boot
	* \return Time since boot in microseconds (uint32_t format)
	*/
	virtual uint32_t GpioTicks() = 0;

};


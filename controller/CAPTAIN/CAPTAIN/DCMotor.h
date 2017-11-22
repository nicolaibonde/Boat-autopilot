#pragma once
#include "ISpeedMotor.h"
#include "IMotorStatusGetter.h"
#include "MotorStatus.h"
#include "IGPIO.h"

/**
 * \brief describes a DC motor that is controlled via pwm
 * \details A DCMotor is an implementation of the ISpeedMotor interface which describes all motors where you set a speed \n
 * it is also an implementation of the IMotorStatusGetter interface which describes the GetStatus function used for getting the current status of motor.
 * \see ISpeedMotor, IMotorStatusGetter
 */
class DCMotor :
	public ISpeedMotor, public IMotorStatusGetter
{
public:
	/**
	 * \brief Explicit contructor
	 * \details setup the gpio to know the pin and the pwm frequency of the output.
	 * \param gpio A IGPIO is used to set the pwm duty cycle of the motor
	 */
	DCMotor(IGPIO & gpio);

	/**
	 * \brief destructor
	 */
	~DCMotor();
	
	/**
	 * \brief set the speed of the motor using gpio.
	 * \param speed used to set the speed of the motor defined as a percentage from 0 to 100
	 */
	void SetSpeed(const double speed) override;

	/**
	 * \brief get the status object for the motor
	 * \return returns a MotorStatus object which contains the percentage and the motor type here SPEED
	 */
	MotorStatus GetStatus() override;

private:
	/**
	 * \brief keeps track of the current speed
	 */
	double speed_;
	/**
	 * \brief a reference to the gpio
	 */
	IGPIO & gpio_;
	/**
	 * \brief the gpio pin on which is the pwm is outputted
	 */
	unsigned pin_;
	/**
	 * \brief the pwm frequency on the gpio
	 */
	int frequency_;
};

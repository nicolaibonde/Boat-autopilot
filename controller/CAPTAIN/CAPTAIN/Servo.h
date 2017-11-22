#pragma once
#include "IPositionMotor.h"
#include "IMotorStatusGetter.h"
#include "MotorStatus.h"
#include "IGPIO.h"

/**
* \brief describes a servo motor that is controlled via pwm
* \details A Servo is an implementation of the IPositionMotor interface which describes all motors where you set a position \n
* it is also an implementation of the IMotorStatusGetter interface which describes the GetStatus function used for getting the current status of motor.
* \see IPositionMotor, IMotorStatusGetter
*/
class Servo :
	public IPositionMotor, public IMotorStatusGetter
{
public:
	/**
	* \brief Explicit contructor
	* \details setup the gpio to be a servo, 
	* \param gpio A IGPIO is used to set the position of the motor
	*/
	Servo(IGPIO & gpio);;
	/**
	 * \brief destructor
	 */
	~Servo() override;
	/**
	 * \brief Set the position of the servo
	 * \param position The position as a percentage 0-100
	 */
	void SetPosition(const double position) override;
	/**
	* \brief get the status object for the motor
	* \return returns a MotorStatus object which contains the percentage and the motor type here POSITION
	*/
	MotorStatus GetStatus() override;

private:
	/**
	 * \brief Calculate the servo duty-cycle from a percentage 0-100
	 * \param percentage a percentage from 0-100
	 * \return the correct value for the servo, so that 50% is translated to a centered servo.
	 */
	unsigned percentageToServo(double percentage) const;

private:
	/**
	 * \brief the current position of the motor
	 */
	double position_;
	/**
	 * \brief a reference to the gpio
	 */
	IGPIO & gpio_;
	/**
	 * \brief the pin on the gpio where it will output
	 */
	unsigned pin_;
};

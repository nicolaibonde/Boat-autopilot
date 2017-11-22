#pragma once

/**
* \brief An interface that describes a speed motor
* \details A speed motor is a motor where you tell how fast to go.
*/
class ISpeedMotor
{
public:
	/**
	 * \brief destructor
	 */
	virtual ~ISpeedMotor(){};
	/**
	 * \brief Set the speed of the motor
	 * \param speed the speed the motor should go as a percentage from 0-100
	 */
	virtual void SetSpeed(const double speed) {};
};


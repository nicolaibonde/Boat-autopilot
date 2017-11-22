#pragma once

/**
 * \brief An interface that describes a position motor
 * \details A position motor is a motor where you tell it where to be and it goes there.
 * You do not have control of the speed with which it gets there.
 */
class IPositionMotor
{
public:
	/**
	 * \brief destructor
	 */
	virtual ~IPositionMotor(){};
	/**
	 * \brief Set the position of the motor
	 * \param position the positon the motor should go to in a percentage 0-100
	 */
	virtual void SetPosition(const double position) = 0;
};


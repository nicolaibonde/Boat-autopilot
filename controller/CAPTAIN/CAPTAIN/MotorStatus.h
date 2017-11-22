#pragma once
#include "IStatus.h"
#include "MotorType.h"


/**
 * \brief A status object for a motor, it implements the IStatus interface
 */
class MotorStatus:
	public IStatus
{
public:
	/**
	 * \brief Explicit constructor
	 * \param percentage What is the current percentage of the motor ex. the speed or the position as a percentage
	 * \param motor_type What is the motor type of the object creating this MotorStatus object
	 * \see MotorType
	 */
	MotorStatus(const double percentage, MotorType motor_type);
	/**
	 * \brief Destructor
	 */
	~MotorStatus();
	/**
	 * \brief Get status object as a serialized JSON string
	 * \return Returns a JSON object serialized as a string
	 */
	std::string GetString() override;

private:
	/**
	 * \brief defines what type the motor is
	 * \see MotorType
	 */
	MotorType motor_type_;
	/**
	 * \brief What percentage is the motor set to, this could as an example be a position or a speed both defined as a percentage 
	 */
	double percentage_;
};

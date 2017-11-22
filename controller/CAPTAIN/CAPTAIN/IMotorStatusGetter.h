#pragma once
#include "MotorStatus.h"

/**
 * \brief An interface that describes how a motor can return a motor status object
 */
class IMotorStatusGetter
{
public:
	/**
	 * \brief destructor
	 */
	virtual ~IMotorStatusGetter() {};
	/**
	 * \brief get the MotorStatus for the given motor.
	 * \return returns a MotorStatus object which contains a percentage and a MotorType
	 * \see MotorStatus
	 */
	virtual MotorStatus GetStatus() = 0;
};

#pragma once
#include <string>

/**
 * \brief An interface describing a status
 */
class IStatus
{
public:
	/**
	 * \brief Destructor
	 */
	virtual ~IStatus(){};
	/**
	 * \brief get the string with the status of the object that created it.
	 * \return Returns a string containg the status of the object creating it.
	 */
	virtual std::string GetString() = 0;
};


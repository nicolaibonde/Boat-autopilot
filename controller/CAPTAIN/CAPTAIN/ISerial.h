#pragma once
#include <string>


/**
 * \brief An interface for a serial device
 */
class ISerial
{
public:
	/**
	 * \brief Destructor
	 */
	virtual ~ISerial() {};
	/**
	 * \brief Read a line from the serial buffer if there is any
	 * \return Should return the next line in the serial buffer
	 */
	virtual std::string ReadLine() = 0;
	/**
	 * \brief Write a line to the serial buffer
	 */
	virtual void WriteString(std::string) = 0;
};
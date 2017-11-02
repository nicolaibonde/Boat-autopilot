#pragma once
#include <string>


class ISerial
{
public:
	virtual ~ISerial() {};
	virtual std::string ReadLine() = 0;
	virtual void WriteString(std::string) = 0;
};
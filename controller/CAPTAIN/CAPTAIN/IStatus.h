#pragma once
#include <string>

class IStatus
{
public:
	virtual ~IStatus(){};
	virtual std::string GetString() = 0;
};


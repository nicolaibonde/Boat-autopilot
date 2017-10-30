#pragma once
#include <string>

class IAutopilot
{
public:
	virtual ~IAutopilot() {};
	virtual void Run(std::string xte_telegram, int checksum) = 0;
	virtual void Stop() = 0;
	virtual void SetParameters(double p, double i, double d) = 0;
};


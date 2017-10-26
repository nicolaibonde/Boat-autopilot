#pragma once

class ITransmitter
{
public:
	virtual ~ITransmitter() {};
	virtual void TransmitFromNav(std::string const, std::string const) = 0;
};

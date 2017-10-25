#pragma once

class ITransmitter
{
public:
	virtual ~ITransmitter() {};
	virtual void TransmitFromNav(std::string) = 0;
};
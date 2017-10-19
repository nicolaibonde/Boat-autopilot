#pragma once

class ITransmitter
{
public:
	virtual ~ITransmitter() {};
	virtual void TransmitFromNav() {};
};


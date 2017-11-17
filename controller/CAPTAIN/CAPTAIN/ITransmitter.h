#pragma once

class INavigation;

class ITransmitter
{
public:
	virtual ~ITransmitter() {};
	virtual void NavAcquisition(INavigation*) = 0;;
	virtual void TransmitFromNav(std::string const) = 0;
};

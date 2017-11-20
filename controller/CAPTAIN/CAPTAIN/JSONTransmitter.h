#pragma once
#include <string>
#include "IMotorStatusGetter.h"
#include "ITransmitter.h"
#include "NavigationData.h"
#include "IGPS.h"

class INavigation; //Forward declaration

class JSONTransmitter :
	public ITransmitter
{
public:
	JSONTransmitter(IMotorStatusGetter& dc_motor, 
		IMotorStatusGetter& servo, IGPS& gps, const std::string filepath);
	
	~JSONTransmitter();

	void NavAcquisition(INavigation* navigation) override;
	
	void TransmitFromNav(std::string const timestamp) override;

private:
	INavigation* navigation_;
	IMotorStatusGetter& dc_motor_;
	IMotorStatusGetter& servo_;
	std::string filepath_;
	IGPS& gps_;
	std::string formatEte(int ete) const;
	std::string formatPathString(std::vector<Coordinate> path) const;
};

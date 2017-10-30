#pragma once
#include <string>
#include "IMotorStatusGetter.h"
#include "ITransmitter.h"
#include "NavigationData.h"
#include "INavigation.h"
#include "IGPS.h"

class JSONTransmitter :
	public ITransmitter
{
public:
	JSONTransmitter(INavigation& navigation, IMotorStatusGetter& dc_motor, 
		IMotorStatusGetter& servo, IGPS& gps);
	
	~JSONTransmitter();
	
	void TransmitFromNav(std::string const filepath, std::string const timestamp) override;

private:
	INavigation& navigation_;
	IMotorStatusGetter& dc_motor_;
	IMotorStatusGetter& servo_;
	IGPS& gps_;
	std::string formatEte(int ete) const;
	std::string formatPathString(std::vector<Coordinate> path) const;
};

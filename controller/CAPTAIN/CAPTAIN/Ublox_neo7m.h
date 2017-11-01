#pragma once
#include "IGPS.h"
#include "SimpleSerial.h"
#include <thread>

class Ublox_neo7m:
	public IGPS
{
public:
	Ublox_neo7m();
	~Ublox_neo7m();
	Pose GetPose() override;
	GPSStatus GetStatus() override;
	std::thread Setup() override;

private:
	void getGPSData();

	bool checksum(std::string telegram);
	double convertDegreeMinutes2Degrees(std::string degree_minutes) const;

private:
	SimpleSerial serial_;	
	Pose pose_;
	GPSStatus status_;
};

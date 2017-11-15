#pragma once
#include "IGPS.h"
#include <thread>
#include "ISerial.h"

class Ublox_neo7m:
	public IGPS
{
public:
	Ublox_neo7m(ISerial &serial);
	~Ublox_neo7m();
	Pose GetPose() override;
	double GetSpeed() override;
	GPSStatus GetStatus() override;
	std::thread Setup() override;
	void StopThread() override;

private:
	void getGPSData();

	bool checksum(std::string telegram);
	double convertDegreeMinutes2Degrees(std::string degree_minutes) const;
	Pose calculatePose(Coordinate old_coordinate, Coordinate new_coordinate);

private:
	ISerial &serial_;	
	Pose pose_;
	Pose old_pose_;
	double speed_;
	GPSStatus status_;
	bool thread_run_;
};

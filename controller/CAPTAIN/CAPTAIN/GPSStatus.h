#pragma once
#include "IStatus.h"
#include "Pose.h"
class GPSStatus :
	public IStatus
{
public:
	GPSStatus(const double fix, const int satellites, const double hdop, const int fix_timestamp, const Pose pose);
	~GPSStatus();
	std::string GetString() override;

private:
	double fix_;
	int satellites_;
	double hdop_;
	int fix_timestamp_;
	Pose pose_;
};


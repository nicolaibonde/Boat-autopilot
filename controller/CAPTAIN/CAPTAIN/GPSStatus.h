#pragma once
#include "IStatus.h"
#include "Pose.h"

class GPSStatus :
	public IStatus
{
public:
	GPSStatus(const double frequency, const double delay, const Pose pose);
	~GPSStatus();
	std::string GetString() override;

private:
	double frequency_;
	double delay_;
	Pose pose_;
};


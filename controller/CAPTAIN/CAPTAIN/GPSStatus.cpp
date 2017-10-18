#include "GPSStatus.h"


GPSStatus::GPSStatus(const double frequency, const double delay, double, const Pose pose) : pose_(pose)
{
	frequency_ = frequency;
	delay_ = delay;
}

GPSStatus::~GPSStatus()
{
}

std::string GPSStatus::GetString()
{
	throw "GPSStatus::GetString() is not yet implemented!";
}

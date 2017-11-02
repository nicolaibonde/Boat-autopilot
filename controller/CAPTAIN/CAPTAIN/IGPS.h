#pragma once
#include "Pose.h"
#include "GPSStatus.h"
#include <thread>

class IGPS
{
public:
	virtual ~IGPS(){};
	virtual std::thread Setup() = 0;
	virtual Pose GetPose() = 0;
	virtual GPSStatus GetStatus() = 0;
};


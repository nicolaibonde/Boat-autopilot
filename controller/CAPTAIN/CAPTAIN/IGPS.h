#pragma once
#include "Pose.h"
#include "GPSStatus.h"

class IGPS
{
public:
	virtual ~IGPS(){};
	virtual void Setup() = 0;
	virtual Pose GetPose() = 0;
	virtual GPSStatus GetStatus() = 0;
};
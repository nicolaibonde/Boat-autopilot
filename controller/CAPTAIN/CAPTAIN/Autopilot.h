#pragma once
#include "IAutopilot.h"
#include "ISpeedMotor.h"
#include "IPositionMotor.h"
#include "MiniPID.h"
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>


class Autopilot :
	public IAutopilot
{
public:
	Autopilot(IPositionMotor &rudder, ISpeedMotor &thruster);
	~Autopilot();

	void Run(std::string telegram) override;
	void Stop() override;
	void SetParameters(double p, double i, double d) override;

private:
	bool checksum(std::string telegram);

	
private:
	double p_;
	double i_;
	double d_;

	bool running_;
	double max_speed_;
	double current_speed;

	MiniPID rudder_pid_;
	MiniPID thruster_pid_;
	IPositionMotor &rudder_;
	ISpeedMotor &thruster_;
};


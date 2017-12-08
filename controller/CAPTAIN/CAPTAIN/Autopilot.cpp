#include "Autopilot.h"

//Setup the Autopilot with 2 PID's and 2 references to the motors
Autopilot::Autopilot(IPositionMotor& rudder, ISpeedMotor& thruster): rudder_pid_(0, 0, 0), thruster_pid_(1,0,0),
                                                                     rudder_(rudder), thruster_(thruster)
{
	max_speed_ = 10; //in km/h
	p_ = -1; //Invalid value
	i_ = -1; //Invalid value
	d_ = -1; //Invalid value
	thruster_pid_.setOutputLimits(0, 40); //Hardcoding limits, maybe this should be done through the user interface
	rudder_pid_.setOutputLimits(35, 70);  //A servo has diffuculties at the edges ei. 0 and 90 so it is limited

	thruster_pid_.setPID(1,0,0);

}

Autopilot::~Autopilot()
{
	//If the autopilot is destroyed make sure that the boat stops
	Stop();
}


/*$GPAPB, A, A, 0.10, R, N, V, V, 011, M, DEST, 011, M, 011, M * 3C

where:
APB     Autopilot format B
A       Loran - C blink / SNR warning, general warning
A       Loran - C cycle warning
0.10    cross - track error distance
R       steer Right to correct(or L for Left)
N       cross - track error units - nautical miles(K for kilometers)
V       arrival alarm - circle
V       arrival alarm - perpendicular
011, M   magnetic bearing, origin to destination
DEST    destination waypoint ID
011, M   magnetic bearing, present position to destination
011, M   magnetic heading to steer(bearings could represent True as 033, T)*/

//----------------------------------------------------------------------------------\\
	//For CAPTAIN, all bearings are True, NOT magnetic, hence the "T"'s in the telegrams\\
	//----------------------------------------------------------------------------------\\

//See continuePath directly under this function to inspect the NMEA GPAPB command structure
//$GPAPB,A,A,0.042439928,L,K,V,V,29.201387,T,2,281.281750,T,281.281750,T*3C


void Autopilot::Run(std::string telegram)
{
	//Check if any of the pid values are invalid
	if (p_ != -1 && i_ != -1 && d_ != -1) {
		running_ = true;

		//Hold a list of strings split by "," delimiter
		std::vector<std::string> split_telegram;

		//Splitting the telegram on "," delimiter
		boost::split(split_telegram, telegram, boost::is_any_of(","));
		//check if the telegram checks out
		if (checksum(telegram)) {

			//Check if the telegram is a GPAPB ie an autopilot telegram
			if (split_telegram[0].find("APB") != std::string::npos)
			{
				//If so then regulate on the bearing, which is the how far we are from the desired
				 
				double bearing = stod(split_telegram[13]);
				if(split_telegram[4] == "R"){
					bearing = -bearing;
				}
				rudder_.SetPosition(rudder_pid_.getOutput(bearing, 0));
			}
			//Check if the telegram is gpvtg which is a velocity telegram
			if (split_telegram[0].find("VTG") != std::string::npos)
			{
				//regulate on the speed toward the max speed 
				const double speed = stod(split_telegram[7]);
				thruster_.SetSpeed(thruster_pid_.getOutput(speed, max_speed_));
			}
		}
	}
}

void Autopilot::Stop()
{
	running_ = false;

	//Stopping means that the thruster should stop and the rudder should go to a default position
	thruster_.SetSpeed(0);
	rudder_.SetPosition(50);
}

bool Autopilot::checksum(std::string telegram)
{
	//Check if it is a complete telegram
	if (telegram[0] == '$' && telegram[telegram.size() - 3] == '*')
	{
		//extract the checksum hex number
		const std::string expected_checksum_string = telegram.substr(telegram.size() - 2, telegram.size() - 1);

		//Remove $ from beginning and checksum and * from end
		std::string bare_telegram = telegram.substr(1, telegram.size() - 4);

		//Calculate the checksum of the bare_telegram
		int result_checksum = 0;
		for (char& c : bare_telegram) {
			result_checksum = result_checksum ^ c;
		}

		//Convert the int checksum to a hex string 
		std::stringstream checksum_stream;
		checksum_stream << std::uppercase << std::hex << result_checksum;
		const std::string result_checksum_string(checksum_stream.str());

		//Return if the expected checksum is the same as the calculated
		return result_checksum_string == expected_checksum_string;
	}
}

void Autopilot::SetParameters(double p, double i, double d)
{
	//Set the parameters for the rudder pid.
	p_ = p;
	i_ = i;
	d_ = d;
	rudder_pid_.setPID(p_, i_, d_);
}


